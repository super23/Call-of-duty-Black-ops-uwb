#pragma once

bool __cdecl Com_IsBspLoaded();
char *__cdecl Com_GetBspLump(LumpType type, unsigned int elemSize, unsigned int *count);
char *__cdecl Com_GetBspLumpCountForVersion(int version);
char *__cdecl Com_ValidateBspLumpData(
        LumpType type,
        unsigned int offset,
        unsigned int length,
        unsigned int elemSize,
        unsigned int *count);
const void *__cdecl Com_LoadBspLump(char *mapname, LumpType type, unsigned int elemSize, unsigned int *count);
unsigned __int8 *__cdecl Com_ReadLumpOutOfBsp(int h, LumpType type, unsigned int elemSize, unsigned int *count);
unsigned __int8 *__cdecl Com_ReadLumpOutOfBspAtOffset(
        int h,
        LumpType type,
        unsigned int offset,
        unsigned int length,
        unsigned int elemSize,
        unsigned int *count);
void __cdecl Com_UnloadBspLump(LumpType type);
bool __cdecl Com_BspHasLump(LumpType type);
unsigned int __cdecl Com_GetBspVersion();
unsigned int __cdecl Com_GetBspChecksum();
bool __cdecl Com_BspError();
char __cdecl Com_CheckVersionLumpCountError(int version);
void __cdecl Com_LoadBsp(char *filename);
void __cdecl Com_UnloadBsp();
void __cdecl Com_CleanupBsp();
char *__cdecl Com_EntityString(int *numEntityChars);
void __cdecl Com_SaveLump(LumpType type, const void *newLump, unsigned int size, ComSaveLumpBehavior behavior);
const char *__cdecl Com_GetHunkStringCopy(char *string);
void __cdecl Com_LoadWaterData();
void __cdecl Byte4CopyBgraToVertexColor(const unsigned __int8 *rgbaFrom, unsigned __int8 *nativeTo);
void __cdecl Com_LoadBurnableData();
int __cdecl Com_BurnableCellSort(unsigned int *va, unsigned int *vb);
void __cdecl Com_PackBurnableSample(const DiskBurnableSample *in, ComBurnableSample *out);
void __cdecl Com_LoadWorld(char *name);
void __cdecl Com_LoadWorld_LoadObj(char *name);
ComPrimaryLight *Com_LoadPrimaryLights();
const char *__cdecl Com_GetLightDefName(
        char *defName,
        const ComPrimaryLight *primaryLights,
        unsigned int primaryLightCount);
ComPrimaryLight *Com_LoadPrimaryLights_Version14();
const DiskPrimaryLight_Version16 *Com_LoadPrimaryLights_Version16();
void __cdecl Com_LoadWorld_FastFile(const char *name);
void __cdecl Com_ShutdownWorld();
