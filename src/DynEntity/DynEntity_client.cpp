#include "DynEntity_client.h"

void __cdecl DynEntCl_RegisterDvars()
{
  dynEnt_bulletForce = _Dvar_RegisterFloat(
                         "dynEnt_bulletForce",
                         800.0,
                         0.0,
                         1000000.0,
                         0x1080u,
                         "Force applied from bullet hit");
  dynEnt_explodeForce = _Dvar_RegisterFloat(
                          "dynEnt_explodeForce",
                          12500.0,
                          0.0,
                          1000000.0,
                          0x80u,
                          "Force applied from explosion hit");
  dynEnt_explodeUpbias = _Dvar_RegisterFloat(
                           "dynEnt_explodeUpbias",
                           0.5,
                           0.0,
                           2.0,
                           0x80u,
                           "Upward bias applied to force directions from explosion hits");
  dynEnt_explodeSpinScale = _Dvar_RegisterFloat(
                              "dynEnt_explodeSpinScale",
                              4.0,
                              0.0,
                              100.0,
                              0x80u,
                              "Scale of the random offset from the center of mass for explosion forces.");
  dynEnt_explodeMinForce = _Dvar_RegisterFloat(
                             "dynEnt_explodeMinForce",
                             40.0,
                             0.0,
                             3.4028235e38,
                             0x80u,
                             "Force below which dynents won't even bother waking up");
  dynEnt_explodeMaxEnts = _Dvar_RegisterInt(
                            "dynEnt_explodeMaxEnts",
                            20,
                            0,
                            4096,
                            0x80u,
                            "The maximum number of dynents that can be awakened by one explosion");
  dynEnt_spawnedLimit = _Dvar_RegisterInt(
                          "dynEnt_spawnedLimit",
                          50,
                          0,
                          256,
                          0x80u,
                          "The max number of extra dynents (not placed on Radiant, spawned from script, or destructibles)");
  dynEnt_sentientAutoActivate = _Dvar_RegisterBool(
                                  "dynEnt_sentientAutoActivate",
                                  1,
                                  0,
                                  "This allows the player and AI to push around dyn ents");
  DynEntPieces_RegisterDvars();
}

XModel *__cdecl DynEntCl_GetCurrentXModel(const DynEntityDef *dynEntDef, const DynEntityClient *dynEntClient)
{
  if ( dynEntDef->destroyedxModel && DynEntCl_IsHealthGone(dynEntDef, dynEntClient) )
    return dynEntDef->destroyedxModel;
  else
    return dynEntDef->xModel;
}

bool __cdecl DynEntCl_IsHealthGone(const DynEntityDef *dynEntDef, const DynEntityClient *dynEntClient)
{
  return dynEntDef->health > 0 && dynEntClient->health <= 0;
}

void __cdecl DynEntCl_LinkModel(unsigned __int16 dynEntId)
{
  int v1; // [esp+4h] [ebp-204h]
  int v2; // [esp+8h] [ebp-200h]
  int v3; // [esp+Ch] [ebp-1FCh]
  int v4; // [esp+14h] [ebp-1F4h]
  int v5; // [esp+18h] [ebp-1F0h]
  int v6; // [esp+1Ch] [ebp-1ECh]
  int v7; // [esp+24h] [ebp-1E4h]
  int v8; // [esp+28h] [ebp-1E0h]
  int v9; // [esp+2Ch] [ebp-1DCh]
  float v10; // [esp+134h] [ebp-D4h]
  float v11; // [esp+138h] [ebp-D0h]
  float v12; // [esp+13Ch] [ebp-CCh]
  DynEntityPose *dynEntPose; // [esp+160h] [ebp-A8h]
  float modelBoundsVec3[2][3]; // [esp+164h] [ebp-A4h] BYREF
  float4 worldBoundsFloat4[2]; // [esp+17Ch] [ebp-8Ch]
  XModel *model; // [esp+19Ch] [ebp-6Ch]
  DynEntityClient *dynEntClient; // [esp+1A0h] [ebp-68h]
  const DynEntityDef *dynEntDef; // [esp+1A4h] [ebp-64h]
  float modelAxis[3][3]; // [esp+1A8h] [ebp-60h] BYREF
  float worldBoundsVec3[2][3]; // [esp+1CCh] [ebp-3Ch] BYREF
  float4 modelBoundsFloat4[2]; // [esp+1E4h] [ebp-24h]

  dynEntDef = DynEnt_GetEntityDef(dynEntId, DYNENT_DRAW_MODEL);
  dynEntPose = DynEnt_GetClientPose(dynEntId, DYNENT_DRAW_MODEL);
  dynEntClient = DynEnt_GetClientEntity(dynEntId, DYNENT_DRAW_MODEL);
  model = DynEntCl_GetCurrentXModel(dynEntDef, dynEntClient);
  if ( !model
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp", 148, 0, "%s", "model") )
  {
    __debugbreak();
  }
  dynEntPose->radius = XModelGetRadius(model);
  XModelGetBounds(model, modelBoundsVec3[0], modelBoundsVec3[1]);
  *(_QWORD *)modelBoundsFloat4[0].v = *(_QWORD *)&modelBoundsVec3[0][0];
  *(_QWORD *)&modelBoundsFloat4[0].unitVec[2].packed = __PAIR64__(
                                                         *(unsigned int *)&FLOAT_0_0,
                                                         LODWORD(modelBoundsVec3[0][2]));
  modelBoundsFloat4[1].v[0] = modelBoundsVec3[1][0];
  modelBoundsFloat4[1].v[1] = modelBoundsVec3[1][1];
  modelBoundsFloat4[1].v[2] = modelBoundsVec3[1][2];
  modelBoundsFloat4[1].u[3] = *(unsigned int *)&FLOAT_0_0;
  UnitQuatToAxis(dynEntPose->pose.quat, modelAxis);
  v10 = dynEntPose->pose.origin[0];
  v11 = dynEntPose->pose.origin[1];
  v12 = dynEntPose->pose.origin[2];
  if ( modelAxis[0][0] >= 0.0 )
    v9 = 0;
  else
    v9 = -1;
  if ( modelAxis[0][1] >= 0.0 )
    v8 = 0;
  else
    v8 = -1;
  if ( modelAxis[0][2] >= 0.0 )
    v7 = 0;
  else
    v7 = -1;
  if ( modelAxis[1][0] >= 0.0 )
    v6 = 0;
  else
    v6 = -1;
  if ( modelAxis[1][1] >= 0.0 )
    v5 = 0;
  else
    v5 = -1;
  if ( modelAxis[1][2] >= 0.0 )
    v4 = 0;
  else
    v4 = -1;
  if ( modelAxis[2][0] >= 0.0 )
    v3 = 0;
  else
    v3 = -1;
  if ( modelAxis[2][1] >= 0.0 )
    v2 = 0;
  else
    v2 = -1;
  if ( modelAxis[2][2] >= 0.0 )
    v1 = 0;
  else
    v1 = -1;
  worldBoundsFloat4[0].v[0] = (float)(COERCE_FLOAT(modelBoundsFloat4[1].u[0] & v9 | modelBoundsFloat4[0].u[0] & ~v9)
                                    * modelAxis[0][0])
                            + v10;
  worldBoundsFloat4[0].v[1] = (float)(COERCE_FLOAT(modelBoundsFloat4[1].u[0] & v8 | modelBoundsFloat4[0].u[0] & ~v8)
                                    * modelAxis[0][1])
                            + v11;
  worldBoundsFloat4[0].v[2] = (float)(COERCE_FLOAT(modelBoundsFloat4[1].u[0] & v7 | modelBoundsFloat4[0].u[0] & ~v7)
                                    * modelAxis[0][2])
                            + v12;
  worldBoundsFloat4[0].v[3] = (float)(modelBoundsFloat4[0].v[0] * 0.0) + 0.0;
  worldBoundsFloat4[0].v[0] = (float)(COERCE_FLOAT(modelBoundsFloat4[1].u[1] & v6 | modelBoundsFloat4[0].u[1] & ~v6)
                                    * modelAxis[1][0])
                            + worldBoundsFloat4[0].v[0];
  worldBoundsFloat4[0].v[1] = (float)(COERCE_FLOAT(modelBoundsFloat4[1].u[1] & v5 | modelBoundsFloat4[0].u[1] & ~v5)
                                    * modelAxis[1][1])
                            + worldBoundsFloat4[0].v[1];
  worldBoundsFloat4[0].v[2] = (float)(COERCE_FLOAT(modelBoundsFloat4[1].u[1] & v4 | modelBoundsFloat4[0].u[1] & ~v4)
                                    * modelAxis[1][2])
                            + worldBoundsFloat4[0].v[2];
  worldBoundsFloat4[0].v[3] = (float)(modelBoundsFloat4[0].v[1] * 0.0) + worldBoundsFloat4[0].v[3];
  worldBoundsFloat4[0].v[0] = (float)(COERCE_FLOAT(modelBoundsFloat4[1].u[2] & v3 | modelBoundsFloat4[0].u[2] & ~v3)
                                    * modelAxis[2][0])
                            + worldBoundsFloat4[0].v[0];
  worldBoundsFloat4[0].v[1] = (float)(COERCE_FLOAT(modelBoundsFloat4[1].u[2] & v2 | modelBoundsFloat4[0].u[2] & ~v2)
                                    * modelAxis[2][1])
                            + worldBoundsFloat4[0].v[1];
  worldBoundsFloat4[0].v[2] = (float)(COERCE_FLOAT(modelBoundsFloat4[1].u[2] & v1 | modelBoundsFloat4[0].u[2] & ~v1)
                                    * modelAxis[2][2])
                            + worldBoundsFloat4[0].v[2];
  worldBoundsFloat4[0].v[3] = (float)(modelBoundsFloat4[0].v[2] * 0.0) + worldBoundsFloat4[0].v[3];
  worldBoundsFloat4[1].v[0] = (float)(COERCE_FLOAT(modelBoundsFloat4[0].u[0] & v9 | modelBoundsFloat4[1].u[0] & ~v9)
                                    * modelAxis[0][0])
                            + v10;
  worldBoundsFloat4[1].v[1] = (float)(COERCE_FLOAT(modelBoundsFloat4[0].u[0] & v8 | modelBoundsFloat4[1].u[0] & ~v8)
                                    * modelAxis[0][1])
                            + v11;
  worldBoundsFloat4[1].v[2] = (float)(COERCE_FLOAT(modelBoundsFloat4[0].u[0] & v7 | modelBoundsFloat4[1].u[0] & ~v7)
                                    * modelAxis[0][2])
                            + v12;
  worldBoundsFloat4[1].v[3] = (float)(modelBoundsFloat4[1].v[0] * 0.0) + 0.0;
  worldBoundsFloat4[1].v[0] = (float)(COERCE_FLOAT(modelBoundsFloat4[0].u[1] & v6 | modelBoundsFloat4[1].u[1] & ~v6)
                                    * modelAxis[1][0])
                            + worldBoundsFloat4[1].v[0];
  worldBoundsFloat4[1].v[1] = (float)(COERCE_FLOAT(modelBoundsFloat4[0].u[1] & v5 | modelBoundsFloat4[1].u[1] & ~v5)
                                    * modelAxis[1][1])
                            + worldBoundsFloat4[1].v[1];
  worldBoundsFloat4[1].v[2] = (float)(COERCE_FLOAT(modelBoundsFloat4[0].u[1] & v4 | modelBoundsFloat4[1].u[1] & ~v4)
                                    * modelAxis[1][2])
                            + worldBoundsFloat4[1].v[2];
  worldBoundsFloat4[1].v[3] = (float)(modelBoundsFloat4[1].v[1] * 0.0) + worldBoundsFloat4[1].v[3];
  worldBoundsFloat4[1].v[0] = (float)(COERCE_FLOAT(modelBoundsFloat4[0].u[2] & v3 | modelBoundsFloat4[1].u[2] & ~v3)
                                    * modelAxis[2][0])
                            + worldBoundsFloat4[1].v[0];
  worldBoundsFloat4[1].v[1] = (float)(COERCE_FLOAT(modelBoundsFloat4[0].u[2] & v2 | modelBoundsFloat4[1].u[2] & ~v2)
                                    * modelAxis[2][1])
                            + worldBoundsFloat4[1].v[1];
  worldBoundsFloat4[1].v[2] = (float)(COERCE_FLOAT(modelBoundsFloat4[0].u[2] & v1 | modelBoundsFloat4[1].u[2] & ~v1)
                                    * modelAxis[2][2])
                            + worldBoundsFloat4[1].v[2];
  worldBoundsFloat4[1].v[3] = (float)(modelBoundsFloat4[1].v[2] * 0.0) + worldBoundsFloat4[1].v[3];
  *(_QWORD *)&worldBoundsVec3[0][0] = *(_QWORD *)worldBoundsFloat4[0].v;
  LODWORD(worldBoundsVec3[0][2]) = worldBoundsFloat4[0].u[2];
  worldBoundsVec3[1][0] = worldBoundsFloat4[1].v[0];
  worldBoundsVec3[1][1] = worldBoundsFloat4[1].v[1];
  worldBoundsVec3[1][2] = worldBoundsFloat4[1].v[2];
  DynEnt_LinkEntity(DYNENT_COLL_CLIENT_FIRST, dynEntId, worldBoundsVec3[0], worldBoundsVec3[1]);
  R_LinkDynEnt(dynEntId, DYNENT_DRAW_MODEL, worldBoundsVec3[0], worldBoundsVec3[1]);
}

const DynEntityDef *__cdecl DynEnt_GetEntityDef(unsigned __int16 dynEntId, DynEntityDrawType drawType)
{
  if ( dynEntId >= (unsigned int)cm.dynEntCount[drawType]
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\dynentity\\DynEntity_load_obj.h",
          60,
          0,
          "dynEntId doesn't index cm.dynEntCount[DynEntGetClientCollType( drawType )]\n\t%i not in [0, %i)",
          dynEntId,
          cm.dynEntCount[drawType]) )
  {
    __debugbreak();
  }
  return (const DynEntityDef *)((char *)cm.dynEntPoseList[drawType - 2] + 84 * dynEntId);
}

DynEntityClient *__cdecl DynEnt_GetClientEntity(unsigned __int16 dynEntId, DynEntityDrawType drawType)
{
  if ( dynEntId >= (unsigned int)cm.dynEntCount[drawType]
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\dynentity\\DynEntity_load_obj.h",
          67,
          0,
          "dynEntId doesn't index cm.dynEntCount[DynEntGetClientCollType( drawType )]\n\t%i not in [0, %i)",
          dynEntId,
          cm.dynEntCount[drawType]) )
  {
    __debugbreak();
  }
  return &cm.dynEntClientList[drawType][dynEntId];
}

void __cdecl DynEntCl_LinkBrush(unsigned __int16 dynEntId)
{
  DynEntityPose *dynEntPose; // [esp+4h] [ebp-24h]
  float absMaxs[3]; // [esp+8h] [ebp-20h] BYREF
  const DynEntityDef *dynEntDef; // [esp+14h] [ebp-14h]
  GfxBrushModel *bmodel; // [esp+18h] [ebp-10h]
  float absMins[3]; // [esp+1Ch] [ebp-Ch] BYREF

  dynEntDef = DynEnt_GetEntityDef(dynEntId, DYNENT_DRAW_BRUSH);
  dynEntPose = DynEnt_GetClientPose(dynEntId, DYNENT_DRAW_BRUSH);
  if ( dynEntDef->xModel
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          178,
          0,
          "%s",
          "!dynEntDef->xModel") )
  {
    __debugbreak();
  }
  dynEntPose->radius = DynEntCl_UpdateBModelWorldBounds(dynEntDef, &dynEntPose->pose);
  bmodel = R_GetBrushModel(dynEntDef->brushModel);
  absMins[0] = bmodel->writable.mins[0];
  absMins[1] = bmodel->writable.mins[1];
  absMins[2] = bmodel->writable.mins[2];
  absMaxs[0] = bmodel->writable.maxs[0];
  absMaxs[1] = bmodel->writable.maxs[1];
  absMaxs[2] = bmodel->writable.maxs[2];
  DynEnt_LinkEntity(DYNENT_COLL_CLIENT_BRUSH, dynEntId, absMins, absMaxs);
  R_LinkDynEnt(dynEntId, DYNENT_DRAW_BRUSH, absMins, absMaxs);
}

double __cdecl DynEntCl_UpdateBModelWorldBounds(const DynEntityDef *dynEntDef, const GfxPlacement *pose)
{
  int v3; // [esp+4h] [ebp-1C0h]
  int v4; // [esp+8h] [ebp-1BCh]
  int v5; // [esp+Ch] [ebp-1B8h]
  int v6; // [esp+14h] [ebp-1B0h]
  int v7; // [esp+18h] [ebp-1ACh]
  int v8; // [esp+1Ch] [ebp-1A8h]
  int v9; // [esp+24h] [ebp-1A0h]
  int v10; // [esp+28h] [ebp-19Ch]
  int v11; // [esp+2Ch] [ebp-198h]
  float *maxs; // [esp+30h] [ebp-194h]
  float v13; // [esp+134h] [ebp-90h]
  float v14; // [esp+138h] [ebp-8Ch]
  float v15; // [esp+13Ch] [ebp-88h]
  __int64 rotatedBounds; // [esp+158h] [ebp-6Ch]
  float rotatedBounds_8; // [esp+160h] [ebp-64h]
  float rotatedBounds_16; // [esp+168h] [ebp-5Ch]
  float rotatedBounds_20; // [esp+16Ch] [ebp-58h]
  float rotatedBounds_24; // [esp+170h] [ebp-54h]
  __int64 bounds; // [esp+178h] [ebp-4Ch]
  __int64 bounds_8; // [esp+180h] [ebp-44h]
  __int64 bounds_16; // [esp+188h] [ebp-3Ch]
  __int64 bounds_24; // [esp+190h] [ebp-34h]
  float axis[3][3]; // [esp+19Ch] [ebp-28h] BYREF
  GfxBrushModel *bmodel; // [esp+1C0h] [ebp-4h]

  bmodel = R_GetBrushModel(dynEntDef->brushModel);
  bounds = *(_QWORD *)&bmodel->bounds[0][0];
  bounds_8 = *(_QWORD *)&bmodel->bounds[0][2];
  bounds_16 = *(_QWORD *)&bmodel->bounds[1][0];
  bounds_24 = *(_QWORD *)&bmodel->bounds[1][2];
  UnitQuatToAxis(pose->quat, axis);
  v13 = pose->origin[0];
  v14 = pose->origin[1];
  v15 = pose->origin[2];
  if ( axis[0][0] >= 0.0 )
    v11 = 0;
  else
    v11 = -1;
  if ( axis[0][1] >= 0.0 )
    v10 = 0;
  else
    v10 = -1;
  if ( axis[0][2] >= 0.0 )
    v9 = 0;
  else
    v9 = -1;
  if ( axis[1][0] >= 0.0 )
    v8 = 0;
  else
    v8 = -1;
  if ( axis[1][1] >= 0.0 )
    v7 = 0;
  else
    v7 = -1;
  if ( axis[1][2] >= 0.0 )
    v6 = 0;
  else
    v6 = -1;
  if ( axis[2][0] >= 0.0 )
    v5 = 0;
  else
    v5 = -1;
  if ( axis[2][1] >= 0.0 )
    v4 = 0;
  else
    v4 = -1;
  if ( axis[2][2] >= 0.0 )
    v3 = 0;
  else
    v3 = -1;
  *(float *)&rotatedBounds = (float)(COERCE_FLOAT(bounds_24 & v5 | bounds_8 & ~v5) * axis[2][0])
                           + (float)((float)(COERCE_FLOAT(HIDWORD(bounds_16) & v8 | HIDWORD(bounds) & ~v8) * axis[1][0])
                                   + (float)((float)(COERCE_FLOAT(bounds_16 & v11 | bounds & ~v11) * axis[0][0]) + v13));
  *((float *)&rotatedBounds + 1) = (float)(COERCE_FLOAT(bounds_24 & v4 | bounds_8 & ~v4) * axis[2][1])
                                 + (float)((float)(COERCE_FLOAT(HIDWORD(bounds_16) & v7 | HIDWORD(bounds) & ~v7)
                                                 * axis[1][1])
                                         + (float)((float)(COERCE_FLOAT(bounds_16 & v10 | bounds & ~v10) * axis[0][1])
                                                 + v14));
  rotatedBounds_8 = (float)(COERCE_FLOAT(bounds_24 & v3 | bounds_8 & ~v3) * axis[2][2])
                  + (float)((float)(COERCE_FLOAT(HIDWORD(bounds_16) & v6 | HIDWORD(bounds) & ~v6) * axis[1][2])
                          + (float)((float)(COERCE_FLOAT(bounds_16 & v9 | bounds & ~v9) * axis[0][2]) + v15));
  rotatedBounds_16 = (float)(COERCE_FLOAT(bounds_8 & v5 | bounds_24 & ~v5) * axis[2][0])
                   + (float)((float)(COERCE_FLOAT(HIDWORD(bounds) & v8 | HIDWORD(bounds_16) & ~v8) * axis[1][0])
                           + (float)((float)(COERCE_FLOAT(bounds & v11 | bounds_16 & ~v11) * axis[0][0]) + v13));
  rotatedBounds_20 = (float)(COERCE_FLOAT(bounds_8 & v4 | bounds_24 & ~v4) * axis[2][1])
                   + (float)((float)(COERCE_FLOAT(HIDWORD(bounds) & v7 | HIDWORD(bounds_16) & ~v7) * axis[1][1])
                           + (float)((float)(COERCE_FLOAT(bounds & v10 | bounds_16 & ~v10) * axis[0][1]) + v14));
  rotatedBounds_24 = (float)(COERCE_FLOAT(bounds_8 & v3 | bounds_24 & ~v3) * axis[2][2])
                   + (float)((float)(COERCE_FLOAT(HIDWORD(bounds) & v6 | HIDWORD(bounds_16) & ~v6) * axis[1][2])
                           + (float)((float)(COERCE_FLOAT(bounds & v9 | bounds_16 & ~v9) * axis[0][2]) + v15));
  *(_QWORD *)bmodel->writable.mins = rotatedBounds;
  bmodel->writable.mins[2] = rotatedBounds_8;
  maxs = bmodel->writable.maxs;
  bmodel->writable.maxs[0] = rotatedBounds_16;
  maxs[1] = rotatedBounds_20;
  maxs[2] = rotatedBounds_24;
  return RadiusFromBounds(bmodel->bounds[0], bmodel->bounds[1]);
}

void __cdecl DynEntCl_UnlinkEntity(unsigned __int16 dynEntId, DynEntityCollType drawType)
{
  DynEnt_UnlinkEntity(drawType, dynEntId);
  R_UnlinkDynEnt(dynEntId, (DynEntityDrawType)drawType);
}

void __cdecl DynEnt_DestroyPhysics(PhysWorld worldIndex, const DynEntityDef *dynEntDef, DynEntityClient *dynEntClient)
{
  __int16 cindex; // [esp+4h] [ebp-Ch]
  int i; // [esp+8h] [ebp-8h]
  int physObjId; // [esp+Ch] [ebp-4h]

  if ( dynEntClient->physObjId )
  {
    physObjId = dynEntClient->physObjId;
    dynEntClient->physObjId = 0;
    Phys_ObjDestroy(0, physObjId);
  }
  for ( i = 0; i < 4; ++i )
  {
    cindex = dynEntDef->physConstraints[i];
    if ( cindex != 511 )
      cm.constraints[cindex].constraintHandle = 0;
  }
}

void __cdecl DynEntCl_Shutdown(int localClientNum)
{
  int i; // [esp+0h] [ebp-1Ch]
  DynEntityDrawType drawType; // [esp+4h] [ebp-18h]
  DynEntityClient *dynEntClient; // [esp+Ch] [ebp-10h]
  DynEntityClient *dynEntClienta; // [esp+Ch] [ebp-10h]
  const DynEntityDef *dynEntDefa; // [esp+10h] [ebp-Ch]
  const DynEntityDef *dynEntDef; // [esp+10h] [ebp-Ch]
  unsigned __int16 dynEntCount; // [esp+14h] [ebp-8h]
  unsigned __int16 dynEntId; // [esp+18h] [ebp-4h]
  unsigned __int16 dynEntIda; // [esp+18h] [ebp-4h]

  if ( CL_LocalClient_GetActiveCount() && localClientNum == jpeg_mem_init() )
  {
    DynEnt_ResetBurning();
    DynEnt_ResetFading();
    for ( dynEntId = cm.originalDynEntCount; dynEntId < (int)cm.dynEntCount[0]; ++dynEntId )
    {
      dynEntClient = DynEnt_GetClientEntity(dynEntId, DYNENT_DRAW_MODEL);
      if ( dynEntClient->physObjId )
      {
        dynEntDefa = DynEnt_GetEntityDef(dynEntId, DYNENT_DRAW_MODEL);
        DynEnt_DestroyPhysics(PHYS_WORLD_DYNENT, dynEntDefa, dynEntClient);
      }
      if ( (dynEntClient->flags & 1) != 0 )
        DynEntCl_UnlinkEntity(dynEntId, DYNENT_COLL_CLIENT_FIRST);
    }
    cm.dynEntCount[0] = cm.originalDynEntCount;
    for ( drawType = DYNENT_DRAW_MODEL; (unsigned int)drawType < DYNENT_DRAW_COUNT; ++drawType )
    {
      dynEntCount = DynEnt_GetEntityCount((DynEntityCollType)drawType);
      for ( dynEntIda = 0; dynEntIda < (int)dynEntCount; ++dynEntIda )
      {
        dynEntDef = DynEnt_GetEntityDef(dynEntIda, drawType);
        dynEntClienta = DynEnt_GetClientEntity(dynEntIda, drawType);
        if ( dynEntClienta->physObjId )
          DynEnt_DestroyPhysics(PHYS_WORLD_DYNENT, dynEntDef, dynEntClienta);
        if ( (dynEntClienta->flags & 1) != 0 )
          DynEntCl_UnlinkEntity(dynEntIda, (DynEntityCollType)drawType);
      }
      DynEnt_ClearCollWorld((DynEntityCollType)drawType);
    }
    numExtraDynEnts = 0;
    for ( i = 0; i < 256; ++i )
      usedExtraDynEnts[i] = 0;
  }
}

void __cdecl DynEntCl_InitEntities(int localClientNum)
{
  int i; // [esp+8h] [ebp-28h]
  DynEntityPose *dynEntPosea; // [esp+Ch] [ebp-24h]
  DynEntityPose *dynEntPose; // [esp+Ch] [ebp-24h]
  DynEntityDrawType drawType; // [esp+10h] [ebp-20h]
  DynEntityDrawType drawTypea; // [esp+10h] [ebp-20h]
  DynEntityClient *dynEntClient; // [esp+1Ch] [ebp-14h]
  DynEntityClient *dynEntClienta; // [esp+1Ch] [ebp-14h]
  DynEntityClient *dynEntClientb; // [esp+1Ch] [ebp-14h]
  const DynEntityDef *dynEntDefb; // [esp+20h] [ebp-10h]
  const DynEntityDef *dynEntDef; // [esp+20h] [ebp-10h]
  const DynEntityDef *dynEntDefa; // [esp+20h] [ebp-10h]
  unsigned __int16 dynEntCount; // [esp+24h] [ebp-Ch]
  unsigned __int16 dynEntCounta; // [esp+24h] [ebp-Ch]
  unsigned __int16 dynEntId; // [esp+28h] [ebp-8h]
  unsigned __int16 dynEntIda; // [esp+28h] [ebp-8h]
  unsigned __int16 dynEntIdb; // [esp+28h] [ebp-8h]
  DynEntityColl *dynEntColl; // [esp+2Ch] [ebp-4h]

  if ( localClientNum == jpeg_mem_init() && !r_reflectionProbeGenerate->current.enabled )
  {
    DynEnt_ResetBurning();
    DynEnt_ResetFading();
    for ( dynEntId = cm.originalDynEntCount; dynEntId < (int)cm.dynEntCount[0]; ++dynEntId )
    {
      dynEntClient = DynEnt_GetClientEntity(dynEntId, DYNENT_DRAW_MODEL);
      if ( dynEntClient->physObjId )
      {
        dynEntDefb = DynEnt_GetEntityDef(dynEntId, DYNENT_DRAW_MODEL);
        DynEnt_DestroyPhysics(PHYS_WORLD_DYNENT, dynEntDefb, dynEntClient);
      }
      DynEntCl_UnlinkEntity(dynEntId, DYNENT_COLL_CLIENT_FIRST);
    }
    cm.dynEntCount[0] = cm.originalDynEntCount;
    for ( drawType = DYNENT_DRAW_MODEL; (unsigned int)drawType < DYNENT_DRAW_COUNT; ++drawType )
    {
      DynEnt_ClearCollWorld((DynEntityCollType)drawType);
      dynEntCount = DynEnt_GetEntityCount((DynEntityCollType)drawType);
      for ( dynEntIda = 0; dynEntIda < (int)dynEntCount; ++dynEntIda )
      {
        dynEntDef = DynEnt_GetEntityDef(dynEntIda, drawType);
        dynEntPosea = DynEnt_GetClientPose(dynEntIda, drawType);
        dynEntClienta = DynEnt_GetClientEntity(dynEntIda, drawType);
        dynEntColl = DynEnt_GetEntityColl((DynEntityCollType)drawType, dynEntIda);
        memcpy(dynEntPosea, &dynEntDef->pose, 0x1Cu);
        if ( dynEntClienta->physObjId )
          DynEnt_DestroyPhysics(PHYS_WORLD_DYNENT, dynEntDef, dynEntClienta);
        dynEntColl->nextEntInSector = 0;
        dynEntColl->sector = 0;
        dynEntClienta->flags = 2;
        dynEntClienta->health = dynEntDef->health;
        DynEnt_GetEntityProps(dynEntDef->type);
        dynEntClienta->flags |= 1u;
        if ( drawType )
          DynEntCl_LinkBrush(dynEntIda);
        else
          DynEntCl_LinkModel(dynEntIda);
        if ( dynEntDef->physPreset && (dynEntDef->physPreset->flags & 1) != 0 && (dynEntDef->physPreset->flags & 2) == 0 )
          DynEnt_CalcPhysPreset(dynEntDef);
      }
    }
    for ( drawTypea = DYNENT_DRAW_MODEL; (unsigned int)drawTypea < DYNENT_DRAW_COUNT; ++drawTypea )
    {
      dynEntCounta = DynEnt_GetEntityCount((DynEntityCollType)drawTypea);
      for ( dynEntIdb = 0; dynEntIdb < (int)dynEntCounta; ++dynEntIdb )
      {
        dynEntDefa = DynEnt_GetEntityDef(dynEntIdb, drawTypea);
        dynEntPose = DynEnt_GetClientPose(dynEntIdb, drawTypea);
        dynEntClientb = DynEnt_GetClientEntity(dynEntIdb, drawTypea);
        if ( (dynEntDefa->flags & 1) != 0 && !dynEntClientb->physObjId )
          dynEntClientb->physObjId = DynEntCl_CreatePhysObj(dynEntDefa, dynEntClientb, &dynEntPose->pose);
      }
    }
    numExtraDynEnts = 0;
    for ( i = 0; i < 256; ++i )
      usedExtraDynEnts[i] = 0;
  }
}

DynEntityColl *__cdecl DynEnt_GetEntityColl(DynEntityCollType collType, unsigned __int16 dynEntId)
{
  if ( (unsigned int)collType >= DYNENT_COLL_COUNT
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\dynentity\\DynEntity_load_obj.h",
          87,
          0,
          "collType doesn't index DYNENT_COLL_COUNT\n\t%i not in [0, %i)",
          collType,
          4) )
  {
    __debugbreak();
  }
  if ( dynEntId >= (unsigned int)cm.dynEntCount[collType]
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\dynentity\\DynEntity_load_obj.h",
          88,
          0,
          "dynEntId doesn't index cm.dynEntCount[collType]\n\t%i not in [0, %i)",
          dynEntId,
          cm.dynEntCount[collType]) )
  {
    __debugbreak();
  }
  return &cm.dynEntCollList[collType][dynEntId];
}

void __cdecl DynEntCl_DestroyEntityModel(unsigned __int16 id)
{
  DynEntityClient *ent; // [esp+0h] [ebp-10h]
  int extraId; // [esp+4h] [ebp-Ch]
  unsigned __int16 absDynEntId; // [esp+8h] [ebp-8h]

  ent = &cm.dynEntClientList[0][id];
  ent->flags = 0;
  ent->health = -1;
  if ( ent->physObjId && ent->physObjId != -1 )
    Phys_ObjDestroy(0, ent->physObjId);
  absDynEntId = DynEnt_GetClientAbsId(id, DYNENT_DRAW_MODEL);
  DynEntCl_RemoveFromFadeList(absDynEntId);
  ent->physObjId = 0;
  DynEntCl_SetBurning(id, DYNENT_DRAW_MODEL, 0);
  DynEntCl_UnlinkEntity(id, DYNENT_COLL_CLIENT_FIRST);
  if ( id == cm.dynEntCount[0] - 1 )
    --cm.dynEntCount[0];
  extraId = id - cm.originalDynEntCount;
  if ( usedExtraDynEnts[extraId] != 1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          434,
          0,
          "%s",
          "usedExtraDynEnts[extraId] == 1") )
  {
    __debugbreak();
  }
  usedExtraDynEnts[extraId] = 0;
  if ( --numExtraDynEnts < 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          439,
          0,
          "%s",
          "numExtraDynEnts >= 0") )
  {
    __debugbreak();
  }
}

void __cdecl DynEnt_FixupLightConstraints(int localClientNum, int entnum, int primaryLightIndex)
{
  const DynEntityPose *dynEntPose; // [esp+40h] [ebp-4Ch]
  float dynent_mat[3][3]; // [esp+44h] [ebp-48h] BYREF
  float offset[3]; // [esp+68h] [ebp-24h] BYREF
  float forward[3]; // [esp+74h] [ebp-18h] BYREF
  centity_s *light_cent; // [esp+80h] [ebp-Ch]
  PhysConstraint *c; // [esp+84h] [ebp-8h]
  int i; // [esp+88h] [ebp-4h]

  for ( i = 0; i < cm.num_constraints; ++i )
  {
    c = &cm.constraints[i];
    if ( c->type == CONSTRAINT_LIGHT && c->target_index2 == primaryLightIndex )
    {
      c->centity_num[localClientNum] = entnum;
      light_cent = CG_GetEntity(localClientNum, entnum);
      if ( c->target_index1 < 0 )
        Com_Error(
          ERR_DROP,
          "Bad Light constraint at (%g %g %g) linked to entity at (%g %g %g)",
          c->pos[0],
          c->pos[1],
          c->pos[2],
          light_cent->pose.origin[0],
          light_cent->pose.origin[1],
          light_cent->pose.origin[2]);
      dynEntPose = DynEnt_GetClientPose(c->target_index1);
      UnitQuatToAxis(dynEntPose->pose.quat, dynent_mat);
      offset[0] = light_cent->pose.origin[0] - dynEntPose->pose.origin[0];
      offset[1] = light_cent->pose.origin[1] - dynEntPose->pose.origin[1];
      offset[2] = light_cent->pose.origin[2] - dynEntPose->pose.origin[2];
      MatrixTransposeTransformVector(offset, dynent_mat, c->offset);
      AngleVectors(light_cent->pose.angles, forward, 0, 0);
      MatrixTransposeTransformVector(forward, dynent_mat, c->dir);
      c->constraintHandle = 1;
    }
  }
}

void __cdecl MatrixTransposeTransformVector(const float *in1, const float (*in2)[3], float *out)
{
  if ( in1 == out
    && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.h", 1423, 0, "%s", "in1 != out") )
  {
    __debugbreak();
  }
  *out = (float)((float)(*in1 * (*in2)[0]) + (float)(in1[1] * (float)(*in2)[1])) + (float)(in1[2] * (float)(*in2)[2]);
  out[1] = (float)((float)(*in1 * (float)(*in2)[3]) + (float)(in1[1] * (float)(*in2)[4]))
         + (float)(in1[2] * (float)(*in2)[5]);
  out[2] = (float)((float)(*in1 * (float)(*in2)[6]) + (float)(in1[1] * (float)(*in2)[7]))
         + (float)(in1[2] * (float)(*in2)[8]);
}

void __cdecl DynEntCl_ProcessEntities(int localClientNum, int time)
{
  DynEntityPose *dynEntPose; // [esp+18h] [ebp-20h]
  DynEntityPose *dynEntPosea; // [esp+18h] [ebp-20h]
  float origin[3]; // [esp+1Ch] [ebp-1Ch] BYREF
  DynEntityClient *dynEntClient; // [esp+28h] [ebp-10h]
  const DynEntityDef *dynEntDef; // [esp+2Ch] [ebp-Ch]
  unsigned __int16 dynEntCount; // [esp+30h] [ebp-8h]
  unsigned __int16 dynEntId; // [esp+34h] [ebp-4h]

  if ( !r_reflectionProbeGenerate->current.enabled )
  {
    //PIXBeginNamedEvent(-1, "process dyn ents");
    dynEntCount = DynEnt_GetEntityCount(DYNENT_COLL_CLIENT_FIRST);
    for ( dynEntId = 0; dynEntId < (int)dynEntCount; ++dynEntId )
    {
      dynEntClient = DynEnt_GetClientEntity(dynEntId, DYNENT_DRAW_MODEL);
      if ( (dynEntClient->flags & 1) != 0 && dynEntClient->physObjId )
      {
        dynEntDef = DynEnt_GetEntityDef(dynEntId, DYNENT_DRAW_MODEL);
        if ( Phys_ObjIsAsleep(dynEntClient->physObjId) )
        {
          DynEnt_DestroyPhysics(PHYS_WORLD_DYNENT, dynEntDef, dynEntClient);
        }
        else
        {
          dynEntPose = DynEnt_GetClientPose(dynEntId, DYNENT_DRAW_MODEL);
          Phys_ObjGetInterpolatedState(dynEntClient->physObjId, origin, dynEntPose->pose.quat);
          DynEnt_UpdateConstraints(localClientNum, dynEntClient, dynEntDef);
          if ( !VecNCompareCustomEpsilon(origin, dynEntPose->pose.origin, 0.0099999998, 3) )
          {
            dynEntPose->pose.origin[0] = origin[0];
            dynEntPose->pose.origin[1] = origin[1];
            dynEntPose->pose.origin[2] = origin[2];
            DynEntCl_LinkModel(dynEntId);
          }
          DynEnt_UpdateLightConstraint(localClientNum, dynEntDef, dynEntPose);
        }
      }
    }
    dynEntCount = DynEnt_GetEntityCount(DYNENT_COLL_CLIENT_BRUSH);
    for ( dynEntId = 0; dynEntId < (int)dynEntCount; ++dynEntId )
    {
      dynEntClient = DynEnt_GetClientEntity(dynEntId, DYNENT_DRAW_BRUSH);
      if ( (dynEntClient->flags & 1) != 0 && dynEntClient->physObjId )
      {
        dynEntDef = DynEnt_GetEntityDef(dynEntId, DYNENT_DRAW_BRUSH);
        if ( Phys_ObjIsAsleep(dynEntClient->physObjId) )
        {
          DynEnt_DestroyPhysics(PHYS_WORLD_DYNENT, dynEntDef, dynEntClient);
        }
        else
        {
          dynEntPosea = DynEnt_GetClientPose(dynEntId, DYNENT_DRAW_BRUSH);
          Phys_ObjGetInterpolatedState(dynEntClient->physObjId, origin, dynEntPosea->pose.quat);
          DynEnt_UpdateConstraints(localClientNum, dynEntClient, dynEntDef);
          if ( !VecNCompareCustomEpsilon(origin, dynEntPosea->pose.origin, 0.0099999998, 3) )
          {
            dynEntPosea->pose.origin[0] = origin[0];
            dynEntPosea->pose.origin[1] = origin[1];
            dynEntPosea->pose.origin[2] = origin[2];
            DynEntCl_LinkBrush(dynEntId);
          }
          DynEnt_UpdateLightConstraint(localClientNum, dynEntDef, dynEntPosea);
        }
      }
    }
    DynEnt_UpdateBurning();
    DynEnt_UpdateFading(time);
    if ( g_DXDeviceThread == GetCurrentThreadId() )
      D3DPERF_EndEvent();
  }
}

void __cdecl DynEnt_UpdateLightConstraint(
        int localClientNum,
        const DynEntityDef *dynEntDef,
        const DynEntityPose *dynEntPose)
{
  centity_s *cent; // [esp+Ch] [ebp-6Ch]
  int entnum; // [esp+10h] [ebp-68h]
  PhysConstraint *c; // [esp+14h] [ebp-64h]
  float dynent_mat[3][3]; // [esp+18h] [ebp-60h] BYREF
  float origin[3]; // [esp+3Ch] [ebp-3Ch] BYREF
  int cindex; // [esp+48h] [ebp-30h]
  float rotated_offset[3]; // [esp+4Ch] [ebp-2Ch] BYREF
  float angles[3]; // [esp+58h] [ebp-20h] BYREF
  int i; // [esp+64h] [ebp-14h]
  float rotated_dir[3]; // [esp+68h] [ebp-10h] BYREF
  int clientindex; // [esp+74h] [ebp-4h]

  for ( i = 0; i < 4; ++i )
  {
    cindex = dynEntDef->physConstraints[i];
    if ( cindex != 511 )
    {
      c = &cm.constraints[cindex];
      if ( c->type == CONSTRAINT_LIGHT )
      {
        for ( clientindex = 0; clientindex < 1; ++clientindex )
        {
          entnum = c->centity_num[clientindex];
          if ( entnum > 0 )
          {
            cent = CG_GetEntity(clientindex, entnum);
            UnitQuatToAxis(dynEntPose->pose.quat, dynent_mat);
            MatrixTransformVector(c->offset, dynent_mat, rotated_offset);
            origin[0] = dynEntPose->pose.origin[0] + rotated_offset[0];
            origin[1] = dynEntPose->pose.origin[1] + rotated_offset[1];
            origin[2] = dynEntPose->pose.origin[2] + rotated_offset[2];
            CG_SetOrigin(cent, origin);
            MatrixTransformVector(c->dir, dynent_mat, rotated_dir);
            vectoangles(rotated_dir, angles);
            CG_SetAngle(cent, angles);
          }
        }
      }
    }
  }
}

void __cdecl DynEnt_UpdateConstraints(int localClientNum, DynEntityClient *dynEntClient, const DynEntityDef *dynEntDef)
{
  bool v3; // [esp+0h] [ebp-3Ch]
  centity_s *light; // [esp+10h] [ebp-2Ch]
  int entnum; // [esp+14h] [ebp-28h]
  int clientindex; // [esp+18h] [ebp-24h]
  PhysConstraint *c; // [esp+28h] [ebp-14h]
  bool removeConstraint; // [esp+2Fh] [ebp-Dh]
  __int16 cindex; // [esp+30h] [ebp-Ch]
  int i; // [esp+34h] [ebp-8h]
  int damage; // [esp+38h] [ebp-4h]

  damage = DynEnt_GetDamage(dynEntClient, dynEntDef);
  if ( damage )
  {
    for ( i = 0; i < 4; ++i )
    {
      cindex = dynEntDef->physConstraints[i];
      if ( cindex != 511 )
      {
        c = &cm.constraints[cindex];
        v3 = c->constraintHandle && c->min_health && c->min_health < damage;
        removeConstraint = v3;
        if ( c->constraintHandle
          && c->timeout
          && CG_GetLocalClientGlobals(localClientNum)->time > dynEntClient->physicsStartTime + c->timeout )
        {
          removeConstraint = 1;
        }
        if ( removeConstraint )
        {
          if ( dynEntClient->physObjId )
          {
            *(unsigned int *)(*(unsigned int *)dynEntClient->physObjId + 272) = 3;
            Phys_ObjAddCollFlags(dynEntClient->physObjId, 455);
          }
          switch ( c->type )
          {
            case CONSTRAINT_POINT:
              phys_sys::destroy((rigid_body_constraint_point *const)c->constraintHandle);
              break;
            case CONSTRAINT_DISTANCE:
            case CONSTRAINT_ROPE:
              if ( Rope_IsValid(c->rope_index) )
              {
                if ( (g_ropes[c->rope_index].m_flags & 0x20) != 0 )
                  Rope_RemoveAnchor(c->rope_index, g_ropes[c->rope_index].m_num_particles - 1);
                else
                  Rope_RemoveAnchor(c->rope_index, 0);
              }
              c->rope_index = -1;
              phys_sys::destroy((rigid_body_constraint_distance *const)c->constraintHandle);
              break;
            case CONSTRAINT_HINGE:
              phys_sys::destroy((rigid_body_constraint_hinge *const)c->constraintHandle);
              break;
            case CONSTRAINT_LIGHT:
              for ( clientindex = 0; clientindex < 1; ++clientindex )
              {
                entnum = c->centity_num[clientindex];
                if ( entnum != -1 )
                {
                  if ( CL_LocalClient_IsActive(clientindex) )
                  {
                    light = CG_GetEntity(clientindex, entnum);
                    if ( light )
                      light->nextState.lerp.u.loopFx.period = *(unsigned int *)&FLOAT_0_0;
                  }
                }
              }
              break;
          }
          c->constraintHandle = 0;
        }
      }
    }
  }
}

int __cdecl DynEnt_GetDamage(DynEntityClient *dynEntClient, const DynEntityDef *dynEntDef)
{
  if ( dynEntDef->health <= 0 )
    return 0;
  else
    return dynEntDef->health - dynEntClient->health;
}

// local variable allocation has failed, the output may be wrong!
void  DynEnt_SetupConstraints(PhysConstraint *a1@<ebp>, const DynEntityDef *dynEntDef)
{
  int PhysObj; // eax
  int v3; // eax
  int v4; // eax
  _BYTE v5[12]; // [esp+Ch] [ebp-22Ch] BYREF
  phys_vec3 b2refloc; // [esp+18h] [ebp-220h] BYREF
  _BYTE b2axisloc_8[12]; // [esp+30h] [ebp-208h] OVERLAPPED BYREF
  float b2axis[3][3]; // [esp+3Ch] [ebp-1FCh] BYREF
  float poseinvaxis2[3][3]; // [esp+60h] [ebp-1D8h] BYREF
  float poseaxis2[3][3]; // [esp+84h] [ebp-1B4h] BYREF
  phys_vec3 b1refloc; // [esp+A8h] [ebp-190h] BYREF
  _BYTE b1axisloc_8[12]; // [esp+C0h] [ebp-178h] OVERLAPPED BYREF
  float b1axis[3][3]; // [esp+CCh] [ebp-16Ch] BYREF
  float poseinvaxis[3][3]; // [esp+F0h] [ebp-148h] BYREF
  float poseaxis[3][3]; // [esp+114h] [ebp-124h] BYREF
  float caxis[3][3]; // [esp+138h] [ebp-100h] BYREF
  _BYTE v17[8]; // [esp+15Ch] [ebp-DCh] BYREF
  phys_vec3 b2loc; // [esp+168h] [ebp-D0h] BYREF
  float v19; // [esp+188h] [ebp-B0h]
  cg_s *v20; // [esp+18Ch] [ebp-ACh]
  float v21; // [esp+190h] [ebp-A8h]
  float temp[3]; // [esp+194h] [ebp-A4h] BYREF
  cg_s *cgameGlob; // [esp+1A0h] [ebp-98h]
  float z; // [esp+1A4h] [ebp-94h]
  float b2_anchor_loc[3]; // [esp+1A8h] [ebp-90h] BYREF
  float trans2[3]; // [esp+1B4h] [ebp-84h]
  float dyn_ent2_anchor_loc[3]; // [esp+1C0h] [ebp-78h]
  float y; // [esp+1CCh] [ebp-6Ch] BYREF
  int damage2; // [esp+1D0h] [ebp-68h]
  float w; // [esp+1D4h] [ebp-64h]
  float b1_anchor_loc[3]; // [esp+1D8h] [ebp-60h] BYREF
  float trans1[3]; // [esp+1E4h] [ebp-54h]
  float dyn_ent1_anchor_loc[3]; // [esp+1F0h] [ebp-48h]
  const DynEntityPose *ClientPose; // [esp+1FCh] [ebp-3Ch]
  int damage1; // [esp+200h] [ebp-38h]
  const DynEntityPose *dynEnt2Pose; // [esp+204h] [ebp-34h]
  const DynEntityPose *dynEnt1Pose; // [esp+208h] [ebp-30h]
  DynEntityClient *dynEnt2; // [esp+20Ch] [ebp-2Ch]
  DynEntityClient *dynEnt1; // [esp+210h] [ebp-28h]
  const DynEntityDef *dedef2; // [esp+214h] [ebp-24h]
  const DynEntityDef *dedef1; // [esp+218h] [ebp-20h]
  rigid_body *rb2; // [esp+21Ch] [ebp-1Ch]
  rigid_body *rb1; // [esp+220h] [ebp-18h]
  PhysObjUserData *userData2; // [esp+224h] [ebp-14h]
  PhysObjUserData *userData1; // [esp+228h] [ebp-10h]
  PhysConstraint *c; // [esp+22Ch] [ebp-Ch] BYREF
  void *cindex; // [esp+230h] [ebp-8h]
  void *retaddr; // [esp+238h] [ebp+0h]

  c = a1;
  cindex = retaddr;
  for ( userData1 = 0; (int)userData1 < 4; userData1 = (PhysObjUserData *)((char *)userData1 + 1) )
  {
    LOWORD(userData2) = dynEntDef->physConstraints[(unsigned int)userData1];
    if ( (__int16)userData2 != 511 )
    {
      if ( !cm.constraints
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
              889,
              0,
              "%s",
              "cm.constraints") )
      {
        __debugbreak();
      }
      rb1 = (rigid_body *)&cm.constraints[(__int16)userData2];
      if ( !LODWORD(rb1->m_t_vel.x) )
      {
        rb2 = 0;
        dedef1 = 0;
        dedef2 = (const DynEntityDef *)phys_sys::get_environment_rigid_body();
        dynEnt1 = (DynEntityClient *)phys_sys::get_environment_rigid_body();
        dynEnt2 = 0;
        dynEnt1Pose = 0;
        dynEnt2Pose = 0;
        damage1 = 0;
        ClientPose = 0;
        dyn_ent1_anchor_loc[2] = 0.0;
        if ( LODWORD(rb1->m_last_position.z) == 1 )
        {
          dynEnt2 = (DynEntityClient *)DynEnt_GetEntityDef(LOWORD(rb1->m_last_position.w));
          dynEnt2Pose = (const DynEntityPose *)DynEnt_GetClientEntity(LOWORD(rb1->m_last_position.w));
          ClientPose = DynEnt_GetClientPose(LOWORD(rb1->m_last_position.w));
          if ( !LODWORD(dynEnt2Pose->pose.quat[0]) )
          {
            LODWORD(rb1->m_t_vel.x) = 1;
            PhysObj = DynEntCl_CreatePhysObj(
                        (const DynEntityDef *)dynEnt2,
                        (DynEntityClient *)dynEnt2Pose,
                        &ClientPose->pose);
            LODWORD(dynEnt2Pose->pose.quat[0]) = PhysObj;
          }
          if ( !LODWORD(dynEnt2Pose->pose.quat[0]) )
            continue;
          rb2 = (rigid_body *)LODWORD(dynEnt2Pose->pose.quat[0]);
          dedef2 = (const DynEntityDef *)LODWORD(rb2->m_last_position.x);
          LODWORD(dyn_ent1_anchor_loc[1]) = DynEnt_GetDamage(
                                              (DynEntityClient *)dynEnt2Pose,
                                              (const DynEntityDef *)dynEnt2);
          if ( LODWORD(rb1->m_mat.w.x) && SLODWORD(rb1->m_mat.w.x) < SLODWORD(dyn_ent1_anchor_loc[1]) )
          {
            if ( LODWORD(rb1->m_last_position.y) == 8 && Rope_IsValid(LODWORD(rb1->m_t_vel.y)) )
              Rope_RemoveAnchor(LODWORD(rb1->m_t_vel.y), 0);
            continue;
          }
          LODWORD(dyn_ent1_anchor_loc[0]) = &rb1->m_mat.x.y;
          trans1[0] = rb1->m_mat.x.y;
          trans1[1] = rb1->m_mat.x.z;
          trans1[2] = rb1->m_mat.x.w;
          Phys_NitrousVecToVec3(&rb2->m_mat.z, b1_anchor_loc);
          y = trans1[0] + b1_anchor_loc[0];
          *(float *)&damage2 = trans1[1] + b1_anchor_loc[1];
          w = trans1[2] + b1_anchor_loc[2];
        }
        else
        {
          LODWORD(dyn_ent2_anchor_loc[2]) = &rb1->m_mat.x.y;
          y = rb1->m_mat.x.y;
          damage2 = SLODWORD(rb1->m_mat.x.z);
          w = rb1->m_mat.x.w;
        }
        if ( LODWORD(rb1->m_moved_vec.z) != 1 )
        {
          LODWORD(temp[1]) = &rb1->m_mat.y;
          temp[2] = rb1->m_mat.y.x;
          cgameGlob = (cg_s *)LODWORD(rb1->m_mat.y.y);
          z = rb1->m_mat.y.z;
LABEL_33:
          v4 = jpeg_mem_init();
          LODWORD(temp[0]) = CG_GetLocalClientGlobals(v4);
          if ( dynEnt2Pose )
            dynEnt2Pose->pose.origin[0] = *(float *)(LODWORD(temp[0]) + 263304);
          if ( damage1 )
            *(unsigned int *)(damage1 + 16) = *(unsigned int *)(LODWORD(temp[0]) + 263304);
          if ( dedef2 == (const DynEntityDef *)phys_sys::get_environment_rigid_body() )
          {
            dedef2 = (const DynEntityDef *)dynEnt1;
            dynEnt1 = (DynEntityClient *)phys_sys::get_environment_rigid_body();
            v19 = temp[2];
            v20 = cgameGlob;
            v21 = z;
            temp[2] = y;
            cgameGlob = (cg_s *)damage2;
            z = w;
            y = v19;
            damage2 = (int)v20;
            w = v21;
          }
          dedef2[3].pose.origin[0] = 0.0;
          if ( (*(unsigned int *)&dynEnt2[3].burnTime & 2) == 0
            && (LODWORD(rb1->m_last_position.y) == 1 || LODWORD(rb1->m_last_position.y) == 3) )
          {
            Phys_ObjRemoveCollFlags(LODWORD(dynEnt2Pose->pose.quat[0]), 450);
          }
          Phys_Vec3ToNitrousVec(&y, (phys_vec3 *)&b2loc.y);
          Phys_Vec3ToNitrousVec(&temp[2], (phys_vec3 *)v17);
          switch ( LODWORD(rb1->m_last_position.y) )
          {
            case 1:
              LODWORD(caxis[2][2]) = phys_sys::create_rbc_point(
                                       (environment_rigid_body *)dedef2,
                                       (environment_rigid_body *)dynEnt1,
                                       0);
              if ( LODWORD(caxis[2][2]) )
              {
                rigid_body_constraint_point::set(
                  (rigid_body_constraint_point *)LODWORD(caxis[2][2]),
                  (phys_vec3 *)&b2loc.y,
                  (const phys_vec3 *)v17);
                rb1->m_t_vel.x = caxis[2][2];
              }
              else
              {
                rb1->m_t_vel.x = 0.0;
              }
              break;
            case 2:
            case 8:
              LODWORD(caxis[2][1]) = phys_sys::create_rbc_dist(
                                       (environment_rigid_body *)dedef2,
                                       (environment_rigid_body *)dynEnt1,
                                       0);
              if ( LODWORD(caxis[2][1]) )
              {
                rigid_body_constraint_distance::set(
                  (rigid_body_constraint_distance *)LODWORD(caxis[2][1]),
                  (phys_vec3 *)&b2loc.y,
                  (const phys_vec3 *)v17,
                  0.0,
                  rb1->m_mat.w.z);
                rb1->m_t_vel.x = caxis[2][1];
              }
              else
              {
                rb1->m_t_vel.x = 0.0;
              }
              break;
            case 3:
              Phys_ObjRemoveCollFlags(LODWORD(dynEnt2Pose->pose.quat[0]), 1);
              LODWORD(caxis[2][0]) = phys_sys::create_rbc_hinge(
                                       (environment_rigid_body *)dedef2,
                                       (environment_rigid_body *)dynEnt1,
                                       0);
              if ( LODWORD(caxis[2][0]) )
              {
                AnglesToAxis(&rb1->m_mat.y.w, (float (*)[3])poseaxis[2]);
                UnitQuatToAxis(dynEntDef->pose.quat, (float (*)[3])poseinvaxis[2]);
                MatrixTranspose((const float (*)[3])poseinvaxis[2], (float (*)[3])b1axis[2]);
                MatrixMultiply(
                  (const float (*)[3])poseaxis[2],
                  (const float (*)[3])b1axis[2],
                  (float (*)[3])b1axisloc_8);
                Phys_Vec3ToNitrousVec(b1axis[1], (phys_vec3 *)&b1refloc.y);
                Phys_Vec3ToNitrousVec((float *)b1axisloc_8, (phys_vec3 *)poseaxis2[2]);
                if ( dynEnt1Pose )
                {
                  UnitQuatToAxis(&dynEnt1Pose->pose.quat[1], (float (*)[3])poseinvaxis2[2]);
                  MatrixTranspose((const float (*)[3])poseinvaxis2[2], (float (*)[3])b2axis[2]);
                  MatrixMultiply(
                    (const float (*)[3])poseaxis[2],
                    (const float (*)[3])b2axis[2],
                    (float (*)[3])b2axisloc_8);
                  Phys_Vec3ToNitrousVec(b2axis[1], (phys_vec3 *)&b2refloc.y);
                  Phys_Vec3ToNitrousVec((float *)b2axisloc_8, (phys_vec3 *)v5);
                }
                else
                {
                  Phys_Vec3ToNitrousVec(caxis[1], (phys_vec3 *)&b2refloc.y);
                  Phys_Vec3ToNitrousVec(poseaxis[2], (phys_vec3 *)v5);
                }
                rigid_body_constraint_hinge::set(
                  (rigid_body_constraint_hinge *)LODWORD(caxis[2][0]),
                  (int)&c,
                  (phys_vec3 *)&b2loc.y,
                  (const phys_vec3 *)v17,
                  (phys_vec3 *)&b1refloc.y,
                  (phys_vec3 *)&b2refloc.y,
                  (const phys_vec3 *)poseaxis2[2],
                  (const phys_vec3 *)v5,
                  rb1->m_gravity_acc_vec.y * 0.017453292,
                  rb1->m_gravity_acc_vec.z * 0.017453292,
                  rb1->m_mat.w.w);
              }
              else
              {
                rb1->m_t_vel.x = 0.0;
              }
              break;
          }
          continue;
        }
        dynEnt1Pose = (const DynEntityPose *)DynEnt_GetEntityDef(LOWORD(rb1->m_moved_vec.w));
        damage1 = (int)DynEnt_GetClientEntity(LOWORD(rb1->m_moved_vec.w));
        LODWORD(dyn_ent1_anchor_loc[2]) = DynEnt_GetClientPose(LOWORD(rb1->m_moved_vec.w));
        if ( !*(unsigned int *)damage1 )
        {
          LODWORD(rb1->m_t_vel.x) = 1;
          v3 = DynEntCl_CreatePhysObj(
                 (const DynEntityDef *)dynEnt1Pose,
                 (DynEntityClient *)damage1,
                 (const GfxPlacement *)LODWORD(dyn_ent1_anchor_loc[2]));
          *(unsigned int *)damage1 = v3;
        }
        if ( *(unsigned int *)damage1 )
        {
          dedef1 = *(const DynEntityDef **)damage1;
          dynEnt1 = (DynEntityClient *)dedef1->type;
          LODWORD(dyn_ent2_anchor_loc[1]) = DynEnt_GetDamage(
                                              (DynEntityClient *)damage1,
                                              (const DynEntityDef *)dynEnt1Pose);
          if ( LODWORD(rb1->m_mat.w.x) && SLODWORD(rb1->m_mat.w.x) < SLODWORD(dyn_ent2_anchor_loc[1]) )
          {
            if ( LODWORD(rb1->m_last_position.y) == 8 && Rope_IsValid(LODWORD(rb1->m_t_vel.y)) )
              Rope_RemoveAnchor(LODWORD(rb1->m_t_vel.y), 0);
            continue;
          }
          LODWORD(dyn_ent2_anchor_loc[0]) = &rb1->m_mat.y;
          trans2[0] = rb1->m_mat.y.x;
          trans2[1] = rb1->m_mat.y.y;
          trans2[2] = rb1->m_mat.y.z;
          Phys_NitrousVecToVec3((const phys_vec3 *)&dedef1->targetname, b2_anchor_loc);
          temp[2] = trans2[0] + b2_anchor_loc[0];
          *(float *)&cgameGlob = trans2[1] + b2_anchor_loc[1];
          z = trans2[2] + b2_anchor_loc[2];
          goto LABEL_33;
        }
      }
    }
  }
}

int __cdecl DynEnt_GetSurfaceType(const DynEntityDef *dynEntDef)
{
  int surfaceFlags; // [esp+0h] [ebp-8h]

  surfaceFlags = 0;
  if ( dynEntDef->xModel )
  {
    if ( dynEntDef->xModel->collSurfs )
      surfaceFlags = dynEntDef->xModel->collSurfs->surfFlags;
  }
  else if ( dynEntDef->brushModel < cm.numSubModels )
  {
    surfaceFlags = CM_ModelSurfaceFlags(dynEntDef->brushModel);
  }
  return (unsigned __int8)((int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20] & surfaceFlags) >> 20);
}

void __cdecl DynEnt_CalcPhysPreset(const DynEntityDef *dynEntDef)
{
  PhysPreset *physPreset; // [esp+0h] [ebp-2Ch]
  float mins[3]; // [esp+4h] [ebp-28h] BYREF
  float maxs[3]; // [esp+10h] [ebp-1Ch] BYREF
  float dims[3]; // [esp+1Ch] [ebp-10h] BYREF
  int surfaceType; // [esp+28h] [ebp-4h]

  physPreset = dynEntDef->physPreset;
  DynEnt_GetLocalBounds(dynEntDef, mins, maxs);
  dims[0] = maxs[0] - mins[0];
  dims[1] = maxs[1] - mins[1];
  dims[2] = maxs[2] - mins[2];
  surfaceType = DynEnt_GetSurfaceType(dynEntDef);
  Phys_CalcPreset(physPreset, dims, surfaceType);
}

int __cdecl DynEntCl_CreatePhysObj(
        const DynEntityDef *dynEntDef,
        DynEntityClient *dynEntClient,
        const GfxPlacement *pose)
{
  broad_phase_info *i; // [esp+Ch] [ebp-30h]
  broad_phase_info *bpi; // [esp+14h] [ebp-28h]
  unsigned __int16 absId; // [esp+20h] [ebp-1Ch]
  create_gjk_geom_collision_visitor collision_visitor; // [esp+24h] [ebp-18h] BYREF
  int physId; // [esp+2Ch] [ebp-10h]
  gjk_geom_list_t gjk_geom_list; // [esp+30h] [ebp-Ch] BYREF
  PhysObjUserData *phys_user_data; // [esp+38h] [ebp-4h]
  int savedregs; // [esp+3Ch] [ebp+0h] BYREF

  if ( !dynEntDef
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          1128,
          0,
          "%s",
          "dynEntDef") )
  {
    __debugbreak();
  }
  if ( !dynEntClient
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          1129,
          0,
          "%s",
          "dynEntClient") )
  {
    __debugbreak();
  }
  if ( !DynEnt_GetEntityProps(dynEntDef->type)->usePhysics
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          1130,
          0,
          "%s",
          "DynEnt_GetEntityProps( dynEntDef->type )->usePhysics") )
  {
    __debugbreak();
  }
  if ( !dynEntDef->physPreset
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          1131,
          0,
          "%s",
          "dynEntDef->physPreset") )
  {
    __debugbreak();
  }
  gjk_geom_list.m_first_geom = 0;
  gjk_geom_list.m_geom_count = 0;
  collision_visitor.__vftable = (create_gjk_geom_collision_visitor_vtbl *)&create_gjk_geom_collision_visitor::`vftable';
  collision_visitor.gjk_geom_list = &gjk_geom_list;
  create_gjk_geom(dynEntDef, &collision_visitor, (unsigned int)&cls.recentServers[7546].city[57]);
  physId = (int)Phys_ObjCreate(0, pose->origin, pose->quat, vec3_origin, dynEntDef->physPreset, &gjk_geom_list, 1, -1);
  dynEntClient->physObjId = physId;
  if ( physId )
  {
    phys_user_data = (PhysObjUserData *)dynEntClient->physObjId;
    if ( (phys_user_data->m_bpb->m_flags & 1) != 0 )
    {
      bpi = broad_phase_base::get_bpi(phys_user_data->m_bpb);
      bpi->m_my_collision_type_flags |= 4u;
    }
    else
    {
      for ( i = broad_phase_base::get_bpg(phys_user_data->m_bpb)->m_list_bpi_head;
            i;
            i = (broad_phase_info *)i->m_list_bpb_next )
      {
        i->m_my_collision_type_flags |= 4u;
      }
    }
    DynEnt_SetupConstraints((PhysConstraint *)&savedregs, dynEntDef);
    absId = DynEnt_GetAbsId(dynEntDef);
    CScr_NotifyNum(0, absId, 4u, cscr_const.touch, 0);
    return physId;
  }
  else
  {
    Com_PrintWarning(1, "DynEntCl_CreatePhysObj: Unable to create physics object.\n");
    return 0;
  }
}

broad_phase_info *__thiscall broad_phase_base::get_bpi(broad_phase_base *this)
{
  if ( (this->m_flags & 1) == 0
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_broad_phase_base.h",
         99,
         "is_bpi()",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  return (broad_phase_info *)this;
}

broad_phase_group *__thiscall broad_phase_base::get_bpg(broad_phase_base *this)
{
  if ( (this->m_flags & 2) == 0
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_broad_phase_base.h",
         100,
         "is_bpg()",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  return (broad_phase_group *)this;
}

void *__thiscall create_gjk_geom_collision_visitor::allocate(
        create_gjk_geom_collision_visitor *this,
        int size,
        int alignment,
        bool no_error)
{
  if ( !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h", 1140, 0, "%s", "0") )
    __debugbreak();
  return 0;
}

char __thiscall create_gjk_geom_collision_visitor::query_create_prolog(gjk_collision_visitor *this, const void *geom)
{
  return 1;
}

void __thiscall gjk_geom_list_t::add_geom(gjk_geom_list_t *this, gjk_base_t *geom)
{
  if ( !geom && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h", 1029, 0, "%s", "geom") )
    __debugbreak();
  geom->m_next_geom = this->m_first_geom;
  this->m_first_geom = geom;
  ++this->m_geom_count;
}

void __thiscall create_gjk_geom_collision_visitor::query_create_epilog_1(
        create_gjk_geom_collision_visitor *this,
        gjk_base_t *gjk_geom)
{
  gjk_geom_list_t::add_geom(this->gjk_geom_list, gjk_geom);
}

unsigned __int16 __cdecl DynEntCl_CreateEntityModel(
        XModel *model,
        const float *quat,
        const float *origin,
        const float *hitPos,
        const float *hitDir,
        int mod,
        PhysPreset *preset)
{
  PhysPreset *physPreset; // [esp+8h] [ebp-2Ch]
  int i; // [esp+14h] [ebp-20h]
  DynEntityPose *dynEntPose; // [esp+18h] [ebp-1Ch]
  const DynEntityProps *dynEntProps; // [esp+1Ch] [ebp-18h]
  DynEntityClient *dynEntClient; // [esp+24h] [ebp-10h]
  DynEntityDef *dynEntDef; // [esp+28h] [ebp-Ch]
  unsigned __int16 dynEntId; // [esp+2Ch] [ebp-8h]
  DynEntityColl *dynEntColl; // [esp+30h] [ebp-4h]

  if ( (!model || !model->physPreset && !preset)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          1194,
          0,
          "%s",
          "model && ( model->physPreset || preset )") )
  {
    __debugbreak();
  }
  dynEntId = DynEntCl_AddEntityModel();
  if ( dynEntId == 0xFFFF )
    return -1;
  dynEntDef = DynEnt_GetEntityDef(dynEntId, DYNENT_DRAW_MODEL);
  memset((unsigned __int8 *)dynEntDef, 0, sizeof(DynEntityDef));
  dynEntDef->type = DYNENT_TYPE_CLUTTER;
  dynEntDef->pose.origin[0] = *origin;
  dynEntDef->pose.origin[1] = origin[1];
  dynEntDef->pose.origin[2] = origin[2];
  dynEntDef->pose.quat[0] = *quat;
  dynEntDef->pose.quat[1] = quat[1];
  dynEntDef->pose.quat[2] = quat[2];
  dynEntDef->pose.quat[3] = quat[3];
  dynEntDef->xModel = model;
  if ( model->physPreset )
    physPreset = model->physPreset;
  else
    physPreset = preset;
  dynEntDef->physPreset = physPreset;
  dynEntDef->health = 0;
  dynEntDef->contents = model->contents;
  dynEntDef->flags |= 2u;
  for ( i = 0; i < 4; ++i )
    dynEntDef->physConstraints[i] = 511;
  dynEntClient = DynEnt_GetClientEntity(dynEntId, DYNENT_DRAW_MODEL);
  dynEntColl = DynEnt_GetEntityColl(DYNENT_COLL_CLIENT_FIRST, dynEntId);
  dynEntPose = DynEnt_GetClientPose(dynEntId, DYNENT_DRAW_MODEL);
  memcpy(dynEntPose, &dynEntDef->pose, 0x1Cu);
  if ( dynEntClient->physObjId )
    DynEnt_DestroyPhysics(PHYS_WORLD_DYNENT, dynEntDef, dynEntClient);
  dynEntClient->flags = 2;
  dynEntClient->health = dynEntDef->health;
  dynEntClient->fadeTime = 0;
  dynEntProps = DynEnt_GetEntityProps(dynEntDef->type);
  dynEntClient->flags |= 1u;
  dynEntColl->nextEntInSector = 0;
  dynEntColl->sector = 0;
  DynEntCl_LinkModel(dynEntId);
  if ( dynEntProps->usePhysics )
  {
    if ( !dynEntClient->physObjId )
      dynEntClient->physObjId = DynEntCl_CreatePhysObj(dynEntDef, dynEntClient, &dynEntPose->pose);
    Phys_ObjAddCustomForce(dynEntClient->physObjId, hitPos, hitDir, mod, dynEntDef->physPreset);
  }
  return dynEntId;
}

unsigned __int16 __cdecl DynEntCl_AddEntityModel()
{
  int extraDynEntLimit; // [esp+0h] [ebp-8h]
  int id; // [esp+4h] [ebp-4h]
  int ida; // [esp+4h] [ebp-4h]

  extraDynEntLimit = dynEnt_spawnedLimit->current.integer;
  if ( CG_IsShowingZombieMap() && !zombietron->current.enabled )
    extraDynEntLimit /= 2;
  if ( numExtraDynEnts > extraDynEntLimit )
  {
    DynEntCl_CleanUpOldModels();
    if ( numExtraDynEnts == 256 )
      return -1;
  }
  for ( id = 0; id < 256; ++id )
  {
    if ( !usedExtraDynEnts[id] )
    {
      usedExtraDynEnts[id] = 1;
      break;
    }
  }
  ida = id + cm.originalDynEntCount;
  ++numExtraDynEnts;
  if ( ida > cm.dynEntCount[0]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          519,
          0,
          "%s",
          "id <= cm.dynEntCount[DYNENT_COLL_CLIENT_MODEL]") )
  {
    __debugbreak();
  }
  if ( ida == cm.dynEntCount[0] )
    ++cm.dynEntCount[0];
  return ida;
}

void DynEntCl_CleanUpOldModels()
{
  float v0; // [esp+0h] [ebp-4Ch]
  float v1; // [esp+4h] [ebp-48h]
  float *v2; // [esp+14h] [ebp-38h]
  int i; // [esp+1Ch] [ebp-30h]
  const DynEntityPose *dynEntPose; // [esp+20h] [ebp-2Ch]
  float distSq; // [esp+24h] [ebp-28h]
  unsigned __int16 id; // [esp+28h] [ebp-24h]
  cg_s *cgameGlob; // [esp+2Ch] [ebp-20h]
  int localClientNum; // [esp+30h] [ebp-1Ch]
  float playerPositions[3]; // [esp+34h] [ebp-18h] BYREF
  float furthestDistSq; // [esp+40h] [ebp-Ch]
  int numPlayers; // [esp+44h] [ebp-8h]
  unsigned __int16 furthestId; // [esp+48h] [ebp-4h]

  numPlayers = 0;
  for ( localClientNum = 0; localClientNum < 1; ++localClientNum )
  {
    if ( CL_LocalClient_IsActive(localClientNum) )
    {
      cgameGlob = CG_GetLocalClientGlobals(localClientNum);
      v2 = &playerPositions[3 * numPlayers];
      *v2 = cgameGlob->refdef.vieworg[0];
      v2[1] = cgameGlob->refdef.vieworg[1];
      v2[2] = cgameGlob->refdef.vieworg[2];
      ++numPlayers;
    }
  }
  furthestDistSq = *(float *)&FLOAT_0_0;
  furthestId = -1;
  for ( id = cm.originalDynEntCount; id < (int)cm.dynEntCount[0]; ++id )
  {
    dynEntPose = DynEnt_GetClientPose(id, DYNENT_DRAW_MODEL);
    distSq = FLOAT_3_4028235e38;
    for ( i = 0; i < numPlayers; ++i )
    {
      v1 = Vec3DistanceSq(dynEntPose->pose.origin, &playerPositions[3 * i]);
      if ( (float)(distSq - v1) < 0.0 )
        v0 = distSq;
      else
        v0 = v1;
      distSq = v0;
    }
    if ( distSq > furthestDistSq )
    {
      furthestDistSq = distSq;
      furthestId = id;
    }
  }
  if ( furthestId != 0xFFFF )
    DynEntCl_DestroyEntityModel(furthestId);
}

void __cdecl DynEntCl_PointTrace(const pointtrace_t *clip, trace_t *results)
{
  _QWORD start[2]; // [esp+28h] [ebp-20h] BYREF
  _QWORD end[2]; // [esp+38h] [ebp-10h] BYREF

  //PIXBeginNamedEvent(-1, "dyn_pointtrace");
  if ( !clip
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp", 1380, 0, "%s", "clip") )
  {
    __debugbreak();
  }
  if ( !results
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp", 1381, 0, "%s", "results") )
  {
    __debugbreak();
  }
  if ( results->fraction > 1.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          1382,
          0,
          "%s\n\t(results->fraction) = %g",
          "(results->fraction <= 1.0f)",
          results->fraction) )
  {
    __debugbreak();
  }
  if ( results->fraction != 0.0 )
  {
    start[0] = *(_QWORD *)clip->extents.start.vec.v;
    LODWORD(start[1]) = clip->extents.start.vec.u[2];
    end[0] = *(_QWORD *)clip->extents.end.vec.v;
    LODWORD(end[1]) = clip->extents.end.vec.u[2];
    HIDWORD(start[1]) = *(unsigned int *)&FLOAT_0_0;
    HIDWORD(end[1]) = LODWORD(results->fraction);
    DynEntCl_PointTrace_r(DYNENT_COLL_CLIENT_BRUSH, clip, 1u, (float *)start, (float *)end, results);
    if ( results->fraction == 0.0 )
    {
      if ( GetCurrentThreadId() != g_DXDeviceThread )
        return;
    }
    else
    {
      DynEntCl_PointTrace_r(DYNENT_COLL_CLIENT_FIRST, clip, 1u, (float *)start, (float *)end, results);
      if ( g_DXDeviceThread != GetCurrentThreadId() )
        return;
    }
    D3DPERF_EndEvent();
    return;
  }
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

void __cdecl DynEntCl_PointTrace_r(
        DynEntityCollType drawType,
        const pointtrace_t *clip,
        unsigned int sectorIndex,
        float *p1,
        float *p2,
        trace_t *results)
{
  float v6; // [esp+10h] [ebp-50h]
  DynEntityPose *dynEntPose; // [esp+14h] [ebp-4Ch]
  DynEntityCollSector *sector; // [esp+1Ch] [ebp-44h]
  unsigned __int16 listIndex; // [esp+20h] [ebp-40h]
  float t1; // [esp+24h] [ebp-3Ch]
  float frac; // [esp+28h] [ebp-38h]
  DynEntityClient *dynEntClient; // [esp+30h] [ebp-30h]
  const DynEntityDef *dynEntDef; // [esp+34h] [ebp-2Ch]
  float t2; // [esp+38h] [ebp-28h]
  DynEntityColl *dynEntColl; // [esp+3Ch] [ebp-24h]
  float p[4]; // [esp+40h] [ebp-20h] BYREF
  float mid[4]; // [esp+50h] [ebp-10h] BYREF

  if ( !clip
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp", 1288, 0, "%s", "clip") )
  {
    __debugbreak();
  }
  if ( !p1
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp", 1289, 0, "%s", "p1") )
  {
    __debugbreak();
  }
  if ( !p2
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp", 1290, 0, "%s", "p2") )
  {
    __debugbreak();
  }
  if ( !results
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp", 1291, 0, "%s", "results") )
  {
    __debugbreak();
  }
  if ( results->fraction > 1.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          1292,
          0,
          "%s\n\t(results->fraction) = %g",
          "(results->fraction <= 1.0f)",
          results->fraction) )
  {
    __debugbreak();
  }
  p[0] = *p1;
  p[1] = p1[1];
  p[2] = p1[2];
  p[3] = p1[3];
  while ( sectorIndex )
  {
    sector = DynEnt_GetCollSector(drawType, sectorIndex);
    if ( (clip->contentmask & sector->contents) == 0 )
      break;
    for ( listIndex = sector->entListHead; listIndex; listIndex = dynEntColl->nextEntInSector )
    {
      dynEntColl = DynEnt_GetEntityColl(drawType, listIndex - 1);
      dynEntClient = DynEnt_GetClientEntity(listIndex - 1, (DynEntityDrawType)drawType);
      if ( (dynEntClient->flags & 2) != 0 && (dynEntClient->flags & 1) != 0 )
      {
        dynEntDef = DynEnt_GetEntityDef(listIndex - 1, (DynEntityDrawType)drawType);
        if ( (clip->contentmask & DynEnt_GetContents(dynEntDef)) != 0 )
        {
          dynEntPose = DynEnt_GetClientPose(listIndex - 1, (DynEntityDrawType)drawType);
          if ( intersect_extents_sphere(&clip->extents, dynEntPose->pose.origin, dynEntPose->radius, results->fraction) )
          {
            if ( drawType )
            {
              if ( drawType != DYNENT_COLL_CLIENT_BRUSH
                && !Assert_MyHandler(
                      "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
                      1331,
                      0,
                      "%s\n\t(drawType) = %i",
                      "(drawType == DYNENT_DRAW_BRUSH)",
                      drawType) )
              {
                __debugbreak();
              }
              DynEnt_PointTraceToBrush(dynEntDef, &dynEntPose->pose, clip, results);
            }
            else
            {
              DynEnt_PointTraceToModel(dynEntDef, &dynEntPose->pose, clip, results);
            }
            if ( results->fraction == 0.0 )
              return;
          }
        }
      }
    }
    t1 = p[sector->tree.axis] - sector->tree.dist;
    t2 = p2[sector->tree.axis] - sector->tree.dist;
    if ( (float)(t1 * t2) < 0.0 )
    {
      if ( p[3] >= results->fraction )
        return;
      frac = t1 / (float)(t1 - t2);
      if ( frac < 0.0
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
              1352,
              0,
              "%s",
              "frac >= 0.0f") )
      {
        __debugbreak();
      }
      if ( frac > 1.0
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
              1353,
              0,
              "%s",
              "frac <= 1.0f") )
      {
        __debugbreak();
      }
      mid[0] = (float)((float)(*p2 - p[0]) * frac) + p[0];
      mid[1] = (float)((float)(p2[1] - p[1]) * frac) + p[1];
      mid[2] = (float)((float)(p2[2] - p[2]) * frac) + p[2];
      mid[3] = (float)((float)(p2[3] - p[3]) * frac) + p[3];
      DynEntCl_PointTrace_r((DynEntityDrawType)drawType, clip, sector->tree.child[t2 >= 0.0], p, mid, results);
      if ( results->fraction == 0.0 )
        return;
      sectorIndex = sector->tree.child[t2 < 0.0];
      p[0] = mid[0];
      p[1] = mid[1];
      p[2] = mid[2];
      p[3] = mid[3];
    }
    else
    {
      if ( (float)(t2 - t1) < 0.0 )
        v6 = p2[sector->tree.axis] - sector->tree.dist;
      else
        v6 = p[sector->tree.axis] - sector->tree.dist;
      sectorIndex = sector->tree.child[v6 < 0.0];
    }
  }
}

void __cdecl DynEntCl_AreaEntities_r(
        DynEntityCollType drawType,
        unsigned int sectorIndex,
        DynEntityAreaParms *areaParms)
{
  DynEntityCollSector *sector; // [esp+0h] [ebp-1Ch]
  unsigned __int16 listIndex; // [esp+4h] [ebp-18h]
  DynEntityClient *dynEntClient; // [esp+Ch] [ebp-10h]
  const DynEntityDef *dynEntDef; // [esp+10h] [ebp-Ch]
  DynEntityColl *dynEntColl; // [esp+14h] [ebp-8h]
  unsigned int nextSectorIndex; // [esp+18h] [ebp-4h]

  if ( !areaParms
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          1576,
          0,
          "%s",
          "areaParms") )
  {
    __debugbreak();
  }
  while ( sectorIndex )
  {
    sector = DynEnt_GetCollSector(drawType, sectorIndex);
    if ( (areaParms->contentMask & sector->contents) == 0 )
      break;
    for ( listIndex = sector->entListHead; listIndex; listIndex = dynEntColl->nextEntInSector )
    {
      dynEntColl = DynEnt_GetEntityColl(drawType, listIndex - 1);
      dynEntClient = DynEnt_GetClientEntity(listIndex - 1, (DynEntityDrawType)drawType);
      if ( (dynEntClient->flags & 2) != 0 && (dynEntClient->flags & 1) != 0 )
      {
        dynEntDef = DynEnt_GetEntityDef(listIndex - 1, (DynEntityDrawType)drawType);
        if ( DynEnt_EntityInArea(dynEntDef, dynEntColl, areaParms->mins, areaParms->maxs, areaParms->contentMask) )
        {
          if ( areaParms->count == areaParms->maxCount )
            return;
          areaParms->list[areaParms->count++] = listIndex - 1;
        }
      }
    }
    if ( areaParms->maxs[sector->tree.axis] <= sector->tree.dist )
    {
      if ( areaParms->mins[sector->tree.axis] >= sector->tree.dist )
        return;
      sectorIndex = sector->tree.child[1];
    }
    else if ( sector->tree.dist <= areaParms->mins[sector->tree.axis] )
    {
      sectorIndex = sector->tree.child[0];
    }
    else
    {
      nextSectorIndex = sector->tree.child[1];
      DynEntCl_AreaEntities_r((DynEntityDrawType)drawType, sector->tree.child[0], areaParms);
      sectorIndex = nextSectorIndex;
    }
  }
}

unsigned __int16 __cdecl DynEntCl_AreaEntities(
        DynEntityDrawType drawType,
        const float *mins,
        const float *maxs,
        int contentMask,
        unsigned __int16 dynEntMaxCount,
        unsigned __int16 *dynEntList)
{
  DynEntityAreaParms areaParms; // [esp+0h] [ebp-14h] BYREF

  if ( !mins
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp", 1641, 0, "%s", "mins") )
  {
    __debugbreak();
  }
  if ( !maxs
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp", 1642, 0, "%s", "maxs") )
  {
    __debugbreak();
  }
  if ( !dynEntMaxCount
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          1643,
          0,
          "%s\n\t(dynEntMaxCount) = %i",
          "(dynEntMaxCount > 0)",
          0) )
  {
    __debugbreak();
  }
  if ( !dynEntList
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          1644,
          0,
          "%s",
          "dynEntList") )
  {
    __debugbreak();
  }
  areaParms.mins = mins;
  areaParms.maxs = maxs;
  areaParms.contentMask = contentMask;
  areaParms.list = dynEntList;
  areaParms.maxCount = dynEntMaxCount;
  areaParms.count = 0;
  DynEntCl_AreaEntities_r((DynEntityCollType)drawType, 1u, &areaParms);
  return areaParms.count;
}

void __cdecl DynEntCl_TurnOffLight(const DynEntityDef *dynEntDef)
{
  centity_s *light; // [esp+8h] [ebp-18h]
  int entnum; // [esp+Ch] [ebp-14h]
  int clientindex; // [esp+10h] [ebp-10h]
  PhysConstraint *c; // [esp+14h] [ebp-Ch]
  __int16 cindex; // [esp+18h] [ebp-8h]
  int i; // [esp+1Ch] [ebp-4h]

  for ( i = 0; i < 4; ++i )
  {
    cindex = dynEntDef->physConstraints[i];
    if ( cindex != 511 )
    {
      c = &cm.constraints[cindex];
      if ( c->type == CONSTRAINT_LIGHT )
      {
        for ( clientindex = 0; clientindex < 1; ++clientindex )
        {
          entnum = c->centity_num[clientindex];
          if ( entnum != -1 && CL_LocalClient_IsActive(clientindex) )
          {
            light = CG_GetEntity(clientindex, entnum);
            if ( light )
              light->nextState.lerp.u.loopFx.period = *(unsigned int *)&FLOAT_0_0;
          }
        }
      }
    }
  }
}

void __cdecl DynEntCl_Damage(
        int localClientNum,
        unsigned __int16 dynEntId,
        DynEntityCollType drawType,
        const float *hitPos,
        const float *hitDir,
        int damage)
{
  DynEntityPose *dynEntPose; // [esp+8h] [ebp-38h]
  DynEntityPose *dynEntPosea; // [esp+8h] [ebp-38h]
  DynEntityPose *dynEntPoseb; // [esp+8h] [ebp-38h]
  DynEntityPose *dynEntPosec; // [esp+8h] [ebp-38h]
  DynEntityClient *dynEntClient; // [esp+10h] [ebp-30h]
  const DynEntityDef *dynEntDef; // [esp+14h] [ebp-2Ch]
  unsigned __int16 absId; // [esp+18h] [ebp-28h]
  float axis[3][3]; // [esp+1Ch] [ebp-24h] BYREF

  if ( dynEntId == 0xFFFF
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          1806,
          0,
          "%s",
          "dynEntId != DYNENT_INVALID_ID") )
  {
    __debugbreak();
  }
  if ( !hitPos
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp", 1807, 0, "%s", "hitPos") )
  {
    __debugbreak();
  }
  if ( !hitDir
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp", 1808, 0, "%s", "hitDir") )
  {
    __debugbreak();
  }
  if ( damage <= 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          1809,
          0,
          "%s\n\t(damage) = %i",
          "((damage > 0))",
          damage) )
  {
    __debugbreak();
  }
  if ( localClientNum == jpeg_mem_init() )
  {
    CG_GetLocalClientGlobals(localClientNum);
    dynEntDef = DynEnt_GetEntityDef(dynEntId, (DynEntityDrawType)drawType);
    dynEntClient = DynEnt_GetClientEntity(dynEntId, (DynEntityDrawType)drawType);
    if ( (dynEntClient->flags & 3) == 0
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
            1820,
            0,
            "%s",
            "dynEntClient->flags & (DYNENT_CL_ACTIVE | DYNENT_CL_VISIBLE)") )
    {
      __debugbreak();
    }
    dynEntClient->health -= damage;
    if ( DynEnt_GetEntityProps(dynEntDef->type)->destroyable )
    {
      if ( dynEntClient->health > 0 )
        return;
      dynEntClient->flags &= 0xFFFCu;
      if ( dynEntClient->physObjId )
      {
        Phys_ObjDestroy(0, dynEntClient->physObjId);
        dynEntClient->physObjId = 0;
      }
      DynEntCl_UnlinkEntity(dynEntId, drawType);
      if ( dynEntDef->destroyFx || dynEntDef->destroyPieces )
      {
        dynEntPose = DynEnt_GetClientPose(dynEntId, (DynEntityDrawType)drawType);
        UnitQuatToAxis(dynEntPose->pose.quat, axis);
        if ( dynEntDef->destroyFx )
          DynEntCl_PlayEventFx(dynEntDef->destroyFx, dynEntPose->pose.origin, axis);
        if ( dynEntDef->destroyPieces )
          DynEntPieces_SpawnPieces(
            localClientNum,
            dynEntDef->destroyPieces,
            dynEntPose->pose.origin,
            axis,
            hitPos,
            hitDir);
      }
      if ( dynEntDef->destroySound )
      {
        dynEntPosea = DynEnt_GetClientPose(dynEntId, (DynEntityDrawType)drawType);
        CG_PlaySound(localClientNum, 1022, dynEntPosea->pose.origin, 0, 0, 1.0, dynEntDef->destroySound);
      }
      goto LABEL_36;
    }
    if ( damage + dynEntClient->health > 0 && dynEntClient->health < 0 )
    {
      absId = DynEnt_GetClientAbsId(dynEntId, (DynEntityDrawType)drawType);
      CScr_NotifyNum(localClientNum, absId, 4u, cscr_const.death, 0);
      if ( dynEntDef->destroyFx )
      {
        dynEntPoseb = DynEnt_GetClientPose(dynEntId, (DynEntityDrawType)drawType);
        UnitQuatToAxis(dynEntPoseb->pose.quat, axis);
        DynEntCl_PlayEventFx(dynEntDef->destroyFx, dynEntPoseb->pose.origin, axis);
      }
      if ( dynEntDef->destroySound )
      {
        dynEntPosec = DynEnt_GetClientPose(dynEntId, (DynEntityDrawType)drawType);
        CG_PlaySound(localClientNum, 1022, dynEntPosec->pose.origin, 0, 0, 1.0, dynEntDef->destroySound);
      }
LABEL_36:
      DynEntCl_TurnOffLight(dynEntDef);
    }
  }
}

void __cdecl DynEntCl_PlayEventFx(const FxEffectDef *def, const float *origin, const float (*axis)[3])
{
  cg_s *LocalClientGlobals; // eax
  int localClientNum; // [esp+4h] [ebp-4h]

  for ( localClientNum = 0; localClientNum < 1; ++localClientNum )
  {
    if ( CL_LocalClient_IsActive(localClientNum) )
    {
      LocalClientGlobals = CG_GetLocalClientGlobals(localClientNum);
      FX_PlayOrientedEffect(localClientNum, def, LocalClientGlobals->time, origin, axis);
    }
  }
}

unsigned int __cdecl DynEntCl_PlayBoltedFX(const FxEffectDef *fx, unsigned __int16 absDynEntId)
{
  int localClientNum; // [esp+0h] [ebp-Ch]
  cg_s *cgameGlob; // [esp+4h] [ebp-8h]
  unsigned int retHandle; // [esp+8h] [ebp-4h]

  retHandle = 0;
  for ( localClientNum = 0; localClientNum < 1; ++localClientNum )
  {
    if ( CL_LocalClient_IsActive(localClientNum) )
    {
      cgameGlob = CG_GetLocalClientGlobals(localClientNum);
      if ( retHandle )
        FX_PlayBoltedEffect_DynEnt(localClientNum, fx, cgameGlob->time, absDynEntId);
      else
        retHandle = FX_PlayBoltedEffect_DynEnt(localClientNum, fx, cgameGlob->time, absDynEntId);
    }
  }
  if ( retHandle )
    return retHandle;
  else
    return 0;
}

void __cdecl DynEntCl_RemoveFromFadeList(unsigned __int16 absDynEntId)
{
  int v1; // edx
  int i; // [esp+0h] [ebp-4h]

  for ( i = 0; i < gNumFadingDynEnts; ++i )
  {
    if ( gDynEntFadeData[i].id == absDynEntId )
    {
      --gNumFadingDynEnts;
      v1 = dword_337F00C[2 * gNumFadingDynEnts];
      *(unsigned int *)&gDynEntFadeData[i].id = *(unsigned int *)&gDynEntFadeData[gNumFadingDynEnts].id;
      dword_337F00C[2 * i] = v1;
    }
  }
}

void __cdecl DynEntCl_SetFadeOut(unsigned __int16 dynEntId, DynEntityDrawType drawType)
{
  DynEntityDrawType v2; // [esp+4h] [ebp-20h] BYREF
  unsigned __int16 id; // [esp+8h] [ebp-1Ch] BYREF
  int dynEntTime; // [esp+Ch] [ebp-18h]
  int i; // [esp+10h] [ebp-14h]
  int currentTime; // [esp+14h] [ebp-10h]
  int oldestTime; // [esp+18h] [ebp-Ch]
  int oldestDynEntIndex; // [esp+1Ch] [ebp-8h]
  unsigned __int16 absDynEntId; // [esp+20h] [ebp-4h]

  absDynEntId = DynEnt_GetClientAbsId(dynEntId, drawType);
  if ( gNumFadingDynEnts >= 32 )
  {
    oldestTime = -1000000;
    oldestDynEntIndex = 0;
    currentTime = CG_GetLocalClientGlobals(0)->time;
    for ( i = 0; i < gNumFadingDynEnts; ++i )
    {
      dynEntTime = currentTime - dword_337F00C[2 * i];
      if ( dynEntTime > oldestTime )
      {
        oldestTime = dynEntTime;
        oldestDynEntIndex = i;
      }
    }
    if ( oldestTime <= -1000000
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
            1987,
            0,
            "%s",
            "oldestTime > -1000000") )
    {
      __debugbreak();
    }
    if ( oldestTime > -1000000 )
    {
      DynEnt_GetClientIdDrawType(gDynEntFadeData[oldestDynEntIndex].id, &id, &v2);
      gDynEntFadeData[oldestDynEntIndex].id = absDynEntId;
      dword_337F00C[2 * oldestDynEntIndex] = currentTime;
      DynEntCl_DestroyEntityModel(id);
    }
  }
  else
  {
    gDynEntFadeData[gNumFadingDynEnts].id = absDynEntId;
    dword_337F00C[2 * gNumFadingDynEnts++] = CG_GetLocalClientGlobals(0)->time;
  }
}

void __cdecl DynEnt_UpdateFading(int currentTime)
{
  DynEntityDrawType drawType; // [esp+0h] [ebp-10h] BYREF
  unsigned __int16 id; // [esp+4h] [ebp-Ch] BYREF
  DynEntityClient *dynEntClient; // [esp+8h] [ebp-8h]
  int i; // [esp+Ch] [ebp-4h]

  for ( i = 0; i < gNumFadingDynEnts; ++i )
  {
    dynEntClient = DynEnt_GetClientEntity(gDynEntFadeData[i].id);
    if ( currentTime - dword_337F00C[2 * i] > 25000 )
    {
      dynEntClient->fadeTime = currentTime - LOWORD(dword_337F00C[2 * i]) - 25000;
      if ( dynEntClient->fadeTime > 0x3E8u )
      {
        DynEnt_GetClientIdDrawType(gDynEntFadeData[i].id, &id, &drawType);
        DynEntCl_DestroyEntityModel(id);
      }
    }
  }
}

double __cdecl DynEntCl_GetFadeTime(unsigned __int16 dynEntId, DynEntityDrawType drawType)
{
  float v4; // [esp+4h] [ebp-Ch]
  float v5; // [esp+8h] [ebp-8h]

  v4 = (float)DynEnt_GetClientEntity(dynEntId, drawType)->fadeTime / 1000.0;
  if ( (float)(v4 - 1.0) < 0.0 )
    v5 = v4;
  else
    v5 = FLOAT_1_0;
  if ( (float)(0.0 - v4) < 0.0 )
    return v5;
  else
    return *(float *)&FLOAT_0_0;
}

void __cdecl DynEntCl_SetBurning(unsigned __int16 dynEntId, DynEntityDrawType drawType, bool burning)
{
  unsigned int v3; // ecx
  int i; // [esp+0h] [ebp-28h]
  const DynEntityPose *dynEntPose; // [esp+4h] [ebp-24h]
  const DynEntityDef *dynEntDef; // [esp+8h] [ebp-20h]
  DynEnt_BurnData *burnData; // [esp+10h] [ebp-18h]
  unsigned int fx; // [esp+18h] [ebp-10h]
  DynEntityClient *dynEntClient; // [esp+20h] [ebp-8h]
  unsigned __int16 absDynEntId; // [esp+24h] [ebp-4h]

  dynEntClient = DynEnt_GetClientEntity(dynEntId, drawType);
  absDynEntId = DynEnt_GetClientAbsId(dynEntId, drawType);
  if ( burning )
  {
    if ( (dynEntClient->flags & 8) != 0 )
    {
      if ( !dynEntClient->physObjId && dynEntClient->health < -50 )
      {
        dynEntDef = DynEnt_GetEntityDef(dynEntId, drawType);
        dynEntPose = DynEnt_GetClientPose(dynEntId, drawType);
        dynEntClient->physObjId = DynEntCl_CreatePhysObj(dynEntDef, dynEntClient, &dynEntPose->pose);
      }
    }
    else
    {
      if ( (_S2_5 & 1) == 0 )
      {
        _S2_5 |= 1u;
        fxdefsm = FX_Register("destructibles/fx_dest_fire_trail_sm");
      }
      fx = DynEntCl_PlayBoltedFX(fxdefsm, absDynEntId);
      if ( UniqueHandleToEffect(0, fx) )
      {
        dynEntClient->flags |= 8u;
        if ( gNumBurningDynEnts >= 16 )
        {
          FX_ThroughWithEffect(0, hEffect[0], 1);
          hEffect[0] = fx;
          gDynEntBurnData[0].id = absDynEntId;
        }
        else
        {
          burnData = (DynEnt_BurnData *)(8 * gNumBurningDynEnts + 53997848);
          hEffect[2 * gNumBurningDynEnts] = fx;
          burnData->id = absDynEntId;
          ++gNumBurningDynEnts;
        }
      }
    }
  }
  else
  {
    for ( i = 0; i < gNumBurningDynEnts; ++i )
    {
      if ( gDynEntBurnData[i].id == absDynEntId )
      {
        --gNumBurningDynEnts;
        FX_ThroughWithEffect(0, hEffect[2 * i], 1);
        hEffect[2 * i] = 0;
        v3 = hEffect[2 * gNumBurningDynEnts];
        *(unsigned int *)&gDynEntBurnData[i].id = *(unsigned int *)&gDynEntBurnData[gNumBurningDynEnts].id;
        hEffect[2 * i] = v3;
      }
    }
    dynEntClient->flags &= ~8u;
  }
}

void DynEnt_ResetFading()
{
  gNumFadingDynEnts = 0;
}

void DynEnt_ResetBurning()
{
  gNumBurningDynEnts = 0;
}

void DynEnt_UpdateBurning()
{
  unsigned int v0; // ecx
  unsigned int v1; // eax
  const DynEntityPose *dynEntPose; // [esp+0h] [ebp-1Ch]
  const DynEntityDef *dynEntDef; // [esp+4h] [ebp-18h]
  int playedTime; // [esp+Ch] [ebp-10h]
  int adjustedPlayedTime; // [esp+10h] [ebp-Ch]
  DynEntityClient *dynEntClient; // [esp+14h] [ebp-8h]
  int i; // [esp+18h] [ebp-4h]

  for ( i = 0; i < gNumBurningDynEnts; ++i )
  {
    dynEntClient = DynEnt_GetClientEntity(gDynEntBurnData[i].id);
    if ( UniqueHandleToEffect(0, hEffect[2 * i]) )
    {
      playedTime = FX_GetEffectPlayedTime(0, hEffect[2 * i]);
      adjustedPlayedTime = playedTime - 54321 * gDynEntBurnData[i].id % 5000;
      if ( adjustedPlayedTime > 2000 && adjustedPlayedTime < 2500 && !dynEntClient->physObjId )
      {
        dynEntDef = DynEnt_GetEntityDef(gDynEntBurnData[i].id);
        dynEntPose = DynEnt_GetClientPose(gDynEntBurnData[i].id);
        if ( DynEnt_GetEntityProps(dynEntDef->type)->usePhysics )
          dynEntClient->physObjId = DynEntCl_CreatePhysObj(dynEntDef, dynEntClient, &dynEntPose->pose);
      }
      if ( playedTime > dynEntClient->burnTime )
      {
        dynEntClient->burnTime = playedTime;
        if ( dynEntClient->burnTime > 0x7530u )
          dynEntClient->burnTime = 30000;
      }
      if ( playedTime > 5000 && (gNumBurningDynEnts > 4 || playedTime > 20000) )
      {
        --gNumBurningDynEnts;
        FX_ThroughWithEffect(0, hEffect[2 * i], 1);
        v0 = hEffect[2 * gNumBurningDynEnts];
        *(unsigned int *)&gDynEntBurnData[i].id = *(unsigned int *)&gDynEntBurnData[gNumBurningDynEnts].id;
        hEffect[2 * i] = v0;
        dynEntClient->flags &= ~8u;
      }
    }
    else
    {
      --gNumBurningDynEnts;
      v1 = hEffect[2 * gNumBurningDynEnts];
      *(unsigned int *)&gDynEntBurnData[i].id = *(unsigned int *)&gDynEntBurnData[gNumBurningDynEnts].id;
      hEffect[2 * i] = v1;
      dynEntClient->flags &= ~8u;
    }
  }
}

void __cdecl DynEntCl_FlameDamage(
        int localClientNum,
        unsigned __int16 dynEntId,
        DynEntityCollType drawType,
        const float *hitPos,
        const float *hitVel,
        int damage)
{
  float hitDir[3]; // [esp+14h] [ebp-1Ch] BYREF
  const DynEntityProps *dynEntProps; // [esp+20h] [ebp-10h]
  DynEntityClient *dynEntClient; // [esp+24h] [ebp-Ch]
  const DynEntityDef *dynEntDef; // [esp+28h] [ebp-8h]
  int surfaceType; // [esp+2Ch] [ebp-4h]

  if ( dynEntId == 0xFFFF
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          2205,
          0,
          "%s",
          "dynEntId != DYNENT_INVALID_ID") )
  {
    __debugbreak();
  }
  if ( !hitPos
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp", 2206, 0, "%s", "hitPos") )
  {
    __debugbreak();
  }
  if ( !hitVel
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp", 2207, 0, "%s", "hitVel") )
  {
    __debugbreak();
  }
  if ( damage <= 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          2208,
          0,
          "%s\n\t(damage) = %i",
          "((damage > 0))",
          damage) )
  {
    __debugbreak();
  }
  if ( localClientNum == jpeg_mem_init() )
  {
    dynEntDef = DynEnt_GetEntityDef(dynEntId, (DynEntityDrawType)drawType);
    dynEntClient = DynEnt_GetClientEntity(dynEntId, (DynEntityDrawType)drawType);
    dynEntProps = DynEnt_GetEntityProps(dynEntDef->type);
    surfaceType = DynEnt_GetSurfaceType(dynEntDef);
    if ( Com_SurfaceBurns(surfaceType) && (dynEntClient->flags & 3) != 0 )
    {
      DynEntCl_Damage(localClientNum, dynEntId, drawType, hitPos, hitVel, damage);
      DynEntCl_SetBurning(dynEntId, (DynEntityDrawType)drawType, 1);
      if ( dynEntProps->usePhysics )
      {
        if ( dynEntClient->physObjId )
        {
          hitDir[0] = *hitVel;
          hitDir[1] = hitVel[1];
          hitDir[2] = hitVel[2];
          if ( Vec3Normalize(hitDir) > 0.0099999998 )
            Phys_ObjBulletImpact(dynEntClient->physObjId, hitPos, hitDir, 50.0, 1.0);
        }
      }
    }
  }
}

void __cdecl DynEntCl_SetBurnFrac(unsigned __int16 dynEntId, DynEntityDrawType drawType, float frac)
{
  if ( (frac < 0.0 || frac > 1.0)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          2240,
          0,
          "%s\n\t(frac) = %g",
          "(frac >= 0.0f && frac <= 1.0f)",
          frac) )
  {
    __debugbreak();
  }
  DynEnt_GetClientEntity(dynEntId, drawType)->burnTime = (int)(float)(5000.0 * frac);
}

double __cdecl DynEntCl_GetBurnTime(unsigned __int16 dynEntId, DynEntityDrawType drawType)
{
  float v4; // [esp+4h] [ebp-Ch]
  float v5; // [esp+8h] [ebp-8h]

  v4 = (float)DynEnt_GetClientEntity(dynEntId, drawType)->burnTime / 5000.0;
  if ( (float)(v4 - 1.0) < 0.0 )
    v5 = v4;
  else
    v5 = FLOAT_1_0;
  if ( (float)(0.0 - v4) < 0.0 )
    return v5;
  else
    return *(float *)&FLOAT_0_0;
}

void __cdecl DynEntCl_MeleeEvent(int localClientNum, int attackerEntNum)
{
  float value; // [esp+0h] [ebp-6Ch]
  float v3; // [esp+4h] [ebp-68h]
  float v4; // [esp+8h] [ebp-64h]
  const centity_s *attacker; // [esp+14h] [ebp-58h]
  int damage; // [esp+18h] [ebp-54h]
  float right[3]; // [esp+24h] [ebp-48h] BYREF
  float end[3]; // [esp+30h] [ebp-3Ch] BYREF
  float forward[3]; // [esp+3Ch] [ebp-30h] BYREF
  float up[3]; // [esp+48h] [ebp-24h] BYREF
  int weaponIndex; // [esp+54h] [ebp-18h]
  float eyePos[3]; // [esp+58h] [ebp-14h] BYREF
  unsigned int traceIndex; // [esp+64h] [ebp-8h]
  const WeaponDef *weapDef; // [esp+68h] [ebp-4h]

  attacker = CG_GetEntity(localClientNum, attackerEntNum);
  if ( attacker->nextState.eType == 1 || BG_IsUseAsMeleeWeapon(attacker->nextState.weapon) )
    weaponIndex = attacker->nextState.weapon;
  else
    weaponIndex = attacker->nextState.lerp.u.player.meleeWeapon;
  if ( weaponIndex )
  {
    weapDef = BG_GetWeaponDef(weaponIndex);
    damage = weapDef->iMeleeDamage;
    CG_CalcEyePoint(localClientNum, attackerEntNum, eyePos);
    CG_GetViewDirection(localClientNum, attackerEntNum, forward, right, up);
    if ( BG_IsBayonetWeapon(weaponIndex) )
      value = player_bayonetRange->current.value;
    else
      value = player_meleeRange->current.value;
    for ( traceIndex = 0; traceIndex < 5; ++traceIndex )
    {
      if ( !player_meleeRange
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
              2287,
              0,
              "%s",
              "player_meleeRange") )
      {
        __debugbreak();
      }
      if ( !player_meleeWidth
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
              2288,
              0,
              "%s",
              "player_meleeWidth") )
      {
        __debugbreak();
      }
      if ( !player_meleeHeight
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
              2289,
              0,
              "%s",
              "player_meleeHeight") )
      {
        __debugbreak();
      }
      if ( !player_bayonetRange
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
              2290,
              0,
              "%s",
              "player_bayonetRange") )
      {
        __debugbreak();
      }
      end[0] = (float)(value * forward[0]) + eyePos[0];
      end[1] = (float)(value * forward[1]) + eyePos[1];
      end[2] = (float)(value * forward[2]) + eyePos[2];
      v4 = player_meleeWidth->current.value * (float)traceOffsets_0[traceIndex][0];
      end[0] = (float)(v4 * right[0]) + end[0];
      end[1] = (float)(v4 * right[1]) + end[1];
      end[2] = (float)(v4 * right[2]) + end[2];
      v3 = player_meleeHeight->current.value * dword_D52AA4[2 * traceIndex];
      end[0] = (float)(v3 * up[0]) + end[0];
      end[1] = (float)(v3 * up[1]) + end[1];
      end[2] = (float)(v3 * up[2]) + end[2];
      if ( DynEntCl_DynEntImpactEvent(localClientNum, attacker->nextState.number, eyePos, end, damage, 1) )
        break;
    }
  }
}

void __cdecl DynEntCl_EntityImpactEvent(
        const trace_t *trace,
        int localClientNum,
        int sourceEntityNum,
        const float *start,
        float *hitPos,
        bool isMelee)
{
  float v6; // [esp+24h] [ebp-5Ch]
  float presetBulletForceScale; // [esp+4Ch] [ebp-34h]
  float force[3]; // [esp+50h] [ebp-30h] BYREF
  unsigned int boneName; // [esp+5Ch] [ebp-24h] BYREF
  int id; // [esp+60h] [ebp-20h]
  float hitDir[3]; // [esp+68h] [ebp-18h] BYREF
  DObj *obj; // [esp+74h] [ebp-Ch]
  PhysPreset *physPreset; // [esp+78h] [ebp-8h]
  centity_s *cent; // [esp+7Ch] [ebp-4h]
  int savedregs; // [esp+80h] [ebp+0h] BYREF

  //PIXBeginNamedEvent(-1, "DynEntCl_EntityImpactEvent");
  if ( trace->hitType != TRACE_HITTYPE_ENTITY )
  {
    if ( g_DXDeviceThread != GetCurrentThreadId() )
      return;
    goto LABEL_3;
  }
  if ( DynEntCl_EventNeedsProcessed(localClientNum, sourceEntityNum) )
  {
    cent = CG_GetEntity(localClientNum, trace->hitId);
    if ( !cent
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp", 2318, 0, "%s", "cent") )
    {
      __debugbreak();
    }
    if ( cent->pose.isRagdoll )
    {
      MapHitLocationToRagdollBoneName((hitLocation_t)trace->partGroup, &boneName);
      id = Ragdoll_GetRBForBone((phys_free_list<RagdollBody>::T_internal_base *)trace->hitId, boneName);
      if ( id )
      {
        force[0] = *hitPos - *start;
        force[1] = hitPos[1] - start[1];
        force[2] = hitPos[2] - start[2];
        Vec3Normalize(force);
        v6 = scale_0 * dynEnt_bulletForce->current.value;
        force[0] = v6 * force[0];
        force[1] = v6 * force[1];
        force[2] = v6 * force[2];
        Phys_ObjAddForce((int)&savedregs, id, hitPos, force, 0);
      }
    }
    if ( cent->pose.physObjId != -1 && cent->pose.physObjId )
    {
      hitDir[0] = *hitPos - *start;
      hitDir[1] = hitPos[1] - start[1];
      hitDir[2] = hitPos[2] - start[2];
      Vec3Normalize(hitDir);
      if ( isMelee )
        CG_PlaySound(localClientNum, 1022, hitPos, 0, 0, 1.0, cgMedia.meleeKnifeHitOther);
      else
        DynEntCl_PlayImpactEffects(
          localClientNum,
          sourceEntityNum,
          (unsigned __int8)((int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20] & trace->sflags) >> 20),
          hitPos,
          trace->normal.vec.v);
      obj = Com_GetClientDObj(cent->nextState.number, localClientNum);
      if ( obj )
      {
        physPreset = DObjGetPhysPreset(obj);
        presetBulletForceScale = FLOAT_1_0;
        if ( physPreset )
          presetBulletForceScale = physPreset->bulletForceScale;
        Phys_ObjBulletImpact(
          cent->pose.physObjId,
          hitPos,
          hitDir,
          dynEnt_bulletForce->current.value,
          presetBulletForceScale);
      }
      if ( GetCurrentThreadId() != g_DXDeviceThread )
        return;
    }
    else if ( g_DXDeviceThread != GetCurrentThreadId() )
    {
      return;
    }
    D3DPERF_EndEvent();
    return;
  }
  if ( g_DXDeviceThread == GetCurrentThreadId() )
LABEL_3:
    D3DPERF_EndEvent();
}

void __cdecl DynEntCl_PlayImpactEffects(
        int localClientNum,
        int sourceEntityNum,
        unsigned int surfType,
        float *hitPos,
        const float *hitNormal)
{
  float axis[3][3]; // [esp+14h] [ebp-34h] BYREF
  const centity_s *attacker; // [esp+38h] [ebp-10h]
  unsigned int hitSound; // [esp+3Ch] [ebp-Ch]
  const WeaponDef *weaponDef; // [esp+40h] [ebp-8h]
  const FxEffectDef *hitFx; // [esp+44h] [ebp-4h]

  if ( surfType >= 0x1F
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          1680,
          0,
          "surfType doesn't index SURF_TYPECOUNT\n\t%i not in [0, %i)",
          surfType,
          31) )
  {
    __debugbreak();
  }
  if ( !hitPos
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp", 1681, 0, "%s", "hitPos") )
  {
    __debugbreak();
  }
  if ( !hitNormal
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          1682,
          0,
          "%s",
          "hitNormal") )
  {
    __debugbreak();
  }
  attacker = CG_GetEntity(localClientNum, sourceEntityNum);
  weaponDef = BG_GetWeaponDef(attacker->nextState.weapon);
  if ( !weaponDef
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          1687,
          0,
          "%s",
          "weaponDef") )
  {
    __debugbreak();
  }
  hitFx = 0;
  hitSound = 0;
  switch ( weaponDef->impactType )
  {
    case IMPACT_TYPE_BULLET_SMALL:
      hitFx = *(const FxEffectDef **)(*(unsigned int *)&cgMedia.fxDogBlood->flags + 4 * surfType);
      hitSound = cgMedia.bulletHitSmallSound[surfType];
      break;
    case IMPACT_TYPE_BULLET_LARGE:
      hitFx = *(const FxEffectDef **)(*(unsigned int *)&cgMedia.fxDogBlood->flags + 4 * surfType + 420);
      hitSound = cgMedia.bulletHitLargeSound[surfType];
      break;
    case IMPACT_TYPE_BULLET_AP:
      hitFx = *(const FxEffectDef **)(*(unsigned int *)&cgMedia.fxDogBlood->flags + 4 * surfType + 980);
      hitSound = cgMedia.bulletHitAPSound[surfType];
      break;
    case IMPACT_TYPE_BULLET_XTREME:
      hitFx = *(const FxEffectDef **)(*(unsigned int *)&cgMedia.fxDogBlood->flags + 4 * surfType + 1260);
      hitSound = cgMedia.bulletHitXTremeSound[surfType];
      break;
    case IMPACT_TYPE_SHOTGUN:
      hitFx = *(const FxEffectDef **)(*(unsigned int *)&cgMedia.fxDogBlood->flags + 4 * surfType + 700);
      hitSound = cgMedia.shotgunHitSound[surfType];
      break;
    default:
      break;
  }
  if ( hitFx && (*hitNormal != 0.0 || hitNormal[1] != 0.0 || hitNormal[2] != 0.0) )
  {
    *(_QWORD *)&axis[0][0] = *(_QWORD *)hitNormal;
    axis[0][2] = hitNormal[2];
    CG_RandomEffectAxis(axis[0], axis[1], axis[2]);
    DynEntCl_PlayEventFx(hitFx, hitPos, axis);
  }
  CG_PlaySound(localClientNum, 1022, hitPos, 0, 0, 1.0, hitSound);
}

char __cdecl DynEntCl_EventNeedsProcessed(int localClientNum, int sourceEntityNum)
{
  jpeg_common_struct *v3; // [esp+0h] [ebp-Ch]
  snapshot_s *nextSnap; // [esp+4h] [ebp-8h]

  if ( CG_GetLocalClientGlobalsForEnt(localClientNum, sourceEntityNum) )
  {
    nextSnap = CG_GetLocalClientGlobals(localClientNum)->nextSnap;
    if ( (nextSnap->ps.otherFlags & 6) == 0 || sourceEntityNum != nextSnap->ps.clientNum )
      return 0;
  }
  else if ( localClientNum != jpeg_mem_init(v3) )
  {
    return 0;
  }
  return 1;
}

char __cdecl DynEntCl_DynEntImpactEvent(
        int localClientNum,
        int sourceEntityNum,
        const float *start,
        const float *end,
        int damage,
        bool isMelee)
{
  int PhysObj; // eax
  snapshot_s *nextSnap; // [esp+40h] [ebp-C8h]
  DynEntityPose *dynEntPose; // [esp+54h] [ebp-B4h]
  pointtrace_t clip; // [esp+58h] [ebp-B0h] BYREF
  float hitDir[3]; // [esp+A0h] [ebp-68h] BYREF
  DynEntityDrawType drawType; // [esp+ACh] [ebp-5Ch] BYREF
  DynEntityClient *dynEntClient; // [esp+B4h] [ebp-54h]
  const DynEntityDef *dynEntDef; // [esp+B8h] [ebp-50h]
  trace_t trace; // [esp+BCh] [ebp-4Ch] BYREF
  float hitPos[3]; // [esp+F8h] [ebp-10h] BYREF
  unsigned __int16 dynEntId; // [esp+104h] [ebp-4h]

  //PIXBeginNamedEvent(-1, "DynEntCl_DynEntImpactEvent");
  TraceExtents::TraceExtents(&clip.extents);
  memset(&trace, 0, 16);
  if ( !start
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp", 2379, 0, "%s", "start") )
  {
    __debugbreak();
  }
  if ( !end
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp", 2380, 0, "%s", "end") )
  {
    __debugbreak();
  }
  if ( damage < 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          2381,
          0,
          "%s\n\t(damage) = %i",
          "((damage >= 0))",
          damage) )
  {
    __debugbreak();
  }
  if ( CG_GetLocalClientGlobalsForEnt(localClientNum, sourceEntityNum) )
  {
    nextSnap = CG_GetLocalClientGlobals(localClientNum)->nextSnap;
    if ( (nextSnap->ps.otherFlags & 6) == 0 || sourceEntityNum != nextSnap->ps.clientNum )
    {
      if ( g_DXDeviceThread == GetCurrentThreadId() )
        D3DPERF_EndEvent();
      return 0;
    }
    goto LABEL_24;
  }
  if ( localClientNum == jpeg_mem_init() )
  {
LABEL_24:
    memset((unsigned __int8 *)&trace, 0, sizeof(trace));
    trace.fraction = FLOAT_1_0;
    *(_QWORD *)clip.extents.start.vec.v = *(_QWORD *)start;
    clip.extents.start.vec.v[2] = start[2];
    *(_QWORD *)clip.extents.end.vec.v = *(_QWORD *)end;
    clip.extents.end.vec.v[2] = end[2];
    CM_CalcTraceExtents(&clip.extents);
    clip.ignoreEntParams = 0;
    clip.contentmask = 8396819;
    clip.bLocational = 1;
    clip.priorityMap = 0;
    DynEntCl_PointTrace(&clip, &trace);
    dynEntId = Trace_GetDynEntHitId(&trace, &drawType);
    if ( dynEntId == 0xFFFF )
    {
      if ( GetCurrentThreadId() == g_DXDeviceThread )
        D3DPERF_EndEvent();
      return 0;
    }
    else
    {
      dynEntDef = DynEnt_GetEntityDef(dynEntId, drawType);
      dynEntClient = DynEnt_GetClientEntity(dynEntId, drawType);
      if ( (dynEntClient->flags & 1) == 0
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
              2416,
              0,
              "%s",
              "dynEntClient->flags & DYNENT_CL_ACTIVE") )
      {
        __debugbreak();
      }
      Vec3Lerp(start, end, trace.fraction, hitPos);
      if ( isMelee )
        CG_PlaySound(localClientNum, 1022, hitPos, 0, 0, 1.0, cgMedia.meleeHit);
      else
        DynEntCl_PlayImpactEffects(
          localClientNum,
          sourceEntityNum,
          (unsigned __int8)((int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20] & trace.sflags) >> 20),
          hitPos,
          trace.normal.vec.v);
      if ( (dynEntDef->flags & 4) != 0 )
      {
        if ( g_DXDeviceThread == GetCurrentThreadId() )
          D3DPERF_EndEvent();
        return 0;
      }
      else
      {
        hitDir[0] = *end - *start;
        hitDir[1] = end[1] - start[1];
        hitDir[2] = end[2] - start[2];
        Vec3Normalize(hitDir);
        if ( DynEnt_GetEntityProps(dynEntDef->type)->usePhysics )
        {
          dynEntPose = DynEnt_GetClientPose(dynEntId, drawType);
          if ( !dynEntClient->physObjId )
          {
            PhysObj = DynEntCl_CreatePhysObj(dynEntDef, dynEntClient, &dynEntPose->pose);
            dynEntClient->physObjId = PhysObj;
          }
          if ( dynEntClient->physObjId )
            Phys_ObjBulletImpact(
              dynEntClient->physObjId,
              hitPos,
              hitDir,
              dynEnt_bulletForce->current.value,
              dynEntDef->physPreset->bulletForceScale);
        }
        if ( damage )
          DynEntCl_Damage(localClientNum, dynEntId, (DynEntityCollType)drawType, hitPos, hitDir, damage);
        if ( GetCurrentThreadId() == g_DXDeviceThread )
          D3DPERF_EndEvent();
        return 1;
      }
    }
  }
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  return 0;
}

void __cdecl DynEntCl_ExplosionEvent(
        int localClientNum,
        bool isCylinder,
        float *origin,
        float innerRadius,
        float outerRadius,
        float *impulse,
        float inScale,
        int innerDamage,
        int outerDamage)
{
  int PhysObj; // eax
  double v10; // st7
  double v11; // st7
  double v12; // st7
  float v14; // [esp+38h] [ebp-207Ch]
  int damage; // [esp+3Ch] [ebp-2078h]
  DynEntityPose *pose; // [esp+40h] [ebp-2074h]
  float CylindricalRadiusDistSqr; // [esp+44h] [ebp-2070h]
  unsigned int ClosestEntities; // [esp+48h] [ebp-206Ch]
  float radiusMaxs[2]; // [esp+4Ch] [ebp-2068h] BYREF
  float v20; // [esp+54h] [ebp-2060h]
  float v; // [esp+58h] [ebp-205Ch] BYREF
  float v22; // [esp+5Ch] [ebp-2058h]
  float v23; // [esp+60h] [ebp-2054h]
  DynEntityDrawType drawType; // [esp+64h] [ebp-2050h]
  DynEntityClient *dynEntClient; // [esp+68h] [ebp-204Ch]
  float impulsea[3]; // [esp+6Ch] [ebp-2048h] BYREF
  const DynEntityDef *dynEntDef; // [esp+78h] [ebp-203Ch]
  float v28; // [esp+7Ch] [ebp-2038h]
  float outPosition; // [esp+80h] [ebp-2034h] BYREF
  float v30; // [esp+84h] [ebp-2030h]
  float v31; // [esp+88h] [ebp-202Ch]
  unsigned __int16 hitEnts[4096]; // [esp+8Ch] [ebp-2028h] BYREF
  float v33; // [esp+2090h] [ebp-24h]
  unsigned int i; // [esp+2094h] [ebp-20h]
  float radiusMins[2]; // [esp+2098h] [ebp-1Ch] BYREF
  float v36; // [esp+20A0h] [ebp-14h]
  float v37; // [esp+20A4h] [ebp-10h]
  float v38; // [esp+20A8h] [ebp-Ch]
  DynEntityColl *dynEntColl; // [esp+20ACh] [ebp-8h]
  float v40; // [esp+20B0h] [ebp-4h]
  int savedregs; // [esp+20B4h] [ebp+0h] BYREF

  if ( !origin
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp", 2515, 0, "%s", "origin") )
  {
    __debugbreak();
  }
  if ( innerRadius < 0.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          2516,
          0,
          "%s",
          "innerRadius >= 0.0f") )
  {
    __debugbreak();
  }
  if ( outerRadius < innerRadius
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          2517,
          0,
          "%s",
          "outerRadius >= innerRadius") )
  {
    __debugbreak();
  }
  if ( innerDamage < 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          2518,
          0,
          "%s\n\t(innerDamage) = %i",
          "((innerDamage >= 0))",
          innerDamage) )
  {
    __debugbreak();
  }
  if ( outerDamage < 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          2519,
          0,
          "%s\n\t(outerDamage) = %i",
          "((outerDamage >= 0))",
          outerDamage) )
  {
    __debugbreak();
  }
  if ( DynEntCl_EventNeedsProcessed(localClientNum, 1023) && outerRadius != 0.0 )
  {
    Rope_ExplosionEvent(origin, innerRadius, outerRadius, innerDamage, outerDamage);
    v40 = outerRadius * outerRadius;
    v37 = innerRadius * innerRadius;
    v28 = *(float *)&FLOAT_0_0;
    if ( (float)(outerRadius * outerRadius) > (float)(innerRadius * innerRadius) )
      v28 = 1.0 / (float)(innerRadius - outerRadius);
    LODWORD(v14) = COERCE_UNSIGNED_INT(1.4142135 * outerRadius) ^ _mask__NegFloat_;
    radiusMins[0] = *origin + v14;
    radiusMins[1] = origin[1] + v14;
    v36 = origin[2] + v14;
    radiusMaxs[0] = *origin + (float)(1.4142135 * outerRadius);
    radiusMaxs[1] = origin[1] + (float)(1.4142135 * outerRadius);
    v20 = origin[2] + (float)(1.4142135 * outerRadius);
    if ( isCylinder )
    {
      v36 = FLOAT_N3_4028235e38;
      v20 = FLOAT_3_4028235e38;
    }
    for ( drawType = DYNENT_DRAW_MODEL; (unsigned int)drawType < DYNENT_DRAW_COUNT; ++drawType )
    {
      ClosestEntities = DynEntCl_GetClosestEntities(
                          (DynEntityCollType)drawType,
                          radiusMins,
                          radiusMaxs,
                          origin,
                          hitEnts,
                          isCylinder);
      for ( i = 0; i < ClosestEntities; ++i )
      {
        dynEntDef = DynEnt_GetEntityDef(hitEnts[i], drawType);
        dynEntClient = DynEnt_GetClientEntity(hitEnts[i], drawType);
        if ( (dynEntDef->flags & 8) == 0 )
        {
          if ( (dynEntClient->flags & 1) == 0
            && !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
                  2557,
                  0,
                  "%s",
                  "dynEntClient->flags & DYNENT_CL_ACTIVE") )
          {
            __debugbreak();
          }
          pose = DynEnt_GetClientPose(hitEnts[i], drawType);
          dynEntColl = DynEnt_GetEntityColl((DynEntityCollType)drawType, hitEnts[i]);
          if ( isCylinder )
            CylindricalRadiusDistSqr = DynEnt_GetCylindricalRadiusDistSqr(dynEntColl, origin);
          else
            CylindricalRadiusDistSqr = DynEnt_GetRadiusDistSqr(dynEntColl, origin);
          if ( CylindricalRadiusDistSqr < v40 )
          {
            v38 = inScale;
            if ( CylindricalRadiusDistSqr > v37 )
            {
              if ( v40 <= v37
                && !Assert_MyHandler(
                      "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
                      2573,
                      1,
                      "%s",
                      "outerRadiusSqr > innerRadiusSqr") )
              {
                __debugbreak();
              }
              v38 = (float)((float)(fsqrt(CylindricalRadiusDistSqr) - outerRadius) * v28) * v38;
            }
            if ( v38 < 0.0
              && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
                    2576,
                    1,
                    "%s",
                    "scale >= 0") )
            {
              __debugbreak();
            }
            v = pose->pose.origin[0] - *origin;
            v22 = pose->pose.origin[1] - origin[1];
            v23 = pose->pose.origin[2] - origin[2];
            outPosition = pose->pose.origin[0];
            v30 = pose->pose.origin[1];
            v31 = pose->pose.origin[2];
            if ( 0.0 == *impulse && impulse[1] == 0.0 && impulse[2] == 0.0 )
            {
              if ( isCylinder )
                v23 = *(float *)&FLOAT_0_0;
              Vec3Normalize(&v);
              v23 = v23 + dynEnt_explodeUpbias->current.value;
              Vec3Normalize(&v);
            }
            else
            {
              v = *impulse;
              v22 = impulse[1];
              v23 = impulse[2];
            }
            if ( DynEnt_GetEntityProps(dynEntDef->type)->usePhysics )
            {
              if ( !dynEntClient->physObjId )
              {
                PhysObj = DynEntCl_CreatePhysObj(dynEntDef, dynEntClient, &pose->pose);
                dynEntClient->physObjId = PhysObj;
              }
              if ( dynEntClient->physObjId )
              {
                v33 = (float)(v38 * dynEntDef->physPreset->explosiveForceScale) * dynEnt_explodeForce->current.value;
                if ( v33 > dynEnt_explodeMinForce->current.value )
                {
                  impulsea[0] = v33 * v;
                  impulsea[1] = v33 * v22;
                  impulsea[2] = v33 * v23;
                  Phys_ObjGetCenterOfMass(dynEntClient->physObjId, &outPosition);
                  v10 = flrand(-1.0, 1.0);
                  outPosition = v10 * dynEnt_explodeSpinScale->current.value + outPosition;
                  v11 = flrand(-1.0, 1.0);
                  v30 = v11 * dynEnt_explodeSpinScale->current.value + v30;
                  v12 = flrand(-1.0, 1.0);
                  v31 = v12 * dynEnt_explodeSpinScale->current.value + v31;
                  Phys_ObjAddForce((int)&savedregs, dynEntClient->physObjId, &outPosition, impulsea, 0);
                }
              }
            }
            damage = (int)(float)((float)((float)(innerDamage - outerDamage) * v38) + (float)outerDamage);
            if ( damage )
              DynEntCl_Damage(localClientNum, hitEnts[i], (DynEntityCollType)drawType, &outPosition, &v, damage);
          }
        }
      }
    }
  }
}

unsigned int __cdecl DynEntCl_GetClosestEntities(
        DynEntityCollType drawType,
        float *radiusMins,
        float *radiusMaxs,
        float *origin,
        unsigned __int16 *hitEnts,
        bool isCylinder)
{
  double CylindricalRadiusDistSqr; // st7
  unsigned int unsignedInt_low; // [esp+C8h] [ebp-8014h]
  MaterialMemory v9[4096]; // [esp+CCh] [ebp-8010h] BYREF
  DynEntityClient *ClientEntity; // [esp+80D0h] [ebp-Ch]
  unsigned int i; // [esp+80D4h] [ebp-8h]
  DynEntityColl *dynEntColl; // [esp+80D8h] [ebp-4h]

  unsignedInt_low = DynEntCl_AreaEntities(
                      (DynEntityDrawType)drawType,
                      radiusMins,
                      radiusMaxs,
                      8396819,
                      0x1000u,
                      hitEnts);
  if ( unsignedInt_low > dynEnt_explodeMaxEnts->current.integer )
  {
    for ( i = 0; i < unsignedInt_low; ++i )
    {
      LOWORD(v9[i].memory) = hitEnts[i];
      ClientEntity = DynEnt_GetClientEntity(hitEnts[i], (DynEntityDrawType)drawType);
      if ( (ClientEntity->flags & 1) == 0
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
              2472,
              0,
              "%s",
              "dynEntClient->flags & DYNENT_CL_ACTIVE") )
      {
        __debugbreak();
      }
      dynEntColl = DynEnt_GetEntityColl(drawType, hitEnts[i]);
      if ( isCylinder )
        CylindricalRadiusDistSqr = DynEnt_GetCylindricalRadiusDistSqr(dynEntColl, origin);
      else
        CylindricalRadiusDistSqr = DynEnt_GetRadiusDistSqr(dynEntColl, origin);
      *(float *)&v9[i].material = CylindricalRadiusDistSqr;
    }
    std::_Sort<RagdollSortStruct *,int,bool (__cdecl *)(RagdollSortStruct const &,RagdollSortStruct const &)>(
      v9,
      &v9[unsignedInt_low],
      (int)(8 * unsignedInt_low) >> 3,
      (bool (__cdecl *)(const MaterialMemory *, const MaterialMemory *))DynEntCl_CompareDynEntsForExplosion);
    unsignedInt_low = LOWORD(dynEnt_explodeMaxEnts->current.unsignedInt);
    if ( unsignedInt_low != dynEnt_explodeMaxEnts->current.integer
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
            2484,
            0,
            "%s",
            "hitCount == (uint)dynEnt_explodeMaxEnts->current.integer") )
    {
      __debugbreak();
    }
    for ( i = 0; i < unsignedInt_low; ++i )
      hitEnts[i] = v9[i].memory;
  }
  return unsignedInt_low;
}

bool __cdecl DynEntCl_CompareDynEntsForExplosion(const DynEntSortStruct *ent1, const DynEntSortStruct *ent2)
{
  return ent2->distSq > ent1->distSq;
}

void __cdecl DynEntCl_DestroyEvent(
        int localClientNum,
        unsigned __int16 dynEntId,
        DynEntityCollType drawType,
        const float *hitPos,
        const float *hitDir)
{
  DynEntityPose *dynEntPose; // [esp+8h] [ebp-34h]
  DynEntityClient *dynEntClient; // [esp+10h] [ebp-2Ch]
  const DynEntityDef *dynEntDef; // [esp+14h] [ebp-28h]
  float axis[3][3]; // [esp+18h] [ebp-24h] BYREF

  if ( dynEntId == 0xFFFF
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
          2630,
          0,
          "%s",
          "dynEntId != DYNENT_INVALID_ID") )
  {
    __debugbreak();
  }
  if ( !hitPos
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp", 2631, 0, "%s", "hitPos") )
  {
    __debugbreak();
  }
  if ( !hitDir
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp", 2632, 0, "%s", "hitDir") )
  {
    __debugbreak();
  }
  if ( DynEntCl_EventNeedsProcessed(localClientNum, 1023) )
  {
    CG_GetLocalClientGlobals(localClientNum);
    dynEntDef = DynEnt_GetEntityDef(dynEntId, (DynEntityDrawType)drawType);
    dynEntClient = DynEnt_GetClientEntity(dynEntId, (DynEntityDrawType)drawType);
    if ( (dynEntClient->flags & 1) != 0
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
            2642,
            0,
            "%s",
            "!(dynEntClient->flags & DYNENT_CL_ACTIVE)") )
    {
      __debugbreak();
    }
    if ( (dynEntClient->flags & 2) == 0
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_client.cpp",
            2643,
            0,
            "%s",
            "dynEntClient->flags & DYNENT_CL_VISIBLE") )
    {
      __debugbreak();
    }
    dynEntClient->flags &= ~2u;
    DynEntCl_UnlinkEntity(dynEntId, drawType);
    dynEntPose = DynEnt_GetClientPose(dynEntId, (DynEntityDrawType)drawType);
    if ( dynEntDef->destroyFx || dynEntDef->destroyPieces )
      UnitQuatToAxis(dynEntPose->pose.quat, axis);
    if ( dynEntDef->destroyFx )
      DynEntCl_PlayEventFx(dynEntDef->destroyFx, dynEntPose->pose.origin, axis);
    if ( dynEntDef->destroyPieces )
      DynEntPieces_SpawnPieces(localClientNum, dynEntDef->destroyPieces, dynEntPose->pose.origin, axis, hitPos, hitDir);
    if ( dynEntDef->destroySound )
      CG_PlaySound(localClientNum, 1022, dynEntPose->pose.origin, 0, 0, 1.0, dynEntDef->destroySound);
    DynEntCl_TurnOffLight(dynEntDef);
  }
}

void __cdecl CreateRopes(int localClientNum)
{
  char v1; // al
  char NumParticles; // al
  char v3; // al
  int v4; // [esp+Ch] [ebp-28h]
  int target_index2; // [esp+10h] [ebp-24h]
  float rope_width; // [esp+20h] [ebp-14h]
  float force_scale; // [esp+24h] [ebp-10h]
  PhysConstraint *constraint; // [esp+28h] [ebp-Ch]
  int i; // [esp+2Ch] [ebp-8h]
  int rope_index; // [esp+30h] [ebp-4h]

  if ( CL_LocalClient_IsFirstActive(localClientNum) )
  {
    Rope_Reset();
    if ( !r_reflectionProbeGenerate->current.enabled )
    {
      rope_index = 0;
      for ( i = 0; i < cm.num_constraints; ++i )
      {
        constraint = &cm.constraints[i];
        if ( constraint->type == CONSTRAINT_ROPE )
        {
          rope_width = constraint->minAngle;
          if ( rope_width <= 0.0 )
            rope_width = FLOAT_1_8;
          force_scale = constraint->scale[0];
          if ( force_scale <= 0.0 )
            force_scale = FLOAT_1_0;
          Rope_Create(
            rope_index,
            constraint->pos,
            constraint->pos2,
            constraint->distance,
            0.89999998,
            rope_width,
            force_scale,
            1);
          if ( constraint->attach_point_type1 != ATTACH_POINT_ENT || constraint->target_index1 == -1 )
          {
            if ( constraint->attach_point_type1 == ATTACH_POINT_DYNENT )
            {
              Rope_AddDynEntAnchor(rope_index, 0, constraint->pos, constraint->target_index1);
            }
            else if ( constraint->attach_point_type1 == ATTACH_POINT_WORLD )
            {
              Rope_AddAnchor(rope_index, 0, constraint->pos, -1, -1, ROPE_WORLD_CONSTRAINT);
            }
          }
          else
          {
            Rope_AddEntAnchor(rope_index, 0, constraint->pos, constraint->target_index1, 0);
          }
          if ( constraint->attach_point_type2 != ATTACH_POINT_ENT || constraint->target_index2 == -1 )
          {
            if ( constraint->attach_point_type2 == ATTACH_POINT_DYNENT )
            {
              target_index2 = constraint->target_index2;
              NumParticles = Rope_GetNumParticles(rope_index);
              Rope_AddDynEntAnchor(rope_index, NumParticles - 1, constraint->pos2, target_index2);
            }
            else if ( constraint->attach_point_type2 == ATTACH_POINT_WORLD
                   && (constraint->pos[0] != constraint->pos2[0]
                    || constraint->pos[1] != constraint->pos2[1]
                    || constraint->pos[2] != constraint->pos2[2]) )
            {
              v3 = Rope_GetNumParticles(rope_index);
              Rope_AddAnchor(rope_index, v3 - 1, constraint->pos2, -1, -1, ROPE_WORLD_CONSTRAINT);
            }
          }
          else
          {
            v4 = constraint->target_index2;
            v1 = Rope_GetNumParticles(rope_index);
            Rope_AddEntAnchor(rope_index, v1 - 1, constraint->pos2, v4, 0);
          }
          if ( constraint->material )
            Rope_SetMaterial(rope_index, constraint->material);
          if ( (constraint->flags & 1) != 0 )
            Rope_SetFlag(rope_index, 1, 1);
          Rope_Settle(rope_index);
          constraint->rope_index = rope_index++;
        }
      }
    }
  }
}

void __cdecl MapHitLocationToRagdollBoneName(hitLocation_t hitLoc, unsigned int *boneName)
{
  *boneName = 0;
  switch ( hitLoc )
  {
    case HITLOC_HELMET:
    case HITLOC_HEAD:
      *boneName = SL_FindLowercaseString("j_head", SCRIPTINSTANCE_SERVER);
      break;
    case HITLOC_NECK:
      *boneName = SL_FindLowercaseString("j_neck", SCRIPTINSTANCE_SERVER);
      break;
    case HITLOC_TORSO_UPR:
      *boneName = SL_FindLowercaseString("j_spineupper", SCRIPTINSTANCE_SERVER);
      break;
    case HITLOC_TORSO_LWR:
      *boneName = SL_FindLowercaseString("j_spinelower", SCRIPTINSTANCE_SERVER);
      break;
    case HITLOC_R_ARM_UPR:
      *boneName = SL_FindLowercaseString("j_elbow_ri", SCRIPTINSTANCE_SERVER);
      break;
    case HITLOC_L_ARM_UPR:
      *boneName = SL_FindLowercaseString("j_elbow_le", SCRIPTINSTANCE_SERVER);
      break;
    case HITLOC_R_ARM_LWR:
    case HITLOC_R_HAND:
      *boneName = SL_FindLowercaseString("j_wrist_ri", SCRIPTINSTANCE_SERVER);
      break;
    case HITLOC_L_ARM_LWR:
    case HITLOC_L_HAND:
      *boneName = SL_FindLowercaseString("j_wrist_le", SCRIPTINSTANCE_SERVER);
      break;
    case HITLOC_R_LEG_UPR:
      *boneName = SL_FindLowercaseString("j_knee_ri", SCRIPTINSTANCE_SERVER);
      break;
    case HITLOC_L_LEG_UPR:
      *boneName = SL_FindLowercaseString("j_knee_le", SCRIPTINSTANCE_SERVER);
      break;
    case HITLOC_R_LEG_LWR:
    case HITLOC_R_FOOT:
      *boneName = SL_FindLowercaseString("j_ankle_ri", SCRIPTINSTANCE_SERVER);
      break;
    case HITLOC_L_LEG_LWR:
    case HITLOC_L_FOOT:
      *boneName = SL_FindLowercaseString("j_ankle_le", SCRIPTINSTANCE_SERVER);
      break;
    default:
      return;
  }
}

const DynEntityDef *__cdecl DynEnt_GetEntityDef(unsigned __int16 absDynEntId)
{
  DynEntityDrawType drawType; // [esp+0h] [ebp-8h] BYREF
  unsigned __int16 id; // [esp+4h] [ebp-4h] BYREF

  DynEnt_GetClientIdDrawType(absDynEntId, &id, &drawType);
  return DynEnt_GetEntityDef(id, drawType);
}

DynEntityClient *__cdecl DynEnt_GetClientEntity(unsigned __int16 id)
{
  unsigned __int16 brushid; // [esp+0h] [ebp-4h]

  if ( id < cm.originalDynEntCount + 256 )
    return &cm.dynEntClientList[0][id];
  brushid = id - cm.originalDynEntCount - 256;
  if ( &cm.dynEntClientList[0][id] != &cm.dynEntClientList[1][brushid]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\DynEntity\\DynEntity_load_obj.cpp",
          1503,
          0,
          "%s",
          "&cm.dynEntClientList[DYNENT_DRAW_MODEL][id] == &cm.dynEntClientList[DYNENT_DRAW_BRUSH][brushid]") )
  {
    __debugbreak();
  }
  return &cm.dynEntClientList[1][brushid];
}

