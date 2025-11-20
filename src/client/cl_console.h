#pragma once

void __cdecl Con_ToggleConsole();
void __cdecl Con_GetTextCopy(char *text, int maxSize);
void __cdecl Con_TimeJumped(int localClientNum, int serverTime);
void __cdecl Con_ResetMessageWindowTimes(MessageWindow *msgwnd, int serverTime);
void __cdecl Con_TimeNudged(int localClientNum, int serverTimeNudge);
void __cdecl Con_NudgeMessageWindowTimes(MessageWindow *msgwnd, int serverTimeNudge, int serverTime);
void __cdecl Con_ClearNotify(int localClientNum);
void __cdecl Con_ClearMessageWindow(MessageWindow *msgwnd);
void __cdecl Con_ClearErrors(int localClientNum);
void __cdecl Con_CheckResize();
void __cdecl Con_OneTimeInit();
void __cdecl Con_InitMessageWindow(
        MessageWindow *msgwnd,
        Message *messages,
        MessageLine *lines,
        char *text,
        int lineCount,
        int padding,
        int textPoolSize,
        int scrollTime,
        int fadeIn,
        int fadeOut);
void __cdecl Con_Init();
void __cdecl Con_ChatModePublic_f();
void __cdecl SetupChatField(int localClientNum, int teamChat, int widthInPixels);
void __cdecl Con_ChatModeTeam_f();
void __cdecl Con_Echo_f();
void __cdecl Con_Clear_f();
void __cdecl Con_InitClientAssets();
void __cdecl Con_InitMessageBuffer();
void __cdecl CL_ConsolePrint(int localClientNum, int channel, const char *txt, int duration, int pixelWidth, int flags);
char __cdecl CL_ConsolePrint_AddLine(
        int localClientNum,
        int channel,
        const char *txt,
        int duration,
        int pixelWidth,
        char color,
        int flags);
void __cdecl Con_UpdateNotifyMessage(int localClientNum, unsigned int channel, int duration, int flags);
void __cdecl Con_UpdateNotifyMessageWindow(
        int localClientNum,
        unsigned int channel,
        int duration,
        int flags,
        print_msg_dest_t dest);
int __cdecl Con_GetDefaultMsgDuration(print_msg_dest_t dest);
void __cdecl Con_UpdateMessage(int localClientNum, MessageWindow *msgwnd, int duration);
MessageWindow *__cdecl Con_GetDestWindow(int localClientNum, print_msg_dest_t dest);
void __cdecl Con_UpdateNotifyLine(int localClientNum, unsigned int channel, bool lineFeed, int flags);
void __cdecl Con_UpdateNotifyLineWindow(
        int localClientNum,
        unsigned int channel,
        bool lineFeed,
        int flags,
        print_msg_dest_t dest);
void __cdecl Con_UpdateMessageWindowLine(int localClientNum, MessageWindow *msgwnd, int linefeed, int flags);
void __cdecl Con_FreeFirstMessageWindowLine(MessageWindow *msgwnd);
void __cdecl Con_CopyCurrentConsoleLineText(MessageWindow *msgwnd, MessageLine *msgLine);
bool __cdecl Con_NeedToFreeMessageWindowLine(MessageWindow *msgwnd, int charCount);
int __cdecl PrintTimeTotal(MessageWindow *msgwnd, MessageLine *line);
int __cdecl PrintableCharsCount(const MessageWindow *msgwnd, MessageLine *line);
int __cdecl GetNextValidPrintTimeForLine(int localClientNum, MessageWindow *msgwnd, char flags);
int __cdecl LatestActiveTypewrittenLineIdx(MessageWindow *msgwnd);
int __cdecl PrintTimeWriteOut(MessageWindow *msgwnd, MessageLine *line);
void __cdecl Con_Linefeed(int localClientNum, unsigned int channel, int flags);
const char *__cdecl CL_TextLineWrapPosition(
        const char *txt,
        int bufferSize,
        int pixelsAvailable,
        Font_s *font,
        float scale,
        int lineBroken);
void __cdecl CL_ConsoleFixPosition();
void __cdecl CL_ReviveMessagePrint(
        int localClientNum,
        char *reviveString,
        Material *iconShader,
        float iconWidth,
        float iconHeight,
        bool horzFlipIcon);
int __cdecl CL_AddMessageChar(char *msg, unsigned int msgLen, unsigned int msgMaxLen, char c);
unsigned int __cdecl CL_AddMessageIcon(
        char *msg,
        unsigned int msgLen,
        unsigned int msgMaxLen,
        Material *iconShader,
        float iconWidth,
        float iconHeight,
        bool horzFlipIcon);
void __cdecl CL_DeathMessagePrint(
        int localClientNum,
        char *attackerName,
        char attackerColorIndex,
        char *victimName,
        char victimColorIndex,
        Material *iconShader,
        float iconWidth,
        float iconHeight,
        bool horzFlipIcon);
void __cdecl Con_AutoCompleteFromList(
        const char **strings,
        unsigned int stringCount,
        const char *prefix,
        char *completed,
        unsigned int sizeofCompleted);
const char *__cdecl Con_TokenizeInput();
char __cdecl Con_AnySpaceAfterCommand();
bool __cdecl Con_IsAutoCompleteMatch(const char *query, const char *matchToText, int matchTextLen);
bool __cdecl Con_HasTooManyMatchesToShow();
bool __cdecl Con_IsDvarCommand(const char *cmd);
void __cdecl Con_ForEach(
        bool isDvarCommand,
        void (__cdecl *callbackDvar)(const char *),
        void (__cdecl *callbackCmd)(const char *));
char __cdecl Con_CycleAutoComplete(int step);
bool __cdecl Con_HasActiveAutoComplete();
char __cdecl Con_CommitToAutoComplete();
char __cdecl Con_CancelAutoComplete();
void __cdecl Con_AllowAutoCompleteCycling(bool isAllowed);
void __cdecl Con_DrawGameMessageWindow(
        int localClientNum,
        unsigned int windowIndex,
        int xPos,
        int yPos,
        int horzAlign,
        int vertAlign,
        Font_s *font,
        float fontScale,
        float *color,
        int textStyle,
        char textAlignMode,
        msgwnd_mode_t mode);
void __cdecl Con_DrawMessageWindow(
        int localClientNum,
        MessageWindow *msgwnd,
        int x,
        int y,
        int charHeight,
        int horzAlign,
        int vertAlign,
        Font_s *font,
        float *color,
        int textStyle,
        float msgwndScale,
        msgwnd_mode_t mode,
        char textAlignMode);
void __cdecl Con_DrawMessageWindowNewToOld(
        int localClientNum,
        MessageWindow *msgwnd,
        int x,
        int y,
        int hudCharHeight,
        int horzAlign,
        int vertAlign,
        bool up,
        Font_s *font,
        float *color,
        int textStyle,
        float msgwndScale,
        char textAlignMode);
bool __cdecl Con_ShouldProcessMessage(Message *message, int serverTime);
void __cdecl Con_DrawMessageLineOnHUD(
        int localClientNum,
        const ScreenPlacement *scrPlace,
        int x,
        int y,
        int charHeight,
        int horzAlign,
        int vertAlign,
        Font_s *font,
        const MessageWindow *msgwnd,
        int lineIdx,
        float *color,
        int textStyle,
        float msgwndScale,
        char textAlignMode);
bool __cdecl LineVisible(const MessageWindow *msgwnd, int lineIdx, int time);
void __cdecl TypewriterSounds(int localClientNum, const MessageWindow *msgwnd, MessageLine *line);
void __cdecl Con_CullFinishedLines(int serverTime, MessageWindow *msgwnd);
double __cdecl Con_GetMessageAlpha(Message *message, MessageWindow *msgwnd, int serverTime, bool scrollsIntoPlace);
void __cdecl Con_DrawMessageWindowOldToNew(
        int localClientNum,
        MessageWindow *msgwnd,
        int x,
        int y,
        int charHeight,
        int horzAlign,
        int vertAlign,
        bool up,
        Font_s *font,
        float *color,
        int textStyle,
        float msgwndScale,
        char textAlignMode);
bool __cdecl CL_ShouldntDrawMessageWindow(int localClientNum);
void __cdecl Con_DrawMiniConsole(int localClientNum, int xPos, int yPos, float alpha);
void __cdecl Con_ClearMiniConsole(int localClientNum);
void __cdecl Con_DrawErrors(int localClientNum, int xPos, int yPos, float alpha);
bool __cdecl Con_IsValidGameMessageWindow(unsigned int windowIndex);
bool __cdecl Con_IsGameMessageWindowActive(int localClientNum, unsigned int windowIndex);
void __cdecl Con_DrawSay(int localClientNum, int x, int y);
void __cdecl Con_ToggleConsoleOutput();
void __cdecl Con_DrawConsole(int localClientNum);
void __cdecl Con_DrawSolidConsole(int localClientNum);
void __cdecl Con_DrawInput(int localClientNum);
void __cdecl ConDrawInput_Text(char *str, const float *color);
void __cdecl ConDrawInput_TextAndOver(char *str, const float *color);
int __cdecl ConDrawInput_TextWidth(const char *text);
void __cdecl ConDrawInput_Box(int lines, const float *color);
void __cdecl ConDraw_Box(float x, float y, float w, float h, const float *color);
void __cdecl ConDrawInput_IncrMatchCounter(const char *str);
void __cdecl ConDrawInput_DvarMatch(char *str);
void __cdecl ConDrawInput_TextLimitChars(char *str, int maxChars, const float *color);
void __cdecl ConDrawInput_DetailedDvarMatch(char *str);
void __cdecl ConDrawInput_AutoCompleteArg(const char **stringList, int stringCount);
int __cdecl ConDrawInput_TextFieldFirstArgChar();
int __cdecl ConDrawInput_GetDvarDescriptionLines(const dvar_s *dvar);
void __cdecl ConDrawInput_DetailedCmdMatch(char *str);
void __cdecl ConDrawInput_CmdMatch(char *str);
void __cdecl Con_DrawAutoCompleteChoice(int localClientNum, bool isDvarCommand, const char *originalCommand);
unsigned int __cdecl Con_GetAutoCompleteColorCodedString(
        char *query,
        const char *matchToText,
        int matchTextLen,
        bool isDvarCommand,
        const char *originalCommand,
        char *colorCoded);
int __cdecl Con_GetAutoCompleteColorCodedStringDiscontiguous(
        const char *query,
        const char *matchToText,
        int matchTextLen,
        char *colorCoded);
int __cdecl Con_GetAutoCompleteColorCodedStringContiguous(
        char *query,
        const char *matchToText,
        int matchTextLen,
        char *colorCoded);
void __cdecl Con_DrawInputPrompt(int localClientNum);
void Con_DrawOuputWindow();
void __cdecl Con_DrawOutputScrollBar(float x, float y, float width, float height);
void __cdecl Con_DrawOutputText(float x, float y);
void __cdecl Con_DrawOutputVersion(float x, float y, float width, float height);
char *__cdecl Con_GetVersionString();
void __cdecl Con_PageUp();
void __cdecl Con_PageDown();
void __cdecl Con_Top();
void __cdecl Con_Bottom();
void __cdecl Con_Close(int localClientNum);
bool __cdecl Con_IsActive(int localClientNum);
void __cdecl CL_PlayTextFXPulseSounds(
        char localClientNum,
        int currentTime,
        int strLength,
        int fxBirthTime,
        int fxLetterTime,
        int fxDecayStartTime,
        int *soundTimeKeeper);
void __cdecl Con_Restricted_Add(const char *name);
int __cdecl Con_Restricted_SigleExist(const char *name);
void __cdecl Con_Restricted_SetState(e_restricted_initState state);
void __cdecl Con_Restricted_ShutDown();
e_restricted_initState __cdecl Con_Restricted_GetState();
void __cdecl Con_Restricted_SecureConfigs();
void __cdecl Con_Restricted_InitLists();
void __cdecl Con_Restricted_LoadTable(const StringTable *table);
void __cdecl Con_Restricted_ResetLists();
void Con_Restricted_SetLists();
void Con_Restricted_Reset();
void __cdecl Con_Restricted_ForEach(
        bool isDvarCommand,
        void (__cdecl *callbackDvar)(const char *),
        void (__cdecl *callbackCmd)(const char *));
void __cdecl Con_Restricted_ExecuteBuf(int localClientNum, int controllerIndex, char *buffer);
void __cdecl Con_Restricted_AddBuf(char *buf);
int __cdecl Con_Restricted_RestrictBuf(const char *buf, int start, int length, char *buffer, int buffer_size);
char __cdecl Con_Restricted_ValidateConfig(const char *fileName);
int __cdecl ConDrawInput_CompareStrings(const char **e0, const char **e1);
