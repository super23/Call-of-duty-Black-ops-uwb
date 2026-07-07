#include "g_save.h"

#include "actor.h"
#include "pathnode.h"
#include "sentient.h"
#include "g_items.h"

#include <game/g_loadgame_sp.h>
#include <game/g_main_wrapper.h>
#include <game/g_bsp.h>
#include <game/g_utils_wrapper.h>
#include <bgame/bg_weapons.h>
#include <bgame/bg_local.h>
#include <bgame/bg_weapons_def.h>
#include <qcommon/common.h>
#include <universal/com_files.h>
#include <qcommon/ent.h>
#include <qcommon/cm_world.h>
#ifdef KISAK_SP
#include <server_sp/sv_main_sp.h>
#include <server_sp/sv_init_sp.h>
#else
#include <server_mp/sv_main_mp.h>
#include <server_mp/sv_init_mp.h>
#endif
#include <game/g_scr_vehicle.h>
#include <server/server.h>
#include <string.h>
#include <universal/assertive.h>
#include <win32/win_shared.h>

// Decomp: CoDSPBlackOps.map.c — g_save / savememory

static const int kSentientSaveSize = 140;
static const int kEntityCacheEntrySize = 216;
static const int kMaxEntityCacheEntries = 350;

SaveGame g_saveGame;
unsigned __int8 g_saveServerBuffer[0x80000];
unsigned __int8 g_saveClientBuffer[0x80000];

static bool s_saveMemoryInitialized;
static bool s_saveInProgress;
static bool s_loadInProgress;
static unsigned int s_saveIdCounter;
static int s_saveCommitTimeMs;
static int g_saveEntityCacheCount;
static unsigned __int8 g_saveEntityCache[kMaxEntityCacheEntries * kEntityCacheEntrySize];

static const int kSaveHeaderSize = 0x4D8;
static const int kSaveVersionMin = 307;
static const int kSaveVersionMax = 308;

static int SaveMemory_ResetGameBuffers()
{
    memset(g_saveServerBuffer, 0, sizeof(g_saveServerBuffer));
    memset(g_saveClientBuffer, 0, sizeof(g_saveClientBuffer));
    memset(&g_saveGame, 0, sizeof(g_saveGame));
    g_saveGame.serverBuffer = g_saveServerBuffer;
    g_saveGame.clientBuffer = g_saveClientBuffer;
    g_saveGame.isUsingGlobalBuffer = true;
    return 1;
}

static void WriteCStyleString(unsigned __int8 *string, int maxLen, MemoryFile *memFile)
{
    int len;
    unsigned __int8 lenByte;
    unsigned __int16 lenShort;

    iassert(memFile);
    if ( !string )
        string = (unsigned __int8 *)"";
    len = (int)strlen((const char *)string);
    iassert(len < maxLen);
    if ( maxLen > 256 )
    {
        lenShort = (unsigned __int16)len;
        MemFile_WriteData(memFile, 2, (unsigned __int8 *)&lenShort);
    }
    else
    {
        lenByte = (unsigned __int8)len;
        MemFile_WriteData(memFile, 1, &lenByte);
    }
    if ( len > 0 )
        MemFile_WriteData(memFile, len, string);
}

static void WriteWeaponIndex(unsigned int weaponIndex, MemoryFile *memFile)
{
    unsigned __int8 *weaponName;

    iassert(memFile);
    if ( weaponIndex )
        weaponName = (unsigned __int8 *)BG_WeaponName(weaponIndex);
    else
        weaponName = (unsigned __int8 *)"";
    WriteCStyleString(weaponName, 256, memFile);
}

static void WriteItemIndex(int itemIndex, MemoryFile *memFile)
{
    unsigned __int8 modelIndex;

    iassert(memFile);
    if ( itemIndex )
    {
        iassert(bg_itemlist[itemIndex].giType == IT_WEAPON);
        WriteWeaponIndex(itemIndex % 128, memFile);
        modelIndex = (unsigned __int8)(itemIndex / 128);
        MemFile_WriteData(memFile, 1, &modelIndex);
    }
    else
    {
        WriteCStyleString((unsigned __int8 *)"", 256, memFile);
    }
}

static int Path_SaveIndex(pathnode_t *node)
{
    pathnode_t *nodes;

    if ( !node )
        return 0;
    nodes = gameWorldCurrent->path.nodes;
    if ( node < nodes || node >= nodes + g_path.actualNodeCount )
    {
        iassert(0);
        return 0;
    }
    return (int)((node - nodes) >> 7) + 1;
}

static pathnode_t *Path_LoadNode(int index)
{
    if ( index <= 0 || index > (int)g_path.actualNodeCount )
        return 0;
    return &gameWorldCurrent->path.nodes[index - 1];
}

static void SkipCStyleString(MemoryFile *memFile, int maxLen)
{
    unsigned __int8 lenByte;
    unsigned __int16 lenShort;
    int len;
    unsigned __int8 scratch[1024];

    if ( maxLen > 256 )
    {
        MemFile_ReadData(memFile, 2, (unsigned __int8 *)&lenShort);
        len = lenShort;
    }
    else
    {
        MemFile_ReadData(memFile, 1, &lenByte);
        len = lenByte;
    }
    if ( len > 0 )
        MemFile_ReadData(memFile, len, scratch);
}

static void SkipConfigstrings(MemoryFile *memFile, int count)
{
    int i;
    unsigned __int16 len;
    unsigned __int8 scratch[1024];

    for ( i = 0; i < count; ++i )
    {
        MemFile_ReadData(memFile, 2, (unsigned __int8 *)&len);
        if ( len > 0 )
            MemFile_ReadData(memFile, len, scratch);
    }
}

static void G_LoadInitState(SaveGame *save)
{
    MemoryFile *memFile;
    int numWeapons;
    int weaponIndex;
    unsigned __int8 itemMarker;

    SaveMemory_MoveToSegment(save, 1);
    memFile = SaveMemory_GetServerMemoryFile(save);
    MemFile_ReadData(memFile, 4, (unsigned __int8 *)&numWeapons);
    for ( weaponIndex = 1; weaponIndex < numWeapons; ++weaponIndex )
        SkipCStyleString(memFile, 256);
    do
    {
        MemFile_ReadData(memFile, 1, &itemMarker);
        if ( itemMarker )
            break;
        SkipCStyleString(memFile, 256);
        MemFile_ReadData(memFile, 1, &itemMarker);
    }
    while ( 1 );
    SkipConfigstrings(memFile, 512);
    SkipConfigstrings(memFile, 196);
    SkipConfigstrings(memFile, 256);
    SkipConfigstrings(memFile, 144);
    SkipConfigstrings(memFile, 1023);
    SkipConfigstrings(memFile, 1);
    SkipConfigstrings(memFile, 1);
    SkipConfigstrings(memFile, 1);
    SkipConfigstrings(memFile, 1);
    SkipConfigstrings(memFile, 1);
    SkipConfigstrings(memFile, 1);
    SkipConfigstrings(memFile, 1);
    SkipConfigstrings(memFile, 32);
    SkipConfigstrings(memFile, 32);
    SkipConfigstrings(memFile, 96);
    SkipConfigstrings(memFile, 8);
    SkipConfigstrings(memFile, 1);
    SkipConfigstrings(memFile, 32);
    SkipConfigstrings(memFile, 8);
}

static void G_LoadWeaponCue(MemoryFile *memFile)
{
    int cueIndex;
    int entnum;

    for ( cueIndex = 0; cueIndex < 32; ++cueIndex )
    {
        MemFile_ReadData(memFile, 4, (unsigned __int8 *)&entnum);
        if ( entnum > 0 && entnum < MAX_GENTITIES )
            level.droppedWeaponCue[cueIndex].setEnt(&g_entities[entnum]);
        else
            level.droppedWeaponCue[cueIndex].setEnt(0);
    }
}

static void G_LoadMainState(SaveGame *save)
{
    MemoryFile *memFile;
    char zoneName[64];
    int entIndex;
    int cacheIndex;

    iassert(save);
    memFile = SaveMemory_GetServerMemoryFile(save);
    G_LoadInitState(save);
    SaveMemory_MoveToSegment(save, 2);
    MemFile_ReadData(memFile, sizeof(zoneName), (unsigned __int8 *)zoneName);
    ReadPathNodes(memFile);
    G_LoadWeaponCue(memFile);
    MemFile_ReadData(memFile, 4, (unsigned __int8 *)&entIndex);
    while ( entIndex >= 0 )
    {
        if ( entIndex > 0 && entIndex < MAX_GENTITIES )
        {
            MemFile_ReadData(
                memFile,
                sizeof(entityState_s),
                (unsigned __int8 *)&sv.svEntities[entIndex].baseline.s);
            sv.svEntities[entIndex].worldSector = 1;
        }
        MemFile_ReadData(memFile, 4, (unsigned __int8 *)&entIndex);
    }
    g_saveEntityCacheCount = 0;
    MemFile_ReadData(memFile, 4, (unsigned __int8 *)&cacheIndex);
    while ( cacheIndex >= 0 )
    {
        if ( cacheIndex < kMaxEntityCacheEntries )
        {
            MemFile_ReadData(
                memFile,
                kEntityCacheEntrySize,
                g_saveEntityCache + cacheIndex * kEntityCacheEntrySize);
            if ( cacheIndex >= g_saveEntityCacheCount )
                g_saveEntityCacheCount = cacheIndex + 1;
        }
        MemFile_ReadData(memFile, 4, (unsigned __int8 *)&cacheIndex);
    }
}

static bool G_WriteGame(SaveGame *pendingSave, const char *description, SaveGame *saveHandle)
{
    (void)description;
    iassert(pendingSave);
    iassert(saveHandle);
    if ( !SaveMemory_InitializeGameSave(saveHandle) )
        return false;
    SaveMemory_StartSegment(saveHandle, 0);
    G_SaveState(saveHandle);
    SaveMemory_StartSegment(saveHandle, -1);
    if ( !SaveMemory_IsSuccessful(saveHandle) )
    {
        SaveMemory_RollbackSave(saveHandle);
        return false;
    }
    SaveMemory_FinalizeSave(saveHandle);
    CG_SavePlayerState(SaveMemory_GetClientMemoryFile(saveHandle));
    return SaveMemory_IsSuccessful(saveHandle);
}

int __cdecl SaveMemory_InitializeSaveSystem()
{
    s_saveCommitTimeMs = 0;
    s_saveMemoryInitialized = true;
    return SaveMemory_ResetGameBuffers();
}

unsigned int __cdecl SaveMemory_GenerateSaveId()
{
    return ++s_saveIdCounter;
}

MemoryFile *__cdecl SaveMemory_GetServerMemoryFile(SaveGame *save)
{
    iassert(save);
    return &save->serverMemFile;
}

MemoryFile *__cdecl SaveMemory_GetClientMemoryFile(SaveGame *save)
{
    iassert(save);
    return &save->clientMemFile;
}

void __cdecl SaveMemory_SaveWriteImmediate(SaveGame *save, const void *data, int size)
{
    MemoryFile *memFile;

    iassert(save);
    iassert(data);
    memFile = SaveMemory_GetServerMemoryFile(save);
    MemFile_WriteData(memFile, size, (unsigned __int8 *)data);
}

void __cdecl SaveMemory_SaveReadImmediate(SaveGame *save, void *data, int size)
{
    MemoryFile *memFile;

    iassert(save);
    iassert(data);
    memFile = SaveMemory_GetServerMemoryFile(save);
    MemFile_ReadData(memFile, size, (unsigned __int8 *)data);
}

SaveGame *__cdecl SaveMemory_GetSaveHandle(int slot)
{
    (void)slot;
    return &g_saveGame;
}

void __cdecl SaveMemory_ClearSaveGame(SaveGame *save)
{
    iassert(save);
    SaveMemory_ResetGameBuffers();
}

void __cdecl SaveMemory_StartSegment(SaveGame *save, int index)
{
    MemFile_StartSegment(SaveMemory_GetServerMemoryFile(save), index);
}

void __cdecl SaveMemory_MoveToSegment(SaveGame *save, int index)
{
    MemFile_MoveToSegment(SaveMemory_GetServerMemoryFile(save), index);
}

bool __cdecl SaveMemory_InitializeGameSave(SaveGame *save)
{
    iassert(save);
    iassert(save->isUsingGlobalBuffer);
    MemFile_InitForWriting(&save->serverMemFile, 0x80000, save->serverBuffer, true, true);
    MemFile_InitForWriting(&save->clientMemFile, 0x80000, save->clientBuffer, true, true);
    save->saveState = SAVE_STATE_SAVING;
    save->saveSuccessful = false;
    save->isSaving = true;
    save->waitingForCommit = false;
    s_saveCommitTimeMs = 0;
    s_saveInProgress = true;
    return true;
}

bool __cdecl SaveMemory_FinalizeSave(SaveGame *save)
{
    iassert(save);
    iassert(save->saveState == SAVE_STATE_SAVING);
    MemFile_EndSegment(&save->serverMemFile);
    MemFile_EndSegment(&save->clientMemFile);
    save->saveState = SAVE_STATE_SAVED;
    save->saveSuccessful = SaveMemory_IsSuccessful(save);
    save->isSaving = false;
    s_saveInProgress = false;
    return save->saveSuccessful;
}

void __cdecl SaveMemory_FreeMemory(SaveGame *save)
{
    iassert(save);
    if ( save->isUsingGlobalBuffer )
    {
        SaveMemory_ClearSaveGame(save);
        return;
    }
    save->saveState = SAVE_STATE_NONE;
    save->isSaving = false;
    save->isLoading = false;
    save->saveSuccessful = false;
}

bool __cdecl SaveMemory_InitializeLoad(SaveGame *save)
{
    int serverSize;

    iassert(save);
    serverSize = save->serverMemFile.bytesUsed;
    if ( serverSize <= 0 )
        serverSize = 0x80000;
    MemFile_InitForReading(&save->serverMemFile, serverSize, save->serverBuffer, true);
    MemFile_InitForReading(&save->clientMemFile, 0x80000, save->clientBuffer, true);
    save->saveState = SAVE_STATE_LOADING;
    save->isLoading = true;
    s_loadInProgress = true;
    return true;
}

bool __cdecl SaveMemory_IsRecentlyLoaded()
{
    return s_saveCommitTimeMs != 0 && Sys_Milliseconds() - s_saveCommitTimeMs < 2000;
}

bool __cdecl SaveMemory_FinalizeLoad(SaveGame *save)
{
    iassert(save);
    iassert(save->saveState == SAVE_STATE_LOADING);
    if ( !SaveMemory_IsSuccessful(save) )
    {
        SaveMemory_FreeMemory(save);
        return false;
    }
    save->saveState = SAVE_STATE_LOADED;
    save->saveSuccessful = true;
    save->isLoading = false;
    s_loadInProgress = false;
    s_saveCommitTimeMs = Sys_Milliseconds();
    return true;
}

bool __cdecl SaveMemory_IsSuccessful(SaveGame *save)
{
    iassert(save);
    return !save->serverMemFile.memoryOverflow && !save->clientMemFile.memoryOverflow;
}

bool __cdecl SaveMemory_IsSaving(SaveGame *save)
{
    iassert(save);
    return save->isSaving || s_saveInProgress;
}

bool __cdecl SaveMemory_IsLoading(SaveGame *save)
{
    iassert(save);
    return save->isLoading || s_loadInProgress;
}

bool __cdecl SaveMemory_IsWaitingForCommit(SaveGame *save)
{
    iassert(save);
    return save->waitingForCommit;
}

unsigned int __cdecl SaveMemory_CalculateChecksum(SaveGame *save)
{
    unsigned int checksum = 0;
    int i;

    iassert(save);
    for ( i = 0; i < save->serverMemFile.bytesUsed; ++i )
        checksum += save->serverBuffer[i];
    save->checksum = checksum;
    return checksum;
}

bool __cdecl SaveMemory_InitializeLoadFromBuffer(SaveGame *save, unsigned __int8 *buffer, int size)
{
    iassert(save);
    iassert(buffer);
    if ( size > (int)sizeof(g_saveServerBuffer) )
        return false;
    memcpy(save->serverBuffer, buffer, size);
    save->serverMemFile.bytesUsed = size;
    return SaveMemory_InitializeLoad(save);
}

void __cdecl SaveMemory_SetBuffer(SaveGame *save, unsigned __int8 *buffer, int size)
{
    iassert(save);
    iassert(buffer);
    save->serverBuffer = buffer;
    save->serverMemFile.buffer = buffer;
    save->serverMemFile.bufferSize = size;
}

int __cdecl SaveMemory_GetTotalLoadSize(SaveGame *save)
{
    iassert(save);
    return save->serverMemFile.bytesUsed;
}

void __cdecl SaveMemory_CreateHeader(SaveGame *save)
{
    iassert(save);
    save->saveId = SaveMemory_GenerateSaveId();
}

void *__cdecl SaveMemory_GetHeader(SaveGame *save)
{
    iassert(save);
    return save;
}

// Decomp: CoDSP_rdBlackOps.map.c:8267E1E8 (SaveMemory_ReadLoadFromDevice)
bool __cdecl SaveMemory_ReadLoadFromDevice(SaveGame *save, int slot)
{
    const char *filename;
    unsigned int header[kSaveHeaderSize / sizeof(unsigned int)];
    unsigned int serverBodySize;
    unsigned int clientBodySize;
    int saveVersion;
    int saveId;
    int fileHandle;
    unsigned int fileSize;
    dvar_s *uiCampaign;

    iassert(save);

    filename = G_GetPendingLoadGamePath();
    if ( !filename || !filename[0] )
    {
        Com_PrintWarning(15, "SaveMemory_ReadLoadFromDevice: no save path set\n");
        return false;
    }

    SaveMemory_ResetGameBuffers();

    fileHandle = 0;
    fileSize = FS_FOpenFileRead(filename, &fileHandle);
    if ( fileHandle <= 0 )
    {
        Com_PrintWarning(15, "SaveMemory_ReadLoadFromDevice: can't find savegame %s\n", filename);
        return false;
    }

    if ( FS_Read((unsigned __int8 *)header, kSaveHeaderSize, fileHandle) != (unsigned int)kSaveHeaderSize )
    {
        FS_FCloseFile(fileHandle);
        Com_PrintWarning(15, "SaveMemory_ReadLoadFromDevice: bad save read [%s]\n", filename);
        return false;
    }

    saveVersion = (int)header[0];
    if ( saveVersion < kSaveVersionMin || saveVersion > kSaveVersionMax )
    {
        FS_FCloseFile(fileHandle);
        Com_PrintWarning(
            15,
            "SaveMemory_ReadLoadFromDevice: bad save version %d, expecting %d-%d [%s]\n",
            saveVersion,
            kSaveVersionMin,
            kSaveVersionMax,
            filename);
        return false;
    }

    serverBodySize = header[276];
    clientBodySize = header[277];
    if ( serverBodySize > 0x80000 || clientBodySize > 0x80000 )
    {
        FS_FCloseFile(fileHandle);
        Com_PrintWarning(15, "SaveMemory_ReadLoadFromDevice: save body too large [%s]\n", filename);
        return false;
    }

    if ( (int)fileSize < kSaveHeaderSize + (int)serverBodySize + (int)clientBodySize )
    {
        FS_FCloseFile(fileHandle);
        Com_PrintWarning(15, "SaveMemory_ReadLoadFromDevice: save file truncated [%s]\n", filename);
        return false;
    }

    if ( FS_Read(save->serverBuffer, serverBodySize, fileHandle) != serverBodySize )
    {
        FS_FCloseFile(fileHandle);
        Com_PrintWarning(15, "SaveMemory_ReadLoadFromDevice: could not read server body [%s]\n", filename);
        return false;
    }
    save->serverMemFile.bytesUsed = (int)serverBodySize;

    if ( FS_Read(save->clientBuffer, clientBodySize, fileHandle) != clientBodySize )
    {
        FS_FCloseFile(fileHandle);
        Com_PrintWarning(15, "SaveMemory_ReadLoadFromDevice: could not read client body [%s]\n", filename);
        return false;
    }
    save->clientMemFile.bytesUsed = (int)clientBodySize;

    FS_FCloseFile(fileHandle);

    uiCampaign = (dvar_s *)_Dvar_RegisterString("ui_campaign", "american", 0x1000u, "The current campaign");
    if ( uiCampaign )
        Dvar_SetString(uiCampaign, (char *)&header[104]);

    Com_Printf(15, "Save game build number: %s\n", (char *)&header[72]);

    saveId = (int)header[1];
    if ( slot && saveId != slot )
    {
        Com_PrintWarning(
            15,
            "SaveMemory_ReadLoadFromDevice: save id mismatch (save=%d slot=%d) [%s]\n",
            saveId,
            slot,
            filename);
        return false;
    }

    I_strncpyz(save->mapName, (char *)&header[8], sizeof(save->mapName));
    if ( header[248] )
        I_strncpyz(save->description, (char *)&header[248], sizeof(save->description));

    save->saveId = header[3] + 1;
    save->isUsingGlobalBuffer = true;

    return SaveMemory_InitializeLoad(save);
}

bool __cdecl SaveMemory_IsCommittedSaveAvailable(SaveGame *save)
{
    iassert(save);
    return save->saveState == SAVE_STATE_SAVED && save->saveSuccessful;
}

bool __cdecl SaveMemory_CommitSave(SaveGame *save)
{
    iassert(save);
    save->waitingForCommit = false;
    return SaveMemory_FinalizeSave(save);
}

void __cdecl SaveMemory_RollbackSave(SaveGame *save)
{
    iassert(save);
    SaveMemory_ClearSaveGame(save);
}

bool __cdecl SaveMemory_WriteSaveToDevice(SaveGame *save, int slot)
{
    (void)slot;
    iassert(save);
    return save->saveSuccessful;
}

bool __cdecl SaveMemory_FinalizeSaveCommit(SaveGame *save)
{
    return SaveMemory_CommitSave(save);
}

bool __cdecl SaveMemory_ReadZorkSaveFromDevice(SaveGame *save, int slot)
{
    return SaveMemory_ReadLoadFromDevice(save, slot);
}

bool __cdecl SaveMemory_WriteZorkSaveToDevice(SaveGame *save, int slot)
{
    return SaveMemory_WriteSaveToDevice(save, slot);
}

bool __cdecl SaveMemory_IsWrittenToDevice(SaveGame *save)
{
    iassert(save);
    return save->saveSuccessful;
}

bool __cdecl SaveMemory_IsAvailable(SaveGame *save)
{
    iassert(save);
    return s_saveMemoryInitialized;
}

void __cdecl SaveMemory_ForceCommitSave(SaveGame *save)
{
    SaveMemory_CommitSave(save);
}

void __cdecl SV_BeginSaveGame()
{
    s_saveInProgress = true;
}

bool __cdecl SV_EndSaveGame()
{
    s_saveInProgress = false;
    return true;
}

void __cdecl WriteSentient(sentient_s *sentient, MemoryFile *memFile)
{
    iassert(sentient);
    iassert(memFile);
    MemFile_WriteByte(memFile, sentient->inuse);
    if ( sentient->inuse )
        MemFile_WriteData(memFile, kSentientSaveSize, (unsigned __int8 *)sentient);
}

void __cdecl ReadSentient(sentient_s *sentient, MemoryFile *memFile)
{
    unsigned __int8 inuse;

    iassert(sentient);
    iassert(memFile);
    MemFile_ReadData(memFile, 1, &inuse);
    sentient->inuse = inuse != 0;
    if ( sentient->inuse )
    {
        MemFile_ReadData(memFile, kSentientSaveSize, (unsigned __int8 *)sentient);
        iassert(sentient->inuse);
    }
}

void __cdecl WriteActorPotentialCoverNodes(actor_s *actor, MemoryFile *memFile)
{
    int i;

    iassert(actor);
    iassert(memFile);
    MemFile_WriteInt(memFile, actor->iPotentialCoverNodeCount);
    for ( i = 0; i < actor->iPotentialCoverNodeCount; ++i )
        MemFile_WriteInt(memFile, Path_SaveIndex(actor->pPotentialCoverNode[i]));
}

void __cdecl ReadActorPotentialCoverNodes(actor_s *actor, MemoryFile *memFile)
{
    int i;
    int count;
    int nodeIndex;

    iassert(actor);
    iassert(memFile);
    MemFile_ReadData(memFile, 4, (unsigned __int8 *)&count);
    actor->iPotentialCoverNodeCount = count;
    for ( i = 0; i < count && i < 10; ++i )
    {
        MemFile_ReadData(memFile, 4, (unsigned __int8 *)&nodeIndex);
        actor->pPotentialCoverNode[i] = Path_LoadNode(nodeIndex);
    }
}

void __cdecl WriteActor(actor_s *actor, MemoryFile *memFile)
{
    iassert(memFile);
    MemFile_WriteInt(memFile, actor->inuse);
    if ( actor->inuse )
    {
        MemFile_WriteData(memFile, sizeof(actor_s), (unsigned __int8 *)actor);
        WriteActorPotentialCoverNodes(actor, memFile);
    }
}

void __cdecl ReadActor(actor_s *actor, MemoryFile *memFile)
{
    gentity_s *ent;
    sentient_s *sentient;

    iassert(actor);
    iassert(memFile);
    MemFile_ReadData(memFile, 4, (unsigned __int8 *)&actor->inuse);
    if ( actor->inuse )
    {
        ent = actor->ent;
        sentient = actor->sentient;
        MemFile_ReadData(memFile, sizeof(actor_s), (unsigned __int8 *)actor);
        actor->ent = ent;
        actor->sentient = sentient;
        ReadActorPotentialCoverNodes(actor, memFile);
        iassert(actor->inuse);
    }
}

void __cdecl WritePathNodes(MemoryFile *memFile)
{
    int nodeCount;

    iassert(memFile);
    nodeCount = gameWorldCurrent->path.nodeCount;
    MemFile_WriteInt(memFile, nodeCount);
    if ( nodeCount > 0 )
        MemFile_WriteData(
            memFile,
            nodeCount * sizeof(pathnode_t),
            (unsigned __int8 *)gameWorldCurrent->path.nodes);
}

void __cdecl ReadPathNodes(MemoryFile *memFile)
{
    int nodeCount;

    iassert(memFile);
    MemFile_ReadData(memFile, 4, (unsigned __int8 *)&nodeCount);
    if ( nodeCount > 0 && nodeCount <= gameWorldCurrent->path.nodeCount )
        MemFile_ReadData(memFile, nodeCount * sizeof(pathnode_t), (unsigned __int8 *)gameWorldCurrent->path.nodes);
}

void __cdecl G_SaveConfigstrings(int firstIndex, int count, MemoryFile *memFile)
{
    int i;
    char configString[1024];
    unsigned __int16 len;

    iassert(memFile);
    for ( i = 0; i < count; ++i )
    {
        SV_GetConfigstring(firstIndex + i, configString, sizeof(configString));
        len = (unsigned __int16)strlen(configString);
        iassert(len < sizeof(configString));
        MemFile_WriteData(memFile, 2, (unsigned __int8 *)&len);
        if ( len > 0 )
            MemFile_WriteData(memFile, len, (unsigned __int8 *)configString);
    }
}

void __cdecl G_SaveInitConfigstrings(MemoryFile *memFile)
{
    iassert(memFile);
    G_SaveConfigstrings(1560, 512, memFile);
    G_SaveConfigstrings(2072, 196, memFile);
    G_SaveConfigstrings(2268, 256, memFile);
    G_SaveConfigstrings(2572, 144, memFile);
    G_SaveConfigstrings(487, 1023, memFile);
    G_SaveConfigstrings(1519, 1, memFile);
    G_SaveConfigstrings(7, 1, memFile);
    G_SaveConfigstrings(8, 1, memFile);
    G_SaveConfigstrings(9, 1, memFile);
    G_SaveConfigstrings(1553, 1, memFile);
    G_SaveConfigstrings(1556, 1, memFile);
    G_SaveConfigstrings(1555, 1, memFile);
    G_SaveConfigstrings(327, 32, memFile);
    G_SaveConfigstrings(359, 32, memFile);
    G_SaveConfigstrings(391, 96, memFile);
    G_SaveConfigstrings(1510, 8, memFile);
    G_SaveConfigstrings(10, 1, memFile);
    G_SaveConfigstrings(3003, 32, memFile);
    G_SaveConfigstrings(319, 8, memFile);
}

void __cdecl G_SaveItems(MemoryFile *memFile)
{
    int itemIndex;
    unsigned __int8 endMarker;

    iassert(memFile);
    for ( itemIndex = 1; itemIndex < 128; ++itemIndex )
    {
        if ( IsItemRegistered(itemIndex) )
        {
            endMarker = 0;
            MemFile_WriteData(memFile, 1, &endMarker);
            WriteItemIndex(itemIndex, memFile);
        }
    }
    endMarker = 1;
    MemFile_WriteData(memFile, 1, &endMarker);
}

void __cdecl G_SaveInitState(SaveGame *save)
{
    MemoryFile *memFile;
    int weaponIndex;
    int numWeapons;

    iassert(save);
    SaveMemory_StartSegment(save, 1);
    memFile = SaveMemory_GetServerMemoryFile(save);
    numWeapons = BG_GetNumWeapons();
    MemFile_WriteInt(memFile, numWeapons);
    for ( weaponIndex = 1; weaponIndex < numWeapons; ++weaponIndex )
        WriteCStyleString((unsigned __int8 *)BG_WeaponName(weaponIndex), 256, memFile);
    G_SaveItems(memFile);
    G_SaveInitConfigstrings(memFile);
}

void __cdecl G_SaveBaselines(MemoryFile *memFile)
{
    int entIndex;

    iassert(memFile);
    for ( entIndex = 1; entIndex < sv.num_entities; ++entIndex )
    {
        if ( sv.svEntities[entIndex].worldSector )
        {
            MemFile_WriteInt(memFile, entIndex);
            MemFile_WriteData(
                memFile,
                sizeof(entityState_s),
                (unsigned __int8 *)&sv.svEntities[entIndex].baseline.s);
        }
    }
    MemFile_WriteInt(memFile, -1);
}

void __cdecl G_SaveEntityCache(MemoryFile *memFile)
{
    int cacheIndex;

    iassert(memFile);
    for ( cacheIndex = 0; cacheIndex < g_saveEntityCacheCount; ++cacheIndex )
    {
        MemFile_WriteInt(memFile, cacheIndex);
        MemFile_WriteData(
            memFile,
            kEntityCacheEntrySize,
            g_saveEntityCache + cacheIndex * kEntityCacheEntrySize);
    }
    MemFile_WriteInt(memFile, -1);
}

void __cdecl G_SaveWeaponCue(MemoryFile *memFile)
{
    int cueIndex;
    int entnum;

    iassert(memFile);
    for ( cueIndex = 0; cueIndex < 32; ++cueIndex )
    {
        if ( level.droppedWeaponCue[cueIndex].isDefined() )
            entnum = level.droppedWeaponCue[cueIndex].entnum() + 1;
        else
            entnum = 0;
        MemFile_WriteInt(memFile, entnum);
    }
}

void __cdecl G_SaveDvars(MemoryFile *memFile)
{
    iassert(memFile);
}

void __cdecl G_SaveLevelBGS(MemoryFile *memFile)
{
    iassert(memFile);
}

void __cdecl G_SaveTimedDamage(MemoryFile *memFile)
{
    iassert(memFile);
}

void __cdecl G_SaveCurveInfo(MemoryFile *memFile)
{
    (void)memFile;
}

void __cdecl G_SaveVehicleInfo(MemoryFile *memFile)
{
    int vehicleIndex;
    int fieldIndex;
    const vehicle_info_t *info;
    int *fieldPtr;

    iassert(memFile);
    for ( vehicleIndex = 0; vehicleIndex < bg_numVehicleInfos; ++vehicleIndex )
    {
        info = BG_GetVehicleInfo(vehicleIndex);
        if ( !info )
            continue;
        fieldPtr = (int *)((char *)info + 2384);
        for ( fieldIndex = 0; fieldIndex < 19; ++fieldIndex )
        {
            if ( fieldPtr[fieldIndex] )
                MemFile_WriteInt(memFile, fieldPtr[fieldIndex]);
            fieldPtr++;
        }
    }
}

void __cdecl G_SaveMainState(SaveGame *save)
{
    MemoryFile *memFile;
    char zoneName[64];

    iassert(save);
    memFile = SaveMemory_GetServerMemoryFile(save);
    SaveMemory_StartSegment(save, 2);
    memset(zoneName, 0, sizeof(zoneName));
    MemFile_WriteData(memFile, sizeof(zoneName), (unsigned __int8 *)zoneName);
    G_SaveDvars(memFile);
    WritePathNodes(memFile);
    G_SaveWeaponCue(memFile);
    G_SaveBaselines(memFile);
    G_SaveEntityCache(memFile);
    G_SaveLevelBGS(memFile);
    G_SaveTimedDamage(memFile);
    G_SaveCurveInfo(memFile);
    G_SaveVehicleInfo(memFile);
}

void __cdecl G_SaveState(SaveGame *save)
{
    iassert(save);
    G_SaveInitState(save);
    G_SaveMainState(save);
}

void __cdecl BG_SaveFlameSystem(MemoryFile *memFile) { (void)memFile; }
void __cdecl BG_SaveShellShockDvars(MemoryFile *memFile) { (void)memFile; }
void __cdecl CG_SaveBolt(MemoryFile *memFile) { (void)memFile; }
void __cdecl CG_SaveView_f() {}
void __cdecl CG_SaveEntities(MemoryFile *memFile) { (void)memFile; }
void __cdecl CG_SaveEntity(MemoryFile *memFile, int entnum) { (void)memFile; (void)entnum; }
void __cdecl CG_SaveFakeEntities(MemoryFile *memFile) { (void)memFile; }
void __cdecl CG_SaveFakeEntity(MemoryFile *memFile, int entnum) { (void)memFile; (void)entnum; }
void __cdecl CG_SaveScreen(MemoryFile *memFile) { (void)memFile; }
void __cdecl CG_SaveScreenSection(MemoryFile *memFile, int section) { (void)memFile; (void)section; }
void __cdecl CG_SaveViewModelAnimTrees(MemoryFile *memFile) { (void)memFile; }
void __cdecl CG_SaveWeaponState(MemoryFile *memFile) { (void)memFile; }
void __cdecl CG_SaveDynEntities(MemoryFile *memFile) { (void)memFile; }
void __cdecl CG_SaveDestructibles(MemoryFile *memFile) { (void)memFile; }
void __cdecl CG_SaveGenericFilters(MemoryFile *memFile) { (void)memFile; }
void __cdecl CG_SavePlayerState(MemoryFile *memFile) { (void)memFile; }

bool __cdecl G_SaveGame(SaveGame *pendingSave, const char *description)
{
    SaveGame *saveHandle;

    iassert(pendingSave);
    if ( !Com_IsMenuLevel(0) && level.maxclients > 0 && svs.clients[0].header.state < CS_CONNECTED )
        return false;
    if ( !s_saveMemoryInitialized )
        SaveMemory_InitializeSaveSystem();
    saveHandle = SaveMemory_GetSaveHandle(0);
    iassert(saveHandle);
    SV_BeginSaveGame();
    if ( !G_WriteGame(pendingSave, description, saveHandle) )
    {
        SV_EndSaveGame();
        return false;
    }
    if ( description )
        I_strncpyz(saveHandle->description, description, sizeof(saveHandle->description));
    SaveMemory_CreateHeader(saveHandle);
    SaveMemory_CalculateChecksum(saveHandle);
    SV_EndSaveGame();
    return true;
}

bool __cdecl G_PreLoadGame(SaveGame *pendingSave, const char *mapName)
{
    iassert(pendingSave);
    iassert(mapName);
    if ( !s_saveMemoryInitialized )
        SaveMemory_InitializeSaveSystem();
    I_strncpyz(pendingSave->mapName, mapName, sizeof(pendingSave->mapName));
    return SaveMemory_InitializeLoad(pendingSave);
}

bool __cdecl G_LoadGame(SaveGame *pendingSave)
{
    unsigned int randomSeed;

    iassert(pendingSave);
    if ( !SaveMemory_IsLoading(pendingSave) )
        return false;
    G_LoadMainState(pendingSave);
    if ( !SaveMemory_FinalizeLoad(pendingSave) )
        return false;
    randomSeed = Sys_Milliseconds();
    CM_LinkWorld();
    G_srand(randomSeed);
    return true;
}
