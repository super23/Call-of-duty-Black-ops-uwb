#include "r_dobj_skin.h"

void __cdecl R_FlagXModelAsSkinned(GfxSceneEntity *sceneEnt, unsigned int surfaceCount)
{
  if ( sceneEnt->cull.state != 3
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp",
          954,
          0,
          "%s\n\t(sceneEnt->cull.state) = %i",
          "(sceneEnt->cull.state == CULL_STATE_SKINNED_PENDING)",
          sceneEnt->cull.state) )
  {
    __debugbreak();
  }
  sceneEnt->cull.state = surfaceCount + 4;
}

void __cdecl R_SkinSceneDObj(
        GfxSceneEntity *sceneEnt,
        GfxSceneEntity *localSceneEnt,
        const DObj *obj,
        DObjAnimMat *boneMatrix,
        int waitForCullState)
{
  volatile unsigned int state; // [esp+0h] [ebp-8h]
  unsigned int surfaceCount; // [esp+4h] [ebp-4h]

  if ( !localSceneEnt
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp", 1008, 0, "%s", "localSceneEnt") )
  {
    __debugbreak();
  }
  if ( !boneMatrix
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp", 1009, 0, "%s", "boneMatrix") )
  {
    __debugbreak();
  }
  if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp", 1010, 0, "%s", "obj") )
    __debugbreak();
  if ( localSceneEnt->cull.state < 4 )
  {
    if ( _InterlockedCompareExchange((volatile signed __int32 *)&sceneEnt->cull, 3, 2) == 2 )
    {
      surfaceCount = R_SkinSceneDObjModels(localSceneEnt, obj, boneMatrix);
      R_FlagXModelAsSkinned(localSceneEnt, surfaceCount);
    }
    else if ( waitForCullState )
    {
      do
      {
        state = sceneEnt->cull.state;
        if ( state < 3
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp",
                1043,
                0,
                "%s\n\t(state) = %i",
                "(state >= CULL_STATE_SKINNED_PENDING)",
                state) )
        {
          __debugbreak();
        }
      }
      while ( state == 3 );
      if ( state < 4
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp",
              1046,
              0,
              "%s\n\t(state) = %i",
              "(state >= CULL_STATE)",
              state) )
      {
        __debugbreak();
      }
    }
  }
}

unsigned int  R_SkinSceneDObjModels@<eax>(
        int a1@<ebp>,
        GfxSceneEntity *sceneEnt,
        const DObj *obj,
        DObjAnimMat *boneMatrix)
{
  void *v4; // esp
  const char *Name; // eax
  unsigned __int8 *v7; // [esp-1D38h] [ebp-1D44h]
  signed __int32 startSurfPos; // [esp-1D34h] [ebp-1D40h]
  signed __int32 firstSurf; // [esp-1D2Ch] [ebp-1D38h]
  unsigned int vertsSize; // [esp-1D24h] [ebp-1D30h]
  int v11; // [esp-1D20h] [ebp-1D2Ch]
  unsigned __int8 *v12; // [esp-1D18h] [ebp-1D24h]
  GfxModelRigidSurface *rigidSurf2_; // [esp-1D18h] [ebp-1D24h]
  unsigned int offset_; // [esp-1D14h] [ebp-1D20h]
  unsigned int offset; // [esp-1D14h] [ebp-1D20h]
  unsigned __int8 *v16; // [esp-1D10h] [ebp-1D1Ch]
  GfxModelSkinnedSurface *surfPos2; // [esp-1D10h] [ebp-1D1Ch]
  GfxSkinCacheEntry *SkinCacheEntry; // [esp-1D0Ch] [ebp-1D18h]
  unsigned int skinnedCachedOffset; // [esp-1D08h] [ebp-1D14h]
  int skinnedCacheOffset; // [esp-1D04h] [ebp-1D10h]
  unsigned __int8 *v21; // [esp-1CFCh] [ebp-1D08h]
  int v22; // [esp-1CF8h] [ebp-1D04h]
  unsigned int totalSurfSize; // [esp-1CF8h] [ebp-1D04h]
  int ii; // [esp-1CF4h] [ebp-1D00h]
  unsigned int n; // [esp-1CF0h] [ebp-1CFCh]
  int v26; // [esp-1CECh] [ebp-1CF8h]
  int v27; // [esp-1CE8h] [ebp-1CF4h] BYREF
  int v28; // [esp-1CE4h] [ebp-1CF0h]
  int v29; // [esp-1CE0h] [ebp-1CECh]
  int v30; // [esp-1CDCh] [ebp-1CE8h]
  int v31; // [esp-1CD8h] [ebp-1CE4h]
  int m; // [esp-1CD4h] [ebp-1CE0h]
  XSurface *v33; // [esp-1CD0h] [ebp-1CDCh]
  unsigned int k; // [esp-1CCCh] [ebp-1CD8h]
  XSurface *v35; // [esp-1CC8h] [ebp-1CD4h]
  int v36; // [esp-1CC4h] [ebp-1CD0h]
  int v37; // [esp-1CC0h] [ebp-1CCCh]
  unsigned int v38; // [esp-1CBCh] [ebp-1CC8h]
  unsigned int Surfaces; // [esp-1CB8h] [ebp-1CC4h]
  XSurface *v40; // [esp-1CB4h] [ebp-1CC0h] BYREF
  GfxModelSurfaceInfo v41; // [esp-1CB0h] [ebp-1CBCh] BYREF
  int v42; // [esp-1CA4h] [ebp-1CB0h]
  const cpose_t *Model; // [esp-1CA0h] [ebp-1CACh]
  int v44; // [esp-1C9Ch] [ebp-1CA8h]
  unsigned int v45; // [esp-1C98h] [ebp-1CA4h]
  unsigned int v46[5]; // [esp-1C94h] [ebp-1CA0h] BYREF
  SkinXModelCmd v47; // [esp-1C80h] [ebp-1C8Ch] BYREF
  int j; // [esp-1C50h] [ebp-1C5Ch]
  unsigned int v49[9]; // [esp-1C4Ch] [ebp-1C58h]
  int i; // [esp-1C28h] [ebp-1C34h]
  unsigned int totalSurfaceCount; // [esp-1C24h] [ebp-1C30h]
  unsigned int NumModels; // [esp-1C20h] [ebp-1C2Ch]
  unsigned int v53; // [esp-1C1Ch] [ebp-1C28h]
  int numSkinnedVerts; // [esp-1C18h] [ebp-1C24h] BYREF
  unsigned __int8 *v55; // [esp-1C14h] [ebp-1C20h]
  unsigned __int8 v56[7184]; // [esp-1C10h] [ebp-1C1Ch] BYREF
  int v57; // [esp+0h] [ebp-Ch]
  void *v58; // [esp+4h] [ebp-8h]
  void *retaddr; // [esp+Ch] [ebp+0h]

  v57 = a1;
  v58 = retaddr;
  v4 = alloca(7488);
  if ( sceneEnt->cull.skinnedSurfs.firstSurf
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp",
          313,
          0,
          "%s",
          "!sceneEnt->cull.skinnedSurfs.firstSurf") )
  {
    __debugbreak();
  }
  if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp", 314, 0, "%s", "obj") )
    __debugbreak();
  if ( !boneMatrix
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp", 315, 0, "%s", "boneMatrix") )
  {
    __debugbreak();
  }
  if ( !useFastFile->current.enabled
    && DObjBad(obj)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp",
          318,
          0,
          "%s",
          "!IsObjFileLoad() || !DObjBad( obj )") )
  {
    __debugbreak();
  }
  v55 = v56;
  numSkinnedVerts = 0;
  v53 = 0;
  NumModels = DObjGetNumModels(obj);
  if ( !NumModels )
    return 0;
  totalSurfaceCount = 0;
  for ( i = 0; i < 4; ++i )
    v49[i] = 0;
  for ( j = 0; j < 5; ++j )
    v47.surfacePartBits[j] = 0;
  DObjGetHidePartBits(obj, v46);
  v47.viewoffset[0] = scene.def.viewOffset[0];
  v47.viewoffset[1] = scene.def.viewOffset[1];
  v47.viewoffset[2] = scene.def.viewOffset[2];
  v45 = 0;
  while ( v45 < NumModels )
  {
    Model = (const cpose_t *)DObjGetModel(obj, v45);
    if ( !Model
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp", 373, 0, "%s", "model") )
    {
      __debugbreak();
    }
    v44 = XModelNumBones(Model);
    v42 = sceneEnt->cull.lods[v45];
    if ( v42 >= 0 )
    {
      v41.boneIndex = v53;
      v41.baseMat = XModelGetBasePose((const XModel *)Model);
      v41.boneCount = v44;
      v41.gfxEntIndex = sceneEnt->gfxEntIndex;
      v41.lightingHandle = 0;
      Surfaces = XModelGetSurfaces((const XModel *)Model, &v40, v42);
      if ( !v40
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp", 401, 0, "%s", "surfaces") )
      {
        __debugbreak();
      }
      if ( !Surfaces
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp",
              402,
              0,
              "%s",
              "surfaceCount") )
      {
        __debugbreak();
      }
      totalSurfaceCount += Surfaces;
      v41.dobjModelIndex = v45;
      v38 = v53 >> 5;
      v37 = v53 & 0x1F;
      v36 = 32 - v37;
      v35 = v40;
      for ( k = 0; k < Surfaces; ++k )
      {
        v33 = &v35[k];
        for ( m = 0; m < 5; ++m )
          v49[m + 4] = v33->partBits[m];
        if ( v37 )
        {
          v27 = v49[4 - v38] >> v37;
          v28 = (v49[5 - v38] >> v37) | (v49[4 - v38] << v36);
          v29 = (v49[6 - v38] >> v37) | (v49[5 - v38] << v36);
          v30 = (v49[7 - v38] >> v37) | (v49[6 - v38] << v36);
          v31 = (v49[8 - v38] >> v37) | (v49[7 - v38] << v36);
        }
        else
        {
          v27 = v49[4 - v38];
          v28 = v49[5 - v38];
          v29 = v49[6 - v38];
          v30 = v49[7 - v38];
          v31 = v49[8 - v38];
        }
        if ( DObjIsRecorded(obj) )
          DObjSkelSetSkel(obj, &v27);
        if ( !r_useHidePartbits->current.enabled )
          goto LABEL_52;
        v26 = 0;
        for ( n = 0; n < 5; ++n )
          v26 |= *(&v27 + n) & v46[n];
        if ( v26 )
        {
          *(unsigned int *)v55 = -3;
          v55 += 4;
        }
        else
        {
LABEL_52:
          for ( ii = 0; ii < 5; ++ii )
            v47.surfacePartBits[ii] |= *(&v27 + ii);
          v22 = R_PreSkinXSurface(obj, v33, &v41, (unsigned int *)&numSkinnedVerts, v55);
          v21 = v55;
          *((unsigned int *)v55 + 1) = &v40[k];
          *(GfxModelSurfaceInfo *)(v21 + 8) = v41;
          if ( !v22
            && !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp",
                  544,
                  0,
                  "%s",
                  "surfBufSize") )
          {
            __debugbreak();
          }
          v55 += v22;
        }
      }
    }
    ++v45;
    v53 += v44;
  }
  if ( !DObjSkelAreBonesUpToDate(obj, v47.surfacePartBits)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp",
          554,
          0,
          "%s",
          "DObjSkelAreBonesUpToDate( obj, skinCmd.surfacePartBits )") )
  {
    __debugbreak();
  }
  if ( numSkinnedVerts )
  {
    if ( gfxBuf_skinCache )
    {
      skinnedCacheOffset = R_AllocSkinnedCachedVerts(
                             numSkinnedVerts,
                             (frontEndDataOut->gfxEnts[sceneEnt->gfxEntIndex].renderFxFlags & 1) != 0);
      if ( skinnedCacheOffset < 0 )
      {
        R_WarnOncePerFrame(R_WARN_MAX_SKINNED_CACHE_VERTICES);
        return 0;
      }
      skinnedCachedOffset = 0x80000001;
      if ( gfxBuf_fastSkin )
      {
        SkinCacheEntry = CG_GetSkinCacheEntry(sceneEnt->info.pose);
        if ( SkinCacheEntry->ageCount < 3u
          && dword_B472528 - SkinCacheEntry->frameCount == 1
          && SkinCacheEntry->numSkinnedVerts == numSkinnedVerts )
        {
          skinnedCachedOffset = SkinCacheEntry->skinnedCachedOffset;
          ++SkinCacheEntry->ageCount;
        }
        else
        {
          SkinCacheEntry->ageCount = 0;
        }
        SkinCacheEntry->frameCount = dword_B472528;
        SkinCacheEntry->numSkinnedVerts = numSkinnedVerts;
        if ( SkinCacheEntry->numSkinnedVerts != numSkinnedVerts
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp",
                625,
                0,
                "%s",
                "skinCacheEntry->numSkinnedVerts == numSkinnedVerts") )
        {
          __debugbreak();
        }
        SkinCacheEntry->skinnedCachedOffset = skinnedCacheOffset;
      }
      v16 = v56;
      for ( offset_ = 0; offset_ < totalSurfaceCount; ++offset_ )
      {
        v12 = v16;
        if ( *(unsigned int *)v16 == -2 )
        {
          v16 += 56;
        }
        else if ( *(unsigned int *)v16 == -3 )
        {
          v16 += 4;
        }
        else
        {
          v16 += 24;
          v11 = 32 * *(unsigned int *)v12;
          *(unsigned int *)v12 = v11 + skinnedCacheOffset;
          *((unsigned int *)v12 + 5) = v11 + skinnedCachedOffset;
        }
      }
      if ( v55 != v16
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp",
              679,
              0,
              "%s",
              "surfPos == (byte *)surfPos2") )
      {
        __debugbreak();
      }
    }
    else
    {
      vertsSize = 32 * numSkinnedVerts;
      if ( !frontEndDataOut->tempSkinBuf
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp",
              692,
              0,
              "%s",
              "frontEndDataOut->tempSkinBuf") )
      {
        __debugbreak();
      }
      firstSurf = _InterlockedExchangeAdd(&frontEndDataOut->tempSkinPos, vertsSize);
      if ( vertsSize + firstSurf > (unsigned int)&loc_800000 )
      {
        R_WarnOncePerFrame(R_WARN_TEMP_SKIN_BUF_SIZE);
        return 0;
      }
      Z_VirtualCommit((char *)&frontEndDataOut->tempSkinBuf[firstSurf], vertsSize, 22);
      surfPos2 = (GfxModelSkinnedSurface *)v56;
      for ( offset = 0; offset < totalSurfaceCount; ++offset )
      {
        rigidSurf2_ = (GfxModelRigidSurface *)surfPos2;
        if ( surfPos2->skinnedCachedOffset == -2 )
        {
          surfPos2 = (GfxModelSkinnedSurface *)((char *)surfPos2 + 56);
        }
        else if ( surfPos2->skinnedCachedOffset == -3 )
        {
          surfPos2 = (GfxModelSkinnedSurface *)((char *)surfPos2 + 4);
        }
        else
        {
          ++surfPos2;
          rigidSurf2_->surf.oldSkinnedCachedOffset = (int)&frontEndDataOut->tempSkinBuf[32
                                                                                      * rigidSurf2_->surf.skinnedCachedOffset
                                                                                      + firstSurf];
          rigidSurf2_->surf.skinnedCachedOffset = -1;
        }
      }
      if ( v55 != (unsigned __int8 *)surfPos2
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp",
              753,
              0,
              "%s",
              "surfPos == (byte *)surfPos2") )
      {
        __debugbreak();
      }
    }
  }
  totalSurfSize = v55 - v56;
  startSurfPos = _InterlockedExchangeAdd(&frontEndDataOut->surfPos, v55 - v56);
  if ( totalSurfSize + startSurfPos <= 0x40000 )
  {
    if ( (startSurfPos & 3) != 0
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp",
            854,
            0,
            "%s",
            "!(startSurfPos & 3)") )
    {
      __debugbreak();
    }
    v7 = &frontEndDataOut->surfsBuffer[startSurfPos];
    sceneEnt->cull.skinnedSurfs.firstSurf = &frontEndDataOut->surfsBuffer[startSurfPos];
    memcpy(v7, v56, totalSurfSize);
    if ( !totalSurfaceCount
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp",
            883,
            0,
            "%s",
            "totalSurfaceCount") )
    {
      __debugbreak();
    }
    if ( r_xdebug->current.integer )
      R_XModelDebug(obj, v47.surfacePartBits);
    if ( numSkinnedVerts )
    {
      v47.modelSurfs = v7;
      v47.surfCount = totalSurfaceCount;
      v47.mat = boneMatrix;
      Sys_AddWorkerCmdInternal(&r_model_skinWorkerCmd, &v47, 0);
    }
    return totalSurfaceCount;
  }
  else
  {
    Name = DObjGetName(obj);
    Com_PrintWarning(
      1,
      "FAILED to allocate surfs on frame %u: DObj %s needs %i surfs\n",
      rg.frontEndFrameCount,
      Name,
      totalSurfSize);
    R_WarnOncePerFrame(R_WARN_MAX_SCENE_SURFS_SIZE);
    return 0;
  }
}

int __cdecl R_AllocSkinnedCachedVerts(int vertCount, bool highPriority)
{
  volatile int offset; // [esp+4h] [ebp-Ch]
  unsigned int size; // [esp+8h] [ebp-8h]
  unsigned int MAX_SKINNED_CACHE_VERTICES; // [esp+Ch] [ebp-4h]

  if ( vertCount <= 0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp", 54, 0, "%s", "vertCount > 0") )
  {
    __debugbreak();
  }
  if ( !r_drawSkinnedModels->current.enabled )
    return -1;
  if ( !gfxBuf.skinnedCacheLockAddr )
    return -1;
  g_skinErrorFlags.0 = 0;
  size = 32 * vertCount;
  MAX_SKINNED_CACHE_VERTICES = highPriority ? 0x40000 : 257024;
  while ( 1 )
  {
    offset = frontEndDataOut->skinnedCacheVb->used;
    if ( size + offset > 32 * MAX_SKINNED_CACHE_VERTICES )
      break;
    if ( _InterlockedCompareExchange(&frontEndDataOut->skinnedCacheVb->used, size + offset, offset) == offset )
      goto LABEL_13;
  }
  offset = 32 * MAX_SKINNED_CACHE_VERTICES;
LABEL_13:
  if ( size + offset > (unsigned int)&loc_6AACC0 )
    g_skinErrorFlags.0 = ($4BAAEF421A7DA1E87559C5E7C38D0061)(g_skinErrorFlags.allbits | 2);
  if ( size + offset > 0x7D8000 )
    g_skinErrorFlags.0 = ($4BAAEF421A7DA1E87559C5E7C38D0061)(g_skinErrorFlags.allbits | 1);
  if ( size + offset > 32 * MAX_SKINNED_CACHE_VERTICES )
    return -1;
  else
    return offset;
}

// local variable allocation has failed, the output may be wrong!
int  R_PreSkinXSurface@<eax>(
        GfxModelRigidSurface *a1@<ebp>,
        const DObj *obj,
        XSurface *surf,
        const GfxModelSurfaceInfo *surfaceInfo,
        unsigned int *numSkinnedVerts,
        GfxModelRigidSurface *surfPos)
{
  float origin_[3]; // [esp-8h] [ebp-D8h] OVERLAPPED BYREF
  float origin[3]; // [esp+4h] [ebp-CCh] BYREF
  DObjSkelMat skelMat; // [esp+14h] [ebp-BCh] BYREF
  const DObjAnimMat *mat_1; // [esp+60h] [ebp-70h]
  DObjSkelMat invBaseMat; // [esp+64h] [ebp-6Ch] OVERLAPPED BYREF
  const DObjAnimMat *mat; // [esp+ACh] [ebp-24h]
  int offset; // [esp+B0h] [ebp-20h]
  DObjAnimMat *RotTransArray; // [esp+B4h] [ebp-1Ch]
  GfxModelRigidSurface *rigidSurf_1; // [esp+B8h] [ebp-18h]
  unsigned int boneOffset; // [esp+BCh] [ebp-14h]
  const DObjAnimMat *boneMatrix; // [esp+C0h] [ebp-10h]
  GfxModelRigidSurface *rigidSurf; // [esp+C4h] [ebp-Ch]
  void *v19; // [esp+C8h] [ebp-8h]
  void *retaddr; // [esp+D0h] [ebp+0h]

  rigidSurf = a1;
  v19 = retaddr;
  if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp", 165, 0, "%s", "obj") )
    __debugbreak();
  if ( !surf && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp", 166, 0, "%s", "surf") )
    __debugbreak();
  if ( !surfaceInfo
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp", 167, 0, "%s", "surfaceInfo") )
  {
    __debugbreak();
  }
  if ( (surf->flags & 0x80) == 0
    && (HIBYTE(boneMatrix) = useFastFile->current.enabled, boneOffset = HIBYTE(boneMatrix) != 0, HIBYTE(boneMatrix))
    && surf->vertListCount == 1 )
  {
    surf->vertList = surf->vertList;
    rigidSurf_1 = surfPos;
    surfPos->surf.skinnedCachedOffset = -2;
    if ( rigidSurf_1 != surfPos
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp",
            191,
            0,
            "%s",
            "&rigidSurf->surf == reinterpret_cast< GfxModelSkinnedSurface * >( surfPos )") )
    {
      __debugbreak();
    }
    rigidSurf_1->placement.scale = FLOAT_1_0;
    RotTransArray = &DObjGetRotTransArray(obj)[surfaceInfo->boneIndex];
    offset = surf->vertList->boneOffset >> 6;
    mat = &surfaceInfo->baseMat[offset];
    ConvertQuatToInverseSkelMat(mat, &invBaseMat);
    mat_1 = &RotTransArray[offset];
    ConvertQuatToSkelMat(mat_1, &skelMat);
    QuatMultiplyInverse(mat->quat, mat_1->quat, origin);
    Vec4Normalize(origin);
    Vec4Copy(origin, rigidSurf_1->placement.base.quat);
    R_TransformSkelMat(invBaseMat.origin, (const float4 *)&skelMat, origin_);
    Vec3Copy(origin_, rigidSurf_1->placement.base.origin);
    return 56;
  }
  else
  {
    surfPos->surf.skinnedCachedOffset = *numSkinnedVerts;
    *numSkinnedVerts += surf->vertCount;
    return 24;
  }
}

void __cdecl QuatMultiplyInverse(const float *in1, const float *in2, float *out)
{
  *out = (float)((float)((float)(COERCE_FLOAT(*(unsigned int *)in1 ^ _mask__NegFloat_) * in2[3]) + (float)(in1[3] * *in2))
               - (float)(in1[2] * in2[1]))
       + (float)(in1[1] * in2[2]);
  out[1] = (float)((float)((float)(COERCE_FLOAT(*((unsigned int *)in1 + 1) ^ _mask__NegFloat_) * in2[3])
                         + (float)(in1[2] * *in2))
                 + (float)(in1[3] * in2[1]))
         - (float)(*in1 * in2[2]);
  out[2] = (float)((float)((float)(COERCE_FLOAT(*((unsigned int *)in1 + 2) ^ _mask__NegFloat_) * in2[3])
                         - (float)(in1[1] * *in2))
                 + (float)(*in1 * in2[1]))
         + (float)(in1[3] * in2[2]);
  out[3] = (float)((float)((float)(in1[3] * in2[3]) + (float)(*in1 * *in2)) + (float)(in1[1] * in2[1]))
         + (float)(in1[2] * in2[2]);
}

void __cdecl R_TransformSkelMat(const float *in1, const float4 *in2, float *out)
{
  *out = (float)((float)((float)(*in1 * in2->v[0]) + (float)(in1[1] * in2[1].v[0])) + (float)(in1[2] * in2[2].v[0]))
       + in2[3].v[0];
  out[1] = (float)((float)((float)(*in1 * in2->v[1]) + (float)(in1[1] * in2[1].v[1])) + (float)(in1[2] * in2[2].v[1]))
         + in2[3].v[1];
  out[2] = (float)((float)((float)(*in1 * in2->v[2]) + (float)(in1[1] * in2[1].v[2])) + (float)(in1[2] * in2[2].v[2]))
         + in2[3].v[2];
}

void __cdecl R_SkinGfxEntityCmd(GfxSceneEntity **data)
{
  DObjAnimMat *boneMatrix; // [esp+0h] [ebp-14h]
  const DObj *obj; // [esp+4h] [ebp-10h] BYREF
  GfxSceneEntity *localSceneEnt; // [esp+8h] [ebp-Ch] BYREF
  GfxSceneEntity *sceneEnt; // [esp+Ch] [ebp-8h]
  GfxSceneEntity **pSceneEnt; // [esp+10h] [ebp-4h]
  int savedregs; // [esp+14h] [ebp+0h] BYREF

  if ( !data && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp", 1092, 0, "%s", "data") )
    __debugbreak();
  pSceneEnt = data;
  sceneEnt = *data;
  boneMatrix = R_UpdateSceneEntBounds((GfxSceneEntity *)&savedregs, sceneEnt, &localSceneEnt, &obj, 0);
  if ( boneMatrix )
  {
    if ( !localSceneEnt
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_dobj_skin.cpp",
            1107,
            0,
            "%s",
            "localSceneEnt") )
    {
      __debugbreak();
    }
    R_SkinSceneDObj(sceneEnt, localSceneEnt, obj, boneMatrix, 0);
  }
}

