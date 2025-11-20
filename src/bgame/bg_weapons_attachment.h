#pragma once

const char **__cdecl BG_GetAttachmentNames();
const char *__cdecl BG_GetAttachmentName(eAttachment index);
int __cdecl BG_GetAttachmentIndex(const char *name);
const char *__cdecl BG_GetAttachmentPointName(eAttachmentPoint index);
int __cdecl BG_GetAttachmentPointIndex(const char *name);
int __cdecl BG_GetWeaponOptionGroup(const char *name);
const char *__cdecl BG_GetAttachmentDisplayName(eAttachment attachment);
int __cdecl BG_GetAttachmentArrayIndex(eAttachment attachment);
const char *__cdecl BG_GetAttachmentReference(eAttachment attachment);
const char *__cdecl BG_GetAttachmentImage(eAttachment attachment);
const char *__cdecl BG_GetAttachmentDesc(eAttachment attachment);
int __cdecl BG_GetAttachmentCost(eAttachment attachment);
int __cdecl BG_GetAttachmentPointCost(eAttachmentPoint attachmentPoint);
eAttachmentPoint __cdecl BG_GetAttachmentPointIndexFromAttachment(eAttachment attachment);
bool __cdecl BG_AreAttachmentsCompatible(eAttachment attachment1, eAttachment attachment2);
const AttachmentTableEntry *__cdecl BG_GetAttachmentTableEntry(unsigned int index);
const AttachmentPointTableEntry *__cdecl BG_GetAttachmentPointTableEntry(unsigned int index);
int __cdecl BG_GetWeaponAttachmentCount();
int __cdecl BG_GetWeaponOptionCountForGroup(eWeaponOptionGroup optionGroup);
int __cdecl BG_GetWeaponOptionCount();
const char *__cdecl BG_GetWeaponOptionDisplayName(int weaponOption);
int __cdecl BG_GetWeaponOptionArrayIndex(int weaponOption);
const char *__cdecl BG_GetWeaponOptionReference(int weaponOption);
const char *__cdecl BG_GetWeaponOptionImage(int weaponOption);
int __cdecl BG_GetWeaponOptionCost(int weaponOption);
eWeaponOptionGroup __cdecl BG_GetWeaponOptionGroup(int weaponOption);
int __cdecl BG_GetWeaponOptionUnlockLvl(int weaponOption);
int __cdecl BG_GetWeaponOptionUnlockPLevel(int weaponOption);
int __cdecl BG_GetWeaponOptionNumFromIndexAndGroup(int index, eWeaponOptionGroup weaponOptionGroup);
int __cdecl BG_GetWeaponOptionFromName(const char *optionName);
void __cdecl BG_LoadWeaponAttachmentTable();
void __cdecl BG_LoadAttachmentRow(const StringTable *attachmentTable, int row, AttachmentTableEntry *entry, int *count);
void __cdecl BG_LoadWeaponAttachmentTableRow(
        const StringTable *attachmentTable,
        int row,
        WeaponAttachmentTableEntry *entry);
void __cdecl BG_LoadAttachmentPointRow(
        const StringTable *attachmentTable,
        int row,
        AttachmentPointTableEntry *entry,
        int *count);
void __cdecl BG_LoadWeaponOptions(const StringTable *attachmentTable);
void __cdecl BG_LoadWeaponOptionRow(
        const StringTable *attachmentTable,
        int row,
        WeaponOptionTableEntry *entry,
        int *count);
