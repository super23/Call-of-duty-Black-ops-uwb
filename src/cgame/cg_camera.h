#pragma once

void __cdecl InterpolateAnglesSmooth(float *curAngles, float *initialAngles, float *targetAngles, float t);
void __cdecl InterpolatePositionSmooth(float *curPos, const float *initialPos, const float *targetPos, float t);
void __cdecl CG_StartCameraTween(int localClientNum, float tweenTime);
double __cdecl CG_UpdateCameraTweenFOV(int localClientNum, float currentFov);
void __cdecl CG_UpdateCameraTween(int localClientNum);
void __cdecl CG_UpdateVehicleInitView(int localClientNum, CameraMode newMode);
void __cdecl CG_UpdateCameraTransition(int localClientNum, CameraMode oldMode, CameraMode newMode, bool useTagCamera);
bool __cdecl CG_ShouldStartCameraTween(int localClientNum, float tweenTime);
void __cdecl CG_UpdateVehicleBindings(int localClientNum);
int __cdecl CG_RemapVehicleButton(int localClientNum, int *twokeys, int button);
bool __cdecl CG_RenderPlayerFromMissilePOV(int localClientNum);
bool __cdecl CG_ExtraCamViewActive(int localClientNum);
bool __cdecl ShouldDoCameraTransition(cg_s *cgameGlob, CameraMode prevMode, CameraMode newMode, bool useTagCamera);
CameraMode __cdecl CG_UpdateCameraMode(int localClientNum);
int __cdecl CG_UpdateExtraCamMode(int localClientNum);
void __cdecl CG_ApplyViewAnimation(int localClientNum);
ViewModelInfo *__cdecl CG_GetLocalClientViewModelInfo(int localClientNum);
double __cdecl CG_GetViewFov(int localClientNum);
bool __cdecl CG_ShouldDefaultViewFov(int localClientNum);
void __cdecl CG_CalcFov(int localClientNum, float fov_x);
void __cdecl CG_CalculateGunnerOffset_Sway(
        const float *playerViewAngles,
        unsigned int weapon,
        float *swayViewAngles,
        float *swayOffset,
        float *swayAngles,
        float ssSwayScale,
        int frametime);
void __cdecl CG_OffsetVehicleGunner(int localClientNum, cg_s *cgameGlob);
void __cdecl Vec3Lerp(const float *start, const float *end, float fraction, float *endpos);
void __cdecl CG_OffsetVehicleAnimCam(int localClientNum);
void __cdecl CG_GetExtraCamOrigin(int localClientNum, float *out);
void __cdecl CG_CalcExtraCamViewValues(int localClientNum);
void __cdecl CG_CalcMissileViewValues(int localClientNum);
void __cdecl CG_GetMissileViewOrigin(int localClientNum, float *out);
void __cdecl CG_CalcMissileAngleValues(int localClientNum);
void __cdecl CG_CalcVehicleViewValues(int localClientNum);
void __cdecl CG_Calc3rdPersonSpringDamp(
        float *currPos,
        float *trgPos,
        float *prevTrgPos,
        float deltaTime,
        float springConst,
        float dampConst,
        float springLen,
        float *newPos);
double __cdecl GetPitchOffsetRelativeToADirection(float *angles, float *direction);
void __cdecl CG_Calc3rdPersonVehicleViewValues(int localClientNum);
double __cdecl Vec3Distance(const float *v1, const float *v2);
void __cdecl RotatePoint(const float *v, const float *q, float *out);
void __cdecl CG_OffsetVehicleView(int localClientNum, CameraMode camMode);
double __cdecl ThirdPersonViewTrace(
        cg_s *cgameGlob,
        const float *start,
        const float *end,
        int contentMask,
        float *result,
        bool pitch_up,
        bool ignoreStartSolid);
void __cdecl Vec3NormalizeFast(float *v);
void __cdecl ThirdPerson_TraceCalcEndpos(const float *start, const float *end, float fraction, float *endpos);
void __cdecl MovieCameraViewTrace(int localClientNum, int contentMask);
int __cdecl CG_ScrCamActive(int localClientNum);
void __cdecl CG_ScrCamCalcView(int localClientNum);
void __cdecl ScrCam_CalcOrientOrigin(const float *lookAtVec, cg_s *cgameGlob);
void __cdecl ScrCam_CalcOrientDir(float *lookAtVec, cg_s *cgameGlob);
void __cdecl CG_MovieCamCalcView(int localClientNum);
void __cdecl CG_RadiantCamCalcView(int localClientNum);
void __cdecl CG_ExtraCamDebug_SetView(int localClientNum);
void __cdecl CG_ExtraCamDebug_SaveView(int localClientNum);
void __cdecl CG_CalcFov_ExtraCam(int localClientNum);
void __cdecl CG_ExtraCam_GetViewOrigin(int localClientNum, float *out);
bool __cdecl CG_ExtraCamIsActive(int localClientNum);
clientActive_t *__cdecl CL_GetLocalClientGlobals(int localClientNum);
int __cdecl CL_GetLocalClientConnectionState(int localClientNum);
ExtraCamClientStateRestore *__thiscall ExtraCamClientStateRestore::ExtraCamClientStateRestore(
        ExtraCamClientStateRestore *this,
        int localClientNum);
void __thiscall ExtraCamClientStateRestore::~ExtraCamClientStateRestore(ExtraCamClientStateRestore *this);
void __cdecl RotatePoint(float *point, const float (*mat)[3]);
