#include "bg_vehicles_sp.h"
#include <game/g_scr_vehicle.h>

unsigned __int16 __cdecl BG_VehiclesGetSlotTagName(int seatIndex)
{
    return *s_seatTags[seatIndex];
}
