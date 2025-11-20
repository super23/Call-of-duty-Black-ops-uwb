#include "rb_depthprepass.h"

void  R_DepthPrepass(
        int a1@<ebp>,
        unsigned __int8 renderTargetId,
        const GfxViewInfo *viewInfo,
        GfxCmdBuf *cmdBuf)
{
  void *v4; // esp
  GfxCmdBufSourceState v5; // [esp-1AA0h] [ebp-1AACh] BYREF
  unsigned int v6[3]; // [esp+0h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]

  v6[0] = a1;
  v6[1] = retaddr;
  v4 = alloca(6816);
  if ( !r_reflectionProbeGenerate->current.enabled )
  {
    R_InitCmdBufSourceState(&v5, &viewInfo->input, 1);
    R_SetRenderTargetSize(&v5, renderTargetId);
    R_SetViewportStruct(&v5, &viewInfo->cullViewInfo.sceneViewport);
    R_SetADSZScaleConstants(viewInfo->localClientNum, &v5);
    R_DrawCall(
      (int)v6,
      (void (__cdecl *)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *))R_DepthPrepassCallback,
      viewInfo,
      &v5,
      viewInfo,
      0,
      &viewInfo->cullViewInfo.viewParms,
      cmdBuf,
      0);
  }
}

void __cdecl R_DepthPrepassCallback(const GfxViewInfo *userData, GfxCmdBufContext context)
{
  int v2; // [esp+44h] [ebp-64h]
  jqBatchGroup *i; // [esp+48h] [ebp-60h]
  GfxDrawSurfListInfo info; // [esp+50h] [ebp-58h] BYREF
  unsigned __int8 baseTechType; // [esp+A3h] [ebp-5h]
  const GfxViewInfo *viewInfo; // [esp+A4h] [ebp-4h]

  v2 = 3;
  for ( i = info.group; --v2 >= 0; ++i )
  {
    i->QueuedBatchCount = 0;
    i->ExecutingBatchCount = 0;
  }
  viewInfo = userData;
  if ( (userData->sceneComposition.renderingMode & 7) == 0 )
    R_HW_EnableScissor(
      context.state->prim.device,
      viewInfo->cullViewInfo.scissorViewport.x,
      viewInfo->cullViewInfo.scissorViewport.y,
      viewInfo->cullViewInfo.scissorViewport.width,
      viewInfo->cullViewInfo.scissorViewport.height);
  if ( viewInfo->renderingFloatZ )
  {
    if ( viewInfo->isMissileCamera )
    {
      R_SetRenderTarget(context, 0x17u);
    }
    else
    {
      if ( !dword_B50E834[5 * (viewInfo->isMissileCamera ? 23 : 7)]
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_depthprepass.cpp",
              213,
              0,
              "%s",
              "R_HaveFloatZ(viewInfo->isMissileCamera)") )
      {
        __debugbreak();
      }
      R_SetRenderTarget(context, 7u);
    }
    baseTechType = 1;
    R_DrawQuadMesh(context, rgp.shadowClearMaterial, &viewInfo->fullSceneViewMesh->meshData);
    context.source->input.consts[75][0] = *(float *)&FLOAT_0_0;
    context.source->input.consts[75][1] = *(float *)&FLOAT_0_0;
    context.source->input.consts[75][2] = *(float *)&FLOAT_0_0;
    context.source->input.consts[75][3] = FLOAT_1_0;
    R_DirtyCodeConstant(context.source, 0x4Bu);
  }
  else
  {
    R_SetRenderTarget(context, dx.supportsSceneNullRenderTarget + 3);
    baseTechType = 0;
  }
  R_SetWindShaderConstants(context.source);
  memcpy(&info, &viewInfo->drawList[3], sizeof(info));
  info.baseTechType = baseTechType;
  PIXBeginNamedEvent(-1, "zprepass decalInfo");
  R_DrawSurfs(context, 0, &info);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  memcpy(&info, viewInfo->drawList, sizeof(info));
  info.baseTechType = baseTechType;
  PIXBeginNamedEvent(-1, "zprepass litInfo");
  R_DrawSurfs(context, 0, &info);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  if ( (viewInfo->sceneComposition.renderingMode & 7) == 0 )
    R_HW_DisableScissor(context.state->prim.device);
}

