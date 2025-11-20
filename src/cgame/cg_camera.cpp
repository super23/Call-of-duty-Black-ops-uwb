#include "cg_camera.h"

void __cdecl InterpolateAnglesSmooth(float *curAngles, float *initialAngles, float *targetAngles, float t)
{
  long double v4; // [esp-18h] [ebp-4Ch]
  float v5; // [esp-Ch] [ebp-40h]
  float v6; // [esp-8h] [ebp-3Ch]
  float k; // [esp-4h] [ebp-38h]
  float time; // [esp+0h] [ebp-34h]
  float j; // [esp+4h] [ebp-30h]
  float newDeltaYaw; // [esp+8h] [ebp-2Ch]
  float i; // [esp+Ch] [ebp-28h]
  float newStartPitch; // [esp+18h] [ebp-1Ch]
  float deltaAngles_4; // [esp+20h] [ebp-14h]
  float deltaAngles_8; // [esp+24h] [ebp-10h]

  deltaAngles_4 = initialAngles[1];
  deltaAngles_8 = initialAngles[2];
  for ( i = *initialAngles; COERCE_FLOAT(LODWORD(i) & _mask__AbsFloat_) > 90.0; i = i - (float)(180.0 * newDeltaYaw) )
  {
    if ( i <= 0.0 )
      newDeltaYaw = FLOAT_N1_0;
    else
      newDeltaYaw = FLOAT_1_0;
  }
  for ( j = *targetAngles; COERCE_FLOAT(LODWORD(j) & _mask__AbsFloat_) > 90.0; j = j - (float)(180.0 * time) )
  {
    if ( j <= 0.0 )
      time = FLOAT_N1_0;
    else
      time = FLOAT_1_0;
  }
  newStartPitch = targetAngles[2] - deltaAngles_8;
  for ( k = targetAngles[1] - deltaAngles_4;
        COERCE_FLOAT(LODWORD(k) & _mask__AbsFloat_) > 180.0;
        k = k - (float)(360.0 * v6) )
  {
    if ( (float)(targetAngles[1] - deltaAngles_4) <= 0.0 )
      v6 = FLOAT_N1_0;
    else
      v6 = FLOAT_1_0;
  }
  __libm_sse2_cos(v4);
  v5 = (float)((float)((float)(3.1415927 * t) - 3.1415927) + 1.0) * 0.5;
  *curAngles = (float)(v5 * (float)(j - i)) + i;
  curAngles[1] = (float)(v5 * k) + deltaAngles_4;
  curAngles[2] = (float)(v5 * newStartPitch) + deltaAngles_8;
}

void __cdecl InterpolatePositionSmooth(float *curPos, const float *initialPos, const float *targetPos, float t)
{
  long double deltaPos; // [esp+0h] [ebp-10h]
  float deltaPos_4; // [esp+4h] [ebp-Ch]
  float deltaPos_8; // [esp+8h] [ebp-8h]
  float time; // [esp+Ch] [ebp-4h]

  __libm_sse2_cos(deltaPos);
  time = (float)((float)((float)(3.1415927 * t) - 3.1415927) + 1.0) * 0.5;
  deltaPos_4 = time * (float)(targetPos[1] - initialPos[1]);
  deltaPos_8 = time * (float)(targetPos[2] - initialPos[2]);
  *curPos = *initialPos + (float)(time * (float)(*targetPos - *initialPos));
  curPos[1] = initialPos[1] + deltaPos_4;
  curPos[2] = initialPos[2] + deltaPos_8;
}

void __cdecl CG_StartCameraTween(int localClientNum, float tweenTime)
{
  float lastFOV; // [esp+0h] [ebp-38h]
  cg_s *cgameGlob; // [esp+30h] [ebp-8h]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  if ( (float)((float)(cgameGlob->cameraData.tweenDuration
                     - (float)(cgameGlob->time - cgameGlob->cameraData.tweenStartTime))
             * 0.001) <= tweenTime )
  {
    if ( ((LODWORD(cgameGlob->cameraData.lastViewOrg[0]) & 0x7F800000) == 0x7F800000
       || (LODWORD(cgameGlob->cameraData.lastViewOrg[1]) & 0x7F800000) == 0x7F800000
       || (LODWORD(cgameGlob->cameraData.lastViewOrg[2]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_camera.cpp",
            104,
            0,
            "%s",
            "!IS_NAN((cgameGlob->cameraData.lastViewOrg)[0]) && !IS_NAN((cgameGlob->cameraData.lastViewOrg)[1]) && !IS_NA"
            "N((cgameGlob->cameraData.lastViewOrg)[2])") )
    {
      __debugbreak();
    }
    if ( ((LODWORD(cgameGlob->cameraData.lastViewAngles[0]) & 0x7F800000) == 0x7F800000
       || (LODWORD(cgameGlob->cameraData.lastViewAngles[1]) & 0x7F800000) == 0x7F800000
       || (LODWORD(cgameGlob->cameraData.lastViewAngles[2]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_camera.cpp",
            105,
            0,
            "%s",
            "!IS_NAN((cgameGlob->cameraData.lastViewAngles)[0]) && !IS_NAN((cgameGlob->cameraData.lastViewAngles)[1]) && "
            "!IS_NAN((cgameGlob->cameraData.lastViewAngles)[2])") )
    {
      __debugbreak();
    }
    cgameGlob->cameraData.tweenDuration = tweenTime;
    cgameGlob->cameraData.tweenStartTime = cgameGlob->time;
    if ( (float)(cgameGlob->cameraData.lastFOV - 4.0) < 0.0 )
      lastFOV = FLOAT_4_0;
    else
      lastFOV = cgameGlob->cameraData.lastFOV;
    cgameGlob->cameraData.tweenStartFOV = lastFOV;
    cgameGlob->cameraData.tweenStartOrg[0] = cgameGlob->cameraData.lastViewOrg[0];
    cgameGlob->cameraData.tweenStartOrg[1] = cgameGlob->cameraData.lastViewOrg[1];
    cgameGlob->cameraData.tweenStartOrg[2] = cgameGlob->cameraData.lastViewOrg[2];
    cgameGlob->cameraData.tweenStartAngles[0] = cgameGlob->cameraData.lastViewAngles[0];
    cgameGlob->cameraData.tweenStartAngles[1] = cgameGlob->cameraData.lastViewAngles[1];
    cgameGlob->cameraData.tweenStartAngles[2] = cgameGlob->cameraData.lastViewAngles[2];
  }
}

double __cdecl CG_UpdateCameraTweenFOV(int localClientNum, float currentFov)
{
  float tweenTime; // [esp+4h] [ebp-8h]
  cg_s *cgameGlob; // [esp+8h] [ebp-4h]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  if ( cgameGlob->cameraData.tweenStartTime > cgameGlob->time )
    cgameGlob->cameraData.tweenStartTime = 0;
  tweenTime = (float)(cgameGlob->time - cgameGlob->cameraData.tweenStartTime) * 0.001;
  if ( cgameGlob->cameraData.tweenDuration > tweenTime )
    currentFov = (float)((float)(currentFov - cgameGlob->cameraData.tweenStartFOV)
                       * (float)(tweenTime / cgameGlob->cameraData.tweenDuration))
               + cgameGlob->cameraData.tweenStartFOV;
  cgameGlob->cameraData.lastFOV = currentFov;
  return currentFov;
}

void __cdecl CG_UpdateCameraTween(int localClientNum)
{
  float *lastViewAngles; // [esp+4h] [ebp-78h]
  float *refdefViewAngles; // [esp+8h] [ebp-74h]
  Camera *p_cameraData; // [esp+Ch] [ebp-70h]
  float *vieworg; // [esp+10h] [ebp-6Ch]
  float *v5; // [esp+34h] [ebp-48h]
  float *v6; // [esp+38h] [ebp-44h]
  float deltaAngles[3]; // [esp+44h] [ebp-38h] BYREF
  float t; // [esp+50h] [ebp-2Ch]
  centity_s *vehicle; // [esp+54h] [ebp-28h]
  float tweenTime; // [esp+58h] [ebp-24h]
  cg_s *cgameGlob; // [esp+5Ch] [ebp-20h]
  float tweenStartPos[3]; // [esp+60h] [ebp-1Ch] BYREF
  float tweenStartAngles[3]; // [esp+6Ch] [ebp-10h] BYREF
  playerState_s *ps; // [esp+78h] [ebp-4h]
  int savedregs; // [esp+7Ch] [ebp+0h] BYREF

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  tweenTime = (float)(cgameGlob->time - cgameGlob->cameraData.tweenStartTime) * 0.001;
  if ( cgameGlob->cameraData.tweenDuration > tweenTime )
  {
    ps = &cgameGlob->predictedPlayerState;
    vehicle = 0;
    if ( cgameGlob->predictedPlayerState.viewlocked_entNum != 1023 )
      vehicle = CG_GetEntity(localClientNum, ps->viewlocked_entNum);
    if ( vehicle )
    {
      CG_AdjustPositionForMover(
        (int)&savedregs,
        localClientNum,
        cgameGlob->cameraData.tweenStartOrg,
        vehicle->nextState.number,
        cgameGlob->cameraData.lastTime,
        cgameGlob->time,
        cgameGlob->cameraData.tweenStartOrg,
        deltaAngles);
      v5 = cgameGlob->cameraData.tweenStartAngles;
      v6 = cgameGlob->cameraData.tweenStartAngles;
      cgameGlob->cameraData.tweenStartAngles[0] = cgameGlob->cameraData.tweenStartAngles[0] + deltaAngles[0];
      v5[1] = v6[1] + deltaAngles[1];
      v5[2] = v6[2] + deltaAngles[2];
    }
    t = tweenTime / cgameGlob->cameraData.tweenDuration;
    tweenStartPos[0] = cgameGlob->cameraData.tweenStartOrg[0];
    tweenStartPos[1] = cgameGlob->cameraData.tweenStartOrg[1];
    tweenStartPos[2] = cgameGlob->cameraData.tweenStartOrg[2];
    tweenStartAngles[0] = cgameGlob->cameraData.tweenStartAngles[0];
    tweenStartAngles[1] = cgameGlob->cameraData.tweenStartAngles[1];
    tweenStartAngles[2] = cgameGlob->cameraData.tweenStartAngles[2];
    if ( ((LODWORD(tweenStartPos[0]) & 0x7F800000) == 0x7F800000
       || (LODWORD(tweenStartPos[1]) & 0x7F800000) == 0x7F800000
       || (LODWORD(tweenStartPos[2]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_camera.cpp",
            194,
            0,
            "%s",
            "!IS_NAN((tweenStartPos)[0]) && !IS_NAN((tweenStartPos)[1]) && !IS_NAN((tweenStartPos)[2])") )
    {
      __debugbreak();
    }
    if ( ((LODWORD(tweenStartAngles[0]) & 0x7F800000) == 0x7F800000
       || (LODWORD(tweenStartAngles[1]) & 0x7F800000) == 0x7F800000
       || (LODWORD(tweenStartAngles[2]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_camera.cpp",
            195,
            0,
            "%s",
            "!IS_NAN((tweenStartAngles)[0]) && !IS_NAN((tweenStartAngles)[1]) && !IS_NAN((tweenStartAngles)[2])") )
    {
      __debugbreak();
    }
    InterpolatePositionSmooth(cgameGlob->refdef.vieworg, tweenStartPos, cgameGlob->refdef.vieworg, t);
    InterpolateAnglesSmooth(cgameGlob->refdefViewAngles, tweenStartAngles, cgameGlob->refdefViewAngles, t);
  }
  p_cameraData = &cgameGlob->cameraData;
  vieworg = cgameGlob->refdef.vieworg;
  cgameGlob->cameraData.lastViewOrg[0] = cgameGlob->refdef.vieworg[0];
  p_cameraData->lastViewOrg[1] = vieworg[1];
  p_cameraData->lastViewOrg[2] = vieworg[2];
  lastViewAngles = cgameGlob->cameraData.lastViewAngles;
  refdefViewAngles = cgameGlob->refdefViewAngles;
  cgameGlob->cameraData.lastViewAngles[0] = cgameGlob->refdefViewAngles[0];
  lastViewAngles[1] = refdefViewAngles[1];
  lastViewAngles[2] = refdefViewAngles[2];
  cgameGlob->cameraData.lastTime = cgameGlob->time;
}

void __cdecl CG_UpdateVehicleInitView(int localClientNum, CameraMode newMode)
{
  cg_s *cgameGlob; // [esp+4h] [ebp-4h]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  if ( newMode >= CAM_VEHICLE && newMode <= CAM_VEHICLE_GUNNER )
    cgameGlob->vehicleInitView = 1;
}

void __cdecl CG_UpdateCameraTransition(int localClientNum, CameraMode oldMode, CameraMode newMode, bool useTagCamera)
{
  float *v4; // [esp+28h] [ebp-58h]
  float *v5; // [esp+2Ch] [ebp-54h]
  float *v6; // [esp+40h] [ebp-40h]
  float *lastViewAngles; // [esp+44h] [ebp-3Ch]
  float *missileViewAngles; // [esp+48h] [ebp-38h]
  float *viewangles; // [esp+4Ch] [ebp-34h]
  float v10; // [esp+50h] [ebp-30h] BYREF
  float v11; // [esp+54h] [ebp-2Ch]
  float v12; // [esp+58h] [ebp-28h]
  float angles; // [esp+5Ch] [ebp-24h] BYREF
  float v14; // [esp+60h] [ebp-20h]
  float v15; // [esp+64h] [ebp-1Ch]
  float temp[3]; // [esp+68h] [ebp-18h] BYREF
  float tweenTime; // [esp+74h] [ebp-Ch]
  cg_s *cgameGlob; // [esp+78h] [ebp-8h]
  playerState_s *ps; // [esp+7Ch] [ebp-4h]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  ps = &cgameGlob->predictedPlayerState;
  tweenTime = *(float *)&FLOAT_0_0;
  switch ( newMode )
  {
    case CAM_NORMAL:
      if ( (oldMode == CAM_VEHICLE || oldMode == CAM_VEHICLE_THIRDPERSON || oldMode == CAM_VEHICLE_GUNNER)
        && CG_IsVehicleRemoteControl(SLOWORD(cgameGlob->cameraData.lastVehicleInfoIndex)) )
      {
        tweenTime = *(float *)&FLOAT_0_0;
      }
      else if ( oldMode == CAM_TURRET )
      {
        tweenTime = FLOAT_0_40000001;
      }
      else if ( oldMode == CAM_VEHICLE || oldMode == CAM_VEHICLE_THIRDPERSON || oldMode == CAM_VEHICLE_GUNNER )
      {
        tweenTime = cg_cameraVehicleExitTweenTime->current.value;
        cgameGlob->cameraData.useTagCamera = useTagCamera;
      }
      break;
    case CAM_LINKED:
      if ( (ps->eFlags & 0x4000) != 0 )
      {
        if ( cgameGlob->cameraData.lastVehicleSeatPos != ps->vehiclePos )
          tweenTime = FLOAT_0_30000001;
      }
      else if ( oldMode == CAM_VEHICLE || oldMode == CAM_VEHICLE_THIRDPERSON || oldMode == CAM_VEHICLE_GUNNER )
      {
        tweenTime = cg_cameraVehicleExitTweenTime->current.value;
        cgameGlob->cameraData.useTagCamera = useTagCamera;
      }
      break;
    case CAM_VEHICLE:
      if ( (ps->otherFlags & 2) == 0 )
        tweenTime = FLOAT_0_5;
      break;
    case CAM_VEHICLE_THIRDPERSON:
      tweenTime = FLOAT_0_40000001;
      break;
    case CAM_VEHICLE_GUNNER:
      if ( (unsigned int)oldMode <= CAM_VEHICLE )
        tweenTime = sTweenTime_0;
      if ( (oldMode == CAM_VEHICLE || oldMode == CAM_VEHICLE_GUNNER)
        && cgameGlob->cameraData.lastVehicleSeatPos != ps->vehiclePos )
      {
        tweenTime = sTweenTime;
      }
      break;
    case CAM_MISSILE:
      cgameGlob->cameraData.missileWasKillCam = cgameGlob->inKillCam == 1;
      if ( !cgameGlob->inKillCam )
      {
        missileViewAngles = cgameGlob->cameraData.missileViewAngles;
        viewangles = ps->viewangles;
        cgameGlob->cameraData.missileViewAngles[0] = ps->viewangles[0];
        missileViewAngles[1] = viewangles[1];
        missileViewAngles[2] = viewangles[2];
      }
      break;
    default:
      break;
  }
  if ( useTagCamera != cgameGlob->cameraData.useTagCamera && tweenTime == 0.0 )
  {
    v6 = ps->viewangles;
    lastViewAngles = cgameGlob->cameraData.lastViewAngles;
    ps->viewangles[0] = cgameGlob->cameraData.lastViewAngles[0];
    v6[1] = lastViewAngles[1];
    v6[2] = lastViewAngles[2];
    ps->viewangles[2] = *(float *)&FLOAT_0_0;
    temp[0] = ps->viewangles[0] - ps->delta_angles[0];
    temp[1] = ps->viewangles[1] - ps->delta_angles[1];
    temp[2] = ps->viewangles[2] - ps->delta_angles[2];
    CL_SetViewAngles(localClientNum, temp);
    Com_Printf(0, "CL_SetViewAngles() - ALPHA - (%3.0f, %3.0f, %3.0f)\n", temp[0], temp[1], temp[2]);
    tweenTime = FLOAT_0_40000001;
  }
  if ( CG_ShouldStartCameraTween(localClientNum, tweenTime) )
    CG_StartCameraTween(localClientNum, tweenTime);
  if ( oldMode == CAM_VEHICLE_GUNNER )
  {
    if ( (ps->otherFlags & 2) == 0 )
    {
      ps->viewangles[2] = *(float *)&FLOAT_0_0;
      angles = ps->viewangles[0] - ps->delta_angles[0];
      v14 = ps->viewangles[1] - ps->delta_angles[1];
      v15 = ps->viewangles[2] - ps->delta_angles[2];
      CL_SetViewAngles(localClientNum, &angles);
      Com_Printf(0, "CL_SetViewAngles() - BRAVO - (%3.0f, %3.0f, %3.0f)\n", angles, v14, v15);
    }
  }
  else if ( oldMode == CAM_MISSILE && !cgameGlob->cameraData.missileWasKillCam )
  {
    v4 = ps->viewangles;
    v5 = cgameGlob->cameraData.missileViewAngles;
    ps->viewangles[0] = cgameGlob->cameraData.missileViewAngles[0];
    v4[1] = v5[1];
    v4[2] = v5[2];
    v10 = ps->viewangles[0] - ps->delta_angles[0];
    v11 = ps->viewangles[1] - ps->delta_angles[1];
    v12 = ps->viewangles[2] - ps->delta_angles[2];
    CL_SetViewAngles(localClientNum, &v10);
    Com_Printf(0, "CL_SetViewAngles() - DELTA - (%3.0f, %3.0f, %3.0f)\n", v10, v11, v12);
  }
}

bool __cdecl CG_ShouldStartCameraTween(int localClientNum, float tweenTime)
{
  CG_GetLocalClientGlobals(localClientNum);
  return tweenTime > 0.0;
}

void __cdecl CG_UpdateVehicleBindings(int localClientNum)
{
  const vehicle_info_t *info; // [esp+8h] [ebp-4Ch]
  centity_s *vehicle; // [esp+Ch] [ebp-48h]
  int reverseBrakeButton; // [esp+10h] [ebp-44h]
  int gasButton; // [esp+14h] [ebp-40h]
  int attackSecondaryButton; // [esp+18h] [ebp-3Ch]
  cg_s *cgameGlob; // [esp+1Ch] [ebp-38h]
  int attackButton; // [esp+20h] [ebp-34h]
  int moveUpButton; // [esp+24h] [ebp-30h]
  int switchSeatButton; // [esp+28h] [ebp-2Ch]
  int firePickupButton; // [esp+2Ch] [ebp-28h]
  int dropDeployableButton; // [esp+30h] [ebp-24h]
  int boostButton; // [esp+34h] [ebp-20h]
  int specialAbilityButton; // [esp+38h] [ebp-1Ch]
  int twokeys[2]; // [esp+3Ch] [ebp-18h] BYREF
  int swapPickupButton; // [esp+44h] [ebp-10h]
  playerState_s *ps; // [esp+48h] [ebp-Ch]
  int moveDownButton; // [esp+4Ch] [ebp-8h]
  int handBrakeButton; // [esp+50h] [ebp-4h]

  if ( CL_IsLocalClientInGame(localClientNum) )
  {
    cgameGlob = CG_GetLocalClientGlobals(localClientNum);
    if ( cgameGlob )
    {
      if ( cgameGlob->snap )
      {
        ps = &cgameGlob->predictedPlayerState;
        if ( (cgameGlob->predictedPlayerState.eFlags & 0x4000) != 0 && !ps->vehiclePos && ps->viewlocked_entNum != 1023 )
        {
          vehicle = CG_GetEntity(localClientNum, ps->viewlocked_entNum);
          info = CG_GetVehicleInfo(vehicle->nextState.un2.vehicleState.vehicleInfoIndex);
          gasButton = info->gasButton;
          boostButton = info->boostButton;
          reverseBrakeButton = info->reverseBrakeButton;
          handBrakeButton = info->handBrakeButton;
          attackButton = info->attackButton;
          attackSecondaryButton = info->attackSecondaryButton;
          moveUpButton = info->moveUpButton;
          moveDownButton = info->moveDownButton;
          switchSeatButton = info->switchSeatButton;
          specialAbilityButton = info->specialAbilityButton;
          firePickupButton = info->firePickupButton;
          swapPickupButton = info->swapPickupButton;
          dropDeployableButton = info->dropDeployableButton;
          Cbuf_ExecuteBuffer(0, 0, "unbindall2");
          if ( Dvar_GetBool("gpad_enabled") )
          {
            if ( !I_strcmp(info->name, "motorcycle_player") )
              gasButton = 18;
          }
          else if ( !I_strcmp(info->name, "motorcycle_player") )
          {
            gasButton = 119;
          }
          Key_SetBinding(localClientNum, 18, (char *)&toastPopupTitle, 1);
          Key_SetBinding(localClientNum, 19, (char *)&toastPopupTitle, 1);
          Key_SetBinding(localClientNum, 6, (char *)&toastPopupTitle, 1);
          Key_SetBinding(localClientNum, 5, (char *)&toastPopupTitle, 1);
          Key_SetBinding(localClientNum, 1, (char *)&toastPopupTitle, 1);
          Key_SetBinding(localClientNum, 2, (char *)&toastPopupTitle, 1);
          Key_SetBinding(localClientNum, 3, (char *)&toastPopupTitle, 1);
          Key_SetBinding(localClientNum, 4, (char *)&toastPopupTitle, 1);
          Key_SetBinding(localClientNum, 16, (char *)&toastPopupTitle, 1);
          Key_SetBinding(localClientNum, 17, (char *)&toastPopupTitle, 1);
          if ( gasButton > 0 )
          {
            CG_RemapVehicleButton(localClientNum, twokeys, gasButton);
            Key_SetBindings(localClientNum, twokeys, "+gas", 1);
          }
          if ( reverseBrakeButton > 0 )
          {
            CG_RemapVehicleButton(localClientNum, twokeys, reverseBrakeButton);
            Key_SetBindings(localClientNum, twokeys, "+reverse", 1);
          }
          if ( handBrakeButton > 0 )
          {
            CG_RemapVehicleButton(localClientNum, twokeys, reverseBrakeButton);
            Key_SetBindings(localClientNum, twokeys, "+handbrake", 1);
          }
          if ( attackButton > 0 )
          {
            CG_RemapVehicleButton(localClientNum, twokeys, attackButton);
            Key_SetBindings(localClientNum, twokeys, "+vehicleattack", 1);
          }
          if ( attackSecondaryButton > 0 )
          {
            CG_RemapVehicleButton(localClientNum, twokeys, attackSecondaryButton);
            Key_SetBindings(localClientNum, twokeys, "+vehicleattacksecond", 1);
          }
          if ( moveUpButton > 0 )
          {
            CG_RemapVehicleButton(localClientNum, twokeys, moveUpButton);
            Key_SetBindings(localClientNum, twokeys, "+vehiclemoveup", 1);
          }
          if ( moveDownButton > 0 )
          {
            CG_RemapVehicleButton(localClientNum, twokeys, moveDownButton);
            Key_SetBindings(localClientNum, twokeys, "+vehiclemovedown", 1);
          }
          if ( switchSeatButton > 0 )
          {
            CG_RemapVehicleButton(localClientNum, twokeys, switchSeatButton);
            Key_SetBindings(localClientNum, twokeys, "+switchseat", 1);
          }
          if ( boostButton > 0 )
          {
            CG_RemapVehicleButton(localClientNum, twokeys, boostButton);
            Key_SetBindings(localClientNum, twokeys, "+vehicleboost", 1);
          }
          if ( specialAbilityButton > 0 )
          {
            CG_RemapVehicleButton(localClientNum, twokeys, specialAbilityButton);
            Key_SetBindings(localClientNum, twokeys, "+vehiclespecialability", 1);
          }
          if ( firePickupButton > 0 )
          {
            CG_RemapVehicleButton(localClientNum, twokeys, firePickupButton);
            Key_SetBindings(localClientNum, twokeys, "+vehiclefirepickup", 1);
          }
          if ( swapPickupButton > 0 )
          {
            CG_RemapVehicleButton(localClientNum, twokeys, swapPickupButton);
            Key_SetBindings(localClientNum, twokeys, "+vehicleswappickup", 1);
          }
          if ( dropDeployableButton > 0 )
          {
            CG_RemapVehicleButton(localClientNum, twokeys, dropDeployableButton);
            Key_SetBindings(localClientNum, twokeys, "+vehicledropdeployable", 1);
          }
        }
      }
    }
  }
}

int __cdecl CG_RemapVehicleButton(int localClientNum, int *twokeys, int button)
{
  int bindCount; // [esp+0h] [ebp-4h]

  bindCount = 0;
  if ( button == 19 )
    return Key_GetCommandAssignment(localClientNum, "+attack", twokeys, 0);
  if ( button != 18 )
  {
    switch ( button )
    {
      case 6:
        return Key_GetCommandAssignment(localClientNum, "+frag", twokeys, 0);
      case 5:
        return Key_GetCommandAssignment(localClientNum, "+smoke", twokeys, 0);
      case 2:
        return Key_GetCommandAssignment(localClientNum, "+stance", twokeys, 0);
      case 119:
        return Key_GetCommandAssignment(localClientNum, "+forward", twokeys, 0);
    }
    if ( button != 200 )
      return bindCount;
    return Key_GetCommandAssignment(localClientNum, "+attack", twokeys, 0);
  }
  bindCount = Key_GetCommandAssignment(localClientNum, "+speed_throw", twokeys, 0);
  if ( !bindCount )
    return Key_GetCommandAssignment(localClientNum, "+toggleads_throw", twokeys, 0);
  return bindCount;
}

bool __cdecl CG_RenderPlayerFromMissilePOV(int localClientNum)
{
  return (CG_GetLocalClientGlobals(localClientNum)->predictedPlayerState.eFlags2 & 0x40000) != 0;
}

bool __cdecl CG_ExtraCamViewActive(int localClientNum)
{
  const cg_s *cgameGlob; // [esp+0h] [ebp-4h]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  return cgameGlob->extraCamActive && (cgameGlob->predictedPlayerState.weapFlags & 0x200000) != 0;
}

bool __cdecl ShouldDoCameraTransition(cg_s *cgameGlob, CameraMode prevMode, CameraMode newMode, bool useTagCamera)
{
  if ( cgameGlob->inKillCam != cgameGlob->wasInKillCam )
    return 0;
  if ( cgameGlob->nextSnap && cgameGlob->nextSnap->ps.clientNum != cgameGlob->snap->ps.clientNum )
    return 0;
  if ( prevMode == newMode )
    goto LABEL_13;
  if ( prevMode >= CAM_VEHICLE && prevMode <= CAM_VEHICLE_GUNNER )
    return 0;
  if ( newMode >= CAM_VEHICLE && newMode <= CAM_VEHICLE_GUNNER )
    return 0;
LABEL_13:
  if ( prevMode != newMode )
    return 1;
  if ( useTagCamera == cgameGlob->cameraData.useTagCamera )
    return (cgameGlob->predictedPlayerState.eFlags & 0x4000) != 0
        && cgameGlob->cameraData.lastVehicleSeatPos != cgameGlob->predictedPlayerState.vehiclePos;
  return 1;
}

CameraMode __cdecl CG_UpdateCameraMode(int localClientNum)
{
  bool v2; // [esp+Ch] [ebp-30h]
  const vehicle_info_t *info; // [esp+18h] [ebp-24h]
  centity_s *vehicle; // [esp+1Ch] [ebp-20h]
  bool bSkipTransition; // [esp+23h] [ebp-19h]
  cg_s *cgameGlob; // [esp+24h] [ebp-18h]
  CameraMode newMode; // [esp+28h] [ebp-14h]
  CameraMode prevMode; // [esp+34h] [ebp-8h]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  CG_GetLocalClientStaticGlobals(localClientNum);
  prevMode = cgameGlob->cameraData.lastCamMode;
  newMode = CAM_NORMAL;
  bSkipTransition = 0;
  if ( cgameGlob->predictedPlayerState.pm_type == 2 )
  {
    newMode = CAM_NORMAL;
  }
  else if ( cgameGlob->cameraMode == 1 )
  {
    newMode = CAM_NORMAL;
  }
  else if ( cgameGlob->radiantCamInUse )
  {
    newMode = CAM_RADIANT;
  }
  else if ( player_topDownCamMode->current.integer )
  {
    newMode = CAM_TOP_DOWN;
  }
  else if ( (cgameGlob->predictedPlayerState.eFlags & 0x4000) != 0 )
  {
    vehicle = CG_GetEntity(localClientNum, cgameGlob->predictedPlayerState.viewlocked_entNum);
    info = CG_GetVehicleInfo(vehicle->nextState.un2.vehicleState.vehicleInfoIndex);
    v2 = (!info->type || info->type == 4 || info->type == 1) && info->cameraMode == 1;
    cgameGlob->cameraData.lastVehicleInfoIndex = vehicle->nextState.un2.vehicleState.vehicleInfoIndex;
    if ( cgameGlob->predictedPlayerState.vehiclePos < 1 || cgameGlob->predictedPlayerState.vehiclePos > 4 )
    {
      if ( cgameGlob->renderingThirdPerson == TP_FOR_MODEL && v2
        || !cgameGlob->predictedPlayerState.vehiclePos && info->thirdPersonDriver )
      {
        newMode = CAM_VEHICLE_THIRDPERSON;
      }
      else if ( cgameGlob->predictedPlayerState.viewlocked_entNum != 1023 && !cgameGlob->predictedPlayerState.vehiclePos )
      {
        newMode = CAM_VEHICLE;
      }
    }
    else
    {
      newMode = CAM_VEHICLE_GUNNER;
    }
  }
  else if ( (cgameGlob->predictedPlayerState.eFlags & 0x300) == 0
         || cgameGlob->predictedPlayerState.viewlocked_entNum == 1023 )
  {
    if ( CG_RenderPlayerFromMissilePOV(localClientNum) )
    {
      newMode = CAM_MISSILE;
    }
    else if ( cgameGlob->predictedPlayerState.pm_type == 1 )
    {
      newMode = CAM_LINKED;
    }
  }
  else
  {
    newMode = CAM_TURRET;
  }
  if ( cgameGlob->predictedPlayerState.clientNum != cgameGlob->cameraData.lastClientNum )
  {
    bSkipTransition = 1;
    if ( prevMode != newMode
      && !cgameGlob->inKillCam
      && newMode == CAM_NORMAL
      && prevMode >= CAM_VEHICLE
      && prevMode <= CAM_VEHICLE_GUNNER
      && (cgameGlob->predictedPlayerState.otherFlags & 2) == 0 )
    {
      CL_ClampViewAngle(
        localClientNum,
        0,
        COERCE_FLOAT(LODWORD(cgameGlob->predictedPlayerState.delta_angles[0]) ^ _mask__NegFloat_),
        COERCE_FLOAT(LODWORD(cgameGlob->predictedPlayerState.delta_angles[0]) ^ _mask__NegFloat_));
      Com_Printf(0, "CL_SetViewAngles() - HUEY\n");
    }
  }
  if ( !bSkipTransition && ShouldDoCameraTransition(cgameGlob, prevMode, newMode, 0) )
    CG_UpdateCameraTransition(localClientNum, prevMode, newMode, 0);
  if ( prevMode != newMode )
    CG_UpdateVehicleInitView(localClientNum, newMode);
  if ( cgameGlob->vehicleInitView )
    CG_UpdateVehicleBindings(localClientNum);
  cgameGlob->cameraData.lastCamMode = newMode;
  cgameGlob->cameraData.lastVehicleSeatPos = cgameGlob->predictedPlayerState.vehiclePos;
  cgameGlob->cameraData.useTagCamera = 0;
  cgameGlob->cameraData.lastClientNum = cgameGlob->predictedPlayerState.clientNum;
  return newMode;
}

int __cdecl CG_UpdateExtraCamMode(int localClientNum)
{
  CameraMode newMode; // [esp+0h] [ebp-4h]

  newMode = CAM_NORMAL;
  if ( CG_ExtraCamViewActive(localClientNum) )
    newMode = CAM_EXTRACAM;
  if ( CG_RenderPlayerFromMissilePOV(localClientNum) )
    return 6;
  return newMode;
}

void __cdecl CG_ApplyViewAnimation(int localClientNum)
{
  cg_s *cgameGlob; // [esp+20h] [ebp-10h]
  const ViewModelInfo *viewModelInfo; // [esp+24h] [ebp-Ch]
  const playerState_s *ps; // [esp+2Ch] [ebp-4h]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  ps = &cgameGlob->predictedPlayerState;
  if ( cgameGlob->predictedPlayerState.pm_type != 3
    && cgameGlob->predictedPlayerState.pm_type != 2
    && (cgameGlob->predictedPlayerState.eFlags & 0x300) == 0
    && !player_topDownCamMode->current.integer
    && !BG_UsingVehicleWeapon(ps)
    && cgameGlob->renderingThirdPerson != TP_FOR_MODEL
    && (int)BG_GetViewmodelWeaponIndex(ps) > 0 )
  {
    viewModelInfo = CG_GetLocalClientViewModelInfo(localClientNum);
    if ( viewModelInfo->viewModelDObj )
    {
      cgameGlob->viewModelAxis[0][0] = cgameGlob->refdef.viewaxis[0][0];
      cgameGlob->viewModelAxis[0][1] = cgameGlob->refdef.viewaxis[0][1];
      cgameGlob->viewModelAxis[0][2] = cgameGlob->refdef.viewaxis[0][2];
      cgameGlob->viewModelAxis[1][0] = cgameGlob->refdef.viewaxis[1][0];
      cgameGlob->viewModelAxis[1][1] = cgameGlob->refdef.viewaxis[1][1];
      cgameGlob->viewModelAxis[1][2] = cgameGlob->refdef.viewaxis[1][2];
      cgameGlob->viewModelAxis[2][0] = cgameGlob->refdef.viewaxis[2][0];
      cgameGlob->viewModelAxis[2][1] = cgameGlob->refdef.viewaxis[2][1];
      cgameGlob->viewModelAxis[2][2] = cgameGlob->refdef.viewaxis[2][2];
      cgameGlob->viewModelAxis[3][0] = cgameGlob->refdef.vieworg[0];
      cgameGlob->viewModelAxis[3][1] = cgameGlob->refdef.vieworg[1];
      cgameGlob->viewModelAxis[3][2] = cgameGlob->refdef.vieworg[2];
      CG_UpdateViewModelPose(viewModelInfo->viewModelDObj, localClientNum);
      if ( CG_DObjGetWorldTagMatrix(
             &cgameGlob->viewModelPose,
             viewModelInfo->viewModelDObj,
             scr_const.tag_camera,
             cgameGlob->refdef.viewaxis,
             cgameGlob->refdef.vieworg) )
      {
        AxisToAngles(cgameGlob->refdef.viewaxis, cgameGlob->refdefViewAngles);
      }
    }
  }
}

ViewModelInfo *__cdecl CG_GetLocalClientViewModelInfo(int localClientNum)
{
  if ( localClientNum
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\cgame\\../cgame/cg_local.h",
          236,
          0,
          "%s\n\t(localClientNum) = %i",
          "(localClientNum == 0)",
          localClientNum) )
  {
    __debugbreak();
  }
  return cg_viewModelArray;
}

double __cdecl CG_GetViewFov(int localClientNum)
{
  int v2; // eax
  float cameraFOV; // [esp+Ch] [ebp-80h]
  float fAdsZoomFov1; // [esp+10h] [ebp-7Ch]
  float fAdsZoomFov2; // [esp+14h] [ebp-78h]
  float fAdsZoomFov3; // [esp+18h] [ebp-74h]
  int v8; // [esp+1Ch] [ebp-70h]
  float v9; // [esp+20h] [ebp-6Ch]
  float v10; // [esp+24h] [ebp-68h]
  float v11; // [esp+28h] [ebp-64h]
  const vehicle_info_t *info; // [esp+54h] [ebp-38h]
  centity_s *vehicle; // [esp+58h] [ebp-34h]
  unsigned intthisTime; // [esp+5Ch] [ebp-30h]
  float weaponFov; // [esp+64h] [ebp-28h]
  float posLerp; // [esp+68h] [ebp-24h]
  cg_s *cgameGlob; // [esp+6Ch] [ebp-20h]
  const WeaponVariantDef *weapVariantDef; // [esp+74h] [ebp-18h]
  int weapIndex; // [esp+78h] [ebp-14h]
  const WeaponDef *weapDef; // [esp+80h] [ebp-Ch]
  float zoomFrac; // [esp+84h] [ebp-8h]
  float viewFov; // [esp+88h] [ebp-4h]
  float viewFova; // [esp+88h] [ebp-4h]
  float viewFovb; // [esp+88h] [ebp-4h]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  weapIndex = CG_GetPlayerWeapon(&cgameGlob->predictedPlayerState, localClientNum);
  weapVariantDef = BG_GetWeaponVariantDef(weapIndex);
  weapDef = BG_GetWeaponDef(weapIndex);
  viewFov = cg_fov->current.value;
  if ( (viewFov < 1.0 || viewFov > 160.0)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_camera.cpp",
          932,
          1,
          "%s\n\t(viewFov) = %g",
          "(viewFov >= 1 && viewFov <= 160)",
          viewFov) )
  {
    __debugbreak();
  }
  if ( CG_ShouldDefaultViewFov(localClientNum) )
    return viewFov;
  if ( BG_IsAimDownSightWeapon(weapIndex) )
  {
    if ( weapVariantDef->fAdsZoomFov1 <= 0.0099999998
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_camera.cpp",
            943,
            0,
            "%s",
            "weapVariantDef->fAdsZoomFov1 > 0.01f") )
    {
      __debugbreak();
    }
    posLerp = cgameGlob->predictedPlayerState.fWeaponPosFrac;
    if ( cg_adsZoomToggleStyle->current.integer )
    {
      v8 = cgameGlob->predictedPlayerState.adsZoomSelect % 3;
      if ( v8 == 1 )
      {
        if ( (float)(weapVariantDef->fAdsZoomFov2 - viewFov) < 0.0 )
          fAdsZoomFov2 = weapVariantDef->fAdsZoomFov2;
        else
          fAdsZoomFov2 = viewFov;
        weaponFov = fAdsZoomFov2;
      }
      else if ( v8 == 2 )
      {
        if ( (float)(weapVariantDef->fAdsZoomFov1 - viewFov) < 0.0 )
          fAdsZoomFov1 = weapVariantDef->fAdsZoomFov1;
        else
          fAdsZoomFov1 = viewFov;
        weaponFov = fAdsZoomFov1;
      }
      else
      {
        if ( (float)(weapVariantDef->fAdsZoomFov3 - viewFov) < 0.0 )
          fAdsZoomFov3 = weapVariantDef->fAdsZoomFov3;
        else
          fAdsZoomFov3 = viewFov;
        weaponFov = fAdsZoomFov3;
      }
    }
    else
    {
      v2 = cgameGlob->predictedPlayerState.adsZoomSelect % 4;
      if ( v2 )
      {
        if ( v2 == 2 )
        {
          if ( (float)(weapVariantDef->fAdsZoomFov1 - viewFov) < 0.0 )
            v9 = weapVariantDef->fAdsZoomFov1;
          else
            v9 = viewFov;
          weaponFov = v9;
        }
        else
        {
          if ( (float)(weapVariantDef->fAdsZoomFov2 - viewFov) < 0.0 )
            v10 = weapVariantDef->fAdsZoomFov2;
          else
            v10 = viewFov;
          weaponFov = v10;
        }
      }
      else
      {
        if ( (float)(weapVariantDef->fAdsZoomFov3 - viewFov) < 0.0 )
          v11 = weapVariantDef->fAdsZoomFov3;
        else
          v11 = viewFov;
        weaponFov = v11;
      }
    }
    if ( (_S1_1 & 1) == 0 )
    {
      _S1_1 |= 1u;
      lastTime = Sys_Milliseconds();
    }
    thisTime = Sys_Milliseconds();
    if ( posLerp == 1.0 )
    {
      viewFov = weaponFov;
    }
    else if ( posLerp != 0.0 )
    {
      if ( cgameGlob->playerEntity.bPositionToADS )
      {
        zoomFrac = posLerp - (float)(1.0 - weapVariantDef->fAdsZoomInFrac);
        if ( zoomFrac > 0.0 )
          zoomFrac = zoomFrac / weapVariantDef->fAdsZoomInFrac;
      }
      else
      {
        zoomFrac = posLerp - (float)(1.0 - weapVariantDef->fAdsZoomOutFrac);
        if ( zoomFrac > 0.0 )
          zoomFrac = zoomFrac / weapVariantDef->fAdsZoomOutFrac;
      }
      if ( zoomFrac > 0.0 )
        viewFov = viewFov - (float)((float)(viewFov - weaponFov) * zoomFrac);
    }
    lastTime = thisTime;
  }
  if ( (cgameGlob->predictedPlayerState.otherFlags & 2) == 0
    && cgameGlob->predictedPlayerState.weaponstate == 35
    && weapDef != BG_GetWeaponDef(0) )
  {
    viewFova = cg_fov->current.value;
    if ( !weapDef->deployTime
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_camera.cpp",
            1017,
            0,
            "%s",
            "weapDef->deployTime != 0") )
    {
      __debugbreak();
    }
    viewFov = (float)((float)(55.0 - cg_fov->current.value)
                    * (float)(1.0
                            - (float)((float)cgameGlob->predictedPlayerState.weaponTime / (float)weapDef->deployTime)))
            + viewFova;
  }
  if ( (cgameGlob->predictedPlayerState.eFlags & 0x300) != 0 )
    viewFov = FLOAT_55_0;
  if ( (cgameGlob->predictedPlayerState.eFlags & 0x4000) != 0 )
  {
    vehicle = CG_GetEntity(localClientNum, cgameGlob->predictedPlayerState.viewlocked_entNum);
    if ( vehicle )
    {
      if ( ((*((unsigned int *)vehicle + 201) >> 1) & 1) != 0 )
      {
        info = CG_GetVehicleInfo(vehicle->nextState.un2.vehicleState.vehicleInfoIndex);
        if ( info->cameraFOV <= cg_fovMin->current.value )
          cameraFOV = viewFov;
        else
          cameraFOV = info->cameraFOV;
        viewFov = cameraFOV;
      }
    }
  }
  viewFovb = viewFov * cg_fovScale->current.value;
  if ( (float)(cg_fovMin->current.value - viewFovb) < 0.0 )
    return viewFovb;
  else
    return cg_fovMin->current.value;
}

bool __cdecl CG_ShouldDefaultViewFov(int localClientNum)
{
  cg_s *cgameGlob; // [esp+0h] [ebp-8h]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  if ( Demo_IsThirdPersonCamera() || Demo_IsMovieCamera() )
    return 1;
  if ( cgameGlob->cameraMode )
    return 1;
  if ( (cgameGlob->predictedPlayerState.eFlags & 0x300) != 0 || (cgameGlob->predictedPlayerState.eFlags & 0x4000) != 0 )
    return 0;
  return (cgameGlob->predictedPlayerState.otherFlags & 2) != 0 && cgameGlob->renderingThirdPerson == TP_FOR_MODEL;
}

void __cdecl CG_CalcFov(int localClientNum, float fov_x)
{
  double v2; // xmm0_8
  long double v3; // [esp+4h] [ebp-18h]
  long double var18a; // [esp+4h] [ebp-18h]
  cg_s *cgameGlob; // [esp+Ch] [ebp-10h]
  float dxDzAtDefaultAspectRatio; // [esp+10h] [ebp-Ch]
  const cgs_t *cgs; // [esp+14h] [ebp-8h]
  float fov_xa; // [esp+28h] [ebp+Ch]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  cgs = CG_GetLocalClientStaticGlobals(localClientNum);
  if ( fov_x <= 0.0 )
    fov_x = CG_GetViewFov(localClientNum);
  fov_xa = CG_UpdateCameraTweenFOV(localClientNum, fov_x);
  __libm_sse2_tan(v3);
  dxDzAtDefaultAspectRatio = (float)(fov_xa * 0.017453292) * 0.5;
  *((float *)&var18a + 1) = (float)(dxDzAtDefaultAspectRatio * 0.75) * cgs->viewAspect;
  cgameGlob->refdef.tanHalfFovX = *((float *)&var18a + 1);
  cgameGlob->refdef.tanHalfFovY = dxDzAtDefaultAspectRatio * 0.75;
  cgameGlob->refdef.fov_x = fov_xa;
  *(float *)&var18a = (float)(cg_fov->current.value * 0.017453292) * 0.5;
  v2 = *(float *)&var18a;
  __libm_sse2_tan(var18a);
  *(float *)&v2 = v2;
  cgameGlob->zoomSensitivity = dxDzAtDefaultAspectRatio / *(float *)&v2;
}

void __cdecl CG_CalculateGunnerOffset_Sway(
        const float *playerViewAngles,
        unsigned int weapon,
        float *swayViewAngles,
        float *swayOffset,
        float *swayAngles,
        float ssSwayScale,
        int frametime)
{
  float v7; // [esp+10h] [ebp-54h]
  float v8; // [esp+14h] [ebp-50h]
  float v9; // [esp+1Ch] [ebp-48h]
  float v10; // [esp+24h] [ebp-40h]
  float deltaOffset_8; // [esp+34h] [ebp-30h]
  float swayPitchScale; // [esp+38h] [ebp-2Ch]
  float swayPitchScalea; // [esp+38h] [ebp-2Ch]
  float dt; // [esp+3Ch] [ebp-28h]
  float swayVertScale; // [esp+40h] [ebp-24h]
  float swayVertScalea; // [esp+40h] [ebp-24h]
  float swayLerpSpeed; // [esp+44h] [ebp-20h]
  float swayHorizScale; // [esp+48h] [ebp-1Ch]
  float swayHorizScalea; // [esp+48h] [ebp-1Ch]
  float deltaAngles[3]; // [esp+4Ch] [ebp-18h] BYREF
  float swayYawScale; // [esp+58h] [ebp-Ch]
  float swayMaxAngle; // [esp+5Ch] [ebp-8h]
  const WeaponDef *weapDef; // [esp+60h] [ebp-4h]

  if ( frametime )
  {
    dt = (float)frametime * 0.001;
    if ( weapon )
    {
      weapDef = BG_GetWeaponDef(weapon);
      swayMaxAngle = weapDef->swayMaxAngle;
      swayLerpSpeed = weapDef->swayLerpSpeed;
      swayPitchScale = weapDef->swayPitchScale;
      swayYawScale = weapDef->swayYawScale;
      swayHorizScale = weapDef->swayHorizScale;
      swayVertScale = weapDef->swayVertScale;
    }
    else
    {
      swayMaxAngle = FLOAT_30_0;
      swayLerpSpeed = FLOAT_6_0;
      if ( ssSwayScale > 25.0 )
        swayLerpSpeed = FLOAT_2_0;
      swayPitchScale = FLOAT_2_0;
      swayYawScale = FLOAT_2_0;
      swayHorizScale = FLOAT_2_0;
      swayVertScale = FLOAT_2_0;
    }
    swayPitchScalea = swayPitchScale * ssSwayScale;
    swayYawScale = swayYawScale * ssSwayScale;
    swayHorizScalea = swayHorizScale * ssSwayScale;
    swayVertScalea = swayVertScale * ssSwayScale;
    AnglesSubtract(playerViewAngles, swayViewAngles, deltaAngles);
    if ( dt == 0.0 && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_camera.cpp", 1179, 0, "%s", "dt") )
      __debugbreak();
    deltaAngles[0] = (float)(1.0 / (float)(60.0 * dt)) * deltaAngles[0];
    deltaAngles[1] = (float)(1.0 / (float)(60.0 * dt)) * deltaAngles[1];
    deltaAngles[2] = (float)(1.0 / (float)(60.0 * dt)) * deltaAngles[2];
    if ( (float)(deltaAngles[0] - swayMaxAngle) < 0.0 )
      v10 = deltaAngles[0];
    else
      v10 = swayMaxAngle;
    if ( (float)(COERCE_FLOAT(LODWORD(swayMaxAngle) ^ _mask__NegFloat_) - deltaAngles[0]) < 0.0 )
      v8 = v10;
    else
      LODWORD(v8) = LODWORD(swayMaxAngle) ^ _mask__NegFloat_;
    deltaAngles[0] = v8;
    if ( (float)(deltaAngles[1] - swayMaxAngle) < 0.0 )
      v9 = deltaAngles[1];
    else
      v9 = swayMaxAngle;
    if ( (float)(COERCE_FLOAT(LODWORD(swayMaxAngle) ^ _mask__NegFloat_) - deltaAngles[1]) < 0.0 )
      v7 = v9;
    else
      LODWORD(v7) = LODWORD(swayMaxAngle) ^ _mask__NegFloat_;
    deltaAngles[1] = v7;
    deltaOffset_8 = deltaAngles[0] * swayVertScalea;
    swayOffset[1] = DiffTrack(v7 * swayHorizScalea, swayOffset[1], swayLerpSpeed, dt);
    swayOffset[2] = DiffTrack(deltaOffset_8, swayOffset[2], swayLerpSpeed, dt);
    deltaAngles[0] = deltaAngles[0] * swayPitchScalea;
    deltaAngles[1] = deltaAngles[1] * swayYawScale;
    *swayAngles = DiffTrackAngle(deltaAngles[0], *swayAngles, swayLerpSpeed, dt);
    swayAngles[1] = DiffTrackAngle(deltaAngles[1], swayAngles[1], swayLerpSpeed, dt);
    *swayViewAngles = *playerViewAngles;
    swayViewAngles[1] = playerViewAngles[1];
    swayViewAngles[2] = playerViewAngles[2];
  }
}

void __cdecl CG_OffsetVehicleGunner(int localClientNum, cg_s *cgameGlob)
{
  int WeaponIndexForName; // eax
  double v3; // st7
  double v4; // st7
  double v5; // st7
  float *v6; // [esp+20h] [ebp-254h]
  float *v7; // [esp+24h] [ebp-250h]
  float v8; // [esp+2Ch] [ebp-248h]
  float v9; // [esp+38h] [ebp-23Ch]
  float v10; // [esp+74h] [ebp-200h]
  float v11; // [esp+80h] [ebp-1F4h]
  float v12; // [esp+88h] [ebp-1ECh]
  float v13; // [esp+8Ch] [ebp-1E8h]
  float *v14; // [esp+90h] [ebp-1E4h]
  float *viewangles; // [esp+94h] [ebp-1E0h]
  float out[9]; // [esp+B8h] [ebp-1BCh] BYREF
  float playerAngles[3]; // [esp+DCh] [ebp-198h] BYREF
  float mtx[3][3]; // [esp+E8h] [ebp-18Ch] BYREF
  float vehiclePitchInfluence; // [esp+10Ch] [ebp-168h]
  float asdf[3]; // [esp+110h] [ebp-164h]
  float viewAxis[3][3]; // [esp+11Ch] [ebp-158h] BYREF
  float temp[3]; // [esp+140h] [ebp-134h] BYREF
  float vehicleYawInfluence; // [esp+14Ch] [ebp-128h]
  float deltaAxis[3][3]; // [esp+150h] [ebp-124h] BYREF
  float tagMtx2[4][3]; // [esp+174h] [ebp-100h] BYREF
  float vehicleRollInfluence; // [esp+1A4h] [ebp-D0h]
  float v27; // [esp+1A8h] [ebp-CCh]
  float fc; // [esp+1ACh] [ebp-C8h]
  float crouchPos[3]; // [esp+1B0h] [ebp-C4h] BYREF
  float f; // [esp+1BCh] [ebp-B8h]
  float tagCrouchMtx[4][3]; // [esp+1C0h] [ebp-B4h] BYREF
  DObj *obj; // [esp+1F0h] [ebp-84h]
  vehicle_info_t *info; // [esp+1F4h] [ebp-80h]
  int weapon; // [esp+1F8h] [ebp-7Ch]
  int gunnerIndex; // [esp+1FCh] [ebp-78h]
  unsigned __int16 *gunnerViewCrouchTags[4]; // [esp+200h] [ebp-74h]
  float viewPos[3]; // [esp+210h] [ebp-64h]
  float offset[3]; // [esp+21Ch] [ebp-58h] BYREF
  centity_s *vehicle; // [esp+228h] [ebp-4Ch]
  playerState_s *ps; // [esp+22Ch] [ebp-48h]
  float tagMtx[4][3]; // [esp+230h] [ebp-44h] BYREF
  const WeaponDef *weapDef; // [esp+260h] [ebp-14h]
  unsigned __int16 *gunnerViewTags[4]; // [esp+264h] [ebp-10h]

  gunnerViewTags[0] = &scr_const.tag_gunner_barrel1;
  gunnerViewTags[1] = &scr_const.tag_gunner_barrel2;
  gunnerViewTags[2] = &scr_const.tag_gunner_barrel3;
  gunnerViewTags[3] = &scr_const.tag_gunner_barrel4;
  gunnerViewCrouchTags[0] = &scr_const.tag_gunner1;
  gunnerViewCrouchTags[1] = &scr_const.tag_gunner2;
  gunnerViewCrouchTags[2] = &scr_const.tag_gunner3;
  gunnerViewCrouchTags[3] = &scr_const.tag_gunner4;
  ps = &cgameGlob->predictedPlayerState;
  vehicle = CG_GetEntity(localClientNum, cgameGlob->predictedPlayerState.viewlocked_entNum);
  obj = Com_GetClientDObj(vehicle->nextState.number, localClientNum);
  if ( obj )
  {
    gunnerIndex = ps->vehiclePos - 1;
    if ( CG_DObjGetWorldTagMatrix(&vehicle->pose, obj, *gunnerViewTags[gunnerIndex], tagMtx, tagMtx[3]) )
    {
      info = CG_GetVehicleInfo(vehicle->nextState.un2.vehicleState.vehicleInfoIndex);
      weapon = info->gunnerWeaponIndex[gunnerIndex];
      if ( !weapon )
      {
        WeaponIndexForName = CG_GetWeaponIndexForName(info->gunnerWeapon[gunnerIndex]);
        AssignToSmallerType<unsigned short>(&info->gunnerWeaponIndex[gunnerIndex], WeaponIndexForName);
        weapon = info->gunnerWeaponIndex[gunnerIndex];
      }
      weapDef = BG_GetWeaponDef(weapon);
      if ( weapDef )
      {
        *(_QWORD *)viewPos = *(_QWORD *)&tagMtx[3][0];
        viewPos[2] = tagMtx[3][2];
        BG_CalcVehicleTurretWeaponPosOffset(ps->fWeaponPosFrac, weapDef, offset);
        viewPos[0] = (float)(offset[2] * tagMtx[2][0])
                   + (float)((float)(offset[1] * tagMtx[1][0]) + (float)((float)(offset[0] * tagMtx[0][0]) + viewPos[0]));
        viewPos[1] = (float)(offset[2] * tagMtx[2][1])
                   + (float)((float)(offset[1] * tagMtx[1][1]) + (float)((float)(offset[0] * tagMtx[0][1]) + viewPos[1]));
        viewPos[2] = (float)(offset[2] * tagMtx[2][2])
                   + (float)((float)(offset[1] * tagMtx[1][2]) + (float)((float)(offset[0] * tagMtx[0][2]) + viewPos[2]));
        if ( cgameGlob->vehicleInitView )
        {
          v14 = swayViewAngles[localClientNum];
          viewangles = ps->viewangles;
          *v14 = ps->viewangles[0];
          v14[1] = viewangles[1];
          v14[2] = viewangles[2];
        }
        CG_CalculateGunnerOffset_Sway(
          ps->viewangles,
          weapon,
          swayViewAngles[localClientNum],
          swayOffset[localClientNum],
          swayAngles[localClientNum],
          4.0,
          cgameGlob->frametime);
        v13 = swayOffset[localClientNum][0];
        viewPos[0] = (float)(v13 * tagMtx[0][0]) + viewPos[0];
        viewPos[1] = (float)(v13 * tagMtx[0][1]) + viewPos[1];
        viewPos[2] = (float)(v13 * tagMtx[0][2]) + viewPos[2];
        v12 = *(float *)&dword_E86ACC[3 * localClientNum];
        viewPos[0] = (float)(v12 * tagMtx[1][0]) + viewPos[0];
        viewPos[1] = (float)(v12 * tagMtx[1][1]) + viewPos[1];
        viewPos[2] = (float)(v12 * tagMtx[1][2]) + viewPos[2];
        LODWORD(v11) = dword_E86AD0[3 * localClientNum] ^ _mask__NegFloat_;
        viewPos[0] = (float)(v11 * tagMtx[2][0]) + viewPos[0];
        viewPos[1] = (float)(v11 * tagMtx[2][1]) + viewPos[1];
        viewPos[2] = (float)(v11 * tagMtx[2][2]) + viewPos[2];
        cgameGlob->refdef.vieworg[0] = viewPos[0];
        cgameGlob->refdef.vieworg[1] = viewPos[1];
        cgameGlob->refdef.vieworg[2] = viewPos[2];
        AxisToAngles(tagMtx, cgameGlob->refdefViewAngles);
        if ( ps->leanf > 0.0 )
        {
          v10 = (float)(ps->leanf * 3.1415927) - 1.5707964;
          fc = cos(v10);
          v27 = sin(v10);
          f = (float)(v27 + 1.0) / 2.0;
          if ( CG_DObjGetWorldTagMatrix(
                 &vehicle->pose,
                 obj,
                 *gunnerViewCrouchTags[gunnerIndex],
                 tagCrouchMtx,
                 tagCrouchMtx[3]) )
          {
            crouchPos[0] = (float)(crouchOfs[0] * tagCrouchMtx[0][0]) + tagCrouchMtx[3][0];
            crouchPos[1] = (float)(crouchOfs[0] * tagCrouchMtx[0][1]) + tagCrouchMtx[3][1];
            crouchPos[2] = (float)(crouchOfs[0] * tagCrouchMtx[0][2]) + tagCrouchMtx[3][2];
            crouchPos[0] = (float)(*(float *)&dword_E0289C * tagCrouchMtx[1][0]) + crouchPos[0];
            crouchPos[1] = (float)(*(float *)&dword_E0289C * tagCrouchMtx[1][1]) + crouchPos[1];
            crouchPos[2] = (float)(*(float *)&dword_E0289C * tagCrouchMtx[1][2]) + crouchPos[2];
            crouchPos[0] = (float)(*(float *)&dword_E028A0 * tagCrouchMtx[2][0]) + crouchPos[0];
            crouchPos[1] = (float)(*(float *)&dword_E028A0 * tagCrouchMtx[2][1]) + crouchPos[1];
            crouchPos[2] = (float)(*(float *)&dword_E028A0 * tagCrouchMtx[2][2]) + crouchPos[2];
            Vec3Lerp(cgameGlob->refdef.vieworg, crouchPos, f, cgameGlob->refdef.vieworg);
            v3 = AngleNormalize180(cgameGlob->refdefViewAngles[0]);
            cgameGlob->refdefViewAngles[0] = v3 * (1.0 - f);
          }
          else if ( !CG_DObjGetWorldTagMatrix(
                       &vehicle->pose,
                       obj,
                       *gunnerViewCrouchTags[gunnerIndex],
                       tagCrouchMtx,
                       tagCrouchMtx[3])
                 && !Assert_MyHandler(
                       "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_camera.cpp",
                       1294,
                       0,
                       "%s",
                       "CG_DObjGetWorldTagMatrix( &vehicle->pose, obj, *gunnerViewCrouchTags[gunnerIndex], tagCrouchMtx, "
                       "tagCrouchMtx[3] )") )
          {
            __debugbreak();
          }
        }
        AnglesToAxis(vehicle->pose.angles, tagMtx2);
        tagMtx2[3][0] = vehicle->pose.origin[0];
        tagMtx2[3][1] = vehicle->pose.origin[1];
        tagMtx2[3][2] = vehicle->pose.origin[2];
        if ( cgameGlob->vehicleInitView )
        {
          MatrixTranspose(tagMtx2, cgameGlob->prevVehicleInvAxis);
          cgameGlob->vehicleInitView = 0;
        }
        MatrixMultiply(cgameGlob->prevVehicleInvAxis, tagMtx2, deltaAxis);
        MatrixTranspose(tagMtx2, cgameGlob->prevVehicleInvAxis);
        if ( gVehicleRelativeGunnerAngles )
        {
          MatrixTranspose(deltaAxis, (float (*)[3])out);
          AxisCopy((const float (*)[3])out, deltaAxis);
        }
        playerAngles[0] = cgameGlob->refdefViewAngles[0];
        playerAngles[1] = cgameGlob->refdefViewAngles[1];
        playerAngles[2] = cgameGlob->refdefViewAngles[2];
        AnglesToAxis(playerAngles, viewAxis);
        MatrixMultiply(viewAxis, deltaAxis, mtx);
        AxisToAngles(mtx, temp);
        if ( ps->fWeaponPosFrac == 1.0 || (ps->eFlags & 0x40) != 0 && weapDef->weapType != WEAPTYPE_PROJECTILE )
        {
          vehiclePitchInfluence = cg_viewVehicleInfluenceGunnerFiring->current.value;
          vehicleYawInfluence = cg_viewVehicleInfluenceGunnerFiring->current.vector[1];
          vehicleRollInfluence = cg_viewVehicleInfluenceGunnerFiring->current.vector[2];
        }
        else
        {
          vehiclePitchInfluence = cg_viewVehicleInfluenceGunner->current.value;
          vehicleYawInfluence = cg_viewVehicleInfluenceGunner->current.vector[1];
          vehicleRollInfluence = cg_viewVehicleInfluenceGunner->current.vector[2];
        }
        if ( gVehicleRelativeGunnerAngles )
        {
          vehiclePitchInfluence = 1.0 - vehiclePitchInfluence;
          vehicleYawInfluence = 1.0 - vehicleYawInfluence;
        }
        else
        {
          vehiclePitchInfluence = *(float *)&FLOAT_0_0;
          vehicleYawInfluence = *(float *)&FLOAT_0_0;
          vehicleRollInfluence = *(float *)&FLOAT_0_0;
        }
        v9 = cgameGlob->refdefViewAngles[0];
        v4 = AngleNormalize180(temp[0] - v9);
        asdf[0] = v4 * vehiclePitchInfluence + v9;
        v8 = cgameGlob->refdefViewAngles[1];
        v5 = AngleNormalize180(temp[1] - v8);
        asdf[1] = v5 * vehicleYawInfluence + v8;
        asdf[2] = *(float *)&FLOAT_0_0;
        asdf[0] = asdf[0] - cgameGlob->refdefViewAngles[0];
        asdf[1] = asdf[1] - cgameGlob->refdefViewAngles[1];
        asdf[2] = *(float *)&FLOAT_0_0;
        v6 = ps->viewangles;
        v7 = ps->viewangles;
        ps->viewangles[0] = ps->viewangles[0] + asdf[0];
        v6[1] = v7[1] + asdf[1];
        v6[2] = v7[2] + asdf[2];
        temp[0] = ps->viewangles[0] - ps->delta_angles[0];
        temp[1] = ps->viewangles[1] - ps->delta_angles[1];
        temp[2] = ps->viewangles[2] - ps->delta_angles[2];
        CL_SetViewAngles(localClientNum, temp);
        Com_Printf(0, "CL_SetViewAngles() - TANGO - (%3.0f, %3.0f, %3.0f)\n", temp[0], temp[1], temp[2]);
        if ( !Demo_IsPaused()
          && !Demo_IsCompleted()
          && !Demo_GetClipPausedState()
          && cl_paused->current.value == 0.0
          && vehicle->vehicle
          && vehicle->vehicle->lastGunnerFire[gunnerIndex] > cgameGlob->time - 100 )
        {
          cgameGlob->refdefViewAngles[0] = crandom() * weapDef->vertViewJitter + cgameGlob->refdefViewAngles[0];
          cgameGlob->refdefViewAngles[1] = crandom() * weapDef->horizViewJitter + cgameGlob->refdefViewAngles[1];
        }
      }
    }
  }
}

void __cdecl Vec3Lerp(const float *start, const float *end, float fraction, float *endpos)
{
  *endpos = (float)((float)(*end - *start) * fraction) + *start;
  endpos[1] = (float)((float)(end[1] - start[1]) * fraction) + start[1];
  endpos[2] = (float)((float)(end[2] - start[2]) * fraction) + start[2];
}

void __cdecl CG_OffsetVehicleAnimCam(int localClientNum)
{
  float v1; // [esp+Ch] [ebp-84h]
  cg_s *cgameGlob; // [esp+20h] [ebp-70h]
  float vehPos; // [esp+24h] [ebp-6Ch]
  float vehPos_4; // [esp+28h] [ebp-68h]
  float vehPos_8; // [esp+2Ch] [ebp-64h]
  centity_s *vehicle; // [esp+30h] [ebp-60h]
  float vehAngles[3]; // [esp+34h] [ebp-5Ch] BYREF
  float camAngles[3]; // [esp+40h] [ebp-50h] BYREF
  int i; // [esp+4Ch] [ebp-44h]
  playerState_s *ps; // [esp+50h] [ebp-40h]
  float axis[3][3]; // [esp+54h] [ebp-3Ch] BYREF
  float vec[3]; // [esp+78h] [ebp-18h] BYREF
  float camPos[3]; // [esp+84h] [ebp-Ch]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  ps = &cgameGlob->predictedPlayerState;
  vehicle = CG_GetEntity(localClientNum, cgameGlob->predictedPlayerState.viewlocked_entNum);
  vehPos = vehicle->pose.origin[0];
  vehPos_4 = vehicle->pose.origin[1];
  vehPos_8 = vehicle->pose.origin[2];
  vehAngles[0] = vehicle->pose.angles[0];
  vehAngles[1] = vehicle->pose.angles[1];
  vehAngles[2] = vehicle->pose.angles[2];
  AnglesToAxis(vehAngles, axis);
  camPos[0] = vehPos;
  camPos[1] = vehPos_4;
  camPos[2] = vehPos_8;
  for ( i = 0; i < 3; ++i )
  {
    v1 = camOffset[i];
    camPos[0] = (float)(v1 * axis[i][0]) + camPos[0];
    camPos[1] = (float)(v1 * axis[i][1]) + camPos[1];
    camPos[2] = (float)(v1 * axis[i][2]) + camPos[2];
  }
  vec[0] = vehPos - camPos[0];
  vec[1] = vehPos_4 - camPos[1];
  vec[2] = vehPos_8 - camPos[2];
  vectoangles(vec, camAngles);
  cgameGlob->refdef.vieworg[0] = camPos[0];
  cgameGlob->refdef.vieworg[1] = camPos[1];
  cgameGlob->refdef.vieworg[2] = camPos[2];
  cgameGlob->refdefViewAngles[0] = camAngles[0];
  cgameGlob->refdefViewAngles[1] = camAngles[1];
  cgameGlob->refdefViewAngles[2] = camAngles[2];
}

void __cdecl CG_GetExtraCamOrigin(int localClientNum, float *out)
{
  cg_s *cgameGlob; // [esp+4h] [ebp-4h]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  *out = cgameGlob->extraCamOrigin[0];
  out[1] = cgameGlob->extraCamOrigin[1];
  out[2] = cgameGlob->extraCamOrigin[2];
}

void __cdecl CG_CalcExtraCamViewValues(int localClientNum)
{
  cg_s *cgameGlob; // [esp+8h] [ebp-4h]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  CG_GetExtraCamOrigin(localClientNum, cgameGlob->refdef.vieworg);
  cgameGlob->refdefViewAngles[0] = cgameGlob->extraCamAngles[0];
  cgameGlob->refdefViewAngles[1] = cgameGlob->extraCamAngles[1];
  cgameGlob->refdefViewAngles[2] = cgameGlob->extraCamAngles[2];
}

void __cdecl CG_CalcMissileViewValues(int localClientNum)
{
  cg_s *cgameGlob; // [esp+38h] [ebp-Ch]
  centity_s *missile; // [esp+3Ch] [ebp-8h]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  missile = CG_GetEntity(localClientNum, cgameGlob->predictedPlayerState.viewlocked_entNum);
  if ( missile->nextState.eType == 4 )
  {
    cgameGlob->storedRemoteCameraOrigin[0] = missile->pose.origin[0];
    cgameGlob->storedRemoteCameraOrigin[1] = missile->pose.origin[1];
    cgameGlob->storedRemoteCameraOrigin[2] = missile->pose.origin[2];
    cgameGlob->storedRemoteCameraAngles[0] = missile->pose.angles[0];
    cgameGlob->storedRemoteCameraAngles[1] = missile->pose.angles[1];
    cgameGlob->storedRemoteCameraAngles[2] = missile->pose.angles[2];
    cgameGlob->refdef.vieworg[0] = missile->pose.origin[0];
    cgameGlob->refdef.vieworg[1] = missile->pose.origin[1];
    cgameGlob->refdef.vieworg[2] = missile->pose.origin[2];
    cgameGlob->refdefViewAngles[0] = missile->pose.angles[0];
    cgameGlob->refdefViewAngles[1] = missile->pose.angles[1];
    cgameGlob->refdefViewAngles[2] = missile->pose.angles[2];
  }
  else
  {
    cgameGlob->refdef.vieworg[0] = cgameGlob->storedRemoteCameraOrigin[0];
    cgameGlob->refdef.vieworg[1] = cgameGlob->storedRemoteCameraOrigin[1];
    cgameGlob->refdef.vieworg[2] = cgameGlob->storedRemoteCameraOrigin[2];
    cgameGlob->refdefViewAngles[0] = cgameGlob->storedRemoteCameraAngles[0];
    cgameGlob->refdefViewAngles[1] = cgameGlob->storedRemoteCameraAngles[1];
    cgameGlob->refdefViewAngles[2] = cgameGlob->storedRemoteCameraAngles[2];
  }
}

void __cdecl CG_GetMissileViewOrigin(int localClientNum, float *out)
{
  const cg_s *cgameGlob; // [esp+Ch] [ebp-Ch]
  const centity_s *missile; // [esp+10h] [ebp-8h]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  missile = CG_GetEntity(localClientNum, cgameGlob->predictedPlayerState.viewlocked_entNum);
  *out = missile->pose.origin[0];
  out[1] = missile->pose.origin[1];
  out[2] = missile->pose.origin[2];
}

void __cdecl CG_CalcMissileAngleValues(int localClientNum)
{
  double v1; // xmm0_8
  double v2; // xmm0_8
  double v3; // xmm0_8
  long double v4; // [esp+0h] [ebp-30h]
  long double v5; // [esp+0h] [ebp-30h]
  long double v6; // [esp+0h] [ebp-30h]
  cg_s *cgameGlob; // [esp+20h] [ebp-10h]
  centity_s *missile; // [esp+28h] [ebp-8h]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  missile = CG_GetEntity(localClientNum, cgameGlob->predictedPlayerState.viewlocked_entNum);
  if ( missile
    && missile->nextState.eType == 4
    && (cgameGlob->predictedPlayerState.eFlags2 & 0x10000000) != 0
    && !Demo_IsPaused()
    && !Demo_IsCompleted()
    && !Demo_GetClipPausedState() )
  {
    if ( cgameGlob->time - missile->currentState.pos.trTime >= 100
      || cgameGlob->time - cgameGlob->missileViewWobbleLastTime <= 100 )
    {
      cgameGlob->missileViewWobbleAmp[0] = cgameGlob->missileViewWobbleAmp[0]
                                         - (float)((float)CG_irand(
                                                            0,
                                                            (int)(float)(cgameGlob->missileViewWobbleAmp[0] * d))
                                                 / 100.0);
      cgameGlob->missileViewWobbleAmp[1] = cgameGlob->missileViewWobbleAmp[1]
                                         - (float)((float)CG_irand(
                                                            0,
                                                            (int)(float)(cgameGlob->missileViewWobbleAmp[1] * d))
                                                 / 100.0);
      cgameGlob->missileViewWobbleAmp[2] = cgameGlob->missileViewWobbleAmp[2]
                                         - (float)((float)CG_irand(
                                                            0,
                                                            (int)(float)(cgameGlob->missileViewWobbleAmp[2] * d))
                                                 / 100.0);
    }
    else
    {
      cgameGlob->missileViewWobbleLastTime = cgameGlob->time;
      cgameGlob->missileViewWobbleAmp[0] = (float)CG_irand(0, p) / 100.0;
      cgameGlob->missileViewWobbleAmp[1] = (float)CG_irand(0, y) / 100.0;
      cgameGlob->missileViewWobbleAmp[2] = (float)CG_irand(0, r) / 100.0;
      cgameGlob->missileViewWobbleFreq[0] = (float)CG_irand(200, fp) / 100.0;
      cgameGlob->missileViewWobbleFreq[1] = (float)CG_irand(200, fy) / 100.0;
      cgameGlob->missileViewWobbleFreq[2] = (float)CG_irand(200, fr) / 100.0;
    }
    v1 = (float)((float)((float)(int)(Sys_Milliseconds() - cgameGlob->missileViewWobbleLastTime) / 1000.0)
               * cgameGlob->missileViewWobbleFreq[2]);
    __libm_sse2_sin(v4);
    *(float *)&v1 = v1;
    cgameGlob->refdefViewAngles[2] = missile->pose.angles[2]
                                   + (float)(*(float *)&v1 * cgameGlob->missileViewWobbleAmp[2]);
    v2 = (float)((float)((float)(int)(Sys_Milliseconds() - cgameGlob->missileViewWobbleLastTime) / 1000.0)
               * cgameGlob->missileViewWobbleFreq[0]);
    __libm_sse2_sin(v5);
    *(float *)&v2 = v2;
    cgameGlob->refdefViewAngles[0] = (float)(*(float *)&v2 * cgameGlob->missileViewWobbleAmp[0])
                                   + cgameGlob->refdefViewAngles[0];
    *((float *)&v6 + 1) = (float)((float)(int)(Sys_Milliseconds() - cgameGlob->missileViewWobbleLastTime) / 1000.0)
                        * cgameGlob->missileViewWobbleFreq[1];
    v3 = *((float *)&v6 + 1);
    __libm_sse2_sin(v6);
    *(float *)&v3 = v3;
    cgameGlob->refdefViewAngles[1] = (float)(*(float *)&v3 * cgameGlob->missileViewWobbleAmp[1])
                                   + cgameGlob->refdefViewAngles[1];
  }
}

void __cdecl CG_CalcVehicleViewValues(int localClientNum)
{
  double v1; // st7
  double v2; // st7
  float v3; // [esp+18h] [ebp-368h]
  float frametime; // [esp+1Ch] [ebp-364h]
  float v5; // [esp+24h] [ebp-35Ch]
  bool v6; // [esp+28h] [ebp-358h]
  float *v7; // [esp+2Ch] [ebp-354h]
  float v8; // [esp+34h] [ebp-34Ch]
  float fWeaponPosFrac; // [esp+48h] [ebp-338h]
  float v10; // [esp+50h] [ebp-330h]
  float v11; // [esp+54h] [ebp-32Ch]
  float v12; // [esp+58h] [ebp-328h]
  float v13; // [esp+5Ch] [ebp-324h]
  float v14; // [esp+64h] [ebp-31Ch]
  float v15; // [esp+68h] [ebp-318h]
  float v16; // [esp+6Ch] [ebp-314h]
  float v17; // [esp+70h] [ebp-310h]
  int j; // [esp+80h] [ebp-300h]
  int i; // [esp+94h] [ebp-2ECh]
  float *vieworg; // [esp+9Ch] [ebp-2E4h]
  float v21; // [esp+A4h] [ebp-2DCh]
  float v22; // [esp+ACh] [ebp-2D4h]
  float v23; // [esp+B0h] [ebp-2D0h]
  float *v24; // [esp+B4h] [ebp-2CCh]
  float *angles; // [esp+B8h] [ebp-2C8h]
  float v26; // [esp+E8h] [ebp-298h]
  float v27; // [esp+ECh] [ebp-294h]
  float v28; // [esp+F0h] [ebp-290h]
  float m_forward_vel; // [esp+F4h] [ebp-28Ch]
  float *viewangles; // [esp+F8h] [ebp-288h]
  float viewHeightCurrent; // [esp+104h] [ebp-27Ch]
  bool lerpViewBackToForward; // [esp+13Fh] [ebp-241h]
  usercmd_s ucmd; // [esp+140h] [ebp-240h] BYREF
  int CurrentCmdNumber; // [esp+174h] [ebp-20Ch]
  float angleDiff; // [esp+178h] [ebp-208h]
  usercmd_s cmd; // [esp+17Ch] [ebp-204h] BYREF
  int cmdNumber; // [esp+1B4h] [ebp-1CCh]
  float start[3]; // [esp+1B8h] [ebp-1C8h] BYREF
  float swayScale; // [esp+1C4h] [ebp-1BCh]
  float maxSpringOffsetTimeMs; // [esp+1C8h] [ebp-1B8h]
  float frameTimeMs; // [esp+1CCh] [ebp-1B4h]
  float ratioOffset; // [esp+1D0h] [ebp-1B0h]
  float maxSpringOffset; // [esp+1D4h] [ebp-1ACh]
  float speedRatio; // [esp+1D8h] [ebp-1A8h]
  float speedRatioTimed; // [esp+1DCh] [ebp-1A4h]
  const vehicle_info_t *info; // [esp+1E0h] [ebp-1A0h]
  DObj *obj; // [esp+1E4h] [ebp-19Ch]
  float mtx[3][3]; // [esp+1E8h] [ebp-198h] BYREF
  float viewAnglesBackup[3]; // [esp+20Ch] [ebp-174h]
  float viewVehicleInfluence; // [esp+218h] [ebp-168h]
  cg_s *cgameGlob; // [esp+21Ch] [ebp-164h]
  float bodyMtx[4][3]; // [esp+220h] [ebp-160h] BYREF
  int weapon; // [esp+250h] [ebp-130h]
  float desiredCamPos[3]; // [esp+254h] [ebp-12Ch]
  centity_s *vehicle; // [esp+260h] [ebp-120h]
  float viewAxis[3][3]; // [esp+264h] [ebp-11Ch] BYREF
  float temp[3]; // [esp+288h] [ebp-F8h] BYREF
  float identQuat[4]; // [esp+294h] [ebp-ECh] BYREF
  float offsetVec[3]; // [esp+2A4h] [ebp-DCh] BYREF
  float deltaAxis[3][3]; // [esp+2B0h] [ebp-D0h] BYREF
  bool viewFollowVehicle; // [esp+2D7h] [ebp-A9h]
  float deltaQuat[4]; // [esp+2D8h] [ebp-A8h] BYREF
  playerState_s *ps; // [esp+2E8h] [ebp-98h]
  float playerMtx[4][3]; // [esp+2ECh] [ebp-94h] BYREF
  float tagMtx[4][3]; // [esp+31Ch] [ebp-64h] BYREF
  const WeaponDef *weapDef; // [esp+34Ch] [ebp-34h]
  float barrelMtx[4][3]; // [esp+350h] [ebp-30h] BYREF

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  ps = &cgameGlob->predictedPlayerState;
  vehicle = CG_GetEntity(localClientNum, cgameGlob->predictedPlayerState.viewlocked_entNum);
  obj = Com_GetClientDObj(vehicle->nextState.number, localClientNum);
  weapDef = 0;
  info = CG_GetVehicleInfo(vehicle->nextState.un2.vehicleState.vehicleInfoIndex);
  if ( obj
    && CG_DObjGetWorldTagMatrix(&vehicle->pose, obj, scr_const.tag_body, bodyMtx, bodyMtx[3])
    && (ps->vehicleType != 2
     || CG_DObjGetWorldTagMatrix(&vehicle->pose, obj, scr_const.tag_barrel, barrelMtx, barrelMtx[3]))
    && CG_DObjGetWorldTagMatrix(&vehicle->pose, obj, scr_const.tag_player, playerMtx, playerMtx[3]) )
  {
    *(_QWORD *)&tagMtx[0][0] = *(_QWORD *)&bodyMtx[0][0];
    tagMtx[0][2] = bodyMtx[0][2];
    tagMtx[1][0] = bodyMtx[1][0];
    tagMtx[1][1] = bodyMtx[1][1];
    tagMtx[1][2] = bodyMtx[1][2];
    tagMtx[2][0] = bodyMtx[2][0];
    tagMtx[2][1] = bodyMtx[2][1];
    tagMtx[2][2] = bodyMtx[2][2];
    if ( ps->vehicleType == 2 )
    {
      tagMtx[3][0] = barrelMtx[3][0];
      tagMtx[3][1] = barrelMtx[3][1];
      tagMtx[3][2] = barrelMtx[3][2];
    }
    else
    {
      tagMtx[3][0] = playerMtx[3][0];
      tagMtx[3][1] = playerMtx[3][1];
      tagMtx[3][2] = playerMtx[3][2];
    }
    if ( (ps->eFlags & 0x4000) != 0 && ps->viewlocked_entNum != 1023 )
    {
      if ( cgameGlob->vehicleInitView )
      {
        MatrixTranspose(tagMtx, cgameGlob->prevVehicleInvAxis);
        cgameGlob->vehicleInitView = 0;
        cgameGlob->cameraData.lastViewInputTime = 0;
      }
      MatrixMultiply(cgameGlob->prevVehicleInvAxis, tagMtx, deltaAxis);
      MatrixTranspose(tagMtx, cgameGlob->prevVehicleInvAxis);
      weapon = CG_GetPlayerWeapon(ps, localClientNum);
      weapDef = BG_GetWeaponDef(weapon);
      *(_QWORD *)desiredCamPos = *(_QWORD *)&tagMtx[3][0];
      desiredCamPos[2] = tagMtx[3][2];
      if ( (ps->eFlags2 & 0x10000000) != 0 )
      {
        desiredCamPos[0] = (float)(60.0 * playerMtx[2][0]) + desiredCamPos[0];
        desiredCamPos[1] = (float)(60.0 * playerMtx[2][1]) + desiredCamPos[1];
        desiredCamPos[2] = (float)(60.0 * playerMtx[2][2]) + desiredCamPos[2];
      }
      else
      {
        viewHeightCurrent = ps->viewHeightCurrent;
        desiredCamPos[0] = (float)(viewHeightCurrent * playerMtx[2][0]) + desiredCamPos[0];
        desiredCamPos[1] = (float)(viewHeightCurrent * playerMtx[2][1]) + desiredCamPos[1];
        desiredCamPos[2] = (float)(viewHeightCurrent * playerMtx[2][2]) + desiredCamPos[2];
      }
      if ( cgameGlob->inKillCam
        && ps->vehicleType == 6
        && info->isDrivable
        && info->numberOfSeats == 1
        && !info->numberOfGunners )
      {
        ps->otherFlags &= ~2u;
      }
      if ( (ps->otherFlags & 2) != 0 && cgameGlob->renderingThirdPerson == TP_FOR_MODEL )
      {
        viewAnglesBackup[0] = ps->viewangles[0];
        viewAnglesBackup[1] = ps->viewangles[1];
        viewAnglesBackup[2] = ps->viewangles[2];
        viewangles = ps->viewangles;
        ps->viewangles[0] = *(float *)&FLOAT_0_0;
        viewangles[1] = *(float *)&FLOAT_0_0;
        viewangles[2] = *(float *)&FLOAT_0_0;
      }
      memset(offsetVec, 0, sizeof(offsetVec));
      if ( weapDef && weapon && ((ps->otherFlags & 2) == 0 || cgameGlob->renderingThirdPerson != TP_FOR_MODEL) )
        BG_CalcVehicleTurretWeaponPosOffset(ps->fWeaponPosFrac, weapDef, offsetVec);
      if ( cg_thirdPerson->current.enabled && (ps->otherFlags & 2) == 0 )
      {
        if ( offsetVec[0] > -300.0 )
          offsetVec[0] = FLOAT_N300_0;
        if ( cg_thirdPersonRange->current.value > COERCE_FLOAT(LODWORD(offsetVec[0]) ^ _mask__NegFloat_) )
          LODWORD(offsetVec[0]) = cg_thirdPersonRange->current.integer ^ _mask__NegFloat_;
      }
      if ( info->thirdPersonCameraSpringDistance > 0.0
        && info->thirdPersonCameraSpringTime > 0.0
        && NitrousVehicle::get_max_speed(vehicle->nitrousVeh, 0) > 0.0
        && cgameGlob->frametime > 0 )
      {
        maxSpringOffset = info->thirdPersonCameraSpringDistance;
        maxSpringOffsetTimeMs = maxSpringOffset / (float)(info->thirdPersonCameraSpringTime * 1000.0);
        frameTimeMs = (float)cgameGlob->frametime * 0.001;
        m_forward_vel = vehicle->nitrousVeh->m_forward_vel;
        v28 = m_forward_vel / NitrousVehicle::get_max_speed(vehicle->nitrousVeh, 0);
        if ( 1.0 <= 0.0
          && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.h", 730, 0, "%s", "min < max") )
        {
          __debugbreak();
        }
        if ( v28 >= 0.0 )
        {
          if ( v28 <= 1.0 )
            speedRatio = v28;
          else
            speedRatio = FLOAT_1_0;
        }
        else
        {
          speedRatio = *(float *)&FLOAT_0_0;
        }
        v27 = (float)(maxSpringOffsetTimeMs / frameTimeMs) * speedRatio;
        if ( 1.0 <= 0.1
          && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.h", 730, 0, "%s", "min < max") )
        {
          __debugbreak();
        }
        if ( v27 >= 0.1 )
        {
          if ( v27 <= 1.0 )
            speedRatioTimed = (float)(maxSpringOffsetTimeMs / frameTimeMs) * speedRatio;
          else
            speedRatioTimed = FLOAT_1_0;
        }
        else
        {
          speedRatioTimed = FLOAT_0_1;
        }
        v26 = maxSpringOffset * speedRatio;
        if ( maxSpringOffset <= 0.0
          && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.h", 730, 0, "%s", "min < max") )
        {
          __debugbreak();
        }
        if ( v26 >= 0.0 )
        {
          if ( v26 <= maxSpringOffset )
            ratioOffset = maxSpringOffset * speedRatio;
          else
            ratioOffset = maxSpringOffset;
        }
        else
        {
          ratioOffset = *(float *)&FLOAT_0_0;
        }
        cgameGlob->cameraData.lastSpringOffset = (float)((float)(1.0 - speedRatioTimed)
                                                       * cgameGlob->cameraData.lastSpringOffset)
                                               + (float)(ratioOffset * speedRatioTimed);
        offsetVec[0] = offsetVec[0] - cgameGlob->cameraData.lastSpringOffset;
      }
      if ( ps->vehicleType == 2 )
      {
        *(_QWORD *)&viewAxis[0][0] = *(_QWORD *)&barrelMtx[0][0];
        viewAxis[0][2] = barrelMtx[0][2];
        viewAxis[1][0] = barrelMtx[1][0];
        viewAxis[1][1] = barrelMtx[1][1];
        viewAxis[1][2] = barrelMtx[1][2];
        viewAxis[2][0] = barrelMtx[2][0];
        viewAxis[2][1] = barrelMtx[2][1];
        viewAxis[2][2] = barrelMtx[2][2];
      }
      else
      {
        AnglesToAxis(ps->viewangles, viewAxis);
      }
      if ( COERCE_FLOAT(LODWORD(offsetVec[0]) & _mask__AbsFloat_) <= 200.0 )
      {
        desiredCamPos[0] = (float)(offsetVec[0] * playerMtx[0][0]) + desiredCamPos[0];
        desiredCamPos[1] = (float)(offsetVec[0] * playerMtx[0][1]) + desiredCamPos[1];
        desiredCamPos[2] = (float)(offsetVec[0] * playerMtx[0][2]) + desiredCamPos[2];
      }
      else
      {
        desiredCamPos[0] = (float)(offsetVec[0] * viewAxis[0][0]) + desiredCamPos[0];
        desiredCamPos[1] = (float)(offsetVec[0] * viewAxis[0][1]) + desiredCamPos[1];
        desiredCamPos[2] = (float)(offsetVec[0] * viewAxis[0][2]) + desiredCamPos[2];
      }
      desiredCamPos[0] = (float)(offsetVec[1] * playerMtx[1][0]) + desiredCamPos[0];
      desiredCamPos[1] = (float)(offsetVec[1] * playerMtx[1][1]) + desiredCamPos[1];
      desiredCamPos[2] = (float)(offsetVec[1] * playerMtx[1][2]) + desiredCamPos[2];
      desiredCamPos[0] = (float)(offsetVec[2] * playerMtx[2][0]) + desiredCamPos[0];
      desiredCamPos[1] = (float)(offsetVec[2] * playerMtx[2][1]) + desiredCamPos[1];
      desiredCamPos[2] = (float)(offsetVec[2] * playerMtx[2][2]) + desiredCamPos[2];
      v6 = (!info->type || info->type == 4 || info->type == 1) && (info->cameraMode == 1 || info->cameraMode == 5);
      viewFollowVehicle = v6;
      if ( info->cameraMode == 4 )
        viewFollowVehicle = 1;
      if ( viewFollowVehicle )
      {
        if ( cgameGlob->vehicleInitView )
        {
          v24 = swayViewAngles_0[localClientNum];
          angles = vehicle->pose.angles;
          *v24 = vehicle->pose.angles[0];
          v24[1] = angles[1];
          v24[2] = angles[2];
        }
        cgameGlob->vehicleInitView = 0;
        swayScale = FLOAT_4_0;
        if ( cgameGlob->renderingThirdPerson == TP_FOR_MODEL )
        {
          if ( ps->vehicleType == 4 )
            swayScale = FLOAT_60_0;
          else
            swayScale = FLOAT_10_0;
        }
        CG_CalculateGunnerOffset_Sway(
          vehicle->pose.angles,
          weapon,
          swayViewAngles_0[localClientNum],
          swayOffset_0[localClientNum],
          swayAngles_0[localClientNum],
          swayScale,
          cgameGlob->frametime);
        v23 = swayOffset_0[localClientNum][0];
        desiredCamPos[0] = (float)(v23 * viewAxis[0][0]) + desiredCamPos[0];
        desiredCamPos[1] = (float)(v23 * viewAxis[0][1]) + desiredCamPos[1];
        desiredCamPos[2] = (float)(v23 * viewAxis[0][2]) + desiredCamPos[2];
        v22 = *(float *)&dword_E86AF0[3 * localClientNum];
        desiredCamPos[0] = (float)(v22 * playerMtx[1][0]) + desiredCamPos[0];
        desiredCamPos[1] = (float)(v22 * playerMtx[1][1]) + desiredCamPos[1];
        desiredCamPos[2] = (float)(v22 * playerMtx[1][2]) + desiredCamPos[2];
        LODWORD(v21) = dword_E86AF4[3 * localClientNum] ^ _mask__NegFloat_;
        desiredCamPos[0] = (float)(v21 * playerMtx[2][0]) + desiredCamPos[0];
        desiredCamPos[1] = (float)(v21 * playerMtx[2][1]) + desiredCamPos[1];
        desiredCamPos[2] = (float)(v21 * playerMtx[2][2]) + desiredCamPos[2];
      }
      vieworg = cgameGlob->refdef.vieworg;
      cgameGlob->refdef.vieworg[0] = desiredCamPos[0];
      vieworg[1] = desiredCamPos[1];
      vieworg[2] = desiredCamPos[2];
      if ( offsetVec[0] != 0.0 && ps->fWeaponPosFrac < 0.5 )
      {
        start[0] = (float)(COERCE_FLOAT(LODWORD(offsetVec[0]) ^ _mask__NegFloat_) * viewAxis[0][0]) + desiredCamPos[0];
        start[1] = (float)(COERCE_FLOAT(LODWORD(offsetVec[0]) ^ _mask__NegFloat_) * viewAxis[0][1]) + desiredCamPos[1];
        start[2] = (float)(COERCE_FLOAT(LODWORD(offsetVec[0]) ^ _mask__NegFloat_) * viewAxis[0][2]) + desiredCamPos[2];
        ThirdPersonViewTrace(
          cgameGlob,
          start,
          cgameGlob->refdef.vieworg,
          (int)&loc_800811,
          cgameGlob->refdef.vieworg,
          1,
          0);
      }
      memset(identQuat, 0, 12);
      identQuat[3] = FLOAT_1_0;
      AxisToQuat(deltaAxis, deltaQuat);
      viewVehicleInfluence = info->viewInfluence;
      if ( viewFollowVehicle )
        viewVehicleInfluence = FLOAT_1_0;
      if ( info->type == 6 )
      {
        for ( i = 0; i < 2; ++i )
          cmd.button_bits.array[i] = 0;
        cmdNumber = CL_GetCurrentCmdNumber(localClientNum);
        if ( CL_GetUserCmd(localClientNum, cmdNumber, &cmd) )
        {
          angleDiff = AngleDelta(ps->viewangles[1], vehicle->pose.angles[1]);
          if ( maxDiff > COERCE_FLOAT(LODWORD(angleDiff) & _mask__AbsFloat_) && cmd.yawmove < 10 && cmd.yawmove > -10 )
          {
            if ( (float)(viewVehicleInfluence
                       - (float)((float)(1.0 - (float)(COERCE_FLOAT(LODWORD(angleDiff) & _mask__AbsFloat_) / maxDiff))
                               * maxInfluence)) < 0.0 )
              v5 = (float)(1.0 - (float)(COERCE_FLOAT(LODWORD(angleDiff) & _mask__AbsFloat_) / maxDiff)) * maxInfluence;
            else
              v5 = viewVehicleInfluence;
            viewVehicleInfluence = v5;
          }
        }
        cgameGlob->refdef.noLodCullOut = 1;
      }
      QuatLerp(identQuat, deltaQuat, viewVehicleInfluence, deltaQuat);
      QuatToAxis(deltaQuat, deltaAxis);
      MatrixMultiply(viewAxis, deltaAxis, mtx);
      AxisToAngles(mtx, temp);
      AnglesToAxis(ps->viewangles, viewAxis);
      MatrixMultiply(viewAxis, deltaAxis, mtx);
      AxisToAngles(mtx, temp);
      if ( viewFollowVehicle )
      {
        lerpViewBackToForward = 0;
        for ( j = 0; j < 2; ++j )
          ucmd.button_bits.array[j] = 0;
        CurrentCmdNumber = CL_GetCurrentCmdNumber(localClientNum);
        if ( CL_GetUserCmd(localClientNum, CurrentCmdNumber, &ucmd) )
        {
          if ( ucmd.pitchmove || ucmd.yawmove || (ps->eFlags & 0x40) != 0 )
            cgameGlob->cameraData.lastViewInputTime = ucmd.serverTime;
          if ( info->autoRecenterOnAccel
            && cgameGlob->time - cgameGlob->cameraData.lastViewInputTime > 370
            && vehicle
            && vehicle->nitrousVeh
            && vehicle->nitrousVeh->m_throttle > 0.5 )
          {
            lerpViewBackToForward = 1;
          }
          if ( bitarray<51>::testBit(&ucmd.button_bits, 2u) )
            cgameGlob->cameraData.lastViewInputTime = -vehRecenterDelay->current.integer;
          if ( cgameGlob->time - cgameGlob->cameraData.lastViewInputTime > vehRecenterDelay->current.integer )
            lerpViewBackToForward = 1;
        }
        if ( lerpViewBackToForward )
        {
          frametime = (float)cgameGlob->frametime;
          v15 = AngleNormalize180(vehicle->pose.angles[1] - temp[1]) * lerpRate * frametime * 0.001;
          LODWORD(v16) = LODWORD(maxLerpRate) ^ _mask__NegFloat_;
          v17 = maxLerpRate;
          if ( maxLerpRate <= COERCE_FLOAT(LODWORD(maxLerpRate) ^ _mask__NegFloat_)
            && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.h", 730, 0, "%s", "min < max") )
          {
            __debugbreak();
          }
          if ( v16 <= v15 )
          {
            if ( v15 <= v17 )
              v14 = v15;
            else
              v14 = v17;
          }
          else
          {
            v14 = v16;
          }
          temp[1] = temp[1] + v14;
          v3 = (float)cgameGlob->frametime;
          v11 = AngleNormalize180(vehicle->pose.angles[0] - temp[0]) * lerpRate * v3 * 0.001;
          LODWORD(v12) = LODWORD(maxLerpRate) ^ _mask__NegFloat_;
          v13 = maxLerpRate;
          if ( maxLerpRate <= COERCE_FLOAT(LODWORD(maxLerpRate) ^ _mask__NegFloat_)
            && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.h", 730, 0, "%s", "min < max") )
          {
            __debugbreak();
          }
          if ( v12 <= v11 )
          {
            if ( v11 <= v13 )
              v10 = v11;
            else
              v10 = v13;
          }
          else
          {
            v10 = v12;
          }
          temp[0] = temp[0] + v10;
        }
      }
      cgameGlob->refdefViewAngles[0] = temp[0];
      cgameGlob->refdefViewAngles[1] = temp[1];
      fWeaponPosFrac = ps->fWeaponPosFrac;
      v1 = AngleNormalize180(temp[2] - 0.0);
      cgameGlob->refdefViewAngles[2] = v1 * fWeaponPosFrac + 0.0;
      if ( info->type != 6 && cgameGlob->renderingThirdPerson == TP_OFF )
        ps->viewangles[0] = temp[0];
      if ( ps->vehicleType == 6 && (ps->vehicleType != 2 || !vehControlMode || !vehControlMode->current.integer) )
        ps->viewangles[1] = temp[1];
      if ( ps->vehicleType != 2 || !vehicle->nextState.weapon )
        ps->viewangles[1] = temp[1];
      if ( viewFollowVehicle || info->type == 6 )
      {
        ps->viewangles[2] = *(float *)&FLOAT_0_0;
        temp[0] = ps->viewangles[0] - ps->delta_angles[0];
        temp[1] = ps->viewangles[1] - ps->delta_angles[1];
        temp[2] = ps->viewangles[2] - ps->delta_angles[2];
        CL_SetViewAngles(localClientNum, temp);
        Com_Printf(0, "CL_SetViewAngles() - CHARLIE - (%3.0f, %3.0f, %3.0f)\n", temp[0], temp[1], temp[2]);
      }
      if ( info->type == 6 )
      {
        MatrixMultiply(viewAxis, cgameGlob->prevVehicleInvAxis, mtx);
        AxisToAngles(mtx, temp);
        LODWORD(ps->viewangles[2]) = LODWORD(temp[2]) ^ _mask__NegFloat_;
      }
      else if ( info->type == 1 )
      {
        MatrixMultiply(viewAxis, cgameGlob->prevVehicleInvAxis, mtx);
        AxisToAngles(mtx, temp);
        LODWORD(v8) = LODWORD(rollFrac) ^ _mask__NegFloat_;
        v2 = AngleNormalize180(temp[2] - 0.0);
        ps->viewangles[2] = v2 * v8 + 0.0;
      }
      else
      {
        ps->viewangles[2] = *(float *)&FLOAT_0_0;
      }
      if ( (ps->otherFlags & 2) != 0 && cgameGlob->renderingThirdPerson == TP_FOR_MODEL )
      {
        v7 = ps->viewangles;
        ps->viewangles[0] = viewAnglesBackup[0];
        v7[1] = viewAnglesBackup[1];
        v7[2] = viewAnglesBackup[2];
      }
    }
    else if ( ps->vehicleType != 6 )
    {
      MatrixTranspose(tagMtx, cgameGlob->prevVehicleInvAxis);
    }
  }
}

void __cdecl CG_Calc3rdPersonSpringDamp(
        float *currPos,
        float *trgPos,
        float *prevTrgPos,
        float deltaTime,
        float springConst,
        float dampConst,
        float springLen,
        float *newPos)
{
  float velocity; // [esp+20h] [ebp-20h]
  float velocity_4; // [esp+24h] [ebp-1Ch]
  float velocity_8; // [esp+28h] [ebp-18h]
  float forceMag; // [esp+2Ch] [ebp-14h]
  float disp[3]; // [esp+30h] [ebp-10h] BYREF
  float dispLength; // [esp+3Ch] [ebp-4h]

  disp[0] = *currPos - *trgPos;
  disp[1] = currPos[1] - trgPos[1];
  disp[2] = currPos[2] - trgPos[2];
  velocity = deltaTime * (float)(*prevTrgPos - *trgPos);
  velocity_4 = deltaTime * (float)(prevTrgPos[1] - trgPos[1]);
  velocity_8 = deltaTime * (float)(prevTrgPos[2] - trgPos[2]);
  dispLength = Abs(disp);
  forceMag = (float)((float)(springLen - dispLength) * springConst)
           + (float)((float)((float)((float)((float)(disp[0] * velocity) + (float)(disp[1] * velocity_4))
                                   + (float)(disp[2] * velocity_8))
                           / dispLength)
                   * dampConst);
  Vec3Normalize(disp);
  *newPos = (float)((float)(forceMag * deltaTime) * disp[0]) + *currPos;
  newPos[1] = (float)((float)(forceMag * deltaTime) * disp[1]) + currPos[1];
  newPos[2] = (float)((float)(forceMag * deltaTime) * disp[2]) + currPos[2];
  if ( ((*(unsigned int *)newPos & 0x7F800000) == 0x7F800000
     || ((unsigned int)newPos[1] & 0x7F800000) == 0x7F800000
     || ((unsigned int)newPos[2] & 0x7F800000) == 0x7F800000)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_camera.cpp",
          2000,
          0,
          "%s",
          "!IS_NAN((newPos)[0]) && !IS_NAN((newPos)[1]) && !IS_NAN((newPos)[2])") )
  {
    __debugbreak();
  }
}

double __cdecl GetPitchOffsetRelativeToADirection(float *angles, float *direction)
{
  double v2; // xmm0_8
  long double v4; // [esp+0h] [ebp-54h]
  float directionXY[3]; // [esp+14h] [ebp-40h] BYREF
  float up[3]; // [esp+24h] [ebp-30h]
  float axis[3][3]; // [esp+30h] [ebp-24h] BYREF

  AnglesToAxis(angles, axis);
  *(_QWORD *)up = *(_QWORD *)&axis[2][0];
  up[2] = axis[2][2];
  directionXY[0] = *direction;
  directionXY[1] = direction[1];
  directionXY[2] = *(float *)&FLOAT_0_0;
  Vec2Normalize(directionXY);
  *(float *)&v4 = (float)((float)(up[0] * directionXY[0]) + (float)(up[1] * directionXY[1]))
                + (float)(up[2] * directionXY[2]);
  v2 = *(float *)&v4;
  __libm_sse2_acos(v4);
  *(float *)&v2 = v2;
  return 90.0 - (float)(*(float *)&v2 * 57.295776);
}

void __cdecl CG_Calc3rdPersonVehicleViewValues(int localClientNum)
{
  double v1; // st7
  float v2; // [esp+18h] [ebp-268h]
  float v3; // [esp+1Ch] [ebp-264h]
  float frametime; // [esp+20h] [ebp-260h]
  float v5; // [esp+24h] [ebp-25Ch]
  float *viewangles; // [esp+30h] [ebp-250h]
  float v7; // [esp+44h] [ebp-23Ch]
  float v8; // [esp+48h] [ebp-238h]
  float *vieworg; // [esp+5Ch] [ebp-224h]
  float v10; // [esp+64h] [ebp-21Ch]
  float v11; // [esp+6Ch] [ebp-214h]
  float v12; // [esp+70h] [ebp-210h]
  float m_forward_vel; // [esp+74h] [ebp-20Ch]
  float v14; // [esp+7Ch] [ebp-204h]
  float v15; // [esp+80h] [ebp-200h]
  float v16; // [esp+90h] [ebp-1F0h]
  float v17; // [esp+94h] [ebp-1ECh]
  float v18; // [esp+A0h] [ebp-1E0h]
  float v19; // [esp+A4h] [ebp-1DCh]
  float v20; // [esp+A8h] [ebp-1D8h]
  float *refdefViewAngles; // [esp+FCh] [ebp-184h]
  float *lastViewAngles; // [esp+100h] [ebp-180h]
  float *v23; // [esp+104h] [ebp-17Ch]
  Camera *p_cameraData; // [esp+108h] [ebp-178h]
  int i; // [esp+118h] [ebp-168h]
  float newCamPos[3]; // [esp+11Ch] [ebp-164h] BYREF
  float maxSpringOffsetTimeMs; // [esp+128h] [ebp-158h]
  float frameTimeMs; // [esp+12Ch] [ebp-154h]
  float ratioOffset; // [esp+130h] [ebp-150h]
  float maxSpringOffset; // [esp+134h] [ebp-14Ch]
  float speedRatio; // [esp+138h] [ebp-148h]
  float speedRatioTimed; // [esp+13Ch] [ebp-144h]
  float maxLerp; // [esp+140h] [ebp-140h]
  float maxLerpRate; // [esp+144h] [ebp-13Ch]
  float pitchAdjust; // [esp+148h] [ebp-138h]
  float lerpRate; // [esp+14Ch] [ebp-134h]
  float axis[3][3]; // [esp+150h] [ebp-130h] BYREF
  float tagMtx[4][3]; // [esp+174h] [ebp-10Ch] BYREF
  float lookAtPos[3]; // [esp+1A4h] [ebp-DCh] BYREF
  float camDefaultDist; // [esp+1B0h] [ebp-D0h]
  const vehicle_info_t *info; // [esp+1B4h] [ebp-CCh]
  bool lerpViewBackToForward; // [esp+1BBh] [ebp-C5h]
  float minpitch; // [esp+1BCh] [ebp-C4h]
  float vehiclePitchOffset; // [esp+1C0h] [ebp-C0h]
  cg_s *cgameGlob; // [esp+1C4h] [ebp-BCh]
  float relative_up[3]; // [esp+1C8h] [ebp-B8h] BYREF
  float desiredCamPos[3]; // [esp+1D4h] [ebp-ACh] BYREF
  float quat[4]; // [esp+1E0h] [ebp-A0h] BYREF
  centity_s *vehicle; // [esp+1F0h] [ebp-90h]
  float temp[3]; // [esp+1F4h] [ebp-8Ch] BYREF
  float camDefaultLookHeight; // [esp+200h] [ebp-80h]
  float maxpitch; // [esp+204h] [ebp-7Ch]
  float previousCamPos[3]; // [esp+208h] [ebp-78h]
  float up[3]; // [esp+214h] [ebp-6Ch] BYREF
  float newLookDir[3]; // [esp+220h] [ebp-60h] BYREF
  playerState_s *ps; // [esp+22Ch] [ebp-54h]
  bool setPlayerAngles; // [esp+233h] [ebp-4Dh]
  usercmd_s cmd; // [esp+234h] [ebp-4Ch] BYREF
  int cmdNumber; // [esp+26Ch] [ebp-14h]
  float newLookAngles[3]; // [esp+270h] [ebp-10h] BYREF
  float normalizedPitch; // [esp+27Ch] [ebp-4h]

  for ( i = 0; i < 2; ++i )
    cmd.button_bits.array[i] = 0;
  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  ps = &cgameGlob->predictedPlayerState;
  vehicle = CG_GetEntity(localClientNum, cgameGlob->predictedPlayerState.viewlocked_entNum);
  info = CG_GetVehicleInfo(vehicle->nextState.un2.vehicleState.vehicleInfoIndex);
  if ( ((*((unsigned int *)vehicle + 201) >> 1) & 1) != 0 && (vehicle->nitrousVeh || ps->vehicleType == 6) )
  {
    if ( cgameGlob->vehicleInitView )
    {
      cgameGlob->cameraData.lastViewInputTime = cgameGlob->time;
      cgameGlob->cameraData.lastViewTraceFraction = FLOAT_1_0;
    }
    camDefaultDist = info->thirdPersonCameraRange;
    camDefaultLookHeight = (float)(info->thirdPersonCameraHeight[1] - info->thirdPersonCameraHeight[0])
                         + info->thirdPersonCameraHeight[0];
    lerpViewBackToForward = 0;
    setPlayerAngles = 0;
    cmdNumber = CL_GetCurrentCmdNumber(localClientNum);
    if ( CL_GetUserCmd(localClientNum, cmdNumber, &cmd) )
    {
      if ( COERCE_FLOAT(COERCE_UNSIGNED_INT((float)cmd.pitchmove) & _mask__AbsFloat_) > 10.0
        || COERCE_FLOAT(COERCE_UNSIGNED_INT((float)cmd.yawmove) & _mask__AbsFloat_) > 10.0 )
      {
        cgameGlob->cameraData.lastViewInputTime = cmd.serverTime;
      }
    }
    else
    {
      setPlayerAngles = 1;
    }
    previousCamPos[0] = cgameGlob->cameraData.lastViewOrg[0];
    previousCamPos[1] = cgameGlob->cameraData.lastViewOrg[1];
    previousCamPos[2] = cgameGlob->cameraData.lastViewOrg[2];
    up[0] = *(float *)&FLOAT_0_0;
    up[1] = *(float *)&FLOAT_0_0;
    up[2] = camDefaultLookHeight;
    AnglesToQuat(vehicle->pose.angles, quat);
    RotatePoint(up, quat, relative_up);
    lookAtPos[0] = vehicle->pose.origin[0] + relative_up[0];
    lookAtPos[1] = vehicle->pose.origin[1] + relative_up[1];
    lookAtPos[2] = vehicle->pose.origin[2] + relative_up[2];
    if ( ps->vehicleType == 6 )
    {
      AnglesToAxis(vehicle->pose.angles, tagMtx);
      lookAtPos[0] = (float)(lookAtOffsetInFront * tagMtx[0][0]) + lookAtPos[0];
      lookAtPos[1] = (float)(lookAtOffsetInFront * tagMtx[0][1]) + lookAtPos[1];
      lookAtPos[2] = (float)(lookAtOffsetInFront * tagMtx[0][2]) + lookAtPos[2];
      lookAtPos[0] = (float)(lookAtOffsetToSide * tagMtx[1][0]) + lookAtPos[0];
      lookAtPos[1] = (float)(lookAtOffsetToSide * tagMtx[1][1]) + lookAtPos[1];
      lookAtPos[2] = (float)(lookAtOffsetToSide * tagMtx[1][2]) + lookAtPos[2];
    }
    if ( setPlayerAngles )
    {
      newLookDir[0] = lookAtPos[0] - previousCamPos[0];
      newLookDir[1] = lookAtPos[1] - previousCamPos[1];
      newLookDir[2] = lookAtPos[2] - previousCamPos[2];
      if ( cgameGlob->vehicleInitView )
        newLookDir[2] = *(float *)&FLOAT_0_0;
      Vec3Normalize(newLookDir);
    }
    else
    {
      AnglesToAxis(ps->viewangles, axis);
      *(_QWORD *)newLookDir = *(_QWORD *)&axis[0][0];
      newLookDir[2] = axis[0][2];
    }
    vectoangles(newLookDir, newLookAngles);
    LODWORD(minpitch) = LODWORD(info->thirdPersonCameraMinPitchClamp) ^ _mask__NegFloat_;
    maxpitch = info->thirdPersonCameraMaxPitchClamp;
    vehiclePitchOffset = GetPitchOffsetRelativeToADirection(vehicle->pose.angles, newLookDir);
    normalizedPitch = AngleNormalize180(newLookAngles[0]);
    if ( COERCE_FLOAT(COERCE_UNSIGNED_INT(maxpitch - minpitch) & _mask__AbsFloat_) <= 0.001 )
    {
      newLookAngles[0] = minpitch + vehiclePitchOffset;
    }
    else
    {
      v19 = minpitch + vehiclePitchOffset;
      v20 = maxpitch + vehiclePitchOffset;
      if ( (float)(maxpitch + vehiclePitchOffset) <= (float)(minpitch + vehiclePitchOffset)
        && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.h", 730, 0, "%s", "min < max") )
      {
        __debugbreak();
      }
      if ( v19 <= normalizedPitch )
      {
        if ( normalizedPitch <= v20 )
          v18 = normalizedPitch;
        else
          v18 = v20;
      }
      else
      {
        v18 = v19;
      }
      newLookAngles[0] = v18;
    }
    if ( normalizedPitch != newLookAngles[0] )
      setPlayerAngles = 1;
    if ( lerpViewBackToForward )
    {
      lerpRate = info->cameraAutoCenterLerpRate;
      maxLerpRate = info->cameraAutoCenterMaxLerpRate;
      maxLerp = (float)((float)cgameGlob->frametime * maxLerpRate) * 0.001;
      if ( (float)(maxLerp - 0.000099999997) < 0.0 )
        v5 = FLOAT_0_000099999997;
      else
        v5 = maxLerp;
      maxLerp = v5;
      if ( info->cameraMode != 2 )
      {
        frametime = (float)cgameGlob->frametime;
        v17 = AngleNormalize180(vehicle->pose.angles[1] - newLookAngles[1]) * lerpRate * frametime * 0.001;
        if ( maxLerpRate <= COERCE_FLOAT(LODWORD(maxLerpRate) ^ _mask__NegFloat_)
          && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.h", 730, 0, "%s", "min < max") )
        {
          __debugbreak();
        }
        if ( COERCE_FLOAT(LODWORD(maxLerpRate) ^ _mask__NegFloat_) <= v17 )
        {
          if ( v17 <= maxLerpRate )
            v16 = v17;
          else
            v16 = maxLerpRate;
        }
        else
        {
          LODWORD(v16) = LODWORD(maxLerpRate) ^ _mask__NegFloat_;
        }
        newLookAngles[1] = newLookAngles[1] + v16;
      }
      pitchAdjust = AngleNormalize180(
                      (float)((float)(info->thirdPersonCameraPitch[1] - info->thirdPersonCameraPitch[0])
                            * cgameGlob->cameraData.lastViewTraceFraction)
                    + info->thirdPersonCameraPitch[0]);
      v3 = (float)cgameGlob->frametime;
      v15 = AngleNormalize180(pitchAdjust - newLookAngles[0]) * lerpRate * v3 * 0.001;
      if ( maxLerpRate <= COERCE_FLOAT(LODWORD(maxLerpRate) ^ _mask__NegFloat_)
        && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.h", 730, 0, "%s", "min < max") )
      {
        __debugbreak();
      }
      if ( COERCE_FLOAT(LODWORD(maxLerpRate) ^ _mask__NegFloat_) <= v15 )
      {
        if ( v15 <= maxLerpRate )
          v14 = v15;
        else
          v14 = maxLerpRate;
      }
      else
      {
        LODWORD(v14) = LODWORD(maxLerpRate) ^ _mask__NegFloat_;
      }
      newLookAngles[0] = newLookAngles[0] + v14;
    }
    if ( !damped_spring
      && vehicle->nitrousVeh
      && (info->thirdPersonCameraSpringDistance <= -0.001 || info->thirdPersonCameraSpringDistance >= 0.001)
      && info->thirdPersonCameraSpringTime > 0.0
      && NitrousVehicle::get_max_speed(vehicle->nitrousVeh, 0) > 0.0
      && cgameGlob->frametime > 0 )
    {
      LODWORD(maxSpringOffset) = LODWORD(info->thirdPersonCameraSpringDistance) & _mask__AbsFloat_;
      maxSpringOffsetTimeMs = maxSpringOffset / (float)(info->thirdPersonCameraSpringTime * 1000.0);
      frameTimeMs = (float)cgameGlob->frametime * 0.001;
      m_forward_vel = vehicle->nitrousVeh->m_forward_vel;
      v12 = m_forward_vel / NitrousVehicle::get_max_speed(vehicle->nitrousVeh, 0);
      if ( 1.0 <= 0.0
        && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.h", 730, 0, "%s", "min < max") )
      {
        __debugbreak();
      }
      if ( v12 >= 0.0 )
      {
        if ( v12 <= 1.0 )
          speedRatio = v12;
        else
          speedRatio = FLOAT_1_0;
      }
      else
      {
        speedRatio = *(float *)&FLOAT_0_0;
      }
      v11 = (float)(maxSpringOffsetTimeMs / frameTimeMs) * speedRatio;
      if ( 1.0 <= 0.1
        && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.h", 730, 0, "%s", "min < max") )
      {
        __debugbreak();
      }
      if ( v11 >= 0.1 )
      {
        if ( v11 <= 1.0 )
          speedRatioTimed = (float)(maxSpringOffsetTimeMs / frameTimeMs) * speedRatio;
        else
          speedRatioTimed = FLOAT_1_0;
      }
      else
      {
        speedRatioTimed = FLOAT_0_1;
      }
      v10 = info->thirdPersonCameraSpringDistance * speedRatio;
      if ( maxSpringOffset <= COERCE_FLOAT(LODWORD(maxSpringOffset) ^ _mask__NegFloat_)
        && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.h", 730, 0, "%s", "min < max") )
      {
        __debugbreak();
      }
      if ( COERCE_FLOAT(LODWORD(maxSpringOffset) ^ _mask__NegFloat_) <= v10 )
      {
        if ( v10 <= maxSpringOffset )
          ratioOffset = v10;
        else
          ratioOffset = maxSpringOffset;
      }
      else
      {
        LODWORD(ratioOffset) = LODWORD(maxSpringOffset) ^ _mask__NegFloat_;
      }
      cgameGlob->cameraData.lastSpringOffset = (float)((float)(1.0 - speedRatioTimed)
                                                     * cgameGlob->cameraData.lastSpringOffset)
                                             + (float)(ratioOffset * speedRatioTimed);
      camDefaultDist = camDefaultDist + cgameGlob->cameraData.lastSpringOffset;
    }
    AngleVectors(newLookAngles, newLookDir, 0, 0);
    desiredCamPos[0] = (float)(COERCE_FLOAT(LODWORD(camDefaultDist) ^ _mask__NegFloat_) * newLookDir[0]) + lookAtPos[0];
    desiredCamPos[1] = (float)(COERCE_FLOAT(LODWORD(camDefaultDist) ^ _mask__NegFloat_) * newLookDir[1]) + lookAtPos[1];
    desiredCamPos[2] = (float)(COERCE_FLOAT(LODWORD(camDefaultDist) ^ _mask__NegFloat_) * newLookDir[2]) + lookAtPos[2];
    if ( damped_spring )
    {
      CG_Calc3rdPersonSpringDamp(
        cgameGlob->cameraData.lastViewOrg,
        desiredCamPos,
        lastDesiredCamPos,
        (float)cgameGlob->frametime * 0.001,
        springConst,
        dampConst,
        springLength,
        newCamPos);
      lastDesiredCamPos[0] = desiredCamPos[0];
      dword_E86B14 = LODWORD(desiredCamPos[1]);
      dword_E86B18 = LODWORD(desiredCamPos[2]);
      desiredCamPos[0] = newCamPos[0];
      desiredCamPos[1] = newCamPos[1];
      desiredCamPos[2] = newCamPos[2];
    }
    vieworg = cgameGlob->refdef.vieworg;
    cgameGlob->refdef.vieworg[0] = desiredCamPos[0];
    vieworg[1] = desiredCamPos[1];
    vieworg[2] = desiredCamPos[2];
    if ( ps->vehicleType != 6 )
      ThirdPersonViewTrace(
        cgameGlob,
        lookAtPos,
        cgameGlob->refdef.vieworg,
        (int)&cls.recentServers[7545].hostName[6],
        cgameGlob->refdef.vieworg,
        0,
        0);
    v1 = Vec3Distance(cgameGlob->refdef.vieworg, lookAtPos);
    v7 = v1 / info->thirdPersonCameraRange;
    if ( (float)(v7 - 1.0) < 0.0 )
      v8 = v1 / info->thirdPersonCameraRange;
    else
      v8 = FLOAT_1_0;
    if ( (float)(0.0 - v7) < 0.0 )
      v2 = v8;
    else
      v2 = *(float *)&FLOAT_0_0;
    cgameGlob->cameraData.lastViewTraceFraction = v2;
    newLookDir[0] = lookAtPos[0] - cgameGlob->refdef.vieworg[0];
    newLookDir[1] = lookAtPos[1] - cgameGlob->refdef.vieworg[1];
    newLookDir[2] = lookAtPos[2] - cgameGlob->refdef.vieworg[2];
    Vec3Normalize(newLookDir);
    vectoangles(newLookDir, newLookAngles);
    cgameGlob->refdefViewAngles[0] = newLookAngles[0];
    cgameGlob->refdefViewAngles[1] = newLookAngles[1];
    if ( info->type == 6 )
      cgameGlob->refdefViewAngles[2] = vehicle->pose.angles[2];
    else
      cgameGlob->refdefViewAngles[2] = *(float *)&FLOAT_0_0;
    if ( setPlayerAngles )
    {
      viewangles = ps->viewangles;
      ps->viewangles[0] = newLookAngles[0];
      viewangles[1] = newLookAngles[1];
      viewangles[2] = newLookAngles[2];
      temp[0] = ps->viewangles[0] - ps->delta_angles[0];
      temp[1] = ps->viewangles[1] - ps->delta_angles[1];
      temp[2] = ps->viewangles[2] - ps->delta_angles[2];
      CL_SetViewAngles(localClientNum, temp);
      Com_Printf(0, "CL_SetViewAngles() - JUNGLE - (%3.0f, %3.0f, %3.0f)\n", temp[0], temp[1], temp[2]);
    }
    cgameGlob->vehicleInitView = 0;
  }
  else
  {
    v23 = cgameGlob->refdef.vieworg;
    p_cameraData = &cgameGlob->cameraData;
    cgameGlob->refdef.vieworg[0] = cgameGlob->cameraData.lastViewOrg[0];
    v23[1] = p_cameraData->lastViewOrg[1];
    v23[2] = p_cameraData->lastViewOrg[2];
    refdefViewAngles = cgameGlob->refdefViewAngles;
    lastViewAngles = cgameGlob->cameraData.lastViewAngles;
    cgameGlob->refdefViewAngles[0] = cgameGlob->cameraData.lastViewAngles[0];
    refdefViewAngles[1] = lastViewAngles[1];
    refdefViewAngles[2] = lastViewAngles[2];
  }
}

double __cdecl Vec3Distance(const float *v1, const float *v2)
{
  float dir[3]; // [esp+4h] [ebp-Ch] BYREF

  dir[0] = *v2 - *v1;
  dir[1] = v2[1] - v1[1];
  dir[2] = v2[2] - v1[2];
  return Abs(dir);
}

void __cdecl RotatePoint(const float *v, const float *q, float *out)
{
  float t4; // [esp+0h] [ebp-24h]
  float t5; // [esp+4h] [ebp-20h]
  float t6; // [esp+8h] [ebp-1Ch]
  float t8; // [esp+Ch] [ebp-18h]
  float t3; // [esp+10h] [ebp-14h]
  float t2; // [esp+14h] [ebp-10h]
  float t7; // [esp+18h] [ebp-Ch]
  float t9; // [esp+1Ch] [ebp-8h]
  float t10; // [esp+20h] [ebp-4h]

  t2 = q[3] * *q;
  t3 = q[3] * q[1];
  t4 = q[3] * q[2];
  t5 = COERCE_FLOAT(*(unsigned int *)q ^ _mask__NegFloat_) * *q;
  t6 = *q * q[1];
  t7 = *q * q[2];
  t8 = COERCE_FLOAT(*((unsigned int *)q + 1) ^ _mask__NegFloat_) * q[1];
  t9 = q[1] * q[2];
  t10 = COERCE_FLOAT(*((unsigned int *)q + 2) ^ _mask__NegFloat_) * q[2];
  *out = (float)((float)((float)((float)((float)(t8 + t10) * *v) + (float)((float)(t6 - t4) * v[1]))
                       + (float)((float)(t3 + t7) * v[2]))
               * 2.0)
       + *v;
  out[1] = (float)((float)((float)((float)((float)(t4 + t6) * *v) + (float)((float)(t5 + t10) * v[1]))
                         + (float)((float)(t9 - t2) * v[2]))
                 * 2.0)
         + v[1];
  out[2] = (float)((float)((float)((float)((float)(t7 - t3) * *v) + (float)((float)(t2 + t9) * v[1]))
                         + (float)((float)(t5 + t8) * v[2]))
                 * 2.0)
         + v[2];
}

void __cdecl CG_OffsetVehicleView(int localClientNum, CameraMode camMode)
{
  bool v2; // [esp+4h] [ebp-50h]
  float v3; // [esp+8h] [ebp-4Ch]
  float pitchOffset; // [esp+24h] [ebp-30h]
  const vehicle_info_t *info; // [esp+28h] [ebp-2Ch]
  bool skipLerp; // [esp+2Fh] [ebp-25h]
  centity_s *Entity; // [esp+30h] [ebp-24h]
  float lerpRate; // [esp+34h] [ebp-20h]
  const WeaponDef *weapDef; // [esp+3Ch] [ebp-18h]
  cg_s *cgameGlob; // [esp+44h] [ebp-10h]
  int weapon; // [esp+48h] [ebp-Ch]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  CG_GetEntity(localClientNum, cgameGlob->predictedPlayerState.viewlocked_entNum);
  v2 = Demo_IsPaused() || Demo_IsCompleted() || Demo_GetClipPausedState();
  if ( (cgameGlob->predictedPlayerState.eFlags & 0x40) != 0 )
  {
    weapon = CG_GetPlayerWeapon(&cgameGlob->predictedPlayerState, localClientNum);
    weapDef = BG_GetWeaponDef(weapon);
    if ( weapDef )
    {
      if ( !v2 )
      {
        cgameGlob->refdefViewAngles[0] = crandom() * weapDef->vertViewJitter + cgameGlob->refdefViewAngles[0];
        cgameGlob->refdefViewAngles[1] = crandom() * weapDef->horizViewJitter + cgameGlob->refdefViewAngles[1];
      }
    }
  }
  if ( camMode == CAM_VEHICLE )
  {
    Entity = CG_GetEntity(localClientNum, cgameGlob->predictedPlayerState.viewlocked_entNum);
    info = CG_GetVehicleInfo(Entity->nextState.un2.vehicleState.vehicleInfoIndex);
    lerpRate = FLOAT_0_75;
    skipLerp = 0;
    if ( Demo_IsPlaying() )
    {
      if ( prevTimes[localClientNum] > cgameGlob->time || cgameGlob->time - prevTimes[localClientNum] > 100 )
        skipLerp = 1;
      prevTimes[localClientNum] = cgameGlob->time;
    }
    if ( info->type != 2 && !skipLerp )
    {
      if ( info->type == 1 )
        lerpRate = FLOAT_0_89999998;
      v3 = AngleNormalize180(prevPitch[localClientNum] - cgameGlob->refdefViewAngles[0]) * lerpRate;
      if ( COERCE_FLOAT(LODWORD(FLOAT_30_0) ^ _mask__NegFloat_) >= 30.0
        && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.h", 730, 0, "%s", "min < max") )
      {
        __debugbreak();
      }
      if ( COERCE_FLOAT(LODWORD(FLOAT_30_0) ^ _mask__NegFloat_) <= v3 )
      {
        if ( v3 <= 30.0 )
          pitchOffset = v3;
        else
          pitchOffset = FLOAT_30_0;
      }
      else
      {
        LODWORD(pitchOffset) = LODWORD(FLOAT_30_0) ^ _mask__NegFloat_;
      }
      cgameGlob->refdefViewAngles[0] = cgameGlob->refdefViewAngles[0] + pitchOffset;
    }
    prevPitch[localClientNum] = cgameGlob->refdefViewAngles[0];
  }
}

double __cdecl ThirdPersonViewTrace(
        cg_s *cgameGlob,
        const float *start,
        const float *end,
        int contentMask,
        float *result,
        bool pitch_up,
        bool ignoreStartSolid)
{
  float dir[3]; // [esp+24h] [ebp-7Ch] BYREF
  col_context_t context; // [esp+30h] [ebp-70h] BYREF
  float testEnd[3]; // [esp+58h] [ebp-48h] BYREF
  trace_t trace; // [esp+64h] [ebp-3Ch] BYREF

  memset(&trace, 0, 16);
  col_context_t::col_context_t(&context);
  if ( (cgameGlob->predictedPlayerState.eFlags & 0x4000) != 0 )
    CG_TraceCapsule(
      &trace,
      start,
      MYMINS,
      MYMAXS,
      end,
      cgameGlob->predictedPlayerState.viewlocked_entNum,
      contentMask,
      &context);
  else
    CG_TraceCapsule(
      &trace,
      start,
      MYMINS,
      MYMAXS,
      end,
      cgameGlob->predictedPlayerState.clientNum,
      contentMask,
      &context);
  if ( ignoreStartSolid || !trace.startsolid )
  {
    if ( trace.fraction == 1.0 )
    {
      *result = *end;
      result[1] = end[1];
      result[2] = end[2];
      return trace.fraction;
    }
    else
    {
      ThirdPerson_TraceCalcEndpos(start, end, trace.fraction, testEnd);
      if ( pitch_up )
      {
        testEnd[2] = (float)((float)(1.0 - trace.fraction) * 32.0) + testEnd[2];
        if ( (cgameGlob->predictedPlayerState.eFlags & 0x4000) != 0 )
          CG_TraceCapsule(
            &trace,
            start,
            MYMINS,
            MYMAXS,
            testEnd,
            cgameGlob->predictedPlayerState.viewlocked_entNum,
            contentMask,
            &context);
        else
          CG_TraceCapsule(
            &trace,
            start,
            MYMINS,
            MYMAXS,
            testEnd,
            cgameGlob->predictedPlayerState.clientNum,
            contentMask,
            &context);
        ThirdPerson_TraceCalcEndpos(start, testEnd, trace.fraction, result);
        return trace.fraction;
      }
      else
      {
        *result = testEnd[0];
        result[1] = testEnd[1];
        result[2] = testEnd[2];
        return trace.fraction;
      }
    }
  }
  else
  {
    dir[0] = *end - *start;
    dir[1] = end[1] - start[1];
    dir[2] = end[2] - start[2];
    Vec3NormalizeFast(dir);
    *result = *start + dir[0];
    result[1] = start[1] + dir[1];
    result[2] = start[2] + dir[2];
    return 0.0;
  }
}

void __cdecl Vec3NormalizeFast(float *v)
{
  float invLength; // [esp+1Ch] [ebp-4h]

  invLength = I_rsqrt(COERCE_INT((float)((float)(*v * *v) + (float)(v[1] * v[1])) + (float)(v[2] * v[2])));
  *v = *v * invLength;
  v[1] = v[1] * invLength;
  v[2] = v[2] * invLength;
}

void __cdecl ThirdPerson_TraceCalcEndpos(const float *start, const float *end, float fraction, float *endpos)
{
  if ( fraction <= 0.001 )
  {
    *endpos = *start;
    endpos[1] = start[1];
    endpos[2] = start[2];
  }
  else
  {
    Vec3Lerp(start, end, fraction, endpos);
  }
}

void __cdecl MovieCameraViewTrace(int localClientNum, int contentMask)
{
  float *v2; // [esp+8h] [ebp-1A4h]
  float *v3; // [esp+Ch] [ebp-1A0h]
  float *v4; // [esp+10h] [ebp-19Ch]
  float *v5; // [esp+28h] [ebp-184h]
  float *v6; // [esp+2Ch] [ebp-180h]
  float *v7; // [esp+30h] [ebp-17Ch]
  float *v8; // [esp+34h] [ebp-178h]
  float *movieCameraVelocity; // [esp+3Ch] [ebp-170h]
  float *movieCameraOrigin; // [esp+40h] [ebp-16Ch]
  float *v11; // [esp+44h] [ebp-168h]
  int j; // [esp+60h] [ebp-14Ch]
  col_context_t context; // [esp+64h] [ebp-148h] BYREF
  float dir[3]; // [esp+8Ch] [ebp-120h] BYREF
  float d; // [esp+98h] [ebp-114h]
  int numbumps; // [esp+9Ch] [ebp-110h]
  float endClipVelocity[3]; // [esp+A0h] [ebp-10Ch] BYREF
  cg_s *cgameGlob; // [esp+ACh] [ebp-100h]
  int k; // [esp+B0h] [ebp-FCh]
  float planes[8][3]; // [esp+B4h] [ebp-F8h] BYREF
  int permutation[8]; // [esp+118h] [ebp-94h] BYREF
  int numplanes; // [esp+138h] [ebp-74h]
  int bumpcount; // [esp+13Ch] [ebp-70h]
  float testEnd[3]; // [esp+140h] [ebp-6Ch] BYREF
  trace_t trace; // [esp+14Ch] [ebp-60h] BYREF
  float endVelocity[3]; // [esp+188h] [ebp-24h] BYREF
  float clipVelocity[3]; // [esp+194h] [ebp-18h] BYREF
  int i; // [esp+1A0h] [ebp-Ch]
  float traceFractionScale; // [esp+1A4h] [ebp-8h]
  float into; // [esp+1A8h] [ebp-4h]

  col_context_t::col_context_t(&context);
  memset(&trace, 0, 16);
  numplanes = 0;
  numbumps = 4;
  traceFractionScale = FLOAT_0_01667;
  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  endVelocity[0] = cgameGlob->movieCameraVelocity[0];
  endVelocity[1] = cgameGlob->movieCameraVelocity[1];
  endVelocity[2] = cgameGlob->movieCameraVelocity[2];
  Vec3NormalizeTo(cgameGlob->movieCameraVelocity, planes[numplanes++]);
  for ( bumpcount = 0; bumpcount < numbumps; ++bumpcount )
  {
    testEnd[0] = (float)(traceFractionScale * cgameGlob->movieCameraVelocity[0]) + cgameGlob->movieCameraOrigin[0];
    testEnd[1] = (float)(traceFractionScale * cgameGlob->movieCameraVelocity[1]) + cgameGlob->movieCameraOrigin[1];
    testEnd[2] = (float)(traceFractionScale * cgameGlob->movieCameraVelocity[2]) + cgameGlob->movieCameraOrigin[2];
    CG_TraceCapsule(&trace, cgameGlob->movieCameraOrigin, MYMINS_0, MYMAXS_0, testEnd, -1, contentMask, &context);
    if ( trace.allsolid )
    {
      cgameGlob->movieCameraVelocity[2] = *(float *)&FLOAT_0_0;
      return;
    }
    if ( trace.startsolid )
    {
      movieCameraOrigin = cgameGlob->movieCameraOrigin;
      v11 = cgameGlob->movieCameraOrigin;
      cgameGlob->movieCameraOrigin[0] = cgameGlob->movieCameraOrigin[0] + trace.normal.vec.v[0];
      movieCameraOrigin[1] = v11[1] + trace.normal.vec.v[1];
      movieCameraOrigin[2] = v11[2] + trace.normal.vec.v[2];
    }
    if ( trace.fraction > 0.0 )
      Vec3Lerp(cgameGlob->movieCameraOrigin, testEnd, trace.fraction, cgameGlob->movieCameraOrigin);
    if ( trace.fraction == 1.0 )
      return;
    traceFractionScale = traceFractionScale - (float)(traceFractionScale * trace.fraction);
    if ( numplanes >= 8 )
    {
      movieCameraVelocity = cgameGlob->movieCameraVelocity;
      cgameGlob->movieCameraVelocity[0] = *(float *)&FLOAT_0_0;
      movieCameraVelocity[1] = *(float *)&FLOAT_0_0;
      movieCameraVelocity[2] = *(float *)&FLOAT_0_0;
      return;
    }
    for ( i = 0; i < numplanes; ++i )
    {
      if ( (float)((float)((float)(trace.normal.vec.v[0] * planes[i][0]) + (float)(trace.normal.vec.v[1] * planes[i][1]))
                 + (float)(trace.normal.vec.v[2] * planes[i][2])) > 0.99900001 )
      {
        PM_ClipVelocity(cgameGlob->movieCameraVelocity, trace.normal.vec.v, cgameGlob->movieCameraVelocity);
        v7 = cgameGlob->movieCameraVelocity;
        v8 = cgameGlob->movieCameraVelocity;
        cgameGlob->movieCameraVelocity[0] = trace.normal.vec.v[0] + cgameGlob->movieCameraVelocity[0];
        v7[1] = trace.normal.vec.v[1] + v8[1];
        v7[2] = trace.normal.vec.v[2] + v8[2];
        break;
      }
    }
    if ( i >= numplanes )
    {
      v6 = planes[numplanes];
      *(_QWORD *)v6 = *(_QWORD *)trace.normal.vec.v;
      v6[2] = trace.normal.vec.v[2];
      into = PM_PermuteRestrictiveClipPlanes(cgameGlob->movieCameraVelocity, ++numplanes, planes, permutation);
      if ( into < 0.1 )
      {
        PM_ClipVelocity(cgameGlob->movieCameraVelocity, planes[permutation[0]], clipVelocity);
        PM_ClipVelocity(endVelocity, planes[permutation[0]], endClipVelocity);
        for ( j = 1; j < numplanes; ++j )
        {
          v5 = planes[permutation[j]];
          if ( (float)((float)((float)(clipVelocity[0] * *v5) + (float)(clipVelocity[1] * v5[1]))
                     + (float)(clipVelocity[2] * v5[2])) < 0.1 )
          {
            PM_ClipVelocity(clipVelocity, planes[permutation[j]], clipVelocity);
            PM_ClipVelocity(endClipVelocity, planes[permutation[j]], endClipVelocity);
            if ( (float)((float)((float)(clipVelocity[0] * planes[permutation[0]][0])
                               + (float)(clipVelocity[1] * planes[permutation[0]][1]))
                       + (float)(clipVelocity[2] * planes[permutation[0]][2])) < 0.0 )
            {
              Vec3Cross(planes[permutation[0]], planes[permutation[j]], dir);
              Vec3Normalize(dir);
              d = (float)((float)(dir[0] * cgameGlob->movieCameraVelocity[0])
                        + (float)(dir[1] * cgameGlob->movieCameraVelocity[1]))
                + (float)(dir[2] * cgameGlob->movieCameraVelocity[2]);
              clipVelocity[0] = d * dir[0];
              clipVelocity[1] = d * dir[1];
              clipVelocity[2] = d * dir[2];
              d = (float)((float)(dir[0] * endVelocity[0]) + (float)(dir[1] * endVelocity[1]))
                + (float)(dir[2] * endVelocity[2]);
              endClipVelocity[0] = d * dir[0];
              endClipVelocity[1] = d * dir[1];
              endClipVelocity[2] = d * dir[2];
              for ( k = 1; k < numplanes; ++k )
              {
                if ( k != j )
                {
                  v4 = planes[permutation[k]];
                  if ( (float)((float)((float)(clipVelocity[0] * *v4) + (float)(clipVelocity[1] * v4[1]))
                             + (float)(clipVelocity[2] * v4[2])) < 0.1 )
                  {
                    v3 = cgameGlob->movieCameraVelocity;
                    cgameGlob->movieCameraVelocity[0] = *(float *)&FLOAT_0_0;
                    v3[1] = *(float *)&FLOAT_0_0;
                    v3[2] = *(float *)&FLOAT_0_0;
                    return;
                  }
                }
              }
            }
          }
        }
        v2 = cgameGlob->movieCameraVelocity;
        cgameGlob->movieCameraVelocity[0] = clipVelocity[0];
        v2[1] = clipVelocity[1];
        v2[2] = clipVelocity[2];
        endVelocity[0] = endClipVelocity[0];
        endVelocity[1] = endClipVelocity[1];
        endVelocity[2] = endClipVelocity[2];
      }
    }
  }
}

int __cdecl CG_ScrCamActive(int localClientNum)
{
  return CG_GetLocalClientGlobals(localClientNum)->cameraData.scriptCam.flags & 1;
}

void __cdecl CG_ScrCamCalcView(int localClientNum)
{
  centity_s *v1; // [esp+28h] [ebp-14h]
  centity_s *Entity; // [esp+2Ch] [ebp-10h]
  centity_s *cent; // [esp+30h] [ebp-Ch]
  cg_s *cgameGlob; // [esp+34h] [ebp-8h]
  const ScriptCamera *camera; // [esp+38h] [ebp-4h]

  if ( !(unsigned __int8)CG_ScrCamActive(localClientNum)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_camera.cpp",
          2570,
          0,
          "%s",
          "CG_ScrCamActive( localClientNum )") )
  {
    __debugbreak();
  }
  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  camera = &cgameGlob->cameraData.scriptCam;
  if ( (cgameGlob->cameraData.scriptCam.flags & 4) != 0 )
  {
    cent = CG_GetEntity(localClientNum, cgameGlob->cameraData.scriptCam.originCent);
    if ( !cent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_camera.cpp", 2578, 0, "%s", "cent") )
      __debugbreak();
    cgameGlob->refdef.vieworg[0] = cent->pose.origin[0];
    cgameGlob->refdef.vieworg[1] = cent->pose.origin[1];
    cgameGlob->refdef.vieworg[2] = cent->pose.origin[2];
  }
  else
  {
    cgameGlob->refdef.vieworg[0] = cgameGlob->cameraData.scriptCam.origin[0];
    cgameGlob->refdef.vieworg[1] = cgameGlob->cameraData.scriptCam.origin[1];
    cgameGlob->refdef.vieworg[2] = cgameGlob->cameraData.scriptCam.origin[2];
    AnglesToAxis(cgameGlob->cameraData.scriptCam.angles, cgameGlob->refdef.viewaxis);
  }
  if ( (camera->flags & 0x10) != 0 )
  {
    Entity = CG_GetEntity(localClientNum, cgameGlob->cameraData.scriptCam.lookAtCent);
    if ( !Entity && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_camera.cpp", 2591, 0, "%s", "cent") )
      __debugbreak();
    ScrCam_CalcOrientOrigin(Entity->pose.origin, cgameGlob);
  }
  else if ( (camera->flags & 8) != 0 )
  {
    ScrCam_CalcOrientOrigin(cgameGlob->cameraData.scriptCam.lookat, cgameGlob);
  }
  else if ( (camera->flags & 0x20) != 0 )
  {
    v1 = CG_GetEntity(localClientNum, cgameGlob->cameraData.scriptCam.originCent);
    if ( !v1 && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_camera.cpp", 2602, 0, "%s", "cent") )
      __debugbreak();
    AnglesToAxis(v1->pose.angles, cgameGlob->refdef.viewaxis);
  }
}

void __cdecl ScrCam_CalcOrientOrigin(const float *lookAtVec, cg_s *cgameGlob)
{
  float delta[3]; // [esp+4h] [ebp-Ch] BYREF

  delta[0] = *lookAtVec - cgameGlob->refdef.vieworg[0];
  delta[1] = lookAtVec[1] - cgameGlob->refdef.vieworg[1];
  delta[2] = lookAtVec[2] - cgameGlob->refdef.vieworg[2];
  ScrCam_CalcOrientDir(delta, cgameGlob);
}

void __cdecl ScrCam_CalcOrientDir(float *lookAtVec, cg_s *cgameGlob)
{
  float angles[3]; // [esp+0h] [ebp-Ch] BYREF

  vectoangles(lookAtVec, angles);
  AnglesToAxis(angles, cgameGlob->refdef.viewaxis);
}

void __cdecl CG_MovieCamCalcView(int localClientNum)
{
  cg_s *cgameGlob; // [esp+8h] [ebp-4h]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  cgameGlob->movieCameraAngles[2] = *(float *)&FLOAT_0_0;
  AnglesToAxis(cgameGlob->movieCameraAngles, cgameGlob->refdef.viewaxis);
  cgameGlob->refdef.vieworg[0] = cgameGlob->movieCameraOrigin[0];
  cgameGlob->refdef.vieworg[1] = cgameGlob->movieCameraOrigin[1];
  cgameGlob->refdef.vieworg[2] = cgameGlob->movieCameraOrigin[2];
}

void __cdecl CG_RadiantCamCalcView(int localClientNum)
{
  int i; // [esp+14h] [ebp-44h]
  cg_s *cgameGlob; // [esp+18h] [ebp-40h]
  usercmd_s cmd; // [esp+1Ch] [ebp-3Ch] BYREF
  int cmdNumber; // [esp+54h] [ebp-4h]

  for ( i = 0; i < 2; ++i )
    cmd.button_bits.array[i] = 0;
  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  cmdNumber = CL_GetCurrentCmdNumber(localClientNum);
  if ( CL_GetUserCmd(localClientNum, cmdNumber, &cmd)
    && (abs8(cmd.forwardmove) > 0x32u
     || abs8(cmd.rightmove) > 0x32u
     || abs8(cmd.pitchmove) > 0x32u
     || abs8(cmd.yawmove) > 0x32u) )
  {
    cgameGlob->radiantCamInUse = 0;
  }
  cgameGlob->refdefViewAngles[0] = cgameGlob->radiantCameraAngles[0];
  cgameGlob->refdefViewAngles[1] = cgameGlob->radiantCameraAngles[1];
  cgameGlob->refdefViewAngles[2] = cgameGlob->radiantCameraAngles[2];
  AnglesToAxis(cgameGlob->radiantCameraAngles, cgameGlob->refdef.viewaxis);
  cgameGlob->refdef.vieworg[0] = cgameGlob->radiantCameraOrigin[0];
  cgameGlob->refdef.vieworg[1] = cgameGlob->radiantCameraOrigin[1];
  cgameGlob->refdef.vieworg[2] = cgameGlob->radiantCameraOrigin[2];
}

void __cdecl CG_ExtraCamDebug_SetView(int localClientNum)
{
  cg_s *cgameGlob; // [esp+10h] [ebp-4h]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  if ( CL_LocalClient_IsFirstActive(localClientNum) )
  {
    if ( r_missile_cam_debug_display->current.integer == 3 )
    {
      cgameGlob->refdef.vieworg[0] = g_extraCamPos[0];
      LODWORD(cgameGlob->refdef.vieworg[1]) = dword_E869F4;
      LODWORD(cgameGlob->refdef.vieworg[2]) = dword_E869F8;
      cgameGlob->refdefViewAngles[0] = g_extraCamViewAngles[0];
      LODWORD(cgameGlob->refdefViewAngles[1]) = dword_E86A00;
      LODWORD(cgameGlob->refdefViewAngles[2]) = dword_E86A04;
      memcpy(cgameGlob->refdef.viewaxis, g_extraCamViewAxis, sizeof(cgameGlob->refdef.viewaxis));
    }
  }
}

void __cdecl CG_ExtraCamDebug_SaveView(int localClientNum)
{
  cg_s *cgameGlob; // [esp+1Ch] [ebp-4h]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  if ( CL_LocalClient_IsFirstActive(localClientNum) && r_missile_cam_debug_display->current.integer == 3 )
  {
    if ( r_missile_cam_debug_display->modified )
    {
      Dvar_ClearModified(r_missile_cam_debug_display);
      g_extraCamPos[0] = cgameGlob->refdef.vieworg[0];
      dword_E869F4 = LODWORD(cgameGlob->refdef.vieworg[1]);
      dword_E869F8 = LODWORD(cgameGlob->refdef.vieworg[2]);
      g_extraCamViewAngles[0] = cgameGlob->refdefViewAngles[0];
      dword_E86A00 = LODWORD(cgameGlob->refdefViewAngles[1]);
      dword_E86A04 = LODWORD(cgameGlob->refdefViewAngles[2]);
      memcpy(g_extraCamViewAxis, cgameGlob->refdef.viewaxis, sizeof(g_extraCamViewAxis));
    }
    CL_AddDebugAxis(g_extraCamPos, g_extraCamViewAxis, 40.0, 1, 1);
  }
}

void __cdecl CG_CalcFov_ExtraCam(int localClientNum)
{
  long double v1; // [esp+0h] [ebp-1Ch]
  float value; // [esp+0h] [ebp-1Ch]
  float fov_x; // [esp+8h] [ebp-14h]
  cg_s *cgameGlob; // [esp+Ch] [ebp-10h]
  float dxDzAtDefaultAspectRatio; // [esp+10h] [ebp-Ch]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  fov_x = cg_fovExtraCam->current.value;
  __libm_sse2_tan(v1);
  dxDzAtDefaultAspectRatio = (float)(fov_x * 0.017453292) * 0.5;
  if ( r_extracam_custom_aspectratio->current.value <= 0.0 )
    value = R_ExtraCam_AspectRatio();
  else
    value = r_extracam_custom_aspectratio->current.value;
  cgameGlob->refdef.tanHalfFovX = dxDzAtDefaultAspectRatio * value;
  cgameGlob->refdef.tanHalfFovY = dxDzAtDefaultAspectRatio;
  cgameGlob->refdef.fov_x = fov_x;
}

void __cdecl CG_ExtraCam_GetViewOrigin(int localClientNum, float *out)
{
  if ( CG_ExtraCamViewActive(localClientNum) )
    CG_GetExtraCamOrigin(localClientNum, out);
  if ( CG_RenderPlayerFromMissilePOV(localClientNum) )
  {
    CG_GetMissileViewOrigin(localClientNum, out);
  }
  else if ( r_missile_cam_debug_display->current.integer == 3 )
  {
    *out = g_extraCamPos[0];
    *((unsigned int *)out + 1) = dword_E869F4;
    *((unsigned int *)out + 2) = dword_E869F8;
  }
}

bool __cdecl CG_ExtraCamIsActive(int localClientNum)
{
  if ( !R_ExtraCam_GfxResourcesReady() )
    return 0;
  if ( r_reflectionProbeGenerate->current.enabled )
    return 0;
  if ( CL_GetLocalClientConnectionState(localClientNum) != 10 )
    return 0;
  CL_GetLocalClientGlobals(localClientNum);
  if ( CG_GetLocalClientGlobals(localClientNum)->cameraMode == 1 )
    return 0;
  if ( CG_ExtraCamViewActive(localClientNum) )
    return 1;
  return r_missile_cam_debug_display->current.integer != 0;
}

clientActive_t *__cdecl CL_GetLocalClientGlobals(int localClientNum)
{
  if ( localClientNum
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\cgame\\../client/client.h",
          192,
          0,
          "%s\n\t(localClientNum) = %i",
          "(localClientNum == 0)",
          localClientNum) )
  {
    __debugbreak();
  }
  return clients;
}

int __cdecl CL_GetLocalClientConnectionState(int localClientNum)
{
  if ( localClientNum
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\cgame\\../client/client.h",
          217,
          0,
          "%s\n\t(localClientNum) = %i",
          "(localClientNum == 0)",
          localClientNum) )
  {
    __debugbreak();
  }
  return dword_FB2C3C;
}

ExtraCamClientStateRestore *__thiscall ExtraCamClientStateRestore::ExtraCamClientStateRestore(
        ExtraCamClientStateRestore *this,
        int localClientNum)
{
  cg_s *LocalClientGlobals; // eax

  this->m_localClientNum = localClientNum;
  LocalClientGlobals = CG_GetLocalClientGlobals(this->m_localClientNum);
  this->m_vieworg[0] = LocalClientGlobals->refdef.vieworg[0];
  this->m_vieworg[1] = LocalClientGlobals->refdef.vieworg[1];
  this->m_vieworg[2] = LocalClientGlobals->refdef.vieworg[2];
  this->m_refdefViewAngles[0] = LocalClientGlobals->refdefViewAngles[0];
  this->m_refdefViewAngles[1] = LocalClientGlobals->refdefViewAngles[1];
  this->m_refdefViewAngles[2] = LocalClientGlobals->refdefViewAngles[2];
  AxisCopy(LocalClientGlobals->refdef.viewaxis, this->m_viewaxis);
  R_ExtraCam_SaveDpvsData(this->m_localClientNum, this->m_buffer, 0x360u);
  return this;
}

void __thiscall ExtraCamClientStateRestore::~ExtraCamClientStateRestore(ExtraCamClientStateRestore *this)
{
  cg_s *LocalClientGlobals; // eax

  LocalClientGlobals = CG_GetLocalClientGlobals(this->m_localClientNum);
  LocalClientGlobals->refdef.vieworg[0] = this->m_vieworg[0];
  LocalClientGlobals->refdef.vieworg[1] = this->m_vieworg[1];
  LocalClientGlobals->refdef.vieworg[2] = this->m_vieworg[2];
  LocalClientGlobals->refdefViewAngles[0] = this->m_refdefViewAngles[0];
  LocalClientGlobals->refdefViewAngles[1] = this->m_refdefViewAngles[1];
  LocalClientGlobals->refdefViewAngles[2] = this->m_refdefViewAngles[2];
  AxisCopy(this->m_viewaxis, LocalClientGlobals->refdef.viewaxis);
  R_ExtraCam_RestoreDpvsData(this->m_localClientNum, this->m_buffer);
}

void __cdecl RotatePoint(float *point, const float (*mat)[3])
{
  float tvec; // [esp+8h] [ebp-Ch]
  float tvec_4; // [esp+Ch] [ebp-8h]
  float tvec_8; // [esp+10h] [ebp-4h]

  tvec = *point;
  tvec_4 = point[1];
  tvec_8 = point[2];
  *point = (float)((float)((*mat)[0] * *point) + (float)((float)(*mat)[1] * tvec_4))
         + (float)((float)(*mat)[2] * tvec_8);
  point[1] = (float)((float)((float)(*mat)[3] * tvec) + (float)((float)(*mat)[4] * tvec_4))
           + (float)((float)(*mat)[5] * tvec_8);
  point[2] = (float)((float)((float)(*mat)[6] * tvec) + (float)((float)(*mat)[7] * tvec_4))
           + (float)((float)(*mat)[8] * tvec_8);
}

