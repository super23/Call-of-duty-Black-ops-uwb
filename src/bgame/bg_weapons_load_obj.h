#pragma once

char *__cdecl BG_GetPlayerAnimTypeName(int index);
int __cdecl BG_GetPlayerAnimTypeIndex(int typeEnum);
const char *__cdecl BG_GetWeaponTypeName(weapType_t type);
const char *__cdecl BG_GetWeaponClassName(weapClass_t type);
const char *__cdecl BG_GetWeaponInventoryTypeName(weapInventoryType_t type);
void __cdecl BG_LoadWeaponStrings();
void __cdecl BG_LoadPlayerAnimTypes();
void __cdecl BG_ClearWeaponDefInternal();
void __cdecl SetConfigString(char **ppszConfigString, const char *pszKeyValue);
flameTable *__cdecl BG_LoadFlameTableInternal(const char *folder, char *name);
void __cdecl SetConfigString2(unsigned __int8 *pMember, const char *pszKeyValue);
void __cdecl InitFlameTable(flameTable *fTable);
flameTable *__cdecl BG_GetFlameTable(const char *folder, char *name);
char __cdecl BG_LoadWeaponFile(char *szFileName, char *szBuffer, int iBufferSize);
void __cdecl BG_LoadWeaponMergeSupport();
char __cdecl BG_SplitWeaponDefNames(
        const char *name,
        char **sources,
        WeaponComponentList *componentAll,
        char *outputName);
int __cdecl BG_MergeWeaponDefClipName(char **value, char *mergedValue, int size);
int __cdecl BG_MergeWeaponDefFloatValue(char **value, char *mergedValue);
int __cdecl BG_MergeWeaponDefHidetags(char **value, char *mergedValue, int size);
bool __cdecl BG_IsAnimationField(const char *fieldName);
bool __cdecl BG_IsADSField(const char *fieldName);
char __cdecl BG_IsGripAnimationName(const char *name);
int __cdecl BG_MergeWeaponDefAnimations(const char *fieldName, char **value, char *mergedValue, unsigned int size);
int __cdecl BG_MergeWeaponDefSpecialCases(const char *fieldName, char **value, char *mergedValue, int size);
char __cdecl BG_LoadWeaponVariantDefFile(WeaponFullDef *weapFullDef, const char *folder, char *name);
int __cdecl BG_ParseWeaponDefSpecificFieldType(WeaponFullDef *pStruct, const char *pValue, int iFieldType);
int __cdecl Weapon_GetStringArrayIndex(const char *value, const char **stringArray, int arraySize);
char **__cdecl BG_RegisterSurfaceTypeSounds(const char *surfaceSoundBase);
WeaponVariantDef *__cdecl BG_LoadWeaponVariantDefInternal(const char *folder, char *name);
void __cdecl BG_SetupTransitionTimes(WeaponVariantDef *weapVariantDef);
void __cdecl BG_CheckWeaponDamageRanges(WeaponDef *weapDef);
void __cdecl BG_CheckProjectileValues(WeaponFullDef *weaponFullDef);
void __cdecl InitWeaponDef(WeaponFullDef *weapFullDef);
char __cdecl BG_WeaponUpdateField(const char *weaponName, char *keyValue);
bool __cdecl BG_FlameTableUpdateField(const char *flameTableName, char *keyValue);
WeaponVariantDef *__cdecl BG_LoadDefaultWeaponVariantDef();
WeaponFullDef *__cdecl BG_LoadDefaultWeaponVariantDef_LoadObj();
WeaponVariantDef *__cdecl BG_LoadDefaultWeaponVariantDef_FastFile();
