#pragma once

void __cdecl R_ShowLightVisCachePoints(const float *viewOrigin, const DpvsPlane *clipPlanes, int clipPlaneCount);
bool __cdecl R_SortedHistoryEntry(
        unsigned __int16 x,
        unsigned __int16 y,
        unsigned __int16 z,
        GfxSortedHistoryAdd addMode);
htab *__cdecl addHash(const unsigned __int16 (*data)[3]);
htab *__cdecl htabAlloc();
unsigned __int64 __cdecl HashXyz(const unsigned __int16 (*p)[3]);
int __cdecl HashPackPoint(const unsigned __int16 (*p)[3]);
htab *__cdecl findhash(const unsigned __int16 (*data)[3]);
void __cdecl R_ApplyLightGridColorsPatch(const GfxModelLightingPatch *patch, unsigned __int8 *pixels);
void __cdecl R_SetLightGridColorsVec4(
        const GfxDecodedLightGridColors *colors,
        float primaryWeight,
        unsigned __int8 *pixels);
void __cdecl SetPixelColorLightGridVec4_PC(unsigned __int8 *v, const float *color, float primaryWeight);
void __cdecl R_DecodeLightGridColors(
        const GfxCompressedLightGridColors *colors,
        GfxDecodedLightGridColors *decodedColors);
void __cdecl R_DecodeLightGridColorsWeighted(
        const GfxCompressedLightGridColors *colors,
        GfxDecodedLightGridColors *decodedColors,
        float weight);
unsigned __int8 __cdecl R_GetPrimaryLightFromGrid(
        const GfxLightGrid *lightGrid,
        const float *samplePos,
        unsigned __int8 sunPrimaryLightIndex);
unsigned __int8 __cdecl R_LightGridLookup(
        const GfxLightGrid *lightGrid,
        const float *samplePos,
        float *cornerWeight,
        const GfxLightGridEntry **cornerEntry,
        unsigned int *defaultGridEntry);
void __cdecl R_ShowLightGrid(
        const GfxLightGrid *lightGrid,
        const unsigned int *pos,
        const float *samplePos,
        const GfxLightGridEntry **cornerEntry,
        bool *suppressEntry,
        bool honorSuppression);
void __cdecl R_ShowGridOrigin(const float *origin);
void __cdecl R_ShowGridBox(const unsigned int *pos);
void __cdecl R_ShowGridCorner(unsigned int x, unsigned int y, unsigned int z, float halfSize, const float *color);
void __cdecl R_UpdateVisHistory(const GfxLightGrid *lightGrid, const unsigned int *pos);
void AllocAllMemoryNeeded();
void __cdecl R_GetLightGridSampleEntryQuad(
        const GfxLightGrid *lightGrid,
        const unsigned int *pos,
        const GfxLightGridEntry **entries,
        unsigned int *defaultGridEntry);
bool __cdecl R_IsValidLightGridSample(
        const GfxLightGrid *lightGrid,
        const GfxLightGridEntry *entry,
        char cornerIndex,
        const unsigned int *pos,
        const float *samplePos);
unsigned int __cdecl R_GetLightingAtPoint(
        const GfxLightGrid *remoteLightGrid,
        const float *samplePos,
        unsigned int nonSunPrimaryLightIndex,
        unsigned __int16 dest,
        GfxModelLightExtrapolation extrapolateBehavior,
        bool useHeroLighting);
void __cdecl R_SetLightGridColorsFromIndex(
        const GfxLightGrid *lightGrid,
        unsigned int colorsIndex,
        const float *heroPos,
        float primaryLightWeight,
        unsigned __int16 dest);
GfxModelLightingPatch *__cdecl R_BackEndDataAllocAndClearModelLightingPatch(GfxBackEndData *frontEndDataOut);
void __cdecl R_BlendAndSetLightGridColors(
        const GfxLightGrid *lightGrid,
        unsigned __int8 *colorsIndex,
        const float *colorsWeight,
        unsigned int colorsCount,
        const float *heroPos,
        float primaryLightWeight,
        float weightNormalizeScale,
        unsigned __int16 dest);
unsigned __int8 __cdecl R_ExtrapolateLightingAtPoint(
        const GfxLightGrid *lightGrid,
        const float *heroPos,
        unsigned __int16 dest,
        GfxModelLightExtrapolation extrapolateBehavior,
        unsigned int defaultGridEntry);
char __cdecl R_CanLightInfluenceLightGridCorner(
        const GfxLightGrid *lightGrid,
        const ComPrimaryLight *light,
        const float *samplePos,
        char cornerIndex);
void __cdecl R_GetAverageLightingAtPoint(const float *samplePos, float *outColor);
void __cdecl R_GetAverageSecondaryLightingAtPoint(const float *samplePos, float *outColor);
void __cdecl R_SetLightGridSampleDeltas(int rowStride, int sliceStride);
void __cdecl R_InitLightVisHistory(char *bspName);
void __cdecl R_LightVisHistoryFilename(char *bspName, char *filename);
void __cdecl R_SaveLightVisHistory();
void __cdecl HashUnpackPoint(unsigned __int64 value, unsigned __int16 (*p)[3]);
int __cdecl find_hash(const char *a1);
