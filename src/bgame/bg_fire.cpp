#include "bg_fire.h"

void __cdecl BG_Flame_RegisterDvars()
{
  fire_audio_repeat_duration = _Dvar_RegisterInt(
                                 "fire_audio_repeat_duration",
                                 1500,
                                 0,
                                 10000,
                                 0x1000u,
                                 "The duration in msec in between player fire hurt sounds");
  fire_audio_random_max_duration = _Dvar_RegisterInt(
                                     "fire_audio_random_max_duration",
                                     1000,
                                     0,
                                     10000,
                                     0x1000u,
                                     "This is the max duration in msec in between player fire hurt sounds, a random msec "
                                     "from 0 to this is added to the repeat time");
  fire_burn_time = _Dvar_RegisterInt(
                     "fire_burn_time",
                     3000,
                     0,
                     60000,
                     0x1000u,
                     "the duration in ms that a burn cell will persist");
  fire_world_damage = _Dvar_RegisterInt(
                        "fire_world_damage",
                        5,
                        0,
                        10000,
                        0x1000u,
                        "the amount of damage to apply from a damage volume caused by world fire");
  fire_world_damage_rate = _Dvar_RegisterFloat(
                             "fire_world_damage_rate",
                             1.0,
                             0.0,
                             100.0,
                             0x1000u,
                             "the number of seconds interval over which wolrd fire damage is applied");
  fire_world_damage_duration = _Dvar_RegisterFloat(
                                 "fire_world_damage_duration",
                                 5.0,
                                 0.0,
                                 100.0,
                                 0x1000u,
                                 "the number of seconds over which world fire damage is applied");
  fire_debug = _Dvar_RegisterBool("DebugFireManager", 0, 0x80u, "Display fire manager cell information.");
}

void __cdecl BG_InitFire()
{
  const FxEffectDef *v0; // eax
  const FxEffectDef *v1; // eax
  char fx_name[256]; // [esp+0h] [ebp-108h] BYREF
  int i; // [esp+104h] [ebp-4h]

  gLastBurnParticleSpawnTime = 0;
  if ( !gFireInited )
  {
    gFireInited = 1;
    for ( i = 0; i < 3; ++i )
    {
      sprintf(fx_name, "misc/fx_fire_manager%i_1sec", i + 1);
      v0 = FX_Register(fx_name);
      g_FM_fx_flamethrower_effect[i] = v0;
    }
    for ( i = 0; i < 1; ++i )
    {
      sprintf(fx_name, "misc/fx_fire_fthrow_vert_short_2sec");
      v1 = FX_Register(fx_name);
      g_FM_fx_wall_effect[i] = v1;
    }
  }
  BG_ResetFire();
}

void __cdecl BG_ShutdownFire()
{
  gFireInited = 0;
  gLastBurnParticleSpawnTime = 0;
}

void __cdecl BG_ResetFire()
{
  memset((unsigned __int8 *)g_FM_ActiveCells, 0, sizeof(g_FM_ActiveCells));
  memset((unsigned __int8 *)g_FM_BurnData, 0, sizeof(g_FM_BurnData));
  g_FM_BurnDataActiveCount = 0;
  Sync_VisualBurn_To_FM_State();
}

bool __cdecl TerrainScorch_AllowSurfacesCallback(int surfIndex)
{
  Material *materialHandle; // [esp+4h] [ebp-4h]

  materialHandle = rgp.world->dpvs.surfaces[surfIndex].material;
  return (materialHandle->info.gameFlags & 4) == 0 && (materialHandle->stateFlags & 4) == 0;
}

void __cdecl TerrainScorch(float *loresCellPos, bool instant)
{
  unsigned __int16 v2; // ax
  float v3; // [esp+4h] [ebp-10B4h]
  float v4; // [esp+8h] [ebp-10B0h]
  float v5; // [esp+18h] [ebp-10A0h]
  float v6; // [esp+1Ch] [ebp-109Ch]
  float v7; // [esp+2Ch] [ebp-108Ch]
  float v8; // [esp+30h] [ebp-1088h]
  unsigned __int16 *v9; // [esp+34h] [ebp-1084h]
  unsigned __int16 v10; // [esp+38h] [ebp-1080h]
  float v11; // [esp+3Ch] [ebp-107Ch]
  float v12; // [esp+40h] [ebp-1078h]
  int k; // [esp+44h] [ebp-1074h]
  int j; // [esp+48h] [ebp-1070h]
  unsigned int i; // [esp+4Ch] [ebp-106Ch]
  GfxSurface **surfLists; // [esp+5Ch] [ebp-105Ch] BYREF
  unsigned __int8 *v17; // [esp+60h] [ebp-1058h]
  float maxs[4]; // [esp+64h] [ebp-1054h] BYREF
  unsigned int surfCounts[2]; // [esp+74h] [ebp-1044h] BYREF
  unsigned __int16 *v20; // [esp+7Ch] [ebp-103Ch]
  unsigned int v21[1025]; // [esp+80h] [ebp-1038h] BYREF
  unsigned int stream2LockedSize; // [esp+1084h] [ebp-34h] BYREF
  float mins[3]; // [esp+1088h] [ebp-30h] BYREF
  float floatIn; // [esp+1094h] [ebp-24h]
  int (__cdecl *allowSurf[4])(int, void *); // [esp+1098h] [ebp-20h] BYREF
  GfxWorldVertex *v26; // [esp+10A8h] [ebp-10h]
  float v27; // [esp+10ACh] [ebp-Ch]
  unsigned int v28; // [esp+10B0h] [ebp-8h]
  unsigned __int8 *v29; // [esp+10B4h] [ebp-4h]
  int savedregs; // [esp+10B8h] [ebp+0h] BYREF

  allowSurf[0] = (int (__cdecl *)(int, void *))TerrainScorch_AllowSurfacesCallback;
  stream2LockedSize = 0;
  v17 = R_VertexStream2_Lock(&stream2LockedSize);
  if ( v17 )
  {
    maxs[3] = FLOAT_240_0;
    *(float *)&surfCounts[1] = FLOAT_96_0;
    allowSurf[1] = (int (__cdecl *)(int, void *))(LODWORD(FLOAT_240_0) ^ _mask__NegFloat_);
    allowSurf[2] = (int (__cdecl *)(int, void *))(LODWORD(FLOAT_240_0) ^ _mask__NegFloat_);
    allowSurf[3] = (int (__cdecl *)(int, void *))(LODWORD(FLOAT_96_0) ^ _mask__NegFloat_);
    v27 = 240.0 * 240.0;
    maxs[0] = *loresCellPos + 240.0;
    maxs[1] = loresCellPos[1] + 240.0;
    maxs[2] = loresCellPos[2] + 96.0;
    mins[0] = *loresCellPos + COERCE_FLOAT(LODWORD(FLOAT_240_0) ^ _mask__NegFloat_);
    mins[1] = loresCellPos[1] + COERCE_FLOAT(LODWORD(FLOAT_240_0) ^ _mask__NegFloat_);
    mins[2] = loresCellPos[2] + COERCE_FLOAT(LODWORD(FLOAT_96_0) ^ _mask__NegFloat_);
    floatIn = (float)G_GetTime() * 0.001;
    surfLists = (GfxSurface **)v21;
    R_BoxSurfaces((int)&savedregs, mins, maxs, allowSurf, 0, &surfLists, 0x400u, surfCounts, 1u);
    for ( i = 0; i < surfCounts[0]; ++i )
    {
      v28 = v21[i];
      if ( *(int *)(v28 + 44) >= 0 )
      {
        if ( *(unsigned int *)(v28 + 44) >= stream2LockedSize
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_fire.cpp",
                465,
                0,
                "%s",
                "(uint)(triSurf->stream2ByteOffset) < stream2ByteSize") )
        {
          __debugbreak();
        }
        v29 = &v17[*(unsigned int *)(v28 + 44)];
        v26 = &g_worldDraw->vd.vertices[*(unsigned int *)(v28 + 28)];
        v20 = &g_worldDraw->indices[*(unsigned int *)(v28 + 36)];
        for ( j = 0; j < *(unsigned __int16 *)(v28 + 34); ++j )
        {
          for ( k = 0; k < 3; ++k )
          {
            v10 = v20[k];
            if ( v10 >= (int)*(unsigned __int16 *)(v28 + 32)
              && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_fire.cpp",
                    481,
                    0,
                    "%s",
                    "vertIndex < triSurf->vertexCount") )
            {
              __debugbreak();
            }
            v11 = Vec3DistanceSq(v26[v10].xyz, loresCellPos);
            if ( v11 <= v27 )
            {
              v12 = v11 / v27;
              v9 = (unsigned __int16 *)&v29[4 * v10];
              if ( instant )
              {
                *v9 = HalfFromFloat(1.0);
                v9[1] = HalfFromFloat(1.0);
              }
              else
              {
                v8 = FloatFromHalf(v9[1]);
                v7 = 1.0 - v12;
                if ( (float)(1.0 - v12) > v8 )
                {
                  if ( v8 >= 0.0000099999997 )
                  {
                    v6 = FloatFromHalf(*v9);
                    v5 = (float)(floatIn - v6) / 3.0;
                    if ( (float)(v5 - 1.0) < 0.0 )
                      v4 = (float)(floatIn - v6) / 3.0;
                    else
                      v4 = FLOAT_1_0;
                    if ( (float)(0.0 - v5) < 0.0 )
                      v3 = v4;
                    else
                      v3 = *(float *)&FLOAT_0_0;
                    v2 = HalfFromFloat(floatIn - (float)((float)((float)(v3 * v8) / v7) * 3.0));
                  }
                  else
                  {
                    v2 = HalfFromFloat(floatIn);
                  }
                  *v9 = v2;
                  v9[1] = HalfFromFloat(v7);
                }
              }
            }
          }
          v20 += 3;
        }
      }
    }
    R_VertexStream2_Unlock();
  }
}

unsigned __int16 __cdecl HalfFromFloat(float floatIn)
{
  int m; // [esp+Ch] [ebp-14h]
  int e; // [esp+10h] [ebp-10h]
  int s; // [esp+14h] [ebp-Ch]

  s = HIWORD(LODWORD(floatIn)) & 0x8000;
  e = (unsigned __int8)(LODWORD(floatIn) >> 23) - 112;
  m = LODWORD(floatIn) & 0x7FFFFF;
  if ( e > 0 )
  {
    if ( (unsigned __int8)(LODWORD(floatIn) >> 23) == 255 )
    {
      if ( m )
      {
        if ( !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_fire.cpp",
                235,
                0,
                "%s\n\t(floatIn) = %g",
                "(a)",
                floatIn) )
          __debugbreak();
        return (m >> 13 == 0) | (m >> 13) | s | 0x7C00;
      }
      else
      {
        return s | 0x7C00;
      }
    }
    else if ( e <= 30 )
    {
      return (m >> 13) | s | ((_WORD)e << 10);
    }
    else
    {
      return s | 0x7C00;
    }
  }
  else if ( e >= -10 )
  {
    return s | ((int)((unsigned int)&loc_800000 | m) >> (1 - e) >> 13);
  }
  else
  {
    return 0;
  }
}

double __cdecl FloatFromHalf(unsigned __int16 halfIn)
{
  int m; // [esp+0h] [ebp-10h]
  int e; // [esp+4h] [ebp-Ch]
  FloatFromHalf::__l2::<unnamed_type_v> v; // [esp+Ch] [ebp-4h]

  e = ((int)halfIn >> 10) & 0x1F;
  m = halfIn & 0x3FF;
  if ( !e )
  {
    if ( (halfIn & 0x3FF) == 0 )
      return COERCE_FLOAT((int)halfIn >> 15 << 31);
    while ( (m & 0x400) == 0 )
    {
      m *= 2;
      --e;
    }
    ++e;
    m &= ~0x400u;
    return COERCE_FLOAT((m << 13) | ((e + 112) << 23) | ((int)halfIn >> 15 << 31));
  }
  if ( e != 31 )
    return COERCE_FLOAT((m << 13) | ((e + 112) << 23) | ((int)halfIn >> 15 << 31));
  if ( (halfIn & 0x3FF) != 0 )
    v.i = (m << 13) | ((int)halfIn >> 15 << 31) | 0x7F800000;
  else
    v.i = ((int)halfIn >> 15 << 31) | 0x7F800000;
  return v.f;
}

void __cdecl CG_UpdateFire(int localClientNum)
{
  int v1; // [esp+8h] [ebp-6Ch]
  int v2; // [esp+Ch] [ebp-68h]
  int v3; // [esp+10h] [ebp-64h]
  int v4; // [esp+14h] [ebp-60h]
  int v5; // [esp+18h] [ebp-5Ch]
  int v6; // [esp+1Ch] [ebp-58h]
  signed int cur_time; // [esp+54h] [ebp-20h]
  int i; // [esp+70h] [ebp-4h]
  int ia; // [esp+70h] [ebp-4h]

  cur_time = Sys_Milliseconds();
  if ( !localClientNum )
  {
    for ( i = 0; i < 256; ++i )
    {
      if ( g_FM_ActiveCells[i].is_active && cur_time > g_FM_ActiveCells[i].endtime )
        g_FM_ActiveCells[i].is_active = 0;
    }
    if ( g_FM_BurnDataLastScanIndex >= g_FM_BurnDataActiveCount )
      g_FM_BurnDataLastScanIndex = 0;
    for ( ia = 0; ia < g_FM_BurnDataActiveCount; ++ia )
    {
      if ( ia != g_FM_BurnDataLastScanIndex )
      {
        v6 = dword_E2FEF4[7 * ia] < dword_E2FEF4[7 * g_FM_BurnDataLastScanIndex]
           ? dword_E2FEF4[7 * g_FM_BurnDataLastScanIndex]
           : dword_E2FEF4[7 * ia];
        v5 = dword_E2FEF8[7 * ia] < dword_E2FEF8[7 * g_FM_BurnDataLastScanIndex]
           ? dword_E2FEF8[7 * g_FM_BurnDataLastScanIndex]
           : dword_E2FEF8[7 * ia];
        v4 = dword_E2FEFC[7 * ia] < dword_E2FEFC[7 * g_FM_BurnDataLastScanIndex]
           ? dword_E2FEFC[7 * g_FM_BurnDataLastScanIndex]
           : dword_E2FEFC[7 * ia];
        v3 = g_FM_BurnData[g_FM_BurnDataLastScanIndex].min[0] < g_FM_BurnData[ia].min[0]
           ? g_FM_BurnData[g_FM_BurnDataLastScanIndex].min[0]
           : g_FM_BurnData[ia].min[0];
        v2 = dword_E2FEEC[7 * g_FM_BurnDataLastScanIndex] < dword_E2FEEC[7 * ia]
           ? dword_E2FEEC[7 * g_FM_BurnDataLastScanIndex]
           : dword_E2FEEC[7 * ia];
        v1 = dword_E2FEF0[7 * g_FM_BurnDataLastScanIndex] < dword_E2FEF0[7 * ia]
           ? dword_E2FEF0[7 * g_FM_BurnDataLastScanIndex]
           : dword_E2FEF0[7 * ia];
        if ( (v4 - v1) * (v5 - v2) * (v6 - v3) == dword_E2FF00[7 * g_FM_BurnDataLastScanIndex] + dword_E2FF00[7 * ia] )
        {
          dword_E2FF00[7 * ia] += dword_E2FF00[7 * g_FM_BurnDataLastScanIndex];
          dword_E2FEF4[7 * ia] = v6;
          dword_E2FEF8[7 * ia] = v5;
          dword_E2FEFC[7 * ia] = v4;
          g_FM_BurnData[ia].min[0] = v3;
          dword_E2FEEC[7 * ia] = v2;
          dword_E2FEF0[7 * ia] = v1;
          if ( g_FM_BurnDataLastScanIndex != g_FM_BurnDataActiveCount - 1 )
            memcpy(
              &g_FM_BurnData[g_FM_BurnDataLastScanIndex],
              &g_FM_BurnData[g_FM_BurnDataActiveCount - 1],
              sizeof(_fm_burntree_data));
          --g_FM_BurnDataActiveCount;
          break;
        }
      }
    }
    if ( ia >= g_FM_BurnDataActiveCount )
      ++g_FM_BurnDataLastScanIndex;
  }
}

void __cdecl CG_RenderFire()
{
  float origin[3]; // [esp+10h] [ebp-38h] BYREF
  float mins[3]; // [esp+1Ch] [ebp-2Ch] BYREF
  float maxs[3]; // [esp+28h] [ebp-20h] BYREF
  float color[4]; // [esp+34h] [ebp-14h] BYREF
  int i; // [esp+44h] [ebp-4h]

  if ( fire_debug && fire_debug->current.enabled )
  {
    color[0] = FLOAT_1_0;
    color[1] = FLOAT_1_0;
    color[2] = FLOAT_1_0;
    color[3] = FLOAT_1_0;
    memset(mins, 0, sizeof(mins));
    for ( i = 0; i < g_FM_BurnDataActiveCount; ++i )
    {
      maxs[0] = (float)(dword_E2FEF4[7 * i] - g_FM_BurnData[i].min[0]);
      maxs[1] = (float)(dword_E2FEF8[7 * i] - dword_E2FEEC[7 * i]);
      maxs[2] = (float)(dword_E2FEFC[7 * i] - dword_E2FEF0[7 * i]);
      origin[0] = (float)g_FM_BurnData[i].min[0];
      origin[1] = (float)dword_E2FEEC[7 * i];
      origin[2] = (float)dword_E2FEF0[7 * i];
      CG_DebugBox(origin, mins, maxs, 0.0, color, 1, 0);
    }
    for ( i = 0; i < 256; ++i )
    {
      if ( g_FM_ActiveCells[i].is_active )
        G_DebugStar(g_FM_ActiveCells[i].pos, colorRed, 0);
    }
  }
}

double __cdecl GetFoliageBurnRadius(TerrainFireSource fireSrc)
{
  if ( fireSrc == TFS_FLAMETHROWER )
    return 60.0;
  else
    return 24.0;
}

void __cdecl CG_SetFireToWall(int localClientNum, float *pos, int *models, int models_count, TerrainFireSource fireSrc)
{
  signed int v5; // eax
  cg_s *LocalClientGlobals; // eax
  float rad; // [esp+0h] [ebp-3Ch]
  int fxIndex; // [esp+10h] [ebp-2Ch]
  int curTime; // [esp+14h] [ebp-28h]
  float axis[3][3]; // [esp+18h] [ebp-24h] BYREF

  axis[0][0] = *(float *)&FLOAT_0_0;
  *(_QWORD *)&axis[0][1] = __PAIR64__(LODWORD(FLOAT_1_0), *(unsigned int *)&FLOAT_0_0);
  *(_QWORD *)&axis[1][0] = __PAIR64__(LODWORD(FLOAT_N1_0), *(unsigned int *)&FLOAT_0_0);
  axis[1][2] = *(float *)&FLOAT_0_0;
  *(_QWORD *)&axis[2][0] = __PAIR64__(*(unsigned int *)&FLOAT_0_0, LODWORD(FLOAT_1_0));
  axis[2][2] = *(float *)&FLOAT_0_0;
  v5 = rand() & 0x80000000;
  if ( v5 < 0 )
    v5 = 0;
  fxIndex = v5;
  curTime = CG_GetLocalClientGlobals(localClientNum)->time;
  if ( curTime - gLastBurnParticleSpawnTime > 500 )
  {
    gLastBurnParticleSpawnTime = curTime;
    LocalClientGlobals = CG_GetLocalClientGlobals(localClientNum);
    FX_PlayOrientedEffect(localClientNum, g_FM_fx_wall_effect[fxIndex], LocalClientGlobals->time, pos, axis);
  }
  rad = GetFoliageBurnRadius(fireSrc);
  R_FoliageNotifyBurn(pos, rad, models, models_count, 0);
}

void __cdecl CG_SetFireToTerrain(
        float *pos,
        float radius,
        int fireStarterClientNum,
        int weaponUsed,
        float pSpread,
        int *models,
        int models_count,
        TerrainFireSource fireSrc)
{
  cg_s *LocalClientGlobals; // eax
  float rad; // [esp+0h] [ebp-BCh]
  float v10; // [esp+10h] [ebp-ACh]
  float v11; // [esp+14h] [ebp-A8h]
  float v12; // [esp+18h] [ebp-A4h]
  float v13; // [esp+1Ch] [ebp-A0h]
  float v14; // [esp+20h] [ebp-9Ch]
  float v15; // [esp+24h] [ebp-98h]
  FM_ACTIVECELL_DATA *v16; // [esp+28h] [ebp-94h]
  FM_ACTIVECELL_DATA *v17; // [esp+2Ch] [ebp-90h]
  int localClientNum; // [esp+58h] [ebp-64h]
  float cell_pos[3]; // [esp+5Ch] [ebp-60h] BYREF
  float inv_dim[3]; // [esp+68h] [ebp-54h]
  int cur_time; // [esp+74h] [ebp-48h]
  int fxIndex; // [esp+78h] [ebp-44h]
  float dim[3]; // [esp+7Ch] [ebp-40h]
  float cell_pos2[3]; // [esp+88h] [ebp-34h] BYREF
  int i; // [esp+94h] [ebp-28h]
  float axis[3][3]; // [esp+98h] [ebp-24h] BYREF

  dim[0] = FLOAT_32_0;
  dim[1] = FLOAT_32_0;
  dim[2] = FLOAT_32_0;
  inv_dim[0] = FLOAT_0_03125;
  inv_dim[1] = FLOAT_0_03125;
  inv_dim[2] = FLOAT_0_03125;
  cur_time = Sys_Milliseconds();
  cell_pos[0] = *pos * 0.03125;
  cell_pos[1] = pos[1] * 0.03125;
  cell_pos[2] = pos[2] * 0.03125;
  v15 = floor(cell_pos[0]);
  cell_pos[0] = v15;
  v14 = floor(cell_pos[1]);
  cell_pos[1] = v14;
  v13 = floor(cell_pos[2]);
  cell_pos[2] = v13;
  for ( i = 0; i < 256; ++i )
  {
    if ( g_FM_ActiveCells[i].is_active )
    {
      cell_pos2[0] = g_FM_ActiveCells[i].pos[0] * inv_dim[0];
      cell_pos2[1] = g_FM_ActiveCells[i].pos[1] * inv_dim[1];
      cell_pos2[2] = g_FM_ActiveCells[i].pos[2] * inv_dim[2];
      v12 = floor(cell_pos2[0]);
      cell_pos2[0] = v12;
      v11 = floor(cell_pos2[1]);
      cell_pos2[1] = v11;
      v10 = floor(cell_pos2[2]);
      cell_pos2[2] = v10;
      if ( Vec3DistanceSq(cell_pos, cell_pos2) < 1.0 )
      {
        v17 = &g_FM_ActiveCells[i];
        v17->pos[0] = *pos;
        v17->pos[1] = pos[1];
        v17->pos[2] = pos[2];
        g_FM_ActiveCells[i].endtime = fire_burn_time->current.integer + cur_time;
        return;
      }
    }
  }
  cell_pos[0] = cell_pos[0] * dim[0];
  cell_pos[1] = cell_pos[1] * dim[1];
  cell_pos[2] = cell_pos[2] * dim[2];
  for ( i = 0; i < 256; ++i )
  {
    if ( !g_FM_ActiveCells[i].is_active )
    {
      v16 = &g_FM_ActiveCells[i];
      v16->pos[0] = *pos;
      v16->pos[1] = pos[1];
      v16->pos[2] = pos[2];
      g_FM_ActiveCells[i].is_active = 1;
      g_FM_ActiveCells[i].endtime = fire_burn_time->current.integer + cur_time;
      break;
    }
  }
  AddBurnCell(cell_pos);
  rad = GetFoliageBurnRadius(fireSrc);
  R_FoliageNotifyBurn(pos, rad, models, models_count, 0);
  axis[0][0] = *(float *)&FLOAT_0_0;
  *(_QWORD *)&axis[0][1] = __PAIR64__(LODWORD(FLOAT_1_0), *(unsigned int *)&FLOAT_0_0);
  *(_QWORD *)&axis[1][0] = __PAIR64__(LODWORD(FLOAT_N1_0), *(unsigned int *)&FLOAT_0_0);
  axis[1][2] = *(float *)&FLOAT_0_0;
  *(_QWORD *)&axis[2][0] = __PAIR64__(*(unsigned int *)&FLOAT_0_0, LODWORD(FLOAT_1_0));
  axis[2][2] = *(float *)&FLOAT_0_0;
  fxIndex = rand() % 3;
  if ( !g_FM_fx_flamethrower_effect[fxIndex]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_fire.cpp",
          768,
          0,
          "%s",
          "g_FM_fx_flamethrower_effect[fxIndex] != NULL") )
  {
    __debugbreak();
  }
  for ( localClientNum = 0; localClientNum < 1; ++localClientNum )
  {
    if ( CL_LocalClient_IsActive(localClientNum) && fireSrc == TFS_FLAMETHROWER )
    {
      LocalClientGlobals = CG_GetLocalClientGlobals(localClientNum);
      FX_PlayOrientedEffect(localClientNum, g_FM_fx_flamethrower_effect[fxIndex], LocalClientGlobals->time, pos, axis);
    }
  }
  TerrainScorch(pos, 0);
}

void __cdecl AddBurnCell(float *pos)
{
  int v1; // [esp+0h] [ebp-68h]
  int v2; // [esp+4h] [ebp-64h]
  int v3; // [esp+8h] [ebp-60h]
  int v4; // [esp+Ch] [ebp-5Ch]
  int v5; // [esp+10h] [ebp-58h]
  int v6; // [esp+14h] [ebp-54h]
  int ipos; // [esp+4Ch] [ebp-1Ch]
  int ipos_4; // [esp+50h] [ebp-18h]
  int ipos_8; // [esp+54h] [ebp-14h]
  int i; // [esp+64h] [ebp-4h]
  int ia; // [esp+64h] [ebp-4h]

  ipos = (int)*pos;
  ipos_4 = (int)pos[1];
  ipos_8 = (int)pos[2];
  if ( g_FM_BurnDataActiveCount != 4096 )
  {
    for ( i = 0; i < g_FM_BurnDataActiveCount; ++i )
    {
      if ( ipos >= g_FM_BurnData[i].min[0]
        && ipos < dword_E2FEF4[7 * i]
        && ipos_4 >= dword_E2FEEC[7 * i]
        && ipos_4 < dword_E2FEF8[7 * i]
        && ipos_8 >= dword_E2FEF0[7 * i]
        && ipos_8 < dword_E2FEFC[7 * i] )
      {
        return;
      }
    }
    for ( ia = 0; ia < g_FM_BurnDataActiveCount; ++ia )
    {
      if ( dword_E2FEF4[7 * ia] < ipos + 32 )
        v6 = ipos + 32;
      else
        v6 = dword_E2FEF4[7 * ia];
      if ( dword_E2FEF8[7 * ia] < ipos_4 + 32 )
        v5 = ipos_4 + 32;
      else
        v5 = dword_E2FEF8[7 * ia];
      if ( dword_E2FEFC[7 * ia] < ipos_8 + 32 )
        v4 = ipos_8 + 32;
      else
        v4 = dword_E2FEFC[7 * ia];
      if ( ipos < g_FM_BurnData[ia].min[0] )
        v3 = (int)*pos;
      else
        v3 = g_FM_BurnData[ia].min[0];
      if ( ipos_4 < dword_E2FEEC[7 * ia] )
        v2 = (int)pos[1];
      else
        v2 = dword_E2FEEC[7 * ia];
      if ( ipos_8 < dword_E2FEF0[7 * ia] )
        v1 = (int)pos[2];
      else
        v1 = dword_E2FEF0[7 * ia];
      if ( (v4 - v1) * (v5 - v2) * (v6 - v3) == dword_E2FF00[7 * ia] + 0x8000 )
      {
        dword_E2FF00[7 * ia] += 0x8000;
        dword_E2FEF4[7 * ia] = v6;
        dword_E2FEF8[7 * ia] = v5;
        dword_E2FEFC[7 * ia] = v4;
        g_FM_BurnData[ia].min[0] = v3;
        dword_E2FEEC[7 * ia] = v2;
        dword_E2FEF0[7 * ia] = v1;
        return;
      }
    }
    g_FM_BurnData[g_FM_BurnDataActiveCount].min[0] = ipos;
    dword_E2FEEC[7 * g_FM_BurnDataActiveCount] = ipos_4;
    dword_E2FEF0[7 * g_FM_BurnDataActiveCount] = ipos_8;
    dword_E2FEF4[7 * g_FM_BurnDataActiveCount] = ipos + 32;
    dword_E2FEF8[7 * g_FM_BurnDataActiveCount] = ipos_4 + 32;
    dword_E2FEFC[7 * g_FM_BurnDataActiveCount] = ipos_8 + 32;
    dword_E2FF00[7 * g_FM_BurnDataActiveCount++] = 0x8000;
  }
}

void __cdecl SetTerrainScorchInitialState()
{
  R_VertexStream2_Clear();
}

void __cdecl Sync_VisualBurn_To_FM_State()
{
  SetTerrainScorchInitialState();
  R_FoliageSetInitialBurnState();
}

void __cdecl MemFile_WriteInt(MemoryFile *memFile, int value)
{
  if ( (unsigned int)(memFile->cacheBufferUsed + 4) >= 0x7FF8 )
    MemFile_WriteDataFlushInternal(memFile);
  *(unsigned int *)&memFile->cacheBuffer[memFile->cacheBufferUsed] = value;
  memFile->cacheBufferUsed += 4;
}

void __cdecl CG_GenerateFireSounds(int localClientNum)
{
  float *p_snapFlags; // ecx
  float level; // [esp+4h] [ebp-94h]
  float v3; // [esp+8h] [ebp-90h]
  float v4; // [esp+Ch] [ebp-8Ch]
  float v5; // [esp+10h] [ebp-88h]
  float v6; // [esp+24h] [ebp-74h]
  unsigned int j; // [esp+3Ch] [ebp-5Ch]
  unsigned int ja; // [esp+3Ch] [ebp-5Ch]
  cg_s *cgameGlob; // [esp+40h] [ebp-58h]
  float toCell[3]; // [esp+44h] [ebp-54h] BYREF
  float fireDensity[8]; // [esp+50h] [ebp-48h]
  float angle; // [esp+70h] [ebp-28h]
  float distanceAttenuation; // [esp+74h] [ebp-24h]
  float distance; // [esp+78h] [ebp-20h]
  unsigned int i; // [esp+7Ch] [ebp-1Ch]
  float playerPosition[3]; // [esp+80h] [ebp-18h]
  float soundPosition[3]; // [esp+8Ch] [ebp-Ch] BYREF

  if ( (_S1 & 1) == 0 )
  {
    _S1 |= 1u;
    SQRT2 = 1.0 / fsqrt(2.0);
  }
  if ( (_S1 & 2) == 0 )
  {
    _S1 |= 2u;
    SOUND_DIRECTIONS[0][0] = SQRT2;
    dword_E4DB34 = LODWORD(SQRT2);
    dword_E4DB38 = *(unsigned int *)&FLOAT_0_0;
    dword_E4DB3C = LODWORD(FLOAT_1_0);
    dword_E4DB40 = *(unsigned int *)&FLOAT_0_0;
    dword_E4DB44 = *(unsigned int *)&FLOAT_0_0;
    dword_E4DB48 = LODWORD(SQRT2);
    dword_E4DB4C = LODWORD(SQRT2) ^ _mask__NegFloat_;
    dword_E4DB50 = *(unsigned int *)&FLOAT_0_0;
    dword_E4DB54 = *(unsigned int *)&FLOAT_0_0;
    dword_E4DB58 = LODWORD(FLOAT_N1_0);
    dword_E4DB5C = *(unsigned int *)&FLOAT_0_0;
    dword_E4DB60 = LODWORD(SQRT2) ^ _mask__NegFloat_;
    dword_E4DB64 = LODWORD(SQRT2) ^ _mask__NegFloat_;
    dword_E4DB68 = *(unsigned int *)&FLOAT_0_0;
    dword_E4DB6C = LODWORD(FLOAT_N1_0);
    dword_E4DB70 = *(unsigned int *)&FLOAT_0_0;
    dword_E4DB74 = *(unsigned int *)&FLOAT_0_0;
    dword_E4DB78 = LODWORD(SQRT2) ^ _mask__NegFloat_;
    dword_E4DB7C = LODWORD(SQRT2);
    dword_E4DB80 = *(unsigned int *)&FLOAT_0_0;
    dword_E4DB84 = *(unsigned int *)&FLOAT_0_0;
    dword_E4DB88 = LODWORD(FLOAT_1_0);
    dword_E4DB8C = *(unsigned int *)&FLOAT_0_0;
  }
  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  if ( cgameGlob->nextSnap )
  {
    p_snapFlags = (float *)&cgameGlob->nextSnap->snapFlags;
    playerPosition[0] = p_snapFlags[13];
    playerPosition[1] = p_snapFlags[14];
    playerPosition[2] = p_snapFlags[15];
    for ( j = 0; j < 8; ++j )
      fireDensity[j] = *(float *)&FLOAT_0_0;
    for ( i = 0; i < 0x100; ++i )
    {
      if ( g_FM_ActiveCells[i].is_active )
      {
        toCell[0] = g_FM_ActiveCells[i].pos[0] - playerPosition[0];
        toCell[1] = g_FM_ActiveCells[i].pos[1] - playerPosition[1];
        toCell[2] = g_FM_ActiveCells[i].pos[2] - playerPosition[2];
        distance = Vec3Normalize(toCell);
        if ( distance <= 500.0 )
        {
          if ( (float)((float)(distance / 500.0) - 1.0) < 0.0 )
            v6 = distance / 500.0;
          else
            v6 = FLOAT_1_0;
          if ( (float)(0.0 - (float)(distance / 500.0)) < 0.0 )
            v3 = v6;
          else
            v3 = *(float *)&FLOAT_0_0;
          distanceAttenuation = (float)(1.0 - v3) * (float)(1.0 - v3);
          for ( ja = 0; ja < 8; ++ja )
          {
            angle = (float)((float)(toCell[0] * (float)SOUND_DIRECTIONS[ja][0])
                          + (float)(toCell[1] * (float)SOUND_DIRECTIONS[ja][1]))
                  + (float)(toCell[2] * (float)SOUND_DIRECTIONS[ja][2]);
            if ( angle >= 0.0 )
              fireDensity[ja] = (float)(angle * distanceAttenuation) + fireDensity[ja];
          }
        }
      }
    }
    for ( i = 0; i < 8; ++i )
    {
      soundPosition[0] = (float)(128.0 * (float)SOUND_DIRECTIONS[i][0]) + playerPosition[0];
      soundPosition[1] = (float)(128.0 * (float)SOUND_DIRECTIONS[i][1]) + playerPosition[1];
      soundPosition[2] = (float)(128.0 * (float)SOUND_DIRECTIONS[i][2]) + playerPosition[2];
      soundPosition[2] = soundPosition[2] + 16.0;
      v4 = fireDensity[i] / 4.0;
      if ( (float)(v4 - 1.0) < 0.0 )
        v5 = fireDensity[i] / 4.0;
      else
        v5 = FLOAT_1_0;
      if ( (float)(0.0 - v4) < 0.0 )
        level = v5;
      else
        level = *(float *)&FLOAT_0_0;
      CG_FireManagerUpdate(localClientNum, i, soundPosition, level);
    }
  }
}

