#pragma once

void __cdecl Field_DrawTextOverride(
        int localClientNum,
        const field_t *edit,
        int x,
        int y,
        int horzAlign,
        int vertAlign,
        char *str,
        int drawLen,
        int cursorPos);
void __cdecl Field_Draw(int localClientNum, field_t *edit, int x, int y, int horzAlign, int vertAlign);
void __cdecl Field_AdjustScroll(const ScreenPlacement *scrPlace, field_t *edit);
void __cdecl Console_Key(int localClientNum, int key);
char __cdecl Field_KeyDownEvent(int localClientNum, const ScreenPlacement *scrPlace, field_t *edit, int key);
char __cdecl Field_Paste(int localClientNum, const ScreenPlacement *scrPlace, field_t *edit);
bool __cdecl Field_CharEvent(int localClientNum, const ScreenPlacement *scrPlace, field_t *edit, int ch);
void CompleteCommand();
void __cdecl PrintMatches(const char *s);
void __cdecl ConcatRemaining(char *src, char *start);
int keyConcatArgs();
void CompleteCmdArgument();
void __cdecl ReplaceConsoleInputArgument(int replaceCount, const char *replacement);
void CompleteDvarArgument();
void __cdecl UpdateMatches(bool searchCmds, int *matchLenAfterCmds, int *matchLenAfterDvars);
void __cdecl FindMatches(const char *s);
bool __cdecl Console_IsRconCmd(const char *commandString);
char __cdecl Console_IsClientDisconnected();
int __cdecl Key_GetOverstrikeMode(int localClientNum);
void __cdecl Key_SetOverstrikeMode(int localClientNum, int state);
int __cdecl Key_IsDown(int localClientNum, int keynum);
int __cdecl Key_StringToKeynum(const char *str);
bool __cdecl Key_IsValidGamePadChar(char key);
const char *__cdecl Key_KeynumToString(int keynum, int translate);
void __cdecl Key_SetBinding(int localClientNum, int keynum, char *binding, int bindNum);
void __cdecl Key_SetBindings(int localClientNum, int *twokeys, char *binding, int bindNum);
const char *__cdecl Key_GetBinding(int localClientNum, unsigned int keynum, int bindNum);
int __cdecl Key_GetCommandAssignment(int localClientNum, const char *command, int *twokeys, int bindNum);
int __cdecl Key_GetCommandAssignmentInternal(
        int localClientNum,
        const char *command,
        int *twokeys,
        int gamePadOnly,
        int bindNum);
bool __cdecl Key_IsCommandBound(int localClientNum, const char *command, int bindNum);
void __cdecl Key_Unbind_f();
void __cdecl Key_Unbind2_f();
void __cdecl Key_Unbindall_f();
void __cdecl Key_Unbindall2_f();
void __cdecl Key_Bind_f();
void __cdecl Key_Bind2_f();
void __cdecl Key_WriteBindingsToTempBuf(int localClientNum);
void __cdecl Key_RestoreBindingsFromTempBuf(int localClientNum);
void __cdecl Key_WriteBindings(int localClientNum, int f);
int __cdecl Key_WriteBindingsToBuffer(int localClientNum, char *buffer, int bufferSize);
void __cdecl Key_Bindlist_f();
void __cdecl CL_InitKeyCommands();
void __cdecl CL_KeyEvent(int localClientNum, int key, int down, unsigned int time);
void __cdecl Message_Key(int localClientNum, int key);
bool __cdecl CL_MouseInputShouldBypassMenus(int localClientNum, int key);
void __cdecl CL_HandleKeySpectatorInput(int localClientNum, int key);
void __cdecl CL_CharEvent(int localClientNum, int key);
void __cdecl CL_ConsoleCharEvent(int localClientNum, int key);
void __cdecl Key_ClearStates(int localClientNum);
int __cdecl CL_GetKeyBinding(int localClientNum, const char *command, char (*keyNames)[128], int bindNum);
int __cdecl CL_GetKeyBindingInternal(
        int localClientNum,
        const char *command,
        char (*keyNames)[128],
        int gamePadOnly,
        int bindNum);
int __cdecl CL_GetGamePadBinding(int localClientNum, const char *command, char (*keyNames)[128], int bindNum);
int __cdecl CL_IsKeyPressed(int localClientNum, const char *keyName);
void __cdecl Key_Shutdown();
bool __cdecl Key_IsCatcherActive(int localClientNum, int mask);
void __cdecl Key_AddCatcher(int localClientNum, int orMask);
void __cdecl Key_RemoveCatcher(int localClientNum, int andMask);
void __cdecl Key_SetCatcher(int localClientNum, int catcher);
void __cdecl Key_ContextIndex_SetCatcher(int contextIndex, int catcher);
