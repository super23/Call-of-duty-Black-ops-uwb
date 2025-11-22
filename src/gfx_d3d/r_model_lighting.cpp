#include "r_model_lighting.h"

GfxModelLightingPatch *__cdecl R_BackEndDataAllocAndClearModelLightingPatch(GfxBackEndData *frontEndDataOut)
{
  unsigned int patchIndex; // [esp+4h] [ebp-4h]

  patchIndex = _InterlockedExchangeAdd(&frontEndDataOut->modelLightingPatchCount, 1u);
  if ( patchIndex >= 0x1000 )
    Com_Error(ERR_FATAL, "modelLightingPatchList ran out of elements.");
  memset(
    (unsigned __int8 *)&frontEndDataOut->modelLightingPatchList[patchIndex],
    0,
    sizeof(frontEndDataOut->modelLightingPatchList[patchIndex]));
  return &frontEndDataOut->modelLightingPatchList[patchIndex];
}

void __cdecl R_SetModelLightingCoords(unsigned __int16 handle, float *out)
{
  unsigned int entryIndex; // [esp+14h] [ebp-18h]
  float yCoord; // [esp+18h] [ebp-14h]
  float xCoord; // [esp+28h] [ebp-4h]

  entryIndex = R_ModelLightingIndexFromHandle(handle);
  xCoord = ((double)(4 * (entryIndex & 0x3F)) + 2.0) * 0.00390625;
  yCoord = ((double)((entryIndex >> 4) & 0xFFFFFFFC) + 2.0) * modelLightGlob.invImageHeight;
  *out = xCoord;
  out[1] = yCoord;
  out[2] = FLOAT_0_5;
  out[3] = FLOAT_1_0;
}

unsigned int __cdecl R_ModelLightingIndexFromHandle(unsigned __int16 handle)
{
  if ( (!handle || handle > modelLightGlob.totalEntryLimit)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp",
          164,
          0,
          "handle not in [1, modelLightGlob.totalEntryLimit]\n\t%i not in [%i, %i]",
          handle,
          1,
          modelLightGlob.totalEntryLimit) )
  {
    __debugbreak();
  }
  return handle - 1;
}

char __cdecl R_AllocStaticModelLighting(const GfxStaticModelDrawInst *smodelDrawInst, unsigned int smodelIndex)
{
  unsigned __int16 handle; // [esp+0h] [ebp-10h]
  unsigned __int16 handlea; // [esp+0h] [ebp-10h]
  unsigned int smodelIndexPrev; // [esp+4h] [ebp-Ch]
  unsigned int entryIndex; // [esp+8h] [ebp-8h]

  if ( !rgp.world
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp", 229, 0, "%s", "rgp.world") )
  {
    __debugbreak();
  }
  handle = smodelDrawInst->lightingHandle;
  if ( handle )
  {
    entryIndex = R_ModelLightingIndexFromHandle(handle);
    if ( !r_cacheSModelLighting->current.enabled )
    {
      if ( smodelIndex >> 5 >= 0x800
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp",
              245,
              0,
              "smodelIndex >> 5 doesn't index ARRAY_COUNT( smodelLightGlob.lightingBits )\n\t%i not in [0, %i)",
              smodelIndex >> 5,
              2048) )
      {
        __debugbreak();
      }
      smodelLightGlob.lightingBits[smodelIndex >> 5] |= 0x80000000 >> (smodelIndex & 0x1F);
      smodelLightGlob.local.anyNewLighting = 1;
    }
  }
  else
  {
    if ( smodelLightGlob.local.assignedCount >= smodelLightGlob.local.entryLimit )
    {
      do
      {
        if ( !smodelLightGlob.local.freeableCount )
          return 0;
        handlea = smodelLightGlob.freeableHandles[--smodelLightGlob.local.freeableCount];
        entryIndex = R_ModelLightingIndexFromHandle(handlea);
      }
      while ( smodelLightGlob.local.frameCount == smodelLightGlob.local.usedFrameCount[entryIndex] );
      smodelIndexPrev = smodelLightGlob.local.smodelIndex[entryIndex];
      rgp.world->dpvs.smodelDrawInsts[smodelIndexPrev].lightingHandle = 0;
      R_UncacheStaticModel(smodelIndexPrev);
    }
    else
    {
      entryIndex = smodelLightGlob.local.assignedCount++;
      handlea = entryIndex + 1;
    }
    if ( !handlea
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp", 287, 0, "%s", "handle") )
    {
      __debugbreak();
    }
    smodelDrawInst->lightingHandle = handlea;
    smodelLightGlob.local.smodelIndex[entryIndex] = truncate_cast<unsigned short>(smodelIndex);
    if ( smodelIndex >> 5 >= 0x800
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp",
            296,
            0,
            "smodelIndex >> 5 doesn't index ARRAY_COUNT( smodelLightGlob.lightingBits )\n\t%i not in [0, %i)",
            smodelIndex >> 5,
            2048) )
    {
      __debugbreak();
    }
    smodelLightGlob.lightingBits[smodelIndex >> 5] |= 0x80000000 >> (smodelIndex & 0x1F);
    smodelLightGlob.local.anyNewLighting = 1;
  }
  smodelLightGlob.local.usedFrameCount[entryIndex] = smodelLightGlob.local.frameCount;
  return 1;
}

unsigned int __cdecl R_AllocModelLighting_DynEnts(
        float *lightingOrigin,
        unsigned int dynEntId,
        bool useHeroLighting,
        unsigned __int16 *cachedLightingHandle,
        GfxLightingInfo *lightingInfoOut)
{
  return R_AllocModelLighting(
           lightingOrigin,
           0.0,
           useHeroLighting,
           cachedLightingHandle,
           (unsigned int (__cdecl *)(const void *))R_DynEntPrimaryLightCallback,
           &dynEntId,
           lightingInfoOut);
}

unsigned int __cdecl R_AllocModelLighting(
        float *lightingOrigin,
        float lightingOriginToleranceSq,
        bool useHeroLighting,
        unsigned __int16 *cachedLightingHandle,
        unsigned int (__cdecl *GetPrimaryLightCallback)(const void *),
        const void *userData,
        GfxLightingInfo *lightingInfoOut)
{
  unsigned int v7; // ecx
  int v10; // eax
  unsigned int v11; // ecx
  unsigned int v12; // edx
  GfxLightingInfo *lightingInfo; // esi
  int v14; // [esp+8h] [ebp-80h]
  float *v15; // [esp+Ch] [ebp-7Ch]
  float *v16; // [esp+20h] [ebp-68h]
  float distMovedSq; // [esp+24h] [ebp-64h]
  bool lightingOriginIsStationary; // [esp+28h] [ebp-60h]
  float boxMaxs[3]; // [esp+2Ch] [ebp-5Ch] BYREF
  float boxMins[3]; // [esp+38h] [ebp-50h] BYREF
  unsigned int pixelFreeRover; // [esp+44h] [ebp-44h]
  unsigned int entryIndex; // [esp+48h] [ebp-40h]
  unsigned int usedCount; // [esp+4Ch] [ebp-3Ch]
  unsigned int usedIndex; // [esp+50h] [ebp-38h]
  unsigned __int16 lightingHandle; // [esp+54h] [ebp-34h]
  CalcLightingCmd cmd; // [esp+58h] [ebp-30h] BYREF
  unsigned int nonSunPrimaryLightIndex; // [esp+84h] [ebp-4h]

  if ( r_showLightingOrigins->current.enabled )
  {
    boxMins[0] = *lightingOrigin + -1.0;
    boxMins[1] = lightingOrigin[1] + -1.0;
    boxMins[2] = lightingOrigin[2] + -1.0;
    boxMaxs[0] = *lightingOrigin + 1.0;
    boxMaxs[1] = lightingOrigin[1] + 1.0;
    boxMaxs[2] = lightingOrigin[2] + 1.0;
    R_AddDebugBox(&frontEndDataOut->debugGlobals, boxMins, boxMaxs, colorGreen);
  }
  if ( !cachedLightingHandle
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp",
          339,
          0,
          "%s",
          "cachedLightingHandle") )
  {
    __debugbreak();
  }
  lightingHandle = *cachedLightingHandle;
  if ( lightingHandle
    && r_cacheModelLighting->current.enabled
    && ((entryIndex = R_ModelLightingIndexFromHandle(lightingHandle),
         usedIndex = entryIndex - modelLightGlob.xmodel.baseIndex,
         lightingOriginToleranceSq > 0.0)
      ? (distMovedSq = Vec3DistanceSq(lightingOrigin, modelLightGlob.lightingOrigins[usedIndex]),
         lightingOriginIsStationary = lightingOriginToleranceSq > distMovedSq)
      : ((v16 = modelLightGlob.lightingOrigins[usedIndex], *lightingOrigin != *v16)
      || lightingOrigin[1] != v16[1]
      || lightingOrigin[2] != v16[2]
       ? (v14 = 0)
       : (v14 = 1),
         lightingOriginIsStationary = v14),
        lightingOriginIsStationary) )
  {
    modelLightGlob.currPixelFreeBits[usedIndex >> 5] &= ~(0x80000000 >> (usedIndex & 0x1F));
    if ( modelLightGlob.lightingInfo[usedIndex].primaryLightIndex == 255 )
      Sys_WaitWorkerCmdInternal(&r_model_lightingWorkerCmd);
    if ( modelLightGlob.lightingInfo[usedIndex].primaryLightIndex >= rgp.world->primaryLightCount
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp",
            371,
            0,
            "modelLightGlob.lightingInfo[usedIndex].primaryLightIndex doesn't index rgp.world->primaryLightCount\n"
            "\t%i not in [0, %i)",
            modelLightGlob.lightingInfo[usedIndex].primaryLightIndex,
            rgp.world->primaryLightCount) )
    {
      __debugbreak();
    }
    v7 = modelLightGlob.lightingInfo[usedIndex].lightingHandle;
    *(unsigned int *)&lightingInfoOut->primaryLightIndex = *(unsigned int *)&modelLightGlob.lightingInfo[usedIndex].primaryLightIndex;
    lightingInfoOut->lightingHandle = v7;
    return lightingHandle;
  }
  else if ( modelLightGlob.allocModelFail )
  {
    return 0;
  }
  else
  {
    pixelFreeRover = modelLightGlob.pixelFreeRover;
    while ( 1 )
    {
      if ( !_BitScanReverse(
              (unsigned int *)&v10,
              modelLightGlob.prevPrevPixelFreeBits[pixelFreeRover]
            & modelLightGlob.prevPixelFreeBits[pixelFreeRover]
            & modelLightGlob.currPixelFreeBits[pixelFreeRover]) )
        v10 = `CountLeadingZeros'::`2'::notFound;
      usedCount = v10 ^ 0x1F;
      if ( (v10 ^ 0x1Fu) < 0x20 )
        break;
      pixelFreeRover = (pixelFreeRover + 1) % modelLightGlob.pixelFreeBitsWordCount;
      if ( pixelFreeRover == modelLightGlob.pixelFreeRover )
      {
        modelLightGlob.allocModelFail = 1;
        R_WarnOncePerFrame(R_WARN_MODEL_LIGHT_CACHE);
        return 0;
      }
    }
    modelLightGlob.pixelFreeRover = pixelFreeRover;
    modelLightGlob.currPixelFreeBits[pixelFreeRover] &= ~(0x80000000 >> usedCount);
    usedIndex = usedCount + 32 * pixelFreeRover;
    if ( usedIndex >= modelLightGlob.xmodelEntryLimit
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp",
            401,
            0,
            "usedIndex doesn't index modelLightGlob.xmodelEntryLimit\n\t%i not in [0, %i)",
            usedIndex,
            modelLightGlob.xmodelEntryLimit) )
    {
      __debugbreak();
    }
    entryIndex = usedIndex + modelLightGlob.xmodel.baseIndex;
    lightingHandle = usedIndex + LOWORD(modelLightGlob.xmodel.baseIndex) + 1;
    v15 = modelLightGlob.lightingOrigins[usedIndex];
    *v15 = *lightingOrigin;
    v15[1] = lightingOrigin[1];
    v15[2] = lightingOrigin[2];
    *cachedLightingHandle = lightingHandle;
    lightingInfoOut->reflectionProbeIndex = R_CalcReflectionProbeIndex(lightingOrigin);
    nonSunPrimaryLightIndex = GetPrimaryLightCallback(userData);
    lightingInfoOut->lightingHandle = lightingHandle;
    v11 = lightingInfoOut->lightingHandle;
    v12 = usedIndex;
    lightingInfo = modelLightGlob.lightingInfo;
    *(unsigned int *)&modelLightGlob.lightingInfo[usedIndex].primaryLightIndex = *(unsigned int *)&lightingInfoOut->primaryLightIndex;
    lightingInfo[v12].lightingHandle = v11;
    modelLightGlob.lightingInfo[usedIndex].primaryLightIndex = -1;
    cmd.entryIndex = entryIndex;
    cmd.lightingOrigin[0] = *lightingOrigin;
    cmd.lightingOrigin[1] = lightingOrigin[1];
    cmd.lightingOrigin[2] = lightingOrigin[2];
    cmd.nonSunPrimaryLightIndex = nonSunPrimaryLightIndex;
    cmd.extrapolateBehavior = GFX_MODELLIGHT_SHOW_MISSING;
    cmd.useHeroLighting = useHeroLighting;
    cmd.primaryLightIndex1 = &lightingInfoOut->primaryLightIndex;
    cmd.primaryLightIndex2 = &modelLightGlob.lightingInfo[usedIndex].primaryLightIndex;
    cmd.pixels = modelLightGlob.image->pixels;
    Sys_AddWorkerCmdInternal(&r_model_lightingWorkerCmd, (unsigned __int8 *)&cmd, 0);
    return lightingHandle;
  }
}

unsigned int __cdecl R_DynEntPrimaryLightCallback(unsigned int *userData)
{
  return rgp.world->nonSunPrimaryLightForModelDynEnt[*userData];
}

unsigned int __cdecl R_AllocModelLighting_PrimaryLight(
        float *lightingOrigin,
        unsigned int primaryLightIndex,
        bool useHeroLighting,
        unsigned __int16 *cachedLightingHandle,
        GfxLightingInfo *lightingInfoOut)
{
  return R_AllocModelLighting(
           lightingOrigin,
           0.0,
           useHeroLighting,
           cachedLightingHandle,
           R_PassbackPrimaryLightCallback,
           &primaryLightIndex,
           lightingInfoOut);
}

unsigned int __cdecl R_PassbackPrimaryLightCallback(const void *userData)
{
  return *(unsigned int *)userData;
}

unsigned int __cdecl R_AllocModelLighting_Box(
        const GfxViewInfo *viewInfo,
        float *lightingOrigin,
        float originToleranceSq,
        const float *boxMins,
        const float *boxMaxs,
        bool useHeroLighting,
        unsigned __int16 *cachedLightingHandle,
        GfxLightingInfo *lightingInfoOut)
{
  GfxFindLightForBox boxWork; // [esp+18h] [ebp-1Ch] BYREF

  boxWork.viewInfo = viewInfo;
  boxWork.midPoint[0] = (float)(*boxMins + *boxMaxs) * 0.5;
  boxWork.midPoint[1] = (float)(boxMins[1] + boxMaxs[1]) * 0.5;
  boxWork.midPoint[2] = (float)(boxMins[2] + boxMaxs[2]) * 0.5;
  boxWork.halfSize[0] = boxWork.midPoint[0] - *boxMins;
  boxWork.halfSize[1] = boxWork.midPoint[1] - boxMins[1];
  boxWork.halfSize[2] = boxWork.midPoint[2] - boxMins[2];
  return R_AllocModelLighting(
           lightingOrigin,
           originToleranceSq,
           useHeroLighting,
           cachedLightingHandle,
           R_GetPrimaryLightForBoxCallback,
           &boxWork,
           lightingInfoOut);
}

unsigned int __cdecl R_GetPrimaryLightForBoxCallback(const void *userData)
{
  return R_GetNonSunPrimaryLightForBox(
           *(const GfxViewInfo **)userData,
           (const float *)userData + 1,
           (const float *)userData + 4);
}

unsigned int __cdecl R_AllocModelLighting_Sphere(
        const GfxViewInfo *viewInfo,
        float *lightingOrigin,
        float originToleranceSq,
        const float *origin,
        float radius,
        bool useHeroLighting,
        unsigned __int16 *cachedLightingHandle,
        GfxLightingInfo *lightingInfoOut)
{
  GfxFindLightForSphere sphereWork; // [esp+18h] [ebp-14h] BYREF

  sphereWork.viewInfo = viewInfo;
  sphereWork.origin[0] = *origin;
  sphereWork.origin[1] = origin[1];
  sphereWork.origin[2] = origin[2];
  sphereWork.radius = radius;
  return R_AllocModelLighting(
           lightingOrigin,
           originToleranceSq,
           useHeroLighting,
           cachedLightingHandle,
           R_GetPrimaryLightForSphereCallback,
           &sphereWork,
           lightingInfoOut);
}

unsigned int __cdecl R_GetPrimaryLightForSphereCallback(const void *userData)
{
  return R_GetNonSunPrimaryLightForSphere(
           *(const GfxViewInfo **)userData,
           (const float *)userData + 1,
           *((float *)userData + 4));
}

void __cdecl R_ToggleModelLightingFrame()
{
  unsigned int entryIndex; // [esp+0h] [ebp-4h]

  ++smodelLightGlob.local.frameCount;
  modelLightGlob.modFrameCount = (modelLightGlob.modFrameCount + 1) % 4;
  modelLightGlob.allocModelFail = 0;
  modelLightGlob.prevPrevPixelFreeBits = modelLightGlob.pixelFreeBits[(modelLightGlob.modFrameCount + 2) % 4];
  modelLightGlob.prevPixelFreeBits = modelLightGlob.pixelFreeBits[(modelLightGlob.modFrameCount + 3) % 4];
  modelLightGlob.currPixelFreeBits = modelLightGlob.pixelFreeBits[modelLightGlob.modFrameCount % 4];
  Com_Memset(modelLightGlob.currPixelFreeBits, 255, modelLightGlob.pixelFreeBitsSize);
  smodelLightGlob.local.freeableCount = 0;
  for ( entryIndex = 0; entryIndex < smodelLightGlob.local.assignedCount; ++entryIndex )
  {
    if ( smodelLightGlob.local.frameCount - smodelLightGlob.local.usedFrameCount[entryIndex] >= 4 )
      smodelLightGlob.freeableHandles[smodelLightGlob.local.freeableCount++] = entryIndex + 1;
  }
}

void __cdecl R_CalcModelLightingCmd(CalcLightingCmd *data)
{
  R_CalcModelLighting(
    data->entryIndex,
    data->lightingOrigin,
    data->nonSunPrimaryLightIndex,
    data->extrapolateBehavior,
    data->useHeroLighting,
    data->primaryLightIndex1,
    data->primaryLightIndex2);
}

void __cdecl R_CalcModelLighting(
        unsigned int entryIndex,
        const float *lightingOrigin,
        unsigned int nonSunPrimaryLightIndex,
        GfxModelLightExtrapolation extrapolateBehavior,
        bool useHeroLighting,
        unsigned __int8 *primaryLightIndex1,
        unsigned __int8 *primaryLightIndex2)
{
  unsigned __int16 dest; // [esp+Ch] [ebp-8h]
  unsigned __int8 lightingAtPoint; // [esp+13h] [ebp-1h]

  //PIXBeginNamedEvent(-1, "model lighting");
  dest = truncate_cast<unsigned short>(entryIndex);
  if ( nonSunPrimaryLightIndex >= rgp.world->primaryLightCount
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp",
          808,
          0,
          "nonSunPrimaryLightIndex doesn't index rgp.world->primaryLightCount\n\t%i not in [0, %i)",
          nonSunPrimaryLightIndex,
          rgp.world->primaryLightCount) )
  {
    __debugbreak();
  }
  lightingAtPoint = R_GetLightingAtPoint(
                      &rgp.world->lightGrid,
                      lightingOrigin,
                      nonSunPrimaryLightIndex,
                      dest,
                      extrapolateBehavior,
                      useHeroLighting);
  *primaryLightIndex1 = lightingAtPoint;
  if ( primaryLightIndex2 )
    *primaryLightIndex2 = lightingAtPoint;
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
}

void __cdecl R_BeginAllStaticModelLighting()
{
  unsigned int size; // [esp+0h] [ebp-4h]

  if ( smodelLightGlob.local.anyNewLighting
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp",
          904,
          0,
          "%s",
          "!smodelLightGlob.local.anyNewLighting") )
  {
    __debugbreak();
  }
  size = 4 * ((rgp.world->dpvs.smodelCount + 31) >> 5);
  if ( size >= 0x2000
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp",
          908,
          0,
          "%s",
          "size < sizeof( smodelLightGlob.lightingBits )") )
  {
    __debugbreak();
  }
  Com_Memset(smodelLightGlob.lightingBits, 0, size);
}

void __cdecl R_SetAllStaticModelLighting()
{
  int v1; // eax
  unsigned int wordCount; // [esp+14h] [ebp-1Ch]
  unsigned int bits; // [esp+18h] [ebp-18h]
  unsigned int indexLow; // [esp+20h] [ebp-10h]
  unsigned int wordIndex; // [esp+28h] [ebp-8h]

  //PIXBeginNamedEvent(-1, "R_SetAllStaticModelLighting");
  if ( !smodelLightGlob.local.anyNewLighting )
  {
    if ( g_DXDeviceThread != GetCurrentThreadId() )
      return;
    goto LABEL_20;
  }
  smodelLightGlob.local.anyNewLighting = 0;
  wordCount = (rgp.world->dpvs.smodelCount + 31) >> 5;
  if ( wordCount >= 0x2000
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp",
          931,
          0,
          "%s",
          "wordCount < sizeof( smodelLightGlob.lightingBits )") )
  {
    __debugbreak();
  }
  for ( wordIndex = 0; wordIndex < wordCount; ++wordIndex )
  {
    bits = smodelLightGlob.lightingBits[wordIndex];
    if ( bits )
    {
      while ( 1 )
      {
        if ( !_BitScanReverse((unsigned int *)&v1, bits) )
          v1 = `CountLeadingZeros'::`2'::notFound;
        indexLow = v1 ^ 0x1F;
        if ( (v1 ^ 0x1Fu) >= 0x20 )
          break;
        if ( ((0x80000000 >> indexLow) & bits) == 0
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp",
                948,
                0,
                "%s",
                "bits & bit") )
        {
          __debugbreak();
        }
        bits &= ~(0x80000000 >> indexLow);
        R_SetStaticModelLighting(indexLow + 32 * wordIndex);
      }
    }
  }
  if ( g_DXDeviceThread == GetCurrentThreadId() )
LABEL_20:
    D3DPERF_EndEvent();
}

void __cdecl R_SetStaticModelLighting(unsigned int smodelIndex)
{
  unsigned __int8 primaryLight; // [esp+2h] [ebp-Eh]
  unsigned int entryIndex; // [esp+4h] [ebp-Ch]
  GfxStaticModelDrawInst *smodelDrawInst; // [esp+8h] [ebp-8h]
  const GfxStaticModelInst *smodelInst; // [esp+Ch] [ebp-4h]

  if ( smodelIndex >= rgp.world->dpvs.smodelCount
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp",
          862,
          0,
          "smodelIndex doesn't index rgp.world->dpvs.smodelCount\n\t%i not in [0, %i)",
          smodelIndex,
          rgp.world->dpvs.smodelCount) )
  {
    __debugbreak();
  }
  smodelInst = &rgp.world->dpvs.smodelInsts[smodelIndex];
  smodelDrawInst = &rgp.world->dpvs.smodelDrawInsts[smodelIndex];
  entryIndex = R_ModelLightingIndexFromHandle(smodelDrawInst->lightingHandle);
  if ( smodelInst->groundLighting.packed )
  {
    R_SetModelGroundLighting(entryIndex, (const unsigned __int8 *)&smodelInst->groundLighting);
  }
  else
  {
    primaryLight = smodelDrawInst->primaryLightIndex;
    R_CalcModelLighting(
      entryIndex,
      smodelInst->lightingOrigin,
      primaryLight,
      GFX_MODELLIGHT_EXTRAPOLATE,
      0,
      &smodelDrawInst->primaryLightIndex,
      0);
    smodelDrawInst->primaryLightIndex = primaryLight;
  }
}

void __cdecl R_SetModelGroundLighting(unsigned int entryIndex, const unsigned __int8 *groundLighting)
{
  GfxModelLightingPatch *patch; // [esp+8h] [ebp-4h]

  patch = R_BackEndDataAllocAndClearModelLightingPatch(frontEndDataOut);
  patch->modelLightingIndex = truncate_cast<unsigned short>(entryIndex);
  if ( patch->colorsCount
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp",
          850,
          1,
          "%s",
          "patch->colorsCount == 0") )
  {
    __debugbreak();
  }
  *(unsigned int *)patch->groundLighting = *(unsigned int *)groundLighting;
  patch->useHeroLighting = 0;
}

void __cdecl R_SetStaticModelLightingCoordsForSource(unsigned int smodelIndex, GfxCmdBufSourceState *source)
{
  R_SetStaticModelLightingCoords(smodelIndex, source->input.consts[78]);
  ++source->constVersions[102];
}

void __cdecl R_SetStaticModelLightingCoords(unsigned int smodelIndex, float *out)
{
  R_SetModelLightingCoords(rgp.world->dpvs.smodelDrawInsts[smodelIndex].lightingHandle, out);
}

void __cdecl R_GetPackedStaticModelLightingCoords(unsigned int smodelIndex, PackedLightingCoords *packedCoords)
{
  unsigned int entryIndex; // [esp+0h] [ebp-10h]

  entryIndex = R_ModelLightingIndexFromHandle(rgp.world->dpvs.smodelDrawInsts[smodelIndex].lightingHandle);
  if ( (modelLightGlob.imageHeight & (modelLightGlob.imageHeight - 1)) != 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp",
          994,
          0,
          "%s",
          "IsPowerOf2( modelLightGlob.imageHeight )") )
  {
    __debugbreak();
  }
  if ( modelLightGlob.imageHeight > 0x100
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp",
          995,
          0,
          "modelLightGlob.imageHeight <= GFX_ML_IMAGE_WIDTH\n\t%i, %i",
          modelLightGlob.imageHeight,
          256) )
  {
    __debugbreak();
  }
  packedCoords->array[0] = truncate_cast<unsigned char>(4 * (entryIndex & 0x3F) + 2);
  packedCoords->array[1] = truncate_cast<unsigned char>(((entryIndex >> 4) & 0xFFFFFFFC) + 2
                                                                                         * (0x100
                                                                                          / modelLightGlob.imageHeight));
  packedCoords->array[2] = 0x80;
  packedCoords->array[3] = 0;
}

bool __cdecl R_StaticModelHasLighting(unsigned int smodelIndex)
{
  return rgp.world->dpvs.smodelDrawInsts[smodelIndex].lightingHandle != 0;
}

void __cdecl RB_PatchModelLighting(const GfxModelLightingPatch *patchList, unsigned int patchCount)
{
  unsigned __int8 *pixels; // [esp+Ch] [ebp-14h]
  unsigned int sampleIndex; // [esp+10h] [ebp-10h]
  unsigned int patchIter; // [esp+14h] [ebp-Ch]
  const GfxModelLightingPatch *patch; // [esp+18h] [ebp-8h]

  if ( patchCount )
  {
    if ( modelLightGlob.lockedBox.pBits
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp",
            1052,
            0,
            "%s",
            "modelLightGlob.lockedBox.pBits == NULL") )
    {
      __debugbreak();
    }
    ((void (__stdcall *)(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int))modelLightGlob.image->texture.basemap->__vftable[1].Release)(
      (GfxTexture)modelLightGlob.image->texture.basemap,
      0,
      &modelLightGlob.lockedBox,
      0,
      0);
    R_SetModelLightingSampleDeltas();
    R_SetLightGridSampleDeltas(modelLightGlob.lockedBox.RowPitch, modelLightGlob.lockedBox.SlicePitch);
    for ( patchIter = 0; patchIter < patchCount; ++patchIter )
    {
      patch = &patchList[patchIter];
      pixels = (unsigned __int8 *)modelLightGlob.lockedBox.pBits
             + 16 * (patch->modelLightingIndex & 0x3F)
             + modelLightGlob.lockedBox.RowPitch * ((patch->modelLightingIndex >> 4) & 0xFFFFFFFC);
      if ( patch->colorsCount )
      {
        R_ApplyLightGridColorsPatch(patch, pixels);
      }
      else
      {
        for ( sampleIndex = 0; sampleIndex < 0x40; ++sampleIndex )
          *(unsigned int *)&pixels[s_modelLightingSampleDelta[sampleIndex]] = *(unsigned int *)patch->groundLighting;
      }
    }
    if ( !modelLightGlob.lockedBox.pBits
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp",
            1074,
            0,
            "%s",
            "modelLightGlob.lockedBox.pBits") )
    {
      __debugbreak();
    }
    modelLightGlob.image->texture.basemap->__vftable[1].GetDevice(modelLightGlob.image->texture.basemap, 0);
    modelLightGlob.lockedBox.pBits = 0;
  }
}

unsigned int R_SetModelLightingSampleDeltas()
{
  unsigned int result; // eax
  unsigned int i; // [esp+0h] [ebp-10h]
  unsigned int sampleIndex; // [esp+4h] [ebp-Ch]
  unsigned int dz; // [esp+8h] [ebp-8h]
  unsigned int dy; // [esp+Ch] [ebp-4h]

  sampleIndex = 0;
  for ( dz = 0; dz < 4; ++dz )
  {
    for ( dy = 0; dy < 4; ++dy )
    {
      for ( i = 0; i < 4; ++i )
        s_modelLightingSampleDelta[sampleIndex++] = modelLightGlob.lockedBox.SlicePitch * dz
                                                  + modelLightGlob.lockedBox.RowPitch * dy
                                                  + 4 * i;
    }
    result = dz + 1;
  }
  return result;
}

void __cdecl R_SetModelLightingLookupScale(GfxCmdBufInput *input)
{
  float lookupScale[4]; // [esp+4h] [ebp-10h] BYREF

  lookupScale[0] = FLOAT_0_005859375;
  lookupScale[1] = 1.5 * modelLightGlob.invImageHeight;
  lookupScale[2] = FLOAT_0_375;
  lookupScale[3] = *(float *)&FLOAT_0_0;
  R_SetInputCodeConstantFromVec4(input, 0x35u, lookupScale);
}

void __cdecl R_SetupDynamicModelLighting(GfxCmdBufInput *input)
{
  GfxImage *image; // [esp+0h] [ebp-4h]

  image = modelLightGlob.image;
  if ( !input && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state.h", 1850, 0, "%s", "input") )
    __debugbreak();
  input->codeImages[3] = image;
  R_SetModelLightingLookupScale(input);
}

void __cdecl R_SetupCachedStaticModelLighting(GfxCmdBufSourceState *source)
{
  source->input.consts[78][0] = *(float *)&FLOAT_0_0;
  source->input.consts[78][1] = *(float *)&FLOAT_0_0;
  source->input.consts[78][2] = FLOAT_0_5;
  source->input.consts[78][3] = FLOAT_1_0;
  ++source->constVersions[102];
}

void __cdecl R_InitModelLightingGlobals()
{
  int v1; // eax
  unsigned int totalBitsNeeded; // [esp+Ch] [ebp-8h]
  unsigned int i; // [esp+10h] [ebp-4h]

  modelLightGlob.xmodelEntryLimit = gfxCfg.maxClientViews << 10;
  if ( !_BitScanReverse((unsigned int *)&v1, gfxCfg.maxClientViews << 10) )
    v1 = `CountLeadingZeros'::`2'::notFound;
  totalBitsNeeded = 32 - (v1 ^ 0x1F);
  for ( smodelLightGlob.local.entryLimit = (1 << (32 - (v1 ^ 0x1F))) - modelLightGlob.xmodelEntryLimit;
        smodelLightGlob.local.entryLimit < 0x800;
        smodelLightGlob.local.entryLimit += 1 << totalBitsNeeded++ )
  {
    ;
  }
  if ( smodelLightGlob.local.entryLimit > 0x1000
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp",
          1148,
          0,
          "%s\n\t(smodelLightGlob.local.entryLimit) = %i",
          "(smodelLightGlob.local.entryLimit <= 4096)",
          smodelLightGlob.local.entryLimit) )
  {
    __debugbreak();
  }
  modelLightGlob.totalEntryLimit = 1 << totalBitsNeeded;
  modelLightGlob.entryBitsY = totalBitsNeeded - 6;
  modelLightGlob.imageHeight = 1 << (totalBitsNeeded - 6 + 2);
  modelLightGlob.invImageHeight = 1.0 / (double)(unsigned int)(1 << (totalBitsNeeded - 6 + 2));
  modelLightGlob.xmodel.baseIndex = smodelLightGlob.local.entryLimit;
  if ( (modelLightGlob.xmodelEntryLimit & 0x1F) != 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp",
          1165,
          0,
          "%s\n\t(modelLightGlob.xmodelEntryLimit) = %i",
          "(!(modelLightGlob.xmodelEntryLimit & 31))",
          modelLightGlob.xmodelEntryLimit) )
  {
    __debugbreak();
  }
  modelLightGlob.pixelFreeBitsSize = modelLightGlob.xmodelEntryLimit >> 3;
  modelLightGlob.pixelFreeBitsWordCount = modelLightGlob.xmodelEntryLimit >> 5;
  modelLightGlob.lightingOrigins = (float (*)[3])R_AllocModelLightingGlobal(12 * modelLightGlob.xmodelEntryLimit);
  for ( i = 0; i < 4; ++i )
    modelLightGlob.pixelFreeBits[i] = (unsigned int *)R_AllocModelLightingGlobal(modelLightGlob.pixelFreeBitsSize);
  modelLightGlob.lightingInfo = (GfxLightingInfo *)R_AllocModelLightingGlobal(8 * modelLightGlob.xmodelEntryLimit);
}

char *__cdecl R_AllocModelLightingGlobal(unsigned int bytes)
{
  return Z_VirtualAlloc(bytes, "R_AllocModelLightingGlobal", 19);
}

void __cdecl R_ShutdownModelLightingGlobals()
{
  unsigned int i; // [esp+0h] [ebp-4h]

  for ( i = 0; i < 4; ++i )
    R_FreeGlobalVariable(modelLightGlob.pixelFreeBits[i]);
  R_FreeGlobalVariable(modelLightGlob.lightingOrigins);
  R_FreeGlobalVariable(modelLightGlob.lightingInfo);
}

void __cdecl R_ResetModelLighting()
{
  unsigned int v0; // eax
  float *v1; // [esp+0h] [ebp-14h]
  unsigned int entryIndex; // [esp+4h] [ebp-10h]
  unsigned int usedIndex; // [esp+8h] [ebp-Ch]
  unsigned int i; // [esp+Ch] [ebp-8h]
  unsigned int smodelIndex; // [esp+10h] [ebp-4h]

  for ( i = 0; i < 4; ++i )
    Com_Memset(modelLightGlob.pixelFreeBits[i], 255, modelLightGlob.pixelFreeBitsSize);
  for ( usedIndex = 0; usedIndex < modelLightGlob.xmodelEntryLimit; ++usedIndex )
  {
    v1 = modelLightGlob.lightingOrigins[usedIndex];
    *v1 = FLOAT_3_4028235e38;
    v1[1] = FLOAT_3_4028235e38;
    v1[2] = FLOAT_3_4028235e38;
  }
  if ( smodelLightGlob.local.assignedCount
    && !rgp.world
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp",
          1207,
          0,
          "%s",
          "smodelLightGlob.local.assignedCount == 0 || rgp.world != NULL") )
  {
    __debugbreak();
  }
  for ( entryIndex = 0; entryIndex < smodelLightGlob.local.assignedCount; ++entryIndex )
  {
    smodelIndex = smodelLightGlob.local.smodelIndex[entryIndex];
    if ( smodelIndex >= rgp.world->dpvs.smodelCount
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp",
            1211,
            0,
            "smodelIndex doesn't index rgp.world->dpvs.smodelCount\n\t%i not in [0, %i)",
            smodelIndex,
            rgp.world->dpvs.smodelCount) )
    {
      __debugbreak();
    }
    if ( entryIndex != R_ModelLightingIndexFromHandle(rgp.world->dpvs.smodelDrawInsts[smodelIndex].lightingHandle) )
    {
      v0 = R_ModelLightingIndexFromHandle(rgp.world->dpvs.smodelDrawInsts[smodelIndex].lightingHandle);
      if ( !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp",
              1212,
              0,
              "entryIndex == R_ModelLightingIndexFromHandle( rgp.world->dpvs.smodelDrawInsts[smodelIndex].lightingHandle )\n\t%i, %i",
              entryIndex,
              v0) )
        __debugbreak();
    }
    rgp.world->dpvs.smodelDrawInsts[smodelIndex].lightingHandle = 0;
  }
  smodelLightGlob.local.assignedCount = 0;
}

void __cdecl R_InitModelLightingImage()
{
  if ( !modelLightGlob.totalEntryLimit
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp",
          1222,
          0,
          "%s",
          "modelLightGlob.totalEntryLimit") )
  {
    __debugbreak();
  }
  modelLightGlob.image = Image_AllocProg(12, 4u, 1u);
  Image_SetupAndLoad(modelLightGlob.image, 256, modelLightGlob.imageHeight, 4, 10, D3DFMT_A8R8G8B8);
}

void __cdecl R_ShutdownModelLightingImage()
{
  if ( !modelLightGlob.totalEntryLimit
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_lighting.cpp",
          1258,
          0,
          "%s",
          "modelLightGlob.totalEntryLimit") )
  {
    __debugbreak();
  }
  if ( modelLightGlob.image )
  {
    Image_Release(modelLightGlob.image);
    modelLightGlob.image = 0;
  }
}

void __cdecl R_InitStaticModelLighting()
{
  smodelLightGlob.local.assignedCount = 0;
  smodelLightGlob.local.freeableCount = 0;
}

void __cdecl R_SetModelLightingCoordsForSource(unsigned __int16 handle, GfxCmdBufSourceState *source)
{
  R_SetModelLightingCoords(handle, source->input.consts[78]);
  ++source->constVersions[102];
}

