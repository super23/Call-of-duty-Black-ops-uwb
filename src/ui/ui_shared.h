#pragma once

void __cdecl Menu_Setup(UiContext *dc);
void __cdecl LerpColor(float *a, float *b, float *c, float t);
int __cdecl Color_Parse(const char **p, float (*c)[4]);
int __cdecl String_Parse(const char **p, char *out, int len);
bool __cdecl Command_Parse(char **p, char *out, int len);
bool __cdecl Rect_ContainsPoint(int contextIndex, const rectDef_s *rect, float x, float y);
itemDef_s *__cdecl Menu_GetMatchingItemByNumber(menuDef_t *menu, int index, char *name);
void __cdecl Script_SetColor(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_SetBackground(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
itemDef_s *__cdecl Menu_FindItemByName(menuDef_t *menu, const char *p);
void __cdecl UI_UpdateContextItem(
        UiContext *dc,
        const char *menuName,
        const char *itemName,
        float offsetX,
        float offsetY);
void __cdecl UI_UpdateContextItemInternal(
        UiContext *dc,
        const char *menuName,
        const char *itemName,
        float offsetX,
        float offsetY);
void __cdecl Item_SnapRectClientToMouse(UiContext *dc, itemDef_s *item, float x, float y);
void __cdecl UI_AnimateItem(
        int localClientNum,
        UiContext *dc,
        const char *menuName,
        const char *itemName,
        const char *newStateName,
        int milliseconds);
void __cdecl UI_AnimateItemInternal(
        int localClientNum,
        UiContext *dc,
        const char *menuName,
        const char *itemName,
        const char *newStateName,
        int milliseconds,
        bool sysTime);
animParamsDef_t *__cdecl Item_FindStateByName(itemDef_s *item, const char *p);
void __cdecl Item_AnimateToState(
        int localClientNum,
        UiContext *dc,
        itemDef_s *item,
        animParamsDef_t *state,
        int milliseconds,
        bool sysTime);
void __cdecl Item_CompleteAnimation(itemDef_s *item);
void __cdecl Item_UpdateAnimation(int localClientNum, UiContext *dc, itemDef_s *item);
void __cdecl Item_LerpAnimationParameters(int localClientNum, int contextIndex, itemDef_s *item, float lerpValue);
void __cdecl UI_AnimateItemSysTime(
        int localClientNum,
        UiContext *dc,
        const char *menuName,
        const char *itemName,
        const char *newStateName,
        int milliseconds);
void __cdecl UI_SetShaderTime(int localClientNum);
void __cdecl Script_SetItemColor(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Item_SetColor(int contextIndex, itemDef_s *item, const char *name, float *color);
int __cdecl Menu_ItemsMatchingGroup(menuDef_t *menu, char *name);
int __cdecl Menus_MenuIsInStack(UiContext *dc, menuDef_t *menu);
int __cdecl Menus_MenuInStackLocalClientNum(UiContext *dc, menuDef_t *menu);
menuDef_t *__cdecl Menus_FindByName(const UiContext *dc, const char *p);
void __cdecl Menus_HideByName(const UiContext *dc, const char *menuName);
void __cdecl Menus_ShowByName(const UiContext *dc, const char *windowName);
void __cdecl Menus_CloseByName(int localClientNum, UiContext *dc, const char *p);
void __cdecl Menus_Close(int localClientNum, UiContext *dc, menuDef_t *menu);
void  Menu_RunLeaveFocusScript(
        GenericEventHandler *a1@<ebp>,
        int localClientNum,
        UiContext *dc,
        menuDef_t *menu);
itemDef_s *__cdecl Menu_ClearFocus(int localClientNum, UiContext *dc, menuDef_t *menu);
int __cdecl Menus_RemoveFromStack(int localClientNum, UiContext *dc, menuDef_t *pMenu);
void __cdecl Menu_GainFocusDueToClose(int localClientNum, UiContext *dc, menuDef_t *menu);
void __cdecl Menu_CallOnFocusDueToOpen(int localClientNum, UiContext *dc, menuDef_t *menu);
void  Menu_RunFocusScript(GenericEventHandler *a1@<ebp>, int localClientNum, UiContext *dc, menuDef_t *menu);
void  Menu_RunCloseScript(GenericEventHandler *a1@<ebp>, int localClientNum, UiContext *dc, menuDef_t *menu);
void __cdecl Menus_CompleteItemAnimations(int localClientNum, UiContext *dc, menuDef_t *menu);
void __cdecl Menus_ClearFlagsOnClose(int localClientNum, UiContext *dc, menuDef_t *menu);
void __cdecl Menus_CloseImmediateByName(int localClientNum, UiContext *dc, const char *p);
void __cdecl Menus_RemoveMenu(XAssetHeader header);
char __cdecl Item_DoesEventExist(itemDef_s *item, const char *eventName);
char __cdecl Menus_DoesEventExist(menuDef_t *menu, const char *eventName);
void __cdecl Menus_SetFocusToItem(int localClientNum, UiContext *dc, const char *menuName, const char *itemName);
itemDef_s *__cdecl Menu_FindFirstFocusableItemByName(int localClientNum, UiContext *dc, menuDef_t *menu, const char *p);
void __cdecl Menus_CloseAll(int localClientNum, UiContext *dc);
void __cdecl Menus_CloseAllBehindMain(int localClientNum, UiContext *dc);
void __cdecl UI_ClearErrors();
void __cdecl Script_Show(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Menu_ShowItemByName(int contextIndex, menuDef_t *menu, char *p, int bShow);
void __cdecl Item_Show(int contextIndex, itemDef_s *item, int bShow);
void __cdecl Script_Hide(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_FadeIn(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Menu_FadeItemByName(int contextIndex, menuDef_t *menu, char *p, int fadeOut);
void __cdecl Item_Fade(int contextIndex, itemDef_s *item, int fadeOut);
void __cdecl Script_FadeOut(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_ShowMenu(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_HideMenu(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_Open(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_OpenImmediate(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_OpenForGameType(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_CloseForGameType(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_ActivateBlur(int localClientNum, UiContext *dc, itemDef_s *item);
char __cdecl RemoveMenuFromBlurStack(int localClientNum, UiContext *dc, const char *menuName);
void __cdecl Script_DeactivateBlur(int localClientNum, UiContext *dc, itemDef_s *item);
void __cdecl Script_SetDvarStringUsingTable(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
const char *__cdecl Script_TableLookupParse(UiContext *dc, const char **args);
void __cdecl Script_OpParse(int *operand, const char **args);
int __cdecl Script_IntParse(UiContext *dc, const char **args);
void __cdecl Script_Close(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_CloseImmediate(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_InGameOpen(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_InGameClose(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_FocusFirstInMenu(int localClientNum, UiContext *dc, itemDef_s *item);
itemDef_s *__cdecl Menu_FocusFirstSelectableItem(int localClientNum, UiContext *dc, menuDef_t *menu);
int __cdecl Menu_GetCursorItem(int contextIndex, menuDef_t *menu);
void __cdecl Script_SetFocus(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_SetFocusByDvar(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_ChangeState(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_SetDvar(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_ResetDvar(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_ExecKeyHandler(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_ExecKeyPress(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_Exec(int localClientNum, UiContext *dc, itemDef_s *item, char **args);
void __cdecl Script_ExecHandler(
        int localClientNum,
        int controllerIndex,
        itemDef_s *item,
        char **args,
        void (__cdecl *textCallback)(int, int, itemDef_s *, const char *));
void __cdecl Script_AddTextWrapper(int localClientNum, int controllerIndex, itemDef_s *__formal, const char *text);
void __cdecl Script_ExecNow(int localClientNum, UiContext *dc, itemDef_s *item, char **args);
void __cdecl Script_ExecDvar(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_ExecOnDvarStringValue(int localClientNum, UiContext *dc, itemDef_s *item, char **args);
void __cdecl Script_ConditionalExecHandler(
        int localClientNum,
        UiContext *dc,
        itemDef_s *item,
        char **args,
        bool (__cdecl *shouldExec)(const char *, const char *),
        void (__cdecl *textCallback)(int, int, itemDef_s *, const char *));
bool __cdecl Script_ExecIfStringsEqual(const char *dvarValue, const char *testValue);
void __cdecl Script_ExecOnDvarIntValue(int localClientNum, UiContext *dc, itemDef_s *item, char **args);
bool __cdecl Script_ExecIfIntsEqual(const char *dvarValue, const char *testValue);
void __cdecl Script_ExecOnDvarFloatValue(int localClientNum, UiContext *dc, itemDef_s *item, char **args);
bool __cdecl Script_ExecIfFloatsEqual(const char *dvarValue, const char *testValue);
void __cdecl Script_ExecNowOnDvarStringValue(int localClientNum, UiContext *dc, itemDef_s *item, char **args);
void __cdecl Script_ExecNowOnDvarIntValue(int localClientNum, UiContext *dc, itemDef_s *item, char **args);
void __cdecl Script_ExecNowOnDvarFloatValue(int localClientNum, UiContext *dc, itemDef_s *item, char **args);
void __cdecl Script_RespondOnDvarStringValue(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_ConditionalResponseHandler(
        int localClientNum,
        UiContext *dc,
        itemDef_s *item,
        const char **args,
        bool (__cdecl *shouldRespond)(const char *, const char *));
void __cdecl Script_RespondOnDvarIntValue(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_RespondOnDvarFloatValue(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_SetLocalVarBool(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
UILocalVarContext *__cdecl Script_ParseLocalVar(UiContext *dc, const char **args);
void __cdecl Script_SetLocalVarInt(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_SetLocalVarFloat(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_SetLocalVarString(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_FeederTop(int localClientNum, UiContext *dc, itemDef_s *item);
void __cdecl Script_FeederBottom(int localClientNum, UiContext *dc, itemDef_s *item);
void __cdecl Script_SetDvarFromLocString(int localClientNum, UiContext *dc, itemDef_s *item, char **args);
void __cdecl Script_Play();
void __cdecl Script_ScriptMenuResponse(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
void __cdecl Script_SetUIVisibilityBit(int localClientNum, UiContext *dc, itemDef_s *item, const char **args);
bool __cdecl Item_IsOutcomeAccepted(bool outcome, bool fireOnTrue);
char __cdecl Item_ArePrereqsSatisfied(GenericEventScript *eventScript, nestingStack_t *stack);
void __cdecl Item_MarkProcessed(GenericEventScript *eventScript, nestingStack_t *stack, bool isConditionSatisfied);
char __cdecl Item_IsBlockProcessed(GenericEventScript *eventScript, nestingStack_t *stack, bool *result);
void __cdecl Item_RunSingleEventScript(
        int localClientNum,
        UiContext *dc,
        itemDef_s *item,
        GenericEventScript *eventScript);
bool __cdecl Item_IsConditionSatisfied(
        int localClientNum,
        UiContext *dc,
        itemDef_s *item,
        GenericEventScript *eventScript);
void __cdecl Item_RunEventScript(int localClientNum, UiContext *dc, itemDef_s *item, const char *eventName);
void __cdecl Item_RunScript(int localClientNum, UiContext *dc, itemDef_s *item, const char *s);
void __cdecl UIAnimation_RunEventScript(
        int localClientNum,
        UiContext *dc,
        itemDef_s *item,
        animParamsDef_t *animState,
        const char *eventName);
void __cdecl UI_RegisterCmds();
void __cdecl UI_SetLocalVarBool_f();
void __cdecl UI_SetLocalVarInt_f();
void __cdecl UI_SetLocalVarFloat_f();
void __cdecl UI_SetLocalVarString_f();
void __cdecl UI_FadeItem_f();
void __cdecl UI_ShowItem_f();
void __cdecl UI_HideItem_f();
void __cdecl UI_ShowMenu_f();
void __cdecl UI_HideMenu_f();
void __cdecl UI_OpenMenu_f();
void __cdecl UI_CloseMenu_f();
void __cdecl UI_OpenMenuImmediate_f();
void __cdecl UI_CloseMenuImmediate_f();
void __cdecl UI_ChangeMenuOpenSlideDirection_f();
void __cdecl UI_ChangeMenuCloseSlideDirection_f();
void __cdecl UI_OpenToastPopup_f();
void __cdecl UI_FocusItem_f();
void __cdecl UI_MoveFeeder_f();
void __cdecl UI_AddToFeeder_f();
void __cdecl UI_AddToFeederExtended_f();
void __cdecl UI_ClearFeeder_f();
void __cdecl UI_ClearFeederWithoutResetCursor_f();
void __cdecl UI_RaiseFeederEvent_f();
void __cdecl UI_RefreshFeeder_f();
void __cdecl UI_RefreshFeederSelection_f();
void __cdecl UI_ChangeRowStatus_f();
void __cdecl UI_PlaySound_f();
void __cdecl UI_ValidatePrivateMatchGametype_f();
void __cdecl UI_SetActiveMenu_f();
int __cdecl Item_SetFocus(int localClientNum, UiContext *dc, itemDef_s *item, float x, float y);
textDef_s *__cdecl Item_GetTextRect(int contextIndex, const itemDef_s *item);
const char *__cdecl Item_ListBox_MaxScroll(int localClientNum, int contextIndex, itemDef_s *item);
int __cdecl Item_ListBox_Viewmax(int localClientNum, int contextIndex, itemDef_s *item);
bool __cdecl Window_IsHorizontal(const windowDef_t *w);
double __cdecl Item_ListBox_GetMaxHeight(int localClientNum, int contextIndex, itemDef_s *item);
bool __cdecl Item_ListBox_RequiresScroll(int localClientNum, int contextIndex, itemDef_s *item);
void __cdecl Item_ListBox_SetCursorPos(
        int localClientNum,
        int contextIndex,
        itemDef_s *item,
        int max,
        int viewmax,
        int newCursorPos,
        bool autoScroll);
void __cdecl Item_ListBox_Scroll(
        int localClientNum,
        int contextIndex,
        itemDef_s *item,
        int max,
        int scrollmax,
        int viewmax,
        int delta);
bool __cdecl Item_IsTextField(const itemDef_s *item);
void __cdecl Item_TextField_BeginEdit(int localClientNum, int contextIndex, itemDef_s *item);
void __cdecl Menus_Open(int localClientNum, UiContext *dc, menuDef_t *menu);
void __cdecl Menus_AddToStack(int localClientNum, UiContext *dc, menuDef_t *pMenu);
void __cdecl Menu_LoseFocusDueToOpen(int localClientNum, UiContext *dc, menuDef_t *menu);
int __cdecl Menus_OpenByName(int localClientNum, UiContext *dc, const char *p);
int __cdecl Menus_OpenImmediateByName(int localClientNum, UiContext *dc, const char *p);
void  Menus_SetupOpenMenu(GenericEventHandler *a1@<ebp>, int localClientNum, UiContext *dc, menuDef_t *menu);
itemDef_s *__cdecl Menu_SetNextCursorItem(
        int localClientNum,
        UiContext *dc,
        menuDef_t *menu,
        int usedTab,
        int usedKeyboard);
int __cdecl Item_HasFocusEvent(int localClientNum, UiContext *dc, itemDef_s *item);
void __cdecl Menus_PrintAllLoadedMenus(UiContext *dc);
int  Menu_CheckOnKey@<eax>(int a1@<ebp>, int localClientNum, UiContext *dc, menuDef_t *menu, int key);
void  Menu_HandleKey(int a1@<ebp>, int localClientNum, UiContext *dc, menuDef_t *menu, int key, int down);
itemDef_s *__cdecl Item_LoseFocus(int localClientNum, UiContext *dc, itemDef_s *item);
bool __cdecl Item_IsModal(itemDef_s *item);
int __cdecl Item_ListBox_HandleKey(int localClientNum, UiContext *dc, itemDef_s *item, int key, int down, int force);
void __cdecl Item_ListBox_Page(
        int localClientNum,
        int contextIndex,
        itemDef_s *item,
        int max,
        int scrollmax,
        int viewmax,
        int delta);
bool __cdecl Item_TextField_HandleKey(int localClientNum, UiContext *dc, itemDef_s *item, int key);
int __cdecl Menu_GetItemIndex(menuDef_t *menu, itemDef_s *item);
void __cdecl Item_TextField_EnsureCursorVisible(int contextIndex, itemDef_s *item, const char *text);
int __cdecl Item_HandleKey(int localClientNum, UiContext *dc, itemDef_s *item, int key, int down);
int __cdecl Item_OwnerDraw_HandleKey(itemDef_s *item, int key);
int __cdecl Item_YesNo_HandleKey(UiContext *dc, itemDef_s *item, int key);
bool __cdecl Item_ShouldHandleKey(UiContext *dc, itemDef_s *item, int key);
bool __cdecl Item_ContainsMouse(UiContext *dc, itemDef_s *item);
int __cdecl Item_Multi_HandleKey(UiContext *dc, itemDef_s *item, int key);
int __cdecl Item_Multi_CountSettings(itemDef_s *item);
int __cdecl Item_Multi_FindDvarByValue(itemDef_s *item);
int __cdecl Item_List_NextEntryForKey(int key, int current, int count);
int __cdecl Item_DvarEnum_HandleKey(UiContext *dc, itemDef_s *item, int key);
int __cdecl Item_DvarEnum_CountSettings(itemDef_s *item);
int __cdecl Item_DvarEnum_EnumIndex(itemDef_s *item);
void __cdecl Item_StartCapture(int localClientNum, UiContext *dc, itemDef_s *item, int key);
int __cdecl Item_Slider_OverSlider(int contextIndex, itemDef_s *item, float x, float y);
double __cdecl Item_Slider_ThumbPosition(itemDef_s *item);
double __cdecl Item_GetRectPlacementX(int alignX, float x0, float containerWidth, float selfWidth);
int __cdecl Item_ListBox_OverLB(int localClientNum, int contextIndex, itemDef_s *item, float x, float y);
double __cdecl Item_ListBox_ScrollHeight(int localClientNum, int contextIndex, itemDef_s *item);
double __cdecl Item_ListBox_ThumbPosition(int localClientNum, int contextIndex, itemDef_s *item);
void __cdecl Scroll_ListBox_AutoFunc(int localClientNum, UiContext *dc, void *p);
void __cdecl Scroll_ListBox_ThumbFunc(int localClientNum, UiContext *dc, scrollInfo_s *p);
void __cdecl Scroll_Slider_ThumbFunc(int localClientNum, UiContext *dc, itemDef_s **p);
void __cdecl Scroll_Slider_SetThumbPos(UiContext *dc, itemDef_s *item);
int __cdecl Item_Slider_HandleKey(UiContext *dc, itemDef_s *item, int key);
void __cdecl Item_Action(int localClientNum, UiContext *dc, itemDef_s *item);
itemDef_s *__cdecl Menu_SetPrevCursorItem(int localClientNum, UiContext *dc, menuDef_t *menu, int usedKeyboard);
void __cdecl Menus_HandleOOBClick(int localClientNum, UiContext *dc, menuDef_t *menu, int key, int down);
int __cdecl Display_VisibleMenuCount(UiContext *dc);
rectDef_s *__cdecl Item_CorrectedTextRect(int contextIndex, itemDef_s *item);
const rectDef_s *__cdecl Item_GetNonTextRect(int contextIndex, const itemDef_s *item);
char __cdecl Menu_ItemsAreAnimating(int localClientNum, UiContext *dc, menuDef_t *menu);
void __cdecl Item_TextColor(UiContext *dc, itemDef_s *item, float (*newColor)[4]);
void __cdecl Fade(
        int *flags,
        float *f,
        float clamp,
        int *nextTime,
        int offsetTime,
        int bFlags,
        float fadeAmount,
        float fadeInAmount,
        UiContext *dc);
double __cdecl Item_GetTextPlacementX(int alignX, float x0, float containerWidth, float selfWidth);
double __cdecl Item_GetTextPlacementY(int alignY, float y0, float containerHeight, float selfHeight);
int __cdecl UI_PickWordWrapLineWidth(
        const char *text,
        int bufferSize,
        Font_s *font,
        float normalizedScale,
        int targetLineCount,
        int widthGuess,
        int widthLimit);
void __cdecl UI_DrawWrappedText(
        const ScreenPlacement *scrPlace,
        const char *text,
        const rectDef_s *rect,
        Font_s *font,
        float x,
        float y,
        float scale,
        const float *color,
        int style,
        char textAlignMode,
        rectDef_s *textRect);
void __cdecl DrawWrappedText(
        const ScreenPlacement *scrPlace,
        const char *text,
        const rectDef_s *rect,
        Font_s *font,
        float x,
        float y,
        float scale,
        const float *color,
        int style,
        char textAlignMode,
        rectDef_s *textRect,
        bool subtitle,
        const float *subtitleGlowColor,
        bool cinematic);
void __cdecl UI_DrawWrappedTextSubtitled(
        const ScreenPlacement *scrPlace,
        const char *text,
        const rectDef_s *rect,
        Font_s *font,
        float x,
        float y,
        float scale,
        const float *color,
        int style,
        char textAlignMode,
        rectDef_s *textRect,
        const float *subtitleGlowColor,
        bool cinematic);
char __cdecl UI_GetSticksTranslatedString(int localClientNum, const char *command, char *keys);
int __cdecl UI_GetKeyBindingLocalizedString(int localClientNum, const char *command, char *keys, int bindNum);
void __cdecl GetPlatformCommand(int localClientNum, const char *command, const char **consoleCommand);
int __cdecl Display_KeyBindPending();
int __cdecl Item_Bind_HandleKey(int localClientNum, UiContext *dc, itemDef_s *item, int key, int down);
menuDef_t *__cdecl Menu_GetFocused(UiContext *dc);
void __cdecl Menu_SetFeederSelection(
        int localClientNum,
        UiContext *dc,
        menuDef_t *menu,
        int feeder,
        int index,
        const char *name);
int __cdecl Menu_GetFeederSelection(UiContext *dc, menuDef_t *menu, int feeder, const char *name);
int __cdecl Menus_AnyFullScreenVisible(UiContext *dc);
int __cdecl Menu_HandleMouseMove(int localClientNum, UiContext *dc, menuDef_t *menu);
void __cdecl Item_MouseEnter(int localClientNum, UiContext *dc, itemDef_s *item, float x, float y);
void __cdecl Item_ListBox_MouseEnter(int localClientNum, int contextIndex, itemDef_s *item, float x, float y);
int __cdecl Item_ListBox_GetIndexForMouse(int contextIndex, itemDef_s *item, float x, float y);
void __cdecl Item_ListBox_ModalSelection(int localClientNum, int contextIndex, itemDef_s *item, int x, int y);
void __cdecl Item_Listbox_Mouse_Column_Set(int localClientNum, int contextIndex, itemDef_s *item, int x, int y);
void __cdecl Item_MouseLeave(int localClientNum, UiContext *dc, itemDef_s *item);
void __cdecl Item_SetMouseOver(const UiContext *dc, itemDef_s *item, int focus);
bool __cdecl IsVisible(char flags);
char  Menu_IsVisible@<al>(GenericEventHandler *a1@<ebp>, int localClientNum, UiContext *dc, menuDef_t *menu);
void __cdecl Menu_PerformTransitionEffects(int localClientNum, UiContext *dc, menuDef_t *menu);
char  Menu_Paint@<al>(
        GenericEventHandler *a1@<ebp>,
        int localClientNum,
        UiContext *dc,
        ScreenPlacementStack *scrPlaceViewStack,
        menuDef_t *menu,
        const ScreenPlacement *UI3DOverrideId);
void __cdecl Window_Paint(
        int localClientNum,
        UiContext *dc,
        windowDef_t *w,
        float fadeAmount,
        float fadeInAmount,
        float fadeClamp,
        float fadeCycle,
        int itemType,
        itemDef_s *item);
void __cdecl Item_Paint(int localClientNum, UiContext *dc, itemDef_s *item);
void __cdecl Item_Text_Paint(int localClientNum, UiContext *dc, itemDef_s *item);
Font_s *__cdecl Item_GetFont(int contextIndex, const itemDef_s *item);
void __cdecl Item_SetTextExtents(int contextIndex, itemDef_s *item, const char *text);
void __cdecl ToWindowCoords(float *x, float *y, const windowDef_t *window);
void __cdecl Item_Text_AutoWrapped_Paint(
        int contextIndex,
        itemDef_s *item,
        const char *text,
        const float *color,
        bool subtitle,
        const float *subtitleGlowColor,
        bool cinematic);
void __cdecl Item_TextField_Paint(int localClientNum, UiContext *dc, itemDef_s *item);
void __cdecl Item_YesNo_Paint(int localClientNum, UiContext *dc, itemDef_s *item);
void __cdecl Item_Multi_Paint(int localClientNum, UiContext *dc, itemDef_s *item);
const char *__cdecl Item_Multi_Setting(itemDef_s *item);
void __cdecl Item_DvarEnum_Paint(int localClientNum, UiContext *dc, itemDef_s *item);
const char *__cdecl Item_DvarEnum_Setting(itemDef_s *item);
void __cdecl Item_Slider_Paint(UiContext *dc, itemDef_s *item);
double __cdecl Item_GetRectPlacementY(int alignY, float y0, float containerHeight, float selfHeight);
void __cdecl Item_Bind_Paint(int localClientNum, UiContext *dc, itemDef_s *item);
void __cdecl Item_ListBox_Paint(int localClientNum, UiContext *dc, itemDef_s *item);
double __cdecl Item_ListBox_ThumbDrawPosition(int localClientNum, UiContext *dc, itemDef_s *item);
void __cdecl Item_ListBox_PaintTextElem(
        int localClientNum,
        int contextIndex,
        itemDef_s *item,
        itemGroup_t row,
        int col,
        int maxChars,
        int halignment,
        int valignment,
        float x,
        float y,
        float w,
        float h,
        bool onFocus);
double __cdecl Item_GetTextAlignAdj(int alignment, float width, float textWidth);
double __cdecl Item_ListBox_LanguageScale(language_t language, int feederID, int col);
void __cdecl Item_ListBox_PaintBackground(
        int localClientNum,
        int contextIndex,
        itemDef_s *item,
        float x,
        float y,
        int row);
// local variable allocation has failed, the output may be wrong!
void __cdecl Item_ListBox_PaintHighlight(int localClientNum, int contextIndex, itemDef_s *item, float x, float y);
void __cdecl Item_OwnerDraw_Paint(int localClientNum, UiContext *dc, itemDef_s *item);
void __cdecl Item_GameMsgWindow_Paint(int localClientNum, UiContext *dc, itemDef_s *item);
int __cdecl Menu_Count(UiContext *dc);
void __cdecl Menu_PaintAll_BeginVisibleList(char *stringBegin, unsigned int stringSize);
void __cdecl Menu_PaintAll_AppendToVisibleList(char *stringBegin, unsigned int stringSize, const char *stringToAppend);
void __cdecl Menu_PaintAll_DrawVisibleList(char *stringBegin, UiContext *dc);
void __cdecl Menu_PaintAll(int localClientNum, UiContext *dc);
int __cdecl GetBlurFocus(int localClientNum, UiContext *dc);
void __cdecl UI_SetSystemCursorPos(UiContext *dc, float x, float y);
int __cdecl Display_MouseMove(int localClientNum, UiContext *dc);
int __cdecl UI_ResetCachedMousePosition(int localClientNum, UiContext *dc);
int __cdecl Menu_OverActiveItem(int contextIndex, menuDef_t *menu, float x, float y);
void __cdecl UI_AddMenuList(int localClientNum, UiContext *dc, MenuList *menuList, int close);
void __cdecl UI_AddMenu(int localClientNum, UiContext *dc, menuDef_t *menu, int close);
void __cdecl UI_PlaySound(char context, char *aliasname);
void __cdecl UI_ClearLocalUIVisibilityBits(int localClientNum);
void __cdecl UI_SetLocalUIVisbilityBit(int localClientNum, int bitShift, int value);
int __cdecl UI_GetMenuScreen();
int __cdecl UI_GetForcedMenuScreen();
int __cdecl UI_GetMenuScreenForError();
char __cdecl Menu_DoesMenuOrParentsHaveControlFlag(UiContext *dc, menuDef_t *menu, int controlFlag);
void __cdecl UI_SetLoadingScreenMaterial();
