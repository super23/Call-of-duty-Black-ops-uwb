#pragma once

void __cdecl LiveCombatRecord_OpenFriendsCombatRecord(int localClientNum);
double __cdecl LiveCombatRecord_GetSortedItemData(int index, bool forFriend, sortedItemParams_t param);
int __cdecl LiveCombatRecord_GetSortedItemPositionByItemIndex(int itemIndex, bool forFriend);
const char *__cdecl LiveCombatRecord_GetEquipmentUsageStringSuffix(const char *equipmentName);
const char *__cdecl LiveCombatRecord_GetPersonalBestStatNameByIndex(int index);
const char *__cdecl LiveCombatRecord_GetGameTypeName(unsigned int index);
int __cdecl LiveCombatRecord_GetGameTypeLbIndex(unsigned int index);
int __cdecl LiveCombatRecord_GetGameTypeCount();
const char *__cdecl LiveCombatRecord_GetHitLocationImageRef(hitLocation_t hitLocation);
void __cdecl LiveCombatRecord_Init();
void __cdecl LiveCombatRecord_CombatRecordViewUpdateCmd();
void __cdecl LiveCombatRecord_BuildSortedItemListByStats(
        int controllerIndex,
        itemSortingCriteria_t criterion,
        playerStatsKeyIndex_t param,
        playerStatsKeyIndex_t divisionParam);
int __cdecl LiveCombatRecord_CompareItemsByStats(float *arg0, float *arg1);
int __cdecl LiveCombatRecord_CompareMatchCount(float *arg0, float *arg1);
void __cdecl LiveCombatRecord_BuildSortedItem(
        int controllerIndex,
        itemSortingCriteria_t criterion,
        bool forOtherPlayer,
        int index,
        int itemNumber,
        playerStatsKeyIndex_t param,
        playerStatsKeyIndex_t divisionParam);
bool __cdecl LiveCombatRecord_FilterCurrentSortedItem(
        int controllerIndex,
        itemSortingCriteria_t criterion,
        int itemNumber,
        sortedStatLoadoutSlots_t currLoadout);
