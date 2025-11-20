#pragma once

void __cdecl Flame_Class_Source_Sound_Init(flameSource_t *source);
void __cdecl Flame_Class_Source_Sound(
        int localClientNum,
        bool isFiring,
        bool isFirstPerson,
        int time,
        flameTable *table,
        flameSource_t *source);
