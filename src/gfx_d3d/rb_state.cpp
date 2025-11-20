#include "rb_state.h"

void __cdecl RB_SetInitialState()
{
  memset((unsigned __int8 *)&gfxCmdBufInput, 0, sizeof(gfxCmdBufInput));
  R_InitCmdBufSourceState(&gfxCmdBufSourceState, &gfxCmdBufInput, 0);
  gfxCmdBufState.prim.device = dx.device;
  R_SetInitialContextState(dx.device);
  R_SetTexFilter();
  R_InitCmdBufState(&gfxCmdBufState);
  RB_InitSceneViewport();
  if ( !stru_B50E858.surface.color
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_state.cpp",
          81,
          0,
          "%s",
          "gfxRenderTargets[R_RENDERTARGET_FRAME_BUFFER].surface.color") )
  {
    __debugbreak();
  }
  if ( !g_allocateMinimalResources
    && !stru_B50E858.surface.depthStencil
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_state.cpp",
          83,
          0,
          "%s",
          "gfxRenderTargets[R_RENDERTARGET_FRAME_BUFFER].surface.depthStencil") )
  {
    __debugbreak();
  }
  if ( !stru_B50E858.width
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_state.cpp",
          91,
          0,
          "%s",
          "gfxRenderTargets[R_RENDERTARGET_FRAME_BUFFER].width") )
  {
    __debugbreak();
  }
  if ( !stru_B50E858.height
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_state.cpp",
          92,
          0,
          "%s",
          "gfxRenderTargets[R_RENDERTARGET_FRAME_BUFFER].height") )
  {
    __debugbreak();
  }
  if ( dword_B473FD0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_state.cpp",
          94,
          1,
          "%s",
          "gfxCmdBufSourceState.renderTargetWidth == 0") )
  {
    __debugbreak();
  }
  if ( dword_B473FD4
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_state.cpp",
          95,
          1,
          "%s",
          "gfxCmdBufSourceState.renderTargetHeight == 0") )
  {
    __debugbreak();
  }
  if ( dword_B473FCC
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_state.cpp",
          96,
          1,
          "%s",
          "gfxCmdBufSourceState.viewportBehavior == 0") )
  {
    __debugbreak();
  }
  R_SetRenderTargetSize(&gfxCmdBufSourceState, 2u);
  R_SetRenderTarget(gfxCmdBufContext, 2u);
  if ( gfxCmdBufState.prim.indexBuffer
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_state.cpp",
          104,
          1,
          "%s",
          "gfxCmdBufState.prim.indexBuffer == NULL") )
  {
    __debugbreak();
  }
  if ( gfxCmdBufState.prim.streams[0].vb
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_state.cpp",
          105,
          1,
          "%s",
          "gfxCmdBufState.prim.streams[0].vb == NULL") )
  {
    __debugbreak();
  }
  if ( gfxCmdBufState.prim.streams[0].offset
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_state.cpp",
          106,
          1,
          "%s",
          "gfxCmdBufState.prim.streams[0].offset == 0") )
  {
    __debugbreak();
  }
  if ( gfxCmdBufState.prim.streams[0].stride
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_state.cpp",
          107,
          1,
          "%s",
          "gfxCmdBufState.prim.streams[0].stride == 0") )
  {
    __debugbreak();
  }
  if ( gfxCmdBufState.prim.streams[1].vb
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_state.cpp",
          108,
          1,
          "%s",
          "gfxCmdBufState.prim.streams[1].vb == NULL") )
  {
    __debugbreak();
  }
  if ( gfxCmdBufState.prim.streams[1].offset
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_state.cpp",
          109,
          1,
          "%s",
          "gfxCmdBufState.prim.streams[1].offset == 0") )
  {
    __debugbreak();
  }
  if ( gfxCmdBufState.prim.streams[1].stride
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_state.cpp",
          110,
          1,
          "%s",
          "gfxCmdBufState.prim.streams[1].stride == 0") )
  {
    __debugbreak();
  }
  if ( gfxCmdBufState.vertexShader
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_state.cpp",
          114,
          1,
          "%s",
          "gfxCmdBufState.vertexShader == NULL") )
  {
    __debugbreak();
  }
  if ( gfxCmdBufState.material
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_state.cpp",
          116,
          1,
          "%s",
          "gfxCmdBufState.material == NULL") )
  {
    __debugbreak();
  }
  if ( gfxCmdBufState.technique
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_state.cpp",
          117,
          1,
          "%s",
          "gfxCmdBufState.technique == NULL") )
  {
    __debugbreak();
  }
  if ( gfxCmdBufState.alphaRef )
  {
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_state.cpp",
            120,
            1,
            "%s",
            "gfxCmdBufState.alphaRef == 0") )
      __debugbreak();
  }
}

void __cdecl RB_UnbindAllImages()
{
  unsigned int samplerIndex; // [esp+0h] [ebp-8h]
  int semaphore; // [esp+4h] [ebp-4h]

  if ( dx.device )
  {
    semaphore = R_AcquireDXDeviceOwnership(0);
    for ( samplerIndex = 0; samplerIndex < vidConfig.maxTextureMaps; ++samplerIndex )
      R_DisableSampler(&gfxCmdBufState, samplerIndex);
    if ( semaphore )
      R_ReleaseDXDeviceOwnership();
  }
}

void __cdecl RB_InitSceneViewport()
{
  gfxCmdBufSourceState.viewportBehavior = GFX_USE_VIEWPORT_FOR_VIEW;
  gfxCmdBufSourceState.renderTargetWidth = 0;
  gfxCmdBufSourceState.renderTargetHeight = vidConfig.displayWidth;
  *(unsigned int *)&gfxCmdBufSourceState.viewportIsDirty = vidConfig.displayHeight;
}

void __cdecl RB_BindDefaultImages()
{
  GfxCmdBufContext context; // [esp+0h] [ebp-10h]
  unsigned int samplerIndex; // [esp+8h] [ebp-8h]

  context.source = &gfxCmdBufSourceState;
  context.state = &gfxCmdBufState;
  for ( samplerIndex = 0; samplerIndex < 0x10; ++samplerIndex )
    R_SetSampler(context, samplerIndex, 1u, rgp.whiteImage);
}

void __cdecl RB_InitImages()
{
  int semaphore; // [esp+0h] [ebp-4h]

  RB_InitCodeImages();
  semaphore = R_AcquireDXDeviceOwnership(0);
  RB_BindDefaultImages();
  if ( semaphore )
    R_ReleaseDXDeviceOwnership();
}

