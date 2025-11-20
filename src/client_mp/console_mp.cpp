#include "console_mp.h"

int __cdecl Live_GetNecessaryBandwidth()
{
  return band_dedicated->current.integer;
}

