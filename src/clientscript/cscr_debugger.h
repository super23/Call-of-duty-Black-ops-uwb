#pragma once

void __cdecl Scr_AddManualBreakpoint(scriptInstance_t inst, unsigned __int8 *codePos);
void __cdecl Scr_AddBreakpoint(scriptInstance_t inst, const unsigned __int8 *codePos);
char *__cdecl Scr_FindBreakpointInfo(scriptInstance_t inst, const char *codePos);
Scr_Breakpoint *__cdecl Scr_AllocBreakpoint();
void __cdecl Scr_FreeBreakpoint(Scr_Breakpoint *breakpoint);
void __thiscall Scr_ScriptWindow::SetScriptFile(Scr_ScriptWindow *this, scriptInstance_t inst, const char *name);
void __thiscall Scr_ScriptWindow::AddBreakpoint(
        Scr_ScriptWindow *this,
        scriptInstance_t inst,
        Scr_Breakpoint **pBreakpoint,
        char *codePos,
        int builtinIndex,
        Scr_WatchElement_s *element,
        unsigned __int8 type);
Scr_WatchElement_s *__cdecl Scr_ReadElement(scriptInstance_t inst);
void __thiscall Scr_ScriptWindow::GetSourcePos(
        Scr_ScriptWindow *this,
        scriptInstance_t inst,
        unsigned int *start,
        unsigned int *end);
char __thiscall Scr_ScriptWindow::AddBreakpointAtSourcePos(
        Scr_ScriptWindow *this,
        scriptInstance_t inst,
        Scr_WatchElement_s *element,
        unsigned __int8 breakpointType,
        bool user,
        Scr_Breakpoint **pBreakpoint,
        unsigned int startSourcePos,
        unsigned int endSourcePos);
void __thiscall Scr_ScriptWindow::ToggleBreakpointInternal(
        Scr_ScriptWindow *this,
        scriptInstance_t inst,
        Scr_WatchElement_s *element,
        bool force,
        bool overwrite,
        unsigned __int8 breakpointType,
        bool user);
void __cdecl Scr_WriteElement(Scr_WatchElement_s *element);
void __cdecl Scr_FreeLineBreakpoint(scriptInstance_t inst, Scr_Breakpoint *breakpoint, bool deleteElement);
void __cdecl Scr_RemoveManualBreakpoint(scriptInstance_t inst, unsigned __int8 *codePos);
void __cdecl Scr_RemoveBreakpoint(scriptInstance_t inst, unsigned __int8 *codePos);
char *__thiscall Scr_ScriptWindow::GetBreakpointCodePos(Scr_ScriptWindow *this, scriptInstance_t inst);
void __cdecl Scr_MonitorCommand(const char *text, scriptInstance_t inst);
void __thiscall Scr_ScriptWindow::RunToCursor(Scr_ScriptWindow *this, scriptInstance_t inst);
void __cdecl Scr_ResumeBreakpoints(scriptInstance_t inst);
void __cdecl Scr_SetTempBreakpoint(scriptInstance_t inst, char *codePos, unsigned int threadId);
void __thiscall Scr_ScriptWindow::EnterCallInternal(Scr_ScriptWindow *this, scriptInstance_t inst);
void __thiscall Scr_ScriptWindow::Init(Scr_ScriptWindow *this, scriptInstance_t inst);
void __thiscall Scr_AbstractScriptList::Init(Scr_AbstractScriptList *this, scriptInstance_t inst);
void __thiscall Scr_AbstractScriptList::Shutdown(Scr_AbstractScriptList *this, scriptInstance_t inst);
void __cdecl Scr_FreeDebugMem(scriptInstance_t inst, char *ptr);
void __thiscall Scr_ScriptList::AddFile(
        Scr_ScriptList *this,
        scriptInstance_t inst,
        const char *filename,
        Scr_AddFileInfo *info);
void __thiscall Scr_ScriptList::Init(Scr_ScriptList *this, scriptInstance_t inst);
unsigned int *__cdecl Scr_AllocDebugMem(scriptInstance_t inst, int size, const char *name);
void __thiscall Scr_ScriptList::Shutdown(Scr_ScriptList *this, scriptInstance_t inst);
void __thiscall Scr_ScriptCallStack::UpdateStack(Scr_ScriptCallStack *this, scriptInstance_t inst);
Scr_WatchElement_s *__thiscall Scr_ScriptWatch::GetElementWithId_r(
        Scr_ScriptWatch *this,
        Scr_WatchElement_s *element,
        int id);
Scr_WatchElement_s *__thiscall Scr_ScriptWatch::GetElementWithId(Scr_ScriptWatch *this, int id);
Scr_WatchElement_s **__thiscall Scr_ScriptWatch::GetElementRef(Scr_ScriptWatch *this, Scr_WatchElement_s *element);
Scr_WatchElement_s *__thiscall Scr_ScriptWatch::GetElementPrev(Scr_ScriptWatch *this, Scr_WatchElement_s *element);
void __thiscall Scr_ScriptWatch::ToggleExpandElement(
        Scr_ScriptWatch *this,
        scriptInstance_t inst,
        Scr_WatchElement_s *element);
void __thiscall Scr_ScriptWatch::ExpandElement(
        Scr_ScriptWatch *this,
        scriptInstance_t inst,
        Scr_WatchElement_s *element,
        bool expand);
Scr_WatchElement_s *__thiscall Scr_ScriptWatch::CreateBreakpointElement(
        Scr_ScriptWatch *this,
        scriptInstance_t inst,
        Scr_WatchElement_s *element,
        unsigned int bufferIndex,
        unsigned int sourcePos,
        bool user);
Scr_WatchElement_s *__cdecl Scr_GetElementRoot(Scr_WatchElement_s *element);
Scr_WatchElement_s *__thiscall Scr_ScriptWatch::PasteNonBreakpointElement(
        Scr_ScriptWatch *this,
        scriptInstance_t inst,
        Scr_WatchElement_s *element,
        char *text,
        bool user);
Scr_WatchElement_s *__thiscall Scr_ScriptWatch::DeleteElementInternal(
        Scr_ScriptWatch *this,
        scriptInstance_t inst,
        Scr_WatchElement_s *element);
Scr_WatchElement_s *__thiscall Scr_ScriptWatch::BackspaceElementInternal(
        Scr_ScriptWatch *this,
        scriptInstance_t inst,
        Scr_WatchElement_s *element);
void __thiscall Scr_ScriptWatch::Init(Scr_ScriptWatch *this, scriptInstance_t inst);
void __thiscall Scr_ScriptWatch::Shutdown(Scr_ScriptWatch *this, scriptInstance_t inst);
void __cdecl Scr_FreeWatchElementChildrenStrict(scriptInstance_t inst, Scr_WatchElement_s *element);
void __cdecl Scr_FreeWatchElementChildren(scriptInstance_t inst, Scr_WatchElement_s *element);
void __cdecl Scr_RemoveValue(scriptInstance_t inst, Scr_WatchElement_s *element);
void __cdecl Scr_FreeWatchElementText(scriptInstance_t inst, Scr_WatchElement_s *element);
void __thiscall Scr_ScriptWatch::FreeWatchElement(
        Scr_ScriptWatch *this,
        scriptInstance_t inst,
        Scr_WatchElement_s *element);
void __thiscall Scr_ScriptWatch::EvaluateWatchChildren(
        Scr_ScriptWatch *this,
        scriptInstance_t inst,
        Scr_WatchElement_s *parentElement);
VariableValue __cdecl Scr_GetArrayIndexValue(scriptInstance_t inst, unsigned int name);
bool __cdecl Scr_IsSortWatchElement(Scr_WatchElement_s *element);
int __cdecl CompareArrayIndices(unsigned int *arg1, unsigned int *arg2);
int __cdecl CompareArrayIndicesClient(unsigned int *arg1, unsigned int *arg2);
void __cdecl Scr_DeltaElementValueText(Scr_WatchElement_s *element, const char *oldValueText);
void __cdecl Scr_SetNonFieldElementRefText(scriptInstance_t inst, Scr_WatchElement_s *element);
void __cdecl Scr_PostSetText(scriptInstance_t inst, Scr_WatchElement_s *element);
char *__cdecl Scr_GetElementThreadPos(scriptInstance_t inst, Scr_WatchElement_s *element);
void __cdecl Scr_SetElementRefText(scriptInstance_t inst, Scr_WatchElement_s *element, char *fieldText);
void __cdecl Scr_DeltaElementRefText(Scr_WatchElement_s *element, const char *oldRefText, char *fieldText);
void __cdecl Scr_ConnectElementChildren(Scr_WatchElement_s *parentElement);
void __cdecl Scr_SortElementChildren(scriptInstance_t inst, Scr_WatchElement_s *parentElement);
int __cdecl CompareThreadElements(int *arg1, int *arg2);
char __thiscall Scr_ScriptWatch::PostEvaluateWatchElement(
        Scr_ScriptWatch *this,
        scriptInstance_t inst,
        Scr_WatchElement_s *element,
        VariableValue *value);
char __thiscall Scr_ScriptWatch::EvaluateWatchChildElement(
        Scr_ScriptWatch *this,
        scriptInstance_t inst,
        Scr_WatchElement_s *element,
        unsigned int fieldName,
        Scr_WatchElement_s *childElement,
        bool hardcodedField);
void __thiscall Scr_ScriptWatch::EvaluateWatchElementExpression(
        Scr_ScriptWatch *this,
        scriptInstance_t inst,
        Scr_WatchElement_s *element,
        VariableValue *value);
void __thiscall Scr_ScriptWatch::EvaluateWatchElement(
        Scr_ScriptWatch *this,
        scriptInstance_t inst,
        Scr_WatchElement_s *element);
Scr_WatchElement_s *__thiscall Scr_ScriptWatch::CreateWatchElement(
        Scr_ScriptWatch *this,
        scriptInstance_t inst,
        char *text,
        Scr_WatchElement_s **prevElem,
        const char *name);
Scr_WatchElement_s *__cdecl Scr_CreateWatchElement(
        scriptInstance_t inst,
        char *text,
        Scr_WatchElement_s **prevElem,
        const char *name);
void __thiscall Scr_ScriptWatch::AddElement(
        Scr_ScriptWatch *this,
        scriptInstance_t inst,
        Scr_WatchElement_s *element,
        char *text);
Scr_WatchElement_s *__thiscall Scr_ScriptWatch::CloneElement(
        Scr_ScriptWatch *this,
        scriptInstance_t inst,
        Scr_WatchElement_s *element);
void __thiscall Scr_ScriptWatch::Evaluate(Scr_ScriptWatch *this, scriptInstance_t inst);
void __cdecl Scr_Evaluate(scriptInstance_t inst);
void __cdecl Scr_CheckBreakonNotify(
        scriptInstance_t inst,
        unsigned int notifyListOwnerId,
        unsigned int stringValue,
        VariableValue *top,
        const char *pos,
        unsigned int localId);
void __cdecl Scr_SpecialBreakpoint(
        scriptInstance_t inst,
        VariableValue *top,
        const char *pos,
        unsigned int localId,
        int opcode,
        int type);
bool __cdecl Scr_AllowBreakpoint(scriptInstance_t inst);
void __thiscall Scr_ScriptWatch::DisplayThreadPos(
        Scr_ScriptWatch *this,
        scriptInstance_t inst,
        Scr_WatchElement_s *element);
void __cdecl Scr_UnbreakAllAssignmentPos(scriptInstance_t inst);
void __cdecl Scr_RemoveAssignmentBreakpoint(scriptInstance_t inst, unsigned __int8 *codePos);
bool __cdecl Scr_RefToVariable(scriptInstance_t inst, unsigned int id, int isObject);
void __thiscall Scr_ScriptWatch::UpdateBreakpoints(Scr_ScriptWatch *this, scriptInstance_t inst, bool add);
void __cdecl Scr_BreakOnAllAssignmentPos(scriptInstance_t inst);
void __cdecl Scr_AddAssignmentBreakpoint(scriptInstance_t inst, unsigned __int8 *codePos);
Scr_WatchElement_s *__thiscall Scr_ScriptWatch::AddBreakpoint(
        Scr_ScriptWatch *this,
        scriptInstance_t inst,
        Scr_WatchElement_s *element,
        unsigned __int8 type);
Scr_WatchElement_s *__thiscall Scr_ScriptWatch::RemoveBreakpoint(
        Scr_ScriptWatch *this,
        scriptInstance_t inst,
        Scr_WatchElement_s *element);
void __thiscall Scr_ScriptWatch::ToggleWatchElementBreakpoint(
        Scr_ScriptWatch *this,
        scriptInstance_t inst,
        Scr_WatchElement_s *element,
        unsigned __int8 type);
void __thiscall Scr_ScriptWatch::ToggleBreakpointInternal(
        Scr_ScriptWatch *this,
        scriptInstance_t inst,
        Scr_WatchElement_s *element,
        unsigned __int8 type);
void __thiscall Scr_ScriptWatch::SortHitBreakpointsTop(Scr_ScriptWatch *this, scriptInstance_t inst);
void __cdecl Scr_InitDebuggerMain(scriptInstance_t inst);
void __cdecl Scr_ShutdownDebuggerMain(scriptInstance_t inst);
void __cdecl Scr_InitDebugger(scriptInstance_t inst);
void __cdecl Scr_ShutdownDebugger(scriptInstance_t inst);
void __cdecl Scr_InitDebuggerSystem(scriptInstance_t inst);
void Scr_InitBreakpoints();
void __cdecl Scr_ShutdownDebuggerSystem(scriptInstance_t inst);
void __cdecl Scr_ConnectRemote(scriptInstance_t inst);
void __cdecl Scr_AddAssignmentPos(scriptInstance_t inst, char *codePos);
void __cdecl Scr_ResetAbortDebugger(scriptInstance_t inst);
void __cdecl Scr_RunDebugger(scriptInstance_t inst);
void __cdecl Scr_DisplayDebugger(scriptInstance_t inst);
void __cdecl Scr_DisplayDebuggerRemote(scriptInstance_t inst);
void __cdecl Scr_DisplayDebuggerRemoteInternal(scriptInstance_t inst);
void __cdecl Scr_Step(scriptInstance_t inst);
void __cdecl Scr_WatchElementHitBreakpoint(Scr_WatchElement_s *element, bool enabled);
void __cdecl Scr_HitBreakpointInternal(scriptInstance_t inst);
void __cdecl Scr_DisplayHitBreakpointRemote(scriptInstance_t inst);
int __cdecl Scr_HitBreakpoint(
        scriptInstance_t inst,
        VariableValue *top,
        const char *pos,
        unsigned int localId,
        int hitBreakpoint);
bool __cdecl Scr_ConditionalExpression(scriptInstance_t inst, Scr_WatchElement_s *element, unsigned int localId);
void __cdecl Scr_HitBuiltinBreakpoint(
        scriptInstance_t inst,
        VariableValue *top,
        const char *pos,
        unsigned int localId,
        int opcode,
        int builtinIndex,
        unsigned int outparamcount);
void __cdecl Scr_DebugKillThread(scriptInstance_t inst, unsigned int threadId, const char *codePos);
void __cdecl Scr_DebugTerminateThread(scriptInstance_t inst, int topThread);
void __cdecl Scr_ProcessDebugMessages(scriptInstance_t inst);
void __cdecl Scr_ToggleBreakpointRemote(scriptInstance_t inst);
void __cdecl Scr_StepRemote(scriptInstance_t inst);
void __cdecl Scr_RunToCursorRemote(scriptInstance_t inst);
void __cdecl Scr_EnterCallRemote(scriptInstance_t inst);
void __cdecl Scr_ExpandElementRemote(scriptInstance_t inst);
void __cdecl Scr_PasteElementRemote(scriptInstance_t inst);
void __cdecl Scr_InsertElementRemote(scriptInstance_t inst);
void __cdecl Scr_DeleteElementRemote(scriptInstance_t inst);
void __cdecl Scr_BackspaceElementRemote(scriptInstance_t inst);
void __cdecl Scr_AddTextRemote(scriptInstance_t inst);
void __cdecl Scr_CloneElementRemote(scriptInstance_t inst);
void __cdecl Scr_DisplayThreadPosRemote(scriptInstance_t inst);
void __cdecl Scr_ToggleWatchElementBreakpointRemote(scriptInstance_t inst);
void __cdecl Scr_KeepAliveRemote(scriptInstance_t inst);
int __cdecl Scr_UpdateDebugSocket(scriptInstance_t inst);
void __cdecl Sys_ConsolePrintRemote(scriptInstance_t inst, int localClientNum);
void __cdecl Scr_UpdateRemoteDebugger(scriptInstance_t inst);
void __cdecl Scr_UpdateDebugger(scriptInstance_t inst);
char __cdecl Scr_WatchElementHasSameValue(scriptInstance_t inst, Scr_WatchElement_s *element, VariableValue *newValue);
int __cdecl Scr_HitAssignmentBreakpoint(
        scriptInstance_t inst,
        VariableValue *top,
        const char *pos,
        unsigned int localId,
        int forceBreak);
bool __cdecl Scr_IgnoreErrors(scriptInstance_t inst);
int __cdecl CompareThreadIndices(unsigned int *arg1, unsigned int *arg2);
