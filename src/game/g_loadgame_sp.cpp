#include "g_save.h"
#include "g_scr_main.h"

#include <game/g_cmds_sp.h>
#include <game/g_main_sp.h>
#include <qcommon/cmd.h>
#include <qcommon/common.h>
#include <server/sv_game.h>
#include <server_sp/sv_init_sp.h>
#include <universal/dvar.h>
#include <universal/q_shared.h>
#include <string.h>

// Decomp: CoDSP_rdBlackOps.map.c — loadgame / loadgame_continue server commands

extern const dvar_t *g_reloading;
extern const dvar_t *sv_mapname;

static char s_loadGamePath[256];

const char *__cdecl G_GetPendingLoadGamePath()
{
    return s_loadGamePath[0] ? s_loadGamePath : NULL;
}

void __cdecl G_ClearPendingLoadGamePath()
{
    s_loadGamePath[0] = 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (SV_LoadGame_f)
void __cdecl SV_LoadGame_f()
{
    SaveGame *save;
    char savePath[256];
    char mapName[64];

    if ( SV_Cmd_Argc() < 2 )
    {
        Com_PrintWarning(15, "usage: loadgame <savepath>\n");
        return;
    }
    I_strncpyz(savePath, SV_Cmd_Argv(1), sizeof(savePath));
    I_strncpyz(s_loadGamePath, savePath, sizeof(s_loadGamePath));

    save = SaveMemory_GetSaveHandle(0);
    if ( !SaveMemory_IsAvailable(save) )
        SaveMemory_InitializeSaveSystem();

    if ( !SaveMemory_ReadLoadFromDevice(save, 0) )
    {
        Com_PrintWarning(15, "loadgame: could not read save [%s]\n", savePath);
        return;
    }

    I_strncpyz(mapName, save->mapName, sizeof(mapName));
    if ( !mapName[0] && sv_mapname )
        I_strncpyz(mapName, sv_mapname->current.string, sizeof(mapName));

    if ( !mapName[0] )
    {
        Com_PrintWarning(15, "loadgame: save has no map name\n");
        return;
    }

    level.loading = LOADING_SAVEGAME;
    Dvar_SetInt((dvar_s *)g_reloading, 4);
    SV_SpawnServer(0, mapName, 0, 1);
}

// Decomp: CoDSP_rdBlackOps.map.c (SV_LoadGameContinue_f)
void __cdecl SV_LoadGameContinue_f()
{
    SaveGame *save;

    if ( GScr_IsForceLevelEnd() )
        return;

    save = SaveMemory_GetSaveHandle(0);
    if ( !SaveMemory_IsAvailable(save) )
    {
        Com_PrintWarning(15, "loadgame_continue: save system unavailable\n");
        return;
    }

    if ( !SaveMemory_IsLoading(save) )
    {
        if ( !SaveMemory_ReadLoadFromDevice(save, 0) )
        {
            Com_PrintWarning(15, "loadgame_continue: no save data to restore\n");
            if ( g_reloading )
                Dvar_SetInt((dvar_s *)g_reloading, 0);
            return;
        }
    }

    level.loading = LOADING_SAVEGAME;
    if ( G_LoadGame(save) )
    {
        Com_Printf(15, "loadgame_continue: save restored\n");
        Scr_SaveRestored(&g_entities[0]);
    }
    else
        Com_PrintWarning(15, "loadgame_continue: G_LoadGame failed\n");

    if ( g_reloading )
        Dvar_SetInt((dvar_s *)g_reloading, 0);
}
