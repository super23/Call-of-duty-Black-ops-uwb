#include "ui_feeders_sp.h"
#include <ui/ui_utils.h>
#include <ui/ui_main_pc.h>
#include <ui_sp/ui_main_sp.h>
#include <universal/q_shared.h>

int __cdecl UI_Project_FeederCount(int localClientNum, int contextIndex, float feederID, listBoxDef_s *listPtr)
{
    (void)localClientNum;
    (void)contextIndex;
    (void)listPtr;
    if ( (int)feederID == 1 )
        return sharedUiInfo.mapCount;
    return 0;
}

char *__cdecl UI_GetMapLoadNameForCurrentIndex(int index)
{
    if ( index < 0 || index >= sharedUiInfo.mapCount )
        return (char *)"";
    return sharedUiInfo.mapList[index].mapLoadName;
}

char *__cdecl UI_FeederItemText_Maps(int controllerIndex, itemDef_s *item, int index, int column, Material **handle)
{
    (void)controllerIndex;
    (void)item;
    if ( index < 0 || index >= sharedUiInfo.mapCount )
        return (char *)"";
    if ( column == 1 )
        return UI_SafeTranslateString(sharedUiInfo.mapList[index].mapName);
    if ( column == 0 )
    {
        *handle = sharedUiInfo.assets.whiteMaterial;
        return (char *)"";
    }
    return (char *)"";
}

void __cdecl UI_FeederSelection_Maps(int contextIndex, float feederID, int index)
{
    char *mapLoadName;

    (void)contextIndex;
    (void)feederID;
    mapLoadName = UI_GetMapLoadNameForCurrentIndex(index);
    if ( index >= 0 && index < sharedUiInfo.mapCount )
        Dvar_SetStringByName("ui_preview", mapLoadName);
}

void __cdecl UI_OverrideCursorPos_Maps(int contextIndex, listBoxDef_s *listPtr)
{
    const char *preview;
    char *mapLoadName;

    if ( listPtr->cursorPos[contextIndex] >= sharedUiInfo.mapCount )
        listPtr->cursorPos[contextIndex] = 0;
    if ( !sharedUiInfo.mapCount )
    {
        listPtr->cursorPos[contextIndex] = 0;
        Dvar_SetStringByName("ui_preview", (char *)"");
        return;
    }
    if ( Dvar_GetBool("ui_mapPackChanged") )
    {
        listPtr->cursorPos[contextIndex] = 0;
        Dvar_SetBoolByName("ui_mapPackChanged", 0);
        return;
    }
    mapLoadName = UI_GetMapLoadNameForCurrentIndex(listPtr->cursorPos[contextIndex]);
    preview = Dvar_GetString("ui_preview");
    if ( I_strcmp(preview, mapLoadName) )
        Dvar_SetStringByName("ui_preview", mapLoadName);
}

int __cdecl UI_FeederDoubleClick(int localClientNum, int contextIndex, float feederID, itemGroup_t index)
{
    (void)localClientNum;
    (void)contextIndex;
    (void)feederID;
    (void)index;
    return 0;
}

void __cdecl UI_FeederDoubleClickDlcMaps(int localClientNum, int contextIndex, int index)
{
    (void)localClientNum;
    (void)contextIndex;
    (void)index;
}

bool __cdecl UI_FeederItemEnabled(int localClientNum, int contextIndex, float feederID, int index)
{
    (void)localClientNum;
    (void)contextIndex;
    (void)feederID;
    (void)index;
    return true;
}

Material *__cdecl UI_FeederItemImage(float feederID, int index)
{
    int actual;

    if ( feederID != 4.0f )
        return 0;
    UI_SelectedMap(index, &actual);
    return UI_GetLevelShot(actual);
}

void __cdecl UI_FeederBackColor(
    int localClientNum,
    int contextIndex,
    itemDef_s *item,
    float feederID,
    int index,
    float *color)
{
    (void)localClientNum;
    (void)contextIndex;
    (void)feederID;
    (void)index;
    *color = item->window.backColor[0];
    color[1] = item->window.backColor[1];
    color[2] = item->window.backColor[2];
    color[3] = item->window.backColor[3];
}
