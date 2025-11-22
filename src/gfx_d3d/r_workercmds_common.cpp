#include "r_workercmds_common.h"

void __cdecl R_ProcessCmd_UpdateFxSpotLight(FxCmd *cmd)
{
  //PIXBeginNamedEvent(-1, "R_ProcessCmd_UpdateFxSpotLight");
  BLOPS_NULLSUB();
  FX_UpdateSpotLight(cmd);
  Sys_SetUpdateSpotLightEffectEvent();
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

void __cdecl R_ProcessCmd_UpdateFxNonDependent(FxCmd *cmd)
{
  //PIXBeginNamedEvent(-1, "R_ProcessCmd_UpdateFxNonDependent");
  FX_UpdateNonDependent(cmd);
  Sys_SetUpdateNonDependentEffectsEvent();
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

void __cdecl R_DrawEffects(FxCmd *cmd)
{
  FxGenerateVertsCmd genVertsCmd; // [esp+10h] [ebp-58h] BYREF

  //PIXBeginNamedEvent(-1, "R_DrawEffects");
  //PIXBeginNamedEvent(-1, "fx surfs");
  FX_AddNonSpriteDrawSurfs(cmd);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  FX_FillGenerateVertsCmd(cmd->localClientNum, &genVertsCmd);
  genVertsCmd.useReflection = cmd->useReflection;
  genVertsCmd.reflectionHeight = cmd->reflectionHeight;
  genVertsCmd.genVertsCameraType = cmd->updateCameraType;
  Sys_AddWorkerCmdInternal(&fx_drawWorkerCmd, (unsigned __int8 *)&genVertsCmd, 0);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

void __cdecl R_ProcessCmd_UpdateFxRemaining(FxCmd *cmd)
{
  //PIXBeginNamedEvent(-1, "R_ProcessCmd_UpdateFxRemaining");
  if ( !Sys_QueryD3DDeviceOKEvent() )
  {
    if ( g_DXDeviceThread != GetCurrentThreadId() )
      return;
    goto LABEL_11;
  }
  CG_AddSceneTracerBeams(cmd->localClientNum);
  CG_GenerateSceneVerts(cmd->localClientNum);
  //PIXBeginNamedEvent(-1, "wait for fx_update");
  if ( sys_smp_allowed->current.enabled )
    Sys_WaitUpdateNonDependentEffectsCompleted();
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  //PIXBeginNamedEvent(-1, "wait for fx_add_mark");
  Sys_AssistAndWaitWorkerCmdInternal(&fx_add_markWorkerCmd);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  FX_EndUpdate(cmd->localClientNum);
  R_DrawEffects(cmd);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
LABEL_11:
    D3DPERF_EndEvent();
}

void __cdecl R_UpdateSpotLightEffect(FxCmd *cmd)
{
  //PIXBeginNamedEvent(-1, "R_UpdateSpotLightEffect");
  Sys_ResetUpdateSpotLightEffectEvent();
  Sys_AddWorkerCmdInternal(&fx_update_spotWorkerCmd, (unsigned __int8 *)cmd, 0);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

void __cdecl R_UpdateNonDependentEffects(FxCmd *cmd)
{
  //PIXBeginNamedEvent(-1, "R_UpdateNonDependentEffects");
  Sys_ResetUpdateNonDependentEffectsEvent();
  Sys_AddWorkerCmdInternal(&fx_updateWorkerCmd, (unsigned __int8 *)cmd, 0);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

void __cdecl R_UpdateRemainingEffects(FxCmd *cmd)
{
  //PIXBeginNamedEvent(-1, "R_UpdateRemainingEffects");
  Sys_AddWorkerCmdInternal(&fx_update_remainingWorkerCmd, (unsigned __int8 *)cmd, 0);
  Sys_AddWorkerCmdInternal(&fx_update_remaining_ppuWorkerCmd, (unsigned __int8 *)cmd, 0);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
}

void __cdecl R_UpdateXModelBoundsDelayed(GfxSceneEntity *sceneEnt)
{
  Sys_AddWorkerCmdInternal(&dobj_skelWorkerCmd, (unsigned __int8 *)&sceneEnt, 0);
}

void __cdecl R_SkinGfxEntityDelayed(GfxSceneEntity *sceneEnt)
{
  Sys_AddWorkerCmdInternal(&dobj_skinWorkerCmd, (unsigned __int8 *)&sceneEnt, 0);
}

