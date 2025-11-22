#include "r_spotshadow.h"

void __cdecl R_AddSpotShadowEntCmd(const GfxSpotShadowEntCmd *data)
{
  DObjAnimMat *boneMatrix; // [esp+18h] [ebp-30h]
  const DObj *obj; // [esp+1Ch] [ebp-2Ch] BYREF
  GfxSceneEntity *localSceneEnt; // [esp+20h] [ebp-28h] BYREF
  const GfxLight *localLight; // [esp+24h] [ebp-24h]
  float boxHalfSize[3]; // [esp+28h] [ebp-20h] BYREF
  GfxSceneEntity *sceneEnt; // [esp+34h] [ebp-14h]
  float boxCenter[3]; // [esp+38h] [ebp-10h] BYREF
  const GfxSpotShadowEntCmd *cmd; // [esp+44h] [ebp-4h]
  int savedregs; // [esp+48h] [ebp+0h] BYREF

  cmd = data;
  sceneEnt = data->sceneEnt;
  boneMatrix = R_UpdateSceneEntBounds((GfxSceneEntity *)&savedregs, sceneEnt, &localSceneEnt, &obj, 1);
  if ( boneMatrix )
  {
    if ( !localSceneEnt
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_spotshadow.cpp",
            179,
            0,
            "%s",
            "localSceneEnt") )
    {
      __debugbreak();
    }
    localLight = cmd->light;
    boxCenter[0] = (float)(localSceneEnt->cull.mins[0] + localSceneEnt->cull.maxs[0]) * 0.5;
    boxCenter[1] = (float)(localSceneEnt->cull.mins[1] + localSceneEnt->cull.maxs[1]) * 0.5;
    boxCenter[2] = (float)(localSceneEnt->cull.mins[2] + localSceneEnt->cull.maxs[2]) * 0.5;
    boxHalfSize[0] = boxCenter[0] - localSceneEnt->cull.mins[0];
    boxHalfSize[1] = boxCenter[1] - localSceneEnt->cull.mins[1];
    boxHalfSize[2] = boxCenter[2] - localSceneEnt->cull.mins[2];
    if ( CullBoxFromCone(localLight->origin, localLight->dir, localLight->cosHalfFovOuter, boxCenter, boxHalfSize) )
    {
      CG_UsedDObjCalcPose(localSceneEnt->info.pose);
    }
    else
    {
      CG_CullIn(localSceneEnt->info.pose);
      R_SkinSceneDObj(sceneEnt, localSceneEnt, obj, boneMatrix, 0);
      R_AddSceneDObj(localSceneEnt->entnum, localLight->spotShadowIndex + 3);
    }
  }
  else if ( localSceneEnt )
  {
    CG_UsedDObjCalcPose(localSceneEnt->info.pose);
  }
}

char __cdecl R_AddSpotShadowsForLight(
        GfxViewInfo *viewInfo,
        GfxLight *light,
        unsigned int shadowableLightIndex,
        unsigned int totalSpotLightCount,
        float spotShadowFade)
{
  float nearPlaneBias; // [esp+4h] [ebp-38h]
  bool v7; // [esp+8h] [ebp-34h]
  float v8; // [esp+18h] [ebp-24h]
  float v9; // [esp+1Ch] [ebp-20h]
  float v10; // [esp+20h] [ebp-1Ch]
  unsigned int tileCount; // [esp+30h] [ebp-Ch]
  GfxSpotShadow *spotShadow; // [esp+34h] [ebp-8h]
  unsigned int spotShadowIndex; // [esp+38h] [ebp-4h]
  int savedregs; // [esp+3Ch] [ebp+0h] BYREF

  if ( !light
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_spotshadow.cpp", 275, 0, "%s", "light") )
  {
    __debugbreak();
  }
  if ( light->type != 2
    && light->type != 3
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_spotshadow.cpp",
          276,
          0,
          "%s\n\t(light->type) = %i",
          "(light->type == GFX_LIGHT_TYPE_SPOT || light->type == GFX_LIGHT_TYPE_OMNI)",
          light->type) )
  {
    __debugbreak();
  }
  spotShadowIndex = frontEndDataOut->spotShadowCount;
  if ( spotShadowIndex >= 4
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_spotshadow.cpp",
          279,
          0,
          "spotShadowIndex doesn't index R_SPOTSHADOW_TILE_COUNT\n\t%i not in [0, %i)",
          spotShadowIndex,
          4) )
  {
    __debugbreak();
  }
  ++frontEndDataOut->spotShadowCount;
  light->spotShadowIndex = spotShadowIndex;
  spotShadow = &frontEndDataOut->spotShadows[spotShadowIndex];
  spotShadow->shadowableLightIndex = truncate_cast<unsigned char>(shadowableLightIndex);
  spotShadow->light = light;
  spotShadow->fade = spotShadowFade;
  spotShadow->flags &= ~1u;
  v7 = sm_qualitySpotShadow->current.enabled
    && !Com_BitCheckAssert(frontEndDataOut->shadowableLightHasShadowMap, rgp.world->sunPrimaryLightIndex, 32);
  if ( v7 && spotShadowIndex < 2 )
  {
    spotShadow->viewport.x = 0;
    spotShadow->viewport.y = spotShadowIndex * (unsigned int)dx.singleSampleDepthStencilSurface;
    spotShadow->viewport.width = (int)dx.singleSampleDepthStencilSurface;
    spotShadow->viewport.height = (int)dx.singleSampleDepthStencilSurface;
    spotShadow->image = stru_B50E948.image;
    spotShadow->renderTargetId = 14;
    v8 = 0.5 / (float)(2 * (int)dx.singleSampleDepthStencilSurface);
    v9 = 0.5 / (float)(int)dx.singleSampleDepthStencilSurface;
    v10 = -0.25 / (float)(2 * (int)dx.singleSampleDepthStencilSurface);
    spotShadow->pixelAdjust[0] = 0.25 / (float)(int)dx.singleSampleDepthStencilSurface;
    spotShadow->pixelAdjust[1] = v8;
    spotShadow->pixelAdjust[2] = v9;
    spotShadow->pixelAdjust[3] = v10;
    spotShadow->clearScreen = spotShadowIndex == 0;
    spotShadow->clearMesh = &gfxMeshGlob.sunShadowClearMeshData[spotShadowIndex];
    tileCount = 2;
  }
  else
  {
    spotShadow->viewport.x = 0;
    spotShadow->viewport.y = spotShadowIndex << 9;
    spotShadow->viewport.width = 512;
    spotShadow->viewport.height = 512;
    spotShadow->image = stru_B50E95C.image;
    spotShadow->renderTargetId = 15;
    spotShadow->pixelAdjust[0] = FLOAT_0_00048828125;
    spotShadow->pixelAdjust[1] = FLOAT_0_00024414062;
    spotShadow->pixelAdjust[2] = FLOAT_0_0009765625;
    spotShadow->pixelAdjust[3] = FLOAT_N0_00012207031;
    if ( v7 )
      spotShadow->clearScreen = spotShadowIndex == 2;
    else
      spotShadow->clearScreen = spotShadowIndex == 0;
    spotShadow->clearMesh = &gfxMeshGlob.spotShadowClearMeshData[spotShadowIndex];
    tileCount = 4;
  }
  if ( R_IsPrimaryLight(shadowableLightIndex) )
    nearPlaneBias = *(float *)&FLOAT_0_0;
  else
    nearPlaneBias = scene.dynamicSpotLightNearPlaneOffset;
  R_SetViewParmsForLight((int)&savedregs, light, &spotShadow->shadowViewParms, nearPlaneBias);
  R_GetSpotShadowLookupMatrix(&spotShadow->shadowViewParms, 0, 0, spotShadowIndex, tileCount, &spotShadow->lookupMatrix);
  if ( R_IsPrimaryLight(shadowableLightIndex) )
    R_AddSpotShadowModelEntities(viewInfo->localClientNum, shadowableLightIndex, light);
  return 1;
}

// local variable allocation has failed, the output may be wrong!
void  R_SetViewParmsForLight(
        int a1@<ebp>,
        const GfxLight *light,
        GfxViewParms *viewParms,
        float nearPlaneBias)
{
  double v4; // xmm0_8
  double v5; // xmm0_8
  float v6; // xmm0_4
  long double v7; // [esp+14h] [ebp-CCh]
  long double v8; // [esp+14h] [ebp-CCh]
  float tanHalfFovX; // [esp+18h] [ebp-C8h]
  float cosHalfFovOuter; // [esp+2Ch] [ebp-B4h]
  int COS_HALF_CLAMP_EPSILON; // [esp+3Ch] [ebp-A4h]
  _BYTE v12[140]; // [esp+44h] [ebp-9Ch] OVERLAPPED BYREF
  float *v13; // [esp+D0h] [ebp-10h]
  int v14; // [esp+D4h] [ebp-Ch]
  void *v15; // [esp+D8h] [ebp-8h]
  void *retaddr; // [esp+E0h] [ebp+0h]

  v14 = a1;
  v15 = retaddr;
  v13 = viewParms->axis[0];
  *(unsigned int *)&v12[136] = light->dir;
  LODWORD(viewParms->axis[0][0]) = LODWORD(light->dir[0]) ^ _mask__NegFloat_;
  *((unsigned int *)v13 + 1) = *(unsigned int *)(*(unsigned int *)&v12[136] + 4) ^ _mask__NegFloat_;
  *((unsigned int *)v13 + 2) = *(unsigned int *)(*(unsigned int *)&v12[136] + 8) ^ _mask__NegFloat_;
  PerpendicularVector(viewParms->axis[0], viewParms->axis[2]);
  Vec3Cross(viewParms->axis[2], viewParms->axis[0], viewParms->axis[1]);
  *(unsigned int *)&v12[132] = viewParms->origin;
  *(unsigned int *)&v12[128] = light->origin;
  viewParms->origin[0] = light->origin[0];
  *(float *)(*(unsigned int *)&v12[132] + 4) = *(float *)(*(unsigned int *)&v12[128] + 4);
  *(float *)(*(unsigned int *)&v12[132] + 8) = *(float *)(*(unsigned int *)&v12[128] + 8);
  viewParms->origin[3] = FLOAT_1_0;
  MatrixForViewer(viewParms->origin, viewParms->axis, (float (*)[4])&v12[64]);
  memset(v12, 0, 0x40u);
  v4 = light->angles[2];
  __libm_sse2_sin(v7);
  *(float *)&v4 = v4;
  COS_HALF_CLAMP_EPSILON = LODWORD(v4);
  v5 = light->angles[2];
  __libm_sse2_cos(v8);
  *(float *)&v5 = v5;
  *(unsigned int *)v12 = LODWORD(v5);
  *(unsigned int *)&v12[4] = COS_HALF_CLAMP_EPSILON ^ _mask__NegFloat_;
  *(unsigned int *)&v12[16] = COS_HALF_CLAMP_EPSILON;
  *(unsigned int *)&v12[20] = LODWORD(v5);
  *(float *)&v12[40] = FLOAT_1_0;
  *(float *)&v12[60] = FLOAT_1_0;
  MatrixMultiply44((const float (*)[4])&v12[64], (const float (*)[4])v12, viewParms->viewMatrix.m);
  if ( sm_showSpotAxis->current.enabled )
    R_AddDebugAxis(&frontEndDataOut->debugGlobals, viewParms->origin, viewParms->axis, 100.0, 1);
  if ( !light->canUseShadowMap
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_spotshadow.cpp",
          60,
          0,
          "%s",
          "light->canUseShadowMap") )
  {
    __debugbreak();
  }
  if ( (light->cosHalfFovOuter <= 0.0 || light->cosHalfFovOuter >= 1.0)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_spotshadow.cpp",
          61,
          0,
          "%s\n\t(light->cosHalfFovOuter) = %g",
          "(light->cosHalfFovOuter > 0.0f && light->cosHalfFovOuter < 1.0f)",
          light->cosHalfFovOuter) )
  {
    __debugbreak();
  }
  cosHalfFovOuter = light->cosHalfFovOuter;
  if ( (float)(cosHalfFovOuter - (float)(1.0 - 0.001)) < 0.0 )
    v6 = light->cosHalfFovOuter;
  else
    v6 = 1.0 - 0.001;
  if ( (float)(0.001 - cosHalfFovOuter) >= 0.0 )
    v6 = FLOAT_0_001;
  tanHalfFovX = fsqrt(1.0 - (float)(v6 * v6)) / v6;
  FinitePerspectiveMatrix(tanHalfFovX, tanHalfFovX, nearPlaneBias + 1.0, light->radius, viewParms->projectionMatrix.m);
  R_SetupViewProjectionMatrices(viewParms, 0);
  viewParms->depthHackNearClip = viewParms->projectionMatrix.m[3][2];
}

void __cdecl R_GetSpotShadowLookupMatrix(
        const GfxViewParms *shadowViewParms,
        int usingGridTiling,
        int usingFullShadowBuffer,
        unsigned int spotShadowIndex,
        unsigned int tileCount,
        GfxMatrix *lookupMatrix)
{
  float x0; // [esp+34h] [ebp-20h]
  float xScale; // [esp+38h] [ebp-1Ch]
  float x1; // [esp+3Ch] [ebp-18h]
  float xShift; // [esp+40h] [ebp-14h]
  float yShift; // [esp+44h] [ebp-10h]
  float y1; // [esp+48h] [ebp-Ch]
  float y0; // [esp+4Ch] [ebp-8h]
  float yScale; // [esp+50h] [ebp-4h]

  if ( !tileCount
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_spotshadow.cpp", 83, 0, "%s", "tileCount") )
  {
    __debugbreak();
  }
  if ( usingGridTiling )
  {
    if ( usingFullShadowBuffer )
    {
      x0 = *(float *)&FLOAT_0_0;
      x1 = FLOAT_1_0;
      y1 = *(float *)&FLOAT_0_0;
      y0 = FLOAT_1_0;
    }
    else
    {
      x0 = (double)(spotShadowIndex - 2 * (spotShadowIndex >> 1)) * 0.5;
      x1 = x0 + 0.5;
      y1 = (double)(spotShadowIndex >> 1) * 0.5;
      y0 = y1 + 0.5;
    }
  }
  else
  {
    x0 = *(float *)&FLOAT_0_0;
    x1 = FLOAT_1_0;
    y1 = (double)spotShadowIndex * (1.0 / (double)tileCount);
    y0 = 1.0 / (double)tileCount + y1;
  }
  xScale = (float)(x1 - x0) * 0.5;
  xShift = (float)(x1 + x0) * 0.5;
  yScale = (float)(y1 - y0) * 0.5;
  yShift = (float)(y1 + y0) * 0.5;
  lookupMatrix->m[0][0] = (float)(shadowViewParms->viewProjectionMatrix.m[0][0] * xScale)
                        + (float)(shadowViewParms->viewProjectionMatrix.m[0][3] * xShift);
  lookupMatrix->m[1][0] = (float)(shadowViewParms->viewProjectionMatrix.m[1][0] * xScale)
                        + (float)(shadowViewParms->viewProjectionMatrix.m[1][3] * xShift);
  lookupMatrix->m[2][0] = (float)(shadowViewParms->viewProjectionMatrix.m[2][0] * xScale)
                        + (float)(shadowViewParms->viewProjectionMatrix.m[2][3] * xShift);
  lookupMatrix->m[3][0] = (float)(shadowViewParms->viewProjectionMatrix.m[3][0] * xScale)
                        + (float)(shadowViewParms->viewProjectionMatrix.m[3][3] * xShift);
  lookupMatrix->m[0][1] = (float)(shadowViewParms->viewProjectionMatrix.m[0][1] * yScale)
                        + (float)(shadowViewParms->viewProjectionMatrix.m[0][3] * yShift);
  lookupMatrix->m[1][1] = (float)(shadowViewParms->viewProjectionMatrix.m[1][1] * yScale)
                        + (float)(shadowViewParms->viewProjectionMatrix.m[1][3] * yShift);
  lookupMatrix->m[2][1] = (float)(shadowViewParms->viewProjectionMatrix.m[2][1] * yScale)
                        + (float)(shadowViewParms->viewProjectionMatrix.m[2][3] * yShift);
  lookupMatrix->m[3][1] = (float)(shadowViewParms->viewProjectionMatrix.m[3][1] * yScale)
                        + (float)(shadowViewParms->viewProjectionMatrix.m[3][3] * yShift);
  lookupMatrix->m[0][2] = shadowViewParms->viewProjectionMatrix.m[0][2];
  lookupMatrix->m[1][2] = shadowViewParms->viewProjectionMatrix.m[1][2];
  lookupMatrix->m[2][2] = shadowViewParms->viewProjectionMatrix.m[2][2];
  lookupMatrix->m[3][2] = shadowViewParms->viewProjectionMatrix.m[3][2];
  lookupMatrix->m[0][3] = shadowViewParms->viewProjectionMatrix.m[0][3];
  lookupMatrix->m[1][3] = shadowViewParms->viewProjectionMatrix.m[1][3];
  lookupMatrix->m[2][3] = shadowViewParms->viewProjectionMatrix.m[2][3];
  lookupMatrix->m[3][3] = shadowViewParms->viewProjectionMatrix.m[3][3];
}

void __cdecl R_AddSpotShadowModelEntities(int localClientNum, unsigned int primaryLightIndex, const GfxLight *light)
{
  const GfxEntity *gfxEnt; // [esp+4h] [ebp-18h]
  volatile int sceneEntIndex; // [esp+8h] [ebp-14h]
  volatile int sceneEntIndexa; // [esp+8h] [ebp-14h]
  unsigned int entnum; // [esp+10h] [ebp-Ch]
  unsigned int entnuma; // [esp+10h] [ebp-Ch]
  GfxSpotShadowEntCmd cmd; // [esp+14h] [ebp-8h] BYREF

  gfxEnt = 0;
  cmd.light = light;
  for ( sceneEntIndex = 0; sceneEntIndex < scene.sceneDObjCount; ++sceneEntIndex )
  {
    cmd.sceneEnt = &scene.sceneDObj[sceneEntIndex];
    if ( cmd.sceneEnt->gfxEntIndex )
    {
      gfxEnt = &frontEndDataOut->gfxEnts[cmd.sceneEnt->gfxEntIndex];
      if ( (gfxEnt->renderFxFlags & 1) != 0 )
        continue;
    }
    entnum = cmd.sceneEnt->entnum;
    if ( entnum == gfxCfg.entnumNone || !R_IsEntityVisibleToPrimaryLight(localClientNum, entnum, primaryLightIndex) )
    {
      if ( gfxEnt )
      {
        if ( (gfxEnt->renderFxFlags & 0x20) != 0 )
          Sys_AddWorkerCmdInternal(&r_spot_shadow_entWorkerCmd, (unsigned __int8 *)&cmd, 0);
      }
    }
    else
    {
      Sys_AddWorkerCmdInternal(&r_spot_shadow_entWorkerCmd, (unsigned __int8 *)&cmd, 0);
    }
  }
  for ( sceneEntIndexa = 0; sceneEntIndexa < scene.sceneModelCount; ++sceneEntIndexa )
  {
    if ( !scene.sceneModel[sceneEntIndexa].gfxEntIndex
      || (frontEndDataOut->gfxEnts[scene.sceneModel[sceneEntIndexa].gfxEntIndex].renderFxFlags & 1) == 0 )
    {
      entnuma = scene.sceneModel[sceneEntIndexa].entnum;
      if ( entnuma != gfxCfg.entnumNone )
      {
        if ( R_IsEntityVisibleToPrimaryLight(localClientNum, entnuma, primaryLightIndex) )
          *(_BYTE *)(scene.dynSModelVisBitsCamera[light->spotShadowIndex - 10] + entnuma) = 1;
      }
    }
  }
}

void __cdecl R_GenerateAllSortedSpotShadowDrawSurfs(GfxViewInfo *viewInfo, int viewIndex)
{
  unsigned int spotShadowIndex; // [esp+4h] [ebp-4h]

  for ( spotShadowIndex = 0; spotShadowIndex < frontEndDataOut->spotShadowCount; ++spotShadowIndex )
  {
    if ( R_IsPrimaryLight(frontEndDataOut->spotShadows[spotShadowIndex].shadowableLightIndex) )
      R_GenerateSortedPrimarySpotShadowDrawSurfs(
        viewInfo,
        viewIndex,
        spotShadowIndex,
        frontEndDataOut->spotShadows[spotShadowIndex].shadowableLightIndex);
  }
  R_EmitSpotShadowMapSurfs(viewInfo);
}

void __cdecl R_GenerateSortedPrimarySpotShadowDrawSurfs(
        const GfxViewInfo *viewInfo,
        int viewIndex,
        unsigned int spotShadowIndex,
        unsigned int shadowableLightIndex)
{
  unsigned int PrimaryLightCount; // eax

  if ( shadowableLightIndex >= Com_GetPrimaryLightCount() )
  {
    PrimaryLightCount = Com_GetPrimaryLightCount();
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_spotshadow.cpp",
            435,
            0,
            "shadowableLightIndex doesn't index Com_GetPrimaryLightCount()\n\t%i not in [0, %i)",
            shadowableLightIndex,
            PrimaryLightCount) )
      __debugbreak();
  }
  //PIXBeginNamedEvent(-1, "bsp surfaces");
  R_AddAllBspDrawSurfacesSpotShadow(spotShadowIndex, shadowableLightIndex);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  //PIXBeginNamedEvent(-1, "static model surfaces");
  R_AddAllStaticModelSurfacesSpotShadow(viewIndex, spotShadowIndex, shadowableLightIndex);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  //PIXBeginNamedEvent(-1, "scene ent surfaces");
  R_AddAllSceneEntSurfacesSpotShadow(viewInfo, spotShadowIndex, shadowableLightIndex);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
}

void __cdecl R_EmitSpotShadowMapSurfs(GfxViewInfo *viewInfo)
{
  float *origin; // [esp+Ch] [ebp-14h]
  int firstDrawSurf; // [esp+10h] [ebp-10h]
  GfxDrawSurfListInfo *info; // [esp+14h] [ebp-Ch]
  GfxSpotShadow *spotShadow; // [esp+18h] [ebp-8h]
  unsigned int spotShadowIndex; // [esp+1Ch] [ebp-4h]

  //PIXBeginNamedEvent(-1, "emit spot shadow map surfs");
  for ( spotShadowIndex = 0; spotShadowIndex < frontEndDataOut->spotShadowCount; ++spotShadowIndex )
  {
    spotShadow = &frontEndDataOut->spotShadows[spotShadowIndex];
    info = &viewInfo->drawList[spotShadowIndex + 10];
    R_InitDrawSurfListInfo(info);
    info->baseTechType = gfxMetrics.shadowmapBuildTechType;
    info->viewInfo = viewInfo;
    origin = spotShadow->light->origin;
    info->viewOrigin[0] = *origin;
    info->viewOrigin[1] = origin[1];
    info->viewOrigin[2] = origin[2];
    info->viewOrigin[3] = FLOAT_1_0;
    if ( viewInfo->drawList[spotShadowIndex + 10].cameraView
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_spotshadow.cpp",
            479,
            0,
            "%s",
            "!info->cameraView") )
    {
      __debugbreak();
    }
    firstDrawSurf = frontEndDataOut->drawSurfCount;
    if ( R_IsPrimaryLight(spotShadow->shadowableLightIndex) )
    {
      R_MergeAndEmitDrawSurfLists(3 * spotShadowIndex + 27, 1u, frontEndDataOut);
      R_MergeAndEmitDrawSurfLists(3 * spotShadowIndex + 28, 2u, frontEndDataOut);
      info->drawSurfs = &frontEndDataOut->drawSurfs[firstDrawSurf];
      viewInfo->drawList[spotShadowIndex + 10].drawSurfCount = frontEndDataOut->drawSurfCount - firstDrawSurf;
    }
    else
    {
      if ( !R_IsDynamicShadowedLight(spotShadow->shadowableLightIndex)
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_spotshadow.cpp",
              495,
              0,
              "%s",
              "R_IsDynamicShadowedLight( spotShadow->shadowableLightIndex )") )
      {
        __debugbreak();
      }
      if ( frontEndDataOut->emissiveSpotLightCount == 1 )
        R_EmitShadowedLightPartitionSurfs(
          viewInfo,
          frontEndDataOut->emissiveSpotDrawSurfCount,
          frontEndDataOut->emissiveSpotDrawSurfs,
          info);
    }
  }
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
}

