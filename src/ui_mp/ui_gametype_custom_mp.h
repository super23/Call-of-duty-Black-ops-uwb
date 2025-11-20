#pragma once

void __cdecl EnableMinimumCustomClasses();
void __cdecl UI_Gametype_ShowErrorPopup(char *title, char *message);
void __cdecl UI_Gametype_WriteDvarChunk(MemoryFile *memFile, char *dvarName);
void __cdecl MemFile_WriteFloat(MemoryFile *memFile, float valuef);
char __cdecl UI_Gametype_ReadDvarChunk(MemoryFile *memFile);
void __cdecl MemFile_ReadVec3(MemoryFile *memFile, float *value);
const dvar_s *__cdecl RegisterDvarIfUndefined(const char *name, int defaultValue);
void __cdecl RegisterCustomClassDvarsIfNeeded();
void __cdecl GetClassLoadoutForClassName(const char *className, unsigned int customClassSlotNum);
void __cdecl UI_Gametype_BuildDefaultCustomClasses();
void __cdecl UI_Gametype_CustomClassesReset();
char __cdecl UI_Gametype_ReadCustomClassChunk(MemoryFile *memFile);
char __cdecl UI_Gametype_ReadCustomClassDescriptionChunk(MemoryFile *memFile);
bool __cdecl UI_Gametype_HasDvarChanged(const dvar_s *dvar);
bool __cdecl UI_Gametype_HasDvarChanged(const char *dvarName);
void __cdecl UI_Gametype_WriteDvarIfChanged(MemoryFile *memFile, char *dvarName);
void __cdecl UI_Gametype_WriteDvars(MemoryFile *memFile);
char __cdecl HasClassDescriptionDataChanged();
void __cdecl UI_Gametype_WriteCustomClasses(MemoryFile *memFile);
void __cdecl UI_Gametype_WriteHeader(MemoryFile *memFile, UIGametypeFileHeader *header);
void __cdecl UI_Gametype_ReadHeader(MemoryFile *memFile, UIGametypeFileHeader *header);
void __cdecl UI_Gametype_WriteToMemFile(MemoryFile *memFile, UIGametypeFileHeader *header);
void __cdecl UI_Gametype_CopyDvarsToCustom();
void __cdecl UI_Gametype_CopyCustomToDvars();
char __cdecl UI_Gametype_HasAnyDvarsChanged(const char **dvarNameList, int dvarNameListSize, bool areGameSpecificDvars);
void __cdecl UI_Gametype_CheckIfAnythingChanged_f();
void __cdecl CheckForMinimumTeamClasses();
void __cdecl UI_Gametype_UpdateCustomClassDvarsForClass_f();
void __cdecl UI_Gametype_UpdateClassDataFromDvars_f();
void __cdecl UI_Gametype_CreateDefaultCustomClassIfNeeded_f();
void __cdecl UI_Gametype_ApplyCustomClassTeamSelections_f();
void __cdecl UI_Gametype_UpdateCustomClassTeamDvars_f();
char __cdecl UI_Gametype_ReadChunk(MemoryFile *memFile);
char __cdecl UI_Gametype_ReadChunks(MemoryFile *memFile);
char __cdecl UI_Gametype_ReadFromMemFile(MemoryFile *memFile);
void __cdecl ResetCustomClassDvars();
void __cdecl UI_Gametype_ResetDvars();
void __cdecl UI_Gametype_EndChanges();
void __cdecl UI_Gametype_EndChanges_f();
void __cdecl UI_Gametype_SetupCustomModeNameAndDesc();
void __cdecl UI_Gametype_BeginChanges_f();
bool __cdecl UI_Gametype_CanUseCustom();
bool __cdecl UI_Gametype_IsUsingCustom();
void __cdecl UI_Gametype_ResetGamemode();
void __cdecl UI_Gametype_Custom_Reset_f();
void __cdecl UI_Gametype_RevertChanges_f();
bool __cdecl isValidClassSlotNum(unsigned int slotNum);
unsigned __int8 __cdecl GetCustomClassLoadoutItemForSlot(
        int controllerIndex,
        unsigned int slotNum,
        const char *itemName);
unsigned __int8 __cdecl GetCustomClassLoadoutItemForClassName(
        int controllerIndex,
        const char *className,
        const char *itemName);
int __cdecl GetCustomClassModifierForClass(int controllerIndex, unsigned int classNum, const char *itemName);
void __cdecl UI_Gametype_EquipCustomClassLoadoutItem(
        int controllerIndex,
        const char *customClassName,
        unsigned __int8 itemIndex,
        const char *loadoutName);
int __thiscall _CustomClassData::GetClassModifierValue(_CustomClassData *this, const char *modifierName);
unsigned __int8 __thiscall _CustomClassData::GetLoadoutItem(_CustomClassData *this, const char *itemName);
void __thiscall _CustomClassData::SetLoadoutItem(
        _CustomClassData *this,
        const char *itemName,
        unsigned __int8 itemIndex);
void __thiscall _CustomClassData::PopulateCustomDvarsFromClassData(_CustomClassData *this);
void __thiscall _CustomClassData::SaveClassDataFromDvars(_CustomClassData *this);
void __thiscall _CustomClassData::WriteClassToClientData(_CustomClassData *this, msg_t *msg);
void __thiscall _CustomClassData::ReadClassFromClientData(_CustomClassData *this, msg_t *msg);
void __cdecl UI_Gametype_Custom_WriteClassClientData(msg_t *msg);
void __cdecl UI_Gametype_Custom_ReadClassClientData(msg_t *msg);
void __cdecl UI_Gametype_Custom_WriteKillstreakClientData(msg_t *msg);
void __cdecl UI_Gametype_Custom_ReadKillstreakClientData(msg_t *msg);
void __cdecl UI_Gametype_Custom_WritePerkDvarClientData(msg_t *msg);
void __cdecl UI_Gametype_Custom_ReadPerkDvarClientData(msg_t *msg);
void __cdecl UI_Gametype_Custom_WriteClientData(msg_t *msg);
char __cdecl UI_Gametype_Custom_ReadClientData(msg_t *msg);
bool __cdecl UI_Gametype_IsDirty();
void __cdecl UI_Gametype_MarkDirty();
bool __cdecl UI_Gametype_HasPerkChangedFromDefault(int itemIndex);
bool __cdecl UI_Gametype_Custom_IsPerkTweakable(int itemIndex);
void __cdecl UI_Gametype_Custom_ResolveNumKillsConflicts();
void __cdecl UI_Gametype_Custom_UploadToFileShareSuccess(int controllerIndex, unsigned __int64 fileID);
void __cdecl UI_Gametype_Custom_UploadToFileShareFailure();
void __cdecl UI_Gametype_Custom_UploadToFileShare(
        int controllerIndex,
        unsigned __int16 fileSlot,
        const char *fileName,
        MemoryFile *customGameMode);
void __cdecl UI_Gametype_UploadToFileShare_f();
void __cdecl UI_Gametype_FileShareDownloadComplete(dwFileShareReadFileTask *task);
void __cdecl UI_Gametype_FileShareDownloadFailed(dwFileShareReadFileTask *task);
void __cdecl UI_Gametype_DownloadFromFileShare_f();
void __cdecl SV_GameType_DownloadFromFileshare_f();
void __cdecl UI_Gametype_DisableCheats_f();
void __cdecl UI_Gametype_CopyCustomClass_f();
void __cdecl UI_Gametype_Custom_Init();
void __cdecl UI_Gametype_CustomGameModeDataToDvars();
void __cdecl UI_Gametype_DvarsToCustomGameModeData(CustomGameModeData *customGameModeData);
