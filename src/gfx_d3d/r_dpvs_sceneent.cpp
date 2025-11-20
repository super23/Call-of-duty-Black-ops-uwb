#include "r_dpvs_sceneent.h"

void  R_AddCellSceneEntSurfacesInFrustumCmd(unsigned int a1@<ebp>, GfxWorldDpvsPlanes *data)
{
  bool v2; // zf
  int v3; // eax
  int v4; // eax
  int v5; // [esp+18h] [ebp-C0h]
  const DpvsPlane *v6; // [esp+20h] [ebp-B8h]
  signed int v7; // [esp+24h] [ebp-B4h]
  const float *v8; // [esp+28h] [ebp-B0h]
  int v9; // [esp+30h] [ebp-A8h]
  const DpvsPlane *bmodel; // [esp+38h] [ebp-A0h]
  int v11; // [esp+3Ch] [ebp-9Ch]
  GfxSceneEntity *v12; // [esp+40h] [ebp-98h]
  GfxSceneEntity *sceneEnt; // [esp+4Ch] [ebp-8Ch]
  GfxSceneEntity *sceneEnta; // [esp+4Ch] [ebp-8Ch]
  GfxEntCellRefInfo *info; // [esp+54h] [ebp-84h]
  GfxEntCellRefInfo *infoa; // [esp+54h] [ebp-84h]
  unsigned int sceneEntIndex; // [esp+58h] [ebp-80h]
  unsigned int sceneEntIndexa; // [esp+58h] [ebp-80h]
  unsigned int bit; // [esp+5Ch] [ebp-7Ch]
  unsigned int bita; // [esp+5Ch] [ebp-7Ch]
  unsigned int entnum; // [esp+60h] [ebp-78h]
  unsigned int entnuma; // [esp+60h] [ebp-78h]
  signed int indexLow; // [esp+64h] [ebp-74h]
  const DpvsPlane *bits; // [esp+68h] [ebp-70h]
  unsigned int wordIndex; // [esp+6Ch] [ebp-6Ch]
  unsigned int wordIndexa; // [esp+6Ch] [ebp-6Ch]
  int innerPlaneCount; // [esp+70h] [ebp-68h]
  unsigned int *entCellBits; // [esp+78h] [ebp-60h]
  unsigned int *entCellBitsa; // [esp+78h] [ebp-60h]
  const DpvsPlane *planes; // [esp+7Ch] [ebp-5Ch] BYREF
  unsigned int *entCellBitsEA; // [esp+80h] [ebp-58h]
  unsigned int offset; // [esp+84h] [ebp-54h]
  DpvsEntityCmd dpvsEntity; // [esp+88h] [ebp-50h]
  int nodes_high; // [esp+98h] [ebp-40h]
  int v35; // [esp+9Ch] [ebp-3Ch]
  unsigned int *cellCount; // [esp+A0h] [ebp-38h]
  int frustumPlaneCount; // [esp+A4h] [ebp-34h]
  int planeCount; // [esp+A8h] [ebp-30h]
  const DpvsPlane *planesEA; // [esp+ACh] [ebp-2Ch]
  unsigned int cellIndex; // [esp+B0h] [ebp-28h]
  unsigned int viewIndex; // [esp+B4h] [ebp-24h]
  unsigned __int16 *sceneDObjIndex; // [esp+B8h] [ebp-20h]
  unsigned __int16 *sceneXModelIndex; // [esp+BCh] [ebp-1Ch]
  GfxEntCellRefInfo *entInfo; // [esp+C0h] [ebp-18h]
  unsigned int wordCount; // [esp+C4h] [ebp-14h]
  GfxWorldDpvsPlanes *worldDpvsPlanes; // [esp+C8h] [ebp-10h]
  unsigned int localClientNum; // [esp+CCh] [ebp-Ch]
  GfxSceneDpvs *sceneDpvs; // [esp+D0h] [ebp-8h]
  GfxSceneDpvs *retaddr; // [esp+D8h] [ebp+0h]

  localClientNum = a1;
  sceneDpvs = retaddr;
  worldDpvsPlanes = data;
  wordCount = (unsigned int)&scene.dpvs;
  entInfo = (GfxEntCellRefInfo *)scene.dpvs.localClientNum;
  sceneXModelIndex = (unsigned __int16 *)&rgp.world->dpvsPlanes;
  sceneDObjIndex = (unsigned __int16 *)(gfxCfg.entCount >> 5);
  if ( scene.dpvs.localClientNum >= gfxCfg.maxClientViews
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dpvs_sceneent.cpp",
          213,
          0,
          "localClientNum doesn't index gfxCfg.maxClientViews\n\t%i not in [0, %i)",
          entInfo,
          gfxCfg.maxClientViews) )
  {
    __debugbreak();
  }
  viewIndex = scene.dynSModelVisBitsCamera[(unsigned int)entInfo - 4];
  cellIndex = (unsigned int)scene.dpvs.sceneXModelIndex;
  planesEA = (const DpvsPlane *)scene.dpvs.sceneDObjIndex;
  planeCount = LOWORD(worldDpvsPlanes->nodes);
  frustumPlaneCount = (int)worldDpvsPlanes->planes;
  cellCount = (unsigned int *)worldDpvsPlanes->cellCount;
  v35 = BYTE2(worldDpvsPlanes->nodes);
  nodes_high = HIBYTE(worldDpvsPlanes->nodes);
  if ( (unsigned int)frustumPlaneCount >= *(unsigned int *)sceneXModelIndex
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dpvs_sceneent.cpp",
          225,
          0,
          "cellIndex doesn't index worldDpvsPlanes->cellCount\n\t%i not in [0, %i)",
          frustumPlaneCount,
          *(unsigned int *)sceneXModelIndex) )
  {
    __debugbreak();
  }
  dpvsEntity.sceneEnt = *(GfxSceneEntity **)(wordCount + 4 * planeCount + 4);
  entCellBitsEA = cellCount;
  LOWORD(offset) = v35;
  HIWORD(offset) = frustumPlaneCount;
  entCellBits = (unsigned int *)((unsigned int)sceneDObjIndex * (unsigned int)entInfo);
  if ( (unsigned int)((unsigned int)sceneDObjIndex * (unsigned int)entInfo) >= 0x100
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dpvs_sceneent.cpp",
          234,
          0,
          "offset doesn't index MAX_TOTAL_ENT_COUNT >> 5\n\t%i not in [0, %i)",
          entCellBits,
          256) )
  {
    __debugbreak();
  }
  entCellBitsa = &entCellBits[64 * frustumPlaneCount];
  innerPlaneCount = (int)cellCount;
  wordIndex = *((unsigned int *)sceneXModelIndex + 3) + 4 * (unsigned int)entCellBitsa;
  if ( nodes_high > v35
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dpvs_sceneent.cpp",
          261,
          0,
          "%s",
          "frustumPlaneCount <= planeCount") )
  {
    __debugbreak();
  }
  bits = (const DpvsPlane *)&cellCount[5 * nodes_high];
  indexLow = v35 - nodes_high;
  for ( entnum = 0; entnum < (unsigned int)sceneDObjIndex; ++entnum )
  {
    bit = *(unsigned int *)(wordIndex + 4 * entnum);
    while ( 1 )
    {
      v2 = !_BitScanReverse((unsigned int *)&v3, bit);
      if ( v2 )
        v3 = `CountLeadingZeros'::`2'::notFound;
      sceneEntIndex = v3 ^ 0x1F;
      if ( (v3 ^ 0x1Fu) >= 0x20 )
        break;
      info = (GfxEntCellRefInfo *)(sceneEntIndex + 32 * entnum);
      if ( ((0x80000000 >> sceneEntIndex) & bit) == 0
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dpvs_sceneent.cpp",
              278,
              0,
              "%s",
              "bits & bit") )
      {
        __debugbreak();
      }
      bit &= ~(0x80000000 >> sceneEntIndex);
      if ( !*((_BYTE *)&info->radius + (unsigned int)dpvsEntity.sceneEnt) )
      {
        sceneEnt = (GfxSceneEntity *)*((unsigned __int16 *)planesEA->coeffs + (unsigned int)info);
        if ( sceneEnt == (GfxSceneEntity *)0xFFFF )
        {
          sceneEnta = (GfxSceneEntity *)*(unsigned __int16 *)(cellIndex + 2 * (unsigned int)info);
          if ( sceneEnta != (GfxSceneEntity *)0xFFFF
            && !R_CullSphereDpvs(
                  scene.sceneModel[(unsigned int)sceneEnta].placement.base.origin,
                  *(float *)(viewIndex + 4 * (unsigned int)info),
                  bits,
                  indexLow) )
          {
            *((_BYTE *)&info->radius + (unsigned int)dpvsEntity.sceneEnt) = 1;
          }
        }
        else
        {
          v12 = &scene.sceneDObj[(unsigned int)sceneEnt];
          if ( !R_CullSphereDpvs(v12->placement.base.origin, *(float *)(viewIndex + 4 * (unsigned int)info), bits, indexLow) )
          {
            if ( v12->cull.state < 2 )
              goto LABEL_36;
            v11 = 0;
            bmodel = (const DpvsPlane *)innerPlaneCount;
            while ( v11 < v35 )
            {
              if ( R_DpvsPlaneMaxSignedDistToBox(bmodel, v12->cull.mins) <= 0.0 )
              {
                v9 = 1;
                goto LABEL_35;
              }
              ++v11;
              ++bmodel;
            }
            v9 = 0;
LABEL_35:
            if ( !v9 )
            {
LABEL_36:
              planes = (const DpvsPlane *)&scene.sceneDObj[(unsigned int)sceneEnt];
              if ( v12->cull.state < 2 )
                Sys_AddWorkerCmdInternal(&r_dpvs_entityWorkerCmd, &planes, 0);
              else
                R_AddEntitySurfacesInFrustumCmd(&planes);
            }
          }
        }
      }
    }
  }
  wordIndexa = *((unsigned int *)sceneXModelIndex + 3) + 4 * (unsigned int)entCellBitsa + (*(unsigned int *)sceneXModelIndex << 10);
  for ( entnuma = 0; entnuma < (unsigned int)sceneDObjIndex; ++entnuma )
  {
    bita = *(unsigned int *)(wordIndexa + 4 * entnuma);
    while ( 1 )
    {
      v2 = !_BitScanReverse((unsigned int *)&v4, bita);
      if ( v2 )
        v4 = `CountLeadingZeros'::`2'::notFound;
      sceneEntIndexa = v4 ^ 0x1F;
      if ( (v4 ^ 0x1Fu) >= 0x20 )
        break;
      infoa = (GfxEntCellRefInfo *)(sceneEntIndexa + 32 * entnuma);
      if ( ((0x80000000 >> sceneEntIndexa) & bita) == 0
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dpvs_sceneent.cpp",
              382,
              0,
              "%s",
              "bits & bit") )
      {
        __debugbreak();
      }
      bita &= ~(0x80000000 >> sceneEntIndexa);
      if ( !*((_BYTE *)&infoa->radius + (unsigned int)dpvsEntity.sceneEnt) )
      {
        v8 = *(const float **)(viewIndex + 4 * (unsigned int)infoa);
        v7 = 0;
        v6 = bits;
        while ( v7 < indexLow )
        {
          if ( R_DpvsPlaneMaxSignedDistToBox(v6, v8) <= 0.0 )
          {
            v5 = 1;
            goto LABEL_56;
          }
          ++v7;
          ++v6;
        }
        v5 = 0;
LABEL_56:
        if ( !v5 )
          *((_BYTE *)&infoa->radius + (unsigned int)dpvsEntity.sceneEnt) = 1;
      }
    }
  }
}

