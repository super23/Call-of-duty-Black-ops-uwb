#pragma once

void __cdecl CG_DrawWaterTrail(int localClientNum, const float *pos, float waterHeight);
int __cdecl CG_GetNextRippleTime(centity_s *cent);
void __cdecl CG_SetNextRippleTime(centity_s *cent, int time, float speed, bool in_water);
int __cdecl CG_GetNextWaterCheck(centity_s *cent);
void __cdecl CG_SetNextWaterCheck(centity_s *cent, int time, bool in_water);
void __cdecl CG_SetCentWaterHeight(centity_s *cent, float height);
double __cdecl CG_GetCentWaterHeight(centity_s *cent);
void __cdecl CG_DrawWaterTrail(int localClientNum, centity_s *cent);
void __cdecl CG_DrawWaterTrail(int localClientNum);
void __cdecl CG_FxSetTestPosition();
void __cdecl CG_FxTest();
void __cdecl CG_PlayTestFx(int localClientNum);
void __cdecl CG_ViewRegisterDvars();
void __cdecl CG_UpdateTurretKillCam(int localClientNum);
void __cdecl CG_GetPlayerKillCamEntityLookAt(int localClientNum, float *lookAtPos);
void __cdecl CG_UpdateTurretKillCam(float distance, GfxDepthOfField *dof);
void __cdecl CG_GetTurretEntityOrgAngles(int localClientNum, float *origin, float *angles);
void __cdecl CG_UpdateVehicleKillCam(int localClientNum);
void __cdecl Vec3Copy(const float *from, float *to);
void __cdecl CG_GetKillCamEntityOrgAngles(int localClientNum, float *origin, float *angles);
const vehicle_info_t *__cdecl CG_GetKillCamVehicleInfo(int localClientNum);
void __cdecl CG_UpdateVehicleKillCamDof(const vehicle_info_t *info, float distance, GfxDepthOfField *dof);
void __cdecl CG_UpdateScriptedKillCam(int localClientNum);
double __cdecl LookAtAxisAndDistance(const float *origin, const float *lookAt, float (*axis)[3]);
void __cdecl CG_UpdateScriptedKillCamDof(float distance, GfxDepthOfField *dof);
void __cdecl CG_UpdateDestructibleKillCam(int localClientNum);
void __cdecl CG_UpdateDestructibleKillCamDof(float distance, GfxDepthOfField *dof);
void __cdecl CG_UpdateArtilleryKillCam(int localClientNum);
void __cdecl CG_UpdateDogKillCam(int localClientNum);
void __cdecl Vec3Sub(const float *a, const float *b, float *diff);
bool __cdecl CG_KillCamEntityEnabled(int localClientNum);
void __cdecl CG_InitView(int localClientNum);
void __cdecl LerpKillCamView(int localClientNum);
void __cdecl CG_CalcViewValues(int localClientNum);
void __cdecl CG_DevSaveCamera(float *angles, float *origin);
void  CG_OffsetChaseCamView(clientActive_t *a1@<ebp>, int localClientNum, CameraMode camMode);
// local variable allocation has failed, the output may be wrong!
void  CG_OffsetThirdPersonView(DObj *a1@<ebp>, int localClientNum);
void __cdecl CG_CalcVrect(int localClientNum);
void __cdecl CG_SmoothCameraZ(cg_s *cgameGlob);
void __cdecl CG_OffsetFirstPersonView(cg_s *cgameGlob);
void __cdecl CG_CalcCubemapViewValues(cg_s *cgameGlob);
void __cdecl CG_CalcTurretViewValues(int localClientNum);
void __cdecl CG_UpdateKillCamEntity(KillCamEntityType killCamEntityType, int localClientNum);
void __cdecl CG_UpdateHelicopterKillCam(KillCamEntityType killCamEntityType, int localClientNum);
void __cdecl CG_UpdateHelicopterKillCamDof(float distance, GfxDepthOfField *dof);
void __cdecl CG_UpdateExplosiveKillCam(int localClientNum, KillCamEntityType killCamEntityType);
double __cdecl LookAtBothPoints(const float *point1, const float *point2, const float *campos, float (*lookaxis)[3]);
void __cdecl CG_KillcamCameraTrace(
        const float *originalBombOrigin,
        const float *bombAngles,
        const float *desiredCamPos,
        int passEntIndex,
        KillCamEntityRestState killCamEntityRestState,
        float *campos);
void __cdecl RewindTrace(trace_t *trace, const float *start, const float *end, float amount);
void __cdecl DebugTraceLine(float *start, float *end, float frac, const float *color);
bool __cdecl CG_ShouldRenderThirdPerson(CameraMode camMode);
void __cdecl CG_UpdateThirdPerson(int localClientNum, bool forExtraCam);
const ClientViewParams *__cdecl CG_GetLocalClientViewParams(int localClientNum);
const ClientViewParams *__cdecl CG_GetLocalClientViewParams(
        int localClientNum,
        int activeClientIndex,
        int activeClientCountArrayIndex);
void __cdecl GetCeilingHeight(cg_s *cgameGlob);
void __cdecl CG_UpdateViewOffset(int localClientNum);
void __cdecl CG_UpdateKillCamEntityViewOffset(int localClientNum);
void __cdecl CG_UpdateRemoteControlEntityViewOffset(int localClientNum);
void __cdecl CG_SetupFlameFX(int localClientNum, int enable);
void __cdecl CG_SetupElectrifiedFX(int localClientNum, int enable);
bool __cdecl CG_IsInfrared(int localClientNum);
bool __cdecl CG_IsTvguided(int localClientNum, bool onlyADS);
int  CG_DrawActiveFrame@<eax>(
        int a1@<esi>,
        int localClientNum,
        int serverTime,
        DemoType demoType,
        CubemapShot cubemapShot,
        int cubemapSize,
        int renderScreen);
void __cdecl CG_UpdateTestFX(int localClientNum);
void __cdecl CG_KickAngles(cg_s *cgameGlob);
void __cdecl UpdateKillCamEntityCache(int localClientNum);
void __cdecl CG_CacheKillCamEntityPose(int localClientNum);
void __cdecl CG_CacheKillCamLookAtEntityOrigin(int localClientNum);
void __cdecl CG_UpdateEntInfo(int localClientNum);
void __cdecl CG_UpdateIKTiming(int localClientNum);
void __cdecl DumpAnims(int localClientNum);
void __cdecl DrawShellshockBlend(int localClientNum);
void __cdecl CG_UpdateTimeScale(int localClientNum);
void __cdecl CG_UpdateSceneDepthOfField(int localClientNum);
void __cdecl CG_UpdateAdsDof(int localClientNum, GfxDepthOfField *dof);
double __cdecl CG_UpdateAdsDofValue(float currentValue, float targetValue, float maxChange, float dt);
bool __cdecl ShouldDoDefaultAdsDof(int localClientNum);
void __cdecl CG_UpdatePoisonFX(cg_s *cgameGlob);
void __cdecl CG_UpdateGenericFilter(cg_s *cgameGlob);
void __cdecl CG_UpdateWaterSheetingFX(cg_s *cgameGlob);
void __cdecl CG_UpdateFlameFX(cg_s *cgameGlob);
void __cdecl CG_UpdateElectrifiedFX(cg_s *cgameGlob);
void __cdecl CG_ValidateWeaponSelect(cg_s *cgameGlob);
void __cdecl CG_PickValidWeaponSelect(cg_s *cgameGlob);
int __cdecl CG_DrawExtraCamFrame(
        int localClientNum,
        int serverTime,
        DemoType demoType,
        CubemapShot cubemapShot,
        int cubemapSize,
        int renderScreen);
void __cdecl CG_CalcViewValues_ExtraCam(int localClientNum);
void __cdecl CG_CalcVrect_ExtraCam(int localClientNum);
void __cdecl CG_ClearSaveScreenFx(int localClientNum);
void __cdecl CG_SaveScreen(int localClientNum);
void __cdecl CG_SaveScreenSection(float viewX, float viewY, float viewWidth, float viewHeight, int screenTimerId);
void __cdecl CG_BlendSavedScreenShockBlurred(
        int fadeMsec,
        float viewX,
        float viewY,
        float viewWidth,
        float viewHeight,
        int screenTimerId);
void __cdecl CG_BlendSavedScreenShockFlashed(
        int localClientNum,
        float intensityWhiteout,
        float intensityScreengrab,
        float viewX,
        float viewY,
        float viewWidth,
        float viewHeight);
void __cdecl CG_ResetFullscreenFX(int localClientNum);
