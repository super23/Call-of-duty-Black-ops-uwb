#pragma once

MaterialUsage *__cdecl R_GetMaterialUsageData(Material *material);
void __cdecl R_CreateMaterialList();
void __cdecl R_ShutdownMaterialUsage();
void __cdecl R_InterpretSunLightParseParams(SunLightParseParams *sunParse);
void __cdecl R_UpdateLightsFromDvars();
void __cdecl R_CopyParseParamsFromDvars(SunLightParseParams *sunParse);
void __cdecl R_LoadWorld(char *name, int *checksum, int savegame, int location);
void __cdecl R_CopyParseParamsToDvars(const SunLightParseParams *sunParse, int savegame);
void R_InitDynamicData();
void __cdecl R_SetWorldPtr_LoadObj(const char *name);
void __cdecl R_SetWorldPtr_FastFile(const char *name);
void R_RegisterSkyboxModel();
void __cdecl R_ShutdownWorld();
void __cdecl R_ReleaseWorld();
void __cdecl R_ReloadWorld();
unsigned int __cdecl R_GetDebugReflectionProbeLocs(float (*locArray)[3], unsigned int maxCount);
void __cdecl R_BspGenerateReflections();
void __cdecl R_RegisterSkyboxModel(char *xmodelName);
