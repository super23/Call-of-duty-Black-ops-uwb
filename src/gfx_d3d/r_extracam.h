#pragma once

double __cdecl R_ExtraCam_AspectRatio();
void __cdecl R_ExtraCam_Init(int location);
void __cdecl CreateExtraCamRenderTargets(eExtraCamResolution resConfig, int location);
void __cdecl R_ExtraCam_Shutdown();
unsigned int __cdecl R_ExtraCam_RenderTargetWidth();
unsigned int __cdecl R_ExtraCam_RenderTargetHeight();
bool __cdecl R_ExtraCam_GfxResourcesReady();
bool __cdecl R_ExtraCam_UseShadowmap();
