#include "bg_vehicles_mp.h"

unsigned __int16 __cdecl BG_VehiclesGetSlotTagName(int seatIndex)
{
  return *s_seatTags[seatIndex];
}

