#include "flame_cull.h"
#include "flame_class_chunk.h"

void __cdecl Flame_Cull_Stream_Chunk(bool is_server, flameChunk_s *cull, flameChunk_s *keep)
{
    if ( cull->spawnSmokeInterval && !keep->spawnSmokeInterval )
        keep->spawnSmokeInterval = cull->spawnSmokeInterval;
    if ( cull->spawnDripsInterval && !keep->spawnDripsInterval )
        keep->spawnDripsInterval = cull->spawnDripsInterval;
    Flame_Class_Chunk_Free(is_server, cull);
}

void __cdecl Flame_Cull_Stream_Chunks(bool is_server, flameStream_s *stream)
{
    flameChunk_s *thisChunk;
    flameChunk_s *nextChunk;
    int chunkAgeMs;
    float cullDist;
    float thisRemainingLife;
    float nextRemainingLife;
    float thisEndOrg[3];
    float nextEndOrg[3];
    float endpointDistSq;
    float originDistSq;

    thisChunk = stream->chunkList;
    while ( thisChunk )
    {
        nextChunk = (flameChunk_s *)thisChunk->gen.listLocal.next;
        if ( !nextChunk )
            break;

        chunkAgeMs = thisChunk->gen.age.lastUpdateTime - thisChunk->gen.age.startTime;
        if ( (float)(stream->flameVars->flameVar_streamChunkCullMinLife * 1000.0) <= (float)chunkAgeMs
            && (float)chunkAgeMs <= (float)(stream->flameVars->flameVar_streamChunkCullMaxLife * 1000.0) )
        {
            cullDist = thisChunk->gen.size.current * stream->flameVars->flameVar_streamChunkCullDistSizeFrac;

            // CoDMPServer.c:705476-705483 — remaining life is ms, not converted to seconds.
            thisRemainingLife = (float)(thisChunk->gen.age.endTime - thisChunk->gen.age.lastUpdateTime);
            thisEndOrg[0] = thisRemainingLife * thisChunk->gen.phys.velocity[0] + thisChunk->gen.phys.origin[0];
            thisEndOrg[1] = thisRemainingLife * thisChunk->gen.phys.velocity[1] + thisChunk->gen.phys.origin[1];
            thisEndOrg[2] = thisRemainingLife * thisChunk->gen.phys.velocity[2] + thisChunk->gen.phys.origin[2];

            nextRemainingLife = (float)(nextChunk->gen.age.endTime - nextChunk->gen.age.lastUpdateTime);
            nextEndOrg[0] = nextRemainingLife * nextChunk->gen.phys.velocity[0] + nextChunk->gen.phys.origin[0];
            nextEndOrg[1] = nextRemainingLife * nextChunk->gen.phys.velocity[1] + nextChunk->gen.phys.origin[1];
            nextEndOrg[2] = nextRemainingLife * nextChunk->gen.phys.velocity[2] + nextChunk->gen.phys.origin[2];

            endpointDistSq = Vec3DistanceSq(nextEndOrg, thisEndOrg);
            if ( cullDist * cullDist > endpointDistSq )
            {
                originDistSq = Vec3DistanceSq(nextChunk->gen.phys.origin, thisChunk->gen.phys.origin);
                if ( cullDist * cullDist > originDistSq )
                {
                    if ( nextChunk->gen.size.current <= thisChunk->gen.size.current )
                    {
                        Flame_Cull_Stream_Chunk(is_server, nextChunk, thisChunk);
                        nextChunk = (flameChunk_s *)thisChunk->gen.listLocal.next;
                    }
                    else
                    {
                        Flame_Cull_Stream_Chunk(is_server, thisChunk, nextChunk);
                    }
                }
            }
        }

        thisChunk = nextChunk;
    }
}
