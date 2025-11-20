#include "r_scene.h"

GfxScene *__cdecl R_GetScene()
{
  return &scene;
}

unsigned int __cdecl R_AllocSceneModel()
{
  unsigned int sceneEntIndex; // [esp+0h] [ebp-4h]

  if ( !rg.registered
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 207, 0, "%s", "rg.registered") )
  {
    __debugbreak();
  }
  if ( !rg.inFrame
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 209, 0, "%s", "rg.inFrame") )
  {
    __debugbreak();
  }
  sceneEntIndex = _InterlockedExchangeAdd(&scene.sceneModelCount, 1u);
  if ( sceneEntIndex >= r_modelLimit->current.integer )
  {
    scene.sceneModelCount = r_modelLimit->current.integer;
    R_WarnOncePerFrame(R_WARN_KNOWN_MODELS, r_modelLimit->current.integer);
  }
  return sceneEntIndex;
}

int __cdecl R_AllocTextureOverride(
        unsigned __int16 modelIndexMask,
        const GfxImage *img1,
        GfxImage *img2,
        int prevOverride)
{
  return R_AllocTextureOverride(0, modelIndexMask, img1, img2, prevOverride);
}

int __cdecl R_AllocTextureOverride(
        const Material *material,
        unsigned __int16 modelIndexMask,
        const GfxImage *img1,
        GfxImage *img2,
        int prevOverride)
{
  __int16 texOver; // [esp+4h] [ebp-4h]

  if ( !img1 || !img2 || img1 == img2 )
    return prevOverride;
  if ( !R_StreamTouchImageAndCheck(img2, 0) )
    return prevOverride;
  texOver = _InterlockedExchangeAdd(&frontEndDataOut->textureOverrideCount, 1u);
  if ( (unsigned int)texOver < 0x100 )
  {
    frontEndDataOut->textureOverrides[texOver].material = material;
    frontEndDataOut->textureOverrides[texOver].img1 = img1;
    frontEndDataOut->textureOverrides[texOver].img2 = img2;
    frontEndDataOut->textureOverrides[texOver].dobjModelMask = modelIndexMask;
    frontEndDataOut->textureOverrides[texOver].prev = prevOverride;
    return texOver;
  }
  else
  {
    frontEndDataOut->textureOverrideCount = 256;
    R_WarnOncePerFrame(R_WARN_TEXTURE_OVERRIDES, 256);
    return -1;
  }
}

GfxBrushModel *__cdecl R_GetBrushModel(unsigned int modelIndex)
{
  if ( !rgp.world
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 309, 0, "%s", "rgp.world") )
  {
    __debugbreak();
  }
  if ( modelIndex >= rgp.world->modelCount
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          310,
          0,
          "modelIndex doesn't index rgp.world->modelCount\n\t%i not in [0, %i)",
          modelIndex,
          rgp.world->modelCount) )
  {
    __debugbreak();
  }
  return &rgp.world->models[modelIndex];
}

void __cdecl R_AddBrushModelToSceneFromAngles(
        const GfxBrushModel *bmodel,
        const float *origin,
        const float *angles,
        unsigned __int16 entnum,
        const ShaderConstantSet *constants)
{
  unsigned int sceneEntIndex; // [esp+4h] [ebp-8h]
  GfxSceneBrush *sceneBrush; // [esp+8h] [ebp-4h]

  if ( !bmodel && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 319, 0, "%s", "bmodel") )
    __debugbreak();
  if ( r_drawEntities->current.enabled && r_drawBModels->current.enabled && bmodel->surfaceCount )
  {
    sceneEntIndex = R_AllocSceneBrush();
    if ( sceneEntIndex < 0x400 )
    {
      sceneBrush = &scene.sceneBrush[sceneEntIndex];
      sceneBrush->bmodel = bmodel;
      sceneBrush->entnum = entnum;
      sceneBrush->placement.origin[0] = *origin;
      sceneBrush->placement.origin[1] = origin[1];
      sceneBrush->placement.origin[2] = origin[2];
      AnglesToQuat(angles, sceneBrush->placement.quat);
      sceneBrush->brushConstantSet = r_shader_constant_set_enable->current.enabled ? constants : 0;
    }
  }
}

unsigned int __cdecl R_AllocSceneBrush()
{
  unsigned int sceneEntIndex; // [esp+0h] [ebp-4h]

  if ( !rg.registered
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 293, 0, "%s", "rg.registered") )
  {
    __debugbreak();
  }
  if ( !rg.inFrame
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 295, 0, "%s", "rg.inFrame") )
  {
    __debugbreak();
  }
  sceneEntIndex = _InterlockedExchangeAdd(&scene.sceneBrushCount, 1u);
  if ( sceneEntIndex >= r_brushLimit->current.integer )
  {
    scene.sceneBrushCount = r_brushLimit->current.integer;
    R_WarnOncePerFrame(R_WARN_KNOWN_MODELS, r_brushLimit->current.integer);
  }
  return sceneEntIndex;
}

void __cdecl R_AddGlassBrushToScene(
        const GfxBrushModel *bmodel,
        const float *origin,
        const float *angles,
        Material *altStreamingMaterial)
{
  GfxSceneGlassBrush *glassBrush; // [esp+4h] [ebp-8h]
  unsigned int glassIndex; // [esp+8h] [ebp-4h]

  if ( !bmodel && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 351, 0, "%s", "bmodel") )
    __debugbreak();
  if ( bmodel->surfaceCount )
  {
    if ( !rg.inFrame
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 356, 0, "%s", "rg.inFrame") )
    {
      __debugbreak();
    }
    glassIndex = _InterlockedExchangeAdd(&scene.glassBrushCount, 1u);
    if ( glassIndex < 0x400 )
    {
      glassBrush = &scene.glassBrush[glassIndex];
      glassBrush->bmodel = bmodel;
      glassBrush->altStreamingMaterial = altStreamingMaterial;
      glassBrush->placement.origin[0] = *origin;
      glassBrush->placement.origin[1] = origin[1];
      glassBrush->placement.origin[2] = origin[2];
      AnglesToQuat(angles, glassBrush->placement.quat);
    }
    else
    {
      scene.glassBrushCount = 1024;
      R_WarnOncePerFrame(R_WARN_KNOWN_MODELS, 1024);
    }
  }
}

void __cdecl R_AddDObjToScene(
        const DObj *obj,
        const cpose_t *pose,
        unsigned int entnum,
        unsigned int renderFxFlags,
        float *lightingOrigin,
        float materialTime,
        float materialTime2,
        float burnFraction,
        float wetness,
        char altXModel,
        int textureOverrideIndex,
        const ShaderConstantSet *dobjConstantSet,
        const ShaderConstantSet *dobjConstantSetExtraCam,
        float lightingOriginToleranceSq,
        float scale)
{
  unsigned __int8 v15; // [esp+0h] [ebp-60h]
  unsigned __int8 v16; // [esp+4h] [ebp-5Ch]
  int v17; // [esp+8h] [ebp-58h]
  int integer; // [esp+Ch] [ebp-54h]
  int n; // [esp+30h] [ebp-30h]
  GfxEntity *gfxEnt2; // [esp+34h] [ebp-2Ch]
  GfxSceneModel *sceneModel; // [esp+38h] [ebp-28h]
  XModel *model; // [esp+3Ch] [ebp-24h]
  float radius; // [esp+40h] [ebp-20h]
  float radiusa; // [esp+40h] [ebp-20h]
  GfxEntity *gfxEnt; // [esp+44h] [ebp-1Ch]
  GfxSceneEntity *sceneEnt; // [esp+4Ch] [ebp-14h]
  unsigned int sceneEntIndex; // [esp+50h] [ebp-10h]
  unsigned int sceneEntIndexa; // [esp+50h] [ebp-10h]
  unsigned int gfxEntIndex; // [esp+54h] [ebp-Ch]
  unsigned int gfxEntIndex2; // [esp+58h] [ebp-8h]

  if ( !Sys_IsMainThread()
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 402, 0, "%s", "Sys_IsMainThread()") )
  {
    __debugbreak();
  }
  if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 403, 0, "%s", "obj") )
    __debugbreak();
  if ( !pose && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 404, 0, "%s", "pose") )
    __debugbreak();
  if ( entnum >= gfxCfg.entCount
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          405,
          0,
          "entnum doesn't index gfxCfg.entCount\n\t%i not in [0, %i)",
          entnum,
          gfxCfg.entCount) )
  {
    __debugbreak();
  }
  if ( r_drawEntities->current.enabled && scene.dpvs.sceneDObjIndex[entnum] == 0xFFFF )
  {
    for ( n = 0; n < DObjGetNumModels(obj); ++n )
    {
      if ( (DObjGetModel(obj, n)->flags & 0x100000) != 0 )
      {
        renderFxFlags |= 0x400000u;
        break;
      }
    }
    if ( scene.dpvs.sceneDObjIndex[entnum] != 0xFFFF
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
            440,
            0,
            "%s\n\t(scene.dpvs.sceneDObjIndex[entnum]) = %i",
            "(scene.dpvs.sceneDObjIndex[entnum] == (65535))",
            scene.dpvs.sceneDObjIndex[entnum]) )
    {
      __debugbreak();
    }
    if ( materialTime == 0.0 && !renderFxFlags && burnFraction <= 0.0 && wetness == 0.0 && textureOverrideIndex < 0 )
    {
      LOWORD(gfxEntIndex) = 0;
      LOWORD(gfxEntIndex2) = 0;
    }
    else
    {
      gfxEntIndex = _InterlockedExchangeAdd(&frontEndDataOut->gfxEntCount, 1u);
      if ( gfxEntIndex >= 0x100 )
      {
        frontEndDataOut->gfxEntCount = 256;
        R_WarnOncePerFrame(R_WARN_KNOWN_SPECIAL_MODELS, 256);
        return;
      }
      gfxEnt = &frontEndDataOut->gfxEnts[gfxEntIndex];
      frontEndDataOut->gfxEnts[gfxEntIndex].materialTime = materialTime;
      if ( r_swrk_override_enable->current.enabled )
        integer = r_swrk_override_characterCharredAmount->current.integer;
      else
        integer = LODWORD(burnFraction);
      LODWORD(gfxEnt->destructibleBurnAmount) = integer;
      gfxEnt->destructibleFadeAmount = *(float *)&FLOAT_0_0;
      gfxEnt->renderFxFlags = renderFxFlags;
      gfxEnt->wetness = wetness;
      gfxEnt->textureOverrideIndex = textureOverrideIndex;
      if ( materialTime2 == 0.0 )
      {
        LOWORD(gfxEntIndex2) = 0;
      }
      else
      {
        gfxEntIndex2 = _InterlockedExchangeAdd(&frontEndDataOut->gfxEntCount, 1u);
        gfxEnt2 = &frontEndDataOut->gfxEnts[gfxEntIndex2];
        frontEndDataOut->gfxEnts[gfxEntIndex2].materialTime = materialTime2;
        if ( r_swrk_override_enable->current.enabled )
          v17 = r_swrk_override_characterCharredAmount->current.integer;
        else
          v17 = LODWORD(burnFraction);
        LODWORD(gfxEnt2->destructibleBurnAmount) = v17;
        gfxEnt2->destructibleFadeAmount = *(float *)&FLOAT_0_0;
        gfxEnt2->renderFxFlags = renderFxFlags;
        gfxEnt2->wetness = wetness;
        gfxEnt2->textureOverrideIndex = textureOverrideIndex;
      }
    }
    if ( dobjConstantSet && r_shader_constant_set_enable->current.enabled && R_ShaderConstantSetIsUsed(dobjConstantSet) )
      v16 = R_ShaderConstantSet_CopyToFrontEndDataOut(dobjConstantSet);
    else
      v16 = 0;
    if ( dobjConstantSetExtraCam
      && r_shader_constant_set_enable->current.enabled
      && R_ShaderConstantSetIsUsed(dobjConstantSetExtraCam) )
    {
      v15 = R_ShaderConstantSet_CopyToFrontEndDataOut(dobjConstantSetExtraCam);
    }
    else
    {
      v15 = v16;
    }
    if ( (renderFxFlags & 4) != 0 || DObjGetTree(obj) || DObjGetNumModels(obj) != 1 )
    {
      sceneEntIndexa = R_AllocSceneDObj();
      if ( sceneEntIndexa < 0x400 )
      {
        sceneEnt = &scene.sceneDObj[sceneEntIndexa];
        sceneEnt->obj = obj;
        sceneEnt->entnum = entnum;
        scene.dpvs.sceneDObjIndex[entnum] = sceneEntIndexa;
        sceneEnt->info.pose = pose;
        if ( sceneEnt->cull.state
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
                540,
                1,
                "%s\n\t(sceneEnt->cull.state) = %i",
                "(sceneEnt->cull.state == CULL_STATE_OUT)",
                sceneEnt->cull.state) )
        {
          __debugbreak();
        }
        radiusa = DObjGetRadius(obj);
        CG_GetPoseOrigin(pose, sceneEnt->placement.base.origin);
        CG_GetPoseQuat(pose, sceneEnt->placement.base.quat);
        sceneEnt->placement.scale = scale;
        sceneEnt->cull.mins[0] = sceneEnt->placement.base.origin[0] + COERCE_FLOAT(LODWORD(radiusa) ^ _mask__NegFloat_);
        sceneEnt->cull.mins[1] = sceneEnt->placement.base.origin[1] + COERCE_FLOAT(LODWORD(radiusa) ^ _mask__NegFloat_);
        sceneEnt->cull.mins[2] = sceneEnt->placement.base.origin[2] + COERCE_FLOAT(LODWORD(radiusa) ^ _mask__NegFloat_);
        sceneEnt->cull.maxs[0] = sceneEnt->placement.base.origin[0] + radiusa;
        sceneEnt->cull.maxs[1] = sceneEnt->placement.base.origin[1] + radiusa;
        sceneEnt->cull.maxs[2] = sceneEnt->placement.base.origin[2] + radiusa;
        sceneEnt->lightingOrigin[0] = *lightingOrigin;
        sceneEnt->lightingOrigin[1] = lightingOrigin[1];
        sceneEnt->lightingOrigin[2] = lightingOrigin[2];
        sceneEnt->gfxEntIndex = gfxEntIndex;
        sceneEnt->gfxEntIndex2 = gfxEntIndex2;
        sceneEnt->altXModelIndex = altXModel + 1;
        sceneEnt->entShaderConstantSetIndex = v16;
        sceneEnt->entShaderConstantSetIndexExtraCam = v15;
        sceneEnt->lightingOriginToleranceSq = lightingOriginToleranceSq;
        CG_PredictiveSkinCEntity(sceneEnt);
      }
    }
    else
    {
      sceneEntIndex = R_AllocSceneModel();
      if ( sceneEntIndex < 0x400 )
      {
        sceneModel = &scene.sceneModel[sceneEntIndex];
        model = DObjGetModel(obj, 0);
        sceneModel->model = model;
        sceneModel->obj = obj;
        sceneModel->entnum = entnum;
        scene.dpvs.sceneXModelIndex[entnum] = sceneEntIndex;
        sceneModel->cachedLightingHandle = Ragdoll_HandleBody(pose);
        sceneModel->lightingOriginToleranceSq = lightingOriginToleranceSq;
        radius = XModelGetRadius(model);
        CG_GetPoseOrigin(pose, sceneModel->placement.base.origin);
        CG_GetPoseQuat(pose, sceneModel->placement.base.quat);
        sceneModel->placement.scale = scale;
        sceneModel->radius = radius;
        sceneModel->lightingOrigin[0] = *lightingOrigin;
        sceneModel->lightingOrigin[1] = lightingOrigin[1];
        sceneModel->lightingOrigin[2] = lightingOrigin[2];
        sceneModel->gfxEntIndex = gfxEntIndex;
        sceneModel->modelShaderConstantSetIndex = v16;
        sceneModel->modelShaderConstantSetIndexExtraCam = v15;
      }
    }
  }
}

unsigned int __cdecl R_AllocSceneDObj()
{
  unsigned int sceneEntIndex; // [esp+0h] [ebp-4h]

  if ( !rg.registered
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 188, 0, "%s", "rg.registered") )
  {
    __debugbreak();
  }
  if ( !rg.inFrame
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 190, 0, "%s", "rg.inFrame") )
  {
    __debugbreak();
  }
  sceneEntIndex = _InterlockedExchangeAdd(&scene.sceneDObjCount, 1u);
  if ( sceneEntIndex >= r_dobjLimit->current.integer )
  {
    scene.sceneDObjCount = r_dobjLimit->current.integer;
    R_WarnOncePerFrame(R_WARN_KNOWN_MODELS, r_dobjLimit->current.integer);
  }
  return sceneEntIndex;
}

GfxParticleCloud *__cdecl R_AddParticleCloudToScene(Material *material)
{
  volatile unsigned int cloudIndex; // [esp+Ch] [ebp-4h]

  cloudIndex = _InterlockedExchangeAdd(&frontEndDataOut->cloudCount, 1u);
  if ( cloudIndex < 0x100 )
  {
    if ( R_AddParticleCloudDrawSurf(cloudIndex, material) )
      return &frontEndDataOut->clouds[cloudIndex];
    else
      return 0;
  }
  else
  {
    _InterlockedExchangeAdd(&frontEndDataOut->cloudCount, 0xFFFFFFFF);
    R_WarnOncePerFrame(R_WARN_MAX_CLOUDS);
    return 0;
  }
}

void __cdecl R_AddOmniLightToScene(const float *org, const float (*axis)[3], int radius, float r, float g, float b)
{
  int integer; // xmm0_4
  int v7; // xmm0_4
  float v8; // xmm0_4
  long double v9; // [esp-50h] [ebp-6Ch]
  long double v10; // [esp-48h] [ebp-64h]
  float v11; // [esp-40h] [ebp-5Ch]
  float v12; // [esp-3Ch] [ebp-58h]
  float v13; // [esp-38h] [ebp-54h]
  float v14; // [esp-2Ch] [ebp-48h]
  float v15; // [esp-28h] [ebp-44h]
  float v16; // [esp-24h] [ebp-40h]
  GfxLight *v17; // [esp-Ch] [ebp-28h]
  GfxLight *v18; // [esp-Ch] [ebp-28h]
  int i; // [esp-8h] [ebp-24h]
  float radiusa; // [esp+28h] [ebp+Ch]

  if ( rg.registered && rgp.world )
  {
    if ( !rg.inFrame
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 604, 0, "%s", "rg.inFrame") )
    {
      __debugbreak();
    }
    if ( *(float *)&radius > 0.0 )
    {
      if ( scene.addedLightCount < 32 )
      {
        if ( rg.isNotRenderingFullScreen )
        {
          if ( (float)(r_dlightMaxNonFullScreenRadius->current.value - *(float *)&radius) < 0.0 )
            integer = r_dlightMaxNonFullScreenRadius->current.integer;
          else
            integer = radius;
          radiusa = *(float *)&integer;
        }
        else
        {
          if ( (float)(r_dlightMaxFullScreenRadius->current.value - *(float *)&radius) < 0.0 )
            v7 = r_dlightMaxFullScreenRadius->current.integer;
          else
            v7 = radius;
          radiusa = *(float *)&v7;
        }
        for ( i = 0; i < scene.addedLightCount; ++i )
        {
          v17 = &scene.addedLight[i];
          if ( v17->type == 3
            && COERCE_FLOAT(COERCE_UNSIGNED_INT(radiusa - v17->radius) & _mask__AbsFloat_) < 10.0
            && 10.0 * 10.0 > Vec3DistanceSq(org, v17->origin) )
          {
            v17->origin[0] = (float)(0.5 * *org) + (float)(0.5 * v17->origin[0]);
            v17->origin[1] = (float)(0.5 * org[1]) + (float)(0.5 * v17->origin[1]);
            v17->origin[2] = (float)(0.5 * org[2]) + (float)(0.5 * v17->origin[2]);
            if ( (float)(radiusa - v17->radius) < 0.0 )
              v8 = v17->radius;
            else
              v8 = radiusa;
            v17->radius = v8;
            if ( r <= 0.040449999 )
            {
              v16 = r / 12.92;
            }
            else
            {
              __libm_sse2_pow(v9, v10);
              v16 = (float)(r + 0.055) / 1.0549999;
            }
            v17->diffuseColor[0] = v17->diffuseColor[0] + v16;
            if ( g <= 0.040449999 )
            {
              v15 = g / 12.92;
            }
            else
            {
              __libm_sse2_pow(v9, v10);
              v15 = (float)(g + 0.055) / 1.0549999;
            }
            v17->diffuseColor[1] = v17->diffuseColor[1] + v15;
            if ( b <= 0.040449999 )
            {
              v14 = b / 12.92;
            }
            else
            {
              __libm_sse2_pow(v9, v10);
              v14 = (float)(b + 0.055) / 1.0549999;
            }
            v17->diffuseColor[2] = v17->diffuseColor[2] + v14;
            return;
          }
        }
        v18 = &scene.addedLight[scene.addedLightCount++];
        memset(&v18->type, 0, sizeof(GfxLight));
        v18->def = rgp.dlightDef;
        v18->type = 3;
        v18->origin[0] = *org;
        v18->origin[1] = org[1];
        v18->origin[2] = org[2];
        v18->radius = radiusa;
        v18->color[0] = r;
        v18->color[1] = g;
        v18->color[2] = b;
        v18->canUseShadowMap = 0;
        v18->spotShadowIndex = -1;
        if ( b <= 0.040449999 )
        {
          v13 = b / 12.92;
        }
        else
        {
          __libm_sse2_pow(v9, v10);
          v13 = (float)(b + 0.055) / 1.0549999;
        }
        if ( g <= 0.040449999 )
        {
          v12 = g / 12.92;
        }
        else
        {
          __libm_sse2_pow(v9, v10);
          v12 = (float)(g + 0.055) / 1.0549999;
        }
        if ( r <= 0.040449999 )
        {
          v11 = r / 12.92;
        }
        else
        {
          __libm_sse2_pow(v9, v10);
          v11 = (float)(r + 0.055) / 1.0549999;
        }
        v18->diffuseColor[0] = v11;
        v18->diffuseColor[1] = v12;
        v18->diffuseColor[2] = v13;
        v18->diffuseColor[3] = FLOAT_1_0;
      }
      else
      {
        R_WarnOncePerFrame(R_WARN_MAX_DLIGHTS);
      }
    }
  }
}

void __cdecl R_AddSpotLightToScene(const float *org, const float (*axis)[3], float radius, float r, float g, float b)
{
  double v6; // xmm0_8
  float v7; // xmm0_4
  long double v8; // [esp+10h] [ebp-6Ch]
  long double v9; // [esp+10h] [ebp-6Ch]
  long double v10; // [esp+10h] [ebp-6Ch]
  long double v11; // [esp+10h] [ebp-6Ch]
  long double v12; // [esp+10h] [ebp-6Ch]
  long double v13; // [esp+18h] [ebp-64h]
  float value; // [esp+1Ch] [ebp-60h]
  float v15; // [esp+28h] [ebp-54h]
  float v16; // [esp+2Ch] [ebp-50h]
  float v17; // [esp+30h] [ebp-4Ch]
  float spotLightFovInner; // [esp+54h] [ebp-28h]
  float dir; // [esp+58h] [ebp-24h]
  float dir_4; // [esp+5Ch] [ebp-20h]
  float dir_8; // [esp+60h] [ebp-1Ch]
  float spotLightOffset; // [esp+64h] [ebp-18h]
  GfxLight *light; // [esp+70h] [ebp-Ch]
  int savedregs; // [esp+7Ch] [ebp+0h] BYREF
  float radiusa; // [esp+8Ch] [ebp+10h]

  dir = (*axis)[0];
  dir_4 = (*axis)[1];
  dir_8 = (*axis)[2];
  if ( !r_enableFlashlight->current.integer && rg.registered && rgp.world )
  {
    if ( !rg.inFrame
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 680, 0, "%s", "rg.inFrame") )
    {
      __debugbreak();
    }
    if ( radius > 0.0 )
    {
      if ( scene.addedLightCount < 32 )
      {
        light = &scene.addedLight[scene.addedLightCount++];
        memset(&light->type, 0, sizeof(GfxLight));
        light->exponent = 1;
        light->type = 2;
        radiusa = r_flashLightRange->current.value;
        v6 = (float)((float)(r_flashLightEndRadius->current.value - r_flashLightStartRadius->current.value) / radiusa);
        __libm_sse2_atan(v8);
        v7 = v6;
        spotLightFovInner = v7 * r_spotLightFovInnerFraction->current.value;
        __libm_sse2_tan(v9);
        spotLightOffset = r_flashLightStartRadius->current.value / v7;
        light->canUseShadowMap = 1;
        light->spotShadowIndex = -1;
        light->type = 2;
        light->cullDist = (int)radiusa;
        light->origin[0] = *org;
        light->origin[1] = org[1];
        light->origin[2] = org[2];
        light->dir[0] = -1.0 * dir;
        light->dir[1] = -1.0 * dir_4;
        light->dir[2] = -1.0 * dir_8;
        light->origin[0] = (float)(spotLightOffset * light->dir[0]) + light->origin[0];
        light->origin[1] = (float)(spotLightOffset * light->dir[1]) + light->origin[1];
        light->origin[2] = (float)(spotLightOffset * light->dir[2]) + light->origin[2];
        light->radius = radiusa + spotLightOffset;
        if ( b <= 0.040449999 )
        {
          v17 = b / 12.92;
        }
        else
        {
          __libm_sse2_pow(v10, v13);
          v17 = (float)(b + 0.055) / 1.0549999;
        }
        if ( g <= 0.040449999 )
        {
          v16 = g / 12.92;
        }
        else
        {
          __libm_sse2_pow(v10, v13);
          v16 = (float)(g + 0.055) / 1.0549999;
        }
        if ( r <= 0.040449999 )
        {
          v15 = r / 12.92;
        }
        else
        {
          __libm_sse2_pow(v10, v13);
          v15 = (float)(r + 0.055) / 1.0549999;
        }
        light->diffuseColor[0] = v15;
        light->diffuseColor[1] = v16;
        light->diffuseColor[2] = v17;
        value = r_flashLightBrightness->current.value;
        light->diffuseColor[0] = value * light->diffuseColor[0];
        light->diffuseColor[1] = value * light->diffuseColor[1];
        light->diffuseColor[2] = value * light->diffuseColor[2];
        LODWORD(v11) = light->specularColor;
        HIDWORD(v11) = light->diffuseColor;
        light->specularColor[0] = light->diffuseColor[0];
        light->specularColor[1] = light->diffuseColor[1];
        light->specularColor[2] = light->diffuseColor[2];
        __libm_sse2_cos(v11);
        light->cosHalfFovInner = spotLightFovInner;
        __libm_sse2_cos(v12);
        light->cosHalfFovOuter = v7;
        light->angles[0] = *(float *)&FLOAT_0_0;
        light->angles[1] = *(float *)&FLOAT_0_0;
        light->angles[2] = *(float *)&FLOAT_0_0;
        light->attenuation[0] = FLOAT_1_0;
        light->attenuation[1] = *(float *)&FLOAT_0_0;
        light->attenuation[2] = *(float *)&FLOAT_0_0;
        light->attenuation[3] = FLOAT_1_0;
        light->aAbB[0] = FLOAT_0_75;
        light->aAbB[1] = FLOAT_1_0;
        light->aAbB[2] = FLOAT_0_75;
        light->aAbB[3] = FLOAT_1_0;
        light->falloff[0] = *(float *)&FLOAT_0_0;
        light->falloff[1] = radiusa;
        light->falloff[2] = (float)((float)(radiusa - 0.0) * 0.0) + 0.0;
        light->falloff[3] = (float)((float)(radiusa - 0.0) * 0.0) + 0.0;
        SpotLightViewMatrixDir3(
          (unsigned int)&savedregs,
          (const float *)axis,
          &(*axis)[3],
          &(*axis)[6],
          light->viewMatrix.m);
        SpotLightProjectionMatrix(light->cosHalfFovOuter, light->falloff[0], light->falloff[1], light->projMatrix.m);
        light->def = rgp.flashLightDef;
        scene.dynamicSpotLightNearPlaneOffset = spotLightOffset;
        R_CalcSpotLightPlanes(light, spotLightOffset, scene.dynamicSpotLightPlanes);
      }
      else
      {
        R_WarnOncePerFrame(R_WARN_MAX_DLIGHTS);
      }
    }
  }
}

double __cdecl R_GetDefaultNearClip()
{
  if ( (float)(1.0 - r_znear->current.value) < 0.0 )
    return r_znear->current.value;
  else
    return MINIMUM_Z_NEAR;
}

void __cdecl R_SetupViewProjectionMatrices(GfxViewParms *viewParms, bool offsetmatrix)
{
  if ( rg.renderHiResShot && rg.hiResShotTiles > 0 && offsetmatrix )
  {
    viewParms->projectionMatrix.m[0][0] = (float)rg.hiResShotTiles * viewParms->projectionMatrix.m[0][0];
    viewParms->projectionMatrix.m[1][1] = (float)rg.hiResShotTiles * viewParms->projectionMatrix.m[1][1];
    viewParms->projectionMatrix.m[2][0] = (float)((float)(rg.hiResShotRow / (float)rg.hiResShotTiles)
                                                + (float)((float)(rg.hiResShotRow + 1.0) / (float)rg.hiResShotTiles))
                                        / (float)((float)(rg.hiResShotRow / (float)rg.hiResShotTiles)
                                                - (float)((float)(rg.hiResShotRow + 1.0) / (float)rg.hiResShotTiles));
    viewParms->projectionMatrix.m[2][1] = (float)((float)(rg.hiResShotCol / (float)rg.hiResShotTiles)
                                                + (float)((float)(rg.hiResShotCol + 1.0) / (float)rg.hiResShotTiles))
                                        / (float)((float)(COERCE_FLOAT(LODWORD(rg.hiResShotCol) ^ _mask__NegFloat_)
                                                        / (float)rg.hiResShotTiles)
                                                + (float)((float)(rg.hiResShotCol + 1.0) / (float)rg.hiResShotTiles));
    viewParms->projectionMatrix.m[2][0] = (float)rg.hiResShotTiles + viewParms->projectionMatrix.m[2][0];
    viewParms->projectionMatrix.m[2][1] = viewParms->projectionMatrix.m[2][1] - (float)rg.hiResShotTiles;
  }
  MatrixMultiply44(viewParms->viewMatrix.m, viewParms->projectionMatrix.m, viewParms->viewProjectionMatrix.m);
  MatrixInverse44((const float *)&viewParms->viewProjectionMatrix, (float *)&viewParms->inverseViewProjectionMatrix);
}

void __cdecl R_AddBModelSurfacesCamera(
        BModelDrawInfo *bmodelInfo,
        const GfxBrushModel *bmodel,
        GfxDrawSurf **drawSurfs,
        GfxDrawSurf **lastDrawSurfs,
        unsigned int reflectionProbeIndex,
        float burnAmt,
        float fadeAmt,
        char visLightsMask)
{
  unsigned __int64 v8; // rax
  unsigned __int64 v9; // rax
  unsigned __int64 v10; // rax
  unsigned int surfId; // [esp+8h] [ebp-28h]
  unsigned int surfIndex; // [esp+Ch] [ebp-24h]
  int drawSurf; // [esp+10h] [ebp-20h]
  int drawSurfa; // [esp+10h] [ebp-20h]
  unsigned __int64 drawSurfb; // [esp+10h] [ebp-20h]
  unsigned int drawSurf_4; // [esp+14h] [ebp-1Ch]
  unsigned int drawSurf_4a; // [esp+14h] [ebp-1Ch]
  const Material *material; // [esp+1Ch] [ebp-14h]
  BModelSurface *modelSurf; // [esp+20h] [ebp-10h]
  const GfxSurface *bspSurf; // [esp+24h] [ebp-Ch]
  unsigned int region; // [esp+28h] [ebp-8h]
  unsigned int count; // [esp+2Ch] [ebp-4h]

  if ( !bmodel && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 994, 0, "%s", "bmodel") )
    __debugbreak();
  surfId = bmodelInfo->surfId;
  modelSurf = (BModelSurface *)((char *)frontEndDataOut + 4 * surfId);
  if ( reflectionProbeIndex >= 8
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          999,
          0,
          "reflectionProbeIndex doesn't index 1 << MTL_SORT_ENVMAP_BITS\n\t%i not in [0, %i)",
          reflectionProbeIndex,
          8) )
  {
    __debugbreak();
  }
  if ( gfxDrawMethod.emissiveTechType >= 0x82u
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          1001,
          0,
          "gfxDrawMethod.emissiveTechType doesn't index TECHNIQUE_COUNT\n\t%i not in [0, %i)",
          gfxDrawMethod.emissiveTechType,
          130) )
  {
    __debugbreak();
  }
  count = 0;
  surfIndex = bmodel->startSurfIndex;
  while ( count < bmodel->surfaceCount )
  {
    bspSurf = modelSurf->surf;
    material = bspSurf->material;
    if ( rgp.sortedMaterials[(material->info.drawSurf.packed >> 31) & 0xFFF] != material
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
            1007,
            0,
            "%s",
            "rgp.sortedMaterials[material->info.drawSurf.fields.materialSortedIndex] == material") )
    {
      __debugbreak();
    }
    region = material->cameraRegion;
    if ( region != 3 )
    {
      if ( drawSurfs[region] >= lastDrawSurfs[region] )
      {
        R_WarnOncePerFrame(R_WARN_MAX_SCENE_DRAWSURFS, "R_AddBModelSurfacesCamera");
        return;
      }
      modelSurf->bmodelBurnAmt = burnAmt;
      modelSurf->bmodelFadeAmt = fadeAmt;
      if ( surfId >= 0x10000
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
              1022,
              0,
              "surfId doesn't index 1 << MTL_SORT_OBJECT_ID_BITS\n\t%i not in [0, %i)",
              surfId,
              0x10000) )
      {
        __debugbreak();
      }
      v8 = (unsigned __int64)(bspSurf->lightmapIndex & 0x1F) << 20;
      drawSurf_4 = HIDWORD(v8) | HIDWORD(material->info.drawSurf.packed) & 0xFF87FFFF | 0x300000;
      drawSurf = ((reflectionProbeIndex & 7) << 25)
               | v8 & 0xF1FFFFFF
               | (unsigned __int16)surfId
               | *(unsigned int *)&material->info.drawSurf.fields & 0xF00F0000;
      v9 = (unsigned __int64)(visLightsMask & 1) << 29;
      drawSurfa = v9 | drawSurf & 0xDFFFFFFF;
      drawSurf_4a = HIDWORD(v9) | (bspSurf->primaryLightIndex << 11) | drawSurf_4 & 0xFFF807FF;
      v10 = (unsigned __int64)((visLightsMask & 2) != 0) << 30;
      LODWORD(drawSurfb) = v10 | drawSurfa & 0xBFFFFFFF;
      HIDWORD(drawSurfb) = HIDWORD(v10) | drawSurf_4a;
      drawSurfs[region]->packed = drawSurfb;
      ++drawSurfs[region];
    }
    ++count;
    ++surfIndex;
    ++modelSurf;
    surfId += 5;
  }
}

GfxDrawSurf *__cdecl R_AddBModelSurfaces(
        BModelDrawInfo *bmodelInfo,
        const GfxBrushModel *bmodel,
        unsigned __int8 techType,
        GfxDrawSurf *drawSurf,
        GfxDrawSurf *lastDrawSurf)
{
  unsigned int surfId; // [esp+Ch] [ebp-24h]
  unsigned int surfIndex; // [esp+10h] [ebp-20h]
  const Material *material; // [esp+14h] [ebp-1Ch]
  unsigned int newDrawSurf_4; // [esp+1Ch] [ebp-14h]
  BModelSurface *modelSurf; // [esp+24h] [ebp-Ch]
  unsigned int count; // [esp+2Ch] [ebp-4h]

  if ( !bmodel && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 1051, 0, "%s", "bmodel") )
    __debugbreak();
  surfId = bmodelInfo->surfId;
  modelSurf = (BModelSurface *)((char *)frontEndDataOut + 4 * surfId);
  count = 0;
  surfIndex = bmodel->startSurfIndex;
  while ( count < bmodel->surfaceCount )
  {
    if ( drawSurf >= lastDrawSurf )
    {
      R_WarnOncePerFrame(R_WARN_MAX_SCENE_DRAWSURFS, "R_AddBModelSurfaces");
      return drawSurf;
    }
    material = modelSurf->surf->material;
    if ( rgp.sortedMaterials[(material->info.drawSurf.packed >> 31) & 0xFFF] != material
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
            1066,
            0,
            "%s",
            "rgp.sortedMaterials[material->info.drawSurf.fields.materialSortedIndex] == material") )
    {
      __debugbreak();
    }
    if ( Material_GetTechnique(material, techType) )
    {
      if ( surfId >= 0x10000
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
              1071,
              0,
              "surfId doesn't index 1 << MTL_SORT_OBJECT_ID_BITS\n\t%i not in [0, %i)",
              surfId,
              0x10000) )
      {
        __debugbreak();
      }
      newDrawSurf_4 = HIDWORD(material->info.drawSurf.packed) & 0xFF87FFFF | 0x300000;
      *(unsigned int *)&drawSurf->fields = (unsigned __int16)surfId | *(unsigned int *)&material->info.drawSurf.fields & 0xFFFF0000;
      HIDWORD(drawSurf->packed) = newDrawSurf_4;
      ++drawSurf;
    }
    ++count;
    ++surfIndex;
    ++modelSurf;
    surfId += 5;
  }
  return drawSurf;
}

const MaterialTechnique *__cdecl Material_GetTechnique(const Material *material, unsigned __int8 techType)
{
  const char *v2; // eax
  const MaterialTechniqueSet *techSet; // [esp+4h] [ebp-4h]

  techSet = Material_GetTechniqueSet(material);
  if ( !techSet )
  {
    v2 = va(
           "material '%s' missing techset. %d tech %d",
           material->info.name,
           material->localTechniqueSet != 0,
           techType);
    if ( !Assert_MyHandler(
            "c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_material.h",
            588,
            0,
            "%s\n\t%s",
            "techSet",
            v2) )
      __debugbreak();
  }
  return techSet->techniques[techType];
}

MaterialTechniqueSet *__cdecl Material_GetTechniqueSet(const Material *material)
{
  const char *v1; // eax

  if ( !material
    && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_material.h", 553, 0, "%s", "material") )
  {
    __debugbreak();
  }
  if ( !material->localTechniqueSet )
  {
    v1 = va(
           "material '%s' missing techset. If you are building fastfile, check Launcher for error messages.",
           material->info.name);
    if ( !Assert_MyHandler(
            "c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_material.h",
            554,
            0,
            "%s\n\t%s",
            "material->techniqueSet",
            v1) )
      __debugbreak();
  }
  return material->localTechniqueSet;
}

void __cdecl R_SetEnablePlayerShadowFlag(bool flag)
{
  rg.enablePlayerShadowFlag = flag;
}

void __cdecl R_AddXModelSurfacesCamera(
        XModelDrawInfo *modelInfo,
        const XModel *model,
        float *origin,
        unsigned int gfxEntIndex,
        unsigned int lightingHandle,
        unsigned __int8 primaryLightIndex,
        int isShadowReceiver,
        int depthHack,
        GfxDrawSurf **drawSurfs,
        GfxDrawSurf **lastDrawSurfs,
        unsigned int reflectionProbeIndex,
        unsigned int constantSetIndex,
        char visLightsMask)
{
  unsigned __int64 v13; // rax
  unsigned __int64 v14; // rax
  unsigned __int64 v15; // rax
  unsigned __int64 v16; // rax
  unsigned __int64 v17; // rax
  unsigned __int64 v18; // rax
  const XSurface *XSurface; // eax
  int NumTris; // eax
  const XSurface *v21; // eax
  char *v22; // eax
  char *v23; // eax
  char *v24; // eax
  char customIndex; // [esp+Ch] [ebp-3Ch]
  unsigned int surfId; // [esp+10h] [ebp-38h]
  int totalVertCount; // [esp+14h] [ebp-34h]
  GfxDrawSurf drawSurf; // [esp+18h] [ebp-30h] BYREF
  int totalTriCount; // [esp+24h] [ebp-24h]
  Material *const *material; // [esp+28h] [ebp-20h]
  unsigned int subMatIndex; // [esp+2Ch] [ebp-1Ch]
  int skinnedCachedOffset; // [esp+30h] [ebp-18h]
  int lod; // [esp+34h] [ebp-14h]
  GfxModelRigidSurface *modelSurf; // [esp+38h] [ebp-10h]
  surfaceType_t surfType; // [esp+3Ch] [ebp-Ch]
  unsigned int region; // [esp+40h] [ebp-8h]
  unsigned int numsurfs; // [esp+44h] [ebp-4h]

  if ( !lightingHandle
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 1147, 0, "%s", "lightingHandle") )
  {
    __debugbreak();
  }
  totalTriCount = 0;
  totalVertCount = 0;
  if ( !model && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 1154, 0, "%s", "model") )
    __debugbreak();
  surfId = modelInfo->surfId;
  modelSurf = (GfxModelRigidSurface *)((char *)frontEndDataOut + 4 * surfId);
  lod = modelInfo->lod;
  numsurfs = XModelGetSurfCount(model, lod);
  material = XModelGetSkins(model, lod);
  if ( !material
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 1164, 0, "%s", "material") )
  {
    __debugbreak();
  }
  if ( reflectionProbeIndex >= 8
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          1166,
          0,
          "reflectionProbeIndex doesn't index 1 << MTL_SORT_ENVMAP_BITS\n\t%i not in [0, %i)",
          reflectionProbeIndex,
          8) )
  {
    __debugbreak();
  }
  if ( gfxDrawMethod.emissiveTechType >= 0x82u
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          1168,
          0,
          "gfxDrawMethod.emissiveTechType doesn't index TECHNIQUE_COUNT\n\t%i not in [0, %i)",
          gfxDrawMethod.emissiveTechType,
          130) )
  {
    __debugbreak();
  }
  subMatIndex = 0;
  while ( subMatIndex < numsurfs )
  {
    skinnedCachedOffset = modelSurf->surf.skinnedCachedOffset;
    if ( skinnedCachedOffset == -3 )
    {
      ++surfId;
      modelSurf = (GfxModelRigidSurface *)((char *)modelSurf + 4);
    }
    else
    {
      if ( !*material
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 1181, 0, "%s", "*material") )
      {
        __debugbreak();
      }
      if ( rgp.sortedMaterials[((*material)->info.drawSurf.packed >> 31) & 0xFFF] != *material
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
              1182,
              0,
              "%s",
              "rgp.sortedMaterials[(*material)->info.drawSurf.fields.materialSortedIndex] == *material") )
      {
        __debugbreak();
      }
      region = (*material)->cameraRegion;
      if ( region == 3 )
      {
        surfId += 14;
        ++modelSurf;
      }
      else
      {
        if ( drawSurfs[region] >= lastDrawSurfs[region] )
        {
          R_WarnOncePerFrame(R_WARN_MAX_SCENE_DRAWSURFS, "R_AddXModelSurfacesCamera");
          break;
        }
        if ( skinnedCachedOffset == -2 )
        {
          surfType = SF_BEGIN_XMODEL;
        }
        else
        {
          if ( skinnedCachedOffset != -1
            && !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
                  1224,
                  0,
                  "%s\n\t(skinnedCachedOffset) = %i",
                  "(skinnedCachedOffset == (-1))",
                  skinnedCachedOffset) )
          {
            __debugbreak();
          }
          surfType = SF_XMODEL_RIGID_SKINNED;
        }
        modelSurf->surf.info.gfxEntIndex = gfxEntIndex;
        modelSurf->surf.info.lightingHandle = lightingHandle;
        modelSurf->surf.info.dobjModelIndex = 0;
        if ( surfId >= 0x10000
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
                1237,
                0,
                "surfId doesn't index 1 << MTL_SORT_OBJECT_ID_BITS\n\t%i not in [0, %i)",
                surfId,
                0x10000) )
        {
          __debugbreak();
        }
        customIndex = isShadowReceiver != 0;
        if ( gfxEntIndex && frontEndDataOut->gfxEnts[gfxEntIndex].destructibleBurnAmount > 0.0 )
          customIndex |= 2u;
        HIDWORD(v13) = HIDWORD((*material)->info.drawSurf.packed);
        *(unsigned int *)&drawSurf.fields = (*material)->info.drawSurf.fields;
        HIDWORD(drawSurf.packed) = HIDWORD(v13);
        LODWORD(v13) = drawSurf.fields;
        v14 = (unsigned __int64)((unsigned int)((v13 >> 58) & 0x3F) - depthHack) << 58;
        *(unsigned int *)&drawSurf.fields |= v14;
        HIDWORD(drawSurf.packed) = HIDWORD(v14) | HIDWORD(drawSurf.packed) & 0x3FFFFFF;
        HIDWORD(drawSurf.packed) = ((surfType & 0xF) << 19) | HIDWORD(drawSurf.packed) & 0xFF87FFFF;
        *(unsigned int *)&drawSurf.fields = (unsigned __int16)surfId | *(unsigned int *)&drawSurf.fields & 0xFFFF0000;
        v15 = (unsigned __int64)(reflectionProbeIndex & 7) << 25;
        *(unsigned int *)&drawSurf.fields = v15 | *(unsigned int *)&drawSurf.fields & 0xF1FFFFFF;
        HIDWORD(drawSurf.packed) |= HIDWORD(v15);
        v16 = (unsigned __int64)(customIndex & 0x1F) << 20;
        *(unsigned int *)&drawSurf.fields = v16 | *(unsigned int *)&drawSurf.fields & 0xFE0FFFFF;
        HIDWORD(drawSurf.packed) |= HIDWORD(v16);
        HIDWORD(drawSurf.packed) = (primaryLightIndex << 11) | HIDWORD(drawSurf.packed) & 0xFFF807FF;
        R_XModelDrawSurfEncodeShaderConstantSet(&drawSurf, constantSetIndex);
        v17 = (unsigned __int64)(visLightsMask & 1) << 29;
        *(unsigned int *)&drawSurf.fields = v17 | *(unsigned int *)&drawSurf.fields & 0xDFFFFFFF;
        HIDWORD(drawSurf.packed) |= HIDWORD(v17);
        v18 = (unsigned __int64)((visLightsMask & 2) != 0) << 30;
        *(unsigned int *)&drawSurf.fields = v18 | *(unsigned int *)&drawSurf.fields & 0xBFFFFFFF;
        HIDWORD(drawSurf.packed) |= HIDWORD(v18);
        drawSurfs[region]->fields = drawSurf.fields;
        ++drawSurfs[region];
        if ( r_showTriCounts->current.enabled )
        {
          XSurface = R_GetXSurface(modelSurf, surfType);
          NumTris = XSurfaceGetNumTris(XSurface);
          totalTriCount += NumTris;
        }
        else if ( r_showVertCounts->current.enabled )
        {
          v21 = R_GetXSurface(modelSurf, surfType);
          totalVertCount += XSurfaceGetNumVerts(v21);
        }
        surfId += 14;
        ++modelSurf;
      }
    }
    ++subMatIndex;
    ++material;
  }
  if ( r_showTriCounts->current.enabled )
  {
    v22 = va("%i", totalTriCount);
    R_AddXModelDebugString(origin, v22);
  }
  else if ( r_showVertCounts->current.enabled )
  {
    v23 = va("%i", totalVertCount);
    R_AddXModelDebugString(origin, v23);
  }
  else if ( r_showSurfCounts->current.enabled )
  {
    v24 = va("%i", numsurfs);
    R_AddXModelDebugString(origin, v24);
  }
}

const XSurface *__cdecl R_GetXSurface(unsigned int *modelSurf, surfaceType_t surfType)
{
  bool v3; // [esp+0h] [ebp-4h]

  if ( !modelSurf
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 1093, 0, "%s", "modelSurf") )
  {
    __debugbreak();
  }
  v3 = (unsigned int)surfType >= SF_BEGIN_XMODEL && (unsigned int)surfType < SF_END_XMODEL;
  if ( !v3
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          1094,
          0,
          "%s",
          "R_IsModelSurfaceType( surfType )") )
  {
    __debugbreak();
  }
  return (const XSurface *)modelSurf[1];
}

GfxDrawSurf *__cdecl R_AddXModelSurfaces(
        XModelDrawInfo *modelInfo,
        const XModel *model,
        unsigned __int8 techType,
        GfxDrawSurf *drawSurf,
        GfxDrawSurf *lastDrawSurf)
{
  unsigned int surfId; // [esp+10h] [ebp-2Ch]
  Material **material; // [esp+14h] [ebp-28h]
  unsigned int subMatIndex; // [esp+18h] [ebp-24h]
  unsigned __int64 newDrawSurf; // [esp+1Ch] [ebp-20h]
  int skinnedCachedOffset; // [esp+28h] [ebp-14h]
  int lod; // [esp+2Ch] [ebp-10h]
  GfxModelRigidSurface *modelSurf; // [esp+30h] [ebp-Ch]
  char surfType; // [esp+34h] [ebp-8h]
  unsigned int numsurfs; // [esp+38h] [ebp-4h]

  if ( !model && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 1310, 0, "%s", "model") )
    __debugbreak();
  surfId = modelInfo->surfId;
  modelSurf = (GfxModelRigidSurface *)((char *)frontEndDataOut + 4 * surfId);
  lod = modelInfo->lod;
  numsurfs = XModelGetSurfCount(model, lod);
  material = XModelGetSkins(model, lod);
  if ( !material
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 1320, 0, "%s", "material") )
  {
    __debugbreak();
  }
  for ( subMatIndex = 0; subMatIndex < numsurfs; ++subMatIndex )
  {
    skinnedCachedOffset = modelSurf->surf.skinnedCachedOffset;
    if ( modelSurf->surf.skinnedCachedOffset == -3 )
    {
      ++surfId;
      modelSurf = (GfxModelRigidSurface *)((char *)modelSurf + 4);
    }
    else
    {
      if ( !*material
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 1333, 0, "%s", "*material") )
      {
        __debugbreak();
      }
      if ( rgp.sortedMaterials[((*material)->info.drawSurf.packed >> 31) & 0xFFF] != *material
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
              1334,
              0,
              "%s",
              "rgp.sortedMaterials[(*material)->info.drawSurf.fields.materialSortedIndex] == *material") )
      {
        __debugbreak();
      }
      if ( Material_GetTechnique(*material, techType) )
      {
        if ( drawSurf >= lastDrawSurf )
        {
          R_WarnOncePerFrame(R_WARN_MAX_SCENE_DRAWSURFS, "R_AddXModelSurfaces");
          return drawSurf;
        }
        if ( skinnedCachedOffset == -2 )
        {
          surfType = 7;
        }
        else
        {
          if ( skinnedCachedOffset != -1
            && !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
                  1352,
                  0,
                  "%s\n\t(skinnedCachedOffset) = %i",
                  "(skinnedCachedOffset == (-1))",
                  skinnedCachedOffset) )
          {
            __debugbreak();
          }
          surfType = 8;
        }
        if ( surfId >= 0x10000
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
                1361,
                0,
                "surfId doesn't index 1 << MTL_SORT_OBJECT_ID_BITS\n\t%i not in [0, %i)",
                surfId,
                0x10000) )
        {
          __debugbreak();
        }
        HIDWORD(newDrawSurf) = ((surfType & 0xF) << 19) | HIDWORD((*material)->info.drawSurf.packed) & 0xFF87FFFF;
        LODWORD(newDrawSurf) = (unsigned __int16)surfId | *(unsigned int *)&(*material)->info.drawSurf.fields & 0xFFFF0000;
        drawSurf->packed = newDrawSurf;
        ++drawSurf;
        surfId += 14;
        ++modelSurf;
      }
      else
      {
        surfId += 14;
        ++modelSurf;
      }
    }
    ++material;
  }
  return drawSurf;
}

void __cdecl R_AddDObjSurfacesCamera(
        GfxSceneEntity *sceneEnt,
        unsigned __int16 lightingHandle,
        unsigned __int8 primaryLightIndex,
        GfxDrawSurf **drawSurfs,
        GfxDrawSurf **lastDrawSurfs,
        char visLightsMask,
        bool isMissileCamera)
{
  const cpose_t *v7; // eax
  int packed_high; // ecx
  unsigned __int64 v9; // rax
  unsigned __int64 v10; // rax
  unsigned __int64 v11; // rax
  unsigned __int64 v12; // rax
  const XSurface *XSurface; // eax
  int NumTris; // eax
  const XSurface *v15; // eax
  char *v16; // eax
  char *v17; // eax
  char *v18; // eax
  float val; // [esp+0h] [ebp-B4h]
  float lodDist; // [esp+4h] [ebp-B0h]
  bool lodDist_4; // [esp+8h] [ebp-ACh]
  float v22; // [esp+10h] [ebp-A4h]
  unsigned __int8 entShaderConstantSetIndexExtraCam; // [esp+17h] [ebp-9Dh]
  char customIndex; // [esp+30h] [ebp-84h]
  bool infraredDraw; // [esp+3Bh] [ebp-79h]
  unsigned int surfId; // [esp+3Ch] [ebp-78h]
  unsigned int surfIda; // [esp+3Ch] [ebp-78h]
  DObjAnimMat *boneMatrix; // [esp+40h] [ebp-74h]
  int totalVertCount; // [esp+44h] [ebp-70h]
  const DObj *obj; // [esp+48h] [ebp-6Ch]
  GfxDrawSurf drawSurf; // [esp+4Ch] [ebp-68h] BYREF
  int totalTriCount; // [esp+58h] [ebp-5Ch]
  Material *const *material; // [esp+5Ch] [ebp-58h]
  XModel *model; // [esp+60h] [ebp-54h]
  unsigned int subMatIndex; // [esp+64h] [ebp-50h]
  unsigned int surfSize; // [esp+68h] [ebp-4Ch]
  unsigned int gfxEntIndexToUse; // [esp+6Ch] [ebp-48h]
  int skinnedCachedOffset; // [esp+70h] [ebp-44h]
  const GfxEntity *gfxEnt; // [esp+74h] [ebp-40h]
  int lod; // [esp+78h] [ebp-3Ch]
  unsigned int depthHack; // [esp+7Ch] [ebp-38h]
  void *modelSurf; // [esp+80h] [ebp-34h]
  int isShadowReceiver; // [esp+84h] [ebp-30h]
  surfaceType_t surfType; // [esp+88h] [ebp-2Ch]
  unsigned int region; // [esp+8Ch] [ebp-28h]
  unsigned int gfxEntIndex; // [esp+90h] [ebp-24h]
  unsigned int renderFxFlags; // [esp+94h] [ebp-20h]
  unsigned int numsurfs; // [esp+98h] [ebp-1Ch]
  unsigned int gfxEntIndex2; // [esp+9Ch] [ebp-18h]
  unsigned int modelIndex; // [esp+A0h] [ebp-14h]
  unsigned int modelCount; // [esp+A4h] [ebp-10h]
  GfxModelSkinnedSurface *skinnedSurf; // [esp+A8h] [ebp-Ch]
  bool iAmThePlayer; // [esp+AFh] [ebp-5h]
  int totalSurfCount; // [esp+B0h] [ebp-4h]

  isShadowReceiver = 0;
  modelSurf = sceneEnt->cull.skinnedSurfs.firstSurf;
  if ( modelSurf )
  {
    totalTriCount = 0;
    totalVertCount = 0;
    totalSurfCount = 0;
    obj = sceneEnt->obj;
    if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 1439, 0, "%s", "obj") )
      __debugbreak();
    iAmThePlayer = DObjIsPlayerShadow(obj);
    if ( !iAmThePlayer || !rg.enablePlayerShadowFlag )
    {
      boneMatrix = DObjGetRotTransArray(obj);
      modelCount = DObjGetNumModels(obj);
      if ( sceneEnt->reflectionProbeIndex >= 8u
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
              1455,
              0,
              "sceneEnt->reflectionProbeIndex doesn't index 1 << MTL_SORT_ENVMAP_BITS\n\t%i not in [0, %i)",
              sceneEnt->reflectionProbeIndex,
              8) )
      {
        __debugbreak();
      }
      if ( gfxDrawMethod.emissiveTechType >= 0x82u
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
              1458,
              0,
              "gfxDrawMethod.emissiveTechType doesn't index TECHNIQUE_COUNT\n\t%i not in [0, %i)",
              gfxDrawMethod.emissiveTechType,
              130) )
      {
        __debugbreak();
      }
      gfxEntIndex = sceneEnt->gfxEntIndex;
      if ( gfxEntIndex )
      {
        gfxEnt = &frontEndDataOut->gfxEnts[gfxEntIndex];
        renderFxFlags = gfxEnt->renderFxFlags;
        depthHack = (renderFxFlags & 2) != 0;
      }
      else
      {
        isShadowReceiver = 0;
        depthHack = 0;
      }
      gfxEntIndex2 = sceneEnt->gfxEntIndex2;
      gfxEntIndexToUse = gfxEntIndex;
      for ( modelIndex = 0; ; ++modelIndex )
      {
        if ( modelIndex >= modelCount )
        {
          if ( r_showTriCounts->current.enabled )
          {
            v16 = va("%i", totalTriCount);
            R_AddXModelDebugString(sceneEnt->placement.base.origin, v16);
          }
          else if ( r_showVertCounts->current.enabled )
          {
            v17 = va("%i", totalVertCount);
            R_AddXModelDebugString(sceneEnt->placement.base.origin, v17);
          }
          else if ( r_showSurfCounts->current.enabled )
          {
            v18 = va("%i", totalSurfCount);
            R_AddXModelDebugString(sceneEnt->placement.base.origin, v18);
          }
          return;
        }
        if ( sceneEnt->altXModelIndex )
        {
          if ( sceneEnt->altXModelIndex == modelIndex + 1 )
            gfxEntIndexToUse = gfxEntIndex2;
          else
            gfxEntIndexToUse = gfxEntIndex;
        }
        lod = sceneEnt->cull.lods[modelIndex];
        if ( lod < 0 )
        {
          v7 = (const cpose_t *)DObjGetModel(obj, modelIndex);
          boneMatrix += XModelNumBones(v7);
          continue;
        }
        model = DObjGetModel(obj, modelIndex);
        if ( !model
          && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 1504, 0, "%s", "model") )
        {
          __debugbreak();
        }
        numsurfs = XModelGetSurfCount(model, lod);
        material = XModelGetSkins(model, lod);
        infraredDraw = 0;
        if ( gfxEntIndexToUse )
          infraredDraw = (frontEndDataOut->gfxEnts[gfxEntIndexToUse].renderFxFlags & 0x80) != 0;
        if ( !material
          && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 1518, 0, "%s", "material") )
        {
          __debugbreak();
        }
        subMatIndex = 0;
        while ( subMatIndex < numsurfs )
        {
          if ( infraredDraw )
            material = &rgp.infraredWhite;
          skinnedSurf = (GfxModelSkinnedSurface *)modelSurf;
          skinnedCachedOffset = *(unsigned int *)modelSurf;
          if ( skinnedCachedOffset == -2 )
          {
            surfType = SF_BEGIN_XMODEL;
            surfSize = 56;
          }
          else
          {
            if ( skinnedCachedOffset == -3 )
            {
              surfSize = 4;
              goto LABEL_34;
            }
            surfType = SF_XMODEL_SKINNED;
            surfSize = 24;
          }
          if ( !*material
            && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 1547, 0, "%s", "*material") )
          {
            __debugbreak();
          }
          if ( rgp.sortedMaterials[((*material)->info.drawSurf.packed >> 31) & 0xFFF] != *material
            && !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
                  1548,
                  0,
                  "%s",
                  "rgp.sortedMaterials[(*material)->info.drawSurf.fields.materialSortedIndex] == *material") )
          {
            __debugbreak();
          }
          region = (*material)->cameraRegion;
          if ( region != 3 )
          {
            if ( drawSurfs[region] >= lastDrawSurfs[region] )
            {
              R_WarnOncePerFrame(R_WARN_MAX_SCENE_DRAWSURFS, "R_AddDObjSurfacesCamera");
              break;
            }
            skinnedSurf->info.gfxEntIndex = gfxEntIndexToUse;
            skinnedSurf->info.lightingHandle = lightingHandle;
            skinnedSurf->info.dobjModelIndex = modelIndex;
            surfId = (_BYTE *)modelSurf - (_BYTE *)frontEndDataOut;
            if ( (((_BYTE)modelSurf - (_BYTE)frontEndDataOut) & 3) != 0
              && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
                    1588,
                    0,
                    "%s",
                    "!(surfId & 3)") )
            {
              __debugbreak();
            }
            surfIda = surfId >> 2;
            if ( surfIda >= 0x10000
              && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
                    1590,
                    0,
                    "surfId doesn't index 1 << MTL_SORT_OBJECT_ID_BITS\n\t%i not in [0, %i)",
                    surfIda,
                    0x10000) )
            {
              __debugbreak();
            }
            packed_high = HIDWORD((*material)->info.drawSurf.packed);
            *(unsigned int *)&drawSurf.fields = (*material)->info.drawSurf.fields;
            HIDWORD(drawSurf.packed) = packed_high;
            customIndex = isShadowReceiver != 0;
            if ( gfxEntIndexToUse && frontEndDataOut->gfxEnts[gfxEntIndexToUse].destructibleBurnAmount > 0.0 )
            {
              customIndex |= 2u;
              HIDWORD(drawSurf.packed) = (unsigned int)&cls.rankedServers[711].game[35]
                                       | HIDWORD(drawSurf.packed) & 0xFE7FFFFF;
            }
            HIDWORD(drawSurf.packed) = ((((drawSurf.packed >> 58) & 0x3F) - depthHack) << 26)
                                     | HIDWORD(drawSurf.packed) & 0x3FFFFFF;
            HIDWORD(drawSurf.packed) = ((surfType & 0xF) << 19) | HIDWORD(drawSurf.packed) & 0xFF87FFFF;
            *(unsigned int *)&drawSurf.fields = (unsigned __int16)surfIda | *(unsigned int *)&drawSurf.fields & 0xFFFF0000;
            v9 = (unsigned __int64)(sceneEnt->reflectionProbeIndex & 7) << 25;
            *(unsigned int *)&drawSurf.fields = v9 | *(unsigned int *)&drawSurf.fields & 0xF1FFFFFF;
            HIDWORD(drawSurf.packed) |= HIDWORD(v9);
            v10 = (unsigned __int64)(customIndex & 0x1F) << 20;
            *(unsigned int *)&drawSurf.fields = v10 | *(unsigned int *)&drawSurf.fields & 0xFE0FFFFF;
            HIDWORD(drawSurf.packed) |= HIDWORD(v10);
            HIDWORD(drawSurf.packed) = (primaryLightIndex << 11) | HIDWORD(drawSurf.packed) & 0xFFF807FF;
            if ( isMissileCamera )
              entShaderConstantSetIndexExtraCam = sceneEnt->entShaderConstantSetIndexExtraCam;
            else
              entShaderConstantSetIndexExtraCam = sceneEnt->entShaderConstantSetIndex;
            R_XModelDrawSurfEncodeShaderConstantSet(&drawSurf, entShaderConstantSetIndexExtraCam);
            v11 = (unsigned __int64)(visLightsMask & 1) << 29;
            *(unsigned int *)&drawSurf.fields = v11 | *(unsigned int *)&drawSurf.fields & 0xDFFFFFFF;
            HIDWORD(drawSurf.packed) |= HIDWORD(v11);
            v12 = (unsigned __int64)((visLightsMask & 2) != 0) << 30;
            *(unsigned int *)&drawSurf.fields = v12 | *(unsigned int *)&drawSurf.fields & 0xBFFFFFFF;
            HIDWORD(drawSurf.packed) |= HIDWORD(v12);
            drawSurfs[region]->fields = drawSurf.fields;
            ++drawSurfs[region];
            if ( r_showTriCounts->current.enabled )
            {
              XSurface = R_GetXSurface(modelSurf, surfType);
              NumTris = XSurfaceGetNumTris(XSurface);
              totalTriCount += NumTris;
            }
            else if ( r_showVertCounts->current.enabled )
            {
              v15 = R_GetXSurface(modelSurf, surfType);
              totalVertCount += XSurfaceGetNumVerts(v15);
            }
          }
LABEL_34:
          ++subMatIndex;
          ++material;
          modelSurf = (char *)modelSurf + surfSize;
        }
        lodDist_4 = (model->flags & 0x80000) != 0;
        lodDist = model->lodInfo[lod].dist;
        v22 = Vec3Distance(boneMatrix->trans, rg.lodParms.origin);
        val = I_fres(sceneEnt->placement.scale) * v22;
        ShodLodInfo(boneMatrix->trans, lod, model->numLods, val, lodDist, lodDist_4);
        boneMatrix += XModelNumBones((const cpose_t *)model);
        totalSurfCount += numsurfs;
      }
    }
  }
}

GfxDrawSurf *__cdecl R_AddDObjSurfaces(
        GfxSceneEntity *sceneEnt,
        unsigned __int8 techType,
        GfxDrawSurf *drawSurf,
        GfxDrawSurf *lastDrawSurf)
{
  int v5; // eax
  int packed_high; // ecx
  unsigned __int64 v7; // rax
  bool v8; // [esp+7h] [ebp-6Dh]
  char customIndex; // [esp+18h] [ebp-5Ch]
  unsigned int surfId; // [esp+1Ch] [ebp-58h]
  unsigned int surfIda; // [esp+1Ch] [ebp-58h]
  DObj *obj; // [esp+20h] [ebp-54h]
  Material **material; // [esp+24h] [ebp-50h]
  XModel *model; // [esp+28h] [ebp-4Ch]
  unsigned int subMatIndex; // [esp+2Ch] [ebp-48h]
  unsigned int surfSize; // [esp+30h] [ebp-44h]
  GfxDrawSurf newDrawSurf; // [esp+34h] [ebp-40h] BYREF
  int skinnedCachedOffset; // [esp+40h] [ebp-34h]
  bool isShadowMapBuildTech; // [esp+47h] [ebp-2Dh]
  const GfxEntity *gfxEnt; // [esp+48h] [ebp-2Ch]
  int lod; // [esp+4Ch] [ebp-28h]
  unsigned int depthHack; // [esp+50h] [ebp-24h]
  void *modelSurf; // [esp+54h] [ebp-20h]
  surfaceType_t surfType; // [esp+58h] [ebp-1Ch]
  unsigned int gfxEntIndex; // [esp+5Ch] [ebp-18h]
  unsigned int renderFxFlags; // [esp+60h] [ebp-14h]
  unsigned int numsurfs; // [esp+64h] [ebp-10h]
  unsigned int modelIndex; // [esp+68h] [ebp-Ch]
  unsigned int modelCount; // [esp+6Ch] [ebp-8h]
  GfxModelSkinnedSurface *skinnedSurf; // [esp+70h] [ebp-4h]

  modelSurf = sceneEnt->cull.skinnedSurfs.firstSurf;
  if ( !modelSurf )
    return drawSurf;
  v5 = XModelNumBones(sceneEnt->info.pose);
  obj = Com_GetClientDObj(sceneEnt->entnum, v5);
  if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 1695, 0, "%s", "obj") )
    __debugbreak();
  modelCount = DObjGetNumModels(obj);
  v8 = techType == 3 || techType == 2;
  isShadowMapBuildTech = v8;
  gfxEntIndex = sceneEnt->gfxEntIndex;
  if ( gfxEntIndex )
  {
    gfxEnt = &frontEndDataOut->gfxEnts[gfxEntIndex];
    renderFxFlags = gfxEnt->renderFxFlags;
    depthHack = (renderFxFlags & 2) != 0;
  }
  else
  {
    depthHack = 0;
  }
  for ( modelIndex = 0; modelIndex < modelCount; ++modelIndex )
  {
    lod = sceneEnt->cull.lods[modelIndex];
    if ( lod >= 0 )
    {
      model = DObjGetModel(obj, modelIndex);
      if ( !model
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 1720, 0, "%s", "model") )
      {
        __debugbreak();
      }
      numsurfs = XModelGetSurfCount(model, lod);
      material = XModelGetSkins(model, lod);
      if ( !material
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 1724, 0, "%s", "material") )
      {
        __debugbreak();
      }
      subMatIndex = 0;
LABEL_25:
      if ( subMatIndex >= numsurfs )
        continue;
      skinnedSurf = (GfxModelSkinnedSurface *)modelSurf;
      skinnedCachedOffset = *(unsigned int *)modelSurf;
      if ( skinnedCachedOffset == -2 )
      {
        surfType = SF_BEGIN_XMODEL;
        surfSize = 56;
      }
      else
      {
        if ( skinnedCachedOffset == -3 )
        {
          surfSize = 4;
LABEL_24:
          ++subMatIndex;
          ++material;
          modelSurf = (char *)modelSurf + surfSize;
          goto LABEL_25;
        }
        surfType = SF_XMODEL_SKINNED;
        surfSize = 24;
      }
      if ( !*material
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 1749, 0, "%s", "*material") )
      {
        __debugbreak();
      }
      if ( rgp.sortedMaterials[((*material)->info.drawSurf.packed >> 31) & 0xFFF] != *material
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
              1750,
              0,
              "%s",
              "rgp.sortedMaterials[(*material)->info.drawSurf.fields.materialSortedIndex] == *material") )
      {
        __debugbreak();
      }
      if ( Material_GetTechnique(*material, techType)
        && (!isShadowMapBuildTech || ((*material)->info.gameFlags & 0x40) != 0) )
      {
        if ( drawSurf >= lastDrawSurf )
        {
          R_WarnOncePerFrame(R_WARN_MAX_SCENE_DRAWSURFS, "R_AddDObjSurfaces");
          continue;
        }
        customIndex = 0;
        packed_high = HIDWORD((*material)->info.drawSurf.packed);
        *(unsigned int *)&newDrawSurf.fields = (*material)->info.drawSurf.fields;
        HIDWORD(newDrawSurf.packed) = packed_high;
        if ( sceneEnt->gfxEntIndex && frontEndDataOut->gfxEnts[sceneEnt->gfxEntIndex].destructibleBurnAmount > 0.0 )
        {
          customIndex = 2;
          HIDWORD(newDrawSurf.packed) = (unsigned int)&cls.rankedServers[711].game[35]
                                      | HIDWORD(newDrawSurf.packed) & 0xFE7FFFFF;
        }
        surfId = (_BYTE *)modelSurf - (_BYTE *)frontEndDataOut;
        if ( (((_BYTE)modelSurf - (_BYTE)frontEndDataOut) & 3) != 0
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
                1779,
                0,
                "%s",
                "!(surfId & 3)") )
        {
          __debugbreak();
        }
        surfIda = surfId >> 2;
        if ( surfIda >= 0x10000
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
                1781,
                0,
                "surfId doesn't index 1 << MTL_SORT_OBJECT_ID_BITS\n\t%i not in [0, %i)",
                surfIda,
                0x10000) )
        {
          __debugbreak();
        }
        HIDWORD(newDrawSurf.packed) = ((((newDrawSurf.packed >> 58) & 0x3F) - depthHack) << 26)
                                    | HIDWORD(newDrawSurf.packed) & 0x3FFFFFF;
        HIDWORD(newDrawSurf.packed) = ((surfType & 0xF) << 19) | HIDWORD(newDrawSurf.packed) & 0xFF87FFFF;
        *(unsigned int *)&newDrawSurf.fields = (unsigned __int16)surfIda | *(unsigned int *)&newDrawSurf.fields & 0xFFFF0000;
        v7 = (unsigned __int64)(customIndex & 0x1F) << 20;
        *(unsigned int *)&newDrawSurf.fields = v7 | *(unsigned int *)&newDrawSurf.fields & 0xFE0FFFFF;
        HIDWORD(newDrawSurf.packed) |= HIDWORD(v7);
        R_XModelDrawSurfEncodeShaderConstantSet(&newDrawSurf, sceneEnt->entShaderConstantSetIndex);
        drawSurf->fields = newDrawSurf.fields;
        ++drawSurf;
      }
      goto LABEL_24;
    }
  }
  return drawSurf;
}

void __cdecl R_SetCharacterPrimaryLightScaleInfo(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms)
{
  float specularScale; // ecx

  if ( !viewInfo
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 3606, 0, "%s", "viewInfo") )
  {
    __debugbreak();
  }
  if ( !sceneParms
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 3607, 0, "%s", "sceneParms") )
  {
    __debugbreak();
  }
  if ( r_heroLighting->current.enabled )
  {
    if ( r_primaryLightUseTweaks->current.enabled )
    {
      viewInfo->charPrimaryLightScale.diffuseScale = r_primaryLightTweakDiffuseStrength->current.value;
      viewInfo->charPrimaryLightScale.specularScale = r_primaryLightTweakSpecularStrength->current.value;
    }
    else
    {
      specularScale = sceneParms->charPrimaryLightScale.specularScale;
      viewInfo->charPrimaryLightScale.diffuseScale = sceneParms->charPrimaryLightScale.diffuseScale;
      viewInfo->charPrimaryLightScale.specularScale = specularScale;
    }
  }
  else
  {
    viewInfo->charPrimaryLightScale.diffuseScale = FLOAT_1_0;
    viewInfo->charPrimaryLightScale.specularScale = FLOAT_1_0;
  }
}

bool __cdecl R_EndFencePending()
{
  return R_HW_IsFencePending(&frontEndDataOut->endFence);
}

bool __cdecl R_DynamicFencePending()
{
  volatile unsigned int currentFrame; // [esp+4h] [ebp-4h]

  currentFrame = *frontEndDataOut->dynamicBufferCurrentFrame;
  return currentFrame && currentFrame < frontEndDataOut->frameCount;
}

void __cdecl R_SetEndTime(int endTime)
{
  rg.endTime = endTime;
}

void __cdecl R_WaitEndTime()
{
  PIXBeginNamedEvent(-1, "max fps spin");
  while ( (int)(Sys_Milliseconds() - rg.endTime) < 0 )
    NET_Sleep(1u);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

void __cdecl R_SetSunConstants(GfxCmdBufInput *input)
{
  GfxLight *sun; // [esp+30h] [ebp-10h]

  sun = &input->data->sunLight;
  if ( sun->type != 1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          4082,
          0,
          "%s",
          "sun->type == GFX_LIGHT_TYPE_DIR") )
  {
    __debugbreak();
  }
  R_SetInputCodeConstantFromVec4(input, 0x32u, sun->dir);
  R_SetInputCodeConstant(input, 0x33u, sun->diffuseColor[0], sun->diffuseColor[1], sun->diffuseColor[2], 1.0);
  R_SetInputCodeConstant(input, 0x34u, sun->specularColor[0], sun->specularColor[1], sun->specularColor[2], 1.0);
}

void __cdecl R_SetInputCodeConstant(GfxCmdBufInput *input, unsigned int constant, float x, float y, float z, float w)
{
  float *v6; // ecx

  if ( constant >= 0xC5
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state.h",
          1733,
          0,
          "constant doesn't index CONST_SRC_CODE_COUNT_FLOAT4\n\t%i not in [0, %i)",
          constant,
          197) )
  {
    __debugbreak();
  }
  if ( s_codeConstUpdateFreq[constant] != 2
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state.h",
          1735,
          0,
          "%s\n\t(constant) = %i",
          "(s_codeConstUpdateFreq[constant] == MTL_UPDATE_RARELY)",
          constant) )
  {
    __debugbreak();
  }
  if ( constant < 0x2F
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state.h",
          1737,
          0,
          "%s\n\t(constant) = %i",
          "(!R_IsChangeablePixelShaderConst( constant ))",
          constant) )
  {
    __debugbreak();
  }
  input->consts[constant][0] = x;
  v6 = input->consts[constant];
  v6[1] = y;
  v6[2] = z;
  v6[3] = w;
}

void __cdecl R_SetInputCodeConstantFromVec4(GfxCmdBufInput *input, unsigned int constant, float *value)
{
  float *v3; // [esp+0h] [ebp-4h]

  if ( constant >= 0xC5
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state.h",
          1744,
          0,
          "constant doesn't index CONST_SRC_CODE_COUNT_FLOAT4\n\t%i not in [0, %i)",
          constant,
          197) )
  {
    __debugbreak();
  }
  if ( s_codeConstUpdateFreq[constant] != 2
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state.h",
          1746,
          0,
          "%s\n\t(constant) = %i",
          "(s_codeConstUpdateFreq[constant] == MTL_UPDATE_RARELY)",
          constant) )
  {
    __debugbreak();
  }
  if ( constant < 0x2F
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state.h",
          1748,
          0,
          "%s\n\t(constant) = %i",
          "(!R_IsChangeablePixelShaderConst( constant ))",
          constant) )
  {
    __debugbreak();
  }
  v3 = input->consts[constant];
  *v3 = *value;
  v3[1] = value[1];
  v3[2] = value[2];
  v3[3] = value[3];
}

void  R_SetHDRControlConstants(float a1@<ebp>, GfxCmdBufInput *input, const GfxViewInfo *viewInfo)
{
  float v3; // xmm0_4
  float v4; // xmm0_4
  double v5; // xmm0_8
  double w; // xmm0_8
  double v7; // xmm0_8
  double v8; // xmm0_8
  double v9; // xmm0_8
  double v10; // xmm0_8
  double v11; // xmm0_8
  double v12; // xmm0_8
  long double v13; // [esp+10h] [ebp-18Ch]
  long double v14; // [esp+10h] [ebp-18Ch]
  long double v15; // [esp+10h] [ebp-18Ch]
  long double v16; // [esp+10h] [ebp-18Ch]
  long double v17; // [esp+10h] [ebp-18Ch]
  long double v18; // [esp+10h] [ebp-18Ch]
  long double v19; // [esp+10h] [ebp-18Ch]
  long double v20; // [esp+10h] [ebp-18Ch]
  float floatTime; // [esp+20h] [ebp-17Ch]
  float v22; // [esp+28h] [ebp-174h]
  float v23; // [esp+34h] [ebp-168h]
  float v24; // [esp+40h] [ebp-15Ch]
  float v25; // [esp+4Ch] [ebp-150h]
  float v26; // [esp+58h] [ebp-144h]
  float v27; // [esp+58h] [ebp-144h]
  float v28; // [esp+58h] [ebp-144h]
  float v29; // [esp+58h] [ebp-144h]
  float v30; // [esp+60h] [ebp-13Ch]
  float vx[2]; // [esp+64h] [ebp-138h]
  float bpwpscale; // [esp+6Ch] [ebp-130h]
  float rangeC[4]; // [esp+70h] [ebp-12Ch]
  float rangeB[4]; // [esp+80h] [ebp-11Ch]
  float rangeA[4]; // [esp+90h] [ebp-10Ch]
  float Cadd[4]; // [esp+A0h] [ebp-FCh]
  float Cmul[5]; // [esp+B0h] [ebp-ECh]
  float Badd[4]; // [esp+C4h] [ebp-D8h]
  float Bmul[4]; // [esp+D4h] [ebp-C8h]
  float Amul[5]; // [esp+E4h] [ebp-B8h]
  float v41; // [esp+F8h] [ebp-A4h]
  int j; // [esp+FCh] [ebp-A0h]
  float v43; // [esp+100h] [ebp-9Ch]
  float x; // [esp+104h] [ebp-98h]
  int i; // [esp+108h] [ebp-94h]
  float bloomS; // [esp+10Ch] [ebp-90h]
  float curveE[4]; // [esp+110h] [ebp-8Ch]
  float z; // [esp+120h] [ebp-7Ch]
  float curveS[5]; // [esp+124h] [ebp-78h]
  float remapE[5]; // [esp+138h] [ebp-64h]
  float remapS[5]; // [esp+14Ch] [ebp-50h]
  float linearE[5]; // [esp+160h] [ebp-3Ch]
  float linearS[4]; // [esp+174h] [ebp-28h]
  float v54; // [esp+184h] [ebp-18h]
  float blackPoint; // [esp+188h] [ebp-14h]
  float whitePoint; // [esp+18Ch] [ebp-10h]
  float exposure; // [esp+190h] [ebp-Ch]
  float debugLayers; // [esp+194h] [ebp-8h]
  float retaddr; // [esp+19Ch] [ebp+0h]

  exposure = a1;
  debugLayers = retaddr;
  if ( r_debugLayers->current.enabled )
    whitePoint = FLOAT_1_0;
  else
    whitePoint = *(float *)&FLOAT_0_0;
  blackPoint = whitePoint;
  R_SetInputCodeConstant(input, 0x89u, viewInfo->exposureRemap.remapMul[0], 0.0, 0.0, 0.0);
  R_SetInputCodeConstant(input, 0x8Au, 1.0, 0.0, 0.0, whitePoint);
  if ( r_reflectionProbeGenerate->current.enabled )
  {
    R_SetInputCodeConstant(input, 0x89u, 0.0625, 1.0, 1.0, 0.0);
    R_SetInputCodeConstant(input, 0x8Au, 0.0, 0.0, 0.0, 0.0);
  }
  v54 = viewInfo->exposureValue.blackPoint[3];
  linearS[3] = viewInfo->exposureValue.whitePoint[0];
  linearS[2] = viewInfo->exposureValue.blackPoint[0];
  LODWORD(linearS[1]) = viewInfo->exposureValue.linearStart;
  linearE[2] = viewInfo->exposureValue.linearStart[0];
  linearE[3] = viewInfo->exposureValue.linearStart[1];
  linearE[4] = viewInfo->exposureValue.linearStart[2];
  linearS[0] = viewInfo->exposureValue.linearStart[3];
  LODWORD(linearE[1]) = viewInfo->exposureValue.linearEnd;
  remapS[2] = viewInfo->exposureValue.linearEnd[0];
  remapS[3] = viewInfo->exposureValue.linearEnd[1];
  remapS[4] = viewInfo->exposureValue.linearEnd[2];
  linearE[0] = viewInfo->exposureValue.linearEnd[3];
  LODWORD(remapS[1]) = viewInfo->exposureValue.remapStart;
  remapE[2] = viewInfo->exposureValue.remapStart[0];
  remapE[3] = viewInfo->exposureValue.remapStart[1];
  remapE[4] = viewInfo->exposureValue.remapStart[2];
  remapS[0] = viewInfo->exposureValue.remapStart[3];
  LODWORD(remapE[1]) = viewInfo->exposureValue.remapEnd;
  curveS[2] = viewInfo->exposureValue.remapEnd[0];
  curveS[3] = viewInfo->exposureValue.remapEnd[1];
  curveS[4] = viewInfo->exposureValue.remapEnd[2];
  remapE[0] = viewInfo->exposureValue.remapEnd[3];
  LODWORD(curveS[1]) = viewInfo->exposureValue.scurveStart;
  curveE[2] = viewInfo->exposureValue.scurveStart[0];
  curveE[3] = viewInfo->exposureValue.scurveStart[1];
  z = viewInfo->exposureValue.scurveStart[2];
  curveS[0] = viewInfo->exposureValue.scurveStart[3];
  LODWORD(curveE[1]) = viewInfo->exposureValue.scurveEnd;
  x = viewInfo->exposureValue.scurveEnd[0];
  i = LODWORD(viewInfo->exposureValue.scurveEnd[1]);
  bloomS = viewInfo->exposureValue.scurveEnd[2];
  curveE[0] = viewInfo->exposureValue.scurveEnd[3];
  v43 = FLOAT_1_0;
  for ( j = 0; j < 3; ++j )
  {
    if ( linearE[j + 2] <= 0.99998474 )
      v41 = linearE[j + 2];
    else
      v41 = FLOAT_0_99998474;
    linearE[j + 2] = v41;
    if ( linearE[j + 2] < remapS[j + 2] )
      v3 = remapS[j + 2];
    else
      v3 = linearE[j + 2] + 0.000015287891;
    Amul[4] = v3;
    remapS[j + 2] = v3;
    if ( remapE[j + 2] <= 0.99998474 )
      Amul[3] = remapE[j + 2];
    else
      Amul[3] = FLOAT_0_99998474;
    remapE[j + 2] = Amul[3];
    if ( remapE[j + 2] < curveS[j + 2] )
      v4 = curveS[j + 2];
    else
      v4 = remapE[j + 2] + 0.000015287891;
    Amul[2] = v4;
    curveS[j + 2] = v4;
    if ( linearE[j + 2] == 0.0 )
      Amul[1] = *(float *)&FLOAT_0_0;
    else
      Amul[1] = 1.0 / linearE[j + 2];
    Bmul[j + 1] = Amul[1];
    Badd[j + 1] = 1.0 / (float)(remapS[j + 2] - linearE[j + 2]);
    Cmul[j + 2] = COERCE_FLOAT(LODWORD(linearE[j + 2]) ^ _mask__NegFloat_) * Badd[j + 1];
    if ( remapS[j + 2] == 1.0 )
      Cmul[1] = *(float *)&FLOAT_0_0;
    else
      Cmul[1] = 1.0 / (float)(1.0 - remapS[j + 2]);
    Cadd[j + 1] = Cmul[1];
    rangeA[j + 1] = COERCE_FLOAT(LODWORD(remapS[j + 2]) ^ _mask__NegFloat_) * Cadd[j + 1];
    curveE[j + 2] = curveE[j + 2] * 2.0;
    *(&x + j) = *(&x + j) * 2.0;
    rangeB[j + 1] = remapE[j + 2];
    rangeC[j + 1] = curveS[j + 2] - remapE[j + 2];
    vx[j] = 1.0 - curveS[j + 2];
  }
  v30 = 1.0 / (float)(linearS[3] - linearS[2]);
  R_SetInputCodeConstant(input, 0x79u, v30 * v54, 0.0, 1.0 - v43, 1.0);
  R_SetInputCodeConstant(input, 0x7Au, COERCE_FLOAT(LODWORD(linearS[2]) ^ _mask__NegFloat_) * v30, 0.0, 0.0, 0.0);
  R_SetInputCodeConstant(input, 0x7Bu, Bmul[1], Bmul[2], Bmul[3], 1.0);
  R_SetInputCodeConstant(input, 0x7Cu, Badd[1], Badd[2], Badd[3], 0.0);
  R_SetInputCodeConstant(input, 0x7Du, Cmul[2], Cmul[3], Cmul[4], 0.0);
  R_SetInputCodeConstant(input, 0x7Eu, Cadd[1], Cadd[2], Cadd[3], 0.0);
  R_SetInputCodeConstant(input, 0x7Fu, rangeA[1], rangeA[2], rangeA[3], 0.0);
  R_SetInputCodeConstant(input, 0x80u, curveE[2], curveE[3], z, 0.0);
  R_SetInputCodeConstant(input, 0x81u, x, *(float *)&i, bloomS, 0.0);
  R_SetInputCodeConstant(input, 0x82u, rangeB[1], rangeB[2], rangeB[3], 0.0);
  R_SetInputCodeConstant(input, 0x83u, rangeC[1], rangeC[2], rangeC[3], 0.0);
  R_SetInputCodeConstant(input, 0x84u, vx[0], vx[1], bpwpscale, 0.0);
  v5 = (float)(r_waterWaveAngle->current.value * 0.017453292);
  __libm_sse2_cos(v13);
  *(float *)&v5 = v5;
  v26 = *(float *)&v5;
  w = (float)(r_waterWaveAngle->current.value * 0.017453292);
  __libm_sse2_sin(v14);
  *(float *)&w = w;
  v25 = 6.2831855 / r_waterWaveWavelength->current.value;
  R_SetInputCodeConstant(input, 0x79u, v26 * v25, *(float *)&w * v25, v26, *(float *)&w);
  v7 = (float)(r_waterWaveAngle->current.vector[1] * 0.017453292);
  __libm_sse2_cos(v15);
  *(float *)&v7 = v7;
  v27 = *(float *)&v7;
  v8 = (float)(r_waterWaveAngle->current.vector[1] * 0.017453292);
  __libm_sse2_sin(v16);
  *(float *)&v8 = v8;
  v24 = 6.2831855 / r_waterWaveWavelength->current.vector[1];
  R_SetInputCodeConstant(input, 0x7Au, v27 * v24, *(float *)&v8 * v24, v27, *(float *)&v8);
  v9 = (float)(r_waterWaveAngle->current.vector[2] * 0.017453292);
  __libm_sse2_cos(v17);
  *(float *)&v9 = v9;
  v28 = *(float *)&v9;
  v10 = (float)(r_waterWaveAngle->current.vector[2] * 0.017453292);
  __libm_sse2_sin(v18);
  *(float *)&v10 = v10;
  v23 = 6.2831855 / r_waterWaveWavelength->current.vector[2];
  R_SetInputCodeConstant(input, 0x7Bu, v28 * v23, *(float *)&v10 * v23, v28, *(float *)&v10);
  v11 = (float)(r_waterWaveAngle->current.vector[3] * 0.017453292);
  __libm_sse2_cos(v19);
  *(float *)&v11 = v11;
  v29 = *(float *)&v11;
  v12 = (float)(r_waterWaveAngle->current.vector[3] * 0.017453292);
  __libm_sse2_sin(v20);
  *(float *)&v12 = v12;
  v22 = 6.2831855 / r_waterWaveWavelength->current.vector[3];
  R_SetInputCodeConstant(input, 0x7Cu, v29 * v22, *(float *)&v12 * v22, v29, *(float *)&v12);
  floatTime = viewInfo->sceneDef.floatTime;
  R_SetInputCodeConstant(
    input,
    0x7Du,
    (float)((float)(fsqrt(386.22 * v25) * floatTime) * r_waterWaveSpeed->current.value)
  + r_waterWavePhase->current.value,
    (float)((float)(fsqrt(386.22 * v24) * floatTime) * r_waterWaveSpeed->current.vector[1])
  + r_waterWavePhase->current.vector[1],
    (float)((float)(fsqrt(386.22 * v23) * floatTime) * r_waterWaveSpeed->current.vector[2])
  + r_waterWavePhase->current.vector[2],
    (float)((float)(fsqrt(386.22 * v22) * floatTime) * r_waterWaveSpeed->current.vector[3])
  + r_waterWavePhase->current.vector[3]);
  R_SetInputCodeConstant(
    input,
    0x7Eu,
    r_waterWaveAmplitude->current.value,
    r_waterWaveAmplitude->current.vector[1],
    r_waterWaveAmplitude->current.vector[2],
    r_waterWaveAmplitude->current.vector[3]);
  R_SetInputCodeConstant(
    input,
    0x7Fu,
    r_waterWaveSteepness->current.value,
    r_waterWaveSteepness->current.vector[1],
    r_waterWaveSteepness->current.vector[2],
    r_waterWaveSteepness->current.vector[3]);
}

void __cdecl R_GenerateMarkVertsForDynamicModels(const GfxLight *visibleLights, int visibleLightCount)
{
  GfxSceneModel *sceneModel; // [esp+4h] [ebp-24h]
  int dobjIndex; // [esp+8h] [ebp-20h]
  unsigned __int8 reflectionProbeIndex; // [esp+Fh] [ebp-19h]
  unsigned int indexCount; // [esp+10h] [ebp-18h] BYREF
  int brushModelIndex; // [esp+14h] [ebp-14h]
  unsigned __int16 entnum; // [esp+18h] [ebp-10h]
  unsigned __int16 lightHandle; // [esp+1Ch] [ebp-Ch]
  int modelIndex; // [esp+20h] [ebp-8h]
  GfxSceneEntity *sceneEntity; // [esp+24h] [ebp-4h]

  FX_BeginGeneratingMarkVertsForEntModels(scene.dpvs.localClientNum, &indexCount);
  for ( dobjIndex = 0; dobjIndex != scene.sceneDObjCount; ++dobjIndex )
  {
    sceneEntity = &scene.sceneDObj[dobjIndex];
    entnum = sceneEntity->entnum;
    if ( entnum < gfxCfg.entnumOrdinaryEnd && (scene.sceneDObjVisData[0][dobjIndex] & 1) != 0 )
    {
      lightHandle = Ragdoll_HandleBody(sceneEntity->info.pose)->lightingHandle;
      FX_GenerateMarkVertsForEntDObj(
        scene.dpvs.localClientNum,
        entnum,
        &indexCount,
        lightHandle,
        sceneEntity->reflectionProbeIndex,
        (DObj *)sceneEntity->obj,
        sceneEntity->info.pose,
        visibleLights,
        visibleLightCount);
    }
  }
  for ( modelIndex = 0; modelIndex != scene.sceneModelCount; ++modelIndex )
  {
    sceneModel = &scene.sceneModel[modelIndex];
    entnum = sceneModel->entnum;
    if ( entnum < gfxCfg.entnumOrdinaryEnd && (scene.sceneModelVisData[0][modelIndex] & 1) != 0 )
    {
      reflectionProbeIndex = sceneModel->reflectionProbeIndex;
      if ( !sceneModel->cachedLightingHandle
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
              3957,
              0,
              "%s",
              "sceneModel->cachedLightingHandle") )
      {
        __debugbreak();
      }
      if ( sceneModel->obj )
        FX_GenerateMarkVertsForEntXModel(
          scene.dpvs.localClientNum,
          entnum,
          &indexCount,
          *sceneModel->cachedLightingHandle,
          reflectionProbeIndex,
          &sceneModel->placement,
          visibleLights,
          visibleLightCount);
    }
  }
  for ( brushModelIndex = 0; brushModelIndex != scene.sceneBrushCount; ++brushModelIndex )
  {
    entnum = scene.sceneBrush[brushModelIndex].entnum;
    if ( entnum < gfxCfg.entnumOrdinaryEnd && (scene.sceneBrushVisData[0][brushModelIndex] & 1) != 0 )
      FX_GenerateMarkVertsForEntBrush(
        scene.dpvs.localClientNum,
        entnum,
        &indexCount,
        scene.sceneBrush[brushModelIndex].reflectionProbeIndex,
        &scene.sceneBrush[brushModelIndex].placement,
        visibleLights,
        visibleLightCount);
  }
  FX_EndGeneratingMarkVertsForEntModels(scene.dpvs.localClientNum);
}

void  R_SetSkyDynamicIntensity(float a1@<ebp>, const float *viewForward, GfxCmdBufInput *input)
{
  float value; // xmm0_4
  long double v4; // [esp+1Ch] [ebp-8Ch] BYREF
  char textBuff[64]; // [esp+28h] [ebp-80h]
  Font_s *v6; // [esp+68h] [ebp-40h]
  float textY; // [esp+6Ch] [ebp-3Ch]
  float textX; // [esp+70h] [ebp-38h]
  Font_s *font; // [esp+74h] [ebp-34h]
  float skyIntensity; // [esp+78h] [ebp-30h]
  float interp; // [esp+7Ch] [ebp-2Ch]
  float v12; // [esp+80h] [ebp-28h]
  float diff; // [esp+84h] [ebp-24h]
  float dotAngle1; // [esp+88h] [ebp-20h]
  float dotAngle0; // [esp+8Ch] [ebp-1Ch]
  float factor1; // [esp+90h] [ebp-18h]
  float factor0; // [esp+94h] [ebp-14h]
  float angle0; // [esp+9Ch] [ebp-Ch]
  float dot; // [esp+A0h] [ebp-8h]
  float retaddr; // [esp+A8h] [ebp+0h]

  angle0 = a1;
  dot = retaddr;
  if ( r_reflectionProbeGenerate->current.enabled )
  {
    R_SetInputCodeConstant(input, 0xC2u, 1.0, 1.0, 1.0, 1.0);
  }
  else
  {
    factor0 = viewForward[2];
    if ( r_sky_intensity_useDebugValues->current.enabled )
    {
      factor1 = r_sky_intensity_angle0->current.value;
      dotAngle0 = r_sky_intensity_angle1->current.value;
      dotAngle1 = r_sky_intensity_factor0->current.value;
      value = r_sky_intensity_factor1->current.value;
    }
    else
    {
      factor1 = rgp.world->skyDynIntensity.angle0;
      dotAngle0 = rgp.world->skyDynIntensity.angle1;
      dotAngle1 = rgp.world->skyDynIntensity.factor0;
      value = rgp.world->skyDynIntensity.factor1;
    }
    diff = value;
    __libm_sse2_cos(v4);
    v12 = (float)(90.0 - factor1) * 0.017453292;
    __libm_sse2_cos(v4);
    interp = (float)(90.0 - dotAngle0) * 0.017453292;
    skyIntensity = interp - v12;
    if ( COERCE_FLOAT(COERCE_UNSIGNED_INT(interp - v12) & _mask__AbsFloat_) <= 0.000099999997 )
    {
      textX = *(float *)&FLOAT_0_0;
    }
    else
    {
      *(float *)&font = (float)(factor0 - v12) / skyIntensity;
      if ( 1.0 <= 0.0
        && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.h", 730, 0, "%s", "min < max") )
      {
        __debugbreak();
      }
      if ( *(float *)&font >= 0.0 )
      {
        if ( *(float *)&font <= 1.0 )
          textX = *(float *)&font;
        else
          textX = FLOAT_1_0;
      }
      else
      {
        textX = *(float *)&FLOAT_0_0;
      }
      textX = textX * textX;
    }
    textY = (float)((float)(1.0 - textX) * dotAngle1) + (float)(diff * textX);
    if ( r_sky_intensity_showDebugDisplay->current.enabled )
    {
      v6 = R_RegisterFont("fonts/consoleFont", 1);
      *(float *)&textBuff[60] = FLOAT_100_0;
      *(float *)&textBuff[56] = FLOAT_100_0;
      _snprintf((char *)&v4, 0x40u, "intensity0 angle=%.2f factor=%.2f", factor1, dotAngle1);
      R_AddCmdDrawText(
        (char *)&v4,
        64,
        v6,
        *(float *)&textBuff[60],
        *(float *)&textBuff[56],
        1.5,
        2.0,
        0.0,
        colorRed,
        0);
      *(float *)&textBuff[56] = *(float *)&textBuff[56] + 20.0;
      _snprintf((char *)&v4, 0x40u, "intensity1 angle=%.2f factor=%.2f", dotAngle0, diff);
      R_AddCmdDrawText(
        (char *)&v4,
        64,
        v6,
        *(float *)&textBuff[60],
        *(float *)&textBuff[56],
        1.5,
        2.0,
        0.0,
        colorRed,
        0);
      *(float *)&textBuff[56] = *(float *)&textBuff[56] + 20.0;
      _snprintf((char *)&v4, 0x40u, "intensity=%.2f interp=%.2f", textY, textX);
      R_AddCmdDrawText(
        (char *)&v4,
        64,
        v6,
        *(float *)&textBuff[60],
        *(float *)&textBuff[56],
        1.5,
        2.0,
        0.0,
        colorRed,
        0);
    }
    R_SetInputCodeConstant(input, 0xC2u, textY, textY, textY, textY);
  }
}

void __cdecl R_InitScene()
{
  scene.drawSurfs[0] = (GfxDrawSurf *)&scene;
  scene.drawSurfs[1] = scene.smodelDrawSurfsLight;
  scene.drawSurfs[2] = scene.entDrawSurfsLight;
  scene.drawSurfs[9] = scene.codemeshDrawSurfsLight;
  scene.drawSurfs[3] = scene.bspDrawSurfsDecal;
  scene.drawSurfs[4] = scene.smodelDrawSurfsDecal;
  scene.drawSurfs[5] = scene.entDrawSurfsDecal;
  scene.drawSurfs[6] = scene.fxDrawSurfsLight;
  scene.drawSurfs[7] = scene.fxDrawSurfsLightAuto;
  scene.drawSurfs[8] = scene.fxDrawSurfsLightDecal;
  scene.drawSurfs[10] = scene.bspDrawSurfsEmissive;
  scene.drawSurfs[11] = scene.smodelDrawSurfsEmissive;
  scene.drawSurfs[12] = scene.entDrawSurfsEmissive;
  scene.drawSurfs[13] = scene.fxDrawSurfsEmissive;
  scene.drawSurfs[14] = scene.fxDrawSurfsEmissiveAuto;
  scene.drawSurfs[15] = scene.fxDrawSurfsEmissiveDecal;
  scene.drawSurfs[16] = scene.reflectedFxDrawSurfsEmissive;
  scene.drawSurfs[17] = scene.reflectedFxDrawSurfsEmissiveAuto;
  scene.drawSurfs[18] = scene.reflectedFxDrawSurfsEmissiveDecal;
  scene.drawSurfs[19] = scene.bspSunShadowDrawSurfs0;
  scene.drawSurfs[20] = scene.smodelSunShadowDrawSurfs0;
  scene.drawSurfs[21] = scene.entSunShadowDrawSurfs0;
  scene.drawSurfs[22] = scene.codemeshSunShadowDrawSurfs0;
  scene.drawSurfs[23] = scene.bspSunShadowDrawSurfs1;
  scene.drawSurfs[24] = scene.smodelSunShadowDrawSurfs1;
  scene.drawSurfs[25] = scene.entSunShadowDrawSurfs1;
  scene.drawSurfs[26] = scene.codemeshSunShadowDrawSurfs1;
  scene.drawSurfs[27] = scene.bspSpotShadowDrawSurfs0;
  scene.drawSurfs[28] = scene.smodelSpotShadowDrawSurfs0;
  scene.drawSurfs[29] = scene.entSpotShadowDrawSurfs0;
  scene.drawSurfs[30] = scene.bspSpotShadowDrawSurfs1;
  scene.drawSurfs[31] = scene.smodelSpotShadowDrawSurfs1;
  scene.drawSurfs[32] = scene.entSpotShadowDrawSurfs1;
  scene.drawSurfs[33] = scene.bspSpotShadowDrawSurfs2;
  scene.drawSurfs[34] = scene.smodelSpotShadowDrawSurfs2;
  scene.drawSurfs[35] = scene.entSpotShadowDrawSurfs2;
  scene.drawSurfs[36] = scene.bspSpotShadowDrawSurfs3;
  scene.drawSurfs[37] = scene.smodelSpotShadowDrawSurfs3;
  scene.drawSurfs[38] = scene.entSpotShadowDrawSurfs3;
  scene.maxDrawSurfCount[0] = 0x2000;
  scene.maxDrawSurfCount[1] = 0x2000;
  scene.maxDrawSurfCount[2] = 0x2000;
  scene.maxDrawSurfCount[9] = 1024;
  scene.maxDrawSurfCount[3] = 512;
  scene.maxDrawSurfCount[4] = 512;
  scene.maxDrawSurfCount[5] = 512;
  scene.maxDrawSurfCount[6] = 0x2000;
  scene.maxDrawSurfCount[7] = 0x2000;
  scene.maxDrawSurfCount[8] = 0x2000;
  scene.maxDrawSurfCount[10] = 0x2000;
  scene.maxDrawSurfCount[11] = 0x2000;
  scene.maxDrawSurfCount[12] = 0x2000;
  scene.maxDrawSurfCount[13] = 0x2000;
  scene.maxDrawSurfCount[14] = 0x2000;
  scene.maxDrawSurfCount[15] = 0x2000;
  scene.maxDrawSurfCount[16] = 0x2000;
  scene.maxDrawSurfCount[17] = 0x2000;
  scene.maxDrawSurfCount[18] = 0x2000;
  scene.maxDrawSurfCount[19] = 4096;
  scene.maxDrawSurfCount[20] = 4096;
  scene.maxDrawSurfCount[21] = 4096;
  scene.maxDrawSurfCount[22] = 64;
  scene.maxDrawSurfCount[23] = 0x2000;
  scene.maxDrawSurfCount[24] = 0x2000;
  scene.maxDrawSurfCount[25] = 0x2000;
  scene.maxDrawSurfCount[26] = 64;
  scene.maxDrawSurfCount[27] = 256;
  scene.maxDrawSurfCount[28] = 256;
  scene.maxDrawSurfCount[29] = 512;
  scene.maxDrawSurfCount[30] = 256;
  scene.maxDrawSurfCount[31] = 256;
  scene.maxDrawSurfCount[32] = 512;
  scene.maxDrawSurfCount[33] = 256;
  scene.maxDrawSurfCount[34] = 256;
  scene.maxDrawSurfCount[35] = 512;
  scene.maxDrawSurfCount[36] = 256;
  scene.maxDrawSurfCount[37] = 256;
  scene.maxDrawSurfCount[38] = 512;
}

void __cdecl R_ClearScene(int localClientNum)
{
  unsigned int viewIndex; // [esp+4h] [ebp-4h]

  if ( !rg.inFrame
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 6667, 0, "%s", "rg.inFrame") )
  {
    __debugbreak();
  }
  if ( !Sys_IsMainThread()
    && !Sys_IsRenderThread()
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          6668,
          0,
          "%s",
          "Sys_IsMainThread() || Sys_IsRenderThread()") )
  {
    __debugbreak();
  }
  scene.dpvs.localClientNum = localClientNum;
  Com_Memset((unsigned int *)scene.sceneDObj, 0, 132 * scene.sceneDObjCount);
  Com_Memset((unsigned int *)&scene.sceneModel[0].info, 0, 76 * scene.sceneModelCount);
  Com_Memset((unsigned int *)&scene.sceneBrush[0].info.surfId, 0, 44 * scene.sceneBrushCount);
  Com_Memset((unsigned int *)scene.glassBrush, 0, 40 * scene.glassBrushCount);
  scene.addedLightCount = 0;
  memset((unsigned __int8 *)scene.drawSurfCount, 0, sizeof(scene.drawSurfCount));
  for ( viewIndex = 0; viewIndex < 7; ++viewIndex )
    Com_Memset((unsigned int *)scene.sceneModelVisData[viewIndex], 1, scene.sceneModelCount);
  scene.sceneDObjCount = 0;
  scene.sceneModelCount = 0;
  scene.sceneBrushCount = 0;
  scene.glassBrushCount = 0;
  if ( rgp.world )
    R_ClearDpvsScene();
  R_ClearSceneDynSModelVisBits();
}

void R_ClearSceneDynSModelVisBits()
{
  memset((unsigned __int8 *)scene.dynSModelVisBitsCamera, 0, sizeof(scene.dynSModelVisBitsCamera));
  memset((unsigned __int8 *)scene.dynSModelVisBitsSunShadow, 0, sizeof(scene.dynSModelVisBitsSunShadow));
}

void __cdecl R_ClearScene_For_ExtraCam(int localClientNum)
{
  unsigned int viewIndex; // [esp+4h] [ebp-4h]

  if ( !rg.inFrame
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 6711, 0, "%s", "rg.inFrame") )
  {
    __debugbreak();
  }
  if ( !Sys_IsMainThread()
    && !Sys_IsRenderThread()
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          6712,
          0,
          "%s",
          "Sys_IsMainThread() || Sys_IsRenderThread()") )
  {
    __debugbreak();
  }
  scene.dpvs.localClientNum = localClientNum;
  scene.addedLightCount = 0;
  for ( viewIndex = 0; viewIndex < 7; ++viewIndex )
    Com_Memset((unsigned int *)scene.sceneModelVisData[viewIndex], 1, scene.sceneModelCount);
  scene.drawSurfCount[6] = 0;
  scene.drawSurfCount[7] = 0;
  scene.drawSurfCount[8] = 0;
  scene.drawSurfCount[13] = 0;
  scene.drawSurfCount[14] = 0;
  scene.drawSurfCount[15] = 0;
  scene.drawSurfCount[16] = 0;
  scene.drawSurfCount[17] = 0;
  scene.drawSurfCount[18] = 0;
  if ( rgp.world )
    R_ClearDpvsScene_For_ExtraCam();
  R_ClearSceneDynSModelVisBits();
}

int __cdecl R_GetLocalClientNum()
{
  return scene.dpvs.localClientNum;
}

void __cdecl R_SetLodOrigin(const refdef_s *refdef)
{
  if ( r_lockPvs->modified )
  {
    Dvar_ClearModified(r_lockPvs);
    R_SetViewParmsForScene(refdef, &lockPvsViewParms);
  }
  R_UpdateLodParms(refdef, &rg.lodParms);
  scene.def.viewOffset[0] = refdef->vieworg[0];
  scene.def.viewOffset[1] = refdef->vieworg[1];
  scene.def.viewOffset[2] = refdef->vieworg[2];
  scene.def.time = refdef->time;
  scene.def.floatTime = (float)scene.def.time * 0.001;
  R_UpdateFrameFog(refdef->localClientNum);
  R_UpdateFrameSun();
}

void __cdecl R_UpdateFrameFog(unsigned int localClientNum)
{
  float lerpPos; // [esp+20h] [ebp-Ch]
  int fadeTime; // [esp+24h] [ebp-8h]
  GfxClientFog *f; // [esp+28h] [ebp-4h]

  if ( localClientNum >= 4
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          873,
          0,
          "localClientNum doesn't index ARRAY_COUNT(rg.clientFogs)\n\t%i not in [0, %i)",
          localClientNum,
          4) )
  {
    __debugbreak();
  }
  f = &rg.clientFogs[localClientNum];
  if ( scene.def.time < f->settings[4].finishTime )
  {
    fadeTime = f->settings[4].finishTime - f->settings[4].startTime;
    if ( fadeTime <= 0 )
      fadeTime = 1;
    lerpPos = (float)(scene.def.time - f->settings[4].startTime) / (float)fadeTime;
    if ( lerpPos > 1.0 )
      lerpPos = FLOAT_1_0;
    f->settings[2].fogStart = (float)((float)(f->settings[4].fogStart - f->settings[3].fogStart) * lerpPos)
                            + f->settings[3].fogStart;
    f->settings[2].heightDensity = (float)((float)(f->settings[4].heightDensity - f->settings[3].heightDensity) * lerpPos)
                                 + f->settings[3].heightDensity;
    f->settings[2].density = (float)((float)(f->settings[4].density - f->settings[3].density) * lerpPos)
                           + f->settings[3].density;
    f->settings[2].baseHeight = (float)((float)(f->settings[4].baseHeight - f->settings[3].baseHeight) * lerpPos)
                              + f->settings[3].baseHeight;
    lerp_0(
      (float (*)[4])f->settings[2].color,
      (const float (*)[4])f->settings[3].color,
      (const float (*)[4])f->settings[4].color,
      lerpPos);
    lerp_0(
      (float (*)[4])f->settings[2].sunFogColor,
      (const float (*)[4])f->settings[3].sunFogColor,
      (const float (*)[4])f->settings[4].sunFogColor,
      lerpPos);
    lerp_1(
      (float (*)[3])f->settings[2].sunFogDir,
      (const float (*)[3])f->settings[3].sunFogDir,
      (const float (*)[3])f->settings[4].sunFogDir,
      lerpPos);
    Vec3Normalize(f->settings[2].sunFogDir);
    f->settings[2].sunFogStartAng = lerp(f->settings[3].sunFogStartAng, f->settings[4].sunFogStartAng, lerpPos);
    f->settings[2].sunFogEndAng = lerp(f->settings[3].sunFogEndAng, f->settings[4].sunFogEndAng, lerpPos);
    f->settings[2].maxDensity = (float)((float)(f->settings[4].maxDensity - f->settings[3].maxDensity) * lerpPos)
                              + f->settings[3].maxDensity;
  }
  else
  {
    memcpy(&f->settings[2], &f->settings[4], sizeof(f->settings[2]));
  }
  if ( f->index && !r_fog_disable->current.enabled )
    memcpy(&frontEndDataOut->fogSettings, &f->settings[2], sizeof(frontEndDataOut->fogSettings));
  else
    memset((unsigned __int8 *)&frontEndDataOut->fogSettings, 0, sizeof(frontEndDataOut->fogSettings));
}

double __cdecl lerp(float from, float to, float t)
{
  return (to - from) * t + from;
}

void __cdecl R_SetViewParmsForScene(const refdef_s *refdef, GfxViewParms *viewParms)
{
  float v2; // [esp+Ch] [ebp-28h]
  float DefaultNearClip; // [esp+10h] [ebp-24h]

  memset((unsigned __int8 *)viewParms, 0, sizeof(GfxViewParms));
  viewParms->origin[0] = refdef->vieworg[0];
  viewParms->origin[1] = refdef->vieworg[1];
  viewParms->origin[2] = refdef->vieworg[2];
  viewParms->origin[3] = FLOAT_1_0;
  viewParms->axis[0][0] = refdef->viewaxis[0][0];
  viewParms->axis[0][1] = refdef->viewaxis[0][1];
  viewParms->axis[0][2] = refdef->viewaxis[0][2];
  viewParms->axis[1][0] = refdef->viewaxis[1][0];
  viewParms->axis[1][1] = refdef->viewaxis[1][1];
  viewParms->axis[1][2] = refdef->viewaxis[1][2];
  viewParms->axis[2][0] = refdef->viewaxis[2][0];
  viewParms->axis[2][1] = refdef->viewaxis[2][1];
  viewParms->axis[2][2] = refdef->viewaxis[2][2];
  MatrixForViewer(viewParms->origin, viewParms->axis, viewParms->viewMatrix.m);
  if ( refdef->zNear <= 0.0 )
    DefaultNearClip = R_GetDefaultNearClip();
  else
    DefaultNearClip = refdef->zNear;
  if ( (float)(1.0 - DefaultNearClip) < 0.0 )
    v2 = DefaultNearClip;
  else
    v2 = MINIMUM_Z_NEAR;
  viewParms->zNear = v2;
  R_SetupProjection(refdef->tanHalfFovX, refdef->tanHalfFovY, viewParms);
  R_SetupViewProjectionMatrices(viewParms, 1);
}

void __cdecl R_SetupProjection(float tanHalfFovX, float tanHalfFovY, GfxViewParms *viewParms)
{
  InfinitePerspectiveMatrix(tanHalfFovX, tanHalfFovY, viewParms->zNear, viewParms->projectionMatrix.m);
  LODWORD(viewParms->depthHackNearClip) = r_znear_depthhack->current.integer ^ _mask__NegFloat_;
}

GfxBackEndData *R_UpdateFrameSun()
{
  GfxBackEndData *result; // eax
  bool v1; // [esp+Ch] [ebp-38h]
  int v2; // [esp+10h] [ebp-34h]
  int v3; // [esp+14h] [ebp-30h]
  float *specularColor; // [esp+38h] [ebp-Ch]
  float *diffuseColor; // [esp+3Ch] [ebp-8h]
  float *dir; // [esp+40h] [ebp-4h]

  if ( !rgp.world
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 6992, 0, "%s", "rgp.world") )
  {
    __debugbreak();
  }
  if ( !rgp.world->sunLight
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          6993,
          0,
          "%s",
          "rgp.world->sunLight") )
  {
    __debugbreak();
  }
  memcpy(&frontEndDataOut->sunLight, rgp.world->sunLight, sizeof(frontEndDataOut->sunLight));
  if ( rg.useSunDirOverride )
  {
    if ( rg.useSunDirLerp )
    {
      R_LerpDir(
        rg.sunDirOverride,
        rg.sunDirOverrideTarget,
        rg.sunDirLerpBeginTime,
        rg.sunDirLerpEndTime,
        scene.def.time,
        frontEndDataOut->sunLight.dir);
    }
    else
    {
      dir = frontEndDataOut->sunLight.dir;
      frontEndDataOut->sunLight.dir[0] = rg.sunDirOverride[0];
      dir[1] = rg.sunDirOverride[1];
      dir[2] = rg.sunDirOverride[2];
    }
  }
  if ( rg.useSunLightOverride )
  {
    diffuseColor = frontEndDataOut->sunLight.diffuseColor;
    frontEndDataOut->sunLight.diffuseColor[0] = rg.diffuseSunLightOverride[0];
    diffuseColor[1] = rg.diffuseSunLightOverride[1];
    diffuseColor[2] = rg.diffuseSunLightOverride[2];
    specularColor = frontEndDataOut->sunLight.specularColor;
    frontEndDataOut->sunLight.specularColor[0] = rg.specularSunLightOverride[0];
    specularColor[1] = rg.specularSunLightOverride[1];
    specularColor[2] = rg.specularSunLightOverride[2];
  }
  v2 = 0;
  if ( sm_enable->current.enabled )
  {
    if ( rg.useSunDirOverride
      || (*(float *)(r_lightTweakSunDirection.integer + 24) != *(float *)(r_lightTweakSunDirection.integer + 56)
       || *(float *)(r_lightTweakSunDirection.integer + 28) != *(float *)(r_lightTweakSunDirection.integer + 60)
       || *(float *)(r_lightTweakSunDirection.integer + 32) != *(float *)(r_lightTweakSunDirection.integer + 64)
        ? (v3 = 0)
        : (v3 = 1),
          !v3) )
    {
      v2 = 1;
    }
  }
  result = frontEndDataOut;
  frontEndDataOut->prim.hasSunDirChanged = v2;
  v1 = rg.useSunDirOverride
    || AngleNormalize180(*(float *)(r_lightTweakSunDirection.integer + 24) - *(float *)(r_lightTweakSunDirection.integer
                                                                                      + 56)) > 5.0
    || AngleNormalize180(*(float *)(r_lightTweakSunDirection.integer + 28) - *(float *)(r_lightTweakSunDirection.integer
                                                                                      + 60)) > 5.0;
  frontEndDataOut->hasApproxSunDirChanged = v1;
  return result;
}

void __cdecl R_LerpDir(
        const float *dirBegin,
        const float *dirEnd,
        int beginLerpTime,
        int endLerpTime,
        int currTime,
        float *result)
{
  float fraction; // [esp+8h] [ebp-18h]
  float v7; // [esp+18h] [ebp-8h]
  float lerpFraction; // [esp+1Ch] [ebp-4h]

  if ( endLerpTime <= beginLerpTime
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          6981,
          0,
          "%s",
          "endLerpTime > beginLerpTime") )
  {
    __debugbreak();
  }
  lerpFraction = (float)(currTime - beginLerpTime) / (float)(endLerpTime - beginLerpTime);
  if ( (float)(lerpFraction - 1.0) < 0.0 )
    v7 = (float)(currTime - beginLerpTime) / (float)(endLerpTime - beginLerpTime);
  else
    v7 = FLOAT_1_0;
  if ( (float)(0.0 - lerpFraction) < 0.0 )
    fraction = v7;
  else
    fraction = *(float *)&FLOAT_0_0;
  Vec3Lerp(dirBegin, dirEnd, fraction, result);
  Vec3Normalize(result);
}

void __cdecl R_UpdateLodParms(const refdef_s *refdef, GfxLodParms *lodParms)
{
  float v2; // [esp+4h] [ebp-2Ch]
  float v3; // [esp+8h] [ebp-28h]
  float value; // [esp+14h] [ebp-1Ch]
  float v5; // [esp+18h] [ebp-18h]
  float invFovScale; // [esp+2Ch] [ebp-4h]

  if ( r_lockPvs->current.enabled )
  {
    lodParms->origin[0] = lockPvsViewParms.origin[0];
    lodParms->origin[1] = lockPvsViewParms.origin[1];
    lodParms->origin[2] = lockPvsViewParms.origin[2];
  }
  else
  {
    lodParms->origin[0] = refdef->vieworg[0];
    lodParms->origin[1] = refdef->vieworg[1];
    lodParms->origin[2] = refdef->vieworg[2];
  }
  lodParms->extraCamOriginValid = refdef->extraCamPosValid;
  lodParms->extraCamOrigin[0] = refdef->extraCamPos[0];
  lodParms->extraCamOrigin[1] = refdef->extraCamPos[1];
  lodParms->extraCamOrigin[2] = refdef->extraCamPos[2];
  invFovScale = refdef->tanHalfFovY * 2.1186731;
  value = r_lodScaleRigid->current.value;
  v5 = r_lodBiasRigid->current.value;
  if ( invFovScale < lodParms->ramp[0].appliedInvScale )
  {
    if ( (float)(lodParms->ramp[0].appliedInvScale / invFovScale) > r_fovScaleThresholdRigid->current.value )
      lodParms->ramp[0].appliedInvScale = invFovScale;
  }
  else
  {
    lodParms->ramp[0].appliedInvScale = invFovScale;
  }
  lodParms->ramp[0].scale = value * lodParms->ramp[0].appliedInvScale;
  lodParms->ramp[0].bias = v5 * lodParms->ramp[0].appliedInvScale;
  v2 = r_lodScaleSkinned->current.value;
  v3 = r_lodBiasSkinned->current.value;
  if ( invFovScale < lodParms->ramp[1].appliedInvScale )
  {
    if ( (float)(lodParms->ramp[1].appliedInvScale / invFovScale) > r_fovScaleThresholdSkinned->current.value )
      lodParms->ramp[1].appliedInvScale = invFovScale;
  }
  else
  {
    lodParms->ramp[1].appliedInvScale = invFovScale;
  }
  lodParms->ramp[1].scale = v2 * lodParms->ramp[1].appliedInvScale;
  lodParms->ramp[1].bias = v3 * lodParms->ramp[1].appliedInvScale;
  lodParms->valid = 1;
}

void __cdecl R_RenderScene(refdef_s *refdef, int frameTime)
{
  GfxViewParms *v2; // [esp+Ch] [ebp-14F8h]
  GfxSceneParms sceneParms; // [esp+1Ch] [ebp-14E8h] BYREF
  GfxViewParms *viewParms; // [esp+1500h] [ebp-4h]

  if ( refdef->tanHalfFovX <= 0.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          7168,
          0,
          "%s",
          "refdef->tanHalfFovX > 0") )
  {
    __debugbreak();
  }
  if ( refdef->tanHalfFovY <= 0.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          7169,
          0,
          "%s",
          "refdef->tanHalfFovY > 0") )
  {
    __debugbreak();
  }
  if ( !refdef->height
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 7170, 0, "%s", "refdef->height > 0") )
  {
    __debugbreak();
  }
  if ( !refdef->width
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 7171, 0, "%s", "refdef->width > 0") )
  {
    __debugbreak();
  }
  if ( refdef->localClientNum >= gfxCfg.maxClientViews
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          7172,
          0,
          "refdef->localClientNum doesn't index gfxCfg.maxClientViews\n\t%i not in [0, %i)",
          refdef->localClientNum,
          gfxCfg.maxClientViews) )
  {
    __debugbreak();
  }
  if ( rg.registered && !r_norefresh->current.enabled )
  {
    if ( r_logFile->current.integer )
      RB_LogPrint("====== R_RenderScene ======\n");
    if ( !rgp.world )
      Com_Error(ERR_DROP, &byte_D6D070);
    rg.viewOrg[0] = refdef->vieworg[0];
    rg.viewOrg[1] = refdef->vieworg[1];
    rg.viewOrg[2] = refdef->vieworg[2];
    *(_QWORD *)rg.viewDir = *(_QWORD *)&refdef->viewaxis[0][0];
    rg.viewDir[2] = refdef->viewaxis[0][2];
    R_MaterialParameterTweakUpdate();
    R_UpdateExposureValue(refdef);
    R_CheckShadowMapVolumes(refdef->vieworg);
    viewParms = R_AllocViewParms();
    R_SetViewParmsForScene(refdef, viewParms);
    R_SetSceneParms(refdef, &sceneParms);
    R_CorrectLodScale(refdef);
    R_WorldLod_FrameUpdate(scene.def.floatTime, viewParms->origin, refdef->localClientNum);
    if ( scene.def.time != refdef->time
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
            7213,
            0,
            "%s",
            "scene.def.time == refdef->time") )
    {
      __debugbreak();
    }
    if ( !rg.lodParms.valid
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
            7215,
            0,
            "%s",
            "rg.lodParms.valid") )
    {
      __debugbreak();
    }
    if ( r_lockPvs->current.enabled )
      v2 = &lockPvsViewParms;
    else
      v2 = viewParms;
    frontEndDataOut->hideMatureContent = refdef->hideMatureContent;
    frontEndDataOut->splitscreen = refdef->splitscreen;
    frontEndDataOut->extraCamLargeFrameSize = 1;
    R_GenerateSortedDrawSurfs(&sceneParms, v2, viewParms, frameTime, refdef, 0);
  }
}

void __cdecl R_CheckShadowMapVolumes(const float *position)
{
  unsigned int j; // [esp+0h] [ebp-18h]
  unsigned int control; // [esp+4h] [ebp-14h]
  unsigned int i; // [esp+Ch] [ebp-Ch]
  unsigned int planeIndex; // [esp+10h] [ebp-8h]

  if ( sm_sunShadowSmall->current.enabled )
    Dvar_SetBoolIfChanged((dvar_s *)sm_sunShadowSmallEnable, 1);
  else
    Dvar_SetBoolIfChanged((dvar_s *)sm_sunShadowSmallEnable, 0);
  Dvar_SetBoolIfChanged((dvar_s *)sm_sunEnable, 1);
  Dvar_SetBoolIfChanged((dvar_s *)sm_spotEnable, 1);
  if ( rgp.world->shadowMapVolumeCount )
  {
    i = 0;
    while ( 2 )
    {
      control = rgp.world->shadowMapVolumes[i].control;
      planeIndex = (unsigned __int16)control;
      for ( j = 0; ; ++j )
      {
        if ( j >= (unsigned int)BYTE2(control) + 1 )
        {
          if ( ((unsigned int)&cls.rankedServers[711].game[35] & control) != 0 )
            Dvar_SetBoolIfChanged((dvar_s *)sm_sunEnable, 0);
          if ( ((unsigned int)&cls.wagerServers[5331].basictraining & control) != 0 )
            Dvar_SetBoolIfChanged((dvar_s *)sm_sunShadowSmallEnable, 1);
          if ( (control & 0x4000000) != 0 )
            Dvar_SetBoolIfChanged((dvar_s *)sm_spotEnable, 0);
          return;
        }
        if ( (float)((float)((float)(rgp.world->shadowMapVolumePlanes[planeIndex].plane[2] * position[2])
                           + (float)((float)(rgp.world->shadowMapVolumePlanes[planeIndex].plane[1] * position[1])
                                   + (float)(rgp.world->shadowMapVolumePlanes[planeIndex].plane[0] * *position)))
                   + rgp.world->shadowMapVolumePlanes[planeIndex].plane[3]) < 0.0 )
          break;
        ++planeIndex;
      }
      if ( ++i < rgp.world->shadowMapVolumeCount )
        continue;
      break;
    }
  }
}

void __cdecl R_UpdateExposureValue(refdef_s *refdef)
{
  float v1; // [esp+0h] [ebp-28h]
  float v2; // [esp+4h] [ebp-24h]
  float v3; // [esp+8h] [ebp-20h]
  float v4; // [esp+Ch] [ebp-1Ch]
  float lerpSpeed; // [esp+10h] [ebp-18h]
  float lerpBaseDecrease; // [esp+14h] [ebp-14h]
  float lerpBaseDecreasea; // [esp+14h] [ebp-14h]
  float feather; // [esp+18h] [ebp-10h] BYREF
  float targetExposure; // [esp+1Ch] [ebp-Ch]
  float lerpBaseIncrease; // [esp+20h] [ebp-8h]
  int exposureId; // [esp+24h] [ebp-4h]

  feather = FLOAT_50_0;
  exposureId = R_CheckExposureVolumes(refdef->vieworg, &feather);
  if ( exposureId == -1 )
  {
    targetExposure = rgp.world->sunParse.sunSettings[0].exposure;
    lerpBaseIncrease = FLOAT_15_0;
    lerpBaseDecrease = FLOAT_20_0;
  }
  else
  {
    if ( rgp.world->exposureVolumes[exposureId].exposure == -1.0 )
      targetExposure = exposureSettings.exposureValues[HIBYTE(rgp.world->exposureVolumes[exposureId].control)].blackPoint[3];
    else
      targetExposure = rgp.world->exposureVolumes[exposureId].exposure;
    feather = feather * rgp.world->exposureVolumes[exposureId].featherRange;
    if ( feather <= 1.0 )
      v4 = feather;
    else
      v4 = FLOAT_1_0;
    feather = v4;
    targetExposure = (float)(rgp.world->exposureVolumes[exposureId].featherAdjust * v4) + targetExposure;
    lerpBaseIncrease = 15.0 * rgp.world->exposureVolumes[exposureId].luminanceIncreaseScale;
    lerpBaseDecreasea = 20.0 * rgp.world->exposureVolumes[exposureId].luminanceDecreaseScale;
    if ( lerpBaseIncrease >= 1.0 )
      v3 = lerpBaseIncrease;
    else
      v3 = FLOAT_1_0;
    lerpBaseIncrease = v3;
    if ( lerpBaseDecreasea >= 1.0 )
      v2 = lerpBaseDecreasea;
    else
      v2 = FLOAT_1_0;
    lerpBaseDecrease = v2;
  }
  if ( refdef->exposure <= targetExposure )
    lerpSpeed = lerpBaseDecrease;
  else
    lerpSpeed = lerpBaseIncrease;
  if ( lerpSpeed >= 1.0 )
    v1 = lerpSpeed;
  else
    v1 = FLOAT_1_0;
  refdef->exposure = (float)((float)((float)(v1 - 1.0) * refdef->exposure) + targetExposure) / v1;
  refdef->exposureValue.blackPoint[0] = *(float *)&FLOAT_0_0;
  refdef->exposureValue.blackPoint[1] = *(float *)&FLOAT_0_0;
  refdef->exposureValue.blackPoint[2] = *(float *)&FLOAT_0_0;
  refdef->exposureValue.blackPoint[3] = refdef->exposure;
  refdef->exposureValue.whitePoint[0] = FLOAT_8_0;
  refdef->exposureValue.whitePoint[1] = FLOAT_8_0;
  refdef->exposureValue.whitePoint[2] = FLOAT_8_0;
}

unsigned int __cdecl R_CheckExposureVolumes(float *position, float *feather)
{
  float v3; // [esp+0h] [ebp-1Ch]
  unsigned int j; // [esp+4h] [ebp-18h]
  unsigned int control; // [esp+8h] [ebp-14h]
  unsigned int i; // [esp+10h] [ebp-Ch]
  unsigned int planeIndex; // [esp+14h] [ebp-8h]
  float dot; // [esp+18h] [ebp-4h]

  if ( !rgp.world->exposureVolumeCount )
    return -1;
  i = 0;
  do
  {
    control = rgp.world->exposureVolumes[i].control;
    planeIndex = (unsigned __int16)control;
    for ( j = 0; ; ++j )
    {
      if ( j >= (unsigned int)BYTE2(control) + 1 )
        return i;
      dot = (float)((float)(rgp.world->exposureVolumePlanes[planeIndex].plane[2] * position[2])
                  + (float)((float)(rgp.world->exposureVolumePlanes[planeIndex].plane[1] * position[1])
                          + (float)(rgp.world->exposureVolumePlanes[planeIndex].plane[0] * *position)))
          + rgp.world->exposureVolumePlanes[planeIndex].plane[3];
      if ( dot < 0.0 )
        break;
      if ( dot <= *feather )
        v3 = (float)((float)(rgp.world->exposureVolumePlanes[planeIndex].plane[2] * position[2])
                   + (float)((float)(rgp.world->exposureVolumePlanes[planeIndex].plane[1] * position[1])
                           + (float)(rgp.world->exposureVolumePlanes[planeIndex].plane[0] * *position)))
           + rgp.world->exposureVolumePlanes[planeIndex].plane[3];
      else
        v3 = *feather;
      *feather = v3;
      ++planeIndex;
    }
    ++i;
  }
  while ( i < rgp.world->exposureVolumeCount );
  return -1;
}

void __cdecl R_GenerateSortedDrawSurfs(
        const GfxSceneParms *sceneParms,
        const GfxViewParms *viewParmsDpvs,
        const GfxViewParms *viewParmsDraw,
        int frameTime,
        const refdef_s *refdef,
        bool forMissileCamView)
{
  int v6; // eax
  jpeg_decompress_struct *v7; // [esp+10h] [ebp-280h]
  jpeg_decompress_struct *v8; // [esp+10h] [ebp-280h]
  bool v9; // [esp+18h] [ebp-278h]
  bool v11; // [esp+23h] [ebp-26Dh]
  unsigned int dynamicSModelCount; // [esp+24h] [ebp-26Ch]
  int VisibleDLights; // [esp+2Ch] [ebp-264h]
  float v14; // [esp+40h] [ebp-250h]
  float v15; // [esp+7Ch] [ebp-214h]
  bool v16; // [esp+C3h] [ebp-1CDh]
  GfxImage *blackImage; // [esp+1BCh] [ebp-D4h]
  GfxImage *v18; // [esp+1C4h] [ebp-CCh]
  GfxImage *image; // [esp+1CCh] [ebp-C4h]
  float v20; // [esp+1D8h] [ebp-B8h]
  float v21; // [esp+1DCh] [ebp-B4h]
  char *Name; // [esp+1E0h] [ebp-B0h]
  int j; // [esp+1E8h] [ebp-A8h]
  int partitionIndex; // [esp+1ECh] [ebp-A4h]
  unsigned int i; // [esp+1F4h] [ebp-9Ch]
  float bestError; // [esp+1F8h] [ebp-98h]
  unsigned int bestNum; // [esp+1FCh] [ebp-94h]
  unsigned int bestDen; // [esp+208h] [ebp-88h]
  unsigned int den; // [esp+20Ch] [ebp-84h]
  GfxDrawSurfListInfo *litPunchThruInfo; // [esp+214h] [ebp-7Ch]
  int pointLightCount; // [esp+218h] [ebp-78h]
  int firstDrawSurfCount; // [esp+21Ch] [ebp-74h]
  GfxDrawSurfListInfo *litPostResolveInfo; // [esp+220h] [ebp-70h]
  GfxDrawSurfListInfo *litInfo; // [esp+224h] [ebp-6Ch]
  bool usePreTess; // [esp+22Bh] [ebp-65h]
  DynSModelClient smodelClient; // [esp+22Ch] [ebp-64h] BYREF
  GfxViewInfo *viewInfo; // [esp+240h] [ebp-50h]
  int viewInfoIndex; // [esp+244h] [ebp-4Ch]
  bool forStereoRightEyeView; // [esp+24Bh] [ebp-45h]
  int semaphore; // [esp+24Ch] [ebp-44h]
  ShadowType dynamicShadowType; // [esp+250h] [ebp-40h]
  SceneEntCmd sceneEntCmd; // [esp+254h] [ebp-3Ch] BYREF
  FxCmd cmd; // [esp+258h] [ebp-38h] BYREF
  bool extraCamActive; // [esp+28Fh] [ebp-1h]

  Name = va("R_GenerateSortedDrawSurfs (cl=%d v=%d)", sceneParms->localClientNum, rg.viewInfoCount);
  PIXBeginNamedEvent(-1, Name);
  forStereoRightEyeView = 0;
  if ( frontEndDataOut->viewInfoCount != rg.viewInfoCount
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          5435,
          0,
          "%s",
          "frontEndDataOut->viewInfoCount == rg.viewInfoCount") )
  {
    __debugbreak();
  }
  viewInfoIndex = rg.viewInfoCount++;
  frontEndDataOut->viewInfoCount = rg.viewInfoCount;
  if ( (unsigned int)viewInfoIndex >= 4
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          5441,
          1,
          "viewInfoIndex doesn't index GFX_MAX_CLIENT_VIEWS\n\t%i not in [0, %i)",
          viewInfoIndex,
          4) )
  {
    __debugbreak();
  }
  frontEndDataOut->viewInfoIndex = viewInfoIndex;
  viewInfo = &frontEndDataOut->viewInfo[viewInfoIndex];
  viewInfo->hasCmdBuf = viewInfoIndex == 0;

  if ( !forMissileCamView || R_ExtraCam_UseShadowmap() )
    dynamicShadowType = (ShadowType)R_DynamicShadowType();
  else
    dynamicShadowType = SHADOW_NONE;

  viewInfo->dynSModelView = &frontEndDataOut->dynSModelClientViewArray[viewInfoIndex];
  viewInfo->dynSModelState = &frontEndDataOut->dynSModelState;
  BG_EvalVehicleName((jpeg_decompress_struct *)viewInfo->dynSModelView);
  rg.isNotRenderingFullScreen = !sceneParms->isRenderingFullScreen;
  rg.sunShadowFull = 1;
  LODWORD(rg.sunShadowmapScale) = sm_sunShadowScale->current.integer;
  bestDen = 1;
  bestNum = 1;
  bestError = FLOAT_1_0;
  for ( den = 1; den <= 10; ++den )
  {
    v21 = (double)den * rg.sunShadowmapScale + 0.5;
    v15 = floor(v21);
    v20 = (double)(unsigned int)(__int64)v15 / (double)den - rg.sunShadowmapScale;
    if ( COERCE_FLOAT(LODWORD(v20) & _mask__AbsFloat_) < bestError )
    {
      LODWORD(bestError) = LODWORD(v20) & _mask__AbsFloat_;
      bestDen = den;
      bestNum = (__int64)v15;
    }
  }
  rg.sunShadowmapScale = (double)bestNum / (double)bestDen;
  rg.sunShadowmapScaleNum = (float)bestNum;
  v14 = ceil((float)((float)(int)dx.singleSampleDepthStencilSurface * rg.sunShadowmapScale));
  rg.sunShadowSize = (__int64)v14;
  rg.sunShadowPartitionRatio = 4.0 / rg.sunShadowmapScale;
  rg.drawSunShadow = 0;

  if ( !rg.lodParms.valid
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 5543, 0, "%s", "rg.lodParms.valid") )
  {
    __debugbreak();
  }
  if ( !rg.correctedLodParms[sceneParms->localClientNum].valid
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          5544,
          0,
          "%s",
          "rg.correctedLodParms[sceneParms->localClientNum].valid") )
  {
    __debugbreak();
  }

  memcpy(&viewInfo->input, &gfxCmdBufInput, sizeof(viewInfo->input));
  viewInfo->input.data = frontEndDataOut;
  viewInfo->sceneDef = scene.def;
  memcpy(viewInfo, viewParmsDraw, 0x140u);
  viewInfo->cullViewInfo.sceneViewport = sceneParms->sceneViewport;
  viewInfo->cullViewInfo.displayViewport = sceneParms->displayViewport;
  viewInfo->cullViewInfo.scissorViewport = sceneParms->scissorViewport;
  viewInfo->dynamicShadowType = dynamicShadowType;
  viewInfo->localClientNum = sceneParms->localClientNum;
  viewInfo->isRenderingFullScreen = sceneParms->isRenderingFullScreen;
  viewInfo->playerTeleported = sceneParms->playerTeleported;
  viewInfo->blurRadius = sceneParms->blurRadius;
  viewInfo->noLodCullOut = refdef->noLodCullOut;
  viewInfo->needsFloatZ = 0;
  frontEndDataOut->sunShadow.sunShadowCount = 0;
  frontEndDataOut->spotShadowCount = 0;
  Com_Memcpy(frontEndDataOut->shadowableLights, sceneParms->primaryLights, 368 * rgp.world->primaryLightCount);
  frontEndDataOut->shadowableLightCount = rgp.world->primaryLightCount;
  if ( forMissileCamView )
  {
    viewInfo->isMissileCamera = 1;
    viewInfo->sceneComposition.mainSceneMSAA = 22;
    image = stru_B50E9FC.image;
    if ( viewInfo == (GfxViewInfo *)-9872
      && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state.h", 1850, 0, "%s", "input") )
    {
      __debugbreak();
    }
    viewInfo->input.codeImages[19] = image;
    v18 = rt.image;
    if ( viewInfo == (GfxViewInfo *)-9872
      && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state.h", 1850, 0, "%s", "input") )
    {
      __debugbreak();
    }
    viewInfo->input.codeImages[9] = v18;
    blackImage = rgp.blackImage;
    if ( viewInfo == (GfxViewInfo *)-9872
      && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state.h", 1850, 0, "%s", "input") )
    {
      __debugbreak();
    }
    viewInfo->input.codeImages[11] = blackImage;
    R_SetInputCodeConstant(&viewInfo->input, 0xC3u, 1.0, 1.0, 1.0, 1.0);
  }
  else
  {
    viewInfo->isMissileCamera = 0;
    viewInfo->sceneComposition.mainSceneMSAA = 3;
    R_SetInputCodeConstant(&viewInfo->input, 0xC3u, 0.0, 0.0, 0.0, 0.0);
  }
  if ( r_debugShowPrimaryLights->current.enabled )
  {
    for ( i = 0; i < frontEndDataOut->shadowableLightCount; ++i )
      DrawLightDebug(viewInfo, &frontEndDataOut->shadowableLights[i], colorWhite);
  }
  viewInfo->sunVisibility = sceneParms->sunVisibility;
  scene.shadowableLightIsUsed[0] = 0;
  scene.shadowableLightIsUsed[1] = 0;
  scene.shadowableLightIsUsed[2] = 0;
  scene.shadowableLightIsUsed[3] = 0;
  scene.shadowableLightIsUsed[4] = 0;
  scene.shadowableLightIsUsed[5] = 0;
  scene.shadowableLightIsUsed[6] = 0;
  scene.shadowableLightIsUsed[7] = 0;
  R_SetHeroLighting(&viewInfo->input, viewInfo, refdef);
  R_SetSkyDynamicIntensity(viewInfo->cullViewInfo.viewParms.axis[0], &viewInfo->input);
  R_SetupDynamicModelLighting(&viewInfo->input);
  R_SetFrameFog(&viewInfo->input, viewInfo);
  R_SetSunConstants(&viewInfo->input, 1.0);
  DrawSunDirectionDebug(viewParmsDraw->origin, viewParmsDraw->axis[0]);
  DrawOutdoorBoundsVolumeDebug();
  R_SetDepthOfField(viewInfo, sceneParms);
  R_SetFilmInfo(viewInfo, sceneParms);
  R_SetBloomInfo(viewInfo, sceneParms);
  R_SetCharacterPrimaryLightScaleInfo(viewInfo, sceneParms);
  R_DrawCineWarning();
  R_DrawFogParams();
  R_SetInputCodeConstant(&viewInfo->input, 0x31u, 0.984375 * viewParmsDraw->zNear, 0.0, 0.0, 0.0);
  R_SetFullSceneViewMesh(viewInfoIndex, viewInfo);
  R_SetupWorldSurfacesDpvs(viewParmsDpvs, forMissileCamView ? 0x41 : 0);
  R_SetViewFrustumPlanes(viewInfo);
  R_SetDoubleVision(viewInfo, sceneParms);
  R_SetWaterSheetingFx(viewInfo, sceneParms);
  R_SetFlameFx(viewInfo, sceneParms);
  R_SetReviveFx(viewInfo, sceneParms);
  R_SetPoisonFx(viewInfo, sceneParms);
  R_SetGenericFilter(viewInfo, sceneParms);
  R_SetElectrifiedFx(viewInfo, sceneParms);
  R_SetTransportedFx(viewInfo, sceneParms);
  GlassCl_BeginGenerateVerts(sceneParms->localClientNum, viewInfoIndex);
  GlassCl_Update(sceneParms->localClientNum);
  memcpy(&viewInfo->saveScreenFx, &sceneParms->saveScreenFx, sizeof(viewInfo->saveScreenFx));
  R_SetSceneComposition(viewInfo, sceneParms, forMissileCamView);
  R_SetExposure(viewInfo, refdef);
  R_SetHDRControlConstants(&viewInfo->input, viewInfo);

  PIXBeginNamedEvent(-1, "R_InitialEntityCulling");
  R_InitialEntityCulling();
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  PIXBeginNamedEvent(-1, "R_AddWorldSurfacesDpvs");
  v6 = R_CellForPoint(viewParmsDpvs->origin);
  R_AddWorldSurfacesDpvs(viewParmsDpvs, v6);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  PIXBeginNamedEvent(-1, "WaitFXSpot");
  Sys_WaitWorkerCmdInternal(&fx_update_spotWorkerCmd);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  R_AddEmissiveSpotLight(viewInfo);
  R_WaitForFXUpdateWorkerCmds();
  if ( forMissileCamView )
    VisibleDLights = 0;
  else
    VisibleDLights = R_GetVisibleDLights(viewInfo->visibleLights);
  viewInfo->visibleLightCount = VisibleDLights;
  if ( frontEndDataOut->emissiveSpotLightCount
    && !viewInfo->visibleLightCount
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          5714,
          0,
          "%s",
          "frontEndDataOut->emissiveSpotLightCount == 0 || viewInfo->visibleLightCount != 0") )
  {
    __debugbreak();
  }
  R_BeginAllStaticModelLighting();
  extraCamActive = refdef->extraCamPosValid;
  if ( Sys_QueryD3DDeviceOKEvent() )
    usePreTess = r_pretess->current.enabled;
  else
    usePreTess = 0;
  if ( usePreTess )
  {
    if ( extraCamActive && viewInfo->isMissileCamera )
      R_BeginPreTess(0);
    else
      R_BeginPreTess(1);
  }
  PIXBeginNamedEvent(-1, "wait for r_dpvs_static");
  Sys_WaitWorkerCmdInternal(&r_dpvs_staticWorkerCmd);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  R_AddSkyboxModel(viewParmsDraw->origin);
  PIXBeginNamedEvent(-1, "bsp surfaces");
  if ( gfxDrawMethod.drawScene == GFX_DRAW_SCENE_STANDARD )
  {
    R_AddAllBspDrawSurfacesCamera(viewInfo->localClientNum, viewInfo->visibleLights, viewInfo->visibleLightCount);
    if ( rgp.world->sunPrimaryLightIndex )
    {
      if ( sm_sunEnable->current.enabled )
      {
        if ( sm_sunAlwaysCastsShadow->current.enabled )
          Com_BitSetAssert(scene.shadowableLightIsUsed, rgp.world->sunPrimaryLightIndex, 128);
      }
      else
      {
        Com_BitClearAssert(scene.shadowableLightIsUsed, rgp.world->sunPrimaryLightIndex, 128);
      }
    }
    Com_Memset(frontEndDataOut->shadowableLightHasShadowMap, 0, 32);
    if ( R_GetAllowShadowMaps() )
      R_ChooseShadowedLights(viewInfo);
    R_UpdateDrawMethod(frontEndDataOut, viewInfo);
    if ( dynamicShadowType == SHADOW_MAP
      && Com_BitCheckAssert(frontEndDataOut->shadowableLightHasShadowMap, rgp.world->sunPrimaryLightIndex, 32) )
    {
      rg.drawSunShadow = 1;
      R_SetupSunShadowMaps(viewParmsDpvs, &frontEndDataOut->sunShadow);
      R_SetSunShadowConstants(&viewInfo->input, &frontEndDataOut->sunShadow.sunProj);
      R_SunShadowMaps(&frontEndDataOut->sunShadow);
    }
  }
  else
  {
    R_AddAllBspDrawSurfacesCameraNonlit(rgp.world->dpvs.litSurfsBegin, rgp.world->dpvs.litSurfsEnd, 0);
    R_AddAllBspDrawSurfacesCameraNonlit(rgp.world->dpvs.decalSurfsBegin, rgp.world->dpvs.decalSurfsEnd, 3u);
  }
  R_AddAllBspDrawSurfacesCameraNonlit(rgp.world->dpvs.emissiveSurfsBegin, rgp.world->dpvs.emissiveSurfsEnd, 0xAu);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  R_AddAllStaticModelSurfacesCamera(
    viewInfo->localClientNum,
    viewInfo->playerTeleported,
    viewInfo->visibleLights,
    viewInfo->visibleLightCount,
    0);
  PIXBeginNamedEvent(-1, "DynEntPieces_AddDrawSurfs");
  DynEntPieces_AddDrawSurfs(viewInfo->localClientNum);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  PIXBeginNamedEvent(-1, "wait for r_dpvs_dynmodel");
  Sys_WaitWorkerCmdInternal(&r_dpvs_dynmodelWorkerCmd);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  PIXBeginNamedEvent(-1, "wait for r_dpvs_dynbrush");
  Sys_WaitWorkerCmdInternal(&r_dpvs_dynbrushWorkerCmd);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  PIXBeginNamedEvent(-1, "draw all dyn ent");
  R_DrawAllDynEnt(viewInfo);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  if ( gfxDrawMethod.drawScene == GFX_DRAW_SCENE_STANDARD
    && dynamicShadowType == SHADOW_MAP
    && Com_BitCheckAssert(frontEndDataOut->shadowableLightHasShadowMap, rgp.world->sunPrimaryLightIndex, 32) )
  {
    PIXBeginNamedEvent(-1, "wait for more r_dpvs_static");
    Sys_WaitWorkerCmdInternal(&r_dpvs_staticWorkerCmd);
    if ( g_DXDeviceThread == GetCurrentThreadId() )
      D3DPERF_EndEvent();
    PIXBeginNamedEvent(-1, "bsp surfaces");
    R_AddAllBspDrawSurfacesSunShadow(viewInfo->localClientNum);
    if ( g_DXDeviceThread == GetCurrentThreadId() )
      D3DPERF_EndEvent();
    PIXBeginNamedEvent(-1, "static model surfaces");
    R_AddAllStaticModelSurfacesSunShadow(viewInfoIndex);
    if ( GetCurrentThreadId() == g_DXDeviceThread )
      D3DPERF_EndEvent();
  }
  semaphore = R_ReleaseDXDeviceOwnership();
  FX_BeginMarks(viewInfo->localClientNum);
  memset((unsigned __int8 *)&cmd, 0, sizeof(cmd));
  cmd.localClientNum = viewInfo->localClientNum;
  cmd.visibleLights = viewInfo->visibleLights;
  cmd.visibleLightCount = viewInfo->visibleLightCount;
  Sys_AddWorkerCmdInternal(&fx_marks_drawWorkerCmd, &cmd, 0);
  PIXBeginNamedEvent(-1, "wait for r_dpvs_sceneent r_dpvs_entity");
  Sys_WaitWorkerCmdInternal(&r_dpvs_sceneentWorkerCmd);
  Sys_WaitWorkerCmdInternal(&r_dpvs_entityWorkerCmd);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  PIXBeginNamedEvent(-1, "wait for r_spot_shadow_ent");
  Sys_WaitWorkerCmdInternal(&r_spot_shadow_entWorkerCmd);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  if ( semaphore )
    R_AcquireDXDeviceOwnership(0);
  PIXBeginNamedEvent(-1, "draw all scene ent");
  R_DrawAllSceneEnt(viewInfo);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  PIXBeginNamedEvent(-1, "wait for dobj_skin");
  Sys_WaitWorkerCmdInternal(&dobj_skinWorkerCmd);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  PIXBeginNamedEvent(-1, "add workercmd r_add_sceneent");
  sceneEntCmd.viewInfo = viewInfo;
  Sys_AddWorkerCmdInternal(&r_add_sceneentWorkerCmd, &sceneEntCmd, 0);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  R_SortAllStaticModelSurfacesCamera();
  R_GetLightSurfs(viewInfo->visibleLights, viewInfo->visibleLightCount);
  R_GetPointLightShadowSurfs(viewInfo, scene.visLightShadow, viewInfo->visibleLights, viewInfo->visibleLightCount);
  if ( viewInfo->visibleLightCount )
    R_SetDLightsConstants(&viewInfo->input, viewInfo, viewInfo->visibleLights, viewInfo->visibleLightCount);
  else
    R_SetDLightsConstantsDefaults(&viewInfo->input);
  if ( gfxDrawMethod.drawScene == GFX_DRAW_SCENE_STANDARD )
  {
    PIXBeginNamedEvent(-1, "Sun Shadow");
    if ( dynamicShadowType == SHADOW_MAP )
    {
      if ( Com_BitCheckAssert(frontEndDataOut->shadowableLightHasShadowMap, rgp.world->sunPrimaryLightIndex, 32) )
      {
        PIXBeginNamedEvent(-1, "scene ent surfaces");
        R_AddAllSceneEntSurfacesSunShadow();
        if ( GetCurrentThreadId() == g_DXDeviceThread )
          D3DPERF_EndEvent();
        R_SortAllStaticModelSurfacesSunShadow();
        R_SortAllCodeMeshSurfacesSunShadow();
        R_GenerateSortedSunShadowDrawSurfs(viewInfo);
      }
      else
      {
        for ( partitionIndex = 0; partitionIndex < 2; ++partitionIndex )
          R_InitDrawSurfListInfo(&viewInfo->drawList[partitionIndex + 8]);
      }
      R_GenerateAllSortedSpotShadowDrawSurfs(viewInfo, viewInfoIndex);
    }
    if ( GetCurrentThreadId() == g_DXDeviceThread )
      D3DPERF_EndEvent();
  }
  R_SetAllStaticModelLighting();
  smodelClient.localClientNum = viewInfo->localClientNum;
  smodelClient.visBitsCamera = scene.dynSModelVisBitsCamera;
  smodelClient.visBitsSunShadow = scene.dynSModelVisBitsSunShadow;
  if ( (int)rgp.world->dpvs.dynamicSModelCount > 2048 )
    dynamicSModelCount = 2048;
  else
    dynamicSModelCount = rgp.world->dpvs.dynamicSModelCount;
  smodelClient.numDynSModel = dynamicSModelCount;
  smodelClient.numDynSModelInMap = rgp.world->dpvs.dynamicSModelCount;
  R_DynSModelBuildClientView(&smodelClient, viewInfo->dynSModelView, &frontEndDataOut->dynSModelState);
  PIXBeginNamedEvent(-1, "wait for r_add_sceneent");
  Sys_AssistAndWaitWorkerCmdInternal(&r_model_lightingWorkerCmd);
  Sys_AssistAndWaitWorkerCmdInternal(&r_add_sceneentWorkerCmd);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  if ( usePreTess )
  {
    v11 = !extraCamActive || viewInfo->isMissileCamera;
    BG_EvalVehicleName((jpeg_decompress_struct *)v11);
  }
  litInfo = viewInfo->drawList;
  R_InitDrawSurfListInfo(viewInfo->drawList);
  litInfo->baseTechType = gfxDrawMethod.baseTechType;
  litInfo->viewInfo = viewInfo;
  litInfo->viewOrigin[0] = viewParmsDraw->origin[0];
  litInfo->viewOrigin[1] = viewParmsDraw->origin[1];
  litInfo->viewOrigin[2] = viewParmsDraw->origin[2];
  litInfo->viewOrigin[3] = viewParmsDraw->origin[3];
  litInfo->cameraView = 1;
  litInfo->shaderHDRmode = viewInfo->sceneComposition.opaqueShaderRemap;
  litPunchThruInfo = &viewInfo->drawList[2];
  R_InitDrawSurfListInfo(&viewInfo->drawList[2]);
  litPunchThruInfo->baseTechType = gfxDrawMethod.baseTechType;
  litPunchThruInfo->viewInfo = viewInfo;
  litPunchThruInfo->viewOrigin[0] = viewParmsDraw->origin[0];
  litPunchThruInfo->viewOrigin[1] = viewParmsDraw->origin[1];
  litPunchThruInfo->viewOrigin[2] = viewParmsDraw->origin[2];
  litPunchThruInfo->viewOrigin[3] = viewParmsDraw->origin[3];
  litPunchThruInfo->cameraView = 1;
  litPunchThruInfo->shaderHDRmode = viewInfo->sceneComposition.opaqueShaderRemap;
  litPostResolveInfo = &viewInfo->drawList[1];
  R_InitDrawSurfListInfo(&viewInfo->drawList[1]);
  litPostResolveInfo->baseTechType = gfxDrawMethod.baseTechType;
  litPostResolveInfo->viewInfo = viewInfo;
  litPostResolveInfo->viewOrigin[0] = viewParmsDraw->origin[0];
  litPostResolveInfo->viewOrigin[1] = viewParmsDraw->origin[1];
  litPostResolveInfo->viewOrigin[2] = viewParmsDraw->origin[2];
  litPostResolveInfo->viewOrigin[3] = viewParmsDraw->origin[3];
  litPostResolveInfo->cameraView = 1;
  litPostResolveInfo->shaderHDRmode = viewInfo->sceneComposition.opaqueShaderRemap;
  PIXBeginNamedEvent(-1, "water simulation render");
  if ( r_gfxopt_water_simulation->current.enabled && sceneParms->isRenderingFullScreen )
    R_WaterSimulationRender(viewParmsDraw->origin, frameTime, litInfo->viewInfoIndex);
  else
    R_WaterSimulationSwitchOff();
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  PIXBeginNamedEvent(-1, "set up litInfo");
  firstDrawSurfCount = frontEndDataOut->drawSurfCount;
  R_MergeAndEmitDrawSurfLists(0, 3u, frontEndDataOut);
  litInfo->drawSurfs = &frontEndDataOut->drawSurfs[firstDrawSurfCount];
  litInfo->drawSurfCount = frontEndDataOut->drawSurfCount - firstDrawSurfCount;
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  PIXBeginNamedEvent(-1, "split litInfo");
  R_SplitDrawSurfacesPrimarySortKey(viewInfo->drawList, &viewInfo->drawList[1], 16);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  R_DynSModelWaitWorker();
  if ( r_debugShowDynamicLights->current.enabled )
  {
    for ( j = 0; j < viewInfo->visibleLightCount; ++j )
      DrawLightDebug(viewInfo, &viewInfo->visibleLights[j], colorGreen);
  }
  pointLightCount = 0;
  if ( r_dlightLimit->current.integer && gfxDrawMethod.drawScene == GFX_DRAW_SCENE_STANDARD && !forMissileCamView )
  {
    PIXBeginNamedEvent(-1, "emit dlight surfs");
    pointLightCount = R_EmitPointLightPartitionSurfs(
                        viewInfo,
                        viewInfo->visibleLights,
                        viewInfo->visibleLightCount,
                        viewParmsDpvs->origin);
    if ( GetCurrentThreadId() == g_DXDeviceThread )
      D3DPERF_EndEvent();
  }
  viewInfo->pointLightCount = pointLightCount;
  if ( !forMissileCamView && !forStereoRightEyeView )
  {
    if ( !refdef->localClientNum )
      R_StreamUpdate(viewParmsDraw->origin);
    R_StreamUpdatePerClient(viewParmsDraw->origin);
  }
  if ( !viewInfo->localClientNum && !forMissileCamView && !forStereoRightEyeView )
    BG_EvalVehicleName(v7);
  if ( !forMissileCamView )
  {
    PIXBeginNamedEvent(-1, "DynEntCl_ProcessEntities");
    DynEntCl_ProcessEntities(viewInfo->localClientNum, refdef->time);
    if ( g_DXDeviceThread == GetCurrentThreadId() )
      D3DPERF_EndEvent();
  }
  PIXBeginNamedEvent(-1, "wait for fx_marks_draw");
  Sys_WaitWorkerCmdInternal(&fx_marks_drawWorkerCmd);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  if ( Sys_QueryD3DDeviceOKEvent() )
  {
    v9 = fx_marks && fx_marks->current.enabled;
    v16 = v9;
  }
  else
  {
    v16 = 0;
  }
  if ( v16 )
  {
    PIXBeginNamedEvent(-1, "Mark verts");
    if ( fx_marks_smodels->current.enabled )
      FX_GenerateMarkVertsForStaticModels(
        viewInfo->localClientNum,
        rgp.world->dpvs.smodelCount,
        rgp.world->dpvs.smodelVisData[0],
        viewInfo->visibleLights,
        viewInfo->visibleLightCount);
    if ( fx_marks_ents->current.enabled )
      R_GenerateMarkVertsForDynamicModels(viewInfo->visibleLights, viewInfo->visibleLightCount);
    if ( GetCurrentThreadId() == g_DXDeviceThread )
      D3DPERF_EndEvent();
  }
  R_FinishDecalAndEmissiveDrawSurfs(viewInfo, viewParmsDraw, frontEndDataOut, forMissileCamView);
  R_ShowCull(viewParmsDraw->origin);
  viewInfo->renderSeeThruDecals = CG_IsShowingZombieMap((bool)v7)
                               && FX_GetFrameTotalSeeThruDecalCount(viewInfo->localClientNum);
  BG_EvalVehicleName(v8);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
}

bool __cdecl R_GetAllowShadowMaps()
{
  return sm_enable->current.enabled && g_allowShadowMaps;
}

bool __cdecl R_DynamicShadowType()
{
  return R_GetAllowShadowMaps();
}

void __cdecl R_SetSceneComposition(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms, bool extraCam)
{
  viewInfo->sceneComposition.ui3d = 20;
  viewInfo->sceneComposition.seeThruDecal = 19;
  viewInfo->sceneComposition.frameBuffer = 2;
  viewInfo->sceneComposition.renderingMode = 0;
  viewInfo->sceneComposition.extraCam = 22;
  if ( extraCam )
  {
    viewInfo->sceneComposition.mainSceneMSAA = 22;
    viewInfo->sceneComposition.mainScene = 22;
  }
  else
  {
    viewInfo->sceneComposition.mainSceneMSAA = 3;
    viewInfo->sceneComposition.mainScene = 3;
  }
}

void __cdecl R_SetExposure(GfxViewInfo *viewInfo, const refdef_s *refdef)
{
  int integer; // [esp+Ch] [ebp-5Ch]

  viewInfo->exposureValue = refdef->exposureValue;
  if ( r_exposureTweak->current.enabled )
  {
    integer = r_exposureValue->current.integer;
    viewInfo->exposureValue.blackPoint[0] = *(float *)&FLOAT_0_0;
    viewInfo->exposureValue.blackPoint[1] = *(float *)&FLOAT_0_0;
    viewInfo->exposureValue.blackPoint[2] = *(float *)&FLOAT_0_0;
    LODWORD(viewInfo->exposureValue.blackPoint[3]) = integer;
    viewInfo->exposureValue.whitePoint[0] = FLOAT_8_0;
    viewInfo->exposureValue.whitePoint[1] = FLOAT_8_0;
    viewInfo->exposureValue.whitePoint[2] = FLOAT_8_0;
    viewInfo->exposureValue.whitePoint[3] = FLOAT_1_0;
  }
  else
  {
    Dvar_SetFloatIfChanged((dvar_s *)r_exposureValue, viewInfo->exposureValue.blackPoint[3]);
  }
  viewInfo->postEmissiveBrightening = refdef->postEmissiveBrightening;
  viewInfo->exposureRemap.remapMul[0] = (float)(1.0
                                              / (float)(viewInfo->exposureValue.whitePoint[0]
                                                      - viewInfo->exposureValue.blackPoint[0]))
                                      * viewInfo->exposureValue.blackPoint[3];
  viewInfo->exposureRemap.remapMul[1] = (float)(1.0
                                              / (float)(viewInfo->exposureValue.whitePoint[1]
                                                      - viewInfo->exposureValue.blackPoint[1]))
                                      * viewInfo->exposureValue.blackPoint[3];
  viewInfo->exposureRemap.remapMul[2] = (float)(1.0
                                              / (float)(viewInfo->exposureValue.whitePoint[2]
                                                      - viewInfo->exposureValue.blackPoint[2]))
                                      * viewInfo->exposureValue.blackPoint[3];
  viewInfo->exposureRemap.remapAdd[0] = COERCE_FLOAT(LODWORD(viewInfo->exposureValue.blackPoint[0]) ^ _mask__NegFloat_)
                                      * (float)(1.0
                                              / (float)(viewInfo->exposureValue.whitePoint[0]
                                                      - viewInfo->exposureValue.blackPoint[0]));
  viewInfo->exposureRemap.remapAdd[1] = COERCE_FLOAT(LODWORD(viewInfo->exposureValue.blackPoint[1]) ^ _mask__NegFloat_)
                                      * (float)(1.0
                                              / (float)(viewInfo->exposureValue.whitePoint[1]
                                                      - viewInfo->exposureValue.blackPoint[1]));
  viewInfo->exposureRemap.remapAdd[2] = COERCE_FLOAT(LODWORD(viewInfo->exposureValue.blackPoint[2]) ^ _mask__NegFloat_)
                                      * (float)(1.0
                                              / (float)(viewInfo->exposureValue.whitePoint[2]
                                                      - viewInfo->exposureValue.blackPoint[2]));
  viewInfo->exposureRemap.remapMul[0] = viewInfo->exposureValue.blackPoint[3] / 8.0;
  viewInfo->exposureRemap.remapMul[1] = viewInfo->exposureValue.blackPoint[3] / 8.0;
  viewInfo->exposureRemap.remapMul[2] = viewInfo->exposureValue.blackPoint[3] / 8.0;
  viewInfo->exposureRemap.remapAdd[0] = *(float *)&FLOAT_0_0;
  viewInfo->exposureRemap.remapAdd[1] = *(float *)&FLOAT_0_0;
  viewInfo->exposureRemap.remapAdd[2] = *(float *)&FLOAT_0_0;
}

void __cdecl R_SetDepthOfField(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms)
{
  Font_s *v2; // [esp+24h] [ebp-21Ch]
  Font_s *font; // [esp+28h] [ebp-218h]
  char buff[516]; // [esp+30h] [ebp-210h] BYREF
  float x; // [esp+238h] [ebp-8h]
  float y; // [esp+23Ch] [ebp-4h]

  if ( r_dof_tweak->current.enabled )
  {
    if ( r_dof_nearBlur->current.value < 4.0
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
            3186,
            0,
            "%s\n\t(r_dof_nearBlur->current.value) = %g",
            "(r_dof_nearBlur->current.value >= 4.0f)",
            r_dof_nearBlur->current.value) )
    {
      __debugbreak();
    }
    viewInfo->dof.viewModelStart = r_dof_viewModelStart->current.value;
    viewInfo->dof.viewModelEnd = r_dof_viewModelEnd->current.value;
    viewInfo->dof.nearStart = r_dof_nearStart->current.value;
    viewInfo->dof.nearEnd = r_dof_nearEnd->current.value;
    viewInfo->dof.farStart = r_dof_farStart->current.value;
    viewInfo->dof.farEnd = r_dof_farEnd->current.value;
    viewInfo->dof.nearBlur = r_dof_nearBlur->current.value;
    viewInfo->dof.farBlur = r_dof_farBlur->current.value;
  }
  else if ( r_dof_enable->current.enabled )
  {
    memcpy(&viewInfo->dof, &sceneParms->dof, sizeof(viewInfo->dof));
  }
  else
  {
    viewInfo->dof.viewModelStart = 0.0;
    viewInfo->dof.viewModelEnd = 0.0;
    viewInfo->dof.nearStart = 0.0;
    viewInfo->dof.nearEnd = 0.0;
    viewInfo->dof.farStart = 0.0;
    viewInfo->dof.farEnd = 0.0;
    viewInfo->dof.nearBlur = 0.0;
    viewInfo->dof.farBlur = 0.0;
  }
  if ( r_dof_showdebug->current.enabled )
  {
    font = R_RegisterFont("fonts/consoleFont", 1);
    x = FLOAT_60_0;
    y = FLOAT_60_0;
    _snprintf(buff, 0x200u, "DOF.viewModelStart = %.2f", viewInfo->dof.viewModelStart);
    R_AddCmdDrawText(buff, 0x7FFFFFFF, font, x, y, 1.5, 2.0, 0.0, colorRed, 0);
    y = y + 25.0;
    _snprintf(buff, 0x200u, "DOF.viewModelEnd   = %.2f", viewInfo->dof.viewModelEnd);
    R_AddCmdDrawText(buff, 0x7FFFFFFF, font, x, y, 1.5, 2.0, 0.0, colorRed, 0);
    y = y + 25.0;
    _snprintf(buff, 0x200u, "DOF.nearStart      = %.2f", viewInfo->dof.nearStart);
    R_AddCmdDrawText(buff, 0x7FFFFFFF, font, x, y, 1.5, 2.0, 0.0, colorRed, 0);
    y = y + 25.0;
    _snprintf(buff, 0x200u, "DOF.nearEnd        = %.2f", viewInfo->dof.nearEnd);
    R_AddCmdDrawText(buff, 0x7FFFFFFF, font, x, y, 1.5, 2.0, 0.0, colorRed, 0);
    y = y + 25.0;
    _snprintf(buff, 0x200u, "DOF.farStart       = %.2f", viewInfo->dof.farStart);
    R_AddCmdDrawText(buff, 0x7FFFFFFF, font, x, y, 1.5, 2.0, 0.0, colorRed, 0);
    y = y + 25.0;
    _snprintf(buff, 0x200u, "DOF.farEnd         = %.2f", viewInfo->dof.farEnd);
    R_AddCmdDrawText(buff, 0x7FFFFFFF, font, x, y, 1.5, 2.0, 0.0, colorRed, 0);
    y = y + 25.0;
    _snprintf(buff, 0x200u, "DOF.nearBlur       = %.2f", viewInfo->dof.nearBlur);
    R_AddCmdDrawText(buff, 0x7FFFFFFF, font, x, y, 1.5, 2.0, 0.0, colorRed, 0);
    y = y + 25.0;
    _snprintf(buff, 0x200u, "DOF.farBlur        = %.2f", viewInfo->dof.farBlur);
    R_AddCmdDrawText(buff, 0x7FFFFFFF, font, x, y, 1.5, 2.0, 0.0, colorRed, 0);
    y = y + 25.0;
    y = y + 25.0;
    _snprintf(buff, 0x200u, "r_dof_tweak        = %d", r_dof_tweak->current.color[0] != 0);
    R_AddCmdDrawText(buff, 0x7FFFFFFF, font, x, y, 1.5, 2.0, 0.0, colorRed, 0);
    y = y + 25.0;
    _snprintf(buff, 0x200u, "r_dof_enable       = %d", r_dof_enable->current.color[0] != 0);
    R_AddCmdDrawText(buff, 0x7FFFFFFF, font, x, y, 1.5, 2.0, 0.0, colorRed, 0);
    y = y + 25.0;
  }
  if ( RB_UsingDepthOfFieldFX(viewInfo) )
  {
    if ( viewInfo->isMissileCamera
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
            3263,
            0,
            "%s",
            "!viewInfo->isMissileCamera") )
    {
      __debugbreak();
    }
    if ( !dx.supportsIntZ )
    {
      if ( !stru_B50E8BC.surface.color )
        Com_Error(
          ERR_FATAL,
          "Depth of field used (enabled via r_dof_enable or r_dof_tweak) with no float-z buffer (r_floatz wasn't enabled "
          "when the renderer was started.)\n");
      viewInfo->needsFloatZ = 1;
    }
    if ( com_statmon->current.enabled )
    {
      v2 = R_RegisterFont("fonts/consoleFont", 1);
      if ( cg_draw2D->current.enabled )
        R_AddCmdDrawText("DOF", 0x7FFFFFFF, v2, 0.0, 320.0, 1.5, 2.0, 0.0, colorRedFaded, 0);
    }
  }
}

void __cdecl R_SetDoubleVision(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms)
{
  Font_s *font; // [esp+24h] [ebp-4h]

  memcpy(&viewInfo->doubleVision, &sceneParms->doubleVision, sizeof(viewInfo->doubleVision));
  if ( R_UsingDoubleVision(viewInfo) && com_statmon->current.enabled )
  {
    font = R_RegisterFont("fonts/consoleFont", 1);
    if ( cg_draw2D->current.enabled )
      R_AddCmdDrawText("DOUBLEVISION", 0x7FFFFFFF, font, 0.0, 320.0, 1.5, 2.0, 0.0, colorRedFaded, 0);
  }
}

void __cdecl R_SetWaterSheetingFx(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms)
{
  memcpy(&viewInfo->waterSheetingFx, &sceneParms->waterSheetingFx, sizeof(viewInfo->waterSheetingFx));
  if ( r_waterSheetingFX_enable->current.enabled )
  {
    viewInfo->waterSheetingFx.blurRadius = r_waterSheetingFX_radius->current.value;
    viewInfo->waterSheetingFx.distortionMagnitude = r_waterSheetingFX_magnitude->current.value;
    viewInfo->waterSheetingFx.distortionScale[0] = r_waterSheetingFX_distortionScaleFactor->current.value;
    viewInfo->waterSheetingFx.distortionScale[1] = r_waterSheetingFX_distortionScaleFactor->current.vector[1];
    viewInfo->waterSheetingFx.duration = 0;
    viewInfo->waterSheetingFx.frameRate = *(float *)&FLOAT_0_0;
  }
}

void __cdecl R_SetFlameFx(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms)
{
  memcpy(&viewInfo->flameFx, &sceneParms->flameFx, sizeof(viewInfo->flameFx));
  if ( r_flameFX_enable->current.enabled )
  {
    viewInfo->flameFx.distortionMagnitude = r_flameFX_magnitude->current.value;
    viewInfo->flameFx.distortionScale[0] = r_flameFX_distortionScaleFactor->current.value;
    viewInfo->flameFx.distortionScale[1] = r_flameFX_distortionScaleFactor->current.vector[1];
    viewInfo->flameFx.frameRate = (float)r_flameFX_FPS->current.integer;
  }
}

void __cdecl R_SetElectrifiedFx(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms)
{
  memcpy(&viewInfo->electrifiedFx, &sceneParms->electrifiedFx, sizeof(viewInfo->electrifiedFx));
  if ( r_flameFX_enable->current.enabled )
  {
    viewInfo->electrifiedFx.distortionMagnitude = r_flameFX_magnitude->current.value;
    viewInfo->electrifiedFx.distortionScale[0] = r_flameFX_distortionScaleFactor->current.value;
    viewInfo->electrifiedFx.distortionScale[1] = r_flameFX_distortionScaleFactor->current.vector[1];
    viewInfo->electrifiedFx.frameRate = (float)r_flameFX_FPS->current.integer;
  }
}

void __cdecl R_SetTransportedFx(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms)
{
  memcpy(&viewInfo->transportedFx, &sceneParms->transportedFx, sizeof(viewInfo->transportedFx));
  if ( r_flameFX_enable->current.enabled )
  {
    viewInfo->transportedFx.distortionMagnitude = r_flameFX_magnitude->current.value;
    viewInfo->transportedFx.distortionScale[0] = r_flameFX_distortionScaleFactor->current.value;
    viewInfo->transportedFx.distortionScale[1] = r_flameFX_distortionScaleFactor->current.vector[1];
    viewInfo->transportedFx.frameRate = (float)r_flameFX_FPS->current.integer;
  }
}

void __cdecl R_SetReviveFx(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms)
{
  DvarValue *v2; // [esp+Ch] [ebp-14h]
  DvarValue *v3; // [esp+14h] [ebp-Ch]
  DvarValue *p_current; // [esp+1Ch] [ebp-4h]

  if ( r_reviveFX_debug->current.enabled )
  {
    viewInfo->reviveFx.enabled = 1;
    viewInfo->reviveFx.reviveEdgeColorTemp = r_reviveFX_edgeColorTemp->current.value;
    viewInfo->reviveFx.reviveEdgeSaturation = r_reviveFX_edgeSaturation->current.value;
    p_current = &r_reviveFX_edgeScale->current;
    viewInfo->reviveFx.reviveEdgeScale[0] = r_reviveFX_edgeScale->current.value;
    viewInfo->reviveFx.reviveEdgeScale[1] = p_current->vector[1];
    viewInfo->reviveFx.reviveEdgeScale[2] = p_current->vector[2];
    v3 = &r_reviveFX_edgeContrast->current;
    viewInfo->reviveFx.reviveEdgeContrast[0] = r_reviveFX_edgeContrast->current.value;
    viewInfo->reviveFx.reviveEdgeContrast[1] = v3->vector[1];
    viewInfo->reviveFx.reviveEdgeContrast[2] = v3->vector[2];
    v2 = &r_reviveFX_edgeOffset->current;
    viewInfo->reviveFx.reviveEdgeOffset[0] = r_reviveFX_edgeOffset->current.value;
    viewInfo->reviveFx.reviveEdgeOffset[1] = v2->vector[1];
    viewInfo->reviveFx.reviveEdgeOffset[2] = v2->vector[2];
    viewInfo->reviveFx.reviveEdgeMaskAdjust = r_reviveFX_edgeMaskAdjust->current.value;
    viewInfo->reviveFx.reviveEdgeAmount = r_reviveFX_edgeAmount->current.value;
  }
  else
  {
    memcpy(&viewInfo->reviveFx, &sceneParms->reviveFx, sizeof(viewInfo->reviveFx));
  }
}

void __cdecl R_SetGenericFilter(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms)
{
  float *dir; // [esp+4h] [ebp-8h]

  memcpy(
    (unsigned __int8 *)&viewInfo->genericFilter,
    (unsigned __int8 *)&sceneParms->genericFilter,
    sizeof(viewInfo->genericFilter));
  dir = frontEndDataOut->sunLight.dir;
  viewInfo->genericFilter.sunPosition[0] = (float)(10000.0 * frontEndDataOut->sunLight.dir[0])
                                         + viewInfo->cullViewInfo.viewParms.origin[0];
  viewInfo->genericFilter.sunPosition[1] = (float)(10000.0 * dir[1]) + viewInfo->cullViewInfo.viewParms.origin[1];
  viewInfo->genericFilter.sunPosition[2] = (float)(10000.0 * dir[2]) + viewInfo->cullViewInfo.viewParms.origin[2];
}

void __cdecl R_SetPoisonFx(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms)
{
  float curAmount; // edx
  double v3; // xmm0_8
  long double POISON_BLUR_SLOW; // [esp+0h] [ebp-1Ch]
  long double POISON_BLUR_SLOWa; // [esp+0h] [ebp-1Ch]
  float blend; // [esp+4h] [ebp-18h]
  float blenda; // [esp+4h] [ebp-18h]
  float blur1; // [esp+8h] [ebp-14h]
  float range; // [esp+Ch] [ebp-10h]
  float base; // [esp+18h] [ebp-4h]

  curAmount = sceneParms->poisonFx.curAmount;
  viewInfo->poisonFx.curAmountTarget = sceneParms->poisonFx.curAmountTarget;
  viewInfo->poisonFx.curAmount = curAmount;
  if ( r_poisonFX_debug_enable->current.enabled )
    viewInfo->poisonFx.curAmount = r_poisonFX_debug_amount->current.value;
  if ( viewInfo->poisonFx.curAmount > 0.0 )
  {
    blend = r_poisonFX_pulse->current.value - 1.0;
    if ( blend < 0.0 )
      blend = *(float *)&FLOAT_0_0;
    *((float *)&POISON_BLUR_SLOW + 1) = (float)((float)((float)(-2.0 * blend) + 3.0) * blend) * blend;
    *(float *)&POISON_BLUR_SLOW = FLOAT_6_0;
    blur1 = viewInfo->sceneDef.floatTime / 3.0;
    base = r_poisonFX_blurMin->current.value;
    range = r_poisonFX_blurMax->current.value - base;
    v3 = (float)((float)((float)(viewInfo->sceneDef.floatTime / 6.0) * 3.1415927) * 2.5);
    __libm_sse2_sin(POISON_BLUR_SLOW);
    *(float *)&v3 = v3;
    __libm_sse2_sin(POISON_BLUR_SLOWa);
    viewInfo->blurRadius = (float)((float)(1.0 - blenda)
                                 * (float)((float)((float)((float)(*(float *)&v3 * 0.5) + 0.5) * range) + base))
                         + (float)(blenda
                                 * (float)((float)((float)((float)((float)((float)(blur1 * 3.1415927) * 2.5) * 0.5) + 0.5)
                                                 * range)
                                         + base));
    viewInfo->blurRadius = viewInfo->blurRadius * viewInfo->poisonFx.curAmount;
  }
}

void __cdecl R_SetFilmInfo(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms)
{
  DvarValue *v2; // [esp+Ch] [ebp-3Ch]
  DvarValue *v3; // [esp+14h] [ebp-34h]
  int v4; // [esp+1Ch] [ebp-2Ch]
  DvarValue *v5; // [esp+24h] [ebp-24h]
  DvarValue *v6; // [esp+2Ch] [ebp-1Ch]
  DvarValue *v7; // [esp+34h] [ebp-14h]
  DvarValue *v8; // [esp+3Ch] [ebp-Ch]
  DvarValue *p_current; // [esp+44h] [ebp-4h]

  if ( !viewInfo
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 3458, 0, "%s", "viewInfo") )
  {
    __debugbreak();
  }
  if ( !sceneParms
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 3459, 0, "%s", "sceneParms") )
  {
    __debugbreak();
  }
  if ( r_filmUseTweaks->current.enabled )
  {
    viewInfo->film.enabled = r_filmTweakEnable->current.enabled;
    p_current = &r_filmTweakHue->current;
    viewInfo->film.filmHue[0] = r_filmTweakHue->current.value;
    viewInfo->film.filmHue[1] = p_current->vector[1];
    viewInfo->film.filmHue[2] = p_current->vector[2];
    v8 = &r_filmTweakSaturation->current;
    viewInfo->film.filmSaturation[0] = r_filmTweakSaturation->current.value;
    viewInfo->film.filmSaturation[1] = v8->vector[1];
    viewInfo->film.filmSaturation[2] = v8->vector[2];
    v7 = &r_filmTweakColorTemp->current;
    viewInfo->film.filmColorTemp[0] = r_filmTweakColorTemp->current.value;
    viewInfo->film.filmColorTemp[1] = v7->vector[1];
    viewInfo->film.filmColorTemp[2] = v7->vector[2];
    v6 = &r_filmTweakDarkTint->current;
    viewInfo->film.filmDarkTint[0] = r_filmTweakDarkTint->current.value;
    viewInfo->film.filmDarkTint[1] = v6->vector[1];
    viewInfo->film.filmDarkTint[2] = v6->vector[2];
    v5 = &r_filmTweakMidTint->current;
    viewInfo->film.filmMidTint[0] = r_filmTweakMidTint->current.value;
    viewInfo->film.filmMidTint[1] = v5->vector[1];
    viewInfo->film.filmMidTint[2] = v5->vector[2];
    v4 = LODWORD(r_lightTweakSunDirection.vector[2]) + 24;
    viewInfo->film.filmLightTint[0] = *(float *)(LODWORD(r_lightTweakSunDirection.vector[2]) + 24);
    viewInfo->film.filmLightTint[1] = *(float *)(v4 + 4);
    viewInfo->film.filmLightTint[2] = *(float *)(v4 + 8);
    v3 = &r_filmTweakContrast->current;
    viewInfo->film.filmContrast[0] = r_filmTweakContrast->current.value;
    viewInfo->film.filmContrast[1] = v3->vector[1];
    viewInfo->film.filmContrast[2] = v3->vector[2];
    v2 = &r_filmTweakBleach->current;
    viewInfo->film.filmBleach[0] = r_filmTweakBleach->current.value;
    viewInfo->film.filmBleach[1] = v2->vector[1];
    viewInfo->film.filmBleach[2] = v2->vector[2];
    viewInfo->film.filmMidStart = r_filmTweakMidStart->current.value;
    viewInfo->film.filmMidEnd = r_filmTweakMidEnd->current.value;
    viewInfo->film.filmDarkFeather = r_filmTweakDarkFeather->current.value;
    viewInfo->film.filmLightFeather = r_filmTweakLightFeather->current.value;
    viewInfo->film.sCurveEnable = (float)r_sCurveEnable->current.integer;
    viewInfo->film.sCurveShoulderStrength = r_sCurveShoulderStrength->current.value;
    viewInfo->film.sCurveLinearStrength = r_sCurveLinearStrength->current.value;
    viewInfo->film.sCurveLinearAngle = r_sCurveLinearAngle->current.value;
    viewInfo->film.sCurveToeStrength = r_sCurveToeStrength->current.value;
    viewInfo->film.sCurveToeNumerator = r_sCurveToeNumerator->current.value;
    viewInfo->film.sCurveToeDenominator = r_sCurveToeDenominator->current.value;
  }
  else
  {
    memcpy(&viewInfo->film, &sceneParms->film, sizeof(viewInfo->film));
  }
  if ( r_filmTweakLut->current.integer >= 0 )
    viewInfo->film.filmLut = (float)r_filmTweakLut->current.integer;
}

void __cdecl R_SetBloomInfo(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms)
{
  float v2; // [esp+8h] [ebp-110h]
  float v3; // [esp+Ch] [ebp-10Ch]
  float v4; // [esp+10h] [ebp-108h]
  float v5; // [esp+14h] [ebp-104h]
  DvarValue *v6; // [esp+1Ch] [ebp-FCh]
  DvarValue *v7; // [esp+2Ch] [ebp-ECh]
  DvarValue *v8; // [esp+34h] [ebp-E4h]
  DvarValue *v9; // [esp+4Ch] [ebp-CCh]
  DvarValue *v10; // [esp+5Ch] [ebp-BCh]
  DvarValue *p_current; // [esp+64h] [ebp-B4h]
  float tx; // [esp+F8h] [ebp-20h]
  float z; // [esp+FCh] [ebp-1Ch]
  float tw; // [esp+104h] [ebp-14h]
  float ty; // [esp+10Ch] [ebp-Ch]
  float x; // [esp+110h] [ebp-8h]
  float xa; // [esp+110h] [ebp-8h]
  float y; // [esp+114h] [ebp-4h]
  float ya; // [esp+114h] [ebp-4h]

  if ( r_bloomTweaks->current.enabled )
  {
    *(DvarValue *)viewInfo->bloom.bloomTintWeights = r_bloomTintWeights->current;
    *(DvarValue *)viewInfo->bloom.bloomColorScale = r_bloomColorScale->current;
    *(DvarValue *)viewInfo->bloom.bloomTintScale = r_bloomTintScale->current;
    *(DvarValue *)viewInfo->bloom.bloomCurveBreakpoint = r_bloomCurveBreakpoint->current;
    *(DvarValue *)viewInfo->bloom.bloomCurveLoBlack = r_bloomCurveLoBlack->current;
    *(DvarValue *)viewInfo->bloom.bloomCurveLoGamma = r_bloomCurveLoGamma->current;
    *(DvarValue *)viewInfo->bloom.bloomCurveLoWhite = r_bloomCurveLoWhite->current;
    *(DvarValue *)viewInfo->bloom.bloomCurveLoRemapBlack = r_bloomCurveLoRemapBlack->current;
    *(DvarValue *)viewInfo->bloom.bloomCurveLoRemapWhite = r_bloomCurveLoRemapWhite->current;
    *(DvarValue *)viewInfo->bloom.bloomCurveHiBlack = r_bloomCurveHiBlack->current;
    *(DvarValue *)viewInfo->bloom.bloomCurveHiGamma = r_bloomCurveHiGamma->current;
    *(DvarValue *)viewInfo->bloom.bloomCurveHiWhite = r_bloomCurveHiWhite->current;
    *(DvarValue *)viewInfo->bloom.bloomCurveHiRemapBlack = r_bloomCurveHiRemapBlack->current;
    *(DvarValue *)viewInfo->bloom.bloomCurveHiRemapWhite = r_bloomCurveHiRemapWhite->current;
    *(DvarValue *)viewInfo->bloom.bloomExpansionControl = r_bloomExpansionControl->current;
    *(DvarValue *)viewInfo->bloom.bloomExpansionWeights = r_bloomExpansionWeights->current;
    viewInfo->bloom.bloomBlurRadius = r_bloomBlurRadius->current.value;
    viewInfo->bloom.bloomExpansionSource = r_bloomExpansionSource->current.integer;
    viewInfo->bloom.bloomPersistence = r_bloomPersistence->current.value;
    *(DvarValue *)viewInfo->bloom.bloomStreakXLevels0 = r_bloomStreakXLevels0->current;
    *(DvarValue *)viewInfo->bloom.bloomStreakXLevels1 = r_bloomStreakXLevels1->current;
    p_current = &r_bloomStreakXInnerTint->current;
    viewInfo->bloom.bloomStreakXInnerTint[0] = r_bloomStreakXInnerTint->current.value;
    viewInfo->bloom.bloomStreakXInnerTint[1] = p_current->vector[1];
    viewInfo->bloom.bloomStreakXInnerTint[2] = p_current->vector[2];
    v10 = &r_bloomStreakXOuterTint->current;
    viewInfo->bloom.bloomStreakXOuterTint[0] = r_bloomStreakXOuterTint->current.value;
    viewInfo->bloom.bloomStreakXOuterTint[1] = v10->vector[1];
    viewInfo->bloom.bloomStreakXOuterTint[2] = v10->vector[2];
    *(DvarValue *)viewInfo->bloom.bloomStreakXTintControl = r_bloomStreakXTintControl->current;
    v9 = &r_bloomStreakXTint->current;
    viewInfo->bloom.bloomStreakXTint[0] = r_bloomStreakXTint->current.value;
    viewInfo->bloom.bloomStreakXTint[1] = v9->vector[1];
    viewInfo->bloom.bloomStreakXTint[2] = v9->vector[2];
    *(DvarValue *)viewInfo->bloom.bloomStreakYLevels0 = r_bloomStreakYLevels0->current;
    *(DvarValue *)viewInfo->bloom.bloomStreakYLevels1 = r_bloomStreakYLevels1->current;
    v8 = &r_bloomStreakYInnerTint->current;
    viewInfo->bloom.bloomStreakYInnerTint[0] = r_bloomStreakYInnerTint->current.value;
    viewInfo->bloom.bloomStreakYInnerTint[1] = v8->vector[1];
    viewInfo->bloom.bloomStreakYInnerTint[2] = v8->vector[2];
    v7 = &r_bloomStreakYOuterTint->current;
    viewInfo->bloom.bloomStreakYOuterTint[0] = r_bloomStreakYOuterTint->current.value;
    viewInfo->bloom.bloomStreakYOuterTint[1] = v7->vector[1];
    viewInfo->bloom.bloomStreakYOuterTint[2] = v7->vector[2];
    *(DvarValue *)viewInfo->bloom.bloomStreakYTintControl = *(DvarValue *)(LODWORD(r_lightTweakSunDirection.vector[3])
                                                                         + 24);
    v6 = &r_bloomStreakYTint->current;
    viewInfo->bloom.bloomStreakYTint[0] = r_bloomStreakYTint->current.value;
    viewInfo->bloom.bloomStreakYTint[1] = v6->vector[1];
    viewInfo->bloom.bloomStreakYTint[2] = v6->vector[2];
  }
  else
  {
    memcpy(&viewInfo->bloom, &sceneParms->bloom, sizeof(viewInfo->bloom));
  }
  if ( viewInfo->bloom.bloomExpansionSource )
  {
    viewInfo->bloomExpansionXpos = viewInfo->bloom.bloomExpansionControl[2];
    viewInfo->bloomExpansionYpos = viewInfo->bloom.bloomExpansionControl[3];
  }
  else
  {
    x = rgp.world->sun.sunFxPosition[0];
    y = rgp.world->sun.sunFxPosition[1];
    z = rgp.world->sun.sunFxPosition[2];
    tw = (float)((float)(x * viewInfo->cullViewInfo.viewParms.viewProjectionMatrix.m[0][3])
               + (float)(y * viewInfo->cullViewInfo.viewParms.viewProjectionMatrix.m[1][3]))
       + (float)(z * viewInfo->cullViewInfo.viewParms.viewProjectionMatrix.m[2][3]);
    tx = (float)((float)((float)(x * viewInfo->cullViewInfo.viewParms.viewProjectionMatrix.m[0][0])
                       + (float)(y * viewInfo->cullViewInfo.viewParms.viewProjectionMatrix.m[1][0]))
               + (float)(z * viewInfo->cullViewInfo.viewParms.viewProjectionMatrix.m[2][0]))
       / tw;
    ty = (float)((float)((float)(x * viewInfo->cullViewInfo.viewParms.viewProjectionMatrix.m[0][1])
                       + (float)(y * viewInfo->cullViewInfo.viewParms.viewProjectionMatrix.m[1][1]))
               + (float)(z * viewInfo->cullViewInfo.viewParms.viewProjectionMatrix.m[2][1]))
       / tw;
    viewInfo->bloomExpansionXpos = (float)(tx * 0.5) + 0.5;
    viewInfo->bloomExpansionYpos = 0.5 - (float)(ty * 0.5);
    if ( (float)((float)((float)(1.0 / (float)(0.75 - 1.0)) * COERCE_FLOAT(LODWORD(tx) & _mask__AbsFloat_))
               - (float)((float)(1.0 / (float)(0.75 - 1.0)) * 1.0)) >= 0.0 )
      v5 = (float)((float)(1.0 / (float)(0.75 - 1.0)) * COERCE_FLOAT(LODWORD(tx) & _mask__AbsFloat_))
         - (float)((float)(1.0 / (float)(0.75 - 1.0)) * 1.0);
    else
      v5 = *(float *)&FLOAT_0_0;
    if ( v5 <= 1.0 )
      v4 = v5;
    else
      v4 = FLOAT_1_0;
    if ( (float)((float)((float)(1.0 / (float)(0.75 - 1.0)) * COERCE_FLOAT(LODWORD(ty) & _mask__AbsFloat_))
               - (float)((float)(1.0 / (float)(0.75 - 1.0)) * 1.0)) >= 0.0 )
      v3 = (float)((float)(1.0 / (float)(0.75 - 1.0)) * COERCE_FLOAT(LODWORD(ty) & _mask__AbsFloat_))
         - (float)((float)(1.0 / (float)(0.75 - 1.0)) * 1.0);
    else
      v3 = *(float *)&FLOAT_0_0;
    if ( v3 <= 1.0 )
      v2 = v3;
    else
      v2 = FLOAT_1_0;
    xa = (float)(v4 * v4) * (float)(3.0 - (float)(2.0 * v4));
    ya = (float)(v2 * v2) * (float)(3.0 - (float)(2.0 * v2));
    viewInfo->bloom.bloomExpansionWeights[0] = (float)(xa * ya) * viewInfo->bloom.bloomExpansionWeights[0];
    viewInfo->bloom.bloomExpansionWeights[1] = (float)(xa * ya) * viewInfo->bloom.bloomExpansionWeights[1];
    viewInfo->bloom.bloomExpansionWeights[2] = (float)(xa * ya) * viewInfo->bloom.bloomExpansionWeights[2];
    viewInfo->bloom.bloomExpansionWeights[3] = (float)(xa * ya) * viewInfo->bloom.bloomExpansionWeights[3];
  }
}

void __cdecl R_SetFullSceneViewMesh(int viewInfoIndex, GfxViewInfo *viewInfo)
{
  float width; // [esp+24h] [ebp-14h]
  float height; // [esp+28h] [ebp-10h]
  GfxQuadMeshData *quadMesh; // [esp+2Ch] [ebp-Ch]
  float x; // [esp+30h] [ebp-8h]
  float y; // [esp+34h] [ebp-4h]

  quadMesh = &gfxMeshGlob.fullSceneViewMesh[viewInfoIndex];
  viewInfo->fullSceneViewMesh = quadMesh;
  x = (float)viewInfo->cullViewInfo.sceneViewport.x;
  y = (float)viewInfo->cullViewInfo.sceneViewport.y;
  width = (float)viewInfo->cullViewInfo.sceneViewport.width;
  height = (float)viewInfo->cullViewInfo.sceneViewport.height;
  if ( quadMesh->x != x || quadMesh->y != y || quadMesh->width != width || quadMesh->height != height )
  {
    R_SyncRenderThread();
    R_SetQuadMesh(quadMesh, x, y, width, height, 0.0, 0.0, 1.0, 1.0, 0xFFFFFFFF);
    if ( (quadMesh->x != x || quadMesh->y != y || quadMesh->width != width || quadMesh->height != height)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
            3648,
            1,
            "%s",
            "quadMesh->x == x && quadMesh->y == y && quadMesh->width == width && quadMesh->height == height") )
    {
      __debugbreak();
    }
  }
}

void __cdecl R_GenerateSortedSunShadowDrawSurfs(GfxViewInfo *viewInfo)
{
  R_MergeAndEmitSunShadowMapsSurfs(viewInfo);
  if ( !frontEndDataOut
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 3849, 0, "%s", "frontEndDataOut") )
  {
    __debugbreak();
  }
}

void __cdecl R_AddEmissiveSpotLight(GfxViewInfo *viewInfo)
{
  bool v1; // [esp+8h] [ebp-8h]

  frontEndDataOut->emissiveSpotLightCount = 0;
  if ( !viewInfo->isMissileCamera )
  {
    v1 = r_dlightLimit->current.integer && gfxDrawMethod.drawScene == GFX_DRAW_SCENE_STANDARD;
    if ( v1 && !R_CullDynamicSpotLightInCameraView() )
    {
      if ( scene.addedLightCount <= 0
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
              3899,
              0,
              "%s",
              "scene.addedLightCount > 0") )
      {
        __debugbreak();
      }
      if ( scene.isAddedLightCulled[0]
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
              3900,
              0,
              "%s",
              "!scene.isAddedLightCulled[0]") )
      {
        __debugbreak();
      }
      if ( scene.addedLight[0].type != 2
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
              3903,
              0,
              "%s",
              "spotLight->type == GFX_LIGHT_TYPE_SPOT") )
      {
        __debugbreak();
      }
      memcpy(&frontEndDataOut->emissiveSpotLight, scene.addedLight, sizeof(frontEndDataOut->emissiveSpotLight));
      frontEndDataOut->emissiveSpotLightIndex = 0;
      frontEndDataOut->emissiveSpotDrawSurfCount = 0;
      frontEndDataOut->emissiveSpotDrawSurfs = 0;
      frontEndDataOut->emissiveSpotLightCount = 1;
      if ( scene.addedLight[0].canUseShadowMap )
        R_AddDynamicShadowableLight(viewInfo, scene.addedLight);
    }
  }
}

int __cdecl R_GetVisibleDLights(GfxLight *visibleLights)
{
  int visibleLightCount; // [esp+14h] [ebp-4h]

  PIXBeginNamedEvent(-1, "R_GetVisibleDLights");
  visibleLightCount = 0;
  if ( r_dlightLimit->current.integer && gfxDrawMethod.drawScene == GFX_DRAW_SCENE_STANDARD )
  {
    R_CullDynamicPointLightsInCameraView();
    PIXBeginNamedEvent(-1, "lights partition");
    visibleLightCount = R_GetPointLightPartitions(visibleLights);
    if ( GetCurrentThreadId() == g_DXDeviceThread )
      D3DPERF_EndEvent();
  }
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  return visibleLightCount;
}

void __cdecl R_GetLightSurfs(const GfxLight *visibleLights, int visibleLightCount)
{
  if ( visibleLightCount )
  {
    PIXBeginNamedEvent(-1, "R_GetLightSurfs");
    PIXBeginNamedEvent(-1, "light surfs bsp");
    R_GetBspLightSurfs(visibleLights, visibleLightCount);
    if ( g_DXDeviceThread == GetCurrentThreadId() )
      D3DPERF_EndEvent();
    PIXBeginNamedEvent(-1, "light surfs smodel");
    R_GetStaticModelLightSurfs(visibleLights, visibleLightCount);
    if ( g_DXDeviceThread == GetCurrentThreadId() )
      D3DPERF_EndEvent();
    PIXBeginNamedEvent(-1, "light surfs scene ent");
    R_GetSceneEntLightSurfs(visibleLights, visibleLightCount);
    if ( GetCurrentThreadId() == g_DXDeviceThread )
      D3DPERF_EndEvent();
    if ( g_DXDeviceThread == GetCurrentThreadId() )
      D3DPERF_EndEvent();
  }
}

void __cdecl R_GetPointLightShadowSurfs(
        GfxViewInfo *viewInfo,
        GfxVisibleLight *visibleLights,
        const GfxLight *lights,
        int numLights)
{
  PIXBeginNamedEvent(-1, "R_GetPointLightShadowSurfs");
  if ( frontEndDataOut->emissiveSpotLightCount )
  {
    if ( frontEndDataOut->emissiveSpotLightCount != 1
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
            4032,
            0,
            "%s",
            "frontEndDataOut->emissiveSpotLightCount == 1") )
    {
      __debugbreak();
    }
    if ( frontEndDataOut->emissiveSpotLightIndex >= 4
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
            4033,
            0,
            "frontEndDataOut->emissiveSpotLightIndex doesn't index MAX_VISIBLE_DLIGHTS\n\t%i not in [0, %i)",
            frontEndDataOut->emissiveSpotLightIndex,
            4) )
    {
      __debugbreak();
    }
    if ( frontEndDataOut->emissiveSpotLightIndex
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
            4035,
            0,
            "%s",
            "frontEndDataOut->emissiveSpotLightIndex == 0") )
    {
      __debugbreak();
    }
    if ( numLights <= 0
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 4037, 0, "%s", "numLights > 0") )
    {
      __debugbreak();
    }
    if ( lights->type != 2
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
            4038,
            0,
            "%s",
            "lights[0].type == GFX_LIGHT_TYPE_SPOT") )
    {
      __debugbreak();
    }
    frontEndDataOut->emissiveSpotDrawSurfCount = visibleLights->drawSurfCount;
    frontEndDataOut->emissiveSpotDrawSurfs = visibleLights->drawSurfs;
  }
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

void __cdecl R_SetSunShadowConstants(GfxCmdBufInput *input, const GfxSunShadowProjection *sunProj)
{
  R_SetInputCodeConstantFromVec4(input, 0x2Fu, sunProj->switchPartition);
  R_SetInputCodeConstantFromVec4(input, 0x30u, sunProj->shadowmapScale);
}

// local variable allocation has failed, the output may be wrong!
void  R_SetHeroLighting(float a1@<ebp>, GfxCmdBufInput *input, GfxViewInfo *viewInfo)
{
  _BYTE x[128]; // [esp+10h] [ebp-17Ch] OVERLAPPED BYREF
  int integer; // [esp+9Ch] [ebp-F0h]
  float v5; // [esp+A0h] [ebp-ECh]
  float v6; // [esp+A4h] [ebp-E8h]
  _BYTE sc[212]; // [esp+A8h] [ebp-E4h] OVERLAPPED BYREF
  float s; // [esp+17Ch] [ebp-10h]
  float bwght; // [esp+180h] [ebp-Ch]
  float gwght; // [esp+184h] [ebp-8h]
  float retaddr; // [esp+18Ch] [ebp+0h]

  bwght = a1;
  gwght = retaddr;
  s = FLOAT_0_25;
  *(float *)&sc[208] = FLOAT_0_5;
  *(float *)&sc[204] = FLOAT_0_25;
  *(unsigned int *)&sc[200] = r_heroLightSaturation->current.integer;
  s = (float)(1.0 - *(float *)&sc[200]) * 0.25;
  *(float *)&sc[208] = (float)(1.0 - *(float *)&sc[200]) * 0.5;
  *(float *)&sc[204] = s;
  *(float *)&sc[136] = s + *(float *)&sc[200];
  *(float *)&sc[140] = s;
  *(float *)&sc[144] = s;
  *(unsigned int *)&sc[148] = *(unsigned int *)&FLOAT_0_0;
  *(float *)&sc[152] = *(float *)&sc[208];
  *(float *)&sc[156] = *(float *)&sc[208] + *(float *)&sc[200];
  *(float *)&sc[160] = *(float *)&sc[208];
  *(unsigned int *)&sc[164] = *(unsigned int *)&FLOAT_0_0;
  *(float *)&sc[168] = s;
  *(float *)&sc[172] = s;
  *(float *)&sc[176] = s + *(float *)&sc[200];
  memset(&sc[180], 0, 16);
  *(float *)&sc[196] = FLOAT_1_0;
  colorTempMatrix((float (*)[4])&sc[72], r_heroLightColorTemp->current.value);
  MatrixMultiply44((const float (*)[4])&sc[72], (const float (*)[4])&sc[136], (float (*)[4])&sc[8]);
  *(unsigned int *)&sc[4] = &r_heroLightScale->current;
  integer = r_heroLightScale->current.integer;
  v5 = r_heroLightScale->current.vector[1];
  v6 = r_heroLightScale->current.vector[2];
  *(float *)sc = r_heroLightScale->current.vector[3];
  R_SetInputCodeConstant(
    input,
    0xA9u,
    viewInfo->charPrimaryLightScale.diffuseScale,
    viewInfo->charPrimaryLightScale.specularScale,
    1.0,
    1.0);
  *(unsigned int *)&x[64] = integer;
  memset(&x[68], 0, 16);
  *(float *)&x[84] = v5;
  memset(&x[88], 0, 16);
  *(float *)&x[104] = v6;
  memset(&x[108], 0, 16);
  *(float *)&x[124] = FLOAT_1_0;
  MatrixMultiply44((const float (*)[4])&sc[8], (const float (*)[4])&x[64], (float (*)[4])x);
  R_SetInputCodeConstant(input, 0xA6u, *(float *)x, *(float *)&x[16], *(float *)&x[32], 0.0);
  R_SetInputCodeConstant(input, 0xA7u, *(float *)&x[4], *(float *)&x[20], *(float *)&x[36], 0.0);
  R_SetInputCodeConstant(input, 0xA8u, *(float *)&x[8], *(float *)&x[24], *(float *)&x[40], 0.0);
}

void __cdecl R_SetDLightsConstantsDefaults(GfxCmdBufInput *input)
{
  GfxImage *v1; // [esp+0h] [ebp-8h]
  GfxImage *whiteImage; // [esp+4h] [ebp-4h]

  whiteImage = rgp.whiteImage;
  if ( !input && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state.h", 1850, 0, "%s", "input") )
    __debugbreak();
  input->codeImages[17] = whiteImage;
  R_SetInputCodeImageSamplerState(input, 0x11u, 0x61u);
  v1 = rgp.whiteImage;
  if ( !input && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state.h", 1850, 0, "%s", "input") )
    __debugbreak();
  input->codeImages[7] = v1;
  R_SetInputCodeImageSamplerState(input, 7u, 0x62u);
}

void __cdecl R_SetInputCodeImageSamplerState(
        GfxCmdBufInput *input,
        unsigned int codeTexture,
        unsigned __int8 samplerState)
{
  const char *v3; // eax

  if ( codeTexture >= 0x2B
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state.h",
          1832,
          0,
          "codeTexture doesn't index TEXTURE_SRC_CODE_COUNT\n\t%i not in [0, %i)",
          codeTexture,
          43) )
  {
    __debugbreak();
  }
  if ( (samplerState & 7) == 0 )
  {
    v3 = va("R_SetCodeImageSamplerState %d %d", codeTexture, samplerState);
    if ( !Assert_MyHandler(
            "c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state.h",
            1833,
            0,
            "%s\n\t%s",
            "samplerState & SAMPLER_FILTER_MASK",
            v3) )
      __debugbreak();
  }
  input->codeImageSamplerStates[codeTexture] = samplerState;
}

// local variable allocation has failed, the output may be wrong!
void  R_SetDLightsConstants(
        float a1@<ebp>,
        GfxCmdBufInput *input,
        const GfxViewInfo *viewInfo,
        const GfxLight *visibleLights,
        int visibleLightCount)
{
  float v5; // xmm0_4
  float w; // xmm0_4
  _BYTE v7[76]; // [esp+20h] [ebp-2ACh] OVERLAPPED BYREF
  float v8; // [esp+6Ch] [ebp-260h]
  float y; // [esp+70h] [ebp-25Ch]
  float eAdd; // [esp+74h] [ebp-258h]
  float z; // [esp+78h] [ebp-254h]
  float eMul; // [esp+7Ch] [ebp-250h]
  float x; // [esp+80h] [ebp-24Ch]
  float sAdd; // [esp+84h] [ebp-248h]
  GfxImage *whiteImage; // [esp+88h] [ebp-244h]
  const GfxImage *sMul; // [esp+8Ch] [ebp-240h]
  _BYTE v17[80]; // [esp+90h] [ebp-23Ch] OVERLAPPED BYREF
  float v18; // [esp+E0h] [ebp-1ECh]
  float eyeOffset[4]; // [esp+E4h] [ebp-1E8h]
  float *v20; // [esp+F4h] [ebp-1D8h]
  float value; // [esp+F8h] [ebp-1D4h]
  float *v23; // [esp+100h] [ebp-1CCh]
  float *v24; // [esp+104h] [ebp-1C8h]
  unsigned int j; // [esp+108h] [ebp-1C4h]
  float *v26; // [esp+10Ch] [ebp-1C0h]
  _BYTE *v27; // [esp+110h] [ebp-1BCh]
  unsigned int sli; // [esp+114h] [ebp-1B8h]
  _BYTE *v29; // [esp+118h] [ebp-1B4h]
  float *v30; // [esp+11Ch] [ebp-1B0h]
  _BYTE *v31; // [esp+120h] [ebp-1ACh]
  float44 *p_projMatrix; // [esp+124h] [ebp-1A8h]
  float *v33; // [esp+128h] [ebp-1A4h]
  _BYTE *v34; // [esp+12Ch] [ebp-1A0h]
  float *v35; // [esp+130h] [ebp-19Ch]
  _BYTE *v36; // [esp+134h] [ebp-198h]
  float *v37; // [esp+138h] [ebp-194h]
  _BYTE *v38; // [esp+13Ch] [ebp-190h]
  float44 *p_viewMatrix; // [esp+140h] [ebp-18Ch]
  float *dir; // [esp+144h] [ebp-188h]
  float *attenuation; // [esp+148h] [ebp-184h]
  float *v42; // [esp+14Ch] [ebp-180h]
  float *origin; // [esp+150h] [ebp-17Ch]
  float v44; // [esp+154h] [ebp-178h]
  int i; // [esp+158h] [ebp-174h]
  GfxSpotShadow *v46; // [esp+15Ch] [ebp-170h]
  _BYTE scale[140]; // [esp+160h] [ebp-16Ch] OVERLAPPED BYREF
  float farEdge; // [esp+1ECh] [ebp-E0h]
  int nearEdge; // [esp+1F0h] [ebp-DCh]
  float cutOff; // [esp+1F4h] [ebp-D8h]
  float cutOn; // [esp+1F8h] [ebp-D4h]
  int lightExponent; // [esp+1FCh] [ebp-D0h]
  float lightCosHalfFovOuter; // [esp+200h] [ebp-CCh]
  float lightCosHalfFovInner; // [esp+204h] [ebp-C8h]
  float spotShadowFade; // [esp+208h] [ebp-C4h]
  float lightRadius; // [esp+20Ch] [ebp-C0h]
  float lightDir[3]; // [esp+210h] [ebp-BCh] BYREF
  float lightAttentuation[4]; // [esp+21Ch] [ebp-B0h] BYREF
  float lightOrigin[4]; // [esp+22Ch] [ebp-A0h]
  float v60; // [esp+23Ch] [ebp-90h]
  float specularColor[3]; // [esp+240h] [ebp-8Ch] BYREF
  float diffuseColor[3]; // [esp+24Ch] [ebp-80h] BYREF
  GfxLightDef *lightDef; // [esp+258h] [ebp-74h]
  float blues[4]; // [esp+25Ch] [ebp-70h] BYREF
  float greens[4]; // [esp+26Ch] [ebp-60h] BYREF
  float reds[4]; // [esp+27Ch] [ebp-50h] BYREF
  float falloffs[4]; // [esp+28Ch] [ebp-40h] BYREF
  float zs[4]; // [esp+29Ch] [ebp-30h] BYREF
  float ys[4]; // [esp+2ACh] [ebp-20h] BYREF
  float xs[4]; // [esp+2BCh] [ebp-10h]
  float retaddr; // [esp+2CCh] [ebp+0h]

  xs[1] = a1;
  xs[2] = retaddr;
  xs[0] = *(float *)&FLOAT_0_0;
  memset(ys, 0, sizeof(ys));
  memset(zs, 0, sizeof(zs));
  memset(falloffs, 0, sizeof(falloffs));
  memset(reds, 0, sizeof(reds));
  memset(greens, 0, sizeof(greens));
  lightDef = *(GfxLightDef **)&FLOAT_0_0;
  memset(blues, 0, sizeof(blues));
  memset(diffuseColor, 0, sizeof(diffuseColor));
  memset(specularColor, 0, sizeof(specularColor));
  lightOrigin[2] = *(float *)&FLOAT_0_0;
  lightOrigin[3] = *(float *)&FLOAT_0_0;
  v60 = *(float *)&FLOAT_0_0;
  lightOrigin[0] = FLOAT_1_0;
  memset(lightDir, 0, sizeof(lightDir));
  memset(lightAttentuation, 0, sizeof(lightAttentuation));
  lightCosHalfFovInner = *(float *)&FLOAT_0_0;
  spotShadowFade = *(float *)&FLOAT_0_0;
  lightRadius = *(float *)&FLOAT_0_0;
  lightCosHalfFovOuter = *(float *)&FLOAT_0_0;
  lightExponent = *(unsigned int *)&FLOAT_0_0;
  cutOn = *(float *)&FLOAT_0_0;
  cutOff = *(float *)&FLOAT_0_0;
  nearEdge = 0;
  farEdge = *(float *)&FLOAT_0_0;
  memset(scale, 0, sizeof(scale));
  v46 = 0;
  for ( i = 0; i < visibleLightCount; ++i )
  {
    if ( visibleLights[i].type == 3 )
    {
      ys[i + 1] = visibleLights[i].origin[0] - viewInfo->cullViewInfo.viewParms.origin[0];
      zs[i + 1] = visibleLights[i].origin[1] - viewInfo->cullViewInfo.viewParms.origin[1];
      falloffs[i + 1] = visibleLights[i].origin[2] - viewInfo->cullViewInfo.viewParms.origin[2];
      reds[i + 1] = -1.0 / (float)(visibleLights[i].radius * visibleLights[i].radius);
      v44 = 1.0 / viewInfo->exposureRemap.remapMul[0];
      greens[i + 1] = (float)(visibleLights[i].diffuseColor[0] * r_diffuseColorScale->current.value) * v44;
      blues[i + 1] = (float)(visibleLights[i].diffuseColor[1] * r_diffuseColorScale->current.value) * v44;
      diffuseColor[i + 1] = (float)(visibleLights[i].diffuseColor[2] * r_diffuseColorScale->current.value) * v44;
    }
    else
    {
      if ( visibleLights[i].type != 2
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
              4490,
              0,
              "%s",
              "visibleLights[li].type == GFX_LIGHT_TYPE_SPOT") )
      {
        __debugbreak();
      }
      diffuseColor[0] = *(float *)&visibleLights[i].def;
      origin = viewInfo->cullViewInfo.viewParms.origin;
      v42 = visibleLights[i].origin;
      lightAttentuation[1] = *v42 - viewInfo->cullViewInfo.viewParms.origin[0];
      lightAttentuation[2] = visibleLights[i].origin[1] - viewInfo->cullViewInfo.viewParms.origin[1];
      lightAttentuation[3] = visibleLights[i].origin[2] - viewInfo->cullViewInfo.viewParms.origin[2];
      lightCosHalfFovOuter = visibleLights[i].radius;
      attenuation = visibleLights[i].attenuation;
      lightDir[0] = *attenuation;
      lightDir[1] = visibleLights[i].attenuation[1];
      lightDir[2] = visibleLights[i].attenuation[2];
      lightAttentuation[0] = visibleLights[i].attenuation[3];
      lightDir[0] = lightDir[0] + 0.000015287891;
      dir = visibleLights[i].dir;
      lightCosHalfFovInner = *dir;
      spotShadowFade = visibleLights[i].dir[1];
      lightRadius = visibleLights[i].dir[2];
      cutOn = visibleLights[i].cosHalfFovInner;
      cutOff = visibleLights[i].cosHalfFovOuter;
      nearEdge = visibleLights[i].exponent;
      farEdge = visibleLights[i].falloff[0];
      *(float *)&scale[136] = visibleLights[i].falloff[1];
      *(float *)&scale[132] = visibleLights[i].falloff[2];
      *(float *)&scale[128] = visibleLights[i].falloff[3];
      p_viewMatrix = &visibleLights[i].viewMatrix;
      *(_QWORD *)&scale[64] = *(_QWORD *)&p_viewMatrix->m[0][0];
      *(float *)&scale[72] = visibleLights[i].viewMatrix.m[0][2];
      *(float *)&scale[76] = visibleLights[i].viewMatrix.m[0][3];
      v38 = &scale[80];
      v37 = visibleLights[i].viewMatrix.m[1];
      *(float *)&scale[80] = *v37;
      *(float *)&scale[84] = visibleLights[i].viewMatrix.m[1][1];
      *(float *)&scale[88] = visibleLights[i].viewMatrix.m[1][2];
      *(float *)&scale[92] = visibleLights[i].viewMatrix.m[1][3];
      v36 = &scale[96];
      v35 = visibleLights[i].viewMatrix.m[2];
      *(float *)&scale[96] = *v35;
      *(float *)&scale[100] = visibleLights[i].viewMatrix.m[2][1];
      *(float *)&scale[104] = visibleLights[i].viewMatrix.m[2][2];
      *(float *)&scale[108] = visibleLights[i].viewMatrix.m[2][3];
      v34 = &scale[112];
      v33 = visibleLights[i].viewMatrix.m[3];
      *(float *)&scale[112] = *v33;
      *(float *)&scale[116] = visibleLights[i].viewMatrix.m[3][1];
      *(float *)&scale[120] = visibleLights[i].viewMatrix.m[3][2];
      *(float *)&scale[124] = visibleLights[i].viewMatrix.m[3][3];
      p_projMatrix = &visibleLights[i].projMatrix;
      *(float *)scale = p_projMatrix->m[0][0];
      *(float *)&scale[4] = visibleLights[i].projMatrix.m[0][1];
      *(float *)&scale[8] = visibleLights[i].projMatrix.m[0][2];
      *(float *)&scale[12] = visibleLights[i].projMatrix.m[0][3];
      v31 = &scale[16];
      v30 = visibleLights[i].projMatrix.m[1];
      *(float *)&scale[16] = *v30;
      *(float *)&scale[20] = visibleLights[i].projMatrix.m[1][1];
      *(float *)&scale[24] = visibleLights[i].projMatrix.m[1][2];
      *(float *)&scale[28] = visibleLights[i].projMatrix.m[1][3];
      v29 = &scale[32];
      sli = (unsigned int)visibleLights[i].projMatrix.m[2];
      *(float *)&scale[32] = *(float *)sli;
      *(float *)&scale[36] = visibleLights[i].projMatrix.m[2][1];
      *(float *)&scale[40] = visibleLights[i].projMatrix.m[2][2];
      *(float *)&scale[44] = visibleLights[i].projMatrix.m[2][3];
      v27 = &scale[48];
      v26 = visibleLights[i].projMatrix.m[3];
      *(float *)&scale[48] = *v26;
      *(float *)&scale[52] = visibleLights[i].projMatrix.m[3][1];
      *(float *)&scale[56] = visibleLights[i].projMatrix.m[3][2];
      *(float *)&scale[60] = visibleLights[i].projMatrix.m[3][3];
      for ( j = 0; j < frontEndDataOut->shadowableLightCount; ++j )
      {
        if ( frontEndDataOut->shadowableLights[j].type == 2
          && frontEndDataOut->shadowableLights[j].spotShadowIndex != -1
          && visibleLights[i].def == frontEndDataOut->shadowableLights[j].def )
        {
          v24 = frontEndDataOut->shadowableLights[j].origin;
          v23 = visibleLights[i].origin;
          if ( *v23 == *v24 && v23[1] == v24[1] && v23[2] == v24[2] )
          {
            v46 = &frontEndDataOut->spotShadows[frontEndDataOut->shadowableLights[j].spotShadowIndex];
            lightExponent = LODWORD(v46->fade);
            break;
          }
        }
      }
      value = r_diffuseColorScale->current.value;
      v20 = visibleLights[i].diffuseColor;
      specularColor[0] = value * *v20;
      specularColor[1] = value * visibleLights[i].diffuseColor[1];
      specularColor[2] = value * visibleLights[i].diffuseColor[2];
      eyeOffset[3] = r_specularColorScale->current.value;
      LODWORD(eyeOffset[2]) = visibleLights[i].specularColor;
      lightOrigin[2] = eyeOffset[3] * *(float *)LODWORD(eyeOffset[2]);
      lightOrigin[3] = eyeOffset[3] * visibleLights[i].specularColor[1];
      v60 = eyeOffset[3] * visibleLights[i].specularColor[2];
    }
  }
  R_SetInputCodeConstant(input, 0x8Bu, ys[1], ys[2], ys[3], xs[0]);
  R_SetInputCodeConstant(input, 0x8Cu, zs[1], zs[2], zs[3], ys[0]);
  R_SetInputCodeConstant(input, 0x8Du, falloffs[1], falloffs[2], falloffs[3], zs[0]);
  R_SetInputCodeConstant(input, 0x8Eu, reds[1], reds[2], reds[3], falloffs[0]);
  R_SetInputCodeConstant(input, 0x8Fu, greens[1], greens[2], greens[3], reds[0]);
  R_SetInputCodeConstant(input, 0x90u, blues[1], blues[2], blues[3], greens[0]);
  R_SetInputCodeConstant(input, 0x91u, diffuseColor[1], diffuseColor[2], *(float *)&lightDef, blues[0]);
  if ( viewInfo->cullViewInfo.viewParms.origin[3] == 0.0 )
  {
    *(unsigned int *)&v17[72] = *(unsigned int *)&FLOAT_0_0;
    *(unsigned int *)&v17[76] = *(unsigned int *)&FLOAT_0_0;
    v18 = *(float *)&FLOAT_0_0;
  }
  else
  {
    LODWORD(eyeOffset[1]) = viewInfo->cullViewInfo.viewParms.origin;
    *(_QWORD *)&v17[72] = *(_QWORD *)viewInfo->viewParms.origin;
    v18 = viewInfo->cullViewInfo.viewParms.origin[2];
  }
  eyeOffset[0] = FLOAT_1_0;
  if ( LODWORD(diffuseColor[0]) )
  {
    *(unsigned int *)&v17[68] = *(unsigned int *)(LODWORD(diffuseColor[0]) + 4);
    if ( !input && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state.h", 1850, 0, "%s", "input") )
      __debugbreak();
    input->codeImages[17] = *(const GfxImage **)&v17[68];
    R_SetInputCodeImageSamplerState(input, 0x11u, *(_BYTE *)(LODWORD(diffuseColor[0]) + 8));
  }
  else
  {
    *(unsigned int *)&v17[64] = rgp.whiteImage;
    if ( !input && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state.h", 1850, 0, "%s", "input") )
      __debugbreak();
    input->codeImages[17] = *(const GfxImage **)&v17[64];
    R_SetInputCodeImageSamplerState(input, 0x11u, 0x61u);
  }
  if ( v46 )
  {
    memcpy(v17, &v46->lookupMatrix, 0x40u);
    MatrixTransformVector44((const float *)&v17[72], (const float (*)[4])v17, (float *)&v17[48]);
    R_SetInputCodeConstantFromVec4(input, 0x9Du, (float *)v17);
    R_SetInputCodeConstantFromVec4(input, 0x9Eu, (float *)&v17[16]);
    R_SetInputCodeConstantFromVec4(input, 0x9Fu, (float *)&v17[32]);
    R_SetInputCodeConstantFromVec4(input, 0xA0u, (float *)&v17[48]);
    sMul = v46->image;
    if ( !input && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state.h", 1850, 0, "%s", "input") )
      __debugbreak();
    input->codeImages[7] = sMul;
    R_SetInputCodeImageSamplerState(input, 7u, 0x62u);
    R_SetInputCodeConstantFromVec4(input, 0x47u, v46->pixelAdjust);
  }
  else
  {
    whiteImage = rgp.whiteImage;
    if ( !input && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_state.h", 1850, 0, "%s", "input") )
      __debugbreak();
    input->codeImages[7] = whiteImage;
    R_SetInputCodeImageSamplerState(input, 7u, 0x62u);
  }
  R_SetInputCodeConstantFromVec4(input, 0x95u, lightDir);
  if ( farEdge == *(float *)&scale[132] )
    sAdd = FLOAT_1_0;
  else
    sAdd = 1.0 / (float)(*(float *)&scale[132] - farEdge);
  x = sAdd;
  if ( farEdge == *(float *)&scale[132] )
    LODWORD(v5) = LODWORD(farEdge) ^ _mask__NegFloat_;
  else
    v5 = COERCE_FLOAT(LODWORD(farEdge) ^ _mask__NegFloat_) * (float)(1.0 / (float)(*(float *)&scale[132] - farEdge));
  eMul = v5;
  z = v5;
  if ( *(float *)&scale[128] == *(float *)&scale[136] )
    eAdd = FLOAT_1_0;
  else
    eAdd = 1.0 / (float)(*(float *)&scale[136] - *(float *)&scale[128]);
  y = eAdd;
  if ( *(float *)&scale[128] == *(float *)&scale[136] )
    LODWORD(w) = *(unsigned int *)&scale[128] ^ _mask__NegFloat_;
  else
    w = COERCE_FLOAT(*(unsigned int *)&scale[128] ^ _mask__NegFloat_)
      * (float)(1.0 / (float)(*(float *)&scale[136] - *(float *)&scale[128]));
  v8 = w;
  *(float *)&v7[72] = w;
  R_SetInputCodeConstant(input, 0x96u, x, y, z, w);
  MatrixTransformVector44(&lightAttentuation[1], (const float (*)[4])&scale[64], (float *)&scale[112]);
  *(unsigned int *)&v7[68] = &scale[112];
  *(unsigned int *)&v7[64] = &scale[112];
  *(unsigned int *)&scale[112] ^= _mask__NegFloat_;
  *(unsigned int *)&scale[116] ^= _mask__NegFloat_;
  *(unsigned int *)&scale[120] ^= _mask__NegFloat_;
  MatrixMultiply44((const float (*)[4])&scale[64], (const float (*)[4])scale, (float (*)[4])v7);
  R_SetInputCodeConstant(input, 0x97u, *(float *)v7, *(float *)&v7[16], *(float *)&v7[32], *(float *)&v7[48]);
  R_SetInputCodeConstant(input, 0x98u, *(float *)&v7[4], *(float *)&v7[20], *(float *)&v7[36], *(float *)&v7[52]);
  R_SetInputCodeConstant(input, 0x99u, *(float *)&v7[8], *(float *)&v7[24], *(float *)&v7[40], *(float *)&v7[56]);
  R_SetInputCodeConstant(input, 0x9Au, *(float *)&v7[12], *(float *)&v7[28], *(float *)&v7[44], *(float *)&v7[60]);
  R_SetInputCodeConstant(
    input,
    0x92u,
    lightAttentuation[1],
    lightAttentuation[2],
    lightAttentuation[3],
    1.0 / (float)(lightCosHalfFovOuter * lightCosHalfFovOuter));
  R_SetInputCodeConstant(input, 0x93u, specularColor[0], specularColor[1], specularColor[2], 1.0);
  R_SetInputCodeConstant(input, 0x94u, lightOrigin[2], lightOrigin[3], v60, 1.0);
  R_SetInputCodeConstant(input, 0x9Bu, lightCosHalfFovInner, spotShadowFade, lightRadius, 0.0);
  R_SetInputCodeConstant(
    input,
    0x9Cu,
    1.0 / (float)(cutOn - cutOff),
    COERCE_FLOAT(COERCE_UNSIGNED_INT(1.0 / (float)(cutOn - cutOff)) ^ _mask__NegFloat_) * cutOff,
    0.0,
    *(float *)&lightExponent);
}

void R_DrawCineWarning()
{
  Font_s *font; // [esp+1Ch] [ebp-8h]
  char *msg; // [esp+20h] [ebp-4h]

  if ( com_statmon->current.enabled && R_Cinematic_IsStarted() )
  {
    msg = "CINE";
    font = R_RegisterFont("fonts/consoleFont", 1);
    if ( R_Cinematic_IsUnderrun() )
      msg = "CINE UNDERRUN!";
    if ( cg_draw2D->current.enabled )
      R_AddCmdDrawText(msg, 0x7FFFFFFF, font, 0.0, 360.0, 1.5, 2.0, 0.0, colorRedFaded, 0);
  }
}

void __cdecl R_SplitDrawSurfacesPrimarySortKey(
        GfxDrawSurfListInfo *srcList,
        GfxDrawSurfListInfo *destList,
        int sortkeyID)
{
  unsigned __int16 start; // [esp+18h] [ebp-4h]

  for ( start = 0;
        start < srcList->drawSurfCount
     && (sortkeyID < 0
      || (sortkeyID < 0 || sortkeyID >= 0) && ((srcList->drawSurfs[start].packed >> 58) & 0x3F) < sortkeyID);
        ++start )
  {
    ;
  }
  destList->drawSurfCount = srcList->drawSurfCount - start;
  destList->drawSurfs = &srcList->drawSurfs[start];
  srcList->drawSurfCount = start;
}

void __cdecl DrawLightDebug(const GfxViewInfo *viewInfo, const GfxLight *L, const float *debugColor)
{
  float target[3]; // [esp+14h] [ebp-40h] BYREF
  float mins[3]; // [esp+20h] [ebp-34h] BYREF
  float ext[3]; // [esp+2Ch] [ebp-28h]
  float maxs[3]; // [esp+38h] [ebp-1Ch] BYREF
  float lightColor[4]; // [esp+44h] [ebp-10h] BYREF

  ext[0] = FLOAT_10_0;
  ext[1] = FLOAT_10_0;
  ext[2] = FLOAT_10_0;
  maxs[0] = L->origin[0] + 10.0;
  maxs[1] = L->origin[1] + 10.0;
  maxs[2] = L->origin[2] + 10.0;
  mins[0] = (float)(-1.0 * 10.0) + L->origin[0];
  mins[1] = (float)(-1.0 * 10.0) + L->origin[1];
  mins[2] = (float)(-1.0 * 10.0) + L->origin[2];
  lightColor[0] = L->color[0];
  lightColor[1] = L->color[1];
  lightColor[2] = L->color[2];
  lightColor[3] = FLOAT_1_0;
  lightColor[0] = (float)(viewInfo->exposureValue.blackPoint[3] / 8.0) * lightColor[0];
  lightColor[1] = (float)(viewInfo->exposureValue.blackPoint[3] / 8.0) * lightColor[1];
  lightColor[2] = (float)(viewInfo->exposureValue.blackPoint[3] / 8.0) * lightColor[2];
  R_AddDebugBox(&frontEndDataOut->debugGlobals, mins, maxs, debugColor);
  if ( L->type == 2 )
  {
    target[0] = (float)(-20.0 * L->dir[0]) + L->origin[0];
    target[1] = (float)(-20.0 * L->dir[1]) + L->origin[1];
    target[2] = (float)(-20.0 * L->dir[2]) + L->origin[2];
    R_AddDebugLine(&frontEndDataOut->debugGlobals, L->origin, target, lightColor, 0);
  }
  else
  {
    R_AddDebugLine(&frontEndDataOut->debugGlobals, mins, maxs, lightColor, 0);
  }
}

void DrawOutdoorBoundsVolumeDebug()
{
  float boxColor[4]; // [esp+0h] [ebp-14h] BYREF

  if ( r_showOutdoorVolumeDebug->current.enabled )
  {
    if ( rgp.world->numOutdoorBounds )
    {
      boxColor[0] = FLOAT_1_0;
      boxColor[1] = FLOAT_1_0;
      boxColor[2] = FLOAT_1_0;
      boxColor[3] = FLOAT_1_0;
      R_AddDebugBox(
        &frontEndDataOut->debugGlobals,
        (const float *)rgp.world->outdoorBounds,
        rgp.world->outdoorBounds->bounds[1],
        boxColor);
    }
  }
}

void __cdecl DrawSunDirectionDebug(const float *viewOrg, const float *viewForward)
{
  float boxColor[4]; // [esp+4h] [ebp-A4h] BYREF
  float lineEnd[3]; // [esp+14h] [ebp-94h] BYREF
  float zdir[3]; // [esp+20h] [ebp-88h] BYREF
  float sunColor[4]; // [esp+2Ch] [ebp-7Ch] BYREF
  float mins[3]; // [esp+3Ch] [ebp-6Ch] BYREF
  float zColor[4]; // [esp+48h] [ebp-60h] BYREF
  float xColor[4]; // [esp+58h] [ebp-50h] BYREF
  float ext[3]; // [esp+68h] [ebp-40h]
  float ydir[3]; // [esp+74h] [ebp-34h] BYREF
  float maxs[3]; // [esp+80h] [ebp-28h] BYREF
  float yColor[4]; // [esp+8Ch] [ebp-1Ch] BYREF
  float xdir[3]; // [esp+9Ch] [ebp-Ch] BYREF

  if ( r_showSunDirectionDebug->current.enabled )
  {
    if ( !lastShowDebug )
    {
      debugOrigin[0] = (float)(100.0 * *viewForward) + *viewOrg;
      *(float *)&dword_AA1C458 = (float)(100.0 * viewForward[1]) + viewOrg[1];
      *(float *)&dword_AA1C45C = (float)(100.0 * viewForward[2]) + viewOrg[2];
    }
    lineEnd[0] = (float)(200.0 * frontEndDataOut->sunLight.dir[0]) + debugOrigin[0];
    lineEnd[1] = (float)(200.0 * frontEndDataOut->sunLight.dir[1]) + *(float *)&dword_AA1C458;
    lineEnd[2] = (float)(200.0 * frontEndDataOut->sunLight.dir[2]) + *(float *)&dword_AA1C45C;
    sunColor[0] = FLOAT_1_0;
    sunColor[1] = FLOAT_1_0;
    sunColor[2] = FLOAT_1_0;
    sunColor[3] = FLOAT_1_0;
    R_AddDebugLine(&frontEndDataOut->debugGlobals, debugOrigin, lineEnd, sunColor, 0);
    boxColor[0] = FLOAT_1_0;
    boxColor[1] = FLOAT_1_0;
    boxColor[2] = FLOAT_1_0;
    boxColor[3] = FLOAT_1_0;
    ext[0] = FLOAT_5_0;
    ext[1] = FLOAT_5_0;
    ext[2] = FLOAT_5_0;
    maxs[0] = debugOrigin[0] + 5.0;
    maxs[1] = *(float *)&dword_AA1C458 + 5.0;
    maxs[2] = *(float *)&dword_AA1C45C + 5.0;
    mins[0] = (float)(-1.0 * 5.0) + debugOrigin[0];
    mins[1] = (float)(-1.0 * 5.0) + *(float *)&dword_AA1C458;
    mins[2] = (float)(-1.0 * 5.0) + *(float *)&dword_AA1C45C;
    R_AddDebugBox(&frontEndDataOut->debugGlobals, mins, maxs, boxColor);
    xColor[0] = FLOAT_1_0;
    xColor[1] = *(float *)&FLOAT_0_0;
    xColor[2] = *(float *)&FLOAT_0_0;
    xColor[3] = FLOAT_1_0;
    yColor[0] = *(float *)&FLOAT_0_0;
    yColor[1] = FLOAT_1_0;
    yColor[2] = *(float *)&FLOAT_0_0;
    yColor[3] = FLOAT_1_0;
    zColor[0] = *(float *)&FLOAT_0_0;
    zColor[1] = *(float *)&FLOAT_0_0;
    zColor[2] = FLOAT_1_0;
    zColor[3] = FLOAT_1_0;
    xdir[0] = (float)(40.0 * 1.0) + debugOrigin[0];
    xdir[1] = (float)(40.0 * 0.0) + *(float *)&dword_AA1C458;
    xdir[2] = (float)(40.0 * 0.0) + *(float *)&dword_AA1C45C;
    ydir[0] = (float)(40.0 * 0.0) + debugOrigin[0];
    ydir[1] = (float)(40.0 * 1.0) + *(float *)&dword_AA1C458;
    ydir[2] = xdir[2];
    zdir[0] = ydir[0];
    zdir[1] = xdir[1];
    zdir[2] = (float)(40.0 * 1.0) + *(float *)&dword_AA1C45C;
    R_AddDebugLine(&frontEndDataOut->debugGlobals, debugOrigin, xdir, xColor, 0);
    R_AddDebugLine(&frontEndDataOut->debugGlobals, debugOrigin, ydir, yColor, 0);
    R_AddDebugLine(&frontEndDataOut->debugGlobals, debugOrigin, zdir, zColor, 0);
  }
  lastShowDebug = r_showSunDirectionDebug->current.enabled;
}

unsigned intR_SortAllCodeMeshSurfacesSunShadow()
{
  unsigned intresult; // eax

  PIXBeginNamedEvent(-1, "R_SortAllCodeMeshSurfacesSunShadow");
  R_SortDrawSurfs(scene.drawSurfs[22], scene.drawSurfCount[22]);
  R_SortDrawSurfs(scene.drawSurfs[26], scene.drawSurfCount[26]);
  result = GetCurrentThreadId();
  if ( result == (unsigned int)g_DXDeviceThread )
  {
    result = 0;
    if ( !HIDWORD(g_DXDeviceThread) )
      return D3DPERF_EndEvent();
  }
  return result;
}

void R_DrawFogParams()
{
  Font_s *font; // [esp+20h] [ebp-A0h]
  GfxFog *f; // [esp+2Ch] [ebp-94h]
  float y; // [esp+34h] [ebp-8Ch]
  float ya; // [esp+34h] [ebp-8Ch]
  float yb; // [esp+34h] [ebp-8Ch]
  float yc; // [esp+34h] [ebp-8Ch]
  float yd; // [esp+34h] [ebp-8Ch]
  float ye; // [esp+34h] [ebp-8Ch]
  float yf; // [esp+34h] [ebp-8Ch]
  float yg; // [esp+34h] [ebp-8Ch]
  float yh; // [esp+34h] [ebp-8Ch]
  float yi; // [esp+34h] [ebp-8Ch]
  char text[132]; // [esp+38h] [ebp-88h] BYREF

  if ( r_drawDebugFogParams->current.enabled )
  {
    f = &frontEndDataOut->fogSettings;
    font = R_RegisterFont("fonts/consoleFont", 1);
    _snprintf(text, 0x80u, "fog.startTime = %d", f->startTime);
    R_AddCmdDrawText(text, 128, font, 100.0, 150.0, 1.5, 2.0, 0.0, colorWhite, 0);
    _snprintf(text, 0x80u, "fog.finishTime = %d", f->finishTime);
    R_AddCmdDrawText(text, 128, font, 100.0, 25.0 + 150.0, 1.5, 2.0, 0.0, colorWhite, 0);
    y = (float)(25.0 + 150.0) + 25.0;
    _snprintf(text, 0x80u, "fog.fogStart = %.4f", f->fogStart);
    R_AddCmdDrawText(text, 128, font, 100.0, y, 1.5, 2.0, 0.0, colorWhite, 0);
    ya = y + 25.0;
    _snprintf(text, 0x80u, "fog.density = %.4f", f->density);
    R_AddCmdDrawText(text, 128, font, 100.0, ya, 1.5, 2.0, 0.0, colorWhite, 0);
    yb = ya + 25.0;
    _snprintf(text, 0x80u, "fog.maxDensity = %.4f", f->maxDensity);
    R_AddCmdDrawText(text, 128, font, 100.0, yb, 1.5, 2.0, 0.0, colorWhite, 0);
    yc = yb + 25.0;
    _snprintf(text, 0x80u, "fog.heightDensity = %.4f", f->heightDensity);
    R_AddCmdDrawText(text, 128, font, 100.0, yc, 1.5, 2.0, 0.0, colorWhite, 0);
    yd = yc + 25.0;
    _snprintf(text, 0x80u, "fog.baseHeight = %.4f", f->baseHeight);
    R_AddCmdDrawText(text, 128, font, 100.0, yd, 1.5, 2.0, 0.0, colorWhite, 0);
    ye = yd + 25.0;
    _snprintf(text, 0x80u, "fog.colorScale = %.4", f->color[3]);
    R_AddCmdDrawText(text, 128, font, 100.0, ye, 1.5, 2.0, 0.0, colorWhite, 0);
    yf = ye + 25.0;
    _snprintf(text, 0x80u, "fog.color = %.2f, %.2f, %.2f", f->color[0], f->color[1], f->color[2]);
    R_AddCmdDrawText(text, 128, font, 100.0, yf, 1.5, 2.0, 0.0, colorWhite, 0);
    yg = yf + 25.0;
    _snprintf(text, 0x80u, "fog.sunFogColor = %.2f %.2f %.2f", f->sunFogColor[0], f->sunFogColor[1], f->sunFogColor[2]);
    R_AddCmdDrawText(text, 128, font, 100.0, yg, 1.5, 2.0, 0.0, colorWhite, 0);
    yh = yg + 25.0;
    _snprintf(text, 0x80u, "fog.sunFogDir = %.2f %.2f %.2f", f->sunFogDir[0], f->sunFogDir[1], f->sunFogDir[2]);
    R_AddCmdDrawText(text, 128, font, 100.0, yh, 1.5, 2.0, 0.0, colorWhite, 0);
    yi = yh + 25.0;
    _snprintf(text, 0x80u, "fog.sunFogStartAngle = %g", f->sunFogStartAng);
    R_AddCmdDrawText(text, 128, font, 100.0, yi, 1.5, 2.0, 0.0, colorWhite, 0);
    _snprintf(text, 0x80u, "fog.sunFogEndAngle = %g", f->sunFogEndAng);
    R_AddCmdDrawText(text, 128, font, 100.0, yi + 25.0, 1.5, 2.0, 0.0, colorWhite, 0);
  }
}

unsigned intR_WaitForFXUpdateWorkerCmds()
{
  unsigned intresult; // eax

  PIXBeginNamedEvent(-1, "R_WaitForFXUpdateWorkerCmds");
  PIXBeginNamedEvent(-1, "wait fx_update");
  Sys_WaitWorkerCmdInternal(&fx_updateWorkerCmd);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  PIXBeginNamedEvent(-1, "wait fx_update_remaining");
  Sys_WaitWorkerCmdInternal(&fx_update_remainingWorkerCmd);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  PIXBeginNamedEvent(-1, "wait fx_update_remaining_ppu");
  Sys_WaitWorkerCmdInternal(&fx_update_remaining_ppuWorkerCmd);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  result = GetCurrentThreadId();
  if ( result == (unsigned int)g_DXDeviceThread )
  {
    result = 0;
    if ( !HIDWORD(g_DXDeviceThread) )
      return D3DPERF_EndEvent();
  }
  return result;
}

void __cdecl R_FinishDecalAndEmissiveDrawSurfs(
        GfxViewInfo *viewInfo,
        const GfxViewParms *viewParmsDraw,
        GfxBackEndData *backendData,
        bool forMissileCamView)
{
  int firstDrawSurfCount; // [esp+64h] [ebp-14h]
  int firstDrawSurfCounta; // [esp+64h] [ebp-14h]
  int firstDrawSurfCountb; // [esp+64h] [ebp-14h]
  GfxDrawSurfListInfo *emissiveInfo; // [esp+6Ch] [ebp-Ch]

  PIXBeginNamedEvent(-1, "R_FinishDecalAndEmissiveDrawSurfs");
  PIXBeginNamedEvent(-1, "wait for fx_draw");
  Sys_WaitWorkerCmdInternal(&fx_drawWorkerCmd);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  GlassCl_WaitGenerateVerts();
  PIXBeginNamedEvent(-1, "R_SortDrawSurfs FX_CAMERA_LIT");
  R_SortDrawSurfs(scene.drawSurfs[6], scene.drawSurfCount[6]);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  PIXBeginNamedEvent(-1, "R_SortDrawSurfs CODEMESH_CAMERA_LIT");
  R_SortDrawSurfs(scene.drawSurfs[9], scene.drawSurfCount[9]);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  PIXBeginNamedEvent(-1, "set up decalInfo");
  R_InitDrawSurfListInfo(&viewInfo->drawList[3]);
  viewInfo->drawList[3].baseTechType = gfxDrawMethod.baseTechType;
  viewInfo->drawList[3].viewInfo = viewInfo;
  viewInfo->drawList[3].viewOrigin[0] = viewParmsDraw->origin[0];
  viewInfo->drawList[3].viewOrigin[1] = viewParmsDraw->origin[1];
  viewInfo->drawList[3].viewOrigin[2] = viewParmsDraw->origin[2];
  viewInfo->drawList[3].viewOrigin[3] = viewParmsDraw->origin[3];
  viewInfo->drawList[3].cameraView = 1;
  viewInfo->drawList[3].shaderHDRmode = viewInfo->sceneComposition.alphaShaderRemap;
  R_InitDrawSurfListInfo(&viewInfo->drawList[6]);
  viewInfo->drawList[6].baseTechType = 10;
  viewInfo->drawList[6].viewInfo = viewInfo;
  viewInfo->drawList[6].viewOrigin[0] = viewParmsDraw->origin[0];
  viewInfo->drawList[6].viewOrigin[1] = viewParmsDraw->origin[1];
  viewInfo->drawList[6].viewOrigin[2] = viewParmsDraw->origin[2];
  viewInfo->drawList[6].viewOrigin[3] = viewParmsDraw->origin[3];
  viewInfo->drawList[6].cameraView = 1;
  viewInfo->drawList[6].shaderHDRmode = 0;
  firstDrawSurfCount = backendData->drawSurfCount;
  R_MergeAndEmitDrawSurfLists(3u, 7u, backendData);
  viewInfo->drawList[3].drawSurfs = &backendData->drawSurfs[firstDrawSurfCount];
  viewInfo->drawList[3].drawSurfCount = backendData->drawSurfCount - firstDrawSurfCount;
  if ( !forMissileCamView )
    R_SplitDrawSurfacesPrimarySortKey(&viewInfo->drawList[3], &viewInfo->drawList[6], 60);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  PIXBeginNamedEvent(-1, "sort fx surfs");
  R_SortDrawSurfs(scene.drawSurfs[13], scene.drawSurfCount[13]);
  R_SortDrawSurfs(scene.drawSurfs[16], scene.drawSurfCount[16]);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  PIXBeginNamedEvent(-1, "set up emissiveInfo");
  emissiveInfo = &viewInfo->drawList[4];
  R_InitDrawSurfListInfo(&viewInfo->drawList[4]);
  viewInfo->drawList[4].baseTechType = R_GetEmissiveTechnique(viewInfo, gfxDrawMethod.emissiveTechType);
  viewInfo->drawList[4].viewInfo = viewInfo;
  viewInfo->drawList[4].viewOrigin[0] = viewParmsDraw->origin[0];
  viewInfo->drawList[4].viewOrigin[1] = viewParmsDraw->origin[1];
  viewInfo->drawList[4].viewOrigin[2] = viewParmsDraw->origin[2];
  viewInfo->drawList[4].viewOrigin[3] = viewParmsDraw->origin[3];
  viewInfo->drawList[4].cameraView = 1;
  viewInfo->drawList[4].shaderHDRmode = viewInfo->sceneComposition.emissiveShaderRemap;
  if ( backendData->emissiveSpotLightCount )
  {
    if ( backendData->emissiveSpotLightCount != 1
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
            5268,
            0,
            "%s",
            "backendData->emissiveSpotLightCount == 1") )
    {
      __debugbreak();
    }
    viewInfo->drawList[4].light = &backendData->emissiveSpotLight;
  }
  firstDrawSurfCounta = backendData->drawSurfCount;
  R_MergeAndEmitDrawSurfLists(0xAu, 6u, backendData);
  emissiveInfo->drawSurfs = &backendData->drawSurfs[firstDrawSurfCounta];
  viewInfo->drawList[4].drawSurfCount = backendData->drawSurfCount - firstDrawSurfCounta;
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
  PIXBeginNamedEvent(-1, "set up reflectedInfo");
  R_InitDrawSurfListInfo(&viewInfo->drawList[5]);
  viewInfo->drawList[5].baseTechType = 9;
  viewInfo->drawList[5].viewInfo = viewInfo;
  viewInfo->drawList[5].viewOrigin[0] = viewParmsDraw->origin[0];
  viewInfo->drawList[5].viewOrigin[1] = viewParmsDraw->origin[1];
  viewInfo->drawList[5].viewOrigin[2] = viewParmsDraw->origin[2];
  viewInfo->drawList[5].viewOrigin[3] = viewParmsDraw->origin[3];
  viewInfo->drawList[5].cameraView = 1;
  viewInfo->drawList[5].shaderHDRmode = 0;
  firstDrawSurfCountb = backendData->drawSurfCount;
  R_MergeAndEmitDrawSurfLists(0x10u, 3u, backendData);
  viewInfo->drawList[5].drawSurfs = &backendData->drawSurfs[firstDrawSurfCountb];
  viewInfo->drawList[5].drawSurfCount = backendData->drawSurfCount - firstDrawSurfCountb;
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  if ( !viewInfo->needsFloatZ )
    viewInfo->needsFloatZ = R_DoesDrawSurfListInfoNeedFloatz(viewInfo, emissiveInfo);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

unsigned __int8 __cdecl R_GetEmissiveTechnique(const GfxViewInfo *viewInfo, unsigned __int8 baseTech)
{
  int PrimaryLightCount; // eax

  if ( baseTech != 5 )
    return baseTech;
  if ( r_fullbright->current.enabled )
    return 4;
  if ( !frontEndDataOut->emissiveSpotLightCount )
    return 5;
  if ( !r_spotLightShadows->current.enabled )
    return 5;
  PrimaryLightCount = Com_GetPrimaryLightCount();
  if ( !Com_BitCheckAssert(frontEndDataOut->shadowableLightHasShadowMap, PrimaryLightCount, 32) )
    return 5;
  if ( Com_GetPrimaryLightCount() + 1 != frontEndDataOut->shadowableLightCount
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          4054,
          0,
          "%s",
          "Com_GetPrimaryLightCount() + GFX_MAX_EMISSIVE_SPOT_LIGHTS == frontEndDataOut->shadowableLightCount") )
  {
    __debugbreak();
  }
  return 6;
}

char __cdecl R_DoesDrawSurfListInfoNeedFloatz(GfxViewInfo *viewInfo, GfxDrawSurfListInfo *emissiveInfo)
{
  const MaterialTechnique *technique; // [esp+8h] [ebp-Ch]
  unsigned int surfIndex; // [esp+Ch] [ebp-8h]

  if ( dx.supportsIntZ && !viewInfo->isMissileCamera )
    return 0;
  for ( surfIndex = 0; ; ++surfIndex )
  {
    if ( surfIndex == emissiveInfo->drawSurfCount )
      return 0;
    technique = Material_GetTechnique(
                  rgp.sortedMaterials[(emissiveInfo->drawSurfs[surfIndex].packed >> 31) & 0xFFF],
                  emissiveInfo->baseTechType);
    if ( technique )
    {
      if ( (technique->flags & 0x40) != 0 )
        break;
    }
  }
  if ( !dword_B50E834[5 * (viewInfo->isMissileCamera ? 23 : 7)] )
    Com_Error(ERR_FATAL, "Renderer attempted to use technique that uses floatz buffer, but it wasn't created.\n");
  return 1;
}

void __cdecl R_SetSceneParms(const refdef_s *refdef, GfxSceneParms *sceneParms)
{
  float specularScale; // eax
  float curAmount; // edx
  bool v4; // [esp+30h] [ebp-Ch]
  unsigned int displayHeight; // [esp+34h] [ebp-8h]
  unsigned int displayWidth; // [esp+38h] [ebp-4h]

  if ( !refdef && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 6832, 0, "%s", "refdef") )
    __debugbreak();
  if ( !sceneParms
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 6833, 0, "%s", "sceneParms") )
  {
    __debugbreak();
  }
  if ( refdef->blurRadius < 0.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          6834,
          0,
          "%s\n\t(refdef->blurRadius) = %g",
          "(refdef->blurRadius >= 0.0f)",
          refdef->blurRadius) )
  {
    __debugbreak();
  }
  sceneParms->localClientNum = refdef->localClientNum;
  sceneParms->blurRadius = refdef->blurRadius;
  memcpy(&sceneParms->dof, &refdef->dof, sizeof(sceneParms->dof));
  memcpy(&sceneParms->film, &refdef->visionset, sizeof(sceneParms->film));
  specularScale = refdef->visionset.charPrimaryLightScale.specularScale;
  sceneParms->charPrimaryLightScale.diffuseScale = refdef->visionset.charPrimaryLightScale.diffuseScale;
  sceneParms->charPrimaryLightScale.specularScale = specularScale;
  memcpy(&sceneParms->bloom, &refdef->visionset.bloom, sizeof(sceneParms->bloom));
  memcpy(&sceneParms->waterSheetingFx, &refdef->waterSheetingFx, sizeof(sceneParms->waterSheetingFx));
  memcpy(&sceneParms->flameFx, &refdef->flameFx, sizeof(sceneParms->flameFx));
  memcpy(&sceneParms->reviveFx, &refdef->visionset.reviveFx, sizeof(sceneParms->reviveFx));
  sceneParms->sunVisibility = refdef->sunVisibility;
  memcpy(&sceneParms->doubleVision, &refdef->doubleVision, sizeof(sceneParms->doubleVision));
  memcpy(
    (unsigned __int8 *)&sceneParms->genericFilter,
    (unsigned __int8 *)&refdef->genericFilter,
    sizeof(sceneParms->genericFilter));
  curAmount = refdef->poisonFx.curAmount;
  sceneParms->poisonFx.curAmountTarget = refdef->poisonFx.curAmountTarget;
  sceneParms->poisonFx.curAmount = curAmount;
  memcpy(&sceneParms->electrifiedFx, &refdef->electrifiedFx, sizeof(sceneParms->electrifiedFx));
  memcpy(&sceneParms->transportedFx, &refdef->transportedFx, sizeof(sceneParms->transportedFx));
  memcpy(&sceneParms->saveScreenFx, &refdef->saveScreenFx, sizeof(sceneParms->saveScreenFx));
  displayWidth = vidConfig.displayWidth;
  displayHeight = vidConfig.displayHeight;
  v4 = refdef->width == vidConfig.displayWidth && refdef->height == vidConfig.displayHeight;
  sceneParms->isRenderingFullScreen = v4;
  sceneParms->playerTeleported = refdef->playerTeleported != 0;
  sceneParms->displayViewport.x = refdef->x;
  sceneParms->displayViewport.y = refdef->y;
  sceneParms->displayViewport.width = refdef->width;
  sceneParms->displayViewport.height = refdef->height;
  sceneParms->sceneViewport.x = (int)((double)(vidConfig.sceneWidth * refdef->x) / (double)displayWidth + 0.5);
  sceneParms->sceneViewport.y = (int)((double)(vidConfig.sceneHeight * refdef->y) / (double)displayHeight + 0.5);
  sceneParms->sceneViewport.width = vidConfig.sceneWidth * (refdef->width + refdef->x) / displayWidth
                                  - sceneParms->sceneViewport.x;
  sceneParms->sceneViewport.height = vidConfig.sceneHeight * (refdef->height + refdef->y) / displayHeight
                                   - sceneParms->sceneViewport.y;
  if ( refdef->useScissorViewport )
  {
    sceneParms->scissorViewport.x = vidConfig.sceneWidth * refdef->scissorViewport.x / displayWidth;
    sceneParms->scissorViewport.y = vidConfig.sceneHeight * refdef->scissorViewport.y / displayHeight;
    sceneParms->scissorViewport.width = vidConfig.sceneWidth
                                      * (refdef->scissorViewport.width + refdef->scissorViewport.x)
                                      / displayWidth
                                      - sceneParms->scissorViewport.x;
    sceneParms->scissorViewport.height = vidConfig.sceneHeight
                                       * (refdef->scissorViewport.height + refdef->scissorViewport.y)
                                       / displayHeight
                                       - sceneParms->scissorViewport.y;
  }
  else
  {
    sceneParms->scissorViewport = sceneParms->sceneViewport;
  }
  sceneParms->primaryLights = refdef->primaryLights;
}

void __cdecl R_CorrectLodScale(const refdef_s *refdef)
{
  R_UpdateLodParms(refdef, &rg.correctedLodParms[refdef->localClientNum]);
}

void __cdecl R_RenderMissileCam(const refdef_s *refdef, int frameTime)
{
  GfxViewParms *v2; // [esp+Ch] [ebp-14F8h]
  GfxSceneParms sceneParms; // [esp+1Ch] [ebp-14E8h] BYREF
  GfxViewParms *viewParms; // [esp+1500h] [ebp-4h]

  if ( refdef->tanHalfFovX <= 0.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          7268,
          0,
          "%s",
          "refdef->tanHalfFovX > 0") )
  {
    __debugbreak();
  }
  if ( refdef->tanHalfFovY <= 0.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          7269,
          0,
          "%s",
          "refdef->tanHalfFovY > 0") )
  {
    __debugbreak();
  }
  if ( !refdef->height
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 7270, 0, "%s", "refdef->height > 0") )
  {
    __debugbreak();
  }
  if ( !refdef->width
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 7271, 0, "%s", "refdef->width > 0") )
  {
    __debugbreak();
  }
  if ( refdef->localClientNum >= gfxCfg.maxClientViews
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          7272,
          0,
          "refdef->localClientNum doesn't index gfxCfg.maxClientViews\n\t%i not in [0, %i)",
          refdef->localClientNum,
          gfxCfg.maxClientViews) )
  {
    __debugbreak();
  }
  if ( rg.registered && !r_norefresh->current.enabled )
  {
    if ( r_logFile->current.integer )
      RB_LogPrint("====== R_RenderMissileCam ======\n");
    if ( !rgp.world )
      Com_Error(ERR_DROP, &byte_D6D070);
    rg.viewOrg[0] = refdef->vieworg[0];
    rg.viewOrg[1] = refdef->vieworg[1];
    rg.viewOrg[2] = refdef->vieworg[2];
    *(_QWORD *)rg.viewDir = *(_QWORD *)&refdef->viewaxis[0][0];
    rg.viewDir[2] = refdef->viewaxis[0][2];
    viewParms = R_AllocViewParms();
    R_SetViewParmsForScene(refdef, viewParms);
    R_MissileCam_SetSceneParms(refdef, &sceneParms);
    R_CorrectLodScale(refdef);
    R_WorldLod_FrameUpdate(scene.def.floatTime, viewParms->origin, refdef->localClientNum);
    if ( scene.def.time != refdef->time
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
            7312,
            0,
            "%s",
            "scene.def.time == refdef->time") )
    {
      __debugbreak();
    }
    if ( !rg.lodParms.valid
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
            7314,
            0,
            "%s",
            "rg.lodParms.valid") )
    {
      __debugbreak();
    }
    if ( r_lockPvs->current.enabled )
      v2 = &lockPvsViewParms;
    else
      v2 = viewParms;
    frontEndDataOut->hideMatureContent = refdef->hideMatureContent;
    frontEndDataOut->splitscreen = refdef->splitscreen;
    frontEndDataOut->extraCamLargeFrameSize = refdef->extraCamLargeFrame;
    R_GenerateSortedDrawSurfs(&sceneParms, v2, viewParms, frameTime, refdef, 1);
    R_UI3D_SetupBackendData_UVConstants(&frontEndDataOut->viewInfo[frontEndDataOut->viewInfoIndex].rbUI3D);
  }
}

void __cdecl R_MissileCam_SetSceneParms(const refdef_s *refdef, GfxSceneParms *sceneParms)
{
  float specularScale; // edx

  if ( !refdef && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 6907, 0, "%s", "refdef") )
    __debugbreak();
  if ( !sceneParms
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp", 6908, 0, "%s", "sceneParms") )
  {
    __debugbreak();
  }
  if ( refdef->blurRadius < 0.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_scene.cpp",
          6909,
          0,
          "%s\n\t(refdef->blurRadius) = %g",
          "(refdef->blurRadius >= 0.0f)",
          refdef->blurRadius) )
  {
    __debugbreak();
  }
  memset((unsigned __int8 *)sceneParms, 0, sizeof(GfxSceneParms));
  sceneParms->localClientNum = refdef->localClientNum;
  specularScale = refdef->visionset.charPrimaryLightScale.specularScale;
  sceneParms->charPrimaryLightScale.diffuseScale = refdef->visionset.charPrimaryLightScale.diffuseScale;
  sceneParms->charPrimaryLightScale.specularScale = specularScale;
  sceneParms->isRenderingFullScreen = 1;
  sceneParms->playerTeleported = refdef->playerTeleported != 0;
  sceneParms->displayViewport.x = 0;
  sceneParms->displayViewport.y = 0;
  sceneParms->displayViewport.width = R_ExtraCam_RenderTargetWidth();
  sceneParms->displayViewport.height = R_ExtraCam_RenderTargetHeight();
  sceneParms->sceneViewport.x = 0;
  sceneParms->sceneViewport.y = 0;
  sceneParms->sceneViewport.width = R_ExtraCam_RenderTargetWidth();
  sceneParms->sceneViewport.height = R_ExtraCam_RenderTargetHeight();
  sceneParms->scissorViewport = sceneParms->sceneViewport;
  sceneParms->primaryLights = refdef->primaryLights;
}

void __cdecl R_LinkDObjEntity(int localClientNum, unsigned int entnum, float *origin, float radius)
{
  R_FilterDObjIntoCells(localClientNum, entnum, origin, radius);
  R_LinkSphereEntityToPrimaryLights(localClientNum, entnum, origin, radius);
}

void __cdecl R_LinkBModelEntity(int localClientNum, unsigned int entnum, GfxBrushModel *bmodel)
{
  R_FilterBModelIntoCells(localClientNum, entnum, bmodel);
  R_LinkBoxEntityToPrimaryLights(localClientNum, entnum, bmodel->writable.mins, bmodel->writable.maxs);
}

void __cdecl R_UnlinkEntity(int localClientNum, unsigned int entnum)
{
  R_UnfilterEntFromCells(localClientNum, entnum);
  R_UnlinkEntityFromPrimaryLights(localClientNum, entnum);
}

void __cdecl R_LinkDynEnt(unsigned int dynEntId, DynEntityDrawType drawType, float *mins, float *maxs)
{
  R_FilterDynEntIntoCells(dynEntId, drawType, mins, maxs);
  R_LinkDynEntToPrimaryLights(dynEntId, drawType, mins, maxs);
}

void __cdecl R_UnlinkDynEnt(unsigned int dynEntId, DynEntityDrawType drawType)
{
  R_UnfilterDynEntFromCells(dynEntId, drawType);
  R_UnlinkDynEntFromPrimaryLights(dynEntId, drawType);
}

void __cdecl R_PvsLock_GetViewOrigin(float *out)
{
  *out = lockPvsViewParms.origin[0];
  out[1] = lockPvsViewParms.origin[1];
  out[2] = lockPvsViewParms.origin[2];
}

void __cdecl R_PvsLock_GetViewAxis(float (*out)[3])
{
  AxisCopy(lockPvsViewParms.axis, out);
}

void __cdecl ShodLodInfo(
        const float *origin,
        int lod,
        int numLods,
        float curDist,
        float lodDist,
        bool lodDistAutoGenerated)
{
  char *s; // [esp+14h] [ebp-4h]

  if ( r_showLodInfo->current.integer )
  {
    s = 0;
    switch ( r_showLodInfo->current.integer )
    {
      case 1:
        goto LABEL_6;
      case 2:
        s = va("%i/%i", lod, numLods);
        break;
      case 3:
        s = va("%.0f\"", curDist);
        break;
      case 4:
        if ( lodDistAutoGenerated )
LABEL_6:
          s = va("%i/%i %.0f/%.0f\"", lod, numLods, curDist, lodDist);
        break;
      default:
        break;
    }
    if ( s )
      R_AddScaledDebugString(
        &frontEndDataOut->debugGlobals,
        rg.debugViewParms,
        origin,
        (const float *)(&(&bad_alloc_Message_482)[2 * numLods])[lodDistAutoGenerated],
        s);
  }
}

void __cdecl R_PerMap_Init()
{
  R_PerMap_DpvsGlobInit();
  r_glob.remoteScreenLastSceneResolveTarget = 0;
}

void __cdecl R_AddXModelDebugString(const float *origin, char *string)
{
  R_AddScaledDebugString(&frontEndDataOut->debugGlobals, rg.debugViewParms, origin, colorCyan, string);
}

