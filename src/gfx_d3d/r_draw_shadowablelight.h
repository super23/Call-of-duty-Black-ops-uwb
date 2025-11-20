#pragma once

// local variable allocation has failed, the output may be wrong!
void  R_SetLightProperties(
        float a1@<ebp>,
        GfxCmdBufSourceState *source,
        const GfxLight *light,
        const GfxLightDef *def,
        LightHasShadowMap hasShadowMap,
        float spotShadowFade);
void __cdecl R_SetShadowableLight(GfxCmdBufSourceState *source, unsigned int shadowableLightIndex);
void __cdecl R_SetDrawSurfsShadowableLight(GfxCmdBufSourceState *source, const GfxDrawSurfListInfo *info);
unsigned int __cdecl R_GetShadowableLightIndex(
        const GfxBackEndData *data,
        const GfxViewInfo *viewInfo,
        const GfxLight *light);
