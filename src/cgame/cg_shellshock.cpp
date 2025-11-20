#include "cg_shellshock.h"

void __cdecl CG_PerturbCamera(cg_s *cgameGlob)
{
  float rot[3][3]; // [esp+18h] [ebp-48h] BYREF
  float axis[3][3]; // [esp+3Ch] [ebp-24h] BYREF

  if ( cgameGlob->shellshock.viewDelta[0] != 0.0 || cgameGlob->shellshock.viewDelta[1] != 0.0 )
  {
    rot[0][0] = FLOAT_1_0;
    *(_QWORD *)&rot[0][1] = *(_QWORD *)cgameGlob->shellshock.viewDelta;
    rot[2][0] = *(float *)&FLOAT_0_0;
    *(_QWORD *)&rot[2][1] = __PAIR64__(LODWORD(FLOAT_1_0), *(unsigned int *)&FLOAT_0_0);
    Vec3Normalize(rot[0]);
    Vec3Cross(rot[2], rot[0], rot[1]);
    Vec3Normalize(rot[1]);
    Vec3Cross(rot[0], rot[1], rot[2]);
    AxisCopy(cgameGlob->refdef.viewaxis, axis);
    MatrixMultiply(rot, axis, cgameGlob->refdef.viewaxis);
  }
}

int __cdecl CG_DrawShellShockSavedScreenBlendBlurred(
        int localClientNum,
        const shellshock_parms_t *parms,
        int start,
        int duration)
{
  int dt; // [esp+24h] [ebp-10h]
  cg_s *cgameGlob; // [esp+28h] [ebp-Ch]
  const ClientViewParams *view; // [esp+2Ch] [ebp-8h]
  int screenBlendTime; // [esp+30h] [ebp-4h]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  if ( start && duration > 0 )
  {
    dt = duration + start - cgameGlob->time;
    if ( dt > 0 )
    {
      screenBlendTime = parms->screenBlend.blurredEffectTime;
      if ( dt < parms->screenBlend.blurredFadeTime )
        screenBlendTime = (int)((float)((float)screenBlendTime
                                      * (float)((float)dt / (float)parms->screenBlend.blurredFadeTime))
                              + 9.313225746154785e-10);
      if ( cgameGlob->shellshock.hasSavedScreen )
      {
        view = CG_GetLocalClientViewParams(localClientNum);
        CG_BlendSavedScreenShockBlurred(screenBlendTime, view->x, view->y, view->width, view->height, localClientNum);
      }
      SaveScreenToBuffer(localClientNum);
      cgameGlob->shellshock.hasSavedScreen = 1;
      return 1;
    }
    else
    {
      cgameGlob->shellshock.hasSavedScreen = 0;
      return 0;
    }
  }
  else
  {
    cgameGlob->shellshock.hasSavedScreen = 0;
    return 0;
  }
}

void __cdecl SaveScreenToBuffer(int localClientNum)
{
  const ClientViewParams *view; // [esp+14h] [ebp-8h]

  if ( CL_LocalClient_GetActiveCount() > 1 )
  {
    view = CG_GetLocalClientViewParams(localClientNum);
    CG_SaveScreenSection(view->x, view->y, view->width, view->height, localClientNum);
  }
  else
  {
    CG_SaveScreen(localClientNum);
  }
}

int __cdecl CG_DrawShellShockSavedScreenBlendFlashed(
        int localClientNum,
        const shellshock_parms_t *parms,
        int start,
        int duration)
{
  int dt; // [esp+18h] [ebp-14h]
  cg_s *cgameGlob; // [esp+1Ch] [ebp-10h]
  float whiteFactor; // [esp+20h] [ebp-Ch]
  float whiteFactora; // [esp+20h] [ebp-Ch]
  float whiteFactorb; // [esp+20h] [ebp-Ch]
  const ClientViewParams *view; // [esp+24h] [ebp-8h]
  float grabFactor; // [esp+28h] [ebp-4h]
  float grabFactora; // [esp+28h] [ebp-4h]
  float grabFactorb; // [esp+28h] [ebp-4h]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  if ( start && duration > 0 )
  {
    dt = duration + start - cgameGlob->time;
    if ( dt > 0 )
    {
      whiteFactor = (float)parms->screenBlend.flashWhiteFadeTime;
      grabFactor = (float)parms->screenBlend.flashShotFadeTime;
      if ( whiteFactor <= (float)dt )
        whiteFactora = FLOAT_1_0;
      else
        whiteFactora = (float)dt / whiteFactor;
      if ( grabFactor <= (float)dt )
        grabFactora = FLOAT_1_0;
      else
        grabFactora = (float)dt / grabFactor;
      whiteFactorb = BlendSmooth(whiteFactora);
      grabFactorb = BlendSmooth(grabFactora);
      if ( cgameGlob->shellshock.hasSavedScreen )
      {
        view = CG_GetLocalClientViewParams(localClientNum);
        CG_BlendSavedScreenShockFlashed(
          localClientNum,
          whiteFactorb,
          grabFactorb,
          view->x,
          view->y,
          view->width,
          view->height);
      }
      else
      {
        SaveScreenToBuffer(localClientNum);
      }
      cgameGlob->shellshock.hasSavedScreen = 1;
      return 1;
    }
    else
    {
      cgameGlob->shellshock.hasSavedScreen = 0;
      return 0;
    }
  }
  else
  {
    cgameGlob->shellshock.hasSavedScreen = 0;
    return 0;
  }
}

double __cdecl BlendSmooth(float percent)
{
  float sin; // [esp+10h] [ebp-4h]

  sin = sin((float)((float)(percent - 0.5) * 3.1415927));
  return (sin + 1.0) * 0.5;
}

void __cdecl EndShellShock(int localClientNum)
{
  EndShellShockSound(localClientNum);
  EndShellShockLookControl(localClientNum);
  EndShellShockCamera(localClientNum);
  EndShellShockScreen(localClientNum);
}

void __cdecl EndShellShockSound(int localClientNum)
{
  cg_s *cgameGlob; // [esp+8h] [ebp-4h]

  SND_SetSnapshot(SND_SNAPSHOT_SHELLSHOCK, "default", 0.0, 1.0);
  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  if ( cgameGlob->shellshock.loopEndTime )
  {
    cgameGlob->shellshock.loopEndTime = 0;
    CG_PlaySound(localClientNum, 1023, vec3_origin, 0, 0, 1.0, cgameGlob->shellshock.parms->sound.endAbort);
  }
}

void __cdecl EndShellShockLookControl(int localClientNum)
{
  CG_GetLocalClientGlobals(localClientNum)->shellshock.sensitivity = FLOAT_1_0;
  CL_CapTurnRate(localClientNum, 0.0, 0.0);
}

void __cdecl EndShellShockCamera(int localClientNum)
{
  cg_s *cgameGlob; // eax

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  cgameGlob->shellshock.viewDelta[0] = *(float *)&FLOAT_0_0;
  cgameGlob->shellshock.viewDelta[1] = *(float *)&FLOAT_0_0;
}

void __cdecl EndShellShockScreen(int localClientNum)
{
  CG_GetLocalClientGlobals(localClientNum)->shellshock.hasSavedScreen = 0;
}

void __cdecl CG_UpdateShellShock(int localClientNum, const shellshock_parms_t *parms, int start, int duration)
{
  bool killcamEntityEnabled; // [esp+7h] [ebp-5h]
  int time; // [esp+8h] [ebp-4h]

  time = CG_GetLocalClientGlobals(localClientNum)->time - start;
  if ( start && time >= 0 )
  {
    killcamEntityEnabled = CG_KillCamEntityEnabled(localClientNum);
    UpdateShellShockSound(localClientNum, parms, time, duration);
    if ( Demo_IsThirdPersonCamera() || Demo_IsMovieCamera() || killcamEntityEnabled )
    {
      EndShellShockLookControl(localClientNum);
      EndShellShockCamera(localClientNum);
      EndShellShockScreen(localClientNum);
    }
    else
    {
      UpdateShellShockLookControl(localClientNum, parms, time, duration);
      UpdateShellShockCamera(localClientNum, parms, time, duration);
      UpdateShellShockVisionSet(localClientNum, parms, time, duration);
    }
  }
  else
  {
    SND_DeactivateEnvironmentEffects(2, 0);
    EndShellShock(localClientNum);
  }
}

void __cdecl UpdateShellShockSound(int localClientNum, const shellshock_parms_t *parms, int time, int duration)
{
  bool shellshockActive; // [esp+1Bh] [ebp-19h]
  int dt; // [esp+1Ch] [ebp-18h]
  cg_s *cgameGlob; // [esp+20h] [ebp-14h]
  SndEntHandle sndEnt; // [esp+24h] [ebp-10h]

  shellshockActive = 0;
  if ( !parms && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_shellshock.cpp", 452, 0, "%s", "parms") )
    __debugbreak();
  if ( time < 0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_shellshock.cpp", 453, 0, "%s", "time >= 0") )
  {
    __debugbreak();
  }
  if ( duration < 0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_shellshock.cpp", 454, 0, "%s", "duration >= 0") )
  {
    __debugbreak();
  }
  if ( parms->sound.affect )
  {
    cgameGlob = CG_GetLocalClientGlobals(localClientNum);
    dt = parms->sound.fadeOutTime + parms->sound.modEndDelay + duration - time;
    if ( time >= parms->sound.fadeInTime )
    {
      if ( dt <= parms->sound.fadeOutTime )
      {
        if ( dt >= 0 && dt < parms->sound.fadeOutTime )
        {
          SND_DeactivateEnvironmentEffects(2, dt);
          SND_SetSnapshot(SND_SNAPSHOT_SHELLSHOCK, "default", 0.0, 1.0);
        }
      }
      else
      {
        SND_SetEnvironmentEffects(2, parms->sound.roomtype, parms->sound.drylevel, parms->sound.wetlevel, 0);
        SND_SetSnapshot(SND_SNAPSHOT_SHELLSHOCK, parms->sound.snapshot, 0.0, 1.0);
        shellshockActive = 1;
      }
    }
    else
    {
      SND_SetEnvironmentEffects(
        2,
        parms->sound.roomtype,
        parms->sound.drylevel,
        parms->sound.wetlevel,
        parms->sound.fadeInTime - time);
      SND_SetSnapshot(SND_SNAPSHOT_SHELLSHOCK, parms->sound.snapshot, 0.0, 1.0);
      shellshockActive = 1;
    }
    sndEnt.field = CG_SndEntHandle(localClientNum, cgameGlob->clientNum).field;
    if ( cgameGlob->shellshockSoundActive && shellshockActive )
    {
      SND_Play(parms->sound.loop, 0, 1.0, sndEnt, 0, 0, 0);
    }
    else if ( !cgameGlob->shellshockSoundActive && shellshockActive && parms->sound.loop[0] )
    {
      SND_Play(parms->sound.loop, parms->sound.fadeInTime, 1.0, sndEnt, 0, 0, 0);
      cgameGlob->shellshockSoundActive = 1;
    }
    else if ( cgameGlob->shellshockSoundActive )
    {
      if ( !shellshockActive )
      {
        SND_Play(parms->sound.loop, -parms->sound.fadeOutTime, 1.0, sndEnt, 0, 0, 0);
        SND_Play(parms->sound.end, 0, 1.0, sndEnt, 0, 0, 0);
        cgameGlob->shellshockSoundActive = 0;
      }
    }
  }
  else
  {
    SND_DeactivateEnvironmentEffects(2, 0);
  }
}

void __cdecl UpdateShellShockLookControl(int localClientNum, const shellshock_parms_t *parms, int time, int duration)
{
  float fade; // [esp+8h] [ebp-8h]
  cg_s *cgameGlob; // [esp+Ch] [ebp-4h]

  if ( !parms && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_shellshock.cpp", 516, 0, "%s", "parms") )
    __debugbreak();
  if ( time < 0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_shellshock.cpp", 517, 0, "%s", "time >= 0") )
  {
    __debugbreak();
  }
  if ( duration < 0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_shellshock.cpp", 518, 0, "%s", "duration >= 0") )
  {
    __debugbreak();
  }
  if ( !parms->lookControl.affect )
    goto LABEL_11;
  if ( duration - time < parms->lookControl.fadeTime )
  {
    if ( duration - time <= 0 )
    {
LABEL_11:
      EndShellShockLookControl(localClientNum);
      return;
    }
    fade = (float)((float)(duration - time) * 1.0) / (float)parms->lookControl.fadeTime;
  }
  else
  {
    fade = FLOAT_1_0;
  }
  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  if ( fade == 1.0 )
  {
    cgameGlob->shellshock.sensitivity = parms->lookControl.mouseSensitivity;
    CL_CapTurnRate(localClientNum, parms->lookControl.maxPitchSpeed, parms->lookControl.maxYawSpeed);
  }
  else
  {
    cgameGlob->shellshock.sensitivity = (float)((float)(parms->lookControl.mouseSensitivity - 1.0) * fade) + 1.0;
    CL_CapTurnRate(localClientNum, parms->lookControl.maxPitchSpeed / fade, parms->lookControl.maxYawSpeed / fade);
  }
}

void __cdecl UpdateShellShockCamera(int localClientNum, const shellshock_parms_t *parms, int time, int duration)
{
  int dt; // [esp+20h] [ebp-20h]
  float t; // [esp+24h] [ebp-1Ch]
  float ta; // [esp+24h] [ebp-1Ch]
  cg_s *cgameGlob; // [esp+28h] [ebp-18h]
  float radius; // [esp+2Ch] [ebp-14h]
  const float *perturb; // [esp+30h] [ebp-10h]
  int base; // [esp+38h] [ebp-8h]
  float scale; // [esp+3Ch] [ebp-4h]

  dt = duration - time;
  if ( duration - time > 0 )
  {
    cgameGlob = CG_GetLocalClientGlobals(localClientNum);
    if ( !parms
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_shellshock.cpp", 581, 0, "%s", "parms") )
    {
      __debugbreak();
    }
    scale = FLOAT_1_0;
    if ( dt < parms->view.fadeTime )
      scale = (float)dt / (float)parms->view.fadeTime;
    radius = parms->view.kickRadius * (float)((float)((float)(3.0 - (float)(2.0 * scale)) * scale) * scale);
    t = (float)time * parms->view.kickRate;
    base = (int)(t - 0.4999999990686774);
    ta = t - (float)base;
    perturb = (const float *)(8 * (((_BYTE)base + 61 * (_BYTE)duration) & 0x7F) + 13171344);
    cgameGlob->shellshock.viewDelta[0] = CubicInterpolate(
                                           ta,
                                           cg_perturbations[((_BYTE)base + 61 * (_BYTE)duration) & 0x7F][0],
                                           cg_perturbations[(((unsigned __int8)base + 61 * (unsigned __int8)duration)
                                                           & 0x7F)
                                                          + 1][0],
                                           cg_perturbations[(((unsigned __int8)base + 61 * (unsigned __int8)duration)
                                                           & 0x7F)
                                                          + 2][0],
                                           cg_perturbations[(((unsigned __int8)base + 61 * (unsigned __int8)duration)
                                                           & 0x7F)
                                                          + 3][0])
                                       * radius;
    cgameGlob->shellshock.viewDelta[1] = CubicInterpolate(ta, perturb[1], perturb[3], perturb[5], perturb[7]) * radius;
  }
  else
  {
    EndShellShockCamera(localClientNum);
  }
}

double __cdecl CubicInterpolate(float t, float x0, float x1, float x2, float x3)
{
  return ((t * (float)((float)((float)(x3 - x2) + x1) - x0)
         + (float)((float)(x0 - x1) - (float)((float)((float)(x3 - x2) + x1) - x0)))
        * t
        + (float)(x2 - x0))
       * t
       + x1;
}

void __cdecl UpdateShellShockVisionSet(int localClientNum, const shellshock_parms_t *parms, int time, int duration)
{
  cg_s *cgameGlob; // [esp+0h] [ebp-4h]

  if ( !parms && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_shellshock.cpp", 603, 0, "%s", "parms") )
    __debugbreak();
  if ( time < 0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_shellshock.cpp", 604, 0, "%s", "time >= 0") )
  {
    __debugbreak();
  }
  if ( duration < 0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_shellshock.cpp", 605, 0, "%s", "duration >= 0") )
  {
    __debugbreak();
  }
  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  if ( parms->visionSet.name[0] )
  {
    if ( time >= duration || cgameGlob->shellshock.visionSetStarted )
    {
      if ( time >= duration && cgameGlob->shellshock.visionSetStarted )
      {
        cgameGlob->shellshock.visionSetStarted = 0;
        CG_VisionSetStartLerp_To(
          cgameGlob->localClientNum,
          VISIONSETMODE_NAKED,
          VISIONSETLERP_TO_SMOOTH,
          cgameGlob->visionNameNaked,
          parms->visionSet.outTime);
      }
    }
    else
    {
      cgameGlob->shellshock.visionSetStarted = 1;
      CG_VisionSetStartLerp_To(
        cgameGlob->localClientNum,
        VISIONSETMODE_NAKED,
        VISIONSETLERP_TO_SMOOTH,
        parms->visionSet.name,
        parms->visionSet.inTime);
    }
  }
  else if ( cgameGlob->shellshock.visionSetStarted )
  {
    cgameGlob->shellshock.visionSetStarted = 0;
    CG_VisionSetStartLerp_To(
      cgameGlob->localClientNum,
      VISIONSETMODE_NAKED,
      VISIONSETLERP_TO_SMOOTH,
      cgameGlob->visionNameNaked,
      3000);
  }
}

void __cdecl CG_StartShellShock(cg_s *cgameGlob, const shellshock_parms_t *parms, int start, int duration)
{
  cgameGlob->shellshock.parms = parms;
  cgameGlob->shellshock.startTime = start;
  cgameGlob->shellshock.duration = duration;
}

bool __cdecl CG_Flashbanged(int localClientNum)
{
  const cg_s *cgameGlob; // [esp+4h] [ebp-4h]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  return cgameGlob->shellshock.duration + cgameGlob->shellshock.startTime - cgameGlob->time > 0
      && cgameGlob->shellshock.parms->screenBlend.type != SHELLSHOCK_VIEWTYPE_BLURRED;
}

bool __cdecl CG_Flared(int localClientNum)
{
  const cg_s *cgameGlob; // [esp+14h] [ebp-4h]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  return (cgameGlob->snap->ps.perks[1] & 0x400) == 0 && cgameGlob->snap->ps.visionSetLerpRatio >= 0.25;
}

