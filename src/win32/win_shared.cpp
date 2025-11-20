#include "win_shared.h"

unsigned int__cdecl Sys_Milliseconds()
{
  if ( !initialized_1 )
  {
    sys_timeBase = timeGetTime();
    initialized_1 = 1;
  }
  return timeGetTime() - sys_timeBase;
}

unsigned int__cdecl Sys_MillisecondsRaw()
{
  return timeGetTime();
}

void __cdecl Sys_SnapVector(float *v)
{
  float *va; // [esp+10h] [ebp+8h]

  *v = (float)(int)*v;
  va = v + 1;
  *va = (float)(int)*va;
  va[1] = (float)(int)va[1];
}

