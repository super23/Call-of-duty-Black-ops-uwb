#pragma once

int __cdecl UI_GameVariants_GetClassParameters();
void __cdecl UI_GV_StartAddingEvent_f();
void __cdecl UI_GV_StartEditingFeeder(int localClientNum, int feederId, int feederCount);
void __cdecl UI_GV_StartAddingAction_f();
void __cdecl UI_GV_StartAddingParam_f();
void __cdecl UI_GV_StartAddingTarget_f();
void __cdecl UI_GV_StartAddingConditional_f();
void SetNextPopupToAddOrEditConfirm();
void __cdecl UI_GV_EditSelectedRule_f();
GVGlob *__cdecl GetSelectedRule(int localClientNum);
GVGlob *__cdecl GetRuleForFeederIndex(int feederIndex);
void __cdecl UI_GV_UpdateRuleFromDvars(GVRule *rule);
int __cdecl GetEventIndexFromFeederIndex(int feederIndex);
bool __cdecl IsEventValid(GVEvent *ev);
bool __cdecl IsFreeForAllMode();
char __cdecl AreCustomClassesAvailable();
int __cdecl GetActionIndexFromFeederIndex(int feederIndex);
bool __cdecl IsActionValid(GVAction *action);
int __cdecl GetTargetIDFromFeederIndex(int feederIndex);
GVEvent *__cdecl GetEventByDvarIndex(const char *dvarName);
unsigned __int8 __cdecl GetValidTargetMask(unsigned __int8 targetMask);
GVConditional *__cdecl GetConditionalForDvarIndex(
        GVConditional *result,
        const char *lhsDvarIndexName,
        const char *opDvarIndexName,
        const char *rhsDvarIndexName);
unsigned int __cdecl GetCondLhsIndexForFeederIndex(int feederIndex);
unsigned __int8 __cdecl GetValidCondMask(unsigned __int8 condMask);
void __cdecl UI_GV_SetRuleParameterFromDisplayDvars(GVRule *rule);
const char *__cdecl GetParamValueForDvarIndex(int actionIndex, const char *paramIndexDvarName);
const char *__cdecl GetPerkParamValueFromDvarIndex(GVRule *rule, const char *paramDvarIndexName);
void __cdecl UI_GV_AddNewRule_f();
int __cdecl UI_GV_GetFirstInvalidRuleIndex();
void __cdecl UI_GV_DeleteSelected_f();
void __cdecl UI_GV_NavForward_f();
void __cdecl UI_GV_NavBackward_f();
void __cdecl UI_GV_UpdateSelectedRule_f();
void __cdecl UI_InitGametypeVariants();
void __cdecl UI_GV_ResetFeeder_f();
int __cdecl UI_GetGVRulesCount();
int __cdecl UI_GetGVEventCount();
int __cdecl GetValidEventCount();
int __cdecl UI_GetGVActionCount();
int __cdecl GetValidActionCount();
int __cdecl UI_GetGVTargetCount();
int __cdecl GetValidTargetCount(unsigned __int8 targetMask);
int __cdecl UI_GetGVParamCount();
GVAction *__cdecl GetActionByDvarIndex(const char *dvarName);
int __cdecl UI_GetGVCondLhsCount();
int __cdecl GetValidCondLhsCount(unsigned __int8 condMask);
int __cdecl UI_GetGVCondOpCount();
GVConditionalLhs *__cdecl GetCondLhsForDvarIndex(const char *lhsDvarIndexName);
GVConditionalLhs *__cdecl GetCondLhsForFeederIndex(int feederIndex);
int __cdecl UI_GetGVCondRhsCount();
int __cdecl UI_FeederCount_GameVariants(int contextIndex, float feederID);
char *__cdecl GetItemGroupDisplayValue(itemGroup_t itemGroup, const char *value);
char *__cdecl UI_FeederItemText_GameVariantRules(int index, int column);
char *__cdecl UI_GV_GetRuleParamDisplayValue(GVRule *rule);
int __cdecl GetParamIndexForValue(int actionIndex, const char *paramValue);
char *__cdecl UI_FeederItemText_GameVariantEvents(int index, int column);
char *__cdecl UI_FeederItemText_GameVariantActions(int index, int column);
char *__cdecl UI_FeederItemText_GameVariantTargets(int index, int column);
GVTarget *__cdecl GetTargetAtFeederIndex(int feederIndex);
char *__cdecl UI_FeederItemText_GameVariantParams(int index, int column);
char *__cdecl UI_FeederItemText_GameVariantCondLhs(int index, int column);
char *__cdecl UI_FeederItemText_GameVariantCondOp(int index, int column);
char *__cdecl UI_FeederItemText_GameVariantCondRhs(int index, int column);
char *__cdecl UI_FeederItemText_GameVariants(float feederID, int index, int column);
void __cdecl UI_FeederSelection_GameVariantRules(int contextIndex, int index);
int __cdecl GetFeederIndexFromEventIndex(int eventIndex);
int __cdecl GetFeederIndexFromActionIndex(int actionIndex);
int __cdecl GetFeederIndexFromTargetId(int targetId, unsigned __int8 targetMask);
int __cdecl GetFeederIndexForCondLhsIndex(int condLhsIndex, unsigned __int8 condMask);
void __cdecl UI_FeederSelection_GameVariantEvent(int contextIndex, int index);
void __cdecl UI_FeederSelection_GameVariantAction(int localClientNum, int contextIndex, int index);
void __cdecl UI_FeederSelection_GameVariantTarget(int contextIndex, int index);
void __cdecl UI_FeederSelection_GameVariantCondOp(int contextIndex, int index);
void __cdecl UI_FeederSelection_GameVariantCondRhs(int contextIndex, int index);
void __cdecl UI_FeederSelection_GameVariants(int localClientNum, int contextIndex, float feederID, int index);
void __cdecl UI_FeederSelection_GameVariantParam(int localClientNum, int contextIndex, int index);
void __cdecl UI_FeederSelection_GameVariantCondLhs(int localClientNum, int contextIndex, int index);
void __cdecl UI_GameVariants_ArchiveRule(MemoryFile *memFile, GVRule *rule);
void __cdecl MemFile_ArchiveByte(MemoryFile *memFile, unsigned __int8 *value);
void __cdecl UI_GameVariants_ArchiveParameter(MemoryFile *memFile, GVParameter *param);
unsigned int __cdecl UI_GameVariants_GetRuleSize(const GVRule *rule);
void __cdecl UI_GameVariants_WriteRuleToMemoryFile(MemoryFile *memFile, GVRule *rule);
void __cdecl MemFile_WriteUInt(MemoryFile *memFile, unsigned int value);
void __cdecl UI_GameVariants_WriteToMemoryFile(MemoryFile *memFile);
void __cdecl UI_GameVariants_Reset();
char __cdecl UI_GameVariants_ReadGameVariantRuleChunk(MemoryFile *memFile);
void __cdecl Scr_GameVariants_AddConditional(GVRule *rule);
void __cdecl Scr_GameVariants_AddParams(GVRule *rule);
void __cdecl Scr_GameVariants_AddPerkString(char *paramValue);
void __cdecl Scr_GameVariants_GetRule();
GVGlob *__thiscall GVGlob::GVGlob(GVGlob *this);
GVAction *__thiscall GVAction::GVAction(
        GVAction *this,
        char *name,
        char *scriptName,
        unsigned __int8 parameterType,
        GVValue *parameters,
        int parametersSize);
