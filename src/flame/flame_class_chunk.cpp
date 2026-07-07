#include "flame_class_chunk.h"
#include "flame_class_stream.h"
#include "flame_class_fire.h"
#include "flame_class_drips.h"
#include "flame_class_smoke.h"
#include <universal/assertive.h>
#include <universal/com_math_anglevectors.h>
#include <cgame_mp/cg_local_mp.h>
#include <cgame/cg_drawtools.h>

flameChunk_s flameChunks[800];
flameChunk_s *flameChunksFree;
flameChunk_s *flameChunksUsed;

flameChunk_s sv_flameChunks[800];
flameChunk_s *sv_flameChunksFree;
flameChunk_s *sv_flameChunksUsed;

int g_ChunkCount;
int g_ChunkCountWaterMark;

void __cdecl Flame_Class_Chunk_Init()
{
    memset(flameChunks, 0, sizeof(flameChunks));
    Flame_List_Init((flameGeneric_s*)flameChunks, 112, 800);
    flameChunksFree = flameChunks;
    flameChunksUsed = 0;

    memset(sv_flameChunks, 0, sizeof(sv_flameChunks));
    Flame_List_Init((flameGeneric_s *)sv_flameChunks, 112, 800);
    sv_flameChunksFree = sv_flameChunks;
    sv_flameChunksUsed = 0;

    ++g_ChunkCount;
}

flameChunk_s *__cdecl Flame_Class_Chunk_Alloc(bool is_server)
{
    flameChunk_s *freeChunk;

    if ( is_server )
        freeChunk = sv_flameChunksFree;
    else
        freeChunk = flameChunksFree;
    if ( !freeChunk )
        return 0;
    if ( is_server )
    {
        Flame_List_Move_Global(
            &freeChunk->gen,
            (flameGeneric_s **)&sv_flameChunksFree,
            (flameGeneric_s **)&sv_flameChunksUsed);
    }
    else
    {
        Flame_List_Move_Global(&freeChunk->gen, (flameGeneric_s **)&flameChunksFree, (flameGeneric_s **)&flameChunksUsed);
        if ( ++g_ChunkCount > g_ChunkCountWaterMark )
            g_ChunkCountWaterMark = g_ChunkCount;
    }
    return freeChunk;
}

void __cdecl Flame_Class_Chunk_Free(bool is_server, flameChunk_s *chunk)
{
    if ( chunk->gen.stream )
        Flame_List_Sub_Local(&chunk->gen, (flameGeneric_s **)&chunk->gen.stream->chunkList);
    if ( is_server )
    {
        Flame_List_Move_Global(&chunk->gen, (flameGeneric_s **)&sv_flameChunksUsed, (flameGeneric_s **)&sv_flameChunksFree);
    }
    else
    {
        Flame_List_Move_Global(&chunk->gen, (flameGeneric_s **)&flameChunksUsed, (flameGeneric_s **)&flameChunksFree);
        --g_ChunkCount;
    }
    Flame_Item_Init(&chunk->gen, sizeof(flameChunk_s));
}

void __cdecl Flame_Phys_Spawn_Items(bool is_server, int curTime, flameChunk_s *chunk)
{
    int fireLifeMaxMs;
    int fireLifeMinMs;
    float lifeFrac;

    lifeFrac = (float)(chunk->gen.age.lastUpdateTime - chunk->gen.age.startTime)
        / (float)(chunk->gen.age.endTime - chunk->gen.age.startTime);
    if ( chunk->spawnFireInterval && curTime >= chunk->spawnFireInterval + chunk->lastSpawnFire )
    {
        fireLifeMinMs = (int)(1000.0f * chunk->gen.stream->flameVars->flameVar_streamChunkFireMinLifeFracStart);
        fireLifeMaxMs = (int)(1000.0f * chunk->gen.stream->flameVars->flameVar_streamChunkFireMinLifeFracEnd);
        if ( chunk->gen.age.lastUpdateTime - chunk->gen.age.startTime >= fireLifeMinMs )
        {
            if ( chunk->gen.age.lastUpdateTime - chunk->gen.age.startTime <= fireLifeMaxMs )
                Flame_Class_Fire_Spawn(
                    is_server,
                    chunk,
                    curTime,
                    (1.0f - (float)((chunk->gen.age.lastUpdateTime - chunk->gen.age.startTime - fireLifeMinMs) / (fireLifeMaxMs - fireLifeMinMs)))
                        * chunk->gen.stream->flameVars->flameVar_streamChunkFireMinLifeFrac);
            else
                Flame_Class_Fire_Spawn(is_server, chunk, curTime, 0.0f);
        }
        else
        {
            Flame_Class_Fire_Spawn(is_server, chunk, curTime, chunk->gen.stream->flameVars->flameVar_streamChunkFireMinLifeFrac);
        }
        chunk->lastSpawnFire += chunk->spawnFireInterval;
        chunk->spawnFireInterval = (int)(Flame_Random(is_server)
            * ((chunk->gen.stream->flameVars->flameVar_streamChunkSpawnFireIntervalEnd
                - chunk->gen.stream->flameVars->flameVar_streamChunkSpawnFireIntervalStart)
                * lifeFrac
                + chunk->gen.stream->flameVars->flameVar_streamChunkSpawnFireIntervalStart)
            * 1000.0);
    }
    if ( chunk->spawnDripsInterval && chunk->gen.age.lastUpdateTime - chunk->gen.age.startTime > chunk->spawnDripsInterval )
    {
        Flame_Class_Drips_Spawn(is_server, chunk, curTime, 0.0f);
        chunk->spawnDripsInterval = 0;
    }
    if ( !is_server
        && chunk->spawnSmokeInterval
        && chunk->gen.age.lastUpdateTime - chunk->gen.age.startTime > chunk->spawnSmokeInterval )
    {
        Flame_Class_Smoke_Spawn(chunk, curTime);
        chunk->spawnSmokeInterval = 0;
    }
}

void __cdecl Flame_Class_Chunk_Age(bool is_server, int time)
{
    phys_static_array<flameGeneric_s *,1000> *flamesArray;
    flamePhysics_t physBackup;
    flameChunk_s *nextChunk;
    int curTime;
    flameChunk_s *chunk;

    chunk = is_server ? sv_flameChunksUsed : flameChunksUsed;
    while ( chunk )
    {
        nextChunk = (flameChunk_s *)chunk->gen.listGlobal.next;
        curTime = time;
        if ( chunk->gen.age.lastUpdateTime < chunk->gen.age.endTime
            && chunk->spawnFireInterval
            && curTime > chunk->spawnFireInterval + chunk->lastSpawnFire )
        {
            curTime = chunk->spawnFireInterval + chunk->lastSpawnFire;
        }
        if ( chunk->gen.delete_chunk
            || chunk->gen.age.lastUpdateTime - chunk->gen.age.startTime >= (int)(1000.0f
                * chunk->gen.stream->flameVars->flameVar_streamChunkSpawnFireMaxLifeFrac) )
        {
            Flame_Class_Chunk_Free(is_server, chunk);
        }
        else
        {
            while ( curTime <= time )
            {
                if ( chunk->gen.age.lastUpdateTime - chunk->gen.age.startTime < (int)(1000.0f
                    * chunk->gen.stream->flameVars->flameVar_streamChunkSpawnFireMaxLifeFrac) )
                {
                    if ( curTime == time )
                    {
                        flamesArray = is_server ? &sv_flames : &cl_flames;
                        Flame_Phys_Update_Item_Chunk(&chunk->gen, curTime, flamesArray);
                        Flame_Phys_Spawn_Items(is_server, curTime, chunk);
                    }
                    else
                    {
                        // Intermediate fire-spawn substep: run physics/spawn at curTime,
                        // then restore only gen.phys (0x34 bytes at chunk start).
                        memcpy(&physBackup, &chunk->gen.phys, sizeof(physBackup));
                        flamesArray = is_server ? &sv_flames : &cl_flames;
                        Flame_Phys_Update_Item_Chunk(&chunk->gen, curTime, flamesArray);
                        Flame_Phys_Spawn_Items(is_server, curTime, chunk);
                        memcpy(&chunk->gen.phys, &physBackup, sizeof(physBackup));
                    }
                }
                if ( curTime == time )
                    break;
                curTime = time;
            }
        }
        chunk = nextChunk;
    }
    Flame_Phys_Update_Items(is_server);
}

flameChunk_s *__cdecl Flame_Class_Chunk_Spawn(
                bool is_server,
                flameChunkSpawnVars_t *spawnVars,
                flameGeneric_s **chunkList,
                float *velocityAdd,
                flameTable *fTable)
{
    int minFireLifeOffsetMs;
    float speed;
    flameChunk_s *chunk;

    chunk = Flame_Class_Chunk_Alloc(is_server);
    if ( !chunk )
        return 0;
    chunk->gen.type = 1;
    chunk->gen.id = flame_spawn_id++;
    chunk->gen.delete_chunk = 0;
    chunk->gen.age.startTime = spawnVars->time;
    chunk->gen.age.endTime = spawnVars->duration + spawnVars->time;
    chunk->gen.age.lastUpdateTime = spawnVars->time;
    chunk->gen.size.current = spawnVars->sizeStart;
    chunk->gen.size.rate = spawnVars->sizeRate;
    chunk->gen.phys.origin[0] = spawnVars->origin[0];
    chunk->gen.phys.origin[1] = spawnVars->origin[1];
    chunk->gen.phys.origin[2] = spawnVars->origin[2];
    AngleVectors(spawnVars->angle, chunk->gen.phys.velocity, 0, 0);
    speed = spawnVars->speed;
    chunk->gen.phys.velocity[0] = speed * chunk->gen.phys.velocity[0];
    chunk->gen.phys.velocity[1] = speed * chunk->gen.phys.velocity[1];
    chunk->gen.phys.velocity[2] = speed * chunk->gen.phys.velocity[2];
    chunk->gen.phys.velocity[0] = chunk->gen.phys.velocity[0] + velocityAdd[0];
    chunk->gen.phys.velocity[1] = chunk->gen.phys.velocity[1] + velocityAdd[1];
    chunk->gen.phys.velocity[2] = chunk->gen.phys.velocity[2] + velocityAdd[2];
    chunk->gen.phys.invStartSpeed = Flame_CalcInvStartSpeed(1.0f, spawnVars->speed);
    chunk->gen.phys.gravity = spawnVars->gravityStart;
    chunk->gen.phys.rotation = Flame_CRandom(is_server) * fTable->flameVar_streamChunkRotationRange;
    chunk->gen.phys.rotVel = 0.0f;
    chunk->spawnFireInterval = (int)(1000.0f * fTable->flameVar_streamChunkSpawnFireIntervalStart);
    minFireLifeOffsetMs = (int)(1000.0f * fTable->flameVar_streamChunkSpawnFireMinLifeFrac);
    chunk->lastSpawnFire = spawnVars->time - (int)(Flame_Random(is_server) * (double)chunk->spawnFireInterval) + minFireLifeOffsetMs;
    chunk->spawnDripsInterval = 0;
    chunk->spawnSmokeInterval = 0;
    if ( chunkList )
        Flame_List_Add_Local(&chunk->gen, chunkList);
    return chunk;
}

void __cdecl Flame_Class_Chunk_Render_Item(int localClientNum, flameChunk_s *chunk)
{
    float halfSize;
    float negHalfSize;
    float pointOffsetRight;
    float pointOffsetDown;
    float fwd[3];
    float points[4][2];
    float right[3];
    float start[3];
    float end[3];
    float angles[3];
    float down[3];
    int cornerIndex;
    cg_s *clientGlobals;

    clientGlobals = CG_GetLocalClientGlobals(localClientNum);
    iassert(clientGlobals);

    if ( flame_debug_render->current.integer > 0 )
    {
        points[0][0] = -1.0f;
        points[0][1] = -1.0f;

        points[1][0] = -1.0f;
        points[1][1] = 1.0f;

        points[2][0] = 1.0f;
        points[2][1] = 1.0f;

        points[3][0] = 1.0f;
        points[3][1] = -1.0f;

        AxisToAngles(clientGlobals->refdef.viewaxis, angles);
        angles[2] = chunk->gen.phys.rotation;
        AngleVectors(angles, fwd, right, down);
        halfSize = chunk->gen.size.current * 0.5f;
        right[0] = halfSize * right[0];
        right[1] = halfSize * right[1];
        right[2] = halfSize * right[2];
        negHalfSize = chunk->gen.size.current * -0.5f;
        down[0] = negHalfSize * down[0];
        down[1] = negHalfSize * down[1];
        down[2] = negHalfSize * down[2];
        start[0] = points[3][0] * right[0] + chunk->gen.phys.origin[0];
        start[1] = points[3][0] * right[1] + chunk->gen.phys.origin[1];
        start[2] = points[3][0] * right[2] + chunk->gen.phys.origin[2];
        start[0] = points[3][1] * down[0] + start[0];
        start[1] = points[3][1] * down[1] + start[1];
        start[2] = points[3][1] * down[2] + start[2];
        for ( cornerIndex = 0; cornerIndex < 4; ++cornerIndex )
        {
            pointOffsetRight = points[cornerIndex][0];
            end[0] = pointOffsetRight * right[0] + chunk->gen.phys.origin[0];
            end[1] = pointOffsetRight * right[1] + chunk->gen.phys.origin[1];
            end[2] = pointOffsetRight * right[2] + chunk->gen.phys.origin[2];
            pointOffsetDown = points[cornerIndex][1];
            end[0] = pointOffsetDown * down[0] + end[0];
            end[1] = pointOffsetDown * down[1] + end[1];
            end[2] = pointOffsetDown * down[2] + end[2];
            CG_DebugLine(start, end, colorRed, 1, 1);
            start[0] = end[0];
            start[1] = end[1];
            start[2] = end[2];
        }
    }
}

void __cdecl Flame_Class_Chunk_Render_All(int localClientNum)
{
    flameChunk_s *chunk;

    if ( flame_debug_render->current.integer > 0 )
    {
        for ( chunk = flameChunksUsed; chunk; chunk = (flameChunk_s *)chunk->gen.listGlobal.next )
            Flame_Class_Chunk_Render_Item(localClientNum, chunk);
    }
}

