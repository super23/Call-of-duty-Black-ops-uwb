#pragma once
#include <qcommon/cmd.h>
#include <client/screen_placement.h>

enum UILocalVarType : __int32
{                                       // XREF: UILocalVar/r
    UILOCALVAR_INT    = 0x0,
    UILOCALVAR_FLOAT  = 0x1,
    UILOCALVAR_STRING = 0x2,
};

struct UILocalVar // sizeof=0xC
{                                       // XREF: UILocalVarContext/r
    UILocalVarType type;
    const char *name;
    //UILocalVar::<unnamed_type_u> u;
    union //UILocalVar::<unnamed_type_u> // sizeof=0x4
    {                                       // XREF: UILocalVar/r
        int integer;
        float value;
        const char *string;
    } u;
};

struct UILocalVarContext // sizeof=0xC00
{                                       // XREF: UiContext/r
    UILocalVar table[256];
};

struct cursor_t // sizeof=0x8
{                                       // XREF: UiContext/r
                                        // UiContext/r
    float x;
    float y;
};

struct BlurStackEntry // sizeof=0x40
{                                       // XREF: UiContext/r
    char menuName[64];
};

struct rectDef_s // sizeof=0x18
{                                       // XREF: .data:rect/r
    float x;                            // XREF: CG_CompassDrawRound(int,rectDef_s const *,Material *,float * const)+A6/w
    float y;                            // XREF: CG_CompassDrawRound(int,rectDef_s const *,Material *,float * const)+AC/w
    float w;                            // XREF: CG_CompassDrawRound(int,rectDef_s const *,Material *,float * const)+B2/w
    float h;                            // XREF: CG_CompassDrawRound(int,rectDef_s const *,Material *,float * const)+B8/w
    int horzAlign;                      // XREF: CG_CompassDrawRound(int,rectDef_s const *,Material *,float * const)+BE/w
    int vertAlign;                      // XREF: CG_CompassDrawRound(int,rectDef_s const *,Material *,float * const)+C4/w
};

struct windowDef_t // sizeof=0xA4
{                                       // XREF: menuDef_t/r
    const char *name;
    rectDef_s rect;
    rectDef_s rectClient;
    const char *group;
    unsigned __int8 style;
    unsigned __int8 border;
    unsigned __int8 modal;
    unsigned __int8 frameSides;
    float frameTexSize;
    float frameSize;
    int ownerDraw;
    int ownerDrawFlags;
    float borderSize;
    int staticFlags;
    int dynamicFlags[1];
    int nextTime;
    float foreColor[4];
    float backColor[4];
    float borderColor[4];
    float outlineColor[4];
    float rotation;
    Material *background;
};

struct ScriptCondition // sizeof=0x10
{                                       // XREF: ScriptConditionNext/r
    bool fireOnTrue;
    // padding byte
    // padding byte
    // padding byte
    int constructID;
    int blockID;
    ScriptCondition *next;
};

union expressionRpnDataUnion // sizeof=0x8
{                                       // XREF: expressionRpn/r
    Operand constant;
    void *cmd;
    int cmdIdx;
};

struct expressionRpn // sizeof=0xC
{
    int type;
    expressionRpnDataUnion data;
};

struct ExpressionStatement // sizeof=0x10
{                                       // XREF: textExp_s/r
                                        // GenericEventScript/r ...
    char *filename;                     // XREF: PC_CndStackPush(int,ExpressionStatement,bool,bool,int,int,int)+47/r
                                        // PC_EventScript_Parse+398/r ...
    int line;                           // XREF: PC_CndStackPush(int,ExpressionStatement,bool,bool,int,int,int)+4C/r
                                        // PC_EventScript_Parse+3A0/r ...
    int numRpn;                         // XREF: PC_CndStackPush(int,ExpressionStatement,bool,bool,int,int,int)+52/r
                                        // PC_EventScript_Parse+3A9/r ...
    expressionRpn *rpn;                 // XREF: PC_CndStackPush(int,ExpressionStatement,bool,bool,int,int,int)+58/r
                                        // PC_EventScript_Parse+3B2/r ...
};

struct GenericEventScript // sizeof=0x2C
{                                       // XREF: GenericEventScriptNext/r
    ScriptCondition *prerequisites;
    ExpressionStatement condition;
    int type;
    bool fireOnTrue;
    // padding byte
    // padding byte
    // padding byte
    const char *action;
    int blockID;
    int constructID;
    GenericEventScript *next;
};

struct GenericEventHandler // sizeof=0xC
{                                       // XREF: GenericEventHandlerNext/r
    const char *name;
    GenericEventScript *eventScript;
    GenericEventHandler *next;
};

struct __declspec(align(8)) menuDef_t // sizeof=0x190
{                                       // XREF: XAssetPoolEntry<menuDef_t>/r
    windowDef_t window;
    const char *font;
    int fullScreen;
    int ui3dWindowId;
    int itemCount;
    int fontIndex;
    int cursorItem[1];
    int fadeCycle;
    int priority;
    float fadeClamp;
    float fadeAmount;
    float fadeInAmount;
    float blurRadius;
    int openSlideSpeed;
    int closeSlideSpeed;
    int openSlideDirection;
    int closeSlideDirection;
    rectDef_s initialRectInfo;
    int openFadingTime;
    int closeFadingTime;
    int fadeTimeCounter;
    int slideTimeCounter;
    GenericEventHandler *onEvent;
    ItemKeyHandler *onKey;
    ExpressionStatement visibleExp;
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    unsigned __int64 showBits;
    unsigned __int64 hideBits;
    const char *allowedBinding;
    const char *soundName;
    int imageTrack;
    int control;
    float focusColor[4];
    float disableColor[4];
    ExpressionStatement rectXExp;
    ExpressionStatement rectYExp;
    itemDef_s **items;
    // padding byte
    // padding byte
    // padding byte
    // padding byte
};

struct menuClientNum_t // sizeof=0x8
{                                       // XREF: UiContext/r
    menuDef_t *menu;
    int localClientNum;
};

struct UiContext // sizeof=0x1828
{                                       // XREF: .data:uiInfo_s * uiInfoArray/r
                                        // uiInfo_s/r ...
    int contextIndex;
    float bias;
    int realTime;
    int frameTime;
    cursor_t cursor;
    cursor_t prevCursor;
    int isCursorVisible;
    int screenWidth;
    int screenHeight;
    float screenAspect;
    float FPS;
    float blurRadiusOut;
    BlurStackEntry blurMenuStack[8];
    int blurMenuStackIndex;
    menuDef_t *Menus[600];
    int menuCount;
    menuClientNum_t menuStack[16];
    int openMenuCount;
    bool enableSlideAndFadeEffectForMenu;
    // padding byte
    // padding byte
    // padding byte
    UILocalVarContext localVars;
};

enum uiMenuCommand_t : __int32
{                                       // XREF: uiInfo_s/r
    UIMENU_NONE            = 0x0,
    UIMENU_MAIN            = 0x1,
    UIMENU_INGAME          = 0x2,
    UIMENU_PREGAME         = 0x3,
    UIMENU_POSTGAME        = 0x4,
    UIMENU_WM_QUICKMESSAGE = 0x5,
    UIMENU_SCRIPT_POPUP    = 0x6,
    UIMENU_SCOREBOARD      = 0x7,
    UIMENU_GAMERCARD       = 0x8,
    UIMENU_MUTEERROR       = 0x9,
    UIMENU_ENDOFGAME       = 0xA,
};

struct uiInfo_s // sizeof=0x2764
{                                       // XREF: uiInfo_t/r
    UiContext uiDC;
    int playerRefresh;
    int playerIndex;
    int timeIndex;
    int previousTimes[4];
    uiMenuCommand_t currentMenuType;
    bool allowScriptMenuResponse;
    char toastPopupTitle[256];
    char toastPopupDesc[256];
    char toastPopupIconName[256];
    // padding byte
    // padding byte
    // padding byte
    int toastPopupDuration;
    int toastPopupTimeCounter;
    bool toastPopupOpened;
    // padding byte
    // padding byte
    // padding byte
    int contractIndex;
    char findPlayerName[1024];
    char foundPlayerServerAddresses[16][64];
    char foundPlayerServerNames[16][64];
    int numFoundPlayerServers;
    int nextFindPlayerRefresh;
};

struct MenuList // sizeof=0xC
{                                       // XREF: $CD64A558AFC89A5F4974E935559855BB/r
                                        // XAssetPoolEntry<MenuList>/r
    const char *name;
    int menuCount;                      // XREF: Menu_New+7F/r
                                        // Menu_New:loc_77E40A/r ...
    menuDef_t **menus;                  // XREF: UI_LoadMenu_LoadObj+17/w
                                        // Menu_New+9F/r ...
};

struct ConversionArguments // sizeof=0x28
{                                       // XREF: ?LiveNews_PopulateFriendNews@@YAXH_KPAD@Z/r
    int argCount;                       // XREF: CG_DrawWarText:loc_4D6B37/w
    const char *args[9];                // XREF: CG_DrawWarText+325/w
};


uiInfo_s *__cdecl UI_UIContext_GetInfo(int contextIndex);
uiInfo_s *__cdecl UI_GetInfo(int localClientNum);
UILocalVarContext *__cdecl UI_UIContext_GetLocalVarsContext(int contextIndex);
const char *__cdecl UI_GetMonthAbbrev(unsigned int month);
const char **__cdecl UI_GetServerFilter(const char **filter, int filtera);
bool __cdecl UI_KeysBypassMenu(int localClientNum);
char *__cdecl UI_GetMenuBuffer(char *filename);
char *__cdecl GetMenuBuffer_LoadObj(char *filename);
char *__cdecl GetMenuBuffer_FastFile(const char *filename);
void __cdecl UI_DrawBuildNumber(int contextIndex);
void __cdecl UI_DrawSides(
        const ScreenPlacement *scrPlace,
        float x,
        float y,
        float w,
        float h,
        int horzAlign,
        int vertAlign,
        float size,
        const float *color);
void __cdecl UI_DrawTopBottom(
        const ScreenPlacement *scrPlace,
        float x,
        float y,
        float w,
        float h,
        int horzAlign,
        int vertAlign,
        float size,
        const float *color);
void __cdecl UI_DrawRect(
        const ScreenPlacement *scrPlace,
        float x,
        float y,
        float width,
        float height,
        int horzAlign,
        int vertAlign,
        float size,
        const float *color);
void __cdecl UI_DrawHighlightRect(
        const ScreenPlacement *scrPlace,
        float x,
        float y,
        float w,
        float h,
        int horzAlign,
        int vertAlign,
        float size,
        const float *hiColor,
        const float *loColor);
int __cdecl UI_TextWidth(const char *text, int maxChars, Font_s *font, float scale);
int __cdecl UI_TextHeight(Font_s *font, float scale);
void __cdecl UI_DrawText(
        const ScreenPlacement *scrPlace,
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        int horzAlign,
        int vertAlign,
        float scale,
        const float *color,
        int style);
void __cdecl UI_DrawTextRotated(
        const ScreenPlacement *scrPlace,
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        int horzAlign,
        int vertAlign,
        float scale,
        const float *color,
        int style,
        float rotation);
void __cdecl UI_DrawTextWithGlowW(
        const ScreenPlacement *scrPlace,
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        float w,
        int horzAlign,
        int vertAlign,
        float scale,
        const float *color,
        int style,
        const float *glowColor,
        bool subtitle,
        bool cinematic);
void __cdecl UI_DrawTextWithGlow(
        const ScreenPlacement *scrPlace,
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        int horzAlign,
        int vertAlign,
        float scale,
        const float *color,
        int style,
        const float *glowColor,
        bool subtitle,
        bool cinematic);
void __cdecl UI_DrawTextNoSnap(
        const ScreenPlacement *scrPlace,
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        int horzAlign,
        int vertAlign,
        float scale,
        const float *color,
        int style);
void __cdecl UI_DrawTextWithCursor(
        const ScreenPlacement *scrPlace,
        char *text,
        int maxChars,
        Font_s *font,
        float x,
        float y,
        int horzAlign,
        int vertAlign,
        float scale,
        const float *color,
        int style,
        int cursorPos,
        char cursor);
Font_s *__cdecl UI_GetFontHandleDefault(float scale);
Font_s *__cdecl UI_GetFontHandle(const ScreenPlacement *scrPlace, int fontEnum, float scale);
void __cdecl UI_UpdateTime(int localClientNum, int realtime);
void __cdecl UI_Refresh(int localClientNum);
bool __cdecl UI_ShouldDrawBuildNumber(int localClientNum);
bool __cdecl UI_ShouldUpdateFriends();
void __cdecl UI_Shutdown(int localClientNum);
int __cdecl Load_ScriptMenu(int localClientNum, const char *pszMenu, int imageTrack);
MenuList *__cdecl Load_ScriptMenuInternal(const char *pszMenu, int imageTrack);
char *__cdecl UI_GetMapDisplayName(const char *pszMap);
char *__cdecl UI_GetMapDisplayNameFromPartialLoadNameMatch(const char *mapName, int *mapLoadNameLen);
char *__cdecl UI_GetGameTypeDisplayName(const char *pszGameType);
char *__cdecl UI_GetGameTypeDisplayShortName(const char *pszGameType);
void __cdecl UI_Update(const char *name);
void __cdecl UI_VerifyLanguage();
void __cdecl UI_UpdateDisplayServers(int localClientNum, uiInfo_s *uiInfo);
int __cdecl UI_GetClientNumForPlayerListNum(int playerListIndex);
void __cdecl UI_RunMenuScript(int localClientNum, int contextIndex, __int64 args);
char __cdecl UI_GetOpenOrCloseMenuOnDvarArgs(
        const char **args,
        const char *cmd,
        char *dvarName,
        int dvarNameLen,
        char *testValue,
        int testValueLen,
        char *menuName,
        int menuNameLen);
void __cdecl UI_OpenMenuOnDvar(
        int localClientNum,
        uiInfo_s *uiInfo,
        const char *cmd,
        const char *menuName,
        const char *dvarName,
        const char *testValue);
bool __cdecl UI_DvarValueTest(const char *cmd, const char *dvarName, const char *testValue, bool wantMatch);
void __cdecl UI_CloseMenuOnDvar(
        int localClientNum,
        uiInfo_s *uiInfo,
        const char *cmd,
        const char *menuName,
        const char *dvarName,
        const char *testValue);
void __cdecl UI_Pause(int localClientNum, int b);
void __cdecl UI_MouseEvent(int localClientNum, int x, int y);
uiMenuCommand_t __cdecl UI_GetActiveMenu(int localClientNum);
const char *__cdecl UI_GetTopActiveMenuName(int localClientNum);
int __cdecl UI_IsFullscreen(int localClientNum);
double __cdecl UI_GetBlurRadius(int localClientNum);
char *__cdecl UI_SafeTranslateString(const char *reference);
bool __cdecl UI_AnyMenuVisible(int localClientNum);
char *__cdecl UI_ReplaceConversionString(char *sourceString, const char *replaceString);
char *__cdecl UI_ReplaceConversionStrings(char *sourceString, int numStrings, const char **replaceStrings);
char *__cdecl UI_ReplaceConversionInt(char *sourceString, int replaceInt);
char *__cdecl UI_ReplaceConversionInts(char *sourceString, int numInts, int *replaceInts);
void __cdecl UI_FilterStringForButtonAnimation(char *str, unsigned int strMaxSize);
void __cdecl UI_ReplaceConversions(
        char *sourceString,
        ConversionArguments *arguments,
        char *outputString,
        int outputStringSize);
void __cdecl UI_CloseAll(int localClientNum);
void __cdecl UI_CloseFocusedMenu(int localClientNum);
bool __cdecl Menu_IsMenuOpenAndVisible(int localClientNum, const char *menuName);
int __cdecl UI_PopupScriptMenu(int localClientNum, const char *menuName, bool useMouse);
void __cdecl UI_ClosePopupScriptMenu(int localClientNum, bool allowResponse);
bool __cdecl UI_AllowScriptMenuResponse(int localClientNum);
void __cdecl UI_CloseInGameMenu(int localClientNum);
void __cdecl UI_UpdatePendingPings(uiInfo_s *uiInfo);
int __cdecl UI_CheckExecKey(int localClientNum, int key);
int __cdecl UI_OwnerDrawWidth(int ownerDraw, Font_s *font, float scale);
char __cdecl UI_DrawRecordLevel(int contextIndex, rectDef_s *rect);
int __cdecl UI_GetTalkerClientNum(int localClientNum, int contextIndex, int num);
void __cdecl UI_DrawPrettyLine(
        int contextIndex,
        float *p1,
        float *p2,
        float lineWidth,
        int horzAlign,
        int vertAlign,
        float *color);
void __cdecl UI_OwnerDraw(
        int localClientNum,
        itemDef_s *item,
        float x,
        float y,
        float w,
        float h,
        int horzAlign,
        int vertAlign,
        int ownerDraw,
        int ownerDrawFlags,
        float *color,
        Material *material,
        rectDef_s parentRect,
        const char *dvarName);
void __cdecl UI_DrawLocalTalking(int contextIndex, const rectDef_s *rect, const float *color);
void __cdecl UI_DrawPlaylistIcon(int localClientNum, const rectDef_s *rect, const float *color);
void __cdecl UI_DrawCategoryIcon(int localClientNum, rectDef_s *rect, float *color);
void __cdecl UI_DrawControllerIcon(int localClientNum, int contextIndex, const rectDef_s *rect, const float *color);
void __cdecl UI_PrettyLineOwnerdraw(int contextIndex, rectDef_s *rect, float lineWidth, float *color);
void __cdecl UI_OwnerDrawText(
        int localClientNum,
        int contextIndex,
        itemDef_s *item,
        float x,
        float y,
        float w,
        float h,
        int horzAlign,
        int vertAlign,
        float text_x,
        float text_y,
        int ownerDraw,
        int ownerDrawFlags,
        int align,
        Font_s *font,
        float scale,
        float *color,
        Material *material,
        int textStyle,
        rectDef_s parentRect,
        int textAlignMode,
        char *text);
void __cdecl UI_DrawKeyBindStatus(
        int contextIndex,
        rectDef_s *rect,
        Font_s *font,
        float scale,
        float *color,
        int textStyle);
void __cdecl UI_DrawTalkerNum(
        int localClientNum,
        int contextIndex,
        int num,
        rectDef_s *rect,
        Font_s *font,
        float *color,
        float textScale,
        int style);
void __cdecl UI_DrawReservedSlots(
        int contextIndex,
        rectDef_s *rect,
        Font_s *font,
        float *color,
        float textScale,
        int style);
void __cdecl UI_DrawLoggedInUser(
        int localClientNum,
        int contextIndex,
        rectDef_s *rect,
        Font_s *font,
        float scale,
        float *color,
        int textStyle);
double __cdecl UI_GetTextAlignAdj(int alignment, float width, float textWidth);
void __cdecl UI_DrawLoggedInUserName(
        int localClientNum,
        int contextIndex,
        rectDef_s *rect,
        Font_s *font,
        float scale,
        float *color,
        int textStyle);
void __cdecl UI_DrawPlaylistDescription(
        int localClientNum,
        rectDef_s *rect,
        Font_s *font,
        float *color,
        float textScale,
        int style,
        char textAlignment);
char *__cdecl GetXpLockDescription(int localClientNum, int playlistId);
char *__cdecl GetPLevelLockDescription(int localClientNum, int playlistId);
void __cdecl UI_DrawScrollingText(
        int contextIndex,
        rectDef_s *rect,
        Font_s *font,
        float scale,
        float *color,
        int style);
void __cdecl UI_DrawScrollingTextCategory(
        int contextIndex,
        rectDef_s *rect,
        Font_s *font,
        float scale,
        float *color,
        int style);
bool __cdecl UI_OwnerDrawVisible(__int16 flags);
int __cdecl UI_OwnerDrawHandleKey(int ownerDraw, int flags, int key);
void __cdecl UI_InitOnceForAllClients();
void UI_RegisterDvars();
void UI_AssetCache();
void UI_InitUIInfos();
void __cdecl UI_Init(int localClientNum);
char __cdecl UI_KeyEvent_AutoJoinButtonPressed(int localClientNum, UiContext *dc, menuDef_t *menu, int key, int down);
void __cdecl UI_KeyEvent(int localClientNum, int key, int down);
bool __cdecl UI_KeyEvent_CancelButtonPressed(UiContext *dc, menuDef_t *menu, int key, int down);
void __cdecl UI_CloseAllMenus(int localClientNum);
void __cdecl UI_UIContext_OpenMenu(int localClientNum, int contextIndex, const char *menuName);
void __cdecl UI_UIContext_CloseMenu(int localClientNum, int contextIndex, const char *menuName);
void __cdecl UI_UIContext_CloseMenuImmediate(int localClientNum, int contextIndex, const char *menuName);
void __cdecl UI_UIContext_OpenToastPopup(
        int contextIndex,
        const char *toastPopupIconName,
        const char *toastPopupTitle,
        const char *toastPopupDesc,
        int toastPopupDuration);
void __cdecl UI_OpenMenu(int localClientNum, const char *menuName);
void __cdecl UI_CloseMenu(int localClientNum, const char *menuName);
void __cdecl UI_CloseMenuImmediate(int localClientNum, const char *menuName);
void __cdecl UI_OpenToastPopup(
        int localClientNum,
        const char *toastPopupIconName,
        const char *toastPopupTitle,
        const char *toastPopupDesc,
        int toastPopupDuration);
void __cdecl UI_DrawMapLevelshot(int localClientNum);
void __cdecl UI_LoadIngameMenus(int contextIndex);
unsigned __int8 __cdecl UI_ToUpper_CodePage_1250(unsigned __int8 character);
unsigned __int8 __cdecl UI_ToUpper_CodePage_1251(unsigned __int8 character);
unsigned __int8 __cdecl UI_ToUpper_CodePage_1252(unsigned __int8 character);
unsigned __int8 __cdecl UI_CharToUpper(unsigned __int8 character);
char *__cdecl UI_ToUpper(const char *sourceString);
