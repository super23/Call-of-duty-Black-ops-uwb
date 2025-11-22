#pragma once

struct Material;

struct rectDef_s // sizeof=0x18
{                                       // XREF: .data:rect/r
                                        // columnInfo_s/r ...
    float x;                            // XREF: CG_CompassDrawRound(int,rectDef_s const *,Material *,float * const)+A6/w
                                        // CG_CompassDrawRound(int,rectDef_s const *,Material *,float * const)+F4/r ...
    float y;                            // XREF: CG_CompassDrawRound(int,rectDef_s const *,Material *,float * const)+AC/w
                                        // CG_CompassDrawRound(int,rectDef_s const *,Material *,float * const)+D7/o ...
    float w;                            // XREF: CG_CompassDrawRound(int,rectDef_s const *,Material *,float * const)+B2/w
                                        // CG_CompassDrawRound(int,rectDef_s const *,Material *,float * const)+D3/o ...
    float h;                            // XREF: CG_CompassDrawRound(int,rectDef_s const *,Material *,float * const)+B8/w
                                        // CG_CompassDrawRound(int,rectDef_s const *,Material *,float * const)+CF/o ...
    int horzAlign;                      // XREF: CG_CompassDrawRound(int,rectDef_s const *,Material *,float * const)+BE/w
                                        // CG_CompassDrawRound(int,rectDef_s const *,Material *,float * const)+CB/r ...
    int vertAlign;                      // XREF: CG_CompassDrawRound(int,rectDef_s const *,Material *,float * const)+C4/w
                                        // CG_CompassDrawRound(int,rectDef_s const *,Material *,float * const)+C7/r ...
};

struct windowDef_t // sizeof=0xA4
{                                       // XREF: menuDef_t/r
                                        // itemDef_s/r ...
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
    struct Material *background;
};

enum expDataType : __int32
{                                       // XREF: Operand/r
                                        // operandDataType/r ...
    VAL_INT    = 0x0,
    VAL_FLOAT  = 0x1,
    VAL_STRING = 0x2,
};

union operandInternalDataUnion // sizeof=0x4
{                                       // XREF: Expression_Parse(char const * *,ExpressionStatement *,void *,int)+19/w
                                        // Expression_Parse(char const * *,ExpressionStatement *,void *,int):loc_78A62C/r ...
    int intVal;
    float floatVal;
    const char *string;
};

struct Operand // sizeof=0x8
{                                       // XREF: GetExpressionStringInt(int,itemDef_s *,ExpressionStatement *,char const * *,int *)+40/o
                                        // expressionRpnDataUnion/r ...
    expDataType dataType;               // XREF: Expression_Parse(char const * *,ExpressionStatement *,void *,int)+16/w
                                        // Expression_Parse(char const * *,ExpressionStatement *,void *,int)+1A0/r ...
    operandInternalDataUnion internals; // XREF: Expression_Parse(char const * *,ExpressionStatement *,void *,int)+19/w
                                        // Expression_Parse(char const * *,ExpressionStatement *,void *,int):loc_78A62C/r ...
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

struct textExp_s // sizeof=0x10
{                                       // XREF: textExp_t/r
    ExpressionStatement textExp;
};

struct gameMsgDef_s // sizeof=0x8
{                                       // XREF: gameMsgDef_t/r
    int gameMsgWindowIndex;
    int gameMsgWindowMode;
};

struct columnInfo_s // sizeof=0x20
{                                       // XREF: listBoxDef_s/r
    int elementStyle;
    int maxChars;
    rectDef_s rect;
};

struct MenuCell // sizeof=0xC
{                                       // XREF: ItemParse_menuItemsDef/r
    int type;                           // XREF: ItemParse_menuItemsDef+375/r
    int maxChars;                       // XREF: ItemParse_menuItemsDef+340/r
                                        // ItemParse_menuItemsDef+38A/r
    char *stringValue;
};

struct MenuRow // sizeof=0x18
{
    MenuCell *cells;
    char *eventName;
    char *onFocusEventName;
    bool disableArg;
    // padding byte
    // padding byte
    // padding byte
    int status;
    int name;
};

struct listBoxDef_s // sizeof=0x29C
{                                       // XREF: listBoxDef_t/r
    int mousePos;
    int cursorPos[1];
    int startPos[1];
    int endPos[1];
    int drawPadding;
    float elementWidth;
    float elementHeight;
    int numColumns;
    float special;
    columnInfo_s columnInfo[16];
    int notselectable;
    int noScrollBars;
    int usePaging;
    float selectBorder[4];
    float disableColor[4];
    float focusColor[4];
    float elementHighlightColor[4];
    float elementBackgroundColor[4];
    Material *selectIcon;
    Material *backgroundItemListbox;
    Material *highlightTexture;
    int noBlinkingHighlight;
    MenuRow *rows;
    int maxRows;
    int rowCount;
};

struct multiDef_s // sizeof=0x18C
{                                       // XREF: multiDef_t/r
    const char *dvarList[32];
    const char *dvarStr[32];
    float dvarValue[32];
    int count;
    int actionOnEnterPressOnly;
    int strDef;
};

struct editFieldDef_s // sizeof=0x24
{                                       // XREF: editFieldDef_t/r
    int cursorPos[1];
    float minVal;
    float maxVal;
    float defVal;
    float range;
    int maxChars;
    int maxCharsGotoNext;
    int maxPaintChars;
    int paintOffset;
};

struct enumDvarDef_s // sizeof=0x4
{                                       // XREF: enumDvarDef_t/r
    const char *enumDvarName;
};

union focusDefData_t // sizeof=0x4
{                                       // XREF: focusItemDef_s/r
    listBoxDef_s *listBox;
    multiDef_s *multi;
    editFieldDef_s *editField;
    enumDvarDef_s *enumDvar;
    void *data;
};

struct focusItemDef_s // sizeof=0x18
{                                       // XREF: focusItemDef_t/r
    const char *mouseEnterText;
    const char *mouseExitText;
    const char *mouseEnter;
    const char *mouseExit;
    struct ItemKeyHandler *onKey;
    focusDefData_t focusTypeData;
};

union textDefData_t // sizeof=0x4
{                                       // XREF: textDef_s/r
    focusItemDef_s *focusItemDef;
    gameMsgDef_s *gameMsgDef;
    void *data;
};


struct textDef_s // sizeof=0x44
{                                       // XREF: textDef_t/r
    rectDef_s textRect[1];
    int alignment;
    int fontEnum;
    int itemFlags;
    int textAlignMode;
    float textalignx;
    float textaligny;
    float textscale;
    int textStyle;
    const char *text;
    textExp_s *textExpData;
    textDefData_t textTypeData;
};

struct imageDef_s // sizeof=0x10
{                                       // XREF: imageDef_t/r
    ExpressionStatement materialExp;
};

struct ownerDrawDef_s // sizeof=0x10
{                                       // XREF: ownerDrawDef_t/r
    ExpressionStatement dataExp;
};

union itemDefData_t // sizeof=0x4
{                                       // XREF: itemDef_s/r
    textDef_s *textDef;
    imageDef_s *imageDef;
    focusItemDef_s *blankButtonDef;
    ownerDrawDef_s *ownerDrawDef;
    void *data;
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

struct ItemKeyHandler // sizeof=0xC
{                                       // XREF: ItemKeyHandlerNext/r
    int key;
    GenericEventScript *keyScript;
    ItemKeyHandler *next;
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
    struct itemDef_s **items;
    // padding byte
    // padding byte
    // padding byte
    // padding byte
};

struct rectData_s // sizeof=0x40
{                                       // XREF: rectData_t/r
    ExpressionStatement rectXExp;
    ExpressionStatement rectYExp;
    ExpressionStatement rectWExp;
    ExpressionStatement rectHExp;
};

struct animParamsDef_t // sizeof=0x6C
{                                       // XREF: UIAnimInfo/r
                                        // UIAnimInfo/r ...
    const char *name;
    rectDef_s rectClient;
    float borderSize;
    float foreColor[4];
    float backColor[4];
    float borderColor[4];
    float outlineColor[4];
    float textScale;
    float rotation;
    GenericEventHandler *onEvent;
};

struct UIAnimInfo // sizeof=0xEC
{
    int animStateCount;
    animParamsDef_t **animStates;
    animParamsDef_t currentAnimState;
    animParamsDef_t nextAnimState;
    int animating;
    int animStartTime;
    int animDuration;
};

struct __declspec(align(8)) itemDef_s // sizeof=0x110
{                                       // XREF: itemDef_t/r
                                        // Menu_RunLeaveFocusScript/r ...
    windowDef_t window;
    int type;
    int dataType;
    int imageTrack;
    const char *dvar;
    const char *dvarTest;
    const char *enableDvar;
    int dvarFlags;
    itemDefData_t typeData;
    menuDef_t *parent;
    rectData_s *rectExpData;
    ExpressionStatement visibleExp;
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    unsigned __int64 showBits;
    unsigned __int64 hideBits;
    ExpressionStatement forecolorAExp;
    int ui3dWindowId;
    GenericEventHandler *onEvent;
    UIAnimInfo *animInfo;               // XREF: Menu_RunLeaveFocusScript+10/w
                                        // Menu_RunFocusScript+10/w ...
    // padding byte
    // padding byte
    // padding byte
    // padding byte
};

struct CmdArgs // sizeof=0x28EC
{                                       // XREF: Cmd_Init(void)+10/o
                                        // Com_InitThreadData(int)+50/o ...
    int nesting;                        // XREF: SV_Cmd_Argc(void)+3/r
                                        // SV_Cmd_Argc(void)+E/r ...
    int localClientNum[8];
    int controllerIndex[8];
    itemDef_s *itemDef[8];
    int argshift[8];
    int argc[8];                        // XREF: SV_Cmd_Argc(void)+3A/r
                                        // SV_Cmd_Argv(int)+6E/r
    const char **argv[8];               // XREF: SV_Cmd_Argv(int)+7D/r
    char textPool[8192];
    const char *argvPool[512];
    int usedTextPool[8];
    int totalUsedArgvPool;              // XREF: SV_Cmd_TokenizeString(char const *)+F/r
    int totalUsedTextPool;
};


struct __declspec(align(4)) cmd_function_s // sizeof=0x18
{                                       // XREF: .data:BG_EmblemsFlushResults_VAR/r
                                        // .data:BG_EmblemsPurchaseBackgroundByIndex_f_VAR/r ...
    cmd_function_s *next;
    const char *name;
    const char *autoCompleteDir;
    const char *autoCompleteExt;
    void (__cdecl *function)();
    bool consoleAccess;
    // padding byte
    // padding byte
    // padding byte
};


void __cdecl Cmd_Wait_f();
void __cdecl Cbuf_Init();
void __cdecl Cbuf_AddText(int localClientNum, const char *text);
void __cdecl Cbuf_InsertText(int localClientNum, const char *text);
void __cdecl Cbuf_AddServerText_f();
void __cdecl Cmd_ExecuteServerString(char *text);
void __cdecl Cmd_ShiftArgs(int count, CmdArgs *args);
void __cdecl Cbuf_SV_Execute();
void __cdecl Cmd_AddServerCommandInternal(const char *cmdName, void (__cdecl *function)(), cmd_function_s *allocedCmd);
void __cdecl Cbuf_ExecuteBuffer_Restricted(int localClientNum, int controllerIndex, char *buffer);
void __cdecl Cbuf_ExecuteBufferInternal(
        int localClientNum,
        int controllerIndex,
        itemDef_s *item,
        char *buffer,
        char ***dvarWhitelistArray,
        bool restrict);
void __cdecl Cmd_ExecSafeDvarSingleCommmandOnly(
        int localClientNum,
        int controllerIndex,
        char *singleCommand,
        const char ***dvarWhitelistArray);
void __cdecl Cbuf_ExecuteBuffer(int localClientNum, int controllerIndex, char *buffer);
void __cdecl Cbuf_ExecuteBufferUI(int localClientNum, int controllerIndex, itemDef_s *item, char *buffer);
void __cdecl Cbuf_Execute(int localClientNum, int controllerIndex);
void __cdecl Cbuf_ExecuteInternal(int localClientNum, int controllerIndex);
void __cdecl Cmd_Vstr_f();
void __cdecl SV_Cmd_ArgvBuffer(int arg, char *buffer, int bufferLength);
void __cdecl Cmd_ArgsBuffer(int start, char *buffer, int bufLength);
void __cdecl Cmd_TokenizeStringWithLimit(const char *text_in, int max_tokens);
void __cdecl Cmd_TokenizeStringKernel(
        int localClientNum,
        int localControllerIndex,
        itemDef_s *item,
        const char *text_in,
        int max_tokens,
        bool evalExpressions,
        CmdArgs *args);
int __cdecl Cmd_TokenizeStringInternal(
        const char *text_in,
        int max_tokens,
        bool evalExpressions,
        const char **argv,
        CmdArgs *argsPriv);
bool __cdecl Cmd_IsWhiteSpaceChar(unsigned __int8 letter);
void __cdecl Cmd_EmitString(const char *str, CmdArgs *argsPriv);
void __cdecl Cmd_EvaluateExpression(const char **text_in, CmdArgs *argsPriv);
itemDef_s *__cdecl Cmd_ItemDef();
void __cdecl AssertCmdArgsConsistency(const CmdArgs *args);
void __cdecl Cmd_TokenizeStringWithLimitNoEval(const char *text_in, int max_tokens);
void __cdecl Cmd_TokenizeString(const char *text_in);
void __cdecl Cmd_TokenizeStringNoEval(const char *text_in);
void __cdecl Cmd_EndTokenizedString();
void __cdecl Cmd_EndTokenizedStringKernel(CmdArgs *args);
void __cdecl SV_Cmd_TokenizeString(const char *text_in);
void __cdecl SV_Cmd_EndTokenizedString();
cmd_function_s *__cdecl Cmd_FindCommand(const char *cmdName);
void __cdecl Cmd_AddCommandInternal(const char *cmdName, void (__cdecl *function)(), cmd_function_s *allocedCmd);
void __cdecl Cmd_RemoveCommand(const char *cmdName);
void __cdecl Cmd_SetAutoComplete(const char *cmdName, const char *dir, const char *ext);
void __cdecl Cmd_Shutdown();
void __cdecl Cmd_ForEach(void (__cdecl *callback)(const char *));
void __cdecl Cmd_ResetConAccessCommands();
char __cdecl Cmd_SetConAccess(const char *str);
void __cdecl Cmd_ForEachConsoleAccessName(void (__cdecl *callback)(const char *));
const char **__cdecl Cmd_GetAutoCompleteFileList(const char *cmdName, int *fileCount);
void __cdecl Cmd_ComErrorCleanup();
void __cdecl Cmd_ResetArgs(CmdArgs *args);
void __cdecl Cmd_ExecuteSingleCommandInternal(
        int localClientNum,
        int controllerIndex,
        itemDef_s *item,
        char *text,
        bool restrict);
void __cdecl Cmd_ExecuteSingleCommand(int localClientNum, int controllerIndex, char *text);
void __cdecl Cmd_List_f();
void __cdecl Cmd_Init();
void __cdecl Cmd_Exec_f();
char __cdecl Cmd_ExecFromDisk(int localClientNum, int controllerIndex, const char *filename);
char __cdecl Cmd_ExecFromDiskAddText(int localClientNum, int controllerIndex, const char *filename);
char __cdecl Cmd_ExecFromFastFile(int localClientNum, int controllerIndex, char *filename);
void __cdecl Cmd_ExecAddText_f();
