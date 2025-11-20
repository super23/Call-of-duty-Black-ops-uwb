#pragma once

__int64 __cdecl PCache_Time();
void __cdecl PCache_Lock();
void __cdecl PCache_Unlock();
bool __cdecl PCache_IsLocked();
unsigned __int64 __cdecl PCache_Hash(int controllerIndex, unsigned __int64 xuid);
void __cdecl PCache_Init();
void __cdecl PCache_GetRank_f();
void __cdecl PCache_Update(int controllerIndex);
PCacheComponent *__cdecl PCache_GetOrphanComponent(int controllerIndex, unsigned __int64 xuid, unsigned int type);
PCacheEntry *__cdecl PCache_GetEntry(int controllerIndex, unsigned __int64 xuid);
PCacheComponent *__cdecl PCache_GetEntryComponent(PCacheEntry *entry, unsigned int type);
PCacheComponent *__cdecl PCache_GetComponent(int controllerIndex, unsigned __int64 xuid, unsigned int type);
bool __cdecl PCache_TouchComponent(PCacheComponent *component);
void __cdecl PCache_InvalidateComponent(PCacheComponent *component);
void __cdecl PCache_ReleasePlayerEmblem(PCacheComponent *comp);
void __cdecl PCache_BatchUpdatePlayerEmblems(int controllerIndex, PCachePlayerEmblem *emblems, int count);
int __cdecl PCache_GetClientNumForXuid(int localClientNum, unsigned __int64 xuid);
void __cdecl PCache_InvalidatePlayerEmblem(int controllerIndex, unsigned __int64 xuid);
GfxImage *__cdecl PCache_GetPlayerEmblem(int controllerIndex, unsigned __int64 xuid);
__int16 __cdecl PCache_GetPlayerEmblemBackgroundID(int controllerIndex, unsigned __int64 xuid);
