#include "r_draw_lit.h"

// local variable allocation has failed, the output may be wrong!
void  R_SetDrawLitConstants(
        int a1@<ebp>,
        GfxCmdBufSourceState *source,
        const GfxViewInfo *viewInfo,
        const GfxViewParms *viewParms)
{
  float v4; // [esp+2Ch] [ebp-B0h]
  float v5; // [esp+30h] [ebp-ACh]
  float v6; // [esp+34h] [ebp-A8h]
  float v7; // [esp+40h] [ebp-9Ch]
  float v8; // [esp+44h] [ebp-98h]
  float v9; // [esp+48h] [ebp-94h]
  int v10; // [esp+54h] [ebp-88h]
  float integer; // [esp+58h] [ebp-84h]
  _BYTE x[76]; // [esp+60h] [ebp-7Ch] OVERLAPPED BYREF
  float treeScatterAmount; // [esp+ACh] [ebp-30h]
  float treeScatterIntensity; // [esp+B0h] [ebp-2Ch]
  SunLightParseParams *p_sunParse; // [esp+B4h] [ebp-28h]
  float *v16; // [esp+B8h] [ebp-24h]
  int v17; // [esp+BCh] [ebp-20h]
  SunLightParseParams *sunParse; // [esp+C0h] [ebp-1Ch]
  float v19; // [esp+C4h] [ebp-18h]
  float v20; // [esp+C8h] [ebp-14h]
  const float *CloudArea; // [esp+CCh] [ebp-10h]
  unsigned int v22[2]; // [esp+D0h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+DCh] [ebp+0h]

  v22[0] = a1;
  v22[1] = retaddr;
  R_SetWindShaderConstants(source);
  R_SetSunConstants(&source->input);
  CloudArea = (const float *)R_GetCloudArea();
  v20 = *CloudArea - viewParms->origin[0];
  v19 = CloudArea[1] - viewParms->origin[1];
  sunParse = (SunLightParseParams *)*((unsigned int *)CloudArea + 2);
  v17 = *((unsigned int *)CloudArea + 3);
  v16 = source->input.consts[115];
  source->input.consts[115][0] = v20;
  v16[1] = v19;
  *((unsigned int *)v16 + 2) = sunParse;
  *((unsigned int *)v16 + 3) = v17;
  R_DirtyCodeConstant(source, 0x73u);
  R_SetADSZScaleConstants(viewInfo->localClientNum, source);
  R_SetFrameFog(COERCE_FLOAT(v22), &source->input, viewInfo);
  p_sunParse = &rgp.world->sunParse;
  treeScatterIntensity = rgp.world->sunParse.treeScatterIntensity;
  treeScatterAmount = rgp.world->sunParse.treeScatterAmount;
  *(unsigned int *)&x[72] = source->input.consts[119];
  source->input.consts[119][0] = treeScatterIntensity;
  *(float *)(*(unsigned int *)&x[72] + 4) = treeScatterAmount;
  *(unsigned int *)(*(unsigned int *)&x[72] + 8) = *(unsigned int *)&FLOAT_0_0;
  *(unsigned int *)(*(unsigned int *)&x[72] + 12) = *(unsigned int *)&FLOAT_0_0;
  R_DirtyCodeConstant(source, 0x77u);
  colorTempMatrix((float (*)[4])x, r_skyColorTemp->current.value);
  integer = (float)r_treeScale->current.integer;
  v10 = r_testScale->current.integer;
  LODWORD(source->input.consts[68][0]) = r_skyTransition->current.integer;
  source->input.consts[68][1] = *(float *)&FLOAT_0_0;
  source->input.consts[68][2] = integer;
  LODWORD(source->input.consts[68][3]) = v10;
  R_DirtyCodeConstant(source, 0x44u);
  v9 = *(float *)&x[16];
  v8 = *(float *)&x[32];
  v7 = *(float *)&x[48];
  source->input.consts[44][0] = *(float *)x;
  source->input.consts[44][1] = v9;
  source->input.consts[44][2] = v8;
  source->input.consts[44][3] = v7;
  R_DirtyCodeConstant(source, 0x2Cu);
  v6 = *(float *)&x[20];
  v5 = *(float *)&x[36];
  v4 = *(float *)&x[52];
  source->input.consts[45][0] = *(float *)&x[4];
  source->input.consts[45][1] = v6;
  source->input.consts[45][2] = v5;
  source->input.consts[45][3] = v4;
  R_DirtyCodeConstant(source, 0x2Du);
  Vec4Set(source->input.consts[46], *(float *)&x[8], *(float *)&x[24], *(float *)&x[40], *(float *)&x[56]);
  R_DirtyCodeConstant(source, 0x2Eu);
}

void  R_DrawCloakHDR(int a1@<ebp>, const GfxViewInfo *viewInfo, GfxCmdBuf *cmdBuf, CloakPhaseID phase)
{
  void *v4; // esp
  float treeScatterAmount; // [esp-1AC0h] [ebp-1ACCh]
  float v6; // [esp-1AB4h] [ebp-1AC0h]
  unsigned int v7; // [esp-1AB0h] [ebp-1ABCh]
  unsigned int v8; // [esp-1AACh] [ebp-1AB8h]
  const float *CloudArea; // [esp-1AA4h] [ebp-1AB0h]
  GfxCmdBufSourceState v10; // [esp-1AA0h] [ebp-1AACh] BYREF
  unsigned int v11[3]; // [esp+0h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]

  v11[0] = a1;
  v11[1] = retaddr;
  v4 = alloca(6864);
  PIXBeginNamedEvent(-1, "R_DrawCloak");
  R_InitCmdBufSourceState(&v10, &viewInfo->input, 1);
  R_SetRenderTargetSize(&v10, viewInfo->sceneComposition.mainSceneMSAA);
  R_SetViewportStruct(&v10, &viewInfo->cullViewInfo.sceneViewport);
  R_SetWindShaderConstants(&v10);
  CloudArea = (const float *)R_GetCloudArea();
  *(float *)&v8 = CloudArea[1] - viewInfo->cullViewInfo.viewParms.origin[1];
  v7 = *((unsigned int *)CloudArea + 2);
  v6 = CloudArea[3];
  v10.input.consts[115][0] = *CloudArea - viewInfo->cullViewInfo.viewParms.origin[0];
  *(_QWORD *)&v10.gap0[1844] = __PAIR64__(v7, v8);
  v10.input.consts[115][3] = v6;
  R_DirtyCodeConstant(&v10, 0x73u);
  treeScatterAmount = rgp.world->sunParse.treeScatterAmount;
  v10.input.consts[119][0] = rgp.world->sunParse.treeScatterIntensity;
  *(_QWORD *)&v10.gap0[1908] = __PAIR64__(*(unsigned int *)&FLOAT_0_0, LODWORD(treeScatterAmount));
  v10.input.consts[119][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(&v10, 0x77u);
  R_SetADSZScaleConstants(viewInfo->localClientNum, &v10);
  if ( phase )
  {
    if ( phase == CLOAK_PHASE_CLOAKED )
      R_DrawCall(
        (int)v11,
        (void (__cdecl *)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *))R_DrawCloakPostEmissiveCallbackHDR,
        viewInfo,
        &v10,
        viewInfo,
        &viewInfo->drawList[6],
        &viewInfo->cullViewInfo.viewParms,
        cmdBuf,
        0);
  }
  else
  {
    R_DrawCall(
      (int)v11,
      (void (__cdecl *)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *))R_DrawCloakPrePassCallbackHDR,
      viewInfo,
      &v10,
      viewInfo,
      &viewInfo->drawList[6],
      &viewInfo->cullViewInfo.viewParms,
      cmdBuf,
      0);
  }
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

void __cdecl R_DrawCloakPrePassCallbackHDR(
        const GfxViewInfo *userData,
        GfxCmdBufContext context,
        GfxCmdBufContext prepassContext)
{
  int v3; // [esp+30h] [ebp-60h]
  jqBatchGroup *i; // [esp+34h] [ebp-5Ch]
  GfxDrawSurfListInfo info; // [esp+38h] [ebp-58h] BYREF
  const GfxViewInfo *viewInfo; // [esp+8Ch] [ebp-4h]

  v3 = 3;
  for ( i = info.group; --v3 >= 0; ++i )
  {
    i->QueuedBatchCount = 0;
    i->ExecutingBatchCount = 0;
  }
  viewInfo = userData;
  R_SetRenderTarget(context, userData->sceneComposition.mainSceneMSAA);
  if ( (viewInfo->sceneComposition.renderingMode & 7) == 0 )
    R_HW_EnableScissor(
      context.state->prim.device,
      viewInfo->cullViewInfo.scissorViewport.x,
      viewInfo->cullViewInfo.scissorViewport.y,
      viewInfo->cullViewInfo.scissorViewport.width,
      viewInfo->cullViewInfo.scissorViewport.height);
  memcpy(&info, &viewInfo->drawList[6], sizeof(info));
  info.baseTechType = 10;
  R_DrawSurfs(context, prepassContext.state, &viewInfo->drawList[6]);
  if ( (viewInfo->sceneComposition.renderingMode & 7) == 0 )
    R_HW_DisableScissor(context.state->prim.device);
}

void __cdecl R_DrawCloakPostEmissiveCallbackHDR(
        const GfxViewInfo *userData,
        GfxCmdBufContext context,
        GfxCmdBufContext prepassContext)
{
  int v3; // [esp+30h] [ebp-60h]
  jqBatchGroup *i; // [esp+34h] [ebp-5Ch]
  GfxDrawSurfListInfo info; // [esp+38h] [ebp-58h] BYREF
  const GfxViewInfo *viewInfo; // [esp+8Ch] [ebp-4h]

  v3 = 3;
  for ( i = info.group; --v3 >= 0; ++i )
  {
    i->QueuedBatchCount = 0;
    i->ExecutingBatchCount = 0;
  }
  viewInfo = userData;
  R_SetRenderTarget(context, userData->sceneComposition.mainSceneMSAA);
  if ( (viewInfo->sceneComposition.renderingMode & 7) == 0 )
    R_HW_EnableScissor(
      context.state->prim.device,
      viewInfo->cullViewInfo.scissorViewport.x,
      viewInfo->cullViewInfo.scissorViewport.y,
      viewInfo->cullViewInfo.scissorViewport.width,
      viewInfo->cullViewInfo.scissorViewport.height);
  memcpy(&info, &viewInfo->drawList[6], sizeof(info));
  info.baseTechType = 4;
  R_DrawSurfs(context, prepassContext.state, &info);
  if ( (viewInfo->sceneComposition.renderingMode & 7) == 0 )
    R_HW_DisableScissor(context.state->prim.device);
}

void  R_DrawLit(
        int a1@<ebp>,
        const GfxViewInfo *viewInfo,
        GfxCmdBuf *cmdBuf,
        GfxCmdBuf *prepassCmdBuf,
        LitPhaseID phase)
{
  void *v5; // esp
  GfxCmdBufSourceState v6; // [esp-1AA0h] [ebp-1AACh] BYREF
  unsigned int v7[3]; // [esp+0h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]

  v7[0] = a1;
  v7[1] = retaddr;
  v5 = alloca(6832);
  R_InitCmdBufSourceState(&v6, &viewInfo->input, 1);
  R_SetRenderTargetSize(&v6, viewInfo->sceneComposition.mainSceneMSAA);
  R_SetViewportStruct(&v6, &viewInfo->cullViewInfo.sceneViewport);
  R_SetDrawLitConstants((int)v7, &v6, viewInfo, &viewInfo->cullViewInfo.viewParms);
  if ( phase )
  {
    if ( phase == LIT_PHASE_POST_RESOLVE )
    {
      R_DrawCall(
        (int)v7,
        (void (__cdecl *)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *))R_DrawLitPostResolveCallback,
        viewInfo,
        &v6,
        viewInfo,
        &viewInfo->drawList[1],
        &viewInfo->cullViewInfo.viewParms,
        cmdBuf,
        prepassCmdBuf);
    }
    else if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_draw_lit.cpp", 636, 0, "invalid phase") )
    {
      __debugbreak();
    }
  }
  else
  {
    R_DrawCall(
      (int)v7,
      (void (__cdecl *)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *))R_DrawLitCallback,
      viewInfo,
      &v6,
      viewInfo,
      viewInfo->drawList,
      &viewInfo->cullViewInfo.viewParms,
      cmdBuf,
      prepassCmdBuf);
  }
}

void __cdecl R_DrawLitCallback(char *userData, GfxCmdBufContext context, GfxCmdBufContext prepassContext)
{
  R_SetRenderTarget(context, userData[13808]);
  if ( prepassContext.state )
    R_SetRenderTarget(prepassContext, userData[13808]);
  if ( (*((unsigned int *)userData + 3464) & 7) == 0 )
    R_HW_EnableScissor(
      context.state->prim.device,
      *((unsigned int *)userData + 88),
      *((unsigned int *)userData + 89),
      *((unsigned int *)userData + 90),
      *((unsigned int *)userData + 91));
  R_DrawSurfs(context, prepassContext.state, (const GfxDrawSurfListInfo *)(userData + 8752));
  if ( (*((unsigned int *)userData + 3464) & 7) == 0 )
    R_HW_DisableScissor(context.state->prim.device);
}

void __cdecl R_DrawLitPostResolveCallback(char *userData, GfxCmdBufContext context, GfxCmdBufContext prepassContext)
{
  R_SetRenderTarget(context, userData[13808]);
  if ( prepassContext.state )
    R_SetRenderTarget(prepassContext, userData[13808]);
  if ( (*((unsigned int *)userData + 3464) & 7) == 0 )
    R_HW_EnableScissor(
      context.state->prim.device,
      *((unsigned int *)userData + 88),
      *((unsigned int *)userData + 89),
      *((unsigned int *)userData + 90),
      *((unsigned int *)userData + 91));
  R_DrawSurfs(context, prepassContext.state, (const GfxDrawSurfListInfo *)(userData + 8832));
  if ( (*((unsigned int *)userData + 3464) & 7) == 0 )
    R_HW_DisableScissor(context.state->prim.device);
}

void  R_DrawDecal(int a1@<ebp>, const GfxViewInfo *viewInfo, GfxCmdBuf *cmdBuf, GfxCmdBuf *prepassCmdBuf)
{
  void *v4; // esp
  float treeScatterAmount; // [esp-1AC0h] [ebp-1ACCh]
  float v6; // [esp-1AB4h] [ebp-1AC0h]
  unsigned int v7; // [esp-1AB0h] [ebp-1ABCh]
  unsigned int v8; // [esp-1AACh] [ebp-1AB8h]
  const float *CloudArea; // [esp-1AA4h] [ebp-1AB0h]
  GfxCmdBufSourceState v10; // [esp-1AA0h] [ebp-1AACh] BYREF
  unsigned int v11[3]; // [esp+0h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]

  v11[0] = a1;
  v11[1] = retaddr;
  v4 = alloca(6864);
  PIXBeginNamedEvent(-1, "R_DrawDecal");
  R_InitCmdBufSourceState(&v10, &viewInfo->input, 1);
  R_SetRenderTargetSize(&v10, viewInfo->sceneComposition.mainSceneMSAA);
  R_SetViewportStruct(&v10, &viewInfo->cullViewInfo.sceneViewport);
  R_SetWindShaderConstants(&v10);
  CloudArea = (const float *)R_GetCloudArea();
  *(float *)&v8 = CloudArea[1] - viewInfo->cullViewInfo.viewParms.origin[1];
  v7 = *((unsigned int *)CloudArea + 2);
  v6 = CloudArea[3];
  v10.input.consts[115][0] = *CloudArea - viewInfo->cullViewInfo.viewParms.origin[0];
  *(_QWORD *)&v10.gap0[1844] = __PAIR64__(v7, v8);
  v10.input.consts[115][3] = v6;
  R_DirtyCodeConstant(&v10, 0x73u);
  treeScatterAmount = rgp.world->sunParse.treeScatterAmount;
  v10.input.consts[119][0] = rgp.world->sunParse.treeScatterIntensity;
  *(_QWORD *)&v10.gap0[1908] = __PAIR64__(*(unsigned int *)&FLOAT_0_0, LODWORD(treeScatterAmount));
  v10.input.consts[119][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(&v10, 0x77u);
  R_SetADSZScaleConstants(viewInfo->localClientNum, &v10);
  R_DrawCall(
    (int)v11,
    (void (__cdecl *)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *))R_DrawDecalCallback,
    viewInfo,
    &v10,
    viewInfo,
    &viewInfo->drawList[3],
    &viewInfo->cullViewInfo.viewParms,
    cmdBuf,
    prepassCmdBuf);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

void __cdecl R_DrawDecalCallback(char *userData, GfxCmdBufContext context, GfxCmdBufContext prepassContext)
{
  R_SetRenderTarget(context, userData[13808]);
  if ( prepassContext.state )
    R_SetRenderTarget(prepassContext, userData[13808]);
  if ( (*((unsigned int *)userData + 3464) & 7) == 0 )
    R_HW_EnableScissor(
      context.state->prim.device,
      *((unsigned int *)userData + 88),
      *((unsigned int *)userData + 89),
      *((unsigned int *)userData + 90),
      *((unsigned int *)userData + 91));
  R_DrawSurfs(context, prepassContext.state, (const GfxDrawSurfListInfo *)(userData + 8992));
  if ( (*((unsigned int *)userData + 3464) & 7) == 0 )
    R_HW_DisableScissor(context.state->prim.device);
}

