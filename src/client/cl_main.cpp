#include "cl_main.h"

void __cdecl CL_RunNetworkFrame(int localClientNum)
{
  int i; // [esp+0h] [ebp-Ch]
  int firstSignedInController; // [esp+4h] [ebp-8h]

  Com_LocalClient_GetControllerIndex(localClientNum);
  if ( g_doLiveFrameHack )
  {
    firstSignedInController = -1;
    for ( i = 0; i < 1; ++i )
    {
      BG_EvalVehicleName();
      if ( Live_IsSignedIn(i)
        && !CG_IsShowingZombieMap()
        && Flame_GetLocalClientSourceRange()
        && firstSignedInController == -1 )
      {
        firstSignedInController = i;
      }
    }
    Live_Frame();
    dwUpdateLanSession();
  }
  else
  {
    g_doLiveFrameHack = 1;
  }
  Live_CheckOngoingTasks();
}

char __cdecl CL_AnyLocalClientsRunning()
{
  int localClientNum; // [esp+0h] [ebp-4h]

  for ( localClientNum = 0; localClientNum < 1; ++localClientNum )
  {
    if ( CL_LocalClient_IsCUIFlagSet(localClientNum, 2) )
      return 1;
  }
  return 0;
}

void __cdecl CL_DontUnlockControllers(bool flag)
{
  s_dontUnlockControllers = flag;
}

void __cdecl CL_Command_ProvisionallyDisableAllClients()
{
  int localClientNum; // [esp+0h] [ebp-4h]

  for ( localClientNum = 0; localClientNum < 1; ++localClientNum )
  {
    BG_EvalVehicleName();
    if ( !s_dontUnlockControllers )
      Com_LocalClient_SetBeingUsed(localClientNum, 0);
  }
  CL_DontUnlockControllers(0);
}

void __cdecl CL_Command_DisableAllButPrimaryClients()
{
  int localClientNum; // [esp+0h] [ebp-4h]

  for ( localClientNum = 0; localClientNum < 1; ++localClientNum )
  {
    if ( !Com_LocalClient_IsPrimary(localClientNum) )
    {
      BG_EvalVehicleName();
      Com_LocalClient_SetBeingUsed(localClientNum, 0);
    }
  }
}

void __cdecl CL_Command_DisableAllClients()
{
  int localClientNum; // [esp+0h] [ebp-4h]

  for ( localClientNum = 0; localClientNum < 1; ++localClientNum )
  {
    BG_EvalVehicleName();
    Com_LocalClient_SetBeingUsed(localClientNum, 0);
  }
}

void __cdecl CL_Command_SetClientBeingUsedAndPrimary()
{
  Com_LocalClient_SetBeingUsed(0, 1);
  Com_LocalClient_SetPrimary(0, 1);
}

void __cdecl CL_Command_SetClientBeingUsedAndPrimaryAndActive()
{
  Com_LocalClient_SetBeingUsed(0, 1);
  Com_LocalClient_SetPrimary(0, 1);
  BG_EvalVehicleName();
}

void __cdecl CL_Command_SetClientPrimary()
{
  Com_LocalClient_SetPrimary(0, 1);
}

void __cdecl CL_Command_SetClientBeingUsed()
{
  Com_LocalClient_SetBeingUsed(0, 1);
}

void __cdecl CL_Command_SetClientBeingUsedAndActive()
{
  Com_LocalClient_SetBeingUsed(0, 1);
  BG_EvalVehicleName();
}

void __cdecl CL_Command_SetClientNotBeingUsed()
{
  BG_EvalVehicleName();
  Com_LocalClient_SetBeingUsed(0, 0);
}

void __cdecl CL_SetupClientsForIngame()
{
  Com_LocalClients_CompressClients();
  Com_LocalClients_AssignUIContextsForInGame();
  CL_LocalClients_SetAllUsedActive();
}

void __cdecl CL_SetupClientsForFrontend()
{
  Com_LocalClients_AssignUIContextsForFrontEnd();
}

void __cdecl CL_DrawFramedPicPhysical(
        float x,
        float y,
        float w,
        float h,
        float thicknessW,
        float thicknessH,
        float thicknessTex,
        int sides,
        const float *color,
        Material *material)
{
  R_AddCmdDrawFramed(x, y, w, h, thicknessW, thicknessH, thicknessTex, sides, color, material);
}

void __cdecl CL_RegisterCommands()
{
  CL_Platform_RegisterCommands();
  Cmd_AddCommandInternal("updategamerprofile", CL_UpdateGamerProfile, &CL_UpdateGamer//Profile_VAR);
  Cmd_AddCommandInternal("commitdvartoprofiles", CL_CommitDvarToProfiles, &CL_CommitDvarToProfiles_VAR);
  Cmd_AddCommandInternal("updatedvarsfromprofile", CL_UpdateDvarsFromProfile, &CL_UpdateDvarsFrom//Profile_VAR);
  Cmd_AddCommandInternal("updateprofilefromstats", CL_UpdateProfileFromStats, &CL_UpdateProfileFromStats_VAR);
}

void __cdecl CL_UpdateDvarsFromProfile()
{
  Gamer//Profile_UpdateDvarsFromProfile(0);
}

void __cdecl CL_UpdateGamerProfile()
{
  Gamer//Profile_UpdateProfileFromDvars(0, PROFILE_WRITE_IF_CHANGED);
}

void __cdecl CL_CommitDvarToProfiles()
{
  int LocalClientNum; // eax
  int controllerIndex; // [esp+10h] [ebp-424h]
  char dvarStringCopy[1024]; // [esp+14h] [ebp-420h] BYREF
  const char *dvarName; // [esp+414h] [ebp-20h]
  const dvar_s *dvar; // [esp+418h] [ebp-1Ch]
  DvarValue dvarValue; // [esp+41Ch] [ebp-18h]
  int originalControllerIndex; // [esp+430h] [ebp-4h]

  if ( Cmd_Argc() == 2 )
  {
    originalControllerIndex = 0;
    dvarName = Cmd_Argv(1);
    dvar = Dvar_FindVar(dvarName);
    if ( dvar )
    {
      dvarValue = dvar->current;
      if ( dvar->type == DVAR_TYPE_STRING )
        I_strncpyz(dvarStringCopy, dvar->current.string, 1024);
      Gamer//Profile_UpdateProfileFromDvars(originalControllerIndex, PROFILE_WRITE_IF_CHANGED);
      for ( controllerIndex = 0; controllerIndex < 1; ++controllerIndex )
      {
        LocalClientNum = Com_ControllerIndex_GetLocalClientNum(controllerIndex);
        if ( Com_LocalClient_IsBeingUsed(LocalClientNum) )
        {
          Gamer//Profile_UpdateDvarsFromProfile(controllerIndex);
          Dvar_SetDvarValueFromSource((dvar_s *)dvar, dvarValue, DVAR_SOURCE_INTERNAL);
          if ( dvar->type == DVAR_TYPE_STRING )
            Dvar_SetFromString((dvar_s *)dvar, dvarStringCopy);
          Gamer//Profile_UpdateProfileFromDvars(controllerIndex, PROFILE_WRITE_IF_CHANGED);
        }
      }
      Gamer//Profile_UpdateDvarsFromProfile(originalControllerIndex);
    }
  }
}

void __cdecl CL_UpdateProfileFromStats()
{
  const char *v0; // eax

  if ( Cmd_Argc() == 2 )
  {
    v0 = Cmd_Argv(1);
    atoi(v0);
  }
}

void __cdecl CL_DrawSpinnerPhysical(float x, float y, float w, float h, const float *color)
{
  float v5; // [esp+2Ch] [ebp-30h]
  float v6; // [esp+30h] [ebp-2Ch]
  int wedge; // [esp+38h] [ebp-24h]
  float angle; // [esp+40h] [ebp-1Ch]
  int ms; // [esp+44h] [ebp-18h]
  float fadedColor[4]; // [esp+48h] [ebp-14h] BYREF
  float maxSize; // [esp+58h] [ebp-4h]

  maxSize = FLOAT_64_0;
  if ( color )
  {
    fadedColor[0] = *color;
    fadedColor[1] = color[1];
    fadedColor[2] = color[2];
    fadedColor[3] = color[3];
  }
  else
  {
    fadedColor[0] = colorWhite[0];
    fadedColor[1] = 1.0;
    fadedColor[2] = 1.0;
    fadedColor[3] = 1.0;
  }
  ms = (int)Sys_Milliseconds() % 60000;
  if ( (float)(w - maxSize) < 0.0 )
    v6 = w;
  else
    v6 = maxSize;
  maxSize = v6;
  if ( (float)((float)(h / cls.vidConfig.aspectRatioDisplayPixel) - v6) < 0.0 )
    v5 = h / cls.vidConfig.aspectRatioDisplayPixel;
  else
    v5 = maxSize;
  maxSize = v5;
  if ( w > v5 )
  {
    x = (float)((float)(w - maxSize) * 0.5) + x;
    w = maxSize;
  }
  maxSize = maxSize * cls.vidConfig.aspectRatioDisplayPixel;
  if ( h > maxSize )
  {
    y = (float)((float)(h - maxSize) * 0.5) + y;
    h = maxSize;
  }
  angle = (float)((float)ms / 857.14288) * 360.0;
  for ( wedge = 0; wedge < 8; ++wedge )
  {
    R_AddCmdDrawStretchPicRotateXY(x, y, w, h, 0.0, 0.0, 1.0, 1.0, angle, fadedColor, cls.spinnerMaterial);
    fadedColor[3] = fadedColor[3] * 0.60000002;
    angle = angle - 45.0;
  }
}

void __cdecl CL_DrawSpinner(
        const ScreenPlacement *scrPlace,
        float x,
        float y,
        float w,
        float h,
        int horzAlign,
        int vertAlign,
        const float *color)
{
  ScrPlace_ApplyRect(scrPlace, &x, &y, &w, &h, horzAlign, vertAlign);
  CL_DrawSpinnerPhysical(x, y, w, h, color);
}

void __cdecl CL_DrawSpinnerLoadbarPhysical(float x, float y, float w, float h, const float *color, float percentDone)
{
  float v6; // [esp+2Ch] [ebp-3Ch]
  float v7; // [esp+30h] [ebp-38h]
  float v8; // [esp+34h] [ebp-34h]
  float v9; // [esp+38h] [ebp-30h]
  int wedge; // [esp+40h] [ebp-28h]
  float angle; // [esp+48h] [ebp-20h]
  int ms; // [esp+4Ch] [ebp-1Ch]
  float fadedColor[4]; // [esp+50h] [ebp-18h] BYREF
  float done_angle; // [esp+60h] [ebp-8h]
  float maxSize; // [esp+64h] [ebp-4h]
  float percentDonea; // [esp+84h] [ebp+1Ch]

  maxSize = FLOAT_64_0;
  ms = (int)Sys_Milliseconds() % 60000;
  if ( (float)(w - 64.0) < 0.0 )
    v7 = w;
  else
    v7 = maxSize;
  maxSize = v7;
  if ( (float)(h - v7) < 0.0 )
    v6 = h;
  else
    v6 = maxSize;
  maxSize = v6;
  if ( w > v6 )
  {
    x = (float)((float)(w - maxSize) * 0.5) + x;
    w = maxSize;
  }
  if ( h > maxSize )
  {
    y = (float)((float)(h - maxSize) * 0.5) + y;
    h = maxSize;
  }
  angle = (float)((float)ms / 857.14288) * 360.0;
  done_angle = *(float *)&FLOAT_0_0;
  percentDonea = percentDone * 360.0;
  for ( wedge = 0; wedge < 8; ++wedge )
  {
    if ( (float)(0.0 - (float)((float)(percentDonea - done_angle) / 45.0)) < 0.0 )
      v9 = (float)(percentDonea - done_angle) / 45.0;
    else
      v9 = *(float *)&FLOAT_0_0;
    if ( (float)(v9 - 1.0) < 0.0 )
      v8 = v9;
    else
      v8 = FLOAT_1_0;
    memset(fadedColor, 0, 12);
    fadedColor[3] = v8;
    if ( percentDonea >= 180.0 )
      fadedColor[0] = 1.0 - (float)((float)(percentDonea - 180.0) / 180.0);
    else
      fadedColor[0] = FLOAT_1_0;
    if ( percentDonea < 240.0 )
      fadedColor[1] = percentDonea / 240.0;
    else
      fadedColor[1] = FLOAT_1_0;
    R_AddCmdDrawStretchPicRotateXY(x, y, w, h, 0.0, 0.0, 1.0, 1.0, angle, fadedColor, cls.spinnerMaterial);
    done_angle = done_angle + 45.0;
    angle = angle + 45.0;
  }
}

void __cdecl CL_DrawSpinnerLoadbar(
        const ScreenPlacement *scrPlace,
        float x,
        float y,
        float w,
        float h,
        int horzAlign,
        int vertAlign,
        const float *color,
        float percentDone)
{
  ScrPlace_ApplyRect(scrPlace, &x, &y, &w, &h, horzAlign, vertAlign);
  CL_DrawSpinnerLoadbarPhysical(x, y, w, h, color, percentDone);
}

void __cdecl CL_RegisterDvars()
{
  BG_EvalVehicleName();
}

