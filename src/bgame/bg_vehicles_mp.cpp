#include "bg_vehicles_mp.h"
#include <clientscript/scr_const.h>
#include <game/g_scr_vehicle.h>

// Decomp: CoDMPServer.c:153396  (RVA 0044F080)
unsigned __int16 __cdecl BG_VehiclesGetSlotTagName(int seatIndex)
{
    return *s_seatTags[seatIndex];
}
