#include "r_sunshadow.h"

void __cdecl R_SunShadowMapBoundingPoly(
        const GfxSunShadowBoundingPoly *boundingPoly,
        float sampleSize,
        float (*polyInClipSpace)[9][2],
        int *pointIsNear,
        unsigned int partitionRes)
{
  float v5; // [esp+14h] [ebp-10h]
  float viewOrgOffset; // [esp+18h] [ebp-Ch]
  float viewOrgOffset_4; // [esp+1Ch] [ebp-8h]
  int pointIndex; // [esp+20h] [ebp-4h]

  v5 = 2.0 / ((double)partitionRes * sampleSize);
  viewOrgOffset = v5 * boundingPoly->snapDelta[0];
  viewOrgOffset_4 = v5 * boundingPoly->snapDelta[1];
  for ( pointIndex = 0; pointIndex < boundingPoly->pointCount; ++pointIndex )
  {
    (*polyInClipSpace)[pointIndex][0] = boundingPoly->points[pointIndex][0] + viewOrgOffset;
    (*polyInClipSpace)[pointIndex][1] = boundingPoly->points[pointIndex][1] + viewOrgOffset_4;
    pointIsNear[pointIndex] = boundingPoly->pointIsNear[pointIndex];
  }
}

void __cdecl R_SetupSunShadowMaps(const GfxViewParms *viewParms, GfxSunShadow *sunShadow)
{
  float *origin; // [esp+1Ch] [ebp-110h]
  float sunOrigin[3]; // [esp+24h] [ebp-108h] BYREF
  unsigned int planeCount; // [esp+30h] [ebp-FCh]
  const GfxSunShadowProjection *sunProj; // [esp+34h] [ebp-F8h]
  float snappedViewOrgInClipSpace[2][2]; // [esp+38h] [ebp-F4h] BYREF
  unsigned int sunShadowPartitionRes; // [esp+48h] [ebp-E4h]
  float partitionFraction[4]; // [esp+4Ch] [ebp-E0h] BYREF
  float nearClip; // [esp+5Ch] [ebp-D0h] BYREF
  int partitionIndex; // [esp+60h] [ebp-CCh]
  float boundingPolyClipSpacePlanes[9][4]; // [esp+64h] [ebp-C8h] BYREF
  float sampleSizeNear; // [esp+F8h] [ebp-34h]
  float sunAxis[3][3]; // [esp+FCh] [ebp-30h] BYREF
  float farClip; // [esp+120h] [ebp-Ch] BYREF
  GfxViewParms *shadowViewParms; // [esp+124h] [ebp-8h]
  GfxSunShadowPartition *partition; // [esp+128h] [ebp-4h]

  sampleSizeNear = sm_sunSampleSizeNear->current.value;
  sunShadowPartitionRes = (unsigned int)dx.singleSampleDepthStencilSurface;
  R_GetSunAxes(sunAxis);
  R_SetupSunShadowMapProjection(
    viewParms,
    sunAxis,
    sunShadow,
    snappedViewOrgInClipSpace,
    partitionFraction,
    sunShadowPartitionRes);
  LODWORD(sunOrigin[0]) = LODWORD(sunShadow->sunProj.viewMatrix[3][0]) ^ _mask__NegFloat_;
  LODWORD(sunOrigin[1]) = LODWORD(sunShadow->sunProj.viewMatrix[3][1]) ^ _mask__NegFloat_;
  LODWORD(sunOrigin[2]) = LODWORD(sunShadow->sunProj.viewMatrix[3][2]) ^ _mask__NegFloat_;
  R_GetSceneExtentsAlongDir(sunOrigin, sunAxis[0], &nearClip, &farClip);
  sunProj = &sunShadow->sunProj;
  for ( partitionIndex = 0; partitionIndex < 2; ++partitionIndex )
  {
    partition = &sunShadow->partition[partitionIndex];
    shadowViewParms = &partition->shadowViewParms;
    memset((unsigned __int8 *)partition, 0xB0u, 0x140u);
    origin = shadowViewParms->origin;
    LODWORD(shadowViewParms->origin[0]) = LODWORD(sunAxis[0][0]) ^ _mask__NegFloat_;
    *((unsigned int *)origin + 1) = LODWORD(sunAxis[0][1]) ^ _mask__NegFloat_;
    *((unsigned int *)origin + 2) = LODWORD(sunAxis[0][2]) ^ _mask__NegFloat_;
    shadowViewParms->origin[3] = *(float *)&FLOAT_0_0;
    AxisCopy(sunAxis, shadowViewParms->axis);
    memcpy(shadowViewParms, sunProj, 0x40u);
    R_SunShadowMapProjectionMatrix(
      snappedViewOrgInClipSpace[partitionIndex],
      sampleSizeNear,
      nearClip,
      farClip,
      shadowViewParms,
      sunShadowPartitionRes);
    planeCount = R_SunShadowMapClipSpaceClipPlanes(
                   &partition->boundingPoly,
                   partitionIndex,
                   sampleSizeNear,
                   boundingPolyClipSpacePlanes,
                   sunShadowPartitionRes);
    R_SetupShadowSurfacesDpvs(shadowViewParms, boundingPolyClipSpacePlanes, planeCount, partitionIndex);
    sampleSizeNear = sampleSizeNear * rg.sunShadowPartitionRatio;
    ++sunShadow->sunShadowCount;
  }
  partition = sunShadow->partition;
  R_GetSunShadowLookupMatrix(
    &sunShadow->partition[0].shadowViewParms,
    sunProj,
    partitionFraction,
    &sunShadow->lookupMatrix);
}

void __cdecl R_GetSceneExtentsAlongDir(const float *origin, const float *forward, float *nearCap, float *farCap)
{
  float v4; // [esp+8h] [ebp-50h]
  DpvsPlane nearPlane; // [esp+1Ch] [ebp-3Ch] BYREF
  float distMin; // [esp+34h] [ebp-24h]
  float bounds[2][3]; // [esp+38h] [ebp-20h] BYREF
  float farthest; // [esp+50h] [ebp-8h]
  float distMax; // [esp+54h] [ebp-4h]

  R_CalcBoxVsCylinderRayMinBox(
    rgp.world->mins,
    dpvsGlob.viewOrg,
    forward,
    dpvsGlob.sunShadow.sunShadowDrawDist,
    bounds[0]);
  nearPlane.coeffs[0] = *forward;
  nearPlane.coeffs[1] = forward[1];
  nearPlane.coeffs[2] = forward[2];
  LODWORD(nearPlane.coeffs[3]) = *((unsigned int *)origin + 2) ^ _mask__NegFloat_;
  R_SetDpvsPlaneSides(&nearPlane);
  distMin = R_DpvsPlaneMinSignedDistToBox(&nearPlane, bounds[0]);
  distMax = R_DpvsPlaneMaxSignedDistToBox(&nearPlane, bounds[0]);
  farthest = (float)((float)((float)((float)(nearPlane.coeffs[0] * dpvsGlob.viewOrg[0])
                                   + (float)(nearPlane.coeffs[1] * dpvsGlob.viewOrg[1]))
                           + (float)(nearPlane.coeffs[2] * dpvsGlob.viewOrg[2]))
                   + nearPlane.coeffs[3])
           + dpvsGlob.sunShadow.sunShadowDrawDist;
  if ( farthest > distMin )
  {
    if ( (float)(farthest - distMax) < 0.0 )
      v4 = farthest;
    else
      v4 = distMax;
    distMax = v4;
  }
  if ( distMax <= distMin
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_sunshadow.cpp",
          151,
          0,
          "%s",
          "distMin < distMax") )
  {
    __debugbreak();
  }
  *nearCap = distMin;
  *farCap = distMax;
}

void __cdecl R_CalcBoxVsCylinderRayMinBox(
        const float *box,
        const float *origin,
        const float *forward,
        float radius,
        float *outBox)
{
  int a3; // [esp+1Ch] [ebp-20h]
  float dist; // [esp+20h] [ebp-1Ch]
  float inter[3]; // [esp+24h] [ebp-18h] BYREF
  int axis; // [esp+30h] [ebp-Ch]
  int side; // [esp+34h] [ebp-8h]
  int points; // [esp+38h] [ebp-4h]

  points = 0;
  outBox[2] = FLOAT_3_4028235e38;
  outBox[1] = FLOAT_3_4028235e38;
  *outBox = FLOAT_3_4028235e38;
  outBox[5] = FLOAT_N3_4028235e38;
  outBox[4] = FLOAT_N3_4028235e38;
  outBox[3] = FLOAT_N3_4028235e38;
  for ( side = 0; side < 6; side += 3 )
  {
    for ( axis = 0; axis < 3; ++axis )
    {
      if ( forward[axis] != 0.0 )
      {
        dist = (float)(box[axis + side] - origin[axis]) / forward[axis];
        inter[0] = (float)(dist * *forward) + *origin;
        inter[1] = (float)(dist * forward[1]) + origin[1];
        inter[2] = (float)(dist * forward[2]) + origin[2];
        a3 = ~(_BYTE)axis & 2;
        if ( box[(axis & 1) == 0] <= inter[(axis & 1) == 0]
          && box[a3] <= inter[a3]
          && inter[(axis & 1) == 0] <= box[((axis & 1) == 0) + 3]
          && inter[a3] <= box[a3 + 3] )
        {
          ++points;
          Vec3Min(outBox, inter, outBox);
          Vec3Max(outBox + 3, inter, outBox + 3);
        }
      }
    }
  }
  if ( points < 2 )
  {
    *outBox = *box;
    outBox[1] = box[1];
    outBox[2] = box[2];
    outBox[3] = box[3];
    outBox[4] = box[4];
    outBox[5] = box[5];
  }
  else
  {
    *outBox = *outBox + COERCE_FLOAT(LODWORD(radius) ^ _mask__NegFloat_);
    outBox[1] = outBox[1] + COERCE_FLOAT(LODWORD(radius) ^ _mask__NegFloat_);
    outBox[2] = outBox[2] + COERCE_FLOAT(LODWORD(radius) ^ _mask__NegFloat_);
    outBox[3] = outBox[3] + radius;
    outBox[4] = outBox[4] + radius;
    outBox[5] = outBox[5] + radius;
    Vec3Max(outBox, box, outBox);
    Vec3Min(outBox + 3, box + 3, outBox + 3);
  }
}

void __cdecl R_GetSunAxes(float (*sunAxis)[3][3])
{
  float *dir; // [esp+14h] [ebp-4h]

  if ( !frontEndDataOut
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_sunshadow.cpp", 160, 0, "%s", "frontEndDataOut") )
  {
    __debugbreak();
  }
  if ( frontEndDataOut->sunLight.type != 1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_sunshadow.cpp",
          161,
          0,
          "%s",
          "frontEndDataOut->sunLight.type == GFX_LIGHT_TYPE_DIR") )
  {
    __debugbreak();
  }
  dir = frontEndDataOut->sunLight.dir;
  LODWORD((*sunAxis)[0][0]) = LODWORD(frontEndDataOut->sunLight.dir[0]) ^ _mask__NegFloat_;
  LODWORD((*sunAxis)[0][1]) = *((unsigned int *)dir + 1) ^ _mask__NegFloat_;
  LODWORD((*sunAxis)[0][2]) = *((unsigned int *)dir + 2) ^ _mask__NegFloat_;
  if ( (float)((float)((*sunAxis)[0][0] * (*sunAxis)[0][0]) + (float)((*sunAxis)[0][1] * (*sunAxis)[0][1])) >= 0.1 )
  {
    (*sunAxis)[2][0] = *(float *)&FLOAT_0_0;
    (*sunAxis)[2][1] = *(float *)&FLOAT_0_0;
    (*sunAxis)[2][2] = FLOAT_1_0;
  }
  else
  {
    (*sunAxis)[2][0] = FLOAT_1_0;
    (*sunAxis)[2][1] = *(float *)&FLOAT_0_0;
    (*sunAxis)[2][2] = *(float *)&FLOAT_0_0;
  }
  Vec3Cross((*sunAxis)[2], (const float *)sunAxis, (*sunAxis)[1]);
  Vec3Normalize((*sunAxis)[1]);
  Vec3Cross((const float *)sunAxis, (*sunAxis)[1], (*sunAxis)[2]);
}

unsigned int __cdecl R_SunShadowMapClipSpaceClipPlanes(
        const GfxSunShadowBoundingPoly *boundingPoly,
        int partitionIndex,
        float sampleSize,
        float (*boundingPolyClipSpacePlanes)[9][4],
        unsigned int partitionRes)
{
  unsigned int planeCount; // [esp+24h] [ebp-80h]
  int pointIsNear[10]; // [esp+28h] [ebp-7Ch] BYREF
  int pointIndex; // [esp+50h] [ebp-54h]
  float frustumBoundPolyInClipSpace[10][2]; // [esp+54h] [ebp-50h] BYREF

  R_SunShadowMapBoundingPoly(boundingPoly, sampleSize, frustumBoundPolyInClipSpace, pointIsNear, partitionRes);
  *(_QWORD *)&frustumBoundPolyInClipSpace[boundingPoly->pointCount][0] = *(_QWORD *)&frustumBoundPolyInClipSpace[0][0];
  pointIsNear[boundingPoly->pointCount] = pointIsNear[0];
  planeCount = 0;
  for ( pointIndex = 0; pointIndex < boundingPoly->pointCount; ++pointIndex )
  {
    if ( !partitionIndex || !rg.sunShadowFull || pointIsNear[pointIndex] || pointIsNear[pointIndex + 1] )
    {
      (*boundingPolyClipSpacePlanes)[planeCount][0] = frustumBoundPolyInClipSpace[pointIndex + 1][1]
                                                    - frustumBoundPolyInClipSpace[pointIndex][1];
      (*boundingPolyClipSpacePlanes)[planeCount][1] = frustumBoundPolyInClipSpace[pointIndex][0]
                                                    - frustumBoundPolyInClipSpace[pointIndex + 1][0];
      (*boundingPolyClipSpacePlanes)[planeCount][2] = *(float *)&FLOAT_0_0;
      LODWORD((*boundingPolyClipSpacePlanes)[planeCount][3]) = COERCE_UNSIGNED_INT(
                                                                 (float)(frustumBoundPolyInClipSpace[pointIndex][0]
                                                                       * (*boundingPolyClipSpacePlanes)[planeCount][0])
                                                               + (float)(frustumBoundPolyInClipSpace[pointIndex][1]
                                                                       * (*boundingPolyClipSpacePlanes)[planeCount][1]))
                                                             ^ _mask__NegFloat_;
      ++planeCount;
    }
  }
  if ( partitionIndex && rg.sunShadowFull )
  {
    if ( planeCount > 5
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_sunshadow.cpp",
            271,
            0,
            "%s\n\t(planeCount) = %i",
            "(planeCount <= 9 - 4)",
            planeCount) )
    {
      __debugbreak();
    }
    for ( pointIndex = 0; pointIndex < 4; ++pointIndex )
    {
      (*boundingPolyClipSpacePlanes)[planeCount][0] = (float)((float)g_shadowFrustumBound[pointIndex + 1][1]
                                                            - (float)g_shadowFrustumBound[pointIndex][1])
                                                    * rg.sunShadowmapScale;
      (*boundingPolyClipSpacePlanes)[planeCount][1] = (float)((float)g_shadowFrustumBound[pointIndex][0]
                                                            - (float)g_shadowFrustumBound[pointIndex + 1][0])
                                                    * rg.sunShadowmapScale;
      (*boundingPolyClipSpacePlanes)[planeCount][2] = *(float *)&FLOAT_0_0;
      (*boundingPolyClipSpacePlanes)[planeCount][3] = COERCE_FLOAT(
                                                        COERCE_UNSIGNED_INT(
                                                          (float)((float)g_shadowFrustumBound[pointIndex][0]
                                                                * (*boundingPolyClipSpacePlanes)[planeCount][0])
                                                        + (float)((float)g_shadowFrustumBound[pointIndex][1]
                                                                * (*boundingPolyClipSpacePlanes)[planeCount][1]))
                                                      ^ _mask__NegFloat_)
                                                    * rg.sunShadowmapScale;
      ++planeCount;
    }
  }
  return planeCount;
}

void __cdecl R_SunShadowMapProjectionMatrix(
        const float *snappedViewOrgInClipSpace,
        float shadowSampleSize,
        float nearClip,
        float farClip,
        GfxViewParms *shadowViewParms,
        unsigned int partitionRes)
{
  float shadowSampleExtents; // [esp+8h] [ebp-4h]

  if ( shadowSampleSize == 0.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_sunshadow.cpp",
          313,
          0,
          "%s",
          "shadowSampleSize") )
  {
    __debugbreak();
  }
  memset((unsigned __int8 *)&shadowViewParms->projectionMatrix, 0, sizeof(shadowViewParms->projectionMatrix));
  shadowSampleExtents = (double)partitionRes * shadowSampleSize;
  shadowViewParms->projectionMatrix.m[0][0] = 2.0 / shadowSampleExtents;
  shadowViewParms->projectionMatrix.m[3][0] = *snappedViewOrgInClipSpace;
  shadowViewParms->projectionMatrix.m[1][1] = shadowViewParms->projectionMatrix.m[0][0];
  shadowViewParms->projectionMatrix.m[3][1] = snappedViewOrgInClipSpace[1];
  shadowViewParms->projectionMatrix.m[2][2] = 1.0 / (float)((float)(farClip - nearClip) + 2.0);
  shadowViewParms->projectionMatrix.m[3][2] = COERCE_FLOAT(COERCE_UNSIGNED_INT(nearClip - 1.0) ^ _mask__NegFloat_)
                                            * shadowViewParms->projectionMatrix.m[2][2];
  shadowViewParms->projectionMatrix.m[3][3] = FLOAT_1_0;
  shadowViewParms->depthHackNearClip = shadowViewParms->projectionMatrix.m[3][2];
  R_SetupViewProjectionMatrices(shadowViewParms, 0);
}

void __cdecl R_SetupSunShadowMapProjection(
        const GfxViewParms *viewParms,
        const float (*sunAxis)[3][3],
        GfxSunShadow *sunShadow,
        float (*snappedViewOrgInClipSpace)[2][2],
        float *partitionFraction,
        unsigned int partitionRes)
{
  unsigned int v6; // [esp+18h] [ebp-2F8h]
  float v7; // [esp+1Ch] [ebp-2F4h]
  unsigned int v8; // [esp+38h] [ebp-2D8h]
  float v9; // [esp+3Ch] [ebp-2D4h]
  unsigned int v10; // [esp+60h] [ebp-2B0h]
  float v11; // [esp+64h] [ebp-2ACh]
  unsigned int sunShadowSize; // [esp+80h] [ebp-290h]
  float v13; // [esp+84h] [ebp-28Ch]
  float v14; // [esp+B4h] [ebp-25Ch]
  float v15; // [esp+CCh] [ebp-244h]
  bool v16; // [esp+D8h] [ebp-238h]
  float v17; // [esp+F4h] [ebp-21Ch]
  float v18; // [esp+F8h] [ebp-218h]
  float v19; // [esp+104h] [ebp-20Ch]
  float *shadowmapScale; // [esp+108h] [ebp-208h]
  float v21; // [esp+110h] [ebp-200h]
  float v22; // [esp+11Ch] [ebp-1F4h]
  float v23; // [esp+128h] [ebp-1E8h]
  float v24; // [esp+134h] [ebp-1DCh]
  float v25; // [esp+140h] [ebp-1D0h]
  float v26; // [esp+144h] [ebp-1CCh]
  float v27; // [esp+148h] [ebp-1C8h]
  float *v28; // [esp+160h] [ebp-1B0h]
  float v29; // [esp+164h] [ebp-1ACh]
  float minsInSunProj[2][2]; // [esp+19Ch] [ebp-174h] BYREF
  float sizeInSunProj[2]; // [esp+1ACh] [ebp-164h]
  float frustumPoints[4][3]; // [esp+1B4h] [ebp-15Ch] BYREF
  float maxsInSunProj[2][2]; // [esp+1E4h] [ebp-12Ch] BYREF
  GfxSunShadowProjection *sunProj; // [esp+1F4h] [ebp-11Ch]
  float frustumPointsInSunProj[2][8][2]; // [esp+1F8h] [ebp-118h] BYREF
  int snappedViewOrgInTicks[2]; // [esp+27Ch] [ebp-94h]
  float viewOrgInTexSpace[2][2]; // [esp+284h] [ebp-8Ch] BYREF
  float shadowOrg[3]; // [esp+294h] [ebp-7Ch]
  float viewOrgInSunProj[2]; // [esp+2A0h] [ebp-70h] BYREF
  float sampleSizeFar; // [esp+2A8h] [ebp-68h]
  unsigned int farShadowBegin; // [esp+2ACh] [ebp-64h]
  GfxSunShadowPartition *partitionNear; // [esp+2B0h] [ebp-60h]
  float offset[2]; // [esp+2B4h] [ebp-5Ch]
  unsigned int pointIndex; // [esp+2BCh] [ebp-54h]
  unsigned int farShadowEnd; // [esp+2C0h] [ebp-50h]
  float frustumPoint[3]; // [esp+2C4h] [ebp-4Ch]
  int useShadowOffset; // [esp+2D0h] [ebp-40h]
  unsigned int farShadowMiddle; // [esp+2D4h] [ebp-3Ch]
  GfxSunShadowPartition *partitionFar; // [esp+2D8h] [ebp-38h]
  float sampleSizeNear; // [esp+2DCh] [ebp-34h]
  float scale; // [esp+2E0h] [ebp-30h]
  float maxSizeInSunProj; // [esp+2E4h] [ebp-2Ch]
  float shadowOrgInSunProj[2]; // [esp+2E8h] [ebp-28h]
  float farShadowSizeRatio; // [esp+2F0h] [ebp-20h]
  float scaleToFitUsable; // [esp+2F4h] [ebp-1Ch]
  float viewOrgInPixels[2][2]; // [esp+2F8h] [ebp-18h] BYREF
  float snappedViewOrgInSunProj[2]; // [esp+308h] [ebp-8h] BYREF

  LODWORD(viewOrgInSunProj[0]) = COERCE_UNSIGNED_INT(
                                   (float)((float)(viewParms->origin[0] * (float)(*sunAxis)[1][0])
                                         + (float)(viewParms->origin[1] * (float)(*sunAxis)[1][1]))
                                 + (float)(viewParms->origin[2] * (float)(*sunAxis)[1][2]))
                               ^ _mask__NegFloat_;
  viewOrgInSunProj[1] = (float)((float)(viewParms->origin[0] * (float)(*sunAxis)[2][0])
                              + (float)(viewParms->origin[1] * (float)(*sunAxis)[2][1]))
                      + (float)(viewParms->origin[2] * (float)(*sunAxis)[2][2]);

  minsInSunProj[0][0] = FLOAT_3_4028235e38;
  minsInSunProj[0][1] = FLOAT_3_4028235e38;

  maxsInSunProj[0][0] = FLOAT_N3_4028235e38;
  maxsInSunProj[0][1] = FLOAT_N3_4028235e38;

  R_GetFrustumNearClipPoints(&viewParms->inverseViewProjectionMatrix, frustumPoints);

  for ( pointIndex = 0; pointIndex < 4; ++pointIndex )
  {
    frustumPoint[0] = frustumPoints[pointIndex][0];
    frustumPoint[1] = frustumPoints[pointIndex][1];
    frustumPoint[2] = frustumPoints[pointIndex][2];
    LODWORD(frustumPointsInSunProj[0][pointIndex][0]) = COERCE_UNSIGNED_INT(
                                                          (float)((float)(frustumPoints[pointIndex][0]
                                                                        * (float)(*sunAxis)[1][0])
                                                                + (float)(frustumPoints[pointIndex][1]
                                                                        * (float)(*sunAxis)[1][1]))
                                                        + (float)(frustumPoints[pointIndex][2] * (float)(*sunAxis)[1][2]))
                                                      ^ _mask__NegFloat_;
    frustumPointsInSunProj[0][pointIndex][1] = (float)((float)(frustumPoints[pointIndex][0] * (float)(*sunAxis)[2][0])
                                                     + (float)(frustumPoints[pointIndex][1] * (float)(*sunAxis)[2][1]))
                                             + (float)(frustumPoints[pointIndex][2] * (float)(*sunAxis)[2][2]);
    AddPointToBounds2D(frustumPointsInSunProj[0][pointIndex], minsInSunProj[0], maxsInSunProj[0]);
  }

  *(_QWORD *)&frustumPointsInSunProj[0][4][0] = *(_QWORD *)viewOrgInSunProj;

  *(_QWORD *)&minsInSunProj[1][0] = *(_QWORD *)&minsInSunProj[0][0];

  *(_QWORD *)&maxsInSunProj[1][0] = *(_QWORD *)&maxsInSunProj[0][0];

  memcpy(frustumPointsInSunProj[1], frustumPointsInSunProj, 0x20u);

  for ( pointIndex = 0; pointIndex < 4; ++pointIndex )
  {
    offset[0] = frustumPointsInSunProj[1][pointIndex][0] - viewOrgInSunProj[0];
    offset[1] = frustumPointsInSunProj[1][pointIndex][1] - viewOrgInSunProj[1];
    v28 = frustumPointsInSunProj[1][pointIndex + 4];
    v29 = 0.75 / rg.sunShadowPartitionRatio;
    *v28 = (float)((float)(0.75 / rg.sunShadowPartitionRatio) * offset[0]) + viewOrgInSunProj[0];
    v28[1] = (float)(v29 * offset[1]) + viewOrgInSunProj[1];
    AddPointToBounds2D(frustumPointsInSunProj[1][pointIndex + 4], minsInSunProj[1], maxsInSunProj[1]);
  }
  AddPointToBounds2D(viewOrgInSunProj, minsInSunProj[0], maxsInSunProj[0]);
  sizeInSunProj[0] = maxsInSunProj[0][0] - minsInSunProj[0][0];
  sizeInSunProj[1] = maxsInSunProj[0][1] - minsInSunProj[0][1];

  if ( (float)((float)(maxsInSunProj[0][0] - minsInSunProj[0][0]) - (float)(maxsInSunProj[0][1] - minsInSunProj[0][1])) < 0.0 )
    v19 = sizeInSunProj[1];
  else
    v19 = sizeInSunProj[0];

  maxSizeInSunProj = v19;

  sampleSizeNear = sm_sunSampleSizeNear->current.value;
  sampleSizeFar = sampleSizeNear * rg.sunShadowPartitionRatio;
  scale = (float)(sampleSizeNear * rg.sunShadowPartitionRatio) * rg.sunShadowmapScaleNum;
  dpvsGlob.sunShadow.sunShadowDrawDist = (double)partitionRes * (float)(sampleSizeNear * rg.sunShadowPartitionRatio);
  v18 = floor((float)(viewOrgInSunProj[0] / scale));
  snappedViewOrgInTicks[0] = (int)v18;
  v17 = floor((float)(viewOrgInSunProj[1] / scale));
  snappedViewOrgInTicks[1] = (int)v17;
  snappedViewOrgInSunProj[0] = (float)snappedViewOrgInTicks[0] * scale;
  snappedViewOrgInSunProj[1] = (float)snappedViewOrgInTicks[1] * scale;
  scaleToFitUsable = (double)(partitionRes - 1) / maxSizeInSunProj;
  farShadowBegin = (partitionRes - rg.sunShadowSize) >> 1;
  farShadowEnd = partitionRes - farShadowBegin;
  farShadowMiddle = partitionRes >> 1;
  farShadowSizeRatio = (double)rg.sunShadowSize / (double)partitionRes;
  partitionNear = sunShadow->partition;
  partitionFar = &sunShadow->partition[1];

  shadowOrg[0] = sm_sunShadowCenter->current.value;
  shadowOrg[1] = sm_sunShadowCenter->current.vector[1];
  shadowOrg[2] = sm_sunShadowCenter->current.vector[2];
  v16 = shadowOrg[0] == 0.0 && shadowOrg[1] == 0.0 && shadowOrg[2] == 0.0;
  useShadowOffset = !v16;

  LODWORD(shadowOrgInSunProj[0]) = COERCE_UNSIGNED_INT(
                                     (float)((float)(shadowOrg[0] * (float)(*sunAxis)[1][0])
                                           + (float)(shadowOrg[1] * (float)(*sunAxis)[1][1]))
                                   + (float)(shadowOrg[2] * (float)(*sunAxis)[1][2]))
                                 ^ _mask__NegFloat_;
  shadowOrgInSunProj[1] = (float)((float)(shadowOrg[0] * (float)(*sunAxis)[2][0])
                                + (float)(shadowOrg[1] * (float)(*sunAxis)[2][1]))
                        + (float)(shadowOrg[2] * (float)(*sunAxis)[2][2]);
  v27 = (double)partitionRes / maxSizeInSunProj * sizeInSunProj[0] - 0.001;
  v15 = ceil(v27);
  partitionNear->viewport.width = (unsigned __int16)(int)v15;
  v26 = (double)partitionRes / maxSizeInSunProj * sizeInSunProj[1] - 0.001;
  v14 = ceil(v26);
  partitionNear->viewport.height = (unsigned __int16)(int)v14;
  if ( viewOrgInSunProj[0] < (float)((float)(minsInSunProj[0][0] + maxsInSunProj[0][0]) * 0.5) )
  {
    viewOrgInPixels[0][0] = (float)((float)(viewOrgInSunProj[0] - minsInSunProj[0][0]) * scaleToFitUsable) + 1.0;
    if ( useShadowOffset )
    {
      viewOrgInPixels[1][0] = (double)farShadowMiddle + (viewOrgInSunProj[0] - shadowOrgInSunProj[0]) / sampleSizeFar;
      partitionFar->viewport.x = farShadowBegin;
    }
    else
    {
      viewOrgInPixels[1][0] = (double)(farShadowBegin + 1)
                            + (viewOrgInSunProj[0] - minsInSunProj[0][0]) * scaleToFitUsable * farShadowSizeRatio;
      v24 = (minsInSunProj[1][0] - minsInSunProj[0][0]) * scaleToFitUsable + (double)farShadowBegin;
      v11 = floor(v24);
      if ( (int)farShadowBegin < (int)v11 )
        v10 = (int)v11;
      else
        v10 = farShadowBegin;
      partitionFar->viewport.x = v10;
    }
    partitionNear->viewport.x = 0;
    partitionFar->viewport.width = farShadowEnd - partitionFar->viewport.x;
  }
  else
  {
    viewOrgInPixels[0][0] = (double)(partitionRes - 1) - (maxsInSunProj[0][0] - viewOrgInSunProj[0]) * scaleToFitUsable;
    if ( useShadowOffset )
    {
      viewOrgInPixels[1][0] = (double)farShadowMiddle - (viewOrgInSunProj[0] - shadowOrgInSunProj[0]) / sampleSizeFar;
      partitionFar->viewport.width = rg.sunShadowSize;
    }
    else
    {
      viewOrgInPixels[1][0] = (double)(farShadowEnd - 1)
                            - (maxsInSunProj[0][0] - viewOrgInSunProj[0]) * scaleToFitUsable * farShadowSizeRatio;
      v25 = (double)farShadowEnd - (maxsInSunProj[0][0] - maxsInSunProj[1][0]) * scaleToFitUsable;
      v13 = ceil(v25);
      if ( (int)((int)v13 - farShadowBegin) < (int)rg.sunShadowSize )
        sunShadowSize = (int)v13 - farShadowBegin;
      else
        sunShadowSize = rg.sunShadowSize;
      partitionFar->viewport.width = sunShadowSize;
    }
    partitionNear->viewport.x = partitionRes - partitionNear->viewport.width;
    partitionFar->viewport.x = farShadowBegin;
  }
  if ( (float)((float)(minsInSunProj[0][1] + maxsInSunProj[0][1]) * 0.5) < viewOrgInSunProj[1] )
  {
    viewOrgInPixels[0][1] = (float)((float)(maxsInSunProj[0][1] - viewOrgInSunProj[1]) * scaleToFitUsable) + 1.0;
    if ( useShadowOffset )
    {
      viewOrgInPixels[1][1] = (double)farShadowMiddle + (viewOrgInSunProj[1] - shadowOrgInSunProj[1]) / sampleSizeFar;
      partitionFar->viewport.y = farShadowBegin;
    }
    else
    {
      viewOrgInPixels[1][1] = (double)(farShadowBegin + 1)
                            + (maxsInSunProj[0][1] - viewOrgInSunProj[1]) * scaleToFitUsable * farShadowSizeRatio;
      v22 = (maxsInSunProj[0][1] - maxsInSunProj[1][1]) * scaleToFitUsable + (double)farShadowBegin;
      v7 = floor(v22);
      if ( (int)farShadowBegin < (int)v7 )
        v6 = (int)v7;
      else
        v6 = farShadowBegin;
      partitionFar->viewport.y = v6;
    }
    partitionNear->viewport.y = 0;
    partitionFar->viewport.height = farShadowEnd - partitionFar->viewport.y;
  }
  else
  {
    viewOrgInPixels[0][1] = (double)(partitionRes - 1) - (viewOrgInSunProj[1] - minsInSunProj[0][1]) * scaleToFitUsable;
    if ( useShadowOffset )
    {
      viewOrgInPixels[1][1] = (double)farShadowMiddle - (viewOrgInSunProj[1] - shadowOrgInSunProj[1]) / sampleSizeFar;
      partitionFar->viewport.height = rg.sunShadowSize;
    }
    else
    {
      viewOrgInPixels[1][1] = (double)(farShadowEnd - 1)
                            - (viewOrgInSunProj[1] - minsInSunProj[0][1]) * scaleToFitUsable * farShadowSizeRatio;
      v23 = (double)farShadowEnd - (minsInSunProj[1][1] - minsInSunProj[0][1]) * scaleToFitUsable;
      v9 = ceil(v23);
      if ( (int)((int)v9 - farShadowBegin) < (int)rg.sunShadowSize )
        v8 = (int)v9 - farShadowBegin;
      else
        v8 = rg.sunShadowSize;
      partitionFar->viewport.height = v8;
    }
    partitionNear->viewport.y = partitionRes - partitionNear->viewport.height;
    partitionFar->viewport.y = farShadowBegin;
  }
  if ( !rg.sunShadowFull )
    partitionFar->viewport = partitionNear->viewport;
  R_GetSunShadowMapPartitionViewOrgInTextureSpace(
    viewOrgInPixels[0],
    viewOrgInSunProj,
    snappedViewOrgInSunProj,
    sampleSizeNear,
    viewOrgInTexSpace[0],
    partitionRes);
  R_GetSunShadowMapPartitionViewOrgInTextureSpace(
    viewOrgInPixels[1],
    viewOrgInSunProj,
    snappedViewOrgInSunProj,
    sampleSizeFar,
    viewOrgInTexSpace[1],
    partitionRes);
  sunProj = &sunShadow->sunProj;
  sunShadow->sunProj.switchPartition[3] = 1.0 / rg.sunShadowPartitionRatio;
  sunProj->switchPartition[0] = viewOrgInTexSpace[1][0] - (float)(viewOrgInTexSpace[0][0] * sunProj->switchPartition[3]);
  sunProj->switchPartition[1] = (float)((float)(viewOrgInTexSpace[1][1]
                                              - (float)(viewOrgInTexSpace[0][1] * sunProj->switchPartition[3]))
                                      + 1.0)
                              * 0.5;
  sunProj->switchPartition[2] = *(float *)&FLOAT_0_0;

  shadowmapScale = sunProj->shadowmapScale;
  v21 = 32.0 / rg.sunShadowmapScale;
  sunProj->shadowmapScale[0] = 16.0 / rg.sunShadowmapScale;
  shadowmapScale[1] = v21;
  shadowmapScale[2] = *(float *)&FLOAT_0_0;
  shadowmapScale[3] = *(float *)&FLOAT_0_0;
  (*snappedViewOrgInClipSpace)[0][0] = (float)(viewOrgInTexSpace[0][0] * 2.0) - 1.0;
  (*snappedViewOrgInClipSpace)[0][1] = 1.0 - (float)(viewOrgInTexSpace[0][1] * 2.0);
  (*snappedViewOrgInClipSpace)[1][0] = (float)(viewOrgInTexSpace[1][0] * 2.0) - 1.0;
  (*snappedViewOrgInClipSpace)[1][1] = 1.0 - (float)(viewOrgInTexSpace[1][1] * 2.0);
  R_SetupSunShadowMapViewMatrix(snappedViewOrgInSunProj, (const float (*)[3])sunAxis, sunProj);
  R_SetupSunShadowBoundingPoly(
    frustumPointsInSunProj[0],
    viewOrgInSunProj,
    snappedViewOrgInSunProj,
    maxSizeInSunProj,
    (const float *)snappedViewOrgInClipSpace,
    &partitionNear->boundingPoly,
    5u);
  R_SetupSunShadowBoundingPoly(
    frustumPointsInSunProj[1],
    viewOrgInSunProj,
    snappedViewOrgInSunProj,
    maxSizeInSunProj,
    (*snappedViewOrgInClipSpace)[1],
    &partitionFar->boundingPoly,
    8u);
  R_SetupSunShadowMapPartitionFraction(viewParms, scaleToFitUsable, sunProj, partitionFraction);
  R_SetupNearRegionPlane(partitionFraction);
}

void __cdecl R_GetFrustumNearClipPoints(const GfxMatrix *invViewProjMtx, float (*frustumPoints)[3])
{
  R_ClipSpaceToWorldSpace(invViewProjMtx, (const float (*)[4][3])g_clipSpacePoints, 4, (float (*)[4][3])frustumPoints);
}

void __cdecl R_ClipSpaceToWorldSpace(
        const GfxMatrix *invViewProjMtx,
        const float (*clipSpacePoints)[4][3],
        int pointCount,
        float (*worldSpacePoints)[4][3])
{
  float *v4; // [esp+0h] [ebp-30h]
  float xyz; // [esp+8h] [ebp-28h]
  float xyz_4; // [esp+Ch] [ebp-24h]
  float xyz_8; // [esp+10h] [ebp-20h]
  float homogenousPoint[4]; // [esp+14h] [ebp-1Ch]
  int pointIndex; // [esp+24h] [ebp-Ch]
  int term; // [esp+28h] [ebp-8h]
  float scale; // [esp+2Ch] [ebp-4h]

  for ( pointIndex = 0; pointIndex < pointCount; ++pointIndex )
  {
    xyz = (*clipSpacePoints)[pointIndex][0];
    xyz_4 = (*clipSpacePoints)[pointIndex][1];
    xyz_8 = (*clipSpacePoints)[pointIndex][2];
    for ( term = 0; term < 4; ++term )
      homogenousPoint[term] = (float)((float)((float)(xyz * invViewProjMtx->m[0][term])
                                            + (float)(xyz_4 * invViewProjMtx->m[1][term]))
                                    + (float)(xyz_8 * invViewProjMtx->m[2][term]))
                            + invViewProjMtx->m[3][term];
    if ( homogenousPoint[3] <= 0.0
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_sunshadow.cpp",
            186,
            0,
            "%s",
            "homogenousPoint[3] > 0") )
    {
      __debugbreak();
    }
    scale = 1.0 / homogenousPoint[3];
    v4 = (*worldSpacePoints)[pointIndex];
    *v4 = (float)(1.0 / homogenousPoint[3]) * homogenousPoint[0];
    v4[1] = scale * homogenousPoint[1];
    v4[2] = scale * homogenousPoint[2];
  }
}

void __cdecl R_SetupSunShadowBoundingPoly(
        float (*frustumPointsInSunProj)[2],
        const float *viewOrgInSunProj,
        const float *snappedViewOrgInSunProj,
        float maxSizeInSunProj,
        const float *snappedViewOrgInClipSpace,
        GfxSunShadowBoundingPoly *boundingPoly,
        unsigned int pointCount)
{
  unsigned int nearPointIndex; // [esp+Ch] [ebp-94h]
  float scaleToClipSpace; // [esp+10h] [ebp-90h]
  int pointIndex; // [esp+14h] [ebp-8Ch]
  float tempFrustumPointsInSunProj[8][2]; // [esp+18h] [ebp-88h] BYREF
  float frustumBoundingPolyInSunProj[9][2]; // [esp+58h] [ebp-48h] BYREF

  memcpy((unsigned __int8 *)tempFrustumPointsInSunProj, (unsigned __int8 *)frustumPointsInSunProj, 8 * pointCount);
  boundingPoly->pointCount = Com_ConvexHull(tempFrustumPointsInSunProj, pointCount, frustumBoundingPolyInSunProj);
  if ( (boundingPoly->pointCount < 3 || boundingPoly->pointCount > 9)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_sunshadow.cpp",
          208,
          1,
          "boundingPoly->pointCount not in [3, R_SUNSHADOW_SIDE_PLANES_MAX]\n\t%i not in [%i, %i]",
          boundingPoly->pointCount,
          3,
          9) )
  {
    __debugbreak();
  }
  scaleToClipSpace = 2.0 / maxSizeInSunProj;
  for ( pointIndex = 0; pointIndex < boundingPoly->pointCount; ++pointIndex )
  {
    boundingPoly->points[pointIndex][0] = (float)((float)(frustumBoundingPolyInSunProj[pointIndex][0] - *viewOrgInSunProj)
                                                * scaleToClipSpace)
                                        + *snappedViewOrgInClipSpace;
    boundingPoly->points[pointIndex][1] = (float)((float)(frustumBoundingPolyInSunProj[pointIndex][1]
                                                        - viewOrgInSunProj[1])
                                                * scaleToClipSpace)
                                        + snappedViewOrgInClipSpace[1];
    boundingPoly->pointIsNear[pointIndex] = 0;
    for ( nearPointIndex = 4; nearPointIndex < pointCount; ++nearPointIndex )
    {
      if ( frustumBoundingPolyInSunProj[pointIndex][0] == (*frustumPointsInSunProj)[2 * nearPointIndex]
        && frustumBoundingPolyInSunProj[pointIndex][1] == (*frustumPointsInSunProj)[2 * nearPointIndex + 1] )
      {
        boundingPoly->pointIsNear[pointIndex] = 1;
        break;
      }
    }
  }
  boundingPoly->snapDelta[0] = *viewOrgInSunProj - *snappedViewOrgInSunProj;
  boundingPoly->snapDelta[1] = viewOrgInSunProj[1] - snappedViewOrgInSunProj[1];
}

void __cdecl R_SetupSunShadowMapViewMatrix(
        const float *snappedViewOrgInSunProj,
        const float (*sunAxis)[3],
        GfxSunShadowProjection *sunProj)
{
  LODWORD(sunProj->viewMatrix[0][0]) = LODWORD((*sunAxis)[3]) ^ _mask__NegFloat_;
  LODWORD(sunProj->viewMatrix[1][0]) = LODWORD((*sunAxis)[4]) ^ _mask__NegFloat_;
  LODWORD(sunProj->viewMatrix[2][0]) = LODWORD((*sunAxis)[5]) ^ _mask__NegFloat_;
  LODWORD(sunProj->viewMatrix[3][0]) = *(unsigned int *)snappedViewOrgInSunProj ^ _mask__NegFloat_;
  sunProj->viewMatrix[0][1] = (*sunAxis)[6];
  sunProj->viewMatrix[1][1] = (*sunAxis)[7];
  sunProj->viewMatrix[2][1] = (*sunAxis)[8];
  LODWORD(sunProj->viewMatrix[3][1]) = *((unsigned int *)snappedViewOrgInSunProj + 1) ^ _mask__NegFloat_;
  sunProj->viewMatrix[0][2] = (*sunAxis)[0];
  sunProj->viewMatrix[1][2] = (*sunAxis)[1];
  sunProj->viewMatrix[2][2] = (*sunAxis)[2];
  sunProj->viewMatrix[3][2] = *(float *)&FLOAT_0_0;
  sunProj->viewMatrix[0][3] = *(float *)&FLOAT_0_0;
  sunProj->viewMatrix[1][3] = *(float *)&FLOAT_0_0;
  sunProj->viewMatrix[2][3] = *(float *)&FLOAT_0_0;
  sunProj->viewMatrix[3][3] = FLOAT_1_0;
}

void __cdecl R_SetupSunShadowMapPartitionFraction(
        const GfxViewParms *viewParms,
        float scaleToFitUsable,
        GfxSunShadowProjection *sunProj,
        float *partitionFraction)
{
  float v4; // [esp+4h] [ebp-14h]
  float zNear; // [esp+Ch] [ebp-Ch]

  zNear = viewParms->zNear;
  if ( zNear == 0.0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_sunshadow.cpp", 349, 0, "%s", "zNear") )
  {
    __debugbreak();
  }
  v4 = 1.0 / (float)((float)(zNear * sm_sunSampleSizeNear->current.value) * scaleToFitUsable);
  *partitionFraction = v4 * viewParms->axis[0][0];
  partitionFraction[1] = v4 * viewParms->axis[0][1];
  partitionFraction[2] = v4 * viewParms->axis[0][2];
  *((unsigned int *)partitionFraction + 3) = COERCE_UNSIGNED_INT(
                                         (float)((float)(viewParms->origin[0] * *partitionFraction)
                                               + (float)(viewParms->origin[1] * partitionFraction[1]))
                                       + (float)(viewParms->origin[2] * partitionFraction[2]))
                                     ^ _mask__NegFloat_;
}

void __cdecl R_GetSunShadowMapPartitionViewOrgInTextureSpace(
        const float *viewOrgInPixels,
        const float *viewOrgInSunProj,
        const float *snappedViewOrgInSunProj,
        float sampleSize,
        float *viewOrgInTexSpace,
        unsigned int partitionRes)
{
  float v6; // [esp+18h] [ebp-18h]
  float v7; // [esp+1Ch] [ebp-14h]
  float snappedViewOrgInPixels_4; // [esp+24h] [ebp-Ch]

  snappedViewOrgInPixels_4 = viewOrgInPixels[1]
                           - (float)((float)(snappedViewOrgInSunProj[1] - viewOrgInSunProj[1]) / sampleSize);
  v7 = floor((float)((float)((float)(*snappedViewOrgInSunProj - *viewOrgInSunProj) / sampleSize) + *viewOrgInPixels));
  v6 = floor(snappedViewOrgInPixels_4);
  *viewOrgInTexSpace = 1.0 / (double)partitionRes * v7;
  viewOrgInTexSpace[1] = 1.0 / (double)partitionRes * v6;
}

void __cdecl R_SetupNearRegionPlane(const float *partitionFraction)
{
  float size; // [esp+1Ch] [ebp-14h]
  DpvsPlane *shadowFarPlane; // [esp+20h] [ebp-10h]
  unsigned int partitionIndex; // [esp+24h] [ebp-Ch]
  float length; // [esp+2Ch] [ebp-4h]
  float lengtha; // [esp+2Ch] [ebp-4h]
  float lengthb; // [esp+2Ch] [ebp-4h]

  scene.shadowNearPlane[0].coeffs[0] = *partitionFraction;
  scene.shadowNearPlane[0].coeffs[1] = partitionFraction[1];
  scene.shadowNearPlane[0].coeffs[2] = partitionFraction[2];
  scene.shadowNearPlane[0].coeffs[3] = partitionFraction[3];

  length = Abs(scene.shadowNearPlane[0].coeffs);
  if ( length <= 0.0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_sunshadow.cpp", 391, 0, "%s", "length > 0") )
  {
    __debugbreak();
  }
  scene.shadowNearPlane[0].coeffs[0] = (float)(1.0 / length) * scene.shadowNearPlane[0].coeffs[0];
  scene.shadowNearPlane[0].coeffs[1] = (float)(1.0 / length) * scene.shadowNearPlane[0].coeffs[1];
  scene.shadowNearPlane[0].coeffs[2] = (float)(1.0 / length) * scene.shadowNearPlane[0].coeffs[2];
  scene.shadowNearPlane[0].coeffs[3] = (float)(1.0 / length) * scene.shadowNearPlane[0].coeffs[3];
  R_SetDpvsPlaneSides(scene.shadowNearPlane);
  scene.shadowNearPlane[1].coeffs[0] = *partitionFraction;
  scene.shadowNearPlane[1].coeffs[1] = partitionFraction[1];
  scene.shadowNearPlane[1].coeffs[2] = partitionFraction[2];
  scene.shadowNearPlane[1].coeffs[3] = partitionFraction[3];
  scene.shadowNearPlane[1].coeffs[3] = scene.shadowNearPlane[1].coeffs[3] - 0.75;
  lengtha = Abs(scene.shadowNearPlane[1].coeffs);
  if ( lengtha <= 0.0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_sunshadow.cpp", 401, 0, "%s", "length > 0") )
  {
    __debugbreak();
  }
  scene.shadowNearPlane[1].coeffs[0] = (float)(1.0 / lengtha) * scene.shadowNearPlane[1].coeffs[0];
  scene.shadowNearPlane[1].coeffs[1] = (float)(1.0 / lengtha) * scene.shadowNearPlane[1].coeffs[1];
  scene.shadowNearPlane[1].coeffs[2] = (float)(1.0 / lengtha) * scene.shadowNearPlane[1].coeffs[2];
  scene.shadowNearPlane[1].coeffs[3] = (float)(1.0 / lengtha) * scene.shadowNearPlane[1].coeffs[3];
  R_SetDpvsPlaneSides(&scene.shadowNearPlane[1]);
  size = FLOAT_1_0;
  for ( partitionIndex = 0; partitionIndex < 2; ++partitionIndex )
  {
    shadowFarPlane = &scene.shadowFarPlane[partitionIndex];
    shadowFarPlane->coeffs[0] = *partitionFraction;
    shadowFarPlane->coeffs[1] = partitionFraction[1];
    shadowFarPlane->coeffs[2] = partitionFraction[2];
    shadowFarPlane->coeffs[3] = partitionFraction[3];
    shadowFarPlane->coeffs[3] = shadowFarPlane->coeffs[3] - size;
    lengthb = Abs(shadowFarPlane->coeffs);
    if ( lengthb <= 0.0
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_sunshadow.cpp", 414, 0, "%s", "length > 0") )
    {
      __debugbreak();
    }
    shadowFarPlane->coeffs[0] = (float)(-1.0 / lengthb) * shadowFarPlane->coeffs[0];
    shadowFarPlane->coeffs[1] = (float)(-1.0 / lengthb) * shadowFarPlane->coeffs[1];
    shadowFarPlane->coeffs[2] = (float)(-1.0 / lengthb) * shadowFarPlane->coeffs[2];
    shadowFarPlane->coeffs[3] = (float)(-1.0 / lengthb) * shadowFarPlane->coeffs[3];
    R_SetDpvsPlaneSides(shadowFarPlane);
    size = size * rg.sunShadowPartitionRatio;
  }
}

void __cdecl R_GetSunShadowLookupMatrix(
        const GfxViewParms *shadowViewParms,
        const GfxSunShadowProjection *sunProj,
        const float *partitionFraction,
        GfxMatrix *lookupMatrix)
{
  float x0; // [esp+0h] [ebp-20h]
  float xScale; // [esp+4h] [ebp-1Ch]
  float xShift; // [esp+Ch] [ebp-14h]
  float yShift; // [esp+10h] [ebp-10h]
  float y0; // [esp+18h] [ebp-8h]
  float yScale; // [esp+1Ch] [ebp-4h]

  x0 = (float)(0.5 / (float)(int)dx.singleSampleDepthStencilSurface) + 0.0;
  y0 = (float)(0.5 / (float)(2 * (int)dx.singleSampleDepthStencilSurface)) + 0.5;
  xScale = (float)((float)((float)(0.5 / (float)(int)dx.singleSampleDepthStencilSurface) + 1.0) - x0) * 0.5;
  xShift = (float)((float)((float)(0.5 / (float)(int)dx.singleSampleDepthStencilSurface) + 1.0) + x0) * 0.5;
  yScale = (float)((float)((float)(0.5 / (float)(2 * (int)dx.singleSampleDepthStencilSurface)) + 0.0) - y0) * 0.5;
  yShift = (float)((float)((float)(0.5 / (float)(2 * (int)dx.singleSampleDepthStencilSurface)) + 0.0) + y0) * 0.5;
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
  lookupMatrix->m[0][3] = *partitionFraction;
  lookupMatrix->m[1][3] = partitionFraction[1];
  lookupMatrix->m[2][3] = partitionFraction[2];
  lookupMatrix->m[3][3] = partitionFraction[3];
}

void __cdecl R_SunShadowMaps()
{
  int partitionIndex; // [esp+10h] [ebp-8h]
  unsigned int viewIndex; // [esp+14h] [ebp-4h]

  if ( !rgp.world
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_sunshadow.cpp", 775, 0, "%s", "rgp.world") )
  {
    __debugbreak();
  }
  PIXBeginNamedEvent(-1, "shadow maps");
  shadowGlob.defaultShadowCasterTechnique = Material_GetTechnique(
                                              rgp.depthPrepassMaterial,
                                              gfxMetrics.shadowmapBuildTechType);
  for ( partitionIndex = 0; partitionIndex < 2; ++partitionIndex )
  {
    viewIndex = partitionIndex + 1;
    if ( (partitionIndex == -1 || viewIndex > 2)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_sunshadow.cpp",
            791,
            0,
            "%s\n\t(viewIndex) = %i",
            "((viewIndex >= SCENE_VIEW_SUNSHADOW_0) && (viewIndex <= SCENE_VIEW_SUNSHADOW_1))",
            viewIndex) )
    {
      __debugbreak();
    }
    R_AddWorldSurfacesFrustumOnly(viewIndex);
  }
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

void __cdecl R_MergeAndEmitSunShadowMapsSurfs(GfxViewInfo *viewInfo)
{
  float *dir; // [esp+Ch] [ebp-18h]
  int firstDrawSurf; // [esp+10h] [ebp-14h]
  GfxDrawSurfListInfo *info; // [esp+14h] [ebp-10h]
  unsigned int partitionIndex; // [esp+18h] [ebp-Ch]
  GfxSunShadow *sunShadow; // [esp+1Ch] [ebp-8h]

  sunShadow = &frontEndDataOut->sunShadow;
  PIXBeginNamedEvent(-1, "emit sun shadow map surfs");
  if ( frontEndDataOut->sunLight.type != 1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_sunshadow.cpp",
          815,
          0,
          "%s",
          "frontEndDataOut->sunLight.type == GFX_LIGHT_TYPE_DIR") )
  {
    __debugbreak();
  }
  for ( partitionIndex = 0; partitionIndex < 2; ++partitionIndex )
  {
    info = &viewInfo->drawList[partitionIndex + 8];
    R_InitDrawSurfListInfo(info);
    info->baseTechType = gfxMetrics.shadowmapBuildTechType;
    info->viewInfo = viewInfo;
    dir = frontEndDataOut->sunLight.dir;
    info->viewOrigin[0] = frontEndDataOut->sunLight.dir[0];
    info->viewOrigin[1] = dir[1];
    info->viewOrigin[2] = dir[2];
    info->viewOrigin[3] = *(float *)&FLOAT_0_0;
    if ( viewInfo->drawList[partitionIndex + 8].cameraView )
    {
      if ( !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_sunshadow.cpp",
              835,
              0,
              "%s",
              "!info->cameraView") )
        __debugbreak();
    }
    firstDrawSurf = frontEndDataOut->drawSurfCount;
    R_MergeAndEmitDrawSurfLists(4 * partitionIndex + 19, 1u, frontEndDataOut);
    R_MergeAndEmitDrawSurfLists(4 * partitionIndex + 20, 3u, frontEndDataOut);
    info->drawSurfs = &frontEndDataOut->drawSurfs[firstDrawSurf];
    viewInfo->drawList[partitionIndex + 8].drawSurfCount = frontEndDataOut->drawSurfCount - firstDrawSurf;
    sunShadow->partition[partitionIndex].partitionIndex = partitionIndex;
  }
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

