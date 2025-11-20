#pragma once

void __cdecl R_DrawSpotShadowMapCallback(const GfxSpotShadow **userData, GfxCmdBufContext context);
void __cdecl RB_SpotShadowMaps(const GfxBackEndData *data, const GfxViewInfo *viewInfo);
void  R_DrawSpotShadowMapArray(int a1@<ebp>, const GfxViewInfo *viewInfo, GfxCmdBuf *cmdBuf);
void __cdecl RB_DrawSpotShadowOverlay();
void __cdecl RB_SetSpotShadowOverlayScaleAndBias(const GfxSpotShadow *spotShadow);
