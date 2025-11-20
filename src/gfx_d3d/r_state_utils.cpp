#include "r_state_utils.h"

void __cdecl R_InitCmdBufSourceState(GfxCmdBufSourceState *source, const GfxCmdBufInput *input, int cameraView)
{
  float *v3; // eax
  unsigned int constant; // [esp+Ch] [ebp-4h]

  if ( !source
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state_utils.cpp", 33, 0, "%s", "source") )
  {
    __debugbreak();
  }
  if ( !input
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state_utils.cpp", 34, 0, "%s", "input") )
  {
    __debugbreak();
  }
  memset((unsigned __int8 *)source, 0, 0x1A90u);
  memcpy(&source->2048, input, 0xE90u);
  for ( constant = 0; constant < 0xC5; ++constant )
  {
    if ( input == &gfxCmdBufInput || s_codeConstUpdateFreq[constant] != 2 || constant < 0x2F )
    {
      source->input.consts[constant][0] = FLOAT_3_4028235e38;
      v3 = source->input.consts[constant];
      v3[1] = FLOAT_3_4028235e38;
      v3[2] = FLOAT_3_4028235e38;
      v3[3] = *(float *)&FLOAT_0_0;
    }
    else
    {
      source->constVersions[constant + 24] = 1;
    }
  }
  if ( source->cameraView
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state_utils.cpp",
          68,
          1,
          "%s",
          "source->shadowableLightForShadowLookupMatrix == PRIMARY_LIGHT_NONE") )
  {
    __debugbreak();
  }
  *(float *)&source->sceneViewport.x = FLOAT_1_0;
  *(float *)&source->scissorViewport.x = FLOAT_1_0;
  source->scissorViewport.y = cameraView;
}

void __cdecl R_InitCmdBufState(GfxCmdBufState *state)
{
  unsigned int samplerIndex; // [esp+14h] [ebp-Ch]
  IDirect3DDevice9 *device; // [esp+18h] [ebp-8h]
  unsigned int forceSamplerState; // [esp+1Ch] [ebp-4h]

  if ( !state
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state_utils.cpp", 131, 0, "%s", "state") )
  {
    __debugbreak();
  }
  device = state->prim.device;
  memset(state->refSamplerState, 0, sizeof(GfxCmdBufState));
  state->prim.device = device;
  state->depthRangeType = GFX_DEPTH_RANGE_FULL;
  state->depthRangeNear = *(float *)&FLOAT_0_0;
  state->depthRangeFar = FLOAT_1_0;
  state->renderTargetId = 0;
  R_SetDefaultStateBits(state->activeStateBits);
  R_SetDefaultStateBits(state->refStateBits);
  R_SetCompleteState(device, state->activeStateBits);
  R_SetDefaultAlphaTestFunction(state);
  forceSamplerState = R_DecodeSamplerState(1u);
  for ( samplerIndex = 0; samplerIndex < 0x10; ++samplerIndex )
  {
    R_HW_ForceSamplerState(state->prim.device, samplerIndex, forceSamplerState);
    state->refSamplerState[samplerIndex] = 1;
    state->samplerState[samplerIndex] = forceSamplerState;
  }
  state->prim.vertexDecl = (IDirect3DVertexDeclaration9 *)-1;
  state->pixelShader = (const MaterialPixelShader *)-1;
  state->stateOverride = 0;
  state->prim.viewStats = state->prim.frameStats.viewStats;
}

void __cdecl R_HW_ForceSamplerState(IDirect3DDevice9 *device, unsigned int samplerIndex, unsigned int samplerState)
{
  const char *v3; // eax
  const char *v4; // eax
  const char *v5; // eax
  const char *v6; // eax
  const char *v7; // eax
  const char *v8; // eax
  const char *v9; // eax
  int v10; // [esp+0h] [ebp-4Ch]
  int v11; // [esp+4h] [ebp-48h]
  int v12; // [esp+8h] [ebp-44h]
  int v13; // [esp+Ch] [ebp-40h]
  int v14; // [esp+10h] [ebp-3Ch]
  int v15; // [esp+14h] [ebp-38h]
  int v16; // [esp+18h] [ebp-34h]
  int v17; // [esp+1Ch] [ebp-30h]
  int v18; // [esp+20h] [ebp-2Ch]
  int v19; // [esp+24h] [ebp-28h]
  int v20; // [esp+28h] [ebp-24h]
  int v21; // [esp+2Ch] [ebp-20h]
  int semaphore; // [esp+30h] [ebp-1Ch]
  int hr; // [esp+34h] [ebp-18h]

  R_AssertDXDeviceOwnership();
  if ( r_logFile && r_logFile->current.integer )
    RB_LogPrint("device->SetSamplerState( samplerIndex, D3DSAMP_MINFILTER, minFilter )\n");
  semaphore = R_AcquireDXDeviceOwnership(0);
  hr = ((int (__thiscall *)(IDirect3DDevice9 *, IDirect3DDevice9 *, unsigned int, int, int))device->SetSamplerState)(
         device,
         device,
         samplerIndex,
         6,
         (unsigned __int16)(samplerState & 0xF00) >> 8);
  if ( semaphore )
    R_ReleaseDXDeviceOwnership();
  if ( hr < 0 )
  {
    ++g_disableRendering;
    v3 = R_ErrorDescription(hr);
    Com_Error(
      ERR_FATAL,
      "c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_setstate_d3d.h (%i) device->SetSamplerState( samplerIndex, D3DSAMP_M"
      "INFILTER, minFilter ) failed: %s\n",
      288,
      v3);
  }
  R_AssertDXDeviceOwnership();
  if ( r_logFile && r_logFile->current.integer )
    RB_LogPrint("device->SetSamplerState( samplerIndex, D3DSAMP_MAGFILTER, magFilter )\n");
  v20 = R_AcquireDXDeviceOwnership(0);
  v21 = device->SetSamplerState(
          device,
          samplerIndex,
          D3DSAMP_MAGFILTER,
          (unsigned __int16)(samplerState & 0xF000) >> 12);
  if ( v20 )
    R_ReleaseDXDeviceOwnership();
  if ( v21 < 0 )
  {
    ++g_disableRendering;
    v4 = R_ErrorDescription(v21);
    Com_Error(
      ERR_FATAL,
      "c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_setstate_d3d.h (%i) device->SetSamplerState( samplerIndex, D3DSAMP_M"
      "AGFILTER, magFilter ) failed: %s\n",
      291,
      v4);
  }
  if ( (unsigned __int8)samplerState > 1u )
  {
    R_AssertDXDeviceOwnership();
    if ( r_logFile && r_logFile->current.integer )
      RB_LogPrint("device->SetSamplerState( samplerIndex, D3DSAMP_MAXANISOTROPY, anisotropy )\n");
    v18 = R_AcquireDXDeviceOwnership(0);
    v19 = device->SetSamplerState(device, samplerIndex, D3DSAMP_MAXANISOTROPY, (unsigned __int8)samplerState);
    if ( v18 )
      R_ReleaseDXDeviceOwnership();
    if ( v19 < 0 )
    {
      ++g_disableRendering;
      v5 = R_ErrorDescription(v19);
      Com_Error(
        ERR_FATAL,
        "c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_setstate_d3d.h (%i) device->SetSamplerState( samplerIndex, D3DSAMP"
        "_MAXANISOTROPY, anisotropy ) failed: %s\n",
        295,
        v5);
    }
  }
  R_AssertDXDeviceOwnership();
  if ( r_logFile && r_logFile->current.integer )
    RB_LogPrint("device->SetSamplerState( samplerIndex, D3DSAMP_MIPFILTER, mipFilter )\n");
  v16 = R_AcquireDXDeviceOwnership(0);
  v17 = ((int (__thiscall *)(IDirect3DDevice9 *, IDirect3DDevice9 *, unsigned int, int, unsigned int))device->SetSamplerState)(
          device,
          device,
          samplerIndex,
          7,
          (samplerState & 0xF0000) >> 16);
  if ( v16 )
    R_ReleaseDXDeviceOwnership();
  if ( v17 < 0 )
  {
    ++g_disableRendering;
    v6 = R_ErrorDescription(v17);
    Com_Error(
      ERR_FATAL,
      "c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_setstate_d3d.h (%i) device->SetSamplerState( samplerIndex, D3DSAMP_M"
      "IPFILTER, mipFilter ) failed: %s\n",
      298,
      v6);
  }
  R_AssertDXDeviceOwnership();
  if ( r_logFile && r_logFile->current.integer )
    RB_LogPrint("device->SetSamplerState( samplerIndex, D3DSAMP_ADDRESSU, address )\n");
  v14 = R_AcquireDXDeviceOwnership(0);
  v15 = device->SetSamplerState(device, samplerIndex, D3DSAMP_ADDRESSU, (samplerState & 0x300000) >> 20);
  if ( v14 )
    R_ReleaseDXDeviceOwnership();
  if ( v15 < 0 )
  {
    ++g_disableRendering;
    v7 = R_ErrorDescription(v15);
    Com_Error(
      ERR_FATAL,
      "c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_setstate_d3d.h (%i) device->SetSamplerState( samplerIndex, D3DSAMP_A"
      "DDRESSU, address ) failed: %s\n",
      305,
      v7);
  }
  R_AssertDXDeviceOwnership();
  if ( r_logFile && r_logFile->current.integer )
    RB_LogPrint("device->SetSamplerState( samplerIndex, D3DSAMP_ADDRESSV, address )\n");
  v12 = R_AcquireDXDeviceOwnership(0);
  v13 = device->SetSamplerState(
          device,
          samplerIndex,
          D3DSAMP_ADDRESSV,
          ((unsigned int)&loc_C00000 & samplerState) >> 22);
  if ( v12 )
    R_ReleaseDXDeviceOwnership();
  if ( v13 < 0 )
  {
    ++g_disableRendering;
    v8 = R_ErrorDescription(v13);
    Com_Error(
      ERR_FATAL,
      "c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_setstate_d3d.h (%i) device->SetSamplerState( samplerIndex, D3DSAMP_A"
      "DDRESSV, address ) failed: %s\n",
      308,
      v8);
  }
  R_AssertDXDeviceOwnership();
  if ( r_logFile && r_logFile->current.integer )
    RB_LogPrint("device->SetSamplerState( samplerIndex, D3DSAMP_ADDRESSW, address )\n");
  v10 = R_AcquireDXDeviceOwnership(0);
  v11 = ((int (__thiscall *)(IDirect3DDevice9 *, IDirect3DDevice9 *, unsigned int, int, unsigned int))device->SetSamplerState)(
          device,
          device,
          samplerIndex,
          3,
          (samplerState & 0x3000000) >> 24);
  if ( v10 )
    R_ReleaseDXDeviceOwnership();
  if ( v11 < 0 )
  {
    ++g_disableRendering;
    v9 = R_ErrorDescription(v11);
    Com_Error(
      ERR_FATAL,
      "c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_setstate_d3d.h (%i) device->SetSamplerState( samplerIndex, D3DSAMP_A"
      "DDRESSW, address ) failed: %s\n",
      311,
      v9);
  }
}

void __cdecl R_SetDefaultAlphaTestFunction(GfxCmdBufState *state)
{
  const char *v1; // eax
  int semaphore; // [esp+0h] [ebp-Ch]
  int hr; // [esp+4h] [ebp-8h]
  IDirect3DDevice9 *device; // [esp+8h] [ebp-4h]

  device = state->prim.device;
  if ( !device && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state.h", 2011, 0, "%s", "device") )
    __debugbreak();
  R_AssertDXDeviceOwnership();
  if ( r_logFile && r_logFile->current.integer )
    RB_LogPrint("device->SetRenderState( D3DRS_ALPHAREF, 0 )\n");
  semaphore = R_AcquireDXDeviceOwnership(0);
  hr = device->SetRenderState(device, D3DRS_ALPHAREF, 0);
  if ( semaphore )
    R_ReleaseDXDeviceOwnership();
  if ( hr < 0 )
  {
    ++g_disableRendering;
    v1 = R_ErrorDescription(hr);
    Com_Error(
      ERR_FATAL,
      "c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state.h (%i) device->SetRenderState( D3DRS_ALPHAREF, 0 ) failed: %s\n",
      2013,
      v1);
  }
  state->alphaRef = 0;
}

unsigned int __cdecl R_DecodeSamplerState(unsigned __int8 samplerState)
{
  unsigned int tableIndex; // [esp+0h] [ebp-8h]

  tableIndex = samplerState & 0x1F;
  if ( tableIndex >= 0x18
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state.h",
          2421,
          0,
          "tableIndex doesn't index ARRAY_COUNT(s_decodeSamplerFilterState)\n\t%i not in [0, %i)",
          tableIndex,
          24) )
  {
    __debugbreak();
  }
  return (unsigned int)&cls.rankedServers[14655].mapName[3]
       | s_decodeSamplerFilterState[tableIndex]
       | ((samplerState & 0x20) << 16)
       | ((samplerState & 0x40) << 17)
       | ((samplerState & 0x80) << 18);
}

void __cdecl R_SetDefaultStateBits(unsigned int *stateBits)
{
  *stateBits = 0;
  stateBits[1] = 0;
  *stateBits |= 0x800u;
  *stateBits |= 0x18000000u;
  *stateBits |= 0x4000u;
  *stateBits = *stateBits;
  *stateBits = *stateBits;
  stateBits[1] |= 2u;
  stateBits[1] = stateBits[1];
}

void __cdecl R_InitCmdBufSamplerState(GfxCmdBufState *state)
{
  unsigned int samplerIndex; // [esp+8h] [ebp-Ch]
  unsigned int forceSamplerState; // [esp+10h] [ebp-4h]

  if ( !state
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state_utils.cpp", 208, 0, "%s", "state") )
  {
    __debugbreak();
  }
  forceSamplerState = R_DecodeSamplerState(1u);
  for ( samplerIndex = 0; samplerIndex < 0x10; ++samplerIndex )
  {
    R_HW_ForceSamplerState(state->prim.device, samplerIndex, forceSamplerState);
    state->refSamplerState[samplerIndex] = 1;
    state->samplerState[samplerIndex] = forceSamplerState;
  }
}

GfxCmdBufSourceState *__cdecl R_GetActiveWorldMatrix(GfxCmdBufSourceState *source)
{
  R_WorldMatrixChanged(source);
  return source;
}

void __cdecl R_WorldMatrixChanged(GfxCmdBufSourceState *source)
{
  ++HIWORD(source->viewParms3D);
  ++LOWORD(source->skinnedPlacement.base.quat[0]);
  ++LOWORD(source->skinnedPlacement.base.quat[1]);
  ++LOWORD(source->skinnedPlacement.base.quat[2]);
  source->constVersions[221] = HIWORD(source->viewParms3D);
}

void __cdecl R_Set2D(GfxCmdBufSourceState *source)
{
  GfxViewport viewport; // [esp+10h] [ebp-10h] BYREF
  int savedregs; // [esp+20h] [ebp+0h] BYREF

  if ( source->scissorViewport.width != 2 )
  {
    PIXBeginNamedEvent(-1, "R_Set2D");
    source->scissorViewport.width = 2;
    LOBYTE(source[1].matrices.matrix[0].m[2][2]) = 1;
    source->skinnedPlacement.base.origin[0] = *(float *)&FLOAT_0_0;
    source->skinnedPlacement.base.origin[1] = *(float *)&FLOAT_0_0;
    source->skinnedPlacement.base.origin[2] = *(float *)&FLOAT_0_0;
    source->skinnedPlacement.scale = FLOAT_1_0;
    R_GetViewport(source, &viewport);
    R_CmdBufSet2D((GfxViewParms *)&savedregs, source, &viewport);
    if ( g_DXDeviceThread == GetCurrentThreadId() )
      D3DPERF_EndEvent();
  }
}

// local variable allocation has failed, the output may be wrong!
void  R_CmdBufSet2D(GfxViewParms *a1@<ebp>, GfxCmdBufSourceState *source, GfxViewport *viewport)
{
  _BYTE v3[140]; // [esp-8h] [ebp-9Ch] OVERLAPPED BYREF
  float v4; // [esp+84h] [ebp-10h]
  GfxViewParms *viewParms; // [esp+88h] [ebp-Ch]
  float invHeight; // [esp+8Ch] [ebp-8h]
  float retaddr; // [esp+94h] [ebp+0h]

  viewParms = a1;
  invHeight = retaddr;
  if ( viewport->width <= 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state_utils.cpp",
          271,
          0,
          "%s\n\t(viewport->width) = %i",
          "(viewport->width > 0)",
          viewport->width) )
  {
    __debugbreak();
  }
  if ( viewport->height <= 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state_utils.cpp",
          272,
          0,
          "%s\n\t(viewport->height) = %i",
          "(viewport->height > 0)",
          viewport->height) )
  {
    __debugbreak();
  }
  v4 = 1.0 / (float)viewport->width;
  *(float *)&v3[136] = 1.0 / (float)viewport->height;
  *(unsigned int *)&v3[132] = source->viewParms.viewMatrix.m[3];
  memset(&v3[64], 0, 0x40u);
  *(float *)&v3[64] = 2.0 * v4;
  *(float *)&v3[84] = -2.0 * *(float *)&v3[136];
  *(float *)&v3[112] = -1.0 - v4;
  *(float *)&v3[116] = *(float *)&v3[136] + 1.0;
  *(float *)&v3[120] = FLOAT_1_0;
  *(float *)&v3[124] = FLOAT_1_0;
  R_MatrixIdentity44((float (*)[4])v3);
  R_MatrixIdentity44(*(float (**)[4])&v3[132]);
  memcpy((void *)(*(unsigned int *)&v3[132] + 64), &v3[64], 0x40u);
  memcpy((void *)(*(unsigned int *)&v3[132] + 128), &v3[64], 0x40u);
  memset((unsigned __int8 *)(*(unsigned int *)&v3[132] + 192), 0, 0x40u);
  ++LOWORD(source->depthHackFlags);
  ++HIWORD(source->depthHackFlags);
  ++HIWORD(source->skinnedPlacement.base.quat[0]);
  memcpy(R_GetActiveWorldMatrix(source), v3, 0x40u);
}

void __cdecl R_MatrixIdentity44(float (*out)[4])
{
  if ( !out && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state_utils.cpp", 25, 0, "%s", "out") )
    __debugbreak();
  memcpy(out, g_identityMatrix44, 0x40u);
}

void __cdecl R_CmdBufSet3D(GfxCmdBufSourceState *source)
{
  GfxCmdBufSourceState *worldMatrix; // [esp+Ch] [ebp-4h]

  ++LOWORD(source->depthHackFlags);
  ++HIWORD(source->depthHackFlags);
  ++HIWORD(source->skinnedPlacement.base.quat[0]);
  worldMatrix = R_GetActiveWorldMatrix(source);
  R_MatrixIdentity44((float (*)[4])worldMatrix);
  worldMatrix->matrices.matrix[0].m[3][0] = worldMatrix->matrices.matrix[0].m[3][0]
                                          - source->skinnedPlacement.base.origin[0];
  worldMatrix->matrices.matrix[0].m[3][1] = worldMatrix->matrices.matrix[0].m[3][1]
                                          - source->skinnedPlacement.base.origin[1];
  worldMatrix->matrices.matrix[0].m[3][2] = worldMatrix->matrices.matrix[0].m[3][2]
                                          - source->skinnedPlacement.base.origin[2];
}

void __cdecl R_Set3D(GfxCmdBufSourceState *source)
{
  if ( !source->sceneDef.time
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state_utils.cpp",
          358,
          0,
          "%s",
          "source->viewParms3D") )
  {
    __debugbreak();
  }
  if ( source->scissorViewport.width != 1 )
  {
    PIXBeginNamedEvent(-1, "R_Set3D");
    source->scissorViewport.width = 1;
    memcpy(source->viewParms.viewMatrix.m[3], (const void *)source->sceneDef.time, 0x140u);
    if ( source->viewParms.zFar == 0.0 )
    {
      source->skinnedPlacement.base.origin[0] = *(float *)&FLOAT_0_0;
      source->skinnedPlacement.base.origin[1] = *(float *)&FLOAT_0_0;
      source->skinnedPlacement.base.origin[2] = *(float *)&FLOAT_0_0;
    }
    else
    {
      source->skinnedPlacement.base.origin[0] = source->viewParms.axis[2][2];
      source->skinnedPlacement.base.origin[1] = source->viewParms.depthHackNearClip;
      source->skinnedPlacement.base.origin[2] = source->viewParms.zNear;
    }
    source->skinnedPlacement.scale = FLOAT_1_0;
    R_CmdBufSet3D(source);
    UpdateVPosToWorld(source);
    if ( g_DXDeviceThread == GetCurrentThreadId() )
      D3DPERF_EndEvent();
  }
}

void __cdecl R_BeginView(GfxCmdBufSourceState *source, const GfxSceneDef *sceneDef, const GfxViewParms *viewParms)
{
  *(GfxSceneDef *)&source->scissorViewport.height = *sceneDef;
  source->sceneDef.time = (int)viewParms;
  *(float *)&source->sceneViewport.y = sceneDef->viewOffset[0];
  *(float *)&source->sceneViewport.width = sceneDef->viewOffset[1];
  *(float *)&source->sceneViewport.height = sceneDef->viewOffset[2];
  source->scissorViewport.width = 0;
  R_Set3D(source);
  source[1].matrices.matrix[0].m[0][3] = *(float *)&FLOAT_0_0;
  source[1].matrices.matrix[0].m[1][0] = *(float *)&FLOAT_0_0;
  source[1].matrices.matrix[0].m[1][1] = *(float *)&FLOAT_0_0;
  source[1].matrices.matrix[0].m[1][2] = *(float *)&FLOAT_0_0;
  R_SetGameTime(source, source->materialTime);
  R_DeriveNearPlaneConstantsForView(source);
}

void __cdecl R_DeriveNearPlaneConstantsForView(GfxCmdBufSourceState *source)
{
  __int64 v1; // [esp+Ch] [ebp-4Ch]
  __int64 v2; // [esp+1Ch] [ebp-3Ch]
  __int64 v3; // [esp+2Ch] [ebp-2Ch]
  float scale; // [esp+54h] [ebp-4h]
  float scalea; // [esp+54h] [ebp-4h]

  if ( (float)(0.0000099999997 * source->viewParms.axis[2][1]) <= COERCE_FLOAT(
                                                                    LODWORD(source->viewParms.inverseViewProjectionMatrix.m[3][3])
                                                                  & _mask__AbsFloat_)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state_utils.cpp",
          406,
          0,
          "%s\n\t(mtx->m[0][3]) = %g",
          "(I_fabs( mtx->m[0][3] ) < 1.0e-5f * mtx->m[3][3])",
          source->viewParms.inverseViewProjectionMatrix.m[3][3]) )
  {
    __debugbreak();
  }
  if ( (float)(0.0000099999997 * source->viewParms.axis[2][1]) <= COERCE_FLOAT(
                                                                    LODWORD(source->viewParms.origin[3])
                                                                  & _mask__AbsFloat_)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state_utils.cpp",
          407,
          0,
          "%s\n\t(mtx->m[1][3]) = %g",
          "(I_fabs( mtx->m[1][3] ) < 1.0e-5f * mtx->m[3][3])",
          source->viewParms.origin[3]) )
  {
    __debugbreak();
  }
  if ( source->viewParms.axis[2][1] == 0.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state_utils.cpp",
          408,
          0,
          "%s",
          "mtx->m[3][3] != 0") )
  {
    __debugbreak();
  }
  scale = 1.0 / source->viewParms.axis[2][1];
  *(float *)&v3 = (float)(scale * source->viewParms.axis[1][2]) - source->viewParms.depthHackNearClip;
  *((float *)&v3 + 1) = (float)(scale * source->viewParms.axis[2][0]) - source->viewParms.zNear;
  source->input.consts[16][0] = (float)(scale * source->viewParms.axis[1][1]) - source->viewParms.axis[2][2];
  *(_QWORD *)&source->gap0[260] = v3;
  source->input.consts[16][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(source, 0x10u);
  scalea = scale + scale;
  *(float *)&v2 = scalea * source->viewParms.inverseViewProjectionMatrix.m[3][1];
  *((float *)&v2 + 1) = scalea * source->viewParms.inverseViewProjectionMatrix.m[3][2];
  source->input.consts[17][0] = scalea * source->viewParms.inverseViewProjectionMatrix.m[3][0];
  *(_QWORD *)&source->gap0[276] = v2;
  source->input.consts[17][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(source, 0x11u);
  *(float *)&v1 = COERCE_FLOAT(LODWORD(scalea) ^ _mask__NegFloat_) * source->viewParms.origin[1];
  *((float *)&v1 + 1) = COERCE_FLOAT(LODWORD(scalea) ^ _mask__NegFloat_) * source->viewParms.origin[2];
  source->input.consts[18][0] = COERCE_FLOAT(LODWORD(scalea) ^ _mask__NegFloat_) * source->viewParms.origin[0];
  *(_QWORD *)&source->gap0[292] = v1;
  source->input.consts[18][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(source, 0x12u);
}

void __cdecl R_SetShadowLookupMatrix(GfxCmdBufSourceState *source, const GfxMatrix *matrix)
{
  ++HIWORD(source->skinnedPlacement.base.quat[1]);
  memcpy(source->shadowLookupMatrix.m[3], matrix, 0x40u);
}

