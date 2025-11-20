#include "r_rope_render.h"

void __cdecl R_Rope_GenerateVerts_Camera(RopeGenerateVertsCmd *cmd)
{
  R_Rope_GenerateVerts_Internal(cmd, 0, 9u, 0);
}

void __cdecl R_Rope_GenerateVerts_Internal(
        const RopeGenerateVertsCmd *cmd,
        char sceneIndex,
        unsigned int drawsurfRegion,
        bool forceLowResLOD)
{
  double v4; // st7
  __int16 v5; // [esp+0h] [ebp-52Ch]
  __int16 v6; // [esp+4h] [ebp-528h]
  __int16 v7; // [esp+8h] [ebp-524h]
  __int16 v8; // [esp+Ch] [ebp-520h]
  __int16 v9; // [esp+10h] [ebp-51Ch]
  __int16 v10; // [esp+14h] [ebp-518h]
  __int16 v11; // [esp+18h] [ebp-514h]
  __int16 v12; // [esp+1Ch] [ebp-510h]
  __int16 v13; // [esp+20h] [ebp-50Ch]
  __int16 v14; // [esp+24h] [ebp-508h]
  const unsigned __int16 *v15; // [esp+28h] [ebp-504h]
  bool v16; // [esp+30h] [ebp-4FCh]
  int v17; // [esp+38h] [ebp-4F4h]
  int v18; // [esp+4Ch] [ebp-4E0h]
  PackedUnitVec v19; // [esp+68h] [ebp-4C4h]
  PackedUnitVec v20; // [esp+84h] [ebp-4A8h]
  int v21; // [esp+A4h] [ebp-488h]
  int v22; // [esp+B8h] [ebp-474h]
  PackedUnitVec v23; // [esp+D4h] [ebp-458h]
  PackedUnitVec v24; // [esp+F0h] [ebp-43Ch]
  int v25; // [esp+110h] [ebp-41Ch]
  int v26; // [esp+124h] [ebp-408h]
  PackedUnitVec v27; // [esp+140h] [ebp-3ECh]
  PackedUnitVec v28; // [esp+15Ch] [ebp-3D0h]
  int v29; // [esp+17Ch] [ebp-3B0h]
  int v30; // [esp+190h] [ebp-39Ch]
  PackedUnitVec v31; // [esp+1ACh] [ebp-380h]
  PackedUnitVec v32; // [esp+1C8h] [ebp-364h]
  int v33; // [esp+1E8h] [ebp-344h]
  int v34; // [esp+1FCh] [ebp-330h]
  PackedUnitVec v35; // [esp+218h] [ebp-314h]
  PackedUnitVec v36; // [esp+234h] [ebp-2F8h]
  float *v37; // [esp+254h] [ebp-2D8h]
  float *v38; // [esp+25Ch] [ebp-2D0h]
  __int64 b_4; // [esp+2B8h] [ebp-274h]
  __int64 a; // [esp+2C4h] [ebp-268h]
  float a_8; // [esp+2CCh] [ebp-260h]
  float v42; // [esp+2D8h] [ebp-254h]
  float v43; // [esp+2DCh] [ebp-250h]
  float other_4; // [esp+2E8h] [ebp-244h]
  float other_8; // [esp+2ECh] [ebp-240h]
  __int64 nrm2; // [esp+2F4h] [ebp-238h]
  float nrm2_8; // [esp+2FCh] [ebp-230h]
  __int64 v; // [esp+314h] [ebp-218h] BYREF
  unsigned int v49; // [esp+31Ch] [ebp-210h]
  float4 tpos2; // [esp+320h] [ebp-20Ch]
  float4 nrm3; // [esp+330h] [ebp-1FCh]
  float invHalfCircumfenceTimes4; // [esp+340h] [ebp-1ECh]
  float4 segDiff; // [esp+344h] [ebp-1E8h] BYREF
  float4 cameraVec; // [esp+354h] [ebp-1D8h] BYREF
  float4 nrm0; // [esp+364h] [ebp-1C8h]
  float4 nrm4; // [esp+374h] [ebp-1B8h]
  float4 perpUpDir; // [esp+384h] [ebp-1A8h] BYREF
  float4 tpos3; // [esp+394h] [ebp-198h]
  float4 lerpedRadius; // [esp+3A4h] [ebp-188h]
  float4 basePos; // [esp+3B4h] [ebp-178h]
  float4 tpos0; // [esp+3C4h] [ebp-168h]
  float4 nrm1; // [esp+3D4h] [ebp-158h]
  float4 tpos4; // [esp+3E4h] [ebp-148h]
  float alpha; // [esp+3F4h] [ebp-138h]
  float4 perpDir; // [esp+3F8h] [ebp-134h] BYREF
  float4 tpos1; // [esp+408h] [ebp-124h]
  int i; // [esp+418h] [ebp-114h]
  float endRadius; // [esp+41Ch] [ebp-110h]
  r_double_index_t workingIndex; // [esp+420h] [ebp-10Ch]
  int indexPairIter; // [esp+424h] [ebp-108h]
  r_double_index_t *baseIndices; // [esp+428h] [ebp-104h] BYREF
  bool useLowResLOD; // [esp+42Fh] [ebp-FDh]
  int ropeTextureUDist; // [esp+430h] [ebp-FCh]
  int segIter; // [esp+434h] [ebp-F8h]
  float4 distVec; // [esp+438h] [ebp-F4h]
  int INDEX_COUNT; // [esp+448h] [ebp-E4h]
  float4 *baseArgs; // [esp+44Ch] [ebp-E0h]
  float4 dist; // [esp+450h] [ebp-DCh]
  float4 beamWorldEnd; // [esp+460h] [ebp-CCh]
  unsigned int primaryLightIndex; // [esp+470h] [ebp-BCh]
  const unsigned __int16 *templateIndices; // [esp+474h] [ebp-B8h]
  float4 ropeCenter; // [esp+478h] [ebp-B4h] BYREF
  float4 radius; // [esp+488h] [ebp-A4h]
  const VisualRope *vrope; // [esp+498h] [ebp-94h]
  float beginRadius; // [esp+49Ch] [ebp-90h]
  int vertexCount; // [esp+4A0h] [ebp-8Ch]
  unsigned __int16 offset; // [esp+4A4h] [ebp-88h]
  float4 prePos; // [esp+4A8h] [ebp-84h]
  int indexCount; // [esp+4B8h] [ebp-74h]
  GfxPackedVertex *verts; // [esp+4BCh] [ebp-70h]
  int segCount; // [esp+4C0h] [ebp-6Ch]
  r_double_index_t *indices; // [esp+4C4h] [ebp-68h]
  float4 beamWorldBegin; // [esp+4C8h] [ebp-64h]
  GfxColor lerpedColor; // [esp+4D8h] [ebp-54h]
  unsigned int argOffset; // [esp+4DCh] [ebp-50h] BYREF
  unsigned int lightHandle; // [esp+4E0h] [ebp-4Ch]
  int SLICE_COUNT; // [esp+4E4h] [ebp-48h]
  unsigned __int16 baseVertex; // [esp+4E8h] [ebp-44h] BYREF
  float4 *args; // [esp+4ECh] [ebp-40h]
  int TRI_COUNT; // [esp+4F0h] [ebp-3Ch]
  GfxPackedVertex *baseVerts; // [esp+4F4h] [ebp-38h]
  float4 radiusVec; // [esp+4F8h] [ebp-34h] BYREF
  RopeRenderInfo *ropeInfo; // [esp+508h] [ebp-24h]
  float4 viewOrg; // [esp+510h] [ebp-1Ch]
  float (*segVerts)[3]; // [esp+524h] [ebp-8h]
  int vropeIter; // [esp+528h] [ebp-4h]

  R_BeginCodeMeshVerts();
  segVerts = 0;
  if ( !cmd && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_rope_render.cpp", 88, 0, "%s", "cmd") )
    __debugbreak();
  if ( !cmd->ropeInfo
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_rope_render.cpp", 89, 0, "%s", "cmd->ropeInfo") )
  {
    __debugbreak();
  }
  ropeInfo = cmd->ropeInfo;
  *(_QWORD *)viewOrg.v = *(_QWORD *)cmd->vieworg;
  viewOrg.u[2] = LODWORD(cmd->vieworg[2]);
  viewOrg.u[3] = *(unsigned int *)&FLOAT_0_0;
  for ( vropeIter = 0; vropeIter != ropeInfo->ropeCount; ++vropeIter )
  {
    vrope = &ropeInfo->ropes[vropeIter];
    if ( ((1 << sceneIndex) & vrope->visibilityFlags) != 0 )
    {
      beginRadius = vrope->beginRadius;
      endRadius = vrope->endRadius;
      segCount = vrope->segmentCount;
      segVerts = (float (*)[3])vrope->segmentVerts;
      *(_QWORD *)beamWorldBegin.v = *(_QWORD *)segVerts;
      beamWorldBegin.u[2] = LODWORD((*segVerts)[2]);
      beamWorldBegin.u[3] = *(unsigned int *)&FLOAT_0_0;
      *(_QWORD *)beamWorldEnd.v = *(_QWORD *)&segVerts[segCount][0];
      beamWorldEnd.u[2] = LODWORD(segVerts[segCount][2]);
      beamWorldEnd.u[3] = *(unsigned int *)&FLOAT_0_0;
      ropeCenter.v[0] = (float)(beamWorldBegin.v[0] + beamWorldEnd.v[0]) * 0.5;
      ropeCenter.v[1] = (float)(beamWorldBegin.v[1] + beamWorldEnd.v[1]) * 0.5;
      ropeCenter.v[2] = (float)(beamWorldBegin.v[2] + beamWorldEnd.v[2]) * 0.5;
      ropeCenter.v[3] = (float)(0.0 + 0.0) * 0.5;
      radiusVec.v[0] = (float)(beamWorldBegin.v[0] - beamWorldEnd.v[0]) * 0.5;
      radiusVec.v[1] = (float)(beamWorldBegin.v[1] - beamWorldEnd.v[1]) * 0.5;
      radiusVec.v[2] = (float)(beamWorldBegin.v[2] - beamWorldEnd.v[2]) * 0.5;
      radiusVec.v[3] = (float)(0.0 - 0.0) * 0.5;
      radius.u[0] = fsqrt(
                      (float)((float)((float)(radiusVec.v[0] * radiusVec.v[0]) + (float)(radiusVec.v[1] * radiusVec.v[1]))
                            + (float)(radiusVec.v[2] * radiusVec.v[2]))
                    + (float)(radiusVec.v[3] * radiusVec.v[3]));
      radius.u[1] = radius.u[0];
      radius.u[2] = radius.u[0];
      radius.u[3] = radius.u[0];
      distVec.v[0] = viewOrg.v[0] - ropeCenter.v[0];
      distVec.v[1] = viewOrg.v[1] - ropeCenter.v[1];
      distVec.v[2] = viewOrg.v[2] - ropeCenter.v[2];
      distVec.v[3] = viewOrg.v[3] - ropeCenter.v[3];
      dist.u[0] = fsqrt(
                    (float)((float)((float)((float)(viewOrg.v[0] - ropeCenter.v[0])
                                          * (float)(viewOrg.v[0] - ropeCenter.v[0]))
                                  + (float)((float)(viewOrg.v[1] - ropeCenter.v[1])
                                          * (float)(viewOrg.v[1] - ropeCenter.v[1])))
                          + (float)((float)(viewOrg.v[2] - ropeCenter.v[2]) * (float)(viewOrg.v[2] - ropeCenter.v[2])))
                  + (float)((float)(viewOrg.v[3] - ropeCenter.v[3]) * (float)(viewOrg.v[3] - ropeCenter.v[3])));
      dist.u[1] = dist.u[0];
      dist.u[2] = dist.u[0];
      dist.u[3] = dist.u[0];
      primaryLightIndex = R_GetNonSunPrimaryLightForBox(0, ropeCenter.v, radiusVec.v);
      if ( !primaryLightIndex )
        primaryLightIndex = rgp.world->sunPrimaryLightIndex;
      lightHandle = R_AllocModelLighting_PrimaryLight(
                      ropeCenter.v,
                      primaryLightIndex,
                      0,
                      vrope->lightingHandle,
                      &lightingInfo_0);
      if ( lightHandle )
      {
        v16 = forceLowResLOD || (float)(dist.v[0] - radius.v[0]) > gRopeLOD;
        useLowResLOD = v16;
        SLICE_COUNT = v16 ? 1 : 4;
        TRI_COUNT = 2 * SLICE_COUNT;
        INDEX_COUNT = 6 * SLICE_COUNT;
        if ( v16 )
          v15 = templateIndices_low;
        else
          v15 = templateIndices_high;
        templateIndices = v15;
        indexCount = INDEX_COUNT * segCount;
        vertexCount = (SLICE_COUNT + 1) * (segCount + 1);
        if ( !R_ReserveCodeMeshIndices(INDEX_COUNT * segCount, &baseIndices)
          || !R_ReserveCodeMeshVerts(vertexCount, &baseVertex)
          || !R_ReserveCodeMeshArgs(1, &argOffset) )
        {
          break;
        }
        indices = baseIndices;
        indexPairIter = 0;
        for ( segIter = 0; segIter < segCount; ++segIter )
        {
          offset = segIter * (SLICE_COUNT + 1);
          for ( i = 0; i < INDEX_COUNT; i += 2 )
          {
            workingIndex.value[0] = offset + templateIndices[i] + baseVertex;
            workingIndex.value[1] = offset + templateIndices[i + 1] + baseVertex;
            indices[indexPairIter++] = workingIndex;
          }
        }
        if ( indexPairIter != indexCount / 2
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_rope_render.cpp",
                225,
                0,
                "%d,%d",
                indexPairIter,
                indexCount / 2) )
        {
          __debugbreak();
        }
        baseVerts = R_GetCodeMeshVerts(baseVertex);
        verts = baseVerts;
        prePos = beamWorldBegin;
        lerpedColor.packed = -1;
        ropeTextureUDist = *(int *)&FLOAT_0_0;
        for ( segIter = 0; segIter <= segCount; ++segIter )
        {
          alpha = (float)segIter / (float)segCount;
          lerpedRadius.v[0] = (float)((float)(endRadius - beginRadius) * alpha) + beginRadius;
          lerpedRadius.v[1] = lerpedRadius.v[0];
          lerpedRadius.v[2] = lerpedRadius.v[0];
          lerpedRadius.v[3] = lerpedRadius.v[0];
          invHalfCircumfenceTimes4 = 0.25 / (float)(lerpedRadius.v[0] * 3.1415927);
          *(_QWORD *)basePos.v = *(_QWORD *)&segVerts[segIter][0];
          basePos.u[2] = LODWORD(segVerts[segIter][2]);
          basePos.u[3] = *(unsigned int *)&FLOAT_0_0;
          if ( segIter )
          {
            if ( segIter == segCount )
            {
              v38 = segVerts[segIter - 1];
              v42 = v38[1];
              v43 = v38[2];
              segDiff.v[0] = basePos.v[0] - *v38;
              segDiff.v[1] = basePos.v[1] - v42;
              segDiff.v[2] = basePos.v[2] - v43;
              segDiff.v[3] = basePos.v[3] - 0.0;
            }
            else
            {
              a = *(_QWORD *)&segVerts[segIter - 1][0];
              a_8 = segVerts[segIter - 1][2];
              v37 = segVerts[segIter + 1];
              b_4 = *(_QWORD *)(v37 + 1);
              segDiff.v[0] = *v37 - *(float *)&a;
              segDiff.v[1] = *(float *)&b_4 - *((float *)&a + 1);
              segDiff.v[2] = *((float *)&b_4 + 1) - a_8;
              segDiff.v[3] = 0.0 - 0.0;
            }
          }
          else
          {
            other_4 = (*segVerts)[4];
            other_8 = (*segVerts)[5];
            segDiff.v[0] = (*segVerts)[3] - basePos.v[0];
            segDiff.v[1] = other_4 - basePos.v[1];
            segDiff.v[2] = other_8 - basePos.v[2];
            segDiff.v[3] = 0.0 - basePos.v[3];
          }
          Vec3NormalizeTo(segDiff.v, segDiff.v);
          segDiff.u[3] = *(unsigned int *)&FLOAT_0_0;
          cameraVec.v[0] = basePos.v[0] - viewOrg.v[0];
          cameraVec.v[1] = basePos.v[1] - viewOrg.v[1];
          cameraVec.v[2] = basePos.v[2] - viewOrg.v[2];
          cameraVec.v[3] = basePos.v[3] - viewOrg.v[3];
          perpDir.v[0] = basePos.v[0] - viewOrg.v[0];
          perpDir.v[1] = basePos.v[1] - viewOrg.v[1];
          perpDir.v[2] = basePos.v[2] - viewOrg.v[2];
          perpDir.v[3] = basePos.v[3] - viewOrg.v[3];
          Vec3Cross(segDiff.v, cameraVec.v, perpDir.v);
          perpDir.u[3] = *(unsigned int *)&FLOAT_0_0;
          Vec3NormalizeTo(perpDir.v, perpDir.v);
          perpDir.u[3] = *(unsigned int *)&FLOAT_0_0;
          Vec3Cross(segDiff.v, perpDir.v, perpUpDir.v);
          perpUpDir.u[3] = *(unsigned int *)&FLOAT_0_0;
          Vec3NormalizeTo(perpUpDir.v, perpUpDir.v);
          perpUpDir.u[3] = *(unsigned int *)&FLOAT_0_0;
          prePos.v[0] = basePos.v[0] - prePos.v[0];
          prePos.v[1] = basePos.v[1] - prePos.v[1];
          prePos.v[2] = basePos.v[2] - prePos.v[2];
          prePos.v[3] = basePos.v[3] - prePos.v[3];
          v = *(_QWORD *)prePos.v;
          v49 = prePos.u[2];
          v4 = Abs((const float *)&v);
          *(float *)&ropeTextureUDist = v4 * invHalfCircumfenceTimes4 + *(float *)&ropeTextureUDist;
          prePos = basePos;
          nrm0.u[0] = perpDir.u[0] ^ _mask__NegFloat_;
          nrm0.u[1] = perpDir.u[1] ^ _mask__NegFloat_;
          nrm0.u[2] = perpDir.u[2] ^ _mask__NegFloat_;
          nrm0.u[3] = perpDir.u[3] ^ _mask__NegFloat_;
          tpos0.v[0] = (float)(COERCE_FLOAT(perpDir.u[0] ^ _mask__NegFloat_) * lerpedRadius.v[0]) + basePos.v[0];
          tpos0.v[1] = (float)(COERCE_FLOAT(perpDir.u[1] ^ _mask__NegFloat_) * lerpedRadius.v[1]) + basePos.v[1];
          tpos0.v[2] = (float)(COERCE_FLOAT(perpDir.u[2] ^ _mask__NegFloat_) * lerpedRadius.v[2]) + basePos.v[2];
          tpos0.v[3] = (float)(COERCE_FLOAT(perpDir.u[3] ^ _mask__NegFloat_) * lerpedRadius.v[3]) + basePos.v[3];
          nrm1 = perpDir;
          tpos1.v[0] = (float)(perpDir.v[0] * lerpedRadius.v[0]) + basePos.v[0];
          tpos1.v[1] = (float)(perpDir.v[1] * lerpedRadius.v[1]) + basePos.v[1];
          tpos1.v[2] = (float)(perpDir.v[2] * lerpedRadius.v[2]) + basePos.v[2];
          tpos1.v[3] = (float)(perpDir.v[3] * lerpedRadius.v[3]) + basePos.v[3];
          nrm2 = *(_QWORD *)perpUpDir.v;
          nrm2_8 = perpUpDir.v[2];
          tpos2.v[0] = (float)(perpUpDir.v[0] * lerpedRadius.v[0]) + basePos.v[0];
          tpos2.v[1] = (float)(perpUpDir.v[1] * lerpedRadius.v[1]) + basePos.v[1];
          tpos2.v[2] = (float)(perpUpDir.v[2] * lerpedRadius.v[2]) + basePos.v[2];
          tpos2.v[3] = (float)(perpUpDir.v[3] * lerpedRadius.v[3]) + basePos.v[3];
          nrm3.v[0] = COERCE_FLOAT(perpDir.u[0] ^ _mask__NegFloat_) * 0.70710677;
          nrm3.v[1] = COERCE_FLOAT(perpDir.u[1] ^ _mask__NegFloat_) * 0.70710677;
          nrm3.v[2] = COERCE_FLOAT(perpDir.u[2] ^ _mask__NegFloat_) * 0.70710677;
          nrm3.v[3] = COERCE_FLOAT(perpDir.u[3] ^ _mask__NegFloat_) * 0.70710677;
          nrm3.v[0] = (float)(perpUpDir.v[0] * 0.70710677) + nrm3.v[0];
          nrm3.v[1] = (float)(perpUpDir.v[1] * 0.70710677) + nrm3.v[1];
          nrm3.v[2] = (float)(perpUpDir.v[2] * 0.70710677) + nrm3.v[2];
          nrm3.v[3] = (float)(perpUpDir.v[3] * 0.70710677) + nrm3.v[3];
          tpos3.v[0] = (float)(nrm3.v[0] * lerpedRadius.v[0]) + basePos.v[0];
          tpos3.v[1] = (float)(nrm3.v[1] * lerpedRadius.v[1]) + basePos.v[1];
          tpos3.v[2] = (float)(nrm3.v[2] * lerpedRadius.v[2]) + basePos.v[2];
          tpos3.v[3] = (float)(nrm3.v[3] * lerpedRadius.v[3]) + basePos.v[3];
          nrm4.v[0] = (float)(perpUpDir.v[0] * 0.70710677) + (float)(perpDir.v[0] * 0.70710677);
          nrm4.v[1] = (float)(perpUpDir.v[1] * 0.70710677) + (float)(perpDir.v[1] * 0.70710677);
          nrm4.v[2] = (float)(perpUpDir.v[2] * 0.70710677) + (float)(perpDir.v[2] * 0.70710677);
          nrm4.v[3] = (float)(perpUpDir.v[3] * 0.70710677) + (float)(perpDir.v[3] * 0.70710677);
          tpos4.v[0] = (float)(nrm4.v[0] * lerpedRadius.v[0]) + basePos.v[0];
          tpos4.v[1] = (float)(nrm4.v[1] * lerpedRadius.v[1]) + basePos.v[1];
          tpos4.v[2] = (float)(nrm4.v[2] * lerpedRadius.v[2]) + basePos.v[2];
          tpos4.v[3] = (float)(nrm4.v[3] * lerpedRadius.v[3]) + basePos.v[3];
          verts->xyz[0] = tpos0.v[0];
          *(_QWORD *)&verts->xyz[1] = *(_QWORD *)&tpos0.unitVec[1].packed;
          verts->color = lerpedColor;
          verts->binormalSign = FLOAT_1_0;
          v36.array[0] = (int)(float)((float)(nrm0.v[0] * 127.0) + 127.5);
          v36.array[1] = (int)(float)((float)(nrm0.v[1] * 127.0) + 127.5);
          v36.array[2] = (int)(float)((float)(nrm0.v[2] * 127.0) + 127.5);
          v36.array[3] = 63;
          verts->normal = v36;
          v35.array[0] = (int)(float)((float)(segDiff.v[0] * 127.0) + 127.5);
          v35.array[1] = (int)(float)((float)(segDiff.v[1] * 127.0) + 127.5);
          v35.array[2] = (int)(float)((float)(segDiff.v[2] * 127.0) + 127.5);
          v35.array[3] = 63;
          verts->tangent = v35;
          if ( (int)((2 * *(unsigned int *)&FLOAT_0_0) ^ 0x80000000) >> 14 < 0x3FFF )
            v34 = (int)((2 * *(unsigned int *)&FLOAT_0_0) ^ 0x80000000) >> 14;
          else
            v34 = 0x3FFF;
          if ( v34 > -16384 )
            v14 = v34;
          else
            v14 = -16384;
          if ( (int)((2 * ropeTextureUDist) ^ 0x80000000) >> 14 < 0x3FFF )
            v33 = (int)((2 * ropeTextureUDist) ^ 0x80000000) >> 14;
          else
            v33 = 0x3FFF;
          if ( v33 > -16384 )
            v13 = v33;
          else
            v13 = -16384;
          verts->texCoord.packed = (v13 & 0x3FFF | (ropeTextureUDist >> 16) & 0xC000)
                                 + ((v14 & 0x3FFF | (*(int *)&FLOAT_0_0 >> 16) & 0xC000) << 16);
          ++verts;
          if ( !useLowResLOD )
          {
            *(_QWORD *)verts->xyz = *(_QWORD *)tpos3.v;
            LODWORD(verts->xyz[2]) = tpos3.u[2];
            verts->color = lerpedColor;
            verts->binormalSign = FLOAT_1_0;
            v32.array[0] = (int)(float)((float)(nrm3.v[0] * 127.0) + 127.5);
            v32.array[1] = (int)(float)((float)(nrm3.v[1] * 127.0) + 127.5);
            v32.array[2] = (int)(float)((float)(nrm3.v[2] * 127.0) + 127.5);
            v32.array[3] = 63;
            verts->normal = v32;
            v31.array[0] = (int)(float)((float)(segDiff.v[0] * 127.0) + 127.5);
            v31.array[1] = (int)(float)((float)(segDiff.v[1] * 127.0) + 127.5);
            v31.array[2] = (int)(float)((float)(segDiff.v[2] * 127.0) + 127.5);
            v31.array[3] = 63;
            verts->tangent = v31;
            if ( (int)((2 * LODWORD(FLOAT_0_2)) ^ 0x80000000) >> 14 < 0x3FFF )
              v30 = (int)((2 * LODWORD(FLOAT_0_2)) ^ 0x80000000) >> 14;
            else
              v30 = 0x3FFF;
            if ( v30 > -16384 )
              v12 = v30;
            else
              v12 = -16384;
            if ( (int)((2 * ropeTextureUDist) ^ 0x80000000) >> 14 < 0x3FFF )
              v29 = (int)((2 * ropeTextureUDist) ^ 0x80000000) >> 14;
            else
              v29 = 0x3FFF;
            if ( v29 > -16384 )
              v11 = v29;
            else
              v11 = -16384;
            verts->texCoord.packed = (v11 & 0x3FFF | (ropeTextureUDist >> 16) & 0xC000)
                                   + ((v12 & 0x3FFF | (SLODWORD(FLOAT_0_2) >> 16) & 0xC000) << 16);
            ++verts;
            *(_QWORD *)verts->xyz = *(_QWORD *)tpos2.v;
            LODWORD(verts->xyz[2]) = tpos2.u[2];
            verts->color = lerpedColor;
            verts->binormalSign = FLOAT_1_0;
            v28.array[0] = (int)(float)((float)(*(float *)&nrm2 * 127.0) + 127.5);
            v28.array[1] = (int)(float)((float)(*((float *)&nrm2 + 1) * 127.0) + 127.5);
            v28.array[2] = (int)(float)((float)(nrm2_8 * 127.0) + 127.5);
            v28.array[3] = 63;
            verts->normal = v28;
            v27.array[0] = (int)(float)((float)(segDiff.v[0] * 127.0) + 127.5);
            v27.array[1] = (int)(float)((float)(segDiff.v[1] * 127.0) + 127.5);
            v27.array[2] = (int)(float)((float)(segDiff.v[2] * 127.0) + 127.5);
            v27.array[3] = 63;
            verts->tangent = v27;
            if ( (int)((2 * LODWORD(FLOAT_0_40000001)) ^ 0x80000000) >> 14 < 0x3FFF )
              v26 = (int)((2 * LODWORD(FLOAT_0_40000001)) ^ 0x80000000) >> 14;
            else
              v26 = 0x3FFF;
            if ( v26 > -16384 )
              v10 = v26;
            else
              v10 = -16384;
            if ( (int)((2 * ropeTextureUDist) ^ 0x80000000) >> 14 < 0x3FFF )
              v25 = (int)((2 * ropeTextureUDist) ^ 0x80000000) >> 14;
            else
              v25 = 0x3FFF;
            if ( v25 > -16384 )
              v9 = v25;
            else
              v9 = -16384;
            verts->texCoord.packed = (v9 & 0x3FFF | (ropeTextureUDist >> 16) & 0xC000)
                                   + ((v10 & 0x3FFF | (SLODWORD(FLOAT_0_40000001) >> 16) & 0xC000) << 16);
            ++verts;
            *(_QWORD *)verts->xyz = *(_QWORD *)tpos4.v;
            LODWORD(verts->xyz[2]) = tpos4.u[2];
            verts->color = lerpedColor;
            verts->binormalSign = FLOAT_1_0;
            v24.array[0] = (int)(float)((float)(nrm4.v[0] * 127.0) + 127.5);
            v24.array[1] = (int)(float)((float)(nrm4.v[1] * 127.0) + 127.5);
            v24.array[2] = (int)(float)((float)(nrm4.v[2] * 127.0) + 127.5);
            v24.array[3] = 63;
            verts->normal = v24;
            v23.array[0] = (int)(float)((float)(segDiff.v[0] * 127.0) + 127.5);
            v23.array[1] = (int)(float)((float)(segDiff.v[1] * 127.0) + 127.5);
            v23.array[2] = (int)(float)((float)(segDiff.v[2] * 127.0) + 127.5);
            v23.array[3] = 63;
            verts->tangent = v23;
            if ( (int)((2 * LODWORD(FLOAT_0_80000001)) ^ 0x80000000) >> 14 < 0x3FFF )
              v22 = (int)((2 * LODWORD(FLOAT_0_80000001)) ^ 0x80000000) >> 14;
            else
              v22 = 0x3FFF;
            if ( v22 > -16384 )
              v8 = v22;
            else
              v8 = -16384;
            if ( (int)((2 * ropeTextureUDist) ^ 0x80000000) >> 14 < 0x3FFF )
              v21 = (int)((2 * ropeTextureUDist) ^ 0x80000000) >> 14;
            else
              v21 = 0x3FFF;
            if ( v21 > -16384 )
              v7 = v21;
            else
              v7 = -16384;
            verts->texCoord.packed = (v7 & 0x3FFF | (ropeTextureUDist >> 16) & 0xC000)
                                   + ((v8 & 0x3FFF | (SLODWORD(FLOAT_0_80000001) >> 16) & 0xC000) << 16);
            ++verts;
          }
          *(_QWORD *)verts->xyz = *(_QWORD *)tpos1.v;
          LODWORD(verts->xyz[2]) = tpos1.u[2];
          verts->color = lerpedColor;
          verts->binormalSign = FLOAT_1_0;
          v20.array[0] = (int)(float)((float)(nrm1.v[0] * 127.0) + 127.5);
          v20.array[1] = (int)(float)((float)(nrm1.v[1] * 127.0) + 127.5);
          v20.array[2] = (int)(float)((float)(nrm1.v[2] * 127.0) + 127.5);
          v20.array[3] = 63;
          verts->normal = v20;
          v19.array[0] = (int)(float)((float)(segDiff.v[0] * 127.0) + 127.5);
          v19.array[1] = (int)(float)((float)(segDiff.v[1] * 127.0) + 127.5);
          v19.array[2] = (int)(float)((float)(segDiff.v[2] * 127.0) + 127.5);
          v19.array[3] = 63;
          verts->tangent = v19;
          if ( (int)((2 * LODWORD(FLOAT_1_0)) ^ 0x80000000) >> 14 < 0x3FFF )
            v18 = (int)((2 * LODWORD(FLOAT_1_0)) ^ 0x80000000) >> 14;
          else
            v18 = 0x3FFF;
          if ( v18 > -16384 )
            v6 = v18;
          else
            v6 = -16384;
          if ( (int)((2 * ropeTextureUDist) ^ 0x80000000) >> 14 < 0x3FFF )
            v17 = (int)((2 * ropeTextureUDist) ^ 0x80000000) >> 14;
          else
            v17 = 0x3FFF;
          if ( v17 > -16384 )
            v5 = v17;
          else
            v5 = -16384;
          verts->texCoord.packed = (v5 & 0x3FFF | (ropeTextureUDist >> 16) & 0xC000)
                                 + ((v6 & 0x3FFF | (SLODWORD(FLOAT_1_0) >> 16) & 0xC000) << 16);
          ++verts;
        }
        baseArgs = (float4 *)R_GetCodeMeshArgs(argOffset);
        args = baseArgs;
        baseArgs->u[0] = lightHandle;
        args->u[1] = lightHandle;
        args->u[2] = lightHandle;
        args->u[3] = lightHandle;
        R_AddRopeCodeMeshDrawSurf(
          vrope->material,
          baseIndices,
          indexCount,
          argOffset,
          1u,
          primaryLightIndex,
          drawsurfRegion);
      }
    }
  }
  R_EndCodeMeshVerts();
  Sys_AssistAndWaitWorkerCmdInternal(&r_model_lightingWorkerCmd);
}

void __cdecl R_Rope_ClearAll()
{
  g_ropeInfo.ropeCount = 0;
}

void __cdecl R_Rope_Add(const VisualRope *vropeIn)
{
  VisualRope *vr; // [esp+8h] [ebp-4h]

  if ( g_ropeInfo.ropeCount != 96 )
  {
    vr = &g_ropeInfo.ropes[g_ropeInfo.ropeCount];
    memcpy(vr, vropeIn, sizeof(VisualRope));
    vr->drawSurf = 0;
    if ( !vr->lightingHandle
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_rope_render.cpp",
            424,
            0,
            "%s",
            "vr->lightingHandle") )
    {
      __debugbreak();
    }
    ++g_ropeInfo.ropeCount;
  }
}

RopeRenderInfo *__cdecl R_Rope_GetInfo()
{
  return &g_ropeInfo;
}

unsigned __int8 __cdecl R_Rope_CalcVisibility(const float *minmax, unsigned int localClientNum)
{
  signed int sceneIdx; // [esp+0h] [ebp-8h]
  unsigned __int8 visFlag; // [esp+7h] [ebp-1h]

  visFlag = 0;
  for ( sceneIdx = 0; sceneIdx < 3; ++sceneIdx )
  {
    if ( !R_CullBoxCurDpvs_SceneSelect(minmax, localClientNum, sceneIdx) )
      visFlag |= 1 << sceneIdx;
  }
  return visFlag;
}

const Material *__cdecl R_Rope_GetMaterialFromGfxWorld()
{
  return rgp.world->ropeMaterial;
}

