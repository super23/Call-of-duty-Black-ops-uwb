#include "rb_draw3d.h"

const float (*__cdecl R_GetCloudArea())[4]
{
  return (const float (*)[4])r_cloudArea;
}

void __cdecl R_CalcGameTimeVec(float gameTime, float *out)
{
  long double v2; // st7
  float v3; // [esp+8h] [ebp-1Ch]
  float cosOfFracPartOfGameTime; // [esp+18h] [ebp-Ch]
  float sinOfFracPartOfGameTime; // [esp+1Ch] [ebp-8h]
  float fracPartOfGameTime; // [esp+20h] [ebp-4h]

  v3 = floor(gameTime);
  fracPartOfGameTime = gameTime - v3;
  v2 = (float)(6.2831855 * fracPartOfGameTime);
  cosOfFracPartOfGameTime = cos(v2);
  sinOfFracPartOfGameTime = sin(v2);
  *out = sinOfFracPartOfGameTime;
  out[1] = cosOfFracPartOfGameTime;
  out[2] = fracPartOfGameTime;
  out[3] = gameTime;
}

void __cdecl R_ShowTris(GfxCmdBufContext context, const GfxDrawSurfListInfo *info)
{
  int v2; // [esp+14h] [ebp-60h]
  jqBatchGroup *i; // [esp+18h] [ebp-5Ch]
  GfxDrawSurfListInfo debugInfo; // [esp+1Ch] [ebp-58h] BYREF
  int showTris; // [esp+70h] [ebp-4h]

  showTris = r_showTris->current.integer;
  if ( showTris )
  {
    v2 = 3;
    for ( i = debugInfo.group; --v2 >= 0; ++i )
    {
      i->QueuedBatchCount = 0;
      i->ExecutingBatchCount = 0;
    }
    if ( showTris != 3 || info == &info->viewInfo->drawList[4] )
    {
      if ( showTris == 2 )
        R_ClearScreen(context.state->prim.device, 6u, colorWhite, 1.0, 0, 0);
      memcpy(&debugInfo, info, sizeof(debugInfo));
      debugInfo.baseTechType = 125;
      R_DrawSurfs(context, 0, &debugInfo);
    }
  }
}

void  R_DrawEmissive(int a1@<ebp>, const GfxViewInfo *viewInfo, GfxCmdBuf *cmdBuf)
{
  void *v3; // esp
  GfxCmdBufSourceState v4; // [esp-1AA0h] [ebp-1AACh] BYREF
  unsigned int v5[3]; // [esp+0h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]

  v5[0] = a1;
  v5[1] = retaddr;
  v3 = alloca(6816);
  R_InitCmdBufSourceState(&v4, &viewInfo->input, 1);
  R_SetRenderTargetSize(&v4, viewInfo->sceneComposition.mainSceneMSAA);
  R_SetViewportStruct(&v4, &viewInfo->cullViewInfo.sceneViewport);
  R_SetADSZScaleConstants(viewInfo->localClientNum, &v4);
  R_DrawCall(
    (int)v5,
    (void (__cdecl *)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *))R_DrawEmissiveCallback,
    viewInfo,
    &v4,
    viewInfo,
    &viewInfo->drawList[4],
    &viewInfo->cullViewInfo.viewParms,
    cmdBuf,
    0);
}

void __cdecl R_DrawEmissiveCallback(char *userData, GfxCmdBufContext context)
{
  PIXBeginNamedEvent(-1, "R_DrawEmissiveCallback");
  R_UpdateCodeConstant(context.source, 0x5Eu, 0.0, 0.0, 1.0, 1.0);
  PIXBeginNamedEvent(-1, "emissive");
  R_DrawSurfs(context, 0, (const GfxDrawSurfListInfo *)(userData + 9072));
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  R_ShowTris(context, (const GfxDrawSurfListInfo *)(userData + 8752));
  R_ShowTris(context, (const GfxDrawSurfListInfo *)(userData + 8992));
  R_ShowTris(context, (const GfxDrawSurfListInfo *)(userData + 9072));
  if ( (*((unsigned int *)userData + 3464) & 7) == 0 )
    R_HW_DisableScissor(context.state->prim.device);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

void __cdecl R_HW_DisableScissor(IDirect3DDevice9 *device)
{
  const char *v1; // eax
  int semaphore; // [esp+0h] [ebp-8h]
  int hr; // [esp+4h] [ebp-4h]

  R_AssertDXDeviceOwnership();
  if ( r_logFile && r_logFile->current.integer )
    RB_LogPrint("device->SetRenderState( D3DRS_SCISSORTESTENABLE, 0 )\n");
  semaphore = R_AcquireDXDeviceOwnership(0);
  hr = ((int (__thiscall *)(IDirect3DDevice9 *, IDirect3DDevice9 *, int, unsigned int))device->SetRenderState)(
         device,
         device,
         174,
         0);
  if ( semaphore )
    R_ReleaseDXDeviceOwnership();
  if ( hr < 0 )
  {
    ++g_disableRendering;
    v1 = R_ErrorDescription(hr);
    Com_Error(
      ERR_FATAL,
      "c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_setstate_d3d.h (%i) device->SetRenderState( D3DRS_SCISSORTESTENABLE,"
      " 0 ) failed: %s\n",
      701,
      v1);
  }
}

void  R_DrawReflected(int a1@<ebp>, const GfxViewInfo *viewInfo, GfxCmdBuf *cmdBuf)
{
  void *v3; // esp
  GfxCmdBufSourceState v4; // [esp-1AA0h] [ebp-1AACh] BYREF
  unsigned int v5[3]; // [esp+0h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]

  v5[0] = a1;
  v5[1] = retaddr;
  v3 = alloca(6816);
  R_InitCmdBufSourceState(&v4, &viewInfo->input, 1);
  R_SetRenderTargetSize(&v4, viewInfo->sceneComposition.mainSceneMSAA);
  R_SetViewportStruct(&v4, &viewInfo->cullViewInfo.sceneViewport);
  R_DrawCall(
    (int)v5,
    (void (__cdecl *)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *))R_DrawReflectedCallback,
    viewInfo,
    &v4,
    viewInfo,
    &viewInfo->drawList[5],
    &viewInfo->cullViewInfo.viewParms,
    cmdBuf,
    0);
}

void __cdecl R_DrawReflectedCallback(char *userData, GfxCmdBufContext context)
{
  float clearColor[4]; // [esp+20h] [ebp-14h] BYREF
  unsigned __int8 surface; // [esp+33h] [ebp-1h]

  surface = 10;
  R_InitLocalCmdBufState(context.state);
  R_SetRenderTargetSize(context.source, 0xAu);
  R_SetRenderTarget(context, 0xAu);
  R_Set3D(context.source);
  memset(clearColor, 0, sizeof(clearColor));
  R_ClearScreen(context.state->prim.device, 1u, clearColor, 1.0, 0, 0);
  PIXBeginNamedEvent(-1, "reflected");
  R_DrawSurfs(context, 0, (const GfxDrawSurfListInfo *)(userData + 9152));
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  R_HW_DisableScissor(context.state->prim.device);
}

void __cdecl R_InitLocalCmdBufState(GfxCmdBufState *state)
{
  memcpy(state->refSamplerState, gfxCmdBufState.refSamplerState, sizeof(GfxCmdBufState));
  memset((unsigned __int8 *)state->vertexShaderConstState, 0, sizeof(state->vertexShaderConstState));
  memset((unsigned __int8 *)state->pixelShaderConstState, 0, sizeof(state->pixelShaderConstState));
}

bool __cdecl RB_ShouldDrawCoronas()
{
  return !R_StereoActivated() && !r_reflectionProbeGenerate->current.enabled;
}

void __cdecl RB_Draw3DInternal(const GfxViewInfo *viewInfo)
{
  if ( !rgp.world
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_draw3d.cpp", 7732, 0, "%s", "rgp.world") )
  {
    __debugbreak();
  }
  if ( gfxDrawMethod.drawScene )
  {
    if ( gfxDrawMethod.drawScene == GFX_DRAW_SCENE_FULLBRIGHT )
    {
      RB_FullbrightDrawCommands(viewInfo);
    }
    else if ( gfxDrawMethod.drawScene == GFX_DRAW_SCENE_DEBUGSHADER )
    {
      RB_DebugShaderDrawCommands(viewInfo);
    }
    else
    {
      if ( gfxDrawMethod.drawScene != GFX_DRAW_SCENE_STANDARD
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_draw3d.cpp",
              7775,
              0,
              "%s\n\t(gfxDrawMethod.drawScene) = %i",
              "(gfxDrawMethod.drawScene == GFX_DRAW_SCENE_STANDARD)",
              gfxDrawMethod.drawScene) )
      {
        __debugbreak();
      }
      RB_StandardDrawCommands(viewInfo);
    }
  }
  else
  {
    R_InitLocalCmdBufState(&gfxCmdBufState);
    R_SetRenderTargetSize(&gfxCmdBufSourceState, 2u);
    R_SetRenderTarget(gfxCmdBufContext, 2u);
    memcpy(gfxCmdBufState.refSamplerState, gfxCmdBufState.refSamplerState, sizeof(gfxCmdBufState));
  }
}

void __cdecl RB_FullbrightDrawCommands(const GfxViewInfo *viewInfo)
{
  const GfxBackEndData *data; // [esp+30h] [ebp-8h]
  GfxCmdBuf cmdBuf; // [esp+34h] [ebp-4h] BYREF

  PIXBeginNamedEvent(-1, "RB_FullbrightDrawCommands");
  data = backEndData;
  R_SetAndClearSceneTarget(0, viewInfo);
  R_InitContext(data, &cmdBuf);
  R_DrawFullbright(viewInfo, &gfxCmdBufContext.source->input, &cmdBuf);
  R_InitCmdBufSourceState(gfxCmdBufContext.source, &viewInfo->input, 0);
  R_InitLocalCmdBufState(gfxCmdBufContext.state);
  R_SetRenderTargetSize(gfxCmdBufContext.source, viewInfo->sceneComposition.mainSceneMSAA);
  R_SetRenderTarget(gfxCmdBufContext, viewInfo->sceneComposition.mainSceneMSAA);
  R_BeginView(gfxCmdBufContext.source, &viewInfo->sceneDef, &viewInfo->cullViewInfo.viewParms);
  R_SetViewportStruct(gfxCmdBufContext.source, &viewInfo->cullViewInfo.sceneViewport);
  RB_DrawSun(viewInfo->localClientNum);
  memcpy(gfxCmdBufState.refSamplerState, gfxCmdBufContext.state->refSamplerState, sizeof(gfxCmdBufState));
  RB_EndSceneRendering(gfxCmdBufContext, &viewInfo->input, viewInfo);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

void __cdecl RB_EndSceneRendering(GfxCmdBufContext context, const GfxCmdBufInput *input, const GfxViewInfo *viewInfo)
{
  PIXBeginNamedEvent(-1, "RB_EndSceneRendering()");
  R_HW_InsertFence(&backEndData->endFence);
  R_InitCmdBufSourceState(context.source, input, 0);
  R_InitLocalCmdBufState(context.state);
  R_SetRenderTargetSize(&gfxCmdBufSourceState, viewInfo->sceneComposition.mainSceneMSAA);
  R_BeginView(&gfxCmdBufSourceState, &viewInfo->sceneDef, &viewInfo->cullViewInfo.viewParms);
  R_SetViewportStruct(&gfxCmdBufSourceState, &viewInfo->cullViewInfo.sceneViewport);
  R_SetRenderTarget(gfxCmdBufContext, viewInfo->sceneComposition.mainSceneMSAA);
  if ( developer->current.integer )
  {
    R_Set3D(&gfxCmdBufSourceState);
    if ( gfxCmdBufSourceState.scissorViewport.width != 1
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_draw3d.cpp",
            4182,
            0,
            "%s\n\t(gfxCmdBufSourceState.viewMode) = %i",
            "(gfxCmdBufSourceState.viewMode == VIEW_MODE_3D)",
            gfxCmdBufSourceState.scissorViewport.width) )
    {
      __debugbreak();
    }
    RB_DrawDebug((const GfxViewParms *)gfxCmdBufSourceState.viewParms.viewMatrix.m[3]);
    RB_ShowCollision((const GfxViewParms *)gfxCmdBufSourceState.viewParms.viewMatrix.m[3]);
  }
  memcpy(gfxCmdBufState.refSamplerState, context.state->refSamplerState, sizeof(gfxCmdBufState));
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

void __cdecl R_SetAndClearSceneTarget(const GfxViewport *viewport, const GfxViewInfo *viewInfo)
{
  R_InitLocalCmdBufState(&gfxCmdBufState);
  PIXBeginNamedEvent(-1, "Clear Render Target");
  R_SetRenderTargetSize(&gfxCmdBufSourceState, viewInfo->sceneComposition.mainSceneMSAA);
  R_SetRenderTarget(gfxCmdBufContext, viewInfo->sceneComposition.mainSceneMSAA);
  if ( (viewInfo->sceneComposition.renderingMode & 7) != 0 )
    R_ClearForFrameBuffer(gfxCmdBufState.prim.device, 0);
  else
    R_ClearForFrameBuffer(gfxCmdBufState.prim.device, viewport);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  memcpy(gfxCmdBufState.refSamplerState, gfxCmdBufState.refSamplerState, sizeof(gfxCmdBufState));
}

void __cdecl R_ClearForFrameBuffer(IDirect3DDevice9 *device, const GfxViewport *viewport)
{
  float clearColor[4]; // [esp+10h] [ebp-10h] BYREF

  R_GetClearColor(clearColor);
  R_ClearScreen(device, 7u, clearColor, 1.0, 0, viewport);
}

void __cdecl R_DrawFullbright(const GfxViewInfo *viewInfo, GfxCmdBufInput *input, GfxCmdBuf *cmdBuf)
{
  int savedregs; // [esp+1Ch] [ebp+0h] BYREF

  PIXBeginNamedEvent(-1, "R_DrawFullbrightOrDebugShader - Lit");
  R_DrawFullbrightOrDebugShader(
    (int)&savedregs,
    (void (__cdecl *)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *))R_DrawFullbrightLitCallback,
    viewInfo,
    viewInfo->drawList,
    cmdBuf);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  PIXBeginNamedEvent(-1, "R_DrawFullbrightOrDebugShader - Decal");
  R_DrawFullbrightOrDebugShader(
    (int)&savedregs,
    (void (__cdecl *)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *))R_DrawFullbrightDecalCallback,
    viewInfo,
    &viewInfo->drawList[3],
    cmdBuf);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  PIXBeginNamedEvent(-1, "R_DrawFullbrightOrDebugShader - Emissive");
  R_DrawFullbrightOrDebugShader(
    (int)&savedregs,
    (void (__cdecl *)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *))R_DrawFullbrightEmissiveCallback,
    viewInfo,
    &viewInfo->drawList[4],
    cmdBuf);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

void __cdecl R_DrawFullbrightLitCallback(char *data, GfxCmdBufContext context)
{
  R_SetRenderTarget(context, data[13808]);
  if ( (*((unsigned int *)data + 3464) & 7) == 0 )
    R_HW_EnableScissor(
      context.state->prim.device,
      *((unsigned int *)data + 88),
      *((unsigned int *)data + 89),
      *((unsigned int *)data + 90),
      *((unsigned int *)data + 91));
  R_DrawSurfs(context, 0, (const GfxDrawSurfListInfo *)(data + 8752));
  if ( (*((unsigned int *)data + 3464) & 7) == 0 )
    R_HW_DisableScissor(context.state->prim.device);
}

void __cdecl R_HW_EnableScissor(
        IDirect3DDevice9 *device,
        unsigned int x,
        unsigned int y,
        unsigned int w,
        unsigned int h)
{
  const char *v5; // eax
  const char *v6; // eax
  int v7; // [esp+0h] [ebp-20h]
  int v8; // [esp+4h] [ebp-1Ch]
  int semaphore; // [esp+8h] [ebp-18h]
  int hr; // [esp+Ch] [ebp-14h]
  tagRECT scissor; // [esp+10h] [ebp-10h] BYREF

  scissor.left = x;
  scissor.top = y;
  scissor.right = w + x;
  scissor.bottom = h + y;
  R_AssertDXDeviceOwnership();
  if ( r_logFile && r_logFile->current.integer )
    RB_LogPrint("device->SetRenderState( D3DRS_SCISSORTESTENABLE, 1 )\n");
  semaphore = R_AcquireDXDeviceOwnership(0);
  hr = device->SetRenderState(device, D3DRS_SCISSORTESTENABLE, 1u);
  if ( semaphore )
    R_ReleaseDXDeviceOwnership();
  if ( hr < 0 )
  {
    ++g_disableRendering;
    v5 = R_ErrorDescription(hr);
    Com_Error(
      ERR_FATAL,
      "c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_setstate_d3d.h (%i) device->SetRenderState( D3DRS_SCISSORTESTENABLE,"
      " 1 ) failed: %s\n",
      694,
      v5);
  }
  R_AssertDXDeviceOwnership();
  if ( r_logFile && r_logFile->current.integer )
    RB_LogPrint("device->SetScissorRect( &scissor )\n");
  v7 = R_AcquireDXDeviceOwnership(0);
  v8 = device->SetScissorRect(device, &scissor);
  if ( v7 )
    R_ReleaseDXDeviceOwnership();
  if ( v8 < 0 )
  {
    ++g_disableRendering;
    v6 = R_ErrorDescription(v8);
    Com_Error(
      ERR_FATAL,
      "c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_setstate_d3d.h (%i) device->SetScissorRect( &scissor ) failed: %s\n",
      695,
      v6);
  }
}

void __cdecl R_DrawFullbrightDecalCallback(char *data, GfxCmdBufContext context)
{
  R_SetRenderTarget(context, data[13808]);
  if ( (*((unsigned int *)data + 3464) & 7) == 0 )
    R_HW_EnableScissor(
      context.state->prim.device,
      *((unsigned int *)data + 88),
      *((unsigned int *)data + 89),
      *((unsigned int *)data + 90),
      *((unsigned int *)data + 91));
  R_DrawSurfs(context, 0, (const GfxDrawSurfListInfo *)(data + 8992));
  if ( (*((unsigned int *)data + 3464) & 7) == 0 )
    R_HW_DisableScissor(context.state->prim.device);
}

void __cdecl R_DrawFullbrightEmissiveCallback(char *data, GfxCmdBufContext context)
{
  R_SetRenderTarget(context, data[13808]);
  if ( (*((unsigned int *)data + 3464) & 7) == 0 )
    R_HW_EnableScissor(
      context.state->prim.device,
      *((unsigned int *)data + 88),
      *((unsigned int *)data + 89),
      *((unsigned int *)data + 90),
      *((unsigned int *)data + 91));
  R_DrawSurfs(context, 0, (const GfxDrawSurfListInfo *)(data + 9072));
  R_ShowTris(context, (const GfxDrawSurfListInfo *)(data + 8752));
  R_ShowTris(context, (const GfxDrawSurfListInfo *)(data + 8832));
  R_ShowTris(context, (const GfxDrawSurfListInfo *)(data + 8992));
  R_ShowTris(context, (const GfxDrawSurfListInfo *)(data + 9072));
  if ( (*((unsigned int *)data + 3464) & 7) == 0 )
    R_HW_DisableScissor(context.state->prim.device);
}

void  R_DrawFullbrightOrDebugShader(
        int a1@<ebp>,
        void (__cdecl *callback)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *),
        const GfxViewInfo *viewInfo,
        const GfxDrawSurfListInfo *info,
        GfxCmdBuf *cmdBuf)
{
  void *v5; // esp
  float v6; // [esp-1AB4h] [ebp-1AC0h]
  unsigned int v7; // [esp-1AB0h] [ebp-1ABCh]
  unsigned int v8; // [esp-1AACh] [ebp-1AB8h]
  GfxCmdBufSourceState v9; // [esp-1AA0h] [ebp-1AACh] BYREF
  const float *CloudArea; // [esp-8h] [ebp-14h]
  unsigned int v11[3]; // [esp+0h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]

  v11[0] = a1;
  v11[1] = retaddr;
  v5 = alloca(6864);
  CloudArea = (const float *)R_GetCloudArea();
  R_InitCmdBufSourceState(&v9, &viewInfo->input, 1);
  R_SetRenderTargetSize(&v9, viewInfo->sceneComposition.mainSceneMSAA);
  R_SetViewportStruct(&v9, &viewInfo->cullViewInfo.sceneViewport);
  R_SetWindShaderConstants(&v9);
  memset(&v9.gap0[1824], 0, 16);
  R_DirtyCodeConstant(&v9, 0x72u);
  *(float *)&v8 = CloudArea[1] - viewInfo->cullViewInfo.viewParms.origin[1];
  v7 = *((unsigned int *)CloudArea + 2);
  v6 = CloudArea[3];
  v9.input.consts[115][0] = *CloudArea - viewInfo->cullViewInfo.viewParms.origin[0];
  *(_QWORD *)&v9.gap0[1844] = __PAIR64__(v7, v8);
  v9.input.consts[115][3] = v6;
  R_DirtyCodeConstant(&v9, 0x73u);
  *(_QWORD *)&v9.gap0[704] = __PAIR64__(*(unsigned int *)&FLOAT_0_0, LODWORD(FLOAT_1_0));
  v9.input.consts[44][2] = *(float *)&FLOAT_0_0;
  v9.input.consts[44][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(&v9, 0x2Cu);
  *(_QWORD *)&v9.gap0[720] = __PAIR64__(LODWORD(FLOAT_1_0), *(unsigned int *)&FLOAT_0_0);
  v9.input.consts[45][2] = *(float *)&FLOAT_0_0;
  v9.input.consts[45][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(&v9, 0x2Du);
  v9.input.consts[46][0] = *(float *)&FLOAT_0_0;
  *(_QWORD *)&v9.gap0[740] = __PAIR64__(LODWORD(FLOAT_1_0), *(unsigned int *)&FLOAT_0_0);
  v9.input.consts[46][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(&v9, 0x2Eu);
  R_DrawCall((int)v11, callback, viewInfo, &v9, viewInfo, info, &viewInfo->cullViewInfo.viewParms, cmdBuf, 0);
}

void __cdecl RB_DebugShaderDrawCommands(const GfxViewInfo *viewInfo)
{
  const GfxBackEndData *data; // [esp+8h] [ebp-8h]
  GfxCmdBuf cmdBuf; // [esp+Ch] [ebp-4h] BYREF

  data = backEndData;
  R_SetAndClearSceneTarget(0, viewInfo);
  R_InitContext(data, &cmdBuf);
  R_DrawDebugShader(viewInfo, &cmdBuf);
  RB_EndSceneRendering(gfxCmdBufContext, &viewInfo->input, viewInfo);
}

void __cdecl R_DrawDebugShader(const GfxViewInfo *viewInfo, GfxCmdBuf *cmdBuf)
{
  int savedregs; // [esp+0h] [ebp+0h] BYREF

  R_DrawFullbrightOrDebugShader(
    (int)&savedregs,
    (void (__cdecl *)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *))R_DrawDebugShaderLitCallback,
    viewInfo,
    viewInfo->drawList,
    cmdBuf);
  R_DrawFullbrightOrDebugShader(
    (int)&savedregs,
    (void (__cdecl *)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *))R_DrawDebugShaderDecalCallback,
    viewInfo,
    &viewInfo->drawList[3],
    cmdBuf);
  R_DrawFullbrightOrDebugShader(
    (int)&savedregs,
    (void (__cdecl *)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *))R_DrawDebugShaderEmissiveCallback,
    viewInfo,
    &viewInfo->drawList[4],
    cmdBuf);
}

void __cdecl R_DrawDebugShaderLitCallback(char *data, GfxCmdBufContext context)
{
  R_SetRenderTarget(context, 3u);
  R_HW_EnableScissor(
    context.state->prim.device,
    *((unsigned int *)data + 88),
    *((unsigned int *)data + 89),
    *((unsigned int *)data + 90),
    *((unsigned int *)data + 91));
  R_DrawSurfs(context, 0, (const GfxDrawSurfListInfo *)(data + 8752));
  R_HW_DisableScissor(context.state->prim.device);
}

void __cdecl R_DrawDebugShaderDecalCallback(char *data, GfxCmdBufContext context)
{
  R_SetRenderTarget(context, 3u);
  R_DrawSurfs(context, 0, (const GfxDrawSurfListInfo *)(data + 8992));
}

void __cdecl R_DrawDebugShaderEmissiveCallback(char *data, GfxCmdBufContext context)
{
  R_SetRenderTarget(context, 3u);
  R_HW_EnableScissor(
    context.state->prim.device,
    *((unsigned int *)data + 88),
    *((unsigned int *)data + 89),
    *((unsigned int *)data + 90),
    *((unsigned int *)data + 91));
  R_DrawSurfs(context, 0, (const GfxDrawSurfListInfo *)(data + 9072));
  R_ShowTris(context, (const GfxDrawSurfListInfo *)(data + 8752));
  R_ShowTris(context, (const GfxDrawSurfListInfo *)(data + 8832));
  R_ShowTris(context, (const GfxDrawSurfListInfo *)(data + 8992));
  R_ShowTris(context, (const GfxDrawSurfListInfo *)(data + 9072));
  R_HW_DisableScissor(context.state->prim.device);
}

void __cdecl RB_StandardDrawCommands(const GfxViewInfo *viewInfo)
{
  unsigned __int8 depthPrepassRT; // [esp+7Fh] [ebp-51h]
  float v2[4]; // [esp+80h] [ebp-50h] BYREF
  unsigned __int8 setupRenderTargetId; // [esp+91h] [ebp-3Fh]
  bool drawWaypoints; // [esp+93h] [ebp-3Dh]
  const GfxBackEndData *data; // [esp+94h] [ebp-3Ch]
  GfxCmdBuf cmdBuf; // [esp+98h] [ebp-38h] BYREF
  bool needsDepthPrepass; // [esp+9Ch] [ebp-34h]
  unsigned __int8 floatzRenderTarget; // [esp+9Dh] [ebp-33h]
  bool applyPostFX; // [esp+9Eh] [ebp-32h]
  bool resolveDistortion; // [esp+9Fh] [ebp-31h]
  ShadowType dynamicShadowType; // [esp+A0h] [ebp-30h]
  int isRenderingFullScreen; // [esp+A4h] [ebp-2Ch]
  float clearColor[4]; // [esp+A8h] [ebp-28h] BYREF
  GfxViewport oldSceneViewport; // [esp+B8h] [ebp-18h]
  unsigned __int8 whichToClearForScene; // [esp+CBh] [ebp-5h]
  bool isMissileCam; // [esp+CCh] [ebp-4h]
  unsigned __int8 whichToClearForSetup; // [esp+CDh] [ebp-3h]
  bool drawReflected; // [esp+CEh] [ebp-2h]
  bool need_float_z; // [esp+CFh] [ebp-1h]

  PIXBeginNamedEvent(-1, "RB_StandardDrawCommands");
  data = backEndData;
  isMissileCam = viewInfo->isMissileCamera;
  floatzRenderTarget = isMissileCam ? 23 : 7;
  resolveDistortion = 1;
  drawReflected = !isMissileCam;
  drawWaypoints = drawReflected;
  applyPostFX = drawReflected;
  if ( !isMissileCam )
    RB_UI3D_RenderToTexture(viewInfo->cmds, &viewInfo->rbUI3D, &viewInfo->input);
  dynamicShadowType = viewInfo->dynamicShadowType;
  isRenderingFullScreen = viewInfo->isRenderingFullScreen;
  oldSceneViewport = viewInfo->sceneViewport;
  if ( dynamicShadowType == SHADOW_MAP )
  {
    if ( Com_BitCheckAssert(data->shadowableLightHasShadowMap, rgp.world->sunPrimaryLightIndex, 32) )
      RB_SunShadowMaps(data, viewInfo);
    RB_SpotShadowMaps(data, viewInfo);
  }
  needsDepthPrepass = !isMissileCam;
  whichToClearForScene = 7;
  R_InitContext(data, &cmdBuf);
  if ( viewInfo->needsFloatZ
    && !dword_B50E834[5 * (viewInfo->isMissileCamera ? 23 : 7)]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_draw3d.cpp",
          7133,
          0,
          "%s",
          "!viewInfo->needsFloatZ || R_HaveFloatZ(viewInfo->isMissileCamera)") )
  {
    __debugbreak();
  }
  need_float_z = viewInfo->needsFloatZ;
  viewInfo->needsFloatZ = 1;
  setupRenderTargetId = floatzRenderTarget;
  whichToClearForSetup = 7;
  if ( !dx.supportsIntZ || isMissileCam )
  {
    D3DPERF_BeginEvent(-1, L"Float Z");
    R_InitLocalCmdBufState(&gfxCmdBufState);
    R_SetRenderTargetSize(&gfxCmdBufSourceState, setupRenderTargetId);
    R_SetRenderTarget(gfxCmdBufContext, setupRenderTargetId);
    R_ClearScreen(gfxCmdBufState.prim.device, whichToClearForSetup, colorWhite, 1.0, 0, 0);
    memcpy(gfxCmdBufState.refSamplerState, gfxCmdBufState.refSamplerState, sizeof(gfxCmdBufState));
    R_InitContext(data, &cmdBuf);
    viewInfo->renderingFloatZ = 1;
    R_DepthPrepass(setupRenderTargetId, viewInfo, &cmdBuf);
    viewInfo->renderingFloatZ = 0;
    D3DPERF_EndEvent();
  }
  if ( viewInfo->isMissileCamera )
  {
    R_InitLocalCmdBufState(&gfxCmdBufState);
    PIXBeginNamedEvent(-1, "Clear ExtraCam Render Target");
    R_SetRenderTargetSize(&gfxCmdBufSourceState, 0x16u);
    R_SetRenderTarget(gfxCmdBufContext, 0x16u);
    memset(v2, 0, 12);
    v2[3] = FLOAT_1_0;
    R_ClearScreen(gfxCmdBufState.prim.device, 7u, v2, 1.0, 0, 0);
    if ( GetCurrentThreadId() == g_DXDeviceThread )
      D3DPERF_EndEvent();
  }
  else
  {
    R_InitLocalCmdBufState(&gfxCmdBufState);
    R_SetRenderTargetSize(&gfxCmdBufSourceState, viewInfo->sceneComposition.mainSceneMSAA);
    R_SetRenderTarget(gfxCmdBufContext, viewInfo->sceneComposition.mainSceneMSAA);
    if ( R_GetClearColor(clearColor) || (whichToClearForScene & 0xFE) != 0 )
      R_ClearScreen(gfxCmdBufState.prim.device, 7u, clearColor, 1.0, 0, 0);
  }
  memcpy(gfxCmdBufState.refSamplerState, gfxCmdBufState.refSamplerState, sizeof(gfxCmdBufState));
  if ( needsDepthPrepass )
  {
    D3DPERF_BeginEvent(-1, L"R_DepthPrepass");
    R_InitContext(data, &cmdBuf);
    depthPrepassRT = dx.supportsSceneNullRenderTarget + 3;
    R_SetRenderTargetSize(&gfxCmdBufSourceState, depthPrepassRT);
    R_SetRenderTarget(gfxCmdBufContext, depthPrepassRT);
    R_DepthPrepass(viewInfo->sceneComposition.mainSceneMSAA, viewInfo, &cmdBuf);
    D3DPERF_EndEvent();
  }
  if ( viewInfo->isMissileCamera )
  {
    R_SetRenderTargetSize(&gfxCmdBufSourceState, 0x16u);
    R_SetRenderTarget(gfxCmdBufContext, 0x16u);
  }
  else if ( dx.supportsSceneNullRenderTarget )
  {
    R_SetRenderTargetSize(&gfxCmdBufSourceState, 3u);
    R_SetRenderTarget(gfxCmdBufContext, 3u);
  }
  viewInfo->needsFloatZ = need_float_z;
  if ( viewInfo->drawList[6].drawSurfCount )
  {
    R_InitContext(data, &cmdBuf);
    PIXBeginNamedEvent(-1, "CloakPrePass");
    R_DrawCloakHDR(viewInfo, &cmdBuf, CLOAK_PHASE_PREPASS);
    if ( GetCurrentThreadId() == g_DXDeviceThread )
      D3DPERF_EndEvent();
  }
  R_InitContext(data, &cmdBuf);
  PIXBeginNamedEvent(-1, "Lit");
  R_DrawLit(viewInfo, &cmdBuf, 0, LIT_PHASE_MAIN);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  if ( dx.supportsIntZ && !viewInfo->isMissileCamera )
  {
    PIXBeginNamedEvent(-1, "Resolve IntZ");
    R_InitCmdBufSourceState(&gfxCmdBufSourceState, &viewInfo->input, 0);
    R_InitLocalCmdBufState(&gfxCmdBufState);
    R_SetRenderTargetSize(gfxCmdBufContext.source, viewInfo->sceneComposition.mainSceneMSAA);
    R_SetRenderTarget(gfxCmdBufContext, viewInfo->sceneComposition.mainSceneMSAA);
    R_ResolveIntZ_PC();
    memcpy(gfxCmdBufState.refSamplerState, gfxCmdBufState.refSamplerState, sizeof(gfxCmdBufState));
    if ( GetCurrentThreadId() == g_DXDeviceThread )
      D3DPERF_EndEvent();
  }
  if ( (viewInfo->sceneComposition.renderingMode & 7) == 0 )
    R_ResolveDistortion(viewInfo);
  if ( drawReflected )
  {
    R_InitContext(data, &cmdBuf);
    PIXBeginNamedEvent(-1, "Reflected");
    R_DrawReflected(viewInfo, &cmdBuf);
    if ( g_DXDeviceThread == GetCurrentThreadId() )
      D3DPERF_EndEvent();
  }
  PIXBeginNamedEvent(-1, "LitPostResolve");
  R_InitContext(data, &cmdBuf);
  R_DrawLit(viewInfo, &cmdBuf, 0, LIT_PHASE_POST_RESOLVE);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  if ( r_setFrameBufferAlpha->current.enabled )
    RB_SetFrameBufferAlpha();
  R_InitContext(data, &cmdBuf);
  R_DrawDecal(viewInfo, &cmdBuf, 0);
  if ( viewInfo->isRenderingFullScreen != isRenderingFullScreen
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_draw3d.cpp",
          7334,
          0,
          "%s",
          "viewInfo->isRenderingFullScreen == isRenderingFullScreen") )
  {
    __debugbreak();
  }
  R_InitCmdBufSourceState(&gfxCmdBufSourceState, &viewInfo->input, 0);
  R_InitLocalCmdBufState(&gfxCmdBufState);
  R_SetRenderTargetSize(&gfxCmdBufSourceState, viewInfo->sceneComposition.mainSceneMSAA);
  R_SetRenderTarget(gfxCmdBufContext, viewInfo->sceneComposition.mainSceneMSAA);
  R_BeginView(&gfxCmdBufSourceState, &viewInfo->sceneDef, &viewInfo->cullViewInfo.viewParms);
  R_SetViewportStruct(&gfxCmdBufSourceState, &viewInfo->cullViewInfo.sceneViewport);
  RB_DrawSun(viewInfo->localClientNum);
  if ( RB_ShouldDrawCoronas(isMissileCam) )
    RB_DrawCoronas(viewInfo->localClientNum);
  if ( !isMissileCam )
    RB_DrawSuperFlareOccluders(viewInfo);
  if ( dx.supportsIntZ && !viewInfo->isMissileCamera )
  {
    PIXBeginNamedEvent(-1, "Resolve IntZ");
    R_InitCmdBufSourceState(&gfxCmdBufSourceState, &viewInfo->input, 0);
    R_InitLocalCmdBufState(&gfxCmdBufState);
    R_SetRenderTargetSize(gfxCmdBufContext.source, viewInfo->sceneComposition.mainSceneMSAA);
    R_SetRenderTarget(gfxCmdBufContext, viewInfo->sceneComposition.mainSceneMSAA);
    R_ResolveIntZ_PC();
    memcpy(gfxCmdBufState.refSamplerState, gfxCmdBufState.refSamplerState, sizeof(gfxCmdBufState));
    if ( GetCurrentThreadId() == g_DXDeviceThread )
      D3DPERF_EndEvent();
  }
  memcpy(gfxCmdBufState.refSamplerState, gfxCmdBufState.refSamplerState, sizeof(gfxCmdBufState));
  if ( r_enableDlights->current.enabled )
  {
    R_InitContext(data, &cmdBuf);
    PIXBeginNamedEvent(-1, "Dynamic Lights");
    R_DrawLights(viewInfo, &cmdBuf);
    if ( GetCurrentThreadId() == g_DXDeviceThread )
      D3DPERF_EndEvent();
  }
  R_ResolveDistortion(viewInfo);
  R_InitContext(data, &cmdBuf);
  PIXBeginNamedEvent(-1, "Emissive");
  R_DrawEmissive(viewInfo, &cmdBuf);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  if ( drawWaypoints )
  {
    R_InitCmdBufSourceState(&gfxCmdBufSourceState, &viewInfo->input, 0);
    R_InitLocalCmdBufState(&gfxCmdBufState);
    R_SetRenderTargetSize(&gfxCmdBufSourceState, viewInfo->sceneComposition.mainSceneMSAA);
    R_SetRenderTarget(gfxCmdBufContext, viewInfo->sceneComposition.mainSceneMSAA);
    R_BeginView(&gfxCmdBufSourceState, &viewInfo->sceneDef, &viewInfo->cullViewInfo.viewParms);
    R_SetViewportStruct(&gfxCmdBufSourceState, &viewInfo->cullViewInfo.sceneViewport);
    RB_DrawWaypoints(viewInfo->localClientNum);
    memcpy(gfxCmdBufState.refSamplerState, gfxCmdBufState.refSamplerState, sizeof(gfxCmdBufState));
  }
  if ( viewInfo->postEmissiveBrightening > 0.0 )
  {
    PIXBeginNamedEvent(-1, "postEmissiveBrightening");
    R_InitLocalCmdBufState(&gfxCmdBufState);
    R_SetRenderTargetSize(&gfxCmdBufSourceState, viewInfo->sceneComposition.mainSceneMSAA);
    R_SetRenderTarget(gfxCmdBufContext, viewInfo->sceneComposition.mainSceneMSAA);
    R_SetViewportStruct(&gfxCmdBufSourceState, &viewInfo->cullViewInfo.sceneViewport);
    RB_DrawFullScreenColoredQuad(
      rgp.blendMaterialNoDepth,
      0.0,
      0.0,
      1.0,
      1.0,
      (unsigned __int8)(int)(float)((float)(viewInfo->postEmissiveBrightening * 255.0) + 0.5));
    memcpy(gfxCmdBufState.refSamplerState, gfxCmdBufState.refSamplerState, sizeof(gfxCmdBufState));
    if ( GetCurrentThreadId() == g_DXDeviceThread )
      D3DPERF_EndEvent();
  }
  if ( viewInfo->drawList[6].drawSurfCount )
  {
    if ( (viewInfo->sceneComposition.renderingMode & 7) == 0 )
      R_Resolve(gfxCmdBufContext, stru_B50E8A8.image);
    R_InitContext(data, &cmdBuf);
    PIXBeginNamedEvent(-1, "CloakPostEmissive");
    R_DrawCloakHDR(viewInfo, &cmdBuf, CLOAK_PHASE_CLOAKED);
    if ( GetCurrentThreadId() == g_DXDeviceThread )
      D3DPERF_EndEvent();
  }
  if ( !isMissileCam )
    RB_EndSceneRendering(gfxCmdBufContext, &viewInfo->input, viewInfo);
  if ( applyPostFX )
    RB_StandardPostEffects(viewInfo);
  if ( !isRenderingFullScreen && !isMissileCam )
    viewInfo->cullViewInfo.sceneViewport = oldSceneViewport;
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

void RB_SetFrameBufferAlpha()
{
  if ( tess.indexCount )
    RB_EndTessSurface();
  dword_B473FCC = 1;
  R_Set2D(&gfxCmdBufSourceState);
  R_InitLocalCmdBufState(&gfxCmdBufState);
  gfxCmdBufSourceState.viewParms.viewProjectionMatrix.m[1][2] = FLOAT_1_0;
  gfxCmdBufSourceState.viewParms.inverseViewProjectionMatrix.m[1][2] = FLOAT_1_0;
  RB_DrawStretchPicZ(
    rgp.setAlphaMaterial,
    0.0,
    0.0,
    -1.0,
    (float)dword_B473FD0,
    (float)dword_B473FD4,
    0.0,
    0.0,
    1.0,
    1.0,
    0xFFFFFFFF,
    GFX_PRIM_STATS_CODE);
  RB_DrawStretchPicZ(
    rgp.setAlphaMaterial,
    0.0,
    0.0,
    0.0,
    (float)dword_B473FD0,
    (float)dword_B473FD4,
    0.0,
    0.0,
    1.0,
    1.0,
    0xFFFFFFu,
    GFX_PRIM_STATS_CODE);
  RB_EndTessSurface();
  memcpy(gfxCmdBufState.refSamplerState, gfxCmdBufState.refSamplerState, sizeof(gfxCmdBufState));
}

void  R_DrawLights(int a1@<ebp>, const GfxViewInfo *viewInfo, GfxCmdBuf *cmdBuf)
{
  void *v3; // esp
  int integer; // [esp-1AB0h] [ebp-1ABCh]
  unsigned int v5; // [esp-1AACh] [ebp-1AB8h]
  GfxCmdBufSourceState v6; // [esp-1AA0h] [ebp-1AACh] BYREF
  int v7; // [esp+0h] [ebp-Ch]
  void *v8; // [esp+4h] [ebp-8h]
  void *retaddr; // [esp+Ch] [ebp+0h]

  v7 = a1;
  v8 = retaddr;
  v3 = alloca(6848);
  R_InitCmdBufSourceState(&v6, &viewInfo->input, 1);
  R_SetRenderTargetSize(&v6, viewInfo->sceneComposition.mainSceneMSAA);
  R_SetViewportStruct(&v6, &viewInfo->cullViewInfo.sceneViewport);
  R_SetWindShaderConstants(&v6);
  memset(&v6.gap0[1824], 0, 16);
  R_DirtyCodeConstant(&v6, 0x72u);
  *(float *)&v5 = (float)r_treeScale->current.integer;
  integer = r_testScale->current.integer;
  LODWORD(v6.input.consts[68][0]) = r_skyTransition->current.integer;
  *(_QWORD *)&v6.gap0[1092] = __PAIR64__(v5, *(unsigned int *)&FLOAT_0_0);
  LODWORD(v6.input.consts[68][3]) = integer;
  R_DirtyCodeConstant(&v6, 0x44u);
  R_DrawPointLitSurfs(&v6, viewInfo, cmdBuf);
}

void __cdecl R_DrawPointLitSurfs(GfxCmdBufSourceState *source, const GfxViewInfo *viewInfo, GfxCmdBuf *cmdBuf)
{
  float v3; // [esp+24h] [ebp-144h]
  float v4; // [esp+28h] [ebp-140h]
  float v5; // [esp+2Ch] [ebp-13Ch]
  float v6; // [esp+30h] [ebp-138h]
  float v7; // [esp+34h] [ebp-134h]
  float v8; // [esp+4Ch] [ebp-11Ch]
  float radius; // [esp+50h] [ebp-118h]
  float edgePoint; // [esp+88h] [ebp-E0h]
  float edgePoint_4; // [esp+8Ch] [ebp-DCh]
  float edgePoint_8; // [esp+90h] [ebp-D8h]
  GfxPointLitSurfsInfo info; // [esp+9Ch] [ebp-CCh] BYREF
  int pointLightCount; // [esp+B8h] [ebp-B0h]
  const GfxBackEndData *data; // [esp+BCh] [ebp-ACh]
  float perp[3]; // [esp+C0h] [ebp-A8h] BYREF
  float dist[4]; // [esp+CCh] [ebp-9Ch]
  float halfWidth; // [esp+DCh] [ebp-8Ch]
  float width; // [esp+E0h] [ebp-88h]
  float height; // [esp+E4h] [ebp-84h]
  const GfxLight *light; // [esp+E8h] [ebp-80h]
  float sign; // [esp+ECh] [ebp-7Ch]
  const PointLightPartition *pointLightPartition; // [esp+F0h] [ebp-78h]
  float offset[3]; // [esp+F4h] [ebp-74h] BYREF
  float edgeGoalPoint[3]; // [esp+100h] [ebp-68h]
  float tangentDist; // [esp+10Ch] [ebp-5Ch]
  float edgeDir[3]; // [esp+110h] [ebp-58h] BYREF
  float planeDist; // [esp+11Ch] [ebp-4Ch]
  int partitionIndex; // [esp+120h] [ebp-48h]
  const GfxViewParms *viewParms; // [esp+124h] [ebp-44h]
  float tangentDistSq; // [esp+128h] [ebp-40h]
  GfxColor color; // [esp+12Ch] [ebp-3Ch] BYREF
  float x; // [esp+130h] [ebp-38h]
  float y; // [esp+134h] [ebp-34h]
  unsigned int axis; // [esp+138h] [ebp-30h]
  float offsetDistSq; // [esp+13Ch] [ebp-2Ch]
  GfxDrawPrimArgs args; // [esp+140h] [ebp-28h]
  unsigned int planeIndex; // [esp+14Ch] [ebp-1Ch]
  float halfHeight; // [esp+150h] [ebp-18h]
  float perpDir[3]; // [esp+154h] [ebp-14h] BYREF
  float perpDist; // [esp+160h] [ebp-8h]
  float w; // [esp+164h] [ebp-4h]
  int savedregs; // [esp+168h] [ebp+0h] BYREF

  if ( !viewInfo
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_draw3d.cpp", 4872, 0, "%s", "viewInfo") )
  {
    __debugbreak();
  }
  pointLightCount = viewInfo->pointLightCount;
  if ( pointLightCount )
  {
    R_ConvertColorToBytes(colorWhite, (unsigned __int8 *)&color);
    data = source->input.data;
    args.baseIndex = 0;
    args.vertexCount = 4;
    args.triCount = 2;
    viewParms = &viewInfo->cullViewInfo.viewParms;
    info.viewInfo = viewInfo;
    for ( partitionIndex = 0; partitionIndex < pointLightCount; ++partitionIndex )
    {
      pointLightPartition = &viewInfo->pointLightPartitions[partitionIndex];
      light = &pointLightPartition->light;
      info.drawSurfInfo = &pointLightPartition->info;
      info.clearQuadMesh = &viewInfo->pointLightMeshData[partitionIndex];
      dist[0] = FLOAT_1_0;
      dist[1] = FLOAT_N1_0;
      dist[2] = FLOAT_1_0;
      dist[3] = FLOAT_N1_0;
      offset[0] = pointLightPartition->light.origin[0] - viewParms->origin[0];
      offset[1] = pointLightPartition->light.origin[1] - viewParms->origin[1];
      offset[2] = pointLightPartition->light.origin[2] - viewParms->origin[2];
      offsetDistSq = (float)((float)(offset[0] * offset[0]) + (float)(offset[1] * offset[1]))
                   + (float)(offset[2] * offset[2]);
      tangentDistSq = offsetDistSq - (float)(pointLightPartition->light.radius * pointLightPartition->light.radius);
      if ( tangentDistSq > 1.0 )
      {
        tangentDist = fsqrt(tangentDistSq);
        perpDist = offsetDistSq / tangentDist;
        sign = FLOAT_1_0;
        planeIndex = 0;
        while ( planeIndex < 4 )
        {
          axis = planeIndex >> 1;
          perp[0] = sign * viewParms->axis[2 - (planeIndex >> 1)][0];
          perp[1] = sign * viewParms->axis[2 - (planeIndex >> 1)][1];
          perp[2] = sign * viewParms->axis[2 - (planeIndex >> 1)][2];
          Vec3Cross(perp, offset, perpDir);
          Vec3Normalize(perpDir);
          edgeGoalPoint[0] = (float)(COERCE_FLOAT(LODWORD(perpDist) ^ _mask__NegFloat_) * perpDir[0])
                           + viewParms->origin[0];
          edgeGoalPoint[1] = (float)(COERCE_FLOAT(LODWORD(perpDist) ^ _mask__NegFloat_) * perpDir[1])
                           + viewParms->origin[1];
          edgeGoalPoint[2] = (float)(COERCE_FLOAT(LODWORD(perpDist) ^ _mask__NegFloat_) * perpDir[2])
                           + viewParms->origin[2];
          edgeDir[0] = edgeGoalPoint[0] - light->origin[0];
          edgeDir[1] = edgeGoalPoint[1] - light->origin[1];
          edgeDir[2] = edgeGoalPoint[2] - light->origin[2];
          Vec3Normalize(edgeDir);
          radius = light->radius;
          edgePoint = (float)(radius * edgeDir[0]) + light->origin[0];
          edgePoint_4 = (float)(radius * edgeDir[1]) + light->origin[1];
          edgePoint_8 = (float)(radius * edgeDir[2]) + light->origin[2];
          if ( (float)((float)((float)((float)(viewInfo->frustumPlanes[planeIndex][0] * edgePoint)
                                     + (float)(viewInfo->frustumPlanes[planeIndex][1] * edgePoint_4))
                             + (float)(viewInfo->frustumPlanes[planeIndex][2] * edgePoint_8))
                     + viewInfo->frustumPlanes[planeIndex][3]) > 0.0 )
          {
            w = (float)((float)((float)(edgePoint * viewParms->viewProjectionMatrix.m[0][3])
                              + (float)(edgePoint_4 * viewParms->viewProjectionMatrix.m[1][3]))
                      + (float)(edgePoint_8 * viewParms->viewProjectionMatrix.m[2][3]))
              + viewParms->viewProjectionMatrix.m[3][3];
            if ( w > 0.0 )
            {
              planeDist = (float)((float)((float)(edgePoint * viewParms->viewProjectionMatrix.m[0][axis])
                                        + (float)(edgePoint_4 * viewParms->viewProjectionMatrix.m[1][axis]))
                                + (float)(edgePoint_8 * viewParms->viewProjectionMatrix.m[2][axis]))
                        + viewParms->viewProjectionMatrix.m[3][axis];
              if ( (float)((float)(planeDist / w) - 1.0) < 0.0 )
                v8 = planeDist / w;
              else
                v8 = FLOAT_1_0;
              if ( (float)(-1.0 - (float)(planeDist / w)) < 0.0 )
                v7 = v8;
              else
                v7 = FLOAT_N1_0;
              dist[planeIndex] = v7;
            }
          }
          ++planeIndex;
          LODWORD(sign) ^= _mask__NegFloat_;
        }
      }
      halfWidth = (float)(viewInfo->cullViewInfo.sceneViewport.width >> 1);
      halfHeight = (float)(viewInfo->cullViewInfo.sceneViewport.height >> 1);
      x = (float)(dist[1] + 1.0) * halfWidth;
      y = (float)(1.0 - dist[2]) * halfHeight;
      width = (float)(dist[0] - dist[1]) * halfWidth;
      height = (float)(dist[2] - dist[3]) * halfHeight;
      v6 = floor(x);
      info.x = (int)v6;
      v5 = floor(y);
      info.y = (int)v5;
      v4 = ceil((float)((float)(dist[0] + 1.0) * halfWidth));
      info.w = (int)v4 - info.x;
      v3 = ceil((float)((float)(1.0 - dist[3]) * halfHeight));
      info.h = (int)v3 - info.y;
      info.x += viewInfo->cullViewInfo.sceneViewport.x;
      info.y += viewInfo->cullViewInfo.sceneViewport.y;
      R_SetQuadMeshData(info.clearQuadMesh, x, y, width, height, 0.0, 0.0, 1.0, 1.0, 0xFFFFFFFF);
      R_DrawCall(
        (int)&savedregs,
        (void (__cdecl *)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *))R_DrawPointLitSurfsCallback,
        &info,
        source,
        viewInfo,
        &pointLightPartition->info,
        &viewInfo->cullViewInfo.viewParms,
        cmdBuf,
        0);
    }
  }
}

void __cdecl R_DrawPointLitSurfsCallback(GfxMeshData **userData, GfxCmdBufContext context)
{
  R_SetRenderTarget(context, (*userData)[383].vb.total);
  R_Set2D(context.source);
  R_DrawQuadMesh(context, rgp.clearAlphaMaterial, userData[1]);
  R_Set3D(context.source);
  if ( ((*userData)[384].vertSize & 7) == 0 )
    R_HW_EnableScissor(
      context.state->prim.device,
      (unsigned int)userData[3],
      (unsigned int)userData[4],
      (unsigned int)userData[5],
      (unsigned int)userData[6]);
  R_DrawSurfs(context, 0, (const GfxDrawSurfListInfo *)userData[2]);
  if ( ((*userData)[384].vertSize & 7) == 0 )
    R_HW_DisableScissor(context.state->prim.device);
}

void __cdecl R_ResolveDistortion(const GfxViewInfo *viewInfo)
{
  unsigned __int8 mainSceneMSAA; // [esp+0h] [ebp-18h]
  unsigned __int8 resolveSrc; // [esp+17h] [ebp-1h]

  PIXBeginNamedEvent(-1, "Resolve Distortion");
  resolveSrc = viewInfo->sceneComposition.mainSceneMSAA;
  if ( resolveSrc == 3 )
    mainSceneMSAA = 5;
  else
    mainSceneMSAA = viewInfo->sceneComposition.mainSceneMSAA;
  if ( resolveSrc == mainSceneMSAA )
  {
    if ( g_DXDeviceThread != GetCurrentThreadId() )
      return;
  }
  else
  {
    R_SetRenderTargetSize(&gfxCmdBufSourceState, resolveSrc);
    R_SetRenderTarget(gfxCmdBufContext, resolveSrc);
    R_Resolve(gfxCmdBufContext, gfxRenderTargets[mainSceneMSAA].image);
    if ( g_DXDeviceThread != GetCurrentThreadId() )
      return;
  }
  D3DPERF_EndEvent();
}

void __cdecl RB_StandardPostEffects(const GfxViewInfo *viewInfo)
{
  const GfxBackEndData *data; // [esp+Ch] [ebp-4h]

  data = backEndData;
  PIXBeginNamedEvent(-1, "RB_StandardPostEffects");
  viewInfo->input.data = data;
  R_InitCmdBufSourceState(&gfxCmdBufSourceState, &viewInfo->input, 0);
  R_InitLocalCmdBufState(&gfxCmdBufState);
  R_SetResolvedScene(gfxCmdBufContext);
  R_BeginView(&gfxCmdBufSourceState, &viewInfo->sceneDef, &viewInfo->cullViewInfo.viewParms);
  R_SetViewportStruct(&gfxCmdBufSourceState, &viewInfo->cullViewInfo.sceneViewport);
  RB_ApplyLatePostEffects(viewInfo);
  R_SetRenderTargetSize(&gfxCmdBufSourceState, 3u);
  R_SetRenderTarget(gfxCmdBufContext, 3u);
  RB_DrawSunPostEffects(viewInfo->localClientNum, viewInfo->sunVisibility);
  memcpy(gfxCmdBufState.refSamplerState, gfxCmdBufState.refSamplerState, sizeof(gfxCmdBufState));
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

void __cdecl R_SetResolvedScene(GfxCmdBufContext context)
{
  R_SetRenderTargetSize(context.source, 3u);
  R_SetRenderTarget(context, 3u);
}

void __cdecl RB_ApplyLatePostEffects(const GfxViewInfo *viewInfo)
{
  PIXBeginNamedEvent(-1, "RB_ApplyLatePostEffects");
  RB_ProcessPostEffects(viewInfo);
  R_SetRenderTargetSize(&gfxCmdBufSourceState, 3u);
  R_SetRenderTarget(gfxCmdBufContext, 3u);
  RB_DrawDebugPostEffects();
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

void RB_DrawDebugPostEffects()
{
  if ( !r_showFbColorDebug
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_draw3d.cpp",
          5170,
          0,
          "%s",
          "r_showFbColorDebug") )
  {
    __debugbreak();
  }
  if ( !r_showFloatZDebug
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_draw3d.cpp",
          5171,
          0,
          "%s",
          "r_showFloatZDebug") )
  {
    __debugbreak();
  }
  if ( r_showFbColorDebug->current.integer == 1 )
  {
    RB_ShowFbColorDebug_Screen();
  }
  else if ( r_showFbColorDebug->current.integer == 2 )
  {
    RB_ShowFbColorDebug_Feedback();
  }
  else if ( r_showFloatZDebug->current.enabled )
  {
    RB_ShowFloatZDebug();
  }
}

void RB_ShowFbColorDebug_Screen()
{
  float halfScreenWidth; // [esp+38h] [ebp-10h]
  float halfScreenHeight; // [esp+3Ch] [ebp-Ch]
  float quarterScreenWidth; // [esp+40h] [ebp-8h]

  if ( tess.indexCount )
    RB_EndTessSurface();
  R_Set2D(&gfxCmdBufSourceState);
  halfScreenWidth = (float)dword_B473FD0 * 0.5;
  halfScreenHeight = (float)dword_B473FD4 * 0.5;
  quarterScreenWidth = halfScreenWidth * 0.5;
  RB_DrawStretchPic(
    rgp.frameColorDebugMaterial,
    quarterScreenWidth,
    halfScreenHeight * 0.5,
    quarterScreenWidth,
    halfScreenHeight * 0.5,
    0.0,
    0.0,
    1.0,
    1.0,
    0xFFFF0000,
    GFX_PRIM_STATS_CODE);
  RB_DrawStretchPic(
    rgp.frameColorDebugMaterial,
    halfScreenWidth,
    halfScreenHeight * 0.5,
    halfScreenWidth * 0.5,
    halfScreenHeight * 0.5,
    0.0,
    0.0,
    1.0,
    1.0,
    0xFF00FF00,
    GFX_PRIM_STATS_CODE);
  RB_DrawStretchPic(
    rgp.frameColorDebugMaterial,
    quarterScreenWidth,
    halfScreenHeight,
    quarterScreenWidth,
    halfScreenHeight * 0.5,
    0.0,
    0.0,
    1.0,
    1.0,
    0xFF0000FF,
    GFX_PRIM_STATS_CODE);
  RB_DrawStretchPic(
    rgp.frameAlphaDebugMaterial,
    halfScreenWidth,
    halfScreenHeight,
    halfScreenWidth * 0.5,
    halfScreenHeight * 0.5,
    0.0,
    0.0,
    1.0,
    1.0,
    0xFFFFFFFF,
    GFX_PRIM_STATS_CODE);
  RB_EndTessSurface();
}

void RB_ShowFbColorDebug_Feedback()
{
  float quarterScreenHeight; // [esp+40h] [ebp-10h]
  float halfScreenWidth; // [esp+44h] [ebp-Ch]
  float halfScreenHeight; // [esp+48h] [ebp-8h]
  float quarterScreenWidth; // [esp+4Ch] [ebp-4h]

  if ( tess.indexCount )
    RB_EndTessSurface();
  halfScreenWidth = (float)dword_B473FD0 * 0.5;
  halfScreenHeight = (float)dword_B473FD4 * 0.5;
  quarterScreenWidth = halfScreenWidth * 0.5;
  quarterScreenHeight = halfScreenHeight * 0.5;
  R_SetRenderTargetSize(&gfxCmdBufSourceState, 2u);
  R_SetRenderTarget(gfxCmdBufContext, 2u);
  R_Set2D(&gfxCmdBufSourceState);
  gfxCmdBufSourceState.input.consts[44][0] = FLOAT_1_0;
  gfxCmdBufSourceState.input.consts[44][1] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[44][2] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[44][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x2Cu);
  gfxCmdBufSourceState.input.consts[45][0] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[45][1] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[45][2] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[45][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x2Du);
  gfxCmdBufSourceState.input.consts[46][0] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[46][1] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[46][2] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[46][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x2Eu);
  RB_DrawStretchPic(
    rgp.colorChannelMixerMaterial,
    quarterScreenWidth,
    quarterScreenHeight,
    quarterScreenWidth,
    quarterScreenHeight,
    0.25,
    0.25,
    0.5,
    0.5,
    0xFFFFFFFF,
    GFX_PRIM_STATS_CODE);
  RB_EndTessSurface();
  gfxCmdBufSourceState.input.consts[44][0] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[44][1] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[44][2] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[44][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x2Cu);
  gfxCmdBufSourceState.input.consts[45][0] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[45][1] = FLOAT_1_0;
  gfxCmdBufSourceState.input.consts[45][2] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[45][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x2Du);
  gfxCmdBufSourceState.input.consts[46][0] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[46][1] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[46][2] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[46][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x2Eu);
  RB_DrawStretchPic(
    rgp.colorChannelMixerMaterial,
    halfScreenWidth,
    quarterScreenHeight,
    halfScreenWidth * 0.5,
    quarterScreenHeight,
    0.5,
    0.25,
    0.75,
    0.5,
    0xFFFFFFFF,
    GFX_PRIM_STATS_CODE);
  RB_EndTessSurface();
  Vec4Set(gfxCmdBufSourceState.input.consts[44], 0.0, 0.0, 0.0, 0.0);
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x2Cu);
  Vec4Set(gfxCmdBufSourceState.input.consts[45], 0.0, 0.0, 0.0, 0.0);
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x2Du);
  Vec4Set(gfxCmdBufSourceState.input.consts[46], 0.0, 0.0, 1.0, 0.0);
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x2Eu);
  RB_DrawStretchPic(
    rgp.colorChannelMixerMaterial,
    quarterScreenWidth,
    halfScreenHeight,
    quarterScreenWidth,
    halfScreenHeight * 0.5,
    0.25,
    0.5,
    0.5,
    0.75,
    0xFFFFFFFF,
    GFX_PRIM_STATS_CODE);
  RB_EndTessSurface();
  R_SetCodeConstant(&gfxCmdBufSourceState, 0x2Cu, 0.0, 0.0, 0.0, 1.0);
  R_SetCodeConstant(&gfxCmdBufSourceState, 0x2Du, 0.0, 0.0, 0.0, 1.0);
  R_SetCodeConstant(&gfxCmdBufSourceState, 0x2Eu, 0.0, 0.0, 0.0, 1.0);
  RB_DrawStretchPic(
    rgp.colorChannelMixerMaterial,
    halfScreenWidth,
    halfScreenHeight,
    halfScreenWidth * 0.5,
    halfScreenHeight * 0.5,
    0.5,
    0.5,
    0.75,
    0.75,
    0xFFFFFFFF,
    GFX_PRIM_STATS_CODE);
  RB_EndTessSurface();
}

void RB_ShowFloatZDebug()
{
  float halfScreenWidth; // [esp+2Ch] [ebp-Ch]
  float halfScreenHeight; // [esp+30h] [ebp-8h]

  if ( stru_B50E8BC.surface.color )
  {
    if ( tess.indexCount )
      RB_EndTessSurface();
    halfScreenWidth = (float)dword_B473FD0 * 0.5;
    halfScreenHeight = (float)dword_B473FD4 * 0.5;
    R_Set2D(&gfxCmdBufSourceState);
    RB_DrawStretchPic(
      rgp.floatZDisplayMaterial,
      halfScreenWidth * 0.5,
      halfScreenHeight * 0.5,
      halfScreenWidth,
      halfScreenHeight,
      0.25,
      0.25,
      0.75,
      0.75,
      0xFFFFFFFF,
      GFX_PRIM_STATS_CODE);
    RB_EndTessSurface();
  }
}

void __cdecl RB_Draw3DCommon()
{
  if ( !rgp.world
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_draw3d.cpp", 7786, 0, "%s", "rgp.world") )
  {
    __debugbreak();
  }
  if ( gfxDrawMethod.drawScene )
  {
    if ( gfxDrawMethod.drawScene == GFX_DRAW_SCENE_FULLBRIGHT )
    {
      RB_FullbrightDrawCommandsCommon();
    }
    else if ( gfxDrawMethod.drawScene == GFX_DRAW_SCENE_DEBUGSHADER )
    {
      RB_DebugShaderDrawCommandsCommon();
    }
    else
    {
      if ( gfxDrawMethod.drawScene != GFX_DRAW_SCENE_STANDARD
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_draw3d.cpp",
              7816,
              0,
              "%s\n\t(gfxDrawMethod.drawScene) = %i",
              "(gfxDrawMethod.drawScene == GFX_DRAW_SCENE_STANDARD)",
              gfxDrawMethod.drawScene) )
      {
        __debugbreak();
      }
      RB_StandardDrawCommandsCommon();
    }
  }
}

unsigned intRB_FullbrightDrawCommandsCommon()
{
  unsigned intresult; // eax
  const GfxBackEndData *data; // [esp+14h] [ebp-Ch]
  unsigned int viewInfoIndex; // [esp+1Ch] [ebp-4h]

  PIXBeginNamedEvent(-1, "RB_FullbrightDrawCommandsCommon");
  data = backEndData;
  for ( viewInfoIndex = 0; viewInfoIndex < data->viewInfoCount; ++viewInfoIndex )
    RB_FullbrightRenderCommands(&data->viewInfo[viewInfoIndex]);
  result = GetCurrentThreadId();
  if ( result == (unsigned int)g_DXDeviceThread )
  {
    result = 0;
    if ( !HIDWORD(g_DXDeviceThread) )
      return D3DPERF_EndEvent();
  }
  return result;
}

void __cdecl RB_FullbrightRenderCommands(const GfxViewInfo *viewInfo)
{
  const GfxBackEndData *data; // [esp+14h] [ebp-8h]

  PIXBeginNamedEvent(-1, "RB_FullbrightRenderCommands");
  data = backEndData;
  viewInfo->input.data = backEndData;
  R_InitCmdBufSourceState(gfxCmdBufContext.source, &gfxCmdBufInput, 0);
  gfxCmdBufContext.source->input.data = data;
  R_InitLocalCmdBufState(gfxCmdBufContext.state);
  R_SetRenderTargetSize(gfxCmdBufContext.source, 2u);
  R_SetRenderTarget(gfxCmdBufContext, 2u);
  R_BeginView(gfxCmdBufContext.source, &viewInfo->sceneDef, &viewInfo->cullViewInfo.viewParms);
  R_SetViewportStruct(gfxCmdBufContext.source, &viewInfo->cullViewInfo.displayViewport);
  if ( viewInfo->cmds )
  {
    RB_SetUI3DSamplerAndConstants(gfxCmdBufContext.source, &viewInfo->rbUI3D);
    RB_ExecuteRenderCommandsLoop(viewInfo->cmds, 0);
  }
  memcpy(gfxCmdBufState.refSamplerState, gfxCmdBufContext.state->refSamplerState, sizeof(gfxCmdBufState));
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

GfxCmdBufSourceState *RB_DebugShaderDrawCommandsCommon()
{
  GfxCmdBufSourceState *result; // eax
  const GfxBackEndData *data; // [esp+8h] [ebp-Ch]
  unsigned int viewInfoIndex; // [esp+10h] [ebp-4h]

  result = gfxCmdBufContext.source;
  data = backEndData;
  for ( viewInfoIndex = 0; viewInfoIndex < data->viewInfoCount; ++viewInfoIndex )
  {
    RB_DebugShaderRenderCommands(&data->viewInfo[viewInfoIndex]);
    result = (GfxCmdBufSourceState *)(viewInfoIndex + 1);
  }
  return result;
}

void __cdecl RB_DebugShaderRenderCommands(const GfxViewInfo *viewInfo)
{
  const GfxBackEndData *data; // [esp+14h] [ebp-8h]

  PIXBeginNamedEvent(-1, "RB_DebugShaderRenderCommands");
  data = backEndData;
  viewInfo->input.data = backEndData;
  R_InitCmdBufSourceState(gfxCmdBufContext.source, &gfxCmdBufInput, 0);
  gfxCmdBufContext.source->input.data = data;
  R_InitLocalCmdBufState(gfxCmdBufContext.state);
  R_SetResolvedScene(gfxCmdBufContext);
  R_BeginView(gfxCmdBufContext.source, &viewInfo->sceneDef, &viewInfo->cullViewInfo.viewParms);
  R_SetViewportStruct(gfxCmdBufContext.source, &viewInfo->cullViewInfo.displayViewport);
  if ( viewInfo->cmds )
  {
    RB_SetUI3DSamplerAndConstants(gfxCmdBufContext.source, &viewInfo->rbUI3D);
    RB_ExecuteRenderCommandsLoop(viewInfo->cmds, 0);
  }
  memcpy(gfxCmdBufState.refSamplerState, gfxCmdBufContext.state->refSamplerState, sizeof(gfxCmdBufState));
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
}

unsigned intRB_StandardDrawCommandsCommon()
{
  unsigned intresult; // eax
  float w; // [esp+40h] [ebp-34h]
  unsigned int viewInfoIndex; // [esp+64h] [ebp-10h]
  const GfxBackEndData *data; // [esp+6Ch] [ebp-8h]
  const GfxViewInfo *viewInfo; // [esp+70h] [ebp-4h]
  GfxViewInfo *viewInfoa; // [esp+70h] [ebp-4h]

  PIXBeginNamedEvent(-1, "RB_StandardDrawCommandsCommon");
  data = backEndData;
  if ( !backEndData->viewInfoCount )
  {
    result = GetCurrentThreadId();
    if ( result != g_DXDeviceThread )
      return result;
    return D3DPERF_EndEvent();
  }
  for ( viewInfoIndex = 0; viewInfoIndex < data->viewInfoCount; ++viewInfoIndex )
  {
    viewInfo = &data->viewInfo[viewInfoIndex];
    if ( !viewInfo->isMissileCamera )
      RB_StandardRenderCommands(viewInfo);
  }
  viewInfoa = backEndData->viewInfo;
  R_InitCmdBufSourceState(&gfxCmdBufSourceState, &viewInfoa->input, 0);
  R_SetRenderTargetSize(&gfxCmdBufSourceState, 2u);
  R_SetRenderTarget(gfxCmdBufContext, 2u);
  R_BeginView(&gfxCmdBufSourceState, &viewInfoa->sceneDef, &viewInfoa->cullViewInfo.viewParms);
  R_SetViewportStruct(&gfxCmdBufSourceState, &viewInfoa->cullViewInfo.displayViewport);
  R_Set2D(&gfxCmdBufSourceState);
  R_InitLocalCmdBufState(&gfxCmdBufState);
  if ( r_ui3d_debug_display->current.enabled )
  {
    R_SetCodeImageTexture(&gfxCmdBufSourceState, 8u, image.image);
    RB_DrawStretchPic(
      rgp.feedbackReplaceMaterial,
      50.0,
      40.0,
      (float)((float)image.width / (float)image.height) * 175.0,
      175.0,
      0.0,
      0.0,
      1.0,
      1.0,
      0xFFFFFFFF,
      GFX_PRIM_STATS_HUD);
    RB_EndTessSurface();
  }
  if ( r_missile_cam_debug_display->current.integer )
  {
    w = R_ExtraCam_AspectRatio() * 200.0;
    R_SetCodeImageTexture(&gfxCmdBufSourceState, 8u, rgp.blackImage);
    RB_DrawStretchPic(
      rgp.feedbackReplaceMaterial,
      50.0 - 3.0,
      250.0 - 3.0,
      (float)(3.0 * 2.0) + w,
      (float)(3.0 * 2.0) + 200.0,
      0.0,
      0.0,
      1.0,
      1.0,
      0xFFFFFFFF,
      GFX_PRIM_STATS_HUD);
    RB_EndTessSurface();
    if ( rt.image )
      R_SetCodeImageTexture(&gfxCmdBufSourceState, 8u, rt.image);
    RB_DrawStretchPic(
      rgp.feedbackReplaceMaterial,
      50.0,
      250.0,
      w,
      200.0,
      0.0,
      0.0,
      1.0,
      1.0,
      0xFFFFFFFF,
      GFX_PRIM_STATS_HUD);
    RB_EndTessSurface();
  }
  if ( viewInfoa->dynamicShadowType == SHADOW_MAP && sm_showOverlay->current.integer )
  {
    if ( sm_showOverlay->current.integer != 1
      && sm_showOverlay->current.integer != 2
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_draw3d.cpp",
            7710,
            0,
            "%s\n\t(sm_showOverlay->current.integer) = %i",
            "(sm_showOverlay->current.integer == GFX_SM_OVERLAY_SUN || sm_showOverlay->current.integer == GFX_SM_OVERLAY_SPOT)",
            sm_showOverlay->current.integer) )
    {
      __debugbreak();
    }
    if ( sm_showOverlay->current.integer == 1 )
      RB_DrawSunShadowOverlay();
    else
      RB_DrawSpotShadowOverlay();
  }
  memcpy(gfxCmdBufState.refSamplerState, gfxCmdBufState.refSamplerState, sizeof(gfxCmdBufState));
  result = GetCurrentThreadId();
  if ( result == (unsigned int)g_DXDeviceThread )
  {
    result = 0;
    if ( !HIDWORD(g_DXDeviceThread) )
      return D3DPERF_EndEvent();
  }
  return result;
}

void __cdecl R_SetCodeImageTexture(GfxCmdBufSourceState *source, unsigned int codeTexture, const GfxImage *image)
{
  if ( !source && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state.h", 1859, 0, "%s", "source") )
    __debugbreak();
  if ( codeTexture >= 0x2B
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state.h",
          1860,
          0,
          "codeTexture doesn't index TEXTURE_SRC_CODE_COUNT\n\t%i not in [0, %i)",
          codeTexture,
          43) )
  {
    __debugbreak();
  }
  source->input.codeImages[codeTexture] = image;
  source->input.codeImageRenderTargetControl[codeTexture].packed = 0;
}

void __cdecl RB_StandardRenderCommands(const GfxViewInfo *viewInfo)
{
  const GfxBackEndData *data; // [esp+18h] [ebp-8h]

  data = backEndData;
  PIXBeginNamedEvent(-1, "RB_StandardRenderCommands");
  R_StartADSZScaleFrame();
  viewInfo->input.data = data;
  R_InitCmdBufSourceState(&gfxCmdBufSourceState, &gfxCmdBufInput, 0);
  gfxCmdBufSourceState.input.data = backEndData;
  R_InitLocalCmdBufState(&gfxCmdBufState);
  R_SetRenderTargetSize(&gfxCmdBufSourceState, 2u);
  R_SetRenderTarget(gfxCmdBufContext, 2u);
  if ( (viewInfo->sceneComposition.renderingMode & 7) != 0 )
  {
    if ( (viewInfo->sceneComposition.renderingMode & 0x40000000) != 0 )
      gfxCmdBufSourceState.input.codeImageRenderTargetControl[10].packed = (unsigned int)&loc_800000
                                                                         | (viewInfo->sceneComposition.mainSceneFinal << 24)
                                                                         | 0x62;
    else
      gfxCmdBufSourceState.input.codeImageRenderTargetControl[10].packed = (unsigned int)&loc_800000
                                                                         | (viewInfo->sceneComposition.mainSceneSaved << 24)
                                                                         | 0x62;
  }
  R_BeginView(&gfxCmdBufSourceState, &viewInfo->sceneDef, &viewInfo->cullViewInfo.viewParms);
  R_SetViewportStruct(&gfxCmdBufSourceState, &viewInfo->cullViewInfo.displayViewport);
  gfxCmdBufSourceState.input.consts[170][0] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[170][1] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[170][2] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[170][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0xAAu);
  gfxCmdBufSourceState.input.consts[171][0] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[171][1] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[171][2] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[171][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0xABu);
  if ( viewInfo->cmds )
  {
    RB_SetUI3DSamplerAndConstants(&gfxCmdBufSourceState, &viewInfo->rbUI3D);
    RB_ExecuteRenderCommandsLoop(viewInfo->cmds, 0);
  }
  memcpy(gfxCmdBufState.refSamplerState, gfxCmdBufState.refSamplerState, sizeof(gfxCmdBufState));
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

