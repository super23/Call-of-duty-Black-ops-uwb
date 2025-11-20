#pragma once

void __cdecl Flame_Class_Smoke_Init();
flameSmoke_t *__cdecl Flame_Class_Smoke_Alloc();
void __cdecl Flame_Class_Smoke_Free(flameSmoke_t *smoke);
void __cdecl Flame_Class_Smoke_Age(bool is_server, int time);
flameSmoke_t *__cdecl Flame_Class_Smoke_Spawn(flameChunk_s *fromChunk, int curTime);
void __cdecl Flame_Class_Smoke_Render_Item(int localClientNum, flameSmoke_t *smoke);
void __cdecl Flame_Class_Smoke_Render_All(int localClientNum);
