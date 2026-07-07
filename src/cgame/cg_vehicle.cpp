#include "cg_vehicle.h"
#include <client/splitscreen.h>
#include <universal/com_math_anglevectors.h>
#include <gfx_d3d/r_water_sim.h>
#include <bgame/bg_weapons.h>
#include <clientscript/cscr_memorytree.h>
#ifdef KISAK_SP
#include <cgame_sp/cg_local_sp.h>
#include <cgame_sp/cg_vehicles_sp.h>
#else
#include <cgame_mp/cg_local_mp.h>
#include <cgame_mp/cg_vehicles_mp.h>
#endif
#include <xanim/dobj_utils.h>
#include <xanim/xmodel_utils.h>
#include <clientscript/scr_const.h>
#include <universal/surfaceflags.h>
#include <sound/snd_bank.h>
#include "cg_sound.h"
#include <clientscript/cscr_vm.h>
#include "cg_scr_main.h"
#include "cg_event.h"
#include <bgame/bg_slidemove.h>
#include <clientscript/cscr_stringlist.h>
#include <gfx_d3d/r_foliage.h>
#include <sound/snd_public_async.h>
#include <sound/snd_utils.h>
#include <qcommon/dobj_management.h>
#include <physics/phys_traverse.h>

namespace
{
static constexpr float kAngleToShort = 182.04445f;
static constexpr float kShortToAngle = 0.0054931641f;

static int FloatAngleToShort(float angle)
{
    return (int)floor(angle * kAngleToShort + 0.5f);
}

static float ShortToFloatAngle(signed short angle)
{
    return (float)angle * kShortToAngle;
}

static float LerpVehicleGunPitch(const LerpEntityState *currentState, const entityState_s *nextState, float frac)
{
    const float currentAngle = ShortToFloatAngle(currentState->u.vehicle.gunPitch);
    const float nextAngle = ShortToFloatAngle(nextState->lerp.u.vehicle.gunPitch);
    return AngleNormalize180(nextAngle - currentAngle) * frac + currentAngle;
}

static float LerpVehicleThrottle(const LerpEntityState *currentState, const entityState_s *nextState, float frac)
{
    const float currentAngle = ShortToFloatAngle(currentState->u.vehicle.throttle);
    const float nextAngle = ShortToFloatAngle(nextState->lerp.u.vehicle.throttle);
    return AngleNormalize180(nextAngle - currentAngle) * frac + currentAngle;
}

static float LerpGunnerAngleShort(signed short currentAngle, signed short nextAngle, float frac)
{
    const float fromAngle = ShortToFloatAngle(currentAngle);
    const float toAngle = ShortToFloatAngle(nextAngle);
    return AngleNormalize180(toAngle - fromAngle) * frac + fromAngle;
}

static float Clamp01(float value)
{
    float result = value;
    if (value - 1.0f >= 0.0f)
        result = 1.0f;
    if (0.0f - value >= 0.0f)
        result = 0.0f;
    return result;
}

// MT_Alloc leaves proximity_data as raw storage; reset()/update() need a valid vtable.
static void CG_Vehicle_InitVehicleCache(vehicle_cache_t *cache)
{
    new (&cache->proximity_data) colgeom_visitor_inlined_t<200>();
    cache->proximity_data.reset();
    for (int wheelIndex = 0; wheelIndex < 6; ++wheelIndex)
    {
        cache->hit_indices[wheelIndex] = -1;
        cache->lastOrigin[0] = FLT_MAX;
        cache->lastOrigin[1] = FLT_MAX;
        cache->lastOrigin[2] = FLT_MAX;
        cache->lastAngles[0] = FLT_MAX;
        cache->lastAngles[1] = FLT_MAX;
        cache->lastAngles[2] = FLT_MAX;
    }
}
}

const int seat_positions[2] = { 0, 1 };
const int yawturn_masks[2] = { 1048576, 4194304 };
const int pitchturn_masks[2] = { 2097152, 8388608 };
const int overheating_shifts[2] = { 24, 25 };
const int overheating_masks[2] = { 16777216, 33554432 };
const int overheat_flags[2] = { 4, 8 };

// *WARNING* One or more selections were skipped as they could not be interpreted as c data
float foam = 1.9;
float foamLerpRate = 0.5;
float foamRadius = 30.0;
float depth = -2.0;
float lerpRate_0 = 0.1;
float radius = 30.0;
float dist1 = 200.0;
float depth2 = 2.0;
float lerpRate2 = 0.18000001;
float radius2 = 30.0;
float dist2 = -200.0;
float s_minVelocityForTireSound = 0.025;
float recoilDist = 24.0;
float recoilBackPow = 0.25;
int recoilBackTime = 250;
int recoilDuration = 1500;
float aScrollSpeed = 300.0;
int fadetime = 500;
bool playTreadClacks = true;
bool playTreadSqueaks = true;
bool playBodyDmgLoop = true;
bool playFireSounds = true;
bool playOverheatSounds = true;
bool playTurretRotationSounds = true;

const int aaWheels_0[2][3] =
{ { 0, 4, 2 }, { 1, 5, 3 } };

const int s_kaiSurfaceGroups[31] =
{
  0,
  0,
  1,
  0,
  0,
  1,
  0,
  0,
  0,
  1,
  0,
  1,
  1,
  1,
  0,
  0,
  1,
  1,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  1,
  0,
  0
};
const int aaWheels[2][2] = { { 0, 2 }, { 1, 3 } };

unsigned __int16 *g_wheelTags[6] =
{
    &scr_const.tag_wheel_front_left,
    &scr_const.tag_wheel_front_right,
    &scr_const.tag_wheel_back_left,
    &scr_const.tag_wheel_back_right,
    &scr_const.tag_wheel_middle_left,
    &scr_const.tag_wheel_middle_right
};

unsigned __int16 *g_gunnerBarrelTags[4] =
{
    &scr_const.tag_gunner_barrel1,
    &scr_const.tag_gunner_barrel2,
    &scr_const.tag_gunner_barrel3,
    &scr_const.tag_gunner_barrel4
};

unsigned __int16 *g_gunnerTurretTags[4] =
{
    &scr_const.tag_gunner_turret1,
    &scr_const.tag_gunner_turret2,
    &scr_const.tag_gunner_turret3,
    &scr_const.tag_gunner_turret4,
};

//----- (0050E410) --------------------------------------------------------
// void __usercall CG_UpdateVehicleWaterFX(char *a1@<esi>, int localClientNum, centity_s *cent);
void CG_UpdateVehicleWaterFX(int localClientNum, centity_s *cent)
{
    float axis[3][3];
    float point[3];
    if ( CL_LocalClient_IsFirstActive(localClientNum) )
    {
        AnglesToAxis(cent->pose.angles, axis);
        point[0] = (float)(dist1 * axis[0][0]) + cent->pose.origin[0];
        point[1] = (float)(dist1 * axis[0][1]) + cent->pose.origin[1];
        point[2] = (float)(dist1 * axis[0][2]) + cent->pose.origin[2];
        R_WaterSimulationSetHeight(cent->pose.origin, radius, depth, lerpRate_0);
        point[0] = (float)(dist2 * axis[0][0]) + cent->pose.origin[0];
        point[1] = (float)(dist2 * axis[0][1]) + cent->pose.origin[1];
        point[2] = (float)(dist2 * axis[0][2]) + cent->pose.origin[2];
        R_WaterSimulationSetHeight(point, radius2, depth2, lerpRate2);
    }
}

//----- (0050E550) --------------------------------------------------------
// local variable allocation has failed, the output may be wrong!
// void __usercall CG_Vehicle_PreControllers(int a1@<ebp>, int a2@<esi>, int localClientNum, DObj *obj, centity_s *cent);
void CG_Vehicle_PreControllers(int localClientNum, const DObj *obj, centity_s *cent)
{
    col_context_t context;
    float expand_vec[3];
    float trace_points[6][2][3];
    float wheelPos[3];
    const DObjAnimMat *bonePose;
    unsigned int boneIndex;
    int wheelPresenceMask;
    float boundsMax[3];
    float boundsMin[3];
    int wheelBit;
    vehicle_cache_t *vehicleCache;
    float axisAngles[3];
    float axisLocVec3[3];
    float clampedWheelHeight;
    float wheelHeightLerpFactor;
    float wheelHeightLerpUnclamped;
    const VehicleParameter *vehicleParams;
    float rearWheelCenterDist;
    float frontWheelCenterDist;
    rigid_body_constraint_wheel *pairedWheel;
    const VehicleParameter *nitrousParams;
    float wheelCenterDist;
    float wheelCenterDistOther;
    rigid_body_constraint_wheel *frontWheelPair;
    float newWheelHeight;
    const VehicleParameter *suspParams;
    float wheelRadiusDelta;
    rigid_body_constraint_wheel *nitrousWheel;
    int wheelIndex;
    float axis[4][3];
    float suspTravel;
    float weaponSpin;
    const WeaponDef *turretWeapon;
    int gunnerSeatIndex;
    const clientInfo_t *ci;
    int clientNum;
    float gunnerYawAngle;
    float gunnerPitchAngle;
    float recoilSinAngle;
    float recoilClampHigh;
    float recoilClampLow;
    float recoilProgress;
    float recoilPowBase;
    float barrelPitchAngle;
    float zeroShort;
    float steerPitchDeltaShort;
    float steerPitchDelta;
    float steerYawAngle;
    float steerYawFrom;
    float steerYawTo;
    float rollAngle;
    float rollFrom;
    float rollTo;
    float pitchAngle;
    const vehicle_info_t *vehicleInfoForPitch;
    float yawDelta;
    float yawAngle;
    float newViewAngles[3];
    float newViewAxis[3][3];
    float yawAxis[3][3];
    float yawAngles[3];
    float vehAxis[3][3];
    float vehYaw;
    float vehAngles[3];
    float gunnerPitchOffset;
    float localTurretYaw;
    float vehicleBodyYaw;
    float interpolatedGunYaw;
    float nextGunYawAngle;
    float currentGunYawAngle;
    float turretAnglesAxis[3][3];
    float turretAngles[3];
    float invMat[3][3];
    float baseAnglesAxis[3][3];
    float baseAngles[3];
    float gunnerYawPacked;
    float gunnerPitchPacked;
    float gunnerYawPackedNext;
    float gunnerPitchPackedNext;
    float tagPitch;
    float tagYaw;
    float tagAngles[3];
    float tagAxis[4][3];
    DObjAnimMat animMat;
    unsigned char boneLookupIndex;
    int relativeGunnerIndex;
    DObj *gunnerDObj;
    unsigned __int16 *gunnerViewTags[4];
    int i;
    int numWheels;
    const vehicle_info_t *info;
    float speed;
    float forward[3];
    const playerState_s *ps;
    const entityState_s *ns;
    const LerpEntityState *currentState;
    cg_s *cgameGlob;
    bool barrelPitchAlreadySet;
    int ignoreGunner;
    trace_t trace;
    static int steeringWheelTag;
    memset(&trace, 0, 16);
    if (!obj
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp",
            339,
            0,
            "%s",
            "obj"))
    {
        __debugbreak();
    }
    ignoreGunner = -1;
    barrelPitchAlreadySet = false;
    if (!cent->vehicle)
    {
        cent->vehicle = (cgVehicle_s *)MT_Alloc(84, 21, SCRIPTINSTANCE_SERVER);
        if (!cent->vehicle
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp",
                347,
                0,
                "%s",
                "cent->vehicle"))
        {
            __debugbreak();
        }
        memset((unsigned __int8 *)cent->vehicle, 0, 0x54u);
    }
    cgameGlob = CG_GetLocalClientGlobals(localClientNum);
    currentState = &cent->currentState;
    ns = &cent->nextState;
    ps = &cgameGlob->predictedPlayerState;
    AngleVectors(cent->pose.angles, forward, 0, 0);
    speed = ns->lerp.pos.trDelta[0] * forward[0]
        + ns->lerp.pos.trDelta[1] * forward[1]
        + ns->lerp.pos.trDelta[2] * forward[2];
    if (cent->nitrousVeh)
        speed = cent->nitrousVeh->m_forward_vel;
    VEH_UpdateMaterialTime(cent, cgameGlob->frametime, speed);
    info = CG_GetVehicleInfo(cent->nextState.vehicleState.vehicleInfoIndex);
    numWheels = 0;
    if (info->type == 1)
    {
        numWheels = 2;
    }
    else if (info->type)
    {
        if (info->type == 2)
            numWheels = 6;
    }
    else
    {
        numWheels = 4;
    }
    if (!cent->vehicle->vehicle_cache && numWheels > 0)
    {
        cent->vehicle->vehicle_cache = (vehicle_cache_t *)MT_Alloc(1928, 21, SCRIPTINSTANCE_SERVER);
        if (!cent->vehicle->vehicle_cache
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp",
                382,
                0,
                "%s",
                "cent->vehicle->vehicle_cache"))
        {
            __debugbreak();
        }
        CG_Vehicle_InitVehicleCache(cent->vehicle->vehicle_cache);
    }
    if ((ps->eFlags & 0x4000) != 0 && ps->viewlocked_entNum == cent->nextState.number)
    {
        // T5 retail: CoDMPServer.c:249290-249328. Helicopters (gVehicleRelativeGunnerAngles): pack
        // viewangles + barrel bind-pose into entity gunnerAngles for server sync. Retail does NOT
        // write cent->pose.vehicle.gunnerPitch/Yaw here — pose is driven by snapshot lerp elsewhere.
        if (ps->vehiclePos >= 1 && ps->vehiclePos <= 4 && gVehicleRelativeGunnerAngles)
        {
            gunnerViewTags[0] = &scr_const.tag_gunner_barrel1;
            gunnerViewTags[1] = &scr_const.tag_gunner_barrel2;
            gunnerViewTags[2] = &scr_const.tag_gunner_barrel3;
            gunnerViewTags[3] = &scr_const.tag_gunner_barrel4;
            gunnerDObj = Com_GetClientDObj(cent->nextState.number, localClientNum);
            if (gunnerDObj)
            {
                relativeGunnerIndex = ps->vehiclePos - 1;
                boneLookupIndex = 0;
                DObjGetBoneIndex(gunnerDObj, *gunnerViewTags[relativeGunnerIndex], &boneLookupIndex, -1);
                DObjGetBasePoseMatrix(gunnerDObj, boneLookupIndex, &animMat);
                QuatToAxis(animMat.quat, tagAxis);
                AxisToAngles(tagAxis, tagAngles);
                tagPitch = tagAngles[0];
                tagYaw = tagAngles[1];
                gunnerPitchPacked = ps->viewangles[0] + tagPitch;
                cent->currentState.u.vehicle.gunnerAngles[relativeGunnerIndex].pitch = FloatAngleToShort(gunnerPitchPacked);
                gunnerYawPacked = ps->viewangles[1] + tagYaw;
                cent->currentState.u.vehicle.gunnerAngles[relativeGunnerIndex].yaw = FloatAngleToShort(gunnerYawPacked);
                gunnerPitchPackedNext = ps->viewangles[0] + tagPitch;
                cent->nextState.lerp.u.vehicle.gunnerAngles[relativeGunnerIndex].pitch = FloatAngleToShort(gunnerPitchPackedNext);
                gunnerYawPackedNext = ps->viewangles[1] + tagYaw;
                cent->nextState.lerp.u.vehicle.gunnerAngles[relativeGunnerIndex].yaw = FloatAngleToShort(gunnerYawPackedNext);
            }
        }
        else if (!vehLockTurretToPlayerView->current.enabled || ps->vehiclePos)
        {
            nextGunYawAngle = ShortToFloatAngle(ns->lerp.u.vehicle.gunYaw) + ns->lerp.apos.trBase[1];
            currentGunYawAngle = ShortToFloatAngle(currentState->u.vehicle.gunYaw) + currentState->apos.trBase[1];
            interpolatedGunYaw = AngleNormalize180(nextGunYawAngle - currentGunYawAngle) * cgameGlob->frameInterpolation
                + currentGunYawAngle;
            vehicleBodyYaw = cent->pose.angles[1];
            localTurretYaw = AngleNormalize180(interpolatedGunYaw - vehicleBodyYaw);
            cent->pose.vehicle.yaw = FloatAngleToShort(localTurretYaw);
            if (ps->vehiclePos >= 1 && ps->vehiclePos <= 4)
            {
                vehAngles[0] = cent->pose.angles[0];
                vehAngles[1] = cent->pose.angles[1];
                vehAngles[2] = cent->pose.angles[2];
                vehYaw = vehAngles[1];
                vehAngles[1] = 0.0f;
                AnglesToAxis(vehAngles, vehAxis);
                yawAngles[0] = 0.0f;
                yawAngles[1] = AngleNormalize180(ps->viewangles[1] - vehYaw);
                yawAngles[2] = 0.0f;
                AnglesToAxis(yawAngles, yawAxis);
                MatrixMultiply(yawAxis, vehAxis, newViewAxis);
                AxisToAngles(newViewAxis, newViewAngles);
                gunnerPitchOffset = AngleNormalize180(newViewAngles[0]);
                cgameGlob->gunnerPitchOffset = gunnerPitchOffset;
                gunnerSeatIndex = ps->vehiclePos - 1;
                cent->pose.vehicle.gunnerPitch[gunnerSeatIndex] = FloatAngleToShort(
                    AngleNormalize180(ps->viewangles[0] - gunnerPitchOffset));
                cent->pose.vehicle.gunnerYaw[gunnerSeatIndex] = FloatAngleToShort(
                    AngleNormalize180(ps->viewangles[1] - interpolatedGunYaw));
                ignoreGunner = gunnerSeatIndex;
            }
            DObjLock(obj);
            DObjSkelClear(obj);
            DObjUnlock(obj);
        }
        else
        {
            baseAngles[0] = cent->pose.angles[0];
            baseAngles[1] = cent->pose.angles[1];
            baseAngles[2] = cent->pose.angles[2];
            AnglesToAxis(baseAngles, baseAnglesAxis);
            MatrixTranspose(baseAnglesAxis, invMat);
            turretAngles[0] = ps->viewangles[0];
            turretAngles[1] = ps->viewangles[1];
            turretAngles[2] = cent->pose.angles[2];
            AnglesToAxis(turretAngles, turretAnglesAxis);
            MatrixMultiply(turretAnglesAxis, invMat, baseAnglesAxis);
            AxisToAngles(baseAnglesAxis, turretAngles);
            cent->pose.vehicle.yaw = FloatAngleToShort(turretAngles[1]);
            cent->pose.vehicle.barrelPitch = FloatAngleToShort(turretAngles[0]);
            DObjLock(obj);
            DObjSkelClear(obj);
            DObjUnlock(obj);
            barrelPitchAlreadySet = true;
        }
    }
    else
    {
        nextGunYawAngle = ShortToFloatAngle(ns->lerp.u.vehicle.gunYaw) + ns->lerp.apos.trBase[1];
        currentGunYawAngle = ShortToFloatAngle(currentState->u.vehicle.gunYaw) + currentState->apos.trBase[1];
        yawAngle = AngleNormalize180(nextGunYawAngle - currentGunYawAngle) * cgameGlob->frameInterpolation
            + currentGunYawAngle;
        yawDelta = yawAngle - cent->pose.angles[1];
        cent->pose.vehicle.yaw = FloatAngleToShort(yawDelta);
    }
    if (cent->nitrousVeh)
    {
        zeroShort = 0.0f;
        cent->pose.vehicle.pitch = FloatAngleToShort(zeroShort);
        cent->pose.vehicle.roll = FloatAngleToShort(zeroShort);
        cent->pose.vehicle.steerPitch = FloatAngleToShort(zeroShort);
        cent->pose.vehicle.steerYaw = FloatAngleToShort(zeroShort);
    }
    else
    {
        vehicleInfoForPitch = CG_GetVehicleInfo((unsigned __int8)cent->nextState.vehicleState.vehicleInfoIndex);
        if (vehicleInfoForPitch->isNitrous)
        {
            zeroShort = 0.0f;
            cent->pose.vehicle.pitch = FloatAngleToShort(zeroShort);
            cent->pose.vehicle.roll = FloatAngleToShort(zeroShort);
        }
        else
        {
            pitchAngle = LerpVehicleThrottle(currentState, ns, cgameGlob->frameInterpolation);
            cent->pose.vehicle.pitch = FloatAngleToShort(pitchAngle);
            rollFrom = currentState->u.turret.gunAngles[1];
            rollTo = ns->lerp.u.turret.gunAngles[1];
            rollAngle = AngleNormalize180(rollTo - rollFrom) * cgameGlob->frameInterpolation + rollFrom;
            cent->pose.vehicle.roll = FloatAngleToShort(rollAngle);
        }
        steerYawFrom = currentState->u.turret.gunAngles[0];
        steerYawTo = ns->lerp.u.turret.gunAngles[0];
        steerYawAngle = AngleNormalize180(steerYawTo - steerYawFrom) * cgameGlob->frameInterpolation + steerYawFrom;
        cent->pose.vehicle.steerYaw = FloatAngleToShort(steerYawAngle);
        if (info->wheelRotRate == 0.0f)
        {
            cent->pose.vehicle.steerPitch = 0;
        }
        else
        {
            steerPitchDelta = (float)cgameGlob->frametime * 0.001f * info->wheelRotRate * speed;
            steerPitchDeltaShort = (float)FloatAngleToShort(steerPitchDelta);
            cent->pose.vehicle.steerPitch += (int)steerPitchDeltaShort;
        }
    }
    if (!barrelPitchAlreadySet)
    {
        barrelPitchAngle = LerpVehicleGunPitch(currentState, ns, cgameGlob->frameInterpolation);
        cent->pose.vehicle.barrelPitch = FloatAngleToShort(barrelPitchAngle);
    }
    if (cent->vehicle->fireTime && cent->vehicle->fireTime > cgameGlob->time - recoilDuration)
    {
        if (cent->vehicle->fireTime <= cgameGlob->time - recoilBackTime)
        {
            recoilProgress = 1.0f - (float)(cgameGlob->time - cent->vehicle->fireTime - recoilBackTime)
                / (float)(recoilDuration - recoilBackTime);
        }
        else
        {
            recoilPowBase = (float)(cgameGlob->time - cent->vehicle->fireTime) / (float)recoilBackTime;
            recoilProgress = powf(recoilPowBase, recoilBackPow);
        }
        recoilClampHigh = Clamp01(recoilProgress);
        recoilClampLow = recoilClampHigh;
        recoilSinAngle = recoilClampLow * 3.1415927f - 1.5707964f;
        recoilProgress = (sinf(recoilSinAngle) + 1.0f) * 0.5f;
        cent->pose.vehicle.barrelRecoil = recoilProgress * recoilDist;
    }
    else
    {
        cent->pose.vehicle.barrelRecoil = 0.0f;
    }
    for (i = 0; i < 4; ++i)
    {
        if (ignoreGunner != i)
        {
            gunnerPitchAngle = LerpGunnerAngleShort(
                currentState->u.vehicle.gunnerAngles[i].pitch,
                ns->lerp.u.vehicle.gunnerAngles[i].pitch,
                cgameGlob->frameInterpolation);
            cent->pose.vehicle.gunnerPitch[i] = FloatAngleToShort(gunnerPitchAngle);
            gunnerYawAngle = LerpGunnerAngleShort(
                currentState->u.vehicle.gunnerAngles[i].yaw,
                ns->lerp.u.vehicle.gunnerAngles[i].yaw,
                cgameGlob->frameInterpolation);
            cent->pose.vehicle.gunnerYaw[i] = FloatAngleToShort(gunnerYawAngle);
        }
    }
    DObjGetBoneIndex(obj, scr_const.tag_body, &cent->pose.vehicle.tag_body, -1);
    DObjGetBoneIndex(obj, scr_const.tag_turret, &cent->pose.vehicle.tag_turret, -1);
    DObjGetBoneIndex(obj, scr_const.tag_barrel, &cent->pose.vehicle.tag_barrel, -1);
    if (info->type != 6)
        DObjGetBoneIndex(obj, scr_const.tag_barrel_recoil, &cent->pose.vehicle.tag_barrel_recoil, -1);
    cent->pose.vehicle.tag_minigun_spin = -2;
    cent->pose.vehicle.minigun_rotation = 0;
    for (i = 0; i < 4; ++i)
    {
        DObjGetBoneIndex(obj, *g_gunnerBarrelTags[i], &cent->pose.vehicle.tag_gunner_barrel[i], -1);
        DObjGetBoneIndex(obj, *g_gunnerTurretTags[i], &cent->pose.vehicle.tag_gunner_turret[i], -1);
    }
    for (clientNum = 0; clientNum < 32; ++clientNum)
    {
        ci = &cgameGlob->bgs.clientinfo[clientNum];
        if (!ci->infoValid)
            continue;
        if (ci->attachedVehEntNum != cent->nextState.number || ci->attachedVehSeat < 1 || ci->attachedVehSeat > 4)
            continue;
        gunnerSeatIndex = ci->attachedVehSeat - 1;
        turretWeapon = BG_GetWeaponDef(info->gunnerWeaponIndex[gunnerSeatIndex]);
        if (turretWeapon->fireType != WEAPON_FIRETYPE_MINIGUN)
            continue;
        DObjGetBoneIndex(obj, scr_const.tag_minigun_right_barrel, &cent->pose.vehicle.tag_minigun_spin, -1);
        weaponSpin = ci->weaponSpin;
        cent->pose.vehicle.minigun_rotation = FloatAngleToShort(weaponSpin);
        break;
    }
    cent->pose.vehicle.extra_wheel_rot_scale = info->extraWheelRotScale;
    for (i = 0; i < 4; ++i)
    {
        if (info->extraWheelTags[i])
            DObjGetBoneIndex(obj, info->extraWheelTags[i], &cent->pose.vehicle.tag_extra_tank_wheels[i], -1);
        else
            cent->pose.vehicle.tag_extra_tank_wheels[i] = -2;
    }
    if (cent->pose.cullIn == 2)
    {
        suspTravel = cent->pose.vehicle.time;
        AnglesToAxis(cent->pose.angles, axis);
        axis[3][0] = cent->pose.origin[0];
        axis[3][1] = cent->pose.origin[1];
        axis[3][2] = cent->pose.origin[2];
        bonePose = XModelGetBasePose(DObjGetModel(obj, 0));
        if (*g_wheelTags[0] != scr_const.tag_wheel_front_left
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp",
                667,
                0,
                "%s",
                "*g_wheelTags[TAG_WHEEL_FRONT_LEFT] == scr_const.tag_wheel_front_left"))
        {
            __debugbreak();
        }
        if (*g_wheelTags[1] != scr_const.tag_wheel_front_right
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp",
                668,
                0,
                "%s",
                "*g_wheelTags[TAG_WHEEL_FRONT_RIGHT] == scr_const.tag_wheel_front_right"))
        {
            __debugbreak();
        }
        if (cent->nitrousVeh)
        {
            for (wheelIndex = 0; wheelIndex < 6; ++wheelIndex)
            {
                cent->vehicle->wheelSurfType[wheelIndex] = cent->nitrousVeh->m_wheel_surf_types[wheelIndex];
                nitrousWheel = cent->nitrousVeh->m_wheels[wheelIndex];
                if ((float)(uintptr_t)nitrousWheel == 0.0f)
                    continue;
                vehicleParams = cent->nitrousVeh->m_parameter;
                wheelRadiusDelta = vehicleParams->m_wheel_radius - nitrousWheel->m_wheel_radius;
                newWheelHeight = nitrousWheel->m_wheel_displaced_center_dist - vehicleParams->m_susp_adj + wheelRadiusDelta;
                if (info->type == 1)
                {
                    if (wheelIndex == 0)
                    {
                        frontWheelPair = cent->nitrousVeh->m_wheels[1];
                        wheelCenterDist = nitrousWheel->m_wheel_displaced_center_dist;
                        if (frontWheelPair)
                            wheelCenterDistOther = frontWheelPair->m_wheel_displaced_center_dist;
                        else
                            wheelCenterDistOther = wheelCenterDist;
                        nitrousParams = cent->nitrousVeh->m_parameter;
                        newWheelHeight = (wheelCenterDist + wheelCenterDistOther) * 0.5f - nitrousParams->m_susp_adj + wheelRadiusDelta;
                        DObjGetBoneIndex(obj, scr_const.tag_wheel_front, &cent->pose.vehicle.wheelBoneIndex[0], -1);
                    }
                    else if (wheelIndex == 2)
                    {
                        pairedWheel = cent->nitrousVeh->m_wheels[3];
                        frontWheelCenterDist = nitrousWheel->m_wheel_displaced_center_dist;
                        if (pairedWheel)
                            rearWheelCenterDist = pairedWheel->m_wheel_displaced_center_dist;
                        else
                            rearWheelCenterDist = frontWheelCenterDist;
                        suspParams = cent->nitrousVeh->m_parameter;
                        newWheelHeight = (frontWheelCenterDist + rearWheelCenterDist) * 0.5f - suspParams->m_susp_adj + wheelRadiusDelta;
                        DObjGetBoneIndex(obj, scr_const.tag_wheel_back, &cent->pose.vehicle.wheelBoneIndex[2], -1);
                    }
                }
                else
                {
                    DObjGetBoneIndex(obj, *g_wheelTags[wheelIndex], &cent->pose.vehicle.wheelBoneIndex[wheelIndex], -1);
                }
                if (cent->nitrousVeh->m_owner)
                {
                    cent->pose.vehicle.wheelHeight[wheelIndex] = newWheelHeight;
                }
                else
                {
                    wheelHeightLerpUnclamped = (float)cgameGlob->frametime * 0.001f * 30.0f;
                    wheelHeightLerpFactor = Clamp01(wheelHeightLerpUnclamped);
                    cent->pose.vehicle.wheelHeight[wheelIndex] = (newWheelHeight - cent->pose.vehicle.wheelHeight[wheelIndex])
                        * wheelHeightLerpFactor
                        + cent->pose.vehicle.wheelHeight[wheelIndex];
                }
                clampedWheelHeight = cent->pose.vehicle.wheelHeight[wheelIndex];
                if (info->nitrousVehParams.m_susp_min_height > clampedWheelHeight)
                    clampedWheelHeight = info->nitrousVehParams.m_susp_min_height;
                cent->pose.vehicle.wheelHeight[wheelIndex] = clampedWheelHeight;
                cent->pose.vehicle.nitrousWheelRotation[wheelIndex] = nitrousWheel->m_wheel_pos * 57.295776f;
                if (cent->nitrousVeh->m_vehicle_info->type == 2 || (wheelIndex != 0 && wheelIndex != 1))
                {
                    cent->pose.vehicle.nitrousWheelYaw[wheelIndex] = 0.0f;
                }
                else
                {
                    Phys_NitrousVecToVec3(&nitrousWheel->m_b1_wheel_axis_loc, axisLocVec3);
                    vectoangles(axisLocVec3, axisAngles);
                    cent->pose.vehicle.nitrousWheelYaw[wheelIndex] = axisAngles[1] - 90.0f;
                }
            }
            if (info->type == 1)
            {
                if (!steeringWheelTag)
                    steeringWheelTag = SL_GetLowercaseString("tag_steering_wheel", 0, SCRIPTINSTANCE_SERVER);
                if (!DObjGetBoneIndex(obj, steeringWheelTag, &cent->pose.vehicle.wheelBoneIndex[1], -1)
                    || cgameGlob->predictedPlayerState.viewlocked_entNum == cent->nextState.number)
                {
                    cent->pose.vehicle.wheelBoneIndex[1] = -2;
                }
                else
                {
                    cent->pose.vehicle.nitrousWheelYaw[1] = cent->pose.vehicle.nitrousWheelYaw[0];
                    cent->pose.vehicle.nitrousWheelRotation[1] = 0.0f;
                    cent->pose.vehicle.wheelHeight[1] = 0.0f;
                }
            }
            if (cent->nitrousVeh->m_vehicle_info->type == 2)
                R_FoliageNotifyVehiclePosition(localClientNum, cent->pose.origin);
        }
        else
        {
            vehicleCache = cent->vehicle->vehicle_cache;
            if (vehicleCache)
            {
                if (VecNCompareCustomEpsilon(vehicleCache->lastOrigin, cent->pose.origin, 0.001f, 3)
                    && VecNCompareCustomEpsilon(vehicleCache->lastAngles, cent->pose.angles, 0.001f, 3))
                {
                    wheelBit = 1;
                    for (i = 0; i < numWheels; ++i)
                    {
                        if (wheelBit & vehicleCache->wheel_mask)
                        {
                            cent->pose.vehicle.wheelHeight[i] = vehicleCache->hit_fractions[i];
                            cent->vehicle->wheelSurfType[i] = (unsigned __int8)((vehicleCache->hit_sflags[i] & 0x3F00000) >> 20);
                        }
                        wheelBit *= 2;
                    }
                }
                else
                {
                    boundsMin[0] = FLT_MAX;
                    boundsMin[1] = FLT_MAX;
                    boundsMin[2] = FLT_MAX;
                    boundsMax[0] = -FLT_MAX;
                    boundsMax[1] = -FLT_MAX;
                    boundsMax[2] = -FLT_MAX;
                    wheelPresenceMask = 0;
                    wheelBit = 1;
                    for (i = 0; i < numWheels; ++i)
                    {
                        if (DObjGetBoneIndex(obj, *g_wheelTags[i], &cent->pose.vehicle.wheelBoneIndex[i], -1))
                        {
                            wheelPresenceMask |= wheelBit;
                            boneIndex = cent->pose.vehicle.wheelBoneIndex[i];
                            MatrixTransformVector43(bonePose[boneIndex].trans, axis, wheelPos);
                            trace_points[i][0][0] = 40.0f * axis[2][0] + wheelPos[0];
                            trace_points[i][0][1] = 40.0f * axis[2][1] + wheelPos[1];
                            trace_points[i][0][2] = 40.0f * axis[2][2] + wheelPos[2];
                            trace_points[i][1][0] = -suspTravel * axis[2][0] + wheelPos[0];
                            trace_points[i][1][1] = -suspTravel * axis[2][1] + wheelPos[1];
                            trace_points[i][1][2] = -suspTravel * axis[2][2] + wheelPos[2];
                            Vec3Min(trace_points[i][0], boundsMin, boundsMin);
                            Vec3Min(trace_points[i][1], boundsMin, boundsMin);
                            Vec3Max(trace_points[i][0], boundsMax, boundsMax);
                            Vec3Max(trace_points[i][1], boundsMax, boundsMax);
                        }
                        wheelBit *= 2;
                    }
                    expand_vec[0] = 30.0f;
                    expand_vec[1] = 30.0f;
                    expand_vec[2] = 30.0f;
                    vehicleCache->proximity_data.update(boundsMin, boundsMax, 529, expand_vec);
                    context = col_context_t();
                    context.prims = vehicleCache->proximity_data.prims;
                    context.nprims = vehicleCache->proximity_data.nprims;
                    wheelBit = 1;
                    for (i = 0; i < numWheels; ++i)
                    {
                        if (wheelBit & wheelPresenceMask)
                        {
                            if (vehicleCache->hit_indices[i] > 0)
                            {
                                trace.normal.vec.v[0] = vehicleCache->hit_normals[i][0];
                                trace.normal.vec.v[1] = vehicleCache->hit_normals[i][1];
                                trace.normal.vec.v[2] = vehicleCache->hit_normals[i][2];
                                trace.sflags = vehicleCache->hit_sflags[i];
                            }
                            trace_point_vs_env(
                                &trace,
                                trace_points[i][0],
                                trace_points[i][1],
                                &context,
                                &vehicleCache->hit_indices[i]);
                            if (vehicleCache->hit_indices[i] <= 0)
                            {
                                vehicleCache->hit_fractions[i] = 1.0f;
                                vehicleCache->hit_sflags[i] = 0;
                            }
                            else
                            {
                                vehicleCache->hit_fractions[i] = trace.fraction;
                                vehicleCache->hit_normals[i][0] = trace.normal.vec.v[0];
                                vehicleCache->hit_normals[i][1] = trace.normal.vec.v[1];
                                vehicleCache->hit_normals[i][2] = trace.normal.vec.v[2];
                                vehicleCache->hit_sflags[i] = trace.sflags;
                                vehicleCache->lastOrigin[0] = cent->pose.origin[0];
                                vehicleCache->lastOrigin[1] = cent->pose.origin[1];
                                vehicleCache->lastOrigin[2] = cent->pose.origin[2];
                                vehicleCache->lastAngles[0] = cent->pose.angles[0];
                                vehicleCache->lastAngles[1] = cent->pose.angles[1];
                                vehicleCache->lastAngles[2] = cent->pose.angles[2];
                                vehicleCache->wheel_mask = wheelPresenceMask;
                            }
                            cent->pose.vehicle.wheelHeight[i] = trace.fraction;
                            cent->vehicle->wheelSurfType[i] = (unsigned __int8)((trace.sflags & 0x3F00000) >> 20);
                        }
                        wheelBit *= 2;
                    }
                }
            }
        }
    }
    else
    {
        for (i = 0; i < 6; ++i)
            cent->pose.vehicle.wheelBoneIndex[i] = -2;
    }
}

//----- (00510CF0) --------------------------------------------------------
// void __usercall VEH_UpdateMaterialTime(float a1@<ebp>, char *a2@<esi>, centity_s *cent, int dt, float speed);
void VEH_UpdateMaterialTime(centity_s *cent, int dt, float speed)
{
    phys_vec3 nitrousVelocity;
    float deltaTime;
    const vehicle_info_t *info;
    info = CG_GetVehicleInfo(cent->nextState.vehicleState.vehicleInfoIndex);
    if (info->texScroll)
    {
        deltaTime = (float)((float)dt * (float)((float)(speed * 1.0) / 176.0)) * info->texScrollScale;
        cent->vehicle->materialTime = (float)(deltaTime * 0.001) + cent->vehicle->materialTime;
        cent->vehicle->materialTime2 = (float)(deltaTime * 0.001) + cent->vehicle->materialTime2;
        if (cent->destructible)
        {
            if (cent->nitrousVeh)
            {
                //deltaTime = (float)dt * (float)(NitrousVehicle::get_a_velocity(cent->nitrousVeh, &nitrousVelocity)->z / 176.0);
                deltaTime = (float)dt * (float)(cent->nitrousVeh->get_a_velocity(&nitrousVelocity)->z / 176.0);
                cent->vehicle->materialTime = (float)((float)(deltaTime * 0.001) * aScrollSpeed) + cent->vehicle->materialTime;
                cent->vehicle->materialTime2 = cent->vehicle->materialTime2 - (float)((float)(deltaTime * 0.001) * aScrollSpeed);
            }
        }
    }
    else
    {
        cent->vehicle->materialTime = 0.0f;
        cent->vehicle->materialTime2 = 0.0f;
    }
}

//----- (00510EC0) --------------------------------------------------------
// int __usercall GetSoundInfoForVehicle@<eax>(char *a1@<esi>, int localClientNum, centity_s *cent, float *slideRate);
int GetSoundInfoForVehicle(int localClientNum, const centity_s *cent, float *slideRate)
{
    int hardnessPick;
    int surface_group_index;
    int wheelIndex;
    unsigned int surface_type_index;
    rigid_body_constraint_wheel *wheel;
    int iWheel;
    int iSide;
    int hardness;
    int surfType;
    *slideRate = 0.0f;
    surfType = -1;
    if ( cent->nitrousVeh )
    {
        hardness = -1;
        for ( iSide = 0; iSide < 2; ++iSide )
        {
            for ( iWheel = 0; iWheel < 2; ++iWheel )
            {
                wheelIndex = aaWheels[iSide][iWheel];
                surface_type_index = cent->nitrousVeh->m_wheel_surf_types[wheelIndex];
                if ( cent->nitrousVeh->m_wheel_state[wheelIndex].m_state == WHEEL_STATE_SKIDDING
                    && iWheel == 1
                    && cent->nitrousVeh->m_wheel_state[wheelIndex].m_rate > *slideRate )
                {
                    *slideRate = cent->nitrousVeh->m_wheel_state[wheelIndex].m_rate;
                }
                wheel = cent->nitrousVeh->m_wheels[wheelIndex];
                if ( wheel && (wheel->m_wheel_flags & 1) != 0 )
                {
                    if ( surface_type_index > 0x1E
                        && !Assert_MyHandler(
                                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp",
                                    923,
                                    0,
                                    "%s",
                                    "surface_type_index >= 0 && surface_type_index < ( sizeof( s_kaiSurfaceGroups ) / sizeof( s_kaiSurfaceGroups[ 0 ] ) )") )
                    {
                        __debugbreak();
                    }
                    surface_group_index = s_kaiSurfaceGroups[surface_type_index];
                    if ( (unsigned int)surface_group_index >= 2
                        && !Assert_MyHandler(
                                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp",
                                    926,
                                    0,
                                    "%s",
                                    "surface_group_index >= 0 && surface_group_index < NUM_SURF_GROUPS") )
                    {
                        __debugbreak();
                    }
                    if ( hardness < surface_group_index )
                        hardnessPick = surface_group_index;
                    else
                        hardnessPick = hardness;
                    hardness = hardnessPick;
                    if ( hardnessPick == surface_group_index )
                        surfType = surface_type_index;
                }
            }
        }
    }
    return surfType;
}

//----- (00511090) --------------------------------------------------------
// void __cdecl GetSoundOriginForVehicle(centity_s *cent, float *origin);
void GetSoundOriginForVehicle(const centity_s *cent, float *origin)
{
    origin[0] = cent->pose.origin[0];
    origin[1] = cent->pose.origin[1];
    origin[2] = cent->pose.origin[2] + 5.0f;
}

//----- (005110F0) --------------------------------------------------------
// void __usercall CG_CrossFadeVehicleMaterialEffects(int a1@<esi>, int localClientNum, centity_s *cent, vehicle_info_t *info, int groundEffectIndex, int surfType, float ratio, int fadeTime);
void CG_CrossFadeVehicleMaterialEffects(
                int localClientNum,
                const centity_s *cent,
                const vehicle_info_t *info,
                unsigned int groundEffectIndex,
                int surfType,
                float ratio,
                int fadeTime)
{
    const char *lastSurfaceName;
    char *fadeOutAlias;
    unsigned int fadeOutAliasId;
    const char *surfaceName;
    char *fadeInAlias;
    const char *prevSurfaceName;
    char *crossFadeOutAlias;
    bool transition;
    float soundOrigin[3];
    bool still_fading_out;
    const char *soundAlias;
    cgVehicleWheelEffect *wheelEffect;
    if ( !cent->vehicle
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp", 954, 0, "%s", "cent->vehicle") )
    {
        __debugbreak();
    }
    if ( groundEffectIndex >= 3
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp",
                    955,
                    0,
                    "groundEffectIndex doesn't index NUM_VEHICLE_MATERIAL_SNDS\n\t%i not in [0, %i)",
                    groundEffectIndex,
                    3) )
    {
        __debugbreak();
    }
    wheelEffect = &cent->vehicle->wheelEffects[groundEffectIndex];
    if ( ratio < 0.0000152879 )
        ratio = 0.0f;
    soundAlias = info->sndMaterialNames[groundEffectIndex];
    if ( *soundAlias )
    {
        still_fading_out = 1;
        GetSoundOriginForVehicle(cent, soundOrigin);
        if ( wheelEffect->lastSurfaceType == -1 || wheelEffect->surfaceType == wheelEffect->lastSurfaceType )
        {
            still_fading_out = 0;
        }
        else
        {
            lastSurfaceName = Com_SurfaceTypeToName(wheelEffect->lastSurfaceType);
            fadeOutAlias = va("%s_%s", soundAlias, lastSurfaceName);
            fadeOutAliasId = SND_FindAliasId(fadeOutAlias);
            CG_PlaySoundWithHandle(localClientNum, cent->nextState.number, soundOrigin, -fadeTime, 0, 1.0, fadeOutAliasId);
            still_fading_out = 0;
        }
        transition = 0;
        if (wheelEffect->surfaceType != surfType && !still_fading_out)
        {
            transition = 1;
            wheelEffect->lastSurfaceType = wheelEffect->surfaceType;
            wheelEffect->surfaceType = surfType;
        }
        if ( wheelEffect->surfaceType != -1 )
        {
            surfaceName = Com_SurfaceTypeToName(wheelEffect->surfaceType);
            fadeInAlias = va("%s_%s", soundAlias, surfaceName);
            CG_PlaySound(localClientNum, cent->nextState.number, soundOrigin, transition ? fadeTime : 0, 0, ratio, fadeInAlias);
        }
        if ( transition && wheelEffect->lastSurfaceType != -1 )
        {
            prevSurfaceName = Com_SurfaceTypeToName(wheelEffect->lastSurfaceType);
            crossFadeOutAlias = va("%s_%s", soundAlias, prevSurfaceName);
            CG_PlaySound(localClientNum, cent->nextState.number, soundOrigin, -fadeTime, 0, 1.0, crossFadeOutAlias);
        }
    }
}

//----- (00511310) --------------------------------------------------------
// void __usercall CG_UpdateVehicleNotifies(int a1@<esi>, int localClientNum, centity_s *cent);
void CG_UpdateVehicleNotifies(int localClientNum, centity_s *cent)
{
    char *value;
    int notifyFlags;
    if ( cent->nitrousVeh )
    {
        notifyFlags = cent->nitrousVeh->m_notify_flags;
        if ( notifyFlags )
        {
            if ( (notifyFlags & 1) != 0 )
            {
                if ( cent->nitrousVeh->m_collision_entnum == 1022 )
                    Scr_AddUndefined(SCRIPTINSTANCE_CLIENT);
                else
                    Scr_AddEntityNum(cent->nitrousVeh->m_collision_entnum, 0, SCRIPTINSTANCE_CLIENT, localClientNum);
                value = (char *)Com_SurfaceTypeToName(cent->nitrousVeh->m_collision_stype);
                Scr_AddString(value, SCRIPTINSTANCE_CLIENT);
                Scr_AddFloat(cent->nitrousVeh->m_collision_intensity, SCRIPTINSTANCE_CLIENT);
                Scr_AddVector(cent->nitrousVeh->m_collision_hitn, SCRIPTINSTANCE_CLIENT);
                Scr_AddVector(cent->nitrousVeh->m_collision_hitp, SCRIPTINSTANCE_CLIENT);
                CScr_Notify(localClientNum, cent, cscr_const.veh_collision, 5u);
            }
            if ( (notifyFlags & 2) != 0 )
                CScr_Notify(localClientNum, cent, cscr_const.veh_inair, 0);
            if ( (notifyFlags & 4) != 0 )
                CScr_Notify(localClientNum, cent, cscr_const.veh_landed, 0);
            if ( (notifyFlags & 8) != 0 )
                CScr_Notify(localClientNum, cent, cscr_const.veh_suspension_limit_activated, 0);
            if ( (notifyFlags & 0x10) != 0 )
                CScr_Notify(localClientNum, cent, cscr_const.veh_engine_stutter, 0);
        }
    }
}

//----- (005114B0) --------------------------------------------------------
// void __usercall CG_UpdateVehicleSounds(int a1@<esi>, int localClientNum, centity_s *cent);
void CG_UpdateVehicleSounds(int localClientNum, centity_s *cent)
{
    unsigned __int16 clientIndex;
    float speedIPS;
    const vehicle_info_t *info;
    const cg_s *cgameGlob;
    bool isLocalPlayersVehicle;
    bool vehicleOccupied;
    info = CG_GetVehicleInfo(cent->nextState.vehicleState.vehicleInfoIndex);
    cgameGlob = CG_GetLocalClientGlobals(localClientNum);
    speedIPS = Abs(cent->currentState.pos.trDelta);
    if ( cent->nitrousVeh )
        speedIPS = fabs(cent->nitrousVeh->m_forward_vel);
    isLocalPlayersVehicle = cgameGlob->bgs.clientinfo[cgameGlob->clientNum].attachedVehEntNum == cent->nextState.number;
    vehicleOccupied = 0;
    for ( clientIndex = 0; !vehicleOccupied && clientIndex < com_maxclients->current.integer; ++clientIndex )
        vehicleOccupied = cgameGlob->bgs.clientinfo[clientIndex].attachedVehEntNum == cent->nextState.number;
    if ( cent->nitrousVeh && cent->vehicle && (speedIPS != 0.0 || vehicleOccupied) )
        CG_UpdateSurfaceSounds(localClientNum, cent, info);
    if ( vehicleOccupied )
        CG_UpdateVehicleEngineSounds(localClientNum, cent, info, isLocalPlayersVehicle);
    CG_UpdateBoostSounds(localClientNum, cent, info, isLocalPlayersVehicle);
    CG_UpdateTurretAndOverheatSounds(localClientNum, cent, info, isLocalPlayersVehicle);
    CG_UpdateFireSounds(localClientNum, cent, info, isLocalPlayersVehicle);
}

//----- (00511670) --------------------------------------------------------
// void __usercall CG_UpdateSurfaceSounds(int a1@<esi>, int localClientNum, centity_s *cent, vehicle_info_t *info);
void CG_UpdateSurfaceSounds(int localClientNum, centity_s *cent, const vehicle_info_t *info)
{
    bool isWheeledVehicle;
    bool isTankVehicle;
    isTankVehicle = info && info->type == 2;
    isWheeledVehicle = info && (!info->type || info->type == 1);
    if ( !cent->nitrousVeh
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp", 1210, 0, "%s", "cent->nitrousVeh") )
    {
        __debugbreak();
    }
    if ( isTankVehicle )
    {
        CG_UpdateTankSurfaceSounds(localClientNum, cent, info);
    }
    else if ( isWheeledVehicle )
    {
        CG_UpdateWheeledSurfaceSounds(localClientNum, cent, info);
    }
}

//----- (00511740) --------------------------------------------------------
// void __usercall CG_UpdateWheeledSurfaceSounds(int a1@<ebp>, int a2@<esi>, int localClientNum, centity_s *cent, vehicle_info_t *info);
void CG_UpdateWheeledSurfaceSounds(
    int localClientNum,
    centity_s *cent,
    const vehicle_info_t *info)
{
    int velocityForward;
    float throttle;
    float speed;
    float maxSpeed;
    float speedRatio;
    float surfaceRatio;
    float accelRatio;
    float peelOut;
    // Query sound info (fills throttle)
    throttle = 0.0f;
    velocityForward = GetSoundInfoForVehicle(localClientNum, cent, &throttle);
    // ---- surface rolling ratio (speed based) ----
    speed = fabsf(cent->nitrousVeh->m_forward_vel);
    maxSpeed = cent->nitrousVeh->get_max_speed(0);
    speedRatio = (speed / maxSpeed) * (float)M_PI_2;
    speedRatio = sinf(speedRatio);
    if (speedRatio < 0.0f)
        surfaceRatio = 0.0f;
    else if (speedRatio > 1.0f)
        surfaceRatio = 1.0f;
    else
        surfaceRatio = speedRatio;
    CG_CrossFadeVehicleMaterialEffects(
        localClientNum,
        cent,
        info,
        0,
        velocityForward,
        surfaceRatio,
        fadetime
    );
    // ---- acceleration / throttle ratio ----
    accelRatio = sinf((float)M_PI_2 * throttle);
    if (accelRatio < 0.0f)
        accelRatio = 0.0f;
    else if (accelRatio > 1.0f)
        accelRatio = 1.0f;
    CG_CrossFadeVehicleMaterialEffects(
        localClientNum,
        cent,
        info,
        1,
        velocityForward,
        accelRatio,
        fadetime
    );
    // ---- peel-out / burnout ----
    peelOut = cent->nitrousVeh->is_peeling_out() ? 1.0f : 0.0f;
    CG_CrossFadeVehicleMaterialEffects(
        localClientNum,
        cent,
        info,
        2,
        velocityForward,
        peelOut,
        fadetime
    );
}

//----- (00511980) --------------------------------------------------------
// local variable allocation has failed, the output may be wrong!
// void __usercall CG_UpdateTankSurfaceSounds(int a1@<ebp>, int a2@<esi>, int localClientNum, centity_s *cent, vehicle_info_t *info);
void CG_UpdateTankSurfaceSounds(
    int localClientNum,
    centity_s *cent,
    const vehicle_info_t *info)
{
    float axis[3][3];
    float wheelOrigins[8];
    float leftOrigin[3];
    float rightOrigin[3];
    float speedIPS;
    float speedRatio;
    float healthRatio;
    float damageRatio;
    float attenuation;
    float currLeftTreadHealthRatio;
    float currRightTreadHealthRatio;
    float frametimeSeconds;
    float frameDistInches;
    bool playDamagedSounds;
    bool playClacks;
    int *accum[1][2];
    int rateLengths[1];
    phys_vec3 velocity;
    float verticalVel;
    // --- orientation ---
    AnglesToAxis(cent->pose.angles, axis);
    // left/right tread origins
    leftOrigin[0] = cent->pose.origin[0] + 35.0f * axis[0][0];
    leftOrigin[1] = cent->pose.origin[1] + 35.0f * axis[0][1];
    leftOrigin[2] = cent->pose.origin[2] + 35.0f * axis[0][2];
    rightOrigin[0] = cent->pose.origin[0] - 35.0f * axis[0][0];
    rightOrigin[1] = cent->pose.origin[1] - 35.0f * axis[0][1];
    rightOrigin[2] = cent->pose.origin[2] - 35.0f * axis[0][2];
    // --- speed ---
    speedIPS = Abs(cent->currentState.pos.trDelta);
    speedRatio = speedIPS / info->engineSndSpeed;
    if (speedRatio < 0.0f) speedRatio = 0.0f;
    else if (speedRatio > 1.0f) speedRatio = 1.0f;
    attenuation = speedRatio;
    // --- health ---
    currLeftTreadHealthRatio = CG_VehGetHealthPercentageLeftTread(cent);
    currRightTreadHealthRatio = CG_VehGetHealthPercentageRightTread(cent);
    healthRatio = CG_VehGetHealthPercentageEntity(cent);
    damageRatio = 1.0f - healthRatio;
    // --- damaged body loop ---
    if (playBodyDmgLoop)
    {
        float vol = attenuation * currRightTreadHealthRatio;
        CG_PlaySound(
            localClientNum,
            cent->nextState.number,
            leftOrigin,
            0, 0,
            vol,
            info->sndNames[11]);
        CG_PlaySound(
            localClientNum,
            cent->nextState.number,
            rightOrigin,
            0, 0,
            vol,
            info->sndNames[12]);
    }
    // --- tread grind / squeak ---
    if (playTreadSqueaks)
    {
        playDamagedSounds =
            info->treadDamageLoopHealthRatio > currLeftTreadHealthRatio;
        int leftIndex = playDamagedSounds ? 15 : 13;
        int rightIndex =
            (info->treadDamageLoopHealthRatio > currRightTreadHealthRatio) ? 16 : 14;
        float vol = info->treadGrindAttenuation * attenuation;
        CG_PlaySound(
            localClientNum,
            cent->nextState.number,
            leftOrigin,
            0, 0,
            vol,
            info->sndNames[leftIndex]);
        CG_PlaySound(
            localClientNum,
            cent->nextState.number,
            rightOrigin,
            0, 0,
            vol,
            info->sndNames[rightIndex]);
    }
    // --- clacks ---
    if (info->sndMaterialNames[0][0] && cent->nitrousVeh)
    {
        const cg_s *cgameGlob = CG_GetLocalClientGlobals(localClientNum);
        frametimeSeconds = cgameGlob->frametime * 0.001f;
        frameDistInches = (float)info->trackLengthInches;
        float frameMove = frametimeSeconds * speedIPS;
        if (cent->nitrousVeh)
            verticalVel = cent->nitrousVeh->get_a_velocity(&velocity)->z;
        else
            verticalVel = cent->nextState.lerp.apos.trDelta[2];
        for (int side = 0; side < 2; ++side)
        {
            int *distAccum =
                side == 0
                ? &cent->nitrousVeh->m_trackDistAccumLeftInches
                : &cent->nitrousVeh->m_trackDistAccumRightInches;
            int oldDist = *distAccum;
            int newDist = oldDist + (int)frameMove;
            if ((newDist / (int)frameDistInches) != (oldDist / (int)frameDistInches))
            {
                *distAccum %= (int)frameDistInches;
                if (playTreadClacks)
                {
                    // surface selection logic preserved
                    // sound name composed from material + surface
                    // randomness preserved
                    if ((float)(rand() % 101) / 100.0f >= damageRatio)
                    {
                        const char *surfName =
                            Com_SurfaceTypeToName(
                                cent->nitrousVeh->m_wheel_surf_types[0]);
                        const char *alias =
                            va("%s_%s", info->sndMaterialNames[0], surfName);
                        CG_PlaySound(
                            localClientNum,
                            cent->nextState.number,
                            side == 0 ? leftOrigin : rightOrigin,
                            0, 0,
                            1.0f,
                            alias);
                    }
                }
            }
            *distAccum = newDist;
        }
    }
}

//----- (005122A0) --------------------------------------------------------
// void __usercall CG_UpdateFireSounds(int a1@<esi>, int localClientNum, centity_s *cent, vehicle_info_t *info, bool isLocalPlayersVehicle);
void CG_UpdateFireSounds(
                int localClientNum,
                centity_s *cent,
                const vehicle_info_t *info,
                bool isLocalPlayersVehicle)
{
    const char *burningLoopAlias;
    float healthRatio;
    if ( playFireSounds )
    {
        healthRatio = CG_VehGetHealthPercentageEntity(cent);
        if ( healthRatio < 0.5 )
        {
            if ( isLocalPlayersVehicle )
                burningLoopAlias = "tank_burning_1p_loop";
            else
                burningLoopAlias = "tank_burning_3p_loop";
            CG_PlaySound(localClientNum, cent->nextState.number, cent->pose.origin, 0, 0, 1.0, (char*)burningLoopAlias);
        }
    }
}

//----- (00512320) --------------------------------------------------------
// void __usercall CG_UpdateBoostSounds(int a1@<esi>, int localClientNum, centity_s *cent, vehicle_info_t *info, bool isLocalPlayersVehicle);
void CG_UpdateBoostSounds(
                int localClientNum,
                centity_s *cent,
                const vehicle_info_t *info,
                bool isLocalPlayersVehicle)
{
    float soundOrigin[3];
    int sndNameIndex;
    bool wasBoosting;
    if ( cent->nitrousVeh )
    {
        wasBoosting = (cent->nitrousVeh->m_sfx_state_flags & 2) != 0;
        //if ( NitrousVehicle::is_boosting(cent->nitrousVeh) && !wasBoosting )
        if ( cent->nitrousVeh->is_boosting() && !wasBoosting)
        {
            GetSoundOriginForVehicle(cent, soundOrigin);
            sndNameIndex = isLocalPlayersVehicle + 17;
            CG_PlaySound(localClientNum, cent->nextState.number, soundOrigin, 0, 0, 1.0, (char*)info->sndNames[sndNameIndex]);
        }
        //if ( NitrousVehicle::is_boosting(cent->nitrousVeh) )
        if ( cent->nitrousVeh->is_boosting() )
            cent->nitrousVeh->m_sfx_state_flags |= 2u;
        else
            cent->nitrousVeh->m_sfx_state_flags &= ~2u;
    }
}

// Clamp logic matches retail CG_UpdateVehicleEngineSounds idle-loop branch (CoDMPServer.c @ 00512420).
static float CG_ClampSoundAttenuation(float attenuation)
{
    if ((float)(attenuation - 1.0f) < 0.0f)
    {
        if ((float)(0.0f - attenuation) < 0.0f)
            return attenuation;
        return 0.0f;
    }
    return 1.0f;
}

//----- (00512420) --------------------------------------------------------
// void __usercall CG_UpdateVehicleEngineSounds(int a1@<ebp>, int a2@<esi>, int localClientNum, centity_s *cent, vehicle_info_t *info, bool isLocalPlayersVehicle);
void CG_UpdateVehicleEngineSounds(
    int localClientNum,
    centity_s *cent,
    const vehicle_info_t *info,
    bool isLocalPlayersVehicle)
{
    float throttle;
    float speedIPS;
    float speedMPH;
    float speedRatio;
    float healthRatio;
    float engineRPMs;
    float rpm;
    float rpmScale;
    float pitch;
    float idleAtten;
    float loopVolume;
    float loadLevel;
    float noLoadCoef, loadCoef;
    float soundOrigin[3];
    float scriptEngineVolume;
    cgVehicleWheelEffect *wheelEffects;
    int engineVolumeEncoded;
    // ---- throttle ----
    throttle = cent->nextState.lerp.u.vehicle.throttle * (1.0f / 32768.0f);
    if (cent->nitrousVeh)
        throttle = cent->nitrousVeh->m_throttle;
    // ---- speed ----
    speedIPS = Abs(cent->currentState.pos.trDelta);//fabsf(cent->currentState.pos.trDelta);
    speedRatio = speedIPS / info->engineSndSpeed;
    if (speedRatio < 0.0f) speedRatio = 0.0f;
    else if (speedRatio > 1.0f) speedRatio = 1.0f;
    speedMPH = speedIPS / 17.6f;
    // ---- health / damage ----
    healthRatio = CG_VehGetHealthPercentageEntity(cent);
    engineRPMs = 1.0f - healthRatio;
    wheelEffects = cent->vehicle->wheelEffects;
    // ---- RPM calculation ----
    if (wheelEffects->surfaceType == -1)
    {
        rpm = ((info->gear2MaxRPMs - info->idleRPMs) * throttle)
            + info->idleRPMs;
    }
    else if (cent->nitrousVeh && throttle == 0.0f)
    {
        rpm = info->idleRPMs;
    }
    else if (speedMPH >= info->gearChangeMPH)
    {
        float slope =
            (info->gear2MaxRPMs - info->gear2MinRPMs) /
            (info->maxSpeed - info->gearChangeMPH);
        rpm = ((speedMPH - info->gearChangeMPH) * slope)
            + info->gear2MinRPMs;
    }
    else
    {
        float slope =
            (info->gear1MaxRPMs - info->gear1MinRPMs) /
            info->gearChangeMPH;
        rpm = (speedMPH * slope) + info->gear1MinRPMs;
    }
    // ---- pitch scaling ----
    rpmScale = rpm / info->idleRPMs;
    pitch = rpm / info->engineModLoopNaturalRPMs;
    // ---- sound origin ----
    GetSoundOriginForVehicle(cent, soundOrigin);
    // ---- idle attenuation ----
    {
        float denom = info->idleRPMsMax - info->idleRPMs;
        if (denom <= 0.0f)
            denom = 1.0f;
        idleAtten = (rpm - info->idleRPMs) / denom;
    }
    if (idleAtten < 0.0f) idleAtten = 0.0f;
    else if (idleAtten > 1.0f) idleAtten = 1.0f;
    // Retail (CoDMPServer / CG_UpdateVehicleEngineSounds): linear (pi/2)*t clamped to [0,1].
    loopVolume = CG_ClampSoundAttenuation(1.5707964f * idleAtten);
    // ---- idle loop ----
    int handle = CG_PlaySoundWithHandle(
        localClientNum,
        cent->nextState.number,
        soundOrigin,
        0,
        0,
        loopVolume,
        info->sndNames[isLocalPlayersVehicle]);
    SND_SetPlaybackPitch(handle, rpmScale);
    // ---- load vs no-load blend ----
    loadLevel = (fabsf(throttle) - speedRatio) + engineRPMs;
    if (loadLevel < 0.0f) loadLevel = 0.0f;
    else if (loadLevel > 1.0f) loadLevel = 1.0f;
    SND_EqualPowerFadeCoefs(loadLevel, &noLoadCoef, &loadCoef);
    // ---- no-load loop ----
    handle = CG_PlaySoundWithHandle(
        localClientNum,
        cent->nextState.number,
        soundOrigin,
        0,
        0,
        CG_ClampSoundAttenuation(noLoadCoef * idleAtten) * scriptEngineVolume,
        info->sndNames[isLocalPlayersVehicle + 7]);
    SND_SetPlaybackPitch(handle, pitch);
    // ---- load loop ----
    handle = CG_PlaySoundWithHandle(
        localClientNum,
        cent->nextState.number,
        soundOrigin,
        0,
        0,
        CG_ClampSoundAttenuation(loadCoef * idleAtten) * scriptEngineVolume,
        info->sndNames[isLocalPlayersVehicle + 9]);
    SND_SetPlaybackPitch(handle, pitch);
}

//----- (005129D0) --------------------------------------------------------
// void __usercall CG_UpdateTurretAndOverheatSounds(int a1@<esi>, int localClientNum, centity_s *cent, vehicle_info_t *info, bool isLocalPlayersVehicle);
void CG_UpdateTurretAndOverheatSounds(
                int localClientNum,
                centity_s *cent,
                const vehicle_info_t *info,
                bool isLocalPlayersVehicle)
{
    const FxEffectDef *viewLastShotEjectEffect;
    char *overheatSoundPlayer;
    char *rotateLoopSoundPlayer;
    float rotateLoopVolume;
    float pitchRotateVolume;
    float yawRotateVolume;
    bool isLocalSeat;
    float pitchTurnWeight;
    float yawTurnWeight;
    unsigned __int16 flash;
    float gunPitch;
    float gunYaw;
    float yawMax;
    float pitchMax;
    float yawMin;
    float pitchLockFade;
    float pitchMin;
    float yawLockFade;
    int overheatFlag;
    int vehSeat;
    char isOverheating;
    const WeaponDef *pWeapon;
    bool gun_turning_pitch;
    bool gun_turning_yaw;
    char wasOverheating;
    unsigned int seatLoopIndex;
    const cg_s *cgameGlob;
    if ( (playTurretRotationSounds || playOverheatSounds) && cent->nitrousVeh )
    {
        cgameGlob = CG_GetLocalClientGlobals(localClientNum);
        for ( seatLoopIndex = 0; seatLoopIndex < 2; ++seatLoopIndex )
        {
            vehSeat = seat_positions[seatLoopIndex];
            if ( vehSeat
                && (vehSeat < 1 || vehSeat > 4)
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp",
                            1402,
                            0,
                            "%s",
                            "( vehSeat == VEH_POS_DRIVER ) || VEH_GUNNER_POSITION( vehSeat )") )
            {
                __debugbreak();
            }
            isLocalSeat = cgameGlob->bgs.clientinfo[cgameGlob->clientNum].attachedVehSeat == vehSeat && isLocalPlayersVehicle;
            gun_turning_yaw = CG_VehGetSeatGunTurningYaw(cent, vehSeat);
            gun_turning_pitch = CG_VehGetSeatGunTurningPitch(cent, vehSeat);
            overheatFlag = overheat_flags[seatLoopIndex];
            wasOverheating = (overheatFlag & cent->nitrousVeh->m_sfx_state_flags) != 0;
            isOverheating = 0;
            if ( isLocalSeat )
            {
                if ( vehSeat )
                {
                    if ( vehSeat >= 1 && vehSeat <= 4 )
                        isOverheating = BG_PlayerWeaponOverheating(
                                                            &cgameGlob->predictedPlayerState,
                                                            *(unsigned __int16 *)&info->gunnerWeapon[3][2 * vehSeat + 62]);
                }
                else
                {
                    isOverheating = cent->nextState.lerp.u.turret.overheating != 0;
                }
            }
            else
            {
                isOverheating = CG_VehGetSeatGunOverheating(cent, vehSeat);
            }
            if ( isOverheating != wasOverheating || gun_turning_yaw || gun_turning_pitch )
            {
                if ( vehSeat )
                {
                    pWeapon = BG_GetWeaponDef(*(unsigned __int16 *)&info->gunnerWeapon[3][2 * vehSeat + 62]);
                    if ( !pWeapon
                        && !Assert_MyHandler(
                                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp",
                                    1458,
                                    0,
                                    "%s",
                                    "pWeapon != NULL") )
                    {
                        __debugbreak();
                    }
                }
                else
                {
                    pWeapon = BG_GetWeaponDef(cent->nextState.weapon);
                    if ( !pWeapon
                        && !Assert_MyHandler(
                                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_vehicle.cpp",
                                    1452,
                                    0,
                                    "%s",
                                    "pWeapon != NULL") )
                    {
                        __debugbreak();
                    }
                }
                if ( (gun_turning_yaw || gun_turning_pitch) && playTurretRotationSounds )
                {
                    if ( vehSeat )
                    {
                        gunYaw = (float)cent->currentState.u.vehicle.gunnerAngles[vehSeat - 1].yaw * 0.0054931641;
                        yawMin = -pWeapon->rightArc;
                        yawMax = pWeapon->leftArc;
                        gunPitch = (float)*(__int16 *)&cent->currentState.u.primaryLight.colorAndExp[4 * vehSeat + 4] * 0.0054931641;
                        pitchMin = -pWeapon->topArc;
                        pitchMax = pWeapon->bottomArc;
                    }
                    else
                    {
                        gunYaw = (float)cent->currentState.u.vehicle.gunYaw * 0.0054931641;
                        yawMin = -info->turretViewLimits.horizSpanRight;
                        yawMax = info->turretViewLimits.horizSpanLeft;
                        gunPitch = (float)cent->currentState.u.vehicle.gunPitch * 0.0054931641;
                        pitchMin = -info->turretViewLimits.vertSpanUp;
                        pitchMax = info->turretViewLimits.vertSpanDown;
                    }
                    yawLockFade = 1.0f;
                    pitchLockFade = 1.0f;
                    if ( (float)(yawMin + 3.0) <= gunYaw )
                    {
                        if ( gunYaw > (float)(yawMax - 3.0) )
                            yawLockFade = (float)(yawMax - gunYaw) / 3.0;
                    }
                    else
                    {
                        yawLockFade = (float)(gunYaw - yawMin) / 3.0;
                    }
                    if ( (float)(pitchMin + 3.0) <= gunPitch )
                    {
                        if ( gunPitch > (float)(pitchMax - 3.0) )
                            pitchLockFade = (float)(pitchMax - gunPitch) / 3.0;
                    }
                    else
                    {
                        pitchLockFade = (float)(gunPitch - pitchMin) / 3.0;
                    }
                    if ( gun_turning_yaw )
                        yawTurnWeight = 1.0f;
                    else
                        yawTurnWeight = 0.0f;
                    if ( (float)(yawLockFade - yawTurnWeight) < 0.0 )
                        yawRotateVolume = yawLockFade;
                    else
                        yawRotateVolume = yawTurnWeight;
                    if ( gun_turning_pitch )
                        pitchTurnWeight = 1.0f;
                    else
                        pitchTurnWeight = 0.0f;
                    if ( (float)(pitchLockFade - pitchTurnWeight) < 0.0 )
                        pitchRotateVolume = pitchLockFade;
                    else
                        pitchRotateVolume = pitchTurnWeight;
                    if ( (float)(yawRotateVolume - pitchRotateVolume) < 0.0 )
                        rotateLoopVolume = pitchRotateVolume;
                    else
                        rotateLoopVolume = yawRotateVolume;
                    if ( isLocalSeat )
                        rotateLoopSoundPlayer = (char *)pWeapon->rotateLoopSoundPlayer;
                    else
                        rotateLoopSoundPlayer = (char *)pWeapon->rotateLoopSound;
                    if ( rotateLoopSoundPlayer )
                        CG_PlaySound(localClientNum, cent->nextState.number, cent->pose.origin, 0, 0, rotateLoopVolume, rotateLoopSoundPlayer);
                }
                if ( playOverheatSounds )
                {
                    if ( isOverheating && !wasOverheating )
                    {
                        if ( isLocalSeat )
                            overheatSoundPlayer = (char *)pWeapon->overheatSoundPlayer;
                        else
                            overheatSoundPlayer = (char *)pWeapon->overheatSound;
                        CG_PlaySound(localClientNum, cent->nextState.number, cent->pose.origin, 0, 0, 1.0, overheatSoundPlayer);
                        flash = scr_const.tag_flash;
                        if ( vehSeat >= 1 && vehSeat <= 4 )
                            flash = *(&scr_const.tag_gunner_turret4 + vehSeat);
                        if ( isLocalSeat )
                            viewLastShotEjectEffect = pWeapon->viewLastShotEjectEffect;
                        else
                            viewLastShotEjectEffect = pWeapon->worldLastShotEjectEffect;
                        if ( viewLastShotEjectEffect )
                            CG_PlayBoltedEffect(localClientNum, viewLastShotEjectEffect, cent->nextState.number, flash);
                    }
                    if ( isOverheating )
                        cent->nitrousVeh->m_sfx_state_flags |= overheatFlag;
                    else
                        cent->nitrousVeh->m_sfx_state_flags &= ~overheatFlag;
                }
            }
        }
    }
}
