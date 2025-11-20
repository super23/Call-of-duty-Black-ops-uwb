#include "g_missile.h"

void __cdecl G_RegisterMissileDvars()
{
  missileHellfireMaxSlope = _Dvar_RegisterFloat(
                              "missileHellfireMaxSlope",
                              0.30000001,
                              0.0,
                              3.4028235e38,
                              0x80u,
                              "This limits how steeply the hellfire missile can turn upward when climbing");
  missileHellfireUpAccel = _Dvar_RegisterFloat(
                             "missileHellfireUpAccel",
                             20.0,
                             0.1,
                             3.4028235e38,
                             0x80u,
                             "The rate at which the hellfire missile curves upward");
  missileJavClimbHeightDirect = _Dvar_RegisterFloat(
                                  "missileJavClimbHeightDirect",
                                  10000.0,
                                  0.0,
                                  3.4028235e38,
                                  0x80u,
                                  "In direct-fire mode, how far above the target the rocket will aim for when climbing.");
  missileJavClimbHeightTop = _Dvar_RegisterFloat(
                               "missileJavClimbHeightTop",
                               15000.0,
                               0.0,
                               3.4028235e38,
                               0x80u,
                               "In top-fire mode, how far above the target the rocket will aim for when climbing.");
  missileJavClimbAngleDirect = _Dvar_RegisterFloat(
                                 "missileJavClimbAngleDirect",
                                 85.0,
                                 0.0,
                                 3.4028235e38,
                                 0x80u,
                                 "In direct-fire mode, the minimum angle between the rocket and target until the rocket s"
                                 "tops climbing.  Smaller angles make for higher climbs.");
  missileJavClimbAngleTop = _Dvar_RegisterFloat(
                              "missileJavClimbAngleTop",
                              50.0,
                              0.0,
                              3.4028235e38,
                              0x80u,
                              "In top-fire mode, the minimum angle between the rocket and target until the rocket stops c"
                              "limbing.  Smaller angles make for higher climbs.");
  missileJavClimbCeilingDirect = _Dvar_RegisterFloat(
                                   "missileJavClimbCeilingDirect",
                                   0.0,
                                   0.0,
                                   3.4028235e38,
                                   0x80u,
                                   "In direct-fire mode, how high the missile needs to reach before it descends.");
  missileJavClimbCeilingTop = _Dvar_RegisterFloat(
                                "missileJavClimbCeilingTop",
                                3000.0,
                                0.0,
                                3.4028235e38,
                                0x80u,
                                "In top-fire mode, how high the missile needs to reach before it descends.");
  missileJavTurnRateDirect = _Dvar_RegisterFloat(
                               "missileJavTurnRateDirect",
                               60.0,
                               0.0,
                               3.4028235e38,
                               0x80u,
                               "In direct-fire mode, how sharp the rocket can turn, in angles/sec.");
  missileJavTurnRateTop = _Dvar_RegisterFloat(
                            "missileJavTurnRateTop",
                            100.0,
                            0.0,
                            3.4028235e38,
                            0x80u,
                            "In top-fire mode, how sharp the rocket can turn, in angles/sec.");
  missileJavAccelClimb = _Dvar_RegisterFloat(
                           "missileJavAccelClimb",
                           300.0,
                           0.0,
                           3.4028235e38,
                           0x80u,
                           "Rocket acceleration when climbing.");
  missileJavAccelDescend = _Dvar_RegisterFloat(
                             "missileJavAccelDescend",
                             3000.0,
                             0.0,
                             3.4028235e38,
                             0x80u,
                             "Rocket acceleration when descending towards target.");
  missileJavSpeedLimitClimb = _Dvar_RegisterFloat(
                                "missileJavSpeedLimitClimb",
                                1000.0,
                                0.0,
                                3.4028235e38,
                                0x80u,
                                "Rocket's speed limit when climbing.");
  missileJavSpeedLimitDescend = _Dvar_RegisterFloat(
                                  "missileJavSpeedLimitDescend",
                                  6000.0,
                                  0.0,
                                  3.4028235e38,
                                  0x80u,
                                  "Rocket's speed limit when descending towards target.");
  missileJavTurnDecel = _Dvar_RegisterFloat("missileJavTurnDecel", 0.050000001, 0.0, 1.0, 0x80u, &toastPopupTitle);
  missileJavClimbToOwner = _Dvar_RegisterFloat(
                             "missileJavClimbToOwner",
                             700.0,
                             0.0,
                             3.4028235e38,
                             0x80u,
                             &toastPopupTitle);
  missileWaterMaxDepth = _Dvar_RegisterFloat(
                           "missileWaterMaxDepth",
                           60.0,
                           0.0,
                           3.4028235e38,
                           0x80u,
                           "If a missile explodes deeper under water than this, they explosion effect/sound will not play.");
  missileTVGuidedStickDeadzone = _Dvar_RegisterInt(
                                   "missileTVGuidedStickDeadzone",
                                   15,
                                   -128,
                                   127,
                                   0x80u,
                                   "defines the dead zone for stick movement (stick values range from -128..127");
  missileTVGuidedTurnRate = _Dvar_RegisterFloat(
                              "missileTVGuidedTurnRate",
                              3.0,
                              0.0,
                              3.4028235e38,
                              0x80u,
                              "max turn rate of a tv guided missile (multiplied by the stick deflection [0-1])");
  missilePlantableSize = _Dvar_RegisterFloat(
                           "missilePlantableSize",
                           5.0,
                           0.0,
                           3.4028235e38,
                           0x80u,
                           "Size of the bounding box used for plantable equipment collision.");
  missileMolotovBlobNum = _Dvar_RegisterInt(
                            "missileMolotovBlobNum",
                            5,
                            1,
                            10,
                            0x80u,
                            "The number of fire blobs a molotov should spawn on impact, these count against the server entities.");
  missileMolotovBlobTime = _Dvar_RegisterFloat(
                             "missileMolotovBlobTime",
                             2.0,
                             0.1,
                             10.0,
                             0x80u,
                             "The life time of a molotov fire blob in seconds.");
  missileTVGuidedBoost = _Dvar_RegisterFloat(
                           "missileTVGuidedBoost",
                           2.0,
                           0.1,
                           10.0,
                           0x80u,
                           "The multiplier that the TV guided missile recieves while boosting.");
  missileTVGuidedBoostSpeedUp = _Dvar_RegisterInt(
                                  "missileTVGuidedBoostSpeedUp",
                                  1000,
                                  0,
                                  10000,
                                  0x80u,
                                  "Speed increase factor to get to full boost.");
  missileTVGuidedBoostSpeedDown = _Dvar_RegisterInt(
                                    "missileTVGuidedBoostSpeedDown",
                                    -1000,
                                    -10000,
                                    0,
                                    0x80u,
                                    "Speed decrease factor to return from full boost.");
  missileTVGuidedMaxRollAngle = _Dvar_RegisterFloat(
                                  "missileTVGuidedMaxRollAngle",
                                  35.0,
                                  0.0,
                                  3.4028235e38,
                                  0x80u,
                                  "Max roll the missile will have when turning.");
  missileTVGuidedTurningRollAccel = _Dvar_RegisterFloat(
                                      "missileTVGuidedTurningRollAccel",
                                      1.5,
                                      0.0,
                                      3.4028235e38,
                                      0x80u,
                                      "Roll acceleration when the missile is turning.");
  missileTVGuidedFlatteningRollAccel = _Dvar_RegisterFloat(
                                         "missileTVGuidedFlatteningRollAccel",
                                         2.0,
                                         0.0,
                                         3.4028235e38,
                                         0x80u,
                                         "Roll acceleration when the missile is flattening out after a turn.");
  missileTVGuidedMPSpecific = _Dvar_RegisterBool(
                                "missileTVGuidedMPSpecific",
                                1,
                                0x80u,
                                "MP specific Guided Missile functionality");
  grenadeRestThreshold = _Dvar_RegisterFloat(
                           "grenadeRestThreshold",
                           20.0,
                           0.0,
                           3.4028235e38,
                           0x80u,
                           "The speed threshold below which grenades will come to rest");
  grenadeRollingEnabled = _Dvar_RegisterBool(
                            "grenadeRollingEnabled",
                            1,
                            0x1080u,
                            "Enables the new \"rolling\" grenade behavior");
  grenadeFrictionLow = _Dvar_RegisterFloat(
                         "grenadeFrictionLow",
                         0.0099999998,
                         0.0,
                         1.0,
                         0x80u,
                         "The amount of friction (0 to 1) for slower/rolling grenades");
  grenadeFrictionHigh = _Dvar_RegisterFloat(
                          "grenadeFrictionHigh",
                          0.40000001,
                          0.0,
                          1.0,
                          0x80u,
                          "The amount of friction (0 to 1) for fast-moving grenades");
  grenadeFrictionThresh = _Dvar_RegisterFloat(
                            "grenadeFrictionMaxThresh",
                            100.0,
                            0.0,
                            3.4028235e38,
                            0x80u,
                            "The speed threshold that determines whether to use grenadeFrictionLow/High");
  grenadeBumpFreq = _Dvar_RegisterFloat(
                      "grenadeBumpFreq",
                      0.30000001,
                      0.0,
                      3.4028235e38,
                      0x80u,
                      "How likely (per server frame) a bump will occur");
  grenadeBumpMag = _Dvar_RegisterFloat(
                     "grenadeBumpMag",
                     0.40000001,
                     0.0,
                     3.4028235e38,
                     0x80u,
                     "Size of bumps (as a fraction of the grenade's current speed)");
  grenadeBumpMax = _Dvar_RegisterFloat(
                     "grenadeBumpMax",
                     100.0,
                     0.0,
                     3.4028235e38,
                     0x80u,
                     "Maximum upward speed of a bump (inches/sec)");
  grenadeWobbleFreq = _Dvar_RegisterFloat(
                        "grenadeWobbleFreq",
                        0.079999998,
                        0.0,
                        3.4028235e38,
                        0x80u,
                        "Wobble cycles per inch of rolling distance (approx)");
  grenadeWobbleFwdMag = _Dvar_RegisterFloat(
                          "grenadeWobbleFwdMag",
                          10.0,
                          0.0,
                          3.4028235e38,
                          0x80u,
                          "The forward rolling speed will oscillate +/- this amount");
  grenadeWobbleSideMag = _Dvar_RegisterFloat(
                           "grenadeWobbleSideMag",
                           1.0,
                           0.0,
                           3.4028235e38,
                           0x80u,
                           "The distance to wobble left and right");
  grenadeWobbleSideDamp = _Dvar_RegisterFloat(
                            "grenadeWobbleSideDamp",
                            0.0099999998,
                            0.0,
                            3.4028235e38,
                            0x80u,
                            "The rate at which the amount of side-to-side wobbling decreases as overall grenade speed increases");
  grenadeCurveMax = _Dvar_RegisterFloat(
                      "grenadeCurveMax",
                      4.0,
                      0.0,
                      3.4028235e38,
                      0x80u,
                      "Largest rolling curvature (will be random between +/- this value)");
  grenadeBounceRestitutionMax = _Dvar_RegisterFloat(
                                  "grenadeBounceRestitutionMax",
                                  0.30000001,
                                  0.0,
                                  3.4028235e38,
                                  0x80u,
                                  "Cap to keep code from increasing bounce restitution too high.");
}

void __cdecl G_RegisterMissileDebugDvars()
{
  missileDebugDraw = _Dvar_RegisterBool("missileDebugDraw", 0, 0, "Draw guided missile trajectories.");
  missileDebugText = _Dvar_RegisterBool("missileDebugText", 0, 0, "Print debug missile info to console.");
  missileDebugAttractors = _Dvar_RegisterBool(
                             "missileDebugAttractors",
                             0,
                             0,
                             "Draw the attractors and repulsors.  Attractors are green, and repulsors are yellow.");
}

bool __cdecl collide_missle_entity_filter(int entnum)
{
  return *(__int16 *)(sv.bpsWindow[8] + entnum * sv.bpsWindow[9] + 190) != 4;
}

void __cdecl G_MissileTrace(
        trace_t *results,
        const float *start,
        const float *end,
        int passEntityNum,
        int contentmask,
        unsigned int weapon)
{
  float plantableSize; // [esp+Ch] [ebp-54h]
  col_context_t context; // [esp+10h] [ebp-50h] BYREF
  float mins[3]; // [esp+38h] [ebp-28h] BYREF
  float maxs[3]; // [esp+44h] [ebp-1Ch] BYREF
  float dir[3]; // [esp+50h] [ebp-10h] BYREF
  const WeaponDef *weapDef; // [esp+5Ch] [ebp-4h]

  weapDef = BG_GetWeaponDef(weapon);
  if ( !weapDef && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 420, 0, "%s", "weapDef") )
    __debugbreak();
  if ( weapDef->plantable )
  {
    plantableSize = missilePlantableSize->current.value;
    col_context_t::col_context_t(&context);
    LODWORD(mins[0]) = LODWORD(plantableSize) ^ _mask__NegFloat_;
    LODWORD(mins[1]) = LODWORD(plantableSize) ^ _mask__NegFloat_;
    mins[2] = *(float *)&FLOAT_0_0;
    maxs[0] = plantableSize;
    maxs[1] = plantableSize;
    maxs[2] = plantableSize * 2.0;
    G_TraceCapsule(results, start, mins, maxs, end, passEntityNum, contentmask, &context);
  }
  else
  {
    G_LocationalTrace(
      results,
      start,
      end,
      passEntityNum,
      contentmask,
      bulletPriorityMap,
      (int (__cdecl *)(int, col_context_t *))collide_missle_entity_filter);
  }
  if ( results->startsolid )
  {
    results->fraction = *(float *)&FLOAT_0_0;
    dir[0] = *start - *end;
    dir[1] = start[1] - end[1];
    dir[2] = start[2] - end[2];
    Vec3NormalizeTo(dir, results->normal.vec.v);
  }
}

void __cdecl G_TimedObjectThink(gentity_s *ent)
{
  ent->s.lerp.eFlags &= ~0x10u;
}

// local variable allocation has failed, the output may be wrong!
void  G_ExplodeMissile(cStaticModel_s *a1@<ebp>, gentity_s *ent)
{
  char *v2; // eax
  unsigned __int8 v3; // al
  unsigned __int8 v4; // al
  unsigned __int8 v5; // al
  unsigned __int8 v6; // al
  unsigned __int8 v7; // al
  long double v8; // [esp+20h] [ebp-1ACh]
  gentity_s *v9; // [esp+24h] [ebp-1A8h]
  gentity_s *v10; // [esp+28h] [ebp-1A4h]
  gentity_s *v11; // [esp+2Ch] [ebp-1A0h]
  gentity_s *v12; // [esp+30h] [ebp-19Ch]
  float v13[3]; // [esp+34h] [ebp-198h] BYREF
  int SplashMethodOfDeath; // [esp+40h] [ebp-18Ch]
  float v15[2]; // [esp+44h] [ebp-188h] BYREF
  float forwardDir[3]; // [esp+50h] [ebp-17Ch]
  float coneCos; // [esp+5Ch] [ebp-170h]
  gentity_s *v18; // [esp+60h] [ebp-16Ch]
  int *v19; // [esp+64h] [ebp-168h]
  float v20; // [esp+68h] [ebp-164h] BYREF
  float v21; // [esp+6Ch] [ebp-160h]
  float v22; // [esp+70h] [ebp-15Ch]
  float endpos[4]; // [esp+74h] [ebp-158h] BYREF
  __int128 impact_normal; // [esp+84h] [ebp-148h] OVERLAPPED BYREF
  trace_t tr; // [esp+94h] [ebp-138h] BYREF
  col_context_t explosionPos; // [esp+D0h] [ebp-FCh] BYREF
  float *currentOrigin; // [esp+F8h] [ebp-D4h]
  int *p_clipAmmoCount; // [esp+FCh] [ebp-D0h]
  float *v29; // [esp+100h] [ebp-CCh]
  const float *v30; // [esp+104h] [ebp-C8h]
  gentity_s *v31; // [esp+108h] [ebp-C4h]
  unsigned __int8 surfType; // [esp+10Fh] [ebp-BDh]
  const float *normal; // [esp+110h] [ebp-BCh] BYREF
  float waterSurfacePos[3]; // [esp+11Ch] [ebp-B0h] BYREF
  float waterNormal[3]; // [esp+128h] [ebp-A4h] BYREF
  col_context_t context; // [esp+134h] [ebp-98h] BYREF
  float end[3]; // [esp+15Ch] [ebp-70h]
  float v38; // [esp+168h] [ebp-64h]
  bool inWater; // [esp+16Fh] [ebp-5Dh]
  float v40; // [esp+170h] [ebp-5Ch] BYREF
  float waterHeight; // [esp+174h] [ebp-58h]
  float v42; // [esp+178h] [ebp-54h] OVERLAPPED
  float origin[3]; // [esp+17Ch] [ebp-50h] BYREF
  const WeaponDef *weapDef; // [esp+188h] [ebp-44h]
  gentity_s *other; // [esp+18Ch] [ebp-40h]
  trace_t trace; // [esp+190h] [ebp-3Ch] BYREF
  int retaddr; // [esp+1CCh] [ebp+0h]

  trace.staticModel = a1;
  trace.hitPartition = retaddr;
  origin[2] = *(float *)&FLOAT_0_0;
  weapDef = *(const WeaponDef **)&FLOAT_0_0;
  other = *(gentity_s **)&FLOAT_0_0;
  trace.normal.vec.u[0] = *(unsigned int *)&FLOAT_0_0;
  LODWORD(origin[1]) = ent;
  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 1751, 0, "%s", "ent") )
    __debugbreak();
  if ( !ent->s.weapon
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 1752, 0, "%s", "ent->s.weapon") )
  {
    __debugbreak();
  }
  LODWORD(origin[0]) = BG_GetWeaponDef(ent->s.weapon);
  if ( !LODWORD(origin[0])
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 1754, 0, "%s", "weapDef") )
  {
    __debugbreak();
  }
  if ( *(unsigned int *)(LODWORD(origin[0]) + 1640) == 6 )
    G_UnlinkPlayerToRocket(ent);
  if ( *(_BYTE *)(LODWORD(origin[0]) + 1387) && ent->s.groundEntityNum == 1023 || ForcedDud(ent) )
  {
    if ( level.time - ent->item[0].index <= 60000 )
    {
      ent->nextthink = 50;
    }
    else
    {
      Scr_Notify(ent, scr_const.death, 0);
      G_FreeEntity(ent);
    }
  }
  else
  {
    BG_EvaluateTrajectory(&ent->s.lerp.pos, level.time, &v40);
    v40 = (float)(int)v40;
    waterHeight = (float)(int)waterHeight;
    v42 = (float)(int)v42;
    if ( BG_ValidateOrigin(&v40, 16, 17, svs.mapCenter) )
    {
      G_SetOrigin(ent, &v40);
      inWater = 1;
      v38 = CM_GetWaterHeight(ent->r.currentOrigin, 1000.0, -200.0);
      LODWORD(end[2]) = v38 >= ent->r.currentOrigin[2];
      HIBYTE(end[1]) = LOBYTE(end[2]);
      if ( !LOBYTE(end[2]) )
        HIBYTE(end[1]) = SV_PointContents(ent->r.currentOrigin, -1, 32) != 0;
      if ( HIBYTE(end[1]) )
      {
        LODWORD(end[0]) = ent->r.currentOrigin;
        context.locational = LODWORD(ent->r.currentOrigin[0]);
        context.priorityMap = (unsigned __int8 *)LODWORD(ent->r.currentOrigin[1]);
        context.collide_entity_func = (int (__cdecl *)(int, col_context_t *))LODWORD(ent->r.currentOrigin[2]);
        *(float *)&context.collide_entity_func = *(float *)&context.collide_entity_func
                                               + missileWaterMaxDepth->current.value;
        col_context_t::col_context_t((col_context_t *)waterNormal);
        G_TraceCapsule(
          (trace_t *)&origin[2],
          (const float *)&context.locational,
          vec3_origin,
          vec3_origin,
          ent->r.currentOrigin,
          ent->s.number,
          32,
          (col_context_t *)waterNormal);
        if ( HIBYTE(trace.hitType) || trace.normal.vec.v[1] >= 1.0 )
        {
          inWater = 0;
        }
        else
        {
          waterSurfacePos[0] = origin[2];
          LODWORD(waterSurfacePos[1]) = weapDef;
          LODWORD(waterSurfacePos[2]) = other;
          Vec3Lerp((const float *)&context.locational, ent->r.currentOrigin, trace.normal.vec.v[1], (float *)&normal);
        }
      }
      ent->s.lerp.eFlags |= 0x20u;
      if ( HIBYTE(end[1]) )
        surfType = 20;
      else
        surfType = ent->s.surfType;
      v2 = (char *)Com_SurfaceTypeToName(surfType);
      Scr_AddString(v2, SCRIPTINSTANCE_SERVER);
      Scr_AddVector(ent->r.currentOrigin, SCRIPTINSTANCE_SERVER);
      Scr_Notify(ent, scr_const.explode, 2u);
      v31 = 0;
      if ( inWater )
      {
        v31 = G_Spawn();
        v31->s.eType = 0;
        v31->s.lerp.eFlags |= 0x20u;
        v31->s.weapon = ent->s.weapon;
        v31->s.weaponModel = ent->s.weaponModel;
        v31->r.contents = ent->r.contents;
        G_BroadcastEntity(v31);
        if ( HIBYTE(end[1]) )
        {
          G_SetOrigin(v31, (const float *)&normal);
          v31->s.surfType = 20;
          v30 = waterSurfacePos;
        }
        else
        {
          G_SetOrigin(v31, ent->r.currentOrigin);
          if ( (*(unsigned int *)(LODWORD(origin[0]) + 1584) == 1 || *(unsigned int *)(LODWORD(origin[0]) + 1584) == 2)
            && ent->s.groundEntityNum != 1023 )
          {
            p_clipAmmoCount = &ent->item[1].clipAmmoCount;
            currentOrigin = ent->r.currentOrigin;
            *(float *)&context.locational = (float)(-16.0 * ent->mover.midTime) + ent->r.currentOrigin[0];
            *(float *)&context.priorityMap = (float)(-16.0 * ent->mover.aMidTime) + ent->r.currentOrigin[1];
            *(float *)&context.collide_entity_func = (float)(-16.0 * ent->trigger.exposureLerpToLighter)
                                                   + ent->r.currentOrigin[2];
          }
          else
          {
            v29 = ent->r.currentOrigin;
            context.locational = LODWORD(ent->r.currentOrigin[0]);
            context.priorityMap = (unsigned __int8 *)LODWORD(ent->r.currentOrigin[1]);
            context.collide_entity_func = (int (__cdecl *)(int, col_context_t *))LODWORD(ent->r.currentOrigin[2]);
            *(float *)&context.collide_entity_func = *(float *)&context.collide_entity_func - 16.0;
          }
          col_context_t::col_context_t(&explosionPos);
          G_TraceCapsule(
            (trace_t *)&origin[2],
            ent->r.currentOrigin,
            vec3_origin,
            vec3_origin,
            (const float *)&context.locational,
            ent->s.number,
            2065,
            &explosionPos);
          if ( *(_BYTE *)(LODWORD(origin[0]) + 1524) )
            v30 = up;
          else
            v30 = &origin[2];
          v31->s.surfType = (int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20] & trace.normal.vec.u[2]) >> 20;
        }
        if ( *(unsigned int *)(LODWORD(origin[0]) + 1516) && *(unsigned int *)(LODWORD(origin[0]) + 1516) != 6 )
        {
          switch ( *(unsigned int *)(LODWORD(origin[0]) + 1516) )
          {
            case 1:
              v4 = DirToByte(v30);
              G_AddEvent(v31, 0x3Au, v4);
              break;
            case 2:
              v5 = DirToByte(v30);
              G_AddEvent(v31, 0x3Cu, v5);
              v31->s.lerp.u.actor.index.actorNum = level.time;
              break;
            case 7:
              v6 = DirToByte(v30);
              G_AddEvent(v31, 0x42u, v6);
              break;
            default:
              v7 = DirToByte(v30);
              G_AddEvent(v31, 0x3Du, v7);
              v31->s.lerp.u.actor.index.actorNum = level.time;
              break;
          }
        }
        else
        {
          if ( *(_WORD *)(LODWORD(origin[0]) + 884)
            && G_DObjGetWorldTagPos(ent, *(unsigned __int16 *)(LODWORD(origin[0]) + 884), (float *)&tr.staticModel) )
          {
            G_SetOrigin(v31, (const float *)&tr.staticModel);
          }
          v3 = DirToByte(v30);
          G_AddEvent(v31, 0x39u, v3);
        }
        if ( (*(unsigned int *)(LODWORD(origin[0]) + 1516) == 5 || *(unsigned int *)(LODWORD(origin[0]) + 1516) == 2)
          && *(unsigned int *)(LODWORD(origin[0]) + 1520) )
        {
          v31->s.lerp.pos.trBase[0] = (float)(int)v31->s.lerp.pos.trBase[0];
          v31->s.lerp.pos.trBase[1] = (float)(int)v31->s.lerp.pos.trBase[1];
          v31->s.lerp.pos.trBase[2] = (float)(int)v31->s.lerp.pos.trBase[2];
          G_SetOrigin(v31, v31->s.lerp.pos.trBase);
          v31->s.lerp.eFlags |= 0x4000u;
          v31->s.lerp.u.actor.index.actorNum = level.time;
          v31->s.lerp.u.loopFx.period = ent->missile.grenade.effectIndex;
          v31->s.time2 = level.time + 61000;
          v31->s.lerp.eFlags |= 0x10u;
          v31->handler = 11;
          v31->nextthink = level.time + 1;
          Com_Printf(
            15,
            "Sending smoke grenade that starts at %i and is at ( %f, %f, %f )\n",
            level.time,
            v31->s.lerp.pos.trBase[0],
            v31->s.lerp.pos.trBase[1],
            v31->s.lerp.pos.trBase[2]);
        }
        else
        {
          G_FreeEntityAfterEvent(v31);
        }
      }
      if ( ent->s.groundEntityNum != 1023
        && ent->s.groundEntityNum != 1022
        && (*(unsigned int *)(LODWORD(origin[0]) + 1584) == 1 || *(unsigned int *)(LODWORD(origin[0]) + 1584) == 2) )
      {
        *(unsigned int *)&tr.walkable = &g_entities[ent->s.groundEntityNum];
        if ( *(unsigned int *)(*(unsigned int *)&tr.walkable + 344) )
        {
          memset((char *)&impact_normal + 4, 0, 12);
          tr.normal.vec.u[0] = *(unsigned int *)&FLOAT_0_0;
          LODWORD(impact_normal) = &ent->item[1].clipAmmoCount;
          LODWORD(endpos[1]) = ent->item[1].clipAmmoCount ^ _mask__NegFloat_;
          LODWORD(endpos[2]) = ent->item[1].index ^ _mask__NegFloat_;
          LODWORD(endpos[3]) = LODWORD(ent->trigger.exposureLerpToLighter) ^ _mask__NegFloat_;
          LODWORD(endpos[0]) = ent->r.currentOrigin;
          v20 = ent->r.currentOrigin[0];
          v21 = ent->r.currentOrigin[1];
          v22 = ent->r.currentOrigin[2];
          v19 = &ent->item[1].clipAmmoCount;
          v20 = (float)(-1.635 * ent->mover.midTime) + v20;
          v21 = (float)(-1.635 * ent->mover.aMidTime) + v21;
          v22 = (float)(-1.635 * ent->trigger.exposureLerpToLighter) + v22;
          G_MissileTrace(
            (trace_t *)((char *)&impact_normal + 4),
            ent->r.currentOrigin,
            &v20,
            ent->s.number,
            ent->clipmask,
            ent->s.weapon);
          Vec3Lerp(ent->r.currentOrigin, &v20, tr.normal.vec.v[1], &v20);
          if ( EntHandle::isDefined(&ent->parent) )
            v18 = EntHandle::ent(&ent->parent);
          else
            v18 = 0;
          G_Damage(
            *(gentity_s **)&tr.walkable,
            ent,
            v18,
            &endpos[1],
            &v20,
            *(unsigned int *)(LODWORD(origin[0]) + 1468),
            0,
            dword_E07CF8[12 * ent->handler],
            ent->s.weapon,
            HITLOC_NONE,
            HIWORD(tr.sflags),
            LOWORD(tr.cflags),
            0);
          origin[1] = *(float *)&tr.walkable;
        }
      }
      if ( *(unsigned int *)(LODWORD(origin[0]) + 1468) )
      {
        coneCos = *(float *)(LODWORD(origin[0]) + 1476) * 0.017453292;
        __libm_sse2_cos(v8);
        forwardDir[2] = coneCos;
        if ( (float)(*(float *)(LODWORD(origin[0]) + 1476) - 180.0) < 0.0 )
          forwardDir[1] = forwardDir[2];
        else
          forwardDir[1] = FLOAT_N1_0;
        forwardDir[0] = forwardDir[1];
        AngleVectors(ent->r.currentAngles, v15, 0, 0);
        SplashMethodOfDeath = GetSplashMethodOfDeath(ent);
        if ( *(_WORD *)(LODWORD(origin[0]) + 884) )
        {
          if ( G_DObjGetWorldTagPos(ent, *(unsigned __int16 *)(LODWORD(origin[0]) + 884), v13) )
          {
            if ( EntHandle::isDefined(&ent->parent) )
              v12 = EntHandle::ent(&ent->parent);
            else
              v12 = 0;
            G_RadiusDamage(
              v13,
              ent,
              v12,
              (float)*(int *)(LODWORD(origin[0]) + 1468),
              (float)*(int *)(LODWORD(origin[0]) + 1472),
              (float)*(int *)(LODWORD(origin[0]) + 1456),
              forwardDir[0],
              v15,
              (gentity_s *)LODWORD(origin[1]),
              SplashMethodOfDeath,
              ent->s.weapon);
          }
          else
          {
            if ( EntHandle::isDefined(&ent->parent) )
              v11 = EntHandle::ent(&ent->parent);
            else
              v11 = 0;
            G_RadiusDamage(
              ent->r.currentOrigin,
              ent,
              v11,
              (float)*(int *)(LODWORD(origin[0]) + 1468),
              (float)*(int *)(LODWORD(origin[0]) + 1472),
              (float)*(int *)(LODWORD(origin[0]) + 1456),
              forwardDir[0],
              v15,
              (gentity_s *)LODWORD(origin[1]),
              SplashMethodOfDeath,
              ent->s.weapon);
          }
        }
        else
        {
          if ( EntHandle::isDefined(&ent->parent) )
            v10 = EntHandle::ent(&ent->parent);
          else
            v10 = 0;
          G_RadiusDamage(
            ent->r.currentOrigin,
            ent,
            v10,
            (float)*(int *)(LODWORD(origin[0]) + 1468),
            (float)*(int *)(LODWORD(origin[0]) + 1472),
            (float)*(int *)(LODWORD(origin[0]) + 1456),
            forwardDir[0],
            v15,
            (gentity_s *)LODWORD(origin[1]),
            SplashMethodOfDeath,
            ent->s.weapon);
        }
      }
      if ( *(unsigned int *)(LODWORD(origin[0]) + 1516) == 2 )
      {
        if ( EntHandle::isDefined(&ent->parent) )
          v9 = EntHandle::ent(&ent->parent);
        else
          v9 = 0;
        G_FlashbangBlast(
          ent->r.currentOrigin,
          (float)*(int *)(LODWORD(origin[0]) + 1456),
          (float)*(int *)(LODWORD(origin[0]) + 1460),
          v9,
          ent->missile.team);
      }
      if ( inWater )
      {
        if ( !v31
          && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 2013, 1, "%s", "eventEnt") )
        {
          __debugbreak();
        }
        SV_LinkEntity((int)&trace.staticModel, v31);
      }
      Scr_Notify(ent, scr_const.death, 0);
      G_FreeEntity(ent);
    }
    else
    {
      if ( *(unsigned int *)(LODWORD(origin[0]) + 1640) == 6 )
        G_UnlinkPlayerToRocket(ent);
      Scr_Notify(ent, scr_const.death, 0);
      G_FreeEntity(ent);
    }
  }
}

int __cdecl ForcedDud(gentity_s *ent)
{
  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 329, 0, "%s", "ent") )
    __debugbreak();
  return ent->s.lerp.u.turret.ownerNum;
}

int __cdecl GetSplashMethodOfDeath(gentity_s *ent)
{
  const WeaponDef *weapDef; // [esp+4h] [ebp-4h]

  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 801, 0, "%s", "ent") )
    __debugbreak();
  if ( !ent->s.weapon
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 802, 0, "%s", "ent->s.weapon") )
  {
    __debugbreak();
  }
  weapDef = BG_GetWeaponDef(ent->s.weapon);
  if ( !weapDef && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 805, 0, "%s", "weapDef") )
    __debugbreak();
  if ( dword_E07CFC[12 * ent->handler] == 4 && weapDef->projExplosion == WEAPPROJEXP_HEAVY )
    return 15;
  else
    return dword_E07CFC[12 * ent->handler];
}

void __cdecl G_UnlinkPlayerToRocket(gentity_s *ent)
{
  gclient_s *client; // [esp+0h] [ebp-4h]

  if ( EntHandle::isDefined(&ent->parent) )
  {
    client = EntHandle::ent(&ent->parent)->client;
    if ( !client && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 858, 0, "%s", "client") )
      __debugbreak();
    if ( (client->ps.eFlags & 0x300) == 0 )
      client->ps.viewlocked_entNum = 1023;
    client->ps.weapFlags &= ~0x200u;
  }
}

void __cdecl Missile_InitAttractors()
{
  memset((unsigned __int8 *)&attrGlob, 0, sizeof(attrGlob));
}

void __cdecl Missile_FreeAttractorRefs(gentity_s *ent)
{
  AttractorRepulsor_t *v1; // ecx
  unsigned int attractorIndex; // [esp+0h] [ebp-4h]

  for ( attractorIndex = 0; attractorIndex < 0x20; ++attractorIndex )
  {
    if ( attrGlob.attractors[attractorIndex].inUse && attrGlob.attractors[attractorIndex].entnum == ent->s.number )
    {
      v1 = &attrGlob.attractors[attractorIndex];
      *(unsigned int *)&v1->inUse = 0;
      v1->entnum = 0;
      v1->origin[0] = 0.0;
      v1->origin[1] = 0.0;
      v1->origin[2] = 0.0;
      v1->strength = 0.0;
      v1->maxDist = 0.0;
    }
  }
}

void __cdecl Scr_MissileCreateAttractorEnt()
{
  unsigned int attractorIndex; // [esp+0h] [ebp-8h]
  gentity_s *ent; // [esp+4h] [ebp-4h]

  attractorIndex = Missile_GetFreeAttractor();
  attrGlob.attractors[attractorIndex].isAttractor = 1;
  ent = Scr_GetEntity(0);
  attrGlob.attractors[attractorIndex].strength = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
  attrGlob.attractors[attractorIndex].maxDist = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
  if ( attrGlob.attractors[attractorIndex].maxDist <= 0.0 )
    Scr_ParamError(2u, "maxDist must be greater than zero", SCRIPTINSTANCE_SERVER);
  attrGlob.attractors[attractorIndex].entnum = ent->s.number;
  ent->flags |= (unsigned int)&loc_800000;
  attrGlob.attractors[attractorIndex].inUse = 1;
  Scr_AddInt(attractorIndex, SCRIPTINSTANCE_SERVER);
}

unsigned int __cdecl Missile_GetFreeAttractor()
{
  const char *v0; // eax
  unsigned int attractorIndex; // [esp+0h] [ebp-4h]

  for ( attractorIndex = 0; attractorIndex < 0x20 && attrGlob.attractors[attractorIndex].inUse; ++attractorIndex )
    ;
  if ( attractorIndex == 32 )
  {
    v0 = va("Ran out of attractor/repulsors.  Max allowed: %i", 32);
    Scr_Error(v0, 0);
  }
  return attractorIndex;
}

void __cdecl Scr_MissileCreateAttractorOrigin()
{
  unsigned int attractorIndex; // [esp+0h] [ebp-4h]

  attractorIndex = Missile_GetFreeAttractor();
  attrGlob.attractors[attractorIndex].isAttractor = 1;
  attrGlob.attractors[attractorIndex].entnum = 1023;
  Scr_GetVector(0, (float *)(28 * attractorIndex + 65863520), SCRIPTINSTANCE_SERVER);
  attrGlob.attractors[attractorIndex].strength = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
  attrGlob.attractors[attractorIndex].maxDist = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
  if ( attrGlob.attractors[attractorIndex].maxDist <= 0.0 )
    Scr_ParamError(2u, "maxDist must be greater than zero", SCRIPTINSTANCE_SERVER);
  attrGlob.attractors[attractorIndex].inUse = 1;
  Scr_AddInt(attractorIndex, SCRIPTINSTANCE_SERVER);
}

void __cdecl Scr_MissileCreateRepulsorEnt()
{
  unsigned int attractorIndex; // [esp+0h] [ebp-4h]

  attractorIndex = Missile_GetFreeAttractor();
  attrGlob.attractors[attractorIndex].isAttractor = 0;
  attrGlob.attractors[attractorIndex].entnum = LOWORD(Scr_GetEntity(0)->s.number);
  attrGlob.attractors[attractorIndex].strength = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
  attrGlob.attractors[attractorIndex].maxDist = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
  if ( attrGlob.attractors[attractorIndex].maxDist <= 0.0 )
    Scr_ParamError(2u, "maxDist must be greater than zero", SCRIPTINSTANCE_SERVER);
  attrGlob.attractors[attractorIndex].inUse = 1;
  Scr_AddInt(attractorIndex, SCRIPTINSTANCE_SERVER);
}

void __cdecl Scr_MissileCreateRepulsorOrigin()
{
  unsigned int attractorIndex; // [esp+0h] [ebp-4h]

  attractorIndex = Missile_GetFreeAttractor();
  attrGlob.attractors[attractorIndex].isAttractor = 0;
  attrGlob.attractors[attractorIndex].entnum = 1023;
  Scr_GetVector(0, (float *)(28 * attractorIndex + 65863520), SCRIPTINSTANCE_SERVER);
  attrGlob.attractors[attractorIndex].strength = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
  attrGlob.attractors[attractorIndex].maxDist = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
  if ( attrGlob.attractors[attractorIndex].maxDist <= 0.0 )
    Scr_ParamError(2u, "maxDist must be greater than zero", SCRIPTINSTANCE_SERVER);
  attrGlob.attractors[attractorIndex].inUse = 1;
  Scr_AddInt(attractorIndex, SCRIPTINSTANCE_SERVER);
}

void __cdecl Scr_MissileDeleteAttractor()
{
  AttractorRepulsor_t *v0; // ecx
  unsigned int attractorIndex; // [esp+0h] [ebp-4h]

  attractorIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER).stringValue;
  if ( attractorIndex >= 0x20 )
    Scr_ParamError(0, "Invalid attractor or repulsor", SCRIPTINSTANCE_SERVER);
  v0 = &attrGlob.attractors[attractorIndex];
  *(unsigned int *)&v0->inUse = 0;
  v0->entnum = 0;
  v0->origin[0] = 0.0;
  v0->origin[1] = 0.0;
  v0->origin[2] = 0.0;
  v0->strength = 0.0;
  v0->maxDist = 0.0;
}

void __cdecl G_MakeMissilePickupItem(gentity_s *ent)
{
  const gitem_s *item; // [esp+8h] [ebp-8h]
  int itemIndex; // [esp+Ch] [ebp-4h]

  ent->r.mins[0] = FLOAT_N1_0;
  ent->r.mins[1] = FLOAT_N1_0;
  ent->r.mins[2] = FLOAT_N1_0;
  ent->r.maxs[0] = FLOAT_1_0;
  ent->r.maxs[1] = FLOAT_1_0;
  ent->r.maxs[2] = FLOAT_1_0;
  ent->r.contents |= 0x200000u;
  item = BG_FindItemForWeapon(ent->s.weapon);
  if ( !item && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 3389, 0, "%s", "item") )
    __debugbreak();
  itemIndex = ((char *)item - (char *)bg_itemlist) >> 2;
  ent->s.un3.item = (unsigned __int16)itemIndex;
  if ( ent->s.un3.item >= 0x800u
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          3394,
          0,
          "ent->s.un3.item doesn't index MAX_ITEMS\n\t%i not in [0, %i)",
          ent->s.un3.item,
          2048) )
  {
    __debugbreak();
  }
  if ( ent->s.un3.item != itemIndex
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          3395,
          0,
          "%s",
          "ent->s.un3.item == itemIndex") )
  {
    __debugbreak();
  }
  if ( item->giType != IT_WEAPON
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          3396,
          0,
          "%s",
          "item->giType == IT_WEAPON") )
  {
    __debugbreak();
  }
  ent->s.clientNum = 32;
}

void __cdecl G_RunMissileInternal(gentity_s *ent)
{
  const float *v1; // [esp+1Ch] [ebp-1C4h]
  float circleDir3[3]; // [esp+A8h] [ebp-138h] BYREF
  float center; // [esp+B4h] [ebp-12Ch] BYREF
  float v4; // [esp+B8h] [ebp-128h]
  float v5; // [esp+BCh] [ebp-124h]
  float radius; // [esp+C0h] [ebp-120h]
  float circleDir2[3]; // [esp+C4h] [ebp-11Ch] BYREF
  float circleDir1[3]; // [esp+D0h] [ebp-110h] BYREF
  const float *color; // [esp+DCh] [ebp-104h]
  unsigned int attractorIndex; // [esp+E0h] [ebp-100h]
  float tracePosSmall[3]; // [esp+E4h] [ebp-FCh] BYREF
  float tracePosLarge[3]; // [esp+F0h] [ebp-F0h] BYREF
  float travel_distance; // [esp+FCh] [ebp-E4h]
  float delta; // [esp+100h] [ebp-E0h]
  float originOffset[3]; // [esp+104h] [ebp-DCh]
  float traceStart[3]; // [esp+110h] [ebp-D0h] BYREF
  int clipMask; // [esp+11Ch] [ebp-C4h]
  float velocity[3]; // [esp+120h] [ebp-C0h] BYREF
  float speed; // [esp+12Ch] [ebp-B4h]
  float vOldOrigin[3]; // [esp+130h] [ebp-B0h] BYREF
  float dir[3]; // [esp+13Ch] [ebp-A4h] BYREF
  float origin[3]; // [esp+148h] [ebp-98h] BYREF
  gentity_s *owner; // [esp+154h] [ebp-8Ch]
  float endpos[3]; // [esp+158h] [ebp-88h] BYREF
  trace_t tr; // [esp+164h] [ebp-7Ch] BYREF
  trace_t trDown; // [esp+19Ch] [ebp-44h] BYREF
  int passEntNum; // [esp+1D8h] [ebp-8h]
  const WeaponDef *weapDef; // [esp+1DCh] [ebp-4h]
  int savedregs; // [esp+1E0h] [ebp+0h] BYREF

  memset(&tr, 0, 16);
  memset(&trDown, 0, 16);
  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 3531, 0, "%s", "ent") )
    __debugbreak();
  if ( ent->s.eType != 4
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          3532,
          0,
          "%s",
          "ent->s.eType == ET_MISSILE") )
  {
    __debugbreak();
  }
  weapDef = BG_GetWeaponDef(ent->s.weapon);
  if ( !weapDef && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 3535, 0, "%s", "weapDef") )
    __debugbreak();
  passEntNum = 1023;
  if ( EntHandle::isDefined(&ent->r.ownerNum) )
  {
    owner = EntHandle::ent(&ent->r.ownerNum);
    if ( owner->tagInfo && owner->client && (owner->client->ps.eFlags & 0x4000) != 0 )
      passEntNum = owner->tagInfo->parent->s.number;
    else
      passEntNum = EntHandle::entnum(&ent->r.ownerNum);
  }
  RunMissile_BroadcastActorEvents(ent);
  if ( weapDef->stickiness != WEAPSTICKINESS_ALL_NO_SENTIENTS
    || ent->s.lerp.pos.trType
    || ent->s.groundEntityNum == 1022 )
  {
    if ( weapDef->isRollingGrenade && ent->s.lerp.pos.trType == 1 )
    {
      ent->s.lerp.pos.trType = G_GetGrenadeTrType(ent);
      ent->s.lerp.pos.trBase[0] = ent->r.currentOrigin[0];
      ent->s.lerp.pos.trBase[1] = ent->r.currentOrigin[1];
      ent->s.lerp.pos.trBase[2] = ent->r.currentOrigin[2];
    }
  }
  else
  {
    origin[0] = ent->r.currentOrigin[0];
    origin[1] = ent->r.currentOrigin[1];
    origin[2] = ent->r.currentOrigin[2];
    origin[0] = (float)(-1.635 * ent->mover.midTime) + origin[0];
    origin[1] = (float)(-1.635 * ent->mover.aMidTime) + origin[1];
    origin[2] = (float)(-1.635 * ent->trigger.exposureLerpToLighter) + origin[2];
    G_MissileTrace(&tr, ent->r.currentOrigin, origin, passEntNum, ent->clipmask, ent->s.weapon);
    if ( tr.fraction == 1.0 )
    {
      ent->s.lerp.pos.trType = 6;
      ent->s.lerp.pos.trTime = level.time;
      ent->s.lerp.pos.trDuration = 0;
      ent->s.lerp.pos.trBase[0] = ent->r.currentOrigin[0];
      ent->s.lerp.pos.trBase[1] = ent->r.currentOrigin[1];
      ent->s.lerp.pos.trBase[2] = ent->r.currentOrigin[2];
      ent->s.lerp.pos.trDelta[0] = *(float *)&FLOAT_0_0;
      ent->s.lerp.pos.trDelta[1] = *(float *)&FLOAT_0_0;
      ent->s.lerp.pos.trDelta[2] = *(float *)&FLOAT_0_0;
    }
  }
  vOldOrigin[0] = ent->r.currentOrigin[0];
  vOldOrigin[1] = ent->r.currentOrigin[1];
  vOldOrigin[2] = ent->r.currentOrigin[2];
  MissileTrajectory(ent, origin);
  if ( weapDef->guidedMissileType == MISSILE_GUIDANCE_TVGUIDED && CheckForMissileClientControlledDetonation(ent) )
    G_ExplodeMissile((cStaticModel_s *)&savedregs, ent);
  if ( ent->s.eType == 4 )
  {
    dir[0] = origin[0] - ent->r.currentOrigin[0];
    dir[1] = origin[1] - ent->r.currentOrigin[1];
    dir[2] = origin[2] - ent->r.currentOrigin[2];
    speed = Vec3Normalize(dir);
    if ( speed < 0.001 )
    {
      G_RunThink(ent);
      return;
    }
    if ( weapDef->guidedMissileType != MISSILE_GUIDANCE_TVGUIDED || UpdateGuidedMissileFuelTime(ent, 0) )
    {
      if ( weapDef->weapType == WEAPTYPE_PROJECTILE && weapDef->projExplosion == WEAPPROJEXP_BOLT )
      {
        BG_EvaluateTrajectoryDelta(&ent->s.lerp.pos, level.time, velocity);
        vectoangles(velocity, ent->r.currentAngles);
        G_SetAngle(ent, ent->r.currentAngles);
      }
      if ( speed <= 20.0 || SV_PointContents(ent->r.currentOrigin, -1, 32) )
      {
        G_MissileTrace(&tr, ent->r.currentOrigin, origin, passEntNum, ent->clipmask, ent->s.weapon);
      }
      else
      {
        clipMask = ent->clipmask | 0x20;
        G_MissileTrace(&tr, ent->r.currentOrigin, origin, passEntNum, clipMask, ent->s.weapon);
      }
      if ( (char *)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20] & tr.sflags) == &cls.rankedServers[11866].game[59] )
      {
        Vec3Lerp(ent->r.currentOrigin, origin, tr.fraction, endpos);
        RunMissile_CreateWaterSplash(ent, endpos, &tr);
        G_MissileTrace(&tr, ent->r.currentOrigin, origin, passEntNum, ent->clipmask, ent->s.weapon);
      }
      if ( (_UNKNOWN **)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20] & tr.sflags) == &loc_900000 )
        Missile_PenetrateGlass(&tr, ent, ent->r.currentOrigin, origin, weapDef->damage, 0);
      Vec3Lerp(ent->r.currentOrigin, origin, tr.fraction, endpos);
      DrawMissileDebug(ent->r.currentOrigin, endpos);
      ent->r.currentOrigin[0] = endpos[0];
      ent->r.currentOrigin[1] = endpos[1];
      ent->r.currentOrigin[2] = endpos[2];
      if ( ((unsigned int)&cls.rankedServers[711].game[35] & ent->s.lerp.eFlags) != 0 )
      {
        if ( weapDef->stickiness == WEAPSTICKINESS_ALL
          || (weapDef->stickiness == WEAPSTICKINESS_GROUND || weapDef->stickiness == WEAPSTICKINESS_GROUND_WITH_YAW)
          && tr.normal.vec.v[2] > 0.69999999 )
        {
          if ( tr.fraction < 1.0 )
          {
            traceStart[0] = (float)(0.13500001 * tr.normal.vec.v[0]) + ent->r.currentOrigin[0];
            traceStart[1] = (float)(0.13500001 * tr.normal.vec.v[1]) + ent->r.currentOrigin[1];
            traceStart[2] = (float)(0.13500001 * tr.normal.vec.v[2]) + ent->r.currentOrigin[2];
            origin[0] = (float)(-1.5 * tr.normal.vec.v[0]) + ent->r.currentOrigin[0];
            origin[1] = (float)(-1.5 * tr.normal.vec.v[1]) + ent->r.currentOrigin[1];
            origin[2] = (float)(-1.5 * tr.normal.vec.v[2]) + ent->r.currentOrigin[2];
            G_MissileTrace(&trDown, traceStart, origin, passEntNum, ent->clipmask, ent->s.weapon);
            if ( trDown.fraction != 1.0 )
            {
              memcpy(&tr, &trDown, sizeof(tr));
              Vec3Lerp(traceStart, origin, tr.fraction, endpos);
              originOffset[0] = endpos[0] - origin[0];
              originOffset[1] = endpos[1] - origin[1];
              originOffset[2] = endpos[2] - origin[2];
              ent->s.lerp.pos.trBase[0] = ent->s.lerp.pos.trBase[0] + (float)(endpos[0] - origin[0]);
              ent->s.lerp.pos.trBase[1] = ent->s.lerp.pos.trBase[1] + originOffset[1];
              ent->s.lerp.pos.trBase[2] = ent->s.lerp.pos.trBase[2] + originOffset[2];
              ent->r.currentOrigin[0] = endpos[0] + originOffset[0];
              ent->r.currentOrigin[1] = endpos[1] + originOffset[1];
              ent->r.currentOrigin[2] = endpos[2] + originOffset[2];
            }
          }
        }
        else if ( tr.fraction == 1.0 || tr.fraction < 1.0 && tr.normal.vec.v[2] > 0.69999999 )
        {
          traceStart[0] = ent->r.currentOrigin[0];
          traceStart[1] = ent->r.currentOrigin[1];
          traceStart[2] = ent->r.currentOrigin[2];
          origin[0] = ent->r.currentOrigin[0];
          origin[1] = ent->r.currentOrigin[1];
          origin[2] = ent->r.currentOrigin[2];
          traceStart[2] = traceStart[2] + 0.13500001;
          origin[2] = origin[2] - 1.5;
          G_MissileTrace(&trDown, traceStart, origin, passEntNum, ent->clipmask, ent->s.weapon);
          if ( trDown.fraction != 1.0 )
          {
            memcpy(&tr, &trDown, sizeof(tr));
            Vec3Lerp(traceStart, origin, tr.fraction, endpos);
            ent->s.lerp.pos.trBase[2] = (float)((float)(endpos[2] + 1.5) - ent->r.currentOrigin[2])
                                      + ent->s.lerp.pos.trBase[2];
            ent->r.currentOrigin[0] = endpos[0];
            ent->r.currentOrigin[1] = endpos[1];
            ent->r.currentOrigin[2] = endpos[2];
            ent->r.currentOrigin[2] = ent->r.currentOrigin[2] + 1.5;
          }
        }
      }
      SV_LinkEntity((int)&savedregs, ent);
      if ( weapDef->iProjectileActivateDist > 0 )
      {
        delta = Vec3Distance(vOldOrigin, endpos);
        travel_distance = ent->mover.aSpeed + delta;
        ent->mover.aSpeed = travel_distance;
      }
      if ( dword_E07CF8[12 * ent->handler] == 3 )
        G_GrenadeTouchTriggerDamage(
          ent,
          vOldOrigin,
          ent->r.currentOrigin,
          weapDef->iExplosionInnerDamage,
          dword_E07CF8[12 * ent->handler]);
      if ( tr.fraction == 1.0 )
      {
        if ( Abs(ent->s.lerp.pos.trDelta) != 0.0 )
        {
          ent->s.groundEntityNum = 1023;
          if ( weapDef->weapType == WEAPTYPE_PROJECTILE
            && (ent->flags & 0x20000) == 0
            && (weapDef->guidedMissileType == MISSILE_GUIDANCE_NONE
             || weapDef->guidedMissileType == MISSILE_GUIDANCE_SIDEWINDER && weapDef->destabilizeDistance > 0) )
          {
            RunMissile_Destabilize(ent);
          }
        }
        goto LABEL_90;
      }
      if ( weapDef->weapType == WEAPTYPE_BOMB )
      {
        Vec3Lerp(ent->r.currentOrigin, origin, 1.0, tracePosSmall);
        G_MissileTrace(&tr, tracePosSmall, ent->r.currentOrigin, passEntNum, ent->clipmask, ent->s.weapon);
        if ( (tr.sflags & 4) != 0 )
        {
          Vec3Lerp(origin, ent->r.currentOrigin, tr.fraction, tracePosSmall);
          ent->r.currentOrigin[0] = tracePosSmall[0];
          ent->r.currentOrigin[1] = tracePosSmall[1];
          ent->r.currentOrigin[2] = tracePosSmall[2];
        }
        else
        {
          Vec3Lerp(ent->r.currentOrigin, origin, 2.0, tracePosLarge);
          G_MissileTrace(&tr, tracePosLarge, ent->r.currentOrigin, passEntNum, ent->clipmask, ent->s.weapon);
          if ( (tr.sflags & 4) != 0 )
          {
            Vec3Lerp(origin, ent->r.currentOrigin, tr.fraction, tracePosLarge);
            ent->r.currentOrigin[0] = tracePosLarge[0];
            ent->r.currentOrigin[1] = tracePosLarge[1];
            ent->r.currentOrigin[2] = tracePosLarge[2];
          }
          else
          {
            MissileImpact((int)&savedregs, ent, &tr, dir, endpos);
          }
        }
        return;
      }
      if ( (tr.sflags & 4) != 0 )
      {
        ent->r.currentOrigin[0] = origin[0];
        ent->r.currentOrigin[1] = origin[1];
        ent->r.currentOrigin[2] = origin[2];
        return;
      }
      if ( (tr.cflags & 0x800) != 0 )
      {
        ent->r.currentOrigin[0] = origin[0];
        ent->r.currentOrigin[1] = origin[1];
        ent->r.currentOrigin[2] = origin[2];
        return;
      }
      MissileImpact((int)&savedregs, ent, &tr, dir, endpos);
      if ( ent->s.eType == 4 )
      {
        if ( weapDef->isRollingGrenade && tr.normal.vec.v[2] > 0.69999999 && ent->s.lerp.pos.trType )
        {
          if ( ent->s.lerp.pos.trType != 6
            && ent->s.lerp.pos.trType != 7
            && !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
                  3795,
                  0,
                  "%s\n\t(ent->s.lerp.pos.trType) = %i",
                  "(ent->s.lerp.pos.trType == TR_GRAVITY || ent->s.lerp.pos.trType == TR_LOW_GRAVITY)",
                  ent->s.lerp.pos.trType) )
          {
            __debugbreak();
          }
          ent->s.lerp.pos.trType = 1;
          ent->s.lerp.pos.trBase[0] = ent->r.currentOrigin[0];
          ent->s.lerp.pos.trBase[1] = ent->r.currentOrigin[1];
          ent->s.lerp.pos.trBase[2] = ent->r.currentOrigin[2];
        }
LABEL_90:
        if ( missileDebugAttractors->current.enabled )
        {
          for ( attractorIndex = 0; attractorIndex < 0x20; ++attractorIndex )
          {
            circleDir1[0] = FLOAT_1_0;
            circleDir1[1] = *(float *)&FLOAT_0_0;
            circleDir1[2] = *(float *)&FLOAT_0_0;
            circleDir2[0] = *(float *)&FLOAT_0_0;
            circleDir2[1] = FLOAT_1_0;
            circleDir2[2] = *(float *)&FLOAT_0_0;
            circleDir3[0] = *(float *)&FLOAT_0_0;
            circleDir3[1] = *(float *)&FLOAT_0_0;
            circleDir3[2] = FLOAT_1_0;
            radius = FLOAT_10_0;
            if ( attrGlob.attractors[attractorIndex].inUse )
            {
              if ( attrGlob.attractors[attractorIndex].entnum == 1023 )
              {
                center = attrGlob.attractors[attractorIndex].origin[0];
                v4 = attrGlob.attractors[attractorIndex].origin[1];
                v5 = attrGlob.attractors[attractorIndex].origin[2];
              }
              else
              {
                if ( attrGlob.attractors[attractorIndex].entnum >= 1024
                  && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
                        3831,
                        0,
                        "%s",
                        "attrGlob.attractors[attractorIndex].entnum < MAX_GENTITIES") )
                {
                  __debugbreak();
                }
                ent = &g_entities[attrGlob.attractors[attractorIndex].entnum];
                center = ent->r.currentOrigin[0];
                v4 = ent->r.currentOrigin[1];
                v5 = ent->r.currentOrigin[2];
              }
              if ( attrGlob.attractors[attractorIndex].isAttractor )
                v1 = colorGreen;
              else
                v1 = colorOrange;
              color = v1;
              G_DebugCircleEx(&center, radius, circleDir1, v1, 0, 1);
              G_DebugCircleEx(&center, radius, circleDir2, color, 0, 1);
              G_DebugCircleEx(&center, radius, circleDir3, color, 0, 1);
            }
          }
        }
        G_RunThink(ent);
        if ( ent->r.inuse && !BG_ValidateOrigin(origin, 16, 17, svs.mapCenter) )
        {
          if ( weapDef->guidedMissileType == MISSILE_GUIDANCE_TVGUIDED )
            G_UnlinkPlayerToRocket(ent);
          Scr_Notify(ent, scr_const.death, 0);
          G_FreeEntity(ent);
        }
      }
    }
  }
}

// local variable allocation has failed, the output may be wrong!
void  MissileImpact(int a1@<ebp>, gentity_s *ent, trace_t *trace, float *dir, float *endpos)
{
  gentity_s *v5; // eax
  unsigned __int8 v6; // al
  gentity_s *v7; // eax
  unsigned __int8 v8; // al
  unsigned __int8 v9; // al
  unsigned __int8 v10; // al
  unsigned __int8 v11; // al
  unsigned __int8 v12; // al
  unsigned __int8 v13; // al
  unsigned __int8 v14; // al
  unsigned __int8 v15; // al
  unsigned __int8 v16; // al
  unsigned __int8 v17; // al
  unsigned __int8 v18; // al
  unsigned int WeaponIndexForName; // eax
  gentity_s *v20; // eax
  gentity_s *v21; // eax
  double v22; // xmm0_8
  float v23; // xmm0_4
  gentity_s *v24; // eax
  gentity_s *v25; // eax
  float fInnerDamage; // [esp+0h] [ebp-18Ch]
  float fOuterDamage; // [esp+4h] [ebp-188h]
  float radius; // [esp+8h] [ebp-184h]
  int radius_max; // [esp+10h] [ebp-17Ch]
  float *radius_min; // [esp+14h] [ebp-178h]
  gentity_s *radius_mina; // [esp+14h] [ebp-178h]
  unsigned __int16 z_up; // [esp+18h] [ebp-174h]
  int fRadiusSqrd; // [esp+1Ch] [ebp-170h]
  unsigned int fRadiusSqrda; // [esp+1Ch] [ebp-170h]
  long double v35; // [esp+20h] [ebp-16Ch]
  gentity_s *v36; // [esp+28h] [ebp-164h]
  float coneAngleCos; // [esp+30h] [ebp-15Ch]
  int SplashMethodOfDeath; // [esp+3Ch] [ebp-150h]
  gentity_s *v39; // [esp+40h] [ebp-14Ch]
  unsigned __int8 v40; // [esp+47h] [ebp-145h]
  unsigned int v41; // [esp+50h] [ebp-13Ch]
  unsigned int partName; // [esp+54h] [ebp-138h] BYREF
  const WeaponDef *grenadeWeapon; // [esp+58h] [ebp-134h]
  int entnum; // [esp+5Ch] [ebp-130h]
  unsigned int boneName; // [esp+60h] [ebp-12Ch]
  const char *v46; // [esp+68h] [ebp-124h]
  const char *v47; // [esp+6Ch] [ebp-120h]
  gentity_s *v48; // [esp+70h] [ebp-11Ch]
  bool v49; // [esp+77h] [ebp-115h]
  float v50; // [esp+78h] [ebp-114h] BYREF
  gentity_s *owner; // [esp+7Ch] [ebp-110h]
  int v52; // [esp+80h] [ebp-10Ch] OVERLAPPED
  float javNormal[3]; // [esp+84h] [ebp-108h]
  const float *normal; // [esp+90h] [ebp-FCh]
  bool depth; // [esp+94h] [ebp-F8h]
  gentity_s *v56; // [esp+98h] [ebp-F4h] OVERLAPPED
  int nomarks; // [esp+9Ch] [ebp-F0h]
  int v58; // [esp+A0h] [ebp-ECh]
  float v59; // [esp+A4h] [ebp-E8h]
  float v60; // [esp+A8h] [ebp-E4h] BYREF
  float speed; // [esp+B0h] [ebp-DCh]
  float velocity[3]; // [esp+B4h] [ebp-D8h] BYREF
  float v63; // [esp+C0h] [ebp-CCh]
  float waterSurfacePos[3]; // [esp+C4h] [ebp-C8h] BYREF
  float v65; // [esp+D0h] [ebp-BCh]
  float waterNormal[3]; // [esp+D4h] [ebp-B8h]
  float v67; // [esp+E0h] [ebp-ACh] OVERLAPPED BYREF
  float v68; // [esp+E4h] [ebp-A8h]
  float waterHeight; // [esp+E8h] [ebp-A4h]
  float preBounceVelocity[3]; // [esp+ECh] [ebp-A0h] BYREF
  float v71; // [esp+F8h] [ebp-94h]
  float v72; // [esp+FCh] [ebp-90h]
  hitLocation_t v73; // [esp+100h] [ebp-8Ch]
  hitLocation_t partGroup; // [esp+104h] [ebp-88h]
  int v75; // [esp+108h] [ebp-84h]
  hitLocation_t hitLocation; // [esp+10Ch] [ebp-80h]
  gentity_s *v77; // [esp+110h] [ebp-7Ch]
  int methodOfDeath; // [esp+114h] [ebp-78h] BYREF
  float v79; // [esp+118h] [ebp-74h] OVERLAPPED
  gentity_s *eventEnt; // [esp+11Ch] [ebp-70h]
  float direction_vector[3]; // [esp+120h] [ebp-6Ch]
  int damage; // [esp+12Ch] [ebp-60h]
  int explosionType; // [esp+130h] [ebp-5Ch]
  int explodeOnImpact; // [esp+134h] [ebp-58h]
  const WeaponDef *weapDef; // [esp+138h] [ebp-54h]
  const WeaponVariantDef *weapVariantDef; // [esp+13Ch] [ebp-50h]
  gentity_s *other; // [esp+140h] [ebp-4Ch]
  int hitEntId; // [esp+144h] [ebp-48h] OVERLAPPED
  int v89; // [esp+148h] [ebp-44h] OVERLAPPED
  trace_t waterTrace; // [esp+14Ch] [ebp-40h] BYREF
  int hitClient; // [esp+184h] [ebp-8h]
  int retaddr; // [esp+18Ch] [ebp+0h]

  waterTrace.hitPartition = a1;
  hitClient = retaddr;
  *(unsigned int *)&waterTrace.walkable = 0;
  other = *(gentity_s **)&FLOAT_0_0;
  hitEntId = *(unsigned int *)&FLOAT_0_0;
  v89 = *(unsigned int *)&FLOAT_0_0;
  waterTrace.normal.vec.u[0] = *(unsigned int *)&FLOAT_0_0;
  HIBYTE(weapVariantDef) = 0;
  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 1187, 0, "%s", "ent") )
    __debugbreak();
  if ( ent->s.eType != 4
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          1188,
          0,
          "%s",
          "ent->s.eType == ET_MISSILE") )
  {
    __debugbreak();
  }
  if ( !ent->s.weapon
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 1189, 0, "%s", "ent->s.weapon") )
  {
    __debugbreak();
  }
  LOWORD(weapDef) = Trace_GetEntityHitId(trace);
  explodeOnImpact = (int)&g_entities[(unsigned __int16)weapDef];
  explosionType = (int)BG_GetWeaponVariantDef(ent->s.weapon);
  damage = (int)BG_GetWeaponDef(ent->s.weapon);
  if ( !explosionType
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 1196, 0, "%s", "weapVariantDef") )
  {
    __debugbreak();
  }
  if ( !damage && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 1197, 0, "%s", "weapDef") )
    __debugbreak();
  LODWORD(direction_vector[2]) = *(unsigned __int8 *)(damage + 1580);
  direction_vector[1] = *(float *)(damage + 1516);
  direction_vector[0] = *(float *)(damage + 860);
  if ( trace->hitType == TRACE_HITTYPE_GLASS )
  {
    BG_EvaluateTrajectoryDelta(&ent->s.lerp.pos, level.time, (float *)&methodOfDeath);
    if ( (float)((float)((float)(*(float *)&methodOfDeath * *(float *)&methodOfDeath) + (float)(v79 * v79))
               + (float)(*(float *)&eventEnt * *(float *)&eventEnt)) >= 0.001 )
    {
      Vec3Normalize((float *)&methodOfDeath);
    }
    else
    {
      methodOfDeath = *(int *)&FLOAT_0_0;
      v79 = *(float *)&FLOAT_0_0;
      *(float *)&eventEnt = FLOAT_1_0;
    }
    GlassSv_Damage(trace->hitId, SLODWORD(direction_vector[0]), 16, ent->r.currentOrigin, (float *)&methodOfDeath);
  }
  else
  {
    ent->s.surfType = (int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20] & trace->sflags) >> 20;
    v77 = 0;
    HIBYTE(hitLocation) = isDud(ent, (const WeaponDef *)damage);
    if ( HIBYTE(hitLocation) )
    {
      LODWORD(direction_vector[1]) = 4;
      direction_vector[2] = 0.0;
      ent->mover.aSpeed = FLOAT_N1_0e10;
      v75 = 16;
    }
    else if ( LODWORD(direction_vector[2]) )
    {
      v75 = dword_E07CF8[12 * ent->handler];
    }
    else
    {
      v75 = 16;
    }
    if ( v75 == 16 || LODWORD(direction_vector[1]) == 9 )
      partGroup = trace->partGroup;
    else
      partGroup = HITLOC_NONE;
    v73 = partGroup;
    if ( v75 != 7 && EntHandle::isDefined(&ent->r.ownerNum) )
    {
      radius_min = EntHandle::ent(&ent->r.ownerNum)->s.lerp.pos.trBase;
      v5 = EntHandle::ent(&ent->r.ownerNum);
      Actor_BroadcastLineEvent(v5, 0, AI_EV_PROJECTILE_IMPACT, 0, radius_min, endpos, 0.0);
    }
    if ( *(_BYTE *)(explodeOnImpact + 351) && SLODWORD(direction_vector[0]) > 0 )
    {
      BG_EvaluateTrajectoryDelta(&ent->s.lerp.pos, level.time, &preBounceVelocity[2]);
      if ( (float)((float)((float)(preBounceVelocity[2] * preBounceVelocity[2]) + (float)(v71 * v71))
                 + (float)(v72 * v72)) < 0.001 )
      {
        preBounceVelocity[2] = *(float *)&FLOAT_0_0;
        v71 = *(float *)&FLOAT_0_0;
        v72 = FLOAT_1_0;
      }
      if ( EntHandle::isDefined(&ent->r.ownerNum) )
        LODWORD(preBounceVelocity[1]) = EntHandle::ent(&ent->r.ownerNum);
      else
        preBounceVelocity[1] = 0.0;
      G_Damage(
        (gentity_s *)explodeOnImpact,
        ent,
        (gentity_s *)LODWORD(preBounceVelocity[1]),
        &preBounceVelocity[2],
        ent->r.currentOrigin,
        SLODWORD(direction_vector[0]),
        0,
        v75,
        ent->s.weapon,
        v73,
        trace->modelIndex,
        trace->partName,
        0);
      HIBYTE(weapVariantDef) = 1;
    }
    if ( *(unsigned int *)(damage + 1640) == 6 )
      G_UnlinkPlayerToRocket(ent);
    if ( ((unsigned int)&cls.rankedServers[711].game[35] & ent->s.lerp.eFlags) == 0
      || LODWORD(direction_vector[2])
      || CheckCrumpleMissile(ent, trace) )
    {
      waterNormal[2] = CM_GetWaterHeight(ent->r.currentOrigin, 1000.0, -200.0);
      LODWORD(waterNormal[1]) = waterNormal[2] >= ent->r.currentOrigin[2];
      HIBYTE(waterNormal[0]) = LOBYTE(waterNormal[1]);
      if ( LOBYTE(waterNormal[1]) )
      {
        waterSurfacePos[1] = *(float *)&FLOAT_0_0;
        waterSurfacePos[2] = *(float *)&FLOAT_0_0;
        v65 = FLOAT_1_0;
        LODWORD(waterSurfacePos[0]) = ent->r.currentOrigin;
        velocity[1] = ent->r.currentOrigin[0];
        velocity[2] = ent->r.currentOrigin[1];
        v63 = waterNormal[2];
      }
      else
      {
        HIBYTE(waterNormal[0]) = SV_PointContents(ent->r.currentOrigin, -1, 32) != 0;
      }
      if ( LODWORD(direction_vector[1]) == 7 && HIBYTE(waterNormal[0]) )
        direction_vector[0] = 0.0;
      if ( !*(_BYTE *)(explodeOnImpact + 351) )
        goto LABEL_98;
      if ( LODWORD(direction_vector[0]) )
      {
        if ( EntHandle::isDefined(&ent->r.ownerNum) )
          LODWORD(velocity[0]) = EntHandle::ent(&ent->r.ownerNum);
        else
          LODWORD(velocity[0]) = &g_entities[1023];
        if ( LogAccuracyHit((gentity_s *)explodeOnImpact, (gentity_s *)LODWORD(velocity[0])) )
          *(unsigned int *)&waterTrace.walkable = 1;
        BG_EvaluateTrajectoryDelta(&ent->s.lerp.pos, level.time, &v60);
        v59 = Abs(&v60);
        if ( v59 == 0.0 )
          speed = FLOAT_1_0;
        if ( *(unsigned int *)(damage + 28) != 1 && *(unsigned int *)(damage + 28) != 6
          || LODWORD(direction_vector[1]) == 7
          || v59 > g_minGrenadeDamageSpeed->current.value )
        {
          v58 = 0;
          if ( *(_BYTE *)(damage + 1358) )
            v58 |= 2u;
          if ( !HIBYTE(weapVariantDef) )
          {
            if ( EntHandle::isDefined(&ent->r.ownerNum) )
              nomarks = (int)EntHandle::ent(&ent->r.ownerNum);
            else
              nomarks = 0;
            G_Damage(
              (gentity_s *)explodeOnImpact,
              ent,
              (gentity_s *)nomarks,
              &v60,
              ent->r.currentOrigin,
              SLODWORD(direction_vector[0]),
              v58,
              v75,
              ent->s.weapon,
              v73,
              trace->modelIndex,
              trace->partName,
              0);
          }
        }
      }
      if ( LODWORD(direction_vector[2]) )
        goto LABEL_98;
      if ( *(unsigned int *)(explodeOnImpact + 324) && !trace->sflags )
        trace->sflags = (int)&off_700000;
      if ( CheckCrumpleMissile(ent, trace) || !isBounceProjectile(ent) )
      {
LABEL_98:
        if ( LODWORD(direction_vector[0]) )
        {
          if ( EntHandle::isDefined(&ent->r.ownerNum) )
            v56 = EntHandle::ent(&ent->r.ownerNum);
          else
            v56 = &g_entities[1022];
          G_CheckHitTriggerDamage(v56, ent->r.currentOrigin, endpos, SLODWORD(direction_vector[0]), v75);
        }
        depth = *(unsigned int *)&waterTrace.walkable || trace->partName;
        normal = (const float *)depth;
        HIBYTE(javNormal[2]) = 1;
        if ( HIBYTE(waterNormal[0]) && LODWORD(direction_vector[1]) != 7 )
        {
          javNormal[1] = waterNormal[2] - endpos[2];
          if ( javNormal[1] <= -1.0 || missileWaterMaxDepth->current.value <= javNormal[1] )
          {
            HIBYTE(javNormal[2]) = 0;
          }
          else
          {
            waterSurfacePos[1] = *(float *)&FLOAT_0_0;
            waterSurfacePos[2] = *(float *)&FLOAT_0_0;
            v65 = FLOAT_1_0;
            velocity[1] = *endpos;
            velocity[2] = endpos[1];
            v63 = waterNormal[2];
          }
        }
        if ( HIBYTE(waterNormal[0]) && HIBYTE(javNormal[2]) )
        {
          LODWORD(javNormal[0]) = &waterSurfacePos[1];
        }
        else if ( ent->s.eType == 4
               && *(unsigned int *)(damage + 1640) == 3
               && ent->missile.missile.flightMode == MISSILEFLIGHTMODE_TOP
               && EntHandle::isDefined(&ent->missileTargetEnt)
               && (v7 = EntHandle::ent(&ent->missileTargetEnt), v7 == (gentity_s *)explodeOnImpact) )
        {
          LODWORD(javNormal[0]) = up;
        }
        else if ( *(_BYTE *)(damage + 1524) )
        {
          LODWORD(javNormal[0]) = up;
        }
        else
        {
          LODWORD(javNormal[0]) = trace;
        }
        if ( !HIBYTE(waterNormal[0]) || HIBYTE(javNormal[2]) || LODWORD(direction_vector[1]) == 7 )
        {
          if ( LODWORD(direction_vector[1]) && LODWORD(direction_vector[1]) != 6 )
          {
            switch ( LODWORD(direction_vector[1]) )
            {
              case 1:
                v10 = DirToByte((const float *)LODWORD(javNormal[0]));
                G_AddEvent(ent, (normal != 0) + 58, v10);
                break;
              case 3:
                v11 = DirToByte((const float *)LODWORD(javNormal[0]));
                G_AddEvent(ent, (normal != 0) + 61, v11);
                break;
              case 2:
                v12 = DirToByte((const float *)LODWORD(javNormal[0]));
                G_AddEvent(ent, 0x3Cu, v12);
                break;
              case 4:
                if ( JavelinProjectile(ent, (const WeaponDef *)damage) )
                {
                  Vec3NormalizeTo(ent->s.lerp.pos.trDelta, &v50);
                  LODWORD(v50) ^= _mask__NegFloat_;
                  owner = (gentity_s *)((unsigned int)owner ^ _mask__NegFloat_);
                  v52 ^= _mask__NegFloat_;
                  v13 = DirToByte(&v50);
                  G_AddEvent(ent, 0x41u, v13);
                }
                else
                {
                  v14 = DirToByte((const float *)LODWORD(javNormal[0]));
                  G_AddEvent(ent, 0x41u, v14);
                }
                break;
              case 7:
                if ( HIBYTE(waterNormal[0]) )
                {
                  direction_vector[2] = 0.0;
                  HIBYTE(javNormal[2]) = 0;
                  v16 = DirToByte((const float *)LODWORD(javNormal[0]));
                  G_AddEvent(ent, 0x44u, v16);
                }
                else
                {
                  v49 = *(unsigned int *)(explodeOnImpact + 344) != 0;
                  if ( !v49 && EntHandle::isDefined((EntHandle *)(explodeOnImpact + 396)) )
                    v49 = EntHandle::ent((EntHandle *)(explodeOnImpact + 396))->scr_vehicle != 0;
                  if ( !v49 )
                    Weapon_Napalm_Flame(ent, trace, missileMolotovBlobNum->current.integer);
                  if ( EntHandle::isDefined(&ent->r.ownerNum) )
                  {
                    v48 = EntHandle::ent(&ent->parent);
                    AssignToSmallerType<short>(&ent->s.attackerEntityNum, v48->s.number);
                  }
                  else
                  {
                    ent->s.attackerEntityNum = -1;
                  }
                  v15 = DirToByte((const float *)LODWORD(javNormal[0]));
                  G_AddEvent(ent, 0x42u, v15);
                }
                break;
              case 8:
                v77 = G_Spawn();
                v77->s.eType = 0;
                v77->s.lerp.eFlags |= 0x20u;
                v77->s.weapon = ent->s.weapon;
                v77->s.weaponModel = ent->s.weaponModel;
                v77->r.contents = ent->r.contents;
                G_SetOrigin(v77, endpos);
                G_BroadcastEntity(v77);
                v17 = DirToByte((const float *)LODWORD(javNormal[0]));
                G_AddEvent(v77, 0x42u, v17);
                v77->s.lerp.pos.trBase[0] = (float)(int)v77->s.lerp.pos.trBase[0];
                v77->s.lerp.pos.trBase[1] = (float)(int)v77->s.lerp.pos.trBase[1];
                v77->s.lerp.pos.trBase[2] = (float)(int)v77->s.lerp.pos.trBase[2];
                G_SetOrigin(v77, v77->s.lerp.pos.trBase);
                v77->s.lerp.eFlags |= 0x4000u;
                v77->s.lerp.u.actor.index.actorNum = level.time;
                v77->s.time2 = level.time + (int)(float)(missileMolotovBlobTime->current.value * 1000.0);
                v77->s.lerp.eFlags |= 0x10u;
                v77->handler = 11;
                v77->nextthink = level.time + 1;
                v77->s.attackerEntityNum = ent->s.attackerEntityNum;
                break;
              case 9:
                v47 = *(const char **)(damage + 1420);
                v46 = v47 + 1;
                v47 += strlen(v47) + 1;
                boneName = v47 - v46;
                entnum = v47 != v46 && (*(unsigned int *)explodeOnImpact || *(unsigned int *)(explodeOnImpact + 324));
                HIBYTE(grenadeWeapon) = entnum;
                partName = 0;
                v41 = (unsigned __int16)weapDef;
                if ( *(unsigned int *)(explodeOnImpact + 324) )
                {
                  MapHitLocationToRagdollBoneName(v73, &partName);
                  if ( *(unsigned int *)(*(unsigned int *)(explodeOnImpact + 324) + 4) == 9
                    && (int)g_scr_data.actorXAnimTrees[376 * *(unsigned int *)(*(unsigned int *)(explodeOnImpact + 324) + 216)
                                                     - 1495] >= 0 )
                  {
                    v41 = (unsigned int)g_scr_data.actorXAnimTrees[376
                                                                 * *(unsigned int *)(*(unsigned int *)(explodeOnImpact + 324) + 216)
                                                                 - 1495];
                  }
                }
                else if ( *(unsigned int *)(explodeOnImpact + 328) )
                {
                  MapHitLocationToRagdollBoneName(v73, &partName);
                }
                else if ( *(unsigned int *)(explodeOnImpact + 344) )
                {
                  partName = trace->partName;
                }
                if ( partName )
                  AssignToSmallerType<unsigned short>((unsigned __int16 *)&ent->s.index, partName);
                AssignToSmallerType<short>(&ent->s.groundEntityNum, v41);
                v18 = DirToByte((const float *)LODWORD(javNormal[0]));
                G_AddEvent(ent, 0xC1u, v18);
                if ( HIBYTE(grenadeWeapon) )
                {
                  WeaponIndexForName = G_GetWeaponIndexForName(*(char **)(damage + 1420));
                  fRadiusSqrd = BG_GetWeaponDef(WeaponIndexForName)->fuseTime;
                  radius_max = G_GetWeaponIndexForName(*(char **)(damage + 1420));
                  v20 = EntHandle::ent(&ent->parent);
                  v21 = G_FireGrenade(v20, endpos, dir, radius_max, 0, 0, fRadiusSqrd);
                  v77 = v21;
                  v21->mover.midTime = trace->normal.vec.v[0];
                  v21->mover.aMidTime = trace->normal.vec.v[1];
                  v21->trigger.exposureLerpToLighter = trace->normal.vec.v[2];
                  AttachMissileToEntity(v77, v41, partName, v73, ent->s.lerp.apos.trDelta);
                  AttachBoltGrenade(v77, (gentity_s *)explodeOnImpact, trace, v73);
                }
                break;
            }
          }
          else
          {
            v9 = DirToByte((const float *)LODWORD(javNormal[0]));
            G_AddEvent(ent, 0x39u, v9);
          }
        }
        else
        {
          v8 = DirToByte((const float *)LODWORD(javNormal[0]));
          G_AddEvent(ent, 0x40u, v8);
        }
        if ( HIBYTE(waterNormal[0]) )
          v40 = 20;
        else
          v40 = (int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20] & trace->sflags) >> 20;
        ent->s.surfType = v40;
        v39 = 0;
        if ( EntHandle::isDefined(&ent->r.ownerNum) )
          v39 = EntHandle::ent(&ent->r.ownerNum);
        if ( *(_BYTE *)(damage + 1390) || *(_BYTE *)(damage + 1389) )
        {
          createRetrieveableProjectile(
            ent,
            (gentity_s *)explodeOnImpact,
            (const WeaponDef *)damage,
            trace,
            v73,
            dir,
            endpos);
        }
        else if ( v39 )
        {
          Scr_AddEntity(v39, SCRIPTINSTANCE_SERVER);
          Scr_Notify(ent, scr_const.death, 1u);
        }
        else
        {
          Scr_Notify(ent, scr_const.death, 0);
        }
        ent->s.lerp.eFlags ^= 2u;
        ent->s.lerp.eFlags |= 0x20u;
        if ( HIBYTE(waterNormal[0]) && HIBYTE(javNormal[2]) )
          G_SetOrigin(ent, &velocity[1]);
        else
          G_SetOrigin(ent, endpos);
        if ( LODWORD(direction_vector[2]) )
        {
          if ( *(unsigned int *)(damage + 1468) )
          {
            SplashMethodOfDeath = GetSplashMethodOfDeath(ent);
            if ( EntHandle::isDefined(&ent->parent) )
            {
              v22 = (float)(*(float *)(damage + 1476) * 0.017453292);
              __libm_sse2_cos(v35);
              v23 = v22;
              if ( (float)(*(float *)(damage + 1476) - 180.0) < 0.0 )
                coneAngleCos = v23;
              else
                coneAngleCos = FLOAT_N1_0;
              if ( explodeOnImpact && *(unsigned int *)(explodeOnImpact + 340) )
                explodeOnImpact = 0;
              fRadiusSqrda = ent->s.weapon;
              radius_mina = (gentity_s *)explodeOnImpact;
              radius = (float)*(int *)(damage + 1456);
              fOuterDamage = (float)*(int *)(damage + 1472);
              fInnerDamage = (float)*(int *)(damage + 1468);
              v24 = EntHandle::ent(&ent->parent);
              G_RadiusDamage(
                endpos,
                ent,
                v24,
                fInnerDamage,
                fOuterDamage,
                radius,
                coneAngleCos,
                dir,
                radius_mina,
                SplashMethodOfDeath,
                fRadiusSqrda);
              Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
              Scr_AddInt(*(unsigned int *)(damage + 1456), SCRIPTINSTANCE_SERVER);
              Scr_AddVector(endpos, SCRIPTINSTANCE_SERVER);
              Scr_AddString(*(char **)explosionType, SCRIPTINSTANCE_SERVER);
              z_up = scr_const.projectile_impact;
              v25 = EntHandle::ent(&ent->parent);
              Scr_Notify(v25, z_up, 4u);
            }
          }
        }
        if ( LODWORD(direction_vector[1]) == 2 )
        {
          if ( EntHandle::isDefined(&ent->parent) )
            v36 = EntHandle::ent(&ent->parent);
          else
            v36 = 0;
          G_FlashbangBlast(
            endpos,
            (float)*(int *)(damage + 1456),
            (float)*(int *)(damage + 1460),
            v36,
            ent->missile.team);
        }
        if ( LODWORD(direction_vector[1]) == 8 )
          G_FreeEntity(ent);
        else
          G_FreeEntityAfterEvent(ent);
        if ( LODWORD(direction_vector[1]) == 8 )
        {
          if ( v77 )
            SV_LinkEntity((int)&waterTrace.hitPartition, v77);
        }
        else
        {
          SV_LinkEntity((int)&waterTrace.hitPartition, ent);
        }
      }
      else if ( BounceMissile((cStaticModel_s *)&waterTrace.hitPartition, ent, trace) && !trace->startsolid )
      {
        G_AddEvent(
          ent,
          0x38u,
          (unsigned __int8)((int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20] & trace->sflags) >> 20));
        ent->s.surfType = (int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20] & trace->sflags) >> 20;
      }
    }
    else
    {
      LODWORD(preBounceVelocity[0]) = ent->s.lerp.pos.trDelta;
      v67 = ent->s.lerp.pos.trDelta[0];
      v68 = ent->s.lerp.pos.trDelta[1];
      waterHeight = ent->s.lerp.pos.trDelta[2];
      if ( BounceMissile((cStaticModel_s *)&waterTrace.hitPartition, ent, trace) && !trace->startsolid )
      {
        if ( *(unsigned int *)(damage + 1584) == 1 || *(unsigned int *)(damage + 1584) == 5 )
        {
          if ( StickMissile(ent, (gentity_s *)explodeOnImpact, (const WeaponDef *)damage, trace, v73, &v67) )
          {
            GScr_AddEntity(ent);
            Scr_Notify((gentity_s *)explodeOnImpact, scr_const.grenade_stuck, 1u);
            Scr_AddVector(trace->normal.vec.v, SCRIPTINSTANCE_SERVER);
            Scr_AddVector(endpos, SCRIPTINSTANCE_SERVER);
            Scr_Notify(ent, scr_const.stationary, 2u);
          }
          else
          {
            Scr_AddVector(trace->normal.vec.v, SCRIPTINSTANCE_SERVER);
            Scr_AddVector(endpos, SCRIPTINSTANCE_SERVER);
            Scr_Notify(ent, scr_const.grenade_bounce, 2u);
          }
        }
        else
        {
          Scr_AddVector(trace->normal.vec.v, SCRIPTINSTANCE_SERVER);
          Scr_AddVector(endpos, SCRIPTINSTANCE_SERVER);
          Scr_Notify(ent, scr_const.grenade_bounce, 2u);
        }
        if ( *(unsigned int *)(explodeOnImpact + 332) && !trace->sflags )
          trace->sflags = (int)&off_700000;
        G_AddEvent(
          ent,
          0x38u,
          (unsigned __int8)((int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20] & trace->sflags) >> 20));
        ent->s.surfType = (int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20] & trace->sflags) >> 20;
      }
      if ( HIBYTE(hitLocation) && !ent->s.lerp.pos.trType )
      {
        v6 = DirToByte(trace->normal.vec.v);
        G_AddEvent(ent, 0x3Fu, v6);
        Scr_Notify(ent, scr_const.death, 0);
        G_FreeEntityAfterEvent(ent);
      }
    }
  }
}

bool __cdecl CheckCrumpleMissile(gentity_s *ent, trace_t *trace)
{
  float velocity[3]; // [esp+Ch] [ebp-20h] BYREF
  int hitTime; // [esp+18h] [ebp-14h]
  float MIN_CRUMPLE_SPEED; // [esp+1Ch] [ebp-10h]
  float cos45; // [esp+20h] [ebp-Ch]
  float speed; // [esp+24h] [ebp-8h]
  const WeaponDef *weapDef; // [esp+28h] [ebp-4h]

  if ( !ent->s.weapon
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 342, 0, "%s", "ent->s.weapon") )
  {
    __debugbreak();
  }
  weapDef = BG_GetWeaponDef(ent->s.weapon);
  if ( !weapDef && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 344, 0, "%s", "weapDef") )
    __debugbreak();
  if ( ForcedDud(ent) )
    return 0;
  if ( weapDef->weapType != WEAPTYPE_PROJECTILE )
    return 0;
  if ( (_UNKNOWN **)trace->sflags == &off_700000 )
    return 1;
  if ( weapDef->bNoCrumpleMissile )
    return 0;
  hitTime = level.previousTime + (int)(float)((float)(level.time - level.previousTime) * trace->fraction);
  BG_EvaluateTrajectoryDelta(&ent->s.lerp.pos, hitTime, velocity);
  speed = Abs(velocity);
  MIN_CRUMPLE_SPEED = FLOAT_500_0;
  if ( speed < 500.0 )
    return 0;
  velocity[0] = (float)(-1.0 / speed) * velocity[0];
  velocity[1] = (float)(-1.0 / speed) * velocity[1];
  velocity[2] = (float)(-1.0 / speed) * velocity[2];
  cos45 = FLOAT_0_70700002;
  return (float)((float)((float)(velocity[0] * trace->normal.vec.v[0]) + (float)(velocity[1] * trace->normal.vec.v[1]))
               + (float)(velocity[2] * trace->normal.vec.v[2])) > 0.70700002;
}

// local variable allocation has failed, the output may be wrong!
bool  BounceMissile@<eax>(cStaticModel_s *a1@<ebp>, gentity_s *ent, trace_t *trace)
{
  float v3; // xmm0_4
  long double v5; // [esp+20h] [ebp-1CCh]
  long double v6; // [esp+20h] [ebp-1CCh]
  int v7; // [esp+38h] [ebp-1B4h]
  float v8[3]; // [esp+3Ch] [ebp-1B0h] BYREF
  float traceEnd[3]; // [esp+48h] [ebp-1A4h]
  float v10; // [esp+54h] [ebp-198h]
  float vDelta[3]; // [esp+58h] [ebp-194h] BYREF
  gentity_s *groundEnt; // [esp+64h] [ebp-188h]
  float v13; // [esp+68h] [ebp-184h]
  float vAngles[3]; // [esp+6Ch] [ebp-180h]
  float v15; // [esp+78h] [ebp-174h]
  float v16; // [esp+7Ch] [ebp-170h]
  float v17; // [esp+80h] [ebp-16Ch]
  float v18[3]; // [esp+84h] [ebp-168h] BYREF
  float targetAngles[3]; // [esp+90h] [ebp-15Ch]
  float revolutions; // [esp+9Ch] [ebp-150h]
  float *linearSpeed; // [esp+A0h] [ebp-14Ch]
  int v22; // [esp+A4h] [ebp-148h]
  float v23[3]; // [esp+A8h] [ebp-144h] BYREF
  float wobbleNewPos[3]; // [esp+B4h] [ebp-138h]
  float v25; // [esp+C0h] [ebp-12Ch]
  float v26; // [esp+C4h] [ebp-128h]
  float wobbleSideScale; // [esp+C8h] [ebp-124h] BYREF
  float v28; // [esp+CCh] [ebp-120h]
  float v29; // [esp+D0h] [ebp-11Ch]
  float side[3]; // [esp+D4h] [ebp-118h]
  float v31; // [esp+E0h] [ebp-10Ch]
  float v32; // [esp+E4h] [ebp-108h]
  float v33; // [esp+E8h] [ebp-104h]
  float wobbleDelta; // [esp+ECh] [ebp-100h]
  float v35; // [esp+F0h] [ebp-FCh]
  int v36; // [esp+F4h] [ebp-F8h] BYREF
  float oldCycle; // [esp+F8h] [ebp-F4h]
  float wobbleFreq; // [esp+FCh] [ebp-F0h]
  trace_t sideTrace; // [esp+100h] [ebp-ECh]
  float v40; // [esp+138h] [ebp-B4h]
  float v41; // [esp+13Ch] [ebp-B0h]
  float mag; // [esp+140h] [ebp-ACh]
  float v43; // [esp+144h] [ebp-A8h] BYREF
  float v44[2]; // [esp+148h] [ebp-A4h] BYREF
  float slideSpeed; // [esp+150h] [ebp-9Ch]
  float slideDir[3]; // [esp+154h] [ebp-98h]
  float v47; // [esp+160h] [ebp-8Ch] OVERLAPPED
  float v48; // [esp+164h] [ebp-88h]
  float v49; // [esp+168h] [ebp-84h] OVERLAPPED
  float v50; // [esp+16Ch] [ebp-80h]
  float *v51; // [esp+170h] [ebp-7Ch]
  float v52; // [esp+174h] [ebp-78h]
  float *trDelta; // [esp+178h] [ebp-74h]
  bool v54; // [esp+17Ch] [ebp-70h]
  bool v55; // [esp+180h] [ebp-6Ch]
  float v56; // [esp+184h] [ebp-68h]
  int shouldRoll; // [esp+188h] [ebp-64h] BYREF
  float v58; // [esp+18Ch] [ebp-60h]
  float dot; // [esp+190h] [ebp-5Ch]
  float velocity[3]; // [esp+194h] [ebp-58h]
  int hitTime; // [esp+1A0h] [ebp-4Ch]
  int surfType; // [esp+1A4h] [ebp-48h] BYREF
  int contents; // [esp+1A8h] [ebp-44h]
  const WeaponDef *weapDef; // [esp+1ACh] [ebp-40h]
  trace_t tempTrace; // [esp+1B0h] [ebp-3Ch]
  int retaddr; // [esp+1ECh] [ebp+0h]

  tempTrace.staticModel = a1;
  tempTrace.hitPartition = retaddr;
  surfType = *(unsigned int *)&FLOAT_0_0;
  contents = *(unsigned int *)&FLOAT_0_0;
  weapDef = *(const WeaponDef **)&FLOAT_0_0;
  tempTrace.normal.vec.u[0] = *(unsigned int *)&FLOAT_0_0;
  if ( !ent->s.weapon
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 529, 0, "%s", "ent->s.weapon") )
  {
    __debugbreak();
  }
  hitTime = (int)BG_GetWeaponDef(ent->s.weapon);
  if ( !hitTime && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 531, 0, "%s", "weapDef") )
    __debugbreak();
  LODWORD(velocity[2]) = SV_PointContents(ent->r.currentOrigin, -1, 32);
  LODWORD(velocity[1]) = (unsigned __int8)((int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20] & trace->sflags) >> 20);
  LODWORD(velocity[0]) = level.previousTime + (int)(float)((float)(level.time - level.previousTime) * trace->fraction);
  BG_EvaluateTrajectoryDelta(&ent->s.lerp.pos, SLODWORD(velocity[0]), (float *)&shouldRoll);
  v56 = (float)((float)(*(float *)&shouldRoll * trace->normal.vec.v[0]) + (float)(v58 * trace->normal.vec.v[1]))
      + (float)(dot * trace->normal.vec.v[2]);
  v55 = grenadeRollingEnabled->current.enabled && *(unsigned int *)(hitTime + 1452);
  v54 = v55;
  if ( v55 )
  {
    trDelta = ent->s.lerp.pos.trDelta;
    LODWORD(v52) = LODWORD(v56) ^ _mask__NegFloat_;
    ent->s.lerp.pos.trDelta[0] = (float)(COERCE_FLOAT(LODWORD(v56) ^ _mask__NegFloat_) * trace->normal.vec.v[0])
                               + *(float *)&shouldRoll;
    trDelta[1] = (float)(v52 * trace->normal.vec.v[1]) + v58;
    trDelta[2] = (float)(v52 * trace->normal.vec.v[2]) + dot;
  }
  else
  {
    v51 = ent->s.lerp.pos.trDelta;
    v50 = -2.0 * v56;
    ent->s.lerp.pos.trDelta[0] = (float)((float)(-2.0 * v56) * trace->normal.vec.v[0]) + *(float *)&shouldRoll;
    v51[1] = (float)(v50 * trace->normal.vec.v[1]) + v58;
    v51[2] = (float)(v50 * trace->normal.vec.v[2]) + dot;
  }
  v49 = ent->s.lerp.pos.trDelta[0];
  if ( (LODWORD(v49) & 0x7F800000) == 0x7F800000
    || (v48 = ent->s.lerp.pos.trDelta[1], (LODWORD(v48) & 0x7F800000) == 0x7F800000)
    || (v47 = ent->s.lerp.pos.trDelta[2], (LODWORD(v47) & 0x7F800000) == 0x7F800000) )
  {
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
            546,
            0,
            "%s",
            "!IS_NAN((ent->s.lerp.pos.trDelta)[0]) && !IS_NAN((ent->s.lerp.pos.trDelta)[1]) && !IS_NAN((ent->s.lerp.pos.trDelta)[2])") )
      __debugbreak();
  }
  if ( *(unsigned int *)(hitTime + 1584) == 1 )
  {
    HIBYTE(slideDir[2]) = 1;
    if ( *(unsigned int *)(hitTime + 1584) == 2
      && (Trace_GetEntityHitId(trace) < 0x20u || g_entities[Trace_GetEntityHitId(trace)].s.eType == 17) )
    {
      HIBYTE(slideDir[2]) = 0;
    }
  }
  else
  {
    LODWORD(slideDir[1]) = !*(unsigned int *)(hitTime + 1584) || Trace_GetEntityHitId(trace) >= 0x20u;
    HIBYTE(slideDir[2]) = LOBYTE(slideDir[1]);
  }
  if ( *(unsigned int *)(hitTime + 1584) != 4 && g_entities[Trace_GetEntityHitId(trace)].s.eType == 4 )
    HIBYTE(slideDir[2]) = 0;
  if ( HIBYTE(slideDir[2])
    && (*(unsigned int *)(hitTime + 1584) == 1 || *(unsigned int *)(hitTime + 1584) == 2 || trace->normal.vec.v[2] > 0.69999999) )
  {
    ent->s.groundEntityNum = Trace_GetEntityHitId(trace);
    g_entities[ent->s.groundEntityNum].flags |= 0x100000u;
  }
  HIBYTE(slideDir[0]) = 0;
  if ( ((unsigned int)&cls.rankedServers[711].game[35] & ent->s.lerp.eFlags) == 0 )
    goto LABEL_103;
  BYTE2(slideDir[0]) = ent->s.lerp.u.turret.ownerNum != 0;
  HIBYTE(slideDir[0]) = GrenadeBounceVelocity(
                          (const float *)&shouldRoll,
                          v56,
                          trace->normal.vec.v,
                          SLODWORD(velocity[1]),
                          (const WeaponDef *)hitTime,
                          &ent->s.lerp.pos,
                          &v43,
                          v44,
                          SBYTE2(slideDir[0]));
  if ( HIBYTE(slideDir[0]) )
  {
    if ( (double)grenadeBumpFreq->current.value > G_random() )
    {
      mag = grenadeBumpMax->current.value;
      v41 = v43 * grenadeBumpMag->current.value;
      v3 = (float)(v41 - mag) < 0.0 ? v41 : mag;
      v40 = v3;
      *(float *)&sideTrace.hitPartition = v3;
      if ( v3 > (float)(bg_gravity->current.value * 0.050000001) )
      {
        sideTrace.staticModel = (cStaticModel_s *)ent->s.lerp.pos.trDelta;
        *(unsigned int *)&sideTrace.walkable = ent->s.lerp.pos.trDelta;
        ent->s.lerp.pos.trDelta[0] = (float)(*(float *)&sideTrace.hitPartition * trace->normal.vec.v[0])
                                   + ent->s.lerp.pos.trDelta[0];
        *(float *)&sideTrace.staticModel->xmodel = (float)(*(float *)&sideTrace.hitPartition * trace->normal.vec.v[1])
                                                 + *(float *)(*(unsigned int *)&sideTrace.walkable + 4);
        sideTrace.staticModel->origin[0] = (float)(*(float *)&sideTrace.hitPartition * trace->normal.vec.v[2])
                                         + *(float *)(*(unsigned int *)&sideTrace.walkable + 8);
      }
    }
    if ( slideSpeed < 25.0 && trace->normal.vec.v[2] > 0.69999999 )
    {
      v36 = *(unsigned int *)&FLOAT_0_0;
      oldCycle = *(float *)&FLOAT_0_0;
      wobbleFreq = *(float *)&FLOAT_0_0;
      sideTrace.normal.vec.u[0] = *(unsigned int *)&FLOAT_0_0;
      v35 = v43 * grenadeWobbleFreq->current.value;
      wobbleDelta = ent->mover.apos2[2];
      for ( ent->mover.apos2[2] = (float)((float)((float)(v35 * 0.050000001) * 2.0) * 3.1415901) + ent->mover.apos2[2];
            ent->mover.apos2[2] > 6.2831802;
            ent->mover.apos2[2] = ent->mover.apos2[2] - 6.2831802 )
      {
        ;
      }
      v33 = ent->mover.apos2[2];
      __libm_sse2_sin(v5);
      v32 = v33;
      __libm_sse2_sin(v6);
      v31 = v33 - wobbleDelta;
      if ( v43 > 0.0099999998 )
      {
        LODWORD(side[2]) = ent->s.lerp.pos.trDelta;
        side[1] = (float)((float)(v31 * grenadeWobbleFwdMag->current.value) + v43) / v43;
        LODWORD(side[0]) = ent->s.lerp.pos.trDelta;
        ent->s.lerp.pos.trDelta[0] = side[1] * ent->s.lerp.pos.trDelta[0];
        *(float *)(LODWORD(side[2]) + 4) = side[1] * *(float *)(LODWORD(side[0]) + 4);
        *(float *)(LODWORD(side[2]) + 8) = side[1] * *(float *)(LODWORD(side[0]) + 8);
      }
      Vec3Cross(v44, trace->normal.vec.v, &wobbleSideScale);
      v26 = 1.0 - (float)(v43 * grenadeWobbleSideDamp->current.value);
      if ( (float)(0.0 - v26) < 0.0 )
        v25 = v26;
      else
        v25 = *(float *)&FLOAT_0_0;
      wobbleNewPos[2] = v25;
      if ( v25 > 0.0 )
      {
        wobbleNewPos[1] = (float)(v31 * grenadeWobbleSideMag->current.value) * wobbleNewPos[2];
        LODWORD(wobbleNewPos[0]) = ent->r.currentOrigin;
        v23[0] = (float)(wobbleNewPos[1] * wobbleSideScale) + ent->r.currentOrigin[0];
        v23[1] = (float)(wobbleNewPos[1] * v28) + ent->r.currentOrigin[1];
        v23[2] = (float)(wobbleNewPos[1] * v29) + ent->r.currentOrigin[2];
        if ( EntHandle::isDefined(&ent->r.ownerNum) )
          v22 = EntHandle::entnum(&ent->r.ownerNum);
        else
          v22 = 1023;
        G_MissileTrace((trace_t *)&v36, ent->r.currentOrigin, v23, v22, ent->clipmask, ent->s.weapon);
        Vec3Lerp(ent->r.currentOrigin, v23, sideTrace.normal.vec.v[1], ent->r.currentOrigin);
      }
      if ( ent->mover.apos3[0] == 0.0 )
        ent->mover.apos3[0] = G_random() * 2.0 - 1.0;
      linearSpeed = ent->s.lerp.pos.trDelta;
      revolutions = ent->mover.apos3[0] * grenadeCurveMax->current.value;
      LODWORD(targetAngles[2]) = ent->s.lerp.pos.trDelta;
      ent->s.lerp.pos.trDelta[0] = (float)(revolutions * wobbleSideScale) + ent->s.lerp.pos.trDelta[0];
      linearSpeed[1] = (float)(revolutions * v28) + *(float *)(LODWORD(targetAngles[2]) + 4);
      linearSpeed[2] = (float)(revolutions * v29) + *(float *)(LODWORD(targetAngles[2]) + 8);
    }
    targetAngles[1] = Abs(ent->s.lerp.pos.trDelta);
    targetAngles[0] = targetAngles[1] / (float)((float)(3.1415901 * rollRadius) * 2.0);
    vectoangles(ent->s.lerp.pos.trDelta, v18);
    ent->s.lerp.apos.trBase[1] = v18[1];
    ent->s.lerp.apos.trBase[2] = FLOAT_90_0;
    ent->s.lerp.apos.trDelta[0] = 360.0 * targetAngles[0];
    ent->s.lerp.apos.trDelta[1] = *(float *)&FLOAT_0_0;
    ent->s.lerp.apos.trDelta[2] = *(float *)&FLOAT_0_0;
  }
  v17 = ent->r.currentOrigin[0];
  if ( (LODWORD(v17) & 0x7F800000) == 0x7F800000
    || (v16 = ent->r.currentOrigin[1], (LODWORD(v16) & 0x7F800000) == 0x7F800000)
    || (v15 = ent->r.currentOrigin[2], (LODWORD(v15) & 0x7F800000) == 0x7F800000) )
  {
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
            654,
            0,
            "%s",
            "!IS_NAN((ent->r.currentOrigin)[0]) && !IS_NAN((ent->r.currentOrigin)[1]) && !IS_NAN((ent->r.currentOrigin)[2])") )
      __debugbreak();
  }
  vAngles[2] = ent->s.lerp.pos.trDelta[0];
  if ( (LODWORD(vAngles[2]) & 0x7F800000) == 0x7F800000
    || (vAngles[1] = ent->s.lerp.pos.trDelta[1], (LODWORD(vAngles[1]) & 0x7F800000) == 0x7F800000)
    || (vAngles[0] = ent->s.lerp.pos.trDelta[2], (LODWORD(vAngles[0]) & 0x7F800000) == 0x7F800000) )
  {
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
            655,
            0,
            "%s",
            "!IS_NAN((ent->s.lerp.pos.trDelta)[0]) && !IS_NAN((ent->s.lerp.pos.trDelta)[1]) && !IS_NAN((ent->s.lerp.pos.trDelta)[2])") )
      __debugbreak();
  }
  if ( !HIBYTE(slideDir[2])
    || *(unsigned int *)(hitTime + 1584) != 1
    && *(unsigned int *)(hitTime + 1584) != 2
    && (trace->normal.vec.v[2] <= 0.69999999
     || *(unsigned int *)(hitTime + 1584) != 3
     && *(unsigned int *)(hitTime + 1584) != 4
     && grenadeRestThreshold->current.value <= Abs(ent->s.lerp.pos.trDelta)) )
  {
LABEL_103:
    traceEnd[1] = 0.1 * trace->normal.vec.v[0];
    traceEnd[2] = 0.1 * trace->normal.vec.v[1];
    v10 = 0.1 * trace->normal.vec.v[2];
    if ( v10 > 0.0 )
      v10 = *(float *)&FLOAT_0_0;
    LODWORD(traceEnd[0]) = ent->r.currentOrigin;
    v8[0] = ent->r.currentOrigin[0] + traceEnd[1];
    v8[1] = ent->r.currentOrigin[1] + traceEnd[2];
    v8[2] = ent->r.currentOrigin[2] + v10;
    if ( EntHandle::isDefined(&ent->r.ownerNum) )
    {
      v7 = EntHandle::entnum(&ent->r.ownerNum);
      G_MissileTrace((trace_t *)&surfType, ent->r.currentOrigin, v8, v7, ent->clipmask, ent->s.weapon);
    }
    else
    {
      G_MissileTrace((trace_t *)&surfType, ent->r.currentOrigin, v8, 1023, ent->clipmask, ent->s.weapon);
    }
    Vec3Lerp(ent->r.currentOrigin, v8, tempTrace.normal.vec.v[1], ent->r.currentOrigin);
    ent->s.lerp.pos.trBase[0] = ent->r.currentOrigin[0];
    ent->s.lerp.pos.trBase[1] = ent->r.currentOrigin[1];
    ent->s.lerp.pos.trBase[2] = ent->r.currentOrigin[2];
    ent->s.lerp.pos.trTime = level.time;
    if ( HIBYTE(slideDir[0])
      || (float)((float)((float)(ent->s.lerp.pos.trDelta[0] * trace->normal.vec.v[0])
                       + (float)(ent->s.lerp.pos.trDelta[1] * trace->normal.vec.v[1]))
               + (float)(ent->s.lerp.pos.trDelta[2] * trace->normal.vec.v[2])) <= 25.0 )
    {
      goto LABEL_117;
    }
    if ( *(unsigned int *)(hitTime + 1588) )
    {
      if ( *(unsigned int *)(hitTime + 1588) == 1 )
      {
        MissileLandAngles(ent, trace, &vDelta[2], 0, 1);
LABEL_116:
        ent->s.lerp.apos.trBase[0] = vDelta[2];
        LODWORD(ent->s.lerp.apos.trBase[1]) = groundEnt;
        ent->s.lerp.apos.trBase[2] = v13;
        ent->s.lerp.apos.trTime = level.time;
LABEL_117:
        if ( LODWORD(velocity[2]) )
          return 0;
        *(float *)&shouldRoll = ent->s.lerp.pos.trDelta[0] - *(float *)&shouldRoll;
        v58 = ent->s.lerp.pos.trDelta[1] - v58;
        dot = ent->s.lerp.pos.trDelta[2] - dot;
        v56 = Abs((const float *)&shouldRoll);
        return v56 > 100.0;
      }
      if ( *(unsigned int *)(hitTime + 1588) == 2 )
      {
        MissileLandAngles(ent, trace, &vDelta[2], 1, 0);
        goto LABEL_116;
      }
    }
    MissileLandAngles(ent, trace, &vDelta[2], 0, 0);
    goto LABEL_116;
  }
  G_SetOrigin(ent, ent->r.currentOrigin);
  if ( *(unsigned int *)(hitTime + 1584) == 4 || *(unsigned int *)(hitTime + 1584) == 2 )
  {
    MissileLandAnglesFlatMaintainingDirection(ent, trace, &vDelta[2]);
  }
  else if ( *(unsigned int *)(hitTime + 1584) == 3 )
  {
    MissileLandAnglesFlat(ent, trace, &vDelta[2]);
  }
  else
  {
    MissileLandAngles(ent, trace, &vDelta[2], 1, 0);
  }
  G_SetAngle(ent, &vDelta[2]);
  LODWORD(vDelta[1]) = &ent->mover.midTime;
  ent->mover.midTime = trace->normal.vec.v[0];
  *(float *)(LODWORD(vDelta[1]) + 4) = trace->normal.vec.v[1];
  *(float *)(LODWORD(vDelta[1]) + 8) = trace->normal.vec.v[2];
  if ( !*(_BYTE *)(hitTime + 1594) )
    ent->nextthink = 0;
  CheckGrenadeDanger(ent);
  if ( ent->s.groundEntityNum != 1023 && ent->s.groundEntityNum != 1022 )
  {
    if ( ent->s.groundEntityNum >= 1024
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
            686,
            0,
            "%s",
            "ent->s.groundEntityNum < MAX_GENTITIES") )
    {
      __debugbreak();
    }
    if ( ent->s.groundEntityNum < 1024 )
    {
      LODWORD(vDelta[0]) = &g_entities[ent->s.groundEntityNum];
      if ( !LODWORD(vDelta[0])
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 690, 0, "%s", "groundEnt") )
      {
        __debugbreak();
      }
      if ( LODWORD(vDelta[0]) )
      {
        if ( *(_WORD *)(LODWORD(vDelta[0]) + 190) == 6 )
        {
          ent->flags |= 0x1000u;
          G_EntLinkTo(ent, (gentity_s *)LODWORD(vDelta[0]), 0);
        }
      }
    }
  }
  Scr_Notify(ent, scr_const.stationary, 0);
  return 1;
}

void __cdecl MissileLandAngles(gentity_s *ent, trace_t *trace, float *vAngles, int bForceAlign, int bKeepSpeed)
{
  double v5; // st7
  double v6; // st7
  char v7; // al
  int hitTime; // [esp+1Ch] [ebp-28h]
  float fSurfacePitch; // [esp+20h] [ebp-24h]
  float fAbsAngDelta; // [esp+24h] [ebp-20h]
  const WeaponDef *weapDef; // [esp+30h] [ebp-14h]
  float fAngleDelta; // [esp+40h] [ebp-4h]

  weapDef = BG_GetWeaponDef(ent->s.weapon);
  hitTime = level.previousTime + (int)(float)((float)(level.time - level.previousTime) * trace->fraction);
  BG_EvaluateTrajectory(&ent->s.lerp.apos, hitTime, vAngles);
  if ( !grenadeRollingEnabled->current.enabled || !weapDef->bKeepRolling || ent->s.lerp.u.turret.ownerNum )
  {
    if ( trace->normal.vec.v[2] <= 0.1 )
    {
      if ( !bForceAlign && !bKeepSpeed )
      {
        v7 = G_rand();
        ent->s.lerp.apos.trDelta[0] = AngleNormalize360((float)((v7 & 0x7F) - 63) + ent->s.lerp.apos.trDelta[0]);
      }
    }
    else
    {
      fSurfacePitch = PitchForYawOnNormal(vAngles[1], trace->normal.vec.v);
      fAngleDelta = AngleNormalize180(fSurfacePitch - *vAngles);
      LODWORD(fAbsAngDelta) = LODWORD(fAngleDelta) & _mask__AbsFloat_;
      if ( !bForceAlign || bKeepSpeed )
      {
        ent->s.lerp.apos.trBase[0] = *vAngles;
        ent->s.lerp.apos.trBase[1] = vAngles[1];
        ent->s.lerp.apos.trBase[2] = vAngles[2];
        ent->s.lerp.apos.trTime = hitTime;
        if ( fAbsAngDelta >= 80.0 )
        {
          if ( !bKeepSpeed )
            ent->s.lerp.apos.trDelta[0] = (G_random() * 0.30000001 + 0.85000002) * ent->s.lerp.apos.trDelta[0];
        }
        else if ( bKeepSpeed )
        {
          ent->s.lerp.apos.trDelta[0] = ent->s.lerp.apos.trDelta[0] * -1.0;
        }
        else
        {
          ent->s.lerp.apos.trDelta[0] = (G_random() * 0.30000001 + 0.85000002) * ent->s.lerp.apos.trDelta[0] * -1.0;
        }
      }
      *vAngles = AngleNormalize180(*vAngles);
      if ( bForceAlign || fAbsAngDelta < 45.0 )
      {
        if ( COERCE_FLOAT(*(unsigned int *)vAngles & _mask__AbsFloat_) <= 90.0 )
          v5 = AngleNormalize360(fSurfacePitch);
        else
          v5 = AngleNormalize360(fSurfacePitch + 180.0);
        *vAngles = v5;
      }
      else
      {
        if ( fAbsAngDelta >= 80.0 )
          v6 = AngleNormalize360(*vAngles);
        else
          v6 = AngleNormalize360((float)(fAngleDelta * 0.25) + *vAngles);
        *vAngles = v6;
      }
    }
    if ( bForceAlign && weapDef->rotateType == WEAPROTATE_CYLINDER_ROTATE )
      vAngles[2] = FLOAT_90_0;
    if ( bForceAlign && !bKeepSpeed && weapDef->rotateType == WEAPROTATE_BLADE_ROTATE )
      vAngles[2] = FLOAT_90_0;
  }
}

void __cdecl MissileLandAnglesFlat(gentity_s *ent, trace_t *trace, float *angles)
{
  double v3; // xmm0_8
  long double hitTime; // [esp+0h] [ebp-24h]
  float right[3]; // [esp+8h] [ebp-1Ch] BYREF
  float normalRightComponent; // [esp+14h] [ebp-10h]
  float up[3]; // [esp+18h] [ebp-Ch] BYREF

  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 284, 0, "%s", "ent") )
    __debugbreak();
  if ( !trace && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 285, 0, "%s", "trace") )
    __debugbreak();
  LODWORD(hitTime) = level.previousTime + (int)(float)((float)(level.time - level.previousTime) * trace->fraction);
  BG_EvaluateTrajectory(&ent->s.lerp.apos, SLODWORD(hitTime), angles);
  NearestPitchAndYawOnPlane(angles, trace->normal.vec.v, angles);
  if ( angles[2] != 0.0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 294, 0, "%s", "angles[ROLL] == 0.f") )
  {
    __debugbreak();
  }
  AngleVectors(angles, 0, right, up);
  normalRightComponent = (float)((float)(right[0] * trace->normal.vec.v[0]) + (float)(right[1] * trace->normal.vec.v[1]))
                       + (float)(right[2] * trace->normal.vec.v[2]);
  *((float *)&hitTime + 1) = (float)((float)(up[0] * trace->normal.vec.v[0]) + (float)(up[1] * trace->normal.vec.v[1]))
                           + (float)(up[2] * trace->normal.vec.v[2]);
  v3 = normalRightComponent;
  __libm_sse2_atan2(hitTime, *(long double *)right);
  *(float *)&v3 = v3;
  angles[2] = (float)(*(float *)&v3 * 180.0) / 3.1415927;
}

void __cdecl MissileLandAnglesFlatMaintainingDirection(gentity_s *ent, trace_t *trace, float *angles)
{
  double v3; // xmm0_8
  long double hitTime; // [esp+8h] [ebp-24h]
  float right[3]; // [esp+10h] [ebp-1Ch] BYREF
  float normalRightComponent; // [esp+1Ch] [ebp-10h]
  float up[3]; // [esp+20h] [ebp-Ch] BYREF

  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 310, 0, "%s", "ent") )
    __debugbreak();
  if ( !trace && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 311, 0, "%s", "trace") )
    __debugbreak();
  LODWORD(hitTime) = level.previousTime + (int)(float)((float)(level.time - level.previousTime) * trace->fraction);
  BG_EvaluateTrajectory(&ent->s.lerp.apos, SLODWORD(hitTime), angles);
  *angles = PitchForYawOnNormal(angles[1], trace->normal.vec.v);
  angles[2] = *(float *)&FLOAT_0_0;
  AngleVectors(angles, 0, right, up);
  normalRightComponent = (float)((float)(right[0] * trace->normal.vec.v[0]) + (float)(right[1] * trace->normal.vec.v[1]))
                       + (float)(right[2] * trace->normal.vec.v[2]);
  *((float *)&hitTime + 1) = (float)((float)(up[0] * trace->normal.vec.v[0]) + (float)(up[1] * trace->normal.vec.v[1]))
                           + (float)(up[2] * trace->normal.vec.v[2]);
  v3 = normalRightComponent;
  __libm_sse2_atan2(hitTime, *(long double *)right);
  *(float *)&v3 = v3;
  angles[2] = (float)(*(float *)&v3 * 180.0) / 3.1415927;
}

void __cdecl CheckGrenadeDanger(gentity_s *grenadeEnt)
{
  char *v1; // eax
  gentity_s *v2; // eax
  float damageRadiusSquared; // [esp+Ch] [ebp-10h]
  gentity_s *ent; // [esp+10h] [ebp-Ch]
  int i; // [esp+14h] [ebp-8h]
  const WeaponDef *weapDef; // [esp+18h] [ebp-4h]

  if ( !grenadeEnt
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 382, 0, "%s", "grenadeEnt") )
  {
    __debugbreak();
  }
  if ( !grenadeEnt->s.weapon
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          383,
          0,
          "%s",
          "grenadeEnt->s.weapon != WP_NONE") )
  {
    __debugbreak();
  }
  weapDef = BG_GetWeaponDef(grenadeEnt->s.weapon);
  if ( !weapDef && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 385, 0, "%s", "weapDef") )
    __debugbreak();
  damageRadiusSquared = (float)weapDef->iExplosionRadius * (float)weapDef->iExplosionRadius;
  ent = g_entities;
  for ( i = 0; i < level.maxclients; ++i )
  {
    if ( ent->r.inuse )
    {
      if ( !ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 393, 0, "%s", "ent->client") )
      {
        __debugbreak();
      }
      if ( G_WithinDamageRadius(grenadeEnt->r.currentOrigin, damageRadiusSquared, ent) )
      {
        v1 = (char *)BG_WeaponName(grenadeEnt->s.weapon);
        Scr_AddString(v1, SCRIPTINSTANCE_SERVER);
        if ( EntHandle::isDefined(&grenadeEnt->parent) )
        {
          v2 = EntHandle::ent(&grenadeEnt->parent);
          Scr_AddEntity(v2, SCRIPTINSTANCE_SERVER);
        }
        else
        {
          Scr_AddUndefined(SCRIPTINSTANCE_SERVER);
        }
        Scr_AddEntity(grenadeEnt, SCRIPTINSTANCE_SERVER);
        Scr_Notify(ent, scr_const.grenadedanger, 3u);
      }
    }
    ++ent;
  }
}

char __cdecl GrenadeBounceVelocity(
        const float *preBounceVelocity,
        float dot,
        const float *normal,
        int surfType,
        const WeaponDef *weapDef,
        trajectory_t *pos,
        float *rollSlideSpeed,
        float *rollSlideDir,
        bool isDud)
{
  float v10; // [esp+0h] [ebp-88h]
  float value; // [esp+4h] [ebp-84h]
  bool v12; // [esp+8h] [ebp-80h]
  float v13; // [esp+1Ch] [ebp-6Ch]
  float v14; // [esp+24h] [ebp-64h]
  float v15; // [esp+44h] [ebp-44h]
  float slideDir[3]; // [esp+5Ch] [ebp-2Ch] BYREF
  float slideSpeed; // [esp+68h] [ebp-20h]
  float v18; // [esp+6Ch] [ebp-1Ch]
  float frictionScale; // [esp+70h] [ebp-18h]
  float tr_n; // [esp+74h] [ebp-14h]
  float per; // [esp+78h] [ebp-10h]
  float par; // [esp+7Ch] [ebp-Ch]
  float bounceFactor; // [esp+80h] [ebp-8h]
  bool shouldRoll; // [esp+87h] [ebp-1h]

  bounceFactor = Abs(preBounceVelocity);
  if ( bounceFactor <= 0.0 || dot > 0.0 )
    goto LABEL_30;
  v12 = grenadeRollingEnabled->current.enabled && weapDef->isRollingGrenade;
  shouldRoll = v12;
  if ( !v12 || isDud )
  {
    par = weapDef->parallelBounce[surfType];
    per = weapDef->perpendicularBounce[surfType];
    if ( weapDef->bKeepRolling && !isDud && grenadeRollThreshold > Abs(pos->trDelta) )
      par = (float)(par * rollFrac_0) + par;
    tr_n = (float)((float)(pos->trDelta[0] * *normal) + (float)(pos->trDelta[1] * normal[1]))
         + (float)(pos->trDelta[2] * normal[2]);
    v15 = (float)(per - par) * tr_n;
    pos->trDelta[0] = (float)(v15 * *normal) + (float)(par * pos->trDelta[0]);
    pos->trDelta[1] = (float)(v15 * normal[1]) + (float)(par * pos->trDelta[1]);
    pos->trDelta[2] = (float)(v15 * normal[2]) + (float)(par * pos->trDelta[2]);
    goto LABEL_30;
  }
  slideSpeed = Vec3NormalizeTo(pos->trDelta, slideDir);
  if ( slideSpeed <= grenadeFrictionThresh->current.value )
    value = grenadeFrictionLow->current.value;
  else
    value = grenadeFrictionHigh->current.value;
  frictionScale = 1.0 - value;
  pos->trDelta[0] = (float)(1.0 - value) * pos->trDelta[0];
  pos->trDelta[1] = frictionScale * pos->trDelta[1];
  pos->trDelta[2] = frictionScale * pos->trDelta[2];
  if ( COERCE_FLOAT(LODWORD(dot) & _mask__AbsFloat_) >= 25.0 )
  {
    v18 = 1.0 - (float)((float)(1.0 - weapDef->perpendicularBounce[surfType]) * normal[2]);
    if ( (float)(grenadeBounceRestitutionMax->current.value - weapDef->perpendicularBounce[surfType]) < 0.0 )
      v14 = weapDef->perpendicularBounce[surfType];
    else
      v14 = grenadeBounceRestitutionMax->current.value;
    if ( (float)(v14 - v18) < 0.0 )
      v10 = v14;
    else
      v10 = v18;
    v18 = v10;
    v13 = COERCE_FLOAT(LODWORD(v10) ^ _mask__NegFloat_) * dot;
    pos->trDelta[0] = (float)(v13 * *normal) + pos->trDelta[0];
    pos->trDelta[1] = (float)(v13 * normal[1]) + pos->trDelta[1];
    pos->trDelta[2] = (float)(v13 * normal[2]) + pos->trDelta[2];
LABEL_30:
    if ( ((LODWORD(pos->trDelta[0]) & 0x7F800000) == 0x7F800000
       || (LODWORD(pos->trDelta[1]) & 0x7F800000) == 0x7F800000
       || (LODWORD(pos->trDelta[2]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
            511,
            0,
            "%s",
            "!IS_NAN((pos->trDelta)[0]) && !IS_NAN((pos->trDelta)[1]) && !IS_NAN((pos->trDelta)[2])") )
    {
      __debugbreak();
    }
    return 0;
  }
  if ( rollSlideSpeed )
    *rollSlideSpeed = slideSpeed;
  if ( rollSlideDir )
  {
    *rollSlideDir = slideDir[0];
    rollSlideDir[1] = slideDir[1];
    rollSlideDir[2] = slideDir[2];
  }
  return 1;
}

bool __cdecl JavelinProjectile(gentity_s *ent, const WeaponDef *weapDef)
{
  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 770, 0, "%s", "ent") )
    __debugbreak();
  if ( !weapDef && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 771, 0, "%s", "weapDef") )
    __debugbreak();
  return ent->s.eType == 4 && weapDef->guidedMissileType == MISSILE_GUIDANCE_JAVELIN;
}

bool __cdecl isDud(gentity_s *ent, const WeaponDef *weapDef)
{
  return ForcedDud(ent) || GrenadeDud(ent, weapDef) || JavelinDud(ent, weapDef);
}

bool __cdecl GrenadeDud(gentity_s *ent, const WeaponDef *weapDef)
{
  bool dud; // [esp+7h] [ebp-1h]

  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 751, 0, "%s", "ent") )
    __debugbreak();
  if ( !weapDef && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 752, 0, "%s", "weapDef") )
    __debugbreak();
  dud = 0;
  if ( weapDef->iProjectileActivateDist > 0 )
    return (float)weapDef->iProjectileActivateDist > ent->mover.aSpeed;
  return dud;
}

bool __cdecl JavelinDud(gentity_s *ent, const WeaponDef *weapDef)
{
  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 784, 0, "%s", "ent") )
    __debugbreak();
  if ( !weapDef && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 785, 0, "%s", "weapDef") )
    __debugbreak();
  return JavelinProjectile(ent, weapDef) && ent->missile.missile.stage == MISSILESTAGE_SOFTLAUNCH;
}

void __cdecl AttachBoltGrenade(gentity_s *ent, gentity_s *hitEnt, trace_t *trace, hitLocation_t hitLocation)
{
  unsigned int LowercaseString; // eax
  unsigned int tagName; // [esp+0h] [ebp-Ch] BYREF
  unsigned int boneName; // [esp+4h] [ebp-8h] BYREF
  int ownerIndex; // [esp+8h] [ebp-4h]

  ent->flags |= 0x1000u;
  ent->s.lerp.pos.trType = 0;
  ownerIndex = 0;
  if ( EntHandle::isDefined(&ent->parent) && EntHandle::ent(&ent->parent)->client )
    ownerIndex = EntHandle::ent(&ent->parent)->client - level.clients;
  if ( (unsigned int)ownerIndex >= level.maxclients
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          887,
          0,
          "ownerIndex doesn't index level.maxclients\n\t%i not in [0, %i)",
          ownerIndex,
          level.maxclients) )
  {
    __debugbreak();
  }
  AssignToSmallerType<unsigned char>(&ent->s.faction.iHeadIconTeam, ent->missile.team | (4 * ownerIndex));
  if ( hitEnt->client )
  {
    boneName = 0;
    MapHitLocationToRagdollBoneName(hitLocation, &boneName);
    G_EntLinkTo(ent, hitEnt, boneName);
    Scr_AddEntity(hitEnt, SCRIPTINSTANCE_SERVER);
    Scr_Notify(ent, scr_const.stuck_to_player, 1u);
  }
  else if ( ent->actor )
  {
    tagName = 0;
    MapHitLocationToRagdollBoneName(hitLocation, &tagName);
    if ( !G_EntLinkTo(ent, hitEnt, tagName) )
    {
      LowercaseString = SL_FindLowercaseString("j_knee_ri", SCRIPTINSTANCE_SERVER);
      G_EntLinkTo(ent, hitEnt, LowercaseString);
    }
  }
  else if ( hitEnt->scr_vehicle )
  {
    if ( !G_EntLinkTo(ent, hitEnt, trace->partName) )
      G_EntLinkTo(ent, hitEnt, 0);
  }
  else
  {
    G_EntLinkTo(ent, hitEnt, 0);
  }
  GScr_AddEntity(ent);
  Scr_Notify(hitEnt, scr_const.grenade_stuck, 1u);
  Scr_AddVector(trace->normal.vec.v, SCRIPTINSTANCE_SERVER);
  Scr_AddVector(ent->r.currentOrigin, SCRIPTINSTANCE_SERVER);
  Scr_Notify(ent, scr_const.stationary, 2u);
}

void __cdecl AttachMissileToEntity(
        gentity_s *missile,
        unsigned int entnum,
        unsigned int boneName,
        int hitLocation,
        float *velocity)
{
  float AttachedMissileOffset; // [esp+0h] [ebp-44h]
  const WeaponDef *weapDef; // [esp+1Ch] [ebp-28h]
  float dir[3]; // [esp+20h] [ebp-24h] BYREF
  float bonePos[3]; // [esp+2Ch] [ebp-18h] BYREF
  float offset[3]; // [esp+38h] [ebp-Ch] BYREF

  if ( !missile && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 974, 0, "%s", "missile") )
    __debugbreak();
  if ( entnum >= 0x400
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          975,
          0,
          "%s",
          "entnum >= 0 && entnum < MAX_GENTITIES") )
  {
    __debugbreak();
  }
  if ( entnum <= 0x3FE && boneName )
  {
    weapDef = BG_GetWeaponDef(missile->s.weapon);
    if ( !weapDef
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 982, 0, "%s", "weapDef") )
    {
      __debugbreak();
    }
    if ( weapDef && weapDef->rotateType == WEAPROTATE_BLADE_ROTATE )
    {
      G_DObjGetWorldTagPos(&g_entities[entnum], boneName, bonePos);
      dir[0] = *velocity;
      dir[1] = velocity[1];
      dir[2] = *(float *)&FLOAT_0_0;
      Vec3Normalize(dir);
      missile->r.currentAngles[0] = *(float *)&FLOAT_0_0;
      missile->r.currentAngles[1] = *(float *)&FLOAT_0_0;
      missile->r.currentAngles[2] = *(float *)&FLOAT_0_0;
      missile->r.currentAngles[1] = vectoyaw(dir);
      missile->s.lerp.apos.trBase[0] = missile->r.currentAngles[0];
      missile->s.lerp.apos.trBase[1] = missile->r.currentAngles[1];
      missile->s.lerp.apos.trBase[2] = missile->r.currentAngles[2];
      if ( hitLocation )
      {
        LODWORD(dir[0]) ^= _mask__NegFloat_;
        LODWORD(dir[1]) ^= _mask__NegFloat_;
        LODWORD(dir[2]) ^= _mask__NegFloat_;
        AttachedMissileOffset = GetAttachedMissileOffset(hitLocation);
        offset[0] = (float)(AttachedMissileOffset * dir[0]) + bonePos[0];
        offset[1] = (float)(AttachedMissileOffset * dir[1]) + bonePos[1];
        offset[2] = (float)(AttachedMissileOffset * dir[2]) + bonePos[2];
        G_SetOrigin(missile, offset);
      }
    }
  }
}

double __cdecl GetAttachedMissileOffset(int hitlocation)
{
  double result; // st7

  switch ( hitlocation )
  {
    case 1:
    case 2:
    case 3:
      result = 5.0;
      break;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
      result = 2.0;
      break;
    default:
      result = 7.5;
      break;
  }
  return result;
}

int __cdecl StickMissile(
        gentity_s *ent,
        gentity_s *other,
        const WeaponDef *weapDef,
        trace_t *trace,
        hitLocation_t hitLocation,
        float *velocity)
{
  unsigned int LowercaseString; // eax
  unsigned int v8; // eax
  unsigned int tagName; // [esp+0h] [ebp-18h] BYREF
  team_t eTeam; // [esp+4h] [ebp-14h]
  unsigned int boneName; // [esp+8h] [ebp-10h] BYREF
  int team; // [esp+Ch] [ebp-Ch]
  int entnum; // [esp+10h] [ebp-8h]
  int isStuck; // [esp+14h] [ebp-4h]

  isStuck = 0;
  if ( other->client )
  {
    team = other->client->sess.cs.team;
    if ( team == ent->missile.team && !weapDef->timedDetonation && team )
      return isStuck;
    entnum = other->s.number;
    if ( other->client->ps.pm_type == 9
      && (int)g_scr_data.actorXAnimTrees[376 * other->client->ps.corpseIndex - 1495] >= 0 )
    {
      entnum = (int)g_scr_data.actorXAnimTrees[376 * other->client->ps.corpseIndex - 1495];
    }
    ent->flags |= 0x1000u;
    boneName = 0;
    MapHitLocationToRagdollBoneName(hitLocation, &boneName);
    AttachMissileToEntity(ent, entnum, boneName, hitLocation, velocity);
    if ( other->client->ps.pm_type == 9 )
      G_EntLinkTo(ent, &g_entities[entnum], boneName);
    else
      G_EntLinkTo(ent, other, boneName);
    ent->s.lerp.pos.trType = 0;
    Scr_AddEntity(other, SCRIPTINSTANCE_SERVER);
    Scr_Notify(ent, scr_const.stuck_to_player, 1u);
    return 1;
  }
  else if ( other->actor )
  {
    eTeam = other->actor->sentient->eTeam;
    if ( eTeam == ent->missile.team && !weapDef->timedDetonation && eTeam )
      return isStuck;
    ent->flags |= 0x1000u;
    tagName = 0;
    MapHitLocationToRagdollBoneName(hitLocation, &tagName);
    AttachMissileToEntity(ent, other->s.number, tagName, hitLocation, velocity);
    if ( !G_EntLinkTo(ent, other, tagName) )
    {
      LowercaseString = SL_FindLowercaseString("j_knee_ri", SCRIPTINSTANCE_SERVER);
      AttachMissileToEntity(ent, other->s.number, LowercaseString, 12, velocity);
      v8 = SL_FindLowercaseString("j_knee_ri", SCRIPTINSTANCE_SERVER);
      G_EntLinkTo(ent, other, v8);
    }
    ent->s.lerp.pos.trType = 0;
    return 1;
  }
  else if ( other->scr_vehicle && weapDef->stickiness != WEAPSTICKINESS_FLESH )
  {
    ent->flags |= 0x1000u;
    if ( !G_EntLinkTo(ent, other, trace->partName) )
      G_EntLinkTo(ent, other, 0);
    ent->s.lerp.pos.trType = 0;
    return 1;
  }
  return isStuck;
}

bool __cdecl isBounceProjectile(gentity_s *ent)
{
  const WeaponDef *weapDef; // [esp+0h] [ebp-4h]

  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 1095, 0, "%s", "ent") )
    __debugbreak();
  if ( ent->s.eType != 4
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          1096,
          0,
          "%s",
          "ent->s.eType == ET_MISSILE") )
  {
    __debugbreak();
  }
  if ( !ent->s.weapon
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 1097, 0, "%s", "ent->s.weapon") )
  {
    __debugbreak();
  }
  weapDef = BG_GetWeaponDef(ent->s.weapon);
  return ((unsigned int)&cls.rankedServers[711].game[35] & ent->s.lerp.eFlags) != 0
      || weapDef->stickiness != WEAPSTICKINESS_ALL && weapDef->stickiness != WEAPSTICKINESS_FLESH;
}

void __cdecl createRetrieveableProjectile(
        gentity_s *ent,
        gentity_s *other,
        const WeaponDef *weapDef,
        trace_t *trace,
        hitLocation_t hitLocation,
        float *dir,
        float *endpos)
{
  gentity_s *v7; // eax
  unsigned int boneName; // [esp+4h] [ebp-14h] BYREF
  float angles[3]; // [esp+8h] [ebp-10h] BYREF
  gentity_s *target_ent; // [esp+14h] [ebp-4h]

  target_ent = 0;
  boneName = 0;
  if ( weapDef->stickiness )
  {
    MapHitLocationToRagdollBoneName(hitLocation, &boneName);
    if ( other->client )
    {
      target_ent = other;
      if ( other->client->ps.pm_type == 9 )
        target_ent = (gentity_s *)(sv.bpsWindow[8]
                                 + (int)g_scr_data.actorXAnimTrees[376 * other->client->ps.corpseIndex - 1495]
                                 * sv.bpsWindow[9]);
    }
    else if ( other->actor )
    {
      target_ent = other;
      if ( SV_DObjGetBoneIndex(other, boneName) < 0 )
        boneName = SL_FindLowercaseString("j_knee_ri", SCRIPTINSTANCE_SERVER);
    }
    else if ( other && other->s.number != 1022 )
    {
      target_ent = other;
    }
  }
  if ( boneName )
    Scr_AddConstString(boneName, SCRIPTINSTANCE_SERVER);
  else
    Scr_AddString((char *)&toastPopupTitle, SCRIPTINSTANCE_SERVER);
  if ( target_ent && target_ent->r.inuse )
    Scr_AddEntity(target_ent, SCRIPTINSTANCE_SERVER);
  else
    Scr_AddUndefined(SCRIPTINSTANCE_SERVER);
  if ( EntHandle::isDefined(&ent->r.ownerNum) )
  {
    v7 = EntHandle::ent(&ent->r.ownerNum);
    Scr_AddEntity(v7, SCRIPTINSTANCE_SERVER);
  }
  else
  {
    Scr_AddUndefined(SCRIPTINSTANCE_SERVER);
  }
  vectoangles(dir, angles);
  Scr_AddVector(angles, SCRIPTINSTANCE_SERVER);
  Scr_AddVector(trace->normal.vec.v, SCRIPTINSTANCE_SERVER);
  Scr_AddVector(endpos, SCRIPTINSTANCE_SERVER);
  Scr_Notify(ent, scr_const.stationary, 6u);
}

void __cdecl Missile_PenetrateGlass(
        trace_t *results,
        gentity_s *ent,
        const float *start,
        const float *end,
        int damage,
        bool predicted)
{
  int passEntityNum; // [esp+0h] [ebp-28h]
  gentity_s *v7; // [esp+4h] [ebp-24h]
  int contents; // [esp+8h] [ebp-20h]
  float vel[3]; // [esp+Ch] [ebp-1Ch] BYREF
  gentity_s *owner; // [esp+18h] [ebp-10h]
  gentity_s *hitEnt; // [esp+1Ch] [ebp-Ch]
  hitLocation_t hitLoc; // [esp+20h] [ebp-8h]
  unsigned __int16 hitEntId; // [esp+24h] [ebp-4h]

  if ( !results && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 2032, 0, "%s", "results") )
    __debugbreak();
  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 2033, 0, "%s", "ent") )
    __debugbreak();
  if ( !start && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 2034, 0, "%s", "start") )
    __debugbreak();
  if ( !end && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 2035, 0, "%s", "end") )
    __debugbreak();
  hitEntId = Trace_GetEntityHitId(results);
  if ( hitEntId < 0x3FEu )
  {
    hitEnt = &g_entities[hitEntId];
    if ( hitEnt->takedamage )
    {
      if ( damage )
      {
        BG_EvaluateTrajectoryDelta(&ent->s.lerp.pos, level.time, vel);
        if ( (float)((float)((float)(vel[0] * vel[0]) + (float)(vel[1] * vel[1])) + (float)(vel[2] * vel[2])) >= 1.0 )
        {
          if ( !predicted )
          {
            if ( EntHandle::isDefined(&ent->r.ownerNum) )
              v7 = EntHandle::ent(&ent->r.ownerNum);
            else
              v7 = 0;
            owner = v7;
            hitLoc = results->partGroup;
            if ( hitEnt->destructible && damage < 1000 )
              damage = 1000;
            G_Damage(
              hitEnt,
              ent,
              owner,
              vel,
              ent->r.currentOrigin,
              damage,
              0,
              16,
              ent->s.weapon,
              hitLoc,
              results->modelIndex,
              results->partName,
              0);
          }
          contents = hitEnt->r.contents;
          hitEnt->r.contents = 0;
          if ( EntHandle::isDefined(&ent->r.ownerNum) )
          {
            passEntityNum = EntHandle::entnum(&ent->r.ownerNum);
            G_MissileTrace(results, start, end, passEntityNum, ent->clipmask, ent->s.weapon);
          }
          else
          {
            G_MissileTrace(results, start, end, 1023, ent->clipmask, ent->s.weapon);
          }
          hitEnt->r.contents = contents;
        }
      }
    }
  }
}

void __cdecl DrawMissileDebug(float *start, float *end)
{
  float color[4]; // [esp+0h] [ebp-10h] BYREF

  color[0] = FLOAT_1_0;
  color[1] = *(float *)&FLOAT_0_0;
  color[2] = *(float *)&FLOAT_0_0;
  color[3] = FLOAT_1_0;
  if ( g_debugBullets->current.integer >= 5 || missileDebugDraw->current.enabled )
    CG_DebugLine(start, end, color, 1, 500);
}

void __cdecl RunMissile_Destabilize(gentity_s *missile)
{
  double v1; // st7
  double v2; // st7
  float iProjectileSpeed; // [esp+20h] [ebp-40h]
  float perturbationMax; // [esp+30h] [ebp-30h]
  const WeaponDef *weaponDef; // [esp+34h] [ebp-2Ch]
  float newAngleAccel[3]; // [esp+38h] [ebp-28h]
  float direction[3]; // [esp+44h] [ebp-1Ch] BYREF
  int axis; // [esp+50h] [ebp-10h]
  float newAPos[3]; // [esp+54h] [ebp-Ch] BYREF
  int savedregs; // [esp+60h] [ebp+0h] BYREF

  if ( !missile && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 2428, 0, "%s", "missile") )
    __debugbreak();
  if ( missile->s.eType != 4
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          2429,
          0,
          "%s",
          "missile->s.eType == ET_MISSILE") )
  {
    __debugbreak();
  }
  if ( (missile->flags & 0x20000) != 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          2430,
          0,
          "%s",
          "!(missile->flags & FL_STABLE_MISSILES)") )
  {
    __debugbreak();
  }
  weaponDef = BG_GetWeaponDef(missile->s.weapon);
  if ( missile->s.lerp.pos.trTime + (int)missile->mover.aDecelTime >= level.time )
  {
    if ( (missile->flags & 0x10000) == 0 )
      return;
  }
  else
  {
    perturbationMax = RunMissile_GetPerturbation(weaponDef->destabilizationCurvatureMax);
    if ( (missile->flags & 0x10000) != 0 )
    {
      for ( axis = 0; axis < 3; ++axis )
      {
        v1 = G_flrand(0.0, 1.0);
        newAngleAccel[axis] = v1 * perturbationMax;
      }
      if ( missile->mover.pos3[0] < 0.0 )
        LODWORD(newAngleAccel[0]) ^= _mask__NegFloat_;
      if ( missile->mover.pos2[2] > 0.0 )
        LODWORD(newAngleAccel[1]) ^= _mask__NegFloat_;
    }
    else
    {
      for ( axis = 0; axis < 3; ++axis )
      {
        v2 = G_flrand(-1.0, 1.0);
        newAngleAccel[axis] = v2 * perturbationMax;
      }
    }
    *(spawner_ent_t *)((char *)&missile->spawner + 44) = *(spawner_ent_t *)newAngleAccel;
    missile->mover.pos3[1] = newAngleAccel[2];
    missile->s.lerp.pos.trTime = level.time;
    missile->mover.aDecelTime = weaponDef->destabilizationRateTime * 1000.0;
    missile->flags |= 0x10000u;
  }
  newAPos[0] = (float)(0.050000001 * missile->mover.pos2[2]) + missile->s.lerp.apos.trBase[0];
  newAPos[1] = (float)(0.050000001 * missile->mover.pos3[0]) + missile->s.lerp.apos.trBase[1];
  newAPos[2] = (float)(0.050000001 * missile->mover.pos3[1]) + missile->s.lerp.apos.trBase[2];
  G_SetAngle(missile, newAPos);
  AngleVectors(newAPos, direction, 0, 0);
  iProjectileSpeed = (float)weaponDef->iProjectileSpeed;
  missile->s.lerp.pos.trDelta[0] = iProjectileSpeed * direction[0];
  missile->s.lerp.pos.trDelta[1] = iProjectileSpeed * direction[1];
  missile->s.lerp.pos.trDelta[2] = iProjectileSpeed * direction[2];
  if ( ((LODWORD(missile->s.lerp.pos.trDelta[0]) & 0x7F800000) == 0x7F800000
     || (LODWORD(missile->s.lerp.pos.trDelta[1]) & 0x7F800000) == 0x7F800000
     || (LODWORD(missile->s.lerp.pos.trDelta[2]) & 0x7F800000) == 0x7F800000)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          2472,
          0,
          "%s",
          "!IS_NAN((missile->s.lerp.pos.trDelta)[0]) && !IS_NAN((missile->s.lerp.pos.trDelta)[1]) && !IS_NAN((missile->s."
          "lerp.pos.trDelta)[2])") )
  {
    __debugbreak();
  }
  missile->s.lerp.pos.trBase[0] = missile->r.currentOrigin[0];
  missile->s.lerp.pos.trBase[1] = missile->r.currentOrigin[1];
  missile->s.lerp.pos.trBase[2] = missile->r.currentOrigin[2];
  missile->s.lerp.pos.trType = 1;
  missile->s.lerp.apos.trType = 1;
  Missile_ApplyAttractorsRepulsors(COERCE_FLOAT(&savedregs), missile);
}

double __cdecl RunMissile_GetPerturbation(float destabilizationCurvatureMax)
{
  if ( (destabilizationCurvatureMax >= 1000000000.0 || destabilizationCurvatureMax < 0.0)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          2089,
          0,
          "%s\n\t(destabilizationCurvatureMax) = %g",
          "(destabilizationCurvatureMax < 1000000000.0f && destabilizationCurvatureMax >= 0.0f)",
          destabilizationCurvatureMax) )
  {
    __debugbreak();
  }
  return destabilizationCurvatureMax;
}

void  Missile_ApplyAttractorsRepulsors(float a1@<ebp>, gentity_s *missile)
{
  double v2; // xmm0_8
  float v3; // xmm0_4
  float v4; // xmm0_4
  float v5; // xmm0_4
  long double v6; // [esp-24h] [ebp-ACh]
  float v7; // [esp-24h] [ebp-ACh]
  float v9; // [esp+8h] [ebp-80h]
  float v10; // [esp+8h] [ebp-80h]
  float angleToAttractor; // [esp+Ch] [ebp-7Ch]
  float v12; // [esp+10h] [ebp-78h]
  float force; // [esp+14h] [ebp-74h] BYREF
  float totalDist; // [esp+18h] [ebp-70h]
  float perpDist; // [esp+1Ch] [ebp-6Ch]
  float perpDir[3]; // [esp+20h] [ebp-68h] BYREF
  float perpDelta[3]; // [esp+2Ch] [ebp-5Ch] BYREF
  float v18; // [esp+38h] [ebp-50h]
  float forwardDist; // [esp+3Ch] [ebp-4Ch]
  float delta[3]; // [esp+40h] [ebp-48h]
  float v21; // [esp+4Ch] [ebp-3Ch]
  float v22; // [esp+50h] [ebp-38h]
  gentity_s *ent; // [esp+54h] [ebp-34h]
  float attractorOrigin[3]; // [esp+58h] [ebp-30h]
  float v25; // [esp+64h] [ebp-24h]
  unsigned int attractorIndex; // [esp+68h] [ebp-20h]
  float forceVector[3]; // [esp+6Ch] [ebp-1Ch] BYREF
  float forwardDir[3]; // [esp+78h] [ebp-10h]
  float retaddr; // [esp+88h] [ebp+0h]

  forwardDir[1] = a1;
  forwardDir[2] = retaddr;
  LODWORD(forwardDir[0]) = BG_GetWeaponDef(missile->s.weapon);
  if ( Vec3NormalizeTo(missile->s.lerp.pos.trDelta, forceVector) < 0.0000099999997 )
    return;
  attractorOrigin[2] = *(float *)&FLOAT_0_0;
  v25 = *(float *)&FLOAT_0_0;
  attractorIndex = *(unsigned int *)&FLOAT_0_0;
  for ( attractorOrigin[1] = 0.0; LODWORD(attractorOrigin[1]) < 0x20; ++LODWORD(attractorOrigin[1]) )
  {
    if ( attrGlob.attractors[LODWORD(attractorOrigin[1])].inUse )
    {
      if ( attrGlob.attractors[LODWORD(attractorOrigin[1])].entnum == 1023 )
      {
        LODWORD(attractorOrigin[0]) = 28 * LODWORD(attractorOrigin[1]) + 65863520;
        v21 = attrGlob.attractors[LODWORD(attractorOrigin[1])].origin[0];
        v22 = attrGlob.attractors[LODWORD(attractorOrigin[1])].origin[1];
        ent = (gentity_s *)LODWORD(attrGlob.attractors[LODWORD(attractorOrigin[1])].origin[2]);
      }
      else
      {
        if ( attrGlob.attractors[LODWORD(attractorOrigin[1])].entnum >= 1024
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
                2355,
                0,
                "%s",
                "attrGlob.attractors[attractorIndex].entnum < MAX_GENTITIES") )
        {
          __debugbreak();
        }
        LODWORD(delta[2]) = &g_entities[attrGlob.attractors[LODWORD(attractorOrigin[1])].entnum];
        LODWORD(delta[1]) = LODWORD(delta[2]) + 292;
        v21 = *(float *)(LODWORD(delta[2]) + 292);
        v22 = *(float *)(LODWORD(delta[2]) + 296);
        ent = *(gentity_s **)(LODWORD(delta[2]) + 300);
      }
      LODWORD(delta[0]) = missile->s.lerp.pos.trBase;
      perpDelta[2] = v21 - missile->s.lerp.pos.trBase[0];
      v18 = v22 - missile->s.lerp.pos.trBase[1];
      forwardDist = *(float *)&ent - missile->s.lerp.pos.trBase[2];
      perpDelta[1] = (float)((float)(perpDelta[2] * forceVector[0]) + (float)(v18 * forceVector[1]))
                   + (float)(forwardDist * forceVector[2]);
      if ( perpDelta[1] > 0.0 )
      {
        LODWORD(perpDelta[0]) = LODWORD(perpDelta[1]) ^ _mask__NegFloat_;
        perpDir[0] = (float)(COERCE_FLOAT(LODWORD(perpDelta[1]) ^ _mask__NegFloat_) * forceVector[0]) + perpDelta[2];
        perpDir[1] = (float)(COERCE_FLOAT(LODWORD(perpDelta[1]) ^ _mask__NegFloat_) * forceVector[1]) + v18;
        perpDir[2] = (float)(COERCE_FLOAT(LODWORD(perpDelta[1]) ^ _mask__NegFloat_) * forceVector[2]) + forwardDist;
        v12 = Vec3NormalizeTo(perpDir, &force);
        if ( v12 < 0.0000099999997 )
        {
          if ( attrGlob.attractors[LODWORD(attractorOrigin[1])].isAttractor )
            continue;
          force = *(float *)&FLOAT_0_0;
          totalDist = *(float *)&FLOAT_0_0;
          perpDist = FLOAT_N1_0;
        }
        if ( !attrGlob.attractors[LODWORD(attractorOrigin[1])].isAttractor && perpDist > 0.0 )
        {
          force = *(float *)&FLOAT_0_0;
          totalDist = *(float *)&FLOAT_0_0;
          perpDist = FLOAT_N1_0;
          v12 = *(float *)&FLOAT_0_0;
        }
        angleToAttractor = Abs(&perpDelta[2]);
        if ( angleToAttractor <= attrGlob.attractors[LODWORD(attractorOrigin[1])].maxDist )
        {
          if ( attrGlob.attractors[LODWORD(attractorOrigin[1])].maxDist <= 0.0
            && !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
                  2387,
                  0,
                  "%s",
                  "attrGlob.attractors[attractorIndex].maxDist > 0") )
          {
            __debugbreak();
          }
          v9 = (float)(1.0 - (float)(angleToAttractor / attrGlob.attractors[LODWORD(attractorOrigin[1])].maxDist))
             * attrGlob.attractors[LODWORD(attractorOrigin[1])].strength;
          v2 = (float)(COERCE_FLOAT(LODWORD(v12) & _mask__AbsFloat_) / perpDelta[1]);
          __libm_sse2_atan(v6);
          v3 = v2;
          if ( attrGlob.attractors[LODWORD(attractorOrigin[1])].isAttractor )
            v4 = v3 * 0.63662028;
          else
            v4 = (float)(v3 * 0.63662028) - 1.0;
          v10 = v9 * v4;
          if ( attrGlob.attractors[LODWORD(attractorOrigin[1])].isAttractor )
          {
            if ( (float)(v10
                       - (float)((float)((float)((float)*(int *)(LODWORD(forwardDir[0]) + 1480) * v12) / perpDelta[1])
                               * 20.0)) < 0.0 )
              v5 = v10;
            else
              v5 = (float)((float)((float)*(int *)(LODWORD(forwardDir[0]) + 1480) * v12) / perpDelta[1]) * 20.0;
            v10 = v5;
          }
          attractorOrigin[2] = (float)(v10 * force) + attractorOrigin[2];
          v25 = (float)(v10 * totalDist) + v25;
          *(float *)&attractorIndex = (float)(v10 * perpDist) + *(float *)&attractorIndex;
        }
      }
    }
  }
  if ( attractorOrigin[2] != 0.0 || v25 != 0.0 || *(float *)&attractorIndex != 0.0 )
  {
    missile->s.lerp.pos.trDelta[0] = (float)(0.050000001 * attractorOrigin[2]) + missile->s.lerp.pos.trDelta[0];
    missile->s.lerp.pos.trDelta[1] = (float)(0.050000001 * v25) + missile->s.lerp.pos.trDelta[1];
    missile->s.lerp.pos.trDelta[2] = (float)(0.050000001 * *(float *)&attractorIndex) + missile->s.lerp.pos.trDelta[2];
    Vec3NormalizeTo(missile->s.lerp.pos.trDelta, forceVector);
    v7 = (float)*(int *)(LODWORD(forwardDir[0]) + 1480);
    missile->s.lerp.pos.trDelta[0] = v7 * forceVector[0];
    missile->s.lerp.pos.trDelta[1] = v7 * forceVector[1];
    missile->s.lerp.pos.trDelta[2] = v7 * forceVector[2];
    vectoangles(forceVector, missile->s.lerp.apos.trBase);
  }
}

void __cdecl RunMissile_CreateWaterSplash(const gentity_s *missile, const float *hitPos, const trace_t *trace)
{
  gentity_s *tent; // [esp+Ch] [ebp-10h]
  float reflect[3]; // [esp+10h] [ebp-Ch] BYREF

  if ( !missile && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 2486, 0, "%s", "missile") )
    __debugbreak();
  if ( !trace && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 2487, 0, "%s", "trace") )
    __debugbreak();
  Vec3NormalizeTo(missile->s.lerp.pos.trDelta, reflect);
  if ( reflect[2] < 0.0 )
    reflect[2] = reflect[2] * -1.0;
  tent = G_TempEntity(hitPos, 56);
  tent->s.eventParm = DirToByte(trace->normal.vec.v);
  tent->s.un1.scale = 0;
  tent->s.surfType = (int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20] & trace->sflags) >> 20;
  AssignToSmallerType<short>(&tent->s.otherEntityNum, missile->s.number);
  tent->s.weapon = missile->s.weapon;
}

void __cdecl RunMissile_BroadcastActorEvents(gentity_s *missile)
{
  int methodOfDeath; // [esp+4h] [ebp-8h]
  const WeaponDef *weapDef; // [esp+8h] [ebp-4h]

  if ( !missile && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 2508, 0, "%s", "missile") )
    __debugbreak();
  methodOfDeath = dword_E07CF8[12 * missile->handler];
  weapDef = BG_GetWeaponDef(missile->s.weapon);
  if ( !weapDef && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 2512, 0, "%s", "weapDef") )
    __debugbreak();
  if ( methodOfDeath == 3 )
  {
    if ( weapDef->offhandClass )
    {
      if ( level.time - missile->item[0].ammoCount >= 250 )
      {
        Actor_BroadcastPointEvent(missile, AI_EV_GRENADE_PING, -1, missile->r.currentOrigin, 0.0);
        missile->item[0].ammoCount = level.time;
      }
    }
  }
  else
  {
    Actor_BroadcastPointEvent(missile, AI_EV_PROJECTILE_PING, -1, missile->r.currentOrigin, 0.0);
  }
}

void __cdecl MissileTrajectory(gentity_s *ent, float *result)
{
  const float *MapCenter; // eax
  float v3; // [esp+48h] [ebp-48h]
  float v4; // [esp+54h] [ebp-3Ch]
  float dir[3]; // [esp+60h] [ebp-30h] BYREF
  bool keepRoll; // [esp+6Fh] [ebp-21h]
  float roll; // [esp+70h] [ebp-20h]
  bool validOrigin; // [esp+77h] [ebp-19h]
  float dbgStart[3]; // [esp+78h] [ebp-18h] BYREF
  float forwardSpeed; // [esp+84h] [ebp-Ch]
  const WeaponDef *weapDef; // [esp+88h] [ebp-8h]
  float accel; // [esp+8Ch] [ebp-4h]

  validOrigin = 1;
  keepRoll = 0;
  roll = *(float *)&FLOAT_0_0;
  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 3279, 0, "%s", "ent") )
    __debugbreak();
  if ( !result && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 3280, 0, "%s", "result") )
    __debugbreak();
  weapDef = BG_GetWeaponDef(ent->s.weapon);
  if ( !weapDef && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 3283, 0, "%s", "weapDef") )
    __debugbreak();
  MapCenter = (const float *)SV_GetMapCenter();
  validOrigin = BG_ValidateOrigin(ent->r.currentOrigin, 16, 17, MapCenter);
  if ( validOrigin )
  {
    if ( level.time <= ent->s.lerp.u.actor.index.actorNum
      || ent->s.lerp.pos.trType == 3
      || ent->handler != 12
      || weapDef->weapType == WEAPTYPE_BOMB
      || weapDef->guidedMissileType == MISSILE_GUIDANCE_BALLISTIC )
    {
      BG_EvaluateTrajectory(&ent->s.lerp.pos, level.time, result);
    }
    else
    {
      if ( weapDef->timeToAccelerate > 0.0 )
      {
        AngleVectors(ent->r.currentAngles, dir, 0, 0);
        forwardSpeed = (float)((float)(dir[0] * ent->s.lerp.pos.trDelta[0])
                             + (float)(dir[1] * ent->s.lerp.pos.trDelta[1]))
                     + (float)(dir[2] * ent->s.lerp.pos.trDelta[2]);
        if ( forwardSpeed < (float)weapDef->iProjectileSpeed )
        {
          accel = (float)weapDef->iProjectileSpeed / weapDef->timeToAccelerate;
          v3 = accel * 0.050000001;
          ent->s.lerp.pos.trDelta[0] = (float)((float)(accel * 0.050000001) * dir[0]) + ent->s.lerp.pos.trDelta[0];
          ent->s.lerp.pos.trDelta[1] = (float)(v3 * dir[1]) + ent->s.lerp.pos.trDelta[1];
          ent->s.lerp.pos.trDelta[2] = (float)(v3 * dir[2]) + ent->s.lerp.pos.trDelta[2];
        }
        else
        {
          v4 = (float)weapDef->iProjectileSpeed - forwardSpeed;
          ent->s.lerp.pos.trDelta[0] = (float)(v4 * dir[0]) + ent->s.lerp.pos.trDelta[0];
          ent->s.lerp.pos.trDelta[1] = (float)(v4 * dir[1]) + ent->s.lerp.pos.trDelta[1];
          ent->s.lerp.pos.trDelta[2] = (float)(v4 * dir[2]) + ent->s.lerp.pos.trDelta[2];
        }
      }
      if ( weapDef->projectileCurvature > 0.0 )
      {
        ent->s.lerp.pos.trDelta[0] = (float)(0.050000001 * ent->mover.pos2[2]) + ent->s.lerp.pos.trDelta[0];
        ent->s.lerp.pos.trDelta[1] = (float)(0.050000001 * ent->mover.pos3[0]) + ent->s.lerp.pos.trDelta[1];
        ent->s.lerp.pos.trDelta[2] = (float)(0.050000001 * ent->mover.pos3[1]) + ent->s.lerp.pos.trDelta[2];
      }
      if ( missileDebugDraw->current.enabled )
      {
        dbgStart[0] = ent->r.currentOrigin[0];
        dbgStart[1] = ent->r.currentOrigin[1];
        dbgStart[2] = ent->r.currentOrigin[2];
      }
      GuidedMissileSteering(ent);
      if ( weapDef->guidedMissileType == MISSILE_GUIDANCE_TVGUIDED )
      {
        ent->s.lerp.pos.trBase[0] = (float)(0.050000001 * ent->s.lerp.pos.trDelta[0]) + ent->s.lerp.pos.trBase[0];
        ent->s.lerp.pos.trBase[1] = (float)(0.050000001 * ent->s.lerp.pos.trDelta[1]) + ent->s.lerp.pos.trBase[1];
        ent->s.lerp.pos.trBase[2] = (float)(0.050000001 * ent->s.lerp.pos.trDelta[2]) + ent->s.lerp.pos.trBase[2];
        roll = ent->s.lerp.apos.trBase[2];
        keepRoll = 1;
        vectoangles(ent->s.lerp.pos.trDelta, ent->s.lerp.apos.trBase);
        ent->s.lerp.apos.trBase[2] = roll;
        G_SetAngle(ent, ent->s.lerp.apos.trBase);
        ent->s.lerp.apos.trType = 1;
        MissileTrajectoryClientControlled(ent, result);
      }
      if ( weapDef->guidedMissileType != MISSILE_GUIDANCE_JAVELIN || ent->missile.missile.stage )
      {
        ent->s.lerp.pos.trBase[0] = (float)(0.050000001 * ent->s.lerp.pos.trDelta[0]) + ent->s.lerp.pos.trBase[0];
        ent->s.lerp.pos.trBase[1] = (float)(0.050000001 * ent->s.lerp.pos.trDelta[1]) + ent->s.lerp.pos.trBase[1];
        ent->s.lerp.pos.trBase[2] = (float)(0.050000001 * ent->s.lerp.pos.trDelta[2]) + ent->s.lerp.pos.trBase[2];
        *result = ent->s.lerp.pos.trBase[0];
        result[1] = ent->s.lerp.pos.trBase[1];
        result[2] = ent->s.lerp.pos.trBase[2];
      }
      else
      {
        BG_EvaluateTrajectory(&ent->s.lerp.pos, level.time, result);
      }
      if ( missileDebugDraw->current.enabled )
      {
        if ( weapDef->guidedMissileType == MISSILE_GUIDANCE_JAVELIN && ent->missile.missile.stage == MISSILESTAGE_ASCENT )
          CG_DebugLine(dbgStart, result, colorOrange, 0, 200);
        else
          CG_DebugLine(dbgStart, result, colorRed, 0, 200);
      }
      if ( ent->s.lerp.apos.trType == 1 && MissileIsReadyForSteering(ent) )
      {
        vectoangles(ent->s.lerp.pos.trDelta, ent->r.currentAngles);
        if ( keepRoll )
          ent->r.currentAngles[2] = roll;
        G_SetAngle(ent, ent->r.currentAngles);
        ent->s.lerp.apos.trType = 1;
      }
    }
  }
  else
  {
    Com_Printf(
      15,
      "Removing missile at position  ( %f, %f, %f )\n",
      ent->r.currentOrigin[0],
      ent->r.currentOrigin[1],
      ent->r.currentOrigin[2]);
    Scr_Notify(ent, scr_const.death, 0);
    G_FreeEntity(ent);
  }
}

bool __cdecl MissileIsReadyForSteering(gentity_s *ent)
{
  return (float)(BG_GetWeaponDef(ent->s.weapon)->timeToAccelerate
               - (float)((float)(level.time - ent->s.lerp.pos.trTime) * 0.001)) <= 0.0;
}

void __cdecl MissileTrajectoryClientControlled(gentity_s *ent, float *result)
{
  float dirOrig[3]; // [esp+10h] [ebp-20h] BYREF
  gentity_s *owner; // [esp+1Ch] [ebp-14h]
  float targetSpeed; // [esp+20h] [ebp-10h]
  float speed; // [esp+24h] [ebp-Ch]
  const WeaponDef *weapDef; // [esp+28h] [ebp-8h]
  usercmd_s *cmd; // [esp+2Ch] [ebp-4h]

  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 3057, 0, "%s", "ent") )
    __debugbreak();
  if ( !result && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 3058, 0, "%s", "result") )
    __debugbreak();
  ent->clipmask &= ~0x800u;
  owner = EntHandle::ent(&ent->parent);
  if ( !owner->client
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 3066, 0, "%s", "owner->client") )
  {
    __debugbreak();
  }
  weapDef = BG_GetWeaponDef(ent->s.weapon);
  cmd = &owner->client->sess.cmd;
  AngleVectors(ent->r.currentAngles, dirOrig, 0, 0);
  speed = (float)((float)(dirOrig[0] * ent->s.lerp.pos.trDelta[0]) + (float)(dirOrig[1] * ent->s.lerp.pos.trDelta[1]))
        + (float)(dirOrig[2] * ent->s.lerp.pos.trDelta[2]);
  if ( bitarray<51>::testBit(&cmd->button_bits, 0xFu) && (ent->missile.flags & 1) != 0 )
  {
    targetSpeed = (float)weapDef->iProjectileSpeed
                + (float)((float)weapDef->iProjectileSpeed * missileTVGuidedBoost->current.value);
    ent->missile.flags |= 4u;
    if ( targetSpeed > speed )
      speed = (float)missileTVGuidedBoostSpeedUp->current.integer * 0.050000001;
    if ( speed >= targetSpeed )
      speed = targetSpeed - speed;
  }
  else
  {
    if ( !bitarray<51>::testBit(&cmd->button_bits, 0xFu) && (ent->missile.flags & 1) == 0 )
      ent->missile.flags |= 1u;
    targetSpeed = (float)weapDef->iProjectileSpeed;
    ent->missile.flags &= ~4u;
    if ( speed < targetSpeed )
      speed = targetSpeed - speed;
    else
      speed = (float)missileTVGuidedBoostSpeedDown->current.integer * 0.050000001;
  }
  ent->s.lerp.pos.trDelta[0] = (float)(speed * dirOrig[0]) + ent->s.lerp.pos.trDelta[0];
  ent->s.lerp.pos.trDelta[1] = (float)(speed * dirOrig[1]) + ent->s.lerp.pos.trDelta[1];
  ent->s.lerp.pos.trDelta[2] = (float)(speed * dirOrig[2]) + ent->s.lerp.pos.trDelta[2];
  *result = ent->s.lerp.pos.trBase[0];
  result[1] = ent->s.lerp.pos.trBase[1];
  result[2] = ent->s.lerp.pos.trBase[2];
}

void __cdecl GuidedMissileSteering(gentity_s *ent)
{
  float v1; // [esp+14h] [ebp-124h]
  float value; // [esp+18h] [ebp-120h]
  gentity_s *v3; // [esp+54h] [ebp-E4h]
  trace_t tr; // [esp+58h] [ebp-E0h] BYREF
  float v5; // [esp+94h] [ebp-A4h]
  float v6; // [esp+98h] [ebp-A0h]
  float v7; // [esp+9Ch] [ebp-9Ch]
  float origin[3]; // [esp+A0h] [ebp-98h] BYREF
  float deltaRoll; // [esp+ACh] [ebp-8Ch]
  gentity_s *owner; // [esp+B0h] [ebp-88h]
  float baseAngle[3]; // [esp+B4h] [ebp-84h] BYREF
  usercmd_s *usercmd; // [esp+C0h] [ebp-78h]
  float desiredAngle; // [esp+C4h] [ebp-74h]
  float farPoint[3]; // [esp+C8h] [ebp-70h]
  bool usingGamepad; // [esp+D7h] [ebp-61h]
  float rollAccel; // [esp+D8h] [ebp-60h]
  float nearPoint[3]; // [esp+DCh] [ebp-5Ch]
  float currentHorzSpeed; // [esp+E8h] [ebp-50h]
  float currentRight[2]; // [esp+ECh] [ebp-4Ch] BYREF
  float toTarget[3]; // [esp+F4h] [ebp-44h]
  float currentHorzDir[2]; // [esp+100h] [ebp-38h] BYREF
  float steer[3]; // [esp+108h] [ebp-30h] BYREF
  float targetPos[3]; // [esp+114h] [ebp-24h] BYREF
  bool tvGuided; // [esp+123h] [ebp-15h]
  const WeaponDef *weapDef; // [esp+124h] [ebp-14h]
  float maxAccel; // [esp+128h] [ebp-10h]
  float toTargetRelative[3]; // [esp+12Ch] [ebp-Ch] BYREF

  weapDef = BG_GetWeaponDef(ent->s.weapon);
  if ( weapDef->guidedMissileType
    && (weapDef->guidedMissileType == MISSILE_GUIDANCE_WIREGUIDED
     || weapDef->guidedMissileType == MISSILE_GUIDANCE_TVGUIDED
     || EntHandle::isDefined(&ent->missileTargetEnt))
    && (weapDef->guidedMissileType == MISSILE_GUIDANCE_WIREGUIDED
     || weapDef->guidedMissileType == MISSILE_GUIDANCE_TVGUIDED
     || IsMissileLockedOn(ent))
    && MissileIsReadyForSteering(ent) )
  {
    if ( weapDef->guidedMissileType == MISSILE_GUIDANCE_JAVELIN )
    {
      JavelinSteering(ent, weapDef);
    }
    else
    {
      currentHorzSpeed = Vec2NormalizeTo(ent->s.lerp.pos.trDelta, currentHorzDir);
      currentRight[0] = currentHorzDir[1];
      LODWORD(currentRight[1]) = LODWORD(currentHorzDir[0]) ^ _mask__NegFloat_;
      tvGuided = 0;
      maxAccel = *(float *)&FLOAT_0_0;
      if ( weapDef->guidedMissileType == MISSILE_GUIDANCE_TVGUIDED )
      {
        if ( !EntHandle::isDefined(&ent->r.ownerNum)
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
                3158,
                0,
                "%s",
                "ent->r.ownerNum.isDefined()") )
        {
          __debugbreak();
        }
        owner = EntHandle::ent(&ent->r.ownerNum);
        usercmd = &owner->client->sess.cmd;
        usingGamepad = 0;
        desiredAngle = *(float *)&FLOAT_0_0;
        deltaRoll = *(float *)&FLOAT_0_0;
        rollAccel = *(float *)&FLOAT_0_0;
        baseAngle[0] = ent->r.currentAngles[0];
        baseAngle[1] = ent->r.currentAngles[1];
        baseAngle[2] = ent->r.currentAngles[2];
        if ( usercmd->pitchmove > missileTVGuidedStickDeadzone->current.integer
          || usercmd->pitchmove < -missileTVGuidedStickDeadzone->current.integer )
        {
          baseAngle[0] = (float)((float)((float)usercmd->pitchmove / 126.0) * missileTVGuidedTurnRate->current.value)
                       + baseAngle[0];
          usingGamepad = 1;
        }
        if ( usercmd->yawmove <= missileTVGuidedStickDeadzone->current.integer
          && usercmd->yawmove >= -missileTVGuidedStickDeadzone->current.integer )
        {
          rollAccel = missileTVGuidedFlatteningRollAccel->current.value;
        }
        else
        {
          baseAngle[1] = (float)((float)((float)usercmd->yawmove / 126.0) * missileTVGuidedTurnRate->current.value)
                       + baseAngle[1];
          usingGamepad = 1;
          maxAccel = (float)((float)((float)usercmd->yawmove / 126.0) * weapDef->maxSteeringAccel) * -1.0;
          tvGuided = 1;
          desiredAngle = (float)((float)usercmd->yawmove / 126.0)
                       * COERCE_FLOAT(missileTVGuidedMaxRollAngle->current.integer ^ _mask__NegFloat_);
          rollAccel = missileTVGuidedTurningRollAccel->current.value;
        }
        if ( ent->s.lerp.apos.trBase[2] <= desiredAngle )
        {
          if ( desiredAngle > ent->s.lerp.apos.trBase[2] )
          {
            deltaRoll = ent->s.lerp.apos.trBase[2] - desiredAngle;
            ent->s.lerp.apos.trBase[2] = (float)((float)((float)(deltaRoll * 0.050000001) * -1.0) * rollAccel)
                                       + ent->s.lerp.apos.trBase[2];
          }
        }
        else
        {
          deltaRoll = desiredAngle - ent->s.lerp.apos.trBase[2];
          ent->s.lerp.apos.trBase[2] = (float)((float)(deltaRoll * 0.050000001) * rollAccel)
                                     + ent->s.lerp.apos.trBase[2];
        }
        if ( !usingGamepad )
        {
          baseAngle[0] = (float)usercmd->pitchmove + baseAngle[0];
          baseAngle[1] = (float)usercmd->yawmove + baseAngle[1];
          ent->s.lerp.apos.trBase[2] = (float)usercmd->yawmove + ent->s.lerp.apos.trBase[2];
          if ( (float)(missileTVGuidedMaxRollAngle->current.value - ent->r.currentAngles[2]) < 0.0 )
            value = missileTVGuidedMaxRollAngle->current.value;
          else
            value = ent->r.currentAngles[2];
          ent->s.lerp.apos.trBase[2] = value;
          if ( (float)(ent->r.currentAngles[2]
                     - COERCE_FLOAT(missileTVGuidedMaxRollAngle->current.integer ^ _mask__NegFloat_)) < 0.0 )
            LODWORD(v1) = missileTVGuidedMaxRollAngle->current.integer ^ _mask__NegFloat_;
          else
            v1 = ent->r.currentAngles[2];
          ent->s.lerp.apos.trBase[2] = v1;
        }
        AngleVectors(baseAngle, targetPos, 0, 0);
        farPoint[0] = FLOAT_300_0;
        farPoint[1] = FLOAT_300_0;
        farPoint[2] = FLOAT_300_0;
        targetPos[0] = targetPos[0] * 300.0;
        targetPos[1] = targetPos[1] * 300.0;
        targetPos[2] = targetPos[2] * 300.0;
        nearPoint[0] = ent->r.currentOrigin[0];
        nearPoint[1] = ent->r.currentOrigin[1];
        nearPoint[2] = ent->r.currentOrigin[2];
        targetPos[0] = targetPos[0] + nearPoint[0];
        targetPos[1] = targetPos[1] + nearPoint[1];
        targetPos[2] = targetPos[2] + nearPoint[2];
      }
      else if ( weapDef->guidedMissileType == MISSILE_GUIDANCE_WIREGUIDED )
      {
        if ( !EntHandle::isDefined(&ent->r.ownerNum)
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
                3225,
                0,
                "%s",
                "ent->r.ownerNum.isDefined()") )
        {
          __debugbreak();
        }
        v3 = EntHandle::ent(&ent->r.ownerNum);
        AngleVectors(v3->s.lerp.apos.trBase, targetPos, 0, 0);
        v5 = FLOAT_30000_0;
        v6 = FLOAT_30000_0;
        v7 = FLOAT_30000_0;
        targetPos[0] = targetPos[0] * 30000.0;
        targetPos[1] = targetPos[1] * 30000.0;
        targetPos[2] = targetPos[2] * 30000.0;
        G_GetPlayerViewOrigin(&v3->client->ps, origin);
        targetPos[0] = targetPos[0] + origin[0];
        targetPos[1] = targetPos[1] + origin[1];
        targetPos[2] = targetPos[2] + origin[2];
        memset(&tr, 0, 16);
        G_LocationalTraceAllowChildren(
          &tr,
          origin,
          targetPos,
          v3->s.number,
          (int)&cls.recentServers[7543].countrycode[1],
          0);
        if ( tr.fraction < 1.0 )
          Vec3Lerp(origin, targetPos, tr.fraction, targetPos);
      }
      else
      {
        GetTargetPosition(ent, targetPos);
      }
      if ( missileDebugDraw->current.enabled )
      {
        CG_DebugLine(ent->s.lerp.pos.trBase, targetPos, colorGreen, 0, 200);
        G_DebugCircle(targetPos, 10.0, colorOrange, 0, 0, 500);
      }
      toTarget[0] = targetPos[0] - ent->s.lerp.pos.trBase[0];
      toTarget[1] = targetPos[1] - ent->s.lerp.pos.trBase[1];
      toTarget[2] = targetPos[2] - ent->s.lerp.pos.trBase[2];
      toTargetRelative[0] = (float)(toTarget[0] * currentHorzDir[0]) + (float)(toTarget[1] * currentHorzDir[1]);
      toTargetRelative[1] = (float)(toTarget[0] * currentRight[0]) + (float)(toTarget[1] * currentRight[1]);
      toTargetRelative[2] = toTarget[2];
      memset(steer, 0, sizeof(steer));
      if ( weapDef->guidedMissileType == MISSILE_GUIDANCE_TVGUIDED )
        MissileTVGuidedHorzSteerToTarget(ent, currentRight, toTargetRelative, steer, maxAccel);
      else
        MissileHorzSteerToTarget(ent, currentRight, toTargetRelative, currentHorzSpeed, steer, tvGuided);
      MissileVerticalSteering(ent, toTargetRelative, currentHorzSpeed, steer);
      ent->s.lerp.pos.trDelta[0] = (float)(0.050000001 * steer[0]) + ent->s.lerp.pos.trDelta[0];
      ent->s.lerp.pos.trDelta[1] = (float)(0.050000001 * steer[1]) + ent->s.lerp.pos.trDelta[1];
      ent->s.lerp.pos.trDelta[2] = (float)(0.050000001 * steer[2]) + ent->s.lerp.pos.trDelta[2];
    }
  }
}

char __cdecl IsMissileLockedOn(gentity_s *ent)
{
  if ( !EntHandle::isDefined(&ent->missileTargetEnt) )
    return 0;
  if ( !EntHandle::ent(&ent->missileTargetEnt)->r.inuse
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 2541, 0, "%s", "target->r.inuse") )
  {
    __debugbreak();
  }
  return 1;
}

void __cdecl GetTargetPosition(gentity_s *ent, float *result)
{
  float v2; // [esp+4h] [ebp-3Ch]
  float v3; // [esp+Ch] [ebp-34h]
  float v4; // [esp+10h] [ebp-30h]
  gentity_s *target; // [esp+18h] [ebp-28h]
  float axis[3][3]; // [esp+1Ch] [ebp-24h] BYREF

  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 2551, 0, "%s", "ent") )
    __debugbreak();
  if ( ent->s.eType != 4
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          2552,
          0,
          "%s",
          "ent->s.eType == ET_MISSILE") )
  {
    __debugbreak();
  }
  if ( !EntHandle::isDefined(&ent->missileTargetEnt)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          2553,
          0,
          "%s",
          "ent->missileTargetEnt.isDefined()") )
  {
    __debugbreak();
  }
  target = EntHandle::ent(&ent->missileTargetEnt);
  AnglesToAxis(target->r.currentAngles, axis);
  *result = target->r.currentOrigin[0];
  result[1] = target->r.currentOrigin[1];
  result[2] = target->r.currentOrigin[2];
  v4 = ent->mover.pos3[2];
  *result = (float)(v4 * axis[0][0]) + *result;
  result[1] = (float)(v4 * axis[0][1]) + result[1];
  result[2] = (float)(v4 * axis[0][2]) + result[2];
  v3 = ent->mover.apos1[0];
  *result = (float)(v3 * axis[1][0]) + *result;
  result[1] = (float)(v3 * axis[1][1]) + result[1];
  result[2] = (float)(v3 * axis[1][2]) + result[2];
  v2 = ent->mover.apos1[1];
  *result = (float)(v2 * axis[2][0]) + *result;
  result[1] = (float)(v2 * axis[2][1]) + result[1];
  result[2] = (float)(v2 * axis[2][2]) + result[2];
}

void __cdecl MissileTVGuidedHorzSteerToTarget(
        gentity_s *ent,
        const float *currentRight,
        const float *toTargetRelative,
        float *steer,
        float accel)
{
  *steer = accel * *currentRight;
  steer[1] = accel * currentRight[1];
}

void __cdecl MissileHorzSteerToTarget(
        gentity_s *ent,
        const float *currentRight,
        const float *toTargetRelative,
        float currentHorzSpeed,
        float *steer,
        bool tvGuided)
{
  float v6; // xmm0_4
  gentity_s *v7; // eax
  float v8; // [esp+0h] [ebp-48h]
  float v9; // [esp+14h] [ebp-34h]
  float maxSteeringAccel; // [esp+18h] [ebp-30h]
  float v11; // [esp+24h] [ebp-24h]
  float dist; // [esp+28h] [ebp-20h]
  float targetPos[3]; // [esp+2Ch] [ebp-1Ch] BYREF
  float radius; // [esp+38h] [ebp-10h]
  float tightestRadius; // [esp+3Ch] [ebp-Ch]
  const WeaponDef *weapDef; // [esp+40h] [ebp-8h]
  float accel; // [esp+44h] [ebp-4h]
  int savedregs; // [esp+48h] [ebp+0h] BYREF

  weapDef = BG_GetWeaponDef(ent->s.weapon);
  if ( toTargetRelative[1] == 0.0 )
    radius = FLOAT_3_4028235e38;
  else
    radius = (float)((float)(*toTargetRelative * *toTargetRelative) + (float)(toTargetRelative[1] * toTargetRelative[1]))
           / (float)(2.0 * toTargetRelative[1]);
  if ( weapDef->maxSteeringAccel <= 0.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          2590,
          0,
          "%s",
          "weapDef->maxSteeringAccel > 0") )
  {
    __debugbreak();
  }
  tightestRadius = (float)((float)weapDef->iProjectileSpeed * (float)weapDef->iProjectileSpeed)
                 / weapDef->maxSteeringAccel;
  if ( *toTargetRelative <= 0.0 || tvGuided )
  {
    if ( (float)(tightestRadius + 60.0) <= COERCE_FLOAT(LODWORD(radius) & _mask__AbsFloat_) )
    {
      if ( toTargetRelative[1] <= 0.0 )
      {
        LODWORD(v9) = LODWORD(weapDef->maxSteeringAccel) ^ _mask__NegFloat_;
        *steer = v9 * *currentRight;
        v6 = v9 * currentRight[1];
      }
      else
      {
        maxSteeringAccel = weapDef->maxSteeringAccel;
        *steer = maxSteeringAccel * *currentRight;
        v6 = maxSteeringAccel * currentRight[1];
      }
      steer[1] = v6;
      if ( EntHandle::isDefined(&ent->missileTargetEnt) )
      {
        v7 = EntHandle::ent(&ent->missileTargetEnt);
        if ( BG_GetWeaponDef(v7->s.weapon)->guidedMissileType == MISSILE_GUIDANCE_TVGUIDED )
        {
          GetTargetPosition(ent, targetPos);
          dist = Vec3Distance(ent->r.currentOrigin, targetPos);
          if ( (float)weapDef->iExplosionRadius > dist )
            G_ExplodeMissile((cStaticModel_s *)&savedregs, ent);
        }
      }
    }
  }
  else if ( toTargetRelative[1] != 0.0 )
  {
    if ( tightestRadius > COERCE_FLOAT(LODWORD(radius) & _mask__AbsFloat_) )
    {
      if ( radius <= 0.0 )
        LODWORD(radius) = LODWORD(tightestRadius) ^ _mask__NegFloat_;
      else
        radius = tightestRadius;
    }
    if ( radius == 0.0
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 2606, 0, "%s", "radius != 0") )
    {
      __debugbreak();
    }
    accel = (float)((float)(2.0 * currentHorzSpeed) * currentHorzSpeed) / radius;
    if ( (float)(accel - weapDef->maxSteeringAccel) < 0.0 )
      v11 = accel;
    else
      v11 = weapDef->maxSteeringAccel;
    if ( (float)(COERCE_FLOAT(LODWORD(weapDef->maxSteeringAccel) ^ _mask__NegFloat_) - accel) < 0.0 )
      v8 = v11;
    else
      LODWORD(v8) = LODWORD(weapDef->maxSteeringAccel) ^ _mask__NegFloat_;
    accel = v8;
    *steer = v8 * *currentRight;
    steer[1] = accel * currentRight[1];
  }
}

void __cdecl MissileVerticalSteering(
        gentity_s *ent,
        const float *toTargetRelative,
        float currentHorzSpeed,
        float *steer)
{
  int v4; // [esp+10h] [ebp-3Ch]
  float v5; // [esp+18h] [ebp-34h]
  int integer; // [esp+24h] [ebp-28h]
  float toTargetHorzRelativeDir[2]; // [esp+34h] [ebp-18h] BYREF
  float horzDistToTarg; // [esp+3Ch] [ebp-10h]
  float minTimeToTarg; // [esp+40h] [ebp-Ch]
  float maxVertSpeedAtMaxAccel; // [esp+44h] [ebp-8h]
  const WeaponDef *weapDef; // [esp+48h] [ebp-4h]

  weapDef = BG_GetWeaponDef(ent->s.weapon);
  horzDistToTarg = Vec2NormalizeTo(toTargetRelative, toTargetHorzRelativeDir);
  steer[2] = *(float *)&FLOAT_0_0;
  if ( horzDistToTarg != 0.0 )
  {
    switch ( weapDef->guidedMissileType )
    {
      case MISSILE_GUIDANCE_SIDEWINDER:
      case MISSILE_GUIDANCE_WIREGUIDED:
      case MISSILE_GUIDANCE_TVGUIDED:
        goto $LN10_39;
      case MISSILE_GUIDANCE_HELLFIRE:
        if ( ent->s.lerp.pos.trDuration )
        {
$LN10_39:
          MissileVerticalSteerToTarget(
            ent,
            toTargetHorzRelativeDir,
            horzDistToTarg,
            toTargetRelative[2],
            currentHorzSpeed,
            steer);
        }
        else
        {
          if ( *toTargetRelative <= 0.0 || currentHorzSpeed == 0.0 )
          {
            if ( !weapDef->iProjectileSpeed
              && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
                    2691,
                    0,
                    "%s",
                    "weapDef->iProjectileSpeed != 0") )
            {
              __debugbreak();
            }
            minTimeToTarg = horzDistToTarg / (float)weapDef->iProjectileSpeed;
          }
          else
          {
            minTimeToTarg = *toTargetRelative / (float)weapDef->iProjectileSpeed;
          }
          if ( minTimeToTarg <= 0.0
            && !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
                  2695,
                  0,
                  "%s",
                  "minTimeToTarg > 0") )
          {
            __debugbreak();
          }
          maxVertSpeedAtMaxAccel = toTargetRelative[2] / minTimeToTarg;
          maxVertSpeedAtMaxAccel = (float)((float)(0.5 * weapDef->maxSteeringAccel) * minTimeToTarg)
                                 + maxVertSpeedAtMaxAccel;
          maxVertSpeedAtMaxAccel = maxVertSpeedAtMaxAccel * 0.89999998;
          if ( maxVertSpeedAtMaxAccel > (float)(currentHorzSpeed * missileHellfireMaxSlope->current.value) )
            maxVertSpeedAtMaxAccel = currentHorzSpeed * missileHellfireMaxSlope->current.value;
          if ( maxVertSpeedAtMaxAccel <= ent->s.lerp.pos.trDelta[2] )
          {
            ent->s.lerp.pos.trDuration = 1;
          }
          else
          {
            steer[2] = (float)(maxVertSpeedAtMaxAccel - ent->s.lerp.pos.trDelta[2]) * 20.0;
            v5 = steer[2];
            if ( (float)(v5 - missileHellfireUpAccel->current.value) < 0.0 )
              integer = *((unsigned int *)steer + 2);
            else
              integer = missileHellfireUpAccel->current.integer;
            if ( (float)(COERCE_FLOAT(LODWORD(weapDef->maxSteeringAccel) ^ _mask__NegFloat_) - v5) < 0.0 )
              v4 = integer;
            else
              v4 = LODWORD(weapDef->maxSteeringAccel) ^ _mask__NegFloat_;
            *((unsigned int *)steer + 2) = v4;
          }
        }
        break;
      default:
        return;
    }
  }
}

void __cdecl MissileVerticalSteerToTarget(
        gentity_s *ent,
        const float *toTargetHorzRelDir,
        float horzDistToTarg,
        float vertDistToTarg,
        float currentHorzSpeed,
        float *steer)
{
  float v6; // [esp+0h] [ebp-20h]
  float v7; // [esp+4h] [ebp-1Ch]
  float maxSteeringAccel; // [esp+10h] [ebp-10h]
  const WeaponDef *weapDef; // [esp+1Ch] [ebp-4h]

  if ( *toTargetHorzRelDir >= 0.0 )
  {
    weapDef = BG_GetWeaponDef(ent->s.weapon);
    steer[2] = (float)((float)((float)(COERCE_FLOAT(COERCE_UNSIGNED_INT(*toTargetHorzRelDir * currentHorzSpeed) & _mask__AbsFloat_)
                                     * vertDistToTarg)
                             / horzDistToTarg)
                     - ent->s.lerp.pos.trDelta[2])
             * 20.0;
    v7 = steer[2];
    if ( (float)(v7 - weapDef->maxSteeringAccel) < 0.0 )
      maxSteeringAccel = steer[2];
    else
      maxSteeringAccel = weapDef->maxSteeringAccel;
    if ( (float)(COERCE_FLOAT(LODWORD(weapDef->maxSteeringAccel) ^ _mask__NegFloat_) - v7) < 0.0 )
      v6 = maxSteeringAccel;
    else
      LODWORD(v6) = LODWORD(weapDef->maxSteeringAccel) ^ _mask__NegFloat_;
    steer[2] = v6;
  }
}

void __cdecl JavelinSteering(gentity_s *ent, const WeaponDef *weapDef)
{
  const char *v2; // [esp+20h] [ebp-64h]
  float v[6]; // [esp+3Ch] [ebp-48h] BYREF
  float *trBase; // [esp+54h] [ebp-30h]
  float *currentOrigin; // [esp+58h] [ebp-2Ch]
  float limit; // [esp+5Ch] [ebp-28h]
  float distance2D; // [esp+60h] [ebp-24h]
  float height; // [esp+64h] [ebp-20h]
  float distance3D; // [esp+68h] [ebp-1Ch]
  float toTarget[3]; // [esp+6Ch] [ebp-18h] BYREF
  float targetPos[3]; // [esp+78h] [ebp-Ch] BYREF

  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 2999, 0, "%s", "ent") )
    __debugbreak();
  if ( !EntHandle::isDefined(&ent->missileTargetEnt)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          3000,
          0,
          "%s",
          "ent->missileTargetEnt.isDefined()") )
  {
    __debugbreak();
  }
  if ( ent->missile.missile.stage == MISSILESTAGE_SOFTLAUNCH )
  {
    if ( level.time - ent->s.lerp.u.actor.index.actorNum < weapDef->projIgnitionDelay )
    {
      if ( missileDebugText->current.enabled )
        Com_Printf(15, "Javelin: softlaunch\n");
      return;
    }
    ent->missile.missile.stage = MISSILESTAGE_ASCENT;
    ent->s.lerp.pos.trType = 1;
    trBase = ent->s.lerp.pos.trBase;
    currentOrigin = ent->r.currentOrigin;
    ent->s.lerp.pos.trBase[0] = ent->r.currentOrigin[0];
    trBase[1] = currentOrigin[1];
    trBase[2] = currentOrigin[2];
  }
  GetTargetPosition(ent, targetPos);
  if ( ent->missile.missile.stage == MISSILESTAGE_ASCENT )
  {
    if ( JavelinClimbEnd(ent, targetPos) )
      ent->missile.missile.stage = MISSILESTAGE_DESCENT;
    else
      JavelinClimbOffset(ent, targetPos);
  }
  LODWORD(v[5]) = ent->s.lerp.pos.trBase;
  toTarget[0] = targetPos[0] - ent->s.lerp.pos.trBase[0];
  toTarget[1] = targetPos[1] - ent->s.lerp.pos.trBase[1];
  toTarget[2] = targetPos[2] - ent->s.lerp.pos.trBase[2];
  Vec3Normalize(toTarget);
  JavelinRotateVelocity(ent, ent->s.lerp.pos.trDelta, toTarget, ent->s.lerp.pos.trDelta);
  if ( missileDebugText->current.enabled )
  {
    height = (float)(ent->s.lerp.pos.trBase[2] - targetPos[2]) - ent->mover.apos1[1];
    limit = JavelinClimbCeiling(ent);
    v[0] = targetPos[0] - ent->s.lerp.pos.trBase[0];
    v[1] = targetPos[1] - ent->s.lerp.pos.trBase[1];
    distance2D = Vec2Length(v);
    distance3D = Vec3Distance(ent->s.lerp.pos.trBase, targetPos);
    if ( ent->missile.missile.stage == MISSILESTAGE_ASCENT )
      v2 = "A";
    else
      v2 = "D";
    Com_Printf(15, "Jav:%s h:%.0f/%.0f dist 2d:%.0f 3d:%.0f\n", v2, height, limit, distance2D, distance3D);
  }
}

void __cdecl JavelinClimbOffset(gentity_s *ent, float *targetPos)
{
  gentity_s *v2; // [esp+0h] [ebp-2Ch]
  float value; // [esp+4h] [ebp-28h]
  gentity_s *target; // [esp+20h] [ebp-Ch]
  float ownerDir[2]; // [esp+24h] [ebp-8h] BYREF

  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 2801, 0, "%s", "ent") )
    __debugbreak();
  if ( ent->s.eType != 4
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          2802,
          0,
          "%s",
          "ent->s.eType == ET_MISSILE") )
  {
    __debugbreak();
  }
  if ( ent->missile.missile.flightMode )
  {
    if ( ent->missile.missile.flightMode != MISSILEFLIGHTMODE_DIRECT
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
            2810,
            0,
            "%s",
            "ent->missile.missile.flightMode == MISSILEFLIGHTMODE_DIRECT") )
    {
      __debugbreak();
    }
    targetPos[2] = targetPos[2] + missileJavClimbHeightDirect->current.value;
    if ( EntHandle::isDefined(&ent->r.ownerNum) )
      v2 = EntHandle::ent(&ent->r.ownerNum);
    else
      v2 = &g_entities[1023];
    if ( !EntHandle::isDefined(&ent->missileTargetEnt)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
            2814,
            0,
            "%s",
            "ent->missileTargetEnt.isDefined()") )
    {
      __debugbreak();
    }
    target = EntHandle::ent(&ent->missileTargetEnt);
    ownerDir[0] = v2->s.lerp.pos.trBase[0] - target->s.lerp.pos.trBase[0];
    ownerDir[1] = v2->s.lerp.pos.trBase[1] - target->s.lerp.pos.trBase[1];
    Vec2Normalize(ownerDir);
    value = missileJavClimbToOwner->current.value;
    ownerDir[0] = value * ownerDir[0];
    ownerDir[1] = value * ownerDir[1];
    *targetPos = *targetPos + ownerDir[0];
    targetPos[1] = targetPos[1] + ownerDir[1];
  }
  else
  {
    targetPos[2] = targetPos[2] + missileJavClimbHeightTop->current.value;
  }
}

void __cdecl JavelinRotateVelocity(gentity_s *ent, const float *currentVel, const float *targetDir, float *resultVel)
{
  float currentDir[3]; // [esp+Ch] [ebp-20h] BYREF
  float len; // [esp+18h] [ebp-14h]
  float resultDir[3]; // [esp+1Ch] [ebp-10h] BYREF
  float turnDiff; // [esp+28h] [ebp-4h]

  len = Vec3NormalizeTo(currentVel, currentDir);
  turnDiff = JavelinRotateDir(ent, currentDir, targetDir, resultDir);
  if ( turnDiff < 30.0 )
  {
    if ( ent->missile.missile.stage == MISSILESTAGE_ASCENT || currentVel[2] > 0.0 )
    {
      len = (float)(missileJavAccelClimb->current.value * 0.050000001) + len;
      if ( len > missileJavSpeedLimitClimb->current.value )
        len = missileJavSpeedLimitClimb->current.value;
    }
    else
    {
      if ( ent->missile.missile.stage != MISSILESTAGE_DESCENT
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
              2932,
              0,
              "%s",
              "ent->missile.missile.stage == MISSILESTAGE_DESCENT") )
      {
        __debugbreak();
      }
      len = (float)(missileJavAccelDescend->current.value * 0.050000001) + len;
      if ( len > missileJavSpeedLimitDescend->current.value )
        len = missileJavSpeedLimitDescend->current.value;
    }
  }
  len = (float)(1.0 - (float)((float)(turnDiff / 180.0) * missileJavTurnDecel->current.value)) * len;
  *resultVel = len * resultDir[0];
  resultVel[1] = len * resultDir[1];
  resultVel[2] = len * resultDir[2];
}

double __cdecl JavelinRotateDir(gentity_s *ent, const float *currentDir, const float *targetDir, float *resultDir)
{
  float end[3]; // [esp+34h] [ebp-4Ch] BYREF
  float maxDPS; // [esp+40h] [ebp-40h]
  float targetQuat[4]; // [esp+44h] [ebp-3Ch] BYREF
  float frac; // [esp+54h] [ebp-2Ch]
  float resultQuat[4]; // [esp+58h] [ebp-28h] BYREF
  float targetDPS; // [esp+68h] [ebp-18h]
  float currentQuat[4]; // [esp+6Ch] [ebp-14h] BYREF
  float dot; // [esp+7Ch] [ebp-4h]

  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 2858, 0, "%s", "ent") )
    __debugbreak();
  maxDPS = JavelinMaxDPS(ent);
  dot = (float)(1.0
              - (float)((float)((float)((float)((float)(*targetDir * *currentDir) + (float)(targetDir[1] * currentDir[1]))
                                      + (float)(targetDir[2] * currentDir[2]))
                              + 1.0)
                      * 0.5))
      * 180.0;
  if ( dot > 0.1 )
  {
    targetDPS = dot / 0.050000001;
    if ( maxDPS <= (float)(dot / 0.050000001) )
    {
      frac = maxDPS / targetDPS;
      if ( missileDebugText->current.enabled )
        Com_Printf(15, "dot:%.2f frac:%.2f =%.0f/%.0f ", dot, frac, maxDPS, targetDPS);
      VecToQuat(currentDir, currentQuat);
      VecToQuat(targetDir, targetQuat);
      QuatSlerp(currentQuat, targetQuat, frac, resultQuat);
      Vec4Normalize(resultQuat);
      UnitQuatToForward(resultQuat, resultDir);
      if ( missileDebugDraw->current.enabled )
      {
        end[0] = (float)(100.0 * *currentDir) + ent->s.lerp.pos.trBase[0];
        end[1] = (float)(100.0 * currentDir[1]) + ent->s.lerp.pos.trBase[1];
        end[2] = (float)(100.0 * currentDir[2]) + ent->s.lerp.pos.trBase[2];
        CG_DebugLine(ent->s.lerp.pos.trBase, end, colorBlue, 0, 200);
        end[0] = (float)(100.0 * *targetDir) + ent->s.lerp.pos.trBase[0];
        end[1] = (float)(100.0 * targetDir[1]) + ent->s.lerp.pos.trBase[1];
        end[2] = (float)(100.0 * targetDir[2]) + ent->s.lerp.pos.trBase[2];
        CG_DebugLine(ent->s.lerp.pos.trBase, end, colorLtGrey, 0, 200);
        end[0] = (float)(100.0 * *resultDir) + ent->s.lerp.pos.trBase[0];
        end[1] = (float)(100.0 * resultDir[1]) + ent->s.lerp.pos.trBase[1];
        end[2] = (float)(100.0 * resultDir[2]) + ent->s.lerp.pos.trBase[2];
        CG_DebugLine(ent->s.lerp.pos.trBase, end, colorGreen, 0, 200);
      }
      return dot;
    }
    else
    {
      *resultDir = *targetDir;
      resultDir[1] = targetDir[1];
      resultDir[2] = targetDir[2];
      if ( missileDebugText->current.enabled )
        Com_Printf(15, "dot:%.2f (%.0f > %.0f) ", dot, maxDPS, targetDPS);
      return 0.0;
    }
  }
  else
  {
    *resultDir = *targetDir;
    resultDir[1] = targetDir[1];
    resultDir[2] = targetDir[2];
    if ( missileDebugText->current.enabled )
      Com_Printf(15, "dot:%.2f ", dot);
    return 0.0;
  }
}

double __cdecl JavelinMaxDPS(gentity_s *ent)
{
  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 2826, 0, "%s", "ent") )
    __debugbreak();
  if ( ent->s.eType != 4
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          2827,
          0,
          "%s",
          "ent->s.eType == ET_MISSILE") )
  {
    __debugbreak();
  }
  if ( ent->missile.missile.flightMode == MISSILEFLIGHTMODE_TOP )
    return missileJavTurnRateTop->current.value;
  if ( ent->missile.missile.flightMode != MISSILEFLIGHTMODE_DIRECT
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          2831,
          0,
          "%s",
          "ent->missile.missile.flightMode == MISSILEFLIGHTMODE_DIRECT") )
  {
    __debugbreak();
  }
  return missileJavTurnRateDirect->current.value;
}

void __cdecl VecToQuat(const float *vec, float *quat)
{
  float angles[3]; // [esp+0h] [ebp-30h] BYREF
  float axis[3][3]; // [esp+Ch] [ebp-24h] BYREF

  vectoangles(vec, angles);
  AnglesToAxis(angles, axis);
  AxisToQuat(axis, quat);
}

double __cdecl JavelinClimbCeiling(gentity_s *ent)
{
  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 2947, 0, "%s", "ent") )
    __debugbreak();
  if ( ent->s.eType != 4
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          2948,
          0,
          "%s",
          "ent->s.eType == ET_MISSILE") )
  {
    __debugbreak();
  }
  if ( ent->missile.missile.flightMode == MISSILEFLIGHTMODE_DIRECT )
    return missileJavClimbCeilingDirect->current.value;
  if ( ent->missile.missile.flightMode )
  {
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
            2956,
            0,
            "%s",
            "ent->missile.missile.flightMode == MISSILEFLIGHTMODE_TOP") )
      __debugbreak();
  }
  return missileJavClimbCeilingTop->current.value;
}

bool __cdecl JavelinClimbEnd(gentity_s *ent, const float *targetPos)
{
  int savedregs; // [esp+0h] [ebp+0h] BYREF

  if ( !JavelinClimbIsAboveCeiling(ent, targetPos) )
    return 0;
  if ( JavelinClimbExceededAngle(COERCE_FLOAT(&savedregs), ent, targetPos) )
    return 1;
  return JavelinClimbWithinDistance(ent, targetPos) != 0;
}

char  JavelinClimbExceededAngle@<al>(float a1@<ebp>, gentity_s *ent, const float *targetPos)
{
  double v3; // xmm0_8
  long double v5; // [esp-10h] [ebp-3Ch]
  float toTarget[3]; // [esp+Ch] [ebp-20h] BYREF
  float value; // [esp+18h] [ebp-14h]
  float currentHorzDir[2]; // [esp+1Ch] [ebp-10h]
  float limit; // [esp+24h] [ebp-8h]
  float retaddr; // [esp+2Ch] [ebp+0h]

  currentHorzDir[1] = a1;
  limit = retaddr;
  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 2743, 0, "%s", "ent") )
    __debugbreak();
  if ( ent->s.eType != 4
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          2744,
          0,
          "%s",
          "ent->s.eType == ET_MISSILE") )
  {
    __debugbreak();
  }
  currentHorzDir[0] = ent->mover.apos2[0];
  if ( LODWORD(currentHorzDir[0]) )
  {
    if ( ent->missile.missile.flightMode != MISSILEFLIGHTMODE_DIRECT
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
            2752,
            0,
            "%s",
            "ent->missile.missile.flightMode == MISSILEFLIGHTMODE_DIRECT") )
    {
      __debugbreak();
    }
    value = missileJavClimbAngleDirect->current.value;
  }
  else
  {
    value = missileJavClimbAngleTop->current.value;
  }
  Vec2NormalizeTo(ent->s.lerp.pos.trDelta, &toTarget[1]);
  v3 = (float)((float)((float)((float)(*targetPos - ent->s.lerp.pos.trBase[0]) * toTarget[1])
                     + (float)((float)(targetPos[1] - ent->s.lerp.pos.trBase[1]) * toTarget[2]))
             / (float)(targetPos[2] - ent->s.lerp.pos.trBase[2]));
  __libm_sse2_atan(v5);
  *(float *)&v3 = v3;
  if ( value <= COERCE_FLOAT(COERCE_UNSIGNED_INT(*(float *)&v3 * 57.295776) & _mask__AbsFloat_) )
    return 0;
  if ( missileDebugText->current.enabled )
    Com_Printf(15, "Javelin: *** Exceeded climb angle ***\n");
  return 1;
}

char __cdecl JavelinClimbWithinDistance(gentity_s *ent, const float *targetPos)
{
  float v[2]; // [esp+8h] [ebp-Ch] BYREF
  float distance; // [esp+10h] [ebp-4h]

  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 2779, 0, "%s", "ent") )
    __debugbreak();
  if ( ent->s.eType != 4
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          2780,
          0,
          "%s",
          "ent->s.eType == ET_MISSILE") )
  {
    __debugbreak();
  }
  v[0] = *targetPos - ent->s.lerp.pos.trBase[0];
  v[1] = targetPos[1] - ent->s.lerp.pos.trBase[1];
  distance = Vec2Length(v);
  if ( distance >= 400.0 )
    return 0;
  if ( missileDebugText->current.enabled )
    Com_Printf(15, "Javelin: *** Exceeded climb distance ***\n");
  return 1;
}

bool __cdecl JavelinClimbIsAboveCeiling(gentity_s *ent, const float *targetPos)
{
  float limit; // [esp+0h] [ebp-8h]
  float height; // [esp+4h] [ebp-4h]

  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 2967, 0, "%s", "ent") )
    __debugbreak();
  if ( ent->s.eType != 4
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          2968,
          0,
          "%s",
          "ent->s.eType == ET_MISSILE") )
  {
    __debugbreak();
  }
  height = (float)(ent->s.lerp.pos.trBase[2] - targetPos[2]) - ent->mover.apos1[1];
  limit = JavelinClimbCeiling(ent);
  return height > limit;
}

bool __cdecl CheckForMissileClientControlledDetonation(gentity_s *ent)
{
  gentity_s *owner; // [esp+0h] [ebp-Ch]
  const WeaponDef *weapDef; // [esp+4h] [ebp-8h]
  usercmd_s *cmd; // [esp+8h] [ebp-4h]

  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 3409, 0, "%s", "ent") )
    __debugbreak();
  if ( ent->s.eType != 4
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          3410,
          0,
          "%s",
          "ent->s.eType == ET_MISSILE") )
  {
    __debugbreak();
  }
  owner = EntHandle::ent(&ent->parent);
  if ( !owner->client
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 3413, 0, "%s", "owner->client") )
  {
    __debugbreak();
  }
  weapDef = BG_GetWeaponDef(ent->s.weapon);
  cmd = &owner->client->sess.cmd;
  if ( weapDef->guidedMissileType != MISSILE_GUIDANCE_TVGUIDED )
    return 0;
  if ( !bitarray<51>::testBit(&owner->client->sess.cmd.button_bits, 0) && (ent->missile.flags & 2) == 0 )
    ent->missile.flags |= 2u;
  return bitarray<51>::testBit(&cmd->button_bits, 0) && (ent->missile.flags & 2) != 0;
}

char __cdecl UpdateGuidedMissileFuelTime(gentity_s *ent, bool initTimer)
{
  gentity_s *owner; // [esp+0h] [ebp-Ch]
  const WeaponDef *weapDef; // [esp+4h] [ebp-8h]
  playerState_s *ps; // [esp+8h] [ebp-4h]
  int savedregs; // [esp+Ch] [ebp+0h] BYREF

  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 3442, 0, "%s", "ent") )
    __debugbreak();
  if ( ent->s.eType != 4
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          3443,
          0,
          "%s",
          "ent->s.eType == ET_MISSILE") )
  {
    __debugbreak();
  }
  owner = EntHandle::ent(&ent->parent);
  if ( !owner->client
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 3446, 0, "%s", "owner->client") )
  {
    __debugbreak();
  }
  ps = &owner->client->ps;
  weapDef = BG_GetWeaponDef(ent->s.weapon);
  if ( !ps->weapon )
    return 1;
  if ( initTimer )
    ent->missile.timeAlive = level.time;
  if ( (ent->missile.flags & 4) != 0 )
    BG_AddFuelTankTime(ps, ps->weapon, 2 * (level.time - ent->missile.timeAlive));
  else
    BG_AddFuelTankTime(ps, ps->weapon, level.time - ent->missile.timeAlive);
  if ( (float)BG_GetFuelTankTime(ps, ps->weapon) <= (float)(weapDef->projLifetime * 1000.0) )
  {
    ent->missile.timeAlive = level.time;
    return 1;
  }
  else
  {
    BG_SetFuelTankTime(ps, ps->weapon, (int)(float)(weapDef->projLifetime * 1000.0));
    G_ExplodeMissile((cStaticModel_s *)&savedregs, ent);
    return 0;
  }
}

unsigned __int8 __cdecl G_GetGrenadeTrType(gentity_s *grenade)
{
  if ( EntHandle::isDefined(&grenade->parent) && EntHandle::ent(&grenade->parent)->s.eType == 17 )
    return 7;
  else
    return 6;
}

void __cdecl G_RunMissile(gentity_s *ent)
{
  bool doAntilag; // [esp+3h] [ebp-329h]
  AntilagClientStore antilagClients; // [esp+4h] [ebp-328h] BYREF
  const WeaponDef *weapDef; // [esp+328h] [ebp-4h]

  weapDef = BG_GetWeaponDef(ent->s.weapon);
  if ( !weapDef && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 3867, 0, "%s", "weapDef") )
    __debugbreak();
  doAntilag = weapDef->bUseAntiLagRewind;
  if ( doAntilag )
    G_AntiLagRewindClientPos(ent->missile.antilagTimeOffset + level.time, &antilagClients);
  G_RunMissileInternal(ent);
  if ( doAntilag )
    G_AntiLag_RestoreClientPos(&antilagClients);
}

int __cdecl G_PredictMissile(gentity_s *ent, int duration, float *vLandPos, int allowBounce, int *timeAtRest)
{
  int v6; // [esp+10h] [ebp-3B8h]
  int v7; // [esp+14h] [ebp-3B4h]
  int passEntityNum; // [esp+18h] [ebp-3B0h]
  trajectory_t pos; // [esp+38h] [ebp-390h] BYREF
  float origin[3]; // [esp+5Ch] [ebp-36Ch] BYREF
  gentity_s backupEnt; // [esp+68h] [ebp-360h] BYREF
  int time; // [esp+360h] [ebp-68h]
  float endpos[3]; // [esp+364h] [ebp-64h] BYREF
  trace_t tr; // [esp+370h] [ebp-58h] BYREF
  float org[3]; // [esp+3ACh] [ebp-1Ch] BYREF
  const WeaponDef *weapDef; // [esp+3B8h] [ebp-10h]
  float traceStart[3]; // [esp+3BCh] [ebp-Ch] BYREF

  memset(&tr, 0, 16);
  memcpy(&pos, &ent->s.lerp.pos, sizeof(pos));
  BG_EvaluateTrajectory(&pos, level.time - 50, org);
  memcpy(&backupEnt, ent, sizeof(backupEnt));
  *timeAtRest = ent->nextthink;
  weapDef = BG_GetWeaponDef(ent->s.weapon);
  if ( !weapDef && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 3978, 0, "%s", "weapDef") )
    __debugbreak();
  *vLandPos = *(float *)&FLOAT_0_0;
  vLandPos[1] = *(float *)&FLOAT_0_0;
  vLandPos[2] = *(float *)&FLOAT_0_0;
  for ( time = level.time; time < duration + level.time; time += 50 )
  {
    BG_EvaluateTrajectory(&pos, time, origin);
    if ( EntHandle::isDefined(&ent->r.ownerNum) )
    {
      passEntityNum = EntHandle::entnum(&ent->r.ownerNum);
      G_MissileTrace(&tr, org, origin, passEntityNum, ent->clipmask, ent->s.weapon);
    }
    else
    {
      G_MissileTrace(&tr, org, origin, 1023, ent->clipmask, ent->s.weapon);
    }
    if ( tr.startsolid )
    {
      if ( time != level.time )
        goto LABEL_36;
      org[0] = origin[0];
      org[1] = origin[1];
      org[2] = origin[2];
    }
    else
    {
      if ( (_UNKNOWN **)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20] & tr.sflags) == &loc_900000 )
        Missile_PenetrateGlass(&tr, ent, org, origin, weapDef->damage, 1);
      Vec3Lerp(org, origin, tr.fraction, endpos);
      DrawMissilePredictDebug(org, endpos);
      org[0] = endpos[0];
      org[1] = endpos[1];
      org[2] = endpos[2];
      if ( weapDef->stickiness == WEAPSTICKINESS_ALL || weapDef->stickiness == WEAPSTICKINESS_ALL_NO_SENTIENTS )
      {
        if ( tr.fraction < 1.0 )
        {
          traceStart[0] = (float)(0.13500001 * tr.normal.vec.v[0]) + org[0];
          traceStart[1] = (float)(0.13500001 * tr.normal.vec.v[1]) + org[1];
          traceStart[2] = (float)(0.13500001 * tr.normal.vec.v[2]) + org[2];
          origin[0] = (float)(-1.5 * tr.normal.vec.v[0]) + org[0];
          origin[1] = (float)(-1.5 * tr.normal.vec.v[1]) + org[1];
          origin[2] = (float)(-1.5 * tr.normal.vec.v[2]) + org[2];
          if ( EntHandle::isDefined(&ent->r.ownerNum) )
          {
            v7 = EntHandle::entnum(&ent->r.ownerNum);
            G_MissileTrace(&tr, traceStart, origin, v7, ent->clipmask, ent->s.weapon);
          }
          else
          {
            G_MissileTrace(&tr, traceStart, origin, 1023, ent->clipmask, ent->s.weapon);
          }
          Vec3Lerp(traceStart, origin, tr.fraction, endpos);
          if ( tr.fraction != 1.0 && Trace_GetEntityHitId(&tr) == 1022 )
          {
            org[0] = endpos[0] + (float)(endpos[0] - origin[0]);
            org[1] = endpos[1] + (float)(endpos[1] - origin[1]);
            org[2] = endpos[2] + (float)(endpos[2] - origin[2]);
          }
        }
      }
      else if ( tr.fraction == 1.0 || tr.fraction < 1.0 && tr.normal.vec.v[2] > 0.69999999 )
      {
        traceStart[0] = org[0];
        traceStart[1] = org[1];
        origin[0] = org[0];
        origin[1] = org[1];
        traceStart[2] = org[2] + 0.13500001;
        origin[2] = org[2] - 1.5;
        if ( EntHandle::isDefined(&ent->r.ownerNum) )
        {
          v6 = EntHandle::entnum(&ent->r.ownerNum);
          G_MissileTrace(&tr, traceStart, origin, v6, ent->clipmask, ent->s.weapon);
        }
        else
        {
          G_MissileTrace(&tr, traceStart, origin, 1023, ent->clipmask, ent->s.weapon);
        }
        Vec3Lerp(traceStart, origin, tr.fraction, endpos);
        if ( tr.fraction != 1.0 )
        {
          pos.trBase[2] = (float)((float)(endpos[2] + 1.5) - org[2]) + pos.trBase[2];
          org[0] = endpos[0];
          org[1] = endpos[1];
          org[2] = endpos[2] + 1.5;
        }
      }
      if ( tr.fraction != 1.0 )
      {
        if ( (tr.sflags & 4) != 0 )
        {
LABEL_36:
          memcpy(ent, &backupEnt, sizeof(gentity_s));
          return 0;
        }
        if ( !allowBounce
          || ((unsigned int)&cls.rankedServers[711].game[35] & ent->s.lerp.eFlags) == 0
          || (PredictBounceMissile(ent, &pos, &tr, time, time + (int)(float)(50.0 * tr.fraction) - 50, org, endpos),
              pos.trTime = time,
              !pos.trType) )
        {
          *timeAtRest = time;
          break;
        }
      }
    }
  }
  *vLandPos = org[0];
  vLandPos[1] = org[1];
  vLandPos[2] = org[2];
  if ( ((*(unsigned int *)vLandPos & 0x7F800000) == 0x7F800000
     || ((unsigned int)vLandPos[1] & 0x7F800000) == 0x7F800000
     || ((unsigned int)vLandPos[2] & 0x7F800000) == 0x7F800000)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          4084,
          0,
          "%s",
          "!IS_NAN((vLandPos)[0]) && !IS_NAN((vLandPos)[1]) && !IS_NAN((vLandPos)[2])") )
  {
    __debugbreak();
  }
  memcpy(ent, &backupEnt, sizeof(gentity_s));
  if ( allowBounce && ((unsigned int)&cls.rankedServers[711].game[35] & ent->s.lerp.eFlags) != 0 )
    return ent->nextthink;
  else
    return time;
}

void __cdecl DrawMissilePredictDebug(float *start, float *end)
{
  float color[4]; // [esp+0h] [ebp-10h] BYREF

  color[0] = FLOAT_0_5;
  color[1] = *(float *)&FLOAT_0_0;
  color[2] = FLOAT_1_0;
  color[3] = FLOAT_1_0;
  if ( g_debugBullets->current.integer >= 5 )
    CG_DebugLine(start, end, color, 1, 1000);
}

void __cdecl PredictBounceMissile(
        gentity_s *ent,
        trajectory_t *pos,
        trace_t *trace,
        int time,
        int velocityTime,
        float *origin,
        float *endpos)
{
  float v7; // [esp+3Ch] [ebp-30h]
  float velocity[3]; // [esp+40h] [ebp-2Ch] BYREF
  int hitTime; // [esp+4Ch] [ebp-20h]
  int surfType; // [esp+50h] [ebp-1Ch]
  float bounceFactor; // [esp+54h] [ebp-18h]
  const WeaponDef *weapDef; // [esp+58h] [ebp-14h]
  float dot; // [esp+5Ch] [ebp-10h]
  float vDelta[3]; // [esp+60h] [ebp-Ch]

  if ( !ent->s.weapon
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 3905, 0, "%s", "ent->s.weapon") )
  {
    __debugbreak();
  }
  weapDef = BG_GetWeaponDef(ent->s.weapon);
  if ( !weapDef && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 3907, 0, "%s", "weapDef") )
    __debugbreak();
  surfType = (unsigned __int8)((int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20] & trace->sflags) >> 20);
  hitTime = velocityTime;
  BG_EvaluateTrajectoryDelta(pos, velocityTime, velocity);
  dot = (float)((float)(velocity[0] * trace->normal.vec.v[0]) + (float)(velocity[1] * trace->normal.vec.v[1]))
      + (float)(velocity[2] * trace->normal.vec.v[2]);
  v7 = -2.0 * dot;
  pos->trDelta[0] = (float)((float)(-2.0 * dot) * trace->normal.vec.v[0]) + velocity[0];
  pos->trDelta[1] = (float)(v7 * trace->normal.vec.v[1]) + velocity[1];
  pos->trDelta[2] = (float)(v7 * trace->normal.vec.v[2]) + velocity[2];
  if ( ((LODWORD(pos->trDelta[0]) & 0x7F800000) == 0x7F800000
     || (LODWORD(pos->trDelta[1]) & 0x7F800000) == 0x7F800000
     || (LODWORD(pos->trDelta[2]) & 0x7F800000) == 0x7F800000)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          3916,
          0,
          "%s",
          "!IS_NAN((pos->trDelta)[0]) && !IS_NAN((pos->trDelta)[1]) && !IS_NAN((pos->trDelta)[2])") )
  {
    __debugbreak();
  }
  if ( ((unsigned int)&cls.rankedServers[711].game[35] & ent->s.lerp.eFlags) == 0 )
    goto LABEL_27;
  bounceFactor = Abs(velocity);
  if ( bounceFactor > 0.0 && dot <= 0.0 )
  {
    dot = dot / COERCE_FLOAT(LODWORD(bounceFactor) ^ _mask__NegFloat_);
    bounceFactor = (float)((float)(weapDef->perpendicularBounce[surfType] - weapDef->parallelBounce[surfType]) * dot)
                 + weapDef->parallelBounce[surfType];
    pos->trDelta[0] = bounceFactor * pos->trDelta[0];
    pos->trDelta[1] = bounceFactor * pos->trDelta[1];
    pos->trDelta[2] = bounceFactor * pos->trDelta[2];
    if ( ((LODWORD(pos->trDelta[0]) & 0x7F800000) == 0x7F800000
       || (LODWORD(pos->trDelta[1]) & 0x7F800000) == 0x7F800000
       || (LODWORD(pos->trDelta[2]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
            3928,
            0,
            "%s",
            "!IS_NAN((pos->trDelta)[0]) && !IS_NAN((pos->trDelta)[1]) && !IS_NAN((pos->trDelta)[2])") )
    {
      __debugbreak();
    }
  }
  if ( weapDef->stickiness == WEAPSTICKINESS_ALL
    || weapDef->stickiness == WEAPSTICKINESS_ALL_NO_SENTIENTS
    || trace->normal.vec.v[2] > 0.69999999
    && (weapDef->stickiness == WEAPSTICKINESS_GROUND
     || weapDef->stickiness == WEAPSTICKINESS_GROUND_WITH_YAW
     || Abs(pos->trDelta) < 20.0) )
  {
    pos->trBase[0] = *endpos;
    pos->trBase[1] = endpos[1];
    pos->trBase[2] = endpos[2];
    pos->trType = 0;
    pos->trTime = 0;
    pos->trDuration = 0;
    pos->trDelta[0] = *(float *)&FLOAT_0_0;
    pos->trDelta[1] = *(float *)&FLOAT_0_0;
    pos->trDelta[2] = *(float *)&FLOAT_0_0;
  }
  else
  {
LABEL_27:
    vDelta[0] = 0.1 * trace->normal.vec.v[0];
    vDelta[1] = 0.1 * trace->normal.vec.v[1];
    vDelta[2] = 0.1 * trace->normal.vec.v[2];
    if ( vDelta[2] > 0.0 )
      vDelta[2] = *(float *)&FLOAT_0_0;
    pos->trBase[0] = *origin + vDelta[0];
    pos->trBase[1] = origin[1] + vDelta[1];
    pos->trBase[2] = origin[2] + vDelta[2];
    pos->trTime = time;
  }
}

void __cdecl G_InitGrenadeEntity(gentity_s *parent, gentity_s *grenade)
{
  gentity_s *ent; // [esp+0h] [ebp-14h]
  int fusetime; // [esp+Ch] [ebp-8h]
  const WeaponDef *weapDef; // [esp+10h] [ebp-4h]

  if ( !parent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 4104, 0, "%s", "parent") )
    __debugbreak();
  if ( !grenade && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 4105, 0, "%s", "grenade") )
    __debugbreak();
  if ( !grenade->s.weapon
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 4107, 0, "%s", "grenade->s.weapon") )
  {
    __debugbreak();
  }
  weapDef = BG_GetWeaponDef(grenade->s.weapon);
  if ( !weapDef && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 4109, 0, "%s", "weapDef") )
    __debugbreak();
  grenade->s.eType = 4;
  grenade->s.lerp.eFlags = (int)&cls.rankedServers[711].game[35];
  grenade->s.lerp.u.actor.index.actorNum = level.time;
  fusetime = weapDef->aiFuseTime;
  if ( parent && parent->client )
    fusetime = weapDef->fuseTime;
  grenade->s.lerp.u.actor.team = fusetime;
  grenade->r.contents = 256;
  grenade->r.contents |= 0x2000u;
  grenade->r.mins[0] = FLOAT_N1_5;
  grenade->r.mins[1] = FLOAT_N1_5;
  grenade->r.mins[2] = FLOAT_N1_5;
  grenade->r.maxs[0] = FLOAT_1_5;
  grenade->r.maxs[1] = FLOAT_1_5;
  grenade->r.maxs[2] = FLOAT_1_5;
  if ( parent && parent->client )
    grenade->missile.antilagTimeOffset = parent->client->lastServerTime - level.time;
  else
    grenade->missile.antilagTimeOffset = 0;
  if ( !parent->client
    || parent->client->ps.throwBackGrenadeOwner == 1023
    || (Scr_AddEntity(grenade, SCRIPTINSTANCE_SERVER),
        Scr_AddEntity(&g_entities[parent->client->ps.throwBackGrenadeOwner], SCRIPTINSTANCE_SERVER),
        Scr_Notify(parent, scr_const.grenade_throwback, 2u),
        parent->client->ps.grenadeTimeLeft >= 0) )
  {
    EntHandle::setEnt(&grenade->r.ownerNum, parent);
    EntHandle::setEnt(&grenade->parent, parent);
    grenade->s.lerp.u.loopFx.period = parent->s.clientNum;
  }
  else
  {
    EntHandle::setEnt(&grenade->r.ownerNum, &g_entities[parent->client->ps.throwBackGrenadeOwner]);
    if ( EntHandle::isDefined(&grenade->r.ownerNum) )
    {
      ent = EntHandle::ent(&grenade->r.ownerNum);
      EntHandle::setEnt(&grenade->parent, ent);
    }
    else
    {
      EntHandle::setEnt(&grenade->parent, 0);
    }
  }
  grenade->clipmask = (int)&cls.recentServers[7544].adr.port + 3;
  if ( weapDef->plantable )
    grenade->clipmask |= (unsigned int)&cls.recentServers[7647].hostName[20];
  if ( weapDef->stickiness == WEAPSTICKINESS_GROUND_WITH_YAW )
    grenade->clipmask &= ~0x2000000u;
  grenade->handler = 10;
  G_BroadcastEntity(grenade);
  grenade->r.svFlags = 4;
  if ( weapDef->bThrowBack )
    G_MakeMissilePickupItem(grenade);
  if ( parent->client )
    grenade->missile.team = parent->client->sess.cs.team;
  else
    grenade->missile.team = TEAM_FREE;
}

void __cdecl G_InitGrenadeMovement(
        gentity_s *grenade,
        const float *start,
        const float *dir,
        int rotate,
        WeapRotateType rotateType)
{
  float v5; // [esp+Ch] [ebp-34h]
  float v6; // [esp+14h] [ebp-2Ch]

  if ( !grenade && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 4195, 0, "%s", "grenade") )
    __debugbreak();
  grenade->item[1].ammoCount = *(unsigned int *)&FLOAT_0_0;
  grenade->s.lerp.pos.trType = G_GetGrenadeTrType(grenade);
  grenade->s.lerp.pos.trTime = level.time;
  grenade->r.currentOrigin[0] = *start;
  grenade->r.currentOrigin[1] = start[1];
  grenade->r.currentOrigin[2] = start[2];
  grenade->s.lerp.pos.trBase[0] = *start;
  grenade->s.lerp.pos.trBase[1] = start[1];
  grenade->s.lerp.pos.trBase[2] = start[2];
  grenade->s.lerp.pos.trDelta[0] = *dir;
  grenade->s.lerp.pos.trDelta[1] = dir[1];
  grenade->s.lerp.pos.trDelta[2] = dir[2];
  if ( ((LODWORD(grenade->s.lerp.pos.trDelta[0]) & 0x7F800000) == 0x7F800000
     || (LODWORD(grenade->s.lerp.pos.trDelta[1]) & 0x7F800000) == 0x7F800000
     || (LODWORD(grenade->s.lerp.pos.trDelta[2]) & 0x7F800000) == 0x7F800000)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          4205,
          0,
          "%s",
          "!IS_NAN((grenade->s.lerp.pos.trDelta)[0]) && !IS_NAN((grenade->s.lerp.pos.trDelta)[1]) && !IS_NAN((grenade->s."
          "lerp.pos.trDelta)[2])") )
  {
    __debugbreak();
  }
  vectoangles(dir, grenade->r.currentAngles);
  grenade->s.lerp.pos.trDelta[0] = (float)(int)grenade->s.lerp.pos.trDelta[0];
  grenade->s.lerp.pos.trDelta[1] = (float)(int)grenade->s.lerp.pos.trDelta[1];
  grenade->s.lerp.pos.trDelta[2] = (float)(int)grenade->s.lerp.pos.trDelta[2];
  if ( rotate )
  {
    grenade->s.lerp.apos.trType = 3;
    grenade->s.lerp.apos.trTime = level.time;
    grenade->s.lerp.apos.trBase[0] = grenade->r.currentAngles[0];
    grenade->s.lerp.apos.trBase[1] = grenade->r.currentAngles[1];
    grenade->s.lerp.apos.trBase[2] = grenade->r.currentAngles[2];
    grenade->s.lerp.apos.trBase[0] = AngleNormalize360(grenade->s.lerp.apos.trBase[0] - 120.0);
    if ( rotateType == WEAPROTATE_BLADE_ROTATE )
    {
      grenade->s.lerp.apos.trDelta[0] = FLOAT_1500_0;
      grenade->s.lerp.apos.trDelta[1] = *(float *)&FLOAT_0_0;
      grenade->s.lerp.apos.trDelta[2] = *(float *)&FLOAT_0_0;
    }
    else
    {
      v6 = G_flrand(340.0, 800.0);
      grenade->s.lerp.apos.trDelta[0] = (double)(2 * G_irand(0, 2) - 1) * v6;
      grenade->s.lerp.apos.trDelta[1] = *(float *)&FLOAT_0_0;
      v5 = G_flrand(180.0, 540.0);
      grenade->s.lerp.apos.trDelta[2] = (double)(2 * G_irand(0, 2) - 1) * v5;
      grenade->s.lerp.pos.trDelta[0] = (float)(int)grenade->s.lerp.pos.trDelta[0];
      grenade->s.lerp.pos.trDelta[1] = (float)(int)grenade->s.lerp.pos.trDelta[1];
      grenade->s.lerp.pos.trDelta[2] = (float)(int)grenade->s.lerp.pos.trDelta[2];
    }
    grenade->r.currentAngles[0] = grenade->s.lerp.apos.trBase[0];
    grenade->r.currentAngles[1] = grenade->s.lerp.apos.trBase[1];
    grenade->r.currentAngles[2] = grenade->s.lerp.apos.trBase[2];
  }
  else
  {
    G_SetAngle(grenade, grenade->r.currentAngles);
  }
}

gentity_s *__cdecl G_FireGrenade(
        gentity_s *parent,
        float *start,
        float *dir,
        int grenadeWPID,
        unsigned __int8 grenModel,
        int rotate,
        int time)
{
  char *Name; // eax
  char *v8; // eax
  float speed; // [esp+0h] [ebp-18h]
  gentity_s *grenade; // [esp+Ch] [ebp-Ch]
  DObj *obj; // [esp+10h] [ebp-8h]
  const WeaponDef *weapDef; // [esp+14h] [ebp-4h]

  if ( !parent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 4297, 0, "%s", "parent") )
    __debugbreak();
  weapDef = BG_GetWeaponDef(grenadeWPID);
  if ( !weapDef && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 4300, 0, "%s", "weapDef") )
    __debugbreak();
  grenade = G_Spawn();
  Scr_SetString(&grenade->classname, scr_const.grenade, SCRIPTINSTANCE_SERVER);
  AssignToSmallerType<unsigned short>(&grenade->s.weapon, grenadeWPID);
  grenade->s.weaponModel = grenModel;
  grenade->s.lerp.u.turret.ownerNum = 0;
  grenade->missile.grenade.effectIndex = 55;
  if ( parent->client )
    grenade->missile.grenade.effectIndex = parent->client->ps.smokeColorIndex;
  if ( parent->client )
    grenade->s.attackerEntityNum = parent->s.clientNum;
  else
    grenade->s.attackerEntityNum = -1;
  G_InitGrenadeEntity(parent, grenade);
  if ( rotate && weapDef->rotate )
    G_InitGrenadeMovement(grenade, start, dir, 1, weapDef->rotateType);
  else
    G_InitGrenadeMovement(grenade, start, dir, 0, weapDef->rotateType);
  if ( parent->client )
  {
    speed = Abs(dir);
    grenade->s.lerp.u.actor.index.actorNum += CalcMissileNoDrawTime(speed);
  }
  InitGrenadeTimer(parent, grenade, weapDef, time);
  if ( grenade->model
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 4333, 0, "%s", "!grenade->model") )
  {
    __debugbreak();
  }
  if ( weapDef->projectileModel )
  {
    Name = (char *)XModelGetName(weapDef->projectileModel);
    G_SetModel(grenade, Name);
  }
  G_DObjUpdate(grenade);
  obj = Com_GetServerDObj(grenade->s.number);
  if ( obj )
  {
    grenade->r.contents |= DObjGetContents(obj);
    DObjCalcBounds(obj, grenade->r.mins, grenade->r.maxs);
  }
  v8 = (char *)BG_WeaponName(grenadeWPID);
  Scr_AddString(v8, SCRIPTINSTANCE_SERVER);
  Scr_AddEntity(grenade, SCRIPTINSTANCE_SERVER);
  if ( weapDef->iProjectileActivateDist )
    Scr_Notify(parent, scr_const.grenade_launcher_fire, 2u);
  else
    Scr_Notify(parent, scr_const.grenade_fire, 2u);
  return grenade;
}

int __cdecl CalcMissileNoDrawTime(float speed)
{
  int v3; // [esp+4h] [ebp-8h]

  if ( (int)(float)((float)((float)(-35.0 * speed) / 600.0) + 85.0) < 50 )
    v3 = (int)(float)((float)((float)(-35.0 * speed) / 600.0) + 85.0);
  else
    v3 = 50;
  if ( v3 > 20 )
    return v3;
  else
    return 20;
}

void __cdecl InitGrenadeTimer(const gentity_s *parent, gentity_s *grenade, const WeaponDef *weapDef, int time)
{
  if ( !parent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 4249, 0, "%s", "parent") )
    __debugbreak();
  if ( !grenade && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 4250, 0, "%s", "grenade") )
    __debugbreak();
  if ( !weapDef && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 4251, 0, "%s", "weapDef") )
    __debugbreak();
  if ( !weapDef->iProjectileActivateDist || time > 0 )
  {
    if ( !parent->client || weapDef->timedDetonation )
    {
      if ( parent->client && parent->client->ps.grenadeTimeLeft )
      {
        grenade->nextthink = parent->client->ps.grenadeTimeLeft + level.time;
        parent->client->ps.grenadeTimeLeft = 0;
      }
      else
      {
        grenade->nextthink = time + level.time;
      }
    }
    else
    {
      parent->client->ps.grenadeTimeLeft = 0;
    }
  }
  if ( grenade->handler != 10
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          4274,
          0,
          "%s",
          "grenade->handler == ENT_HANDLER_GRENADE") )
  {
    __debugbreak();
  }
  grenade->item[0].index = level.time;
  if ( !grenade->nextthink )
    grenade->nextthink = level.time + 30000;
  if ( grenade->nextthink > level.time + 60000 )
    grenade->nextthink = level.time + 60000;
}

gentity_s *__cdecl G_FireRocket(
        gentity_s *parent,
        unsigned int weaponIndex,
        float *start,
        float *dir,
        const float *gunVel,
        gentity_s *target,
        const float *targetOffset)
{
  int v7; // eax
  long double v8; // st7
  char *Name; // eax
  int v10; // edx
  int Contents; // eax
  char *v12; // eax
  $0D33AF6AB483EB176B99DAC6E021D6CF *v14; // [esp+10h] [ebp-B0h]
  $0D33AF6AB483EB176B99DAC6E021D6CF *v15; // [esp+18h] [ebp-A8h]
  $0D33AF6AB483EB176B99DAC6E021D6CF *v16; // [esp+1Ch] [ebp-A4h]
  float v17; // [esp+20h] [ebp-A0h]
  float *currentOrigin; // [esp+30h] [ebp-90h]
  float *v19; // [esp+40h] [ebp-80h]
  float *v20; // [esp+44h] [ebp-7Ch]
  float *trBase; // [esp+48h] [ebp-78h]
  float *trDelta; // [esp+4Ch] [ebp-74h]
  float iProjectileSpeed; // [esp+50h] [ebp-70h]
  float *v24; // [esp+54h] [ebp-6Ch]
  float *v25; // [esp+58h] [ebp-68h]
  float *v26; // [esp+5Ch] [ebp-64h]
  float v27; // [esp+60h] [ebp-60h]
  float sinT; // [esp+7Ch] [ebp-44h]
  float theta; // [esp+80h] [ebp-40h]
  float r; // [esp+84h] [ebp-3Ch]
  float v31[3]; // [esp+88h] [ebp-38h] BYREF
  float cosT; // [esp+94h] [ebp-2Ch]
  float up[3]; // [esp+98h] [ebp-28h] BYREF
  float right[3]; // [esp+A4h] [ebp-1Ch] BYREF
  DObj *obj; // [esp+B0h] [ebp-10h]
  int ownerIndex; // [esp+B4h] [ebp-Ch]
  gentity_s *bolt; // [esp+B8h] [ebp-8h]
  const WeaponDef *weapDef; // [esp+BCh] [ebp-4h]
  int savedregs; // [esp+C0h] [ebp+0h] BYREF

  if ( !parent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 4395, 0, "%s", "parent") )
    __debugbreak();
  weapDef = BG_GetWeaponDef(weaponIndex);
  if ( !weapDef && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 4398, 0, "%s", "weapDef") )
    __debugbreak();
  Vec3Normalize(dir);
  if ( weapDef->guidedMissileType == MISSILE_GUIDANCE_JAVELIN )
  {
    dir[2] = dir[2] + MYJAVELINOFFSET;
    Vec3Normalize(dir);
    AngleVectors(parent->s.lerp.apos.trBase, 0, right, 0);
    v27 = MYJAVELINOFFSET_RIGHT;
    *start = (float)(MYJAVELINOFFSET_RIGHT * right[0]) + *start;
    start[1] = (float)(v27 * right[1]) + start[1];
    start[2] = (float)(v27 * right[2]) + start[2];
  }
  if ( weapDef->guidedMissileType == MISSILE_GUIDANCE_TVGUIDED )
  {
    *start = (float)(13.0 * *dir) + *start;
    start[1] = (float)(13.0 * dir[1]) + start[1];
    start[2] = (float)(13.0 * dir[2]) + start[2];
  }
  bolt = G_Spawn();
  Scr_SetString(&bolt->classname, scr_const.rocket, SCRIPTINSTANCE_SERVER);
  bolt->s.eType = 4;
  AssignToSmallerType<unsigned short>(&bolt->s.weapon, weaponIndex);
  bolt->s.weaponModel = 0;
  bolt->s.lerp.u.actor.index.actorNum = level.time;
  if ( parent->client )
  {
    v7 = CalcMissileNoDrawTime((float)weapDef->iProjectileSpeed);
    bolt->s.lerp.u.actor.index.actorNum += v7;
  }
  if ( parent->pTurretInfo && (parent->pTurretInfo->flags & 0x20000) != 0 )
  {
    EntHandle::setEnt(&bolt->r.ownerNum, &g_entities[parent->s.otherEntityNum]);
    EntHandle::setEnt(&bolt->parent, &g_entities[parent->s.otherEntityNum]);
  }
  else
  {
    EntHandle::setEnt(&bolt->r.ownerNum, parent);
    EntHandle::setEnt(&bolt->parent, parent);
  }
  bolt->clipmask = (int)&cls.recentServers[7544].adr.port + 3;
  bolt->handler = 12;
  InitRocketTimer(bolt, weapDef);
  bolt->item[1].ammoCount = *(unsigned int *)&FLOAT_0_0;
  EntHandle::setEnt(&bolt->missileTargetEnt, target);
  if ( targetOffset )
  {
    v26 = &bolt->mover.pos3[2];
    bolt->mover.pos3[2] = *targetOffset;
    v26[1] = targetOffset[1];
    v26[2] = targetOffset[2];
  }
  else
  {
    v25 = &bolt->mover.pos3[2];
    bolt->mover.pos3[2] = *(float *)&FLOAT_0_0;
    v25[1] = *(float *)&FLOAT_0_0;
    v25[2] = *(float *)&FLOAT_0_0;
  }
  if ( parent->client )
    bolt->missile.team = parent->client->sess.cs.team;
  else
    bolt->missile.team = parent->team;
  if ( weapDef->bForceBounce )
    bolt->s.lerp.eFlags = (int)&cls.rankedServers[711].game[35];
  ownerIndex = 0;
  if ( parent->client )
    ownerIndex = parent->client - level.clients;
  if ( (unsigned int)ownerIndex >= level.maxclients
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          4474,
          0,
          "ownerIndex doesn't index level.maxclients\n\t%i not in [0, %i)",
          ownerIndex,
          level.maxclients) )
  {
    __debugbreak();
  }
  AssignToSmallerType<unsigned char>(&bolt->s.faction.iHeadIconTeam, bolt->missile.team | (4 * ownerIndex));
  G_BroadcastEntity(bolt);
  if ( weapDef->guidedMissileType == MISSILE_GUIDANCE_TVGUIDED )
  {
    bolt->r.contents |= 0x2100u;
    bolt->r.svFlags |= 4u;
    bolt->takedamage = 1;
    bolt->health = 10000;
  }
  bolt->s.lerp.pos.trType = 3;
  if ( weapDef->timeToAccelerate <= 0.0 )
  {
    trDelta = bolt->s.lerp.pos.trDelta;
    iProjectileSpeed = (float)weapDef->iProjectileSpeed;
    bolt->s.lerp.pos.trDelta[0] = iProjectileSpeed * *dir;
    trDelta[1] = iProjectileSpeed * dir[1];
    trDelta[2] = iProjectileSpeed * dir[2];
    bolt->s.lerp.pos.trDelta[2] = (float)weapDef->iProjectileSpeedUp + bolt->s.lerp.pos.trDelta[2];
  }
  else
  {
    bolt->s.lerp.pos.trType = 1;
    v24 = bolt->s.lerp.pos.trDelta;
    bolt->s.lerp.pos.trDelta[0] = *(float *)&FLOAT_0_0;
    v24[1] = *(float *)&FLOAT_0_0;
    v24[2] = *(float *)&FLOAT_0_0;
  }
  bolt->s.lerp.pos.trTime = level.time;
  trBase = bolt->s.lerp.pos.trBase;
  bolt->s.lerp.pos.trBase[0] = *start;
  trBase[1] = start[1];
  trBase[2] = start[2];
  v19 = bolt->s.lerp.pos.trDelta;
  v20 = bolt->s.lerp.pos.trDelta;
  bolt->s.lerp.pos.trDelta[0] = bolt->s.lerp.pos.trDelta[0] + *gunVel;
  v19[1] = v20[1] + gunVel[1];
  v19[2] = v20[2] + gunVel[2];
  if ( ((LODWORD(bolt->s.lerp.pos.trDelta[0]) & 0x7F800000) == 0x7F800000
     || (LODWORD(bolt->s.lerp.pos.trDelta[1]) & 0x7F800000) == 0x7F800000
     || (LODWORD(bolt->s.lerp.pos.trDelta[2]) & 0x7F800000) == 0x7F800000)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          4507,
          0,
          "%s",
          "!IS_NAN((bolt->s.lerp.pos.trDelta)[0]) && !IS_NAN((bolt->s.lerp.pos.trDelta)[1]) && !IS_NAN((bolt->s.lerp.pos.trDelta)[2])") )
  {
    __debugbreak();
  }
  bolt->s.lerp.pos.trDelta[0] = (float)(int)bolt->s.lerp.pos.trDelta[0];
  bolt->s.lerp.pos.trDelta[1] = (float)(int)bolt->s.lerp.pos.trDelta[1];
  bolt->s.lerp.pos.trDelta[2] = (float)(int)bolt->s.lerp.pos.trDelta[2];
  currentOrigin = bolt->r.currentOrigin;
  bolt->r.currentOrigin[0] = *start;
  currentOrigin[1] = start[1];
  currentOrigin[2] = start[2];
  vectoangles(dir, bolt->r.currentAngles);
  G_SetAngle(bolt, bolt->r.currentAngles);
  if ( weapDef->projectileCurvature > 0.0 )
  {
    bolt->s.lerp.pos.trType = 1;
    AngleVectors(bolt->r.currentAngles, 0, v31, up);
    theta = G_random() * 360.0;
    r = G_random() * weapDef->projectileCurvature;
    v8 = (float)(theta * 0.017453292);
    cosT = cos(v8);
    sinT = sin(v8);
    v16 = &bolt->missile.44;
    v17 = r * cosT;
    bolt->mover.pos2[2] = (float)(r * cosT) * v31[0];
    v16->missile.curvature[1] = v17 * v31[1];
    v16->missile.curvature[2] = v17 * v31[2];
    v14 = &bolt->missile.44;
    v15 = &bolt->missile.44;
    bolt->mover.pos2[2] = (float)((float)(r * sinT) * up[0]) + bolt->mover.pos2[2];
    v14->missile.curvature[1] = (float)((float)(r * sinT) * up[1]) + v15->missile.curvature[1];
    v14->missile.curvature[2] = (float)((float)(r * sinT) * up[2]) + v15->missile.curvature[2];
    if ( ((bolt->missile.grenade.effectIndex & 0x7F800000) == 0x7F800000
       || (LODWORD(bolt->mover.pos3[0]) & 0x7F800000) == 0x7F800000
       || (LODWORD(bolt->mover.pos3[1]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
            4532,
            0,
            "%s",
            "!IS_NAN((bolt->missile.missile.curvature)[0]) && !IS_NAN((bolt->missile.missile.curvature)[1]) && !IS_NAN((b"
            "olt->missile.missile.curvature)[2])") )
    {
      __debugbreak();
    }
  }
  if ( weapDef->guidedMissileType )
  {
    bolt->s.lerp.pos.trType = 1;
    bolt->s.lerp.apos.trType = 1;
    bolt->s.lerp.pos.trDuration = 0;
    if ( weapDef->guidedMissileType == MISSILE_GUIDANCE_JAVELIN )
    {
      bolt->s.lerp.pos.trType = 6;
      bolt->s.lerp.pos.trTime = level.time;
      bolt->missile.missile.stage = MISSILESTAGE_SOFTLAUNCH;
      bolt->missile.missile.flightMode = !G_TargetAttackProfileTop(target);
    }
    else if ( weapDef->guidedMissileType == MISSILE_GUIDANCE_BALLISTIC )
    {
      bolt->s.lerp.pos.trType = 6;
      bolt->s.lerp.pos.trTime = level.time;
    }
  }
  if ( weapDef->guidedMissileType == MISSILE_GUIDANCE_TVGUIDED )
  {
    if ( bolt->model
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 4566, 0, "%s", "!bolt->model") )
    {
      __debugbreak();
    }
    if ( weapDef->projectileModel )
    {
      Name = (char *)XModelGetName(weapDef->projectileModel);
      G_SetModel(bolt, Name);
    }
  }
  if ( !weapDef->iProjectileSpeed
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          4572,
          0,
          "%s",
          "weapDef->iProjectileSpeed") )
  {
    __debugbreak();
  }
  bolt->mover.aDecelTime = (float)((float)weapDef->destabilizeDistance / (float)weapDef->iProjectileSpeed) * 1000.0;
  bolt->s.lerp.u.turret.ownerNum = 0;
  if ( weapDef->destabilizationRateTime == 0.0 )
    v10 = bolt->flags | 0x20000;
  else
    v10 = bolt->flags | parent->flags & 0x20000;
  bolt->flags = v10;
  G_DObjUpdate(bolt);
  obj = Com_GetServerDObj(bolt->s.number);
  if ( obj )
  {
    Contents = DObjGetContents(obj);
    bolt->r.contents |= Contents;
    DObjCalcBounds(obj, bolt->r.mins, bolt->r.maxs);
  }
  SV_LinkEntity((int)&savedregs, bolt);
  if ( weapDef->guidedMissileType == MISSILE_GUIDANCE_TVGUIDED )
    G_LinkPlayerToRocket(bolt, parent);
  if ( target )
    Scr_AddEntity(target, SCRIPTINSTANCE_SERVER);
  else
    Scr_AddUndefined(SCRIPTINSTANCE_SERVER);
  v12 = (char *)BG_WeaponName(weaponIndex);
  Scr_AddString(v12, SCRIPTINSTANCE_SERVER);
  Scr_AddEntity(bolt, SCRIPTINSTANCE_SERVER);
  Scr_Notify(parent, scr_const.missile_fire, 3u);
  return bolt;
}

void __cdecl G_LinkPlayerToRocket(gentity_s *ent, gentity_s *player)
{
  gentity_s *turret; // [esp+8h] [ebp-8h]
  gclient_s *client; // [esp+Ch] [ebp-4h]

  client = player->client;
  if ( !client && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 827, 0, "%s", "client") )
    __debugbreak();
  if ( (client->ps.eFlags & 0x300) != 0 )
  {
    turret = &g_entities[client->ps.viewlocked_entNum];
    if ( turret->s.eType == 11 )
      G_ClientStopUsingTurret(turret);
  }
  AssignToSmallerType<short>(&client->ps.viewlocked_entNum, ent->s.number);
  client->ps.weapFlags |= 0x200u;
  client->ps.weapFlags &= ~0x100000u;
  client->ps.linkAngles[0] = client->ps.viewangles[0];
  client->ps.linkAngles[1] = client->ps.viewangles[1];
  client->ps.linkAngles[2] = client->ps.viewangles[2];
}

void __cdecl InitRocketTimer(gentity_s *bolt, const WeaponDef *weapDef)
{
  gclient_s *ps; // [esp+0h] [ebp-8h]

  if ( !bolt && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 4367, 0, "%s", "bolt") )
    __debugbreak();
  if ( !weapDef && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 4368, 0, "%s", "weapDef") )
    __debugbreak();
  bolt->nextthink = level.time + (int)(float)(weapDef->projLifetime * 1000.0);
  if ( weapDef->guidedMissileType == MISSILE_GUIDANCE_TVGUIDED )
  {
    if ( !EntHandle::isDefined(&bolt->parent)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
            4375,
            0,
            "%s",
            "bolt->parent.isDefined()") )
    {
      __debugbreak();
    }
    if ( !EntHandle::ent(&bolt->parent)->client
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
            4376,
            0,
            "%s",
            "bolt->parent.ent()->client") )
    {
      __debugbreak();
    }
    ps = EntHandle::ent(&bolt->parent)->client;
    UpdateGuidedMissileFuelTime(bolt, 1);
    BG_SetFuelTankTime(&ps->ps, ps->ps.weapon, 0);
  }
  if ( bolt->nextthink > level.time + 60000 )
    bolt->nextthink = level.time + 60000;
}

gentity_s *__cdecl G_DropBomb(
        gentity_s *parent,
        unsigned int weaponIndex,
        float *start,
        float *dir,
        const float *gunVel,
        gentity_s *target,
        const float *targetOffset)
{
  int v7; // eax
  long double v8; // st7
  int v9; // ecx
  $0D33AF6AB483EB176B99DAC6E021D6CF *v11; // [esp+10h] [ebp-A4h]
  $0D33AF6AB483EB176B99DAC6E021D6CF *v12; // [esp+18h] [ebp-9Ch]
  $0D33AF6AB483EB176B99DAC6E021D6CF *v13; // [esp+1Ch] [ebp-98h]
  float v14; // [esp+20h] [ebp-94h]
  float *currentOrigin; // [esp+30h] [ebp-84h]
  float *v16; // [esp+40h] [ebp-74h]
  float *trBase; // [esp+44h] [ebp-70h]
  float *trDelta; // [esp+48h] [ebp-6Ch]
  float iProjectileSpeed; // [esp+4Ch] [ebp-68h]
  float *v20; // [esp+50h] [ebp-64h]
  float *v21; // [esp+54h] [ebp-60h]
  float *v22; // [esp+58h] [ebp-5Ch]
  float v23; // [esp+5Ch] [ebp-58h]
  float sinT; // [esp+78h] [ebp-3Ch]
  float theta; // [esp+7Ch] [ebp-38h]
  float r; // [esp+80h] [ebp-34h]
  float v27[3]; // [esp+84h] [ebp-30h] BYREF
  float cosT; // [esp+90h] [ebp-24h]
  float up[3]; // [esp+94h] [ebp-20h] BYREF
  float right[3]; // [esp+A0h] [ebp-14h] BYREF
  gentity_s *bolt; // [esp+ACh] [ebp-8h]
  const WeaponDef *weapDef; // [esp+B0h] [ebp-4h]
  int savedregs; // [esp+B4h] [ebp+0h] BYREF

  if ( !parent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 4621, 0, "%s", "parent") )
    __debugbreak();
  weapDef = BG_GetWeaponDef(weaponIndex);
  if ( !weapDef && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp", 4624, 0, "%s", "weapDef") )
    __debugbreak();
  Vec3Normalize(dir);
  if ( weapDef->guidedMissileType == MISSILE_GUIDANCE_JAVELIN )
  {
    dir[2] = dir[2] + MYJAVELINOFFSET_0;
    Vec3Normalize(dir);
    AngleVectors(parent->s.lerp.apos.trBase, 0, right, 0);
    v23 = MYJAVELINOFFSET_RIGHT_0;
    *start = (float)(MYJAVELINOFFSET_RIGHT_0 * right[0]) + *start;
    start[1] = (float)(v23 * right[1]) + start[1];
    start[2] = (float)(v23 * right[2]) + start[2];
  }
  bolt = G_Spawn();
  Scr_SetString(&bolt->classname, scr_const.rocket, SCRIPTINSTANCE_SERVER);
  bolt->s.eType = 4;
  AssignToSmallerType<unsigned short>(&bolt->s.weapon, weaponIndex);
  bolt->s.lerp.u.actor.index.actorNum = level.time;
  if ( parent->client )
  {
    v7 = CalcMissileNoDrawTime((float)weapDef->iProjectileSpeed);
    bolt->s.lerp.u.actor.index.actorNum += v7;
  }
  EntHandle::setEnt(&bolt->r.ownerNum, parent);
  EntHandle::setEnt(&bolt->parent, parent);
  bolt->clipmask = (int)&cls.recentServers[7544].adr.port + 3;
  bolt->handler = 12;
  InitRocketTimer(bolt, weapDef);
  if ( targetOffset )
  {
    v22 = &bolt->mover.pos3[2];
    bolt->mover.pos3[2] = *targetOffset;
    v22[1] = targetOffset[1];
    v22[2] = targetOffset[2];
  }
  else
  {
    v21 = &bolt->mover.pos3[2];
    bolt->mover.pos3[2] = *(float *)&FLOAT_0_0;
    v21[1] = *(float *)&FLOAT_0_0;
    v21[2] = *(float *)&FLOAT_0_0;
  }
  if ( parent->client )
    bolt->missile.team = parent->client->sess.cs.team;
  else
    bolt->missile.team = TEAM_FREE;
  G_BroadcastEntity(bolt);
  bolt->s.lerp.pos.trType = 6;
  if ( weapDef->timeToAccelerate <= 0.0 )
  {
    trDelta = bolt->s.lerp.pos.trDelta;
    iProjectileSpeed = (float)weapDef->iProjectileSpeed;
    bolt->s.lerp.pos.trDelta[0] = iProjectileSpeed * *dir;
    trDelta[1] = iProjectileSpeed * dir[1];
    trDelta[2] = iProjectileSpeed * dir[2];
  }
  else
  {
    bolt->s.lerp.pos.trType = 1;
    v20 = bolt->s.lerp.pos.trDelta;
    bolt->s.lerp.pos.trDelta[0] = *(float *)&FLOAT_0_0;
    v20[1] = *(float *)&FLOAT_0_0;
    v20[2] = *(float *)&FLOAT_0_0;
  }
  bolt->s.lerp.pos.trTime = level.time;
  trBase = bolt->s.lerp.pos.trBase;
  bolt->s.lerp.pos.trBase[0] = *start;
  trBase[1] = start[1];
  trBase[2] = start[2];
  v16 = bolt->s.lerp.pos.trDelta;
  bolt->s.lerp.pos.trDelta[0] = *gunVel;
  v16[1] = gunVel[1];
  v16[2] = gunVel[2];
  if ( ((LODWORD(bolt->s.lerp.pos.trDelta[0]) & 0x7F800000) == 0x7F800000
     || (LODWORD(bolt->s.lerp.pos.trDelta[1]) & 0x7F800000) == 0x7F800000
     || (LODWORD(bolt->s.lerp.pos.trDelta[2]) & 0x7F800000) == 0x7F800000)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          4691,
          0,
          "%s",
          "!IS_NAN((bolt->s.lerp.pos.trDelta)[0]) && !IS_NAN((bolt->s.lerp.pos.trDelta)[1]) && !IS_NAN((bolt->s.lerp.pos.trDelta)[2])") )
  {
    __debugbreak();
  }
  bolt->s.lerp.pos.trDelta[0] = (float)(int)bolt->s.lerp.pos.trDelta[0];
  bolt->s.lerp.pos.trDelta[1] = (float)(int)bolt->s.lerp.pos.trDelta[1];
  bolt->s.lerp.pos.trDelta[2] = (float)(int)bolt->s.lerp.pos.trDelta[2];
  currentOrigin = bolt->r.currentOrigin;
  bolt->r.currentOrigin[0] = *start;
  currentOrigin[1] = start[1];
  currentOrigin[2] = start[2];
  vectoangles(dir, bolt->r.currentAngles);
  G_SetAngle(bolt, bolt->r.currentAngles);
  if ( weapDef->projectileCurvature > 0.0 )
  {
    bolt->s.lerp.pos.trType = 1;
    AngleVectors(bolt->r.currentAngles, 0, v27, up);
    theta = G_random() * 360.0;
    r = G_random() * weapDef->projectileCurvature;
    v8 = (float)(theta * 0.017453292);
    cosT = cos(v8);
    sinT = sin(v8);
    v13 = &bolt->missile.44;
    v14 = r * cosT;
    bolt->mover.pos2[2] = (float)(r * cosT) * v27[0];
    v13->missile.curvature[1] = v14 * v27[1];
    v13->missile.curvature[2] = v14 * v27[2];
    v11 = &bolt->missile.44;
    v12 = &bolt->missile.44;
    bolt->mover.pos2[2] = (float)((float)(r * sinT) * up[0]) + bolt->mover.pos2[2];
    v11->missile.curvature[1] = (float)((float)(r * sinT) * up[1]) + v12->missile.curvature[1];
    v11->missile.curvature[2] = (float)((float)(r * sinT) * up[2]) + v12->missile.curvature[2];
    if ( ((bolt->missile.grenade.effectIndex & 0x7F800000) == 0x7F800000
       || (LODWORD(bolt->mover.pos3[0]) & 0x7F800000) == 0x7F800000
       || (LODWORD(bolt->mover.pos3[1]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
            4716,
            0,
            "%s",
            "!IS_NAN((bolt->missile.missile.curvature)[0]) && !IS_NAN((bolt->missile.missile.curvature)[1]) && !IS_NAN((b"
            "olt->missile.missile.curvature)[2])") )
    {
      __debugbreak();
    }
  }
  if ( weapDef->guidedMissileType )
  {
    if ( weapDef->guidedMissileType != MISSILE_GUIDANCE_BALLISTIC )
    {
      bolt->s.lerp.pos.trType = 1;
      bolt->s.lerp.apos.trType = 1;
      bolt->s.lerp.pos.trDuration = 0;
      if ( weapDef->guidedMissileType == MISSILE_GUIDANCE_JAVELIN )
      {
        bolt->s.lerp.pos.trType = 6;
        bolt->s.lerp.pos.trTime = level.time;
        bolt->missile.missile.stage = MISSILESTAGE_SOFTLAUNCH;
        bolt->missile.missile.flightMode = !G_TargetAttackProfileTop(target);
      }
    }
  }
  if ( weapDef->guidedMissileType == MISSILE_GUIDANCE_BALLISTIC )
  {
    bolt->s.lerp.apos.trType = 3;
    bolt->s.lerp.apos.trTime = level.time;
    bolt->s.lerp.apos.trDelta[2] = FLOAT_75_0;
  }
  if ( !weapDef->iProjectileSpeed
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_missile.cpp",
          4751,
          0,
          "%s",
          "weapDef->iProjectileSpeed") )
  {
    __debugbreak();
  }
  bolt->mover.aDecelTime = (float)((float)weapDef->destabilizeDistance / (float)weapDef->iProjectileSpeed) * 1000.0;
  bolt->s.lerp.u.turret.ownerNum = 0;
  if ( weapDef->destabilizationRateTime == 0.0 )
    v9 = bolt->flags | 0x20000;
  else
    v9 = bolt->flags | parent->flags & 0x20000;
  bolt->flags = v9;
  SV_LinkEntity((int)&savedregs, bolt);
  return bolt;
}

