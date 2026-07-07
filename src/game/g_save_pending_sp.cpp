#include "g_save.h"

#include <game/g_main_sp.h>
#include <game/g_utils_wrapper.h>
#include <clientscript/scr_const.h>
#include <qcommon/common.h>
#include <universal/com_shared.h>
#include <universal/q_shared.h>
#include <win32/win_shared.h>
#include <string.h>
#include <universal/assertive.h>

// Decomp: CoDSP_rdBlackOps.map.c — pending save queue (SV_AddPendingSave / SV_ProcessPendingSaves)

static bool s_commitForced;
static PendingSave s_pendingSaves[PENDING_SAVES_LIMIT];
static int s_pendingSaveCount;
static int s_pendingLastSaveTime;
static bool s_pendingSaveSameFrame;

extern const dvar_t *g_reloading;

// Decomp: CoDSP_rdBlackOps.map.c:6234396
bool __cdecl SaveMemory_IsCommitForced()
{
    return s_commitForced;
}

// Decomp: CoDSP_rdBlackOps.map.c:6234402
void __cdecl SaveMemory_ClearForcedCommitFlag()
{
    s_commitForced = false;
}

// Decomp: CoDSP_rdBlackOps.map.c:6108239
bool __cdecl G_CommitSavedGame(int saveId)
{
    SaveGame *save;

    save = SaveMemory_GetSaveHandle(0);
    iassert(save);
    if ( (int)save->saveId != saveId )
        return false;
    return SaveMemory_CommitSave(save);
}

static void SV_ResetPendingLastSaveTime()
{
    s_pendingLastSaveTime = Sys_Milliseconds();
}

static int SV_ProcessPendingSave(PendingSave *pendingSave)
{
    SaveGame *saveHandle;
    gentity_s *player;
    bool saved;

    iassert(pendingSave);
    player = G_Find(0, 356, scr_const.player);
    if ( !player || player->health <= 0 )
        return 0;

    if ( pendingSave->callback )
        pendingSave->callback();

    saveHandle = SaveMemory_GetSaveHandle(0);
    iassert(saveHandle);
    saveHandle->saveId = pendingSave->saveId;
    saveHandle->waitingForCommit = !pendingSave->commitNow;
    I_strncpyz(saveHandle->mapName, pendingSave->filename, sizeof(saveHandle->mapName));

    saved = G_SaveGame(saveHandle, pendingSave->description);
    if ( saved && pendingSave->commitNow )
        G_CommitSavedGame(pendingSave->saveId);
    return saved ? 1 : 0;
}

// Decomp: CoDSP_rdBlackOps.map.c:6727820
int __cdecl SV_AddPendingSave(
    const char *filename,
    const char *description,
    const char *screenshot,
    int loadGame,
    unsigned int saveFlags,
    bool commitNow,
    void (*callback)())
{
    PendingSave *pendingSave;
    int now;

    iassert(filename);
    iassert(description);
    iassert(screenshot);
    (void)saveFlags;
    if ( Com_IsMenuLevel(0) )
        return -5;
    if ( g_reloading && g_reloading->current.integer )
    {
        Com_Printf(15, "savegame request ignored\n");
        return -1;
    }
    now = Sys_Milliseconds();
    if ( now - s_pendingLastSaveTime < 3000 )
    {
        if ( !commitNow )
            Com_PrintWarning(15, "SAVEGAME Warning: Save frequency too high. Save %s ignored\n", filename);
        return -3;
    }
    SV_ResetPendingLastSaveTime();
    if ( !loadGame )
    {
        if ( s_pendingSaveSameFrame )
        {
            Com_PrintWarning(15, "Warning: Multiple Autosaves attempted in same frame. Save %s ignored\n", filename);
            return -2;
        }
        s_pendingSaveSameFrame = true;
    }
    if ( s_pendingSaveCount >= PENDING_SAVES_LIMIT )
    {
        Com_PrintWarning(15, "Warning: Pending Saves limit exceeded. Save %s ignored\n", filename);
        return -4;
    }
    pendingSave = &s_pendingSaves[s_pendingSaveCount++];
    I_strncpyz(pendingSave->filename, filename, sizeof(pendingSave->filename));
    I_strncpyz(pendingSave->description, description, sizeof(pendingSave->description));
    I_strncpyz(pendingSave->screenshot, screenshot, sizeof(pendingSave->screenshot));
    pendingSave->loadGame = loadGame;
    pendingSave->saveFlags = saveFlags;
    pendingSave->commitNow = commitNow;
    pendingSave->callback = callback;
    pendingSave->callbackData = 0;
    pendingSave->saveId = SaveMemory_GenerateSaveId();
    return (int)pendingSave->saveId;
}

// Decomp: CoDSP_rdBlackOps.map.c:6728330
void __cdecl SV_ProcessPendingSaves()
{
    SaveGame *saveHandle;
    int processed;

    if ( !s_pendingSaveCount )
        return;
    if ( g_reloading && g_reloading->current.integer )
    {
        Com_Printf(15, "savegame request ignored\n");
        return;
    }
    saveHandle = SaveMemory_GetSaveHandle(0);
    if ( !SaveMemory_IsAvailable(saveHandle) )
        return;
    do
    {
        processed = SV_ProcessPendingSave(&s_pendingSaves[0]);
        if ( s_pendingSaveCount > 1 )
            memmove(&s_pendingSaves[0], &s_pendingSaves[1], sizeof(PendingSave) * (s_pendingSaveCount - 1));
        --s_pendingSaveCount;
    }
    while ( processed && s_pendingSaveCount > 0 );
    if ( !s_pendingSaveCount )
        s_pendingSaveSameFrame = false;
}
