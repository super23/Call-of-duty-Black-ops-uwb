#pragma once

void __cdecl R_AddDelayedStaticModelDrawSurf(
        GfxDelayedCmdBuf *delayedCmdBuf,
        XSurface *xsurf,
        unsigned __int8 *list,
        unsigned int count,
        unsigned int which_lod);
void __cdecl R_WritePrimDrawSurfInt(GfxDelayedCmdBuf *delayedCmdBuf, unsigned int value);
void __cdecl R_WritePrimDrawSurfData(GfxDelayedCmdBuf *delayedCmdBuf, unsigned __int8 *data, unsigned int count);
GfxStaticModelId __cdecl R_GetStaticModelId(unsigned int smodelIndex, int lod);
void __cdecl R_AddAllStaticModelSurfacesCamera(
        int viewIndex,
        int teleported,
        const GfxLight *visibleLights,
        int visibleLightCount,
        bool noLodCullOut);
void __cdecl R_SkinStaticModelsCameraForLod(
        const XModel *model,
        unsigned __int8 primaryLightIndex,
        int modelFade,
        unsigned __int8 *list,
        unsigned int count,
        unsigned int surfType,
        unsigned int lod,
        bool prepassSizeCull,
        GfxSModelDrawSurfLightingData *surfData,
        char visLightsMask,
        bool noDynamicShadow);
void __cdecl R_SkinStaticModelsCameraForLod_Internal(
        const XModel *model,
        unsigned __int8 primaryLightIndex,
        int modelFade,
        unsigned __int8 *list,
        unsigned int count,
        unsigned int surfType,
        unsigned int lod,
        bool prepassSizeCull,
        GfxSModelDrawSurfLightingData *surfData,
        bool needsCharredTech,
        char visLightsMask,
        bool noDynamicShadow);
char __cdecl R_PreTessStaticModelCachedList(
        const XModel *model,
        unsigned __int16 *list,
        unsigned int count,
        unsigned int lod,
        int surfaceIndex,
        GfxDrawSurf drawSurf,
        GfxDrawSurfList *drawSurfList,
        GfxDelayedCmdBuf *delayedCmdBuf);
void __cdecl R_SkinStaticModelsCamera(
        const XModel *model,
        unsigned __int8 primaryLightIndex,
        int modelFade,
        unsigned __int16 (*staticModelLodList)[4][128],
        unsigned __int16 (*staticModelLodCount)[4],
        bool prepassSizeCull,
        GfxSModelDrawSurfLightingData *surfData,
        char visLightsMask,
        bool noDynamicShadow);
void __cdecl R_SkinStaticModelsCameraForSurface(
        const XModel *model,
        unsigned __int8 primaryLightIndex,
        int modelFade,
        unsigned __int16 (*staticModelLodList)[128],
        unsigned __int16 *staticModelLodCount,
        unsigned int surfType,
        bool prepassSizeCull,
        GfxSModelDrawSurfLightingData *surfData,
        char visLightsMask,
        bool noDynamicShadow);
void __cdecl R_ShowCountsStaticModel(int smodelIndex, int lod);
void __cdecl R_DumpStaticModelLodInfo(const GfxStaticModelDrawInst *smodelDrawInst, float dist);
void __cdecl R_StaticModelWriteInfoHeader(int fileHandle);
void __cdecl R_StaticModelWriteInfo(int fileHandle, const GfxStaticModelDrawInst *smodelDrawInst, float dist);
void R_EndDumpStaticModelLodInfo();
void __cdecl R_SortAllStaticModelSurfacesCamera();
void __cdecl R_SortAllStaticModelSurfacesSunShadow();
void __cdecl R_AddAllStaticModelSurfacesSunShadow(int viewIndex);
void __cdecl R_AddAllStaticModelSurfacesRangeSunShadow(
        int viewIndex,
        unsigned int partitionIndex,
        unsigned int maxDrawSurfCount);
void __cdecl R_SkinStaticModelsShadowForLod(
        const XModel *model,
        unsigned __int8 *list,
        unsigned int count,
        unsigned int surfType,
        unsigned int lod,
        GfxSModelDrawSurfData *surfData);
void __cdecl R_SkinStaticModelsShadow(
        const XModel *model,
        unsigned __int16 (*staticModelLodList)[4][128],
        unsigned __int16 (*staticModelLodCount)[4],
        GfxSModelDrawSurfData *surfData);
void __cdecl R_SkinStaticModelsShadowForSurface(
        const XModel *model,
        unsigned __int16 (*staticModelLodList)[128],
        unsigned __int16 *staticModelLodCount,
        unsigned int surfType,
        GfxSModelDrawSurfData *surfData);
void __cdecl R_AddAllStaticModelSurfacesSpotShadow(
        int viewIndex,
        unsigned int spotShadowIndex,
        unsigned int primaryLightIndex);
