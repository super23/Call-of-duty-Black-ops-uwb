#include "bg_wind.h"

void __cdecl BG_Wind_RegisterDvars()
{
  g_EnableGlobalWind = _Dvar_RegisterBool("enable_global_wind", 0, 0x1000u, "enable wind effect");
  g_EnableGrassWind = _Dvar_RegisterBool("enable_grass_wind", 0, 0x1000u, "enable grass wind effect");
  g_GlobalWindVector = _Dvar_RegisterVec3(
                         "wind_global_vector",
                         COERCE_UNSIGNED_INT(1.0),
                         COERCE_UNSIGNED_INT(0.0),
                         COERCE_UNSIGNED_INT(0.0),
                         -1000.0,
                         1000.0,
                         0x1000u,
                         "global wind force in units/sec. This is also the hi-level wind");
  g_GlobalLowWindAltitude = _Dvar_RegisterFloat(
                              "wind_global_low_altitude",
                              0.0,
                              -100000.0,
                              100000.0,
                              0x1000u,
                              "altitude of global low-level wind");
  g_GlobalHiWindAltitude = _Dvar_RegisterFloat(
                             "wind_global_hi_altitude",
                             0.0,
                             -100000.0,
                             100000.0,
                             0x1000u,
                             "altitude of global hi-level wind");
  g_GlobalLowWindStrengthPercentage = _Dvar_RegisterFloat(
                                        "wind_global_low_strength_percent",
                                        1.0,
                                        -20.0,
                                        20.0,
                                        0x1000u,
                                        "strength of global low-level wind, expressed as a percentage of wind_global_vector");
  g_GlobalWindLeafScale = _Dvar_RegisterFloat(
                            "wind_leaf_scale",
                            176.0,
                            0.0,
                            50000.0,
                            0x1000u,
                            "global wind grass tension (stiffness)");
  g_GlobalWindGrassGustInterval = _Dvar_RegisterFloat(
                                    "wind_grass_gustinterval",
                                    250.0,
                                    1.0,
                                    50000.0,
                                    0x1000u,
                                    "global wind force gust interval");
  g_GlobalWindGrassScale = _Dvar_RegisterFloat(
                             "wind_grass_scale",
                             10000.0,
                             1.0,
                             50000.0,
                             0x1000u,
                             "global wind force scaler");
  g_GlobalWindGrassTension = _Dvar_RegisterFloat(
                               "wind_grass_tension",
                               0.001,
                               0.0,
                               1.0,
                               0x1000u,
                               "global wind grass tension (stiffness)");
  g_GlobalWindGustSpeedMultiplier = _Dvar_RegisterFloat(
                                      "wind_grass_gust_speed",
                                      1.0,
                                      0.0,
                                      10000.0,
                                      0x1000u,
                                      "speed multiplier for gusts");
  g_GlobalWindGustRadius = _Dvar_RegisterFloat(
                             "wind_grass_gust_radius",
                             400.0,
                             0.0,
                             10000.0,
                             0x1000u,
                             "size of global wind gust effector");
  g_GlobalWindGustStrength = _Dvar_RegisterFloat(
                               "wind_grass_gust_strength",
                               0.5,
                               0.0,
                               1.0,
                               0x1000u,
                               "percentage to bump the wind in a gust");
  g_GlobalWindGustDistance = _Dvar_RegisterFloat(
                               "wind_grass_gust_distance",
                               2000.0,
                               0.0,
                               100000.0,
                               0x1000u,
                               "max distance at which to place gusts");
  g_GlobalWindDebugDisplay = _Dvar_RegisterBool("wind_debug_display", 0, 0x1080u, "shows wind debug display");
  g_GlobalTreeBend = _Dvar_RegisterFloat("tree_bend", 1.0, 0.0, 5.0, 0x1000u, "bend amount of tree trunk");
  g_GlobalTreeRandom = _Dvar_RegisterFloat(
                         "tree_random",
                         0.1,
                         0.0,
                         5.0,
                         0x1000u,
                         "random addition to bend amount of tree trunk");
  g_GlobalTreeFrequency = _Dvar_RegisterFloat(
                            "tree_frequency",
                            2.0,
                            0.0,
                            20.0,
                            0x1000u,
                            "frequency of tree bend oscillation");
  g_localWindVector = _Dvar_RegisterVec4(
                        "wind_local_vector",
                        COERCE_UNSIGNED_INT(0.0),
                        COERCE_UNSIGNED_INT(1.0),
                        COERCE_UNSIGNED_INT(0.0),
                        COERCE_UNSIGNED_INT(0.0),
                        -1000000.0,
                        1000000.0,
                        0x1000u,
                        "localized wind force in units/sec");
  g_localWindAmplitude = _Dvar_RegisterVec4(
                           "wind_local_amplitude",
                           COERCE_UNSIGNED_INT(1.0),
                           COERCE_UNSIGNED_INT(1.0),
                           COERCE_UNSIGNED_INT(1.0),
                           COERCE_UNSIGNED_INT(0.0),
                           -1000000.0,
                           1000000.0,
                           0x1000u,
                           "localized wind amplitude");
  g_localWindFrequency = _Dvar_RegisterVec3(
                           "wind_local_frequency",
                           COERCE_UNSIGNED_INT(6.0),
                           COERCE_UNSIGNED_INT(6.0),
                           COERCE_UNSIGNED_INT(6.0),
                           0.0,
                           40.0,
                           0x1000u,
                           "frequency of tree bend oscillation");
  g_localWindGustRadius = _Dvar_RegisterFloat(
                            "wind_local_gust_radius",
                            1000.0,
                            0.0,
                            10000.0,
                            0x1000u,
                            "size of local wind gust effector");
  g_localWindGustStrength = _Dvar_RegisterFloat(
                              "wind_local_gust_strength",
                              0.80000001,
                              0.0,
                              1.0,
                              0x1000u,
                              "percentage to bump the wind in a local gust");
}

void __cdecl DisplayWindDebug()
{
  float value; // [esp+1Ch] [ebp-ACh]
  float start[3]; // [esp+28h] [ebp-A0h] BYREF
  float end[3]; // [esp+34h] [ebp-94h] BYREF
  float debugCircleDir[3]; // [esp+40h] [ebp-88h] BYREF
  int i; // [esp+4Ch] [ebp-7Ch]
  float loWindStart[3]; // [esp+50h] [ebp-78h] BYREF
  float pos[3]; // [esp+5Ch] [ebp-6Ch] BYREF
  float vertLineStart[3]; // [esp+68h] [ebp-60h] BYREF
  float vertColor[4]; // [esp+74h] [ebp-54h] BYREF
  float hiWindEnd[3]; // [esp+84h] [ebp-44h] BYREF
  float lineColor[4]; // [esp+90h] [ebp-38h] BYREF
  float hiWindStart[3]; // [esp+A0h] [ebp-28h] BYREF
  float vertLineEnd[3]; // [esp+ACh] [ebp-1Ch] BYREF
  float loWindEnd[3]; // [esp+B8h] [ebp-10h] BYREF
  cg_s *cgameGlob; // [esp+C4h] [ebp-4h]

  if ( g_GlobalWindDebugDisplay )
  {
    cgameGlob = CG_GetLocalClientGlobals(0);
    if ( g_GlobalWindDebugDisplay->current.enabled )
    {
      if ( !debugValLastFrame && cgameGlob )
      {
        windDebugPos[0] = (float)(100.0 * cgameGlob->refdef.viewaxis[0][0]) + cgameGlob->refdef.vieworg[0];
        *(float *)&dword_E867B4 = (float)(100.0 * cgameGlob->refdef.viewaxis[0][1]) + cgameGlob->refdef.vieworg[1];
        *(float *)&dword_E867B8 = (float)(100.0 * cgameGlob->refdef.viewaxis[0][2]) + cgameGlob->refdef.vieworg[2];
      }
      vertLineStart[0] = windDebugPos[0];
      LODWORD(vertLineStart[1]) = dword_E867B4;
      vertLineStart[2] = FLOAT_10000_0;
      vertLineEnd[0] = windDebugPos[0];
      LODWORD(vertLineEnd[1]) = dword_E867B4;
      vertLineEnd[2] = FLOAT_N10000_0;
      loWindStart[0] = windDebugPos[0];
      LODWORD(loWindStart[1]) = dword_E867B4;
      loWindStart[2] = g_GlobalLowWindAltitude->current.value;
      hiWindStart[0] = windDebugPos[0];
      LODWORD(hiWindStart[1]) = dword_E867B4;
      hiWindStart[2] = g_GlobalHiWindAltitude->current.value;
      value = g_GlobalLowWindStrengthPercentage->current.value;
      loWindEnd[0] = (float)(value * g_GlobalWindVector->current.value) + windDebugPos[0];
      loWindEnd[1] = (float)(value * g_GlobalWindVector->current.vector[1]) + *(float *)&dword_E867B4;
      loWindEnd[2] = (float)(value * g_GlobalWindVector->current.vector[2]) + loWindStart[2];
      hiWindEnd[0] = windDebugPos[0] + g_GlobalWindVector->current.value;
      hiWindEnd[1] = *(float *)&dword_E867B4 + g_GlobalWindVector->current.vector[1];
      hiWindEnd[2] = hiWindStart[2] + g_GlobalWindVector->current.vector[2];
      vertColor[0] = FLOAT_0_75;
      vertColor[1] = FLOAT_0_75;
      vertColor[2] = FLOAT_0_75;
      vertColor[3] = FLOAT_1_0;
      CG_DebugLine(vertLineStart, vertLineEnd, vertColor, 0, 2);
      CG_DebugLine(loWindEnd, hiWindEnd, vertColor, 0, 2);
      lineColor[0] = FLOAT_1_0;
      lineColor[1] = *(float *)&FLOAT_0_0;
      lineColor[2] = FLOAT_1_0;
      lineColor[3] = FLOAT_1_0;
      CG_DebugLine(loWindStart, loWindEnd, lineColor, 0, 2);
      CG_DebugLine(hiWindStart, hiWindEnd, lineColor, 0, 2);
      for ( i = 0; i < 16; ++i )
      {
        pos[0] = g_GrassWind[0][i].pos[0];
        LODWORD(pos[1]) = dword_E8653C[7 * i];
        pos[2] = *(float *)&FLOAT_0_0;
        start[0] = pos[0];
        start[1] = pos[1];
        end[0] = pos[0];
        end[1] = pos[1];
        end[2] = *(float *)&FLOAT_0_0;
        start[2] = FLOAT_1000_0;
        CM_TracePointDown(start, end, 2097, (int)&bg_vehicleInfos[11].rotorTailStartFx[20], pos, 0, 0);
        G_DebugStar(pos, lineColor, 3);
        debugCircleDir[0] = *(float *)&FLOAT_0_0;
        debugCircleDir[1] = *(float *)&FLOAT_0_0;
        debugCircleDir[2] = FLOAT_1_0;
        G_DebugCircleEx(pos, g_GlobalWindGustRadius->current.value, debugCircleDir, colorWhite, 1, 1);
      }
    }
    debugValLastFrame = g_GlobalWindDebugDisplay->current.enabled;
  }
}

void __cdecl SeedVariantWind(
        VariantWind *wind,
        int i,
        int start_time,
        const float *seedWindVector,
        float windLeafScale)
{
  int v5; // esi
  float v6; // [esp+Ch] [ebp-3Ch]
  float v7; // [esp+10h] [ebp-38h]
  float v8; // [esp+14h] [ebp-34h]
  float v9; // [esp+1Ch] [ebp-2Ch]
  float v10; // [esp+20h] [ebp-28h]
  float v11; // [esp+24h] [ebp-24h]
  float v_scale; // [esp+28h] [ebp-20h]
  float varint_4; // [esp+3Ch] [ebp-Ch]
  float varint_8; // [esp+40h] [ebp-8h]

  wind->reset_timer = start_time - 1875 * (16 - i);
  wind->gust_start = start_time - (int)(float)(g_GlobalWindGrassGustInterval->current.value / 16.0) * (16 - i);
  v5 = (int)(float)(g_GlobalWindGrassGustInterval->current.value / 2.0);
  wind->gust_dur = rand() % v5 + v5;
  varint_4 = g_WindVariants[(i + 1) % 17];
  varint_8 = g_WindVariants[(i + 2) % 17];
  wind->wind_vector[0] = *seedWindVector * g_WindVariants[i % 17];
  wind->wind_vector[1] = seedWindVector[1] * varint_4;
  wind->wind_vector[2] = seedWindVector[2] * varint_8;
  wind->wind_vector[0] = *seedWindVector + wind->wind_vector[0];
  wind->wind_vector[1] = seedWindVector[1] + wind->wind_vector[1];
  wind->wind_vector[2] = seedWindVector[2] + wind->wind_vector[2];
  v_scale = (float)((float)((float)(wind->wind_vector[0] * wind->wind_vector[0])
                          + (float)(wind->wind_vector[1] * wind->wind_vector[1]))
                  + (float)(wind->wind_vector[2] * wind->wind_vector[2]))
          / (float)(windLeafScale * windLeafScale);
  v9 = (float)(rand() - 0x3FFF) / 32767.0;
  v10 = (float)(rand() - 0x3FFF) / 32767.0;
  v11 = (float)(rand() - 0x3FFF) / 32767.0;
  wind->rustle_amplitude[0] = v_scale * v9;
  wind->rustle_amplitude[1] = v_scale * v10;
  wind->rustle_amplitude[2] = v_scale * v11;
  v6 = (float)(rand() - 0x3FFF) / 32767.0;
  v7 = (float)(rand() - 0x3FFF) / 32767.0;
  v8 = (float)(rand() - 0x3FFF) / 32767.0;
  wind->rustle_angular_frequency[0] = (float)(v_scale * 3.1415927) * v6;
  wind->rustle_angular_frequency[1] = (float)(v_scale * 3.1415927) * v7;
  wind->rustle_angular_frequency[2] = (float)(v_scale * 3.1415927) * v8;
}

void __cdecl BG_SeedVarintWinds(unsigned int variant_seed, int start_time)
{
  int j; // [esp+4h] [ebp-8h]
  int i; // [esp+8h] [ebp-4h]
  int ia; // [esp+8h] [ebp-4h]
  int ib; // [esp+8h] [ebp-4h]

  srand(variant_seed);
  for ( i = 0; i < 17; ++i )
    g_WindVariants[i] = (float)((float)(rand() - 0x3FFF) * 0.1) / 32767.0;
  for ( ia = 0; ia < 16; ++ia )
    SeedVariantWind(
      &g_VariantWind[ia],
      ia,
      start_time,
      &g_GlobalWindVector->current.value,
      g_GlobalWindLeafScale->current.value);
  SeedVariantWind(
    &g_localWind.wind,
    0,
    start_time,
    &g_GlobalWindVector->current.value,
    g_GlobalWindLeafScale->current.value);
  for ( j = 0; j < 1; ++j )
  {
    for ( ib = 0; ib < 16; ++ib )
    {
      dword_E86550[112 * j + 7 * ib] = 0;
      dword_E8654C[112 * j + 7 * ib] = 0;
    }
  }
}

void  BG_UpdateVariantWind(
        int a1@<esi>,
        VariantWind *wind,
        int variantIndex,
        const float *seedWindVector,
        int cur_time)
{
  int v5; // esi
  long double v6; // [esp-1Ch] [ebp-2Ch]
  float v7; // [esp-14h] [ebp-24h]
  float v8; // [esp-10h] [ebp-20h]
  float v9; // [esp-Ch] [ebp-1Ch]
  float v10; // [esp-4h] [ebp-14h]
  float v; // [esp+0h] [ebp-10h]

  LODWORD(v6) = a1;
  if ( wind->reset_timer <= cur_time )
  {
    wind->reset_timer = cur_time + 30000;
    v10 = g_WindVariants[(variantIndex + 1) % 17];
    v = g_WindVariants[(variantIndex + 2) % 17];
    wind->wind_vector[0] = *seedWindVector * g_WindVariants[variantIndex % 17];
    wind->wind_vector[1] = seedWindVector[1] * v10;
    wind->wind_vector[2] = seedWindVector[2] * v;
    wind->wind_vector[0] = *seedWindVector + wind->wind_vector[0];
    wind->wind_vector[1] = seedWindVector[1] + wind->wind_vector[1];
    wind->wind_vector[2] = seedWindVector[2] + wind->wind_vector[2];
  }
  if ( wind->gust_dur + wind->gust_start <= cur_time )
  {
    wind->gust_start = cur_time;
    v5 = (int)(float)(g_GlobalWindGrassGustInterval->current.value / 2.0);
    wind->gust_dur = rand() % v5 + v5;
    v9 = (float)((float)(wind->wind_vector[0] * wind->wind_vector[0])
               + (float)(wind->wind_vector[1] * wind->wind_vector[1]))
       + (float)(wind->wind_vector[2] * wind->wind_vector[2]);
    v8 = -v9;
    v7 = random() * v8 + v9 + 1.0;
    __libm_sse2_log(v6);
    wind->blade_acceleration = v7 / g_GlobalWindGrassScale->current.value;
  }
  wind->blade_velocity = wind->blade_velocity + wind->blade_acceleration;
  wind->blade_acceleration = (float)(COERCE_FLOAT(g_GlobalWindGrassTension->current.integer ^ _mask__NegFloat_)
                                   * wind->blade_velocity)
                           + wind->blade_acceleration;
}

void  CG_UpdateWind(int a1@<ebp>, int a2@<esi>, int cur_time)
{
  double v3; // xmm0_8
  double v4; // xmm0_8
  double v5; // st7
  int v6; // eax
  long double v7; // [esp+8h] [ebp-10Ch]
  unsigned int v8[3]; // [esp+18h] [ebp-FCh] BYREF
  FX_WindInfo fxWind; // [esp+24h] [ebp-F0h]
  float v10; // [esp+40h] [ebp-D4h]
  float treeBend; // [esp+44h] [ebp-D0h]
  float v12; // [esp+48h] [ebp-CCh]
  float v13; // [esp+4Ch] [ebp-C8h]
  float v14; // [esp+50h] [ebp-C4h]
  GrassWind *v15; // [esp+54h] [ebp-C0h]
  GrassWind *v16; // [esp+58h] [ebp-BCh]
  float *v17; // [esp+5Ch] [ebp-B8h]
  float v18; // [esp+60h] [ebp-B4h]
  float v19; // [esp+64h] [ebp-B0h]
  float v20; // [esp+68h] [ebp-ACh]
  GrassWind *v21; // [esp+6Ch] [ebp-A8h]
  GrassWind *v22; // [esp+70h] [ebp-A4h]
  float scalar; // [esp+74h] [ebp-A0h]
  float v24; // [esp+78h] [ebp-9Ch]
  float v25; // [esp+7Ch] [ebp-98h]
  float v26; // [esp+80h] [ebp-94h]
  float rot[3]; // [esp+84h] [ebp-90h] BYREF
  float v28; // [esp+90h] [ebp-84h]
  float dist[3]; // [esp+94h] [ebp-80h]
  centity_s *Entity; // [esp+A0h] [ebp-74h]
  float v31[2]; // [esp+A4h] [ebp-70h] BYREF
  int v32; // [esp+B0h] [ebp-64h]
  float v33; // [esp+B4h] [ebp-60h]
  float v34; // [esp+B8h] [ebp-5Ch]
  float v35; // [esp+BCh] [ebp-58h]
  cg_s *LocalClientGlobals; // [esp+C0h] [ebp-54h]
  int n; // [esp+C4h] [ebp-50h]
  float v38; // [esp+C8h] [ebp-4Ch] BYREF
  float windRustleAngle[3]; // [esp+D4h] [ebp-40h]
  float v40; // [esp+E0h] [ebp-34h]
  int m; // [esp+E4h] [ebp-30h]
  int k; // [esp+E8h] [ebp-2Ch]
  DvarValue *p_current; // [esp+ECh] [ebp-28h]
  int i; // [esp+F4h] [ebp-20h]
  int v46; // [esp+104h] [ebp-10h]
  int v47; // [esp+108h] [ebp-Ch]
  void *v48; // [esp+10Ch] [ebp-8h]
  void *retaddr; // [esp+114h] [ebp+0h]

  v47 = a1;
  v48 = retaddr;
  LODWORD(v7) = a2;
  PIXBeginNamedEvent(-1, "CG_UpdateWind");
  if ( (_S1_0 & 1) == 0 )
  {
    _S1_0 |= 1u;
    last_time = cur_time;
  }
  if ( g_EnableGlobalWind->current.enabled )
  {
    if ( cg_paused->current.integer )
    {
      last_time = cur_time;
      if ( GetCurrentThreadId() == g_DXDeviceThread )
        goto LABEL_5;
    }
    else
    {
      i = (int)&g_GlobalWindVector->current;
      if ( g_GlobalWindVector->current.value != g_LastGlobalWindVector[0]
        || *(float *)(i + 4) != g_LastGlobalWindVector[1]
        || *(float *)(i + 8) != g_LastGlobalWindVector[2]
        || *(float *)(i + 12) != g_LastGlobalWindVector[3] )
      {
        p_current = &g_GlobalWindVector->current;
        *(DvarValue *)g_LastGlobalWindVector = g_GlobalWindVector->current;
        BG_SeedVarintWinds(0x7FFFu, cur_time);
      }
      for ( k = 0; k < 16; ++k )
      {
        BG_UpdateVariantWind(a2, &g_VariantWind[k], k, &g_GlobalWindVector->current.value, cur_time);
        for ( m = 0; m < 3; ++m )
        {
          v40 = (float)((float)cur_time / 1000.0) * g_VariantWind[k].rustle_angular_frequency[m];
          windRustleAngle[2] = fmod(v40, 6.2831855);
          windRustleAngle[1] = windRustleAngle[2];
          v3 = windRustleAngle[2];
          __libm_sse2_sin(v7);
          *(float *)&v3 = v3;
          windRustleAngle[0] = *(float *)&v3;
          windRustleAngle[m + 1] = g_VariantWind[k].rustle_amplitude[m] * *(float *)&v3;
        }
        R_SetVariantWindRustleAngle(k, &v38);
        R_SetVariantWindSpringAngle(k, g_VariantWind[k].blade_velocity);
      }
      if ( g_EnableGrassWind->current.enabled )
      {
        for ( n = 0; n < 1; ++n )
        {
          if ( CL_LocalClient_IsActive(n) )
          {
            LocalClientGlobals = CG_GetLocalClientGlobals(n);
            if ( LocalClientGlobals )
            {
              for ( k = 0; k < 16; ++k )
              {
                if ( k || g_localWind.entnum == 1023 )
                {
                  a2 = 28 * k;
                  if ( dword_E8654C[112 * n + 7 * k] + dword_E86550[112 * n + 7 * k] >= cur_time )
                  {
                    v20 = (float)(cur_time - last_time) / 1000.0;
                    v19 = v20;
                    v17 = (float *)&dword_E86540[112 * n + 7 * k];
                    v18 = v20 * *v17;
                    v19 = v20 * v17[1];
                    v16 = &g_GrassWind[n][k];
                    v15 = v16;
                    v16->pos[0] = v16->pos[0] + v18;
                    v16->pos[1] = v15->pos[1] + v19;
                  }
                  else
                  {
                    *(float *)&dword_E86540[112 * n + 7 * k] = (float)(g_GlobalWindVector->current.value
                                                                     * g_WindVariants[k % 17])
                                                             + g_GlobalWindVector->current.value;
                    *(float *)&dword_E86544[112 * n + 7 * k] = (float)(g_GlobalWindVector->current.vector[1]
                                                                     * g_WindVariants[(k + 1) % 17])
                                                             + g_GlobalWindVector->current.vector[1];
                    *(float *)&dword_E86540[112 * n + 7 * k] = *(float *)&dword_E86540[112 * n + 7 * k]
                                                             * g_GlobalWindGustSpeedMultiplier->current.value;
                    *(float *)&dword_E86544[112 * n + 7 * k] = *(float *)&dword_E86544[112 * n + 7 * k]
                                                             * g_GlobalWindGustSpeedMultiplier->current.value;
                    v5 = flrand(0.1, 1.0);
                    *(float *)&dword_E86548[112 * n + 7 * k] = v5;
                    dword_E86550[112 * n + 7 * k] = cur_time;
                    v6 = irand(5000, 10000);
                    dword_E8654C[112 * n + 7 * k] = v6;
                    LODWORD(dist[0]) = &dword_E86540[112 * n + 7 * k];
                    rot[1] = *(float *)LODWORD(dist[0]);
                    rot[2] = *(float *)(LODWORD(dist[0]) + 4);
                    v28 = *(float *)(LODWORD(dist[0]) + 8);
                    LODWORD(rot[1]) ^= _mask__NegFloat_;
                    LODWORD(rot[2]) ^= _mask__NegFloat_;
                    LODWORD(v28) ^= _mask__NegFloat_;
                    rot[0] = (float)((float)dword_E8654C[112 * n + 7 * k] / 2.0) / 1000.0;
                    rot[1] = rot[0] * rot[1];
                    rot[2] = rot[0] * rot[2];
                    v28 = rot[0] * v28;
                    g_GrassWind[n][k].pos[0] = LocalClientGlobals->refdef.vieworg[0] + rot[1];
                    *(float *)&dword_E8653C[112 * n + 7 * k] = LocalClientGlobals->refdef.vieworg[1] + rot[2];
                    Vec3Normalize(&rot[1]);
                    LODWORD(v24) = LODWORD(rot[2]) ^ _mask__NegFloat_;
                    v25 = rot[1];
                    v26 = *(float *)&FLOAT_0_0;
                    scalar = flrand(
                               COERCE_FLOAT(g_GlobalWindGustDistance->current.integer ^ _mask__NegFloat_),
                               g_GlobalWindGustDistance->current.value);
                    v24 = scalar * v24;
                    v25 = scalar * v25;
                    v26 = scalar * 0.0;
                    v22 = &g_GrassWind[n][k];
                    v21 = v22;
                    v22->pos[0] = v22->pos[0] + v24;
                    v22->pos[1] = v21->pos[1] + v25;
                    v22->dir[0] = v21->dir[0] + v26;
                  }
                  v14 = (float)((float)(cur_time - dword_E86550[112 * n + 7 * k]) / (float)dword_E8654C[112 * n + 7 * k])
                      * 3.1415927;
                  v13 = fmod(v14, 6.2831855);
                  v12 = v13;
                  __libm_sse2_sin(v7);
                  treeBend = v13;
                  R_FoliageSetGrassWindForces(
                    n,
                    g_GrassWind[n][k].pos,
                    g_GlobalWindGustRadius->current.value,
                    (float)(g_GlobalWindGustStrength->current.value * *(float *)&dword_E86548[112 * n + 7 * k]) * v13);
                }
                else
                {
                  BG_UpdateVariantWind(a2, &g_localWind.wind, 0, &g_localWindVector->current.value, cur_time);
                  for ( m = 0; m < 3; ++m )
                  {
                    v35 = (float)((float)cur_time / 1000.0) * g_localWindFrequency->current.vector[m];
                    v34 = fmod(v35, 6.2831855);
                    v33 = v34;
                    v4 = v34;
                    __libm_sse2_sin(v7);
                    *(float *)&v4 = v4;
                    v32 = LODWORD(v4);
                    v31[m] = g_localWindAmplitude->current.vector[m] * *(float *)&v4;
                  }
                  Entity = CG_GetEntity(n, g_localWind.entnum);
                  R_SetLocalWind(Entity->pose.origin, g_GlobalWindGustRadius->current.value, v31);
                  LODWORD(dist[2]) = g_GrassWind[n];
                  LODWORD(dist[1]) = Entity->pose.origin;
                  *(float *)LODWORD(dist[2]) = Entity->pose.origin[0];
                  *(float *)(LODWORD(dist[2]) + 4) = *(float *)(LODWORD(dist[1]) + 4);
                  R_FoliageSetGrassWindForces(
                    n,
                    Entity->pose.origin,
                    g_localWindGustRadius->current.value,
                    g_localWindGustStrength->current.value);
                }
              }
            }
          }
        }
      }
      DisplayWindDebug();
      v10 = (float)((float)cur_time * g_GlobalTreeFrequency->current.value) / 1000.0;
      __libm_sse2_sin(v7);
      fxWind.windVectorMagnitude = v10;
      fxWind.lowWindStrengthPercent = (float)(g_GlobalTreeRandom->current.value * v10) + g_GlobalTreeBend->current.value;
      R_SetWindDirection(&g_GlobalWindVector->current.value, fxWind.lowWindStrengthPercent);
      LODWORD(fxWind.hiWindAltitude) = &g_GlobalWindVector->current;
      v8[0] = g_GlobalWindVector->current.integer;
      v8[1] = LODWORD(g_GlobalWindVector->current.vector[1]);
      v8[2] = LODWORD(g_GlobalWindVector->current.vector[2]);
      LODWORD(fxWind.windVector[1]) = g_GlobalHiWindAltitude->current.integer;
      LODWORD(fxWind.windVector[0]) = g_GlobalLowWindAltitude->current.integer;
      LODWORD(fxWind.windVector[2]) = g_GlobalLowWindStrengthPercentage->current.integer;
      FX_SetGlobalWind((const FX_WindInfo *)v8);
      last_time = cur_time;
      if ( g_DXDeviceThread == GetCurrentThreadId() )
        D3DPERF_EndEvent();
    }
  }
  else
  {
    v46 = 0;
    if ( GetCurrentThreadId() == g_DXDeviceThread )
LABEL_5:
      D3DPERF_EndEvent();
  }
}

void __cdecl BG_GetVariantWindVector(int which, float *out)
{
  VariantWind *v2; // eax

  if ( which >= 16
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_wind.cpp",
          511,
          0,
          "%s",
          "which<MAX_VARIANT_WIND") )
  {
    __debugbreak();
  }
  *out = g_VariantWind[which].wind_vector[0];
  v2 = &g_VariantWind[which];
  out[1] = v2->wind_vector[1];
  out[2] = v2->wind_vector[2];
}

void __cdecl BG_GetGlobalWind(float *out)
{
  DvarValue *p_current; // [esp+0h] [ebp-4h]

  if ( g_GlobalWindVector )
  {
    p_current = &g_GlobalWindVector->current;
    *out = g_GlobalWindVector->current.value;
    out[1] = p_current->vector[1];
    out[2] = p_current->vector[2];
  }
  else
  {
    *out = *(float *)&FLOAT_0_0;
    out[1] = *(float *)&FLOAT_0_0;
    out[2] = *(float *)&FLOAT_0_0;
  }
}

void __cdecl CG_EnableLocalWindSource(int entnum)
{
  g_localWind.entnum = entnum;
}

double __cdecl CG_GetLocalWindSourceScale(int localClientNum, float *pos)
{
  float gust_dist; // [esp+14h] [ebp-Ch]
  centity_s *ent; // [esp+18h] [ebp-8h]
  float ent_dist; // [esp+1Ch] [ebp-4h]

  if ( g_localWind.entnum == 1023 )
    return 0.0;
  ent = CG_GetEntity(localClientNum, g_localWind.entnum);
  gust_dist = g_GlobalWindGustRadius->current.value * g_GlobalWindGustRadius->current.value;
  ent_dist = Vec3DistanceSq(pos, ent->pose.origin);
  if ( ent_dist <= gust_dist )
    return (gust_dist - ent_dist) / gust_dist;
  else
    return 0.0;
}

