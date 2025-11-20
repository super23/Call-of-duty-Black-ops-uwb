#pragma once

void __cdecl R_GetReflectionProbePosition(unsigned int index, float *pos);
void __cdecl R_ReflectionProbeRegisterDvars();
void __cdecl R_CalcCubeMapViewValues(refdef_s *refdef, CubemapShot cubemapShot, int cubemapSize);
void __cdecl R_GenerateReflections(char *mapname, GfxReflectionProbe *probes, unsigned int probeCount);
void __cdecl R_GenerateReflectionRawDataAll(DiskGfxReflectionProbe *probeRawData, int probeCount, bool *generateProbe);
void  R_GenerateReflectionRawData(int a1@<ebp>, DiskGfxReflectionProbe *probeRawData);
char __cdecl R_CopyReflectionsFromLumpData(
        DiskGfxReflectionProbe *probeRawData,
        const DiskGfxReflectionProbe *probeRawLumpData,
        int lumpProbeCount);
bool __cdecl R_ReflectionProbeGenerateExitWhenDone();
void __cdecl R_SetBlackProbe(unsigned __int8 *target, unsigned int bytestozero);
void __cdecl R_GenerateDummyReflectionImages(
        GfxReflectionProbe *probes,
        const DiskGfxDummyReflectionProbe *probeRawData,
        int probeCount,
        int probeBaseIndex,
        int probeHighMipSize);
GfxImage *__cdecl R_GenerateReflectionImageFromRawData(
        const unsigned __int8 *rawPixels,
        int probeIndex,
        int probeHighMipSize);
void R_LoadColorCorrectionData();
void __cdecl R_ParseColorCorrectionData(const char *buf, const char *filename);
char __cdecl R_VerifyFieldNames(const char **buf, const char *filename);
ColorCorrectionData *R_CreateDefaultColorCorrectionEntry();
void __cdecl R_GenerateReflectionImages(
        GfxReflectionProbe *probes,
        const DiskGfxReflectionProbe *probeRawData,
        int probeCount,
        int probeBaseIndex,
        int probeHighMipSize);
