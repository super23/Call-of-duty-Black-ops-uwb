#pragma once

// Decomp: CoDSP_rdBlackOps.map.c — loadgame server commands

void __cdecl SV_LoadGame_f();
void __cdecl SV_LoadGameContinue_f();
const char *__cdecl G_GetPendingLoadGamePath();
void __cdecl G_ClearPendingLoadGamePath();
