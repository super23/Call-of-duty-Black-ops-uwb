#include "r_draw_sunshadow.h"

void  R_DrawSunShadowMap(
        int a1@<ebp>,
        const GfxViewInfo *viewInfo,
        unsigned int partitionIndex,
        GfxCmdBuf *cmdBuf)
{
  void *v4; // esp
  unsigned int v5[2]; // [esp+18h] [ebp-1AC4h] BYREF
  float v6; // [esp+20h] [ebp-1ABCh]
  const GfxDrawSurfListInfo *v7; // [esp+24h] [ebp-1AB8h]
  GfxSunShadowPartition *v8; // [esp+28h] [ebp-1AB4h]
  const GfxBackEndData *data; // [esp+2Ch] [ebp-1AB0h]
  GfxCmdBufSourceState state; // [esp+30h] [ebp-1AACh] BYREF
  int v11; // [esp+1AD0h] [ebp-Ch]
  void *v12; // [esp+1AD4h] [ebp-8h]
  void *retaddr; // [esp+1ADCh] [ebp+0h]

  v11 = a1;
  v12 = retaddr;
  v4 = alloca(0x1AC0);
  R_InitCmdBufSourceState(&state, &viewInfo->input, 0);
  R_SetWindShaderConstants(&state);
  R_SetRenderTargetSize(&state, 14u);
  data = viewInfo->input.data;
  v8 = &data->sunShadow.partition[partitionIndex];
  v7 = &viewInfo->drawList[partitionIndex + 8];
  v6 = data->sunShadow.partition[partitionIndex].shadowViewParms.projectionMatrix.m[2][2];
  R_UpdateCodeConstant(
    &state,
    0x14u,
    (float)(sm_polygonOffsetBias->current.value * 0.25) * v6,
    sm_polygonOffsetScale->current.value,
    0.0,
    0.0);
  R_SetViewportValues(
    &state,
    0,
    partitionIndex * (unsigned int)dx.singleSampleDepthStencilSurface,
    (int)dx.singleSampleDepthStencilSurface,
    (int)dx.singleSampleDepthStencilSurface);
  v5[0] = v8;
  v5[1] = v7;
  R_DrawCall(R_DrawSunShadowMapCallback, v5, &state, viewInfo, v7, &v8->shadowViewParms, cmdBuf, 0);
}

void __cdecl R_DrawSunShadowMapCallback(const GfxSunShadowPartition **userData, GfxCmdBufContext context)
{
  const GfxDrawSurfListInfo *drawList; // [esp+34h] [ebp-8h]
  const GfxSunShadowPartition *partition; // [esp+38h] [ebp-4h]

  partition = *userData;
  drawList = (const GfxDrawSurfListInfo *)userData[1];
  R_SetRenderTarget(context, 0xEu);
  if ( !partition->partitionIndex )
    R_ClearScreen(context.state->prim.device, 3u, shadowmapClearColor, 1.0, 0, 0);
  R_HW_EnableScissor(
    context.state->prim.device,
    partition->viewport.x,
    partition->viewport.y + partition->partitionIndex * (int)dx.singleSampleDepthStencilSurface,
    partition->viewport.width,
    partition->viewport.height);
  R_DrawSurfs(context, 0, drawList);
  R_HW_DisableScissor(context.state->prim.device);
}

