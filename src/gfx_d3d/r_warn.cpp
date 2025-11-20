#include "r_warn.h"

void R_WarnOncePerFrame(GfxWarningType warnType, ...)
{
  char message[1028]; // [esp+0h] [ebp-410h] BYREF
  float frameRate; // [esp+408h] [ebp-8h]
  char *vargs; // [esp+40Ch] [ebp-4h]
  va_list va; // [esp+41Ch] [ebp+Ch] BYREF

  va_start(va, warnType);
  if ( !r_warningRepeatDelay
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_warn.cpp", 163, 0, "%s", "r_warningRepeatDelay") )
  {
    __debugbreak();
  }
  frameRate = R_UpdateFrameRate();
  if ( s_warnCount[warnType] < rg.frontEndFrameCount )
  {
    s_warnCount[warnType] = rg.frontEndFrameCount + (int)(float)(frameRate * r_warningRepeatDelay->current.value);
    va_copy(vargs, va);
    _vsnprintf(message, 0x400u, s_warnFormat[warnType], va);
    vargs = 0;
    Com_PrintWarning(8, "%s", message);
  }
}

double __cdecl R_UpdateFrameRate()
{
  int frameTime; // [esp+0h] [ebp-8h]
  unsigned intcurrent; // [esp+4h] [ebp-4h]

  if ( frameCount != rg.frontEndFrameCount )
  {
    if ( frameCount )
    {
      if ( frameCount + 1 == rg.frontEndFrameCount )
      {
        current = Sys_Milliseconds();
        frameTime = current - previous;
        previous = current;
        if ( !frameTime )
          frameTime = 1;
        if ( frameTime >= 0 )
          frameRate = 1000.0 / (float)frameTime;
        else
          frameRate = *(float *)&FLOAT_0_0;
      }
      else
      {
        frameRate = *(float *)&FLOAT_0_0;
      }
    }
    else
    {
      previous = Sys_Milliseconds();
    }
    frameCount = rg.frontEndFrameCount;
  }
  return frameRate;
}

void __cdecl R_WarnInitDvars()
{
  r_warningRepeatDelay = _Dvar_RegisterFloat(
                           "r_warningRepeatDelay",
                           5.0,
                           0.0,
                           30.0,
                           0,
                           "Number of seconds after displaying a \"per-frame\" warning before it will display again");
}

