#pragma once

void __cdecl BG_RegisterDvars();
char *__cdecl BG_GetEntityTypeName(int eType);
const gitem_s *__cdecl BG_FindItemForWeapon(unsigned int weapon);
const gitem_s *__cdecl G_FindItem(char *pickupName);
bool __cdecl BG_PlayerTouchesItem(const playerState_s *ps, const entityState_s *item, int atTime);
bool __cdecl BG_PlayerCanPickUpWeaponType(const WeaponDef *weapDef, const playerState_s *ps);
bool __cdecl BG_CanItemBeGrabbed(const entityState_s *ent, const playerState_s *ps, int touched);
char __cdecl WeaponEntCanBeGrabbed(
        const entityState_s *weaponEntState,
        const playerState_s *ps,
        int touched,
        unsigned int weapIdx);
bool __cdecl BG_PlayerHasWeapon(const playerState_s *ps, unsigned int weaponIndex);
const PlayerHeldWeapon *__cdecl BG_GetHeldWeaponConst(const playerState_s *ps, unsigned int weaponIndex);
int __cdecl BG_GetHeldWeaponSlot(const playerState_s *ps, unsigned int weaponIndex);
char __cdecl HaveRoomForAmmo(const playerState_s *ps, unsigned int weaponIndex);
bool __cdecl BG_PlayerHasRoomForEntAllAmmoTypes(const entityState_s *ent, const playerState_s *ps);
void __cdecl BG_EvaluateTrajectory(const trajectory_t *tr, int atTime, float *result);
double __cdecl Vec3NormalizeTo(const float *v, float *out);
void __cdecl BG_EvaluateTrajectoryDelta(const trajectory_t *tr, int atTime, float *result);
bool __cdecl BG_ValidateOrigin(const float *pos, char xyBits, char zBits, const float *mapCenter);
bool __cdecl BG_ValidateOriginValue(float val, char bits, float mapCenterValue);
void __cdecl BG_AddPredictableEventToPlayerstate(unsigned int newEvent, unsigned int eventParm, playerState_s *ps);
void __cdecl BG_AddUnpredictableEventToPlayerstate(unsigned int newEvent, unsigned int eventParm, playerState_s *ps);
void __cdecl BG_AddEntityStateEvent(unsigned int newEvent, unsigned int eventParm, entityState_s *es, int eventTime);
void __cdecl BG_PlayerToEntitySetFlags(playerState_s *ps, entityState_s *s);
void __cdecl BG_PlayerToEntitySetPitchAngles(playerState_s *ps, entityState_s *s);
void __cdecl BG_PlayerStateToEntityState(playerState_s *ps, entityState_s *s, int snap, unsigned __int8 handler);
void __cdecl BG_PlayerToEntityProcessEvents(playerState_s *ps, entityState_s *s, unsigned __int8 handler);
void __cdecl BG_PlayerToEntityProcessEvents_Internal(
        entityState_s *s,
        unsigned __int8 handler,
        __int16 *eventSequence,
        __int16 *oldEventSequence,
        int *events,
        unsigned int *eventParms,
        unsigned int eventTime);
char __cdecl BG_PlayerToEntityShouldAddEvent(int event);
void __cdecl BG_PlayerToEntitySetMisc(playerState_s *ps, entityState_s *s);
void __thiscall renderOptions_s::CopyWeaponOptions(renderOptions_s *this, const renderOptions_s *o);
unsigned __int8 __cdecl BG_GetPlayerWeaponModel(const playerState_s *ps, unsigned int weaponIndex);
renderOptions_s __cdecl BG_PlayerWeaponOptions(const playerState_s *ps, unsigned int weaponIndex);
void __cdecl BG_PlayerToEntitySetTrajectory(playerState_s *ps, entityState_s *s, int snap);
// local variable allocation has failed, the output may be wrong!
char  BG_CheckProne@<al>(
        cStaticModel_s *a1@<ebp>,
        const playerState_s *ps,
        int passEntityNum,
        const float *vPos,
        float fSize,
        float fHeight,
        float fYaw,
        float *pfTorsoPitch,
        float *pfWaistPitch,
        bool isAlreadyProne,
        bool isOnGround,
        bool groundIsWalkable,
        unsigned __int8 handler,
        proneCheckType_t proneCheckType,
        float prone_feet_dist);
void __cdecl Phys_Vec3ToNitrousVec(float *inVector, phys_vec3 *outVector);
void __cdecl BG_GetPlayerViewOrigin(const playerState_s *ps, float *origin, int time);
void __cdecl BG_GetPlayerViewDirection(const playerState_s *ps, float *forward, float *right, float *up);
void __cdecl BG_LerpHudColors(const hudelem_s *elem, int time, hudelem_color_t *toColor);
int __cdecl BG_LoadShellShockDvars(const char *name);
void __cdecl BG_SetShellShockParmsFromDvars(shellshock_parms_t *parms);
int __cdecl BG_SaveShellShockDvars(const char *name);
shellshock_parms_t *__cdecl BG_GetShellshockParms(unsigned int index);
bool __cdecl BG_IsShellshockAnim(const playerState_s *ps);
void __cdecl BG_CreateXAnim(XAnim_s *anims, unsigned int animIndex, char *name);
void __cdecl BG_CheckThread();
int __cdecl BG_GetMaxSprintTime(const playerState_s *ps);
bool __cdecl BG_ClipMoveToDobj(const entityState_s *es);
int __cdecl BG_GetFriendlyFireStatus();
const char *__cdecl BG_DisplayName(const clientInfo_t *ci, int type);
void __cdecl BG_ClipCameraToHeliPatch(float *origin, float *oldpos, float *velocity, float (*breachPos)[3]);
const phys_vec3 *__cdecl Phys_Vec3ToNitrousVec(const phys_vec3 *result, float *inVector);


extern const dvar_s *bg_viewKickScale;
extern const dvar_s *bg_viewKickMax;
extern const dvar_s *bg_viewKickMin;
extern const dvar_s *bg_viewKickRandom;
extern const dvar_s *player_view_pitch_up;
extern const dvar_s *player_view_pitch_down;
extern const dvar_s *player_lean_shift;
extern const dvar_s *player_lean_shift_crouch;
extern const dvar_s *player_lean_rotate;
extern const dvar_s *player_lean_rotate_crouch;
extern const dvar_s *player_useSlopeAnims;
extern const dvar_s *bg_ladder_yawcap;
extern const dvar_s *bg_prone_yawcap;
extern const dvar_s *bg_foliagesnd_minspeed;
extern const dvar_s *bg_foliagesnd_maxspeed;
extern const dvar_s *bg_foliagesnd_slowinterval;
extern const dvar_s *bg_foliagesnd_fastinterval;
extern const dvar_s *bg_foliagesnd_resetinterval;
extern const dvar_s *bg_friendlyFire;
extern const dvar_s *bg_fallDamageMinHeight;
extern const dvar_s *bg_fallDamageMaxHeight;
extern const dvar_s *friction;
extern const dvar_s *stopspeed;
extern const dvar_s *cg_ufo_scaler;
extern const dvar_s *bg_swingSpeed;
extern const dvar_s *bg_proneSwingSpeed;
extern const dvar_s *bg_legYawTolerance;
extern const dvar_s *bg_legYawCrouchTolerance;
extern const dvar_s *bg_legYawProneTolerance;
extern const dvar_s *bg_viewBobAmplitudeBase;
extern const dvar_s *bg_viewBobAmplitudeSprinting;
extern const dvar_s *bg_viewBobAmplitudeDtp;
extern const dvar_s *bg_viewBobAmplitudeSwimming;
extern const dvar_s *bg_viewBobAmplitudeStanding;
extern const dvar_s *bg_viewBobAmplitudeStandingAds;
extern const dvar_s *bg_viewBobAmplitudeDucked;
extern const dvar_s *bg_viewBobAmplitudeDuckedAds;
extern const dvar_s *bg_viewBobAmplitudeProne;
extern const dvar_s *bg_viewBobAmplitudeRoll;
extern const dvar_s *bg_viewBobMax;
extern const dvar_s *bg_forceDurationOverride;
extern const dvar_s *bg_blendTimeOverride;
extern const dvar_s *bg_weaponBobAmplitudeBase;
extern const dvar_s *bg_weaponBobAmplitudeSprinting;
extern const dvar_s *bg_weaponBobAmplitudeDtp;
extern const dvar_s *bg_weaponBobAmplitudeSwimming;
extern const dvar_s *bg_weaponBobAmplitudeStanding;
extern const dvar_s *bg_weaponBobAmplitudeDucked;
extern const dvar_s *bg_weaponBobAmplitudeProne;
extern const dvar_s *bg_weaponBobAmplitudeRoll;
extern const dvar_s *bg_weaponBobMax;
extern const dvar_s *bg_weaponBobLag;
extern const dvar_s *bg_weaponBobFrequencySwimming;
extern const dvar_s *bg_weaponBobHeavyWeaponScalar;
extern const dvar_s *bg_forceExplosiveBullets;
extern const dvar_s *bg_aimSpreadMoveSpeedThreshold;
extern const dvar_s *bg_maxGrenadeIndicatorSpeed;
extern const dvar_s *player_breath_hold_time;
extern const dvar_s *player_breath_gasp_time;
extern const dvar_s *player_breath_fire_delay;
extern const dvar_s *player_breath_gasp_scale;
extern const dvar_s *player_breath_hold_lerp;
extern const dvar_s *player_breath_gasp_lerp;
extern const dvar_s *player_breath_snd_lerp;
extern const dvar_s *player_breath_snd_delay;
extern const dvar_s *player_scopeExitOnDamage;
extern const dvar_s *player_adsExitDelay;
extern const dvar_s *player_move_factor_on_torso;
extern const dvar_s *player_debugHealth;
extern const dvar_s *player_debugSprint;
extern const dvar_s *player_sustainAmmo;
extern const dvar_s *player_clipSizeMultiplier;
extern const dvar_s *player_lastStandBleedoutTime;
extern const dvar_s *player_lastStandBleedoutTimeNoRevive;
extern const dvar_s *revive_time_taken;
extern const dvar_s *player_lastStandHealthOverlayTime;
extern const dvar_s *player_reviveTriggerRadius;
extern const dvar_s *player_revivePlayerListCycleTime;
extern const dvar_s *bg_vsmode_hud;
extern const dvar_s *player_enableShuffleAnims;
extern const dvar_s *player_animRunThreshhold;
extern const dvar_s *player_animWalkThreshhold;
extern const dvar_s *player_runbkThreshhold;
extern const dvar_s *player_moveThreshhold;
extern const dvar_s *player_footstepsThreshhold;
extern const dvar_s *player_runThreshhold;
extern const dvar_s *player_knockbackMoveThreshhold;
extern const dvar_s *player_sprintThreshhold;
extern const dvar_s *player_waterSpeedScale;
extern const dvar_s *player_floatSpeed;
extern const dvar_s *player_disableWeaponsInWater;
extern const dvar_s *player_sliding_friction;
extern const dvar_s *player_sliding_wishspeed;
extern const dvar_s *player_sliding_velocity_cap;
extern const dvar_s *player_strafeSpeedScale;
extern const dvar_s *player_backSpeedScale;
extern const dvar_s *player_strafeAnimCosAngle;
extern const dvar_s *player_slopeAnimAngle;
extern const dvar_s *player_spectateSpeedScale;
extern const dvar_s *player_enduranceSpeedScale;
extern const dvar_s *player_sprintForwardMinimum;
extern const dvar_s *player_sprintSpeedScale;
extern const dvar_s *player_sprintUnlimited;
extern const dvar_s *player_sprintTime;
extern const dvar_s *player_sprintMinTime;
extern const dvar_s *player_sprintRechargePause;
extern const dvar_s *player_sprintStrafeSpeedScale;
extern const dvar_s *player_sprintCameraBob;
extern const dvar_s *player_turnRateScale;
extern const dvar_s *player_turnAnims;
extern const dvar_s *player_bayonetLaunchProof;
extern const dvar_s *player_bayonetLaunchDebugging;
extern const dvar_s *player_bayonetLaunchZCap;
extern const dvar_s *xanim_debug;
extern const dvar_s *animscript_debug;
extern const dvar_s *anim_debugSpeeds;
extern const dvar_s *animScript_listAnims;
extern const dvar_s *player_dmgtimer_timePerPoint;
extern const dvar_s *player_dmgtimer_maxTime;
extern const dvar_s *player_dmgtimer_minScale;
extern const dvar_s *player_dmgtimer_stumbleTime;
extern const dvar_s *player_dmgtimer_flinchTime;
extern const dvar_s *bg_shock_soundLoop;
extern const dvar_s *bg_shock_soundLoopSilent;
extern const dvar_s *bg_shock_soundEnd;
extern const dvar_s *bg_shock_soundEndAbort;
extern const dvar_s *bg_shock_screenType;
extern const dvar_s *bg_shock_screenBlurBlendTime;
extern const dvar_s *bg_shock_screenBlurBlendFadeTime;
extern const dvar_s *bg_shock_screenFlashWhiteFadeTime;
extern const dvar_s *bg_shock_screenFlashShotFadeTime;
extern const dvar_s *bg_shock_viewKickPeriod;
extern const dvar_s *bg_shock_viewKickRadius;
extern const dvar_s *bg_shock_viewKickFadeTime;
extern const dvar_s *bg_shock_sound;
extern const dvar_s *bg_shock_soundFadeInTime;
extern const dvar_s *bg_shock_soundFadeOutTime;
extern const dvar_s *bg_shock_soundLoopFadeTime;
extern const dvar_s *bg_shock_soundLoopEndDelay;
extern const dvar_s *bg_shock_soundRoomType;
extern const dvar_s *bg_shock_soundDryLevel;
extern const dvar_s *bg_shock_soundWetLevel;
extern const dvar_s *bg_shock_soundModEndDelay;
extern const dvar_s *bg_shock_soundSnapshot;
extern const dvar_s *bg_shock_lookControl;
extern const dvar_s *bg_shock_lookControl_maxpitchspeed;
extern const dvar_s *bg_shock_lookControl_maxyawspeed;
extern const dvar_s *bg_shock_lookControl_mousesensitivityscale;
extern const dvar_s *bg_shock_lookControl_fadeTime;
extern const dvar_s *bg_shock_movement;
extern const dvar_s *bg_shock_animation;
extern const dvar_s *bg_shock_visionset_name;
extern const dvar_s *bg_shock_visionset_inTime;
extern const dvar_s *bg_shock_visionset_outTime;
extern const dvar_s *player_meleeRange;
extern const dvar_s *player_meleeWidth;
extern const dvar_s *player_meleeHeight;
extern const dvar_s *player_bayonetRange;
extern const dvar_s *player_bayonetTargetDist;
extern const dvar_s *player_burstFireCooldown;
extern const dvar_s *bg_gravity;
extern const dvar_s *bg_lowGravity;
extern const dvar_s *player_swimTime;
extern const dvar_s *player_swimDamage;
extern const dvar_s *player_swimDamagerInterval;
extern const dvar_s *player_viewLockEnt;
extern const dvar_s *player_viewRateScale;
extern const dvar_s *player_topDownCamMode;
extern const dvar_s *player_topDownCamOffset;
extern const dvar_s *player_topDownCamAngles;
extern const dvar_s *player_topDownCamCenterPos;
extern const dvar_s *player_topDownCursorDist;
extern const dvar_s *player_topDownCursorPos;
extern const dvar_s *player_forceRedCrosshair;
extern const dvar_s *bullet_penetrationMinFxDist;
extern const dvar_s *cg_cinematicFullscreen;
extern const dvar_s *cg_debugMounting;
extern const dvar_s *player_AimBlend_Back_Low;
extern const dvar_s *player_AimBlend_Back_Mid;
extern const dvar_s *player_AimBlend_Back_Up;
extern const dvar_s *player_AimBlend_Pelvis;
extern const dvar_s *player_AimBlend_Neck;
extern const dvar_s *player_AimBlend_Head;
extern const dvar_s *player_AimBlend_Shoulder;
extern const dvar_s *dog_MeleeDamage;
extern const dvar_s *vehControlMode;
extern const dvar_s *vehRecenterDelay;
extern const dvar_s *vehLockTurretToPlayerView;
extern const dvar_s *vehLocationalVehicleSeatEntry;
extern const dvar_s *vehCameraTurretOffset;
extern const dvar_s *vehCameraTurretOffsetADS;
extern const dvar_s *vehicle_perk_boost_duration_seconds;
extern const dvar_s *vehicle_riding;
extern const dvar_s *vehicle_selfCollision;
extern const dvar_s *g_bDebugRenderPlayerCollision;
extern const dvar_s *bg_disableWeaponPlantingInWater;
extern const dvar_s *bg_plantInWaterDepth;
extern const dvar_s *bg_drawGrenadeInHand;
extern const dvar_s *bg_playStandToCrouchAnims;
extern const dvar_s *bg_maxWeaponAnimScale;
extern const dvar_s *bg_gunXOffset;
extern const dvar_s *bg_weaponleftbone;
extern const dvar_s *bg_weaponrightbone;
extern const dvar_s *bg_slopeFrames;
extern const dvar_s *debug_rope;
extern const dvar_s *vehicle_sounds_cutoff;
extern const dvar_s *footstep_sounds_cutoff;
extern const dvar_s *show_reticle_during_swimming;
extern const dvar_s *ragdoll_reactivation_cutoff;
extern const dvar_s *bg_teleportAlignTime;
extern const dvar_s *bg_enableIKActiveFix;
extern const dvar_s *waterbrush_entity;
extern const dvar_s *dive2swim;
extern const dvar_s *dive_recharge;
extern const dvar_s *playerPushAmount;
extern const dvar_s *bg_freeCamClipToHeliPatch;