#pragma once

void __cdecl R_AddSpotShadowEntCmd(const GfxSpotShadowEntCmd *data);
char __cdecl R_AddSpotShadowsForLight(
        GfxViewInfo *viewInfo,
        GfxLight *light,
        unsigned int shadowableLightIndex,
        unsigned int totalSpotLightCount,
        float spotShadowFade);
// local variable allocation has failed, the output may be wrong!
void  R_SetViewParmsForLight(
        int a1@<ebp>,
        const GfxLight *light,
        GfxViewParms *viewParms,
        float nearPlaneBias);
void __cdecl R_GetSpotShadowLookupMatrix(
        const GfxViewParms *shadowViewParms,
        int usingGridTiling,
        int usingFullShadowBuffer,
        unsigned int spotShadowIndex,
        unsigned int tileCount,
        GfxMatrix *lookupMatrix);
void __cdecl R_AddSpotShadowModelEntities(int localClientNum, unsigned int primaryLightIndex, const GfxLight *light);
void __cdecl R_GenerateAllSortedSpotShadowDrawSurfs(GfxViewInfo *viewInfo, int viewIndex);
void __cdecl R_GenerateSortedPrimarySpotShadowDrawSurfs(
        const GfxViewInfo *viewInfo,
        int viewIndex,
        unsigned int spotShadowIndex,
        unsigned int shadowableLightIndex);
void __cdecl R_EmitSpotShadowMapSurfs(GfxViewInfo *viewInfo);
