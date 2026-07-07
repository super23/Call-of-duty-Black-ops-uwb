#include "flame_sound.h"
#include "flame_class_stream.h"
#include <universal/assertive.h>
#include <cgame_mp/cg_main_mp.h>
#include <sound/snd_utils.h>
#include <cgame/cg_sound.h>

void __cdecl Flame_Class_Source_Sound_Init(flameSource_t *source)
{
    source->wasFiring = 0;
    source->lastSoundTime = 0;
    source->flameSoundPoint[0] = 0.0f;
    source->flameSoundPoint[1] = 0.0f;
    source->flameSoundPoint[2] = 0.0f;
}

void __cdecl Flame_Class_Source_Sound(
                int localClientNum,
                bool isFiring,
                bool isFirstPerson,
                int time,
                flameTable *table,
                flameSource_t *source)
{
    flameRenderList_s *renderList;
    float listener[3];
    float fuelStripPoints[2][3];
    const float *lastFuelVert;
    const playerState_s *playerState;
    int soundFlags;

    iassert(table);
    iassert(source);
    (void)isFirstPerson;

    if ( !source->lastSoundTime )
    {
        source->lastSoundTime = time;
        source->flameSoundPoint[0] = source->origin[0];
        source->flameSoundPoint[1] = source->origin[1];
        source->flameSoundPoint[2] = source->origin[2];
    }

    if ( source->currentStream && source->currentStream->chunkList )
    {
        playerState = CG_GetPredictedPlayerState(localClientNum);
        listener[0] = playerState->origin[0];
        listener[1] = playerState->origin[1];
        listener[2] = playerState->origin[2];

        renderList = source->currentStream->renderList;
        fuelStripPoints[0][0] = renderList->fuelVerts[0][0];
        fuelStripPoints[0][1] = renderList->fuelVerts[0][1];
        fuelStripPoints[0][2] = renderList->fuelVerts[0][2];

        lastFuelVert = renderList->fuelVerts[(int)table->flameVar_streamFuelNumSegments - 1];
        fuelStripPoints[1][0] = lastFuelVert[0];
        fuelStripPoints[1][1] = lastFuelVert[1];
        fuelStripPoints[1][2] = lastFuelVert[2];

        SND_GetNearestPointOnStrip(listener, fuelStripPoints, 2u, source->flameSoundPoint);
        nanassertvec3(source->flameSoundPoint);
    }
    else
    {
        source->flameSoundPoint[0] = source->origin[0];
        source->flameSoundPoint[1] = source->origin[1];
        source->flameSoundPoint[2] = source->origin[2];
    }

    soundFlags = 0;
    if ( !source->wasFiring && isFiring )
        soundFlags |= 1u;
    if ( source->wasFiring && !isFiring )
        soundFlags |= 2u;
    if ( isFiring )
        soundFlags |= 4u;

    source->wasFiring = isFiring;
    source->lastSoundTime = time;

    CG_FlameSourceUpdate(
        localClientNum,
        source->entityNum,
        source->origin,
        source->flameSoundPoint,
        (snd_flame_source_flags)soundFlags,
        (char *)table->flameOffLoopSound,
        (char *)table->flameIgniteSound,
        (char *)table->flameOnLoopSound,
        (char *)table->flameCooldownSound);
}
