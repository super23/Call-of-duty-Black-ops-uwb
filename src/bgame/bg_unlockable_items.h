#pragma once

const char *__cdecl BG_UnlockablesGetCustomClassNameForInt(customClass_t customClassNum);
const char *__cdecl BG_UnlockablesGetCustomClassName(customClass_t customClassNum);
int __cdecl BG_UnlockablesGetFaceCamoItemIndex(int faceCamoNum);
int __cdecl BG_UnlockablesGetItemGroupFromName(const char *groupName);
int __cdecl FindEntryInList(const char *stringEntry, const char **textArray, int numEntries);
const char *__cdecl BG_UnlockablesGetItemGroupName(itemGroup_t groupIndex);
const char *__cdecl BG_UnlockablesGetLoadoutSlotName(loadoutSlot_t loadoutSlot);
bool __cdecl BG_UnlockablesIsItemInDefaultClass(int itemIndex);
itemInfo_t *__cdecl BG_UnlockablesGetItemInfo(int itemIndex);
char __cdecl BG_UnlockablesAllItemsUnlocked();
unsigned int __cdecl BG_UnlockablesGetEquippedClanTagFeatureIndex(int controllerIndex);
int __cdecl BG_UnlockablesGetClanTagFeatureCount();
const char *__cdecl BG_UnlockablesGetClanTagFeature(unsigned int index, clanTagFeatureColumn_t column);
int __cdecl BG_UnlockablesGetClanTagFeaturePLvlUnlock(unsigned int index);
int __cdecl BG_UnlockablesGetClanTagFeatureLvlUnlock(unsigned int index);
bool __cdecl BG_UnlockablesIsClanTagFeatureLockedForRank(int PLevel, int rank, unsigned int index);
bool __cdecl BG_UnlockablesIsClanTagFeatureLocked(int controllerIndex, unsigned int index);
bool __cdecl BG_UnlockablesIsClanTagFeaturePurchased(int controllerIndex, unsigned int index);
char __cdecl BG_UnlockablesAllItemsFree();
void __cdecl BG_UnlockablesSetClanTagFeatureNew(int controllerIndex, unsigned int index);
void __cdecl BG_UnlockablesPurchaseClanTagFeature(int controllerIndex, unsigned int index);
int __cdecl BG_UnlockablesGetProfessionalIndex();
void __cdecl BG_InitUnlockables();
void BG_UnlockablesBuildDefaultClasses();
const char *__cdecl BG_UnlockablesGetLoadoutName(loadoutSlot_t loadoutSlot);
int __cdecl BG_UnlockablesCompareItemsBySortKey(const void *arg0, const void *arg1);
char __cdecl BG_UnlockablesBuildItemInfo(int itemIndex, itemInfo_t *itemInfo);
int __cdecl BG_UnlockablesParseDefaultClassesForItem(itemInfo_t *itemInfo, const char *defaultClassList);
void __cdecl BG_UnlockablesParseAttachmentsForItem(itemInfo_t *itemInfo, const char *attachmentList);
void __cdecl BG_UnlockablesParseDualWieldInfo(itemInfo_t *itemInfo);
void __cdecl BG_UnlockablesParseFaceCamoInfo(itemInfo_t *itemInfo);
void __cdecl BG_UnlockablesParsePerkProChallenges(itemInfo_t *itemInfo);
void BG_UnlockablesBuildActualDefaultItems();
void BG_UnlockablesBuildDefaultItems();
void BG_UnlockablesBuildClanTagFeatureRows();
int BG_UnlockablesSetProfessionalIndex();
int __cdecl BG_UnlockablesGetItemAttachmentNumWithAttachPoint(int itemIndex, int attachmentNum, int attachmentPoint);
eAttachment __cdecl BG_UnlockablesGetItemAttachment(int itemIndex, int attachmentNum);
int __cdecl BG_UnlockablesGetItemAttachmentIndexForAttachmentPoint(
        int itemIndex,
        eAttachment attachment,
        eAttachmentPoint point);
const char *__cdecl BG_UnlockablesGetItemName(int itemIndex);
const char *__cdecl BG_UnlockablesGetItemRef(int itemIndex);
int __cdecl BG_UnlockablesGetItemCount(int itemIndex);
const char *__cdecl BG_UnlockablesGetItemImage(int itemIndex);
int __cdecl BG_UnlockablesGetItemUnlockLevel(int itemIndex);
int __cdecl BG_UnlockablesGetItemUnlockPLevel(int itemIndex);
int __cdecl BG_UnlockablesGetItemCost(int itemIndex);
int __cdecl BG_UnlockablesGetItemDualWieldCost(int itemIndex);
int __cdecl BG_UnlockablesGetItemDualWieldIndex(int itemIndex);
int __cdecl BG_UnlockablesGetItemDualWieldBaseIndex(int itemIndex);
int __cdecl BG_UnlockablesGetItemSellPrice(int itemIndex);
const char *__cdecl BG_UnlockablesGetItemDesc(int itemIndex);
int __cdecl BG_UnlockablesGetNumItemAttachmentsWithAttachPoint(int itemIndex, int attachPoint);
int __cdecl BG_UnlockablesGetNumItemAttachments(int itemIndex);
int __cdecl BG_UnlockablesGetNumItemAttachmentsAtPoint(int itemIndex, eAttachmentPoint point);
eAttachment __cdecl BG_UnlockablesGetItemAttachmentAtPointByIndex(
        int itemIndex,
        eAttachmentPoint point,
        int attachmentIndex);
const char *__cdecl BG_UnlockablesGetItemGroup(int itemIndex);
int __cdecl BG_UnlockablesGetItemGroupEnum(int itemIndex);
int __cdecl BG_UnlockablesGetLoadoutSlotFromString(const char *slotName);
int __cdecl BG_UnlockablesGetItemLoadoutSlot(int itemIndex);
bool __cdecl BG_UnlockablesIsItemLockedForRank(int pLevel, int rank, int itemIndex);
bool __cdecl BG_UnlockablesIsItemLocked(int controllerIndex, int itemIndex);
bool __cdecl BG_UnlockablesIsItemDualWieldLocked(int controllerIndex, int itemIndex);
bool __cdecl BG_UnlockablesIsItemPassive(int itemIndex);
bool __cdecl BG_UnlockablesItemHasDualWield(int itemIndex);
bool __cdecl BG_UnlockablesItemIsDualWield(int itemIndex);
bool __cdecl BG_UnlockablesIsItemValidNotNull(int itemIndex);
void __cdecl BG_UnlockablesSetItemNew(int controllerIndex, int itemIndex);
void __cdecl SV_SetItemNewForClient(char *clientBuffer, int itemIndex, int clientRank, int clientPLevel);
bool __cdecl BG_UnlockablesIsItemNew(int controllerIndex, int itemIndex);
char __cdecl BG_UnlockablesIsItemGroupNew(int controllerIndex, const char *groupName);
void __cdecl BG_UnlockablesPurchaseItem(int controllerIndex, int itemIndex, int cost);
char __cdecl BG_UnlockablesSetItemPurchased(int controllerIndex, int itemIndex, bool isPurchased);
void __cdecl BG_UnlockablesSellItem(int controllerIndex, int itemIndex);
void __cdecl BG_UnlockablesSetPurchasedBits(unsigned __int8 *statsBuffer, unsigned __int8 *purchasedItems);
bool __cdecl BG_UnlockablesIsItemPurchasedFromBuffer(unsigned __int8 *statsBuffer, int itemIndex);
bool __cdecl BG_UnlockablesIsItemPurchased(int controllerIndex, int itemIndex);
bool __cdecl BG_UnlockablesIsItemDualWieldPurchased(int controllerIndex, int itemIndex);
const char *__cdecl BG_UnlockablesGetItemAttachmentDisplayName(int itemIndex, int attachmentNum);
const char *__cdecl BG_UnlockablesGetItemAttachmentDesc(int itemIndex, int attachmentNum);
int __cdecl BG_UnlockablesGetItemAttachmentCost(int itemIndex, int attachmentNum);
const char *__cdecl BG_UnlockablesGetItemAttachmentRef(int itemIndex, int attachmentNum);
bool __cdecl BG_UnlockablesGetAttachmentPurchased(int controllerIndex, const char *attachmentRef);
void __cdecl BG_UnlockablesPurchaseItemAttachmentPoint(int controllerIndex, int itemIndex, const char *attachmentPoint);
int __cdecl BG_UnlockablesIsItemAttachmentValid(itemInfo_t *itemInfo, const char *attachmentString);
void __cdecl BG_UnlockablesPurchaseItemAttachment(int controllerIndex, int itemIndex, eAttachment attachmentNum);
char __cdecl BG_UnlockablesSetItemAttachmentPurchased(
        int controllerIndex,
        int itemIndex,
        int attachmentNum,
        bool isPurchased);
char __cdecl BG_UnlockablesSetAttachmentPurchased(int controllerIndex, const char *attachmentRef, bool isPurchased);
bool __cdecl BG_UnlockablesIsItemAttachmentPointPurchased(
        int controllerIndex,
        int itemIndex,
        const char *attachmentPoint);
bool __cdecl BG_UnlockablesIsItemAttachmentPurchased(int controllerIndex, int itemIndex, int attachmentNum);
int __cdecl BG_UnlockablesGetWeaponOptionFromName(const char *optionName);
bool __cdecl BG_UnlockablesIsItemOptionPurchased(int controllerIndex, int itemIndex, int optionIndex);
int __cdecl BG_UnlockablesGetItemIndexFromName(const char *itemName);
int __cdecl BG_UnlockablesGetItemIndexFromRef(const char *itemName);
int __cdecl BG_UnlockablesGetItemIndexforWeapon(unsigned int weaponIndex);
void __cdecl BG_UnlockablesClearItemNew(int controllerIndex, int itemIndex);
void __cdecl BG_UnlockablesClearAllItemsNew(int controllerIndex);
int __cdecl BG_UnlockablesGetAssociatedBody(int itemIndex);
int __cdecl BG_UnlockablesGetAssociatedBodyWithPrimary(int itemIndex);
int __cdecl BG_UnlockablesGetAssociatedBodyWithPerk(int itemIndex);
void __cdecl BG_UnlockablesEquipClassToSlot(
        int controllerIndex,
        const char *customClassName,
        int itemIndex,
        const char *loadoutName);
void __cdecl BG_UnlockablesEquipClassSpecialPropertiesToSlot(
        int controllerIndex,
        const char *customClassName,
        int itemIndex,
        const char *loadoutName);
void __cdecl BG_UnlockablesEquipAssociatedBody(int controllerIndex, const char *customClassName, int itemIndex);
unsigned int __cdecl BG_UnlockablesGetEquippedItemInSlot(
        int controllerIndex,
        const char *customClassName,
        const char *slotName);
bool __cdecl BG_UnlockablesIsItemEquipped(int controllerIndex, const char *customClassName, int itemIndex);
bool __cdecl BG_UnlockablesIsItemEquippedInAnyCustomClass(int controllerIndex, int itemIndex);
int __cdecl BG_UnlockablesGetActualDefaultItem(unsigned int loadoutSlot);
int __cdecl BG_UnlockablesGetDefaultItem(unsigned int loadoutSlot);
int __cdecl BG_UnlockablesGetDefaultWeaponItem();
int __cdecl BG_UnlockablesGetDefaultClassSlotIndexFromName(const char *className, const char *slotName);
const char *__cdecl BG_UnlockablesGetDefaultClassSlotFromName(const char *className, const char *slotName);
bool __cdecl BG_UnlockablesIsProItemUnlocked(int controllerIndex, int itemIndex);
bool __cdecl BG_UnlockablesIsProItem(int itemIndex);
int __cdecl BG_UnlockablesGetProItem(int itemIndex);
int __cdecl BG_UnlockablesBuildItemListForGroup(itemGroup_t group);
int __cdecl BG_UnlockablesBuildItemListForGroupForWeaponTable(itemGroup_t group);
int __cdecl BG_UnlockablesGetGroupItemCount();
bool __cdecl BG_UnlockablesIsItemClassified(int itemIndex);
bool __cdecl BG_UnlockablesIsItemClassifiedGeneric(int controllerIndex, int itemIndex);
int __cdecl BG_UnlockablesGetNumPurchasesBeforeDeclassified(int itemIndex);
int __cdecl BG_UnlockablesGetItemIndexInSlot(int itemNum);
void __cdecl BG_UnlockablesClearWeaponInfo(int controllerIndex, int itemIndex);
char __cdecl BG_UnlockablesSetItemOptionPurchased(
        int controllerIndex,
        int itemIndex,
        int optionIndex,
        bool isPurchased);
char __cdecl BG_UnlockablesSetProItemUnlocked(int controllerIndex, int itemIndex, bool isUnlocked);
bool __cdecl BG_UnlockablesIsWeaponOptionLocked(int controllerIndex, int weaponOptionIndex);
bool __cdecl SV_CacValidate_IsItemPurchased(unsigned __int8 *cacBuffer, int itemIndex, int pLevel, int rank);
bool __cdecl SV_CACValidate_IsItemAttachmentPurchased(
        unsigned __int8 *cacbuffer,
        int itemIndex,
        int attachmentNum,
        int pLevel,
        int rank);
bool __cdecl SV_CACValidate_IsItemOptionPurchased(
        unsigned __int8 *cacbuffer,
        int itemIndex,
        int optionIndex,
        int pLevel,
        int rank);
bool __cdecl SV_CACValidate_IsClanTagFeaturePurchased(
        unsigned __int8 *cacbuffer,
        unsigned int index,
        int plevel,
        int rank);
bool __cdecl SV_CACValidate_IsClanTagFeatureLocked(int rank, int plevel, unsigned int index);
void __cdecl SV_CACValidate_ClearWeaponInfo(char *buffer, int itemIndex);
char __cdecl SV_CACValidate_SetProItemUnlocked(char *buffer, int itemIndex, bool isUnlocked);
char __cdecl SV_CACValidate_SetItemPurchased(char *buffer, int itemIndex, bool isPurchased);
char __cdecl SV_CACValidate_SetItemAttachmentPurchased(
        char *buffer,
        int itemIndex,
        int attachmentNum,
        bool isPurchased);
char __cdecl SV_CACValidate_SetItemOptionPurchased(char *buffer, int itemIndex, int optionIndex, bool isPurchased);
bool __cdecl SV_CACValidate_IsItemDualWieldPurchased(unsigned __int8 *cacbuffer, int itemIndex, int pLevel, int rank);
char __cdecl SV_CACValidate_CheckIfEquippedItemsArePurchased(unsigned __int8 *cacBuffer, int pLevel, int rank);
void __cdecl SV_SetClanTagFeatureNewForClient(char *buffer, int clientRank, int clientPrestige, unsigned int index);
void __cdecl BG_UnlockableItemsInit();
void __cdecl BG_UnlockablesPurchaseClanTagFeatureCmd();
void __cdecl BG_UnlockablesPurchaseCurrentItemAttachmentCmd();
void __cdecl BG_UnlockablesPurchaseItemAttachmentPointCmd();
int __cdecl Cmd_Argc();
void __cdecl BG_UnlockablesPurchaseCurrentItemCmd();
void __cdecl BG_UnlockablesPurchaseItemCmd();
void __cdecl BG_UnlockablesSellCurrentItemCmd();
void __cdecl BG_UnlockablesPurchaseWeaponOptionCmd();
void __cdecl BG_UnlockablesPurchaseItemOption(int controllerIndex, int itemIndex, int optionIndex);
void __cdecl BG_UnlockablesPurchaseCurrentItemOptionCmd();
void __cdecl BG_UnlockablesEquipClassCmd();
void __cdecl BG_UnlockablesEquipClass(int controllerIndex, const char *customClassName, int itemIndex);
void __cdecl BG_UnlockablesEquipDefaultItemToSlotCmd();
void __cdecl BG_UnlockablesEquipDefaultGlobalItemToSlotCmd();
void __cdecl BG_UnlockablesEquipClassAttachmentCmd();
void __cdecl BG_UnlockablesEquipClassAttachment(
        int controllerIndex,
        const char *customClassName,
        const char *itemName,
        const char *attachmentName,
        const char *attachmentPoint);
void __cdecl BG_UnlockablesEquipClassCurrentAttachmentCmd();
void __cdecl BG_UnlockablesEquipClassCurrentOptionCmd();
void __cdecl BG_UnlockablesToggleWeaponOptionCmd();
void __cdecl BG_UnlockablesEquipClassCurrentItemCmd();
void __cdecl BG_UnlockablesEquipClassCurrentItemCmdInternal(bool isGlobal);
int __cdecl BG_UnlockablesGetFaceCamoNumFromItemIndex(int itemIndex);
void __cdecl BG_UnlockablesEquipClassCurrentGlobalItemCmd();
void __cdecl BG_UnlockablesEquipClassItemCmd();
void __cdecl BG_UnlockablesEquipDefaultClassCmd();
void __cdecl BG_UnlockablesEquipDefaultClass(
        int controllerIndex,
        const char *customClassName,
        defaultClass_t defaultClass);
void __cdecl BG_UnlockablesEquipDefaultGlobalItemsCmd();
void __cdecl BG_UnlockablesClearItemNewCmd();
void __cdecl BG_UnlockablesClearAllItemsNewCmd();
void __cdecl BG_ReplaceItemsWithPurchasedProItem(int controllerIndex);
int __cdecl BG_UnlockablesGetProItemIfPurchased(int controllerIndex, int itemIndex);
void __cdecl BG_UnlockablesPurchaseCurrentProItemCmd();
void __cdecl BG_UnlockablesBuildItemListForSlotNameCmd();
int __cdecl BG_UnlockablesBuildItemListForSlotName(int controllerIndex, const char *slotName);
int __cdecl BG_UnlockablesGetNumPurchasedItemsInCurrentList(int controllerIndex);
void __cdecl BG_UnlockablesBuildItemListForChallengesPerksCmd();
int __cdecl BG_UnlockablesBuildItemListForChallengesPerks(int controllerIndex, const char *slotName);
void __cdecl BG_UnlockablesBuildItemListForGroupCmd();
void __cdecl BG_UnlockablesBuildItemListForGroupForWeaponTableCmd();
void __cdecl BG_UnlockablesBuildItemListForGroupNameCmd();
int __cdecl BG_UnlockablesBuildItemListForGroupName(int controllerIndex, const char *groupName);
void __cdecl BG_UnlockablesBuildItemListForSlotNameAndGroupCmd();
int __cdecl BG_UnlockablesBuildItemListForSlotNameAndGroup(
        int controllerIndex,
        const char *slotName,
        const char *groupName);
void __cdecl OpenAttachmentMenuForCurrentItemCmd();
void __cdecl BG_UnlockablesEquipItemInSlotSortedCmd();
void __cdecl BG_UnlockablesEquipItemInSlotSortedCmdInternal(bool isGlobalItem);
void __cdecl BG_UnlockablesEquipGlobalItemInSlotSortedCmd();
void __cdecl BG_ClearAllAttachmentsForItemCmd();
void __cdecl BG_ClearAttachmentForItemAtPointCmd();
void __cdecl BG_UnlockablesSetCurrentItemIndexCmd();
void __cdecl BG_UnlockablesCopyClassCmd();
void __cdecl BG_UnlockablesSetClanTagCmd();
int __cdecl SV_GetMaxAttachCount();
