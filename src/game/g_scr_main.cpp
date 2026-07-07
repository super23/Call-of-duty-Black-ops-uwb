#include "g_main_sp.h"
#include "g_level_tail_sp.h"
#include "g_scr_main.h"
#include <clientscript/cscr_vm.h>
#include <clientscript/cscr_animtree.h>
#include <cgame/cg_scr_main.h>
#include "g_spawn_sp.h"
#include "g_load_utils.h"
#include <clientscript/cscr_stringlist.h>
#include <clientscript/cscr_variable.h>
#include <clientscript/cscr_main.h>
#include <universal/com_memory.h>
#include <client/con_channels.h>
#include <server/sv_game.h>
#include <server_sp/sv_main_sp.h>
#include <server_sp/sv_init_sp.h>
#include <server_sp/sv_main_pc_sp.h>
#include <client_sp/sv_client_sp.h>
#include <client/cl_debugdata.h>
#include <cgame/cg_drawtools.h>
#include <game/g_debug.h>
#include <game/g_weapon.h>
#include <xanim/xmodel_utils.h>
#include <bgame/bg_weapons_ammo.h>
#include <game/g_utils_wrapper.h>
#include <clientscript/scr_const.h>
#include <server/sv_world.h>
#include <game/turret.h>
#include <game/g_scr_helicopter.h>
#include <game/g_vehicle_path.h>
#include <qcommon/dobj_management.h>
#include <xanim/dobj_utils.h>
#include <game/actor_script_cmd.h>
#include <game/actor_sp.h>
#include <game/actor_threat.h>
#include <game/actor_event_listeners.h>
#include "g_combat_sp.h"
#include "g_active_sp.h"
#include <game/g_mover.h>
#include <bgame/bg_perks.h>
#include <demo/demo_recording.h>
#include <record/recorder.h>
#include <qcommon/cm_world.h>
#include <bgame/bg_weapons_load_obj.h>
#include "g_client_script_cmd_sp.h"
#include <universal/surfaceflags.h>
#include <universal/com_math_anglevectors.h>
#include <universal/com_math.h>
#include <cstring>
#include <bgame/bg_misc.h>
#include <bgame/bg_animation.h>
#include <database/db_assetnames.h>
#include <database/db_registry.h>
#include <bgame/bg_pmove.h>
#include <clientscript/cscr_variable.h>
#include <clientscript/cscr_debugger.h>
#include <game/pregame_wrapper.h>
#include <live/live_contracts.h>
#include <live/live_storage_win.h>
#include <live/live_counter.h>
#include <live/live_stats.h>
#include <ui_sp/ui_gametype_variants_sp.h>
#include <glass/glass_server.h>
#include <game/g_missile.h>
#include <game/actor_spawner.h>
#include <game/actor_animapi.h>
#include <gfx_d3d/r_reflection_probe.h>
#include "g_cmds_sp.h"
#include <client_sp/g_client_sp.h>
#include <universal/com_files.h>
#include <universal/q_parse.h>
#include <DW/MatchRecorder.h>
#include <client/splitscreen.h>
#include <bgame/bg_unlockable_items.h>
#include <bgame/bg_collectibles_sp.h>
#include <live/live_win.h>
#include <gfx_d3d/r_fog.h>
#include <game/bullet.h>
#include <cgame/cg_event.h>
#include <game/g_client_fields.h>
#include <game/g_scr_mover.h>
#include <game/actor_wrapper.h>
#include <win32/win_shared.h>
#include <qcommon/cm_load.h>
#include <game/g_targets.h>
#include <client/cl_main.h>
#include <stringed/stringed_hooks.h>
#include <gfx_d3d/r_dpvs.h>
#include <sound/snd_bank.h>
#include <gfx_d3d/r_primarylights.h>
#include <game/g_save.h>
#include <game/g_misc_sp.h>
#include <game/g_scr_vehicle.h>
#include <game/actor_corpse.h>
#include <game/g_animscripted.h>
#include <game/actor_state.h>
#include <game/sentient.h>
#include <game/g_spawnsystem_sp.h>
#include <ui/ui_main.h>
#include <cgame_sp/cg_ents_sp.h>
#include <qcommon/com_bsp.h>
#include <cmath>
#include "actor_badplace.h"
#include "sentient_script_cmd.h"
#include "pathnode.h"
#include "g_bsp.h"
#include "actor_aim.h"
#include "actor_sp.h"
#include <gfx_d3d/r_cinematic.h>
#include <universal/CurveManager.h>
#include <cgame/cg_draw_debug.h>
#include <physics/rope.h>
#include <gfx_d3d/r_stream.h>
#include <physics/physconstraints_load_obj.h>
#include <physics/destructible.h>
#include <client/cl_keys.h>
#include <qcommon/common.h>
#include <qcommon/cmd.h>
#include <qcommon/com_clients.h>
#include <universal/com_shared.h>
#include <EffectsCore/fx_system.h>

scr_data_t g_scr_data;

float g_mapSunLight[3];
float g_mapSunDirection[3];
int g_scr_numRestarts;

static scr_animtree_t g_gameGenericHumanTree;

// Decomp: CoDSP_rdBlackOps.map.c (dword_8392316C / dword_83923170)
int g_scriptAiLimit;
bool g_disableGrenadeSuicide;

// Decomp: CoDSP_rdBlackOps.map.c (dword_839A27BC / dword_839A27B8)
static int g_spawnTrackCount;
static int g_messageTrackSize;

static const dvar_s *missionGlobals[8];
static bool s_missionGlobalsRegistered;

static void GScr_RegisterMissionGlobals()
{
    char name[32];
    int i;

    if ( s_missionGlobalsRegistered )
        return;
    s_missionGlobalsRegistered = true;

    BG_Collectibles_RegisterDvars();
    for ( i = 0; i < 8; ++i )
    {
        Com_sprintf(name, sizeof(name), "mg%d", i);
        missionGlobals[i] = _Dvar_RegisterInt(
            name,
            0,
            0,
            255,
            0x4001u,
            "Used by script for keeping track of mission globals");
    }
}

static TurretInfo *Scr_GetTurretInfo(gentity_s *ent);

// Decomp: CoDSP_rdBlackOps.map.c (assertCmd)
void assertCmd()
{
    if ( !Scr_GetInt(0, SCRIPTINSTANCE_CLIENT) )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "assert fail", 1);
}

// Decomp: CoDSP_rdBlackOps.map.c (assertexCmd)
void assertexCmd()
{
    if ( !Scr_GetInt(0, SCRIPTINSTANCE_CLIENT) )
    {
        const char *message = Scr_GetString(1u, SCRIPTINSTANCE_CLIENT);
        Scr_Error(SCRIPTINSTANCE_CLIENT, va("assert fail: %s", message), 1);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (assertmsgCmd)
void assertmsgCmd()
{
    const char *message = Scr_GetString(0, SCRIPTINSTANCE_CLIENT);
    Scr_Error(SCRIPTINSTANCE_CLIENT, va("assert fail: %s", message), 1);
}

// Decomp: CoDSP_rdBlackOps.map.c (print)
void print()
{
    const int paramCount = Scr_GetNumParam(SCRIPTINSTANCE_CLIENT);

    for ( int paramIndex = 0; paramIndex < paramCount; ++paramIndex )
    {
        const char *debugString = Scr_GetDebugString(paramIndex, SCRIPTINSTANCE_CLIENT);
        Com_Printf(cg_level.scriptPrintChannel, "%s", debugString);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (println)
void println()
{
    print();
    Com_Printf(cg_level.scriptPrintChannel, "\n");
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsCollectors)
void GScr_IsCollectors()
{
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_ParamError(0, "Usage : IsCollectors [player]", SCRIPTINSTANCE_SERVER);
    if ( !Scr_GetEntity(0) )
        Scr_ParamError(0, "IsCollectors Error: param 1 is not an entity.", SCRIPTINSTANCE_SERVER);
    Scr_AddBool(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c:9071
unsigned int __cdecl    GScr_AllocString(const char *s)
{
    return Scr_AllocString(s, 1, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_LoadLevel @ 825F0348)
// Retail runs the gametype "init" handle (gametypescript, loaded non-fatally in
// GScr_LoadCallbackScripts) BEFORE the level "main". With default ui_gametype="" this
// is a no-op for SP campaign; it matters for gametypes that ship maps/gametypes/<name>.gsc.
void __cdecl Scr_LoadLevel()
{
    unsigned __int16 t;

    if ( g_scr_data.gametypescript )
    {
        t = Scr_ExecThread(SCRIPTINSTANCE_SERVER, g_scr_data.gametypescript, 0);
        Scr_FreeThread(t, SCRIPTINSTANCE_SERVER);
    }
    if ( g_scr_data.levelscript )
    {
        t = Scr_ExecThread(SCRIPTINSTANCE_SERVER, g_scr_data.levelscript, 0);
        Scr_FreeThread(t, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSPBlackOps.map.c:3938005
static int GScr_LoadScriptAndLabelFunctions(
    scriptInstance_t inst,
    const char *filename,
    const char *label,
    ScriptFunctions *functions,
    int bEnforceExists)
{
    int func;

    if ( !g_loadScripts || !g_loadScripts->current.enabled )
    {
        // Retail still compiles animscripts when g_loadScripts is off so #using_animtree registers trees.
        if ( !I_strncmp(filename, "animscripts/", 12) )
            Scr_LoadScript(inst, (char *)filename);
        if ( functions->count < functions->maxSize )
            functions->address[functions->count++] = 0;
        return 0;
    }
    if ( functions->count >= functions->maxSize )
    {
        Com_PrintError(15, "CODE ERROR: GScr_LoadScriptAndLabel: functions->maxSize exceeded\n");
        return 0;
    }
    if ( !Scr_LoadScript(inst, (char *)filename) )
    {
        functions->address[functions->count++] = 0;
        Com_Printf(15, "Could not find script '%s'\n", filename);
        if ( bEnforceExists )
            Com_Error(ERR_DROP, "Could not find script '%s'", filename);
        return 0;
    }
    func = Scr_GetFunctionHandle(inst, filename, label);
    functions->address[functions->count++] = func;
    if ( !func )
    {
        Com_Printf(15, "Could not find label '%s' in script '%s'\n", label, filename);
        if ( bEnforceExists )
            Com_Error(ERR_DROP, "Could not find label '%s' in script '%s'", label, filename);
    }
    return func;
}

// Decomp: CoDSPBlackOps.map.c:3939903
static int GScr_SetScriptAndLabel(
    ScriptFunctions *functions,
    const char *filename,
    const char *label,
    int bEnforceExists)
{
    int func;

    if ( !g_loadScripts || !g_loadScripts->current.enabled )
        return 0;
    if ( functions->count >= functions->maxSize )
        Com_Error(ERR_DROP, "CODE ERROR: GScr_SetScriptAndLabel: functions->maxSize exceeded");
    func = functions->address[functions->count++];
    if ( !func && bEnforceExists )
        Com_Error(ERR_DROP, "Could not find label '%s' in script '%s'", label, filename);
    return func;
}

// Decomp: CoDSP_rdBlackOps.map.c:9059
int __cdecl GScr_LoadScriptAndLabel(scriptInstance_t inst, const char *filename, const char *label, int bEnforceExists)
{
    int func;

    if ( !g_loadScripts || !g_loadScripts->current.enabled )
        return 0;
    if ( !Scr_LoadScript(inst, (char*)filename) && bEnforceExists )
        Com_Error(ERR_DROP, "Could not find script '%s'", filename);
    func = Scr_GetFunctionHandle(inst, filename, label);
    if ( !func )
    {
        if ( bEnforceExists )
            Com_Error(ERR_DROP, "Could not find label '%s' in script '%s'", label, filename);
    }
    return func;
}

static int GScr_LoadCallbackFromSetup(scriptInstance_t inst, const char *label)
{
    int func;

    if ( !g_loadScripts || !g_loadScripts->current.enabled )
        return 0;
    // Decomp: CoDSP_rdBlackOps.map.c — verified code_post_gfx.ff (Unlinker): maps/_callbacksetup.gsc.
    // Zombies use the same SP callback setup; maps/mp/gametypes/_callbacksetup is common_mp.ff (MP only).
    if ( !Scr_LoadScript(inst, (char *)"maps/_callbacksetup") )
        Com_Error(ERR_DROP, "Could not find script 'maps/_callbacksetup'");
    func = Scr_GetFunctionHandle(inst, "maps/_callbacksetup", label);
    if ( !func )
        Com_Error(ERR_DROP, "Could not find label '%s' in script '%s'", label, "maps/_callbacksetup");
    return func;
}

// Decomp: CoDSPBlackOps.map.c:3938909
void __cdecl GScr_LoadCallbackScripts(scriptInstance_t inst)
{
    scr_gametype_data_t *gametype;

    if ( !g_loadScripts || !g_loadScripts->current.enabled )
        return;

    gametype = &g_scr_data.gametype;
    g_scr_data.saverestored = GScr_LoadCallbackFromSetup(inst, "CodeCallback_SaveRestored");
    gametype->startupgametype = GScr_LoadCallbackFromSetup(inst, "CodeCallback_StartGameType");
    gametype->playerconnect = GScr_LoadCallbackFromSetup(inst, "CodeCallback_PlayerConnect");
    gametype->playerdisconnect = GScr_LoadCallbackFromSetup(inst, "CodeCallback_PlayerDisconnect");
    gametype->actordamage = GScr_LoadCallbackFromSetup(inst, "CodeCallback_ActorDamage");
    gametype->playerdamage = GScr_LoadCallbackFromSetup(inst, "CodeCallback_PlayerDamage");
    gametype->playerkilled = GScr_LoadCallbackFromSetup(inst, "CodeCallback_PlayerKilled");
    gametype->actorkilled = GScr_LoadCallbackFromSetup(inst, "CodeCallback_ActorKilled");
    gametype->playerrevive = GScr_LoadCallbackFromSetup(inst, "CodeCallback_PlayerRevive");
    gametype->playerlaststand = GScr_LoadCallbackFromSetup(inst, "CodeCallback_PlayerLastStand");
    g_scr_data.levelnotify = GScr_LoadCallbackFromSetup(inst, "CodeCallback_LevelNotify");
    gametype->vehicledamage = GScr_LoadCallbackFromSetup(inst, "CodeCallback_VehicleDamage");
    g_scr_data.actorshouldreact = GScr_LoadCallbackFromSetup(inst, "CodeCallback_ActorShouldReact");
    g_scr_data.faceeventnotify = GScr_LoadCallbackFromSetup(inst, "CodeCallback_FaceEventNotify");
    g_scr_data.disconnectedduringload = GScr_LoadCallbackFromSetup(inst, "CodeCallback_DisconnectedDuringLoad");

    // Decomp: CoDSP_rdBlackOps.map.c — verified code_post_gfx.ff (Unlinker): maps/_destructible.gsc (not maps/mp/_destructible).
    if ( !Scr_LoadScript(inst, (char *)"maps/_destructible") )
        Com_Error(ERR_DROP, "Could not find script 'maps/_destructible'");
    g_scr_data.destructible_callback = Scr_GetFunctionHandle(
        inst,
        "maps/_destructible",
        "CodeCallback_DestructibleEvent");
    if ( !g_scr_data.destructible_callback )
        Com_Error(ERR_DROP, "Could not find label '%s' in script '%s'", "CodeCallback_DestructibleEvent", "maps/_destructible");

    if ( ui_gametype && ui_gametype->current.string[0] )
    {
        char filename[68];

        Com_sprintf(filename, sizeof(filename), "maps/gametypes/%s", ui_gametype->current.string);
        if ( Scr_LoadScript(inst, filename) )
            g_scr_data.gametypescript = Scr_GetFunctionHandle(inst, filename, "init");
    }

    g_scr_data.menumessage = GScr_LoadCallbackFromSetup(inst, "CodeCallback_MenuMessage");
    g_scr_data.dec20message = GScr_LoadCallbackFromSetup(inst, "CodeCallback_Dec20Message");
    g_scr_data.glassSmash = GScr_LoadCallbackFromSetup(inst, "CodeCallback_GlassSmash");
}

// Decomp: CoDSPBlackOps.map.c:3938091
static void GScr_LoadSingleAnimScriptFunctions(
    scriptInstance_t inst,
    scr_animscript_t *pAnim,
    const char *name,
    ScriptFunctions *functions)
{
    char filename[64];

    iassert(pAnim);
    iassert(name);
    Com_sprintf(filename, sizeof(filename), "animscripts/%s", name);
    pAnim->name = GScr_AllocString(name);
    if ( !g_loadScripts || !g_loadScripts->current.enabled )
    {
        Scr_LoadScript(inst, filename);
        if ( functions->count < functions->maxSize )
            functions->address[functions->count++] = 0;
        pAnim->func = 0;
        return;
    }
    pAnim->func = GScr_LoadScriptAndLabelFunctions(inst, filename, "main", functions, 1);
}

// Decomp: CoDSPBlackOps.map.c:3938156
void __cdecl GScr_LoadAnimScripts(scriptInstance_t inst, ScriptFunctions *functions)
{
    AnimScriptList *animList;

    animList = &g_scr_data.humanAnim;
    GScr_LoadSingleAnimScriptFunctions(inst, &animList->combat, "combat", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &animList->concealment_crouch, "concealment_crouch", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &animList->concealment_prone, "concealment_prone", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &animList->concealment_stand, "concealment_stand", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &animList->cover_arrival, "cover_arrival", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &animList->cover_crouch, "cover_crouch", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &animList->cover_left, "cover_left", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &animList->cover_pillar, "cover_pillar", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &animList->cover_prone, "cover_prone", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &animList->cover_right, "cover_right", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &animList->cover_stand, "cover_stand", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &animList->cover_wide_left, "cover_wide_left", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &animList->cover_wide_right, "cover_wide_right", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &animList->death, "death", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &animList->grenade_return_throw, "grenade_return_throw", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &animList->init, "init", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &animList->pain, "pain", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &animList->react, "react", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &animList->move, "move", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &animList->scripted, "scripted", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &animList->stop, "stop", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &animList->grenade_cower, "grenade_cower", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &animList->flashed, "flashed", functions);
    GScr_LoadScriptAndLabelFunctions(inst, "animscripts/scripted", "init", functions, 1);
    GScr_LoadScriptAndLabelFunctions(inst, "animscripts/init_mode_sp", "init", functions, 1);
}

// Decomp: CoDSPBlackOps.map.c:3938394
void __cdecl GScr_LoadZombieAnimScripts(scriptInstance_t inst, ScriptFunctions *functions)
{
    GScr_LoadSingleAnimScriptFunctions(inst, &g_scr_data.dogAnim.combat, "zombie_combat", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &g_scr_data.dogAnim.death, "zombie_death", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &g_scr_data.dogAnim.init, "zombie_init", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &g_scr_data.dogAnim.pain, "zombie_pain", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &g_scr_data.dogAnim.move, "zombie_move", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &g_scr_data.dogAnim.scripted, "zombie_scripted", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &g_scr_data.dogAnim.stop, "zombie_stop", functions);
    GScr_LoadScriptAndLabelFunctions(inst, "animscripts/zombie_scripted", "init", functions, 1);
}

// Decomp: CoDSPBlackOps.map.c:3938450
void __cdecl GScr_LoadZombieDogAnimScripts(scriptInstance_t inst, ScriptFunctions *functions)
{
    GScr_LoadSingleAnimScriptFunctions(inst, &g_scr_data.zombieDogAnim.combat, "zombie_dog_combat", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &g_scr_data.zombieDogAnim.death, "zombie_dog_death", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &g_scr_data.zombieDogAnim.init, "zombie_dog_init", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &g_scr_data.zombieDogAnim.pain, "zombie_dog_pain", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &g_scr_data.zombieDogAnim.move, "zombie_dog_move", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &g_scr_data.zombieDogAnim.scripted, "zombie_dog_scripted", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &g_scr_data.zombieDogAnim.stop, "zombie_dog_stop", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &g_scr_data.zombieDogAnim.flashed, "zombie_dog_flashed", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &g_scr_data.zombieDogAnim.turn, "zombie_dog_turn", functions);
#ifdef KISAK_SP
    g_scr_data.init_dog_animations = GScr_LoadScriptAndLabel(
        inst,
        "animscripts/zombie_dog_init",
        "initDogAnimations",
        0);
#endif
}

// Decomp: CoDSPBlackOps.map.c:3939963 / CoDSP_rdBlackOps.map.c:9070
int GScr_LoadLevelScript(scriptInstance_t inst, const char *mapname, ScriptFunctions *functions)
{
    char filename[64];

    if ( I_strnicmp(mapname, "mp/", 3) && I_strnicmp(mapname, "mp\\", 3) )
    {
        // Decomp: CoDSP_rdBlackOps.map.c GScr_LoadLevelScript @ 825F0250
        Com_sprintf(filename, sizeof(filename), "maps/%s", mapname);
        return GScr_LoadScriptAndLabelFunctions(inst, filename, "main", functions, 0);
    }
    return 0;
}

static bool GScr_HasLoadedActorType(const char actorTypes[][64], int actorTypeCount, const char *actorType)
{
    for ( int i = 0; i < actorTypeCount; ++i )
    {
        if ( !I_stricmp(actorTypes[i], actorType) )
            return true;
    }
    return false;
}

static bool GScr_TryAddActorType(char actorTypes[][64], int *actorTypeCount, const char *actorType)
{
    if ( *actorTypeCount >= 32 || GScr_HasLoadedActorType(actorTypes, *actorTypeCount, actorType) )
        return false;
    I_strncpyz(actorTypes[*actorTypeCount], actorType, 64);
    ++*actorTypeCount;
    return true;
}

static bool GScr_HasLoadedTraverseAnimscript(const char animscripts[][64], int animscriptCount, const char *animscript)
{
    for ( int i = 0; i < animscriptCount; ++i )
    {
        if ( !I_stricmp(animscripts[i], animscript) )
            return true;
    }
    return false;
}

static bool GScr_TryAddTraverseAnimscript(char animscripts[][64], int *animscriptCount, const char *animscript)
{
    if ( *animscriptCount >= 32 || GScr_HasLoadedTraverseAnimscript(animscripts, *animscriptCount, animscript) )
        return false;
    I_strncpyz(animscripts[*animscriptCount], animscript, 64);
    ++*animscriptCount;
    return true;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_LoadDogAnimScripts @ 825EF750) — entity load phase only.
static void GScr_LoadDogAnimScriptsFunctions(scriptInstance_t inst, ScriptFunctions *functions)
{
    GScr_LoadSingleAnimScriptFunctions(inst, &g_scr_data.dogAnim.combat, "dog_combat", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &g_scr_data.dogAnim.death, "dog_death", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &g_scr_data.dogAnim.init, "dog_init", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &g_scr_data.dogAnim.pain, "dog_pain", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &g_scr_data.dogAnim.move, "dog_move", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &g_scr_data.dogAnim.scripted, "dog_scripted", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &g_scr_data.dogAnim.stop, "dog_stop", functions);
    GScr_LoadSingleAnimScriptFunctions(inst, &g_scr_data.dogAnim.flashed, "dog_flashed", functions);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_LoadScriptsForEntities @ 825EF3C8)
void __cdecl GScr_LoadScriptsForEntities(scriptInstance_t inst, ScriptFunctions *functions)
{
    SpawnVar spawnVar;
    const char *classname;
    const char *weaponinfo;
    char actorTypes[32][64];
    int actorTypeCount;
    bool loadedDogAnims;
    char filename[64];
    char actorType[64];

    actorTypeCount = 0;
    loadedDogAnims = false;
    if ( !G_ParseSpawnVars(&spawnVar) )
        Com_Error(ERR_DROP, "GScr_LoadScriptsForEntities: no entities to parse");
    while ( G_ParseSpawnVars(&spawnVar) )
    {
        if ( !G_SpawnString(&spawnVar, "classname", "", &classname) )
            continue;
        if ( I_strnicmp(classname, "actor_", 6) )
        {
            if ( !I_stricmp(classname, "misc_mg42") || !I_stricmp(classname, "misc_turret") )
            {
                if ( G_SpawnString(&spawnVar, "weaponinfo", "", &weaponinfo) )
                {
                    const int weaponIndex = G_GetWeaponIndexForName((char *)weaponinfo);
                    if ( weaponIndex )
                    {
                        const WeaponDef *weaponDef = BG_GetWeaponDef(weaponIndex);
                        if ( weaponDef->weapClass == WEAPCLASS_TURRET )
                        {
                            if ( weaponDef->szScript && weaponDef->szScript[0] )
                            {
                                GScr_LoadSingleAnimScriptFunctions(
                                    inst,
                                    &g_scr_data.dogAnim.weapons[weaponIndex],
                                    weaponDef->szScript,
                                    functions);
                            }
                        }
                        else
                        {
                            Com_PrintError(
                                24,
                                "ERROR: weapClass in weapon info '%s' for %s must be 'turret'\n",
                                weaponinfo,
                                classname);
                        }
                    }
                    else
                    {
                        Com_PrintError(
                            24,
                            "ERROR: could not find weapon info '%s' for %s\n",
                            weaponinfo,
                            classname);
                    }
                }
            }
            continue;
        }

        I_strncpyz(actorType, classname + 6, sizeof(actorType));
        if ( !GScr_TryAddActorType(actorTypes, &actorTypeCount, actorType) )
            continue;

        if ( !zombiemode->current.enabled && !loadedDogAnims )
        {
            if ( I_stristr(actorType, "dog") || I_stristr(actorType, "hound") )
            {
                loadedDogAnims = true;
                GScr_LoadDogAnimScriptsFunctions(inst, functions);
            }
        }

        Com_sprintf(filename, sizeof(filename), "aitype/%s", actorType);
        GScr_LoadScriptAndLabelFunctions(inst, filename, "main", functions, 1);
        GScr_LoadScriptAndLabelFunctions(inst, filename, "precache", functions, 1);
        GScr_LoadScriptAndLabelFunctions(inst, filename, "spawner", functions, 1);
    }
    G_ResetEntityParsePoint();
}

struct GScr_PathnodeScriptLoadContext
{
    ScriptFunctions *functions;
    char traverseAnimscripts[32][64];
    int traverseAnimscriptCount;
};

static void GScr_LoadScriptsForPathNodeWithContext(
    scriptInstance_t inst,
    pathnode_t *loadNode,
    GScr_PathnodeScriptLoadContext *ctx)
{
    char *animscript;
    char filename[68];

    if ( G_ExitAfterToolComplete() || !loadNode->constant.type )
        return;

    if ( loadNode->constant.type == NODE_NEGOTIATION_BEGIN )
    {
        if ( !loadNode->constant.animscript )
        {
            Com_PrintError(
                1,
                "ERROR: Pathnode (%s) at (%g %g %g) has no animscript specified\n",
                nodeStringTable[loadNode->constant.type],
                loadNode->constant.vOrigin[0],
                loadNode->constant.vOrigin[1],
                loadNode->constant.vOrigin[2]);
            loadNode->constant.type = NODE_BADNODE;
            return;
        }

        animscript = SL_ConvertToString(loadNode->constant.animscript, inst);
        if ( !animscript )
            return;

        if ( !GScr_TryAddTraverseAnimscript(ctx->traverseAnimscripts, &ctx->traverseAnimscriptCount, animscript) )
            return;

        Com_sprintf(filename, sizeof(filename), "animscripts/traverse/%s", animscript);
        loadNode->constant.animscriptfunc = GScr_LoadScriptAndLabelFunctions(
            inst,
            filename,
            "main",
            ctx->functions,
            1);
        if ( !loadNode->constant.animscriptfunc )
        {
            Com_PrintError(
                1,
                "ERROR: Pathnode (%s) at (%g %g %g) cannot find animscript '%s'\n",
                nodeStringTable[loadNode->constant.type],
                loadNode->constant.vOrigin[0],
                loadNode->constant.vOrigin[1],
                loadNode->constant.vOrigin[2],
                animscript);
            loadNode->constant.type = NODE_BADNODE;
        }
    }
    else if ( loadNode->constant.animscript )
    {
        iassert(!loadNode->constant.animscript);
    }
}

static void GScr_LoadScriptsForPathNodeCallback(scriptInstance_t inst, pathnode_t *loadNode, void *userData)
{
    GScr_LoadScriptsForPathNodeWithContext(inst, loadNode, (GScr_PathnodeScriptLoadContext *)userData);
}

// Decomp: CoDSPBlackOps.map.c:3938683
static void GScr_LoadScriptsForPathNodes(scriptInstance_t inst, ScriptFunctions *functions)
{
    GScr_PathnodeScriptLoadContext ctx;

    if ( !G_ExitAfterToolComplete() )
    {
        ctx.functions = functions;
        ctx.traverseAnimscriptCount = 0;
        Path_CallFunctionForNodes(inst, GScr_LoadScriptsForPathNodeCallback, &ctx);
    }
}

// Decomp: CoDSPBlackOps.map.c:3938516
void __cdecl GScr_LoadScriptsForPathNode(scriptInstance_t inst, pathnode_t *loadNode, ScriptFunctions *functions)
{
    GScr_PathnodeScriptLoadContext ctx;

    ctx.functions = functions;
    ctx.traverseAnimscriptCount = 0;
    GScr_LoadScriptsForPathNodeWithContext(inst, loadNode, &ctx);
}

// Decomp: CoDSPBlackOps.map.c:3940263
static void GScr_SetSingleAnimScript(
    ScriptFunctions *functions,
    scr_animscript_t *pAnim,
    const char *name)
{
    char filename[64];

    iassert(pAnim);
    iassert(name);
    Com_sprintf(filename, sizeof(filename), "animscripts/%s", name);
    pAnim->func = GScr_SetScriptAndLabel(functions, filename, "main", 1);
    pAnim->name = GScr_AllocString(name);
}

// Decomp: CoDSPBlackOps.map.c:3940372
static void GScr_SetAnimScripts(ScriptFunctions *functions)
{
    AnimScriptList *animList;

    animList = &g_scr_data.humanAnim;
    GScr_SetSingleAnimScript(functions, &animList->combat, "combat");
    GScr_SetSingleAnimScript(functions, &animList->concealment_crouch, "concealment_crouch");
    GScr_SetSingleAnimScript(functions, &animList->concealment_prone, "concealment_prone");
    GScr_SetSingleAnimScript(functions, &animList->concealment_stand, "concealment_stand");
    GScr_SetSingleAnimScript(functions, &animList->cover_arrival, "cover_arrival");
    GScr_SetSingleAnimScript(functions, &animList->cover_crouch, "cover_crouch");
    GScr_SetSingleAnimScript(functions, &animList->cover_left, "cover_left");
    GScr_SetSingleAnimScript(functions, &animList->cover_pillar, "cover_pillar");
    GScr_SetSingleAnimScript(functions, &animList->cover_prone, "cover_prone");
    GScr_SetSingleAnimScript(functions, &animList->cover_right, "cover_right");
    GScr_SetSingleAnimScript(functions, &animList->cover_stand, "cover_stand");
    GScr_SetSingleAnimScript(functions, &animList->cover_wide_left, "cover_wide_left");
    GScr_SetSingleAnimScript(functions, &animList->cover_wide_right, "cover_wide_right");
    GScr_SetSingleAnimScript(functions, &animList->death, "death");
    GScr_SetSingleAnimScript(functions, &animList->grenade_return_throw, "grenade_return_throw");
    GScr_SetSingleAnimScript(functions, &animList->init, "init");
    GScr_SetSingleAnimScript(functions, &animList->pain, "pain");
    GScr_SetSingleAnimScript(functions, &animList->react, "react");
    GScr_SetSingleAnimScript(functions, &animList->move, "move");
    GScr_SetSingleAnimScript(functions, &animList->scripted, "scripted");
    GScr_SetSingleAnimScript(functions, &animList->stop, "stop");
    GScr_SetSingleAnimScript(functions, &animList->grenade_cower, "grenade_cower");
    GScr_SetSingleAnimScript(functions, &animList->flashed, "flashed");
    g_scr_data.scripted_init = GScr_SetScriptAndLabel(functions, "animscripts/scripted", "init", 1);
    g_scr_data.init_mode_sp = GScr_SetScriptAndLabel(functions, "animscripts/init_mode_sp", "init", 1);
    g_animScriptTable[AI_SPECIES_HUMAN] = &g_scr_data.humanAnim;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetDogAnimScripts @ 825F1140)
static void GScr_SetDogAnimScripts(ScriptFunctions *functions)
{
    GScr_SetSingleAnimScript(functions, &g_scr_data.dogAnim.combat, "dog_combat");
    GScr_SetSingleAnimScript(functions, &g_scr_data.dogAnim.death, "dog_death");
    GScr_SetSingleAnimScript(functions, &g_scr_data.dogAnim.init, "dog_init");
    GScr_SetSingleAnimScript(functions, &g_scr_data.dogAnim.pain, "dog_pain");
    GScr_SetSingleAnimScript(functions, &g_scr_data.dogAnim.move, "dog_move");
    GScr_SetSingleAnimScript(functions, &g_scr_data.dogAnim.scripted, "dog_scripted");
    GScr_SetSingleAnimScript(functions, &g_scr_data.dogAnim.stop, "dog_stop");
    GScr_SetSingleAnimScript(functions, &g_scr_data.dogAnim.flashed, "dog_flashed");
    g_animScriptTable[AI_SPECIES_DOG] = &g_scr_data.dogAnim;
}

// Decomp: CoDSPBlackOps.map.c:3940618
static void GScr_SetZombieDogAnimScripts(ScriptFunctions *functions)
{
    GScr_SetSingleAnimScript(functions, &g_scr_data.zombieDogAnim.combat, "zombie_dog_combat");
    GScr_SetSingleAnimScript(functions, &g_scr_data.zombieDogAnim.death, "zombie_dog_death");
    GScr_SetSingleAnimScript(functions, &g_scr_data.zombieDogAnim.init, "zombie_dog_init");
    GScr_SetSingleAnimScript(functions, &g_scr_data.zombieDogAnim.pain, "zombie_dog_pain");
    GScr_SetSingleAnimScript(functions, &g_scr_data.zombieDogAnim.move, "zombie_dog_move");
    GScr_SetSingleAnimScript(functions, &g_scr_data.zombieDogAnim.scripted, "zombie_dog_scripted");
    GScr_SetSingleAnimScript(functions, &g_scr_data.zombieDogAnim.stop, "zombie_dog_stop");
    GScr_SetSingleAnimScript(functions, &g_scr_data.zombieDogAnim.flashed, "zombie_dog_flashed");
    GScr_SetSingleAnimScript(functions, &g_scr_data.zombieDogAnim.turn, "zombie_dog_turn");
    g_animScriptTable[AI_SPECIES_DOG] = &g_scr_data.zombieDogAnim;
    g_animScriptTable[AI_SPECIES_ZOMBIE_DOG] = &g_scr_data.zombieDogAnim;
}

// Decomp: CoDSPBlackOps.map.c:3940561
static void GScr_SetZombieAnimScripts(ScriptFunctions *functions)
{
    GScr_SetSingleAnimScript(functions, &g_scr_data.dogAnim.combat, "zombie_combat");
    GScr_SetSingleAnimScript(functions, &g_scr_data.dogAnim.death, "zombie_death");
    GScr_SetSingleAnimScript(functions, &g_scr_data.dogAnim.init, "zombie_init");
    GScr_SetSingleAnimScript(functions, &g_scr_data.dogAnim.pain, "zombie_pain");
    GScr_SetSingleAnimScript(functions, &g_scr_data.dogAnim.move, "zombie_move");
    GScr_SetSingleAnimScript(functions, &g_scr_data.dogAnim.scripted, "zombie_scripted");
    GScr_SetSingleAnimScript(functions, &g_scr_data.dogAnim.stop, "zombie_stop");
    GScr_SetScriptAndLabel(functions, "animscripts/zombie_scripted", "init", 1);
    g_animScriptTable[AI_SPECIES_ZOMBIE] = &g_scr_data.dogAnim;
}

// Decomp: CoDSPBlackOps.map.c:3939963
static void GScr_SetLevelScript(ScriptFunctions *functions)
{
    const char *mapname;
    char filename[64];

    mapname = Dvar_GetString("mapname");
    if ( I_strnicmp(mapname, "mp/", 3) && I_strnicmp(mapname, "mp\\", 3) )
    {
        Com_sprintf(filename, sizeof(filename), "maps/%s", mapname);
        g_scr_data.levelscript = GScr_SetScriptAndLabel(functions, filename, "main", 0);
    }
    else
    {
        g_scr_data.levelscript = 0;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (SP_turret_XAnimPrecache @ 8268EA18)
static void SP_turret_XAnimPrecache(ScriptFunctions *functions, SpawnVar *spawnVar, const char *classname)
{
    const char *weaponinfo;
    int weaponIndex;
    const WeaponDef *weaponDef;

    if ( !G_SpawnString(spawnVar, "weaponinfo", "", &weaponinfo) )
        return;
    weaponIndex = G_GetWeaponIndexForName((char *)weaponinfo);
    if ( !weaponIndex )
        Com_Error(ERR_DROP, "ERROR: could not find weapon info '%s' for %s", weaponinfo, classname);
    weaponDef = BG_GetWeaponDef(weaponIndex);
    if ( weaponDef->weapClass != WEAPCLASS_TURRET )
        Com_Error(ERR_DROP, "ERROR: weapClass in weapon info '%s' for %s must be 'turret'", weaponinfo, classname);
    if ( weaponDef->szScript && weaponDef->szScript[0] )
        GScr_SetSingleAnimScript(functions, &g_scr_data.dogAnim.weapons[weaponIndex], weaponDef->szScript);
    else
    {
        g_scr_data.dogAnim.weapons[weaponIndex].func = 0;
        g_scr_data.dogAnim.weapons[weaponIndex].name = 0;
    }
}

// Decomp: CoDSPBlackOps.map.c:3985391
static void GScr_SetScriptsAndAnimsForEntities(ScriptFunctions *functions)
{
    SpawnVar spawnVar;
    const char *classname;
    bool loadedDogAnims;
    char filename[64];
    char actorType[64];
    AITypeScript *typeScript;

    loadedDogAnims = false;
    if ( !G_ParseSpawnVars(&spawnVar) )
        Com_Error(ERR_DROP, "GScr_SetScriptsAndAnimsForEntities: no entities to parse");
    while ( G_ParseSpawnVars(&spawnVar) )
    {
        if ( !G_SpawnString(&spawnVar, "classname", "", &classname) )
            continue;
        if ( I_strnicmp(classname, "actor_", 6) )
        {
            if ( !I_stricmp(classname, "misc_mg42") || !I_stricmp(classname, "misc_turret") )
                SP_turret_XAnimPrecache(functions, &spawnVar, classname);
            continue;
        }
        I_strncpyz(actorType, classname + 6, sizeof(actorType));
        if ( Hunk_FindDataForFile(0, actorType) )
            continue;
        if ( !zombiemode->current.enabled && !loadedDogAnims )
        {
            if ( I_stristr(actorType, "dog") || I_stristr(actorType, "hound") )
            {
                loadedDogAnims = true;
                GScr_SetDogAnimScripts(functions);
            }
        }
        typeScript = (AITypeScript *)Hunk_AllocLow(sizeof(AITypeScript), "AITypeScript", 5);
        Com_sprintf(filename, sizeof(filename), "aitype/%s", actorType);
        typeScript->main = GScr_SetScriptAndLabel(functions, filename, "main", 1);
        typeScript->precache = GScr_SetScriptAndLabel(functions, filename, "precache", 1);
        typeScript->spawner = GScr_SetScriptAndLabel(functions, filename, "spawner", 1);
        Hunk_SetDataForFile(0, actorType, typeScript, (void *(__cdecl *)(int))GScr_AnimscriptAlloc);
    }
    G_ResetEntityParsePoint();
}

// Decomp: CoDSPBlackOps.map.c:3939473
void __cdecl GScr_LoadScripts(scriptInstance_t inst, const char *mapname, ScriptFunctions *functions)
{
    Scr_BeginLoadScripts(inst, 1);

    // Decomp: CoDSP_rdBlackOps.map.c (GScr_LoadScripts @ 825EFD10) — always invoked; g_loadScripts
    // is checked inside GScr_LoadScriptAndLabel, except animscripts still compile for animtrees.
    GScr_LoadScriptAndLabelFunctions(inst, "codescripts/delete", "main", functions, 1);
    GScr_LoadScriptAndLabelFunctions(inst, "codescripts/struct", "initstructs", functions, 1);
    GScr_LoadScriptAndLabelFunctions(inst, "codescripts/struct", "createstruct", functions, 1);
    GScr_LoadScriptAndLabelFunctions(inst, "codescripts/struct", "findstruct", functions, 1);
    GScr_LoadCallbackScripts(inst);

    if ( zombiemode->current.enabled )
    {
        GScr_LoadZombieAnimScripts(inst, functions);
        GScr_LoadZombieDogAnimScripts(inst, functions);
    }
    else
    {
        GScr_LoadAnimScripts(inst, functions);
        GScr_LoadDogAnimScriptsFunctions(inst, functions);
    }

    GScr_LoadLevelScript(inst, mapname, functions);
    GScr_LoadScriptsForEntities(inst, functions);
    GScr_LoadScriptsForPathNodes(inst, functions);
    Scr_PostCompileScripts(inst);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_PostLoadScripts @ 825F14C8)
// EXTERNAL: exported as YAXXZ from g_scr_main.obj but is an internal init callback
// (GScr_LoadScriptsAndAnims), not a script-callable builtin — do not add to functions[].
// Decomp: CoDSPBlackOps.map.c:3985538
void __cdecl GScr_PostLoadScripts(scriptInstance_t inst, ScriptFunctions *functions)
{
    signed int classnum;

    if ( g_loadScripts && g_loadScripts->current.enabled )
    {
        g_scr_data.delete_ = GScr_SetScriptAndLabel(functions, "codescripts/delete", "main", 1);
        g_scr_data.initstructs = GScr_SetScriptAndLabel(functions, "codescripts/struct", "initstructs", 1);
        g_scr_data.createstruct = GScr_SetScriptAndLabel(functions, "codescripts/struct", "createstruct", 1);
        g_scr_data.findstruct = GScr_SetScriptAndLabel(functions, "codescripts/struct", "findstruct", 1);

        if ( zombiemode->current.enabled )
        {
            GScr_SetZombieAnimScripts(functions);
            GScr_SetZombieDogAnimScripts(functions);
        }
        else
        {
            GScr_SetAnimScripts(functions);
            GScr_SetDogAnimScripts(functions);
        }

        GScr_SetLevelScript(functions);
        GScr_SetScriptsAndAnimsForEntities(functions);
        GScr_SetScriptsForPathNodes(functions);
    }

    for ( classnum = 0; classnum < 5; ++classnum )
        Scr_SetClassMap(inst, classnum);
    GScr_AddFieldsForEntity();
    GScr_AddFieldsForHudElems();
    GScr_AddFieldsForPathnode();
    GScr_AddFieldsForVehicleNode();
    GScr_AddFieldsForRadiant();
    Scr_EndLoadScripts(inst);
}

// Decomp: CoDSPBlackOps.map.c:3904672
void __cdecl GScr_LoadScriptsAndAnims(scriptInstance_t inst)
{
    int address[128];
    ScriptFunctions functions;
    const char *mapname;

    functions.maxSize = 128;
    functions.count = 0;
    functions.address = address;
    mapname = Dvar_GetString("mapname");

    GScr_RegisterMissionGlobals();
    GScr_LoadScripts(inst, mapname, &functions);
    functions.maxSize = functions.count;
    functions.count = 0;
    GScr_PostLoadScripts(inst, &functions);
    if ( g_loadScripts && g_loadScripts->current.enabled && functions.maxSize != functions.count )
        Com_Error(ERR_DROP, "Script function count mismatch");

    BG_LoadAnim(mapname);
    GScr_FindAnimTrees(inst);
    BG_PostLoadAnim(mapname);
    G_LoadAnimTreeInstances();
}

// Decomp: CoDSPBlackOps.map.c:3940226
void __cdecl GScr_FindAnimTrees(scriptInstance_t inst)
{
    g_gameGenericHumanTree = Scr_FindAnimTree(inst, "generic_human");
    if ( !g_gameGenericHumanTree.anims )
        Com_Error(ERR_DROP, "Could not find animtree 'generic_human'");
}

// Decomp: CoDSP_rdBlackOps.map.c:9062
void __cdecl GScr_LoadDogAnimScripts(scriptInstance_t inst)
{
    GScr_LoadSingleAnimScript(inst, &g_scr_data.dogAnim.combat, "dog_combat");
    GScr_LoadSingleAnimScript(inst, &g_scr_data.dogAnim.death, "dog_death");
    GScr_LoadSingleAnimScript(inst, &g_scr_data.dogAnim.init, "dog_init");
    GScr_LoadSingleAnimScript(inst, &g_scr_data.dogAnim.pain, "dog_pain");
    GScr_LoadSingleAnimScript(inst, &g_scr_data.dogAnim.move, "dog_move");
    GScr_LoadSingleAnimScript(inst, &g_scr_data.dogAnim.stop, "dog_stop");
    GScr_LoadSingleAnimScript(inst, &g_scr_data.dogAnim.flashed, "dog_flashed");
    GScr_LoadSingleAnimScript(inst, &g_scr_data.dogAnim.jump, "dog_jump");
    GScr_LoadSingleAnimScript(inst, &g_scr_data.dogAnim.turn, "dog_turn");
    g_animScriptTable[AI_SPECIES_DOG] = &g_scr_data.dogAnim;
}

// Decomp: CoDSP_rdBlackOps.map.c:9061
void __cdecl GScr_LoadSingleAnimScript(scriptInstance_t inst, scr_animscript_t *pAnim, const char *name)
{
    char filename[64];

    iassert(pAnim);
    iassert(name);

    Com_sprintf(filename, sizeof(filename), "animscripts/%s", name);

    pAnim->name = GScr_AllocString(name);
    if ( !g_loadScripts || !g_loadScripts->current.enabled )
    {
        Scr_LoadScript(inst, filename);
        pAnim->func = 0;
        return;
    }
    pAnim->func = GScr_LoadScriptAndLabel(inst, filename, "main", 1);
}

// Decomp: CoDSP_rdBlackOps.map.c:9081
void GScr_SetScriptsForPathNodes(ScriptFunctions *functions)
{
    if ( !G_ExitAfterToolComplete() )
        Path_CallFunctionForNodes(SCRIPTINSTANCE_SERVER, (PathNodeCallback_t)GScr_SetScriptsForPathNode, functions);
}

// Decomp: CoDSP_rdBlackOps.map.c:9082 @ 825F0890 — functions must be consumed via GScr_SetScriptAndLabel during PostLoad.
void __cdecl GScr_SetScriptsForPathNode(scriptInstance_t inst, pathnode_t *loadNode, ScriptFunctions *functions)
{
    char *animscript;
    char filename[68];

    if ( !G_ExitAfterToolComplete() && loadNode->constant.type )
    {
        if ( loadNode->constant.type == NODE_NEGOTIATION_BEGIN )
        {
            if ( loadNode->constant.animscript )
            {
                animscript = SL_ConvertToString(loadNode->constant.animscript, SCRIPTINSTANCE_SERVER);
                if ( !animscript
                    && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                                328,
                                0,
                                "%s",
                                "animscript") )
                {
                    __debugbreak();
                }
                loadNode->constant.animscriptfunc = (int)Hunk_FindDataForFile(1, animscript);
                if ( !loadNode->constant.animscriptfunc )
                {
                    Com_sprintf(filename, 0x40u, "animscripts/traverse/%s", animscript);
                    if ( functions )
                        loadNode->constant.animscriptfunc = GScr_SetScriptAndLabel(functions, filename, "main", 1);
                    else
                        loadNode->constant.animscriptfunc = GScr_LoadScriptAndLabel(inst, filename, "main", 1);
                    Hunk_SetDataForFile(
                        1,
                        animscript,
                        (void *)loadNode->constant.animscriptfunc,
                        (void *(__cdecl *)(int))GScr_AnimscriptAlloc);
                }
                if ( !loadNode->constant.animscriptfunc )
                {
                    Com_PrintError(
                        1,
                        "ERROR: Pathnode (%s) at (%g %g %g) cannot find animscript '%s'\n",
                        nodeStringTable[loadNode->constant.type],
                        loadNode->constant.vOrigin[0],
                        loadNode->constant.vOrigin[1],
                        loadNode->constant.vOrigin[2],
                        animscript);
                    loadNode->constant.type = NODE_BADNODE;
                }
            }
            else
            {
                Com_PrintError(
                    1,
                    "ERROR: Pathnode (%s) at (%g %g %g) has no animscript specified\n",
                    nodeStringTable[loadNode->constant.type],
                    loadNode->constant.vOrigin[0],
                    loadNode->constant.vOrigin[1],
                    loadNode->constant.vOrigin[2]);
                loadNode->constant.type = NODE_BADNODE;
            }
        }
        else if ( loadNode->constant.animscript )
        {
            if ( !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                            316,
                            0,
                            "%s",
                            "!loadNode->constant.animscript") )
                __debugbreak();
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c:9083
unsigned __int8 *__cdecl GScr_AnimscriptAlloc(unsigned int size)
{
    return Hunk_AllocLow(size, "GScr_AnimscriptAlloc", 5);
}

// Decomp: CoDSP - pregame not used in SP campaign flow
void __cdecl Scr_LoadPreGame()
{
}

// NOT CALLED IN SP. Retail CoDSP GScr_LoadScripts has no gametype-main loader; this exists only
// for parity with the MP API (g_scr_main_mp.cpp). Do NOT re-add the call to GScr_LoadScripts:
// the SP campaign has no maps/gametypes/solo.gsc, so enforcing it ERR_DROPs the level load.
void __cdecl GScr_LoadGameTypeScript()
{
    char filename[68];
    const char *gametype;

    if ( !g_loadScripts || !g_loadScripts->current.enabled )
        return;
    if ( !g_gametype || !g_gametype->current.string[0] )
        return;

    gametype = g_gametype->current.string;
    Com_sprintf(filename, sizeof(filename), "maps/gametypes/%s", gametype);
    // Decomp: verified code_post_gfx.ff (Unlinker) — retail has maps/gametypes/zom.txt only; zombie logic is maps/_zombiemode* in common_zombie.ff.
    g_scr_data.gametype.main = GScr_LoadScriptAndLabel(
        SCRIPTINSTANCE_SERVER,
        filename,
        "main",
        !(zombiemode && zombiemode->current.enabled));
    if ( !g_scr_data.gametype.main && zombiemode && zombiemode->current.enabled )
        Com_DPrintf(17, "SP zombie: no maps/gametypes/%s — using maps/_zombiemode* framework\n", gametype);
}

// Decomp: CoDSP - no MP-style pregame script in SP
int GScr_LoadPreGameScript()
{
    return 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_FreeScripts)
void __cdecl GScr_FreeScripts(scriptInstance_t inst)
{
    signed int classnum;

    for ( classnum = 0; classnum < 5; ++classnum )
        Scr_RemoveClassMap(inst, classnum);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_GetClanId)
void __cdecl ScrCmd_GetClanId(scr_entref_t entref)
{
    Scr_AddString("0", SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_GetClanName)
void __cdecl ScrCmd_GetClanName(scr_entref_t entref)
{
    Scr_AddString((char *)"", SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_CreatePrintChannel)
void GScr_CreatePrintChannel()
{
    char *name;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("illegal call to createprintchannel()", 0);
    name = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( !Con_OpenChannel(name, 1) )
        Scr_Error("Unable to create new channel.    Maximum number of channels exeeded.", 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_printChannelSet)
void GScr_printChannelSet()
{
    int channel = 25;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
    {
        Scr_Error("illegal call to setprintchannel()", 0);
        return;
    }
    const int previousChannel = level.scriptPrintChannel;
    const int paramType = Scr_GetType(0, SCRIPTINSTANCE_SERVER);
    if ( paramType == 2 )
    {
        const char *channelName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        if ( !Con_GetChannel(channelName, &channel) )
        {
            Scr_ParamError(0, "Invalid Print Channel", SCRIPTINSTANCE_SERVER);
            return;
        }
    }
    else if ( paramType != 6 || (channel = Scr_GetInt(0, SCRIPTINSTANCE_SERVER), !Con_IsChannelOpen(channel)) )
    {
        Scr_ParamError(0, "Invalid Print Channel", SCRIPTINSTANCE_SERVER);
        return;
    }
    if ( Con_ScriptHasPermission(channel) )
    {
        level.scriptPrintChannel = channel;
        Scr_AddInt(previousChannel, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_ParamError(0, "Script does not have permission to print to this channel", SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_LocalizationError)
void __cdecl Scr_LocalizationError(unsigned int iParm, const char *pszErrorMessage)
{
    Scr_ParamError(iParm, pszErrorMessage, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_ConstructMessageString)
void __cdecl Scr_ConstructMessageString(
                int firstParmIndex,
                int lastParmIndex,
                const char *errorContext,
                char *string,
                unsigned int stringLimit)
{
    const char *fmtMsg;
    const char *displayName;
    const char *fmtMsg2;
    const char *fmtMsg3;
    const char *fmtMsg4;
    const char *fmtMsg5;
    unsigned int tokenLength;
    unsigned int displayNameTokenLength;
    unsigned int charIndex;
    unsigned int charAlt;
    unsigned int tokenLen;
    int type;
    gentity_s *ent;
    char *token;
    unsigned int stringLen;

    stringLen = 0;
    while ( firstParmIndex <= lastParmIndex )
    {
        type = Scr_GetType(firstParmIndex, SCRIPTINSTANCE_SERVER);
        if ( type == 3 )
        {
            token = Scr_GetIString(firstParmIndex, SCRIPTINSTANCE_SERVER);
            tokenLen = strlen(token);
            Scr_ValidateLocalizedStringRef(firstParmIndex, token, tokenLen);
            if ( stringLen + tokenLen + 1 >= stringLimit )
            {
                fmtMsg = va("%s is too long. Max length is %i\n", errorContext, stringLimit);
                Scr_ParamError(firstParmIndex, fmtMsg, SCRIPTINSTANCE_SERVER);
            }
            if ( stringLen )
                string[stringLen++] = 20;
        }
        else if ( type == 1 && Scr_GetPointerType(firstParmIndex, SCRIPTINSTANCE_SERVER) == 19 )
        {
            ent = Scr_GetEntity(firstParmIndex);
            if ( !ent->client )
                Scr_ParamError(firstParmIndex, "Entity is not a player", SCRIPTINSTANCE_SERVER);
            displayName = CS_DisplayName(&ent->client->sess.cs, 3);
            token = va("%s^7", displayName);
            displayNameTokenLength = strlen(token);
            tokenLen = displayNameTokenLength;
            if ( stringLen + displayNameTokenLength + 1 >= stringLimit )
            {
                fmtMsg2 = va("%s is too long. Max length is %i\n", errorContext, stringLimit);
                Scr_ParamError(firstParmIndex, fmtMsg2, SCRIPTINSTANCE_SERVER);
            }
            if ( displayNameTokenLength )
                string[stringLen++] = 21;
        }
        else
        {
            token = Scr_GetString(firstParmIndex, SCRIPTINSTANCE_SERVER);
            tokenLength = strlen(token);
            tokenLen = tokenLength;
            for ( charIndex = 0; charIndex < tokenLength; ++charIndex )
            {
                if ( token[charIndex] == 20 || token[charIndex] == 21 || token[charIndex] == 22 )
                {
                    fmtMsg3 = va("bad escape character (%i) present in string", token[charIndex]);
                    Scr_ParamError(firstParmIndex, fmtMsg3, SCRIPTINSTANCE_SERVER);
                }
                if ( isalpha(token[charIndex]) )
                {
                    if ( loc_warnings->current.enabled )
                    {
                        if ( loc_warningsAsErrors->current.enabled )
                        {
                            fmtMsg4 = va("non-localized %s strings are not allowed to have letters in them: \"%s\"", errorContext, token);
                            Scr_LocalizationError(firstParmIndex, fmtMsg4);
                        }
                        else
                        {
                            Com_PrintWarning(
                                17,
                                "WARNING: Non-localized %s string is not allowed to have letters in it. Must be changed over to a localiz"
                                "ed string: \"%s\"\n",
                                errorContext,
                                token);
                        }
                    }
                    break;
                }
            }
            if ( stringLen + tokenLength + 1 >= stringLimit )
            {
                fmtMsg5 = va("%s is too long. Max length is %i\n", errorContext, stringLimit);
                Scr_ParamError(firstParmIndex, fmtMsg5, SCRIPTINSTANCE_SERVER);
            }
            if ( tokenLength )
                string[stringLen++] = 21;
        }
        for ( charAlt = 0; charAlt < tokenLen; ++charAlt )
        {
            if ( token[charAlt] == 20 || token[charAlt] == 21 || token[charAlt] == 22 )
                string[stringLen] = 46;
            else
                string[stringLen] = token[charAlt];
            ++stringLen;
        }
        ++firstParmIndex;
    }
    string[stringLen] = 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_ValidateLocalizedStringRef)
void __cdecl Scr_ValidateLocalizedStringRef(unsigned int parmIndex, const char *token, int tokenLen)
{
    const char *fmtMsg;
    int charIter;

    if ( !token
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 627, 0, "%s", "token") )
    {
        __debugbreak();
    }
    if ( tokenLen < 0
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 628, 0, "%s", "tokenLen >= 0") )
    {
        __debugbreak();
    }
    if ( tokenLen > 1 )
    {
        for ( charIter = 0; charIter < tokenLen; ++charIter )
        {
            if ( !isalnum(token[charIter]) && token[charIter] != 95 )
            {
                fmtMsg = va(
                             "Illegal localized string reference: %s must contain only alpha-numeric characters and underscores",
                             token);
                Scr_ParamError(parmIndex, fmtMsg, SCRIPTINSTANCE_SERVER);
            }
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_ValidateNonLocalizedStringRef @ 825F1E90)
void __cdecl Scr_ValidateNonLocalizedStringRef(unsigned int parmIndex, const char *token, int tokenLen)
{
    const char *fmtMsg;
    int charIter;

    if ( !token
        && !Assert_MyHandler("C:\\projects\\cod\\codsrc\\src\\game\\g_scr_main.cpp", 0x2E2u, 0, "%s", "token") )
    {
        __debugbreak();
    }
    for ( charIter = 0; charIter < tokenLen; ++charIter )
    {
        if ( token[charIter] == 20 || token[charIter] == 21 || token[charIter] == 22 )
        {
            fmtMsg = va("bad escape character (%i) present in string", token[charIter]);
            Scr_ParamError(parmIndex, fmtMsg, SCRIPTINSTANCE_SERVER);
        }
        if ( isalpha(token[charIter]) )
            break;
    }
    if ( loc_warnings->current.enabled )
    {
        if ( loc_warningsAsErrors->current.enabled )
        {
            fmtMsg = va(
                "non-localized %s strings are not allowed to have letters in them: \"%s\"",
                "string",
                token);
            Scr_LocalizationError(parmIndex, fmtMsg);
        }
        else
        {
            Com_PrintWarning(
                24,
                "WARNING: Non-localized %s string is not allowed to have letters in it. Must be changed over to a localized string: \"%s\"\n",
                "string",
                token);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_NonLocalizedStringErrorPrefix @ 825F2020)
unsigned int __cdecl Scr_NonLocalizedStringErrorPrefix(
    unsigned int parmIndex,
    unsigned int stringLen,
    const char *errorContext,
    unsigned int prefixLen,
    unsigned int stringLimit,
    char *outString)
{
    const char *fmtMsg;

    if ( !loc_warnings->current.enabled )
        return prefixLen;
    if ( prefixLen + stringLen + 14 >= stringLimit )
    {
        fmtMsg = va("%s is too long. Max length is %i\n", errorContext, stringLimit);
        Scr_ParamError(parmIndex, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    if ( prefixLen + 13 >= stringLimit
        && !Assert_MyHandler(
            "C:\\projects\\cod\\codsrc\\src\\game\\g_scr_main.cpp",
            0x304u,
            0,
            "%s",
            "stringLen + LOCALIZATION_ERROR_PREFIX_LENGTH < stringLimit") )
    {
        __debugbreak();
    }
    memcpy(outString + prefixLen, "UNLOCALIZED: ", 13);
    return prefixLen + 13;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_IsValidMessageChar @ 825F2120)
bool __cdecl Scr_IsValidMessageChar(char ch)
{
    return ch >= 31 || Key_IsValidGamePadChar(ch);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_MakeGameMessage)
void __cdecl Scr_MakeGameMessage(int iClientNum, const char *pszCmd)
{
    int NumParam;
    const char *fmtMsg;
    char string[1028];

    NumParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    Scr_ConstructMessageString(0, NumParam - 1, "Game Message", string, 0x400u);
    fmtMsg = va("%s \"%s\"", pszCmd, string);
    SV_GameSendServerCommand(iClientNum, SV_CMD_CAN_IGNORE, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_VerifyWeaponIndex)
void __cdecl Scr_VerifyWeaponIndex(int weaponIndex, const char *weaponName)
{
    const char *fmtMsg;

    if ( !weaponName
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 761, 0, "%s", "weaponName") )
    {
        __debugbreak();
    }
    if ( !weaponIndex )
    {
        if ( I_stricmp("none", weaponName) )
            fmtMsg = va(
                         "Unknown weapon name \"%s\": script may need to call PreCacheItem(\"%s\") during level init.\n",
                         weaponName,
                         weaponName);
        else
            fmtMsg = va("Weapon name \"%s\" is not valid.\n", weaponName);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (iprintln)
void iprintln()
{
    const char *fmtMsg;

    fmtMsg = va("%c", 102);
    Scr_MakeGameMessage(-1, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (iprintlnbold)
void iprintlnbold()
{
    const char *fmtMsg;

    fmtMsg = va("%c", 103);
    Scr_MakeGameMessage(-1, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_print3d)
void GScr_print3d()
{
    const int numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParam < 2 || numParam > 6 )
    {
        Scr_Error("illegal call to print3d()", 0);
        return;
    }

    float origin[3];
    float rgb[3];
    float scale = 1.0f;
    float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    VariableUnion duration;
    duration.intValue = 1;

    Scr_GetVector(0, origin, SCRIPTINSTANCE_SERVER);
    const char *text = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 3 )
    {
        Scr_GetVector(2u, rgb, SCRIPTINSTANCE_SERVER);
        color[0] = rgb[0];
        color[1] = rgb[1];
        color[2] = rgb[2];
    }
    if ( numParam >= 4 )
        color[3] = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 5 )
        scale = Scr_GetFloat(4u, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 6 )
        duration.intValue = Scr_GetInt(5u, SCRIPTINSTANCE_SERVER);

    G_AddDebugString(origin, color, scale, (char *)text, duration.intValue);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_line)
void GScr_line()
{
    const int numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParam < 2 || numParam > 6 )
    {
        Scr_Error("illegal call to line()", 0);
        return;
    }

    float start[3];
    float end[3];
    float rgb[3];
    float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    int depthTest = 0;
    VariableUnion duration;
    duration.intValue = 0;

    Scr_GetVector(0, start, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, end, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 3 )
    {
        Scr_GetVector(2u, rgb, SCRIPTINSTANCE_SERVER);
        color[0] = rgb[0];
        color[1] = rgb[1];
        color[2] = rgb[2];
    }
    if ( numParam >= 4 )
        color[3] = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 5 )
        depthTest = Scr_GetInt(4u, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 6 )
        duration.intValue = Scr_GetInt(5u, SCRIPTINSTANCE_SERVER);

    CL_AddDebugLine(start, end, color, depthTest, duration.intValue);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_box)
void GScr_box()
{
    const int numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParam < 1 || numParam > 8 )
    {
        Scr_Error("illegal call to box()", 0);
        return;
    }

    float pos[3];
    float mins[3] = { -10.0f, -10.0f, -10.0f };
    float maxs[3] = { 10.0f, 10.0f, 10.0f };
    float yaw = 0.0f;
    float rgb[3];
    float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    int depthTest = 0;
    int duration = 0;

    Scr_GetVector(0, pos, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 2 )
        Scr_GetVector(1u, mins, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 3 )
        Scr_GetVector(2u, maxs, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 4 )
    {
        yaw = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
        if ( (yaw < 0.0 || yaw > 360.0)
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                        906,
                        0,
                        "%s",
                        "yaw >= 0.0f && yaw <= 360.0f") )
        {
            __debugbreak();
        }
    }
    if ( numParam >= 5 )
    {
        Scr_GetVector(4u, rgb, SCRIPTINSTANCE_SERVER);
        color[0] = rgb[0];
        color[1] = rgb[1];
        color[2] = rgb[2];
    }
    if ( numParam >= 6 )
        color[3] = Scr_GetFloat(5u, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 7 )
        depthTest = Scr_GetInt(6u, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 8 )
        duration = Scr_GetInt(7u, SCRIPTINSTANCE_SERVER);

    CG_DebugBox(pos, mins, maxs, yaw, color, depthTest, duration);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_debugstar)
void GScr_debugstar()
{
    const int numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParam < 1 || numParam > 3 )
    {
        Scr_Error("illegal call to debugstar()", 0);
        return;
    }

    float location[3];
    float rgb[3];
    float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    VariableUnion duration;
    duration.intValue = 10;

    Scr_GetVector(0, location, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 2 )
        duration.intValue = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 3 )
    {
        Scr_GetVector(2u, rgb, SCRIPTINSTANCE_SERVER);
        color[0] = rgb[0];
        color[1] = rgb[1];
        color[2] = rgb[2];
    }

    CL_AddDebugStar(location, color, duration.intValue);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_circle)
void GScr_circle()
{
    const int numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParam < 1 || numParam > 6 )
    {
        Scr_Error("illegal call to circle()", 0);
        return;
    }

    float center[3];
    float rgb[3];
    float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float radius = 10.0f;
    int depthTest = 0;
    VariableUnion onGround;
    VariableUnion duration;
    onGround.intValue = 0;
    duration.intValue = 0;

    Scr_GetVector(0, center, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 2 )
        radius = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 3 )
    {
        Scr_GetVector(2u, rgb, SCRIPTINSTANCE_SERVER);
        color[0] = rgb[0];
        color[1] = rgb[1];
        color[2] = rgb[2];
    }
    if ( numParam >= 4 )
        depthTest = Scr_GetInt(3u, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 5 )
        onGround.intValue = Scr_GetInt(4u, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 6 )
        duration.intValue = Scr_GetInt(5u, SCRIPTINSTANCE_SERVER);

    G_DebugCircle(center, radius, color, depthTest, onGround.intValue, duration.intValue);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_sphere)
void GScr_sphere()
{
    const int numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParam < 1 || numParam > 7 )
    {
        Scr_Error("illegal call to sphere()", 0);
        return;
    }

    float center[3];
    float rgb[3];
    float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float radius = 10.0f;
    int depthTest = 0;
    VariableUnion sideCount;
    VariableUnion duration;
    sideCount.intValue = 10;
    duration.intValue = 0;

    Scr_GetVector(0, center, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 2 )
        radius = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 3 )
    {
        Scr_GetVector(2u, rgb, SCRIPTINSTANCE_SERVER);
        color[0] = rgb[0];
        color[1] = rgb[1];
        color[2] = rgb[2];
    }
    if ( numParam >= 4 )
        color[3] = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 5 )
        depthTest = Scr_GetInt(4u, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 6 )
        sideCount.intValue = Scr_GetInt(5u, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 7 )
        duration.intValue = Scr_GetInt(6u, SCRIPTINSTANCE_SERVER);

    CG_DebugSphere(center, radius, color, sideCount.intValue, depthTest, duration.intValue);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetArrayValues_Vector)
int __cdecl Scr_GetArrayValues_Vector(
                unsigned int parameter_index,
                unsigned int parent_id,
                float (*vector_array)[3],
                int vector_array_size,
                const char *array_type_description)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    float *vectorDst;
    float *next;
    int script_array_size;
    signed int vector_array_index;
    VariableValueInternal *entry_value;
    int id;

    vector_array_index = 0;
    if ( !vector_array
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 1097, 0, "%s", "vector_array") )
    {
        __debugbreak();
    }
    if ( !parent_id
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 1098, 0, "%s", "parent_id") )
    {
        __debugbreak();
    }
    if ( GetObjectType(SCRIPTINSTANCE_SERVER, parent_id) != 20
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    1099,
                    0,
                    "%s",
                    "GetObjectType(SCRIPTINSTANCE_SERVER, parent_id)==VAR_ARRAY") )
    {
        __debugbreak();
    }
    script_array_size = GetArraySize(SCRIPTINSTANCE_SERVER, parent_id);
    if ( script_array_size > vector_array_size )
    {
        fmtMsg2 = va("contents of vector array are too large (must be <= %ld) (%s)", vector_array_size, array_type_description);
        Scr_ParamError(parameter_index, fmtMsg2, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        while ( vector_array_index < script_array_size )
        {
            id = GetArrayVariable(SCRIPTINSTANCE_SERVER, parent_id, vector_array_index);
            if ( !id
                && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 1106, 0, "%s", "id") )
            {
                __debugbreak();
            }
            entry_value = &gScrVarGlob[0].variableList[id + 0x8000];
            if ( (entry_value->w.status & 0x60) == 0
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                            1108,
                            0,
                            "%s",
                            "(entry_value->w.status & VAR_STAT_MASK) != VAR_STAT_FREE") )
            {
                __debugbreak();
            }
            if ( (entry_value->w.status & 0x1F) != 4 )
            {
                fmtMsg = va("contents of array must be vectors (%s)", array_type_description);
                Scr_ParamError(parameter_index, fmtMsg, SCRIPTINSTANCE_SERVER);
                return 0;
            }
            vectorDst = &(*vector_array)[3 * vector_array_index];
            next = (float *)entry_value->u.next;
            *vectorDst = *next;
            vectorDst[1] = next[1];
            vectorDst[2] = next[2];
            ++vector_array_index;
        }
    }
    return vector_array_index;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_linelist)
void GScr_linelist()
{
    const int numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParam < 1 || numParam > 5 )
    {
        Scr_Error("illegal call to linelist()", 0);
        return;
    }

    float points[256][3];
    float rgb[3];
    float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    int depthTest = 0;
    int duration = 0;

    VariableUnion pointArrayId;
    pointArrayId.intValue = Scr_GetObject(0, SCRIPTINSTANCE_SERVER);
    int pointCount = Scr_GetArrayValues_Vector(0, pointArrayId.stringValue, points, 256, "line list");
    if ( numParam >= 2 )
    {
        Scr_GetVector(1u, rgb, SCRIPTINSTANCE_SERVER);
        color[0] = rgb[0];
        color[1] = rgb[1];
        color[2] = rgb[2];
    }
    if ( numParam >= 3 )
        color[3] = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 4 )
        depthTest = Scr_GetInt(3u, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 5 )
        duration = Scr_GetInt(4u, SCRIPTINSTANCE_SERVER);

    for ( int pointIndex = 0; pointIndex < pointCount - 1; pointIndex += 2 )
        CL_AddDebugLine(points[pointIndex], points[pointIndex + 1], color, depthTest, duration);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsDefined)
void GScr_IsDefined()
{
    const int type = Scr_GetType(0, SCRIPTINSTANCE_SERVER);
    if ( type == 1 )
    {
        const int pointerType = Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER);
        if ( pointerType < 13
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                        1228,
                        0,
                        "%s",
                        "type >= FIRST_OBJECT") )
        {
            __debugbreak();
        }
        if ( pointerType >= 21 || pointerType == 18 )
            Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
        else
            Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        if ( type >= 13
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                        1233,
                        0,
                        "%s",
                        "type < FIRST_OBJECT") )
        {
            __debugbreak();
        }
        Scr_AddInt(type != 0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_AddDebugCommand)
void GScr_AddDebugCommand()
{
    char *String;

    String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    Cbuf_AddText(0, String);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsMP)
void GScr_IsMP()
{
#ifdef KISAK_SP
    Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
#else
    Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
#endif
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsFloat)
void GScr_IsFloat()
{
    int Type;

    Type = Scr_GetType(0, SCRIPTINSTANCE_SERVER);
    Scr_AddInt(Type == 5, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsInt)
void GScr_IsInt()
{
    int Type;

    Type = Scr_GetType(0, SCRIPTINSTANCE_SERVER);
    Scr_AddInt(Type == 6, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsVec)
void GScr_IsVec()
{
    int Type;

    Type = Scr_GetType(0, SCRIPTINSTANCE_SERVER);
    Scr_AddInt(Type == 4, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsString)
void GScr_IsString()
{
    int Type;

    Type = Scr_GetType(0, SCRIPTINSTANCE_SERVER);
    Scr_AddInt(Type == 2, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsArray)
void GScr_IsArray()
{
    const int type = Scr_GetType(0, SCRIPTINSTANCE_SERVER);
    if ( type == 1 )
    {
        const int pointerType = Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER);
        if ( pointerType < 13
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                        1326,
                        0,
                        "%s",
                        "type >= FIRST_OBJECT") )
        {
            __debugbreak();
        }
        Scr_AddInt(pointerType == 20, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        if ( type >= 13
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                        1331,
                        0,
                        "%s",
                        "type < FIRST_OBJECT") )
        {
            __debugbreak();
        }
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsAlive)
void GScr_IsAlive()
{
    if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) == 1
        && Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) == 19
        && Scr_GetEntity(0)->health > 0 )
    {
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetDvar)
void GScr_GetDvar()
{
    VariableUnion varUnion;
    char *dvarName;
    char *dvarValue;

    if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) == 6 )
    {
        varUnion.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        dvarValue = (char *)SV_Archived_Dvar_GetVariantString(varUnion.intValue);
    }
    else
    {
        dvarName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        dvarValue = (char *)SV_Archived_Dvar_GetVariantString(dvarName);
    }
    Scr_AddString(dvarValue, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetDvarInt)
void GScr_GetDvarInt()
{
    VariableUnion varUnion;
    const char *VariantString;
    char *dvarName;
    int dvarValue;

    if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) == 6 )
    {
        varUnion.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        VariantString = SV_Archived_Dvar_GetVariantString(varUnion.intValue);
    }
    else
    {
        dvarName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        VariantString = SV_Archived_Dvar_GetVariantString(dvarName);
    }
    dvarValue = atoi(VariantString);
    Scr_AddInt(dvarValue, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetDvarFloat)
void GScr_GetDvarFloat()
{
    VariableUnion varUnion;
    const char *VariantString;
    char *dvarName;
    float dvarValue;

    if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) == 6 )
    {
        varUnion.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        VariantString = SV_Archived_Dvar_GetVariantString(varUnion.intValue);
    }
    else
    {
        dvarName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        VariantString = SV_Archived_Dvar_GetVariantString(dvarName);
    }
    dvarValue = atof(VariantString);
    Scr_AddFloat(dvarValue, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetDvarColorRed)
void GScr_GetDvarColorRed()
{
    VariableUnion varUnion;
    char *dvarName;
    float value;

    if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) == 6 )
    {
        varUnion.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        value = Dvar_GetColorRed(varUnion.intValue);
    }
    else
    {
        dvarName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        value = Dvar_GetColorRed(dvarName);
    }
    Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetDvarColorGreen)
void GScr_GetDvarColorGreen()
{
    VariableUnion varUnion;
    char *dvarName;
    float value;

    if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) == 6 )
    {
        varUnion.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        value = Dvar_GetColorGreen(varUnion.intValue);
    }
    else
    {
        dvarName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        value = Dvar_GetColorGreen(dvarName);
    }
    Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetDvarColorBlue)
void GScr_GetDvarColorBlue()
{
    VariableUnion varUnion;
    char *dvarName;
    float value;

    if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) == 6 )
    {
        varUnion.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        value = Dvar_GetColorBlue(varUnion.intValue);
    }
    else
    {
        dvarName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        value = Dvar_GetColorBlue(dvarName);
    }
    Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetDvarColorAlpha @ 825F4368)
void GScr_GetDvarColorAlpha()
{
    VariableUnion varUnion;
    char *dvarName;
    float value;

    if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) == 6 )
    {
        varUnion.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        value = Dvar_GetColorAlpha(varUnion.intValue);
    }
    else
    {
        dvarName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        value = Dvar_GetColorAlpha(dvarName);
    }
    Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetDvar)
void GScr_SetDvar()
{
    const char *fmtMsg;
    int NumParam;
    char string[1024];
    char outString[1028];
    const char *dvarName;
    int type;
    const dvar_s *dvar;
    const char *dvarValue;

    dvarName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( !Dvar_IsValidName(dvarName) )
    {
        fmtMsg = va("Dvar %s has an invalid dvar name", dvarName);
        Scr_Error(fmtMsg, 0);
    }
    type = Scr_GetType(1u, SCRIPTINSTANCE_SERVER);
    if ( type == 3 )
    {
        NumParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
        Scr_ConstructMessageString(1, NumParam - 1, "Dvar Value", string, 0x400u);
        dvarValue = string;
    }
    else
    {
        dvarValue = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    }
    CleanDvarValue(dvarValue, outString, 1024);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) >= 3 )
        Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
    Dvar_SetFromStringByName(dvarName, outString);
    dvar = Dvar_FindVar(dvarName);
    if ( !dvar
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 1515, 0, "%s", "dvar") )
    {
        __debugbreak();
    }
    Dvar_AddFlags(dvar, 1024);
}

// Decomp: CoDSP_rdBlackOps.map.c (CleanDvarValue)
void __cdecl CleanDvarValue(const char *dvarValue, char *outString, int size)
{
    int i;

    for ( i = 0; i < size - 1 && dvarValue[i]; ++i )
    {
        *outString = I_CleanChar(dvarValue[i]);
        if ( *outString == 34 )
            *outString = 39;
        ++outString;
    }
    *outString = 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetDebugDvar)
void GScr_GetDebugDvar()
{
    const dvar_s *dvar;
    char *dvarName;

    dvarName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    dvar = Dvar_FindVar(dvarName);
    if ( !dvar )
        Scr_AddString("", SCRIPTINSTANCE_SERVER);
    else
        Scr_AddString((char *)Dvar_DisplayableValue(dvar), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetDebugDvarInt)
void GScr_GetDebugDvarInt()
{
    char *dvarName;
    const char *variantString;
    int value;

    dvarName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    variantString = Dvar_GetVariantString(dvarName);
    value = atoi(variantString);
    Scr_AddInt(value, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetDebugDvarFloat @ 825F4628)
void GScr_GetDebugDvarFloat()
{
    char *dvarName;
    const char *variantString;
    float value;

    dvarName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    variantString = Dvar_GetVariantString(dvarName);
    value = atof(variantString);
    Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetSavedDvar)
void GScr_SetSavedDvar()
{
    const char *fmtMsg;
    int NumParam;
    char string[1024];
    char outString[1028];
    const char *dvarName;
    const char *dvarValue;
    const dvar_s *dvar;

    dvarName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( Scr_GetType(1u, SCRIPTINSTANCE_SERVER) == 3 )
    {
        NumParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
        Scr_ConstructMessageString(1, NumParam - 1, "Dvar Value", string, 0x400u);
        dvarValue = string;
    }
    else
    {
        dvarValue = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    }
    CleanDvarValue(dvarValue, outString, 1024);
    if ( !Dvar_IsValidName(dvarName) )
    {
        fmtMsg = va("Dvar %s has an invalid dvar name", dvarName);
        Scr_Error(fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    dvar = Dvar_FindVar(dvarName);
    if ( !dvar )
    {
        fmtMsg = va("SetSavedDvar(): The dvar \"%s\" does not exist.", dvarName);
        Scr_Error(fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    if ( (dvar->flags & 0x1000) == 0 )
        Scr_Error("SetSavedDvar can only be called on dvars with the SAVED flag set", SCRIPTINSTANCE_SERVER);
    Dvar_SetFromStringByNameFromSource(dvarName, outString, DVAR_SOURCE_SCRIPT, 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WaterSimEnable)
void GScr_WaterSimEnable()
{
    int value;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("watersimenable() called with wrong params.\n", SCRIPTINSTANCE_SERVER);
    value = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    Dvar_SetBoolByName("r_watersim_enabled", value != 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WaterPlop @ 826072D0)
void GScr_WaterPlop()
{
    float origin[3];
    int intensity;
    float scale;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 3 )
        Scr_Error("waterplop() called with wrong params.\n", 0);
    Scr_GetVector(0, origin, SCRIPTINSTANCE_SERVER);
    intensity = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    scale = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    SV_GameSendServerCommand(
        -1,
        SV_CMD_CAN_IGNORE,
        va("%c %0.2f %0.2f %0.2f %i %0.2f", '$', origin[0], origin[1], origin[2], intensity, scale));
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetPersistentProfileVar @ 826073A8)
void GScr_SetPersistentProfileVar()
{
    unsigned int index;
    unsigned int value;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2 )
        Scr_Error("SetPersistentProfileVar() called with wrong params.\n", 0);
    index = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    value = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    if ( index >= 8 )
        Scr_Error(
            "SetPersistentProfileVar() called with wrong param range. Index must be 0 to MISSION_GLOBAL_MAX_DVARS.\n",
            0);
    if ( value >= 0x100 )
        Scr_Error("SetPersistentProfileVar() called with wrong param range. Value must be 0 to 255.\n", 0);
    GScr_RegisterMissionGlobals();
    if ( missionGlobals[index] )
        Dvar_SetInt((dvar_s *)missionGlobals[index], value);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetPersistentProfileVar @ 82607470)
void GScr_GetPersistentProfileVar()
{
    unsigned int index;
    int defaultValue;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2 )
        Scr_Error("GetPersistentProfileVar() called with wrong params.\n", 0);
    index = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    defaultValue = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    if ( index >= 8 )
        Scr_Error(
            "GetPersistentProfileVar() called with wrong param range. Index must be 0 to MISSION_GLOBAL_MAX_DVARS.\n",
            0);
    GScr_RegisterMissionGlobals();
    if ( CL_LocalClient_IsActive(0) && missionGlobals[index] )
    {
        Scr_AddInt(missionGlobals[index]->current.integer, SCRIPTINSTANCE_SERVER);
        return;
    }
    Scr_AddInt(defaultValue, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_HasCollectible @ 8261B530)
void GScr_HasCollectible()
{
    int collectibleIndex;

    collectibleIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    GScr_RegisterMissionGlobals();
    Scr_AddBool(BG_HasCollectible(collectibleIndex, 0, 0, collectibleIndex, 0, 0, 0), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetCollectible @ 8261B570)
void GScr_SetCollectible()
{
    int collectibleIndex;

    collectibleIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    GScr_RegisterMissionGlobals();
    BG_SetCollectible(collectibleIndex, 0, 0, collectibleIndex, 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_PlayBink @ 8261BF40)
void GScr_PlayBink()
{
    const char *movieName;
    int flags;

    movieName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    flags = 0;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 2 )
        flags = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    SV_GameSendServerCommand(-1, SV_CMD_CAN_IGNORE, va("%c %s %i", 'l', movieName, flags));
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsSentient)
void GScr_IsSentient()
{
    if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) == 1
        && Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) == 19
        && Scr_GetEntity(0)->sentient )
    {
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsGodMode)
void GScr_IsGodMode()
{
    gentity_s *ent;

    ent = Scr_GetEntity(0);
    if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) == 1
        && Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) == 19
        && ent->client
        && (ent->client->flags & 3) != 0 )
    {
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WeaponDualWieldWeaponName)
void GScr_WeaponDualWieldWeaponName()
{
    char *weaponName;
    char *dualWeaponName;
    unsigned int weaponIndex;
    const WeaponDef *weapDef;

    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    Scr_VerifyWeaponIndex(weaponIndex, weaponName);
    weapDef = BG_GetWeaponDef(weaponIndex);
    if ( !weapDef->bDualWield || !weapDef->dualWieldWeaponIndex )
        Scr_AddConstString(scr_const.none, SCRIPTINSTANCE_SERVER);
    else
    {
        dualWeaponName = (char *)BG_WeaponName(weapDef->dualWieldWeaponIndex);
        Scr_AddString(dualWeaponName, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WeaponIsGasWeapon)
void GScr_WeaponIsGasWeapon()
{
    char *weaponName;
    unsigned int weaponIndex;
    const WeaponDef *weapDef;

    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    if ( !weaponIndex )
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    else
    {
        weapDef = BG_GetWeaponDef(weaponIndex);
        Scr_AddInt(weapDef->weapType == WEAPTYPE_GAS, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ModelHasPhysPreset)
void GScr_ModelHasPhysPreset()
{
    char *modelName;
    XModel *model;

    modelName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    model = SV_XModelGet(modelName);
    Scr_AddInt(model && model->physPreset != 0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_NumRemoteClients) — offline SP returns 0
void GScr_NumRemoteClients()
{
    Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetTeamForEntity)
void __cdecl GScr_SetTeamForEntity(scr_entref_t entref)
{
    const char *fmtMsg;
    char *slStr;
    char *slStr2;
    char *slStr3;
    unsigned __int16 team;
    gentity_s *ent;

    ent = GetEntity(entref);
    team = (unsigned __int16)Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    if ( team == scr_const.allies )
    {
        ent->team = 2;
    }
    else if ( team == scr_const.axis )
    {
        ent->team = 1;
    }
    else if ( team == scr_const.none )
    {
        ent->team = 0;
    }
    else
    {
        slStr3 = SL_ConvertToString(scr_const.none, SCRIPTINSTANCE_SERVER);
        slStr2 = SL_ConvertToString(scr_const.axis, SCRIPTINSTANCE_SERVER);
        slStr = SL_ConvertToString(scr_const.allies, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("setteamforentity: invalid team used must be %s, %s or %s", slStr, slStr2, slStr3);
        Scr_Error(fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    if ( ent->sentient )
    {
        if ( ent->team == 2 )
            ent->sentient->eTeam = TEAM_ALLIES;
        else if ( ent->team == 1 )
            ent->sentient->eTeam = TEAM_AXIS;
        else
            ent->sentient->eTeam = TEAM_FREE;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetTime)
void GScr_GetTime()
{
    Scr_AddInt(level.time, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetDifficulty)
static const char *const difficultyStrings[] =
{
    "easy",
    "medium",
    "hard",
    "fu",
};

void GScr_GetDifficulty()
{
    int skill;

    if ( !sv_gameskill )
    {
        Scr_AddString("medium", SCRIPTINSTANCE_SERVER);
        return;
    }

    skill = sv_gameskill->current.integer;
    if ( skill < 0 || skill > 3 )
    {
        if ( !Assert_MyHandler(
                 "C:\\projects\\cod\\codsrc\\src\\game\\g_scr_main.cpp",
                 0x9B8,
                 0,
                 "%s",
                 "sv_gameskill->current.integer >= DIFFICULTY_EASY && sv_gameskill->current.integer <= DIFFICULTY_FU") )
        {
            __debugbreak();
        }
    }

    Scr_AddString(difficultyStrings[skill], SCRIPTINSTANCE_SERVER);
}

// LWSS ADD
void GScr_GetCorpseArray()
{
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        Scr_Error("cannot call getcorpsearray with parameters", 0);

    Scr_MakeArray(SCRIPTINSTANCE_SERVER);

    for ( int entIndex = 0; entIndex < level.num_entities; ++entIndex )
    {
        gentity_s *ent = &level.gentities[entIndex];
        if ( !ent->r.linked )
            continue;
        const short entityType = ent->s.eType;
        if ( entityType == ET_PLAYER_CORPSE || entityType == ET_ACTOR_CORPSE )
        {
            Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
            Scr_AddArray(SCRIPTINSTANCE_SERVER);
        }
    }
}
// LWSS END

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetPlayers)
void Scr_GetPlayers()
{
    team_t team = TEAM_ALLIES;
    bool filterByTeam = false;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        const unsigned int teamString = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);

        if ( teamString == scr_const.all )
        {
            filterByTeam = false;
        }
        else
        {
            filterByTeam = true;
            if ( teamString == scr_const.allies )
                team = TEAM_ALLIES;
            else if ( teamString == scr_const.axis )
                team = TEAM_AXIS;
            else if ( teamString == scr_const.spectator )
                team = TEAM_SPECTATOR;
            else if ( teamString == scr_const.free )
                team = TEAM_FREE;
            else if ( teamString == scr_const.localplayers )
                team = TEAM_LOCALPLAYERS;
            else
                Scr_Error(va("Unknown team %s passed to getplayers()", Scr_GetString(0, SCRIPTINSTANCE_SERVER)), SCRIPTINSTANCE_SERVER);
        }
    }

    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for ( int clientNum = 0; clientNum < com_maxclients->current.integer; ++clientNum )
    {
        gentity_s *ent = &level.gentities[clientNum];

        if ( !ent->client )
            continue;
        if ( ent->client->sess.connected != CON_CONNECTED )
            continue;
        if ( filterByTeam && ent->client->sess.cs.team != team )
            continue;

        Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
        Scr_AddArray(SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetNumExpectedPlayers @ 8261B140)
void __cdecl Scr_GetNumExpectedPlayers()
{
    int count;
    int i;

    count = 0;
    for ( i = 0; i < com_maxclients->current.integer; ++i )
    {
        if ( svs.clients[i].header.state > CS_ZOMBIE )
            ++count;
    }
    Scr_AddInt(count, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetNumConnectedPlayers @ 8261B250)
void __cdecl Scr_GetNumConnectedPlayers()
{
    int count;
    int i;
    client_t *cl;

    count = 0;
    for ( i = 0; i < com_maxclients->current.integer; ++i )
    {
        cl = &svs.clients[i];
        // PC SP: decomp misread offset+4 as gamestateMessageNum (360 CA_ACTIVE == 10).
        // Count clients fully in-world; stale check left synchronize_players() on a black hudelem forever.
        if ( cl->header.state >= CS_ACTIVE )
            ++count;
    }
    Scr_AddInt(count, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetAttachmentIndex)
void GScr_GetAttachmentIndex()
{
    eAttachment attachmentIndex;
    char *attachmentName;

    attachmentName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    attachmentIndex = BG_GetAttachmentIndex(attachmentName);
    Scr_AddInt(attachmentIndex, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetEntByNum)
void Scr_GetEntByNum()
{
    int entnum;

    entnum = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)entnum < 0x400 )
    {
        if ( g_entities[entnum].r.inuse )
            Scr_AddEntity(&g_entities[entnum], SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetTeamFlag ~6116856)
int __cdecl Scr_GetTeamFlag(const char *pszTeamName, const char *pszCaller)
{
    if ( !I_stricmp(pszTeamName, "axis") )
        return 2;
    if ( !I_stricmp(pszTeamName, "allies") )
        return 4;
    if ( !I_stricmp(pszTeamName, "neutral") )
        return 8;
    if ( !I_stricmp(pszTeamName, "all") )
        return 14;
    Scr_Error(va("unknown team '%s' in %s (should be axis, allies, or neutral)", pszTeamName, pszCaller), 0);
    return 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetTeamFlags ~6116840)
int __cdecl Scr_GetTeamFlags(unsigned int paramIndex, const char *pszCaller)
{
    int teamFlags;
    unsigned int i;

    teamFlags = 0;
    for ( i = paramIndex; i < Scr_GetNumParam(SCRIPTINSTANCE_SERVER); ++i )
        teamFlags |= Scr_GetTeamFlag(Scr_GetString(i, SCRIPTINSTANCE_SERVER), pszCaller);
    return teamFlags;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetSpecies ~6117054)
static int Scr_GetSpecies(unsigned int speciesString)
{
    int i;
    const char *speciesName;

    if ( speciesString == scr_const.all )
        return AI_SPECIES_ALL;
    for ( i = 0; i < MAX_AI_SPECIES; ++i )
    {
        if ( speciesString == *g_AISpeciesNames[i] )
            return i;
    }
    speciesName = SL_ConvertToString(speciesString, SCRIPTINSTANCE_SERVER);
    Scr_Error(va("unknown species '%s' (should be human, dog, or all)", speciesName), 0);
    return AI_SPECIES_ALL;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetAIArray ~6116799)
void Scr_GetAIArray()
{
    int teamFlags;
    actor_s *actor;

    teamFlags = Scr_GetTeamFlags(0, "getaiarray");
    if ( !teamFlags )
        teamFlags = 14;
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for ( actor = Actor_FirstActor(teamFlags); actor; actor = Actor_NextActor(actor, teamFlags) )
    {
        if ( actor->Physics.bIsAlive && !actor->delayedDeath && !G_IsSpeciesDog(actor->species) )
        {
            Scr_AddEntity(actor->ent, SCRIPTINSTANCE_SERVER);
            Scr_AddArray(SCRIPTINSTANCE_SERVER);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetAISpeciesArray ~6116991)
void Scr_GetAISpeciesArray()
{
    int teamFlags;
    int species;
    actor_s *actor;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        teamFlags = Scr_GetTeamFlag(Scr_GetString(0, SCRIPTINSTANCE_SERVER), "getaiarray");
    else
        teamFlags = 14;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 1 )
        species = AI_SPECIES_ALL;
    else
        species = Scr_GetSpecies(Scr_GetConstString(1u, SCRIPTINSTANCE_SERVER));
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for ( actor = Actor_FirstActor(teamFlags); actor; actor = Actor_NextActor(actor, teamFlags) )
    {
        if ( actor->Physics.bIsAlive && !actor->delayedDeath
            && (species == actor->species || species == AI_SPECIES_ALL) )
        {
            Scr_AddEntity(actor->ent, SCRIPTINSTANCE_SERVER);
            Scr_AddArray(SCRIPTINSTANCE_SERVER);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetSpawnerArray ~6117099)
void Scr_GetSpawnerArray()
{
    int i;
    gentity_s *ent;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        Scr_Error("cannot call getspawnerarray with parameters", 0);
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for ( i = 0; i < level.num_entities; ++i )
    {
        ent = &level.gentities[i];
        if ( ent->r.inuse && ent->s.eType == ET_ACTOR_SPAWNER )
        {
            Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
            Scr_AddArray(SCRIPTINSTANCE_SERVER);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetScriptMoverArray @ 825F5520)
void Scr_GetScriptMoverArray()
{
    int i;
    gentity_s *ent;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        Scr_Error("cannot call getscriptmoverarray with parameters", 0);
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for ( i = 0; i < level.num_entities; ++i )
    {
        ent = &level.gentities[i];
        if ( ent->r.inuse && ent->s.eType == ET_SCRIPTMOVER )
        {
            Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
            Scr_AddArray(SCRIPTINSTANCE_SERVER);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetItemArray @ 825F5608)
void Scr_GetItemArray()
{
    int i;
    gentity_s *ent;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        Scr_Error("cannot call getitemarray with parameters", 0);
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for ( i = 0; i < level.num_entities; ++i )
    {
        ent = &level.gentities[i];
        if ( ent->r.inuse && ent->s.eType == ET_ITEM )
        {
            Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
            Scr_AddArray(SCRIPTINSTANCE_SERVER);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetSpawnerTeamArray @ 825F5800)
void Scr_GetSpawnerTeamArray()
{
    int i;
    int teamFlags;
    gentity_s *ent;

    teamFlags = Scr_GetTeamFlags(0, "getspawnerteamarray");
    if ( !teamFlags )
        Scr_Error("no team was specified - use getspawnerarray instead", 0);
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for ( i = 0; i < level.num_entities; ++i )
    {
        ent = &level.gentities[i];
        if ( ent->r.inuse
            && ent->s.eType == ET_ACTOR_SPAWNER
            && (teamFlags & (1 << ent->spawner.team)) != 0 )
        {
            Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
            Scr_AddArray(SCRIPTINSTANCE_SERVER);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetNumRestarts @ 825F5028)
void Scr_GetNumRestarts()
{
    Scr_AddInt(g_scr_numRestarts, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetMapSunLight @ 82610A50)
void Scr_GetMapSunLight()
{
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        Scr_Error("Incorrect number of parameters\n", 0);
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    Scr_AddFloat(g_mapSunLight[0], SCRIPTINSTANCE_SERVER);
    Scr_AddArray(SCRIPTINSTANCE_SERVER);
    Scr_AddFloat(g_mapSunLight[1], SCRIPTINSTANCE_SERVER);
    Scr_AddArray(SCRIPTINSTANCE_SERVER);
    Scr_AddFloat(g_mapSunLight[2], SCRIPTINSTANCE_SERVER);
    Scr_AddArray(SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetMapSunDirection @ 82610CB8)
void Scr_GetMapSunDirection()
{
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        Scr_Error("Incorrect number of parameters\n", 0);
    Scr_AddVector(g_mapSunDirection, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetSunLight @ 82610AE8)
void Scr_SetSunLight()
{
    float diffuse[3];
    float specular[3];
    const char *msg;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 3 && Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 6 )
        Scr_Error("Incorrect number of parameters\n", 0);
    diffuse[0] = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    diffuse[1] = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    diffuse[2] = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 6 )
    {
        specular[0] = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
        specular[1] = Scr_GetFloat(4u, SCRIPTINSTANCE_SERVER);
        specular[2] = Scr_GetFloat(5u, SCRIPTINSTANCE_SERVER);
        msg = va("%g %g %g %g %g %g", diffuse[0], diffuse[1], diffuse[2], specular[0], specular[1], specular[2]);
    }
    else
    {
        msg = va("%g %g %g %g %g %g", diffuse[0], diffuse[1], diffuse[2], diffuse[0], diffuse[1], diffuse[2]);
    }
    SV_SetConfigstring(8, (char *)msg);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_ResetSunLight @ 82610C68)
void Scr_ResetSunLight()
{
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        Scr_Error("Incorrect number of parameters\n", 0);
    SV_SetConfigstring(8, "");
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetKeyBinding @ 825F6638)
void GScr_GetKeyBinding()
{
    char keyNames[2][128];
    const char *command;
    int localClientNum;
    int numBinds;

    command = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    localClientNum = Com_ControllerIndex_GetLocalClientNum(0);
    memset(keyNames, 0, sizeof(keyNames));
    numBinds = CL_GetKeyBinding(localClientNum, command, keyNames, 0);
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    Scr_AddString(keyNames[0], SCRIPTINSTANCE_SERVER);
    Scr_AddArrayStringIndexed(scr_const.key1, SCRIPTINSTANCE_SERVER);
    Scr_AddString(keyNames[1], SCRIPTINSTANCE_SERVER);
    Scr_AddArrayStringIndexed(scr_const.key2, SCRIPTINSTANCE_SERVER);
    Scr_AddInt(numBinds, SCRIPTINSTANCE_SERVER);
    Scr_AddArrayStringIndexed(GScr_AllocString("numBinds"), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetCommandFromKey @ 825F6708)
void GScr_GetCommandFromKey()
{
    const char *key;
    const char *command;
    int localClientNum;

    key = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    localClientNum = Com_ControllerIndex_GetLocalClientNum(0);
    command = CL_GetCommandFromKey(localClientNum, key);
    Scr_AddString(command ? command : "", SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetDumpCmdsForTimeDelta @ 825F6008) — stub until AD_* anim dump module is ported
void GScr_GetDumpCmdsForTimeDelta()
{
    (void)Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    (void)Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    Scr_AddString("", SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsAnimLeaf @ 825F6328)
void GScr_IsAnimLeaf()
{
    scr_anim_s anim;
    XAnim_s *anims;

    anim = Scr_GetAnim(0, 0, SCRIPTINSTANCE_SERVER);
    anims = Scr_GetAnims(anim.tree, SCRIPTINSTANCE_SERVER);
    Scr_AddInt(IsLeafNode(&anims->entries[anim.index]), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsAnimLooping @ 825F6388)
void GScr_IsAnimLooping()
{
    scr_anim_s anim;
    XAnim_s *anims;

    anim = Scr_GetAnim(0, 0, SCRIPTINSTANCE_SERVER);
    anims = Scr_GetAnims(anim.tree, SCRIPTINSTANCE_SERVER);
    Scr_AddInt(XAnimIsLooped(anims, anim.index), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetSnapshotIndexArray @ 8260FB58)
void Scr_GetSnapshotIndexArray()
{
    int i;
    client_t *cl;

    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for ( i = 0; i < com_maxclients->current.integer; ++i )
    {
        cl = &svs.clients[i];
        if ( cl->header.state == CS_ACTIVE )
            Scr_AddInt(cl->messageAcknowledge + 1, SCRIPTINSTANCE_SERVER);
        else
            Scr_AddUndefined(SCRIPTINSTANCE_SERVER);
        Scr_AddArray(SCRIPTINSTANCE_SERVER);
    }
    Scr_AddInt(level.time + 1000, SCRIPTINSTANCE_SERVER);
    Scr_AddArray(SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetDate @ 8261A1B0)
void GScr_GetDate()
{
    qtime_s q;
    static const char *monthNames[12] =
    {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    const char *month;

    Com_RealTime(&q, 1);
    month = (q.tm_mon >= 0 && q.tm_mon < 12) ? monthNames[q.tm_mon] : "";
    Scr_AddString(va("%s %i %i", month, q.tm_mday, q.tm_year + 1900), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetAICount @ 8261BB20)
void GScr_GetAICount()
{
    int count;
    actor_s *actor;

    count = 0;
    for ( actor = Actor_FirstActor(14); actor; actor = Actor_NextActor(actor, 14) )
    {
        if ( actor->Physics.bIsAlive )
            ++count;
    }
    Scr_AddInt(count, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetAILimit @ 8261BB08)
void GScr_GetAILimit()
{
    Scr_AddInt(g_scriptAiLimit, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_EnableGrenadeSuicide @ 8261BEB8)
void GScr_EnableGrenadeSuicide()
{
    g_disableGrenadeSuicide = false;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_flushGump @ 8261C9E8)
void GScr_flushGump()
{
    const char *arg;

    arg = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( arg && *arg )
        Cbuf_AddText(0, "flushGump\n");
}

// Decomp: CoDSP_rdBlackOps.map.c (SentientCmd_CreateThreatBiasGroup ~6239925)
void SentientCmd_CreateThreatBiasGroup()
{
    unsigned int groupName;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_ParamError(0u, "createthreatbiasgroup [name]", SCRIPTINSTANCE_SERVER);
    groupName = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    Actor_CreateThreatBiasGroup(groupName);
}

// Decomp: CoDSP_rdBlackOps.map.c (SentientCmd_GetThreatBias ~6239949) — global builtin
void GScr_GetThreatBias()
{
    scr_entref_t entref = {};

    SentientCmd_GetThreatBias(entref);
}

// Decomp: CoDSP_rdBlackOps.map.c (SentientCmd_SetThreatBias ~826869B8)
void GScr_SetThreatBias()
{
    SentientCmd_SetThreatBias();
}

// Decomp: CoDSP_rdBlackOps.map.c (SentientCmd_SetThreatBiasAgainstAll ~82686AA8)
void GScr_SetThreatBiasAgainstAll()
{
    SentientCmd_SetThreatBiasAgainstAll();
}

// Decomp: CoDSP_rdBlackOps.map.c (SentientCmd_ThreatBiasGroupExists ~82686858)
void GScr_ThreatBiasGroupExists()
{
    SentientCmd_ThreatBiasGroupExists();
}

// Decomp: CoDSP_rdBlackOps.map.c (SentientCmd_SetIgnoreMeGroup ~82686B50)
void GScr_SetIgnoreMeGroup()
{
    SentientCmd_SetIgnoreMeGroup();
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetAILimit ~6152219)
void GScr_SetAILimit()
{
    int limit;

    limit = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( limit > 32 )
        Scr_ParamError(0u, "SetAILimit must take a value between 0 and 32 inclusive.", SCRIPTINSTANCE_SERVER);
    g_scriptAiLimit = limit;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_DisableGrenadeSuicide ~6152534)
void GScr_DisableGrenadeSuicide()
{
    g_disableGrenadeSuicide = true;
}

// Decomp: CoDSP_rdBlackOps.map.c (Reset_SpawnTrackBuffer ~6118136)
void Reset_SpawnTrackBuffer()
{
    g_spawnTrackCount = 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (TrackSpawn ~6118157)
void TrackSpawn(unsigned int name, const float *origin)
{
    (void)name;
    (void)origin;
    ++g_spawnTrackCount;
}

// Decomp: CoDSP_rdBlackOps.map.c (TrackMessage ~6118483)
void TrackMessage(unsigned int messageSize)
{
    if ( g_messageTrackSize <= (int)messageSize )
        g_messageTrackSize = messageSize;
}

// Decomp: CoDSP_rdBlackOps.map.c (Reset_MessageTracker ~6118497)
void Reset_MessageTracker()
{
    g_messageTrackSize = 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_OkToSpawn ~6118504)
void GScr_OkToSpawn()
{
    if ( (unsigned int)(g_messageTrackSize + 50 * g_spawnTrackCount) >= 0x320u || g_spawnTrackCount >= 4 )
        Scr_AddBool(0, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddBool(1, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_IsNodeOccupied ~6209435)
void Scr_IsNodeOccupied()
{
    pathnode_t *node;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("illegal call to isnodeoccupied()", 0);
    node = Scr_GetPathnode(0, SCRIPTINSTANCE_SERVER);
    if ( level.time > node->dynamic.iFreeTime )
        Scr_AddBool(0, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddBool(1, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_BadPlace_Cylinder ~6141724)
void Scr_BadPlace_Cylinder()
{
    unsigned int name;
    int duration;
    int teamFlags;
    badplace_arc_t arc;

    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 5 )
        Scr_Error("Incorrect badplace_cylinder() call.\n", 0);
    if ( *Scr_GetString(0, SCRIPTINSTANCE_SERVER) )
        name = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    else
        name = 0;
    duration = (int)ceilf(Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER) * 1000.0f);
    Scr_GetVector(2u, arc.origin, SCRIPTINSTANCE_SERVER);
    arc.radius = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
    arc.halfheight = Scr_GetFloat(4u, SCRIPTINSTANCE_SERVER) * 0.5f;
    arc.angle0 = 0.0f;
    arc.angle1 = 360.0f;
    teamFlags = Scr_GetTeamFlags(5u, "badplace_cylinder");
    if ( !teamFlags )
        teamFlags = 14;
    Path_MakeArcBadPlace(name, duration, teamFlags, &arc);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_BadPlace_Arc ~6141780)
void Scr_BadPlace_Arc()
{
    unsigned int name;
    int duration;
    int teamFlags;
    float dir[3];
    float centerYaw;
    badplace_arc_t arc;

    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 8 )
        Scr_Error("Incorrect usage for badplace_arc()\n", 0);
    if ( *Scr_GetString(0, SCRIPTINSTANCE_SERVER) )
        name = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    else
        name = 0;
    duration = (int)ceilf(Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER) * 1000.0f);
    Scr_GetVector(2u, arc.origin, SCRIPTINSTANCE_SERVER);
    arc.radius = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
    arc.halfheight = Scr_GetFloat(4u, SCRIPTINSTANCE_SERVER) * 0.5f;
    Scr_GetVector(5u, dir, SCRIPTINSTANCE_SERVER);
    centerYaw = vectoyaw(dir);
    arc.angle0 = centerYaw - Scr_GetFloat(6u, SCRIPTINSTANCE_SERVER);
    if ( arc.angle0 > centerYaw )
        Scr_Error("left angle < 0 in badplace_arc\n", 0);
    arc.angle1 = centerYaw + Scr_GetFloat(7u, SCRIPTINSTANCE_SERVER);
    if ( arc.angle1 < centerYaw )
        Scr_Error("right angle < 0 in badplace_arc\n", 0);
    if ( arc.angle1 - arc.angle0 < 360.0f )
    {
        arc.angle0 = AngleNormalize360(arc.angle0);
        arc.angle1 = AngleNormalize360(arc.angle1);
    }
    else
    {
        arc.angle0 = 0.0f;
        arc.angle1 = 360.0f;
    }
    teamFlags = Scr_GetTeamFlags(8u, "badplace_arc");
    if ( !teamFlags )
        teamFlags = 14;
    Path_MakeArcBadPlace(name, duration, teamFlags, &arc);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_BadPlace_Brush ~6141910)
void Scr_BadPlace_Brush()
{
    unsigned int name;
    int duration;
    int teamFlags;
    gentity_s *brushEnt;

    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 3 )
        Scr_Error("Incorrect badplace_brush() call.\n", 0);
    if ( *Scr_GetString(0, SCRIPTINSTANCE_SERVER) )
        name = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    else
        name = 0;
    duration = (int)ceilf(Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER) * 1000.0f);
    brushEnt = Scr_GetEntity(2);
    teamFlags = Scr_GetTeamFlags(3u, "badplace_brush");
    if ( !teamFlags )
        teamFlags = 14;
    Path_MakeBrushBadPlace(name, duration, teamFlags, brushEnt);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_BadPlace_Delete ~6141711)
void Scr_BadPlace_Delete()
{
    unsigned int name;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("Incorrect number of parameters\n", 0);
    if ( !*Scr_GetString(0, SCRIPTINSTANCE_SERVER) )
        Scr_Error("badplace_delete called with name \"\"\n", 0);
    name = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    Path_RemoveBadPlace(name);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_BadPlacesEnable ~6132885)
void GScr_BadPlacesEnable()
{
    int enabled;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("BadPlacesEnable() called with wrong params.\n", 0);
    enabled = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    Dvar_SetBool((dvar_s *)ai_enableBadPlaces, enabled != 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetCinematicTimeRemaining ~6135824)
void GScr_GetCinematicTimeRemaining()
{
    Scr_AddFloat(R_Cinematic_GetTimeRemaining(), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetWeaponStowedModel)
void Scr_GetWeaponStowedModel()
{
    char *fmtMsg;
    unsigned int iWeaponIndex;
    char *pszWeaponName;
    const WeaponDef *weapDef;

    pszWeaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = G_GetWeaponIndexForName(pszWeaponName);
    weapDef = BG_GetWeaponDef(iWeaponIndex);
    if ( iWeaponIndex )
    {
        if ( *((unsigned int *)weapDef->worldModel + 1)
            && weapDef->weapClass != WEAPCLASS_GRENADE
            && weapDef->weapClass != WEAPCLASS_KILLSTREAK_ALT_STORED_WEAPON )
        {
            Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
            return;
        }
    }
    else if ( *pszWeaponName )
    {
        if ( I_stricmp(pszWeaponName, "none") )
        {
            fmtMsg = va("unknown weapon '%s' in WeaponHasStowedModel\n", pszWeaponName);
            Com_Printf(17, fmtMsg);
        }
    }
    Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetWeaponModel)
void Scr_GetWeaponModel()
{
    char *fmtMsg;
    const WeaponDef *WeaponDef;
    char *Name;
    unsigned int weaponModel;
    unsigned int iWeaponIndex;
    char *pszWeaponName;

    pszWeaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = G_GetWeaponIndexForName(pszWeaponName);
    weaponModel = 0;
    if ( iWeaponIndex )
    {
        if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 2 )
        {
            weaponModel = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
            if ( weaponModel < 0x100 )
            {
                if ( !BG_GetWeaponDef(iWeaponIndex)->worldModel[weaponModel] )
                    weaponModel = 0;
            }
            else
            {
                weaponModel = 0;
            }
        }
        WeaponDef = BG_GetWeaponDef(iWeaponIndex);
        Name = (char *)XModelGetName(WeaponDef->worldModel[weaponModel]);
        Scr_AddString(Name, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        if ( *pszWeaponName )
        {
            if ( I_stricmp(pszWeaponName, "none") )
            {
                fmtMsg = va("unknown weapon '%s' in getWeaponModel\n", pszWeaponName);
                Com_Printf(17, fmtMsg);
            }
        }
        Scr_AddString((char *)"", SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetWeaponClipModel ~825F5EB8)
// Map: CoDSP_rd.map VA 0x825f5eb8 (g_scr_main.obj)
void Scr_GetWeaponClipModel()
{
    char *fmtMsg;
    const WeaponDef *weaponDef;
    char *name;
    unsigned int weaponIndex;
    char *weaponName;

    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    if ( weaponIndex )
    {
        weaponDef = BG_GetWeaponDef(weaponIndex);
        if ( weaponDef->worldClipModel )
        {
            name = (char *)XModelGetName(weaponDef->worldClipModel);
            Scr_AddString(name, SCRIPTINSTANCE_SERVER);
            return;
        }
    }
    else if ( *weaponName )
    {
        if ( I_stricmp(weaponName, "none") )
        {
            fmtMsg = va("unknown weapon '%s' in getWeaponClipModel\n", weaponName);
            Com_Printf(17, fmtMsg);
        }
    }
    Scr_AddString((char *)"", SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetWeaponMuzzlePoint ~825FCB20)
void Scr_GetWeaponMuzzlePoint(scr_entref_t entref)
{
    gentity_s *ent;
    weaponParms wp;

    ent = GetEntity(entref);
    Weapon_SetWeaponParamsWeapon(&wp, ent->s.weapon);
    G_CalcMuzzlePoints(ent, &wp, 1);
    Scr_AddVector(wp.muzzleTrace, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetWeaponForwardDir ~825FCBA0)
void Scr_GetWeaponForwardDir(scr_entref_t entref)
{
    gentity_s *ent;
    weaponParms wp;

    ent = GetEntity(entref);
    Weapon_SetWeaponParamsWeapon(&wp, ent->s.weapon);
    G_CalcMuzzlePoints(ent, &wp, 1);
    Scr_AddVector(wp.forward, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetAmmoCount)
void __cdecl GScr_GetAmmoCount(scr_entref_t entref)
{
    int ammoCount;
    char *weaponName;
    unsigned int weaponIndex;
    gentity_s *ent;

    ent = GetPlayerEntity(entref);
    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 1630, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 1631, 0, "%s", "ent->client") )
    {
        __debugbreak();
    }
    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    if ( weaponIndex )
    {
        ammoCount = BG_WeaponAmmo(&ent->client->ps, weaponIndex);
        Scr_AddInt(ammoCount, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GetPlayerEntity)
gentity_s *__cdecl GetPlayerEntity(scr_entref_t entref)
{
    char *slStr;
    const char *fmtMsg;
    const char *slStr2;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 463, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->client )
    {
        if ( ent->targetname )
            slStr2 = SL_ConvertToString(ent->targetname, SCRIPTINSTANCE_SERVER);
        else
            slStr2 = "<undefined>";
        slStr = SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER);
        fmtMsg = va(
                     "only valid on players; called on entity %i at %.0f %.0f %.0f classname %s targetname %s\n",
                     entref.entnum,
                     ent->r.currentOrigin[0],
                     ent->r.currentOrigin[1],
                     ent->r.currentOrigin[2],
                     slStr,
                     slStr2);
        Scr_Error(fmtMsg, 0);
    }
    return ent;
}

// Decomp: CoDSP_rdBlackOps.map.c (GetEntity)
gentity_s *__cdecl GetEntity(scr_entref_t entref)
{
    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        return 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                        449,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        return &g_entities[entref.entnum];
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetAnimLength)
void GScr_GetAnimLength()
{
    float value;
    scr_anim_s anim;
    XAnim_s *anims;

    anim = Scr_GetAnim(0, 0, SCRIPTINSTANCE_SERVER);
    anims = Scr_GetAnims(anim.tree, SCRIPTINSTANCE_SERVER);
    if ( !XAnimIsPrimitive(anims, anim.index) )
        Scr_ParamError(0, "non-primitive animation has no concept of length", SCRIPTINSTANCE_SERVER);
    value = XAnimGetLength(anims, anim.index);
    Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_AnimHasNotetrack)
void GScr_AnimHasNotetrack()
{
    const XAnim_s *Anims;
    unsigned __int8 hasNotetrack;
    unsigned __int16 name;
    const char *anim;

    anim = Scr_GetAnim(0, 0, SCRIPTINSTANCE_SERVER).linkPointer;
    name = (unsigned __int16)Scr_GetConstString(1u, SCRIPTINSTANCE_SERVER);
    Anims = Scr_GetAnims(HIWORD(anim), SCRIPTINSTANCE_SERVER);
    hasNotetrack = XAnimNotetrackExists(Anims, (unsigned __int16)anim, name);
    Scr_AddBool(hasNotetrack, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetNotetrackTimes)
void GScr_GetNotetrackTimes()
{
    const XAnim_s *Anims;
    VariableUnion name;
    const char *anim;

    anim = Scr_GetAnim(0, 0, SCRIPTINSTANCE_SERVER).linkPointer;
    name.intValue = Scr_GetConstString(1u, SCRIPTINSTANCE_SERVER);
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    Anims = Scr_GetAnims(HIWORD(anim), SCRIPTINSTANCE_SERVER);
    XAnimAddNotetrackTimesToScriptArray(Anims, (unsigned __int16)anim, name.stringValue);
}

// SP: maps/_anim.gsc GetNotetracksInDelta(animation, startDelta, endDelta)
void GScr_GetNotetracksInDelta()
{
    const XAnim_s *Anims;
    scr_anim_s anim;
    float startDelta;
    float endDelta;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 3 )
        Scr_Error("wrong number of parameters", 0);
    anim = Scr_GetAnim(0, 0, SCRIPTINSTANCE_SERVER);
    startDelta = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    endDelta = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    Anims = Scr_GetAnims(anim.tree, SCRIPTINSTANCE_SERVER);
    if ( !XAnimIsPrimitive(const_cast<XAnim_s *>(Anims), anim.index) )
        Scr_ParamError(0, "non-primitive animation has no notetracks", SCRIPTINSTANCE_SERVER);
    XAnimAddNotetracksInDeltaToScriptArray(Anims, anim.index, startDelta, endDelta);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetBrushModelCenter)
void GScr_GetBrushModelCenter()
{
    gentity_s *pEnt;
    float vCenter[3];

    pEnt = Scr_GetEntity(0);
    vCenter[0] = pEnt->r.absmin[0] + pEnt->r.absmax[0];
    vCenter[1] = pEnt->r.absmin[1] + pEnt->r.absmax[1];
    vCenter[2] = pEnt->r.absmin[2] + pEnt->r.absmax[2];
    vCenter[0] = 0.5 * vCenter[0];
    vCenter[1] = 0.5 * vCenter[1];
    vCenter[2] = 0.5 * vCenter[2];
    Scr_AddVector(vCenter, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_Spawn)
void GScr_Spawn()
{
    char *slStr;
    const char *fmtMsg;
    float *currentOrigin;
    float origin[3];
    int iSpawnFlags;
    unsigned __int16 classname;
    gentity_s *ent;

    classname = (unsigned __int16)Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, origin, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 2 )
        iSpawnFlags = 0;
    else
        iSpawnFlags = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
    ent = G_Spawn();
    Scr_SetString(&ent->classname, classname, SCRIPTINSTANCE_SERVER);
    currentOrigin = ent->r.currentOrigin;
    ent->r.currentOrigin[0] = origin[0];
    currentOrigin[1] = origin[1];
    currentOrigin[2] = origin[2];
    ent->spawnflags = iSpawnFlags;
    if ( G_CallSpawnEntity(ent) )
    {
        Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        slStr = SL_ConvertToString(classname, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("unable to spawn \"%s\" entity", slStr);
        Scr_Error(fmtMsg, 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_CodeSpawn ~825F6F48)
void GScr_CodeSpawn()
{
    char *slStr;
    const char *fmtMsg;
    float *currentOrigin;
    float origin[3];
    int iSpawnFlags;
    unsigned __int16 classname;
    gentity_s *ent;
    const char *defName;

    classname = (unsigned __int16)Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, origin, SCRIPTINSTANCE_SERVER);
    if ( !BG_ValidateOrigin(origin, 18, 17, svs.mapCenter) )
        Scr_ParamError(1u, "entity", SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 2 )
        iSpawnFlags = 0;
    else
        iSpawnFlags = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
    TrackSpawn(classname, origin);
    ent = G_Spawn();
    Scr_SetString(&ent->classname, classname, SCRIPTINSTANCE_SERVER);
    currentOrigin = ent->r.currentOrigin;
    ent->r.currentOrigin[0] = origin[0];
    currentOrigin[1] = origin[1];
    currentOrigin[2] = origin[2];
    ent->spawnflags = iSpawnFlags;
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 5 )
    {
        defName = Scr_GetString(5u, SCRIPTINSTANCE_SERVER);
        G_SetupDestructible(ent, (char *)defName);
        ent->model = 1;
    }
    if ( G_CallSpawnEntity(ent) )
    {
        Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        slStr = SL_ConvertToString(classname, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("unable to spawn \"%s\" entity", slStr);
        Scr_Error(fmtMsg, 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetVisionSetNaked ~82610278)
void __cdecl ScrCmd_GetVisionSetNaked(scr_entref_t entref)
{
    gentity_s *ent;
    int clientNum;
    char configString[1024];
    char visionName[64];
    const char *configStringPtr;
    int i;

    clientNum = -1;
    ent = GetEntity(entref);
    if ( ent && ent->r.inuse && ent->client )
        clientNum = ent->s.number;
    else
        Scr_Error("getvisionsetnaked() called on an invalid client entity.\n", 0);
    SV_GetConfigstring(1554, configString, 1024);
    configStringPtr = Info_ValueForKey(configString, va("%i", clientNum));
    Com_Parse(&configStringPtr);
    I_strncpyz(visionName, Com_Parse(&configStringPtr)->token, 64);
    for ( i = 0; visionName[i]; ++i )
    {
        if ( !I_isforfilename((unsigned char)visionName[i]) )
        {
            visionName[i] = 0;
            break;
        }
    }
    Scr_AddString(visionName, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_VisionSetLastStand ~82610798)
static unsigned char s_visionSetLastStandSeq;

static void VisionSetLastStand_Apply(gentity_s *ent, const char *name, int duration)
{
    char *fmtMsg;
    char *keyMsg;
    char configString[1024];
    int clientNum;

    clientNum = -1;
    if ( ent && ent->r.inuse && ent->client )
        clientNum = ent->s.number;
    else
        Scr_Error("visionsetlaststand() called on an invalid client entity.\n", 0);
    SV_GetConfigstring(3148, configString, 1024);
    fmtMsg = va("\"%s\" %i %c", name, duration, s_visionSetLastStandSeq);
    keyMsg = va("%i", clientNum);
    Info_SetValueForKey(configString, keyMsg, fmtMsg);
    s_visionSetLastStandSeq = (s_visionSetLastStandSeq + 1) | 1;
    SV_SetConfigstring(3148, configString);
}

void __cdecl GScr_VisionSetLastStand()
{
    gentity_s *ent;
    int duration;
    int numParam;
    float tempValue2;
    char *name;
    int clientIndex;

    duration = 1000;
    numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParam != 1 && numParam != 2 )
        Scr_Error("USAGE: player visionsetlaststand( <visionset name>, <transition time> )\n", 0);
    if ( numParam == 2 )
    {
        tempValue2 = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER) * 1000.0f;
        duration = (int)(tempValue2 + 9.313225746154785e-10);
    }
    name = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    ent = 0;
    for ( clientIndex = 0; clientIndex < level.maxclients; ++clientIndex )
    {
        if ( g_entities[clientIndex].r.inuse && g_entities[clientIndex].client )
        {
            ent = &g_entities[clientIndex];
            break;
        }
    }
    VisionSetLastStand_Apply(ent, name, duration);
}

void __cdecl ScrCmd_VisionSetLastStand(scr_entref_t entref)
{
    int duration;
    int numParam;
    float tempValue2;
    gentity_s *ent;
    char *name;

    duration = 1000;
    numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParam != 1 && numParam != 2 )
        Scr_Error("USAGE: player visionsetlaststand( <visionset name>, <transition time> )\n", 0);
    if ( numParam == 2 )
    {
        tempValue2 = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER) * 1000.0f;
        duration = (int)(tempValue2 + 9.313225746154785e-10);
    }
    ent = GetEntity(entref);
    name = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    VisionSetLastStand_Apply(ent, name, duration);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Player_SetOffscreenMaterial ~82650CB0)
void __cdecl GScr_Player_SetOffscreenMaterial()
{
    char configString[1024];
    char *materialName;
    int clientNum;

    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 2 )
        Scr_Error("Too few arguments\n", 0);
    clientNum = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( clientNum >= 4 )
        Scr_Error(va("%i is not a valid player number", clientNum), 0);
    materialName = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    SV_GetConfigstring(clientNum + 2716, configString, 1024);
    Info_SetValueForKey(configString, "material", materialName);
    SV_SetConfigstring(clientNum + 2716, configString);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Player_SetOffscreenDownMaterial ~82650D90)
void __cdecl GScr_Player_SetOffscreenDownMaterial()
{
    char configString[1024];
    char *materialName;
    int clientNum;

    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 2 )
        Scr_Error("Too few arguments\n", 0);
    clientNum = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( clientNum >= 4 )
        Scr_Error(va("%i is not a valid player number", clientNum), 0);
    materialName = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    SV_GetConfigstring(clientNum + 2720, configString, 1024);
    Info_SetValueForKey(configString, "material", materialName);
    SV_SetConfigstring(clientNum + 2720, configString);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_MakeFakeAI ~82617A80)
void __cdecl ScrCmd_MakeFakeAI(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->s.eType != ET_SCRIPTMOVER || (ent->flags & 0x2000) == 0 )
        Scr_Error("makeFakeAI must be applied to a script_model", 0);
    ent->s.lerp.eFlags &= ~4u;
    ent->s.lerp.eFlags |= 2u;
    ent->r.mins[0] = -15.0f;
    ent->r.mins[1] = -15.0f;
    ent->r.mins[2] = 0.0f;
    ent->r.maxs[0] = 15.0f;
    ent->r.maxs[1] = 15.0f;
    ent->r.maxs[2] = 72.0f;
    ent->flags |= 0x8000u;
    ent->handler = 7;
    SV_LinkEntity(ent);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_BloodImpact ~8261E6B0)
void __cdecl ScrCmd_BloodImpact(scr_entref_t entref)
{
    gentity_s *ent;
    gentity_s *tent;
    unsigned __int16 impactType;
    VariableUnion impactString;

    ent = GetEntity(entref);
    if ( !Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        Scr_Error("BloodImpact() called without an argument\n", 0);
    impactString.intValue = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    if ( impactString.stringValue == scr_const.none )
        impactType = 0;
    else if ( impactString.stringValue == scr_const.normal )
        impactType = 1;
    else if ( impactString.stringValue == scr_const.hero )
        impactType = 2;
    else
        Scr_Error("BloodImpact() only takes \"normal\", \"hero\" or \"none\" as parameters.\n", 0);
    tent = G_TempEntity(vec3_origin, EV_BLOOD_IMPACTS);
    tent->s.otherEntityNum = ent->s.number;
    tent->s.eventParm = impactType;
    Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScrCmd_SetExploderId ~8261E7E8)
void __cdecl ScrCmd_SetExploderId(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->s.eType != ET_SCRIPTMOVER )
        Scr_Error("setExploderId must be applied to a script mover", 0);
    if ( !Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        Scr_Error("setExploderId requires an exploder id argument\n", 0);
    ent->s.lerp.u.scriptMover.exploderIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScrCmd_TransmitTargetName ~8261E8B0)
void __cdecl ScrCmd_TransmitTargetName(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    ent->s.targetname = ent->targetname;
    ent->s.clientLinkInfo.parentEnt = (ent->s.clientLinkInfo.parentEnt & 0xFEFE) | 0x100;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SpawnCollision)
void GScr_SpawnCollision()
{
    const char *fmtMsg;
    float *currentAngles;
    float *currentOrigin;
    float origin[3];
    float angles[3];
    unsigned int targetname;
    gentity_s *ent;
    const char *modelname;

    modelname = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    targetname = Scr_GetConstString(1u, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(2u, origin, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(3u, angles, SCRIPTINSTANCE_SERVER);
    ent = G_Spawn();
    Scr_SetString(&ent->classname, scr_const.script_model, SCRIPTINSTANCE_SERVER);
    G_SetModel(ent, (char *)modelname);
    if ( !ent->model )
    {
        fmtMsg = va("SpawnCollision: Collision model name %s is not valid.", modelname);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    Scr_SetString(&ent->targetname, targetname, SCRIPTINSTANCE_SERVER);
    currentOrigin = ent->r.currentOrigin;
    ent->r.currentOrigin[0] = origin[0];
    currentOrigin[1] = origin[1];
    currentOrigin[2] = origin[2];
    currentAngles = ent->r.currentAngles;
    ent->r.currentAngles[0] = angles[0];
    currentAngles[1] = angles[1];
    currentAngles[2] = angles[2];
    G_CallSpawnEntity(ent);
    ent->s.lerp.eFlags |= 0x200u;
    Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SpawnVehicle)
void GScr_SpawnVehicle()
{
    const char *fmtMsg;
    char *slStr;
    char *slStr2;
    float *currentAngles;
    float *currentOrigin;
    float origin[3];
    float angles[3];
    unsigned int targetname;
    unsigned int vehicletype;
    gentity_s *ent;
    unsigned int destructibledef;
    const char *modelname;

    modelname = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    targetname = Scr_GetConstString(1u, SCRIPTINSTANCE_SERVER);
    vehicletype = Scr_GetConstString(2u, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(3u, origin, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(4u, angles, SCRIPTINSTANCE_SERVER);
    ent = G_Spawn();
    Scr_SetString(&ent->classname, scr_const.script_vehicle, SCRIPTINSTANCE_SERVER);
    G_SetModel(ent, (char *)modelname);
    if ( !ent->model )
    {
        fmtMsg = va("SpawnVehicle: Vehicle model name %s is not valid.", modelname);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    Scr_SetString(&ent->targetname, targetname, SCRIPTINSTANCE_SERVER);
    currentOrigin = ent->r.currentOrigin;
    ent->r.currentOrigin[0] = origin[0];
    currentOrigin[1] = origin[1];
    currentOrigin[2] = origin[2];
    currentAngles = ent->r.currentAngles;
    ent->r.currentAngles[0] = angles[0];
    currentAngles[1] = angles[1];
    currentAngles[2] = angles[2];
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 5 )
    {
        destructibledef = Scr_GetConstString(5u, SCRIPTINSTANCE_SERVER);
        slStr = SL_ConvertToString(destructibledef, SCRIPTINSTANCE_SERVER);
        G_SetupDestructible(ent, slStr);
    }
    slStr2 = SL_ConvertToString(vehicletype, SCRIPTINSTANCE_SERVER);
    G_SpawnVehicle(ent, slStr2, 0);
    Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
    G_MakeVehicleUsable(ent);
}

// Decomp: CoDSP_rdBlackOps.map.c (NetworkOriginError ~825F6778)
static void NetworkOriginError(const char *entityType, const float *origin)
{
    const float maxXY = (float)(1 << (18 - 1));
    const float maxZ = (float)(1 << (17 - 1));

    Scr_Error(
        va(
            "Trying to spawn %s at position (%0.2f, %0.2f, %0.2f) will place it outside of map ranges that can be sent across the network.\n"
            " Which given a map center (%0.2f, %0.2f, %0.2f) should be between (%0.2f, %0.2f, %0.2f) to (%0.2f, %0.2f, %0.2f)\n",
            entityType,
            origin[0],
            origin[1],
            origin[2],
            svs.mapCenter[0],
            svs.mapCenter[1],
            svs.mapCenter[2],
            svs.mapCenter[0] - maxXY,
            svs.mapCenter[1] - maxXY,
            svs.mapCenter[2] - maxZ,
            svs.mapCenter[0] + maxXY,
            svs.mapCenter[1] + maxXY,
            svs.mapCenter[2] + maxZ),
        0);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_CodeSpawnVehicle ~825F7190)
void GScr_CodeSpawnVehicle()
{
    char *slStr;
    float *currentAngles;
    float *currentOrigin;
    float origin[3];
    float angles[3];
    unsigned int targetname;
    unsigned int vehicletype;
    gentity_s *ent;
    unsigned int destructibledef;
    const vehicle_info_t *vehicleInfo;
    int vehicleInfoIndex;

    targetname = Scr_GetConstString(1u, SCRIPTINSTANCE_SERVER);
    vehicletype = Scr_GetConstString(2u, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(3u, origin, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(4u, angles, SCRIPTINSTANCE_SERVER);
    if ( !BG_ValidateOrigin(origin, 18, 17, svs.mapCenter) )
        NetworkOriginError("vehicle", origin);
    TrackSpawn(vehicletype, origin);
    ent = G_Spawn();
    Scr_SetString(&ent->classname, scr_const.script_vehicle, SCRIPTINSTANCE_SERVER);
    if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) == VAR_STRING )
    {
        G_SetModel(ent, (char *)Scr_GetString(0, SCRIPTINSTANCE_SERVER));
    }
    else
    {
        slStr = SL_ConvertToString(vehicletype, SCRIPTINSTANCE_SERVER);
        vehicleInfoIndex = VEH_GetVehicleInfoFromName(slStr);
        vehicleInfo = BG_GetVehicleInfo(vehicleInfoIndex);
        G_SetModel(ent, (char *)vehicleInfo->model);
    }
    Scr_SetString(&ent->targetname, targetname, SCRIPTINSTANCE_SERVER);
    currentOrigin = ent->r.currentOrigin;
    ent->r.currentOrigin[0] = origin[0];
    currentOrigin[1] = origin[1];
    currentOrigin[2] = origin[2];
    currentAngles = ent->r.currentAngles;
    ent->r.currentAngles[0] = angles[0];
    currentAngles[1] = angles[1];
    currentAngles[2] = angles[2];
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 5 )
    {
        destructibledef = Scr_GetConstString(5u, SCRIPTINSTANCE_SERVER);
        slStr = SL_ConvertToString(destructibledef, SCRIPTINSTANCE_SERVER);
        G_SetupDestructible(ent, slStr);
    }
    slStr = SL_ConvertToString(vehicletype, SCRIPTINSTANCE_SERVER);
    G_SpawnVehicle(ent, slStr, 0);
    Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SpawnPlane)
void GScr_SpawnPlane()
{
    char *slStr;
    const char *fmtMsg;
    float *currentOrigin;
    float origin[3];
    int iSpawnFlags;
    int team;
    gentity_s *owner;
    unsigned __int16 classname;
    gentity_s *ent;
    int ownerIndex;

    owner = Scr_GetEntity(0);
    if ( !owner->client )
        Scr_ParamError(0, "Owner entity is not a player", SCRIPTINSTANCE_SERVER);
    classname = (unsigned __int16)Scr_GetConstString(1u, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(2u, origin, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 3 )
        iSpawnFlags = 0;
    else
        iSpawnFlags = Scr_GetInt(3u, SCRIPTINSTANCE_SERVER);
    ent = G_Spawn();
    Scr_SetString(&ent->classname, classname, SCRIPTINSTANCE_SERVER);
    currentOrigin = ent->r.currentOrigin;
    ent->r.currentOrigin[0] = origin[0];
    currentOrigin[1] = origin[1];
    currentOrigin[2] = origin[2];
    ent->spawnflags = iSpawnFlags;
    team = owner->client->sess.cs.team;
    if ( (unsigned int)team >= 4
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    1876,
                    0,
                    "team doesn't index (1 << 2)\n\t%i not in [0, %i)",
                    team,
                    4) )
    {
        __debugbreak();
    }
    ownerIndex = owner->client - level.clients;
    if ( (unsigned int)ownerIndex >= level.maxclients
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    1878,
                    0,
                    "ownerIndex doesn't index level.maxclients\n\t%i not in [0, %i)",
                    ownerIndex,
                    level.maxclients) )
    {
        __debugbreak();
    }
    // Decomp: CoDSP_rdBlackOps.map.c (InitScriptMover) — InitScriptMover forces ET_SCRIPTMOVER (426001);
    // GScr_SpawnPlane sets ET_PLANE and owner faction only after successful G_CallSpawnEntity.
    if ( G_CallSpawnEntity(ent) )
    {
        Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
        ent->s.eType = ET_PLANE;
        ent->s.faction.iHeadIconTeam = team | (4 * ownerIndex);
    }
    else
    {
        ent->s.eType = ET_PLANE;
        ent->s.faction.iHeadIconTeam = team | (4 * ownerIndex);
        slStr = SL_ConvertToString(classname, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("unable to spawn \"%s\" entity", slStr);
        Scr_Error(fmtMsg, 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SpawnTimedFX)
void GScr_SpawnTimedFX()
{
    unsigned __int8 directionByte;
    char *weaponName;
    float origin[3];
    int time;
    int weaponIndex;
    gentity_s *ent;
    float direction[3];
    const WeaponDef *weapDef;

    direction[0] = 0.0f;
    direction[1] = 0.0f;
    direction[2] = 1.0f;
    time = 10;
    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, origin, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 2 )
        Scr_GetVector(2u, direction, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 3 )
        time = Scr_GetInt(3u, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    Scr_VerifyWeaponIndex(weaponIndex, weaponName);
    weapDef = BG_GetWeaponDef(weaponIndex);
    ent = G_Spawn();
    G_SetOrigin(ent, origin);
    ent->s.weapon = weaponIndex;
    Vec3Normalize(direction);
    ent->s.eType = ET_GENERAL;
    ent->s.lerp.eFlags |= 0x20u;
    ent->s.weapon = ent->s.weapon;
    G_BroadcastEntity(ent);
    directionByte = DirToByte(direction);
    G_AddEvent(ent, 0x43u, directionByte);
    ent->s.lerp.pos.trBase[0] = (float)(int)ent->s.lerp.pos.trBase[0];
    ent->s.lerp.pos.trBase[1] = (float)(int)ent->s.lerp.pos.trBase[1];
    ent->s.lerp.pos.trBase[2] = (float)(int)ent->s.lerp.pos.trBase[2];
    G_SetOrigin(ent, ent->s.lerp.pos.trBase);
    ent->s.lerp.eFlags |= 0x4000u;
    ent->s.lerp.u.actor.actorNum = level.time;
    ent->s.time2 = level.time + 1000 * time;
    ent->s.lerp.eFlags |= 0x10u;
    ent->handler = 11;
    ent->nextthink = level.time + 1;
    SV_LinkEntity( ent);
}

// Decomp: CoDSP_rdBlackOps.map.c (SpawnTurretInternal)
gentity_s *__cdecl SpawnTurretInternal(unsigned int classname, float *origin, const char *weaponinfoname)
{
    gentity_s *ent;

    ent = G_Spawn();
    Scr_SetString(&ent->classname, classname, SCRIPTINSTANCE_SERVER);
    ent->r.currentOrigin[0] = *origin;
    ent->r.currentOrigin[1] = origin[1];
    ent->r.currentOrigin[2] = origin[2];
    G_SpawnTurret(ent, weaponinfoname, 0);
    return ent;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SpawnTurret)
void GScr_SpawnTurret()
{
    float origin[3];
    unsigned __int16 classname;
    gentity_s *ent;
    const char *weaponinfoname;

    ent = 0;
    classname = (unsigned __int16)Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, origin, SCRIPTINSTANCE_SERVER);
    weaponinfoname = Scr_GetString(2u, SCRIPTINSTANCE_SERVER);
    ent = SpawnTurretInternal(classname, origin, weaponinfoname);
    ent->takedamage = 1;
    ent->r.svFlags = 4;
    SV_LinkEntity(ent);
    Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SpawnHelicopter)
void GScr_SpawnHelicopter()
{
    float *currentAngles;
    float *currentOrigin;
    float origin[3];
    gentity_s *owner;
    const char *vehicleInfoName;
    float angles[3];
    gentity_s *ent;
    const char *modelname;

    owner = Scr_GetEntity(0);
    if ( !owner->client )
        Scr_ParamError(0, "Owner entity is not a player", SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, origin, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(2u, angles, SCRIPTINSTANCE_SERVER);
    vehicleInfoName = Scr_GetString(3u, SCRIPTINSTANCE_SERVER);
    modelname = Scr_GetString(4u, SCRIPTINSTANCE_SERVER);
    ent = G_Spawn();
    Scr_SetString(&ent->classname, scr_const.script_vehicle, SCRIPTINSTANCE_SERVER);
    currentOrigin = ent->r.currentOrigin;
    ent->r.currentOrigin[0] = origin[0];
    currentOrigin[1] = origin[1];
    currentOrigin[2] = origin[2];
    currentAngles = ent->r.currentAngles;
    ent->r.currentAngles[0] = angles[0];
    currentAngles[1] = angles[1];
    currentAngles[2] = angles[2];
    G_SpawnHelicopter(ent, owner, (char *)vehicleInfoName, (char *)modelname);
    Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetTurretCarried)
void __cdecl GScr_SetTurretCarried(scr_entref_t entref)
{
    char *slStr;
    const char *fmtMsg;
    VariableUnion varUnion;
    gentity_s *self;

    self = GetEntity(entref);
    if ( !self->pTurretInfo )
    {
        slStr = SL_ConvertToString(self->classname, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("entity type '%s' is not a turret", slStr);
        Scr_Error(fmtMsg, 0);
    }
    varUnion.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    Turret_SetTurretCarried(self, varUnion.intValue);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetAnimTreesLoaded)
void GScr_GetAnimTreesLoaded()
{
    XAnim_s *animTree;
    unsigned __int16 treeIndex;

    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for (treeIndex = 1; ; ++treeIndex)
    {
        if (treeIndex > gScrAnimPub[0].xanim_num[1])
            break;
        animTree = gScrAnimPub[0].xanim_lookup[1][treeIndex].anims;
        if (animTree
            && animTree->size > 1
            && animTree->debugName
            && &animTree->debugName[strlen(animTree->debugName) + 1] != animTree->debugName + 1)
        {
            Scr_AddString((char *)animTree->debugName, SCRIPTINSTANCE_SERVER);
            Scr_AddArray(SCRIPTINSTANCE_SERVER);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_FindAnimByName @ 82612A88)
void GScr_FindAnimByName()
{
    const char *treeName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( !strlen(treeName) )
        return;

    const char *animName = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    if ( !strlen(animName) )
        return;

    XAnim_s *animTree = 0;
    unsigned __int16 treeIndex = 0;
    for ( treeIndex = 1; treeIndex <= gScrAnimPub[0].xanim_num[1]; ++treeIndex )
    {
        animTree = gScrAnimPub[0].xanim_lookup[1][treeIndex].anims;
        if ( animTree && animTree->debugName && !strcmp(animTree->debugName, treeName) )
            break;
        animTree = 0;
    }
    if ( !animTree )
    {
        Scr_Error(va("Couldn't find animtree %s\n", treeName), 0);
        return;
    }

    for ( unsigned __int16 animIndex = 1; animIndex <= animTree->size - 1; ++animIndex )
    {
        if ( animTree->entries[animIndex].bCreated && !strcmp(animTree->debugAnimNames[animIndex], animName) )
        {
            scr_anim_s foundAnim;
            foundAnim.tree = treeIndex;
            foundAnim.index = animIndex;
            Scr_AddAnim(foundAnim, SCRIPTINSTANCE_SERVER);
            return;
        }
    }

    Scr_AddAnim((scr_anim_s)65537, SCRIPTINSTANCE_SERVER);
    Scr_Error(va("Couldn't find anim %s in animtree %s\n", animName, treeName), 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_PrecacheTurret)
void GScr_PrecacheTurret()
{
    char *turretInfo;

    if ( !level.initializing )
        Scr_Error("precacheTurret must be called before any wait statements in the level script\n", 0);
    turretInfo = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    G_GetWeaponIndexForName(turretInfo);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_SetMoveSpeedScale)
void __cdecl ScrCmd_SetMoveSpeedScale(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetPlayerEntity(entref);
    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 2217, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 2218, 0, "%s", "ent->client") )
    {
        __debugbreak();
    }
    ent->client->sess.moveSpeedScaleMultiplier = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_GetMoveSpeedScale)
void __cdecl ScrCmd_GetMoveSpeedScale(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetPlayerEntity(entref);
    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 2230, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 2231, 0, "%s", "ent->client") )
    {
        __debugbreak();
    }
    Scr_AddFloat(ent->client->sess.moveSpeedScaleMultiplier, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_AddAIEventListener)
void __cdecl ScrCmd_AddAIEventListener(scr_entref_t entref)
{
    gentity_s *ent;
    unsigned __int16 eventString;

    ent = GetEntity(entref);
    iassert(ent);
    eventString = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    Actor_EventListener_Add(ent->s.number, eventString);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_RemoveAIEventListener)
void __cdecl ScrCmd_RemoveAIEventListener(scr_entref_t entref)
{
    gentity_s *ent;
    unsigned __int16 eventString;

    ent = GetEntity(entref);
    iassert(ent);
    if ( !Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        Actor_EventListener_RemoveEntity(ent->s.number);
        return;
    }
    eventString = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    Actor_EventListener_Remove(ent->s.number, eventString);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_attach)
void __cdecl ScrCmd_attach(scr_entref_t entref)
{
    gentity_s *ent = GetEntity(entref);
    char *modelName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    VariableUnion tagName;
    VariableUnion ignoreCollision;
    const int numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);

    if ( numParam < 2 )
        tagName.intValue = scr_const._;
    else
        tagName.intValue = Scr_GetConstLowercaseString(1u, SCRIPTINSTANCE_SERVER);
    if ( numParam < 3 )
        ignoreCollision.intValue = 0;
    else
        ignoreCollision.intValue = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 4 )
        Scr_GetInt(3u, SCRIPTINSTANCE_SERVER); // optional attach index; consumed by script API only
    const char *stowedWeaponName = "";
    if ( numParam >= 5 )
        stowedWeaponName = Scr_GetString(4u, SCRIPTINSTANCE_SERVER);

    if ( G_EntDetach(ent, modelName, tagName.stringValue) )
    {
        const char *tagStr = SL_ConvertToString(tagName.stringValue, SCRIPTINSTANCE_SERVER);
        Scr_Error(va("model '%s' already attached to tag '%s'", modelName, tagStr), 0);
    }
    if ( !G_EntAttach(ent, modelName, tagName.stringValue, ignoreCollision.intValue) )
    {
        const char *tagStr = SL_ConvertToString(tagName.stringValue, SCRIPTINSTANCE_SERVER);
        Scr_Error(va("failed to attach model '%s' to tag '%s'", modelName, tagStr), 0);
    }
    if ( ent->client )
    {
        if ( *stowedWeaponName )
        {
            ent->client->ps.stowedWeapon = BG_FindWeaponIndexForName(stowedWeaponName);
            ent->client->ps.stowedWeaponCamo = 0;
            for ( int weaponSlot = 0; weaponSlot < 15; ++weaponSlot )
            {
                if ( ent->client->ps.heldWeapons[weaponSlot].weapon == ent->client->ps.stowedWeapon )
                {
                    ent->client->ps.stowedWeaponCamo = ent->client->ps.heldWeapons[weaponSlot].options.i & 0x3F;
                    break;
                }
            }
        }
        else
        {
            ent->client->ps.stowedWeapon = 0;
        }
        level_bgs.clientinfo[ent->s.number].dobjDirty = 1;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_detach)
void __cdecl ScrCmd_detach(scr_entref_t entref)
{
    unsigned int modelNameStr;
    char *slStr;
    char *slStr2;
    const char *fmtMsg;
    char *slStr3;
    VariableUnion varUnion;
    char *modelName;
    gentity_s *ent;
    int i;

    ent = GetEntity(entref);
    modelName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 2 )
        varUnion.intValue = scr_const._;
    else
        varUnion.intValue = Scr_GetConstLowercaseString(1u, SCRIPTINSTANCE_SERVER);
    if ( !G_EntDetach(ent, modelName, varUnion.stringValue) )
    {
        Com_Printf(24, "Current attachments:\n");
        for ( i = 0; i < 19; ++i )
        {
            if ( ent->attachModelNames[i] )
            {
                if ( ent->attachTagNames[i] )
                {
                    slStr3 = SL_ConvertToString(ent->attachTagNames[i], SCRIPTINSTANCE_SERVER);
                    modelNameStr = G_ModelName(ent->attachModelNames[i]);
                    slStr = SL_ConvertToString(modelNameStr, SCRIPTINSTANCE_SERVER);
                    Com_Printf(24, "model: '%s', tag: '%s'\n", slStr, slStr3);
                }
            }
        }
        slStr2 = SL_ConvertToString(varUnion.stringValue, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("failed to detach model '%s' from tag '%s'", modelName, slStr2);
        Scr_Error(fmtMsg, 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_detachAll)
void __cdecl ScrCmd_detachAll(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    G_EntDetachAll(ent);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_GetAttachSize)
void __cdecl ScrCmd_GetAttachSize(scr_entref_t entref)
{
    gentity_s *ent;
    int i;

    ent = GetEntity(entref);
    for ( i = 0; i < 19 && ent->attachModelNames[i]; ++i )
        ;
    Scr_AddInt(i, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_GetAttachModelName)
void __cdecl ScrCmd_GetAttachModelName(scr_entref_t entref)
{
    unsigned int modelNameStr;
    gentity_s *ent;
    int i;

    ent = GetEntity(entref);
    i = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)i >= 0x13 || !ent->attachModelNames[i] )
        Scr_ParamError(0, "bad index", SCRIPTINSTANCE_SERVER);
    modelNameStr = G_ModelName(ent->attachModelNames[i]);
    Scr_AddConstString(modelNameStr, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_GetAttachTagName)
void __cdecl ScrCmd_GetAttachTagName(scr_entref_t entref)
{
    gentity_s *ent;
    int i;

    ent = GetEntity(entref);
    i = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)i >= 0x13 || !ent->attachModelNames[i] )
        Scr_ParamError(0, "bad index", SCRIPTINSTANCE_SERVER);
    if ( !ent->attachTagNames[i]
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    2364,
                    0,
                    "%s",
                    "ent->attachTagNames[i]") )
    {
        __debugbreak();
    }
    Scr_AddConstString(ent->attachTagNames[i], SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_GetAttachIgnoreCollision)
void __cdecl ScrCmd_GetAttachIgnoreCollision(scr_entref_t entref)
{
    gentity_s *ent;
    int i;

    ent = GetEntity(entref);
    i = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)i >= 0x13 || !ent->attachModelNames[i] )
        Scr_ParamError(0, "bad index", SCRIPTINSTANCE_SERVER);
    Scr_AddBool((ent->attachIgnoreCollision & (1 << i)) != 0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (G_EntityStateSetPartBits)
void __cdecl G_EntityStateSetPartBits(gentity_s *ent, const unsigned int *partBits)
{
    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 2387, 0, "%s", "ent") )
        __debugbreak();
    ent->s.partBits[0] = *partBits;
    ent->s.partBits[1] = partBits[1];
    ent->s.partBits[2] = partBits[2];
    ent->s.partBits[3] = partBits[3];
    ent->s.partBits[4] = partBits[4];
}

// Decomp: CoDSP_rdBlackOps.map.c (G_EntityStateGetPartBits)
void __cdecl G_EntityStateGetPartBits(const gentity_s *ent, unsigned int *partBits)
{
    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 2394, 0, "%s", "ent") )
        __debugbreak();
    *partBits = ent->s.partBits[0];
    partBits[1] = ent->s.partBits[1];
    partBits[2] = ent->s.partBits[2];
    partBits[3] = ent->s.partBits[3];
    partBits[4] = ent->s.partBits[4];
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_UseWeaponHideTags @ 8260E258)
void __cdecl GScr_UseWeaponHideTags(scr_entref_t entref)
{
    gentity_s *ent;
    DObj *obj;
    const char *weaponName;
    unsigned int weaponIndex;
    const WeaponVariantDef *weapVariantDef;
    unsigned int partBits[5];
    int tagIndex;
    unsigned __int8 boneIndex;

    if ( !Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        Scr_Error("useweaponhidetags <weaponName>.\n", 0);
    weaponName = Scr_GetString(0u, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName((char *)weaponName);
    if ( !weaponIndex )
        Scr_Error(va("useweaponhidetags called with unknown weapon name %s\n", weaponName), 0);
    weapVariantDef = BG_GetWeaponVariantDef(weaponIndex);
    ent = GetEntity(entref);
    memset(partBits, 0, sizeof(partBits));
    G_EntityStateSetPartBits(ent, partBits);
    obj = Com_GetServerDObj(ent->s.number);
    if ( !obj )
        return;
    for ( tagIndex = 0; tagIndex < 32 && weapVariantDef->hideTags[tagIndex]; ++tagIndex )
    {
        boneIndex = -2;
        if ( DObjGetBoneIndex(obj, weapVariantDef->hideTags[tagIndex], &boneIndex, -1) )
            partBits[(int)boneIndex >> 5] |= 0x80000000 >> (boneIndex & 0x1F);
    }
    DObjSetHidePartBits(obj, partBits);
    G_EntityStateSetPartBits(ent, partBits);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_hidepart)
void __cdecl ScrCmd_hidepart(scr_entref_t entref)
{
    char *slStr;
    const char *fmtMsg;
    char *slStr2;
    const char *fmtMsg2;
    const char *targetModelName;
    unsigned int tagName;
    unsigned __int8 boneIndex;
    DObj *obj;
    const char *modelName;
    gentity_s *ent;
    unsigned int partBits[5];

    ent = GetEntity(entref);
    obj = Com_GetServerDObj(ent->s.number);
    if ( !obj )
        Scr_Error("entity has no model", 0);
    boneIndex = -2;
    tagName = Scr_GetConstLowercaseString(0, SCRIPTINSTANCE_SERVER);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        if ( !DObjGetBoneIndex(obj, tagName, &boneIndex, -1) )
        {
            slStr = SL_ConvertToString(tagName, SCRIPTINSTANCE_SERVER);
            fmtMsg = va("cannot find part '%s' in entity model", slStr);
            Scr_Error(fmtMsg, 0);
        }
    }
    else
    {
        modelName = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
        if ( !DObjGetModelBoneIndex(obj, modelName, tagName, &boneIndex) )
        {
            targetModelName = modelName;
            slStr2 = SL_ConvertToString(tagName, SCRIPTINSTANCE_SERVER);
            fmtMsg2 = va("cannot find part '%s' in entity model '%s'", slStr2, targetModelName);
            Scr_Error(fmtMsg2, 0);
        }
    }
    G_EntityStateGetPartBits(ent, partBits);
    partBits[(int)boneIndex >> 5] |= 0x80000000 >> (boneIndex & 0x1F);
    DObjSetHidePartBits(obj, partBits);
    G_EntityStateSetPartBits(ent, partBits);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_showpart)
void __cdecl ScrCmd_showpart(scr_entref_t entref)
{
    char *slStr;
    const char *fmtMsg;
    char *slStr2;
    const char *fmtMsg2;
    const char *targetModelName;
    unsigned int tagName;
    unsigned __int8 boneIndex;
    DObj *obj;
    const char *modelName;
    gentity_s *ent;
    unsigned int partBits[5];

    ent = GetEntity(entref);
    obj = Com_GetServerDObj(ent->s.number);
    if ( !obj )
        Scr_Error("entity has no model", 0);
    boneIndex = -2;
    tagName = Scr_GetConstLowercaseString(0, SCRIPTINSTANCE_SERVER);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        if ( !DObjGetBoneIndex(obj, tagName, &boneIndex, -1) )
        {
            slStr = SL_ConvertToString(tagName, SCRIPTINSTANCE_SERVER);
            fmtMsg = va("cannot find part '%s' in entity model", slStr);
            Scr_Error(fmtMsg, 0);
        }
    }
    else
    {
        modelName = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
        if ( !DObjGetModelBoneIndex(obj, modelName, tagName, &boneIndex) )
        {
            targetModelName = modelName;
            slStr2 = SL_ConvertToString(tagName, SCRIPTINSTANCE_SERVER);
            fmtMsg2 = va("cannot find part '%s' in entity model '%s'", slStr2, targetModelName);
            Scr_Error(fmtMsg2, 0);
        }
    }
    G_EntityStateGetPartBits(ent, partBits);
    partBits[(int)boneIndex >> 5] &= ~(0x80000000 >> (boneIndex & 0x1F));
    DObjSetHidePartBits(obj, partBits);
    G_EntityStateSetPartBits(ent, partBits);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_showallparts)
void __cdecl ScrCmd_showallparts(scr_entref_t entref)
{
    DObj *obj;
    gentity_s *ent;
    unsigned int partBits[5];

    ent = GetEntity(entref);
    obj = Com_GetServerDObj(ent->s.number);
    if ( !obj )
        Scr_Error("entity has no model", 0);
    memset(partBits, 0, sizeof(partBits));
    DObjSetHidePartBits(obj, partBits);
    G_EntityStateSetPartBits(ent, partBits);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_SetVisibleToPlayer)
void __cdecl ScrCmd_SetVisibleToPlayer(scr_entref_t entref)
{
    gentity_s *ent;
    gentity_s *player;

    ent = GetEntity(entref);
    player = Scr_GetEntity(0);
    if ( !player->client
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    2496,
                    0,
                    "%s",
                    "player->client") )
    {
        __debugbreak();
    }
    ent->r.clientMask[player->s.number >> 5] &= ~(1 << (player->s.number & 0x1F));
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_SetInvisibleToPlayer)
void __cdecl ScrCmd_SetInvisibleToPlayer(scr_entref_t entref)
{
    int clientMaskValue;
    int invisible;
    gentity_s *ent;
    int clientNum;
    gentity_s *player;

    invisible = 1;
    ent = GetEntity(entref);
    player = Scr_GetEntity(0);
    if ( !player->client
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    2512,
                    0,
                    "%s",
                    "player->client") )
    {
        __debugbreak();
    }
    clientNum = player->s.number;
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 1 )
        invisible = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    if ( invisible )
        clientMaskValue = ent->r.clientMask[clientNum >> 5] | (1 << (clientNum & 0x1F));
    else
        clientMaskValue = ent->r.clientMask[clientNum >> 5] & ~(1 << (clientNum & 0x1F));
    ent->r.clientMask[clientNum >> 5] = clientMaskValue;
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_SetVisibleToAll)
void __cdecl ScrCmd_SetVisibleToAll(scr_entref_t entref)
{
    GetEntity(entref)->r.clientMask[0] = 0;
}

// LWSS ADD: later retail MP script API — not present in CoDOMPServer 1.0 decomp (see manifest STUB).
// Sets lerp.eFlags bit 0x8000000 on the entity; kbBack used the same. Client honors it in CG_GetLightingOrigin.
// BlackOps.singleplayer.c ~175268 uses this bit for render-effect Z when EF_RENDER_EFFECT (0x200); different path.
void ScrCmd_OverrideLightingOrigin(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    ent->s.lerp.eFlags |= 0x8000000u;
}
// LWSS END

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_SetForceNoCull)
void __cdecl ScrCmd_SetForceNoCull(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    ent->s.lerp.eFlags2 |= 0x4000000u;
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_SetInvisibleToAll)
void __cdecl ScrCmd_SetInvisibleToAll(scr_entref_t entref)
{
    GetEntity(entref)->r.clientMask[0] = -1;
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_SetVisibleToTeam)
void __cdecl ScrCmd_SetVisibleToTeam(scr_entref_t entref)
{
    char *slStr;
    const char *fmtMsg;
    int teamNum;
    unsigned __int16 team;
    int entIndex;
    gentity_s *ent;
    gentity_s *clientEnt;

    ent = GetEntity(entref);
    team = (unsigned __int16)Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    if ( team != scr_const.allies && team != scr_const.axis )
    {
        slStr = SL_ConvertToString(team, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("Illegal team string '%s'. Must be allies, or axis.", slStr);
        Scr_Error(fmtMsg, 0);
    }
    if ( team == scr_const.allies )
        teamNum = 2;
    else
        teamNum = 1;
    ent->r.clientMask[0] = -1;
    clientEnt = g_entities;
    for ( entIndex = 0; entIndex < com_maxclients->current.integer; ++entIndex )
    {
        if ( clientEnt->r.inuse )
        {
            if ( !clientEnt->client
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                            2621,
                            0,
                            "%s",
                            "clientEnt->client") )
            {
                __debugbreak();
            }
            if ( clientEnt->client->sess.cs.team == teamNum )
                ent->r.clientMask[clientEnt->s.number >> 5] &= ~(1 << (clientEnt->s.number & 0x1F));
        }
        ++clientEnt;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_IsLinkedTo)
void __cdecl ScrCmd_IsLinkedTo(scr_entref_t entref)
{
    bool IsLinkedTo;
    gentity_s *parent;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) != 1 || Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) != 19 )
        Scr_ParamError(0, "not an entity", SCRIPTINSTANCE_SERVER);
    if ( (ent->flags & 0x1000) != 0 )
    {
        parent = Scr_GetEntity(0);
        if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) != 1 || Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) != 19 )
            Scr_ParamError(0, "not an entity", SCRIPTINSTANCE_SERVER);
        IsLinkedTo = G_EntIsLinkedTo(ent, parent);
        Scr_AddInt(IsLinkedTo, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_LinkTo)
void __cdecl ScrCmd_LinkTo(scr_entref_t entref)
{
    char *slStr;
    const char *fmtMsg;
    unsigned int modelNameStr;
    char *slStr2;
    const char *fmtMsg2;
    unsigned int modelNameStr2;
    char *slStr3;
    const char *fmtMsg3;
    char *slStr4;
    VariableUnion tagName;
    float originOffset[3];
    float anglesOffset[3];
    int numParam;
    gentity_s *parent;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) != 1 || Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) != 19 )
        Scr_ParamError(0, "not an entity", SCRIPTINSTANCE_SERVER);
    if ( (ent->flags & 0x1000) == 0 )
    {
        slStr = SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("entity (classname: '%s') does not currently support linkTo", slStr);
        Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    parent = Scr_GetEntity(0);
    numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    tagName.intValue = 0;
    if ( numParam >= 2 )
    {
        tagName.intValue = Scr_GetConstLowercaseString(1u, SCRIPTINSTANCE_SERVER);
        if ( tagName.intValue == scr_const._ )
            tagName.intValue = 0;
    }
    if ( numParam > 2 )
    {
        Scr_GetVector(2u, originOffset, SCRIPTINSTANCE_SERVER);
        Scr_GetVector(3u, anglesOffset, SCRIPTINSTANCE_SERVER);
        if ( G_EntLinkToWithOffset(ent, parent, tagName.stringValue, originOffset, anglesOffset) )
            return;
    }
    else if ( G_EntLinkTo(ent, parent, tagName.stringValue) )
    {
        return;
    }
    if ( !SV_DObjExists(parent) )
    {
        if ( !parent->model )
            Scr_Error("failed to link entity since parent has no model", 0);
        modelNameStr = G_ModelName(parent->model);
        slStr2 = SL_ConvertToString(modelNameStr, SCRIPTINSTANCE_SERVER);
        fmtMsg2 = va("failed to link entity since parent model '%s' is invalid", slStr2);
        Scr_Error(fmtMsg2, 0);
    }
    if ( !parent->model
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    2712,
                    0,
                    "%s",
                    "parent->model") )
    {
        __debugbreak();
    }
    if ( tagName.intValue )
    {
        if ( SV_DObjGetBoneIndex(parent, tagName.stringValue) < 0 )
        {
            SV_DObjDumpInfo(parent);
            modelNameStr2 = G_ModelName(parent->model);
            slStr4 = SL_ConvertToString(modelNameStr2, SCRIPTINSTANCE_SERVER);
            slStr3 = SL_ConvertToString(tagName.stringValue, SCRIPTINSTANCE_SERVER);
            fmtMsg3 = va("failed to link entity since tag '%s' does not exist in parent model '%s'", slStr3, slStr4);
            Scr_Error(fmtMsg3, 0);
        }
    }
    Scr_Error("failed to link entity", 0);
}

static float Scr_ClampLinkArc(float input)
{
    float clamped;

    if ( (float)(input - 180.0) < 0.0 )
        clamped = input;
    else
        clamped = 180.0f;
    if ( (float)(0.0 - input) < 0.0 )
        return clamped;
    return 0.0f;
}

// Decomp: CoDSP_rdBlackOps.map.c (PlayerLinkToInternal ~825F91E8)
static bool PlayerLinkToInternal(
    gentity_s *player,
    gentity_s *parent,
    unsigned int tagName,
    float linkAnglesFrac,
    float minClamp0,
    float minClamp1,
    float maxClamp0,
    float maxClamp1)
{
    if ( !player || !player->client || !parent )
        return false;

    player->client->linkAnglesFrac = linkAnglesFrac;
    player->client->linkAnglesLocked = false;
    player->client->ps.linkFlags &= ~1u;
    player->client->link_rotationMovesEyePos = false;
    player->client->prevLinkAnglesSet = false;
    player->client->linkAnglesMaxClamp[0] = maxClamp0;
    player->client->linkAnglesMaxClamp[1] = maxClamp1;
    player->client->linkAnglesMinClamp[0] = minClamp0;
    player->client->linkAnglesMinClamp[1] = minClamp1;
    return G_EntLinkTo(player, parent, tagName) != 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_PlayerLinkTo ~825F93B8)
void ScrCmd_PlayerLinkTo(scr_entref_t entref)
{
    float bottomArc;
    float topArc;
    float leftArc;
    float rightArc;
    float linkAnglesFrac;
    float parentAxis[4][3];
    VariableUnion tagName;
    int numParam;
    gentity_s *parent;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) != 1 || Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) != 19 )
        Scr_ParamError(0, "not an entity", SCRIPTINSTANCE_SERVER);
    if ( !ent->client )
        Scr_ObjectError("not a player entity", SCRIPTINSTANCE_SERVER);
    if ( (ent->flags & 0x1000) == 0
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_main.cpp",
                    4915,
                    0,
                    "%s",
                    "ent->flags & FL_SUPPORTS_LINKTO") )
    {
        __debugbreak();
    }
    parent = Scr_GetEntity(0);
    numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    tagName.intValue = 0;
    if ( numParam > 1 )
    {
        if ( Scr_GetType(1u, SCRIPTINSTANCE_SERVER) )
        {
            tagName.intValue = Scr_GetConstLowercaseString(1u, SCRIPTINSTANCE_SERVER);
            if ( tagName.intValue == scr_const._ )
                tagName.intValue = 0;
        }
    }
    if ( numParam <= 2 )
        linkAnglesFrac = 0.0f;
    else
        linkAnglesFrac = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    if ( numParam <= 3 )
        rightArc = 180.0f;
    else
        rightArc = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
    if ( numParam <= 4 )
        leftArc = 180.0f;
    else
        leftArc = Scr_GetFloat(4u, SCRIPTINSTANCE_SERVER);
    if ( numParam <= 5 )
        topArc = 180.0f;
    else
        topArc = Scr_GetFloat(5u, SCRIPTINSTANCE_SERVER);
    if ( numParam <= 6 )
        bottomArc = 180.0f;
    else
        bottomArc = Scr_GetFloat(6u, SCRIPTINSTANCE_SERVER);

    G_UpdateViewAngleClamp(ent->client, parent->r.currentAngles);
    if ( numParam > 7 && Scr_GetInt(7u, SCRIPTINSTANCE_SERVER) )
        ent->client->ps.linkFlags |= 2u;
    else
        ent->client->ps.linkFlags &= ~2u;

    if ( !PlayerLinkToInternal(
            ent,
            parent,
            tagName.stringValue,
            linkAnglesFrac,
            -Scr_ClampLinkArc(topArc),
            Scr_ClampLinkArc(bottomArc),
            -Scr_ClampLinkArc(rightArc),
            Scr_ClampLinkArc(leftArc)) )
    {
        Scr_Error("failed to link entity", 0);
        return;
    }

    if ( (ent->client->ps.linkFlags & 2) != 0 )
    {
        G_CalcTagParentAxis(ent, parentAxis);
        AxisToAngles(parentAxis, ent->client->ps.linkAngles);
    }
    else
    {
        ent->client->ps.linkAngles[0] = 0.0f;
        ent->client->ps.linkAngles[1] = 0.0f;
        ent->client->ps.linkAngles[2] = 0.0f;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_PlayerLinkToAbsolute ~825F9FF8)
void ScrCmd_PlayerLinkToAbsolute(scr_entref_t entref)
{
    VariableUnion tagName;
    int numParam;
    gentity_s *parent;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) != 1 || Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) != 19 )
        Scr_ParamError(0, "Not an entity", SCRIPTINSTANCE_SERVER);
    if ( !ent->client )
        Scr_ObjectError("Not a player entity", SCRIPTINSTANCE_SERVER);
    if ( (ent->flags & 0x1000) == 0
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_main.cpp",
                    5077,
                    0,
                    "%s",
                    "ent->flags & FL_SUPPORTS_LINKTO") )
    {
        __debugbreak();
    }
    parent = Scr_GetEntity(0);
    numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    tagName.intValue = 0;
    if ( numParam > 1 )
    {
        if ( Scr_GetType(1u, SCRIPTINSTANCE_SERVER) )
        {
            tagName.intValue = Scr_GetConstLowercaseString(1u, SCRIPTINSTANCE_SERVER);
            if ( tagName.intValue == scr_const._ )
                tagName.intValue = 0;
        }
    }
    ent->client->linkAnglesFrac = 1.0f;
    ent->client->linkAnglesLocked = true;
    ent->client->link_rotationMovesEyePos = true;
    ent->client->ps.linkAngles[0] = 0.0f;
    ent->client->ps.linkAngles[1] = 0.0f;
    ent->client->ps.linkAngles[2] = 0.0f;
    ent->client->ps.linkFlags |= 1u;
    if ( !G_EntLinkTo(ent, parent, tagName.stringValue) )
        Scr_Error("Failed to link entity", 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_PlayerLinkToDelta)
void __cdecl ScrCmd_PlayerLinkToDelta(scr_entref_t entref)
{
    float *linkAngles;
    float maxPitchClamp;
    float minPitchClamp;
    float maxYawClamp;
    float minYawClamp;
    float linkAnglesFrac;
    float pitchMaxInput;
    float clampedPitchMax;
    float pitchMinInput;
    float clampedPitchMin;
    float yawMaxInput;
    float clampedYawMax;
    float yawMinInput;
    float clampedYawMin;
    VariableUnion tagName;
    float originOffset[3];
    float anglesOffset[3];
    int numParam;
    gentity_s *parent;
    float parentAxis[4][3];
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) != 1 || Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) != 19 )
        Scr_ParamError(0, "not an entity", SCRIPTINSTANCE_SERVER);
    if ( !ent->client )
        Scr_ObjectError("not a player entity", SCRIPTINSTANCE_SERVER);
    if ( (ent->flags & 0x1000) == 0
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    2741,
                    0,
                    "%s",
                    "ent->flags & FL_SUPPORTS_LINKTO") )
    {
        __debugbreak();
    }
    parent = Scr_GetEntity(0);
    numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    tagName.intValue = 0;
    if ( numParam > 1 )
    {
        if ( Scr_GetType(1u, SCRIPTINSTANCE_SERVER) )
        {
            tagName.intValue = Scr_GetConstLowercaseString(1u, SCRIPTINSTANCE_SERVER);
            if ( tagName.intValue == scr_const._ )
                tagName.intValue = 0;
        }
    }
    if ( numParam <= 2 )
        linkAnglesFrac = 0.0f;
    else
        linkAnglesFrac = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    ent->client->linkAnglesFrac = linkAnglesFrac;
    ent->client->linkAnglesLocked = 0;
    if ( numParam <= 3 )
        yawMinInput = 180.0f;
    else
        yawMinInput = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
    if ( (float)(yawMinInput - 180.0) < 0.0 )
        clampedYawMin = yawMinInput;
    else
        clampedYawMin = 180.0f;
    if ( (float)(0.0 - yawMinInput) < 0.0 )
        minYawClamp = clampedYawMin;
    else
        minYawClamp = 0.0f;
    ent->client->linkAnglesMinClamp[1] = -minYawClamp;
    if ( numParam <= 4 )
        yawMaxInput = 180.0f;
    else
        yawMaxInput = Scr_GetFloat(4u, SCRIPTINSTANCE_SERVER);
    if ( (float)(yawMaxInput - 180.0) < 0.0 )
        clampedYawMax = yawMaxInput;
    else
        clampedYawMax = 180.0f;
    if ( (float)(0.0 - yawMaxInput) < 0.0 )
        maxYawClamp = clampedYawMax;
    else
        maxYawClamp = 0.0f;
    ent->client->linkAnglesMaxClamp[1] = maxYawClamp;
    if ( numParam <= 5 )
        pitchMinInput = 180.0f;
    else
        pitchMinInput = Scr_GetFloat(5u, SCRIPTINSTANCE_SERVER);
    if ( (float)(pitchMinInput - 180.0) < 0.0 )
        clampedPitchMin = pitchMinInput;
    else
        clampedPitchMin = 180.0f;
    if ( (float)(0.0 - pitchMinInput) < 0.0 )
        minPitchClamp = clampedPitchMin;
    else
        minPitchClamp = 0.0f;
    ent->client->linkAnglesMinClamp[0] = -minPitchClamp;
    if ( numParam <= 6 )
        pitchMaxInput = 180.0f;
    else
        pitchMaxInput = Scr_GetFloat(6u, SCRIPTINSTANCE_SERVER);
    if ( (float)(pitchMaxInput - 180.0) < 0.0 )
        clampedPitchMax = pitchMaxInput;
    else
        clampedPitchMax = 180.0f;
    if ( (float)(0.0 - pitchMaxInput) < 0.0 )
        maxPitchClamp = clampedPitchMax;
    else
        maxPitchClamp = 0.0f;
    ent->client->linkAnglesMaxClamp[0] = maxPitchClamp;
    G_UpdateViewAngleClamp(ent->client, parent->r.currentAngles);
    if ( numParam > 7 && Scr_GetInt(7u, SCRIPTINSTANCE_SERVER) )
        ent->client->ps.linkFlags |= 2u;
    else
        ent->client->ps.linkFlags &= ~2u;
    ent->client->prevLinkAnglesSet = 0;
    parent->r.svFlags &= ~1u;
    if ( numParam > 8 )
    {
        Scr_GetVector(8u, originOffset, SCRIPTINSTANCE_SERVER);
        Scr_GetVector(9u, anglesOffset, SCRIPTINSTANCE_SERVER);
        if ( !G_EntLinkToWithOffset(ent, parent, tagName.stringValue, originOffset, anglesOffset) )
        {
            Scr_Error("failed to link entity", 0);
            return;
        }
    }
    else if ( !G_EntLinkTo(ent, parent, tagName.stringValue) )
    {
        Scr_Error("failed to link entity", 0);
        return;
    }
    if ( (ent->client->ps.linkFlags & 2) != 0 )
    {
        G_CalcTagParentAxis(ent, parentAxis);
        AxisToAngles(parentAxis, ent->client->ps.linkAngles);
    }
    else
    {
        linkAngles = ent->client->ps.linkAngles;
        *linkAngles = 0.0f;
        linkAngles[1] = 0.0f;
        linkAngles[2] = 0.0f;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_Unlink)
void __cdecl ScrCmd_Unlink(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->client && (ent->client->ps.eFlags & 0x4000) != 0 )
        VEH_UnlinkPlayer(ent, 0, (char*)"ScrCmd_Unlink");
    else
        G_EntUnlink(ent);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_GetLinkedEnt)
void __cdecl ScrCmd_GetLinkedEnt(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->tagInfo )
        Scr_AddEntity(ent->tagInfo->parent, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_EnableLinkTo)
void __cdecl ScrCmd_EnableLinkTo(scr_entref_t entref)
{
    char *slStr;
    const char *fmtMsg;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( (ent->flags & 0x1000) != 0 )
        Scr_ObjectError("entity already has linkTo enabled", SCRIPTINSTANCE_SERVER);
    if ( ent->s.eType || ent->physicsObject )
    {
        slStr = SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("entity (classname: '%s') does not currently support enableLinkTo", slStr);
        Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    if ( ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 2832, 0, "%s", "!ent->client") )
    {
        __debugbreak();
    }
    ent->flags |= 0x1000u;
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_GetOrigin)
void __cdecl ScrCmd_GetOrigin(scr_entref_t entref)
{
    float origin[3];
    gentity_s *ent;

    ent = GetEntity(entref);
    origin[0] = ent->r.currentOrigin[0];
    origin[1] = ent->r.currentOrigin[1];
    origin[2] = ent->r.currentOrigin[2];
    Scr_AddVector(ent->r.currentOrigin, SCRIPTINSTANCE_SERVER);
}

#ifdef KISAK_SP
// Decomp: CoDSP_rdBlackOps.map.c (GScr_LookAtEntity ~8261F5E8)
// Map: CoDSP_rd.map VA 0x8261f5e8 (g_scr_main.obj)
void __cdecl ScrCmd_LookAtEntity(scr_entref_t entref)
{
    gentity_s *ent;
    gentity_s *targetEnt;

    ent = GetEntity(entref);
    if ( !ent->actor )
    {
        Scr_Error(
            va("lookatentity must be called on an AI, not on a '%s'", SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER)),
            0);
    }
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) == 1
            && Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) == 19 )
        {
            targetEnt = Scr_GetEntity(0);
            ent->actor->sentient->scriptTargetEnt.setEnt(targetEnt);
        }
    }
    else
    {
        ent->actor->sentient->scriptTargetEnt.setEnt(0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_GetDebugEye ~825FD440)
// Map: CoDSP_rd.map VA 0x825fd440 (g_scr_main.obj)
void __cdecl ScrCmd_GetDebugEye(scr_entref_t entref)
{
    gentity_s *ent;
    float eyePos[3];

    ent = GetEntity(entref);
    if ( !ent->sentient )
    {
        Scr_Error(
            va(
                "getDebugEye must be called on an AI or player, not on a '%s'",
                SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER)),
            0);
    }
    Sentient_GetDebugEyePosition(ent->sentient, eyePos);
    Scr_AddVector(eyePos, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (pathnode spawnflags check)
void __cdecl ScrCmd_HasSpawnFlag(scr_entref_t entref)
{
    gentity_s *ent;
    pathnode_t *node;
    unsigned int flag;

    flag = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( entref.classnum == 2 )
    {
        if ( entref.entnum >= g_path.actualNodeCount )
            Scr_Error("bad pathnode entref", 0);
        node = &gameWorldCurrent->path.nodes[entref.entnum];
        Scr_AddBool((node->constant.spawnflags & flag) != 0, SCRIPTINSTANCE_SERVER);
    }
    else if ( entref.classnum == 3 )
    {
        Scr_AddBool(GScr_VehicleNodeHasSpawnFlag(entref.entnum, flag), SCRIPTINSTANCE_SERVER);
    }
    else if ( entref.classnum == 0 )
    {
        ent = &g_entities[entref.entnum];
        Scr_AddBool((ent->spawnflags & flag) != 0, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        // Decomp: common_scripts/utility.gsc has_spawnflag — structs without spawnflags return false.
        Scr_AddBool(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GibEnt ~82620858)
static void GibEntCommon(gentity_s *ent)
{
    unsigned int flags;
    unsigned int constString;
    unsigned int objectId;
    unsigned int siblingId;
    VariableValue indexValue;
    unsigned int tag;
    unsigned int index;

    flags = 0;
    constString = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    if ( constString == scr_const.normal )
        flags = 256;
    else if ( constString == scr_const.up )
        flags = 512;
    if ( Scr_GetPointerType(1u, SCRIPTINSTANCE_SERVER) != VAR_ARRAY )
        Scr_ParamError(1u, va("Parameter (%s) must be an array", Scr_GetTypeName(1u, SCRIPTINSTANCE_SERVER)), SCRIPTINSTANCE_SERVER);
    objectId = Scr_GetObject(1u, SCRIPTINSTANCE_SERVER);
    index = 0;
    for ( siblingId = FindFirstSibling(SCRIPTINSTANCE_SERVER, objectId); siblingId; siblingId = FindNextSibling(SCRIPTINSTANCE_SERVER, siblingId) )
    {
        indexValue = Scr_GetArrayIndexValue(SCRIPTINSTANCE_SERVER, siblingId);
        if ( indexValue.type != VAR_INTEGER )
            Scr_Error(va("Array passed to gib contained member [%i] - valid types are int.", index), 0);
        tag = indexValue.u.intValue;
        if ( tag >= 8 )
            Scr_Error("Gib tag array passed to 'Gib' contains value out of range 0 -> 7", 0);
        flags |= 1u << tag;
        ++index;
    }
    G_AddEvent(ent, EV_GIB, flags);
}

void __cdecl ScrCmd_Gib(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    GibEntCommon(ent);
}

void __cdecl GScr_GibEnt(scr_entref_t entref)
{
    GibEntCommon(GetEntity(entref));
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_HasEyes ~8261F770)
// Map: CoDSP_rd.map VA 0x8261F770 (g_scr_main.obj)
void __cdecl ScrCmd_HasEyes(scr_entref_t entref)
{
    gentity_s *ent;
    int enable;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("HasEyes() called with wrong params.\n", 0);
    ent = GetEntity(entref);
    enable = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( enable )
        ent->flags |= 0x20000u;
    else
        ent->flags &= ~0x20000u;
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_GetCentroid ~825FAC10)
// Map: CoDSP_rd.map VA 0x825FAC10 (g_scr_main.obj)
void __cdecl ScrCmd_GetCentroid(scr_entref_t entref)
{
    gentity_s *ent;
    float centroid[3];

    ent = GetEntity(entref);
    G_EntityCentroid(ent, centroid);
    Scr_AddVector(centroid, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_SetDeathContents ~825FF3E0)
// Map: CoDSP_rd.map VA 0x825FF3E0 (g_scr_main.obj)
void __cdecl ScrCmd_SetDeathContents(scr_entref_t entref)
{
    gentity_s *ent;
    int contents;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("setdeathcontents takes one parameter\n", 0);
    ent = GetEntity(entref);
    if ( !ent->actor )
        Scr_Error("setdeathcontents must be called on an AI only.", 0);
    contents = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    ent->actor->deathContents = contents;
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_PlayerSetGroundReferenceEnt ~825F9108)
// Map: CoDSP_rd.map VA 0x825F9108 (g_scr_main.obj)
void __cdecl ScrCmd_PlayerSetGroundReferenceEnt(scr_entref_t entref)
{
    gentity_s *ent;
    gentity_s *refEnt;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_ObjectError("not a player entity", SCRIPTINSTANCE_SERVER);
    if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) )
    {
        if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) != 1 || Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) != VAR_POINTER )
            Scr_ParamError(0, "not an entity", SCRIPTINSTANCE_SERVER);
        refEnt = Scr_GetEntity(0);
        *(int *)((char *)ent->client + 7440) = refEnt->s.number;
    }
    else
    {
        *(int *)((char *)ent->client + 7440) = ENTITYNUM_NONE;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetWeaponAccuracy ~8260B740)
// Map: CoDSP_rd.map VA 0x8260B740 (g_scr_main.obj)
void __cdecl GScr_GetWeaponAccuracy()
{
    actor_s *actor;
    sentient_s *targetSentient;
    const char *weaponName;
    unsigned int weaponIndex;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2 )
        Scr_Error(
            "Illegal call to getweaponaccuracy(self,self.weapon), AI entity and weapon name are mandatory fields.",
            0);
    actor = Scr_GetEntity(0)->actor;
    if ( !actor )
        Scr_Error("getweaponaccuracy must be called on an AI only.", 0);
    targetSentient = Actor_GetTargetSentient(actor);
    if ( !targetSentient )
        Scr_Error("getweaponaccuracy requires a valid enemy target.", 0);
    weaponName = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName((char *)weaponName);
    Scr_VerifyWeaponIndex(weaponIndex, weaponName);
    (void)weaponIndex;
    Scr_AddFloat(Actor_GetPlayerSightAccuracy(actor, targetSentient), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_PlayWeaponDeathEffects ~8260E3F8)
// Map: CoDSP_rd.map VA 0x8260E3F8 (g_scr_main.obj)
void __cdecl GScr_PlayWeaponDeathEffects(scr_entref_t entref)
{
    const char *weaponName;
    unsigned int weaponIndex;
    gentity_s *ent;
    gentity_s *tent;
    float origin[3];

    if ( !Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        Scr_Error("PlayWeaponDeathEffects <weaponName>.\n", 0);
    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName((char *)weaponName);
    if ( !weaponIndex )
        Scr_Error(va("PlayWeaponDeathEffects called with unknown weapon name %s\n", weaponName), 0);
    ent = GetEntity(entref);
    origin[0] = 0.0f;
    origin[1] = 0.0f;
    origin[2] = 0.0f;
    tent = G_TempEntity(origin, EV_PLAY_WEAPON_DEATH_EFFECTS);
    AssignToSmallerType<short>(&tent->s.otherEntityNum, ent->s.number);
    AssignToSmallerType<unsigned short>(&tent->s.weapon, weaponIndex);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 2 )
        tent->s.eventParm = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
}
#endif

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_GetAngles)
void __cdecl ScrCmd_GetAngles(scr_entref_t entref)
{
    gentity_s *pSelf;

    pSelf = GetEntity(entref);
    Scr_AddVector(pSelf->r.currentAngles, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_GetMins)
void __cdecl ScrCmd_GetMins(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_AddVector(ent->r.mins, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_GetMaxs)
void __cdecl ScrCmd_GetMaxs(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_AddVector(ent->r.maxs, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_GetAbsMins)
void __cdecl ScrCmd_GetAbsMins(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_AddVector(ent->r.absmin, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_GetAbsMaxs)
void __cdecl ScrCmd_GetAbsMaxs(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_AddVector(ent->r.absmax, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_GetPointInBounds)
void __cdecl ScrCmd_GetPointInBounds(scr_entref_t entref)
{
    float ratioX;
    float ratioZ;
    float result[3];
    gentity_s *ent;
    float ratioY;

    ent = GetEntity(entref);
    ratioX = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    ratioY = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    ratioZ = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    G_GetEntityBoundsPoint(ent, ratioX, ratioY, ratioZ, result);
    Scr_AddVector(result, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_GetEye)
void __cdecl ScrCmd_GetEye(scr_entref_t entref)
{
    char *slStr;
    const char *fmtMsg;
    gentity_s *ent;
    float eye[3];

    ent = GetEntity(entref);
    if ( !ent->sentient )
    {
        slStr = SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("getEye must be called on an AI or player, not on a '%s'", slStr);
        Scr_Error(fmtMsg, 0);
    }
    Sentient_GetEyePosition(ent->sentient, eye);
    Scr_AddVector(eye, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_GetEyeApprox)
void __cdecl ScrCmd_GetEyeApprox(scr_entref_t entref)
{
    gentity_s *ent;
    float eye[3];

    ent = GetEntity(entref);
    eye[0] = ent->r.currentOrigin[0];
    eye[1] = ent->r.currentOrigin[1];
    eye[2] = ent->r.currentOrigin[2] + 40.0;
    Scr_AddVector(eye, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_UseBy)
void __cdecl ScrCmd_UseBy(scr_entref_t entref)
{
    gentity_s *pOther;
    void (__cdecl *use)(gentity_s *, gentity_s *, gentity_s *);
    gentity_s *pEnt;

    pEnt = GetEntity(entref);
    pOther = Scr_GetEntity(0);
    Scr_AddEntity(pOther, SCRIPTINSTANCE_SERVER);
    Scr_Notify(pEnt, scr_const.trigger, 1u);
    use = entityHandlers[pEnt->handler].use;
    if ( use )
        use(pEnt, pOther, pOther);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_IsTouching)
void __cdecl ScrCmd_IsTouching(scr_entref_t entref)
{
    PROF_SCOPED("ScrCmd_IsTouching");

    gentity_s *ent = GetEntity(entref);
    const gentity_s *otherEnt;
    // Brush/cylinder entities use the other operand as the moving bounds source.
    if ( ent->r.bmodel || (ent->r.svFlags & 0x60) != 0 )
    {
        gentity_s *brushEnt = ent;
        ent = Scr_GetEntity(0);
        if ( ent->r.bmodel || (ent->r.svFlags & 0x60) != 0 )
            Scr_Error("istouching cannot be called on 2 brush/cylinder entities", 0);
        otherEnt = brushEnt;
    }
    else
    {
        otherEnt = Scr_GetEntity(0);
    }
    if ( !ent
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 3003, 0, "%s", "ent") )
    {
        __debugbreak();
    }
    for ( int axis = 0; axis < 3; ++axis )
    {
        if ( ent->r.maxs[axis] >= ent->r.mins[axis] )
            continue;
        const char *className = SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER);
        const char *entInfo = va(
            "entnum: %d, origin: %g %g %g, classname: %s",
            ent->s.number,
            ent->r.currentOrigin[0],
            ent->r.currentOrigin[1],
            ent->r.currentOrigin[2],
            className);
        if ( !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                        3004 + axis,
                        0,
                        "%s\n\t%s",
                        "ent->r.maxs[axis] >= ent->r.mins[axis]",
                        entInfo) )
            __debugbreak();
    }
    float worldMins[3];
    float worldMaxs[3];
    worldMins[0] = ent->r.currentOrigin[0] + ent->r.mins[0];
    worldMins[1] = ent->r.currentOrigin[1] + ent->r.mins[1];
    worldMins[2] = ent->r.currentOrigin[2] + ent->r.mins[2];
    worldMaxs[0] = ent->r.currentOrigin[0] + ent->r.maxs[0];
    worldMaxs[1] = ent->r.currentOrigin[1] + ent->r.maxs[1];
    worldMaxs[2] = ent->r.currentOrigin[2] + ent->r.maxs[2];
    float extraBoundary[3] = { 0.0f, 0.0f, 0.0f };
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 1 )
        Scr_GetVector(1u, extraBoundary, SCRIPTINSTANCE_SERVER);
    worldMins[0] -= extraBoundary[0];
    worldMins[1] -= extraBoundary[1];
    worldMins[2] -= extraBoundary[2];
    worldMaxs[0] += extraBoundary[0];
    worldMaxs[1] += extraBoundary[1];
    worldMaxs[2] += extraBoundary[2];
    ExpandBoundsToWidth(worldMins, worldMaxs);
    Scr_AddInt(SV_EntityContact(worldMins, worldMaxs, otherEnt), SCRIPTINSTANCE_SERVER);
}

// LWSS ADD
void ScrCmd_IsTouchingVolume(scr_entref_t entref)
{
    ScrCmd_IsTouching(entref);
}
// LWSS END

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_IsTouchingSwept)
void __cdecl ScrCmd_IsTouchingSwept(scr_entref_t entref)
{
    PROF_SCOPED("ScrCmd_IsTouchingSwept");

    gentity_s *ent = GetEntity(entref);
    const gentity_s *otherEnt;
    if ( ent->r.bmodel || (ent->r.svFlags & 0x60) != 0 )
    {
        gentity_s *brushEnt = ent;
        ent = Scr_GetEntity(0);
        if ( ent->r.bmodel || (ent->r.svFlags & 0x60) != 0 )
            Scr_Error("istouchingswept cannot be called on 2 brush/cylinder entities", 0);
        otherEnt = brushEnt;
    }
    else
    {
        otherEnt = Scr_GetEntity(0);
    }
    if ( !ent
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 3049, 0, "%s", "ent") )
    {
        __debugbreak();
    }
    for ( int axis = 0; axis < 3; ++axis )
    {
        if ( ent->r.maxs[axis] >= ent->r.mins[axis] )
            continue;
        const char *className = SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER);
        const char *entInfo = va(
            "entnum: %d, origin: %g %g %g, classname: %s",
            ent->s.number,
            ent->r.currentOrigin[0],
            ent->r.currentOrigin[1],
            ent->r.currentOrigin[2],
            className);
        if ( !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                        3050 + axis,
                        0,
                        "%s\n\t%s",
                        "ent->r.maxs[axis] >= ent->r.mins[axis]",
                        entInfo) )
            __debugbreak();
    }
    float worldMins[3];
    float worldMaxs[3];
    worldMins[0] = ent->r.currentOrigin[0] + ent->r.mins[0];
    worldMins[1] = ent->r.currentOrigin[1] + ent->r.mins[1];
    worldMins[2] = ent->r.currentOrigin[2] + ent->r.mins[2];
    worldMaxs[0] = ent->r.currentOrigin[0] + ent->r.maxs[0];
    worldMaxs[1] = ent->r.currentOrigin[1] + ent->r.maxs[1];
    worldMaxs[2] = ent->r.currentOrigin[2] + ent->r.maxs[2];
    const int numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParam > 1 )
    {
        const float sweepStartZ = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
        worldMins[2] = (worldMins[2] - sweepStartZ < 0.0f) ? worldMins[2] : sweepStartZ;
    }
    if ( numParam > 2 )
    {
        const float sweepEndZ = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
        worldMaxs[2] = (sweepEndZ - worldMaxs[2] < 0.0f) ? worldMaxs[2] : sweepEndZ;
    }
    ExpandBoundsToWidth(worldMins, worldMaxs);
    Scr_AddInt(SV_EntityContact(worldMins, worldMaxs, otherEnt), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_SoundExists)
void ScrCmd_SoundExists()
{
    snd_alias_list_t *Alias;
    char *soundName;

    soundName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    Alias = SND_FindAlias(soundName);
    Scr_AddBool(Alias != 0, SCRIPTINSTANCE_SERVER);
}

// Decomp: BlackOps.singleplayer.c sub_7F45D0 (ScrCmd_PlaySound)
void __cdecl ScrCmd_PlaySound(scr_entref_t entref)
{
    int NumParam;
    const char *fmtMsg;
    gentity_s *Entity;
    unsigned int AliasId;
    unsigned int notifyString;

    NumParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( !NumParam || NumParam > 2 )
    {
        fmtMsg = va("playsound has %d parameters.    There should be one or two.", NumParam);
        Scr_Error(fmtMsg, 0);
    }
    AliasId = 0;
    if ( NumParam >= 1 )
        AliasId = SND_FindAliasId(Scr_GetString(0, SCRIPTINSTANCE_SERVER));
    notifyString = 0;
    if ( NumParam >= 2 )
        notifyString = Scr_GetConstString(1u, SCRIPTINSTANCE_SERVER);
    Entity = GetEntity(entref);
    if ( Entity && AliasId )
        G_PlaySoundAlias(Entity, AliasId, notifyString, 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_PlaySoundOnTag)
void __cdecl ScrCmd_PlaySoundOnTag(scr_entref_t entref)
{
    gentity_s *ent = GetEntity(entref);
    int soundAliasId = 0;
    int tagIndex = 0;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        soundAliasId = SND_FindAliasId(Scr_GetString(0, SCRIPTINSTANCE_SERVER));
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) >= 2 )
    {
        const unsigned int tag = Scr_GetConstLowercaseString(1u, SCRIPTINSTANCE_SERVER);
        if ( tag == scr_const.tag_origin || SV_DObjGetBoneIndex(ent, tag) >= 0 )
            tagIndex = G_TagIndex(Scr_GetString(1u, SCRIPTINSTANCE_SERVER));
        else
        {
            SV_DObjDumpInfo(ent);
            const unsigned int modelNameStr = G_ModelName(ent->model);
            const char *modelName = SL_ConvertToString(modelNameStr, SCRIPTINSTANCE_SERVER);
            const char *tagName = SL_ConvertToString(tag, SCRIPTINSTANCE_SERVER);
            Scr_ParamError(
                1u,
                va("tag '%s' does not exist on entity with model '%s'", tagName, modelName),
                SCRIPTINSTANCE_SERVER);
        }
    }
    if ( ent && soundAliasId )
        G_PlaySoundAlias(ent, soundAliasId, 0, tagIndex);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_PlaySoundToTeam)
void __cdecl ScrCmd_PlaySoundToTeam(scr_entref_t entref)
{
    char *slStr;
    const char *fmtMsg;
    const char *fmtMsg2;
    char *String;
    gentity_s *Entity;
    unsigned int AliasId;
    gentity_s *tempEnt;
    unsigned __int16 team;
    gentity_s *ignoreClientEnt;
    int entIndex;
    gentity_s *clientEnt;

    PROF_SCOPED("ScrCmd_PlaySoundToTeam");

    team = (unsigned __int16)Scr_GetConstString(1u, SCRIPTINSTANCE_SERVER);
    if ( team != scr_const.allies && team != scr_const.axis )
    {
        slStr = SL_ConvertToString(team, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("Illegal team string '%s'. Must be allies, or axis.", slStr);
        Scr_Error(fmtMsg, 0);
    }
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) >= 3 )
    {
        ignoreClientEnt = Scr_GetEntity(2u);
        if ( !ignoreClientEnt->client )
        {
            fmtMsg2 = va("entity %i is not a player", ignoreClientEnt->s.number);
            Scr_ObjectError(fmtMsg2, SCRIPTINSTANCE_SERVER);
        }
    }
    else
    {
        ignoreClientEnt = 0;
    }
    String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    AliasId = SND_FindAliasId(String);
    Entity = GetEntity(entref);
    tempEnt = G_PlaySoundAlias(Entity, AliasId, 0, 0);
    if ( !tempEnt )
    {
        return;
    }
    tempEnt->r.clientMask[0] = -1;
    clientEnt = g_entities;
    for ( entIndex = 0; entIndex < com_maxclients->current.integer; ++entIndex )
    {
        if ( clientEnt->r.inuse && clientEnt != ignoreClientEnt )
        {
            if ( !clientEnt->client
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                            3203,
                            0,
                            "%s",
                            "clientEnt->client") )
            {
                __debugbreak();
            }
            tempEnt->r.clientMask[clientEnt->s.number >> 5] &= ~(1 << (clientEnt->s.number & 0x1F));
        }
        ++clientEnt;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_PlayBattleChatterToTeam)
void __cdecl ScrCmd_PlayBattleChatterToTeam(scr_entref_t entref)
{
    char *slStr;
    const char *fmtMsg;
    const char *fmtMsg2;
    gentity_s *tempEnt;
    int teamNum;
    unsigned __int16 team;
    gentity_s *ignoreClientEnt;
    int entIndex;
    gentity_s *clientEnt;

    tempEnt = StartScriptPlayBattleChatterOnEnt(entref);
    team = (unsigned __int16)Scr_GetConstString(2u, SCRIPTINSTANCE_SERVER);
    if ( team != scr_const.allies && team != scr_const.axis )
    {
        slStr = SL_ConvertToString(team, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("Illegal team string '%s'. Must be allies, or axis.", slStr);
        Scr_Error(fmtMsg, 0);
    }
    if ( team == scr_const.allies )
        teamNum = 2;
    else
        teamNum = 1;
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) >= 4 )
    {
        ignoreClientEnt = Scr_GetEntity(3u);
        if ( !ignoreClientEnt->client )
        {
            fmtMsg2 = va("entity %i is not a player", ignoreClientEnt->s.number);
            Scr_ObjectError(fmtMsg2, SCRIPTINSTANCE_SERVER);
        }
    }
    tempEnt->r.clientMask[0] = -1;
    clientEnt = g_entities;
    for ( entIndex = 0; entIndex < com_maxclients->current.integer; ++entIndex )
    {
        if ( clientEnt->r.inuse )
        {
            if ( !clientEnt->client
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                            3269,
                            0,
                            "%s",
                            "clientEnt->client") )
            {
                __debugbreak();
            }
            if ( clientEnt->client->sess.cs.team == teamNum )
                tempEnt->r.clientMask[clientEnt->s.number >> 5] &= ~(1 << (clientEnt->s.number & 0x1F));
        }
        ++clientEnt;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (StartScriptPlayBattleChatterOnEnt)
gentity_s *__cdecl StartScriptPlayBattleChatterOnEnt(scr_entref_t entref)
{
    gentity_s *ent = GetEntity(entref);
    gentity_s *soundEnt = G_TempEntity(ent->r.currentOrigin, EV_SOUND_BATTLECHAT_ALIAS);
    soundEnt->r.svFlags |= 8u;
    soundEnt->s.loopSoundId = SND_FindAliasId(Scr_GetString(0, SCRIPTINSTANCE_SERVER));
    soundEnt->s.un3.item = SND_FindAliasId(Scr_GetString(1u, SCRIPTINSTANCE_SERVER));
    return soundEnt;
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_PlaySoundToPlayer)
void __cdecl ScrCmd_PlaySoundToPlayer(scr_entref_t entref)
{
    const char *fmtMsg;
    char *String;
    gentity_s *Entity;
    unsigned int AliasId;
    gentity_s *tempEnt;
    gentity_s *clientEnt;

    clientEnt = Scr_GetEntity(1u);
    if ( !clientEnt->client )
    {
        fmtMsg = va("entity %i is not a player", clientEnt->s.number);
        Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    AliasId = SND_FindAliasId(String);
    Entity = GetEntity(entref);
    tempEnt = G_PlaySoundAlias(Entity, AliasId, 0, 0);
    if ( tempEnt )
    {
        tempEnt->r.clientMask[0] = -1;
        tempEnt->r.clientMask[clientEnt->s.number >> 5] &= ~(1 << (clientEnt->s.number & 0x1F));
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PlaySoundAtPosition)
void  Scr_PlaySoundAtPosition()
{
    int NumParam;
    char *fmtMsg;
    char *String;
    unsigned int AliasId;
    float origin[3];

    if (Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2)
    {
        NumParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
        fmtMsg = va("playsoundatposition has %d parameters.  There should be two.", NumParam);
        Scr_Error(fmtMsg, 0);
    }
    Scr_GetVector(1u, origin, SCRIPTINSTANCE_SERVER);
    String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    AliasId = SND_FindAliasId(String);
    G_PlaySoundAliasAtPoint(origin, AliasId);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_PlayLoopSound)
void __cdecl ScrCmd_PlayLoopSound(scr_entref_t entref)
{
    char *String;
    const char *fmtMsg;
    float fadeTime;
    float fadeTimea;
    gentity_s *pEnt;

    pEnt = GetEntity(entref);
    pEnt->r.broadcastTime = -1;
    String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    pEnt->s.loopSoundId = SND_FindAliasId(String);
    fadeTime = 0.0f;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 2 )
    {
        fadeTimea = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
        if ( fadeTimea < 0.0 || fadeTimea > 32.0 )
        {
            fmtMsg = va("playloopsound: invalid fade value %f. it must be between 0 and 32 seconds.", fadeTimea);
            Scr_ParamError(1u, fmtMsg, SCRIPTINSTANCE_SERVER);
        }
        fadeTime = fadeTimea * 1000.0;
    }
    pEnt->s.loopSoundFade = (int)fadeTime;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetSoundBlend @ 826175A0)
void __cdecl GScr_SetSoundBlend(scr_entref_t entref)
{
    gentity_s *ent;
    const char *soundA;
    const char *soundB;
    unsigned int aliasA;
    unsigned int aliasB;
    float blend;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 3 )
        Scr_Error("setSoundBlend <soundA> <soundB> <blend>.\n", 0);
    ent = GetEntity(entref);
    if ( ent->s.eType != ET_SOUND_BLEND )
        Scr_Error("setSoundBlend can only be called on sound_blend entities\n", 0);
    soundA = Scr_GetString(0u, SCRIPTINSTANCE_SERVER);
    soundB = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    aliasA = SND_FindAliasId(soundA);
    aliasB = SND_FindAliasId(soundB);
    if ( !aliasA || !aliasB )
        Scr_Error(va("setSoundBlend called with unknown sound name %s or %s\n", soundA, soundB), 0);
    blend = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    if ( blend < 0.0f || blend > 1.0f )
        Scr_ParamError(2u, "blend must be between 0 and 1", SCRIPTINSTANCE_SERVER);
    if ( !ent->s.lerp.u.anonymous.data[1] )
    {
        ent->s.lerp.u.anonymous.data[1] = aliasA;
        ent->s.lerp.u.anonymous.data[2] = aliasB;
    }
    ent->s.anim.fTorsoPitch = blend;
    ent->s.loopSoundId = aliasB;
    ent->s.loopSoundFade = 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_StopLoopSound)
void __cdecl ScrCmd_StopLoopSound(scr_entref_t entref)
{
    const char *fmtMsg;
    float fadeTime;
    float fadeTimea;
    gentity_s *pEnt;

    pEnt = GetEntity(entref);
    pEnt->r.broadcastTime = level.time + 300;
    fadeTime = 0.0f;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        fadeTimea = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
        if ( fadeTimea < 0.0 || fadeTimea > 32.0 )
        {
            fmtMsg = va("stoploopsound: invalid fade value %f. it must be between 0 and 32 seconds.", fadeTimea);
            Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
        }
        fadeTime = fadeTimea * -1000.0;
    }
    else
    {
        pEnt->s.loopSoundId = 0;
    }
    pEnt->s.loopSoundFade = (int)fadeTime;
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_StopSound) — stops alias on entity via EV_STOP_SOUND_ALIAS temp ent.
void __cdecl ScrCmd_StopSound(scr_entref_t entref)
{
    gentity_s *ent;
    gentity_s *tmp;
    unsigned int aliasId;
    char *soundName;

    ent = GetEntity(entref);
    soundName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    aliasId = SND_FindAliasId(soundName);
    if ( aliasId )
    {
        tmp = G_TempEntity(ent->r.currentOrigin, EV_STOP_SOUND_ALIAS);
        tmp->s.loopSoundId = aliasId;
        AssignToSmallerType<short>(&tmp->s.otherEntityNum, ent->s.number);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_StopSounds)
void __cdecl ScrCmd_StopSounds(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    G_AddEvent(ent, EV_STOPSOUNDS, 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_IsWaitingOnSound) — notify-string sound wait not ported yet; safe false.
void __cdecl ScrCmd_IsWaitingOnSound(scr_entref_t entref)
{
    GetEntity(entref);
    Scr_AddBool(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_Delete)
void __cdecl ScrCmd_Delete(scr_entref_t entref)
{
    gentity_s *pEnt;

    pEnt = GetEntity(entref);
    if ( pEnt->client )
        Scr_Error("Cannot delete a client entity", 0);
    if ( level.currentEntityThink == pEnt->s.number )
        Scr_Error("Cannot delete entity during its think", 0);
    Scr_Notify(pEnt, scr_const.death, 0);
    G_FreeEntity(pEnt);
}

// Decomp: CoDSP_rdBlackOps.map.c (SetModelInternal)
void __cdecl SetModelInternal(gentity_s *ent, char *modelName)
{
    DObj *obj;

    G_SetModel(ent, modelName);
    G_DObjUpdate(ent);
    if ( ent->s.eType == 6 )
    {
        obj = Com_GetServerDObj(ent->s.number);
        if ( obj )
        {
            ent->r.contents |= DObjGetContents(obj);
            DObjCalcBounds(obj, ent->r.mins, ent->r.maxs);
        }
    }
    SV_LinkEntity(ent);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_SetModel)
void __cdecl ScrCmd_SetModel(scr_entref_t entref)
{
    char *modelName;
    gentity_s *pEnt;

    pEnt = GetEntity(entref);
    modelName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    SetModelInternal(pEnt, modelName);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_SetEnemyModel)
void __cdecl ScrCmd_SetEnemyModel(scr_entref_t entref)
{
    char *modelName;
    gentity_s *pEnt;
    int modelIndex;

    pEnt = GetEntity(entref);
    modelName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( *modelName )
    {
        modelIndex = G_ModelIndex(modelName);
        if ( modelIndex != (unsigned __int16)modelIndex
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                        3558,
                        0,
                        "%s",
                        "modelIndex == (modelNameIndex_t) modelIndex") )
        {
            __debugbreak();
        }
        pEnt->s.enemyModel = modelIndex;
    }
    else
    {
        pEnt->s.enemyModel = 0;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_GetNormalHealth)
void __cdecl ScrCmd_GetNormalHealth(scr_entref_t entref)
{
    gentity_s *pEnt;

    pEnt = GetEntity(entref);
    if ( pEnt->client )
    {
        if ( pEnt->health )
            Scr_AddFloat((float)pEnt->health / (float)pEnt->client->sess.maxHealth, SCRIPTINSTANCE_SERVER);
        else
            Scr_AddFloat(0.0, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddFloat((float)pEnt->health, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_SetNormalHealth)
void __cdecl ScrCmd_SetNormalHealth(scr_entref_t entref)
{
    const char *fmtMsg;
    int newHealth;
    float normalHealth;
    gentity_s *ent;

    ent = GetEntity(entref);
    normalHealth = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    if ( normalHealth > 1.0 )
        normalHealth = 1.0f;
    if ( ent->client )
    {
        newHealth = (int)((float)((float)ent->client->sess.maxHealth * normalHealth) + 9.313225746154785e-10);
        fmtMsg = va("%c \"%i\"", 74, 0);
        SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg);
    }
    else if ( ent->maxHealth )
    {
        newHealth = (int)(float)((float)ent->maxHealth * normalHealth);
    }
    else
    {
        newHealth = (int)normalHealth;
    }
    if ( newHealth > 0 )
        ent->health = newHealth;
    else
        Com_PrintError(24, "ERROR: Cannot setnormalhealth to 0 or below.\n");
}

// Decomp: CoDSP_rdBlackOps.map.c (825FEA78 ScrCmd_SetMaxHealth)
void __cdecl ScrCmd_SetMaxHealth(scr_entref_t entref)
{
    int maxHealth;
    gentity_s *ent;

    ent = GetEntity(entref);
    maxHealth = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    ent->health = maxHealth;
    ent->maxHealth = maxHealth;
    if ( ent->client )
        ent->client->sess.maxHealth = maxHealth;
}

// Decomp: BlackOps.singleplayer.c sub_7F5080
void __cdecl ScrCmd_DoDamage(scr_entref_t entref)
{
    PROF_SCOPED("ScrCmd_DoDamage");

    const int numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParam < 2 || numParam > 6 )
    {
        Scr_Error(
            "Usage: doDamage( <health>, <source position>, <attacker>, <destructible_piece_index>, <means of death> )\n",
            0);
        return;
    }

    const float damage = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    float source[3];
    Scr_GetVector(1u, source, SCRIPTINSTANCE_SERVER);

    gentity_s *attacker = 0;
    int destructiblePieceIndex = -1;
    meansOfDeath_t mod = MOD_UNKNOWN;
    int dflags = 0;
    int weapon = -1;

    if ( numParam >= 3 )
    {
        const int attackerType = Scr_GetType(2u, SCRIPTINSTANCE_SERVER);
        if ( attackerType == VAR_ENTITY || attackerType == VAR_POINTER )
            attacker = Scr_GetEntityAllowNull(2u, SCRIPTINSTANCE_SERVER);
    }
    if ( numParam >= 4 && Scr_GetType(3u, SCRIPTINSTANCE_SERVER) == VAR_INTEGER )
        destructiblePieceIndex = Scr_GetInt(3u, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 5 && Scr_GetType(4u, SCRIPTINSTANCE_SERVER) == VAR_STRING )
        mod = (meansOfDeath_t)G_MeansOfDeathFromScriptParam(4u);
    if ( numParam >= 6 && Scr_GetType(5u, SCRIPTINSTANCE_SERVER) == VAR_STRING )
        weapon = G_GetWeaponIndexForName(Scr_GetString(5u, SCRIPTINSTANCE_SERVER));

    gentity_s *ent = GetEntity(entref);
    if ( IS_NAN(source[0]) || IS_NAN(source[1]) || IS_NAN(source[2]) )
    {
        Scr_Error(va("Source Damage vector is invalid : %f %f %f", source[0], source[1], source[2]), 0);
        return;
    }

    float from[3];
    if ( ent->client )
    {
        if ( (IS_NAN(ent->client->ps.origin[0]) || IS_NAN(ent->client->ps.origin[1]) || IS_NAN(ent->client->ps.origin[2]))
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                        3704,
                        0,
                        "%s",
                        "!IS_NAN((ent->client->ps.origin)[0]) && !IS_NAN((ent->client->ps.origin)[1]) && !IS_NAN((ent->client->ps.origin)[2])") )
        {
            __debugbreak();
        }
        from[0] = ent->client->ps.origin[0] - source[0];
        from[1] = ent->client->ps.origin[1] - source[1];
        from[2] = ent->client->ps.origin[2] - source[2];
    }
    else
    {
        if ( (IS_NAN(ent->r.currentOrigin[0]) || IS_NAN(ent->r.currentOrigin[1]) || IS_NAN(ent->r.currentOrigin[2]))
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                        3709,
                        0,
                        "%s",
                        "!IS_NAN((ent->r.currentOrigin)[0]) && !IS_NAN((ent->r.currentOrigin)[1]) && !IS_NAN((ent->r.currentOrigin)[2])") )
        {
            __debugbreak();
        }
        from[0] = ent->r.currentOrigin[0] - source[0];
        from[1] = ent->r.currentOrigin[1] - source[1];
        from[2] = ent->r.currentOrigin[2] - source[2];
    }
    if ( (IS_NAN(from[0]) || IS_NAN(from[1]) || IS_NAN(from[2]))
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    3713,
                    0,
                    "%s",
                    "!IS_NAN((from)[0]) && !IS_NAN((from)[1]) && !IS_NAN((from)[2])") )
    {
        __debugbreak();
    }

    float *dir = 0;
    if ( Vec3Normalize(from) != 0.0 )
        dir = from;

    float damageAmount = damage;
    if ( ent->destructible )
    {
        const int destructibleMod = mod ? mod : MOD_EXPLOSIVE;
        if ( destructiblePieceIndex < 0 )
            DestructibleRadiusDamage(ent, source, damageAmount, 10.0, 400.0, destructibleMod, attacker);
        else
            damageAmount = (float)DestructibleDamage(
                ent,
                attacker,
                dir,
                source,
                (int)damageAmount,
                mod,
                0,
                destructiblePieceIndex);
        dflags |= 0x10u;
    }
    G_Damage(ent, attacker, attacker, dir, source, (int)damageAmount, dflags, mod, weapon, HITLOC_NONE, 0, 0, 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_GetVelocity)
void __cdecl ScrCmd_GetVelocity(scr_entref_t entref)
{
    scr_vehicle_s *scr_vehicle;
    gclient_s *client;
    float velocity[3];
    gentity_s *ent;

    memset(velocity, 0, sizeof(velocity));
    ent = GetEntity(entref);
    if ( ent->sentient )
    {
        Sentient_GetVelocity(ent->sentient, velocity);
    }
    else if ( ent->scr_vehicle )
    {
        scr_vehicle = ent->scr_vehicle;
        velocity[0] = scr_vehicle->phys.vel[0];
        velocity[1] = scr_vehicle->phys.vel[1];
        velocity[2] = scr_vehicle->phys.vel[2];
    }
    else if ( ent->client )
    {
        client = ent->client;
        velocity[0] = client->ps.velocity[0];
        velocity[1] = client->ps.velocity[1];
        velocity[2] = client->ps.velocity[2];
    }
    else
    {
        velocity[0] = ent->s.lerp.pos.trDelta[0];
        velocity[1] = ent->s.lerp.pos.trDelta[1];
        velocity[2] = ent->s.lerp.pos.trDelta[2];
    }
    Scr_AddVector(velocity, SCRIPTINSTANCE_SERVER);
}

// Map: CoDSP_rd.map getaivelocity @ g_scr_main.obj (retail s_builtinMethods)
void ScrCmd_GetAiVelocity(scr_entref_t entref)
{
    actor_s *actor;
    gentity_s *ent;
    float velocity[3];

    ent = GetEntity(entref);
    actor = ent->actor;
    if ( !actor )
    {
        Scr_Error("getAiVelocity must be called on an AI entity.\n", SCRIPTINSTANCE_SERVER);
        return;
    }
    velocity[0] = actor->Physics.vVelocity[0];
    velocity[1] = actor->Physics.vVelocity[1];
    velocity[2] = actor->Physics.vVelocity[2];
    Scr_AddVector(velocity, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_FakeFire)
void __cdecl GScr_FakeFire(scr_entref_t entref)
{
    const char *fmtMsg;
    float origin[3];
    int iWeaponIndex;
    gentity_s *owner;
    gentity_s *ent;
    const char *pszWeaponName;
    int argc;

    ent = GetEntity(entref);
    argc = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( argc != 4 )
        Scr_Error("FakeFire( <owner>, <origin>, <weapon>, <shot count> ) takes 4 parameters", 0);
    owner = Scr_GetEntity(0);
    if ( !owner->client )
        Scr_ParamError(0, "Owner entity is not a player", SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, origin, SCRIPTINSTANCE_SERVER);
    pszWeaponName = Scr_GetString(2u, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = G_GetWeaponIndexForName((char *)pszWeaponName);
    if ( !iWeaponIndex )
    {
        if ( *pszWeaponName )
        {
            fmtMsg = va("Invalid weapon name %s", pszWeaponName);
            Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
        }
        else
        {
            Scr_ParamError(0, "Invalid weapon name", SCRIPTINSTANCE_SERVER);
        }
    }
    ent->s.lerp.eFlags |= 0x40u;
    if ( ent->s.eType == 1 )
    {
        ent = G_TempEntity(origin, EV_FAKE_FIRE);
        ent->s.eventParm = iWeaponIndex;
    }
    else
    {
        G_AddEvent(ent, 0x45u, iWeaponIndex);
    }
    ent->s.otherEntityNum = owner->s.number;
    ent->s.un1.scale = (unsigned __int8)Scr_GetInt(3u, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetCameraSpikeActive)
void __cdecl GScr_SetCameraSpikeActive(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->client )
    {
        if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        {
            if ( BG_GetWeaponDef(ent->client->ps.weapon)->guidedMissileType != MISSILE_GUIDANCE_TVGUIDED
                || ent->client->ps.fWeaponPosFrac < 0.5 )
            {
                if ( Scr_GetInt(0, SCRIPTINSTANCE_SERVER) )
                    ent->client->ps.weapFlags |= 0x200000u;
                else
                    ent->client->ps.weapFlags &= ~0x200000u;
            }
        }
        else
        {
            Scr_Error("USAGE: <toggle> not specified\n", 0);
        }
    }
    else
    {
        Scr_Error("USAGE: Must be called on a client\n", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_MakeUsable)
void __cdecl ScrCmd_MakeUsable(scr_entref_t entref)
{
    __int16 team;
    char *teamString;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->s.eType == 14 )
        G_MakeVehicleUsable(ent);
    else
        ent->r.contents |= 0x200000u;
    if ( ent->s.eType == 6 && Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        teamString = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        if ( I_stricmp(teamString, "allies") )
            team = I_stricmp(teamString, "axis") == 0;
        else
            team = 2;
        ent->s.eventParm &= 0x3FFFu;
        ent->s.eventParm |= team << 14;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_MakeUnusable)
void __cdecl ScrCmd_MakeUnusable(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->s.eType == ET_VEHICLE )
    {
        // Same sequence as CMD_VEH_MakeVehicleUnusable: refresh tags, then clear use.
        G_MakeVehicleUsable(ent);
        ent->spawnflags &= ~1u;
        ent->r.contents &= ~0x200000u;
    }
    else
    {
        ent->r.contents &= ~0x200000u;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_Show)
void __cdecl ScrCmd_Show(scr_entref_t entref)
{
    gentity_s *pEnt;

    pEnt = GetEntity(entref);
    pEnt->s.lerp.eFlags &= ~0x20u;
    if ( pEnt->client )
        pEnt->client->ps.eFlags &= ~0x20u;
    pEnt->r.clientMask[0] = 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_Hide)
void __cdecl ScrCmd_Hide(scr_entref_t entref)
{
    gentity_s *pEnt;

    pEnt = GetEntity(entref);
    pEnt->s.lerp.eFlags |= 0x20u;
    if ( pEnt->client )
        pEnt->client->ps.eFlags |= 0x20u;
    pEnt->r.clientMask[0] = -1;
    if ( pEnt->s.eType == 14 )
        G_HideVehicle(pEnt);
    G_ClearGroundEntityRefs(pEnt);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_Ghost)
void __cdecl ScrCmd_Ghost(scr_entref_t entref)
{
    gentity_s *pEnt;

    pEnt = GetEntity(entref);
    pEnt->s.lerp.eFlags |= 0x20u;
    if ( pEnt->client )
        pEnt->client->ps.eFlags |= 0x20u;
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_ShowToPlayer)
void __cdecl ScrCmd_ShowToPlayer(scr_entref_t entref)
{
    gentity_s *pEnt;
    gentity_s *clientEnt;

    pEnt = GetEntity(entref);
    clientEnt = Scr_GetEntity(0);
    if ( clientEnt->s.number < 32 )
    {
        pEnt->s.lerp.eFlags &= ~0x20u;
        pEnt->r.clientMask[clientEnt->s.number >> 5] &= ~(1 << (clientEnt->s.number & 0x1F));
    }
    else
    {
        Scr_Error("showToClient error: param must be a client entity\n", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_SetContents)
void __cdecl ScrCmd_SetContents(scr_entref_t entref)
{
    int contents;
    gentity_s *ent;

    ent = GetEntity(entref);
    contents = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    Scr_AddInt(ent->r.contents, SCRIPTINSTANCE_SERVER);
    ent->r.contents = contents;
    SV_LinkEntity(ent);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_StartFiring)
void __cdecl GScr_StartFiring(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_GetTurretInfo(ent)->flags |= 4u;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_StopFiring)
void __cdecl GScr_StopFiring(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_GetTurretInfo(ent)->flags &= ~4u;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ShootTurret)
void __cdecl GScr_ShootTurret(scr_entref_t entref)
{
    gentity_s *ent;

    PROF_SCOPED("shootturret");
    ent = GetEntity(entref);
    if ( !ent->pTurretInfo )
    {
        Scr_Error(va("entity type '%s' is not a turret", SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER)), 0);
    }
    turret_shoot(ent);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_StopShootTurret)
void __cdecl GScr_StopShootTurret(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_GetTurretInfo(ent);
    ent->s.lerp.eFlags &= ~0x40u;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetMode)
void __cdecl GScr_SetMode(scr_entref_t entref)
{
    TurretInfo *turretInfo;
    gentity_s *ent;
    unsigned int mode;

    ent = GetEntity(entref);
    turretInfo = Scr_GetTurretInfo(ent);
    mode = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    if ( mode == scr_const.auto_ai )
    {
        turretInfo->flags &= 0xFFFFFFFC;
        turretInfo->flags |= 3u;
    }
    else if ( mode == scr_const.manual )
    {
        turretInfo->flags &= 0xFFFFFFFC;
    }
    else if ( mode == scr_const.manual_ai )
    {
        turretInfo->flags &= 0xFFFFFFFC;
        turretInfo->flags |= 1u;
    }
    else if ( mode == scr_const.auto_nonai )
    {
        turretInfo->flags &= 0xFFFFFFFC;
        turretInfo->flags |= 2u;
    }
    else
    {
        Scr_Error("Error setting the mode of a turret.\n", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetTurretOwner)
void __cdecl GScr_GetTurretOwner(scr_entref_t entref)
{
    gentity_s *ent;
    gentity_s *ownerEnt;

    ent = GetEntity(entref);
    Scr_GetTurretInfo(ent);
    if ( ent->active )
    {
        if ( ent->r.ownerNum.isDefined() )
        {
            ownerEnt = ent->r.ownerNum.ent();
            Scr_AddEntity(ownerEnt, SCRIPTINSTANCE_SERVER);
        }
        else
        {
            Scr_AddEntity(&g_entities[1023], SCRIPTINSTANCE_SERVER);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetTargetEntity)
void __cdecl GScr_SetTargetEntity(scr_entref_t entref)
{
    gentity_s *ent;
    gentity_s *targetEnt;

    ent = GetEntity(entref);
    targetEnt = Scr_GetEntity(0);
    Scr_GetTurretInfo(ent)->manualTarget.setEnt(targetEnt);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetAiSpread)
void __cdecl GScr_SetAiSpread(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_GetTurretInfo(ent)->aiSpread = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetPlayerSpread)
void __cdecl GScr_SetPlayerSpread(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_GetTurretInfo(ent)->playerSpread = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetConvergenceTime)
void __cdecl GScr_SetConvergenceTime(scr_entref_t entref)
{
    TurretInfo *turretInfo;
    char *typeString;
    gentity_s *ent;
    int axis;

    ent = GetEntity(entref);
    turretInfo = Scr_GetTurretInfo(ent);
    axis = 1; // default: yaw
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 1 )
    {
        typeString = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
        if ( !strcmp(typeString, "yaw") )
            axis = 1;
        else if ( !strcmp(typeString, "pitch") )
            axis = 0;
        else
            Scr_Error("Convergence type should be either 'pitch' or 'yaw'", 0);
    }
    turretInfo->convergenceTime[axis] = (int)(Scr_GetFloat(0, SCRIPTINSTANCE_SERVER) * 1000.0);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetSuppressionTime)
void __cdecl GScr_SetSuppressionTime(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_GetTurretInfo(ent)->suppressTime = (int)(Scr_GetFloat(0, SCRIPTINSTANCE_SERVER) * 1000.0);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ClearTargetEntity)
void __cdecl GScr_ClearTargetEntity(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_GetTurretInfo(ent)->manualTarget.setEnt(0);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetTurretTeam)
void __cdecl GScr_SetTurretTeam(scr_entref_t entref)
{
    TurretInfo *turretInfo;
    gentity_s *ent;
    char *pszTeam;

    pszTeam = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    ent = GetEntity(entref);
    turretInfo = Scr_GetTurretInfo(ent);
    ent->s.faction.iHeadIconTeam &= ~3u;
    if ( !I_stricmp(pszTeam, "axis") )
    {
        turretInfo->eTeam = TEAM_AXIS;
        ent->s.faction.iHeadIconTeam |= 1u;
    }
    else if ( !I_stricmp(pszTeam, "allies") )
    {
        turretInfo->eTeam = TEAM_ALLIES;
        ent->s.faction.iHeadIconTeam |= 2u;
    }
    else if ( !I_stricmp(pszTeam, "free") )
    {
        turretInfo->eTeam = TEAM_FREE;
    }
    else
    {
        Scr_Error(va("unknown team '%s', should be 'axis' or 'allies' or 'free'\n", pszTeam), 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetTurretIgnoreGoals)
void __cdecl GScr_SetTurretIgnoreGoals(scr_entref_t entref)
{
    TurretInfo *turretInfo;
    gentity_s *ent;

    ent = GetEntity(entref);
    turretInfo = Scr_GetTurretInfo(ent);
    if ( Scr_GetInt(0, SCRIPTINSTANCE_SERVER) )
        turretInfo->flags |= 0x2000u;
    else
        turretInfo->flags &= ~0x2000u;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_MakeTurretUsable)
void __cdecl GScr_MakeTurretUsable(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_GetTurretInfo(ent)->flags |= 0x1000u;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_MakeTurretUnusable)
void __cdecl GScr_MakeTurretUnusable(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_GetTurretInfo(ent)->flags &= ~0x1000u;
}

// Decomp: CoDSP_rdBlackOps.map.c:9328
void __cdecl GScr_SetTurretAccuracy(scr_entref_t entref)
{
    Com_PrintWarning(24, "WARNING: Turret Accuracy no longer has any effect\n");
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetTurretTarget)
void __cdecl GScr_GetTurretTarget(scr_entref_t entref)
{
    TurretInfo *turretInfo;
    gentity_s *ent;
    gentity_s *targetEnt;

    ent = GetEntity(entref);
    turretInfo = Scr_GetTurretInfo(ent);
    if ( turretInfo->target.isDefined() && (turretInfo->flags & 0x40) != 0 )
    {
        targetEnt = turretInfo->target.ent();
        Scr_AddEntity(targetEnt, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_DisconnectPaths)
void __cdecl GScr_DisconnectPaths(scr_entref_t entref)
{
    char *slStr;
    const char *fmtMsg;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !Path_IsDynamicBlockingEntity(ent) )
    {
        if ( ent->classname == scr_const.script_brushmodel )
            Scr_Error("script_brushmodel must have DYNAMICPATH set to disconnect paths", 0);
        slStr = SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("entity of type '%s' cannot disconnect paths.\n \n ", slStr);
        Scr_Error(fmtMsg, 0);
    }
    Path_DisconnectPathsForEntity(ent);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ConnectPaths)
void __cdecl GScr_ConnectPaths(scr_entref_t entref)
{
    char *slStr;
    const char *fmtMsg;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !Path_IsDynamicBlockingEntity(ent) )
    {
        if ( ent->classname == scr_const.script_brushmodel )
            Scr_Error("script_brushmodel must have DYNAMICPATH set to connect paths", 0);
        slStr = SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("entity of type '%s' cannot connect paths \n\n", slStr);
        Scr_Error(fmtMsg, 0);
    }
    Path_ConnectPathsForEntity(ent);
}

// LWSS ADD
void __cdecl ScrCmd_SetStance(scr_entref_t entref)
{
    gentity_s *ent;
    unsigned short stance;

    ent = GetEntity(entref);
    stance = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);

    if (!ent->client)
    {
        Scr_Error("SetStance is only defined for players.", SCRIPTINSTANCE_SERVER);
        return;
    }

    if (stance == scr_const.stand)
    {
        ent->client->ps.pm_flags &= ~3;
        ent->client->ps.viewHeightTarget = 60;
        G_AddEvent(ent, 8, SCRIPTINSTANCE_SERVER);
    }
    else if (stance == scr_const.crouch)
    {
        ent->client->ps.pm_flags = (ent->client->ps.pm_flags & ~3) | 2;
        ent->client->ps.viewHeightTarget = 40;
        G_AddEvent(ent, 9, SCRIPTINSTANCE_SERVER);
    }
    else if (stance == scr_const.prone)
    {
        if (!(ent->client->ps.pm_flags & 1))
            ent->client->ps.proneDirection = ent->client->ps.viewHeightCurrent;
        ent->client->ps.pm_flags = (ent->client->ps.pm_flags & ~3) | 1;
        ent->client->ps.viewHeightTarget = 11;
        G_AddEvent(ent, 10, SCRIPTINSTANCE_SERVER);
    }
}
// LWSS END

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_GetStance)
void __cdecl ScrCmd_GetStance(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->client )
    {
        if ( (ent->client->ps.pm_flags & 1) != 0 )
        {
            Scr_AddConstString(scr_const.prone, SCRIPTINSTANCE_SERVER);
        }
        else if ( (ent->client->ps.pm_flags & 2) != 0 )
        {
            Scr_AddConstString(scr_const.crouch, SCRIPTINSTANCE_SERVER);
        }
        else
        {
            Scr_AddConstString(scr_const.stand, SCRIPTINSTANCE_SERVER);
        }
    }
    else
    {
        Scr_Error("GetStance is only defined for players.", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetStableMissile)
void __cdecl Scr_SetStableMissile(scr_entref_t entref)
{
    unsigned int tempValue1;
    int stableMissile;
    gentity_s *ent;

    ent = GetEntity(entref);
    stableMissile = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( ent->s.eType != 1 )
        Scr_Error("Type should be a player", 0);
    if ( stableMissile )
        tempValue1 = ent->flags | 0x20000;
    else
        tempValue1 = ent->flags & 0xFFFDFFFF;
    ent->flags = tempValue1;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetCursorHint)
void __cdecl GScr_SetCursorHint(scr_entref_t entref)
{
    const char *fmtMsg;
    char *pszHint;
    gentity_s *pEnt;
    int i;
    int ia;

    pEnt = GetEntity(entref);
    if ( pEnt->s.eType == 4
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    4687,
                    0,
                    "%s",
                    "pEnt->s.eType != ET_MISSILE") )
    {
        __debugbreak();
    }
    pszHint = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( (pEnt->classname == scr_const.trigger_use
         || pEnt->classname == scr_const.trigger_use_touch
         || pEnt->classname == scr_const.trigger_radius
         || pEnt->classname == scr_const.trigger_radius_use)
        && !I_stricmp(pszHint, "HINT_INHERIT") )
    {
        pEnt->s.un3.item = -1;
    }
    else
    {
        for ( i = 1; i < 8; ++i )
        {
            if ( !I_stricmp(pszHint, hintStrings[i]) )
            {
                pEnt->s.un3.item = i;
                return;
            }
        }
        Com_Printf(24, "List of valid hint type strings\n");
        if ( pEnt->classname == scr_const.trigger_use
            || pEnt->classname == scr_const.trigger_use_touch
            || pEnt->classname == scr_const.trigger_radius
            || pEnt->classname == scr_const.trigger_radius_use )
        {
            Com_Printf(24, "HINT_INHERIT (for trigger_use or trigger_use_touch or trigger_radius entities only)\n");
        }
        for ( ia = 1; ia < 8; ++ia )
            Com_Printf(24, "%s\n", hintStrings[ia]);
        fmtMsg = va("%s is not a valid hint type. See above for list of valid hint types\n", pszHint);
        Scr_Error(fmtMsg, 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (G_GetHintStringIndex)
int __cdecl G_GetHintStringIndex(int *piIndex, char *pszString)
{
    char szConfigString[1024];
    int i;

    for ( i = 0; i < 96; ++i )
    {
        SV_GetConfigstring(i + 419, szConfigString, 1024);
        if ( !szConfigString[0] )
        {
            SV_SetConfigstring(i + 419, pszString);
            *piIndex = i;
            return 1;
        }
        if ( !strcmp(pszString, szConfigString) )
        {
            *piIndex = i;
            return 1;
        }
    }
    *piIndex = -1;
    return 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetHintString)
void __cdecl GScr_SetHintString(scr_entref_t entref)
{
    char *String;
    int NumParam;
    const char *fmtMsg;
    char szHint[1024];
    int type;
    gentity_s *pEnt;
    int i;

    pEnt = GetEntity(entref);
    if ( pEnt->classname != scr_const.trigger_use
        && pEnt->classname != scr_const.trigger_use_touch
        && pEnt->classname != scr_const.trigger_radius
        && pEnt->classname != scr_const.trigger_radius_use
        && pEnt->s.eType != 6 )
    {
        Scr_Error(
            "The setHintString command only works on trigger_use, trigger_radius, trigger_radius_use, trigger_use_touch, or scr"
            "ipt mover entities.\n",
            0);
    }
    type = Scr_GetType(0, SCRIPTINSTANCE_SERVER);
    if ( type != 2 || (String = Scr_GetString(0, SCRIPTINSTANCE_SERVER), I_stricmp(String, "")) )
    {
        NumParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
        Scr_ConstructMessageString(0, NumParam - 1, "Hint String", szHint, 0x400u);
        if ( !G_GetHintStringIndex(&i, szHint) )
        {
            fmtMsg = va("Too many different hintstring values. Max allowed is %i different strings", 96);
            Scr_Error(fmtMsg, 0);
        }
        pEnt->s.un1.scale = i;
    }
    else
    {
        pEnt->s.un1.scale = -1;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetHintStringForPerk)
void __cdecl GScr_SetHintStringForPerk(scr_entref_t entref)
{
    char *String;
    char *tempValue2;
    const char *fmtMsg;
    int hintIndex;
    char szHint[1024];
    int perkIndex;
    int type;
    gentity_s *pEnt;

    pEnt = GetEntity(entref);
    if ( pEnt->s.eType != 6 )
        Scr_Error("The SetHintStringForPerk command only works on script mover entities.\n", 0);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2 )
        Scr_Error("SetHintStringForPerk Usage: <perk>, <hint string>", 0);
    String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    perkIndex = BG_GetPerkIndexForName(String);
    pEnt->s.eventParm &= 0xC000u;
    type = Scr_GetType(1u, SCRIPTINSTANCE_SERVER);
    if ( type != 2 || (tempValue2 = Scr_GetString(1u, SCRIPTINSTANCE_SERVER), I_stricmp(tempValue2, "")) )
    {
        Scr_ConstructMessageString(1, 1, "Hint String", szHint, 0x400u);
        if ( !G_GetHintStringIndex(&hintIndex, szHint) )
        {
            fmtMsg = va("Too many different hintstring values. Max allowed is %i different strings", 96);
            Scr_Error(fmtMsg, 0);
        }
        if ( perkIndex >= 64
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                        4832,
                        0,
                        "%s",
                        "perkIndex < 64") )
        {
            __debugbreak();
        }
        if ( perkIndex >= 64 )
            perkIndex = 0;
        pEnt->s.eventParm |= (_WORD)perkIndex << 8;
        pEnt->s.eventParm |= (unsigned __int8)hintIndex;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetHintLowPriority)
void __cdecl GScr_SetHintLowPriority(scr_entref_t entref)
{
    unsigned int tempValue1;
    gentity_s *pEnt;

    pEnt = GetEntity(entref);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("SetHintLowPriority Usage: <bool>", 0);
    if ( Scr_GetInt(0, SCRIPTINSTANCE_SERVER) )
        tempValue1 = pEnt->flags | 0x200;
    else
        tempValue1 = pEnt->flags & 0xFFFFFDFF;
    pEnt->flags = tempValue1;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetReviveHintString)
void __cdecl GScr_SetReviveHintString(scr_entref_t entref)
{
    char *String;
    const char *fmtMsg;
    char *team;
    char szHint[1024];
    int type;
    gentity_s *pEnt;
    int i;

    pEnt = GetEntity(entref);
    if ( pEnt->classname != scr_const.trigger_use
        && pEnt->classname != scr_const.trigger_use_touch
        && pEnt->classname != scr_const.trigger_radius )
    {
        Scr_Error("The setHintString command only works on trigger_use, trigger_radius or trigger_use_touch entities.\n", 0);
    }
    type = Scr_GetType(0, SCRIPTINSTANCE_SERVER);
    if ( type != 2 || (String = Scr_GetString(0, SCRIPTINSTANCE_SERVER), I_stricmp(String, "")) )
    {
        if ( pEnt->classname == scr_const.trigger_radius )
        {
            pEnt->team = 0;
            if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 1 )
            {
                team = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
                if ( !strcmp(team, "allies") )
                {
                    pEnt->team = 2;
                }
                else if ( !strcmp(team, "axis") )
                {
                    pEnt->team = 1;
                }
            }
        }
        Scr_ConstructMessageString(0, 0, "Hint String", szHint, 0x400u);
        if ( !G_GetHintStringIndex(&i, szHint) )
        {
            fmtMsg = va("Too many different hintstring values. Max allowed is %i different strings", 96);
            Scr_Error(fmtMsg, 0);
        }
        pEnt->s.un1.scale = i;
    }
    else
    {
        pEnt->s.un1.scale = -1;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_UseTriggerRequireLookAt)
void __cdecl GScr_UseTriggerRequireLookAt(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->classname != scr_const.trigger_use && ent->classname != scr_const.trigger_radius_use )
        Scr_Error("The UseTriggerRequireLookAt command only works on trigger_use and trigger_radius_use entities.\n", 0);
    ent->trigger.requireLookAt = 1;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsMartyrdomGrenade)
void __cdecl GScr_IsMartyrdomGrenade(scr_entref_t entref)
{
    gentity_s *pEnt;

    pEnt = GetEntity(entref);
    Scr_AddBool((pEnt->flags & 0x8000) != 0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetEntityNumber)
void __cdecl GScr_GetEntityNumber(scr_entref_t entref)
{
    gentity_s *pEnt;

    pEnt = GetEntity(entref);
    Scr_AddInt(pEnt->s.number, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_EnableGrenadeTouchDamage)
void __cdecl GScr_EnableGrenadeTouchDamage(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->classname != scr_const.trigger_damage )
        Scr_Error("Currently on supported on damage triggers", 0);
    ent->flags |= 0x4000u;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_DisableGrenadeTouchDamage)
void __cdecl GScr_DisableGrenadeTouchDamage(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->classname != scr_const.trigger_damage )
        Scr_Error("Currently on supported on damage triggers", 0);
    ent->flags &= ~0x4000u;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_MissileSetTarget)
void __cdecl GScr_MissileSetTarget(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *Entity;
    gentity_s *missile;

    missile = GetEntity(entref);
    if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) )
        Entity = Scr_GetEntity(0);
    else
        Entity = 0;
    if ( missile->classname != scr_const.rocket )
    {
        fmtMsg = va("Entity %i is not a rocket\n", missile->s.number);
        Scr_Error(fmtMsg, 0);
    }
    missile->missileTargetEnt.setEnt(Entity);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 1 )
    {
        missile->mover.pos3[2] = 0.0f;
        missile->mover.apos1[0] = 0.0f;
        missile->mover.apos1[1] = 0.0f;
    }
    else
    {
        Scr_GetVector(1u, &missile->mover.pos3[2], SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_EnableAimAssist)
void __cdecl GScr_EnableAimAssist(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->r.bmodel )
        Scr_Error("Currently only supported on entities with brush models", 0);
    ent->s.lerp.eFlags |= 0x800u;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_DisableAimAssist)
void __cdecl GScr_DisableAimAssist(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->r.bmodel )
        Scr_Error("Currently only supported on entities with brush models", 0);
    ent->s.lerp.eFlags &= ~0x800u;
}

// Decomp: CoDSP_rdBlackOps.map.c
static TurretInfo *Scr_GetTurretInfo(gentity_s *ent)
{
    if ( !ent->pTurretInfo )
    {
        Scr_Error(
            va("entity type '%s' is not a turret", SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER)),
            0);
    }
    return ent->pTurretInfo;
}

void __cdecl G_InitObjectives()
{
    int i;

    for ( i = 0; i < 32; ++i )
        ClearObjective(&level.objectives[i]);
}

// Decomp: CoDSP_rdBlackOps.map.c (ObjectiveStateIndexFromString)
int __cdecl ObjectiveStateIndexFromString(objectiveState_t *piStateIndex, unsigned int stateString)
{
    if ( stateString == scr_const.empty )
    {
        *piStateIndex = OBJST_EMPTY;
    }
    else if ( stateString == scr_const.invisible )
    {
        *piStateIndex = OBJST_INVISIBLE;
    }
    else if ( stateString == scr_const.current )
    {
        *piStateIndex = OBJST_CURRENT;
    }
    else if ( stateString == scr_const.active )
    {
        *piStateIndex = OBJST_ACTIVE;
    }
    else if ( stateString == scr_const.done )
    {
        *piStateIndex = OBJST_DONE;
    }
    else if ( stateString == scr_const.failed )
    {
        *piStateIndex = OBJST_FAILED;
    }
    else
    {
        *piStateIndex = OBJST_EMPTY;
        return 0;
    }
    return 1;
}

// Decomp: CoDSP_rdBlackOps.map.c (ClearObjective)
void __cdecl ClearObjective(objective_t *obj)
{
    obj->state = OBJST_EMPTY;
    obj->origin[0] = 0.0f;
    obj->origin[1] = 0.0f;
    obj->origin[2] = 0.0f;
    obj->entNum = 1023;
    obj->teamNum = 0;
    obj->icon = 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (PrintObjectiveUpdate @ 82601348)
static void PrintObjectiveUpdate(unsigned __int16 stateName, const char *objectiveString, int objNum, int clientNum)
{
    if ( zombiemode->current.enabled )
        return;
    if ( stateName == scr_const.current || stateName == scr_const.active )
    {
        if ( objectiveString && objectiveString[0] && I_stricmp(objectiveString, "GAME_OBJECTIVESUPDATED") )
            SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, va("%c \"%i\" \"%s\"", 'V', objNum, objectiveString));
    }
    else if ( stateName == scr_const.invisible )
    {
        SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, va("%c \"%i\"", 'V', 1));
    }
    else if ( stateName == scr_const.empty || stateName == scr_const.failed )
    {
        SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, va("%c \"%i\"", 'V', 2));
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (SetObjectiveIconIntoConfigString @ 82602030)
static void SetObjectiveIconIntoConfigString(char *configString, unsigned int paramNum)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    char *shaderName;
    int i;

    shaderName = Scr_GetString(paramNum, SCRIPTINSTANCE_SERVER);
    for ( i = 0; shaderName[i]; ++i )
    {
        if ( shaderName[i] <= 31 || shaderName[i] >= 127 )
        {
            fmtMsg = va(
                "Illegal character '%c'(ascii %i) in objective icon name: %s\n",
                shaderName[i],
                (unsigned __int8)shaderName[i],
                shaderName);
            Scr_ParamError(paramNum, fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( i >= 64 )
    {
        fmtMsg2 = va("Objective icon name is too long (> %i): %s\n", 63, shaderName);
        Scr_ParamError(paramNum, fmtMsg2, SCRIPTINSTANCE_SERVER);
    }
    Info_SetValueForKey(configString, "icon", va("%i", G_MaterialIndex(shaderName)));
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Objective_Add_Internal @ 82601648)
void __cdecl Scr_Objective_Add_Internal(unsigned int firstParamIndex, int teamFilter)
{
    const char *fmtMsg;
    char *slStr;
    const char *fmtMsg2;
    objectiveState_t state;
    objective_t *obj;
    char configString[1024];
    char objectiveString[1024];
    const char *localizedString;
    int numParam;
    unsigned __int16 stateName;
    int objNum;
    int clientNum;
    gentity_s *entity;
    float origin[3];

    numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParam < (int)(firstParamIndex + 2) )
        Scr_Error(
            "objective_add needs at least the first two parameters out of its parameter list of: index state [string] [position]\n",
            0);
    objNum = Scr_GetInt(firstParamIndex, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)objNum >= 0x20 )
    {
        fmtMsg = va("index %i is an illegal objective index. Valid indexes are 0 to %i\n", objNum, 31);
        Scr_ParamError(firstParamIndex, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    SV_GetConfigstring(objNum + 327, configString, sizeof(configString));
    stateName = (unsigned __int16)Scr_GetConstString(firstParamIndex + 1, SCRIPTINSTANCE_SERVER);
    if ( !ObjectiveStateIndexFromString(&state, stateName) )
    {
        slStr = SL_ConvertToString(stateName, SCRIPTINSTANCE_SERVER);
        fmtMsg2 = va(
            "Illegal objective state \"%s\". Valid states are \"empty\", \"active\", \"invisible\", \"done\", \"current\", \"failed\"\n",
            slStr);
        Scr_ParamError(firstParamIndex + 1, fmtMsg2, SCRIPTINSTANCE_SERVER);
    }
    Info_SetValueForKey(configString, "state", va("%i", state));
    obj = &level.objectives[objNum];
    level.objectivesClientMask[objNum][0] = 0;
    level.objectivesClientMask[objNum][1] = 0;
    ClearObjective_OnEntity(obj);
    obj->state = state;
    obj->teamNum = 0;
    localizedString = "GAME_OBJECTIVESUPDATED";
    if ( numParam > (int)(firstParamIndex + 2) )
    {
        if ( Scr_GetType(firstParamIndex + 2, SCRIPTINSTANCE_SERVER) == VAR_STRING )
        {
            Scr_ConstructMessageString(
                firstParamIndex + 2,
                numParam - 1,
                "Objective String",
                objectiveString,
                sizeof(objectiveString));
            if ( !Info_Validate(objectiveString) || strchr(objectiveString, '\\') )
            {
                fmtMsg = va(
                    "Objective strings can not have a \", a ;, or a \\ in them. Illegal objective string: %s\n",
                    objectiveString);
                Scr_Error(fmtMsg, 0);
            }
            Info_SetValueForKey(configString, "str", objectiveString);
            localizedString = objectiveString;
            if ( numParam > (int)(firstParamIndex + 3) )
            {
                if ( Scr_GetType(firstParamIndex + 3, SCRIPTINSTANCE_SERVER) == VAR_VECTOR )
                {
                    Scr_GetVector(firstParamIndex + 3, origin, SCRIPTINSTANCE_SERVER);
                    Info_SetValueForKey(
                        configString,
                        "org0",
                        va("%i %i %i", (int)origin[0], (int)origin[1], (int)origin[2]));
                    Info_RemoveKey(configString, "ent0");
                    obj->origin[0] = (float)(int)origin[0];
                    obj->origin[1] = (float)(int)origin[1];
                    obj->origin[2] = (float)(int)origin[2];
                    obj->entNum = 1023;
                }
                else
                {
                    entity = Scr_GetEntity(firstParamIndex + 3);
                    Info_SetValueForKey(configString, "ent0", va("%i", entity->s.number));
                    Info_RemoveKey(configString, "org0");
                    obj->entNum = entity->s.number;
                    entity->r.svFlags |= 0x10u;
                }
                if ( numParam > (int)(firstParamIndex + 4) )
                {
                    SetObjectiveIconIntoConfigString(configString, firstParamIndex + 4);
                    SetObjectiveIcon(obj, firstParamIndex + 4);
                }
            }
        }
    }
    if ( teamFilter == -1 )
    {
        SV_SetConfigstring(objNum + 327, configString);
        SV_GameSendServerCommand(-1, SV_CMD_CAN_IGNORE, va("%c \"%i\"", 'J', 74));
        PrintObjectiveUpdate(stateName, localizedString, objNum, -1);
        obj->teamNum = 0;
    }
    else
    {
        Info_SetValueForKey(configString, "team", va("%i", teamFilter));
        obj->teamNum = teamFilter;
        SV_SetConfigstring(objNum + 327, configString);
        for ( clientNum = 0; clientNum < level.maxclients; ++clientNum )
        {
            if ( level.clients[clientNum].sess.connected == CON_CONNECTED
                && level.gentities[clientNum].client
                && teamFilter == (int)G_GetClientTeam(clientNum) )
            {
                SV_GameSendServerCommand(clientNum, SV_CMD_CAN_IGNORE, va("%c \"%i\"", 'J', 74));
                PrintObjectiveUpdate(stateName, localizedString, objNum, clientNum);
            }
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Objective_Add)
void Scr_Objective_Add()
{
    Scr_Objective_Add_Internal(0, -1);
}

// Decomp: CoDSP_rdBlackOps.map.c (ClearObjective_OnEntity)
void __cdecl ClearObjective_OnEntity(objective_t *obj)
{
    gentity_s *pEnt;

    if ( obj->entNum != 1023 )
    {
        pEnt = &g_entities[obj->entNum];
        if ( pEnt->r.inuse )
            pEnt->r.svFlags &= ~0x10u;
        obj->entNum = 1023;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (SetObjectiveIcon)
void __cdecl SetObjectiveIcon(objective_t *obj, unsigned int paramNum)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    char *shaderName;
    int i;

    shaderName = Scr_GetString(paramNum, SCRIPTINSTANCE_SERVER);
    for ( i = 0; shaderName[i]; ++i )
    {
        if ( shaderName[i] <= 31 || shaderName[i] >= 127 )
        {
            fmtMsg = va(
                         "Illegal character '%c'(ascii %i) in objective icon name: %s\n",
                         shaderName[i],
                         (unsigned __int8)shaderName[i],
                         shaderName);
            Scr_ParamError(3u, fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( i >= 64 )
    {
        fmtMsg2 = va("Objective icon name is too long (> %i): %s\n", 63, shaderName);
        Scr_ParamError(3u, fmtMsg2, SCRIPTINSTANCE_SERVER);
    }
    obj->icon = G_MaterialIndex(shaderName);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Objective_Delete)
void Scr_Objective_Delete()
{
    const char *fmtMsg;
    int objNum;

    objNum = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)objNum >= 0x20 )
    {
        fmtMsg = va("index %i is an illegal objective index. Valid indexes are 0 to %i\n", objNum, 31);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    ClearObjective_OnEntity(&level.objectives[objNum]);
    ClearObjective(&level.objectives[objNum]);
    level.objectivesClientMask[objNum][0] = 0;
    level.objectivesClientMask[objNum][1] = 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Objective_State)
void Scr_Objective_State()
{
    const char *fmtMsg;
    char *String;
    const char *fmtMsg2;
    objectiveState_t state;
    objective_t *obj;
    unsigned __int16 stateName;
    int objNum;

    objNum = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)objNum >= 0x20 )
    {
        fmtMsg = va("index %i is an illegal objective index. Valid indexes are 0 to %i\n", objNum, 31);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    obj = &level.objectives[objNum];
    stateName = (unsigned __int16)Scr_GetConstString(1u, SCRIPTINSTANCE_SERVER);
    if ( !ObjectiveStateIndexFromString(&state, stateName) )
    {
        String = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
        fmtMsg2 = va(
                     "Illegal objective state \"%s\". Valid states are \"empty\", \"invisible\", \"current\", \"active\"\n",
                     String);
        Scr_ParamError(1u, fmtMsg2, SCRIPTINSTANCE_SERVER);
    }
    obj->state = state;
    if ( state == OBJST_EMPTY || state == OBJST_INVISIBLE )
        ClearObjective_OnEntity(obj);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Objective_Icon)
void Scr_Objective_Icon()
{
    const char *fmtMsg;
    int objNum;

    objNum = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)objNum >= 0x20 )
    {
        fmtMsg = va("index %i is an illegal objective index. Valid indexes are 0 to %i\n", objNum, 31);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    SetObjectiveIcon(&level.objectives[objNum], 1u);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Objective_Position)
void Scr_Objective_Position()
{
    const char *fmtMsg;
    objective_t *obj;
    int objNum;

    objNum = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)objNum >= 0x20 )
    {
        fmtMsg = va("index %i is an illegal objective index. Valid indexes are 0 to %i\n", objNum, 31);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    obj = &level.objectives[objNum];
    ClearObjective_OnEntity(obj);
    Scr_GetVector(1u, obj->origin, SCRIPTINSTANCE_SERVER);
    obj->origin[0] = (float)(int)obj->origin[0];
    obj->origin[1] = (float)(int)obj->origin[1];
    obj->origin[2] = (float)(int)obj->origin[2];
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Objective_OnEntity)
void Scr_Objective_OnEntity()
{
    const char *fmtMsg;
    gentity_s *entity;
    objective_t *obj;
    int objNum;

    objNum = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)objNum >= 0x20 )
    {
        fmtMsg = va("index %i is an illegal objective index. Valid indexes are 0 to %i\n", objNum, 31);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    obj = &level.objectives[objNum];
    ClearObjective_OnEntity(obj);
    entity = Scr_GetEntity(1u);
    entity->r.svFlags |= 0x10u;
    obj->entNum = entity->s.number;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Objective_Current)
void Scr_Objective_Current()
{
    const char *fmtMsg;
    objective_t *obj;
    int numParam;
    int makeCurrent[32];
    int i;
    int objNum;

    numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    memset((unsigned __int8 *)makeCurrent, 0, sizeof(makeCurrent));
    for ( i = 0; i < numParam; ++i )
    {
        objNum = Scr_GetInt(i, SCRIPTINSTANCE_SERVER);
        if ( (unsigned int)objNum >= 0x20 )
        {
            fmtMsg = va("index %i is an illegal objective index. Valid indexes are 0 to %i\n", objNum, 31);
            Scr_ParamError(i, fmtMsg, SCRIPTINSTANCE_SERVER);
        }
        makeCurrent[objNum] = 1;
    }
    for ( objNum = 0; objNum < 32; ++objNum )
    {
        obj = &level.objectives[objNum];
        if ( makeCurrent[objNum] )
        {
            obj->state = OBJST_CURRENT;
        }
        else if ( obj->state == OBJST_CURRENT )
        {
            obj->state = OBJST_ACTIVE;
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Objective_SetVisibleToPlayer)
void Scr_Objective_SetVisibleToPlayer()
{
    const char *fmtMsg;
    int objNum;
    gentity_s *player;

    objNum = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)objNum >= 0x20 )
    {
        fmtMsg = va("index %i is an illegal objective index. Valid indexes are 0 to %i\n", objNum, 31);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    player = Scr_GetEntity(1u);
    if ( !player->client
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    5293,
                    0,
                    "%s",
                    "player->client") )
    {
        __debugbreak();
    }
    level.objectivesClientMask[objNum][player->s.number >> 5] &= ~(1 << (player->s.number & 0x1F));
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Objective_SetInvisibleToPlayer)
void Scr_Objective_SetInvisibleToPlayer()
{
    const char *fmtMsg;
    int objNum;
    gentity_s *player;

    objNum = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)objNum >= 0x20 )
    {
        fmtMsg = va("index %i is an illegal objective index. Valid indexes are 0 to %i\n", objNum, 31);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    player = Scr_GetEntity(1u);
    if ( !player->client
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    5320,
                    0,
                    "%s",
                    "player->client") )
    {
        __debugbreak();
    }
    level.objectivesClientMask[objNum][player->s.number >> 5] |= 1 << (player->s.number & 0x1F);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Objective_SetVisibleToAll)
void  Scr_Objective_SetVisibleToAll()
{
    const char *fmtMsg;
    int objNum;

    objNum = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ((unsigned int)objNum >= 0x20)
    {
        fmtMsg = va("index %i is an illegal objective index. Valid indexes are 0 to %i\n", objNum, 31);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    level.objectivesClientMask[objNum][0] = 0;
    level.objectivesClientMask[objNum][1] = 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Objective_SetInvisibleToAll)
void Scr_Objective_SetInvisibleToAll()
{
    char *fmtMsg;
    int objNum;

    objNum = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ((unsigned int)objNum >= 0x20)
    {
        fmtMsg = va("index %i is an illegal objective index. Valid indexes are 0 to %i\n", objNum, 31);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    level.objectivesClientMask[objNum][0] = -1;
    level.objectivesClientMask[objNum][1] = -1;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Objective_SetSize)
void Scr_Objective_SetSize()
{
    const char *fmtMsg;
    float Float;
    float tempValue2;
    float height;
    gentity_s *ent;
    objective_t *objective;
    int objectiveIndex;

    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 2 )
        Scr_Error("Objective_Size() called with wrong params.\n", 0);
    objectiveIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)objectiveIndex >= 0x20 )
    {
        fmtMsg = va("index %i is an illegal objective index. Valid indexes are 0 to %i\n", objectiveIndex, 31);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    objective = &level.objectives[objectiveIndex];
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 2 )
    {
        ent = Scr_GetEntity(1u);
        if ( ent )
        {
            height = ent->r.maxs[1] - ent->r.mins[1];
            objective->size[0] = ent->r.maxs[0] - ent->r.mins[0];
            objective->size[1] = height;
        }
        else
        {
            Scr_ParamError(1u, "Illegal entity parameter for Objective_SetSize.", SCRIPTINSTANCE_SERVER);
        }
    }
    else
    {
        Float = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
        tempValue2 = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
        objective->size[0] = Float;
        objective->size[1] = tempValue2;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Objective_SetColor)
void Scr_Objective_SetColor()
{
    const char *fmtMsg;
    int objectiveIndex;
    float color[4];

    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 4 )
        Scr_Error("Objective_SetColor() called with wrong params.\n", 0);
    objectiveIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)objectiveIndex >= 0x20 )
    {
        fmtMsg = va("index %i is an illegal objective index. Valid indexes are 0 to %i\n", objectiveIndex, 31);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    color[0] = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    color[1] = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    color[2] = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
    color[3] = 1.0f;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 5 )
        color[3] = Scr_GetFloat(4u, SCRIPTINSTANCE_SERVER);
    Byte4PackRgba(color, (unsigned __int8 *)&level.objectives[objectiveIndex].color);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_Objective_Team)
void GScr_Objective_Team()
{
    const char *fmtMsg;
    char *slStr;
    const char *fmtMsg2;
    objective_t *obj;
    unsigned __int16 team;
    int objNum;

    objNum = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)objNum >= 0x20 )
    {
        fmtMsg = va("index %i is an illegal objective index. Valid indexes are 0 to %i\n", objNum, 31);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    obj = &level.objectives[objNum];
    team = (unsigned __int16)Scr_GetConstString(1u, SCRIPTINSTANCE_SERVER);
    if ( team == scr_const.allies )
    {
        obj->teamNum = 2;
    }
    else if ( team == scr_const.axis )
    {
        obj->teamNum = 1;
    }
    else if ( team == scr_const.none )
    {
        obj->teamNum = 0;
    }
    else
    {
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsAssetLoaded ~6133516)
void GScr_IsAssetLoaded()
{
    const char *typeName;
    const char *assetName;
    XAssetType assetType;
    int i;

    typeName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    assetName = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    assetType = ASSET_TYPE_COUNT;
    for ( i = 0; i < ASSET_TYPE_COUNT; ++i )
    {
        if ( !I_stricmp(typeName, g_assetNames[i]) )
        {
            assetType = (XAssetType)i;
            break;
        }
    }
    if ( assetType == ASSET_TYPE_COUNT )
        Scr_Error(va("type %s is not a valid asset type: see g_assetNames in db_assetnames.h\n", typeName), 0);
    Scr_AddBool(DB_FindXAssetEntry(assetType, assetName) != NULL, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsBeingWatched ~6126845)
void GScr_IsBeingWatched(scr_entref_t entref)
{
    gentity_s *ent;
    gentity_s *clientEnt;
    int clientNum;
    bool watched;
    float viewDir[3];
    float toTarget[3];
    float dist;
    float dot;

    ent = GetEntity(entref);
    iassert(ent);
    watched = false;
    for ( clientNum = 0; clientNum < level.maxclients; ++clientNum )
    {
        if ( level.clients[clientNum].sess.sessionState < SESS_STATE_SPECTATOR )
            continue;
        clientEnt = &g_entities[clientNum];
        if ( !clientEnt->client )
            continue;
        AngleVectors(clientEnt->client->ps.viewangles, viewDir, 0, 0);
        Vec3Sub(ent->r.currentOrigin, clientEnt->client->ps.origin, toTarget);
        dist = Vec3Normalize(toTarget);
        dot = (viewDir[0] * toTarget[0]) + (viewDir[1] * toTarget[1]) + (viewDir[2] * toTarget[2]);
        if ( dot > 0.70700002f && dist < 1000.0f )
        {
            watched = true;
            break;
        }
        if ( dist < 200.0f )
        {
            watched = true;
            break;
        }
    }
    Scr_AddBool(watched, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsCoopEPD ~6151504)
void GScr_IsCoopEPD()
{
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        Scr_Error("USAGE: bool = IsCoopEPD()\n", 0);
    Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_IsPlayerReloading ~6121553)
void ScrCmd_IsPlayerReloading(scr_entref_t entref)
{
    gentity_s *ent;
    weaponstate_t weaponstate;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_Error("isplayerreloading should only be called for players.", 0);
    weaponstate = ent->client->ps.weaponstate;
    Scr_AddBool(
        weaponstate == WEAPON_RELOADING_INTERUPT
        || weaponstate == WEAPON_RELOAD_START_INTERUPT
        || weaponstate == WEAPON_RELOAD_QUICK
        || weaponstate == WEAPON_RELOAD_END
        || weaponstate == WEAPON_RELOAD_START
        || weaponstate == WEAPON_RELOAD_QUICK_EMPTY
        || weaponstate == WEAPON_MELEE_INIT,
        SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsSprinting ~6155535)
void GScr_IsSprinting(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_Error("IsSprinting can only be called on a player.", 0);
    Scr_AddInt(PM_IsSprinting(&ent->client->ps), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_ItemWeaponSetOptions ~6121739)
void ScrCmd_ItemWeaponSetOptions(scr_entref_t entref)
{
    gentity_s *itemEnt;
    int options;

    itemEnt = GetEntity(entref);
    if ( itemEnt->s.eType != 3 )
        Scr_Error("Entity is not an item.", 0);
    if ( bg_itemlist[itemEnt->s.un3.item].giType != IT_WEAPON )
        Scr_Error("Item entity is not a weapon.", 0);
    options = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    itemEnt->s.renderOptions.i = ((unsigned __int8)options << 26) | (itemEnt->s.renderOptions.i & 0x3FFFFFF);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_LerpSunDirection ~6141646)
void Scr_LerpSunDirection()
{
    float startDir[3];
    float endDir[3];
    float lerpTime;
    int endTime;
    char configString[256];

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 3 )
        Scr_Error("Incorrect number of parameters\n", 0);
    Scr_GetVector(0, startDir, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, endDir, SCRIPTINSTANCE_SERVER);
    lerpTime = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    Vec3Normalize(startDir);
    Vec3Normalize(endDir);
    if ( lerpTime <= 0.001f )
        Scr_Error("Lerp time must be greater than 1 ms\n", 0);
    endTime = level.time + (int)(lerpTime * 1000.0f);
    Com_sprintf(
        configString,
        sizeof(configString),
        "%g %g %g %g %g %g %d %d",
        startDir[0],
        startDir[1],
        startDir[2],
        endDir[0],
        endDir[1],
        endDir[2],
        level.time,
        endTime);
    SV_SetConfigstring(9, configString);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_loadGump ~6153053)
void GScr_loadGump()
{
    const char *gumpName;
    char cmd[256];

    gumpName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( gumpName && gumpName[0] )
    {
        Com_sprintf(cmd, sizeof(cmd), "loadGump %s\n", gumpName);
        Cbuf_AddText(0, cmd);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_MagicGrenadeType ~6121879)
void ScrCmd_MagicGrenadeType(scr_entref_t entref)
{
    gentity_s *ent;
    const char *weaponName;
    int weaponIndex;
    const WeaponDef *weaponDef;
    float origin[3];
    float velocity[3];
    int fuseTime;
    gentity_s *grenade;
    int numParam;

    ent = GetEntity(entref);
    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName((char *)weaponName);
    weaponDef = BG_GetWeaponDef(weaponIndex);
    if ( weaponDef->weapType != WEAPTYPE_GRENADE )
        Scr_Error("MagicGrenadeManual invalid weapon type specified.\n", 0);
    numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParam != 3 && numParam != 4 )
        Scr_Error("<ent> MagicGrenadeManual <weaponname> <origin> <velocity> [time To Blow (seconds)].\n", 0);
    Scr_GetVector(1u, origin, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(2u, velocity, SCRIPTINSTANCE_SERVER);
    fuseTime = 5000;
    if ( numParam == 4 )
        fuseTime = (int)(Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER) * 1000.0f);
    grenade = G_FireGrenade(ent, origin, velocity, weaponIndex, 0, 1, fuseTime);
    Scr_AddEntity(grenade, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_MakeLocalizedString ~6153181)
void Scr_MakeLocalizedString()
{
    char constructed[1024];
    char finalString[1024];
    const char *localized;
    unsigned int searchPos;
    unsigned int stringLen;
    int numParam;

    numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    Scr_ConstructMessageString(0, numParam - 1, "Script localized string", constructed, 0x400u);
    localized = SEH_LocalizeTextMessage(constructed, "Script localized string", LOCMSG_SAFE);
    stringLen = strlen(localized);
    if ( stringLen >= sizeof(finalString) )
        Com_Error(ERR_DROP, "Translated message too long to process: %s\n", constructed);
    memcpy(finalString, localized, stringLen + 1);
    searchPos = 0;
    while ( SEH_ReplaceDirective(0, &searchPos, &stringLen, finalString, 0x400u) )
        ;
    Scr_AddString(finalString, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_NetUpdate ~6141401)
void GScr_NetUpdate(scr_entref_t entref)
{
    gentity_s *ent;
    int enable;

    ent = GetEntity(entref);
    enable = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( enable )
        ent->s.clientLinkInfo.flags &= ~0x80u;
    else
        ent->s.clientLinkInfo.flags |= 0x80u;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Objective_Add_Team ~826020B8)
void Scr_Objective_Add_Team()
{
    unsigned __int16 teamName;
    int teamNum;

    teamNum = -1;
    teamName = (unsigned __int16)Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    if ( teamName == scr_const.allies )
        Scr_Objective_Add_Internal(1, 2);
    else
    {
        if ( teamName == scr_const.axis )
            teamNum = 1;
        Scr_Objective_Add_Internal(1, teamNum);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Objective_AdditionalCurrent ~6129131)
void Scr_Objective_AdditionalCurrent()
{
    const char *fmtMsg;
    objective_t *obj;
    int numParam;
    int makeCurrent[32];
    int i;
    int objNum;

    numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    memset((unsigned __int8 *)makeCurrent, 0, sizeof(makeCurrent));
    for ( i = 0; i < numParam; ++i )
    {
        objNum = Scr_GetInt(i, SCRIPTINSTANCE_SERVER);
        if ( (unsigned int)objNum >= 0x20 )
        {
            fmtMsg = va("index %i is an illegal objective index. Valid indexes are 0 to %i\n", objNum, 31);
            Scr_ParamError(i, fmtMsg, SCRIPTINSTANCE_SERVER);
        }
        makeCurrent[objNum] = 1;
    }
    for ( objNum = 0; objNum < 32; ++objNum )
    {
        obj = &level.objectives[objNum];
        if ( makeCurrent[objNum] && obj->state != OBJST_CURRENT )
            obj->state = OBJST_CURRENT;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Objective_AdditionalPosition ~6128303)
void Scr_Objective_AdditionalPosition()
{
    const char *fmtMsg;
    objective_t *obj;
    gentity_s *entity;
    unsigned int posIndex;
    int objNum;

    objNum = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)objNum >= 0x20 )
    {
        fmtMsg = va("index %i is an illegal objective index. Valid indexes are 0 to %i\n", objNum, 31);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    posIndex = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    if ( posIndex >= 8 )
    {
        fmtMsg = va("index %i is an illegal position index. Valid indexes are 0 to %i\n", posIndex, 7);
        Scr_Error(fmtMsg, 0);
    }
    if ( posIndex != 0 )
        return;
    obj = &level.objectives[objNum];
    if ( Scr_GetType(2u, SCRIPTINSTANCE_SERVER) == VAR_VECTOR )
    {
        ClearObjective_OnEntity(obj);
        Scr_GetVector(2u, obj->origin, SCRIPTINSTANCE_SERVER);
        obj->origin[0] = (float)(int)obj->origin[0];
        obj->origin[1] = (float)(int)obj->origin[1];
        obj->origin[2] = (float)(int)obj->origin[2];
        obj->entNum = 1023;
    }
    else
    {
        entity = Scr_GetEntity(2u);
        ClearObjective_OnEntity(obj);
        entity->r.svFlags |= 0x10u;
        obj->entNum = entity->s.number;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Objective_Ring ~6129227)
void Scr_Objective_Ring()
{
    const char *fmtMsg;
    objective_t *obj;
    int objNum;

    objNum = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)objNum >= 0x20 )
    {
        fmtMsg = va("index %i is an illegal objective index. Valid indexes are 0 to %i\n", objNum, 31);
        Scr_Error(fmtMsg, 0);
    }
    obj = &level.objectives[objNum];
    obj->use3D ^= 2;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Objective_Set3D ~6128553)
void Scr_Objective_Set3D()
{
    const char *fmtMsg;
    objective_t *obj;
    int numParam;
    int objNum;
    int ringMode;

    numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    objNum = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)objNum >= 0x20 )
    {
        fmtMsg = va("index %i is an illegal objective index. Valid indexes are 0 to %i\n", objNum, 31);
        Scr_Error(fmtMsg, 0);
    }
    if ( (unsigned int)(numParam - 2) > 4 )
        Scr_Error(va("Invalid argument count. Expected 2-6, given %d\n", numParam), 0);
    obj = &level.objectives[objNum];
    obj->use3D = 0;
    obj->alt_3D_text = NULL;
    if ( numParam >= 6 )
    {
        ringMode = Scr_GetInt(4u, SCRIPTINSTANCE_SERVER);
        if ( ringMode >= 0 )
            obj->use3D |= 2 * (ringMode & 0xF);
    }
    if ( numParam >= 5 )
    {
        float offset[3];
        Scr_GetVector(5u, offset, SCRIPTINSTANCE_SERVER);
        obj->origin[0] = offset[0];
        obj->origin[1] = offset[1];
        obj->origin[2] = offset[2];
    }
    if ( numParam >= 4 )
        obj->use3D |= Scr_GetInt(3u, SCRIPTINSTANCE_SERVER) != 0;
    obj->use3D |= 1;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Objective_Size ~6129291)
void Scr_Objective_Size()
{
    Scr_Objective_SetSize();
}

void __cdecl Scr_Objective_String_Internal(bool sendMessage)
{
    const char *fmtMsg;
    char msgBuf[1024];
    objective_t *obj;
    int numParam;
    int objNum;
    int i;

    objNum = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)objNum >= 0x20 )
    {
        fmtMsg = va("index %i is an illegal objective index. Valid indexes are 0 to %i\n", objNum, 31);
        Scr_Error(fmtMsg, 0);
    }
    numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    Scr_ConstructMessageString(1, numParam - 1, "Objective String", msgBuf, 0x400u);
    if ( !Info_Validate(msgBuf) || strchr(msgBuf, '\\') )
        Scr_Error(va("Objective strings can not have a \", a ;, or a \\ in them. Illegal objective string: %s\n", msgBuf), 0);
    for ( i = 0; msgBuf[i]; ++i )
    {
        if ( (unsigned __int8)msgBuf[i] <= 0x1Fu && msgBuf[i] != 20 && msgBuf[i] != 21 )
            Scr_Error(
                va("Illegal character '%c'(ascii %i) in objective string: %s\n", msgBuf[i], (unsigned __int8)msgBuf[i], msgBuf),
                0);
        if ( (unsigned __int8)msgBuf[i] >= 0x7Fu )
            Scr_Error(
                va("Illegal character '%c'(ascii %i) in objective string: %s\n", msgBuf[i], (unsigned __int8)msgBuf[i], msgBuf),
                0);
    }
    obj = &level.objectives[objNum];
    if ( sendMessage && obj->state != OBJST_DONE )
    {
        const char *cmd = va("%c \"%i\" \"%s\"", 'V', objNum, msgBuf);
        SV_GameSendServerCommand(-1, SV_CMD_CAN_IGNORE, cmd);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Objective_String ~6127902)
void Scr_Objective_String()
{
    Scr_Objective_String_Internal(true);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Objective_String_NoMessage ~6128073)
void Scr_Objective_String_NoMessage()
{
    Scr_Objective_String_Internal(false);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_Pause3DCinematic ~6135791)
void GScr_Pause3DCinematic()
{
    const char *cmd;
    int paused;

    paused = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    cmd = va("%c %i", 59, paused);
    SV_GameSendServerCommand(-1, SV_CMD_CAN_IGNORE, cmd);
}

// Decomp: CoDSP_rdBlackOps.map.c (GetNormalised2DMapPosition)
void __cdecl GetNormalised2DMapPosition(float *inPos, float *outPos)
{
    float posDeltaX;
    float posDeltaY;

    *outPos = *inPos;
    outPos[1] = inPos[1];
    posDeltaX = *outPos - level.compassMapUpperLeft[0];
    posDeltaY = outPos[1] - level.compassMapUpperLeft[1];
    // Rotate into compass space and normalize to [0,1] on each axis.
    *outPos = (float)(level.compassNorth[1] * posDeltaX) - (float)(level.compassNorth[0] * posDeltaY);
    outPos[1] = (float)((-(level.compassNorth[1])) * posDeltaY) - (float)(level.compassNorth[0] * posDeltaX);
    if ( level.compassMapWorldSize[0] != 0.0 && level.compassMapWorldSize[1] != 0.0 )
    {
        *outPos = *outPos / level.compassMapWorldSize[0];
        outPos[1] = outPos[1] / level.compassMapWorldSize[1];
    }
    if ( *outPos < 0.0f )
        *outPos = 0.0f;
    else if ( *outPos > 1.0f )
        *outPos = 1.0f;
    if ( outPos[1] < 0.0f )
        outPos[1] = 0.0f;
    else if ( outPos[1] > 1.0f )
        outPos[1] = 1.0f;
}

// Decomp: CoDSP_rdBlackOps.map.c (SetArtilleryIconLocation)
void __cdecl SetArtilleryIconLocation()
{
    char *slStr;
    const char *fmtMsg;
    int activeBits;
    playerState_s *ps;
    gentity_s *player;
    int i;
    float outPos[2];
    unsigned __int16 team;
    int yPos;
    float vPos[3];
    int xPos;
    int teamNum;
    int isActive;
    int isMortar;
    int artilleryIconLocation;
    int numParams;
    int clientNum;

    numParams = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParams < 2 || numParams > 5 )
        Scr_Error("Incorrect number of parameters to artilleryiconlocation\n", 0);
    isMortar = 0;
    clientNum = -1;
    if ( numParams > 4 )
        clientNum = Scr_GetInt(4u, SCRIPTINSTANCE_SERVER);
    if ( numParams > 3 )
        isMortar = Scr_GetInt(3u, SCRIPTINSTANCE_SERVER);
    isActive = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
    teamNum = 0;
    artilleryIconLocation = 0;
    if ( isActive )
    {
        Scr_GetVector(0, vPos, SCRIPTINSTANCE_SERVER);
        vPos[0] = (float)(int)vPos[0];
        vPos[1] = (float)(int)vPos[1];
        vPos[2] = (float)(int)vPos[2];
        GetNormalised2DMapPosition(vPos, outPos);
        team = (unsigned __int16)Scr_GetConstString(1u, SCRIPTINSTANCE_SERVER);
        if ( team == scr_const.allies )
        {
            teamNum = 2;
        }
        else if ( team == scr_const.axis )
        {
            teamNum = 1;
        }
        else if ( team == scr_const.none )
        {
            teamNum = 0;
        }
        else
        {
            slStr = SL_ConvertToString(team, SCRIPTINSTANCE_SERVER);
            fmtMsg = va("Illegal team string '%s'. Must be allies, axis, or none.", slStr);
            Scr_ParamError(1u, fmtMsg, SCRIPTINSTANCE_SERVER);
        }
        xPos = (unsigned __int8)(int)(float)(outPos[0] * 255.0);
        yPos = (unsigned __int8)(int)(float)(outPos[1] * 255.0);
        artilleryIconLocation = xPos | (yPos << 8);
    }
    for ( i = 0; i < com_maxclients->current.integer; ++i )
    {
        player = &g_entities[i];
        if ( player->client )
        {
            ps = &player->client->ps;
            if ( isActive )
            {
                if ( (teamNum || clientNum == ps->clientNum) && player->client->sess.cs.team == teamNum )
                    activeBits = 0x10000;
                else
                    activeBits = 0x20000;
                if ( isMortar )
                    artilleryIconLocation |= 0x40000u;
                ps->artilleryInboundIconLocation = activeBits | artilleryIconLocation;
            }
            else
            {
                ps->artilleryInboundIconLocation &= 0xFFF8FFFF;
            }
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_LogPrint)
void GScr_LogPrint()
{
    unsigned int tempValue0;
    int iStringLen;
    char string[1024];
    int iNumParms;
    int i;
    const char *pszToken;

    string[0] = 0;
    iStringLen = 0;
    iNumParms = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    for ( i = 0; i < iNumParms; ++i )
    {
        pszToken = Scr_GetString(i, SCRIPTINSTANCE_SERVER);
        tempValue0 = strlen(pszToken);
        if ( (int)(tempValue0 + iStringLen) >= 1024 )
            break;
        I_strncat(string, 1024, pszToken);
        iStringLen += tempValue0;
    }
    G_LogPrintf(string);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WorldEntNumber)
void GScr_WorldEntNumber()
{
    Scr_AddInt(1022, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_Obituary)
void GScr_Obituary()
{
    gentity_s *pOtherEnt;
    char *pszWeapon;
    const WeaponDef *weapondef;
    unsigned int iWeaponNum;
    gentity_s *pEnt;
    int iMODNum;

    pszWeapon = Scr_GetString(2u, SCRIPTINSTANCE_SERVER);
    iWeaponNum = G_GetWeaponIndexForName(pszWeapon);
    weapondef = BG_GetWeaponDef(iWeaponNum);
    iMODNum = G_MeansOfDeathFromScriptParam(3u);
    pOtherEnt = Scr_GetEntity(0);
    pEnt = G_TempEntity(vec3_origin, EV_OBITUARY);
    pEnt->s.otherEntityNum = pOtherEnt->s.number;
    if (Scr_GetType(1u, SCRIPTINSTANCE_SERVER) == 1 && Scr_GetPointerType(1u, SCRIPTINSTANCE_SERVER) == 19)
        pEnt->s.attackerEntityNum = Scr_GetEntity(1u)->s.number;
    else
        pEnt->s.attackerEntityNum = 1022;
    pEnt->r.svFlags = 8;
    if (iMODNum == 16 && weapondef->impactType != IMPACT_TYPE_BLADE
        || iMODNum == 7
        || iMODNum == 9
        || iMODNum == 13
        || iMODNum == 12
        || iMODNum == 10)
    {
        pEnt->s.eventParm = iMODNum;
        pEnt->s.weaponModel = 0;
    }
    else
    {
        pEnt->s.eventParm = iWeaponNum;
        pEnt->s.weaponModel = 1;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ReviveObituary)
void __cdecl GScr_ReviveObituary()
{
    const gentity_s *player_entity;
    int entity_index;
    gentity_s *temporary_entity;
    unsigned __int16 client_index;
    int client_indexa;
    gentity_s *victim_entity;

    victim_entity = Scr_GetEntity(0);
    if ( victim_entity->s.eType == 1 )
    {
        temporary_entity = G_TempEntity(vec3_origin, EV_REVIVE_OBITUARY);
        client_index = victim_entity->client->ps.clientNum;
        temporary_entity->r.clientMask[0] = -1;
        temporary_entity->s.eventParms[0] = client_index;
        for ( entity_index = 0; entity_index < level.num_entities; ++entity_index )
        {
            player_entity = &g_entities[entity_index];
            if ( player_entity->client )
            {
                if ( player_entity->client->sess.cs.team == victim_entity->client->sess.cs.team )
                {
                    client_indexa = player_entity->client->ps.clientNum;
                    temporary_entity->r.clientMask[client_indexa >> 5] &= ~(1 << (client_indexa & 0x1F));
                }
            }
        }
    }
    else
    {
        Scr_Error("Can only call ReviveObituary on player entities", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_LerpFloat)
void GScr_LerpFloat()
{
    float tempValue0;
    float tempValue1;
    float from;
    float time;
    float to;

    from = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    to = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    time = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    if ( (float)(time - 1.0) < 0.0 )
        tempValue1 = time;
    else
        tempValue1 = 1.0f;
    if ( (float)(0.0 - time) < 0.0 )
        tempValue0 = tempValue1;
    else
        tempValue0 = 0.0f;
    Scr_AddFloat((float)((float)(to - from) * tempValue0) + from, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_LerpVector)
void GScr_LerpVector()
{
    float tempValue0;
    float tempValue1;
    float from[3];
    float time;
    float to[3];
    float retVal[3];

    Scr_GetVector(0, from, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, to, SCRIPTINSTANCE_SERVER);
    time = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    if ( (float)(time - 1.0) < 0.0 )
        tempValue1 = time;
    else
        tempValue1 = 1.0f;
    if ( (float)(0.0 - time) < 0.0 )
        tempValue0 = tempValue1;
    else
        tempValue0 = 0.0f;
    time = tempValue0;
    retVal[0] = (float)((float)(to[0] - from[0]) * tempValue0) + from[0];
    retVal[1] = (float)((float)(to[1] - from[1]) * tempValue0) + from[1];
    retVal[2] = (float)((float)(to[2] - from[2]) * tempValue0) + from[2];
    Scr_AddVector(retVal, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_AddDemoBookmark)
void GScr_AddDemoBookmark()
{
    int time;
    int clientNum1;
    VariableUnion clientNum2;
    int type;

    if ( Demo_IsEnabled() && Demo_IsRecording() )
    {
        clientNum2.intValue = 255;
        type = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        time = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
        clientNum1 = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
        if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 4 )
            clientNum2.intValue = Scr_GetInt(3u, SCRIPTINSTANCE_SERVER);
        if ( clientNum2.intValue > 32 )
            clientNum2.intValue = 255;
        Demo_AddBookmark(type, time, clientNum1, clientNum2.intValue);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_UpdateSpawnPoints)
void __cdecl Scr_UpdateSpawnPoints()
{
    unsigned __int16 callback;

    if ( !g_scr_data.updatespawnpoints )
        return;
    callback = Scr_ExecThread(SCRIPTINSTANCE_SERVER, g_scr_data.updatespawnpoints, 0);
    Scr_FreeThread(callback, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_positionWouldTelefrag)
void GScr_positionWouldTelefrag()
{
    float mins[3];
    float maxs[3];
    float vPos[3];

    Scr_GetVector(0, vPos, SCRIPTINSTANCE_SERVER);
    mins[0] = vPos[0] + -15.0;
    mins[1] = vPos[1] + -15.0;
    mins[2] = vPos[2] + 0.0;
    maxs[0] = vPos[0] + 15.0;
    maxs[1] = vPos[1] + 15.0;
    maxs[2] = vPos[2] + 70.0;
    Scr_BoundsWouldTelefrag(mins, maxs);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_BoundsWouldTelefrag)
void __cdecl Scr_BoundsWouldTelefrag(float *mins, float *maxs)
{
    int pm_type;
    int entityList[1025];
    gentity_s *entity;
    int tempValue5;
    int i;

    entityList[1024] = 0x2800000;
    tempValue5 = CM_AreaEntities(mins, maxs, entityList, 1024, 0x2800000);
    for ( i = 0; i < tempValue5; ++i )
    {
        entity = &g_entities[entityList[i]];
        if ( entity->client )
            pm_type = entity->client->ps.pm_type;
        else
            pm_type = 9;
        if ( pm_type < 9 || entity->s.eType == 14 )
        {
            Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
            return;
        }
    }
    Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_BoundsWouldTelefrag)
void GScr_BoundsWouldTelefrag()
{
    float mins[3];
    float maxs[3];

    Scr_GetVector(0, mins, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, maxs, SCRIPTINSTANCE_SERVER);
    Scr_BoundsWouldTelefrag(mins, maxs);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ReadTeamForSpawnPoints)
int __cdecl GScr_ReadTeamForSpawnPoints(unsigned int index)
{
    char *String;
    const char *fmtMsg;
    unsigned __int16 teamName;

    teamName = (unsigned __int16)Scr_GetConstString(index, SCRIPTINSTANCE_SERVER);
    if ( teamName == scr_const.allies )
        return 2;
    if ( teamName == scr_const.axis )
        return 1;
    if ( teamName != scr_const.free )
    {
        String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("team \"%s\" should be \"allies\", \"axis\", or \"free\"", String);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    return 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_RecordUsedSpawnPoint)
void GScr_RecordUsedSpawnPoint()
{
    float origin[3];
    int point_team;
    gentity_s *player;

    player = Scr_GetEntity(0);
    point_team = GScr_ReadTeamForSpawnPoints(1u);
    Scr_GetVector(2u, origin, SCRIPTINSTANCE_SERVER);
    //BLOPS_NULLSUB();
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_getStartTime)
void GScr_getStartTime()
{
    Scr_AddInt(level.startTime, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_PrecacheMenu)
void GScr_PrecacheMenu()
{
    const char *fmtMsg;
    char *pszNewMenu;
    int iConfigNum;
    int iConfigNuma;
    char szConfigString[1028];

    pszNewMenu = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    for ( iConfigNum = 0; iConfigNum < 32; ++iConfigNum )
    {
        SV_GetConfigstring(iConfigNum + 2548, szConfigString, 1024);
        if ( !I_stricmp(szConfigString, pszNewMenu) )
        {
            Com_DPrintf(24, "Script tried to precache the menu '%s' more than once\n", pszNewMenu);
            return;
        }
    }
    for ( iConfigNuma = 0; iConfigNuma < 32; ++iConfigNuma )
    {
        SV_GetConfigstring(iConfigNuma + 2548, szConfigString, 1024);
        if ( !szConfigString[0] )
            break;
    }
    if ( iConfigNuma == 32 )
    {
        fmtMsg = va("Too many menus precached. Max allowed menus is %i", 32);
        Scr_Error(fmtMsg, 0);
    }
    SV_SetConfigstring(iConfigNuma + 2548, pszNewMenu);
}

// Decomp: CoDSP_rdBlackOps.map.c:9697
int __cdecl GScr_GetScriptMenuIndex(const char *pszMenu)
{
    const char *fmtMsg;
    int iConfigNum;
    char szConfigString[1028];

    for ( iConfigNum = 0; iConfigNum < 32; ++iConfigNum )
    {
        SV_GetConfigstring(iConfigNum + 2548, szConfigString, 1024);
        if ( !I_stricmp(szConfigString, pszMenu) )
            return iConfigNum;
    }
    fmtMsg = va("Menu '%s' was not precached\n", pszMenu);
    Scr_Error(fmtMsg, 0);
    return 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_PrecacheStatusIcon)
void GScr_PrecacheStatusIcon()
{
    const char *fmtMsg;
    char *pszNewIcon;
    int iConfigNum;
    int iConfigNuma;
    char szConfigString[1028];

    pszNewIcon = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    for ( iConfigNum = 0; iConfigNum < 8; ++iConfigNum )
    {
        SV_GetConfigstring(iConfigNum + 3092, szConfigString, 1024);
        if ( !I_stricmp(szConfigString, pszNewIcon) )
        {
            Com_DPrintf(24, "Script tried to precache the player status icon '%s' more than once\n", pszNewIcon);
            return;
        }
    }
    for ( iConfigNuma = 0; iConfigNuma < 8; ++iConfigNuma )
    {
        SV_GetConfigstring(iConfigNuma + 3092, szConfigString, 1024);
        if ( !szConfigString[0] )
            break;
    }
    if ( iConfigNuma == 8 )
    {
        fmtMsg = va("Too many player status icons precached. Max allowed is %i", 8);
        Scr_Error(fmtMsg, 0);
    }
    SV_SetConfigstring(iConfigNuma + 3092, pszNewIcon);
}

// Decomp: CoDSP_rdBlackOps.map.c:9451
int __cdecl GScr_GetStatusIconIndex(const char *pszIcon)
{
    const char *fmtMsg;
    int iConfigNum;
    char szConfigString[1028];

    if ( !*pszIcon )
        return 0;
    for ( iConfigNum = 0; iConfigNum < 8; ++iConfigNum )
    {
        SV_GetConfigstring(iConfigNum + 3092, szConfigString, 1024);
        if ( !I_stricmp(szConfigString, pszIcon) )
            return iConfigNum + 1;
    }
    fmtMsg = va("Status icon '%s' was not precached\n", pszIcon);
    Scr_Error(fmtMsg, 0);
    return 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_PrecacheHeadIcon)
void GScr_PrecacheHeadIcon()
{
    const char *fmtMsg;
    char *pszNewIcon;
    int iConfigNum;
    int iConfigNuma;
    char szConfigString[1028];

    pszNewIcon = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    for ( iConfigNum = 0; iConfigNum < 15; ++iConfigNum )
    {
        SV_GetConfigstring(iConfigNum + 3100, szConfigString, 1024);
        if ( !I_stricmp(szConfigString, pszNewIcon) )
        {
            Com_DPrintf(24, "Script tried to precache the player head icon '%s' more than once\n", pszNewIcon);
            return;
        }
    }
    for ( iConfigNuma = 0; iConfigNuma < 15; ++iConfigNuma )
    {
        SV_GetConfigstring(iConfigNuma + 3100, szConfigString, 1024);
        if ( !szConfigString[0] )
            break;
    }
    if ( iConfigNuma == 15 )
    {
        fmtMsg = va("Too many player head icons precached. Max allowed is %i", 15);
        Scr_Error(fmtMsg, 0);
    }
    SV_SetConfigstring(iConfigNuma + 3100, pszNewIcon);
}

// Decomp: CoDSP_rdBlackOps.map.c:9454
int __cdecl GScr_GetHeadIconIndex(const char *pszIcon)
{
    const char *fmtMsg;
    int iConfigNum;
    char szConfigString[1028];

    if ( !*pszIcon )
        return 0;
    for ( iConfigNum = 0; iConfigNum < 15; ++iConfigNum )
    {
        SV_GetConfigstring(iConfigNum + 3100, szConfigString, 1024);
        if ( !I_stricmp(szConfigString, pszIcon) )
            return iConfigNum + 1;
    }
    fmtMsg = va("Head icon '%s' was not precached\n", pszIcon);
    Scr_Error(fmtMsg, 0);
    return 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WeaponClipSize)
void GScr_WeaponClipSize()
{
    int ClipSize;
    char *weaponName;
    unsigned int weaponIndex;

    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    Scr_VerifyWeaponIndex(weaponIndex, weaponName);
    ClipSize = BG_GetClipSize(weaponIndex);
    Scr_AddInt(ClipSize, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WeaponIsSemiAuto)
void GScr_WeaponIsSemiAuto()
{
    char *weaponName;
    unsigned int weaponIndex;
    const WeaponDef *weapDef;

    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    Scr_VerifyWeaponIndex(weaponIndex, weaponName);
    weapDef = BG_GetWeaponDef(weaponIndex);
    Scr_AddInt(weapDef->fireType == WEAPON_FIRETYPE_SINGLESHOT, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WeaponIsBoltAction)
void GScr_WeaponIsBoltAction()
{
    char *weaponName;
    unsigned int weaponIndex;
    const WeaponDef *weapDef;

    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    Scr_VerifyWeaponIndex(weaponIndex, weaponName);
    weapDef = BG_GetWeaponDef(weaponIndex);
    Scr_AddInt(weapDef->bBoltAction, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetWeaponDef @ 8260B2C0)
static const WeaponDef *__cdecl GScr_LookupWeaponDef()
{
    char *weaponName;
    unsigned int weaponIndex;

    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    if ( !weaponIndex )
        Scr_Error("unknown weapon", 0);
    return BG_GetWeaponDef(weaponIndex);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WeaponClipType @ 8260B6E0)
void GScr_WeaponClipType()
{
    char *weaponName;
    unsigned int weaponIndex;
    const WeaponDef *weapDef;

    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    Scr_VerifyWeaponIndex(weaponIndex, weaponName);
    weapDef = BG_GetWeaponDef(weaponIndex);
    Scr_AddInt(weapDef->clipType, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WeaponDoGibbing @ 8260A9A8)
void GScr_WeaponDoGibbing()
{
    char *weaponName;
    unsigned int weaponIndex;
    const WeaponDef *weapDef;

    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    Scr_VerifyWeaponIndex(weaponIndex, weaponName);
    weapDef = BG_GetWeaponDef(weaponIndex);
    Scr_AddBool(weapDef->doGibbing, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WeaponMaxGibDistance @ 8260AA08)
void GScr_WeaponMaxGibDistance()
{
    const WeaponDef *weapDef;

    weapDef = GScr_LookupWeaponDef();
    Scr_AddFloat(weapDef->maxGibDistance, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WeaponIsSniperWeapon @ 8260AC90)
void GScr_WeaponIsSniperWeapon()
{
    char *weaponName;
    unsigned int weaponIndex;
    const WeaponDef *weapDef;

    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    if ( !weaponIndex )
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
        return;
    }
    weapDef = BG_GetWeaponDef(weaponIndex);
    Scr_AddInt(BG_GetPlayerAnimTypeIndex(1) == weapDef->playerAnimType, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WeaponHasBayonet @ 8260ADA0)
void GScr_WeaponHasBayonet()
{
    char *weaponName;
    unsigned int weaponIndex;

    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    if ( !weaponIndex )
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
        return;
    }
    Scr_AddInt(BG_IsBayonetWeapon(weaponIndex), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WeaponIsDualWield @ 8260AE18)
void GScr_WeaponIsDualWield()
{
    char *weaponName;
    unsigned int weaponIndex;
    const WeaponDef *weapDef;

    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    Scr_VerifyWeaponIndex(weaponIndex, weaponName);
    weapDef = BG_GetWeaponDef(weaponIndex);
    Scr_AddInt(weapDef->bDualWield, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WeaponFightDist @ 8260B290)
void GScr_WeaponFightDist()
{
    const WeaponDef *weapDef;

    weapDef = GScr_LookupWeaponDef();
    Scr_AddFloat(weapDef->fightDist, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WeaponMaxDist @ 8260B318)
void GScr_WeaponMaxDist()
{
    const WeaponDef *weapDef;

    weapDef = GScr_LookupWeaponDef();
    Scr_AddFloat(weapDef->maxDist, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WeaponType)
void GScr_WeaponType()
{
    char *WeaponTypeName;
    char *weaponName;
    unsigned int weaponIndex;
    const WeaponDef *weapDef;

    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    Scr_VerifyWeaponIndex(weaponIndex, weaponName);
    weapDef = BG_GetWeaponDef(weaponIndex);
    WeaponTypeName = (char *)BG_GetWeaponTypeName(weapDef->weapType);
    Scr_AddString(WeaponTypeName, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WeaponClass)
void GScr_WeaponClass()
{
    char *WeaponClassName;
    char *weaponName;
    unsigned int weaponIndex;
    const WeaponDef *weapDef;

    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    if ( weaponIndex )
    {
        weapDef = BG_GetWeaponDef(weaponIndex);
        WeaponClassName = (char *)BG_GetWeaponClassName(weapDef->weapClass);
        Scr_AddString(WeaponClassName, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddString("none", SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WeaponIsMountable)
void GScr_WeaponIsMountable()
{
    char *weaponName;
    unsigned int weaponIndex;
    const WeaponDef *weapDef;

    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    weapDef = BG_GetWeaponDef(weaponIndex);
    Scr_AddInt(weapDef->mountableWeapon, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WeaponInventoryType)
void GScr_WeaponInventoryType()
{
    char *WeaponInventoryTypeName;
    char *weaponName;
    unsigned int weaponIndex;
    const WeaponDef *weapDef;

    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    Scr_VerifyWeaponIndex(weaponIndex, weaponName);
    weapDef = BG_GetWeaponDef(weaponIndex);
    WeaponInventoryTypeName = (char *)BG_GetWeaponInventoryTypeName(weapDef->inventoryType);
    Scr_AddString(WeaponInventoryTypeName, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WeaponStartAmmo)
void GScr_WeaponStartAmmo()
{
    int StartAmmo;
    char *weaponName;
    unsigned int weaponIndex;

    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    Scr_VerifyWeaponIndex(weaponIndex, weaponName);
    StartAmmo = BG_GetStartAmmo(weaponIndex);
    Scr_AddInt(StartAmmo, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WeaponMaxAmmo)
void GScr_WeaponMaxAmmo()
{
    int MaxAmmo;
    char *weaponName;
    unsigned int weaponIndex;

    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    Scr_VerifyWeaponIndex(weaponIndex, weaponName);
    MaxAmmo = BG_GetMaxAmmo(weaponIndex);
    Scr_AddInt(MaxAmmo, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WeaponAltWeaponName)
void GScr_WeaponAltWeaponName()
{
    char *tempValue0;
    char *weaponName;
    int altWeaponIndex;
    unsigned int weaponIndex;

    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    Scr_VerifyWeaponIndex(weaponIndex, weaponName);
    altWeaponIndex = BG_GetWeaponVariantDef(weaponIndex)->altWeaponIndex;
    if ( altWeaponIndex )
    {
        tempValue0 = (char *)BG_WeaponName(altWeaponIndex);
        Scr_AddString(tempValue0, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddConstString(scr_const.none, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetWatcherWeapons)
void GScr_GetWatcherWeapons()
{
    char *tempValue0;
    unsigned int i;

    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for ( i = 0; i < bg_lastParsedWeaponIndex; ++i )
    {
        if ( BG_GetWeaponDef(i)->bDieOnRespawn )
        {
            tempValue0 = (char *)BG_WeaponName(i);
            Scr_AddString(tempValue0, SCRIPTINSTANCE_SERVER);
            Scr_AddArray(SCRIPTINSTANCE_SERVER);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetRetrievableWeapons)
void GScr_GetRetrievableWeapons()
{
    char *tempValue0;
    unsigned int i;

    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for ( i = 0; i < bg_lastParsedWeaponIndex; ++i )
    {
        if ( BG_GetWeaponDef(i)->bRetrievable )
        {
            tempValue0 = (char *)BG_WeaponName(i);
            Scr_AddString(tempValue0, SCRIPTINSTANCE_SERVER);
            Scr_AddArray(SCRIPTINSTANCE_SERVER);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetWeaponIndexFromName)
void GScr_GetWeaponIndexFromName()
{
    char *weaponName;
    int weaponIndex;

    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    Scr_AddInt(weaponIndex, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetWeaponFireSound)
void GScr_GetWeaponFireSound()
{
    char *weaponFireSound;
    int weaponIndex;

    weaponFireSound = (char *)"";
    weaponIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( weaponIndex && weaponIndex != -1 )
        weaponFireSound = (char *)BG_GetWeaponDef(weaponIndex)->fireSound;
    Scr_AddString(weaponFireSound, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetWeaponFireSoundPlayer)
void GScr_GetWeaponFireSoundPlayer()
{
    int weaponIndex;
    char *weaponFireSoundPlayer;

    weaponFireSoundPlayer = (char *)"";
    weaponIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( weaponIndex && weaponIndex != -1 )
        weaponFireSoundPlayer = (char *)BG_GetWeaponDef(weaponIndex)->fireSoundPlayer;
    Scr_AddString(weaponFireSoundPlayer, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetWeaponPickupSound)
void GScr_GetWeaponPickupSound()
{
    int weaponIndex;
    char *weaponPickupSound;

    weaponPickupSound = (char *)"";
    weaponIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( weaponIndex && weaponIndex != -1 )
        weaponPickupSound = (char *)BG_GetWeaponDef(weaponIndex)->pickupSound;
    Scr_AddString(weaponPickupSound, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetWeaponPickupSoundPlayer)
void GScr_GetWeaponPickupSoundPlayer()
{
    int weaponIndex;
    char *weaponPickupSoundPlayer;

    weaponPickupSoundPlayer = (char *)"";
    weaponIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( weaponIndex && weaponIndex != -1 )
        weaponPickupSoundPlayer = (char *)BG_GetWeaponDef(weaponIndex)->pickupSoundPlayer;
    Scr_AddString(weaponPickupSoundPlayer, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsTurretActive)
void GScr_IsTurretActive()
{
    gentity_s *ent;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("illegal call to isturretactive()\n", 0);
    ent = Scr_GetEntity(0);
    if ( !ent )
        Scr_Error("NULL turret passed to isturretactive", 0);
    Scr_AddBool(ent->active != 0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsTurretFiring)
void GScr_IsTurretFiring()
{
    char *slStr;
    const char *fmtMsg;
    gentity_s *ent;
    bool firing;

    ent = Scr_GetEntity(0);
    if ( !ent->pTurretInfo )
    {
        slStr = SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("entity type '%s' is not a turret", slStr);
        Scr_Error(fmtMsg, 0);
    }
    if ( (ent->s.lerp.eFlags & 0x40) != 0 )
    {
        firing = 1;
    }
    else if ( ent->pTurretInfo->state )
    {
        firing = 1;
    }
    else
    {
        firing = (ent->pTurretInfo->flags & 4) != 0;
    }
    Scr_AddBool(firing, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetDefaultDropPitch)
void __cdecl GScr_SetDefaultDropPitch(scr_entref_t entref)
{
    float pitch;
    gentity_s *ent;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        ent = GetEntity(entref);
        if ( ent->pTurretInfo )
        {
            pitch = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
            turret_SetDefaultDropPitch(ent, pitch);
        }
        else
        {
            Scr_Error("entity is not a turret", 0);
        }
    }
    else
    {
        Scr_Error("illegal call to setdefaultdroppitch()\n", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetScanningPitch)
void __cdecl GScr_SetScanningPitch(scr_entref_t entref)
{
    float pitch;
    gentity_s *ent;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        ent = GetEntity(entref);
        if ( ent->pTurretInfo )
        {
            pitch = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
            turret_SetScanningPitch(ent, pitch);
        }
        else
        {
            Scr_Error("entity is not a turret", 0);
        }
    }
    else
    {
        Scr_Error("illegal call to setdefaultdroppitch()\n", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WeaponFireTime)
void GScr_WeaponFireTime()
{
    float value; // xmm0_4
    unsigned int iWeaponIndex;
    char *pszWeaponName;

    pszWeaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = G_GetWeaponIndexForName(pszWeaponName);
    if ( iWeaponIndex )
    {
        value = (float)BG_GetWeaponDef(iWeaponIndex)->iFireTime * 0.001;
        Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddFloat(0.0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WeaponReloadTime)
void GScr_WeaponReloadTime()
{
    float value; // xmm0_4
    char *weaponName;
    unsigned int weaponIndex;

    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    Scr_VerifyWeaponIndex(weaponIndex, weaponName);
    if ( weaponIndex )
    {
        value = (float)BG_GetWeaponDef(weaponIndex)->iReloadAddTime * 0.001;
        Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddFloat(0.0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsWeaponClipOnly)
void GScr_IsWeaponClipOnly()
{
    bool IsClipOnly;
    char *weapName;
    unsigned int weapIdx;

    weapName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weapIdx = G_GetWeaponIndexForName(weapName);
    if ( weapIdx )
    {
        IsClipOnly = BG_WeaponIsClipOnly(weapIdx);
        Scr_AddBool(IsClipOnly, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddBool(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsWeaponDetonationTimed)
void GScr_IsWeaponDetonationTimed()
{
    char *weapName;
    unsigned int weapIdx;
    const WeaponDef *weapDef;

    weapName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weapIdx = G_GetWeaponIndexForName(weapName);
    if ( weapIdx )
    {
        weapDef = BG_GetWeaponDef(weapIdx);
        Scr_AddBool(weapDef->timedDetonation, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddBool(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_PrecacheLocationSelector)
void GScr_PrecacheLocationSelector()
{
    const char *fmtMsg;
    int configStringIndex;
    int firstFreeConfigStringIndex;
    char szConfigString[1024];
    const char *pszNewMtl;

    pszNewMtl = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    for ( configStringIndex = 0; configStringIndex < 15; ++configStringIndex )
    {
        SV_GetConfigstring(configStringIndex + 1553, szConfigString, 1024);
        if ( !I_stricmp(szConfigString, pszNewMtl) )
        {
            Com_DPrintf(24, "Script tried to precache the location selector '%s' more than once\n", pszNewMtl);
            return;
        }
    }
    for ( firstFreeConfigStringIndex = 0; firstFreeConfigStringIndex < 15; ++firstFreeConfigStringIndex )
    {
        SV_GetConfigstring(firstFreeConfigStringIndex + 1553, szConfigString, 1024);
        if ( !szConfigString[0] )
            break;
    }
    if ( firstFreeConfigStringIndex == 15 )
    {
        fmtMsg = va("Too many location selectors precached. Max allowed is %i", 15);
        Scr_Error(fmtMsg, 0);
    }
    SV_SetConfigstring(firstFreeConfigStringIndex + 1553, (char *)pszNewMtl);
}

// Decomp: CoDSP_rdBlackOps.map.c:9456
int __cdecl GScr_GetLocSelIndex(const char *mtlName)
{
    const char *fmtMsg;
    int iConfigNum;
    char szConfigString[1028];

    if ( !mtlName
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 6760, 0, "%s", "mtlName") )
    {
        __debugbreak();
    }
    if ( !*mtlName )
        return 0;
    for ( iConfigNum = 0; iConfigNum < 15; ++iConfigNum )
    {
        SV_GetConfigstring(iConfigNum + 1553, szConfigString, 1024);
        if ( !I_stricmp(szConfigString, mtlName) )
            return iConfigNum + 1;
    }
    fmtMsg = va("Location selector '%s' was not precached\n", mtlName);
    Scr_Error(fmtMsg, 0);
    return 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_BulletTrace)
void Scr_BulletTrace()
{
    char *value;
    float vNorm[3];
    int bIgnoreWater;
    float vEnd[3];
    int bIgnoreGlass;
    gentity_s *pIgnoreEnt;
    int iClipMask;
    trace_t trace;
    float endpos[3];
    int iIgnoreEntNum;
    int iSurfaceTypeIndex;
    float vStart[3];
    unsigned __int16 hitEntId;

    PROF_SCOPED("Scr_BulletTrace");

    pIgnoreEnt = 0;
    iIgnoreEntNum = 1023;
    iClipMask = 0x280E033;
    memset(&trace, 0, 16);
    Scr_GetVector(0, vStart, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, vEnd, SCRIPTINSTANCE_SERVER);
    if ( !Scr_GetInt(2u, SCRIPTINSTANCE_SERVER) )
        iClipMask &= 0xFDFF7FFF;
    if ( Scr_GetType(3u, SCRIPTINSTANCE_SERVER) == 1 && Scr_GetPointerType(3u, SCRIPTINSTANCE_SERVER) == 19 )
    {
        pIgnoreEnt = Scr_GetEntity(3u);
        iIgnoreEntNum = pIgnoreEnt->s.number;
    }
    bIgnoreWater = 0;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 5 )
        bIgnoreWater = Scr_GetInt(4u, SCRIPTINSTANCE_SERVER);
    if ( bIgnoreWater )
        iClipMask &= ~0x20u;
    bIgnoreGlass = 0;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 6 )
        bIgnoreGlass = Scr_GetInt(5u, SCRIPTINSTANCE_SERVER);
    if ( bIgnoreGlass )
        iClipMask &= ~0x10u;
    G_LocationalTrace(&trace, vStart, vEnd, iIgnoreEntNum, iClipMask, 0, 0);
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    Scr_AddFloat(trace.fraction, SCRIPTINSTANCE_SERVER);
    Scr_AddArrayStringIndexed(scr_const.fraction, SCRIPTINSTANCE_SERVER);
    Vec3Lerp(vStart, vEnd, trace.fraction, endpos);
    Scr_AddVector(endpos, SCRIPTINSTANCE_SERVER);
    Scr_AddArrayStringIndexed(scr_const.position, SCRIPTINSTANCE_SERVER);
    hitEntId = Trace_GetEntityHitId(&trace);
    if ( hitEntId == 1023 || hitEntId == 1022 )
        Scr_AddUndefined(SCRIPTINSTANCE_SERVER);
    else
        Scr_AddEntity(&g_entities[hitEntId], SCRIPTINSTANCE_SERVER);
    Scr_AddArrayStringIndexed(scr_const.entity, SCRIPTINSTANCE_SERVER);
    if ( trace.fraction >= 1.0 )
    {
        vNorm[0] = vEnd[0] - vStart[0];
        vNorm[1] = vEnd[1] - vStart[1];
        vNorm[2] = vEnd[2] - vStart[2];
        Vec3Normalize(vNorm);
        Scr_AddVector(vNorm, SCRIPTINSTANCE_SERVER);
        Scr_AddArrayStringIndexed(scr_const.normal, SCRIPTINSTANCE_SERVER);
        Scr_AddConstString(scr_const.none, SCRIPTINSTANCE_SERVER);
        Scr_AddArrayStringIndexed(scr_const.surfacetype, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddVector(trace.normal.vec.v, SCRIPTINSTANCE_SERVER);
        Scr_AddArrayStringIndexed(scr_const.normal, SCRIPTINSTANCE_SERVER);
        iSurfaceTypeIndex = (unsigned __int8)((int)(0x3F00000 & trace.sflags) >> 20);
        value = (char *)Com_SurfaceTypeToName(iSurfaceTypeIndex);
        Scr_AddString(value, SCRIPTINSTANCE_SERVER);
        Scr_AddArrayStringIndexed(scr_const.surfacetype, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GroundTrace)
void Scr_GroundTrace()
{
    float vNorm[3];
    int bIgnoreWater;
    float vEnd[3];
    int bIgnoreGlass;
    gentity_s *pIgnoreEnt;
    int iClipMask;
    trace_t trace;
    float endpos[3];
    int iIgnoreEntNum;
    int iSurfaceTypeIndex;
    float vStart[3];
    unsigned __int16 hitEntId;

    PROF_SCOPED("Scr_GroundTrace");

    pIgnoreEnt = 0;
    iIgnoreEntNum = ENTITYNUM_NONE;
    iClipMask = 0x280E033;
    memset(&trace, 0, sizeof(trace));
    Scr_GetVector(0, vStart, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, vEnd, SCRIPTINSTANCE_SERVER);
    if ( !Scr_GetInt(2u, SCRIPTINSTANCE_SERVER) )
        iClipMask &= 0xFDFF3FFF;
    if ( Scr_GetType(3u, SCRIPTINSTANCE_SERVER) == 1 && Scr_GetPointerType(3u, SCRIPTINSTANCE_SERVER) == 19 )
    {
        pIgnoreEnt = Scr_GetEntity(3u);
        iIgnoreEntNum = pIgnoreEnt->s.number;
    }
    bIgnoreWater = 0;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 5 )
        bIgnoreWater = Scr_GetInt(4u, SCRIPTINSTANCE_SERVER);
    if ( bIgnoreWater )
        iClipMask &= ~0x20u;
    bIgnoreGlass = 0;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 6 )
        bIgnoreGlass = Scr_GetInt(5u, SCRIPTINSTANCE_SERVER);
    if ( bIgnoreGlass )
        iClipMask &= ~0x10u;
    G_LocationalTrace(&trace, vStart, vEnd, iIgnoreEntNum, iClipMask, 0, 0);
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    Scr_AddFloat(trace.fraction, SCRIPTINSTANCE_SERVER);
    Scr_AddArrayStringIndexed(scr_const.fraction, SCRIPTINSTANCE_SERVER);
    Vec3Lerp(vStart, vEnd, trace.fraction, endpos);
    Scr_AddVector(endpos, SCRIPTINSTANCE_SERVER);
    Scr_AddArrayStringIndexed(scr_const.position, SCRIPTINSTANCE_SERVER);
    hitEntId = Trace_GetEntityHitId(&trace);
    if ( hitEntId == ENTITYNUM_NONE || hitEntId == ENTITYNUM_WORLD )
        Scr_AddUndefined(SCRIPTINSTANCE_SERVER);
    else
        Scr_AddEntity(&g_entities[hitEntId], SCRIPTINSTANCE_SERVER);
    Scr_AddArrayStringIndexed(scr_const.entity, SCRIPTINSTANCE_SERVER);
    if ( trace.fraction >= 1.0f )
    {
        vNorm[0] = vEnd[0] - vStart[0];
        vNorm[1] = vEnd[1] - vStart[1];
        vNorm[2] = vEnd[2] - vStart[2];
        Vec3Normalize(vNorm);
        Scr_AddVector(vNorm, SCRIPTINSTANCE_SERVER);
        Scr_AddArrayStringIndexed(scr_const.normal, SCRIPTINSTANCE_SERVER);
        Scr_AddConstString(scr_const.none, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddVector(trace.normal.vec.v, SCRIPTINSTANCE_SERVER);
        Scr_AddArrayStringIndexed(scr_const.normal, SCRIPTINSTANCE_SERVER);
        iSurfaceTypeIndex = (unsigned __int8)((int)(0x3F00000 & trace.sflags) >> 20);
        Scr_AddString((char *)Com_SurfaceTypeToName(iSurfaceTypeIndex), SCRIPTINSTANCE_SERVER);
    }
    Scr_AddArrayStringIndexed(scr_const.surfacetype, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_BulletTracePassed)
void Scr_BulletTracePassed()
{
    bool tracePassed;
    int hitnum;
    float vEnd[3];
    gentity_s *pIgnoreEnt;
    int iClipMask;
    int iIgnoreEntNum;
    float vStart[3];

    pIgnoreEnt = 0;
    iIgnoreEntNum = 1023;
    iClipMask = 0x280E033;
    Scr_GetVector(0, vStart, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, vEnd, SCRIPTINSTANCE_SERVER);
    if ( !Scr_GetInt(2u, SCRIPTINSTANCE_SERVER) )
        iClipMask &= 0xFDFF7FFF;
    if ( Scr_GetType(3u, SCRIPTINSTANCE_SERVER) == 1 && Scr_GetPointerType(3u, SCRIPTINSTANCE_SERVER) == 19 )
    {
        pIgnoreEnt = Scr_GetEntity(3u);
        iIgnoreEntNum = pIgnoreEnt->s.number;
    }

    //col_context_t::col_context_t(&context, iClipMask);
    col_context_t context(iClipMask);
    //col_context_t::init_locational(&context, iIgnoreEntNum);
    context.init_locational(iIgnoreEntNum);
    hitnum = -1;
    tracePassed = SV_SightTracePoint(&hitnum, vStart, vEnd, &context);
    Scr_AddBool(tracePassed, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SightTracePassed)
void __cdecl Scr_SightTracePassed()
{
    col_context_t context;
    float vEnd[3];
    int iClipMask;
    int iIgnoreEntNum;
    int hitNum;
    float vStart[3];

    iIgnoreEntNum = 1023;
    iClipMask = 0x2809803;
    Scr_GetVector(0, vStart, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, vEnd, SCRIPTINSTANCE_SERVER);
    if ( !Scr_GetInt(2u, SCRIPTINSTANCE_SERVER) )
        iClipMask &= 0xFDFF7FFF;
    if ( Scr_GetType(3u, SCRIPTINSTANCE_SERVER) == 1 && Scr_GetPointerType(3u, SCRIPTINSTANCE_SERVER) == 19 )
        iIgnoreEntNum = Scr_GetEntity(3u)->s.number;
    context = col_context_t(iClipMask);
    context.init_locational(iIgnoreEntNum);
    hitNum = 0;
    SV_SightTracePoint(&hitNum, vStart, vEnd, &context);
    Scr_AddBool(hitNum == 0, SCRIPTINSTANCE_SERVER);
}

const int scriptMaskToPhysicsMask[3] = { 42003603, 529, 32 };

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PhysicsTrace)
void Scr_PhysicsTrace()
{
    char *surfaceTypeName;
    float vNorm[3];
    int i;
    col_context_t context;
    float mins[3];
    float start[3];
    float end[3];
    int maskType;
    gentity_s *pIgnoreEnt;
    float endpos[3];
    trace_t trace;
    float maxs[3];
    int mask;
    int iIgnoreEntNum;
    int iSurfaceTypeIndex;
    unsigned __int16 hitEntId;

    memset(&trace, 0, 16);
    maskType = 1;
    pIgnoreEnt = 0;
    iIgnoreEntNum = 1023;

    const int numParams = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParams < 2 || numParams > 6 )
        Scr_Error("illegal call to PhysicsTrace()", 0);

    Scr_GetVector(0, start, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, end, SCRIPTINSTANCE_SERVER);
    memset(mins, 0, sizeof(mins));
    memset(maxs, 0, sizeof(maxs));

    if ( numParams >= 3 )
        Scr_GetVector(2u, mins, SCRIPTINSTANCE_SERVER);
    if ( numParams >= 4 )
        Scr_GetVector(3u, maxs, SCRIPTINSTANCE_SERVER);
    if ( numParams >= 5
        && Scr_GetType(4u, SCRIPTINSTANCE_SERVER) == 1
        && Scr_GetPointerType(4u, SCRIPTINSTANCE_SERVER) == 19 )
    {
        pIgnoreEnt = Scr_GetEntity(4u);
        iIgnoreEntNum = pIgnoreEnt->s.number;
    }
    if ( numParams >= 6 )
        maskType = Scr_GetInt(5u, SCRIPTINSTANCE_SERVER);
    mask = 0;
    for ( i = 0; (unsigned int)i < 3; ++i )
    {
        if ( (maskType & (1 << i)) != 0 )
            mask |= scriptMaskToPhysicsMask[i];
    }
    //col_context_t::col_context_t(&context);
    G_TraceCapsule(&trace, start, mins, maxs, end, iIgnoreEntNum, mask, &context);
    Vec3Lerp(start, end, trace.fraction, endpos);
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    Scr_AddFloat(trace.fraction, SCRIPTINSTANCE_SERVER);
    Scr_AddArrayStringIndexed(scr_const.fraction, SCRIPTINSTANCE_SERVER);
    Scr_AddVector(endpos, SCRIPTINSTANCE_SERVER);
    Scr_AddArrayStringIndexed(scr_const.position, SCRIPTINSTANCE_SERVER);
    hitEntId = Trace_GetEntityHitId(&trace);
    if ( hitEntId == 1023 || hitEntId == 1022 )
        Scr_AddUndefined(SCRIPTINSTANCE_SERVER);
    else
        Scr_AddEntity(&g_entities[hitEntId], SCRIPTINSTANCE_SERVER);
    Scr_AddArrayStringIndexed(scr_const.entity, SCRIPTINSTANCE_SERVER);
    if ( trace.fraction >= 1.0 )
    {
        vNorm[0] = end[0] - start[0];
        vNorm[1] = end[1] - start[1];
        vNorm[2] = end[2] - start[2];
        Vec3Normalize(vNorm);
        Scr_AddVector(vNorm, SCRIPTINSTANCE_SERVER);
        Scr_AddArrayStringIndexed(scr_const.normal, SCRIPTINSTANCE_SERVER);
        Scr_AddConstString(scr_const.none, SCRIPTINSTANCE_SERVER);
        Scr_AddArrayStringIndexed(scr_const.surfacetype, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddVector(trace.normal.vec.v, SCRIPTINSTANCE_SERVER);
        Scr_AddArrayStringIndexed(scr_const.normal, SCRIPTINSTANCE_SERVER);
        iSurfaceTypeIndex = (unsigned __int8)((int)(0x3F00000 & trace.sflags) >> 20);
        surfaceTypeName = (char *)Com_SurfaceTypeToName(iSurfaceTypeIndex);
        Scr_AddString(surfaceTypeName, SCRIPTINSTANCE_SERVER);
        Scr_AddArrayStringIndexed(scr_const.surfacetype, SCRIPTINSTANCE_SERVER);
    }
}

// LWSS ADD
void Scr_PlayerBulletTrace()
{
    char *surfaceName;
    float endPos[3];
    float traceDir[3];
    gentity_s *ignoreEnt;
    int ignoreEntNum;
    int clipMask;
    trace_t trace;
    float start[3];
    float end[3];
    unsigned __int16 hitEntId;
    int surfaceTypeIndex;

    ignoreEnt = 0;
    ignoreEntNum = 1023;
    clipMask = 0x280E033;
    memset(&trace, 0, 16);
    Scr_GetVector(0, start, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, end, SCRIPTINSTANCE_SERVER);
    if ( !Scr_GetInt(2u, SCRIPTINSTANCE_SERVER) )
        clipMask &= 0xFDFF7FFF;
    if ( Scr_GetType(3u, SCRIPTINSTANCE_SERVER) == 1 && Scr_GetPointerType(3u, SCRIPTINSTANCE_SERVER) == 19 )
    {
        ignoreEnt = Scr_GetEntity(3u);
        ignoreEntNum = ignoreEnt->s.number;
    }
    G_TraceCapsule(&trace, start, playerMins, playerMaxs, end, ignoreEntNum, clipMask, 0);
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    Scr_AddFloat(trace.fraction, SCRIPTINSTANCE_SERVER);
    Scr_AddArrayStringIndexed(scr_const.fraction, SCRIPTINSTANCE_SERVER);
    Vec3Lerp(start, end, trace.fraction, endPos);
    Scr_AddVector(endPos, SCRIPTINSTANCE_SERVER);
    Scr_AddArrayStringIndexed(scr_const.position, SCRIPTINSTANCE_SERVER);
    hitEntId = Trace_GetEntityHitId(&trace);
    if ( hitEntId == 1023 || hitEntId == 1022 )
        Scr_AddUndefined(SCRIPTINSTANCE_SERVER);
    else
        Scr_AddEntity(&g_entities[hitEntId], SCRIPTINSTANCE_SERVER);
    Scr_AddArrayStringIndexed(scr_const.entity, SCRIPTINSTANCE_SERVER);
    if ( trace.fraction >= 1.0 )
    {
        traceDir[0] = end[0] - start[0];
        traceDir[1] = end[1] - start[1];
        traceDir[2] = end[2] - start[2];
        Vec3Normalize(traceDir);
        Scr_AddVector(traceDir, SCRIPTINSTANCE_SERVER);
        Scr_AddArrayStringIndexed(scr_const.normal, SCRIPTINSTANCE_SERVER);
        Scr_AddConstString(scr_const.none, SCRIPTINSTANCE_SERVER);
        Scr_AddArrayStringIndexed(scr_const.surfacetype, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddVector(trace.normal.vec.v, SCRIPTINSTANCE_SERVER);
        Scr_AddArrayStringIndexed(scr_const.normal, SCRIPTINSTANCE_SERVER);
        surfaceTypeIndex = (unsigned __int8)((int)(0x3F00000 & trace.sflags) >> 20);
        surfaceName = (char *)Com_SurfaceTypeToName(surfaceTypeIndex);
        Scr_AddString(surfaceName, SCRIPTINSTANCE_SERVER);
        Scr_AddArrayStringIndexed(scr_const.surfacetype, SCRIPTINSTANCE_SERVER);
    }
}
// LWSS END

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PlayerPhysicsTrace)
void Scr_PlayerPhysicsTrace()
{
    col_context_t context;
    float start[3];
    float end[3];
    float endpos[3];
    trace_t trace;

    memset(&trace, 0, 16);
    Scr_GetVector(0, start, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, end, SCRIPTINSTANCE_SERVER);
    //col_context_t::col_context_t(&context);
    G_TraceCapsule(&trace, start, playerMins, playerMaxs, end, 1023, 8519697, &context);
    Vec3Lerp(start, end, trace.fraction, endpos);
    Scr_AddVector(endpos, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_RandomInt)
void Scr_RandomInt()
{
    int randomValue;
    int iMax;

    iMax = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( iMax > 0 )
    {
        randomValue = G_irand(0, iMax);
        Scr_AddInt(randomValue, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Com_Printf(24, "RandomInt parm: %d    ", iMax);
        Scr_Error("RandomInt parm must be positive integer.\n", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_RandomFloat)
void Scr_RandomFloat()
{
    float max;
    float fMax;

    fMax = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    max = G_flrand(0.0, fMax);
    Scr_AddFloat(max, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_RandomIntRange)
void Scr_RandomIntRange()
{
    int randomValue;
    int iMax;
    int iMin;

    iMin = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    iMax = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    if ( iMax <= iMin )
    {
        Com_Printf(24, "RandomIntRange parms: %d %d ", iMin, iMax);
        Scr_Error("RandomIntRange range must be positive integer.\n", 0);
    }
    randomValue = G_irand(iMin, iMax);
    Scr_AddInt(randomValue, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_RandomFloatRange)
void Scr_RandomFloatRange()
{
    float max;
    float fMin;
    float fMax;

    fMin = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    fMax = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    if ( fMin > fMax )
    {
        Com_Printf(24, "Scr_RandomFloatRange parms: %f %f ", fMin, fMax);
        Scr_Error("Scr_RandomFloatRange range must be positive float.\n", 0);
    }
    max = G_flrand(fMin, fMax);
    Scr_AddFloat(max, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_log)
void GScr_log()
{
#if 0
    double value; // xmm0_8
    long double tempValue1;
    scriptInstance_t tempValue2;

    *((float *)&tempValue1 + 1) = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    LODWORD(tempValue1) = 0;
    value = *((float *)&tempValue1 + 1);
    __libm_sse2_log(tempValue1);
    *(float *)&value = value;
    Scr_AddFloat(*(float *)&value, tempValue2);
#endif

    float value = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    value = log(value);
    Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_sin)
void GScr_sin()
{
#if 0
    double value; // xmm0_8
    long double tempValue1;
    scriptInstance_t tempValue2;

    *((float *)&tempValue1 + 1) = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER) * 0.017453292;
    LODWORD(tempValue1) = 0;
    value = *((float *)&tempValue1 + 1);
    __libm_sse2_sin(tempValue1);
    *(float *)&value = value;
    Scr_AddFloat(*(float *)&value, tempValue2);
#endif

    float value = DEG2RAD(Scr_GetFloat(0, SCRIPTINSTANCE_SERVER));
    value = sin(value);
    Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_cos)
void GScr_cos()
{
#if 0
    double value; // xmm0_8
    long double tempValue1;
    scriptInstance_t tempValue2;

    *((float *)&tempValue1 + 1) = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER) * 0.017453292;
    LODWORD(tempValue1) = 0;
    value = *((float *)&tempValue1 + 1);
    __libm_sse2_cos(tempValue1);
    *(float *)&value = value;
    Scr_AddFloat(*(float *)&value, tempValue2);
#endif

    float value = DEG2RAD(Scr_GetFloat(0, SCRIPTINSTANCE_SERVER));
    value = cos(value);
    Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_tan)
void GScr_tan()
{
    float tempValue0;
    float sinT;
    float cosT;

    tempValue0 = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER) * 0.017453292;
    cosT = cos(tempValue0);
    sinT = sin(tempValue0);
    if ( cosT == 0.0 )
        Scr_Error("divide by 0", 0);
    Scr_AddFloat(sinT / cosT, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_asin)
void GScr_asin()
{
    const char *fmtMsg;
    float value;

    value = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    if ( value < -1.0f || value > 1.0f )
    {
        fmtMsg = va("%g out of range", value);
        Scr_Error(fmtMsg, 0);
    }
    Scr_AddFloat(RAD2DEG(asin(value)), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_acos)
void GScr_acos()
{
    const char *fmtMsg;
    float value;

    value = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    if ( value < -1.0f || value > 1.0f )
    {
        fmtMsg = va("%g out of range", value);
        Scr_Error(fmtMsg, 0);
    }
    Scr_AddFloat(RAD2DEG(acos(value)), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_atan)
void GScr_atan()
{
#if 0
    double tempValue0; // xmm0_8
    long double tempValue1;
    scriptInstance_t tempValue2;

    *((float *)&tempValue1 + 1) = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    LODWORD(tempValue1) = 0;
    tempValue0 = *((float *)&tempValue1 + 1);
    __libm_sse2_atan(tempValue1);
    *(float *)&tempValue0 = tempValue0;
    Scr_AddFloat(*(float *)&tempValue0 * 57.295776, tempValue2);
#endif
    float value = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    value = atan(value);
    Scr_AddFloat(RAD2DEG(value), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_abs)
void GScr_abs()
{
    float Float;

    Float = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    Scr_AddFloat(fabs(Float), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_min)
void GScr_min()
{
    float value;
    float Float;
    float tempValue2;

    Float = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    tempValue2 = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    if ( (float)(tempValue2 - Float) < 0.0 )
        value = tempValue2;
    else
        value = Float;
    Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_max)
void GScr_max()
{
    float value;
    float Float;
    float tempValue2;

    Float = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    tempValue2 = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    if ( (float)(Float - tempValue2) < 0.0 )
        value = tempValue2;
    else
        value = Float;
    Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_floor)
void GScr_floor()
{
    float tempValue0;
    float Float;

    Float = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    tempValue0 = floor(Float);
    Scr_AddFloat(tempValue0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ceil)
void GScr_ceil()
{
    float tempValue0;
    float Float;

    Float = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    tempValue0 = ceil(Float);
    Scr_AddFloat(tempValue0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_sqrt)
void GScr_sqrt()
{
    float Float;

    Float = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    Scr_AddFloat(sqrtf(Float), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_CastInt)
void GScr_CastInt()
{
    VariableUnion varUnion;
    double Float;
    char *String;
    int tempValue3;
    const char *TypeName;
    const char *fmtMsg;
    int Type;

    Type = Scr_GetType(0, SCRIPTINSTANCE_SERVER);
    switch ( Type )
    {
        case 2:
            String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
            tempValue3 = atoi(String);
            Scr_AddInt(tempValue3, SCRIPTINSTANCE_SERVER);
            break;
        case 5:
            Float = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
            Scr_AddInt((int)Float, SCRIPTINSTANCE_SERVER);
            break;
        case 6:
            varUnion.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
            Scr_AddInt(varUnion.intValue, SCRIPTINSTANCE_SERVER);
            break;
        default:
            TypeName = Scr_GetTypeName(0, SCRIPTINSTANCE_SERVER);
            fmtMsg = va("cannot cast %s to int", TypeName);
            Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
            break;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_CastFloat)
void GScr_CastFloat()
{
    float intValue; // xmm0_4
    char *String;
    const char *TypeName;
    const char *fmtMsg;
    float value;
    float tempValue5;
    int Type;

    Type = Scr_GetType(0, SCRIPTINSTANCE_SERVER);
    switch ( Type )
    {
        case 2:
            String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
            tempValue5 = atof(String);
            Scr_AddFloat(tempValue5, SCRIPTINSTANCE_SERVER);
            break;
        case 5:
            value = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
            Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
            break;
        case 6:
            intValue = (float)Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
            Scr_AddFloat(intValue, SCRIPTINSTANCE_SERVER);
            break;
        default:
            TypeName = Scr_GetTypeName(0, SCRIPTINSTANCE_SERVER);
            fmtMsg = va("cannot cast %s to float", TypeName);
            Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
            break;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_VectorFromLineToPoint)
void GScr_VectorFromLineToPoint()
{
    float segmentB[3];
    float result[3];
    float BA[3];
    float PA[3];
    float fraction;
    float segmentLengthSq;
    float segmentA[3];
    float P[3];

    Scr_GetVector(0, segmentA, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, segmentB, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(2u, P, SCRIPTINSTANCE_SERVER);
    BA[0] = segmentB[0] - segmentA[0];
    BA[1] = segmentB[1] - segmentA[1];
    BA[2] = segmentB[2] - segmentA[2];
    segmentLengthSq = (float)((float)((float)(segmentB[0] - segmentA[0]) * (float)(segmentB[0] - segmentA[0]))
                                                    + (float)((float)(segmentB[1] - segmentA[1]) * (float)(segmentB[1] - segmentA[1])))
                                    + (float)((float)(segmentB[2] - segmentA[2]) * (float)(segmentB[2] - segmentA[2]));
    if ( segmentLengthSq == 0.0 )
        Scr_ParamError(0, "The two points on the line must be different from each other", SCRIPTINSTANCE_SERVER);
    PA[0] = P[0] - segmentA[0];
    PA[1] = P[1] - segmentA[1];
    PA[2] = P[2] - segmentA[2];
    fraction = (float)((float)((float)(BA[0] * (float)(P[0] - segmentA[0])) + (float)(BA[1] * (float)(P[1] - segmentA[1])))
                                     + (float)(BA[2] * (float)(P[2] - segmentA[2])))
                     / segmentLengthSq;
    result[0] = (float)((-(fraction)) * BA[0]) + (float)(P[0] - segmentA[0]);
    result[1] = (float)((-(fraction)) * BA[1]) + (float)(P[1] - segmentA[1]);
    result[2] = (float)((-(fraction)) * BA[2]) + (float)(P[2] - segmentA[2]);
    Scr_AddVector(result, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_PointOnSegmentNearestToPoint)
void GScr_PointOnSegmentNearestToPoint()
{
    float segmentB[3];
    float BA[3];
    float PA[3];
    float fraction;
    float segmentLengthSq;
    float segmentA[3];
    float P[3];
    float nearPoint[3];

    Scr_GetVector(0, segmentA, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, segmentB, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(2u, P, SCRIPTINSTANCE_SERVER);
    BA[0] = segmentB[0] - segmentA[0];
    BA[1] = segmentB[1] - segmentA[1];
    BA[2] = segmentB[2] - segmentA[2];
    segmentLengthSq = (float)((float)((float)(segmentB[0] - segmentA[0]) * (float)(segmentB[0] - segmentA[0]))
                                                    + (float)((float)(segmentB[1] - segmentA[1]) * (float)(segmentB[1] - segmentA[1])))
                                    + (float)((float)(segmentB[2] - segmentA[2]) * (float)(segmentB[2] - segmentA[2]));
    if ( segmentLengthSq == 0.0 )
        Scr_ParamError(0, "Line segment must not have zero length", SCRIPTINSTANCE_SERVER);
    PA[0] = P[0] - segmentA[0];
    PA[1] = P[1] - segmentA[1];
    PA[2] = P[2] - segmentA[2];
    fraction = (float)((float)((float)(BA[0] * (float)(P[0] - segmentA[0])) + (float)(BA[1] * (float)(P[1] - segmentA[1])))
                                     + (float)(BA[2] * (float)(P[2] - segmentA[2])))
                     / segmentLengthSq;
    if ( fraction >= 0.0 )
    {
        if ( fraction <= 1.0 )
        {
            nearPoint[0] = (float)(fraction * BA[0]) + segmentA[0];
            nearPoint[1] = (float)(fraction * BA[1]) + segmentA[1];
            nearPoint[2] = (float)(fraction * BA[2]) + segmentA[2];
            Scr_AddVector(nearPoint, SCRIPTINSTANCE_SERVER);
        }
        else
        {
            Scr_AddVector(segmentB, SCRIPTINSTANCE_SERVER);
        }
    }
    else
    {
        Scr_AddVector(segmentA, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Distance)
void Scr_Distance()
{
    float value;
    float tempValue0[3];
    float tempValue1[3];

    Scr_GetVector(0, tempValue0, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, tempValue1, SCRIPTINSTANCE_SERVER);
    value = Vec3Distance(tempValue0, tempValue1);
    Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Distance2D)
void Scr_Distance2D()
{
    float value;
    float v[2];
    float tempValue0[3];
    float tempValue1[3];

    Scr_GetVector(0, tempValue0, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, tempValue1, SCRIPTINSTANCE_SERVER);
    v[0] = tempValue1[0] - tempValue0[0];
    v[1] = tempValue1[1] - tempValue0[1];
    value = Vec2Length(v);
    Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Distance2DSquared ~82605DA8)
void Scr_Distance2DSquared()
{
    float value;
    float v[2];
    float tempValue0[3];
    float tempValue1[3];

    Scr_GetVector(0, tempValue0, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, tempValue1, SCRIPTINSTANCE_SERVER);
    v[0] = tempValue1[0] - tempValue0[0];
    v[1] = tempValue1[1] - tempValue0[1];
    value = Vec2LengthSq(v);
    Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_DistanceSquared)
void Scr_DistanceSquared()
{
    float value;
    float tempValue0[3];
    float tempValue1[3];

    Scr_GetVector(0, tempValue0, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, tempValue1, SCRIPTINSTANCE_SERVER);
    value = Vec3DistanceSq(tempValue0, tempValue1);
    Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_BulletSpread ~825FBE80)
void Scr_BulletSpread()
{
    float spread;
    float start[3];
    float target[3];
    float end[3];
    weaponParms wp;

    Scr_GetVector(0, start, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, target, SCRIPTINSTANCE_SERVER);
    spread = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    Weapon_SetWeaponParamsWeapon(&wp, 0);
    wp.muzzleTrace[0] = start[0];
    wp.muzzleTrace[1] = start[1];
    wp.muzzleTrace[2] = start[2];
    wp.forward[0] = target[0] - start[0];
    wp.forward[1] = target[1] - start[1];
    wp.forward[2] = target[2] - start[2];
    Vec3Normalize(wp.forward);
    PerpendicularVector(wp.forward, wp.up);
    Vec3Cross(wp.forward, wp.up, wp.right);
    Bullet_Endpos(level.time, spread, end, 0, &wp, sv_bullet_range->current.value);
    Scr_AddVector(end, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Length)
void Scr_Length()
{
    float value;
    float v[3];

    Scr_GetVector(0, v, SCRIPTINSTANCE_SERVER);
    value = Abs(v);
    Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_LengthSquared)
void Scr_LengthSquared()
{
    float v[3];

    Scr_GetVector(0, v, SCRIPTINSTANCE_SERVER);
    Scr_AddFloat((float)((float)(v[0] * v[0]) + (float)(v[1] * v[1])) + (float)(v[2] * v[2]), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Closer)
void Scr_Closer()
{
    float fDistBSqrd;
    float vB[3];
    float fDistASqrd;
    float vRef[3];
    float vA[3];

    Scr_GetVector(0, vRef, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, vA, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(2u, vB, SCRIPTINSTANCE_SERVER);
    fDistASqrd = Vec3DistanceSq(vA, vRef);
    fDistBSqrd = Vec3DistanceSq(vB, vRef);
    Scr_AddInt(fDistBSqrd > fDistASqrd, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_VectorDot)
void Scr_VectorDot()
{
    float b[3];
    float a[3];

    Scr_GetVector(0, a, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, b, SCRIPTINSTANCE_SERVER);
    Scr_AddFloat((float)((float)(a[0] * b[0]) + (float)(a[1] * b[1])) + (float)(a[2] * b[2]), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_VectorCross)
void Scr_VectorCross()
{
    float b[3];
    float tempVec[3];
    float a[3];

    Scr_GetVector(0, a, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, b, SCRIPTINSTANCE_SERVER);
    Vec3Cross(a, b, tempVec);
    Scr_AddVector(tempVec, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_VectorNormalize)
void Scr_VectorNormalize()
{
    float b[3];
    float a[3];

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("wrong number of arguments to vectornormalize!", 0);
    Scr_GetVector(0, a, SCRIPTINSTANCE_SERVER);
    b[0] = a[0];
    b[1] = a[1];
    b[2] = a[2];
    Vec3Normalize(b);
    Scr_AddVector(b, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_VectorToAngles)
void Scr_VectorToAngles()
{
    float angles[3];
    float vec[3];

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("wrong number of arguments to vectortoangle!", 0);
    Scr_GetVector(0, vec, SCRIPTINSTANCE_SERVER);
    vectoangles(vec, angles);
    Scr_AddVector(angles, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_VectorLerp)
void Scr_VectorLerp()
{
    float from[3];
    float result[3];
    float fraction;
    float to[3];

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 3 )
        Scr_Error("wrong number of arguments to vectorlerp", 0);
    Scr_GetVector(0, from, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, to, SCRIPTINSTANCE_SERVER);
    fraction = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    Vec3Lerp(from, to, fraction, result);
    Scr_AddVector(result, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_AnglesToUp)
void Scr_AnglesToUp()
{
    float angles[3];
    float up[3];

    Scr_GetVector(0, angles, SCRIPTINSTANCE_SERVER);
    AngleVectors(angles, 0, 0, up);
    Scr_AddVector(up, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_AnglesToRight)
void Scr_AnglesToRight()
{
    float right[3];
    float angles[3];

    Scr_GetVector(0, angles, SCRIPTINSTANCE_SERVER);
    AngleVectors(angles, 0, right, 0);
    Scr_AddVector(right, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_AnglesToForward)
void Scr_AnglesToForward()
{
    float forward[3];
    float angles[3];

    Scr_GetVector(0, angles, SCRIPTINSTANCE_SERVER);
    AngleVectors(angles, forward, 0, 0);
    Scr_AddVector(forward, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_CombineAngles)
void Scr_CombineAngles()
{
    float anglesfinal[3];
    float axisB[3][3];
    float anglesA[3];
    float axisA[3][3];
    float anglesB[3];
    float combinedaxis[3][3];

    Scr_GetVector(0, anglesA, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, anglesB, SCRIPTINSTANCE_SERVER);
    AnglesToAxis(anglesA, axisA);
    AnglesToAxis(anglesB, axisB);
    MatrixMultiply(axisB, axisA, combinedaxis);
    AxisToAngles(combinedaxis, anglesfinal);
    Scr_AddVector(anglesfinal, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_ClampAngle180)
void Scr_ClampAngle180()
{
    float tempValue0;
    float anglea;
    float angle;

    anglea = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    tempValue0 = floor((float)(anglea / 360.0));
    angle = ((float)(anglea / 360.0) - tempValue0) * 360.0;
    if ( angle <= 180.0 )
        Scr_AddFloat(angle, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddFloat(angle - 360.0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_AbsAngleClamp180)
void Scr_AbsAngleClamp180()
{
    float tempValue0;
    float anglea;
    float angle;

    anglea = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    tempValue0 = floor((float)(anglea / 360.0));
    angle = ((float)(anglea / 360.0) - tempValue0) * 360.0;
    if ( angle <= 180.0 )
        Scr_AddFloat(angle, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddFloat(360.0 - angle, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_AbsAngleClamp360)
void Scr_AbsAngleClamp360()
{
    Scr_AddFloat(AngleNormalize360(Scr_GetFloat(0, SCRIPTINSTANCE_SERVER)), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_AngleLerp)
void Scr_AngleLerp()
{
    float from;
    float to;
    float frac;
    float delta;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 3 )
        Scr_Error("AngleLerp requires 3 parameters", 0);
    from = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    to = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    frac = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    delta = AngleNormalize180(to - from);
    Scr_AddFloat(from + frac * delta, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_AllowMipsToLoad)
void Scr_AllowMipsToLoad()
{
    const char *materialName;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("call AllowMipsToLoad with one parameter: material name (include mc/ or wc/ if applicable)", 0);
    materialName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    SV_GameSendServerCommand(-1, SV_CMD_CAN_IGNORE, va("%c 0 %s", '-', materialName));
}

// Decomp: CoDSP_rdBlackOps.map.c (G_CanSpawnTurret)
void GScr_CanSpawnTurret()
{
    Scr_AddBool(G_CanSpawnTurret(), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_CodeSpawnTurret)
void GScr_CodeSpawnTurret()
{
    float origin[3];
    unsigned int classname;
    gentity_s *ent;
    const char *weaponinfoname;

    classname = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, origin, SCRIPTINSTANCE_SERVER);
    weaponinfoname = Scr_GetString(2u, SCRIPTINSTANCE_SERVER);
    if ( !BG_ValidateOrigin(origin, 18, 17, svs.mapCenter) )
        NetworkOriginError("entity", origin);
    ent = SpawnTurretInternal(classname, origin, weaponinfoname);
    Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_CleanupSpawnedDynEnts)
void GScr_CleanupSpawnedDynEnts()
{
    SV_GameSendServerCommand(-1, SV_CMD_CAN_IGNORE, va("%c", '>'));
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ClearLocalizedStrings)
void GScr_ClearLocalizedStrings()
{
    const char *typeName;
    const char *fmtMsg;
    VariableUnion id;

    if ( Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) != 20 )
    {
        typeName = Scr_GetTypeName(0, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("Parameter (%s) must be an array", typeName);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    id.intValue = Scr_GetObject(0, SCRIPTINSTANCE_SERVER);
    Scr_AddArrayKeys(id.stringValue, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_CrunchArray)
void GScr_CrunchArray()
{
    const char *typeName;
    const char *fmtMsg;
    unsigned int objectId;
    unsigned int index;
    unsigned int siblingId;
    VariableValue indexValue;
    VariableValueInternal *entryValue;
    char packed[16];
    char result[16];
    int val;
    int valIndex;

    if ( Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) != 20 )
    {
        typeName = Scr_GetTypeName(0, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("Parameter (%s) must be an array", typeName);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    objectId = Scr_GetObject(0, SCRIPTINSTANCE_SERVER);
    memset(result, 0, sizeof(result));
    memset(packed, 0, sizeof(packed));
    index = 0;
    siblingId = FindFirstSibling(SCRIPTINSTANCE_SERVER, objectId);
    while ( siblingId )
    {
        entryValue = &gScrVarGlob[SCRIPTINSTANCE_SERVER].variableList[siblingId + 0x8000];
        indexValue = Scr_GetArrayIndexValue(SCRIPTINSTANCE_SERVER, entryValue->w.status >> 8);
        if ( indexValue.type == 2 )
        {
            val = atoi(SL_ConvertToString(indexValue.u.stringValue, SCRIPTINSTANCE_SERVER));
        }
        else if ( indexValue.type == 6 )
        {
            val = indexValue.u.intValue;
        }
        else
        {
            fmtMsg = va(
                "Array passed to CrunchArray contained member [%i] of type %s - valid types are string and int.",
                index,
                var_typename[indexValue.type]);
            Scr_Error(fmtMsg, 0);
        }
        valIndex = val / 6;
        if ( valIndex >= 16 )
            Com_Error(ERR_DROP, "valIndex < 16");
        packed[valIndex] |= (unsigned char)(1 << (val % 6));
        siblingId = FindNextSibling(SCRIPTINSTANCE_SERVER, siblingId);
        ++index;
    }
    for ( index = 0; index < 16; ++index )
    {
        if ( packed[index] )
            result[index] = (char)(packed[index] + 33);
    }
    Scr_AddString(result, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_DestructibleHasNotify)
void GScr_DestructibleHasNotify()
{
    gentity_s *ent;
    unsigned int notifyName;

    ent = Scr_GetEntity(0);
    notifyName = Scr_GetConstString(1u, SCRIPTINSTANCE_SERVER);
    if ( !ent->destructible || !ent->destructible->ddef )
        Scr_Error("DestructibleHasNotify called on an entity that is not a destructible.", 0);
    Scr_AddBool(
        Destructible_HasNotify(ent->destructible->ddef, notifyName),
        SCRIPTINSTANCE_SERVER);
}

int g_drawCompassFriendlies;

// Decomp: CoDSP_rdBlackOps.map.c (GScr_DrawCompassFriendlies)
void GScr_DrawCompassFriendlies()
{
    g_drawCompassFriendlies = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_DrawCurve)
void Scr_DrawCurve()
{
    const int curve = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        cCurveManager::SetCurveDraw(curve, 0);
        return;
    }
    if ( Scr_GetType(1u, SCRIPTINSTANCE_SERVER) != 6 )
    {
        float color[4];

        color[0] = 0.0f;
        color[1] = 0.0f;
        color[2] = 0.0f;
        color[3] = 1.0f;
        Scr_GetVector(1u, color, SCRIPTINSTANCE_SERVER);
        cCurveManager::SetCurveDraw(curve, color);
    }
    else if ( Scr_GetInt(1u, SCRIPTINSTANCE_SERVER) )
    {
        cCurveManager::SetCurveDraw(curve, 0);
    }
    else
    {
        cCurveManager::SetCurveDraw(curve, 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_DumpAnims)
void __cdecl GScr_DumpAnims(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    SV_DObjDisplayAnim(ent, "server:\n");
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_AnimRelative)
static void Scr_AnimRelative(scr_entref_t entref)
{
    gentity_s *entity;
    DObj *obj;
    XAnimTree_s *entAnimTree;
    unsigned int mode;
    scr_anim_s rootAnim;
    scr_anim_s anim;
    actor_s *actor;
    unsigned int notifyName;
    const char *targetName;
    const char *className;
    float animOrigin[3];
    float animAngles[3];

    entity = GetEntity(entref);
    obj = Com_GetServerDObj(entity->s.number);
    if ( !obj )
        Scr_ObjectError("No model exists.", SCRIPTINSTANCE_SERVER);
    entAnimTree = GScr_GetEntAnimTree(entity);
    if ( !entAnimTree )
        Com_Error(ERR_DROP, "CODE ERROR: pAnimTree");
    rootAnim.tree = 0;
    rootAnim.index = 0;
    mode = 0;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 4 )
    {
        mode = Scr_GetConstString(4u, SCRIPTINSTANCE_SERVER);
        if ( mode != scr_const.normal && mode != scr_const.deathplant )
        {
            Scr_Error(
                va(
                    "Illegal mode %s for animScripted. Valid modes are normal and deathplant",
                    SL_ConvertToString(mode, SCRIPTINSTANCE_SERVER)),
                0);
        }
        if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 5 && Scr_GetType(5u, SCRIPTINSTANCE_SERVER) )
            rootAnim = Scr_GetAnim(5u, entAnimTree, SCRIPTINSTANCE_SERVER);
    }
    anim = Scr_GetAnim(3u, entAnimTree, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(2u, animAngles, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, animOrigin, SCRIPTINSTANCE_SERVER);
    notifyName = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    actor = entity->actor;
    if ( actor && !actor->Physics.bIsAlive )
    {
        if ( entity->targetname )
            targetName = SL_ConvertToString(entity->targetname, SCRIPTINSTANCE_SERVER);
        else
            targetName = "<undefined>";
        className = SL_ConvertToString(entity->classname, SCRIPTINSTANCE_SERVER);
        Scr_Error(
            va(
                "tried to play a scripted animation on a dead AI; entity %i team %s origin %g %g %g targetname %s classname %s\n",
                entity->s.number,
                Sentient_NameForTeam(entity->sentient->eTeam),
                entity->r.currentOrigin[0],
                entity->r.currentOrigin[1],
                entity->r.currentOrigin[2],
                targetName,
                className),
            0);
    }
    if ( !rootAnim.tree && rootAnim.index )
        Com_Error(ERR_DROP, "CODE ERROR: root.tree || !root.index");
    G_Animscripted(
        entity,
        animOrigin,
        animAngles,
        anim.index,
        rootAnim.index,
        notifyName,
        mode == scr_const.deathplant);
    if ( actor )
        Actor_PushState(actor, AIS_SCRIPTEDANIM);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_animrelative)
void __cdecl ScrCmd_animrelative(scr_entref_t entref)
{
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 6 )
        Scr_Error("too many parameters", 0);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 4 )
        Scr_Error("Incorrect number of parameters for ScrCmd_animrelative command", 0);
    Scr_AnimRelative(entref);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_BypassSledgehammer)
void __cdecl GScr_BypassSledgehammer(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->client )
        Scr_Error("BypassSledgehammer called on a player.", 0);
    ent->flags |= 0x20000u;
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_DisableClientLinkTo)
void __cdecl ScrCmd_DisableClientLinkTo(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    ent->s.clientLinkInfo.parentEnt |= 0x200;
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_EnableClientLinkTo)
void __cdecl ScrCmd_EnableClientLinkTo(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    ent->s.clientLinkInfo.parentEnt &= 0xFDFF;
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_DisableDepthBuoyancyAdjustments)
void __cdecl ScrCmd_DisableDepthBuoyancyAdjustments(scr_entref_t entref)
{
    gentity_s *ent;
    gentity_s *tempEnt;

    ent = GetEntity(entref);
    tempEnt = G_TempEntity(vec3_origin, EV_DISABLE_DEPTH_BUOYANCY_ADJUSTMENTS);
    AssignToSmallerType<short>(&tempEnt->s.otherEntityNum, ent->s.number);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_DontInterpolate)
void __cdecl ScrCmd_DontInterpolate(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->client )
        *reinterpret_cast<unsigned int *>(reinterpret_cast<char *>(ent->client) + 224) ^= 2u;
    else
        ent->flags ^= 2u;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_ClearPlayerGravity)
void __cdecl Scr_ClearPlayerGravity(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->client )
        ent->client->ps.gravity = 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_RotatePoint)
void Scr_RotatePoint()
{
    float result[3];
    float quat[4];
    float angles[3];
    float point[3];

    Scr_GetVector(0, point, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, angles, SCRIPTINSTANCE_SERVER);
    AnglesToQuat(angles, quat);
    RotatePoint(point, quat, result);
    Scr_AddVector(result, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_IsSubStr)
void Scr_IsSubStr()
{
    char *tempValue0;
    char *tempValue1;
    char *String;

    String = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    tempValue0 = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    tempValue1 = strstr(tempValue0, String);
    Scr_AddBool(tempValue1 != 0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetSubStr)
void Scr_GetSubStr()
{
    VariableUnion varUnion;
    int source;
    char c;
    char tempString[1028];
    int start;
    int end;
    int dest;
    const char *s;

    s = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    start = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 3 )
        varUnion.intValue = 0x7FFFFFFF;
    else
        varUnion.intValue = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
    end = varUnion.intValue;
    source = start;
    for ( dest = 0; source < end; ++dest )
    {
        if ( dest >= 1024 )
            Scr_Error("string too long", 0);
        c = s[source];
        if ( !c )
            break;
        tempString[dest] = c;
        ++source;
    }
    tempString[dest] = 0;
    Scr_AddString(tempString, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_ToLower)
void Scr_ToLower()
{
    char tempValue0;
    char tempString[1028];
    const char *s;
    int i;

    s = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    i = 0;
    while ( i < 1024 )
    {
        tempValue0 = tolower(*s);
        tempString[i] = tempValue0;
        if ( !tempValue0 )
        {
            Scr_AddString(tempString, SCRIPTINSTANCE_SERVER);
            return;
        }
        ++i;
        ++s;
    }
    Scr_Error("string too long", 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_StrTok)
void Scr_StrTok()
{
    int source;
    char c;
    unsigned int delimId;
    char tempString[1028];
    const char *delim;
    int dest;
    const char *s;
    int i;
    int delimLen;
    unsigned int sId;

    sId = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    delimId = Scr_GetConstString(1u, SCRIPTINSTANCE_SERVER);
    s = SL_ConvertToString(sId, SCRIPTINSTANCE_SERVER);
    delim = SL_ConvertToString(delimId, SCRIPTINSTANCE_SERVER);
    SL_AddRefToString(sId, SCRIPTINSTANCE_SERVER);
    SL_AddRefToString(delimId, SCRIPTINSTANCE_SERVER);
    delimLen = strlen(delim);
    dest = 0;
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for ( source = 0; ; ++source )
    {
        bool matchedDelimiter;

        c = s[source];
        if ( !c )
            break;

        matchedDelimiter = false;
        for ( i = 0; i < delimLen; ++i )
        {
            if ( c == delim[i] )
            {
                if ( dest )
                {
                    tempString[dest] = 0;
                    Scr_AddString(tempString, SCRIPTINSTANCE_SERVER);
                    Scr_AddArray(SCRIPTINSTANCE_SERVER);
                    dest = 0;
                }
                matchedDelimiter = true;
                break;
            }
        }
        if ( matchedDelimiter )
            continue;

        tempString[dest++] = c;
        if ( dest >= 1024 )
        {
            SL_RemoveRefToString(SCRIPTINSTANCE_SERVER, sId);
            SL_RemoveRefToString(SCRIPTINSTANCE_SERVER, delimId);
            Scr_Error("string too long", 0);
        }
    }
    if ( dest )
    {
        tempString[dest] = 0;
        Scr_AddString(tempString, SCRIPTINSTANCE_SERVER);
        Scr_AddArray(SCRIPTINSTANCE_SERVER);
        dest = 0;
    }
    SL_RemoveRefToString(SCRIPTINSTANCE_SERVER, sId);
    SL_RemoveRefToString(SCRIPTINSTANCE_SERVER, delimId);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_NeedsRevive)
void __cdecl GScr_NeedsRevive(scr_entref_t entref)
{
    gentity_s *pEnt;
    int needsRevive;

    needsRevive = 0;
    pEnt = GetEntity(entref);
    if ( !pEnt->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 7727, 0, "%s", "pEnt->client") )
    {
        __debugbreak();
    }
    if ( Scr_GetInt(0, SCRIPTINSTANCE_SERVER) )
        needsRevive = 1;
    if ( pEnt->client->ps.clientNum >= 0x20u
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    7736,
                    0,
                    "pEnt->client->ps.clientNum doesn't index MAX_CLIENTS\n\t%i not in [0, %i)",
                    pEnt->client->ps.clientNum,
                    32) )
    {
        __debugbreak();
    }
    bgs->clientinfo[pEnt->client->ps.clientNum].needsRevive = needsRevive;
    G_GetClientState(pEnt->client->ps.clientNum)->needsRevive = needsRevive;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsInSecondChance)
void __cdecl GScr_IsInSecondChance(scr_entref_t entref)
{
    clientState_s *client;
    gentity_s *pEnt;

    pEnt = GetEntity(entref);
    if ( !pEnt->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 7763, 0, "%s", "pEnt->client") )
    {
        __debugbreak();
    }
    if ( pEnt->client->ps.clientNum >= 0x20u
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    7765,
                    0,
                    "pEnt->client->ps.clientNum doesn't index MAX_CLIENTS\n\t%i not in [0, %i)",
                    pEnt->client->ps.clientNum,
                    32) )
    {
        __debugbreak();
    }
    client = G_GetClientState(pEnt->client->ps.clientNum);
    Scr_AddBool(client->needsRevive, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetBurn)
void __cdecl GScr_SetBurn(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *ent;
    int clientNum;
    float burnTime;

    clientNum = -1;
    burnTime = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    if ( burnTime < 0.0 )
        Scr_ParamError(1u, "Time must be positive", SCRIPTINSTANCE_SERVER);
    ent = GetEntity(entref);
    if ( ent && ent->r.inuse && ent->client )
        clientNum = ent->s.number;
    else
        Scr_Error("setburn() called on an invalid client entity.\n", 0);
    fmtMsg = va("%c %i", 87, (int)(float)(burnTime * 1000.0));
    SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetElectrified)
void __cdecl GScr_SetElectrified(scr_entref_t entref)
{
    const char *fmtMsg;
    float effectTime;
    gentity_s *ent;
    int clientNum;

    clientNum = -1;
    effectTime = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    if ( effectTime < 0.0 )
        Scr_ParamError(1u, "Time must be positive", SCRIPTINSTANCE_SERVER);
    ent = GetEntity(entref);
    if ( ent && ent->r.inuse && ent->client )
        clientNum = ent->s.number;
    else
        Scr_Error("setelectrified() called on an invalid client entity.\n", 0);
    fmtMsg = va("%c %i", 40, (int)(float)(effectTime * 1000.0));
    SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetBlur ~82606DF0)
// Map: CoDSP_rd.map VA 0x82606DF0 (g_scr_main.obj)
void __cdecl GScr_SetBlur(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *ent;
    int clientNum;
    float blurValue;
    float blurTime;

    clientNum = -1;
    blurValue = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    blurTime = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    if ( blurTime < 0.0 )
        Scr_ParamError(1u, "Time must be positive", SCRIPTINSTANCE_SERVER);
    if ( blurValue < 0.0 )
        Scr_ParamError(0, "Blur value must be greater than 0", SCRIPTINSTANCE_SERVER);
    ent = GetEntity(entref);
    if ( ent && ent->r.inuse && ent->client )
        clientNum = ent->s.number;
    else
        Scr_Error("setblur() called on an invalid client entity.\n", 0);
    fmtMsg = va("A %i %f %i %i", (int)(float)(blurTime * 1000.0), blurValue, 1, 0);
    SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_StartFadingBlur ~82606F48)
void __cdecl GScr_StartFadingBlur(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *ent;
    int clientNum;
    float blurValue;
    float blurTime;

    clientNum = -1;
    blurValue = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    blurTime = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    if ( blurTime < 0.0 )
        Scr_ParamError(1u, "Time must be positive", SCRIPTINSTANCE_SERVER);
    if ( blurValue <= 0.0 )
        Scr_ParamError(0, "Blur value must be greater than 0", SCRIPTINSTANCE_SERVER);
    ent = GetEntity(entref);
    if ( ent && ent->r.inuse && ent->client )
        clientNum = ent->s.number;
    else
        Scr_Error("setblur() called on an invalid client entity.\n", 0);
    fmtMsg = va("%c %i %f", '|', (int)(float)(blurTime * 1000.0), blurValue);
    SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_StartTanning)
void __cdecl GScr_StartTanning(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->client )
        ent->client->ps.eFlags2 |= 0x200000u;
    else
        ent->s.lerp.eFlags2 |= 0x200000u;
}

// LWSS ADD
void __cdecl GScr_SetWaterDrops(scr_entref_t entref)
{
    gentity_s *pSelf;
    int count;
    const char *cmd;

    count = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if (count < 0)
        Scr_ParamError(1, "Count must be either zero, or a positive number", SCRIPTINSTANCE_SERVER);

    pSelf = GetEntity(entref);

    if (!pSelf->r.linked || !pSelf->client)
    {
        Scr_Error("setwaterdrops() called on an invalid client entity.\n", SCRIPTINSTANCE_SERVER);
        return;
    }

    cmd = va("%c %i", '0', count);
    SV_GameSendServerCommand(pSelf->s.number, SV_CMD_RELIABLE, cmd);
}

// LWSS END
// Decomp: CoDSP_rdBlackOps.map.c (GScr_StopBurning)
void __cdecl GScr_StopBurning(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->client )
        ent->client->ps.eFlags2 &= ~0x200000u;
    else
        ent->s.lerp.eFlags2 &= ~0x200000u;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SpawnNapalmGroundFlame @ 825F76C8)
void GScr_SpawnNapalmGroundFlame()
{
    char *weaponName;
    float origin[3];
    int time;
    int weaponIndex;
    gentity_s *ent;
    float direction[3];

    time = 10;
    Scr_GetVector(0, origin, SCRIPTINSTANCE_SERVER);
    weaponName = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(2u, direction, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 3 )
        time = Scr_GetInt(3u, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    Scr_VerifyWeaponIndex(weaponIndex, weaponName);
    BG_GetWeaponDef(weaponIndex);
    ent = G_Spawn();
    G_SetOrigin(ent, origin);
    G_SetAngle(ent, direction);
    ent->s.weapon = weaponIndex;
    ent->s.eType = ET_GENERAL;
    ent->s.lerp.eFlags |= 0x20u;
    G_BroadcastEntity(ent);
    G_AddEvent(ent, 0x43u, 0);
    ent->s.lerp.pos.trBase[0] = (float)(int)ent->s.lerp.pos.trBase[0];
    ent->s.lerp.pos.trBase[1] = (float)(int)ent->s.lerp.pos.trBase[1];
    ent->s.lerp.pos.trBase[2] = (float)(int)ent->s.lerp.pos.trBase[2];
    G_SetOrigin(ent, ent->s.lerp.pos.trBase);
    G_SetAngle(ent, ent->s.lerp.apos.trBase);
    ent->s.lerp.eFlags |= 0x4000u;
    ent->s.lerp.u.actor.actorNum = level.time;
    ent->s.time2 = level.time + 1000 * time;
    ent->s.lerp.eFlags |= 0x10u;
    ent->s.lerp.eFlags2 |= 1u;
    ent->handler = 15;
    ent->nextthink = level.time + 1;
    SV_LinkEntity(ent);
}

// Decomp: CoDSPBlackOps.map.c (GScr_SpawnNapalmGroundFlame method)
void __cdecl GScr_SpawnNapalmGroundFlame_method(scr_entref_t entref)
{
    char *weaponName;
    float origin[3];
    int time;
    int weaponIndex;
    gentity_s *ent;
    float direction[3];
    const WeaponDef *weapDef;

    ent = GetEntity(entref);
    time = 10;
    Scr_GetVector(0, origin, SCRIPTINSTANCE_SERVER);
    weaponName = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(2u, direction, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 3 )
        time = Scr_GetInt(3u, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    Scr_VerifyWeaponIndex(weaponIndex, weaponName);
    weapDef = BG_GetWeaponDef(weaponIndex);
    G_SetOrigin(ent, origin);
    G_SetAngle(ent, direction);
    ent->s.weapon = weaponIndex;
    ent->s.eType = ET_GENERAL;
    ent->s.lerp.eFlags |= 0x20u;
    ent->s.weapon = ent->s.weapon;
    G_BroadcastEntity(ent);
    G_AddEvent(ent, 0x42u, 0);
    ent->s.lerp.pos.trBase[0] = (float)(int)ent->s.lerp.pos.trBase[0];
    ent->s.lerp.pos.trBase[1] = (float)(int)ent->s.lerp.pos.trBase[1];
    ent->s.lerp.pos.trBase[2] = (float)(int)ent->s.lerp.pos.trBase[2];
    G_SetOrigin(ent, ent->s.lerp.pos.trBase);
    G_SetAngle(ent, ent->s.lerp.apos.trBase);
    ent->s.lerp.eFlags |= 0x4000u;
    ent->s.lerp.u.actor.actorNum = level.time;
    ent->s.time2 = level.time + 1000 * time;
    ent->s.lerp.eFlags |= 0x10u;
    ent->s.lerp.eFlags2 |= 1u;
    ent->handler = 11;
    ent->nextthink = level.time + 1;
    SV_LinkEntity(ent);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_RestoreDefaultDropPitch)
void __cdecl GScr_RestoreDefaultDropPitch(scr_entref_t entref)
{
    gentity_s *ent;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        Scr_Error("illegal call to restoredefaultdroppitch()\n", 0);
    }
    else
    {
        ent = GetEntity(entref);
        if ( ent->pTurretInfo )
            turret_RestoreDefaultDropPitch(ent);
        else
            Scr_Error("entity is not a turret", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_clearCenterPopups)
void __cdecl GScr_clearCenterPopups(scr_entref_t entref)
{
    const char *fmtMsg;
    int clientNum;

    clientNum = GetEntity(entref)->s.number;
    fmtMsg = va("%c %c", 91, 110);
    SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_clearPopups)
void __cdecl GScr_clearPopups(scr_entref_t entref)
{
    const char *fmtMsg;
    int clientNum;

    clientNum = GetEntity(entref)->s.number;
    fmtMsg = va("%c %c", 91, 105);
    SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_DisplayGameModeMessage)
void __cdecl GScr_DisplayGameModeMessage(scr_entref_t entref)
{
    const char *fmtMsg;
    int sentToClientNum;
    char *sound;
    char gameModeMessage[132];

    sound = (char *)"";
    sentToClientNum = GetEntity(entref)->s.number;
    Scr_ConstructMessageString(0, 0, "Game Message", gameModeMessage, 0x80u);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 2 )
        sound = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    fmtMsg = va("%c %c %s %s", 91, 99, gameModeMessage, sound);
    SV_GameSendServerCommand(sentToClientNum, SV_CMD_RELIABLE, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_DisplayTeamMessage)
void __cdecl GScr_DisplayTeamMessage(scr_entref_t entref)
{
    const char *fmtMsg;
    int configStringIndex;
    char message[132];
    gentity_s *playerEnt;
    const char *sound;
    gentity_s *sendToEntity;
    int clientNum;

    sendToEntity = GetEntity(entref);
    playerEnt = Scr_GetEntity(1u);
    sound = "";
    Scr_ConstructMessageString(0, 0, "Team Message String", message, 0x80u);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 3 )
        sound = Scr_GetString(2u, SCRIPTINSTANCE_SERVER);
    configStringIndex = G_LocalizedStringIndex(message);
    clientNum = sendToEntity->s.number;
    fmtMsg = va("%c %c %i %i %s", 91, 104, configStringIndex, playerEnt->client->ps.clientNum, sound);
    SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_DisplayMedal)
void __cdecl GScr_DisplayMedal(scr_entref_t entref)
{
    const char *fmtMsg;
    char *sound;
    float xpScale;
    gentity_s *player_entity;
    int teamBased;
    int clientNum;
    int medalIndex;

    player_entity = GetEntity(entref);
    medalIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    teamBased = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    xpScale = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    sound = Scr_GetString(3u, SCRIPTINSTANCE_SERVER);
    clientNum = player_entity->s.number;
    fmtMsg = va("%c %c %i %i %.2f %s", 91, 102, medalIndex, teamBased, xpScale, sound);
    SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_DisplayContract)
void __cdecl GScr_DisplayContract(scr_entref_t entref)
{
    const char *fmtMsg;
    int contractIndex;
    int passed;
    char *sound;
    gentity_s *player_entity;
    int clientNum;

    player_entity = GetEntity(entref);
    contractIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    sound = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    passed = 0;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 3 )
        passed = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
    clientNum = player_entity->s.number;
    fmtMsg = va("%c %c %i %s %i", 91, 98, contractIndex, sound, passed);
    SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_DisplayChallengeComplete)
void __cdecl GScr_DisplayChallengeComplete(scr_entref_t entref)
{
    const char *fmtMsg;
    int tier;
    char *sound;
    float xpScale;
    int index;
    int weaponIndex;
    char *type;
    int clientNum;

    clientNum = GetEntity(entref)->s.number;
    tier = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    index = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    xpScale = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    weaponIndex = Scr_GetInt(3u, SCRIPTINSTANCE_SERVER);
    sound = Scr_GetString(4u, SCRIPTINSTANCE_SERVER);
    type = Scr_GetString(5u, SCRIPTINSTANCE_SERVER);
    fmtMsg = va("%c %c %i %i %.2f %i %s %s", 91, 100, tier, index, xpScale, weaponIndex, type, sound);
    SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_DisplayEndGameMilestoneComplete)
void __cdecl GScr_DisplayEndGameMilestoneComplete(scr_entref_t entref)
{
    const char *fmtMsg;
    int tier;
    int slot;
    int index;
    int weaponIndex;
    char *type;
    int clientNum;

    clientNum = GetEntity(entref)->s.number;
    Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
    Scr_GetInt(3u, SCRIPTINSTANCE_SERVER);
    Scr_GetString(4u, SCRIPTINSTANCE_SERVER);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 5
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    8125,
                    0,
                    "%s",
                    "Scr_GetNumParam() == 5") )
    {
        __debugbreak();
    }
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 5 )
    {
        slot = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        tier = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
        index = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
        weaponIndex = Scr_GetInt(3u, SCRIPTINSTANCE_SERVER);
        type = Scr_GetString(4u, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("%c %c %i %i %i %i %s", 91, 108, slot, tier, index, weaponIndex, type);
        SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, fmtMsg);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_DisplayEndGame)
void __cdecl GScr_DisplayEndGame(scr_entref_t entref)
{
    const char *fmtMsg;
    int challengeIndex2;
    int challengeIndex0;
    int promoted;
    int challengeIndex1;
    int clientNum;

    clientNum = GetEntity(entref)->s.number;
    promoted = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    challengeIndex0 = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    challengeIndex1 = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
    challengeIndex2 = Scr_GetInt(3u, SCRIPTINSTANCE_SERVER);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 4
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    8160,
                    0,
                    "%s",
                    "Scr_GetNumParam() == 4") )
    {
        __debugbreak();
    }
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 4 )
    {
        fmtMsg = va("%c %c %i %i %i %i", 91, 109, promoted, challengeIndex0, challengeIndex1, challengeIndex2);
        SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, fmtMsg);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ClearEndGameComplete)
void __cdecl GScr_ClearEndGameComplete(scr_entref_t entref)
{
    const char *fmtMsg;
    int clientNum;

    clientNum = GetEntity(entref)->s.number;
    fmtMsg = va("%c %c", 91, 107);
    SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_DisplayKillstreak)
void __cdecl GScr_DisplayKillstreak(scr_entref_t entref)
{
    const char *fmtMsg;
    int streakCount;
    int killstreakTableNumber;
    int clientNum;
    gentity_s *player_entity;

    player_entity = GetEntity(entref);
    if ( player_entity->s.eType == 1 )
    {
        streakCount = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        killstreakTableNumber = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
        clientNum = player_entity->s.number;
        fmtMsg = va("%c %c %i %i", 91, 101, streakCount, killstreakTableNumber);
        SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, fmtMsg);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_DisplayRankUp)
void __cdecl GScr_DisplayRankUp(scr_entref_t entref)
{
    const char *fmtMsg;
    int prestige;
    int rank;
    char *sound;
    int clientNum;
    gentity_s *player_entity;

    player_entity = GetEntity(entref);
    if ( player_entity->s.eType == 1 )
    {
        rank = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        prestige = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
        if ( prestige < 0 )
            prestige = 0;
        sound = Scr_GetString(2u, SCRIPTINSTANCE_SERVER);
        clientNum = player_entity->s.number;
        fmtMsg = va("%c %c %i %i %s", 91, 103, rank, prestige, sound);
        SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, fmtMsg);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_DisplayWagerPopup)
void __cdecl GScr_DisplayWagerPopup(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    char *subMessageString;
    int subMessageStringIndex;
    int messageStringIndex;
    int points;
    char *messageString;
    int clientNum;
    gentity_s *player_entity;

    player_entity = GetEntity(entref);
    if ( player_entity->s.eType == 1 )
    {
        messageString = Scr_GetIString(0, SCRIPTINSTANCE_SERVER);
        messageStringIndex = G_FindConfigstringIndex(messageString, 515, 1023, 0, "WAGER POPUP ERROR:");
        if ( messageStringIndex )
        {
            points = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
            clientNum = player_entity->s.number;
            if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 3 )
            {
                subMessageString = Scr_GetIString(2u, SCRIPTINSTANCE_SERVER);
                subMessageStringIndex = G_FindConfigstringIndex(subMessageString, 515, 1023, 0, "WAGER POPUP ERROR:");
                if ( subMessageStringIndex )
                {
                    fmtMsg = va("%c %c %i %i %i", 91, 106, messageStringIndex, points, subMessageStringIndex);
                    SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, fmtMsg);
                }
                else
                {
                    Scr_Error("Error displaying wager popup: sub message not precached", 0);
                }
            }
            else
            {
                fmtMsg2 = va("%c %c %i %i", 91, 106, messageStringIndex, points);
                SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, fmtMsg2);
            }
        }
        else
        {
            Scr_Error("Error displaying wager popup: message not precached", 0);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_DisplayHudAnim)
void __cdecl GScr_DisplayHudAnim(scr_entref_t entref)
{
    const char *fmtMsg;
    char *hudAnimName;
    char *hudMenuName;
    int clientNum;
    gentity_s *player_entity;

    player_entity = GetEntity(entref);
    if ( player_entity->s.eType == 1 )
    {
        clientNum = player_entity->client->ps.clientNum;
        hudAnimName = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
        hudMenuName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("%c %c %s %s", 91, 97, hudMenuName, hudAnimName);
        SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, fmtMsg);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsFiringTurret)
void __cdecl GScr_IsFiringTurret(scr_entref_t entref)
{
    char *slStr;
    const char *fmtMsg;
    bool IsFiring;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->pTurretInfo )
    {
        slStr = SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("entity type '%s' is not a turret", slStr);
        Scr_Error(fmtMsg, 0);
    }
    IsFiring = turret_IsFiring(ent);
    Scr_AddBool(IsFiring, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsTurretLockedOn)
void __cdecl GScr_IsTurretLockedOn(scr_entref_t entref)
{
    char *slStr;
    const char *fmtMsg;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->pTurretInfo )
    {
        slStr = SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("entity type '%s' is not a turret", slStr);
        Scr_Error(fmtMsg, 0);
    }
    if ( ent->pTurretInfo->state == 1 )
        Scr_AddBool(1u, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddBool(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ShootUp)
void __cdecl GScr_ShootUp(scr_entref_t entref)
{
    float velocity_8;
    trajectory_t *trajectory;
    gentity_s *pEnt;

    pEnt = GetEntity(entref);
    velocity_8 = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    trajectory = &pEnt->s.lerp.pos;
    pEnt->s.lerp.pos.trTime = level.time;
    pEnt->s.lerp.pos.trDuration = (int)(float)(1000.0 * 1000.0);
    pEnt->s.lerp.pos.trBase[0] = pEnt->r.currentOrigin[0];
    pEnt->s.lerp.pos.trBase[1] = pEnt->r.currentOrigin[1];
    pEnt->s.lerp.pos.trBase[2] = pEnt->r.currentOrigin[2];
    pEnt->s.lerp.pos.trDelta[0] = 0.0f;
    pEnt->s.lerp.pos.trDelta[1] = 0.0f;
    pEnt->s.lerp.pos.trDelta[2] = velocity_8;
    if ( ((LODWORD(pEnt->s.lerp.pos.trDelta[0]) & 0x7F800000) == 0x7F800000
         || (LODWORD(pEnt->s.lerp.pos.trDelta[1]) & 0x7F800000) == 0x7F800000
         || (LODWORD(pEnt->s.lerp.pos.trDelta[2]) & 0x7F800000) == 0x7F800000)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    8395,
                    0,
                    "%s",
                    "!IS_NAN((trajectory->trDelta)[0]) && !IS_NAN((trajectory->trDelta)[1]) && !IS_NAN((trajectory->trDelta)[2])") )
    {
        __debugbreak();
    }
    trajectory->trType = 6;
    BG_EvaluateTrajectory(trajectory, level.time, pEnt->r.currentOrigin);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetWaterHeight)
void __cdecl GScr_GetWaterHeight()
{
    float pos[3];
    float height;

    Scr_GetVector(0, pos, SCRIPTINSTANCE_SERVER);
    height = CM_GetWaterHeight(pos, 200.0, -200.0);
    Scr_AddFloat(height, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_DepthInWater)
void __cdecl GScr_DepthInWater(scr_entref_t entref)
{
    float tempValue1;
    float waterHeight;
    gentity_s *ent;

    ent = GetEntity(entref);
    waterHeight = CM_GetWaterHeight(ent->r.currentOrigin, 200.0, -200.0) - ent->r.currentOrigin[2];
    if ( (float)(waterHeight - 0.0) < 0.0 )
        tempValue1 = 0.0f;
    else
        tempValue1 = waterHeight;
    Scr_AddFloat(tempValue1, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_DepthOfPlayerInWater)
void __cdecl GScr_DepthOfPlayerInWater(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 8459, 0, "%s", "ent->client") )
    {
        __debugbreak();
    }
    Scr_AddInt(ent->client->ps.waterlevel, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetCurve)
void Scr_GetCurve()
{
    Scr_AddInt(cCurveManager::GetFreeCurve(), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_BuildCurve)
void Scr_BuildCurve()
{
    const int curve = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    cCurveManager::BuildCurve(curve);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_StartCurve)
void Scr_StartCurve()
{
    const int curve = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    cCurveManager::StartCurve(curve);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_StopCurve @ 82603D30)
void Scr_StopCurve()
{
    const int curve = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    cCurveManager::StopCurve(curve);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_FreeCurve @ 82603EF8)
void Scr_FreeCurve()
{
    const int curve = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    cCurveManager::FreeCurve(curve);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetCurveCurPos @ 82603BC0)
void Scr_GetCurveCurPos()
{
    float pos[3];
    const int curve = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);

    cCurveManager::GetCurClientPos(curve, pos);
    Scr_AddVector(pos, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetCurveServerPos @ 82603C18)
void Scr_GetCurveServerPos()
{
    float pos[3];
    const int curve = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);

    cCurveManager::GetCurServerPos(curve, pos);
    Scr_AddVector(pos, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetAnimFrameCount @ 825F6470)
void GScr_GetAnimFrameCount()
{
    scr_anim_s anim;
    XAnim_s *anims;

    anim = Scr_GetAnim(0, 0, SCRIPTINSTANCE_SERVER);
    anims = Scr_GetAnims(anim.tree, SCRIPTINSTANCE_SERVER);
    if ( !XAnimIsPrimitive(anims, anim.index) )
        Scr_ParamError(0, "non-primitive animation has no concept of length", SCRIPTINSTANCE_SERVER);
    Scr_AddInt(XAnimGetFrameCount(anims, anim.index), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetAnimDumpModel @ 825F60C8) — stub until AD_* anim dump module is ported
void GScr_GetAnimDumpModel()
{
    Scr_AddString("", SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetAnimDumpTree @ 825F60A0) — stub until AD_* anim dump module is ported
void GScr_GetAnimDumpTree()
{
    Scr_AddString("", SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_AddNodeToCurve)
void Scr_AddNodeToCurve()
{
    float pos[3];
    const int curve = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);

    Scr_GetVector(1u, pos, SCRIPTINSTANCE_SERVER);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 2 )
    {
        cCurveManager::AddNodeToCurve(curve, pos);
    }
    else
    {
        const float timePeriod = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
        cCurveManager::AddNodeToCurve(curve, pos, timePeriod);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GetRopeIndex)
int __cdecl GetRopeIndex(unsigned int targetname)
{
    int ropeIndex = 0;

    for ( int i = 0; i < cm.num_constraints; ++i )
    {
        if ( cm.constraints[i].type == CONSTRAINT_ROPE )
        {
            if ( cm.constraints[i].targetname == targetname )
                return ropeIndex;
            ++ropeIndex;
        }
    }
    return -1;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_CreateRope)
void Scr_CreateRope()
{
    static const int ROPE_NO_ENTITY = 959;

    float start[3];
    float end[3];
    float length;
    int numParam;
    int ropeId;
    int ent1Num;
    int tag1;
    int health;
    gentity_s *ent1;
    int i;

    numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParam < 3 )
        Scr_Error("Incorrect number of parameters", 0);

    Scr_GetVector(0, start, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, end, SCRIPTINSTANCE_SERVER);
    length = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    if ( length < 0.0f )
        Scr_ParamError(2u, "length is negative", SCRIPTINSTANCE_SERVER);

    if ( g_totalRopeCount >= 300 )
    {
        Scr_AddInt(g_totalRopeCount, SCRIPTINSTANCE_SERVER);
        return;
    }

    ropeId = g_totalRopeCount;
    ent1Num = ROPE_NO_ENTITY;
    tag1 = 0;
    health = 0;
    ent1 = 0;

    if ( numParam > 3 )
    {
        ent1 = Scr_GetEntity(3u);
        ent1Num = ent1->s.number;
        if ( numParam > 4 )
            tag1 = Scr_GetConstLowercaseString(4u, SCRIPTINSTANCE_SERVER);

        for ( i = 0; i < g_entsWithRopesCount; ++i )
        {
            if ( g_entsWithRopes[i] == ent1Num )
                break;
        }
        if ( i == g_entsWithRopesCount )
        {
            if ( g_entsWithRopesCount >= 64 )
            {
                Com_Printf(1, "Too many entity rope to entity anchors.\n");
            }
            else
            {
                g_entsWithRopes[g_entsWithRopesCount++] = ent1Num;
            }
        }
    }

    if ( numParam > 5 )
        health = Scr_GetInt(5u, SCRIPTINSTANCE_SERVER);

    if ( numParam > 6 )
    {
        gentity_s *ent2 = Scr_GetEntity(6u);
        (void)ent2;
        if ( numParam > 7 )
            (void)Scr_GetConstLowercaseString(7u, SCRIPTINSTANCE_SERVER);
    }

    SV_GameSendServerCommand(
        -1,
        SV_CMD_RELIABLE,
        va(
            "# %d %d %d %1.1f %1.1f %1.1f %1.1f %1.1f %1.1f %d %d %d",
            (int)'N',
            ropeId,
            (int)length,
            start[0],
            start[1],
            start[2],
            end[0],
            end[1],
            end[2],
            ent1Num,
            tag1,
            health));

    Scr_AddInt(g_totalRopeCount, SCRIPTINSTANCE_SERVER);
    ++g_totalRopeCount;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetRope)
void Scr_GetRope()
{
    unsigned int targetname;
    int ropeIndex;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("Incorrect number of parameters.", 0);

    targetname = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    ropeIndex = GetRopeIndex(targetname);
    Scr_AddInt(ropeIndex, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetCurveBSpline)
void Scr_SetCurveBSpline()
{
    const int curve = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    cCurveManager::SetCurveBSpline(curve);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetCurveRounded)
void Scr_SetCurveRounded()
{
    const int curve = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    cCurveManager::SetCurveRounded(curve);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetCurveSmooth)
void Scr_SetCurveSmooth()
{
    const int curve = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    cCurveManager::SetCurveSmooth(curve);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetCurveSpeed)
void Scr_SetCurveSpeed()
{
    const int curve = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    const float speed = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    cCurveManager::SetCurveSpeed(curve, speed);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetCurveNotifyEnt)
void Scr_SetCurveNotifyEnt()
{
    const int curve = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);

    if ( Scr_GetPointerType(1u, SCRIPTINSTANCE_SERVER) == 19 )
    {
        gentity_s *ent = Scr_GetEntity(1u);
        cCurveManager::SetNotifyEnt(curve, ent->s.number);
    }
    else if ( Scr_GetPointerType(1u, SCRIPTINSTANCE_SERVER) == 17 )
    {
        cCurveManager::SetNotifyLevel(curve);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetCurveCameraEnt)
void Scr_SetCurveCameraEnt()
{
    const int curve = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    gentity_s *ent = Scr_GetEntity(1u);
    cCurveManager::SetCameraEnt(curve, ent->s.number);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_SetGravity)
void ScrCmd_SetGravity()
{
    Dvar_SetFloat((dvar_s *)bg_gravity, Scr_GetFloat(0, SCRIPTINSTANCE_SERVER));
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetDebugOrigin)
void GScr_SetDebugOrigin()
{
    float origin[3];
    Scr_GetVector(0, origin, SCRIPTINSTANCE_SERVER);
    CG_SetDebugOrigin(origin);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetDebugAngles)
void GScr_SetDebugAngles()
{
    float angles[3];
    Scr_GetVector(0, angles, SCRIPTINSTANCE_SERVER);
    CG_SetDebugAngles(angles);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetCellInvisibleAtPos)
void Scr_SetCellInvisibleAtPos()
{
    float pos[3];
    int cellIndex;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 || Scr_GetType(0, SCRIPTINSTANCE_SERVER) != 4 )
        Scr_Error("invalid call to SetCellInvisibleAtPos. Check doc for correct usage!", 0);

    Scr_GetVector(0, pos, SCRIPTINSTANCE_SERVER);
    cellIndex = R_CellForPoint(pos);
    if ( !R_ForcedInvisibleCell_TurnOn(cellIndex) )
        Scr_Error(va("SetCellInvisibleAtPos failed. Pos not in valid cell (%.2f,%.2f,%.2f)\n", pos[0], pos[1], pos[2]), 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetCellVisibleAtPos)
void Scr_SetCellVisibleAtPos()
{
    float pos[3];
    int cellIndex;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 || Scr_GetType(0, SCRIPTINSTANCE_SERVER) != 4 )
        Scr_Error("invalid call to SetCellVisibleAtPos. Check doc for correct usage!", 0);

    Scr_GetVector(0, pos, SCRIPTINSTANCE_SERVER);
    cellIndex = R_CellForPoint(pos);
    if ( !R_ForcedInvisibleCell_TurnOff(cellIndex) )
        Scr_Error(va("SetCellVisibleAtPos failed. Pos not in valid cell (%.2f,%.2f,%.2f)\n", pos[0], pos[1], pos[2]), 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetAnimForceNew)
void __cdecl GScr_SetAnimForceNew(scr_entref_t entref)
{
    unsigned int notifyFlags;
    unsigned int numParams;
    float rate;
    XAnimTree_s *tree;
    DObj *obj;
    int cmdIndex;
    float goalWeight;
    float goalTime;
    int error;
    scr_anim_s anim;
    gentity_s *ent;

    ent = GetEntity(entref);
    tree = GScr_GetEntAnimTree(ent);
    rate = 1.0f;
    goalTime = 0.2f;
    goalWeight = 1.0f;
    numParams = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParams )
    {
        anim = Scr_GetAnim(0, tree, SCRIPTINSTANCE_SERVER);
        XAnimGetParamValue(tree, anim.index, "rate", &rate);
        XAnimGetParamValue(tree, anim.index, "goaltime", &goalTime);
        XAnimGetParamValue(tree, anim.index, "goalweight", &goalWeight);
    }
    if ( numParams > 4 )
        Scr_Error("too many parameters", 0);
    if ( numParams < 1 )
        Scr_Error("too many parameters", 0);
    if ( numParams >= 5 )
    {
        rate = GScr_GetOptionalFloat(4u, rate);
        if ( rate < 0.0f )
            Scr_ParamError(4u, "must set nonnegative rate", SCRIPTINSTANCE_SERVER);
    }
    if ( numParams >= 4 )
    {
        goalTime = GScr_GetOptionalFloat(3u, goalTime);
        if ( goalTime < 0.0f )
            Scr_ParamError(3u, "must set nonnegative goal time", SCRIPTINSTANCE_SERVER);
    }
    if ( numParams >= 3 )
    {
        const float transitionTime = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
        if ( transitionTime < 0.0f || transitionTime > 1.0f )
            Scr_ParamError(2u, "must set a time between 0 and 1", SCRIPTINSTANCE_SERVER);
        goalTime = transitionTime;
    }
    if ( numParams >= 2 )
    {
        goalWeight = GScr_GetOptionalFloat(1u, goalWeight);
        if ( goalWeight < 0.0f || goalWeight > 1.0f )
            Scr_ParamError(1u, "must set nonnegative weight", SCRIPTINSTANCE_SERVER);
    }
    anim = Scr_GetAnim(0, tree, SCRIPTINSTANCE_SERVER);
    obj = Com_GetServerDObj(ent->s.number);
    if ( !obj )
        Scr_ObjectError("No model exists.", SCRIPTINSTANCE_SERVER);
    cmdIndex = 0;
    if ( goalWeight <= 0.001f )
        notifyFlags = 0;
    else
        notifyFlags = 2;
    error = XAnimSetCompleteGoalWeight(
                        obj,
                        anim.index,
                        goalWeight,
                        goalTime,
                        rate,
                        0,
                        notifyFlags,
                        1,
                        cmdIndex);
    if ( error )
        GScr_HandleAnimError(error);
    else
        G_FlagAnimForUpdate(ent);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_SetCellInvisible)
void __cdecl ScrCmd_SetCellInvisible(scr_entref_t entref)
{
    gentity_s *ent;
    int cellIndex;

    if ( !zombiemode->current.enabled )
        Scr_Error("Invalid call to SetCellInvisible()\n", 0);

    ent = GetEntity(entref);
    iassert(ent);
    cellIndex = R_CellForPoint(ent->r.currentOrigin);
    if ( !R_ForcedInvisibleCell_TurnOn(cellIndex) )
        Scr_Error(va("SetCellInvisible failed. ent not in valid cell (%.2f,%.2f,%.2f)\n",
                     ent->r.currentOrigin[0], ent->r.currentOrigin[1], ent->r.currentOrigin[2]), 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_SetCellVisible)
void __cdecl ScrCmd_SetCellVisible(scr_entref_t entref)
{
    gentity_s *ent;
    int cellIndex;

    if ( !zombiemode->current.enabled )
        Scr_Error("Invalid call to SetCellVisible()\n", 0);

    ent = GetEntity(entref);
    iassert(ent);
    cellIndex = R_CellForPoint(ent->r.currentOrigin);
    if ( !R_ForcedInvisibleCell_TurnOff(cellIndex) )
        Scr_Error(va("SetCellVisible failed. Pos not in valid cell (%.2f,%.2f,%.2f)\n",
                     ent->r.currentOrigin[0], ent->r.currentOrigin[1], ent->r.currentOrigin[2]), 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetClientFlagAsVal)
void __cdecl GScr_SetClientFlagAsVal(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *ent;
    int value;

    ent = GetEntity(entref);
    value = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( value >= 0 && value < 0x10000 )
    {
        if ( ent->client )
            ent->client->ps.eFlags2 = (ent->client->ps.eFlags2 & 0xFFFF0000) | value;
        else
            ent->s.lerp.eFlags2 = (ent->s.lerp.eFlags2 & 0xFFFF0000) | value;
    }
    else
    {
        fmtMsg = va("SetClientFlagAsVal: Index %i out of range (0 - %i)\n", value, 15);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetDoubleVision)
void __cdecl GScr_SetDoubleVision(scr_entref_t entref)
{
    const char *cmd;
    float intensity;
    float duration;
    gentity_s *ent;
    int clientNum;

    intensity = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    duration = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    if ( duration < 0.0f )
        Scr_ParamError(1u, "Time must be positive", SCRIPTINSTANCE_SERVER);
    if ( intensity < 0.0f )
        Scr_ParamError(0, "Double vision value must be greater than 0", SCRIPTINSTANCE_SERVER);

    ent = GetEntity(entref);
    if ( !ent || !ent->r.linked || !ent->client )
        Scr_Error("setdoublevision() called on an invalid client entity.\n", 0);

    clientNum = ent->s.number;
    cmd = va("%c %i %f", '4', (int)(duration * 1000.0f), intensity);
    SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, cmd);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetFriendlyChain)
void __cdecl GScr_SetFriendlyChain(scr_entref_t entref)
{
    pathnode_t *node;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_Error("Entity must be a player", 0);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("sentient SetFriendlyChain <node>, where <none> is on a friendly chain.\n", 0);

    node = Scr_GetPathnode(0, SCRIPTINSTANCE_SERVER);
    if ( !ent->sentient )
        Scr_Error("Entity must be a player", 0);
    if ( node->constant.type < 1 )
        Path_ConvertNodeToIndex(node);
    ent->sentient->pActualChainPos = node;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetHudWarningType)
void __cdecl GScr_SetHudWarningType(scr_entref_t entref)
{
    const char *typeName;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->actor )
    {
        Scr_Error(va("sethudwarningtype must be called on an AI, not on a '%s'",
                     SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER)), 0);
    }

    typeName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( !I_stricmp(typeName, "banzai") )
        ent->actor->hudWarningType = 1;
    else if ( !I_stricmp(typeName, "banzai_grenadesuicide") )
        ent->actor->hudWarningType = 2;
    else if ( !I_stricmp(typeName, "zombie_friend") )
        ent->actor->hudWarningType = 3;
    else
        ent->actor->hudWarningType = 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetLocalWindSource)
void __cdecl GScr_SetLocalWindSource(scr_entref_t entref)
{
    gentity_s *ent;
    int enabled;

    ent = GetEntity(entref);
    enabled = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    G_AddEvent(ent, EV_SETLOCALWIND, enabled);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetEntGravityTrajectory)
void __cdecl Scr_SetEntGravityTrajectory(scr_entref_t entref)
{
    gentity_s *ent;
    int enabled;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("Bad number of paramaters\n", 0);

    ent = GetEntity(entref);
    if ( ent->s.eType == ET_SCRIPTMOVER || ent->s.eType == ET_EVENTS )
    {
        enabled = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        ent->s.eType = enabled ? ET_EVENTS : ET_SCRIPTMOVER;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_DeleteRope)
void Scr_DeleteRope()
{
    int ropeIndex;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("Incorrect number of parameters.", 0);

    ropeIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( ropeIndex < 0 )
        Scr_ParamError(0, "Bad rope index", SCRIPTINSTANCE_SERVER);

    SV_GameSendServerCommand(-1, SV_CMD_RELIABLE, va("# %d %d", (int)'Q', ropeIndex));
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_PlayerAnimScriptEvent ~826160C8)
void ScrCmd_PlayerAnimScriptEvent(scr_entref_t entref)
{
    gentity_s *ent;
    const char *eventName;

    ent = GetEntity(entref);
    if ( !ent->client )
    {
        Scr_Error("PlayerAnimScriptEvent is only defined for players.", SCRIPTINSTANCE_SERVER);
        return;
    }
    ent->client->ps.pm_flags |= 0x100000u;
    eventName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    ent->client->ps.scriptedAnim = BG_StringHashValue(eventName);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_PlayerAnimScriptEventEnd ~82616180)
void ScrCmd_PlayerAnimScriptEventEnd(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->client )
    {
        Scr_Error("PlayerAnimScriptEventEnd is only defined for players.", SCRIPTINSTANCE_SERVER);
        return;
    }
    ent->client->ps.pm_flags &= ~0x100000u;
    ent->client->ps.scriptedAnim = 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PlayerPositionValid ~82604E58)
void Scr_PlayerPositionValid()
{
    trace_t trace;
    col_context_t context;
    float pos[3];

    Scr_GetVector(0, pos, SCRIPTINSTANCE_SERVER);
    G_TraceCapsule(&trace, pos, playerMins, playerMaxs, pos, ENTITYNUM_NONE, 42057745, &context);
    if ( trace.fraction < 1.0f || trace.allsolid || trace.startsolid )
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PreventMipsFromLoading ~8260D2B0)
void Scr_PreventMipsFromLoading()
{
    const char *materialName;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("call PreventMipsFromLoading with one parameter: material name (include mc/ or wc/ if applicable)", 0);
    materialName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    SV_GameSendServerCommand(-1, SV_CMD_RELIABLE, va("%c 1 %s", '-', materialName));
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_RefreshHudAmmoCounter ~8261ADB8)
void Scr_RefreshHudAmmoCounter()
{
    SV_GameSendServerCommand(-1, SV_CMD_CAN_IGNORE, va("%c \"%i\"", 'J', 1));
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_RefreshHudCompass ~8261AD78)
void Scr_RefreshHudCompass()
{
    SV_GameSendServerCommand(-1, SV_CMD_CAN_IGNORE, va("%c \"%i\"", 'J', 3));
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ReportClientDisconnected ~8261BEF0)
void GScr_ReportClientDisconnected()
{
    const char *reason;

    reason = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    SV_GameSendServerCommand(-1, SV_CMD_CAN_IGNORE, va("%c %s %s", 'e', "EXE_LEFTGAME", reason));
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ResetAILimit ~8261BED0)
void GScr_ResetAILimit()
{
    g_scriptAiLimit = 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_ResetCurve ~82603DA0)
void Scr_ResetCurve()
{
    int curve;

    curve = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    cCurveManager::ResetCurve(curve);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_ResetSunDirection ~82610F18)
void Scr_ResetSunDirection()
{
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        Scr_Error("Incorrect number of parameters\n", 0);
    SV_SetConfigstring(9, "");
}

static int Scr_RopeFlagFromName(const char *flagName)
{
    if ( !I_stricmp(flagName, "keep_ent_anchors") )
        return 16;
    if ( !I_stricmp(flagName, "collide") )
        return 1;
    if ( !I_stricmp(flagName, "detach_opposite_anchor") )
        return 32;
    if ( !I_stricmp(flagName, "force_update") )
        return 64;
    if ( !I_stricmp(flagName, "no_wind") )
        return 128;
    if ( !I_stricmp(flagName, "no_lod") )
        return 256;
    return 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_RopeSetFlag ~8260D780)
void Scr_RopeSetFlag()
{
    int ropeId;
    const char *flagName;
    int flagBit;
    int enabled;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 3 )
        Scr_Error("Incorrect number of parameters", 0);
    ropeId = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    flagName = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    flagBit = Scr_RopeFlagFromName(flagName);
    if ( !flagBit )
        return;
    enabled = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER) != 0;
    SV_GameSendServerCommand(
        -1,
        SV_CMD_RELIABLE,
        va("# %d %d %d %d", (int)'S', ropeId, flagBit, enabled));
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_RopeSetParam ~8260D918)
void Scr_RopeSetParam()
{
    int ropeId;
    const char *paramName;
    float width;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 3 )
        Scr_Error("Incorrect number of parameters", 0);
    ropeId = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    paramName = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    if ( I_stricmp(paramName, "width") )
        return;
    width = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    SV_GameSendServerCommand(
        -1,
        SV_CMD_RELIABLE,
        va("# %d %d %d %1.1f", (int)'T', ropeId, 1, width));
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_RopeAddEntityAnchor ~8260D9F0)
void Scr_RopeAddEntityAnchor()
{
    int ropeId;
    float fraction;
    gentity_s *ent;
    float anchorLoc[3];

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 4 )
        Scr_Error("Incorrect number of parameters", 0);
    ropeId = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    fraction = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    if ( fraction < 0.0f || fraction > 1.0f )
        Scr_Error("The fraction must be between 0 and 1", 0);
    ent = Scr_GetEntity(2u);
    if ( !ent || !ent->r.inuse )
        Scr_Error("Bad entity specified", 0);
    Scr_GetVector(3u, anchorLoc, SCRIPTINSTANCE_SERVER);
    SV_GameSendServerCommand(
        -1,
        SV_CMD_RELIABLE,
        va(
            "# %d %d %1.1f %d %1.1f %1.1f %1.1f",
            (int)'U',
            ropeId,
            fraction,
            ent->s.number,
            anchorLoc[0],
            anchorLoc[1],
            anchorLoc[2]));
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_RopeAddWorldAnchor ~8260DBE0)
void Scr_RopeAddWorldAnchor()
{
    int ropeId;
    float fraction;
    float anchorLoc[3];

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 3 )
        Scr_Error("Incorrect number of parameters", 0);
    ropeId = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    fraction = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    if ( fraction < 0.0f || fraction > 1.0f )
        Scr_Error("The fraction must be between 0 and 1", 0);
    Scr_GetVector(2u, anchorLoc, SCRIPTINSTANCE_SERVER);
    SV_GameSendServerCommand(
        -1,
        SV_CMD_RELIABLE,
        va(
            "# %d %d %1.1f %d %1.1f %1.1f %1.1f",
            (int)'U',
            ropeId,
            fraction,
            -1,
            anchorLoc[0],
            anchorLoc[1],
            anchorLoc[2]));
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_RopeCollide ~8260D720)
void Scr_RopeCollide()
{
    Com_Printf(
        25,
        "RopeCollide is obsolete. Use ropesetflag( ropeid, \"collide\", 1 ) instead.\n");
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_RopeRemoveAnchor ~8260DD98)
void Scr_RopeRemoveAnchor()
{
    int ropeId;
    float fraction;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2 )
        Scr_Error("Incorrect number of parameters.", 0);
    ropeId = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    fraction = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    if ( fraction < 0.0f || fraction > 1.0f )
        Scr_Error("The fraction must be between 0 and 1", 0);
    SV_GameSendServerCommand(
        -1,
        SV_CMD_RELIABLE,
        va("# %d %d %1.1f", (int)'V', ropeId, fraction));
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_RopeMoveAnchor ~8260DE60)
void Scr_RopeMoveAnchor()
{
    int ropeId;
    float fraction;
    float anchorLoc[3];

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 3 )
        Scr_Error("Incorrect number of parameters", 0);
    ropeId = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    fraction = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    if ( fraction < 0.0f || fraction > 1.0f )
        Scr_Error("The fraction must be between 0 and 1", 0);
    Scr_GetVector(2u, anchorLoc, SCRIPTINSTANCE_SERVER);
    SV_GameSendServerCommand(
        -1,
        SV_CMD_RELIABLE,
        va(
            "# %d %d %1.1f %1.1f %1.1f %1.1f",
            (int)'W',
            ropeId,
            fraction,
            anchorLoc[0],
            anchorLoc[1],
            anchorLoc[2]));
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_RopeGetPosition ~8260E018)
void Scr_RopeGetPosition()
{
    int ropeId;
    float fraction;
    float pos[3];

    if ( onlinegame->current.enabled )
        Scr_Error("ropegetposition doesnt support online games.", 0);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2 )
        Scr_Error("Incorrect number of parameters", 0);
    ropeId = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    fraction = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    if ( fraction < 0.0f || fraction > 1.0f )
        Scr_Error("The fraction must be between 0 and 1", 0);
    pos[0] = 0.0f;
    pos[1] = 0.0f;
    pos[2] = 0.0f;
    Rope_GetPosition(ropeId, fraction, pos);
    Scr_AddVector(pos, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_ScaleBuoyancy ~825FAB70)
void ScrCmd_ScaleBuoyancy(scr_entref_t entref)
{
    gentity_s *ent;
    gentity_s *tmp;
    float scale;

    ent = GetEntity(entref);
    scale = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    tmp = G_TempEntity(vec3_origin, EV_SCALE_BUOYANCY);
    AssignToSmallerType<short>(&tmp->s.otherEntityNum, ent->s.number);
    AssignToSmallerType<unsigned short>(&tmp->s.eventParm, (unsigned short)(int)(scale * 1000.0f));
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetAnimDumpUseServerAnims ~825F60F0)
void GScr_SetAnimDumpUseServerAnims()
{
    Dvar_SetBoolByName("scriptmover_useServerAnims", Scr_GetInt(0, SCRIPTINSTANCE_SERVER) > 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SoundFade)
void Scr_SoundFade()
{
    const char *fmtMsg;
    float fTargetVol;
    int iFadeTime;

    fTargetVol = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 1 )
        iFadeTime = 0;
    else
        iFadeTime = (int)(Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER) * 1000.0);
    fmtMsg = va("%c %f %i\n", 113, fTargetVol, iFadeTime);
    SV_GameSendServerCommand(-1, SV_CMD_RELIABLE, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PrecacheModel)
void Scr_PrecacheModel()
{
    char *modelName;

    if (!level.initializing)
        Scr_Error("precacheModel must be called before any wait statements in the gametype or level script\n", 0);
    modelName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if (!*modelName)
        Scr_ParamError(0, "Model name string is empty", SCRIPTINSTANCE_SERVER);
    if (useFastFile->current.enabled)
        Scr_ErrorOnDefaultAsset(ASSET_TYPE_XMODEL, modelName);
    G_ModelIndex(modelName);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_ErrorOnDefaultAsset)
void __cdecl Scr_ErrorOnDefaultAsset(XAssetType type, char *assetName)
{
    const char *XAssetTypeName;
    const char *fmtMsg;

    DB_FindXAssetHeader(type, assetName, 1, -1);
    if ( DB_IsXAssetDefault(type, assetName) )
    {
#ifdef KISAK_SP
        if ( Com_IsMenuLevel(Dvar_GetString("mapname")) )
            return;
        if ( zombiemode->current.enabled && level.initializing )
            return;
#endif
        XAssetTypeName = DB_GetXAssetTypeName(type);
        fmtMsg = va("precache %s '%s' failed", XAssetTypeName, assetName);
        Scr_NeverTerminalError(fmtMsg, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PrecacheShellShock)
void Scr_PrecacheShellShock()
{
    shellshock_parms_t *ShellshockParms;
    char *shellshockName;
    unsigned int index;

    if ( !level.initializing )
        Scr_Error("precacheShellShock must be called before any wait statements in the gametype or level script\n", 0);
    shellshockName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    index = G_ShellShockIndex(shellshockName);
    if ( !BG_LoadShellShockDvars(shellshockName) )
        Com_Error(ERR_DROP, "couldn't find shell shock %s -- see console", shellshockName);
    ShellshockParms = BG_GetShellshockParms(index);
    BG_SetShellShockParmsFromDvars(ShellshockParms);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PrecacheItem)
void Scr_PrecacheItem()
{
    const char *fmtMsg;
    char *pszItemName;

    if ( !level.initializing )
        Scr_Error("precacheItem must be called before any wait statements in the gametype or level script\n", 0);
    pszItemName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( !G_FindItem(pszItemName) )
    {
        fmtMsg = va("unknown item '%s'", pszItemName);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PrecacheShader)
void Scr_PrecacheShader()
{
    char *shaderName;

    if (!level.initializing)
        Scr_Error("precacheShader must be called before any wait statements in the gametype or level script\n", 0);
    shaderName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if (!*shaderName)
        Scr_ParamError(0, "Shader name string is empty", SCRIPTINSTANCE_SERVER);
    G_MaterialIndex( shaderName);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PrecacheString)
void Scr_PrecacheString()
{
    char *stringName;

    if (!level.initializing)
        Scr_Error("precacheString must be called before any wait statements in the gametype or level script\n", 0);
    stringName = Scr_GetIString(0, SCRIPTINSTANCE_SERVER);
    if (*stringName)
        G_LocalizedStringIndex( stringName);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GrenadeExplosionEffect)
void Scr_GrenadeExplosionEffect()
{
    unsigned __int8 tempValue0;
    col_context_t context;
    float vDir[3];
    float vOrg[3];
    float vEnd[3];
    trace_t trace;
    gentity_s *pEnt;
    float vPos[3];

    memset(&trace, 0, 16);
    //col_context_t::col_context_t(&context);
    Scr_GetVector(0, vOrg, SCRIPTINSTANCE_SERVER);
    vPos[0] = vOrg[0];
    vPos[1] = vOrg[1];
    vPos[2] = vOrg[2] + 1.0;
    pEnt = G_TempEntity(vPos, EV_GRENADE_EXPLODE);
    vDir[0] = 0.0f;
    vDir[1] = 0.0f;
    vDir[2] = 1.0f;
    tempValue0 = DirToByte(vDir);
    pEnt->s.eventParm = tempValue0;
    vEnd[0] = vPos[0];
    vEnd[1] = vPos[1];
    vEnd[2] = vPos[2] - 17.0;
    G_TraceCapsule(&trace, vPos, vec3_origin, vec3_origin, vEnd, 1023, 2065, &context);
    pEnt->s.surfType = (trace.sflags & 0x3F00000) >> 20;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_RadiusDamage)
void GScr_RadiusDamage()
{
    GScr_RadiusDamageInternal(0);
}

// Decomp: CoDSP_rdBlackOps.map.c:9468
void __cdecl GScr_RadiusDamageInternal(gentity_s *inflictor)
{
    char *String;
    gentity_s *attacker;
    meansOfDeath_t mod;
    float max_damage;
    float origin[3];
    float range;
    int weapon;
    float min_damage;

    Scr_GetVector(0, origin, SCRIPTINSTANCE_SERVER);
    range = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    max_damage = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    min_damage = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
    attacker = &g_entities[1022];
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 4 && Scr_GetType(4u, SCRIPTINSTANCE_SERVER) )
        attacker = Scr_GetEntity(4u);
    mod = MOD_EXPLOSIVE;
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 5 && Scr_GetType(5u, SCRIPTINSTANCE_SERVER) )
        mod = (meansOfDeath_t)G_MeansOfDeathFromScriptParam(5u);
    weapon = -1;
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 6 && Scr_GetType(6u, SCRIPTINSTANCE_SERVER) )
    {
        String = Scr_GetString(6u, SCRIPTINSTANCE_SERVER);
        weapon = G_GetWeaponIndexForName(String);
    }
    level.bPlayerIgnoreRadiusDamage = level.bPlayerIgnoreRadiusDamageLatched;
    G_RadiusDamage(origin, inflictor, attacker, max_damage, min_damage, range, 1.0, 0, inflictor, mod, weapon);
    level.bPlayerIgnoreRadiusDamage = 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_TimedRadiusDamage @ 82611628)
void Scr_TimedRadiusDamage()
{
    float origin[3];
    float radius;
    int damage;
    float rate;
    float life;
    int fireStarterClientNum;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 6 )
        Scr_Error("Incorrect usage for timedradiusdamage()", 0);
    Scr_GetVector(0, origin, SCRIPTINSTANCE_SERVER);
    radius = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    damage = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
    rate = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
    life = Scr_GetFloat(4u, SCRIPTINSTANCE_SERVER);
    fireStarterClientNum = Scr_GetInt(5u, SCRIPTINSTANCE_SERVER);
    TimedRadiusDamage(origin, radius, damage, rate, life, fireStarterClientNum);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_EntityRadiusDamage)
void __cdecl GScr_EntityRadiusDamage(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    GScr_RadiusDamageInternal(ent);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GlassRadiusDamage)
void GScr_GlassRadiusDamage()
{
    meansOfDeath_t mod;
    float max_damage;
    float origin[3];
    float range;
    float min_damage;

    Scr_GetVector(0, origin, SCRIPTINSTANCE_SERVER);
    range = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    max_damage = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    min_damage = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
    mod = MOD_EXPLOSIVE;
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 4 && Scr_GetType(4u, SCRIPTINSTANCE_SERVER) )
        mod = (meansOfDeath_t)G_MeansOfDeathFromScriptParam(4u);
    GlassSv_RadiusDamage(origin, range, 1.0, 0, max_damage, min_damage, mod);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_Detonate)
void __cdecl GScr_Detonate(scr_entref_t entref)
{
    gentity_s *ent;
    const WeaponDef *weapDef;
    gentity_s *player;

    ent = GetEntity(entref);
    weapDef = BG_GetWeaponDef(ent->s.weapon);
    if ( ent->s.eType != 4
        || !weapDef
        || weapDef->weapType != WEAPTYPE_GRENADE && weapDef->weapType != WEAPTYPE_PROJECTILE )
    {
        Scr_ObjectError("entity is not a grenade or projectile", SCRIPTINSTANCE_SERVER);
    }
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) )
        {
            player = Scr_GetEntity(0);
            if ( !player->client )
                Scr_ParamError(0, "Entity is not a player", SCRIPTINSTANCE_SERVER);
            ent->parent.setEnt(player);
        }
        else
        {
            ent->parent.setEnt(&g_entities[1022]);
        }
    }
    G_ExplodeMissile(ent);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetPlayerIgnoreRadiusDamage)
void GScr_SetPlayerIgnoreRadiusDamage()
{
    level.bPlayerIgnoreRadiusDamageLatched = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c:9472
void __cdecl GScr_DamageConeTrace(scr_entref_t entref)
{
    GScr_DamageConeTraceInternal(entref, 8396819);
}

// Decomp: CoDSP_rdBlackOps.map.c:9473
void __cdecl GScr_DamageConeTraceInternal(scr_entref_t entref, int contentMask)
{
    float damageAngles[3];
    float coneAngleDegrees;
    gentity_s *target;
    float damageOrigin[3];
    gentity_s *ignoreEnt;
    float damageAmount;
    float coneAngleCos;
    unsigned int numParam;
    bool useDirectionalCone;

    target = GetEntity(entref);
    Scr_GetVector(0, damageOrigin, SCRIPTINSTANCE_SERVER);
    numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    ignoreEnt = numParam > 1 ? Scr_GetEntity(1u) : 0;
    useDirectionalCone = numParam > 2;
    if ( useDirectionalCone )
        Scr_GetVector(2u, damageAngles, SCRIPTINSTANCE_SERVER);
    if ( numParam <= 3 )
        coneAngleDegrees = 65.0f;
    else
        coneAngleDegrees = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
    coneAngleCos = useDirectionalCone ? cosf(coneAngleDegrees * 0.017453292f) : 1.0f;
    damageAmount = CanDamage(
        target,
        ignoreEnt,
        damageOrigin,
        coneAngleCos,
        useDirectionalCone ? damageAngles : 0,
        contentMask);
    Scr_AddFloat(damageAmount, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c:9474
void __cdecl GScr_SightConeTrace(scr_entref_t entref)
{
    GScr_DamageConeTraceInternal(entref, 14337);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_PlayerSightTrace)
void __cdecl GScr_PlayerSightTrace(scr_entref_t entref)
{
    gclient_s *client;
    float dist;
    int distance;
    float viewdir[3];
    gentity_s *ent;
    int hitNum;
    float itemPosition[3];
    float dot;
    float playerEyes[3];
    float objdir[3];

    ent = GetEntity(entref);
    client = ent->client;
    playerEyes[0] = client->ps.origin[0];
    playerEyes[1] = client->ps.origin[1];
    playerEyes[2] = client->ps.origin[2];
    playerEyes[2] = playerEyes[2] + ent->client->ps.viewHeightCurrent;
    Scr_GetVector(0, itemPosition, SCRIPTINSTANCE_SERVER);
    distance = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    hitNum = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
    AngleVectors(ent->r.currentAngles, viewdir, 0, 0);
    objdir[0] = itemPosition[0] - playerEyes[0];
    objdir[1] = itemPosition[1] - playerEyes[1];
    objdir[2] = itemPosition[2] - playerEyes[2];
    dist = Abs(objdir);
    objdir[0] = (float)(1.0 / dist) * objdir[0];
    objdir[1] = (float)(1.0 / dist) * objdir[1];
    objdir[2] = (float)(1.0 / dist) * objdir[2];
    Vec3Normalize(viewdir);
    Vec3Normalize(objdir);
    dot = (float)((float)(viewdir[0] * objdir[0]) + (float)(viewdir[1] * objdir[1])) + (float)(viewdir[2] * objdir[2]);
    if ( dot >= 0.70700002 && dist <= (float)distance || dist < 100.0 && dot > 0.0 )
    {
        //col_context_t::col_context_t(&context, (int)&loc_806823);
        col_context_t context(0x806823);
        context.passEntityNum0 = ent->s.number;
        SV_SightTracePoint(&hitNum, playerEyes, itemPosition, &context);
        Scr_AddInt(hitNum, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_HeliTurretSightTrace)
void __cdecl GScr_HeliTurretSightTrace(scr_entref_t entref)
{
    gclient_s *client;
    gentity_s *copterEnt;
    float turretPosition[3];
    int hitNum;
    gentity_s *player;
    float playerEyes[3];

    copterEnt = GetEntity(entref);
    Scr_GetVector(0, turretPosition, SCRIPTINSTANCE_SERVER);
    player = Scr_GetEntity(1u);
    client = player->client;
    playerEyes[0] = client->ps.origin[0];
    playerEyes[1] = client->ps.origin[1];
    playerEyes[2] = client->ps.origin[2];
    playerEyes[2] = playerEyes[2] + player->client->ps.viewHeightCurrent;
    hitNum = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
    //col_context_t::col_context_t(&context, 14337);
    col_context_t context(0x3801);
    context.passEntityNum0 = copterEnt->s.number;
    SV_SightTracePoint(&hitNum, playerEyes, turretPosition, &context);
    Scr_AddInt(hitNum, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_HeliTurretDogTrace)
void __cdecl GScr_HeliTurretDogTrace(scr_entref_t entref)
{
    gentity_s *dog;
    gentity_s *copterEnt;
    float turretPosition[3];
    int hitNum;
    float dogEyes[3];

    copterEnt = GetEntity(entref);
    Scr_GetVector(0, turretPosition, SCRIPTINSTANCE_SERVER);
    dog = Scr_GetEntity(1u);
    dogEyes[0] = dog->r.currentOrigin[0];
    dogEyes[1] = dog->r.currentOrigin[1];
    dogEyes[2] = dog->r.currentOrigin[2];
    dogEyes[2] = dogEyes[2] + 24.0;
    hitNum = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
    //col_context_t::col_context_t(&context, 14337);
    col_context_t context(0x3801);
    context.passEntityNum0 = copterEnt->s.number;
    SV_SightTracePoint(&hitNum, dogEyes, turretPosition, &context);
    Scr_AddInt(hitNum, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_VisionSetLerpRatio)
void __cdecl GScr_VisionSetLerpRatio(scr_entref_t entref)
{
    float visionSetLerpRatio;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_Error("USAGE: Must be called on a client\n", 0);
    visionSetLerpRatio = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    if ( visionSetLerpRatio < 0.0f )
        visionSetLerpRatio = 0.0f;
    else if ( visionSetLerpRatio > 1.0f )
        visionSetLerpRatio = 1.0f;
    ent->client->ps.visionSetLerpRatio = visionSetLerpRatio;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_DirectionalHitIndicator)
void __cdecl GScr_DirectionalHitIndicator(scr_entref_t entref)
{
    gentity_s *pSelf;
    unsigned __int16 hitEntBitArray1;
    float zero_vec[3];
    gentity_s *temporary_entity;
    int hitEntBitArray0;
    int client_index;

    pSelf = GetEntity(entref);
    hitEntBitArray1 = 0;
    hitEntBitArray0 = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 2 || !Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        Scr_Error("USAGE: DirectionalHitIndicator ( <victims0>, <victims1> )\n", 0);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 2 )
        hitEntBitArray1 = (unsigned __int16)Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    memset(zero_vec, 0, sizeof(zero_vec));
    temporary_entity = G_TempEntity(zero_vec, EV_DIRECTIONAL_HIT_INDICATOR);
    temporary_entity->r.clientMask[0] = -1;
    temporary_entity->s.eventParms[0] = hitEntBitArray0;
    temporary_entity->s.eventParms[1] = hitEntBitArray1;
    client_index = pSelf->client->ps.clientNum;
    temporary_entity->r.clientMask[client_index >> 5] &= ~(1 << (client_index & 0x1F));
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_DoCowardsWayAnims)
void __cdecl GScr_DoCowardsWayAnims(scr_entref_t entref)
{
    float value; // xmm0_4
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_Error("USAGE: Must be called on a client\n", 0);
    if ( !ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 9014, 0, "%s", "ent->client") )
    {
        __debugbreak();
    }
    value = (float)(BG_AnimScriptEvent(&g_pmove[ent->client->ps.clientNum], ANIM_ET_LASTSTAND_SUICIDE, 0, 1) - 100)
                / 1000.0;
    Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_StartPoisoning)
void __cdecl GScr_StartPoisoning(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_Error("USAGE: Must be called on a client\n", 0);
    if ( !ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 9039, 0, "%s", "ent->client") )
    {
        __debugbreak();
    }
    ent->client->ps.poisoned = 1;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_StopPoisoning)
void __cdecl GScr_StopPoisoning(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_Error("USAGE: Must be called on a client\n", 0);
    if ( !ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 9060, 0, "%s", "ent->client") )
    {
        __debugbreak();
    }
    ent->client->ps.poisoned = 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_StartBinocs)
void __cdecl GScr_StartBinocs(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_Error("USAGE: Must be called on a client\n", 0);
    if ( !ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 9081, 0, "%s", "ent->client") )
    {
        __debugbreak();
    }
    ent->client->ps.binoculars = 1;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_StopBinocs)
void __cdecl GScr_StopBinocs(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_Error("USAGE: Must be called on a client\n", 0);
    if ( !ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 9102, 0, "%s", "ent->client") )
    {
        __debugbreak();
    }
    ent->client->ps.binoculars = 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsFlared)
void __cdecl GScr_IsFlared(scr_entref_t entref)
{
    gentity_s *ent;
    unsigned int isFlared;

    isFlared = 0;
    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_Error("USAGE: Must be called on a client\n", 0);
    if ( !ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 9124, 0, "%s", "ent->client") )
    {
        __debugbreak();
    }
    if ( ent->client->ps.visionSetLerpRatio > 0.0 )
        isFlared = 1;
    Scr_AddBool(isFlared, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsPoisoned)
void __cdecl GScr_IsPoisoned(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_Error("USAGE: Must be called on a client\n", 0);
    Scr_AddBool(ent->client->ps.poisoned, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetMoveDelta)
void GScr_GetMoveDelta()
{
    const XAnim_s *Anims;
    unsigned int index;
    float time1;
    float time2;
    int NumParam;
    float trans[3];
    float endTime;
    float startTime;
    float rot[2];
    scr_anim_s anim;

    startTime = 0.0f;
    endTime = 1.0f;
    NumParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( NumParam != 1 )
    {
        if ( NumParam != 2 )
        {
            endTime = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
            if ( endTime < 0.0 || endTime > 1.0 )
                Scr_ParamError(2u, "end time must be between 0 and 1", SCRIPTINSTANCE_SERVER);
        }
        startTime = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
        if ( startTime < 0.0 || startTime > 1.0 )
            Scr_ParamError(1u, "start time must be between 0 and 1", SCRIPTINSTANCE_SERVER);
    }
    anim = Scr_GetAnim(0, 0, SCRIPTINSTANCE_SERVER);
    time2 = endTime;
    time1 = startTime;
    index = anim.index;
    Anims = Scr_GetAnims(anim.tree, SCRIPTINSTANCE_SERVER);
    XAnimGetRelDelta(Anims, index, rot, trans, time1, time2);
    Scr_AddVector(trans, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetAngleDelta)
void GScr_GetAngleDelta()
{
    const XAnim_s *Anims;
    unsigned int index;
    float time1;
    float time1a;
    float time2;
    int NumParam;
    float trans[3];
    float endTime;
    float startTime;
    float rot[2];
    scr_anim_s anim;

    startTime = 0.0f;
    endTime = 1.0f;
    NumParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( NumParam != 1 )
    {
        if ( NumParam != 2 )
        {
            endTime = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
            if ( endTime < 0.0 || endTime > 1.0 )
                Scr_ParamError(2u, "end time must be between 0 and 1", SCRIPTINSTANCE_SERVER);
        }
        startTime = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
        if ( startTime < 0.0 || startTime > 1.0 )
            Scr_ParamError(1u, "start time must be between 0 and 1", SCRIPTINSTANCE_SERVER);
    }
    anim = Scr_GetAnim(0, 0, SCRIPTINSTANCE_SERVER);
    time2 = endTime;
    time1 = startTime;
    index = anim.index;
    Anims = Scr_GetAnims(anim.tree, SCRIPTINSTANCE_SERVER);
    XAnimGetRelDelta(Anims, index, rot, trans, time1, time2);
    time1a = RotationToYaw(rot);
    Scr_AddFloat(time1a, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetStartOrigin)
void GScr_GetStartOrigin()
{
    scr_anim_s anim;
    const XAnim_s *anims;
    float origin[3];
    float angles[3];
    float axis[4][3];
    float rot[3];
    float trans[3];
    float result[3];

    Scr_GetVector(0, origin, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, angles, SCRIPTINSTANCE_SERVER);
    anim = Scr_GetAnim(2u, 0, SCRIPTINSTANCE_SERVER);
    anims = Scr_GetAnims(anim.tree, SCRIPTINSTANCE_SERVER);
    XAnimGetAbsDelta(anims, anim.index, rot, trans, 0.0f);
    AnglesToAxis(angles, axis);
    axis[3][0] = origin[0];
    axis[3][1] = origin[1];
    axis[3][2] = origin[2];
    MatrixTransformVector43(trans, axis, result);
    Scr_AddVector(result, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetStartAngles)
void GScr_GetStartAngles()
{
    scr_anim_s anim;
    const XAnim_s *anims;
    float origin[3];
    float angles[3];
    float axis[3][3];
    float yawAxis[3][3];
    float newMat[3][3];
    float resultAngles[3];
    float rot[3];
    float trans[3];

    Scr_GetVector(0, origin, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, angles, SCRIPTINSTANCE_SERVER);
    anim = Scr_GetAnim(2u, 0, SCRIPTINSTANCE_SERVER);
    anims = Scr_GetAnims(anim.tree, SCRIPTINSTANCE_SERVER);
    XAnimGetAbsDelta(anims, anim.index, rot, trans, 0.0f);
    AnglesToAxis(angles, axis);
    YawToAxis((float)RotationToYaw(rot), yawAxis);
    MatrixMultiply(yawAxis, axis, newMat);
    AxisToAngles(newMat, resultAngles);
    Scr_AddVector(resultAngles, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c:9529
void GScr_GetNorthYaw()
{
    float value;
    char northYawString[32];

    SV_GetConfigstring(0x60Cu, northYawString, 32);
    value = atof(northYawString);
    Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_LoadFX)
void Scr_LoadFX()
{
    char *filename;
    int id;

    filename = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( !I_strncmp(filename, "fx/", 3) )
        Scr_ParamError(0, "effect name should start after the 'fx' folder.", SCRIPTINSTANCE_SERVER);
    id = G_EffectIndex(filename);
    if ( !id && !level.initializing )
        Scr_Error(
            "loadFx must be called before any wait statements in the level script, or on an already loaded effect\n",
            0);
    Scr_AddInt(id, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PlayFX)
void Scr_PlayFX()
{
    float pos[3];
    int numParams;
    int fxId;
    gentity_s *ent;
    float axis[3][3];
    float vecLength;

    numParams = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParams < 2 || numParams > 4 )
        Scr_Error("Incorrect number of parameters", 0);
    fxId = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, pos, SCRIPTINSTANCE_SERVER);
    ent = G_TempEntity(pos, EV_PLAY_FX);
    if ( ent->s.lerp.apos.trType
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    9314,
                    1,
                    "%s",
                    "ent->s.lerp.apos.trType == TR_STATIONARY") )
    {
        __debugbreak();
    }
    ent->s.eventParm = (unsigned __int8)fxId;
    if ( numParams == 2 )
    {
        Scr_SetFxAngles(0, axis, ent->s.lerp.apos.trBase);
    }
    else
    {
        if ( numParams != 3
            && numParams != 4
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                        9322,
                        1,
                        "%s\n\t(numParams) = %i",
                        "(numParams == 3 || numParams == 4)",
                        numParams) )
        {
            __debugbreak();
        }
        Scr_GetVector(2u, axis[0], SCRIPTINSTANCE_SERVER);
        vecLength = Vec3Normalize(axis[0]);
        if ( vecLength == 0.0 )
            Scr_FxParamError(2u, "playFx called with (0 0 0) forward direction", fxId);
        if ( numParams == 3 )
        {
            Scr_SetFxAngles(1u, axis, ent->s.lerp.apos.trBase);
        }
        else
        {
            if ( numParams != 4
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                            9335,
                            1,
                            "%s\n\t(numParams) = %i",
                            "(numParams == 4)",
                            numParams) )
            {
                __debugbreak();
            }
            Scr_GetVector(3u, axis[2], SCRIPTINSTANCE_SERVER);
            vecLength = Vec3Normalize(axis[2]);
            if ( vecLength == 0.0 )
                Scr_FxParamError(3u, "playFx called with (0 0 0) up direction", fxId);
            Scr_SetFxAngles(2u, axis, ent->s.lerp.apos.trBase);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetFxAngles)
void __cdecl Scr_SetFxAngles(unsigned int givenAxisCount, float (*axis)[3], float *angles)
{
    const char *fmtMsg;
    float reflectScale;

    if ( givenAxisCount > 2
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    9241,
                    0,
                    "givenAxisCount not in [0, 2]\n\t%i not in [%i, %i]",
                    givenAxisCount,
                    0,
                    2) )
    {
        __debugbreak();
    }
    if ( givenAxisCount == 1 )
    {
        vectoangles((const float *)axis, angles);
    }
    else if ( givenAxisCount == 2 )
    {
        reflectScale = -((float)((float)((*axis)[0] * (*axis)[6]) + (float)((*axis)[1] * (*axis)[7])) + (float)((*axis)[2] * (*axis)[8]));
        (*axis)[6] = (float)(reflectScale * (*axis)[0]) + (*axis)[6];
        (*axis)[7] = (float)(reflectScale * (*axis)[1]) + (*axis)[7];
        (*axis)[8] = (float)(reflectScale * (*axis)[2]) + (*axis)[8];
        if ( Vec3Normalize(&(*axis)[6]) == 0.0 )
        {
            fmtMsg = va("forward and up vectors are the same direction or exact opposite directions");
            Scr_Error(fmtMsg, 0);
        }
        Vec3Cross(&(*axis)[6], (const float *)axis, &(*axis)[3]);
        AxisToAngles(axis, angles);
    }
    else
    {
        *angles = 270.0f;
        angles[1] = 0.0f;
        angles[2] = 0.0f;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_FxParamError)
void __cdecl Scr_FxParamError(unsigned int paramIndex, const char *errorString, int fxId)
{
    const char *fmtMsg;
    char fxName[1028];

    if ( !errorString
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 9285, 0, "%s", "errorString") )
    {
        __debugbreak();
    }
    if ( fxId )
        SV_GetConfigstring(fxId + 2080, fxName, 1024);
    else
        strcpy(fxName, "not successfully loaded");
    fmtMsg = va("%s (effect = %s)\n", errorString, fxName);
    Scr_ParamError(paramIndex, fmtMsg, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PlayFXOnTag)
void Scr_PlayFXOnTag()
{
    const char *fmtMsg;
    char *slStr;
    char *tempValue2;
    unsigned int modelNameStr;
    char *slStr2;
    const char *fmtMsg2;
    char *slStr3;
    char *fmtMsg3;
    char *slStr4;
    int fxId;
    gentity_s *ent;
    unsigned int tag;
    signed int csIndex;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 3 )
        Scr_Error("Incorrect number of parameters", 0);
    fxId = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( fxId <= 0 || fxId >= 196 )
    {
        fmtMsg = va("effect id %i is invalid\n", fxId);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    ent = Scr_GetEntity(1u);
    if ( !ent->model )
        Scr_ParamError(1u, "cannot play fx on entity with no model", SCRIPTINSTANCE_SERVER);
    tag = Scr_GetConstLowercaseString(2u, SCRIPTINSTANCE_SERVER);
    slStr = SL_ConvertToString(tag, SCRIPTINSTANCE_SERVER);
    tempValue2 = strchr(slStr, 0x22u);
    if ( tempValue2 )
        Scr_ParamError(2u, "cannot use \" characters in tag names\n", SCRIPTINSTANCE_SERVER);
    if ( tag != scr_const.tag_origin && SV_DObjGetBoneIndex(ent, tag) < 0 )
    {
        SV_DObjDumpInfo(ent);
        modelNameStr = G_ModelName(ent->model);
        slStr4 = SL_ConvertToString(modelNameStr, SCRIPTINSTANCE_SERVER);
        slStr2 = SL_ConvertToString(tag, SCRIPTINSTANCE_SERVER);
        fmtMsg2 = va("tag '%s' does not exist on entity with model '%s'", slStr2, slStr4);
        Scr_ParamError(2u, fmtMsg2, SCRIPTINSTANCE_SERVER);
    }
    slStr3 = SL_ConvertToString(tag, SCRIPTINSTANCE_SERVER);
    fmtMsg3 = va("%03d%s", fxId, slStr3);
    csIndex = G_FindConfigstringIndex(fmtMsg3, 2276, 256, 1, 0);
    if ( (csIndex <= 0 || csIndex >= 256)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    9378,
                    0,
                    "%s",
                    "csIndex > 0 && csIndex < MAX_EFFECT_TAGS") )
    {
        __debugbreak();
    }
    G_AddEvent(ent, 0x47u, csIndex);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PlayLoopedFX)
void Scr_PlayLoopedFX()
{
    int NumParam;
    float pos[3];
    int fxId;
    int repeat;
    gentity_s *ent;
    int givenAxisCount;
    float axis[3][3];
    float cullDist;

    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 3
        || (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 6 )
    {
        Scr_Error("Incorrect number of parameters", 0);
    }
    givenAxisCount = 0;
    cullDist = 0.0f;
    fxId = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    NumParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( NumParam == 6 )
    {
        ++givenAxisCount;
        Scr_GetVector(5u, axis[2], SCRIPTINSTANCE_SERVER);
        if ( Vec3Normalize(axis[2]) == 0.0 )
            Scr_FxParamError(5u, "playLoopedFx called with (0 0 0) up direction", fxId);
    }
    if ( NumParam >= 5 )
    {
        Scr_GetVector(4u, axis[0], SCRIPTINSTANCE_SERVER);
        if ( Vec3Normalize(axis[0]) == 0.0 )
            Scr_FxParamError(4u, "playLoopedFx called with (0 0 0) forward direction", fxId);
        ++givenAxisCount;
    }
    if ( NumParam >= 4 )
        cullDist = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(2u, pos, SCRIPTINSTANCE_SERVER);
    repeat = (int)(Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER) * 1000.0 + 9.313225746154785e-10);
    if ( repeat <= 0 )
        Scr_FxParamError(1u, "playLoopedFx called with repeat < 0.001 seconds", fxId);
    ent = G_Spawn();
    ent->s.eType = ET_LOOP_FX;
    ent->r.svFlags |= 8u;
    ent->s.un1.scale = fxId;
    if ( ent->s.un1.scale != fxId
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    9428,
                    0,
                    "%s",
                    "ent->s.un1.eventParm2 == fxId") )
    {
        __debugbreak();
    }
    G_SetOrigin(ent, pos);
    Scr_SetFxAngles(givenAxisCount, axis, ent->s.lerp.apos.trBase);
    ent->s.lerp.u.turret.gunAngles[0] = cullDist;
    ent->s.lerp.u.loopFx.period = repeat;
    SV_LinkEntity(ent);
    Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SpawnFX)
void Scr_SpawnFX()
{
    int NumParam;
    float pos[3];
    int fxId;
    gentity_s *ent;
    int givenAxisCount;
    float axis[3][3];

    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 2
        || (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 4 )
    {
        Scr_Error("Incorrect number of parameters", 0);
    }
    givenAxisCount = 0;
    fxId = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    NumParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( NumParam == 4 )
    {
        Scr_GetVector(3u, axis[2], SCRIPTINSTANCE_SERVER);
        if ( Vec3Normalize(axis[2]) == 0.0 )
            Scr_FxParamError(3u, "spawnFx called with (0 0 0) up direction", fxId);
        ++givenAxisCount;
    }
    if ( NumParam >= 3 )
    {
        Scr_GetVector(2u, axis[0], SCRIPTINSTANCE_SERVER);
        if ( Vec3Normalize(axis[0]) == 0.0 )
            Scr_FxParamError(2u, "spawnFx called with (0 0 0) forward direction", fxId);
        ++givenAxisCount;
    }
    Scr_GetVector(1u, pos, SCRIPTINSTANCE_SERVER);
    ent = G_Spawn();
    ent->s.eType = ET_FX;
    ent->r.svFlags |= 8u;
    ent->s.un1.scale = fxId;
    if ( ent->s.un1.scale != fxId
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    9474,
                    0,
                    "ent->s.un1.eventParm2 == fxId\n\t%i, %i",
                    ent->s.un1.scale,
                    fxId) )
    {
        __debugbreak();
    }
    pos[0] = (float)(int)pos[0];
    pos[1] = (float)(int)pos[1];
    pos[2] = (float)(int)pos[2];
    G_SetOrigin(ent, pos);
    Scr_SetFxAngles(givenAxisCount, axis, ent->s.lerp.apos.trBase);
    if ( ent->s.time2
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    9480,
                    1,
                    "%s\n\t(ent->s.time2) = %i",
                    "(ent->s.time2 == 0)",
                    ent->s.time2) )
    {
        __debugbreak();
    }
    SV_LinkEntity(ent);
    Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_CodePlayLoopedFX ~8260C510)
void Scr_CodePlayLoopedFX()
{
    int NumParam;
    float pos[3];
    int fxId;
    int repeat;
    gentity_s *ent;
    int givenAxisCount;
    float axis[3][3];
    float cullDist;

    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 3
        || (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 6 )
    {
        Scr_Error("Incorrect number of parameters", 0);
    }
    givenAxisCount = 0;
    cullDist = 0.0f;
    fxId = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    NumParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( NumParam != 4 )
    {
        if ( NumParam != 5 )
        {
            if ( NumParam != 6 )
                goto get_pos;
            ++givenAxisCount;
            Scr_GetVector(5u, axis[2], SCRIPTINSTANCE_SERVER);
            if ( Vec3Normalize(axis[2]) == 0.0 )
                Scr_FxParamError(5u, "playLoopedFx called with (0 0 0) up direction", fxId);
        }
        Scr_GetVector(4u, axis[0], SCRIPTINSTANCE_SERVER);
        if ( Vec3Normalize(axis[0]) == 0.0 )
            Scr_FxParamError(4u, "playLoopedFx called with (0 0 0) forward direction", fxId);
        ++givenAxisCount;
    }
    cullDist = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
get_pos:
    Scr_GetVector(2u, pos, SCRIPTINSTANCE_SERVER);
    repeat = (int)floorf(Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER) * 1000.0f + 0.5f);
    if ( repeat <= 0 )
        Scr_FxParamError(1u, "playLoopedFx called with repeat < 0.001 seconds", fxId);
    ent = G_Spawn();
    ent->s.eType = ET_LOOP_FX;
    ent->s.un1.eventParm2 = (unsigned __int8)fxId;
    if ( ent->s.un1.eventParm2 != (unsigned __int8)fxId
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_main.cpp",
                    0x39C8u,
                    0,
                    "%s",
                    "ent->s.un1.eventParm2 == fxId") )
    {
        __debugbreak();
    }
    G_SetOrigin(ent, pos);
    Scr_SetFxAngles(givenAxisCount, axis, ent->s.lerp.apos.trBase);
    ent->s.lerp.u.turret.gunAngles[0] = cullDist;
    ent->s.lerp.u.loopFx.period = repeat;
    SV_LinkEntity(ent);
    Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_CodeSpawnFX ~8260C7A0)
void Scr_CodeSpawnFX()
{
    int NumParam;
    float pos[3];
    int fxId;
    gentity_s *ent;
    int givenAxisCount;
    float axis[3][3];

    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 2
        || (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 4 )
    {
        Scr_Error("Incorrect number of parameters", 0);
    }
    givenAxisCount = 0;
    fxId = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    NumParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( NumParam != 3 )
    {
        if ( NumParam != 4 )
            goto get_pos;
        Scr_GetVector(3u, axis[2], SCRIPTINSTANCE_SERVER);
        if ( Vec3Normalize(axis[2]) == 0.0 )
            Scr_FxParamError(3u, "spawnFx called with (0 0 0) up direction", fxId);
        ++givenAxisCount;
    }
    Scr_GetVector(2u, axis[0], SCRIPTINSTANCE_SERVER);
    if ( Vec3Normalize(axis[0]) == 0.0 )
        Scr_FxParamError(2u, "spawnFx called with (0 0 0) forward direction", fxId);
    ++givenAxisCount;
get_pos:
    Scr_GetVector(1u, pos, SCRIPTINSTANCE_SERVER);
    ent = G_Spawn();
    ent->s.eType = ET_FX;
    ent->s.un1.eventParm2 = (unsigned __int8)fxId;
    if ( ent->s.un1.eventParm2 != (unsigned __int8)fxId
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_main.cpp",
                    0x3A03u,
                    0,
                    "ent->s.un1.eventParm2 == fxId\n\t%i, %i",
                    ent->s.un1.eventParm2,
                    fxId) )
    {
        __debugbreak();
    }
    G_SetOrigin(ent, pos);
    Scr_SetFxAngles(givenAxisCount, axis, ent->s.lerp.apos.trBase);
    if ( ent->s.time2
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_main.cpp",
                    0x3A06u,
                    1,
                    "%s\n\t(ent->s.time2) = %i",
                    "(ent->s.time2 == 0)",
                    ent->s.time2) )
    {
        __debugbreak();
    }
    SV_LinkEntity(ent);
    Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_ClientExploder_Internal ~8260CBB0)
void __cdecl Scr_ClientExploder_Internal(int exploderId)
{
    const char *fmtMsg;

    fmtMsg = va("%c %i", '@', exploderId);
    SV_GameSendServerCommand(-1, SV_CMD_CAN_IGNORE, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_ActivateClientExploder ~8260CB78)
void Scr_ActivateClientExploder()
{
    Scr_ClientExploder_Internal(Scr_GetInt(0, SCRIPTINSTANCE_SERVER));
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_DeactivateClientExploder ~8260CBF8)
void Scr_DeactivateClientExploder()
{
    Scr_ClientExploder_Internal(-Scr_GetInt(0, SCRIPTINSTANCE_SERVER));
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_ClearAllCorpses ~82611460)
void Scr_ClearAllCorpses()
{
    G_RemoveAllActorCorpses();
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_TriggerFX)
void Scr_TriggerFX()
{
    float tempValue1;
    gentity_s *ent;

    if (!Scr_GetNumParam(SCRIPTINSTANCE_SERVER) || (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 2)
        Scr_Error("Incorrect number of parameters", 0);
    ent = Scr_GetEntity( 0);
    if (!ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 9496, 0, "%s", "ent"))
        __debugbreak();
    if (ent->s.eType != 8)
        Scr_ParamError(0, "entity wasn't created with 'newFx'", SCRIPTINSTANCE_SERVER);
    if (Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 2)
    {
        tempValue1 = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER) * 1000.0;
        ent->s.time2 = (int)(tempValue1 + 9.313225746154785e-10);
    }
    else
    {
        ent->s.time2 = level.time;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_SpawnActor)
void __cdecl ScrCmd_SpawnActor(scr_entref_t entref)
{
    char *slStr;
    const char *fmtMsg;
    double tempValue3;
    double tempValue4;
    double tempValue5;
    const char *slStr2;
    gentity_s *guy;
    int noEnemyInfo;
    VariableUnion targetname;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->s.eType != 18 )
    {
        if ( ent->targetname )
            slStr2 = SL_ConvertToString(ent->targetname, SCRIPTINSTANCE_SERVER);
        else
            slStr2 = "<unnamed>";
        tempValue5 = ent->r.currentOrigin[2];
        tempValue4 = ent->r.currentOrigin[1];
        tempValue3 = ent->r.currentOrigin[0];
        slStr = SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER);
        fmtMsg = va(
                     "dospawn can only be called on actor spawners\n"
                     "attempted to call dospawn on entity with name '%s' of type '%s' at (%.0f %.0f %.0f)\n",
                     slStr2,
                     slStr,
                     tempValue3,
                     tempValue4,
                     tempValue5);
        Scr_Error(fmtMsg, 0);
    }
    noEnemyInfo = 0;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        noEnemyInfo = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    targetname.intValue = 0;
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) >= 2 )
        targetname.intValue = Scr_GetConstString(1u, SCRIPTINSTANCE_SERVER);
    TrackSpawn(ent->targetname, ent->r.currentOrigin);
    guy = SpawnActor(ent, targetname.stringValue, FORCE_SPAWN, noEnemyInfo == 0);
    if ( guy )
    {
        ent->item[0].clipAmmoCount = level.time;
        Scr_AddEntity(guy, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_CodeSpawn ~6121061)
void __cdecl ScrCmd_CodeSpawnerSpawn(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *slStr;
    const char *slStr2;
    double tempValue3;
    double tempValue4;
    double tempValue5;
    gentity_s *guy;
    int noEnemyInfo;
    VariableUnion targetname;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->s.eType != ET_ACTOR_SPAWNER )
    {
        if ( ent->targetname )
            slStr2 = SL_ConvertToString(ent->targetname, SCRIPTINSTANCE_SERVER);
        else
            slStr2 = "<unnamed>";
        tempValue5 = ent->r.currentOrigin[2];
        tempValue4 = ent->r.currentOrigin[1];
        tempValue3 = ent->r.currentOrigin[0];
        slStr = SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER);
        fmtMsg = va(
                     "CodeSpawnerSpawn can only be called on actor spawners\n"
                     "attempted to call CodeSpawnerSpawn on entity with name '%s' of type '%s' at (%.0f %.0f %.0f)\n",
                     slStr2,
                     slStr,
                     tempValue3,
                     tempValue4,
                     tempValue5);
        Scr_Error(fmtMsg, 0);
    }
    if ( ent->item[0].clipAmmoCount < level.time )
    {
        noEnemyInfo = 0;
        if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
            noEnemyInfo = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        targetname.intValue = 0;
        if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) >= 2 )
            targetname.intValue = Scr_GetConstString(1u, SCRIPTINSTANCE_SERVER);
        TrackSpawn(ent->targetname, ent->r.currentOrigin);
        guy = SpawnActor(ent, targetname.stringValue, CHECK_SPAWN, noEnemyInfo == 0);
        if ( guy )
        {
            ent->item[0].clipAmmoCount = level.time;
            guy->parent.setEnt(ent);
            Scr_AddEntity(guy, SCRIPTINSTANCE_SERVER);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_CodeForceSpawn ~6121135)
void __cdecl ScrCmd_CodeSpawnerForceSpawn(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *slStr;
    const char *slStr2;
    double tempValue3;
    double tempValue4;
    double tempValue5;
    gentity_s *guy;
    int noEnemyInfo;
    VariableUnion targetname;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->s.eType != ET_ACTOR_SPAWNER )
    {
        if ( ent->targetname )
            slStr2 = SL_ConvertToString(ent->targetname, SCRIPTINSTANCE_SERVER);
        else
            slStr2 = "<unnamed>";
        tempValue5 = ent->r.currentOrigin[2];
        tempValue4 = ent->r.currentOrigin[1];
        tempValue3 = ent->r.currentOrigin[0];
        slStr = SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER);
        fmtMsg = va(
                     "CodeSpawnerForceSpawn can only be called on actor spawners\n"
                     "attempted to call CodeSpawnerForceSpawn on entity with name '%s' of type '%s' at (%.0f %.0f %.0f)\n",
                     slStr2,
                     slStr,
                     tempValue3,
                     tempValue4,
                     tempValue5);
        Scr_Error(fmtMsg, 0);
    }
    if ( ent->item[0].clipAmmoCount < level.time )
    {
        noEnemyInfo = 0;
        if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
            noEnemyInfo = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        targetname.intValue = 0;
        if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) >= 2 )
            targetname.intValue = Scr_GetConstString(1u, SCRIPTINSTANCE_SERVER);
        TrackSpawn(ent->targetname, ent->r.currentOrigin);
        guy = SpawnActor(ent, targetname.stringValue, FORCE_SPAWN, noEnemyInfo == 0);
        if ( guy )
        {
            ent->item[0].clipAmmoCount = level.time;
            guy->parent.setEnt(ent);
            Scr_AddEntity(guy, SCRIPTINSTANCE_SERVER);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_CreateDynEntAndLaunch)
void __cdecl GScr_CreateDynEntAndLaunch()
{
    char *String;
    gentity_s *dynEntEvent;
    float *trDelta;
    float *trBase;
    float pos[3];
    float force[3];
    float angles[3];
    int fxId;
    float hitpos[3];
    int modelIndex;
    gentity_s *tempent;

    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 5 )
        Scr_Error(
            "CreateDynEntAndLaunch called with invalid params. CreateDynEntAndLaunch( <model>, <pos>, <angles>, <hitpos>, <force>, <fx> )",
            0);
    fxId = 0;
    String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    modelIndex = G_ModelIndex(String);
    Scr_GetVector(1u, pos, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(2u, angles, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(3u, hitpos, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(4u, force, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 5 )
        fxId = Scr_GetInt(5u, SCRIPTINSTANCE_SERVER);
    dynEntEvent = G_TempEntity(pos, EV_CREATE_DYNENT);
    tempent = dynEntEvent;
    dynEntEvent->s.lerp.pos.trDelta[0] = hitpos[0];
    dynEntEvent->s.lerp.pos.trDelta[1] = hitpos[1];
    dynEntEvent->s.lerp.pos.trDelta[2] = hitpos[2];
    trBase = tempent->s.lerp.apos.trBase;
    tempent->s.lerp.apos.trBase[0] = angles[0];
    trBase[1] = angles[1];
    trBase[2] = angles[2];
    trDelta = tempent->s.lerp.apos.trDelta;
    tempent->s.lerp.apos.trDelta[0] = force[0];
    trDelta[1] = force[1];
    trDelta[2] = force[2];
    tempent->s.index.brushmodel = modelIndex;
    tempent->s.un1.scale = fxId;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PhysicsExplosionSphere)
void Scr_PhysicsExplosionSphere()
{
    float pos[3];
    gentity_s *ent;

    if ((unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 4
        || (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 6)
    {
        Scr_Error("Incorrect number of parameters", 0);
    }
    Scr_GetVector(0, pos, SCRIPTINSTANCE_SERVER);
    ent = G_TempEntity(pos, EV_PHYS_EXPLOSION_SPHERE);
    ent->s.eventParm = (unsigned __int16)Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    ent->s.lerp.u.turret.gunAngles[0] = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    ent->s.lerp.u.actor.team = 0.0f;
    ent->s.lerp.u.destructibleHit.modelState3 = 0.0f;
    if (ent->s.lerp.u.turret.gunAngles[0] < 0.0)
        Scr_ParamError(2u, "Radius is negative", SCRIPTINSTANCE_SERVER);
    if (ent->s.lerp.u.turret.gunAngles[0] > (float)ent->s.eventParm)
        Scr_Error("Inner radius is outside the outer radius", 0);
    ent->s.lerp.u.turret.gunAngles[1] = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
    if ((unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 4)
        ent->s.lerp.u.turret.heatVal = Scr_GetFloat(4u, SCRIPTINSTANCE_SERVER);
    if ((unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 5)
        ent->s.lerp.u.turret.gunAngles[2] = Scr_GetFloat(5u, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_CreateStreamerHint)
void Scr_CreateStreamerHint()
{
    float origin[3];
    gentity_s *ent;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2 )
        Scr_Error("Incorrect number of parameters", 0);
    Scr_GetVector(0, origin, SCRIPTINSTANCE_SERVER);
    ent = G_Spawn();
    G_SetOrigin(ent, origin);
    ent->s.eType = ET_STREAMER_HINT;
    ent->flags |= 0x1000u;
    ent->s.lerp.pos.trType = 0;
    ent->s.lerp.u.turret.gunAngles[0] = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    if ( ent->s.lerp.u.turret.gunAngles[0] < 0.0 )
        Scr_ParamError(1u, "streamer hint factor is negative", SCRIPTINSTANCE_SERVER);
    SV_LinkEntity(ent);
    Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PhysicsRadiusJolt)
void Scr_PhysicsRadiusJolt()
{
    float pos[3];
    gentity_s *ent;

    if (Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 4)
        Scr_Error("Incorrect number of parameters", 0);
    Scr_GetVector(0, pos, SCRIPTINSTANCE_SERVER);
    ent = G_TempEntity(pos, EV_PHYS_EXPLOSION_JOLT);
    ent->s.eventParm = (unsigned __int16)Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    ent->s.lerp.u.turret.gunAngles[0] = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    if (ent->s.lerp.u.turret.gunAngles[0] < 0.0)
        Scr_ParamError(2u, "Radius is negative", SCRIPTINSTANCE_SERVER);
    if (ent->s.lerp.u.turret.gunAngles[0] > (float)ent->s.eventParm)
        Scr_Error("Inner radius is outside the outer radius", 0);
    Scr_GetVector(3u, &ent->s.lerp.u.turret.gunAngles[1], SCRIPTINSTANCE_SERVER);
    if (0.0 == ent->s.lerp.u.turret.gunAngles[1]
        && ent->s.lerp.u.turret.gunAngles[2] == 0.0
        && ent->s.lerp.u.primaryLight.cosHalfFovOuter == 0.0)
    {
        ent->s.lerp.u.turret.gunAngles[1] = 1.1754944;// eN38;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PhysicsExplosionCylinder)
void Scr_PhysicsExplosionCylinder()
{
    float pos[3];
    gentity_s *ent;

    if (Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 4)
        Scr_Error("Incorrect number of parameters", 0);
    Scr_GetVector(0, pos, SCRIPTINSTANCE_SERVER);
    ent = G_TempEntity(pos, EV_PHYS_EXPLOSION_CYLINDER);
    ent->s.eventParm = (unsigned __int16)Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    ent->s.lerp.u.turret.gunAngles[0] = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    if (ent->s.lerp.u.turret.gunAngles[0] < 0.0)
        Scr_ParamError(2u, "Radius is negative", SCRIPTINSTANCE_SERVER);
    if (ent->s.lerp.u.turret.gunAngles[0] > (float)ent->s.eventParm)
        Scr_Error("Inner radius is outside the outer radius", 0);
    ent->s.lerp.u.turret.gunAngles[1] = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetExponentialFog)
void Scr_SetExponentialFog()
{
    float green;
    float startDist;
    float blue;
    float red;
    float density;
    float time;
    float halfwayDist;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 6 )
        Scr_Error(
            "Incorrect number of parameters\n"
            "USAGE: setExpFog(<startDist>, <halfwayDist>, <red>, <green>, <blue>, <transition time>)\n",
            0);
    startDist = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    if ( startDist < 0.0 )
        Scr_Error("setExpFog: startDist must be greater or equal to 0", 0);
    halfwayDist = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    if ( halfwayDist <= 0.0 )
        Scr_Error("setExpFog: halfwayDist must be greater than 0", 0);
    density = 1.0 / halfwayDist;
    red = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    green = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
    blue = Scr_GetFloat(4u, SCRIPTINSTANCE_SERVER);
    time = Scr_GetFloat(5u, SCRIPTINSTANCE_SERVER);
    Dvar_SetColor((dvar_s *)g_fogColorReadOnly, red, green, blue, 1.0);
    Dvar_SetFloat((dvar_s *)g_fogStartDistReadOnly, startDist);
    Dvar_SetFloat((dvar_s *)g_fogHalfDistReadOnly, halfwayDist);
    if ( ((float)(1.0 / halfwayDist) <= 0.0 || density > 1.0)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    9792,
                    0,
                    "%s\n\t(density) = %g",
                    "(density > 0 && density <= 1)",
                    density) )
    {
        __debugbreak();
    }
    Scr_SetFog(
        -1,
        "setExpFog",
        startDist,
        density,
        0.0,
        0.0,
        red,
        green,
        blue,
        time,
        1.0,
        0.5,
        0.5,
        0.5,
        1.0,
        0.0,
        0.0,
        0.0,
        0.0,
        1.0);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetFog)
void __cdecl Scr_SetFog(
                int clientNum,
                const char *cmd,
                float start,
                float density,
                float heightDensity,
                float baseHeight,
                float r,
                float g,
                float b,
                float time,
                float colorScale,
                float sunColR,
                float sunColG,
                float sunColB,
                float sunDirX,
                float sunDirY,
                float sunDirZ,
                float sunStartAng,
                float sunEndAng,
                float maxFogOpacity)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    char *fmtMsg3;
    int i;
    gentity_s *ent;

    if ( start < 0.0 )
    {
        fmtMsg = va("%s: near distance must be >= 0", cmd);
        Scr_Error(fmtMsg, 0);
    }
    if ( time < 0.0 )
    {
        fmtMsg2 = va("%s: transition time must be >= 0 seconds", cmd);
        Scr_Error(fmtMsg2, 0);
    }
    if ( sv.state != SS_GAME )
        return;
    fmtMsg3 = va(
                    "%g %g %g %g %g %g %g %.0f %g %g %g %g %g %g %g %g %g %g",
                    start,
                    density,
                    heightDensity,
                    baseHeight,
                    r,
                    g,
                    b,
                    (float)(time * 1000.0),
                    colorScale,
                    sunColR,
                    sunColG,
                    sunColB,
                    sunDirX,
                    sunDirY,
                    sunDirZ,
                    sunStartAng,
                    sunEndAng,
                    maxFogOpacity);
    if ( clientNum < 0 )
    {
        for ( i = 0; i < com_maxclients->current.integer; ++i )
        {
            ent = &g_entities[i];
            if ( ent->client )
                G_setfog(i, fmtMsg3);
        }
    }
    else
    {
        G_setfog(clientNum, fmtMsg3);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetVolumetricFog / GScr_SetVolumetricFog)
static void Scr_SetVolumetricFogForClient(int clientNum)
{
    float tempValue0;
    float tempValue1;
    float sunDirY;
    float green;
    float startDist;
    float baseHeight;
    float blue;
    float sunStartAng;
    float red;
    float fogColorScale;
    float maxFogOpacity;
    float density;
    float time;
    float sunColorB;
    float halfwayDist;
    float sunDirZ;
    float halfwayHeight;
    float sunStopAng;
    float sunColorR;
    float sunColorG;
    float sunDirX;

    sunColorR = 0.5f;
    sunColorG = 0.5f;
    sunColorB = 0.5f;
    sunDirX = 1.0f;
    sunDirY = 0.0f;
    sunDirZ = 0.0f;
    sunStartAng = 0.0f;
    sunStopAng = 0.0f;
    maxFogOpacity = 1.0f;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 8 && Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 18 )
        Scr_Error(
            "Incorrect number of parameters\n"
            "USAGE: setVolFog(<startDist>, <halfwayDist>, <halfwayHeight>, <baseHeight>, <red>, <green>, <blue>, <transition ti"
            "me>)\n"
            "OR:        SetVolFog(<startDist>, <halfwayDist>, <halfwayHeight>, <baseHeight>, <red>, <green>, <blue>, <fogColorScale"
            ">, <sunFogRed>, <sunFogGreen>, <sunFogBlue>, <sunFogDirX>, <sunFogDirY>, <sunFogDirZ>, <sunFogStartAng>, <sunFogEn"
            "dAng>, <transition time>)\n",
            0);
    startDist = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    if ( startDist < 0.0 )
        Scr_Error("setExpFog: startDist must be greater or equal to 0", 0);
    halfwayDist = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    if ( halfwayDist <= 0.0 )
        Scr_Error("setVolFog: halfwayDist must be greater than 0", 0);
    halfwayHeight = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    if ( halfwayHeight < 0.0 )
        Scr_Error("setVolFog: halfwayHeight must be greater or equal to 0", 0);
    baseHeight = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
    density = 1.0 / halfwayDist;
    if ( halfwayHeight < 1.0 )
        tempValue1 = 0.0f;
    else
        tempValue1 = 1.0 / halfwayHeight;
    red = Scr_GetFloat(4u, SCRIPTINSTANCE_SERVER);
    green = Scr_GetFloat(5u, SCRIPTINSTANCE_SERVER);
    blue = Scr_GetFloat(6u, SCRIPTINSTANCE_SERVER);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 18 )
    {
        fogColorScale = Scr_GetFloat(7u, SCRIPTINSTANCE_SERVER);
        sunColorR = Scr_GetFloat(8u, SCRIPTINSTANCE_SERVER);
        sunColorG = Scr_GetFloat(9u, SCRIPTINSTANCE_SERVER);
        sunColorB = Scr_GetFloat(0xAu, SCRIPTINSTANCE_SERVER);
        sunDirX = Scr_GetFloat(0xBu, SCRIPTINSTANCE_SERVER);
        sunDirY = Scr_GetFloat(0xCu, SCRIPTINSTANCE_SERVER);
        sunDirZ = Scr_GetFloat(0xDu, SCRIPTINSTANCE_SERVER);
        sunStartAng = Scr_GetFloat(0xEu, SCRIPTINSTANCE_SERVER);
        sunStopAng = Scr_GetFloat(0xFu, SCRIPTINSTANCE_SERVER);
        time = Scr_GetFloat(0x10u, SCRIPTINSTANCE_SERVER);
        maxFogOpacity = Scr_GetFloat(0x11u, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Com_Printf(1, "setVolFog: Old syntax used. Please update script.\n");
        if ( green <= red )
            tempValue0 = red;
        else
            tempValue0 = green;
        fogColorScale = tempValue0;
        if ( blue > tempValue0 )
            fogColorScale = blue;
        red = red * (float)(1.0 / fogColorScale);
        green = green * (float)(1.0 / fogColorScale);
        blue = blue * (float)(1.0 / fogColorScale);
        time = Scr_GetFloat(7u, SCRIPTINSTANCE_SERVER);
    }
    if ( (density <= 0.0 || density > 1.0)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_main.cpp",
                    0x3EF3u,
                    0,
                    "%s\n\t(density) = %g",
                    "(density > 0 && density <= 1)",
                    density) )
    {
        __debugbreak();
    }
    if ( (tempValue1 < 0.0 || tempValue1 > 1.0)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_main.cpp",
                    0x3EF4u,
                    0,
                    "%s\n\t(heightDensity) = %g",
                    "(heightDensity >= 0 && heightDensity <= 1)",
                    tempValue1) )
    {
        __debugbreak();
    }
    Scr_SetFog(
        clientNum,
        "setVolFog",
        startDist,
        density,
        tempValue1,
        baseHeight,
        red,
        green,
        blue,
        time,
        fogColorScale,
        sunColorR,
        sunColorG,
        sunColorB,
        sunDirX,
        sunDirY,
        sunDirZ,
        sunStartAng,
        sunStopAng,
        maxFogOpacity);
}

void Scr_SetVolumetricFog()
{
    Scr_SetVolumetricFogForClient(-1);
}

void __cdecl GScr_SetVolumetricFog(scr_entref_t entref)
{
    gentity_s *ent;
    int clientNum;

    clientNum = -1;
    ent = GetEntity(entref);
    if ( ent && ent->r.inuse && ent->client )
        clientNum = ent->s.number;
    else
        Scr_Error("setvolumetricfog() called on an invalid client entity.\n", 0);
    Scr_SetVolumetricFogForClient(clientNum);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetCullDist)
void Scr_SetCullDist()
{
    double Float;
    char *fmtMsg;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("Incorrect number of parameters\n", 0);
    Float = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    fmtMsg = va("%g", Float);
    SV_SetConfigstring(7, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_VisionSetNaked / GScr_VisionSetNaked)
static void Scr_SetVisionSetNakedForClient(int clientNum)
{
    char configString[1024];
    char *fmtMsg;
    char *keyMsg;
    int NumParam;
    float tempValue2;
    int duration;
    char *name;
    int i;

    duration = 1000;
    NumParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( NumParam != 1 && NumParam != 2 )
        Scr_Error("USAGE: VisionSetNaked( <visionset name>, <transition time> )\n", 0);
    if ( NumParam == 2 )
    {
        tempValue2 = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER) * 1000.0;
        duration = (int)(tempValue2 + 9.313225746154785e-10);
    }
    name = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    SV_GetConfigstring(1554, configString, 1024);
    fmtMsg = va("\"%s\" %i", name, duration);
    if ( clientNum < 0 )
    {
        for ( i = 0; i < com_maxclients->current.integer; ++i )
        {
            keyMsg = va("%i", i);
            Info_SetValueForKey(configString, keyMsg, fmtMsg);
        }
    }
    else
    {
        keyMsg = va("%i", clientNum);
        Info_SetValueForKey(configString, keyMsg, fmtMsg);
    }
    SV_SetConfigstring(1554, configString);
}

void Scr_VisionSetNaked()
{
    Scr_SetVisionSetNakedForClient(-1);
}

void __cdecl GScr_VisionSetNaked(scr_entref_t entref)
{
    gentity_s *ent;
    int clientNum;

    clientNum = -1;
    ent = GetEntity(entref);
    if ( ent && ent->r.inuse && ent->client )
        clientNum = ent->s.number;
    else
        Scr_Error("visionsetnaked() called on an invalid client entity.\n", 0);
    Scr_SetVisionSetNakedForClient(clientNum);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_VisionSetNight)
void Scr_VisionSetNight()
{
    char *fmtMsg;
    int NumParam;
    float tempValue2;
    int duration;
    char *name;

    duration = 1000;
    NumParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( NumParam != 1 && NumParam != 2 )
        Scr_Error("USAGE: VisionSetNight( <visionset name>, <transition time> )\n", 0);
    if ( NumParam == 2 )
    {
        tempValue2 = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER) * 1000.0;
        duration = (int)(tempValue2 + 9.313225746154785e-10);
    }
    name = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    fmtMsg = va("\"%s\" %i", name, duration);
    SV_SetConfigstring(1551, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_TableLookupRowNum)
void Scr_TableLookupRowNum()
{
    char *filename;
    char *stringValue;
    int returnValueRow;
    const StringTable *tablePtr;
    int comparisonColumn;

    if ( useFastFile->current.enabled )
    {
        if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 2 )
            Scr_Error("USAGE: tableLookupRowNum( filename, searchColumnNum, searchValue )\n", 0);
        filename = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        StringTable_GetAsset(filename, (XAssetHeader *)&tablePtr);
        comparisonColumn = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
        stringValue = Scr_GetString(2u, SCRIPTINSTANCE_SERVER);
        returnValueRow = StringTable_LookupRowNumForValue(tablePtr, comparisonColumn, stringValue);
        Scr_AddInt(returnValueRow, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Com_Printf(16, "You cannot do table lookups without fastfiles.\n");
        Scr_AddString((char *)"", SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_TableLookupColumnForRow)
void Scr_TableLookupColumnForRow()
{
    char *filename;
    char *returnValue;
    const StringTable *tablePtr;
    int row;
    int column;

    if ( useFastFile->current.enabled )
    {
        if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 2 )
            Scr_Error("USAGE: tableLookupColumnForRow( filename, row, column )\n", 0);
        filename = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        StringTable_GetAsset(filename, (XAssetHeader *)&tablePtr);
        row = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
        column = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
        returnValue = (char *)StringTable_GetColumnValueForRow(tablePtr, row, column);
        Scr_AddString(returnValue, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Com_Printf(16, "You cannot do table lookups without fastfiles.\n");
        Scr_AddString((char *)"", SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_TableLookup)
void Scr_TableLookup()
{
    char *stringValue;
    char *filename;
    char *returnValue;
    const StringTable *tablePtr;
    int returnValueColumn;
    int comparisonColumn;

    if ( useFastFile->current.enabled )
    {
        if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 3 )
            Scr_Error("USAGE: tableLookup( filename, searchColumnNum, searchValue, returnValueColumnNum )\n", 0);
        filename = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        StringTable_GetAsset(filename, (XAssetHeader *)&tablePtr);
        comparisonColumn = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
        stringValue = Scr_GetString(2u, SCRIPTINSTANCE_SERVER);
        returnValueColumn = Scr_GetInt(3u, SCRIPTINSTANCE_SERVER);
        returnValue = (char *)StringTable_Lookup(tablePtr, comparisonColumn, stringValue, returnValueColumn);
        Scr_AddString(returnValue, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Com_Printf(16, "You cannot do table lookups without fastfiles.\n");
        Scr_AddString((char *)"", SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_TableLookupIString)
void Scr_TableLookupIString()
{
    char *stringValue;
    char *filename;
    char *returnValue;
    const StringTable *tablePtr;
    int returnValueColumn;
    int comparisonColumn;

    if ( useFastFile->current.enabled )
    {
        if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 3 )
            Scr_Error("USAGE: tableLookupIString( filename, searchColumnNum, searchValue, returnValueColumnNum )\n", 0);
        filename = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        StringTable_GetAsset(filename, (XAssetHeader *)&tablePtr);
        comparisonColumn = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
        stringValue = Scr_GetString(2u, SCRIPTINSTANCE_SERVER);
        returnValueColumn = Scr_GetInt(3u, SCRIPTINSTANCE_SERVER);
        returnValue = (char *)StringTable_Lookup(tablePtr, comparisonColumn, stringValue, returnValueColumn);
        Scr_AddIString(returnValue, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Com_Printf(16, "You cannot do table lookups without fastfiles.\n");
        Scr_AddIString((char *)"", SCRIPTINSTANCE_SERVER);
    }
}

float locs[255][3];
// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetReflectionLocs)
void __cdecl Scr_GetReflectionLocs()
{
    unsigned int i;
    unsigned int count;

    count = R_GetDebugReflectionProbeLocs(locs, 0xFFu);
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for ( i = 0; i < count; ++i )
    {
        Scr_AddVector(locs[i], SCRIPTINSTANCE_SERVER);
        Scr_AddArray(SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetReflectionOrigin)
void __cdecl Scr_GetReflectionOrigin()
{
    float probePos[3];
    float queryPos[3];
    unsigned int index;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Com_Error(ERR_DROP, "Invalid origin");
    Scr_GetVector(0, queryPos, SCRIPTINSTANCE_SERVER);
    index = R_CalcReflectionProbeIndex(queryPos);
    R_GetReflectionProbePosition(index, probePos);
    Scr_AddVector(probePos, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsPlayer)
void GScr_IsPlayer()
{
    if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) == 1
        && Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) == 19
        && Scr_GetEntity(0)->client )
    {
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsPlayerNumber)
void GScr_IsPlayerNumber()
{
    if ( Scr_GetInt(0, SCRIPTINSTANCE_SERVER) < 0x20 )
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetWinningPlayer)
void GScr_SetWinningPlayer()
{
    char *tempValue0;
    char *pszWinner;
    int iWinner;
    char buffer[1024];
    gentity_s *pEnt;

    pEnt = Scr_GetEntity(0);
    iWinner = pEnt->s.number + 1;
    SV_GetConfigstring(0x15u, buffer, 1024);
    pszWinner = va("%i", iWinner);
    tempValue0 = Info_ValueForKey(buffer, "winner");
    if ( I_stricmp(tempValue0, pszWinner) )
    {
        Info_SetValueForKey(buffer, "winner", pszWinner);
        SV_SetConfigstring(21, buffer);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetWinningTeam)
void GScr_SetWinningTeam()
{
    char *slStr;
    const char *fmtMsg;
    char *tempValue2;
    char *pszWinner;
    unsigned __int16 team;
    int iWinner;
    char buffer[1028];

    team = (unsigned __int16)Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    if ( team == scr_const.allies )
    {
        iWinner = -2;
    }
    else if ( team == scr_const.axis )
    {
        iWinner = -1;
    }
    else
    {
        if ( team != scr_const.none )
        {
            slStr = SL_ConvertToString(team, SCRIPTINSTANCE_SERVER);
            fmtMsg = va("Illegal team string '%s'. Must be allies, axis, or none.", slStr);
            Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
            return;
        }
        iWinner = 0;
    }
    SV_GetConfigstring(0x15u, buffer, 1024);
    pszWinner = va("%i", iWinner);
    tempValue2 = Info_ValueForKey(buffer, "winner");
    if ( I_stricmp(tempValue2, pszWinner) )
    {
        Info_SetValueForKey(buffer, "winner", pszWinner);
        SV_SetConfigstring(21, buffer);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_Announcement)
void GScr_Announcement()
{
    int NumParam;
    int tempValue1;
    VariableUnion varUnion;
    const char *fmtMsg;
    char string[1028];

    NumParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    Scr_ConstructMessageString(0, NumParam - 2, "Announcement", string, 0x400u);
    tempValue1 = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    varUnion.intValue = Scr_GetInt(tempValue1 - 1, SCRIPTINSTANCE_SERVER);
    fmtMsg = va("%c \"%s\" %i", 99, string, varUnion.intValue);
    SV_GameSendServerCommand(-1, SV_CMD_CAN_IGNORE, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ClientAnnouncement)
void GScr_ClientAnnouncement()
{
    int NumParam;
    int tempValue1;
    VariableUnion varUnion;
    const char *fmtMsg;
    char string[1024];
    gentity_s *pEnt;

    pEnt = Scr_GetEntity(0);
    NumParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    Scr_ConstructMessageString(1, NumParam - 2, "Announcement", string, 0x400u);
    tempValue1 = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    varUnion.intValue = Scr_GetInt(tempValue1 - 1, SCRIPTINSTANCE_SERVER);
    fmtMsg = va("%c \"%s\" %i", 99, string, varUnion.intValue);
    SV_GameSendServerCommand(pEnt->s.number, SV_CMD_CAN_IGNORE, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetTeamScore)
void GScr_GetTeamScore()
{
    char *slStr;
    const char *fmtMsg;
    unsigned __int16 team;

    team = (unsigned __int16)Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    if ( team != scr_const.allies && team != scr_const.axis )
    {
        slStr = SL_ConvertToString(team, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("Illegal team string '%s'. Must be allies, or axis.", slStr);
        Scr_Error(fmtMsg, 0);
    }
    if ( team == scr_const.allies )
        Scr_AddInt(level_tail.teamScores[2], SCRIPTINSTANCE_SERVER);
    else
        Scr_AddInt(level_tail.teamScores[1], SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetTeamScore)
void GScr_SetTeamScore()
{
    char *slStr;
    const char *fmtMsg;
    const char *fmtMsg2;
    unsigned __int16 team;
    int teamScore;

    team = (unsigned __int16)Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    if ( team != scr_const.allies && team != scr_const.axis )
    {
        slStr = SL_ConvertToString(team, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("Illegal team string '%s'. Must be allies, or axis.", slStr);
        Scr_Error(fmtMsg, 0);
    }
    teamScore = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    if ( team == scr_const.allies )
    {
        level_tail.teamScores[2] = teamScore;
        fmtMsg2 = va("%c %i", 72, teamScore);
    }
    else
    {
        level_tail.teamScores[1] = teamScore;
        fmtMsg2 = va("%c %i", 71, teamScore);
    }
    SV_GameSendServerCommand(-1, SV_CMD_CAN_IGNORE, fmtMsg2);
    level.bUpdateScoresForIntermission = 1;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetClientNameMode)
void GScr_SetClientNameMode()
{
    unsigned __int16 mode;

    mode = (unsigned __int16)Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    if ( mode == scr_const.auto_change )
    {
        level.manualNameChange = 0;
    }
    else if ( mode == scr_const.manual_change )
    {
        level.manualNameChange = 1;
    }
    else
    {
        Scr_Error("Unknown mode", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_UpdateClientNames)
void GScr_UpdateClientNames()
{
    gclient_s *clients;
    char oldname[32];
    int i;

    if (!level.manualNameChange)
        Scr_Error("Only works in [manual_change] mode", 0);
    i = 0;
    clients = level.clients;
    while (i < level.maxclients)
    {
        if (clients->sess.connected == CON_CONNECTED)
        {
            if (strcmp(clients->sess.cs.name, clients->sess.newnetname))
            {
                I_strncpyz(oldname, clients->sess.cs.name, 32);
                I_strncpyz(clients->sess.cs.name, clients->sess.newnetname, 32);
                ClientUserinfoChanged(i);
            }
        }
        ++i;
        ++clients;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetTeamPlayersAlive)
void GScr_GetTeamPlayersAlive()
{
    char *slStr;
    const char *fmtMsg;
    int iLivePlayers;
    unsigned __int16 team;
    int iTeamNum;
    gentity_s *pEnt;
    int i;

    team = (unsigned __int16)Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    if ( team != scr_const.allies && team != scr_const.axis )
    {
        slStr = SL_ConvertToString(team, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("Illegal team string '%s'. Must be allies, or axis.", slStr);
        Scr_Error(fmtMsg, 0);
    }
    if ( team == scr_const.allies )
        iTeamNum = 2;
    else
        iTeamNum = 1;
    iLivePlayers = 0;
    for ( i = 0; i < com_maxclients->current.integer; ++i )
    {
        pEnt = &g_entities[i];
        if ( pEnt->r.inuse && pEnt->client->sess.cs.team == iTeamNum && pEnt->health > 0 )
            ++iLivePlayers;
    }
    Scr_AddInt(iLivePlayers, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetDroppedWeapons)
void GScr_GetDroppedWeapons()
{
    gentity_s *ent;
    int i;

    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for (i = 0; i < 32; ++i)
    {
        if (level.droppedWeaponCue[i].isDefined())
        {
            //ent = EntHandle::ent(&level.droppedWeaponCue[i]);
            ent = level.droppedWeaponCue[i].ent();
            Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
            Scr_AddArray(SCRIPTINSTANCE_SERVER);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetNumParts)
void __cdecl GScr_GetNumParts()
{
    char *String;
    int tempValue1;
    XModel *model;

    String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    model = SV_XModelGet(String);
    tempValue1 = XModelNumBones(model);
    Scr_AddInt(tempValue1, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetPartName)
void __cdecl GScr_GetPartName()
{
    char *String;
    const char *fmtMsg;
    XModel *model;
    unsigned __int16 name;
    unsigned int index;
    unsigned int numbones;

    String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    model = SV_XModelGet(String);
    index = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    numbones = XModelNumBones(model);
    if ( index >= numbones )
    {
        fmtMsg = va("index out of range (0 - %d)", numbones - 1);
        Scr_ParamError(1u, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    name = XModelBoneNames(model)[index];
    if ( !name )
        Scr_ParamError(0, "bad model", SCRIPTINSTANCE_SERVER);
    Scr_AddConstString(name, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_Earthquake)
void GScr_Earthquake()
{
    float tempValue1;
    int clientNum;
    float source[3];
    gentity_s *tent;
    int duration;
    gentity_s *target;
    float radius;
    float scale;

    scale = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    tempValue1 = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER) * 1000.0;
    duration = (int)(tempValue1 + 9.313225746154785e-10);
    Scr_GetVector(2u, source, SCRIPTINSTANCE_SERVER);
    radius = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
    if (scale <= 0.0)
        Scr_ParamError(0, "Scale must be greater than 0", SCRIPTINSTANCE_SERVER);
    if (duration <= 0)
        Scr_ParamError(1u, "duration must be greater than 0", SCRIPTINSTANCE_SERVER);
    if (radius <= 0.0)
        Scr_ParamError(3u, "Radius must be greater than 0", SCRIPTINSTANCE_SERVER);
    if ((unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 5)
        target = 0;
    else
        target = Scr_GetEntity(4u);
    tent = G_TempEntity(source, EV_EARTHQUAKE);
    tent->s.lerp.u.turret.gunAngles[0] = scale;
    tent->s.lerp.u.actor.team = duration;
    tent->s.lerp.u.turret.gunAngles[1] = radius;
    if (target)
    {
        if (target->client)
        {
            tent->r.clientMask[0] = -1;
            clientNum = target->client->ps.clientNum;
            tent->r.clientMask[clientNum >> 5] &= ~(1 << (clientNum & 0x1F));
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ShellShock)
void __cdecl GScr_ShellShock(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    float tempValue3;
    int duration;
    char *shock;
    gentity_s *ent;
    char s[1024];
    int id;

    PROF_SCOPED("GScr_ShellShock");

    SV_CheckThread();
    ent = GetPlayerEntity(entref);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2 )
        Scr_Error("USAGE: <player> shellshock(<shellshockname>, <duration>)\n", 0);
    shock = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    for ( id = 1; ; ++id )
    {
        if ( id >= 16 )
        {
            fmtMsg2 = va("shellshock '%s' was not precached\n", shock);
            Scr_Error(fmtMsg2, 0);
            return;
        }
        SV_GetConfigstring(id + 2532, s, 1024);
        if ( !I_stricmp(s, shock) )
            break;
    }
    tempValue3 = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER) * 1000.0;
    duration = (int)(tempValue3 + 9.313225746154785e-10);
    if ( (unsigned int)duration > 0xEA60 )
    {
        fmtMsg = va("duration %g should be >= 0 and <= 60", (float)((float)duration * 0.001));
        Scr_ParamError(1u, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    ent->client->ps.shellshockIndex = id;
    ent->client->ps.shellshockTime = level.time;
    ent->client->ps.shellshockDuration = duration;
    if ( (ent->client->ps.perks[1] & 0x2000) != 0 )
        ent->client->ps.shellshockDuration = (int)(float)((float)ent->client->ps.shellshockDuration
                                                                                                        * perk_shellShockReduction->current.value);
    if ( ent->health > 0 )
    {
        ent->client->ps.pm_flags |= 0x10000u;
        bgs = &level_bgs;
        iassert(bgs == &level_bgs);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_StopShellShock)
void __cdecl GScr_StopShellShock(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetPlayerEntity(entref);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        Scr_Error("USAGE: <player> stopshellshock()\n", 0);
    ent->client->ps.shellshockIndex = 0;
    ent->client->ps.shellshockTime = 0;
    ent->client->ps.shellshockDuration = 0;
    ent->client->ps.pm_flags &= ~0x10000u;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetTagOrigin)
void __cdecl GScr_GetTagOrigin(scr_entref_t entref)
{
    VariableUnion tagName;
    gentity_s *ent;

    ent = GetEntity(entref);
    tagName.intValue = Scr_GetConstLowercaseString(0, SCRIPTINSTANCE_SERVER);
    GScr_UpdateTagInternal(ent, tagName.stringValue, &level.cachedTagMat, 1);
    Scr_AddVector(level.cachedTagMat.tagMat[3], SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c:9272
int __cdecl GScr_UpdateTagInternal(
                gentity_s *ent,
                unsigned int tagName,
                cached_tag_mat_t *cachedTag,
                int showScriptError)
{
    char *slStr;
    const char *fmtMsg;
    unsigned int modelNameStr;
    char *slStr2;
    const char *fmtMsg2;
    char *slStr3;

    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 10708, 0, "%s", "ent") )
        __debugbreak();
    if ( ent->s.number == cachedTag->entnum && level.time == cachedTag->time && tagName == cachedTag->name )
        return 1;
    if ( !SV_DObjExists(ent) )
    {
        if ( showScriptError )
        {
            slStr = SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER);
            fmtMsg = va("entity has no model defined (classname '%s')", slStr);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
        return 0;
    }
    if ( G_DObjGetWorldTagMatrix(ent, tagName, cachedTag->tagMat) )
    {
        cachedTag->entnum = ent->s.number;
        cachedTag->time = level.time;
        Scr_SetString(&cachedTag->name, tagName, SCRIPTINSTANCE_SERVER);
        return 1;
    }
    if ( showScriptError )
    {
        SV_DObjDumpInfo(ent);
        modelNameStr = G_ModelName(ent->model);
        slStr3 = SL_ConvertToString(modelNameStr, SCRIPTINSTANCE_SERVER);
        slStr2 = SL_ConvertToString(tagName, SCRIPTINSTANCE_SERVER);
        fmtMsg2 = va("tag '%s' does not exist in model '%s' (or any attached submodels)", slStr2, slStr3);
        Scr_ParamError(0, fmtMsg2, SCRIPTINSTANCE_SERVER);
    }
    return 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetTagAngles)
void __cdecl GScr_GetTagAngles(scr_entref_t entref)
{
    VariableUnion tagName;
    float angles[3];
    gentity_s *ent;

    ent = GetEntity(entref);
    tagName.intValue = Scr_GetConstLowercaseString(0, SCRIPTINSTANCE_SERVER);
    GScr_UpdateTagInternal(ent, tagName.stringValue, &level.cachedTagMat, 1);
    AxisToAngles(level.cachedTagMat.tagMat, angles);
    Scr_AddVector(angles, SCRIPTINSTANCE_SERVER);
}

static float GScr_GetRelativeYawToPoint(const gentity_s *ent, const float *from, const float *target)
{
    float delta[3];
    float angles[3];

    delta[0] = target[0] - from[0];
    delta[1] = target[1] - from[1];
    delta[2] = target[2] - from[2];
    vectoangles(delta, angles);
    return AngleNormalize180(ent->r.currentAngles[1] - angles[1]);
}

void __cdecl GScr_GetYawToOrigin(scr_entref_t entref)
{
    float target[3];
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_GetVector(0, target, SCRIPTINSTANCE_SERVER);
    Scr_AddFloat(GScr_GetRelativeYawToPoint(ent, ent->r.currentOrigin, target), SCRIPTINSTANCE_SERVER);
}

void __cdecl GScr_GetEyeYawToOrigin(scr_entref_t entref)
{
    char *slStr;
    const char *fmtMsg;
    float eye[3];
    float target[3];
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->sentient )
    {
        slStr = SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("getEyeYawToOrigin must be called on an AI or player, not on a '%s'", slStr);
        Scr_Error(fmtMsg, 0);
    }
    Scr_GetVector(0, target, SCRIPTINSTANCE_SERVER);
    Sentient_GetEyePosition(ent->sentient, eye);
    Scr_AddFloat(GScr_GetRelativeYawToPoint(ent, eye, target), SCRIPTINSTANCE_SERVER);
}

void __cdecl GScr_GetYawToTag(scr_entref_t entref)
{
    VariableUnion tagName;
    float target[3];
    gentity_s *ent;

    ent = GetEntity(entref);
    tagName.intValue = Scr_GetConstLowercaseString(0, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, target, SCRIPTINSTANCE_SERVER);
    GScr_UpdateTagInternal(ent, tagName.stringValue, &level.cachedTagMat, 1);
    Scr_AddFloat(GScr_GetRelativeYawToPoint(ent, level.cachedTagMat.tagMat[3], target), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetEntnum)
void __cdecl GScr_GetEntnum(scr_entref_t entref)
{
    Scr_AddInt(entref.entnum, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetDepthOfField)
void __cdecl GScr_SetDepthOfField(scr_entref_t entref)
{
    float dofNearBlur;
    float dofFarBlur;
    float dofNearStart;
    float dofFarStart;
    gentity_s *ent;
    float dofFarEnd;
    float dofNearEnd;

    ent = GetPlayerEntity(entref);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 6 )
        Scr_Error("Incorrect number of parameters\n", 0);
    dofNearStart = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    dofNearEnd = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    dofFarStart = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    dofFarEnd = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
    dofNearBlur = Scr_GetFloat(4u, SCRIPTINSTANCE_SERVER);
    dofFarBlur = Scr_GetFloat(5u, SCRIPTINSTANCE_SERVER);
    if ( dofNearStart < 0.0 )
        Scr_ParamError(0, "near start must be >= 0", SCRIPTINSTANCE_SERVER);
    if ( dofNearEnd < 0.0 )
        Scr_ParamError(1u, "near end must be >= 0", SCRIPTINSTANCE_SERVER);
    if ( dofFarStart < 0.0 )
        Scr_ParamError(2u, "far start must be >= 0", SCRIPTINSTANCE_SERVER);
    if ( dofFarEnd < 0.0 )
        Scr_ParamError(3u, "far end must be >= 0", SCRIPTINSTANCE_SERVER);
    if ( dofNearStart >= dofNearEnd )
    {
        dofNearStart = 0.0f;
        dofNearEnd = 0.0f;
    }
    if ( dofFarStart >= dofFarEnd || dofFarBlur == 0.0 )
    {
        dofFarStart = 0.0f;
        dofFarEnd = 0.0f;
    }
    else if ( dofNearEnd > dofFarStart )
    {
        Scr_ParamError(
            2u,
            "far start must be >= near end, or far depth of field should be disabled with far start >= far end or far blur == 0",
            SCRIPTINSTANCE_SERVER);
    }
    ent->client->ps.dofNearStart = dofNearStart;
    ent->client->ps.dofNearEnd = dofNearEnd;
    ent->client->ps.dofFarStart = dofFarStart;
    ent->client->ps.dofFarEnd = dofFarEnd;
    ent->client->ps.dofNearBlur = dofNearBlur;
    ent->client->ps.dofFarBlur = dofFarBlur;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetDepthOfField_NearStart)
void __cdecl GScr_GetDepthOfField_NearStart(scr_entref_t entref)
{
    gentity_s *ent = GetPlayerEntity(entref);
    Scr_AddFloat(ent->client->ps.dofNearStart, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetDepthOfField_NearEnd)
void __cdecl GScr_GetDepthOfField_NearEnd(scr_entref_t entref)
{
    gentity_s *ent = GetPlayerEntity(entref);
    Scr_AddFloat(ent->client->ps.dofNearEnd, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetDepthOfField_FarStart)
void __cdecl GScr_GetDepthOfField_FarStart(scr_entref_t entref)
{
    gentity_s *ent = GetPlayerEntity(entref);
    Scr_AddFloat(ent->client->ps.dofFarStart, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetDepthOfField_NearBlur)
void __cdecl GScr_GetDepthOfField_NearBlur(scr_entref_t entref)
{
    gentity_s *ent = GetPlayerEntity(entref);
    Scr_AddFloat(ent->client->ps.dofNearBlur, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetViewModelDepthOfField)
void __cdecl GScr_SetViewModelDepthOfField(scr_entref_t entref)
{
    gentity_s *ent;
    float dofEnd;
    float dofStart;

    ent = GetPlayerEntity(entref);
    dofStart = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    dofEnd = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    if ( dofStart < 0.0 )
        Scr_ParamError(0, "start must be >= 0", SCRIPTINSTANCE_SERVER);
    if ( dofEnd < 0.0 )
        Scr_ParamError(1u, "end must be >= 0", SCRIPTINSTANCE_SERVER);
    if ( dofStart >= dofEnd )
    {
        dofStart = 0.0f;
        dofEnd = 0.0f;
    }
    ent->client->ps.dofViewmodelStart = dofStart;
    ent->client->ps.dofViewmodelEnd = dofEnd;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetDepthOfField_FarEnd ~6148630)
void __cdecl GScr_GetDepthOfField_FarEnd(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetPlayerEntity(entref);
    Scr_AddFloat(ent->client->ps.dofFarEnd, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetDepthOfField_FarBlur ~6148712)
void __cdecl GScr_GetDepthOfField_FarBlur(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetPlayerEntity(entref);
    Scr_AddFloat(ent->client->ps.dofFarBlur, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_FloatLonger ~6121247)
void __cdecl ScrCmd_FloatLonger(scr_entref_t entref)
{
    gentity_s *ent;
    gentity_s *tempEnt;

    ent = GetEntity(entref);
    tempEnt = G_TempEntity(vec3_origin, EV_FLOAT_LONGER);
    tempEnt->s.otherEntityNum = ent->s.number;
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_ForceBuoyancy ~6121259)
void __cdecl ScrCmd_ForceBuoyancy(scr_entref_t entref)
{
    gentity_s *ent;
    gentity_s *tempEnt;

    ent = GetEntity(entref);
    tempEnt = G_TempEntity(vec3_origin, EV_FORCE_BUOYANCY);
    tempEnt->s.otherEntityNum = ent->s.number;
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_ForceTexturesToLoad ~6138496)
void __cdecl ScrCmd_ForceTexturesToLoad(scr_entref_t entref)
{
    gentity_s *ent;
    int enable;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("call ForceTexturesToLoad with one parameter: true or false", 0);
    ent = GetEntity(entref);
    enable = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    R_Stream_ForceEntityTexturesToLoad(ent->s.number, enable != 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_EnableSledgehammer ~6155521)
void __cdecl GScr_EnableSledgehammer(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->client )
        Scr_Error("EnableSledgehammer called on a player.", 0);
    ent->flags &= ~0x20000u;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetAnimAssetType ~6145817)
void __cdecl GScr_GetAnimAssetType(scr_entref_t entref)
{
    gentity_s *ent;
    XAnimTree_s *tree;
    scr_anim_s anim;
    unsigned __int8 assetType;

    ent = GetEntity(entref);
    tree = GScr_GetEntAnimTree(ent);
    anim = Scr_GetAnim(0, tree, SCRIPTINSTANCE_SERVER);
    assetType = XAnimGetAssetType(tree->anims, anim.index);
    Scr_AddInt(assetType, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetAnimValue ~6148275)
void __cdecl GScr_GetAnimValue(scr_entref_t entref)
{
    gentity_s *ent;
    XAnimTree_s *tree;
    unsigned int numParams;
    bool hasDefault;
    scr_anim_s anim;
    const char *paramName;
    float defaultValue;
    float value;

    ent = GetEntity(entref);
    tree = GScr_GetEntAnimTree(ent);
    hasDefault = false;
    defaultValue = 0.0f;
    value = 0.0f;
    numParams = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParams != 2 )
    {
        if ( numParams != 3 )
            Scr_Error("wrong number of many parameters", 0);
        hasDefault = true;
        defaultValue = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    }
    paramName = SL_ConvertToString(Scr_GetConstString(1u, SCRIPTINSTANCE_SERVER), SCRIPTINSTANCE_SERVER);
    anim = Scr_GetAnim(0, tree, SCRIPTINSTANCE_SERVER);
    if ( XAnimGetParamValue(tree, anim.index, paramName, &value) )
        Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
    else if ( hasDefault )
        Scr_AddFloat(defaultValue, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddBool(false, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ViewKick)
void __cdecl GScr_ViewKick(scr_entref_t entref)
{
    double Float;
    const char *fmtMsg;
    float *damage_from;
    float *tempValue4;
    float origin[3];
    gentity_s *ent;

    ent = GetPlayerEntity(entref);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2 )
        Scr_Error("USAGE: <player> viewkick <force 0-127> <source position>\n", 0);
    ent->client->damage_blood = (ent->maxHealth * Scr_GetInt(0, SCRIPTINSTANCE_SERVER) + 50) / 100;
    if ( ent->client->damage_blood < 0 )
    {
        Float = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("viewkick: damage %g < 0\n", Float);
        Scr_Error(fmtMsg, 0);
    }
    Scr_GetVector(1u, origin, SCRIPTINSTANCE_SERVER);
    damage_from = ent->client->damage_from;
    tempValue4 = ent->client->ps.origin;
    *damage_from = *tempValue4 - origin[0];
    damage_from[1] = tempValue4[1] - origin[1];
    damage_from[2] = tempValue4[2] - origin[2];
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_LocalToWorldCoords)
void __cdecl GScr_LocalToWorldCoords(scr_entref_t entref)
{
    float vLocal[3];
    float vWorld[3];
    gentity_s *ent;
    float axis[3][3];

    ent = GetEntity(entref);
    Scr_GetVector(0, vLocal, SCRIPTINSTANCE_SERVER);
    AnglesToAxis(ent->r.currentAngles, axis);
    MatrixTransformVector(vLocal, axis, vWorld);
    vWorld[0] = vWorld[0] + ent->r.currentOrigin[0];
    vWorld[1] = vWorld[1] + ent->r.currentOrigin[1];
    vWorld[2] = vWorld[2] + ent->r.currentOrigin[2];
    Scr_AddVector(vWorld, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WorldToLocalCoords @ 82617690)
void __cdecl GScr_WorldToLocalCoords(scr_entref_t entref)
{
    float vLocal[3];
    float vWorld[3];
    gentity_s *ent;
    float axis[4][3];

    ent = GetEntity(entref);
    Scr_GetVector(0, vWorld, SCRIPTINSTANCE_SERVER);
    AnglesToAxis(ent->r.currentAngles, axis);
    axis[3][0] = ent->r.currentOrigin[0];
    axis[3][1] = ent->r.currentOrigin[1];
    axis[3][2] = ent->r.currentOrigin[2];
    MatrixTransposeTransformVector43(vWorld, axis, vLocal);
    Scr_AddVector(vLocal, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetRightArc)
void __cdecl GScr_SetRightArc(scr_entref_t entref)
{
    TurretInfo *pTurretInfo;

    pTurretInfo = GetEntity(entref)->pTurretInfo;
    if ( !pTurretInfo )
        Scr_Error("entity is not a turret", 0);
    pTurretInfo->arcmin[1] = -Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    if ( pTurretInfo->arcmin[1] > 0.0 )
        pTurretInfo->arcmin[1] = 0.0f;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetLeftArc)
void __cdecl GScr_SetLeftArc(scr_entref_t entref)
{
    TurretInfo *pTurretInfo;

    pTurretInfo = GetEntity(entref)->pTurretInfo;
    if ( !pTurretInfo )
        Scr_Error("entity is not a turret", 0);
    pTurretInfo->arcmax[1] = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    if ( pTurretInfo->arcmax[1] < 0.0 )
        pTurretInfo->arcmax[1] = 0.0f;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetTopArc)
void __cdecl GScr_SetTopArc(scr_entref_t entref)
{
    TurretInfo *pTurretInfo;

    pTurretInfo = GetEntity(entref)->pTurretInfo;
    if ( !pTurretInfo )
        Scr_Error("entity is not a turret", 0);
    pTurretInfo->arcmin[0] = -Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    if ( pTurretInfo->arcmin[0] > 0.0 )
        pTurretInfo->arcmin[0] = 0.0f;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetBottomArc)
void __cdecl GScr_SetBottomArc(scr_entref_t entref)
{
    TurretInfo *pTurretInfo;

    pTurretInfo = GetEntity(entref)->pTurretInfo;
    if ( !pTurretInfo )
        Scr_Error("entity is not a turret", 0);
    pTurretInfo->arcmax[0] = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    if ( pTurretInfo->arcmax[0] < 0.0 )
        pTurretInfo->arcmax[0] = 0.0f;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_PlaceSpawnPoint)
void __cdecl GScr_PlaceSpawnPoint(scr_entref_t entref)
{
    __int16 EntityHitId; // ax
    col_context_t context;
    float vEnd[3];
    trace_t trace;
    gentity_s *pEnt;
    float vStart[3];

    memset(&trace, 0, 16);
    //col_context_t::col_context_t(&context);
    pEnt = GetEntity(entref);
    vStart[0] = pEnt->r.currentOrigin[0];
    vStart[1] = pEnt->r.currentOrigin[1];
    vStart[2] = pEnt->r.currentOrigin[2];
    vEnd[0] = pEnt->r.currentOrigin[0];
    vEnd[1] = pEnt->r.currentOrigin[1];
    vEnd[2] = pEnt->r.currentOrigin[2];
    vEnd[2] = vEnd[2] + 128.0;
    G_TraceCapsule(
        &trace,
        vStart,
        playerMins,
        playerMaxs,
        vEnd,
        pEnt->s.number,
        0x2818011,
        &context);
    Vec3Lerp(vStart, vEnd, trace.fraction, vStart);
    vEnd[0] = vStart[0];
    vEnd[1] = vStart[1];
    vEnd[2] = vStart[2] - 262144.0;
    G_TraceCapsule(
        &trace,
        vStart,
        playerMins,
        playerMaxs,
        vEnd,
        pEnt->s.number,
        0x2818011,
        &context);
    EntityHitId = Trace_GetEntityHitId(&trace);
    pEnt->s.groundEntityNum = EntityHitId;
    g_entities[pEnt->s.groundEntityNum].flags |= 0x100000u;
    Vec3Lerp(vStart, vEnd, trace.fraction, vStart);
    G_TraceCapsule(
        &trace,
        vStart,
        playerMins,
        playerMaxs,
        vStart,
        pEnt->s.number,
        0x2818011,
        &context);
    if ( trace.allsolid )
        Com_PrintWarning(
            24,
            "WARNING: Spawn point entity %i is in solid at (%i, %i, %i)\n",
            pEnt->s.number,
            (int)pEnt->r.currentOrigin[0],
            (int)pEnt->r.currentOrigin[1],
            (int)pEnt->r.currentOrigin[2]);
    G_SetOrigin(pEnt, vStart);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_SendFaceEvent)
void __cdecl ScrCmd_SendFaceEvent(scr_entref_t entref)
{
    unsigned __int16 face_event;
    gentity_s *ent;

    ent = GetEntity(entref);
    face_event = (unsigned __int16)Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    if ( face_event == scr_const.face_casual )
    {
        G_AddEvent(ent, 0xC5u, 0);
    }
    else if ( face_event == scr_const.face_alert )
    {
        G_AddEvent(ent, 0xC5u, 1u);
    }
    else if ( face_event == scr_const.face_cqb )
    {
        G_AddEvent(ent, 0xC5u, 2u);
    }
    else if ( face_event == scr_const.face_running )
    {
        G_AddEvent(ent, 0xC5u, 3u);
    }
    else if ( face_event == scr_const.face_shoot_single )
    {
        G_AddEvent(ent, 0xC5u, 4u);
    }
    else if ( face_event == scr_const.face_shoot_burst )
    {
        G_AddEvent(ent, 0xC5u, 5u);
    }
    else if ( face_event == scr_const.face_react )
    {
        G_AddEvent(ent, 0xC5u, 7u);
    }
    else if ( face_event == scr_const.face_talk )
    {
        G_AddEvent(ent, 0xC5u, 8u);
    }
    else if ( face_event == scr_const.face_talk_long )
    {
        G_AddEvent(ent, 0xC5u, 9u);
    }
    else if ( face_event == scr_const.face_pain )
    {
        G_AddEvent(ent, 0xC5u, 0xAu);
    }
    else if ( face_event == scr_const.face_death )
    {
        G_AddEvent(ent, 0xC5u, 0xBu);
    }
    else if ( face_event == scr_const.face_melee )
    {
        G_AddEvent(ent, 0xC5u, 6u);
    }
    else
    {
        Com_PrintError(19, "Invalid face state specified by script\n");
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_TestSpawnPoint)
void GScr_TestSpawnPoint()
{
    col_context_t context;
    trace_t trace;
    float point[3];

    memset(&trace, 0, 16);
    //col_context_t::col_context_t(&context);
    Scr_GetVector(0, point, SCRIPTINSTANCE_SERVER);
    G_TraceCapsule(&trace, point, playerMins, playerMaxs, point, 1023, (int)0x810011, &context);
    if ( trace.startsolid || trace.allsolid )
        Scr_AddBool(0, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddBool(1u, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c:6133920
static void SaveCompleteCallback()
{
}

// Decomp: CoDSP_rdBlackOps.map.c:6133738
void __cdecl Scr_CheckForSaveErrors(int result)
{
    if ( result == -2 )
        Scr_Error("Attempting to save multiple times in a single frame.\n", 0);
    else if ( result == -1 )
        Scr_Error("Attempting to save during a restart.\n", 0);
    else if ( result == -4 )
        Scr_Error("Pending save buffer exceeded.\n", 0);
}

// Decomp: CoDSP_rdBlackOps.map.c:6133928
void Scr_SaveGame()
{
    const char *mapname;
    const char *saveName;
    const char *description;
    const char *saveProfile;
    int numParam;
    bool commitNow;
    char savePath[64];
    gentity_s *player;
    int result;

    mapname = Dvar_GetString("mapname");
    if ( Com_IsMenuLevel(mapname) )
        return;

    player = G_Find(0, 356, scr_const.player);
    if ( !player || player->health <= 0 )
        Scr_Error("Attempting to save while dead\n", 0);

    saveName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( !saveName || !saveName[0] )
        saveName = "auto";

    numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    description = ( numParam > 1 ) ? Scr_GetIString(1u, SCRIPTINSTANCE_SERVER) : "";
    saveProfile = ( numParam <= 2 ) ? "$default" : Scr_GetString(2u, SCRIPTINSTANCE_SERVER);
    commitNow = numParam > 3 && Scr_GetInt(3u, SCRIPTINSTANCE_SERVER) != 0;

    if ( I_stricmp(saveName, "levelstart") )
        Com_sprintf(savePath, 0x40u, "autosave\\%s-%s", mapname, saveName);
    else
        Com_sprintf(savePath, 0x40u, "autosave\\%s", mapname);

    SaveMemory_ClearForcedCommitFlag();
    result = SV_AddPendingSave(
        savePath,
        description,
        saveProfile,
        0,
        commitNow ? 2u : 0u,
        commitNow,
        SaveCompleteCallback);
    Scr_CheckForSaveErrors(result);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SaveGameNoCommit ~6134107)
void Scr_SaveGameNoCommit()
{
    const char *mapname;
    const char *saveName;
    const char *description;
    const char *saveProfile;
    int numParam;
    bool commitNow;
    char savePath[64];
    gentity_s *player;
    int clientNum;
    int result;

    for ( clientNum = 0; clientNum < level.maxclients; ++clientNum )
    {
        player = &g_entities[clientNum];
        if ( player->client && player->health <= 0 )
            Scr_Error("Attempting to save while dead\n", 0);
    }

    mapname = Dvar_GetString("mapname");
    saveName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( !saveName || !saveName[0] )
        saveName = "auto";

    numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    description = ( numParam > 1 ) ? Scr_GetIString(1u, SCRIPTINSTANCE_SERVER) : "";
    saveProfile = ( numParam <= 2 ) ? "$default" : Scr_GetString(2u, SCRIPTINSTANCE_SERVER);
    commitNow = numParam > 3 && Scr_GetInt(3u, SCRIPTINSTANCE_SERVER) != 0;

    if ( I_stricmp(saveName, "levelstart") )
        Com_sprintf(savePath, 0x40u, "autosave\\%s-%s", mapname, saveName);
    else
        Com_sprintf(savePath, 0x40u, "autosave\\%s", mapname);

    SaveMemory_ClearForcedCommitFlag();
    result = SV_AddPendingSave(
        savePath,
        description,
        saveProfile,
        0,
        1u,
        commitNow,
        SaveCompleteCallback);
    Scr_CheckForSaveErrors(result);
    Scr_AddInt(result, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PrefetchLevel ~6138622)
void Scr_PrefetchLevel()
{
    const char *levelName;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("call prefetchLevel with one parameter: level (fastfile) name (ie creek_1)", 0);
    levelName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    SV_GameSendServerCommand(-1, SV_CMD_CAN_IGNORE, va("%c %s", '+', levelName));
}

static bool g_forceLevelEnd;

bool GScr_IsForceLevelEnd()
{
    return g_forceLevelEnd;
}

void GScr_ClearForceLevelEnd()
{
    g_forceLevelEnd = false;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ForceLevelEnd ~6152209)
void GScr_ForceLevelEnd()
{
    g_forceLevelEnd = true;
    Dvar_SetString((dvar_s *)nextmap, "");
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_IsSaveSuccessful ~6134293)
void Scr_IsSaveSuccessful()
{
    SaveGame *save;

    save = SaveMemory_GetSaveHandle(0);
    Scr_AddBool(SaveMemory_IsSuccessful(save), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_IsRecentlyLoaded ~6134304)
void Scr_IsRecentlyLoaded()
{
    Scr_AddBool(SaveMemory_IsRecentlyLoaded(), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c:6134313
void Scr_CommitSave()
{
    SaveGame *save;
    int commitSaveId;
    void *header;

    save = SaveMemory_GetSaveHandle(0);
    iassert(save);

    commitSaveId = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( SaveMemory_IsCommitForced() )
    {
        SaveMemory_ClearForcedCommitFlag();
        return;
    }

    if ( SaveMemory_IsWaitingForCommit(save) )
        goto commit_save;

    save = SaveMemory_GetSaveHandle(1);
    iassert(save);
    header = SaveMemory_GetHeader(save);
    iassert(header);
    if ( commitSaveId != (int)save->saveId )
        Scr_Error("Save Commit attempted with no valid committable save available", 0);

commit_save:
    save = SaveMemory_GetSaveHandle(0);
    header = SaveMemory_GetHeader(save);
    iassert(header);
    if ( commitSaveId != (int)save->saveId )
        Scr_Error(va("Save Commit attempted on incorrect save currentsave:%i, commit:%i", commitSaveId, save->saveId), 0);
    if ( !G_CommitSavedGame(commitSaveId) )
        Scr_Error("Attempting to commit an invalid save", 0);
}

static void GScr_SetNextMap(const char *mapname)
{
    I_strncpyz(level.nextMap, mapname, sizeof(level.nextMap));
}

// Decomp: CoDSPBlackOps.map.c:3962907
void GScr_ChangeLevel()
{
    gentity_s *player;
    const char *nextMap;
    int paramCount;

    player = G_Find(0, 356, scr_const.player);
    if ( !player )
        Com_Error(ERR_DROP, "player");

    if ( player->health <= 0 || (g_reloading && g_reloading->current.integer) )
        return;

    paramCount = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( paramCount != 1 )
    {
        if ( paramCount != 2 )
        {
            const float exitTime = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
            level.missionFadeDuration = (int)floorf(exitTime * 1000.0f + 0.5f);
            if ( level.missionFadeDuration < 0 )
                Scr_ParamError(2, "exitTime cannot be negative", SCRIPTINSTANCE_SERVER);
        }
        level.savepersist = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    }

    nextMap = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( paramCount < 3 && level.missionFadeDuration >= 0 )
        ; // default fade already set from map or prior script call

    level.changeLevelPending = 1;
    GScr_SetNextMap(nextMap);
}

// Decomp: CoDSPBlackOps.map.c:3963001
void GScr_MissionSuccess()
{
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 2 )
        Scr_Error("missionSuccess only takes two parameters: missionSuccess(nextMap, persistent)\n", 0);
    GScr_ChangeLevel();
    level.endMissionScreen = 1;
}

// Decomp: CoDSPBlackOps.map.c:3963018
void GScr_MissionFailed()
{
    gentity_s *player;
    float spawnOrigin[3];
    float spawnAngles[3];

    SV_GameSendServerCommand(-1, SV_CMD_CAN_IGNORE, va("%c hud_missionFailed 1", 'v'));
    player = G_Find(0, 356, scr_const.player);
    if ( player )
    {
        spawnOrigin[0] = 0.0f;
        spawnOrigin[1] = 0.0f;
        spawnOrigin[2] = 0.0f;
        spawnAngles[0] = 0.0f;
        spawnAngles[1] = 0.0f;
        spawnAngles[2] = 0.0f;
        ClientSpawn(player, spawnOrigin, spawnAngles);
        level.loadgameContinuePending = 1;
    }
}

// Decomp: CoDSPBlackOps.map.c:3961148
void GScr_ExitLevel()
{
    if ( level.finished )
    {
        if ( level.finished == 1 )
            Scr_Error("map_restart already called", 0);
        else
            Scr_Error("exitlevel already called", 0);
    }
    level.finished = 3;
    level.savepersist = 0;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        level.savepersist = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    SV_MatchEnd();
    ExitLevel();
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_FastRestart)
void GScr_FastRestart()
{
    GScr_MapRestart();
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_MapRestart)
void GScr_MapRestart()
{
    if ( level.finished )
    {
        if ( level.finished == 1 )
            Scr_Error("map_restart already called", 0);
        else
            Scr_Error("exitlevel already called", 0);
    }
    level.finished = 1;
    level.savepersist = 0;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        level.savepersist = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    Cbuf_AddText(0, "fast_restart\n");
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_LoadMap)
void GScr_LoadMap()
{
    const char *fmtMsg;
    char *mapname;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        mapname = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        if ( SV_MapExists(mapname) )
        {
            if ( level.finished )
            {
                if ( level.finished == 2 )
                    Scr_Error("map already called", 0);
                else
                    Scr_Error("exitlevel already called", 0);
            }
            level.finished = 2;
            level.savepersist = 0;
            if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 1 )
                level.savepersist = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
            fmtMsg = va("map %s\n", mapname);
            Cbuf_AddText(0, fmtMsg);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_KillServer)
void GScr_KillServer()
{
    level.finished = 3;
    level.savepersist = 0;
    SV_KillLocalServer();
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_AddTestClient)
void GScr_AddTestClient()
{
    gentity_s *ent;

    ent = SV_AddTestClient();
    if ( ent )
        Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_MakeDvarServerInfo)
void GScr_MakeDvarServerInfo()
{
    const char *fmtMsg;
    int NumParam;
    char string[1024];
    char outString[1028];
    const char *dvarName;
    int type;
    const dvar_s *dvar;
    const char *dvarValue;

    dvarName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( !Dvar_IsValidName(dvarName) )
    {
        fmtMsg = va("Dvar %s has an invalid dvar name", dvarName);
        Scr_Error(fmtMsg, 0);
    }
    dvar = Dvar_FindVar(dvarName);
    if ( dvar )
    {
        Dvar_AddFlags(dvar, 1280);
    }
    else
    {
        type = Scr_GetType(1u, SCRIPTINSTANCE_SERVER);
        if ( type == 3 )
        {
            NumParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
            Scr_ConstructMessageString(1, NumParam - 1, "Dvar Value", string, 0x400u);
            dvarValue = string;
        }
        else
        {
            dvarValue = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
        }
        CleanDvarValue(dvarValue, outString, 1024);
        _Dvar_RegisterString(dvarName, (char *)dvarValue, 0x4500u, "Script defined user info dvar");
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetBombTimer)
void GScr_SetBombTimer()
{
    const char *fmtMsg;
    char *name;
    int value;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 2 )
    {
        name = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        value = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
        if ( I_stricmp("A", name) && I_stricmp("primary", name) )
        {
            if ( I_stricmp("B", name) && I_stricmp("secondary", name) )
            {
                fmtMsg = va("GScr_SetBombTimer: Unknown bomb timer name %s", name);
                Scr_Error(fmtMsg, 0);
            }
            else
            {
                level.matchState.archivedState.bombTimer[1] = value;
            }
        }
        else
        {
            level.matchState.archivedState.bombTimer[0] = value;
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetMatchTalkFlag)
void GScr_SetMatchTalkFlag()
{
    const char *fmtMsg;
    char *flagName;
    int flagBit;
    int value;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 2 )
    {
        flagName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        value = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
        if ( I_stricmp("EveryoneHearsEveryone", flagName) )
        {
            if ( I_stricmp("DeadChatWithDead", flagName) )
            {
                if ( I_stricmp("DeadChatWithTeam", flagName) )
                {
                    if ( I_stricmp("DeadHearTeamLiving", flagName) )
                    {
                        if ( I_stricmp("DeadHearAllLiving", flagName) )
                        {
                            fmtMsg = va("GScr_SetMatchTalkFlag: Couldn't find a matching bit for flag name : %s ", flagName);
                            Scr_Error(fmtMsg, 0);
                            return;
                        }
                        flagBit = 16;
                    }
                    else
                    {
                        flagBit = 8;
                    }
                }
                else
                {
                    flagBit = 4;
                }
            }
            else
            {
                flagBit = 2;
            }
        }
        else
        {
            flagBit = 1;
        }
        if ( value )
            level.matchState.unarchivedState.talkFlags |= flagBit;
        else
            level.matchState.unarchivedState.talkFlags &= ~flagBit;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetMatchFlag)
void GScr_SetMatchFlag()
{
    const char *fmtMsg;
    char *flagName;
    bool archived;
    char flagBit;
    int value;

    archived = 1;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 2 )
    {
        flagName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        value = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
        if ( I_stricmp("radar_allies", flagName) )
        {
            if ( I_stricmp("radar_axis", flagName) )
            {
                if ( I_stricmp("bomb_timer", flagName) )
                {
                    if ( I_stricmp("bomb_timer_a", flagName) )
                    {
                        if ( I_stricmp("bomb_timer_b", flagName) )
                        {
                            if ( I_stricmp("bomb_timer_b", flagName) )
                            {
                                if ( I_stricmp("ammocounterhide", flagName) )
                                {
                                    if ( I_stricmp("enable_popups", flagName) )
                                    {
                                        if ( I_stricmp("hud_hardcore", flagName) )
                                        {
                                            if ( I_stricmp("pregame", flagName) )
                                            {
                                                if ( I_stricmp("final_killcam", flagName) )
                                                {
                                                    if ( I_stricmp("round_end_killcam", flagName) )
                                                    {
                                                        if ( I_stricmp("cg_drawSpectatorMessages", flagName) )
                                                        {
                                                            if ( I_stricmp("disableIngameMenu", flagName) )
                                                            {
                                                                fmtMsg = va("GScr_SetMatchFlag: Couldn't find a matching bit for flag name : %s ", flagName);
                                                                Scr_Error(fmtMsg, 0);
                                                                return;
                                                            }
                                                            flagBit = 16;
                                                            archived = 0;
                                                        }
                                                        else
                                                        {
                                                            flagBit = 15;
                                                            archived = 0;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        flagBit = 5;
                                                        archived = 0;
                                                    }
                                                }
                                                else
                                                {
                                                    flagBit = 4;
                                                    archived = 0;
                                                }
                                            }
                                            else
                                            {
                                                flagBit = 14;
                                                archived = 0;
                                            }
                                        }
                                        else
                                        {
                                            flagBit = 13;
                                            archived = 0;
                                        }
                                    }
                                    else
                                    {
                                        flagBit = 8;
                                        archived = 0;
                                    }
                                }
                                else
                                {
                                    flagBit = 12;
                                }
                            }
                            else
                            {
                                flagBit = 11;
                            }
                        }
                        else
                        {
                            flagBit = 11;
                        }
                    }
                    else
                    {
                        flagBit = 10;
                    }
                }
                else
                {
                    flagBit = 9;
                }
            }
            else
            {
                flagBit = 7;
            }
        }
        else
        {
            flagBit = 6;
        }
        if ( archived )
        {
            if ( value )
                level.matchState.archivedState.matchUIVisibilityFlags |= 1 << flagBit >> 4;
            else
                level.matchState.archivedState.matchUIVisibilityFlags &= ~(1 << flagBit >> 4);
        }
        else if ( value )
        {
            level.matchState.unarchivedState.matchUIVisibilityFlags |= 1 << flagBit >> 4;
        }
        else
        {
            level.matchState.unarchivedState.matchUIVisibilityFlags &= ~(1 << flagBit >> 4);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_AllClientsPrint)
void GScr_AllClientsPrint()
{
    const char *fmtMsg;
    char *string;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        string = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("%c \"%s\"", 101, string);
        SV_GameSendServerCommand(-1, SV_CMD_CAN_IGNORE, fmtMsg);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_MapExists)
void GScr_MapExists()
{
    char *mapname;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        mapname = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        if ( SV_MapExists(mapname) )
            Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
        else
            Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsValidGameType)
void GScr_IsValidGameType()
{
    char *gametype;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        gametype = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        if ( Scr_IsValidGameType(gametype) )
            Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
        else
            Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetVoteString)
void GScr_SetVoteString()
{
    char *fmtMsg;
    char *fmtMsg2;
    char *fmtMsg3;
    char *string;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        string = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        SV_SetConfigstring(16, string);
        fmtMsg = va("%i", level_tail.voteTime);
        SV_SetConfigstring(15, fmtMsg);
        fmtMsg2 = va("%i", level_tail.voteYes);
        SV_SetConfigstring(17, fmtMsg2);
        fmtMsg3 = va("%i", level_tail.voteNo);
        SV_SetConfigstring(18, fmtMsg3);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetVoteTime)
void GScr_SetVoteTime()
{
    char *fmtMsg;
    char *fmtMsg2;
    char *fmtMsg3;
    int time;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        time = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("%i", time);
        SV_SetConfigstring(15, fmtMsg);
        fmtMsg2 = va("%i", level_tail.voteYes);
        SV_SetConfigstring(17, fmtMsg2);
        fmtMsg3 = va("%i", level_tail.voteNo);
        SV_SetConfigstring(18, fmtMsg3);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetVoteYesCount)
void GScr_SetVoteYesCount()
{
    char *fmtMsg;
    char *fmtMsg2;
    int yes;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        yes = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("%i", yes);
        SV_SetConfigstring(17, fmtMsg);
        fmtMsg2 = va("%i", level_tail.voteNo);
        SV_SetConfigstring(18, fmtMsg2);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetVoteNoCount)
void GScr_SetVoteNoCount()
{
    char *fmtMsg;
    int no;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        no = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("%i", no);
        SV_SetConfigstring(18, fmtMsg);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_KickPlayer)
void GScr_KickPlayer()
{
    const char *fmtMsg;
    const char *fmtMsg2;
    int playernum;
    char *reason;
    char genericreason[20];

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        playernum = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        strcpy(genericreason, "No reason specified");
        if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 2 )
        {
            reason = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
            if ( !reason || !*reason )
                reason = genericreason;
            fmtMsg = va("clientkick %i %s\n", playernum, reason);
            Cbuf_AddText(0, fmtMsg);
        }
        else
        {
            fmtMsg2 = va("clientkick %i\n", playernum);
            Cbuf_AddText(0, fmtMsg2);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_BanPlayer)
void GScr_BanPlayer()
{
    const char *fmtMsg;
    bool temp;
    int playernum;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        playernum = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        temp = 0;
        if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 2 )
            temp = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER) != 0;
        if ( temp )
            fmtMsg = va("tempBanClient %i\n", playernum);
        else
            fmtMsg = va("banClient %i\n", playernum);
        Cbuf_AddText(0, fmtMsg);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ClientPrint)
void GScr_ClientPrint()
{
    const char *fmtMsg;
    char *string;
    gentity_s *ent;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        ent = Scr_GetEntity(0);
        string = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("%c \"%s\"", 101, string);
        SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_OpenFile)
void GScr_OpenFile()
{
    char *tempValue1;
    char *fmtMsg;
    char *fmtMsg2;
    char *fullpathname;
    int filesize;
    char *filename;
    int tempFile;
    int *f;
    const char *mode;
    int filenum;

    f = 0;
    if ((unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 1)
    {
        filename = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        mode = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
        for (filenum = 0; filenum < 1; ++filenum)
        {
            if (!level.openScriptIOFileHandles[filenum])
            {
                f = &level.openScriptIOFileHandles[filenum];
                break;
            }
        }
        if (!f)
        {
            Com_Printf(24, "OpenFile failed.  %i files already open\n", 1);
            Scr_AddInt(-1, SCRIPTINSTANCE_SERVER);
            return;
        }
        if (!strcmp(mode, "read"))
        {
            fullpathname = va("%s/%s", "scriptdata", filename);
            filesize = FS_FOpenFileByMode(fullpathname, &tempFile, FS_READ);
            if (filesize >= 0)
            {
                tempValue1 = Z_VirtualAlloc(filesize + 1, "GScr_OpenFile", 11);
                level.openScriptIOFileBuffers[filenum] = tempValue1;
                FS_Read((unsigned __int8 *)level.openScriptIOFileBuffers[filenum], filesize, tempFile);
                FS_FCloseFile(tempFile);
                level.openScriptIOFileBuffers[filenum][filesize] = 0;
                Com_BeginParseSession(filename);
                Com_SetCSV(1);
                level.currentScriptIOLineMark[filenum].lines = 0;
                Scr_AddInt(filenum, SCRIPTINSTANCE_SERVER);
            }
            else
            {
                Scr_AddInt(-1, SCRIPTINSTANCE_SERVER);
            }
            return;
        }
        if (!strcmp(mode, "write"))
        {
            fmtMsg = va("%s/%s", "scriptdata", filename);
            *f = FS_FOpenTextFileWrite(fmtMsg);
            if (!*f)
            {
                Scr_AddInt(-1, SCRIPTINSTANCE_SERVER);
                return;
            }
        }
        else if (!strcmp(mode, "append"))
        {
            fmtMsg2 = va("%s/%s", "scriptdata", filename);
            if ((FS_FOpenFileByMode(fmtMsg2, f, FS_APPEND) & 0x80000000) != 0)
            {
                Scr_AddInt(-1, SCRIPTINSTANCE_SERVER);
                return;
            }
        }
        else
        {
            Com_Printf(24, "Valid openfile modes are 'write', 'read', and 'append'\n");
            Scr_AddInt(-1, SCRIPTINSTANCE_SERVER);
            return;
        }
        Scr_AddInt(filenum, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_CloseFile)
void GScr_CloseFile()
{
    int filenum;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        filenum = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        if ( (unsigned int)filenum >= 2 )
        {
            Com_Printf(24, "CloseFile failed, invalid file number %i\n", filenum);
            Scr_AddInt(-1, SCRIPTINSTANCE_SERVER);
            return;
        }
        if ( level.openScriptIOFileHandles[filenum]
            && level.openScriptIOFileBuffers[filenum]
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                        11897,
                        0,
                        "%s",
                        "!((level.openScriptIOFileHandles[filenum] != 0) && (level.openScriptIOFileBuffers[filenum] != NULL))") )
        {
            __debugbreak();
        }
        if ( level.openScriptIOFileHandles[filenum] )
        {
            FS_FCloseFile(level.openScriptIOFileHandles[filenum]);
            level.openScriptIOFileHandles[filenum] = 0;
        }
        else
        {
            if ( !level.openScriptIOFileBuffers[filenum] )
            {
                Com_Printf(24, "CloseFile failed, file number %i was not open\n", filenum);
                Scr_AddInt(-1, SCRIPTINSTANCE_SERVER);
                return;
            }
            Com_EndParseSession();
            Z_VirtualFree(level.openScriptIOFileBuffers[filenum], 11);
            level.openScriptIOFileBuffers[filenum] = 0;
        }
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_FPrintln)
void GScr_FPrintln()
{
    Scr_FPrint_internal(0);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_FPrint_internal)
void __cdecl Scr_FPrint_internal(bool commaBetweenFields)
{
    int NumParam;
    char *s;
    unsigned int arg;
    int filenum;

    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 1 )
    {
        filenum = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        if ( (unsigned int)filenum < 2 )
        {
            if ( level.openScriptIOFileHandles[filenum] )
            {
                for ( arg = 1; arg < Scr_GetNumParam(SCRIPTINSTANCE_SERVER); ++arg )
                {
                    s = Scr_GetString(arg, SCRIPTINSTANCE_SERVER);
                    FS_Write(s, strlen(s), level.openScriptIOFileHandles[filenum]);
                    if ( commaBetweenFields )
                        FS_Write(",", 1u, level.openScriptIOFileHandles[filenum]);
                }
                FS_Write("\n", 1u, level.openScriptIOFileHandles[filenum]);
                NumParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
                Scr_AddInt(NumParam - 1, SCRIPTINSTANCE_SERVER);
            }
            else
            {
                Com_Printf(24, "FPrintln failed, file number %i was not open for writing\n", filenum);
                Scr_AddInt(-1, SCRIPTINSTANCE_SERVER);
            }
        }
        else
        {
            Com_Printf(24, "FPrintln failed, invalid file number %i\n", filenum);
            Scr_AddInt(-1, SCRIPTINSTANCE_SERVER);
        }
    }
    else
    {
        Com_Printf(24, "fprintln requires at least 2 parameters (file, output)\n");
        Scr_AddInt(-1, SCRIPTINSTANCE_SERVER);
    }
}

void GScr_FPrintFields()
{
    Scr_FPrint_internal(1);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_FReadLn)
void GScr_FReadLn()
{
    int tempValue0;
    int ArgCountOnLine;
    bool eof;
    const char *buf;
    const char *token;
    int filenum;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        filenum = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        if ( (unsigned int)filenum < 2 )
        {
            if ( level.openScriptIOFileBuffers[filenum] )
            {
                buf = level.openScriptIOFileBuffers[filenum];
                if ( level.currentScriptIOLineMark[filenum].lines )
                {
                    Com_ParseReturnToMark(&buf, &level.currentScriptIOLineMark[filenum]);
                    Com_SkipRestOfLine(&buf);
                    Com_ParseSetMark(&buf, &level.currentScriptIOLineMark[filenum]);
                    token = (const char *)Com_Parse(&buf);
                    eof = *token == 0;
                    Com_ParseReturnToMark(&buf, &level.currentScriptIOLineMark[filenum]);
                    if ( eof )
                    {
                        Scr_AddInt(-1, SCRIPTINSTANCE_SERVER);
                    }
                    else
                    {
                        ArgCountOnLine = Com_GetArgCountOnLine(&buf);
                        Scr_AddInt(ArgCountOnLine, SCRIPTINSTANCE_SERVER);
                    }
                }
                else
                {
                    Com_ParseSetMark(&buf, &level.currentScriptIOLineMark[filenum]);
                    tempValue0 = Com_GetArgCountOnLine(&buf);
                    Scr_AddInt(tempValue0, SCRIPTINSTANCE_SERVER);
                }
            }
            else
            {
                Com_Printf(24, "freadln failed, file number %i was not open for reading\n", filenum);
                Scr_AddInt(-1, SCRIPTINSTANCE_SERVER);
            }
        }
        else
        {
            Com_Printf(24, "freadln failed, invalid file number %i\n", filenum);
            Scr_AddInt(-1, SCRIPTINSTANCE_SERVER);
        }
    }
    else
    {
        Com_Printf(24, "freadln requires a parameter - the file to read from\n");
        Scr_AddInt(-1, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_FGetArg)
void GScr_FGetArg()
{
    const char *buf;
    int arg;
    int i;
    const char *token;
    int filenum;

    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 1 )
    {
        filenum = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        arg = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
        if ( (unsigned int)filenum < 2 )
        {
            if ( arg >= 0 )
            {
                if ( level.openScriptIOFileBuffers[filenum] )
                {
                    token = 0;
                    buf = level.openScriptIOFileBuffers[filenum];
                    Com_ParseReturnToMark(&buf, &level.currentScriptIOLineMark[filenum]);
                    for ( i = 0; i <= arg; ++i )
                    {
                        token = (const char *)Com_ParseOnLine(&buf);
                        if ( !*token )
                        {
                            Com_Printf(
                                24,
                                "freadline failed, there aren't %i arguments on this line, there are only %i arguments\n",
                                arg + 1,
                                i);
                            Scr_AddString((char *)"", SCRIPTINSTANCE_SERVER);
                            return;
                        }
                    }
                    Scr_AddString((char *)token, SCRIPTINSTANCE_SERVER);
                }
                else
                {
                    Com_Printf(24, "freadline failed, file number %i was not open for reading\n", filenum);
                    Scr_AddString((char *)"", SCRIPTINSTANCE_SERVER);
                }
            }
            else
            {
                Com_Printf(24, "freadline failed, invalid argument number %i\n", arg);
                Scr_AddString((char *)"", SCRIPTINSTANCE_SERVER);
            }
        }
        else
        {
            Com_Printf(24, "freadline failed, invalid file number %i\n", filenum);
            Scr_AddString((char *)"", SCRIPTINSTANCE_SERVER);
        }
    }
    else
    {
        Com_Printf(24, "freadline requires at least 2 parameters (file, string)\n");
        Scr_AddString((char *)"", SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ExecDevgui)
void GScr_ExecDevgui()
{
    const char *fmtMsg;
    char *filename;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        filename = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("exec %s\n", filename);
        Cbuf_AddText(0, fmtMsg);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_IsGlobalStatsServer)
void Scr_IsGlobalStatsServer()
{
    int LicenseType;

    LicenseType = SV_GetLicenseType();
    if ( SV_IsServerRanked(LicenseType) )
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
}

void Scr_SetPlayerStatsForMatchRecording()
{
    unsigned int unusedResult;
    char *statName;
    unsigned int statValue;
    gentity_s *ent;

    PROF_SCOPED("GScr_SetPlayerStatsForMatchRecording");

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 3 )
        Scr_ParamError(0, "recordPlayerStats [player] [statName] [value]", SCRIPTINSTANCE_SERVER);
    ent = Scr_GetEntity(0);
    if ( !ent )
        Scr_ParamError(0, "recordPlayerStats Error: param 0 is not an entity.", SCRIPTINSTANCE_SERVER);
    if ( !ent->client )
        Scr_ParamError(0, "recordPlayerStats Error: param 0 is not an player.", SCRIPTINSTANCE_SERVER);
    statName = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    if ( !statName )
        Scr_ParamError(1u, "recordPlayerStats Error: param 1 is not a string.", SCRIPTINSTANCE_SERVER);
    statValue = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
#ifdef KISAK_LIVE
    MatchRecordSetPlayerStat(ent->client, statName, statValue);
#endif
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetPlayerFinalForMatchRecording)
void GScr_SetPlayerFinalForMatchRecording()
{
    gentity_s *ent;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_ParamError(0, "recordplayermatchend [player]", SCRIPTINSTANCE_SERVER);
    ent = Scr_GetEntity(0);
    if ( !ent )
        Scr_ParamError(0, "recordplayermatchend Error: param 0 is not an entity.", SCRIPTINSTANCE_SERVER);
    if ( !ent->client )
        Scr_ParamError(0, "recordplayermatchend Error: param 0 is not an player.", SCRIPTINSTANCE_SERVER);
#ifdef KISAK_LIVE
    MatchRecordEnd(ent->client);
#endif
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetBeginForMatchRecording)
void GScr_SetBeginForMatchRecording()
{
#ifdef KISAK_LIVE
    MatchRecordBegin();
#endif
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetAssignedTeam)
void GScr_GetAssignedTeam()
{
    Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsSplitscreen)
void GScr_IsSplitscreen()
{
    Scr_AddInt(CL_LocalClient_GetActiveCount() > 1, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SendLeaderboards)
void __cdecl GScr_SendLeaderboards(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *playerEnt;

    playerEnt = GetEntity(entref);
    if ( !playerEnt->client )
        Scr_Error("sendleaderboards: entity must be a player entity", 0);
    fmtMsg = va("%c", 80);
    SV_GameSendServerCommand(playerEnt->s.number, SV_CMD_RELIABLE, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsItemPurchased)
void __cdecl GScr_IsItemPurchased(scr_entref_t entref)
{
    bool IsItemLockedForChallenge;

    IsItemLockedForChallenge = GScr_IsItemLockedForChallenge(entref, 1);
    Scr_AddInt(!IsItemLockedForChallenge, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsItemLockedForChallenge)
bool __cdecl GScr_IsItemLockedForChallenge(scr_entref_t entref, bool purchaseRequired)
{
    int ClientPrestige;
    int ClientRank;
    gentity_s *playerEnt;
    bool isLocked;
    int itemIndex;

    isLocked = 0;
    playerEnt = GetEntity(entref);
    if ( !playerEnt || !playerEnt->client )
    {
        Scr_Error("isItemLocked: entity must be a player entity", 0);
        return 1;
    }
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        itemIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        if ( itemIndex && itemIndex < 256 && CL_LocalClient_GetActiveCount() )
        {
            ClientRank = G_GetClientRank(playerEnt->s.number);
            ClientPrestige = G_GetClientPrestige(playerEnt->s.number);
            isLocked = BG_UnlockablesIsItemLockedForRank(ClientPrestige, ClientRank, itemIndex);
        }
        if ( purchaseRequired && !isLocked )
            return !GScr_IsItemPurchasedForClientNum(playerEnt->s.number, itemIndex);
        return isLocked;
    }
    Scr_Error("isItemLocked: takes one parameter.", 0);
    return 1;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsItemPurchasedForClientNum)
bool __cdecl GScr_IsItemPurchasedForClientNum(unsigned int clientNum, unsigned int itemIndex)
{
    if ( clientNum >= 0x20
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    12614,
                    0,
                    "%s",
                    "( clientNum >= 0 ) && ( clientNum < MAX_CLIENTS )") )
    {
        __debugbreak();
    }
    if ( itemIndex >= 0x100
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    12615,
                    0,
                    "%s",
                    "( itemIndex >= 0 ) && ( itemIndex < MAX_UNLOCKABLE_ITEMS )") )
    {
        __debugbreak();
    }
    return clientNum < 0x20
            && itemIndex < 0x100
            && ((1 << (itemIndex & 7)) & svs.clients[clientNum].purchasedItems[(int)itemIndex >> 3]) == 1 << (itemIndex & 7);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsItemLocked)
void __cdecl GScr_IsItemLocked(scr_entref_t entref)
{
    bool IsItemLockedForChallenge;

    IsItemLockedForChallenge = GScr_IsItemLockedForChallenge(entref, 0);
    Scr_AddInt(IsItemLockedForChallenge, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetRefFromItemIndex)
void GScr_GetRefFromItemIndex()
{
    VariableUnion itemIndex;
    char *itemRef;

    itemIndex.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    itemRef = (char *)BG_UnlockablesGetItemRef(itemIndex.intValue);
    if ( itemRef && *itemRef )
        Scr_AddString(itemRef, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddString((char *)"", SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetItemGroupFromItemIndex)
void GScr_GetItemGroupFromItemIndex()
{
    VariableUnion itemIndex;
    char *itemGroup;

    itemIndex.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    itemGroup = (char *)BG_UnlockablesGetItemGroup(itemIndex.intValue);
    if ( itemGroup && *itemGroup )
        Scr_AddString(itemGroup, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddString((char *)"", SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetBaseWeaponItemIndex)
void GScr_GetBaseWeaponItemIndex()
{
    int WeaponTableItemIndex;
    const WeaponVariantDef *weapVarDef;
    const WeaponVariantDef *weapVariantDef;
    int weaponItemIndex;
    unsigned int iWeaponIndex;
    char *pszWeaponName;

    weaponItemIndex = 0;
    pszWeaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = G_GetWeaponIndexForName(pszWeaponName);
    if (iWeaponIndex)
    {
        weapVariantDef = BG_GetWeaponVariantDef(iWeaponIndex);
        if (weapVariantDef && weapVariantDef->weapDef)
            weaponItemIndex = weapVariantDef->weapDef->itemIndex;
        if (!weaponItemIndex)
        {
            weapVarDef = BG_GetWeaponVariantDef(iWeaponIndex);
            if (weapVarDef->iVariantCount >= 0)
                WeaponTableItemIndex = BG_GetWeaponTableItemIndex(iWeaponIndex);
            else
                WeaponTableItemIndex = BG_GetWeaponTableItemIndex(-weapVarDef->iVariantCount);
            weaponItemIndex = WeaponTableItemIndex;
        }
    }
    Scr_AddInt(weaponItemIndex, SCRIPTINSTANCE_SERVER);
}

const char *lbWagerGameModeEnum[5] =
{ "oic", "hlnd", "gun", "shrp", NULL };

const char *lbTypeEnum[17] =
{
  "tdm",
  "dm",
  "ctf",
  "dom",
  "sab",
  "sd",
  "koth",
  "dem",
  "hctdm",
  "hcdm",
  "hcctf",
  "hcdom",
  "hcsab",
  "hcsd",
  "hckoth",
  "hcdem",
  NULL
};


// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetGameTypeEnumFromName)
void GScr_GetGameTypeEnumFromName()
{
    const char *fmtMsg;
    const char *tempValue1;
    char *gameTypeName;
    int endIndex;
    const char **gameModeEnum;
    int currGameMode;

    gameTypeName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( Scr_GetInt(1u, SCRIPTINSTANCE_SERVER) )
    {
        endIndex = 4;
        gameModeEnum = lbWagerGameModeEnum;
    }
    else
    {
        endIndex = 16;
        gameModeEnum = lbTypeEnum;
    }
    for ( currGameMode = 0; currGameMode < endIndex; ++currGameMode )
    {
        if ( !I_stricmp(gameTypeName, gameModeEnum[currGameMode]) )
        {
            Scr_AddInt(currGameMode, SCRIPTINSTANCE_SERVER);
            return;
        }
    }
    if ( gameTypeName )
        tempValue1 = gameTypeName;
    else
        tempValue1 = "unknown";
    fmtMsg = va("GetGameTypeEnumFromName: Invalid gametype parameter: '%s' supplied to GetGameTypeEnumFromName", tempValue1);
    Scr_Error(fmtMsg, 0);
    Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetWagerGametypeList)
void GScr_GetWagerGametypeList()
{
    int i;

    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for ( i = 0; i < 4; ++i )
    {
        Scr_AddString((char *)lbWagerGameModeEnum[i], SCRIPTINSTANCE_SERVER);
        Scr_AddArray(SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetLoadoutItemFromProfile)
void __cdecl GScr_GetLoadoutItemFromProfile(scr_entref_t entref)
{
    int item;

    if ( !GetEntity(entref)->client )
        Scr_Error("getloadoutitemfromprofile: entity must be a player entity", 0);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 1 )
    {
        Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
        Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    }
    item = RETURN_ZERO32();
    Scr_AddInt(item, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetDStat)
void __cdecl GScr_GetDStat(scr_entref_t entref)
{
#ifdef KISAK_LIVE
    char *String;
    char *tempValue2;
    const char *fmtMsg;
    VariableUnion varUnion;
    unsigned int ClientDIntStat;
    char *ClientDStringStat;
    __int64 tempValue7; // rax
    char *fmtMsg2;
    int Type;
    signed int i;
    gentity_s *playerEnt;
    ddlState_t searchState;
    int argc;

    searchState = *LiveStats_GetRootDDLState();
    playerEnt = GetEntity(entref);
    if ( !playerEnt->client )
        Scr_Error("getdstat: entity must be a player entity", 0);
    argc = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( argc > 8 )
        Scr_Error("getdstat: path is too deep.", 0);
    for ( i = 0; i < argc; ++i )
    {
        Type = Scr_GetType(i, SCRIPTINSTANCE_SERVER);
        if ( Type == 2 )
        {
            if ( searchState.member
                && searchState.member->arraySize > 1
                && searchState.member->enumIndex == -1
                && searchState.arrayIndex == -1 )
            {
                Scr_Error("getdstat: array index (integer) expected. Received a string instead.", 0);
            }
            String = Scr_GetString(i, SCRIPTINSTANCE_SERVER);
            if ( !DDL_MoveToName(&searchState, &searchState, String) )
            {
                tempValue2 = Scr_GetString(i, SCRIPTINSTANCE_SERVER);
                fmtMsg = va("getdstat: Could not find member name %s.", tempValue2);
                Scr_Error(fmtMsg, 0);
            }
        }
        else if ( Type == 6 )
        {
            if ( !searchState.member
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                            12957,
                            0,
                            "%s",
                            "searchState.member") )
            {
                __debugbreak();
            }
            if ( searchState.member->arraySize == 1 )
                Scr_Error("getdstat: member name (string) expected. Received an integer instead.", 0);
            varUnion.intValue = Scr_GetInt(i, SCRIPTINSTANCE_SERVER);
            if ( !DDL_MoveToIndex(&searchState, &searchState, varUnion.intValue, 1) )
                Scr_Error("getdstat: Could not find member array index number.", 0);
        }
        else
        {
            Scr_Error("getdstat: Expected strings or integers only.", 0);
        }
    }
    if ( searchState.member )
    {
        switch ( searchState.member->type )
        {
            case 0:
            case 1:
            case 2:
                ClientDIntStat = SV_GetClientDIntStat(playerEnt->s.number, &searchState);
                Scr_AddInt(ClientDIntStat, SCRIPTINSTANCE_SERVER);
                break;
            case 3:
                LODWORD(tempValue7) = SV_GetClientDInt64Stat(playerEnt->s.number, &searchState);
                fmtMsg2 = va("%llu", tempValue7);
                Scr_AddString(fmtMsg2, SCRIPTINSTANCE_SERVER);
                break;
            case 5:
                ClientDStringStat = SV_GetClientDStringStat(playerEnt->s.number, &searchState);
                Scr_AddString(ClientDStringStat, SCRIPTINSTANCE_SERVER);
                break;
            default:
                Scr_Error("getdstat: stat type undefined", 0);
                Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
                break;
        }
    }
    else
    {
        Scr_Error("getdstat: could not find ddl member.", 0);
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
#else
    // Decomp: CoDSP_rdBlackOps.map.c — offline SP: zero/empty defaults without live stats backend
    gentity_s *playerEnt;
    ddlState_t searchState;
    VariableUnion varUnion;
    int argc;
    signed int i;
    int Type;
    char *String;

    playerEnt = GetEntity(entref);
    if ( !playerEnt->client )
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
        return;
    }
    searchState = *LiveStats_GetRootDDLState();
    argc = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( argc > 8 )
        argc = 8;
    for ( i = 0; i < argc; ++i )
    {
        Type = Scr_GetType(i, SCRIPTINSTANCE_SERVER);
        if ( Type == 2 )
        {
            String = Scr_GetString(i, SCRIPTINSTANCE_SERVER);
            if ( !DDL_MoveToName(&searchState, &searchState, String) )
                break;
        }
        else if ( Type == 6 )
        {
            if ( searchState.member && searchState.member->arraySize == 1 )
                break;
            varUnion.intValue = Scr_GetInt(i, SCRIPTINSTANCE_SERVER);
            if ( !DDL_MoveToIndex(&searchState, &searchState, varUnion.intValue, 1) )
                break;
        }
        else
        {
            break;
        }
    }
    if ( searchState.member )
    {
        switch ( searchState.member->type )
        {
            case 5:
                Scr_AddString("", SCRIPTINSTANCE_SERVER);
                break;
            case 3:
                Scr_AddString("0", SCRIPTINSTANCE_SERVER);
                break;
            default:
                Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
                break;
        }
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
#endif
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetMaxActiveContracts)
void GScr_GetMaxActiveContracts()
{
    Scr_AddInt(3, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetIndexForActiveContract)
void __cdecl GScr_GetIndexForActiveContract(scr_entref_t entref)
{
    unsigned int IndexForActiveContract;
    gentity_s *playerEnt;
    unsigned int activeContractIndex;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        playerEnt = GetEntity(entref);
        if ( playerEnt->client )
        {
            activeContractIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
            IndexForActiveContract = LiveContracts_SV_GetIndexForActiveContract(playerEnt->s.number, activeContractIndex);
            Scr_AddInt(IndexForActiveContract, SCRIPTINSTANCE_SERVER);
        }
        else
        {
            Scr_Error("GetIndexForActiveContract: Entity must be a client", 0);
        }
    }
    else
    {
        Scr_Error("Invalid number of parameters supplied to GetActiveContractsForStat", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetContractStatType)
void GScr_GetContractStatType()
{
    VariableUnion varUnion;
    char *ContractStatType;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        varUnion.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        ContractStatType = LiveContracts_GetContractStatType(varUnion.intValue);
        Scr_AddString(ContractStatType, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_Error("Invalid number of parameters supplied to GetContractStatType", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetContractStatName)
void GScr_GetContractStatName()
{
    VariableUnion varUnion;
    char *ContractStatName;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        varUnion.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        ContractStatName = LiveContracts_GetContractStatName(varUnion.intValue);
        Scr_AddString(ContractStatName, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_Error("Invalid number of parameters supplied to GetContractStatName", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetContractRewardXP)
void GScr_GetContractRewardXP()
{
    VariableUnion varUnion;
    int ContractRewardXP;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        varUnion.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        ContractRewardXP = LiveContracts_GetContractRewardXP(varUnion.intValue);
        Scr_AddInt(ContractRewardXP, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_Error("Invalid number of parameters supplied to GetContractRewardXP", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetContractRewardCP)
void GScr_GetContractRewardCP()
{
    VariableUnion varUnion;
    int ContractRewardCP;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        varUnion.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        ContractRewardCP = LiveContracts_GetContractRewardCP(varUnion.intValue);
        Scr_AddInt(ContractRewardCP, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_Error("Invalid number of parameters supplied to GetContractRewardCP", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetContractRequirements)
void GScr_GetContractRequirements()
{
    char *reqData;
    char *reqType;
    signed int i;
    int contractIndex;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        contractIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        Scr_MakeArray(SCRIPTINSTANCE_SERVER);
        for ( i = 0; i < 5; ++i )
        {
            reqType = LiveContracts_GetRequirementType(contractIndex, i);
            if ( reqType )
            {
                Scr_AddString(reqType, SCRIPTINSTANCE_SERVER);
                Scr_AddArray(SCRIPTINSTANCE_SERVER);
                reqData = LiveContracts_GetRequirementData(contractIndex, i);
                if ( reqData )
                    Scr_AddString(reqData, SCRIPTINSTANCE_SERVER);
                else
                    Scr_AddString((char *)"", SCRIPTINSTANCE_SERVER);
                Scr_AddArray(SCRIPTINSTANCE_SERVER);
            }
        }
    }
    else
    {
        Scr_Error("Invalid number of parameters supplied to GetContractRequirements", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetContractName)
void GScr_GetContractName()
{
    char *ContractName;
    VariableUnion contractIndex;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        contractIndex.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        ContractName = LiveContracts_GetContractName(contractIndex.intValue);
        Scr_AddString(ContractName, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_Error("Invalid number of parameters supplied to GetContractName", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetContractRequiredCount)
void GScr_GetContractRequiredCount()
{
    int ContractRequiredCount;
    VariableUnion contractIndex;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        contractIndex.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        ContractRequiredCount = LiveContracts_GetContractRequiredCount(contractIndex.intValue);
        Scr_AddInt(ContractRequiredCount, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_Error("Invalid number of parameters supplied to GetContractRequiredCount", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetContractResetConditions)
void GScr_GetContractResetConditions()
{
    VariableUnion contractIndex;
    char *resetConditions;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        contractIndex.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        resetConditions = LiveContracts_GetResetConditions(contractIndex.intValue);
        if ( resetConditions )
            Scr_AddString(resetConditions, SCRIPTINSTANCE_SERVER);
        else
            Scr_AddString((char *)"", SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_Error("Invalid number of parameters supplied to GetContractResetConditions", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetActiveContractProgress)
void __cdecl GScr_GetActiveContractProgress(scr_entref_t entref)
{
    int ActiveContractProgress;
    gentity_s *playerEnt;
    unsigned int activeContractIndex;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        playerEnt = GetEntity(entref);
        if ( playerEnt->client )
        {
            activeContractIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
            ActiveContractProgress = LiveContracts_SV_GetActiveContractProgress(playerEnt->s.number, activeContractIndex);
            Scr_AddInt(ActiveContractProgress, SCRIPTINSTANCE_SERVER);
        }
        else
        {
            Scr_Error("GetActiveContractProgress: Entity must be a client", 0);
        }
    }
    else
    {
        Scr_Error("Invalid number of parameters supplied to GetActiveContractProgress", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IncrementActiveContractProgress)
void __cdecl GScr_IncrementActiveContractProgress(scr_entref_t entref)
{
    int increment;
    gentity_s *playerEnt;
    int activeContractIndex;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 2 )
    {
        playerEnt = GetEntity(entref);
        if ( playerEnt->client )
        {
            activeContractIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
            increment = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
            if ( increment > 0 )
                LiveContracts_SV_IncrementActiveContractProgress(playerEnt->s.number, activeContractIndex, increment);
            else
                Scr_Error("IncrementActiveContractProgress: <increment> must be >= 0", 0);
        }
        else
        {
            Scr_Error("IncrementActiveContractProgress: Entity must be a client", 0);
        }
    }
    else
    {
        Scr_Error("Invalid number of parameters supplied to IncrementActiveContractProgress", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ResetActiveContractProgress)
void __cdecl GScr_ResetActiveContractProgress(scr_entref_t entref)
{
    gentity_s *playerEnt;
    unsigned int activeContractIndex;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        playerEnt = GetEntity(entref);
        if ( playerEnt->client )
        {
            activeContractIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
            LiveContracts_SV_ResetActiveContractProgress(playerEnt->s.number, activeContractIndex);
        }
        else
        {
            Scr_Error("ResetActiveContractProgress: Entity must be a client", 0);
        }
    }
    else
    {
        Scr_Error("Invalid number of parameters supplied to ResetActiveContractProgress", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IncrementActiveContractTime)
void __cdecl GScr_IncrementActiveContractTime(scr_entref_t entref)
{
    int increment;
    gentity_s *playerEnt;
    int activeContractIndex;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 2 )
    {
        playerEnt = GetEntity(entref);
        if ( playerEnt->client )
        {
            activeContractIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
            increment = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
            if ( increment >= 0 )
                LiveContracts_SV_IncrementActiveContractTime(playerEnt->s.number, activeContractIndex, increment);
            else
                Scr_Error("IncrementActiveContractTime: <increment> must be >= 0", 0);
        }
        else
        {
            Scr_Error("IncrementActiveContractTime: Entity must be a client", 0);
        }
    }
    else
    {
        Scr_Error("Invalid number of parameters supplied to IncrementActiveContractTime", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsActiveContractComplete)
void __cdecl GScr_IsActiveContractComplete(scr_entref_t entref)
{
    gentity_s *playerEnt;
    unsigned int activeContractIndex;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        playerEnt = GetEntity(entref);
        if ( playerEnt->client )
        {
            activeContractIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
            if ( LiveContracts_SV_GetActiveContractStatus(playerEnt->s.number, activeContractIndex) == 2 )
                Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
            else
                Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
        }
        else
        {
            Scr_Error("IsActiveContractComplete: Entity must be a client", 0);
        }
    }
    else
    {
        Scr_Error("Invalid number of parameters supplied to IsActiveContractComplete", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_HasActiveContractExpired)
void __cdecl GScr_HasActiveContractExpired(scr_entref_t entref)
{
    gentity_s *playerEnt;
    unsigned int activeContractIndex;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        playerEnt = GetEntity(entref);
        if ( playerEnt->client )
        {
            activeContractIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
            if ( LiveContracts_SV_GetActiveContractStatus(playerEnt->s.number, activeContractIndex) == 3 )
                Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
            else
                Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
        }
        else
        {
            Scr_Error("HasActiveContractExpired: Entity must be a client", 0);
        }
    }
    else
    {
        Scr_Error("Invalid number of parameters supplied to HasActiveContractExpired", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetActiveContractTimePassed)
void __cdecl GScr_GetActiveContractTimePassed(scr_entref_t entref)
{
    int CombatTimePassed;
    gentity_s *playerEnt;
    unsigned int activeContractIndex;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        playerEnt = GetEntity(entref);
        if ( playerEnt->client )
        {
            activeContractIndex = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
            CombatTimePassed = LiveContracts_SV_GetCombatTimePassed(playerEnt->s.number, activeContractIndex);
            Scr_AddInt(CombatTimePassed, SCRIPTINSTANCE_SERVER);
        }
        else
        {
            Scr_Error("GetActiveContractTimePassed: Entity must be a client", 0);
        }
    }
    else
    {
        Scr_Error("Invalid number of parameters supplied to GetActiveContractTimePassed", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetFogSettings)
void GScr_GetFogSettings()
{
    unsigned int i;
    float settings[18];

    R_GetFogSettings(settings);
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for ( i = 0; i < 0x11; ++i )
    {
        Scr_AddFloat(settings[i], SCRIPTINSTANCE_SERVER);
        Scr_AddArray(SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_EnableOccluder)
void GScr_EnableOccluder()
{
    char *occluderName;
    int enable;

    occluderName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    enable = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    R_EnableOccluder(occluderName, enable != 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (Gscr_GetCustomClassLoadoutItem)
void Gscr_GetCustomClassLoadoutItem()
{
    VariableUnion varUnion;
    unsigned __int8 CustomClassLoadoutItemForSlot;
    char *String;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2 )
        Scr_Error("GetCustomClassLoadoutItem usage: <classnum>, <itemname>", 0);
    String = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    varUnion.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    CustomClassLoadoutItemForSlot = GetCustomClassLoadoutItemForSlot(0, varUnion.stringValue, String);
    Scr_AddInt(CustomClassLoadoutItemForSlot, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Gscr_GetCustomClassLoadoutModifier)
void Gscr_GetCustomClassLoadoutModifier()
{
    VariableUnion varUnion;
    int CustomClassModifierForClass;
    char *String;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2 )
        Scr_Error("GetCustomClassLoadoutItem usage: <classnum>, <itemname>", 0);
    String = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    varUnion.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    CustomClassModifierForClass = GetCustomClassModifierForClass(0, varUnion.stringValue, String);
    Scr_AddInt(CustomClassModifierForClass, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetDStat)
void __cdecl GScr_SetDStat(scr_entref_t entref)
{
    char *String;
    char *tempValue2;
    const char *fmtMsg;
    VariableUnion varUnion;
    char *tempValue5;
    unsigned __int64 tempValue6; // rax
    char *tempValue7;
    char *tempValue8;
    char *tempValue9;
    char *tempValue10;
    VariableUnion varUnion2;
    char *tempValue12;
    int tempValue13;
    char *tempValue14;
    VariableUnion varUnion3;
    int tempValue16;
    int Type;
    signed int i;
    gentity_s *playerEnt;
    ddlState_t searchState;
    int argc;

    searchState = *LiveStats_GetRootDDLState();
    playerEnt = GetEntity(entref);
    if ( !playerEnt->client )
        Scr_Error("setdstat: entity must be a player entity", 0);
    argc = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( argc > 8 )
        Scr_Error("setdstat: path should be at most 8 element deep", 0);
    for ( i = 0; i < argc - 1; ++i )
    {
        Type = Scr_GetType(i, SCRIPTINSTANCE_SERVER);
        if ( Type == 2 )
        {
            if ( searchState.member
                && searchState.member->arraySize > 1
                && searchState.member->enumIndex == -1
                && searchState.arrayIndex == -1 )
            {
                Scr_Error("setdstat: array index (integer) expected. Received a string instead.", 0);
            }
            String = Scr_GetString(i, SCRIPTINSTANCE_SERVER);
            if ( !DDL_MoveToName(&searchState, &searchState, String) )
            {
                tempValue2 = Scr_GetString(i, SCRIPTINSTANCE_SERVER);
                fmtMsg = va("setdstat: Could not find member name %s.", tempValue2);
                Scr_Error(fmtMsg, 0);
            }
        }
        else if ( Type == 6 )
        {
            if ( !searchState.member
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                            13672,
                            0,
                            "%s",
                            "searchState.member") )
            {
                __debugbreak();
            }
            if ( searchState.member->arraySize == 1 )
                Scr_Error("setdstat: member name (string) expected. Received an integer instead.", 0);
            varUnion.intValue = Scr_GetInt(i, SCRIPTINSTANCE_SERVER);
            if ( !DDL_MoveToIndex(&searchState, &searchState, varUnion.intValue, 1) )
                Scr_Error("setdstat: Could not find member array index number.", 0);
        }
        else
        {
            Scr_Error("setdstat: Expected strings or integers only.", 0);
        }
    }
    if ( searchState.member && searchState.member->arraySize > 1 && searchState.arrayIndex == -1 )
        Scr_Error("setdstat: trying to set a non leaf member of the ddl", 0);
    tempValue16 = Scr_GetType(argc - 1, SCRIPTINSTANCE_SERVER);
    if ( tempValue16 == 2 )
    {
        if ( searchState.member && searchState.member->type == 3 )
        {
            tempValue5 = Scr_GetString(argc - 1, SCRIPTINSTANCE_SERVER);
            tempValue6 = I_atoi64(tempValue5);
            SV_SetClientDInt64Stat(playerEnt->s.number, &searchState, tempValue6);
            if ( debugStats && debugStats->current.enabled )
            {
                tempValue7 = Scr_GetString(argc - 1, SCRIPTINSTANCE_SERVER);
                tempValue13 = I_atoi64(tempValue7);
                tempValue8 = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
                Com_Printf(16, "setStat %i %s %i\n", playerEnt->s.number, tempValue8, tempValue13);
            }
        }
        else if ( searchState.member && searchState.member->type == 5 )
        {
            tempValue9 = Scr_GetString(argc - 1, SCRIPTINSTANCE_SERVER);
            SV_SetClientDStringStat(playerEnt->s.number, &searchState, tempValue9);
            if ( debugStats && debugStats->current.enabled )
            {
                tempValue14 = Scr_GetString(argc - 1, SCRIPTINSTANCE_SERVER);
                tempValue10 = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
                Com_Printf(16, "setStat %i %s %i\n", playerEnt->s.number, tempValue10, tempValue14);
            }
        }
        else
        {
            Scr_Error("setdstat: member expects a string or 64 bit integer only. For regular integers, don't use quotes.", 0);
        }
    }
    else if ( tempValue16 == 6 )
    {
        if ( searchState.member && searchState.member->type <= 2u )
        {
            varUnion2.intValue = Scr_GetInt(argc - 1, SCRIPTINSTANCE_SERVER);
            SV_SetClientDIntStat(playerEnt->s.number, &searchState, varUnion2.stringValue);
            if ( debugStats && debugStats->current.enabled )
            {
                varUnion3.intValue = Scr_GetInt(argc - 1, SCRIPTINSTANCE_SERVER);
                tempValue12 = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
                Com_Printf(16, "setStat %i %s %i\n", playerEnt->s.number, tempValue12, varUnion3.intValue);
            }
        }
        else
        {
            Scr_Error("setdstat: member expects a string or 64 bit integers. Pass the value in quotes.", 0);
        }
    }
    else
    {
        Scr_Error("setdstat: Only string or integer values are acceptable.", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_UploadStats)
void GScr_UploadStats()
{
    const gentity_s *playerEnt;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        playerEnt = Scr_GetEntity(0);
        if ( playerEnt->client )
        {
            if ( playerEnt->s.number >= com_maxclients->current.integer
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                            13764,
                            0,
                            "%s",
                            "playerEnt->s.number < com_maxclients->current.integer") )
            {
                __debugbreak();
            }
            if ( playerEnt->s.number < com_maxclients->current.integer )
                SV_UploadStats(playerEnt->s.number);
        }
        else
        {
            Scr_Error("Non-player entity passed to UploadStats()", 0);
        }
    }
    else
    {
        SV_UploadStats();
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetItemAttachment)
void GScr_GetItemAttachment()
{
    eAttachment ItemAttachment;
    char *AttachmentName;
    VariableUnion attachmentNum;
    VariableUnion itemIndex;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2 )
        Scr_Error("GetItemAttachment( <itemIndex>, <attachmentNum> ) takes 2 parameters", 0);
    itemIndex.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    attachmentNum.intValue = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    ItemAttachment = BG_UnlockablesGetItemAttachment(itemIndex.intValue, attachmentNum.intValue);
    AttachmentName = (char *)BG_GetAttachmentName(ItemAttachment);
    Scr_AddString(AttachmentName, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetDefaultClassSlot)
void GScr_GetDefaultClassSlot()
{
    char *DefaultClassSlotFromName;
    char *className;
    char *slotName;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2 )
        Scr_Error("GetDefaultClassSlot( <classname>, <slot> ) takes 2 parameters", 0);
    className = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    slotName = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    DefaultClassSlotFromName = (char *)BG_UnlockablesGetDefaultClassSlotFromName(className, slotName);
    Scr_AddString(DefaultClassSlotFromName, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetTeamForTrigger)
void __cdecl GScr_SetTeamForTrigger(scr_entref_t entref)
{
    char *slStr;
    const char *fmtMsg;
    char *slStr2;
    const char *fmtMsg2;
    char *slStr3;
    char *slStr4;
    char *slStr5;
    char *slStr6;
    char *slStr7;
    unsigned __int16 team;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->classname != scr_const.trigger_use
        && ent->classname != scr_const.trigger_use_touch
        && ent->classname != scr_const.trigger_radius
        && ent->classname != scr_const.trigger_radius_use )
    {
        slStr6 = SL_ConvertToString(scr_const.trigger_radius_use, SCRIPTINSTANCE_SERVER);
        slStr4 = SL_ConvertToString(scr_const.trigger_radius, SCRIPTINSTANCE_SERVER);
        slStr3 = SL_ConvertToString(scr_const.trigger_use_touch, SCRIPTINSTANCE_SERVER);
        slStr = SL_ConvertToString(scr_const.trigger_use, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("setteamfortrigger: trigger entity must be of type %s, %s, %s or %s", slStr, slStr3, slStr4, slStr6);
        Scr_Error(fmtMsg, 0);
    }
    team = (unsigned __int16)Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    if ( team == scr_const.allies )
    {
        ent->team = 2;
    }
    else if ( team == scr_const.axis )
    {
        ent->team = 1;
    }
    else if ( team == scr_const.none )
    {
        ent->team = 0;
    }
    else
    {
        slStr7 = SL_ConvertToString(scr_const.none, SCRIPTINSTANCE_SERVER);
        slStr5 = SL_ConvertToString(scr_const.axis, SCRIPTINSTANCE_SERVER);
        slStr2 = SL_ConvertToString(scr_const.allies, SCRIPTINSTANCE_SERVER);
        fmtMsg2 = va("setteamfortrigger: invalid team used must be %s, %s or %s", slStr2, slStr5, slStr7);
        Scr_Error(fmtMsg2, 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetPerkForTrigger)
void __cdecl GScr_SetPerkForTrigger(scr_entref_t entref)
{
    char *slStr;
    const char *fmtMsg;
    const char *fmtMsg2;
    char *slStr2;
    char *slStr3;
    char *slStr4;
    char *perkName;
    unsigned int perkIndex;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->classname != scr_const.trigger_use
        && ent->classname != scr_const.trigger_use_touch
        && ent->classname != scr_const.trigger_radius
        && ent->classname != scr_const.trigger_radius_use )
    {
        slStr4 = SL_ConvertToString(scr_const.trigger_radius_use, SCRIPTINSTANCE_SERVER);
        slStr3 = SL_ConvertToString(scr_const.trigger_radius, SCRIPTINSTANCE_SERVER);
        slStr2 = SL_ConvertToString(scr_const.trigger_use_touch, SCRIPTINSTANCE_SERVER);
        slStr = SL_ConvertToString(scr_const.trigger_use, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("setperkfortrigger: trigger entity must be of type %s, %s, %s or %s", slStr, slStr2, slStr3, slStr4);
        Scr_Error(fmtMsg, 0);
    }
    perkName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    perkIndex = BG_GetPerkIndexForName(perkName);
    if ( perkIndex > 0xFF )
    {
        fmtMsg2 = va("setperkfortrigger: perk index '%d' is out of bounds for perk '%s'", perkIndex, perkName);
        Scr_Error(fmtMsg2, 0);
    }
    ent->trigger.perk = perkIndex;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetIgnoreEntForTrigger)
void __cdecl GScr_SetIgnoreEntForTrigger(scr_entref_t entref)
{
    char *slStr;
    const char *fmtMsg;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->classname != scr_const.trigger_radius_use )
    {
        slStr = SL_ConvertToString(scr_const.trigger_radius_use, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("setignoreentfortrigger: trigger entity must be of type %s", slStr);
        Scr_Error(fmtMsg, 0);
    }
    ent->s.otherEntityNum = Scr_GetEntity(0)->s.number;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ClientClaimTrigger)
void __cdecl GScr_ClientClaimTrigger(scr_entref_t entref)
{
    char *slStr;
    const char *fmtMsg;
    char *slStr2;
    char *slStr3;
    char *slStr4;
    gentity_s *clientEnt;
    gentity_s *triggerEnt;

    clientEnt = GetEntity(entref);
    if ( !clientEnt->client )
        Scr_Error("clientclaimtrigger: claimer must be a client.", 0);
    triggerEnt = Scr_GetEntity(0);
    if ( triggerEnt->classname != scr_const.trigger_use
        && triggerEnt->classname != scr_const.trigger_use_touch
        && triggerEnt->classname != scr_const.trigger_radius
        && triggerEnt->classname != scr_const.trigger_radius_use )
    {
        slStr4 = SL_ConvertToString(scr_const.trigger_radius_use, SCRIPTINSTANCE_SERVER);
        slStr3 = SL_ConvertToString(scr_const.trigger_radius, SCRIPTINSTANCE_SERVER);
        slStr2 = SL_ConvertToString(scr_const.trigger_use_touch, SCRIPTINSTANCE_SERVER);
        slStr = SL_ConvertToString(scr_const.trigger_use, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("clientclaimtrigger: trigger entity must be of type %s or %s or %s or %s", slStr, slStr2, slStr3, slStr4);
        Scr_Error(fmtMsg, 0);
    }
    if ( triggerEnt->item[1].ammoCount == 1023 || triggerEnt->item[1].ammoCount == clientEnt->client->ps.clientNum )
        triggerEnt->item[1].ammoCount = clientEnt->client->ps.clientNum;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ClientReleaseTrigger)
void __cdecl GScr_ClientReleaseTrigger(scr_entref_t entref)
{
    char *slStr;
    const char *fmtMsg;
    char *slStr2;
    char *slStr3;
    char *slStr4;
    gentity_s *clientEnt;
    gentity_s *triggerEnt;

    clientEnt = GetEntity(entref);
    if ( !clientEnt->client )
        Scr_Error("clientreleasetrigger: releaser must be a client.", 0);
    triggerEnt = Scr_GetEntity(0);
    if ( triggerEnt->classname != scr_const.trigger_use
        && triggerEnt->classname != scr_const.trigger_use_touch
        && triggerEnt->classname != scr_const.trigger_radius
        && triggerEnt->classname != scr_const.trigger_radius_use )
    {
        slStr4 = SL_ConvertToString(scr_const.trigger_radius_use, SCRIPTINSTANCE_SERVER);
        slStr3 = SL_ConvertToString(scr_const.trigger_radius, SCRIPTINSTANCE_SERVER);
        slStr2 = SL_ConvertToString(scr_const.trigger_use_touch, SCRIPTINSTANCE_SERVER);
        slStr = SL_ConvertToString(scr_const.trigger_use, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("clientreleasetrigger: trigger entity must be of type %s or %s or %s or %s", slStr, slStr2, slStr3, slStr4);
        Scr_Error(fmtMsg, 0);
    }
    if ( triggerEnt->item[1].ammoCount == clientEnt->client->ps.clientNum )
        triggerEnt->item[1].ammoCount = 1023;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ReleaseClaimedTrigger)
void __cdecl GScr_ReleaseClaimedTrigger(scr_entref_t entref)
{
    char *slStr;
    const char *fmtMsg;
    char *slStr2;
    gentity_s *triggerEnt;

    triggerEnt = GetEntity(entref);
    if ( triggerEnt->classname != scr_const.trigger_use && triggerEnt->classname != scr_const.trigger_use_touch )
    {
        slStr2 = SL_ConvertToString(scr_const.trigger_use_touch, SCRIPTINSTANCE_SERVER);
        slStr = SL_ConvertToString(scr_const.trigger_use, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("releaseclaimedtrigger: trigger entity must be of type %s or %s", slStr, slStr2);
        Scr_Error(fmtMsg, 0);
    }
    triggerEnt->item[1].ammoCount = 1023;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetMapCenter)
void GScr_SetMapCenter()
{
    float mapCenter[3];

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("Expected 1 argument to setMapCenter()", 0);
    Scr_GetVector(0, mapCenter, SCRIPTINSTANCE_SERVER);
    SV_SetMapCenter(mapCenter);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetDemoIntermissionPoint)
void GScr_SetDemoIntermissionPoint()
{
    float origin[3];
    float angles[3];

    if ( Demo_IsEnabled() && Demo_IsRecording() )
    {
        if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2 )
            Scr_Error("Expected 2 argument to SetDemoIntermissionPoint()", 0);
        Scr_GetVector(0, origin, SCRIPTINSTANCE_SERVER);
        Scr_GetVector(1u, angles, SCRIPTINSTANCE_SERVER);
        Demo_SetIntermissionPoint(origin, angles);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_StartDemoRecording)
void GScr_StartDemoRecording()
{
    if ( Demo_IsEnabled() )
    {
        if ( Demo_IsIdle() )
            Cbuf_AddText(0, "demo_startrecord\n");
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_StopDemoRecording)
void GScr_StopDemoRecording()
{
    if ( Demo_IsRecording() )
        Cbuf_AddText(0, "demo_stoprecord\n");
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsDemoRecording)
void GScr_IsDemoRecording()
{
    bool IsRecording;

    IsRecording = Demo_IsRecording();
    Scr_AddBool(IsRecording, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (isDemoEnabled)
void isDemoEnabled()
{
    bool IsEnabled;

    IsEnabled = Demo_IsEnabled();
    Scr_AddBool(IsEnabled, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_isTestClient)
void __cdecl GScr_isTestClient(scr_entref_t entref)
{
    bool IsTestClient;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_Error("istestclient: entity must be a player entity", 0);
    IsTestClient = SV_IsTestClient(ent->s.number);
    Scr_AddBool(IsTestClient, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_isDemoClient)
void __cdecl GScr_isDemoClient(scr_entref_t entref)
{
    bool IsDemoClient;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_Error("isdemoclient: entity must be a player entity", 0);
    IsDemoClient = SV_IsDemoClient(ent->s.number);
    Scr_AddBool(IsDemoClient, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetGameEndTime)
void GScr_SetGameEndTime()
{
    VariableUnion varUnion;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("Expected 1 argument to setGameEndTime()", 0);
    varUnion.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    SV_SetGameEndTime(varUnion.intValue);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetTimeScale)
void GScr_SetTimeScale()
{
    float endTimeScale;
    int intValue;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2 )
        Scr_Error("Expected 2 arguments to SetTimeScale()", 0);
    intValue = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    endTimeScale = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    SV_SetTimeScale(endTimeScale, intValue);
}

/* BlackOpsMP.retail.c sub_84C450 @ 0x84C450 — same argument order and quadrant check. */
// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetMiniMap)
void GScr_SetMiniMap()
{
    char *fmtMsg;
    float tempValue1;
    float tempValue2;
    float upperLeft;
    float upperLeft_4;
    char *material;
    char northYawString[32];
    float north[2];
    float lowerRight[2];

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 5 )
        Scr_Error("Expecting 5 arguments", 0);
    material = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    upperLeft = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    upperLeft_4 = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    lowerRight[0] = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
    lowerRight[1] = Scr_GetFloat(4u, SCRIPTINSTANCE_SERVER);
    SV_GetConfigstring(0x60Cu, northYawString, 32);
    tempValue1 = atof(northYawString);
    tempValue2 = tempValue1 * 0.017453292;
    north[0] = cos(tempValue2);
    north[1] = sin(tempValue2);
    level.compassMapWorldSize[0] = (float)((float)(lowerRight[0] - upperLeft) * north[1])
                                                             - (float)((float)(lowerRight[1] - upperLeft_4) * north[0]);
    level.compassMapWorldSize[1] = (float)((-(lowerRight[0] - upperLeft)) * north[0])
                                                             - (float)((float)(lowerRight[1] - upperLeft_4) * north[1]);
    if ( level.compassMapWorldSize[0] < 0.0 || level.compassMapWorldSize[1] < 0.0 )
        Scr_Error(
            "lower-right X and Y coordinates must be both south and east of upper-left X and Y coordinates in terms of the northyaw",
            0);
    level.compassMapUpperLeft[0] = upperLeft;
    level.compassMapUpperLeft[1] = upperLeft_4;
    fmtMsg = va("\"%s\" %f %f %f %f", material, upperLeft, upperLeft_4, lowerRight[0], lowerRight[1]);
    SV_SetConfigstring(1549, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IncrementEscrow)
void GScr_IncrementEscrow()
{
    const char *fmtMsg;
    char *xuidString;
    int amount;
    unsigned __int64 xuid;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 2 )
    {
        xuidString = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        StringToXUID(xuidString, &xuid);
        amount = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("%s %llu %d\n", "incrementescrow", xuid, amount);
        Cbuf_AddText(0, fmtMsg);
    }
    else
    {
        Scr_Error("Invalid number of parameters passed to IncrementEscrow", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetTeamSpyplane)
void GScr_SetTeamSpyplane()
{
    char *slStr;
    char *fmtMsg;
    unsigned __int16 team;
    unsigned int SpyplaneAvailable;

    team = (unsigned __int16)Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    if (team != scr_const.allies && team != scr_const.axis && team != scr_const.none)
    {
        slStr = SL_ConvertToString(team, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("Illegal team string '%s'. Must be allies, axis, or none.", slStr);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    SpyplaneAvailable = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    if (team == scr_const.allies)
    {
        level_tail.teamHasSpyplane[2] = SpyplaneAvailable;
    }
    else if (team == scr_const.axis)
    {
        level_tail.teamHasSpyplane[1] = SpyplaneAvailable;
    }
    else
    {
        if (team != scr_const.none
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                14352,
                0,
                "%s",
                "team == scr_const.none"))
        {
            __debugbreak();
        }
        level_tail.teamHasSpyplane[0] = SpyplaneAvailable;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetTeamSpyplane)
void GScr_GetTeamSpyplane()
{
    char *slStr;
    const char *fmtMsg;
    unsigned __int16 team;

    team = (unsigned __int16)Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    if ( team == scr_const.allies )
    {
        Scr_AddInt(level_tail.teamHasSpyplane[2], SCRIPTINSTANCE_SERVER);
    }
    else if ( team == scr_const.axis )
    {
        Scr_AddInt(level_tail.teamHasSpyplane[1], SCRIPTINSTANCE_SERVER);
    }
    else if ( team == scr_const.none )
    {
        Scr_AddInt(level_tail.teamHasSpyplane[0], SCRIPTINSTANCE_SERVER);
    }
    else
    {
        slStr = SL_ConvertToString(team, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("Illegal team string '%s'. Must be allies, axis, or none.", slStr);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetTeamSatellite)
void GScr_SetTeamSatellite()
{
    char *slStr;
    char *fmtMsg;
    unsigned __int16 team;
    unsigned int SatelliteAvailable;

    team = (unsigned __int16)Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    if (team != scr_const.allies && team != scr_const.axis && team != scr_const.none)
    {
        slStr = SL_ConvertToString(team, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("Illegal team string '%s'. Must be allies, axis, or none.", slStr);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    SatelliteAvailable = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    if (team == scr_const.allies)
    {
        level_tail.teamHasSatellite[2] = SatelliteAvailable;
    }
    else if (team == scr_const.axis)
    {
        level_tail.teamHasSatellite[1] = SatelliteAvailable;
    }
    else
    {
        if (team != scr_const.none
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                14431,
                0,
                "%s",
                "team == scr_const.none"))
        {
            __debugbreak();
        }
        level_tail.teamHasSatellite[0] = SatelliteAvailable;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetTeamSatellite)
void GScr_GetTeamSatellite()
{
    char *slStr;
    const char *fmtMsg;
    unsigned __int16 team;

    team = (unsigned __int16)Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    if ( team == scr_const.allies )
    {
        Scr_AddInt(level_tail.teamHasSatellite[2], SCRIPTINSTANCE_SERVER);
    }
    else if ( team == scr_const.axis )
    {
        Scr_AddInt(level_tail.teamHasSatellite[1], SCRIPTINSTANCE_SERVER);
    }
    else if ( team == scr_const.none )
    {
        Scr_AddInt(level_tail.teamHasSatellite[0], SCRIPTINSTANCE_SERVER);
    }
    else
    {
        slStr = SL_ConvertToString(team, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("Illegal team string '%s'. Must be allies, axis, or none.", slStr);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetArrayKeys)
void GScr_GetArrayKeys()
{
    const char *TypeName;
    const char *fmtMsg;
    VariableUnion id;

    if ( Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) != 20 )
    {
        TypeName = Scr_GetTypeName(0, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("Parameter (%s) must be an array", TypeName);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    id.intValue = Scr_GetObject(0, SCRIPTINSTANCE_SERVER);
    Scr_AddArrayKeys(id.stringValue, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_Launch)
void __cdecl GScr_Launch(scr_entref_t entref)
{
    const char *fmtMsg;
    float *aposTrDelta;
    float *trDelta;
    float avelocity[3];
    float velocity[3];
    gentity_s *ent;

    if ( !Scr_GetNumParam(SCRIPTINSTANCE_SERVER) || (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 2 )
        Scr_Error("Incorrect number of parameters\n", 0);
    ent = GetEntity(entref);
    Scr_GetVector(0, velocity, SCRIPTINSTANCE_SERVER);
    if ( (LODWORD(velocity[0]) & 0x7F800000) == 0x7F800000
        || (LODWORD(velocity[1]) & 0x7F800000) == 0x7F800000
        || (LODWORD(velocity[2]) & 0x7F800000) == 0x7F800000 )
    {
        fmtMsg = va("invalid velocity parameter in launch command: %f %f %f", velocity[0], velocity[1], velocity[2]);
        Scr_Error(fmtMsg, 0);
    }
    if ( ((LODWORD(velocity[0]) & 0x7F800000) == 0x7F800000
         || (LODWORD(velocity[1]) & 0x7F800000) == 0x7F800000
         || (LODWORD(velocity[2]) & 0x7F800000) == 0x7F800000)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    14561,
                    0,
                    "%s",
                    "!IS_NAN((velocity)[0]) && !IS_NAN((velocity)[1]) && !IS_NAN((velocity)[2])") )
    {
        __debugbreak();
    }
    ent->s.lerp.pos.trType = 6;
    ent->s.lerp.pos.trTime = level.time;
    trDelta = ent->s.lerp.pos.trDelta;
    ent->s.lerp.pos.trDelta[0] = velocity[0];
    trDelta[1] = velocity[1];
    trDelta[2] = velocity[2];
    if ( ((LODWORD(ent->s.lerp.pos.trDelta[0]) & 0x7F800000) == 0x7F800000
         || (LODWORD(ent->s.lerp.pos.trDelta[1]) & 0x7F800000) == 0x7F800000
         || (LODWORD(ent->s.lerp.pos.trDelta[2]) & 0x7F800000) == 0x7F800000)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    14571,
                    0,
                    "%s",
                    "!IS_NAN((ent->s.lerp.pos.trDelta)[0]) && !IS_NAN((ent->s.lerp.pos.trDelta)[1]) && !IS_NAN((ent->s.lerp.pos.trDelta)[2])") )
    {
        __debugbreak();
    }
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 1 )
    {
        Scr_GetVector(1u, avelocity, SCRIPTINSTANCE_SERVER);
        ent->s.lerp.apos.trType = 3;
        ent->s.lerp.apos.trTime = level.time;
        aposTrDelta = ent->s.lerp.apos.trDelta;
        ent->s.lerp.apos.trDelta[0] = avelocity[0];
        aposTrDelta[1] = avelocity[1];
        aposTrDelta[2] = avelocity[2];
        if ( ((LODWORD(ent->s.lerp.apos.trDelta[0]) & 0x7F800000) == 0x7F800000
             || (LODWORD(ent->s.lerp.apos.trDelta[1]) & 0x7F800000) == 0x7F800000
             || (LODWORD(ent->s.lerp.apos.trDelta[2]) & 0x7F800000) == 0x7F800000)
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                        14586,
                        0,
                        "%s",
                        "!IS_NAN((ent->s.lerp.apos.trDelta)[0]) && !IS_NAN((ent->s.lerp.apos.trDelta)[1]) && !IS_NAN((ent->s.lerp.apos.trDelta)[2])") )
        {
            __debugbreak();
        }
    }
    ent->physicsObject = 1;
    if ( ent->s.eType != 4 )
    {
        ent->r.contents = 0;
        SV_LinkEntity(ent);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_BulletTracer @ 825FBFD8)
void Scr_BulletTracer()
{
    float start[3];
    float end[3];
    gentity_s *ent;
    int force;

    Scr_GetVector(0, start, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, end, SCRIPTINSTANCE_SERVER);
    ent = G_TempEntity(start, EV_BULLET_HIT);
    ent->s.lerp.u.bulletHit.start[0] = end[0];
    ent->s.lerp.u.bulletHit.start[1] = end[1];
    ent->s.lerp.u.bulletHit.start[2] = end[2];
    force = 0;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 3 )
        force = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER) != 0;
    ent->s.eventParm = force;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_MagicBullet)
void __cdecl GScr_MagicBullet()
{
    const char *fmtMsg;
    const char *WeaponTypeName;
    const char *fmtMsg2;
    weapType_t weapType;
    float source[3];
    gentity_s *tempEnt;
    gentity_s *attacker;
    float dir[3];
    gentity_s *projectile;
    int weapon;
    const char *weapName;
    gentity_s *targetEnt;
    float angles[3];
    weaponParms wp;
    float dest[3];
    float targetOffset[3];
    float vecIn[3];

    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 3 )
        Scr_Error("MagicBullet weaponName sourceLoc destLoc.\n", 0);
    weapName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weapon = G_GetWeaponIndexForName((char *)weapName);
    if ( !weapon )
    {
        fmtMsg = va("MagicBullet called with unknown weapon name %s\n", weapName);
        Scr_Error(fmtMsg, 0);
    }
    Scr_GetVector(1u, vecIn, SCRIPTINSTANCE_SERVER);
    source[0] = vecIn[0];
    source[1] = vecIn[1];
    source[2] = vecIn[2];
    Scr_GetVector(2u, vecIn, SCRIPTINSTANCE_SERVER);
    dest[0] = vecIn[0];
    dest[1] = vecIn[1];
    dest[2] = vecIn[2];
    Weapon_SetWeaponParamsWeapon(&wp, weapon);
    wp.muzzleTrace[0] = source[0];
    wp.muzzleTrace[1] = source[1];
    wp.muzzleTrace[2] = source[2];
    dir[0] = dest[0] - source[0];
    dir[1] = dest[1] - source[1];
    dir[2] = dest[2] - source[2];
    Vec3Normalize(dir);
    wp.forward[0] = dir[0];
    wp.forward[1] = dir[1];
    wp.forward[2] = dir[2];
    memset(wp.right, 0, 24);
    memset(targetOffset, 0, sizeof(targetOffset));
    if ( wp.weapDef->weapType == WEAPTYPE_GRENADE || wp.weapDef->weapType == WEAPTYPE_MINE )
        Scr_Error("MagicBullet() does not work with grenade-type weapons.\n", 0);
    attacker = &g_entities[1022];
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) >= 4 && Scr_GetType(3u, SCRIPTINSTANCE_SERVER) )
        attacker = Scr_GetEntity(3u);
    targetEnt = 0;
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) >= 5 )
        targetEnt = Scr_GetEntity(4u);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) >= 6 )
        Scr_GetVector(5u, targetOffset, SCRIPTINSTANCE_SERVER);
    projectile = 0;
    weapType = wp.weapDef->weapType;
    if ( weapType )
    {
        if ( weapType == WEAPTYPE_PROJECTILE )
        {
            switch ( wp.weapDef->weapClass )
            {
                case WEAPCLASS_GRENADE:
                    projectile = Weapon_GrenadeLauncher_Fire(attacker, weapon, 0, &wp);
                    break;
                case WEAPCLASS_ROCKETLAUNCHER:
                    projectile = Weapon_RocketLauncher_Fire(
                                                 attacker,
                                                 weapon,
                                                 0.0,
                                                 &wp,
                                                 vec3_origin,
                                                 targetEnt,
                                                 targetOffset);
                    break;
                case WEAPCLASS_TURRET:
                    if ( wp.weapDef->weapType == WEAPTYPE_PROJECTILE )
                    {
                        projectile = Weapon_RocketLauncher_Fire(
                                                     attacker,
                                                     weapon,
                                                     0.0,
                                                     &wp,
                                                     vec3_origin,
                                                     targetEnt,
                                                     targetOffset);
                    }
                    else if ( wp.weapDef->weapType == WEAPTYPE_BULLET )
                    {
                        Bullet_Fire(attacker, 0.0, &wp, 0, level.time);
                    }
                    break;
                default:
                    Scr_Error("MagicBullet(): Unhandled projectile weapClass.\n", 0);
                    break;
            }
        }
        else
        {
            WeaponTypeName = BG_GetWeaponTypeName(wp.weapDef->weapType);
            fmtMsg2 = va("MagicBullet(): Unhandled weapType \"%s\".\n", WeaponTypeName);
            Scr_Error(fmtMsg2, 0);
        }
    }
    else
    {
        Bullet_Fire(attacker, 0.0, &wp, 0, level.time);
    }
    tempEnt = G_TempEntity(source, EV_FIRE_WEAPON);
    vectoangles(dir, angles);
    G_SetAngle(tempEnt, angles);
    tempEnt->s.weapon = weapon;
    if ( tempEnt->s.weapon != weapon
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    14680,
                    0,
                    "%s",
                    "tempEnt->s.weapon == weapon") )
    {
        __debugbreak();
    }
    tempEnt->s.eventParms[tempEnt->s.eventSequence & 3] = 0;
    if ( attacker )
        tempEnt->s.eventParm = attacker->s.number;
    if ( projectile )
        Scr_AddEntity(projectile, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_LaunchBomb)
void __cdecl GScr_LaunchBomb(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    float velocity[3];
    float dir[3];
    int iWeaponIndex;
    weaponParms wp;
    float targetOffset[3];
    gentity_s *ent;
    const char *pszWeaponName;
    gentity_s *player;

    player = GetEntity(entref);
    if ( !player->client )
        Scr_Error("LaunchBomb: entity must be a player entity", 0);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 3 )
    {
        pszWeaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        iWeaponIndex = G_GetWeaponIndexForName((char *)pszWeaponName);
        if ( !iWeaponIndex )
        {
            if ( *pszWeaponName )
            {
                fmtMsg = va("Invalid weapon name %s", pszWeaponName);
                Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
            }
            else
            {
                Scr_ParamError(0, "Invalid weapon name", SCRIPTINSTANCE_SERVER);
            }
        }
        Weapon_SetWeaponParamsWeapon(&wp, iWeaponIndex);
        if ( wp.weapDef->weapType
            && wp.weapDef->weapType != WEAPTYPE_PROJECTILE
            && wp.weapDef->weapType != WEAPTYPE_BOMB
            && wp.weapDef->weapType != WEAPTYPE_GAS )
        {
            fmtMsg2 = va("LaunchBomb only support bullet, bomb, gas and projectile weapons\n");
            Scr_Error(fmtMsg2, 0);
        }
        Scr_GetVector(1u, wp.muzzleTrace, SCRIPTINSTANCE_SERVER);
        Scr_GetVector(2u, velocity, SCRIPTINSTANCE_SERVER);
        memset(targetOffset, 0, sizeof(targetOffset));
        dir[0] = velocity[0];
        dir[1] = velocity[1];
        dir[2] = velocity[2];
        Vec3Normalize(dir);
        ent = G_DropBomb(player, iWeaponIndex, wp.muzzleTrace, dir, velocity, 0, targetOffset);
        Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_Error("illegal call to LaunchBomb(<weapon>, <position>, <velocity>)", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_MakeGrenadeDud)
void __cdecl GScr_MakeGrenadeDud(scr_entref_t entref)
{
    gentity_s *grenade;

    grenade = GetEntity(entref);
    if ( grenade->s.eType == 4 )
    {
        if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
            Scr_Error("illegal call to MakeGrenadeDud()", 0);
        else
            grenade->s.lerp.u.turret.ownerNum = 1;
    }
    else
    {
        Scr_Error("MakeGrenadeDud: entity must be a grenade entity", 0);
    }
}

#ifdef KISAK_SP
// Decomp: CoDSP_rdBlackOps.map.c (GScrCmd_ResetMissileDetonationTime ~8260B580)
// Map: CoDSP_rd.map VA 0x8260B580 (g_scr_main.obj)
void __cdecl GScrCmd_ResetMissileDetonationTime(scr_entref_t entref)
{
    gentity_s *ent;
    const WeaponDef *weapDef;
    gentity_s *owner;

    ent = GetEntity(entref);
    if ( ent->s.eType != ET_MISSILE || !ent->s.weapon )
        return;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        ent->nextthink = level.time + (int)(Scr_GetFloat(0, SCRIPTINSTANCE_SERVER) * 1000.0f);
        return;
    }
    weapDef = BG_GetWeaponDef(ent->s.weapon);
    if ( !weapDef || !weapDef->timedDetonation )
        return;
    owner = ent->r.ownerNum.ent();
    if ( ent->r.ownerNum.isDefined() && owner && owner->client )
        ent->nextthink = level.time + weapDef->fuseTime;
    else
        ent->nextthink = level.time + weapDef->aiFuseTime;
}
#endif

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsOnLadder)
void __cdecl GScr_IsOnLadder(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_Error("setstat: entity must be a player entity", 0);
    Scr_AddBool((ent->client->ps.pm_flags & 8) != 0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsMantling)
void __cdecl GScr_IsMantling(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_Error("setstat: entity must be a player entity", 0);
    Scr_AddBool((ent->client->ps.pm_flags & 4) != 0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_StartDoorBreach)
void __cdecl GScr_StartDoorBreach(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_ObjectError("Not a player entity", SCRIPTINSTANCE_SERVER);
    ent->client->ps.pm_flags |= 0x1000000u;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_StopDoorBreach)
void __cdecl GScr_StopDoorBreach(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_ObjectError("Not a player entity", SCRIPTINSTANCE_SERVER);
    ent->client->ps.pm_flags &= ~0x1000000u;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetLightColor)
void __cdecl GScr_GetLightColor(scr_entref_t entref)
{
    gentity_s *ent;
    float unpackedColor[4];

    ent = GScr_SetupLightEntity(entref);
    Byte4UnpackRgba((const unsigned __int8 *)&ent->s.lerp.u, unpackedColor);
    Scr_AddVector(unpackedColor, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetupLightEntity)
gentity_s *__cdecl GScr_SetupLightEntity(scr_entref_t entref)
{
    char *slStr;
    const char *fmtMsg;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 14876, 0, "%s", "ent") )
        __debugbreak();
    if ( ent->s.eType != 10 )
    {
        slStr = SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("Function can only be called on a 'light' entity; actual classname is '%s'\n", slStr);
        Scr_Error(fmtMsg, 0);
    }
    return ent;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetLightColor)
void __cdecl GScr_SetLightColor(scr_entref_t entref)
{
    unsigned __int8 exponent;
    gentity_s *ent;
    float unpackedColor[4];

    ent = GScr_SetupLightEntity(entref);
    Scr_GetVector(0, unpackedColor, SCRIPTINSTANCE_SERVER);
    unpackedColor[3] = 0.0f;
    exponent = ent->s.lerp.u.primaryLight.colorAndExp[3];
    Byte4PackRgba(unpackedColor, (unsigned __int8 *)&ent->s.lerp.u);
    ent->s.lerp.u.primaryLight.colorAndExp[3] = exponent;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetLightIntensity)
void __cdecl GScr_GetLightIntensity(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GScr_SetupLightEntity(entref);
    Scr_AddFloat(ent->s.lerp.u.turret.gunAngles[1], SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetLightIntensity)
void __cdecl GScr_SetLightIntensity(scr_entref_t entref)
{
    int intensityInt;
    float intensity;
    gentity_s *ent;

    ent = GScr_SetupLightEntity(entref);
    intensity = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    if ( intensity < -0.001 )
        Scr_ParamError(0, "intensity must be >= 0", SCRIPTINSTANCE_SERVER);
    if ( (float)(intensity - 0.0) < 0.0 )
        intensityInt = 0;
    else
        intensityInt = LODWORD(intensity);
    ent->s.lerp.u.loopFx.period = intensityInt;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetLightRadius)
void __cdecl GScr_GetLightRadius(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GScr_SetupLightEntity(entref);
    Scr_AddFloat(ent->s.lerp.u.turret.gunAngles[2], SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetLightRadius)
void __cdecl GScr_SetLightRadius(scr_entref_t entref)
{
    const char *fmtMsg;
    int radiusInt;
    float clampedRadius;
    float radius;
    gentity_s *ent;
    const ComPrimaryLight *refLight;

    ent = GScr_SetupLightEntity(entref);
    refLight = Com_GetPrimaryLight(ent->s.index.brushmodel);
    if ( !refLight
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 15013, 0, "%s", "refLight") )
    {
        __debugbreak();
    }
    radius = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    if ( radius >= -0.001 )
    {
        if ( radius > (float)(refLight->radius + 0.001) )
        {
            fmtMsg = va("radius must be less than the bsp radius (%g)", refLight->radius);
            Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    else
    {
        Scr_ParamError(0, "radius must be >= 0", SCRIPTINSTANCE_SERVER);
    }
    if ( (float)(radius - refLight->radius) < 0.0 )
        clampedRadius = radius;
    else
        clampedRadius = refLight->radius;
    if ( (float)(0.0 - radius) < 0.0 )
        radiusInt = LODWORD(clampedRadius);
    else
        radiusInt = 0;
    ent->s.lerp.u.actor.team = radiusInt;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetLightFovInner)
void __cdecl GScr_GetLightFovInner(scr_entref_t entref)
{
    gentity_s *ent = GScr_SetupLightEntity(entref);
    float innerCos = ent->s.lerp.u.primaryLight.cosHalfFovInner;
    float fov = acosf(innerCos) * 2.0f;

    Scr_AddFloat(fov, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetLightFovOuter)
void __cdecl GScr_GetLightFovOuter(scr_entref_t entref)
{
    gentity_s *ent = GScr_SetupLightEntity(entref);
    float outerCos = ent->s.lerp.u.primaryLight.cosHalfFovOuter;
    float fov = acosf(outerCos) * 2.0f;

    Scr_AddFloat(fov, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetLightFovRange)
void __cdecl GScr_SetLightFovRange(scr_entref_t entref)
{
    const char *fmtMsg;
    float clampedInnerCos;
    float clampedOuterCos;
    float cosHalfFovInner;
    float cosHalfFovOuter;
    float fovInner;
    float fovOuter;
    gentity_s *ent;
    const ComPrimaryLight *refLight;

    ent = GScr_SetupLightEntity(entref);
    refLight = Com_GetPrimaryLight(ent->s.index.brushmodel);
    if ( !refLight
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 15091, 0, "%s", "refLight") )
    {
        __debugbreak();
    }

    fovOuter = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    if ( fovOuter < 0.99900001 || fovOuter >= 136.00101 )
        Scr_ParamError(0, "outer fov must be in the range of 1 to 136", SCRIPTINSTANCE_SERVER);

    // cos(half-angle) where input fov is in degrees.
    cosHalfFovOuter = cosf((fovOuter * 0.017453292f) * 0.5f);
    if ( (float)(refLight->cosHalfFovOuter - 0.001f) > cosHalfFovOuter )
        Scr_ParamError(0, "outer fov cannot be larger than the fov when the map was compiled", SCRIPTINSTANCE_SERVER);

    clampedOuterCos = cosHalfFovOuter > 1.0f ? 1.0f : cosHalfFovOuter;
    if ( clampedOuterCos < refLight->cosHalfFovOuter )
        clampedOuterCos = refLight->cosHalfFovOuter;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 2 )
    {
        fovInner = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
        if ( fovInner < -0.001f || fovInner >= (float)(fovOuter + 0.001f) )
            Scr_ParamError(1u, "inner fov must be in the range of 0 to outer fov", SCRIPTINSTANCE_SERVER);

        cosHalfFovInner = cosf((fovInner * 0.017453292f) * 0.5f);
        clampedInnerCos = cosHalfFovInner > 1.0f ? 1.0f : cosHalfFovInner;
        if ( clampedInnerCos <= (float)(clampedOuterCos + 0.001f) )
            clampedInnerCos = clampedOuterCos + 0.001f;
    }
    else
    {
        if ( refLight->cosHalfFovInner < (float)(clampedOuterCos + 0.001f) )
            clampedInnerCos = refLight->cosHalfFovInner;
        else
            clampedInnerCos = clampedOuterCos + 0.001f;
    }

    if ( clampedOuterCos <= 0.0f || clampedInnerCos <= clampedOuterCos || clampedInnerCos > 1.0f )
    {
        fmtMsg = va("%g, %g", clampedOuterCos, clampedInnerCos);
        if ( !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                        15114,
                        0,
                        "%s\n\t%s",
                        "0.0f < cosHalfFovOuter && cosHalfFovOuter < cosHalfFovInner && cosHalfFovInner <= 1.0f",
                        fmtMsg) )
        {
            __debugbreak();
        }
    }

    ent->s.lerp.u.primaryLight.cosHalfFovOuter = clampedOuterCos;
    ent->s.lerp.u.turret.heatVal = clampedInnerCos;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetLightExponent)
void __cdecl GScr_GetLightExponent(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GScr_SetupLightEntity(entref);
    Scr_AddInt(ent->s.lerp.u.primaryLight.colorAndExp[3], SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetLightExponent)
void __cdecl GScr_SetLightExponent(scr_entref_t entref)
{
    int exponent;
    gentity_s *ent;

    ent = GScr_SetupLightEntity(entref);
    exponent = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)exponent > 0x64 )
        Scr_ParamError(0, "exponent must be in the range from 0 to 100", SCRIPTINSTANCE_SERVER);
    ent->s.lerp.u.primaryLight.colorAndExp[3] = exponent;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_StartRagdoll)
void __cdecl GScr_StartRagdoll(scr_entref_t entref)
{
    unsigned __int8 aposTrType;
    unsigned __int8 trType;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    trType = ent->s.lerp.pos.trType;
    if ( trType == 1 )
    {
        ent->s.lerp.pos.trType = 14;
    }
    else if ( trType == 6 )
    {
        ent->s.lerp.pos.trType = 13;
    }
    else
    {
        ent->s.lerp.pos.trType = 12;
    }
    aposTrType = ent->s.lerp.apos.trType;
    if ( aposTrType == 1 )
    {
        ent->s.lerp.apos.trType = 14;
    }
    else if ( aposTrType == 6 )
    {
        ent->s.lerp.apos.trType = 13;
    }
    else
    {
        ent->s.lerp.apos.trType = 12;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsRagdoll)
void __cdecl GScr_IsRagdoll(scr_entref_t entref)
{
    bool IsRagdollTrajectory;
    gentity_s *ent;

    ent = GetEntity(entref);
    IsRagdollTrajectory = Com_IsRagdollTrajectory(&ent->s.lerp.pos);
    Scr_AddInt(IsRagdollTrajectory, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_RagdollLaunch)
void __cdecl GScr_RagdollLaunch(scr_entref_t entref)
{
    unsigned int hitLocationString;
    gentity_s *tempent;
    float force[3];
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( Com_IsRagdollTrajectory(&ent->s.lerp.pos) )
    {
        Scr_GetVector(0, force, SCRIPTINSTANCE_SERVER);
        tempent = G_TempEntity(force, EV_PHYS_LAUNCH);
        tempent->s.otherEntityNum = ent->s.number;
        if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 1 )
        {
            tempent->s.eventParm = 0;
        }
        else
        {
            hitLocationString = Scr_GetConstLowercaseString(1u, SCRIPTINSTANCE_SERVER);
            tempent->s.eventParm = G_GetHitLocationIndexFromString(hitLocationString);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_VehicleLaunch)
void __cdecl GScr_VehicleLaunch(scr_entref_t entref)
{
    float *currentOrigin;
    gentity_s *tempent;
    float force[3];
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->scr_vehicle && ent->scr_vehicle->nitrousVehicle )
    {
        Scr_GetVector(0, force, SCRIPTINSTANCE_SERVER);
        tempent = G_TempEntity(force, EV_PHYS_LAUNCH);
        tempent->s.otherEntityNum = ent->s.number;
        tempent->s.eventParm = 0;
        if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 2 )
            tempent->s.eventParm = (unsigned __int16)Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
        if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 1 )
        {
            tempent->s.lerp.pos.trDelta[0] = 0.0f;
            tempent->s.lerp.pos.trDelta[1] = 0.0f;
            tempent->s.lerp.pos.trDelta[2] = 0.0f;
        }
        else
        {
            Scr_GetVector(1u, tempent->s.lerp.pos.trDelta, SCRIPTINSTANCE_SERVER);
            if ( !tempent->s.eventParm )
            {
                currentOrigin = ent->r.currentOrigin;
                tempent->s.lerp.pos.trDelta[0] = tempent->s.lerp.pos.trDelta[0] - ent->r.currentOrigin[0];
                tempent->s.lerp.pos.trDelta[1] = tempent->s.lerp.pos.trDelta[1] - currentOrigin[1];
                tempent->s.lerp.pos.trDelta[2] = tempent->s.lerp.pos.trDelta[2] - currentOrigin[2];
            }
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GiveAchievement)
void __cdecl GScr_GiveAchievement(scr_entref_t entref)
{
    const char *fmtMsg;
    char *achievement;
    gentity_s *ent;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_ParamError(0, "giveachievement [name]", SCRIPTINSTANCE_SERVER);
    achievement = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( !*achievement )
        Scr_Error("giveachievement: achievement must be named", 0);
    ent = GetEntity(entref);
    if ( !ent || !ent->client )
        Scr_Error("giveachievement: entity must be a player entity", 0);
    fmtMsg = va("%c %s", 56, achievement);
    SV_GameSendServerCommand(ent->s.number, SV_CMD_RELIABLE, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c @ 8261E4D8 (GScr_GiveGamerPicture)
void __cdecl GScr_GiveGamerPicture(scr_entref_t entref)
{
    const char *fmtMsg;
    char *pictureName;
    gentity_s *ent;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_ParamError(0, "givegamerpicture [name]", SCRIPTINSTANCE_SERVER);
    pictureName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( !pictureName || !*pictureName )
        Scr_Error("givegamerpicture: picture must be named", 0);
    ent = GetEntity(entref);
    if ( !ent || !ent->client )
        Scr_Error("givegamerpicture: entity must be a player entity", 0);
    fmtMsg = va("%c %s", 126, pictureName);
    SV_GameSendServerCommand(ent->s.number, SV_CMD_RELIABLE, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetOwner)
void __cdecl GScr_SetOwner(scr_entref_t entref)
{
    gentity_s *owner;
    gentity_s *ent;
    unsigned int ownerIndex;

    ent = GetEntity(entref);
    if ( ent->s.eType == 1 || ent->s.eType == 17 )
        Scr_Error("SetOwner can not be used for players or actors", 0);
    owner = Scr_GetEntity(0);
    if ( !owner->client
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    15353,
                    0,
                    "%s",
                    "owner->client") )
    {
        __debugbreak();
    }
    ownerIndex = owner->client - level.clients;
    if ( ownerIndex >= level.maxclients
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    15355,
                    0,
                    "ownerIndex doesn't index level.maxclients\n\t%i not in [0, %i)",
                    ownerIndex,
                    level.maxclients) )
    {
        __debugbreak();
    }
    ent->s.faction.iHeadIconTeam = ent->s.faction.iHeadIconTeam & 3 | (4 * ownerIndex);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetTurretOwner)
void __cdecl GScr_SetTurretOwner(scr_entref_t entref)
{
    gentity_s *owner;
    gentity_s *ent;
    unsigned int ownerIndex;

    ent = GetEntity(entref);
    if ( ent->s.eType == 1 || ent->s.eType == 17 )
        Scr_Error("SetOwner can not be used for players or actors", 0);
    owner = Scr_GetEntity(0);
    if ( !owner->client
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    15383,
                    0,
                    "%s",
                    "owner->client") )
    {
        __debugbreak();
    }
    ownerIndex = owner->client - level.clients;
    if ( ownerIndex >= level.maxclients
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    15385,
                    0,
                    "ownerIndex doesn't index level.maxclients\n\t%i not in [0, %i)",
                    ownerIndex,
                    level.maxclients) )
    {
        __debugbreak();
    }
    ent->s.faction.iHeadIconTeam = ent->s.faction.iHeadIconTeam & 3 | (4 * ownerIndex);
    ent->s.lerp.u.turret.ownerNum = ownerIndex;
    Turret_SetTurretOwner(ent, owner);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetTurretType)
void __cdecl GScr_SetTurretType(scr_entref_t entref)
{
    const char *fmtMsg;
    char *pszType;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->pTurretInfo
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    15412,
                    0,
                    "%s",
                    "ent->pTurretInfo") )
    {
        __debugbreak();
    }
    pszType = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    ent->pTurretInfo->flags &= ~0x10000u;
    ent->pTurretInfo->flags &= ~0x20000u;
    ent->s.lerp.u.turret.flags &= ~1u;
    ent->s.lerp.u.turret.flags &= ~2u;
    if ( I_stricmp(pszType, "sentry") )
    {
        if ( I_stricmp(pszType, "tow") )
        {
            fmtMsg = va("%s, Unknown turret type.", pszType);
            Scr_Error(SCRIPTINSTANCE_SERVER, fmtMsg, 0);
        }
        else
        {
            ent->pTurretInfo->flags |= 0x20000u;
            ent->s.lerp.u.turret.flags |= 2u;
        }
    }
    else
    {
        ent->pTurretInfo->flags |= 0x10000u;
        ent->s.lerp.u.turret.flags |= 1u;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetTeam)
void __cdecl GScr_SetTeam(scr_entref_t entref)
{
    const char *fmtMsg;
    char team;
    gentity_s *ent;
    char *pszTeam;

    ent = GetEntity(entref);
    if ( ent->s.eType == 1 || ent->s.eType == 17 )
        Scr_Error("SetTeam can not be used for players or actors", 0);
    pszTeam = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    team = 0;
    if ( I_stricmp(pszTeam, "axis") )
    {
        if ( I_stricmp(pszTeam, "allies") )
        {
            if ( I_stricmp(pszTeam, "spectator") )
            {
                if ( I_stricmp(pszTeam, "free") )
                {
                    fmtMsg = va("unknown team '%s', should be axis, allies, or neutral\n", pszTeam);
                    Scr_Error(fmtMsg, 0);
                }
                else
                {
                    team = 0;
                }
            }
            else
            {
                team = 3;
            }
        }
        else
        {
            team = 2;
        }
    }
    else
    {
        team = 1;
    }
    ent->s.faction.iHeadIconTeam = team | (4 * ((int)ent->s.faction.iHeadIconTeam >> 2));
}

// Decomp: BlackOps.singleplayer.c sub_806BF0 (ScrCmd_SetPhysParams)
void __cdecl ScrCmd_SetPhysParams(scr_entref_t entref)
{
    gentity_s *ent;
    actor_s *actor;
    float halfWidth;
    float minZ;
    float maxZ;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 3 )
        Scr_Error("setphysparams takes three parameters\n", 0);
    ent = GetEntity(entref);
    actor = ent->actor;
    if ( !actor )
        Scr_Error("setphysparams must be called on an AI only.", 0);
    halfWidth = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    minZ = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    maxZ = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    actor->Physics.vMins[0] = -halfWidth;
    actor->Physics.vMins[1] = -halfWidth;
    actor->Physics.vMins[2] = minZ;
    actor->Physics.vMaxs[0] = halfWidth;
    actor->Physics.vMaxs[1] = halfWidth;
    actor->Physics.vMaxs[2] = maxZ;
}

// Decomp: BlackOps.singleplayer.c (GScr_SetSpawnerTeam)
void __cdecl GScr_SetSpawnerTeam(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->s.eType != ET_ACTOR_SPAWNER )
        Scr_Error("setspawnerteam can only be called on actor spawners", 0);
    ent->spawner.team = GScr_ReadTeamForSpawnPoints(0);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetTeam)
void __cdecl GScr_GetTeam(scr_entref_t entref)
{
    team_t team;
    gentity_s *ent;

    team = TEAM_FREE;
    ent = GetEntity(entref);
    switch ( ent->s.eType )
    {
        case 1:
            team = ent->client->sess.cs.team;
            break;
        case 4:
            team = ent->missile.team;
            break;
        case 0xE:
            team = G_GetVehicleOccupantsTeam(ent);
            break;
        case 0x11:
            team = ent->sentient->eTeam;
            break;
        default:
            break;
    }
    switch ( team )
    {
        case TEAM_AXIS:
            Scr_AddString("axis", SCRIPTINSTANCE_SERVER);
            break;
        case TEAM_ALLIES:
            Scr_AddString("allies", SCRIPTINSTANCE_SERVER);
            break;
        case TEAM_SPECTATOR:
            Scr_AddString("spectator", SCRIPTINSTANCE_SERVER);
            break;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetCorpseAnim)
void __cdecl GScr_GetCorpseAnim(scr_entref_t entref)
{
    const char *fmtMsg;
    XAnim_s *treeAnims;
    gentity_s *ent;
    scr_anim_s anim;
    corpseInfo_t *corpseInfo;

    ent = GetEntity(entref);
    if ( ent->s.eType == 2 )
    {
        corpseInfo = &g_scr_data.playerCorpseInfo[G_GetPlayerCorpseIndex(ent, "GScr_GetCorpseAnim")];
        anim.index = ent->s.animState.state & 0xFFFFFFBF;
        treeAnims = XAnimGetAnims(corpseInfo->tree);
        anim.tree = Scr_GetAnimsIndex(treeAnims, SCRIPTINSTANCE_SERVER);
        Scr_AddAnim(anim, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        fmtMsg = va("Only valid on player corpses");
        Scr_Error(fmtMsg, 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_ItemWeaponSetAmmo)
void __cdecl ScrCmd_ItemWeaponSetAmmo(scr_entref_t entref)
{
    const char *fmtMsg;
    VariableUnion varUnion;
    int ClipSize;
    int reserveAmmo;
    int clipAmmo;
    unsigned int altIndex;
    signed int weaponIndex;
    gentity_s *itemEnt;

    itemEnt = GetEntity(entref);
    if ( itemEnt->s.eType != 3 )
        Scr_Error("Entity is not an item.", 0);
    if ( bg_itemlist[itemEnt->s.un3.item].giType != IT_WEAPON )
        Scr_Error("Item entity is not a weapon.", 0);
    clipAmmo = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( clipAmmo < 0 )
        Scr_ParamError(0, "Ammo count must not be negative", SCRIPTINSTANCE_SERVER);
    reserveAmmo = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    if ( reserveAmmo < 0 )
        Scr_ParamError(1u, "Ammo count must not be negative", SCRIPTINSTANCE_SERVER);
    altIndex = 0;
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 2 )
    {
        altIndex = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
        if ( altIndex >= 2 )
        {
            fmtMsg = va("Value out of range.    Allowed values: 0 to %i", 2);
            Scr_ParamError(2u, fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    weaponIndex = itemEnt->item[altIndex].index % 2048;
    if ( weaponIndex > 0 )
    {
        if ( BG_GetClipSize(weaponIndex) < 0
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                        15598,
                        0,
                        "%s",
                        "BG_GetClipSize( weaponIndex ) >= 0") )
        {
            __debugbreak();
        }
        ClipSize = BG_GetClipSize(weaponIndex);
        if ( ClipSize < clipAmmo )
            varUnion.intValue = ClipSize;
        else
            varUnion.intValue = clipAmmo;
        itemEnt->item[altIndex].ammoCount = reserveAmmo;
        itemEnt->item[altIndex].clipAmmoCount = varUnion.intValue;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_AddStruct)
void __cdecl Scr_AddStruct()
{
    Scr_AddStruct(SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_ResetTimeout)
void __cdecl Scr_ResetTimeout()
{
    Scr_ResetTimeout(SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ClientSysRegister)
void GScr_ClientSysRegister()
{
    char *pRegSysName;
    char szConfigString[1024];
    int i;
    bool bSetString;
    const char *pSysName;

    pSysName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    bSetString = 0;
    for ( i = 0; i < 8; ++i )
    {
        SV_GetConfigstring(i + 1538, szConfigString, 1024);
        if ( szConfigString[0] )
        {
            pRegSysName = Info_ValueForKey(szConfigString, "n");
            if ( pRegSysName )
            {
                if ( !I_stricmp(pRegSysName, pSysName) )
                    bSetString = 1;
            }
        }
        else
        {
            bSetString = 1;
        }
        if ( bSetString )
        {
            Info_SetValueForKey(szConfigString, "n", pSysName);
            SV_SetConfigstring(i + 1538, szConfigString);
            Scr_AddInt(i, SCRIPTINSTANCE_SERVER);
            return;
        }
    }
    Scr_AddInt(-1, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ClientSysSetState)
void GScr_ClientSysSetState()
{
    const char *fmtMsg;
    const char *fmtMsg2;
    const char *fmtMsg3;
    unsigned int j;
    char str[1028];
    const char *pNewState;
    char szConfigString[1024];
    int i;

    i = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)i <= 8 )
    {
        SV_GetConfigstring(i + 1538, szConfigString, 1024);
        if ( szConfigString[0] )
        {
            szConfigString[0] = 0;
            pNewState = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
            str[0] = 0;
            if ( pNewState )
            {
                I_strncpyz(str, pNewState, 1024);
                for ( j = 0; j < &str[strlen(str) + 1] - &str[1]; ++j )
                {
                    if ( str[j] == 32 )
                        str[j] = 33;
                }
            }
            Info_SetValueForKey(szConfigString, "s", str);
            fmtMsg3 = va("%c %i %s", 57, i, szConfigString);
            SV_GameSendServerCommand(-1, SV_CMD_RELIABLE, fmtMsg3);
        }
        else
        {
            fmtMsg2 = va("ClientSysSetState - state index (%i) unregistered.    Use ClientSysRegister first.", i);
            Scr_Error(fmtMsg2, 1);
        }
    }
    else
    {
        fmtMsg = va("ClientSysSetState - state index (%i) out of bounds (0 - %i)", i, 8);
        Scr_Error(fmtMsg, 1);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsAI)
void GScr_IsAI()
{
    if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) == 1
        && Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) == 19
        && Scr_GetEntity(0)->actor )
    {
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetAITriggerFlags)
void GScr_GetAITriggerFlags()
{
    Scr_AddInt(7, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsVehicle)
void GScr_IsVehicle()
{
    if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) == 1
        && Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) == 19
        && Scr_GetEntity(0)->scr_vehicle )
    {
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_GetShootAtPosition)
void __cdecl ScrCmd_GetShootAtPosition(scr_entref_t entref)
{
    float shootAtPos[3];
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->sentient )
        Sentient_GetEyePosition(ent->sentient, shootAtPos);
    else
        G_EntityCentroid(ent, shootAtPos);
    Scr_AddVector(shootAtPos, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_GetDestructibleName @ 825F8080)
void __cdecl ScrCmd_GetDestructibleName(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->destructible && ent->destructible->ddef && ent->destructible->ddef->name )
        Scr_AddString(ent->destructible->ddef->name, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddString("", SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_GetTimeScale @ 825F8F60)
void ScrCmd_GetTimeScale()
{
    Scr_AddFloat((float)Com_GetTimeScale(), SCRIPTINSTANCE_SERVER);
}

// Thin wrapper: gettimescale stays in s_builtinMethods (entref) while global is void().
void __cdecl ScrCmd_GetTimeScale_method(scr_entref_t entref)
{
    (void)entref;
    ScrCmd_GetTimeScale();
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_HasPath @ 826166E8)
void __cdecl GScr_HasPath(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->actor )
    {
        Scr_AddInt(Actor_HasPath(ent->actor), SCRIPTINSTANCE_SERVER);
        return;
    }
    Scr_Error(va("haspath must be called on an AI, not on a '%s'", SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER)), 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetMaxVehicles)
void __cdecl GScr_GetMaxVehicles()
{
    Scr_AddInt(16, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_DisableDestructiblePieces)
void __cdecl GScr_DisableDestructiblePieces()
{
    VariableUnion varUnion;
    float zero_vec[3];
    gentity_s *tempent;

    varUnion.intValue = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    DisableDestructiblePiece(varUnion.intValue);
    memset(zero_vec, 0, sizeof(zero_vec));
    tempent = G_TempEntity(zero_vec, EV_DESTRUCTIBLE_DISABLE_PIECES);
    tempent->s.eventParm = (unsigned __int16)Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_EnableAllDestructiblePieces)
void __cdecl GScr_EnableAllDestructiblePieces()
{
    float zero_vec[3];
    gentity_s *tempent;

    EnableAllDestructiblePieces();
    memset(zero_vec, 0, sizeof(zero_vec));
    tempent = G_TempEntity(zero_vec, EV_DESTRUCTIBLE_DISABLE_PIECES);
    tempent->s.eventParm = 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ClearSpawnPoints)
void __cdecl GScr_ClearSpawnPoints()
{
    SpawnSystem_ClearPoints();
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetSpawnPointRandomVariation)
void __cdecl GScr_SetSpawnPointRandomVariation()
{
    float variation;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("USAGE: setspawnpointrandomvariation( <variation> )\n", 0);
    variation = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    SpawnSystem_SetRandomVariation(variation);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ClearSpawnPointsBaseWeight)
void __cdecl GScr_ClearSpawnPointsBaseWeight()
{
    VariableUnion team_mask;

    team_mask.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    SpawnSystem_ClearPointsBaseWeight(team_mask.intValue);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetSpawnPointsBaseWeight)
void __cdecl GScr_SetSpawnPointsBaseWeight()
{
    float angle;
    VariableUnion team_mask;
    float position[3];
    float score;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 4 )
        Scr_Error("USAGE: setspawnpointsbaseweight( <team mask>, <objective position>, <angle>, <score> )\n", 0);
    team_mask.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, position, SCRIPTINSTANCE_SERVER);
    angle = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    score = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
    SpawnSystem_SetPointsBaseWeight(team_mask.intValue, position, angle, score);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_AddSpawnPoints)
void __cdecl GScr_AddSpawnPoints()
{
    const char *fmtMsg;
    const char *fmtMsg2;
    scr_entref_t tempValue2;
    scr_entref_t tempValue3;
    VariableValueInternal *parentValue;
    int team;
    int script_array_size;
    int parent_id;
    int i;
    VariableValueInternal *entry_value;
    int id;

    team = GScr_ReadTeamForSpawnPoints(0);
    parent_id = Scr_GetObject(1u, SCRIPTINSTANCE_SERVER);
    if ( !parent_id
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 16044, 0, "%s", "parent_id") )
    {
        __debugbreak();
    }
    if ( GetObjectType(SCRIPTINSTANCE_SERVER, parent_id) != 20
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    16045,
                    0,
                    "%s",
                    "GetObjectType(SCRIPTINSTANCE_SERVER, parent_id)==VAR_ARRAY") )
    {
        __debugbreak();
    }
    script_array_size = GetArraySize(SCRIPTINSTANCE_SERVER, parent_id);
    for ( i = 0; i < script_array_size; ++i )
    {
        id = GetArrayVariable(SCRIPTINSTANCE_SERVER, parent_id, i);
        if ( !id && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 16052, 0, "%s", "id") )
            __debugbreak();
        entry_value = &gScrVarGlob[0].variableList[id + 0x8000];
        parentValue = &gScrVarGlob[0].variableList[id + 1];
        if ( (entry_value->w.status & 0x60) == 0
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                        16055,
                        0,
                        "%s",
                        "(entry_value->w.status & VAR_STAT_MASK) != VAR_STAT_FREE") )
        {
            __debugbreak();
        }
        if ( (entry_value->w.status & 0x1F) != 1 || GetObjectType(SCRIPTINSTANCE_SERVER, entry_value->u.next) != 19 )
        {
            fmtMsg2 = va("contents of spawnpoints array must be entities");
            Scr_ParamError(1u, fmtMsg2, SCRIPTINSTANCE_SERVER);
            break;
        }
        tempValue3 = Scr_GetEntityIdRef(SCRIPTINSTANCE_SERVER, entry_value->u.next);
        if ( !SpawnSystem_AddPoint(team, &g_entities[tempValue3.entnum]) )
        {
            fmtMsg = va("Adding to many spawn points to spawnpoint system.    Max: %i\n", 200);
            Scr_Error(fmtMsg, 0);
        }
    }
    SpawnSystem_SortPoints();
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetSortedSpawnPoints)
void __cdecl GScr_GetSortedSpawnPoints()
{
    const char *fmtMsg;
    int SortedPointEntNum;
    signed int i;
    unsigned int influencer_team;
    unsigned int point_team;
    gentity_s *ent;
    int count;

    point_team = GScr_ReadTeamForSpawnPoints(0);
    influencer_team = GScr_ReadTeamForSpawnPoints(1u);
    ent = 0;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 3 )
    {
        ent = Scr_GetEntity(2u);
        if ( !ent->client )
        {
            fmtMsg = va("getsortedspawnpoints: Entity must be a player");
            Scr_ParamError(2u, fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( ent )
        count = SpawnSystem_UpdateSpawnPointsForPlayer(ent, point_team, influencer_team);
    else
        count = SpawnSystem_UpdateSpawnPointsForTeam(point_team, influencer_team);
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for ( i = 0; i < count; ++i )
    {
        SortedPointEntNum = SpawnSystem_GetSortedPointEntNum(point_team, i);
        Scr_AddEntity(&level.gentities[SortedPointEntNum], SCRIPTINSTANCE_SERVER);
        Scr_AddArray(SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsSpawnPointVisible)
void __cdecl GScr_IsSpawnPointVisible()
{
    unsigned __int8 IsSpawnPointVisible;
    int team;
    float angles[3];
    gentity_s *ent;
    float point[3];

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 4 )
        Scr_Error("USAGE: isspawnpointvisible( <point>, <angle>, <team>, <ignore player> )\n", 0);
    Scr_GetVector(0, point, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, angles, SCRIPTINSTANCE_SERVER);
    team = GScr_ReadTeamForSpawnPoints(2u);
    ent = Scr_GetEntity(3u);
    if ( !ent )
        Scr_Error("USAGE: isspawnpointvisible() ignore player is not a valid entity\n", 0);
    IsSpawnPointVisible = SpawnSystem_IsSpawnPointVisible(point, angles, team, ent);
    Scr_AddBool(IsSpawnPointVisible, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_PredictGrenade)
void __cdecl GScr_PredictGrenade(scr_entref_t entref)
{
    int timeAtRest;
    float vLandPos[3];
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->r.inuse
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    16241,
                    0,
                    "%s",
                    "ent->r.inuse") )
    {
        __debugbreak();
    }
    if ( ent->s.eType != 4
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    16242,
                    0,
                    "%s",
                    "ent->s.eType == ET_MISSILE") )
    {
        __debugbreak();
    }
    if ( G_PredictMissile(ent, 3000, vLandPos, 1, &timeAtRest) )
        Scr_AddVector(vLandPos, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddVector((float *)vec3_origin, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_AddSphereInfluencer)
void __cdecl GScr_AddSphereInfluencer()
{
    const char *fmtMsg;
    const char *fmtMsg2;
    unsigned int entNum;
    float time;
    int curve;
    int timeout;
    int influencer_index;
    float origin[3];
    float radius;
    const char *description;
    int team_mask;
    int type;
    gentity_s *ent;
    float score;

    PROF_SCOPED("GScr_AddSphereInfluencer");

    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 6
        || (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 9 )
    {
        Scr_Error(
            "USAGE: addsphereinfluencer( <type>, <origin>, <radius>, <score>, <team mask>, <description>, <curve>, <timeout>, <entity> )\n",
            0);
    }
    type = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)type > 6 )
    {
        fmtMsg = va("USAGE: addsphereinfluencer type must be a value from %i to %i\n", 0, 6);
        Scr_Error(fmtMsg, 0);
    }
    Scr_GetVector(1u, origin, SCRIPTINSTANCE_SERVER);
    radius = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    score = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
    team_mask = Scr_GetInt(4u, SCRIPTINSTANCE_SERVER);
    description = Scr_GetString(5u, SCRIPTINSTANCE_SERVER);
    curve = 0;
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) >= 7 )
    {
        curve = Scr_GetInt(6u, SCRIPTINSTANCE_SERVER);
        if ( (unsigned int)curve > 4 )
        {
            fmtMsg2 = va("USAGE: addsphereinfluencer curve must be a value from %i to %i\n", 0, 4);
            Scr_Error(fmtMsg2, 0);
        }
    }
    timeout = 0;
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) >= 8 )
    {
        time = Scr_GetFloat(7u, SCRIPTINSTANCE_SERVER);
        if ( time > 0.001 )
            timeout = (int)(float)(time * 1000.0);
    }
    ent = 0;
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 9
        || (ent = Scr_GetEntity(8u)) != 0
        && (ent->s.eType == 1
         || ent->s.eType == 17
         || ent->s.eType == 14
         || ent->s.eType == 6
         || ent->s.eType == 4
         || ent->s.eType == 12) )
    {
        InfluencerTypeValidation(type, ent, "addsphereinfluencer");
        if ( ent )
            entNum = ent->s.number;
        else
            entNum = 1023;
        influencer_index = SpawnSystem_AddSphereInfluencer(
                                                 (eInfluencerType)type,
                                                 origin,
                                                 radius,
                                                 score,
                                                 (eInfluencerScoreCurve)curve,
                                                 team_mask,
                                                 entNum,
                                                 timeout);
        if ( influencer_index == -1 )
        {
            Scr_Error("USAGE: addsphereinfluencer could not create influencer \n", 0);
        }
        else
        {
            Scr_AddInt(influencer_index, SCRIPTINSTANCE_SERVER);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (InfluencerTypeValidation)
void __cdecl InfluencerTypeValidation(int type, gentity_s *ent, const char *function_name)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    const char *fmtMsg3;
    const char *fmtMsg4;
    const char *fmtMsg5;

    if ( type == 1 && (!ent || !ent->client) )
    {
        fmtMsg = va("USAGE: %s type PLAYER can only be called on players\n", function_name);
        Scr_Error(fmtMsg, 0);
    }
    if ( type == 2 && (!ent || !ent->client) )
    {
        fmtMsg2 = va("USAGE: %s type WEAPON can only be called on players\n", function_name);
        Scr_Error(fmtMsg2, 0);
    }
    if ( type == 5 && (!ent || !ent->client) )
    {
        fmtMsg3 = va("USAGE: %s type SQUAD can only be called on players\n", function_name);
        Scr_Error(fmtMsg3, 0);
    }
    if ( type == 3 && (!ent || !ent->actor) )
    {
        fmtMsg4 = va("USAGE: %s type DOG can only be called on dogs\n", function_name);
        Scr_Error(fmtMsg4, 0);
    }
    if ( type == 4 && (!ent || !ent->scr_vehicle) )
    {
        fmtMsg5 = va("USAGE: %s type VEHICLE can only be called on vehicles\n", function_name);
        Scr_Error(fmtMsg5, 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_AddCylinderInfluencer)
void __cdecl GScr_AddCylinderInfluencer()
{
    const char *fmtMsg;
    const char *fmtMsg2;
    unsigned int entNum;
    float time;
    int curve;
    int timeout;
    int influencer_index;
    float origin[3];
    float radius;
    const char *description;
    float forward[3];
    int team_mask;
    int type;
    float up[3];
    gentity_s *ent;
    float length;
    float score;

    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 9
        || (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 0xC )
    {
        Scr_Error(
            "USAGE: addcylinderinfluencer( <type>, <origin>, <forward>, <up>, <radius>, <axis length>, <score>, <team mask>, <d"
            "escription>, <curve>, <timeout>, <entity> )\n",
            0);
    }
    type = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)type > 6 )
    {
        fmtMsg = va("USAGE: addcylinderinfluencer type must be a value from %i to %i\n", 0, 6);
        Scr_Error(fmtMsg, 0);
    }
    Scr_GetVector(1u, origin, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(2u, forward, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(3u, up, SCRIPTINSTANCE_SERVER);
    radius = Scr_GetFloat(4u, SCRIPTINSTANCE_SERVER);
    length = Scr_GetFloat(5u, SCRIPTINSTANCE_SERVER);
    score = Scr_GetFloat(6u, SCRIPTINSTANCE_SERVER);
    team_mask = Scr_GetInt(7u, SCRIPTINSTANCE_SERVER);
    description = Scr_GetString(8u, SCRIPTINSTANCE_SERVER);
    curve = 0;
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) >= 0xA )
    {
        curve = Scr_GetInt(9u, SCRIPTINSTANCE_SERVER);
        if ( (unsigned int)curve > 4 )
        {
            fmtMsg2 = va("USAGE: addcylinderinfluencer curve must be a value from %i to %i\n", 0, 4);
            Scr_Error(fmtMsg2, 0);
        }
    }
    timeout = 0;
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) >= 0xB )
    {
        time = Scr_GetFloat(0xAu, SCRIPTINSTANCE_SERVER);
        if ( time > 0.001 )
            timeout = (int)(float)(time * 1000.0);
    }
    ent = 0;
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 0xC
        || (ent = Scr_GetEntity(0xBu)) != 0
        && (ent->s.eType == 1
         || ent->s.eType == 17
         || ent->s.eType == 14
         || ent->s.eType == 6
         || ent->s.eType == 4
         || ent->s.eType == 12) )
    {
        InfluencerTypeValidation(type, ent, "addcylinderinfluencer");
        if ( ent )
            entNum = ent->s.number;
        else
            entNum = 1023;
        influencer_index = SpawnSystem_AddCylinderInfluencer(
                                                 (eInfluencerType)type,
                                                 origin,
                                                 forward,
                                                 up,
                                                 radius,
                                                 length,
                                                 score,
                                                 (eInfluencerScoreCurve)curve,
                                                 team_mask,
                                                 entNum,
                                                 timeout);
        if ( influencer_index == -1 )
            Scr_Error("USAGE: addcylinderinfluencer could not create influencer \n", 0);
        else
            Scr_AddInt(influencer_index, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_RemoveInfluencer)
void __cdecl GScr_RemoveInfluencer()
{
    unsigned int influencer_id;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("USAGE: removeinfluencer(<influencer id> )\n", 0);
    influencer_id = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( !SpawnSystem_RemoveInfluencer(influencer_id) )
        Scr_Error("Trying to remove influencer that does not exist\n", 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_EnableInfluencer)
void __cdecl GScr_EnableInfluencer()
{
    unsigned int influencer_id;
    int enable;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2 )
        Scr_Error("USAGE: enableinfluencer(<influencer id>, <enable> )\n", 0);
    influencer_id = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    enable = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    if ( !SpawnSystem_EnableInfluencer(influencer_id, enable != 0) )
        Scr_Error("Trying to enable influencer that does not exist\n", 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetInfluencerTeamMask)
void __cdecl GScr_SetInfluencerTeamMask()
{
    unsigned int influencer_id;
    VariableUnion team_mask;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2 )
        Scr_Error("USAGE: setinfluencerteammask( <influencer id>, <team mask> )\n", 0);
    influencer_id = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    team_mask.intValue = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    if ( !SpawnSystem_SetInfluencerTeamMask(influencer_id, team_mask.intValue) )
        Scr_Error("Trying to set influencer team mask on an influencer that does not exist\n", 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetDebugSideSwitch)
void __cdecl GScr_SetDebugSideSwitch()
{
    int enabled;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("USAGE: setdebugsideswitch( <enabled> )\n", 0);
    enabled = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    SpawnSystem_DebugSideSwitch(enabled != 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_CollisionTestPointsInSphere)
void GScr_CollisionTestPointsInSphere()
{
    const char *tempValue0;
    const char *fmtMsg;
    const char *tempValue2;
    const char *fmtMsg2;
    const char *TypeName;
    const char *fmtMsg3;
    VariableUnion varUnion;
    float tempValue7;
    float collision_results[3];
    int point_index;
    float points[256][3];
    float sphere_center[3];
    float sphere_radius;
    float rsquared;
    int element_count;

    if ( Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) == 20 )
    {
        if ( Scr_GetType(1u, SCRIPTINSTANCE_SERVER) == 4 )
        {
            if ( Scr_GetType(2u, SCRIPTINSTANCE_SERVER) == 5 )
            {
                varUnion.intValue = Scr_GetObject(0, SCRIPTINSTANCE_SERVER);
                element_count = Scr_GetArrayValues_Vector(0, varUnion.stringValue, points, 256, "spawn points");
                Scr_GetVector(1u, sphere_center, SCRIPTINSTANCE_SERVER);
                sphere_radius = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
                rsquared = sphere_radius * sphere_radius;
                Scr_MakeArray(SCRIPTINSTANCE_SERVER);
                for ( point_index = 0; point_index < element_count; ++point_index )
                {
                    collision_results[1] = Vec3DistanceSq(points[point_index], sphere_center);
                    if ( rsquared <= collision_results[1] )
                        tempValue7 = 0.0f;
                    else
                        tempValue7 = 1.0f;
                    collision_results[0] = tempValue7;
                    collision_results[2] = 0.0f;
                    Scr_AddVector(collision_results, SCRIPTINSTANCE_SERVER);
                    Scr_AddArray(SCRIPTINSTANCE_SERVER);
                }
            }
            else
            {
                TypeName = Scr_GetTypeName(2u, SCRIPTINSTANCE_SERVER);
                fmtMsg3 = va("Parameter '%s' must be a floating point value (sphere radius)", TypeName);
                Scr_ParamError(2u, fmtMsg3, SCRIPTINSTANCE_SERVER);
            }
        }
        else
        {
            tempValue2 = Scr_GetTypeName(1u, SCRIPTINSTANCE_SERVER);
            fmtMsg2 = va("Parameter '%s' must be a vector (3D point origin)", tempValue2);
            Scr_ParamError(1u, fmtMsg2, SCRIPTINSTANCE_SERVER);
        }
    }
    else
    {
        tempValue0 = Scr_GetTypeName(0, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("Parameter '%s' must be an array", tempValue0);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_CollisionTestPointsInCylinder)
void GScr_CollisionTestPointsInCylinder()
{
    const char *tempValue0;
    const char *fmtMsg;
    const char *tempValue2;
    const char *fmtMsg2;
    const char *tempValue4;
    const char *fmtMsg3;
    const char *tempValue6;
    const char *fmtMsg4;
    const char *TypeName;
    const char *fmtMsg5;
    VariableUnion varUnion;
    double tempValue11;
    float dsquared;
    float midpoint_axial_distance;
    float collision_results[3];
    int point_index;
    float half_cylinder_height;
    float bounding_sphere_radius_squared;
    float cylinder_base[3];
    float axis_midpoint[3];
    float points[256][3];
    float cylinder_height;
    float cylinder_radius;
    float cylinder_radius_squared;
    float cylinder_height_unit_vector[3];
    float bounding_sphere_radius;
    int element_count;

    if ( Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) == 20 )
    {
        if ( Scr_GetType(1u, SCRIPTINSTANCE_SERVER) == 4 )
        {
            if ( Scr_GetType(2u, SCRIPTINSTANCE_SERVER) == 5 )
            {
                if ( Scr_GetType(3u, SCRIPTINSTANCE_SERVER) == 5 )
                {
                    if ( Scr_GetType(4u, SCRIPTINSTANCE_SERVER) == 4 )
                    {
                        varUnion.intValue = Scr_GetObject(0, SCRIPTINSTANCE_SERVER);
                        element_count = Scr_GetArrayValues_Vector(0, varUnion.stringValue, points, 256, "spawn points");
                        Scr_GetVector(1u, cylinder_base, SCRIPTINSTANCE_SERVER);
                        cylinder_radius = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
                        cylinder_height = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
                        half_cylinder_height = 0.5 * cylinder_height;
                        cylinder_radius_squared = cylinder_radius * cylinder_radius;
                        bounding_sphere_radius = cylinder_radius + (float)(0.5 * cylinder_height);
                        bounding_sphere_radius_squared = bounding_sphere_radius * bounding_sphere_radius;
                        Scr_GetVector(4u, cylinder_height_unit_vector, SCRIPTINSTANCE_SERVER);
                        axis_midpoint[0] = (float)(cylinder_height_unit_vector[0] * half_cylinder_height) + cylinder_base[0];
                        axis_midpoint[1] = (float)(cylinder_height_unit_vector[1] * half_cylinder_height) + cylinder_base[1];
                        axis_midpoint[2] = (float)(cylinder_height_unit_vector[2] * half_cylinder_height) + cylinder_base[2];
                        Vec3Normalize(cylinder_height_unit_vector);
                        Scr_MakeArray(SCRIPTINSTANCE_SERVER);
                        for ( point_index = 0; point_index < element_count; ++point_index )
                        {
                            memset(collision_results, 0, sizeof(collision_results));
                            tempValue11 = Vec3DistanceSq(points[point_index], axis_midpoint);
                            if ( bounding_sphere_radius_squared > tempValue11 )
                            {
                                midpoint_axial_distance = fabs((float)((float)((float)(points[point_index][0] - axis_midpoint[0]) * cylinder_height_unit_vector[0])
                                    + (float)((float)(points[point_index][1] - axis_midpoint[1]) * cylinder_height_unit_vector[1]))
                                    + (float)((float)(points[point_index][2] - axis_midpoint[2]) * cylinder_height_unit_vector[2]));
                                if ( half_cylinder_height > midpoint_axial_distance )
                                {
                                    dsquared = Vec3DistanceSq(points[point_index], axis_midpoint);
                                    if ( cylinder_radius_squared > (float)(dsquared
                                                                                                             - (float)(midpoint_axial_distance * midpoint_axial_distance)) )
                                    {
                                        collision_results[0] = 1.0f;
                                        collision_results[1] = Vec3DistanceSq(cylinder_base, points[point_index]);
                                        collision_results[2] = dsquared - (float)(midpoint_axial_distance * midpoint_axial_distance);
                                    }
                                }
                            }
                            Scr_AddVector(collision_results, SCRIPTINSTANCE_SERVER);
                            Scr_AddArray(SCRIPTINSTANCE_SERVER);
                        }
                    }
                    else
                    {
                        TypeName = Scr_GetTypeName(4u, SCRIPTINSTANCE_SERVER);
                        fmtMsg5 = va("Parameter '%s' must be a vector value (cylinder height unit vector)", TypeName);
                        Scr_ParamError(4u, fmtMsg5, SCRIPTINSTANCE_SERVER);
                    }
                }
                else
                {
                    tempValue6 = Scr_GetTypeName(3u, SCRIPTINSTANCE_SERVER);
                    fmtMsg4 = va("Parameter '%s' must be a floating point value (cylinder height)", tempValue6);
                    Scr_ParamError(3u, fmtMsg4, SCRIPTINSTANCE_SERVER);
                }
            }
            else
            {
                tempValue4 = Scr_GetTypeName(2u, SCRIPTINSTANCE_SERVER);
                fmtMsg3 = va("Parameter '%s' must be a floating point value (cylinder radius)", tempValue4);
                Scr_ParamError(2u, fmtMsg3, SCRIPTINSTANCE_SERVER);
            }
        }
        else
        {
            tempValue2 = Scr_GetTypeName(1u, SCRIPTINSTANCE_SERVER);
            fmtMsg2 = va("Parameter '%s' must be a vector (3D point origin)", tempValue2);
            Scr_ParamError(1u, fmtMsg2, SCRIPTINSTANCE_SERVER);
        }
    }
    else
    {
        tempValue0 = Scr_GetTypeName(0, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("Parameter '%s' must be an array", tempValue0);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_CollisionTestPointsInPill)
void GScr_CollisionTestPointsInPill()
{
    const char *tempValue0;
    const char *fmtMsg;
    const char *tempValue2;
    const char *fmtMsg2;
    const char *tempValue4;
    const char *fmtMsg3;
    const char *tempValue6;
    const char *fmtMsg4;
    const char *tempValue8;
    const char *fmtMsg5;
    const char *TypeName;
    const char *fmtMsg6;
    VariableUnion varUnion;
    double tempValue13;
    double tempValue14;
    double tempValue15;
    float distance_from_axis;
    float dsquared;
    float midpoint_axial_distance;
    float collision_results[3];
    int point_index;
    float half_cylinder_height;
    float bounding_sphere_radius_squared;
    float cylinder_radial_vector[3];
    float cylinder_base[3];
    float axis_midpoint[3];
    float cylinder_top[3];
    float points[256][3];
    float cylinder_height;
    float cylinder_radius;
    float cylinder_radius_squared;
    float cylinder_height_unit_vector[3];
    float bounding_sphere_radius;
    int element_count;

    if ( Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) == 20 )
    {
        if ( Scr_GetType(1u, SCRIPTINSTANCE_SERVER) == 4 )
        {
            if ( Scr_GetType(2u, SCRIPTINSTANCE_SERVER) == 5 )
            {
                if ( Scr_GetType(3u, SCRIPTINSTANCE_SERVER) == 5 )
                {
                    if ( Scr_GetType(4u, SCRIPTINSTANCE_SERVER) == 4 )
                    {
                        if ( Scr_GetType(5u, SCRIPTINSTANCE_SERVER) == 4 )
                        {
                            varUnion.intValue = Scr_GetObject(0, SCRIPTINSTANCE_SERVER);
                            element_count = Scr_GetArrayValues_Vector(0, varUnion.stringValue, points, 256, "spawn points");
                            Scr_GetVector(1u, cylinder_base, SCRIPTINSTANCE_SERVER);
                            cylinder_radius = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
                            cylinder_height = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
                            half_cylinder_height = 0.5 * cylinder_height;
                            cylinder_radius_squared = cylinder_radius * cylinder_radius;
                            bounding_sphere_radius = cylinder_radius + (float)(0.5 * cylinder_height);
                            bounding_sphere_radius_squared = bounding_sphere_radius * bounding_sphere_radius;
                            Scr_GetVector(4u, cylinder_height_unit_vector, SCRIPTINSTANCE_SERVER);
                            axis_midpoint[0] = (float)(cylinder_height_unit_vector[0] * half_cylinder_height) + cylinder_base[0];
                            axis_midpoint[1] = (float)(cylinder_height_unit_vector[1] * half_cylinder_height) + cylinder_base[1];
                            axis_midpoint[2] = (float)(cylinder_height_unit_vector[2] * half_cylinder_height) + cylinder_base[2];
                            cylinder_top[0] = (float)(cylinder_height_unit_vector[0] * cylinder_height) + cylinder_base[0];
                            cylinder_top[1] = (float)(cylinder_height_unit_vector[1] * cylinder_height) + cylinder_base[1];
                            cylinder_top[2] = (float)(cylinder_height_unit_vector[2] * cylinder_height) + cylinder_base[2];
                            Vec3Normalize(cylinder_height_unit_vector);
                            Scr_GetVector(5u, cylinder_radial_vector, SCRIPTINSTANCE_SERVER);
                            Vec3Normalize(cylinder_radial_vector);
                            Scr_MakeArray(SCRIPTINSTANCE_SERVER);
                            for ( point_index = 0; point_index < element_count; ++point_index )
                            {
                                memset(collision_results, 0, sizeof(collision_results));
                                tempValue13 = Vec3DistanceSq(points[point_index], axis_midpoint);
                                if ( bounding_sphere_radius_squared > tempValue13 )
                                {
                                    tempValue14 = Vec3DistanceSq(points[point_index], cylinder_base);
                                    if ( cylinder_radius_squared <= tempValue14 )
                                    {
                                        tempValue15 = Vec3DistanceSq(points[point_index], cylinder_top);
                                        if ( cylinder_radius_squared <= tempValue15 )
                                        {
                                            midpoint_axial_distance = fabs((float)((float)((float)(points[point_index][0] - axis_midpoint[0]) * cylinder_height_unit_vector[0])
                                                + (float)((float)(points[point_index][1] - axis_midpoint[1]) * cylinder_height_unit_vector[1]))
                                                + (float)((float)(points[point_index][2] - axis_midpoint[2]) * cylinder_height_unit_vector[2]));
                                            if ( half_cylinder_height > midpoint_axial_distance )
                                            {
                                                dsquared = Vec3DistanceSq(points[point_index], axis_midpoint);
                                                if ( cylinder_radius_squared > (float)(dsquared
                                                                                                                         - (float)(midpoint_axial_distance * midpoint_axial_distance)) )
                                                    collision_results[0] = 1.0f;
                                            }
                                        }
                                        else
                                        {
                                            collision_results[0] = 1.0f;
                                        }
                                    }
                                    else
                                    {
                                        collision_results[0] = 1.0f;
                                    }
                                }
                                if ( collision_results[0] > 0.0 )
                                {
                                    distance_from_axis = (float)((float)((float)(points[point_index][0] - cylinder_base[0])
                                                                                                         * cylinder_radial_vector[0])
                                                                                         + (float)((float)(points[point_index][1] - cylinder_base[1])
                                                                                                         * cylinder_radial_vector[1]))
                                                                         + (float)((float)(points[point_index][2] - cylinder_base[2])
                                                                                         * cylinder_radial_vector[2]);
                                    collision_results[1] = Vec3DistanceSq(cylinder_base, points[point_index]);
                                    collision_results[2] = distance_from_axis * distance_from_axis;
                                }
                                Scr_AddVector(collision_results, SCRIPTINSTANCE_SERVER);
                                Scr_AddArray(SCRIPTINSTANCE_SERVER);
                            }
                        }
                        else
                        {
                            TypeName = Scr_GetTypeName(5u, SCRIPTINSTANCE_SERVER);
                            fmtMsg6 = va("Parameter '%s' must be a vector value (cylinder radial vector)", TypeName);
                            Scr_ParamError(5u, fmtMsg6, SCRIPTINSTANCE_SERVER);
                        }
                    }
                    else
                    {
                        tempValue8 = Scr_GetTypeName(4u, SCRIPTINSTANCE_SERVER);
                        fmtMsg5 = va("Parameter '%s' must be a vector value (cylinder height unit vector)", tempValue8);
                        Scr_ParamError(4u, fmtMsg5, SCRIPTINSTANCE_SERVER);
                    }
                }
                else
                {
                    tempValue6 = Scr_GetTypeName(3u, SCRIPTINSTANCE_SERVER);
                    fmtMsg4 = va("Parameter '%s' must be a floating point value (cylinder height)", tempValue6);
                    Scr_ParamError(3u, fmtMsg4, SCRIPTINSTANCE_SERVER);
                }
            }
            else
            {
                tempValue4 = Scr_GetTypeName(2u, SCRIPTINSTANCE_SERVER);
                fmtMsg3 = va("Parameter '%s' must be a floating point value (cylinder radius)", tempValue4);
                Scr_ParamError(2u, fmtMsg3, SCRIPTINSTANCE_SERVER);
            }
        }
        else
        {
            tempValue2 = Scr_GetTypeName(1u, SCRIPTINSTANCE_SERVER);
            fmtMsg2 = va("Parameter '%s' must be a vector (3D point origin)", tempValue2);
            Scr_ParamError(1u, fmtMsg2, SCRIPTINSTANCE_SERVER);
        }
    }
    else
    {
        tempValue0 = Scr_GetTypeName(0, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("Parameter '%s' must be an array", tempValue0);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_CollisionTestPointsInCone)
void GScr_CollisionTestPointsInCone()
{
    const char *tempValue0;
    const char *fmtMsg;
    const char *tempValue2;
    const char *fmtMsg2;
    const char *tempValue4;
    const char *fmtMsg3;
    const char *tempValue6;
    const char *fmtMsg4;
    const char *tempValue8;
    const char *fmtMsg5;
    const char *TypeName;
    const char *fmtMsg6;
    VariableUnion varUnion;
    double tempValue13;
    float tempValue14;
    float tempValue15;
    float distance_from_axis;
    float distance_from_origin_squared;
    float tp;
    float tp_4;
    float tp_8;
    float tp_dot_to;
    float collision_results[3];
    int point_index;
    float bounding_sphere_radius_squared;
    float axis_midpoint[3];
    float te_dot_to;
    float points[256][3];
    float cone_height;
    float radial_edge[3];
    float cone_base_radius;
    float half_cone_height;
    float te_magnitude_squared;
    float cone_tip[3];
    float bounding_sphere_radius;
    float cone_base[3];
    float cone_height_unit_vector[3];
    float to[3];
    float te[3];
    float teto2_over_te;
    float cone_radial_unit_vector[3];
    int element_count;

    if ( Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) == 20 )
    {
        if ( Scr_GetType(1u, SCRIPTINSTANCE_SERVER) == 4 )
        {
            if ( Scr_GetType(2u, SCRIPTINSTANCE_SERVER) == 5 )
            {
                if ( Scr_GetType(3u, SCRIPTINSTANCE_SERVER) == 5 )
                {
                    if ( Scr_GetType(4u, SCRIPTINSTANCE_SERVER) == 4 )
                    {
                        if ( Scr_GetType(5u, SCRIPTINSTANCE_SERVER) == 4 )
                        {
                            varUnion.intValue = Scr_GetObject(0, SCRIPTINSTANCE_SERVER);
                            element_count = Scr_GetArrayValues_Vector(0, varUnion.stringValue, points, 256, "spawn points");
                            Scr_GetVector(1u, cone_base, SCRIPTINSTANCE_SERVER);
                            cone_base_radius = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
                            cone_height = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
                            Scr_GetVector(4u, cone_height_unit_vector, SCRIPTINSTANCE_SERVER);
                            Scr_GetVector(5u, cone_radial_unit_vector, SCRIPTINSTANCE_SERVER);
                            half_cone_height = 0.5 * cone_height;
                            bounding_sphere_radius = cone_base_radius + (float)(0.5 * cone_height);
                            bounding_sphere_radius_squared = bounding_sphere_radius * bounding_sphere_radius;
                            axis_midpoint[0] = (float)(cone_height_unit_vector[0] * (float)(0.5 * cone_height)) + cone_base[0];
                            axis_midpoint[1] = (float)(cone_height_unit_vector[1] * (float)(0.5 * cone_height)) + cone_base[1];
                            axis_midpoint[2] = (float)(cone_height_unit_vector[2] * (float)(0.5 * cone_height)) + cone_base[2];
                            cone_tip[0] = (float)(cone_height_unit_vector[0] * cone_height) + cone_base[0];
                            cone_tip[1] = (float)(cone_height_unit_vector[1] * cone_height) + cone_base[1];
                            cone_tip[2] = (float)(cone_height_unit_vector[2] * cone_height) + cone_base[2];
                            radial_edge[0] = (float)(cone_radial_unit_vector[0] * cone_base_radius) + cone_base[0];
                            radial_edge[1] = (float)(cone_radial_unit_vector[1] * cone_base_radius) + cone_base[1];
                            radial_edge[2] = (float)(cone_radial_unit_vector[2] * cone_base_radius) + cone_base[2];
                            Vec3Normalize(cone_height_unit_vector);
                            Vec3Normalize(cone_radial_unit_vector);
                            te[0] = radial_edge[0] - cone_tip[0];
                            te[1] = radial_edge[1] - cone_tip[1];
                            te[2] = radial_edge[2] - cone_tip[2];
                            to[0] = cone_base[0] - cone_tip[0];
                            to[1] = cone_base[1] - cone_tip[1];
                            to[2] = cone_base[2] - cone_tip[2];
                            te_dot_to = (float)((float)((float)(radial_edge[0] - cone_tip[0]) * (float)(cone_base[0] - cone_tip[0]))
                                                                + (float)((float)(radial_edge[1] - cone_tip[1]) * (float)(cone_base[1] - cone_tip[1])))
                                                + (float)((float)(radial_edge[2] - cone_tip[2]) * (float)(cone_base[2] - cone_tip[2]));
                            if ( (float)(0.0000099999997
                                                 - (float)((float)((float)((float)(radial_edge[0] - cone_tip[0])
                                                                                                 * (float)(radial_edge[0] - cone_tip[0]))
                                                                                 + (float)((float)(radial_edge[1] - cone_tip[1])
                                                                                                 * (float)(radial_edge[1] - cone_tip[1])))
                                                                 + (float)((float)(radial_edge[2] - cone_tip[2]) * (float)(radial_edge[2] - cone_tip[2])))) < 0.0 )
                                tempValue15 = (float)((float)((float)(radial_edge[0] - cone_tip[0]) * (float)(radial_edge[0] - cone_tip[0]))
                                                        + (float)((float)(radial_edge[1] - cone_tip[1]) * (float)(radial_edge[1] - cone_tip[1])))
                                        + (float)((float)(radial_edge[2] - cone_tip[2]) * (float)(radial_edge[2] - cone_tip[2]));
                            else
                                tempValue15 = 0.0000099999997f;
                            te_magnitude_squared = tempValue15;
                            teto2_over_te = (float)(te_dot_to * te_dot_to) / tempValue15;
                            Scr_MakeArray(SCRIPTINSTANCE_SERVER);
                            for ( point_index = 0; point_index < element_count; ++point_index )
                            {
                                memset(collision_results, 0, sizeof(collision_results));
                                tempValue13 = Vec3DistanceSq(points[point_index], axis_midpoint);
                                if ( bounding_sphere_radius_squared > tempValue13
                                    && half_cone_height > fabs((float)((float)((float)(points[point_index][0] - axis_midpoint[0])
                                                                                                                    * cone_height_unit_vector[0])
                                                                                                    + (float)((float)(points[point_index][1] - axis_midpoint[1])
                                                                                                                    * cone_height_unit_vector[1]))
                                                                                    + (float)((float)(points[point_index][2] - axis_midpoint[2])
                                                                                                    * cone_height_unit_vector[2]))
                                                                                 )
                                {
                                    tp = points[point_index][0] - cone_tip[0];
                                    tp_4 = points[point_index][1] - cone_tip[1];
                                    tp_8 = points[point_index][2] - cone_tip[2];
                                    if ( (float)(0.0000099999997
                                                         - (float)((float)((float)(tp * tp) + (float)(tp_4 * tp_4)) + (float)(tp_8 * tp_8))) < 0.0 )
                                        tempValue14 = (float)((float)(tp * tp) + (float)(tp_4 * tp_4)) + (float)(tp_8 * tp_8);
                                    else
                                        tempValue14 = 0.0000099999997;
                                    tp_dot_to = (float)((float)(tp * to[0]) + (float)(tp_4 * to[1])) + (float)(tp_8 * to[2]);
                                    if ( (float)((float)(tp_dot_to * tp_dot_to) / tempValue14) > teto2_over_te )
                                    {
                                        distance_from_origin_squared = Vec3DistanceSq(cone_base, points[point_index]);
                                        distance_from_axis = (float)((float)((float)(points[point_index][0] - cone_base[0])
                                                                                                             * cone_radial_unit_vector[0])
                                                                                             + (float)((float)(points[point_index][1] - cone_base[1])
                                                                                                             * cone_radial_unit_vector[1]))
                                                                             + (float)((float)(points[point_index][2] - cone_base[2])
                                                                                             * cone_radial_unit_vector[2]);
                                        collision_results[0] = 1.0f;
                                        collision_results[1] = distance_from_origin_squared;
                                        collision_results[2] = distance_from_axis * distance_from_axis;
                                    }
                                }
                                Scr_AddVector(collision_results, SCRIPTINSTANCE_SERVER);
                                Scr_AddArray(SCRIPTINSTANCE_SERVER);
                            }
                        }
                        else
                        {
                            TypeName = Scr_GetTypeName(5u, SCRIPTINSTANCE_SERVER);
                            fmtMsg6 = va("Parameter '%s' must be a vector value (cone radial vector)", TypeName);
                            Scr_ParamError(5u, fmtMsg6, SCRIPTINSTANCE_SERVER);
                        }
                    }
                    else
                    {
                        tempValue8 = Scr_GetTypeName(4u, SCRIPTINSTANCE_SERVER);
                        fmtMsg5 = va("Parameter '%s' must be a vector value (cone height unit vector)", tempValue8);
                        Scr_ParamError(4u, fmtMsg5, SCRIPTINSTANCE_SERVER);
                    }
                }
                else
                {
                    tempValue6 = Scr_GetTypeName(3u, SCRIPTINSTANCE_SERVER);
                    fmtMsg4 = va("Parameter '%s' must be a floating point value (cone height)", tempValue6);
                    Scr_ParamError(3u, fmtMsg4, SCRIPTINSTANCE_SERVER);
                }
            }
            else
            {
                tempValue4 = Scr_GetTypeName(2u, SCRIPTINSTANCE_SERVER);
                fmtMsg3 = va("Parameter '%s' must be a floating point value (cone radius)", tempValue4);
                Scr_ParamError(2u, fmtMsg3, SCRIPTINSTANCE_SERVER);
            }
        }
        else
        {
            tempValue2 = Scr_GetTypeName(1u, SCRIPTINSTANCE_SERVER);
            fmtMsg2 = va("Parameter '%s' must be a vector (3D point origin)", tempValue2);
            Scr_ParamError(1u, fmtMsg2, SCRIPTINSTANCE_SERVER);
        }
    }
    else
    {
        tempValue0 = Scr_GetTypeName(0, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("Parameter '%s' must be an array", tempValue0);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_CollisionTestPointsInBox)
void GScr_CollisionTestPointsInBox()
{
    const char *tempValue0;
    const char *fmtMsg;
    const char *tempValue2;
    const char *fmtMsg2;
    const char *tempValue4;
    const char *fmtMsg3;
    const char *tempValue6;
    const char *fmtMsg4;
    const char *tempValue8;
    const char *fmtMsg5;
    const char *tempValue10;
    const char *fmtMsg6;
    const char *TypeName;
    const char *fmtMsg7;
    VariableUnion varUnion;
    float tempValue15;
    float tempValue16;
    float collision_results[3];
    float tempValue18[3];
    int point_index;
    float bounding_sphere_radius_squared;
    float box_depth;
    float half_box_height;
    float transform[4][3];
    float points[256][3];
    float axes[3][3];
    float local_left[3];
    float box_up[3];
    float box_forward[3];
    float box_height;
    float local_up[3];
    float bounding_sphere_radius;
    float half_box_width;
    float box_width;
    float local_forward[3];
    float half_box_depth;
    float box_origin[3];
    int element_count;
    float box_left[3];

    if ( Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) == 20 )
    {
        if ( Scr_GetType(1u, SCRIPTINSTANCE_SERVER) == 4 )
        {
            if ( Scr_GetType(2u, SCRIPTINSTANCE_SERVER) == 5 )
            {
                if ( Scr_GetType(3u, SCRIPTINSTANCE_SERVER) == 5 )
                {
                    if ( Scr_GetType(4u, SCRIPTINSTANCE_SERVER) == 5 )
                    {
                        if ( Scr_GetType(5u, SCRIPTINSTANCE_SERVER) == 4 )
                        {
                            if ( Scr_GetType(6u, SCRIPTINSTANCE_SERVER) == 4 )
                            {
                                varUnion.intValue = Scr_GetObject(0, SCRIPTINSTANCE_SERVER);
                                element_count = Scr_GetArrayValues_Vector(0, varUnion.stringValue, points, 256, "spawn points");
                                Scr_GetVector(1u, box_origin, SCRIPTINSTANCE_SERVER);
                                box_width = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
                                box_height = Scr_GetFloat(3u, SCRIPTINSTANCE_SERVER);
                                box_depth = Scr_GetFloat(4u, SCRIPTINSTANCE_SERVER);
                                Scr_GetVector(5u, axes[0], SCRIPTINSTANCE_SERVER);
                                Scr_GetVector(6u, axes[1], SCRIPTINSTANCE_SERVER);
                                Vec3Cross(axes[0], axes[1], axes[2]);
                                if ( (float)(box_height - box_width) < 0.0 )
                                    tempValue16 = box_height;
                                else
                                    tempValue16 = box_width;
                                if ( (float)(box_depth - tempValue16) < 0.0 )
                                    tempValue15 = box_depth;
                                else
                                    tempValue15 = tempValue16;
                                bounding_sphere_radius = (float)((float)(box_width + box_height) + box_depth) - tempValue15;
                                bounding_sphere_radius_squared = bounding_sphere_radius * bounding_sphere_radius;
                                half_box_width = 0.5 * box_width;
                                half_box_height = 0.5 * box_height;
                                half_box_depth = 0.5 * box_depth;
                                *(_QWORD *)&transform[0][0] = *(_QWORD *)&axes[0][0];
                                transform[0][2] = axes[0][2];
                                *(_QWORD *)&transform[1][0] = *(_QWORD *)&axes[1][0];
                                transform[1][2] = axes[1][2];
                                *(_QWORD *)&transform[2][0] = *(_QWORD *)&axes[2][0];
                                transform[2][2] = axes[2][2];
                                memset(transform[3], 0, sizeof(const float[3]));
                                local_forward[0] = box_width;
                                local_forward[1] = 0.0f;
                                local_forward[2] = 0.0f;
                                local_left[0] = 0.0f;
                                local_left[1] = box_height;
                                local_left[2] = 0.0f;
                                local_up[0] = 0.0f;
                                local_up[1] = 0.0f;
                                local_up[2] = box_depth;
                                MatrixTransformVector43(local_forward, transform, box_forward);
                                MatrixTransformVector43(local_up, transform, box_up);
                                MatrixTransformVector43(local_left, transform, box_left);
                                Vec3Normalize(box_forward);
                                Vec3Normalize(box_left);
                                Vec3Normalize(box_up);
                                Scr_MakeArray(SCRIPTINSTANCE_SERVER);
                                for ( point_index = 0; point_index < element_count; ++point_index )
                                {
                                    memset(collision_results, 0, sizeof(collision_results));
                                    tempValue18[0] = points[point_index][0] - box_origin[0];
                                    tempValue18[1] = points[point_index][1] - box_origin[1];
                                    tempValue18[2] = points[point_index][2] - box_origin[2];
                                    if ( bounding_sphere_radius_squared > (float)((float)((float)(tempValue18[0] * tempValue18[0])
                                                                                                                                            + (float)(tempValue18[1] * tempValue18[1]))
                                                                                                                            + (float)(tempValue18[2] * tempValue18[2]))
                                        && half_box_width >  (fabs((float)((float)(tempValue18[0] * box_forward[0]) + (float)(tempValue18[1] * box_forward[1])) + (float)(tempValue18[2] * box_forward[2])) )
                                        && half_box_height > (fabs((float)((float)(tempValue18[0] * box_left[0]) + (float)(tempValue18[1] * box_left[1])) + (float)(tempValue18[2] * box_left[2])) )
                                        && half_box_depth >  (fabs((float)((float)(tempValue18[0] * box_up[0]) + (float)(tempValue18[1] * box_up[1])) + (float)(tempValue18[2] * box_up[2]))) )
                                    {
                                        collision_results[0] = 1.0f;
                                        collision_results[1] = (float)((float)(tempValue18[0] * tempValue18[0]) + (float)(tempValue18[1] * tempValue18[1]))
                                                                                 + (float)(tempValue18[2] * tempValue18[2]);
                                    }
                                    Scr_AddVector(collision_results, SCRIPTINSTANCE_SERVER);
                                    Scr_AddArray(SCRIPTINSTANCE_SERVER);
                                }
                            }
                            else
                            {
                                TypeName = Scr_GetTypeName(6u, SCRIPTINSTANCE_SERVER);
                                fmtMsg7 = va("Parameter '%s' must be a vector (up)", TypeName);
                                Scr_ParamError(6u, fmtMsg7, SCRIPTINSTANCE_SERVER);
                            }
                        }
                        else
                        {
                            tempValue10 = Scr_GetTypeName(5u, SCRIPTINSTANCE_SERVER);
                            fmtMsg6 = va("Parameter '%s' must be a vector (forward)", tempValue10);
                            Scr_ParamError(5u, fmtMsg6, SCRIPTINSTANCE_SERVER);
                        }
                    }
                    else
                    {
                        tempValue8 = Scr_GetTypeName(4u, SCRIPTINSTANCE_SERVER);
                        fmtMsg5 = va("Parameter '%s' must be a floating point value (box depth)", tempValue8);
                        Scr_ParamError(4u, fmtMsg5, SCRIPTINSTANCE_SERVER);
                    }
                }
                else
                {
                    tempValue6 = Scr_GetTypeName(3u, SCRIPTINSTANCE_SERVER);
                    fmtMsg4 = va("Parameter '%s' must be a floating point value (box length)", tempValue6);
                    Scr_ParamError(3u, fmtMsg4, SCRIPTINSTANCE_SERVER);
                }
            }
            else
            {
                tempValue4 = Scr_GetTypeName(2u, SCRIPTINSTANCE_SERVER);
                fmtMsg3 = va("Parameter '%s' must be a floating point value (box width)", tempValue4);
                Scr_ParamError(2u, fmtMsg3, SCRIPTINSTANCE_SERVER);
            }
        }
        else
        {
            tempValue2 = Scr_GetTypeName(1u, SCRIPTINSTANCE_SERVER);
            fmtMsg2 = va("Parameter '%s' must be a vector (3D point origin)", tempValue2);
            Scr_ParamError(1u, fmtMsg2, SCRIPTINSTANCE_SERVER);
        }
    }
    else
    {
        tempValue0 = Scr_GetTypeName(0, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("Parameter '%s' must be an array", tempValue0);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_QSortScoredSpawnPointArrayAscending)
void GScr_QSortScoredSpawnPointArrayAscending()
{
    const char *TypeName;
    const char *fmtMsg;
    VariableUnion varUnion;
    int value_index;
    float scored_spawn_points[256][3];
    int element_count;

    if ( Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) == 20 )
    {
        varUnion.intValue = Scr_GetObject(0, SCRIPTINSTANCE_SERVER);
        element_count = Scr_GetArrayValues_Vector(0, varUnion.stringValue, scored_spawn_points, 256, "scored spawn points");
        Scr_MakeArray(SCRIPTINSTANCE_SERVER);
        if ( element_count > 0 )
            qsort(
                scored_spawn_points,
                element_count,
                0xCu,
                (int (__cdecl *)(const void *, const void *))sort_scored_spawn_point_vectors_ascending);
        for ( value_index = 0; value_index < element_count; ++value_index )
        {
            Scr_AddVector(scored_spawn_points[value_index], SCRIPTINSTANCE_SERVER);
            Scr_AddArray(SCRIPTINSTANCE_SERVER);
        }
    }
    else
    {
        TypeName = Scr_GetTypeName(0, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("Parameter '%s' must be an array", TypeName);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (sort_scored_spawn_point_vectors_ascending)
int __cdecl sort_scored_spawn_point_vectors_ascending(float *a, float *b)
{
    if ( a[1] > b[1] )
        return 1;
    if ( b[1] <= a[1] )
        return 0;
    return -1;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_Matrix4x4TransformPoints)
void GScr_Matrix4x4TransformPoints()
{
    const char *fmtMsg;
    const char *fmtMsg2;
    float transformed_point[3];
    int point_index;
    float points[128][3];
    int point_count;
    int element_index;
    int matrix_element_size;
    int parent_id;
    float matrix_transform[16];
    int parameter_index;
    int k_matrix_4x4_element_count;
    VariableValueInternal *entry_value;
    int id;

    k_matrix_4x4_element_count = 16;
    parameter_index = 0;
    parent_id = Scr_GetObject(0, SCRIPTINSTANCE_SERVER);
    if ( GetObjectType(SCRIPTINSTANCE_SERVER, parent_id) != 20
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    17107,
                    0,
                    "%s",
                    "GetObjectType(SCRIPTINSTANCE_SERVER, parent_id)==VAR_ARRAY") )
    {
        __debugbreak();
    }
    matrix_element_size = GetArraySize(SCRIPTINSTANCE_SERVER, parent_id);
    if ( matrix_element_size == 16 )
    {
        for ( element_index = 0; element_index < 16; ++element_index )
        {
            id = GetArrayVariable(SCRIPTINSTANCE_SERVER, parent_id, element_index);
            if ( !id
                && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 17117, 0, "%s", "id") )
            {
                __debugbreak();
            }
            entry_value = &gScrVarGlob[0].variableList[id + 0x8000];
            if ( (entry_value->w.status & 0x60) == 0
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                            17119,
                            0,
                            "%s",
                            "(entry_value->w.status & VAR_STAT_MASK) != VAR_STAT_FREE") )
            {
                __debugbreak();
            }
            if ( (entry_value->w.status & 0x1F) == 5 )
            {
                matrix_transform[element_index] = entry_value->u.u.floatValue;
            }
            else
            {
                if ( (entry_value->w.status & 0x1F) != 6 )
                {
                    fmtMsg = va("contents of transformation matrix array must be numbers");
                    Scr_ParamError(parameter_index, fmtMsg, SCRIPTINSTANCE_SERVER);
                    break;
                }
                matrix_transform[element_index] = (float)entry_value->u.u.intValue;
            }
        }
        if ( element_index == 16 )
        {
            parent_id = Scr_GetObject(++parameter_index, SCRIPTINSTANCE_SERVER);
            point_count = Scr_GetArrayValues_Vector(parameter_index, parent_id, points, 128, "debug geometry points");
            Scr_MakeArray(SCRIPTINSTANCE_SERVER);
            for ( point_index = 0; point_index < point_count; ++point_index )
            {
                transformed_point[0] = (float)((float)((float)(points[point_index][0] * matrix_transform[0])
                                                                                         + (float)(points[point_index][1] * matrix_transform[4]))
                                                                         + (float)(points[point_index][2] * matrix_transform[8]))
                                                         + matrix_transform[3];
                transformed_point[1] = (float)((float)((float)(points[point_index][0] * matrix_transform[1])
                                                                                         + (float)(points[point_index][1] * matrix_transform[5]))
                                                                         + (float)(points[point_index][2] * matrix_transform[9]))
                                                         + matrix_transform[7];
                transformed_point[2] = (float)((float)((float)(points[point_index][0] * matrix_transform[2])
                                                                                         + (float)(points[point_index][1] * matrix_transform[6]))
                                                                         + (float)(points[point_index][2] * matrix_transform[10]))
                                                         + matrix_transform[11];
                Scr_AddVector(transformed_point, SCRIPTINSTANCE_SERVER);
                Scr_AddArray(SCRIPTINSTANCE_SERVER);
            }
        }
    }
    else
    {
        fmtMsg2 = va("first parameter passed to Matrix4x4TransformPoints must be float[16]");
        Scr_ParamError(parameter_index, fmtMsg2, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetNumGVRules)
void GScr_GetNumGVRules()
{
    Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetGVRule)
void GScr_GetGVRule()
{
    Scr_GameVariants_GetRule();
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetWeaponMinDamageRange)
void GScr_GetWeaponMinDamageRange()
{
    const char *fmtMsg;
    unsigned int iWeaponIndex;
    weaponParms wp;
    const char *pszWeaponName;

    pszWeaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = G_GetWeaponIndexForName((char *)pszWeaponName);
    if ( !iWeaponIndex )
    {
        if ( *pszWeaponName )
        {
            fmtMsg = va("Invalid weapon name %s", pszWeaponName);
            Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
        }
        else
        {
            Scr_ParamError(0, "Invalid weapon name", SCRIPTINSTANCE_SERVER);
        }
    }
    Weapon_SetWeaponParamsWeapon(&wp, iWeaponIndex);
    Scr_AddFloat(wp.weapDef->fMinDamageRange, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetWeaponMaxDamageRange)
void GScr_GetWeaponMaxDamageRange()
{
    const char *fmtMsg;
    unsigned int iWeaponIndex;
    weaponParms wp;
    const char *pszWeaponName;

    pszWeaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = G_GetWeaponIndexForName((char *)pszWeaponName);
    if ( !iWeaponIndex )
    {
        if ( *pszWeaponName )
        {
            fmtMsg = va("Invalid weapon name %s", pszWeaponName);
            Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
        }
        else
        {
            Scr_ParamError(0, "Invalid weapon name", SCRIPTINSTANCE_SERVER);
        }
    }
    Weapon_SetWeaponParamsWeapon(&wp, iWeaponIndex);
    Scr_AddFloat(wp.weapDef->fMaxDamageRange, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetWeaponMinDamage)
void GScr_GetWeaponMinDamage()
{
    const char *fmtMsg;
    unsigned int iWeaponIndex;
    weaponParms wp;
    const char *pszWeaponName;

    pszWeaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = G_GetWeaponIndexForName((char *)pszWeaponName);
    if ( !iWeaponIndex )
    {
        if ( *pszWeaponName )
        {
            fmtMsg = va("Invalid weapon name %s", pszWeaponName);
            Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
        }
        else
        {
            Scr_ParamError(0, "Invalid weapon name", SCRIPTINSTANCE_SERVER);
        }
    }
    Weapon_SetWeaponParamsWeapon(&wp, iWeaponIndex);
    Scr_AddFloat((float)wp.weapDef->minDamage, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetWeaponMaxDamage)
void GScr_GetWeaponMaxDamage()
{
    const char *fmtMsg;
    unsigned int iWeaponIndex;
    weaponParms wp;
    const char *pszWeaponName;

    pszWeaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = G_GetWeaponIndexForName((char *)pszWeaponName);
    if ( !iWeaponIndex )
    {
        if ( *pszWeaponName )
        {
            fmtMsg = va("Invalid weapon name %s", pszWeaponName);
            Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
        }
        else
        {
            Scr_ParamError(0, "Invalid weapon name", SCRIPTINSTANCE_SERVER);
        }
    }
    Weapon_SetWeaponParamsWeapon(&wp, iWeaponIndex);
    Scr_AddFloat((float)wp.weapDef->damage, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetWeaponFuseTime)
void GScr_GetWeaponFuseTime()
{
    const char *fmtMsg;
    unsigned int iWeaponIndex;
    weaponParms wp;
    const char *pszWeaponName;

    pszWeaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = G_GetWeaponIndexForName((char *)pszWeaponName);
    if ( !iWeaponIndex )
    {
        if ( *pszWeaponName )
        {
            fmtMsg = va("Invalid weapon name %s", pszWeaponName);
            Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
        }
        else
        {
            Scr_ParamError(0, "Invalid weapon name", SCRIPTINSTANCE_SERVER);
        }
    }
    Weapon_SetWeaponParamsWeapon(&wp, iWeaponIndex);
    Scr_AddFloat((float)wp.weapDef->fuseTime, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetWeaponProjExplosionSound)
void GScr_GetWeaponProjExplosionSound()
{
    const char *fmtMsg;
    unsigned int iWeaponIndex;
    weaponParms wp;
    const char *pszWeaponName;

    pszWeaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = G_GetWeaponIndexForName((char *)pszWeaponName);
    if ( !iWeaponIndex )
    {
        if ( *pszWeaponName )
        {
            fmtMsg = va("Invalid weapon name %s", pszWeaponName);
            Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
        }
        else
        {
            Scr_ParamError(0, "Invalid weapon name", SCRIPTINSTANCE_SERVER);
        }
    }
    Weapon_SetWeaponParamsWeapon(&wp, iWeaponIndex);
    if ( wp.weapDef->projExplosionSound )
        Scr_AddString((char *)wp.weapDef->projExplosionSound, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddString((char *)"", SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsWeaponSpecificUse)
void GScr_IsWeaponSpecificUse()
{
    char *fmtMsg;
    unsigned int iWeaponIndex;
    weaponParms wp;
    const char *pszWeaponName;

    pszWeaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = G_GetWeaponIndexForName((char *)pszWeaponName);
    if ( iWeaponIndex )
    {
        Weapon_SetWeaponParamsWeapon(&wp, iWeaponIndex);
        if ( wp.weapDef->offhandSlot == OFFHAND_SLOT_SPECIFIC_USE )
            Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
        else
            Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        fmtMsg = va("WARNING: unknown weapon '%s' passed to IsWeaponSpecificUse\n", pszWeaponName);
        Com_PrintWarning(24, fmtMsg);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsWeaponEquipment)
void GScr_IsWeaponEquipment()
{
    char *fmtMsg;
    unsigned int iWeaponIndex;
    weaponParms wp;
    const char *pszWeaponName;

    pszWeaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = G_GetWeaponIndexForName((char *)pszWeaponName);
    if ( iWeaponIndex )
    {
        Weapon_SetWeaponParamsWeapon(&wp, iWeaponIndex);
        if ( wp.weapDef->offhandSlot == OFFHAND_SLOT_EQUIPMENT )
            Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
        else
            Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        fmtMsg = va("WARNING: unknown weapon '%s' passed to IsWeaponEquipment\n", pszWeaponName);
        Com_PrintWarning(24, fmtMsg);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsWeaponPrimary)
void GScr_IsWeaponPrimary()
{
    char *fmtMsg;
    unsigned int iWeaponIndex;
    weaponParms wp;
    const char *pszWeaponName;

    pszWeaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = G_GetWeaponIndexForName((char *)pszWeaponName);
    if ( iWeaponIndex )
    {
        Weapon_SetWeaponParamsWeapon(&wp, iWeaponIndex);
        if ( wp.weapDef->inventoryType )
            Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
        else
            Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        fmtMsg = va("WARNING: unknown weapon '%s' passed to IsWeaponPrimary\n", pszWeaponName);
        Com_PrintWarning(24, fmtMsg);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsWeaponScopeOverlay)
void GScr_IsWeaponScopeOverlay()
{
    char *fmtMsg;
    unsigned int iWeaponIndex;
    weaponParms wp;
    const char *pszWeaponName;

    pszWeaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = G_GetWeaponIndexForName((char *)pszWeaponName);
    if ( iWeaponIndex )
    {
        Weapon_SetWeaponParamsWeapon(&wp, iWeaponIndex);
        if ( wp.weapDef->overlayReticle )
            Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
        else
            Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        fmtMsg = va("WARNING: unknown weapon '%s' passed to IsWeaponScopeOverlay\n", pszWeaponName);
        Com_PrintWarning(24, fmtMsg);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_PCServerUpdatePlaylist)
void GScr_PCServerUpdatePlaylist()
{
    SV_FetchWADDeferred();
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetPregameTeam)
void __cdecl GScr_GetPregameTeam(scr_entref_t entref)
{
    team_t AssignedPregameTeam;
    gentity_s *ent;

    ent = GetPlayerEntity(entref);
    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 17561, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 17562, 0, "%s", "ent->client") )
    {
        __debugbreak();
    }
    AssignedPregameTeam = Pregame_GetAssignedPregameTeam(ent->s.number);
    Scr_AddInt(AssignedPregameTeam, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetPregameClass)
void __cdecl GScr_GetPregameClass(scr_entref_t entref)
{
    char *AssignedPregameClass;
    gentity_s *ent;

    ent = GetPlayerEntity(entref);
    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 17577, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 17578, 0, "%s", "ent->client") )
    {
        __debugbreak();
    }
    if ( *Pregame_GetAssignedPregameClass(ent->s.number) )
    {
        AssignedPregameClass = Pregame_GetAssignedPregameClass(ent->s.number);
        Scr_AddString(AssignedPregameClass, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddUndefined(SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetPregameTeam)
void __cdecl GScr_SetPregameTeam(scr_entref_t entref)
{
    unsigned __int16 team;
    gentity_s *ent;

    ent = GetPlayerEntity(entref);
    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 17598, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 17599, 0, "%s", "ent->client") )
    {
        __debugbreak();
    }
    team = (unsigned __int16)Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    if ( team == scr_const.allies )
    {
        Pregame_SetAssignedPregameTeam(ent->s.number, TEAM_ALLIES);
    }
    else if ( team == scr_const.axis )
    {
        Pregame_SetAssignedPregameTeam(ent->s.number, TEAM_AXIS);
    }
    else
    {
        Pregame_SetAssignedPregameTeam(ent->s.number, TEAM_SPECTATOR);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetPregameClass)
void __cdecl GScr_SetPregameClass(scr_entref_t entref)
{
    char *String;
    gentity_s *ent;

    ent = GetPlayerEntity(entref);
    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 17622, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 17623, 0, "%s", "ent->client") )
    {
        __debugbreak();
    }
    String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    Pregame_SetAssignedPregameClass(ent->s.number, String);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_PixBeginEvent)
void GScr_PixBeginEvent()
{
    char *String;
    const char *fmtMsg;

    String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    fmtMsg = va("SCRIPT: %s", String);
    // PIX marker hook point
}

void GScr_PixEndEvent()
{
    //char *String;
    //const char *fmtMsg;
    //
    //String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    //fmtMsg = va("SCRIPT: %s", String);
    // PIX marker hook point
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_PixMarker)
void GScr_PixMarker()
{
    char *String;
    const char *fmtMsg;

    String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    fmtMsg = va("SCRIPT: %s", String);
    //PIXSetMarker(0, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IncrementCounter)
void GScr_IncrementCounter()
{
    int LicenseType;
    char *counterType;
    int counterId;
    int increment;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 2 )
    {
        LicenseType = SV_GetLicenseType();
        if ( SV_IsServerRanked(LicenseType) )
        {
            counterType = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
            increment = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
            if ( increment )
            {
                if ( increment < 0
                    && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                                17746,
                                0,
                                "%s",
                                "increment >= 0") )
                {
                    __debugbreak();
                }
                if ( increment >= 0 )
                {
                    counterId = LiveCounter_CounterStringToID(counterType);
                    if ( counterId == -1 )
                        Com_PrintError(16, "Invalid counter string id: %s\n", counterType);
                    else
                        LiveCounter_IncrementCounterValue(counterId, increment);
                }
                else
                {
                    Com_PrintError(16, "Invalid param: <increment value> must be greater than or equal to zero\n");
                }
            }
        }
    }
    else
    {
        Com_PrintError(16, "Invalid param count. usage: incrementCounter( <counter id>, <increment value> )\n");
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetCounterTotal)
void GScr_GetCounterTotal()
{
    __int64 tempValue0; // rax
    char *fmtMsg;
    char *counterType;
    int counterId;
    int numParam;

    numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    counterType = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( numParam == 1 )
    {
        counterId = LiveCounter_CounterStringToID(counterType);
        if ( counterId == -1 )
        {
            Com_PrintError(16, "Invalid counter string id: %s\n", counterType);
            Scr_AddString((char *)"", SCRIPTINSTANCE_SERVER);
        }
        else
        {
            LODWORD(tempValue0) = LiveCounter_GetCounterTotalValue(counterId);
            if ( tempValue0 == -1 )
            {
                Scr_AddString((char *)"", SCRIPTINSTANCE_SERVER);
            }
            else
            {
                fmtMsg = va("%llu", tempValue0);
                Scr_AddString(fmtMsg, SCRIPTINSTANCE_SERVER);
            }
        }
    }
    else
    {
        Com_PrintError(16, "Invalid param count. usage: <string returned> getCounterTotal( <counter id> ).\n");
        Scr_AddString((char *)"", SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetScoreboardColumns)
void GScr_SetScoreboardColumns()
{
    char *String;
    char *tempValue1;
    signed int i;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 4 )
    {
        for ( i = 0; i < 4; ++i )
        {
            String = Scr_GetString(i, SCRIPTINSTANCE_SERVER);
            level_tail.teamScores[i - 24] = CScr_GetColumnTypeByName(String);
            if ( !level_tail.teamScores[i - 24] )
            {
                tempValue1 = Scr_GetString(i, SCRIPTINSTANCE_SERVER);
                Com_PrintError(16, "Invalid param: %s is not a valid column type.\n", tempValue1);
            }
        }
    }
    else
    {
        Com_PrintError(
            16,
            "Invalid param count. usage: setScoreboardColumns( <column1 name> <column2 name> <column3 name> <column4 name> ).\n");
        Scr_AddString((char *)"", SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetNemesisXuid)
void __cdecl GScr_SetNemesisXuid(scr_entref_t entref)
{
    const ddlState_t *RootDDLState;
    unsigned __int64 nemesisXuid;
    gentity_s *playerEnt;
    const char *nemesisXuidString;
    ddlState_t localState;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        nemesisXuidString = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        nemesisXuid = 0;
        playerEnt = GetEntity(entref);
        if ( playerEnt->client )
        {
            if ( nemesisXuidString )
            {
                RootDDLState = LiveStats_GetRootDDLState();
                DDL_MoveTo(RootDDLState, &localState, 2, "AfterActionReportStats", "nemesisXuid");
                StringToXUID(nemesisXuidString, &nemesisXuid);
                SV_SetClientDInt64Stat(playerEnt->s.number, &localState, nemesisXuid);
            }
        }
        else
        {
            Scr_Error("setnemesisxuid: entity must be a player entity", 0);
        }
    }
    else
    {
        Com_PrintError(16, "Invalid param count. usage: setnemesisxuid <xuid>.\n");
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsPregameEnabled)
void GScr_IsPregameEnabled()
{
    if ( Pregame_isEnabled() )
        Scr_AddBool(1u, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddBool(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ResetPregameData)
void GScr_ResetPregameData()
{
    Pregame_ResetData();
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsPregameGameStarted)
void GScr_IsPregameGameStarted()
{
    if ( Pregame_isEnabled() && Pregame_GetState() == PREGAME_GAMESTARTED )
        Scr_AddBool(1u, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddBool(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_PregameStartGame)
void GScr_PregameStartGame()
{
    Pregame_StartGame();
}

// Decomp: CoDSP_rdBlackOps.map.c (print_0)
void print_0()
{
    char *DebugString;
    int num;
    int i;

    if ( !g_NoScriptSpam->current.enabled )
    {
        num = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
        for ( i = 0; i < num; ++i )
        {
            DebugString = Scr_GetDebugString((unsigned int)i, SCRIPTINSTANCE_SERVER);
            Com_Printf(level.scriptPrintChannel, "%s", DebugString);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (println_0)
void println_0()
{
    if (!g_NoScriptSpam->current.enabled)
    {
        print_0();
        Com_Printf(level.scriptPrintChannel, "\n");
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (FUNCTION_NULLSUB)
void FUNCTION_NULLSUB()
{

}

// Decomp: CoDSP_rdBlackOps.map.c (assertCmd_0)
void assertCmd_0()
{
    if (!Scr_GetInt(0, SCRIPTINSTANCE_SERVER))
        Scr_Error("assert fail", 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (assertexCmd_0)
void assertexCmd_0()
{
    char *String;
    char *fmtMsg;

    if (!Scr_GetInt(0, SCRIPTINSTANCE_SERVER))
    {
        String = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("assert fail: %s", String);
        Scr_Error(fmtMsg, 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (assertmsgCmd_0)
void assertmsgCmd_0()
{
    char *String;
    char *fmtMsg;

    String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    fmtMsg = va("assert fail: %s", String);
    Scr_Error(fmtMsg, 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetPlayerStatsForMatchRecording)
void GScr_SetPlayerStatsForMatchRecording()
{
    DWORD result;
    char *statName;
    unsigned int statValue;
    gentity_s *ent;

    PROF_SCOPED("GScr_SetPlayerStatsForMatchRecording");

    if (Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 3)
        Scr_ParamError(0, "recordPlayerStats [player] [statName] [value]", SCRIPTINSTANCE_SERVER);
    ent = Scr_GetEntity(0);
    if (!ent)
        Scr_ParamError(0, "recordPlayerStats Error: param 0 is not an entity.", SCRIPTINSTANCE_SERVER);
    if (!ent->client)
        Scr_ParamError(0, "recordPlayerStats Error: param 0 is not an player.", SCRIPTINSTANCE_SERVER);
    statName = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    if (!statName)
        Scr_ParamError(1u, "recordPlayerStats Error: param 1 is not a string.", SCRIPTINSTANCE_SERVER);
    statValue = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);

#ifdef KISAK_LIVE
    MatchRecordSetPlayerStat(ent->client, statName, statValue);
#endif
}

// Decomp: CoDSP_rdBlackOps.map.c (missionDvarList / off_83126D6C)
static const char *missionDvarList[] =
{
    "mis_01",
    "mis_difficulty",
    "takeCoverWarnings",
    "r_gamma",
    "snd_menu_voice",
    "snd_menu_music",
    "snd_menu_sfx",
    "snd_menu_master",
    "snd_menu_cinematic",
    "start_in_mp",
    nullptr
};

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetLookatText @ 82617BD0)
void __cdecl GScr_SetLookatText(scr_entref_t entref)
{
    gentity_s *ent;
    unsigned int stringValue;

    ent = GetEntity(entref);
    if ( !ent )
    {
        if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_main.cpp", 0x4F37u, 0, "%s", "ent") )
            __debugbreak();
    }
    stringValue = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    Scr_SetString(&ent->lookAtText0, stringValue, SCRIPTINSTANCE_SERVER);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 1 )
    {
        stringValue = Scr_GetConstIString(1u, SCRIPTINSTANCE_SERVER);
        Scr_SetString(&ent->lookAtText1, stringValue, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetMissionDvar @ 82609F70)
void GScr_SetMissionDvar()
{
    const char *dvarName;
    const char **whitelistEntry;
    const dvar_s *dvar;
    const char *variantString;

    dvarName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    for ( whitelistEntry = missionDvarList; *whitelistEntry; ++whitelistEntry )
    {
        if ( !strcmp(dvarName, *whitelistEntry) )
            break;
    }
    if ( !*whitelistEntry )
        Scr_Error(va("Invalid mission dvar name: %s\n", dvarName), 0);
    dvar = Dvar_FindVar(dvarName);
    if ( !dvar )
        Scr_Error(va("Mission dvar has valid name, but was not found: %s\n", dvarName), 0);
    if ( dvar->type == DVAR_TYPE_INT )
    {
        Dvar_SetInt((dvar_s *)dvar, Scr_GetInt(1u, SCRIPTINSTANCE_SERVER));
        variantString = Dvar_GetVariantString(dvarName);
        Com_Printf(16, " * SetMissionDvar(): \"%s\" set to \"%s\".\n", dvarName, variantString);
        return;
    }
    if ( dvar->type == DVAR_TYPE_STRING )
    {
        Dvar_SetString((dvar_s *)dvar, Scr_GetString(1u, SCRIPTINSTANCE_SERVER));
        variantString = Dvar_GetVariantString(dvarName);
        Com_Printf(16, " * SetMissionDvar(): \"%s\" set to \"%s\".\n", dvarName, variantString);
        return;
    }
    Scr_Error(va("Mission dvar has valid name, but invalid type: %s is of type %i\n", dvarName, dvar->type), 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetNorthYaw @ 8260BBC8)
void GScr_SetNorthYaw()
{
    const char *northYawString;
    float northYawRadians;

    northYawString = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    SV_SetConfigstring(1548, (char *)northYawString);
    northYawRadians = (float)atof(northYawString) * 0.017453292f;
    level.compassNorth[0] = cos(northYawRadians);
    level.compassNorth[1] = sin(northYawRadians);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_SetPhysicsGravityDir @ 825F9080)
void ScrCmd_SetPhysicsGravityDir()
{
    float gravityDir[3];
    const char *fmtMsg;

    Scr_GetVector(0, gravityDir, SCRIPTINSTANCE_SERVER);
    fmtMsg = va("2 %g %g %g", gravityDir[0], gravityDir[1], gravityDir[2]);
    SV_GameSendServerCommand(-1, SV_CMD_RELIABLE, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetPlayerCollision @ 82620618)
void __cdecl Scr_SetPlayerCollision(scr_entref_t entref)
{
    gentity_s *ent;
    int enableCollision;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("Bad number of paramaters\n", 0);
    ent = GetEntity(entref);
    enableCollision = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( enableCollision )
        ent->flags &= ~0x1000000u;
    else
        ent->flags |= 0x1000000u;
    if ( ent->client )
    {
        if ( enableCollision )
            ent->client->ps.eFlags &= ~0x1000000u;
        else
            ent->client->ps.eFlags |= 0x1000000u;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetPlayerGravity @ 82620720)
void __cdecl Scr_SetPlayerGravity(scr_entref_t entref)
{
    gentity_s *ent;
    int gravity;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("Bad number of paramaters\n", 0);
    ent = GetEntity(entref);
    gravity = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( ent->client )
        ent->client->ps.gravity = gravity;
}

static unsigned __int16 Scr_CompressScale(float scale)
{
    if ( scale < 0.0f || scale > 10.0f )
    {
        if ( !Assert_MyHandler(
                    "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../cgame/cg_pose.h",
                    0x7Au,
                    0,
                    "%s\n\t(scale) = %g",
                    "(scale >= 0.0f && scale <= 10.0f)",
                    scale) )
        {
            __debugbreak();
        }
    }
    return (unsigned __int16)(int)(float)(scale * 6553.5f + 0.5f);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_SetScale @ 825FE458)
void __cdecl ScrCmd_SetScale(scr_entref_t entref)
{
    gentity_s *ent;
    float scale;

    ent = GetEntity(entref);
    scale = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    if ( ent->s.eType != ET_SCRIPTMOVER )
        Scr_Error("Can only set scale on script movers", 0);
    ent->s.un1.scale = (unsigned __int8)Scr_CompressScale(scale);
    ent->s.lerp.u.scriptMover.scale = Scr_CompressScale(scale);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_SetShadowHint @ 825FE4D8)
void __cdecl ScrCmd_SetShadowHint(scr_entref_t entref)
{
    gentity_s *ent;
    unsigned int hintString;
    int hintFlags;
    const char *hintName;

    ent = GetEntity(entref);
    hintString = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    hintName = SL_ConvertToString(hintString, SCRIPTINSTANCE_SERVER);
    hintFlags = 0;
    if ( !I_stricmp(hintName, "normal") )
        hintFlags = 0;
    else if ( !I_stricmp(hintName, "never") )
        hintFlags = 0x2000;
    else if ( !I_stricmp(hintName, "high_priority") )
        hintFlags = 0x6000;
    else if ( !I_stricmp(hintName, "low_priority") )
        hintFlags = 0x4000;
    else if ( !I_stricmp(hintName, "always") )
        hintFlags = 0x8000;
    else if ( !I_stricmp(hintName, "receiver") )
        hintFlags = 0xA000;
    else
        Scr_Error(
            "setshadowhint argument must be \"normal\", \"never\", \"high_priority\", \"low_priority\", \"always\", or \"receiver\".",
            0);
    ent->s.lerp.eFlags &= 0xFFFF1FFF;
    ent->s.lerp.eFlags |= hintFlags;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetSunDirection @ 82610D08)
void Scr_SetSunDirection()
{
    float sunDirection[3];
    const char *fmtMsg;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("Incorrect number of parameters\n", 0);
    Scr_GetVector(0, sunDirection, SCRIPTINSTANCE_SERVER);
    Vec3Normalize(sunDirection);
    fmtMsg = va("%g %g %g", sunDirection[0], sunDirection[1], sunDirection[2]);
    SV_SetConfigstring(9, (char *)fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetTransported @ 82607860)
void __cdecl GScr_SetTransported(scr_entref_t entref)
{
    gentity_s *ent;
    int clientNum;
    float transportTime;
    const char *fmtMsg;

    clientNum = -1;
    transportTime = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    if ( transportTime < 0.0 )
        Scr_ParamError(0, "Time must be positive", SCRIPTINSTANCE_SERVER);
    ent = GetEntity(entref);
    if ( ent && ent->r.inuse && ent->client )
        clientNum = ent->s.number;
    else
        Scr_Error("settransported() called on an invalid client entity.\n", 0);
    fmtMsg = va("] %i", (int)(float)(transportTime * 1000.0f));
    SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, fmtMsg);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetUINextLevel @ 8261AA20)
void GScr_SetUINextLevel()
{
    const char *mapName;

    mapName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    Dvar_SetStringByName("ui_mapname", (char *)mapName);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetVehicleAttachments @ 8261F810)
void __cdecl GScr_SetVehicleAttachments(scr_entref_t entref)
{
    gentity_s *ent;
    int enabled;

    ent = GetEntity(entref);
    enabled = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( enabled )
        ent->flags |= 0x2000000u;
    else
        ent->flags &= ~0x2000000u;
    SV_SendServerCommand(0, SV_CMD_RELIABLE, va("= %d %d", ent->s.number, enabled));
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetWaterBrush @ 8261C030)
void GScr_SetWaterBrush()
{
    gentity_s *ent;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        return;
    ent = Scr_GetEntity(0);
    Dvar_SetInt((dvar_s *)waterbrush_entity, ent->s.number);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetWeapon @ 825FC648)
void Scr_SetWeapon()
{
    gentity_s *ent;
    const char *weaponName;
    unsigned int weaponIndex;

    ent = Scr_GetEntity(0);
    weaponName = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName((char *)weaponName);
    if ( !weaponIndex )
        Scr_Error(va("SetWeapon called with unknown weapon name %s\n", weaponName), 0);
    ent->s.weapon = (unsigned __int16)weaponIndex;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetWetness @ 8260E6C8)
void __cdecl GScr_SetWetness(scr_entref_t entref)
{
    gentity_s *ent;
    gentity_s *tempEnt;
    float wetness;
    int invert;
    int eventParm;

    ent = GetEntity(entref);
    wetness = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    invert = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    if ( wetness < 0.0f )
        wetness = 0.0f;
    if ( wetness > 1.0f )
        wetness = 1.0f;
    invert = invert != 0;
    eventParm = (int)(float)(wetness * 127.0f) + (invert << 7);
    tempEnt = G_TempEntity(vec3_origin, EV_SETWETNESS);
    AssignToSmallerType<short>(&tempEnt->s.otherEntityNum, ent->s.number);
    tempEnt->s.eventParm = (unsigned __int16)eventParm;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetZombieName @ 8261F490)
void __cdecl GScr_SetZombieName(scr_entref_t entref)
{
    gentity_s *ent;
    const char *name;

    ent = GetEntity(entref);
    if ( !ent->actor )
    {
        Scr_Error(
            va(
                "SetZombieName must be called on an AI, not on a '%s'",
                SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER)),
            0);
    }
    name = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( ent->client )
    {
        I_strncpyz(ent->client->sess.newnetname, name, 32);
        I_strncpyz(ent->client->sess.cs.name, name, 32);
        ClientUserinfoChanged(ent->s.number);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_SetZombieShrink @ 825FE400)
void __cdecl ScrCmd_SetZombieShrink(scr_entref_t entref)
{
    gentity_s *ent;
    int shrink;

    ent = GetEntity(entref);
    shrink = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    ent->s.un1.scale = (unsigned __int8)shrink;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SnapshotAcknowledged @ 8260FC30)
void Scr_SnapshotAcknowledged()
{
    unsigned int parentId;
    unsigned int arraySize;
    unsigned int elementIndex;
    unsigned int entryId;
    VariableValueInternal *entryValue;
    int snapshotValue;
    bool acknowledged;
    client_t *cl;

    if ( Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) != VAR_ARRAY )
    {
        Scr_ParamError(
            0,
            va("Parameter (%s) must be an array", Scr_GetTypeName(0, SCRIPTINSTANCE_SERVER)),
            SCRIPTINSTANCE_SERVER);
    }
    parentId = Scr_GetObject(0, SCRIPTINSTANCE_SERVER);
    if ( !parentId )
    {
        if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_main.cpp", 0x3FCDu, 0, "%s", "parentId") )
            __debugbreak();
    }
    if ( GetObjectType(SCRIPTINSTANCE_SERVER, parentId) != VAR_ARRAY )
    {
        if ( !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_main.cpp",
                    0x3FCEu,
                    0,
                    "%s",
                    "GetObjectType( SCRIPTINSTANCE_SERVER, parentId ) == VAR_ARRAY") )
        {
            __debugbreak();
        }
    }
    arraySize = GetArraySize(SCRIPTINSTANCE_SERVER, parentId);
    acknowledged = true;
    for ( elementIndex = 0; elementIndex < arraySize; ++elementIndex )
    {
        entryId = GetArrayVariable(SCRIPTINSTANCE_SERVER, parentId, elementIndex);
        if ( !entryId )
        {
            if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_main.cpp", 0x3FD6u, 0, "%s", "id") )
                __debugbreak();
        }
        entryValue = &gScrVarGlob[0].variableList[entryId + 0x8000];
        if ( (entryValue->w.status & 0x60) == 0
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_scr_main.cpp",
                        0x3FD8u,
                        0,
                        "%s",
                        "(entryValue->w.status & VAR_STAT_MASK) != VAR_STAT_FREE") )
        {
            __debugbreak();
        }
        if ( (entryValue->w.type & 0x1F) == VAR_INTEGER )
        {
            snapshotValue = entryValue->u.u.intValue;
            if ( elementIndex < 4u )
            {
                if ( elementIndex < (unsigned int)com_maxclients->current.integer )
                {
                    cl = &svs.clients[elementIndex];
                    if ( cl->header.state == CS_ACTIVE && snapshotValue < cl->messageAcknowledge )
                        acknowledged = false;
                }
            }
            else if ( elementIndex == 4u )
            {
                if ( level.time < snapshotValue )
                    acknowledged = false;
            }
        }
    }
    if ( acknowledged )
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddUndefined(SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SplitViewAllowed @ 8260FA90)
void GScr_SplitViewAllowed()
{
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 3 )
    {
        Scr_Error(
            "Incorrect number of parameters\nUSAGE: splitViewAllowed (<clientNum>, <state>, <time (optional)>)\n",
            0);
    }
    // Split-view viewport control is client-only in retail; accept params so scripts do not error.
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_Start3DCinematic @ 8260A158)
void GScr_Start3DCinematic()
{
    const char *cinematicName;
    unsigned int paramCount;
    int cinematicFlags;
    const char *fmtMsg;

    cinematicFlags = 66;
    paramCount = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( paramCount > 4 )
    {
        Scr_Error(
            "start3DCinematic takes one, two, or three parameters: start3DCinematic(<cinematic name>, <looping>, <inmemory>)\n",
            0);
    }
    else if ( paramCount > 1 )
    {
        if ( paramCount > 2 )
        {
            if ( paramCount > 3 && Scr_GetInt(3u, SCRIPTINSTANCE_SERVER) )
                cinematicFlags = 194;
            if ( Scr_GetInt(2u, SCRIPTINSTANCE_SERVER) )
                cinematicFlags |= 8;
        }
        if ( !Scr_GetInt(1u, SCRIPTINSTANCE_SERVER) )
            cinematicFlags &= ~2;
    }
    cinematicName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( !I_stricmp(cinematicName, "mid_cuba_1")
        || !I_stricmp(cinematicName, "mid_cuba_3")
        || !I_stricmp(cinematicName, "mid_vorkuta_2")
        || !I_stricmp(cinematicName, "mid_vorkuta_3")
        || !I_stricmp(cinematicName, "mid_flashpoint_1")
        || !I_stricmp(cinematicName, "mid_flashpoint_2")
        || !I_stricmp(cinematicName, "mid_hue_city_2")
        || !I_stricmp(cinematicName, "mid_river_1")
        || !I_stricmp(cinematicName, "wmd_load")
        || !I_stricmp(cinematicName, "mid_rebirth_2")
        || !I_stricmp(cinematicName, "int_hudson_explains")
        || !I_stricmp(cinematicName, "int_reznov_disappearing_flashback") )
    {
        cinematicFlags |= 1;
    }
    fmtMsg = va("< %s %i", cinematicName, cinematicFlags);
    SV_GameSendServerCommand(-1, SV_CMD_RELIABLE, fmtMsg);
}

// --- CoDSP g_scr_main.obj handlers (batch 1/2) ---

static ScriptFunctions g_scr_cmdFunctions;

static ScriptFunctions *__cdecl GScr_GetCmdScriptFunctions()
{
    static int address[128];

    g_scr_cmdFunctions.maxSize = 128;
    g_scr_cmdFunctions.count = 0;
    g_scr_cmdFunctions.address = address;
    return &g_scr_cmdFunctions;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScrCmd_GetPlayerConnectionState @ 8261B318)
void __cdecl GScrCmd_GetPlayerConnectionState(scr_entref_t entref)
{
    gentity_s *ent;
    int clientNum;
    connstate_t state;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_Error("GetPlayerConnectionState: entity must be a player entity", 0);
    clientNum = ent - g_entities;
    state = clientUIActives[clientNum].connectionState;
    switch ( state )
    {
    case CA_DISCONNECTED:
        Scr_AddConstString(scr_const.ca_disconnected, SCRIPTINSTANCE_SERVER);
        break;
    case CA_CINEMATIC:
        Scr_AddConstString(scr_const.ca_cinematic, SCRIPTINSTANCE_SERVER);
        break;
    case CA_UICINEMATIC:
        Scr_AddConstString(scr_const.ca_uicinematic, SCRIPTINSTANCE_SERVER);
        break;
    case CA_LOGO:
        Scr_AddConstString(scr_const.ca_logo, SCRIPTINSTANCE_SERVER);
        break;
    case CA_CONNECTING:
        Scr_AddConstString(scr_const.ca_connecting, SCRIPTINSTANCE_SERVER);
        break;
    case CA_CHALLENGING:
        Scr_AddConstString(scr_const.ca_challenging, SCRIPTINSTANCE_SERVER);
        break;
    case CA_CONNECTED:
        Scr_AddConstString(scr_const.ca_connected, SCRIPTINSTANCE_SERVER);
        break;
    case CA_SENDINGSTATS:
        Scr_AddConstString(scr_const.ca_sendingstats, SCRIPTINSTANCE_SERVER);
        break;
    case CA_LOADING:
        Scr_AddConstString(scr_const.ca_loading, SCRIPTINSTANCE_SERVER);
        break;
    case CA_PRIMED:
        Scr_AddConstString(scr_const.ca_primed, SCRIPTINSTANCE_SERVER);
        break;
    case CA_ACTIVE:
        Scr_AddConstString(scr_const.ca_active, SCRIPTINSTANCE_SERVER);
        break;
    default:
        Scr_AddConstString(scr_const.ca_map_restart, SCRIPTINSTANCE_SERVER);
        break;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScrCmd_SetWaterSheeting @ 8261E960)
void __cdecl GScrCmd_SetWaterSheeting(scr_entref_t entref)
{
    gentity_s *ent;
    int enable;
    int durationMs;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_Error("SetWaterSheeting called on an ent that's not a player.", 0);
    enable = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    durationMs = 0;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 2 )
        durationMs = (int)(Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER) * 1000.0f);
    SV_GameSendServerCommand(
        ent - g_entities,
        SV_CMD_RELIABLE,
        va("%c %i %i", 33, enable, durationMs));
}

void GScr_AddEntityCmd()
{
    GScr_AddEntity(Scr_GetEntity(0));
}

void GScr_AddVectorCmd()
{
    float vec[3];

    Scr_GetVector(0, vec, SCRIPTINSTANCE_SERVER);
    GScr_AddVector(vec);
}

void GScr_AllocStringCmd()
{
    Scr_AddConstString(GScr_AllocString(Scr_GetString(0, SCRIPTINSTANCE_SERVER)), SCRIPTINSTANCE_SERVER);
}

void GScr_AnimscriptAllocCmd()
{
    Scr_AddInt((int)GScr_AnimscriptAlloc(Scr_GetInt(0, SCRIPTINSTANCE_SERVER)), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_CoopInfo @ 825F22E8)
void GScr_CoopInfo()
{
    const char *msgType;
    gentity_s *player;
    int msgId;
    int targetClient;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 2 || Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 3 )
        Scr_Error("Incorrect number of parameters", 0);
    player = Scr_GetEntity(1u);
    if ( !player->client )
        Scr_Error("Parameter 2 (player) is either invalid or not a player", 0);
    msgType = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    msgId = -1;
    if ( !I_stricmp(msgType, "msgcoop_1stplace") )
        msgId = 0;
    else if ( !I_stricmp(msgType, "msgcoop_playerdown") )
        msgId = 1;
    else if ( !I_stricmp(msgType, "msgcoop_killstreakwon") )
        msgId = 2;
    else if ( !I_stricmp(msgType, "msgcoop_killstreaklost") )
        msgId = 3;
    else if ( !I_stricmp(msgType, "msgcoop_message_player") )
        msgId = 4;
    else if ( !I_stricmp(msgType, "msgcoop_message_all") )
        msgId = 5;
    else if ( !I_stricmp(msgType, "msgcoop_collectible") )
        msgId = 6;
    if ( msgId < 0 )
        Scr_Error("Parameter 1 (msgcoop_*) is invalid", 0);
    if ( msgId < 2 )
    {
        SV_GameSendServerCommand(
            player - g_entities,
            SV_CMD_RELIABLE,
            va("%c %i", 94, msgId));
        return;
    }
    if ( msgId == 2 || msgId == 3 )
    {
        SV_GameSendServerCommand(
            player - g_entities,
            SV_CMD_CAN_IGNORE,
            va("%c %i", 94, msgId));
        return;
    }
    if ( msgId == 4 || msgId == 5 )
    {
        const char *text = Scr_GetString(2u, SCRIPTINSTANCE_SERVER);
        targetClient = msgId == 4 ? (player - g_entities) : -1;
        SV_GameSendServerCommand(
            targetClient,
            SV_CMD_CAN_IGNORE,
            va("%c %i %i %s", 94, msgId, player - g_entities, text));
        return;
    }
    SV_GameSendServerCommand(
        -1,
        SV_CMD_CAN_IGNORE,
        va("%c %i %i", 94, msgId, player - g_entities));
}

void __cdecl GScr_DamageConeTraceInternalCmd(scr_entref_t entref)
{
    GScr_DamageConeTraceInternal(entref, 8396819);
}

void GScr_FindAnimTreeCmd()
{
    const char *treeName;

    treeName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( !Scr_FindAnimTree(SCRIPTINSTANCE_SERVER, treeName).anims )
        Com_Error(ERR_DROP, "Could not find animtree '%s'", treeName);
}

void GScr_FindAnimTreesCmd()
{
    GScr_FindAnimTrees(SCRIPTINSTANCE_SERVER);
}

void GScr_FreeScriptsCmd()
{
    GScr_FreeScripts(SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetClosestFrameForTime @ 825F5F98) — stub until AD_* anim dump module is ported
void GScr_GetClosestFrameForTime()
{
    (void)Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    (void)Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    Scr_AddString("", SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetDvarVec3 @ 825F44C8)
void GScr_GetDvarVec3()
{
    const dvar_s *dvar;
    const char *name;
    int index;

    if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) == VAR_INTEGER )
    {
        index = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        dvar = Dvar_FindVar((int)index);
    }
    else
    {
        name = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        dvar = Dvar_FindVar(name);
    }
    if ( !dvar )
        Scr_Error("Invalid Dvar passed into GetDvarVec", 0);
    if ( dvar->type != DVAR_TYPE_FLOAT_3 )
        Scr_Error("dvar is not a vector in but GetDvarVec has been called on it", 0);
    Scr_AddVector((float *)dvar->current.vector, SCRIPTINSTANCE_SERVER);
}

void GScr_GetHeadIconIndexCmd()
{
    Scr_AddInt(GScr_GetHeadIconIndex(Scr_GetString(0, SCRIPTINSTANCE_SERVER)), SCRIPTINSTANCE_SERVER);
}

void GScr_GetLocSelIndexCmd()
{
    Scr_AddInt(GScr_GetLocSelIndex(Scr_GetString(0, SCRIPTINSTANCE_SERVER)), SCRIPTINSTANCE_SERVER);
}

void GScr_GetOptionalFloatCmd()
{
    Scr_AddFloat(
        GScr_GetOptionalFloat(0, Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER)),
        SCRIPTINSTANCE_SERVER);
}

void GScr_GetScriptMenuIndexCmd()
{
    Scr_AddInt(GScr_GetScriptMenuIndex(Scr_GetString(0, SCRIPTINSTANCE_SERVER)), SCRIPTINSTANCE_SERVER);
}

void GScr_GetStatusIconIndexCmd()
{
    Scr_AddInt(GScr_GetStatusIconIndex(Scr_GetString(0, SCRIPTINSTANCE_SERVER)), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetTotalTime @ 825F6078) — stub until AD_* anim dump module is ported
void GScr_GetTotalTime()
{
    Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
}

void GScr_GetWeaponDefCmd()
{
    Scr_AddInt((int)GScr_LookupWeaponDef(), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_HasPlayerUsedWeapon @ 82619838)
void GScr_HasPlayerUsedWeapon(scr_entref_t entref)
{
    gentity_s *ent;
    int clientNum;
    char weaponNames[32][128];
    unsigned int weaponMask[4];
    int exclusive;
    int weaponCount;
    unsigned int resultMask;
    VariableValue arrayValue;
    unsigned int arrayId;
    unsigned int elementId;
    int i;
    int j;
    int k;
    unsigned int weaponIndex;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_Error("GetPlayerConnectionState: entity must be a player entity", 0);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2 )
        Scr_Error("Requires 2 params.  Params are weaponname and bool for exclusive check.\n", 0);
    if ( Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) != VAR_ARRAY )
        Scr_ParamError(0, va("Parameter (%s) must be an array", Scr_GetTypeName(0, SCRIPTINSTANCE_SERVER)), SCRIPTINSTANCE_SERVER);
    exclusive = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    clientNum = ent - g_entities;
    memset(weaponNames, 0, sizeof(weaponNames));
    memset(weaponMask, 0, sizeof(weaponMask));
    weaponCount = 0;
    arrayId = Scr_GetObject(0, SCRIPTINSTANCE_SERVER);
    for ( elementId = FindFirstSibling(SCRIPTINSTANCE_SERVER, arrayId); elementId; elementId = FindNextSibling(SCRIPTINSTANCE_SERVER, elementId) )
    {
        arrayValue = Scr_GetArrayIndexValue(SCRIPTINSTANCE_SERVER, elementId);
        if ( arrayValue.type != VAR_STRING )
            Scr_Error(va("Array passed to hasusedweapon contained member [%i] - valid types are string.", weaponCount), 0);
        if ( weaponCount >= 32 )
            break;
        I_strncpyz(weaponNames[weaponCount], SL_ConvertToString(arrayValue.u.stringValue, SCRIPTINSTANCE_SERVER), 128);
        ++weaponCount;
    }
    if ( weaponCount && !I_stricmp(weaponNames[0], "none") )
    {
        for ( i = 0; i < 4; ++i )
        {
            if ( BG_GetWeaponUsedMaskWord(clientNum, i) )
            {
                Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
                return;
            }
        }
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
        return;
    }
    resultMask = 0;
    for ( j = 0; j < weaponCount; ++j )
    {
        if ( !weaponNames[j][0] )
            break;
        weaponIndex = G_GetWeaponIndexForName(weaponNames[j]);
        if ( BG_HasWeaponBeenUsed(clientNum, weaponIndex) )
        {
            resultMask |= 1u << j;
            weaponMask[weaponIndex / 32] |= 1u << (weaponIndex % 32);
        }
    }
    if ( resultMask )
    {
        if ( exclusive )
        {
            for ( k = 0; k < 4; ++k )
            {
                if ( BG_GetWeaponUsedMaskWord(clientNum, k) && BG_GetWeaponUsedMaskWord(clientNum, k) != weaponMask[k] )
                {
                    Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
                    return;
                }
            }
            Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
            return;
        }
        Scr_AddInt(resultMask, SCRIPTINSTANCE_SERVER);
        return;
    }
    Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
}

void __cdecl GScr_RadiusDamageInternalCmd(scr_entref_t entref)
{
    GScr_RadiusDamageInternal(GetEntity(entref));
}

void __cdecl GScr_SetAnimInternalCmd(scr_entref_t entref)
{
    GScr_SetAnimInternal(entref, 1, false);
}

void __cdecl GScr_SetAnimKnobInternalCmd(scr_entref_t entref)
{
    GScr_SetAnimKnobInternal(entref, 1, false);
}

void __cdecl GScr_SetAnimKnobAllInternalCmd(scr_entref_t entref)
{
    GScr_SetAnimKnobAllInternal(entref, 1, false, 0);
}

void GScr_SetAnimScriptsCmd()
{
    GScr_SetAnimScripts(GScr_GetCmdScriptFunctions());
}

void GScr_SetDogAnimScriptsCmd()
{
    GScr_SetDogAnimScripts(GScr_GetCmdScriptFunctions());
}

void GScr_SetExponentialFogCmd()
{
    Scr_SetExponentialFog();
}

void __cdecl GScr_SetFlaggedAnimInternalCmd(scr_entref_t entref)
{
    GScr_SetAnimInternal(entref, 1, true);
}

void __cdecl GScr_SetFlaggedAnimKnobInternalCmd(scr_entref_t entref)
{
    GScr_SetAnimKnobInternal(entref, 1, true);
}

void __cdecl GScr_SetFlaggedAnimKnobAllInternalCmd(scr_entref_t entref)
{
    GScr_SetAnimKnobAllInternal(entref, 1, true, "illegal call to SetFlaggedAnimKnobAll()\n");
}

void GScr_SetScriptsCmd()
{
    ScriptFunctions *functions;

    functions = GScr_GetCmdScriptFunctions();
    g_scr_data.delete_ = GScr_SetScriptAndLabel(functions, "codescripts/delete", "main", 1);
    g_scr_data.initstructs = GScr_SetScriptAndLabel(functions, "codescripts/struct", "initstructs", 1);
    g_scr_data.createstruct = GScr_SetScriptAndLabel(functions, "codescripts/struct", "createstruct", 1);
    g_scr_data.findstruct = GScr_SetScriptAndLabel(functions, "codescripts/struct", "findstruct", 1);
    if ( zombiemode->current.enabled )
    {
        GScr_SetZombieAnimScripts(functions);
        GScr_SetZombieDogAnimScripts(functions);
    }
    else
    {
        GScr_SetAnimScripts(functions);
    }
    GScr_SetLevelScript(functions);
    GScr_SetScriptsAndAnimsForEntities(functions);
    GScr_SetScriptsForPathNodes(functions);
}

void GScr_SetScriptsAndAnimsForEntitiesCmd()
{
    GScr_SetScriptsAndAnimsForEntities(GScr_GetCmdScriptFunctions());
}

void GScr_SetScriptsForPathNodeCmd()
{
    pathnode_t *node;

    node = Scr_GetPathnode(0, SCRIPTINSTANCE_SERVER);
    GScr_SetScriptsForPathNode(SCRIPTINSTANCE_SERVER, node, GScr_GetCmdScriptFunctions());
}

void GScr_SetScriptsForPathNodesCmd()
{
    GScr_SetScriptsForPathNodes(GScr_GetCmdScriptFunctions());
}

void GScr_SetSingleAnimScriptCmd()
{
    scr_animscript_t *pAnim;
    const char *name;

    pAnim = (scr_animscript_t *)Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    name = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    GScr_SetSingleAnimScript(GScr_GetCmdScriptFunctions(), pAnim, name);
}

void GScr_SetZombieAnimScriptsCmd()
{
    GScr_SetZombieAnimScripts(GScr_GetCmdScriptFunctions());
}

void GScr_SetZombieDogAnimScriptsCmd()
{
    GScr_SetZombieDogAnimScripts(GScr_GetCmdScriptFunctions());
}

void __cdecl GScr_SetupLightEntityCmd(scr_entref_t entref)
{
    GScr_AddEntity(GScr_SetupLightEntity(entref));
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_StartXDoll @ 8261E398)
void __cdecl GScr_StartXDoll(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    ent->s.eType = ET_ACTOR;
    ent->physicsObject = 1;
}

void __cdecl GScr_UpdateTagInternalCmd(scr_entref_t entref)
{
    gentity_s *ent;
    unsigned int tagName;

    ent = GetEntity(entref);
    tagName = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    GScr_UpdateTagInternal(ent, tagName, &level.cachedTagMat, 1);
}

// Decomp: CoDSP_rdBlackOps.map.c (SV_FX_GetVisibility @ 8294B4B0)
static double SV_FX_GetVisibility(const float *start, const float *end)
{
    return FX_GetClientVisibility(0, start, end);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_HACKGetGameGenericHumanTree @ 825F02F8)
scr_animtree_t __cdecl Scr_HACKGetGameGenericHumanTree()
{
    return g_gameGenericHumanTree;
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_PrecacheSpawn @ 825FA910)
void __cdecl ScrCmd_PrecacheSpawn(scr_entref_t entref)
{
    Scr_AddUndefined(SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_UpdateGamerProfile @ 826195A8)
void ScrCmd_UpdateGamerProfile()
{
    Cbuf_AddText(0, "updategamerprofile\n");
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PrecacheMaterial @ 82607D18)
void Scr_PrecacheMaterial()
{
    char *materialName;

    if ( !level.initializing )
        Scr_Error("precacheShader must be called before any wait statements in the gametype or level script\n", 0);
    materialName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( !*materialName )
        Scr_ParamError(0, "Shader name string is empty", SCRIPTINSTANCE_SERVER);
    Scr_ErrorOnDefaultAsset(ASSET_TYPE_MATERIAL, materialName);
    G_MaterialIndex(materialName);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PrecacheRumble @ 82607E38)
void Scr_PrecacheRumble()
{
    char *rumbleName;

    if ( !level.initializing )
        Scr_Error("precacheRumble must be called before any wait statements in the level script\n", 0);
    rumbleName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    G_RumbleIndex(rumbleName);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_PlayRumbleOnEntity_Internal @ 8261CCD8)
void __cdecl ScrCmd_PlayRumbleOnEntity_Internal(scr_entref_t entref, int cmd)
{
    gentity_s *ent;
    char *rumbleName;
    int rumbleIndex;
    const char *fmtMsg;
    int clientNum;

    ent = GetEntity(entref);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("Incorrect number of parameters.\n", 0);
    rumbleName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    rumbleIndex = G_RumbleIndex(rumbleName);
    if ( !rumbleIndex )
    {
        fmtMsg = va("unknown rumble name '%s'", rumbleName);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    if ( cmd == 97 )
        ent->flags |= 0x1000u;
    clientNum = ent->s.clientNum;
    if ( clientNum >= 0 && clientNum < level.maxclients )
        SV_GameSendServerCommand(clientNum, SV_CMD_CAN_IGNORE, va("%c %d %d %d", '*', cmd, ent->s.number, rumbleIndex));
    else
        SV_GameSendServerCommand(-1, SV_CMD_CAN_IGNORE, va("%c %d %d %d", '*', cmd, ent->s.number, rumbleIndex));
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_PlayRumbleOnEntity @ 8261CC98)
void __cdecl ScrCmd_PlayRumbleOnEntity(scr_entref_t entref)
{
    ScrCmd_PlayRumbleOnEntity_Internal(entref, 95);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_PlayRumbleLoopOnEntity @ 8261CE80)
void __cdecl ScrCmd_PlayRumbleLoopOnEntity(scr_entref_t entref)
{
    ScrCmd_PlayRumbleOnEntity_Internal(entref, 97);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_StopRumble @ 8261CEC0)
void __cdecl ScrCmd_StopRumble(scr_entref_t entref)
{
    gentity_s *ent;
    char *rumbleName;
    int rumbleIndex;
    const char *fmtMsg;
    int clientNum;

    ent = GetEntity(entref);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("Incorrect number of parameters.\n", 0);
    rumbleName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    rumbleIndex = G_RumbleIndex(rumbleName);
    if ( !rumbleIndex )
    {
        fmtMsg = va("unknown rumble name '%s'", rumbleName);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    ent->flags &= ~0x1000u;
    clientNum = ent->s.clientNum;
    if ( clientNum >= 0 && clientNum < level.maxclients )
        SV_GameSendServerCommand(clientNum, SV_CMD_CAN_IGNORE, va("%c %d %d %d", '*', 99, ent->s.number, rumbleIndex));
    else
        SV_GameSendServerCommand(-1, SV_CMD_CAN_IGNORE, va("%c %d %d %d", '*', 99, ent->s.number, rumbleIndex));
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PlayRumbleOnPosition_Internal @ 826193F0)
void __cdecl Scr_PlayRumbleOnPosition_Internal(entity_event_t event)
{
    const char *fmtMsg;
    char *rumbleName;
    int rumbleIndex;
    float pos[3];
    gentity_s *ent;

    rumbleName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    rumbleIndex = G_RumbleIndex(rumbleName);
    if ( !rumbleIndex )
    {
        fmtMsg = va("unknown rumble name '%s'", rumbleName);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    Scr_GetVector(1u, pos, SCRIPTINSTANCE_SERVER);
    ent = G_TempEntity(pos, event);
    ent->s.weapon = rumbleIndex;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PlayRumbleOnPosition @ 826193A8)
void Scr_PlayRumbleOnPosition()
{
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2 )
        Scr_ParamError(0, "PlayRumbleOnPosition [rumble name] [pos]", SCRIPTINSTANCE_SERVER);
    Scr_PlayRumbleOnPosition_Internal((entity_event_t)96);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PlayRumbleLoopOnPosition @ 826194A0)
void Scr_PlayRumbleLoopOnPosition()
{
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2 )
        Scr_ParamError(0, "PlayRumbleLoopOnPosition [rumble name] [pos]", SCRIPTINSTANCE_SERVER);
    Scr_PlayRumbleOnPosition_Internal((entity_event_t)98);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_StopAllRumbles @ 826194E8)
void Scr_StopAllRumbles()
{
    float pos[3];
    gentity_s *ent;

    pos[0] = 0.0f;
    pos[1] = 0.0f;
    pos[2] = 0.0f;
    ent = G_TempEntity(pos, (entity_event_t)100);
    ent->s.weapon = 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetFXVis @ 8260CB08)
void Scr_GetFXVis()
{
    float start[3];
    float end[3];
    double visibility;

    Scr_GetVector(0, start, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, end, SCRIPTINSTANCE_SERVER);
    visibility = SV_FX_GetVisibility(start, end);
    Scr_AddFloat((float)visibility, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Fire @ 825FC708)
void Scr_Fire()
{
    const char *fmtMsg;
    const char *WeaponTypeName;
    gentity_s *firer;
    gentity_s *attacker;
    weaponParms wp;
    float forward[3];
    float right[3];
    float up[3];
    float targetOffset[3];
    const WeaponDef *weapDef;
    weapType_t weapType;
    gentity_s *projectile;
    int weapon;

    firer = Scr_GetEntity(0);
    attacker = Scr_GetEntity(1u);
    weapon = firer->s.weapon;
    if ( !weapon )
    {
        if ( !Assert_MyHandler("C:\\projects\\cod\\codsrc\\src\\game\\g_scr_main.cpp", 0x1862u, 0, "%s", "weapon != WP_NONE") )
            __debugbreak();
    }
    Weapon_SetWeaponParamsWeapon(&wp, weapon);
    AngleVectors(firer->r.currentAngles, forward, right, up);
    wp.forward[0] = forward[0];
    wp.forward[1] = forward[1];
    wp.forward[2] = forward[2];
    wp.muzzleTrace[0] = firer->r.currentOrigin[0];
    wp.muzzleTrace[1] = firer->r.currentOrigin[1];
    wp.muzzleTrace[2] = firer->r.currentOrigin[2];
    memset(targetOffset, 0, sizeof(targetOffset));
    weapDef = wp.weapDef;
    if ( weapDef->weapType == WEAPTYPE_GRENADE || weapDef->weapType == WEAPTYPE_MINE )
        Scr_Error("Fire() does not work with grenade-type weapons.\n", 0);
    projectile = 0;
    weapType = weapDef->weapType;
    if ( weapType )
    {
        if ( weapType == WEAPTYPE_PROJECTILE )
        {
            switch ( weapDef->weapClass )
            {
                case WEAPCLASS_GRENADE:
                    Weapon_GrenadeLauncher_Fire(attacker, weapon, 0, &wp);
                    break;
                case WEAPCLASS_ROCKETLAUNCHER:
                    Weapon_RocketLauncher_Fire(
                        attacker,
                        weapon,
                        0.0,
                        &wp,
                        vec3_origin,
                        0,
                        targetOffset);
                    break;
                case WEAPCLASS_TURRET:
                    if ( weapDef->weapType == WEAPTYPE_PROJECTILE )
                    {
                        Weapon_RocketLauncher_Fire(
                            attacker,
                            weapon,
                            0.0,
                            &wp,
                            vec3_origin,
                            0,
                            targetOffset);
                    }
                    else if ( weapDef->weapType == WEAPTYPE_BULLET )
                    {
                        Bullet_Fire(attacker, 0.0, &wp, firer, level.time);
                    }
                    break;
                default:
                    Scr_Error("Fire(): Unhandled projectile weapClass.\n", 0);
                    break;
            }
        }
        else if ( weapType == WEAPTYPE_GAS )
        {
            Weapon_Flamethrower_Fire(firer, &wp);
        }
        else
        {
            WeaponTypeName = BG_GetWeaponTypeName(weapDef->weapType);
            fmtMsg = va("Fire(): Unhandled weapType \"%s\".\n", WeaponTypeName);
            Scr_Error(fmtMsg, 0);
        }
    }
    else
    {
        Bullet_Fire(attacker, 0.0, &wp, firer, level.time);
    }
    G_AddEvent(firer, EV_FIRE_WEAPON, 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_RopeBreak @ 8260D750)
void Scr_RopeBreak()
{
    Com_Printf(
        25,
        "RopeBreak is obsolete. Use roperemoveanchor( ropeid, 0/1 ) instead.\n");
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetAnimRateForLeaf_r @ 82616578)
void __cdecl Scr_SetAnimRateForLeaf_r(gentity_s *ent, XAnimTree_s *tree, float rate, unsigned int infoIndex)
{
    XAnim_s *anims;
    XAnimInfo *info;
    unsigned int nextInfoIndex;
    unsigned int childInfoIndex;

    iassert(ent);
    iassert(tree);
    anims = XAnimGetAnims(tree);
    info = GetAnimInfo(infoIndex);
    nextInfoIndex = info->next;
    if ( nextInfoIndex )
        Scr_SetAnimRateForLeaf_r(ent, tree, rate, nextInfoIndex);
    childInfoIndex = info->children;
    if ( childInfoIndex )
        Scr_SetAnimRateForLeaf_r(ent, tree, rate, childInfoIndex);
    if ( XAnimIsPrimitive(anims, info->animIndex) )
        XAnimSetAnimRate(tree, info->animIndex, rate);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_WeaponGetBayonetInfo @ 8260A808)
void GScr_WeaponGetBayonetInfo()
{
    const char *weaponName;
    unsigned int weaponIndex;
    const WeaponDef *weapDef;
    const WeaponVariantDef *weapVarDef;

    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName((char *)weaponName);
    Scr_VerifyWeaponIndex(weaponIndex, weaponName);
    weapDef = BG_GetWeaponDef(weaponIndex);
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    if ( !BG_IsBayonetWeapon(weaponIndex) )
        return;
    Scr_AddInt(weapDef->iMeleeDamage, SCRIPTINSTANCE_SERVER);
    Scr_AddArray(SCRIPTINSTANCE_SERVER);
    weapVarDef = BG_GetWeaponVariantDef(weaponIndex);
    if ( weapVarDef->szXAnims[7] && weapVarDef->szXAnims[7][0] )
        Scr_AddString(weapVarDef->szXAnims[7], SCRIPTINSTANCE_SERVER);
    else
        Scr_AddString("", SCRIPTINSTANCE_SERVER);
    Scr_AddArray(SCRIPTINSTANCE_SERVER);
    Scr_AddInt(weapDef->iMeleeDelay, SCRIPTINSTANCE_SERVER);
    Scr_AddArray(SCRIPTINSTANCE_SERVER);
    if ( weapVarDef->szXAnims[8] && weapVarDef->szXAnims[8][0] )
        Scr_AddString(weapVarDef->szXAnims[8], SCRIPTINSTANCE_SERVER);
    else
        Scr_AddString("", SCRIPTINSTANCE_SERVER);
    Scr_AddArray(SCRIPTINSTANCE_SERVER);
    Scr_AddInt(weapDef->iMeleeTime, SCRIPTINSTANCE_SERVER);
    Scr_AddArray(SCRIPTINSTANCE_SERVER);
    Scr_AddFloat(player_bayonetRange->current.value, SCRIPTINSTANCE_SERVER);
    Scr_AddArray(SCRIPTINSTANCE_SERVER);
    Scr_AddFloat(Dvar_GetFloat("aim_autobayonet_range"), SCRIPTINSTANCE_SERVER);
    Scr_AddArray(SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_CodeForceSpawn @ 825FA6C8)
void __cdecl ScrCmd_CodeForceSpawn(scr_entref_t entref)
{
    ScrCmd_CodeSpawnerForceSpawn(entref);
}

BuiltinFunctionDef functions[] =
{
  { "createprintchannel", GScr_CreatePrintChannel, 1 },
  { "setprintchannel", GScr_printChannelSet, 1 },
  { "printchannelset", GScr_printChannelSet, 1 },
  { "print", print_0, 1 },
  { "println", println_0, 1 },
  { "iprintln", iprintln, 0 },
  { "iprintlnbold", iprintlnbold, 0 },
  { "print3d", GScr_print3d, 1 },
  { "box", GScr_box, 1 },
  { "line", GScr_line, 1 },
  { "linelist", GScr_linelist, 1 },
  { "debugstar", GScr_debugstar, 1 },
  { "circle", GScr_circle, 1 },
  { "sphere", GScr_sphere, 1 },
  { "getreflectionlocs", Scr_GetReflectionLocs, 1 },
  { "getreflectionorigin", Scr_GetReflectionOrigin, 1 },
  { "logstring", FUNCTION_NULLSUB, 0 },
  { "bbprint", FUNCTION_NULLSUB, 0 },
  { "getent", Scr_GetEnt, 0 },
  { "getentarray", Scr_GetEntArray, 0 },
  { "entsearch", Scr_EntSearch, 0 },
  { "getdynmodels", Scr_GetDynModels, 0 },
  { "getmiscmodels", Scr_GetMiscModels, 0 },
  { "getdestructibledefs", Scr_GetDestructibleDefs, 0 },
  { "getcycleoriginoffset", GScr_GetCycleOriginOffset, 0 },
  { "getaiarray", Scr_GetAIArray, 0 },
  { "getaispeciesarray", Scr_GetAISpeciesArray, 0 },
  { "getspawnerarray", Scr_GetSpawnerArray, 0 },
  { "getspawnerteamarray", Scr_GetSpawnerTeamArray, 0 },
  { "getscriptmoverarray", Scr_GetScriptMoverArray, 0 },
  { "getitemarray", Scr_GetItemArray, 0 },
  { "getnumrestarts", Scr_GetNumRestarts, 0 },
  { "getmapsundirection", Scr_GetMapSunDirection, 0 },
  { "getmapsunlight", Scr_GetMapSunLight, 0 },
  { "getkeybinding", GScr_GetKeyBinding, 0 },
  { "getdumpcmdsfortimedelta", GScr_GetDumpCmdsForTimeDelta, 0 },
  { "getclosestframefortime", GScr_GetClosestFrameForTime, 0 },
  { "getclosestanimdumpframefortime", GScr_GetClosestFrameForTime, 0 },
  { "gettotaltime", GScr_GetTotalTime, 0 },
  { "getanimdumptotaltime", GScr_GetTotalTime, 0 },
  { "getdvarvec3", GScr_GetDvarVec3, 0 },
  { "getdvarvector", GScr_GetDvarVec3, 0 },
  { "getheadiconindex", GScr_GetHeadIconIndexCmd, 0 },
  { "getlocselindex", GScr_GetLocSelIndexCmd, 0 },
  { "getoptionalfloat", GScr_GetOptionalFloatCmd, 0 },
  { "getscriptmenuindex", GScr_GetScriptMenuIndexCmd, 0 },
  { "getstatusiconindex", GScr_GetStatusIconIndexCmd, 0 },
  { "getweapondef", GScr_GetWeaponDefCmd, 0 },
  { "weapongetbayonetinfo", GScr_WeaponGetBayonetInfo, 0 },
  { "weaponbayonetinfo", GScr_WeaponGetBayonetInfo, 0 },
  { "addentity", GScr_AddEntityCmd, 0 },
  { "addvector", GScr_AddVectorCmd, 0 },
  { "allocstring", GScr_AllocStringCmd, 0 },
  { "animscriptalloc", GScr_AnimscriptAllocCmd, 0 },
  { "coopinfo", GScr_CoopInfo, 0 },
  { "findanimtree", GScr_FindAnimTreeCmd, 0 },
  { "findanimtrees", GScr_FindAnimTreesCmd, 0 },
  { "freescripts", GScr_FreeScriptsCmd, 0 },
  { "setscripts", GScr_SetScriptsCmd, 0 },
  { "setanimscripts", GScr_SetAnimScriptsCmd, 0 },
  { "setdoganimscripts", GScr_SetDogAnimScriptsCmd, 0 },
  { "setzombieanimscripts", GScr_SetZombieAnimScriptsCmd, 0 },
  { "setzombiedoganimscripts", GScr_SetZombieDogAnimScriptsCmd, 0 },
  { "setsingleanimscript", GScr_SetSingleAnimScriptCmd, 0 },
  { "setscriptsandanimsforentities", GScr_SetScriptsAndAnimsForEntitiesCmd, 0 },
  { "setscriptsforpathnodes", GScr_SetScriptsForPathNodesCmd, 0 },
  { "setscriptsforpathnode", GScr_SetScriptsForPathNodeCmd, 0 },
  { "shutdown", GScr_Shutdown, 0 },
  { "radiusdamageinternal", GScr_RadiusDamage, 0 },
  { "isanimleaf", GScr_IsAnimLeaf, 0 },
  { "isanimlooping", GScr_IsAnimLooping, 0 },
  { "getsnapshotindexarray", Scr_GetSnapshotIndexArray, 0 },
  { "createthreatbiasgroup", SentientCmd_CreateThreatBiasGroup, 0 },
  { "getthreatbias", GScr_GetThreatBias, 0 },
  { "setthreatbias", SentientCmd_SetThreatBias, 0 },
  { "setthreatbiasagainstall", SentientCmd_SetThreatBiasAgainstAll, 0 },
  { "threatbiasgroupexists", SentientCmd_ThreatBiasGroupExists, 0 },
  { "setignoremegroup", SentientCmd_SetIgnoreMeGroup, 0 },
  { "getweaponaccuracy", GScr_GetWeaponAccuracy, 0 },
  { "oktospawn", GScr_OkToSpawn, 0 },
  { "isnodeoccupied", Scr_IsNodeOccupied, 0 },
  { "badplace_cylinder", Scr_BadPlace_Cylinder, 0 },
  { "badplace_arc", Scr_BadPlace_Arc, 0 },
  { "badplace_brush", Scr_BadPlace_Brush, 0 },
  { "badplace_delete", Scr_BadPlace_Delete, 0 },
  { "badplacesenable", GScr_BadPlacesEnable, 0 },
  { "getcinematictimeremaining", GScr_GetCinematicTimeRemaining, 0 },
  { "setailimit", GScr_SetAILimit, 0 },
  { "disablegrenadesuicide", GScr_DisableGrenadeSuicide, 0 },
  { "enablegrenadesuicide", GScr_EnableGrenadeSuicide, 0 },
  { "getaicount", GScr_GetAICount, 0 },
  { "getailimit", GScr_GetAILimit, 0 },
  { "getnode", Scr_GetNode, 0 },
  { "getnodearray", Scr_GetNodeArray, 0 },
  { "getanynodearray", Scr_GetAnyNodeArray, 0 },
  { "findpath", Scr_FindPath, 0 },
  { "getallnodes", Scr_GetAllNodes, 0 },
  { "setnodepriority", Scr_SetNodePriority, 0 },
  { "getnodeowner", Scr_GetNodeOwner, 0 },
  { "setturretnode", Scr_SetTurretNode, 0 },
  { "unsetturretnode", Scr_UnsetTurretNode, 0 },
  { "getcovernodearray", Scr_GetCoverNodeArray, 0 },
  { "setenablenode", Scr_SetEnableNode, 0 },
  { "linknodes", Scr_LinkNodes, 0 },
  { "unlinknodes", Scr_UnLinkNodes, 0 },
  { "getatrloaded", GScr_GetAnimTreesLoaded, 0 },
  { "getanimtreesloaded", GScr_GetAnimTreesLoaded, 0 },
  { "findanimbyname", GScr_FindAnimByName, 0 },
  { "spawn", GScr_Spawn, 0 },
  { "codespawn", GScr_CodeSpawn, 0 },
  { "spawncollision", GScr_SpawnCollision, 0 },
  { "spawntimedfx", GScr_SpawnTimedFX, 0 },
  { "spawnapalmgroundflame", GScr_SpawnNapalmGroundFlame, 0 },
  { "spawnnapalmgroundflame", GScr_SpawnNapalmGroundFlame, 0 },
  { "spawnvehicle", GScr_SpawnVehicle, 0 },
  { "codespawnvehicle", GScr_CodeSpawnVehicle, 0 },
  { "spawnplane", GScr_SpawnPlane, 0 },
  { "spawnturret", GScr_SpawnTurret, 0 },
  { "precacheturret", GScr_PrecacheTurret, 0 },
  { "spawnstruct", Scr_AddStruct, 0 },
  { "addstruct", Scr_AddStruct, 0 },
  { "spawnhelicopter", GScr_SpawnHelicopter, 0 },
  { "assert", assertCmd_0, 1 },
  { "assertex", assertexCmd_0, 1 },
  { "assertmsg", assertmsgCmd_0, 1 },
  { "adddebugcommand", GScr_AddDebugCommand, 1 },
  { "isdefined", GScr_IsDefined, 0 },
  { "ismp", GScr_IsMP, 0 },
  { "isint", GScr_IsInt, 0 },
  { "isfloat", GScr_IsFloat, 0 },
  { "isvec", GScr_IsVec, 0 },
  { "isstring", GScr_IsString, 0 },
  { "isarray", GScr_IsArray, 0 },
  { "isalive", GScr_IsAlive, 0 },
  { "issentient", GScr_IsSentient, 0 },
  { "isgodmode", GScr_IsGodMode, 0 },
  { "numremoteclients", GScr_NumRemoteClients, 0 },
  { "modelhasphyspreset", GScr_ModelHasPhysPreset, 0 },
  { "getdvar", GScr_GetDvar, 0 },
  { "getdvarint", GScr_GetDvarInt, 0 },
  { "getdvarfloat", GScr_GetDvarFloat, 0 },
  { "getdvarcolorred", GScr_GetDvarColorRed, 0 },
  { "getdvarcolorgreen", GScr_GetDvarColorGreen, 0 },
  { "getdvarcolorblue", GScr_GetDvarColorBlue, 0 },
  { "getdvarcoloralpha", GScr_GetDvarColorAlpha, 0 },
  { "getdebugdvar", GScr_GetDebugDvar, 0 },
  { "getdebugdvarint", GScr_GetDebugDvarInt, 0 },
  { "getdebugdvarfloat", GScr_GetDebugDvarFloat, 0 },
  { "getdifficulty", GScr_GetDifficulty, 0 },
  { "getdate", GScr_GetDate, 0 },
  { "getcommandfromkey", GScr_GetCommandFromKey, 0 },
  { "setdvar", GScr_SetDvar, 0 },
  { "setsaveddvar", GScr_SetSavedDvar, 0 },
  { "watersimenable", GScr_WaterSimEnable, 0 },
  { "waterplop", GScr_WaterPlop, 0 },
  { "gettime", GScr_GetTime, 0 },
  { "getentbynum", Scr_GetEntByNum, 1 },
  { "getweaponmodel", Scr_GetWeaponModel, 0 },
  { "getweaponclipmodel", Scr_GetWeaponClipModel, 0 },
  { "getweaponstowedmodel", Scr_GetWeaponStowedModel, 0 },
  { "getanimlength", GScr_GetAnimLength, 0 },
  { "getanimframecount", GScr_GetAnimFrameCount, 0 },
  { "getanimdumpmodel", GScr_GetAnimDumpModel, 0 },
  { "getanimdumptree", GScr_GetAnimDumpTree, 0 },
  { "animhasnotetrack", GScr_AnimHasNotetrack, 0 },
  { "getnotetracktimes", GScr_GetNotetrackTimes, 0 },
  { "getnotetracksindelta", GScr_GetNotetracksInDelta, 0 },
  { "getbrushmodelcenter", GScr_GetBrushModelCenter, 0 },
  { "getattachmentindex", GScr_GetAttachmentIndex, 0 },
  // LWSS ADD FROM RETAIL BLOPS MP (LATEST FILES FROM STEAM REFERENCE NEWER FUNCTIONS THAT WERE ADDED LATER)
  { "getcorpsearray", GScr_GetCorpseArray, 0 },
  { "getplayers", Scr_GetPlayers, 0 },
  // LWSS END

  { "objective_add", Scr_Objective_Add, 0 },
  { "objective_delete", Scr_Objective_Delete, 0 },
  { "objective_state", Scr_Objective_State, 0 },
  { "objective_icon", Scr_Objective_Icon, 0 },
  { "objective_position", Scr_Objective_Position, 0 },
  { "objective_onentity", Scr_Objective_OnEntity, 0 },
  { "objective_current", Scr_Objective_Current, 0 },
  { "objective_setvisibletoplayer", Scr_Objective_SetVisibleToPlayer, 0 },
  { "objective_setinvisibletoplayer", Scr_Objective_SetInvisibleToPlayer, 0 },
  { "objective_setvisibletoall", Scr_Objective_SetVisibleToAll, 0 },
  { "objective_setinvisibletoall", Scr_Objective_SetInvisibleToAll, 0 },
  { "objective_setsize", Scr_Objective_SetSize, 0 },
  { "objective_setcolor", Scr_Objective_SetColor, 0 },
  { "objective_add_team", Scr_Objective_Add_Team, 0 },
  { "objective_additionalcurrent", Scr_Objective_AdditionalCurrent, 0 },
  { "objective_additionalposition", Scr_Objective_AdditionalPosition, 0 },
  { "objective_ring", Scr_Objective_Ring, 0 },
  { "objective_set3d", Scr_Objective_Set3D, 0 },
  { "objective_size", Scr_Objective_Size, 0 },
  { "objective_string", Scr_Objective_String, 0 },
  { "objective_string_nomessage", Scr_Objective_String_NoMessage, 0 },
  { "missile_createattractorent", Scr_MissileCreateAttractorEnt, 0 },
  { "missile_createattractororigin", Scr_MissileCreateAttractorOrigin, 0 },
  { "missile_createrepulsorent", Scr_MissileCreateRepulsorEnt, 0 },
  { "missile_createrepulsororigin", Scr_MissileCreateRepulsorOrigin, 0 },
  { "missile_deleteattractor", Scr_MissileDeleteAttractor, 0 },
  { "bullettrace", Scr_BulletTrace, 0 },
  { "bullettracepassed", Scr_BulletTracePassed, 0 },
  { "groundtrace", Scr_GroundTrace, 0 },
  { "sighttracepassed", Scr_SightTracePassed, 0 },
  { "physicstrace", Scr_PhysicsTrace, 0 },
  { "playerphysicstrace", Scr_PlayerPhysicsTrace, 0 },
  // LWSS ADD FROM RETAIL BLOPS MP (LATEST FILES FROM STEAM REFERENCE NEWER FUNCTIONS THAT WERE ADDED LATER)
  { "playerbullettrace", Scr_PlayerBulletTrace, 0 },
  // LWSS END
  { "getmovedelta", GScr_GetMoveDelta, 0 },
  { "getangledelta", GScr_GetAngleDelta, 0 },
  { "getstartorigin", GScr_GetStartOrigin, 0 },
  { "getstartangles", GScr_GetStartAngles, 0 },
  { "getnorthyaw", GScr_GetNorthYaw, 0 },
  { "randomint", Scr_RandomInt, 0 },
  { "randomfloat", Scr_RandomFloat, 0 },
  { "randomintrange", Scr_RandomIntRange, 0 },
  { "randomfloatrange", Scr_RandomFloatRange, 0 },
  { "log", GScr_log, 0 },
  { "sin", GScr_sin, 0 },
  { "cos", GScr_cos, 0 },
  { "tan", GScr_tan, 0 },
  { "asin", GScr_asin, 0 },
  { "acos", GScr_acos, 0 },
  { "atan", GScr_atan, 0 },
  { "int", GScr_CastInt, 0 },
  { "castint", GScr_CastInt, 0 },
  { "float", GScr_CastFloat, 0 },
  { "castfloat", GScr_CastFloat, 0 },
  { "flushgump", GScr_flushGump, 0 },
  { "abs", GScr_abs, 0 },
  { "min", GScr_min, 0 },
  { "max", GScr_max, 0 },
  { "floor", GScr_floor, 0 },
  { "ceil", GScr_ceil, 0 },
  { "sqrt", GScr_sqrt, 0 },
  { "lerpfloat", GScr_LerpFloat, 0 },
  { "lerpvector", GScr_LerpVector, 0 },
  { "vectorfromlinetopoint", GScr_VectorFromLineToPoint, 0 },
  { "pointonsegmentnearesttopoint", GScr_PointOnSegmentNearestToPoint, 0 },
  { "distance", Scr_Distance, 0 },
  { "distance2d", Scr_Distance2D, 0 },
  { "distance2dsquared", Scr_Distance2DSquared, 0 },
  { "distancesquared", Scr_DistanceSquared, 0 },
  { "bulletspread", Scr_BulletSpread, 0 },
  { "bullettracer", Scr_BulletTracer, 0 },
  { "length", Scr_Length, 0 },
  { "lengthsquared", Scr_LengthSquared, 0 },
  { "closer", Scr_Closer, 0 },
  { "vectordot", Scr_VectorDot, 0 },
  { "vectorcross", Scr_VectorCross, 0 },
  { "vectornormalize", Scr_VectorNormalize, 0 },
  { "vectortoangles", Scr_VectorToAngles, 0 },
  { "vectorlerp", Scr_VectorLerp, 0 },
  { "anglestoup", Scr_AnglesToUp, 0 },
  { "anglestoright", Scr_AnglesToRight, 0 },
  { "anglestoforward", Scr_AnglesToForward, 0 },
  { "combineangles", Scr_CombineAngles, 0 },
  { "angleclamp180", Scr_ClampAngle180, 0 },
  { "clampangle180", Scr_ClampAngle180, 0 },
  { "absangleclamp180", Scr_AbsAngleClamp180, 0 },
  { "absangleclamp360", Scr_AbsAngleClamp360, 0 },
  { "anglelerp", Scr_AngleLerp, 0 },
  { "allowmipstoload", Scr_AllowMipsToLoad, 0 },
  { "rotatepoint", Scr_RotatePoint, 0 },
  { "getcurve", Scr_GetCurve, 0 },
  { "freecurve", Scr_FreeCurve, 0 },
  { "getcurvecurpos", Scr_GetCurveCurPos, 0 },
  { "getcurveserverpos", Scr_GetCurveServerPos, 0 },
  { "drawcurve", Scr_DrawCurve, 0 },
  { "buildcurve", Scr_BuildCurve, 0 },
  { "startcurve", Scr_StartCurve, 0 },
  { "stopcurve", Scr_StopCurve, 0 },
  { "addnodetocurve", Scr_AddNodeToCurve, 0 },
  { "createrope", Scr_CreateRope, 0 },
  { "deleterope", Scr_DeleteRope, 0 },
  { "getrope", Scr_GetRope, 0 },
  { "setcurvebspline", Scr_SetCurveBSpline, 0 },
  { "setcurvecameraent", Scr_SetCurveCameraEnt, 0 },
  { "setcurvenotifyent", Scr_SetCurveNotifyEnt, 0 },
  { "setcurverounded", Scr_SetCurveRounded, 0 },
  { "setcurvesmooth", Scr_SetCurveSmooth, 0 },
  { "setcurvespeed", Scr_SetCurveSpeed, 0 },
  { "setdebugangles", GScr_SetDebugAngles, 0 },
  { "setdebugorigin", GScr_SetDebugOrigin, 0 },
  { "setgravity", ScrCmd_SetGravity, 0 },
  { "setcellinvisibleatpos", Scr_SetCellInvisibleAtPos, 0 },
  { "setcellvisibleatpos", Scr_SetCellVisibleAtPos, 0 },
  { "issubstr", Scr_IsSubStr, 0 },
  { "getsubstr", Scr_GetSubStr, 0 },
  { "tolower", Scr_ToLower, 0 },
  { "strtok", Scr_StrTok, 0 },
  { "soundfade", Scr_SoundFade, 0 },
  { "playsoundatposition", Scr_PlaySoundAtPosition, 0 },
  { "getvehiclenode", GScr_GetVehicleNode, 0 },
  { "getallvehiclenodes", GScr_GetAllVehicleNodes, 0 },
  { "getvehiclenodearray", GScr_GetVehicleNodeArray, 0 },
  { "setvehiclenodeenabled", GScr_SetVehicleNodeEnabled, 0 },
  { "precachemodel", Scr_PrecacheModel, 0 },
  { "precacheshellshock", Scr_PrecacheShellShock, 0 },
  { "precacheitem", Scr_PrecacheItem, 0 },
  { "precacheshader", Scr_PrecacheShader, 0 },
  { "precachematerial", Scr_PrecacheMaterial, 0 },
  { "precachestring", Scr_PrecacheString, 0 },
  { "precacherumble", Scr_PrecacheRumble, 0 },
  { "precachevehicle", GScr_PrecacheVehicle, 0 },
  { "precachemenu", GScr_PrecacheMenu, 0 },
  { "precachestatusicon", GScr_PrecacheStatusIcon, 0 },
  { "precacheheadicon", GScr_PrecacheHeadIcon, 0 },
  { "precachelocationselector", GScr_PrecacheLocationSelector, 0 },
  { "loadfx", Scr_LoadFX, 0 },
  { "playfx", Scr_PlayFX, 0 },
  { "playfxontag", Scr_PlayFXOnTag, 0 },
  { "getwaterheight", GScr_GetWaterHeight, 0 },
  { "playloopedfx", Scr_PlayLoopedFX, 0 },
  { "spawnfx", Scr_SpawnFX, 0 },
  { "codeplayloopedfx", Scr_CodePlayLoopedFX, 0 },
  { "codespawnfx", Scr_CodeSpawnFX, 0 },
  { "activateclientexploder", Scr_ActivateClientExploder, 0 },
  { "deactivateclientexploder", Scr_DeactivateClientExploder, 0 },
  { "clearallcorpses", Scr_ClearAllCorpses, 0 },
  { "triggerfx", Scr_TriggerFX, 0 },
  { "physicsexplosionsphere", Scr_PhysicsExplosionSphere, 0 },
  { "physicsexplosioncylinder", Scr_PhysicsExplosionCylinder, 0 },
  { "physicsjolt", Scr_PhysicsRadiusJolt, 0 },
  { "physicsradiusjolt", Scr_PhysicsRadiusJolt, 0 },
  { "createstreamerhint", Scr_CreateStreamerHint, 0 },
  { "setexpfog", Scr_SetExponentialFog, 0 },
  { "setexponentialfog", Scr_SetExponentialFog, 0 },
  { "setvolfog", Scr_SetVolumetricFog, 0 },
  { "setculldist", Scr_SetCullDist, 0 },
  { "grenadeexplosioneffect", Scr_GrenadeExplosionEffect, 0 },
  { "magicbullet", GScr_MagicBullet, 0 },
  { "fire", Scr_Fire, 0 },
  { "getfxvisibility", Scr_GetFXVis, 0 },
  { "getfxvis", Scr_GetFXVis, 0 },
  { "ropebreak", Scr_RopeBreak, 0 },
  { "breakrope", Scr_RopeBreak, 0 },
  { "radiusdamage", GScr_RadiusDamage, 0 },
  { "setplayerignoreradiusdamage", GScr_SetPlayerIgnoreRadiusDamage, 0 },
  { "glassradiusdamage", GScr_GlassRadiusDamage, 0 },
  { "getnumparts", GScr_GetNumParts, 0 },
  { "getnumconnectedplayers", Scr_GetNumConnectedPlayers, 0 },
  { "getnumexpectedplayers", Scr_GetNumExpectedPlayers, 0 },
  { "getpartname", GScr_GetPartName, 0 },
  { "earthquake", GScr_Earthquake, 0 },
  { "newhudelem", GScr_NewHudElem, 0 },
  { "newclienthudelem", GScr_NewClientHudElem, 0 },
  { "newteamhudelem", GScr_NewTeamHudElem, 0 },
  { "newscorehudelem", GScr_NewScoreHudElem, 0 },
  { "newdebughudelem", GScr_NewDebugHudElem, 0 },
  { "resettimeout", Scr_ResetTimeout, 0 },
  { "weaponfiretime", GScr_WeaponFireTime, 0 },
  { "weaponreloadtime", GScr_WeaponReloadTime, 0 },
  { "isweaponcliponly", GScr_IsWeaponClipOnly, 0 },
  { "isweapondetonationtimed", GScr_IsWeaponDetonationTimed, 0 },
  { "weaponclipsize", GScr_WeaponClipSize, 0 },
  { "weaponcliptype", GScr_WeaponClipType, 0 },
  { "weapondogibbing", GScr_WeaponDoGibbing, 0 },
  { "weaponfightdist", GScr_WeaponFightDist, 0 },
  { "weaponhasbayonet", GScr_WeaponHasBayonet, 0 },
  { "weaponisdualwield", GScr_WeaponIsDualWield, 0 },
  { "weaponissniperweapon", GScr_WeaponIsSniperWeapon, 0 },
  { "weaponmaxdist", GScr_WeaponMaxDist, 0 },
  { "weaponmaxgibdistance", GScr_WeaponMaxGibDistance, 0 },
  { "weaponissemiauto", GScr_WeaponIsSemiAuto, 0 },
  { "weaponisboltaction", GScr_WeaponIsBoltAction, 0 },
  { "weapontype", GScr_WeaponType, 0 },
  { "weaponclass", GScr_WeaponClass, 0 },
  { "weaponmountable", GScr_WeaponIsMountable, 0 },
  { "weaponismountable", GScr_WeaponIsMountable, 0 },
  { "weaponinventorytype", GScr_WeaponInventoryType, 0 },
  { "weaponstartammo", GScr_WeaponStartAmmo, 0 },
  { "weaponmaxammo", GScr_WeaponMaxAmmo, 0 },
  { "weaponaltweaponname", GScr_WeaponAltWeaponName, 0 },
  { "weapondualwieldweaponname", GScr_WeaponDualWieldWeaponName, 0 },
  { "weaponisgasweapon", GScr_WeaponIsGasWeapon, 0 },
  { "getwatcherweapons", GScr_GetWatcherWeapons, 0 },
  { "getretrievableweapons", GScr_GetRetrievableWeapons, 0 },
  { "getweaponmindamagerange", GScr_GetWeaponMinDamageRange, 0 },
  { "getweaponmaxdamagerange", GScr_GetWeaponMaxDamageRange, 0 },
  { "getweaponmindamage", GScr_GetWeaponMinDamage, 0 },
  { "getweaponmaxdamage", GScr_GetWeaponMaxDamage, 0 },
  { "getweaponfusetime", GScr_GetWeaponFuseTime, 0 },
  { "getweaponprojexplosionsound", GScr_GetWeaponProjExplosionSound, 0 },
  { "isweaponspecificuse", GScr_IsWeaponSpecificUse, 0 },
  { "isweaponscopeoverlay", GScr_IsWeaponScopeOverlay, 0 },
  { "isweaponequipment", GScr_IsWeaponEquipment, 0 },
  { "isweaponprimary", GScr_IsWeaponPrimary, 0 },
  { "isturretactive", GScr_IsTurretActive, 0 },
  { "isturretfiring", GScr_IsTurretFiring, 0 },
  { "getweaponfiresound", GScr_GetWeaponFireSound, 0 },
  { "getweaponfiresoundplayer", GScr_GetWeaponFireSoundPlayer, 0 },
  { "getweaponpickupsoundplayer", GScr_GetWeaponPickupSoundPlayer, 0 },
  { "getweaponpickupsound", GScr_GetWeaponPickupSound, 0 },
  { "getweaponindexfromname", GScr_GetWeaponIndexFromName, 0 },
  { "isplayer", GScr_IsPlayer, 0 },
  { "isplayernumber", GScr_IsPlayerNumber, 0 },
  { "setwinningplayer", GScr_SetWinningPlayer, 0 },
  { "setwinningteam", GScr_SetWinningTeam, 0 },
  { "announcement", GScr_Announcement, 0 },
  { "clientannouncement", GScr_ClientAnnouncement, 0 },
  { "getteamscore", GScr_GetTeamScore, 0 },
  { "setteamscore", GScr_SetTeamScore, 0 },
  { "setclientnamemode", GScr_SetClientNameMode, 0 },
  { "updateclientnames", GScr_UpdateClientNames, 0 },
  { "getteamplayersalive", GScr_GetTeamPlayersAlive, 0 },
  { "getdroppedweapons", GScr_GetDroppedWeapons, 0 },
  { "objective_team", GScr_Objective_Team, 0 },
  { "artilleryiconlocation", SetArtilleryIconLocation, 0 },
  { "logprint", GScr_LogPrint, 0 },
  { "worldentnumber", GScr_WorldEntNumber, 0 },
  { "obituary", GScr_Obituary, 0 },
  { "pause3dcinematic", GScr_Pause3DCinematic, 0 },
  { "reviveobituary", GScr_ReviveObituary, 0 },
  { "adddemobookmark", GScr_AddDemoBookmark, 0 },
  { "positionwouldtelefrag", GScr_positionWouldTelefrag, 0 },
  { "boundswouldtelefrag", GScr_BoundsWouldTelefrag, 0 },
  { "recordusedspawnpoint", GScr_RecordUsedSpawnPoint, 0 },
  { "testspawnpoint", GScr_TestSpawnPoint, 0 },
  { "getstarttime", GScr_getStartTime, 0 },
  { "map_restart", GScr_MapRestart, 0 },
  { "maprestart", GScr_MapRestart, 0 },
  { "fastrestart", GScr_FastRestart, 0 },
  { "exitlevel", GScr_ExitLevel, 0 },
  { "missionSuccess", GScr_MissionSuccess, 0 },
  { "missionFailed", GScr_MissionFailed, 0 },
  { "missionsuccess", GScr_MissionSuccess, 0 },
  { "missionfailed", GScr_MissionFailed, 0 },
  { "changelevel", GScr_ChangeLevel, 0 },
  { "savegame", Scr_SaveGame, 0 },
  { "savegamenocommit", Scr_SaveGameNoCommit, 0 },
  { "prefetchlevel", Scr_PrefetchLevel, 0 },
  { "forcelevelend", GScr_ForceLevelEnd, 0 },
  { "issavesuccessful", Scr_IsSaveSuccessful, 0 },
  { "issaverecentlyloaded", Scr_IsRecentlyLoaded, 0 },
  { "isrecentlyloaded", Scr_IsRecentlyLoaded, 0 },
  { "commitsave", Scr_CommitSave, 0 },
  { "killserver", GScr_KillServer, 0 },
  { "lerpsundirection", Scr_LerpSunDirection, 0 },
  { "loadgump", GScr_loadGump, 0 },
  { "addtestclient", GScr_AddTestClient, 0 },
  { "makedvarserverinfo", GScr_MakeDvarServerInfo, 0 },
  { "setbombtimer", GScr_SetBombTimer, 0 },
  { "setmatchflag", GScr_SetMatchFlag, 0 },
  { "setmatchtalkflag", GScr_SetMatchTalkFlag, 0 },
  { "setarchive", FUNCTION_NULLSUB, 0 },
  { "allclientsprint", GScr_AllClientsPrint, 0 },
  { "clientprint", GScr_ClientPrint, 0 },
  { "mapexists", GScr_MapExists, 0 },
  { "isvalidgametype", GScr_IsValidGameType, 0 },
  { "matchend", FUNCTION_NULLSUB, 0 },
  { "setplayerteamrank", FUNCTION_NULLSUB, 0 },
  { "sendranks", FUNCTION_NULLSUB, 0 },
  { "endparty", FUNCTION_NULLSUB, 0 },
  { "setteamspyplane", GScr_SetTeamSpyplane, 0 },
  { "getteamspyplane", GScr_GetTeamSpyplane, 0 },
  { "setteamsatellite", GScr_SetTeamSatellite, 0 },
  { "getteamsatellite", GScr_GetTeamSatellite, 0 },
  { "getassignedteam", GScr_GetAssignedTeam, 0 },
  { "uploadstats", GScr_UploadStats, 0 },
  { "getdefaultclassslot", GScr_GetDefaultClassSlot, 0 },
  { "getitemattachment", GScr_GetItemAttachment, 0 },
  { "getreffromitemindex", GScr_GetRefFromItemIndex, 0 },
  { "getitemgroupfromitemindex", GScr_GetItemGroupFromItemIndex, 0 },
  { "getbaseweaponitemindex", GScr_GetBaseWeaponItemIndex, 0 },
  { "getgametypeenumfromname", GScr_GetGameTypeEnumFromName, 0 },
  { "getwagergametypelist", GScr_GetWagerGametypeList, 0 },
  { "setscoreboardcolumns", GScr_SetScoreboardColumns, 0 },
  { "iscollectors", GScr_IsCollectors, 0 },
  { "isassetloaded", GScr_IsAssetLoaded, 0 },
  { "iscoopepd", GScr_IsCoopEPD, 0 },
  { "recordplayerstats", GScr_SetPlayerStatsForMatchRecording, 0 },
  { "recordplayermatchend", GScr_SetPlayerFinalForMatchRecording, 0 },
  { "recordmatchbegin", GScr_SetBeginForMatchRecording, 0 },
  { "setvotestring", GScr_SetVoteString, 0 },
  { "setvotetime", GScr_SetVoteTime, 0 },
  { "setvoteyescount", GScr_SetVoteYesCount, 0 },
  { "setvotenocount", GScr_SetVoteNoCount, 0 },
  { "openfile", GScr_OpenFile, 1 },
  { "closefile", GScr_CloseFile, 1 },
  { "fprintln", GScr_FPrintln, 1 },
  { "fprintfields", GScr_FPrintFields, 1 },
  { "freadln", GScr_FReadLn, 1 },
  { "fgetarg", GScr_FGetArg, 1 },
  { "execdevgui", GScr_ExecDevgui, 1 },
  { "reportmtu", FUNCTION_NULLSUB, 0 },
  { "pcserverupdateplaylist", FUNCTION_NULLSUB, 0 },
  { "kick", GScr_KickPlayer, 0 },
  { "ban", GScr_BanPlayer, 0 },
  { "banplayer", GScr_BanPlayer, 0 },
  { "map", GScr_LoadMap, 0 },
  { "makelocalizedstring", Scr_MakeLocalizedString, 0 },
  { "playrumbleonposition", Scr_PlayRumbleOnPosition, 0 },
  { "playrumblelooponposition", Scr_PlayRumbleLoopOnPosition, 0 },
  { "stopallrumbles", Scr_StopAllRumbles, 0 },
  { "updategamerprofile", ScrCmd_UpdateGamerProfile, 0 },
  { "getpersistentprofilevar", GScr_GetPersistentProfileVar, 0 },
  { "setpersistentprofilevar", GScr_SetPersistentProfileVar, 0 },
  { "hascollectible", GScr_HasCollectible, 0 },
  { "setcollectible", GScr_SetCollectible, 0 },
  { "playbink", GScr_PlayBink, 0 },
  { "gettimescale", ScrCmd_GetTimeScale, 0 },
  { "soundexists", ScrCmd_SoundExists, 0 },
  { "issplitscreen", GScr_IsSplitscreen, 0 },
  { "isglobalstatsserver", Scr_IsGlobalStatsServer, 0 },
  { "setminimap", GScr_SetMiniMap, 0 },
  { "setmapcenter", GScr_SetMapCenter, 0 },
  { "isdemorecording", GScr_IsDemoRecording, 0 },
  { "isdemoenabled", isDemoEnabled, 0 },
  { "setdemointermissionpoint", GScr_SetDemoIntermissionPoint, 0 },
  { "startdemorecording", GScr_StartDemoRecording, 0 },
  { "stopdemorecording", GScr_StopDemoRecording, 0 },
  { "setgameendtime", GScr_SetGameEndTime, 0 },
  { "settimescale", GScr_SetTimeScale, 0 },
  { "incrementescrow", GScr_IncrementEscrow, 0 },
  { "getarraykeys", GScr_GetArrayKeys, 0 },
  { "searchforonlinegames", FUNCTION_NULLSUB, 0 },
  { "quitlobby", FUNCTION_NULLSUB, 0 },
  { "quitparty", FUNCTION_NULLSUB, 0 },
  { "startparty", FUNCTION_NULLSUB, 0 },
  { "startprivatematch", FUNCTION_NULLSUB, 0 },
  { "visionsetnaked", Scr_VisionSetNaked, 0 },
  { "visionsetnight", Scr_VisionSetNight, 0 },
  { "visionsetlaststand", GScr_VisionSetLastStand, 0 },
  { "player_setoffscreenmaterial", GScr_Player_SetOffscreenMaterial, 0 },
  { "player_setoffscreendownmaterial", GScr_Player_SetOffscreenDownMaterial, 0 },
  { "tablelookup", Scr_TableLookup, 0 },
  { "tablelookupistring", Scr_TableLookupIString, 0 },
  { "tablelookuprownum", Scr_TableLookupRowNum, 0 },
  { "tablelookupcolumnforrow", Scr_TableLookupColumnForRow, 0 },
  { "endlobby", FUNCTION_NULLSUB, 0 },
  { "clientsysregister", GScr_ClientSysRegister, 0 },
  { "clientsyssetstate", GScr_ClientSysSetState, 0 },
  { "isai", GScr_IsAI, 0 },
  { "getaitriggerflags", GScr_GetAITriggerFlags, 0 },
  { "isvehicle", GScr_IsVehicle, 0 },
  { "getmaxvehicles", GScr_GetMaxVehicles, 0 },
  { "getnumvehicles", GScr_GetNumVehicles, 0 },
  { "setheliheightpatchenabled", GScr_SetHeliHeightPatchEnabled, 0 },
  { "getvehicletreadfxarray", GScr_GetVehicleTreadFXArray, 0 },
  { "disabledestructiblepieces", GScr_DisableDestructiblePieces, 0 },
  { "destructiblehasnotify", GScr_DestructibleHasNotify, 0 },
  { "enablealldestructiblepieces", GScr_EnableAllDestructiblePieces, 0 },
  { "drawcompassfriendlies", GScr_DrawCompassFriendlies, 0 },
  { "canspawnturret", GScr_CanSpawnTurret, 0 },
  { "codespawnturret", GScr_CodeSpawnTurret, 0 },
  { "cleanupspawneddynents", GScr_CleanupSpawnedDynEnts, 0 },
  { "clearlocalizedstrings", GScr_ClearLocalizedStrings, 0 },
  { "cruncharray", GScr_CrunchArray, 0 },
  { "createdynentandlaunch", GScr_CreateDynEntAndLaunch, 0 },
  { "getvehicletriggerflags", GScr_GetMaxVehicles, 0 },
  { "collisiontestpointsinsphere", GScr_CollisionTestPointsInSphere, 0 },
  { "collisiontestpointsincylinder", GScr_CollisionTestPointsInCylinder, 0 },
  { "collisiontestpointsinpill", GScr_CollisionTestPointsInPill, 0 },
  { "collisiontestpointsincone", GScr_CollisionTestPointsInCone, 0 },
  { "collisiontestpointsinbox", GScr_CollisionTestPointsInBox, 0 },
  {
    "qsortscoredspawnpointsascending",
    GScr_QSortScoredSpawnPointArrayAscending,
    0
  },
  { "matrix4x4transformpoints", GScr_Matrix4x4TransformPoints, 0 },
  { "setspawnpointrandomvariation", GScr_SetSpawnPointRandomVariation, 0 },
  { "clearspawnpoints", GScr_ClearSpawnPoints, 0 },
  { "addspawnpoints", GScr_AddSpawnPoints, 0 },
  { "getsortedspawnpoints", GScr_GetSortedSpawnPoints, 0 },
  { "clearspawnpointsbaseweight", GScr_ClearSpawnPointsBaseWeight, 0 },
  { "setspawnpointsbaseweight", GScr_SetSpawnPointsBaseWeight, 0 },
  { "getplayerspawnid", FUNCTION_NULLSUB, 0 },
  { "isspawnpointvisible", GScr_IsSpawnPointVisible, 0 },
  { "addsphereinfluencer", GScr_AddSphereInfluencer, 0 },
  { "addcylinderinfluencer", GScr_AddCylinderInfluencer, 0 },
  { "removeinfluencer", GScr_RemoveInfluencer, 0 },
  { "enableinfluencer", GScr_EnableInfluencer, 0 },
  { "setinfluencerteammask", GScr_SetInfluencerTeamMask, 0 },
  { "setdebugsideswitch", GScr_SetDebugSideSwitch, 1 },
  { "target_set", Scr_Target_Set, 0 },
  { "target_remove", Scr_Target_Remove, 0 },
  { "target_setshader", Scr_Target_SetShader, 0 },
  { "target_setoffscreenshader", Scr_Target_SetOffscreenShader, 0 },
  { "target_isinrect", Scr_Target_IsInRect, 0 },
  { "target_isincircle", Scr_Target_IsInCircle, 0 },
  { "target_startreticlelockon", Scr_Target_StartLockOn, 0 },
  { "target_clearreticlelockon", Scr_Target_ClearLockOn, 0 },
  { "target_getarray", Scr_Target_GetArray, 0 },
  { "target_istarget", Scr_Target_IsTarget, 0 },
  { "target_setattackmode", Scr_Target_SetAttackMode, 0 },
  { "target_setjavelinonly", Scr_Target_SetJavelinOnly, 0 },
  { "target_setturretaquire", Scr_Target_SetTurretAquire, 0 },
  { "getnumgvrules", GScr_GetNumGVRules, 0 },
  { "getgvrule", GScr_GetGVRule, 0 },
  { "getmaxactivecontracts", GScr_GetMaxActiveContracts, 0 },
  { "getcontractstattype", GScr_GetContractStatType, 0 },
  { "getcontractstatname", GScr_GetContractStatName, 0 },
  { "getcontractrewardxp", GScr_GetContractRewardXP, 0 },
  { "getcontractrewardcp", GScr_GetContractRewardCP, 0 },
  { "getcontractrequirements", GScr_GetContractRequirements, 0 },
  { "getcontractname", GScr_GetContractName, 0 },
  { "getcontractrequiredcount", GScr_GetContractRequiredCount, 0 },
  { "getcontractresetconditions", GScr_GetContractResetConditions, 0 },
  { "getfogsettings", GScr_GetFogSettings, 0 },
  { "getcustomclassloadoutitem", Gscr_GetCustomClassLoadoutItem, 0 },
  { "getcustomclassmodifier", Gscr_GetCustomClassLoadoutModifier, 0 },
  { "pixbeginevent", GScr_PixBeginEvent, 0 },
  { "pixendevent", GScr_PixEndEvent, 0 },
  { "pixmarker", GScr_PixMarker, 0 },
  { "changeadvertisedstatus", FUNCTION_NULLSUB, 0 },
  { "setqosgamedatapayload", FUNCTION_NULLSUB, 0 },
  { "resetqosgamedatapayload", FUNCTION_NULLSUB, 0 },
  { "incrementcounter", GScr_IncrementCounter, 0 },
  { "getcountertotal", GScr_GetCounterTotal, 0 },
  { "enableoccluder", GScr_EnableOccluder, 0 },
  { "ispregameenabled", GScr_IsPregameEnabled, 0 },
  { "ispregamegamestarted", GScr_IsPregameGameStarted, 0 },
  { "pregamestartgame", GScr_PregameStartGame, 0 },
  { "resetpregamedata", GScr_ResetPregameData, 0 },
  { "sethostmigrationstatus", FUNCTION_NULLSUB, 0 },
#ifdef KISAK_SP
  { "playerpositionvalid", Scr_PlayerPositionValid, 0 },
  { "preventmipsfromloading", Scr_PreventMipsFromLoading, 0 },
  { "refreshhudammocounter", Scr_RefreshHudAmmoCounter, 0 },
  { "refreshhudcompass", Scr_RefreshHudCompass, 0 },
  { "reportclientdisconnected", GScr_ReportClientDisconnected, 0 },
  { "resetailimit", GScr_ResetAILimit, 0 },
  { "resetcurve", Scr_ResetCurve, 0 },
  { "resetsundirection", Scr_ResetSunDirection, 0 },
  { "resetsunlight", Scr_ResetSunLight, 0 },
  { "setsunlight", Scr_SetSunLight, 0 },
  { "ropeaddentityanchor", Scr_RopeAddEntityAnchor, 0 },
  { "ropeaddworldanchor", Scr_RopeAddWorldAnchor, 0 },
  { "ropecollide", Scr_RopeCollide, 0 },
  { "ropegetposition", Scr_RopeGetPosition, 0 },
  { "ropemoveanchor", Scr_RopeMoveAnchor, 0 },
  { "roperemoveanchor", Scr_RopeRemoveAnchor, 0 },
  { "ropesetflag", Scr_RopeSetFlag, 0 },
  { "ropesetparam", Scr_RopeSetParam, 0 },
  { "setanimdumpuseserveranims", GScr_SetAnimDumpUseServerAnims, 0 },
  { "setmissiondvar", GScr_SetMissionDvar, 0 },
  { "setnorthyaw", GScr_SetNorthYaw, 0 },
  { "setphysicsgravitydir", ScrCmd_SetPhysicsGravityDir, 0 },
  { "setsundirection", Scr_SetSunDirection, 0 },
  { "snapshotacknowledged", Scr_SnapshotAcknowledged, 0 },
  { "splitviewallowed", GScr_SplitViewAllowed, 0 },
  { "start3dcinematic", GScr_Start3DCinematic, 0 },
  { "stop3dcinematic", GScr_Stop3DCinematic, 0 },
  { "startmultiplayergame", GScr_startMultiplayerGame, 0 },
  { "timedradiusdamage", Scr_TimedRadiusDamage, 0 },
  { "zerogravityvolumeon", GScr_ZeroGravityVolumeOn, 0 },
  { "zerogravityvolumeoff", GScr_ZeroGravityVolumeOff, 0 },
  { "setuinextlevel", GScr_SetUINextLevel, 0 },
  { "setwaterbrush", GScr_SetWaterBrush, 0 },
  { "setweapon", Scr_SetWeapon, 0 },
#endif
  // LWSS ADD FROM RETAIL BLOPS MP (LATEST FILES FROM STEAM REFERENCE NEWER FUNCTIONS THAT WERE ADDED LATER)
  { "starthostmigration", FUNCTION_NULLSUB, 0 }, // (Stubbing these out because who cares)
  { "reportfilm", GScr_ReportFilm, 0 },
  { "record3dtext", GScr_Record3DText, 0 },
  { "recordenttext", GScr_RecordEntText, 0 },
  { "recordline", GScr_RecordLine, 0 },
  { "recordent", GScr_RecordEnt, 0 },
  // LWSS END
};



void (__cdecl *__cdecl Scr_GetFunction(const char **pName, int *type))()
{
    unsigned int i;

    for (i = 0; i < ARRAY_COUNT(functions); ++i)
    {
        if (!strcmp(*pName, functions[i].actionString))
        {
            *pName = functions[i].actionString;
            *type = functions[i].type;
            return functions[i].actionFunc;
        }
    }
    return 0;
}

// LWSS ADD
void __cdecl GScr_GetClientFlag(scr_entref_t entref)
{
    const char *tempValue1;
    gentity_s *pSelf;
    int flag;

    pSelf = GetEntity(entref);
    flag = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);

    if ((unsigned int)flag < 0x10)
    {
        if (pSelf->client)
            Scr_AddInt((pSelf->client->ps.eFlags2 >> flag) & 1, SCRIPTINSTANCE_SERVER);
        else
            Scr_AddInt((pSelf->s.lerp.eFlags2 >> flag) & 1, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_ParamError(0, va("GetClientFlag: Index %i out of range (0 - %i)\n", flag, 15), SCRIPTINSTANCE_SERVER);
    }
}
// LWSS END

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetClientFlag)
void __cdecl GScr_SetClientFlag(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;
    int flag;

    pSelf = GetEntity(entref);
    flag = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    // Decomp: CoDSP_rdBlackOps.map.c — flag >= 0 && flag < 16
    if ( flag >= 0 && flag < 16 )
    {
        if ( pSelf->client )
            pSelf->client->ps.eFlags2 |= 1 << flag;
        else
            pSelf->s.lerp.eFlags2 |= 1 << flag;
    }
    else
    {
        fmtMsg = va("SetClientFlag: Index %i out of range (0 - %i)\n", flag, 15);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ClearClientFlag)
void __cdecl GScr_ClearClientFlag(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;
    int flag;

    pSelf = GetEntity(entref);
    flag = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    // Decomp: CoDSP_rdBlackOps.map.c — flag >= 0 && flag < 16
    if ( flag >= 0 && flag < 16 )
    {
        if ( pSelf->client )
            pSelf->client->ps.eFlags2 &= ~(1 << flag);
        else
            pSelf->s.lerp.eFlags2 &= ~(1 << flag);
    }
    else
    {
        fmtMsg = va("ClearClientFlag: Index %i out of range (0 - %i)\n", flag, 15);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsMissileInsideHeightLock)
void __cdecl GScr_IsMissileInsideHeightLock(scr_entref_t entref)
{
    float meshMaxs[3];
    float meshMins[3];
    int i;
    gentity_s *pSelf;

    pSelf = GetEntity(entref);
    if (!pSelf
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 18507, 0, "%s", "pSelf"))
    {
        __debugbreak();
    }
    for (i = 0; i < num_heli_height_lock_patches; ++i)
    {
        CM_ModelBounds(heli_height_lock_patches[i].brushmodel, meshMins, meshMaxs);
        meshMins[0] = meshMins[0] + heli_height_lock_patches[i].origin[0];
        meshMins[1] = meshMins[1] + heli_height_lock_patches[i].origin[1];
        meshMins[2] = meshMins[2] + heli_height_lock_patches[i].origin[2];
        meshMaxs[0] = meshMaxs[0] + heli_height_lock_patches[i].origin[0];
        meshMaxs[1] = meshMaxs[1] + heli_height_lock_patches[i].origin[1];
        meshMaxs[2] = meshMaxs[2] + heli_height_lock_patches[i].origin[2];
        if (pSelf->r.currentOrigin[0] >= meshMins[0]
            && meshMaxs[0] >= pSelf->r.currentOrigin[0]
            && pSelf->r.currentOrigin[1] >= meshMins[1]
            && meshMaxs[1] >= pSelf->r.currentOrigin[1])
        {
            Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
            return;
        }
    }
    Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
}

// LWSS ADD
void GScr_GetGroundEnt(scr_entref_t entref)
{
    gentity_s *pSelf;
    int groundEntityNum;

    pSelf = GetEntity(entref);

    if (pSelf->client)
        groundEntityNum = pSelf->client->ps.groundEntityNum;
    else
        groundEntityNum = pSelf->s.groundEntityNum;

    if (groundEntityNum < 1023)
        Scr_AddEntity(&level.gentities[groundEntityNum], SCRIPTINSTANCE_SERVER);
}
// LWSS END

// Decomp: CoDSP_rdBlackOps.map.c (GScr_IsOnGround)
void __cdecl GScr_IsOnGround(scr_entref_t entref)
{
    gentity_s *pSelf;
    int worldOnly;

    pSelf = GetEntity(entref);
    if ( !pSelf
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp", 18532, 0, "%s", "pSelf") )
    {
        __debugbreak();
    }
    worldOnly = 0;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        worldOnly = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( pSelf->client )
    {
        if ( (pSelf->client->ps.eFlags & 0x300) != 0 )
            Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
        else if ( (pSelf->client->ps.eFlags & 0x4000) != 0 )
            Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
        else if ( pSelf->client->ps.groundEntityNum == 1022 )
            Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
        else if ( worldOnly || pSelf->client->ps.groundEntityNum == 1023 )
            Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
        else
            Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
    else if ( pSelf->s.groundEntityNum == 1022 )
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    else if ( worldOnly || pSelf->s.groundEntityNum == 1023 )
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c:9602
XAnimTree_s *__cdecl GScr_GetEntAnimTree(gentity_s *ent)
{
    const char *EntityTypeName;
    const char *fmtMsg;
    char *slStr;
    XAnimTree_s *tree;

    tree = G_GetEntAnimTree(ent);
    if ( !tree )
    {
        slStr = SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER);
        EntityTypeName = G_GetEntityTypeName(ent);
        fmtMsg = va(
                     "entity of type '%s', classname '%s', origin (%f, %f, %f) does not have an animation tree",
                     EntityTypeName,
                     slStr,
                     ent->r.currentOrigin[0],
                     ent->r.currentOrigin[1],
                     ent->r.currentOrigin[2]);
        Scr_Error(fmtMsg, 0);
    }
    return tree;
}

// Decomp: CoDSP_rdBlackOps.map.c (G_FlagAnimForUpdate)
void __cdecl G_FlagAnimForUpdate(gentity_s *ent)
{
    if ( (ent->flags & 0x2000) == 0 )
        ent->flags |= 0x40000u;
}

// Decomp: BlackOps.singleplayer.c sub_8027D0 (SP server SetAnimTime; MP is client-only CScr_SetAnimTime)
void __cdecl GScr_SetAnimTime(scr_entref_t entref)
{
    gentity_s *ent;
    XAnimTree_s *tree;
    unsigned int numParams;
    float time;
    scr_anim_s anim;
    XAnim_s *anims;
    unsigned __int16 cmdIndex;

    ent = GetEntity(entref);
    tree = GScr_GetEntAnimTree(ent);
    time = 0.0f;
    numParams = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParams < 1 || numParams > 2 )
        Scr_Error("too many parameters", 0);
    if ( numParams >= 2 )
    {
        time = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
        if ( time >= 0.0f )
        {
            if ( time > 1.0f )
            {
                time = 1.0f;
                Scr_ParamError(1u, "must be < 1", SCRIPTINSTANCE_SERVER);
            }
        }
        else
        {
            time = 0.0f;
            Scr_ParamError(1u, "must be > 0", SCRIPTINSTANCE_SERVER);
        }
    }
    anim = Scr_GetAnim(0, tree, SCRIPTINSTANCE_SERVER);
    anims = XAnimGetAnims(tree);
    if ( !XAnimHasTime(anims, anim.index) )
        Scr_ParamError(0, "not a timed animation", SCRIPTINSTANCE_SERVER);
    if ( time == 1.0f && XAnimIsLooped(anims, anim.index) )
        Scr_ParamError(1u, "cannot set time 1 on looping animation", SCRIPTINSTANCE_SERVER);
    cmdIndex = 0;
    XAnimSetTime(tree, anim.index, time, cmdIndex);
    G_FlagAnimForUpdate(ent);
}

// Decomp: cg_scr_main.cpp CScr_GetAnimTime (SP server GScr version)
void __cdecl GScr_GetAnimTime(scr_entref_t entref)
{
    gentity_s *ent;
    XAnimTree_s *tree;
    scr_anim_s anim;
    XAnim_s *anims;
    float value;

    ent = GetEntity(entref);
    tree = GScr_GetEntAnimTree(ent);
    anim = Scr_GetAnim(0, tree, SCRIPTINSTANCE_SERVER);
    anims = XAnimGetAnims(tree);
    if ( !XAnimHasTime(anims, anim.index) )
        Scr_ParamError(0, "blended nonsynchronized animation has no concept of time", SCRIPTINSTANCE_SERVER);
    value = (float)XAnimGetTime(tree, anim.index);
    Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
}

// Decomp: BlackOps.singleplayer.c sub_802950 (SetAnimRateComplete helper)
static void __cdecl GScr_SetAnimRateRecursive(XAnimTree_s *tree, float rate, unsigned int infoIndex)
{
    XAnim_s *anims;
    XAnimInfo *info;
    unsigned int nextInfoIndex;
    unsigned int childInfoIndex;

    anims = XAnimGetAnims(tree);
    info = GetAnimInfo(infoIndex);
    nextInfoIndex = info->next;
    if ( nextInfoIndex )
        GScr_SetAnimRateRecursive(tree, rate, nextInfoIndex);
    childInfoIndex = info->children;
    if ( childInfoIndex )
        GScr_SetAnimRateRecursive(tree, rate, childInfoIndex);
    if ( XAnimIsPrimitive(anims, info->animIndex) )
        XAnimSetAnimRate(tree, info->animIndex, rate);
}

// Decomp: BlackOps.singleplayer.c sub_802A00 (SP server SetAnimRateComplete)
void __cdecl GScr_SetAnimRateComplete(scr_entref_t entref)
{
    gentity_s *ent;
    XAnimTree_s *tree;
    float rate;

    ent = GetEntity(entref);
    tree = GScr_GetEntAnimTree(ent);
    if ( tree && tree->children )
    {
        if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
            Scr_Error("SetAnimRateComplete: Wrong param count, should be 1", 0);
        rate = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
        if ( rate < 0.0f )
            Scr_ParamError(0, "SetAnimRateComplete: must set nonnegative animRate", SCRIPTINSTANCE_SERVER);
        Scr_SetAnimRateForLeaf_r(ent, tree, rate, tree->children);
        G_FlagAnimForUpdate(ent);
    }
}

// Decomp: BlackOps.singleplayer.c sub_802BA0 (SP server SetAnimRate)
void __cdecl GScr_SetAnimRate(scr_entref_t entref)
{
    gentity_s *ent;
    XAnimTree_s *tree;
    unsigned int numParams;
    bool hasExplicitRate;
    scr_anim_s anim;
    const char *paramName;
    float explicitRate;
    float rate;

    ent = GetEntity(entref);
    tree = GScr_GetEntAnimTree(ent);
    hasExplicitRate = false;
    explicitRate = 0.0f;
    numParams = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParams - 2 )
    {
        if ( numParams - 2 != 1 )
            Scr_Error("wrong number of many parameters", 0);
        hasExplicitRate = true;
        explicitRate = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    }
    paramName = SL_ConvertToString(Scr_GetConstString(1u, SCRIPTINSTANCE_SERVER), SCRIPTINSTANCE_SERVER);
    anim = Scr_GetAnim(0, tree, SCRIPTINSTANCE_SERVER);
    rate = 0.0f;
    if ( XAnimGetParamValue(tree, anim.index, paramName, &rate) )
        Scr_AddFloat(rate, SCRIPTINSTANCE_SERVER);
    else if ( hasExplicitRate )
        Scr_AddFloat(explicitRate, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddFloat(0.0f, SCRIPTINSTANCE_SERVER);
}

// Decomp: BlackOps.singleplayer.c sub_800660 (SP server ClearAnim)
void __cdecl GScr_ClearAnim(scr_entref_t entref)
{
    gentity_s *ent;
    XAnimTree_s *tree;
    scr_anim_s anim;
    float blendtime;
    DObj *obj;
    int cmdIndex;

    ent = GetEntity(entref);
    tree = GScr_GetEntAnimTree(ent);
    anim = Scr_GetAnim(0, tree, SCRIPTINSTANCE_SERVER);
    blendtime = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    obj = Com_GetServerDObj(ent->s.number);
    if ( !obj )
        Scr_ObjectError("No model exists.", SCRIPTINSTANCE_SERVER);
    cmdIndex = 0;
    XAnimSetGoalWeight(obj, anim.index, 0.0f, blendtime, 1.0f, 0, 1, 0, cmdIndex);
    XAnimClearTreeGoalWeights(tree, anim.index, blendtime, cmdIndex);
    G_FlagAnimForUpdate(ent);
}

// Decomp: BlackOps.singleplayer.c sub_800770 (SP server ClearAnimLimited)
void __cdecl GScr_ClearAnimLimited(scr_entref_t entref)
{
    gentity_s *ent;
    XAnimTree_s *tree;
    scr_anim_s anim;
    float blendtime;
    DObj *obj;
    int cmdIndex;

    ent = GetEntity(entref);
    tree = GScr_GetEntAnimTree(ent);
    anim = Scr_GetAnim(0, tree, SCRIPTINSTANCE_SERVER);
    blendtime = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
    obj = Com_GetServerDObj(ent->s.number);
    if ( !obj )
        Scr_ObjectError("No model exists.", SCRIPTINSTANCE_SERVER);
    cmdIndex = 0;
    XAnimSetGoalWeight(obj, anim.index, 0.0f, blendtime, 1.0f, 0, 0, 0, cmdIndex);
    XAnimClearGoalWeight(tree, anim.index, blendtime, (unsigned __int16)cmdIndex);
    G_FlagAnimForUpdate(ent);
}

// Decomp: CoDSP_rdBlackOps.map.c:9629
void __cdecl GScr_SetAnim(scr_entref_t entref)
{
    PROF_SCOPED("SetAnim");
    GScr_SetAnimInternal(entref, 1, false);
}

// Decomp: CoDSP_rdBlackOps.map.c:9630 / BlackOps.singleplayer.c sub_802700 family
void __cdecl GScr_SetAnimInternal(scr_entref_t entref, char flags, bool bFlagged)
{
    unsigned int notifyFlags;
    unsigned int notifyName;
    unsigned int numParams;
    unsigned int animParam;
    float rate;
    XAnimTree_s *tree;
    DObj *obj;
    int cmdIndex;
    float goalWeight;
    float goalTime;
    int error;
    scr_anim_s anim;
    gentity_s *ent;

    ent = GetEntity(entref);
    tree = GScr_GetEntAnimTree(ent);
    rate = 1.0f;
    goalTime = 0.2f;
    goalWeight = 1.0f;
    notifyName = 0;
    numParams = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    animParam = 0;
    if ( bFlagged )
    {
        if ( !numParams )
            Scr_Error("Not enough parameters.", 0);
        notifyName = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
        animParam = 1;
        if ( numParams <= animParam )
            Scr_Error("Not enough parameters.", 0);
        anim = Scr_GetAnim(animParam, tree, SCRIPTINSTANCE_SERVER);
        ++animParam;
        if ( animParam < numParams )
        {
            goalWeight = Scr_GetFloat(animParam, SCRIPTINSTANCE_SERVER);
            if ( goalWeight < 0.0 )
                Scr_ParamError(animParam, "must set nonnegative weight", SCRIPTINSTANCE_SERVER);
            ++animParam;
        }
        if ( animParam < numParams )
        {
            goalTime = Scr_GetFloat(animParam, SCRIPTINSTANCE_SERVER);
            if ( goalTime < 0.0 )
                Scr_ParamError(animParam, "must set nonnegative goal time", SCRIPTINSTANCE_SERVER);
            ++animParam;
        }
        if ( animParam < numParams )
        {
            rate = Scr_GetFloat(animParam, SCRIPTINSTANCE_SERVER);
            if ( rate < 0.0 )
                Scr_ParamError(animParam, "must set nonnegative rate", SCRIPTINSTANCE_SERVER);
            ++animParam;
        }
        if ( animParam < numParams )
            Scr_Error("too many parameters", 0);
    }
    else
    {
        if ( numParams )
        {
            anim = Scr_GetAnim(0, tree, SCRIPTINSTANCE_SERVER);
            XAnimGetParamValue(tree, anim.index, "rate", &rate);
            XAnimGetParamValue(tree, anim.index, "goaltime", &goalTime);
            XAnimGetParamValue(tree, anim.index, "goalweight", &goalWeight);
        }
        if ( numParams > 4 )
            Scr_Error("too many parameters", 0);
        if ( !numParams )
            Scr_Error("too many parameters", 0);
        if ( numParams >= 4 )
        {
            rate = GScr_GetOptionalFloat(3u, rate);
            if ( rate < 0.0 )
                Scr_ParamError(3u, "must set nonnegative rate", SCRIPTINSTANCE_SERVER);
        }
        if ( numParams >= 3 )
        {
            goalTime = GScr_GetOptionalFloat(2u, goalTime);
            if ( goalTime < 0.0 )
                Scr_ParamError(2u, "must set nonnegative goal time", SCRIPTINSTANCE_SERVER);
        }
        if ( numParams >= 2 )
        {
            goalWeight = GScr_GetOptionalFloat(1u, goalWeight);
            if ( goalWeight < 0.0 || goalWeight > 1.0 )
                Scr_ParamError(1u, "must set nonnegative weight", SCRIPTINSTANCE_SERVER);
        }
        anim = Scr_GetAnim(0, tree, SCRIPTINSTANCE_SERVER);
    }
    obj = Com_GetServerDObj(ent->s.number);
    if ( !obj )
        Scr_ObjectError("No model exists.", SCRIPTINSTANCE_SERVER);
    cmdIndex = 0;
    if ( (flags & 1) != 0 )
    {
        if ( goalWeight <= 0.001 )
            notifyFlags = 0;
        else
            notifyFlags = 2;
        error = XAnimSetCompleteGoalWeight(
                            obj,
                            anim.index,
                            goalWeight,
                            goalTime,
                            rate,
                            notifyName,
                            notifyFlags,
                            (flags & 2) != 0,
                            cmdIndex);
    }
    else
    {
        if ( goalWeight <= 0.001 )
            notifyFlags = 0;
        else
            notifyFlags = 2;
        error = XAnimSetGoalWeight(
                            obj,
                            anim.index,
                            goalWeight,
                            goalTime,
                            rate,
                            notifyName,
                            notifyFlags,
                            (flags & 2) != 0,
                            cmdIndex);
    }
    if ( error )
        GScr_HandleAnimError(error);
    else
        G_FlagAnimForUpdate(ent);
}

void __cdecl GScr_SetAnimLimited(scr_entref_t entref)
{
    GScr_SetAnimInternal(entref, 0, false);
}

void __cdecl GScr_SetAnimRestart(scr_entref_t entref)
{
    GScr_SetAnimInternal(entref, 3, false);
}

void __cdecl GScr_SetAnimLimitedRestart(scr_entref_t entref)
{
    GScr_SetAnimInternal(entref, 2, false);
}

void __cdecl GScr_SetFlaggedAnim(scr_entref_t entref)
{
    GScr_SetAnimInternal(entref, 1, true);
}

void __cdecl GScr_SetFlaggedAnimLimited(scr_entref_t entref)
{
    GScr_SetAnimInternal(entref, 0, true);
}

void __cdecl GScr_SetFlaggedAnimRestart(scr_entref_t entref)
{
    GScr_SetAnimInternal(entref, 3, true);
}

void __cdecl GScr_SetFlaggedAnimLimitedRestart(scr_entref_t entref)
{
    GScr_SetAnimInternal(entref, 2, true);
}

// Decomp: BlackOps.singleplayer.c sub_800880 / sub_801870
void __cdecl GScr_SetAnimKnobInternal(scr_entref_t entref, char flags, bool bFlagged)
{
    unsigned int notifyFlags;
    unsigned int numParams;
    float rate;
    XAnimTree_s *tree;
    DObj *obj;
    int cmdIndex;
    float goalWeight;
    float goalTime;
    int error;
    scr_anim_s anim;
    gentity_s *ent;
    VariableUnion notifyName;
    XAnim_s *anims;

    ent = GetEntity(entref);
    tree = GScr_GetEntAnimTree(ent);
    rate = 1.0f;
    goalTime = 0.2f;
    goalWeight = 1.0f;
    notifyName.intValue = 0;
    numParams = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( bFlagged )
    {
        if ( numParams >= 2 )
        {
            notifyName.intValue = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
            anim = Scr_GetAnim(1u, tree, SCRIPTINSTANCE_SERVER);
            XAnimGetParamValue(tree, anim.index, "rate", &rate);
            XAnimGetParamValue(tree, anim.index, "goaltime", &goalTime);
            XAnimGetParamValue(tree, anim.index, "goalweight", &goalWeight);
        }
        if ( numParams > 5 )
            Scr_Error("too many parameters", 0);
        if ( numParams < 2 )
            Scr_Error("too many parameters", 0);
        if ( numParams >= 5 )
        {
            rate = GScr_GetOptionalFloat(4u, rate);
            if ( rate < 0.0 )
                Scr_ParamError(4u, "must set nonnegative rate", SCRIPTINSTANCE_SERVER);
        }
        if ( numParams >= 4 )
        {
            goalTime = GScr_GetOptionalFloat(3u, goalTime);
            if ( goalTime < 0.0 )
                Scr_ParamError(3u, "must set nonnegative goal time", SCRIPTINSTANCE_SERVER);
        }
        if ( numParams >= 3 )
        {
            goalWeight = GScr_GetOptionalFloat(2u, goalWeight);
            if ( goalWeight <= 0.0 )
                Scr_ParamError(2u, "must set positive weight", SCRIPTINSTANCE_SERVER);
        }
        anim = Scr_GetAnim(1u, tree, SCRIPTINSTANCE_SERVER);
        anims = XAnimGetAnims(tree);
        if ( !XAnimHasTime(anims, anim.index) )
            Scr_ParamError(1u, "blended nonsynchronized animation has no concept of time", SCRIPTINSTANCE_SERVER);
    }
    else
    {
        if ( numParams )
        {
            anim = Scr_GetAnim(0, tree, SCRIPTINSTANCE_SERVER);
            XAnimGetParamValue(tree, anim.index, "rate", &rate);
            XAnimGetParamValue(tree, anim.index, "goaltime", &goalTime);
            XAnimGetParamValue(tree, anim.index, "goalweight", &goalWeight);
        }
        if ( numParams > 4 )
            Scr_Error("too many parameters", 0);
        if ( !numParams )
            Scr_Error("too many parameters", 0);
        if ( numParams >= 4 )
        {
            rate = GScr_GetOptionalFloat(3u, rate);
            if ( rate < 0.0 )
                Scr_ParamError(3u, "must set nonnegative rate", SCRIPTINSTANCE_SERVER);
        }
        if ( numParams >= 3 )
        {
            goalTime = GScr_GetOptionalFloat(2u, goalTime);
            if ( goalTime < 0.0 )
                Scr_ParamError(2u, "must set nonnegative goal time", SCRIPTINSTANCE_SERVER);
        }
        if ( numParams >= 2 )
        {
            goalWeight = GScr_GetOptionalFloat(1u, goalWeight);
            if ( goalWeight < 0.0 )
                Scr_ParamError(1u, "must set nonnegative weight", SCRIPTINSTANCE_SERVER);
        }
        anim = Scr_GetAnim(0, tree, SCRIPTINSTANCE_SERVER);
    }
    obj = Com_GetServerDObj(ent->s.number);
    if ( !obj )
        Scr_ObjectError("No model exists.", SCRIPTINSTANCE_SERVER);
    cmdIndex = 0;
    if ( (flags & 1) != 0 )
    {
        if ( goalWeight <= 0.001 )
            notifyFlags = 0;
        else
            notifyFlags = 2;
        error = XAnimSetCompleteGoalWeightKnob(
                            obj,
                            anim.index,
                            goalWeight,
                            goalTime,
                            rate,
                            notifyName.stringValue,
                            notifyFlags,
                            (flags & 2) != 0,
                            cmdIndex);
    }
    else
    {
        if ( goalWeight <= 0.001 )
            notifyFlags = 0;
        else
            notifyFlags = 2;
        error = XAnimSetGoalWeightKnob(
                            obj,
                            anim.index,
                            goalWeight,
                            goalTime,
                            rate,
                            notifyName.stringValue,
                            notifyFlags,
                            (flags & 2) != 0,
                            cmdIndex);
    }
    if ( error )
        GScr_HandleAnimError(error);
    else
        G_FlagAnimForUpdate(ent);
}

void __cdecl GScr_SetAnimKnob(scr_entref_t entref)
{
    GScr_SetAnimKnobInternal(entref, 1, 0);
}

void __cdecl GScr_SetAnimKnobLimited(scr_entref_t entref)
{
    GScr_SetAnimKnobInternal(entref, 0, 0);
}

void __cdecl GScr_SetAnimKnobRestart(scr_entref_t entref)
{
    GScr_SetAnimKnobInternal(entref, 3, 0);
}

void __cdecl GScr_SetAnimKnobLimitedRestart(scr_entref_t entref)
{
    GScr_SetAnimKnobInternal(entref, 2, 0);
}

// Decomp: BlackOps.singleplayer.c sub_800D40 / sub_801D70
void __cdecl GScr_SetAnimKnobAllInternal(scr_entref_t entref, char flags, bool bFlagged, const char *paramErrorMsg)
{
    unsigned int notifyFlags;
    unsigned int numParams;
    float rate;
    XAnimTree_s *tree;
    DObj *obj;
    int cmdIndex;
    float goalWeight;
    float goalTime;
    int error;
    scr_anim_s anim;
    scr_anim_s rootAnim;
    gentity_s *ent;
    VariableUnion notifyName;
    XAnim_s *anims;
    unsigned int animParamIndex;
    unsigned int rootParamIndex;
    unsigned int weightParamIndex;
    unsigned int goalTimeParamIndex;
    unsigned int rateParamIndex;

    ent = GetEntity(entref);
    tree = GScr_GetEntAnimTree(ent);
    rate = 1.0f;
    goalTime = 0.2f;
    goalWeight = 1.0f;
    notifyName.intValue = 0;
    numParams = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( bFlagged )
    {
        animParamIndex = 2u;
        rootParamIndex = 1u;
        weightParamIndex = 3u;
        goalTimeParamIndex = 4u;
        rateParamIndex = 5u;
        if ( numParams >= 2 )
        {
            notifyName.intValue = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
            rootAnim = Scr_GetAnim(1u, tree, SCRIPTINSTANCE_SERVER);
            XAnimGetParamValue(tree, rootAnim.index, "rate", &rate);
            XAnimGetParamValue(tree, rootAnim.index, "goaltime", &goalTime);
            XAnimGetParamValue(tree, rootAnim.index, "goalweight", &goalWeight);
        }
        if ( numParams > 6 || numParams < 3 )
            Scr_Error(paramErrorMsg, 0);
    }
    else
    {
        animParamIndex = 0;
        rootParamIndex = 1u;
        weightParamIndex = 2u;
        goalTimeParamIndex = 3u;
        rateParamIndex = 4u;
        if ( numParams >= 2 )
        {
            anim = Scr_GetAnim(0, tree, SCRIPTINSTANCE_SERVER);
            XAnimGetParamValue(tree, anim.index, "rate", &rate);
            XAnimGetParamValue(tree, anim.index, "goaltime", &goalTime);
            XAnimGetParamValue(tree, anim.index, "goalweight", &goalWeight);
        }
        if ( numParams > 5 || numParams < 2 )
            Scr_Error("incorrect number of parameters", 0);
    }
    if ( numParams >= rateParamIndex )
    {
        rate = GScr_GetOptionalFloat(rateParamIndex, rate);
        if ( rate < 0.0 )
            Scr_ParamError(rateParamIndex, "must set nonnegative rate", SCRIPTINSTANCE_SERVER);
    }
    if ( numParams >= goalTimeParamIndex )
    {
        goalTime = GScr_GetOptionalFloat(goalTimeParamIndex, goalTime);
        if ( goalTime < 0.0 )
            Scr_ParamError(goalTimeParamIndex, "must set nonnegative goal time", SCRIPTINSTANCE_SERVER);
    }
    if ( numParams >= weightParamIndex )
    {
        goalWeight = GScr_GetOptionalFloat(weightParamIndex, goalWeight);
        if ( bFlagged )
        {
            if ( goalWeight <= 0.0 )
                Scr_ParamError(weightParamIndex, "must set positive weight", SCRIPTINSTANCE_SERVER);
        }
        else if ( goalWeight < 0.0 )
        {
            Scr_ParamError(weightParamIndex, "must set nonnegative weight", SCRIPTINSTANCE_SERVER);
        }
    }
    anim = Scr_GetAnim(animParamIndex, tree, SCRIPTINSTANCE_SERVER);
    rootAnim = Scr_GetAnim(rootParamIndex, tree, SCRIPTINSTANCE_SERVER);
    if ( rootAnim.tree != anim.tree )
        Scr_Error("root anim is not in the same anim tree", 0);
    if ( rootAnim.index == anim.index )
        Scr_Error("root anim is not an ancestor of the anim", 0);
    if ( bFlagged )
    {
        anims = XAnimGetAnims(tree);
        if ( !XAnimHasTime(anims, rootAnim.index) )
            Scr_ParamError(rootParamIndex, "blended nonsynchronized animation has no concept of time", SCRIPTINSTANCE_SERVER);
    }
    obj = Com_GetServerDObj(ent->s.number);
    if ( !obj )
        Scr_ObjectError("No model exists.", SCRIPTINSTANCE_SERVER);
    cmdIndex = 0;
    if ( (flags & 1) != 0 )
    {
        if ( goalWeight <= 0.001 )
            notifyFlags = 0;
        else
            notifyFlags = 2;
        error = XAnimSetCompleteGoalWeightKnobAll(
                            obj,
                            anim.index,
                            rootAnim.index,
                            goalWeight,
                            goalTime,
                            rate,
                            notifyName.stringValue,
                            notifyFlags,
                            (flags & 2) != 0,
                            cmdIndex);
    }
    else
    {
        if ( goalWeight <= 0.001 )
            notifyFlags = 0;
        else
            notifyFlags = 2;
        error = XAnimSetGoalWeightKnobAll(
                            obj,
                            anim.index,
                            rootAnim.index,
                            goalWeight,
                            goalTime,
                            rate,
                            notifyName.stringValue,
                            notifyFlags,
                            (flags & 2) != 0,
                            cmdIndex);
    }
    if ( error )
        GScr_HandleAnimError(error);
    else
        G_FlagAnimForUpdate(ent);
}

void __cdecl GScr_SetAnimKnobAll(scr_entref_t entref)
{
    GScr_SetAnimKnobAllInternal(entref, 1, 0, 0);
}

void __cdecl GScr_SetAnimKnobAllLimited(scr_entref_t entref)
{
    GScr_SetAnimKnobAllInternal(entref, 0, 0, 0);
}

void __cdecl GScr_SetAnimKnobAllRestart(scr_entref_t entref)
{
    GScr_SetAnimKnobAllInternal(entref, 3, 0, 0);
}

void __cdecl GScr_SetAnimKnobAllLimitedRestart(scr_entref_t entref)
{
    GScr_SetAnimKnobAllInternal(entref, 2, 0, 0);
}

void __cdecl GScr_SetFlaggedAnimKnob(scr_entref_t entref)
{
    GScr_SetAnimKnobInternal(entref, 1, 1);
}

void __cdecl GScr_SetFlaggedAnimKnobLimited(scr_entref_t entref)
{
    GScr_SetAnimKnobInternal(entref, 0, 1);
}

void __cdecl GScr_SetFlaggedAnimKnobRestart(scr_entref_t entref)
{
    GScr_SetAnimKnobInternal(entref, 3, 1);
}

void __cdecl GScr_SetFlaggedAnimKnobLimitedRestart(scr_entref_t entref)
{
    GScr_SetAnimKnobInternal(entref, 2, 1);
}

void __cdecl GScr_SetFlaggedAnimKnobAll(scr_entref_t entref)
{
    GScr_SetAnimKnobAllInternal(entref, 1, 1, "illegal call to SetFlaggedAnimKnobAll()\n");
}

void __cdecl GScr_SetFlaggedAnimKnobAllRestart(scr_entref_t entref)
{
    GScr_SetAnimKnobAllInternal(entref, 3, 1, "illegal call to SetFlaggedAnimKnobAllRestart()\n");
}

// Decomp: CoDSP_rdBlackOps.map.c:9618
void __cdecl GScr_HandleAnimError(int error)
{
    if ( error == 1 )
        Scr_Error("root anim is not an ancestor of the anim", 0);
    if ( error != 2
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    18581,
                    0,
                    "%s",
                    "error == XANIM_ERROR_BAD_NOTIFY") )
    {
        __debugbreak();
    }
    Scr_Error("cannot flag anim since it has 0 effective goal weight", 0);
}

// Decomp: CoDSP_rdBlackOps.map.c:9619
double __cdecl GScr_GetOptionalFloat(unsigned int iParamIndex, float fDefault)
{
    if ( Scr_GetType(iParamIndex, SCRIPTINSTANCE_SERVER) )
        return Scr_GetFloat(iParamIndex, SCRIPTINSTANCE_SERVER);
    else
        return fDefault;
}

void __cdecl ScrCmd_animscriptedInternal(scr_entref_t entref, int bDelayForActor, int bSkipRestart)
{
    (void)bSkipRestart;
    gentity_s *entity;
    DObj *obj;
    XAnimTree_s *entAnimTree;
    unsigned __int16 rootTree;
    unsigned __int16 rootIndex;
    unsigned int mode;
    scr_anim_s rootAnim;
    scr_anim_s anim;
    actor_s *actor;
    unsigned int notifyName;
    const char *targetName;
    const char *className;
    tagInfo_s *tagInfo;
    float animOrigin[4];
    float animAngles[6];
    float parentInvAxis[4][3];
    int cmdIndex;

    entity = GetEntity(entref);
    obj = Com_GetServerDObj(entity->s.number);
    if ( !obj )
        Scr_ObjectError("No model exists.", SCRIPTINSTANCE_SERVER);
    entAnimTree = GScr_GetEntAnimTree(entity);
    if ( !entAnimTree )
        Com_Error(ERR_DROP, "CODE ERROR: pAnimTree");
    rootTree = 0;
    rootIndex = 0;
    mode = 0;
    rootAnim.tree = 0;
    rootAnim.index = 0;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 4 )
    {
        mode = Scr_GetConstString(4u, SCRIPTINSTANCE_SERVER);
        if ( mode != scr_const.normal && mode != scr_const.deathplant )
        {
            Scr_Error(
                va(
                    "Illegal mode %s for animScripted. Valid modes are normal and deathplant",
                    SL_ConvertToString(mode, SCRIPTINSTANCE_SERVER)),
                0);
        }
        if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 5 && Scr_GetType(5u, SCRIPTINSTANCE_SERVER) )
        {
            rootAnim = Scr_GetAnim(5u, entAnimTree, SCRIPTINSTANCE_SERVER);
            rootTree = rootAnim.tree;
            rootIndex = rootAnim.index;
        }
    }
    anim = Scr_GetAnim(3u, entAnimTree, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(2u, animAngles, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, animOrigin, SCRIPTINSTANCE_SERVER);
    notifyName = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    actor = entity->actor;
    if ( !actor )
        goto direct_animscripted;
    if ( !actor->Physics.bIsAlive )
    {
        if ( entity->targetname )
            targetName = SL_ConvertToString(entity->targetname, SCRIPTINSTANCE_SERVER);
        else
            targetName = "<undefined>";
        className = SL_ConvertToString(entity->classname, SCRIPTINSTANCE_SERVER);
        Scr_Error(
            va(
                "tried to play a scripted animation on a dead AI; entity %i team %s origin %g %g %g targetname %s classname %s\n",
                entity->s.number,
                Sentient_NameForTeam(entity->sentient->eTeam),
                entity->r.currentOrigin[0],
                entity->r.currentOrigin[1],
                entity->r.currentOrigin[2],
                targetName,
                className),
            0);
    }
    if ( bDelayForActor )
    {
        Actor_PushState(actor, AIS_SCRIPTEDANIM);
        Actor_KillAnimScript(actor);
        if ( entity->scripted && entity->scripted->anim )
        {
            cmdIndex = 0;
            XAnimSetCompleteGoalWeight(obj, entity->scripted->anim, 1.0f, 0.2f, 1.0f, 0, 0, 0, cmdIndex);
            if ( (entity->flags & 0x2000) == 0 )
                entity->flags |= 0x40000u;
        }
        if ( rootTree )
            Scr_AddAnim(rootAnim, SCRIPTINSTANCE_SERVER);
        else
            Scr_AddUndefined(SCRIPTINSTANCE_SERVER);
        if ( mode )
            Scr_AddConstString(mode, SCRIPTINSTANCE_SERVER);
        else
            Scr_AddUndefined(SCRIPTINSTANCE_SERVER);
        Scr_AddAnim(anim, SCRIPTINSTANCE_SERVER);
        Scr_AddVector(animAngles, SCRIPTINSTANCE_SERVER);
        Scr_AddVector(animOrigin, SCRIPTINSTANCE_SERVER);
        Scr_AddConstString(notifyName, SCRIPTINSTANCE_SERVER);
        Scr_FreeThread(Scr_ExecEntThread(entity, g_scr_data.scripted_init, 6u), SCRIPTINSTANCE_SERVER);
    }
    else
    {
direct_animscripted:
        if ( !rootTree && rootIndex )
            Com_Error(ERR_DROP, "CODE ERROR: root.tree || !root.index");
        G_Animscripted(
            entity,
            animOrigin,
            animAngles,
            anim.index,
            rootIndex,
            notifyName,
            mode == scr_const.deathplant);
    }
    if ( !actor || bDelayForActor )
    {
        tagInfo = entity->tagInfo;
        if ( tagInfo )
        {
            G_CalcTagParentAxis(entity, parentInvAxis);
            MatrixInverseOrthogonal43(parentInvAxis, tagInfo->parentInvAxis);
        }
    }
}

void __cdecl ScrCmd_animscripted(scr_entref_t entref)
{
    unsigned int numParams;

    numParams = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParams > 8 )
        Scr_Error("too many parameters", 0);
    if ( numParams < 4 || numParams > 8 )
        Scr_Error("Incorrect number of parameters for animscripted command", 0);
    ScrCmd_animscriptedInternal(entref, 1, 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_animscriptedskiprestart)
void __cdecl ScrCmd_animscriptedskiprestart(scr_entref_t entref)
{
    unsigned int numParams;

    numParams = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParams > 8 )
        Scr_Error("too many parameters", 0);
    if ( numParams < 4 || numParams > 8 )
        Scr_Error("Incorrect number of parameters for animscripted command", 0);
    ScrCmd_animscriptedInternal(entref, 1, 1);
}

void __cdecl ScrCmd_stopanimscripted(scr_entref_t entref)
{
    gentity_s *entity;
    float blendTime;

    entity = GetEntity(entref);
    blendTime = 0.2f;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) >= 1 )
        blendTime = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    G_StopAnimScripted(entity, blendTime);
}

// Decomp: CoDSP_rdBlackOps.map.c (G_SetAnimTree)
void __cdecl G_SetAnimTree(gentity_s *ent, scr_animtree_t *animtree)
{
    XAnimTree_s *oldAnimTree;
    bool updateDObj;

    G_StopAnimScripted(ent, 0.2f);
    if ( ent->scripted )
        Com_Error(ERR_DROP, "CODE ERROR: !ent->scripted");
    if ( G_GetEntAnimTree(ent) != ent->pAnimTree
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    18729,
                    0,
                    "%s",
                    "G_GetEntAnimTree( ent ) == ent->pAnimTree") )
    {
        __debugbreak();
    }
    oldAnimTree = ent->pAnimTree;
    updateDObj = false;
    if ( !animtree )
    {
        if ( !oldAnimTree )
            return;
        ent->pAnimTree = 0;
        updateDObj = true;
    }
    else if ( !oldAnimTree || XAnimGetAnims(oldAnimTree) != animtree->anims )
    {
        ent->pAnimTree = Com_XAnimCreateSmallTree(animtree->anims);
        updateDObj = true;
    }
    if ( updateDObj )
    {
        G_DObjUpdate(ent);
        if ( oldAnimTree )
            Com_XAnimFreeSmallTree(oldAnimTree);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_UseAnimTree)
void __cdecl GScr_UseAnimTree(scr_entref_t entref)
{
    char *slStr;
    const char *fmtMsg;
    scr_animtree_t animtree;
    gentity_s *ent;

    ent = GetEntity(entref);
    animtree.anims = Scr_GetAnimTree(0, 1u, SCRIPTINSTANCE_SERVER).anims;
    if ( G_GetEntAnimTree(ent) != ent->pAnimTree )
    {
        slStr = SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("cannot change the animtree of classname '%s'", slStr);
        Scr_Error(fmtMsg, 0);
    }
    G_SetAnimTree(ent, &animtree);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_StopUseAnimTree @ 82616040)
void __cdecl GScr_StopUseAnimTree(scr_entref_t entref)
{
    char *slStr;
    const char *fmtMsg;
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( G_GetEntAnimTree(ent) != ent->pAnimTree )
    {
        slStr = SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("cannot change the animtree of classname '%s'", slStr);
        Scr_Error(fmtMsg, 0);
    }
    G_SetAnimTree(ent, 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_startIgnoringSpotLight @ 825F7A88)
void __cdecl ScrCmd_startIgnoringSpotLight(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    ent->s.lerp.eFlags |= 0x400u;
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_stopIgnoringSpotLight @ 825F7AD8)
void __cdecl ScrCmd_stopIgnoringSpotLight(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    ent->s.lerp.eFlags &= ~0x400u;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_StopUsingTurret @ 8261F6C8)
void __cdecl GScr_StopUsingTurret(scr_entref_t entref)
{
    gentity_s *ent;
    gentity_s *turret;

    ent = GetEntity(entref);
    if ( ent->client && (ent->client->ps.eFlags & 0x300) != 0 )
    {
        turret = &g_entities[ent->client->ps.viewlocked_entNum];
        if ( turret->s.eType == ET_MG42 )
            G_ClientStopUsingTurret(turret);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ToggleIK @ 8261E5C8)
void __cdecl GScr_ToggleIK(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( Scr_GetInt(0, SCRIPTINSTANCE_SERVER) )
        ent->s.lerp.eFlags2 &= ~0x10000000u;
    else
        ent->s.lerp.eFlags2 |= 0x10000000u;
    Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_TurretFireDisable @ 82618088)
void __cdecl GScr_TurretFireDisable(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->pTurretInfo )
    {
        Scr_Error(
            va("entity type '%s' is not a turret", SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER)),
            0);
    }
    ent->pTurretInfo->flags |= 0x4000u;
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_TurretFireEnable @ 82618118)
void __cdecl GScr_TurretFireEnable(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->pTurretInfo )
    {
        Scr_Error(
            va("entity type '%s' is not a turret", SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER)),
            0);
    }
    ent->pTurretInfo->flags &= ~0x4000u;
}

// Decomp: CoDSP_rdBlackOps.map.c (ScrCmd_UFOMode @ 825F6218)
void __cdecl ScrCmd_UFOMode(scr_entref_t entref)
{
    gentity_s *ent;
    int enable;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_Error("Tried to set UFO mode on a non-player entity!", 0);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        enable = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    else
        enable = (ent->client->flags & 2) == 0;
    if ( enable <= 0 )
    {
        ent->client->flags &= ~2u;
        Dvar_SetBoolByName("hud_drawHUD", 1);
    }
    else
    {
        ent->client->flags |= 2u;
        Dvar_SetBoolByName("hud_drawHUD", 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_Stop3DCinematic @ 8260A410)
void GScr_Stop3DCinematic()
{
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        Scr_Error("stop3DCinematic takes no parameters: stop3DCinematic()\n", 0);
    SV_GameSendServerCommand(-1, SV_CMD_RELIABLE, ".");
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_startMultiplayerGame @ 8261C950)
void GScr_startMultiplayerGame()
{
    Cbuf_AddText(0, "startMultiplayer\n");
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ZeroGravityVolumeOn @ 825F4670)
void GScr_ZeroGravityVolumeOn()
{
    const char *volumeName;
    int i;

    volumeName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    for ( i = 0; i < num_zero_gravity_volumes; ++i )
    {
        if ( !I_stricmp(zero_gravity_names[i], volumeName) )
            zero_gravity_on[i] = true;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_ZeroGravityVolumeOff @ 825F4708)
void GScr_ZeroGravityVolumeOff()
{
    const char *volumeName;
    int i;

    volumeName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    for ( i = 0; i < num_zero_gravity_volumes; ++i )
    {
        if ( !I_stricmp(zero_gravity_names[i], volumeName) )
            zero_gravity_on[i] = false;
    }
}

void (__cdecl *__cdecl Scr_GetMethod(const char **pName, int *type))(scr_entref_t)
{
    void (__cdecl *method)(scr_entref_t);

    *type = 0;
    method = Player_GetMethod(pName);
    if ( method )
        return method;
    method = ScriptEnt_GetMethod(pName);
    if (method)
        return method;
    method = ScriptVehicle_GetMethod(pName);
    if (method)
        return method;
    method = HudElem_GetMethod(pName);
    if (method)
        return method;
    method = Helicopter_GetMethod(pName);
    if (method)
        return method;
    method = Actor_GetMethod(pName);
    if ( method )
        return method;
    method = Sentient_GetMethod(pName);
    if ( method )
        return method;

    method = BuiltIn_GetMethod(pName, type);

    return method;
}

// Decomp: CoDSP_rdBlackOps.map.c (METHOD_NULLSUB)
static void METHOD_NULLSUB(scr_entref_t ref)
{

}

BuiltinMethodDef s_builtinMethods[] =
{
  { "addaieventlistener", ScrCmd_AddAIEventListener, 0 },
  { "removeaieventlistener", ScrCmd_RemoveAIEventListener, 0 },
  { "attach", ScrCmd_attach, 0 },
  { "detach", ScrCmd_detach, 0 },
  { "detachall", ScrCmd_detachAll, 0 },
  { "bypasssledgehammer", GScr_BypassSledgehammer, 0 },
  { "getattachsize", ScrCmd_GetAttachSize, 0 },
  { "getattachmodelname", ScrCmd_GetAttachModelName, 0 },
  { "getattachtagname", ScrCmd_GetAttachTagName, 0 },
  { "getattachignorecollision", ScrCmd_GetAttachIgnoreCollision, 0 },
  { "getammocount", GScr_GetAmmoCount, 0 },
  { "getweaponmuzzlepoint", Scr_GetWeaponMuzzlePoint, 0 },
  { "getweaponforwarddir", Scr_GetWeaponForwardDir, 0 },
  { "getclanid", ScrCmd_GetClanId, 0 },
  { "getclanname", ScrCmd_GetClanName, 0 },
  { "hidepart", ScrCmd_hidepart, 0 },
  { "useweaponhidetags", GScr_UseWeaponHideTags, 0 },
  { "playweapondeatheffects", GScr_PlayWeaponDeathEffects, 0 },
  { "showpart", ScrCmd_showpart, 0 },
  { "showallparts", ScrCmd_showallparts, 0 },
  { "setvisibletoplayer", ScrCmd_SetVisibleToPlayer, 0 },
  { "setinvisibletoplayer", ScrCmd_SetInvisibleToPlayer, 0 },
  { "setvisibletoall", ScrCmd_SetVisibleToAll, 0 },
  { "setinvisibletoall", ScrCmd_SetInvisibleToAll, 0 },
  { "setvisibletoteam", ScrCmd_SetVisibleToTeam, 0 },
  { "setforcenocull", ScrCmd_SetForceNoCull, 0 },
  // LWSS ADD FROM LATEST BLOPS RETAIL MP
  { "overridelightingorigin", ScrCmd_OverrideLightingOrigin, 0 },
  // LWSS END
  { "islinkedto", ScrCmd_IsLinkedTo, 0 },
  { "linkto", ScrCmd_LinkTo, 0 },
  { "playerlinkto", ScrCmd_PlayerLinkTo, 0 },
  { "playerlinktodelta", ScrCmd_PlayerLinkToDelta, 0 },
  { "playerlinktoabsolute", ScrCmd_PlayerLinkToAbsolute, 0 },
  { "playeranimscriptevent", ScrCmd_PlayerAnimScriptEvent, 0 },
  { "playeranimscripteventend", ScrCmd_PlayerAnimScriptEventEnd, 0 },
  { "scalebuoyancy", ScrCmd_ScaleBuoyancy, 0 },
  { "disabledepthbuoyancyadjustments", ScrCmd_DisableDepthBuoyancyAdjustments, 0 },
  { "unlink", ScrCmd_Unlink, 0 },
  { "getlinkedent", ScrCmd_GetLinkedEnt, 0 },
  { "enablelinkto", ScrCmd_EnableLinkTo, 0 },
  { "disableclientlinkto", ScrCmd_DisableClientLinkTo, 0 },
  { "enableclientlinkto", ScrCmd_EnableClientLinkTo, 0 },
  { "enablesledgehammer", GScr_EnableSledgehammer, 0 },
  { "getorigin", ScrCmd_GetOrigin, 0 },
  { "getangles", ScrCmd_GetAngles, 0 },
  { "getmins", ScrCmd_GetMins, 0 },
  { "getmaxs", ScrCmd_GetMaxs, 0 },
  { "getabsmins", ScrCmd_GetAbsMins, 0 },
  { "getabsmaxs", ScrCmd_GetAbsMaxs, 0 },
  { "getpointinbounds", ScrCmd_GetPointInBounds, 0 },
  { "geteye", ScrCmd_GetEye, 0 },
  { "geteyeapprox", ScrCmd_GetEyeApprox, 0 },
  { "useby", ScrCmd_UseBy, 0 },
  { "setstablemissile", Scr_SetStableMissile, 0 },
  { "istouching", ScrCmd_IsTouching, 0 },
  { "istouchingswept", ScrCmd_IsTouchingSwept, 0 },
  // LWSS ADD FROM LATEST BLOPS RETAIL MP
  { "istouchingvolume", ScrCmd_IsTouchingVolume, 0 },
  // LWSS END
  { "playsound", ScrCmd_PlaySound, 0 },
  { "playsoundontag", ScrCmd_PlaySoundOnTag, 0 },
  { "playsoundasmaster", ScrCmd_PlaySound, 0 },
  { "playsoundtoteam", ScrCmd_PlaySoundToTeam, 0 },
  { "playbattlechattertoteam", ScrCmd_PlayBattleChatterToTeam, 0 },
  { "playsoundtoplayer", ScrCmd_PlaySoundToPlayer, 0 },
  { "playloopsound", ScrCmd_PlayLoopSound, 0 },
  { "setsoundblend", GScr_SetSoundBlend, 0 },
  { "stoploopsound", ScrCmd_StopLoopSound, 0 },
  { "stopsound", ScrCmd_StopSound, 0 },
  { "stopsounds", ScrCmd_StopSounds, 0 },
  { "iswaitingonsound", ScrCmd_IsWaitingOnSound, 0 },
  { "playrumbleonentity", ScrCmd_PlayRumbleOnEntity, 0 },
  { "playrumblelooponentity", ScrCmd_PlayRumbleLoopOnEntity, 0 },
  { "stoprumble", ScrCmd_StopRumble, 0 },
  { "delete", ScrCmd_Delete, 0 },
  { "setmodel", ScrCmd_SetModel, 0 },
  { "setenemymodel", ScrCmd_SetEnemyModel, 0 },
  { "dodamage", ScrCmd_DoDamage, 0 },
  { "getnormalhealth", ScrCmd_GetNormalHealth, 0 },
  { "setnormalhealth", ScrCmd_SetNormalHealth, 0 },
  { "setmaxhealth", ScrCmd_SetMaxHealth, 0 },
  { "show", ScrCmd_Show, 0 },
  { "hide", ScrCmd_Hide, 0 },
  { "ghost", ScrCmd_Ghost, 0 },
  { "laseron", METHOD_NULLSUB, 0 },
  { "laseroff", METHOD_NULLSUB, 0 },
  { "showtoplayer", ScrCmd_ShowToPlayer, 0 },
  { "setcontents", ScrCmd_SetContents, 0 },
  { "startfiring", GScr_StartFiring, 0 },
  { "stopfiring", GScr_StopFiring, 0 },
  { "shootturret", GScr_ShootTurret, 0 },
  { "stopshootturret", GScr_StopShootTurret, 0 },
  { "setmode", GScr_SetMode, 0 },
  { "getturretowner", GScr_GetTurretOwner, 0 },
  { "settargetentity", GScr_SetTargetEntity, 0 },
  { "setplayerspread", GScr_SetPlayerSpread, 0 },
  { "setaispread", GScr_SetAiSpread, 0 },
  { "setconvergencetime", GScr_SetConvergenceTime, 0 },
  { "setsuppressiontime", GScr_SetSuppressionTime, 0 },
  { "cleartargetentity", GScr_ClearTargetEntity, 0 },
  { "setturretteam", GScr_SetTurretTeam, 0 },
  { "maketurretusable", GScr_MakeTurretUsable, 0 },
  { "maketurretunusable", GScr_MakeTurretUnusable, 0 },
  { "setturretaccuracy", GScr_SetTurretAccuracy, 0 },
  { "setturretignoregoals", GScr_SetTurretIgnoreGoals, 0 },
  { "getturrettarget", GScr_GetTurretTarget, 0 },
  { "disconnectpaths", GScr_DisconnectPaths, 0 },
  { "dontinterpolate", ScrCmd_DontInterpolate, 0 },
  { "connectpaths", GScr_ConnectPaths, 0 },
  { "getstance", ScrCmd_GetStance, 0 },
  // LWSS ADD FROM LATEST BLOPS RETAIL MP
  { "setstance", ScrCmd_SetStance, 0 },
  // LWSS END
  { "setcursorhint", GScr_SetCursorHint, 0 },
  { "setrevivehintstring", GScr_SetReviveHintString, 0 },
  { "sethintstring", GScr_SetHintString, 0 },
  { "sethintstringforperk", GScr_SetHintStringForPerk, 0 },
  { "sethintlowpriority", GScr_SetHintLowPriority, 0 },
  { "usetriggerrequirelookat", GScr_UseTriggerRequireLookAt, 0 },
  { "shellshock", GScr_ShellShock, 0 },
  { "setvolfog", GScr_SetVolumetricFog, 0 },
  { "gettagorigin", GScr_GetTagOrigin, 0 },
  { "gettagangles", GScr_GetTagAngles, 0 },
  { "getyawtoorigin", GScr_GetYawToOrigin, 0 },
  { "geteyeyawtoorigin", GScr_GetEyeYawToOrigin, 0 },
  { "getyawtotag", GScr_GetYawToTag, 0 },
  { "getentnum", GScr_GetEntnum, 1 },
  { "stopshellshock", GScr_StopShellShock, 0 },
  { "setdepthoffield", GScr_SetDepthOfField, 0 },
  { "getdepthoffield_nearstart", GScr_GetDepthOfField_NearStart, 0 },
  { "getdepthoffield_nearend", GScr_GetDepthOfField_NearEnd, 0 },
  { "getdepthoffield_farstart", GScr_GetDepthOfField_FarStart, 0 },
  { "getdepthoffield_nearblur", GScr_GetDepthOfField_NearBlur, 0 },
  { "getdepthoffield_farblur", GScr_GetDepthOfField_FarBlur, 0 },
  { "getdepthoffield_farend", GScr_GetDepthOfField_FarEnd, 0 },
  { "getdestructiblename", ScrCmd_GetDestructibleName, 0 },
  { "gettimescale", ScrCmd_GetTimeScale_method, 0 },
  { "haspath", GScr_HasPath, 0 },
  { "setburn", GScr_SetBurn, 0 },
  { "setelectrified", GScr_SetElectrified, 0 },
#ifdef KISAK_SP
  { "startfadingblur", GScr_StartFadingBlur, 0 },
  { "setexploderid", ScrCmd_SetExploderId, 0 },
#endif
  { "spawnnapalmgroundflame", GScr_SpawnNapalmGroundFlame_method, 0 },
  { "needsrevive", GScr_NeedsRevive, 0 },
  { "isinsecondchance", GScr_IsInSecondChance, 0 },
  { "depthinwater", GScr_DepthInWater, 0 },
  { "shootup", GScr_ShootUp, 0 },
  { "depthofplayerinwater", GScr_DepthOfPlayerInWater, 0 },
  { "starttanning", GScr_StartTanning, 0 },
  { "stopburning", GScr_StopBurning, 0 },
  // LWSS ADD FROM LATEST BLOPS RETAIL MP
  { "setwaterdrops", GScr_SetWaterDrops, 0 },
  // LWSS END
  { "restoredefaultdroppitch", GScr_RestoreDefaultDropPitch, 0 },
  { "clearcenterpopups", GScr_clearCenterPopups, 0 },
  { "clearpopups", GScr_clearPopups, 0 },
  { "displaymedal", GScr_DisplayMedal, 0 },
  { "displaygamemodemessage", GScr_DisplayGameModeMessage, 0 },
  { "displayteammessage", GScr_DisplayTeamMessage, 0 },
  { "displaycontract", GScr_DisplayContract, 0 },
  { "displaychallengecomplete", GScr_DisplayChallengeComplete, 0 },
  { "clearendgame", GScr_ClearEndGameComplete, 0 },
  { "displayendgame", GScr_DisplayEndGame, 0 },
  { "displayendgamemilestone", GScr_DisplayEndGameMilestoneComplete, 0 },
  { "displaykillstreak", GScr_DisplayKillstreak, 0 },
  { "displayrankup", GScr_DisplayRankUp, 0 },
  { "displaywagerpopup", GScr_DisplayWagerPopup, 0 },
  { "displayhudanim", GScr_DisplayHudAnim, 0 },
  { "isfiringturret", GScr_IsFiringTurret, 0 },
  { "isturretlockedon", GScr_IsTurretLockedOn, 0 },
  { "setviewmodeldepthoffield", GScr_SetViewModelDepthOfField, 0 },
  { "viewkick", GScr_ViewKick, 0 },
  { "localtoworldcoords", GScr_LocalToWorldCoords, 0 },
  { "worldtolocalcoords", GScr_WorldToLocalCoords, 0 },
  { "startignoringspotlight", ScrCmd_startIgnoringSpotLight, 0 },
  { "stopignoringspotlight", ScrCmd_stopIgnoringSpotLight, 0 },
  { "stopusingturret", GScr_StopUsingTurret, 0 },
  { "toggleik", GScr_ToggleIK, 0 },
  { "turretfiredisable", GScr_TurretFireDisable, 0 },
  { "turretfireenable", GScr_TurretFireEnable, 0 },
  { "ufomode", ScrCmd_UFOMode, 0 },
  { "setrightarc", GScr_SetRightArc, 0 },
  { "setleftarc", GScr_SetLeftArc, 0 },
  { "settoparc", GScr_SetTopArc, 0 },
  { "setbottomarc", GScr_SetBottomArc, 0 },
  { "radiusdamage", GScr_EntityRadiusDamage, 0 },
  { "entityradiusdamage", GScr_EntityRadiusDamage, 0 },
  { "detonate", GScr_Detonate, 0 },
  { "damageconetrace", GScr_DamageConeTrace, 0 },
  { "sightconetrace", GScr_SightConeTrace, 0 },
  { "heliturretsighttrace", GScr_HeliTurretSightTrace, 0 },
  { "heliturretdogtrace", GScr_HeliTurretDogTrace, 0 },
  { "playersighttrace", GScr_PlayerSightTrace, 0 },
  { "visionsetlerpratio", GScr_VisionSetLerpRatio, 0 },
  { "docowardswayanims", GScr_DoCowardsWayAnims, 0 },
  { "startpoisoning", GScr_StartPoisoning, 0 },
  { "stoppoisoning", GScr_StopPoisoning, 0 },
  { "startbinocs", GScr_StartBinocs, 0 },
  { "stopbinocs", GScr_StopBinocs, 0 },
  { "isflared", GScr_IsFlared, 0 },
  { "ispoisoned", GScr_IsPoisoned, 0 },
  { "setcameraspikeactive", GScr_SetCameraSpikeActive, 0 },
  { "ismissileinsideheightlock", GScr_IsMissileInsideHeightLock, 0 },
  { "isonground", GScr_IsOnGround, 0 },
  // LWSS ADD FROM LATEST BLOPS RETAIL MP
  { "getgroundent", GScr_GetGroundEnt, 0 },
  // LWSS END
  { "setanim", GScr_SetAnim, 0 },
  { "setanimlimited", GScr_SetAnimLimited, 0 },
  { "setanimrestart", GScr_SetAnimRestart, 0 },
  { "setanimlimitedrestart", GScr_SetAnimLimitedRestart, 0 },
  { "setflaggedanim", GScr_SetFlaggedAnim, 0 },
  { "setflaggedanimlimited", GScr_SetFlaggedAnimLimited, 0 },
  { "setflaggedanimrestart", GScr_SetFlaggedAnimRestart, 0 },
  { "setflaggedanimlimitedrestart", GScr_SetFlaggedAnimLimitedRestart, 0 },
  { "setanimknob", GScr_SetAnimKnob, 0 },
  { "setanimknoblimited", GScr_SetAnimKnobLimited, 0 },
  { "setanimknobrestart", GScr_SetAnimKnobRestart, 0 },
  { "setanimknoblimitedrestart", GScr_SetAnimKnobLimitedRestart, 0 },
  { "setanimknoball", GScr_SetAnimKnobAll, 0 },
  { "setanimknoballlimited", GScr_SetAnimKnobAllLimited, 0 },
  { "setanimknoballrestart", GScr_SetAnimKnobAllRestart, 0 },
  { "setanimknoballlimitedrestart", GScr_SetAnimKnobAllLimitedRestart, 0 },
  { "setflaggedanimknob", GScr_SetFlaggedAnimKnob, 0 },
  { "setflaggedanimknoblimited", GScr_SetFlaggedAnimKnobLimited, 0 },
  { "setflaggedanimknobrestart", GScr_SetFlaggedAnimKnobRestart, 0 },
  {
    "setflaggedanimknoblimitedrestart",
    GScr_SetFlaggedAnimKnobLimitedRestart,
    0
  },
  { "setflaggedanimknoball", GScr_SetFlaggedAnimKnobAll, 0 },
  { "setflaggedanimknoballrestart", GScr_SetFlaggedAnimKnobAllRestart, 0 },
  { "getanimtime", GScr_GetAnimTime, 0 },
  { "getanimassettype", GScr_GetAnimAssetType, 0 },
  { "getanimvalue", GScr_GetAnimValue, 0 },
  { "setanimtime", GScr_SetAnimTime, 0 },
  { "setanimrate", GScr_SetAnimRate, 0 },
  { "setanimratecomplete", GScr_SetAnimRateComplete, 0 },
  { "setanimforcenew", GScr_SetAnimForceNew, 0 },
  { "setcellinvisible", ScrCmd_SetCellInvisible, 0 },
  { "setcellvisible", ScrCmd_SetCellVisible, 0 },
  { "setclientflagasval", GScr_SetClientFlagAsVal, 0 },
  { "setdoublevision", GScr_SetDoubleVision, 0 },
  { "setentgravitytrajectory", Scr_SetEntGravityTrajectory, 0 },
  { "setfriendlychain", GScr_SetFriendlyChain, 0 },
  { "sethudwarningtype", GScr_SetHudWarningType, 0 },
  { "setlocalwindsource", GScr_SetLocalWindSource, 0 },
  { "clearanim", GScr_ClearAnim, 0 },
  { "clearanimlimited", GScr_ClearAnimLimited, 0 },
  { "clearplayergravity", Scr_ClearPlayerGravity, 0 },
  { "animrelative", ScrCmd_animrelative, 0 },
  { "animscripted", ScrCmd_animscripted, 0 },
  { "animscriptedskiprestart", ScrCmd_animscriptedskiprestart, 0 },
  { "stopanimscripted", ScrCmd_stopanimscripted, 0 },
  { "dumpanims", GScr_DumpAnims, 0 },
  { "useanimtree", GScr_UseAnimTree, 0 },
  { "stopuseanimtree", GScr_StopUseAnimTree, 0 },
  { "ismartyrdomgrenade", GScr_IsMartyrdomGrenade, 0 },
  { "getentitynumber", GScr_GetEntityNumber, 0 },
  { "enablegrenadetouchdamage", GScr_EnableGrenadeTouchDamage, 0 },
  { "disablegrenadetouchdamage", GScr_DisableGrenadeTouchDamage, 0 },
  { "enableaimassist", GScr_EnableAimAssist, 0 },
  { "disableaimassist", GScr_DisableAimAssist, 0 },
  { "placespawnpoint", GScr_PlaceSpawnPoint, 0 },
  { "setspawnclientflag", METHOD_NULLSUB, 0 },
  { "directionalhitindicator", GScr_DirectionalHitIndicator, 0 },
  { "sendfaceevent", ScrCmd_SendFaceEvent, 0 },
  { "setteamfortrigger", GScr_SetTeamForTrigger, 0 },
  { "setteamforentity", GScr_SetTeamForEntity, 0 },
  { "setperkfortrigger", GScr_SetPerkForTrigger, 0 },
  { "setignoreentfortrigger", GScr_SetIgnoreEntForTrigger, 0 },
  { "clientclaimtrigger", GScr_ClientClaimTrigger, 0 },
  { "clientreleasetrigger", GScr_ClientReleaseTrigger, 0 },
  { "releaseclaimedtrigger", GScr_ReleaseClaimedTrigger, 0 },
  { "isitemlocked", GScr_IsItemLocked, 0 },
  { "isitempurchased", GScr_IsItemPurchased, 0 },
  { "getdstat", GScr_GetDStat, 0 },
  { "setdstat", GScr_SetDStat, 0 },
  { "sendleaderboards", GScr_SendLeaderboards, 0 },
  { "setnemesisxuid", GScr_SetNemesisXuid, 0 },
  { "getloadoutitemfromprofile", GScr_GetLoadoutItemFromProfile, 0 },
  { "setmovespeedscale", ScrCmd_SetMoveSpeedScale, 0 },
  { "getmovespeedscale", ScrCmd_GetMoveSpeedScale, 0 },
  { "logstring", METHOD_NULLSUB, 0 },
  { "missile_settarget", GScr_MissileSetTarget, 0 },
  { "missilesettarget", GScr_MissileSetTarget, 0 },
  { "isonladder", GScr_IsOnLadder, 0 },
  { "ismantling", GScr_IsMantling, 0 },
  { "startdoorbreach", GScr_StartDoorBreach, 0 },
  { "stopdoorbreach", GScr_StopDoorBreach, 0 },
  { "startragdoll", GScr_StartRagdoll, 0 },
  { "isragdoll", GScr_IsRagdoll, 0 },
  { "isbeingwatched", GScr_IsBeingWatched, 0 },
  { "isplayerreloading", ScrCmd_IsPlayerReloading, 0 },
  { "issprinting", GScr_IsSprinting, 0 },
  { "itemweaponsetoptions", ScrCmd_ItemWeaponSetOptions, 0 },
  { "magicgrenadetype", ScrCmd_MagicGrenadeType, 0 },
  { "netupdate", GScr_NetUpdate, 0 },
  { "launchragdoll", GScr_RagdollLaunch, 0 },
  { "ragdolllaunch", GScr_RagdollLaunch, 0 },
  { "launchvehicle", GScr_VehicleLaunch, 0 },
  { "vehiclelaunch", GScr_VehicleLaunch, 0 },
  { "giveachievement", GScr_GiveAchievement, 0 },
  { "givegamerpicture", GScr_GiveGamerPicture, 0 },
  { "setvehicleteam", GScr_SetTeam, 0 },
  { "setteam", GScr_SetTeam, 0 },
  { "setspawnerteam", GScr_SetSpawnerTeam, 0 },
  { "setphysparams", ScrCmd_SetPhysParams, 0 },
  { "getteam", GScr_GetTeam, 0 },
  { "setowner", GScr_SetOwner, 0 },
  { "setturretowner", GScr_SetTurretOwner, 0 },
  { "setturrettype", GScr_SetTurretType, 0 },
  { "getcorpseanim", GScr_GetCorpseAnim, 0 },
  { "itemweaponsetammo", ScrCmd_ItemWeaponSetAmmo, 0 },
  { "getlightcolor", GScr_GetLightColor, 0 },
  { "setlightcolor", GScr_SetLightColor, 0 },
  { "getlightintensity", GScr_GetLightIntensity, 0 },
  { "setlightintensity", GScr_SetLightIntensity, 0 },
  { "getlightradius", GScr_GetLightRadius, 0 },
  { "setlightradius", GScr_SetLightRadius, 0 },
  { "getlightfovinner", GScr_GetLightFovInner, 0 },
  { "getlightfovouter", GScr_GetLightFovOuter, 0 },
  { "setlightfovrange", GScr_SetLightFovRange, 0 },
  { "getlightexponent", GScr_GetLightExponent, 0 },
  { "setlightexponent", GScr_SetLightExponent, 0 },
  { "setturretcarried", GScr_SetTurretCarried, 0 },
  { "reportuser", METHOD_NULLSUB, 0 },
  { "getvelocity", ScrCmd_GetVelocity, 0 },
  { "getaivelocity", ScrCmd_GetAiVelocity, 0 },
  { "spawnactor", ScrCmd_SpawnActor, 0 },
  { "codespawnerspawn", ScrCmd_CodeSpawnerSpawn, 0 },
  { "codespawnerforcespawn", ScrCmd_CodeSpawnerForceSpawn, 0 },
  { "codeforcespawn", ScrCmd_CodeForceSpawn, 0 },
  { "precachespawn", ScrCmd_PrecacheSpawn, 0 },
  { "precachespawner", ScrCmd_PrecacheSpawn, 0 },
  { "getplayerconnectionstate", GScrCmd_GetPlayerConnectionState, 0 },
  { "setwatersheeting", GScrCmd_SetWaterSheeting, 0 },
  { "hasplayerusedweapon", GScr_HasPlayerUsedWeapon, 0 },
  { "hasusedweapon", GScr_HasPlayerUsedWeapon, 0 },
  { "damageconetraceinternal", GScr_DamageConeTraceInternalCmd, 0 },
  { "setaniminternal", GScr_SetAnimInternalCmd, 0 },
  { "setanimknobinternal", GScr_SetAnimKnobInternalCmd, 0 },
  { "setanimknoballinternal", GScr_SetAnimKnobAllInternalCmd, 0 },
  { "setflaggedaniminternal", GScr_SetFlaggedAnimInternalCmd, 0 },
  { "setflaggedanimknobinternal", GScr_SetFlaggedAnimKnobInternalCmd, 0 },
  { "setflaggedanimknoballinternal", GScr_SetFlaggedAnimKnobAllInternalCmd, 0 },
  { "setuplightentity", GScr_SetupLightEntityCmd, 0 },
  { "startxdoll", GScr_StartXDoll, 0 },
  { "updatetaginternal", GScr_UpdateTagInternalCmd, 0 },
  { "radiusdamageinternal", GScr_RadiusDamageInternalCmd, 0 },
  { "getshootatpos", ScrCmd_GetShootAtPosition, 0 },
  { "getshootatposition", ScrCmd_GetShootAtPosition, 0 },
  { "setdefaultdroppitch", GScr_SetDefaultDropPitch, 0 },
  { "setscanningpitch", GScr_SetScanningPitch, 0 },
  { "launchbomb", GScr_LaunchBomb, 0 },
  { "launch", GScr_Launch, 0 },
  { "makegrenadedud", GScr_MakeGrenadeDud, 0 },
  { "setclientflag", GScr_SetClientFlag, 0 },
  // LWSS ADD FROM LATEST BLOPS RETAIL MP
  { "getclientflag", GScr_GetClientFlag, 0 },
  // LWSS END
  { "clearclientflag", GScr_ClearClientFlag, 0 },
  { "fakefire", GScr_FakeFire, 0 },
  { "floatlonger", ScrCmd_FloatLonger, 0 },
  { "forcebuoyancy", ScrCmd_ForceBuoyancy, 0 },
  { "forcetexturestoload", ScrCmd_ForceTexturesToLoad, 0 },
  { "makeusable", ScrCmd_MakeUsable, 0 },
  { "makeunusable", ScrCmd_MakeUnusable, 0 },
  { "predictgrenade", GScr_PredictGrenade, 0 },
  { "getindexforactivecontract", GScr_GetIndexForActiveContract, 0 },
  { "getactivecontractprogress", GScr_GetActiveContractProgress, 0 },
  {
    "incrementactivecontractprogress",
    GScr_IncrementActiveContractProgress,
    0
  },
  { "incrementactivecontracttime", GScr_IncrementActiveContractTime, 0 },
  { "isactivecontractcomplete", GScr_IsActiveContractComplete, 0 },
  { "hasactivecontractexpired", GScr_HasActiveContractExpired, 0 },
  { "getactivecontracttimepassed", GScr_GetActiveContractTimePassed, 0 },
  { "resetactivecontractprogress", GScr_ResetActiveContractProgress, 0 },
  { "getpregameclass", GScr_GetPregameClass, 0 },
  { "getpregameteam", GScr_GetPregameTeam, 0 },
  { "setpregameclass", GScr_SetPregameClass, 0 },
  { "setpregameteam", GScr_SetPregameTeam, 0 },
  { "isdemoclient", GScr_isDemoClient, 0 },
  { "istestclient", GScr_isTestClient, 0 },
#ifdef KISAK_SP
  { "bloodimpact", ScrCmd_BloodImpact, 0 },
  { "transmittargetname", ScrCmd_TransmitTargetName, 0 },
  { "makefakeai", ScrCmd_MakeFakeAI, 0 },
  { "getvisionsetnaked", ScrCmd_GetVisionSetNaked, 0 },
  { "visionsetnaked", GScr_VisionSetNaked, 0 },
  { "visionsetlaststand", ScrCmd_VisionSetLastStand, 0 },
  { "lookatentity", ScrCmd_LookAtEntity, 0 },
  { "getdebugeye", ScrCmd_GetDebugEye, 0 },
  { "has_spawnflag", ScrCmd_HasSpawnFlag, 0 },
  { "gib", ScrCmd_Gib, 0 },
  { "gibent", GScr_GibEnt, 0 },
  { "setblur", GScr_SetBlur, 0 },
  { "haseyes", ScrCmd_HasEyes, 0 },
  { "getcentroid", ScrCmd_GetCentroid, 0 },
  { "setdeathcontents", ScrCmd_SetDeathContents, 0 },
  { "setlookattext", GScr_SetLookatText, 0 },
  { "setplayercollision", Scr_SetPlayerCollision, 0 },
  { "setplayergravity", Scr_SetPlayerGravity, 0 },
  { "setscale", ScrCmd_SetScale, 0 },
  { "setshadowhint", ScrCmd_SetShadowHint, 0 },
  { "settransported", GScr_SetTransported, 0 },
  { "setvehicleattachments", GScr_SetVehicleAttachments, 0 },
  { "setwetness", GScr_SetWetness, 0 },
  { "setzombiename", GScr_SetZombieName, 0 },
  { "setzombieshrink", ScrCmd_SetZombieShrink, 0 },
  { "playersetgroundreferenceent", ScrCmd_PlayerSetGroundReferenceEnt, 0 },
  { "resetmissiledetonationtime", GScrCmd_ResetMissileDetonationTime, 0 }
#endif
};



void (__cdecl *__cdecl BuiltIn_GetMethod(const char **pName, int *type))(scr_entref_t)
{
    unsigned int methodIndex;

    for ( methodIndex = 0; methodIndex < ARRAY_COUNT(s_builtinMethods); ++methodIndex )
    {
        if ( !strcmp(*pName, s_builtinMethods[methodIndex].actionString) )
        {
            *pName = s_builtinMethods[methodIndex].actionString;
            *type = s_builtinMethods[methodIndex].type;
            return s_builtinMethods[methodIndex].actionFunc;
        }
    }
    return 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetOrigin)
void __cdecl Scr_SetOrigin(gentity_s *ent, int)
{
    float org[3];

    Scr_GetVector(0, org, SCRIPTINSTANCE_SERVER);
    G_SetOrigin(ent, org);
    if ( ent->r.linked )
        SV_LinkEntity(ent);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetAngles)
void __cdecl Scr_SetAngles(gentity_s *ent, int)
{
    float angles[3];

    Scr_GetVector(0, angles, SCRIPTINSTANCE_SERVER);
    G_SetAngle(ent, angles);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetExposureIndex)
void __cdecl Scr_SetExposureIndex(gentity_s *ent, int)
{
    ent->item[1].index = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetExposureLerpToLighter)
void __cdecl Scr_SetExposureLerpToLighter(gentity_s *ent, int unused)
{
    ent->trigger.exposureLerpToLighter = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetExposureLerpToDarker)
void __cdecl Scr_SetExposureLerpToDarker(gentity_s *ent, int unused)
{
    ent->trigger.exposureLerpToDarker = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetHealth)
void __cdecl Scr_SetHealth(gentity_s *ent, int unused)
{
    int health;

    health = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( ent->client )
    {
        ent->health = health;
        ent->client->ps.stats[0] = health;
    }
    else
    {
        ent->maxHealth = health;
        ent->health = health;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c:9846
void __cdecl GScr_AddVector(float *vVec)
{
    if ( vVec )
        Scr_AddVector(vVec, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddUndefined(SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c:9847
void __cdecl GScr_AddEntity(gentity_s *pEnt)
{
    if ( pEnt )
        Scr_AddEntity(pEnt, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddUndefined(SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_ParseGameTypeList)
void __cdecl Scr_ParseGameTypeList()
{
    if ( useFastFile->current.enabled )
        ((void (__cdecl *)(void (*)()))Scr_ParseGameTypeList_FastFile)(Scr_ParseGameTypeList_FastFile);
    else
        ((void (__cdecl *)(int (*)()))Scr_ParseGameTypeList_LoadObj)(Scr_ParseGameTypeList_LoadObj);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_ParseGameTypeList_LoadObj)
int Scr_ParseGameTypeList_LoadObj()
{
    const char *fmtMsg;
    const char *fmtMsg2;
    int result;
    char *qpath;
    char *src;
    unsigned __int8 buffer[1024];
    char *data_p;
    char *s0;
    char listbuf[4096];
    int f;
    unsigned int tempValue10;
    int idx;
    int len;
    int i;
    char *dest;
    int FileList;

    memset((unsigned __int8 *)g_scr_data.gametype.list, 0, sizeof(g_scr_data.gametype.list));
    idx = 0;
    FileList = FS_GetFileList("maps/gametypes", (char*)"gsc", FS_LIST_PURE_ONLY, listbuf, 4096);
    src = listbuf;
    for ( i = 0; i < FileList; ++i )
    {
        tempValue10 = strlen(src);
        if ( *src == 95 )
        {
            src += tempValue10 + 1;
        }
        else
        {
            if ( !I_stricmp(&src[tempValue10 - 4], ".gsc") )
                src[tempValue10 - 4] = 0;
            if ( idx == 32 )
            {
                Com_Printf(24, "Too many game type scripts found! Only loading the first %i\n", 31);
                break;
            }
            dest = g_scr_data.gametype.list[idx].pszScript;
            I_strncpyz(dest, src, 64);
            //strlwr(dest);
            _strlwr(dest);
            qpath = va("maps/gametypes/%s.txt", src);
            len = FS_FOpenFileByMode(qpath, &f, FS_READ);
            if ( len > 0 && len < 1024 )
            {
                FS_Read(buffer, len, f);
                data_p = (char *)buffer;
                s0 = (char *)Com_Parse((const char **)&data_p);
                I_strncpyz(dest + 64, s0, 64);
                s0 = (char *)Com_Parse((const char **)&data_p);
                *((unsigned int *)dest + 32) = s0 && !I_stricmp(s0, "team");
            }
            else
            {
                if ( len > 0 )
                {
                    fmtMsg2 = va("maps/gametypes/%s.txt", src);
                    Com_PrintWarning(24, "WARNING: GameType description file %s is too big to load.\n", fmtMsg2);
                }
                else
                {
                    fmtMsg = va("maps/gametypes/%s.txt", src);
                    Com_PrintWarning(24, "WARNING: Could not load GameType description file %s for gametype %s\n", fmtMsg, src);
                }
                I_strncpyz(dest + 64, dest, 64);
                *((unsigned int *)dest + 32) = 0;
            }
            ++idx;
            if ( len > 0 )
                FS_FCloseFile(f);
            src += tempValue10 + 1;
        }
    }
    result = idx;
    g_scr_data.gametype.iNumGameTypes = idx;
    return result;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_ParseGameTypeList_FastFile)
void Scr_ParseGameTypeList_FastFile()
{
    const char *fmtMsg;
    const char *fmtMsg2;
    int tempValue2;
    char *fullname;
    RawFile *rawfile;
    parseInfo_t *pszFileName;
    const char *pBuffParse;
    const char *pToken;
    int iNumGameTypes;
    int iFileLength;
    RawFile *gametypesFile;
    const char *gametypesBuf;
    gameTypeScript_t *pGameType;

    memset((unsigned __int8 *)g_scr_data.gametype.list, 0, sizeof(g_scr_data.gametype.list));
    iNumGameTypes = 0;
    gametypesFile = DB_FindXAssetHeader(ASSET_TYPE_RAWFILE, (char*)"maps/gametypes/_gametypes.txt", 1, -1).rawfile;
    if ( gametypesFile )
    {
        gametypesBuf = gametypesFile->buffer;
        while ( 1 )
        {
            pszFileName = Com_Parse(&gametypesBuf);
            if ( !gametypesBuf )
                break;
            if ( iNumGameTypes == 32 )
            {
                Com_Printf(24, "Too many game type scripts found! Only loading the first %i\n", 31);
                break;
            }
            pGameType = &g_scr_data.gametype.list[iNumGameTypes];
            I_strncpyz(pGameType->pszScript, pszFileName->token, 64);
            I_strlwr(pGameType->pszScript);
            fullname = va("maps/gametypes/%s.txt", pszFileName->token);
            rawfile = DB_FindXAssetHeader(ASSET_TYPE_RAWFILE, fullname, 1, -1).rawfile;
            if ( rawfile )
                tempValue2 = strlen(rawfile->buffer);
            else
                tempValue2 = 0;
            iFileLength = tempValue2;
            if ( tempValue2 > 0 && iFileLength < 1024 )
            {
                if ( !rawfile
                    && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                                19406,
                                0,
                                "%s",
                                "rawfile") )
                {
                    __debugbreak();
                }
                pBuffParse = rawfile->buffer;
                pToken = (const char *)Com_Parse(&pBuffParse);
                I_strncpyz(pGameType->pszName, pToken, 64);
                pToken = (const char *)Com_Parse(&pBuffParse);
                pGameType->bTeamBased = pToken && !I_stricmp(pToken, "team");
            }
            else
            {
                if ( iFileLength > 0 )
                {
                    fmtMsg2 = va("maps/gametypes/%s.txt", pszFileName->token);
                    Com_PrintWarning(24, "WARNING: GameType description file %s is too big to load.\n", fmtMsg2);
                }
                else
                {
                    fmtMsg = va("maps/gametypes/%s.txt", pszFileName->token);
                    Com_PrintWarning(
                        24,
                        "WARNING: Could not load GameType description file %s for gametype %s\n",
                        fmtMsg,
                        pszFileName->token);
                }
                I_strncpyz(pGameType->pszName, pGameType->pszScript, 64);
                pGameType->bTeamBased = 0;
            }
            ++iNumGameTypes;
        }
    }
    g_scr_data.gametype.iNumGameTypes = iNumGameTypes;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetGameTypeNameForScript)
char *__cdecl Scr_GetGameTypeNameForScript(const char *pszGameTypeScript)
{
    int i;

    for ( i = 0; i < g_scr_data.gametype.iNumGameTypes; ++i )
    {
        if ( !I_stricmp(g_scr_data.gametype.list[i].pszScript, pszGameTypeScript) )
            return g_scr_data.gametype.list[i].pszName;
    }
    return 0;
}

#ifdef KISAK_SP
// Decomp: CoDSP_rdBlackOps.map.c — SP/coop/zombie scripts use gametypes not present in MP maps/gametypes/*.txt scan.
static bool Scr_SP_IsBuiltinGameType(const char *pszGameType)
{
    static const char *kBuiltin[] = {
        "cmp",
        "solo",
        "coop",
        "zombie",
        "survival",
        "dm",
        "tdm",
        0
    };
    int i;

    if ( !pszGameType || !*pszGameType )
        return false;
    for ( i = 0; kBuiltin[i]; ++i )
    {
        if ( !I_stricmp(pszGameType, kBuiltin[i]) )
            return true;
    }
    return false;
}
#endif

// Decomp: CoDSP_rdBlackOps.map.c (Scr_IsValidGameType)
bool __cdecl Scr_IsValidGameType(const char *pszGameType)
{
#ifdef KISAK_SP
    if ( Scr_SP_IsBuiltinGameType(pszGameType) )
        return true;
#endif
    return Scr_GetGameTypeNameForScript(pszGameType) != 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_LoadGameType)
void __cdecl Scr_LoadGameType()
{
    unsigned __int16 t;

    if ( !g_scr_data.gametype.main )
        return;
    t = Scr_ExecThread(SCRIPTINSTANCE_SERVER, g_scr_data.gametype.main, 0);
    Scr_FreeThread(t, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_StartupGameType)
void __cdecl Scr_StartupGameType()
{
    unsigned __int16 callback;

    if ( !g_scr_data.gametype.startupgametype )
        return;
    callback = Scr_ExecThread(SCRIPTINSTANCE_SERVER, g_scr_data.gametype.startupgametype, 0);
    Scr_FreeThread(callback, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c G_InitAnimScripts @ 8264EF24
void __cdecl G_InitAnimScripts()
{
    unsigned __int16 thread;

    if ( g_scr_data.init_mode_sp )
    {
        thread = Scr_ExecThread(SCRIPTINSTANCE_SERVER, g_scr_data.init_mode_sp, 0);
        Scr_FreeThread(thread, SCRIPTINSTANCE_SERVER);
    }

#ifdef KISAK_SP
    if ( zombiemode && zombiemode->current.enabled && g_scr_data.init_dog_animations )
    {
        thread = Scr_ExecThread(SCRIPTINSTANCE_SERVER, g_scr_data.init_dog_animations, 0);
        Scr_FreeThread(thread, SCRIPTINSTANCE_SERVER);
    }
#endif
}

#ifdef KISAK_SP
static void GScr_SetEntityFieldInt(gentity_s *self, const char *fieldName, int intValue)
{
    unsigned int entId;
    unsigned int varName;
    unsigned int var;
    VariableValue value;
    const char *varUsagePos;

    varUsagePos = gScrVarPub[SCRIPTINSTANCE_SERVER].varUsagePos;
    if ( !varUsagePos )
        gScrVarPub[SCRIPTINSTANCE_SERVER].varUsagePos = "<script entity variable>";
    entId = Scr_GetEntityId(SCRIPTINSTANCE_SERVER, self->s.number, 0, 0);
    varName = SL_GetCanonicalString(SCRIPTINSTANCE_SERVER, (char *)fieldName);
    value.type = VAR_INTEGER;
    value.u.intValue = intValue;
    var = FindVariable(SCRIPTINSTANCE_SERVER, entId, varName);
    if ( var )
        SetVariableValue(SCRIPTINSTANCE_SERVER, var, &value);
    else
        SetVariableEntityFieldValue(SCRIPTINSTANCE_SERVER, entId, varName, &value);
    gScrVarPub[SCRIPTINSTANCE_SERVER].varUsagePos = varUsagePos;
}

void GScr_InitPlayerDowns(gentity_s *self)
{
    char dvarName[32];
    int downs;

    Com_sprintf(dvarName, sizeof(dvarName), "player%idowns", self->s.number);
    if ( !Dvar_FindVar(dvarName) )
        _Dvar_RegisterInt(dvarName, 0, 0, 9999, 0, "player downs counter");
    downs = Dvar_GetInt(dvarName);
    GScr_SetEntityFieldInt(self, "downs", downs);
}
#endif

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PlayerConnect)
void __cdecl Scr_PlayerConnect(gentity_s *self)
{
    unsigned __int16 callback;

    if ( !g_scr_data.gametype.playerconnect )
        return;
    callback = Scr_ExecEntThread(self, g_scr_data.gametype.playerconnect, 0);
    Scr_FreeThread(callback, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PlayerDisconnect)
void __cdecl Scr_PlayerDisconnect(gentity_s *self)
{
    unsigned __int16 callback;

    if ( !g_scr_data.gametype.playerdisconnect )
        return;
    callback = Scr_ExecEntThread(self, g_scr_data.gametype.playerdisconnect, 0);
    Scr_FreeThread(callback, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PlayerDamage)
void __cdecl Scr_PlayerDamage(
                gentity_s *self,
                gentity_s *inflictor,
                gentity_s *attacker,
                int damage,
                int dflags,
                unsigned int meansOfDeath,
                unsigned int iWeapon,
                float *vPoint,
                float *vDir,
                hitLocation_t hitLoc,
                int timeOffset)
{
    unsigned __int16 HitLocationString;
    char *weaponName;
    unsigned __int16 callback;

    if ( !g_scr_data.gametype.playerdamage )
        return;
    Scr_AddInt(timeOffset, SCRIPTINSTANCE_SERVER);
    HitLocationString = G_GetHitLocationString(hitLoc);
    Scr_AddConstString(HitLocationString, SCRIPTINSTANCE_SERVER);
    GScr_AddVector(vDir);
    GScr_AddVector(vPoint);
    weaponName = (char *)BG_WeaponName(iWeapon);
    Scr_AddString(weaponName, SCRIPTINSTANCE_SERVER);
    if ( meansOfDeath <= 0x14 )
        Scr_AddConstString(*modNames[meansOfDeath], SCRIPTINSTANCE_SERVER);
    else
        Scr_AddString("badMOD", SCRIPTINSTANCE_SERVER);
    Scr_AddInt(dflags, SCRIPTINSTANCE_SERVER);
    Scr_AddInt(damage, SCRIPTINSTANCE_SERVER);
    GScr_AddEntity(attacker);
    GScr_AddEntity(inflictor);
    callback = Scr_ExecEntThread(self, g_scr_data.gametype.playerdamage, 0xAu);
    Scr_FreeThread(callback, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PlayerKilled)
void __cdecl Scr_PlayerKilled(
                gentity_s *self,
                gentity_s *inflictor,
                gentity_s *attacker,
                int damage,
                unsigned int meansOfDeath,
                unsigned int iWeapon,
                float *vDir,
                hitLocation_t hitLoc,
                int psTimeOffset,
                int deathAnimDuration)
{
    unsigned __int16 HitLocationString;
    char *weaponName;
    unsigned __int16 callback;

    if ( !g_scr_data.gametype.playerkilled )
        return;
    Scr_AddInt(deathAnimDuration, SCRIPTINSTANCE_SERVER);
    Scr_AddInt(psTimeOffset, SCRIPTINSTANCE_SERVER);
    HitLocationString = G_GetHitLocationString(hitLoc);
    Scr_AddConstString(HitLocationString, SCRIPTINSTANCE_SERVER);
    GScr_AddVector(vDir);
    weaponName = (char *)BG_WeaponName(iWeapon);
    Scr_AddString(weaponName, SCRIPTINSTANCE_SERVER);
    if ( meansOfDeath <= 0x14 )
        Scr_AddConstString(*modNames[meansOfDeath], SCRIPTINSTANCE_SERVER);
    else
        Scr_AddString("badMOD", SCRIPTINSTANCE_SERVER);
    Scr_AddInt(damage, SCRIPTINSTANCE_SERVER);
    GScr_AddEntity(attacker);
    GScr_AddEntity(inflictor);
    callback = Scr_ExecEntThread(self, g_scr_data.gametype.playerkilled, 9u);
    Scr_FreeThread(callback, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_ActorDamage)
void __cdecl Scr_ActorDamage(
                gentity_s *self,
                gentity_s *inflictor,
                gentity_s *attacker,
                int damage,
                int dflags,
                unsigned int meansOfDeath,
                unsigned int iWeapon,
                float *vPoint,
                float *vDir,
                hitLocation_t hitLoc,
                unsigned int modelIndex,
                int timeOffset)
{
    unsigned __int16 HitLocationString;
    char *weaponName;
    unsigned __int16 callback;

    if ( !g_scr_data.gametype.actordamage )
        return;
    // Decomp: CoDSP_rdBlackOps.map.c Scr_ActorDamage @826218D0 — 11 thread params incl. modelIndex.
    Scr_AddInt(timeOffset, SCRIPTINSTANCE_SERVER);
    Scr_AddInt(modelIndex, SCRIPTINSTANCE_SERVER);
    HitLocationString = G_GetHitLocationString(hitLoc);
    Scr_AddConstString(HitLocationString, SCRIPTINSTANCE_SERVER);
    GScr_AddVector(vDir);
    GScr_AddVector(vPoint);
    weaponName = (char *)BG_WeaponName(iWeapon);
    Scr_AddString(weaponName, SCRIPTINSTANCE_SERVER);
    if ( (int)meansOfDeath >= 0 )
        Scr_AddConstString(*modNames[meansOfDeath], SCRIPTINSTANCE_SERVER);
    else
        Scr_AddString("badMOD", SCRIPTINSTANCE_SERVER);
    Scr_AddInt(dflags, SCRIPTINSTANCE_SERVER);
    Scr_AddInt(damage, SCRIPTINSTANCE_SERVER);
    GScr_AddEntity(attacker);
    GScr_AddEntity(inflictor);
    callback = Scr_ExecEntThread(self, g_scr_data.gametype.actordamage, 0xBu);
    Scr_FreeThread(callback, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_ActorKilled)
void __cdecl Scr_ActorKilled(
                gentity_s *self,
                gentity_s *inflictor,
                gentity_s *attacker,
                int damage,
                unsigned int meansOfDeath,
                unsigned int iWeapon,
                float *vDir,
                hitLocation_t hitLoc,
                int psTimeOffset)
{
    unsigned __int16 HitLocationString;
    char *weaponName;
    unsigned __int16 callback;

    if ( !g_scr_data.gametype.actorkilled )
        return;
    Scr_AddInt(psTimeOffset, SCRIPTINSTANCE_SERVER);
    HitLocationString = G_GetHitLocationString(hitLoc);
    Scr_AddConstString(HitLocationString, SCRIPTINSTANCE_SERVER);
    GScr_AddVector(vDir);
    weaponName = (char *)BG_WeaponName(iWeapon);
    Scr_AddString(weaponName, SCRIPTINSTANCE_SERVER);
    if ( meansOfDeath <= 0x14 )
        Scr_AddConstString(*modNames[meansOfDeath], SCRIPTINSTANCE_SERVER);
    else
        Scr_AddString("badMOD", SCRIPTINSTANCE_SERVER);
    Scr_AddInt(damage, SCRIPTINSTANCE_SERVER);
    GScr_AddEntity(attacker);
    GScr_AddEntity(inflictor);
    callback = Scr_ExecEntThread(self, g_scr_data.gametype.actorkilled, 8u);
    Scr_FreeThread(callback, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_VehicleRadiusDamage)
void __cdecl Scr_VehicleRadiusDamage(
                gentity_s *self,
                gentity_s *inflictor,
                gentity_s *attacker,
                int damage,
                float fInnerDamage,
                float fOuterDamage,
                int dflags,
                unsigned int meansOfDeath,
                unsigned int iWeapon,
                float *vPoint,
                float fRadius,
                float coneAngleCos,
                float *coneDirection,
                int timeOffset)
{
    char *weaponName;
    unsigned __int16 callback;

    if ( !g_scr_data.gametype.vehicleradiusdamage )
        return;
    if ( iWeapon == -1 )
        iWeapon = 0;
    Scr_AddInt(timeOffset, SCRIPTINSTANCE_SERVER);
    GScr_AddVector(coneDirection);
    Scr_AddFloat(coneAngleCos, SCRIPTINSTANCE_SERVER);
    Scr_AddFloat(fRadius, SCRIPTINSTANCE_SERVER);
    GScr_AddVector(vPoint);
    weaponName = (char *)BG_WeaponName(iWeapon);
    Scr_AddString(weaponName, SCRIPTINSTANCE_SERVER);
    if ( meansOfDeath <= 0x14 )
        Scr_AddConstString(*modNames[meansOfDeath], SCRIPTINSTANCE_SERVER);
    else
        Scr_AddString("badMOD", SCRIPTINSTANCE_SERVER);
    Scr_AddInt(dflags, SCRIPTINSTANCE_SERVER);
    Scr_AddFloat(fOuterDamage, SCRIPTINSTANCE_SERVER);
    Scr_AddFloat(fInnerDamage, SCRIPTINSTANCE_SERVER);
    Scr_AddInt(damage, SCRIPTINSTANCE_SERVER);
    GScr_AddEntity(attacker);
    GScr_AddEntity(inflictor);
    callback = Scr_ExecEntThread(self, g_scr_data.gametype.vehicleradiusdamage, 0xDu);
    Scr_FreeThread(callback, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_VehicleDamage)
void __cdecl Scr_VehicleDamage(
                gentity_s *self,
                gentity_s *inflictor,
                gentity_s *attacker,
                int damage,
                int dflags,
                unsigned int meansOfDeath,
                unsigned int iWeapon,
                float *vPoint,
                float *vDir,
                hitLocation_t hitLoc,
                int timeOffset,
                unsigned int damageFromUnderneath,
                unsigned int modelIndex,
                unsigned int partName)
{
    unsigned __int16 HitLocationString;
    char *weaponName;
    unsigned __int16 callback;

    if ( !g_scr_data.gametype.vehicledamage )
        return;
    Scr_AddInt(partName, SCRIPTINSTANCE_SERVER);
    Scr_AddInt(modelIndex, SCRIPTINSTANCE_SERVER);
    Scr_AddBool(damageFromUnderneath, SCRIPTINSTANCE_SERVER);
    Scr_AddInt(timeOffset, SCRIPTINSTANCE_SERVER);
    HitLocationString = G_GetHitLocationString(hitLoc);
    Scr_AddConstString(HitLocationString, SCRIPTINSTANCE_SERVER);
    GScr_AddVector(vDir);
    GScr_AddVector(vPoint);
    weaponName = (char *)BG_WeaponName(iWeapon);
    Scr_AddString(weaponName, SCRIPTINSTANCE_SERVER);
    if ( meansOfDeath <= 0x14 )
        Scr_AddConstString(*modNames[meansOfDeath], SCRIPTINSTANCE_SERVER);
    else
        Scr_AddString("badMOD", SCRIPTINSTANCE_SERVER);
    Scr_AddInt(dflags, SCRIPTINSTANCE_SERVER);
    Scr_AddInt(damage, SCRIPTINSTANCE_SERVER);
    GScr_AddEntity(attacker);
    GScr_AddEntity(inflictor);
    callback = Scr_ExecEntThread(self, g_scr_data.gametype.vehicledamage, 0xDu);
    Scr_FreeThread(callback, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SaveRestored @ 82621698)
void __cdecl Scr_SaveRestored(gentity_s *self)
{
    unsigned __int16 callback;

    if ( !g_scr_data.saverestored )
        return;
    callback = Scr_ExecEntThread(self, g_scr_data.saverestored, 0);
    Scr_FreeThread(callback, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PlayerRevive @ 82621D08)
void __cdecl Scr_PlayerRevive(gentity_s *self)
{
    unsigned __int16 callback;

    if ( !g_scr_data.gametype.playerrevive )
        return;
    callback = Scr_ExecEntThread(self, g_scr_data.gametype.playerrevive, 0);
    Scr_FreeThread(callback, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_ActorShouldReact @ 82621A30)
void __cdecl Scr_ActorShouldReact(gentity_s *self)
{
    unsigned __int16 callback;

    if ( !g_scr_data.actorshouldreact )
        return;
    callback = Scr_ExecEntThread(self, g_scr_data.actorshouldreact, 0);
    Scr_FreeThread(callback, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_ClientDisconnectOnLoad @ 82621FF0)
void __cdecl Scr_ClientDisconnectOnLoad(const char *clientName)
{
    unsigned __int16 callback;

    if ( !g_scr_data.disconnectedduringload )
        return;
    Scr_AddString(clientName, SCRIPTINSTANCE_SERVER);
    callback = Scr_ExecThread(SCRIPTINSTANCE_SERVER, g_scr_data.disconnectedduringload, 1u);
    Scr_FreeThread(callback, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PlayerLastStand)
void __cdecl Scr_PlayerLastStand(
                gentity_s *self,
                gentity_s *inflictor,
                gentity_s *attacker,
                int damage,
                unsigned int meansOfDeath,
                unsigned int iWeapon,
                float *vDir,
                hitLocation_t hitLoc,
                int psTimeOffset)
{
    unsigned __int16 HitLocationString;
    char *weaponName;
    unsigned __int16 callback;

    if ( !g_scr_data.gametype.playerlaststand )
        return;
    Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    Scr_AddInt(psTimeOffset, SCRIPTINSTANCE_SERVER);
    HitLocationString = G_GetHitLocationString(hitLoc);
    Scr_AddConstString(HitLocationString, SCRIPTINSTANCE_SERVER);
    GScr_AddVector(vDir);
    weaponName = (char *)BG_WeaponName(iWeapon);
    Scr_AddString(weaponName, SCRIPTINSTANCE_SERVER);
    if ( meansOfDeath <= 0x14 )
        Scr_AddConstString(*modNames[meansOfDeath], SCRIPTINSTANCE_SERVER);
    else
        Scr_AddString("badMOD", SCRIPTINSTANCE_SERVER);
    Scr_AddInt(damage, SCRIPTINSTANCE_SERVER);
    GScr_AddEntity(attacker);
    GScr_AddEntity(inflictor);
    callback = Scr_ExecEntThread(self, g_scr_data.gametype.playerlaststand, 9u);
    Scr_FreeThread(callback, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_VoteCalled)
void __cdecl Scr_VoteCalled(gentity_s *self, char *command, char *param1, char *param2)
{
    Scr_AddString(param2, SCRIPTINSTANCE_SERVER);
    Scr_AddString(param1, SCRIPTINSTANCE_SERVER);
    Scr_AddString(command, SCRIPTINSTANCE_SERVER);
    Scr_Notify(self, scr_const.call_vote, 3u);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_PlayerVote)
void __cdecl Scr_PlayerVote(gentity_s *self, char *option)
{
    Scr_AddString(option, SCRIPTINSTANCE_SERVER);
    Scr_Notify(self, scr_const.vote, 1u);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_Shutdown)
void __cdecl GScr_Shutdown()
{
    if ( level.cachedTagMat.name )
        Scr_SetString(&level.cachedTagMat.name, 0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_Gdt_Update)
void __cdecl GScr_Gdt_Update(char *asset, char *keyValue)
{
    unsigned __int16 t;

    if ( !g_scr_data.levelnotify )
        return;
    Scr_AddString(keyValue, SCRIPTINSTANCE_SERVER);
    Scr_AddString(asset, SCRIPTINSTANCE_SERVER);
    Scr_AddString("gdt_update", SCRIPTINSTANCE_SERVER);
    t = Scr_ExecThread(SCRIPTINSTANCE_SERVER, g_scr_data.levelnotify, 3u);
    Scr_FreeThread(t, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GlassSmash)
void __cdecl Scr_GlassSmash(float *pos, float *dir)
{
    unsigned __int16 t;

    if (g_scr_data.glassSmash)
    {
        Scr_AddVector(dir, SCRIPTINSTANCE_SERVER);
        Scr_AddVector(pos, SCRIPTINSTANCE_SERVER);
        t = Scr_ExecThread(SCRIPTINSTANCE_SERVER, g_scr_data.glassSmash, 2u);
        Scr_FreeThread(t, SCRIPTINSTANCE_SERVER);
    }
}
