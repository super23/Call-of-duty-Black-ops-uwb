#pragma once

int __cdecl R_GetDepthStencilFormat(_D3DFORMAT renderTargetFormat);
bool __cdecl R_IsDepthStencilFormatOk(_D3DFORMAT renderTargetFormat, _D3DFORMAT depthStencilFormat);
void __cdecl R_InitRenderTargets();
void R_InitRenderTargets_PC();
void __cdecl R_ShareRenderTarget(unsigned __int8 idFrom, unsigned __int8 idTo);
void __cdecl AssertUninitializedRenderTarget(const GfxRenderTarget *renderTarget);
void __cdecl R_InitFullscreenRenderTargetImage(
        int imageProgType,
        FullscreenType screenType,
        int picmip,
        _D3DFORMAT format,
        RenderTargetUsage usage,
        GfxRenderTarget *renderTarget);
void __cdecl R_GetFullScreenRes(FullscreenType screenType, int *fullscreenWidth, int *fullscreenHeight);
IDirect3DSurface9 *__cdecl R_AssignSingleSampleDepthStencilSurface();
void __cdecl R_GetFrameBufferDepthStencilRes(int *depthStencilWidth, int *depthStencilHeight);
void __cdecl R_InitRenderTargetImage(
        int imageProgType,
        unsigned __int16 width,
        unsigned __int16 height,
        _D3DFORMAT format,
        RenderTargetUsage usage,
        GfxRenderTarget *renderTarget);
void __cdecl R_AssignImageToRenderTargetDepthStencil(GfxRenderTarget *renderTarget, GfxImage *image);
void __cdecl R_AssignImageToRenderTargetColor(GfxRenderTarget *renderTarget, GfxImage *image);
void __cdecl R_InitRenderTargetImage_Composite(int imageProgType, GfxRenderTarget *renderTarget);
void __cdecl R_InitShadowmapRenderTarget(
        int imageProgType,
        unsigned __int16 tileRes,
        unsigned __int16 tileRowCount,
        GfxRenderTarget *renderTarget,
        ShadowmapRTTiling tilingMethond);
void __cdecl R_InitAndTrackRenderTargetImage(
        int imageProgType,
        unsigned __int16 width,
        unsigned __int16 height,
        _D3DFORMAT format,
        RenderTargetUsage usage,
        GfxRenderTarget *renderTarget);
_D3DFORMAT __cdecl R_InitFrameBufferRenderTarget();
const char *__cdecl R_DescribeFormat(_D3DFORMAT format);
void __cdecl R_InitFrameBufferRenderTarget_Win32(GfxRenderTarget *renderTarget);
void R_InitSceneNullRenderTarget_PC();
void __cdecl R_ShareRenderTarget_ReplaceColor(
        unsigned __int8 idFrom,
        unsigned __int8 idTo,
        IDirect3DSurface9 *colorSurface);
void R_InitIntZRenderTarget_PC();
void __cdecl R_ShutdownRenderTargets();
void __cdecl R_ShutDownSingleRenderTarget(GfxRenderTarget *rt);
const char *__cdecl R_RenderTargetName(unsigned __int8 renderTargetId);
void __cdecl R_CheckExtraCamTargets();
void __cdecl R_InitExtraCamRenderTargets(unsigned int width, unsigned int height, int location);
void __cdecl R_InitSizedRenderTargetImage(
        int imageProgType,
        int fullscreenWidth,
        int fullscreenHeight,
        char picmip,
        _D3DFORMAT format,
        RenderTargetUsage usage,
        GfxRenderTarget *renderTarget);
void __cdecl R_ShutdownExtraCamRenderTargets();
void __cdecl R_InitUI3DRenderTarget(
        unsigned __int16 width,
        unsigned __int16 height,
        int location,
        bool allocatePingPongBuffer);
void __cdecl R_ShutdownUI3DRenderTarget(bool freePingPongBuffer);
