#pragma once

#include <universal/memfile.h>

struct actor_s;
struct sentient_s;
struct MemoryFile;
struct SaveGame;

// Decomp: CoDSPBlackOps.map.c — g_save / savememory

void __cdecl BG_SaveFlameSystem(MemoryFile *memFile);
void __cdecl BG_SaveShellShockDvars(MemoryFile *memFile);
void __cdecl CG_SaveBolt(MemoryFile *memFile);
void __cdecl CG_SaveView_f();
void __cdecl CG_SaveEntities(MemoryFile *memFile);
void __cdecl CG_SaveEntity(MemoryFile *memFile, int entnum);
void __cdecl CG_SaveFakeEntities(MemoryFile *memFile);
void __cdecl CG_SaveFakeEntity(MemoryFile *memFile, int entnum);
void __cdecl CG_SaveScreen(MemoryFile *memFile);
void __cdecl CG_SaveScreenSection(MemoryFile *memFile, int section);
void __cdecl CG_SaveViewModelAnimTrees(MemoryFile *memFile);
void __cdecl CG_SaveWeaponState(MemoryFile *memFile);
void __cdecl CG_SaveDynEntities(MemoryFile *memFile);
void __cdecl G_SaveTimedDamage(MemoryFile *memFile);
void __cdecl WritePathNodes(MemoryFile *memFile);
void __cdecl ReadPathNodes(MemoryFile *memFile);
void __cdecl G_SaveState(SaveGame *save);
void __cdecl G_SaveInitState(SaveGame *save);
void __cdecl G_SaveInitConfigstrings(MemoryFile *memFile);
void __cdecl G_SaveConfigstrings(int firstIndex, int count, MemoryFile *memFile);
void __cdecl G_SaveItems(MemoryFile *memFile);
void __cdecl G_SaveMainState(SaveGame *save);
void __cdecl WriteActor(actor_s *actor, MemoryFile *memFile);
void __cdecl WriteActorPotentialCoverNodes(actor_s *actor, MemoryFile *memFile);
void __cdecl WriteSentient(sentient_s *sentient, MemoryFile *memFile);
void __cdecl G_SaveWeaponCue(MemoryFile *memFile);
void __cdecl G_SaveDvars(MemoryFile *memFile);
void __cdecl G_SaveBaselines(MemoryFile *memFile);
void __cdecl G_SaveEntityCache(MemoryFile *memFile);
void __cdecl G_SaveLevelBGS(MemoryFile *memFile);
bool __cdecl G_SaveGame(SaveGame *pendingSave, const char *description);
bool __cdecl G_PreLoadGame(SaveGame *pendingSave, const char *mapName);
bool __cdecl G_LoadGame(SaveGame *pendingSave);
void __cdecl ReadActor(actor_s *actor, MemoryFile *memFile);
void __cdecl ReadActorPotentialCoverNodes(actor_s *actor, MemoryFile *memFile);
void __cdecl ReadSentient(sentient_s *sentient, MemoryFile *memFile);
void __cdecl G_SaveCurveInfo(MemoryFile *memFile);
void __cdecl G_SaveVehicleInfo(MemoryFile *memFile);
void __cdecl CG_SaveDestructibles(MemoryFile *memFile);
void __cdecl CG_SaveGenericFilters(MemoryFile *memFile);
void __cdecl CG_SavePlayerState(MemoryFile *memFile);

MemoryFile *__cdecl SaveMemory_GetServerMemoryFile(SaveGame *save);
MemoryFile *__cdecl SaveMemory_GetClientMemoryFile(SaveGame *save);
void __cdecl SaveMemory_SaveWriteImmediate(SaveGame *save, const void *data, int size);
void __cdecl SaveMemory_SaveReadImmediate(SaveGame *save, void *data, int size);
SaveGame *__cdecl SaveMemory_GetSaveHandle(int slot);
void __cdecl SaveMemory_ClearSaveGame(SaveGame *save);
void __cdecl SaveMemory_StartSegment(SaveGame *save, int index);
void __cdecl SaveMemory_MoveToSegment(SaveGame *save, int index);
bool __cdecl SaveMemory_InitializeGameSave(SaveGame *save);
bool __cdecl SaveMemory_FinalizeSave(SaveGame *save);
void __cdecl SaveMemory_FreeMemory(SaveGame *save);
bool __cdecl SaveMemory_InitializeLoad(SaveGame *save);
bool __cdecl SaveMemory_FinalizeLoad(SaveGame *save);
bool __cdecl SaveMemory_IsRecentlyLoaded();
bool __cdecl SaveMemory_IsSuccessful(SaveGame *save);
bool __cdecl SaveMemory_IsSaving(SaveGame *save);
bool __cdecl SaveMemory_IsLoading(SaveGame *save);
bool __cdecl SaveMemory_IsWaitingForCommit(SaveGame *save);
unsigned int __cdecl SaveMemory_CalculateChecksum(SaveGame *save);
bool __cdecl SaveMemory_InitializeLoadFromBuffer(SaveGame *save, unsigned __int8 *buffer, int size);
void __cdecl SaveMemory_SetBuffer(SaveGame *save, unsigned __int8 *buffer, int size);
int __cdecl SaveMemory_GetTotalLoadSize(SaveGame *save);
void __cdecl SaveMemory_CreateHeader(SaveGame *save);
void *__cdecl SaveMemory_GetHeader(SaveGame *save);
bool __cdecl SaveMemory_ReadLoadFromDevice(SaveGame *save, int slot);
bool __cdecl SaveMemory_IsCommittedSaveAvailable(SaveGame *save);
bool __cdecl SaveMemory_CommitSave(SaveGame *save);
void __cdecl SaveMemory_RollbackSave(SaveGame *save);
bool __cdecl SaveMemory_WriteSaveToDevice(SaveGame *save, int slot);
bool __cdecl SaveMemory_FinalizeSaveCommit(SaveGame *save);
bool __cdecl SaveMemory_ReadZorkSaveFromDevice(SaveGame *save, int slot);
bool __cdecl SaveMemory_WriteZorkSaveToDevice(SaveGame *save, int slot);
bool __cdecl SaveMemory_IsWrittenToDevice(SaveGame *save);
bool __cdecl SaveMemory_IsAvailable(SaveGame *save);
void __cdecl SaveMemory_ForceCommitSave(SaveGame *save);
bool __cdecl SaveMemory_IsCommitForced();
void __cdecl SaveMemory_ClearForcedCommitFlag();
int __cdecl SaveMemory_InitializeSaveSystem();
unsigned int __cdecl SaveMemory_GenerateSaveId();

bool __cdecl G_CommitSavedGame(int saveId);

#define PENDING_SAVES_LIMIT 3

struct PendingSave
{
    char filename[64];
    char description[256];
    char screenshot[64];
    unsigned int saveId;
    int loadGame;
    unsigned int saveFlags;
    bool commitNow;
    bool pad[3];
    void (*callback)();
    int callbackData;
};

int __cdecl SV_AddPendingSave(
    const char *filename,
    const char *description,
    const char *screenshot,
    int loadGame,
    unsigned int saveFlags,
    bool commitNow,
    void (*callback)());
void __cdecl SV_ProcessPendingSaves();

void __cdecl SV_BeginSaveGame();
bool __cdecl SV_EndSaveGame();

enum SaveGameState : int
{
    SAVE_STATE_NONE = 0,
    SAVE_STATE_SAVING = 1,
    SAVE_STATE_SAVED = 2,
    SAVE_STATE_LOADED = 3,
    SAVE_STATE_LOADING = 4,
};

struct SaveGame // sizeof=0x105A0 (approx, decomp layout)
{
    unsigned __int8 *serverBuffer;
    unsigned __int8 *clientBuffer;
    MemoryFile serverMemFile;
    MemoryFile clientMemFile;
    SaveGameState saveState;
    int loadState;
    bool isUsingGlobalBuffer;
    bool saveSuccessful;
    bool isSaving;
    bool isLoading;
    bool waitingForCommit;
    unsigned int saveId;
    unsigned int checksum;
    char mapName[64];
    char description[256];
};

extern SaveGame g_saveGame;
extern unsigned __int8 g_saveServerBuffer[0x80000];
extern unsigned __int8 g_saveClientBuffer[0x80000];
