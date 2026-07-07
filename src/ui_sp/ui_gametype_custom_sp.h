#pragma once

#include <universal/dvar.h>

struct msg_t;

struct _CustomClassDescription
{
    char name[16];
};

extern _CustomClassDescription g_customGameModeClassDescriptions[10];

void __cdecl UI_Gametype_Custom_Init();
void __cdecl UI_Gametype_Custom_WriteClientData(msg_t *msg);
char __cdecl UI_Gametype_Custom_ReadClientData(msg_t *msg);
void __cdecl UI_Gametype_CustomGameModeDataToDvars();
bool __cdecl UI_Gametype_CanUseCustom();
bool __cdecl UI_Gametype_IsUsingCustom();

unsigned __int8 __cdecl GetCustomClassLoadoutItemForSlot(
    int controllerIndex,
    unsigned int slotNum,
    const char *itemName);
unsigned __int8 __cdecl GetCustomClassLoadoutItemForClassName(
    int controllerIndex,
    const char *customClassName,
    const char *loadoutName);
int __cdecl GetCustomClassModifierForClass(int controllerIndex, unsigned int classNum, const char *itemName);
void __cdecl UI_Gametype_EquipCustomClassLoadoutItem(
    int controllerIndex,
    const char *customClassName,
    unsigned __int8 itemIndex,
    const char *loadoutName);

extern const dvar_t *ui_items_no_cost;
extern const dvar_t *ui_useCustomClassInfo;
extern const dvar_t *custom_class_mode;
extern const dvar_t *custom_killstreak_mode;
