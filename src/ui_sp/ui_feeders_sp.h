#pragma once

#include <ui/ui_shared.h>

char *__cdecl UI_FeederItemText_CustomGametypes(int index);
struct _CustomClassDescription *__cdecl UI_Project_FeederItemText(
    int localClientNum,
    int contextIndex,
    itemDef_s *item,
    float feederID,
    itemGroup_t index,
    int column,
    Material **handle,
    float *angle,
    bool *useOwnerDraw);
void __cdecl UI_Project_FeederItemOwnerDraw(
    int localClientNum,
    int contextIndex,
    float feederID,
    const rectDef_s *rect,
    unsigned int index);
char __cdecl UI_Project_FeederItemColor(
    int controllerIndex,
    int contextIndex,
    itemDef_s *item,
    float feederID,
    int index,
    int column,
    float *color,
    bool onFocus);
void __cdecl UI_OverrideCursorPos_CustomGametypes(int contextIndex, listBoxDef_s *listPtr);
bool __cdecl UI_Project_OverrideCursorPos(int localClientNum, int contextIndex, itemDef_s *item);
void __cdecl UI_FeederSelection_CustomGametypes(int contextIndex, float feederID, int index);
void __cdecl UI_Project_FeederSelection(int localClientNum, int contextIndex, float feederID, int index);
char __cdecl UI_Project_FeederDataExtended(
    int controllerIndex,
    int contextIndex,
    int feederID,
    listBoxDef_s *listBox,
    const char *field,
    char **stringResult);

int __cdecl UI_Project_FeederCount(int localClientNum, int contextIndex, float feederID, listBoxDef_s *listPtr);
char *__cdecl UI_FeederItemText_Maps(int controllerIndex, itemDef_s *item, int index, int column, Material **handle);
void __cdecl UI_FeederSelection_Maps(int contextIndex, float feederID, int index);
void __cdecl UI_OverrideCursorPos_Maps(int contextIndex, listBoxDef_s *listPtr);
char *__cdecl UI_GetMapLoadNameForCurrentIndex(int index);
int __cdecl UI_FeederDoubleClick(int localClientNum, int contextIndex, float feederID, itemGroup_t index);
void __cdecl UI_FeederDoubleClickDlcMaps(int localClientNum, int contextIndex, int index);
bool __cdecl UI_FeederItemEnabled(int localClientNum, int contextIndex, float feederID, int index);
Material *__cdecl UI_FeederItemImage(float feederID, int index);
void __cdecl UI_FeederBackColor(
    int localClientNum,
    int contextIndex,
    itemDef_s *item,
    float feederID,
    int index,
    float *color);
