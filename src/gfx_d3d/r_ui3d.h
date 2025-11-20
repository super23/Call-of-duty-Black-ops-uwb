#pragma once

GfxUI3DStack *__cdecl R_GetUI3DStack();
void __cdecl R_UI3DStack_Push(GfxUI3DStack *stack, int val);
void __cdecl R_UI3DStack_Pop(GfxUI3DStack *stack);
int __cdecl R_UI3DStack_Top(GfxUI3DStack *stack);
ScreenPlacement *__cdecl R_UI3D_ScrPlaceFromTextureWindow(unsigned int windowIndex);
void __cdecl R_UI3D_SetBlurRadius(float blurRadius);
void __cdecl R_UI3D_CheckRenderTarget();
void __cdecl R_UI3D_OnetimeInit(
        unsigned __int16 width,
        unsigned __int16 height,
        bool useDisplaySize,
        int pmemLocation,
        bool allocatePingPongBuffer);
void __cdecl R_UI3D_Shutdown();
void __cdecl R_UI3D_PerframeInit();
void __cdecl R_InitUI3DStack(GfxUI3DStack *stack);
void __cdecl R_UI3D_SetupBackendData_UVConstants(GfxUI3DBackend *ui3dBackend);
void __cdecl R_UI3D_GetUVSetup(unsigned int windowIndex, float *out);
void __cdecl R_UI3D_SetupBackendData(GfxUI3DBackend *ui3dBackend);
void __cdecl R_UI3D_IncrementRenderCmdCount(unsigned int wndIndex);
void __cdecl R_UI3D_SetupTextureWindow(
        unsigned int windowIndex,
        float normX,
        float normY,
        float normWidth,
        float normHeight);
void __cdecl ScrPlace_SetupUI3D(ScreenPlacement *scrPlace, int viewportWidth, int viewportHeight);
void __cdecl RB_UI3D_SetShaderConstants(GfxCmdBufSourceState *source, const GfxUI3DBackend *rbUI3D);
void __cdecl RB_SetUI3DSamplerAndConstants(GfxCmdBufSourceState *cmdBufSrcState, const GfxUI3DBackend *rbUI3D);
void __cdecl RB_UI3D_RenderToTexture(const void *cmds, const GfxUI3DBackend *rbUI3D, const GfxCmdBufInput *input);
int __cdecl RB_UI3D_GetValidWindows(const GfxUI3DBackend *rbUI3D, int *validWindowsArray);
bool __cdecl RB_UI3D_ViewportIsValid(const GfxViewport *vp);
