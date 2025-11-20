#pragma once

void __cdecl CG_RegisterVisionSetsDvars();
void __cdecl CG_InitVisionSetsMenu();
void __cdecl CG_AddVisionSetMenuItem(XAssetHeader header);
void __cdecl CG_InitVisionSets(int localClientNum);
double __cdecl CalculateFlareRatio(float flareRatio, float flareRatioLastFrame);
void __cdecl CG_VisionSetsUpdate(int localClientNum);
void __cdecl UpdateVarsLerpCustom(
        float flareRatio,
        const visionSetVars_t *from,
        const visionSetVars_t *to,
        visionSetLerpData_t *lerpData,
        visionSetVars_t *result);
bool __cdecl LerpBool(bool from, bool to, float fraction, visionSetLerpStyle_t style);
int __cdecl LerpInteger(int from, int to, float fraction, visionSetLerpStyle_t style);
double __cdecl LerpStepFloat(float from, float to, float fraction, visionSetLerpStyle_t style);
double __cdecl LerpFloat(float from, float to, float fraction, visionSetLerpStyle_t style);
void __cdecl LerpVec3(float *from, float *to, float fraction, visionSetLerpStyle_t style, float *result);
void __cdecl LerpVec4(float *from, float *to, float fraction, visionSetLerpStyle_t style, float *result);
void __cdecl UpdateVarsLerp(
        int time,
        const visionSetVars_t *from,
        const visionSetVars_t *to,
        visionSetLerpData_t *lerpData,
        visionSetVars_t *result);
char __cdecl VisionSetCurrent(int localClientNum, visionSetMode_t mode, const char *name);
char __cdecl GetVisionSet(int localClientNum, const char *name, visionSetVars_t *resultSettings);
char __cdecl LoadVisionFile(const char *name, visionSetVars_t *resultSettings);
char *__cdecl RawBufferOpen(const char *name, const char *formatFullPath);
char __cdecl LoadVisionSettingsFromBuffer(const char *buffer, const char *filename, visionSetVars_t *settings);
bool __cdecl ApplyTokenToField(unsigned int fieldNum, char *token, visionSetVars_t *settings);
bool __cdecl CG_VisionSetStartLerp_To(
        int localClientNum,
        visionSetMode_t mode,
        visionSetLerpStyle_t style,
        const char *nameTo,
        int duration);
void __cdecl SetDefaultVision(int localClientNum);
void __cdecl CG_VisionSetConfigString_Naked(int localClientNum);
void __cdecl CG_VisionSetConfigString_Night(int localClientNum);
void __cdecl CG_VisionSetMyChanges();
void __cdecl CG_VisionSetUpdateTweaksFromFile_Film();
bool __cdecl LoadVisionFileForTweaks(visionSetVars_t *setVars);
void __cdecl CG_VisionSetUpdateTweaksFromFile_CharPrimary();
void __cdecl CG_VisionSetUpdateTweaksFromFile_Bloom();
bool __cdecl CG_InFlareVision(int localClientNum);
char __cdecl CG_LookingThroughNightVision(int localClientNum);
void __cdecl DrawVisionSetDebug(int localClientNum, visionSetMode_t curChannel);
void __cdecl CG_VisionSetApplyToRefdef(int localClientNum, bool isExtracam);
void __cdecl CG_InitExposure(int localClientNum, char *mapname);
