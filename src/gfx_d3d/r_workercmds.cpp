#include "r_workercmds.h"

int __cdecl r_dpvs_entityCallback(jqBatch *batch)
{
  unsigned __int16 *data; // [esp+10h] [ebp-4h]

  //PIXBeginNamedEvent(-1, "r_dpvs_entity");
  data = (unsigned __int16 *)jqLockData(batch);
  R_AddEntitySurfacesInFrustumCmd(data);
  jqUnlockData(batch);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  return 0;
}

int __cdecl r_model_skinCallback(jqBatch *batch)
{
  unsigned int *data; // [esp+10h] [ebp-4h]
  int savedregs; // [esp+14h] [ebp+0h] BYREF

  data = jqLockData(batch);
  if ( R_DynamicFenceBusy() )
    return 1;
  *frontEndDataOut->dynamicBufferCurrentFrame = frontEndDataOut->frameCount;
  //PIXBeginNamedEvent(-1, "r_model_skin");
  R_SkinXModelCmd((int)&savedregs, (SkinXModelCmd *)data);
  jqUnlockData(batch);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  return 0;
}

bool __cdecl R_DynamicFenceBusy()
{
  return R_DynamicFencePending();
}

int __cdecl r_dpvs_staticCallback(jqBatch *batch)
{
  unsigned int *data; // [esp+10h] [ebp-4h]

  //PIXBeginNamedEvent(-1, "r_dpvs_static");
  data = jqLockData(batch);
  R_AddCellStaticSurfacesInFrustumCmd((DpvsStaticCellCmd *)data);
  jqUnlockData(batch);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  return 0;
}

int __cdecl r_dpvs_dynmodelCallback(jqBatch *batch)
{
  unsigned int *data; // [esp+10h] [ebp-4h]

  //PIXBeginNamedEvent(-1, "r_dpvs_dynmodel");
  data = jqLockData(batch);
  R_AddCellDynModelSurfacesInFrustumCmd((const DpvsPlane **)data);
  jqUnlockData(batch);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  return 0;
}

int __cdecl r_dpvs_sceneentCallback(jqBatch *batch)
{
  GfxWorldDpvsPlanes *data; // [esp+10h] [ebp-4h]
  int savedregs; // [esp+14h] [ebp+0h] BYREF

  //PIXBeginNamedEvent(-1, "r_dpvs_sceneent");
  data = (GfxWorldDpvsPlanes *)jqLockData(batch);
  R_AddCellSceneEntSurfacesInFrustumCmd((unsigned int)&savedregs, data);
  jqUnlockData(batch);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  return 0;
}

int __cdecl r_dpvs_dynbrushCallback(jqBatch *batch)
{
  unsigned int *data; // [esp+10h] [ebp-4h]

  //PIXBeginNamedEvent(-1, "r_dpvs_dynbrush");
  data = jqLockData(batch);
  R_AddCellDynBrushSurfacesInFrustumCmd((const DpvsPlane **)data);
  jqUnlockData(batch);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  return 0;
}

int __cdecl r_waterCallback(jqBatch *batch)
{
  unsigned int *data; // [esp+10h] [ebp-4h]

  data = jqLockData(batch);
  if ( R_UploadWaterPending() )
    return 1;
  //PIXBeginNamedEvent(-1, "r_water");
  R_UploadWaterTextureInternal((water_t **)data);
  jqUnlockData(batch);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  return 0;
}

int __cdecl R_UploadWaterPending()
{
  if ( !_InterlockedExchangeAdd(&g_waterLock, 1u) )
    return 0;
  R_ExecWaterFailed();
  return 1;
}

signed __int32 R_ExecWaterFailed()
{
  return _InterlockedExchangeAdd(&g_waterLock, 0xFFFFFFFF);
}

int __cdecl r_water_simCallback(jqBatch *batch)
{
  const WaterSimulationCmd *data; // [esp+10h] [ebp-4h]

  data = (const WaterSimulationCmd *)jqLockData(batch);
  if ( R_EndFenceBusy() )
    return 1;
  //PIXBeginNamedEvent(-1, "r_water_sim");
  R_WaterSimulationCmd(data);
  jqUnlockData(batch);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  return 0;
}

bool __cdecl R_EndFenceBusy()
{
  return R_EndFencePending();
}

int __cdecl r_spot_shadow_entCallback(jqBatch *batch)
{
  unsigned int *data; // [esp+10h] [ebp-4h]

  //PIXBeginNamedEvent(-1, "r_spot_shadow_ent");
  data = jqLockData(batch);
  R_AddSpotShadowEntCmd((const GfxSpotShadowEntCmd *)data);
  jqUnlockData(batch);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  return 0;
}

int __cdecl fx_updateCallback(jqBatch *batch)
{
  unsigned int *data; // [esp+10h] [ebp-4h]

  //PIXBeginNamedEvent(-1, "fx_update");
  data = jqLockData(batch);
  R_ProcessCmd_UpdateFxNonDependent((FxCmd *)data);
  jqUnlockData(batch);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  return 0;
}

int __cdecl fx_update_remainingCallback(jqBatch *batch)
{
  char *Name; // [esp+8h] [ebp-10h]
  FxCmd *data; // [esp+14h] [ebp-4h]

  data = (FxCmd *)jqLockData(batch);
  if ( R_FXUpdateRemainingWait() )
    return 1;
  Name = va("fx_update_remaining cl=%d", data->localClientNum);
  //PIXBeginNamedEvent(-1, Name);
  FX_UpdateRemaining(data);
  jqUnlockData(batch);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  return 0;
}

bool __cdecl R_FXUpdateRemainingWait()
{
  return R_FXSpotLightPending();
}

bool __cdecl R_FXSpotLightPending()
{
  return jqPoll(&fx_update_spotModule.Group);
}

int __cdecl fx_update_remaining_ppuCallback(jqBatch *batch)
{
  char *Name; // [esp+8h] [ebp-10h]
  FxCmd *data; // [esp+14h] [ebp-4h]

  data = (FxCmd *)jqLockData(batch);
  if ( R_FXUpdateRemainingPPUWait() )
    return 1;
  Name = va("fx_update_remaining_ppu cl=%d", data->localClientNum);
  //PIXBeginNamedEvent(-1, Name);
  R_ProcessCmd_UpdateFxRemaining(data);
  jqUnlockData(batch);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  return 0;
}

bool __cdecl R_FXUpdateRemainingPPUWait()
{
  return R_FXDependentPending();
}

bool __cdecl R_FXDependentPending()
{
  return jqPoll(&fx_update_remainingModule.Group);
}

int __cdecl fx_drawCallback(jqBatch *batch)
{
  unsigned int *data; // [esp+10h] [ebp-8h]

  data = jqLockData(batch);
  if ( *frontEndDataOut->dynamicBufferCurrentFrame
    && *frontEndDataOut->dynamicBufferCurrentFrame < frontEndDataOut->frameCount )
  {
    return 1;
  }
  *frontEndDataOut->dynamicBufferCurrentFrame = frontEndDataOut->frameCount;
  //PIXBeginNamedEvent(-1, "fx_draw");
  if ( Sys_QueryD3DDeviceOKEvent() )
    FX_GenerateVerts((FxGenerateVertsCmd *)data);
  jqUnlockData(batch);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  return 0;
}

int __cdecl dobj_skelCallback(jqBatch *batch)
{
  unsigned int *data; // [esp+10h] [ebp-4h]

  //PIXBeginNamedEvent(-1, "dobj_skel");
  data = jqLockData(batch);
  R_UpdateGfxEntityBoundsCmd((GfxSceneEntity **)data);
  jqUnlockData(batch);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  return 0;
}

int __cdecl dobj_skinCallback(jqBatch *batch)
{
  unsigned int *data; // [esp+10h] [ebp-4h]

  //PIXBeginNamedEvent(-1, "dobj_skin");
  data = jqLockData(batch);
  R_SkinGfxEntityCmd((GfxSceneEntity **)data);
  jqUnlockData(batch);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  return 0;
}

int __cdecl r_model_lightingCallback(jqBatch *batch)
{
  CalcLightingCmd *data; // [esp+8h] [ebp-4h]

  data = (CalcLightingCmd *)jqLockData(batch);
  //PIXBeginNamedEvent(-1, "r_model_lighting");
  R_CalcModelLightingCmd(data);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  jqUnlockData(batch);
  return 0;
}

int __cdecl r_add_sceneentCallback(jqBatch *batch)
{
  unsigned int *data; // [esp+8h] [ebp-4h]

  data = jqLockData(batch);
  //PIXBeginNamedEvent(-1, "r_add_sceneent");
  R_AddAllSceneEntSurfacesCamera((const GfxViewInfo *)*data);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  jqUnlockData(batch);
  return 0;
}

int __cdecl fx_marks_drawCallback(jqBatch *batch)
{
  unsigned int *data; // [esp+10h] [ebp-4h]

  data = jqLockData(batch);
  if ( *frontEndDataOut->dynamicBufferCurrentFrame
    && *frontEndDataOut->dynamicBufferCurrentFrame < frontEndDataOut->frameCount )
  {
    return 1;
  }
  *frontEndDataOut->dynamicBufferCurrentFrame = frontEndDataOut->frameCount;
  if ( Sys_QueryD3DDeviceOKEvent() )
  {
    //PIXBeginNamedEvent(-1, "fx_marks_draw");
    FX_GenerateMarkVertsForWorld(data[1], (const GfxLight *)data[9], data[10]);
    if ( GetCurrentThreadId() == g_DXDeviceThread )
      D3DPERF_EndEvent();
  }
  jqUnlockData(batch);
  return 0;
}

int __cdecl fx_update_spotCallback(jqBatch *batch)
{
  unsigned int *data; // [esp+8h] [ebp-4h]

  data = jqLockData(batch);
  //PIXBeginNamedEvent(-1, "fx_update_spot");
  R_ProcessCmd_UpdateFxSpotLight((FxCmd *)data);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  jqUnlockData(batch);
  return 0;
}

int __cdecl r_skin_cached_staticmodelCallback(jqBatch *batch)
{
  unsigned int *data; // [esp+0h] [ebp-4h]

  data = jqLockData(batch);
  if ( *frontEndDataOut->dynamicBufferCurrentFrame
    && *frontEndDataOut->dynamicBufferCurrentFrame < frontEndDataOut->frameCount )
  {
    return 1;
  }
  *frontEndDataOut->dynamicBufferCurrentFrame = frontEndDataOut->frameCount;
  R_SkinCachedStaticModelCmd((SkinCachedStaticModelCmd *)data);
  jqUnlockData(batch);
  return 0;
}

