#include "r_sky_load_obj.h"

void __cdecl R_LoadSun(const char *name, sunflare_t *sun)
{
  char *v2; // eax
  const char *nameIter; // [esp+0h] [ebp-54h]
  char sunFile[68]; // [esp+4h] [ebp-50h] BYREF
  const char *firstCharToCopy; // [esp+4Ch] [ebp-8h]
  char *firstPeriod; // [esp+50h] [ebp-4h]

  if ( !name && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_sky_load_obj.cpp", 18, 0, "%s", "name") )
    __debugbreak();
  if ( !sun && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_sky_load_obj.cpp", 19, 0, "%s", "sun") )
    __debugbreak();
  Com_Memset((unsigned int *)sun, 0, 96);
  firstCharToCopy = name;
  for ( nameIter = name; *nameIter; ++nameIter )
  {
    if ( *nameIter == 47 || *nameIter == 92 )
      firstCharToCopy = nameIter + 1;
  }
  I_strncpyz(sunFile, firstCharToCopy, 64);
  strchr((unsigned __int8 *)sunFile, 0x2Eu);
  firstPeriod = v2;
  if ( v2 )
    *firstPeriod = 0;
  if ( sunFile[0] )
    R_LoadSunThroughDvars(sunFile, sun);
}

