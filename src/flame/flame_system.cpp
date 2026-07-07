#include "flame_system.h"
#include <universal/assertive.h>
#include "flame_class_chunk.h"
#include "flame_class_fire.h"
#include "flame_class_smoke.h"
#include "flame_class_stream.h"
#include "flame_class_drips.h"
#include <universal/dvar.h>
#include <universal/q_shared.h>
#include <bgame/bg_weapons.h>
#include <bgame/bg_weapons_def.h>
#include <qcommon/common.h>
#include <cgame_mp/cg_local_mp.h>
#include "flame_sound.h"
#include <gfx_d3d/r_drawsurf.h>
#include <gfx_d3d/r_sprite.h>
#include <vehicle/nitrous_vehicle.h>
#include <clientscript/scr_const.h>
#include <cgame_mp/cg_vehicles_mp.h>
#include <cgame/cg_weapons.h>
#include <demo/demo_playback.h>
#include <cgame/cg_event.h>
#include <EffectsCore/fx_update.h>
#include <universal/com_math_anglevectors.h>
#include <cgame/cg_drawtools.h>
#include <cgame/cg_world.h>
#include <game_mp/g_utils_mp.h>

flameVarDef_t flameVars[128] =
{
  {
    "flameVar_streamChunkGravityStart",
    "stream/Chunk/GravityStart",
    200.0,
    -10000.0,
    10000.0,
    "Stream Gravity at Start"
  },
  {
    "flameVar_streamChunkGravityEnd",
    "stream/Chunk/GravityEnd",
    -240.0,
    -10000.0,
    10000.0,
    "Stream Gravity at End"
  },
  {
    "flameVar_streamChunkMaxSize",
    "stream/Chunk/MaxSize",
    100.0,
    1.0,
    500.0,
    "Maximum size any stream chunk can be"
  },
  {
    "flameVar_streamChunkStartSize",
    "stream/Chunk/StartSize",
    3.0,
    1.0,
    10.0,
    "Starting size of each stream chunk"
  },
  {
    "flameVar_streamChunkEndSize",
    "stream/Chunk/EndSize",
    60.0,
    1.0,
    500.0,
    "Goal ending size of each stream chunk"
  },
  {
    "flameVar_streamChunkStartSizeRand",
    "stream/Chunk/StartSizeRand",
    0.30000001,
    0.0,
    1.0,
    "Maximum percentage of start size to reduce each chunk by randomly"
  },
  {
    "flameVar_streamChunkEndSizeRand",
    "stream/Chunk/EndSizeRand",
    0.69999999,
    0.0,
    1.0,
    "Maximum percentage of end size to reduce each chunk by randomly"
  },
  {
    "flameVar_streamChunkDistScalar",
    "stream/Chunk/DistScalar",
    1.0,
    0.1,
    10.0,
    "Controls the chunk spacing.. lower values cause more overlapping, with decreased performance"
  },
  {
    "flameVar_streamChunkDistSwayScale",
    "stream/Chunk/DistSwyScale",
    0.2,
    0.02,
    1.0,
    "Sets the maximum distance scaling based on angular motion"
  },
  {
    "flameVar_streamChunkDistSwayVelMax",
    "stream/Chunk/DistSwyVelMax",
    90.0,
    1.0,
    1000.0,
    "Sets the maximum sway rate, at which the full dist scaling is applied"
  },
  {
    "flameVar_streamChunkSpeed",
    "stream/Chunk/Speed",
    800.0,
    10.0,
    2000.0,
    "Speed of chunks when they leave the nozzle"
  },
  {
    "flameVar_streamChunkDecel",
    "stream/Chunk/Decel",
    0.40000001,
    0.0,
    20.0,
    "Fraction of current speed to reduce per second"
  },
  {
    "flameVar_streamChunkVelocityAddScale",
    "stream/Chunk/VelAddScale",
    0.75,
    0.0,
    1.0,
    "Fraction of movement speed to add to the firing velocity"
  },
  {
    "flameVar_streamChunkDuration",
    "stream/Chunk/Duration",
    2.4000001,
    0.02,
    10.0,
    "Lifetime of each chunk in seconds"
  },
  {
    "flameVar_streamChunkDurationScaleMaxVel",
    "stream/Chunk/DurMaxVel",
    150.0,
    0.0,
    500.0,
    "Velocity at which to apply full duration scaling"
  },
  {
    "flameVar_streamChunkDurationVelScalar",
    "stream/Chunk/DurVelScalar",
    0.40000001,
    0.0,
    1.0,
    "Maximum movement duration scalar"
  },
  {
    "flameVar_streamChunkSizeSpeedScale",
    "stream/Chunk/SizeSpeedScale",
    0.0,
    -5.0,
    5.0,
    "Controls the rate at which size increases based on current speed"
  },
  {
    "flameVar_streamChunkSizeAgeScale",
    "stream/Chunk/SizeAgeScale",
    -2.0,
    -5.0,
    5.0,
    "Controls the rate at which size increases based on current age"
  },
  {
    "flameVar_streamChunkSpawnFireIntervalStart",
    "stream/Chunk/SpwnFireStart",
    0.1,
    0.0099999998,
    5.0,
    "Interval between spawning flames from chunks at start of chunk life"
  },
  {
    "flameVar_streamChunkSpawnFireIntervalEnd",
    "stream/Chunk/SpwnFireEnd",
    0.80000001,
    0.0099999998,
    5.0,
    "Interval between spawning flames from chunks at end of chunk life"
  },
  {
    "flameVar_streamChunkSpawnFireMinLifeFrac",
    "stream/Chunk/SpwnFireMinLife",
    0.0,
    0.0,
    1.0,
    "Don't spawn fire before this life frac"
  },
  {
    "flameVar_streamChunkSpawnFireMaxLifeFrac",
    "stream/Chunk/SpwnFireMaxLife",
    0.89999998,
    0.1,
    1.0,
    "Don't spawn fire after this life frac"
  },
  {
    "flameVar_streamChunkFireMinLifeFrac",
    "stream/Chunk/FireMinLife",
    0.80000001,
    0.0,
    1.0,
    "Make sure all flames spawned by chunks are at least this old when spawned"
  },
  {
    "flameVar_streamChunkFireMinLifeFracStart",
    "stream/Chunk/FireMinLifeStart",
    0.0,
    0.0,
    1.0,
    "Starting point for adjusting min flame life"
  },
  {
    "flameVar_streamChunkFireMinLifeFracEnd",
    "stream/Chunk/FireMinLifeEnd",
    0.2,
    0.0,
    1.0,
    "End point for adjusting min flame life"
  },
  {
    "flameVar_streamChunkDripsMinLifeFrac",
    "stream/Chunk/DripsMinLife",
    0.0,
    0.0,
    1.0,
    "Make sure all flames spawned by chunks are at least this old when spawned"
  },
  {
    "flameVar_streamChunkDripsMinLifeFracStart",
    "stream/Chunk/DripsMinLifeStart",
    0.0,
    0.0,
    1.0,
    "Starting point for adjusting min drips life"
  },
  {
    "flameVar_streamChunkDripsMinLifeFracEnd",
    "stream/Chunk/DripsMinLifeEnd",
    1.0,
    0.0,
    1.0,
    "End point for adjusting min drips life"
  },
  {
    "flameVar_streamChunkRotationRange",
    "stream/Chunk/RotationRange",
    180.0,
    0.0,
    180.0,
    "Maximum rotation (roll) of chunks"
  },
  {
    "flameVar_streamSizeRandSinWave",
    "stream/Chunk/SizeRandSinWave",
    4.1322398,
    0.1,
    50.0,
    "Stream thickness fluctuation frequency (SINE)"
  },
  {
    "flameVar_streamSizeRandCosWave",
    "stream/Chunk/SizeRandCosWave",
    7.23244,
    0.1,
    50.0,
    "Stream thickness fluctuation frequency (COSINE)"
  },
  {
    "flameVar_streamDripsChunkInterval",
    "stream/Chunk/DripsInterval",
    0.025,
    0.0099999998,
    1.0,
    "Interval between spawning a chunk that will (in it's lifetime) randomly spawn a Drips particle"
  },
  {
    "flameVar_streamDripsChunkMinFrac",
    "stream/Chunk/DripsMinFrac",
    0.0,
    0.0,
    1.0,
    "Minimum life fraction to spawn a Drips particle at"
  },
  {
    "flameVar_streamDripsChunkRandFrac",
    "stream/Chunk/DripsRandFrac",
    0.80000001,
    0.0,
    1.0,
    "Random fraction of life frac to add to the minimum fraction when deciding on Drips spawn time"
  },
  {
    "flameVar_streamSmokeChunkInterval",
    "stream/Chunk/SmokeInterval",
    0.025,
    0.0099999998,
    1.0,
    "Interval between spawning a chunk that will (in it's lifetime) randomly spawn a smoke particle"
  },
  {
    "flameVar_streamSmokeChunkMinFrac",
    "stream/Chunk/SmokeMinFrac",
    0.0,
    0.0,
    1.0,
    "Minimum life fraction to spawn a smoke particle at"
  },
  {
    "flameVar_streamSmokeChunkRandFrac",
    "stream/Chunk/SmokeRandFrac",
    0.80000001,
    0.0,
    1.0,
    "Random fraction of life frac to add to the minimum fraction when deciding on smoke spawn time"
  },
  {
    "flameVar_streamChunkCullDistSizeFrac",
    "stream/Chunk/CullDistSizeFrac",
    0.2,
    0.0,
    3.0,
    "Cull chunks within this percentage of our size"
  },
  {
    "flameVar_streamChunkCullMinLife",
    "stream/Chunk/CullMinLife",
    0.2,
    0.0,
    1.0,
    "Don't cull chunks younger than this age"
  },
  {
    "flameVar_streamChunkCullMaxLife",
    "stream/Chunk/CullMaxLife",
    0.80000001,
    0.0,
    1.0,
    "Don't cull chunks older than this age"
  },
  {
    "flameVar_streamFuelSizeStart",
    "stream/Fuel/SizeStart",
    1.0,
    0.1,
    50.0,
    "Stream fuel size at start"
  },
  {
    "flameVar_streamFuelSizeEnd",
    "stream/Fuel/SizeEnd",
    3.0,
    0.1,
    50.0,
    "Stream fuel size at end"
  },
  {
    "flameVar_streamFuelLength",
    "stream/Fuel/Length",
    96.0,
    0.0099999998,
    512.0,
    "Stream fuel length"
  },
  {
    "flameVar_streamFuelNumSegments",
    "stream/Fuel/NumSegments",
    16.0,
    1.0,
    128.0,
    "Number of stream fuel segments"
  },
  {
    "flameVar_streamFuelAnimLoopTime",
    "stream/Fuel/AnimLoopTime",
    1.0,
    0.1,
    5.0,
    "Animation cycle time for animated fuel sprites"
  },
  {
    "flameVar_streamFlameSizeStart",
    "stream/Flame/SizeStart",
    1.0,
    0.1,
    50.0,
    "Stream flame size at start"
  },
  {
    "flameVar_streamFlameSizeEnd",
    "stream/Flame/SizeEnd",
    3.0,
    0.1,
    50.0,
    "Stream flame size at end"
  },
  {
    "flameVar_streamFlameLength",
    "stream/Flame/Length",
    96.0,
    0.0099999998,
    512.0,
    "Stream flame length"
  },
  {
    "flameVar_streamFlameNumSegments",
    "stream/Flame/NumSegments",
    16.0,
    1.0,
    128.0,
    "Number of stream flame segments"
  },
  {
    "flameVar_streamFlameAnimLoopTime",
    "stream/Flame/AnimLoopTime",
    1.0,
    0.1,
    5.0,
    "Animation cycle time for animated flame sprites"
  },
  {
    "flameVar_streamPrimaryLightRadius",
    "stream/Light/PrimaryRadius",
    375.0,
    0.0,
    1000.0,
    "Primary flamethrower light source radius"
  },
  {
    "flameVar_streamPrimaryLightRadiusFlutter",
    "stream/Light/PrimaryRadiusFlutter",
    120.0,
    0.0,
    1000.0,
    "Primary flamethrower light source radius modulation"
  },
  {
    "flameVar_streamPrimaryLightR",
    "stream/Light/PrimaryR",
    0.69999999,
    0.0,
    1.0,
    "Primary flamethrower light source color R"
  },
  {
    "flameVar_streamPrimaryLightG",
    "stream/Light/PrimaryG",
    0.40000001,
    0.0,
    1.0,
    "Primary flamethrower light source color G"
  },
  {
    "flameVar_streamPrimaryLightB",
    "stream/Light/PrimaryB",
    0.0,
    0.0,
    1000.0,
    "Primary flamethrower light source color B"
  },
  {
    "flameVar_streamPrimaryLightFlutterR",
    "stream/Light/PrimaryFlutterR",
    0.1,
    0.0,
    1.0,
    "Primary flamethrower light source color modulation R"
  },
  {
    "flameVar_streamPrimaryLightFlutterG",
    "stream/Light/PrimaryFlutterG",
    0.1,
    0.0,
    1.0,
    "Primary flamethrower light source color modulation G"
  },
  {
    "flameVar_streamPrimaryLightFlutterB",
    "stream/Light/PrimaryFlutterB",
    0.1,
    0.0,
    1.0,
    "Primary flamethrower light source color modulation B"
  },
  {
    "flameVar_fireLife",
    "stream/fire/Life",
    0.80000001,
    0.1,
    3.0,
    "Life duration of flames"
  },
  {
    "flameVar_fireLifeRand",
    "stream/fire/LifeRand",
    0.60000002,
    0.0,
    3.0,
    "Random additive applied to life duration"
  },
  {
    "flameVar_fireSpeedScale",
    "stream/fire/SpeedScale",
    0.75,
    0.0,
    1.0,
    "Fraction of chunk speed that is applied to starting velocity of flames"
  },
  {
    "flameVar_fireSpeedScaleRand",
    "stream/fire/SpeedScaleRand",
    0.5,
    0.0,
    1.0,
    "Randomize speed scale by this fraction"
  },
  {
    "flameVar_fireVelocityAddZ",
    "stream/fire/VelocityAddZ",
    60.0,
    -200.0,
    200.0,
    "Upwards velocity applied to flame velocity"
  },
  {
    "flameVar_fireVelocityAddZRand",
    "stream/fire/VelocityAddZRand",
    -20.0,
    -200.0,
    200.0,
    "Random additive applied to upwards velocity"
  },
  {
    "flameVar_fireVelocityAddSideways",
    "stream/fire/VelocityAddSideways",
    0.30000001,
    0.0,
    1.0,
    "Random sideways velocity as fraction of forward velocity"
  },
  {
    "flameVar_fireGravity",
    "stream/fire/Gravity",
    -200.0,
    -500.0,
    0.0,
    "Fire gravity at start of life"
  },
  {
    "flameVar_fireGravityEnd",
    "stream/fire/GravityEnd",
    -300.0,
    -500.0,
    0.0,
    "Fire gravity at end of life"
  },
  {
    "flameVar_fireMaxRotVel",
    "stream/fire/MaxRotVel",
    60.0,
    0.0,
    200.0,
    "Maximum rotational velocity of flame"
  },
  {
    "flameVar_fireFriction",
    "stream/fire/Friction",
    4.0,
    0.0,
    10.0,
    "Fraction of starting velocity to deccelerate per second"
  },
  {
    "flameVar_fireEndSizeAdd",
    "stream/fire/EndSizeAdd",
    15.0,
    0.0,
    100.0,
    "Add this to the size of the chunk, to give the end size of the flame"
  },
  {
    "flameVar_fireStartSizeScale",
    "stream/fire/StartSizeScale",
    1.0,
    0.1,
    20.0,
    "Multiply this by the size of the chunk, to give the start size of the flame"
  },
  {
    "flameVar_fireEndSizeScale",
    "stream/fire/EndSizeScale",
    1.0,
    0.0,
    20.0,
    "Multiply this by the size of the chunk, to give the end size of the flame"
  },
  {
    "flameVar_dripsLife",
    "stream/drips/Life",
    5.0,
    0.1,
    10.0,
    "Life duration of flames"
  },
  {
    "flameVar_dripsLifeRand",
    "stream/drips/LifeRand",
    0.60000002,
    0.0,
    3.0,
    "Random additive applied to life duration"
  },
  {
    "flameVar_dripsSpeedScale",
    "stream/drips/SpeedScale",
    0.75,
    0.0,
    1.0,
    "Fraction of chunk speed that is applied to starting velocity of flames"
  },
  {
    "flameVar_dripsSpeedScaleRand",
    "stream/drips/SpeedScaleRand",
    0.5,
    0.0,
    1.0,
    "Randomize speed scale by this fraction"
  },
  {
    "flameVar_dripsVelocityAddZ",
    "stream/drips/VelocityAddZ",
    60.0,
    -200.0,
    200.0,
    "Upwards velocity applied to flame velocity"
  },
  {
    "flameVar_dripsVelocityAddZRand",
    "stream/drips/VelocityAddZRand",
    -20.0,
    -200.0,
    200.0,
    "Random additive applied to upwards velocity"
  },
  {
    "flameVar_dripsVelocityAddSideways",
    "stream/drips/VelocityAddSideways",
    0.30000001,
    0.0,
    1.0,
    "Random sideways velocity as fraction of forward velocity"
  },
  {
    "flameVar_dripsGravity",
    "stream/drips/Gravity",
    200.0,
    -1000.0,
    1000.0,
    "Fire gravity at start of life"
  },
  {
    "flameVar_dripsGravityEnd",
    "stream/drips/GravityEnd",
    200.0,
    -1000.0,
    1000.0,
    "Fire gravity at end of life"
  },
  {
    "flameVar_dripsMaxRotVel",
    "stream/drips/MaxRotVel",
    60.0,
    0.0,
    200.0,
    "Maximum rotational velocity of flame"
  },
  {
    "flameVar_dripsFriction",
    "stream/drips/Friction",
    4.0,
    0.0,
    10.0,
    "Fraction of starting velocity to deccelerate per second"
  },
  {
    "flameVar_dripsEndSizeAdd",
    "stream/drips/EndSizeAdd",
    0.0,
    0.0,
    100.0,
    "Add this to the size of the chunk, to give the end size of the flame"
  },
  {
    "flameVar_dripsStartSizeScale",
    "stream/drips/StartSizeScale",
    0.1,
    0.1,
    20.0,
    "Multiply this by the size of the chunk, to give the start size of the flame"
  },
  {
    "flameVar_dripsEndSizeScale",
    "stream/drips/EndSizeScale",
    0.0,
    0.0,
    20.0,
    "Multiply this by the size of the chunk, to give the end size of the flame"
  },
  {
    "flameVar_smokeLife",
    "stream/smoke/Life",
    2.5,
    0.1,
    5.0,
    "Life duration of smoke"
  },
  {
    "flameVar_smokeLifeRand",
    "stream/smoke/LifeRand",
    1.0,
    0.1,
    5.0,
    "Random time to add to life duration of smoke"
  },
  {
    "flameVar_smokeSpeedScale",
    "stream/smoke/SpeedScale",
    0.34999999,
    0.0,
    1.0,
    "Fraction of chunk velocity to apply to starting velocity of smoke"
  },
  {
    "flameVar_smokeVelocityAddZ",
    "stream/smoke/VelocityAddZ",
    10.0,
    0.0,
    100.0,
    "Upwards velocity applied to starting velocity of smoke"
  },
  {
    "flameVar_smokeGravity",
    "stream/smoke/Gravity",
    -80.0,
    -500.0,
    0.0,
    "Starting gravity of smoke"
  },
  {
    "flameVar_smokeGravityEnd",
    "stream/smoke/GravityEnd",
    -100.0,
    -500.0,
    0.0,
    "Ending gravity of smoke"
  },
  {
    "flameVar_smokeMaxRotation",
    "stream/smoke/MaxRotation",
    20.0,
    0.0,
    180.0,
    "Maximum rotation (ROLL) of smoke"
  },
  {
    "flameVar_smokeMaxRotVel",
    "stream/smoke/MaxRotVel",
    20.0,
    0.0,
    200.0,
    "Maximum rotational velocity of smoke"
  },
  {
    "flameVar_smokeFriction",
    "stream/smoke/Friction",
    4.0,
    0.0,
    10.0,
    "Fraction of initial velocity to deccelerate per second"
  },
  {
    "flameVar_smokeEndSizeAdd",
    "stream/smoke/EndSizeAdd",
    80.0,
    0.0,
    200.0,
    "Add this to the size of the chunk, to give the end size of the smoke"
  },
  {
    "flameVar_smokeStartSizeAdd",
    "stream/smoke/StartSizeAdd",
    20.0,
    0.0,
    200.0,
    "Add this to the size of the chunk, to give the start size of the smoke"
  },
  {
    "flameVar_smokeOriginSizeOfsZScale",
    "stream/smoke/OrgSizeOfsZScale",
    0.69999999,
    0.0,
    3.0,
    "Fraction of size to add to the Z origin of the smoke"
  },
  {
    "flameVar_smokeOriginOfsZ",
    "stream/smoke/OriginOfsZ",
    4.0,
    -30.0,
    100.0,
    "Absoluate value to add to the starting Z origin"
  },
  {
    "flameVar_smokeFadein",
    "stream/smoke/Fadein",
    0.5,
    0.0,
    1.0,
    "Fraction of life to spend fading the smoke in"
  },
  {
    "flameVar_smokeFadeout",
    "stream/smoke/Fadeout",
    0.5,
    0.0,
    1.0,
    "Fraction of life to spend fading the smoke out"
  },
  {
    "flameVar_smokeMaxAlpha",
    "stream/smoke/MaxAlpha",
    0.2,
    0.0,
    1.0,
    "Maximum alpha for smoke"
  },
  {
    "flameVar_smokeBrightness",
    "stream/smoke/Brightness",
    0.5,
    0.0,
    1.0,
    "Brightness value of smoke. 1.0 = untouched"
  },
  {
    "flameVar_smokeOriginOffset",
    "stream/smoke/OriginOffset",
    72.0,
    0.0,
    300.0,
    "Offset multiplier to shift smoke create location"
  },
  {
    "flameVar_collisionSpeedScale",
    "collisionSpeedScale",
    0.60000002,
    0.0,
    1.0,
    "Reduce speed by this scale when colliding with solid objects"
  },
  {
    "flameVar_collisionVolumeScale",
    "collisionVolumeScale",
    0.5,
    0.0,
    1.0,
    "Value used to scale the volume/size of the flame chunks used for character collision"
  },
  { NULL, NULL, 0.0, 0.0, 0.0, NULL },
  { NULL, NULL, 0.0, 0.0, 0.0, NULL },
  { NULL, NULL, 0.0, 0.0, 0.0, NULL },
  { NULL, NULL, 0.0, 0.0, 0.0, NULL },
  { NULL, NULL, 0.0, 0.0, 0.0, NULL },
  { NULL, NULL, 0.0, 0.0, 0.0, NULL },
  { NULL, NULL, 0.0, 0.0, 0.0, NULL },
  { NULL, NULL, 0.0, 0.0, 0.0, NULL },
  { NULL, NULL, 0.0, 0.0, 0.0, NULL },
  { NULL, NULL, 0.0, 0.0, 0.0, NULL },
  { NULL, NULL, 0.0, 0.0, 0.0, NULL },
  { NULL, NULL, 0.0, 0.0, 0.0, NULL },
  { NULL, NULL, 0.0, 0.0, 0.0, NULL },
  { NULL, NULL, 0.0, 0.0, 0.0, NULL },
  { NULL, NULL, 0.0, 0.0, 0.0, NULL },
  { NULL, NULL, 0.0, 0.0, 0.0, NULL },
  { NULL, NULL, 0.0, 0.0, 0.0, NULL },
  { NULL, NULL, 0.0, 0.0, 0.0, NULL },
  { NULL, NULL, 0.0, 0.0, 0.0, NULL },
  { NULL, NULL, 0.0, 0.0, 0.0, NULL },
  { NULL, NULL, 0.0, 0.0, 0.0, NULL },
  { NULL, NULL, 0.0, 0.0, 0.0, NULL }
};

cspField_t flameTableFields[119] =
{
  { "flameVar_streamChunkGravityStart", 0, 7 },
  { "flameVar_streamChunkGravityEnd", 4, 7 },
  { "flameVar_streamChunkMaxSize", 8, 7 },
  { "flameVar_streamChunkStartSize", 12, 7 },
  { "flameVar_streamChunkEndSize", 16, 7 },
  { "flameVar_streamChunkStartSizeRand", 20, 7 },
  { "flameVar_streamChunkEndSizeRand", 24, 7 },
  { "flameVar_streamChunkDistScalar", 28, 7 },
  { "flameVar_streamChunkDistSwayScale", 32, 7 },
  { "flameVar_streamChunkDistSwayVelMax", 36, 7 },
  { "flameVar_streamChunkSpeed", 40, 7 },
  { "flameVar_streamChunkDecel", 44, 7 },
  { "flameVar_streamChunkVelocityAddScale", 48, 7 },
  { "flameVar_streamChunkDuration", 52, 7 },
  { "flameVar_streamChunkDurationScaleMaxVel", 56, 7 },
  { "flameVar_streamChunkDurationVelScalar", 60, 7 },
  { "flameVar_streamChunkSizeSpeedScale", 64, 7 },
  { "flameVar_streamChunkSizeAgeScale", 68, 7 },
  { "flameVar_streamChunkSpawnFireIntervalStart", 72, 7 },
  { "flameVar_streamChunkSpawnFireIntervalEnd", 76, 7 },
  { "flameVar_streamChunkSpawnFireMinLifeFrac", 80, 7 },
  { "flameVar_streamChunkSpawnFireMaxLifeFrac", 84, 7 },
  { "flameVar_streamChunkFireMinLifeFrac", 88, 7 },
  { "flameVar_streamChunkFireMinLifeFracStart", 92, 7 },
  { "flameVar_streamChunkFireMinLifeFracEnd", 96, 7 },
  { "flameVar_streamChunkDripsMinLifeFrac", 100, 7 },
  { "flameVar_streamChunkDripsMinLifeFracStart", 104, 7 },
  { "flameVar_streamChunkDripsMinLifeFracEnd", 108, 7 },
  { "flameVar_streamChunkRotationRange", 112, 7 },
  { "flameVar_streamSizeRandSinWave", 116, 7 },
  { "flameVar_streamSizeRandCosWave", 120, 7 },
  { "flameVar_streamDripsChunkInterval", 124, 7 },
  { "flameVar_streamDripsChunkMinFrac", 128, 7 },
  { "flameVar_streamDripsChunkRandFrac", 132, 7 },
  { "flameVar_streamSmokeChunkInterval", 136, 7 },
  { "flameVar_streamSmokeChunkMinFrac", 140, 7 },
  { "flameVar_streamSmokeChunkRandFrac", 144, 7 },
  { "flameVar_streamChunkCullDistSizeFrac", 148, 7 },
  { "flameVar_streamChunkCullMinLife", 152, 7 },
  { "flameVar_streamChunkCullMaxLife", 156, 7 },
  { "flameVar_streamFuelSizeStart", 160, 7 },
  { "flameVar_streamFuelSizeEnd", 164, 7 },
  { "flameVar_streamFuelLength", 168, 7 },
  { "flameVar_streamFuelNumSegments", 172, 7 },
  { "flameVar_streamFuelAnimLoopTime", 176, 7 },
  { "flameVar_streamFlameSizeStart", 180, 7 },
  { "flameVar_streamFlameSizeEnd", 184, 7 },
  { "flameVar_streamFlameLength", 188, 7 },
  { "flameVar_streamFlameNumSegments", 192, 7 },
  { "flameVar_streamFlameAnimLoopTime", 196, 7 },
  { "flameVar_streamPrimaryLightRadius", 200, 7 },
  { "flameVar_streamPrimaryLightRadiusFlutter", 204, 7 },
  { "flameVar_streamPrimaryLightR", 208, 7 },
  { "flameVar_streamPrimaryLightG", 212, 7 },
  { "flameVar_streamPrimaryLightB", 216, 7 },
  { "flameVar_streamPrimaryLightFlutterR", 220, 7 },
  { "flameVar_streamPrimaryLightFlutterG", 224, 7 },
  { "flameVar_streamPrimaryLightFlutterB", 228, 7 },
  { "flameVar_fireLife", 232, 7 },
  { "flameVar_fireLifeRand", 236, 7 },
  { "flameVar_fireSpeedScale", 240, 7 },
  { "flameVar_fireSpeedScaleRand", 244, 7 },
  { "flameVar_fireVelocityAddZ", 248, 7 },
  { "flameVar_fireVelocityAddZRand", 252, 7 },
  { "flameVar_fireVelocityAddSideways", 256, 7 },
  { "flameVar_fireGravity", 260, 7 },
  { "flameVar_fireGravityEnd", 264, 7 },
  { "flameVar_fireMaxRotVel", 268, 7 },
  { "flameVar_fireFriction", 272, 7 },
  { "flameVar_fireEndSizeAdd", 276, 7 },
  { "flameVar_fireStartSizeScale", 280, 7 },
  { "flameVar_fireEndSizeScale", 284, 7 },
  { "flameVar_dripsLife", 288, 7 },
  { "flameVar_dripsLifeRand", 292, 7 },
  { "flameVar_dripsSpeedScale", 296, 7 },
  { "flameVar_dripsSpeedScaleRand", 300, 7 },
  { "flameVar_dripsVelocityAddZ", 304, 7 },
  { "flameVar_dripsVelocityAddZRand", 308, 7 },
  { "flameVar_dripsVelocityAddSideways", 312, 7 },
  { "flameVar_dripsGravity", 316, 7 },
  { "flameVar_dripsGravityEnd", 320, 7 },
  { "flameVar_dripsMaxRotVel", 324, 7 },
  { "flameVar_dripsFriction", 328, 7 },
  { "flameVar_dripsEndSizeAdd", 332, 7 },
  { "flameVar_dripsStartSizeScale", 336, 7 },
  { "flameVar_dripsEndSizeScale", 340, 7 },
  { "flameVar_smokeLife", 344, 7 },
  { "flameVar_smokeLifeRand", 348, 7 },
  { "flameVar_smokeSpeedScale", 352, 7 },
  { "flameVar_smokeVelocityAddZ", 356, 7 },
  { "flameVar_smokeGravity", 360, 7 },
  { "flameVar_smokeGravityEnd", 364, 7 },
  { "flameVar_smokeMaxRotation", 368, 7 },
  { "flameVar_smokeMaxRotVel", 372, 7 },
  { "flameVar_smokeFriction", 376, 7 },
  { "flameVar_smokeEndSizeAdd", 380, 7 },
  { "flameVar_smokeStartSizeAdd", 384, 7 },
  { "flameVar_smokeOriginSizeOfsZScale", 388, 7 },
  { "flameVar_smokeOriginOfsZ", 392, 7 },
  { "flameVar_smokeFadein", 396, 7 },
  { "flameVar_smokeFadeout", 400, 7 },
  { "flameVar_smokeMaxAlpha", 404, 7 },
  { "flameVar_smokeBrightness", 408, 7 },
  { "flameVar_smokeOriginOffset", 412, 7 },
  { "flameVar_collisionSpeedScale", 416, 7 },
  { "flameVar_collisionVolumeScale", 420, 7 },
  { "name", 424, 0 },
  { "fire", 428, 11 },
  { "smoke", 432, 11 },
  { "heat", 436, 11 },
  { "drips", 440, 11 },
  { "streamFuel", 444, 11 },
  { "streamFuel2", 448, 11 },
  { "streamFlame", 452, 11 },
  { "streamFlame2", 456, 11 },
  { "flameOffLoopSound", 460, 0 },
  { "flameIgniteSound", 464, 0 },
  { "flameOnLoopSound", 468, 0 },
  { "flameCooldownSound", 472, 0 }
};

unsigned int flame_freeze_id;
unsigned int flame_spawn_id;

unsigned __int8 sv_flameSourceLookup[1024];
unsigned __int8 flameSourceLookup[1024];
flameSource_t flameSources[64];
flameSource_t sv_flameSources[64];

flameRender_s flameVarList[16];
int numFlameVars;

const int bg_iNumFlameTableFields = 119;

phys_static_array<flameGeneric_s *, 1000> sv_flames;
phys_static_array<flameGeneric_s *, 1000> cl_flames;

int numFlameTablesLoaded;
const char *loadedFlameTables[8];

const dvar_s *flameDvars[128];

const dvar_t *flame_test;
const dvar_t *flame_use_dvars;
const dvar_t *flame_render;
const dvar_t *flame_team_damage;
const dvar_t *flame_debug_render;
const dvar_t *flame_config_valid;
const dvar_t *default_flameVars_initialHitDamage;
const dvar_t *default_flameVars_timedDamageDuration;
const dvar_t *default_flameVars_timedDamageInterval;
const dvar_t *flameVar_editingFlameTable;
const dvar_t *flameVar_lastFlameTable;
const dvar_t *flame_kick_offset;
const dvar_t *flame_kick_speed;
const dvar_t *flame_kick_recover_speed;

cmd_function_s Flame_CMD_PrintDVarsToConsol_VAR;
cmd_function_s Flame_CMD_Test_Toggle_VAR;
cmd_function_s Flame_CMD_Use_Dvars_Toggle_VAR;

void __cdecl Flame_Init_FlameVars()
{
    int fieldIndex;

    memset(flameVarList, 0, sizeof(flameVarList));
    numFlameVars = 0;
    for (fieldIndex = 0; fieldIndex < bg_iNumFlameTableFields; ++fieldIndex)
    {
        if (flameVars[fieldIndex].dvarName)
            flameDvars[fieldIndex] = _Dvar_RegisterFloat(
                flameVars[fieldIndex].dvarName,
                flameVars[fieldIndex].defaultVal,
                flameVars[fieldIndex].minVal,
                flameVars[fieldIndex].maxVal,
                0,
                flameVars[fieldIndex].description);
    }
}

void __cdecl Flame_Reset_FlameVars()
{
    memset(flameVarList, 0, sizeof(flameVarList));
    numFlameVars = 0;
}

void __cdecl Flame_Init_Classes()
{
    flame_freeze_id = 0;
    flame_spawn_id = 0;
    Flame_Class_Chunk_Init();
    Flame_Class_Fire_Init();
    Flame_Class_Smoke_Init();
    Flame_Class_Stream_Init();
    Flame_Class_Drips_Init();
}

void __cdecl Flame_Init_Sources()
{
    int sourceIndex;

    Flame_Init_DVars();
    memset(flameSources, 0, sizeof(flameSources));
    memset(flameSourceLookup, 0, sizeof(flameSourceLookup));
    memset(sv_flameSources, 0, sizeof(sv_flameSources));
    memset(sv_flameSourceLookup, 0, sizeof(sv_flameSourceLookup));
    for (sourceIndex = 0; sourceIndex < 64; ++sourceIndex)
        sv_flameSources[sourceIndex].is_server_alloc = 1;
}

void __cdecl Flame_Init_DVars()
{
    if ( !flame_test )
    {
        flame_test = _Dvar_RegisterBool("flame_test", 0, 0x4080u, "Turn on/off Flamethrower testing");
        flame_use_dvars = _Dvar_RegisterBool(
                                                "flame_use_dvars",
                                                0,
                                                0x4080u,
                                                "Turn on/off use of dvars for flamethrower behavior");
        flame_render = _Dvar_RegisterBool("flame_render", 1, 0x4080u, "Turn on/off Flamethrower rendering");
        flame_team_damage = _Dvar_RegisterBool(
                                                    "flame_team_damage",
                                                    0,
                                                    0x4080u,
                                                    "Turn on/off Flamethrower damage to friendlies");
        flame_debug_render = _Dvar_RegisterInt(
                                                     "flame_debug_render",
                                                     0,
                                                     0,
                                                     2,
                                                     0x4080u,
                                                     "Turn on/off Flamethrower debug rendering");
        flame_config_valid = _Dvar_RegisterBool("flame_config_valid", 0, 0x4000u, "Internal use only");
        default_flameVars_initialHitDamage = _Dvar_RegisterFloat(
                                                                                     "defaultHitDamage",
                                                                                     5.0,
                                                                                     1.0,
                                                                                     200.0,
                                                                                     0,
                                                                                     "default initial damage delivered by getting hit by a flame ball");
        default_flameVars_timedDamageDuration = _Dvar_RegisterFloat(
                                                                                            "defaultDamageDuration",
                                                                                            300.0,
                                                                                            0.0,
                                                                                            10000.0,
                                                                                            0,
                                                                                            "default duration in milliseconds that damage should continue to get delivered");
        default_flameVars_timedDamageInterval = _Dvar_RegisterFloat(
                                                                                            "defaultDamageInterval",
                                                                                            300.0,
                                                                                            0.0,
                                                                                            10000.0,
                                                                                            0,
                                                                                            "default interval in milliseconds that damage should continue to get delivered");
        flameVar_editingFlameTable = _Dvar_RegisterString(
                                                                     "flameVar_editingFlameTable",
                                                                     "none",
                                                                     0,
                                                                     "This is the most recent flametable that has been put up for editing, mostly included to help artists");
        flameVar_lastFlameTable = _Dvar_RegisterString(
                                                                "flameVar_lastFlameTable",
                                                                "none",
                                                                0,
                                                                "This is the previous flametable that was put up for editing, mostly included to help artists");
        flame_kick_offset = _Dvar_RegisterVec3(
                                                    "flame_kick_offset",
                                                    (-2.0),
                                                    (0.0),
                                                    (-0.5),
                                                    -3.4028235e38,
                                                    3.4028235e38,
                                                    0x80u,
                                                    "Origin offset for flamethrower view model while firing.");
        flame_kick_speed = _Dvar_RegisterFloat(
                                                 "flame_kick_speed",
                                                 4.0,
                                                 0.0,
                                                 3.4028235e38,
                                                 0x80u,
                                                 "Speed at which the flamethrower view model transitions to flame_kick_offset.");
        flame_kick_recover_speed = _Dvar_RegisterFloat(
                                                                 "flame_kick_recover_speed",
                                                                 4.0,
                                                                 0.0,
                                                                 3.4028235e38,
                                                                 0x80u,
                                                                 "Speed at which the flamethrower view model transitions back from flame_kick_offset.");
    }
}

char __cdecl Flame_TableInDevGui(const char *tableName)
{
    int tableIndex;

    for (tableIndex = 0; tableIndex < numFlameTablesLoaded; ++tableIndex)
    {
        if (!I_strcmp(tableName, loadedFlameTables[tableIndex]))
            return 1;
    }
    return 0;
}

void __cdecl Flame_InitDevGui()
{
    unsigned int weaponIndex;
    int tableIndex;
    int dvarIndex;
    const WeaponDef *weapDef;

    numFlameTablesLoaded = 0;
    for (weaponIndex = 0; weaponIndex < BG_GetNumWeapons(); ++weaponIndex)
    {
        weapDef = BG_GetWeaponDef(weaponIndex);
        if (weapDef && weapDef->flameTableFirstPersonPtr && !Flame_TableInDevGui(weapDef->flameTableFirstPerson))
        {
            if (numFlameTablesLoaded >= 8
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\flame\\flame_system.cpp",
                    333,
                    0,
                    "%s\n\t(numFlameTablesLoaded) = %i",
                    "(numFlameTablesLoaded < 8)",
                    numFlameTablesLoaded))
            {
                __debugbreak();
            }
            loadedFlameTables[numFlameTablesLoaded] = weapDef->flameTableFirstPerson;
            if (!numFlameTablesLoaded)
            {
                Dvar_SetString((dvar_s*)flameVar_editingFlameTable, loadedFlameTables[0]);
                Flame_DVarsToFlameVars(weapDef->flameTableFirstPersonPtr);
            }
            ++numFlameTablesLoaded;
        }
        if (weapDef && weapDef->flameTableThirdPersonPtr && !Flame_TableInDevGui(weapDef->flameTableThirdPerson))
        {
            if (numFlameTablesLoaded >= 8
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\flame\\flame_system.cpp",
                    347,
                    0,
                    "%s\n\t(numFlameTablesLoaded) = %i",
                    "(numFlameTablesLoaded < 8)",
                    numFlameTablesLoaded))
            {
                __debugbreak();
            }
            loadedFlameTables[numFlameTablesLoaded] = weapDef->flameTableThirdPerson;
            if (!numFlameTablesLoaded)
            {
                Dvar_SetString((dvar_s *)flameVar_editingFlameTable, loadedFlameTables[0]);
                Flame_DVarsToFlameVars(weapDef->flameTableThirdPersonPtr);
            }
            ++numFlameTablesLoaded;
        }
    }
    Cmd_AddCommandInternal("flame_printdvars", Flame_CMD_PrintDVarsToConsol, &Flame_CMD_PrintDVarsToConsol_VAR);
    Cmd_AddCommandInternal("flame_test_toggle", Flame_CMD_Test_Toggle, &Flame_CMD_Test_Toggle_VAR);
    Cmd_AddCommandInternal("flame_use_dvars_toggle", Flame_CMD_Use_Dvars_Toggle, &Flame_CMD_Use_Dvars_Toggle_VAR);
    Cbuf_InsertText(0, "devgui_cmd \"FT/Toggle Flamethrower:1\" flame_test_toggle");
    Cbuf_InsertText(0, "devgui_cmd \"FT/Toggle Dvar Use:2\" flame_use_dvars_toggle");
    for (tableIndex = 0; tableIndex < numFlameTablesLoaded; ++tableIndex)
    {
        Cbuf_InsertText(
            0,
            va(
                "devgui_cmd \"FT/Flametables:3/%s:%i\" \"set flameVar_editingFlameTable %s\"",
                loadedFlameTables[tableIndex],
                tableIndex + 1,
                loadedFlameTables[tableIndex]));
    }
    for (dvarIndex = 0; dvarIndex < 128; ++dvarIndex)
    {
        if (flameVars[dvarIndex].name)
        {
            Cbuf_InsertText(
                0,
                va(
                    "devgui_dvar \"FT/Edit Dvars:4/%s:%i\" %s",
                    flameVars[dvarIndex].name,
                    dvarIndex + 1,
                    flameVars[dvarIndex].dvarName));
        }
    }
    Cbuf_InsertText(0, "devgui_cmd \"FT/Print DVars:5\" flame_printdvars");
}

void __cdecl Flame_Init()
{
    Flame_Init_Classes();
    Flame_Init_Sources();
}

void __cdecl Flame_Reset()
{
    Flame_Init_Classes();
    Flame_Init_Sources();
    Flame_Reset_FlameVars();
}

void __cdecl Flame_CMD_PrintDVarsToConsol()
{
    int fieldIndex;

    Com_Printf(0, "\n------------------------------------------\nDumping Flamethrower Dvars...\n\n[BEGIN]\n");
    Com_Printf(
        0,
        "These are the float values for the flametable %s. BE CAREFUL NOT TO CLOBBER THE MATERIAL AND SOUND NAMES IN THE WEAP"
        "ONSETTINGS.GDT!\n"
        "\n",
        flameVar_editingFlameTable->current.string);
    for (fieldIndex = 0; fieldIndex < bg_iNumFlameTableFields; ++fieldIndex)
    {
        if (flameVars[fieldIndex].dvarName)
            Com_Printf(0, "        \"%s\" \"%f\"\n", flameVars[fieldIndex].dvarName, flameDvars[fieldIndex]->current.value);
    }
    Com_Printf(0, "[END]\n\nDone.\n------------------------------------------\n");
}

void __cdecl Flame_CMD_Test_Toggle()
{
    if ( flame_test->current.enabled )
        Cbuf_InsertText(0, "flame_test 0");
    else
        Cbuf_InsertText(0, "flame_test 1");
}

void __cdecl Flame_CMD_Use_Dvars_Toggle()
{
    if ( flame_use_dvars->current.enabled )
        Cbuf_InsertText(0, "flame_use_dvars 0");
    else
        Cbuf_InsertText(0, "flame_use_dvars 1");
}

void __cdecl Flame_DVarsToFlameVars(flameTable *fTable)
{
    float *tableFloat;
    int fieldIndex;
    const cspField_t *field;

    iassert(fTable);
    iassert(fTable->name);

    if (I_strcmp(flameVar_editingFlameTable->current.string, flameVar_lastFlameTable->current.string))
    {
        tableFloat = &fTable->flameVar_streamChunkGravityStart;
        field = flameTableFields;
        for (fieldIndex = 0; fieldIndex < bg_iNumFlameTableFields; ++fieldIndex, ++field, ++tableFloat)
        {
            if (field->iFieldType == 7)
                Dvar_SetFloatByName(field->szName, *tableFloat);
        }
        Dvar_SetString((dvar_s *)flameVar_lastFlameTable, flameVar_editingFlameTable->current.string);
    }

    tableFloat = &fTable->flameVar_streamChunkGravityStart;
    field = flameTableFields;
    for (fieldIndex = 0; fieldIndex < bg_iNumFlameTableFields; ++fieldIndex, ++field, ++tableFloat)
    {
        if (field->iFieldType == 7)
            *tableFloat = Dvar_GetFloat(field->szName);
    }
}

flameTable *__cdecl Flame_FindFlameTable(const char *tableName)
{
    unsigned int weaponIndex;
    unsigned int numWeapons;
    const WeaponDef *weapDef;

    numWeapons = BG_GetNumWeapons();
    for (weaponIndex = 0; weaponIndex < numWeapons; ++weaponIndex)
    {
        weapDef = BG_GetWeaponDef(weaponIndex);
        if (weapDef && weapDef->flameTableFirstPersonPtr && !I_strcmp(weapDef->flameTableFirstPerson, tableName))
            return weapDef->flameTableFirstPersonPtr;
        if (weapDef && weapDef->flameTableThirdPersonPtr && !I_strcmp(weapDef->flameTableThirdPerson, tableName))
            return weapDef->flameTableThirdPersonPtr;
    }
    return 0;
}

float __cdecl Flame_SwayRand(float x, float y, int time)
{
    float sx = fmodf((float)time * x * 3.1415927f * 2.0f, 6.2831855f);
    float sy = fmodf((float)time * y * 3.1415927f * 2.0f, 6.2831855f);

    return sinf(sx) * cosf(sy);
}

flameSource_t *__cdecl SV_Flame_Source_Get(int entityNum)
{
    if (sv_flameSourceLookup[entityNum])
        return &sv_flameSources[sv_flameSourceLookup[entityNum] - 1];
    return 0;
}

flameSource_t *__cdecl Flame_Source_Get(int entityNum)
{
    if (flameSourceLookup[entityNum])
        return &flameSources[flameSourceLookup[entityNum] - 1];
    return 0;
}

flameRender_s *__cdecl Flame_Get_FlameRender(const char *name)
{
    bool isFound;
    flameRender_s *flameRenderEntry;
    int renderIndex;

    if (strlen(name) >= 0x7F
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\flame\\flame_system.cpp",
            579,
            0,
            "%s\n\t%s",
            "strlen( name ) < (MAX_FLAMEVAR_NAME-1)",
            "Flame config name is too long"))
    {
        __debugbreak();
    }

    isFound = 0;
    flameRenderEntry = flameVarList;
    for (renderIndex = 0; renderIndex < numFlameVars; ++renderIndex)
    {
        if (!strcmp(name, flameRenderEntry->name))
        {
            isFound = 1;
            break;
        }
        ++flameRenderEntry;
    }

    if (!isFound)
    {
        if (numFlameVars >= 15
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\flame\\flame_system.cpp",
                594,
                0,
                "%s",
                "numFlameVars < MAX_FLAMEVARLIST-1"))
        {
            __debugbreak();
        }
        flameRenderEntry = &flameVarList[numFlameVars++];
        I_strncpyz(flameRenderEntry->name, name, sizeof(flameRenderEntry->name));
    }
    return flameRenderEntry;
}

void __cdecl Flame_Item_Init(flameGeneric_s *item, unsigned int itemSize)
{
    flameGeneric_s *globalPrev;
    flameGeneric_s *globalNext;
    flameGeneric_s *localPrev;
    flameGeneric_s *localNext;

    globalPrev = item->listGlobal.prev;
    globalNext = item->listGlobal.next;
    localPrev = item->listLocal.prev;
    localNext = item->listLocal.next;
    memset(item, 0, itemSize);
    item->listGlobal.prev = globalPrev;
    item->listGlobal.next = globalNext;
    item->listLocal.prev = localPrev;
    item->listLocal.next = localNext;
}

void __cdecl Flame_List_Init(flameGeneric_s *list, int itemSize, int listSize)
{
    int itemIndex;
    char *listTrav;

    listTrav = (char *)list;
    for (itemIndex = 0; itemIndex < listSize; ++itemIndex)
    {
        ((flameGeneric_s *)listTrav)->listGlobal.prev = (flameGeneric_s *)(listTrav - itemSize);
        if (!itemIndex)
            ((flameGeneric_s *)listTrav)->listGlobal.prev = 0;
        ((flameGeneric_s *)listTrav)->listGlobal.next = (flameGeneric_s *)(listTrav + itemSize);
        if (itemIndex >= listSize - 1)
            ((flameGeneric_s *)listTrav)->listGlobal.next = 0;
        listTrav += itemSize;
    }
}

void __cdecl Flame_List_Move_Global(flameGeneric_s *item, flameGeneric_s **fromList, flameGeneric_s **toList)
{
    if ( *fromList )
    {
        if ( item == *fromList )
            *fromList = item->listGlobal.next;
        if ( item->listGlobal.prev )
            item->listGlobal.prev->listGlobal.next = item->listGlobal.next;
        if ( item->listGlobal.next )
            item->listGlobal.next->listGlobal.prev = item->listGlobal.prev;
        item->listGlobal.next = 0;
        item->listGlobal.prev = 0;
        if ( *toList )
        {
            item->listGlobal.next = *toList;
            (*toList)->listGlobal.prev = item;
        }
        *toList = item;
    }
}

void __cdecl Flame_List_Add_Local(flameGeneric_s *item, flameGeneric_s **localList)
{
    item->listLocal.prev = 0;
    item->listLocal.next = *localList;
    if ( *localList )
        (*localList)->listLocal.prev = item;
    *localList = item;
}

void __cdecl Flame_List_Sub_Local(flameGeneric_s *item, flameGeneric_s **localList)
{
    if ( *localList == item )
        *localList = item->listLocal.next;
    if ( item->listLocal.prev )
        item->listLocal.prev->listLocal.next = item->listLocal.next;
    if ( item->listLocal.next )
        item->listLocal.next->listLocal.prev = item->listLocal.prev;
    item->listLocal.prev = 0;
    item->listLocal.next = 0;
}

void __cdecl CG_Flame_Age_All_Objects(int time)
{
    PROF_SCOPED("CG_Flame_Age_All_Objects");

    cl_flames.m_alloc_count = 0;
    Flame_Class_Chunk_Age(0, time);
    Flame_Class_Fire_Age(0, time);
    Flame_Class_Smoke_Age(0, time);
    Flame_Class_Stream_Age(0);
    Flame_Class_Drips_Age(0, time);
}

void __cdecl SV_Flame_Age_All_Objects(int time)
{
    PROF_SCOPED("SV_Flame_Age_All_Objects");

    sv_flames.m_alloc_count = 0;
    Flame_Class_Chunk_Age(1, time);
    Flame_Class_Fire_Age(1, time);
    Flame_Class_Smoke_Age(1, time);
    Flame_Class_Stream_Age(1);
    Flame_Class_Drips_Age(1, time);
}

flameSource_t *__cdecl Flame_Source_Alloc(int entityNum)
{
    flameSource_t *source;
    unsigned __int8 sourceIndex;

    sourceIndex = 0;
    source = flameSources;
    while (sourceIndex < 64)
    {
        if (!source->inUse)
        {
            source->entityNum = entityNum;
            source->inUse = 1;
            flameSourceLookup[entityNum] = sourceIndex + 1;
            Flame_Class_Source_Sound_Init(source);
            return source;
        }
        ++sourceIndex;
        ++source;
    }
    return 0;
}

flameSource_t *__cdecl SV_Flame_Source_Alloc(int entityNum)
{
    flameSource_t *source;
    unsigned __int8 sourceIndex;

    sourceIndex = 0;
    source = sv_flameSources;
    while (sourceIndex < 64)
    {
        if (!source->inUse)
        {
            source->entityNum = entityNum;
            source->inUse = 1;
            sv_flameSourceLookup[entityNum] = sourceIndex + 1;
            return source;
        }
        ++sourceIndex;
        ++source;
    }
    return 0;
}

void __cdecl Flame_Render_Sprite(
    cg_s *clientGlobals,
    Material *material,
    flameGeneric_s *flameGeneric,
    float alpha,
    float frame,
    unsigned __int8 col)
{
    float angles[3];
    unsigned __int8 rgbaColor[4];

    if ( !material )
        return;

    rgbaColor[0] = col;
    rgbaColor[1] = col;
    rgbaColor[2] = col;
    rgbaColor[3] = (int)(alpha * 255.0f);
    AxisToAngles(clientGlobals->refdef.viewaxis, angles);
    // CoDMPServer.c:708423-708424 — v8 = phys.rotation shares stack with angles[2].
    angles[2] = flameGeneric->phys.rotation;
    if ( frame < 0.0f )
        frame = (float)(flameGeneric->age.lastUpdateTime - flameGeneric->age.startTime)
            / (float)(flameGeneric->age.endTime - flameGeneric->age.startTime);
    R_BeginCodeMeshVerts();
    R_GenerateQuadStampCodeMeshVerts(
        material,
        angles,
        flameGeneric->phys.origin,
        flameGeneric->size.current,
        rgbaColor,
        frame);
    R_EndCodeMeshVerts();
}

void __cdecl Flame_Render_Sprites(
    cg_s *clientGlobals,
    Material *material,
    flameGeneric_s **flameGenericList,
    int numItems)
{
    renderQuad_t quads[512];
    float viewAxis[3][3];
    int quadCount;
    int totalCount;
    flameGeneric_s **cur;

    if ( !material || numItems <= 0 )
        return;

    AxisCopy(clientGlobals->refdef.viewaxis, viewAxis);

    cur = &flameGenericList[numItems - 1];

    R_BeginCodeMeshVerts();

    totalCount = 0;

    do
    {
        quadCount = 0;

        while (totalCount < numItems && quadCount < 512)
        {
            flameGeneric_s *f = *cur;
            renderQuad_t *q = &quads[quadCount];

            // CoDMPServer.c:7089470-7089472 — copy origin xyz only (*v7 = **v12; v7[1]=v8[1]; v7[2]=v8[2]).
            q->pos[0] = f->phys.origin[0];
            q->pos[1] = f->phys.origin[1];
            q->pos[2] = f->phys.origin[2];
            // CoDMPServer.c:1057385 reads pos[3] into binormalSign (v11 = v7->pos[3]).
            // Retail stack leaves pos[3] uninitialized; 1.0f matches R_BuildQuadStampCodeMeshVerts
            // binormalSign=1.0f (CoDMPServer.c:1057397 / r_sprite.cpp R_BuildQuadStampCodeMeshVerts).
            q->pos[3] = 1.0f;

            // CoDMPServer.c:7089473 — *((int *)*v12 + 11) = phys.rotation (offset 0x2C).
            q->rotation = f->phys.rotation;
            // CoDMPServer.c:7089474 — *((int *)*v12 + 13) = size.current (offset 0x34).
            q->radius = f->size.current;

            // CoDMPServer.c:7089475-7089476 — lifeFrac from age.{lastUpdateTime,startTime,endTime}.
            q->lifeFrac = (float)(f->age.lastUpdateTime - f->age.startTime)
                / (float)(f->age.endTime - f->age.startTime);

            --cur;
            ++quadCount;
            ++totalCount;
        }

        R_GenerateQuadStampCodeMeshVertsArray(
            material,
            quads,
            quadCount,
            viewAxis
        );

    } while (totalCount < numItems);

    R_EndCodeMeshVerts();
}

static const WeaponDef *CG_Flame_ResolveGasWeapDef(
                unsigned int weaponIndex,
                unsigned int primaryWeaponIndex,
                const WeaponDef **tagWeapDefOut)
{
    const WeaponDef *weapDef;
    const WeaponVariantDef *weapVariantDef;
    unsigned int altIndex;

    if ( !weaponIndex )
        return 0;
    weapDef = BG_GetWeaponDef(weaponIndex);
    if ( !weapDef )
        return 0;

    if ( weapDef->weapType == WEAPTYPE_GAS )
    {
        if ( tagWeapDefOut )
            *tagWeapDefOut = weapDef;
        return weapDef;
    }

    if ( weapDef->inventoryType == WEAPINVENTORY_ALTMODE && primaryWeaponIndex )
    {
        weapVariantDef = BG_GetWeaponVariantDef(primaryWeaponIndex);
        if ( weapVariantDef && weapVariantDef->altWeaponIndex )
        {
            altIndex = weapVariantDef->altWeaponIndex;
            weapDef = BG_GetWeaponDef(altIndex);
            if ( weapDef && weapDef->weapType == WEAPTYPE_GAS )
            {
                if ( tagWeapDefOut )
                    *tagWeapDefOut = BG_GetWeaponDef(primaryWeaponIndex);
                return weapDef;
            }
        }
    }
    return 0;
}

static void CG_Flame_DeactivateLocalSource(int localClientNum, cg_s *cgameGlob)
{
    flameSource_t *source;
    const WeaponDef *gasWeapDef;
    flameTable *table;
    int clientNum;
    unsigned int weaponIndex;
    bool isFirstPerson;

    clientNum = cgameGlob->predictedPlayerState.clientNum;
    weaponIndex = CG_GetPlayerWeapon(&cgameGlob->predictedPlayerState, localClientNum);
    gasWeapDef = CG_Flame_ResolveGasWeapDef(
        weaponIndex,
        cgameGlob->predictedPlayerState.lastWeaponAltModeSwitch,
        0);
    if ( gasWeapDef )
        return;

    source = Flame_Source_Get(clientNum);
    if ( !source || (!source->wasFiring && !source->currentStream) )
        return;

    table = source->currentStream ? source->currentStream->flameVars : 0;
    isFirstPerson = cgameGlob->nextSnap->ps.clientNum == clientNum
        && ((cgameGlob->nextSnap->ps.otherFlags & 2) == 0
            || cgameGlob->renderingThirdPerson != TP_FOR_MODEL);
    if ( source->currentStream )
    {
        Flame_Class_Stream_Free(0, source->currentStream);
        source->currentStream = 0;
    }
    source->bIsFiring = 0;

    if ( table )
    {
        Flame_Class_Source_Sound(
            localClientNum,
            0,
            isFirstPerson,
            cgameGlob->time,
            table,
            source);
    }
}


int __cdecl Flame_GetLocalClientFlameSource(int localClientNum, int EntNum)
{
    // CoDMPServer.c: idb import — entity index is the flameSources lookup key.
    return EntNum;
}

bool __cdecl Flame_GetLocalClientSourceRange(const char *__formal)
{
    // CoDMPServer.c:894990 — always returns 1 in retail.
    return 1;
}

void __cdecl Flame_Source_Update(flameSource_t *source, flameWeaponConfig_t *weaponConfig)
{
    source->bIsFiring = weaponConfig->bIsFiring;
}

void __cdecl SV_Flame_Update_Source_Internal(
                flameSource_t *source,
                int entityNum,
                flameWeaponConfig_t *weaponConfig,
                int time)
{
    // CoDMPServer.c:708502-708516
    if ( weaponConfig->bIsFiring != source->bIsFiring )
        source->currentStream = 0;
    if ( weaponConfig->bIsFiring || weaponConfig->bFireWhileIdle )
        Flame_Class_Stream_Fire_Chunks(source, weaponConfig, time, 0);
    Flame_Source_Update(source, weaponConfig);
}

void __cdecl CG_Flame_Update_Source_Internal(
                flameSource_t *source,
                int entityNum,
                flameWeaponConfig_t *weaponConfig,
                int time,
                flameRender_s *flameRend)
{
    // CoDMPServer.c:708510-708516
    if ( weaponConfig->bIsFiring != source->bIsFiring )
        source->currentStream = 0;
    if ( weaponConfig->bIsFiring || weaponConfig->bFireWhileIdle )
        Flame_Class_Stream_Fire_Chunks(source, weaponConfig, time, flameRend);
    Flame_Source_Update(source, weaponConfig);
}

void __cdecl SV_Flame_Update_Source(int entityNum, flameWeaponConfig_t *weaponConfig, int time)
{
    flameSource_t *source;

    source = SV_Flame_Source_Get(entityNum);
    if (!source)
    {
        source = SV_Flame_Source_Alloc(entityNum);
        if (!source
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\flame\\flame_system.cpp",
                1003,
                0,
                "%s\n\t%s",
                "source",
                "No free flameSources"))
        {
            __debugbreak();
        }
    }
    SV_Flame_Update_Source_Internal(source, entityNum, weaponConfig, time);
}

void __cdecl CG_Flame_Update_Source(int localClientNum)
{
    int entityFlameSourceNum;
    int time;
    float dir[3];
    float start[3];
    trace_t trace;
    flameSource_t *source;
    const clientInfo_t *ci;
    const centity_s *player;
    int clientIndex;
    int weapon;
    const WeaponDef *gunnerWeapDef;
    int gunnerIndex;
    const vehicle_info_t *info;
    unsigned __int8 thirdBoneIndex;
    bool isFireEvent;
    flameWeaponConfig_t flameWeaponConfig;
    flameTable *table;
    int thirdDobjHandle;
    bool isFirstPerson;
    flameRender_s *flameRend;
    unsigned __int8 firstBoneIndex;
    orientation_t thirdOrient;
    int firstDobjHandle;
    orientation_t firstOrient;
    bool isFiring;
    unsigned int bits;
    int snapshotWordIndex;
    unsigned __int16 boneName;
    const centity_s *centForFlags;
    cg_s *cgameGlob;
    centity_s *cent;
    unsigned int centInNextSnapshot[32];
    int usingVehicleTagFlash;
    int entityIndex;
    const WeaponDef *weapDef;
    const WeaponDef *tagWeapDef;
    int clientNum;

    boneName = scr_const.tag_flash;
    weapDef = 0;
    cgameGlob = CG_GetLocalClientGlobals(localClientNum);
    if (cgameGlob->nextSnap && (!zombietron->current.enabled || localClientNum <= 0))
    {
        memcpy(centInNextSnapshot, cgameGlob->centInNextSnapshot, sizeof(centInNextSnapshot));
        clientNum = cgameGlob->clientNum;
        centInNextSnapshot[clientNum >> 5] |= 0x80000000 >> (clientNum & 0x1F);
        clientNum = cgameGlob->predictedPlayerState.clientNum;
        centInNextSnapshot[clientNum >> 5] |= 0x80000000 >> (clientNum & 0x1F);
        for (snapshotWordIndex = 0; snapshotWordIndex < 32; ++snapshotWordIndex)
        {
            bits = centInNextSnapshot[snapshotWordIndex];
            entityIndex = 32 * snapshotWordIndex;
            while (bits)
            {
                if ((bits & 0x80000000) != 0)
                {
                    cent = CG_GetEntity(localClientNum, entityIndex);
                    centForFlags = cent;
                    usingVehicleTagFlash = 0;
                    if ( cent->nextValid )
                    {
                        unsigned int primaryWeaponIndex;
                        unsigned int entityWeaponIndex;

                        entityWeaponIndex = cent->nextState.weapon;
                        primaryWeaponIndex = cent->nextState.lerp.u.player.primaryWeapon;
                        if ( entityIndex == cgameGlob->predictedPlayerState.clientNum )
                        {
                            entityWeaponIndex = CG_GetPlayerWeapon(
                                &cgameGlob->predictedPlayerState,
                                localClientNum);
                            primaryWeaponIndex = cgameGlob->predictedPlayerState.lastWeaponAltModeSwitch;
                        }
                        else if ( cent->nextState.eType == 1 )
                        {
                            entityWeaponIndex = CG_GetClientWeapon(
                                cent->nextState.clientNum,
                                localClientNum);
                        }
                        tagWeapDef = 0;
                        weapDef = CG_Flame_ResolveGasWeapDef(
                            entityWeaponIndex,
                            primaryWeaponIndex,
                            &tagWeapDef);
                        if (cent->nextState.eType == 14)
                        {
                            info = CG_GetVehicleInfo(cent->nextState.vehicleState.vehicleInfoIndex);
                            for (gunnerIndex = 0; gunnerIndex < 4; ++gunnerIndex)
                            {
                                weapon = info->gunnerWeaponIndex[gunnerIndex];
                                gunnerWeapDef = BG_GetWeaponDef(weapon);
                                if (gunnerWeapDef && gunnerWeapDef->weapType == WEAPTYPE_GAS)
                                {
                                    boneName = scr_const.tag_flash_gunner[gunnerIndex];
                                    usingVehicleTagFlash = 1;
                                    weapDef = gunnerWeapDef;
                                    tagWeapDef = gunnerWeapDef;
                                    for (clientIndex = 0; clientIndex < com_maxclients->current.integer; ++clientIndex)
                                    {
                                        player = CG_GetEntity(localClientNum, clientIndex);
                                        if (player->nextState.eType == 1)
                                        {
                                            ci = &cgameGlob->bgs.clientinfo[player->nextState.clientNum];
                                            if (ci->attachedVehEntNum == entityIndex && ci->attachedVehSeat == gunnerIndex + 1)
                                            {
                                                centForFlags = player;
                                                break;
                                            }
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                        if (centForFlags && weapDef && weapDef->weapType == WEAPTYPE_GAS)
                        {
                            if ( !tagWeapDef )
                                tagWeapDef = weapDef;
                            thirdDobjHandle = cent->nextState.number;
                            firstDobjHandle = CG_WeaponDObjHandle(localClientNum);
                            isFirstPerson = cgameGlob->nextSnap->ps.clientNum == entityIndex
                                && ((cgameGlob->nextSnap->ps.otherFlags & 2) == 0
                                    || cgameGlob->renderingThirdPerson != TP_FOR_MODEL);
                            if (Demo_IsThirdPersonCamera() || Demo_IsMovieCamera())
                                isFirstPerson = 0;
                            if (!tagWeapDef->bUseAltTagFlash || isFirstPerson)
                            {
                                if (!usingVehicleTagFlash)
                                    boneName = scr_const.tag_flash;
                            }
                            else
                            {
                                boneName = scr_const.tag_flash_alt;
                            }
                            firstBoneIndex = -2;
                            thirdBoneIndex = -2;
                            isFireEvent = cent->nextState.eType == 50;
                            if (cent->nextState.eType == 6 && (centForFlags->nextState.lerp.eFlags & 0x40) != 0)
                                isFireEvent = 1;
                            if ((isFirstPerson
                                    || isFireEvent
                                    || CG_GetBoneIndex(localClientNum, thirdDobjHandle, boneName, &thirdBoneIndex))
                                && (!isFirstPerson || CG_GetBoneIndex(localClientNum, firstDobjHandle, boneName, &firstBoneIndex)))
                            {
                                // CoDMPServer.c:708699-708711 — eFlags 0x40 gates pilot + stream update.
                                if (flame_test->current.enabled)
                                {
                                    isFiring = 1;
                                }
                                else if ( (centForFlags->nextState.lerp.eFlags & 0x40) == 0
                                    || (cent->nextState.lerp.eFlags & 0x300) != 0
                                    || cgameGlob->predictedPlayerState.waterlevel >= 3 )
                                {
                                    isFiring = isFireEvent;
                                }
                                else
                                {
                                    isFiring = 1;
                                }
                                if ( isFiring )
                                    cent->bMuzzleFlash = 1;
                                if ( isFiring && isFirstPerson && cent->lastMuzzleFlash < cgameGlob->time - weapDef->iRechamberBoltTime )
                                {
                                    cent->bMuzzleFlash = 1;
                                    cent->lastMuzzleFlash = cgameGlob->time;
                                }
                                if ( isFiring )
                                {
                                    if (isFirstPerson)
                                    {
                                        FX_GetBoneOrientation(localClientNum, firstDobjHandle, firstBoneIndex, &firstOrient);
                                    }
                                    else if (isFireEvent)
                                    {
                                        thirdOrient.origin[0] = cent->pose.origin[0];
                                        thirdOrient.origin[1] = cent->pose.origin[1];
                                        thirdOrient.origin[2] = cent->pose.origin[2];
                                        AnglesToAxis(cent->pose.angles, thirdOrient.axis);
                                    }
                                    else
                                    {
                                        FX_GetBoneOrientation(localClientNum, thirdDobjHandle, thirdBoneIndex, &thirdOrient);
                                    }
                                }
                                if ( isFirstPerson )
                                {
                                    table = weapDef->flameTableFirstPersonPtr;
                                    flameRend = Flame_Get_FlameRender(table->name);
                                }
                                else
                                {
                                    table = weapDef->flameTableThirdPersonPtr;
                                    flameRend = Flame_Get_FlameRender(table->name);
                                }
                                {
                                if ( isFiring )
                                {
                                    if (isFirstPerson)
                                    {
                                        flameWeaponConfig.origin[0] = firstOrient.origin[0];
                                        flameWeaponConfig.origin[1] = firstOrient.origin[1];
                                        flameWeaponConfig.origin[2] = firstOrient.origin[2];
                                        AxisToAngles(firstOrient.axis, flameWeaponConfig.angle);
                                    }
                                    else
                                    {
                                        flameWeaponConfig.origin[0] = thirdOrient.origin[0];
                                        flameWeaponConfig.origin[1] = thirdOrient.origin[1];
                                        flameWeaponConfig.origin[2] = thirdOrient.origin[2];
                                        AxisToAngles(thirdOrient.axis, flameWeaponConfig.angle);
                                    }
                                }
                                else
                                {
                                    flameWeaponConfig.origin[0] = cent->nextState.lerp.pos.trBase[0];
                                    flameWeaponConfig.origin[1] = cent->nextState.lerp.pos.trBase[1];
                                    flameWeaponConfig.origin[2] = cent->nextState.lerp.pos.trBase[2];
                                    flameWeaponConfig.angle[0] = cent->nextState.lerp.apos.trBase[0];
                                    flameWeaponConfig.angle[1] = cent->nextState.lerp.apos.trBase[1];
                                    flameWeaponConfig.angle[2] = cent->nextState.lerp.apos.trBase[2];
                                }
                                flameWeaponConfig.fTable = table;
                                flameWeaponConfig.bFireWhileIdle = 0;
                                flameWeaponConfig.bIsFiring = isFiring;
                                flameWeaponConfig.burnRate = 1.0f;
                                flameWeaponConfig.strength = 1.0f;
                                flameWeaponConfig.thickness = 1.0f;
                                flameWeaponConfig.entityOrigin[0] = cent->nextState.lerp.pos.trBase[0];
                                flameWeaponConfig.entityOrigin[1] = cent->nextState.lerp.pos.trBase[1];
                                flameWeaponConfig.entityOrigin[2] = cent->nextState.lerp.pos.trBase[2];
                                flameWeaponConfig.damage = weapDef->damage;
                                flameWeaponConfig.damageDuration = weapDef->damageDuration;
                                flameWeaponConfig.damageInterval = weapDef->damageInterval;
                                entityFlameSourceNum = Flame_GetLocalClientFlameSource(localClientNum, entityIndex);
                                source = Flame_Source_Get(entityFlameSourceNum);
                                if (!source)
                                {
                                    source = Flame_Source_Alloc(entityFlameSourceNum);
                                    if (!source
                                        && !Assert_MyHandler(
                                            "C:\\projects_pc\\cod\\codsrc\\src\\flame\\flame_system.cpp",
                                            1271,
                                            0,
                                            "%s\n\t%s",
                                            "source",
                                            "No free flameSources"))
                                    {
                                        __debugbreak();
                                    }
                                }
                                if (source)
                                {
                                    source->entityNum = entityFlameSourceNum;
                                    source->firstDobjHandle = firstDobjHandle;
                                    source->firstBoneIndex = firstBoneIndex;
                                    source->thirdDobjHandle = thirdDobjHandle;
                                    source->thirdBoneIndex = thirdBoneIndex;
                                    source->isKillcamFlame = cgameGlob->inKillCam;
                                    source->firingThroughGeo = 0;
                                    if ( isFiring && !isFirstPerson )
                                    {
                                        memset(&trace, 0, 16);
                                        AngleVectors(flameWeaponConfig.angle, dir, 0, 0);
                                        start[0] = -23.0f * dir[0] + flameWeaponConfig.origin[0];
                                        start[1] = -23.0f * dir[1] + flameWeaponConfig.origin[1];
                                        start[2] = -23.0f * dir[2] + flameWeaponConfig.origin[2];
                                        if (flame_debug_render->current.integer > 0)
                                            CG_DebugLine(start, flameWeaponConfig.origin, colorRed, 0, 300);
                                        CG_LocationalTrace(
                                            &trace,
                                            start,
                                            flameWeaponConfig.origin,
                                            source->entityNum,
                                            0x280E833,
                                            0,
                                            0);
                                        if (trace.fraction < 1.0f)
                                        {
                                            flameWeaponConfig.origin[0] = start[0];
                                            flameWeaponConfig.origin[1] = start[1];
                                            flameWeaponConfig.origin[2] = start[2];
                                            source->firingThroughGeo = 1;
                                        }
                                    }
                                    time = cgameGlob->time;
                                    CG_Flame_Update_Source_Internal(source, entityIndex, &flameWeaponConfig, time, flameRend);
                                    Flame_Class_Source_Sound(
                                        localClientNum,
                                        isFiring,
                                        isFirstPerson,
                                        time,
                                        table,
                                        source);
                                }
                                }
                            }
                        }
                    }
                }
                ++entityIndex;
                bits *= 2;
            }
        }
    }
    CG_Flame_DeactivateLocalSource(localClientNum, cgameGlob);
}

void __cdecl Flame_Generate_Verts(int localClientNum)
{
    int renderIndex;

    PROF_SCOPED("Flame_Generate_Verts");
    ZoneTextF("(cl=%d)", localClientNum);
    if (flame_render->current.enabled)
    {
        Flame_Class_Smoke_Render_All(localClientNum);
        Flame_Class_Fire_Render_All(localClientNum);
        Flame_Class_Drips_Render_All(localClientNum);
        Flame_Class_Chunk_Render_All(localClientNum);
        Flame_Class_Stream_Render_All(localClientNum);
        for (renderIndex = 0; renderIndex < numFlameVars; ++renderIndex)
        {
            Flame_Class_Fire_Render_Local_List(localClientNum, (flameFire_t *)flameVarList[renderIndex].fireList);
            Flame_Class_Drips_Render_Local_List(localClientNum, (flameDrips_t *)flameVarList[renderIndex].dripsList);
        }
    }
}

void __cdecl CG_Flame_Update_ViewModel(int localClientNum, centity_s *cent)
{
    float oldOffset;
    int axisIndex;
    int recoverAxisIndex;
    int kickAxisIndex;
    cg_s *cgameGlob;

    cgameGlob = CG_GetLocalClientGlobals(localClientNum);
    if ((cent->nextState.lerp.eFlags & 0x40) != 0)
    {
        for (kickAxisIndex = 0; kickAxisIndex < 3; ++kickAxisIndex)
        {
            cgameGlob->flamethrowerKickOffset[kickAxisIndex] =
                flame_kick_offset->current.vector[kickAxisIndex]
                * flame_kick_speed->current.value
                * cgameGlob->frametime
                / 1000.0f
                + cgameGlob->flamethrowerKickOffset[kickAxisIndex];
            if (fabs(cgameGlob->flamethrowerKickOffset[kickAxisIndex]) > fabs(flame_kick_offset->current.vector[kickAxisIndex]))
                cgameGlob->flamethrowerKickOffset[kickAxisIndex] = flame_kick_offset->current.vector[kickAxisIndex];
        }
    }
    else
    {
        for (recoverAxisIndex = 0; recoverAxisIndex < 3; ++recoverAxisIndex)
        {
            oldOffset = cgameGlob->flamethrowerKickOffset[recoverAxisIndex];
            cgameGlob->flamethrowerKickOffset[recoverAxisIndex] = oldOffset
                - flame_kick_offset->current.vector[recoverAxisIndex]
                * flame_kick_recover_speed->current.value
                * cgameGlob->frametime
                / 1000.0f;
            if (fabs(cgameGlob->flamethrowerKickOffset[recoverAxisIndex]) > fabs(oldOffset))
                cgameGlob->flamethrowerKickOffset[recoverAxisIndex] = 0.0f;
        }
    }
    for (axisIndex = 0; axisIndex < 3; ++axisIndex)
    {
        cgameGlob->viewModelAxis[3][0] =
            cgameGlob->flamethrowerKickOffset[axisIndex] * cgameGlob->viewModelAxis[axisIndex][0]
            + cgameGlob->viewModelAxis[3][0];
        cgameGlob->viewModelAxis[3][1] =
            cgameGlob->flamethrowerKickOffset[axisIndex] * cgameGlob->viewModelAxis[axisIndex][1]
            + cgameGlob->viewModelAxis[3][1];
        cgameGlob->viewModelAxis[3][2] =
            cgameGlob->flamethrowerKickOffset[axisIndex] * cgameGlob->viewModelAxis[axisIndex][2]
            + cgameGlob->viewModelAxis[3][2];
    }
}

int flame_randomseed = 12345;
double __cdecl Flame_Random(bool is_server)
{
    if ( is_server )
        return G_random();
    flame_randomseed = 214013 * flame_randomseed + 2531011;
    return (double)((flame_randomseed >> 16) & 0x7FFF) / 32768.0;
}

double __cdecl Flame_CRandom(bool is_server)
{
    return Flame_Random(is_server) * 2.0 - 1.0;
}

double __cdecl Flame_CalcTimeScale(int startTime, int endTime)
{
    if ( endTime - startTime <= 0 )
        return 0.0;
    else
        return 1.0 / (double)(endTime - startTime);
}

double __cdecl Flame_CalcInvStartSpeed(float invInitialSpeed, float speedScale)
{
    float invSpeed;
    float scaledInvSpeed;

    if (invInitialSpeed == 0.0f)
        invSpeed = 0.0f;
    else
        invSpeed = 1.0f / invInitialSpeed;
    scaledInvSpeed = speedScale * invSpeed;
    if (scaledInvSpeed == 0.0f)
        return 0.0;
    return 1.0 / scaledInvSpeed;
}

