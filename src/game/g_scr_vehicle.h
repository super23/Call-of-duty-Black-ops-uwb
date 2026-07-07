#pragma once
#include <vehicle/nitrous_vehicle.h>
#include <clientscript/cscr_variable.h>
#include <bgame/bg_public.h>
#include "g_vehicle_path.h"
#include <physics/destructible.h>
#include <physics/phys_colgeom.h>

// CoDSP_rdBlackOps.map.c TRACK_g_scr_vehicle: 234496 bytes / 3664 stride = 64 slots (MP uses 16).
#define MAX_VEHICLES 64

enum VehicleMoveState : __int32
{                                       // XREF: scr_vehicle_s/r
    VEH_MOVESTATE_STOP          = 0x0,
    VEH_MOVESTATE_MOVE          = 0x1,
    VEH_MOVESTATE_HOVER         = 0x2,
    VEH_MOVESTATE_PLANE_ONCURVE = 0x3,
    VEH_MOVESTATE_PLANE_FREE    = 0x4,
};

enum VehicleTurretState : __int32
{                                       // XREF: VehicleTurret/r
    VEH_TURRET_STOPPED  = 0x0,
    VEH_TURRET_STOPPING = 0x1,
    VEH_TURRET_MOVING   = 0x2,
};

struct VehicleSeat // sizeof=0x4
{                                       // XREF: scr_vehicle_s/r
    int _occupantEntNum;
};

struct vehicle_physic_t // sizeof=0x128
{                                       // XREF: scr_vehicle_s/r
                                        // VehiclePhysicsBackup/r ...
    float origin[3];                    // XREF: CMD_VEH_GetAttachPos(scr_entref_t)+A1/w
                                        // CMD_VEH_GetAttachPos(scr_entref_t)+B1/w ...
    float prevOrigin[3];
    float angles[3];                    // XREF: CMD_VEH_GetAttachPos(scr_entref_t)+D1/w
                                        // CMD_VEH_GetAttachPos(scr_entref_t)+E1/w ...
    float prevAngles[3];
    float maxAngleVel[3];
    float yawAccel;
    float yawDecel;
    char inputAccelerationOLD;
    char inputTurning;
    // padding byte
    // padding byte
    float driverPedal;
    float driverSteer;
    int onGround;
    float colVelDelta[3];
    float mins[3];
    float maxs[3];
    float vel[3];
    float bodyVel[3];
    float rotVel[3];
    float accel[3];
    float maxPitchAngle;
    float maxRollAngle;
    float wheelZVel[6];
    float wheelZPos[6];
    int wheelSurfType[6];
    float worldTilt[3];
    float worldTiltVel[3];
    float heliLockHeight;
    float curveLength;
    int curveID;
    float curveStep;
    float curveTime;
    float timeStep;
};

struct VehicleHover // sizeof=0x1C
{                                       // XREF: scr_vehicle_s/r
    float hoverRadius;
    float hoverSpeed;
    float hoverAccel;
    float hoverGoalPos[3];
    int useHoverAccelForAngles;
};

struct VehicleTurret // sizeof=0x20
{                                       // XREF: scr_vehicle_s/r
    int fireTime;
    int fireBarrel;
    float barrelOffset;
    int flags;
    float heatVal;
    float turretOnTargetRange;
    int overheating;
    VehicleTurretState turretState;
};

struct VehicleJitter // sizeof=0x3C
{                                       // XREF: scr_vehicle_s/r
    int jitterPeriodMin;
    int jitterPeriodMax;
    int jitterEndTime;
    float jitterOffsetRange[3];
    float jitterDeltaAccel[3];
    float jitterAccel[3];
    float jitterPos[3];
};

struct VehicleTarget // sizeof=0x2C
{                                       // XREF: scr_vehicle_s/r
    int valid;
    int hasTargetYaw;
    int targetEnt;
    int lookAtEnt;
    float targetOrigin[3];
    float targetOffset[3];
    float targetYaw;
};

struct VehicleGunnerTags // sizeof=0x10
{                                       // XREF: VehicleTags/r
    int turret;
    int barrel;
    int flash;
    int flash2;
};

struct VehicleTags // sizeof=0xC4
{                                       // XREF: scr_vehicle_s/r
    int player;
    int popout;
    int body;
    int turret;
    int turret_base;
    int barrel;
    int flash[5];
    VehicleGunnerTags gunnerTags[4];
    int wheel[6];
    int seats[11];
    int entryPoints[5];
};

struct vehicle_cache_t // sizeof=0x788
{                                       // XREF: scr_vehicle_s/r
    float lastOrigin[3];
    float lastAngles[3];
    int hit_indices[6];
    int hit_sflags[6];
    float hit_normals[6][3];
    float hit_fractions[6];
    TraceHitType hit_type[6];
    unsigned __int16 hit_id[6];
    colgeom_visitor_inlined_t<200> proximity_data;
    int wheel_mask;
};

struct cgVehicleWheelEffect // sizeof=0x8
{                                       // XREF: cgVehicle_s/r
    int surfaceType;
    int lastSurfaceType;
};

struct cgVehicle_s // sizeof=0x54
{
    int lastGunnerFire[4];
    int wheelSurfType[6];
    int fireTime;
    int flags;
    float materialTime;
    float materialTime2;
    vehicle_cache_t *vehicle_cache;
    cgVehicleWheelEffect wheelEffects[3];
};

struct scr_vehicle_s // sizeof=0xE2C
{                                       // XREF: .data:s_vehicles/r
                                        // scr_vehicle_t/r
    vehicle_pathpos_t pathPos;
    vehicle_physic_t phys;
    int entNum;                         // XREF: G_InitScrVehicles(void)+43/w
    __int16 infoIdx;
    // padding byte
    // padding byte
    int flags;
    int team;
    VehicleMoveState moveState;
    __int16 waitNode;
    // padding byte
    // padding byte
    float waitSpeed;
    VehicleTurret turret;
    float turretRotScale;
    VehicleJitter jitter;
    VehicleHover hover;
    VehicleTurret gunnerTurrets[4];
    VehicleJitter gunnerJitter[4];
    unsigned __int16 lookAtText0;
    unsigned __int16 lookAtText1;
    int manualMode;
    float manualSpeed;
    float manualAccel;
    float manualDecel;
    float manualTime;
    float speed;
    float maxSpeedOverride;
    float maxDragSpeed;
    float turningAbility;
    int hasTarget;
    int hasTargetYaw;
    int hasGoalYaw;
    int stopAtGoal;
    int stopping;
    int targetEnt;
    EntHandle lookAtEnt;
    float targetOrigin[3];
    float targetOffset[3];
    float targetYaw;
    float goalPosition[3];
    float goalYaw;
    float prevGoalYaw;
    float yawOverShoot;
    int yawSlowDown;
    float pathTransitionTime;
    int hasDefaultPitch;
    float defaultPitch;
    float hasGoalRoll;
    float goalRoll;
    float goalRollTime;
    float currentRollTime;
    int numRolls;
    VehicleTarget gunnerTargets[4];
    float nearGoalNotifyDist;
    float joltDir[2];
    float joltTime;
    float joltWave;
    float joltSpeed;
    float joltDecel;
    VehicleTags boneIndex;
    int turretHitNum;
    VehicleSeat seats[11];
    float forcedMaterialSpeed;
    float modelSwapDelay;
    int oneExhaust;
    float deathQuakeScale;
    float deathQuakeDuration;
    float deathQuakeRadius;
    int secTurretAiControlled;
    float frontArmorRegen;
    int addToCompass;
    int m_bSpecialAbilityEventDown;
    int m_bFirePickupEventDown;
    int m_bSwapPickupEventDown;
    int m_bDropDeployableEventDown;
    NitrousVehicle *nitrousVehicle;
    vehicle_cache_t vehicle_cache;
    float heliHeightLockOffset;
    float scriptEngineVolume;
};

const vehicle_info_t *BG_GetVehicleInfo(int index);
gentity_s *GScr_GetVehicle(scr_entref_t entref);
gentity_s *VEH_GetVehicle(int entNum);
int VEH_IsSeatPresent(gentity_s *ent, int seatIdx, const vehicle_info_t *info);
int VEH_GetSeatWeaponIndex(gentity_s *const vehicle, int seatIndex);
int VEH_GetSeatFiringOriginAngles(gentity_s *const vehEnt, int seatIndex, float *origin, float *angles);
VehicleSeat *VEH_GetSeat(scr_vehicle_s *veh, int seatIdx);
VehicleSeat *VEH_GetSeat(gentity_s *vehEnt, int seatIdx);
scr_vehicle_s *GetVehicleLogObjectError(gentity_s *ent);
int VEH_GetSeatOccupantEntNum(scr_vehicle_s *veh, int seatIdx);
int VEH_GetSeatOccupantEntNum(gentity_s *vehEnt, int seatIdx);
int VEH_SetSeatOccupantEntNum(scr_vehicle_s *veh, int seatIdx, unsigned int occEntNum);
gentity_s *VEH_GetSeatOccupantEntity(scr_vehicle_s *veh, int seatIdx);
gentity_s *VEH_GetSeatOccupantEntity(gentity_s *vehEnt, int seatIdx);
void VEH_SetPosition(gentity_s *ent, const float *origin, float *vel, const float *angles);
void VEH_InitEntity(gentity_s *ent, scr_vehicle_s *veh, int infoIdx);
void VEH_InitVehicle(gentity_s *ent, scr_vehicle_s *veh, int infoIdx);
void VEH_InitSeat(scr_vehicle_s *veh, int seatIdx);
void VEH_InitFromInfo(scr_vehicle_s *veh, const vehicle_info_t *info);
void VEH_InitPhysics(gentity_s *ent, int infoIdx);
void VEH_JoltBody(gentity_s *ent, const float *dir, float intensity, float speedFrac, float decel);
void VEH_CalcAccel(gentity_s *ent, char *move, float *bodyAccel, float *rotAccel);
int VEH_CorrectAllSolid(gentity_s *ent, trace_t *trace);
void VEH_ClearGround();
bool VEH_SlideMove(gentity_s *ent, int gravity);
void VEH_ClipVelocity(float *in, float *normal, float *out);
void VEH_AirMove(gentity_s *ent, int gravity);
void VEH_StepSlideMove(gentity_s *ent, int gravity);
bool VEH_ClampPointToBoundingBox(gentity_s *vehicle, const float *point, float *out);
void VEH_GetBoundingBoxFromCollmap(gentity_s *vehicle, float *bbMins, float *bbMaxs);
void VEH_DebugAim(gentity_s *ent, const float *color, int duration);
void VEH_UpdateVelocityWithRotation(gentity_s *ent);
void VEH_GetNewSpeedAndAccel(
                scr_vehicle_s *veh,
                float dt,
                int hovering,
                float speedScale,
                float *newSpeed,
                float *accelMax);
double VEH_AccelerateSpeed(float speed, float tgtSpeed, float accel, float dt);
void VEH_SetLinkAngleClamps(
                const vehicle_info_t *info,
                gentity_s *player,
                int seatIndex,
                float *playerAngles,
                float *vehicleAngles);
void VEH_SetPlayerVehicle(gentity_s *ent, bool enable);
void VEH_LinkPlayer(gentity_s *ent, gentity_s *player, int seatIndex, bool changingSeats);
double    Scr_Vehicle_DamageScale(
                gentity_s *pSelf,
                gentity_s *pAttacker,
                gentity_s *pInflictor,
                const float *point,
                int mod,
                int weapon,
                int *damageFromUnderneath);
void VEH_UnlinkPlayer(gentity_s *player, bool changingSeats, char *error_msg);
char VEH_ExitPosOkay(gentity_s *vehEnt, gentity_s *player, int exitIndex, float *origin);
void G_VehicleFinishedAnimating(gentity_s *player, pmoveVehAnimState_t vehAnimState);
int VEH_GetNextSeat(const vehicle_info_t *info, int currentSeat);
int VEH_NextAvailableSeat(gentity_s *player);
void VEH_SwitchClientToNextSeat(gentity_s *ent, gentity_s *player);
void VEH_SwitchClientToSeat(gentity_s *ent, gentity_s *player, int seatIndex);
void G_ParseScrVehicleInfo();
void VEH_RinitVehiclesUsingInfo(int infoIdx);
char G_VehicleUpdateField(const char *vehicleInfoName, char *keyValue);
void G_ReloadScrVehicleInfo();
void G_InitScrVehicles();
void G_SetupScrVehicles();
void VEH_SetupCollmap(gentity_s *ent);
gentity_s *VEH_GetCollMap(const char *modelname);
void G_FreeScrVehicles();
void G_SpawnVehicle(gentity_s *ent, char *typeName, int load);
int VEH_GetVehicleInfoFromName(char *name);
int G_LoadVehicle(char *name);
void VEH_InitModelAndValidateTags(gentity_s *ent, int *infoIdx);
char VEH_DObjHasRequiredTags(gentity_s *ent, int infoIdx);
void G_FreeVehicle(gentity_s *ent);
void G_HideVehicle(gentity_s *ent);
void G_MakeVehicleUsable(gentity_s *ent);
void G_UpdateVehicleTags(gentity_s *ent);
bool G_IsVehicleRemoteControl(int index);
void GScr_PrecacheVehicle();
void GScr_GetNumVehicles();
void GScr_SetHeliHeightPatchEnabled();
void GScr_GetVehicleTreadFXArray();
void Scr_VehicleRestorePaths(gentity_s *pSelf);
int G_IsVehicleOccupied(gentity_s *ent);
team_t G_GetVehicleOccupantsTeam(gentity_s *vehEnt);
bool G_IsVehicleSeatOccupied(gentity_s *ent, int seatIndex);
int G_GetVehicleSeatPlayerOccupies(gentity_s *ent, gentity_s *player);
int G_GetVehicleSeatToEnter(gentity_s *ent, gentity_s *player, bool scriptUse, int *entryPoint);
int G_GetEntryPointForSeat(gentity_s *ent, int seat);
int G_GetPlayerVehicleMantlePoint(gentity_s *ent, gentity_s *player);
int G_IsVehicleUsable(gentity_s *ent, gentity_s *player);
bool VEH_ShouldEjectOccupants(gentity_s *ent);
bool G_IsVehicleImmune(gentity_s *ent, int mod, char damageFlags, unsigned int weapon);
bool G_IsPlayerDrivingVehicle(const gentity_s *player);
void VEH_Teleport(gentity_s *pSelf, float *origin, float *angles, float *vel);
void VEH_UpdateNOClip(gentity_s *pSelf);
void VEH_UpdateDebug(gentity_s *pSelf);
void Scr_Vehicle_Controller(const gentity_s *pSelf, int *partBits);
void Scr_Vehicle_Init(gentity_s *pSelf);
void VEH_GetWheelOrigin(gentity_s *ent, int idx, float *origin, float *quat);
void VEH_GroundPlant(gentity_s *ent, vehicle_physic_t *phys, int gravity);
void VEH_DebugLine(const float *start, const float *end, float r, float g, float b);
void VEH_DebugBox(float *pos, float width, float r, float g, float b);
void VEH_TouchEntities(gentity_s *ent);
void VEH_PushEntity(gentity_s *ent, gentity_s *target);
bool AttachedStickyMissile(gentity_s *vehicle, gentity_s *missile);
void PushAttachedStickyMissile(gentity_s *vehicle, gentity_s *missile);
void VEH_UpdateHealth(gentity_s *pSelf);
double GetPieceHealthPercentage(
                Destructible *dest,
                unsigned __int16 destructibleLabelString,
                float defaultPercentage);
char GetPieceHealth(Destructible *dest, unsigned __int16 destructibleLabelString, int *pieceHealth);
bool PlayersSeatGunTurning(gentity_s *const pVehEnt, int seatIndex, int angle);
void VEH_UpdateNitrousPosition(gentity_s *pSelf);
void Scr_Vehicle_Think(gentity_s *pSelf);
void VEH_MoveTrace(gentity_s *ent);
void VEH_BackupPosition(gentity_s *ent);
void VEH_UpdateAim(gentity_s *ent);
void VEH_UpdateGunnerAimAll(gentity_s *ent);
void VEH_UpdateGunnerAim(gentity_s *ent, int gunnerIndex);
void VEH_UpdateGunnerWeaponsAll(gentity_s *ent, int msec);
void VEH_UpdateGunnerWeapon(gentity_s *ent, int gunnerIndex, int msec);
gentity_s *VEH_FireGunnerWeapon(gentity_s *ent, int gunnerIndex, gentity_s *attacker);
void VEH_UpdateBody(gentity_s *ent);
void VEH_UpdateSteering(gentity_s *ent);
void VEH_UpdateDriverWeapons(gentity_s *ent);
void VEH_UpdateDriverActions(gentity_s *ent);
void VEH_UpdateClient(gentity_s *ent);
void VEH_GroundTrace(gentity_s *ent);
void VEH_GroundMove(gentity_s *ent);
void VEH_UpdateAvoidance(gentity_s *ent);
void GetClosestPointOnSegment(float *pt1, float *pt2, float *testPt, float *out);
void    VEH_UpdatePath(gentity_s *ent);
void VEH_UpdateAIMove(gentity_s *ent);
void VEH_UpdateMoveToGoal(gentity_s *ent, const float *goalPos);
bool VEH_IsHovering(scr_vehicle_s *veh);
double VEH_UpdateMove_GetDesiredYaw(scr_vehicle_s *veh, float *desiredDir);
void VEH_UpdateMoveOrientation(gentity_s *ent, float *desiredDir);
void VEH_UpdateAngleAndAngularVel(
                int index,
                float desiredAngle,
                float accel,
                float decel,
                float overShoot,
                vehicle_physic_t *phys);
double VEH_CalcAccelFraction(float accel, int infoIdx);
double VEH_CalcAngleForAccel(float accelFraction);
double VEH_CalcStoppingTime(float accel, float accelFraction);
void VEH_UpdateYawAndNotify(gentity_s *ent, float desiredYaw);
double VEH_GetAccelForAngles(scr_vehicle_s *veh);
void VEH_AddFakeDrag(const float *velocity, float maxDragSpeed, float *accelVec);
void VEH_CheckHorizontalVelocityToGoal(
                scr_vehicle_s *veh,
                const float *vecToGoal,
                float accelMax,
                float *accelVec);
void VEH_CheckVerticalVelocityToGoal(scr_vehicle_s *veh, float verticalDist, float *accelVec);
int VEH_UpdateMove_CheckGoalReached(gentity_s *ent, float distToGoal);
double VEH_UpdateMove_CheckStop(scr_vehicle_s *veh, float distToGoal);
void VEH_UpdateMove_CheckNearGoal(gentity_s *ent, float distToGoal);
void VEH_UpdateYawAngularVel(gentity_s *ent, float desiredYaw);
bool VEH_CheckIfGoalYawChanged(gentity_s *ent, float desiredYaw);
void VEH_UpdateHover(gentity_s *ent);
void VEH_SetHoverGoal(gentity_s *ent);
void VEH_UpdatePlaneOnCurve(gentity_s *ent);
void VEH_DebugPlaneOnCurve(gentity_s *ent);
void VEH_UpdatePlaneRoll(gentity_s *ent);
void VEH_UpdatePlaneFree(gentity_s *ent);
void VEH_UpdateChopperPathDrive(gentity_s *ent);
void Scr_Vehicle_Pain(
                gentity_s *pSelf,
                gentity_s *pAttacker,
                int damage,
                const float *point,
                int mod,
                const float *dir,
                hitLocation_t hitLoc,
                int weaponIdx);
void Scr_Vehicle_Touch(gentity_s *pSelf, gentity_s *pOther, int __formal);
unsigned __int16 G_GetVehicleTypeString(int clientNum, int entityNum);
void Scr_Vehicle_Use(gentity_s *pEnt, gentity_s *pOther, gentity_s *__formal);
void Scr_Vehicle_Die(
                gentity_s *pSelf,
                gentity_s *pInflictor,
                gentity_s *pAttacker,
                int damage,
                int mod,
                const int weapon,
                const float *dir,
                const hitLocation_t hitLoc,
                int timeOffset);
void SP_script_vehicle(gentity_s *pSelf, SpawnVar *spawnVar);
void SP_script_vehicle_collmap(gentity_s *pSelf, SpawnVar *v = NULL);
void (*ScriptVehicle_GetMethod(const char **pName))(scr_entref_t);
void CMD_VEH_AttachPath(scr_entref_t entref);
void VEH_ResetWheels(gentity_s *ent, vehicle_physic_t *phys);
void CMD_VEH_GetModel(scr_entref_t entref);
void CMD_VEH_GetAttachPos(scr_entref_t entref);
void CMD_VEH_StartPath(scr_entref_t entref);
void CMD_VEH_DrivePath(scr_entref_t entref);
void CMD_VEH_SetDrivePathPhysicsScale(scr_entref_t entref);
void CMD_VEH_SetSwitchNode(scr_entref_t entref);
void CMD_VEH_SetWaitNode(scr_entref_t entref);
void CMD_VEH_SetWaitSpeed(scr_entref_t entref);
void CMD_VEH_SetSpeed(scr_entref_t entref);
void CMD_VEH_Script_SetSpeed(gentity_s *ent);
void CMD_VEH_SetSpeedImmediate(scr_entref_t entref);
void CMD_VEH_GetSpeed(scr_entref_t entref);
void CMD_VEH_GetSpeedMPH(scr_entref_t entref);
void CMD_VEH_GetGoalSpeedMPH(scr_entref_t entref);
void CMD_VEH_SetBrake(scr_entref_t entref);
void CMD_VEH_SetAcceleration(scr_entref_t entref);
void CMD_VEH_SetDeceleration(scr_entref_t entref);
void CMD_VEH_ResumeSpeed(scr_entref_t entref);
void CMD_VEH_SetYawSpeed(scr_entref_t entref);
void CMD_VEH_SetMaxPitchRoll(scr_entref_t entref);
void CMD_VEH_SetAirResitance(scr_entref_t entref);
void CMD_VEH_GetSteerFactor(scr_entref_t entref);
void CMD_VEH_GetThrottle(scr_entref_t entref);
void CMD_VEH_SetTurningAbility(scr_entref_t entref);
void CMD_VEH_SetJitterParams(scr_entref_t entref);
void CMD_VEH_SetHoverParams(scr_entref_t entref);
void CMD_VEH_JoltBody(scr_entref_t entref);
void CMD_VEH_FreeVehicle(scr_entref_t entref);
void CMD_VEH_GetWheelSurface(scr_entref_t entref);
void CMD_VEH_GetVehicleOwner(scr_entref_t entref);
void CMD_VEH_IsVehicleUsable(scr_entref_t entref);
void CMD_VEH_MakeVehicleUsable(scr_entref_t entref);
void CMD_VEH_MakeVehicleUnusable(scr_entref_t entref);
void CMD_VEH_UseVehicle(scr_entref_t entref);
void CMD_VEH_SetViewClamp(scr_entref_t entref);
void CMD_VEH_ResetViewClamp(scr_entref_t entref);
void CMD_VEH_AddVehicleToCompass(scr_entref_t entref);
void CMD_VEH_RemoveVehicleFromCompass(scr_entref_t entref);
void CMD_VEH_SetHealthPercent(scr_entref_t entref);
void CMD_VEH_SetVehicleLookatText(scr_entref_t entref);
void CMD_VEH_StartEngineSound(scr_entref_t entref);
void CMD_VEH_StopEngineSound(scr_entref_t entref);
void CMD_VEH_SetEngineVolume(scr_entref_t entref);
void CMD_VEH_GetEngineVolume(scr_entref_t entref);
void CMD_VEH_NearGoalNotifyDist(scr_entref_t entref);
void    CMD_VEH_SetGoalPos(scr_entref_t entref);
void CMD_VEH_ClearGoalPos(scr_entref_t entref);
void CMD_VEH_SetPlaneGoalPos(scr_entref_t entref);
void VEH_GenerateCurveForPlane(gentity_s *ent, float (*goals)[3], int numGoals, const float *goalAngles);
void CMD_VEH_SetPlaneBarrelRoll(scr_entref_t entref);
void CMD_VEH_SetGoalYaw(scr_entref_t entref);
void CMD_VEH_ClearGoalYaw(scr_entref_t entref);
void CMD_VEH_SetTargetYaw(scr_entref_t entref);
void CMD_VEH_ClearTargetYaw(scr_entref_t entref);
void CMD_VEH_SetDefaultPitch(scr_entref_t entref);
void CMD_VEH_ClearDefaultPitch(scr_entref_t entref);
void CMD_VEH_GetAngularVelocity(scr_entref_t entref);
void CMD_VEH_SetAngularVelocity(scr_entref_t entref);
void CMD_VEH_SetVehVelocity(scr_entref_t entref);
void CMD_VEH_SetTurretTargetVec(scr_entref_t entref);
void CMD_VEH_SetTurretTargetEnt(scr_entref_t entref);
void CMD_VEH_ClearTurretTarget(scr_entref_t entref);
void CMD_VEH_SetGunnerTargetVec(scr_entref_t entref);
void CMD_VEH_GetGunnerTargetVec(scr_entref_t entref);
void CMD_VEH_SetGunnerTargetEnt(scr_entref_t entref);
void CMD_VEH_ClearGunnerTarget(scr_entref_t entref);
void CMD_VEH_SetGunnerTurretOnTargetRange(scr_entref_t entref);
void CMD_VEH_GetGunnerTargetEnt(scr_entref_t entref);
void CMD_VEH_GetGunnerAnimPitch(scr_entref_t entref);
void CMD_VEH_GetGunnerAnimYaw(scr_entref_t entref);
void CMD_VEH_SetLookAtEnt(scr_entref_t entref);
void CMD_VEH_ClearLookAtEnt(scr_entref_t entref);
void CMD_VEH_ReturnPlayerControl(scr_entref_t entref);
void VEH_CancelAIMove(gentity_s *ent);
void CMD_VEH_SetWeapon(scr_entref_t entref);
void CMD_VEH_FireWeapon(scr_entref_t entref);
int VEH_GetTagBoneIndex(gentity_s *ent, int barrel);
void CMD_VEH_FireGunnerWeapon(scr_entref_t entref);
void CMD_VEH_StopFireWeapon(scr_entref_t entref);
void CMD_VEH_IsTurretReady(scr_entref_t entref);
void CMD_VEH_ForceMaterialSpeed(scr_entref_t entref);
void CMD_VEH_SetMaxSpeed(scr_entref_t entref);
void CMD_VEH_GetOccupants(scr_entref_t entref);
void CMD_VEH_GetSeatOccupant(scr_entref_t entref);
void CMD_VEH_GetOccupantSeat(scr_entref_t entref);
void CMD_VEH_SetSeatOccupied(scr_entref_t entref);
void CMD_VEH_SeatGetWeapon(scr_entref_t entref);
void CMD_VEH_GetSeatFiringOrigin(scr_entref_t entref);
void CMD_VEH_GetSeatFiringAngles(scr_entref_t entref);
void CMD_VEH_IsGunnerFiring(scr_entref_t entref);
void CMD_VEH_DisableGunnerFiring(scr_entref_t entref);
void CMD_VEH_GetTreadHealth(scr_entref_t entref);
void CMD_VEH_finishVehicleDamage(scr_entref_t entref);
void CMD_VEH_finishVehicleRadiusDamage(scr_entref_t entref);
void CMD_VEH_IsVehicleImmuneToDamage(scr_entref_t entref);
void CMD_VEH_IsBoosting(scr_entref_t entref);
void CMD_VEH_GetBoost(scr_entref_t entref);
void G_VehSetClientSideEngineVolume(gentity_s *const ent, float volume);
void G_VehSetClientSideHealthPercentageEntity(gentity_s *const ent, float percentage);
void VehSetClientSideHealth(gentity_s *const ent, float percentage, int mask, char shift);
void VehSetClientSideTime2Val(gentity_s *const ent, int val, int mask, char shift);
void G_VehSetClientSideHealthPercentageLeftTread(gentity_s *const ent, float percentage);
void G_VehSetClientSideHealthPercentageRightTread(gentity_s *const ent, float percentage);
void G_VehSetClientSideSeatOccupancyFlags(gentity_s *const ent, int occupancy_flags);
void G_VehSetClientSideGunTurningYaw(gentity_s *const ent, unsigned int seatIndex, bool turning);
void VehSetClientSideGunTurnRate(gentity_s *const ent, float percentage, int mask, char shift);
void G_VehSetClientSideGunTurningPitch(gentity_s *const ent, unsigned int seatIndex, bool turning);
void G_VehSetClientSideGunOverheating(gentity_s *const ent, unsigned int seatIndex, bool overheating);


extern int bg_numVehicleInfos;
extern vehicle_info_t bg_vehicleInfos[32];

extern unsigned __int16 *s_wheelTags[6];
extern const unsigned __int16 *s_seatTags[11];