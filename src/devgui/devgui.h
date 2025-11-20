#pragma once

void __cdecl DevGui_AddDvar(const char *path, const dvar_s *dvar);
devguiGlob_t *__cdecl DevGui_GetMenu(unsigned __int16 handle);
unsigned __int16 __cdecl DevGui_ConstructPath_r(unsigned __int16 parent, const char *path);
unsigned __int16 __cdecl DevGui_RegisterMenu(unsigned __int16 parentHandle, const char *label);
int __cdecl DevGui_CreateMenu();
unsigned __int16 __cdecl DevGui_GetMenuHandle(DevMenuItem *menu);
int __cdecl DevGui_CompareMenus(const DevMenuItem *menu0, const DevMenuItem *menu1);
unsigned __int16 __cdecl DevGui_FindMenu(unsigned __int16 parentHandle, const char *label);
int __cdecl DevGui_PathToken(const char **pathInOut, char *label, __int16 *sortKeyOut);
char __cdecl DevGui_IsValidPath(const char *path);
void __cdecl DevGui_AddCommand(const char *path, char *command);
void __cdecl DevGui_AddGraph(const char *path, DevGraph *graph);
void __cdecl DevGui_RemoveMenu(const char *path);
void __cdecl DevGui_FreeMenu_r(unsigned __int16 handle);
void __cdecl DevGui_OpenMenu(const char *path);
bool __cdecl DevGui_EditableMenuItem(const DevMenuItem *menu);
void __cdecl DevGui_Draw(int localClientNum);
unsigned __int16 __cdecl DevGui_GetMenuParent(unsigned __int16 handle);
void __cdecl DevGui_DrawMenu(unsigned __int16 menuHandle, unsigned __int16 activeChild, int *origin);
void __cdecl DevGui_DrawMenuVertically(const DevMenuItem *menu, unsigned __int16 activeChild, int *origin);
bool __cdecl DevGui_MenuItemDisabled(const DevMenuItem *menu);
int __cdecl DevGui_SubMenuTextWidth();
int __cdecl DevGui_MaxChildMenuWidth(const DevMenuItem *menu);
int __cdecl DevGui_MenuItemWidth(const DevMenuItem *menu);
void __cdecl DevGui_DrawMenuHorizontally(const DevMenuItem *menu, unsigned __int16 activeChild, int *origin);
void __cdecl DevGui_ChooseOrigin(int *origin);
void __cdecl DevGui_DrawSliders(const DevMenuItem *menu);
void __cdecl DevGui_DrawSliderPath(int x, int y);
int __cdecl DevGui_GetSliderPath(unsigned __int16 menuHandle, char *path, int pathLen);
void __cdecl DevGui_DrawDvarDescription(int x, int y, const dvar_s *dvar);
void __cdecl DevGui_DrawSingleSlider(
        int x,
        int y,
        int rowWidth,
        int rowHeight,
        float fraction,
        const unsigned __int8 *knobColor);
void __cdecl DevGui_DrawDvarValue(int x, int y, const dvar_s *dvar);
int __cdecl DevGui_DvarRowCount(const dvar_s *dvar);
void DevGui_DrawBindNextKey();
void __cdecl DevGui_DrawGraph(const DevMenuItem *menu, int localClientNum);
void __cdecl DevGui_Init();
const dvar_s *DevGui_RegisterDvars();
void __cdecl DevGui_Shutdown();
void DevGui_MenuShutdown();
void __cdecl DevGui_KeyPressed(int key);
void __cdecl DevGui_Update(int localClientNum, float deltaTime);
void DevGui_MoveSelectionHorizontally();
void DevGui_MoveLeft();
void DevGui_SelectPrevMenuItem();
void DevGui_SelectTopLevelChild();
void __cdecl DevGui_AdvanceChildNum(int numberToAdvance);
void DevGui_MoveRight();
void DevGui_SelectNextMenuItem();
void DevGui_MoveSelectionVertically();
void DevGui_MoveUp();
void DevGui_MoveDown();
void __cdecl DevGui_Accept(int localClientNum);
void DevGui_Reject();
int DevGui_UpdateSelection();
int DevGui_ScrollUp();
const dvar_s *__cdecl DevGui_SelectedDvar();
int DevGui_ScrollUpInternal();
int DevGui_ScrollDown();
int DevGui_ScrollDownInternal();
void __cdecl DevGui_UpdateDvar(float deltaTime, bool channelLink, bool dvarReset);
double __cdecl DevGui_PickFloatScrollStep(float min, float max);
void __cdecl DevGui_UpdateGraph(int localClientNum, float deltaTime);
void __cdecl DevGui_AddGraphKnot(DevGraph *graph, int localClientNum);
void __cdecl DevGui_RemoveGraphKnot(DevGraph *graph, int localClientNum);
void __cdecl DevGui_Toggle();
bool __cdecl DevGui_IsActive();
bool __cdecl DevGui_IsInitialized();
