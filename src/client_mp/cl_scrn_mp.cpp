#include "cl_scrn_mp.h"

void __cdecl SCR_DrawSmallStringExt(int x, int y, char *string, const float *setColor)
{
  float v4; // xmm0_4

  v4 = (float)R_TextHeight(cls.consoleFont);
  R_AddCmdDrawText(string, 0x7FFFFFFF, cls.consoleFont, (float)x, (float)y + v4, 1.0, 1.0, 0.0, setColor, 0);
}

void __cdecl SCR_Init()
{
  scr_initialized = 1;
}

void __cdecl CL_DrawScreen(int localClientNum)
{
  //PIXBeginNamedEvent(-1, "CL_DrawScreen");
  if ( !cls.rendererStarted )
  {
    if ( g_DXDeviceThread != GetCurrentThreadId() )
      return;
    goto LABEL_13;
  }
  if ( CL_GetLocalClientConnectionState(localClientNum) == 10 )
  {
    CG_DrawFullScreenDebugOverlays();
    CG_DrawUpperRightDebugInfo(localClientNum);
  }
  else
  {
    BLOPS_NULLSUB();
  }
  if ( cg_draw2D && cg_drawVersion && cg_draw2D->current.enabled && cg_drawVersion->current.enabled )
    CG_DrawVersion();
  Con_DrawConsole(localClientNum);
  DevGui_Draw(localClientNum);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
LABEL_13:
    D3DPERF_EndEvent();
}

double __cdecl CL_GetMenuBlurRadius(int localClientNum)
{
  if ( Key_IsCatcherActive(localClientNum, 16) && cls.uiStarted )
    return UI_GetBlurRadius(localClientNum);
  else
    return 0.0;
}

void __cdecl CL_UpdateUIVisibilityBits(int localClientNum)
{
  int v1; // edx
  int v2; // ecx
  int v3; // edx
  int v4; // edx
  int v5; // edx
  int v6; // edx
  int v7; // edx
  int v8; // edx
  int v9; // edx
  int v10; // edx
  int v11; // edx
  int v12; // edx
  int v13; // edx
  int v14; // edx
  int v15; // edx
  const WeaponDef *WeaponDef; // eax
  int v17; // edx
  int v18; // edx
  int v19; // edx
  int v20; // edx
  int v21; // edx
  int v22; // edx
  int v23; // edx
  int v24; // edx
  int v25; // edx
  int v26; // edx
  int v27; // edx
  int v28; // edx
  int v29; // edx
  int v30; // edx
  int v31; // edx
  int UIContextIndex; // eax
  int v33; // edx
  int v34; // edx
  int v35; // edx
  int v36; // edx
  int v37; // edx
  int v38; // edx
  int v39; // edx
  bool v40; // [esp+54h] [ebp-A8h]
  bool v41; // [esp+70h] [ebp-8Ch]
  bool v42; // [esp+8Ch] [ebp-70h]
  int clientUIVisibilityFlags; // [esp+C0h] [ebp-3Ch]
  __int64 v44; // [esp+D0h] [ebp-2Ch]
  bool inKillCam; // [esp+E3h] [ebp-19h]
  cg_s *cgameGlob; // [esp+ECh] [ebp-10h]
  bool uiActive; // [esp+F1h] [ebp-Bh]
  bool spectatingClient; // [esp+F2h] [ebp-Ah]

  //PIXBeginNamedEvent(-1, "CL_UpdateUIVisibilityBits");
  spectatingClient = 0;
  inKillCam = 0;
  uiActive = 0;
  dword_98DADA8[2 * localClientNum] = 0;
  dword_98DADAC[2 * localClientNum] = 0;
  if ( CL_LocalClient_IsCUIFlagSet(localClientNum, 32) )
  {
    if ( !ui_hud_visible || ui_hud_visible->current.enabled )
    {
      cgameGlob = CG_GetLocalClientGlobals(localClientNum);
      if ( CG_GetLocalClientGlobals(localClientNum)->nextSnap )
      {
        v44 = 16 * cgameGlob->matchUIVisibilityFlags;
        v1 = HIDWORD(v44) | dword_98DADAC[2 * localClientNum];
        dword_98DADA8[2 * localClientNum] |= v44;
        dword_98DADAC[2 * localClientNum] = v1;
        clientUIVisibilityFlags = cgameGlob->bgs.clientinfo[cgameGlob->clientNum].clientUIVisibilityFlags;
        v2 = (clientUIVisibilityFlags >> 31) | dword_98DADAC[2 * localClientNum];
        dword_98DADA8[2 * localClientNum] |= clientUIVisibilityFlags;
        dword_98DADAC[2 * localClientNum] = v2;
        v3 = dword_98DADB4[2 * localClientNum] | dword_98DADAC[2 * localClientNum];
        dword_98DADA8[2 * localClientNum] |= dword_98DADB0[2 * localClientNum];
        dword_98DADAC[2 * localClientNum] = v3;
        if ( CG_IsInGuidedMissile(&cgameGlob->predictedPlayerState) )
        {
          v4 = dword_98DADAC[2 * localClientNum];
          dword_98DADA8[2 * localClientNum] |= 0x10000000u;
          dword_98DADAC[2 * localClientNum] = v4;
        }
        if ( CG_IsInGuidedMissileStatic(&cgameGlob->predictedPlayerState) )
        {
          v5 = dword_98DADAC[2 * localClientNum] | 0x40000;
          dword_98DADA8[2 * localClientNum] = dword_98DADA8[2 * localClientNum];
          dword_98DADAC[2 * localClientNum] = v5;
        }
        if ( cgameGlob->inKillCam )
        {
          v6 = dword_98DADAC[2 * localClientNum];
          dword_98DADA8[2 * localClientNum] |= 0x200000u;
          dword_98DADAC[2 * localClientNum] = v6;
          inKillCam = 1;
        }
        if ( CG_IsSelectingLocation(localClientNum) )
        {
          v7 = dword_98DADAC[2 * localClientNum];
          dword_98DADA8[2 * localClientNum] |= 0x400000u;
          dword_98DADAC[2 * localClientNum] = v7;
        }
        if ( CG_Flashbanged(localClientNum) || CG_Flared(localClientNum) )
        {
          v8 = dword_98DADAC[2 * localClientNum];
          dword_98DADA8[2 * localClientNum] |= 0x800000u;
          dword_98DADAC[2 * localClientNum] = v8;
        }
        if ( CL_IsUIActive(localClientNum) )
        {
          v9 = dword_98DADAC[2 * localClientNum];
          dword_98DADA8[2 * localClientNum] |= 0x1000000u;
          dword_98DADAC[2 * localClientNum] = v9;
          uiActive = 1;
        }
        if ( (cgameGlob->predictedPlayerState.otherFlags & 0x1A) != 0 )
        {
          v10 = dword_98DADAC[2 * localClientNum];
          dword_98DADA8[2 * localClientNum] |= 0x2000000u;
          dword_98DADAC[2 * localClientNum] = v10;
          spectatingClient = 1;
        }
        if ( (cgameGlob->predictedPlayerState.otherFlags & 0x18) != 0 )
        {
          v11 = dword_98DADAC[2 * localClientNum];
          dword_98DADA8[2 * localClientNum] |= 0x10000000u;
          dword_98DADAC[2 * localClientNum] = v11;
          spectatingClient = 1;
        }
        if ( (cgameGlob->predictedPlayerState.otherFlags & 0x10) != 0 )
        {
          v12 = dword_98DADAC[2 * localClientNum] | 0x8000;
          dword_98DADA8[2 * localClientNum] = dword_98DADA8[2 * localClientNum];
          dword_98DADAC[2 * localClientNum] = v12;
        }
        if ( (cgameGlob->predictedPlayerState.otherFlags & 2) != 0 )
        {
          v13 = dword_98DADAC[2 * localClientNum] | 0x10000;
          dword_98DADA8[2 * localClientNum] = dword_98DADA8[2 * localClientNum];
          dword_98DADAC[2 * localClientNum] = v13;
        }
        if ( CG_ScopeIsOverlayed(localClientNum)
          && BG_GetWeaponDef(cgameGlob->predictedPlayerState.weapon)->guidedMissileType != MISSILE_GUIDANCE_TVGUIDED )
        {
          v14 = dword_98DADAC[2 * localClientNum];
          dword_98DADA8[2 * localClientNum] |= 0x4000000u;
          dword_98DADAC[2 * localClientNum] = v14;
        }
        if ( (cgameGlob->predictedPlayerState.eFlags & 0x4000) != 0 )
        {
          v15 = dword_98DADAC[2 * localClientNum];
          dword_98DADA8[2 * localClientNum] |= 0x8000000u;
          dword_98DADAC[2 * localClientNum] = v15;
        }
        if ( cgameGlob->weaponSelect < BG_GetNumWeapons()
          && BG_PlayerHasWeapon(&cgameGlob->predictedPlayerState, cgameGlob->weaponSelect) )
        {
          WeaponDef = BG_GetWeaponDef(cgameGlob->weaponSelect);
        }
        else
        {
          WeaponDef = BG_GetWeaponDef(cgameGlob->predictedPlayerState.weapon);
        }
        if ( WeaponDef->fuelTankWeapon )
        {
          v17 = dword_98DADAC[2 * localClientNum];
          dword_98DADA8[2 * localClientNum] |= 0x20000000u;
          dword_98DADAC[2 * localClientNum] = v17;
        }
        if ( (cgameGlob->predictedPlayerState.locationSelectionInfo & 0xF) != 0xF
          && cgameGlob->predictedPlayerState.locationSelectionInfo )
        {
          v18 = dword_98DADAC[2 * localClientNum];
          dword_98DADA8[2 * localClientNum] |= 0x40000000u;
          dword_98DADAC[2 * localClientNum] = v18;
        }
        if ( CG_JavelinADS(localClientNum) )
        {
          v19 = dword_98DADAC[2 * localClientNum] | 2;
          dword_98DADA8[2 * localClientNum] = dword_98DADA8[2 * localClientNum];
          dword_98DADAC[2 * localClientNum] = v19;
        }
        if ( CG_ExtraCamIsActive(localClientNum) )
        {
          v20 = dword_98DADAC[2 * localClientNum] | 8;
          dword_98DADA8[2 * localClientNum] = dword_98DADA8[2 * localClientNum];
          dword_98DADAC[2 * localClientNum] = v20;
        }
        if ( cgameGlob->extraCamStatic
          || cgameGlob->extraCamActive && (cgameGlob->predictedPlayerState.weapFlags & 0x200000) == 0 )
        {
          v21 = dword_98DADAC[2 * localClientNum] | 0x10;
          dword_98DADA8[2 * localClientNum] = dword_98DADA8[2 * localClientNum];
          dword_98DADAC[2 * localClientNum] = v21;
        }
        if ( (CG_ExtraCamIsActive(localClientNum)
           || cgameGlob->extraCamStatic
           || cgameGlob->extraCamActive && (cgameGlob->predictedPlayerState.weapFlags & 0x200000) == 0)
          && (cgameGlob->predictedPlayerState.eFlags & 0x4000) == 0 )
        {
          v22 = dword_98DADAC[2 * localClientNum] | 4;
          dword_98DADA8[2 * localClientNum] = dword_98DADA8[2 * localClientNum];
          dword_98DADAC[2 * localClientNum] = v22;
        }
        if ( CG_IsShowingZombieMap() )
        {
          v23 = dword_98DADAC[2 * localClientNum] | 0x20;
          dword_98DADA8[2 * localClientNum] = dword_98DADA8[2 * localClientNum];
          dword_98DADAC[2 * localClientNum] = v23;
        }
        if ( cgameGlob->bgs.clientinfo[cgameGlob->clientNum].infoValid )
        {
          switch ( cgameGlob->bgs.clientinfo[cgameGlob->clientNum].team )
          {
            case TEAM_FREE:
              v24 = dword_98DADAC[2 * localClientNum] | 0x40;
              dword_98DADA8[2 * localClientNum] = dword_98DADA8[2 * localClientNum];
              dword_98DADAC[2 * localClientNum] = v24;
              break;
            case TEAM_AXIS:
              v25 = dword_98DADAC[2 * localClientNum] | 0x100;
              dword_98DADA8[2 * localClientNum] = dword_98DADA8[2 * localClientNum];
              dword_98DADAC[2 * localClientNum] = v25;
              break;
            case TEAM_ALLIES:
              v26 = dword_98DADAC[2 * localClientNum] | 0x80;
              dword_98DADA8[2 * localClientNum] = dword_98DADA8[2 * localClientNum];
              dword_98DADAC[2 * localClientNum] = v26;
              break;
            case TEAM_SPECTATOR:
              v27 = dword_98DADAC[2 * localClientNum] | 0x200;
              dword_98DADA8[2 * localClientNum] = dword_98DADA8[2 * localClientNum];
              dword_98DADAC[2 * localClientNum] = v27;
              break;
            default:
              break;
          }
        }
        else
        {
          v28 = dword_98DADAC[2 * localClientNum] | 0x40;
          dword_98DADA8[2 * localClientNum] = dword_98DADA8[2 * localClientNum];
          dword_98DADAC[2 * localClientNum] = v28;
        }
        v42 = (dword_98DADA8[2 * localClientNum] & 0x40) != 0 && (dword_98DADAC[2 * localClientNum] & 0x80) != 0;
        v41 = (dword_98DADA8[2 * localClientNum] & 0x80) != 0 && (dword_98DADAC[2 * localClientNum] & 0x100) != 0;
        v40 = (dword_98DADA8[2 * localClientNum] & 4) != 0 && (dword_98DADAC[2 * localClientNum] & 0x40) != 0;
        if ( ((dword_98DADA8[2 * localClientNum] & 0x2000) == 0
           || v42
           || v41
           || v40
           || (dword_98DADA8[2 * localClientNum] & 2) != 0)
          && ((dword_98DADA8[2 * localClientNum] & 0x2000000) == 0 || !cg_thirdPerson->current.integer) )
        {
          v29 = dword_98DADAC[2 * localClientNum] | 0x400;
          dword_98DADA8[2 * localClientNum] = dword_98DADA8[2 * localClientNum];
          dword_98DADAC[2 * localClientNum] = v29;
        }
        if ( Dvar_GetInt("ui_hud_showobjicons") )
        {
          v30 = dword_98DADAC[2 * localClientNum] | 0x800;
          dword_98DADA8[2 * localClientNum] = dword_98DADA8[2 * localClientNum];
          dword_98DADAC[2 * localClientNum] = v30;
        }
        if ( Dvar_GetInt("ui_hud_obituaries") )
        {
          v31 = dword_98DADAC[2 * localClientNum] | 0x4000;
          dword_98DADA8[2 * localClientNum] = dword_98DADA8[2 * localClientNum];
          dword_98DADAC[2 * localClientNum] = v31;
        }
        UIContextIndex = Com_LocalClient_GetUIContextIndex(localClientNum);
        if ( Menu_IsMenuOpenAndVisible(UIContextIndex, "scoreboard") )
        {
          v33 = dword_98DADAC[2 * localClientNum] | 0x1000;
          dword_98DADA8[2 * localClientNum] = dword_98DADA8[2 * localClientNum];
          dword_98DADAC[2 * localClientNum] = v33;
        }
        if ( Live_IsWagerMatch() )
        {
          v34 = dword_98DADAC[2 * localClientNum] | 0x20000;
          dword_98DADA8[2 * localClientNum] = dword_98DADA8[2 * localClientNum];
          dword_98DADAC[2 * localClientNum] = v34;
        }
        if ( Demo_IsPlaying() )
        {
          v35 = dword_98DADAC[2 * localClientNum];
          dword_98DADA8[2 * localClientNum] |= 0x80000000;
          dword_98DADAC[2 * localClientNum] = v35;
        }
        if ( Demo_IsRenderingMovie() )
        {
          v36 = dword_98DADAC[2 * localClientNum] | 1;
          dword_98DADA8[2 * localClientNum] = dword_98DADA8[2 * localClientNum];
          dword_98DADAC[2 * localClientNum] = v36;
        }
        if ( Demo_IsGameHudHidden() )
        {
          v37 = dword_98DADAC[2 * localClientNum];
          dword_98DADA8[2 * localClientNum] |= 0x80000u;
          dword_98DADAC[2 * localClientNum] = v37;
        }
        if ( Demo_IsDemoHudHidden() )
        {
          v38 = dword_98DADAC[2 * localClientNum];
          dword_98DADA8[2 * localClientNum] |= 0x100000u;
          dword_98DADAC[2 * localClientNum] = v38;
        }
        Demo_UpdateVisibilityBitsForCameraMode(localClientNum, -1);
        if ( (!spectatingClient || inKillCam)
          && (dword_98DADAC[2 * localClientNum] & 0x1000) == 0
          && !uiActive
          && (dword_98DADA8[2 * localClientNum] & 0x20000) == 0
          && (dword_98DADA8[2 * localClientNum] & 0x40000) == 0
          && (dword_98DADA8[2 * localClientNum] & 0x80000) == 0 )
        {
          v39 = dword_98DADAC[2 * localClientNum] | 0x2000;
          dword_98DADA8[2 * localClientNum] = dword_98DADA8[2 * localClientNum];
          dword_98DADAC[2 * localClientNum] = v39;
        }
      }
    }
  }
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
}

void  SCR_UpdateScreen(int a1@<esi>)
{
  dvar_s *v1; // ecx
  int semaphore; // [esp+10h] [ebp-Ch]
  int shouldSkipRender; // [esp+14h] [ebp-8h]

  if ( !updateScreenCalled && !SCR_ShouldSkipUpdateScreen() )
  {
    semaphore = R_AcquireDXDeviceOwnership(r_PumpDemonware);
    if ( Sys_QueryD3DDeviceOKEvent() )
    {
      if ( semaphore )
        R_ReleaseDXDeviceOwnership();
      if ( CL_GetLocalClientConnectionState(0) == 8 )
        Sys_LoadingKeepAlive();
      shouldSkipRender = com_errorEntered;
      if ( com_errorEntered )
      {
        v1 = (dvar_s *)long_blocking_call;
        if ( long_blocking_call->current.enabled )
          shouldSkipRender = 0;
      }
      if ( scr_initialized && !shouldSkipRender )
      {
        updateScreenCalled = 1;
        SND_Update(v1);
        SCR_UpdateFrame(a1);
        updateScreenCalled = 0;
      }
    }
    else if ( semaphore )
    {
      R_ReleaseDXDeviceOwnership();
    }
  }
}

void  SCR_UpdateFrame(int a1@<esi>)
{
  bool IsMature; // al
  float timescale; // [esp+0h] [ebp-38h]
  int cgTime; // [esp+Ch] [ebp-2Ch]
  int seed; // [esp+10h] [ebp-28h]
  bool v5; // [esp+14h] [ebp-24h]
  int refreshedUI; // [esp+28h] [ebp-10h]
  unsigned int streamingFrame; // [esp+30h] [ebp-8h]

  streamingFrame = streamFrontendGlob.frame;
  if ( !Sys_IsMainThread()
    && !Sys_IsRenderThread()
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\client_mp\\cl_scrn_mp.cpp",
          1210,
          0,
          "%s",
          "Sys_IsMainThread() || Sys_IsRenderThread()") )
  {
    __debugbreak();
  }
  R_BeginFrame();
  v5 = Demo_IsPaused() || Demo_IsCompleted() || Demo_GetClipPausedState();
  if ( Demo_IsPlaying() && CL_LocalClient_IsCUIFlagSet(0, 32) )
    seed = CG_GetLocalClientGlobals(0)->latestSnapshotNum;
  else
    seed = 0;
  if ( CL_LocalClient_IsCUIFlagSet(0, 32) )
    cgTime = CG_GetLocalClientGlobals(0)->time;
  else
    cgTime = 0;
  timescale = Com_GetTimescaleForSnd();
  IsMature = CG_IsMature();
  SND_BeginFrame(IsMature, v5, timescale, cgTime, seed);
  CG_SndUpdate();
  CL_CompositeRender();
  GlassCl_WaitUpdate();
  refreshedUI = CL_CGameRendering(a1, 0);
  if ( CL_AllLocalClientsDisconnected() && Sys_IsMainThread() && cls.uiStarted )
    UI_ViewerDraw(0);
  CL_UpdateUIVisibilityBits(0);
  SCR_DrawScreenField(0, refreshedUI);
  CL_DrawScreen(0);
  if ( streamingFrame == streamFrontendGlob.frame )
    R_StreamUpdate_Idle();
  else
    R_StreamUpdate_End();
  SND_EndFrame();
  R_EndFrame();
  R_IssueRenderCommands(3u);
  if ( r_reflectionProbeGenerate->current.enabled && refreshedUI && CL_GetLocalClientGlobals(0)->serverTime > 1000 )
    R_BspGenerateReflections();
}

int  CL_CGameRendering@<eax>(int a1@<esi>, int localClientNum)
{
  DemoType DemoType; // eax
  DemoType v4; // eax
  bool v5; // [esp-4h] [ebp-68h]
  bool v6; // [esp-4h] [ebp-68h]
  int closeSlideSpeed; // [esp+0h] [ebp-64h]
  int closeFadingTime; // [esp+4h] [ebp-60h]
  char *Name; // [esp+30h] [ebp-34h]
  int hiRes; // [esp+3Ch] [ebp-28h]
  menuDef_t *toastPopupMenu; // [esp+40h] [ebp-24h]
  uiInfo_s *uiInfo; // [esp+44h] [ebp-20h]
  int contextIndex; // [esp+48h] [ebp-1Ch]
  clientActive_t *LocalClientGlobals; // [esp+4Ch] [ebp-18h]
  cg_s *cgameGlob; // [esp+58h] [ebp-Ch]
  int extraCamActive; // [esp+60h] [ebp-4h]
  int savedregs; // [esp+64h] [ebp+0h] BYREF

  extraCamActive = CG_ExtraCamIsActive(localClientNum);
  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  if ( CL_GetLocalClientConnectionState(localClientNum) != 10 )
    return 0;
  Name = va("CL_CGameRendering(cl=%d)", localClientNum);
  //PIXBeginNamedEvent(-1, Name);
  R_UI3D_PerframeInit();
  R_BeginClientCmdList2D();
  if ( extraCamActive )
  {
    CG_ExtraCam_GetViewOrigin(localClientNum, cgameGlob->refdef.extraCamPos);
    cgameGlob->refdef.extraCamPosValid = 1;
  }
  else
  {
    cgameGlob->refdef.extraCamPosValid = 0;
  }
  LocalClientGlobals = CL_GetLocalClientGlobals(localClientNum);
  v5 = UI_IsFullscreen(localClientNum) == 0;
  DemoType = CL_GetDemoType();
  if ( CG_DrawActiveFrame(a1, localClientNum, LocalClientGlobals->serverTime, DemoType, CUBEMAPSHOT_NONE, 0, v5) )
  {
    BLOPS_NULLSUB();
    if ( (CL_GetLocalClientUIGlobals(localClientNum)->keyCatchers & 0x10) != 0 )
    {
      UI_UpdateTime(localClientNum, cls.realtime);
      UI_Refresh(localClientNum);
    }
    else
    {
      uiInfo = UI_GetInfo(localClientNum);
      contextIndex = uiInfo->uiDC.contextIndex;
      if ( uiInfo->toastPopupOpened )
      {
        toastPopupMenu = Menus_FindByName(&uiInfo->uiDC, "menu_toast_popup");
        if ( (signed int)(Sys_Milliseconds() - uiInfo->toastPopupTimeCounter) >= uiInfo->toastPopupDuration )
        {
          if ( (Window_GetDynamicFlags(contextIndex, &toastPopupMenu->window) & 0x42000) != 0 )
          {
            if ( (Window_GetDynamicFlags(contextIndex, &toastPopupMenu->window) & 0x42000) != 0 )
            {
              Menu_Paint(
                (GenericEventHandler *)&savedregs,
                localClientNum,
                &uiInfo->uiDC,
                0,
                toastPopupMenu,
                (const ScreenPlacement *)0xFFFFFFFF);
              if ( (Window_GetDynamicFlags(contextIndex, &toastPopupMenu->window) & 0x42000) == 0 )
              {
                Window_RemoveDynamicFlags(contextIndex, &toastPopupMenu->window, 4);
                uiInfo->toastPopupTimeCounter = -1;
                uiInfo->toastPopupOpened = 0;
              }
            }
          }
          else
          {
            Menu_Paint(
              (GenericEventHandler *)&savedregs,
              localClientNum,
              &uiInfo->uiDC,
              0,
              toastPopupMenu,
              (const ScreenPlacement *)0xFFFFFFFF);
            if ( toastPopupMenu->closeSlideSpeed <= 0 || toastPopupMenu->closeSlideDirection >= 4u )
            {
              if ( toastPopupMenu->closeFadingTime > 0 )
              {
                Window_AddDynamicFlags(contextIndex, &toastPopupMenu->window, 0x2000);
                toastPopupMenu->fadeTimeCounter = Sys_Milliseconds();
              }
            }
            else
            {
              Window_AddDynamicFlags(contextIndex, &toastPopupMenu->window, 0x40000);
              toastPopupMenu->slideTimeCounter = Sys_Milliseconds();
              if ( toastPopupMenu->closeFadingTime > 0 )
              {
                Window_AddDynamicFlags(contextIndex, &toastPopupMenu->window, 0x2000);
                if ( toastPopupMenu->closeSlideSpeed < toastPopupMenu->closeFadingTime )
                  closeFadingTime = toastPopupMenu->closeFadingTime;
                else
                  closeFadingTime = toastPopupMenu->closeSlideSpeed;
                toastPopupMenu->closeSlideSpeed = closeFadingTime;
                if ( toastPopupMenu->closeSlideSpeed < toastPopupMenu->closeFadingTime )
                  closeSlideSpeed = toastPopupMenu->closeFadingTime;
                else
                  closeSlideSpeed = toastPopupMenu->closeSlideSpeed;
                toastPopupMenu->closeFadingTime = closeSlideSpeed;
                toastPopupMenu->fadeTimeCounter = Sys_Milliseconds();
              }
            }
          }
        }
        else
        {
          Window_AddDynamicFlags(contextIndex, &toastPopupMenu->window, 4);
          Menu_Paint(
            (GenericEventHandler *)&savedregs,
            localClientNum,
            &uiInfo->uiDC,
            0,
            toastPopupMenu,
            (const ScreenPlacement *)0xFFFFFFFF);
        }
      }
    }
    R_UI3D_SetupBackendData(&frontEndDataOut->viewInfo[frontEndDataOut->viewInfoIndex].rbUI3D);
    if ( extraCamActive )
    {
      hiRes = rg.renderHiResShot;
      rg.renderHiResShot = 0;
      v6 = UI_IsFullscreen(localClientNum) == 0;
      v4 = CL_GetDemoType();
      CG_DrawExtraCamFrame(localClientNum, LocalClientGlobals->serverTime, v4, CUBEMAPSHOT_NONE, 0, v6);
      rg.renderHiResShot = hiRes;
    }
    //PIXBeginNamedEvent(-1, "wait Draw2D");
    if ( GetCurrentThreadId() == g_DXDeviceThread )
      D3DPERF_EndEvent();
    R_AddCmdEndOfList();
    if ( GetCurrentThreadId() == g_DXDeviceThread )
      D3DPERF_EndEvent();
    return 1;
  }
  else
  {
    CL_SendCmd(localClientNum);
    if ( GetCurrentThreadId() == g_DXDeviceThread )
      D3DPERF_EndEvent();
    return 0;
  }
}

int __cdecl Window_GetDynamicFlags(int contextIndex, const windowDef_t *w)
{
  if ( contextIndex
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\ui\\../ui/ui_utils.h",
          36,
          0,
          "contextIndex doesn't index MAX_POSSIBLE_LOCAL_CLIENTS\n\t%i not in [0, %i)",
          contextIndex,
          1) )
  {
    __debugbreak();
  }
  return w->dynamicFlags[contextIndex];
}

int __cdecl CL_GetDemoType()
{
  if ( CL_GetLocalClientConnection(0)->demoplaying )
    return 1;
  if ( Demo_IsPlaying() )
    return 3;
  return 0;
}

void __cdecl SCR_DrawScreenField(int localClientNum, int refreshedUI)
{
  connstate_t clcState; // [esp+8h] [ebp-4h]

  R_BeginSharedCmdList();
  R_AddCmdProjectionSet2D();
  R_UI3D_PerframeInit();
  if ( !CG_IsShowingZombieMap() )
  {
    if ( cls.uiStarted && !Dvar_GetBool("ui_changed_exe") )
    {
      UI_UpdateTime(localClientNum, cls.realtime);
      clcState = CL_GetLocalClientConnectionState(localClientNum);
      if ( UI_IsFullscreen(localClientNum) )
      {
        switch ( clcState )
        {
          case CA_DISCONNECTED:
          case CA_CINEMATIC:
          case CA_LOGO:
          case CA_CONNECTING:
          case CA_CHALLENGING:
          case CA_CONNECTED:
          case CA_SENDINGSTATS:
          case CA_LOADING:
          case CA_PRIMED:
            SCR_ClearScreen();
            break;
          case CA_UICINEMATIC:
$LN5_44:
            SCR_ClearScreen();
            SCR_DrawCinematic();
            break;
          case CA_ACTIVE:
            break;
          default:
LABEL_31:
            Com_Error(ERR_FATAL, &byte_CA0440);
            break;
        }
      }
      else
      {
        switch ( clcState )
        {
          case CA_DISCONNECTED:
            SCR_ClearScreen();
            SND_GameReset();
            CG_SndGameReset();
            if ( Sys_IsMainThread() )
            {
              if ( !CL_AllLocalClientsDisconnected()
                && !Assert_MyHandler(
                      "C:\\projects_pc\\cod\\codsrc\\src\\client_mp\\cl_scrn_mp.cpp",
                      718,
                      0,
                      "%s",
                      "CL_AllLocalClientsDisconnected()") )
              {
                __debugbreak();
              }
              if ( cls.wwwDlInProgress )
              {
                UI_Refresh(localClientNum);
                UI_DrawConnectScreen(localClientNum);
              }
            }
            else if ( CL_AnyLocalClientChallenging()
                   && !Assert_MyHandler(
                         "C:\\projects_pc\\cod\\codsrc\\src\\client_mp\\cl_scrn_mp.cpp",
                         715,
                         0,
                         "%s",
                         "!CL_AnyLocalClientChallenging()") )
            {
              __debugbreak();
            }
            break;
          case CA_CINEMATIC:
          case CA_UICINEMATIC:
            goto $LN5_44;
          case CA_LOGO:
            SCR_ClearScreen();
            CL_DrawLogo(localClientNum);
            break;
          case CA_CONNECTING:
          case CA_CHALLENGING:
          case CA_CONNECTED:
          case CA_SENDINGSTATS:
          case CA_LOADING:
          case CA_PRIMED:
            SCR_ClearScreen();
            UI_Refresh(localClientNum);
            UI_DrawConnectScreen(localClientNum);
            refreshedUI = 1;
            break;
          case CA_ACTIVE:
            SCR_DrawDemoRecording();
            break;
          default:
            goto LABEL_31;
        }
      }
      if ( !refreshedUI && Key_IsCatcherActive(localClientNum, 16) )
        UI_Refresh(localClientNum);
      if ( net_showprofile->current.integer )
        Net_DisplayProfile(localClientNum);
      R_UI3D_SetupBackendData(&frontEndDataOut->rbUI3D);
    }
    else
    {
      SCR_DrawLoadingAndLogo();
    }
  }
}

void SCR_DrawDemoRecording()
{
  signed int pos; // [esp+1Ch] [ebp-430h]
  float xScale; // [esp+20h] [ebp-42Ch] BYREF
  char string[1028]; // [esp+24h] [ebp-428h] BYREF
  const clientConnection_t *clc; // [esp+42Ch] [ebp-20h]
  float color[4]; // [esp+430h] [ebp-1Ch] BYREF
  float x; // [esp+440h] [ebp-Ch] BYREF
  float y; // [esp+444h] [ebp-8h] BYREF
  float yScale; // [esp+448h] [ebp-4h] BYREF

  clc = CL_GetLocalClientConnection(0);
  if ( clc->demorecording )
  {
    pos = FS_FTell(clc->demofile);
    sprintf(string, "RECORDING %s: %ik", clc->demoName, pos / 1024);
    CL_LookupColor(0x37u, color);
    x = 5.0f;
    y = 5.0f;
    xScale = R_NormalizedTextScale(cls.consoleFont, 0.33333334);
    yScale = xScale;
    ScrPlace_ApplyRect(scrPlaceView, &x, &y, &xScale, &yScale, 1, 1);
    R_AddCmdDrawText(string, 0x7FFFFFFF, cls.consoleFont, x, y, xScale, yScale, 0.0, color, 0);
  }
}

void SCR_ClearScreen()
{
  R_AddCmdClearScreen(1, colorBlack, 1.0, 0);
}

void SCR_DrawLoadingAndLogo()
{
  SCR_ClearScreen();
}

char __cdecl SCR_ShouldSkipUpdateScreen()
{
  int clcState; // [esp+0h] [ebp-Ch]
  unsigned inttimeNow; // [esp+4h] [ebp-8h]

  clcState = CL_GetLocalClientConnectionState(0);
  if ( clcState >= 10 )
    return 0;
  timeNow = Sys_Milliseconds();
  if ( (int)(timeNow - s_lastUpdateScreenTime) < (clcState < 8 ? 16 : 33) )
    return 1;
  s_lastUpdateScreenTime = timeNow;
  return 0;
}

void __cdecl SCR_UpdateLoadScreen()
{
  if ( !useFastFile->current.enabled )
    SCR_UpdateScreen();
}

void  CL_CubemapShot_f(int a1@<esi>)
{
  const char *v1; // eax
  const char *v2; // eax
  const char *v3; // eax
  const char *v4; // eax
  const char *v5; // eax
  const char *v6; // eax
  const char *v7; // eax
  const char *v8; // eax
  DemoType DemoType; // eax
  char *v10; // eax
  CubemapShot v11; // [esp-4h] [ebp-C0h]
  float v12; // [esp+0h] [ebp-BCh]
  float v13; // [esp+4h] [ebp-B8h]
  char v14; // [esp+Bh] [ebp-B1h]
  char *v15; // [esp+10h] [ebp-ACh]
  const char *v16; // [esp+14h] [ebp-A8h]
  clientActive_t *LocalClientGlobals; // [esp+50h] [ebp-6Ch]
  float rgb[3]; // [esp+54h] [ebp-68h] BYREF
  int shot; // [esp+60h] [ebp-5Ch]
  int localClientNum; // [esp+64h] [ebp-58h]
  int size; // [esp+68h] [ebp-54h]
  char szBaseName[64]; // [esp+6Ch] [ebp-50h] BYREF
  float n0; // [esp+B0h] [ebp-Ch]
  bool isLightingShot; // [esp+B7h] [ebp-5h]
  float n1; // [esp+B8h] [ebp-4h]

  if ( !CL_LocalClient_IsCUIFlagSet(0, 32) )
  {
    Com_Printf(0, "must be in a map to use this command\n");
    return;
  }
  if ( Cmd_Argc() < 3 || strlen(Cmd_Argv(2)) > 0x28 )
  {
    CL_CubemapShotUsage();
    return;
  }
  v16 = Cmd_Argv(2);
  v15 = szBaseName;
  do
  {
    v14 = *v16;
    *v15++ = *v16++;
  }
  while ( v14 );
  v1 = Cmd_Argv(1);
  size = atoi(v1);
  if ( size < 4 || size > 1024 || (size & (size - 1)) != 0 )
    goto LABEL_23;
  memset(rgb, 0, sizeof(rgb));
  isLightingShot = 0;
  n0 = 1.0f;
  n1 = 1.3329999f;
  if ( Cmd_Argc() == 7 )
  {
    v2 = Cmd_Argv(3);
    if ( !I_stricmp(v2, "lighting") )
    {
      v3 = Cmd_Argv(4);
      rgb[0] = atof(v3);
      v4 = Cmd_Argv(5);
      rgb[1] = atof(v4);
      v5 = Cmd_Argv(6);
      rgb[2] = atof(v5);
      isLightingShot = 1;
      goto LABEL_24;
    }
LABEL_23:
    CL_CubemapShotUsage();
    return;
  }
  if ( Cmd_Argc() == 6 )
  {
    v6 = Cmd_Argv(3);
    if ( !I_stricmp(v6, "fresnel") )
    {
      v7 = Cmd_Argv(4);
      n0 = atof(v7);
      v8 = Cmd_Argv(5);
      n1 = atof(v8);
      if ( n0 >= 1.0 && n1 >= 1.0 )
        goto LABEL_24;
    }
    goto LABEL_23;
  }
  if ( Cmd_Argc() != 3 )
    goto LABEL_23;
LABEL_24:
  R_SyncRenderThread();
  localClientNum = 0;
  LocalClientGlobals = CL_GetLocalClientGlobals(0);
  for ( shot = 1; shot < 7; ++shot )
  {
    R_BeginCubemapShot(size, 1);
    R_BeginFrame();
    R_BeginSharedCmdList();
    R_ClearClientCmdList2D();
    DemoType = CL_GetDemoType();
    CG_DrawActiveFrame(a1, localClientNum, LocalClientGlobals->serverTime, DemoType, (CubemapShot)shot, size, 1);
    R_EndFrame();
    R_IssueRenderCommands(3u);
    R_EndCubemapShot((CubemapShot)shot);
  }
  if ( isLightingShot )
    R_LightingFromCubemapShots(rgb);
  for ( shot = 1; shot < 7; ++shot )
  {
    v13 = n1;
    v12 = n0;
    v11 = shot;
    v10 = va("env/%s%s.tga", szBaseName, demo_tags_enum_string_27[shot + 5]);
    R_SaveCubemapShot(v10, v11, v12, v13);
  }
}

void CL_CubemapShotUsage()
{
  Com_Printf(0, "Syntax: cubemapShot size basefilename [lighting r g b | fresnel n0 n1]\n");
  Com_Printf(0, "* size must be a power of 2 that is at least 4 and not more than 1024.\n");
  Com_Printf(0, "* screenshots will be written to 'env/basefilename_*.tga'\n");
  Com_Printf(0, "* basefilename must not exceed %i chars\n", 40);
  Com_Printf(0, "* If 'lighting' is specified, a diffuse environment-based lighting cubemap is generated.\n");
  Com_Printf(0, "  This takes exponentially longer to make larger image sizes.\n");
  Com_Printf(0, "  16 is a good iteration size.  32 is a good final image size.\n");
  Com_Printf(0, "* If 'fresnel' is specified, the alpha channel of the cubemap contains the reflection factor.\n");
  Com_Printf(0, "  n0 and n1 are the index of refraction of the 'air' and 'water' surfaces, respectively.\n");
  Com_Printf(0, "  The index of refraction must always be 1 or greater.\n");
  Com_Printf(0, "  This is always calculated, and defaults to air-water interface (n0 = 1, n1 = 1.333).\n");
}

void  CL_TakeHiResShot(int a1@<esi>)
{
  const char *v1; // eax
  int tiles; // [esp+10h] [ebp-4h]

  tiles = 1;
  if ( Cmd_Argc() >= 2 )
  {
    v1 = Cmd_Argv(1);
    tiles = atoi(v1);
  }
  HiResScreenshot(a1, tiles);
}

void  HiResScreenshot(int a1@<esi>, int tiles)
{
  int k; // [esp+18h] [ebp-8224h]
  float tileY; // [esp+1Ch] [ebp-8220h]
  int j; // [esp+20h] [ebp-821Ch]
  int i; // [esp+24h] [ebp-8218h]
  bool enabled; // [esp+2Fh] [ebp-820Dh]
  int value; // [esp+34h] [ebp-8208h]
  cg_s *LocalClientGlobals; // [esp+3Ch] [ebp-8200h]
  unsigned int width; // [esp+40h] [ebp-81FCh]
  signed int height; // [esp+44h] [ebp-81F8h]
  jpeg_exif exif; // [esp+4Ch] [ebp-81F0h] BYREF
  int v12; // [esp+5Ch] [ebp-81E0h]
  int v13; // [esp+60h] [ebp-81DCh]
  unsigned __int8 out[32768]; // [esp+64h] [ebp-81D8h] BYREF
  jpeg_encode encode; // [esp+8064h] [ebp-1D8h] BYREF
  bool v16; // [esp+823Bh] [ebp-1h]

  LocalClientGlobals = CG_GetLocalClientGlobals(0);
  CL_GetLocalClientGlobals(0);
  if ( !tiles
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\client_mp\\cl_scrn_mp.cpp", 1769, 0, "%s", "tiles != 0") )
  {
    __debugbreak();
  }
  if ( tiles )
  {
    value = r_forceLod->current.integer;
    Dvar_SetInt((dvar_s *)r_forceLod, 4);
    enabled = r_streamFreezeState->current.enabled;
    Dvar_SetBool((dvar_s *)r_streamFreezeState, 1);
    v16 = r_bloomTweaks->current.enabled;
    Dvar_SetBool((dvar_s *)r_bloomTweaks, 1);
    LocalClientGlobals->hiResShotMode = 1;
    width = cls.vidConfig.displayWidth;
    height = cls.vidConfig.displayHeight;
    if ( (!cls.vidConfig.displayHeight || !cls.vidConfig.displayWidth)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\client_mp\\cl_scrn_mp.cpp",
            1786,
            0,
            "%s",
            "height != 0 && width != 0") )
    {
      __debugbreak();
    }
    if ( height && width )
    {
      if ( (int)(tiles * width) >= 0x10000
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\client_mp\\cl_scrn_mp.cpp",
              1800,
              0,
              "%s",
              "width*tiles < (1<<16)") )
      {
        __debugbreak();
      }
      NET_Sleep(0xC8u);
      for ( i = 0; i < 10; ++i )
        SCR_UpdateScreen(a1);
      NET_Sleep(0xC8u);
      R_BeginHiResShot(tiles);
      exif.gpsLat = 34.021542f;
      exif.gpsLon = -118.44875;
      exif.gpsOffsetLat = 0.0;
      exif.gpsOffsetLon = 0.0;
      exif.gpsOffsetLon = LocalClientGlobals->refdef.vieworg[0] * 0.0254;
      exif.gpsOffsetLat = LocalClientGlobals->refdef.vieworg[1] * 0.0254;
      jpeg_encode::begin(&encode, out, (int)(float)((float)(int)(tiles * width) * 1.0), tiles * height, 256, 1, &exif);
      v12 = tiles * tiles * (height / 16);
      v13 = 0;
      for ( j = 0; j < tiles * height; j += 16 )
      {
        tileY = (float)j / (float)height;
        for ( k = 0; k < tiles; ++k )
        {
          jpeg_screenshot_set_frame_info(
            width,
            height,
            0,
            0,
            tiles,
            COERCE_VOID_((float)k),
            tileY,
            0,
            SLODWORD(1.0f),
            COERCE_INT((float)v13++ / (float)v12));
          R_UpdateHiResShot((float)k, tileY);
          SCR_UpdateScreen(a1);
          jpeg_screenshot_chunk(out, &encode);
        }
      }
      jpeg_encode::end(&encode, out);
      R_EndHiResShot();
      LocalClientGlobals->hiResShotMode = 0;
      Dvar_SetInt((dvar_s *)r_forceLod, value);
      Dvar_SetBool((dvar_s *)r_streamFreezeState, enabled);
      Dvar_SetBool((dvar_s *)r_bloomTweaks, v16);
    }
  }
}

void  CL_TakeHiResShot2(int a1@<esi>)
{
  HiResScreenshot(a1, 2);
}

void  CL_TakeHiResShot3(int a1@<esi>)
{
  HiResScreenshot(a1, 3);
}

void  CL_TakeHiResShot4(int a1@<esi>)
{
  HiResScreenshot(a1, 4);
}

void  CL_TakeHiResShot8(int a1@<esi>)
{
  HiResScreenshot(a1, 8);
}

