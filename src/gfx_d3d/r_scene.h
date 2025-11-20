#pragma once

GfxScene *__cdecl R_GetScene();
unsigned int __cdecl R_AllocSceneModel();
int __cdecl R_AllocTextureOverride(
        unsigned __int16 modelIndexMask,
        const GfxImage *img1,
        GfxImage *img2,
        int prevOverride);
int __cdecl R_AllocTextureOverride(
        const Material *material,
        unsigned __int16 modelIndexMask,
        const GfxImage *img1,
        GfxImage *img2,
        int prevOverride);
GfxBrushModel *__cdecl R_GetBrushModel(unsigned int modelIndex);
void __cdecl R_AddBrushModelToSceneFromAngles(
        const GfxBrushModel *bmodel,
        const float *origin,
        const float *angles,
        unsigned __int16 entnum,
        const ShaderConstantSet *constants);
unsigned int __cdecl R_AllocSceneBrush();
void __cdecl R_AddGlassBrushToScene(
        const GfxBrushModel *bmodel,
        const float *origin,
        const float *angles,
        Material *altStreamingMaterial);
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
        float scale);
unsigned int __cdecl R_AllocSceneDObj();
GfxParticleCloud *__cdecl R_AddParticleCloudToScene(Material *material);
void __cdecl R_AddOmniLightToScene(const float *org, const float (*axis)[3], int radius, float r, float g, float b);
void __cdecl R_AddSpotLightToScene(const float *org, const float (*axis)[3], float radius, float r, float g, float b);
double __cdecl R_GetDefaultNearClip();
void __cdecl R_SetupViewProjectionMatrices(GfxViewParms *viewParms, bool offsetmatrix);
void __cdecl R_AddBModelSurfacesCamera(
        BModelDrawInfo *bmodelInfo,
        const GfxBrushModel *bmodel,
        GfxDrawSurf **drawSurfs,
        GfxDrawSurf **lastDrawSurfs,
        unsigned int reflectionProbeIndex,
        float burnAmt,
        float fadeAmt,
        char visLightsMask);
GfxDrawSurf *__cdecl R_AddBModelSurfaces(
        BModelDrawInfo *bmodelInfo,
        const GfxBrushModel *bmodel,
        unsigned __int8 techType,
        GfxDrawSurf *drawSurf,
        GfxDrawSurf *lastDrawSurf);
const MaterialTechnique *__cdecl Material_GetTechnique(const Material *material, unsigned __int8 techType);
MaterialTechniqueSet *__cdecl Material_GetTechniqueSet(const Material *material);
void __cdecl R_SetEnablePlayerShadowFlag(bool flag);
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
        char visLightsMask);
const XSurface *__cdecl R_GetXSurface(unsigned int *modelSurf, surfaceType_t surfType);
GfxDrawSurf *__cdecl R_AddXModelSurfaces(
        XModelDrawInfo *modelInfo,
        const XModel *model,
        unsigned __int8 techType,
        GfxDrawSurf *drawSurf,
        GfxDrawSurf *lastDrawSurf);
void __cdecl R_AddDObjSurfacesCamera(
        GfxSceneEntity *sceneEnt,
        unsigned __int16 lightingHandle,
        unsigned __int8 primaryLightIndex,
        GfxDrawSurf **drawSurfs,
        GfxDrawSurf **lastDrawSurfs,
        char visLightsMask,
        bool isMissileCamera);
GfxDrawSurf *__cdecl R_AddDObjSurfaces(
        GfxSceneEntity *sceneEnt,
        unsigned __int8 techType,
        GfxDrawSurf *drawSurf,
        GfxDrawSurf *lastDrawSurf);
void __cdecl R_SetCharacterPrimaryLightScaleInfo(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms);
bool __cdecl R_EndFencePending();
bool __cdecl R_DynamicFencePending();
void __cdecl R_SetEndTime(int endTime);
void __cdecl R_WaitEndTime();
void __cdecl R_SetSunConstants(GfxCmdBufInput *input);
void __cdecl R_SetInputCodeConstant(GfxCmdBufInput *input, unsigned int constant, float x, float y, float z, float w);
void __cdecl R_SetInputCodeConstantFromVec4(GfxCmdBufInput *input, unsigned int constant, float *value);
void  R_SetHDRControlConstants(float a1@<ebp>, GfxCmdBufInput *input, const GfxViewInfo *viewInfo);
void __cdecl R_GenerateMarkVertsForDynamicModels(const GfxLight *visibleLights, int visibleLightCount);
void  R_SetSkyDynamicIntensity(float a1@<ebp>, const float *viewForward, GfxCmdBufInput *input);
void __cdecl R_InitScene();
void __cdecl R_ClearScene(int localClientNum);
void R_ClearSceneDynSModelVisBits();
void __cdecl R_ClearScene_For_ExtraCam(int localClientNum);
int __cdecl R_GetLocalClientNum();
void __cdecl R_SetLodOrigin(const refdef_s *refdef);
void __cdecl R_UpdateFrameFog(unsigned int localClientNum);
double __cdecl lerp(float from, float to, float t);
void __cdecl R_SetViewParmsForScene(const refdef_s *refdef, GfxViewParms *viewParms);
void __cdecl R_SetupProjection(float tanHalfFovX, float tanHalfFovY, GfxViewParms *viewParms);
GfxBackEndData *R_UpdateFrameSun();
void __cdecl R_LerpDir(
        const float *dirBegin,
        const float *dirEnd,
        int beginLerpTime,
        int endLerpTime,
        int currTime,
        float *result);
void __cdecl R_UpdateLodParms(const refdef_s *refdef, GfxLodParms *lodParms);
void __cdecl R_RenderScene(refdef_s *refdef, int frameTime);
void __cdecl R_CheckShadowMapVolumes(const float *position);
void __cdecl R_UpdateExposureValue(refdef_s *refdef);
unsigned int __cdecl R_CheckExposureVolumes(float *position, float *feather);
void __cdecl R_GenerateSortedDrawSurfs(
        const GfxSceneParms *sceneParms,
        const GfxViewParms *viewParmsDpvs,
        const GfxViewParms *viewParmsDraw,
        int frameTime,
        const refdef_s *refdef,
        bool forMissileCamView);
bool __cdecl R_GetAllowShadowMaps();
bool __cdecl R_DynamicShadowType();
void __cdecl R_SetSceneComposition(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms, bool extraCam);
void __cdecl R_SetExposure(GfxViewInfo *viewInfo, const refdef_s *refdef);
void __cdecl R_SetDepthOfField(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms);
void __cdecl R_SetDoubleVision(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms);
void __cdecl R_SetWaterSheetingFx(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms);
void __cdecl R_SetFlameFx(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms);
void __cdecl R_SetElectrifiedFx(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms);
void __cdecl R_SetTransportedFx(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms);
void __cdecl R_SetReviveFx(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms);
void __cdecl R_SetGenericFilter(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms);
void __cdecl R_SetPoisonFx(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms);
void __cdecl R_SetFilmInfo(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms);
void __cdecl R_SetBloomInfo(GfxViewInfo *viewInfo, const GfxSceneParms *sceneParms);
void __cdecl R_SetFullSceneViewMesh(int viewInfoIndex, GfxViewInfo *viewInfo);
void __cdecl R_GenerateSortedSunShadowDrawSurfs(GfxViewInfo *viewInfo);
void __cdecl R_AddEmissiveSpotLight(GfxViewInfo *viewInfo);
int __cdecl R_GetVisibleDLights(GfxLight *visibleLights);
void __cdecl R_GetLightSurfs(const GfxLight *visibleLights, int visibleLightCount);
void __cdecl R_GetPointLightShadowSurfs(
        GfxViewInfo *viewInfo,
        GfxVisibleLight *visibleLights,
        const GfxLight *lights,
        int numLights);
void __cdecl R_SetSunShadowConstants(GfxCmdBufInput *input, const GfxSunShadowProjection *sunProj);
// local variable allocation has failed, the output may be wrong!
void  R_SetHeroLighting(float a1@<ebp>, GfxCmdBufInput *input, GfxViewInfo *viewInfo);
void __cdecl R_SetDLightsConstantsDefaults(GfxCmdBufInput *input);
void __cdecl R_SetInputCodeImageSamplerState(
        GfxCmdBufInput *input,
        unsigned int codeTexture,
        unsigned __int8 samplerState);
// local variable allocation has failed, the output may be wrong!
void  R_SetDLightsConstants(
        float a1@<ebp>,
        GfxCmdBufInput *input,
        const GfxViewInfo *viewInfo,
        const GfxLight *visibleLights,
        int visibleLightCount);
void R_DrawCineWarning();
void __cdecl R_SplitDrawSurfacesPrimarySortKey(
        GfxDrawSurfListInfo *srcList,
        GfxDrawSurfListInfo *destList,
        int sortkeyID);
void __cdecl DrawLightDebug(const GfxViewInfo *viewInfo, const GfxLight *L, const float *debugColor);
void DrawOutdoorBoundsVolumeDebug();
void __cdecl DrawSunDirectionDebug(const float *viewOrg, const float *viewForward);
unsigned intR_SortAllCodeMeshSurfacesSunShadow();
void R_DrawFogParams();
unsigned intR_WaitForFXUpdateWorkerCmds();
void __cdecl R_FinishDecalAndEmissiveDrawSurfs(
        GfxViewInfo *viewInfo,
        const GfxViewParms *viewParmsDraw,
        GfxBackEndData *backendData,
        bool forMissileCamView);
unsigned __int8 __cdecl R_GetEmissiveTechnique(const GfxViewInfo *viewInfo, unsigned __int8 baseTech);
char __cdecl R_DoesDrawSurfListInfoNeedFloatz(GfxViewInfo *viewInfo, GfxDrawSurfListInfo *emissiveInfo);
void __cdecl R_SetSceneParms(const refdef_s *refdef, GfxSceneParms *sceneParms);
void __cdecl R_CorrectLodScale(const refdef_s *refdef);
void __cdecl R_RenderMissileCam(const refdef_s *refdef, int frameTime);
void __cdecl R_MissileCam_SetSceneParms(const refdef_s *refdef, GfxSceneParms *sceneParms);
void __cdecl R_LinkDObjEntity(int localClientNum, unsigned int entnum, float *origin, float radius);
void __cdecl R_LinkBModelEntity(int localClientNum, unsigned int entnum, GfxBrushModel *bmodel);
void __cdecl R_UnlinkEntity(int localClientNum, unsigned int entnum);
void __cdecl R_LinkDynEnt(unsigned int dynEntId, DynEntityDrawType drawType, float *mins, float *maxs);
void __cdecl R_UnlinkDynEnt(unsigned int dynEntId, DynEntityDrawType drawType);
void __cdecl R_PvsLock_GetViewOrigin(float *out);
void __cdecl R_PvsLock_GetViewAxis(float (*out)[3]);
void __cdecl ShodLodInfo(
        const float *origin,
        int lod,
        int numLods,
        float curDist,
        float lodDist,
        bool lodDistAutoGenerated);
void __cdecl R_PerMap_Init();
void __cdecl R_AddXModelDebugString(const float *origin, char *string);
