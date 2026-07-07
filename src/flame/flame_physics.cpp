#include "flame_physics.h"
#include "flame_system.h"
#include "flame_class_stream.h"
#include <tl/tl_system.h>
#include <universal/assertive.h>
#include <DynEntity/DynEntity_coll.h>
#include <server_mp/sv_main_mp.h>
#include <qcommon/cm_tracebox.h>
#include <server/sv_world.h>
#include <game_mp/g_main_mp.h>
#include <clientscript/scr_const.h>
#include <qcommon/cm_load.h>
#include <DynEntity/DynEntity_server.h>
#include <glass/glass_server.h>
#include <game_mp/g_combat_mp.h>
#include <DynEntity/DynEntity_client.h>
#include <DynEntity/DynEntity_load_obj.h>
#include <qcommon/dobj_management.h>
#include <cgame_mp/cg_local_mp.h>
#include <cgame/cg_world.h>
#include <physics/destructible.h>
#include <glass/glass_client.h>
#include <game_mp/g_trigger_mp.h>
#include <bgame/bg_fire.h>
#include <game/g_weapon.h>
#include <game/g_debug.h>
#include <gfx_d3d/r_dpvs.h>
#include <qcommon/cm_world.h>
#include <algorithm>
#include <cmath>
#include <client/splitscreen.h>
#include <cgame/cg_drawtools.h>

cdl_proftimer sv_flame_proftimer;
cdl_proftimer cl_flame_proftimer;

colgeom_visitor_inlined_t<500> cl_flame_chunks_proximity_cache;
colgeom_visitor_inlined_t<500> sv_flame_chunks_proximity_cache;

float FLAME_OVERCLIP = 0.001f;

float mins[3] = { -8.0f, -8.0f, -8.0f };
float maxs[3] = { 8.0f, 8.0f, 8.0f };
float size[3] = { 8.0f, 8.0f, 8.0f };

float radius_2 = 1.0f;
float radius_3 = 1.0f;

void __cdecl Flame_ClipVelocity(const float *in, const float *normal, float *out)
{
    float dot;
    float backoff;

    dot = in[0] * normal[0] + in[1] * normal[1] + in[2] * normal[2];
    backoff = -(dot - FLAME_OVERCLIP * fabsf(dot));
    out[0] = in[0] + backoff * normal[0];
    out[1] = in[1] + backoff * normal[1];
    out[2] = in[2] + backoff * normal[2];
}

void __cdecl trace_sphere(trace_t *trace, const float *start, const float *end, float radius, col_context_t *context)
{
    const col_prim_t *prim;
    float dir[3];
    float boundsMin[3];
    float boundsMax[3];
    float partitionMin[3];
    float partitionMax[3];
    int primIndex;

    Vec3Min(start, end, boundsMin);
    Vec3Max(start, end, boundsMax);
    boundsMin[0] = boundsMin[0] - radius;
    boundsMin[1] = boundsMin[1] - radius;
    boundsMin[2] = boundsMin[2] - radius;
    boundsMax[0] = boundsMax[0] + radius;
    boundsMax[1] = boundsMax[1] + radius;
    boundsMax[2] = boundsMax[2] + radius;
    dir[0] = end[0] - start[0];
    dir[1] = end[1] - start[1];
    dir[2] = end[2] - start[2];
    trace->fraction = 1.0f;
    prim = context->prims;
    for ( primIndex = 0; primIndex < context->nprims; ++primIndex )
    {
        if ( prim->type )
        {
            // Port fix: decomp compares tree halfSize/origin here (bogus AABB test).
            // Match trace_sphere_vs_env — use brush mins/maxs for overlap.
            const cbrush_t *brush = prim->brush;
            if ( brush
                && brush->maxs[0] >= boundsMin[0]
                && brush->maxs[1] >= boundsMin[1]
                && brush->maxs[2] >= boundsMin[2]
                && boundsMax[0] >= brush->mins[0]
                && boundsMax[1] >= brush->mins[1]
                && boundsMax[2] >= brush->mins[2] )
            {
                trace_sphere_through_brush(
                    start,
                    dir,
                    &trace->fraction,
                    radius,
                    brush,
                    trace->normal.vec.v,
                    &trace->sflags,
                    NULL);
            }
        }
        else
        {
            const CollisionAabbTree *tree = prim->tree;
            const float *halfSize = prim->tree->halfSize;
            partitionMin[0] = tree->origin[0] - halfSize[0];
            partitionMin[1] = tree->origin[1] - halfSize[1];
            partitionMin[2] = tree->origin[2] - halfSize[2];
            partitionMax[0] = tree->origin[0] + halfSize[0];
            partitionMax[1] = tree->origin[1] + halfSize[1];
            partitionMax[2] = tree->origin[2] + halfSize[2];
            if ( partitionMax[0] >= boundsMin[0]
                && partitionMax[1] >= boundsMin[1]
                && partitionMax[2] >= boundsMin[2]
                && boundsMax[0] >= partitionMin[0]
                && boundsMax[1] >= partitionMin[1]
                && boundsMax[2] >= partitionMin[2] )
            {
                trace_sphere_through_partition(
                    start,
                    dir,
                    &trace->fraction,
                    radius,
                    prim->tree,
                    trace->normal.vec.v,
                    &trace->sflags);
            }
        }
        ++prim;
    }
    if ( trace->fraction == 1.0f )
    {
        iassert(trace);
        trace->hitType = TRACE_HITTYPE_NONE;
        trace->hitId = 0;
    }
    else
    {
        iassert(trace);
        trace->hitType = TRACE_HITTYPE_ENTITY;
        trace->hitId = 1022;
    }
    if ( trace->fraction > 0.0f )
    {
        if ( trace->fraction > 1.0f )
            trace->fraction = 1.0f;
    }
    else
    {
        trace->fraction = 0.0f;
    }
}

void __cdecl Flame_Phys_Collision(
                bool is_server,
                trace_t *trace,
                flameGeneric_s *gen,
                col_context_t *context,
                unsigned __int16 (*dynEnts)[2][100],
                const int *dynEntsCount,
                int *ents,
                int entsCount,
                int *close_characters,
                int *close_characters_count,
                int *close_triggers,
                int *close_triggers_count)
{
    float startPos[3];
    float endPos[3];

    startPos[0] = gen->phys.origin[0];
    startPos[1] = gen->phys.origin[1];
    startPos[2] = gen->phys.origin[2];
    endPos[0] = gen->phys.newPos[0];
    endPos[1] = gen->phys.newPos[1];
    endPos[2] = gen->phys.newPos[2];
    nanassertvec3(startPos);
    if ( is_server )
    {
        sv_flame_proftimer.stamp = tlPcGetTick().QuadPart;
        ++sv_flame_proftimer.calls;
        Flame_Server_Trace(
            trace,
            gen,
            startPos,
            mins,
            maxs,
            endPos,
            context,
            dynEnts,
            dynEntsCount,
            ents,
            entsCount,
            close_characters,
            close_characters_count,
            close_triggers,
            close_triggers_count);
        sv_flame_proftimer.value += tlPcGetTick().QuadPart - sv_flame_proftimer.stamp;
    }
    else
    {
        cl_flame_proftimer.stamp = tlPcGetTick().QuadPart;
        ++cl_flame_proftimer.calls;
        Flame_Client_Trace(
            trace,
            gen,
            startPos,
            mins,
            maxs,
            endPos,
            context,
            dynEnts,
            dynEntsCount,
            ents,
            entsCount,
            close_characters,
            close_characters_count);
        cl_flame_proftimer.value += tlPcGetTick().QuadPart - cl_flame_proftimer.stamp;
    }
}

void __cdecl Flame_Server_Trace(
                trace_t *trace,
                flameGeneric_s *gen,
                const float *startPos,
                const float *mins,
                const float *maxs,
                float *endPos,
                col_context_t *context,
                unsigned __int16 (*dynEnts)[2][100],
                const int *dynEntsCount,
                int *ents,
                int entsCount,
                int *close_characters,
                int *close_characters_count,
                int *close_triggers,
                int *close_triggers_count)
{
    pointtrace_t clip;
    IgnoreEntParams ignoreEntParams;
    trace_t old_trace;
    trace_t trace2;
    float boundsMin[3];
    float boundsMax[3];
    float center[3];
    float hitDir[3];
    float hitPos[3];
    float collisionRadius;
    float dist2;
    int entIndex;

    trace_sphere(trace, startPos, endPos, radius_2, context);
    if ( trace->fraction <= 0.0f )
        return;

    Vec3Min(startPos, endPos, boundsMin);
    Vec3Max(startPos, endPos, boundsMax);
    clip.contentmask = 0x2C0EC33;
    clip.extents.start.vec.v[0] = startPos[0];
    clip.extents.start.vec.v[1] = startPos[1];
    clip.extents.start.vec.v[2] = startPos[2];
    clip.extents.end.vec.v[0] = endPos[0];
    clip.extents.end.vec.v[1] = endPos[1];
    clip.extents.end.vec.v[2] = endPos[2];
    CM_CalcTraceExtents(&clip.extents);
    clip.bLocational = 1;
    clip.priorityMap = bulletPriorityMap;
    SV_SetupIgnoreEntParams(&ignoreEntParams, gen->stream->entityNum);
    ignoreEntParams.ignoreChildren = 0;
    clip.ignoreEntParams = &ignoreEntParams;
    for ( entIndex = 0; entIndex < entsCount; ++entIndex )
    {
        const unsigned int entnum = ents[entIndex];
        bool character_collision = false;
        gentity_s *ent = (gentity_s *)((char *)sv.gentities + entnum * sv.gentitySize);

        clip.bLocational = 1;
        if ( ent && ent->client )
            character_collision = true;
        if ( character_collision )
        {
            if ( *close_characters_count < 16 )
            {
                dist2 = point_aabb_dist2(endPos, ent->r.absmin, ent->r.absmax);
                collisionRadius = gen->size.current * gen->stream->flameVars->flameVar_collisionVolumeScale;
                if ( collisionRadius * collisionRadius > dist2 && gen->stream->entityNum != entnum )
                {
                    int existingIndex;
                    for ( existingIndex = 0; existingIndex < *close_characters_count; ++existingIndex )
                    {
                        if ( close_characters[existingIndex] == (int)entnum )
                            break;
                    }
                    if ( existingIndex < *close_characters_count )
                        continue;
                    center[0] = 0.5f * ent->r.absmin[0] + 0.5f * ent->r.absmax[0];
                    center[1] = 0.5f * ent->r.absmin[1] + 0.5f * ent->r.absmax[1];
                    center[2] = 0.5f * ent->r.absmin[2] + 0.5f * ent->r.absmax[2];
                    if ( !collide_segment(startPos, center, context) )
                        close_characters[(*close_characters_count)++] = entnum;
                }
            }
        }
        else
        {
            svEntity_s *check = &sv.svEntities[entnum];
            if ( check->linkmax[0] >= boundsMin[0]
                && sv.svEntities[entnum].linkmax[1] >= boundsMin[1]
                && sv.svEntities[entnum].linkmax[2] >= boundsMin[2]
                && boundsMax[0] >= sv.svEntities[entnum].linkmin[0]
                && boundsMax[1] >= sv.svEntities[entnum].linkmin[1]
                && boundsMax[2] >= sv.svEntities[entnum].linkmin[2] )
            {
                if ( ent->classname == scr_const.trigger_damage )
                {
                    memcpy(&old_trace, trace, sizeof(old_trace));
                    SV_TracePointToEntity(&clip, check, trace);
                    if ( old_trace.fraction > trace->fraction )
                    {
                        if ( *close_triggers_count < 16 )
                            close_triggers[(*close_triggers_count)++] = entnum;
                        memcpy(trace, &old_trace, sizeof(trace_t));
                    }
                }
                else
                {
                    SV_TracePointToEntity(&clip, check, trace);
                    if ( trace->fraction <= 0.0f )
                        return;
                }
            }
        }
    }
    for ( int drawType = 0; drawType < 2; ++drawType )
    {
        const int dynEntCount = dynEntsCount[drawType];
        for ( int dynEntIndex = 0; dynEntIndex < dynEntCount; ++dynEntIndex )
        {
            const unsigned __int16 id = (*dynEnts)[drawType][dynEntIndex];
            const DynEntityColl *coll = &cm.dynEntCollList[drawType + 2][id];
            if ( boundsMin[2] > coll->linkMaxs[2] )
                break;
            if ( coll->linkMaxs[0] >= boundsMin[0]
                && coll->linkMaxs[1] >= boundsMin[1]
                && coll->linkMaxs[2] >= boundsMin[2]
                && boundsMax[0] >= coll->linkMins[0]
                && boundsMax[1] >= coll->linkMins[1]
                && boundsMax[2] >= coll->linkMins[2] )
            {
                DynEntSv_PointTrace(id, (DynEntityDrawType)drawType, &clip, trace);
                if ( trace->fraction <= 0.0f )
                    return;
            }
        }
    }
    memcpy(&trace2, trace, sizeof(trace2));
    GlassSv_PointTrace(&clip, &trace2);
    if ( trace->fraction > trace2.fraction && trace2.hitType == TRACE_HITTYPE_GLASS )
    {
        Vec3Lerp(clip.extents.start.vec.v, clip.extents.end.vec.v, trace2.fraction, hitPos);
        hitDir[0] = clip.extents.end.vec.v[0] - clip.extents.start.vec.v[0];
        hitDir[1] = clip.extents.end.vec.v[1] - clip.extents.start.vec.v[1];
        hitDir[2] = clip.extents.end.vec.v[2] - clip.extents.start.vec.v[2];
        Vec3Normalize(hitDir);
        GlassSv_Damage(trace2.hitId, 10000, 17, hitPos, hitDir);
    }
}

double __cdecl point_aabb_dist2(const float *point, const float *boxMin, const float *boxMax)
{
    float closest[3];

    closest[0] = point[0];
    closest[1] = point[1];
    closest[2] = point[2];
    if ( point[0] > boxMax[0] )
        closest[0] = boxMax[0];
    if ( closest[1] > boxMax[1] )
        closest[1] = boxMax[1];
    if ( closest[2] > boxMax[2] )
        closest[2] = boxMax[2];
    if ( boxMin[0] > closest[0] )
        closest[0] = boxMin[0];
    if ( boxMin[1] > closest[1] )
        closest[1] = boxMin[1];
    if ( boxMin[2] > closest[2] )
        closest[2] = boxMin[2];
    return (point[0] - closest[0]) * (point[0] - closest[0])
         + (point[1] - closest[1]) * (point[1] - closest[1])
         + (point[2] - closest[2]) * (point[2] - closest[2]);
}

void __cdecl Flame_Client_Trace(
                trace_t *trace,
                flameGeneric_s *gen,
                const float *startPos,
                const float *mins,
                const float *maxs,
                float *endPos,
                col_context_t *context,
                unsigned __int16 (*dynEnts)[2][100],
                const int *dynEntsCount,
                int *ents,
                int entsCount,
                int *close_characters,
                int *close_characters_count)
{
    pointtrace_t clip;
    trace_t trace2;
    float boundsMin[3];
    float boundsMax[3];
    float absMin[3];
    float absMax[3];
    float actorMins[3];
    float actorMaxs[3];
    float collisionRadius;
    float dist2;

    trace_sphere(trace, startPos, endPos, radius_3, context);
    if ( trace->fraction <= 0.0f )
        return;

    Vec3Min(startPos, endPos, boundsMin);
    Vec3Max(startPos, endPos, boundsMax);
    clip.contentmask = 0x2C0EC33;
    clip.extents.start.vec.v[0] = startPos[0];
    clip.extents.start.vec.v[1] = startPos[1];
    clip.extents.start.vec.v[2] = startPos[2];
    clip.extents.end.vec.v[0] = endPos[0];
    clip.extents.end.vec.v[1] = endPos[1];
    clip.extents.end.vec.v[2] = endPos[2];
    CM_CalcTraceExtents(&clip.extents);
    clip.bLocational = 1;
    clip.priorityMap = bulletPriorityMap;
    clip.ignoreEntParams = 0;
    for ( int drawType = 0; drawType < 2; ++drawType )
    {
        const int dynEntCount = dynEntsCount[drawType];
        for ( int dynEntIndex = 0; dynEntIndex < dynEntCount; ++dynEntIndex )
        {
            const unsigned __int16 id = (*dynEnts)[drawType][dynEntIndex];
            const DynEntityColl *coll = &cm.dynEntCollList[drawType][id];
            if ( boundsMin[2] > coll->linkMaxs[2] )
                break;
            if ( coll->linkMaxs[0] >= boundsMin[0]
                && coll->linkMaxs[1] >= boundsMin[1]
                && coll->linkMaxs[2] >= boundsMin[2]
                && boundsMax[0] >= coll->linkMins[0]
                && boundsMax[1] >= coll->linkMins[1]
                && boundsMax[2] >= coll->linkMins[2] )
            {
                const DynEntityDef *dynEntDef = DynEnt_GetEntityDef(id, (DynEntityDrawType)drawType);
                DynEntityPose *dynEntPose = DynEnt_GetClientPose(id, (DynEntityDrawType)drawType);
                if ( drawType )
                {
                    iassert(drawType == DYNENT_DRAW_BRUSH);
                    DynEnt_PointTraceToBrush(dynEntDef, &dynEntPose->pose, &clip, trace);
                }
                else
                {
                    DynEnt_PointTraceToModel(dynEntDef, &dynEntPose->pose, &clip, trace);
                }
                if ( trace->fraction <= 0.0f )
                    return;
            }
        }
    }
    for ( int entIndex = 0; entIndex < entsCount; ++entIndex )
    {
        const int entnum = ents[entIndex];
        if ( (gen->stream->entityNum <= 32 || gen->stream->entityNum != entnum + 32) && gen->stream->entityNum != entnum )
        {
            centity_s *ent = CG_GetEntity(0, entnum);
            DObj *dobj = Com_GetClientDObj(entnum, 0);
            if ( dobj )
            {
                const int contents = CG_GetEntityDObjContents(ent, dobj);
                if ( (contents & 0x2008000) != 0 )
                {
                    actorMins[0] = -15.0f;
                    actorMins[1] = -15.0f;
                    actorMins[2] = 0.0f;
                    actorMaxs[0] = 15.0f;
                    actorMaxs[1] = 15.0f;
                    actorMaxs[2] = 48.0f;
                    absMin[0] = ent->pose.origin[0] - 15.0f;
                    absMin[1] = ent->pose.origin[1] - 15.0f;
                    absMin[2] = ent->pose.origin[2];
                    absMax[0] = ent->pose.origin[0] + 15.0f;
                    absMax[1] = ent->pose.origin[1] + 15.0f;
                    absMax[2] = ent->pose.origin[2] + 48.0f;
                    dist2 = point_aabb_dist2(endPos, absMin, absMax);
                    collisionRadius = gen->size.current * gen->stream->flameVars->flameVar_collisionVolumeScale;
                    if ( collisionRadius * collisionRadius > dist2 && gen->stream->entityNum != entnum && *close_characters_count < 16 )
                        close_characters[(*close_characters_count)++] = entnum;
                }
                else if ( ent->destructible && CG_DestructibleIsClientOnly(ent->destructible) )
                {
                    CG_PointTraceToEntity(&clip, entnum, trace);
                    if ( trace->fraction <= 0.0f )
                        return;
                }
            }
        }
    }
    memcpy(&trace2, trace, sizeof(trace2));
    GlassCl_TracePoint(&clip, &trace2);
}

void __cdecl Flame_Phys_Collision_Response(bool is_server, trace_t *trace, flameGeneric_s *gen)
{
    float dir[3];
    float startPos[3];
    float newVel[3];
    float hitPos[3];
    float speedScale;
    float fraction;

    startPos[0] = gen->phys.origin[0];
    startPos[1] = gen->phys.origin[1];
    startPos[2] = gen->phys.origin[2];
    nanassertvec3(startPos);
    dir[0] = gen->phys.newPos[0] - startPos[0];
    dir[1] = gen->phys.newPos[1] - startPos[1];
    dir[2] = gen->phys.newPos[2] - startPos[2];
    if ( trace->fraction == 1.0f )
    {
        gen->phys.origin[0] = gen->phys.newPos[0];
        gen->phys.origin[1] = gen->phys.newPos[1];
        gen->phys.origin[2] = gen->phys.newPos[2];
        return;
    }
    if ( trace->fraction == 0.0f )
    {
        trace->normal.vec.v[0] = dir[0];
        trace->normal.vec.v[1] = dir[1];
        trace->normal.vec.v[2] = dir[2];
        const float len = Abs(dir);
        if ( len <= 0.000099999997f )
        {
            gen->phys.origin[0] = startPos[0];
            gen->phys.origin[1] = startPos[1];
            gen->phys.origin[2] = startPos[2];
            return;
        }
        trace->normal.vec.v[0] = (-1.0f / len) * trace->normal.vec.v[0];
        trace->normal.vec.v[1] = (-1.0f / len) * trace->normal.vec.v[1];
        trace->normal.vec.v[2] = (-1.0f / len) * trace->normal.vec.v[2];
    }
    if ( gen->id > flame_freeze_id )
        flame_freeze_id = gen->id;
    if ( Flame_Random(is_server) < 0.69999999 )
        gen->delete_chunk = 1;
    Flame_ClipVelocity(gen->phys.velocity, trace->normal.vec.v, newVel);
    gen->phys.velocity[0] = newVel[0];
    gen->phys.velocity[1] = newVel[1];
    gen->phys.velocity[2] = newVel[2];
    speedScale = gen->stream->flameVars->flameVar_collisionSpeedScale;
    gen->phys.velocity[0] = speedScale * gen->phys.velocity[0];
    gen->phys.velocity[1] = speedScale * gen->phys.velocity[1];
    gen->phys.velocity[2] = speedScale * gen->phys.velocity[2];
    fraction = trace->fraction;
    hitPos[0] = fraction * dir[0] + startPos[0];
    hitPos[1] = fraction * dir[1] + startPos[1];
    hitPos[2] = fraction * dir[2] + startPos[2];
    gen->phys.origin[0] = FLAME_OVERCLIP * trace->normal.vec.v[0] + hitPos[0];
    gen->phys.origin[1] = FLAME_OVERCLIP * trace->normal.vec.v[1] + hitPos[1];
    gen->phys.origin[2] = FLAME_OVERCLIP * trace->normal.vec.v[2] + hitPos[2];
}

void __cdecl Flame_Impact_Process(bool is_server, flameGeneric_s *gen, trace_t *trace, int *models, int models_count)
{
    if ( is_server )
    {
        const unsigned __int16 hitEntId = Trace_GetEntityHitId(trace);
        gentity_s *hitEnt;
        gentity_s *attacker;

        if ( hitEntId == 1022 )
            hitEnt = 0;
        else
            hitEnt = (gentity_s *)((char *)sv.gentities + sv.gentitySize * hitEntId);
        if ( hitEntId == 1022 )
            attacker = 0;
        else
            attacker = (gentity_s *)((char *)sv.gentities + gen->stream->entityNum * sv.gentitySize);
        if ( hitEnt )
        {
            const bool hitFlesh = hitEnt->s.eType == 1 || hitEnt->s.eType == 2 && !trace->sflags;
            if ( hitFlesh )
                trace->sflags = 0x700000;
            if ( attacker && attacker->s.number != hitEntId )
            {
                int flameDamage;
                float flameDamageDuration;
                float flameDamageInterval;
                float damageDir[3];
                int damageIntervalMs;

                // Port fix: decomp reads dvar floats via .current.integer; use .value here.
                if ( (float)gen->stream->damage > 0.0f )
                    flameDamage = gen->stream->damage;
                else
                    flameDamage = (int)default_flameVars_initialHitDamage->current.value;
                if ( gen->stream->damageDuration > 0.0f )
                    flameDamageDuration = gen->stream->damageDuration;
                else
                    flameDamageDuration = default_flameVars_timedDamageDuration->current.value;
                if ( gen->stream->damageInterval > 0.0f )
                    flameDamageInterval = gen->stream->damageInterval;
                else
                    flameDamageInterval = default_flameVars_timedDamageInterval->current.value;
                // Port fix: (int)0.3 == 0 makes end_timestamp gate fire every chunk.
                if ( flameDamageInterval < 1.0f )
                    flameDamageInterval = 1.0f;
                damageIntervalMs = (int)flameDamageInterval;
                Flame_trigger_damage(hitEnt, attacker, flameDamage, gen->phys.origin, 17, gen->phys.velocity);
                DynEntityDrawType drawType;
                const unsigned __int16 dynEntHitId = Trace_GetDynEntHitId(trace, &drawType);
                if ( dynEntHitId != 0xFFFF )
                {
                    damageDir[0] = 0.0f;
                    damageDir[1] = 1.0f;
                    damageDir[2] = 0.0f;
                    DynEntSv_Damage(dynEntHitId, drawType, gen->phys.origin, damageDir, flameDamage, 0);
                }
                if ( hitEnt )
                {
                    float damageVec[3];
                    const hitLocation_t hitLoc = (hitLocation_t)trace->partGroup;

                    damageVec[0] = 0.0f;
                    damageVec[1] = 1.0f;
                    damageVec[2] = 0.0f;
                    // Decomp: CoDMPServer.c:706362 — throttle direct G_Damage to damageInterval ms.
                    if ( G_GetTime() >= hitEnt->flame_timed_damage[0].end_timestamp
                        || !hitEnt->flame_timed_damage[0].end_timestamp )
                    {
                        hitEnt->flame_timed_damage[0].attacker = attacker;
                        // Decomp: CoDMPServer.c:706366 — slot damage stays 0; players never
                        // get SV_Flame_Apply_Damage, so burn HP is only applied here.
                        hitEnt->flame_timed_damage[0].damage = 0;
                        hitEnt->flame_timed_damage[0].damageDuration = flameDamageDuration;
                        hitEnt->flame_timed_damage[0].damageInterval = flameDamageInterval;
                        hitEnt->flame_timed_damage[0].start_timestamp = G_GetTime();
                        hitEnt->flame_timed_damage[0].lastupdate_timestamp = 0;
                        hitEnt->flame_timed_damage[0].end_timestamp = G_GetTime() + damageIntervalMs;
                        if ( attacker->r.inuse )
                        {
                            const int damageTime = G_GetTime();
                            G_Damage(
                                hitEnt,
                                attacker,
                                attacker,
                                damageVec,
                                gen->phys.origin,
                                flameDamage,
                                0,
                                17,
                                0xFFFFFFFF,
                                hitLoc,
                                trace->modelIndex,
                                trace->partName,
                                level.time - damageTime);
                        }
                    }
                }
            }
        }
    }
    else
    {
        if ( trace->fraction > 0.0f )
        {
            const bool isWall = fabsf(trace->normal.vec.u[2]) < 0.30000001f;
            if ( (trace->sflags & 0x3F00000) != 0x1400000 )
            {
                if ( isWall )
                {
                    CG_SetFireToWall(0, gen->phys.origin, models, models_count, TFS_FLAMETHROWER);
                }
                else
                {
                    const int weaponIndex = G_GetWeaponIndexForName((char*)"m2_flamethrower_mp");
                    CG_SetFireToTerrain(
                        gen->phys.origin,
                        1.0f,
                        gen->stream->entityNum,
                        weaponIndex,
                        1.0f,
                        models,
                        models_count,
                        TFS_FLAMETHROWER);
                }
            }
        }
        DynEntityDrawType dynDrawType;
        unsigned __int16 entityHitId = Trace_GetDynEntHitId(trace, &dynDrawType);
        if ( entityHitId != 0xFFFF )
            DynEntCl_FlameDamage(0, entityHitId, (DynEntityCollType)dynDrawType, gen->phys.origin, gen->phys.velocity, 5);
        entityHitId = Trace_GetEntityHitId(trace);
        if ( entityHitId != 1023 && entityHitId != 1022 )
        {
            centity_s *cent = CG_GetEntity(0, entityHitId);
            if ( cent->destructible )
            {
                float damageDir[3];
                damageDir[0] = 0.0f;
                damageDir[1] = 1.0f;
                damageDir[2] = 0.0f;
                CG_DestructibleDamage(cent, 0, damageDir, gen->phys.origin, gen->stream->damage, 17, 0, 0, 1);
            }
        }
    }
}

struct Flame_SortByStream
{
    bool operator()(flameGeneric_s *a, flameGeneric_s *b) const
    {
        return a->stream < b->stream;
    }
};

void __cdecl Flame_Phys_Update_Items(bool is_server)
{
    phys_static_array<flameGeneric_s *,1000> *flames;
    int nflames;

    if ( is_server )
        flames = &sv_flames;
    else
        flames = &cl_flames;
    nflames = flames->m_alloc_count;
    if ( !nflames )
        return;

    flameGeneric_s **sortBegin = flames->m_slot_array;
    flameGeneric_s **sortEnd = &flames->m_slot_array[nflames];
    std::sort(sortBegin, sortEnd, Flame_SortByStream{});

    flameStream_s *prevStream = (*sortBegin)->stream;
    int startIndex = 0;
    int count = 0;
    for ( int flameIndex = 0; flameIndex < nflames; ++flameIndex )
    {
        flameGeneric_s *flameItem = flames->m_slot_array[flameIndex];
        if ( flameItem->stream == prevStream )
        {
            ++count;
        }
        else
        {
            iassert(count > 0);
            Flame_Phys_Update_Items_PerStream(is_server, count, &flames->m_slot_array[startIndex]);
            // Port fix: retail decomp resets count to 0 and sets prevStream from
            // items[i+1], dropping the first particle of each new stream group.
            startIndex = flameIndex;
            count = 1;
            prevStream = flameItem->stream;
        }
    }
    if ( count > 0 )
        Flame_Phys_Update_Items_PerStream(is_server, count, &flames->m_slot_array[startIndex]);
    flames->m_alloc_count = 0;
}

void __cdecl Flame_Phys_Update_Items_PerStream(bool is_server, int nitems, flameGeneric_s **items)
{
    col_context_t context;
    colgeom_visitor_inlined_t<500> *proximity_cache;
    trace_t trace;
    trace_t trace2;
    float boundsMin[3];
    float boundsMax[3];
    float center[3];
    float expand_vec[3];
    float lod;
    float nearestClientDist;
    int ents[128];
    int models[256];
    int close_characters[16];
    int close_triggers[16];
    int dynEntsCount[2];
    unsigned __int16 dynEnts[2][100];
    int entsCount;
    int close_characters_count;
    int close_triggers_count;
    int models_count;
    int skip;
    int skipcounter;

    boundsMin[0] = FLT_MAX;
    boundsMin[1] = FLT_MAX;
    boundsMin[2] = FLT_MAX;
    boundsMax[0] = -FLT_MAX;
    boundsMax[1] = -FLT_MAX;
    boundsMax[2] = -FLT_MAX;
    iassert(nitems > 0);
    if ( !(*items)->stream )
        return;

    for ( int boundsIndex = 0; boundsIndex < nitems; ++boundsIndex )
    {
        flameGeneric_s *gen = items[boundsIndex];
        Vec3Min(gen->phys.origin, boundsMin, boundsMin);
        Vec3Min(gen->phys.newPos, boundsMin, boundsMin);
        Vec3Max(gen->phys.origin, boundsMax, boundsMax);
        Vec3Max(gen->phys.newPos, boundsMax, boundsMax);
    }
    boundsMin[0] = boundsMin[0] - size[0];
    boundsMin[1] = boundsMin[1] - size[1];
    boundsMin[2] = boundsMin[2] - size[2];
    boundsMax[0] = boundsMax[0] + size[0];
    boundsMax[1] = boundsMax[1] + size[1];
    boundsMax[2] = boundsMax[2] + size[2];
    Vec3Lerp(boundsMin, boundsMax, 0.5f, center);
    nearestClientDist = FLT_MAX;
    const int numLocalClients = CL_LocalClient_GetActiveCount();
    for ( int clientIndex = 0; clientIndex < numLocalClients; ++clientIndex )
    {
        cg_s *cgameGlob = CG_GetLocalClientGlobals(clientIndex);
        const float dist = Vec3Distance(cgameGlob->refdef.vieworg, center);
        nearestClientDist = dist < nearestClientDist ? dist : nearestClientDist;
    }
    lod = nearestClientDist / 1000.0f;
    if ( lod > 1.0f )
        lod = 1.0f;
    if ( lod < 0.0f )
        lod = 0.0f;
    (void)lod;
    if ( flame_debug_render && flame_debug_render->current.integer > 1 )
    {
        float origin[3] = { 0.0f, 0.0f, 0.0f };
        float clr[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        CG_DebugBox(origin, boundsMin, boundsMax, 0.0f, clr, 1, 3);
    }
    if ( is_server )
        proximity_cache = &sv_flame_chunks_proximity_cache;
    else
        proximity_cache = &cl_flame_chunks_proximity_cache;
    proximity_cache->reset();
    expand_vec[0] = 1.0f;
    expand_vec[1] = 1.0f;
    expand_vec[2] = 1.0f;
    proximity_cache->update(boundsMin, boundsMax, 0x2C0EC33, expand_vec);
    context.prims = proximity_cache->prims;
    context.nprims = proximity_cache->nprims;
    for ( int drawType = 0; drawType < 2; ++drawType )
    {
        if ( is_server )
            dynEntsCount[drawType] = DynEntSv_AreaEntities((DynEntityDrawType)drawType, boundsMin, boundsMax, -1, 0x64u, dynEnts[drawType]);
        else
            dynEntsCount[drawType] = DynEntCl_AreaEntities((DynEntityDrawType)drawType, boundsMin, boundsMax, -1, 0x64u, dynEnts[drawType]);
    }
    if ( is_server )
        entsCount = CM_AreaEntities(boundsMin, boundsMax, ents, 128, 0x2C0EC33);
    else
        entsCount = CG_AreaEntities(boundsMin, boundsMax, ents, 128, 0x2C0EC33);
    close_characters_count = 0;
    close_triggers_count = 0;
    models_count = 0;
    R_GetStaticModels(boundsMin, boundsMax, models, &models_count, 256);
    skip = 1;
    skipcounter = 1;
    memset(&trace, 0, sizeof(trace));
    memset(&trace2, 0, sizeof(trace2));
    for ( int itemIndex = 0; itemIndex < nitems; ++itemIndex )
    {
        flameGeneric_s *gen = items[itemIndex];
        if ( flame_debug_render && flame_debug_render->current.integer > 1 )
        {
            float genClr[4];
            if ( is_server )
            {
                genClr[0] = 0.0f;
                genClr[1] = 0.0f;
                genClr[2] = 1.0f;
            }
            else
            {
                genClr[0] = 1.0f;
                genClr[1] = 0.0f;
                genClr[2] = 0.0f;
            }
            genClr[3] = 1.0f;
            G_DebugStar(gen->phys.origin, genClr, 3);
        }
        --skipcounter;
        if ( !itemIndex || !skipcounter )
        {
            // Port fix: retail (CoDMPServer.c:706705) only zeroes close_characters once per
            // stream batch, so players stay in the list for every later chunk and take extra
            // proximity damage attempts. Reset each collision pass to match LOS re-check.
            close_characters_count = 0;
            close_triggers_count = 0;
            Flame_Phys_Collision(
                is_server,
                &trace,
                gen,
                &context,
                (unsigned __int16 (*)[2][100])dynEnts,
                dynEntsCount,
                ents,
                entsCount,
                close_characters,
                &close_characters_count,
                close_triggers,
                &close_triggers_count);
            memcpy(&trace2, &trace, sizeof(trace2));
            skipcounter = skip;
        }
        Flame_Phys_Collision_Response(is_server, &trace, gen);
        if ( trace.fraction < 1.0f && trace.fraction > 0.0f )
            Flame_Impact_Process(is_server, gen, &trace, models, models_count);
        for ( int characterIndex = 0; characterIndex < close_characters_count; ++characterIndex )
        {
            trace2.hitType = TRACE_HITTYPE_ENTITY;
            trace2.hitId = close_characters[characterIndex];
            trace2.fraction = 0.0f;
            trace2.cflags = 0x8000;
            trace2.modelIndex = 0;
            trace2.partName = 0;
            trace2.partGroup = 0;
            Flame_Impact_Process(is_server, gen, &trace2, models, models_count);
        }
        for ( int triggerIndex = 0; triggerIndex < close_triggers_count; ++triggerIndex )
        {
            trace2.hitType = TRACE_HITTYPE_ENTITY;
            trace2.hitId = close_triggers[triggerIndex];
            trace2.fraction = 0.0f;
            trace2.cflags = 0x8000;
            trace2.modelIndex = 0;
            trace2.partName = 0;
            trace2.partGroup = 0;
            Flame_Impact_Process(is_server, gen, &trace2, models, models_count);
        }
    }
}

void __cdecl Flame_Phys_Update_Item_Stream(flameGeneric_s *gen, int time)
{
    if ( time <= gen->age.lastUpdateTime )
        return;

    const float frametime = (float)(time - gen->age.lastUpdateTime) * 0.001f;
    gen->age.lastUpdateTime = time;
    const float age = (float)(gen->age.lastUpdateTime - gen->age.startTime);
    const float lifeFrac = Flame_CalcTimeScale(gen->age.startTime, gen->age.endTime) * age;
    if ( lifeFrac >= 1.0f )
        return;

    iassert(!IS_NAN(gen->size.rate));
    const float sizeAdd = gen->size.rate * frametime;
    gen->phys.rotation = gen->phys.rotVel * frametime + gen->phys.rotation;
    gen->size.current = gen->size.current + sizeAdd;
    iassert(!IS_NAN(gen->size.current));
    gen->phys.newPos[0] = frametime * gen->phys.velocity[0] + gen->phys.origin[0];
    gen->phys.newPos[1] = frametime * gen->phys.velocity[1] + gen->phys.origin[1];
    gen->phys.newPos[2] = frametime * gen->phys.velocity[2] + gen->phys.origin[2];
}

void __cdecl Flame_Phys_Update_Item_Chunk(
                flameGeneric_s *gen,
                int time,
                phys_static_array<flameGeneric_s *,1000> *flames)
{
    if ( time <= gen->age.lastUpdateTime )
        return;

    flameTable *vars = gen->stream->flameVars;
    const float gravityStart = vars->flameVar_streamChunkGravityStart;
    const float gravityEnd = vars->flameVar_streamChunkGravityEnd;
    const float friction = vars->flameVar_streamChunkDecel;
    const float speedScale = vars->flameVar_streamChunkSizeSpeedScale;
    const float ageScale = vars->flameVar_streamChunkSizeAgeScale;
    const float frametime = (float)(time - gen->age.lastUpdateTime) * 0.001f;
    gen->age.lastUpdateTime = time;
    const float age = (float)(gen->age.lastUpdateTime - gen->age.startTime);
    const float lifeFrac = Flame_CalcTimeScale(gen->age.startTime, gen->age.endTime) * age;
    if ( lifeFrac >= 1.0f )
        return;

    iassert(!IS_NAN(gen->size.rate));
    const float baseSizeAdd = ((1.0f - ageScale) + (2.0f * ageScale * lifeFrac)) * (gen->size.rate * frametime);
    const float speedFactor = (1.0f - Abs(gen->phys.velocity) * gen->phys.invStartSpeed) * (2.0f * speedScale) + (1.0f - speedScale);
    const float sizeAdd = speedFactor * baseSizeAdd;
    float frictionScale = 1.0f - frametime * friction;
    if ( frictionScale < 0.0f )
        frictionScale = 0.0f;

    gen->phys.rotation = gen->phys.rotVel * frametime + gen->phys.rotation;
    gen->size.current = gen->size.current + sizeAdd;
    iassert(!IS_NAN(gen->size.current));
    gen->phys.velocity[0] = frictionScale * gen->phys.velocity[0];
    gen->phys.velocity[1] = frictionScale * gen->phys.velocity[1];
    gen->phys.velocity[2] = frictionScale * gen->phys.velocity[2];
    gen->phys.gravity = (gravityEnd - gravityStart) * lifeFrac + gravityStart;
    gen->phys.velocity[2] = gen->phys.velocity[2] - frametime * gen->phys.gravity;
    gen->phys.newPos[0] = frametime * gen->phys.velocity[0] + gen->phys.origin[0];
    gen->phys.newPos[1] = frametime * gen->phys.velocity[1] + gen->phys.origin[1];
    gen->phys.newPos[2] = frametime * gen->phys.velocity[2] + gen->phys.origin[2];
    if ( flames->m_alloc_count != 1000 )
    {
        flameGeneric_s **slot;
        if ( flames->m_alloc_count < 1000 )
            slot = &flames->m_slot_array[flames->m_alloc_count++];
        else
            tlFatal("phys array add overflow.");
        *slot = gen;
    }
}

void __cdecl Flame_Phys_Update_Item_Fire(flameGeneric_s *gen, int time)
{
    if ( time <= gen->age.lastUpdateTime )
        return;

    flameTable *vars = gen->stream->flameVars;
    const float gravityStart = vars->flameVar_fireGravity;
    const float gravityEnd = vars->flameVar_fireGravityEnd;
    const float friction = vars->flameVar_fireFriction;
    const float frametime = (float)(time - gen->age.lastUpdateTime) * 0.001f;
    gen->age.lastUpdateTime = time;
    const float age = (float)(gen->age.lastUpdateTime - gen->age.startTime);
    const float lifeFrac = Flame_CalcTimeScale(gen->age.startTime, gen->age.endTime) * age;
    if ( lifeFrac >= 1.0f )
        return;

    iassert(!IS_NAN(gen->size.rate));
    const float sizeAdd = gen->size.rate * frametime;
    float frictionScale = 1.0f - frametime * friction;
    if ( frictionScale < 0.0f )
        frictionScale = 0.0f;

    gen->phys.rotation = gen->phys.rotVel * frametime + gen->phys.rotation;
    gen->size.current = gen->size.current + sizeAdd;
    iassert(!IS_NAN(gen->size.current));
    gen->phys.velocity[0] = frictionScale * gen->phys.velocity[0];
    gen->phys.velocity[1] = frictionScale * gen->phys.velocity[1];
    gen->phys.velocity[2] = frictionScale * gen->phys.velocity[2];
    gen->phys.gravity = (gravityEnd - gravityStart) * lifeFrac + gravityStart;
    gen->phys.velocity[2] = gen->phys.velocity[2] - frametime * gen->phys.gravity;
    gen->phys.newPos[0] = frametime * gen->phys.velocity[0] + gen->phys.origin[0];
    gen->phys.newPos[1] = frametime * gen->phys.velocity[1] + gen->phys.origin[1];
    gen->phys.newPos[2] = frametime * gen->phys.velocity[2] + gen->phys.origin[2];
    gen->phys.origin[0] = gen->phys.newPos[0];
    gen->phys.origin[1] = gen->phys.newPos[1];
    gen->phys.origin[2] = gen->phys.newPos[2];
}

void __cdecl Flame_Phys_Update_Item_Smoke(flameGeneric_s *gen, int time)
{
    if ( time <= gen->age.lastUpdateTime )
        return;

    flameTable *vars = gen->stream->flameVars;
    const float gravityStart = vars->flameVar_smokeGravity;
    const float gravityEnd = vars->flameVar_smokeGravityEnd;
    const float friction = vars->flameVar_smokeFriction;
    const float frametime = (float)(time - gen->age.lastUpdateTime) * 0.001f;
    gen->age.lastUpdateTime = time;
    const float age = (float)(gen->age.lastUpdateTime - gen->age.startTime);
    const float lifeFrac = Flame_CalcTimeScale(gen->age.startTime, gen->age.endTime) * age;
    if ( lifeFrac >= 1.0f )
        return;

    iassert(!IS_NAN(gen->size.rate));
    const float sizeAdd = gen->size.rate * frametime;
    float frictionScale = 1.0f - frametime * friction;
    if ( frictionScale < 0.0f )
        frictionScale = 0.0f;

    gen->phys.rotation = gen->phys.rotVel * frametime + gen->phys.rotation;
    gen->size.current = gen->size.current + sizeAdd;
    iassert(!IS_NAN(gen->size.current));
    gen->phys.velocity[0] = frictionScale * gen->phys.velocity[0];
    gen->phys.velocity[1] = frictionScale * gen->phys.velocity[1];
    gen->phys.velocity[2] = frictionScale * gen->phys.velocity[2];
    gen->phys.gravity = (gravityEnd - gravityStart) * lifeFrac + gravityStart;
    gen->phys.velocity[2] = gen->phys.velocity[2] - frametime * gen->phys.gravity;
    gen->phys.newPos[0] = frametime * gen->phys.velocity[0] + gen->phys.origin[0];
    gen->phys.newPos[1] = frametime * gen->phys.velocity[1] + gen->phys.origin[1];
    gen->phys.newPos[2] = frametime * gen->phys.velocity[2] + gen->phys.origin[2];
    gen->phys.origin[0] = gen->phys.newPos[0];
    gen->phys.origin[1] = gen->phys.newPos[1];
    gen->phys.origin[2] = gen->phys.newPos[2];
}

void __cdecl Flame_Phys_Update_Item_Drip(
                flameGeneric_s *gen,
                int time,
                phys_static_array<flameGeneric_s *,1000> *flames)
{
    if ( time <= gen->age.lastUpdateTime )
        return;

    flameTable *vars = gen->stream->flameVars;
    const float gravityStart = vars->flameVar_dripsGravity;
    const float gravityEnd = vars->flameVar_dripsGravityEnd;
    const float friction = vars->flameVar_dripsFriction;
    const float frametime = (float)(time - gen->age.lastUpdateTime) * 0.001f;
    gen->age.lastUpdateTime = time;
    const float age = (float)(gen->age.lastUpdateTime - gen->age.startTime);
    const float lifeFrac = Flame_CalcTimeScale(gen->age.startTime, gen->age.endTime) * age;
    if ( lifeFrac >= 1.0f )
        return;

    iassert(!IS_NAN(gen->size.rate));
    const float sizeAdd = gen->size.rate * frametime;
    float frictionScale = 1.0f - frametime * friction;
    if ( frictionScale < 0.0f )
        frictionScale = 0.0f;

    gen->phys.rotation = gen->phys.rotVel * frametime + gen->phys.rotation;
    gen->size.current = gen->size.current + sizeAdd;
    iassert(!IS_NAN(gen->size.current));
    gen->phys.velocity[0] = frictionScale * gen->phys.velocity[0];
    gen->phys.velocity[1] = frictionScale * gen->phys.velocity[1];
    gen->phys.velocity[2] = frictionScale * gen->phys.velocity[2];
    gen->phys.gravity = (gravityEnd - gravityStart) * lifeFrac + gravityStart;
    gen->phys.velocity[2] = gen->phys.velocity[2] - frametime * gen->phys.gravity;
    gen->phys.newPos[0] = frametime * gen->phys.velocity[0] + gen->phys.origin[0];
    gen->phys.newPos[1] = frametime * gen->phys.velocity[1] + gen->phys.origin[1];
    gen->phys.newPos[2] = frametime * gen->phys.velocity[2] + gen->phys.origin[2];
    if ( flames->m_alloc_count != 1000 )
    {
        flameGeneric_s **slot;
        if ( flames->m_alloc_count < 1000 )
            slot = &flames->m_slot_array[flames->m_alloc_count++];
        else
            tlFatal("phys array add overflow.");
        *slot = gen;
    }
}
