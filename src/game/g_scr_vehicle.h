#pragma once

const vehicle_info_t *__cdecl BG_GetVehicleInfo(int index);
gentity_s *__cdecl GScr_GetVehicle(scr_entref_t entref);
gentity_s *__cdecl VEH_GetVehicle(int entNum);
int __cdecl VEH_IsSeatPresent(gentity_s *ent, int seatIdx, const vehicle_info_t *info);
int __cdecl VEH_GetSeatWeaponIndex(gentity_s *const vehicle, int seatIndex);
int __cdecl VEH_GetSeatFiringOriginAngles(gentity_s *const vehEnt, int seatIndex, float *origin, float *angles);
VehicleSeat *__cdecl VEH_GetSeat(scr_vehicle_s *veh, int seatIdx);
VehicleSeat *__cdecl VEH_GetSeat(gentity_s *vehEnt, int seatIdx);
scr_vehicle_s *__cdecl GetVehicleLogObjectError(gentity_s *ent);
int __cdecl VEH_GetSeatOccupantEntNum(scr_vehicle_s *veh, int seatIdx);
int __cdecl VEH_GetSeatOccupantEntNum(gentity_s *vehEnt, int seatIdx);
int __cdecl VEH_SetSeatOccupantEntNum(scr_vehicle_s *veh, int seatIdx, unsigned int occEntNum);
gentity_s *__cdecl VEH_GetSeatOccupantEntity(scr_vehicle_s *veh, int seatIdx);
gentity_s *__cdecl VEH_GetSeatOccupantEntity(gentity_s *vehEnt, int seatIdx);
void __cdecl VEH_SetPosition(gentity_s *ent, const float *origin, float *vel, const float *angles);
void __cdecl VEH_InitEntity(gentity_s *ent, scr_vehicle_s *veh, int infoIdx);
void __cdecl VEH_InitVehicle(gentity_s *ent, scr_vehicle_s *veh, int infoIdx);
void __cdecl VEH_InitSeat(scr_vehicle_s *veh, int seatIdx);
void __cdecl VEH_InitFromInfo(scr_vehicle_s *veh, const vehicle_info_t *info);
void __cdecl VEH_InitPhysics(gentity_s *ent, int infoIdx);
void __cdecl VEH_JoltBody(gentity_s *ent, const float *dir, float intensity, float speedFrac, float decel);
void __cdecl VEH_CalcAccel(gentity_s *ent, char *move, float *bodyAccel, float *rotAccel);
int __cdecl VEH_CorrectAllSolid(gentity_s *ent, trace_t *trace);
void __cdecl VEH_ClearGround();
bool __cdecl VEH_SlideMove(gentity_s *ent, int gravity);
void __cdecl VEH_ClipVelocity(float *in, float *normal, float *out);
void __cdecl VEH_AirMove(gentity_s *ent, int gravity);
void __cdecl VEH_StepSlideMove(gentity_s *ent, int gravity);
bool __cdecl VEH_ClampPointToBoundingBox(gentity_s *vehicle, const float *point, float *out);
void __cdecl VEH_GetBoundingBoxFromCollmap(gentity_s *vehicle, float *bbMins, float *bbMaxs);
void __cdecl VEH_DebugAim(gentity_s *ent, const float *color, int duration);
void __cdecl VEH_UpdateVelocityWithRotation(gentity_s *ent);
void __cdecl VEH_GetNewSpeedAndAccel(
        scr_vehicle_s *veh,
        float dt,
        int hovering,
        float speedScale,
        float *newSpeed,
        float *accelMax);
double __cdecl VEH_AccelerateSpeed(float speed, float tgtSpeed, float accel, float dt);
void __cdecl VEH_SetLinkAngleClamps(
        const vehicle_info_t *info,
        gentity_s *player,
        int seatIndex,
        float *playerAngles,
        float *vehicleAngles);
void __cdecl VEH_SetPlayerVehicle(gentity_s *ent, bool enable);
void __cdecl VEH_LinkPlayer(gentity_s *ent, gentity_s *player, int seatIndex, bool changingSeats);
double  Scr_Vehicle_DamageScale@<st0>(
        float a1@<ebp>,
        gentity_s *pSelf,
        gentity_s *pAttacker,
        gentity_s *pInflictor,
        const float *point,
        int mod,
        int weapon,
        int *damageFromUnderneath);
void __cdecl VEH_UnlinkPlayer(gentity_s *player, bool changingSeats, char *error_msg);
char __cdecl VEH_ExitPosOkay(gentity_s *vehEnt, gentity_s *player, int exitIndex, float *origin);
void __cdecl G_VehicleFinishedAnimating(gentity_s *player, pmoveVehAnimState_t vehAnimState);
int __cdecl VEH_GetNextSeat(const vehicle_info_t *info, int currentSeat);
int __cdecl VEH_NextAvailableSeat(gentity_s *player);
void __cdecl VEH_SwitchClientToNextSeat(gentity_s *ent, gentity_s *player);
void __cdecl VEH_SwitchClientToSeat(gentity_s *ent, gentity_s *player, int seatIndex);
void __cdecl G_ParseScrVehicleInfo();
void __cdecl VEH_RinitVehiclesUsingInfo(int infoIdx);
char __cdecl G_VehicleUpdateField(const char *vehicleInfoName, char *keyValue);
void __cdecl G_ReloadScrVehicleInfo();
void __cdecl G_InitScrVehicles();
void __cdecl G_SetupScrVehicles();
void __cdecl VEH_SetupCollmap(gentity_s *ent);
gentity_s *__cdecl VEH_GetCollMap(const char *modelname);
void __cdecl G_FreeScrVehicles();
void __cdecl G_SpawnVehicle(gentity_s *ent, char *typeName, int load);
int __cdecl VEH_GetVehicleInfoFromName(char *name);
int __cdecl G_LoadVehicle(char *name);
void __cdecl VEH_InitModelAndValidateTags(gentity_s *ent, int *infoIdx);
char __cdecl VEH_DObjHasRequiredTags(gentity_s *ent, int infoIdx);
void __cdecl G_FreeVehicle(gentity_s *ent);
void __cdecl G_HideVehicle(gentity_s *ent);
void __cdecl G_MakeVehicleUsable(gentity_s *ent);
void __cdecl G_UpdateVehicleTags(gentity_s *ent);
bool __cdecl G_IsVehicleRemoteControl(int index);
void __cdecl GScr_PrecacheVehicle();
void __cdecl GScr_GetVehicleTreadFXArray();
int __cdecl G_IsVehicleOccupied(gentity_s *ent);
team_t __cdecl G_GetVehicleOccupantsTeam(gentity_s *vehEnt);
bool __cdecl G_IsVehicleSeatOccupied(gentity_s *ent, int seatIndex);
int __cdecl G_GetVehicleSeatPlayerOccupies(gentity_s *ent, gentity_s *player);
int __cdecl G_GetVehicleSeatToEnter(gentity_s *ent, gentity_s *player, bool scriptUse, int *entryPoint);
int __cdecl G_GetEntryPointForSeat(gentity_s *ent, int seat);
int __cdecl G_GetPlayerVehicleMantlePoint(gentity_s *ent, gentity_s *player);
int __cdecl G_IsVehicleUsable(gentity_s *ent, gentity_s *player);
bool __cdecl VEH_ShouldEjectOccupants(gentity_s *ent);
bool __cdecl G_IsVehicleImmune(gentity_s *ent, int mod, char damageFlags, unsigned int weapon);
bool __cdecl G_IsPlayerDrivingVehicle(const gentity_s *player);
// local variable allocation has failed, the output may be wrong!
void  VEH_Teleport(NitrousVehicle *a1@<ebp>, gentity_s *pSelf, float *origin, float *angles, float *vel);
void __cdecl VEH_UpdateNOClip(gentity_s *pSelf);
void __cdecl VEH_UpdateDebug(gentity_s *pSelf);
void __cdecl Scr_Vehicle_Controller(const gentity_s *pSelf, int *partBits);
void __cdecl Scr_Vehicle_Init(gentity_s *pSelf);
void __cdecl VEH_GetWheelOrigin(gentity_s *ent, int idx, float *origin, float *quat);
void __cdecl VEH_GroundPlant(gentity_s *ent, vehicle_physic_t *phys, int gravity);
void __cdecl VEH_DebugLine(const float *start, const float *end, float r, float g, float b);
void __cdecl VEH_DebugBox(float *pos, float width, float r, float g, float b);
void __cdecl VEH_TouchEntities(gentity_s *ent);
void __cdecl VEH_PushEntity(gentity_s *ent, gentity_s *target);
bool __cdecl AttachedStickyMissile(gentity_s *vehicle, gentity_s *missile);
void __cdecl PushAttachedStickyMissile(gentity_s *vehicle, gentity_s *missile);
void __cdecl VEH_UpdateHealth(gentity_s *pSelf);
double __cdecl GetPieceHealthPercentage(
        Destructible *dest,
        unsigned __int16 destructibleLabelString,
        float defaultPercentage);
char __cdecl GetPieceHealth(Destructible *dest, unsigned __int16 destructibleLabelString, int *pieceHealth);
bool __cdecl PlayersSeatGunTurning(gentity_s *const pVehEnt, int seatIndex, int angle);
void __cdecl VEH_UpdateNitrousPosition(gentity_s *pSelf);
void __cdecl Scr_Vehicle_Think(gentity_s *pSelf);
void __cdecl VEH_MoveTrace(gentity_s *ent);
void __cdecl VEH_BackupPosition(gentity_s *ent);
void __cdecl VEH_UpdateAim(gentity_s *ent);
void __cdecl VEH_UpdateGunnerAimAll(gentity_s *ent);
void __cdecl VEH_UpdateGunnerAim(gentity_s *ent, int gunnerIndex);
void __cdecl VEH_UpdateGunnerWeaponsAll(gentity_s *ent, int msec);
void __cdecl VEH_UpdateGunnerWeapon(gentity_s *ent, int gunnerIndex, int msec);
gentity_s *__cdecl VEH_FireGunnerWeapon(gentity_s *ent, int gunnerIndex, gentity_s *attacker);
void __cdecl VEH_UpdateBody(gentity_s *ent);
void __cdecl VEH_UpdateSteering(gentity_s *ent);
void __cdecl VEH_UpdateDriverWeapons(gentity_s *ent);
void __cdecl VEH_UpdateDriverActions(gentity_s *ent);
void __cdecl VEH_UpdateClient(gentity_s *ent);
void __cdecl VEH_GroundTrace(gentity_s *ent);
void __cdecl VEH_GroundMove(gentity_s *ent);
void __cdecl VEH_UpdateAvoidance(gentity_s *ent);
void __userpurge NitrousVehicle::update_script_target(NitrousVehicle *this@<ecx>, int a2@<ebp>, float *goal_position);
void __cdecl GetClosestPointOnSegment(float *pt1, float *pt2, float *testPt, float *out);
// local variable allocation has failed, the output may be wrong!
void  VEH_UpdatePath(const vehicle_info_t *a1@<ebp>, gentity_s *ent);
void __cdecl VEH_UpdateAIMove(gentity_s *ent);
void __cdecl VEH_UpdateMoveToGoal(gentity_s *ent, const float *goalPos);
bool __cdecl VEH_IsHovering(scr_vehicle_s *veh);
double __cdecl VEH_UpdateMove_GetDesiredYaw(scr_vehicle_s *veh, float *desiredDir);
void __cdecl VEH_UpdateMoveOrientation(gentity_s *ent, float *desiredDir);
void __cdecl VEH_UpdateAngleAndAngularVel(
        int index,
        float desiredAngle,
        float accel,
        float decel,
        float overShoot,
        vehicle_physic_t *phys);
double __cdecl VEH_CalcAccelFraction(float accel, int infoIdx);
double __cdecl VEH_CalcAngleForAccel(float accelFraction);
double __cdecl VEH_CalcStoppingTime(float accel, float accelFraction);
void __cdecl VEH_UpdateYawAndNotify(gentity_s *ent, float desiredYaw);
double __cdecl VEH_GetAccelForAngles(scr_vehicle_s *veh);
void __cdecl VEH_AddFakeDrag(const float *velocity, float maxDragSpeed, float *accelVec);
void __cdecl VEH_CheckHorizontalVelocityToGoal(
        scr_vehicle_s *veh,
        const float *vecToGoal,
        float accelMax,
        float *accelVec);
void __cdecl VEH_CheckVerticalVelocityToGoal(scr_vehicle_s *veh, float verticalDist, float *accelVec);
int __cdecl VEH_UpdateMove_CheckGoalReached(gentity_s *ent, float distToGoal);
double __cdecl VEH_UpdateMove_CheckStop(scr_vehicle_s *veh, float distToGoal);
void __cdecl VEH_UpdateMove_CheckNearGoal(gentity_s *ent, float distToGoal);
void __cdecl VEH_UpdateYawAngularVel(gentity_s *ent, float desiredYaw);
bool __cdecl VEH_CheckIfGoalYawChanged(gentity_s *ent, float desiredYaw);
void __cdecl VEH_UpdateHover(gentity_s *ent);
void __cdecl VEH_SetHoverGoal(gentity_s *ent);
void __cdecl VEH_UpdatePlaneOnCurve(gentity_s *ent);
void __cdecl VEH_DebugPlaneOnCurve(gentity_s *ent);
void __cdecl VEH_UpdatePlaneRoll(gentity_s *ent);
void __cdecl VEH_UpdatePlaneFree(gentity_s *ent);
void __cdecl VEH_UpdateChopperPathDrive(gentity_s *ent);
void __cdecl Scr_Vehicle_Pain(
        gentity_s *pSelf,
        gentity_s *pAttacker,
        int damage,
        const float *point,
        int mod,
        const float *dir,
        hitLocation_t hitLoc,
        int weaponIdx);
void __cdecl Scr_Vehicle_Touch(gentity_s *pSelf, gentity_s *pOther);
unsigned __int16 __cdecl G_GetVehicleTypeString(int clientNum, int entityNum);
void __cdecl Scr_Vehicle_Use(gentity_s *pEnt, gentity_s *pOther);
void __cdecl Scr_Vehicle_Die(
        gentity_s *pSelf,
        gentity_s *pInflictor,
        gentity_s *pAttacker,
        int damage,
        int mod,
        int weapon,
        float *dir,
        hitLocation_t hitLoc,
        int timeOffset);
void __cdecl SP_script_vehicle(gentity_s *pSelf, SpawnVar *spawnVar);
void __cdecl SP_script_vehicle_collmap(gentity_s *pSelf);
void (__cdecl *__cdecl ScriptVehicle_GetMethod(const char **pName))(scr_entref_t);
void __cdecl CMD_VEH_AttachPath(scr_entref_t entref);
void __cdecl VEH_ResetWheels(gentity_s *ent, vehicle_physic_t *phys);
void __cdecl CMD_VEH_GetModel(scr_entref_t entref);
void __cdecl CMD_VEH_GetAttachPos(scr_entref_t entref);
void __cdecl CMD_VEH_StartPath(scr_entref_t entref);
void __cdecl CMD_VEH_DrivePath(scr_entref_t entref);
void __cdecl CMD_VEH_SetDrivePathPhysicsScale(scr_entref_t entref);
void __cdecl CMD_VEH_SetSwitchNode(scr_entref_t entref);
void __cdecl CMD_VEH_SetWaitNode(scr_entref_t entref);
void __cdecl CMD_VEH_SetWaitSpeed(scr_entref_t entref);
void __cdecl CMD_VEH_SetSpeed(scr_entref_t entref);
void __cdecl CMD_VEH_Script_SetSpeed(gentity_s *ent);
void __cdecl CMD_VEH_SetSpeedImmediate(scr_entref_t entref);
void __cdecl CMD_VEH_GetSpeed(scr_entref_t entref);
void __cdecl CMD_VEH_GetSpeedMPH(scr_entref_t entref);
void __cdecl CMD_VEH_GetGoalSpeedMPH(scr_entref_t entref);
void __cdecl CMD_VEH_SetBrake(scr_entref_t entref);
void __cdecl CMD_VEH_SetAcceleration(scr_entref_t entref);
void __cdecl CMD_VEH_SetDeceleration(scr_entref_t entref);
void __cdecl CMD_VEH_ResumeSpeed(scr_entref_t entref);
void __cdecl CMD_VEH_SetYawSpeed(scr_entref_t entref);
void __cdecl CMD_VEH_SetMaxPitchRoll(scr_entref_t entref);
void __cdecl CMD_VEH_SetAirResitance(scr_entref_t entref);
void __cdecl CMD_VEH_GetSteerFactor(scr_entref_t entref);
void __cdecl CMD_VEH_GetThrottle(scr_entref_t entref);
void __cdecl CMD_VEH_SetTurningAbility(scr_entref_t entref);
void __cdecl CMD_VEH_SetJitterParams(scr_entref_t entref);
void __cdecl CMD_VEH_SetHoverParams(scr_entref_t entref);
void __cdecl CMD_VEH_JoltBody(scr_entref_t entref);
void __cdecl CMD_VEH_FreeVehicle(scr_entref_t entref);
void __cdecl CMD_VEH_GetWheelSurface(scr_entref_t entref);
void __cdecl CMD_VEH_GetVehicleOwner(scr_entref_t entref);
void __cdecl CMD_VEH_IsVehicleUsable(scr_entref_t entref);
void __cdecl CMD_VEH_MakeVehicleUsable(scr_entref_t entref);
void __cdecl CMD_VEH_MakeVehicleUnusable(scr_entref_t entref);
void __cdecl CMD_VEH_UseVehicle(scr_entref_t entref);
void __cdecl CMD_VEH_SetViewClamp();
void __cdecl CMD_VEH_ResetViewClamp(scr_entref_t entref);
void __cdecl CMD_VEH_SetVehicleLookatText(scr_entref_t entref);
void __cdecl CMD_VEH_NearGoalNotifyDist(scr_entref_t entref);
void  CMD_VEH_SetGoalPos(float a1@<ebp>, scr_entref_t entref);
void __cdecl CMD_VEH_ClearGoalPos(scr_entref_t entref);
void __cdecl CMD_VEH_SetPlaneGoalPos(scr_entref_t entref);
void __cdecl VEH_GenerateCurveForPlane(gentity_s *ent, float (*goals)[3], int numGoals, const float *goalAngles);
void __cdecl CMD_VEH_SetPlaneBarrelRoll(scr_entref_t entref);
void __cdecl CMD_VEH_SetGoalYaw(scr_entref_t entref);
void __cdecl CMD_VEH_ClearGoalYaw(scr_entref_t entref);
void __cdecl CMD_VEH_SetTargetYaw(scr_entref_t entref);
void __cdecl CMD_VEH_ClearTargetYaw(scr_entref_t entref);
void __cdecl CMD_VEH_SetDefaultPitch(scr_entref_t entref);
void __cdecl CMD_VEH_ClearDefaultPitch(scr_entref_t entref);
void __cdecl CMD_VEH_GetAngularVelocity(scr_entref_t entref);
void __cdecl CMD_VEH_SetAngularVelocity(scr_entref_t entref);
void __cdecl CMD_VEH_SetVehVelocity(scr_entref_t entref);
void __cdecl CMD_VEH_SetTurretTargetVec(scr_entref_t entref);
void __cdecl CMD_VEH_SetTurretTargetEnt(scr_entref_t entref);
void __cdecl CMD_VEH_ClearTurretTarget(scr_entref_t entref);
void __cdecl CMD_VEH_SetGunnerTargetVec(scr_entref_t entref);
void __cdecl CMD_VEH_GetGunnerTargetVec(scr_entref_t entref);
void __cdecl CMD_VEH_SetGunnerTargetEnt(scr_entref_t entref);
void __cdecl CMD_VEH_ClearGunnerTarget(scr_entref_t entref);
void __cdecl CMD_VEH_SetGunnerTurretOnTargetRange(scr_entref_t entref);
void __cdecl CMD_VEH_GetGunnerTargetEnt(scr_entref_t entref);
void __cdecl CMD_VEH_GetGunnerAnimPitch(scr_entref_t entref);
void __cdecl CMD_VEH_GetGunnerAnimYaw(scr_entref_t entref);
void __cdecl CMD_VEH_SetLookAtEnt(scr_entref_t entref);
void __cdecl CMD_VEH_ClearLookAtEnt(scr_entref_t entref);
void __cdecl CMD_VEH_ReturnPlayerControl(scr_entref_t entref);
void __cdecl VEH_CancelAIMove(gentity_s *ent);
void __cdecl CMD_VEH_SetWeapon(scr_entref_t entref);
void __cdecl CMD_VEH_FireWeapon(scr_entref_t entref);
int __cdecl VEH_GetTagBoneIndex(gentity_s *ent, int barrel);
void __cdecl CMD_VEH_FireGunnerWeapon(scr_entref_t entref);
void __cdecl CMD_VEH_StopFireWeapon(scr_entref_t entref);
void __cdecl CMD_VEH_IsTurretReady(scr_entref_t entref);
void __cdecl CMD_VEH_ForceMaterialSpeed(scr_entref_t entref);
void __cdecl CMD_VEH_SetMaxSpeed(scr_entref_t entref);
void __cdecl CMD_VEH_GetOccupants(scr_entref_t entref);
void __cdecl CMD_VEH_GetSeatOccupant(scr_entref_t entref);
void __cdecl CMD_VEH_GetOccupantSeat(scr_entref_t entref);
void __cdecl CMD_VEH_SetSeatOccupied(scr_entref_t entref);
void __cdecl CMD_VEH_SeatGetWeapon(scr_entref_t entref);
void __cdecl CMD_VEH_GetSeatFiringOrigin(scr_entref_t entref);
void __cdecl CMD_VEH_GetSeatFiringAngles(scr_entref_t entref);
void __cdecl CMD_VEH_IsGunnerFiring(scr_entref_t entref);
void __cdecl CMD_VEH_DisableGunnerFiring(scr_entref_t entref);
void __cdecl CMD_VEH_GetTreadHealth(scr_entref_t entref);
void __cdecl CMD_VEH_finishVehicleDamage(scr_entref_t entref);
void __cdecl CMD_VEH_finishVehicleRadiusDamage(scr_entref_t entref);
void __cdecl CMD_VEH_IsVehicleImmuneToDamage(scr_entref_t entref);
void __cdecl CMD_VEH_IsBoosting(scr_entref_t entref);
void __cdecl CMD_VEH_GetBoost(scr_entref_t entref);
void __cdecl G_VehSetClientSideHealthPercentageEntity(gentity_s *const ent, float percentage);
void __cdecl VehSetClientSideHealth(gentity_s *const ent, float percentage, int mask, char shift);
void __cdecl VehSetClientSideTime2Val(gentity_s *const ent, int val, int mask, char shift);
void __cdecl G_VehSetClientSideHealthPercentageLeftTread(gentity_s *const ent, float percentage);
void __cdecl G_VehSetClientSideHealthPercentageRightTread(gentity_s *const ent, float percentage);
void __cdecl G_VehSetClientSideSeatOccupancyFlags(gentity_s *const ent, int occupancy_flags);
void __cdecl G_VehSetClientSideGunTurningYaw(gentity_s *const ent, unsigned int seatIndex, bool turning);
void __cdecl VehSetClientSideGunTurnRate(gentity_s *const ent, float percentage, int mask, char shift);
void __cdecl G_VehSetClientSideGunTurningPitch(gentity_s *const ent, unsigned int seatIndex, bool turning);
void __cdecl G_VehSetClientSideGunOverheating(gentity_s *const ent, unsigned int seatIndex, bool overheating);
