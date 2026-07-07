#include "g_scr_helicopter.h"
#include <clientscript/cscr_vm.h>
#include <qcommon/cm_load.h>
#include <game/g_utils_wrapper.h>
#include <cgame/cg_scr_main.h>
#include <qcommon/dobj_management.h>
#include <bgame/bg_weapons_def.h>
#include <universal/q_shared.h>
#include <cstring>

static const float kAngleShortToDegrees = 0.0054931641f;

// scr_vehicle_s::flags bit — set by setheliheightlock, consumed in g_helicopter1 / Scr_Vehicle_Think.
static const unsigned int kScrVehicleHeliHeightLockFlag = 0x200u;

// BuiltinMethodDef — CoDMPServer.c ~31418, 27 entries (0x1B).
const BuiltinMethodDef s_methods[27] =
{
  { "freehelicopter", &CMD_Heli_FreeHelicopter, 0 },
  { "setspeed", &CMD_VEH_SetSpeed, 0 },
  { "getspeed", &CMD_VEH_GetSpeed, 0 },
  { "getspeedmph", &CMD_VEH_GetSpeedMPH, 0 },
  { "resumespeed", &CMD_VEH_ResumeSpeed, 0 },
  { "setyawspeed", &CMD_VEH_SetYawSpeed, 0 },
  { "setmaxpitchroll", &CMD_VEH_SetMaxPitchRoll, 0 },
  { "setturningability", &CMD_VEH_SetTurningAbility, 0 },
  { "setairresistance", &CMD_VEH_SetAirResitance, 0 },
  { "sethoverparams", &CMD_VEH_SetHoverParams, 0 },
  { "setneargoalnotifydist", &CMD_VEH_NearGoalNotifyDist, 0 },
  { "setvehgoalpos", &CMD_VEH_SetGoalPos, 0 },
  { "setgoalyaw", &CMD_VEH_SetGoalYaw, 0 },
  { "cleargoalyaw", &CMD_VEH_ClearGoalYaw, 0 },
  { "settargetyaw", &CMD_VEH_SetTargetYaw, 0 },
  { "cleartargetyaw", &CMD_VEH_ClearTargetYaw, 0 },
  { "setlookatent", &CMD_VEH_SetLookAtEnt, 0 },
  { "clearlookatent", &CMD_VEH_ClearLookAtEnt, 0 },
  { "setvehweapon", &CMD_VEH_SetWeapon, 0 },
  { "fireweapon", &CMD_VEH_FireWeapon, 0 },
  { "stopfireweapon", &CMD_VEH_StopFireWeapon, 0 },
  { "setturrettargetvec", &CMD_VEH_SetTurretTargetVec, 0 },
  { "setturrettargetent", &CMD_VEH_SetTurretTargetEnt, 0 },
  { "clearturrettarget", &CMD_VEH_ClearTurretTarget, 0 },
  { "setdamagestage", &CMD_Heli_SetDamageStage, 0 },
  { "setheliheightlock", &CMD_Heli_SetHeliHeightLock, 0 },
  { "isinsideheliheightlock", &CMD_Heli_IsInsideHeliHeightLock, 0 }
};

heli_height_lock_patches_t heli_height_lock_patches[32];

void __cdecl CMD_Heli_FreeHelicopter(scr_entref_t entref)
{
    gentity_s *heliEnt = GScr_GetVehicle(entref);
    G_FreeVehicle(heliEnt);
}

void __cdecl CMD_Heli_SetDamageStage(scr_entref_t entref)
{
    VariableUnion damageStage;
    gentity_s *heliEnt = GScr_GetVehicle(entref);
    damageStage.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    AssignToSmallerType<unsigned char>(&heliEnt->s.un1.scale, damageStage.intValue);
}

void __cdecl CMD_Heli_SetHeliHeightLock(scr_entref_t entref)
{
    gentity_s *heliEnt = GScr_GetVehicle(entref);
    if ( Scr_GetInt(0, SCRIPTINSTANCE_SERVER) )
        heliEnt->scr_vehicle->flags |= kScrVehicleHeliHeightLockFlag;
    else
        heliEnt->scr_vehicle->flags &= ~kScrVehicleHeliHeightLockFlag;
}

void __cdecl CMD_Heli_IsInsideHeliHeightLock(scr_entref_t entref)
{
    float minPlanarMargin;
    float clearanceAfterYMin;
    float clearanceX;
    float meshMaxs[3];
    float meshMins[3];
    int patchIndex;
    gentity_s *heliEnt;
    float *heliOrigin;

    heliEnt = GScr_GetVehicle(entref);
    if (!heliEnt && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_helicopter.cpp", 88, 0, "%s", "ent"))
        __debugbreak();
    if (!heliEnt->scr_vehicle
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_helicopter.cpp",
            89,
            0,
            "%s",
            "ent->scr_vehicle"))
    {
        __debugbreak();
    }
    heliOrigin = heliEnt->scr_vehicle->phys.origin;
    for ( patchIndex = 0; ; ++patchIndex )
    {
        if (patchIndex >= num_heli_height_lock_patches)
        {
            Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
            return;
        }
        CM_ModelBounds(heli_height_lock_patches[patchIndex].brushmodel, meshMins, meshMaxs);
        meshMins[0] = meshMins[0] + heli_height_lock_patches[patchIndex].origin[0];
        meshMins[1] = meshMins[1] + heli_height_lock_patches[patchIndex].origin[1];
        meshMins[2] = meshMins[2] + heli_height_lock_patches[patchIndex].origin[2];
        meshMaxs[0] = meshMaxs[0] + heli_height_lock_patches[patchIndex].origin[0];
        meshMaxs[1] = meshMaxs[1] + heli_height_lock_patches[patchIndex].origin[1];
        meshMaxs[2] = meshMaxs[2] + heli_height_lock_patches[patchIndex].origin[2];
        if (*heliOrigin >= meshMins[0]
            && meshMaxs[0] >= *heliOrigin
            && heliOrigin[1] >= meshMins[1]
            && meshMaxs[1] >= heliOrigin[1])
        {
            break;
        }
    }
    if ((float)((float)(meshMaxs[0] - *heliOrigin) - (float)(*heliOrigin - meshMins[0])) < 0.0f)
        clearanceX = meshMaxs[0] - *heliOrigin;
    else
        clearanceX = *heliOrigin - meshMins[0];
    if ((float)((float)(heliOrigin[1] - meshMins[1]) - clearanceX) < 0.0f)
        clearanceAfterYMin = heliOrigin[1] - meshMins[1];
    else
        clearanceAfterYMin = clearanceX;
    if ((float)((float)(meshMaxs[1] - heliOrigin[1]) - clearanceAfterYMin) < 0.0f)
        minPlanarMargin = meshMaxs[1] - heliOrigin[1];
    else
        minPlanarMargin = clearanceAfterYMin;
    Scr_AddInt((int)minPlanarMargin, SCRIPTINSTANCE_SERVER);
}

void (__cdecl *__cdecl Helicopter_GetMethod(const char **pName))(scr_entref_t)
{
    unsigned int methodIndex;
    for ( methodIndex = 0; methodIndex < ARRAY_COUNT(s_methods); ++methodIndex )
    {
        if ( !strcmp(*pName, s_methods[methodIndex].actionString) )
        {
            *pName = s_methods[methodIndex].actionString;
            return s_methods[methodIndex].actionFunc;
        }
    }
    return 0;
}

void __cdecl G_SpawnHelicopter(gentity_s *ent, gentity_s *owner, char *vehicleInfoName, char *modelName)
{
    scr_vehicle_s *veh;
    team_t team;
    unsigned int ownerIndex;

    G_SetModel(ent, modelName);
    G_SpawnVehicle(ent, vehicleInfoName, 1);
    ent->clipmask = 8389632;
    ent->s.eType = ET_HELICOPTER;
    veh = ent->scr_vehicle;
    bg_vehicleInfos[veh->infoIdx].type = 6;
    veh->targetEnt = 1023;
    if ( veh->lookAtEnt.isDefined()
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_helicopter.cpp",
                    238,
                    0,
                    "%s",
                    "!veh->lookAtEnt.isDefined()") )
    {
        __debugbreak();
    }
    veh->phys.mins[0] = -50.0f;
    veh->phys.mins[1] = -50.0f;
    veh->phys.mins[2] = -50.0f;
    veh->phys.maxs[0] = 50.0f;
    veh->phys.maxs[1] = 50.0f;
    veh->phys.maxs[2] = 50.0f;
    if ( !owner->client
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_helicopter.cpp",
                    245,
                    0,
                    "%s",
                    "owner->client") )
    {
        __debugbreak();
    }
    team = owner->client->sess.cs.team;
    if ( (unsigned int)team >= 4
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_helicopter.cpp",
                    248,
                    0,
                    "team doesn't index (1 << 2)\n\t%i not in [0, %i)",
                    team,
                    4) )
    {
        __debugbreak();
    }
    ownerIndex = owner->client - level.clients;
    if ( ownerIndex >= level.maxclients
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_helicopter.cpp",
                    250,
                    0,
                    "ownerIndex doesn't index level.maxclients\n\t%i not in [0, %i)",
                    ownerIndex,
                    level.maxclients) )
    {
        __debugbreak();
    }
    AssignToSmallerType<unsigned char>(&ent->s.faction.iHeadIconTeam, team | (4 * ownerIndex));
    ent->handler = 25;
    ent->nextthink = level.time + 50;
    ent->r.svFlags |= 0x10u;
    Heli_InitFirstThink(ent);
}

void __cdecl Heli_InitFirstThink(gentity_s *heliEnt)
{
    float wheelWorldPos[3];
    vehicle_physic_t *phys;
    scr_vehicle_s *veh;
    int wheelIndex;

    veh = heliEnt->scr_vehicle;
    phys = &veh->phys;
    for ( wheelIndex = 0; wheelIndex < 6; ++wheelIndex )
    {
        if ( veh->boneIndex.wheel[wheelIndex] >= 0 )
        {
            G_DObjGetWorldBoneIndexPos(heliEnt, veh->boneIndex.wheel[wheelIndex], wheelWorldPos);
            phys->wheelZPos[wheelIndex] = wheelWorldPos[2];
        }
    }
    VEH_SetPosition(heliEnt, phys->origin, phys->vel, phys->angles);
    phys->prevOrigin[0] = phys->origin[0];
    phys->prevOrigin[1] = phys->origin[1];
    phys->prevOrigin[2] = phys->origin[2];
    phys->prevAngles[0] = phys->angles[0];
    phys->prevAngles[1] = phys->angles[1];
    phys->prevAngles[2] = phys->angles[2];
    heliEnt->health = 99999;
    heliEnt->handler = 25;
    heliEnt->nextthink = level.time + 50;
}

void __cdecl Helicopter_Pain(
    gentity_s *heliEnt,
    gentity_s *pAttacker,
    int damage,
    const float *point,
    const int mod,
    const float *dir,
    const hitLocation_t __formal,
    const int __formal2)
{
    const WeaponDef *weapDef;
    if ( pAttacker )
    {
        if ( pAttacker->s.weapon )
        {
            weapDef = BG_GetWeaponDef(pAttacker->s.weapon);
            if ( weapDef->weapType == WEAPTYPE_PROJECTILE || weapDef->weapType == WEAPTYPE_GRENADE )
                VEH_JoltBody(heliEnt, dir, 1.0f, 0.0f, 0.0f);
        }
    }
}

void __cdecl Helicopter_Die(
    gentity_s *heliEnt,
    gentity_s *pInflictor,
    gentity_s *pAttacker,
    int damage,
    int mod,
    const int weapon,
    const float *dir,
    const hitLocation_t __formal,
    int __formal2)
{
    const WeaponDef *weapDef;
    if ( pAttacker )
    {
        if ( pAttacker->s.weapon )
        {
            weapDef = BG_GetWeaponDef(pAttacker->s.weapon);
            if ( weapDef->weapType == WEAPTYPE_PROJECTILE || weapDef->weapType == WEAPTYPE_GRENADE )
                VEH_JoltBody(heliEnt, dir, 1.0f, 0.0f, 0.0f);
        }
    }
}

void __cdecl Helicopter_Controller(const gentity_s *heliEnt, int *partBits)
{
    float mainTurretYawRad;
    float gunnerTurretYawRad;
    float barrelAngles[3];
    DObj *obj;
    scr_vehicle_s *veh;
    float bodyAngles[3];
    float turretAngles[3];
    int gunnerIdx;

    if ( !heliEnt
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_helicopter.cpp", 311, 0, "%s", "pSelf") )
    {
        __debugbreak();
    }
    if ( !heliEnt->scr_vehicle
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_helicopter.cpp",
                    312,
                    0,
                    "%s",
                    "pSelf->scr_vehicle") )
    {
        __debugbreak();
    }
    veh = heliEnt->scr_vehicle;
    obj = Com_GetServerDObj(heliEnt->s.number);
    if ( !obj
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_helicopter.cpp", 316, 0, "%s", "obj") )
    {
        __debugbreak();
    }
    if ( veh->nitrousVehicle )
    {
        memset(bodyAngles, 0, sizeof(bodyAngles));
    }
    else
    {
        bodyAngles[0] = (float)heliEnt->s.lerp.u.vehicle.throttle * kAngleShortToDegrees;
        bodyAngles[1] = 0.0f;
        bodyAngles[2] = heliEnt->s.lerp.u.turret.gunAngles[1];
    }
    if ( veh->boneIndex.body >= 0 )
        DObjSetLocalBoneIndex(obj, partBits, veh->boneIndex.body, vec3_origin, bodyAngles);
    mainTurretYawRad = (float)heliEnt->s.lerp.u.vehicle.gunYaw * kAngleShortToDegrees;
    turretAngles[0] = 0.0f;
    turretAngles[1] = mainTurretYawRad;
    turretAngles[2] = 0.0f;
    barrelAngles[0] = (float)heliEnt->s.lerp.u.vehicle.gunPitch * kAngleShortToDegrees;
    barrelAngles[1] = 0.0f;
    barrelAngles[2] = 0.0f;
    if ( veh->boneIndex.turret >= 0 )
        DObjSetLocalBoneIndex(obj, partBits, veh->boneIndex.turret, vec3_origin, turretAngles);
    if ( veh->boneIndex.barrel >= 0 )
        DObjSetLocalBoneIndex(obj, partBits, veh->boneIndex.barrel, vec3_origin, barrelAngles);
    for ( gunnerIdx = 0; gunnerIdx < 4; ++gunnerIdx )
    {
        gunnerTurretYawRad = (float)heliEnt->s.lerp.u.vehicle.gunnerAngles[gunnerIdx].yaw * kAngleShortToDegrees;
        turretAngles[0] = 0.0f;
        turretAngles[1] = gunnerTurretYawRad;
        turretAngles[2] = 0.0f;
        barrelAngles[0] = (float)heliEnt->s.lerp.u.vehicle.gunnerAngles[gunnerIdx].pitch * kAngleShortToDegrees;
        barrelAngles[1] = 0.0f;
        barrelAngles[2] = 0.0f;
        if ( veh->boneIndex.gunnerTags[gunnerIdx].turret >= 0 )
            DObjSetLocalBoneIndex(obj, partBits, veh->boneIndex.gunnerTags[gunnerIdx].turret, vec3_origin, turretAngles);
        if ( veh->boneIndex.gunnerTags[gunnerIdx].barrel >= 0 )
            DObjSetLocalBoneIndex(obj, partBits, veh->boneIndex.gunnerTags[gunnerIdx].barrel, vec3_origin, barrelAngles);
    }
}

void __cdecl Helicopter_Think(gentity_s *heliEnt)
{
    Scr_Vehicle_Think(heliEnt);
    heliEnt->nextthink = level.time + 50;
}
