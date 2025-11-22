#include "bg_misc.h"
#include <universal/q_shared.h>
#include <universal/dvar.h>

const dvar_s *bg_viewKickScale;
const dvar_s *bg_viewKickMax;
const dvar_s *bg_viewKickMin;
const dvar_s *bg_viewKickRandom;
const dvar_s *player_view_pitch_up;
const dvar_s *player_view_pitch_down;
const dvar_s *player_lean_shift;
const dvar_s *player_lean_shift_crouch;
const dvar_s *player_lean_rotate;
const dvar_s *player_lean_rotate_crouch;
const dvar_s *player_useSlopeAnims;
const dvar_s *bg_ladder_yawcap;
const dvar_s *bg_prone_yawcap;
const dvar_s *bg_foliagesnd_minspeed;
const dvar_s *bg_foliagesnd_maxspeed;
const dvar_s *bg_foliagesnd_slowinterval;
const dvar_s *bg_foliagesnd_fastinterval;
const dvar_s *bg_foliagesnd_resetinterval;
const dvar_s *bg_friendlyFire;
const dvar_s *bg_fallDamageMinHeight;
const dvar_s *bg_fallDamageMaxHeight;
const dvar_s *friction;
const dvar_s *stopspeed;
const dvar_s *cg_ufo_scaler;
const dvar_s *bg_swingSpeed;
const dvar_s *bg_proneSwingSpeed;
const dvar_s *bg_legYawTolerance;
const dvar_s *bg_legYawCrouchTolerance;
const dvar_s *bg_legYawProneTolerance;
const dvar_s *bg_viewBobAmplitudeBase;
const dvar_s *bg_viewBobAmplitudeSprinting;
const dvar_s *bg_viewBobAmplitudeDtp;
const dvar_s *bg_viewBobAmplitudeSwimming;
const dvar_s *bg_viewBobAmplitudeStanding;
const dvar_s *bg_viewBobAmplitudeStandingAds;
const dvar_s *bg_viewBobAmplitudeDucked;
const dvar_s *bg_viewBobAmplitudeDuckedAds;
const dvar_s *bg_viewBobAmplitudeProne;
const dvar_s *bg_viewBobAmplitudeRoll;
const dvar_s *bg_viewBobMax;
const dvar_s *bg_forceDurationOverride;
const dvar_s *bg_blendTimeOverride;
const dvar_s *bg_weaponBobAmplitudeBase;
const dvar_s *bg_weaponBobAmplitudeSprinting;
const dvar_s *bg_weaponBobAmplitudeDtp;
const dvar_s *bg_weaponBobAmplitudeSwimming;
const dvar_s *bg_weaponBobAmplitudeStanding;
const dvar_s *bg_weaponBobAmplitudeDucked;
const dvar_s *bg_weaponBobAmplitudeProne;
const dvar_s *bg_weaponBobAmplitudeRoll;
const dvar_s *bg_weaponBobMax;
const dvar_s *bg_weaponBobLag;
const dvar_s *bg_weaponBobFrequencySwimming;
const dvar_s *bg_weaponBobHeavyWeaponScalar;
const dvar_s *bg_forceExplosiveBullets;
const dvar_s *bg_aimSpreadMoveSpeedThreshold;
const dvar_s *bg_maxGrenadeIndicatorSpeed;
const dvar_s *player_breath_hold_time;
const dvar_s *player_breath_gasp_time;
const dvar_s *player_breath_fire_delay;
const dvar_s *player_breath_gasp_scale;
const dvar_s *player_breath_hold_lerp;
const dvar_s *player_breath_gasp_lerp;
const dvar_s *player_breath_snd_lerp;
const dvar_s *player_breath_snd_delay;
const dvar_s *player_scopeExitOnDamage;
const dvar_s *player_adsExitDelay;
const dvar_s *player_move_factor_on_torso;
const dvar_s *player_debugHealth;
const dvar_s *player_debugSprint;
const dvar_s *player_sustainAmmo;
const dvar_s *player_clipSizeMultiplier;
const dvar_s *player_lastStandBleedoutTime;
const dvar_s *player_lastStandBleedoutTimeNoRevive;
const dvar_s *revive_time_taken;
const dvar_s *player_lastStandHealthOverlayTime;
const dvar_s *player_reviveTriggerRadius;
const dvar_s *player_revivePlayerListCycleTime;
const dvar_s *bg_vsmode_hud;
const dvar_s *player_enableShuffleAnims;
const dvar_s *player_animRunThreshhold;
const dvar_s *player_animWalkThreshhold;
const dvar_s *player_runbkThreshhold;
const dvar_s *player_moveThreshhold;
const dvar_s *player_footstepsThreshhold;
const dvar_s *player_runThreshhold;
const dvar_s *player_knockbackMoveThreshhold;
const dvar_s *player_sprintThreshhold;
const dvar_s *player_waterSpeedScale;
const dvar_s *player_floatSpeed;
const dvar_s *player_disableWeaponsInWater;
const dvar_s *player_sliding_friction;
const dvar_s *player_sliding_wishspeed;
const dvar_s *player_sliding_velocity_cap;
const dvar_s *player_strafeSpeedScale;
const dvar_s *player_backSpeedScale;
const dvar_s *player_strafeAnimCosAngle;
const dvar_s *player_slopeAnimAngle;
const dvar_s *player_spectateSpeedScale;
const dvar_s *player_enduranceSpeedScale;
const dvar_s *player_sprintForwardMinimum;
const dvar_s *player_sprintSpeedScale;
const dvar_s *player_sprintUnlimited;
const dvar_s *player_sprintTime;
const dvar_s *player_sprintMinTime;
const dvar_s *player_sprintRechargePause;
const dvar_s *player_sprintStrafeSpeedScale;
const dvar_s *player_sprintCameraBob;
const dvar_s *player_turnRateScale;
const dvar_s *player_turnAnims;
const dvar_s *player_bayonetLaunchProof;
const dvar_s *player_bayonetLaunchDebugging;
const dvar_s *player_bayonetLaunchZCap;
const dvar_s *xanim_debug;
const dvar_s *animscript_debug;
const dvar_s *anim_debugSpeeds;
const dvar_s *animScript_listAnims;
const dvar_s *player_dmgtimer_timePerPoint;
const dvar_s *player_dmgtimer_maxTime;
const dvar_s *player_dmgtimer_minScale;
const dvar_s *player_dmgtimer_stumbleTime;
const dvar_s *player_dmgtimer_flinchTime;
const dvar_s *bg_shock_soundLoop;
const dvar_s *bg_shock_soundLoopSilent;
const dvar_s *bg_shock_soundEnd;
const dvar_s *bg_shock_soundEndAbort;
const dvar_s *bg_shock_screenType;
const dvar_s *bg_shock_screenBlurBlendTime;
const dvar_s *bg_shock_screenBlurBlendFadeTime;
const dvar_s *bg_shock_screenFlashWhiteFadeTime;
const dvar_s *bg_shock_screenFlashShotFadeTime;
const dvar_s *bg_shock_viewKickPeriod;
const dvar_s *bg_shock_viewKickRadius;
const dvar_s *bg_shock_viewKickFadeTime;
const dvar_s *bg_shock_sound;
const dvar_s *bg_shock_soundFadeInTime;
const dvar_s *bg_shock_soundFadeOutTime;
const dvar_s *bg_shock_soundLoopFadeTime;
const dvar_s *bg_shock_soundLoopEndDelay;
const dvar_s *bg_shock_soundRoomType;
const dvar_s *bg_shock_soundDryLevel;
const dvar_s *bg_shock_soundWetLevel;
const dvar_s *bg_shock_soundModEndDelay;
const dvar_s *bg_shock_soundSnapshot;
const dvar_s *bg_shock_lookControl;
const dvar_s *bg_shock_lookControl_maxpitchspeed;
const dvar_s *bg_shock_lookControl_maxyawspeed;
const dvar_s *bg_shock_lookControl_mousesensitivityscale;
const dvar_s *bg_shock_lookControl_fadeTime;
const dvar_s *bg_shock_movement;
const dvar_s *bg_shock_animation;
const dvar_s *bg_shock_visionset_name;
const dvar_s *bg_shock_visionset_inTime;
const dvar_s *bg_shock_visionset_outTime;
const dvar_s *player_meleeRange;
const dvar_s *player_meleeWidth;
const dvar_s *player_meleeHeight;
const dvar_s *player_bayonetRange;
const dvar_s *player_bayonetTargetDist;
const dvar_s *player_burstFireCooldown;
const dvar_s *bg_gravity;
const dvar_s *bg_lowGravity;
const dvar_s *player_swimTime;
const dvar_s *player_swimDamage;
const dvar_s *player_swimDamagerInterval;
const dvar_s *player_viewLockEnt;
const dvar_s *player_viewRateScale;
const dvar_s *player_topDownCamMode;
const dvar_s *player_topDownCamOffset;
const dvar_s *player_topDownCamAngles;
const dvar_s *player_topDownCamCenterPos;
const dvar_s *player_topDownCursorDist;
const dvar_s *player_topDownCursorPos;
const dvar_s *player_forceRedCrosshair;
const dvar_s *bullet_penetrationMinFxDist;
const dvar_s *cg_cinematicFullscreen;
const dvar_s *cg_debugMounting;
const dvar_s *player_AimBlend_Back_Low;
const dvar_s *player_AimBlend_Back_Mid;
const dvar_s *player_AimBlend_Back_Up;
const dvar_s *player_AimBlend_Pelvis;
const dvar_s *player_AimBlend_Neck;
const dvar_s *player_AimBlend_Head;
const dvar_s *player_AimBlend_Shoulder;
const dvar_s *dog_MeleeDamage;
const dvar_s *vehControlMode;
const dvar_s *vehRecenterDelay;
const dvar_s *vehLockTurretToPlayerView;
const dvar_s *vehLocationalVehicleSeatEntry;
const dvar_s *vehCameraTurretOffset;
const dvar_s *vehCameraTurretOffsetADS;
const dvar_s *vehicle_perk_boost_duration_seconds;
const dvar_s *vehicle_riding;
const dvar_s *vehicle_selfCollision;
const dvar_s *g_bDebugRenderPlayerCollision;
const dvar_s *bg_disableWeaponPlantingInWater;
const dvar_s *bg_plantInWaterDepth;
const dvar_s *bg_drawGrenadeInHand;
const dvar_s *bg_playStandToCrouchAnims;
const dvar_s *bg_maxWeaponAnimScale;
const dvar_s *bg_gunXOffset;
const dvar_s *bg_weaponleftbone;
const dvar_s *bg_weaponrightbone;
const dvar_s *bg_slopeFrames;
const dvar_s *debug_rope;
const dvar_s *vehicle_sounds_cutoff;
const dvar_s *footstep_sounds_cutoff;
const dvar_s *show_reticle_during_swimming;
const dvar_s *ragdoll_reactivation_cutoff;
const dvar_s *bg_teleportAlignTime;
const dvar_s *bg_enableIKActiveFix;
const dvar_s *waterbrush_entity;
const dvar_s *dive2swim;
const dvar_s *dive_recharge;
const dvar_s *playerPushAmount;
const dvar_s *bg_freeCamClipToHeliPatch;


void __cdecl BG_RegisterDvars()
{
  bg_viewKickScale = _Dvar_RegisterFloat(
                       "bg_viewKickScale",
                       0.2,
                       0.0,
                       10.0,
                       0x80u,
                       "The scale to apply to the damage done to caluclate damage view kick");
  bg_viewKickMax = _Dvar_RegisterFloat("bg_viewKickMax", 90.0, 0.0, 90.0, 0x80u, "The maximum view kick");
  bg_viewKickMin = _Dvar_RegisterFloat("bg_viewKickMin", 0.5, 0.0, 90.0, 0x80u, "The minimum view kick");
  bg_viewKickRandom = _Dvar_RegisterFloat(
                        "bg_viewKickRandom",
                        0.40000001,
                        0.0,
                        1.0,
                        0x80u,
                        "The random direction scale view kick");
  player_view_pitch_up = _Dvar_RegisterFloat(
                           "player_view_pitch_up",
                           85.0,
                           -90.0,
                           90.0,
                           0x1180u,
                           "Maximum angle that the player can look up");
  player_view_pitch_down = _Dvar_RegisterFloat(
                             "player_view_pitch_down",
                             85.0,
                             -90.0,
                             90.0,
                             0x1180u,
                             "Maximum angle that the player can look down");
  player_lean_shift = _Dvar_RegisterVec2(
                        "player_lean_shift",
                        unsigned int(5.0),
                        unsigned int(2.5),
                        0.0,
                        20.0,
                        0x80u,
                        "Amount to shift the player 3rd person model when leaning(x:left, y:right)");
  player_lean_shift_crouch = _Dvar_RegisterVec2(
                               "player_lean_shift_crouch",
                               unsigned int(12.5),
                               unsigned int(2.5),
                               0.0,
                               20.0,
                               0x80u,
                               "Amount to shift the player 3rd person model when crouch leaning(x:left, y:right)");
  player_lean_rotate = _Dvar_RegisterVec2(
                         "player_lean_rotate",
                         unsigned int(1.25),
                         unsigned int(1.25),
                         0.0,
                         3.0,
                         0x80u,
                         "Amount to rotate the player 3rd person model when leaning(x:left, y:right)");
  player_lean_rotate_crouch = _Dvar_RegisterVec2(
                                "player_lean_rotate_crouch",
                                unsigned int(1.0),
                                unsigned int(1.0),
                                0.0,
                                3.0,
                                0x80u,
                                "Amount to rotate the player 3rd person model when crouch leaning(x:left, y:right)");
  player_useSlopeAnims = _Dvar_RegisterBool(
                           "player_useSlopeAnims",
                           1,
                           0,
                           "whether to use slope run animations for the player");
  bg_ladder_yawcap = _Dvar_RegisterFloat(
                       "bg_ladder_yawcap",
                       100.0,
                       0.0,
                       360.0,
                       0x180u,
                       "The maximum angle that a player can look around while on a ladder");
  bg_prone_yawcap = _Dvar_RegisterFloat(
                      "bg_prone_yawcap",
                      45.0,
                      0.0,
                      360.0,
                      0x180u,
                      "The maximum angle that a player can look around quickly while prone");
  bg_foliagesnd_minspeed = _Dvar_RegisterFloat(
                             "bg_foliagesnd_minspeed",
                             40.0,
                             0.0,
                             3.4028235e38,
                             0x180u,
                             "The speed that a player must be going to make minimum noise while moving through foliage");
  bg_foliagesnd_maxspeed = _Dvar_RegisterFloat(
                             "bg_foliagesnd_maxspeed",
                             180.0,
                             0.0,
                             3.4028235e38,
                             0x180u,
                             "The speed that a player must be going to make maximum noise while moving through foliage");
  bg_foliagesnd_slowinterval = _Dvar_RegisterInt(
                                 "bg_foliagesnd_slowinterval",
                                 1500,
                                 0,
                                 0x7FFFFFFF,
                                 0x180u,
                                 "The time between each foliage sound when moving slowly");
  bg_foliagesnd_fastinterval = _Dvar_RegisterInt(
                                 "bg_foliagesnd_fastinterval",
                                 500,
                                 0,
                                 0x7FFFFFFF,
                                 0x180u,
                                 "The time between each foliage sound when moving quickly");
  bg_foliagesnd_resetinterval = _Dvar_RegisterInt(
                                  "bg_foliagesnd_resetinterval",
                                  500,
                                  0,
                                  0x7FFFFFFF,
                                  0x180u,
                                  "The time interval before foliage sounds are reset after the player has stopped moving");
  bg_friendlyFire = _Dvar_RegisterInt("scr_team_fftype", 0, 0, 0x7FFFFFFF, 4u, "Friendly fire status");
  bg_fallDamageMinHeight = _Dvar_RegisterFloat(
                             "bg_fallDamageMinHeight",
                             128.0,
                             1.0,
                             3.4028235e38,
                             0x180u,
                             "The height that a player will start to take minimum damage if they fall");
  bg_fallDamageMaxHeight = _Dvar_RegisterFloat(
                             "bg_fallDamageMaxHeight",
                             300.0,
                             1.0,
                             3.4028235e38,
                             0x180u,
                             "The height that a player will take maximum damage when falling");
  friction = _Dvar_RegisterFloat("friction", 5.5, 0.0, 100.0, 0x180u, "Player friction");
  stopspeed = _Dvar_RegisterFloat("stopspeed", 100.0, 0.0, 1000.0, 0x180u, "The player deceleration");
  cg_ufo_scaler = _Dvar_RegisterFloat(
                    "cg_ufo_scaler",
                    6.0,
                    0.001,
                    1000.0,
                    0x180u,
                    "The speed at which ufo camera moves");
  bg_swingSpeed = _Dvar_RegisterFloat(
                    "bg_swingSpeed",
                    0.15000001,
                    0.0,
                    5.0,
                    0x80u,
                    "The rate at which the player's legs swing around when strafing(multi-player only)");
  bg_proneSwingSpeed = _Dvar_RegisterFloat(
                         "bg_proneSwingSpeed",
                         0.079999998,
                         0.0,
                         5.0,
                         0x80u,
                         "The rate at which the player's legs swing around when turning and prone");
  bg_legYawTolerance = _Dvar_RegisterFloat(
                         "bg_legYawTolerance",
                         40.0,
                         0.0,
                         180.0,
                         0x80u,
                         "The amount the player's leg yaw can differ from his torso before moving to match");
  bg_legYawCrouchTolerance = _Dvar_RegisterFloat(
                               "bg_legYawCrouchTolerance",
                               40.0,
                               0.0,
                               180.0,
                               0x80u,
                               "The amount the player's leg yaw can differ from his torso before moving to match");
  bg_legYawProneTolerance = _Dvar_RegisterFloat(
                              "bg_legYawProneTolerance",
                              20.0,
                              0.0,
                              180.0,
                              0x80u,
                              "The amount the player's leg yaw can differ from his torso before moving to match while prone");
  bg_viewBobAmplitudeBase = _Dvar_RegisterFloat(
                              "bg_viewBobAmplitudeBase",
                              0.16,
                              0.0,
                              1.0,
                              0,
                              "The base speed-based view bob amplitude");
  bg_viewBobAmplitudeSprinting = _Dvar_RegisterVec2(
                                   "bg_viewBobAmplitudeSprinting",
                                   unsigned int(0.02),
                                   unsigned int(0.014),
                                   0.0,
                                   1.0,
                                   0x180u,
                                   "The multiplier to apply to the player's speed to get the bob amplitude while sprinting");
  bg_viewBobAmplitudeDtp = _Dvar_RegisterVec2(
                             "bg_viewBobAmplitudeDtp",
                             unsigned int(0.0020000001),
                             unsigned int(0.0020000001),
                             0.0,
                             1.0,
                             0x180u,
                             "The multiplier to apply to the player's speed to get the bob amplitude while diving to prone");
  bg_viewBobAmplitudeSwimming = _Dvar_RegisterVec2(
                                  "bg_viewBobAmplitudeSwimming",
                                  unsigned int(3.0),
                                  unsigned int(2.0),
                                  0.0,
                                  8.0,
                                  0x4080u,
                                  "The multiplier to apply to the player's speed to get the bob amplitude while swimming");
  bg_viewBobAmplitudeStanding = _Dvar_RegisterVec2(
                                  "bg_viewBobAmplitudeStanding",
                                  unsigned int(0.0070000002),
                                  unsigned int(0.0070000002),
                                  0.0,
                                  1.0,
                                  0x4080u,
                                  "The multiplier to apply to the player's speed to get the bob amplitude while standing");
  bg_viewBobAmplitudeStandingAds = _Dvar_RegisterVec2(
                                     "bg_viewBobAmplitudeStandingAds",
                                     unsigned int(0.0070000002),
                                     unsigned int(0.0070000002),
                                     0.0,
                                     1.0,
                                     0x1180u,
                                     "The multiplier to apply to the player's speed to get the view bob amplitude while standing and ADS");
  bg_viewBobAmplitudeDucked = _Dvar_RegisterVec2(
                                "bg_viewBobAmplitudeDucked",
                                unsigned int(0.0074999998),
                                unsigned int(0.0074999998),
                                0.0,
                                1.0,
                                0x180u,
                                "The multiplier to apply to the player's speed to get the bob amplitude while ducking");
  bg_viewBobAmplitudeDuckedAds = _Dvar_RegisterVec2(
                                   "bg_viewBobAmplitudeDuckedAds",
                                   unsigned int(0.0074999998),
                                   unsigned int(0.0074999998),
                                   0.0,
                                   1.0,
                                   0x180u,
                                   "The multiplier to apply to the player's speed to get the view bob amplitude while ducking ADS");
  bg_viewBobAmplitudeProne = _Dvar_RegisterVec2(
                               "bg_viewBobAmplitudeProne",
                               unsigned int(0.079999998),
                               unsigned int(0.039999999),
                               0.0,
                               1.0,
                               0x180u,
                               "The multiplier to apply to the player's speed to get the bob amplitude while prone");
  bg_viewBobAmplitudeRoll = _Dvar_RegisterFloat(
                              "bg_viewBobAmplitudeRoll",
                              1.5,
                              0.0,
                              90.0,
                              0x180u,
                              "The amplitude applied to the roll for view bobbing");
  bg_viewBobMax = _Dvar_RegisterFloat("bg_bobMax", 8.0, 0.0, 36.0, 0x180u, "The maximum allowed bob amplitude");
  bg_forceDurationOverride = _Dvar_RegisterInt(
                               "bg_forceDurationOverride",
                               -1,
                               -1,
                               10000,
                               0x80u,
                               "Force all player animations to have this duration");
  bg_blendTimeOverride = _Dvar_RegisterInt(
                           "bg_blendTimeOverride",
                           -1,
                           -1,
                           10000,
                           0x80u,
                           "Force all player animations to have this blendtime");
  bg_weaponBobAmplitudeBase = _Dvar_RegisterFloat(
                                "bg_weaponBobAmplitudeBase",
                                0.16,
                                0.0,
                                1.0,
                                0,
                                "The base speed-based weapon bob amplitude");
  bg_weaponBobAmplitudeSprinting = _Dvar_RegisterVec2(
                                     "bg_weaponBobAmplitudeSprinting",
                                     unsigned int(0.02),
                                     unsigned int(0.014),
                                     0.0,
                                     1.0,
                                     0x180u,
                                     "The multiplier to apply to the player's speed to get the weapon bob amplitude while sprinting");
  bg_weaponBobAmplitudeDtp = _Dvar_RegisterVec2(
                               "bg_weaponBobAmplitudeDtp",
                               unsigned int(0.0020000001),
                               unsigned int(0.0020000001),
                               0.0,
                               1.0,
                               0x180u,
                               "The multiplier to apply to the player's speed to get the weapon bob amplitude while diving to prone");
  bg_weaponBobAmplitudeSwimming = _Dvar_RegisterVec2(
                                    "bg_weaponBobAmplitudeSwimming",
                                    unsigned int(4.0),
                                    unsigned int(4.0),
                                    0.0,
                                    8.0,
                                    0x4080u,
                                    "The multiplier to apply to the player's speed to get the weapon bob amplitude while swimming");
  bg_weaponBobAmplitudeStanding = _Dvar_RegisterVec2(
                                    "bg_weaponBobAmplitudeStanding",
                                    unsigned int(0.055),
                                    unsigned int(0.025),
                                    0.0,
                                    1.0,
                                    0x4080u,
                                    "The multiplier to apply to the player's speed to get the weapon bob amplitude while standing");
  bg_weaponBobAmplitudeDucked = _Dvar_RegisterVec2(
                                  "bg_weaponBobAmplitudeDucked",
                                  unsigned int(0.045000002),
                                  unsigned int(0.025),
                                  0.0,
                                  1.0,
                                  0x180u,
                                  "The multiplier to apply to the player's speed to get the weapon bob amplitude while ducking");
  bg_weaponBobAmplitudeProne = _Dvar_RegisterVec2(
                                 "bg_weaponBobAmplitudeProne",
                                 unsigned int(0.02),
                                 unsigned int(0.0049999999),
                                 0.0,
                                 1.0,
                                 0x180u,
                                 "The multiplier to apply to the player's speed to get the weapon bob amplitude while prone");
  bg_weaponBobAmplitudeRoll = _Dvar_RegisterFloat(
                                "bg_weaponBobAmplitudeRoll",
                                1.5,
                                0.0,
                                90.0,
                                0x180u,
                                "The amplitude applied to the roll for weapon bobbing");
  bg_weaponBobMax = _Dvar_RegisterFloat(
                      "bg_weaponBobMax",
                      8.0,
                      0.0,
                      36.0,
                      0x180u,
                      "The maximum allowed weapon/viewmodel bob amplitude");
  bg_weaponBobLag = _Dvar_RegisterFloat(
                      "bg_weaponBobLag",
                      0.25,
                      -1.0,
                      1.0,
                      0x180u,
                      "The lag that will be applied the weapon bob cycle");
  bg_weaponBobFrequencySwimming = _Dvar_RegisterFloat(
                                    "bg_weaponBobFrequencySwimming",
                                    0.1,
                                    0.0,
                                    5.0,
                                    0x4080u,
                                    "Controls the speed of the camera bob while swimming");
  bg_weaponBobHeavyWeaponScalar = _Dvar_RegisterFloat(
                                    "bg_weaponBobHeavyWeaponScalar",
                                    1.5,
                                    1.0,
                                    1000.0,
                                    0x4080u,
                                    "Scalar applied to bob angles for heavy weapons (minigun)");
  bg_forceExplosiveBullets = _Dvar_RegisterBool(
                               "bg_forceExplosiveBullets",
                               0,
                               0x180u,
                               "When set, all bullet weapons will fire explosive rounds (Simulates Perk)");
  bg_aimSpreadMoveSpeedThreshold = _Dvar_RegisterFloat(
                                     "bg_aimSpreadMoveSpeedThreshold",
                                     11.0,
                                     0.0,
                                     300.0,
                                     0x180u,
                                     "When player is moving faster than this speed, the aim spread will increase");
  bg_maxGrenadeIndicatorSpeed = _Dvar_RegisterFloat(
                                  "bg_maxGrenadeIndicatorSpeed",
                                  100.0,
                                  0.0,
                                  1000.0,
                                  0x180u,
                                  "Maximum speed of grenade that will show up in indicator and can be thrown back.");
  player_breath_hold_time = _Dvar_RegisterFloat(
                              "player_breath_hold_time",
                              4.5,
                              0.0,
                              30.0,
                              0x180u,
                              "The maximum time a player can hold his breath");
  player_breath_gasp_time = _Dvar_RegisterFloat(
                              "player_breath_gasp_time",
                              1.0,
                              0.0,
                              30.0,
                              0x180u,
                              "The amount of time a player will gasp once they can breath again");
  player_breath_fire_delay = _Dvar_RegisterFloat(
                               "player_breath_fire_delay",
                               0.0,
                               0.0,
                               30.0,
                               0x180u,
                               "The amount of time subtracted from the player remaining breath time when a weapon is fired");
  player_breath_gasp_scale = _Dvar_RegisterFloat(
                               "player_breath_gasp_scale",
                               4.5,
                               0.0,
                               50.0,
                               0x180u,
                               "Scale value to apply to the target waver during a gasp");
  player_breath_hold_lerp = _Dvar_RegisterFloat(
                              "player_breath_hold_lerp",
                              1.0,
                              0.0,
                              50.0,
                              0x180u,
                              "The interpolation rate for the target waver amplitude when holding breath");
  player_breath_gasp_lerp = _Dvar_RegisterFloat(
                              "player_breath_gasp_lerp",
                              6.0,
                              0.0,
                              50.0,
                              0x180u,
                              "The interpolation rate for the target waver amplitude when gasping");
  player_breath_snd_lerp = _Dvar_RegisterFloat(
                             "player_breath_snd_lerp",
                             2.0,
                             0.0,
                             100.0,
                             0x180u,
                             "The interpolation rate for the player hold breath sound");
  player_breath_snd_delay = _Dvar_RegisterFloat(
                              "player_breath_snd_delay",
                              1.0,
                              0.0,
                              2.0,
                              0x180u,
                              "The delay before playing the breathe in sound");
  player_scopeExitOnDamage = _Dvar_RegisterBool(
                               "player_scopeExitOnDamage",
                               0,
                               0x180u,
                               "Exit the scope if the player takes damage");
  player_adsExitDelay = _Dvar_RegisterInt(
                          "player_adsExitDelay",
                          0,
                          0,
                          1000,
                          0x180u,
                          "Delay before exiting aim down sight");
  player_move_factor_on_torso = _Dvar_RegisterFloat(
                                  "player_move_factor_on_torso",
                                  0.0,
                                  0.0,
                                  1.0,
                                  0x80u,
                                  "The contribution movement direction has on player torso direction(multi-player only)");
  player_debugHealth = _Dvar_RegisterBool("player_debugHealth", 0, 0x180u, "Turn on debugging info for player health");
  player_debugSprint = _Dvar_RegisterBool("player_debugSprint", 0, 0x80u, "Turn on debugging info for player sprint");
  player_sustainAmmo = _Dvar_RegisterBool("player_sustainAmmo", 0, 0x80u, "Firing weapon will not decrease clip ammo.");
  player_clipSizeMultiplier = _Dvar_RegisterFloat(
                                "player_clipSizeMultiplier",
                                1.0,
                                0.0,
                                3.4028235e38,
                                0x80u,
                                "Changes the clip size of weapons with more than one bullet in their clip.");
  player_lastStandBleedoutTime = _Dvar_RegisterFloat(
                                   "player_lastStandBleedoutTime",
                                   30.0,
                                   0.0,
                                   3.4028235e38,
                                   0x4080u,
                                   "The time taken by the player to completely bleedout when in last stand (pro version).");
  player_lastStandBleedoutTimeNoRevive = _Dvar_RegisterFloat(
                                           "player_lastStandBleedoutTimeNoRevive",
                                           10.0,
                                           0.0,
                                           3.4028235e38,
                                           0x80u,
                                           "The time taken by the player to completely bleedout when in last stand (non-pro version).");
  revive_time_taken = _Dvar_RegisterFloat(
                        "revive_time_taken",
                        1.15,
                        0.0,
                        3.4028235e38,
                        0x80u,
                        "The time it takes to revive a player in last stand.");
  player_lastStandHealthOverlayTime = _Dvar_RegisterFloat(
                                        "player_lastStandHealthOverlayTime",
                                        8.0,
                                        0.0,
                                        3.4028235e38,
                                        0x80u,
                                        "The time at the end of last stand to apply health (red) overlay");
  player_reviveTriggerRadius = _Dvar_RegisterFloat(
                                 "player_reviveTriggerRadius",
                                 64.0,
                                 0.0,
                                 256.0,
                                 0x80u,
                                 "The radius for the hint string and revive radius");
  player_revivePlayerListCycleTime = _Dvar_RegisterFloat(
                                       "player_revivePlayerListCycleTime",
                                       2.0,
                                       0.0,
                                       3.4028235e38,
                                       0x80u,
                                       "The time for cycling through the list of players who are in last stand.");
  bg_vsmode_hud = _Dvar_RegisterBool("bg_vsmode_hud", 0, 0x180u, "Used to control diferences in vsmode hud.");
  player_enableShuffleAnims = _Dvar_RegisterBool(
                                "player_enableShuffleAnims",
                                0,
                                0x80u,
                                "Whether players should play shuffle anims at low speeds");
  player_animRunThreshhold = _Dvar_RegisterFloat(
                               "player_animRunThreshhold",
                               140.0,
                               0.0,
                               50000.0,
                               0x80u,
                               "The speed at which the player will animate as though they are running");
  player_animWalkThreshhold = _Dvar_RegisterFloat(
                                "player_animWalkThreshhold",
                                30.0,
                                0.0,
                                50000.0,
                                0x80u,
                                "The speed at which the player will animate as though they are walking");
  player_runbkThreshhold = _Dvar_RegisterFloat(
                             "player_runbkThreshhold",
                             60.0,
                             0.0,
                             50000.0,
                             0x180u,
                             "The speed threshold before a player is considered to be running backwards");
  player_moveThreshhold = _Dvar_RegisterFloat(
                            "player_moveThreshhold",
                            2.0,
                            0.0000000099999999,
                            20.0,
                            0x180u,
                            "The speed at which the player is considered to be moving for the purposes of \n"
                            "view model bob and multiplayer model movement");
  player_footstepsThreshhold = _Dvar_RegisterFloat(
                                 "player_footstepsThreshhold",
                                 10.0,
                                 0.0,
                                 50000.0,
                                 0x180u,
                                 "The minimum speed at which the player makes loud footstep noises");
  player_runThreshhold = _Dvar_RegisterFloat(
                           "player_runThreshhold",
                           110.0,
                           0.0,
                           50000.0,
                           0x180u,
                           "The speed threshold before a player is considered to be running forwards");
  player_knockbackMoveThreshhold = _Dvar_RegisterFloat(
                                     "player_knockbackMoveThreshhold",
                                     60.0,
                                     0.0,
                                     50000.0,
                                     0x180u,
                                     "The speed threshold before a player is considered to be running forwards");
  player_sprintThreshhold = _Dvar_RegisterFloat(
                              "player_sprintThreshhold",
                              185.0,
                              0.0,
                              50000.0,
                              0x180u,
                              "The speed threshhold before a player is considered to be sprinting");
  player_waterSpeedScale = _Dvar_RegisterFloat(
                             "player_waterSpeedScale",
                             1.3,
                             0.0,
                             20.0,
                             0x4080u,
                             "Water movement speed scalar.  The larger the value, the faster the movement.");
  player_floatSpeed = _Dvar_RegisterFloat(
                        "player_floatSpeed",
                        20.0,
                        -10000.0,
                        10000.0,
                        0,
                        "The speed at which the player will float up to the top of the water");
  player_disableWeaponsInWater = _Dvar_RegisterBool(
                                   "player_disableWeaponsInWater",
                                   1,
                                   0,
                                   "Disable weapons while in deep water");
  player_sliding_friction = _Dvar_RegisterFloat(
                              "player_sliding_friction",
                              1.5,
                              0.0,
                              100.0,
                              0x4080u,
                              "Controls the friction during slide.");
  player_sliding_wishspeed = _Dvar_RegisterFloat(
                               "player_sliding_wishspeed",
                               4000.0,
                               0.0,
                               10000.0,
                               0x4080u,
                               "Controls how fast the player will slide.");
  player_sliding_velocity_cap = _Dvar_RegisterFloat(
                                  "player_sliding_velocity_cap",
                                  500.0,
                                  0.0,
                                  10000.0,
                                  0x4080u,
                                  "Velocity threshold for the player slide.");
  player_strafeSpeedScale = _Dvar_RegisterFloat(
                              "player_strafeSpeedScale",
                              0.80000001,
                              0.0,
                              20.0,
                              0x180u,
                              "The scale applied to the player speed when strafing");
  player_backSpeedScale = _Dvar_RegisterFloat(
                            "player_backSpeedScale",
                            0.69999999,
                            0.0,
                            20.0,
                            0x180u,
                            "The scale applied to the player speed when moving backwards");
  player_strafeAnimCosAngle = _Dvar_RegisterFloat(
                                "player_strafeAnimCosAngle",
                                0.5,
                                0.0,
                                1.0,
                                0x180u,
                                "Cosine of the angle which player starts using strafe animations");
  player_slopeAnimAngle = _Dvar_RegisterFloat(
                            "player_slopeAnimAngle",
                            30.0,
                            15.0,
                            80.0,
                            0x80u,
                            "Angle at which the player will start animating differently when going up or down slopes");
  player_spectateSpeedScale = _Dvar_RegisterFloat(
                                "player_spectateSpeedScale",
                                1.0,
                                0.0,
                                20.0,
                                0x180u,
                                "The scale applied to the player speed when spectating");
  player_enduranceSpeedScale = _Dvar_RegisterFloat(
                                 "player_enduranceSpeedScale",
                                 1.7,
                                 1.0,
                                 20.0,
                                 0x180u,
                                 "The scale applied to the player speed when they have endurance perk");
  player_sprintForwardMinimum = _Dvar_RegisterInt(
                                  "player_sprintForwardMinimum",
                                  105,
                                  0,
                                  255,
                                  0x100u,
                                  "The minimum forward deflection required to maintain a sprint");
  player_sprintSpeedScale = _Dvar_RegisterFloat(
                              "player_sprintSpeedScale",
                              1.5,
                              0.0,
                              5.0,
                              0x180u,
                              "The scale applied to the player speed when sprinting");
  player_sprintUnlimited = _Dvar_RegisterBool(
                             "player_sprintUnlimited",
                             0,
                             0x180u,
                             "Whether players can sprint forever or not");
  player_sprintTime = _Dvar_RegisterFloat(
                        "player_sprintTime",
                        4.0,
                        0.0,
                        12.8,
                        0x180u,
                        "The base length of time a player can sprint");
  player_sprintMinTime = _Dvar_RegisterFloat(
                           "player_sprintMinTime",
                           1.0,
                           0.0,
                           12.8,
                           0x180u,
                           "The minimum sprint time needed in order to start sprinting");
  player_sprintRechargePause = _Dvar_RegisterFloat(
                                 "player_sprintRechargePause",
                                 0.0,
                                 0.0,
                                 9000.0,
                                 0x180u,
                                 "The length of time the meter will pause before starting to recharge after a player sprints");
  player_sprintStrafeSpeedScale = _Dvar_RegisterFloat(
                                    "player_sprintStrafeSpeedScale",
                                    0.667,
                                    0.0,
                                    5000.0,
                                    0x180u,
                                    "The speed at which you can strafe while sprinting");
  player_sprintCameraBob = _Dvar_RegisterFloat(
                             "player_sprintCameraBob",
                             0.5,
                             0.0,
                             2.0,
                             0x180u,
                             "The speed the camera bobs while you sprint");
  player_turnRateScale = _Dvar_RegisterFloat(
                           "player_turnRateScale",
                           1.0,
                           0.0,
                           100.0,
                           0x4000u,
                           "Scales the rotation rate of the player's view.");
  player_turnAnims = _Dvar_RegisterBool(
                       "player_turnAnims",
                       1,
                       0x180u,
                       "Use animations to turn a player's model in multiplayer");
  player_bayonetLaunchProof = _Dvar_RegisterBool(
                                "player_bayonetLaunchProof",
                                1,
                                0x80u,
                                "Adds extra code to check for bayonet launching, preventing related exploits.");
  player_bayonetLaunchDebugging = _Dvar_RegisterBool(
                                    "player_bayonetLaunchDebugging",
                                    0,
                                    0x80u,
                                    "Simulates bayonet charge without requiring an enemy to do so.");
  player_bayonetLaunchZCap = _Dvar_RegisterFloat(
                               "player_bayonetLaunchZCap",
                               300.0,
                               0.0,
                               500.0,
                               0x80u,
                               "Max Z velocity during bayonet charge.");
  xanim_debug = _Dvar_RegisterBool("xanim_debug", 0, 0, "Turn on Xanim Debugging information");
  animscript_debug = _Dvar_RegisterBool("animscript_debug", 0, 0, "Turn on animscript debugging information");
  anim_debugSpeeds = _Dvar_RegisterBool("anim_debugSpeeds", 0, 0, "Print out animation speed information");
  animScript_listAnims = _Dvar_RegisterBool("animScript_listAnims", 0, 0, "Print out animation list");
  player_dmgtimer_timePerPoint = _Dvar_RegisterFloat(
                                   "player_dmgtimer_timePerPoint",
                                   100.0,
                                   0.0,
                                   3.4028235e38,
                                   0x180u,
                                   "The time in milliseconds that the player is slowed down per point of damage");
  player_dmgtimer_maxTime = _Dvar_RegisterFloat(
                              "player_dmgtimer_maxTime",
                              750.0,
                              0.0,
                              3.4028235e38,
                              0x180u,
                              "The maximum time that the player is slowed due to damage");
  player_dmgtimer_minScale = _Dvar_RegisterFloat(
                               "player_dmgtimer_minScale",
                               0.0,
                               0.0,
                               1.0,
                               0x180u,
                               "The minimum scale value to slow the player by when damaged");
  player_dmgtimer_stumbleTime = _Dvar_RegisterInt(
                                  "player_dmgtimer_stumbleTime",
                                  500,
                                  0,
                                  2000,
                                  0x180u,
                                  "Maximum time to play stumble animations");
  player_dmgtimer_flinchTime = _Dvar_RegisterInt(
                                 "player_dmgtimer_flinchTime",
                                 500,
                                 0,
                                 2000,
                                 0x180u,
                                 "Maximum time to play flinch animations");
  bg_shock_soundLoop = _Dvar_RegisterString("bg_shock_soundLoop", "shellshock_loop", 0x80u, "Shellshock loop alias");
  bg_shock_soundLoopSilent = _Dvar_RegisterString(
                               "bg_shock_soundLoopSilent",
                               "shellshock_loop_silent",
                               0x80u,
                               "The sound that gets blended with the shellshock loop alias");
  bg_shock_soundEnd = _Dvar_RegisterString("bg_shock_soundEnd", "shellshock_end", 0x80u, "Shellshock end sound alias");
  bg_shock_soundEndAbort = _Dvar_RegisterString(
                             "bg_shock_soundEndAbort",
                             "shellshock_end_abort",
                             0x80u,
                             "Shellshock aborted end sound alias");
  bg_shock_screenType = _Dvar_RegisterEnum(
                          "bg_shock_screenType",
                          bg_ShockScreenTypeNames,
                          0,
                          0x80u,
                          "Shell shock screen effect type");
  bg_shock_screenBlurBlendTime = _Dvar_RegisterFloat(
                                   "bg_shock_screenBlurBlendTime",
                                   0.40000001,
                                   0.001,
                                   10.0,
                                   0x80u,
                                   "The amount of time in seconds for the shellshock effect to blend");
  bg_shock_screenBlurBlendFadeTime = _Dvar_RegisterFloat(
                                       "bg_shock_screenBlurBlendFadeTime",
                                       1.0,
                                       0.001,
                                       1000.0,
                                       0x80u,
                                       "The amount of time in seconds for the shellshock effect to fade");
  bg_shock_screenFlashWhiteFadeTime = _Dvar_RegisterFloat(
                                        "bg_shock_screenFlashWhiteFadeTime",
                                        1.0,
                                        0.0,
                                        1000.0,
                                        0x80u,
                                        "In seconds, how soon from the end of the effect to start blending out the whiteout layer.");
  bg_shock_screenFlashShotFadeTime = _Dvar_RegisterFloat(
                                       "bg_shock_screenFlashShotFadeTime",
                                       1.0,
                                       0.0,
                                       1000.0,
                                       0x80u,
                                       "In seconds, how soon from the end of the effect to start blending out the screengrab layer.");
  bg_shock_viewKickPeriod = _Dvar_RegisterFloat(
                              "bg_shock_viewKickPeriod",
                              0.75,
                              0.001,
                              1000.0,
                              0x80u,
                              "The period of the shellshock view kick effect");
  bg_shock_viewKickRadius = _Dvar_RegisterFloat(
                              "bg_shock_viewKickRadius",
                              0.050000001,
                              0.0,
                              1.0,
                              0x80u,
                              "Shell shock kick radius");
  bg_shock_viewKickFadeTime = _Dvar_RegisterFloat(
                                "bg_shock_viewKickFadeTime",
                                3.0,
                                0.001,
                                1000.0,
                                0x80u,
                                "The time for the shellshock kick effect to fade");
  bg_shock_sound = _Dvar_RegisterBool("bg_shock_sound", 1, 0x80u, "Play shell shock sound");
  bg_shock_soundFadeInTime = _Dvar_RegisterFloat(
                               "bg_shock_soundFadeInTime",
                               0.25,
                               0.001,
                               1000.0,
                               0x80u,
                               "Shell shock sound fade in time in seconds");
  bg_shock_soundFadeOutTime = _Dvar_RegisterFloat(
                                "bg_shock_soundFadeOutTime",
                                2.5,
                                0.0,
                                1000.0,
                                0x80u,
                                "Shell shock sound fade out time in seconds");
  bg_shock_soundLoopFadeTime = _Dvar_RegisterFloat(
                                 "bg_shock_soundLoopFadeTime",
                                 1.5,
                                 0.0,
                                 1000.0,
                                 0x80u,
                                 "Shell shock sound loop fade time in seconds");
  bg_shock_soundLoopEndDelay = _Dvar_RegisterFloat(
                                 "bg_shock_soundLoopEndDelay",
                                 -3.0,
                                 -10.0,
                                 1000.0,
                                 0x80u,
                                 "Sound loop end offset time from the end of the shellshock in seconds");
  bg_shock_soundRoomType = _Dvar_RegisterString("bg_shock_soundRoomType", "default", 0x80u, "sound room");
  bg_shock_soundDryLevel = _Dvar_RegisterFloat(
                             "bg_shock_soundDryLevel",
                             1.0,
                             0.0,
                             1.0,
                             0x80u,
                             "Shell shock sound dry level");
  bg_shock_soundWetLevel = _Dvar_RegisterFloat(
                             "bg_shock_soundWetLevel",
                             0.5,
                             0.0,
                             1.0,
                             0x80u,
                             "Shell shock sound wet level");
  bg_shock_soundModEndDelay = _Dvar_RegisterFloat(
                                "bg_shock_soundModEndDelay",
                                2.0,
                                -1000.0,
                                1000.0,
                                0x80u,
                                "The delay from the end of the shell shock to the end of the sound modification");
  bg_shock_soundSnapshot = _Dvar_RegisterString("bg_shock_soundSnapshot", "default", 0x80u, "sound snapshot");
  bg_shock_lookControl = _Dvar_RegisterBool("bg_shock_lookControl", 1, 0x80u, "Alter player control during shellshock");
  bg_shock_lookControl_maxpitchspeed = _Dvar_RegisterFloat(
                                         "bg_shock_lookControl_maxpitchspeed",
                                         90.0,
                                         0.0,
                                         3.4028235e38,
                                         0x80u,
                                         "Maximum pitch movement rate while shellshocked in degrees per second");
  bg_shock_lookControl_maxyawspeed = _Dvar_RegisterFloat(
                                       "bg_shock_lookControl_maxyawspeed",
                                       90.0,
                                       0.0,
                                       3.4028235e38,
                                       0x80u,
                                       "Maximum yaw movement rate while shell shocked in degrees per second");
  bg_shock_lookControl_mousesensitivityscale = _Dvar_RegisterFloat(
                                                 "bg_shock_lookControl_mousesensitivityscale",
                                                 0.5,
                                                 0.0,
                                                 2.0,
                                                 0x80u,
                                                 "Sensitivity scale to apply to a shellshocked player");
  bg_shock_lookControl_fadeTime = _Dvar_RegisterFloat(
                                    "bg_shock_lookControl_fadeTime",
                                    2.0,
                                    0.001,
                                    1000.0,
                                    0x80u,
                                    "The time for the shellshock player control to fade in seconds");
  bg_shock_movement = _Dvar_RegisterFloat(
                        "bg_shock_movement",
                        1.0,
                        0.0,
                        1.0,
                        0x80u,
                        "Affect player's movement speed during shellshock");
  bg_shock_animation = _Dvar_RegisterBool(
                         "bg_shock_animation",
                         0,
                         0x80u,
                         "Play the 3rd person animation during shellshock");
  bg_shock_visionset_name = _Dvar_RegisterString(
                              "bg_shock_visionset_name",
                              (char *)&toastPopupTitle,
                              0x80u,
                              "Vision set that will be transitioned to when this shellshock plays");
  bg_shock_visionset_inTime = _Dvar_RegisterFloat(
                                "bg_shock_visionset_inTime",
                                0.0,
                                0.0,
                                30.0,
                                0x80u,
                                "Vision set in transition time (in seconds)");
  bg_shock_visionset_outTime = _Dvar_RegisterFloat(
                                 "bg_shock_visionset_outTime",
                                 0.0,
                                 0.0,
                                 30.0,
                                 0x80u,
                                 "Vision set out transition time (in seconds)");
  player_meleeRange = _Dvar_RegisterFloat(
                        "player_meleeRange",
                        64.0,
                        0.0,
                        1000.0,
                        0x80u,
                        "The maximum range of the player's melee attack");
  player_meleeWidth = _Dvar_RegisterFloat(
                        "player_meleeWidth",
                        10.0,
                        0.0,
                        1000.0,
                        0x80u,
                        "The width of the player's melee attack");
  player_meleeHeight = _Dvar_RegisterFloat(
                         "player_meleeHeight",
                         10.0,
                         0.0,
                         1000.0,
                         0x80u,
                         "The height of the player's melee attack");
  player_bayonetRange = _Dvar_RegisterFloat(
                          "player_bayonetRange",
                          85.0,
                          0.0,
                          1000.0,
                          0x80u,
                          "The maximum range of the player's bayonet attack");
  player_bayonetTargetDist = _Dvar_RegisterInt(
                               "player_bayonetTargetDist",
                               65,
                               0,
                               1000,
                               0x80u,
                               "The distance to the enemy the player's will stop charging");
  player_burstFireCooldown = _Dvar_RegisterFloat(
                               "player_burstFireCooldown",
                               0.2,
                               0.0,
                               60.0,
                               0x80u,
                               "Seconds after a burst fire before weapons can be fired again.");
  bg_gravity = _Dvar_RegisterFloat(
                 "bg_gravity",
                 800.0,
                 1.0,
                 3.4028235e38,
                 0x2080u,
                 "Gravity in inches per second per second");
  bg_lowGravity = _Dvar_RegisterFloat(
                    "bg_lowGravity",
                    400.0,
                    1.0,
                    3.4028235e38,
                    0x2080u,
                    "Low gravity for slow or floaty objects, in inches per second per second");
  player_swimTime = _Dvar_RegisterFloat(
                      "player_swimTime",
                      16.0,
                      0.0,
                      5000.0,
                      0x4080u,
                      "Set the max time the player can swim underwater before taking damage.");
  player_swimDamage = _Dvar_RegisterInt(
                        "player_swimDamage",
                        15,
                        0,
                        5000,
                        0x4080u,
                        "Damage the player will take each interval while out of breath in deep water.");
  player_swimDamagerInterval = _Dvar_RegisterFloat(
                                 "player_swimDamagerInterval",
                                 1.0,
                                 0.0,
                                 5000.0,
                                 0x4080u,
                                 "Interval in which damage will be dealt while out of breath under water.");
  player_viewLockEnt = _Dvar_RegisterInt(
                         "player_viewLockEnt",
                         0,
                         0,
                         1024,
                         0x1000u,
                         "Set an entity that the player view will be locked too.");
  player_viewRateScale = _Dvar_RegisterFloat(
                           "player_viewRateScale",
                           0.0,
                           0.0,
                           100.0,
                           0x1000u,
                           "Player view rate scale modifier when player doesn't have a weapon.");
  player_topDownCamMode = _Dvar_RegisterInt("player_topDownCamMode", 0, 0, 4, 0x1000u, "Enabled the top down cam.");
  player_topDownCamOffset = _Dvar_RegisterVec3(
                              "player_topDownCamOffset",
                              unsigned int(0.0),
                              unsigned int(0.0),
                              unsigned int(500.0),
                              -3000.0,
                              3000.0,
                              0x1000u,
                              "Sets the camera position relative to the player for the top down cam");
  player_topDownCamAngles = _Dvar_RegisterVec3(
                              "player_topDownCamAngles",
                              unsigned int(75.0),
                              unsigned int(0.0),
                              unsigned int(0.0),
                              -180.0,
                              180.0,
                              0x1000u,
                              "Sets the pitch for the top down cam");
  player_topDownCamCenterPos = _Dvar_RegisterVec3(
                                 "player_topDownCamCenterPos",
                                 unsigned int(-7926.0),
                                 unsigned int(-2838.0),
                                 unsigned int(-88.0),
                                 -20000.0,
                                 20000.0,
                                 0x1000u,
                                 "Set a top down camera influence position, player_topDownCamMode must be set to 3 to use this dvar");
  player_topDownCursorDist = _Dvar_RegisterFloat(
                               "player_topDownCursorDist",
                               250.0,
                               1.0,
                               3.4028235e38,
                               0x1000u,
                               "Distance the virtual mouse cursor is from the player.");
  player_topDownCursorPos = _Dvar_RegisterVec2(
                              "player_topDownCursorPos",
                              unsigned int(0.0),
                              unsigned int(0.0),
                              -3.4028235e38,
                              3.4028235e38,
                              0x1000u,
                              "Position of the virtual mouse cursor is from the player relative to the player.");
  player_forceRedCrosshair = _Dvar_RegisterBool(
                               "player_forceRedCrosshair",
                               0,
                               0x1000u,
                               "Force the player's crosshair to turn red");
  bullet_penetrationMinFxDist = _Dvar_RegisterFloat(
                                  "bullet_penetrationMinFxDist",
                                  30.0,
                                  0.0,
                                  1024.0,
                                  0x80u,
                                  "Min distance a penetrated bullet must travel before it'll trigger the effects");
  cg_cinematicFullscreen = _Dvar_RegisterBool(
                             "cg_cinematicFullscreen",
                             1,
                             0x1000u,
                             "Draw ingame cinematics full screen");
  cg_debugMounting = _Dvar_RegisterInt(
                       "cg_debugMounting",
                       0,
                       0,
                       3,
                       0x80u,
                       "Render debug lines that show where mountable weapons are trying to be placed");
  player_AimBlend_Back_Low = _Dvar_RegisterVec3(
                               "player_AimBlend_Back_Low",
                               unsigned int(0.0),
                               unsigned int(0.30000001),
                               unsigned int(0.5),
                               -1.0,
                               1.0,
                               0,
                               "3rd person player view aim blend - lower back");
  player_AimBlend_Back_Mid = _Dvar_RegisterVec3(
                               "player_AimBlend_Back_Mid",
                               unsigned int(0.1),
                               unsigned int(0.2),
                               unsigned int(0.5),
                               -1.0,
                               1.0,
                               0,
                               "3rd person player view aim blend - mid back");
  player_AimBlend_Back_Up = _Dvar_RegisterVec3(
                              "player_AimBlend_Back_Up",
                              unsigned int(0.5),
                              unsigned int(0.1),
                              unsigned int(-0.60000002),
                              -1.0,
                              1.0,
                              0,
                              "3rd person player view aim blend - upper back");
  player_AimBlend_Pelvis = _Dvar_RegisterVec3(
                             "player_AimBlend_Pelvis",
                             unsigned int(0.40000001),
                             unsigned int(0.40000001),
                             unsigned int(0.0),
                             -1.0,
                             1.0,
                             0,
                             "3rd person player view aim blend - pelvis");
  player_AimBlend_Neck = _Dvar_RegisterVec3(
                           "player_AimBlend_Neck",
                           unsigned int(0.30000001),
                           unsigned int(0.30000001),
                           unsigned int(0.0),
                           -1.0,
                           1.0,
                           0,
                           "3rd person player view aim blend - neck");
  player_AimBlend_Head = _Dvar_RegisterVec3(
                           "player_AimBlend_Head",
                           unsigned int(0.0),
                           unsigned int(0.0),
                           unsigned int(0.0),
                           -1.0,
                           1.0,
                           0,
                           "3rd person player view aim blend - head");
  player_AimBlend_Shoulder = _Dvar_RegisterVec3(
                               "player_AimBlend_Shoulder",
                               unsigned int(1.0),
                               unsigned int(1.0),
                               unsigned int(1.0),
                               -1.0,
                               1.0,
                               0,
                               "3rd person player view aim blend - shoulder");
  dog_MeleeDamage = _Dvar_RegisterInt(
                      "dog_MeleeDamage",
                      150,
                      1,
                      10000,
                      0x1000u,
                      "Controls the damage done when dogs attack.");
  vehControlMode = _Dvar_RegisterInt("vehControlMode", 1, 0, 1, 0, "Changes the vehicle controls and camera mode");
  vehRecenterDelay = _Dvar_RegisterInt(
                       "vehRecenterDelay",
                       3000,
                       0,
                       10000,
                       0,
                       "Specifies a delay in msec, before triggering the camera auto-centering functionality in idle mode");
  vehLockTurretToPlayerView = _Dvar_RegisterBool(
                                "vehLockTurretToPlayerView",
                                0,
                                0x80u,
                                "Locks the turret angles to the player angles and sets the players rotation speed to the turrets rotRate");
  vehLocationalVehicleSeatEntry = _Dvar_RegisterBool(
                                    "vehLocationalVehicleSeatEntry",
                                    1,
                                    0x80u,
                                    "Assigns the player seat based on the entry location, NOT first in first available position");
  vehCameraTurretOffset = _Dvar_RegisterVec3(
                            "vehCameraTurretOffset",
                            unsigned int(0.0),
                            unsigned int(0.0),
                            unsigned int(0.0),
                            -300.0,
                            300.0,
                            0x80u,
                            "Vehicle turret camera offset");
  vehCameraTurretOffsetADS = _Dvar_RegisterVec3(
                               "vehCameraTurretOffsetADS",
                               unsigned int(0.0),
                               unsigned int(0.0),
                               unsigned int(0.0),
                               -300.0,
                               300.0,
                               0x80u,
                               "Vehicle turret camera offset ADS");
  vehicle_perk_boost_duration_seconds = _Dvar_RegisterFloat(
                                          "vehicle_perk_boost_duration_seconds",
                                          0.74000001,
                                          0.0,
                                          3.4028235e38,
                                          0,
                                          "Vehicle perk boost duration in seconds.");
  vehicle_riding = _Dvar_RegisterBool(
                     "vehicle_riding",
                     1,
                     0x1000u,
                     "Turning this on enables players to walk around on top of vehicles");
  vehicle_selfCollision = _Dvar_RegisterBool(
                            "vehicle_selfCollision",
                            0,
                            0x1000u,
                            "Enabled the vehicle to shoot itself with it's own turret.");
  g_bDebugRenderPlayerCollision = _Dvar_RegisterBool(
                                    "debugRenderPlayerCollision",
                                    0,
                                    0,
                                    "Render collision geometry that the player is touching");
  bg_disableWeaponPlantingInWater = _Dvar_RegisterBool(
                                      "bg_disableWeaponPlantingInWater",
                                      1,
                                      0,
                                      "Disables being able to plant mines in the water.");
  bg_plantInWaterDepth = _Dvar_RegisterFloat(
                           "bg_plantInWaterDepth",
                           5.0,
                           0.0,
                           1024.0,
                           0,
                           "Min distance a penetrated bullet must travel before it'll trigger the effects");
  bg_drawGrenadeInHand = _Dvar_RegisterBool(
                           "bg_drawGrenadeInHand",
                           0,
                           0,
                           "Whether or not to draw grenade in hand for grenade animations");
  bg_playStandToCrouchAnims = _Dvar_RegisterBool(
                                "bg_playStandToCrouchAnims",
                                0,
                                0,
                                "Whether or not to use animations to transition between stand and crouch, if not it just blends");
  bg_maxWeaponAnimScale = _Dvar_RegisterFloat(
                            "bg_maxWeaponAnimScale",
                            0.66000003,
                            0.0,
                            3.0,
                            0,
                            "The amount which weapon timers can cause player animations to scale their playback speed by");
  bg_gunXOffset = _Dvar_RegisterFloat(
                    "bg_gunXOffset",
                    0.0,
                    -1000.0,
                    1000.0,
                    0x4000u,
                    "This will offset where the bullets come out of the gun");
  bg_weaponleftbone = _Dvar_RegisterString("bg_weaponleftbone", "tag_weapon_left", 0, "Left hand weapon bone name");
  bg_weaponrightbone = _Dvar_RegisterString(
                         "bg_weaponrightbone",
                         "tag_weapon_right",
                         0,
                         "Right handed weapon bone name");
  bg_slopeFrames = _Dvar_RegisterInt(
                     "bg_slopeFrames",
                     10,
                     0,
                     30,
                     0x80u,
                     "The number of frames to use to determine slope run animations");
  debug_rope = _Dvar_RegisterBool("debug_rope", 0, 0, "Enables the debug rendering for the ropes");
  vehicle_sounds_cutoff = _Dvar_RegisterFloat(
                            "vehicle_sounds_cutoff",
                            3000.0,
                            0.0,
                            30000.0,
                            0x1000u,
                            "Use to turn of the vehicle sounds if the distance between the camera and the vehicle is more"
                            " than this value");
  footstep_sounds_cutoff = _Dvar_RegisterFloat(
                             "footstep_sounds_cutoff",
                             1000.0,
                             0.0,
                             30000.0,
                             0x4000u,
                             "Footstep sounds distance cuttoff");
  show_reticle_during_swimming = _Dvar_RegisterBool(
                                   "show_reticle_during_swimming",
                                   0,
                                   0x4000u,
                                   "Controls if the reticle is visible during swimming.");
  ragdoll_reactivation_cutoff = _Dvar_RegisterFloat(
                                  "ragdoll_reactivation_cutoff",
                                  1200.0,
                                  0.0,
                                  30000.0,
                                  0x4000u,
                                  "Ragdoll reactivation distance cutoff");
  bg_teleportAlignTime = _Dvar_RegisterInt(
                           "bg_teleportAlignTime",
                           500,
                           -1,
                           2000,
                           0x80u,
                           "How long to align the player body after teleporting/respawning");
  bg_enableIKActiveFix = _Dvar_RegisterBool(
                           "bg_enableIKActiveFix",
                           1,
                           0,
                           "Enable fix for the IK active flag being lost whenever a player dobj is rebuilt");
  waterbrush_entity = _Dvar_RegisterInt(
                        "waterbrush_entity",
                        -1,
                        -1,
                        1024,
                        0x80u,
                        "CM_GetWaterHeight function will test against this enitty. Can be used on brushmodels to move the water level");
  dive2swim = _Dvar_RegisterFloat(
                "dive2swim",
                200.0,
                0.0,
                1000.0,
                0x4000u,
                "Use to contorl the dive force applied to the player when pressing B in water.");
  dive_recharge = _Dvar_RegisterFloat(
                    "dive_recharge",
                    1000.0,
                    0.0,
                    100000.0,
                    0x4000u,
                    "Use to space out the dive to swim button presses.");
  playerPushAmount = _Dvar_RegisterFloat(
                       "playerPushAmount",
                       0.0,
                       0.0,
                       1000.0,
                       0x1000u,
                       "If this value is set the player will get pushed away from AIs by the amount specified.");
  bg_freeCamClipToHeliPatch = _Dvar_RegisterBool(
                                "bg_freeCamClipToHeliPatch",
                                1,
                                0,
                                "Toggles clipping to the heli patch rectangle.");
  Jump_RegisterDvars();
  Mantle_RegisterDvars();
  Dtp_RegisterDvars();
  Dog_RegisterDvars();
  VehAnim_RegisterDvars();
  Perks_RegisterDvars();
  zero_idle_movement = _Dvar_RegisterBool("zero_idle_movement", 0, 0x80u, "Turn the ADS idle movement off.");
  BG_Flame_RegisterDvars();
  BG_Wind_RegisterDvars();
}

char *__cdecl BG_GetEntityTypeName(int eType)
{
  if ( eType < 21 )
    return entityTypeNames[eType];
  if ( (unsigned int)(eType - 21) >= 0xCE
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
          1127,
          0,
          "%s\n\t(eType) = %i",
          "(eType - ET_EVENTS >= 0 && eType - ET_EVENTS < EV_MAX_EVENTS)",
          eType) )
  {
    __debugbreak();
  }
  return va("Event %s (%i)", entityTypeNames[eType + 1], eType - 21);
}

const gitem_s *__cdecl BG_FindItemForWeapon(unsigned int weapon)
{
  unsigned int NumWeapons; // eax

  if ( weapon >= BG_GetNumWeapons() )
  {
    NumWeapons = BG_GetNumWeapons();
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
            1140,
            0,
            "weapon doesn't index BG_GetNumWeapons()\n\t%i not in [0, %i)",
            weapon,
            NumWeapons) )
      __debugbreak();
  }
  return (const gitem_s *)&bg_itemlist[weapon];
}

const gitem_s *__cdecl G_FindItem(char *pickupName)
{
  int iIndex; // [esp+0h] [ebp-4h]

  iIndex = G_GetWeaponIndexForName(pickupName);
  if ( iIndex )
    return (const gitem_s *)(4 * iIndex + 15073304);
  else
    return 0;
}

bool __cdecl BG_PlayerTouchesItem(const playerState_s *ps, const entityState_s *item, int atTime)
{
  float origin[3]; // [esp+0h] [ebp-Ch] BYREF

  BG_EvaluateTrajectory(&item->lerp.pos, atTime, origin);
  return (float)(ps->origin[0] - origin[0]) <= 36.0
      && (float)(ps->origin[0] - origin[0]) >= -36.0
      && (float)(ps->origin[1] - origin[1]) <= 36.0
      && (float)(ps->origin[1] - origin[1]) >= -36.0
      && (float)(ps->origin[2] - origin[2]) <= 18.0
      && (float)(ps->origin[2] - origin[2]) >= -88.0;
}

bool __cdecl BG_PlayerCanPickUpWeaponType(const WeaponDef *weapDef, const playerState_s *ps)
{
  if ( !weapDef && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp", 1233, 0, "%s", "weapDef") )
    __debugbreak();
  if ( !ps && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp", 1234, 0, "%s", "ps") )
    __debugbreak();
  if ( weapDef->offhandClass == OFFHAND_CLASS_FLASH_GRENADE && ps->offhandSecondary != PLAYER_OFFHAND_SECONDARY_FLASH )
    return 0;
  return weapDef->offhandClass != OFFHAND_CLASS_SMOKE_GRENADE || ps->offhandSecondary == PLAYER_OFFHAND_SECONDARY_SMOKE;
}

bool __cdecl BG_CanItemBeGrabbed(const entityState_s *ent, const playerState_s *ps, int touched)
{
  const char *v4; // eax
  const WeaponVariantDef *weapVariantDef; // [esp+0h] [ebp-Ch]
  int weapIdx; // [esp+4h] [ebp-8h]

  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp", 1302, 0, "%s", "ent") )
    __debugbreak();
  if ( !ps && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp", 1303, 0, "%s", "ps") )
    __debugbreak();
  if ( (ps->weapFlags & 0x80) != 0 )
    return 0;
  if ( ent->un3.item < 1 || ent->un3.item >= 2048 )
  {
    v4 = va(&byte_C67F30, ent->un3.item, ent->eType);
    Com_Error(ERR_DROP, v4);
  }
  if ( ent->clientNum == ps->clientNum )
    return 0;
  weapIdx = ent->un3.item % 2048;
  BG_GetWeaponDef(weapIdx);
  if ( bg_itemlist[2048 * (ent->un3.item / 2048) + weapIdx] != 1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
          1319,
          0,
          "%s",
          "bg_itemlist[ ITEM_WEAPMODEL( ent->un3.item ) * MAX_WEAPONS + weapIdx].giType == IT_WEAPON") )
  {
    __debugbreak();
  }
  if ( WeaponEntCanBeGrabbed(ent, ps, touched, weapIdx) )
    return 1;
  weapVariantDef = BG_GetWeaponVariantDef(weapIdx);
  return weapVariantDef->altWeaponIndex && WeaponEntCanBeGrabbed(ent, ps, touched, weapVariantDef->altWeaponIndex);
}

char __cdecl WeaponEntCanBeGrabbed(
        const entityState_s *weaponEntState,
        const playerState_s *ps,
        int touched,
        unsigned int weapIdx)
{
  char haveRoomForAmmo; // [esp+2Bh] [ebp-5h]
  const WeaponDef *weapDef; // [esp+2Ch] [ebp-4h]

  weapDef = BG_GetWeaponDef(weapIdx);
  if ( !BG_PlayerCanPickUpWeaponType(weapDef, ps) )
    return 0;
  if ( weaponEntState->eType == 4 && weapDef->bThrowBack )
    return 1;
  if ( touched )
  {
    haveRoomForAmmo = HaveRoomForAmmo(ps, weapIdx);
    if ( weapDef->offhandClass == OFFHAND_CLASS_FRAG_GRENADE && haveRoomForAmmo )
      return 1;
    if ( (BG_PlayerHasWeapon(ps, weapIdx) || BG_PlayerHasCompatibleWeapon(ps, weapIdx)) && haveRoomForAmmo )
      return 1;
    if ( ((unsigned int)&loc_800000 & weaponEntState->lerp.eFlags2) != 0 && (ps->perks[1] & 0x200) != 0 )
      return 1;
  }
  else if ( !BG_PlayerHasWeapon(ps, weapIdx) )
  {
    return 1;
  }
  return 0;
}

bool __cdecl BG_PlayerHasWeapon(const playerState_s *ps, unsigned int weaponIndex)
{
  if ( !ps && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../bgame/bg_weapons.h", 343, 0, "%s", "ps") )
    __debugbreak();
  return BG_GetHeldWeaponConst(ps, weaponIndex) != 0;
}

const PlayerHeldWeapon *__cdecl BG_GetHeldWeaponConst(const playerState_s *ps, unsigned int weaponIndex)
{
  int slot; // [esp+4h] [ebp-4h]

  if ( !ps && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../bgame/bg_weapons.h", 315, 0, "%s", "ps") )
    __debugbreak();
  if ( !weaponIndex )
    return 0;
  slot = BG_GetHeldWeaponSlot(ps, weaponIndex);
  if ( slot == -1 )
    return 0;
  else
    return &ps->heldWeapons[slot];
}

int __cdecl BG_GetHeldWeaponSlot(const playerState_s *ps, unsigned int weaponIndex)
{
  int slot; // [esp+0h] [ebp-4h]

  if ( !ps && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../bgame/bg_weapons.h", 301, 0, "%s", "ps") )
    __debugbreak();
  for ( slot = 0; slot < 15; ++slot )
  {
    if ( weaponIndex == ps->heldWeapons[slot].weapon )
      return slot;
  }
  return -1;
}

char __cdecl HaveRoomForAmmo(const playerState_s *ps, unsigned int weaponIndex)
{
  int slot; // [esp+0h] [ebp-10h]
  int ammoIndex; // [esp+4h] [ebp-Ch]
  const WeaponVariantDef *weaponVarDef; // [esp+8h] [ebp-8h]
  int weapIndex; // [esp+Ch] [ebp-4h]

  if ( !ps && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp", 1205, 0, "%s", "ps") )
    __debugbreak();
  weaponVarDef = BG_GetWeaponVariantDef(weaponIndex);
  ammoIndex = weaponVarDef->iAmmoIndex;
  if ( !*weaponVarDef->szAmmoName )
    return 1;
  for ( slot = 0; slot < 15; ++slot )
  {
    weapIndex = ps->heldWeapons[slot].weapon;
    if ( weapIndex
      && BG_GetWeaponVariantDef(weapIndex)->iAmmoIndex == ammoIndex
      && BG_GetMaxPickupableAmmo(ps, weapIndex) > 0 )
    {
      return 1;
    }
  }
  return 0;
}

bool __cdecl BG_PlayerHasRoomForEntAllAmmoTypes(const entityState_s *ent, const playerState_s *ps)
{
  const char *v2; // eax
  const WeaponVariantDef *weapVariantDef; // [esp+0h] [ebp-8h]
  int weapIdx; // [esp+4h] [ebp-4h]

  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp", 1339, 0, "%s", "ent") )
    __debugbreak();
  if ( !ps && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp", 1340, 0, "%s", "ps") )
    __debugbreak();
  if ( ent->un3.item < 1 || ent->un3.item >= 2048 )
  {
    v2 = va(&byte_C67FB0, ent->un3.item, ent->eType);
    Com_Error(ERR_DROP, v2);
  }
  weapIdx = ent->un3.item % 2048;
  if ( !weapIdx )
    return 0;
  if ( bg_itemlist[2048 * (ent->un3.item / 2048) + weapIdx] != 1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
          1348,
          0,
          "%s",
          "bg_itemlist[ ITEM_WEAPMODEL( ent->un3.item ) * MAX_WEAPONS + weapIdx].giType == IT_WEAPON") )
  {
    __debugbreak();
  }
  if ( !BG_GetMaxPickupableAmmo(ps, weapIdx) )
    return 0;
  weapVariantDef = BG_GetWeaponVariantDef(weapIdx);
  return !weapVariantDef->altWeaponIndex || BG_GetMaxPickupableAmmo(ps, weapVariantDef->altWeaponIndex);
}

void __cdecl BG_EvaluateTrajectory(const trajectory_t *tr, int atTime, float *result)
{
  double v3; // xmm0_8
  long double v4; // [esp-34h] [ebp-ACh]
  float v5; // [esp-10h] [ebp-88h]
  float v6; // [esp-8h] [ebp-80h]
  float v7; // [esp-4h] [ebp-7Ch]
  float v8; // [esp+10h] [ebp-68h]
  float v9; // [esp+38h] [ebp-40h]
  float v10; // [esp+38h] [ebp-40h]
  float v11; // [esp+48h] [ebp-30h]
  float v12; // [esp+48h] [ebp-30h]
  float v13; // [esp+48h] [ebp-30h]
  float v14; // [esp+48h] [ebp-30h]
  float v15; // [esp+48h] [ebp-30h]
  float v16; // [esp+48h] [ebp-30h]

  if ( tr->trType >= 0xFu
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
          1367,
          0,
          "%s\n\t(tr->trType) = %i",
          "(tr->trType < NUM_TRTYPES)",
          tr->trType) )
  {
    __debugbreak();
  }
  if ( !tr && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp", 1369, 0, "%s", "tr") )
    __debugbreak();
  if ( ((LODWORD(tr->trBase[0]) & 0x7F800000) == 0x7F800000
     || (LODWORD(tr->trBase[1]) & 0x7F800000) == 0x7F800000
     || (LODWORD(tr->trBase[2]) & 0x7F800000) == 0x7F800000)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
          1370,
          0,
          "%s",
          "!IS_NAN((tr->trBase)[0]) && !IS_NAN((tr->trBase)[1]) && !IS_NAN((tr->trBase)[2])") )
  {
    __debugbreak();
  }
  if ( ((LODWORD(tr->trDelta[0]) & 0x7F800000) == 0x7F800000
     || (LODWORD(tr->trDelta[1]) & 0x7F800000) == 0x7F800000
     || (LODWORD(tr->trDelta[2]) & 0x7F800000) == 0x7F800000)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
          1371,
          0,
          "%s",
          "!IS_NAN((tr->trDelta)[0]) && !IS_NAN((tr->trDelta)[1]) && !IS_NAN((tr->trDelta)[2])") )
  {
    __debugbreak();
  }
  switch ( tr->trType )
  {
    case 0u:
    case 1u:
    case 0xAu:
    case 0xEu:
      *result = tr->trBase[0];
      result[1] = tr->trBase[1];
      result[2] = tr->trBase[2];
      break;
    case 3u:
    case 0xBu:
    case 0xCu:
      v11 = (float)(atTime - tr->trTime) * 0.001;
      *result = (float)(v11 * tr->trDelta[0]) + tr->trBase[0];
      result[1] = (float)(v11 * tr->trDelta[1]) + tr->trBase[1];
      result[2] = (float)(v11 * tr->trDelta[2]) + tr->trBase[2];
      break;
    case 4u:
      if ( atTime > tr->trDuration + tr->trTime )
        atTime = tr->trDuration + tr->trTime;
      v12 = (float)(atTime - tr->trTime) * 0.001;
      if ( v12 < 0.0 )
        v12 = 0.0f;
      *result = (float)(v12 * tr->trDelta[0]) + tr->trBase[0];
      result[1] = (float)(v12 * tr->trDelta[1]) + tr->trBase[1];
      result[2] = (float)(v12 * tr->trDelta[2]) + tr->trBase[2];
      break;
    case 5u:
      if ( !tr->trDuration
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
              1387,
              0,
              "%s\n\t%s",
              "tr->trDuration != 0",
              "div by 0") )
      {
        __debugbreak();
      }
      if ( tr->trDuration )
      {
        v3 = (float)((float)((float)((float)(atTime - tr->trTime) / (float)tr->trDuration) * 3.1415927) * 2.0);
        __libm_sse2_sin(v4);
        *(float *)&v3 = v3;
        *result = (float)(*(float *)&v3 * tr->trDelta[0]) + tr->trBase[0];
        result[1] = (float)(*(float *)&v3 * tr->trDelta[1]) + tr->trBase[1];
        result[2] = (float)(*(float *)&v3 * tr->trDelta[2]) + tr->trBase[2];
      }
      else
      {
        *result = tr->trBase[0];
        result[1] = tr->trBase[1];
        result[2] = tr->trBase[2];
      }
      break;
    case 6u:
    case 0xDu:
      v13 = (float)(atTime - tr->trTime) * 0.001;
      *result = (float)(v13 * tr->trDelta[0]) + tr->trBase[0];
      result[1] = (float)(v13 * tr->trDelta[1]) + tr->trBase[1];
      result[2] = (float)(v13 * tr->trDelta[2]) + tr->trBase[2];
      result[2] = result[2] - (float)((float)((float)(0.5 * bg_gravity->current.value) * v13) * v13);
      break;
    case 7u:
      v14 = (float)(atTime - tr->trTime) * 0.001;
      *result = (float)(v14 * tr->trDelta[0]) + tr->trBase[0];
      result[1] = (float)(v14 * tr->trDelta[1]) + tr->trBase[1];
      result[2] = (float)(v14 * tr->trDelta[2]) + tr->trBase[2];
      result[2] = result[2] - (float)((float)((float)(0.5 * bg_lowGravity->current.value) * v14) * v14);
      break;
    case 8u:
      if ( atTime > tr->trDuration + tr->trTime )
        atTime = tr->trDuration + tr->trTime;
      v15 = (float)(atTime - tr->trTime) * 0.001;
      if ( !tr->trDuration
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
              1426,
              0,
              "%s\n\t%s",
              "tr->trDuration != 0",
              "div by 0") )
      {
        __debugbreak();
      }
      if ( tr->trDuration )
      {
        v9 = Abs(tr->trDelta) / ((double)tr->trDuration * 0.001);
        Vec3NormalizeTo(tr->trDelta, result);
        v8 = (float)((float)(v9 * 0.5) * v15) * v15;
        *result = (float)(v8 * *result) + tr->trBase[0];
        result[1] = (float)(v8 * result[1]) + tr->trBase[1];
        result[2] = (float)(v8 * result[2]) + tr->trBase[2];
      }
      else
      {
        *result = tr->trBase[0];
        result[1] = tr->trBase[1];
        result[2] = tr->trBase[2];
      }
      break;
    case 9u:
      if ( atTime > tr->trDuration + tr->trTime )
        atTime = tr->trDuration + tr->trTime;
      v16 = (float)(atTime - tr->trTime) * 0.001;
      if ( !tr->trDuration
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
              1445,
              0,
              "%s\n\t%s",
              "tr->trDuration != 0",
              "div by 0") )
      {
        __debugbreak();
      }
      if ( tr->trDuration )
      {
        v10 = Abs(tr->trDelta) / ((double)tr->trDuration * 0.001);
        Vec3NormalizeTo(tr->trDelta, result);
        v6 = (float)(v16 * tr->trDelta[1]) + tr->trBase[1];
        v7 = (float)(v16 * tr->trDelta[2]) + tr->trBase[2];
        v5 = (float)((float)(COERCE_FLOAT(LODWORD(v10) ^ _mask__NegFloat_) * 0.5) * v16) * v16;
        *result = (float)(v5 * *result) + (float)((float)(v16 * tr->trDelta[0]) + tr->trBase[0]);
        result[1] = (float)(v5 * result[1]) + v6;
        result[2] = (float)(v5 * result[2]) + v7;
      }
      else
      {
        *result = tr->trBase[0];
        result[1] = tr->trBase[1];
        result[2] = tr->trBase[2];
      }
      break;
    default:
      Com_Error(ERR_DROP, &byte_C68008, tr->trType);
      break;
  }
  if ( ((LODWORD(tr->trBase[0]) & 0x7F800000) == 0x7F800000
     || (LODWORD(tr->trBase[1]) & 0x7F800000) == 0x7F800000
     || (LODWORD(tr->trBase[2]) & 0x7F800000) == 0x7F800000)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
          1465,
          0,
          "%s",
          "!IS_NAN((tr->trBase)[0]) && !IS_NAN((tr->trBase)[1]) && !IS_NAN((tr->trBase)[2])") )
  {
    __debugbreak();
  }
  if ( ((LODWORD(tr->trDelta[0]) & 0x7F800000) == 0x7F800000
     || (LODWORD(tr->trDelta[1]) & 0x7F800000) == 0x7F800000
     || (LODWORD(tr->trDelta[2]) & 0x7F800000) == 0x7F800000)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
          1466,
          0,
          "%s",
          "!IS_NAN((tr->trDelta)[0]) && !IS_NAN((tr->trDelta)[1]) && !IS_NAN((tr->trDelta)[2])") )
  {
    __debugbreak();
  }
}

double __cdecl Vec3NormalizeTo(const float *v, float *out)
{
  float v3; // [esp+0h] [ebp-10h]
  float length; // [esp+Ch] [ebp-4h]

  length = fsqrt((float)((float)(*v * *v) + (float)(v[1] * v[1])) + (float)(v[2] * v[2]));
  if ( COERCE_FLOAT(LODWORD(length) ^ _mask__NegFloat_) < 0.0 )
    v3 = length;
  else
    v3 = 1.0f;
  *out = *v * (float)(1.0 / v3);
  out[1] = v[1] * (float)(1.0 / v3);
  out[2] = v[2] * (float)(1.0 / v3);
  return length;
}

void __cdecl BG_EvaluateTrajectoryDelta(const trajectory_t *tr, int atTime, float *result)
{
  double v3; // xmm0_8
  long double v4; // [esp+0h] [ebp-5Ch]
  float deltaTime; // [esp+58h] [ebp-4h]
  float deltaTimea; // [esp+58h] [ebp-4h]
  float deltaTimeb; // [esp+58h] [ebp-4h]
  float deltaTimec; // [esp+58h] [ebp-4h]

  if ( ((LODWORD(tr->trBase[0]) & 0x7F800000) == 0x7F800000
     || (LODWORD(tr->trBase[1]) & 0x7F800000) == 0x7F800000
     || (LODWORD(tr->trBase[2]) & 0x7F800000) == 0x7F800000)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
          1482,
          0,
          "%s",
          "!IS_NAN((tr->trBase)[0]) && !IS_NAN((tr->trBase)[1]) && !IS_NAN((tr->trBase)[2])") )
  {
    __debugbreak();
  }
  if ( ((LODWORD(tr->trDelta[0]) & 0x7F800000) == 0x7F800000
     || (LODWORD(tr->trDelta[1]) & 0x7F800000) == 0x7F800000
     || (LODWORD(tr->trDelta[2]) & 0x7F800000) == 0x7F800000)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
          1483,
          0,
          "%s",
          "!IS_NAN((tr->trDelta)[0]) && !IS_NAN((tr->trDelta)[1]) && !IS_NAN((tr->trDelta)[2])") )
  {
    __debugbreak();
  }
  switch ( tr->trType )
  {
    case 0u:
    case 1u:
      *result = 0.0f;
      result[1] = 0.0f;
      result[2] = 0.0f;
      goto LABEL_27;
    case 3u:
      *result = tr->trDelta[0];
      result[1] = tr->trDelta[1];
      result[2] = tr->trDelta[2];
      goto LABEL_27;
    case 4u:
      if ( atTime > tr->trDuration + tr->trTime )
      {
        *result = 0.0f;
        result[1] = 0.0f;
        result[2] = 0.0f;
        return;
      }
      *result = tr->trDelta[0];
      result[1] = tr->trDelta[1];
      result[2] = tr->trDelta[2];
      goto LABEL_27;
    case 5u:
      v3 = (float)((float)((float)((float)(atTime - tr->trTime) / (float)tr->trDuration) * 3.1415927) * 2.0);
      LODWORD(v4) = tr->trType;
      __libm_sse2_cos(v4);
      *(float *)&v3 = v3;
      *result = (float)(*(float *)&v3 * 0.5) * tr->trDelta[0];
      result[1] = (float)(*(float *)&v3 * 0.5) * tr->trDelta[1];
      result[2] = (float)(*(float *)&v3 * 0.5) * tr->trDelta[2];
      goto LABEL_27;
    case 6u:
      deltaTime = (float)(atTime - tr->trTime) * 0.001;
      *result = tr->trDelta[0];
      result[1] = tr->trDelta[1];
      result[2] = tr->trDelta[2];
      result[2] = result[2] - (float)(bg_gravity->current.value * deltaTime);
      goto LABEL_27;
    case 7u:
      deltaTimea = (float)(atTime - tr->trTime) * 0.001;
      *result = tr->trDelta[0];
      result[1] = tr->trDelta[1];
      result[2] = tr->trDelta[2];
      result[2] = result[2] - (float)(bg_lowGravity->current.value * deltaTimea);
      goto $LN5_1;
    case 8u:
$LN5_1:
      if ( atTime > tr->trDuration + tr->trTime )
      {
        *result = 0.0f;
        result[1] = 0.0f;
        result[2] = 0.0f;
        return;
      }
      deltaTimeb = (float)(atTime - tr->trTime) * 0.001;
      *result = (float)(deltaTimeb * deltaTimeb) * tr->trDelta[0];
      result[1] = (float)(deltaTimeb * deltaTimeb) * tr->trDelta[1];
      result[2] = (float)(deltaTimeb * deltaTimeb) * tr->trDelta[2];
      goto LABEL_27;
    case 9u:
      if ( atTime <= tr->trDuration + tr->trTime )
      {
        deltaTimec = (float)(atTime - tr->trTime) * 0.001;
        *result = deltaTimec * tr->trDelta[0];
        result[1] = deltaTimec * tr->trDelta[1];
        result[2] = deltaTimec * tr->trDelta[2];
LABEL_27:
        if ( ((LODWORD(tr->trBase[0]) & 0x7F800000) == 0x7F800000
           || (LODWORD(tr->trBase[1]) & 0x7F800000) == 0x7F800000
           || (LODWORD(tr->trBase[2]) & 0x7F800000) == 0x7F800000)
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
                1541,
                0,
                "%s",
                "!IS_NAN((tr->trBase)[0]) && !IS_NAN((tr->trBase)[1]) && !IS_NAN((tr->trBase)[2])") )
        {
          __debugbreak();
        }
        if ( ((LODWORD(tr->trDelta[0]) & 0x7F800000) == 0x7F800000
           || (LODWORD(tr->trDelta[1]) & 0x7F800000) == 0x7F800000
           || (LODWORD(tr->trDelta[2]) & 0x7F800000) == 0x7F800000)
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
                1542,
                0,
                "%s",
                "!IS_NAN((tr->trDelta)[0]) && !IS_NAN((tr->trDelta)[1]) && !IS_NAN((tr->trDelta)[2])") )
        {
          __debugbreak();
        }
      }
      else
      {
        *result = 0.0f;
        result[1] = 0.0f;
        result[2] = 0.0f;
      }
      return;
    default:
      Com_Error(ERR_DROP, &byte_C6813C, tr->trType);
      goto LABEL_27;
  }
}

bool __cdecl BG_ValidateOrigin(const float *pos, char xyBits, char zBits, const float *mapCenter)
{
  int maxXY; // [esp+0h] [ebp-8h]
  int maxZ; // [esp+4h] [ebp-4h]

  maxXY = 1 << (xyBits - 1);
  maxZ = 1 << (zBits - 1);
  return (int)pos[2] <= (int)(float)((float)maxZ + mapCenter[2])
      && (int)pos[2] >= (int)(float)(mapCenter[2] - (float)maxZ)
      && (int)*pos <= (int)(float)((float)maxXY + *mapCenter)
      && (int)*pos >= (int)(float)(*mapCenter - (float)maxXY)
      && (int)pos[1] <= (int)(float)((float)maxXY + mapCenter[1])
      && (int)pos[1] >= (int)(float)(mapCenter[1] - (float)maxXY);
}

bool __cdecl BG_ValidateOriginValue(float val, char bits, float mapCenterValue)
{
  int maxVal; // [esp+0h] [ebp-4h]

  maxVal = 1 << (bits - 1);
  return (int)val <= (int)(float)((float)maxVal + mapCenterValue)
      && (int)val >= (int)(float)(mapCenterValue - (float)maxVal);
}

void __cdecl BG_AddPredictableEventToPlayerstate(unsigned int newEvent, unsigned int eventParm, playerState_s *ps)
{
  if ( !ps && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp", 1738, 0, "%s", "ps") )
    __debugbreak();
  BG_AddEvent<int,unsigned int>(
    newEvent,
    eventParm,
    &ps->predictableEventSequence,
    ps->predictableEvents,
    ps->predictableEventParms,
    ps->commandTime,
    80);
}

void __cdecl BG_AddUnpredictableEventToPlayerstate(unsigned int newEvent, unsigned int eventParm, playerState_s *ps)
{
  if ( !ps && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp", 1750, 0, "%s", "ps") )
    __debugbreak();
  BG_AddEvent<int,unsigned int>(
    newEvent,
    eventParm,
    &ps->unpredictableEventSequence,
    ps->unpredictableEvents,
    ps->unpredictableEventParms,
    ps->commandTime,
    85);
}

void __cdecl BG_AddEntityStateEvent(unsigned int newEvent, unsigned int eventParm, entityState_s *es, int eventTime)
{
  if ( !es && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp", 1762, 0, "%s", "es") )
    __debugbreak();
  BG_AddEvent<unsigned char,unsigned short>(
    newEvent,
    eventParm,
    &es->eventSequence,
    es->events,
    es->eventParms,
    eventTime,
    69);
}

void __cdecl BG_PlayerToEntitySetFlags(playerState_s *ps, entityState_s *s)
{
  int v2; // edx
  int v3; // ecx

  s->lerp.eFlags = ps->eFlags;
  s->lerp.eFlags2 = ps->eFlags2;
  if ( ps->pm_type < 9 )
    v2 = s->lerp.eFlags & 0xFFFBFFFF;
  else
    v2 = s->lerp.eFlags | 0x40000;
  s->lerp.eFlags = v2;
  if ( (ps->pm_flags & 0x10) != 0 )
    v3 = s->lerp.eFlags | 0x80000;
  else
    v3 = s->lerp.eFlags & 0xFFF7FFFF;
  s->lerp.eFlags = v3;
}

void __cdecl BG_PlayerToEntitySetPitchAngles(playerState_s *ps, entityState_s *s)
{
  float fLerpFrac; // [esp+10h] [ebp-4h]

  if ( PM_GetEffectiveStance(ps) == 1 )
  {
    if ( ps->viewHeightLerpTime )
    {
      fLerpFrac = (float)(ps->commandTime - ps->viewHeightLerpTime)
                / (float)PM_GetViewHeightLerpTime(ps, ps->viewHeightLerpTarget, ps->viewHeightLerpDown);
      if ( fLerpFrac >= 0.0 )
      {
        if ( fLerpFrac > 1.0 )
          fLerpFrac = 1.0f;
      }
      else
      {
        fLerpFrac = 0.0f;
      }
      if ( !ps->viewHeightLerpDown )
        fLerpFrac = 1.0 - fLerpFrac;
    }
    else
    {
      fLerpFrac = 1.0f;
    }
    s->un2.animState.fAimUpDown = AngleNormalize180(ps->fTorsoPitch) * fLerpFrac;
    s->un2.animState.fAimLeftRight = AngleNormalize180(ps->fWaistPitch) * fLerpFrac;
  }
  else
  {
    s->un2.animState.fAimUpDown = 0.0f;
    s->un2.animState.fAimLeftRight = 0.0f;
  }
}

void __cdecl BG_PlayerStateToEntityState(playerState_s *ps, entityState_s *s, int snap, unsigned __int8 handler)
{
  BG_PlayerToEntitySetTrajectory(ps, s, snap);
  BG_PlayerToEntitySetFlags(ps, s);
  BG_PlayerToEntitySetMisc(ps, s);
  BG_PlayerToEntitySetPitchAngles(ps, s);
  BG_PlayerToEntityProcessEvents(ps, s, handler);
  if ( *(unsigned int *)(*((unsigned int *)NtCurrentTeb()->ThreadLocalStoragePointer + _tls_index) + 8) )
    *(_QWORD *)(*(unsigned int *)(*((unsigned int *)NtCurrentTeb()->ThreadLocalStoragePointer + _tls_index) + 8)
              + 1480 * ps->clientNum
              + 1416) = ps->moveType;
  s->loopSoundId = ps->loopSoundId;
  AssignToSmallerType<short>(&s->loopSoundFade, ps->loopSoundFade);
  if ( (ps->eFlags & 0x4000) != 0 )
  {
    s->otherEntityNum = ps->viewlocked_entNum;
    s->lerp.u.player.vehicleAnimBoneIndex = ps->vehicleAnimBoneIndex;
    s->lerp.u.player.vehicleSeat = ps->vehiclePos;
    s->lerp.u.player.vehicleType = ps->vehicleType;
  }
}

void __cdecl BG_PlayerToEntityProcessEvents(playerState_s *ps, entityState_s *s, unsigned __int8 handler)
{
  __int16 v3; // [esp+2h] [ebp-Ah]
  __int16 eventSequenceDelta; // [esp+4h] [ebp-8h]
  __int16 originalEventSequence; // [esp+8h] [ebp-4h]

  originalEventSequence = s->eventSequence;
  BG_PlayerToEntityProcessEvents_Internal(
    s,
    handler,
    &ps->predictableEventSequence,
    &ps->predictableEventSequenceOld,
    ps->predictableEvents,
    ps->predictableEventParms,
    ps->commandTime);
  BG_PlayerToEntityProcessEvents_Internal(
    s,
    handler,
    &ps->unpredictableEventSequence,
    &ps->unpredictableEventSequenceOld,
    ps->unpredictableEvents,
    ps->unpredictableEventParms,
    ps->commandTime);
  s->eventParm = 0;
  if ( originalEventSequence <= s->eventSequence + 64 )
    v3 = originalEventSequence;
  else
    v3 = originalEventSequence - 256;
  eventSequenceDelta = s->eventSequence - v3;
  if ( eventSequenceDelta > 4 )
  {
    Com_PrintWarning(
      17,
      "[%c|%i] Circular event buffer overflow ( Received: %i | Max: %i )\n",
      handler != 1 ? 67 : 83,
      ps->clientNum,
      eventSequenceDelta,
      4);
    Com_PrintWarning(
      17,
      "Events are:\n[0] %s\n[1] %s\n[2] %s\n[3] %s\n",
      eventnames[s->events[0]],
      eventnames[s->events[1]],
      eventnames[s->events[2]],
      eventnames[s->events[3]]);
  }
}

void __cdecl BG_PlayerToEntityProcessEvents_Internal(
        entityState_s *s,
        unsigned __int8 handler,
        __int16 *eventSequence,
        __int16 *oldEventSequence,
        int *events,
        unsigned int *eventParms,
        unsigned int eventTime)
{
  void (__cdecl *playerEvent)(int, int); // [esp+4h] [ebp-14h]
  int i; // [esp+8h] [ebp-10h]
  int event; // [esp+Ch] [ebp-Ch]
  __int16 oldSequenceNum; // [esp+10h] [ebp-8h]
  unsigned int eventParm; // [esp+14h] [ebp-4h]

  if ( *oldEventSequence <= *eventSequence + 64 )
    oldSequenceNum = *oldEventSequence;
  else
    oldSequenceNum = *oldEventSequence - 256;
  if ( *eventSequence < oldSequenceNum
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
          1794,
          0,
          "%s",
          "*eventSequence >= oldSequenceNum") )
  {
    __debugbreak();
  }
  for ( i = oldSequenceNum; i < *eventSequence; ++i )
  {
    event = events[i & 3];
    eventParm = eventParms[i & 3];
    playerEvent = (void (__cdecl *)(int, int))dword_DFF514[8 * handler];
    if ( playerEvent )
      playerEvent(s->number, event);
    if ( BG_PlayerToEntityShouldAddEvent(event) )
      BG_AddEntityStateEvent(event, eventParm, s, eventTime);
  }
  *oldEventSequence = *eventSequence;
}

char __cdecl BG_PlayerToEntityShouldAddEvent(int event)
{
  int j; // [esp+0h] [ebp-4h]
  int ja; // [esp+0h] [ebp-4h]

  for ( j = 0; serverOnlyEvents[j] > 0; ++j )
  {
    if ( serverOnlyEvents[j] == event )
      return 0;
  }
  for ( ja = 0; singleClientEvents[ja] > 0; ++ja )
  {
    if ( singleClientEvents[ja] == event )
      return 0;
  }
  return 1;
}

void __cdecl BG_PlayerToEntitySetMisc(playerState_s *ps, entityState_s *s)
{
  renderOptions_s o; // [esp+30h] [ebp-4h] BYREF

  s->un2.animState.state = ps->legsAnim;
  s->un2.anim.torsoAnim = ps->torsoAnim;
  s->lerp.u.turret.gunAngles[0] = ps->leanf;
  if ( (ps->eFlags & 0x300) != 0 )
    s->otherEntityNum = ps->viewlocked_entNum;
  if ( (ps->otherFlags & 6) != 0 )
    s->eType = 1;
  else
    s->eType = 5;
  s->clientNum = ps->clientNum;
  if ( BG_GetWeaponDef(ps->weapon)->inventoryType == WEAPINVENTORY_ALTMODE )
  {
    if ( !ps->lastWeaponAltModeSwitch
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
            1943,
            0,
            "%s",
            "ps->lastWeaponAltModeSwitch != WP_NONE") )
    {
      __debugbreak();
    }
    s->lerp.u.player.primaryWeapon = ps->lastWeaponAltModeSwitch;
  }
  else
  {
    s->lerp.u.player.primaryWeapon = ps->weapon;
  }
  s->lerp.u.actor.team = ps->moveType;
  s->weapon = ps->weapon;
  s->lastStandPrevWeapon = ps->lastStandPrevWeapon;
  s->weaponModel = BG_GetPlayerWeaponModel(ps, ps->weapon);
  o.i = BG_PlayerWeaponOptions(ps, ps->weapon).i;
  renderOptions_s::CopyWeaponOptions(&s->renderOptions, &o);
  s->renderOptions.i = ((unsigned __int8)HIBYTE(ps->renderOptions.i) >> 2 << 26) | s->renderOptions.i & 0x3FFFFFF;
  s->lerp.u.actor.proneInfo.fBodyPitch = ps->offHandIndex;
  s->lerp.u.player.offhandWeaponModel = BG_GetPlayerWeaponModel(ps, ps->offHandIndex);
  s->lerp.u.player.meleeWeapon = ps->meleeWeapon;
  s->lerp.u.player.meleeWeaponModel = BG_GetPlayerWeaponModel(ps, ps->meleeWeapon);
  s->groundEntityNum = ps->groundEntityNum;
  s->lerp.u.player.stowedWeapon = ps->stowedWeapon;
  s->lerp.u.player.stowedWeaponCamo = ps->stowedWeaponCamo;
}

void __thiscall renderOptions_s::CopyWeaponOptions(renderOptions_s *this, const renderOptions_s *o)
{
  this->i = o->i & 0x3F | this->i & 0xFFFFFFC0;
  this->i = (((o->i >> 6) & 0xF) << 6) | this->i & 0xFFFFFC3F;
  this->i = ((unsigned __int8)HIBYTE(LOWORD(o->i)) >> 2 << 10) | this->i & 0xFFFF03FF;
  this->i = ((HIWORD(o->i) & 7) << 16) | this->i & 0xFFF8FFFF;
  this->i = (((o->i & 0x80000) != 0) << 19) | this->i & 0xFFF7FFFF;
  this->i = (((o->i & 0x100000) != 0) << 20) | this->i & 0xFFEFFFFF;
  this->i = (((o->i >> 21) & 0x1F) << 21) | this->i & 0xFC1FFFFF;
}

unsigned __int8 __cdecl BG_GetPlayerWeaponModel(const playerState_s *ps, unsigned int weaponIndex)
{
  const PlayerHeldWeapon *heldWeapon; // [esp+8h] [ebp-4h]

  if ( !ps && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../bgame/bg_weapons.h", 464, 0, "%s", "ps") )
    __debugbreak();
  heldWeapon = BG_GetHeldWeaponConst(ps, weaponIndex);
  if ( heldWeapon )
    return heldWeapon->model;
  else
    return 0;
}

renderOptions_s __cdecl BG_PlayerWeaponOptions(const playerState_s *ps, unsigned int weaponIndex)
{
  const PlayerHeldWeapon *heldWeapon; // [esp+8h] [ebp-8h]

  if ( !ps && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../bgame/bg_weapons.h", 476, 0, "%s", "ps") )
    __debugbreak();
  heldWeapon = BG_GetHeldWeaponConst(ps, weaponIndex);
  if ( heldWeapon )
    return (renderOptions_s)heldWeapon->options.i;
  else
    return 0;
}

void __cdecl BG_PlayerToEntitySetTrajectory(playerState_s *ps, entityState_s *s, int snap)
{
  float v3; // [esp+0h] [ebp-30h]
  float v4; // [esp+4h] [ebp-2Ch]
  float v5; // [esp+8h] [ebp-28h]
  float v6; // [esp+Ch] [ebp-24h]
  float v7; // [esp+10h] [ebp-20h]
  float v8; // [esp+14h] [ebp-1Ch]

  s->lerp.pos.trType = 1;
  s->lerp.pos.trDuration = 0;
  s->lerp.pos.trTime = 0;
  s->lerp.pos.trDelta[0] = 0.0f;
  s->lerp.pos.trDelta[1] = 0.0f;
  s->lerp.pos.trDelta[2] = 0.0f;
  s->lerp.pos.trBase[0] = ps->origin[0];
  s->lerp.pos.trBase[1] = ps->origin[1];
  s->lerp.pos.trBase[2] = ps->origin[2];
  s->lerp.apos.trType = 1;
  s->lerp.apos.trDuration = 0;
  s->lerp.apos.trTime = 0;
  s->lerp.apos.trDelta[0] = 0.0f;
  s->lerp.apos.trDelta[1] = 0.0f;
  s->lerp.apos.trDelta[2] = 0.0f;
  if ( (ps->eFlags & 0x300) == 0 )
  {
    s->lerp.apos.trBase[0] = ps->viewangles[0];
    s->lerp.apos.trBase[1] = ps->viewangles[1];
    s->lerp.apos.trBase[2] = ps->viewangles[2];
  }
  if ( (ps->movementDir >= 128 || ps->movementDir < -128)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
          1991,
          0,
          "%s\n\t(ps->movementDir) = %i",
          "(ps->movementDir < 128 && ps->movementDir >= -128)",
          ps->movementDir) )
  {
    __debugbreak();
  }
  s->lerp.u.loopFx.period = SLOWORD(ps->movementDir);
  if ( snap )
  {
    if ( s->lerp.pos.trBase[0] >= 0.0 )
      v8 = 0.5f;
    else
      v8 = -0.5f;
    s->lerp.pos.trBase[0] = (float)(int)(float)(s->lerp.pos.trBase[0] + v8);
    if ( s->lerp.pos.trBase[1] >= 0.0 )
      v7 = 0.5f;
    else
      v7 = -0.5f;
    s->lerp.pos.trBase[1] = (float)(int)(float)(s->lerp.pos.trBase[1] + v7);
    if ( s->lerp.pos.trBase[2] >= 0.0 )
      v6 = 0.5f;
    else
      v6 = -0.5f;
    s->lerp.pos.trBase[2] = (float)(int)(float)(s->lerp.pos.trBase[2] + v6);
    if ( s->lerp.apos.trBase[0] >= 0.0 )
      v5 = 0.5f;
    else
      v5 = -0.5f;
    s->lerp.apos.trBase[0] = (float)(int)(float)(s->lerp.apos.trBase[0] + v5);
    if ( s->lerp.apos.trBase[1] >= 0.0 )
      v4 = 0.5f;
    else
      v4 = -0.5f;
    s->lerp.apos.trBase[1] = (float)(int)(float)(s->lerp.apos.trBase[1] + v4);
    if ( s->lerp.apos.trBase[2] >= 0.0 )
      v3 = 0.5f;
    else
      v3 = -0.5f;
    s->lerp.apos.trBase[2] = (float)(int)(float)(s->lerp.apos.trBase[2] + v3);
  }
}

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
        float prone_feet_dist)
{
  float angle; // [esp+4h] [ebp-160h]
  float anglea; // [esp+4h] [ebp-160h]
  float v18; // [esp+10h] [ebp-154h]
  float v19; // [esp+14h] [ebp-150h]
  _BYTE v20[12]; // [esp+18h] [ebp-14Ch] BYREF
  phys_vec3 p1; // [esp+24h] [ebp-140h] BYREF
  float v22; // [esp+44h] [ebp-120h]
  float v23; // [esp+48h] [ebp-11Ch]
  float v24; // [esp+4Ch] [ebp-118h]
  float fWaistPitch; // [esp+50h] [ebp-114h]
  float fTorsoPitch; // [esp+54h] [ebp-110h]
  float vTorsoPos[3]; // [esp+58h] [ebp-10Ch] BYREF
  float v28; // [esp+64h] [ebp-100h]
  float vWaistPos[3]; // [esp+68h] [ebp-FCh] BYREF
  float fWaistTraceDist; // [esp+74h] [ebp-F0h]
  float vFeetPos[3]; // [esp+78h] [ebp-ECh] BYREF
  float fPitchDiff; // [esp+84h] [ebp-E0h]
  float vDelta[3]; // [esp+88h] [ebp-DCh]
  float fFirstTraceDist; // [esp+94h] [ebp-D0h]
  float v35; // [esp+98h] [ebp-CCh] BYREF
  float fTraceRealHeight; // [esp+9Ch] [ebp-C8h]
  float fTraceHeight; // [esp+A0h] [ebp-C4h]
  float vForward[3]; // [esp+A4h] [ebp-C0h] BYREF
  float vEnd[3]; // [esp+B0h] [ebp-B4h] BYREF
  float vStart[3]; // [esp+BCh] [ebp-A8h] BYREF
  float vMaxs[3]; // [esp+C8h] [ebp-9Ch] BYREF
  float vMins[3]; // [esp+D4h] [ebp-90h]
  int iTraceMask; // [esp+E0h] [ebp-84h]
  void (__cdecl *traceFunc)(trace_t *, const float *, const float *, const float *, const float *, int, int, col_context_t *); // [esp+E4h] [ebp-80h]
  bool scale; // [esp+E8h] [ebp-7Ch]
  float fWaistDistance; // [esp+ECh] [ebp-78h] BYREF
  col_context_t context; // [esp+F8h] [ebp-6Ch] BYREF
  int v48; // [esp+120h] [ebp-44h] OVERLAPPED
  int bFirstTraceHit; // [esp+124h] [ebp-40h]
  trace_t trace; // [esp+128h] [ebp-3Ch]
  int retaddr; // [esp+164h] [ebp+0h]

  trace.staticModel = a1;
  trace.hitPartition = retaddr;
  context.collide_entity_func = *(int (__cdecl **)(int, col_context_t *))&FLOAT_0_0;
  v48 = 0;
  bFirstTraceHit = 0;
  trace.normal.vec.u[0] = 0;
  context.priorityMap = 0;
  HIBYTE(context.locational) = 1;
  col_context_t::col_context_t((col_context_t *)&fWaistDistance);
  scale = ps && (ps->pm_flags & 0x400000) != 0 && !Dtp_IsSliding(ps);
  traceFunc = (void (__cdecl *)(trace_t *, const float *, const float *, const float *, const float *, int, int, col_context_t *))scale;
  *(float *)&iTraceMask = PRONE_WAIST_DIST;
  if ( ps && ps->pm_type == 6 )
  {
    *(float *)&iTraceMask = *(float *)&iTraceMask * lastStandDistScale;
    prone_feet_dist = prone_feet_dist * lastStandDistScale;
  }
  vMins[2] = 1.0f;
  vMins[1] = *(float *)&pmoveHandlers[handler].trace;
  if ( !LODWORD(vMins[1])
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp", 2104, 0, "%s", "traceFunc") )
  {
    __debugbreak();
  }
  if ( proneCheckType )
    vMins[0] = 1.1938638e-38;
  else
    LODWORD(vMins[0]) = &loc_810011;
  if ( !isAlreadyProne )
  {
    LODWORD(vMaxs[0]) = LODWORD(fSize) ^ _mask__NegFloat_;
    LODWORD(vMaxs[1]) = LODWORD(fSize) ^ _mask__NegFloat_;
    vMaxs[2] = 0.0f;
    vStart[0] = fSize;
    vStart[1] = fSize;
    vStart[2] = fHeight;
    vEnd[0] = *vPos;
    vEnd[1] = vPos[1];
    vEnd[2] = vPos[2];
    vForward[0] = *vPos;
    vForward[1] = vPos[1];
    vForward[2] = vPos[2];
    vForward[2] = vForward[2] + 10.0;
    ((void (__cdecl *)(int (__cdecl **)(int, col_context_t *), float *, float *, float *, float *, int, unsigned int, float *))LODWORD(vMins[1]))(
      &context.collide_entity_func,
      vEnd,
      vMaxs,
      vStart,
      vForward,
      passEntityNum,
      LODWORD(vMins[0]),
      &fWaistDistance);
    if ( BYTE2(trace.hitType) )
      return 0;
  }
  if ( isOnGround && !groundIsWalkable )
    return 0;
  vMaxs[0] = -6.0f;
  vMaxs[1] = -6.0f;
  vMaxs[2] = -6.0f;
  vStart[0] = 6.0f;
  vStart[1] = 6.0f;
  vStart[2] = 6.0f;
  vForward[0] = 0.0f;
  vForward[1] = fYaw - 180.0;
  vForward[2] = 0.0f;
  AngleVectors(vForward, &v35, 0, 0);
  fFirstTraceDist = fHeight - 6.0;
  vDelta[2] = (float)(fHeight - 6.0) - 6.0;
  vEnd[0] = *vPos;
  vEnd[1] = vPos[1];
  vEnd[2] = vPos[2];
  vEnd[2] = vEnd[2] + (float)(fHeight - 6.0);
  vDelta[1] = prone_feet_dist - 6.0;
  vForward[0] = (float)((float)(prone_feet_dist - 6.0) * v35) + vEnd[0];
  vForward[1] = (float)((float)(prone_feet_dist - 6.0) * fTraceRealHeight) + vEnd[1];
  vForward[2] = (float)((float)(prone_feet_dist - 6.0) * fTraceHeight) + vEnd[2];
  ((void (__cdecl *)(int (__cdecl **)(int, col_context_t *), float *, float *, float *, float *, int, unsigned int, float *))LODWORD(vMins[1]))(
    &context.collide_entity_func,
    vEnd,
    vMaxs,
    vStart,
    vForward,
    passEntityNum,
    LODWORD(vMins[0]),
    &fWaistDistance);
  if ( trace.normal.vec.v[1] >= 1.0 )
  {
    vDelta[0] = prone_feet_dist;
  }
  else
  {
    if ( !isOnGround )
      return 0;
    context.priorityMap = (unsigned __int8 *)1;
    vDelta[0] = (float)((float)(prone_feet_dist - 6.0) * trace.normal.vec.v[1]) + 6.0;
    if ( (float)(fSize + 2.0) > vDelta[0] )
      return 0;
    if ( (float)((float)(fFirstTraceDist * 0.69999999) + *(float *)&iTraceMask) > vDelta[0] )
    {
      context.priorityMap = 0;
      vForward[2] = vForward[2] + 22.0;
      vFeetPos[1] = vForward[0] - vEnd[0];
      vFeetPos[2] = vForward[1] - vEnd[1];
      fPitchDiff = vForward[2] - vEnd[2];
      vFeetPos[0] = Vec3NormalizeTo(&vFeetPos[1], &v35);
      ((void (__cdecl *)(int (__cdecl **)(int, col_context_t *), float *, float *, float *, float *, int, unsigned int, float *))LODWORD(vMins[1]))(
        &context.collide_entity_func,
        vEnd,
        vMaxs,
        vStart,
        vForward,
        passEntityNum,
        LODWORD(vMins[0]),
        &fWaistDistance);
      if ( trace.normal.vec.v[1] >= 1.0 )
      {
        vDelta[0] = prone_feet_dist;
      }
      else
      {
        context.priorityMap = (unsigned __int8 *)1;
        vDelta[0] = (float)(trace.normal.vec.v[1] * vFeetPos[0]) + 6.0;
        if ( (float)((float)(fFirstTraceDist * 0.69999999) + *(float *)&iTraceMask) > vDelta[0] )
          return 0;
      }
    }
  }
  Vec3Lerp(vEnd, vForward, trace.normal.vec.v[1], &vWaistPos[1]);
  vEnd[0] = (float)(*(float *)&iTraceMask * v35) + *vPos;
  vEnd[1] = (float)(*(float *)&iTraceMask * fTraceRealHeight) + vPos[1];
  vEnd[2] = (float)(*(float *)&iTraceMask * fTraceHeight) + vPos[2];
  vEnd[2] = vEnd[2] + fFirstTraceDist;
  vForward[0] = vEnd[0];
  vForward[1] = vEnd[1];
  vForward[2] = vEnd[2] - (float)((float)((float)(fSize * 2.5) + fFirstTraceDist) - 6.0);
  ((void (__cdecl *)(int (__cdecl **)(int, col_context_t *), float *, float *, float *, float *, int, unsigned int, float *))LODWORD(vMins[1]))(
    &context.collide_entity_func,
    vEnd,
    vMaxs,
    vStart,
    vForward,
    passEntityNum,
    LODWORD(vMins[0]),
    &fWaistDistance);
  if ( trace.normal.vec.v[1] == 1.0 )
    goto fail;
  if ( !LOBYTE(trace.hitId) )
    return 0;
  vWaistPos[0] = (float)((float)((float)((float)(fSize * 2.5) + fFirstTraceDist) - 6.0) * trace.normal.vec.v[1]) + 6.0;
  Vec3Lerp(vEnd, vForward, trace.normal.vec.v[1], &vTorsoPos[1]);
  v28 = v28 - 6.0;
  if ( context.priorityMap )
  {
    if ( (float)(-0.75 * vWaistPos[0]) > (float)(vDelta[0] - vWaistPos[0]) )
      goto fail;
    vFeetPos[1] = (float)(6.0 * v35) + (float)(vWaistPos[1] - vTorsoPos[1]);
    vFeetPos[2] = (float)(6.0 * fTraceRealHeight) + (float)(vWaistPos[2] - vTorsoPos[2]);
    fPitchDiff = (float)(6.0 * fTraceHeight) + (float)(fWaistTraceDist - v28);
    fPitchDiff = fPitchDiff + 6.0;
    Vec3Normalize(&vFeetPos[1]);
    vTorsoPos[0] = (float)(prone_feet_dist - 6.0) - *(float *)&iTraceMask;
    vForward[0] = (float)(vTorsoPos[0] * vFeetPos[1]) + vEnd[0];
    vForward[1] = (float)(vTorsoPos[0] * vFeetPos[2]) + vEnd[1];
    vForward[2] = (float)(vTorsoPos[0] * fPitchDiff) + vEnd[2];
    vForward[0] = (float)((float)((float)((float)(prone_feet_dist - 6.0) * v35) + *vPos) + vForward[0]) * 0.5;
    vForward[1] = (float)((float)((float)((float)(prone_feet_dist - 6.0) * fTraceRealHeight) + vPos[1]) + vForward[1])
                * 0.5;
    ((void (__cdecl *)(int (__cdecl **)(int, col_context_t *), float *, float *, float *, float *, int, unsigned int, float *))LODWORD(vMins[1]))(
      &context.collide_entity_func,
      vEnd,
      vMaxs,
      vStart,
      vForward,
      passEntityNum,
      LODWORD(vMins[0]),
      &fWaistDistance);
    if ( trace.normal.vec.v[1] < 1.0 )
    {
      Vec3Lerp(vEnd, vForward, trace.normal.vec.v[1], vEnd);
      vEnd[2] = vEnd[2] + 18.0;
      vForward[2] = vForward[2] + 18.0;
      ((void (__cdecl *)(int (__cdecl **)(int, col_context_t *), float *, float *, float *, float *, int, unsigned int, float *))LODWORD(vMins[1]))(
        &context.collide_entity_func,
        vEnd,
        vMaxs,
        vStart,
        vForward,
        passEntityNum,
        LODWORD(vMins[0]),
        &fWaistDistance);
      if ( trace.normal.vec.v[1] < 1.0 )
        goto fail;
    }
    Vec3Lerp(vEnd, vForward, trace.normal.vec.v[1], &vWaistPos[1]);
  }
  vEnd[0] = vWaistPos[1];
  vEnd[1] = vWaistPos[2];
  vEnd[2] = fWaistTraceDist;
  vForward[0] = vWaistPos[1];
  vForward[1] = vWaistPos[2];
  vForward[2] = fWaistTraceDist - (float)((float)((float)(fWaistTraceDist - v28) * 2.0) + (float)(fSize * 1.0));
  ((void (__cdecl *)(int (__cdecl **)(int, col_context_t *), float *, float *, float *, float *, int, unsigned int, float *))LODWORD(vMins[1]))(
    &context.collide_entity_func,
    vEnd,
    vMaxs,
    vStart,
    vForward,
    passEntityNum,
    LODWORD(vMins[0]),
    &fWaistDistance);
  if ( trace.normal.vec.v[1] != 1.0 )
  {
    if ( !LOBYTE(trace.hitId) )
      return 0;
    Vec3Lerp(vEnd, vForward, trace.normal.vec.v[1], &vWaistPos[1]);
    fWaistTraceDist = fWaistTraceDist - 6.0;
    v24 = *vPos;
    fWaistPitch = vPos[1];
    fTorsoPitch = vPos[2];
    vFeetPos[1] = v24 - vTorsoPos[1];
    vFeetPos[2] = fWaistPitch - vTorsoPos[2];
    fPitchDiff = fTorsoPitch - v28;
    angle = vectopitch(&vFeetPos[1]);
    v23 = AngleNormalize180(angle);
    vFeetPos[1] = vTorsoPos[1] - vWaistPos[1];
    vFeetPos[2] = vTorsoPos[2] - vWaistPos[2];
    fPitchDiff = v28 - fWaistTraceDist;
    anglea = vectopitch(&vFeetPos[1]);
    v22 = AngleNormalize180(anglea);
    vFeetPos[0] = AngleNormalize180(v23 - v22);
    if ( vFeetPos[0] < -50.0 || vFeetPos[0] > 70.0 )
      HIBYTE(context.locational) = 0;
    vMaxs[0] = -0.0f;
    vMaxs[1] = -0.0f;
    vMaxs[2] = -0.0f;
    memset(vStart, 0, sizeof(vStart));
    vEnd[0] = v24;
    vEnd[1] = fWaistPitch;
    vEnd[2] = fTorsoPitch + 5.0;
    vForward[0] = vTorsoPos[1];
    vForward[1] = vTorsoPos[2];
    vForward[2] = v28 + 5.0;
    ((void (__cdecl *)(int (__cdecl **)(int, col_context_t *), float *, float *, float *, float *, int, unsigned int, float *))LODWORD(vMins[1]))(
      &context.collide_entity_func,
      vEnd,
      vMaxs,
      vStart,
      vForward,
      passEntityNum,
      LODWORD(vMins[0]),
      &fWaistDistance);
    if ( trace.normal.vec.v[1] < 1.0 )
      HIBYTE(context.locational) = 0;
    vEnd[0] = vForward[0];
    vEnd[1] = vForward[1];
    vEnd[2] = vForward[2];
    vForward[0] = vWaistPos[1];
    vForward[1] = vWaistPos[2];
    vForward[2] = fWaistTraceDist + 5.0;
    ((void (__cdecl *)(int (__cdecl **)(int, col_context_t *), float *, float *, float *, float *, int, unsigned int, float *))LODWORD(vMins[1]))(
      &context.collide_entity_func,
      vEnd,
      vMaxs,
      vStart,
      vForward,
      passEntityNum,
      LODWORD(vMins[0]),
      &fWaistDistance);
    if ( render_waist_foot_line )
    {
      Phys_Vec3ToNitrousVec(vEnd, (phys_vec3 *)&p1.y);
      Phys_Vec3ToNitrousVec(vForward, (phys_vec3 *)v20);
      render_line((phys_vec3 *)&p1.y, (const phys_vec3 *)v20, colorWhite, 0, 0);
    }
    if ( !enable_new_prone_check->current.integer && trace.normal.vec.v[1] < 1.0 )
    {
      if ( v22 >= 0.0 || v22 <= -20.0 )
      {
        v22 = 0.0f;
        HIBYTE(context.locational) = 0;
      }
      else
      {
        v22 = 0.0f;
      }
    }
    if ( ps && ps->pm_type == 6 )
    {
      LODWORD(v23) ^= _mask__NegFloat_;
      LODWORD(v22) ^= _mask__NegFloat_;
    }
    if ( pfTorsoPitch )
    {
      if ( traceFunc )
        v19 = 0.0f;
      else
        v19 = v23 * vMins[2];
      *pfTorsoPitch = v19;
    }
    if ( pfWaistPitch )
    {
      if ( traceFunc )
        v18 = 0.0f;
      else
        v18 = v22 * vMins[2];
      *pfWaistPitch = v18;
    }
    if ( HIBYTE(context.locational) )
      return 1;
  }
fail:
  if ( isOnGround )
    return 0;
  if ( pfTorsoPitch )
    *pfTorsoPitch = 0.0f;
  if ( pfWaistPitch )
    *pfWaistPitch = 0.0f;
  return 1;
}

void __cdecl Phys_Vec3ToNitrousVec(float *inVector, phys_vec3 *outVector)
{
  outVector->x = *inVector;
  outVector->y = inVector[1];
  outVector->z = inVector[2];
}

void __cdecl BG_GetPlayerViewOrigin(const playerState_s *ps, float *origin, int time)
{
  float delta; // [esp+18h] [ebp-1Ch]
  float fBobCycle; // [esp+1Ch] [ebp-18h]
  float vRight[3]; // [esp+20h] [ebp-14h] BYREF
  float xyspeed; // [esp+2Ch] [ebp-8h]
  float deltaB; // [esp+30h] [ebp-4h]

  if ( (ps->eFlags & 0x300) != 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
          2376,
          0,
          "%s",
          "!( ps->eFlags & EF_TURRET_ACTIVE )") )
  {
    __debugbreak();
  }
  *origin = ps->origin[0];
  origin[1] = ps->origin[1];
  origin[2] = ps->origin[2];
  origin[2] = origin[2] + ps->viewHeightCurrent;
  fBobCycle = BG_GetBobCycle(ps);
  if ( (ps->pm_flags & 8) != 0 )
  {
    if ( time - ps->jumpTime >= 500 )
      xyspeed = ps->velocity[2];
    else
      xyspeed = 0.0f;
  }
  else
  {
    xyspeed = Vec2Length(ps->velocity);
  }
  delta = BG_CalculateViewMovement_VerticalBobFactor(ps, fBobCycle, xyspeed);
  origin[2] = origin[2] + delta;
  deltaB = BG_CalculateViewMovement_HorizontalBobFactor(ps, fBobCycle, xyspeed);
  BG_GetPlayerViewDirection(ps, 0, vRight, 0);
  *origin = (float)(deltaB * vRight[0]) + *origin;
  origin[1] = (float)(deltaB * vRight[1]) + origin[1];
  origin[2] = (float)(deltaB * vRight[2]) + origin[2];
  if ( (ps->eFlags & 0x4000) == 0 )
    AddLeanToPosition(origin, ps->viewangles[1], ps->leanf, 16.0, 20.0);
  if ( (float)(ps->origin[2] + 8.0) > origin[2] )
    origin[2] = ps->origin[2] + 8.0;
}

void __cdecl BG_GetPlayerViewDirection(const playerState_s *ps, float *forward, float *right, float *up)
{
  AngleVectors(ps->viewangles, forward, right, up);
}

void __cdecl BG_LerpHudColors(const hudelem_s *elem, int time, hudelem_color_t *toColor)
{
  float lerp; // [esp+8h] [ebp-8h]
  int timeSinceFadeStarted; // [esp+Ch] [ebp-4h]

  timeSinceFadeStarted = time - elem->fadeStartTime;
  if ( elem->fadeTime <= 0 || timeSinceFadeStarted >= elem->fadeTime )
  {
    *toColor = elem->color;
  }
  else
  {
    if ( timeSinceFadeStarted < 0 )
      timeSinceFadeStarted = 0;
    lerp = (float)timeSinceFadeStarted / (float)elem->fadeTime;
    if ( (lerp < 0.0 || lerp > 1.0)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
            2476,
            0,
            "%s\n\t(lerp) = %g",
            "(lerp >= 0.0f && lerp <= 1.0f)",
            lerp) )
    {
      __debugbreak();
    }
    toColor->r = (int)(float)((float)elem->fromColor.r + (float)((float)(elem->color.r - elem->fromColor.r) * lerp));
    toColor->g = (int)(float)((float)elem->fromColor.g + (float)((float)(elem->color.g - elem->fromColor.g) * lerp));
    toColor->b = (int)(float)((float)elem->fromColor.b + (float)((float)(elem->color.b - elem->fromColor.b) * lerp));
    toColor->a = (int)(float)((float)elem->fromColor.a + (float)((float)(elem->color.a - elem->fromColor.a) * lerp));
  }
}

int __cdecl BG_LoadShellShockDvars(const char *name)
{
  char *filebuf; // [esp+0h] [ebp-F4h]
  const char *bg_shock_dvar_names[32]; // [esp+4h] [ebp-F0h] BYREF
  char fullpath[68]; // [esp+84h] [ebp-70h] BYREF
  int success; // [esp+C8h] [ebp-2Ch]
  bool isOptional[32]; // [esp+CCh] [ebp-28h] BYREF
  int i; // [esp+F0h] [ebp-4h]

  Com_sprintf(fullpath, 0x40u, "shock/%s.shock", name);
  filebuf = Com_LoadRawTextFile(fullpath);
  if ( filebuf
    || (Com_PrintError(17, "couldn't open '%s'.\n", fullpath),
        (filebuf = Com_LoadRawTextFile("shock/default.shock")) != 0) )
  {
    for ( i = 0; i < 32; ++i )
      bg_shock_dvar_names[i] = bgShockDvarNames[i];
    success = Com_LoadDvarsFromBufferOptional(bg_shock_dvar_names, isOptional, 0x20u, filebuf, fullpath);
    Com_UnloadRawTextFile(filebuf);
    return success;
  }
  else
  {
    Com_PrintError(17, "couldn't open 'shock/default.shock'. This is a default shock file that you should have.\n");
    return 0;
  }
}

void __cdecl BG_SetShellShockParmsFromDvars(shellshock_parms_t *parms)
{
  float value; // [esp+8h] [ebp-D0h]

  if ( !parms && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp", 2527, 0, "%s", "parms") )
    __debugbreak();
  parms->screenBlend.blurredEffectTime = (int)((float)(bg_shock_screenBlurBlendTime->current.value * 1000.0)
                                             + 9.313225746154785e-10);
  parms->screenBlend.blurredFadeTime = (int)((float)(bg_shock_screenBlurBlendFadeTime->current.value * 1000.0)
                                           + 9.313225746154785e-10);
  parms->screenBlend.flashShotFadeTime = (int)((float)(bg_shock_screenFlashShotFadeTime->current.value * 1000.0)
                                             + 9.313225746154785e-10);
  parms->screenBlend.flashWhiteFadeTime = (int)((float)(bg_shock_screenFlashWhiteFadeTime->current.value * 1000.0)
                                              + 9.313225746154785e-10);
  if ( parms->screenBlend.blurredFadeTime <= 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
          2533,
          0,
          "%s\n\t(parms->screenBlend.blurredFadeTime) = %i",
          "(parms->screenBlend.blurredFadeTime > 0)",
          parms->screenBlend.blurredFadeTime) )
  {
    __debugbreak();
  }
  if ( parms->screenBlend.blurredEffectTime <= 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
          2534,
          0,
          "%s\n\t(parms->screenBlend.blurredEffectTime) = %i",
          "(parms->screenBlend.blurredEffectTime > 0)",
          parms->screenBlend.blurredEffectTime) )
  {
    __debugbreak();
  }
  parms->screenBlend.type = bg_shock_screenType->current.integer;
  parms->view.fadeTime = 3000;
  if ( (float)(0.001 - bg_shock_viewKickPeriod->current.value) < 0.0 )
    value = bg_shock_viewKickPeriod->current.value;
  else
    value = 0.001f;
  parms->view.kickRate = 0.001 / value;
  parms->view.kickRadius = bg_shock_viewKickRadius->current.value;
  parms->sound.affect = bg_shock_sound->current.enabled;
  strncpy((unsigned __int8 *)parms->sound.loop, (unsigned __int8 *)bg_shock_soundLoop->current.integer, 0x40u);
  strncpy(
    (unsigned __int8 *)parms->sound.loopSilent,
    (unsigned __int8 *)bg_shock_soundLoopSilent->current.integer,
    0x40u);
  strncpy((unsigned __int8 *)parms->sound.end, (unsigned __int8 *)bg_shock_soundEnd->current.integer, 0x40u);
  strncpy((unsigned __int8 *)parms->sound.endAbort, (unsigned __int8 *)bg_shock_soundEndAbort->current.integer, 0x40u);
  parms->sound.fadeInTime = (int)((float)(bg_shock_soundFadeInTime->current.value * 1000.0) + 9.313225746154785e-10);
  parms->sound.fadeOutTime = (int)((float)(bg_shock_soundFadeOutTime->current.value * 1000.0) + 9.313225746154785e-10);
  parms->sound.loopFadeTime = (int)((float)(bg_shock_soundLoopFadeTime->current.value * 1000.0) + 9.313225746154785e-10);
  parms->sound.loopEndDelay = (int)((float)(bg_shock_soundLoopEndDelay->current.value * 1000.0) + 9.313225746154785e-10);
  strncpy((unsigned __int8 *)parms->sound.roomtype, (unsigned __int8 *)bg_shock_soundRoomType->current.integer, 0x40u);
  parms->sound.drylevel = bg_shock_soundDryLevel->current.value;
  parms->sound.wetlevel = bg_shock_soundWetLevel->current.value;
  parms->sound.modEndDelay = (int)((float)(bg_shock_soundModEndDelay->current.value * 1000.0) + 9.313225746154785e-10);
  if ( parms->sound.fadeInTime < 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
          2563,
          0,
          "%s\n\t(parms->sound.fadeInTime) = %i",
          "(parms->sound.fadeInTime >= 0)",
          parms->sound.fadeInTime) )
  {
    __debugbreak();
  }
  if ( parms->sound.fadeOutTime < 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
          2564,
          0,
          "%s\n\t(parms->sound.fadeOutTime) = %i",
          "(parms->sound.fadeOutTime >= 0)",
          parms->sound.fadeOutTime) )
  {
    __debugbreak();
  }
  if ( parms->sound.loopFadeTime < 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
          2565,
          0,
          "%s\n\t(parms->sound.loopFadeTime) = %i",
          "(parms->sound.loopFadeTime >= 0)",
          parms->sound.loopFadeTime) )
  {
    __debugbreak();
  }
  if ( (parms->sound.drylevel < 0.0 || parms->sound.drylevel > 1.0)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
          2566,
          0,
          "%s\n\t(parms->sound.drylevel) = %g",
          "(parms->sound.drylevel >= 0 && parms->sound.drylevel <= 1)",
          parms->sound.drylevel) )
  {
    __debugbreak();
  }
  if ( (parms->sound.wetlevel < 0.0 || parms->sound.wetlevel > 1.0)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
          2567,
          0,
          "%s\n\t(parms->sound.wetlevel) = %g",
          "(parms->sound.wetlevel >= 0 && parms->sound.wetlevel <= 1)",
          parms->sound.wetlevel) )
  {
    __debugbreak();
  }
  parms->lookControl.affect = bg_shock_lookControl->current.enabled;
  parms->lookControl.fadeTime = (int)((float)(bg_shock_lookControl_fadeTime->current.value * 1000.0)
                                    + 9.313225746154785e-10);
  if ( parms->lookControl.fadeTime <= 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
          2571,
          0,
          "%s\n\t(parms->lookControl.fadeTime) = %i",
          "(parms->lookControl.fadeTime > 0)",
          parms->lookControl.fadeTime) )
  {
    __debugbreak();
  }
  parms->lookControl.maxPitchSpeed = bg_shock_lookControl_maxpitchspeed->current.value;
  parms->lookControl.maxYawSpeed = bg_shock_lookControl_maxyawspeed->current.value;
  parms->lookControl.mouseSensitivity = bg_shock_lookControl_mousesensitivityscale->current.value;
  parms->movement.scale = bg_shock_movement->current.value;
  parms->animation.affect = bg_shock_animation->current.enabled;
  I_strncpyz(parms->visionSet.name, bg_shock_visionset_name->current.string, 32);
  parms->visionSet.inTime = (int)(float)(bg_shock_visionset_inTime->current.value * 1000.0);
  parms->visionSet.outTime = (int)(float)(bg_shock_visionset_outTime->current.value * 1000.0);
}

int __cdecl BG_SaveShellShockDvars(const char *name)
{
  const char *bg_shock_dvar_names[32]; // [esp+10h] [ebp-90h] BYREF
  const char *fullpath; // [esp+94h] [ebp-Ch]
  int fh; // [esp+98h] [ebp-8h] BYREF
  int i; // [esp+9Ch] [ebp-4h]

  for ( i = 0; i < 32; ++i )
    bg_shock_dvar_names[i] = bgShockDvarNames[i];
  if ( !Com_SaveDvarsToBuffer(bg_shock_dvar_names, 0x20u, filebuf, 0x10000u) )
    return 0;
  fullpath = va("shock/%s.shock", name);
  if ( (FS_FOpenFileByMode((char *)fullpath, &fh, FS_WRITE) & 0x80000000) != 0 )
    return 0;
  FS_Write(filebuf, strlen(filebuf), fh);
  FS_FCloseFile(fh);
  return 1;
}

shellshock_parms_t *__cdecl BG_GetShellshockParms(unsigned int index)
{
  if ( index >= 0x10
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
          2614,
          0,
          "%s\n\t(index) = %i",
          "(index >= 0 && index < 16)",
          index) )
  {
    __debugbreak();
  }
  return &bg_shellshockParms[index];
}

bool __cdecl BG_IsShellshockAnim(const playerState_s *ps)
{
  return (ps->pm_flags & 0x10000) != 0 && BG_GetShellshockParms(ps->shellshockIndex)->animation.affect;
}

void __cdecl BG_CreateXAnim(XAnim_s *anims, unsigned int animIndex, char *name)
{
  if ( !useFastFile->current.enabled )
    XAnimPrecache(name, (void *(__cdecl *)(int))Hunk_AllocXAnimPrecache);
  XAnimCreate(anims, animIndex, name);
}

void __cdecl BG_CheckThread()
{
  if ( !Sys_IsServerThread()
    && !Sys_IsMainThread()
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp",
          2641,
          0,
          "%s",
          "Sys_IsServerThread() || Sys_IsMainThread()") )
  {
    __debugbreak();
  }
}

int __cdecl BG_GetMaxSprintTime(const playerState_s *ps)
{
  float sprintDuration; // [esp+10h] [ebp-14h]
  float maxSprintTime; // [esp+20h] [ebp-4h]

  if ( (ps->eFlags & 0x4000) != 0 )
  {
    maxSprintTime = vehicle_perk_boost_duration_seconds->current.value * 1000.0;
  }
  else
  {
    sprintDuration = (float)ps->sprintState.sprintDuration;
    maxSprintTime = sprintDuration * BG_GetWeaponDef(ps->weapon)->sprintDurationScale;
    if ( player_sprintTime->current.value == 0.0 )
      maxSprintTime = 0.0f;
  }
  if ( (ps->perks[0] & 0x40000000) != 0 )
    maxSprintTime = (float)(perk_sprintMultiplier->current.value * 1.0) * maxSprintTime;
  if ( (int)maxSprintTime > 0x3FFF )
    return 0x3FFF;
  else
    return (int)maxSprintTime;
}

bool __cdecl BG_ClipMoveToDobj(const entityState_s *es)
{
  if ( !es && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_misc.cpp", 2688, 0, "%s", "es") )
    __debugbreak();
  if ( es->eType == 14 || es->eType == 16 )
    return 1;
  return (char *)es->solid != &cls.rankedServers[711].game[34] && es->eType != 17 && es->eType != 1;
}

int __cdecl BG_GetFriendlyFireStatus()
{
  return bg_friendlyFire->current.integer;
}

const char *__cdecl BG_DisplayName(const clientInfo_t *ci, int type)
{
  return Com_DisplayName(ci->name, ci->clanAbbrev, type);
}

void __cdecl BG_ClipCameraToHeliPatch(float *origin, float *oldpos, float *velocity, float (*breachPos)[3])
{
  float heliPatchZ; // [esp+8h] [ebp-14h] BYREF
  bool oldposValid; // [esp+Fh] [ebp-Dh]
  float newpos[3]; // [esp+10h] [ebp-Ch] BYREF

  if ( bg_freeCamClipToHeliPatch->current.enabled && num_heli_height_lock_patches > 0 )
  {
    oldposValid = CM_GetHeliHeight(oldpos, 3000.0, &heliPatchZ) != 0;
    if ( !CM_GetHeliHeight(origin, 3000.0, &heliPatchZ) )
    {
      newpos[0] = *origin;
      newpos[1] = origin[1];
      newpos[2] = origin[2];
      newpos[0] = *oldpos;
      if ( CM_GetHeliHeight(newpos, 3000.0, &heliPatchZ) )
      {
        *origin = *oldpos;
      }
      else
      {
        newpos[0] = *origin;
        newpos[1] = origin[1];
        newpos[2] = origin[2];
        newpos[1] = oldpos[1];
        if ( CM_GetHeliHeight(newpos, 3000.0, &heliPatchZ) )
        {
          origin[1] = oldpos[1];
        }
        else if ( oldposValid )
        {
          *origin = *oldpos;
          origin[1] = oldpos[1];
          origin[2] = oldpos[2];
          *velocity = 0.0f;
          velocity[1] = 0.0f;
          velocity[2] = 0.0f;
        }
        else if ( breachPos )
        {
          *origin = (*breachPos)[0];
          origin[1] = (*breachPos)[1];
          origin[2] = (*breachPos)[2];
          *velocity = 0.0f;
          velocity[1] = 0.0f;
          velocity[2] = 0.0f;
          if ( CM_GetHeliHeight(origin, 3000.0, &heliPatchZ) )
            origin[2] = heliPatchZ - 1.0;
        }
      }
    }
  }
}

const phys_vec3 *__cdecl Phys_Vec3ToNitrousVec(const phys_vec3 *result, float *inVector)
{
  float v3; // [esp-8h] [ebp-18h]
  float v4; // [esp-4h] [ebp-14h]
  float outVector; // [esp+0h] [ebp-10h]

  v3 = inVector[1];
  v4 = inVector[2];
  result->x = *inVector;
  result->y = v3;
  result->z = v4;
  result->w = outVector;
  return result;
}

