#pragma once

void __cdecl AimAssist_Init(int localClientNum);
const dvar_s *AimAssist_RegisterDvars();
void __cdecl AimAssist_Setup(int localClientNum, const playerState_s *ps);
void __cdecl AimAssist_BackupPlayerState(int localClientNum, const playerState_s *ps);
centity_s *__cdecl CG_GetEntity(int localClientNum, int entityIndex);
char __cdecl AimAssist_PlayerDisabledAutoAim();
void __cdecl AimAssist_UpdateScreenTargets(
        int localClientNum,
        const float *viewOrg,
        const float *viewAngles,
        float tanHalfFovX,
        float tanHalfFovY);
void __cdecl AimAssist_FovScale(AimAssistGlobals *aaGlob, float tanHalfFovY);
// local variable allocation has failed, the output may be wrong!
void  AimAssist_CreateScreenMatrix(
        int a1@<ebp>,
        AimAssistGlobals *aaGlob,
        float tanHalfFovX,
        float tanHalfFovY);
char __cdecl AimAssist_ConvertToClipBounds(
        const AimAssistGlobals *aaGlob,
        const float (*bounds)[3],
        const float (*mtx)[3],
        float (*clipBounds)[3]);
char __cdecl AimAssist_XfmWorldPointToClipSpace(const AimAssistGlobals *aaGlob, float *in, float *out);
double __cdecl AimAssist_GetCrosshairDistSqr(const float *clipMins, const float *clipMaxs);
void __cdecl AimAssist_AddToTargetList(AimAssistGlobals *aaGlob, const AimScreenTarget *screenTarget);
int __cdecl AimAssist_CompareTargets(const AimScreenTarget *screenTargetA, const AimScreenTarget *screenTargetB);
int __cdecl AimAssist_CalcAimPos(
        int localClientNum,
        const centity_s *targetEnt,
        const AimTarget *target,
        float *aimPos);
int __cdecl AimTarget_GetTagPos(int localClientNum, const centity_s *cent, unsigned int tagName, float *pos);
int __cdecl AimAssist_GetScreenTargetCount(int localClientNum);
int __cdecl AimAssist_GetScreenTargetEntity(int localClientNum, unsigned int targetIndex);
void __cdecl AimAssist_ClearEntityReference(int localClientNum, int entIndex);
bool __cdecl AimAssist_IsPrevTargetEntity(int localClientNum, int entIndex);
double __cdecl AngleNormalize180(float angle);
double __cdecl Vec2Length(const float *v);
double __cdecl Vec3Normalize(float *v);
double __cdecl Vec2Normalize(float *v);
void __cdecl AimAssist_UpdateGamePadInput(const AimInput *input, AimOutput *output);
void __cdecl AimAssist_UpdateTweakables(int localClientNum);
void __cdecl AimAssist_UpdateAdsLerp(const AimInput *input);
void __cdecl AimAssist_ApplyTurnRates(const AimInput *input, AimOutput *output);
double __cdecl AimAssist_LerpDvars(const dvar_s *from, const dvar_s *to, float frac);
void __cdecl AimAssist_CalcAdjustedAxis(const AimInput *input, float *pitchAxis, float *yawAxis);
void __cdecl AimAssist_CalcSlowdown(const AimInput *input, float *pitchScale, float *yawScale);
double __cdecl AimAssist_GetAimAssistRange(unsigned int weapIndex, float adsLerp);
const AimScreenTarget *__cdecl AimAssist_GetBestTarget(
        const AimAssistGlobals *aaGlob,
        float range,
        float regionWidth,
        float regionHeight);
bool __cdecl AimAssist_DoBoundsIntersectCenterBox(
        const float *clipMins,
        const float *clipMaxs,
        float clipHalfWidth,
        float clipHalfHeight);
bool __cdecl AimAssist_IsPlayerUsingOffhand(const AimAssistPlayerState *ps);
bool __cdecl AimAssist_IsSlowdownActive(const AimAssistPlayerState *ps);
double __cdecl AimAssist_GetProfileSensitivity();
void __cdecl AimAssist_ApplyAutoAim(const AimInput *input, AimOutput *output);
double __cdecl AimAssist_GetAutoAimRange(unsigned int weapIndex);
void __cdecl AimAssist_ClearAutoAimTarget(AimAssistGlobals *aaGlob);
char __cdecl AimAssist_UpdateAutoAimTarget(AimAssistGlobals *aaGlob);
const AimScreenTarget *__cdecl AimAssist_GetTargetFromEntity(const AimAssistGlobals *aaGlob, int entIndex);
void __cdecl AimAssist_SetAutoAimTarget(AimAssistGlobals *aaGlob, const AimScreenTarget *screenTarget);
void __cdecl AimAssist_ApplyLockOn(const AimInput *input, AimOutput *output);
cg_s *__cdecl CG_GetLocalClientGlobals(int localClientNum);
const AimScreenTarget *__cdecl AimAssist_GetPrevOrBestTarget(
        const AimAssistGlobals *aaGlob,
        float range,
        float regionWidth,
        float regionHeight,
        int prevTargetEnt);
void __cdecl AimAssist_ApplyAutoMelee(const AimInput *input, AimOutput *output);
void __cdecl AimAssist_ClearAutoMeleeTarget(AimAssistGlobals *aaGlob);
char __cdecl AimAssist_UpdateAutoMeleeTarget(AimAssistGlobals *aaGlob, int localClientNum);
void __cdecl AimAssist_SetAutoMeleeTarget(AimAssistGlobals *aaGlob, const AimScreenTarget *screenTarget);
void __cdecl AimAssist_UpdateMouseInput(const AimInput *input, AimOutput *output);
void __cdecl AimAssist_DrawDebugOverlay(int localClientNum);
void __cdecl AimAssist_DrawCenterBox(
        const AimAssistGlobals *aaGlob,
        float clipHalfWidth,
        float clipHalfHeight,
        const float *color);
void __cdecl AimAssist_DrawTargets(int localClientNum, const playerState_s *ps, const float *color);
bool __thiscall bitarray<51>::testBit(bitarray<51> *this, unsigned int pos);
