#pragma once

void __cdecl Scr_DumpScriptThreads(scriptInstance_t inst);
int __cdecl ThreadInfoCompare(unsigned int *info1, unsigned int *info2);
void __cdecl Scr_DumpScriptVariables(
        scriptInstance_t inst,
        bool spreadsheet,
        bool summary,
        bool total,
        bool functionSummary,
        bool lineSort,
        const char *fileName,
        const char *functionName,
        int minCount);
int __cdecl VariableInfoFileNameCompare(unsigned int *info1, unsigned int *info2);
int __cdecl VariableInfoFunctionCompare(unsigned int *info1, unsigned int *info2);
int __cdecl VariableInfoCountCompare(unsigned int *info1, unsigned int *info2);
int __cdecl VariableInfoFileLineCompare(unsigned int *info1, unsigned int *info2);
void __cdecl Scr_DumpScriptVariablesDefault(scriptInstance_t inst);
void __cdecl Scr_InitVariables(scriptInstance_t inst);
void __cdecl Scr_InitVariableRange(scriptInstance_t inst, unsigned int begin, unsigned int end);
void __cdecl Scr_InitClassMap(scriptInstance_t inst);
void __cdecl Scr_ShutdownVariables(scriptInstance_t inst);
void __cdecl Scr_CheckLeaks(scriptInstance_t inst);
void __cdecl Scr_CheckLeakRange(scriptInstance_t inst, unsigned int begin, unsigned int end);
int __cdecl Scr_GetNumScriptVars(scriptInstance_t inst);
int __cdecl Scr_GetNumScriptVarsParent(scriptInstance_t inst);
int __cdecl Scr_GetNumScriptVarsChild(scriptInstance_t inst);
unsigned int __cdecl GetVariableKeyObject(scriptInstance_t inst, unsigned int id);
unsigned int __cdecl GetVariableIndexInternal(scriptInstance_t inst, unsigned int parentId, unsigned int name);
unsigned int __cdecl GetNewVariableIndexInternal2(
        scriptInstance_t inst,
        unsigned int parentId,
        unsigned int name,
        unsigned int index);
unsigned int __cdecl GetNewVariableIndexInternal3(
        scriptInstance_t inst,
        unsigned int parentId,
        unsigned int name,
        unsigned int index);
void __cdecl ClearObject(scriptInstance_t inst, unsigned int parentId);
void __cdecl ClearObjectInternal(scriptInstance_t inst, unsigned int parentId);
void __cdecl MakeVariableExternal(scriptInstance_t inst, unsigned int index, VariableValueInternal *parentValue);
void __cdecl FreeChildValue(scriptInstance_t inst, unsigned int parentId, unsigned int id);
void __cdecl Scr_SetThreadNotifyName(scriptInstance_t inst, unsigned int startLocalId, unsigned int stringValue);
void __cdecl Scr_StopThread(scriptInstance_t inst, unsigned int threadId);
void __cdecl Scr_ClearThread(scriptInstance_t inst, unsigned int parentId);
void __cdecl Scr_RemoveThreadNotifyName(scriptInstance_t inst, unsigned int startLocalId);
unsigned int __cdecl Scr_GetThreadNotifyName(scriptInstance_t inst, unsigned int startLocalId);
void __cdecl Scr_SetThreadWaitTime(scriptInstance_t inst, unsigned int startLocalId, unsigned int waitTime);
void __cdecl Scr_ClearWaitTime(scriptInstance_t inst, unsigned int startLocalId);
unsigned int __cdecl Scr_GetThreadWaitTime(scriptInstance_t inst, unsigned int startLocalId);
unsigned int __cdecl GetParentLocalId(scriptInstance_t inst, unsigned int threadId);
unsigned int __cdecl GetSafeParentLocalId(scriptInstance_t inst, unsigned int threadId);
unsigned int __cdecl GetStartLocalId(scriptInstance_t inst, unsigned int threadId);
void __cdecl Scr_KillThread(scriptInstance_t inst, unsigned int parentId);
void __cdecl Scr_KillEndonThread(scriptInstance_t inst, unsigned int threadId);
unsigned int __cdecl AllocValue(scriptInstance_t inst);
unsigned int __cdecl AllocObject(scriptInstance_t inst);
unsigned int __cdecl AllocVariable(scriptInstance_t inst);
unsigned int __cdecl Scr_AllocArray(scriptInstance_t inst);
unsigned int __cdecl AllocThread(scriptInstance_t inst, unsigned int self);
unsigned int __cdecl AllocChildThread(scriptInstance_t inst, unsigned int self, unsigned int parentLocalId);
ObjectInfo::<unnamed_type_u> __cdecl Scr_GetSelf(scriptInstance_t inst, unsigned int threadId);
void __cdecl FreeValue(scriptInstance_t inst, unsigned int id);
void __cdecl RemoveRefToObject(scriptInstance_t inst, unsigned int id);
void __cdecl FreeVariable(scriptInstance_t inst, unsigned int id);
void __cdecl RemoveRefToEmptyObject(scriptInstance_t inst, unsigned int id);
int __cdecl Scr_GetRefCountToObject(scriptInstance_t inst, unsigned int id);
float *__cdecl Scr_AllocVector(scriptInstance_t inst, float *v);
float *__cdecl Scr_AllocVector(scriptInstance_t inst);
bool __cdecl IsValidArrayIndex(scriptInstance_t inst, unsigned int unsignedValue);
unsigned int __cdecl GetInternalVariableIndex(scriptInstance_t inst, unsigned int unsignedValue);
unsigned int __cdecl FindArrayVariable(scriptInstance_t inst, unsigned int parentId, unsigned int intValue);
unsigned int __cdecl FindArrayVariableIndex(scriptInstance_t inst, unsigned int parentId, unsigned int unsignedValue);
unsigned int __cdecl FindVariable(scriptInstance_t inst, unsigned int parentId, unsigned int unsignedValue);
unsigned int __cdecl FindObjectVariable(scriptInstance_t inst, unsigned int parentId, unsigned int id);
unsigned int __cdecl GetArrayVariableIndex(scriptInstance_t inst, unsigned int parentId, unsigned int unsignedValue);
unsigned int __cdecl Scr_GetVariableFieldIndex(scriptInstance_t inst, int parentId, int name);
VariableUnion __cdecl Scr_FindVariableField(scriptInstance_t inst, unsigned int parentId, unsigned int name);
unsigned int __cdecl Scr_FindAllVariableField(scriptInstance_t inst, unsigned int parentId, unsigned int *names);
void __cdecl ClearVariableField(scriptInstance_t inst, unsigned int parentId, unsigned int name, VariableValue *value);
unsigned int __cdecl GetArrayVariable(scriptInstance_t inst, unsigned int parentId, unsigned int unsignedValue);
unsigned int __cdecl GetNewArrayVariable(scriptInstance_t inst, unsigned int parentId, unsigned int unsignedValue);
unsigned int __cdecl GetNewArrayVariableIndex(scriptInstance_t inst, unsigned int parentId, unsigned int unsignedValue);
unsigned int __cdecl GetNewVariableIndexInternal(scriptInstance_t inst, unsigned int parentId, unsigned int name);
unsigned int __cdecl GetVariable(scriptInstance_t inst, unsigned int parentId, unsigned int unsignedValue);
unsigned int __cdecl GetNewVariable(scriptInstance_t inst, unsigned int parentId, unsigned int unsignedValue);
unsigned int __cdecl GetObjectVariable(scriptInstance_t inst, unsigned int parentId, unsigned int id);
unsigned int __cdecl GetNewObjectVariable(scriptInstance_t inst, unsigned int parentId, unsigned int id);
unsigned int __cdecl GetNewObjectVariableReverse(scriptInstance_t inst, unsigned int parentId, unsigned int id);
unsigned int __cdecl GetNewVariableIndexReverseInternal(
        scriptInstance_t inst,
        unsigned int parentId,
        unsigned int name);
unsigned int __cdecl GetNewVariableIndexReverseInternal2(
        scriptInstance_t inst,
        unsigned int parentId,
        unsigned int name,
        unsigned int index);
void __cdecl RemoveVariable(scriptInstance_t inst, unsigned int parentId, unsigned int unsignedValue);
void __cdecl RemoveNextVariable(scriptInstance_t inst, unsigned int parentId);
void __cdecl RemoveObjectVariable(scriptInstance_t inst, unsigned int parentId, unsigned int id);
void __cdecl RemoveArrayVariable(scriptInstance_t inst, unsigned int parentId, unsigned int unsignedValue);
void __cdecl SafeRemoveVariable(scriptInstance_t inst, unsigned int parentId, unsigned int unsignedValue);
void __cdecl RemoveVariableValue(scriptInstance_t inst, unsigned int parentId, unsigned int index);
void __cdecl SetNewVariableValue(scriptInstance_t inst, unsigned int id, VariableValue *value);
VariableValueInternal::<unnamed_type_u> *__cdecl GetVariableValueAddress(scriptInstance_t inst, unsigned int id);
void __cdecl SetVariableEntityFieldValue(
        scriptInstance_t inst,
        unsigned int entId,
        unsigned int fieldName,
        VariableValue *value);
void __cdecl ClearVariableValue(scriptInstance_t inst, unsigned int id);
void __cdecl SetVariableFieldValue(scriptInstance_t inst, unsigned int id, VariableValue *value);
unsigned int __cdecl Scr_EvalVariableObject(scriptInstance_t inst, unsigned int id);
VariableUnion __cdecl Scr_EvalVariableEntityField(scriptInstance_t inst, unsigned int entId, unsigned int fieldName);
VariableValue __cdecl Scr_EvalVariableField(scriptInstance_t inst, unsigned int id);
void __cdecl Scr_EvalSizeValue(scriptInstance_t inst, VariableValue *value);
unsigned int __cdecl GetArraySize(scriptInstance_t inst, unsigned int id);
unsigned int __cdecl GetObject(scriptInstance_t inst, unsigned int id);
unsigned int __cdecl GetArray(scriptInstance_t inst, unsigned int id);
unsigned int __cdecl FindObject(scriptInstance_t inst, unsigned int id);
bool __cdecl IsFieldObject(scriptInstance_t inst, unsigned int id);
bool __cdecl Scr_IsThreadAlive(unsigned int thread, scriptInstance_t inst);
void __cdecl Scr_EvalBoolNot(scriptInstance_t inst, VariableValue *value);
void __cdecl Scr_EvalBoolComplement(scriptInstance_t inst, VariableValue *value);
void __cdecl Scr_CastBool(scriptInstance_t inst, VariableValue *value);
char __cdecl Scr_CastString(scriptInstance_t inst, VariableValue *value);
void __cdecl Scr_CastDebugString(scriptInstance_t inst, VariableValue *value);
char __cdecl Scr_GetEntClassId(scriptInstance_t inst, unsigned int id);
int __cdecl Scr_GetEntNum(scriptInstance_t inst, unsigned int id);
void __cdecl Scr_ClearVector(scriptInstance_t inst, VariableValue *value);
void __cdecl Scr_CastVector(scriptInstance_t inst, VariableValue *value);
VariableUnion __cdecl Scr_EvalFieldObject(scriptInstance_t inst, unsigned int tempVariable, VariableValue *value);
void __cdecl Scr_UnmatchingTypesError(scriptInstance_t inst, VariableValue *value1, VariableValue *value2);
void __cdecl Scr_EvalOr(scriptInstance_t inst, VariableValue *value1, VariableValue *value2);
void __cdecl Scr_EvalExOr(scriptInstance_t inst, VariableValue *value1, VariableValue *value2);
void __cdecl Scr_EvalAnd(scriptInstance_t inst, VariableValue *value1, VariableValue *value2);
void __cdecl Scr_EvalEquality(scriptInstance_t inst, VariableValue *value1, VariableValue *value2);
void __cdecl Scr_CastWeakerPair(scriptInstance_t inst, VariableValue *value1, VariableValue *value2);
void __cdecl Scr_EvalInequality(scriptInstance_t inst, VariableValue *value1, VariableValue *value2);
void __cdecl Scr_EvalLess(scriptInstance_t inst, VariableValue *value1, VariableValue *value2);
void __cdecl Scr_EvalGreaterEqual(scriptInstance_t inst, VariableValue *value1, VariableValue *value2);
void __cdecl Scr_EvalGreater(scriptInstance_t inst, VariableValue *value1, VariableValue *value2);
void __cdecl Scr_EvalLessEqual(scriptInstance_t inst, VariableValue *value1, VariableValue *value2);
void __cdecl Scr_EvalShiftLeft(scriptInstance_t inst, VariableValue *value1, VariableValue *value2);
void __cdecl Scr_EvalShiftRight(scriptInstance_t inst, VariableValue *value1, VariableValue *value2);
void __cdecl Scr_EvalPlus(scriptInstance_t inst, VariableValue *value1, VariableValue *value2);
void __cdecl Scr_CastWeakerStringPair(scriptInstance_t inst, VariableValue *value1, VariableValue *value2);
void __cdecl Scr_EvalMinus(scriptInstance_t inst, VariableValue *value1, VariableValue *value2);
void __cdecl Scr_EvalMultiply(scriptInstance_t inst, VariableValue *value1, VariableValue *value2);
void __cdecl Scr_EvalDivide(scriptInstance_t inst, VariableValue *value1, VariableValue *value2);
void __cdecl Scr_EvalMod(scriptInstance_t inst, VariableValue *value1, VariableValue *value2);
void __cdecl Scr_EvalBinaryOperator(scriptInstance_t inst, int op, VariableValue *value1, VariableValue *value2);
bool __cdecl IsObjectFree(scriptInstance_t inst, unsigned int id);
unsigned int __cdecl GetValueType(scriptInstance_t inst, unsigned int id);
unsigned int __cdecl GetObjectType(scriptInstance_t inst, unsigned int id);
void __cdecl Scr_FreeEntityNum(unsigned int entnum, unsigned int classnum, scriptInstance_t inst);
void __cdecl Scr_FreeEntityList(scriptInstance_t inst);
void __cdecl Scr_FreeObjects(scriptInstance_t inst);
void __cdecl Scr_SetClassMap(scriptInstance_t inst, unsigned int classnum);
void __cdecl Scr_RemoveClassMap(scriptInstance_t inst, unsigned int classnum);
void __cdecl Scr_AddClassField(unsigned int classnum, char *name, unsigned int offset, scriptInstance_t inst);
VariableUnion __cdecl Scr_GetOffset(unsigned int classnum, char *name, scriptInstance_t inst);
unsigned int __cdecl FindEntityId(scriptInstance_t inst, unsigned int entnum, unsigned int classnum, int clientNum);
unsigned int __cdecl Scr_GetEntityId(scriptInstance_t inst, unsigned int entnum, unsigned int classnum, int clientNum);
unsigned int __cdecl AllocEntity(
        scriptInstance_t inst,
        unsigned int classnum,
        unsigned __int16 entnum,
        unsigned __int8 clientNum);
void __cdecl Scr_EvalArray(scriptInstance_t inst, VariableValue *value, VariableValue *index);
unsigned int __cdecl Scr_FindArrayIndex(scriptInstance_t inst, unsigned int parentId, VariableValue *index);
unsigned int __cdecl Scr_EvalArrayRef(scriptInstance_t inst, unsigned int parentId);
void __cdecl CopyArray(scriptInstance_t inst, unsigned int parentId, unsigned int newParentId);
void __cdecl ClearArray(scriptInstance_t inst, unsigned int parentId, VariableValue *value);
void __cdecl SafeRemoveArrayVariable(scriptInstance_t inst, unsigned int parentId, unsigned int unsignedValue);
void __cdecl SetEmptyArray(scriptInstance_t inst, unsigned int parentId);
void __cdecl Scr_AddArrayKeys(unsigned int parentId, scriptInstance_t inst);
scr_entref_t *__cdecl Scr_GetEntityIdRef(scr_entref_t *__return_ptr retstr, scriptInstance_t inst, unsigned int entId);
void __cdecl Scr_CopyEntityNum(
        unsigned int fromEntnum,
        unsigned int toEntnum,
        unsigned int classnum,
        scriptInstance_t inst);
void __cdecl CopyEntity(scriptInstance_t inst, unsigned int parentId, unsigned int newParentId);
double __cdecl Scr_GetObjectUsage(scriptInstance_t inst, unsigned int parentId);
double __cdecl Scr_GetEntryUsage(scriptInstance_t inst, VariableValueInternal *entryValue);
double __cdecl Scr_GetThreadUsage(scriptInstance_t inst, const VariableStackBuffer *stackBuf, float *endonUsage);
double __cdecl Scr_GetEndonUsage(scriptInstance_t inst, unsigned int parentId);
unsigned int __cdecl Scr_FindField(const char *name, int *type, scriptInstance_t inst);
void __cdecl Scr_AddFields(const char *path, char *extension, scriptInstance_t inst);
void __cdecl Scr_AddFields_LoadObj(scriptInstance_t inst, const char *path, char *extension);
void __cdecl Scr_AddFieldsForFile(scriptInstance_t inst, char *filename);
char *__cdecl Scr_GetSourceFile_LoadObj(char *filename);
XModelPiece *__cdecl Scr_GetSourceFile_FastFile(const char *filename);
void __cdecl Scr_AddFields_FastFile(scriptInstance_t inst, const char *path, const char *extension);
void __cdecl Scr_FreeValue(scriptInstance_t inst, unsigned int id);
void __cdecl Scr_AllocGameVariable(scriptInstance_t inst);
void __cdecl Scr_FreeGameVariable(scriptInstance_t inst, int bComplete);
int __cdecl Scr_MakeValuePrimitive(scriptInstance_t inst, unsigned int parentId);
int __cdecl Scr_GetClassnumForCharId(scriptInstance_t inst, char charId);
int __cdecl Scr_FindAllThreads(scriptInstance_t inst, unsigned int selfId, unsigned int *threads, unsigned int localId);
unsigned int __cdecl Scr_FindAllEndons(scriptInstance_t inst, unsigned int threadId, unsigned int *names);
