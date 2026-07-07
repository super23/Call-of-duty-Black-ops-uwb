#include "flame_class_stream.h"
#include "flame_class_drips.h"
#include <gfx_d3d/r_scene.h>
#include "flame_cull.h"
#include "flame_class_chunk.h"
#include <qcommon/common.h>
#include <cgame/cg_scr_main.h>
#include <demo/demo_playback.h>
#include <EffectsCore/fx_update.h>
#include <universal/CurveManager.h>
#include <EffectsCore/fx_beam.h>
#include <universal/assertive.h>
#include <universal/com_math_anglevectors.h>

static void Flame_Class_Stream_FreeChunks(bool is_server, flameStream_s *stream)
{
    while ( stream->chunkList )
        Flame_Class_Chunk_Free(is_server, stream->chunkList);
    Flame_Class_Drips_Free_If_Stream(is_server, stream);
}

flameStream_s *flameStreamsFree;
flameStream_s *flameStreamsUsed;

flameStream_s flameStreams[64];
flameRenderList_s flameStreamRenderList[64];

flameStream_s sv_flameStreams[64];
flameStream_s *sv_flameStreamsFree;
flameStream_s *sv_flameStreamsUsed;

int g_StreamCount;
int g_StreamCountWaterMark;

void __cdecl Flame_Class_Stream_Init()
{
    int streamIndex;

    memset(flameStreams, 0, sizeof(flameStreams));
    Flame_List_Init((flameGeneric_s*)&flameStreams[0], 140, 64);
    flameStreamsFree = (flameStream_s *)&flameStreams;
    flameStreamsUsed = 0;
    for ( streamIndex = 0; streamIndex < 64; ++streamIndex )
        flameStreams[streamIndex].renderList = &flameStreamRenderList[streamIndex];

    memset((void *)&sv_flameStreams[0], 0, sizeof(sv_flameStreams));
    Flame_List_Init(&sv_flameStreams[0].gen, 140, 64);
    sv_flameStreamsFree = sv_flameStreams;
    sv_flameStreamsUsed = 0;
    ++g_StreamCount;
}

flameStream_s *__cdecl Flame_Class_Stream_Alloc(bool is_server)
{
    flameStream_s *freeStream;

    if ( is_server )
        freeStream = sv_flameStreamsFree;
    else
        freeStream = flameStreamsFree;
    if ( freeStream )
    {
        freeStream->chunkList = 0;
        if ( !is_server )
            freeStream->renderList->flameRend = 0;
        freeStream->flameVars = 0;
        if ( is_server )
        {
            Flame_List_Move_Global(
                &freeStream->gen,
                (flameGeneric_s **)&sv_flameStreamsFree,
                (flameGeneric_s **)&sv_flameStreamsUsed);
        }
        else
        {
            Flame_List_Move_Global(
                &freeStream->gen,
                (flameGeneric_s **)&flameStreamsFree,
                (flameGeneric_s **)&flameStreamsUsed);
            if ( ++g_StreamCount > g_StreamCountWaterMark )
                g_StreamCountWaterMark = g_StreamCount;
        }
    }
    return freeStream;
}

void __cdecl Flame_Class_Stream_Free(bool is_server, flameStream_s *flameStream)
{
    flameSource_t *source;

    Flame_Class_Stream_FreeChunks(is_server, flameStream);
    if ( is_server )
    {
        Flame_Class_Drips_Free_If_Stream(is_server, flameStream);
        source = SV_Flame_Source_Get(flameStream->entityNum);
        if ( source->currentStream == flameStream )
            source->currentStream = 0;
        Flame_List_Move_Global(
            &flameStream->gen,
            (flameGeneric_s **)&sv_flameStreamsUsed,
            (flameGeneric_s **)&sv_flameStreamsFree);
    }
    else
    {
        source = Flame_Source_Get(flameStream->entityNum);
        if ( source->currentStream == flameStream )
            source->currentStream = 0;
        Flame_List_Move_Global(
            &flameStream->gen,
            (flameGeneric_s **)&flameStreamsUsed,
            (flameGeneric_s **)&flameStreamsFree);
        --g_StreamCount;
    }
}

void __cdecl Flame_Class_Stream_Light_Chunks(const flameStream_s *stream)
{
    float color_fade;
    float radius_flutter;
    float blue_flutter;
    int point_count;
    float median_point[3];
    float red_flutter;
    const flameChunk_s *chunk;
    float green_flutter;
    float dummyAxis[3][3];
    const flameChunk_s *nextChunk;

    memset(dummyAxis, 0, sizeof(dummyAxis));
    point_count = 0;
    memset(median_point, 0, sizeof(median_point));
    for ( chunk = stream->chunkList; chunk; chunk = nextChunk )
    {
        nextChunk = (const flameChunk_s *)chunk->gen.listLocal.next;
        median_point[0] = median_point[0] + chunk->gen.phys.origin[0];
        median_point[1] = median_point[1] + chunk->gen.phys.origin[1];
        median_point[2] = median_point[2] + chunk->gen.phys.origin[2];
        ++point_count;
    }
    if ( point_count > 0 )
    {
        median_point[0] = median_point[0] / (float)point_count;
        median_point[1] = median_point[1] / (float)point_count;
        median_point[2] = median_point[2] / (float)point_count;
        if ( point_count == 1 || point_count == 2 )
            color_fade = 0.2f;
        else
            color_fade = 1.0f;
        radius_flutter = Flame_CRandom(0) * stream->flameVars->flameVar_streamPrimaryLightRadiusFlutter;
        red_flutter = Flame_CRandom(0) * stream->flameVars->flameVar_streamPrimaryLightFlutterR;
        // Retail swaps G/B flutter vars when writing blue/green light channels.
        blue_flutter = Flame_CRandom(0) * stream->flameVars->flameVar_streamPrimaryLightFlutterG;
        green_flutter = Flame_CRandom(0) * stream->flameVars->flameVar_streamPrimaryLightFlutterB;
        R_AddOmniLightToScene(
            median_point,
            dummyAxis,
            stream->flameVars->flameVar_streamPrimaryLightRadius + radius_flutter,
            (stream->flameVars->flameVar_streamPrimaryLightR + red_flutter) * color_fade,
            (stream->flameVars->flameVar_streamPrimaryLightG + green_flutter) * color_fade,
            (stream->flameVars->flameVar_streamPrimaryLightB + blue_flutter) * color_fade);
    }
}

void __cdecl Flame_Class_Stream_Age(bool is_server)
{
    flameStream_s *stream;
    flameStream_s *nextStream;

    stream = is_server ? sv_flameStreamsUsed : flameStreamsUsed;
    while ( stream )
    {
        nextStream = (flameStream_s *)stream->gen.listGlobal.next;
        Flame_Cull_Stream_Chunks(is_server, stream);
        if ( !stream->chunkList )
            Flame_Class_Stream_Free(is_server, stream);
        stream = nextStream;
    }
}

void __cdecl Flame_Class_Stream_Fire_Chunks(
                flameSource_t *source,
                flameWeaponConfig_t *weaponConfig,
                int inTime,
                flameRender_s *flameRend)
{
    float lerpFrac;
    float sizeRand;
    int totalTime;
    flameChunk_s *newChunk;
    bool spawnSmoke;
    int chunkTime;
    float diffAngleYaw;
    float diffAnglePitch;
    float diffAngleRoll;
    int scaledDuration;
    flameChunkSpawnVars_t spawnVars;
    float velocityAdd[3];
    flameStream_s *stream;
    int stepTime;
    int lastTime;
    flameTable *fTable;
    float diffOrigin[3];

    newChunk = 0;
    spawnSmoke = 0;
    fTable = weaponConfig->fTable;
    if ( flame_use_dvars && flame_use_dvars->current.enabled )
    {
        fTable = Flame_FindFlameTable(flameVar_editingFlameTable->current.string);
        Flame_DVarsToFlameVars(fTable);
        Com_Printf(16, "FLAMETHROWER TEST ACTIVE\n%s\n", flameVar_editingFlameTable->current.string);
    }
    memset(diffOrigin, 0, sizeof(diffOrigin));
    diffAngleYaw = 0.0f;
    diffAnglePitch = 0.0f;
    diffAngleRoll = 0.0f;
    if ( (weaponConfig->bIsFiring == source->bIsFiring && source->currentStream)
        || (source->currentStream = Flame_Class_Stream_Alloc(source->is_server_alloc)) != 0 )
    {
        stream = source->currentStream;
        stream->gen.stream = stream;
        stream->gen.type = 0;
        stream->entityNum = source->entityNum;
        stream->flameVars = fTable;
        if ( !source->is_server_alloc )
            stream->renderList->flameRend = flameRend;
        stream->damage = weaponConfig->damage;
        stream->damageDuration = weaponConfig->damageDuration;
        stream->damageInterval = weaponConfig->damageInterval;
        stream->isKillcamFlame = source->isKillcamFlame;
        stream->firingThroughGeo = source->firingThroughGeo;
        stepTime = (int)(float)((float)((float)(fTable->flameVar_streamChunkStartSize
                                                                                    * fTable->flameVar_streamChunkDistScalar)
                                                                    * 1000.0)
                                                    / fTable->flameVar_streamChunkSpeed);
        if ( !stepTime )
            stepTime = 1;
        scaledDuration = (int)(float)(fTable->flameVar_streamChunkDuration * 1000.0);
        if ( stream->chunkList )
        {
            diffOrigin[0] = weaponConfig->origin[0] - source->origin[0];
            diffOrigin[1] = weaponConfig->origin[1] - source->origin[1];
            diffOrigin[2] = weaponConfig->origin[2] - source->origin[2];
            diffAngleYaw = AngleNormalize180(weaponConfig->angle[0] - source->angle[0]);
            diffAnglePitch = AngleNormalize180(weaponConfig->angle[1] - source->angle[1]);
            diffAngleRoll = AngleNormalize180(weaponConfig->angle[2] - source->angle[2]);
            lastTime = source->lastUsedTime;
            chunkTime = stepTime + lastTime;
            if ( weaponConfig->entityOrigin[0] == source->entityOrigin[0]
                && weaponConfig->entityOrigin[1] == source->entityOrigin[1]
                && weaponConfig->entityOrigin[2] == source->entityOrigin[2] )
            {
                spawnSmoke = 1;
            }
            memset(velocityAdd, 0, sizeof(velocityAdd));
        }
        else
        {
            lastTime = inTime;
            chunkTime = inTime;
            memset(velocityAdd, 0, sizeof(velocityAdd));
        }
        totalTime = inTime - lastTime;
        while ( chunkTime <= inTime )
        {
            // Retail only spawns chunks within 25ms of the current frame; older
            // catch-up slots are skipped to limit burst density after lag spikes.
            if ( inTime - chunkTime < 25 )
            {
                spawnVars.time = chunkTime;
                spawnVars.duration = scaledDuration;
                if ( chunkTime >= inTime )
                {
                    spawnVars.origin[0] = weaponConfig->origin[0];
                    spawnVars.origin[1] = weaponConfig->origin[1];
                    spawnVars.origin[2] = weaponConfig->origin[2];
                    spawnVars.angle[0] = weaponConfig->angle[0];
                    spawnVars.angle[1] = weaponConfig->angle[1];
                    spawnVars.angle[2] = weaponConfig->angle[2];
                }
                else
                {
                    lerpFrac = 1.0f - (float)((float)(inTime - chunkTime) / (float)totalTime);
                    spawnVars.origin[0] = lerpFrac * diffOrigin[0] + source->origin[0];
                    spawnVars.origin[1] = lerpFrac * diffOrigin[1] + source->origin[1];
                    spawnVars.origin[2] = lerpFrac * diffOrigin[2] + source->origin[2];
                    spawnVars.angle[0] = lerpFrac * diffAngleYaw + source->angle[0];
                    spawnVars.angle[1] = lerpFrac * diffAnglePitch + source->angle[1];
                    spawnVars.angle[2] = lerpFrac * diffAngleRoll + source->angle[2];
                }
                spawnVars.speed = fTable->flameVar_streamChunkSpeed;
                spawnVars.decel = fTable->flameVar_streamChunkDecel;
                spawnVars.gravityStart = fTable->flameVar_streamChunkGravityStart;
                spawnVars.gravityEnd = fTable->flameVar_streamChunkGravityEnd;
                sizeRand = Flame_SwayRand(fTable->flameVar_streamSizeRandSinWave, fTable->flameVar_streamSizeRandCosWave, chunkTime)
                            * 0.5f
                            + 0.5f;
                spawnVars.sizeStart = (1.0f - sizeRand * fTable->flameVar_streamChunkStartSizeRand)
                                                        * fTable->flameVar_streamChunkStartSize;
                spawnVars.sizeEnd = (1.0f - sizeRand * fTable->flameVar_streamChunkEndSizeRand)
                                                    * fTable->flameVar_streamChunkEndSize;
                spawnVars.sizeMax = fTable->flameVar_streamChunkMaxSize;
                spawnVars.sizeRate = (spawnVars.sizeEnd - spawnVars.sizeStart) / (float)scaledDuration;
                newChunk = Flame_Class_Chunk_Spawn(
                                         source->is_server_alloc,
                                         &spawnVars,
                                         (flameGeneric_s **)&stream->chunkList,
                                         velocityAdd,
                                         fTable);
                if ( newChunk )
                {
                    newChunk->gen.stream = stream;
                    if ( spawnSmoke
                        && chunkTime - stream->lastSmokeChunkTime > (int)(1000.0f * fTable->flameVar_streamSmokeChunkInterval) )
                    {
                        newChunk->spawnSmokeInterval = (int)((Flame_Random(source->is_server_alloc) * fTable->flameVar_streamSmokeChunkRandFrac
                                                                                                + fTable->flameVar_streamSmokeChunkMinFrac)
                                                                                             * 1000.0);
                        stream->lastSmokeChunkTime = chunkTime;
                    }
                    if ( chunkTime - stream->lastDripChunkTime > (int)(1000.0f * fTable->flameVar_streamDripsChunkInterval) )
                    {
                        newChunk->spawnDripsInterval = (int)((Flame_Random(source->is_server_alloc) * fTable->flameVar_streamDripsChunkRandFrac
                                                                                                + fTable->flameVar_streamDripsChunkMinFrac)
                                                                                             * 1000.0);
                        stream->lastDripChunkTime = chunkTime;
                    }
                    Flame_Phys_Update_Item_Stream(&newChunk->gen, inTime);
                }
            }
            lastTime = chunkTime;
            chunkTime += stepTime;
        }
        Flame_Phys_Update_Items(source->is_server_alloc);
        if ( newChunk )
        {
            source->origin[0] = spawnVars.origin[0];
            source->origin[1] = spawnVars.origin[1];
            source->origin[2] = spawnVars.origin[2];
            source->angle[0] = spawnVars.angle[0];
            source->angle[1] = spawnVars.angle[1];
            source->angle[2] = spawnVars.angle[2];
            source->entityOrigin[0] = weaponConfig->entityOrigin[0];
            source->entityOrigin[1] = weaponConfig->entityOrigin[1];
            source->entityOrigin[2] = weaponConfig->entityOrigin[2];
            source->lastUsedTime = lastTime;
        }
    }
}

void __cdecl Flame_Class_Stream_Render_Item(
                int localClientNum,
                const flameStream_s *stream,
                Material *material,
                Material *material2,
                float (*verts)[3],
                bool isFire)
{
    float beamLengthSq;
    float animLoopTimeMs;
    float dist;
    float lastDist;
    signed int dobjHandle;
    unsigned __int8 boneIndex;
    cg_s *clientGlobals;
    float minDist;
    const flameSource_t *source;
    int numSegments;
    FxBeam beam;
    orientation_t orient;
    int segCount;
    const flameChunk_s *chunk;
    float segDist;
    int curve;

    beamLengthSq = 0.0f;
    if ( !material || !material2 )
        return;

    source = Flame_Source_Get(stream->entityNum);
    if ( !source )
        return;

    numSegments = isFire
                            ? (int)stream->flameVars->flameVar_streamFlameNumSegments
                            : (int)stream->flameVars->flameVar_streamFuelNumSegments;
    if ( source->currentStream != stream )
        return;

    chunk = stream->chunkList;
    if ( !chunk )
        return;

    if ( isFire )
    {
        beam.beginRadius = stream->flameVars->flameVar_streamFlameSizeStart;
        beam.endRadius = stream->flameVars->flameVar_streamFlameSizeEnd;
    }
    else
    {
        beam.beginRadius = stream->flameVars->flameVar_streamFuelSizeStart;
        beam.endRadius = stream->flameVars->flameVar_streamFuelSizeEnd;
    }
    dobjHandle = source->firstDobjHandle;
    boneIndex = source->firstBoneIndex;
    clientGlobals = CG_GetLocalClientGlobals(localClientNum);
    if ( !CG_IsLocalPlayer(source->entityNum)
        || clientGlobals->nextSnap->ps.otherFlags & 2 && clientGlobals->renderingThirdPerson == TP_FOR_MODEL
        || Demo_IsThirdPersonCamera()
        || Demo_IsMovieCamera() )
    {
        dobjHandle = source->thirdDobjHandle;
        boneIndex = source->thirdBoneIndex;
    }
    if ( FX_GetBoneOrientation(localClientNum, dobjHandle, boneIndex, &orient) )
    {
        beam.begin[0] = orient.origin[0];
        beam.begin[1] = orient.origin[1];
        beam.begin[2] = orient.origin[2];
        beam.end[0] = orient.origin[0];
        beam.end[1] = orient.origin[1];
        beam.end[2] = orient.origin[2];
    }
    else
    {
        // CoDMPServer.c:705298-705305 — zero begin/end when bone lookup fails (not FxBeam memset).
        beam.begin[0] = 0.0f;
        beam.begin[1] = 0.0f;
        beam.begin[2] = 0.0f;
        beam.end[0] = 0.0f;
        beam.end[1] = 0.0f;
        beam.end[2] = 0.0f;
    }
    curve = cCurveManager::GetFreeCurve();
    cCurveManager::AddNodeToCurve(curve, beam.begin);

    if ( isFire )
        beamLengthSq = stream->flameVars->flameVar_streamFlameLength * stream->flameVars->flameVar_streamFlameLength;
    else
        beamLengthSq = stream->flameVars->flameVar_streamFuelLength * stream->flameVars->flameVar_streamFuelLength;
    minDist = 144.0f;
    if ( dobjHandle < 32 )
        minDist = 14400.0f;
    segCount = 1;
    do
    {
        dist = Vec3DistanceSq(beam.begin, chunk->gen.phys.origin);
        lastDist = Vec3DistanceSq(beam.end, chunk->gen.phys.origin);
        if ( lastDist > 1.0f
            && (dist > minDist && beamLengthSq > dist || !chunk->gen.listLocal.next && beamLengthSq > dist) )
        {
            beam.end[0] = chunk->gen.phys.origin[0];
            beam.end[1] = chunk->gen.phys.origin[1];
            beam.end[2] = chunk->gen.phys.origin[2];
            cCurveManager::AddNodeToCurve(curve, beam.end);
            ++segCount;
        }
        chunk = (const flameChunk_s *)chunk->gen.listLocal.next;
    }
    while ( chunk && segCount < 100 );
    cCurveManager::SetCurveBSpline(curve);
    cCurveManager::SortCurve(curve, beam.begin, 1);
    cCurveManager::BuildCurve(curve);
    segCount = 0;
    segDist = 0.0f;
    while ( segCount < numSegments )
    {
        cCurveManager::GetPos(curve, segDist, &(*verts)[3 * segCount]);
        iassert(!IS_NAN((*verts)[3 * segCount])
            && !IS_NAN((*verts)[3 * segCount + 1])
            && !IS_NAN((*verts)[3 * segCount + 2]));
        ++segCount;
        segDist = segDist + 1.0f / (float)numSegments;
    }
    beam.beginColor.packed = -1;
    beam.endColor.packed = -1;
    beam.material = material;
    beam.material2 = material2;
    beam.segmentVerts = (float *)verts;
    beam.segmentCount = numSegments - 1; // CoDMPServer.c:705362 — v29 = numSegments - 1
    beam.perpSegmentCount = 1;           // CoDMPServer.c:705363 — v30 = 1
    beam.wiggleDist = 0.0f;              // CoDMPServer.c:705364 — v31 = 0
    beam.flags = 14;                     // CoDMPServer.c:705365 — v33 = 14 (bits 2|4|8)
    if ( isFire )
        animLoopTimeMs = 1000.0f * stream->flameVars->flameVar_streamFlameAnimLoopTime;
    else
        animLoopTimeMs = 1000.0f * stream->flameVars->flameVar_streamFuelAnimLoopTime;
    beam.animFrac = (float)(source->lastUsedTime % (int)animLoopTimeMs) / (float)(int)animLoopTimeMs;
    // CoDMPServer.c:705366-705368 — v34 = animFrac; FX_Beam_Add queues for FX_Beam_GenerateVerts.
    FX_Beam_Add(&beam);
    cCurveManager::FreeCurve(curve);
}

void __cdecl Flame_Class_Stream_Render_All(int localClientNum)
{
    const flameStream_s *stream;

    for ( stream = flameStreamsUsed; stream; stream = (const flameStream_s *)stream->gen.listGlobal.next )
    {
        if ( stream->flameVars->flameVar_streamFuelLength > 0.1f )
            Flame_Class_Stream_Render_Item(
                localClientNum,
                stream,
                stream->flameVars->streamFuel,
                stream->flameVars->streamFuel2,
                stream->renderList->fuelVerts,
                0);
        if ( stream->flameVars->flameVar_streamFlameLength > 0.1f )
            Flame_Class_Stream_Render_Item(
                localClientNum,
                stream,
                stream->flameVars->streamFlame,
                stream->flameVars->streamFlame2,
                stream->renderList->flameVerts,
                1);
    }
}

void __cdecl CG_Flame_Render()
{
    const flameStream_s *stream;

    PROF_SCOPED("CG_Flame_Render");

    stream = flameStreamsUsed;
    if ( flameStreamsUsed && !Flame_GetLocalClientSourceRange() )
        return;

    while ( stream )
    {
        if ( stream->flameVars->flameVar_streamFlameLength > 0.1f )
            Flame_Class_Stream_Light_Chunks(stream);
        stream = (const flameStream_s *)stream->gen.listGlobal.next;
    }
}
