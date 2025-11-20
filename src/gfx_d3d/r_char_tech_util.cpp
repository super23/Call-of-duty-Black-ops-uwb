#include "r_char_tech_util.h"

char __cdecl TechLit_NeedsCharredTech(const GfxDrawSurf *drawSurf)
{
  unsigned int surfType; // [esp+8h] [ebp-8h]
  bool res; // [esp+Fh] [ebp-1h]

  surfType = (drawSurf->packed >> 51) & 0xF;
  res = 0;
  if ( r_use_separate_char_tech->current.enabled
    && (surfType == 9 || surfType == 7 || surfType == 2)
    && ((drawSurf->packed >> 20) & 2) != 0 )
  {
    return 1;
  }
  return res;
}

