#include "fx_wind.h"

void __cdecl FX_SetGlobalWind(const FX_WindInfo *windInput)
{
  memcpy(&`anonymous namespace'::g_fxGlobalWind, windInput, sizeof(`anonymous namespace'::g_fxGlobalWind));
  `anonymous namespace'::g_fxGlobalWind.windVectorMagnitude = Abs(`anonymous namespace'::g_fxGlobalWind.windVector);
}

const FX_WindInfo *__cdecl FX_GetGlobalWind()
{
  return &`anonymous namespace'::g_fxGlobalWind;
}

