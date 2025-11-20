#pragma once

void __cdecl Scr_AddStruct();
void __cdecl Scr_ResetTimeout();
void __cdecl RemoveRefToValue(scriptInstance_t inst, int type, VariableUnion u);
void __cdecl Scr_ClearErrorMessage(scriptInstance_t inst);
void __cdecl Scr_Init(scriptInstance_t inst);
void __cdecl Scr_VM_Init(scriptInstance_t inst);
void __cdecl Scr_Settings(int developer, int developer_script, int abort_on_error, scriptInstance_t inst);
void __cdecl Scr_Shutdown(scriptInstance_t inst);
void __cdecl VM_Shutdown(scriptInstance_t inst);
void __cdecl Scr_SetLoading(int bLoading, scriptInstance_t inst);
int __cdecl Scr_GetNumScriptThreads(scriptInstance_t inst);
void __cdecl Scr_ClearOutParams(scriptInstance_t inst);
char *__cdecl Scr_GetReturnPos(scriptInstance_t inst, unsigned int *localId);
char *__cdecl Scr_GetNextCodepos(
        scriptInstance_t inst,
        VariableValue *top,
        const char *pos,
        int opcode,
        int mode,
        unsigned int *localId);
void __cdecl VM_CancelNotify(scriptInstance_t inst, unsigned int notifyListOwnerId, unsigned int startLocalId);
void __cdecl VM_CancelNotifyInternal(
        scriptInstance_t inst,
        unsigned int notifyListOwnerId,
        unsigned int startLocalId,
        unsigned int notifyListId,
        unsigned int notifyNameListId,
        unsigned int stringValue);
bool __cdecl Scr_IsEndonThread(scriptInstance_t inst, unsigned int localId);
unsigned int __cdecl Scr_GetWaittillThreadStackId(
        scriptInstance_t inst,
        unsigned int localId,
        unsigned int startLocalId);
char *__cdecl Scr_GetThreadPos(scriptInstance_t inst, unsigned int localId);
const char *__cdecl Scr_GetStackThreadPos(
        scriptInstance_t inst,
        unsigned int endLocalId,
        VariableStackBuffer *stackValue,
        bool killThread);
char *__cdecl Scr_GetRunningThreadPos(scriptInstance_t inst, unsigned int localId);
unsigned int __cdecl Scr_GetWaitThreadStackId(scriptInstance_t inst, unsigned int localId, unsigned int startLocalId);
void __cdecl Scr_NotifyNum_Internal(
        scriptInstance_t inst,
        int clientNum,
        unsigned int entnum,
        unsigned int classnum,
        unsigned int stringValue,
        unsigned int paramcount);
void __cdecl VM_Notify(
        scriptInstance_t inst,
        unsigned int notifyListOwnerId,
        unsigned int stringValue,
        VariableValue *top);
void __cdecl Scr_TerminateThread(scriptInstance_t inst, unsigned int localId);
void __cdecl Scr_TerminateRunningThread(scriptInstance_t inst, unsigned int localId);
void __cdecl Scr_TerminateWaitThread(scriptInstance_t inst, unsigned int localId, unsigned int startLocalId);
void __cdecl VM_TerminateStack(
        scriptInstance_t inst,
        unsigned int endLocalId,
        unsigned int startLocalId,
        VariableStackBuffer *stackValue);
void __cdecl Scr_TerminateWaittillThread(scriptInstance_t inst, unsigned int localId, unsigned int startLocalId);
void __cdecl Scr_NotifyNum(
        unsigned int entnum,
        unsigned int classnum,
        unsigned int stringValue,
        unsigned int paramcount);
void __cdecl CScr_NotifyNum(
        int clientNum,
        unsigned int entnum,
        unsigned int classnum,
        unsigned int stringValue,
        unsigned int paramcount);
void __cdecl Scr_CancelNotifyList(scriptInstance_t inst, unsigned int notifyListOwnerId);
void __cdecl VM_TrimStack(
        scriptInstance_t inst,
        unsigned int startLocalId,
        VariableStackBuffer *stackValue,
        bool fromEndon);
void __cdecl Scr_CancelWaittill(scriptInstance_t inst, unsigned int startLocalId);
unsigned __int16 __cdecl Scr_ExecThread(scriptInstance_t inst, int handle, unsigned int paramcount);
unsigned int __cdecl VM_Execute(scriptInstance_t inst, unsigned int localId, char *pos, unsigned int paramcount);
void __cdecl RemoveRefToValue(scriptInstance_t inst, VariableValue *value);
unsigned int __cdecl Scr_GetLocalVar(scriptInstance_t inst, const char *pos);
unsigned __int16 __cdecl Scr_ReadUnsignedShort(const char **pos);
const char *__cdecl Scr_ReadCodePos(scriptInstance_t inst, const char **pos);
unsigned int __cdecl GetDummyObject(scriptInstance_t inst);
int __cdecl GetDummyFieldValue(scriptInstance_t inst);
void __cdecl VM_PrintJumpHistory(scriptInstance_t inst);
VariableStackBuffer *__cdecl VM_ArchiveStack(scriptInstance_t inst, function_stack_t *stack);
unsigned __int16 __cdecl Scr_ExecEntThreadNum(
        scriptInstance_t inst,
        unsigned int entnum,
        unsigned int classnum,
        int handle,
        unsigned int paramcount,
        unsigned __int16 localClientNum);
void __cdecl Scr_AddExecThread(scriptInstance_t inst, int handle, unsigned int paramcount);
void __cdecl Scr_FreeThread(unsigned __int16 handle, scriptInstance_t inst);
void __cdecl Scr_ExecCode(scriptInstance_t inst, char *pos, unsigned int localId);
bool __cdecl Scr_IsSystemInitied(scriptInstance_t inst);
void __cdecl Scr_InitSystem(scriptInstance_t inst, int sys);
void __cdecl Scr_ShutdownSystem(scriptInstance_t inst, unsigned __int8 sys, int bComplete);
void __cdecl VM_TerminateTime(scriptInstance_t inst, unsigned int timeId);
bool __cdecl Scr_IsSystemActive(unsigned __int8 sys, scriptInstance_t inst);
VariableUnion __cdecl Scr_GetInt(unsigned int index, scriptInstance_t inst);
scr_anim_s __cdecl Scr_GetAnim(unsigned int index, XAnimTree_s *tree, scriptInstance_t inst);
void __cdecl Scr_ErrorInternal(scriptInstance_t inst);
scr_animtree_t __cdecl Scr_GetAnimTree(unsigned int index, unsigned int user, scriptInstance_t inst);
double __cdecl Scr_GetFloat(unsigned int index, scriptInstance_t inst);
VariableUnion __cdecl Scr_GetConstString(unsigned int index, scriptInstance_t inst);
VariableUnion __cdecl Scr_GetConstLowercaseString(unsigned int index, scriptInstance_t inst);
char *__cdecl Scr_GetString(unsigned int index, scriptInstance_t inst);
VariableUnion __cdecl Scr_GetConstStringIncludeNull(unsigned int index, scriptInstance_t inst);
char *__cdecl Scr_GetDebugString(unsigned int index, scriptInstance_t inst);
VariableUnion __cdecl Scr_GetConstIString(unsigned int index, scriptInstance_t inst);
char *__cdecl Scr_GetIString(unsigned int index, scriptInstance_t inst);
void __cdecl Scr_GetVector(unsigned int index, float *vectorValue, scriptInstance_t inst);
int __cdecl Scr_GetFunc(unsigned int index, scriptInstance_t inst);
scr_entref_t *__cdecl Scr_GetEntityRef(scr_entref_t *__return_ptr retstr, unsigned int index, scriptInstance_t inst);
VariableUnion __cdecl Scr_GetObject(unsigned int index, scriptInstance_t inst);
int __cdecl Scr_GetType(unsigned int index, scriptInstance_t inst);
const char *__cdecl Scr_GetTypeName(unsigned int index, scriptInstance_t inst);
unsigned int __cdecl Scr_GetPointerType(unsigned int index, scriptInstance_t inst);
int __cdecl Scr_GetNumParam(scriptInstance_t inst);
void __cdecl Scr_AddBool(unsigned int value, scriptInstance_t inst);
void __cdecl IncInParam(scriptInstance_t inst);
void __cdecl Scr_AddInt(int value, scriptInstance_t inst);
void __cdecl Scr_AddFloat(float value, scriptInstance_t inst);
void __cdecl Scr_AddAnim(scr_anim_s value, scriptInstance_t inst);
void __cdecl Scr_AddUndefined(scriptInstance_t inst);
void __cdecl Scr_AddObject(unsigned int id, scriptInstance_t inst);
void __cdecl Scr_AddEntityNum(unsigned int entnum, unsigned int classnum, scriptInstance_t inst, int clientNum);
void __cdecl Scr_AddStruct(scriptInstance_t inst);
void __cdecl Scr_AddString(char *value, scriptInstance_t inst);
void __cdecl Scr_AddIString(char *value, scriptInstance_t inst);
void __cdecl Scr_AddConstString(unsigned int value, scriptInstance_t inst);
void __cdecl Scr_AddVector(float *value, scriptInstance_t inst);
void __cdecl Scr_MakeArray(scriptInstance_t inst);
void __cdecl Scr_AddArray(scriptInstance_t inst);
void __cdecl Scr_AddArrayStringIndexed(unsigned int stringValue, scriptInstance_t inst);
void __cdecl Scr_Error(const char *error, bool force_terminal);
void __cdecl Scr_Error(scriptInstance_t inst, const char *error, bool force_terminal);
void __cdecl Scr_SetErrorMessage(scriptInstance_t inst, const char *error);
void __cdecl Scr_TerminalError(const char *error, scriptInstance_t inst);
void __cdecl Scr_NeverTerminalError(const char *error, scriptInstance_t inst);
void __cdecl Scr_ParamError(unsigned int index, const char *error, scriptInstance_t inst);
void __cdecl Scr_ObjectError(const char *error, scriptInstance_t inst);
char __cdecl SetEntityFieldValue(
        scriptInstance_t inst,
        unsigned int classnum,
        unsigned int entnum,
        unsigned __int16 clientNum,
        unsigned int offset,
        VariableValue *value);
VariableUnion __cdecl GetEntityFieldValue(
        scriptInstance_t inst,
        unsigned int classnum,
        unsigned int entnum,
        unsigned __int16 clientNum,
        unsigned int offset);
void __cdecl Scr_SetStructField(unsigned int structId, unsigned int index, scriptInstance_t inst);
void __cdecl Scr_SetDynamicEntityField(
        unsigned int entnum,
        unsigned int classnum,
        unsigned int index,
        scriptInstance_t inst,
        unsigned __int16 localClientNum);
void __cdecl Scr_IncTime(scriptInstance_t inst);
void __cdecl Scr_DecTime(scriptInstance_t inst);
void __cdecl Scr_RunCurrentThreads(scriptInstance_t inst);
void __cdecl VM_SetTime(scriptInstance_t inst);
void __cdecl VM_Resume(scriptInstance_t inst, unsigned int timeId);
void __cdecl VM_UnarchiveStack(scriptInstance_t inst, unsigned int startLocalId, VariableStackBuffer *stackValue);
int __cdecl Scr_AddLocalVars(scriptInstance_t inst, unsigned int localId);
void __cdecl Scr_ResetTimeout(scriptInstance_t inst);
bool __cdecl Scr_IsStackClear(scriptInstance_t inst);
void __cdecl Scr_StackClear(scriptInstance_t inst);
