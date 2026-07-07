#include "cg_vehicles_mp.h"
#include <client_mp/cl_main_mp.h>
#include "cg_actors_mp.h"
#include "cg_ents_mp.h"
#include "cg_local_mp.h"
#include <cgame/cg_main.h>
#include <bgame/bg_vehicles_mp.h>

// Client vehicle helpers — CoDMPServer.c:L250927–251277, image RVAs 0x00513140–0x00513A50

//----- (00513140) --------------------------------------------------------
// Decomp: CoDMPServer.c:250927–250950  GetVehicleEntDObj
DObj *__cdecl GetVehicleEntDObj(int localClientNum, centity_s *centVeh)
{
    const cgs_t *cgs;
    if ( !centVeh->nextValid
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame_mp\\cg_vehicles_mp.cpp",
                    60,
                    0,
                    "%s",
                    "centVeh->nextValid") )
    {
        __debugbreak();
    }
    cgs = CG_GetLocalClientStaticGlobals(localClientNum);
    return CG_PreProcess_GetDObj(
                     localClientNum,
                     centVeh->nextState.number,
                     centVeh->nextState.eType,
                     cgs->gameModels[centVeh->nextState.un3.item],
                     0);
}

//----- (005131E0) --------------------------------------------------------
// Decomp: CoDMPServer.c:250953–250967  CG_VehEntityUsingVehicle
bool __cdecl CG_VehEntityUsingVehicle(int localClientNum, int entNum)
{
    centity_s *veh;
    clientInfo_t *ci;
    ci = ClientInfoForEntity(localClientNum, entNum);
    if ( !ci )
        return 0;
    if ( ci->attachedVehEntNum == 1023 || ci->attachedVehEntNum <= 44 )
        return 0;
    if ( (CG_GetEntity(localClientNum, entNum)->nextState.lerp.eFlags & 0x4000) == 0 )
        return 0;
    veh = CG_GetEntity(localClientNum, ci->attachedVehEntNum);
    return veh && veh->nextState.eType == ET_VEHICLE;
}

//----- (00513290) --------------------------------------------------------
// Decomp: CoDMPServer.c:250970–250994  ClientInfoForEntity
clientInfo_t *__cdecl ClientInfoForEntity(int localClientNum, int entNum)
{
    cg_s *cgameGlob;
    centity_s *cent;
    cent = CG_GetEntity(localClientNum, entNum);
    if ( cent->nextState.eType != ET_PLAYER )
        return 0;
    if ( cent->nextState.eType >= ET_EVENTS )
        return 0;
    if ( cent->nextState.clientNum >= 0x20u
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame_mp\\cg_vehicles_mp.cpp",
                    50,
                    0,
                    "cent->nextState.clientNum doesn't index MAX_CLIENTS\n\t%i not in [0, %i)",
                    cent->nextState.clientNum,
                    32) )
    {
        __debugbreak();
    }
    cgameGlob = CG_GetLocalClientGlobals(localClientNum);
    return &cgameGlob->bgs.clientinfo[cent->nextState.clientNum];
}

//----- (00513340) --------------------------------------------------------
// Decomp: CoDMPServer.c:250997–251017  CG_VehEntityUsingRemoteControlVehicle
bool __cdecl CG_VehEntityUsingRemoteControlVehicle(int localClientNum, int entNum)
{
    centity_s *veh;
    clientInfo_t *ci;
    ci = ClientInfoForEntity(localClientNum, entNum);
    if ( !ci )
        return 0;
    if ( ci->attachedVehEntNum == 1023 || ci->attachedVehEntNum <= 44 )
        return 0;
    if ( (CG_GetEntity(localClientNum, entNum)->nextState.lerp.eFlags & 0x4000) == 0 )
        return 0;
    veh = CG_GetEntity(localClientNum, ci->attachedVehEntNum);
    return veh
            && (veh->nextState.eType == ET_VEHICLE
                || veh->nextState.eType == ET_HELICOPTER
                || veh->nextState.eType == ET_PLANE)
            && CG_IsVehicleRemoteControl(veh->nextState.vehicleState.vehicleInfoIndex);
}

//----- (00513430) --------------------------------------------------------
// Decomp: CoDMPServer.c:251020–251023  CG_GetVehicleInfo
const vehicle_info_t *__cdecl CG_GetVehicleInfo(int index)
{
    return BG_GetVehicleInfo(index);
}

//----- (00513450) --------------------------------------------------------
// Decomp: CoDMPServer.c:251026–251054  CG_VehPlayerVehicleSeat
int __cdecl CG_VehPlayerVehicleSeat(int localClientNum, int entNum)
{
    clientInfo_t *ci;
    ci = ClientInfoForEntity(localClientNum, entNum);
    if ( !ci && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame_mp\\cg_vehicles_mp.cpp", 168, 0, "%s", "ci") )
        __debugbreak();
    if ( ci->attachedVehEntNum == 1023
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame_mp\\cg_vehicles_mp.cpp",
                    169,
                    0,
                    "%s",
                    "ci->attachedVehEntNum != ENTITYNUM_NONE") )
    {
        __debugbreak();
    }
    return ci->attachedVehSeat;
}

//----- (005134E0) --------------------------------------------------------
// Decomp: CoDMPServer.c:251057–251098  CG_VehSeatTransformForPlayer
void __cdecl CG_VehSeatTransformForPlayer(int localClientNum, int entNum, float *resultOrigin, float *resultAngles)
{
    clientInfo_t *ci;
    centity_s *centPlayer;
    ci = ClientInfoForEntity(localClientNum, entNum);
    centPlayer = CG_GetEntity(localClientNum, entNum);
    if ( centPlayer->nextState.eType != ET_PLAYER
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame_mp\\cg_vehicles_mp.cpp",
                    246,
                    0,
                    "%s",
                    "centPlayer->nextState.eType == ET_PLAYER") )
    {
        __debugbreak();
    }
    if ( centPlayer->nextState.eType >= ET_EVENTS
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame_mp\\cg_vehicles_mp.cpp",
                    247,
                    0,
                    "%s",
                    "centPlayer->nextState.eType < ET_EVENTS") )
    {
        __debugbreak();
    }
    if ( !ci->attachedVehEntNum
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame_mp\\cg_vehicles_mp.cpp",
                    248,
                    0,
                    "%s",
                    "ci->attachedVehEntNum != 0") )
    {
        __debugbreak();
    }
    SeatTransformForClientInfo(localClientNum, ci, resultOrigin, resultAngles);
}

//----- (005135D0) --------------------------------------------------------
// Decomp: CoDMPServer.c:251101–251126  SeatTransformForClientInfo
void __cdecl SeatTransformForClientInfo(int localClientNum, clientInfo_t *ci, float *resultOrigin, float *resultAngles)
{
    if ( !ci && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame_mp\\cg_vehicles_mp.cpp", 232, 0, "%s", "ci") )
        __debugbreak();
    if ( ci->attachedVehEntNum == 1023
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame_mp\\cg_vehicles_mp.cpp",
                    233,
                    0,
                    "%s",
                    "ci->attachedVehEntNum != ENTITYNUM_NONE") )
    {
        __debugbreak();
    }
    SeatTransformForSlot(localClientNum, ci->attachedVehEntNum, ci->attachedVehSeat, resultOrigin, resultAngles);
}

//----- (00513660) --------------------------------------------------------
// Decomp: CoDMPServer.c:251129–251147  SeatTransformForSlot
void __cdecl SeatTransformForSlot(
                int localClientNum,
                int vehEntNum,
                int vehSlotIdx,
                float *resultOrigin,
                float *resultAngles)
{
    unsigned __int16 tagName;
    float tagOrigin[3];
    float tagMtx[3][3];
    tagName = BG_VehiclesGetSlotTagName(vehSlotIdx);
    GetTagMatrix(localClientNum, vehEntNum, tagName, tagMtx, tagOrigin);
    if ( resultAngles )
        AxisToAngles(tagMtx, resultAngles);
    if ( resultOrigin )
    {
        *resultOrigin = tagOrigin[0];
        resultOrigin[1] = tagOrigin[1];
        resultOrigin[2] = tagOrigin[2];
    }
}

//----- (005136E0) --------------------------------------------------------
// Decomp: CoDMPServer.c:251150–251202  GetTagMatrix
void __cdecl GetTagMatrix(
                int localClientNum,
                int vehEntNum,
                unsigned __int16 tagName,
                float (*resultTagMat)[3],
                float *resultOrigin)
{
    centity_s *centVeh;
    DObj *objVeh;
    centVeh = CG_GetEntity(localClientNum, vehEntNum);
    if ( centVeh->nextState.eType != ET_VEHICLE
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame_mp\\cg_vehicles_mp.cpp",
                    180,
                    0,
                    "%s",
                    "centVeh->nextState.eType == ET_VEHICLE") )
    {
        __debugbreak();
    }
    if ( centVeh->nextState.eType == ET_VEHICLE )
    {
        if ( centVeh->nextValid )
        {
            objVeh = GetVehicleEntDObj(localClientNum, centVeh);
            if ( objVeh )
            {
                CG_DObjGetWorldTagMatrix(&centVeh->pose, objVeh, tagName, resultTagMat, resultOrigin);
            }
            else
            {
                Com_PrintWarning(14, "GetTagMatrix() unable to get vehicle DObj.\n");
                MatrixIdentity33(resultTagMat);
                *resultOrigin = 0.0f;
                resultOrigin[1] = 0.0f;
                resultOrigin[2] = 0.0f;
            }
        }
        else
        {
            Com_PrintWarning(14, "Vehicle being used by a player was not included in latest snapshot.\n");
            MatrixIdentity33(resultTagMat);
            *resultOrigin = 0.0f;
            resultOrigin[1] = 0.0f;
            resultOrigin[2] = 0.0f;
        }
    }
    else
    {
        Com_PrintWarning(14, "Calling GetTagMarix on an entity that is not a vehicle.\n");
        MatrixIdentity33(resultTagMat);
        *resultOrigin = 0.0f;
        resultOrigin[1] = 0.0f;
        resultOrigin[2] = 0.0f;
    }
}

//----- (00513880) --------------------------------------------------------
// Decomp: CoDMPServer.c:251205–251208  CG_VehGetHealthPercentageEntity
double __cdecl CG_VehGetHealthPercentageEntity(const centity_s *cent)
{
    return (double)(cent->nextState.time2 & 0x3F) / 63.0;
}

//----- (005138B0) --------------------------------------------------------
// Decomp: CoDMPServer.c:251211–251214  CG_VehGetHealthPercentageLeftTread
double __cdecl CG_VehGetHealthPercentageLeftTread(const centity_s *cent)
{
    return (double)((cent->nextState.time2 & 0xFC0) >> 6) / 63.0;
}

//----- (005138E0) --------------------------------------------------------
// Decomp: CoDMPServer.c:251217–251220  CG_VehGetHealthPercentageRightTread
double __cdecl CG_VehGetHealthPercentageRightTread(const centity_s *cent)
{
    return (double)((cent->nextState.time2 & 0x3F000) >> 12) / 63.0;
}

//----- (00513910) --------------------------------------------------------
// Decomp: CoDMPServer.c:251223–251226  CG_VehGetSeatOccupancyFlags
int __cdecl CG_VehGetSeatOccupancyFlags(const centity_s *cent)
{
    return (cent->nextState.time2 & 0xC0000) >> 18;
}

const int yawturn_masks[2] = { 1048576, 4194304 };
const int pitchturn_masks[2] = { 2097152, 8388608 };
const int overheating_shifts[2] = { 24, 25 };
const int overheating_masks[2] = { 16777216, 33554432 };

//----- (00513930) --------------------------------------------------------
// Decomp: CoDMPServer.c:251229–251243  CG_VehGetSeatGunTurningYaw
// Note: decomp shifts by yawturn_masks[seat] (nonsense); retail tests the single-bit masks directly.
bool __cdecl CG_VehGetSeatGunTurningYaw(const centity_s *cent, unsigned int seatIndex)
{
    if ( seatIndex >= 2
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame_mp\\cg_vehicles_mp.cpp",
                    316,
                    0,
                    "%s",
                    "seatIndex >= 0 && seatIndex < ( sizeof( _yawturn_masks ) / sizeof ( _yawturn_masks[ 0 ] ) )") )
    {
        __debugbreak();
    }
    return (yawturn_masks[seatIndex] & cent->nextState.time2) != 0;
}

//----- (00513990) --------------------------------------------------------
// Decomp: CoDMPServer.c:251246–251260  CG_VehGetSeatGunTurningPitch
// Note: decomp shifts by pitchturn_masks[seat] (nonsense); retail tests the single-bit masks directly.
bool __cdecl CG_VehGetSeatGunTurningPitch(const centity_s *cent, unsigned int seatIndex)
{
    if ( seatIndex >= 2
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame_mp\\cg_vehicles_mp.cpp",
                    322,
                    0,
                    "%s",
                    "seatIndex >= 0 && seatIndex < ( sizeof( _pitchturn_masks ) / sizeof ( _pitchturn_masks[ 0 ] ) )") )
    {
        __debugbreak();
    }
    return (pitchturn_masks[seatIndex] & cent->nextState.time2) != 0;
}

//----- (005139F0) --------------------------------------------------------
// Decomp: CoDMPServer.c:251263–251277  CG_VehGetSeatGunOverheating
bool __cdecl CG_VehGetSeatGunOverheating(const centity_s *cent, unsigned int seatIndex)
{
    if ( seatIndex >= 2
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame_mp\\cg_vehicles_mp.cpp",
                    328,
                    0,
                    "%s",
                    "seatIndex >= 0 && seatIndex < ( sizeof( _overheating_masks ) / sizeof ( _overheating_masks[ 0 ] ) )") )
    {
        __debugbreak();
    }
    return (overheating_masks[seatIndex] & cent->nextState.time2) >> overheating_shifts[seatIndex] != 0;
}
