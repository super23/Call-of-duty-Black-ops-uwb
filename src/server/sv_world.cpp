#include "sv_world.h"

#include <bgame/bg_public.h>
#include <bgame/bg_local.h>
#include "sv_game.h"
#include <universal/assertive.h>
#include <qcommon/cm_world.h>
#include <universal/com_math_anglevectors.h>
#include <bgame/bg_slidemove.h>
#include <qcommon/cm_test.h>
#include <qcommon/cm_load.h>
#include <qcommon/dobj_management.h>
#include <cgame/cg_world.h>
#include <qcommon/cm_tracebox.h>
#include <bgame/bg_misc.h>
#include <game_mp/g_utils_mp.h>
#include <game_mp/g_main_mp.h>
#include <glass/glass_server.h>
#include <DynEntity/DynEntity_server.h>

static void SnapAngles(float *angles);

unsigned int __cdecl SV_ClipHandleForEntity(const gentity_s *ent)
{
    if ( ent->r.bmodel )
        return ent->s.index.brushmodel;
    else
        return CM_TempBoxModel(ent->r.mins, ent->r.maxs, ent->r.contents);
}

void __cdecl SV_UnlinkEntity(gentity_s *gEnt)
{
    svEntity_s *svEnt = SV_SvEntityForGentity(gEnt);

    gEnt->r.linked = 0;
    CM_UnlinkEntity(svEnt);
}

// Updates abs bounds, PVS clusters, and collision linkage for a server entity.
void SV_LinkEntity(gentity_s *gEnt)
{
    float linkMins[3];
    float linkMaxs[3];
    float rotationAxis[3][3];
    phys_vec3 boundsCenter;
    phys_vec3 boundsHalfExtents;
    phys_vec3 localMins;
    phys_vec3 localMaxs;
    phys_vec3 worldAabbMin;
    phys_vec3 worldAabbMax;
    phys_mat44 entityTransform;

    unsigned int clipHandle;
    int leafCluster;
    int numLeafs;
    unsigned __int16 leafs[128];
    int lastLeaf;
    int axis;
    float boundingRadius;
    int solidWidth;
    int solidHeight;
    int solidZExtent;

    float *origin;
    float *angles;
    svEntity_s *svEnt;
    DObj *serverDObj;

    iassert(gEnt->r.inuse);

    svEnt = SV_SvEntityForGentity(gEnt);

    // Pack entity dimensions into s.solid for client-side prediction (non-bmodel solids).
    if (gEnt->r.bmodel)
    {
        gEnt->s.solid = 0xFFFFFF;
    }
    else if ((gEnt->r.contents & 0x2008001) != 0)
    {
        solidWidth = (int)gEnt->r.maxs[0];
        if (solidWidth < 1)
            solidWidth = 1;
        if (solidWidth > 255)
            solidWidth = 255;

        solidHeight = (int)(float)(-(gEnt->r.mins[2]) + 1.0f);
        if (solidHeight < 1)
            solidHeight = 1;
        if (solidHeight > 255)
            solidHeight = 255;

        solidZExtent = (int)(float)(gEnt->r.maxs[2] + 32.0f);
        if (solidZExtent < 1)
            solidZExtent = 1;
        if (solidZExtent > 255)
            solidZExtent = 255;

        gEnt->s.solid = solidWidth | (solidHeight << 8) | (solidZExtent << 16);
        if (gEnt->s.solid == 0xFFFFFF)
            gEnt->s.solid = 1;
    }
    else
    {
        gEnt->s.solid = 0;
    }

    angles = gEnt->r.currentAngles;
    origin = gEnt->r.currentOrigin;
    nanassertvec3(angles);
    nanassertvec3(origin);
    SnapAngles(angles);

    if (gEnt->r.bmodel)
    {
        Phys_Vec3ToNitrousVec(gEnt->r.mins, &localMins);
        Phys_Vec3ToNitrousVec(gEnt->r.maxs, &localMaxs);
        boundsHalfExtents.x = 0.5f * (localMaxs.x - localMins.x);
        boundsHalfExtents.y = 0.5f * (localMaxs.y - localMins.y);
        boundsHalfExtents.z = 0.5f * (localMaxs.z - localMins.z);
        boundsCenter.x = 0.5f * (localMaxs.x + localMins.x);
        boundsCenter.y = 0.5f * (localMaxs.y + localMins.y);
        boundsCenter.z = 0.5f * (localMaxs.z + localMins.z);
        AnglesToAxis(angles, rotationAxis);
        Phys_AxisToNitrousMat(rotationAxis, &entityTransform);
        Phys_Vec3ToNitrousVec(origin, &entityTransform.w);
        phys_calc_world_aabb(&boundsCenter, &boundsHalfExtents, &entityTransform, &worldAabbMin, &worldAabbMax);
        Phys_NitrousVecToVec3(&worldAabbMin, gEnt->r.absmin);
        Phys_NitrousVecToVec3(&worldAabbMax, gEnt->r.absmax);
    }
    else if (gEnt->sentient)
    {
        gEnt->r.absmin[0] = origin[0] + gEnt->r.mins[0];
        gEnt->r.absmin[1] = origin[1] + gEnt->r.mins[1];
        gEnt->r.absmin[2] = origin[2] + gEnt->r.mins[2];
        gEnt->r.absmax[0] = origin[0] + gEnt->r.maxs[0];
        gEnt->r.absmax[1] = origin[1] + gEnt->r.maxs[1];
        gEnt->r.absmax[2] = origin[2] + gEnt->r.maxs[2];
    }
    else
    {
        boundingRadius = RadiusFromBounds(gEnt->r.mins, gEnt->r.maxs);
        for (axis = 0; axis < 3; ++axis)
        {
            gEnt->r.absmin[axis] = origin[axis] - boundingRadius;
            gEnt->r.absmax[axis] = origin[axis] + boundingRadius;
        }
    }

    // Slightly inflate abs bounds so traces do not miss entity edges.
    gEnt->r.absmin[0] -= 1.0f;
    gEnt->r.absmin[1] -= 1.0f;
    gEnt->r.absmin[2] -= 1.0f;
    gEnt->r.absmax[0] += 1.0f;
    gEnt->r.absmax[1] += 1.0f;
    gEnt->r.absmax[2] += 1.0f;

    svEnt->numClusters = 0;
    svEnt->lastCluster = 0;

    // SVF_NOCLIENT (1) | SVF_BOT (8) | SVF_OBJECTIVE (16) skips cluster assignment.
    if ((gEnt->r.svFlags & 0x19) == 0)
    {
        numLeafs = CM_BoxLeafnums(gEnt->r.absmin, gEnt->r.absmax, leafs, 128, &lastLeaf);
        if (!numLeafs)
        {
            CM_UnlinkEntity(svEnt);
            return;
        }

        int leafIndex = 0;
        for (; leafIndex < numLeafs; ++leafIndex)
        {
            leafCluster = CM_LeafCluster(leafs[leafIndex]);
            if (leafCluster != -1)
            {
                svEnt->clusternums[svEnt->numClusters++] = leafCluster;
                if (svEnt->numClusters == 16)
                    break;
            }
        }

        if (leafIndex != numLeafs)
            svEnt->lastCluster = CM_LeafCluster(lastLeaf);
    }

    gEnt->r.linked = 1;

    if (!gEnt->r.contents)
    {
        CM_UnlinkEntity(svEnt);
        return;
    }

    clipHandle = SV_ClipHandleForEntity(gEnt);
    serverDObj = Com_GetServerDObj(gEnt->s.number);

    // SVF_CLIENTMASK (2) | SVF_MODEL (4): link using locational or DObj bounds instead of abs box.
    if (serverDObj && (gEnt->r.svFlags & 6) != 0)
    {
        if ((gEnt->r.svFlags & 2) != 0)
        {
            linkMins[0] = origin[0] + actorLocationalMins[0];
            linkMins[1] = origin[1] + actorLocationalMins[1];
            linkMins[2] = origin[2] + actorLocationalMins[2];
            linkMaxs[0] = origin[0] + actorLocationalMaxs[0];
            linkMaxs[1] = origin[1] + actorLocationalMaxs[1];
            linkMaxs[2] = origin[2] + actorLocationalMaxs[2];
        }
        else
        {
            DObjGetBounds(serverDObj, linkMins, linkMaxs);
            linkMins[0] += origin[0];
            linkMins[1] += origin[1];
            linkMins[2] += origin[2];
            linkMaxs[0] += origin[0];
            linkMaxs[1] += origin[1];
            linkMaxs[2] += origin[2];
        }
        CM_LinkEntity(svEnt, linkMins, linkMaxs, clipHandle);
    }
    else
    {
        CM_LinkEntity(svEnt, gEnt->r.absmin, gEnt->r.absmax, clipHandle);
    }
}

static void SnapAngles(float *angles)
{
    for (int axis = 0; axis < 3; ++axis)
    {
        const int rounded = (int)(angles[axis] + 9.313225746154785e-10);
        const float delta = (float)rounded - angles[axis];

        if (delta * delta < 0.000001f)
            angles[axis] = (float)rounded;
    }
}

// Capsule move clip test against one linked entity (DObj collmap or brush/bbox).
void __cdecl SV_TraceCapsuleToEntity(const moveclip_t *clip, svEntity_s *check, trace_t *trace)
{
    float expandedMins[3];
    float expandedMaxs[3];
    gentity_s *touch;
    DObj *dobj;
    PhysGeomList *collmapGeoms;
    unsigned int clipHandle;
    const float *angles;
    int entityIndex;
    float traceFractionBefore;

    entityIndex = ((char *)check - (char *)sv.svEntities) / sizeof(svEntity_s);
    touch = SV_GentityNum(entityIndex);

    if ((touch->r.contents & clip->contentmask) == 0)
        return;

    // 1023 (ENTITYNUM_NONE) disables pass-entity filtering.
    if (clip->passEntityNum != 1023
        && (entityIndex == clip->passEntityNum
            || (touch->r.ownerNum.isDefined()
                && (touch->r.ownerNum.entnum() == clip->passEntityNum
                    || touch->r.ownerNum.entnum() == clip->passOwnerNum))))
    {
        return;
    }

    expandedMins[0] = touch->r.absmin[0] + clip->mins[0];
    expandedMins[1] = touch->r.absmin[1] + clip->mins[1];
    expandedMins[2] = touch->r.absmin[2] + clip->mins[2];
    expandedMaxs[0] = touch->r.absmax[0] + clip->maxs[0];
    expandedMaxs[1] = touch->r.absmax[1] + clip->maxs[1];
    expandedMaxs[2] = touch->r.absmax[2] + clip->maxs[2];

    if (!intersect_extents_aabb(&clip->extents, expandedMins, expandedMaxs, trace->fraction))
        return;

    if (BG_ClipMoveToDobj(&touch->s))
        dobj = Com_GetServerDObj(touch->s.number);
    else
        dobj = 0;

    if (dobj)
    {
        if (!DObjHasCollmap(dobj) || !DObjHasContents(dobj, clip->contentmask))
            return;

        collmapGeoms = DObjGetCollmap(dobj);
        if (!collmapGeoms)
            return;

        clipHandle = CM_TempBrushModel(collmapGeoms);
        traceFractionBefore = trace->fraction;
        CM_TransformedBoxTrace(
            trace,
            clip->extents.start.vec.v,
            clip->extents.end.vec.v,
            clip->mins,
            clip->maxs,
            clipHandle,
            clip->contentmask,
            touch->r.currentOrigin,
            touch->r.currentAngles);

        if (trace->fraction < traceFractionBefore)
        {
            iassert(touch->s.number == static_cast<unsigned short>(touch->s.number));
            iassert(trace);
            trace->hitType = TRACE_HITTYPE_ENTITY;
            trace->hitId = touch->s.number;
        }
        return;
    }

    // Script movers without DObj still use brush/bbox; ET_SCRIPTMOVER alone is skipped.
    if (!touch->r.bmodel && touch->s.eType == ET_SCRIPTMOVER)
        return;

    clipHandle = SV_ClipHandleForEntity(touch);
    angles = touch->r.bmodel ? touch->r.currentAngles : vec3_origin;
    traceFractionBefore = trace->fraction;
    CM_TransformedBoxTrace(
        trace,
        clip->extents.start.vec.v,
        clip->extents.end.vec.v,
        clip->mins,
        clip->maxs,
        clipHandle,
        clip->contentmask,
        touch->r.currentOrigin,
        angles);

    if (trace->fraction < traceFractionBefore)
    {
        iassert(touch->s.number == static_cast<unsigned short>(touch->s.number));
        iassert(trace);
        trace->hitType = TRACE_HITTYPE_ENTITY;
        trace->hitId = touch->s.number;
    }
}

static bool SV_ShouldIgnorePointTraceEntity(
    const pointtrace_t *clip,
    int entityIndex,
    const gentity_s *touch)
{
    const IgnoreEntParams *ignore = clip->ignoreEntParams;

    if (!ignore || ignore->baseEntity == 1023)
        return false;

    if (ignore->ignoreSelf && entityIndex == ignore->baseEntity)
        return true;
    if (ignore->ignoreParent && entityIndex == ignore->parentEntity)
        return true;

    if (!touch->r.ownerNum.isDefined())
        return false;

    if (ignore->ignoreSiblings
        && touch->r.ownerNum.entnum() == ignore->parentEntity
        && entityIndex != ignore->baseEntity)
    {
        return true;
    }

    if (ignore->ignoreChildren && touch->r.ownerNum.entnum() == ignore->baseEntity)
        return true;

    return false;
}

// Point trace against one entity: locational DObj line or brush/bbox fallback.
void __cdecl SV_TracePointToEntity(const pointtrace_t *clip, svEntity_s *check, trace_t *trace)
{
    float traceBoundsMins[3];
    float traceBoundsMaxs[3];
    float entAxis[4][3];
    gentity_s *touch;
    unsigned int clipHandle;
    DObj *dobj;
    float localStart[3];
    const float *angles;
    DObjTrace_s objTrace;
    float localEnd[3];
    int entityIndex;
    int partBits[5];
    float traceFractionBefore;
    const float *locationalMins;
    const float *locationalMaxs;
    float normalLengthDelta;

    entityIndex = ((char *)check - (char *)sv.svEntities) / sizeof(svEntity_s);
    touch = SV_GentityNum(entityIndex);

    if ((touch->r.contents & clip->contentmask) == 0 || SV_ShouldIgnorePointTraceEntity(clip, entityIndex, touch))
        return;

    dobj = SV_LocationalTraceDObj(clip, touch);
    if (!dobj)
    {
        PROF_SCOPED("SV_TracePointToEntity 2");

        // Script movers normally use DObj traces; fall back to bounds when no server DObj exists.
        if (!touch->r.bmodel && touch->s.eType == ET_SCRIPTMOVER && (touch->r.svFlags & 4) == 0)
            return;

        if ((check->linkcontents & clip->contentmask) == 0)
            return;
        if (!intersect_extents_aabb(&clip->extents, touch->r.absmin, touch->r.absmax, trace->fraction))
            return;

        clipHandle = SV_ClipHandleForEntity(touch);
        angles = touch->r.bmodel ? touch->r.currentAngles : vec3_origin;
        traceFractionBefore = trace->fraction;
        CM_TransformedBoxTrace(
            trace,
            clip->extents.start.vec.v,
            clip->extents.end.vec.v,
            vec3_origin,
            vec3_origin,
            clipHandle,
            clip->contentmask,
            touch->r.currentOrigin,
            angles);

        if (trace->fraction >= traceFractionBefore)
            return;

        trace->modelIndex = 0;
        trace->partName = 0;
        trace->boneIndex = 0;
        trace->partGroup = 0;
        iassert(touch->s.number == static_cast<unsigned short>(touch->s.number));
        iassert(trace);
        trace->hitType = TRACE_HITTYPE_ENTITY;
        trace->hitId = touch->s.number;
        trace->cflags = touch->r.contents;
        return;
    }

    PROF_SCOPED("SV_TracePointToEntity 1");

    // SVF_MODEL (4): use full abs bounds; otherwise use actor locational hitbox.
    if ((touch->r.svFlags & 4) != 0)
    {
        if (!DObjHasContents(dobj, clip->contentmask))
            return;

        traceBoundsMins[0] = touch->r.absmin[0];
        traceBoundsMins[1] = touch->r.absmin[1];
        traceBoundsMins[2] = touch->r.absmin[2];
        traceBoundsMaxs[0] = touch->r.absmax[0];
        traceBoundsMaxs[1] = touch->r.absmax[1];
        traceBoundsMaxs[2] = touch->r.absmax[2];
    }
    else
    {
        iassert(clip->priorityMap);

        if (touch->client)
        {
            locationalMins = actorLocationalMinsBig;
            locationalMaxs = actorLocationalMaxsBig;
        }
        else
        {
            locationalMins = actorLocationalMins;
            locationalMaxs = actorLocationalMaxs;
        }

        traceBoundsMins[0] = touch->r.currentOrigin[0] + locationalMins[0];
        traceBoundsMins[1] = touch->r.currentOrigin[1] + locationalMins[1];
        traceBoundsMins[2] = touch->r.currentOrigin[2] + locationalMins[2];
        traceBoundsMaxs[0] = touch->r.currentOrigin[0] + locationalMaxs[0];
        traceBoundsMaxs[1] = touch->r.currentOrigin[1] + locationalMaxs[1];
        traceBoundsMaxs[2] = touch->r.currentOrigin[2] + locationalMaxs[2];
    }

    if (!intersect_extents_aabb(&clip->extents, traceBoundsMins, traceBoundsMaxs, trace->fraction))
        return;

    // entAxis[3] is translation; AnglesToAxis only fills the 3x3 rotation rows.
    entAxis[3][0] = touch->r.currentOrigin[0];
    entAxis[3][1] = touch->r.currentOrigin[1];
    entAxis[3][2] = touch->r.currentOrigin[2];
    AnglesToAxis(touch->r.currentAngles, entAxis);
    MatrixTransposeTransformVector43(clip->extents.start.vec.v, entAxis, localStart);
    MatrixTransposeTransformVector43(clip->extents.end.vec.v, entAxis, localEnd);
    objTrace.fraction = trace->fraction;

    if ((touch->r.svFlags & 4) != 0)
    {
        DObjGeomTracelinePartBits(dobj, clip->contentmask, partBits);
        G_DObjCalcPose(touch, partBits);
        DObjGeomTraceline(dobj, localStart, localEnd, clip->contentmask, &objTrace);
    }
    else
    {
        DObjTracelinePartBits(dobj, partBits);
        G_DObjCalcPose(touch, partBits);
        DObjTraceline(dobj, localStart, localEnd, clip->priorityMap, &objTrace);
    }

    if (g_debugLocDamage->current.integer == 2)
        SV_XModelDebugBoxes(touch, colorWhite, partBits, 50);

    if (objTrace.fraction >= trace->fraction)
        return;

    iassert(objTrace.fraction < 1.0f && objTrace.fraction >= 0.0f);

    trace->fraction = objTrace.fraction;
    trace->sflags = objTrace.sflags;
    trace->modelIndex = objTrace.modelIndex;
    trace->partName = objTrace.partName;
    trace->boneIndex = objTrace.localBoneIndex;
    trace->partGroup = objTrace.partGroup;
    MatrixTransformVector(objTrace.normal, entAxis, trace->normal.vec.v);

    normalLengthDelta = Abs(trace->normal.vec.v) - 1.0f;
    if (fabs(normalLengthDelta) >= 0.01f && Abs(trace->normal.vec.v) >= 0.01f)
    {
        const char *normalStr = va(
            "%g %g %g",
            trace->normal.vec.v[0],
            trace->normal.vec.v[1],
            trace->normal.vec.v[2]);
        if (!Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\server\\sv_world.cpp",
                535,
                0,
                "%s\n\t%s",
                "(I_fabs( Vec3Length( trace->normal ) - 1.0f ) < 0.01) || (Vec3Length( trace->normal ) < 0.01)",
                normalStr))
        {
            __debugbreak();
        }
    }

    trace->walkable = trace->normal.vec.v[2] >= 0.69999999f;
    iassert(touch->s.number == static_cast<unsigned short>(touch->s.number));
    iassert(trace);
    trace->hitType = TRACE_HITTYPE_ENTITY;
    trace->hitId = touch->s.number;
    trace->cflags = touch->r.contents;
}

// Returns server DObj for locational damage traces when svFlags and priority map allow it.
DObj *__cdecl SV_LocationalTraceDObj(const pointtrace_t *clip, const gentity_s *touch)
{
    if (!clip->bLocational)
        return 0;
    if ((touch->r.svFlags & 6) == 0)
        return 0;
    if ((touch->r.svFlags & 2) == 0 || clip->priorityMap)
        return Com_GetServerDObj(touch->s.number);
    return 0;
}

// Returns -1 if capsule sight line is blocked by entity, 0 if clear.
int __cdecl SV_SightTraceCapsuleToEntity(const sightclip_t *clip, int entnum)
{
    float sweepMaxs[3];
    gentity_s *touch;
    DObj *dobj;
    PhysGeomList *collmapGeoms;
    int axis;
    unsigned int clipHandle;
    float sweepMins[3];
    const float *angles;
    int blocked;

    touch = SV_GentityNum(entnum);

    if ((touch->r.contents & clip->contentmask) == 0)
        return 0;

    for (axis = 0; axis < 2; ++axis)
    {
        if (clip->passEntityNum[axis] == 1023)
            continue;
        if (entnum == clip->passEntityNum[axis])
            return 0;
        if (touch->r.ownerNum.isDefined() && touch->r.ownerNum.entnum() == clip->passEntityNum[axis])
            return 0;
    }

    // Build axis-aligned bounds enclosing the swept capsule.
    for (axis = 0; axis < 3; ++axis)
    {
        if (clip->end[axis] <= clip->start[axis])
        {
            sweepMins[axis] = clip->end[axis] + clip->mins[axis] - 1.0f;
            sweepMaxs[axis] = clip->start[axis] + clip->maxs[axis] + 1.0f;
        }
        else
        {
            sweepMins[axis] = clip->start[axis] + clip->mins[axis] - 1.0f;
            sweepMaxs[axis] = clip->end[axis] + clip->maxs[axis] + 1.0f;
        }
    }

    if (touch->r.absmin[0] > sweepMaxs[0]
        || touch->r.absmin[1] > sweepMaxs[1]
        || touch->r.absmin[2] > sweepMaxs[2]
        || sweepMins[0] > touch->r.absmax[0]
        || sweepMins[1] > touch->r.absmax[1]
        || sweepMins[2] > touch->r.absmax[2])
    {
        return 0;
    }

    angles = touch->r.currentAngles;
    if (BG_ClipMoveToDobj(&touch->s))
        dobj = Com_GetServerDObj(touch->s.number);
    else
        dobj = 0;

    if (dobj)
    {
        if (!DObjHasCollmap(dobj) || !DObjHasContents(dobj, clip->contentmask))
            return 0;

        collmapGeoms = DObjGetCollmap(dobj);
        if (!collmapGeoms)
            return 0;

        clipHandle = CM_TempBrushModel(collmapGeoms);
        blocked = CM_TransformedBoxSightTrace(
            0,
            clip->start,
            clip->end,
            clip->mins,
            clip->maxs,
            clipHandle,
            clip->contentmask,
            touch->r.currentOrigin,
            angles);
    }
    else
    {
        clipHandle = SV_ClipHandleForEntity(touch);
        if (!touch->r.bmodel)
            angles = vec3_origin;

        blocked = CM_TransformedBoxSightTrace(
            0,
            clip->start,
            clip->end,
            clip->mins,
            clip->maxs,
            clipHandle,
            clip->contentmask,
            touch->r.currentOrigin,
            angles);
    }

    return blocked ? -1 : 0;
}

// Returns -1 if point sight line is blocked by entity, 0 if clear.
int __cdecl SV_SightTracePointToEntity(const sightpointtrace_t *clip, int entnum)
{
    DObj *streamerDObj;
    PhysGeomList *collmapGeoms;
    gentity_s *touch;
    float entAxis[4][3];
    unsigned int clipHandle;
    TraceExtents extents;
    DObj *dobj;
    float traceBoundsMins[3];
    float localStart[3];
    const float *angles;
    DObjTrace_s objTrace;
    float localEnd[3];
    float traceBoundsMaxs[3];
    int passEntityIdx;
    int partBits[5];
    const float *locationalMins;
    const float *locationalMaxs;

    touch = SV_GentityNum(entnum);

    if ((touch->r.contents & clip->contentmask) == 0)
        return 0;

    for (passEntityIdx = 0; passEntityIdx < 2; ++passEntityIdx)
    {
        if (clip->passEntityNum[passEntityIdx] == 1023)
            continue;
        if (entnum == clip->passEntityNum[passEntityIdx])
            return 0;
        if (touch->r.ownerNum.isDefined() && touch->r.ownerNum.entnum() == clip->passEntityNum[passEntityIdx])
            return 0;
    }

    dobj = SV_LocationalSightTraceDObj(clip, touch);
    if (dobj)
    {
        if ((touch->r.svFlags & 4) != 0)
        {
            if (!DObjHasContents(dobj, clip->contentmask))
                return 0;

            DObjGetBounds(dobj, traceBoundsMins, traceBoundsMaxs);
            traceBoundsMins[0] += touch->r.currentOrigin[0];
            traceBoundsMins[1] += touch->r.currentOrigin[1];
            traceBoundsMins[2] += touch->r.currentOrigin[2];
            traceBoundsMaxs[0] += touch->r.currentOrigin[0];
            traceBoundsMaxs[1] += touch->r.currentOrigin[1];
            traceBoundsMaxs[2] += touch->r.currentOrigin[2];
        }
        else
        {
            iassert(clip->priorityMap);

            if (touch->client)
            {
                locationalMins = actorLocationalMinsBig;
                locationalMaxs = actorLocationalMaxsBig;
            }
            else
            {
                locationalMins = actorLocationalMins;
                locationalMaxs = actorLocationalMaxs;
            }

            traceBoundsMins[0] = touch->r.currentOrigin[0] + locationalMins[0];
            traceBoundsMins[1] = touch->r.currentOrigin[1] + locationalMins[1];
            traceBoundsMins[2] = touch->r.currentOrigin[2] + locationalMins[2];
            traceBoundsMaxs[0] = touch->r.currentOrigin[0] + locationalMaxs[0];
            traceBoundsMaxs[1] = touch->r.currentOrigin[1] + locationalMaxs[1];
            traceBoundsMaxs[2] = touch->r.currentOrigin[2] + locationalMaxs[2];
        }

        extents.start.vec.v[0] = clip->start[0];
        extents.start.vec.v[1] = clip->start[1];
        extents.start.vec.v[2] = clip->start[2];
        extents.end.vec.v[0] = clip->end[0];
        extents.end.vec.v[1] = clip->end[1];
        extents.end.vec.v[2] = clip->end[2];
        CM_CalcTraceExtents(&extents);

        if (!intersect_extents_aabb(&extents, traceBoundsMins, traceBoundsMaxs, 1.0f))
            return 0;

        entAxis[3][0] = touch->r.currentOrigin[0];
        entAxis[3][1] = touch->r.currentOrigin[1];
        entAxis[3][2] = touch->r.currentOrigin[2];
        AnglesToAxis(touch->r.currentAngles, entAxis);
        MatrixTransposeTransformVector43(extents.start.vec.v, entAxis, localStart);
        MatrixTransposeTransformVector43(extents.end.vec.v, entAxis, localEnd);
        objTrace.fraction = 1.0f;

        if ((touch->r.svFlags & 4) != 0)
        {
            DObjGeomTracelinePartBits(dobj, clip->contentmask, partBits);
            G_DObjCalcPose(touch, partBits);
            DObjGeomTraceline(dobj, localStart, localEnd, clip->contentmask, &objTrace);
        }
        else
        {
            DObjTracelinePartBits(dobj, partBits);
            G_DObjCalcPose(touch, partBits);
            DObjTraceline(dobj, localStart, localEnd, clip->priorityMap, &objTrace);
        }

        if (objTrace.fraction < 1.0f)
            return -1;

        return 0;
    }

    if (!touch->r.bmodel && touch->s.eType == ET_SCRIPTMOVER)
        return 0;

    // ET_STREAMER_HINT may use DObj collmap for sight checks even without locational trace.
    streamerDObj = touch->s.eType == ET_STREAMER_HINT ? Com_GetServerDObj(touch->s.number) : 0;
    if (streamerDObj)
    {
        if (!DObjHasCollmap(streamerDObj))
            return 0;

        collmapGeoms = DObjGetCollmap(streamerDObj);
        if (!collmapGeoms)
            return 0;

        clipHandle = CM_TempBrushModel(collmapGeoms);
        angles = touch->r.currentAngles;
    }
    else
    {
        clipHandle = SV_ClipHandleForEntity(touch);
        angles = touch->r.bmodel ? touch->r.currentAngles : vec3_origin;
    }

    if (CM_TransformedBoxSightTrace(
            0,
            clip->start,
            clip->end,
            vec3_origin,
            vec3_origin,
            clipHandle,
            clip->contentmask,
            touch->r.currentOrigin,
            angles))
    {
        return -1;
    }

    return 0;
}

DObj *__cdecl SV_LocationalSightTraceDObj(const sightpointtrace_t *clip, const gentity_s *touch)
{
    if (!clip->locational)
        return 0;
    if ((touch->r.svFlags & 6) == 0)
        return 0;
    if ((touch->r.svFlags & 2) == 0 || clip->priorityMap)
        return Com_GetServerDObj(touch->s.number);
    return 0;
}

void __cdecl SV_SetupIgnoreEntParams(IgnoreEntParams *ignoreEntParams, int baseEntity)
{
    gentity_s *baseEnt;

    ignoreEntParams->baseEntity = baseEntity;
    if (baseEntity == 1023)
    {
        ignoreEntParams->parentEntity = -1;
    }
    else
    {
        baseEnt = SV_GentityNum(baseEntity);
        if (baseEnt->r.ownerNum.isDefined())
            ignoreEntParams->parentEntity = baseEnt->r.ownerNum.entnum();
        else
            ignoreEntParams->parentEntity = -1;
    }

    ignoreEntParams->ignoreSelf = 1;
    ignoreEntParams->ignoreChildren = 1;
    ignoreEntParams->ignoreSiblings = 1;
    ignoreEntParams->ignoreParent = 0;
}

static void SV_SetInitialTraceHitFromWorld(trace_t *results)
{
    iassert(results);

    if (results->fraction == 1.0f)
    {
        results->hitType = TRACE_HITTYPE_NONE;
        results->hitId = 0;
    }
    else
    {
        results->hitType = TRACE_HITTYPE_ENTITY;
        results->hitId = 1022;
    }
}

static void SV_ValidateIgnoreEntParams(const IgnoreEntParams *ignoreEntParams)
{
    gentity_s *baseEnt;
    const char *assertDetail;

    if (ignoreEntParams->baseEntity == 1023 || ignoreEntParams->parentEntity == -1)
        return;

    baseEnt = SV_GentityNum(ignoreEntParams->baseEntity);
    if (baseEnt->r.ownerNum.isDefined()
        && baseEnt->r.ownerNum.entnum() == (unsigned int)ignoreEntParams->parentEntity)
    {
        return;
    }

    if (baseEnt->r.ownerNum.isDefined())
    {
        assertDetail = va(
            "base: %d; parent: %d; base's parent: %d\n",
            ignoreEntParams->baseEntity,
            ignoreEntParams->parentEntity,
            baseEnt->r.ownerNum.entnum());
    }
    else
    {
        assertDetail = va(
            "base: %d; parent: %d; base's parent: %d\n",
            ignoreEntParams->baseEntity,
            ignoreEntParams->parentEntity,
            1023);
    }

    if (!Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\server\\sv_world.cpp",
            867,
            0,
            "%s\n\t%s",
            "context.ignoreEntParams->baseEntity == ENTITYNUM_NONE || context.ignoreEntParams->parentEntity == -1 || "
            "(SV_GentityNum( context.ignoreEntParams->baseEntity )->r.ownerNum.isDefined() && (uint)context.ignoreEntParams"
            "->parentEntity == SV_GentityNum( context.ignoreEntParams->baseEntity )->r.ownerNum.entnum())",
            assertDetail))
    {
        __debugbreak();
    }
}

void __cdecl SV_TracePoint(trace_t *results, const float *start, const float *end, col_context_t *context)
{
    pointtrace_t clip;

    PROF_SCOPED("SV_TracePoint");

    nanassertvec3(start);
    nanassertvec3(end);

    CM_BoxTrace(results, start, end, vec3_origin, vec3_origin, context->mask, context);
    iassert(!IS_NAN(results->fraction));
    SV_SetInitialTraceHitFromWorld(results);

    if (results->fraction == 0.0f)
        return;

    if (context->staticmodels)
    {
        CM_PointTraceStaticModels(results, start, end, context->mask);
        iassert(!IS_NAN(results->fraction));
        if (results->fraction == 0.0f)
            return;
    }

    clip.contentmask = context->mask;
    clip.extents.start.vec.v[0] = start[0];
    clip.extents.start.vec.v[1] = start[1];
    clip.extents.start.vec.v[2] = start[2];
    clip.extents.end.vec.v[0] = end[0];
    clip.extents.end.vec.v[1] = end[1];
    clip.extents.end.vec.v[2] = end[2];
    CM_CalcTraceExtents(&clip.extents);
    clip.ignoreEntParams = context->ignoreEntParams;
    clip.bLocational = context->locational;
    clip.priorityMap = context->priorityMap;

    SV_ValidateIgnoreEntParams(context->ignoreEntParams);

    CM_PointTraceToEntities(&clip, results, context);
    DynEntSv_PointTrace(&clip, results);
    GlassSv_PointTrace(&clip, results);
}

void __cdecl SV_TraceCapsule(
                trace_t *results,
                const float *start,
                const float *mins,
                const float *maxs,
                const float *end,
                col_context_t *context)
{
    moveclip_t clip;
    float capsuleCenterOffset[3];

    PROF_SCOPED("SV_TraceCapsule");

    iassert(mins);
    iassert(maxs);
    iassert(maxs[0] >= mins[0]);
    iassert(maxs[1] >= mins[1]);
    iassert(maxs[2] >= mins[2]);
    nanassertvec3(mins);
    nanassertvec3(maxs);
    nanassertvec3(start);
    nanassertvec3(end);

    CM_BoxTrace(results, start, end, mins, maxs, context->mask, context);
    iassert(!IS_NAN(results->fraction));
    SV_SetInitialTraceHitFromWorld(results);

    if (results->fraction == 0.0f)
        return;

    clip.contentmask = context->mask;
    clip.passEntityNum = context->ignoreEntParams->baseEntity;
    SV_ValidateIgnoreEntParams(context->ignoreEntParams);
    clip.passOwnerNum = context->ignoreEntParams->parentEntity;

    clip.outerSize[0] = maxs[0] - mins[0];
    clip.outerSize[1] = maxs[1] - mins[1];
    clip.outerSize[2] = maxs[2] - mins[2];
    clip.maxs[0] = 0.5f * clip.outerSize[0];
    clip.maxs[1] = 0.5f * clip.outerSize[1];
    clip.maxs[2] = 0.5f * clip.outerSize[2];
    clip.mins[0] = -clip.maxs[0];
    clip.mins[1] = -clip.maxs[1];
    clip.mins[2] = -clip.maxs[2];
    clip.outerSize[0] = clip.maxs[0] + 1.0f;
    clip.outerSize[1] = clip.maxs[1] + 1.0f;
    clip.outerSize[2] = clip.maxs[2] + 1.0f;

    capsuleCenterOffset[0] = 0.5f * (maxs[0] + mins[0]);
    capsuleCenterOffset[1] = 0.5f * (maxs[1] + mins[1]);
    capsuleCenterOffset[2] = 0.5f * (maxs[2] + mins[2]);
    clip.extents.start.vec.v[0] = start[0] + capsuleCenterOffset[0];
    clip.extents.start.vec.v[1] = start[1] + capsuleCenterOffset[1];
    clip.extents.start.vec.v[2] = start[2] + capsuleCenterOffset[2];
    clip.extents.end.vec.v[0] = end[0] + capsuleCenterOffset[0];
    clip.extents.end.vec.v[1] = end[1] + capsuleCenterOffset[1];
    clip.extents.end.vec.v[2] = end[2] + capsuleCenterOffset[2];
    CM_CalcTraceExtents(&clip.extents);

    CM_ClipMoveToEntities(&clip, results);
    DynEntSv_ClipMoveTrace(&clip, results);
    GlassSv_ClipMoveTrace(&clip, results);
}

bool __cdecl SV_SightTraceCapsule(
                int *hitNum,
                const float *start,
                const float *mins,
                const float *maxs,
                const float *end,
                col_context_t *context)
{
    sightclip_t clip;
    float capsuleCenterOffset[3];
    float capsuleVolume;

    PROF_SCOPED("SV_SightTrace");

    iassert(mins);
    iassert(maxs);
    iassert(maxs[0] >= mins[0]);
    iassert(maxs[1] >= mins[1]);
    iassert(maxs[2] >= mins[2]);
    nanassertvec3(mins);
    nanassertvec3(maxs);
    nanassertvec3(start);
    nanassertvec3(end);

    *hitNum = CM_BoxSightTrace(*hitNum, start, end, mins, maxs, 0, context->mask);
    if (*hitNum)
        return false;

    capsuleVolume = (maxs[0] - mins[0]) + (maxs[1] - mins[1]) + (maxs[2] - mins[2]);
    iassert(capsuleVolume != 0.0f);

    clip.contentmask = context->mask;
    clip.passEntityNum[0] = context->passEntityNum0;
    clip.passEntityNum[1] = context->passEntityNum1;
    clip.outerSize[0] = maxs[0] - mins[0];
    clip.outerSize[1] = maxs[1] - mins[1];
    clip.outerSize[2] = maxs[2] - mins[2];
    clip.outerSize[0] = 0.5f * clip.outerSize[0];
    clip.outerSize[1] = 0.5f * clip.outerSize[1];
    clip.outerSize[2] = 0.5f * clip.outerSize[2];
    clip.maxs[0] = clip.outerSize[0];
    clip.maxs[1] = clip.outerSize[1];
    clip.maxs[2] = clip.outerSize[2];
    clip.mins[0] = -clip.outerSize[0];
    clip.mins[1] = -clip.outerSize[1];
    clip.mins[2] = -clip.outerSize[2];
    clip.outerSize[0] += 1.0f;
    clip.outerSize[1] += 1.0f;
    clip.outerSize[2] += 1.0f;

    capsuleCenterOffset[0] = 0.5f * (maxs[0] + mins[0]);
    capsuleCenterOffset[1] = 0.5f * (maxs[1] + mins[1]);
    capsuleCenterOffset[2] = 0.5f * (maxs[2] + mins[2]);
    clip.start[0] = start[0] + capsuleCenterOffset[0];
    clip.start[1] = start[1] + capsuleCenterOffset[1];
    clip.start[2] = start[2] + capsuleCenterOffset[2];
    clip.end[0] = end[0] + capsuleCenterOffset[0];
    clip.end[1] = end[1] + capsuleCenterOffset[1];
    clip.end[2] = end[2] + capsuleCenterOffset[2];

    *hitNum = CM_ClipSightTraceToEntities(&clip);
    return *hitNum == 0;
}

bool __cdecl SV_SightTracePoint(int *hitNum, const float *start, const float *end, col_context_t *context)
{
    sightpointtrace_t clip;

    PROF_SCOPED("SV_SightTracePoint");

    nanassertvec3(start);
    nanassertvec3(end);

    *hitNum = CM_SightTracePoint(*hitNum, start, end, context);
    if (*hitNum)
        return false;

    if (context->staticmodels && !CM_PointTraceStaticModelsComplete(start, end, context->mask))
        return false;

    clip.contentmask = context->mask;
    clip.start[0] = start[0];
    clip.start[1] = start[1];
    clip.start[2] = start[2];
    clip.end[0] = end[0];
    clip.end[1] = end[1];
    clip.end[2] = end[2];
    clip.passEntityNum[0] = context->passEntityNum0;
    clip.passEntityNum[1] = context->passEntityNum1;
    clip.locational = context->locational;
    clip.priorityMap = context->priorityMap;
    *hitNum = CM_PointSightTraceToEntities(&clip);

    return *hitNum == 0;
}

int __cdecl SV_PointContents(const float *point, int passEntityNum, int contentMask)
{
    int combinedContents;
    int entityList[1024];
    int numTouching;
    gentity_s *touchEnt;
    int entityIndex;
    int listIndex;
    unsigned int clipHandle;
    int entityContents;

    combinedContents = CM_PointContents(point, 0);
    numTouching = CM_AreaEntities(point, point, entityList, 1024, contentMask);

    for (listIndex = 0; listIndex < numTouching; ++listIndex)
    {
        entityIndex = entityList[listIndex];
        if (entityIndex == passEntityNum)
            continue;

        touchEnt = SV_GentityNum(entityIndex);
        clipHandle = SV_ClipHandleForEntity(touchEnt);
        entityContents = CM_TransformedPointContents(point, clipHandle, touchEnt->r.currentOrigin, touchEnt->r.currentAngles);
        combinedContents |= entityContents;
    }

    return contentMask & combinedContents;
}

void __cdecl G_TraceCapsule(
                trace_t *results,
                const float *start,
                const float *mins,
                const float *maxs,
                const float *end,
                int passEntityNum,
                int contentmask,
                col_context_t *context)
{
    IgnoreEntParams ignoreEntParams;

    PROF_SCOPED("G_TraceCapsule");

    SV_SetupIgnoreEntParams(&ignoreEntParams, passEntityNum);
    context->ignoreEntParams = &ignoreEntParams;
    context->mask = contentmask;
    SV_TraceCapsule(results, start, mins, maxs, end, context);
}

void __cdecl G_LocationalTrace(
                trace_t *results,
                const float *start,
                const float *end,
                int passEntityNum,
                int contentmask,
                unsigned __int8 *priorityMap,
                int (__cdecl *collide_entity_func)(int, col_context_t *))
{
    IgnoreEntParams ignoreEntParams;

    SV_SetupIgnoreEntParams(&ignoreEntParams, passEntityNum);

    col_context_t context(contentmask);

    context.ignoreEntParams = &ignoreEntParams;
    context.init_locational(passEntityNum);
    context.priorityMap = priorityMap;
    context.collide_entity_func = collide_entity_func;
    SV_TracePoint(results, start, end, &context);
}

void __cdecl G_LocationalTraceAllowChildren(
                trace_t *results,
                const float *start,
                const float *end,
                int passEntityNum,
                int contentmask,
                unsigned __int8 *priorityMap)
{
    IgnoreEntParams ignoreEntParams;

    SV_SetupIgnoreEntParams(&ignoreEntParams, passEntityNum);
    ignoreEntParams.ignoreChildren = 0;

    col_context_t context(contentmask);

    context.ignoreEntParams = &ignoreEntParams;
    context.init_locational(passEntityNum);
    context.priorityMap = priorityMap;
    SV_TracePoint(results, start, end, &context);
}

