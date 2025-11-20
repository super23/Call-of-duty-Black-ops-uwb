#pragma once

void __cdecl CM_LoadMap(const char *name, int *checksum);
void CM_InitAllThreadData();
void __cdecl CM_InitThreadData(unsigned int threadContext);
void __cdecl CM_LoadMapData(const char *name);
void __cdecl CM_LoadMapData_LoadObj(const char *name);
void __cdecl CM_LoadMapData_FastFile(const char *name);
void __cdecl Rope_InitRopes();
void __cdecl ShutdownRopes();
void __cdecl CM_Shutdown();
void __cdecl CM_Unload();
int __cdecl CM_LeafCluster(unsigned int leafnum);
void __cdecl CM_ModelBounds(unsigned int model, float *mins, float *maxs);
int __cdecl CM_ModelSurfaceFlags(unsigned int model);
unsigned __int8 *__cdecl CM_Hunk_Alloc(unsigned int size, const char *name, int type);
void __cdecl CM_Hunk_CheckTempMemoryHighClear();
unsigned int __cdecl CM_Hunk_AllocateTempMemoryHigh(int size, const char *name);
void __cdecl CM_Hunk_ClearTempMemoryHigh();
