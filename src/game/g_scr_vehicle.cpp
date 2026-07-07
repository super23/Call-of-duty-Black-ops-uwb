#include "g_scr_vehicle.h"
#include <universal/q_shared.h>
#include <clientscript/cscr_vm.h>
#include <clientscript/scr_const.h>
#include <game/g_main_wrapper.h>
#include "g_weapon.h"
#include <game/g_utils_wrapper.h>
#ifdef KISAK_SP
#include <server_sp/sv_main_sp.h>
#else
#include <server_mp/sv_main_mp.h>
#endif
#include <bgame/bg_misc.h>
#include <clientscript/cscr_stringlist.h>
#include <server/sv_world.h>
#include <server/sv_game.h>
#include <universal/com_math_anglevectors.h>
#include <qcommon/dobj_management.h>
#include <xanim/dobj_utils.h>
#include <xanim/xmodel_utils.h>
#include "bullet.h"
#include <client/cl_debugdata.h>
#include <game/g_misc_wrapper.h>
#ifdef KISAK_SP
#include <client_sp/g_client_sp.h>
#else
#include <client_mp/g_client_mp.h>
#endif
#include <game/g_spawn_wrapper.h>
#include <universal/com_loadutils.h>
#include <bgame/bg_misctables.h>
#ifdef KISAK_SP
#include <server_sp/sv_init_sp.h>
#else
#include <server_mp/sv_init_mp.h>
#endif
#include <universal/CurveManager.h>
#include <physics/phys_assert.h>
#include <cgame/cg_drawtools.h>
#include "g_debug.h"
#include <game/g_active_wrapper.h>
#include "actor_script_cmd.h"
#include <qcommon/cm_world.h>
#include <glass/glass_server.h>
#include <universal/surfaceflags.h>
#include "g_helicopter1.h"
#include "g_scr_helicopter.h"
#include "g_mover.h"
#include "g_targets.h"
#include <game/g_combat_wrapper.h>
#include "g_load_utils.h"
#include <cgame/cg_scr_main.h>
#include <sound/snd_bank.h>
#include <cmath>
#include <cgame/cg_camera.h>

bool gVehicleRelativeGunnerAngles = true;

struct VehiclePhysicsBackup // sizeof=0x204
{                                       // XREF: .data:s_backup/r
    vehicle_pathpos_t pathPos;
    vehicle_physic_t phys;              // XREF: VEH_BackupPosition+F3/o
};
VehiclePhysicsBackup s_backup;

void __cdecl METHODS_NULLSUB(scr_entref_t entref)
{

}

// Looks congruent to retail blops mp latest
const BuiltinMethodDef s_methods_0[94] =
{
  { "attachpath", CMD_VEH_AttachPath, 0 },
  { "vehgetmodel", CMD_VEH_GetModel, 0 },
  { "getattachpos", CMD_VEH_GetAttachPos, 0 },
  { "startpath", CMD_VEH_StartPath, 0 },
  { "drivepath", CMD_VEH_DrivePath, 0 },
  { "setdrivepathphysicsscale", CMD_VEH_SetDrivePathPhysicsScale, 0 },
  { "setswitchnode", CMD_VEH_SetSwitchNode, 0 },
  { "setwaitnode", CMD_VEH_SetWaitNode, 0 },
  { "setwaitspeed", CMD_VEH_SetWaitSpeed, 0 },
  { "setspeed", CMD_VEH_SetSpeed, 0 },
  { "setspeedimmediate", CMD_VEH_SetSpeedImmediate, 0 },
  { "getspeed", CMD_VEH_GetSpeed, 0 },
  { "getspeedmph", CMD_VEH_GetSpeedMPH, 0 },
  { "getgoalspeedmph", CMD_VEH_GetGoalSpeedMPH, 0 },
  { "setbrake", CMD_VEH_SetBrake, 0 },
  { "setacceleration", CMD_VEH_SetAcceleration, 0 },
  { "setdeceleration", CMD_VEH_SetDeceleration, 0 },
  { "resumespeed", CMD_VEH_ResumeSpeed, 0 },
  { "setyawspeed", CMD_VEH_SetYawSpeed, 0 },
  { "setmaxpitchroll", CMD_VEH_SetMaxPitchRoll, 0 },
  { "setturningability", CMD_VEH_SetTurningAbility, 0 },
  { "setjitterparams", CMD_VEH_SetJitterParams, 0 },
  { "setairresistance", CMD_VEH_SetAirResitance, 0 },
  { "getsteerfactor", CMD_VEH_GetSteerFactor, 0 },
  { "getthrottle", CMD_VEH_GetThrottle, 0 },
  { "sethoverparams", CMD_VEH_SetHoverParams, 0 },
  { "joltbody", CMD_VEH_JoltBody, 0 },
  { "freevehicle", CMD_VEH_FreeVehicle, 0 },
  { "getwheelsurface", CMD_VEH_GetWheelSurface, 0 },
  { "getvehicleowner", CMD_VEH_GetVehicleOwner, 0 },
  { "startenginesound", CMD_VEH_StartEngineSound, 0 },
  { "stopenginesound", CMD_VEH_StopEngineSound, 0 },
  { "getenginevolume", CMD_VEH_GetEngineVolume, 0 },
  { "setenginevolume", CMD_VEH_SetEngineVolume, 0 },
  { "isvehicleusable", CMD_VEH_IsVehicleUsable, 0 },
  { "makevehicleusable", CMD_VEH_MakeVehicleUsable, 0 },
  { "makevehicleunusable", CMD_VEH_MakeVehicleUnusable, 0 },
  { "usevehicle", CMD_VEH_UseVehicle, 0 },
  { "setviewclamp", CMD_VEH_SetViewClamp, 0 },
  { "resetviewclamp", CMD_VEH_ResetViewClamp, 0 },
  { "setvehiclelookattext", CMD_VEH_SetVehicleLookatText, 0 },
  { "removevehiclefromcompass", CMD_VEH_RemoveVehicleFromCompass, 0 },
  { "addvehicletocompass", CMD_VEH_AddVehicleToCompass, 0 },
  { "setneargoalnotifydist", CMD_VEH_NearGoalNotifyDist, 0 },
  { "setvehgoalpos", CMD_VEH_SetGoalPos, 0 },
  { "clearvehgoalpos", CMD_VEH_ClearGoalPos, 0 },
  { "setplanegoalpos", CMD_VEH_SetPlaneGoalPos, 0 },
  { "setplanebarrelroll", CMD_VEH_SetPlaneBarrelRoll, 0 },
  { "setgoalyaw", CMD_VEH_SetGoalYaw, 0 },
  { "cleargoalyaw", CMD_VEH_ClearGoalYaw, 0 },
  { "settargetyaw", CMD_VEH_SetTargetYaw, 0 },
  { "cleartargetyaw", CMD_VEH_ClearTargetYaw, 0 },
  { "setlookatent", CMD_VEH_SetLookAtEnt, 0 },
  { "clearlookatent", CMD_VEH_ClearLookAtEnt, 0 },
  { "returnplayercontrol", CMD_VEH_ReturnPlayerControl, 0 },
  { "sethealthpercent", CMD_VEH_SetHealthPercent, 0 },
  { "setturrettargetvec", CMD_VEH_SetTurretTargetVec, 0 },
  { "setturrettargetent", CMD_VEH_SetTurretTargetEnt, 0 },
  { "clearturrettarget", CMD_VEH_ClearTurretTarget, 0 },
  { "setgunnertargetvec", CMD_VEH_SetGunnerTargetVec, 0 },
  { "getgunnertargetvec", CMD_VEH_GetGunnerTargetVec, 0 },
  { "setgunnertargetent", CMD_VEH_SetGunnerTargetEnt, 0 },
  { "cleargunnertarget", CMD_VEH_ClearGunnerTarget, 0 },
  { "setgunnerturretontargetrange", CMD_VEH_SetGunnerTurretOnTargetRange, 0 },
  { "getgunnertargetent", CMD_VEH_GetGunnerTargetEnt, 0 },
  { "getgunneranimpitch", CMD_VEH_GetGunnerAnimPitch, 0 },
  { "getgunneranimyaw", CMD_VEH_GetGunnerAnimYaw, 0 },
  { "setvehweapon", CMD_VEH_SetWeapon, 0 },
  { "fireweapon", CMD_VEH_FireWeapon, 0 },
  { "firegunnerweapon", CMD_VEH_FireGunnerWeapon, 0 },
  { "stopfireweapon", CMD_VEH_StopFireWeapon, 0 },
  { "isturretready", CMD_VEH_IsTurretReady, 0 },
  { "vehforcematerialspeed", CMD_VEH_ForceMaterialSpeed, 0 },
  { "setvehmaxspeed", CMD_VEH_SetMaxSpeed, 0 },
  { "getvehoccupants", CMD_VEH_GetOccupants, 0 },
  { "getseatoccupant", CMD_VEH_GetSeatOccupant, 0 },
  { "getoccupantseat", CMD_VEH_GetOccupantSeat, 0 },
  { "setseatoccupied", CMD_VEH_SetSeatOccupied, 0 },
  { "seatgetweapon", CMD_VEH_SeatGetWeapon, 0 },
  { "getseatfiringorigin", CMD_VEH_GetSeatFiringOrigin, 0 },
  { "getseatfiringangles", CMD_VEH_GetSeatFiringAngles, 0 },
  { "isgunnerfiring", CMD_VEH_IsGunnerFiring, 0 },
  { "disablegunnerfiring", CMD_VEH_DisableGunnerFiring, 0 },
  { "finishvehicledamage", CMD_VEH_finishVehicleDamage, 0 },
  { "finishvehicleradiusdamage", CMD_VEH_finishVehicleRadiusDamage, 0 },
  { "isvehicleimmunetodamage", CMD_VEH_IsVehicleImmuneToDamage, 0 },
  { "setdefaultpitch", CMD_VEH_SetDefaultPitch, 0 },
  { "cleardefaultpitch", CMD_VEH_ClearDefaultPitch, 0 },
  { "getangularvelocity", CMD_VEH_GetAngularVelocity, 0 },
  { "setangularvelocity", CMD_VEH_SetAngularVelocity, 0 },
  { "setvehvelocity", CMD_VEH_SetVehVelocity, 0 },
  { "gettreadhealth", CMD_VEH_GetTreadHealth, 0 },
  { "getboost", CMD_VEH_GetBoost, 0 },
  { "isboosting", CMD_VEH_IsBoosting, 0 }
};

unsigned __int16 *s_wheelTags[6] =
{
    &scr_const.tag_wheel_front_left,
    &scr_const.tag_wheel_front_right,
    &scr_const.tag_wheel_back_left,
    &scr_const.tag_wheel_back_right,
    &scr_const.tag_wheel_middle_left,
    &scr_const.tag_wheel_middle_right,
};

unsigned __int16 *s_flashTags[5] =
{
    &scr_const.tag_flash,
    &scr_const.tag_flash_11,
    &scr_const.tag_flash_2,
    &scr_const.tag_flash_22,
    &scr_const.tag_flash_3,
};

unsigned __int16 *s_gunnerBarrelTags[4] =
{
    &scr_const.tag_gunner_barrel1,
    &scr_const.tag_gunner_barrel2,
    &scr_const.tag_gunner_barrel3,
    &scr_const.tag_gunner_barrel4,
};

unsigned __int16 *s_gunnerTurretTags[4] =
{
    &scr_const.tag_gunner_turret1,
    &scr_const.tag_gunner_turret2,
    &scr_const.tag_gunner_turret3,
    &scr_const.tag_gunner_turret4,
};

unsigned __int16 *s_gunnerFlashTags[8] =
{
    &scr_const.tag_flash_gunner[0],
    &scr_const.tag_flash_gunner[1],
    &scr_const.tag_flash_gunner[2],
    &scr_const.tag_flash_gunner[3],
    &scr_const.tag_flash_gunner[4],
    &scr_const.tag_flash_gunner[5],
    &scr_const.tag_flash_gunner[6],
    &scr_const.tag_flash_gunner[7],
};

// just the above at offset 4-8
//unsigned __int16 *s_gunnerFlashTags2[4] =
//{
//    &scr_const.tag_flash_gunner
//};

const unsigned __int16 *s_seatTags[11] =
{
    &scr_const.tag_driver,
    &scr_const.tag_gunner1,
    &scr_const.tag_gunner2,
    &scr_const.tag_gunner3,
    &scr_const.tag_gunner4,
    &scr_const.tag_passenger1,
    &scr_const.tag_passenger2,
    &scr_const.tag_passenger3,
    &scr_const.tag_passenger4,
    &scr_const.tag_passenger5,
    &scr_const.tag_passenger6,
};

const unsigned __int16 *s_entryPointTags[5] =
{
    &scr_const.tag_enter_left,
    &scr_const.tag_enter_right,
    &scr_const.tag_enter_back,
    &scr_const.tag_enter_gunner3,
    &scr_const.tag_enter_gunner4,
};

const unsigned __int16 *s_entryPointOldTags[5] =
{
    &scr_const.tag_enter_driver,
    &scr_const.tag_enter_gunner1,
    &scr_const.tag_enter_gunner2,
    &scr_const.tag_enter_gunner3,
    &scr_const.tag_enter_gunner4,
};


// *WARNING* One or more selections were skipped as they could not be interpreted as c data


float exitPointOffsetDown = 30.0;
float exitPointOffsetUp = 10.0;



const float s_correctSolidDeltas[26][3] =
{
  { 0.0, 0.0, 1.0 },
  { -1.0, 0.0, 1.0 },
  { 0.0, -1.0, 1.0 },
  { 1.0, 0.0, 1.0 },
  { 0.0, 1.0, 1.0 },
  { -1.0, 0.0, 0.0 },
  { 0.0, -1.0, 0.0 },
  { 1.0, 0.0, 0.0 },
  { 0.0, 1.0, 0.0 },
  { 0.0, 0.0, -1.0 },
  { -1.0, 0.0, -1.0 },
  { 0.0, -1.0, -1.0 },
  { 1.0, 0.0, -1.0 },
  { 0.0, 1.0, -1.0 },
  { -1.0, -1.0, 1.0 },
  { 1.0, -1.0, 1.0 },
  { 1.0, 1.0, 1.0 },
  { -1.0, 1.0, 1.0 },
  { -1.0, -1.0, 0.0 },
  { 1.0, -1.0, 0.0 },
  { 1.0, 1.0, 0.0 },
  { -1.0, 1.0, 0.0 },
  { -1.0, -1.0, -1.0 },
  { 1.0, -1.0, -1.0 },
  { 1.0, 1.0, -1.0 },
  { -1.0, 1.0, -1.0 }
};


scr_vehicle_s s_vehicles[MAX_VEHICLES];
int bg_numVehicleInfos;
vehicle_info_t bg_vehicleInfos[32];

float s_rolls[1];
float s_pitches[1];

struct VehicleLocalPhysics // sizeof=0x40
{                                       // XREF: .data:s_phys/r
    trace_t groundTrace;                // XREF: VEH_SlideMove(gentity_s *,int)+131/r
    int hasGround;                      // XREF: VEH_ClearGround(gentity_s *)+3/w
    int onGround;                       // XREF: VEH_UpdateClient+36D/r
} s_phys;

// Decomp: CoDMPServer.c:427433  BG_GetVehicleInfo
const vehicle_info_t *__cdecl BG_GetVehicleInfo(int index)
{
    if ( index < 0
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 529, 0, "%s", "index >= 0") )
    {
        __debugbreak();
    }
    if ( index >= 32
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    530,
                    0,
                    "%s",
                    "index < MAX_VEHICLE_FILES") )
    {
        __debugbreak();
    }
    return &bg_vehicleInfos[index];
}

// Decomp: CoDMPServer.c:427461  GScr_GetVehicle
gentity_s *__cdecl GScr_GetVehicle(scr_entref_t entref)
{
    if ( !entref.classnum )
        return VEH_GetVehicle(entref.entnum);
    Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
    return 0;
}

// Decomp: CoDMPServer.c:427470  VEH_GetVehicle
gentity_s *__cdecl VEH_GetVehicle(int entNum)
{
    const char *errorMsg;

    if ( entNum >= 1024
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    589,
                    0,
                    "%s",
                    "entNum < MAX_GENTITIES") )
    {
        __debugbreak();
    }
    if ( g_entities[entNum].classname != scr_const.script_vehicle )
    {
        errorMsg = va("entity %i is not a script_vehicle\n", entNum);
        Scr_Error(errorMsg, 0);
    }
    if ( !g_entities[entNum].scr_vehicle )
    {
        errorMsg = va("entity %i doesn't have a script_vehicle\n", entNum);
        Scr_Error(errorMsg, 0);
    }
    return &g_entities[entNum];
}

// Decomp: CoDMPServer.c:427500  VEH_IsSeatPresent
int __cdecl VEH_IsSeatPresent(gentity_s *ent, int seatIdx, const vehicle_info_t *info)
{
    if ( !info )
        info = BG_GetVehicleInfo(ent->scr_vehicle->infoIdx);
    if ( seatIdx < 1 )
        return info->isDrivable;
    if ( seatIdx < info->numberOfGunners + 1 )
        return info->driverControlledGunPos + 1 != seatIdx;
    return seatIdx >= 5 && seatIdx < info->numberOfSeats - info->numberOfGunners - info->isDrivable + 5;
}

// Decomp: CoDMPServer.c:427514  VEH_GetSeatWeaponIndex
int __cdecl VEH_GetSeatWeaponIndex(gentity_s *const vehicle, int seatIndex)
{
    const vehicle_info_t *info; // [esp+8h] [ebp-4h]

    info = BG_GetVehicleInfo(vehicle->scr_vehicle->infoIdx);
    if ( !VEH_IsSeatPresent(vehicle, seatIndex, info) )
        return 0;
    if ( !seatIndex )
        return G_GetWeaponIndexForName((char*)info->turretWeapon);
    if ( seatIndex - 1 >= info->numberOfGunners )
        return 0;
    else
        return G_GetWeaponIndexForName((char *)info->gunnerWeapon[seatIndex - 1]);
}

// Decomp: CoDMPServer.c:427532  VEH_GetSeatFiringOriginAngles
int __cdecl VEH_GetSeatFiringOriginAngles(gentity_s *const vehEnt, int seatIndex, float *origin, float *angles)
{
    const VehicleTurret *turret;
    scr_vehicle_s *veh;
    float flashMatrix[4][3];
    float forward[3];
    int flashBoneIndex;
    bool useAltFlash;

    if ( !VEH_GetSeatWeaponIndex(vehEnt, seatIndex) )
        return 0;
    veh = vehEnt->scr_vehicle;
    if ( seatIndex )
        turret = &veh->gunnerTurrets[seatIndex - 1];
    else
        turret = &veh->turret;
    useAltFlash = (turret->flags & 2) != 0 && !turret->fireBarrel;
    if ( seatIndex )
    {
        if ( useAltFlash )
            flashBoneIndex = veh->boneIndex.gunnerTags[seatIndex - 1].flash2;
        else
            flashBoneIndex = veh->boneIndex.gunnerTags[seatIndex - 1].flash;
    }
    else
        flashBoneIndex = veh->boneIndex.flash[useAltFlash];
    if ( flashBoneIndex < 0 )
        return 0;
    G_DObjGetWorldBoneIndexMatrix(vehEnt, flashBoneIndex, flashMatrix);
    forward[0] = flashMatrix[0][0];
    forward[1] = flashMatrix[0][1];
    forward[2] = flashMatrix[0][2];
    vectoangles(forward, angles);
    *origin = flashMatrix[3][0];
    origin[1] = flashMatrix[3][1];
    origin[2] = flashMatrix[3][2];
    return 1;
}

// Decomp: CoDMPServer.c:427587  VEH_GetSeat
VehicleSeat *__cdecl VEH_GetSeat(scr_vehicle_s *veh, int seatIdx)
{
    if ( !veh && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 731, 0, "%s", "veh") )
        __debugbreak();
    return &veh->seats[seatIdx];
}

// Decomp: CoDMPServer.c:427604  VEH_GetSeat
VehicleSeat *__cdecl VEH_GetSeat(gentity_s *vehEnt, int seatIdx)
{
    scr_vehicle_s *veh; // [esp+4h] [ebp-4h]

    veh = GetVehicleLogObjectError(vehEnt);
    if ( veh )
        return VEH_GetSeat(veh, seatIdx);
    else
        return 0;
}

// Decomp: CoDMPServer.c:427618  GetVehicleLogObjectError
scr_vehicle_s *__cdecl GetVehicleLogObjectError(gentity_s *ent)
{
    scr_vehicle_s *veh; // [esp+0h] [ebp-4h]

    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 717, 0, "%s", "ent") )
        __debugbreak();
    veh = ent->scr_vehicle;
    if ( !veh )
        Scr_ObjectError("VEH_GetSeat(): Entity not a vehicle", SCRIPTINSTANCE_SERVER);
    return veh;
}

// Decomp: CoDMPServer.c:427664  VEH_GetSeatOccupantEntNum
// Decomp: CoDMPServer.c:427640  VEH_GetSeatOccupantEntNum
int __cdecl VEH_GetSeatOccupantEntNum(scr_vehicle_s *veh, int seatIdx)
{
    const VehicleSeat *seat; // [esp+0h] [ebp-4h]

    seat = VEH_GetSeat(veh, seatIdx);
    if ( !seat )
        return 1023;
    if ( seat->_occupantEntNum >= 0x400u
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    750,
                    0,
                    "%s",
                    "seat->_occupantEntNum == ENTITYNUM_NONE || ( seat->_occupantEntNum >= 0 && seat->_occupantEntNum < MAX_GENTITIES )") )
    {
        __debugbreak();
    }
    return seat->_occupantEntNum;
}

int __cdecl VEH_GetSeatOccupantEntNum(gentity_s *vehEnt, int seatIdx)
{
    scr_vehicle_s *veh; // [esp+4h] [ebp-4h]

    veh = GetVehicleLogObjectError(vehEnt);
    if ( veh )
        return VEH_GetSeatOccupantEntNum(veh, seatIdx);
    else
        return 1023;
}

// Decomp: CoDMPServer.c:427678  VEH_SetSeatOccupantEntNum
int __cdecl VEH_SetSeatOccupantEntNum(scr_vehicle_s *veh, int seatIdx, unsigned int occEntNum)
{
    int oldOccEntNum; // [esp+0h] [ebp-8h]
    VehicleSeat *seat; // [esp+4h] [ebp-4h]

    if ( occEntNum >= 0x400
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    765,
                    0,
                    "%s",
                    "occEntNum == ENTITYNUM_NONE || ( occEntNum >= 0 && occEntNum < MAX_GENTITIES )") )
    {
        __debugbreak();
    }
    seat = VEH_GetSeat(veh, seatIdx);
    if ( seat )
    {
        oldOccEntNum = seat->_occupantEntNum;
        seat->_occupantEntNum = occEntNum;
        return oldOccEntNum;
    }
    else
    {
        Scr_ObjectError("VEH_SetSeatOccupantEntNum(): Invalid vehicle/seat specification", SCRIPTINSTANCE_SERVER);
        return 1023;
    }
}

// Decomp: CoDMPServer.c:427726  VEH_GetSeatOccupantEntity
// Decomp: CoDMPServer.c:427712  VEH_GetSeatOccupantEntity
gentity_s *__cdecl VEH_GetSeatOccupantEntity(scr_vehicle_s *veh, int seatIdx)
{
    int occEntNum; // [esp+4h] [ebp-4h]

    occEntNum = VEH_GetSeatOccupantEntNum(veh, seatIdx);
    if ( occEntNum == 1023 || occEntNum == 1022 )
        return 0;
    else
        return &g_entities[occEntNum];
}

gentity_s *__cdecl VEH_GetSeatOccupantEntity(gentity_s *vehEnt, int seatIdx)
{
    scr_vehicle_s *veh; // [esp+4h] [ebp-4h]

    veh = GetVehicleLogObjectError(vehEnt);
    if ( veh )
        return VEH_GetSeatOccupantEntity(veh, seatIdx);
    else
        return 0;
}

// Decomp: CoDMPServer.c:427740  VEH_SetPosition
void __cdecl VEH_SetPosition(gentity_s *ent, const float *origin, float *vel, const float *angles)
{
    const char *errorMsg;
    char *targetName;
    float snappedVel[3];
    float prevVel[3];
    float snappedOrigin[3];
    bool originUnchanged;
    bool posTrBaseMatches;
    bool anglesUnchanged;
    bool aposTrBaseMatches;

    snappedOrigin[0] = *origin;
    snappedOrigin[1] = origin[1];
    snappedOrigin[2] = origin[2];
    snappedVel[0] = *vel;
    snappedVel[1] = vel[1];
    snappedVel[2] = vel[2];
    prevVel[0] = ent->s.lerp.pos.trDelta[0];
    prevVel[1] = ent->s.lerp.pos.trDelta[1];
    prevVel[2] = ent->s.lerp.pos.trDelta[2];
    snappedOrigin[0] = (float)(int)snappedOrigin[0];
    snappedOrigin[1] = (float)(int)snappedOrigin[1];
    snappedOrigin[2] = (float)(int)snappedOrigin[2];
    snappedVel[0] = (float)(int)snappedVel[0];
    snappedVel[1] = (float)(int)snappedVel[1];
    snappedVel[2] = (float)(int)snappedVel[2];
    if ( !BG_ValidateOriginValue(snappedOrigin[2], 17, svs.mapCenter[2]) )
    {
        if ( ent->targetname )
            targetName = SL_ConvertToString(ent->targetname, SCRIPTINSTANCE_SERVER);
        else
            targetName = (char *)"<undefined>";
        errorMsg = va(
            "Vehicle %s has fallen out of the world at (%0.2f, %0.2f, %0.2f)\n",
            targetName,
            *origin,
            origin[1],
            origin[2]);
        Scr_Error(errorMsg, 0);
    }
    originUnchanged = ent->r.currentOrigin[0] == snappedOrigin[0]
        && ent->r.currentOrigin[1] == snappedOrigin[1]
        && ent->r.currentOrigin[2] == snappedOrigin[2];
    posTrBaseMatches = ent->s.lerp.pos.trBase[0] == snappedOrigin[0]
        && ent->s.lerp.pos.trBase[1] == snappedOrigin[1]
        && ent->s.lerp.pos.trBase[2] == snappedOrigin[2];
    anglesUnchanged = ent->r.currentAngles[0] == *angles
        && ent->r.currentAngles[1] == angles[1]
        && ent->r.currentAngles[2] == angles[2];
    aposTrBaseMatches = ent->s.lerp.apos.trBase[0] == *angles
        && ent->s.lerp.apos.trBase[1] == angles[1]
        && ent->s.lerp.apos.trBase[2] == angles[2];
    if ( originUnchanged && posTrBaseMatches && anglesUnchanged && aposTrBaseMatches )
    {
        if ( prevVel[0] != snappedVel[0] || prevVel[1] != snappedVel[1] || prevVel[2] != snappedVel[2] )
        {
            ent->s.lerp.pos.trDelta[0] = snappedVel[0];
            ent->s.lerp.pos.trDelta[1] = snappedVel[1];
            ent->s.lerp.pos.trDelta[2] = snappedVel[2];
        }
    }
    else
    {
        G_SetOrigin(ent, snappedOrigin);
        G_SetAngle(ent, angles);
        ent->s.lerp.pos.trType = 1;
        ent->s.lerp.apos.trType = 1;
        ent->s.lerp.pos.trDelta[0] = snappedVel[0];
        ent->s.lerp.pos.trDelta[1] = snappedVel[1];
        ent->s.lerp.pos.trDelta[2] = snappedVel[2];
        SV_LinkEntity(ent);
    }
}

// Decomp: CoDMPServer.c:427828  VEH_InitEntity
void __cdecl VEH_InitEntity(gentity_s *ent, scr_vehicle_s *veh, int infoIdx)
{
    int WeaponIndexForName; // eax
    float gunPitchQuantized;
    float gunYawQuantized;
    float gunnerPitchQuantized;
    float gunnerYawQuantized;
    int gunnerAngleIndex;
    const vehicle_info_t *info; // [esp+20h] [ebp-8h]

    info = BG_GetVehicleInfo(infoIdx);
    ent->handler = 25;
    ent->r.svFlags = 4;
    ent->r.contents = 8462464;
    if ( (ent->spawnflags & 1) != 0 )
        ent->r.contents |= 0x200000u;
    ent->s.eType = ET_VEHICLE;
    if ( (ent->s.lerp.eFlags & 0x20000) != 0 )
        ent->s.lerp.eFlags |= 0x20000u;
    ent->s.lerp.pos.trType = 1;
    ent->s.lerp.apos.trType = 1;
    ent->s.time2 = 0;
    ent->s.loopSoundId = 0;
    ent->s.loopSoundFade = 0;
    if ( !g_connectpaths->current.integer )
    {
        WeaponIndexForName = G_GetWeaponIndexForName((char*)info->turretWeapon);
        AssignToSmallerType<unsigned short>(&ent->s.weapon, WeaponIndexForName);
    }
    ent->s.weaponModel = 0;
    ent->s.lerp.u.actor.actorNum = 0;
    ent->s.lerp.u.vehicle.drawOnCompass = 0;
    ent->s.lerp.u.vehicle.throttle = (int)floor(0.0 * 182.04445 + 0.5);
    ent->s.lerp.u.loopFx.period = 0;
    gunPitchQuantized = floor(0.0 * 182.04445 + 0.5);
    ent->s.lerp.u.vehicle.gunPitch = (int)gunPitchQuantized;
    gunYawQuantized = floor(0.0 * 182.04445 + 0.5);
    ent->s.lerp.u.vehicle.gunYaw = (int)gunYawQuantized;
    for ( gunnerAngleIndex = 0; gunnerAngleIndex < 4; ++gunnerAngleIndex )
    {
        gunnerPitchQuantized = floor(0.0 * 182.04445 + 0.5);
        ent->s.lerp.u.vehicle.gunnerAngles[gunnerAngleIndex].pitch = (int)gunnerPitchQuantized;
        gunnerYawQuantized = floor(0.0 * 182.04445 + 0.5);
        ent->s.lerp.u.vehicle.gunnerAngles[gunnerAngleIndex].yaw = (int)gunnerYawQuantized;
    }
    ent->s.vehicleState.vehicleInfoIndex = infoIdx;
    ent->scr_vehicle = veh;
    ent->nextthink = level.time + 50;
    ent->takedamage = 1;
    ent->active = 1;
    ent->clipmask = 8454673;
    ent->flags |= 0x800u;
    SV_DObjGetBounds(ent, ent->r.mins, ent->r.maxs);
    SV_LinkEntity(ent);
}

// Decomp: CoDMPServer.c:427889  VEH_InitVehicle
void __cdecl VEH_InitVehicle(gentity_s *ent, scr_vehicle_s *veh, int infoIdx)
{
    int WeaponIndexForName; // eax
    int gunnerIndex;
    const vehicle_info_t *info; // [esp+10h] [ebp-8h]
    int seatIndex; // [esp+14h] [ebp-4h]

    info = BG_GetVehicleInfo(infoIdx);
    veh->infoIdx = infoIdx;
    veh->entNum = ent->s.number;
    veh->moveState = VEH_MOVESTATE_STOP;
    veh->flags = 0;
    veh->waitNode = -1;
    veh->waitSpeed = -1.0f;
    veh->turret.fireTime = 0;
    veh->turret.fireBarrel = 0;
    veh->turret.turretState = VEH_TURRET_STOPPED;
    veh->turret.barrelOffset = 0.0f;
    veh->turret.flags = 0;
    veh->turret.heatVal = 0.0f;
    veh->turret.overheating = 0;
    veh->turretRotScale = 1.0f;
    Com_Memset((unsigned int *)&veh->jitter, 0, 60);
    veh->lookAtText0 = 0;
    veh->lookAtText1 = 0;
    veh->manualMode = 0;
    veh->manualTime = 0.0f;
    veh->speed = 0.0f;
    veh->maxSpeedOverride = 0.0f;
    veh->maxDragSpeed = 60.0 * 17.6;
    veh->turningAbility = 0.5f;
    veh->pathTransitionTime = 0.0f;
    veh->hasTarget = 0;
    veh->targetEnt = 1023;

    iassert(!veh->lookAtEnt.isDefined());

    veh->targetOrigin[0] = 0.0f;
    veh->targetOrigin[1] = 0.0f;
    veh->targetOrigin[2] = 0.0f;
    veh->targetOffset[0] = 0.0f;
    veh->targetOffset[1] = 0.0f;
    veh->targetOffset[2] = 0.0f;
    veh->joltDir[0] = 0.0f;
    veh->joltDir[1] = 0.0f;
    veh->joltTime = 0.0f;
    veh->joltWave = 0.0f;
    veh->joltSpeed = 0.0f;
    veh->joltDecel = 0.0f;
    veh->turretHitNum = 0;
    veh->hover.hoverRadius = 30.0f;
    veh->hover.hoverSpeed = 10.0 * 17.6;
    veh->hover.hoverAccel = 5.0 * 17.6;
    veh->hover.useHoverAccelForAngles = 0;
    veh->yawSlowDown = 0;
    veh->yawOverShoot = 0.1f;
    veh->prevGoalYaw = -1.0f;
    veh->nearGoalNotifyDist = 0.0f;
    veh->modelSwapDelay = info->modelSwapDelay;
    veh->oneExhaust = info->oneExhaust;
    veh->deathQuakeScale = info->deathQuakeScale;
    veh->deathQuakeDuration = info->deathQuakeDuration;
    veh->deathQuakeRadius = info->deathQuakeRadius;
    veh->secTurretAiControlled = info->secTurretAiControlled;
    veh->frontArmorRegen = info->frontArmorRegen;
    veh->addToCompass = info->addToCompass;
    veh->m_bSpecialAbilityEventDown = 0;
    veh->m_bFirePickupEventDown = 0;
    veh->m_bSwapPickupEventDown = 0;
    veh->m_bDropDeployableEventDown = 0;
    for ( seatIndex = 0; seatIndex < 11; ++seatIndex )
        VEH_InitSeat(veh, seatIndex);
    for ( gunnerIndex = 0; gunnerIndex < 4; ++gunnerIndex )
    {
        if ( !g_connectpaths->current.integer && !info->gunnerWeaponIndex[gunnerIndex] )
        {
            WeaponIndexForName = G_GetWeaponIndexForName((char*)info->gunnerWeapon[gunnerIndex]);
            AssignToSmallerType<unsigned short>(&bg_vehicleInfos[infoIdx].gunnerWeaponIndex[gunnerIndex], WeaponIndexForName);
        }
        veh->gunnerTargets[gunnerIndex].targetEnt = 1023;
        veh->gunnerTurrets[gunnerIndex].heatVal = 0.0f;
        veh->gunnerTurrets[gunnerIndex].overheating = 0;
    }
    VEH_SetPosition(ent, ent->r.currentOrigin, (float *)vec3_origin, ent->r.currentAngles);
    VEH_InitPhysics(ent, infoIdx);
    VEH_InitFromInfo(veh, info);
}

// Decomp: CoDMPServer.c:427992  VEH_InitSeat
void __cdecl VEH_InitSeat(scr_vehicle_s *veh, int seatIdx)
{
    VehicleSeat *seat; // [esp+0h] [ebp-4h]

    seat = VEH_GetSeat(veh, seatIdx);
    if ( seat )
        seat->_occupantEntNum = 1023;
}

// Decomp: CoDMPServer.c:428002  VEH_InitFromInfo
void __cdecl VEH_InitFromInfo(scr_vehicle_s *veh, const vehicle_info_t *info)
{
    veh->team = info->eTeam;
    veh->phys.maxPitchAngle = info->maxBodyPitch;
    veh->phys.maxRollAngle = info->maxBodyRoll;
    veh->phys.maxAngleVel[1] = info->rotRate;
    veh->phys.maxAngleVel[0] = info->rotRate * 0.5;
    veh->phys.maxAngleVel[2] = info->rotRate * 0.5;
    veh->phys.yawAccel = info->rotAccel;
    veh->phys.yawDecel = info->rotAccel * 0.64999998;
    veh->manualSpeed = info->maxSpeed;
    veh->manualAccel = info->accel;
    veh->manualDecel = info->accel;
}

// Decomp: CoDMPServer.c:428021  VEH_InitPhysics
void __cdecl VEH_InitPhysics(gentity_s *ent, int infoIdx)
{
    vehicle_physic_t *phys; // [esp+38h] [ebp-10h]
    const vehicle_info_t *info; // [esp+3Ch] [ebp-Ch]
    scr_vehicle_s *veh; // [esp+40h] [ebp-8h]
    int wheelIndex;

    veh = ent->scr_vehicle;
    phys = &veh->phys;
    veh->phys.origin[0] = ent->r.currentOrigin[0];
    veh->phys.origin[1] = ent->r.currentOrigin[1];
    veh->phys.origin[2] = ent->r.currentOrigin[2];
    veh->phys.prevOrigin[0] = ent->r.currentOrigin[0];
    veh->phys.prevOrigin[1] = ent->r.currentOrigin[1];
    veh->phys.prevOrigin[2] = ent->r.currentOrigin[2];
    veh->phys.angles[0] = ent->r.currentAngles[0];
    veh->phys.angles[1] = ent->r.currentAngles[1];
    veh->phys.angles[2] = ent->r.currentAngles[2];
    veh->phys.prevAngles[0] = ent->r.currentAngles[0];
    veh->phys.prevAngles[1] = ent->r.currentAngles[1];
    veh->phys.prevAngles[2] = ent->r.currentAngles[2];
    veh->phys.mins[0] = 0.0f;
    veh->phys.mins[1] = 0.0f;
    veh->phys.mins[2] = 0.0f;
    veh->phys.maxs[0] = 0.0f;
    veh->phys.maxs[1] = 0.0f;
    veh->phys.maxs[2] = 0.0f;
    veh->phys.vel[0] = 0.0f;
    veh->phys.vel[1] = 0.0f;
    veh->phys.vel[2] = 0.0f;
    veh->phys.bodyVel[0] = 0.0f;
    veh->phys.bodyVel[1] = 0.0f;
    veh->phys.bodyVel[2] = 0.0f;
    veh->phys.rotVel[0] = 0.0f;
    veh->phys.rotVel[1] = 0.0f;
    veh->phys.rotVel[2] = 0.0f;
    for ( wheelIndex = 0; wheelIndex < 6; ++wheelIndex )
    {
        phys->wheelZVel[wheelIndex] = 0.0f;
        phys->wheelZPos[wheelIndex] = 0.0f;
        phys->wheelSurfType[wheelIndex] = 0;
    }
    veh->phys.worldTilt[0] = 0.0f;
    veh->phys.worldTilt[1] = 0.0f;
    veh->phys.worldTilt[2] = 0.0f;
    veh->phys.worldTiltVel[0] = 0.0f;
    veh->phys.worldTiltVel[1] = 0.0f;
    veh->phys.worldTiltVel[2] = 0.0f;
    info = BG_GetVehicleInfo(infoIdx);
    if ( info->isNitrous )
    {
        Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
        Sys_EnterCriticalSection(CRITSECT_PHYSICS);
        if ( veh->nitrousVehicle
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                        1259,
                        0,
                        "%s",
                        "!veh->nitrousVehicle") )
        {
            __debugbreak();
        }
        veh->nitrousVehicle = (NitrousVehicle *)NitrousVehicle::add_vehicle(ent->s.number);

        if (veh->nitrousVehicle)
        {
            //NitrousVehicle::init(veh->nitrousVehicle, ent, &info->nitrousVehParams);
            veh->nitrousVehicle->init(ent, &info->nitrousVehParams);
        }
        Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
        Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
    }
}

// Decomp: CoDMPServer.c:428114  VEH_JoltBody
void __cdecl VEH_JoltBody(gentity_s *ent, const float *dir, float intensity, float speedFrac, float decel)
{
    float clampedIntensity;
    float intensityClampedHigh;
    const vehicle_info_t *info;
    scr_vehicle_s *veh;
    float axis[3][3];

    veh = ent->scr_vehicle;
    info = BG_GetVehicleInfo(veh->infoIdx);
    if ( !info->isNitrous && info->type != 6 )
    {
        if ( intensity < 1.0f )
            intensityClampedHigh = intensity;
        else
            intensityClampedHigh = 1.0f;
        if ( intensity > 0.0f )
            clampedIntensity = intensityClampedHigh;
        else
            clampedIntensity = 0.0f;
        AnglesToAxis(veh->phys.angles, axis);
        veh->joltDir[0] = (float)((float)(*dir * axis[0][0]) + (float)(dir[1] * axis[0][1])) + (float)(dir[2] * axis[0][2]);
        veh->joltDir[1] = -((float)((float)(*dir * axis[1][0]) + (float)(dir[1] * axis[1][1])) + (float)(dir[2] * axis[1][2]));
        veh->joltTime = 1.3f;
        veh->joltWave = 0.0f;
        Vec2Normalize(veh->joltDir);
        veh->joltDir[0] = (float)(info->maxBodyPitch * clampedIntensity) * veh->joltDir[0];
        veh->joltDir[1] = (float)(info->maxBodyRoll * clampedIntensity) * veh->joltDir[1];
        veh->joltSpeed = veh->speed * speedFrac;
        veh->joltDecel = decel;
    }
}

// Decomp: CoDMPServer.c:428155  VEH_CalcAccel
void __cdecl VEH_CalcAccel(gentity_s *ent, char *move, float *bodyAccel, float *rotAccel)
{
    float clampedPitchAccel;
    float clampedRollAccel;
    float clampedYawRotVel;
    float clampedYawRotAccel;
    float clampedBodyVel;
    float clampedBodyAccel;
    float targetBodyVel;
    float targetBodyAccel;
    float clampedYawRate;
    float clampedYawAccel;
    gentity_s *player;
    float forwardInputScale;
    const vehicle_info_t *info;
    scr_vehicle_s *veh;
    float tgtVel[3];
    float maxSpeed;
    float steerInputScale;
    int velAxisIndex;
    float tgtRotVel[3];
    float accel;

    veh = ent->scr_vehicle;
    info = BG_GetVehicleInfo(veh->infoIdx);
    if ( veh->joltTime <= 0.0 )
    {
        if ( veh->maxSpeedOverride <= 0.0 )
            maxSpeed = info->maxSpeed;
        else
            maxSpeed = veh->maxSpeedOverride;
        accel = info->accel;
    }
    else
    {
        maxSpeed = veh->joltSpeed;
        accel = veh->joltDecel;
    }
    forwardInputScale = fabs((float)*move) / 127.0f;
    steerInputScale = fabs((float)move[1]) / 127.0f;
    if ( *move <= 0 )
    {
        if ( *move >= 0 )
            tgtVel[0] = 0.0f;
        else
            tgtVel[0] = (float)(veh->phys.bodyVel[0] - (float)(accel * 0.050000001f)) * forwardInputScale;
    }
    else
    {
        tgtVel[0] = (float)((float)(accel * 0.050000001f) + veh->phys.bodyVel[0]) * forwardInputScale;
    }
    tgtVel[1] = 0.0f;
    tgtVel[2] = veh->phys.bodyVel[2];
    for ( velAxisIndex = 0; velAxisIndex < 3; ++velAxisIndex )
    {
        targetBodyVel = tgtVel[velAxisIndex];
        if ( targetBodyVel < maxSpeed )
            clampedBodyVel = tgtVel[velAxisIndex];
        else
            clampedBodyVel = maxSpeed;
        if ( clampedBodyVel > -maxSpeed )
            tgtVel[velAxisIndex] = clampedBodyVel;
        else
            tgtVel[velAxisIndex] = -maxSpeed;
        bodyAccel[velAxisIndex] = (float)(tgtVel[velAxisIndex] - veh->phys.bodyVel[velAxisIndex]) / 0.050000001f;
        targetBodyAccel = bodyAccel[velAxisIndex];
        if ( targetBodyAccel < accel )
            clampedBodyAccel = bodyAccel[velAxisIndex];
        else
            clampedBodyAccel = accel;
        if ( clampedBodyAccel > -accel )
            bodyAccel[velAxisIndex] = clampedBodyAccel;
        else
            bodyAccel[velAxisIndex] = -accel;
    }
    if ( move[2] <= 0 )
    {
        if ( move[1] >= 0 )
        {
            if ( move[1] <= 0 )
                tgtRotVel[1] = 0.0f;
            else
                tgtRotVel[1] = (float)(veh->phys.rotVel[1] - (float)(info->rotAccel * 0.050000001f)) * steerInputScale;
        }
        else
        {
            tgtRotVel[1] = (float)((float)(info->rotAccel * 0.050000001f) + veh->phys.rotVel[1]) * steerInputScale;
        }
        if ( tgtRotVel[1] < info->rotRate )
            clampedYawRate = tgtRotVel[1];
        else
            clampedYawRate = info->rotRate;
        if ( clampedYawRate > -info->rotRate )
            tgtRotVel[1] = clampedYawRate;
        else
            tgtRotVel[1] = -info->rotRate;
        rotAccel[1] = (float)(tgtRotVel[1] - veh->phys.rotVel[1]) / 0.050000001f;
        if ( rotAccel[1] < info->rotAccel )
            clampedYawAccel = rotAccel[1];
        else
            clampedYawAccel = info->rotAccel;
        if ( clampedYawAccel > -info->rotAccel )
            rotAccel[1] = clampedYawAccel;
        else
            rotAccel[1] = -info->rotAccel;
    }
    else
    {
        iassert(ent->r.ownerNum.isDefined());
        player = ent->r.ownerNum.ent();
        iassert(player->client);
        tgtRotVel[1] = AngleNormalize180(player->client->ps.viewangles[1] - veh->phys.prevAngles[1]) / 0.050000001f;
        if ( tgtRotVel[1] < info->rotRate )
            clampedYawRotVel = tgtRotVel[1];
        else
            clampedYawRotVel = info->rotRate;
        if ( clampedYawRotVel > -info->rotRate )
            tgtRotVel[1] = clampedYawRotVel;
        else
            tgtRotVel[1] = -info->rotRate;
        rotAccel[1] = (float)(tgtRotVel[1] - veh->phys.rotVel[1]) / 0.050000001f;
    }
    tgtRotVel[0] = 0.0f;
    *rotAccel = (float)(0.0f - veh->phys.rotVel[0]) / 0.050000001f;
    if ( *rotAccel < info->rotAccel )
        clampedPitchAccel = *rotAccel;
    else
        clampedPitchAccel = info->rotAccel;
    if ( clampedPitchAccel > -info->rotAccel )
        *rotAccel = clampedPitchAccel;
    else
        *rotAccel = -info->rotAccel;
    tgtRotVel[2] = 0.0f;
    rotAccel[2] = (float)(0.0f - veh->phys.rotVel[2]) / 0.050000001f;
    if ( rotAccel[2] < info->rotAccel )
        clampedRollAccel = rotAccel[2];
    else
        clampedRollAccel = info->rotAccel;
    if ( clampedRollAccel > -info->rotAccel )
        rotAccel[2] = clampedRollAccel;
    else
        rotAccel[2] = -info->rotAccel;
}

// Decomp: CoDMPServer.c:428346  VEH_CorrectAllSolid
int __cdecl VEH_CorrectAllSolid(gentity_s *ent, trace_t *trace)
{
    col_context_t context; // [esp+14h] [ebp-40h] BYREF
    vehicle_physic_t *phys; // [esp+3Ch] [ebp-18h]
    scr_vehicle_s *veh; // [esp+40h] [ebp-14h]
    unsigned int solidDeltaIndex;
    float point[3]; // [esp+48h] [ebp-Ch] BYREF

    veh = ent->scr_vehicle;
    phys = &veh->phys;
    //col_context_t::col_context_t(&context);
    for ( solidDeltaIndex = 0; solidDeltaIndex < 0x1A; ++solidDeltaIndex )
    {
        point[0] = phys->origin[0] + (float)s_correctSolidDeltas[solidDeltaIndex][0];
        point[1] = phys->origin[1] + (float)s_correctSolidDeltas[solidDeltaIndex][1];
        point[2] = phys->origin[2] + (float)s_correctSolidDeltas[solidDeltaIndex][2];
        G_TraceCapsule(trace, point, phys->mins, phys->maxs, point, ent->s.number, ent->clipmask, &context);
        if ( !trace->startsolid )
        {
            phys->origin[0] = point[0];
            phys->origin[1] = point[1];
            phys->origin[2] = point[2];
            point[0] = phys->origin[0];
            point[1] = phys->origin[1];
            point[2] = phys->origin[2] - 1.0;
            G_TraceCapsule(trace, phys->origin, phys->mins, phys->maxs, point, ent->s.number, ent->clipmask, &context);
            memcpy(&s_phys, trace, 0x38u);
            Vec3Lerp(phys->origin, point, trace->fraction, phys->origin);
            return 1;
        }
    }
    return 0;
}

// Decomp: CoDMPServer.c:428401  VEH_ClearGround
void __cdecl VEH_ClearGround()
{
    s_phys.hasGround = 0;
}

// Decomp: CoDMPServer.c:428407  VEH_SlideMove
bool __cdecl VEH_SlideMove(gentity_s *ent, int gravity)
{
    float *physVel;
    float *clipVelOut;
    float *zeroVel;
    float *newPlane;
    float *velAfterBump;
    float *velScratch;
    float timeLeft;
    col_context_t context;
    vehicle_physic_t *phys;
    float dir[3];
    int bumpCount;
    scr_vehicle_s *veh;
    float planes[5][3]; // [esp+ACh] [ebp-B4h] BYREF
    float clipVel[3]; // [esp+E8h] [ebp-78h] BYREF
    float end[3]; // [esp+F4h] [ebp-6Ch] BYREF
    float endVel[3]; // [esp+100h] [ebp-60h] BYREF
    int numPlanes; // [esp+10Ch] [ebp-54h]
    trace_t trace; // [esp+110h] [ebp-50h] BYREF
    float endClipVel[3]; // [esp+14Ch] [ebp-14h] BYREF
    int planeIndex;
    int planeIndex2;
    int planeIndex3;
    float dot;

    veh = ent->scr_vehicle;
    phys = &veh->phys;
    memset(&trace, 0, sizeof(trace));
    timeLeft = 0.05f;
    endVel[0] = veh->phys.vel[0];
    endVel[1] = veh->phys.vel[1];
    endVel[2] = veh->phys.vel[2];
    if ( gravity )
    {
        endVel[2] = endVel[2] - 40.0;
        phys->vel[2] = (float)(phys->vel[2] + endVel[2]) * 0.5;
        if ( s_phys.hasGround )
            VEH_ClipVelocity(phys->vel, s_phys.groundTrace.normal.vec.v, phys->vel);
    }
    if ( s_phys.hasGround )
    {
        numPlanes = 1;
        //*(_QWORD *)&planes[0][0] = *(_QWORD *)s_phys.groundTrace.normal.vec.v;
        planes[0][0] = s_phys.groundTrace.normal.vec.v[0];
        planes[0][1] = s_phys.groundTrace.normal.vec.v[1];
        planes[0][2] = s_phys.groundTrace.normal.vec.v[2];
        //LODWORD(planes[0][2]) = s_phys.groundTrace.normal.vec.u[2];
    }
    else
    {
        numPlanes = 0;
    }
    Vec3NormalizeTo(phys->vel, planes[numPlanes++]);
    //col_context_t::col_context_t(&context);
    for ( bumpCount = 0; bumpCount < 4; ++bumpCount )
    {
        end[0] = (float)(timeLeft * phys->vel[0]) + phys->origin[0];
        end[1] = (float)(timeLeft * phys->vel[1]) + phys->origin[1];
        end[2] = (float)(timeLeft * phys->vel[2]) + phys->origin[2];
        G_TraceCapsule(&trace, phys->origin, phys->mins, phys->maxs, end, ent->s.number, ent->clipmask, &context);
        if ( trace.startsolid )
        {
            phys->vel[2] = 0.0f;
            return 1;
        }
        if ( trace.fraction > 0.0 )
            Vec3Lerp(phys->origin, end, trace.fraction, phys->origin);
        if ( trace.fraction == 1.0 )
            break;
        timeLeft = timeLeft - (float)(timeLeft * trace.fraction);
        if ( numPlanes >= 5 )
        {
            physVel = phys->vel;
            phys->vel[0] = 0.0f;
            physVel[1] = 0.0f;
            physVel[2] = 0.0f;
            return 1;
        }
        for ( planeIndex = 0; planeIndex < numPlanes; ++planeIndex )
        {
            if ( (float)((float)((float)(trace.normal.vec.v[0] * planes[planeIndex][0]) + (float)(trace.normal.vec.v[1] * planes[planeIndex][1]))
                                 + (float)(trace.normal.vec.v[2] * planes[planeIndex][2])) > 0.99000001 )
            {
                velAfterBump = phys->vel;
                velScratch = phys->vel;
                phys->vel[0] = trace.normal.vec.v[0] + phys->vel[0];
                velAfterBump[1] = trace.normal.vec.v[1] + velScratch[1];
                velAfterBump[2] = trace.normal.vec.v[2] + velScratch[2];
                break;
            }
        }
        if ( planeIndex >= numPlanes )
        {
            newPlane = planes[numPlanes];
            newPlane[0] = trace.normal.vec.v[0];
            newPlane[1] = trace.normal.vec.v[1];
            newPlane[2] = trace.normal.vec.v[2];
            ++numPlanes;
            for ( planeIndex = 0; planeIndex < numPlanes; ++planeIndex )
            {
                if ( (float)((float)((float)(phys->vel[0] * planes[planeIndex][0]) + (float)(phys->vel[1] * planes[planeIndex][1]))
                                     + (float)(phys->vel[2] * planes[planeIndex][2])) < 0.1 )
                {
                    VEH_ClipVelocity(phys->vel, planes[planeIndex], clipVel);
                    VEH_ClipVelocity(endVel, planes[planeIndex], endClipVel);
                    for ( planeIndex2 = 0; planeIndex2 < numPlanes; ++planeIndex2 )
                    {
                        if ( planeIndex2 != planeIndex
                            && (float)((float)((float)(clipVel[0] * planes[planeIndex2][0]) + (float)(clipVel[1] * planes[planeIndex2][1]))
                                             + (float)(clipVel[2] * planes[planeIndex2][2])) < 0.1 )
                        {
                            VEH_ClipVelocity(clipVel, planes[planeIndex2], clipVel);
                            VEH_ClipVelocity(endClipVel, planes[planeIndex2], endClipVel);
                            if ( (float)((float)((float)(clipVel[0] * planes[planeIndex][0]) + (float)(clipVel[1] * planes[planeIndex][1]))
                                                 + (float)(clipVel[2] * planes[planeIndex][2])) < 0.0 )
                            {
                                Vec3Cross(planes[planeIndex], planes[planeIndex2], dir);
                                Vec3Normalize(dir);
                                dot = (float)((float)(dir[0] * phys->vel[0]) + (float)(dir[1] * phys->vel[1]))
                                        + (float)(dir[2] * phys->vel[2]);
                                clipVel[0] = dot * dir[0];
                                clipVel[1] = dot * dir[1];
                                clipVel[2] = dot * dir[2];
                                dot = (float)((float)(dir[0] * endVel[0]) + (float)(dir[1] * endVel[1])) + (float)(dir[2] * endVel[2]);
                                endClipVel[0] = dot * dir[0];
                                endClipVel[1] = dot * dir[1];
                                endClipVel[2] = dot * dir[2];
                                for ( planeIndex3 = 0; planeIndex3 < numPlanes; ++planeIndex3 )
                                {
                                    if ( planeIndex3 != planeIndex
                                        && planeIndex3 != planeIndex2
                                        && (float)((float)((float)(clipVel[0] * planes[planeIndex3][0]) + (float)(clipVel[1] * planes[planeIndex3][1]))
                                                         + (float)(clipVel[2] * planes[planeIndex3][2])) < 0.1 )
                                    {
                                        zeroVel = phys->vel;
                                        phys->vel[0] = 0.0f;
                                        zeroVel[1] = 0.0f;
                                        zeroVel[2] = 0.0f;
                                        return 1;
                                    }
                                }
                            }
                        }
                    }
                    clipVelOut = phys->vel;
                    phys->vel[0] = clipVel[0];
                    clipVelOut[1] = clipVel[1];
                    clipVelOut[2] = clipVel[2];
                    endVel[0] = endClipVel[0];
                    endVel[1] = endClipVel[1];
                    endVel[2] = endClipVel[2];
                    break;
                }
            }
        }
    }
    if ( gravity )
    {
        physVel = phys->vel;
        phys->vel[0] = endVel[0];
        physVel[1] = endVel[1];
        physVel[2] = endVel[2];
    }
    return bumpCount != 0;
}

// Decomp: CoDMPServer.c:428607  VEH_ClipVelocity
void __cdecl VEH_ClipVelocity(float *in, float *normal, float *out)
{
    int axisIndex;
    float backoff; // [esp+8h] [ebp-4h]
    float backoffa; // [esp+8h] [ebp-4h]

    if ( normal[2] < 0.69999999 || (float)((float)(*in * *in) + (float)(in[1] * in[1])) < (float)(in[2] * in[2]) )
    {
        backoff = (float)((float)(*in * *normal) + (float)(in[1] * normal[1])) + (float)(in[2] * normal[2]);
        if ( backoff >= 0.0 )
            backoffa = backoff / 1.01;
        else
            backoffa = backoff * 1.01;
        for ( axisIndex = 0; axisIndex < 3; ++axisIndex )
            out[axisIndex] = in[axisIndex] - (float)(normal[axisIndex] * backoffa);
    }
    else
    {
        //out[2] = (float)(COERCE_FLOAT(*(unsigned int *)in ^ _mask__NegFloat_) * *normal) - (float)(in[1] * normal[1]);
        out[2] = (float)(-(*in) * *normal) - (float)(in[1] * normal[1]);
        *out = *in * normal[2];
        out[1] = in[1] * normal[2];
    }
}

// Decomp: CoDMPServer.c:428633  VEH_AirMove
void __cdecl VEH_AirMove(gentity_s *ent, int gravity)
{
    if ( s_phys.hasGround )
        VEH_ClipVelocity(ent->scr_vehicle->phys.vel, s_phys.groundTrace.normal.vec.v, ent->scr_vehicle->phys.vel);
    VEH_StepSlideMove(ent, gravity);
}

// Decomp: CoDMPServer.c:428641  VEH_StepSlideMove
void __cdecl VEH_StepSlideMove(gentity_s *ent, int gravity)
{
    float *physVel;
    col_context_t context;
    vehicle_physic_t *phys; // [esp+38h] [ebp-80h]
    scr_vehicle_s *veh; // [esp+3Ch] [ebp-7Ch]
    float startOrigin[3]; // [esp+40h] [ebp-78h] BYREF
    float endpos[3]; // [esp+4Ch] [ebp-6Ch] BYREF
    trace_t trace; // [esp+58h] [ebp-60h] BYREF
    float up[3]; // [esp+94h] [ebp-24h] BYREF
    float down[3]; // [esp+A0h] [ebp-18h] BYREF
    float startVel[3]; // [esp+ACh] [ebp-Ch]

    veh = ent->scr_vehicle;
    phys = &veh->phys;
    memset(&trace, 0, sizeof(trace));
    //col_context_t::col_context_t(&context);
    startOrigin[0] = phys->origin[0];
    startOrigin[1] = phys->origin[1];
    startOrigin[2] = phys->origin[2];
    startVel[0] = phys->vel[0];
    startVel[1] = phys->vel[1];
    startVel[2] = phys->vel[2];
    if ( VEH_SlideMove(ent, gravity) )
    {
        down[0] = startOrigin[0];
        down[1] = startOrigin[1];
        down[2] = startOrigin[2] - 18.0;
        G_TraceCapsule(&trace, startOrigin, phys->mins, phys->maxs, down, ent->s.number, ent->clipmask, &context);
        if ( phys->vel[2] <= 0.0 || trace.fraction != 1.0 && trace.normal.vec.v[2] >= 0.69999999 )
        {
            up[0] = startOrigin[0];
            up[1] = startOrigin[1];
            up[2] = startOrigin[2] + 18.0;
            G_TraceCapsule(&trace, startOrigin, phys->mins, phys->maxs, up, ent->s.number, ent->clipmask, &context);
            if ( !trace.startsolid )
            {
                Vec3Lerp(startOrigin, up, trace.fraction, endpos);
                phys->origin[0] = endpos[0];
                phys->origin[1] = endpos[1];
                phys->origin[2] = endpos[2];
                physVel = phys->vel;
                phys->vel[0] = startVel[0];
                physVel[1] = startVel[1];
                physVel[2] = startVel[2];
                VEH_SlideMove(ent, gravity);
                down[0] = phys->origin[0];
                down[1] = phys->origin[1];
                down[2] = phys->origin[2];
                down[2] = (float)(startOrigin[2] - endpos[2]) + down[2];
                G_TraceCapsule(&trace, phys->origin, phys->mins, phys->maxs, down, ent->s.number, ent->clipmask, &context);
                if ( !trace.startsolid )
                    Vec3Lerp(phys->origin, down, trace.fraction, phys->origin);
                if ( trace.fraction < 1.0 )
                    VEH_ClipVelocity(phys->vel, trace.normal.vec.v, phys->vel);
            }
        }
    }
}

// Decomp: CoDMPServer.c:428749  VEH_ClampPointToBoundingBox
bool __cdecl VEH_ClampPointToBoundingBox(gentity_s *vehicle, const float *point, float *out)
{
    float axisDistance;
    float clampedZ;
    float clampedY;
    float clampedX;
    float localZ;
    float localY;
    float localX;
    float distanceFromMin;
    float distanceFromMax;
    int bbAxisIndex;
    bool clampToMax;
    float smallestDistance;
    int nearestAxisIndex;
    float vehicleInvQuat[4]; // [esp+70h] [ebp-A8h] BYREF
    float bbMins[3]; // [esp+80h] [ebp-98h] BYREF
    float vehicleAxis[3][3]; // [esp+8Ch] [ebp-8Ch] BYREF
    bool wasOutside; // [esp+B3h] [ebp-65h]
    float bbMaxs[3]; // [esp+B4h] [ebp-64h] BYREF
    float playerOrigin[3]; // [esp+C0h] [ebp-58h]
    float vehicleInvAxis[3][3]; // [esp+CCh] [ebp-4Ch] BYREF
    float localPlayerOrigin[3]; // [esp+F0h] [ebp-28h]
    float vehicleQuat[4]; // [esp+FCh] [ebp-1Ch] BYREF
    float clampedLocalOrigin[3]; // [esp+10Ch] [ebp-Ch]

    wasOutside = 1;
    playerOrigin[0] = *point;
    playerOrigin[1] = point[1];
    playerOrigin[2] = point[2];
    VEH_GetBoundingBoxFromCollmap(vehicle, bbMins, bbMaxs);
    playerOrigin[0] = playerOrigin[0] - vehicle->r.currentOrigin[0];
    playerOrigin[1] = playerOrigin[1] - vehicle->r.currentOrigin[1];
    playerOrigin[2] = playerOrigin[2] - vehicle->r.currentOrigin[2];
    AnglesToQuat(vehicle->r.currentAngles, vehicleQuat);
    vehicleInvQuat[0] = -vehicleQuat[0];
    vehicleInvQuat[1] = -vehicleQuat[1];
    vehicleInvQuat[2] = -vehicleQuat[2];
    vehicleInvQuat[3] = vehicleQuat[3];
    QuatToAxis(vehicleInvQuat, vehicleInvAxis);
    localPlayerOrigin[0] = (float)(playerOrigin[1] * vehicleInvAxis[1][0])
                                             + (float)(playerOrigin[0] * vehicleInvAxis[0][0]);
    localPlayerOrigin[1] = (float)(playerOrigin[1] * vehicleInvAxis[1][1])
                                             + (float)(playerOrigin[0] * vehicleInvAxis[0][1]);
    localPlayerOrigin[2] = (float)(playerOrigin[1] * vehicleInvAxis[1][2])
                                             + (float)(playerOrigin[0] * vehicleInvAxis[0][2]);
    localPlayerOrigin[0] = (float)(playerOrigin[2] * vehicleInvAxis[2][0]) + localPlayerOrigin[0];
    localPlayerOrigin[1] = (float)(playerOrigin[2] * vehicleInvAxis[2][1]) + localPlayerOrigin[1];
    localPlayerOrigin[2] = (float)(playerOrigin[2] * vehicleInvAxis[2][2]) + localPlayerOrigin[2];
    if ( localPlayerOrigin[0] < bbMaxs[0] )
        localX = localPlayerOrigin[0];
    else
        localX = bbMaxs[0];
    if ( localPlayerOrigin[0] > bbMins[0] )
        clampedX = localX;
    else
        clampedX = bbMins[0];
    clampedLocalOrigin[0] = clampedX;
    if ( localPlayerOrigin[1] < bbMaxs[1] )
        localY = localPlayerOrigin[1];
    else
        localY = bbMaxs[1];
    if ( localPlayerOrigin[1] > bbMins[1] )
        clampedY = localY;
    else
        clampedY = bbMins[1];
    clampedLocalOrigin[1] = clampedY;
    if ( localPlayerOrigin[2] < bbMaxs[2] )
        localZ = localPlayerOrigin[2];
    else
        localZ = bbMaxs[2];
    if ( localPlayerOrigin[2] > bbMins[2] )
        clampedZ = localZ;
    else
        clampedZ = bbMins[2];
    clampedLocalOrigin[2] = clampedZ;
    if ( clampedLocalOrigin[0] == localPlayerOrigin[0] && clampedLocalOrigin[1] == localPlayerOrigin[1] )
    {
        wasOutside = 0;
        smallestDistance = FLT_MAX;
        nearestAxisIndex = 0;
        clampToMax = true;
        for ( bbAxisIndex = 0; bbAxisIndex < 2; ++bbAxisIndex )
        {
            distanceFromMax = bbMaxs[bbAxisIndex] - clampedLocalOrigin[bbAxisIndex];
            distanceFromMin = clampedLocalOrigin[bbAxisIndex] - bbMins[bbAxisIndex];
            if ( distanceFromMin <= distanceFromMax )
                axisDistance = clampedLocalOrigin[bbAxisIndex] - bbMins[bbAxisIndex];
            else
                axisDistance = bbMaxs[bbAxisIndex] - clampedLocalOrigin[bbAxisIndex];
            if ( smallestDistance > axisDistance )
            {
                smallestDistance = axisDistance;
                nearestAxisIndex = bbAxisIndex;
                clampToMax = distanceFromMin > distanceFromMax;
            }
        }
        if ( clampToMax )
            clampedLocalOrigin[nearestAxisIndex] = bbMaxs[nearestAxisIndex];
        else
            clampedLocalOrigin[nearestAxisIndex] = bbMins[nearestAxisIndex];
    }
    QuatToAxis(vehicleQuat, vehicleAxis);
    *out = clampedLocalOrigin[0] * vehicleAxis[0][0];
    out[1] = clampedLocalOrigin[0] * vehicleAxis[0][1];
    out[2] = clampedLocalOrigin[0] * vehicleAxis[0][2];
    *out = (float)(clampedLocalOrigin[1] * vehicleAxis[1][0]) + *out;
    out[1] = (float)(clampedLocalOrigin[1] * vehicleAxis[1][1]) + out[1];
    out[2] = (float)(clampedLocalOrigin[1] * vehicleAxis[1][2]) + out[2];
    *out = (float)(clampedLocalOrigin[2] * vehicleAxis[2][0]) + *out;
    out[1] = (float)(clampedLocalOrigin[2] * vehicleAxis[2][1]) + out[1];
    out[2] = (float)(clampedLocalOrigin[2] * vehicleAxis[2][2]) + out[2];
    *out = *out + vehicle->r.currentOrigin[0];
    out[1] = out[1] + vehicle->r.currentOrigin[1];
    out[2] = out[2] + vehicle->r.currentOrigin[2];
    return wasOutside;
}

// Decomp: CoDMPServer.c:428905  VEH_GetBoundingBoxFromCollmap
void __cdecl VEH_GetBoundingBoxFromCollmap(gentity_s *vehicle, float *bbMins, float *bbMaxs)
{
    float mergedMax;
    float mergedMin;
    float *modelMaxs;
    float *modelMins;
    BrushWrapper *brush;
    int mergeAxisIndex;
    float brushMins[3]; // [esp+1Ch] [ebp-28h]
    float brushMaxs[3]; // [esp+28h] [ebp-1Ch]
    unsigned int geomIndex; // [esp+34h] [ebp-10h]
    DObj *obj; // [esp+38h] [ebp-Ch]
    XModel *baseModel; // [esp+3Ch] [ebp-8h]
    const PhysGeomList *collmap; // [esp+40h] [ebp-4h]

    obj = Com_GetServerDObj(vehicle->s.number);
    if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 2270, 0, "%s", "obj") )
        __debugbreak();
    if ( DObjGetNumModels(obj) <= 0
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    2272,
                    0,
                    "%s",
                    "DObjGetNumModels( obj ) > 0") )
    {
        __debugbreak();
    }
    baseModel = DObjGetModel(obj, 0);
    *bbMins = FLT_MAX;
    bbMins[1] = FLT_MAX;
    bbMins[2] = FLT_MAX;
    *bbMaxs = -FLT_MAX;
    bbMaxs[1] = -FLT_MAX;
    bbMaxs[2] = -FLT_MAX;
    if ( XModelHasCollmap(baseModel) )
    {
        collmap = XModelGetCollmap(baseModel, 0);
        if ( !collmap
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                        2279,
                        0,
                        "%s\n\t(collmap) = %p",
                        "(collmap != 0)",
                        0) )
        {
            __debugbreak();
        }
        for ( geomIndex = 0; geomIndex < collmap->count; ++geomIndex )
        {
            if ( !collmap->geoms
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                            2283,
                            0,
                            "%s",
                            "collmap->geoms") )
            {
                __debugbreak();
            }
            if ( collmap->geoms[geomIndex].brush )
            {
                brush = collmap->geoms[geomIndex].brush;
                brushMins[0] = brush->mins[0];
                brushMins[1] = brush->mins[1];
                brushMins[2] = brush->mins[2];
                brushMaxs[0] = brush->maxs[0];
                brushMaxs[1] = brush->maxs[1];
                brushMaxs[2] = brush->maxs[2];
                for ( mergeAxisIndex = 0; mergeAxisIndex < 3; ++mergeAxisIndex )
                {
                    if ( bbMins[mergeAxisIndex] <= brushMins[mergeAxisIndex] )
                        mergedMin = bbMins[mergeAxisIndex];
                    else
                        mergedMin = brushMins[mergeAxisIndex];
                    bbMins[mergeAxisIndex] = mergedMin;
                    if ( brushMaxs[mergeAxisIndex] <= bbMaxs[mergeAxisIndex] )
                        mergedMax = bbMaxs[mergeAxisIndex];
                    else
                        mergedMax = brushMaxs[mergeAxisIndex];
                    bbMaxs[mergeAxisIndex] = mergedMax;
                }
            }
        }
    }
    else
    {
        modelMins = baseModel->mins;
        *bbMins = baseModel->mins[0];
        bbMins[1] = modelMins[1];
        bbMins[2] = modelMins[2];
        modelMaxs = baseModel->maxs;
        *bbMaxs = baseModel->maxs[0];
        bbMaxs[1] = modelMaxs[1];
        bbMaxs[2] = modelMaxs[2];
    }
}

// Decomp: CoDMPServer.c:429025  VEH_DebugAim
void __cdecl VEH_DebugAim(gentity_s *ent, const float *color, int duration)
{
    int boneIndex; // [esp+20h] [ebp-C8h]
    float angles[3]; // [esp+24h] [ebp-C4h] BYREF
    weaponParms wp; // [esp+30h] [ebp-B8h] BYREF
    BulletFireParams fireParams;
    float flashMtx[4][3]; // [esp+B8h] [ebp-30h] BYREF

    Weapon_SetWeaponParamsWeapon(&wp, ent->s.weapon);
    boneIndex = ent->scr_vehicle->boneIndex.flash[0];
    if ( boneIndex < 0
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 2782, 1, "%s", "boneIndex >= 0") )
    {
        __debugbreak();
    }
    G_DObjGetWorldBoneIndexMatrix(ent, boneIndex, flashMtx);
    //*(_QWORD *)wp.gunForward = *(_QWORD *)&flashMtx[0][0];
    wp.gunForward[0] = flashMtx[0][0];
    wp.gunForward[1] = flashMtx[0][1];
    wp.gunForward[2] = flashMtx[0][2];
    //*(_QWORD *)wp.muzzleTrace = *(_QWORD *)&flashMtx[3][0];
    wp.muzzleTrace[0] = flashMtx[3][0];
    wp.muzzleTrace[1] = flashMtx[3][1];
    wp.muzzleTrace[2] = flashMtx[3][2];
    //*(_QWORD *)wp.forward = *(_QWORD *)&flashMtx[0][0];
    //*(_QWORD *)wp.forward = *(_QWORD *)&flashMtx[0][0];
    wp.forward[0] = flashMtx[0][0];
    wp.forward[1] = flashMtx[0][1];
    wp.forward[2] = flashMtx[0][2];
    AxisToAngles(flashMtx, angles);
    fireParams.origStart[0] = wp.muzzleTrace[0];
    fireParams.origStart[1] = wp.muzzleTrace[1];
    fireParams.origStart[2] = wp.muzzleTrace[2];
    fireParams.start[0] = wp.muzzleTrace[0];
    fireParams.start[1] = wp.muzzleTrace[1];
    fireParams.start[2] = wp.muzzleTrace[2];
    //*(_QWORD *)wp.right = *(_QWORD *)&flashMtx[1][0];
    wp.right[0] = flashMtx[1][0];
    wp.right[1] = flashMtx[1][1];
    wp.right[2] = flashMtx[1][2];
    //*(_QWORD *)wp.up = *(_QWORD *)&flashMtx[2][0];
    wp.up[0] = flashMtx[2][0];
    wp.up[1] = flashMtx[2][1];
    wp.up[2] = flashMtx[2][2];
    Bullet_Endpos(level.time, wp.weapDef->aiSpread, fireParams.end, fireParams.dir, &wp, 8192.0);
    CL_AddDebugLine(fireParams.start, fireParams.end, color, 1, duration);
}

static float minSpeedToRotate = 50.0f;
// Decomp: CoDMPServer.c:429108  VEH_UpdateVelocityWithRotation
void __cdecl VEH_UpdateVelocityWithRotation(gentity_s *ent)
{
    float forwardSpeedToRotate; // [esp+20h] [ebp-38h]
    float speedFracToRotate; // [esp+24h] [ebp-34h]
    scr_vehicle_s *veh; // [esp+2Ch] [ebp-2Ch]
    float right[3]; // [esp+30h] [ebp-28h] BYREF
    float forward[3]; // [esp+3Ch] [ebp-1Ch] BYREF
    float forwardVelToRotate[3]; // [esp+48h] [ebp-10h]
    float forwardSpeed; // [esp+54h] [ebp-4h]

    veh = ent->scr_vehicle;
    YawVectors(veh->phys.prevAngles[1], forward, right);
    forwardSpeed = (float)((float)(veh->phys.vel[0] * forward[0]) + (float)(veh->phys.vel[1] * forward[1]))
                             + (float)(veh->phys.vel[2] * forward[2]);
    speedFracToRotate = veh->turningAbility;
    if ( forwardSpeed > minSpeedToRotate )
    {
        forwardSpeedToRotate = forwardSpeed * speedFracToRotate;
        forwardVelToRotate[0] = (float)(forwardSpeed * speedFracToRotate) * forward[0];
        forwardVelToRotate[1] = (float)(forwardSpeed * speedFracToRotate) * forward[1];
        forwardVelToRotate[2] = (float)(forwardSpeed * speedFracToRotate) * forward[2];
        veh->phys.vel[0] = veh->phys.vel[0] - forwardVelToRotate[0];
        veh->phys.vel[1] = veh->phys.vel[1] - forwardVelToRotate[1];
        veh->phys.vel[2] = veh->phys.vel[2] - forwardVelToRotate[2];
        YawVectors(veh->phys.angles[1], forward, right);
        forwardVelToRotate[0] = forwardSpeedToRotate * forward[0];
        forwardVelToRotate[1] = forwardSpeedToRotate * forward[1];
        forwardVelToRotate[2] = forwardSpeedToRotate * forward[2];
        veh->phys.vel[0] = veh->phys.vel[0] + (float)(forwardSpeedToRotate * forward[0]);
        veh->phys.vel[1] = veh->phys.vel[1] + forwardVelToRotate[1];
        veh->phys.vel[2] = veh->phys.vel[2] + forwardVelToRotate[2];
    }
}

// Decomp: CoDMPServer.c:429151  VEH_GetNewSpeedAndAccel
void __cdecl VEH_GetNewSpeedAndAccel(
                scr_vehicle_s *veh,
                float dt,
                int hovering,
                float speedScale,
                float *newSpeed,
                float *accelMax)
{
    const vehicle_info_t *info; // [esp+10h] [ebp-10h]
    float decel; // [esp+14h] [ebp-Ch]
    float speed; // [esp+18h] [ebp-8h]
    float scaledSpeed;
    float accel;

    if ( !veh && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 5090, 0, "%s", "veh") )
        __debugbreak();
    if ( hovering )
    {
        speed = veh->hover.hoverSpeed;
        accel = veh->hover.hoverAccel;
        decel = accel * 0.5;
    }
    else if ( veh->manualMode == 1 )
    {
        speed = veh->manualSpeed;
        accel = veh->manualAccel;
        decel = veh->manualDecel;
    }
    else
    {
        if ( (veh->flags & 0x100) != 0 )
        {
            info = BG_GetVehicleInfo(veh->infoIdx);
            speed = veh->pathPos.speed;
        }
        else
        {
            info = BG_GetVehicleInfo(veh->infoIdx);
            speed = info->maxSpeed;
        }
        accel = info->accel;
        decel = accel * 0.5;
    }
    scaledSpeed = speed * speedScale;
    if ( veh->stopping )
    {
        *newSpeed = VEH_AccelerateSpeed(veh->speed, 0.0, decel, dt);
        *accelMax = decel;
    }
    else
    {
        if ( scaledSpeed < 0.0f
            && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 5128, 0, "%s", "speed >= 0.0f") )
        {
            __debugbreak();
        }
        *newSpeed = VEH_AccelerateSpeed(veh->speed, scaledSpeed, accel, dt);
        *accelMax = accel;
    }
}

// Decomp: CoDMPServer.c:429223  VEH_AccelerateSpeed
double __cdecl VEH_AccelerateSpeed(float speed, float tgtSpeed, float accel, float dt)
{
    float newSpeed;

    if ( tgtSpeed <= speed )
    {
        newSpeed = speed - (float)(accel * dt);
        if ( tgtSpeed > newSpeed )
            return tgtSpeed;
    }
    else
    {
        newSpeed = (float)(accel * dt) + speed;
        if ( newSpeed > tgtSpeed )
            return tgtSpeed;
    }
    return newSpeed;
}

// Decomp: CoDMPServer.c:429243  VEH_SetLinkAngleClamps
void __cdecl VEH_SetLinkAngleClamps(
                const vehicle_info_t *info,
                gentity_s *player,
                int seatIndex,
                float *playerAngles,
                float *vehicleAngles)
{
    float zero_vec[3]; // [esp+1Ch] [ebp-18h] BYREF
    int gunnerIndex; // [esp+28h] [ebp-Ch]
    const WeaponDef *weapDef; // [esp+2Ch] [ebp-8h]
    gclient_s *client; // [esp+30h] [ebp-4h]

    if ( !player->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 5986, 0, "%s", "player->client") )
    {
        __debugbreak();
    }
    client = player->client;
    client->linkAnglesMinClamp[1] = -180.0f;
    client->linkAnglesMaxClamp[1] = 180.0f;
    client->linkAnglesMinClamp[0] = -180.0f;
    client->linkAnglesMaxClamp[0] = 180.0f;
    G_UpdateViewAngleClamp(player->client, playerAngles);
    if ( seatIndex < 1 || seatIndex > 4 )
    {
        if ( seatIndex )
        {
            if ( (seatIndex < 5 || seatIndex > 10)
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                            6038,
                            0,
                            "%s",
                            "VEH_PASSENGER_POSITION(seatIndex)") )
            {
                __debugbreak();
            }
            client->linkAnglesMaxClamp[0] = *(float *)&info->gunnerWeapon[3][16 * seatIndex + 40];
            //LODWORD(client->linkAnglesMinClamp[0]) = *(unsigned int *)&info->gunnerWeapon[3][16 * seatIndex + 36] ^ _mask__NegFloat_;
            (client->linkAnglesMinClamp[0]) = -(*(float*)&info->gunnerWeapon[3][16 * seatIndex + 36]);
            client->linkAnglesMaxClamp[1] = *(float *)&info->gunnerWeapon[3][16 * seatIndex + 28];
            //LODWORD(client->linkAnglesMinClamp[1]) = *(unsigned int *)&info->gunnerWeapon[3][16 * seatIndex + 32] ^ _mask__NegFloat_;
            (client->linkAnglesMinClamp[1]) = -(*(float *)&info->gunnerWeapon[3][16 * seatIndex + 32]);
            client->ps.linkFlags |= 2u;
            G_UpdateViewAngleClamp(client, vehicleAngles);
        }
        else if ( info->turretClampPlayerView )
        {
            client->linkAnglesMaxClamp[0] = info->turretViewLimits.vertSpanDown;
            client->linkAnglesMinClamp[0] = -info->turretViewLimits.vertSpanUp;
            client->linkAnglesMaxClamp[1] = info->turretViewLimits.horizSpanLeft;
            client->linkAnglesMinClamp[1] = -info->turretViewLimits.horizSpanRight;
            G_UpdateViewAngleClamp(client, vehicleAngles);
        }
    }
    else
    {
        gunnerIndex = seatIndex - 1;
        weapDef = BG_GetWeaponDef(*(unsigned __int16 *)&info->gunnerWeapon[3][2 * seatIndex + 62]);
        if ( gVehicleRelativeGunnerAngles )
        {
            memset(zero_vec, 0, sizeof(zero_vec));
            SetClientViewAngle(player, zero_vec);
            client->ps.viewAngleClampBase[0] = (float)(weapDef->bottomArc - weapDef->topArc) / 2.0;
            client->ps.viewAngleClampBase[1] = (float)(weapDef->leftArc - weapDef->rightArc) / 2.0;
            client->ps.viewAngleClampRange[0] = (float)(fabs(weapDef->topArc)
                                                                                                + fabs(weapDef->bottomArc))
                                                                                / 2.0;
            client->ps.viewAngleClampRange[1] = (float)(fabs(weapDef->leftArc)
                                                                                                + fabs(weapDef->rightArc))
                                                                                / 2.0;
        }
        else
        {
            client->linkAnglesMaxClamp[0] = info->gunnerRestAngles[gunnerIndex][0] + weapDef->bottomArc;
            client->linkAnglesMinClamp[0] = info->gunnerRestAngles[gunnerIndex][0] - weapDef->topArc;
            client->linkAnglesMaxClamp[1] = info->gunnerRestAngles[gunnerIndex][1] + weapDef->leftArc;
            client->linkAnglesMinClamp[1] = info->gunnerRestAngles[gunnerIndex][1] - weapDef->rightArc;
            G_UpdateViewAngleClamp(client, vehicleAngles);
        }
    }
}

// Decomp: CoDMPServer.c:429335  VEH_SetPlayerVehicle
void __cdecl VEH_SetPlayerVehicle(gentity_s *ent, bool enable)
{
    broad_phase_info *bpi;
    broad_phase_info *bpiNode;
    PhysObjUserData *phys_user_data;

    if ( (!ent || !ent->scr_vehicle || !ent->scr_vehicle->nitrousVehicle)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    6054,
                    0,
                    "%s",
                    "ent && ent->scr_vehicle && ent->scr_vehicle->nitrousVehicle") )
    {
        __debugbreak();
    }
    Sys_EnterCriticalSection(CRITSECT_PHYSICS);
    phys_user_data = ent->scr_vehicle->nitrousVehicle->m_phys_user_data;
    if ( phys_user_data )
    {
        if ( enable )
        {
            if ( phys_user_data->m_bpb->is_bpi() )
            {
                //bpi = broad_phase_base::get_bpi(phys_user_data->m_bpb);
                bpi = phys_user_data->m_bpb->get_bpi();
                bpi->m_env_collision_flags |= 0x20u;
                bpi->m_my_collision_type_flags |= 0x80u;
            }
            else
            {
                for ( bpiNode = phys_user_data->m_bpb->get_bpg()->m_list_bpi_head;
                            bpiNode;
                            bpiNode = (broad_phase_info *)bpiNode->m_list_bpb_next )
                {
                    bpiNode->m_env_collision_flags |= 0x20u;
                    bpiNode->m_my_collision_type_flags |= 0x80u;
                }
            }
        }
        else if ( phys_user_data->m_bpb->is_bpi() )
        {
            bpi = phys_user_data->m_bpb->get_bpi();
            bpi->m_env_collision_flags &= ~0x20u;
            bpi->m_my_collision_type_flags &= ~0x80u;
        }
        else
        {
            for ( bpiNode = phys_user_data->m_bpb->get_bpg()->m_list_bpi_head;
                        bpiNode;
                        bpiNode = (broad_phase_info *)bpiNode->m_list_bpb_next )
            {
                bpiNode->m_env_collision_flags &= ~0x20u;
                bpiNode->m_my_collision_type_flags &= ~0x80u;
            }
        }
    }
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
}

// Decomp: CoDMPServer.c:429398  VEH_LinkPlayer
void __cdecl VEH_LinkPlayer(gentity_s *ent, gentity_s *player, int seatIndex, bool changingSeats)
{
    char *seatBoneName;
    char *occupiedSeatName;
    char *missingSeatName;
    char *linkFailSeatName;
    int occupant;
    gclient_s *client; // [esp+18h] [ebp-78h]
    float playerAngles[3]; // [esp+1Ch] [ebp-74h] BYREF
    const vehicle_info_t *info; // [esp+28h] [ebp-68h]
    scr_vehicle_s *veh; // [esp+2Ch] [ebp-64h]
    float playerMtx[4][3]; // [esp+30h] [ebp-60h] BYREF
    float barrelMtx[4][3]; // [esp+60h] [ebp-30h] BYREF

    veh = ent->scr_vehicle;
    client = player->client;
    if ( !client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 6126, 0, "%s", "client") )
    {
        __debugbreak();
    }
    info = BG_GetVehicleInfo(veh->infoIdx);
    if ( player->client->lastStand && !info->remoteControl )
    {
        Com_PrintWarning(1, "Attempted to link a player in second chance to a vehicle!\n");
        return;
    }
    if ( VEH_GetSeatOccupantEntity(veh, seatIndex) != player )
    {
        if ( (client->ps.eFlags & 0x10000) == 0 )
        {
            if ( (client->ps.eFlags & 0x4000) != 0 )
                Com_Error(ERR_DROP, "VEH_LinkPlayer: Player is already using a vehicle");
            if ( player->r.ownerNum.isDefined() )
                Com_Error(ERR_DROP, "VEH_LinkPlayer: Player already has an owner");
        }
        if ( seatIndex < 0 )
            Com_Error(ERR_DROP, "VEH_LinkPlayer: No valid seat to attach player too");
        if ( veh->boneIndex.seats[seatIndex] < 0 )
        {
            seatBoneName = SL_ConvertToString(*s_seatTags[seatIndex], SCRIPTINSTANCE_SERVER);
            Com_Error(ERR_DROP, "VEH_LinkPlayer: Trying to use vehicle without a bone [%s]", seatBoneName);
        }
        occupant = VEH_GetSeatOccupantEntNum(veh, seatIndex);
        if ( occupant != 1023 && occupant != player->s.number )
        {
            occupiedSeatName = SL_ConvertToString(*s_seatTags[seatIndex], SCRIPTINSTANCE_SERVER);
            Com_Error(ERR_DROP, "VEH_LinkPlayer: Trying to get in seat [%s] that is already occupied", occupiedSeatName);
        }
        G_DObjGetWorldBoneIndexMatrix(ent, veh->boneIndex.seats[seatIndex], playerMtx);
        if ( seatIndex )
        {
            if ( seatIndex <= 0 || seatIndex > 4 )
                goto LABEL_28;
        }
        else if ( veh->boneIndex.barrel >= 0 )
        {
            G_DObjGetWorldBoneIndexMatrix(ent, veh->boneIndex.barrel, barrelMtx);
            AxisToAngles(barrelMtx, playerAngles);
            goto LABEL_29;
        }
        if ( veh->boneIndex.flash[4 * seatIndex + 2] < 0 )
        {
LABEL_28:
            AxisToAngles(playerMtx, playerAngles);
            goto LABEL_29;
        }
        G_DObjGetWorldBoneIndexMatrix(ent, veh->boneIndex.flash[4 * seatIndex + 2], barrelMtx);
        AxisToAngles(barrelMtx, playerAngles);
LABEL_29:
        if ( info->remoteControl )
        {
            client->ps.linkAngles[0] = client->ps.viewangles[0];
            client->ps.linkAngles[1] = client->ps.viewangles[1];
            client->ps.linkAngles[2] = client->ps.viewangles[2];
        }
        playerAngles[2] = 0.0f;
        playerAngles[0] = AngleNormalize180(playerAngles[0]);
        SetClientViewAngle(player, playerAngles);
        client->ps.linkFlags &= ~1u;
        client->linkAnglesLocked = 0;
        if ( !info->remoteControl && !G_EntLinkToWithOffset(player, ent, *s_seatTags[seatIndex], vec3_origin, vec3_origin) )
        {
            missingSeatName = SL_ConvertToString(*s_seatTags[seatIndex], SCRIPTINSTANCE_SERVER);
            Com_PrintWarning(15, "WARNING: vehicle missing bone '%s'\n", missingSeatName);
            if ( !G_EntLinkToWithOffset(player, ent, scr_const.tag_origin, vec3_origin, vec3_origin) )
            {
                linkFailSeatName = SL_ConvertToString(*s_seatTags[seatIndex], SCRIPTINSTANCE_SERVER);
                Com_Error(ERR_DROP, "VEH_LinkPlayer: Cannot link to vehicle bone [%s]", linkFailSeatName);
            }
        }
        if ( info->type == 6 && seatIndex >= 5 && seatIndex <= 10 )
            client->linkAnglesFrac = 1.0f;
        if ( veh->targetEnt == player->s.number )
        {
            veh->hasTarget = 0;
            veh->targetEnt = 1023;
            //EntHandle::setEnt(&veh->lookAtEnt, 0);
            veh->lookAtEnt.setEnt(0);
        }
        VEH_SetSeatOccupantEntNum(veh, seatIndex, player->s.number);
        if ( !seatIndex )
            veh->flags |= 1u;
        ent->s.lerp.eFlags |= 0x4000u;
        player->s.lerp.u.player.vehicleType = info->type;
        player->s.lerp.u.player.vehicleSeat = seatIndex;
        if (!seatIndex)
        {
            ent->r.ownerNum.setEnt(player);
        }
        //EntHandle::setEnt(&player->r.ownerNum, ent);
        player->r.ownerNum.setEnt(ent);
        player->s.otherEntityNum = ent->s.number;
        client->ps.eFlags |= 0x4000u;
        if ( info->remoteControl )
        {
            ent->s.lerp.eFlags2 |= 0x10000000u;
            client->ps.eFlags2 |= 0x10000000u;
        }
        client->ps.viewlocked_entNum = ent->s.number;
        client->ps.vehicleType = info->type;
        client->ps.vehiclePos = seatIndex;
        if ( seatIndex >= 1 && seatIndex <= 4 )
            client->ps.leanf = 0.0f;
        Com_Printf(
            14,
            "VEH_LinkPlayer: Player %i %s is linking. Seat %i.\n",
            player->s.number,
            player->client->sess.cs.name,
            seatIndex);
        VEH_SetLinkAngleClamps(info, player, seatIndex, playerAngles, veh->phys.angles);
        if ( ent->scr_vehicle->nitrousVehicle && player->client )
            VEH_SetPlayerVehicle(ent, 1);
        if ( info->remoteControl )
            player->r.currentAngles[1] = client->ps.linkAngles[1];
        if ( !changingSeats )
        {
            Scr_AddInt(seatIndex, SCRIPTINSTANCE_SERVER);
            Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
            Scr_Notify(player, scr_const.enter_vehicle, 2u);
            Scr_AddInt(seatIndex, SCRIPTINSTANCE_SERVER);
            Scr_AddEntity(player, SCRIPTINSTANCE_SERVER);
            Scr_Notify(ent, scr_const.enter_vehicle, 2u);
        }
    }
}

// Decomp: CoDMPServer.c:429551  Scr_Vehicle_DamageScale
double    Scr_Vehicle_DamageScale(
                gentity_s *pSelf,
                gentity_s *pAttacker,
                gentity_s *pInflictor,
                const float *point,
                int mod,
                int weapon,
                int *damageFromUnderneath)
{
    float zoneDamageScale;
    float sideZoneScale;
    float verticalOffset;
    float horizontalRadiusSq;
    float maxHeightOffset;
    float maxHorizontalDist;
    float damageBlend;
    int axisIndex;
    int bestAxis;
    float bestDot;
    float vdir[3];
    float axis[3][3];
    float scale;
    int isRadiusDamage;
    float backAngleCosLimit;
    vehicle_physic_t *phys;
    scr_vehicle_s *veh;

    veh = pSelf->scr_vehicle;
    phys = &veh->phys;
    backAngleCosLimit = cos(0.3490658700466156f);
    if ( veh->nitrousVehicle && veh->nitrousVehicle->m_vehicle_info->noDirectionalDamage )
        return 1.0;
    if ( mod == 17 )
        return 0.0;
    isRadiusDamage = mod == 4 || mod == 6;
    scale = 1.0f;
    AnglesToAxis(phys->angles, axis);
    if (point)
    {
        vdir[0] = *point - phys->origin[0];
        vdir[1] = point[1] - phys->origin[1];
        vdir[2] = point[2] - phys->origin[2];
    }
    else
    {
        vdir[0] = 1.0f;
        vdir[1] = 0.0f;
        vdir[2] = 0.0f;
    }
    vdir[2] = 0.0f;
    Vec3Normalize(vdir);
    bestDot = 0.0f;
    bestAxis = -1;
    for ( axisIndex = 0; axisIndex < 2; ++axisIndex )
    {
        if ( bestAxis < 0
            || fabs((float)((float)(vdir[0] * axis[axisIndex][0]) + (float)(vdir[1] * axis[axisIndex][1]))
                         + (float)(vdir[2] * axis[axisIndex][2])) > fabs(bestDot) )
        {
            bestDot = (float)((float)(vdir[0] * axis[axisIndex][0]) + (float)(vdir[1] * axis[axisIndex][1]))
                    + (float)(vdir[2] * axis[axisIndex][2]);
            bestAxis = axisIndex;
        }
    }
    if ((unsigned int)bestAxis >= 2
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
            6371,
            0,
            "%s",
            "bestAxis == 0 || bestAxis == 1"))
    {
        __debugbreak();
    }
    if ( isRadiusDamage )
    {
        zoneDamageScale = 1.0f;
        sideZoneScale = 1.0f;
        maxHorizontalDist = pSelf->r.maxs[1] * 0.80000001f;
        maxHeightOffset = pSelf->r.maxs[2] * 0.5f;
        horizontalRadiusSq = Vec3DistanceSq(point, phys->origin);
        if ( mod == 4 )
        {
            if ( (float)(phys->origin[2] + maxHeightOffset) <= (float)(point[2] - 10.0f)
                || (float)(maxHorizontalDist * maxHorizontalDist) <= horizontalRadiusSq )
            {
                return scale * 1.0f;
            }
            *damageFromUnderneath = 1;
            return scale * vehicle_damage_zone_under->current.value;
        }
        if ( bestAxis == 1 )
        {
            zoneDamageScale = vehicle_damage_zone_side->current.value;
            if ( (float)((float)(vdir[0] * axis[0][0]) + (float)(vdir[1] * axis[0][1]) + (float)(vdir[2] * axis[0][2])) >= 0.0f )
                sideZoneScale = vehicle_damage_zone_front->current.value;
            else
                sideZoneScale = vehicle_damage_zone_rear->current.value;
        }
        else
        {
            if ( bestDot >= 0.0f )
                zoneDamageScale = vehicle_damage_zone_front->current.value;
            else
                zoneDamageScale = vehicle_damage_zone_rear->current.value;
            sideZoneScale = vehicle_damage_zone_side->current.value;
        }
        damageBlend = fabs(bestDot);
        return (zoneDamageScale * damageBlend + (1.0f - damageBlend) * sideZoneScale) * scale * 0.69999999f;
    }
    if ( bestAxis == 1 )
        return scale * vehicle_damage_zone_side->current.value;
    if ( bestAxis != 0 )
        return 0.0;
    if ( bestDot >= 0.0f )
        return scale * vehicle_damage_zone_front->current.value;
    if ( backAngleCosLimit <= -bestDot )
        return scale * vehicle_damage_zone_rear->current.value;
    return scale * vehicle_damage_zone_side->current.value;
}

// Decomp: CoDMPServer.c:429719  VEH_UnlinkPlayer
void __cdecl VEH_UnlinkPlayer(gentity_s *player, bool changingSeats, char *error_msg)
{
    gentity_s *ownerVehicle;
    gentity_s *occupantVehicle;
    float playerAngles[3]; // [esp+14h] [ebp-84h] BYREF
    float barrelMtx[4][3]; // [esp+20h] [ebp-78h] BYREF
    int gunnerIndex; // [esp+50h] [ebp-48h]
    gentity_s *occupant; // [esp+54h] [ebp-44h]
    gclient_s *client; // [esp+58h] [ebp-40h]
    vehicle_physic_t *phys; // [esp+5Ch] [ebp-3Ch]
    const vehicle_info_t *info; // [esp+60h] [ebp-38h]
    bool stillOccupied; // [esp+67h] [ebp-31h]
    scr_vehicle_s *veh; // [esp+68h] [ebp-30h]
    float origin[3]; // [esp+6Ch] [ebp-2Ch] BYREF
    float angles[3]; // [esp+78h] [ebp-20h] BYREF
    int playerSeatIndex; // [esp+84h] [ebp-14h]
    gentity_s *ent; // [esp+88h] [ebp-10h]
    int exitPointIndex;
    int seatIndex; // [esp+90h] [ebp-8h]
    int exitPoint; // [esp+94h] [ebp-4h]

    seatIndex = 0;
    client = player->client;
    if ( !client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 6554, 0, "%s", "client") )
    {
        __debugbreak();
    }
    if ( (client->ps.eFlags & 0x300) == 0 )
    {
        if ( (client->ps.eFlags & 0x4000) == 0 )
            Com_Error(ERR_DROP, "VEH_UnlinkPlayer: Player is not using a vehicle or turret attached to vehicle");
        //if ( !EntHandle::isDefined(&player->r.ownerNum) )
        if ( !player->r.ownerNum.isDefined() )
            Com_Error(
                ERR_DROP,
                "VEH_UnlinkPlayer: Player %i %s doesn',27h,'t has an owner. Seat %i. %s",
                player->s.number,
                player->client->sess.cs.name,
                client->ps.vehiclePos,
                error_msg);
        //ent = EntHandle::ent(&player->r.ownerNum);
        ent = player->r.ownerNum.ent();
        veh = ent->scr_vehicle;
        if ( !veh && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 6575, 0, "%s", "veh") )
            __debugbreak();
        phys = &veh->phys;
        info = BG_GetVehicleInfo(veh->infoIdx);
        playerSeatIndex = G_GetVehicleSeatPlayerOccupies(ent, player);
        if ( info->type != 3 && playerSeatIndex < 0 )
            Com_Error(ERR_DROP, "VEH_UnlinkPlayer: Player is not in a vehicle seat");
        if ( !playerSeatIndex )
        {
            veh->flags &= ~1u;
            //EntHandle::setEnt(&ent->r.ownerNum, 0);
            ent->r.ownerNum.setEnt(0);
        }
        if ( info->remoteControl )
        {
            origin[0] = player->r.currentOrigin[0];
            origin[1] = player->r.currentOrigin[1];
            origin[2] = player->r.currentOrigin[2];
        }
        else
        {
            origin[0] = ent->r.currentOrigin[0];
            origin[1] = ent->r.currentOrigin[1];
            origin[2] = ent->r.currentOrigin[2];
            origin[2] = origin[2] + phys->maxs[2];
            exitPoint = G_GetEntryPointForSeat(ent, playerSeatIndex);
            if ( !VEH_ExitPosOkay(ent, player, exitPoint, origin) )
            {
                for ( exitPointIndex = 0; exitPointIndex < 5 && (exitPointIndex == exitPoint || !VEH_ExitPosOkay(ent, player, exitPointIndex, origin)); ++exitPointIndex )
                    ;
                if ( exitPointIndex == 5 )
                {
                    origin[0] = ent->r.currentOrigin[0];
                    origin[1] = ent->r.currentOrigin[1];
                    origin[2] = ent->r.currentOrigin[2];
                    origin[2] = origin[2] + phys->maxs[2];
                }
            }
        }
        if ( playerSeatIndex >= 0 )
            VEH_SetSeatOccupantEntNum(veh, playerSeatIndex, 0x3FFu);
        stillOccupied = 0;
        if ( player->r.ownerNum.ent() == ent )
        {
            for ( seatIndex = 0; seatIndex < 11 && !stillOccupied; ++seatIndex )
            {
                occupant = VEH_GetSeatOccupantEntity(veh, seatIndex);
                if ( occupant )
                {
                    occupantVehicle = occupant->r.ownerNum.ent();
                    if ( occupantVehicle != ent
                        && !Assert_MyHandler(
                                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                                    6638,
                                    0,
                                    "%s",
                                    "occupant->r.ownerNum.ent() == ent") )
                    {
                        __debugbreak();
                    }
                    stillOccupied = 1;
                }
            }
        }
        if ( !stillOccupied )
            ent->s.lerp.eFlags &= ~0x4000u;
        G_EntUnlink(player);
        if ( info->remoteControl )
        {
            angles[0] = player->r.currentAngles[0];
            angles[1] = player->r.currentAngles[1];
            angles[2] = player->r.currentAngles[2];
        }
        else
        {
            angles[0] = client->ps.viewangles[0];
            angles[1] = client->ps.viewangles[1];
            angles[2] = client->ps.viewangles[2];
        }
        angles[2] = 0.0f;
        TeleportPlayer(player, origin, angles);
        client->ps.eFlags &= 0xFFFEBFFF;
        if (!error_msg)
        {
            //MEMORY[0] = 0; // cool random null deref dude
            char *err = NULL;
            *err = 0;
        }
        Com_Printf(
            14,
            "VEH_UnlinkPlayer: Player %i %s is unlinking. Seat %i. %s\n",
            player->s.number,
            player->client->sess.cs.name,
            playerSeatIndex,
            error_msg);
        player->r.ownerNum.setEnt(0);
        client->ps.viewlocked_entNum = 1023;
        client->ps.eFlags &= ~0x4000u;
        player->s.lerp.eFlags &= ~0x4000u;
        client->ps.eFlags2 &= ~0x10000000u;
        player->s.lerp.eFlags2 &= ~0x10000000u;
        if ( !error_msg || I_stricmp("ClientDisconnect", error_msg) )
        {
            if ( playerSeatIndex >= 0 )
            {
                seatIndex = client->ps.vehiclePos;
                if ( playerSeatIndex != seatIndex
                    && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                                6712,
                                0,
                                "%s",
                                "playerSeatIndex == seatIndex") )
                {
                    __debugbreak();
                }
                if ( seatIndex >= 1 && seatIndex <= 4 )
                {
                    gunnerIndex = seatIndex - 1;
                    if ( veh->boneIndex.flash[4 * seatIndex + 2] >= 0 )
                    {
                        G_DObjGetWorldBoneIndexMatrix(ent, veh->boneIndex.gunnerTags[gunnerIndex].barrel, barrelMtx);
                        AxisToAngles(barrelMtx, playerAngles);
                        playerAngles[2] = 0.0f;
                        SetClientViewAngle(player, playerAngles);
                    }
                }
            }
            if ( veh->nitrousVehicle )
                VEH_SetPlayerVehicle(ent, 0);
            if ( !changingSeats )
            {
                if ( info->remoteControl )
                    SetClientViewAngle(player, client->ps.linkAngles);
                Scr_AddInt(seatIndex, SCRIPTINSTANCE_SERVER);
                Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
                Scr_Notify(player, scr_const.exit_vehicle, 2u);
                Scr_AddInt(seatIndex, SCRIPTINSTANCE_SERVER);
                Scr_AddEntity(player, SCRIPTINSTANCE_SERVER);
                Scr_Notify(ent, scr_const.exit_vehicle, 2u);
            }
        }
    }
}

// Decomp: CoDMPServer.c:429927  VEH_ExitPosOkay
char __cdecl VEH_ExitPosOkay(gentity_s *vehEnt, gentity_s *player, int exitIndex, float *origin)
{
    float upOffset;
    float pushOutVec[3];
    col_context_t context;
    float exitMat[4][3];
    float traceEnd[3];
    trace_t traceResults; // [esp+90h] [ebp-60h] BYREF
    float clampedExitPoint[3]; // [esp+CCh] [ebp-24h] BYREF
    float safeVehicleOrigin[3]; // [esp+D8h] [ebp-18h] BYREF
    float exitPoint[3]; // [esp+E4h] [ebp-Ch] BYREF

    memset(&traceResults, 0, sizeof(traceResults));
    //col_context_t::col_context_t(&context);
    if ( vehEnt->scr_vehicle->boneIndex.entryPoints[exitIndex] < 0 )
        return 0;
    G_DObjGetWorldBoneIndexMatrix(vehEnt, vehEnt->scr_vehicle->boneIndex.entryPoints[exitIndex], exitMat);
    //*(_QWORD *)exitPoint = *(_QWORD *)&exitMat[3][0];
    exitPoint[0] = exitMat[3][0];
    exitPoint[1] = exitMat[3][1];
    exitPoint[2] = exitMat[3][2];
    if ( vehicle_riding->current.enabled )
    {
        *origin = exitPoint[0];
        origin[1] = exitPoint[1];
        origin[2] = exitPoint[2];
        traceEnd[0] = exitPoint[0];
        traceEnd[1] = exitPoint[1];
        traceEnd[2] = exitPoint[2];
        upOffset = exitPointOffsetUp;
        *origin = (float)(exitPointOffsetUp * exitMat[2][0]) + *origin;
        origin[1] = (float)(upOffset * exitMat[2][1]) + origin[1];
        origin[2] = (float)(upOffset * exitMat[2][2]) + origin[2];
        traceEnd[0] = *origin;
        traceEnd[1] = origin[1];
        traceEnd[2] = origin[2] - (float)(exitPointOffsetDown + exitPointOffsetUp);
        G_TraceCapsule(
            &traceResults,
            origin,
            player->r.mins,
            player->r.maxs,
            traceEnd,
            player->s.number,
            player->clipmask,
            &context);
        Vec3Lerp(origin, traceEnd, traceResults.fraction, origin);
        return 1;
    }
    else
    {
        exitPoint[2] = exitPoint[2] + exitPointOffsetDown;
        if ( !VEH_ClampPointToBoundingBox(vehEnt, exitPoint, clampedExitPoint) )
        {
            pushOutVec[0] = clampedExitPoint[0] - exitPoint[0];
            pushOutVec[1] = clampedExitPoint[1] - exitPoint[1];
            pushOutVec[2] = 0.0f;
            Vec3NormalizeFast(pushOutVec);
            exitPoint[0] = (float)(100.0 * pushOutVec[0]) + exitPoint[0];
            exitPoint[1] = (float)(100.0 * pushOutVec[1]) + exitPoint[1];
            exitPoint[2] = (float)(100.0 * pushOutVec[2]) + exitPoint[2];
        }
        safeVehicleOrigin[0] = vehEnt->r.currentOrigin[0];
        safeVehicleOrigin[1] = vehEnt->r.currentOrigin[1];
        safeVehicleOrigin[2] = vehEnt->r.currentOrigin[2];
        safeVehicleOrigin[2] = safeVehicleOrigin[2] + 30.0;
        G_TraceCapsule(
            &traceResults,
            safeVehicleOrigin,
            player->r.mins,
            player->r.maxs,
            exitPoint,
            vehEnt->s.number,
            player->clipmask,
            &context);
        if ( traceResults.fraction >= 1.0 )
        {
            traceEnd[0] = exitPoint[0];
            traceEnd[1] = exitPoint[1];
            traceEnd[2] = exitPoint[2] - exitPointOffsetDown;
            G_TraceCapsule(
                &traceResults,
                exitPoint,
                player->r.mins,
                player->r.maxs,
                traceEnd,
                vehEnt->s.number,
                player->clipmask,
                &context);
            Vec3Lerp(exitPoint, traceEnd, traceResults.fraction, origin);
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

// Decomp: CoDMPServer.c:430058  G_VehicleFinishedAnimating
void __cdecl G_VehicleFinishedAnimating(gentity_s *player, pmoveVehAnimState_t vehAnimState)
{
    gentity_s *vehicle; // [esp+0h] [ebp-4h]

    //vehicle = EntHandle::ent(&player->r.ownerNum);
    vehicle = player->r.ownerNum.ent();
    if ( vehAnimState == PMOVE_VEH_ANIM_STATE_COMPLETE )
    {
        if ( player->client->ps.vehicleAnimStage < 8 )
        {
            VEH_LinkPlayer(vehicle, player, player->client->ps.vehiclePos, 0);
            player->client->ps.eFlags &= ~0x10000u;
            player->client->ps.vehicleAnimStage = 0;
            player->client->ps.vehicleAnimBoneIndex = 0;
        }
        else
        {
            if ( (player->client->ps.eFlags & 0x4000) == 0
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                            6763,
                            0,
                            "%s",
                            "player->client->ps.eFlags & EF_VEHICLE_ACTIVE") )
            {
                __debugbreak();
            }
            if ( !vehicle
                && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 6764, 0, "%s", "vehicle") )
            {
                __debugbreak();
            }
            VEH_UnlinkPlayer(player, 0, (char*)"G_VehicleFinishedAnimating");
        }
    }
}

// Decomp: CoDMPServer.c:430102  VEH_GetNextSeat
int __cdecl VEH_GetNextSeat(const vehicle_info_t *info, int currentSeat)
{
    int seatOrderIndex;
    int seatOrderWalkIndex;

    for ( seatOrderIndex = 0; ; ++seatOrderIndex )
    {
        if ( seatOrderIndex >= 11 )
            return -1;
        if ( info->seatSwitchOrder[seatOrderIndex] == currentSeat )
            break;
    }
    if ( seatOrderIndex != 10 && info->seatSwitchOrder[seatOrderIndex + 1] >= 0 )
        return info->seatSwitchOrder[seatOrderIndex + 1];
    for ( seatOrderWalkIndex = seatOrderIndex - 1; seatOrderWalkIndex >= 0; --seatOrderWalkIndex )
    {
        if ( info->seatSwitchOrder[seatOrderWalkIndex] < 0 )
            return info->seatSwitchOrder[seatOrderWalkIndex + 1];
    }
    return info->seatSwitchOrder[0];
}

// Decomp: CoDMPServer.c:430125  VEH_NextAvailableSeat
int __cdecl VEH_NextAvailableSeat(gentity_s *player)
{
    const vehicle_info_t *info; // [esp+4h] [ebp-10h]
    gentity_s *vehicle; // [esp+8h] [ebp-Ch]
    int playerSeatIndex; // [esp+Ch] [ebp-8h]
    int seatIndex; // [esp+10h] [ebp-4h]

    if ( !player
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 6851, 0, "%s", "player") )
    {
        __debugbreak();
    }
    if ( !player->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 6852, 0, "%s", "player->client") )
    {
        __debugbreak();
    }
    if ( player->client->ps.viewlocked_entNum != 1023 )
    {
        vehicle = &g_entities[player->client->ps.viewlocked_entNum];
        if ( !vehicle
            && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 6857, 0, "%s", "vehicle") )
        {
            __debugbreak();
        }
        if ( vehicle->scr_vehicle )
        {
            playerSeatIndex = G_GetVehicleSeatPlayerOccupies(vehicle, player);
            info = BG_GetVehicleInfo(vehicle->scr_vehicle->infoIdx);
            seatIndex = VEH_GetNextSeat(info, playerSeatIndex);
            while ( seatIndex >= 0 && seatIndex != playerSeatIndex )
            {
                if ( VEH_IsSeatPresent(vehicle, seatIndex, 0) )
                {
                    if ( !G_IsVehicleSeatOccupied(vehicle, seatIndex) )
                        return seatIndex;
                    seatIndex = VEH_GetNextSeat(info, seatIndex);
                }
            }
        }
    }
    return -1;
}

// Decomp: CoDMPServer.c:430188  VEH_SwitchClientToNextSeat
void __cdecl VEH_SwitchClientToNextSeat(gentity_s *ent, gentity_s *player)
{
    int nextSeat; // [esp+4h] [ebp-4h]

    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 6889, 0, "%s", "ent") )
        __debugbreak();
    if ( !player
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 6890, 0, "%s", "player") )
    {
        __debugbreak();
    }
    if ( !player->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 6891, 0, "%s", "player->client") )
    {
        __debugbreak();
    }
    if ( vehicle_switch_seat_delay->current.value <= (float)((float)(level.time - player->client->switchSeatTime) / 1000.0) )
    {
        nextSeat = VEH_NextAvailableSeat(player);
        if ( nextSeat >= 0 )
        {
            player->client->switchSeatTime = level.time;
            VEH_SwitchClientToSeat(ent, player, nextSeat);
        }
    }
}

// Decomp: CoDMPServer.c:430240  VEH_SwitchClientToSeat
void __cdecl VEH_SwitchClientToSeat(gentity_s *ent, gentity_s *player, int seatIndex)
{
    int oldSeatIndex; // [esp+0h] [ebp-4h]

    oldSeatIndex = G_GetVehicleSeatPlayerOccupies(ent, player);
    if ( (player->client->ps.eFlags & 0x4000) == 0
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    6787,
                    0,
                    "%s",
                    "player->client->ps.eFlags & EF_VEHICLE_ACTIVE") )
    {
        __debugbreak();
    }
    //if ( !EntHandle::isDefined(&player->r.ownerNum)
    if ( !player->r.ownerNum.isDefined()
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    6788,
                    0,
                    "%s",
                    "player->r.ownerNum.isDefined()") )
    {
        __debugbreak();
    }
    VEH_UnlinkPlayer(player, 1, (char*)"VEH_SwitchClientToSeat");
    VEH_LinkPlayer(ent, player, seatIndex, 1);
    Scr_AddInt(seatIndex, SCRIPTINSTANCE_SERVER);
    Scr_AddInt(oldSeatIndex, SCRIPTINSTANCE_SERVER);
    Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
    Scr_Notify(player, scr_const.change_seat, 3u);
    Scr_AddInt(seatIndex, SCRIPTINSTANCE_SERVER);
    Scr_AddInt(oldSeatIndex, SCRIPTINSTANCE_SERVER);
    Scr_AddEntity(player, SCRIPTINSTANCE_SERVER);
    Scr_Notify(ent, scr_const.change_seat, 3u);
    if ( !seatIndex )
        Scr_Notify(player, scr_const.vehicle_driver, 0);
}

// Decomp: CoDMPServer.c:430282  G_ParseScrVehicleInfo
void __cdecl G_ParseScrVehicleInfo()
{
    bg_numVehicleInfos = 0;
}

// Decomp: CoDMPServer.c:430288  VEH_RinitVehiclesUsingInfo
void __cdecl VEH_RinitVehiclesUsingInfo(int infoIdx)
{
    int vehicleSlotIndex;
    const vehicle_info_t *info; // [esp+Ch] [ebp-8h]
    scr_vehicle_s *veh; // [esp+10h] [ebp-4h]

    info = BG_GetVehicleInfo(infoIdx);
    for ( vehicleSlotIndex = 0; vehicleSlotIndex < MAX_VEHICLES; ++vehicleSlotIndex )
    {
        veh = &s_vehicles[vehicleSlotIndex];
        if ( veh->entNum != 1023 && veh->infoIdx == infoIdx )
        {
            VEH_InitFromInfo(veh, info);
            SV_SendServerCommand(
                0,
                SV_CMD_RELIABLE,
                "%c %d %d",
                61,
                veh->entNum,
                g_entities[veh->entNum].s.lerp.eFlags2 & 0x2000000);
        }
    }
}

// Decomp: CoDMPServer.c:430314  G_VehicleUpdateField
char __cdecl G_VehicleUpdateField(const char *vehicleInfoName, char *keyValue)
{
    const vehicle_info_t *info; // [esp+0h] [ebp-8h]
    int vehicleInfoIndex;

    if ( !vehicleInfoName || !*vehicleInfoName )
        return 0;
    for ( vehicleInfoIndex = 0; vehicleInfoIndex < bg_numVehicleInfos; ++vehicleInfoIndex )
    {
        info = BG_GetVehicleInfo(vehicleInfoIndex);
        if ( !I_stricmp(vehicleInfoName, info->name) )
        {
            ParseConfigStringToStruct(
                (unsigned __int8 *)info,
                s_vehicleFields,
                s_numVehicleFields,
                keyValue,
                25,
                (int (__cdecl *)(unsigned __int8 *, const char *, const int, const int))VEH_ParseSpecificField,
                BG_StringCopy);
            NitrousVehicle::reinit_parms();
            VEH_RinitVehiclesUsingInfo(vehicleInfoIndex);
        }
    }
    return 1;
}

// Decomp: CoDMPServer.c:430343  G_ReloadScrVehicleInfo
void G_ReloadScrVehicleInfo()
{
    char fileName[64];
    char loadBuffer[16384];
    int vehicleInfoIndex;

    for ( vehicleInfoIndex = 0; vehicleInfoIndex < bg_numVehicleInfos; ++vehicleInfoIndex )
    {
        vehicle_info_t *vehicleInfo = const_cast<vehicle_info_t *>(BG_GetVehicleInfo(vehicleInfoIndex));
        sprintf(fileName, "vehicles/%s", vehicleInfo->name);
        char *pszBuffer = Com_LoadInfoString(fileName, "vehicle file", "VEHICLEFILE", loadBuffer);
        if ( pszBuffer
            && ParseConfigStringToStruct(
                (unsigned __int8 *)vehicleInfo,
                s_vehicleFields,
                s_numVehicleFields,
                pszBuffer,
                25,
                (int (__cdecl *)(unsigned __int8 *, const char *, const int, const int))VEH_ParseSpecificField,
                BG_StringCopy) )
        {
            int gunnerIndex;
            int axisIndex;

            for ( gunnerIndex = 0; gunnerIndex < 4; ++gunnerIndex )
            {
                for ( axisIndex = 0; axisIndex < 2; ++axisIndex )
                {
                    float *restAngle = &vehicleInfo->gunnerRestAngles[gunnerIndex][axisIndex];

                    *restAngle = (*restAngle * 0.5f) * 0.017453292f;
                }
            }
        }
    }
    NitrousVehicle::reinit_parms();
}

// Decomp: CoDMPServer.c:430383  G_InitScrVehicles
void __cdecl G_InitScrVehicles()
{
    int vehicleSlotIndex;
    int tiltIndex;

    for ( vehicleSlotIndex = 0; vehicleSlotIndex < MAX_VEHICLES; ++vehicleSlotIndex )
    {
        G_VehInitPathPos(&s_vehicles[vehicleSlotIndex].pathPos);
        s_vehicles[vehicleSlotIndex].entNum = 1023;
    }
    for ( tiltIndex = 0; tiltIndex < 1; ++tiltIndex )
    {
        s_pitches[tiltIndex] = 0.0f;
        s_rolls[tiltIndex] = 0.0f;
    }
    level.vehicles = s_vehicles;
}


// Decomp: CoDMPServer.c:430401  G_SetupScrVehicles
void __cdecl G_SetupScrVehicles()
{
    gentity_s *ent;
    int vehicleSlotIndex;

    for ( vehicleSlotIndex = 0; vehicleSlotIndex < MAX_VEHICLES; ++vehicleSlotIndex )
    {
        if ( s_vehicles[vehicleSlotIndex].entNum != 1023 )
        {
            ent = VEH_GetVehicle(s_vehicles[vehicleSlotIndex].entNum);
            VEH_SetupCollmap(ent);
        }
    }
}

// Decomp: CoDMPServer.c:430422  VEH_SetupCollmap
void __cdecl VEH_SetupCollmap(gentity_s *ent)
{
    unsigned int modelNameIndex;
    char *modelName;
    gentity_s *cmEnt;

    ent->r.contents = 8462464;
    if ( (ent->spawnflags & 1) != 0 )
        ent->r.contents |= 0x200000u;
    modelNameIndex = G_ModelName(ent->model);
    modelName = SL_ConvertToString(modelNameIndex, SCRIPTINSTANCE_SERVER);
    cmEnt = VEH_GetCollMap(modelName);
    if ( cmEnt )
    {
        if ( cmEnt->s.index.brushmodel )
        {
            ent->s.index.brushmodel = cmEnt->s.index.brushmodel;
            SV_SetBrushModel(ent);
            ent->r.contents = 8462464;
            if ( (ent->spawnflags & 1) != 0 )
                ent->r.contents |= 0x200000u;
        }
        else
        {
            modelNameIndex = G_ModelName(ent->model);
            modelName = SL_ConvertToString(modelNameIndex, SCRIPTINSTANCE_SERVER);
            Com_PrintWarning(15, "WARNING: Cannot use empty vehicle collmap for [%s]\n", modelName);
        }
    }
}

// Decomp: CoDMPServer.c:430456  VEH_GetCollMap
gentity_s *__cdecl VEH_GetCollMap(const char *modelname)
{
    char *targetname;
    gentity_s *ent;
    int entityIndex;

    for ( entityIndex = 0; entityIndex < level.num_entities; ++entityIndex )
    {
        ent = &g_entities[entityIndex];
        if ( ent->r.inuse && ent->classname == scr_const.script_vehicle_collmap )
        {
            targetname = SL_ConvertToString(ent->targetname, SCRIPTINSTANCE_SERVER);
            if ( Com_IsLegacyXModelName(targetname) )
                targetname += 7;
            if ( !I_stricmp(targetname, modelname) )
                return &g_entities[entityIndex];
        }
    }
    return 0;
}

// Decomp: CoDMPServer.c:430478  G_FreeScrVehicles
void __cdecl G_FreeScrVehicles()
{
    int vehicleSlotIndex;

    for ( vehicleSlotIndex = 0; vehicleSlotIndex < MAX_VEHICLES; ++vehicleSlotIndex )
        G_VehFreePathPos(&s_vehicles[vehicleSlotIndex].pathPos);
}

// Decomp: CoDMPServer.c:430490  G_SpawnVehicle
void __cdecl G_SpawnVehicle(gentity_s *ent, char *typeName, int load)
{
    const char *precacheError;
    unsigned int weapIdx;
    const vehicle_info_t *info; // [esp+30h] [ebp-10h]
    scr_vehicle_s *veh;
    int infoIdx;
    int vehicleSlotIndex;
    int cacheSlotIndex;

    veh = 0;
    for ( vehicleSlotIndex = 0; vehicleSlotIndex < MAX_VEHICLES; ++vehicleSlotIndex )
    {
        veh = &s_vehicles[vehicleSlotIndex];
        if ( veh->entNum == 1023 )
            break;
    }
    if ( vehicleSlotIndex == MAX_VEHICLES )
        Com_Error(ERR_DROP, "Hit max vehicle count [%d]", MAX_VEHICLES);
    infoIdx = VEH_GetVehicleInfoFromName(typeName);
    if ( infoIdx < 0 )
        Com_Error(ERR_DROP, "Can't find info for script vehicle [%s]", typeName);
    VEH_InitModelAndValidateTags(ent, &infoIdx);
    if ( !level.initializing )
    {
        info = BG_GetVehicleInfo(infoIdx);
        weapIdx = G_GetWeaponIndexForName((char*)info->turretWeapon);
        if ( !IsItemRegistered(weapIdx) )
        {
            precacheError = va("vehicle '%s' not precached", info->name);
            Scr_Error(precacheError, 0);
        }
    }
    memset((unsigned __int8 *)veh, 0, sizeof(scr_vehicle_s));
    // memset clears proximity_data's vtable; reconstruct the polymorphic subobject in place (retail ctor path).
    new (&veh->vehicle_cache.proximity_data) colgeom_visitor_inlined_t<200>();
    veh->vehicle_cache.proximity_data.reset();
    for ( cacheSlotIndex = 0; cacheSlotIndex < 6; ++cacheSlotIndex )
    {
        veh->vehicle_cache.hit_indices[cacheSlotIndex] = -1;
        veh->vehicle_cache.lastOrigin[0] = FLT_MAX;
        veh->vehicle_cache.lastOrigin[1] = FLT_MAX;
        veh->vehicle_cache.lastOrigin[2] = FLT_MAX;
        veh->vehicle_cache.lastAngles[0] = FLT_MAX;
        veh->vehicle_cache.lastAngles[1] = FLT_MAX;
        veh->vehicle_cache.lastAngles[2] = FLT_MAX;
    }
    veh->targetEnt = 1023;
    iassert(!veh->lookAtEnt.isDefined());

    VEH_InitEntity(ent, veh, infoIdx);
    VEH_InitVehicle(ent, veh, infoIdx);
    if ( !load )
        VEH_SetupCollmap(ent);
    G_UpdateVehicleTags(ent);
    if ( (ent->spawnflags & 2) != 0 )
    {
        G_FreeVehicle(ent);
        ent->s.eType = ET_VEHICLE_CORPSE;
    }
}

// Decomp: CoDMPServer.c:430574  VEH_GetVehicleInfoFromName
int __cdecl VEH_GetVehicleInfoFromName(char *name)
{
    const vehicle_info_t *info;
    const vehicle_info_t *defaultInfo;
    int vehicleInfoIndex;
    int loadedInfoIndex;
    int defaultInfoIndex;
    int fallbackInfoIndex;

    if ( !name || !*name )
        return -1;
    for ( vehicleInfoIndex = 0; vehicleInfoIndex < bg_numVehicleInfos; ++vehicleInfoIndex )
    {
        info = BG_GetVehicleInfo(vehicleInfoIndex);
        if ( !I_stricmp(name, info->name) )
            return vehicleInfoIndex;
    }
    loadedInfoIndex = G_LoadVehicle(name);
    if ( loadedInfoIndex >= 0 )
        return loadedInfoIndex;
    Com_PrintWarning(15, "WARNING: couldn't find vehicle info for '%s', attempting to use 'defaultvehicle'.\n", name);
    for ( defaultInfoIndex = 0; defaultInfoIndex < bg_numVehicleInfos; ++defaultInfoIndex )
    {
        defaultInfo = BG_GetVehicleInfo(defaultInfoIndex);
        if ( !I_stricmp("defaultvehicle_mp", defaultInfo->name) )
            return defaultInfoIndex;
    }
    fallbackInfoIndex = G_LoadVehicle((char*)"defaultvehicle_mp");
    if ( fallbackInfoIndex >= 0 )
        return fallbackInfoIndex;
    Com_Error(ERR_DROP, "Cannot find vehicle info for 'defaultvehicle'. This is a default vehicle that you should have.");
    return -1;
}

#if 0
// Decomp: CoDMPServer.c:430613  G_LoadVehicle
int __cdecl G_LoadVehicle(char *name)
{
    double v2; // xmm0_8
    int AliasId; // eax
    long double v4; // [esp+0h] [ebp-4070h]
    unsigned __int8 *v5; // [esp+8h] [ebp-4068h]
    char *v6; // [esp+Ch] [ebp-4064h]
    int i; // [esp+14h] [ebp-405Ch]
    char string[68]; // [esp+18h] [ebp-4058h] BYREF
    int j; // [esp+5Ch] [ebp-4014h]
    char *pszBuffer; // [esp+60h] [ebp-4010h]
    unsigned __int8 *dst; // [esp+64h] [ebp-400Ch]
    char loadBuffer[16384]; // [esp+68h] [ebp-4008h] BYREF
    int index; // [esp+406Ch] [ebp-4h]

    if ( !name && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 831, 0, "%s", "name") )
        __debugbreak();
    if ( level.initializing )
    {
        sprintf(string, "vehicles/%s", name);
        pszBuffer = Com_LoadInfoString(string, "vehicle file", "VEHICLEFILE", loadBuffer);
        index = bg_numVehicleInfos;
        dst = (unsigned __int8 *)BG_GetVehicleInfo(bg_numVehicleInfos);
        memset(dst, 0, 0x1DD8u);
        v6 = name;
        v5 = dst;
        HIDWORD(v4) = dst;
        do
        {
            BYTE3(v4) = *v6;
            *v5++ = *v6++;
        }
        while ( BYTE3(v4) );
        if ( ParseConfigStringToStruct(
                     dst,
                     s_vehicleFields,
                     s_numVehicleFields,
                     pszBuffer,
                     25,
                     (int (__cdecl *)(unsigned __int8 *, const char *, const int, const int))VEH_ParseSpecificField,
                     BG_StringCopy) )
        {
            for ( angleIndex = 0; angleIndex < 4; ++angleIndex )
            {
                v2 = (float)((float)(*(float *)&dst[4 * angleIndex + 2788] * 0.5) * 0.017453292);
                __libm_sse2_cos(v4);
                *(float *)&v2 = v2;
                *(unsigned int *)&dst[4 * angleIndex + 2788] = LODWORD(v2);
            }
            for ( soundAliasIndex = 0; soundAliasIndex < 19; ++soundAliasIndex )
            {
                if ( dst[64 * soundAliasIndex + 1168] )
                {
                    AliasId = SND_FindAliasId((char *)&dst[64 * soundAliasIndex + 1168]);
                    *(unsigned int *)&dst[4 * soundAliasIndex + 2384] = AliasId;
                }
                else
                {
                    *(unsigned int *)&dst[4 * soundAliasIndex + 2384] = 0;
                }
            }
            if ( (unsigned int)bg_numVehicleInfos >= 0x20
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                            875,
                            0,
                            "%s\n\t(bg_numVehicleInfos) = %i",
                            "(bg_numVehicleInfos >= 0 && bg_numVehicleInfos < 32)",
                            bg_numVehicleInfos) )
            {
                __debugbreak();
            }
            SV_SetConfigstring(bg_numVehicleInfos + 3148, name);
            ++bg_numVehicleInfos;
            return index;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        Com_PrintError(15, "ERROR: Trying to load vehicle %s inside the game. The vehicle needs to be precached.\n", name);
        return -1;
    }
}
#else // aislop
// Decomp: CoDMPServer.c:430613  G_LoadVehicle
int __cdecl G_LoadVehicle(char *name)
{
    int index;
    int angleIndex;
    int soundAliasIndex;
    int aliasId;
    char path[68];
    char loadBuffer[16384];
    char *buffer;
    unsigned char *vehicle;
    float *angles;
    char *soundName;
    int *aliasSlot;

    if (!name)
    {
        iassert(!"G_LoadVehicle: name is NULL");
        return -1;
    }

    if (!level.initializing)
    {
        Com_PrintError(
            15,
            "ERROR: Trying to load vehicle %s inside the game. The vehicle needs to be precached.\n",
            name
        );
        return -1;
    }

    if (bg_numVehicleInfos >= 32)
    {
        iassert(!"bg_numVehicleInfos >= 32");
        return -1;
    }

    sprintf(path, "vehicles/%s", name);

    buffer = Com_LoadInfoString(
        path,
        "vehicle file",
        "VEHICLEFILE",
        loadBuffer
    );

    if (!buffer)
        return -1;

    index = bg_numVehicleInfos;

    vehicle = (unsigned char *)BG_GetVehicleInfo(index);

    memset(vehicle, 0, 0x1DD8);

    /* first field is vehicle name string */
    strcpy((char *)vehicle, name);

    if (!ParseConfigStringToStruct(
        vehicle,
        s_vehicleFields,
        s_numVehicleFields,
        buffer,
        25,
        (int(__cdecl *)(unsigned char *, const char *, const int, const int))VEH_ParseSpecificField,
        BG_StringCopy))
    {
        return -1;
    }

    /* ---- Fixup 1: convert 4 angle values ---- */
    angles = (float *)(vehicle + 2788);

    for (angleIndex = 0; angleIndex < 4; ++angleIndex)
        angles[angleIndex] = (angles[angleIndex] * 0.5f) * 0.017453292f;

    for (soundAliasIndex = 0; soundAliasIndex < 19; ++soundAliasIndex)
    {
        soundName = (char *)(vehicle + 1168 + (64 * soundAliasIndex));
        aliasSlot = (int *)(vehicle + 2384 + (4 * soundAliasIndex));

        if (soundName[0])
        {
            aliasId = SND_FindAliasId(soundName);
            *aliasSlot = aliasId;
        }
        else
        {
            *aliasSlot = 0;
        }
    }

    SV_SetConfigstring(index + 3148, name);

    ++bg_numVehicleInfos;

    return index;
}
#endif


// Decomp: CoDMPServer.c:430720  VEH_InitModelAndValidateTags
void __cdecl VEH_InitModelAndValidateTags(gentity_s *ent, int *infoIdx)
{
    const char *assertMsg;
    unsigned int modelName;
    char *modelNameStr;
    int defaultInfoIdx;
    bool isDefault;

    defaultInfoIdx = VEH_GetVehicleInfoFromName((char*)"defaultvehicle_mp");
    isDefault = 0;
    if ( *infoIdx == defaultInfoIdx )
    {
        isDefault = 1;
        G_SetModel(ent, (char *)"defaultvehicle");
        *infoIdx = defaultInfoIdx;
    }
    else if ( ent->model && G_XModelBad(ent->model) )
    {
        isDefault = 1;
        G_OverrideModel(ent->model, (char *)"defaultvehicle");
        if ( G_XModelBad(ent->model) )
        {
            assertMsg = va(
                         "Tried to set default vehicle model but it does not exist Entity:%d Origin %.1f %.1f %.1f",
                         ent->s.number,
                         ent->r.currentOrigin[0],
                         ent->r.currentOrigin[1],
                         ent->r.currentOrigin[2]);
            if ( !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                            1116,
                            0,
                            "%s\n\t%s",
                            "!G_XModelBad( ent->model )",
                            assertMsg) )
                __debugbreak();
        }
        *infoIdx = defaultInfoIdx;
    }
    ent->s.eType = ET_VEHICLE;
    G_DObjUpdate(ent);
    if ( !VEH_DObjHasRequiredTags(ent, *infoIdx) )
    {
        if ( isDefault )
            Com_Error(ERR_DROP, "ERROR: default vehicle is missing a required tag!");
        modelName = G_ModelName(ent->model);
        modelNameStr = SL_ConvertToString(modelName, SCRIPTINSTANCE_SERVER);
        Com_PrintWarning(
            15,
            "WARNING: vehicle '%s' is missing a required tag! switching to default vehicle model and info.\n",
            modelNameStr);
        G_SetModel(ent, (char *)"defaultvehicle");
        *infoIdx = defaultInfoIdx;
        G_DObjUpdate(ent);
        if ( !VEH_DObjHasRequiredTags(ent, *infoIdx) )
            Com_Error(ERR_DROP, "ERROR: default vehicle is missing a required tag!");
    }
}

// Decomp: CoDMPServer.c:430785  VEH_DObjHasRequiredTags
char __cdecl VEH_DObjHasRequiredTags(gentity_s *ent, int infoIdx)
{
    unsigned int modelName;
    char *modelNameStr;
    char *wheelTagName;
    char *seatTagName;
    const vehicle_info_t *info;
    int numWheels;
    int wheelIndex;
    int seatIndex;

    info = BG_GetVehicleInfo(infoIdx);
    if ( !info->type || info->type == 2 )
    {
        numWheels = info->type != 0 ? 6 : 4;
        for ( wheelIndex = 0; wheelIndex < numWheels; ++wheelIndex )
        {
            if ( SV_DObjGetBoneIndex(ent, *s_wheelTags[wheelIndex]) < 0 )
            {
                wheelTagName = SL_ConvertToString(*s_wheelTags[wheelIndex], SCRIPTINSTANCE_SERVER);
                modelName = G_ModelName(ent->model);
                modelNameStr = SL_ConvertToString(modelName, SCRIPTINSTANCE_SERVER);
                Com_PrintWarning(15, "WARNING: vehicle model '%s' is missing '%s'\n", modelNameStr, wheelTagName);
                return 0;
            }
        }
    }
    if ( !info->type || info->type == 2 || info->type == 4 || info->type == 1 )
    {
        for ( seatIndex = 0; seatIndex < 11; ++seatIndex )
        {
            if ( VEH_IsSeatPresent(ent, seatIndex, info) )
            {
                if ( SV_DObjGetBoneIndex(ent, *s_seatTags[seatIndex]) < 0 )
                {
                    seatTagName = SL_ConvertToString(*s_seatTags[seatIndex], SCRIPTINSTANCE_SERVER);
                    modelName = G_ModelName(ent->model);
                    modelNameStr = SL_ConvertToString(modelName, SCRIPTINSTANCE_SERVER);
                    Com_PrintWarning(15, "WARNING: vehicle model '%s' is missing '%s'\n", modelNameStr, seatTagName);
                }
            }
        }
    }
    return 1;
}

// Decomp: CoDMPServer.c:430834  G_FreeVehicle
void __cdecl G_FreeVehicle(gentity_s *ent)
{
    if ( ent->scr_vehicle->entNum == 1023
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    7132,
                    0,
                    "%s",
                    "ent->scr_vehicle->entNum != ENTITYNUM_NONE") )
    {
        __debugbreak();
    }
    ent->health = 0;
    //BLOPS_NULLSUB();
    if ( ent->scr_vehicle->nitrousVehicle )
    {
        NitrousVehicle::remove_vehicle(ent->scr_vehicle->nitrousVehicle);
        ent->scr_vehicle->nitrousVehicle = 0;
    }
    Scr_SetString(&ent->scr_vehicle->lookAtText0, 0, SCRIPTINSTANCE_SERVER);
    Scr_SetString(&ent->scr_vehicle->lookAtText1, 0, SCRIPTINSTANCE_SERVER);
    //EntHandle::setEnt(&ent->scr_vehicle->lookAtEnt, 0);
    ent->scr_vehicle->lookAtEnt.setEnt(0);
    ent->handler = 27;
    ent->nextthink = 0;
    ent->takedamage = 0;
    ent->active = 0;
    ent->s.lerp.eFlags = 0;
    ent->s.lerp.pos.trType = 0;
    ent->s.lerp.apos.trType = 0;
    if ( ent->scr_vehicle->moveState == VEH_MOVESTATE_PLANE_ONCURVE )
    {
        cCurveManager::FreeCurve(ent->scr_vehicle->phys.curveID);
        ent->scr_vehicle->phys.curveID = -1;
        ent->scr_vehicle->phys.curveLength = 0.0f;
        ent->scr_vehicle->phys.curveTime = 0.0f;
        ent->scr_vehicle->moveState = VEH_MOVESTATE_PLANE_FREE;
    }
    if ( ent->scr_vehicle->pathPos.customPath )
        VP_FreeCustomPath(ent->scr_vehicle->pathPos.customPath);
    ent->scr_vehicle->entNum = 1023;
    ent->scr_vehicle = 0;
}

// Decomp: CoDMPServer.c:430879  G_HideVehicle
void __cdecl G_HideVehicle(gentity_s *ent)
{
    int occupantNum; // [esp+0h] [ebp-8h]
    int seatIndex; // [esp+4h] [ebp-4h]

    ent->nextthink = 0;
    ent->takedamage = 0;
    ent->active = 0;
    //EntHandle::setEnt(&ent->scr_vehicle->lookAtEnt, 0);
    ent->scr_vehicle->lookAtEnt.setEnt(0);
    ent->r.contents = 0;
    if ( ent->scr_vehicle->nitrousVehicle )
    {
        NitrousVehicle::remove_vehicle(ent->scr_vehicle->nitrousVehicle);
        ent->scr_vehicle->nitrousVehicle = 0;
    }
    for ( seatIndex = 0; seatIndex < 11; ++seatIndex )
    {
        if ( VEH_IsSeatPresent(ent, seatIndex, 0) && G_IsVehicleSeatOccupied(ent, seatIndex) )
        {
            occupantNum = VEH_GetSeatOccupantEntNum(ent, seatIndex);
            if ( (unsigned int)occupantNum < 0x20 )
                ent->r.clientMask[occupantNum >> 5] &= ~(1 << (occupantNum & 0x1F));
        }
    }
}

// Decomp: CoDMPServer.c:430909  G_MakeVehicleUsable
void __cdecl G_MakeVehicleUsable(gentity_s *ent)
{
    int entryPointIndex;
    bool hasUseTag;

    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 7241, 0, "%s", "ent") )
        __debugbreak();
    ent->spawnflags |= 1u;
    ent->r.contents |= 0x200000u;
    if ( (ent->r.contents & 0x200000) != 0 )
    {
        hasUseTag = 0;
        for ( entryPointIndex = 0; entryPointIndex < 5; ++entryPointIndex )
        {
            if ( ent->scr_vehicle->boneIndex.entryPoints[entryPointIndex] >= 0 )
                hasUseTag = 1;
        }
        if ( !hasUseTag )
            ent->scr_vehicle->boneIndex.entryPoints[0] = SV_DObjGetBoneIndex(ent, scr_const.tag_origin);
        if ( ent->scr_vehicle->boneIndex.seats[0] < 0 )
            ent->scr_vehicle->boneIndex.seats[0] = SV_DObjGetBoneIndex(ent, scr_const.tag_origin);
    }
}

// Decomp: CoDMPServer.c:430943  G_UpdateVehicleTags
void __cdecl G_UpdateVehicleTags(gentity_s *ent)
{
    int gunnerBarrelBone;
    int gunnerTurretBone;
    int gunnerFlashBone;
    int gunnerFlash2Bone;
    int wheelBone;
    int seatBone;
    int entryBone;
    int entryBoneFallback;
    float gunnerYawQuantized;
    float gunnerPitchQuantized;
    const DObj *obj; // [esp+18h] [ebp-5Ch]
    DObjAnimMat animMat; // [esp+1Ch] [ebp-58h] BYREF
    float tagAxis[3][3]; // [esp+3Ch] [ebp-38h] BYREF
    float tagAngles[3]; // [esp+60h] [ebp-14h] BYREF
    scr_vehicle_s *veh;
    int flashIndex;
    int gunnerIndex;
    int wheelIndex;
    int seatIndex;
    int entryPointIndex;

    if (!ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 7281, 0, "%s", "ent"))
        __debugbreak();
    if (!ent->scr_vehicle
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
            7282,
            0,
            "%s",
            "ent->scr_vehicle"))
    {
        __debugbreak();
    }
    veh = ent->scr_vehicle;
    veh->boneIndex.player = SV_DObjGetBoneIndex(ent, scr_const.tag_player);
    veh->boneIndex.popout = SV_DObjGetBoneIndex(ent, scr_const.tag_popout);
    veh->boneIndex.body = SV_DObjGetBoneIndex(ent, scr_const.tag_body);
    veh->boneIndex.turret = SV_DObjGetBoneIndex(ent, scr_const.tag_turret);
    veh->boneIndex.barrel = SV_DObjGetBoneIndex(ent, scr_const.tag_barrel);
    veh->boneIndex.turret_base = SV_DObjGetBoneIndex(ent, scr_const.tag_turret_base);
    for (flashIndex = 0; flashIndex < 5; ++flashIndex)
        veh->boneIndex.flash[flashIndex] = SV_DObjGetBoneIndex(ent, *s_flashTags[flashIndex]);
    for (gunnerIndex = 0; gunnerIndex < 4; ++gunnerIndex)
    {
        gunnerBarrelBone = SV_DObjGetBoneIndex(ent, *s_gunnerBarrelTags[gunnerIndex]);
        veh->boneIndex.gunnerTags[gunnerIndex].barrel = gunnerBarrelBone;
        gunnerTurretBone = SV_DObjGetBoneIndex(ent, *s_gunnerTurretTags[gunnerIndex]);
        veh->boneIndex.gunnerTags[gunnerIndex].turret = gunnerTurretBone;
        gunnerFlashBone = SV_DObjGetBoneIndex(ent, *s_gunnerFlashTags[gunnerIndex]);
        veh->boneIndex.gunnerTags[gunnerIndex].flash = gunnerFlashBone;
        gunnerFlash2Bone = SV_DObjGetBoneIndex(ent, *s_gunnerFlashTags[gunnerIndex + 4]);
        veh->boneIndex.gunnerTags[gunnerIndex].flash2 = gunnerFlash2Bone;
        if (veh->boneIndex.gunnerTags[gunnerIndex].flash2 < 0)
            veh->gunnerTurrets[gunnerIndex].flags &= ~2u;
        else
            veh->gunnerTurrets[gunnerIndex].flags |= 2u;
        if (veh->boneIndex.gunnerTags[gunnerIndex].barrel != 254 && veh->boneIndex.gunnerTags[gunnerIndex].barrel > 0 )
        {
            obj = Com_GetServerDObj(ent->s.number);
            DObjGetBasePoseMatrix((DObj*)obj, veh->boneIndex.gunnerTags[gunnerIndex].barrel, &animMat);
            QuatToAxis(animMat.quat, tagAxis);
            AxisToAngles(tagAxis, tagAngles);
            bg_vehicleInfos[veh->infoIdx].gunnerRestAngles[gunnerIndex][1] = tagAngles[1];
            bg_vehicleInfos[veh->infoIdx].gunnerRestAngles[gunnerIndex][0] = tagAngles[0];
            gunnerYawQuantized = floor(tagAngles[1] * 182.04445 + 0.5);
            ent->s.lerp.u.vehicle.gunnerAngles[gunnerIndex].yaw = (int)gunnerYawQuantized;
            gunnerPitchQuantized = floor(tagAngles[0] * 182.04445 + 0.5);
            ent->s.lerp.u.vehicle.gunnerAngles[gunnerIndex].pitch = (int)gunnerPitchQuantized;
        }
    }
    for (wheelIndex = 0; wheelIndex < 6; ++wheelIndex)
    {
        wheelBone = SV_DObjGetBoneIndex(ent, *s_wheelTags[wheelIndex]);
        veh->boneIndex.wheel[wheelIndex] = wheelBone;
    }
    for (seatIndex = 0; seatIndex < 11; ++seatIndex)
    {
        if (s_seatTags[seatIndex])
        {
            seatBone = SV_DObjGetBoneIndex(ent, *s_seatTags[seatIndex]);
            veh->boneIndex.seats[seatIndex] = seatBone;
        }
    }
    for (entryPointIndex = 0; entryPointIndex < 5; ++entryPointIndex)
    {
        entryBone = SV_DObjGetBoneIndex(ent, *s_entryPointTags[entryPointIndex]);
        veh->boneIndex.entryPoints[entryPointIndex] = entryBone;
        if ( veh->boneIndex.entryPoints[entryPointIndex] < 0 && s_entryPointOldTags[entryPointIndex] )
        {
            entryBoneFallback = SV_DObjGetBoneIndex(ent, *s_entryPointOldTags[entryPointIndex]);
            veh->boneIndex.entryPoints[entryPointIndex] = entryBoneFallback;
        }
    }
    if ((ent->r.contents & 0x200000) != 0)
        G_MakeVehicleUsable(ent);
}

// Decomp: CoDMPServer.c:431052  G_IsVehicleRemoteControl
bool __cdecl G_IsVehicleRemoteControl(int index)
{
    if ( index < 0
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 7391, 0, "%s", "index >= 0") )
    {
        __debugbreak();
    }
    if ( index >= bg_numVehicleInfos
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    7392,
                    0,
                    "%s",
                    "index < bg_numVehicleInfos") )
    {
        __debugbreak();
    }
    return BG_GetVehicleInfo(index)->remoteControl != 0;
}

// Decomp: CoDMPServer.c:431080  GScr_PrecacheVehicle
void __cdecl GScr_PrecacheVehicle()
{
    const char *errorMsg;
    const vehicle_info_t *info;
    int infoIdx;
    char *vehInfo;

    vehInfo = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( !level.initializing )
        Scr_Error("precacheVehicle must be called before any wait statements in the level script\n", 0);
    infoIdx = VEH_GetVehicleInfoFromName(vehInfo);
    if ( infoIdx < 0 )
    {
        errorMsg = va("Cannot find vehicle info for [%s]\n", vehInfo);
        Scr_Error(errorMsg, 0);
    }
    info = BG_GetVehicleInfo(infoIdx);
    G_GetWeaponIndexForName((char*)info->turretWeapon);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetNumVehicles ~82630C38)
void __cdecl GScr_GetNumVehicles()
{
    int count = 0;

    for ( int i = 0; i < MAX_VEHICLES; ++i )
    {
        if ( s_vehicles[i].entNum != 1023 )
            ++count;
    }
    Scr_AddInt(count, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetHeliHeightPatchEnabled ~82630CC0)
void __cdecl GScr_SetHeliHeightPatchEnabled()
{
    unsigned __int16 targetname = (unsigned __int16)Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    int enabled = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);

    for ( int i = 0; i < num_heli_height_lock_patches; ++i )
    {
        if ( heli_height_lock_patches[i].targetname == targetname )
            heli_height_lock_patches[i].enabled = enabled;
    }
}

// Decomp: CoDMPServer.c:431101  GScr_GetVehicleTreadFXArray
void __cdecl GScr_GetVehicleTreadFXArray()
{
    const char *errorMsg;
    int treadFxIndex;
    const vehicle_info_t *info;
    int infoIdx;
    char *vehInfo;

    vehInfo = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    infoIdx = VEH_GetVehicleInfoFromName(vehInfo);
    if ( infoIdx < 0 )
    {
        errorMsg = va("Cannot find vehicle info for [%s]\n", vehInfo);
        Scr_Error(errorMsg, 0);
    }
    info = BG_GetVehicleInfo(infoIdx);
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for ( treadFxIndex = 0; treadFxIndex < 31; ++treadFxIndex )
    {
        Scr_AddString(info->treadFx[treadFxIndex], SCRIPTINSTANCE_SERVER);
        Scr_AddArray(SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:431126  G_IsVehicleOccupied
int __cdecl G_IsVehicleOccupied(gentity_s *ent)
{
    int seatIndex; // [esp+0h] [ebp-4h]

    if ( !ent->scr_vehicle )
        return 0;
    for ( seatIndex = 0; seatIndex < 11; ++seatIndex )
    {
        if ( VEH_IsSeatPresent(ent, seatIndex, 0) && G_IsVehicleSeatOccupied(ent, seatIndex) )
            return 1;
    }
    return 0;
}

// Decomp: CoDMPServer.c:431141  G_GetVehicleOccupantsTeam
team_t __cdecl G_GetVehicleOccupantsTeam(gentity_s *vehEnt)
{
    const gentity_s *pOccupant; // [esp+0h] [ebp-Ch]
    int seatIndex; // [esp+4h] [ebp-8h]
    scr_vehicle_s *veh; // [esp+8h] [ebp-4h]

    veh = vehEnt->scr_vehicle;
    if ( veh )
    {
        for ( seatIndex = 0; seatIndex < 11; ++seatIndex )
        {
            pOccupant = VEH_GetSeatOccupantEntity(veh, seatIndex);
            if ( pOccupant && pOccupant->client )
                return pOccupant->client->sess.cs.team;
        }
    }
    return TEAM_BAD;
}

// Decomp: CoDMPServer.c:431161  G_IsVehicleSeatOccupied
bool __cdecl G_IsVehicleSeatOccupied(gentity_s *ent, int seatIndex)
{
    scr_vehicle_s *veh; // [esp+4h] [ebp-8h]
    const VehicleSeat *seat; // [esp+8h] [ebp-4h]

    veh = ent->scr_vehicle;
    if ( !veh )
        return 0;
    if ( !VEH_IsSeatPresent(ent, seatIndex, 0) )
        Com_Error(ERR_DROP, "G_IsVehicleSeatOccupied() called on invalid seat position");
    seat = VEH_GetSeat(veh, seatIndex);
    return seat && seat->_occupantEntNum != 1023;
}

// Decomp: CoDMPServer.c:431176  G_GetVehicleSeatPlayerOccupies
int __cdecl G_GetVehicleSeatPlayerOccupies(gentity_s *ent, gentity_s *player)
{
    scr_vehicle_s *veh; // [esp+0h] [ebp-8h]
    int seatIndex; // [esp+4h] [ebp-4h]

    veh = ent->scr_vehicle;
    if ( !veh )
        return -1;
    for ( seatIndex = 0; seatIndex < 11; ++seatIndex )
    {
        if ( VEH_IsSeatPresent(ent, seatIndex, 0) && VEH_GetSeatOccupantEntNum(veh, seatIndex) == player->s.number )
            return seatIndex;
    }
    return -1;
}

// Decomp: CoDMPServer.c:431193  G_GetVehicleSeatToEnter
int __cdecl G_GetVehicleSeatToEnter(gentity_s *ent, gentity_s *player, bool scriptUse, int *entryPoint)
{
    bool foundEntryPoint; // [esp+Fh] [ebp-25h]
    const vehicle_info_t *info; // [esp+10h] [ebp-24h]
    scr_vehicle_s *veh; // [esp+14h] [ebp-20h]
    float bestDist; // [esp+18h] [ebp-1Ch]
    float dist;
    int entryPointIndex;
    float entryPointPosition[3];
    int seatIndex;

    if ( player->r.currentOrigin[2] > (float)(ent->r.currentOrigin[2] + ent->r.maxs[2]) )
        return -1;
    veh = ent->scr_vehicle;
    bestDist = 40000.0f;
    seatIndex = -1;
    if ( !veh )
        return -1;
    info = BG_GetVehicleInfo(veh->infoIdx);
    if ( !vehLocationalVehicleSeatEntry->current.enabled || scriptUse )
    {
        for ( seatIndex = 0;
                    seatIndex < 11 && (!VEH_IsSeatPresent(ent, seatIndex, 0) || G_IsVehicleSeatOccupied(ent, seatIndex));
                    ++seatIndex )
        {
            ;
        }
        if ( seatIndex >= 11 )
            return -1;
    }
    if ( scriptUse )
        return seatIndex;
    foundEntryPoint = 0;
    for ( entryPointIndex = 0; entryPointIndex < 5; ++entryPointIndex )
    {
        if ( veh->boneIndex.entryPoints[entryPointIndex] >= 0 )
        {
            G_DObjGetWorldBoneIndexPos(ent, veh->boneIndex.entryPoints[entryPointIndex], entryPointPosition);
            dist = (float)((float)(player->r.currentOrigin[0] - entryPointPosition[0])
                                     * (float)(player->r.currentOrigin[0] - entryPointPosition[0]))
                     + (float)((float)(player->r.currentOrigin[1] - entryPointPosition[1])
                                     * (float)(player->r.currentOrigin[1] - entryPointPosition[1]));
            if ( bestDist > dist && (float)(info->entryPointRadius[entryPointIndex] * info->entryPointRadius[entryPointIndex]) > dist )
            {
                if ( vehLocationalVehicleSeatEntry->current.enabled )
                {
                    if ( !VEH_IsSeatPresent(ent, entryPointIndex, 0) || G_IsVehicleSeatOccupied(ent, entryPointIndex) )
                        continue;
                    seatIndex = entryPointIndex;
                    bestDist = dist;
                }
                if ( entryPoint )
                    *entryPoint = entryPointIndex;
                foundEntryPoint = 1;
            }
        }
    }
    if ( foundEntryPoint )
        return seatIndex;
    else
        return -1;
}

// Decomp: CoDMPServer.c:431260  G_GetEntryPointForSeat
int __cdecl G_GetEntryPointForSeat(gentity_s *ent, int seat)
{
    scr_vehicle_s *veh;
    int entryPoint;
    int entryPointIndex;

    entryPoint = 0;
    veh = ent->scr_vehicle;
    if ( seat <= 5 )
        entryPoint = seat;
    if ( veh->boneIndex.entryPoints[entryPoint] >= 0 )
        return entryPoint;
    for ( entryPointIndex = 0; entryPointIndex < 5; ++entryPointIndex )
    {
        if ( veh->boneIndex.entryPoints[entryPointIndex] >= 0 )
            return entryPointIndex;
    }
    return entryPoint;
}

// Decomp: CoDMPServer.c:431281  G_GetPlayerVehicleMantlePoint
int __cdecl G_GetPlayerVehicleMantlePoint(gentity_s *ent, gentity_s *player)
{
    float vecToPlayer[3];
    const vehicle_info_t *info; // [esp+34h] [ebp-34h]
    scr_vehicle_s *veh; // [esp+38h] [ebp-30h]
    float axis[3][3]; // [esp+3Ch] [ebp-2Ch] BYREF
    float forwardDot; // [esp+60h] [ebp-8h]
    float leftDot; // [esp+64h] [ebp-4h]

    veh = ent->scr_vehicle;
    info = BG_GetVehicleInfo(veh->infoIdx);
    AnglesToAxis(ent->r.currentAngles, axis);
    vecToPlayer[0] = player->r.currentOrigin[0] - ent->r.currentOrigin[0];
    vecToPlayer[1] = player->r.currentOrigin[1] - ent->r.currentOrigin[1];
    vecToPlayer[2] = player->r.currentOrigin[2] - ent->r.currentOrigin[2];
    Vec3Normalize(vecToPlayer);
    forwardDot = (float)((float)(vecToPlayer[0] * axis[0][0]) + (float)(vecToPlayer[1] * axis[0][1]))
                         + (float)(vecToPlayer[2] * axis[0][2]);
    leftDot = (float)((float)(vecToPlayer[0] * axis[1][0]) + (float)(vecToPlayer[1] * axis[1][1]))
                    + (float)(vecToPlayer[2] * axis[1][2]);
    if ( forwardDot > 1.0f )
        forwardDot = 1.0f;
    else if ( forwardDot < -1.0f )
        forwardDot = -1.0f;
    if ( leftDot > 1.0f )
        leftDot = 1.0f;
    else if ( leftDot < -1.0f )
        leftDot = -1.0f;
    if ( forwardDot > info->mantleAngles[0] )
        return 0;
    if ( (-(forwardDot)) > info->mantleAngles[1] )
        return 1;
    if ( (-(leftDot)) > info->mantleAngles[3] )
        return 3;
    if ( leftDot <= info->mantleAngles[2] )
        return 4;
    return 2;
}

// Decomp: CoDMPServer.c:431342  G_IsVehicleUsable
int __cdecl G_IsVehicleUsable(gentity_s *ent, gentity_s *player)
{
    const vehicle_info_t *info; // [esp+0h] [ebp-24h]
    const WeaponDef *weapDef; // [esp+8h] [ebp-1Ch]
    gentity_s *occupant; // [esp+Ch] [ebp-18h]
    int seat_index; // [esp+10h] [ebp-14h]
    gclient_s *client; // [esp+18h] [ebp-Ch]
    int hintString; // [esp+1Ch] [ebp-8h] BYREF
    int seatIndex; // [esp+20h] [ebp-4h]

    client = player->client;
    hintString = -1;
    if ( !client )
        return hintString;
    if ( (client->ps.eFlags & 0x4000) != 0 )
        return hintString;
    //if ( EntHandle::isDefined(&player->r.ownerNum) )
    if ( player->r.ownerNum.isDefined() )
        return hintString;
    if ( ent->health <= 0 )
        return hintString;
    if ( (ent->r.contents & 0x200000) == 0 )
        return hintString;
    if ( (ent->scr_vehicle->flags & 8) == 0 || player->team && ent->team == player->team )
    {
        if ( VEH_ShouldEjectOccupants(ent) )
            return hintString;
        for ( seat_index = 0; seat_index < 11; ++seat_index )
        {
            occupant = VEH_GetSeatOccupantEntity(ent->scr_vehicle, seat_index);
            if ( occupant
                && occupant->client
                && (occupant->client->sess.cs.team == TEAM_FREE || occupant->client->sess.cs.team != client->sess.cs.team) )
            {
                return hintString;
            }
        }
        seatIndex = G_GetVehicleSeatToEnter(ent, player, 0, 0);
        if ( seatIndex == -1 )
            return hintString;
        if ( client->disallowVehicleUsage )
        {
            G_GetHintStringIndex(&hintString, (char*)"GAME_CANNOTUSEVEHICLE");
            return hintString;
        }
        if ( seatIndex )
        {
            if ( seatIndex >= 1 && seatIndex <= 4 )
            {
                info = BG_GetVehicleInfo(ent->scr_vehicle->infoIdx);
                weapDef = BG_GetWeaponDef(*(unsigned __int16 *)&info->gunnerWeapon[3][2 * seatIndex + 62]);
                if ( *weapDef->szUseHintString )
                    return weapDef->iUseHintStringIndex;
                G_GetHintStringIndex(&hintString, (char *)"PLATFORM_USEVEHICLEGUNNER");
                return hintString;
            }
            if ( seatIndex >= 5 && seatIndex <= 10 )
            {
                G_GetHintStringIndex(&hintString, (char *)"PLATFORM_RIDEVEHICLE");
                return hintString;
            }
        }
        else if ( *BG_GetWeaponDef(ent->s.weapon)->szUseHintString )
        {
            return BG_GetWeaponDef(ent->s.weapon)->iUseHintStringIndex;
        }
        G_GetHintStringIndex(&hintString, (char *)"PLATFORM_USEVEHICLE");
        return hintString;
    }
    if ( G_GetPlayerVehicleMantlePoint(ent, player) != 4 )
        G_GetHintStringIndex(&hintString, (char *)"PLATFORM_MANTLEVEHICLE");
    return hintString;
}

// Decomp: CoDMPServer.c:431416  VEH_ShouldEjectOccupants
bool __cdecl VEH_ShouldEjectOccupants(gentity_s *ent)
{
    float *p_x; // eax
    bool result; // al
    phys_vec3 *aVelocity; // [esp+1Ch] [ebp-Ch]
    NitrousVehicle *nitrousVeh; // [esp+24h] [ebp-4h]

    result = 0;
    if ( ent->scr_vehicle->nitrousVehicle )
    {
        nitrousVeh = ent->scr_vehicle->nitrousVehicle;
        if ( nitrousVeh->m_phys_user_data )
        {
            if ( nitrousVeh->m_phys_user_data->body )
            {
                if ( nitrousVeh->m_phys_user_data->body->m_mat.z.z < 0.2 )
                {
                    p_x = &nitrousVeh->m_phys_user_data->body->m_t_vel.x;
                    if ( (float)((float)((float)(*p_x * *p_x)
                                                         + (float)(nitrousVeh->m_phys_user_data->body->m_t_vel.y
                                                                         * nitrousVeh->m_phys_user_data->body->m_t_vel.y))
                                         + (float)(p_x[2] * p_x[2])) < 100.0 )
                    {
                        aVelocity = &nitrousVeh->m_phys_user_data->body->m_a_vel;
                        if ( (float)((float)((float)(aVelocity->x * aVelocity->x) + (float)(aVelocity->y * aVelocity->y))
                                             + (float)(aVelocity->z * aVelocity->z)) < 1.0 )
                            return 1;
                    }
                }
            }
        }
    }
    return result;
}

// Decomp: CoDMPServer.c:431453  G_IsVehicleImmune
bool __cdecl G_IsVehicleImmune(gentity_s *ent, int mod, char damageFlags, unsigned int weapon)
{
    bool result; // eax
    gclient_s *client; // [esp+4h] [ebp-10h]
    const vehicle_info_t *info; // [esp+8h] [ebp-Ch]

    info = BG_GetVehicleInfo(ent->scr_vehicle->infoIdx);
    //if ( !EntHandle::isDefined(&ent->r.ownerNum) || !EntHandle::ent(&ent->r.ownerNum)->client )
    if ( !ent->r.ownerNum.isDefined() || !ent->r.ownerNum.ent()->client)
        goto LABEL_7;
    //client = EntHandle::ent(&ent->r.ownerNum)->client;
    client = ent->r.ownerNum.ent()->client;
    if ( (client->ps.otherFlags & 1) != 0 )
        return 1;
    if ( (client->flags & 3) != 0 )
        return 1;
LABEL_7:
    switch ( mod )
    {
        case 1:
        case 2:
            if ( info->bulletDamage )
                result = 0;
            else
                result = (damageFlags & 2) == 0 || !info->armorPiercingDamage;
            break;
        case 3:
        case 4:
            if ( BG_GetWeaponDef(weapon)->projExplosion == WEAPPROJEXP_HEAVY )
                result = info->heavyExplosiveDamage == 0;
            else
                result = ((damageFlags & 2) == 0 || !info->armorPiercingDamage) && info->grenadeDamage == 0;
            break;
        case 5:
        case 16:
            result = ((damageFlags & 2) == 0 || !info->armorPiercingDamage) && info->projectileDamage == 0;
            break;
        case 6:
            result = info->projectileSplashDamage == 0;
            break;
        case 15:
            result = 0;
            break;
        case 17:
            result = info->remoteControl == 0;
            break;
        default:
            result = 1;
            break;
    }
    return result;
}

// Decomp: CoDMPServer.c:431520  G_IsPlayerDrivingVehicle
bool __cdecl G_IsPlayerDrivingVehicle(const gentity_s *player)
{
    gclient_s *client; // [esp+0h] [ebp-10h]
    const scr_vehicle_s *veh; // [esp+8h] [ebp-8h]

    if ( !player
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 8004, 0, "%s", "player") )
    {
        __debugbreak();
    }
    client = player->client;
    if ( !client )
        return 0;
    if ( (client->ps.eFlags & 0x4000) == 0 )
        return 0;
    //if ( !EntHandle::isDefined(&player->r.ownerNum) )
    if ( !player->r.ownerNum.isDefined() )
        return 0;
    veh = player->r.ownerNum.ent()->scr_vehicle;
    if ( !veh )
        return 0;
    if ( (veh->flags & 0x82) != 0 )
        return 0;
    return BG_GetVehicleInfo(veh->infoIdx)->type != 5;
}

// Decomp: CoDMPServer.c:431548  VEH_Teleport
void __cdecl VEH_Teleport(gentity_s *pSelf, float *origin, float *angles, float *vel)
{
    phys_vec3 *angularVel;
    phys_vec3 *translationalVel;
    rigid_body *body;
    phys_vec3 physNewOrigin;
    phys_vec3 phys_axis[3];
    float newAxis[3][3];
    phys_mat44 *mat;
    PhysObjUserData *physUserData;
    NitrousVehicle *nitrousVeh;
    vehicle_physic_t *phys;

    iassert(pSelf->scr_vehicle);
    
    phys = &pSelf->scr_vehicle->phys;
    Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
    Sys_EnterCriticalSection(CRITSECT_PHYSICS);
    if (pSelf->scr_vehicle->nitrousVehicle)
    {
        if (pSelf->scr_vehicle->nitrousVehicle->m_phys_user_data)
        {
            nitrousVeh = pSelf->scr_vehicle->nitrousVehicle;
            physUserData = nitrousVeh->m_phys_user_data;
            mat = &physUserData->body->m_mat;
            AnglesToAxis(angles, newAxis);
            Phys_Vec3ToNitrousVec(newAxis[0], phys_axis);
            Phys_Vec3ToNitrousVec(newAxis[1], &phys_axis[1]);
            Phys_Vec3ToNitrousVec(newAxis[2], &phys_axis[2]);
            mat->x = phys_axis[0];
            mat->y = phys_axis[1];
            mat->z = phys_axis[2];
            physNewOrigin.x = origin[0];
            physNewOrigin.y = origin[1];
            physNewOrigin.z = origin[2];
            mat->w = physNewOrigin;
            if (   (fabs(mat->w.x)) > 100000.0
                || (fabs(mat->w.y)) > 100000.0
                || (fabs(mat->w.z)) > 100000.0)
            {
                phys_exec_debug_callback(0);
            }
            body = nitrousVeh->m_phys_user_data->body;
            body->m_last_position.x = physNewOrigin.x;
            body->m_last_position.y = physNewOrigin.y;
            body->m_last_position.z = physNewOrigin.z;
            if (   (fabs(nitrousVeh->m_phys_user_data->body->m_last_position.x)) > 100000.0
                || (fabs(nitrousVeh->m_phys_user_data->body->m_last_position.y)) > 100000.0
                || (fabs(nitrousVeh->m_phys_user_data->body->m_last_position.z)) > 100000.0)
            {
                phys_exec_debug_callback(nitrousVeh->m_phys_user_data->body);
            }
            translationalVel = &nitrousVeh->m_phys_user_data->body->m_t_vel;
            angularVel = &nitrousVeh->m_phys_user_data->body->m_a_vel;
            if (vel)
            {
                translationalVel->x = vel[0];
                translationalVel->y = vel[1];
                translationalVel->z = vel[2];
            }
            else
            {
                translationalVel->x = 0.0f;
                translationalVel->y = 0.0f;
                translationalVel->z = 0.0f;
            }
            angularVel->x = 0.0f;
            angularVel->y = 0.0f;
            angularVel->z = 0.0f;
        }
    }
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
    phys->origin[0] = origin[0];
    phys->origin[1] = origin[1];
    phys->origin[2] = origin[2];
    phys->angles[0] = angles[0];
    phys->angles[1] = angles[1];
    phys->angles[2] = angles[2];
    if (vel)
    {
        phys->vel[0] = vel[0];
        phys->vel[1] = vel[1];
        phys->vel[2] = vel[2];
    }
    else
    {
        phys->vel[0] = 0.0f;
        phys->vel[1] = 0.0f;
        phys->vel[2] = 0.0f;
    }
    VEH_SetPosition(pSelf, phys->origin, phys->vel, phys->angles);
}

// Decomp: CoDMPServer.c:431689  VEH_UpdateNOClip
void __cdecl VEH_UpdateNOClip(gentity_s *pSelf)
{
    float newOrigin[3]; // [esp+4h] [ebp-10h] BYREF
    gentity_s *player; // [esp+10h] [ebp-4h]

    player = VEH_GetSeatOccupantEntity(pSelf, 0);
    if ( player && player->client && player->client->ps.pm_type == 2 )
    {
        if ( pSelf->scr_vehicle->nitrousVehicle )
        {
            newOrigin[0] = player->r.currentOrigin[0];
            newOrigin[1] = player->r.currentOrigin[1];
            newOrigin[2] = player->r.currentOrigin[2] + 70.0;
            VEH_Teleport(pSelf, newOrigin, player->client->ps.viewangles, 0);
        }
    }
}

// Decomp: CoDMPServer.c:431713  VEH_UpdateDebug
void __cdecl VEH_UpdateDebug(gentity_s *pSelf)
{
    char *debugText;
    float mtx[4][3];
    DObj *ServerDObj; // [esp+68h] [ebp-5Ch]
    float end[3]; // [esp+6Ch] [ebp-58h] BYREF
    char *text; // [esp+78h] [ebp-4Ch]
    int flash;
    int gunnerIndex;
    DObj *obj; // [esp+84h] [ebp-40h]
    const char *tag_name; // [esp+88h] [ebp-3Ch]
    int boneIndex; // [esp+8Ch] [ebp-38h]
    vehicle_physic_t *phys; // [esp+90h] [ebp-34h]
    const vehicle_info_t *info; // [esp+94h] [ebp-30h]
    float org[3]; // [esp+98h] [ebp-2Ch] BYREF
    float entryPointPosition[3]; // [esp+A4h] [ebp-20h] BYREF
    float color[4]; // [esp+B0h] [ebp-14h] BYREF
    int seatIndex; // [esp+C0h] [ebp-4h]

    if ( g_vehicleDebug->current.enabled )
    {
        phys = &pSelf->scr_vehicle->phys;
        info = BG_GetVehicleInfo(pSelf->scr_vehicle->infoIdx);
        color[0] = 1.0f;
        color[1] = 1.0f;
        color[2] = 0.0f;
        color[3] = 1.0f;
        CG_DebugBox(phys->origin, phys->mins, phys->maxs, phys->angles[1], color, 1, 1);
        org[0] = phys->origin[0];
        org[1] = phys->origin[1];
        org[2] = phys->origin[2];
        org[2] = org[2] + 30.0;
        debugText = va(
                     "Ent %d: origin %g %g %g, angles %g %g %g",
                     pSelf->s.number,
                     pSelf->r.currentOrigin[0],
                     pSelf->r.currentOrigin[1],
                     pSelf->r.currentOrigin[2],
                     pSelf->r.currentAngles[0],
                     pSelf->r.currentAngles[1],
                     pSelf->r.currentAngles[2]);
        CL_AddDebugString(org, colorWhite, 0.69999999, debugText, 0);
        color[0] = 0.0f;
        color[1] = 1.0f;
        color[2] = 0.0f;
        color[3] = 1.0f;
        for ( seatIndex = 0; seatIndex < 5; ++seatIndex )
        {
            boneIndex = pSelf->scr_vehicle->boneIndex.entryPoints[seatIndex];
            if ( boneIndex >= 0 )
            {
                G_DObjGetWorldBoneIndexPos(pSelf, boneIndex, entryPointPosition);
                G_DebugCircle(entryPointPosition, info->entryPointRadius[seatIndex], color, 1, 1, 0);
                obj = Com_GetServerDObj(pSelf->s.number);
                if ( !obj
                    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 8434, 0, "%s", "obj") )
                {
                    __debugbreak();
                }
                tag_name = DObjGetBoneName(obj, boneIndex);
                CL_AddDebugString(entryPointPosition, colorWhite, 0.69999999, (char *)tag_name, 0);
            }
        }
        if ( pSelf->scr_vehicle->pathPos.customPath )
            VP_DrawCustomPath(pSelf->scr_vehicle->pathPos.customPath);
        pSelf->spawnflags |= 1u;
        pSelf->r.contents |= 0x200000u;
        bg_vehicleInfos[pSelf->scr_vehicle->infoIdx].isDrivable = 1;
        if ( pSelf->scr_vehicle->boneIndex.entryPoints[0] < 0 )
            pSelf->scr_vehicle->boneIndex.entryPoints[0] = 0;
        if ( pSelf->scr_vehicle->boneIndex.seats[0] < 0 )
            pSelf->scr_vehicle->boneIndex.seats[0] = SV_DObjGetBoneIndex(pSelf, scr_const.tag_origin);
        for ( gunnerIndex = 0; gunnerIndex < 4; ++gunnerIndex )
        {
            flash = pSelf->scr_vehicle->boneIndex.gunnerTags[gunnerIndex].flash;
            if ( flash >= 0 )
            {
                G_DObjGetWorldBoneIndexMatrix(pSelf, flash, mtx);
                end[0] = (float)(10.0 * mtx[0][0]) + mtx[3][0];
                end[1] = (float)(10.0 * mtx[0][1]) + mtx[3][1];
                end[2] = (float)(10.0 * mtx[0][2]) + mtx[3][2];
                G_DebugLine(mtx[3], end, color, 1);
                G_DebugStar(mtx[3], color, 0);
                ServerDObj = Com_GetServerDObj(pSelf->s.number);
                if ( !ServerDObj
                    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 8525, 0, "%s", "obj") )
                {
                    __debugbreak();
                }
                text = DObjGetBoneName(ServerDObj, flash);
                CL_AddDebugString(entryPointPosition, colorWhite, 0.69999999, text, 0);
            }
        }
    }
    VEH_UpdateNOClip(pSelf);
}

// Decomp: CoDMPServer.c:431837  Scr_Vehicle_Controller
void __cdecl Scr_Vehicle_Controller(const gentity_s *pSelf, int *partBits)
{
    float gunnerYawRad;
    float turretYawRad;
    float barrelAngles[3];
    DObj *obj; // [esp+1Ch] [ebp-24h]
    scr_vehicle_s *veh; // [esp+20h] [ebp-20h]
    float bodyAngles[3]; // [esp+24h] [ebp-1Ch] BYREF
    float turretAngles[3]; // [esp+30h] [ebp-10h] BYREF
    int gunnerIndex;

    if ( !pSelf && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 8561, 0, "%s", "pSelf") )
        __debugbreak();
    if ( !pSelf->scr_vehicle
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    8562,
                    0,
                    "%s",
                    "pSelf->scr_vehicle") )
    {
        __debugbreak();
    }
    veh = pSelf->scr_vehicle;
    obj = Com_GetServerDObj(pSelf->s.number);
    if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 8566, 0, "%s", "obj") )
        __debugbreak();
    if ( veh->nitrousVehicle )
    {
        memset(bodyAngles, 0, sizeof(bodyAngles));
    }
    else
    {
        bodyAngles[0] = (float)pSelf->s.lerp.u.vehicle.throttle * 0.0054931641;
        bodyAngles[1] = 0.0f;
        bodyAngles[2] = pSelf->s.lerp.u.turret.gunAngles[1];
    }
    if ( veh->boneIndex.body >= 0 )
        DObjSetLocalBoneIndex(obj, partBits, veh->boneIndex.body, vec3_origin, bodyAngles);
    turretYawRad = (float)pSelf->s.lerp.u.vehicle.gunYaw * 0.0054931641;
    turretAngles[0] = 0.0f;
    turretAngles[1] = turretYawRad;
    turretAngles[2] = 0.0f;
    barrelAngles[0] = (float)pSelf->s.lerp.u.vehicle.gunPitch * 0.0054931641;
    barrelAngles[1] = 0.0f;
    barrelAngles[2] = 0.0f;
    if ( veh->boneIndex.turret >= 0 )
        DObjSetLocalBoneIndex(obj, partBits, veh->boneIndex.turret, vec3_origin, turretAngles);
    if ( veh->boneIndex.barrel >= 0 )
        DObjSetLocalBoneIndex(obj, partBits, veh->boneIndex.barrel, vec3_origin, barrelAngles);
    for ( gunnerIndex = 0; gunnerIndex < 4; ++gunnerIndex )
    {
        gunnerYawRad = (float)pSelf->s.lerp.u.vehicle.gunnerAngles[gunnerIndex].yaw * 0.0054931641;
        turretAngles[0] = 0.0f;
        turretAngles[1] = gunnerYawRad;
        turretAngles[2] = 0.0f;
        barrelAngles[0] = (float)pSelf->s.lerp.u.vehicle.gunnerAngles[gunnerIndex].pitch * 0.0054931641;
        barrelAngles[1] = 0.0f;
        barrelAngles[2] = 0.0f;
        if ( veh->boneIndex.gunnerTags[gunnerIndex].turret >= 0 )
            DObjSetLocalBoneIndex(obj, partBits, veh->boneIndex.gunnerTags[gunnerIndex].turret, vec3_origin, turretAngles);
        if ( veh->boneIndex.gunnerTags[gunnerIndex].barrel >= 0 )
            DObjSetLocalBoneIndex(obj, partBits, veh->boneIndex.gunnerTags[gunnerIndex].barrel, vec3_origin, barrelAngles);
    }
}

// Decomp: CoDMPServer.c:431909  Scr_Vehicle_Init
void __cdecl Scr_Vehicle_Init(gentity_s *pSelf)
{
    float barrelOffset;
    float diff;
    float pos[3]; // [esp+38h] [ebp-40h] BYREF
    vehicle_physic_t *phys; // [esp+44h] [ebp-34h]
    const vehicle_info_t *info; // [esp+48h] [ebp-30h]
    DObjAnimMat *mtx; // [esp+4Ch] [ebp-2Ch]
    scr_vehicle_s *veh; // [esp+50h] [ebp-28h]
    float origin[3]; // [esp+54h] [ebp-24h] BYREF
    float maxHeight; // [esp+60h] [ebp-18h]
    float radius; // [esp+64h] [ebp-14h]
    int wheelIndex; // [esp+68h] [ebp-10h]
    float wheelRadius; // [esp+6Ch] [ebp-Ch]
    float minHeight; // [esp+70h] [ebp-8h]
    int wheelCount; // [esp+74h] [ebp-4h]

    veh = pSelf->scr_vehicle;
    phys = &veh->phys;
    info = BG_GetVehicleInfo(veh->infoIdx);
    if ( info->type && info->type != 2 )
    {
        if ( info->type == 6 )
        {
            radius = 0.0f;
            minHeight = 0.0f;
            maxHeight = 0.0f;
            for ( wheelIndex = 0; wheelIndex < 6; ++wheelIndex )
            {
                if ( veh->boneIndex.wheel[wheelIndex] >= 0 )
                {
                    mtx = G_DObjGetLocalBoneIndexMatrix(pSelf, veh->boneIndex.wheel[wheelIndex]);
                    wheelRadius = (float)(mtx->trans[0] * mtx->trans[0]) + (float)(mtx->trans[1] * mtx->trans[1]);
                    if ( minHeight > mtx->trans[2] )
                        minHeight = mtx->trans[2];
                    if ( mtx->trans[2] > maxHeight )
                        maxHeight = mtx->trans[2];
                    if ( wheelRadius > radius )
                        radius = wheelRadius;
                }
            }
            if ( radius > 0.0 )
            {
                radius = (float)(sqrtf(radius) + 1.0) * info->heliCollisionScalar;
                diff = maxHeight - minHeight;
                if ( radius > (float)(maxHeight - minHeight) )
                {
                    maxHeight = (float)(0.5 * (float)(radius - diff)) + maxHeight;
                    minHeight = minHeight - (float)(0.5 * (float)(radius - diff));
                }
                phys->mins[0] = -radius;
                phys->mins[1] = -radius;
                phys->mins[2] = minHeight;
                phys->maxs[0] = radius;
                phys->maxs[1] = radius;
                phys->maxs[2] = maxHeight;
            }
        }
    }
    else
    {
        wheelCount = info->type != 0 ? 6 : 4;
        radius = 0.0f;
        for ( wheelIndex = 0; wheelIndex < wheelCount; ++wheelIndex )
        {
            VEH_GetWheelOrigin(pSelf, wheelIndex, origin, 0);
            wheelRadius = (float)((float)(origin[0] * origin[0]) + (float)(origin[1] * origin[1]))
                                    + (float)(origin[2] * origin[2]);
            if ( wheelRadius > radius )
                radius = wheelRadius;
        }
        if ( radius <= 0.0
            && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 8641, 0, "%s", "radius > 0.0f") )
        {
            __debugbreak();
        }
        radius = sqrtf(radius) + 1.0;
        phys->mins[0] = -radius;
        phys->mins[1] = -radius;
        phys->mins[2] = 0.0f;
        phys->maxs[0] = radius;
        phys->maxs[1] = radius;
        phys->maxs[2] = radius * 2.0f;
    }
    for ( wheelIndex = 0; wheelIndex < 6; ++wheelIndex )
    {
        if ( veh->boneIndex.wheel[wheelIndex] >= 0 )
        {
            G_DObjGetWorldBoneIndexPos(pSelf, veh->boneIndex.wheel[wheelIndex], pos);
            phys->wheelZPos[wheelIndex] = pos[2];
        }
    }
    if ( veh->boneIndex.flash[0] >= 0 && veh->boneIndex.barrel >= 0 )
    {
        G_DObjGetWorldBoneIndexPos(pSelf, veh->boneIndex.flash[0], origin);
        G_DObjGetWorldBoneIndexPos(pSelf, veh->boneIndex.barrel, pos);
        barrelOffset = Vec3Distance(pos, origin);
        veh->turret.barrelOffset = barrelOffset;
    }

    if ( !info->type || info->type == 2 )
        VEH_GroundPlant(pSelf, phys, 0);

    VEH_SetPosition(pSelf, phys->origin, phys->vel, phys->angles);

    phys->prevOrigin[0] = phys->origin[0];
    phys->prevOrigin[1] = phys->origin[1];
    phys->prevOrigin[2] = phys->origin[2];
    phys->prevAngles[0] = phys->angles[0];
    phys->prevAngles[1] = phys->angles[1];
    phys->prevAngles[2] = phys->angles[2];

    VEH_TouchEntities(pSelf);
    G_DoTouchTriggers(pSelf);
    if ( pSelf->handler != 25
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    8723,
                    0,
                    "%s",
                    "pSelf->handler == ENT_HANDLER_VEHICLE_INIT") )
    {
        __debugbreak();
    }
    pSelf->handler = 26;
    pSelf->nextthink = level.time + 50;
}

// Decomp: CoDMPServer.c:432057  VEH_GetWheelOrigin
void __cdecl VEH_GetWheelOrigin(gentity_s *ent, int idx, float *origin, float *quat)
{
    char *wheelTagName;
    char *targetNameStr;
    const char *modelName;
    float wheelRadius;
    const vehicle_info_t *info;
    const DObj *obj; // [esp+1Ch] [ebp-18h]
    const char *name; // [esp+20h] [ebp-14h]
    DObjAnimMat *mtx; // [esp+28h] [ebp-Ch]
    scr_vehicle_s *veh; // [esp+2Ch] [ebp-8h]

    veh = ent->scr_vehicle;
    if ( veh->boneIndex.wheel[idx] < 0 )
    {
        info = BG_GetVehicleInfo(veh->infoIdx);
        obj = Com_GetServerDObj(ent->s.number);
        name = 0;
        if ( obj )
            name = DObjGetModel(obj, 0)->name;
        modelName = name ? name : "UNKNOWN";
        targetNameStr = SL_ConvertToString(ent->targetname, SCRIPTINSTANCE_SERVER);
        wheelTagName = SL_ConvertToString(*s_wheelTags[idx], SCRIPTINSTANCE_SERVER);
        Com_Error(ERR_DROP, "Script vehicle [%s] with model [%s] needs [%s] with targetname [%s]", info, modelName, wheelTagName, targetNameStr);
    }
    mtx = G_DObjGetLocalBoneIndexMatrix(ent, veh->boneIndex.wheel[idx]);
    if ( !mtx && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 975, 0, "%s", "mtx") )
        __debugbreak();
    *origin = mtx->trans[0];
    origin[1] = mtx->trans[1];
    origin[2] = mtx->trans[2];
    if ( quat )
    {
        *quat = mtx->quat[0];
        quat[1] = mtx->quat[1];
        quat[2] = mtx->quat[2];
        quat[3] = mtx->quat[3];
    }
    if ( (veh->flags & 0x80) == 0
        && (float)((float)((float)(*origin * *origin) + (float)(origin[1] * origin[1])) + (float)(origin[2] * origin[2])) > (float)(veh->phys.maxs[0] * veh->phys.maxs[0]) )
    {
        Vec3Normalize(origin);
        wheelRadius = veh->phys.maxs[0] - 2.0f;
        *origin = wheelRadius * *origin;
        origin[1] = wheelRadius * origin[1];
        origin[2] = wheelRadius * origin[2];
    }
}

// Decomp: CoDMPServer.c:432119  VEH_GroundPlant
void __cdecl VEH_GroundPlant(gentity_s *ent, vehicle_physic_t *phys, int gravity)
{
    float *hitNormal;
    float *traceEndPt;
    float *traceStartPt;
    float prevOriginZ;
    float proj[4][3]; // [esp+C8h] [ebp-278h] BYREF
    float wheelMatrix[4][3]; // [esp+F8h] [ebp-248h] BYREF
    int contents; // [esp+128h] [ebp-218h]
    col_context_t context; // [esp+12Ch] [ebp-214h] BYREF
    float pt1[3]; // [esp+154h] [ebp-1ECh]
    float plane[4]; // [esp+160h] [ebp-1E0h] BYREF
    const vehicle_info_t *info; // [esp+170h] [ebp-1D0h]
    scr_vehicle_s *veh; // [esp+174h] [ebp-1CCh]
    int numWheels; // [esp+178h] [ebp-1C8h]
    float right[3]; // [esp+17Ch] [ebp-1C4h] BYREF
    int contents_mask; // [esp+188h] [ebp-1B8h]
    float mx[3]; // [esp+18Ch] [ebp-1B4h] BYREF
    float trace_points[37]; // [esp+198h] [ebp-1A8h] BYREF
    float forward[3]; // [esp+22Ch] [ebp-114h] BYREF
    float angles[3]; // [esp+238h] [ebp-108h] BYREF
    float expand_vec[3]; // [esp+244h] [ebp-FCh] BYREF
    trace_t trace; // [esp+250h] [ebp-F0h] BYREF
    float hitPos[3]; // [esp+28Ch] [ebp-B4h] BYREF
    float temp[3]; // [esp+298h] [ebp-A8h] BYREF
    float pt2[3]; // [esp+2A4h] [ebp-9Ch]
    float wheelPos[6][3]; // [esp+2B0h] [ebp-90h] BYREF
    int wheelIndex;
    float mn[3]; // [esp+2FCh] [ebp-44h] BYREF
    vehicle_cache_t *vehicle_cache; // [esp+308h] [ebp-38h]
    float axis[4][3]; // [esp+30Ch] [ebp-34h] BYREF
    float dot; // [esp+33Ch] [ebp-4h]

    memset(&trace, 0, sizeof(trace));
    //col_context_t::col_context_t(&context);
    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 1984, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->scr_vehicle
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    1985,
                    0,
                    "%s",
                    "ent->scr_vehicle") )
    {
        __debugbreak();
    }
    if ( !phys && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 1986, 0, "%s", "phys") )
        __debugbreak();
    veh = ent->scr_vehicle;
    info = BG_GetVehicleInfo(veh->infoIdx);
    if ( info->type
        && info->type != 2
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    1991,
                    0,
                    "%s",
                    "(info->type == VEH_WHEELS_4) || (info->type == VEH_TANK)") )
    {
        __debugbreak();
    }
    if ( info->type )
        numWheels = 6;
    else
        numWheels = 4;
    contents = 529;
    if ( (veh->flags & 1) != 0 )
        contents |= 0x10000u;
    prevOriginZ = phys->prevOrigin[2];
    axis[3][0] = phys->origin[0];
    axis[3][1] = phys->origin[1];
    axis[3][2] = prevOriginZ;
    AnglesToAxis(phys->angles, axis);
    mn[0] = FLT_MAX;
    mn[1] = FLT_MAX;
    mn[2] = FLT_MAX;
    mx[0] = -FLT_MAX;
    mx[1] = -FLT_MAX;
    mx[2] = -FLT_MAX;
    for ( wheelIndex = 0; wheelIndex < numWheels; ++wheelIndex )
    {
        VEH_GetWheelOrigin(ent, wheelIndex, temp, 0);
        MatrixTransformVector43(temp, axis, hitPos);
        traceStartPt = &trace_points[6 * wheelIndex];
        *traceStartPt = hitPos[0];
        traceStartPt[1] = hitPos[1];
        traceStartPt[2] = hitPos[2];
        traceEndPt = &trace_points[6 * wheelIndex + 3];
        *traceEndPt = hitPos[0];
        traceEndPt[1] = hitPos[1];
        traceEndPt[2] = hitPos[2];
        trace_points[6 * wheelIndex + 2] = trace_points[6 * wheelIndex + 2] + 64.0;
        trace_points[6 * wheelIndex + 5] = trace_points[6 * wheelIndex + 5] - 256.0;
        Vec3Min(&trace_points[6 * wheelIndex], mn, mn);
        Vec3Min(&trace_points[6 * wheelIndex + 3], mn, mn);
        Vec3Max(&trace_points[6 * wheelIndex], mx, mx);
        Vec3Max(&trace_points[6 * wheelIndex + 3], mx, mx);
        if ( g_vehicleDebug->current.enabled )
        {
            VEH_DebugBox(hitPos, 4.0, 1.0, 0.0, 0.0);
            VEH_DebugLine(&trace_points[6 * wheelIndex], &trace_points[6 * wheelIndex + 3], 0.0, 0.0, 1.0);
        }
    }
    vehicle_cache = &veh->vehicle_cache;
    contents_mask = contents;
    expand_vec[0] = 30.0f;
    expand_vec[1] = 30.0f;
    expand_vec[2] = 30.0f;
    veh->vehicle_cache.proximity_data.update(mn, mx, contents, expand_vec);
    context.prims = vehicle_cache->proximity_data.prims;
    context.nprims = vehicle_cache->proximity_data.nprims;
    for ( wheelIndex = 0; wheelIndex < numWheels; ++wheelIndex )
    {
        if ( vehicle_cache->hit_indices[wheelIndex] > 0 )
        {
            //*(_QWORD *)trace.normal.vec.v = *(_QWORD *)&vehicle_cache->hit_normals[wheelIndex][0];
            trace.normal.vec.v[0] = vehicle_cache->hit_normals[wheelIndex][0];
            trace.normal.vec.v[1] = vehicle_cache->hit_normals[wheelIndex][1];
            trace.normal.vec.v[2] = vehicle_cache->hit_normals[wheelIndex][2];
            trace.sflags = vehicle_cache->hit_sflags[wheelIndex];
        }
        trace_point_vs_env(&trace, &trace_points[6 * wheelIndex], &trace_points[6 * wheelIndex + 3], &context, &vehicle_cache->hit_indices[wheelIndex]);
        if ( vehicle_cache->hit_indices[wheelIndex] > 0 )
        {
            hitNormal = vehicle_cache->hit_normals[wheelIndex];
            hitNormal[0] = trace.normal.vec.v[0];
            hitNormal[1] = trace.normal.vec.v[1];
            hitNormal[2] = trace.normal.vec.v[2];
            vehicle_cache->hit_sflags[wheelIndex] = trace.sflags;
        }
        if ( trace.fraction >= 1.0 )
        {
            //*(_QWORD *)hitPos = *(_QWORD *)&trace_points[6 * wheelIndex + 3];
            hitPos[0] = trace_points[6 * wheelIndex + 3];
            hitPos[1] = trace_points[6 * wheelIndex + 4];
            hitPos[2] = trace_points[6 * wheelIndex + 5];
            phys->wheelSurfType[wheelIndex] = 0;
        }
        else
        {
            Vec3Lerp(&trace_points[6 * wheelIndex], &trace_points[6 * wheelIndex + 3], trace.fraction, hitPos);
            phys->wheelSurfType[wheelIndex] = (unsigned __int8)((int)(0x3F00000
                                                                                                         & trace.sflags) >> 20);
        }
        if ( gravity )
        {
            phys->wheelZVel[wheelIndex] = phys->wheelZVel[wheelIndex] - 40.0;
            phys->wheelZPos[wheelIndex] = (float)(phys->wheelZVel[wheelIndex] * 0.050000001) + phys->wheelZPos[wheelIndex];
            if ( hitPos[2] > phys->wheelZPos[wheelIndex] )
            {
                phys->wheelZPos[wheelIndex] = hitPos[2];
                phys->wheelZVel[wheelIndex] = 0.0f;
                memcpy(wheelMatrix, axis, 0x24u);
                //*(_QWORD *)&wheelMatrix[3][0] = *(_QWORD *)hitPos;
                wheelMatrix[3][0] = hitPos[0];
                wheelMatrix[3][1] = hitPos[1];
                wheelMatrix[3][2] = hitPos[2];
            }
        }
        else
        {
            phys->wheelZPos[wheelIndex] = hitPos[2];
            phys->wheelZVel[wheelIndex] = 0.0f;
        }
        wheelPos[wheelIndex][0] = hitPos[0];
        wheelPos[wheelIndex][1] = hitPos[1];
        wheelPos[wheelIndex][2] = phys->wheelZPos[wheelIndex];
        if ( g_vehicleDebug->current.enabled )
            VEH_DebugBox(wheelPos[wheelIndex], 4.0, 0.0, 1.0, 0.0);
    }
    pt1[0] = 0.5 * (float)(wheelPos[1][0] + wheelPos[3][0]);
    pt1[1] = 0.5 * (float)(wheelPos[1][1] + wheelPos[3][1]);
    pt1[2] = 0.5 * (float)(wheelPos[1][2] + wheelPos[3][2]);
    pt2[0] = 0.5 * (float)(wheelPos[0][0] + wheelPos[2][0]);
    pt2[1] = 0.5 * (float)(wheelPos[0][1] + wheelPos[2][1]);
    pt2[2] = 0.5 * (float)(wheelPos[0][2] + wheelPos[2][2]);
    right[0] = pt1[0] - pt2[0];
    right[1] = pt1[1] - pt2[1];
    right[2] = pt1[2] - pt2[2];
    Vec3Normalize(right);
    pt1[0] = 0.5 * (float)(wheelPos[0][0] + wheelPos[1][0]);
    pt1[1] = 0.5 * (float)(wheelPos[0][1] + wheelPos[1][1]);
    pt1[2] = 0.5 * (float)(wheelPos[0][2] + wheelPos[1][2]);
    pt2[0] = 0.5 * (float)(wheelPos[2][0] + wheelPos[3][0]);
    pt2[1] = 0.5 * (float)(wheelPos[2][1] + wheelPos[3][1]);
    pt2[2] = 0.5 * (float)(wheelPos[2][2] + wheelPos[3][2]);
    forward[0] = pt1[0] - pt2[0];
    forward[1] = pt1[1] - pt2[1];
    forward[2] = pt1[2] - pt2[2];
    Vec3Normalize(forward);
    Vec3Cross(right, forward, plane);
    plane[3] = (float)((float)(wheelPos[0][0] * plane[0]) + (float)(wheelPos[0][1] * plane[1]))
                     + (float)(wheelPos[0][2] * plane[2]);
    for ( wheelIndex = 1; wheelIndex < numWheels; ++wheelIndex )
    {
        dot = (float)((float)((float)(plane[0] * wheelPos[wheelIndex][0]) + (float)(plane[1] * wheelPos[wheelIndex][1]))
                                + (float)(plane[2] * wheelPos[wheelIndex][2]))
                - plane[3];
        if ( dot > info->suspensionTravel )
            plane[3] = (float)((float)((float)(wheelPos[wheelIndex][0] * plane[0]) + (float)(wheelPos[wheelIndex][1] * plane[1]))
                                             + (float)(wheelPos[wheelIndex][2] * plane[2]))
                             - info->suspensionTravel;
    }
    Vec3Cross(plane, axis[0], axis[1]);
    Vec3Normalize(axis[1]);
    Vec3Cross(axis[1], plane, axis[0]);
    Vec3Normalize(axis[0]);
    AxisToAngles(axis, angles);
    phys->angles[0] = DiffTrackAngle(angles[0], phys->prevAngles[0], 6.0, 0.050000001);
    phys->angles[2] = DiffTrackAngle(angles[2], phys->prevAngles[2], 6.0, 0.050000001);
    if ( phys->angles[0] > 60.0f )
        phys->angles[0] = 60.0f;
    else if ( phys->angles[0] < -60.0f )
        phys->angles[0] = -60.0f;
    if ( phys->angles[2] > 60.0f )
        phys->angles[2] = 60.0f;
    else if ( phys->angles[2] < -60.0f )
        phys->angles[2] = -60.0f;
    if ( ((veh->flags & 0x80) != 0 || (veh->flags & 1) == 0) && plane[2] != 0.0 )
        phys->origin[2] = (-((float)((float)(phys->origin[0] * plane[0]) + (float)(phys->origin[1] * plane[1])) - plane[3])) / plane[2];
    AnglesSubtract(phys->angles, phys->prevAngles, phys->rotVel);
    phys->rotVel[0] = 20.0 * phys->rotVel[0];
    phys->rotVel[1] = 20.0 * phys->rotVel[1];
    phys->rotVel[2] = 20.0 * phys->rotVel[2];
    if ( g_vehicleDebug->current.enabled )
    {
        for ( wheelIndex = 0; wheelIndex < 4; ++wheelIndex )
        {
            proj[wheelIndex][0] = wheelPos[wheelIndex][0];
            proj[wheelIndex][1] = wheelPos[wheelIndex][1];
            proj[wheelIndex][2] = wheelPos[wheelIndex][2];
            proj[wheelIndex][2] = (-((float)((float)(proj[wheelIndex][0] * plane[0]) + (float)(proj[wheelIndex][1] * plane[1])) - plane[3])) / plane[2];
        }
        VEH_DebugLine(proj[0], proj[1], 1.0, 1.0, 0.0);
        VEH_DebugLine(proj[1], proj[3], 1.0, 1.0, 0.0);
        VEH_DebugLine(proj[3], proj[2], 1.0, 1.0, 0.0);
        VEH_DebugLine(proj[2], proj[0], 1.0, 1.0, 0.0);
    }
}

// Decomp: CoDMPServer.c:432488  VEH_DebugLine
void __cdecl VEH_DebugLine(const float *start, const float *end, float r, float g, float b)
{
    float color[4]; // [esp+0h] [ebp-10h] BYREF

    color[0] = r;
    color[1] = g;
    color[2] = b;
    color[3] = 1.0f;
    G_DebugLine(start, end, color, 1);
}

// Decomp: CoDMPServer.c:432503  VEH_DebugBox
void __cdecl VEH_DebugBox(float *pos, float width, float r, float g, float b)
{
    float mins[3]; // [esp+10h] [ebp-28h] BYREF
    float maxs[3]; // [esp+1Ch] [ebp-1Ch] BYREF
    float color[4]; // [esp+28h] [ebp-10h] BYREF

    color[0] = r;
    color[1] = g;
    color[2] = b;
    color[3] = 1.0f;
    //LODWORD(mins[0]) = COERCE_UNSIGNED_INT(width * 0.5) ^ _mask__NegFloat_;
    mins[0] = -(width * 0.5f);
    mins[1] = mins[0];
    mins[2] = mins[0];
    maxs[0] = width * 0.5;
    maxs[1] = width * 0.5;
    maxs[2] = width * 0.5;
    CG_DebugBox(pos, mins, maxs, 0.0, color, 1, 0);
}

static float predictTime = 0.15f;
// Decomp: CoDMPServer.c:432532  VEH_TouchEntities
void __cdecl VEH_TouchEntities(gentity_s *ent)
{
    int contentmask;
    int numTouchEnts;
    float touchMaxs[3];
    DObj *obj;
    scr_vehicle_s *scr_vehicle;
    float absMins[3];
    float absMaxs[3];
    void(__cdecl * vehicleTouch)(gentity_s *, gentity_s *, int);
    gentity_s *touchEnt;
    void(__cdecl * otherTouch)(gentity_s *, gentity_s *, int);
    int entityList[1024];
    int touchEntityIndex;
    float touchMins[3];
    float predictOffset[3];
    float offset[3];

    if (!ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 2649, 0, "%s", "ent"))
        __debugbreak();
    if (!ent->scr_vehicle
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
            2650,
            0,
            "%s",
            "ent->scr_vehicle"))
    {
        __debugbreak();
    }
    scr_vehicle = ent->scr_vehicle;
    BG_GetVehicleInfo(scr_vehicle->infoIdx);
    vehicleTouch = entityHandlers[ent->handler].touch;
    offset[0] = scr_vehicle->phys.origin[0] - scr_vehicle->phys.prevOrigin[0];
    offset[1] = scr_vehicle->phys.origin[1] - scr_vehicle->phys.prevOrigin[1];
    offset[2] = scr_vehicle->phys.origin[2] - scr_vehicle->phys.prevOrigin[2];
    absMins[0] = ent->r.absmin[0];
    absMins[1] = ent->r.absmin[1];
    absMins[2] = ent->r.absmin[2];
    absMaxs[0] = ent->r.absmax[0];
    absMaxs[1] = ent->r.absmax[1];
    absMaxs[2] = ent->r.absmax[2];
    ExtendBounds(absMins, absMaxs, offset);
    predictOffset[0] = predictTime * scr_vehicle->phys.vel[0];
    predictOffset[1] = predictTime * scr_vehicle->phys.vel[1];
    predictOffset[2] = predictTime * scr_vehicle->phys.vel[2];
    absMins[0] = absMins[0] + predictOffset[0];
    absMins[1] = absMins[1] + predictOffset[1];
    absMins[2] = absMins[2] + predictOffset[2];
    absMaxs[0] = absMaxs[0] + predictOffset[0];
    absMaxs[1] = absMaxs[1] + predictOffset[1];
    absMaxs[2] = absMaxs[2] + predictOffset[2];
    contentmask = 0x280E091;
    if (vehicle_riding->current.enabled)
        contentmask = 0x80E091;
    numTouchEnts = CM_AreaEntities(absMins, absMaxs, entityList, 1024, contentmask);
    for ( touchEntityIndex = 0; touchEntityIndex < numTouchEnts; ++touchEntityIndex )
    {
        touchEnt = &g_entities[entityList[touchEntityIndex]];
        otherTouch = entityHandlers[touchEnt->handler].touch;
        if (touchEnt->s.number != ent->s.number
            && (touchEnt->s.eType == 1 || touchEnt->s.eType == 6 || touchEnt->s.eType == 4 || touchEnt->s.eType == 17 || touchEnt->s.eType == 14)
            && touchEnt->s.groundEntityNum != ent->s.number)
        {
            if (touchEnt->classname == scr_const.script_model)
            {
                if (!touchEnt->model)
                    continue;
                obj = Com_GetServerDObj(touchEnt->s.number);
                DObjPhysicsGetBounds(obj, touchMins, touchMaxs);
                touchMins[0] = touchEnt->r.currentOrigin[0] + touchMins[0];
                touchMins[1] = touchEnt->r.currentOrigin[1] + touchMins[1];
                touchMins[2] = touchEnt->r.currentOrigin[2] + touchMins[2];
                touchMaxs[0] = touchEnt->r.currentOrigin[0] + touchMaxs[0];
                touchMaxs[1] = touchEnt->r.currentOrigin[1] + touchMaxs[1];
                touchMaxs[2] = touchEnt->r.currentOrigin[2] + touchMaxs[2];
            }
            else
            {
                touchMins[0] = touchEnt->r.absmin[0];
                touchMins[1] = touchEnt->r.absmin[1];
                touchMins[2] = touchEnt->r.absmin[2];
                touchMaxs[0] = touchEnt->r.absmax[0];
                touchMaxs[1] = touchEnt->r.absmax[1];
                touchMaxs[2] = touchEnt->r.absmax[2];
            }
            ExpandBoundsToWidth(touchMins, touchMaxs);
            touchMins[0] = touchMins[0] - predictOffset[0];
            touchMins[1] = touchMins[1] - predictOffset[1];
            touchMins[2] = touchMins[2] - predictOffset[2];
            touchMaxs[0] = touchMaxs[0] - predictOffset[0];
            touchMaxs[1] = touchMaxs[1] - predictOffset[1];
            touchMaxs[2] = touchMaxs[2] - predictOffset[2];
            if (SV_EntityContact(touchMins, touchMaxs, ent))
            {
                if (Scr_IsSystemActive(1u, SCRIPTINSTANCE_SERVER))
                {
                    Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
                    Scr_Notify(touchEnt, scr_const.touch, 1u);
                    Scr_AddEntity(touchEnt, SCRIPTINSTANCE_SERVER);
                    Scr_Notify(ent, scr_const.touch, 1u);
                }
                if (otherTouch)
                    otherTouch(touchEnt, ent, 1);
                if (vehicleTouch)
                    vehicleTouch(ent, touchEnt, 1);
                if (touchEnt->s.eType == 1 || touchEnt->actor && touchEnt->actor->Physics.bIsAlive)
                    VEH_PushEntity(ent, touchEnt);
            }
        }
    }
    GlassSv_PredictTouch(ent);
}

// Decomp: CoDMPServer.c:432675  VEH_PushEntity
void __cdecl VEH_PushEntity(gentity_s *ent, gentity_s *target)
{
    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 2550, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->scr_vehicle
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    2551,
                    0,
                    "%s",
                    "ent->scr_vehicle") )
    {
        __debugbreak();
    }
    if ( !target
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 2552, 0, "%s", "target") )
    {
        __debugbreak();
    }
    if ( !target->tagInfo && AttachedStickyMissile(ent, target) )
        PushAttachedStickyMissile(ent, target);
}

// Decomp: CoDMPServer.c:432715  AttachedStickyMissile
bool __cdecl AttachedStickyMissile(gentity_s *vehicle, gentity_s *missile)
{
    const WeaponDef *weapDef; // [esp+0h] [ebp-4h]

    if ( !vehicle
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 2209, 0, "%s", "vehicle") )
    {
        __debugbreak();
    }
    if ( !missile
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 2210, 0, "%s", "missile") )
    {
        __debugbreak();
    }
    if ( missile->s.groundEntityNum != vehicle->s.number )
        return 0;
    if ( missile->s.eType != 4 )
        return 0;
    weapDef = BG_GetWeaponDef(missile->s.weapon);
    if ( !weapDef
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 2218, 0, "%s", "weapDef") )
    {
        __debugbreak();
    }
    return weapDef->stickiness == WEAPSTICKINESS_ALL || weapDef->stickiness == WEAPSTICKINESS_ALL_NO_SENTIENTS;
}

// Decomp: CoDMPServer.c:432761  PushAttachedStickyMissile
void __cdecl PushAttachedStickyMissile(gentity_s *vehicle, gentity_s *missile)
{
    scr_vehicle_s *scr_vehicle; // edx
    float newVehMat[3][3]; // [esp+18h] [ebp-F4h] BYREF
    vehicle_physic_t *phys; // [esp+3Ch] [ebp-D0h]
    float origin[3]; // [esp+40h] [ebp-CCh] BYREF
    float oldMissileMat[3][3]; // [esp+4Ch] [ebp-C0h] BYREF
    float newMissileMat[3][3]; // [esp+70h] [ebp-9Ch] BYREF
    float oldVehMatInv[3][3]; // [esp+94h] [ebp-78h] BYREF
    float deltaMat[3][3]; // [esp+B8h] [ebp-54h] BYREF
    float oldVehMat[3][3]; // [esp+DCh] [ebp-30h] BYREF
    float relativeOrig[3]; // [esp+100h] [ebp-Ch] BYREF

    if ( !vehicle
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 2237, 0, "%s", "vehicle") )
    {
        __debugbreak();
    }
    if ( !vehicle->scr_vehicle
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    2238,
                    0,
                    "%s",
                    "vehicle->scr_vehicle") )
    {
        __debugbreak();
    }
    if ( !missile
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 2239, 0, "%s", "missile") )
    {
        __debugbreak();
    }
    scr_vehicle = vehicle->scr_vehicle;
    phys = &scr_vehicle->phys;
    if ( scr_vehicle == (scr_vehicle_s *)-220
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 2242, 0, "%s", "phys") )
    {
        __debugbreak();
    }
    relativeOrig[0] = missile->r.currentOrigin[0] - phys->prevOrigin[0];
    relativeOrig[1] = missile->r.currentOrigin[1] - phys->prevOrigin[1];
    relativeOrig[2] = missile->r.currentOrigin[2] - phys->prevOrigin[2];
    AnglesToAxis(phys->angles, newVehMat);
    AnglesToAxis(phys->prevAngles, oldVehMat);
    MatrixTranspose(oldVehMat, oldVehMatInv);
    MatrixMultiply(oldVehMatInv, newVehMat, deltaMat);
    MatrixTransformVector(relativeOrig, deltaMat, origin);
    origin[0] = origin[0] + phys->origin[0];
    origin[1] = origin[1] + phys->origin[1];
    origin[2] = origin[2] + phys->origin[2];
    missile->r.currentOrigin[0] = origin[0];
    missile->r.currentOrigin[1] = origin[1];
    missile->r.currentOrigin[2] = origin[2];
    missile->s.lerp.pos.trBase[0] = origin[0];
    missile->s.lerp.pos.trBase[1] = origin[1];
    missile->s.lerp.pos.trBase[2] = origin[2];
    AnglesToAxis(missile->r.currentAngles, oldMissileMat);
    MatrixMultiply(oldMissileMat, deltaMat, newMissileMat);
    AxisToAngles(newMissileMat, missile->r.currentAngles);
    missile->s.lerp.apos.trBase[0] = missile->r.currentAngles[0];
    missile->s.lerp.apos.trBase[1] = missile->r.currentAngles[1];
    missile->s.lerp.apos.trBase[2] = missile->r.currentAngles[2];
}

// Decomp: CoDMPServer.c:432856  VEH_UpdateHealth
void __cdecl VEH_UpdateHealth(gentity_s *pSelf)
{
    bool maingun_turningpitch; // [esp+5h] [ebp-1Bh]
    bool maingun_turningyaw; // [esp+6h] [ebp-1Ah]
    bool maingun_overheating; // [esp+7h] [ebp-19h]
    float vehTreadLeftHealthPercentage; // [esp+8h] [ebp-18h]
    bool gunner1_turningpitch; // [esp+Ch] [ebp-14h]
    bool gunner1_turningyaw; // [esp+Dh] [ebp-13h]
    bool driver_seat_occupied; // [esp+Eh] [ebp-12h]
    int seat_occupancy_flags; // [esp+10h] [ebp-10h]
    float vehTreadRightHealthPercentage; // [esp+14h] [ebp-Ch]
    float vehHealthPercentage; // [esp+18h] [ebp-8h]
    bool gunner1_overheating; // [esp+1Fh] [ebp-1h]

    if ( !pSelf->maxHealth || pSelf->health > pSelf->maxHealth )
        pSelf->maxHealth = pSelf->health;
    driver_seat_occupied = VEH_GetSeatOccupantEntity(pSelf, 0) != 0;
    seat_occupancy_flags = (VEH_GetSeatOccupantEntity(pSelf, 1) != 0 ? 2 : 0) | driver_seat_occupied;
    vehHealthPercentage = (float)pSelf->health / (float)pSelf->maxHealth;
    vehTreadLeftHealthPercentage = GetPieceHealthPercentage(
                                                                     pSelf->destructible,
                                                                     scr_const.left_tread,
                                                                     vehHealthPercentage);
    vehTreadRightHealthPercentage = GetPieceHealthPercentage(
                                                                        pSelf->destructible,
                                                                        scr_const.right_tread,
                                                                        vehHealthPercentage);
    maingun_turningyaw = PlayersSeatGunTurning(pSelf, 0, 1);
    maingun_turningpitch = PlayersSeatGunTurning(pSelf, 0, 0);
    gunner1_turningyaw = PlayersSeatGunTurning(pSelf, 1, 1);
    gunner1_turningpitch = PlayersSeatGunTurning(pSelf, 1, 0);
    maingun_overheating = pSelf->scr_vehicle->turret.overheating != 0;
    gunner1_overheating = pSelf->scr_vehicle->gunnerTurrets[0].overheating != 0;
    G_VehSetClientSideHealthPercentageEntity(pSelf, vehHealthPercentage);
    G_VehSetClientSideHealthPercentageLeftTread(pSelf, vehTreadLeftHealthPercentage);
    G_VehSetClientSideHealthPercentageRightTread(pSelf, vehTreadRightHealthPercentage);
    G_VehSetClientSideSeatOccupancyFlags(pSelf, seat_occupancy_flags);
    G_VehSetClientSideGunTurningYaw(pSelf, 0, maingun_turningyaw);
    G_VehSetClientSideGunTurningYaw(pSelf, 1u, gunner1_turningyaw);
    G_VehSetClientSideGunTurningPitch(pSelf, 0, maingun_turningpitch);
    G_VehSetClientSideGunTurningPitch(pSelf, 1u, gunner1_turningpitch);
    G_VehSetClientSideGunOverheating(pSelf, 0, maingun_overheating);
    G_VehSetClientSideGunOverheating(pSelf, 1u, gunner1_overheating);
}

// Decomp: CoDMPServer.c:432904  GetPieceHealthPercentage
double __cdecl GetPieceHealthPercentage(
                Destructible *dest,
                unsigned __int16 destructibleLabelString,
                float defaultPercentage)
{
    int iPieceHealth; // [esp+0h] [ebp-18h] BYREF
    int pieceIndex; // [esp+4h] [ebp-14h]
    float pieceMaxHealth; // [esp+8h] [ebp-10h]
    DestructibleDef *ddef; // [esp+10h] [ebp-8h]
    float pieceHealth; // [esp+14h] [ebp-4h]

    if ( !dest )
        return defaultPercentage;
    ddef = Destructible_GetDDef(dest);
    pieceIndex = Destructible_GetPieceIndexForLabel(ddef, destructibleLabelString);
    if ( !GetPieceHealth(dest, destructibleLabelString, &iPieceHealth) )
        return defaultPercentage;
    pieceHealth = (float)iPieceHealth;
    pieceMaxHealth = (float)Destructible_GetPieceMaxHealth(ddef, pieceIndex);
    return (float)(pieceHealth / pieceMaxHealth);
}

// Decomp: CoDMPServer.c:432924  GetPieceHealth
char __cdecl GetPieceHealth(Destructible *dest, unsigned __int16 destructibleLabelString, int *pieceHealth)
{
    int pieceIndex; // [esp+0h] [ebp-8h]
    DestructibleDef *ddef; // [esp+4h] [ebp-4h]

    if ( !dest )
        return 0;
    ddef = Destructible_GetDDef(dest);
    pieceIndex = Destructible_GetPieceIndexForLabel(ddef, destructibleLabelString);
    if ( pieceIndex < 0 )
        return 0;
    *pieceHealth = Destructible_GetPieceHealth(dest, pieceIndex);
    return 1;
}

// Decomp: CoDMPServer.c:432940  PlayersSeatGunTurning
bool __cdecl PlayersSeatGunTurning(gentity_s *const pVehEnt, int seatIndex, int angle)
{
    const gentity_s *pOccupant; // [esp+Ch] [ebp-4h]

    pOccupant = VEH_GetSeatOccupantEntity(pVehEnt, seatIndex);
    return pOccupant
            && pOccupant->client
            && (int)abs(pOccupant->client->sess.cmd.angles[angle] - pOccupant->client->sess.oldcmd.angles[angle]) >= 5;
}

// Decomp: CoDMPServer.c:432954  VEH_UpdateNitrousPosition
void __cdecl VEH_UpdateNitrousPosition(gentity_s *pSelf)
{
    float throttleQuantized;
    float *pathOrigin;
    NitrousVehicle *nitrousVeh; // [esp+4Ch] [ebp-A0h]
    float absPos[3]; // [esp+50h] [ebp-9Ch] BYREF
    vehicle_physic_t *phys; // [esp+5Ch] [ebp-90h]
    const vehicle_info_t *info; // [esp+60h] [ebp-8Ch]
    scr_vehicle_s *veh; // [esp+68h] [ebp-84h]
    float zero[3]; // [esp+6Ch] [ebp-80h] BYREF
    float hitp[3]; // [esp+78h] [ebp-74h] BYREF
    gentity_s *driver; // [esp+84h] [ebp-68h]
    float tvel[3]; // [esp+88h] [ebp-64h] BYREF
    float intensity; // [esp+94h] [ebp-58h]
    int hit_stype; // [esp+98h] [ebp-54h]
    float angles[3]; // [esp+9Ch] [ebp-50h] BYREF
    float absAxis[3][3]; // [esp+A8h] [ebp-44h] BYREF
    float hitn[3]; // [esp+CCh] [ebp-20h] BYREF
    int hit_entnum; // [esp+D8h] [ebp-14h]
    float avel[3]; // [esp+DCh] [ebp-10h] BYREF
    int notifyFlags; // [esp+E8h] [ebp-4h]

    PROF_SCOPED("VEH_UpdateNitrousPosition");

    if ( !pSelf && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 8896, 0, "%s", "pSelf") )
        __debugbreak();
    if ( !pSelf->scr_vehicle
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    8897,
                    0,
                    "%s",
                    "pSelf->scr_vehicle") )
    {
        __debugbreak();
    }
    veh = pSelf->scr_vehicle;
    phys = &veh->phys;
    info = BG_GetVehicleInfo(veh->infoIdx);
    nitrousVeh = veh->nitrousVehicle;
    memset(zero, 0, sizeof(zero));
    Sys_EnterCriticalSection(CRITSECT_PHYSICS);
    Phys_ObjGetPosition((int)nitrousVeh->m_phys_user_data, absPos, absAxis);
    Phys_ObjGetVelocities((int)nitrousVeh->m_phys_user_data, tvel, avel);
    notifyFlags = nitrousVeh->m_server_notify_flags;
    nitrousVeh->m_server_notify_flags = 0;
    intensity = nitrousVeh->m_collision_intensity;
    hit_stype = nitrousVeh->m_collision_stype;
    hit_entnum = nitrousVeh->m_collision_entnum;
    hitp[0] = nitrousVeh->m_collision_hitp[0];
    hitp[1] = nitrousVeh->m_collision_hitp[1];
    hitp[2] = nitrousVeh->m_collision_hitp[2];
    hitn[0] = nitrousVeh->m_collision_hitn[0];
    hitn[1] = nitrousVeh->m_collision_hitn[1];
    hitn[2] = nitrousVeh->m_collision_hitn[2];
    driver = VEH_GetSeatOccupantEntity(veh, 0);
    if ( driver && driver->client )
        VEH_SetPlayerVehicle(pSelf, 1);
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
    AxisToAngles(absAxis, angles);
    if ( (nitrousVeh->m_flags & 0x100) == 0 && (nitrousVeh->m_flags & 0x80) == 0 )
        veh->speed = nitrousVeh->m_forward_vel;
    if ( (nitrousVeh->m_flags & 0x80) == 0 )
    {
        phys->origin[0] = absPos[0];
        phys->origin[1] = absPos[1];
        phys->origin[2] = absPos[2];
        phys->angles[0] = angles[0];
        phys->angles[1] = angles[1];
        phys->angles[2] = angles[2];
        phys->vel[0] = tvel[0];
        phys->vel[1] = tvel[1];
        phys->vel[2] = tvel[2];
    }
    if ( sv_clientSideVehicles->current.enabled )
    {
        throttleQuantized = floor(nitrousVeh->m_throttle * 32768.0 + 0.5);
        pSelf->s.lerp.u.vehicle.throttle = (int)throttleQuantized;
        //pSelf->s.lerp.u.actor.actorNum = LODWORD(nitrousVeh->m_steer_factor);
        pSelf->s.lerp.u.actor.actorNum = int(nitrousVeh->m_steer_factor); // KISAKTODO: union abuse
        if ( (veh->flags & 0x100) != 0 && (nitrousVeh->m_flags & 0x80) == 0 )
        {
            pathOrigin = veh->pathPos.origin;
            veh->pathPos.origin[0] = absPos[0];
            pathOrigin[1] = absPos[1];
        }
    }
    else if ( info->type && info->type != 1 )
    {
        pSelf->s.lerp.u.actor.actorNum = 0;
    }
    else
    {
        pSelf->s.lerp.u.turret.gunAngles[0] = (float)(info->nitrousVehParams.m_steer_angle_max * 57.295776)
                                                                                * nitrousVeh->m_steer_factor;
    }
    VEH_SetPosition(pSelf, absPos, tvel, angles);
    pSelf->s.lerp.pos.trDelta[0] = tvel[0];
    pSelf->s.lerp.pos.trDelta[1] = tvel[1];
    pSelf->s.lerp.pos.trDelta[2] = tvel[2];
    pSelf->s.lerp.pos.trDelta[0] = (float)(int)pSelf->s.lerp.pos.trDelta[0];
    pSelf->s.lerp.pos.trDelta[1] = (float)(int)pSelf->s.lerp.pos.trDelta[1];
    pSelf->s.lerp.pos.trDelta[2] = (float)(int)pSelf->s.lerp.pos.trDelta[2];
    if ( (float)((float)((float)(avel[0] * avel[0]) + (float)(avel[1] * avel[1])) + (float)(avel[2] * avel[2])) >= 0.001 )
    {
        pSelf->s.lerp.apos.trDelta[0] = avel[0];
        pSelf->s.lerp.apos.trDelta[1] = avel[1];
        pSelf->s.lerp.apos.trDelta[2] = avel[2];
    }
    else
    {
        pSelf->s.lerp.apos.trDelta[0] = 0.0f;
        pSelf->s.lerp.apos.trDelta[1] = 0.0f;
        pSelf->s.lerp.apos.trDelta[2] = 0.0f;
    }
    pSelf->s.lerp.pos.trTime = Phys_GetCurrentTime();
    if ( notifyFlags )
    {
        if ( (notifyFlags & 1) != 0 )
        {
            if ( hit_entnum == 1022 )
                Scr_AddUndefined(SCRIPTINSTANCE_SERVER);
            else
                Scr_AddEntityNum(hit_entnum, 0, SCRIPTINSTANCE_SERVER, 0);
            Scr_AddString((char *)Com_SurfaceTypeToName(hit_stype), SCRIPTINSTANCE_SERVER);
            Scr_AddFloat(intensity, SCRIPTINSTANCE_SERVER);
            Scr_AddVector(hitn, SCRIPTINSTANCE_SERVER);
            Scr_AddVector(hitp, SCRIPTINSTANCE_SERVER);
            Scr_Notify(pSelf, scr_const.veh_collision, 5u);
        }
        if ( (notifyFlags & 2) != 0 )
            Scr_Notify(pSelf, scr_const.veh_inair, 0);
        if ( (notifyFlags & 4) != 0 )
            Scr_Notify(pSelf, scr_const.veh_landed, 0);
        if ( (notifyFlags & 8) != 0 )
            Scr_Notify(pSelf, scr_const.veh_suspension_limit_activated, 0);
        if ( (notifyFlags & 0x10) != 0 )
            Scr_Notify(pSelf, scr_const.veh_engine_stutter, 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_VehicleRestorePaths ~82637B78)
void __cdecl Scr_VehicleRestorePaths(gentity_s *pSelf)
{
    scr_vehicle_s *veh;
    phys_vec3 goalPosition;
    float goalSpeed;

    if ( !pSelf && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 9055, 0, "%s", "pSelf") )
        __debugbreak();
    if ( !pSelf->scr_vehicle
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    9056,
                    0,
                    "%s",
                    "pSelf->scr_vehicle") )
    {
        __debugbreak();
    }
    veh = pSelf->scr_vehicle;
    if ( !veh->nitrousVehicle )
        return;
    if ( (veh->flags & 0x180) != 0 )
    {
        if ( (veh->flags & 0x100) != 0 )
            veh->nitrousVehicle->start_path(1);
        else
            veh->nitrousVehicle->start_path(0);
    }
    else if ( (veh->flags & 2) != 0 )
    {
        goalSpeed = veh->nitrousVehicle->get_max_speed(true);
        if ( veh->manualMode == 1 )
            goalSpeed = veh->manualSpeed;
        Phys_Vec3ToNitrousVec(veh->goalPosition, &goalPosition);
        veh->nitrousVehicle->mVehicleController.SetScriptTarget(
            veh->nitrousVehicle,
            &goalPosition,
            veh->hover.hoverRadius,
            goalSpeed,
            veh->stopAtGoal != 0);
    }
}

// Decomp: CoDMPServer.c:433144  Scr_Vehicle_Think
void __cdecl Scr_Vehicle_Think(gentity_s *pSelf)
{
    NitrousVehicle *nitrousVeh;
    const vehicle_info_t *info; // [esp+14h] [ebp-10h]
    scr_vehicle_s *veh; // [esp+1Ch] [ebp-8h]
    float defaultPitch; // [esp+20h] [ebp-4h]

    PROF_SCOPED("Scr_Vehicle_think");

    if ( !pSelf && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 9059, 0, "%s", "pSelf") )
        __debugbreak();
    if ( !pSelf->scr_vehicle
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    9060,
                    0,
                    "%s",
                    "pSelf->scr_vehicle") )
    {
        __debugbreak();
    }
    veh = pSelf->scr_vehicle;
    info = BG_GetVehicleInfo(veh->infoIdx);
    if ( info->isNitrous && !veh->nitrousVehicle )
    {
        Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
        Sys_EnterCriticalSection(CRITSECT_PHYSICS);
        veh->nitrousVehicle = (NitrousVehicle *)NitrousVehicle::add_vehicle(pSelf->s.number);
        if (veh->nitrousVehicle)
        {
            //NitrousVehicle::init(veh->nitrousVehicle, pSelf, &info->nitrousVehParams);
            veh->nitrousVehicle->init(pSelf, &info->nitrousVehParams);
            Scr_VehicleRestorePaths(pSelf);
        }
        Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
        Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
    }
    VEH_BackupPosition(pSelf);
    memset((unsigned __int8 *)&s_phys, 0, sizeof(s_phys));
    //if ( EntHandle::isDefined(&pSelf->r.ownerNum) && g_entities[EntHandle::entnum(&pSelf->r.ownerNum)].health <= 0 )
    if ( pSelf->r.ownerNum.isDefined() && g_entities[pSelf->r.ownerNum.entnum()].health <= 0)
    {
        G_EntUnlink(&g_entities[pSelf->r.ownerNum.entnum()]);
    }
    if ( (veh->flags & 0x40) == 0 )
        pSelf->s.lerp.eFlags &= ~0x40u;
    if ( info->type != 5 )
    {
        if ( (veh->flags & 0x80) != 0 )
        {
            VEH_UpdatePath(pSelf);
        }
        else if ( (veh->flags & 2) != 0 )
        {
            VEH_UpdateAIMove(pSelf);
        }
        else if ( info->type == 3 )
        {
            VEH_UpdateClientPlane(pSelf);
        }
        else if ( info->type == 6 )
        {
            VEH_UpdateClientChopper(pSelf);
        }
        else
        {
            VEH_UpdateClient(pSelf);
        }
        if ( info->type == 6 && (veh->flags & 0x100) != 0 )
            VEH_UpdateChopperPathDrive(pSelf);
    }
    defaultPitch = 0.0f;
    if ( info->type == 6 )
    {
        if ( veh->hasDefaultPitch )
            defaultPitch = veh->defaultPitch;
        else
            defaultPitch = vehHelicopterDefaultPitch->current.value;
        veh->phys.angles[0] = veh->phys.angles[0] + defaultPitch;
    }
    nitrousVeh = pSelf->scr_vehicle->nitrousVehicle;
    if ( nitrousVeh && nitrousVeh->m_phys_user_data )
        VEH_UpdateNitrousPosition(pSelf);
    else
        VEH_SetPosition(pSelf, veh->phys.origin, veh->phys.vel, veh->phys.angles);
    if ( (veh->flags & 2) != 0 )
        VEH_MoveTrace(pSelf);
    VEH_UpdateAvoidance(pSelf);
    VEH_TouchEntities(pSelf);
    G_DoTouchTriggers(pSelf);
    veh->turret.flags &= ~1u;
    VEH_UpdateDriverWeapons(pSelf);
    VEH_UpdateAim(pSelf);
    VEH_UpdateGunnerAimAll(pSelf);
    VEH_UpdateGunnerWeaponsAll(pSelf, 50);
    VEH_UpdateBody(pSelf);
    if ( !veh->nitrousVehicle )
        VEH_UpdateSteering(pSelf);
    //BLOPS_NULLSUB();
    VEH_UpdateHealth(pSelf);
    pSelf->nextthink = level.time + 50;
    VEH_UpdateDebug(pSelf);
    if ( vehicle_riding->current.enabled )
        G_MoverTeam_New(pSelf);
    if ( info->type == 6 )
        veh->phys.angles[0] = veh->phys.angles[0] - defaultPitch;
}

// Decomp: CoDMPServer.c:433264  VEH_MoveTrace
void __cdecl VEH_MoveTrace(gentity_s *ent)
{
    col_context_t context; // [esp+1Ch] [ebp-84h] BYREF
    vehicle_physic_t *phys; // [esp+44h] [ebp-5Ch]
    float size[3]; // [esp+48h] [ebp-58h]
    int dimIndex; // [esp+54h] [ebp-4Ch]
    trace_t trace; // [esp+58h] [ebp-48h] BYREF
    float maxs[3]; // [esp+94h] [ebp-Ch] BYREF

    memset(&trace, 0, sizeof(trace));
    if ( Scr_IsSystemActive(1u, SCRIPTINSTANCE_SERVER) )
    {
        phys = &ent->scr_vehicle->phys;
        if ( Vec3DistanceSq(phys->prevOrigin, phys->origin) != 0.0 )
        {
            maxs[0] = ent->r.maxs[0];
            maxs[1] = ent->r.maxs[1];
            maxs[2] = ent->r.maxs[2];
            size[0] = maxs[0] - ent->r.mins[0];
            size[1] = maxs[1] - ent->r.mins[1];
            size[2] = maxs[2] - ent->r.mins[2];
            dimIndex = size[0] < size[1];
            if ( size[dimIndex] > size[2] )
                maxs[2] = (float)(size[dimIndex] - size[2]) + maxs[2];
            //col_context_t::col_context_t(&context);
            G_TraceCapsule(&trace, phys->prevOrigin, ent->r.mins, maxs, phys->origin, ent->s.number, 8321, &context);
            if ( trace.fraction < 1.0 && Trace_GetEntityHitId(&trace) == 1022 )
            {
                Scr_AddVector(trace.normal.vec.v, SCRIPTINSTANCE_SERVER);
                Scr_Notify(ent, scr_const.script_vehicle_collision, 1u);
            }
        }
    }
}

// Decomp: CoDMPServer.c:433328  VEH_BackupPosition
void __cdecl VEH_BackupPosition(gentity_s *ent)
{
    scr_vehicle_s *veh; // [esp+18h] [ebp-4h]

    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 2191, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->scr_vehicle
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    2192,
                    0,
                    "%s",
                    "ent->scr_vehicle") )
    {
        __debugbreak();
    }
    veh = ent->scr_vehicle;
    veh->phys.prevOrigin[0] = veh->phys.origin[0];
    veh->phys.prevOrigin[1] = veh->phys.origin[1];
    veh->phys.prevOrigin[2] = veh->phys.origin[2];
    veh->phys.prevAngles[0] = veh->phys.angles[0];
    veh->phys.prevAngles[1] = veh->phys.angles[1];
    veh->phys.prevAngles[2] = veh->phys.angles[2];
    memcpy(&s_backup.phys, &veh->phys, sizeof(s_backup.phys));
}

// Decomp: CoDMPServer.c:433369  VEH_UpdateAim
void __cdecl VEH_UpdateAim(gentity_s *ent)
{
    float gunPitchQuantized;
    float gunYawQuantized;
    vehicle_physic_t *phys;
    const vehicle_info_t *info; // [esp+88h] [ebp-114h]
    float mtx[3][3]; // [esp+8Ch] [ebp-110h] BYREF
    float decompressedGunYaw; // [esp+B0h] [ebp-ECh]
    scr_vehicle_s *veh; // [esp+B4h] [ebp-E8h]
    float tgtDir[3]; // [esp+B8h] [ebp-E4h] BYREF
    float tgtAngles[3]; // [esp+C4h] [ebp-D8h] BYREF
    float invMtx[3][3]; // [esp+D0h] [ebp-CCh] BYREF
    gentity_s *tgtEnt; // [esp+F4h] [ebp-A8h]
    float decompressedGunPitch; // [esp+F8h] [ebp-A4h]
    float deltaAngles[3]; // [esp+FCh] [ebp-A0h] BYREF
    float angles[3]; // [esp+108h] [ebp-94h] BYREF
    float stopAngles[3]; // [esp+114h] [ebp-88h]
    float turretBaseMat[4][3]; // [esp+120h] [ebp-7Ch] BYREF
    float barrelPos[3]; // [esp+150h] [ebp-4Ch] BYREF
    float prevAngles[3]; // [esp+15Ch] [ebp-40h] BYREF
    float axis[3][3]; // [esp+168h] [ebp-34h] BYREF
    gentity_s *player; // [esp+18Ch] [ebp-10h]
    float tgtPos[3]; // [esp+190h] [ebp-Ch] BYREF

    veh = ent->scr_vehicle;
    phys = &veh->phys;
    info = BG_GetVehicleInfo(veh->infoIdx);
    tgtEnt = 0;
    if ( veh->hasTarget && ent->health > 0 )
    {
        if ( veh->targetEnt == 1023 )
        {
            tgtEnt = 0;
            tgtPos[0] = veh->targetOrigin[0];
            tgtPos[1] = veh->targetOrigin[1];
            tgtPos[2] = veh->targetOrigin[2];
        }
        else
        {
            tgtEnt = &g_entities[veh->targetEnt];
            tgtPos[0] = tgtEnt->r.currentOrigin[0] + veh->targetOffset[0];
            tgtPos[1] = tgtEnt->r.currentOrigin[1] + veh->targetOffset[1];
            tgtPos[2] = tgtEnt->r.currentOrigin[2] + veh->targetOffset[2];
        }
        if ( veh->boneIndex.barrel >= 0 )
        {
            G_DObjGetWorldBoneIndexPos(ent, veh->boneIndex.barrel, barrelPos);
            if ( veh->boneIndex.turret_base == -1 )
            {
                angles[0] = phys->angles[0];
                angles[1] = phys->angles[1];
                angles[2] = phys->angles[2];
            }
            else
            {
                G_DObjGetWorldBoneIndexMatrix(ent, veh->boneIndex.turret_base, turretBaseMat);
                AxisToAngles(turretBaseMat, angles);
            }
            //if ( EntHandle::isDefined(&ent->r.ownerNum) )
            if (ent->r.ownerNum.isDefined())
                player = ent->r.ownerNum.ent();
            else
                player = 0;
            if ( player && !player->client->ps.vehiclePos && veh->targetEnt == 1023 )
            {
                tgtDir[0] = veh->targetOrigin[0] - barrelPos[0];
                tgtDir[1] = veh->targetOrigin[1] - barrelPos[1];
                tgtDir[2] = veh->targetOrigin[2] - barrelPos[2];
                Vec3Normalize(tgtDir);
                vectoangles(tgtDir, tgtAngles);
                if ( g_vehicleDebug->current.enabled )
                    CL_AddDebugLine(veh->targetOrigin, barrelPos, colorRed, 1, 1);
                if ( info->type != 6 )
                    tgtAngles[1] = player->client->ps.viewangles[1];
                tgtAngles[2] = 0.0f;
            }
            else
            {
                if ( g_vehicleDebug->current.enabled )
                    CL_AddDebugLine(tgtPos, barrelPos, colorRed, 1, 1);
                tgtDir[0] = tgtPos[0] - barrelPos[0];
                tgtDir[1] = tgtPos[1] - barrelPos[1];
                tgtDir[2] = tgtPos[2] - barrelPos[2];
                Vec3Normalize(tgtDir);
                vectoangles(tgtDir, tgtAngles);
            }
            AnglesToAxis(tgtAngles, axis);
            AnglesToAxis(angles, mtx);
            MatrixTranspose(mtx, invMtx);
            MatrixMultiply(axis, invMtx, mtx);
            AxisToAngles(mtx, tgtAngles);
            decompressedGunYaw = (float)ent->s.lerp.u.vehicle.gunYaw * 0.0054931641;
            decompressedGunPitch = (float)ent->s.lerp.u.vehicle.gunPitch * 0.0054931641;
            decompressedGunYaw = (float)(decompressedGunYaw - phys->angles[1]) + veh->phys.prevAngles[1];
            prevAngles[0] = decompressedGunPitch;
            prevAngles[1] = decompressedGunYaw;
            prevAngles[2] = 0.0f;
            AnglesSubtract(tgtAngles, prevAngles, deltaAngles);
            (deltaAngles[0]) = fabs(deltaAngles[0]);
            (deltaAngles[1]) = fabs(deltaAngles[1]);
            decompressedGunPitch = LinearTrackAngle(
                                                             tgtAngles[0],
                                                             prevAngles[0],
                                                             info->turretRotRate * veh->turretRotScale,
                                                             0.050000001);
            decompressedGunYaw = LinearTrackAngle(
                                                         tgtAngles[1],
                                                         prevAngles[1],
                                                         info->turretRotRate * veh->turretRotScale,
                                                         0.050000001);
            if ( vehLockTurretToPlayerView->current.enabled && player && !player->client->ps.vehiclePos )
                decompressedGunYaw = AngleNormalize180(tgtAngles[1]);
            stopAngles[0] = decompressedGunPitch;
            stopAngles[1] = decompressedGunYaw;
            stopAngles[2] = 0.0f;
            stopAngles[0] = AngleNormalize180(decompressedGunPitch - decompressedGunPitch);
            stopAngles[1] = AngleNormalize180(stopAngles[1] - decompressedGunYaw);
            gunYawQuantized = floor(decompressedGunYaw * 182.04445 + 0.5);
            ent->s.lerp.u.vehicle.gunYaw = (int)gunYawQuantized;
            gunPitchQuantized = floor(decompressedGunPitch * 182.04445 + 0.5);
            ent->s.lerp.u.vehicle.gunPitch = (int)gunPitchQuantized;
            if ( deltaAngles[0] >= 2.0 && stopAngles[0] == 0.0 || deltaAngles[1] >= 2.0 && stopAngles[1] == 0.0 )
            {
                veh->turret.turretState = VEH_TURRET_MOVING;
            }
            else if ( veh->turret.turretState == VEH_TURRET_MOVING )
            {
                veh->turret.turretState = VEH_TURRET_STOPPING;
            }
            else if ( veh->turret.turretState == VEH_TURRET_STOPPING )
            {
                Scr_Notify(ent, scr_const.turret_rotate_stopped, 0);
                veh->turret.turretState = VEH_TURRET_STOPPED;
            }
            if ( deltaAngles[0] >= 1.0 || deltaAngles[1] >= 1.0 )
            {
                Scr_Notify(ent, scr_const.turret_not_on_target, 0);
                Scr_Notify(ent, scr_const.turret_no_vis, 0);
            }
            else
            {
                if ( g_vehicleDebug->current.enabled )
                    VEH_DebugAim(ent, colorBlue, 40);
                Scr_Notify(ent, scr_const.turret_on_target, 0);
                if ( tgtEnt )
                {
                    //col_context_t::col_context_t(&context, 2049);
                    col_context_t context(0x801); // [esp+5Ch] [ebp-140h] BYREF

                    context.passEntityNum0 = ent->s.number;
                    context.passEntityNum1 = tgtEnt->s.number;
                    SV_SightTracePoint(&veh->turretHitNum, barrelPos, tgtPos, &context);
                    if ( veh->turretHitNum )
                        Scr_Notify(ent, scr_const.turret_no_vis, 0);
                    else
                        Scr_Notify(ent, scr_const.turret_on_vistarget, 0);
                }
                else
                {
                    Scr_Notify(ent, scr_const.turret_no_vis, 0);
                }
            }
            if ( g_vehicleDebug->current.enabled )
                VEH_DebugAim(ent, colorYellow, 1);
        }
    }
    else if ( veh->turret.turretState == VEH_TURRET_MOVING )
    {
        veh->turret.turretState = VEH_TURRET_STOPPING;
    }
    else if ( veh->turret.turretState == VEH_TURRET_STOPPING )
    {
        veh->turret.turretState = VEH_TURRET_STOPPED;
    }
}

void __cdecl VEH_UpdateGunnerAimAll(gentity_s *ent)
{
    const vehicle_info_t *info; // [esp+0h] [ebp-8h]
    int i; // [esp+4h] [ebp-4h]

    for ( i = 0; i < 4; ++i )
    {
        info = BG_GetVehicleInfo(ent->scr_vehicle->infoIdx);
        if ( VEH_IsSeatPresent(ent, i + 1, 0) || info->driverControlledGunPos == i )
            VEH_UpdateGunnerAim(ent, i);
    }
}

void __cdecl VEH_UpdateGunnerAim(gentity_s *ent, int gunnerIndex)
{
    float gunnerYawQuantized;
    float gunnerPitchQuantized;
    float viewYawQuantized;
    float viewPitchQuantized;
    float clampedYaw;
    float clampedPitch;
    float leftArc;
    float bottomArc; // [esp+58h] [ebp-130h]
    const vehicle_info_t *info; // [esp+98h] [ebp-F0h]
    float mtx[3][3]; // [esp+9Ch] [ebp-ECh] BYREF
    scr_vehicle_s *veh; // [esp+C0h] [ebp-C8h]
    float tgtDir[3]; // [esp+C4h] [ebp-C4h] BYREF
    float tgtAngles[3]; // [esp+D0h] [ebp-B8h] BYREF
    float invMtx[3][3]; // [esp+DCh] [ebp-ACh] BYREF
    float yaw; // [esp+100h] [ebp-88h]
    gentity_s *tgtEnt; // [esp+104h] [ebp-84h]
    float turretOnTargetRange; // [esp+108h] [ebp-80h]
    float deltaAngles[3]; // [esp+10Ch] [ebp-7Ch] BYREF
    float angles[3]; // [esp+118h] [ebp-70h] BYREF
    float stopAngles[3]; // [esp+124h] [ebp-64h]
    bool worldTargetAngles; // [esp+133h] [ebp-55h]
    float pitch; // [esp+134h] [ebp-54h]
    const WeaponDef *weapDef; // [esp+138h] [ebp-50h]
    float barrelPos[3]; // [esp+13Ch] [ebp-4Ch] BYREF
    float prevAngles[3]; // [esp+148h] [ebp-40h] BYREF
    float axis[3][3]; // [esp+154h] [ebp-34h] BYREF
    gentity_s *player; // [esp+178h] [ebp-10h]
    float tgtPos[3]; // [esp+17Ch] [ebp-Ch] BYREF

    if ( !g_connectpaths->current.integer )
    {
        veh = ent->scr_vehicle;
        info = BG_GetVehicleInfo(veh->infoIdx);
        tgtEnt = 0;
        weapDef = BG_GetWeaponDef(info->gunnerWeaponIndex[gunnerIndex]);
        if ( ent->health > 0 )
        {
            pitch = (float)ent->s.lerp.u.vehicle.gunnerAngles[gunnerIndex].pitch * 0.0054931641;
            yaw = (float)ent->s.lerp.u.vehicle.gunnerAngles[gunnerIndex].yaw * 0.0054931641;
            prevAngles[0] = pitch;
            prevAngles[1] = yaw;
            prevAngles[2] = 0.0f;
            if ( veh->gunnerTargets[gunnerIndex].targetEnt == 1023 )
            {
                tgtEnt = 0;
                tgtPos[0] = veh->gunnerTargets[gunnerIndex].targetOrigin[0];
                tgtPos[1] = veh->gunnerTargets[gunnerIndex].targetOrigin[1];
                tgtPos[2] = veh->gunnerTargets[gunnerIndex].targetOrigin[2];
            }
            else
            {
                tgtEnt = &g_entities[veh->gunnerTargets[gunnerIndex].targetEnt];
                tgtPos[0] = tgtEnt->r.currentOrigin[0] + veh->gunnerTargets[gunnerIndex].targetOffset[0];
                tgtPos[1] = tgtEnt->r.currentOrigin[1] + veh->gunnerTargets[gunnerIndex].targetOffset[1];
                tgtPos[2] = tgtEnt->r.currentOrigin[2] + veh->gunnerTargets[gunnerIndex].targetOffset[2];
            }
            if ( veh->boneIndex.gunnerTags[gunnerIndex].barrel >= 0 )
            {
                worldTargetAngles = 1;
                player = VEH_GetSeatOccupantEntity(veh, gunnerIndex + 1);
                if ( !player && info->driverControlledGunPos == gunnerIndex )
                    player = VEH_GetSeatOccupantEntity(veh, 0);
                if ( player && player->client && player->client->ps.leanf == 0.0 && !veh->gunnerTargets[gunnerIndex].valid )
                {
                    if ( !player->client
                        && !Assert_MyHandler(
                                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                                    3063,
                                    0,
                                    "%s",
                                    "player->client") )
                    {
                        __debugbreak();
                    }
                    if ( (player->client->ps.eFlags & 0x10000) != 0 )
                    {
                        tgtAngles[1] = info->gunnerRestAngles[gunnerIndex][1];
                        tgtAngles[0] = info->gunnerRestAngles[gunnerIndex][0];
                        tgtAngles[2] = 0.0f;
                        worldTargetAngles = 0;
                    }
                    else
                    {
                        tgtAngles[0] = player->client->ps.viewangles[0];
                        tgtAngles[1] = player->client->ps.viewangles[1];
                        tgtAngles[2] = 0.0f;
                        worldTargetAngles = 1;
                    }
                }
                else if ( veh->gunnerTargets[gunnerIndex].valid )
                {
                    G_DObjGetWorldBoneIndexPos(ent, veh->boneIndex.gunnerTags[gunnerIndex].barrel, barrelPos);

                    iassert(!IS_NAN((barrelPos)[0]) && !IS_NAN((barrelPos)[1]) && !IS_NAN((barrelPos)[2]));

                    tgtDir[0] = tgtPos[0] - barrelPos[0];
                    tgtDir[1] = tgtPos[1] - barrelPos[1];
                    tgtDir[2] = tgtPos[2] - barrelPos[2];
                    Vec3Normalize(tgtDir);
                    vectoangles(tgtDir, tgtAngles);
                    worldTargetAngles = 1;
                    if ( g_vehicleDebug->current.enabled )
                        CL_AddDebugLine(tgtPos, barrelPos, colorRed, 1, 1);
                }
                else
                {
                    tgtAngles[0] = prevAngles[0];
                    tgtAngles[1] = prevAngles[1];
                    tgtAngles[2] = prevAngles[2];
                    worldTargetAngles = 0;
                }
                if ( worldTargetAngles )
                {
                    angles[0] = veh->phys.angles[0];
                    angles[1] = veh->phys.angles[1];
                    angles[2] = veh->phys.angles[2];
                    AnglesToAxis(angles, mtx);
                    MatrixTranspose(mtx, invMtx);
                    AnglesToAxis(tgtAngles, axis);
                    MatrixMultiply(axis, invMtx, mtx);
                    AxisToAngles(mtx, tgtAngles);
                }
                AnglesSubtract(tgtAngles, prevAngles, deltaAngles);
                (deltaAngles[0]) = fabs(deltaAngles[0]);
                (deltaAngles[1]) = fabs(deltaAngles[1]);
                if ( player && player->client && !veh->gunnerTargets[gunnerIndex].valid )
                {
                    pitch = AngleNormalize180(tgtAngles[0]);
                    yaw = tgtAngles[1];
                }
                else
                {
                    pitch = LinearTrackAngle(tgtAngles[0], prevAngles[0], info->gunnerRotRate, 0.050000001);
                    yaw = LinearTrackAngle(tgtAngles[1], prevAngles[1], info->gunnerRotRate, 0.050000001);
                }
                stopAngles[0] = pitch;
                stopAngles[1] = yaw;
                stopAngles[2] = 0.0f;
                if ( gunnerIndex == info->driverControlledGunPos )
                {
                    pitch = AngleNormalize180(pitch - info->gunnerRestAngles[gunnerIndex][0]);
                    if ( (float)(pitch - weapDef->bottomArc) < 0.0 )
                        bottomArc = pitch;
                    else
                        bottomArc = weapDef->bottomArc;
                    if ( (float)((-(weapDef->topArc)) - pitch) < 0.0 )
                        clampedPitch = bottomArc;
                    else
                        clampedPitch = -weapDef->topArc;
                    pitch = AngleNormalize180(clampedPitch + info->gunnerRestAngles[gunnerIndex][0]);
                    yaw = AngleNormalize180(yaw - info->gunnerRestAngles[gunnerIndex][1]);
                    if ( (float)(yaw - weapDef->leftArc) < 0.0 )
                        leftArc = yaw;
                    else
                        leftArc = weapDef->leftArc;
                    if ( (float)((-(weapDef->rightArc)) - yaw) < 0.0 )
                        clampedYaw = leftArc;
                    else
                        clampedYaw = -weapDef->rightArc;
                    yaw = AngleNormalize180(clampedYaw + info->gunnerRestAngles[gunnerIndex][1]);
                }
                if ( player && player->client && gVehicleRelativeGunnerAngles && info->driverControlledGunPos != gunnerIndex )
                {
                    viewPitchQuantized = floor((float)(player->client->ps.viewangles[0] + info->gunnerRestAngles[gunnerIndex][0]) * 182.04445 + 0.5);
                    ent->s.lerp.u.vehicle.gunnerAngles[gunnerIndex].pitch = (int)viewPitchQuantized;
                    viewYawQuantized = floor((float)(player->client->ps.viewangles[1] + info->gunnerRestAngles[gunnerIndex][1]) * 182.04445 + 0.5);
                    ent->s.lerp.u.vehicle.gunnerAngles[gunnerIndex].yaw = (int)viewYawQuantized;
                }
                else
                {
                    gunnerPitchQuantized = floor(pitch * 182.04445 + 0.5);
                    ent->s.lerp.u.vehicle.gunnerAngles[gunnerIndex].pitch = (int)gunnerPitchQuantized;
                    gunnerYawQuantized = floor(yaw * 182.04445 + 0.5);
                    ent->s.lerp.u.vehicle.gunnerAngles[gunnerIndex].yaw = (int)gunnerYawQuantized;
                }
                stopAngles[0] = AngleNormalize180(stopAngles[0] - pitch);
                stopAngles[1] = AngleNormalize180(stopAngles[1] - yaw);
                if ( deltaAngles[0] >= 0.050000001 && stopAngles[0] == 0.0
                    || deltaAngles[1] >= 0.050000001 && stopAngles[1] == 0.0 )
                {
                    veh->gunnerTurrets[gunnerIndex].turretState = VEH_TURRET_MOVING;
                }
                else if ( veh->gunnerTurrets[gunnerIndex].turretState == VEH_TURRET_MOVING )
                {
                    veh->gunnerTurrets[gunnerIndex].turretState = VEH_TURRET_STOPPING;
                }
                else if ( veh->gunnerTurrets[gunnerIndex].turretState == VEH_TURRET_STOPPING )
                {
                    Scr_AddInt(gunnerIndex, SCRIPTINSTANCE_SERVER);
                    Scr_Notify(ent, scr_const.turret_rotate_stopped, 1u);
                    veh->gunnerTurrets[gunnerIndex].turretState = VEH_TURRET_STOPPED;
                }
                turretOnTargetRange = veh->gunnerTurrets[gunnerIndex].turretOnTargetRange;
                if ( turretOnTargetRange != 0.0 && turretOnTargetRange > deltaAngles[0] && turretOnTargetRange > deltaAngles[1] )
                {
                    Scr_AddInt(gunnerIndex, SCRIPTINSTANCE_SERVER);
                    Scr_Notify(ent, scr_const.gunner_turret_on_target, 1u);
                }
            }
        }
        else if ( veh->gunnerTurrets[gunnerIndex].turretState == VEH_TURRET_MOVING )
        {
            veh->gunnerTurrets[gunnerIndex].turretState = VEH_TURRET_STOPPING;
        }
        else if ( veh->gunnerTurrets[gunnerIndex].turretState == VEH_TURRET_STOPPING )
        {
            veh->gunnerTurrets[gunnerIndex].turretState = VEH_TURRET_STOPPED;
        }
    }
}

void __cdecl VEH_UpdateGunnerWeaponsAll(gentity_s *ent, int msec)
{
    int i; // [esp+0h] [ebp-4h]

    for ( i = 0; i < 4; ++i )
        VEH_UpdateGunnerWeapon(ent, i, msec);
}

void __cdecl VEH_UpdateGunnerWeapon(gentity_s *ent, int gunnerIndex, int msec)
{
    int WeaponIndexForName; // eax
    int frametime; // [esp+8h] [ebp-18h]
    const vehicle_info_t *info; // [esp+Ch] [ebp-14h]
    scr_vehicle_s *veh; // [esp+10h] [ebp-10h]
    const WeaponDef *weapDef; // [esp+14h] [ebp-Ch]
    gentity_s *player; // [esp+18h] [ebp-8h]
    unsigned int checkButton; // [esp+1Ch] [ebp-4h]

    veh = ent->scr_vehicle;
    info = BG_GetVehicleInfo(veh->infoIdx);
    checkButton = 0;
    if ( (float)ent->health > 0.0 && info->gunnerWeaponIndex[gunnerIndex] )
    {
        veh->gunnerTurrets[gunnerIndex].fireTime -= msec;
        if ( veh->gunnerTurrets[gunnerIndex].fireTime < 0 )
            veh->gunnerTurrets[gunnerIndex].fireTime = 0;
        player = VEH_GetSeatOccupantEntity(veh, gunnerIndex + 1);
        if ( !player && info->driverControlledGunPos == gunnerIndex )
        {
            checkButton = 35;
            player = VEH_GetSeatOccupantEntity(veh, 0);
        }
        weapDef = BG_GetWeaponDef(info->gunnerWeaponIndex[gunnerIndex]);
        if ( weapDef->overheatWeapon )
        {
            frametime = level.time - level.previousTime;
            if ( !player
                //|| !bitarray<51>::testBit(&player->client->button_bits, checkButton)
                || !player->client->button_bits.testBit(checkButton)
                || weapDef->coolWhileFiring
                || veh->gunnerTurrets[gunnerIndex].overheating == 1
                || player->client->ps.leanf > 0.0 )
            {
                veh->gunnerTurrets[gunnerIndex].heatVal = veh->gunnerTurrets[gunnerIndex].heatVal
                                                                                                - (float)((float)((float)frametime * weapDef->cooldownRate) / 1000.0);
                if ( veh->gunnerTurrets[gunnerIndex].heatVal < 0.0 )
                    veh->gunnerTurrets[gunnerIndex].heatVal = 0.0f;
                if ( weapDef->overheatEndVal >= veh->gunnerTurrets[gunnerIndex].heatVal )
                    veh->gunnerTurrets[gunnerIndex].overheating = 0;
            }
            if ( player )
            {
                //if ( bitarray<51>::testBit(&player->client->button_bits, checkButton) )
                if ( player->client->button_bits.testBit(checkButton) )
                {
                    if ( !veh->gunnerTurrets[gunnerIndex].overheating && player->client->ps.leanf == 0.0 )
                    {
                        veh->gunnerTurrets[gunnerIndex].heatVal = (float)((float)((float)frametime * weapDef->overheatRate) / 1000.0)
                                                                                                        + veh->gunnerTurrets[gunnerIndex].heatVal;
                        if ( veh->gunnerTurrets[gunnerIndex].heatVal >= 100.0 )
                        {
                            veh->gunnerTurrets[gunnerIndex].overheating = 1;
                            veh->gunnerTurrets[gunnerIndex].heatVal = 100.0f;
                        }
                    }
                }
            }
        }
        if ( player )
        {
            if ( !player->client
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                            3567,
                            0,
                            "%s",
                            "player->client") )
            {
                __debugbreak();
            }
            if ( (player->client->ps.eFlags & 0x10000) == 0 )
            {
                if ( !info->gunnerWeaponIndex[gunnerIndex] )
                {
                    WeaponIndexForName = G_GetWeaponIndexForName((char*)info->gunnerWeapon[gunnerIndex]);
                    AssignToSmallerType<unsigned short>(
                        &bg_vehicleInfos[veh->infoIdx].gunnerWeaponIndex[gunnerIndex],
                        WeaponIndexForName);
                    if ( !info->gunnerWeaponIndex[gunnerIndex] )
                        Com_Error(ERR_DROP, "Invalid gunner weapon specified for [%s] gunner [%d][%s]", info, gunnerIndex, info->gunnerWeapon[gunnerIndex]);
                }
                //if ( bitarray<51>::testBit(&player->client->button_bits, checkButton) )
                if ( player->client->button_bits.testBit(checkButton) )
                {
                    if ( !veh->gunnerTurrets[gunnerIndex].fireTime && !veh->gunnerTurrets[gunnerIndex].overheating )
                    {
                        VEH_FireGunnerWeapon(ent, gunnerIndex, player);
                        if ( (veh->gunnerTurrets[gunnerIndex].flags & 2) != 0 && (veh->gunnerTurrets[gunnerIndex].flags & 4) == 0 )
                            VEH_FireGunnerWeapon(ent, gunnerIndex, player);
                    }
                }
            }
        }
    }
}

gentity_s *__cdecl VEH_FireGunnerWeapon(gentity_s *ent, int gunnerIndex, gentity_s *attacker)
{
    int WeaponIndexForName;
    const char *errorMsg;
    char *flashTagName;
    float playerSpread;
    gentity_s *defaultAttacker;
    float vel[3]; // [esp+2Ch] [ebp-110h] BYREF
    gentity_s *target; // [esp+38h] [ebp-104h]
    float offset[3]; // [esp+3Ch] [ebp-100h] BYREF
    gentity_s *driver; // [esp+48h] [ebp-F4h]
    DObjTrace_s trace; // [esp+4Ch] [ebp-F0h] BYREF
    float spread; // [esp+70h] [ebp-CCh]
    float joltDir[3]; // [esp+74h] [ebp-C8h] BYREF
    const vehicle_info_t *info; // [esp+80h] [ebp-BCh]
    scr_vehicle_s *veh; // [esp+84h] [ebp-B8h]
    int event; // [esp+88h] [ebp-B4h]
    int flash; // [esp+8Ch] [ebp-B0h]
    gentity_s *proj; // [esp+90h] [ebp-ACh]
    weaponParms wp; // [esp+94h] [ebp-A8h] BYREF
    float barrelMtx[4][3]; // [esp+DCh] [ebp-60h] BYREF
    float flashMtx[4][3]; // [esp+10Ch] [ebp-30h] BYREF

    veh = ent->scr_vehicle;
    info = BG_GetVehicleInfo(veh->infoIdx);
    if ( !info->gunnerWeaponIndex[gunnerIndex] )
    {
        WeaponIndexForName = G_GetWeaponIndexForName((char*)info->gunnerWeapon[gunnerIndex]);
        AssignToSmallerType<unsigned short>(
            &bg_vehicleInfos[veh->infoIdx].gunnerWeaponIndex[gunnerIndex],
            WeaponIndexForName);
        if ( !info->gunnerWeaponIndex[gunnerIndex] )
            Com_Error(ERR_DROP, "Invalid gunner weapon specified for [%s] gunner [%d][%s]", info, gunnerIndex, info->gunnerWeapon[gunnerIndex]);
    }
    if ( (veh->gunnerTurrets[gunnerIndex].flags & 8) != 0 )
        return 0;
    proj = 0;
    Weapon_SetWeaponParamsWeapon(&wp, info->gunnerWeaponIndex[gunnerIndex]);
    if ( wp.weapDef->fireType != WEAPON_FIRETYPE_MINIGUN )
        goto LABEL_13;
    if ( !attacker || !attacker->client )
        return proj;
    if ( attacker->client->ps.weaponSpinLerp < 1.0 )
        return proj;
LABEL_13:
    if ( info->type != 3 && info->type != 6 )
    {
        if ( veh->boneIndex.gunnerTags[gunnerIndex].flash < 0 )
        {
            errorMsg = va("Missing tag_flash_gunner%i for [%s]\n", gunnerIndex + 1, info->name);
            Scr_Error(errorMsg, 0);
        }
        G_DObjGetWorldBoneIndexMatrix(ent, veh->boneIndex.gunnerTags[gunnerIndex].flash, barrelMtx);
        if ( veh->joltTime == 0.0 || veh->joltTime < 0.64999998 )
        {
            joltDir[0] = -barrelMtx[0][0];
            joltDir[1] = -barrelMtx[0][1];
            joltDir[2] = -barrelMtx[0][2];
            VEH_JoltBody(ent, joltDir, 0.2, 0.0, 0.0);
        }
    }
    if ( vehicle_selfCollision->current.enabled && (veh->gunnerTurrets[gunnerIndex].flags & 2) != 0 )
    {
        if ( veh->gunnerTurrets[gunnerIndex].fireBarrel )
            veh->gunnerTurrets[gunnerIndex].fireBarrel = 0;
        else
            veh->gunnerTurrets[gunnerIndex].fireBarrel = 1;
    }
    if ( veh->gunnerTurrets[gunnerIndex].fireBarrel )
    {
        event = gunnerIndex + 178;
        flash = veh->boneIndex.gunnerTags[gunnerIndex].flash2;
        if ( flash < 0 )
        {
            flashTagName = SL_ConvertToString(*s_gunnerFlashTags[gunnerIndex + 4], SCRIPTINSTANCE_SERVER);
            errorMsg = va("Missing %s for vehicle %s\n", flashTagName, info->name);
            Scr_Error(errorMsg, 0);
        }
    }
    else
    {
        event = gunnerIndex + 174;
        flash = veh->boneIndex.gunnerTags[gunnerIndex].flash;
        if ( flash < 0 )
        {
            flashTagName = SL_ConvertToString(*s_gunnerFlashTags[gunnerIndex], SCRIPTINSTANCE_SERVER);
            errorMsg = va("Missing %s for vehicle %s\n", flashTagName, info->name);
            Scr_Error(errorMsg, 0);
        }
    }
    G_DObjGetWorldBoneIndexMatrix(ent, flash, flashMtx);
    if ( !vehicle_selfCollision->current.enabled )
    {
        G_TraceBulletPathForVehTurret(ent, &trace, gunnerIndex);
        if ( trace.fraction != 1.0 )
            return 0;
    }
    //*(_QWORD *)wp.gunForward = *(_QWORD *)&flashMtx[0][0];
    wp.gunForward[0] = flashMtx[0][0];
    wp.gunForward[1] = flashMtx[0][1];
    wp.gunForward[2] = flashMtx[0][2];
    //*(_QWORD *)wp.forward = *(_QWORD *)&flashMtx[0][0];
    wp.forward[0] = flashMtx[0][0];
    wp.forward[1] = flashMtx[0][1];
    wp.forward[2] = flashMtx[0][2];
    //*(_QWORD *)wp.right = *(_QWORD *)&flashMtx[1][0];
    wp.right[0] = flashMtx[1][0];
    wp.right[1] = flashMtx[1][1];
    wp.right[2] = flashMtx[1][2];
    //*(_QWORD *)wp.up = *(_QWORD *)&flashMtx[2][0];
    wp.up[0] = flashMtx[2][0];
    wp.up[1] = flashMtx[2][1];
    wp.up[2] = flashMtx[2][2];
    //*(_QWORD *)wp.muzzleTrace = *(_QWORD *)&flashMtx[3][0];
    wp.muzzleTrace[0] = flashMtx[3][0];
    wp.muzzleTrace[1] = flashMtx[3][1];
    wp.muzzleTrace[2] = flashMtx[3][2];
    if ( !attacker )
    {
        attacker = VEH_GetSeatOccupantEntity(veh, gunnerIndex + 1);
        if ( !attacker && !G_IsVehicleSeatOccupied(ent, gunnerIndex + 1) )
        {
            driver = VEH_GetSeatOccupantEntity(veh, 0);
            if ( driver )
                defaultAttacker = driver;
            else
                defaultAttacker = ent;
            attacker = defaultAttacker;
        }
        if ( !attacker )
            attacker = ent;
    }
    if ( attacker->client )
    {
        if ( attacker->client->ps.vehiclePos )
        {
            G_GetPlayerViewOrigin(&attacker->client->ps, wp.muzzleTrace);
            if ( attacker->client->ps.vehiclePos >= 1
                && attacker->client->ps.vehiclePos <= 4
                && attacker->client->ps.leanf > 0.0 )
            {
                return 0;
            }
        }
    }
    if ( attacker && attacker->client )
        playerSpread = wp.weapDef->playerSpread;
    else
        playerSpread = wp.weapDef->aiSpread;
    spread = playerSpread;
    if ( wp.weapDef->weapType )
    {
        switch ( wp.weapDef->weapType )
        {
            case WEAPTYPE_PROJECTILE:
                if ( info->type == 6 )
                {
                    memset(vel, 0, sizeof(vel));
                }
                else
                {
                    vel[0] = veh->phys.vel[0];
                    vel[1] = veh->phys.vel[1];
                    vel[2] = veh->phys.vel[2];
                }
                if ( attacker && attacker->client && (attacker->client->ps.weapLockFlags & 2) != 0 )
                {
                    target = &level.gentities[attacker->client->ps.weapLockedEntnum];
                    G_TargetGetOffset(target, offset);
                }
                else
                {
                    target = 0;
                    memset(offset, 0, sizeof(offset));
                }
                proj = Weapon_RocketLauncher_Fire(
                                 attacker,
                                 info->gunnerWeaponIndex[gunnerIndex],
                                 spread,
                                 &wp,
                                 vel,
                                 target,
                                 offset);
                break;
            case WEAPTYPE_GAS:
                if ( attacker->client )
                    attacker->client->ps.eFlags |= 0x40u;
                veh->flags |= 0x40u;
                ent->s.lerp.eFlags |= 0x40u;
                Weapon_Flamethrower_Fire(attacker, &wp);
                if ( !attacker
                    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 3483, 0, "%s", "attacker") )
                {
                    __debugbreak();
                }
                break;
            case WEAPTYPE_GRENADE:
                proj = Weapon_GrenadeLauncher_Fire(attacker, info->gunnerWeaponIndex[gunnerIndex], 0, &wp);
                break;
        }
    }
    else
    {
        Bullet_Fire(attacker, spread, &wp, ent, veh->gunnerTurrets[gunnerIndex].fireBarrel + level.time);
    }
    G_AddEvent(ent, event, attacker->s.number);
    veh->gunnerTurrets[gunnerIndex].fireTime = wp.weapDef->iFireTime;
    return proj;
}

void __cdecl VEH_UpdateBody(gentity_s *ent)
{
    float joltSin;
    float throttleQuantized;
    scr_vehicle_s *veh;
    float intensity;

    veh = ent->scr_vehicle;
    if ( !veh->nitrousVehicle && veh->joltTime > 0.0 )
    {
        joltSin = sin(veh->joltWave * 0.017453292f);
        intensity = (veh->joltTime / 1.3f) * joltSin;
        throttleQuantized = floor(intensity * veh->joltDir[0] * 182.04445f + 0.5f);
        ent->s.lerp.u.vehicle.throttle = (int)throttleQuantized;
        ent->s.lerp.u.turret.gunAngles[1] = intensity * veh->joltDir[1];
        veh->joltTime = veh->joltTime - 0.050000001;
        veh->joltWave = veh->joltWave + 36.0;
    }
}

void __cdecl VEH_UpdateSteering(gentity_s *ent)
{
    float wheelAngle;
    float steerScale;
    vehicle_physic_t *phys;
    scr_vehicle_s *veh;
    float forward[3];
    bool isReverse;
    float deltaYaw;

    veh = ent->scr_vehicle;
    if (BG_GetVehicleInfo(veh->infoIdx)->steerWheels)
    {
        isReverse = 0;
        phys = &veh->phys;
        if ((float)((float)((float)(phys->vel[0] * phys->vel[0]) + (float)(phys->vel[1] * phys->vel[1]))
            + (float)(phys->vel[2] * phys->vel[2])) != 0.0)
        {
            AngleVectors(veh->phys.angles, forward, 0, 0);
            isReverse = (float)((float)((float)(forward[0] * veh->phys.vel[0]) + (float)(forward[1] * veh->phys.vel[1]))
                + (float)(forward[2] * veh->phys.vel[2])) < 0.0;
        }
        deltaYaw = AngleNormalize180(veh->phys.angles[1] - veh->phys.prevAngles[1]);
        steerScale = isReverse ? -10.0f : 10.0f;
        deltaYaw = deltaYaw * steerScale;
        if ( deltaYaw > 45.0f )
            wheelAngle = 45.0f;
        else if ( deltaYaw < -45.0f )
            wheelAngle = -45.0f;
        else
            wheelAngle = deltaYaw;
        ent->s.lerp.u.turret.gunAngles[0] = wheelAngle;
    }
    else
    {
        ent->s.lerp.u.actor.actorNum = 0;// *(_DWORD *)&FLOAT_0_0;
    }
}

void __cdecl VEH_UpdateDriverWeapons(gentity_s *ent)
{
    char *flashTagName;
    float *targetOffset;
    float *gunnerTargetOrigin;
    float *driverTargetOrigin;
    float *gasTargetOrigin;
    float *targetOrigin; // [esp+20h] [ebp-104h]
    int hitnum; // [esp+28h] [ebp-FCh] BYREF
    float angles[3]; // [esp+54h] [ebp-D0h] BYREF
    const WeaponDef *gunnerWeapDef; // [esp+60h] [ebp-C4h]
    int driverControlledGunPos; // [esp+64h] [ebp-C0h]
    int gunnerIndex; // [esp+68h] [ebp-BCh]
    bool wasAlreadyInReloadTime; // [esp+6Eh] [ebp-B6h]
    bool nowInReloadTime; // [esp+6Fh] [ebp-B5h]
    gclient_s *client; // [esp+70h] [ebp-B4h]
    const vehicle_info_t *info; // [esp+74h] [ebp-B0h]
    scr_vehicle_s *veh; // [esp+78h] [ebp-ACh]
    float flashPos[3]; // [esp+7Ch] [ebp-A8h] BYREF
    float right[3]; // [esp+88h] [ebp-9Ch] BYREF
    float start[3]; // [esp+94h] [ebp-90h] BYREF
    float end[3]; // [esp+A0h] [ebp-84h] BYREF
    float forward[3]; // [esp+ACh] [ebp-78h] BYREF
    trace_t trace; // [esp+B8h] [ebp-6Ch] BYREF
    const WeaponVariantDef *weapVariantDef; // [esp+F4h] [ebp-30h]
    float offsetVec[3]; // [esp+F8h] [ebp-2Ch] BYREF
    float up[3]; // [esp+104h] [ebp-20h] BYREF
    float barrelPos[3]; // [esp+110h] [ebp-14h] BYREF
    const WeaponDef *weapDef; // [esp+11Ch] [ebp-8h]
    gentity_s *player; // [esp+120h] [ebp-4h]

    veh = ent->scr_vehicle;
    player = 0;
    client = 0;
    info = BG_GetVehicleInfo(veh->infoIdx);
    memset(&trace, 0, sizeof(trace));
    weapVariantDef = 0;
    weapDef = 0;
    VEH_UpdateDriverActions(ent);
    if ( ent->s.weapon )
    {
        weapVariantDef = BG_GetWeaponVariantDef(ent->s.weapon);
        weapDef = BG_GetWeaponDef(ent->s.weapon);
        if ( veh->turret.fireTime > 0 )
        {
            wasAlreadyInReloadTime = veh->turret.fireTime <= weapVariantDef->iReloadTime;
            veh->turret.fireTime -= 50;
            nowInReloadTime = veh->turret.fireTime <= weapVariantDef->iReloadTime;
            if ( !wasAlreadyInReloadTime && nowInReloadTime )
                G_AddEvent(ent, 0x14u, 0);
            if ( veh->turret.fireTime <= 0 )
                G_AddEvent(ent, 0x15u, 0);
        }
    }
    //if ( EntHandle::isDefined(&ent->r.ownerNum) )
    if ( ent->r.ownerNum.isDefined() )
    {
        //player = EntHandle::ent(&ent->r.ownerNum);
        player = ent->r.ownerNum.ent();
        client = player->client;
        if ( !client
            && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 3820, 0, "%s", "client") )
        {
            __debugbreak();
        }
        if ( (player->client->ps.eFlags & 0x10000) == 0 && !player->client->ps.vehiclePos )
        {
            if ( veh->turret.fireTime <= 0
                && ent->s.weapon
                //&& bitarray<51>::testBit(&client->sess.cmd.button_bits, 0x22u)
                && client->sess.cmd.button_bits.testBit(0x22u)
                && (client->ps.pm_flags & 0xC00) == 0 )
            {
                Scr_AddEntity(player, SCRIPTINSTANCE_SERVER);
                Scr_Notify(ent, scr_const.turret_fire, 1u);
            }
            if ( ent->s.weapon )
                veh->hasTarget = 1;
            G_GetPlayerViewOrigin(&client->ps, start);
            if ( vehHelicopterHeadSwayDontSwayTheTurret->current.enabled && info->type == 6 )
                AngleVectors(ent->r.currentAngles, forward, right, up);
            else
                G_GetPlayerViewDirection(player, forward, right, up);
            memset(offsetVec, 0, sizeof(offsetVec));
            if ( !weapDef && info->driverControlledGunPos >= 0 )
            {
                gunnerIndex = info->driverControlledGunPos;
                weapDef = BG_GetWeaponDef(info->gunnerWeaponIndex[gunnerIndex]);
                if ( weapDef )
                    BG_CalcVehicleTurretWeaponPosOffset(player->client->ps.fWeaponPosFrac, weapDef, offsetVec);
            }
            if ( info->type != 6 )
            {
                start[0] = (float)((float)((-(offsetVec[0])) + 300.0) * forward[0]) + start[0];
                start[1] = (float)((float)((-(offsetVec[0])) + 300.0) * forward[1]) + start[1];
                start[2] = (float)((float)((-(offsetVec[0])) + 300.0) * forward[2]) + start[2];
            }
            end[0] = (float)(10240.0 * forward[0]) + start[0];
            end[1] = (float)(10240.0 * forward[1]) + start[1];
            end[2] = (float)(10240.0 * forward[2]) + start[2];
            targetOrigin = veh->targetOrigin;
            veh->targetOrigin[0] = end[0];
            targetOrigin[1] = end[1];
            targetOrigin[2] = end[2];
            G_LocationalTrace(&trace, start, end, ent->r.ownerNum.entnum(), 0x280E893, 0, 0);
            if ( trace.fraction < 1.0 )
                Vec3Lerp(start, end, trace.fraction, veh->targetOrigin);
            if ( info->driverControlledGunPos >= 0 )
            {
                driverControlledGunPos = info->driverControlledGunPos;
                if ( veh->gunnerTargets[driverControlledGunPos].targetEnt == 1023 )
                {
                    gunnerWeapDef = BG_GetWeaponDef(info->gunnerWeaponIndex[driverControlledGunPos]);
                    if ( gunnerWeapDef->weapType == WEAPTYPE_GAS )
                    {
                        angles[0] = client->ps.viewangles[0];
                        angles[1] = client->ps.viewangles[1];
                        angles[2] = client->ps.viewangles[2];
                        angles[0] = angles[0] - 6.0;
                        AngleVectors(angles, forward, 0, 0);
                        gasTargetOrigin = veh->gunnerTargets[driverControlledGunPos].targetOrigin;
                        gasTargetOrigin[0] = 5000.0f * forward[0] + start[0];
                        gasTargetOrigin[1] = 5000.0f * forward[1] + start[1];
                        gasTargetOrigin[2] = 5000.0f * forward[2] + start[2];
                    }
                    else
                    {
                        gunnerTargetOrigin = veh->gunnerTargets[driverControlledGunPos].targetOrigin;
                        driverTargetOrigin = veh->targetOrigin;
                        gunnerTargetOrigin[0] = driverTargetOrigin[0];
                        gunnerTargetOrigin[1] = driverTargetOrigin[1];
                        gunnerTargetOrigin[2] = driverTargetOrigin[2];
                    }
                    targetOffset = veh->gunnerTargets[driverControlledGunPos].targetOffset;
                    *targetOffset = 0.0f;
                    targetOffset[1] = 0.0f;
                    targetOffset[2] = 0.0f;
                    veh->gunnerTargets[driverControlledGunPos].valid = 1;
                }
            }
            if ( ent->s.weapon && info->type == 2 )
            {
                if ( veh->boneIndex.flash[0] < 0 )
                {
                    flashTagName = SL_ConvertToString(*s_flashTags[0], SCRIPTINSTANCE_SERVER);
                    Com_Error(ERR_DROP, "Player vehicle has no %s", flashTagName);
                }
                if ( veh->boneIndex.barrel < 0 && info->type != 3 )
                    Com_Error(ERR_DROP, "Player vehicle has no tag_barrel");
                G_DObjGetWorldBoneIndexPos(ent, veh->boneIndex.flash[0], flashPos);
                if ( info->type != 3 )
                    G_DObjGetWorldBoneIndexPos(ent, veh->boneIndex.barrel, barrelPos);
                hitnum = -1;

                col_context_t context(ent->clipmask); // [esp+2Ch] [ebp-F8h] BYREF
                context.passEntityNum0 = ent->s.number;
                if ( info->type != 3 && !SV_SightTracePoint(&hitnum, barrelPos, flashPos, &context) )
                    veh->turret.flags |= 1u;
            }
        }
    }
}

void __cdecl VEH_UpdateDriverActions(gentity_s *ent)
{
    scr_vehicle_s *veh; // [esp+0h] [ebp-8h]
    gentity_s *player; // [esp+4h] [ebp-4h]

    //if ( EntHandle::isDefined(&ent->r.ownerNum) )
    if ( ent->r.ownerNum.isDefined() )
    {
        //player = EntHandle::ent(&ent->r.ownerNum);
        player = ent->r.ownerNum.ent();
        if ( player )
        {
            if ( player->client )
            {
                veh = ent->scr_vehicle;
                if ( veh )
                {
                    //if ( bitarray<51>::testBit(&player->client->button_bits, 0x27u) )
                    if ( player->client->button_bits.testBit(0x27u) )
                    {
                        Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
                        Scr_Notify(ent, scr_const.veh_usespecialability, 1u);
                        veh->m_bSpecialAbilityEventDown = 1;
                    }
                    else if ( veh->m_bSpecialAbilityEventDown )
                    {
                        Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
                        Scr_Notify(ent, scr_const.veh_usespecialabilityend, 1u);
                        veh->m_bSpecialAbilityEventDown = 0;
                    }
                    if ( player->client->button_bits.testBit(0x28u) )
                    {
                        Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
                        Scr_Notify(ent, scr_const.veh_firepickup, 1u);
                        veh->m_bFirePickupEventDown = 1;
                    }
                    else if ( veh->m_bFirePickupEventDown )
                    {
                        Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
                        Scr_Notify(ent, scr_const.veh_firepickupend, 1u);
                        veh->m_bFirePickupEventDown = 0;
                    }
                    if ( player->client->button_bits.testBit(0x29u) )
                    {
                        Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
                        Scr_Notify(ent, scr_const.veh_swappickup, 1u);
                        veh->m_bSwapPickupEventDown = 1;
                    }
                    else if ( veh->m_bSwapPickupEventDown )
                    {
                        Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
                        Scr_Notify(ent, scr_const.veh_swappickupend, 1u);
                        veh->m_bSwapPickupEventDown = 0;
                    }
                    if ( player->client->button_bits.testBit(0x2Au) )
                    {
                        Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
                        Scr_Notify(ent, scr_const.veh_dropdeployable, 1u);
                        veh->m_bDropDeployableEventDown = 1;
                    }
                    else if ( veh->m_bDropDeployableEventDown )
                    {
                        Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
                        Scr_Notify(ent, scr_const.veh_dropdeployableend, 1u);
                        veh->m_bDropDeployableEventDown = 0;
                    }
                    if ( player->client->button_bits.testBit(0x17u) )
                    {
                        Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
                        Scr_Notify(ent, scr_const.veh_handbreak, 1u);
                    }
                    if ( player->client->button_bits.testBit(1u) )
                    {
                        Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
                        Scr_Notify(ent, scr_const.veh_boost, 1u);
                    }
                    else if ( ent->scr_vehicle
                                 && ent->scr_vehicle->nitrousVehicle
                                 && ent->scr_vehicle->nitrousVehicle->m_vehicle_info->boostDuration > ent->scr_vehicle->nitrousVehicle->m_boost_time_pool )
                    {
                        Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
                        Scr_Notify(ent, scr_const.veh_boost_regen, 1u);
                    }
                }
            }
        }
    }
}

void __cdecl VEH_UpdateClient(gentity_s *ent)
{
    gentity_s *player; // [esp+28h] [ebp-68h]
    const vehicle_info_t *info; // [esp+30h] [ebp-60h]
    scr_vehicle_s *veh; // [esp+34h] [ebp-5Ch]
    char move[4]; // [esp+38h] [ebp-58h] BYREF
    float rotAccel[3]; // [esp+3Ch] [ebp-54h] BYREF
    float bodyAccel[3]; // [esp+48h] [ebp-48h] BYREF
    float worldAccel[3]; // [esp+54h] [ebp-3Ch] BYREF
    float axis[4][3]; // [esp+60h] [ebp-30h] BYREF

    move[0] = 0;
    move[1] = 0;
    move[2] = 0;
    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 4060, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->scr_vehicle
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    4061,
                    0,
                    "%s",
                    "ent->scr_vehicle") )
    {
        __debugbreak();
    }
    veh = ent->scr_vehicle;
    info = BG_GetVehicleInfo(veh->infoIdx);
    if ( !veh->nitrousVehicle )
    {
        player = VEH_GetSeatOccupantEntity(veh, 0);
        if ( player )
        {
            if ( !player->client
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                            4073,
                            0,
                            "%s",
                            "player->client") )
            {
                __debugbreak();
            }
            if ( (player->client->ps.pm_flags & 0xC00) == 0 )
            {
                move[0] = player->client->sess.cmd.forwardmove;
                move[1] = player->client->sess.cmd.rightmove;
                move[2] = player->client->sess.cmd.button_bits.testBit(0xBu) ? 0x7F : 0;
            }
        }
        VEH_CalcAccel(ent, move, bodyAccel, rotAccel);
        veh->phys.rotVel[0] = (float)(0.050000001 * rotAccel[0]) + veh->phys.rotVel[0];
        veh->phys.rotVel[1] = (float)(0.050000001 * rotAccel[1]) + veh->phys.rotVel[1];
        veh->phys.rotVel[2] = (float)(0.050000001 * rotAccel[2]) + veh->phys.rotVel[2];
        veh->phys.angles[1] = AngleNormalize180((float)(veh->phys.rotVel[1] * 0.050000001) + veh->phys.prevAngles[1]);
        veh->phys.angles[0] = 0.0f;
        veh->phys.angles[2] = 0.0f;
        AnglesToAxis(veh->phys.angles, axis);
        memset(axis[3], 0, sizeof(float[3]));
        MatrixTransformVector(bodyAccel, axis, worldAccel);
        veh->phys.vel[0] = (float)(0.050000001 * worldAccel[0]) + veh->phys.vel[0];
        veh->phys.vel[1] = (float)(0.050000001 * worldAccel[1]) + veh->phys.vel[1];
        veh->phys.vel[2] = (float)(0.050000001 * worldAccel[2]) + veh->phys.vel[2];
        if ( veh->phys.vel[0] != 0.0 || veh->phys.vel[1] != 0.0 || veh->phys.vel[2] != 0.0 )
        {
            VEH_GroundTrace(ent);
            if ( s_phys.onGround )
                VEH_GroundMove(ent);
            else
                VEH_AirMove(ent, 1);
        }
        if ( !info->type || info->type == 2 )
            VEH_GroundPlant(ent, &veh->phys, 1);
        MatrixTransposeTransformVector43(veh->phys.vel, axis, veh->phys.bodyVel);
        veh->speed = fabs(veh->phys.bodyVel[0]);
        if ( veh->speed < 0.0
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                        4136,
                        0,
                        "%s",
                        "veh->speed >= 0.0f") )
        {
            __debugbreak();
        }
    }
    if ( VEH_ShouldEjectOccupants(ent) )
    {
        Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
        Scr_Notify(ent, scr_const.veh_ejectoccupants, 1u);
    }
}

void __cdecl VEH_GroundTrace(gentity_s *ent)
{
    col_context_t context; // [esp+Ch] [ebp-84h] BYREF
    vehicle_physic_t *phys; // [esp+34h] [ebp-5Ch]
    scr_vehicle_s *veh; // [esp+38h] [ebp-58h]
    float start[3]; // [esp+3Ch] [ebp-54h] BYREF
    trace_t trace; // [esp+48h] [ebp-48h] BYREF
    float point[3]; // [esp+84h] [ebp-Ch] BYREF

    veh = ent->scr_vehicle;
    phys = &veh->phys;
    memset(&trace, 0, sizeof(trace));
    //col_context_t::col_context_t(&context);
    start[0] = phys->origin[0];
    start[1] = phys->origin[1];
    start[2] = phys->origin[2] + 0.25;
    point[0] = phys->origin[0];
    point[1] = phys->origin[1];
    point[2] = phys->origin[2] - 0.25;
    G_TraceCapsule(&trace, start, phys->mins, phys->maxs, point, ent->s.number, ent->clipmask, &context);
    memcpy(&s_phys, &trace, 0x38u);
    s_phys.hasGround = 0;
    s_phys.onGround = 0;
    if ( (!trace.allsolid || VEH_CorrectAllSolid(ent, &trace))
        && trace.fraction != 1.0
        && (phys->vel[2] <= 0.0
         || (float)((float)((float)(phys->vel[0] * trace.normal.vec.v[0]) + (float)(phys->vel[1] * trace.normal.vec.v[1]))
                            + (float)(phys->vel[2] * trace.normal.vec.v[2])) <= 10.0) )
    {
        s_phys.hasGround = 1;
        if ( trace.normal.vec.v[2] >= 0.69999999 )
            s_phys.onGround = 1;
    }
}

void __cdecl VEH_GroundMove(gentity_s *ent)
{
    float vel; // [esp+24h] [ebp-14h]
    scr_vehicle_s *veh; // [esp+28h] [ebp-10h]
    float oldvel; // [esp+2Ch] [ebp-Ch]
    float oldvel_4; // [esp+30h] [ebp-8h]
    float oldvel_8; // [esp+34h] [ebp-4h]

    veh = ent->scr_vehicle;
    vel = Abs(veh->phys.vel);
    oldvel = veh->phys.vel[0];
    oldvel_4 = veh->phys.vel[1];
    oldvel_8 = veh->phys.vel[2];
    VEH_ClipVelocity(veh->phys.vel, s_phys.groundTrace.normal.vec.v, veh->phys.vel);
    if ( (float)((float)((float)(veh->phys.vel[0] * oldvel) + (float)(veh->phys.vel[1] * oldvel_4))
                         + (float)(veh->phys.vel[2] * oldvel_8)) > 0.0 )
    {
        Vec3Normalize(veh->phys.vel);
        veh->phys.vel[0] = vel * veh->phys.vel[0];
        veh->phys.vel[1] = vel * veh->phys.vel[1];
        veh->phys.vel[2] = vel * veh->phys.vel[2];
    }
    if ( veh->phys.vel[0] != 0.0 || veh->phys.vel[1] != 0.0 )
        VEH_StepSlideMove(ent, 0);
}

static float avoidRadius = 200.0f;
void __cdecl VEH_UpdateAvoidance(gentity_s *ent)
{
    scr_vehicle_s *veh;
    float *lookPos;
    float pushScale;
    float howClose;
    float otherEntPos[3];
    int i;
    float goalPos[3];
    scr_vehicle_s *otherVeh;
    float closestPt[3];
    gentity_s *otherEnt;

    if ( (ent->scr_vehicle->flags & 0x102) != 0 && (ent->scr_vehicle->flags & 0x400) != 0 )
    {
        veh = ent->scr_vehicle;
        if ( (veh->flags & 0x100) != 0 )
        {
            goalPos[0] = veh->pathPos.lookPos[0];
            goalPos[1] = veh->pathPos.lookPos[1];
            goalPos[2] = veh->pathPos.lookPos[2];
        }
        else
        {
            goalPos[0] = veh->goalPosition[0];
            goalPos[1] = veh->goalPosition[1];
            goalPos[2] = veh->goalPosition[2];
        }
        goalPos[2] = ent->r.currentOrigin[2];
        for ( i = 0; i < MAX_VEHICLES; ++i )
        {
            otherVeh = &s_vehicles[i];
            if ( otherVeh->entNum != 1023 && otherVeh->entNum != ent->s.number )
            {
                otherEnt = &g_entities[otherVeh->entNum];
                if ( ent->r.absmin[2] <= otherEnt->r.absmax[2] && otherEnt->r.absmin[2] <= ent->r.absmax[2] )
                {
                    otherEntPos[0] = otherEnt->r.currentOrigin[0];
                    otherEntPos[1] = otherEnt->r.currentOrigin[1];
                    otherEntPos[2] = goalPos[2];
                    GetClosestPointOnSegment(goalPos, ent->r.currentOrigin, otherEnt->r.currentOrigin, closestPt);
                    howClose = (float)((float)((float)(closestPt[0] - otherEntPos[0]) * (float)(closestPt[0] - otherEntPos[0]))
                                                     + (float)((float)(closestPt[1] - otherEntPos[1]) * (float)(closestPt[1] - otherEntPos[1])))
                                     + (float)((float)(closestPt[2] - otherEntPos[2]) * (float)(closestPt[2] - otherEntPos[2]));
                    if ( (float)(avoidRadius * avoidRadius) > howClose )
                    {
                        pushScale = 1.0 / sqrtf(howClose);
                        goalPos[0] = (float)(avoidRadius * (float)(pushScale * (float)(closestPt[0] - otherEntPos[0])))
                                             + otherEnt->r.currentOrigin[0];
                        goalPos[1] = (float)(avoidRadius * (float)(pushScale * (float)(closestPt[1] - otherEntPos[1])))
                                             + otherEnt->r.currentOrigin[1];
                        goalPos[2] = (float)(avoidRadius * (float)(pushScale * (float)(closestPt[2] - otherEntPos[2])))
                                             + otherEnt->r.currentOrigin[2];
                        if ( g_vehicleDebug->current.enabled )
                            G_DebugLine(goalPos, otherEnt->r.currentOrigin, colorWhite, 0);
                    }
                }
            }
        }
        if ( (ent->scr_vehicle->flags & 0x100) != 0 )
        {
            lookPos = ent->scr_vehicle->pathPos.lookPos;
            *lookPos = goalPos[0];
            lookPos[1] = goalPos[1];
            lookPos[2] = goalPos[2];
        }
        else if ( ent->scr_vehicle->nitrousVehicle && (ent->scr_vehicle->flags & 2) != 0 )
        {
            //NitrousVehicle::update_script_target(ent->scr_vehicle->nitrousVehicle, goalPos);
            ent->scr_vehicle->nitrousVehicle->update_script_target(goalPos);
        }
    }
}

void __cdecl GetClosestPointOnSegment(float *pt1, float *pt2, float *testPt, float *out)
{
    float fraction; // [esp+Ch] [ebp-14h]
    float segmentLengthSq; // [esp+10h] [ebp-10h]
    float line; // [esp+14h] [ebp-Ch]
    float line_4; // [esp+18h] [ebp-8h]
    float line_8; // [esp+1Ch] [ebp-4h]

    line = *pt2 - *pt1;
    line_4 = pt2[1] - pt1[1];
    line_8 = pt2[2] - pt1[2];
    segmentLengthSq = (float)((float)(line * line) + (float)(line_4 * line_4)) + (float)(line_8 * line_8);
    if ( segmentLengthSq == 0.0
        || (fraction = (float)((float)((float)(line * (float)(*testPt - *pt1))
                                                                 + (float)(line_4 * (float)(testPt[1] - pt1[1])))
                                                 + (float)(line_8 * (float)(testPt[2] - pt1[2])))
                                 / segmentLengthSq,
                fraction < 0.0) )
    {
        *out = *pt1;
        out[1] = pt1[1];
        out[2] = pt1[2];
    }
    else if ( fraction <= 1.0 )
    {
        *out = (float)(fraction * line) + *pt1;
        out[1] = (float)(fraction * line_4) + pt1[1];
        out[2] = (float)(fraction * line_8) + pt1[2];
    }
    else
    {
        *out = *pt2;
        out[1] = pt2[1];
        out[2] = pt2[2];
    }
}

// Decomp: CoDMPServer.c:433200 (VEH_UpdatePath)
void __cdecl VEH_UpdatePath(gentity_s *ent)
{
    float speed;
    double newSpeed;
    double pathPitch;
    double pathYaw;
    double pathRoll;
    double trackedPitch;
    double trackedYaw;
    double trackedRoll;
    phys_vec3 goalPosition;
    float goalSpeed;
    int endNodeNotifyIdx;
    float *rotVelScratch;
    float *rotVel;
    float *bodyVel;
    float *velScratch;
    float *vel;
    float *prevOrigin;
    float *velOut;
    float pathRollBase;
    float nextPathRoll;
    float manualTimeFrac;
    float pathYawBase;
    float nextPathYaw;
    float pathPitchBase;
    float nextPathPitch;
    int pathNodeNotifyIdx;
    int lastNode;
    float accel;
    float tgtSpeed;
    float manualTargetSpeed;
    bool waitNodeHit;
    float prevSpeed;
    vehicle_pathpos_t nextVpp;
    const vehicle_info_t *info;
    vehicle_physic_t *phys;
    scr_vehicle_s *veh;
    if (!ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 4262, 0, "%s", "ent"))
        __debugbreak();
    if (!ent->scr_vehicle
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
            4263,
            0,
            "%s",
            "ent->scr_vehicle"))
    {
        __debugbreak();
    }
    veh = ent->scr_vehicle;
    phys = &veh->phys;
    info = BG_GetVehicleInfo(veh->infoIdx);
    memcpy(&nextVpp, veh, sizeof(nextVpp));
    prevSpeed = veh->speed;
    waitNodeHit = 0;
    if (nextVpp.nodeIdx >= 0)
    {
        if (veh->pathPos.speed < 0.0
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                4276,
                0,
                "%s",
                "veh->pathPos.speed >= 0.0f"))
        {
            __debugbreak();
        }
        if (veh->manualSpeed < 0.0
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                4277,
                0,
                "%s",
                "veh->manualSpeed >= 0.0f"))
        {
            __debugbreak();
        }
        if (veh->manualMode)
        {
            if (veh->manualMode == 2)
                speed = veh->pathPos.speed;
            else
                speed = veh->manualSpeed;
            manualTargetSpeed = speed;
            tgtSpeed = speed;
            newSpeed = VEH_AccelerateSpeed(veh->speed, speed, veh->manualAccel, 0.050000001f);
            veh->speed = (float)newSpeed;
            if (veh->manualMode == 2 && veh->speed == tgtSpeed)
                veh->manualMode = 0;
        }
        else if ((veh->flags & 0x100) != 0)
        {
            VEH_GetNewSpeedAndAccel(veh, 0.050000001, 0, 1.0, &veh->speed, &accel);
        }
        else
        {
            veh->speed = veh->pathPos.speed;
        }
        if (veh->pathPos.speed <= 0.0)
            veh->manualTime = 0.0f;
        else
            veh->manualTime = (float)(veh->speed / veh->pathPos.speed) + veh->manualTime;
        lastNode = nextVpp.nodeIdx;
        while (veh->manualTime >= 1.0)
        {
            if (G_VehUpdatePathPos(&nextVpp, veh->waitNode))
                waitNodeHit = 1;
            veh->manualTime = veh->manualTime - 1.0;
            if (lastNode != nextVpp.nodeIdx)
            {
                LOWORD(pathNodeNotifyIdx) = nextVpp.nodeIdx;
                if (nextVpp.customPath)
                    LOWORD(pathNodeNotifyIdx) = nextVpp.customPath->pathOrder[nextVpp.nodeIdx];
                Scr_AddEntityNum(SLOWORD(pathNodeNotifyIdx), 3u, SCRIPTINSTANCE_SERVER, 0);
                Scr_Notify(ent, scr_const.reached_node, 1u);
            }
        }
        memcpy(veh, &nextVpp, 0xDCu);
        if (veh->manualTime > 0.0)
        {
            if (veh->manualTime >= 1.0
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    4328,
                    0,
                    "%s",
                    "veh->manualTime < 1.0f"))
            {
                __debugbreak();
            }
            G_VehUpdatePathPos(&nextVpp, -1);
        }
        if (*(_BYTE *)g_vehicleDrawPath->current.integer && *(_BYTE *)g_vehicleDrawPath->current.integer == 49)
            VP_DrawPath(&nextVpp);
        if (veh->pathPos.endOfPath && (veh->flags & 0x100) == 0)
            veh->speed = 0.0f;
        phys->origin[0] = (float)((float)(nextVpp.origin[0] - veh->pathPos.origin[0]) * veh->manualTime)
            + veh->pathPos.origin[0];
        phys->origin[1] = (float)((float)(nextVpp.origin[1] - veh->pathPos.origin[1]) * veh->manualTime)
            + veh->pathPos.origin[1];
        phys->origin[2] = (float)((float)(nextVpp.origin[2] - veh->pathPos.origin[2]) * veh->manualTime)
            + veh->pathPos.origin[2];
        pathPitchBase = veh->pathPos.angles[0];
        nextPathPitch = nextVpp.angles[0];
        manualTimeFrac = veh->manualTime;
        pathPitch = AngleNormalize180(nextVpp.angles[0] - pathPitchBase);
        phys->angles[0] = (float)(pathPitch * manualTimeFrac) + pathPitchBase;
        pathYawBase = veh->pathPos.angles[1];
        nextPathYaw = nextVpp.angles[1];
        pathYaw = AngleNormalize180(nextVpp.angles[1] - pathYawBase);
        phys->angles[1] = (float)(pathYaw * veh->manualTime) + pathYawBase;
        pathRollBase = veh->pathPos.angles[2];
        nextPathRoll = nextVpp.angles[2];
        pathRoll = AngleNormalize180(nextVpp.angles[2] - pathRollBase);
        phys->angles[2] = (float)(pathRoll * veh->manualTime) + pathRollBase;
        trackedPitch = DiffTrackAngle(phys->angles[0], phys->prevAngles[0], 6.0, 0.050000001);
        phys->angles[0] = (float)trackedPitch;
        trackedYaw = DiffTrackAngle(phys->angles[1], phys->prevAngles[1], 4.0, 0.050000001);
        phys->angles[1] = (float)trackedYaw;
        trackedRoll = DiffTrackAngle(phys->angles[2], phys->prevAngles[2], 6.0, 0.050000001);
        phys->angles[2] = (float)trackedRoll;
        if ((veh->flags & 0x100) != 0)
        {
            veh->pathPos.lookPos[0] = (float)((float)(nextVpp.lookPos[0] - veh->pathPos.lookPos[0]) * veh->manualTime)
                + veh->pathPos.lookPos[0];
            veh->pathPos.lookPos[1] = (float)((float)(nextVpp.lookPos[1] - veh->pathPos.lookPos[1]) * veh->manualTime)
                + veh->pathPos.lookPos[1];
            veh->pathPos.lookPos[2] = (float)((float)(nextVpp.lookPos[2] - veh->pathPos.lookPos[2]) * veh->manualTime)
                + veh->pathPos.lookPos[2];
        }
        if ((!info->type || info->type == 2) && (veh->flags & 0x100) == 0)
            VEH_GroundPlant(ent, phys, 1);
        if (g_vehicleDebug->current.enabled)
            VEH_DebugBox(veh->pathPos.lookPos, 8.0, 0.0, 1.0, 1.0);
        if (!veh->nitrousVehicle || (veh->nitrousVehicle->m_flags & 0x80) != 0)
        {
            vel = phys->vel;
            prevOrigin = phys->prevOrigin;
            phys->vel[0] = phys->origin[0] - phys->prevOrigin[0];
            vel[1] = phys->origin[1] - prevOrigin[1];
            vel[2] = phys->origin[2] - prevOrigin[2];
            velOut = phys->vel;
            velScratch = phys->vel;
            phys->vel[0] = 20.0f * phys->vel[0];
            velOut[1] = 20.0f * velScratch[1];
            velOut[2] = 20.0f * velScratch[2];
            bodyVel = phys->bodyVel;
            phys->bodyVel[0] = 0.0f;
            bodyVel[1] = 0.0f;
            bodyVel[2] = 0.0f;
            phys->bodyVel[0] = veh->speed;
            AnglesSubtract(phys->angles, phys->prevAngles, phys->rotVel);
            rotVel = phys->rotVel;
            rotVelScratch = phys->rotVel;
            phys->rotVel[0] = 20.0f * phys->rotVel[0];
            rotVel[1] = 20.0f * rotVelScratch[1];
            rotVel[2] = 20.0f * rotVelScratch[2];
        }
        if (waitNodeHit && veh->waitNode > -1)
            Scr_Notify(ent, scr_const.reached_wait_node, 0);
        if (nextVpp.endOfPath)
        {
            Scr_Notify(ent, scr_const.reached_end_node, 0);
            LOWORD(endNodeNotifyIdx) = veh->pathPos.nodeIdx;
            if (nextVpp.customPath)
                LOWORD(endNodeNotifyIdx) = nextVpp.customPath->pathOrder[veh->pathPos.nodeIdx];
            Scr_AddEntityNum((__int16)endNodeNotifyIdx, 3u, SCRIPTINSTANCE_SERVER, 0);
            Scr_Notify(ent, scr_const.reached_node, 1u);
            if ((veh->flags & 0x100) != 0)
            {
                if (veh->nitrousVehicle)
                {
                    //NitrousVehicle::end_path(veh->nitrousVehicle);
                    veh->nitrousVehicle->end_path();
                }
                if (veh->pathPos.customPath)
                {
                    VP_FreeCustomPath(veh->pathPos.customPath);
                    veh->pathPos.customPath = 0;
                    if (veh->nitrousVehicle)
                    {
                        //goalSpeed = NitrousVehicle::get_max_speed(veh->nitrousVehicle, 1);
                        goalSpeed = veh->nitrousVehicle->get_max_speed(true);
                        if (veh->manualMode == 1)
                            goalSpeed = veh->manualSpeed;
                        Phys_Vec3ToNitrousVec(veh->goalPosition, &goalPosition);
                        //NitrousVehicleController::SetScriptTarget(
                            veh->nitrousVehicle->mVehicleController.SetScriptTarget(
                            veh->nitrousVehicle,
                            &goalPosition,
                            veh->hover.hoverRadius,
                            goalSpeed,
                            veh->stopAtGoal != 0);
                    }
                    veh->flags &= 0xFFFFFE7F;
                    veh->flags |= 2u;
                    veh->moveState = VEH_MOVESTATE_MOVE;
                    veh->stopping = 0;
                }
            }
        }
        if (veh->waitSpeed >= 0.0
            && (veh->waitSpeed >= prevSpeed && veh->speed >= veh->waitSpeed
                || prevSpeed >= veh->waitSpeed && veh->waitSpeed >= veh->speed))
        {
            Scr_Notify(ent, scr_const.reached_wait_speed, 0);
        }
    }
}

void __cdecl VEH_UpdateAIMove(gentity_s *ent)
{
    scr_vehicle_s *veh; // [esp+4h] [ebp-4h]

    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 5877, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->scr_vehicle
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    5878,
                    0,
                    "%s",
                    "ent->scr_vehicle") )
    {
        __debugbreak();
    }
    veh = ent->scr_vehicle;
    switch ( veh->moveState )
    {
        case VEH_MOVESTATE_MOVE:
            VEH_UpdateMoveToGoal(ent, veh->goalPosition);
            break;
        case VEH_MOVESTATE_HOVER:
            VEH_UpdateHover(ent);
            break;
        case VEH_MOVESTATE_PLANE_ONCURVE:
            VEH_UpdatePlaneOnCurve(ent);
            break;
        case VEH_MOVESTATE_PLANE_FREE:
            VEH_UpdatePlaneFree(ent);
            break;
        default:
            return;
    }
}

void __cdecl VEH_UpdateMoveToGoal(gentity_s *ent, const float *goalPos)
{
    float currentSpeed;
    float remainDt;
    float accelScale;
    float speed;
    float alignedVelY;
    float alignedVelZ;
    float axis[3][3];
    float dotVec[2];
    float dt;
    float goalDirNorm[3];
    float accelAlongVel;
    float newVerticalSpeed; // [esp+C4h] [ebp-A0h] BYREF
    float startMoveAngle; // [esp+C8h] [ebp-9Ch]
    float desiredVel[3]; // [esp+CCh] [ebp-98h]
    float newSpeed; // [esp+D8h] [ebp-8Ch] BYREF
    float accelMax; // [esp+DCh] [ebp-88h] BYREF
    float accelMaxDt; // [esp+E0h] [ebp-84h]
    float prevVel[3]; // [esp+E4h] [ebp-80h]
    float accelMagnitude; // [esp+F0h] [ebp-74h]
    float averageVel[3]; // [esp+F4h] [ebp-70h]
    float accelVec[3]; // [esp+100h] [ebp-64h] BYREF
    float speedScale; // [esp+10Ch] [ebp-58h]
    float testPoint[3]; // [esp+110h] [ebp-54h] BYREF
    float lookAheadTime; // [esp+11Ch] [ebp-48h]
    float desiredDir[3]; // [esp+120h] [ebp-44h] BYREF
    vehicle_physic_t *phys; // [esp+12Ch] [ebp-38h]
    const vehicle_info_t *info; // [esp+130h] [ebp-34h]
    scr_vehicle_s *veh; // [esp+134h] [ebp-30h]
    float vecToGoal[3]; // [esp+138h] [ebp-2Ch] BYREF
    float distToGoal; // [esp+144h] [ebp-20h]
    int hovering; // [esp+148h] [ebp-1Ch]
    bool hasGoalhanged; // [esp+14Fh] [ebp-15h]
    float desiredYaw; // [esp+150h] [ebp-14h]
    float realGoalPos[3]; // [esp+154h] [ebp-10h] BYREF
    float angleDiff; // [esp+160h] [ebp-4h]

    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 5248, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->scr_vehicle
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    5249,
                    0,
                    "%s",
                    "ent->scr_vehicle") )
    {
        __debugbreak();
    }
    veh = ent->scr_vehicle;
    phys = &veh->phys;
    hovering = VEH_IsHovering(veh);
    info = BG_GetVehicleInfo(veh->infoIdx);
    realGoalPos[0] = *goalPos;
    realGoalPos[1] = goalPos[1];
    realGoalPos[2] = goalPos[2];
    if ( (veh->flags & 0x200) != 0 )
    {
        lookAheadTime = 1.0f;
        testPoint[0] = (float)(1.0 * veh->phys.vel[0]) + veh->phys.origin[0];
        testPoint[1] = (float)(1.0 * veh->phys.vel[1]) + veh->phys.origin[1];
        testPoint[2] = (float)(1.0 * veh->phys.vel[2]) + veh->phys.origin[2];
        testPoint[2] = testPoint[2] + veh->phys.mins[2];
        if ( !CM_GetHeliHeight(testPoint, 200.0, &veh->phys.heliLockHeight) )
            CM_GetHeliHeight(testPoint, 3000.0, &veh->phys.heliLockHeight);
        realGoalPos[2] = veh->phys.heliLockHeight - veh->phys.mins[2];
    }
    vecToGoal[0] = realGoalPos[0] - phys->origin[0];
    vecToGoal[1] = realGoalPos[1] - phys->origin[1];
    vecToGoal[2] = realGoalPos[2] - phys->origin[2];
    if ( g_vehicleDebug->current.enabled )
        VEH_DebugLine(phys->origin, realGoalPos, 0.5, 1.0, 0.5);
    if ( info->type != 6 )
        vecToGoal[2] = 0.0f;
    distToGoal = Abs(vecToGoal);
    desiredDir[0] = (float)(1.0 / distToGoal) * vecToGoal[0];
    desiredDir[1] = (float)(1.0 / distToGoal) * vecToGoal[1];
    desiredDir[2] = (float)(1.0 / distToGoal) * vecToGoal[2];
    desiredYaw = VEH_UpdateMove_GetDesiredYaw(veh, desiredDir);
    hasGoalhanged = VEH_CheckIfGoalYawChanged(ent, desiredYaw);
    if ( distToGoal <= 0.0 )
    {
        phys->vel[0] = 0.0f;
        phys->vel[1] = 0.0f;
        phys->vel[2] = 0.0f;
        if ( info->type == 2 )
            VEH_GroundPlant(ent, phys, 1);
    }
    else
    {
        dt = 0.05f;
        startMoveAngle = 30.0f;
        if ( info->type == 2 )
            VEH_UpdateYawAngularVel(ent, desiredYaw);
        prevVel[0] = phys->vel[0];
        prevVel[1] = phys->vel[1];
        prevVel[2] = phys->vel[2];
        if ( veh->stopAtGoal )
            dt = VEH_UpdateMove_CheckStop(veh, distToGoal);
        if ( (veh->flags & 0x100) != 0 || info->type != 2 )
            startMoveAngle = 90.0f;
        angleDiff = AngleNormalize180(desiredYaw - phys->angles[1]);
        if ( startMoveAngle <= fabs(angleDiff) )
            speedScale = 0.0f;
        else
            speedScale = 1.0 - (float)(angleDiff / startMoveAngle);
        VEH_GetNewSpeedAndAccel(veh, dt, hovering, speedScale, &newSpeed, &accelMax);
        VEH_GetNewSpeedAndAccel(veh, dt, hovering, 0.5, &newVerticalSpeed, &accelMax);
        if ( info->type == 6 && newVerticalSpeed > (float)(vehHelicopterMaxSpeedVertical->current.value * 17.6) )
            newVerticalSpeed = vehHelicopterMaxSpeedVertical->current.value * 17.6;
        goalDirNorm[0] = (float)(1.0 / distToGoal) * vecToGoal[0];
        goalDirNorm[1] = (float)(1.0 / distToGoal) * vecToGoal[1];
        goalDirNorm[2] = (float)(1.0 / distToGoal) * vecToGoal[2];
        desiredVel[0] = newSpeed * goalDirNorm[0];
        desiredVel[1] = newSpeed * goalDirNorm[1];
        desiredVel[2] = goalDirNorm[2] * newVerticalSpeed;
        accelVec[0] = (float)(newSpeed * goalDirNorm[0]) - prevVel[0];
        accelVec[1] = (float)(newSpeed * goalDirNorm[1]) - prevVel[1];
        accelVec[2] = (float)(goalDirNorm[2] * newVerticalSpeed) - prevVel[2];
        if ( !veh->stopping
            && veh->manualSpeed >= veh->speed
            && (float)((float)(prevVel[0] * desiredVel[0]) + (float)(prevVel[1] * desiredVel[1])) > 0.0
            && (float)((float)(prevVel[0] * accelVec[0]) + (float)(prevVel[1] * accelVec[1])) < 0.0 )
        {
            dotVec[0] = prevVel[0];
            dotVec[1] = prevVel[1];
            Vec2Normalize(dotVec);
            accelAlongVel = (float)(dotVec[0] * accelVec[0]) + (float)(dotVec[1] * accelVec[1]);
            dotVec[0] = accelAlongVel * dotVec[0];
            dotVec[1] = accelAlongVel * dotVec[1];
            accelVec[0] = accelVec[0] - dotVec[0];
            accelVec[1] = accelVec[1] - dotVec[1];
        }
        accelMaxDt = accelMax * dt;
        accelMagnitude = Vec2Length(accelVec);
        if ( accelMagnitude > accelMaxDt )
        {
            if ( accelMagnitude == 0.0
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                            5405,
                            0,
                            "%s",
                            "accelMagnitude") )
            {
                __debugbreak();
            }
            accelVec[0] = (float)(accelMaxDt / accelMagnitude) * accelVec[0];
            accelVec[1] = (float)(accelMaxDt / accelMagnitude) * accelVec[1];
        }
        accelMaxDt = accelMaxDt * 0.5;
        if ( accelVec[2] <= accelMaxDt )
        {
            if ( (-(accelMaxDt)) > accelVec[2] )
                accelVec[2] = -accelMaxDt;
        }
        else
        {
            accelVec[2] = accelMaxDt;
        }
        if ( !hovering )
        {
            VEH_CheckHorizontalVelocityToGoal(veh, vecToGoal, accelMax, accelVec);
            if ( vecToGoal[2] != 0.0 )
                VEH_CheckVerticalVelocityToGoal(veh, vecToGoal[2], accelVec);
        }
        phys->vel[0] = prevVel[0] + accelVec[0];
        phys->vel[1] = prevVel[1] + accelVec[1];
        phys->vel[2] = prevVel[2] + accelVec[2];
        currentSpeed = Abs(phys->vel);
        veh->speed = currentSpeed;
        if ( info->type == 2 )
        {
            AnglesToAxis(ent->r.currentAngles, axis);
            speed = veh->speed;
            alignedVelY = speed * axis[0][1];
            alignedVelZ = speed * axis[0][2];
            phys->vel[0] = speed * axis[0][0];
            phys->vel[1] = alignedVelY;
            phys->vel[2] = alignedVelZ;
        }
        Vec3Lerp(phys->accel, accelVec, 0.5, phys->accel);
        accelMagnitude = Abs(phys->accel);
        if ( accelMagnitude > accelMaxDt && !veh->stopAtGoal )
        {
            accelScale = accelMaxDt / accelMagnitude;
            phys->accel[0] = accelScale * phys->accel[0];
            phys->accel[1] = accelScale * phys->accel[1];
            phys->accel[2] = accelScale * phys->accel[2];
        }
        averageVel[0] = prevVel[0] + phys->vel[0];
        averageVel[1] = prevVel[1] + phys->vel[1];
        averageVel[2] = prevVel[2] + phys->vel[2];
        averageVel[0] = 0.5 * averageVel[0];
        averageVel[1] = 0.5 * averageVel[1];
        averageVel[2] = 0.5 * averageVel[2];
        if ( dt < 0.050000001 )
        {
            remainDt = 0.050000001f - dt;
            phys->origin[0] = remainDt * prevVel[0] + phys->origin[0];
            phys->origin[1] = remainDt * prevVel[1] + phys->origin[1];
            phys->origin[2] = remainDt * prevVel[2] + phys->origin[2];
        }
        phys->origin[0] = (float)(dt * averageVel[0]) + phys->origin[0];
        phys->origin[1] = (float)(dt * averageVel[1]) + phys->origin[1];
        phys->origin[2] = (float)(dt * averageVel[2]) + phys->origin[2];
        if ( info->type != 2 )
            VEH_UpdateMoveOrientation(ent, goalDirNorm);
        if ( info->type == 6 )
            VEH_UpdateVelocityWithRotation(ent);
        if ( info->type == 2 )
            VEH_GroundPlant(ent, phys, 1);
        if ( BG_GetVehicleInfo(veh->infoIdx)->engineSndSpeed == 0.0
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                        5472,
                        0,
                        "%s",
                        "BG_GetVehicleInfo(veh->infoIdx)->engineSndSpeed") )
        {
            __debugbreak();
        }
        if ( !hovering )
        {
            VEH_UpdateMove_CheckNearGoal(ent, distToGoal);
            VEH_UpdateMove_CheckGoalReached(ent, distToGoal);
        }
    }
}

bool __cdecl VEH_IsHovering(scr_vehicle_s *veh)
{
    if ( !veh && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 4438, 0, "%s", "veh") )
        __debugbreak();
    return veh->moveState == VEH_MOVESTATE_HOVER;
}

double __cdecl VEH_UpdateMove_GetDesiredYaw(scr_vehicle_s *veh, float *desiredDir)
{
    float yawDelta;
    float stopAngle;
    float timeToTurn; // [esp+14h] [ebp-24h]
    float timeToStop; // [esp+18h] [ebp-20h]
    float angleDiff; // [esp+1Ch] [ebp-1Ch]
    float vec[3]; // [esp+20h] [ebp-18h] BYREF
    gentity_s *lookAtEnt; // [esp+2Ch] [ebp-Ch]
    vehicle_physic_t *phys; // [esp+30h] [ebp-8h]
    float desiredYaw; // [esp+34h] [ebp-4h]

    phys = &veh->phys;
    //if ( EntHandle::isDefined(&veh->lookAtEnt) )
    if ( veh->lookAtEnt.isDefined() )
    {
        //lookAtEnt = EntHandle::ent(&veh->lookAtEnt);
        lookAtEnt = veh->lookAtEnt.ent();
        vec[0] = lookAtEnt->r.currentOrigin[0] - phys->origin[0];
        vec[1] = lookAtEnt->r.currentOrigin[1] - phys->origin[1];
        vec[2] = lookAtEnt->r.currentOrigin[2] - phys->origin[2];
        return (float)vectoyaw(vec);
    }
    else
    {
        if ( veh->hasGoalYaw && (veh->stopping || VEH_IsHovering(veh)) )
        {
            desiredYaw = veh->goalYaw;
            if ( phys->maxAngleVel[1] == 0.0
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                            4553,
                            0,
                            "%s",
                            "phys->maxAngleVel[YAW]") )
            {
                __debugbreak();
            }
            if ( veh->manualDecel == 0.0
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                            4554,
                            0,
                            "%s",
                            "veh->manualDecel") )
            {
                __debugbreak();
            }
            timeToStop = veh->speed / veh->manualDecel;
            timeToTurn = (float)(phys->maxAngleVel[1] / phys->yawAccel) * 2.0;
            stopAngle = (float)(phys->maxAngleVel[1] * 0.5) * timeToTurn;
            yawDelta = AngleNormalize180(desiredYaw - phys->angles[1]);
            angleDiff = fabs(yawDelta);
            if ( timeToStop > timeToTurn && angleDiff > stopAngle )
                timeToTurn = (float)((float)(angleDiff - stopAngle) / phys->maxAngleVel[1]) + timeToTurn;
            if ( timeToTurn >= timeToStop )
                return desiredYaw;
        }
        if ( veh->hasTargetYaw )
        {
            return veh->targetYaw;
        }
        else if ( VEH_IsHovering(veh) )
        {
            return phys->angles[1];
        }
        else
        {
            return (float)vectoyaw(desiredDir);
        }
    }
}

void __cdecl VEH_UpdateMoveOrientation(gentity_s *ent, float *desiredDir)
{
    float timeToGoal; // [esp+34h] [ebp-50h]
    float stoppingTime; // [esp+38h] [ebp-4Ch]
    scr_vehicle_s *veh; // [esp+44h] [ebp-40h]
    float angularAccel; // [esp+48h] [ebp-3Ch]
    float accelFraction; // [esp+4Ch] [ebp-38h]
    float angle; // [esp+50h] [ebp-34h]
    float horizontalAccel; // [esp+54h] [ebp-30h]
    float bodyDir; // [esp+58h] [ebp-2Ch]
    float bodyDir_4; // [esp+5Ch] [ebp-28h]
    float desiredYaw; // [esp+60h] [ebp-24h]
    float accelVec[3]; // [esp+64h] [ebp-20h] BYREF
    float accelFactor; // [esp+70h] [ebp-14h]
    float perpDir[2]; // [esp+74h] [ebp-10h]
    float dot; // [esp+7Ch] [ebp-8h]
    float stoppingFactor; // [esp+80h] [ebp-4h]

    veh = ent->scr_vehicle;
    desiredYaw = VEH_UpdateMove_GetDesiredYaw(veh, desiredDir);
    VEH_UpdateYawAndNotify(ent, desiredYaw);
    accelVec[0] = veh->phys.accel[0];
    accelVec[1] = veh->phys.accel[1];
    accelVec[2] = veh->phys.accel[2];
    VEH_AddFakeDrag(veh->phys.vel, veh->maxDragSpeed, accelVec);
    horizontalAccel = Vec2Length(accelVec) / 0.050000001;
    Vec3Normalize(accelVec);
    angle = veh->phys.angles[1] * 0.017453292;
    bodyDir = cos(angle);
    bodyDir_4 = sin(angle);
    accelFraction = VEH_CalcAccelFraction(horizontalAccel, veh->infoIdx);
    stoppingFactor = 1.0f;
    if ( veh->stopping && horizontalAccel > 0.0 )
    {
        if ( horizontalAccel == 0.0
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                        4806,
                        0,
                        "%s",
                        "horizontalAccel") )
        {
            __debugbreak();
        }
        timeToGoal = Vec2Length(veh->phys.vel) / horizontalAccel;
        stoppingTime = VEH_CalcStoppingTime(horizontalAccel, accelFraction);
        if ( stoppingTime == 0.0
            && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 4810, 0, "%s", "stoppingTime") )
        {
            __debugbreak();
        }
        if ( stoppingTime > timeToGoal )
            stoppingFactor = timeToGoal / stoppingTime;
    }
    dot = (float)(bodyDir * accelVec[0]) + (float)(bodyDir_4 * accelVec[1]);
    dot = dot * stoppingFactor;
    accelFactor = VEH_CalcAngleForAccel(accelFraction);
    angularAccel = VEH_GetAccelForAngles(veh);
    VEH_UpdateAngleAndAngularVel(
        0,
        (float)(veh->phys.maxPitchAngle * dot) * accelFactor,
        angularAccel,
        angularAccel * 0.40000001,
        0.0,
        &veh->phys);
    perpDir[0] = bodyDir_4;
    perpDir[1] = -bodyDir;
    VEH_UpdateAngleAndAngularVel(
        2,
        (float)(veh->phys.maxRollAngle * (float)((float)((float)(bodyDir_4 * accelVec[0]) + (float)((-(bodyDir)) * accelVec[1])) * stoppingFactor))
    * accelFactor,
        angularAccel,
        angularAccel * 0.40000001,
        0.0,
        &veh->phys);
}

void __cdecl VEH_UpdateAngleAndAngularVel(
                int index,
                float desiredAngle,
                float accel,
                float decel,
                float overShoot,
                vehicle_physic_t *phys)
{
    float absCurAngleVel; // [esp+30h] [ebp-10h]
    float effectiveAccel; // [esp+34h] [ebp-Ch]
    float targetAngleVel; // [esp+38h] [ebp-8h]
    float angleDiff; // [esp+3Ch] [ebp-4h]

    angleDiff = AngleNormalize180(desiredAngle - phys->angles[index]);
    if ( (float)(0.0099999998 * 0.0099999998) <= (float)(angleDiff * angleDiff)
        || (float)(0.050000001 * 0.050000001) <= (float)(phys->rotVel[index] * phys->rotVel[index]) )
    {
        absCurAngleVel = fabs(phys->rotVel[index]);
        targetAngleVel = phys->maxAngleVel[index];
        if ( (float)(angleDiff * phys->rotVel[index]) < 0.0 )
        {
            effectiveAccel = accel;
        }
        else
        {
            if ( decel == 0.0
                && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 4470, 0, "%s", "decel") )
            {
                __debugbreak();
            }
            if ( (overShoot < 0.0 || overShoot >= 1.0)
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                            4474,
                            0,
                            "%s",
                            "overShoot >= 0.f && overShoot < 1.f") )
            {
                __debugbreak();
            }
            if ( (float)((float)(1.0 - overShoot) * (float)((float)(absCurAngleVel * 0.5) * (float)(absCurAngleVel / decel))) < fabs(angleDiff) )
            {
                effectiveAccel = accel;
            }
            else
            {
                targetAngleVel = 0.0f;
                effectiveAccel = decel;
            }
        }
        if (angleDiff < 0.0)
        {
            //LODWORD(targetAngleVel) ^= _mask__NegFloat_;
            targetAngleVel = -targetAngleVel;
        }
        if ( (float)(effectiveAccel * 0.050000001) <= absCurAngleVel
            || (float)(absCurAngleVel * 0.050000001) <= fabs(angleDiff) )
        {
            phys->rotVel[index] = VEH_AccelerateSpeed(phys->rotVel[index], targetAngleVel, effectiveAccel, 0.050000001);
            phys->angles[index] = (float)(phys->rotVel[index] * 0.050000001) + phys->angles[index];
            phys->angles[index] = AngleNormalize180(phys->angles[index]);
        }
        else
        {
            phys->angles[index] = desiredAngle;
            phys->rotVel[index] = 0.0f;
        }
    }
    else
    {
        phys->rotVel[index] = 0.0f;
        phys->angles[index] = desiredAngle;
    }
}

double __cdecl VEH_CalcAccelFraction(float accel, int infoIdx)
{
    float clampedAccel;
    float scaledAccel;
    float minAccel;
    float maxAccel;

    minAccel = 0.0 * 17.6;
    maxAccel = BG_GetVehicleInfo(infoIdx)->accel;
    if ( maxAccel <= (float)(0.0 * 17.6)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    4593,
                    0,
                    "%s",
                    "maxAccel > minAccel") )
    {
        __debugbreak();
    }
    scaledAccel = accel < maxAccel ? accel : maxAccel;
    clampedAccel = accel > minAccel ? scaledAccel : minAccel;
    return (clampedAccel - minAccel) / (maxAccel - minAccel);
}

double __cdecl VEH_CalcAngleForAccel(float accelFraction)
{
    return 1.0 * accelFraction + (1.0 - accelFraction) * 0.1;
}

double __cdecl VEH_CalcStoppingTime(float accel, float accelFraction)
{
    return 3.5 * (float)(1.0 - accelFraction) + (1.0 - (float)(1.0 - accelFraction)) * 2.5;
}

void __cdecl VEH_UpdateYawAndNotify(gentity_s *ent, float desiredYaw)
{
    bool notifyGoalYaw;
    float finalYawDiff;
    float yawDecel; // [esp+20h] [ebp-24h]
    scr_vehicle_s *veh; // [esp+28h] [ebp-1Ch]
    float initialYawDiff; // [esp+2Ch] [ebp-18h]
    float overshoot; // [esp+30h] [ebp-14h]
    float yawAccel; // [esp+34h] [ebp-10h]
    float initalVel; // [esp+38h] [ebp-Ch]

    initialYawDiff = 0.0f;
    veh = ent->scr_vehicle;
    if ( veh->hasGoalYaw )
    {
        initialYawDiff = AngleDelta(veh->phys.angles[1], veh->goalYaw);
    }
    else if ( veh->hasTargetYaw )
    {
        initialYawDiff = AngleDelta(veh->phys.angles[1], veh->targetYaw);
    }
    notifyGoalYaw = (veh->hasGoalYaw || veh->hasTargetYaw) && fabs(initialYawDiff) >= 1.5;
    initalVel = veh->phys.rotVel[1];
    yawAccel = veh->phys.yawAccel;
    yawDecel = veh->phys.yawDecel;
    if ( desiredYaw != veh->prevGoalYaw )
    {
        veh->yawSlowDown = 0;
        veh->prevGoalYaw = desiredYaw;
    }
    if ( veh->yawSlowDown )
    {
        yawAccel = yawAccel * 0.2;
        yawDecel = yawDecel * 0.2;
    }
    if ( veh->hasGoalYaw || veh->hasTargetYaw )
        overshoot = veh->yawOverShoot;
    else
        overshoot = 0.0f;
    VEH_UpdateAngleAndAngularVel(1, desiredYaw, yawAccel, yawDecel, overshoot, &veh->phys);
    if ( veh->hasGoalYaw && (float)(veh->phys.rotVel[1] * initalVel) < 0.0 )
        veh->yawSlowDown = 1;
    if ( notifyGoalYaw )
    {
        if ( veh->hasGoalYaw )
            finalYawDiff = AngleDelta(veh->phys.angles[1], veh->goalYaw);
        else
            finalYawDiff = AngleDelta(veh->phys.angles[1], veh->targetYaw);
        if ( (float)(finalYawDiff * initialYawDiff) < 0.0 || fabs(finalYawDiff) <= 1.5 )
            Scr_Notify(ent, scr_const.goal_yaw, 0);
    }
}

double __cdecl VEH_GetAccelForAngles(scr_vehicle_s *veh)
{
    if ( !veh && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 4690, 0, "%s", "veh") )
        __debugbreak();
    return veh->phys.yawAccel;
}

void __cdecl VEH_AddFakeDrag(const float *velocity, float maxDragSpeed, float *accelVec)
{
    float dragSpeed;
    float dragScale;
    float horizontalVel;
    float velocityVec[2]; // [esp+24h] [ebp-8h] BYREF

    velocityVec[0] = *velocity;
    velocityVec[1] = velocity[1];
    horizontalVel = Vec2Length(velocityVec);
    dragSpeed = horizontalVel < maxDragSpeed ? horizontalVel : maxDragSpeed;
    if ( maxDragSpeed == 0.0
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 4715, 0, "%s", "maxDragSpeed") )
    {
        __debugbreak();
    }
    Vec2Normalize(velocityVec);
    dragScale = 5.0f * (dragSpeed / maxDragSpeed) * (dragSpeed / maxDragSpeed);
    velocityVec[0] = dragScale * velocityVec[0];
    velocityVec[1] = dragScale * velocityVec[1];
    *accelVec = *accelVec + velocityVec[0];
    accelVec[1] = accelVec[1] + velocityVec[1];
}

void __cdecl VEH_CheckHorizontalVelocityToGoal(
                scr_vehicle_s *veh,
                const float *vecToGoal,
                float accelMax,
                float *accelVec)
{
    float turningAbility;
    float *vel;
    float breakVec; // [esp+40h] [ebp-54h]
    float breakVec_4; // [esp+44h] [ebp-50h]
    float breakingAccel; // [esp+48h] [ebp-4Ch]
    float radius; // [esp+54h] [ebp-40h]
    float oldSpeed; // [esp+58h] [ebp-3Ch]
    float requiredDecel; // [esp+5Ch] [ebp-38h]
    float actualDecel; // [esp+64h] [ebp-30h]
    float newSpeed; // [esp+68h] [ebp-2Ch]
    float newVel[3]; // [esp+6Ch] [ebp-28h] BYREF
    vehicle_physic_t *phys; // [esp+78h] [ebp-1Ch]
    float horizontalDist; // [esp+7Ch] [ebp-18h]
    float horizontalSpeed; // [esp+80h] [ebp-14h]
    float perpDir[2]; // [esp+84h] [ebp-10h]
    float radiusVec[2]; // [esp+8Ch] [ebp-8h]

    phys = &veh->phys;
    horizontalDist = Vec2Length(vecToGoal);
    if ( horizontalDist >= 1.0 )
    {
        horizontalSpeed = Vec2Length(phys->vel);
        if ( veh->stopAtGoal
            && (newVel[0] = phys->vel[0] + *accelVec,
                    newVel[1] = phys->vel[1] + accelVec[1],
                    newSpeed = Vec2Length(newVel),
                    newSpeed > 0.0) )
        {
            oldSpeed = Vec2Length(phys->vel);
            requiredDecel = (float)((float)(horizontalSpeed * horizontalSpeed) / (float)(2.0 * horizontalDist)) * 0.050000001;
            if ( requiredDecel > fabs(newSpeed - oldSpeed) )
            {
                newVel[0] = (float)((float)(oldSpeed - requiredDecel) / newSpeed) * newVel[0];
                newVel[1] = (float)((float)(oldSpeed - requiredDecel) / newSpeed) * newVel[1];
                vel = phys->vel;
                *accelVec = newVel[0] - phys->vel[0];
                accelVec[1] = newVel[1] - vel[1];
                actualDecel = Abs(accelVec);
                if ( actualDecel > accelMax )
                {
                    *accelVec = (float)(accelMax / actualDecel) * *accelVec;
                    accelVec[1] = (float)(accelMax / actualDecel) * accelVec[1];
                    accelVec[2] = (float)(accelMax / actualDecel) * accelVec[2];
                }
            }
        }
        else
        {
            perpDir[0] = phys->vel[1];
            perpDir[1] = -phys->vel[0];
            if ( horizontalDist == 0.0
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                            4884,
                            0,
                            "%s",
                            "horizontalDist") )
            {
                __debugbreak();
            }
            radiusVec[0] = fabs((float)(perpDir[0] * *vecToGoal) + (float)(perpDir[1] * vecToGoal[1]));
            if ( radiusVec[0] > horizontalDist )
            {
                radiusVec[1] = (float)(phys->vel[0] * *vecToGoal) + (float)(phys->vel[1] * vecToGoal[1]);
                radiusVec[0] = (float)(1.0 / horizontalSpeed) * radiusVec[0];
                radiusVec[1] = (float)(1.0 / horizontalSpeed) * radiusVec[1];
                radius = (float)((float)(radiusVec[0] * radiusVec[0]) + (float)(radiusVec[1] * radiusVec[1]))
                             / (float)(2.0 * radiusVec[0]);
                if ( radius <= 0.0
                    && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                                4896,
                                0,
                                "%s",
                                "radius > 0.f") )
                {
                    __debugbreak();
                }
                if ( radius > 1.0 && (float)(horizontalSpeed * horizontalSpeed) > (float)(accelMax * radius) )
                {
                    if ( radius == 0.0
                        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 4906, 0, "%s", "radius") )
                    {
                        __debugbreak();
                    }
                    breakingAccel = (float)(horizontalSpeed * horizontalSpeed) / radius;
                    if ( breakingAccel > horizontalSpeed )
                        breakingAccel = horizontalSpeed;
                    if ( veh->stopAtGoal )
                        turningAbility = 1.0f;
                    else
                        turningAbility = veh->turningAbility;
                    breakVec = (float)((float)((float)(  (-(turningAbility)) * breakingAccel) / horizontalSpeed) * 0.05) * phys->vel[0];
                    breakVec_4 = (float)((float)((float)((-(turningAbility)) * breakingAccel) / horizontalSpeed) * 0.05) * phys->vel[1];
                    phys->vel[0] = phys->vel[0] + breakVec;
                    phys->vel[1] = phys->vel[1] + breakVec_4;
                    *accelVec = *accelVec + breakVec;
                    accelVec[1] = accelVec[1] + breakVec_4;
                }
            }
        }
    }
}

void __cdecl VEH_CheckVerticalVelocityToGoal(scr_vehicle_s *veh, float verticalDist, float *accelVec)
{
    float verticalAccel;
    float clampedBreakingAccel;
    float accelerationCap;
    float breakingAccel; // [esp+14h] [ebp-18h]
    float desiredStoppingTime; // [esp+18h] [ebp-14h]
    float verticalSpeed; // [esp+28h] [ebp-4h]

    verticalSpeed = veh->phys.vel[2];
    if ( fabs((unsigned int)accelVec[2]) >= 0.001
        && fabs(verticalSpeed) >= 0.001
        && (float)(verticalSpeed * accelVec[2]) < 0.0
        && (float)(verticalDist * verticalSpeed) > 0.0 )
    {
        desiredStoppingTime = verticalDist / (float)(verticalSpeed * 0.5);
        //if ( (float)((float)(verticalSpeed * 0.050000001) / COERCE_FLOAT(*((unsigned int *)accelVec + 2) ^ _mask__NegFloat_)) > desiredStoppingTime )
        if ( (float)((float)(verticalSpeed * 0.050000001) / -accelVec[2]) > desiredStoppingTime)
        {
            if ( desiredStoppingTime == 0.0
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                            4947,
                            0,
                            "%s",
                            "desiredStoppingTime") )
            {
                __debugbreak();
            }
            breakingAccel = (float)((-(verticalSpeed)) * 0.050000001) / desiredStoppingTime;
            if ( (float)(breakingAccel * breakingAccel) > (float)(accelVec[2] * accelVec[2]) )
            {
                accelerationCap = (float)(veh->manualAccel * 0.050000001) * 3.0;
                if ( breakingAccel < accelerationCap )
                    clampedBreakingAccel = (-verticalSpeed * 0.050000001f) / desiredStoppingTime;
                else
                    clampedBreakingAccel = veh->manualAccel * 0.050000001f * 3.0f;
                if ( breakingAccel < -accelerationCap )
                    verticalAccel = clampedBreakingAccel;
                else
                    verticalAccel = -accelerationCap;
                veh->phys.vel[2] = verticalAccel - accelVec[2] + veh->phys.vel[2];
                accelVec[2] = verticalAccel;
            }
        }
    }
}

int __cdecl VEH_UpdateMove_CheckGoalReached(gentity_s *ent, float distToGoal)
{
    bool goalReached;
    const vehicle_info_t *info; // [esp+18h] [ebp-8h]
    scr_vehicle_s *veh; // [esp+1Ch] [ebp-4h]

    veh = ent->scr_vehicle;
    info = BG_GetVehicleInfo(veh->infoIdx);
    if ( veh->stopAtGoal )
    {
        if ( veh->hover.hoverRadius == 0.0 )
        {
            goalReached = (veh->stopping || distToGoal == 0.0) && veh->speed == 0.0;
        }
        else
        {
            goalReached = veh->hover.hoverRadius >= distToGoal && (2.0f * 17.6f) > veh->speed;
        }
        if ( goalReached )
        {
            if ( info->type == 6 )
                veh->moveState = VEH_MOVESTATE_HOVER;
            else
                veh->moveState = VEH_MOVESTATE_STOP;
            if ( veh->hover.hoverRadius == 0.0 )
            {
                veh->phys.accel[0] = 0.0f;
                veh->phys.accel[1] = 0.0f;
                veh->phys.accel[2] = 0.0f;
                veh->phys.vel[0] = 0.0f;
                veh->phys.vel[1] = 0.0f;
                veh->phys.vel[2] = 0.0f;
            }
LABEL_20:
            Scr_Notify(ent, scr_const.goal, 0);
            return 1;
        }
    }
    else if ( (float)(veh->speed * 0.050000001) >= distToGoal )
    {
        if ( info->type != 6 && !veh->nitrousVehicle )
            veh->moveState = VEH_MOVESTATE_STOP;
        goto LABEL_20;
    }
    return 0;
}

double __cdecl VEH_UpdateMove_CheckStop(scr_vehicle_s *veh, float distToGoal)
{
    float stopDt;
    float remainDt;
    float dt;
    float partialDt;
    float newSpeed; // [esp+20h] [ebp-Ch]
    float stopDist; // [esp+28h] [ebp-4h]

    dt = 0.05f;
    newSpeed = VEH_AccelerateSpeed(veh->speed, veh->manualSpeed, veh->manualAccel, 0.050000001);
    if ( veh->manualDecel == 0.0
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    5043,
                    0,
                    "%s",
                    "veh->manualDecel") )
    {
        __debugbreak();
    }
    stopDist = (float)(newSpeed * 0.5) * (float)(newSpeed / veh->manualDecel);
    if ( stopDist < (float)(distToGoal - (float)(newSpeed * 0.050000001)) || veh->speed <= 0.0 )
    {
        veh->stopping = 0;
    }
    else
    {
        if ( distToGoal > stopDist )
        {
            if ( veh->speed == 0.0
                && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 5055, 0, "%s", "veh->speed") )
            {
                __debugbreak();
            }
            partialDt = 0.050000001f - (distToGoal - stopDist) / veh->speed;
            remainDt = partialDt < 0.050000001f ? partialDt : 0.05f;
            stopDt = partialDt > 0.0f ? remainDt : 0.0f;
            dt = stopDt;
        }
        veh->stopping = 1;
    }
    return dt;
}

void __cdecl VEH_UpdateMove_CheckNearGoal(gentity_s *ent, float distToGoal)
{
    scr_vehicle_s *veh; // [esp+0h] [ebp-4h]

    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 5073, 0, "%s", "ent") )
        __debugbreak();
    veh = ent->scr_vehicle;
    if ( veh->nearGoalNotifyDist != 0.0 && veh->nearGoalNotifyDist > distToGoal )
        Scr_Notify(ent, scr_const.near_goal, 0);
}

void __cdecl VEH_UpdateYawAngularVel(gentity_s *ent, float desiredYaw)
{
    float yawDecel; // [esp+24h] [ebp-24h]
    scr_vehicle_s *veh; // [esp+30h] [ebp-18h]
    float overshoot; // [esp+38h] [ebp-10h]
    float yawAccel; // [esp+3Ch] [ebp-Ch]

    veh = ent->scr_vehicle;
    AngleNormalize180(desiredYaw - veh->phys.angles[1]);
    yawAccel = veh->phys.yawAccel;
    yawDecel = veh->phys.yawDecel;
    if ( desiredYaw != veh->prevGoalYaw )
    {
        veh->yawSlowDown = 0;
        veh->prevGoalYaw = desiredYaw;
    }
    if ( veh->yawSlowDown )
    {
        yawAccel = yawAccel * 0.2;
        yawDecel = yawDecel * 0.2;
    }
    if ( veh->hasGoalYaw || veh->hasTargetYaw )
        overshoot = veh->yawOverShoot;
    else
        overshoot = 0.0f;
    if ( BG_GetVehicleInfo(veh->infoIdx)->type == 2 )
        overshoot = 0.0f;
    VEH_UpdateAngleAndAngularVel(1, desiredYaw, yawAccel, yawDecel, overshoot, &veh->phys);
}

bool __cdecl VEH_CheckIfGoalYawChanged(gentity_s *ent, float desiredYaw)
{
    scr_vehicle_s *veh; // [esp+8h] [ebp-4h]

    veh = ent->scr_vehicle;
    return BG_GetVehicleInfo(veh->infoIdx)->type == 2
            && fabs(desiredYaw - veh->phys.angles[1]) > 1.0;
}

void __cdecl VEH_UpdateHover(gentity_s *ent)
{
    float distToHoverSq;
    scr_vehicle_s *veh;
    float hoverPos[3]; // [esp+1Ch] [ebp-10h] BYREF
    float newHoverDist; // [esp+28h] [ebp-4h]

    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 5493, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->scr_vehicle
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    5494,
                    0,
                    "%s",
                    "ent->scr_vehicle") )
    {
        __debugbreak();
    }
    veh = ent->scr_vehicle;
    hoverPos[0] = veh->goalPosition[0] + veh->hover.hoverGoalPos[0];
    hoverPos[1] = veh->goalPosition[1] + veh->hover.hoverGoalPos[1];
    hoverPos[2] = veh->goalPosition[2] + veh->hover.hoverGoalPos[2];
    VEH_UpdateMoveToGoal(ent, hoverPos);
    newHoverDist = veh->hover.hoverRadius * 0.25;
    distToHoverSq = Vec3DistanceSq(veh->phys.origin, hoverPos);
    if ( newHoverDist * newHoverDist > distToHoverSq )
        VEH_SetHoverGoal(ent);
}

void __cdecl VEH_SetHoverGoal(gentity_s *ent)
{
    int i; // [esp+14h] [ebp-14h]
    scr_vehicle_s *veh; // [esp+18h] [ebp-10h]
    float randomOffset[3]; // [esp+1Ch] [ebp-Ch]

    veh = ent->scr_vehicle;
    if ( veh->hover.hoverRadius == 0.0 )
    {
        veh->hover.hoverGoalPos[0] = 0.0f;
        veh->hover.hoverGoalPos[1] = 0.0f;
        veh->hover.hoverGoalPos[2] = 0.0f;
    }
    else
    {
        for ( i = 0; i < 3; ++i )
            randomOffset[i] = G_flrand((-(veh->hover.hoverRadius)), veh->hover.hoverRadius);
        veh->hover.hoverGoalPos[0] = (float)(-0.5 * veh->hover.hoverGoalPos[0]) + randomOffset[0];
        veh->hover.hoverGoalPos[1] = (float)(-0.5 * veh->hover.hoverGoalPos[1]) + randomOffset[1];
        veh->hover.hoverGoalPos[2] = (float)(-0.5 * veh->hover.hoverGoalPos[2]) + randomOffset[2];
    }
}

void __cdecl VEH_UpdatePlaneOnCurve(gentity_s *ent)
{
    float segmentLength;
    float *vel;
    float speed;
    float yawDiff;
    float pos[3]; // [esp+40h] [ebp-60h] BYREF
    float frameDist; // [esp+4Ch] [ebp-54h]
    float diff[3]; // [esp+50h] [ebp-50h] BYREF
    bool curveDone; // [esp+5Fh] [ebp-41h]
    vehicle_physic_t *phys; // [esp+60h] [ebp-40h]
    scr_vehicle_s *veh; // [esp+64h] [ebp-3Ch]
    float timeThisFrame; // [esp+68h] [ebp-38h]
    float newSpeed; // [esp+6Ch] [ebp-34h] BYREF
    float accelMax; // [esp+70h] [ebp-30h] BYREF
    float curveTime; // [esp+74h] [ebp-2Ch]
    float angles[3]; // [esp+78h] [ebp-28h] BYREF
    float forwardPos[3]; // [esp+84h] [ebp-1Ch] BYREF
    float length; // [esp+90h] [ebp-10h]
    float prevPos[3]; // [esp+94h] [ebp-Ch]

    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 5731, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->scr_vehicle
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    5732,
                    0,
                    "%s",
                    "ent->scr_vehicle") )
    {
        __debugbreak();
    }
    length = 0.0f;
    veh = ent->scr_vehicle;
    phys = &veh->phys;
    VEH_GetNewSpeedAndAccel(veh, 0.050000001, 0, 1.0, &newSpeed, &accelMax);
    frameDist = newSpeed * 0.050000001;
    curveTime = veh->phys.curveTime;
    prevPos[0] = veh->phys.origin[0];
    prevPos[1] = veh->phys.origin[1];
    prevPos[2] = veh->phys.origin[2];
    curveDone = 0;
    while ( frameDist > length && !curveDone )
    {
        curveTime = curveTime + phys->timeStep;
        if ( curveTime > 1.0 )
        {
            curveDone = 1;
            curveTime = 1.0f;
        }
        cCurveManager::GetPos(phys->curveID, curveTime, pos);
        diff[0] = pos[0] - prevPos[0];
        diff[1] = pos[1] - prevPos[1];
        diff[2] = pos[2] - prevPos[2];
        segmentLength = Abs(diff);
        length = segmentLength + length;
        prevPos[0] = pos[0];
        prevPos[1] = pos[1];
        prevPos[2] = pos[2];
    }
    timeThisFrame = curveTime - phys->curveTime;
    phys->curveTime = (float)((float)(frameDist / length) * timeThisFrame) + phys->curveTime;
    if ( (float)(phys->curveTime + phys->timeStep) > 1.0 )
    {
        curveDone = 1;
        phys->curveTime = 1.0 - phys->timeStep;
    }
    cCurveManager::GetPos(phys->curveID, phys->curveTime, pos);
    cCurveManager::GetPos(phys->curveID, phys->curveTime + phys->timeStep, forwardPos);
    phys->origin[0] = pos[0];
    phys->origin[1] = pos[1];
    phys->origin[2] = pos[2];
    phys->prevAngles[0] = phys->angles[0];
    phys->prevAngles[1] = phys->angles[1];
    phys->prevAngles[2] = phys->angles[2];
    diff[0] = forwardPos[0] - pos[0];
    diff[1] = forwardPos[1] - pos[1];
    diff[2] = forwardPos[2] - pos[2];
    Vec3Normalize(diff);
    vectoangles(diff, angles);
    phys->angles[0] = angles[0];
    phys->angles[1] = angles[1];
    if ( veh->hasGoalRoll == 0.0 )
    {
        if ( phys->curveTime > 0.2 )
        {
            yawDiff = phys->prevAngles[1] - phys->angles[1];
            if ( yawDiff <= 0.0 )
            {
                if ( yawDiff < 0.0 && (-(phys->maxRollAngle)) <= phys->angles[2] && fabs((float)(2.0 * yawDiff) - phys->prevAngles[2]) > 2.0 )
                {
                    phys->angles[2] = phys->angles[2] - 1.0;
                }
            }
            else if ( phys->angles[2] <= phys->maxRollAngle
                         && fabs((float)(2.0 * yawDiff) - phys->prevAngles[2]) > 2.0 )
            {
                phys->angles[2] = phys->angles[2] + 1.0;
            }
        }
    }
    else
    {
        VEH_UpdatePlaneRoll(ent);
    }
    veh->speed = newSpeed;
    speed = veh->speed;
    diff[0] = speed * diff[0];
    diff[1] = speed * diff[1];
    diff[2] = speed * diff[2];
    vel = phys->vel;
    phys->vel[0] = diff[0];
    vel[1] = diff[1];
    vel[2] = diff[2];
    if ( g_vehicleDebug->current.enabled )
    {
        cCurveManager::SetCurveDraw(phys->curveID, 0);
        VEH_DebugPlaneOnCurve(ent);
    }
    if ( (float)(phys->curveTime + phys->timeStep) >= 1.0 || curveDone )
    {
        if ( phys->curveTime >= 1.0
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                        5841,
                        0,
                        "%s",
                        "phys->curveTime < 1.0f") )
        {
            __debugbreak();
        }
        veh->moveState = VEH_MOVESTATE_PLANE_FREE;
        cCurveManager::FreeCurve(phys->curveID);
        phys->curveTime = 0.0f;
        phys->curveID = -1;
        Scr_Notify(ent, scr_const.curve_end, 0);
    }
}

void __cdecl VEH_DebugPlaneOnCurve(gentity_s *ent)
{
    scr_vehicle_s *veh; // [esp+4h] [ebp-4Ch]
    float left1[3]; // [esp+8h] [ebp-48h] BYREF
    float left2[3]; // [esp+14h] [ebp-3Ch] BYREF
    float right2[3]; // [esp+20h] [ebp-30h] BYREF
    float color[4]; // [esp+2Ch] [ebp-24h] BYREF
    float x; // [esp+3Ch] [ebp-14h]
    float y; // [esp+40h] [ebp-10h]
    float right1[3]; // [esp+44h] [ebp-Ch] BYREF

    color[0] = 1.0f;
    color[1] = 0.0f;
    color[2] = 0.0f;
    color[3] = 1.0f;
    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 5550, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->scr_vehicle
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    5551,
                    0,
                    "%s",
                    "ent->scr_vehicle") )
    {
        __debugbreak();
    }
    veh = ent->scr_vehicle;
    x = veh->phys.origin[0];
    y = veh->phys.origin[1];
    left1[0] = x + 1000.0;
    left1[1] = y + 1000.0;
    left1[2] = 0.0f;
    right1[0] = x - 1000.0;
    right1[1] = y - 1000.0;
    right1[2] = 0.0f;
    left2[0] = x + 1000.0;
    left2[1] = y - 1000.0;
    left2[2] = 0.0f;
    right2[0] = x - 1000.0;
    right2[1] = y + 1000.0;
    right2[2] = 0.0f;
    CG_DebugLine(left1, left2, color, 0, 5);
    CG_DebugLine(right1, right2, color, 0, 5);
    CG_DebugLine(left1, right2, color, 0, 5);
    CG_DebugLine(left2, right1, color, 0, 5);
}

// Decomp: CoDOMPServer.c:436683 (0x0067C080)  VEH_UpdatePlaneRoll
void __cdecl VEH_UpdatePlaneRoll(gentity_s *ent)
{
    float rollSin;
    scr_vehicle_s *veh;

    if (!ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 5699, 0, "%s", "ent"))
        __debugbreak();
    if (!ent->scr_vehicle
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
            5700,
            0,
            "%s",
            "ent->scr_vehicle"))
    {
        __debugbreak();
    }
    veh = ent->scr_vehicle;
    if (BG_GetVehicleInfo(veh->infoIdx)->type != 3)
        Com_Error(ERR_DROP, "Vehicle type has to be plane to set Roll. ");
    if ((veh->flags & 2) == 0
        || veh->moveState != VEH_MOVESTATE_PLANE_ONCURVE && veh->moveState != VEH_MOVESTATE_PLANE_FREE)
    {
        Com_Error(ERR_DROP, "Can not set roll on non moving plane. ");
    }
    veh->currentRollTime = veh->currentRollTime + 0.050000001;
    //v2 = __libm_sse2_sin((float)((float)((float)(90.0 * veh->currentRollTime) / veh->goalRollTime) * 0.017453292));
    rollSin = sin((float)((float)((float)(90.0 * veh->currentRollTime) / veh->goalRollTime) * 0.017453292));
    veh->phys.angles[2] = veh->goalRoll * rollSin;
    veh->phys.angles[2] = AngleNormalize360(veh->phys.angles[2]);
    if (veh->currentRollTime >= veh->goalRollTime)
        veh->hasGoalRoll = 0.0f;
}

// Decomp: CoDMPServer.c:436708 (0x0067C140)  VEH_UpdatePlaneFree
void __cdecl VEH_UpdatePlaneFree(gentity_s *ent)
{
    vehicle_physic_t *phys;
    float dir[3];
    scr_vehicle_s *veh;
    float dist;

    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 5855, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->scr_vehicle
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    5856,
                    0,
                    "%s",
                    "ent->scr_vehicle") )
    {
        __debugbreak();
    }
    veh = ent->scr_vehicle;
    phys = &veh->phys;
    dir[0] = veh->phys.vel[0];
    dir[1] = veh->phys.vel[1];
    dir[2] = veh->phys.vel[2];
    Vec3Normalize(dir);
    dist = veh->speed * 0.050000001;
    dir[0] = dist * dir[0];
    dir[1] = dist * dir[1];
    dir[2] = dist * dir[2];
    phys->origin[0] = phys->origin[0] + dir[0];
    phys->origin[1] = phys->origin[1] + dir[1];
    phys->origin[2] = phys->origin[2] + dir[2];
}

// Decomp: CoDOMPServer.c:436756  VEH_UpdateChopperPathDrive
void __cdecl VEH_UpdateChopperPathDrive(gentity_s *ent)
{
    float rollAngle;
    float yawAngle;
    float pitchAngle;
    float transitionTimeFrac;
    scr_vehicle_s *veh;
    float pathPosAngles;
    float pathPosAngles_4;
    float pathPosAngles_8;
    float pathPosOrigin;
    float pathPosOrigin_4;
    float pathPosOrigin_8;

    veh = ent->scr_vehicle;
    if ( veh->pathTransitionTime > 0.0 )
    {
        pathPosOrigin = veh->phys.origin[0];
        pathPosOrigin_4 = veh->phys.origin[1];
        pathPosOrigin_8 = veh->phys.origin[2];
        pathPosAngles = veh->phys.angles[0];
        pathPosAngles_4 = veh->phys.angles[1];
        pathPosAngles_8 = veh->phys.angles[2];
    }
    memcpy(&veh->phys, &s_backup.phys, sizeof(veh->phys));
    veh->targetYaw = veh->pathPos.angles[1];
    veh->hasTargetYaw = 1;
    VEH_UpdateMoveToGoal(ent, veh->pathPos.lookPos);
    if ( g_vehicleDebug->current.enabled )
        VEH_DebugBox(veh->pathPos.lookPos, 18.0, 0.60000002, 1.0, 0.60000002);
    veh->pathPos.origin[0] = veh->phys.origin[0];
    veh->pathPos.origin[1] = veh->phys.origin[1];
    if ( veh->pathTransitionTime > 0.0 )
    {
        veh->pathTransitionTime = veh->pathTransitionTime - 0.050000001;
        transitionTimeFrac = 1.0 - (float)(veh->pathTransitionTime / vehHelicopterPathTransitionTime->current.value);
        if ( transitionTimeFrac >= 1.0 )
        {
            veh->flags &= ~0x100u;
            veh->hasTargetYaw = 0;
            veh->phys.origin[0] = pathPosOrigin;
            veh->phys.origin[1] = pathPosOrigin_4;
            veh->phys.origin[2] = pathPosOrigin_8;
            veh->phys.angles[0] = pathPosAngles;
            veh->phys.angles[1] = pathPosAngles_4;
            veh->phys.angles[2] = pathPosAngles_8;
        }
        else
        {
            veh->phys.origin[0] = (float)((float)(pathPosOrigin - veh->phys.origin[0]) * transitionTimeFrac)
                                                    + veh->phys.origin[0];
            veh->phys.origin[1] = (float)((float)(pathPosOrigin_4 - veh->phys.origin[1]) * transitionTimeFrac)
                                                    + veh->phys.origin[1];
            veh->phys.origin[2] = (float)((float)(pathPosOrigin_8 - veh->phys.origin[2]) * transitionTimeFrac)
                                                    + veh->phys.origin[2];
            pitchAngle = veh->phys.angles[0];
            veh->phys.angles[0] = AngleNormalize180(pathPosAngles - pitchAngle) * transitionTimeFrac + pitchAngle;
            yawAngle = veh->phys.angles[1];
            veh->phys.angles[1] = AngleNormalize180(pathPosAngles_4 - yawAngle) * transitionTimeFrac + yawAngle;
            rollAngle = veh->phys.angles[2];
            veh->phys.angles[2] = AngleNormalize180(pathPosAngles_8 - rollAngle) * transitionTimeFrac + rollAngle;
        }
    }
    if ( veh->pathPos.endOfPath && (veh->flags & 0x100) != 0 )
    {
        veh->flags &= ~0x80u;
        veh->flags &= ~0x100u;
        veh->flags |= 2u;
        if ( veh->pathPos.speed > 1.0 )
            veh->moveState = VEH_MOVESTATE_HOVER;
        else
            veh->moveState = VEH_MOVESTATE_STOP;
        veh->stopAtGoal = 1;
        veh->goalPosition[0] = veh->pathPos.lookPos[0];
        veh->goalPosition[1] = veh->pathPos.lookPos[1];
        veh->goalPosition[2] = veh->pathPos.lookPos[2];
        veh->hasTargetYaw = 0;
    }
}

// Decomp: CoDOMPServer.c:436836  Scr_Vehicle_Pain
void __cdecl Scr_Vehicle_Pain(
                gentity_s *pSelf,
                gentity_s *pAttacker,
                int damage,
                const float *point,
                int mod,
                const float *dir,
                hitLocation_t hitLoc,
                int weaponIdx)
{
    const WeaponDef *weapDef;

    weapDef = BG_GetWeaponDef(weaponIdx);
    if ( weapDef->weapType == WEAPTYPE_PROJECTILE
        || weapDef->weapType == WEAPTYPE_GRENADE
        || weapDef->weapType == WEAPTYPE_MINE )
    {
        VEH_JoltBody(pSelf, dir, 1.0, 0.0, 0.0);
    }
    if (pSelf->scr_vehicle->nitrousVehicle)
    {
        //NitrousVehicle::damage(pSelf->scr_vehicle->nitrousVehicle, damage, point, dir, mod);
        pSelf->scr_vehicle->nitrousVehicle->damage(damage, point, dir, mod);
    }
}

// Decomp: CoDMPServer.c:436857 (0x0067C760)  Scr_Vehicle_Touch
void __cdecl Scr_Vehicle_Touch(gentity_s *pSelf, gentity_s *pOther, int __formal)
{
    gentity_s *touchMod;
    int damage;
    float moveLen;
    const vehicle_info_t *info;
    float hitDir[2];
    scr_vehicle_s *veh;
    gentity_s *driver;
    float damageScale;
    float speed;
    float dot;
    float moveDir[3];

    if ( !pSelf && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 9248, 0, "%s", "pSelf") )
        __debugbreak();
    if ( !pSelf->scr_vehicle
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    9249,
                    0,
                    "%s",
                    "pSelf->scr_vehicle") )
    {
        __debugbreak();
    }
    if ( !pOther
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 9250, 0, "%s", "pOther") )
    {
        __debugbreak();
    }
    veh = pSelf->scr_vehicle;
    info = BG_GetVehicleInfo(veh->infoIdx);
    if ( (pOther->s.eType == 1 || pOther->s.eType == 17 || pOther->s.eType == 6 || pOther->s.eType == 14)
        && pOther->takedamage
        && !pOther->tagInfo
        && (pOther->s.eType != 1 || !vehicle_riding->current.enabled)
        && pOther->s.groundEntityNum != pSelf->s.number
        && !info->remoteControl )
    {
        bool applyCollisionDamage;
        driver = VEH_GetSeatOccupantEntity(veh, 0);
        if ( !driver || !pOther->client )
            applyCollisionDamage = true;
        else
        {
            if ( !driver->client
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                            9282,
                            0,
                            "%s",
                            "driver->client") )
            {
                __debugbreak();
            }
            applyCollisionDamage = driver->client->sess.cs.team != pOther->client->sess.cs.team
                || driver->client->sess.cs.team == TEAM_FREE;
        }
        if ( applyCollisionDamage )
        {
            if ( driver )
                touchMod = driver;
            else
                touchMod = pSelf;
            if ( info->collisionDamage > 0.0 )
            {
                moveLen = Vec3NormalizeTo(veh->phys.vel, moveDir);
                if ( moveLen >= 0.001 && pOther->s.eType != 6 )
                {
                    hitDir[0] = pOther->r.currentOrigin[0] - pSelf->r.currentOrigin[0];
                    hitDir[1] = pOther->r.currentOrigin[1] - pSelf->r.currentOrigin[1];
                    Vec2Normalize(hitDir);
                    if ( pOther->client && (pOther->client->ps.pm_flags & 1) != 0 )
                    {
                        G_Damage(
                            pOther,
                            pSelf,
                            touchMod,
                            moveDir,
                            pOther->r.currentOrigin,
                            999999,
                            0,
                            10,
                            0xFFFFFFFF,
                            HITLOC_NONE,
                            0,
                            0,
                            0);
                    }
                    else
                    {
                        dot = (float)(moveDir[0] * hitDir[0]) + (float)(moveDir[1] * hitDir[1]);
                        if ( dot >= 0.80000001 )
                        {
                            if ( info->collisionSpeed == 0.0
                                && !Assert_MyHandler(
                                            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                                            9336,
                                            0,
                                            "%s",
                                            "info->collisionSpeed") )
                            {
                                __debugbreak();
                            }

                            //if ( (_S3_0 & 1) == 0 )
                            //{
                            //    _S3_0 |= 1u;
                            //    minDamageIPS = 5.0 * 17.6;
                            //}
                            static float minDamageIPS = 5.0 * 17.6;

                            speed = fabs(veh->speed);
                            if ( minDamageIPS <= speed )
                            {
                                damageScale = (float)(speed - minDamageIPS) / info->collisionSpeed;
                                if ( damageScale > 1.0 )
                                    damageScale = 1.0f;
                                damageScale = (float)((float)(dot - 0.80000001) / 0.19999999) * damageScale;
                                damage = (int)(float)(damageScale * info->collisionDamage);
                                if ( damage > 0 )
                                    G_Damage(
                                        pOther,
                                        pSelf,
                                        touchMod,
                                        moveDir,
                                        pOther->r.currentOrigin,
                                        damage,
                                        0,
                                        10,
                                        0xFFFFFFFF,
                                        HITLOC_NONE,
                                        0,
                                        0,
                                        0);
                            }
                        }
                    }
                }
            }
        }
    }
}

// Decomp: CoDMPServer.c:437011 (0x0067CC30)  G_GetVehicleTypeString
unsigned __int16 __cdecl G_GetVehicleTypeString(int clientNum, int entityNum)
{
    const vehicle_info_t *info;
    scr_vehicle_s *veh;
    unsigned __int16 string;

    veh = g_entities[entityNum].scr_vehicle;
    if ( !veh )
        return 0;
    info = BG_GetVehicleInfo(veh->infoIdx);
    string = SL_FindString(info->animSet, SCRIPTINSTANCE_SERVER);
    if ( !string )
        return SL_GetString_(SCRIPTINSTANCE_SERVER, info->animSet, 0, 22);
    return string;
}

// Decomp: CoDMPServer.c:437028 (0x0067CCC0)  Scr_Vehicle_Use
void __cdecl Scr_Vehicle_Use(gentity_s *pEnt, gentity_s *pOther, gentity_s *__formal)
{
    team_t vehicle_team;
    int entryPoint;
    int seatIndex;

    if ( pOther->client )
    {
        if ( (pEnt->scr_vehicle->flags & 8) == 0 && (pOther->client->ps.eFlags & 0x4000) == 0 )
        {
            vehicle_team = G_GetVehicleOccupantsTeam(pEnt);
            if ( vehicle_team == TEAM_FREE || vehicle_team == pOther->client->sess.cs.team )
            {
                entryPoint = -1;
                seatIndex = G_GetVehicleSeatToEnter(pEnt, pOther, 0, &entryPoint);
                if ( seatIndex != -1 )
                {
                    VEH_LinkPlayer(pEnt, pOther, seatIndex, 0);
                    if ( !seatIndex )
                        Scr_Notify(pOther, scr_const.vehicle_driver, 0);
                }
            }
        }
    }
}

// Decomp: CoDOMPServer.c:436983  Scr_Vehicle_Die
void __cdecl Scr_Vehicle_Die(
    gentity_s *pSelf,
    gentity_s *pInflictor,
    gentity_s *pAttacker,
    int damage,
    int mod,
    const int weapon,
    const float *dir,
    const hitLocation_t hitLoc,
    int timeOffset)
{
    gentity_s *notifyEnt;
    int driverEntNum;
    char *weaponNameStr;
    unsigned __int16 speedFrac;
    gentity_s *occupant;
    int seatIndex;
    gentity_s *vehicleOccupant;
    const WeaponDef *weapDef;

    vehicleOccupant = 0;
    //if ( EntHandle::isDefined(&pSelf->r.ownerNum) )
    if ( pSelf->r.ownerNum.isDefined() )
    {
        //vehicleOccupant = EntHandle::ent(&pSelf->r.ownerNum);
        vehicleOccupant = pSelf->r.ownerNum.ent();
        Scr_AddBool(1u, SCRIPTINSTANCE_SERVER);
        speedFrac = scr_const.vehicle_death;
        //v9 = EntHandle::ent(&pSelf->r.ownerNum);
        notifyEnt = pSelf->r.ownerNum.ent();
        Scr_Notify(notifyEnt, speedFrac, 1u);
        //v10 = EntHandle::entnum(&pSelf->r.ownerNum);
        driverEntNum = pSelf->r.ownerNum.entnum();
        VEH_UnlinkPlayer(&g_entities[driverEntNum], 0, (char*)"Scr_Vehicle_Die A");
        if ( vehicleOccupant->health > 0 )
        {
            if ( damage > 100000 )
                player_die(vehicleOccupant, pInflictor, pAttacker, damage, mod, weapon, dir, HITLOC_NONE, timeOffset);
            else
                player_die(vehicleOccupant, pInflictor, pAttacker, 100000, mod, weapon, dir, HITLOC_NONE, timeOffset);
        }
    }
    for ( seatIndex = 0; seatIndex < 11; ++seatIndex )
    {
        occupant = VEH_GetSeatOccupantEntity(pSelf->scr_vehicle, seatIndex);
        if ( occupant )
        {
            Scr_AddBool(1u, SCRIPTINSTANCE_SERVER);
            Scr_Notify(occupant, scr_const.vehicle_death, 1u);
            VEH_UnlinkPlayer(occupant, 0, (char *)"Scr_Vehicle_Die B");
            vehicleOccupant = occupant;
            if ( occupant->health > 0 )
            {
                if ( damage > 100000 )
                    player_die(occupant, pInflictor, pAttacker, damage, mod, weapon, dir, HITLOC_NONE, timeOffset);
                else
                    player_die(occupant, pInflictor, pAttacker, 100000, mod, weapon, dir, HITLOC_NONE, timeOffset);
            }
        }
    }
    if ( pAttacker )
    {
        if ( weapon )
        {
            if ( weapon != -1 )
            {
                weapDef = BG_GetWeaponDef(weapon);
                if ( weapDef->weapType == WEAPTYPE_PROJECTILE
                    || weapDef->weapType == WEAPTYPE_GRENADE
                    || weapDef->weapType == WEAPTYPE_MINE )
                {
                    VEH_JoltBody(pSelf, dir, 1.0, 0.0, 0.0);
                }
            }
        }
    }
    if ( vehicleOccupant && pAttacker && pAttacker->client )
    {
        Scr_AddEntity(vehicleOccupant, SCRIPTINSTANCE_SERVER);
        if ( (pAttacker->client->ps.eFlags & 0x4000) != 0 )
        {
            Scr_AddString("tankGun", SCRIPTINSTANCE_SERVER);
        }
        else if ( !weapon || weapon == -1 )
        {
            Scr_AddString("none", SCRIPTINSTANCE_SERVER);
        }
        else
        {
            weaponNameStr = (char *)BG_WeaponName(weapon);
            Scr_AddString(weaponNameStr, SCRIPTINSTANCE_SERVER);
        }
        Scr_Notify(pAttacker, scr_const.destroyed_vehicle, 2u);
    }
    if (pSelf->scr_vehicle->nitrousVehicle)
    {
        //NitrousVehicle::damage(pSelf->scr_vehicle->nitrousVehicle, damage, pSelf->r.currentOrigin, dir, mod);
        pSelf->scr_vehicle->nitrousVehicle->damage(damage, pSelf->r.currentOrigin, dir, mod);
    }
    pSelf->flags |= 0x40000u;
}

// Decomp: CoDMPServer.c:437138 (0x0067D0A0)  SP_script_vehicle
void __cdecl SP_script_vehicle(gentity_s *pSelf, SpawnVar *spawnVar)
{
    const char *typeName;

    if ( spawnVar )
    {
        G_SpawnString(spawnVar, "vehicletype", 0, &typeName);
        G_SpawnVehicle(pSelf, (char *)typeName, 1);
    }
}

// Decomp: CoDMPServer.c:437151 (0x0067D0E0)  SP_script_vehicle_collmap
void __cdecl SP_script_vehicle_collmap(gentity_s *pSelf, SpawnVar *v)
{
    pSelf->r.contents = 0;
    pSelf->s.eType = ET_VEHICLE_COLLMAP;
}

// Decomp: CoDMPServer.c:437158 (0x0067D110)  ScriptVehicle_GetMethod
void (__cdecl *__cdecl ScriptVehicle_GetMethod(const char **pName))(scr_entref_t)
{
    unsigned int methodIndex;

    for ( methodIndex = 0; methodIndex < ARRAY_COUNT(s_methods_0); ++methodIndex )
    {
        if ( !strcmp(*pName, s_methods_0[methodIndex].actionString) )
        {
            *pName = s_methods_0[methodIndex].actionString;
            return s_methods_0[methodIndex].actionFunc;
        }
    }
    return 0;
}

// Decomp: CoDOMPServer.c:437180  CMD_VEH_AttachPath
void __cdecl CMD_VEH_AttachPath(scr_entref_t entref)
{
    const vehicle_info_t *info;
    unsigned __int16 nodeIdx;
    scr_vehicle_s *veh;
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);
    veh = ent->scr_vehicle;
    info = BG_GetVehicleInfo(veh->infoIdx);
    nodeIdx = GScr_GetVehicleNodeIndex(0);
    G_VehSetUpPathPos(&veh->pathPos, nodeIdx);
    veh->flags &= ~0x100u;
    veh->phys.origin[0] = veh->pathPos.origin[0];
    veh->phys.origin[1] = veh->pathPos.origin[1];
    veh->phys.origin[2] = veh->pathPos.origin[2];
    veh->phys.angles[0] = veh->pathPos.angles[0];
    veh->phys.angles[1] = veh->pathPos.angles[1];
    veh->phys.angles[2] = veh->pathPos.angles[2];
    VEH_SetPosition(ent, veh->phys.origin, veh->phys.vel, veh->phys.angles);
    veh->phys.prevOrigin[0] = veh->phys.origin[0];
    veh->phys.prevOrigin[1] = veh->phys.origin[1];
    veh->phys.prevOrigin[2] = veh->phys.origin[2];
    veh->phys.prevAngles[0] = veh->phys.angles[0];
    veh->phys.prevAngles[1] = veh->phys.angles[1];
    veh->phys.prevAngles[2] = veh->phys.angles[2];
    VEH_ResetWheels(ent, &veh->phys);
    if ( !info->type || info->type == 2 )
        VEH_GroundPlant(ent, &veh->phys, 0);
    VEH_SetPosition(ent, veh->phys.origin, veh->phys.vel, veh->phys.angles);
    veh->phys.prevOrigin[0] = veh->phys.origin[0];
    veh->phys.prevOrigin[1] = veh->phys.origin[1];
    veh->phys.prevOrigin[2] = veh->phys.origin[2];
    veh->phys.prevAngles[0] = veh->phys.angles[0];
    veh->phys.prevAngles[1] = veh->phys.angles[1];
    veh->phys.prevAngles[2] = veh->phys.angles[2];
}

// Decomp: CoDMPServer.c:437228 (0x0067D3F0)  VEH_ResetWheels
void __cdecl VEH_ResetWheels(gentity_s *ent, vehicle_physic_t *phys)
{
    scr_vehicle_s *veh;
    int wheelIndex;
    float wheelPos[3];

    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 1004, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->scr_vehicle
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    1005,
                    0,
                    "%s",
                    "ent->scr_vehicle") )
    {
        __debugbreak();
    }
    if ( !phys && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 1006, 0, "%s", "phys") )
        __debugbreak();
    veh = ent->scr_vehicle;
    for ( wheelIndex = 0; wheelIndex < 6; ++wheelIndex )
    {
        if ( veh->boneIndex.wheel[wheelIndex] >= 0 )
        {
            G_DObjGetWorldBoneIndexPos(ent, veh->boneIndex.wheel[wheelIndex], wheelPos);
            phys->wheelZPos[wheelIndex] = wheelPos[2];
        }
    }
}

// Decomp: CoDOMPServer.c:437260  CMD_VEH_GetModel
void __cdecl CMD_VEH_GetModel(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);
    if ( ent && ent->scr_vehicle && ent->scr_vehicle->nitrousVehicle && ent->scr_vehicle->nitrousVehicle->m_xmodel )
        Scr_AddString((char *)ent->scr_vehicle->nitrousVehicle->m_xmodel->name, SCRIPTINSTANCE_SERVER);
    else
        Scr_Error("Incorrect usage of GetModelName()", 0);
}

// Decomp: CoDOMPServer.c:437290  CMD_VEH_GetAttachPos
void __cdecl CMD_VEH_GetAttachPos(scr_entref_t entref)
{
    const vehicle_info_t *info;
    vehicle_physic_t phys;
    __int16 nodeIdx;
    const scr_vehicle_s *veh;
    vehicle_pathpos_t pathPos;
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);
    veh = ent->scr_vehicle;
    info = BG_GetVehicleInfo(veh->infoIdx);
    nodeIdx = GScr_GetVehicleNodeIndex(0);
    G_VehSetUpPathPos(&pathPos, nodeIdx);
    Com_Memcpy(&phys, &veh->phys, 296);
    phys.origin[0] = pathPos.origin[0];
    phys.origin[1] = pathPos.origin[1];
    phys.origin[2] = pathPos.origin[2];
    phys.angles[0] = pathPos.angles[0];
    phys.angles[1] = pathPos.angles[1];
    phys.angles[2] = pathPos.angles[2];
    VEH_ResetWheels(ent, &phys);
    if ( !info->type || info->type == 2 )
        VEH_GroundPlant(ent, &phys, 0);
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    Scr_AddVector(phys.origin, SCRIPTINSTANCE_SERVER);
    Scr_AddArray(SCRIPTINSTANCE_SERVER);
    Scr_AddVector(phys.angles, SCRIPTINSTANCE_SERVER);
    Scr_AddArray(SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:437320  CMD_VEH_StartPath
void __cdecl CMD_VEH_StartPath(scr_entref_t entref)
{
    const vehicle_info_t *info;
    scr_vehicle_s *veh;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    info = BG_GetVehicleInfo(veh->infoIdx);
    if ( veh->pathPos.nodeIdx < 0 )
        Scr_Error("Can't start path on a vehicle that hasn't been attached", 0);
    if ( info->isNitrous && !veh->nitrousVehicle )
        Scr_Error("Can't start physics vehicles on path until the physics is setup", 0);
    if (veh->nitrousVehicle)
    {
        //NitrousVehicle::start_path(veh->nitrousVehicle, 0);
        veh->nitrousVehicle->start_path(0);
    }
    veh->flags |= 0x80u;
    if ( (veh->pathPos.flags & 0x100) != 0 )
        veh->pathTransitionTime = vehHelicopterPathTransitionTime->current.value;
}

// Decomp: CoDOMPServer.c:437350  CMD_VEH_DrivePath
void __cdecl CMD_VEH_DrivePath(scr_entref_t entref)
{
    const vehicle_info_t *info;
    unsigned __int16 nodeIdx;
    scr_vehicle_s *veh;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    info = BG_GetVehicleInfo(veh->infoIdx);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        nodeIdx = GScr_GetVehicleNodeIndex(0);
        G_VehSetUpPathPos(&veh->pathPos, nodeIdx);
    }
    veh->pathPos.flags |= 0x100u;
    if ( info->isNitrous && !veh->nitrousVehicle )
        Scr_Error("Can't start physics vehicles on path until the physics is setup", 0);
    if ( veh->pathPos.nodeIdx < 0 )
        Scr_Error("Invalid start node to DrivePath", 0);
    if ( info->type != 6 && !veh->nitrousVehicle )
        Scr_Error("DrivePath can only be called on helicopters and physics vehicles", 0);
    if (veh->nitrousVehicle)
    {
        //NitrousVehicle::start_path(veh->nitrousVehicle, 1);
        veh->nitrousVehicle->start_path(1);
    }
    veh->flags |= 0x180u;
}

// Decomp: CoDOMPServer.c:437380  CMD_VEH_SetDrivePathPhysicsScale
void __cdecl CMD_VEH_SetDrivePathPhysicsScale(scr_entref_t entref)
{
    const vehicle_info_t *info;
    scr_vehicle_s *veh;
    float scale;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    info = BG_GetVehicleInfo(veh->infoIdx);
    scale = 3.0f;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
        scale = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    if ( info->isNitrous && !veh->nitrousVehicle )
        Scr_Error("Can't SetDrivePathPhysicsScale until the physics is setup", 0);
    if ( info->type != 6 && !veh->nitrousVehicle )
        Scr_Error("SetDrivePathPhysicsScale can only be called on helicopters and physics vehicles", 0);
    if ( veh->nitrousVehicle )
        veh->nitrousVehicle->m_drivepath_scale = scale;
}

// Decomp: CoDOMPServer.c:437410  CMD_VEH_SetSwitchNode
void __cdecl CMD_VEH_SetSwitchNode(scr_entref_t entref)
{
    unsigned __int16 srcNode;
    unsigned __int16 dstNode;
    scr_vehicle_s *veh;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    srcNode = GScr_GetVehicleNodeIndex(0);
    dstNode = GScr_GetVehicleNodeIndex(1u);
    G_VehSetSwitchNode(&veh->pathPos, srcNode, dstNode);
}

// Decomp: CoDOMPServer.c:437430  CMD_VEH_SetWaitNode
void __cdecl CMD_VEH_SetWaitNode(scr_entref_t entref)
{
    scr_vehicle_s *veh;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    veh->waitNode = GScr_GetVehicleNodeIndex(0);
}

// Decomp: CoDOMPServer.c:437450  CMD_VEH_SetWaitSpeed
void __cdecl CMD_VEH_SetWaitSpeed(scr_entref_t entref)
{
    scr_vehicle_s *veh;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    veh->waitSpeed = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER) * 17.6;
    if ( veh->waitSpeed < 0.0 )
        Scr_ParamError(0, "Cannot have a negative wait speed on a vehicle", SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:437470  CMD_VEH_SetSpeed
void __cdecl CMD_VEH_SetSpeed(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);
    CMD_VEH_Script_SetSpeed(ent);
}

// Decomp: CoDOMPServer.c:437490  CMD_VEH_Script_SetSpeed
void __cdecl CMD_VEH_Script_SetSpeed(gentity_s *ent)
{
    const vehicle_info_t *info;
    scr_vehicle_s *veh;

    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 9887, 0, "%s", "ent") )
        __debugbreak();
    veh = ent->scr_vehicle;
    info = BG_GetVehicleInfo(veh->infoIdx);
    if ( !veh && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 9892, 0, "%s", "veh") )
        __debugbreak();
    if ( !info && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 9893, 0, "%s", "info") )
        __debugbreak();
    veh->manualMode = 1;
    veh->manualSpeed = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER) * 17.6;
    if ( veh->nitrousVehicle )
        veh->nitrousVehicle->mVehicleController.m_script_goal_speed = veh->manualSpeed;
    if ( veh->manualSpeed < 0.0 )
    {
        Scr_ParamError(0, "Cannot set negative speed on vehicle", SCRIPTINSTANCE_SERVER);
        veh->manualSpeed = 0.0f;
    }
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 1 )
        veh->manualAccel = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER) * 17.6;
    if ( info->type == 6 && veh->manualSpeed > veh->speed && veh->manualAccel > veh->manualSpeed )
    {
        Com_PrintWarning(15, "WARNING: capping acceleration to speed / sec for vehicle '%d'\n", ent->s.number);
        veh->manualAccel = veh->manualSpeed;
    }
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 2 )
        veh->manualDecel = veh->manualAccel * 0.5;
    else
        veh->manualDecel = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER) * 17.6;
    if ( veh->manualAccel <= 0.0 || veh->manualDecel <= 0.0 )
    {
        Scr_ParamError(1u, "Acceleration/deceleration must be > 0", SCRIPTINSTANCE_SERVER);
        veh->manualAccel = 1.0 * 17.6;
        veh->manualDecel = 1.0 * 17.6;
    }
}

// Decomp: CoDOMPServer.c:437520  CMD_VEH_SetSpeedImmediate
void __cdecl CMD_VEH_SetSpeedImmediate(scr_entref_t entref)
{
    int hasGoalPosition;
    float speed;
    scr_vehicle_s *veh;
    float vecToGoal[3];
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);
    veh = ent->scr_vehicle;
    CMD_VEH_Script_SetSpeed(ent);
    memset(vecToGoal, 0, sizeof(vecToGoal));
    if ( veh->speed > 0.0
        || (veh->goalPosition[0] != 0.0 || veh->goalPosition[1] != 0.0 || veh->goalPosition[2] != 0.0 ? (hasGoalPosition = 0) : (hasGoalPosition = 1),
                !hasGoalPosition) )
    {
        vecToGoal[0] = veh->goalPosition[0] - ent->r.currentOrigin[0];
        vecToGoal[1] = veh->goalPosition[1] - ent->r.currentOrigin[1];
        vecToGoal[2] = veh->goalPosition[2] - ent->r.currentOrigin[2];
        Vec3Normalize(vecToGoal);
    }
    if ( vecToGoal[0] == 0.0 && vecToGoal[1] == 0.0 && vecToGoal[2] == 0.0 )
        AngleVectors(ent->r.currentAngles, vecToGoal, 0, 0);
    veh->speed = veh->manualSpeed;
    veh->phys.bodyVel[0] = veh->speed;
    speed = veh->speed;
    veh->phys.vel[0] = speed * vecToGoal[0];
    veh->phys.vel[1] = speed * vecToGoal[1];
    veh->phys.vel[2] = speed * vecToGoal[2];
}

// Decomp: CoDOMPServer.c:437550  CMD_VEH_GetSpeed
void __cdecl CMD_VEH_GetSpeed(scr_entref_t entref)
{
    scr_vehicle_s *value;

    value = GScr_GetVehicle(entref)->scr_vehicle;
    Scr_AddFloat(value->speed, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:437570  CMD_VEH_GetSpeedMPH
void __cdecl CMD_VEH_GetSpeedMPH(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);
    Scr_AddFloat(ent->scr_vehicle->speed / 17.6, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:437590  CMD_VEH_GetGoalSpeedMPH
void __cdecl CMD_VEH_GetGoalSpeedMPH(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);
    Scr_AddFloat(ent->scr_vehicle->manualSpeed / 17.6, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:437610  CMD_VEH_SetBrake
void __cdecl CMD_VEH_SetBrake(scr_entref_t entref)
{
    scr_vehicle_s *veh;
    float brake;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    brake = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    if ( veh->nitrousVehicle )
        veh->nitrousVehicle->m_script_brake = brake;
    else
        Scr_Error("SetBrake can only be called on nitrous physics vehicles", 0);
}

// Decomp: CoDOMPServer.c:437630  CMD_VEH_SetAcceleration
void __cdecl CMD_VEH_SetAcceleration(scr_entref_t entref)
{
    scr_vehicle_s *veh;
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);
    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 10103, 0, "%s", "ent") )
        __debugbreak();
    veh = ent->scr_vehicle;
    if ( !veh && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 10106, 0, "%s", "veh") )
        __debugbreak();
    veh->manualAccel = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER) * 17.6;
}

// Decomp: CoDOMPServer.c:437650  CMD_VEH_SetDeceleration
void __cdecl CMD_VEH_SetDeceleration(scr_entref_t entref)
{
    scr_vehicle_s *veh;
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);
    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 10126, 0, "%s", "ent") )
        __debugbreak();
    veh = ent->scr_vehicle;
    if ( !veh && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 10129, 0, "%s", "veh") )
        __debugbreak();
    veh->manualDecel = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER) * 17.6;
}

// Decomp: CoDOMPServer.c:437670  CMD_VEH_ResumeSpeed
void __cdecl CMD_VEH_ResumeSpeed(scr_entref_t entref)
{
    float goalSpeed;
    scr_vehicle_s *veh;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    veh->manualMode = 2;
    veh->manualAccel = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER) * 17.6;
    if ( veh->nitrousVehicle )
    {
        //goalSpeed = NitrousVehicle::get_max_speed(veh->nitrousVehicle, 1);
        goalSpeed = veh->nitrousVehicle->get_max_speed(true);
        veh->nitrousVehicle->mVehicleController.m_script_goal_speed = goalSpeed;
        veh->nitrousVehicle->m_speed_scale = 1.0f;
    }
    if ( veh->manualAccel < 0.0 )
        Scr_ParamError(0, "Cannot set negative acceleration on vehicle", SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:437700  CMD_VEH_SetYawSpeed
void __cdecl CMD_VEH_SetYawSpeed(scr_entref_t entref)
{
    scr_vehicle_s *veh;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    veh->phys.maxAngleVel[1] = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    veh->phys.yawAccel = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 2 )
        veh->phys.yawDecel = veh->phys.yawAccel;
    else
        veh->phys.yawDecel = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 3 )
    {
        veh->yawOverShoot = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
        if ( veh->yawOverShoot < 0.0 || veh->yawOverShoot > 1.0 )
            Scr_ParamError(3u, "Overshoot must be in 0 to 1 range", SCRIPTINSTANCE_SERVER);
    }
    if ( veh->phys.maxAngleVel[1] < 0.0 )
        Scr_ParamError(0, "Cannot set negative yaw speed on vehicle", SCRIPTINSTANCE_SERVER);
    if ( veh->phys.yawAccel < 0.0 )
        Scr_ParamError(1u, "Cannot set negative yaw acceleration on vehicle", SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:437730  CMD_VEH_SetMaxPitchRoll
void __cdecl CMD_VEH_SetMaxPitchRoll(scr_entref_t entref)
{
    const vehicle_info_t *info;
    scr_vehicle_s *veh;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    info = BG_GetVehicleInfo(veh->infoIdx);
    if ( info->type != 6 && info->type != 3 )
        Scr_Error("Max Pitch Roll only valid for helicopters and planes.", 0);
    veh->phys.maxPitchAngle = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    veh->phys.maxRollAngle = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    if ( veh->phys.maxPitchAngle < 0.0 )
        Scr_ParamError(0, "Cannot set negative max pitch", SCRIPTINSTANCE_SERVER);
    if ( veh->phys.maxRollAngle < 0.0 )
        Scr_ParamError(1u, "Cannot set negative max roll", SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:437760  CMD_VEH_SetAirResitance
void __cdecl CMD_VEH_SetAirResitance(scr_entref_t entref)
{
    scr_vehicle_s *veh;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    veh->maxDragSpeed = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER) * 17.6;
}

// Decomp: CoDOMPServer.c:437780  CMD_VEH_GetSteerFactor
void __cdecl CMD_VEH_GetSteerFactor(scr_entref_t entref)
{
    float value;

    value = GScr_GetVehicle(entref)->scr_vehicle->nitrousVehicle->m_steer_factor;
    Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:437800  CMD_VEH_GetThrottle
void __cdecl CMD_VEH_GetThrottle(scr_entref_t entref)
{
    float value;

    value = GScr_GetVehicle(entref)->scr_vehicle->nitrousVehicle->m_throttle;
    Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:437820  CMD_VEH_SetTurningAbility
void __cdecl CMD_VEH_SetTurningAbility(scr_entref_t entref)
{
    scr_vehicle_s *veh;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    veh->turningAbility = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:437840  CMD_VEH_SetJitterParams
void __cdecl CMD_VEH_SetJitterParams(scr_entref_t entref)
{
    float jitterPeriodMaxSec;
    float jitterPeriodMinSec;
    scr_vehicle_s *veh;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    Scr_GetVector(0, veh->jitter.jitterOffsetRange, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 1 )
        jitterPeriodMinSec = 0.5f;
    else
        jitterPeriodMinSec = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    veh->jitter.jitterPeriodMin = (int)(float)(jitterPeriodMinSec * 1000.0);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 2 )
        jitterPeriodMaxSec = 1.0f;
    else
        jitterPeriodMaxSec = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    veh->jitter.jitterPeriodMax = (int)(float)(jitterPeriodMaxSec * 1000.0);
    veh->jitter.jitterEndTime = 0;
}

// Decomp: CoDOMPServer.c:437870  CMD_VEH_SetHoverParams
void __cdecl CMD_VEH_SetHoverParams(scr_entref_t entref)
{
    scr_vehicle_s *veh;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    veh->hover.hoverRadius = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 1 )
    {
        veh->hover.hoverSpeed = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
        if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 2 )
            veh->hover.hoverAccel = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDOMPServer.c:437900  CMD_VEH_JoltBody
void __cdecl CMD_VEH_JoltBody(scr_entref_t entref)
{
    float joltDir[3];
    float intensity;
    int numParams;
    float speedFrac;
    float decel;
    gentity_s *ent;
    float joltPos[3];

    ent = GScr_GetVehicle(entref);
    numParams = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    Scr_GetVector(0, joltPos, SCRIPTINSTANCE_SERVER);
    intensity = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    if ( numParams <= 2 )
    {
        speedFrac = 0.0f;
        decel = 0.0f;
    }
    else
    {
        speedFrac = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
        if ( speedFrac < 0.0 || speedFrac > 1.0 )
            Scr_ParamError(2u, "Speed fraction must be between [0,1]", SCRIPTINSTANCE_SERVER);
        decel = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER) * 17.6;
        if ( decel < 0.0 )
            Scr_ParamError(3u, "Deceleration can't be negative", SCRIPTINSTANCE_SERVER);
    }
    joltDir[0] = ent->r.currentOrigin[0] - joltPos[0];
    joltDir[1] = ent->r.currentOrigin[1] - joltPos[1];
    joltDir[2] = ent->r.currentOrigin[2] - joltPos[2];
    Vec3Normalize(joltDir);
    VEH_JoltBody(ent, joltDir, intensity, speedFrac, decel);
}

// Decomp: CoDOMPServer.c:437930  CMD_VEH_FreeVehicle
void __cdecl CMD_VEH_FreeVehicle(scr_entref_t entref)
{
    const char *freeErrorMsg;
    scr_vehicle_s *veh;
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);
    veh = ent->scr_vehicle;
    if ( (veh->flags & 1) != 0 )
    {
        freeErrorMsg = va("Can't free vehicle that a player is using");
        Scr_Error(freeErrorMsg, 0);
    }
    if ( !veh->nitrousVehicle )
    {
        G_FreeVehicle(ent);
        ent->s.eType = ET_VEHICLE_CORPSE;
        Scr_SetString(&ent->classname, scr_const.script_vehicle_corpse, SCRIPTINSTANCE_SERVER);
    }
    ent->flags |= 0x40000u;
    Scr_Notify(ent, scr_const.death, 0);
}

// Decomp: CoDOMPServer.c:437960  CMD_VEH_GetWheelSurface
void __cdecl CMD_VEH_GetWheelSurface(scr_entref_t entref)
{
    const char *noWheelsErrorMsg;
    char *surfaceName;
    const vehicle_info_t *info;
    scr_vehicle_s *veh;
    unsigned int wheelName;
    int wheel;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    info = BG_GetVehicleInfo(veh->infoIdx);
    wheelName = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    wheel = 0;
    if ( info->type != 2 && info->type && info->type != 1 )
    {
        noWheelsErrorMsg = va("Vehicle type [%s] has no wheels\n", info->name);
        Scr_Error(noWheelsErrorMsg, 0);
    }
    if ( wheelName == scr_const.front_left )
    {
        wheel = 0;
    }
    else if ( wheelName == scr_const.front_right )
    {
        wheel = 1;
    }
    else if ( wheelName == scr_const.back_left )
    {
        wheel = 2;
    }
    else if ( wheelName == scr_const.back_right )
    {
        wheel = 3;
    }
    else if ( wheelName == scr_const.middle_left )
    {
        wheel = 4;
    }
    else if ( wheelName == scr_const.middle_right )
    {
        wheel = 5;
    }
    else
    {
        Scr_ParamError(
            0,
            "Valid wheel names are: [front_left, front_right, back_left, back_right, middle_left, middle_right]\n",
            SCRIPTINSTANCE_SERVER);
    }
    if ( !info->type && wheel > 3 )
        Scr_ParamError(0, "Vehicle has no middle wheels\n", SCRIPTINSTANCE_SERVER);
    if ( veh->phys.wheelSurfType[wheel] )
    {
        surfaceName = (char *)Com_SurfaceTypeToName(veh->phys.wheelSurfType[wheel]);
        Scr_AddString(surfaceName, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddString("none", SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDOMPServer.c:438000  CMD_VEH_GetVehicleOwner
void __cdecl CMD_VEH_GetVehicleOwner(scr_entref_t entref)
{
    gentity_s *owner;
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);

    if ( ent->r.ownerNum.isDefined() )
    {
        owner = ent->r.ownerNum.ent();
        Scr_AddEntity(owner, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDOMPServer.c:438020  CMD_VEH_IsVehicleUsable
void __cdecl CMD_VEH_IsVehicleUsable(scr_entref_t entref)
{
    if ( (GScr_GetVehicle(entref)->spawnflags & 1) != 0 )
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:438040  CMD_VEH_MakeVehicleUsable
void __cdecl CMD_VEH_MakeVehicleUsable(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);
    G_MakeVehicleUsable(ent);
}

// Decomp: CoDOMPServer.c:438060  CMD_VEH_MakeVehicleUnusable
void __cdecl CMD_VEH_MakeVehicleUnusable(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);
    G_MakeVehicleUsable(ent);
    ent->spawnflags &= ~1u;
    ent->r.contents &= ~0x200000u;
}

// Decomp: CoDOMPServer.c:438080  CMD_VEH_UseVehicle
void __cdecl CMD_VEH_UseVehicle(scr_entref_t entref)
{
    gentity_s *user;
    gentity_s *ent;
    int seatIndex;

    ent = GScr_GetVehicle(entref);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    10628,
                    0,
                    "%s",
                    "Scr_GetNumParam() == 2") )
    {
        __debugbreak();
    }
    user = Scr_GetEntity(0);
    seatIndex = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    if ( user->client )
    {
        if ( (user->client->ps.eFlags & 0x4000) != 0 )
            VEH_UnlinkPlayer(user, 0, (char*)"CMD_VEH_UseVehicle");
        VEH_LinkPlayer(ent, user, seatIndex, 0);
    }
}

// Decomp: CoDOMPServer.c:438100  CMD_VEH_SetViewClamp
void __cdecl CMD_VEH_SetViewClamp(scr_entref_t entref)
{
    const char *viewClampErrorMsg;
    float negLeftClamp;
    float rightArcParam;
    float rightArcClamp;
    float negRightClamp;
    float topArcParam;
    float topArcClamp;
    float negTopArc;
    float bottomArcParam;
    float bottomArcClamp;
    float negBottomArc;
    float leftArcParam;
    float leftArcClamp;
    int numParam;
    gentity_s *user;

    numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParam < 3 || numParam > 5 )
    {
        viewClampErrorMsg = va(
                     "SetViewClamp:    Does not have manditory parameters SetViewClamp( <user entity>, <right arc>, <left arc>, [<top"
                     " arc>], [<bottom arc>] )\n");
        Scr_Error(viewClampErrorMsg, 0);
    }
    user = Scr_GetEntity(0);
    if ( user->client )
    {
        if ( numParam <= 1 )
            leftArcParam = user->client->linkAnglesMinClamp[1];
        else
            leftArcParam = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
        if ( (float)(leftArcParam - 180.0) < 0.0 )
            leftArcClamp = leftArcParam;
        else
            leftArcClamp = 180.0f;
        if ( (float)(0.0 - leftArcParam) < 0.0 )
            negLeftClamp = leftArcClamp;
        else
            negLeftClamp = 0.0f;
        user->client->linkAnglesMinClamp[1] = -negLeftClamp;
        if ( numParam <= 2 )
            rightArcParam = user->client->linkAnglesMaxClamp[1];
        else
            rightArcParam = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
        if ( (float)(rightArcParam - 180.0) < 0.0 )
            rightArcClamp = rightArcParam;
        else
            rightArcClamp = 180.0f;
        if ( (float)(0.0 - rightArcParam) < 0.0 )
            negRightClamp = rightArcClamp;
        else
            negRightClamp = 0.0f;
        user->client->linkAnglesMaxClamp[1] = negRightClamp;
        if ( numParam <= 3 )
            topArcParam = user->client->linkAnglesMinClamp[0];
        else
            topArcParam = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
        if ( (float)(topArcParam - 180.0) < 0.0 )
            topArcClamp = topArcParam;
        else
            topArcClamp = 180.0f;
        if ( (float)(0.0 - topArcParam) < 0.0 )
            negTopArc = topArcClamp;
        else
            negTopArc = 0.0f;
        user->client->linkAnglesMinClamp[0] = -negTopArc;
        if ( numParam <= 4 )
            bottomArcParam = user->client->linkAnglesMaxClamp[0];
        else
            bottomArcParam = Scr_GetFloat(4u, SCRIPTINSTANCE_SERVER);
        if ( (float)(bottomArcParam - 180.0) < 0.0 )
            bottomArcClamp = bottomArcParam;
        else
            bottomArcClamp = 180.0f;
        if ( (float)(0.0 - bottomArcParam) < 0.0 )
            negBottomArc = bottomArcClamp;
        else
            negBottomArc = 0.0f;
        user->client->linkAnglesMaxClamp[0] = negBottomArc;
    }
}

// Decomp: CoDOMPServer.c:438140  CMD_VEH_ResetViewClamp
void __cdecl CMD_VEH_ResetViewClamp(scr_entref_t entref)
{
    const char *resetViewClampErrorMsg;
    const vehicle_info_t *info;
    scr_vehicle_s *veh;
    gentity_s *user;
    gentity_s *ent;
    int seatIndex;

    ent = GScr_GetVehicle(entref);
    veh = ent->scr_vehicle;
    info = BG_GetVehicleInfo(veh->infoIdx);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
    {
        resetViewClampErrorMsg = va(
                     "SetViewClamp:    Does not have manditory parameters SetViewClamp( <user entity>, <right arc>, <left arc>, [<top"
                     " arc>], [<bottom arc>] )\n");
        Scr_Error(resetViewClampErrorMsg, 0);
    }
    user = Scr_GetEntity(0);
    if ( user->client )
    {
        seatIndex = G_GetVehicleSeatPlayerOccupies(ent, user);
        if ( seatIndex < 0 )
            Scr_Error("ResetViewClamp: Player not on vehicle.", 0);
        VEH_SetLinkAngleClamps(info, user, seatIndex, user->client->ps.viewangles, veh->phys.angles);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (CMD_VEH_AddVehicleToCompass @ 82645980)
static unsigned __int8 CMD_VEH_ResolveCompassIconFaction(gentity_s *ent, const char *iconType)
{
    const vehicle_info_t *info;
    scr_vehicle_s *veh;

    veh = ent->scr_vehicle;
    info = BG_GetVehicleInfo(veh->infoIdx);
    if ( !I_stricmp(iconType, "helicopter") )
        return 4;
    if ( !I_stricmp(iconType, "plane") )
        return 5;
    if ( !I_stricmp(iconType, "automobile") )
        return 6;
    if ( !I_stricmp(iconType, "tank") || !*iconType )
    {
        if ( info->type == 2 )
            return 2;
        if ( info->type == 1 )
            return 3;
        return 1;
    }
    Scr_Error(va("Unrecognized vehicle type given, \"%s\".", iconType), 0);
    return 1;
}

void __cdecl CMD_VEH_AddVehicleToCompass(scr_entref_t entref)
{
    gentity_s *ent;
    scr_vehicle_s *veh;
    const char *iconType;

    ent = GScr_GetVehicle(entref);
    veh = ent->scr_vehicle;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
#ifdef KISAK_SP
        if ( !veh->nitrousVehicle )
        {
            iconType = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
            if ( !*iconType )
            {
                Com_PrintWarning(
                    25,
                    "Script AddVehicleToCompass(); Was not passed a compassIconType, defaulting to \"tank\".\n");
                ent->s.faction.iHeadIconTeam = 1;
            }
            else
            {
                ent->s.faction.iHeadIconTeam = CMD_VEH_ResolveCompassIconFaction(ent, iconType);
            }
        }
#endif
        ent->s.lerp.u.vehicle.drawOnCompass = 1;
    }
#ifdef KISAK_SP
    else
    {
        Com_PrintWarning(
            25,
            "Script AddVehicleToCompass(); Was not passed a compassIconType, defaulting to \"tank\".\n");
        if ( !veh->nitrousVehicle )
            ent->s.faction.iHeadIconTeam = 1;
    }
#endif
}

// Decomp: CoDSP_rdBlackOps.map.c (CMD_VEH_RemoveVehicleFromCompass @ 82645B58)
void __cdecl CMD_VEH_RemoveVehicleFromCompass(scr_entref_t entref)
{
    gentity_s *ent;
    scr_vehicle_s *veh;

    ent = GScr_GetVehicle(entref);
    veh = ent->scr_vehicle;
    ent->s.lerp.u.vehicle.drawOnCompass = 0;
#ifdef KISAK_SP
    if ( !veh->nitrousVehicle )
        ent->s.faction.iHeadIconTeam = 0;
#endif
}

// Decomp: CoDSP_rdBlackOps.map.c (CMD_VEH_SetHealthPercent @ 82648300)
void __cdecl CMD_VEH_SetHealthPercent(scr_entref_t entref)
{
    gentity_s *ent;
    float healthPercent;

    if ( !Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        Scr_Error("Insufficient arguments for setHealthPercent", 0);
    ent = GScr_GetVehicle(entref);
    healthPercent = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    ent->s.time2 = (int)(healthPercent * 15.0f) & 0xF;
}

// Decomp: CoDOMPServer.c:438170  CMD_VEH_SetVehicleLookatText
void __cdecl CMD_VEH_SetVehicleLookatText(scr_entref_t entref)
{
    scr_vehicle_s *veh;
    VariableUnion string1;
    VariableUnion string0;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    string0.intValue = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    Scr_SetString(&veh->lookAtText0, string0.stringValue, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 1 )
    {
        string1.intValue = Scr_GetConstIString(1u, SCRIPTINSTANCE_SERVER);
        Scr_SetString(&veh->lookAtText1, string1.stringValue, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (CMD_VEH_StartEngineSound @ 82645C60)
void __cdecl CMD_VEH_StartEngineSound(scr_entref_t entref)
{
    scr_vehicle_s *veh;
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);
    veh = ent->scr_vehicle;
    veh->flags |= 0x1000u;
    if ( veh->scriptEngineVolume <= 0.0f )
    {
        veh->scriptEngineVolume = 1.0f;
        G_VehSetClientSideEngineVolume(ent, veh->scriptEngineVolume);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (CMD_VEH_StopEngineSound @ 82645C68)
void __cdecl CMD_VEH_StopEngineSound(scr_entref_t entref)
{
    scr_vehicle_s *veh;
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);
    veh = ent->scr_vehicle;
    veh->flags &= ~0x1000u;
    veh->scriptEngineVolume = 0.0f;
    G_VehSetClientSideEngineVolume(ent, 0.0f);
}

// Decomp: CoDSP_rdBlackOps.map.c (CMD_VEH_SetEngineVolume @ 82645C70)
void __cdecl CMD_VEH_SetEngineVolume(scr_entref_t entref)
{
    scr_vehicle_s *veh;
    gentity_s *ent;
    float volume;

    ent = GScr_GetVehicle(entref);
    veh = ent->scr_vehicle;
    volume = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    if ( volume < 0.0f )
        volume = 0.0f;
    else if ( volume > 1.0f )
        volume = 1.0f;
    veh->scriptEngineVolume = volume;
    G_VehSetClientSideEngineVolume(ent, volume);
}

// Decomp: CoDSP_rdBlackOps.map.c (CMD_VEH_GetEngineVolume @ 82645C78)
void __cdecl CMD_VEH_GetEngineVolume(scr_entref_t entref)
{
    scr_vehicle_s *veh;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    Scr_AddFloat(veh->scriptEngineVolume, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:438190  CMD_VEH_NearGoalNotifyDist
void __cdecl CMD_VEH_NearGoalNotifyDist(scr_entref_t entref)
{
    scr_vehicle_s *veh;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    veh->nearGoalNotifyDist = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:438260 (0x0067F5A0)  CMD_VEH_SetGoalPos
void __cdecl CMD_VEH_SetGoalPos(scr_entref_t entref)
{
    char *usePathErrorMsg;
    const vehicle_info_t *info;
    int usePath;
    phys_vec3 goalPosition;
    float goalSpeed;
    scr_vehicle_s *veh;
    gentity_s *ent;
    ent = GScr_GetVehicle(entref);
    veh = ent->scr_vehicle;
    if (!veh->nitrousVehicle)
        veh->manualMode = 1;
    Scr_GetVector(0, veh->goalPosition, SCRIPTINSTANCE_SERVER);
    veh->stopAtGoal = (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 1
        && Scr_GetInt(1u, SCRIPTINSTANCE_SERVER) != 0;
    if (veh->nitrousVehicle)
    {
        goalSpeed = veh->nitrousVehicle->get_max_speed(true);
        if (veh->manualMode == 1)
            goalSpeed = veh->manualSpeed;
        Phys_Vec3ToNitrousVec(veh->goalPosition, &goalPosition);
        //NitrousVehicleController::SetScriptTarget(
            veh->nitrousVehicle->mVehicleController.SetScriptTarget(
            veh->nitrousVehicle,
            &goalPosition,
            veh->hover.hoverRadius,
            goalSpeed,
            veh->stopAtGoal != 0);
    }
    else if (veh->manualSpeed == 0.0 || veh->manualAccel == 0.0 || veh->manualDecel == 0.0)
    {
        Scr_Error("Speed and acceleration must not be zero before setting goal pos", 0);
    }
    veh->flags &= 0xFFFFFE7F;
    veh->flags |= 2u;
    veh->moveState = VEH_MOVESTATE_MOVE;
    veh->stopping = 0;
    usePath = 0;
    if ((unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 2)
        usePath = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
    if (usePath)
    {
        if (VP_FindPath(veh->phys.origin, veh->goalPosition, &veh->pathPos))
        {
            info = BG_GetVehicleInfo(veh->infoIdx);
            veh->pathPos.flags |= 0x100u;
            if (info->type != 6 && !veh->nitrousVehicle)
            {
                usePathErrorMsg = va("SetVehGoalPos with 'usePath' can only be called on helicopters and physics vehicles");
                Scr_Error(usePathErrorMsg, 0);
            }
            if (veh->nitrousVehicle)
            {
                //NitrousVehicle::start_path(veh->nitrousVehicle, 1);
                veh->nitrousVehicle->start_path(1);
            }
            veh->flags |= 0x180u;
            Scr_AddBool(1u, SCRIPTINSTANCE_SERVER);
        }
        else
        {
            veh->moveState = VEH_MOVESTATE_STOP;
            veh->flags &= 0xFFFFFE7D;
            Scr_AddBool(0, SCRIPTINSTANCE_SERVER);
        }
    }
    else
    {
        Scr_AddBool(1u, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDOMPServer.c:438320  CMD_VEH_ClearGoalPos
void __cdecl CMD_VEH_ClearGoalPos(scr_entref_t entref)
{
    scr_vehicle_s *veh;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    if ( veh->nitrousVehicle )
    {
        veh->nitrousVehicle->m_flags &= ~4u;
        //NitrousVehicle::end_path(veh->nitrousVehicle);
        veh->nitrousVehicle->end_path();
        if ( (veh->flags & 0x100) != 0 && veh->pathPos.customPath )
        {
            VP_FreeCustomPath(veh->pathPos.customPath);
            veh->pathPos.customPath = 0;
        }
    }
    else
    {
        veh->moveState = VEH_MOVESTATE_STOP;
    }
    veh->flags &= 0xFFFFFE7F;
}

// Decomp: CoDMPServer.c:438353 (0x0067F8A0)  CMD_VEH_SetPlaneGoalPos
void __cdecl CMD_VEH_SetPlaneGoalPos(scr_entref_t entref)
{
    double Float;
    const char *planeGoalErrorMsg;
    float *goalPosition;
    float *goalPosPtr;
    int goalParamIndex;
    float goals[10][3];
    scr_vehicle_s *veh;
    float goalAngles[3];
    float travelTime;
    float avgSpeed;
    int numGoals;
    float speedChange;
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);
    veh = ent->scr_vehicle;
    numGoals = 0;
    if ( !veh && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 11096, 0, "%s", "veh") )
        __debugbreak();
    for ( goalParamIndex = 0; goalParamIndex < 10 && Scr_GetType(goalParamIndex, SCRIPTINSTANCE_SERVER) == 4; ++goalParamIndex )
    {
        Scr_GetVector(goalParamIndex, goals[goalParamIndex], SCRIPTINSTANCE_SERVER);
        ++numGoals;
    }
    goalPosition = veh->goalPosition;
    goalPosPtr = goals[numGoals - 1];
    veh->goalPosition[0] = *goalPosPtr;
    goalPosition[1] = goalPosPtr[1];
    goalPosition[2] = goalPosPtr[2];
    memset(goalAngles, 0, sizeof(goalAngles));
    veh->hasGoalYaw = 0;
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 1
        && (Scr_GetType(numGoals, SCRIPTINSTANCE_SERVER) == 5 || Scr_GetType(numGoals, SCRIPTINSTANCE_SERVER) == 6) )
    {
        if ( Scr_GetType(numGoals, SCRIPTINSTANCE_SERVER) != 5 )
            Scr_GetType(numGoals, SCRIPTINSTANCE_SERVER);
        Float = Scr_GetFloat(numGoals, SCRIPTINSTANCE_SERVER);
        veh->manualSpeed = Float * 17.6;
    }
    else
    {
        veh->manualSpeed = veh->speed;
        if ( veh->speed <= 0.0 )
        {
            planeGoalErrorMsg = va("entity %i is at rest ( speed of 0 ), new speed is needed to make plane move.", ent->s.number);
            Scr_Error(planeGoalErrorMsg, 0);
        }
    }
    if ( veh->moveState == VEH_MOVESTATE_PLANE_ONCURVE )
    {
        cCurveManager::FreeCurve(veh->phys.curveID);
        veh->phys.curveID = -1;
        veh->phys.curveLength = 0.0f;
        veh->phys.curveTime = 0.0f;
    }
    VEH_GenerateCurveForPlane(ent, goals, numGoals, goalAngles);
    veh->manualMode = 1;
    avgSpeed = (float)(veh->manualSpeed + veh->speed) / 2.0;
    travelTime = veh->phys.curveLength / avgSpeed;
    speedChange = veh->manualSpeed - veh->speed;
    if ( speedChange < 0.0 )
        veh->manualDecel = speedChange / travelTime;
    else
        veh->manualAccel = speedChange / travelTime;
    veh->phys.timeStep = 40.0 / veh->phys.curveLength;
    veh->flags |= 2u;
    veh->moveState = VEH_MOVESTATE_PLANE_ONCURVE;
    veh->stopping = 0;
}

static float tweaker = 0.89999998;
// Decomp: CoDMPServer.c:438444 (0x0067FC70)  VEH_GenerateCurveForPlane
void __cdecl VEH_GenerateCurveForPlane(gentity_s *ent, float (*goals)[3], int numGoals, const float *goalAngles)
{
    int nextGoalIndex;
    double tailCurveLength;
    float manualSpeed;
    int goalIndex;
    float distNodes[3];
    vehicle_physic_t *phys;
    scr_vehicle_s *veh;
    float distBetweenGoals;
    float vecToGoal[3];
    float nodes[5][3];
    float distToGoal;
    float distance[3];
    float timeStep;

    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 5599, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->scr_vehicle
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    5600,
                    0,
                    "%s",
                    "ent->scr_vehicle") )
    {
        __debugbreak();
    }
    veh = ent->scr_vehicle;
    phys = &veh->phys;
    vecToGoal[0] = (*goals)[0] - veh->phys.origin[0];
    vecToGoal[1] = (*goals)[1] - veh->phys.origin[1];
    vecToGoal[2] = (*goals)[2] - veh->phys.origin[2];
    distToGoal = Abs(vecToGoal);
    *(_QWORD *)&nodes[0][0] = *(_QWORD *)veh->phys.origin;
    nodes[0][2] = veh->phys.origin[2];
    AngleVectors(veh->phys.angles, nodes[1], 0, 0);
    nodes[1][0] = (float)(distToGoal * 0.050000001) * nodes[1][0];
    nodes[1][1] = (float)(distToGoal * 0.050000001) * nodes[1][1];
    nodes[1][2] = (float)(distToGoal * 0.050000001) * nodes[1][2];
    nodes[1][0] = veh->phys.origin[0] + nodes[1][0];
    nodes[1][1] = veh->phys.origin[1] + nodes[1][1];
    nodes[1][2] = veh->phys.origin[2] + nodes[1][2];
    AngleVectors(veh->phys.angles, nodes[2], 0, 0);
    nodes[2][0] = (float)(distToGoal * 0.1) * nodes[2][0];
    nodes[2][1] = (float)(distToGoal * 0.1) * nodes[2][1];
    nodes[2][2] = (float)(distToGoal * 0.1) * nodes[2][2];
    nodes[2][0] = veh->phys.origin[0] + nodes[2][0];
    nodes[2][1] = veh->phys.origin[1] + nodes[2][1];
    nodes[2][2] = veh->phys.origin[2] + nodes[2][2];
    if ( veh->hasGoalYaw )
    {
        AngleVectors(goalAngles, nodes[4], 0, 0);
        manualSpeed = veh->manualSpeed;
        nodes[4][0] = manualSpeed * nodes[4][0];
        nodes[4][1] = manualSpeed * nodes[4][1];
        nodes[4][2] = manualSpeed * nodes[4][2];
        nodes[4][0] = veh->goalPosition[0] + nodes[4][0];
        nodes[4][1] = veh->goalPosition[1] + nodes[4][1];
        nodes[4][2] = veh->goalPosition[2] + nodes[4][2];
    }
    phys->curveID = cCurveManager::GetFreeCurve();
    timeStep = Dvar_GetFloat("g_vehiclePlaneCurveTime");
    if ( veh->hasGoalYaw )
        phys->curveStep = timeStep + 1.2;
    else
        phys->curveStep = timeStep + 0.2;
    cCurveManager::AddNodeToCurve(phys->curveID, nodes[0], 0.0);
    cCurveManager::AddNodeToCurve(phys->curveID, nodes[1], 0.1);
    cCurveManager::AddNodeToCurve(phys->curveID, nodes[2], 0.1);
    distance[0] = (*goals)[0] - nodes[2][0];
    distance[1] = (*goals)[1] - nodes[2][1];
    distance[2] = (*goals)[2] - nodes[2][2];
    distBetweenGoals = Abs(distance);
    for ( goalIndex = 0; goalIndex < numGoals; ++goalIndex )
    {
        timeStep = (float)(distBetweenGoals / veh->manualSpeed) * tweaker;
        cCurveManager::AddNodeToCurve(phys->curveID, &(*goals)[3 * goalIndex], timeStep);
        nextGoalIndex = 3 * (goalIndex + 1);
        distance[0] = (*goals)[nextGoalIndex] - (*goals)[3 * goalIndex];
        distance[1] = (*goals)[nextGoalIndex + 1] - (*goals)[3 * goalIndex + 1];
        distance[2] = (*goals)[nextGoalIndex + 2] - (*goals)[3 * goalIndex + 2];
        distBetweenGoals = Abs(distance);
    }
    if ( veh->hasGoalYaw )
        cCurveManager::AddNodeToCurve(phys->curveID, nodes[4], 1.0);
    cCurveManager::BuildCurve(phys->curveID);
    phys->curveLength = cCurveManager::GetCurveLength(phys->curveID);
    if ( veh->hasGoalYaw )
    {
        distNodes[0] = nodes[4][0] - nodes[3][0];
        distNodes[1] = nodes[4][1] - nodes[3][1];
        distNodes[2] = nodes[4][2] - nodes[3][2];
        tailCurveLength = Abs(distNodes);
        phys->curveLength = phys->curveLength - tailCurveLength;
    }
    if ( g_vehicleDebug->current.enabled )
    {
        cCurveManager::SetCurveDraw(phys->curveID, 0);
        VEH_DebugPlaneOnCurve(ent);
    }
}

// Decomp: CoDMPServer.c:438580 (0x00680380)  CMD_VEH_SetPlaneBarrelRoll
void __cdecl CMD_VEH_SetPlaneBarrelRoll(scr_entref_t entref)
{
    float goalRollDegrees;
    scr_vehicle_s *veh;
    float yawDiff;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    veh->numRolls = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    veh->hasGoalRoll = 1.0f;
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 1 )
        veh->goalRollTime = 2.0f;
    else
        veh->goalRollTime = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    veh->currentRollTime = 0.0f;
    yawDiff = veh->phys.prevAngles[1] - veh->phys.angles[1];
    if ( yawDiff >= 0.0 )
    {
        if ( yawDiff <= 0.0 )
        {
            if ( G_irand(0, 1) )
                goalRollDegrees = (float)veh->numRolls * -360.0f;
            else
                goalRollDegrees = (float)veh->numRolls * 360.0f;
            veh->goalRoll = goalRollDegrees;
        }
        else
        {
            veh->goalRoll = (float)veh->numRolls * 360.0f;
        }
    }
    else
    {
        veh->goalRoll = (float)veh->numRolls * -360.0f;
    }
}

// Decomp: CoDOMPServer.c:438600  CMD_VEH_SetGoalYaw
void __cdecl CMD_VEH_SetGoalYaw(scr_entref_t entref)
{
    scr_vehicle_s *veh;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    veh->goalYaw = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    veh->hasGoalYaw = 1;
}

// Decomp: CoDOMPServer.c:438620  CMD_VEH_ClearGoalYaw
void __cdecl CMD_VEH_ClearGoalYaw(scr_entref_t entref)
{
    scr_vehicle_s *scr_vehicle;

    scr_vehicle = GScr_GetVehicle(entref)->scr_vehicle;
    scr_vehicle->goalYaw = 0.0f;
    scr_vehicle->hasGoalYaw = 0;
}

// Decomp: CoDOMPServer.c:438640  CMD_VEH_SetTargetYaw
void __cdecl CMD_VEH_SetTargetYaw(scr_entref_t entref)
{
    scr_vehicle_s *veh;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    veh->targetYaw = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    veh->hasTargetYaw = 1;
}

// Decomp: CoDOMPServer.c:438660  CMD_VEH_ClearTargetYaw
void __cdecl CMD_VEH_ClearTargetYaw(scr_entref_t entref)
{
    scr_vehicle_s *scr_vehicle;

    scr_vehicle = GScr_GetVehicle(entref)->scr_vehicle;
    scr_vehicle->targetYaw = 0.0f;
    scr_vehicle->hasTargetYaw = 0;
}

// Decomp: CoDOMPServer.c:438680  CMD_VEH_SetDefaultPitch
void __cdecl CMD_VEH_SetDefaultPitch(scr_entref_t entref)
{
    scr_vehicle_s *veh;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    veh->defaultPitch = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    veh->hasDefaultPitch = 1;
}

// Decomp: CoDOMPServer.c:438700  CMD_VEH_ClearDefaultPitch
void __cdecl CMD_VEH_ClearDefaultPitch(scr_entref_t entref)
{
    scr_vehicle_s *scr_vehicle;

    scr_vehicle = GScr_GetVehicle(entref)->scr_vehicle;
    scr_vehicle->defaultPitch = 0.0f;
    scr_vehicle->hasDefaultPitch = 0;
}

// Decomp: CoDOMPServer.c:438720  CMD_VEH_GetAngularVelocity
void __cdecl CMD_VEH_GetAngularVelocity(scr_entref_t entref)
{
    scr_vehicle_s *veh;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    Scr_AddVector(veh->phys.rotVel, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:438740  CMD_VEH_SetAngularVelocity
void __cdecl CMD_VEH_SetAngularVelocity(scr_entref_t entref)
{
    scr_vehicle_s *veh;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    Scr_GetVector(0, veh->phys.rotVel, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:438760  CMD_VEH_SetVehVelocity
void __cdecl CMD_VEH_SetVehVelocity(scr_entref_t entref)
{
    const char *heliOnlyErrorMsg;
    scr_vehicle_s *veh;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    if ( BG_GetVehicleInfo(veh->infoIdx)->type != 6 )
    {
        heliOnlyErrorMsg = va("SetVelocity called only be called on a helicopter\n");
        Scr_Error(heliOnlyErrorMsg, 0);
    }
    Scr_GetVector(0, veh->phys.vel, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:438780  CMD_VEH_SetTurretTargetVec
void __cdecl CMD_VEH_SetTurretTargetVec(scr_entref_t entref)
{
    const char *playerVehicleErrorMsg;
    const char *noHealthErrorMsg;
    scr_vehicle_s *veh;
    gentity_s *ent;
    float tgtPos[3];

    ent = GScr_GetVehicle(entref);
    veh = ent->scr_vehicle;
    if ( (veh->flags & 0x10) == 0 )
    {
        if ( (veh->flags & 1) != 0 )
        {
            playerVehicleErrorMsg = va("Can't set target position on player's vehicle");
            Scr_Error(playerVehicleErrorMsg, 0);
        }
        if ( (float)ent->health <= 0.0 )
        {
            noHealthErrorMsg = va("Vehicle must have health to control the turret");
            Scr_Error(noHealthErrorMsg, 0);
        }
        veh->hasTarget = 1;
        veh->targetEnt = 1023;
        Scr_GetVector(0, tgtPos, SCRIPTINSTANCE_SERVER);
        veh->targetOrigin[0] = tgtPos[0];
        veh->targetOrigin[1] = tgtPos[1];
        veh->targetOrigin[2] = tgtPos[2];
        veh->targetOffset[0] = 0.0f;
        veh->targetOffset[1] = 0.0f;
        veh->targetOffset[2] = 0.0f;
    }
}

// Decomp: CoDOMPServer.c:438820  CMD_VEH_SetTurretTargetEnt
void __cdecl CMD_VEH_SetTurretTargetEnt(scr_entref_t entref)
{
    const char *turretHealthErrorMsg;
    int number;
    scr_vehicle_s *veh;
    gentity_s *tgtEnt;
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);
    veh = ent->scr_vehicle;
    if ( (veh->flags & 0x10) == 0 )
    {
        if ( (float)ent->health <= 0.0 )
        {
            turretHealthErrorMsg = va("Vehicle must have health to control the turret");
            Scr_Error(turretHealthErrorMsg, 0);
        }
        tgtEnt = Scr_GetEntity(0);
        veh->hasTarget = 1;
        if ( tgtEnt )
            number = tgtEnt->s.number;
        else
            number = 1023;
        veh->targetEnt = number;
        if ( tgtEnt )
            tgtEnt->flags |= 0x80000u;
        if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 1 )
        {
            veh->targetOffset[0] = 0.0f;
            veh->targetOffset[1] = 0.0f;
            veh->targetOffset[2] = 0.0f;
        }
        else
        {
            Scr_GetVector(1u, veh->targetOffset, SCRIPTINSTANCE_SERVER);
        }
        veh->targetOrigin[0] = 0.0f;
        veh->targetOrigin[1] = 0.0f;
        veh->targetOrigin[2] = 0.0f;
    }
}

// Decomp: CoDOMPServer.c:438860  CMD_VEH_ClearTurretTarget
void __cdecl CMD_VEH_ClearTurretTarget(scr_entref_t entref)
{
    scr_vehicle_s *veh;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    if ( (veh->flags & 0x10) == 0 )
    {
        if ( (veh->flags & 1) == 0 )
            veh->hasTarget = 0;
        veh->targetEnt = 1023;
        veh->targetOrigin[0] = 0.0f;
        veh->targetOrigin[1] = 0.0f;
        veh->targetOrigin[2] = 0.0f;
        veh->targetOffset[0] = 0.0f;
        veh->targetOffset[1] = 0.0f;
        veh->targetOffset[2] = 0.0f;
    }
}

// Decomp: CoDOMPServer.c:438880  CMD_VEH_SetGunnerTargetVec
void __cdecl CMD_VEH_SetGunnerTargetVec(scr_entref_t entref)
{
    float *gunnerTargetOffset;
    float *gunnerTargetOrigin;
    scr_vehicle_s *veh;
    unsigned int gunnerIndex;
    float tgtPos[3];

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    gunnerIndex = 0;
    Scr_GetVector(0, tgtPos, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 1 )
        gunnerIndex = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    if ( gunnerIndex >= 4 )
        Scr_Error("Invalid gunner index set in SetGunnerTargetVec", 0);
    gunnerTargetOrigin = veh->gunnerTargets[gunnerIndex].targetOrigin;
    *gunnerTargetOrigin = tgtPos[0];
    gunnerTargetOrigin[1] = tgtPos[1];
    gunnerTargetOrigin[2] = tgtPos[2];
    gunnerTargetOffset = veh->gunnerTargets[gunnerIndex].targetOffset;
    gunnerTargetOffset[0] = 0.0f;
    gunnerTargetOffset[1] = 0.0f;
    gunnerTargetOffset[2] = 0.0f;
    veh->gunnerTargets[gunnerIndex].valid = 1;
    veh->gunnerTargets[gunnerIndex].targetEnt = 1023;
}

// Decomp: CoDOMPServer.c:438920  CMD_VEH_GetGunnerTargetVec
void __cdecl CMD_VEH_GetGunnerTargetVec(scr_entref_t entref)
{
    scr_vehicle_s *veh;
    int gunnerIndex;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    gunnerIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)gunnerIndex >= 4 )
        Scr_Error("Invalid gunner index set in GetGunnerTargetVec", 0);
    Scr_AddVector(veh->gunnerTargets[gunnerIndex].targetOrigin, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:438940  CMD_VEH_SetGunnerTargetEnt
void __cdecl CMD_VEH_SetGunnerTargetEnt(scr_entref_t entref)
{
    float *gunnerTargetOffset;
    int number;
    scr_vehicle_s *veh;
    unsigned int gunnerIndex;
    gentity_s *tgtEnt;
    float targetOffset[3];
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);
    veh = ent->scr_vehicle;
    gunnerIndex = 0;
    tgtEnt = Scr_GetEntity(0);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 1 )
        memset(targetOffset, 0, sizeof(targetOffset));
    else
        Scr_GetVector(1u, targetOffset, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 2 )
        gunnerIndex = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
    if ( gunnerIndex >= 4 )
        Scr_Error("Invalid gunner index set in SetGunnerTargetEnt", 0);
    veh->gunnerTargets[gunnerIndex].valid = 1;
    if ( tgtEnt )
        number = tgtEnt->s.number;
    else
        number = 1023;
    veh->gunnerTargets[gunnerIndex].targetEnt = number;
    gunnerTargetOffset = veh->gunnerTargets[gunnerIndex].targetOffset;
    gunnerTargetOffset[0] = targetOffset[0];
    gunnerTargetOffset[1] = targetOffset[1];
    gunnerTargetOffset[2] = targetOffset[2];
    veh->gunnerTargets[gunnerIndex].targetOrigin[0] = 0.0f;
    veh->gunnerTargets[gunnerIndex].targetOrigin[1] = 0.0f;
    veh->gunnerTargets[gunnerIndex].targetOrigin[2] = 0.0f;
}

// Decomp: CoDOMPServer.c:438980  CMD_VEH_ClearGunnerTarget
void __cdecl CMD_VEH_ClearGunnerTarget(scr_entref_t entref)
{
    unsigned int *targetOrigin;
    unsigned int *targetOffset;
    scr_vehicle_s *veh;
    unsigned int gunnerIndex;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    gunnerIndex = 0;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        gunnerIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( gunnerIndex >= 4 )
        Scr_Error("Invalid gunner index set in ClearGunnerTarget", 0);
    veh->gunnerTargets[gunnerIndex].valid = 0;
    veh->gunnerTargets[gunnerIndex].targetEnt = 1023;
    veh->gunnerTargets[gunnerIndex].targetOffset[0] = 0.0f;
    veh->gunnerTargets[gunnerIndex].targetOffset[1] = 0.0f;
    veh->gunnerTargets[gunnerIndex].targetOffset[2] = 0.0f;
    veh->gunnerTargets[gunnerIndex].targetOrigin[0] = 0.0f;
    veh->gunnerTargets[gunnerIndex].targetOrigin[1] = 0.0f;
    veh->gunnerTargets[gunnerIndex].targetOrigin[2] = 0.0f;
}

// Decomp: CoDOMPServer.c:439000  CMD_VEH_SetGunnerTurretOnTargetRange
void __cdecl CMD_VEH_SetGunnerTurretOnTargetRange(scr_entref_t entref)
{
    scr_vehicle_s *veh;
    float range;
    int gunnerIndex;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    range = 0.0f;
    gunnerIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 1 )
        range = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)gunnerIndex >= 4 )
        Scr_Error("Invalid gunner index set in SetGunnerTurretOnTargetRange", 0);
    veh->gunnerTurrets[gunnerIndex].turretOnTargetRange = range;
}

// Decomp: CoDOMPServer.c:439030  CMD_VEH_GetGunnerTargetEnt
void __cdecl CMD_VEH_GetGunnerTargetEnt(scr_entref_t entref)
{
    gentity_s *tgtEnt;
    scr_vehicle_s *veh;
    int gunnerIndex;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    gunnerIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)gunnerIndex >= 4 )
        Scr_Error("Invalid gunner index set in GetGunnerTargetEnt", 0);
    if ( veh->gunnerTargets[gunnerIndex].targetEnt == 1023
        || (tgtEnt = &g_entities[veh->gunnerTargets[gunnerIndex].targetEnt], !tgtEnt->r.inuse) )
    {
        Scr_AddUndefined(SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddEntity(tgtEnt, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDOMPServer.c:439060  CMD_VEH_GetGunnerAnimPitch
void __cdecl CMD_VEH_GetGunnerAnimPitch(scr_entref_t entref)
{
    const vehicle_info_t *info;
    int gunnerIndex;
    float pitch;
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);
    info = BG_GetVehicleInfo(ent->scr_vehicle->infoIdx);
    gunnerIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)gunnerIndex >= 4 )
        Scr_Error("Invalid gunner index set in GetGunnerAnimPitch", 0);
    pitch = AngleNormalize180(
                        (float)((float)ent->s.lerp.u.vehicle.gunnerAngles[gunnerIndex].pitch * 0.0054931641)
                    - info->gunnerRestAngles[gunnerIndex][0]);
    Scr_AddFloat(pitch, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:439090  CMD_VEH_GetGunnerAnimYaw
void __cdecl CMD_VEH_GetGunnerAnimYaw(scr_entref_t entref)
{
    const vehicle_info_t *info;
    int gunnerIndex;
    float yaw;
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);
    info = BG_GetVehicleInfo(ent->scr_vehicle->infoIdx);
    gunnerIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)gunnerIndex >= 4 )
        Scr_Error("Invalid gunner index set in GetGunnerAnimYaw", 0);
    yaw = AngleNormalize180(
                    (float)((float)ent->s.lerp.u.vehicle.gunnerAngles[gunnerIndex].yaw * 0.0054931641)
                - info->gunnerRestAngles[gunnerIndex][1]);
    Scr_AddFloat(yaw, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:439110  CMD_VEH_SetLookAtEnt
void __cdecl CMD_VEH_SetLookAtEnt(scr_entref_t entref)
{
    const char *playerVehicleErrorMsg;
    const char *noHealthErrorMsg;
    const char *invalidEntErrorMsg;
    scr_vehicle_s *veh;
    gentity_s *tgtEnt;
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);
    veh = ent->scr_vehicle;
    if ( (veh->flags & 1) != 0 )
    {
        playerVehicleErrorMsg = va("Can't set target on player's vehicle");
        Scr_Error(playerVehicleErrorMsg, 0);
    }
    if ( (float)ent->health <= 0.0 )
    {
        noHealthErrorMsg = va("Vehicle must have health to control");
        Scr_Error(noHealthErrorMsg, 0);
    }
    tgtEnt = Scr_GetEntity(0);
    if ( !tgtEnt )
    {
        invalidEntErrorMsg = va("Invalid entity");
        Scr_Error(invalidEntErrorMsg, 0);
    }
    //EntHandle::setEnt(&veh->lookAtEnt, tgtEnt);
    veh->lookAtEnt.setEnt(tgtEnt);
}

// Decomp: CoDOMPServer.c:439140  CMD_VEH_ClearLookAtEnt
void __cdecl CMD_VEH_ClearLookAtEnt(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);
    ent->scr_vehicle->lookAtEnt.setEnt(0);
}

// Decomp: CoDOMPServer.c:439160  CMD_VEH_ReturnPlayerControl
void __cdecl CMD_VEH_ReturnPlayerControl(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);
    if ( ent->r.ownerNum.isDefined() )
        VEH_CancelAIMove(ent);
    if ( ent->scr_vehicle )
        ent->scr_vehicle->flags &= 0xFFFFFE7F;
}

// Decomp: CoDMPServer.c:439139 (0x00681580)  VEH_CancelAIMove
void __cdecl VEH_CancelAIMove(gentity_s *ent)
{
    const vehicle_info_t *info;
    scr_vehicle_s *veh;

    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 4969, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->scr_vehicle
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    4970,
                    0,
                    "%s",
                    "ent->scr_vehicle") )
    {
        __debugbreak();
    }
    veh = ent->scr_vehicle;
    info = BG_GetVehicleInfo(veh->infoIdx);
    veh->flags &= ~2u;
    if ( info->type == 6 )
        HELI_CancelAIMove(ent);
}

// Decomp: CoDOMPServer.c:439180  CMD_VEH_SetWeapon
void __cdecl CMD_VEH_SetWeapon(scr_entref_t entref)
{
    char *String;
    int WeaponIndexForName;
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);
    String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    WeaponIndexForName = BG_FindWeaponIndexForName(String);
    AssignToSmallerType<unsigned short>(&ent->s.weapon, WeaponIndexForName);
    ent->s.weaponModel = 0;
}

// Decomp: CoDMPServer.c:439191 (0x006816A0)  CMD_VEH_FireWeapon
void __cdecl CMD_VEH_FireWeapon(scr_entref_t entref)
{
    const char *noHealthErrorMsg;
    char *targetNameStr;
    const char *invalidWeaponErrorMsg;
    const char *unsupportedWeaponErrorMsg;
    char *targetNameStr2;
    const char *noBarrelErrorMsg;
    unsigned int ownerEntNum;
    gentity_s *projTargetEnt;
    gentity_s *bombTargetEnt;
    gentity_s *defaultAttacker;
    float clampedPitchDiff;
    float clampedPitchPadding;
    float barrelOffsetNeg;
    float unclampedYawDiff;
    float aimPadding;
    float spread;
    gentity_s *driver;
    float curiosity[3];
    gentity_s *attacker;
    float joltDir[3];
    int numBarrels;
    int boneIndex;
    unsigned __int16 boneName;
    const vehicle_info_t *info;
    scr_vehicle_s *veh;
    float diffAngles[3];
    int event;
    gentity_s *target;
    int startBarrel;
    gentity_s *proj;
    float gunAngles[3];
    weaponParms wp;
    float targetOffset[3];
    gentity_s *ent;
    float bulletAngles[3];
    int barrelLoopIndex;
    float barrelMtx[4][3];
    int barrel;
    gentity_s *player;
    float flashMtx[4][3];

    ent = GScr_GetVehicle(entref);
    veh = ent->scr_vehicle;
    info = BG_GetVehicleInfo(veh->infoIdx);
    player = 0;
    boneIndex = 0;
    if ( (float)ent->health <= 0.0 )
    {
        noHealthErrorMsg = va("Vehicle must have health to control the turret");
        Scr_Error(noHealthErrorMsg, 0);
    }
    if ( !ent->s.weapon )
    {
        targetNameStr = SL_ConvertToString(ent->targetname, SCRIPTINSTANCE_SERVER);
        invalidWeaponErrorMsg = va("Invalid weapon specified for [%s]\n", targetNameStr);
        Scr_Error(invalidWeaponErrorMsg, 0);
    }
    Weapon_SetWeaponParamsWeapon(&wp, ent->s.weapon);
    if ( wp.weapDef->weapType
        && wp.weapDef->weapType != WEAPTYPE_PROJECTILE
        && wp.weapDef->weapType != WEAPTYPE_BOMB
        && wp.weapDef->weapType != WEAPTYPE_GAS
        && wp.weapDef->weapType != WEAPTYPE_GRENADE )
    {
        unsupportedWeaponErrorMsg = va("Vehicles only support bullet, bomb, gas, grenade and projectile weapons\n");
        Scr_Error(unsupportedWeaponErrorMsg, 0);
    }
    if ( info->type != 3 && info->type != 6 )
    {
        if ( veh->boneIndex.barrel < 0 )
        {
            targetNameStr2 = SL_ConvertToString(ent->targetname, SCRIPTINSTANCE_SERVER);
            noBarrelErrorMsg = va("No tag_barrel for [%s]\n", targetNameStr2);
            Scr_Error(noBarrelErrorMsg, 0);
        }
        G_DObjGetWorldBoneIndexMatrix(ent, veh->boneIndex.barrel, barrelMtx);
        joltDir[0] = -barrelMtx[0][0];
        joltDir[1] = -barrelMtx[0][1];
        joltDir[2] = -barrelMtx[0][2];
        VEH_JoltBody(ent, joltDir, 1.0, 0.0, 0.0);
    }
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        boneName = (unsigned __int16)Scr_GetConstLowercaseString(0, SCRIPTINSTANCE_SERVER);
    else
        boneName = 0;
    if ( !Scr_GetNumParam(SCRIPTINSTANCE_SERVER) && info->quadBarrel )
    {
        numBarrels = 2;
        startBarrel = veh->turret.fireBarrel != 0 ? 2 : 0;
        event = (veh->turret.fireBarrel != 0) + 49;
    }
    else
    {
        numBarrels = 1;
        startBarrel = 0;
        event = 29;
    }
    for ( barrelLoopIndex = 0; barrelLoopIndex < numBarrels; ++barrelLoopIndex )
    {
        barrel = barrelLoopIndex + startBarrel;
        boneIndex = VEH_GetTagBoneIndex(ent, barrelLoopIndex + startBarrel);
        if ( boneIndex < 0
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                        12004,
                        1,
                        "%s",
                        "boneIndex >= 0") )
        {
            __debugbreak();
        }
        G_DObjGetWorldBoneIndexMatrix(ent, boneIndex, flashMtx);
        //if ( EntHandle::isDefined(&ent->r.ownerNum)
        if ( ent->r.ownerNum.isDefined()
            && numBarrels == 1
            && (veh->turret.flags & 1) == 0
            && veh->targetEnt == 1023
            && wp.weapDef->weapType )
        {
            //player = EntHandle::ent(&ent->r.ownerNum);
            player = ent->r.ownerNum.ent();
            if ( !player->client
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                            12015,
                            0,
                            "%s",
                            "player->client") )
            {
                __debugbreak();
            }
            wp.gunForward[0] = veh->targetOrigin[0] - flashMtx[3][0];
            wp.gunForward[1] = veh->targetOrigin[1] - flashMtx[3][1];
            wp.gunForward[2] = veh->targetOrigin[2] - flashMtx[3][2];
            Vec3Normalize(wp.gunForward);
            vectoangles(flashMtx[0], gunAngles);
            vectoangles(wp.gunForward, bulletAngles);
            AnglesSubtract(gunAngles, bulletAngles, diffAngles);
            if ( (float)(diffAngles[0] - wp.weapDef->aimPadding) < 0.0 )
                aimPadding = diffAngles[0];
            else
                aimPadding = wp.weapDef->aimPadding;
            if ( (float)((-(wp.weapDef->aimPadding)) - diffAngles[0]) < 0.0 )
                clampedPitchPadding = aimPadding;
            else
                clampedPitchPadding = -wp.weapDef->aimPadding;
            diffAngles[0] = clampedPitchPadding;
            if ( (float)(diffAngles[1] - wp.weapDef->aimPadding) < 0.0 )
                unclampedYawDiff = diffAngles[1];
            else
                unclampedYawDiff = wp.weapDef->aimPadding;
            if ( (float)((-(wp.weapDef->aimPadding)) - diffAngles[1]) < 0.0 )
                clampedPitchDiff = unclampedYawDiff;
            else
                clampedPitchDiff = -wp.weapDef->aimPadding;
            diffAngles[1] = clampedPitchDiff;
            diffAngles[2] = 0.0f;
            AnglesSubtract(gunAngles, diffAngles, gunAngles);
            AngleVectors(gunAngles, wp.gunForward, 0, 0);
        }
        else
        {
            *(_QWORD *)wp.gunForward = *(_QWORD *)&flashMtx[0][0];
            wp.gunForward[2] = flashMtx[0][2];
        }
        vectoangles(wp.gunForward, curiosity);
        wp.forward[0] = wp.gunForward[0];
        wp.forward[1] = wp.gunForward[1];
        wp.forward[2] = wp.gunForward[2];
        *(_QWORD *)wp.right = *(_QWORD *)&flashMtx[1][0];
        wp.right[2] = flashMtx[1][2];
        *(_QWORD *)wp.up = *(_QWORD *)&flashMtx[2][0];
        wp.up[2] = flashMtx[2][2];
        if ( (veh->turret.flags & 1) != 0 )
        {
            barrelOffsetNeg = -veh->turret.barrelOffset;
            wp.muzzleTrace[0] = (float)(barrelOffsetNeg * wp.gunForward[0]) + flashMtx[3][0];
            wp.muzzleTrace[1] = (float)(barrelOffsetNeg * wp.gunForward[1]) + flashMtx[3][1];
            wp.muzzleTrace[2] = (float)(barrelOffsetNeg * wp.gunForward[2]) + flashMtx[3][2];
        }
        else
        {
            *(_QWORD *)wp.muzzleTrace = *(_QWORD *)&flashMtx[3][0];
            wp.muzzleTrace[2] = flashMtx[3][2];
        }
        driver = VEH_GetSeatOccupantEntity(ent, 0);
        if ( driver )
            defaultAttacker = driver;
        else
            defaultAttacker = ent;
        attacker = defaultAttacker;
        if ( defaultAttacker && attacker->client )
            spread = wp.weapDef->playerSpread;
        else
            spread = wp.weapDef->aiSpread;
        if ( wp.weapDef->weapType == WEAPTYPE_BULLET )
        {
            Bullet_Fire(attacker, spread, &wp, ent, level.time);
            continue;
        }
        if ( wp.weapDef->weapType == WEAPTYPE_BOMB )
        {
            if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 1 )
                bombTargetEnt = 0;
            else
                bombTargetEnt = Scr_GetEntity(1u);
            target = bombTargetEnt;
            if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 2 )
                memset(targetOffset, 0, sizeof(targetOffset));
            else
                Scr_GetVector(2u, targetOffset, SCRIPTINSTANCE_SERVER);
            proj = Weapon_BombDrop_Fire(
                             attacker,
                             ent->s.weapon,
                             wp.weapDef->fAdsSpread,
                             &wp,
                             veh->phys.vel,
                             target,
                             targetOffset);
            if ( !proj
                && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 12085, 0, "%s", "proj") )
            {
                __debugbreak();
            }
            Scr_AddEntity(proj, SCRIPTINSTANCE_SERVER);
            continue;
        }
        if ( wp.weapDef->weapType != WEAPTYPE_GAS )
        {
            if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 1 )
                projTargetEnt = 0;
            else
                projTargetEnt = Scr_GetEntity(1u);
            target = projTargetEnt;
            if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 2 )
                memset(targetOffset, 0, sizeof(targetOffset));
            else
                Scr_GetVector(2u, targetOffset, SCRIPTINSTANCE_SERVER);
            proj = Weapon_RocketLauncher_Fire(
                             attacker,
                             ent->s.weapon,
                             wp.weapDef->fAdsSpread,
                             &wp,
                             veh->phys.vel,
                             target,
                             targetOffset);
            if ( !proj
                && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 12105, 0, "%s", "proj") )
            {
                __debugbreak();
            }
            Scr_AddEntity(proj, SCRIPTINSTANCE_SERVER);
            continue;
        }
        veh->flags |= 0x40u;
        ent->s.lerp.eFlags |= 0x40u;
        if ( attacker->client )
            attacker->client->ps.eFlags |= 0x40u;
        Weapon_Flamethrower_Fire(attacker, &wp);
    }
    if ( info->type == 6 )
    {
        G_AddEvent(ent, 0xADu, 0x7FEu);
    }
    else
    {
        //v7 = EntHandle::entnum(&ent->r.ownerNum);
        G_AddEvent(ent, 0xADu, ent->r.ownerNum.entnum());
    }
    Scr_Notify(ent, scr_const.weapon_fired, 0);
    veh->turret.fireTime = wp.weapDef->iFireTime;
    veh->turret.fireBarrel = veh->turret.fireBarrel == 0;
}

// Decomp: CoDMPServer.c:439509 (0x00682310)  VEH_GetTagBoneIndex
int __cdecl VEH_GetTagBoneIndex(gentity_s *ent, int barrel)
{
    VariableUnion boneNameConst;
    char *entTargetName;
    const char *missingTagErrorMsg;
    char *flashTagName;
    const char *missingFlashErrorMsg;
    char *vehicleTargetName;
    char *boneName;
    int boneIndex;
    scr_vehicle_s *veh;

    boneIndex = -1;
    veh = ent->scr_vehicle;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        boneName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        if ( *boneName )
        {
            boneNameConst.intValue = Scr_GetConstLowercaseString(0, SCRIPTINSTANCE_SERVER);
            boneIndex = SV_DObjGetBoneIndex(ent, boneNameConst.stringValue);
            if ( boneIndex < 0 )
            {
                entTargetName = SL_ConvertToString(ent->targetname, SCRIPTINSTANCE_SERVER);
                missingTagErrorMsg = va("No tag %s for [%s]\n", boneName, entTargetName);
                Scr_Error(missingTagErrorMsg, 0);
            }
        }
        else
        {
            boneIndex = -1;
        }
    }
    if ( !Scr_GetNumParam(SCRIPTINSTANCE_SERVER) || boneIndex < 0 )
    {
        if ( veh->boneIndex.flash[barrel] < 0 )
        {
            vehicleTargetName = SL_ConvertToString(ent->targetname, SCRIPTINSTANCE_SERVER);
            flashTagName = SL_ConvertToString(*s_flashTags[barrel], SCRIPTINSTANCE_SERVER);
            missingFlashErrorMsg = va("No %s for [%s]\n", flashTagName, vehicleTargetName);
            Scr_Error(missingFlashErrorMsg, 0);
        }
        else
        {
            return veh->boneIndex.flash[barrel];
        }
    }
    return boneIndex;
}

// Decomp: CoDOMPServer.c:439540  CMD_VEH_FireGunnerWeapon
void __cdecl CMD_VEH_FireGunnerWeapon(scr_entref_t entref)
{
    VariableUnion gunnerIndex;
    gentity_s *proj;
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);
    gunnerIndex.intValue = 0;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        gunnerIndex.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( gunnerIndex.intValue >= 4u )
        Scr_Error("Invalid gunner index set in FireGunnerWeapon", 0);
    proj = VEH_FireGunnerWeapon(ent, gunnerIndex.intValue, 0);
    if ( proj )
        Scr_AddEntity(proj, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:439570  CMD_VEH_StopFireWeapon
void __cdecl CMD_VEH_StopFireWeapon(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GScr_GetVehicle(entref);
    ent->s.lerp.eFlags &= ~0x40u;
    ent->scr_vehicle->flags &= ~0x40u;
}

// Decomp: CoDOMPServer.c:439590  CMD_VEH_IsTurretReady
void __cdecl CMD_VEH_IsTurretReady(scr_entref_t entref)
{
    const char *notPlayerVehicleErrorMsg;
    scr_vehicle_s *veh;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    if ( (veh->flags & 1) == 0 )
    {
        notPlayerVehicleErrorMsg = va("Must be called on a player controlled vehicle");
        Scr_Error(notPlayerVehicleErrorMsg, 0);
    }
    if ( veh->turret.fireTime > 0 )
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:439620  CMD_VEH_ForceMaterialSpeed
void __cdecl CMD_VEH_ForceMaterialSpeed(scr_entref_t entref)
{
    scr_vehicle_s *veh;
    float speed;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    if ( !veh && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 12231, 0, "%s", "veh") )
        __debugbreak();
    if ( Scr_GetInt(0, SCRIPTINSTANCE_SERVER) )
    {
        speed = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
        veh->flags |= 4u;
        veh->forcedMaterialSpeed = speed;
    }
    else
    {
        veh->flags &= ~4u;
    }
}

// Decomp: CoDOMPServer.c:439650  CMD_VEH_SetMaxSpeed
void __cdecl CMD_VEH_SetMaxSpeed(scr_entref_t entref)
{
    float max_speed_scale;
    scr_vehicle_s *veh;
    float speed;

    veh = GScr_GetVehicle(entref)->scr_vehicle;
    if ( !veh && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp", 12266, 0, "%s", "veh") )
        __debugbreak();
    speed = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    veh->maxSpeedOverride = speed * 17.6;
    if ( veh->nitrousVehicle )
    {
        if ( onlinegame->current.enabled )
            Scr_Error("Can not use SetVehMaxSpeed in online games for physics vehicles.", 0);
        if ( speed <= 0.0 )
        {
            veh->nitrousVehicle->m_speed_scale = 1.0f;
        }
        else
        {
            //max_speed_scale = veh->maxSpeedOverride / NitrousVehicle::get_max_speed(veh->nitrousVehicle, 1);
            max_speed_scale = veh->maxSpeedOverride / veh->nitrousVehicle->get_max_speed(true);
            veh->nitrousVehicle->m_speed_scale = max_speed_scale;
        }
    }
}

// Decomp: CoDOMPServer.c:439680  CMD_VEH_GetOccupants
void __cdecl CMD_VEH_GetOccupants(scr_entref_t entref)
{
    gentity_s *occupant;
    gentity_s *ent;
    int seatIndex;

    ent = GScr_GetVehicle(entref);
    if ( !ent->scr_vehicle
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    12308,
                    0,
                    "%s",
                    "ent->scr_vehicle") )
    {
        __debugbreak();
    }
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for ( seatIndex = 0; seatIndex < 11; ++seatIndex )
    {
        occupant = VEH_GetSeatOccupantEntity(ent, seatIndex);
        if ( occupant )
        {
            Scr_AddEntity(occupant, SCRIPTINSTANCE_SERVER);
            Scr_AddArray(SCRIPTINSTANCE_SERVER);
        }
    }
}

// Decomp: CoDOMPServer.c:439710  CMD_VEH_GetSeatOccupant
void __cdecl CMD_VEH_GetSeatOccupant(scr_entref_t entref)
{
    gentity_s *occupant;
    gentity_s *vehicle;
    int seatIndex;

    vehicle = GScr_GetVehicle(entref);
    seatIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    occupant = VEH_GetSeatOccupantEntity(vehicle, seatIndex);
    if ( occupant )
        Scr_AddEntity(occupant, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:439740  CMD_VEH_GetOccupantSeat
void __cdecl CMD_VEH_GetOccupantSeat(scr_entref_t entref)
{
    gentity_s *vehicle;
    gentity_s *player;
    int seatIndex;

    vehicle = GScr_GetVehicle(entref);
    player = Scr_GetEntity(0);
    seatIndex = G_GetVehicleSeatPlayerOccupies(vehicle, player);
    if ( seatIndex >= 0 )
        Scr_AddInt(seatIndex, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:439770  CMD_VEH_SetSeatOccupied
void __cdecl CMD_VEH_SetSeatOccupied(scr_entref_t entref)
{
    int occupied;
    gentity_s *vehicle;
    int seatIndex;
    VehicleSeat *seat;

    vehicle = GScr_GetVehicle(entref);
    seatIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    occupied = 1;
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 1 )
        occupied = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    seat = VEH_GetSeat(vehicle, seatIndex);
    if ( !seat )
        Scr_Error("Invalid seat specified for vehicle in SetSeatOccupied.", 0);
    if ( occupied )
    {
        if ( seat->_occupantEntNum != 1023 )
            Scr_Error("SetSeatOccupied already has an occupant so you can't call set occupied.", 0);
        seat->_occupantEntNum = 1022;
    }
    else if ( seat->_occupantEntNum == 1022 )
    {
        seat->_occupantEntNum = 1023;
    }
    else
    {
        Scr_Error("SetSeatOccupied is either occupied by a player or not set occupied by the script.", 0);
    }
}

// Decomp: CoDOMPServer.c:439800  CMD_VEH_SeatGetWeapon
void __cdecl CMD_VEH_SeatGetWeapon(scr_entref_t entref)
{
    char *weaponName;
    unsigned int weapon;
    gentity_s *vehicle;
    int seatIndex;

    vehicle = GScr_GetVehicle(entref);
    seatIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    weapon = VEH_GetSeatWeaponIndex(vehicle, seatIndex);
    if ( weapon )
    {
        weaponName = (char *)BG_WeaponName(weapon);
        Scr_AddString(weaponName, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddUndefined(SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDOMPServer.c:439830  CMD_VEH_GetSeatFiringOrigin
void __cdecl CMD_VEH_GetSeatFiringOrigin(scr_entref_t entref)
{
    const char *noFiringOriginErrorMsg;
    float origin[3];
    gentity_s *vehicle;
    float angles[3];
    int seatIndex;

    vehicle = GScr_GetVehicle(entref);
    seatIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( VEH_GetSeatFiringOriginAngles(vehicle, seatIndex, origin, angles) )
    {
        Scr_AddVector(origin, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        noFiringOriginErrorMsg = va("Can't get firing origin for a seat that has no weapon or no flash tag (seat %i).", seatIndex);
        Scr_Error(noFiringOriginErrorMsg, 0);
    }
}

// Decomp: CoDOMPServer.c:439860  CMD_VEH_GetSeatFiringAngles
void __cdecl CMD_VEH_GetSeatFiringAngles(scr_entref_t entref)
{
    const char *noFiringAnglesErrorMsg;
    float origin[3];
    gentity_s *vehicle;
    float angles[3];
    int seatIndex;

    vehicle = GScr_GetVehicle(entref);
    seatIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( VEH_GetSeatFiringOriginAngles(vehicle, seatIndex, origin, angles) )
    {
        Scr_AddVector(angles, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        noFiringAnglesErrorMsg = va("Can't get firing angles for a seat that has no weapon or no flash tag (seat %i).", seatIndex);
        Scr_Error(noFiringAnglesErrorMsg, 0);
    }
}

// Decomp: CoDOMPServer.c:439890  CMD_VEH_IsGunnerFiring
void __cdecl CMD_VEH_IsGunnerFiring(scr_entref_t entref)
{
    gentity_s *vehicle;
    bool firing;

    vehicle = GScr_GetVehicle(entref);
    firing = vehicle->scr_vehicle->gunnerTurrets[Scr_GetInt(0, SCRIPTINSTANCE_SERVER)].fireTime > 0;
    Scr_AddBool(firing, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:439910  CMD_VEH_DisableGunnerFiring
void __cdecl CMD_VEH_DisableGunnerFiring(scr_entref_t entref)
{
    unsigned int gunnerTurretFlags;
    int gunnerIndex;
    gentity_s *vehicle;

    vehicle = GScr_GetVehicle(entref);
    gunnerIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( Scr_GetInt(1u, SCRIPTINSTANCE_SERVER) )
        gunnerTurretFlags = vehicle->scr_vehicle->gunnerTurrets[gunnerIndex].flags | 8;
    else
        gunnerTurretFlags = vehicle->scr_vehicle->gunnerTurrets[gunnerIndex].flags & 0xFFFFFFF7;
    vehicle->scr_vehicle->gunnerTurrets[gunnerIndex].flags = gunnerTurretFlags;
}

// Decomp: CoDOMPServer.c:439940  CMD_VEH_GetTreadHealth
void __cdecl CMD_VEH_GetTreadHealth(scr_entref_t entref)
{
    unsigned __int16 right_tread;
    int treadHealth;
    int leftRightIndex;
    gentity_s *vehicle;

    vehicle = GScr_GetVehicle(entref);
    if ( vehicle->destructible
        && ((leftRightIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER)) != 0
            ? (right_tread = scr_const.right_tread)
            : (right_tread = scr_const.left_tread),
                GetPieceHealth(vehicle->destructible, right_tread, &treadHealth)) )
    {
        Scr_AddInt(treadHealth, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(vehicle->health, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDOMPServer.c:439970  CMD_VEH_finishVehicleDamage
void __cdecl CMD_VEH_finishVehicleDamage(scr_entref_t entref)
{
    unsigned __int16 hitLocString;
    float damageAttacker;
    gentity_s *occupant;
    int seatIndex;
    gentity_s *attacker;
    int damage;
    int damagea;
    meansOfDeath_t mod;
    char *weaponName;
    float *dir;
    float localdir[3];
    float damageDir[3];
    float damagePoint[3];
    gentity_s *unusedBulletHit;
    int iWeapon;
    int psTimeOffset;
    gentity_s *self;
    int dflags;
    int isBulletWeapon;
    int damageTeamates;
    int damageFromUnderneath;
    gentity_s *inflictor;
    hitLocation_t hitLoc;
    void (__cdecl *die)(gentity_s *, gentity_s *, gentity_s *, int, int, const int, const float *, const hitLocation_t, int);
    void (__cdecl *pain)(gentity_s *, gentity_s *, int, const float *, const int, const float *, const hitLocation_t, const int);
    int originalDamage;
    unsigned int modelIndex;
    unsigned int partName;
    const float *point;

    self = GScr_GetVehicle(entref);
    inflictor = &g_entities[1022];
    attacker = &g_entities[1022];
    dir = 0;
    point = 0;
    isBulletWeapon = 0;
    unusedBulletHit = 0;
    damage = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
    if ( damage > 0 )
    {
        if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) && Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) == 19 )
            inflictor = Scr_GetEntity(0);
        if ( Scr_GetType(1u, SCRIPTINSTANCE_SERVER) && Scr_GetPointerType(1u, SCRIPTINSTANCE_SERVER) == 19 )
            attacker = Scr_GetEntity(1u);
        dflags = Scr_GetInt(3u, SCRIPTINSTANCE_SERVER);
        mod = (meansOfDeath_t)G_MeansOfDeathFromScriptParam(4u);
        weaponName = Scr_GetString(5u, SCRIPTINSTANCE_SERVER);
        iWeapon = G_GetWeaponIndexForName(weaponName);
        if ( Scr_GetType(6u, SCRIPTINSTANCE_SERVER) )
        {
            Scr_GetVector(6u, damagePoint, SCRIPTINSTANCE_SERVER);
            point = damagePoint;
        }
        if ( Scr_GetType(7u, SCRIPTINSTANCE_SERVER) )
        {
            Scr_GetVector(7u, damageDir, SCRIPTINSTANCE_SERVER);
            dir = damageDir;
        }
        hitLocString = (unsigned __int16)Scr_GetConstString(8u, SCRIPTINSTANCE_SERVER);
        hitLoc = (hitLocation_t)G_GetHitLocationIndexFromString(hitLocString);
        psTimeOffset = Scr_GetInt(9u, SCRIPTINSTANCE_SERVER);
        damageFromUnderneath = Scr_GetInt(0xAu, SCRIPTINSTANCE_SERVER);
        modelIndex = Scr_GetInt(0xBu, SCRIPTINSTANCE_SERVER);
        partName = Scr_GetInt(0xCu, SCRIPTINSTANCE_SERVER);
        damageTeamates = Scr_GetInt(0xDu, SCRIPTINSTANCE_SERVER);
        if ( dir )
            Vec3NormalizeTo(dir, localdir);
        else
            memset(localdir, 0, sizeof(localdir));
        originalDamage = damage;
        damageAttacker = (float)damage;
        damagea = (int)(Scr_Vehicle_DamageScale(
                                            self,
                                            attacker,
                                            inflictor,
                                            point,
                                            mod,
                                            iWeapon,
                                            &damageFromUnderneath)
                                    * damageAttacker);
        if ( damagea && (dflags & 1) == 0 && self->destructible )
            damagea = DestructibleDamage(self, attacker, dir, point, damagea, mod, modelIndex, partName);
        if ( self->health > 0 )
        {
            if ( g_debugDamage->current.enabled )
                Com_Printf(15, "target:%i health:%i damage:%i\n", self->s.number, self->health, damagea);
            self->health -= damagea;
            if ( damageTeamates && self->health <= 0 )
            {
                for ( seatIndex = 0; seatIndex < 11; ++seatIndex )
                {
                    occupant = VEH_GetSeatOccupantEntity(self->scr_vehicle, seatIndex);
                    if ( occupant != attacker )
                    {
                        if ( occupant )
                            VEH_UnlinkPlayer(occupant, 0, (char*)"Ejecting teammates");
                    }
                }
            }
            if ( mod == MOD_PISTOL_BULLET || mod == MOD_RIFLE_BULLET )
                isBulletWeapon = 1;
            G_DamageNotify(
                scr_const.damage,
                self,
                attacker,
                dir,
                (float *)point,
                damagea,
                mod,
                dflags,
                modelIndex,
                partName,
                weaponName);
            if ( self->health > 0 )
            {
                pain = entityHandlers[self->handler].pain;
                if ( pain )
                    pain(self, attacker, originalDamage, point, mod, localdir, hitLoc, iWeapon);
            }
            else
            {
                if ( self->health < -999 )
                    self->health = -999;
                Scr_AddString(weaponName, SCRIPTINSTANCE_SERVER);
                Scr_AddBool(damageFromUnderneath, SCRIPTINSTANCE_SERVER);
                Scr_AddEntity(attacker, SCRIPTINSTANCE_SERVER);
                Scr_Notify(self, scr_const.death, 3u);
                die = entityHandlers[self->handler].die;
                if ( die )
                    die(self, inflictor, attacker, damagea, mod, iWeapon, localdir, hitLoc, psTimeOffset);
            }
        }
    }
}

// Decomp: CoDOMPServer.c:440080  CMD_VEH_finishVehicleRadiusDamage
void __cdecl CMD_VEH_finishVehicleRadiusDamage(scr_entref_t entref)
{
    gentity_s *attacker;
    int damage;
    meansOfDeath_t mod;
    char *weaponName;
    float *coneDirection;
    float localdir[3];
    float coneDir[3];
    float explosionPoint[3];
    gentity_s *unusedBulletHit;
    float radius;
    int iWeapon;
    int psTimeOffset;
    float fOuterDamage;
    gentity_s *self;
    int dflags;
    float fConeAngleCos;
    float damageScale;
    gentity_s *inflictor;
    const float *point;
    float fInnerDamage;

    self = GScr_GetVehicle(entref);
    inflictor = &g_entities[1022];
    attacker = &g_entities[1022];
    coneDirection = 0;
    point = 0;
    unusedBulletHit = 0;
    damage = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
    if ( damage > 0 )
    {
        if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) && Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) == 19 )
            inflictor = Scr_GetEntity(0);
        if ( Scr_GetType(1u, SCRIPTINSTANCE_SERVER) && Scr_GetPointerType(1u, SCRIPTINSTANCE_SERVER) == 19 )
            attacker = Scr_GetEntity(1u);
        fInnerDamage = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
        fOuterDamage = Scr_GetFloat(4u, SCRIPTINSTANCE_SERVER);
        dflags = Scr_GetInt(5u, SCRIPTINSTANCE_SERVER);
        mod = (meansOfDeath_t)G_MeansOfDeathFromScriptParam(6u);
        weaponName = Scr_GetString(7u, SCRIPTINSTANCE_SERVER);
        iWeapon = G_GetWeaponIndexForName(weaponName);
        if ( Scr_GetType(8u, SCRIPTINSTANCE_SERVER) )
        {
            Scr_GetVector(8u, explosionPoint, SCRIPTINSTANCE_SERVER);
            point = explosionPoint;
        }
        radius = Scr_GetFloat(9u, SCRIPTINSTANCE_SERVER);
        fConeAngleCos = Scr_GetFloat(0xAu, SCRIPTINSTANCE_SERVER);
        if ( Scr_GetType(0xBu, SCRIPTINSTANCE_SERVER) )
        {
            Scr_GetVector(0xBu, coneDir, SCRIPTINSTANCE_SERVER);
            Vec3Normalize(coneDir);
            coneDirection = coneDir;
        }
        psTimeOffset = Scr_GetInt(0xCu, SCRIPTINSTANCE_SERVER);
        damageScale = CanDamage(self, inflictor, explosionPoint, fConeAngleCos, coneDirection, 8396819);
        if ( damageScale > 0.0 )
        {
            localdir[0] = self->r.currentOrigin[0] - explosionPoint[0];
            localdir[1] = self->r.currentOrigin[1] - explosionPoint[1];
            localdir[2] = self->r.currentOrigin[2] - explosionPoint[2];
            if ( self->destructible )
                damage = (int)DestructibleRadiusDamage(self, explosionPoint, fInnerDamage, fOuterDamage, radius, mod, attacker);
            G_Damage(
                self,
                inflictor,
                attacker,
                localdir,
                explosionPoint,
                (int)(float)((float)damage * damageScale),
                dflags,
                mod,
                iWeapon,
                HITLOC_NONE,
                0,
                0,
                0);
        }
    }
}

// Decomp: CoDOMPServer.c:440150  CMD_VEH_IsVehicleImmuneToDamage
void __cdecl CMD_VEH_IsVehicleImmuneToDamage(scr_entref_t entref)
{
    char *String;
    meansOfDeath_t mod;
    unsigned int iWeapon;
    gentity_s *vehicle;
    char dflags;

    vehicle = GScr_GetVehicle(entref);
    dflags = (unsigned __int8)Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    mod = (meansOfDeath_t)G_MeansOfDeathFromScriptParam(1u);
    String = Scr_GetString(2u, SCRIPTINSTANCE_SERVER);
    iWeapon = G_GetWeaponIndexForName(String);
    if ( G_IsVehicleImmune(vehicle, mod, dflags, iWeapon) )
        Scr_AddBool(1u, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddBool(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:440180  CMD_VEH_IsBoosting
void __cdecl CMD_VEH_IsBoosting(scr_entref_t entref)
{
    bool isBoosting;
    scr_vehicle_s *veh;

    isBoosting = 0;
    veh = GScr_GetVehicle(entref)->scr_vehicle;
    if ( veh )
        isBoosting = veh->nitrousVehicle != 0;
    Scr_AddBool(isBoosting, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDOMPServer.c:440210  CMD_VEH_GetBoost
void __cdecl CMD_VEH_GetBoost(scr_entref_t entref)
{
    NitrousVehicle *nitVeh;
    scr_vehicle_s *veh;
    float boost;
    gentity_s *self;

    boost = 0.0f;
    self = GScr_GetVehicle(entref);
    if ( self )
    {
        veh = self->scr_vehicle;
        if ( veh )
        {
            nitVeh = veh->nitrousVehicle;
            if ( nitVeh )
                boost = nitVeh->m_boost_time_pool / nitVeh->m_vehicle_info->boostDuration;
        }
    }
    Scr_AddFloat(boost, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:440240 (0x00683600)  G_VehSetClientSideHealthPercentageEntity
void __cdecl G_VehSetClientSideHealthPercentageEntity(gentity_s *const ent, float percentage)
{
    VehSetClientSideHealth(ent, percentage, 63, 0);
}

// Decomp: CoDOMPServer.c:440260  VehSetClientSideHealth
void __cdecl VehSetClientSideHealth(gentity_s *const ent, float percentage, int mask, char shift)
{
    float clampedPct;
    float cappedPct;

    if ( (float)(percentage - 1.0) < 0.0 )
        cappedPct = percentage;
    else
        cappedPct = 1.0f;
    if ( (float)(0.0 - percentage) < 0.0 )
        clampedPct = cappedPct;
    else
        clampedPct = 0.0f;
    VehSetClientSideTime2Val(ent, (int)(float)(clampedPct * 63.0), mask, shift);
}

// Decomp: CoDOMPServer.c:440290  VehSetClientSideTime2Val
void __cdecl VehSetClientSideTime2Val(gentity_s *const ent, int val, int mask, char shift)
{
    ent->s.time2 = mask & (val << shift) | ent->s.time2 & ~mask;
}

void __cdecl G_VehSetClientSideEngineVolume(gentity_s *const ent, float volume)
{
    int val;
    float clamped;

    if ( volume < 0.0f )
        clamped = 0.0f;
    else if ( volume > 1.0f )
        clamped = 1.0f;
    else
        clamped = volume;
    val = (int)(clamped * 63.0f);
    VehSetClientSideTime2Val(ent, val, 0xFC000000, 26);
}

// Decomp: CoDOMPServer.c:440310  G_VehSetClientSideHealthPercentageLeftTread
void __cdecl G_VehSetClientSideHealthPercentageLeftTread(gentity_s *const ent, float percentage)
{
    VehSetClientSideHealth(ent, percentage, 4032, 6);
}

// Decomp: CoDOMPServer.c:440330  G_VehSetClientSideHealthPercentageRightTread
void __cdecl G_VehSetClientSideHealthPercentageRightTread(gentity_s *const ent, float percentage)
{
    VehSetClientSideHealth(ent, percentage, 258048, 12);
}

// Decomp: CoDOMPServer.c:440350  G_VehSetClientSideSeatOccupancyFlags
void __cdecl G_VehSetClientSideSeatOccupancyFlags(gentity_s *const ent, int occupancy_flags)
{
    VehSetClientSideTime2Val(ent, occupancy_flags, 786432, 18);
}

const int yawturn_shifts[2] = { 20, 22 };
const int yawturn_masks_0[2] = { 1048576, 4194304 };
const int pitchturn_shifts[2] = { 21, 23 };
const int pitchturn_masks_0[2] = { 2097152, 8388608 };
const int overheating_shifts_0[2] = { 24, 25 };
const int overheating_masks_0[2] = { 16777216, 33554432 };

// Decomp: CoDOMPServer.c:440370  G_VehSetClientSideGunTurningYaw
void __cdecl G_VehSetClientSideGunTurningYaw(gentity_s *const ent, unsigned int seatIndex, bool turning)
{
    if ( seatIndex >= 2
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    13024,
                    0,
                    "%s",
                    "seatIndex >= 0 && seatIndex < ( sizeof( _yawturn_shifts ) / sizeof ( _yawturn_shifts[ 0 ] ) )") )
    {
        __debugbreak();
    }
    VehSetClientSideGunTurnRate(ent, (float)turning, yawturn_masks_0[seatIndex], yawturn_shifts[seatIndex]);
}

// Decomp: CoDOMPServer.c:440390  VehSetClientSideGunTurnRate
void __cdecl VehSetClientSideGunTurnRate(gentity_s *const ent, float percentage, int mask, char shift)
{
    if ( percentage == 0.0 )
        VehSetClientSideTime2Val(ent, 0, mask, shift);
    else
        VehSetClientSideTime2Val(ent, 1, mask, shift);
}

// Decomp: CoDOMPServer.c:440410  G_VehSetClientSideGunTurningPitch
void __cdecl G_VehSetClientSideGunTurningPitch(gentity_s *const ent, unsigned int seatIndex, bool turning)
{
    if ( seatIndex >= 2
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    13030,
                    0,
                    "%s",
                    "seatIndex >= 0 && seatIndex < ( sizeof( _pitchturn_shifts ) / sizeof ( _pitchturn_shifts[ 0 ] ) )") )
    {
        __debugbreak();
    }
    VehSetClientSideGunTurnRate(ent, (float)turning, pitchturn_masks_0[seatIndex], pitchturn_shifts[seatIndex]);
}

// Decomp: CoDOMPServer.c:440430  G_VehSetClientSideGunOverheating
void __cdecl G_VehSetClientSideGunOverheating(gentity_s *const ent, unsigned int seatIndex, bool overheating)
{
    if ( seatIndex >= 2
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_vehicle.cpp",
                    13036,
                    0,
                    "%s",
                    "seatIndex >= 0 && seatIndex < ( sizeof( _overheating_shifts ) / sizeof ( _overheating_shifts[ 0 ] ) )") )
    {
        __debugbreak();
    }
    VehSetClientSideTime2Val(ent, overheating, overheating_masks_0[seatIndex], overheating_shifts_0[seatIndex]);
}
