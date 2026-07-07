#include "ui_gametype_custom_sp.h"

#include <qcommon/msg.h>
#include <string.h>
#include <universal/dvar.h>

// Decomp: CoDSP_rdBlackOps.map.c — SP menus use same expression surface as MP; custom classes disabled in campaign.

_CustomClassDescription g_customGameModeClassDescriptions[10];

const dvar_t *ui_items_no_cost;
const dvar_t *ui_useCustomClassInfo;
const dvar_t *custom_class_mode;
const dvar_t *custom_killstreak_mode;

void __cdecl UI_Gametype_Custom_Init()
{
    int i;

    ui_items_no_cost = _Dvar_RegisterInt("ui_items_no_cost", 0, 0, 1, 0x80u, "Set to 1 if you want items to have no cost");
    ui_useCustomClassInfo = _Dvar_RegisterBool("ui_useCustomClassInfo", 0, 0x80u, "Use custom class info from UI");
    custom_class_mode = _Dvar_RegisterInt("custom_class_mode", 0, 0, 1, 0, "Set to 1 if you want to use custom classes");
    custom_killstreak_mode = _Dvar_RegisterInt("custom_killstreak_mode", 0, 0, 1, 0, "Custom killstreak mode");
    for ( i = 0; i < 10; ++i )
        g_customGameModeClassDescriptions[i].name[0] = 0;
}

bool __cdecl UI_Gametype_IsUsingCustom()
{
    // Campaign front-end does not expose MP custom rules/classes.
    return false;
}

void __cdecl UI_Gametype_Custom_WriteClientData(msg_t *msg)
{
    MSG_WriteBit0(msg);
}

char __cdecl UI_Gametype_Custom_ReadClientData(msg_t *msg)
{
    // SP should only ever receive "no custom payload" data.
    // If a custom payload is present, report it so caller can reject.
    return MSG_ReadBit(msg) != 0;
}

void __cdecl UI_Gametype_CustomGameModeDataToDvars()
{
    Dvar_SetInt((dvar_s *)custom_class_mode, 0);
    Dvar_SetInt((dvar_s *)custom_killstreak_mode, 0);
    Dvar_SetBool((dvar_s *)ui_useCustomClassInfo, 0);
}

bool __cdecl UI_Gametype_CanUseCustom()
{
    // Custom MP classes/rules are intentionally disabled in SP.
    return false;
}

unsigned __int8 __cdecl GetCustomClassLoadoutItemForSlot(
    int controllerIndex,
    unsigned int slotNum,
    const char *itemName)
{
    (void)controllerIndex;
    (void)slotNum;
    (void)itemName;
    return 0;
}

unsigned __int8 __cdecl GetCustomClassLoadoutItemForClassName(
    int controllerIndex,
    const char *customClassName,
    const char *loadoutName)
{
    (void)controllerIndex;
    (void)customClassName;
    (void)loadoutName;
    return 0;
}

int __cdecl GetCustomClassModifierForClass(int controllerIndex, unsigned int classNum, const char *itemName)
{
    (void)controllerIndex;
    (void)classNum;
    (void)itemName;
    return 0;
}

void __cdecl UI_Gametype_EquipCustomClassLoadoutItem(
    int controllerIndex,
    const char *customClassName,
    unsigned __int8 itemIndex,
    const char *loadoutName)
{
    (void)controllerIndex;
    (void)customClassName;
    (void)itemIndex;
    (void)loadoutName;
}
