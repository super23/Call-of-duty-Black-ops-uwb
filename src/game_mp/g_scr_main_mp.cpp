#include "g_scr_main_mp.h"
#include <clientscript/cscr_vm.h>
#include "g_main_mp.h"
#include <cgame/cg_scr_main.h>
#include "g_spawn_mp.h"
#include <clientscript/cscr_stringlist.h>
#include <universal/com_memory.h>
#include <client/con_channels.h>
#include <server/sv_game.h>
#include <server_mp/sv_main_mp.h>
#include <client/cl_debugdata.h>
#include <cgame/cg_drawtools.h>
#include <game/g_debug.h>
#include <game/g_weapon.h>
#include <xanim/xmodel_utils.h>
#include <bgame/bg_weapons_ammo.h>
#include "g_utils_mp.h"
#include <clientscript/scr_const.h>
#include <server/sv_world.h>
#include <game/turret.h>
#include <game/g_scr_helicopter.h>
#include <qcommon/dobj_management.h>
#include <xanim/dobj_utils.h>
#include <game/actor_script_cmd.h>
#include "g_combat_mp.h"
#include <game/g_mover.h>
#include <server_mp/sv_init_mp.h>
#include <bgame/bg_perks.h>
#include <demo/demo_recording.h>
#include <qcommon/cm_world.h>
#include <bgame/bg_weapons_load_obj.h>
#include "g_client_script_cmd_mp.h"
#include <universal/surfaceflags.h>
#include <universal/com_math_anglevectors.h>
#include <universal/com_math.h>
#include <cstring>
#include <bgame/bg_misc.h>
#include <database/db_assetnames.h>
#include <glass/glass_server.h>
#include <game/g_missile.h>
#include "g_active_mp.h"
#include <game/actor_spawner.h>
#include <gfx_d3d/r_reflection_probe.h>
#include "g_cmds_mp.h"
#include <client_mp/g_client_mp.h>
#include <client_mp/sv_client_mp.h>
#include <server_mp/sv_main_pc_mp.h>
#include <universal/com_files.h>
#include <universal/q_parse.h>
#include <DW/MatchRecorder.h>
#include <client/splitscreen.h>
#include <qcommon/com_gamemodes.h>
#include <bgame/bg_unlockable_items.h>
#include <live/live_stats.h>
#include <live/live_contracts.h>
#include <ui_mp/ui_gametype_custom_mp.h>
#include <gfx_d3d/r_fog.h>
#include <game/bullet.h>
#include <cgame_mp/cg_ents_mp.h>
#include <cgame/cg_event.h>
#include "g_spawnsystem_mp.h"
#include <ui_mp/ui_gametype_variants_mp.h>
#include "pregame.h"
#include <live/live_storage_win.h>
#include <live/live_counter.h>
#include <game/g_client_fields.h>
#include <game/g_scr_mover.h>
#include "actor_mp.h"
#include <win32/win_shared.h>
#include <qcommon/cm_load.h>
#include <game/g_targets.h>
#include <stringed/stringed_hooks.h>
#include <gfx_d3d/r_dpvs.h>
#include <sound/snd_bank.h>
#include <gfx_d3d/r_primarylights.h>

scr_data_t g_scr_data;

static TurretInfo *Scr_GetTurretInfo(gentity_s *ent);

// Decomp: CoDOMPServer.c:228285
void assertCmd()
{
    if ( !Scr_GetInt(0, SCRIPTINSTANCE_CLIENT) )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "assert fail", 1);
}

// Decomp: CoDMPServer.c:228292
void assertexCmd()
{
    if ( !Scr_GetInt(0, SCRIPTINSTANCE_CLIENT) )
    {
        const char *message = Scr_GetString(1u, SCRIPTINSTANCE_CLIENT);
        Scr_Error(SCRIPTINSTANCE_CLIENT, va("assert fail: %s", message), 1);
    }
}

// Decomp: CoDMPServer.c:228306
void assertmsgCmd()
{
    const char *message = Scr_GetString(0, SCRIPTINSTANCE_CLIENT);
    Scr_Error(SCRIPTINSTANCE_CLIENT, va("assert fail: %s", message), 1);
}

// Decomp: CoDMPServer.c:228317
void print()
{
    const int paramCount = Scr_GetNumParam(SCRIPTINSTANCE_CLIENT);

    for ( int paramIndex = 0; paramIndex < paramCount; ++paramIndex )
    {
        const char *debugString = Scr_GetDebugString(paramIndex, SCRIPTINSTANCE_CLIENT);
        Com_Printf(cg_level.scriptPrintChannel, "%s", debugString);
    }
}

// Decomp: CoDMPServer.c:228336
void println()
{
    print();
    Com_Printf(cg_level.scriptPrintChannel, "\n");
}

// Decomp: CoDMPServer.c:407212
void GScr_IsCollectors()
{
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_ParamError(0, "Usage : IsCollectors [player]", SCRIPTINSTANCE_SERVER);
    if ( !Scr_GetEntity(0) )
        Scr_ParamError(0, "IsCollectors Error: param 1 is not an entity.", SCRIPTINSTANCE_SERVER);
    Scr_AddBool(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:453899
unsigned int __cdecl    GScr_AllocString(const char *s)
{
    return Scr_AllocString(s, 1, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:407234
void __cdecl Scr_LoadLevel()
{
    unsigned __int16 t;

    if ( g_scr_data.levelscript )
    {
        t = Scr_ExecThread(SCRIPTINSTANCE_SERVER, g_scr_data.levelscript, 0);
        Scr_FreeThread(t, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:407246
void __cdecl Scr_LoadPreGame()
{
    unsigned __int16 t;

    if (g_scr_data.pregamescript)
    {
        t = Scr_ExecThread(SCRIPTINSTANCE_SERVER, g_scr_data.pregamescript, 0);
        Scr_FreeThread(t, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:407258
void __cdecl GScr_LoadGameTypeScript()
{
    char filename[68];

    Com_sprintf(filename, 0x40u, "maps/mp/gametypes/%s", g_gametype->current.string);
    g_scr_data.gametype.main = GScr_LoadScriptAndLabel(SCRIPTINSTANCE_SERVER, filename, "main", 1);
    g_scr_data.gametype.startupgametype = GScr_LoadScriptAndLabel(
                                                                                    SCRIPTINSTANCE_SERVER,
                                                                                    "maps/mp/gametypes/_callbacksetup",
                                                                                    "CodeCallback_StartGameType",
                                                                                    1);
    g_scr_data.gametype.playerconnect = GScr_LoadScriptAndLabel(
                                                                                SCRIPTINSTANCE_SERVER,
                                                                                "maps/mp/gametypes/_callbacksetup",
                                                                                "CodeCallback_PlayerConnect",
                                                                                1);
    g_scr_data.gametype.playerdisconnect = GScr_LoadScriptAndLabel(
                                                                                     SCRIPTINSTANCE_SERVER,
                                                                                     "maps/mp/gametypes/_callbacksetup",
                                                                                     "CodeCallback_PlayerDisconnect",
                                                                                     1);
    g_scr_data.gametype.playerdamage = GScr_LoadScriptAndLabel(
                                                                             SCRIPTINSTANCE_SERVER,
                                                                             "maps/mp/gametypes/_callbacksetup",
                                                                             "CodeCallback_PlayerDamage",
                                                                             1);
    g_scr_data.gametype.playerkilled = GScr_LoadScriptAndLabel(
                                                                             SCRIPTINSTANCE_SERVER,
                                                                             "maps/mp/gametypes/_callbacksetup",
                                                                             "CodeCallback_PlayerKilled",
                                                                             1);
    g_scr_data.gametype.actordamage = GScr_LoadScriptAndLabel(
                                                                            SCRIPTINSTANCE_SERVER,
                                                                            "maps/mp/gametypes/_callbacksetup",
                                                                            "CodeCallback_ActorDamage",
                                                                            1);
    g_scr_data.gametype.actorkilled = GScr_LoadScriptAndLabel(
                                                                            SCRIPTINSTANCE_SERVER,
                                                                            "maps/mp/gametypes/_callbacksetup",
                                                                            "CodeCallback_ActorKilled",
                                                                            1);
    g_scr_data.gametype.vehicledamage = GScr_LoadScriptAndLabel(
                                                                                SCRIPTINSTANCE_SERVER,
                                                                                "maps/mp/gametypes/_callbacksetup",
                                                                                "CodeCallback_VehicleDamage",
                                                                                1);
    g_scr_data.gametype.vehicleradiusdamage = GScr_LoadScriptAndLabel(
                                                                                            SCRIPTINSTANCE_SERVER,
                                                                                            "maps/mp/gametypes/_callbacksetup",
                                                                                            "CodeCallback_VehicleRadiusDamage",
                                                                                            1);
    g_scr_data.gametype.playerlaststand = GScr_LoadScriptAndLabel(
                                                                                    SCRIPTINSTANCE_SERVER,
                                                                                    "maps/mp/gametypes/_callbacksetup",
                                                                                    "CodeCallback_PlayerLastStand",
                                                                                    1);

    g_scr_data.levelnotify = GScr_LoadScriptAndLabel(
                                        SCRIPTINSTANCE_SERVER,
                                        "maps/mp/gametypes/_callbacksetup",
                                        "CodeCallback_LevelNotify",
                                        1);
    g_scr_data.faceeventnotify = GScr_LoadScriptAndLabel(
                         SCRIPTINSTANCE_SERVER,
                         "maps/mp/gametypes/_callbacksetup",
                         "CodeCallback_FaceEventNotify",
                         0);
    g_scr_data.glassSmash = GScr_LoadScriptAndLabel(
                                        SCRIPTINSTANCE_SERVER,
                                        "maps/mp/gametypes/_callbacksetup",
                                        "CodeCallback_GlassSmash",
                                        0);
}

// Decomp: CoDMPServer.c:407288
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

// Decomp: CoDMPServer.c:407307
void __cdecl    GScr_LoadScripts(scriptInstance_t inst)
{
    Scr_BeginLoadScripts(inst, 1);

    g_scr_data.delete_ = GScr_LoadScriptAndLabel(inst, "codescripts/delete", "main", 1);
    g_scr_data.initstructs = GScr_LoadScriptAndLabel(inst, "codescripts/struct", "initstructs", 1);
    g_scr_data.createstruct = GScr_LoadScriptAndLabel(inst, "codescripts/struct", "createstruct", 1);
    g_scr_data.findstruct = GScr_LoadScriptAndLabel(inst, "codescripts/struct", "findstruct", 1);
    g_scr_data.destructible_callback = GScr_LoadScriptAndLabel(
        inst,
        "maps/mp/_destructible",
        "CodeCallback_DestructibleEvent",
        1);
    g_scr_data.updatespawnpoints = GScr_LoadScriptAndLabel(
        inst,
        "maps/mp/gametypes/_spawning",
        "CodeCallback_UpdateSpawnPoints",
        1);
    GScr_LoadDogAnimScripts(inst);

    if ( g_loadScripts && g_loadScripts->current.enabled )
    {
        GScr_SetScriptsForPathNodes(0);
        GScr_LoadPreGameScript();
        GScr_LoadGameTypeScript();
        GScr_LoadLevelScript();
        Scr_PostCompileScripts(inst);
    }

    // Retail registers entity/pathnode/radiant fields before map spawn even when g_loadScripts is off.
    // GScr_AddFieldsForRadiant allocates gScrVarPub[].fieldBuffer required by Scr_FindField during pathnode spawn.
    GScr_PostLoadScripts(inst);

    // Always finalize the compile session after field registration (retail CGScr_LoadScripts ~211427).
    // GScr_PostLoadScripts uses SL_GetCanonicalString even with g_loadScripts 0; if we skip this,
    // BG_PostLoadAnim's Scr_EndLoadAnimTrees shuts down compile strings while script_loading stays set,
    // and Scr_FreeScripts -> Scr_ArchiveCanonicalStrings crashes on stale indices at shutdown.
    Scr_EndLoadScripts(inst);
}

// Decomp: CoDMPServer.c:407320
void __cdecl    GScr_LoadDogAnimScripts(scriptInstance_t inst)
{
  GScr_LoadSingleAnimScript (inst, &g_scr_data.dogAnim.combat, "dog_combat");
  GScr_LoadSingleAnimScript (inst, &g_scr_data.dogAnim.death, "dog_death");
  GScr_LoadSingleAnimScript (inst, &g_scr_data.dogAnim.init, "dog_init");
  GScr_LoadSingleAnimScript (inst, &g_scr_data.dogAnim.pain, "dog_pain");
  GScr_LoadSingleAnimScript (inst, &g_scr_data.dogAnim.move, "dog_move");
  GScr_LoadSingleAnimScript (inst, &g_scr_data.dogAnim.stop, "dog_stop");
  GScr_LoadSingleAnimScript (inst, &g_scr_data.dogAnim.flashed, "dog_flashed");
  GScr_LoadSingleAnimScript (inst, &g_scr_data.dogAnim.jump, "dog_jump");
  GScr_LoadSingleAnimScript (inst, &g_scr_data.dogAnim.turn, "dog_turn");
  g_animScriptTable[0] = &g_scr_data.dogAnim;
}

// Decomp: CoDMPServer.c:407320
void __cdecl    GScr_LoadSingleAnimScript(scriptInstance_t inst, scr_animscript_t *pAnim, const char *name)
{
    char filename[64];

    iassert(pAnim);
    iassert(name);

    Com_sprintf(filename, sizeof(filename), "maps/mp/animscripts/%s", name);

    pAnim->name = GScr_AllocString(name);
    pAnim->func = GScr_LoadScriptAndLabel(inst, filename, "main", 1);
}

// Decomp: CoDMPServer.c:407358
void GScr_SetScriptsForPathNodes(ScriptFunctions *functions)
{
    if ( !G_ExitAfterToolComplete() )
        Path_CallFunctionForNodes(SCRIPTINSTANCE_SERVER, (PathNodeCallback_t)GScr_SetScriptsForPathNode, functions);
}

// Decomp: CoDMPServer.c:407365
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
                    Com_sprintf(filename, 0x40u, "maps/mp/animscripts/traverse/%s", animscript);
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

// Decomp: CoDMPServer.c:407442
unsigned __int8 *__cdecl GScr_AnimscriptAlloc(unsigned int size)
{
    return Hunk_AllocLow(size, "GScr_AnimscriptAlloc", 5);
}

// Decomp: CoDMPServer.c:407448
int GScr_LoadLevelScript()
{
    int result;
    char filename[64];
    const dvar_s *mapname;

    mapname = _Dvar_RegisterString("mapname", (char *)"", 0x44u, "The current map name");
    Com_sprintf(filename, 0x40u, "maps/mp/%s", mapname->current.string);
    result = GScr_LoadScriptAndLabel(SCRIPTINSTANCE_SERVER, filename, "main", 0);
    if ( !result )
    {
        Com_sprintf(filename, 0x40u, "maps/%s", mapname->current.string);
        result = GScr_LoadScriptAndLabel(SCRIPTINSTANCE_SERVER, filename, "main", 0);
    }
    g_scr_data.levelscript = result;
    return result;
}

// Decomp: CoDMPServer.c:407462
int GScr_LoadPreGameScript()
{
    int result;
    char filename[68];

    Com_sprintf(filename, 0x40u, "maps/mp/gametypes/_pregame");
    result = GScr_LoadScriptAndLabel(SCRIPTINSTANCE_SERVER, filename, "main", 0);
    g_scr_data.pregamescript = result;
    return result;
}

// Decomp: CoDMPServer.c:407474
void __cdecl GScr_PostLoadScripts(scriptInstance_t inst)
{
    signed int classnum;

    for ( classnum = 0; classnum < 5; ++classnum )
        Scr_SetClassMap(inst, classnum);
    GScr_AddFieldsForEntity();
    GScr_AddFieldsForHudElems();
    GScr_AddFieldsForPathnode();
    GScr_AddFieldsForVehicleNode();
    GScr_AddFieldsForRadiant();
}

// Decomp: CoDMPServer.c:407488
void __cdecl GScr_FreeScripts(scriptInstance_t inst)
{
    signed int classnum;

    for ( classnum = 0; classnum < 5; ++classnum )
        Scr_RemoveClassMap(inst, classnum);
}

// Decomp: CoDMPServer.c:407497
void __cdecl ScrCmd_GetClanId(scr_entref_t entref)
{
    Scr_AddString("0", SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:407503
void __cdecl ScrCmd_GetClanName(scr_entref_t entref)
{
    Scr_AddString((char *)"", SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:407509
void GScr_CreatePrintChannel()
{
    char *name;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("illegal call to createprintchannel()", 0);
    name = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( !Con_OpenChannel(name, 1) )
        Scr_Error("Unable to create new channel.    Maximum number of channels exeeded.", 0);
}

// Decomp: CoDMPServer.c:407521
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

// Decomp: CoDMPServer.c:407605
void __cdecl Scr_LocalizationError(unsigned int iParm, const char *pszErrorMessage)
{
    Scr_ParamError(iParm, pszErrorMessage, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:407611
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

// Decomp: CoDMPServer.c:407724
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

// Decomp: CoDMPServer.c:407768
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

// Decomp: CoDMPServer.c:407781
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

// Decomp: CoDMPServer.c:407811
void iprintln()
{
    const char *fmtMsg;

    fmtMsg = va("%c", 102);
    Scr_MakeGameMessage(-1, fmtMsg);
}

// Decomp: CoDMPServer.c:407820
void iprintlnbold()
{
    const char *fmtMsg;

    fmtMsg = va("%c", 103);
    Scr_MakeGameMessage(-1, fmtMsg);
}

// Decomp: CoDMPServer.c:407829
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

// Decomp: CoDMPServer.c:407877
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

// Decomp: CoDMPServer.c:407925
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

// Decomp: CoDMPServer.c:408006
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

// Decomp: CoDMPServer.c:408042
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

// Decomp: CoDMPServer.c:408096
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

// Decomp: CoDMPServer.c:408154
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

// Decomp: CoDMPServer.c:408255
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

// Decomp: CoDMPServer.c:408342
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

// Decomp: CoDMPServer.c:408385
void GScr_AddDebugCommand()
{
    char *String;

    String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    Cbuf_AddText(0, String);
}

// Decomp: CoDMPServer.c:408394
void GScr_IsMP()
{
    Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:408394
void GScr_IsFloat()
{
    int Type;

    Type = Scr_GetType(0, SCRIPTINSTANCE_SERVER);
    Scr_AddInt(Type == 5, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:408403
void GScr_IsInt()
{
    int Type;

    Type = Scr_GetType(0, SCRIPTINSTANCE_SERVER);
    Scr_AddInt(Type == 6, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:408412
void GScr_IsVec()
{
    int Type;

    Type = Scr_GetType(0, SCRIPTINSTANCE_SERVER);
    Scr_AddInt(Type == 4, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:408421
void GScr_IsString()
{
    int Type;

    Type = Scr_GetType(0, SCRIPTINSTANCE_SERVER);
    Scr_AddInt(Type == 2, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:408430
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

// Decomp: CoDMPServer.c:408470
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

// Decomp: CoDMPServer.c:408493
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

// Decomp: CoDMPServer.c:408513
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

// Decomp: CoDMPServer.c:408535
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

// Decomp: CoDMPServer.c:408557
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

// Decomp: CoDMPServer.c:408577
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

// Decomp: CoDMPServer.c:408597
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

// Decomp: CoDMPServer.c:408617
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

// Decomp: CoDMPServer.c:408665
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

// Decomp: CoDMPServer.c:408683
void GScr_GetTime()
{
    Scr_AddInt(level.time, SCRIPTINSTANCE_SERVER);
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

// Decomp: CoDMPServer.c:408690
void GScr_GetAttachmentIndex()
{
    eAttachment attachmentIndex;
    char *attachmentName;

    attachmentName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    attachmentIndex = BG_GetAttachmentIndex(attachmentName);
    Scr_AddInt(attachmentIndex, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:408701
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

// Decomp: CoDMPServer.c:408714
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

// Decomp: CoDMPServer.c:408746
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

// Decomp: CoDMPServer.c:408792
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

// Decomp: CoDMPServer.c:408839
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

// Decomp: CoDMPServer.c:408883
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

// Decomp: CoDMPServer.c:408911
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

// Decomp: CoDMPServer.c:408926
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

// Decomp: CoDMPServer.c:408943
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

// Decomp: CoDMPServer.c:408957
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

// Decomp: CoDMPServer.c:408974
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

// Decomp: CoDMPServer.c:409012
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

// Decomp: CoDMPServer.c:409050
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

// Decomp: CoDMPServer.c:409102
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
    // Decomp: CoDMPServer.c:409159-409163 — InitScriptMover forces ET_SCRIPTMOVER (426001);
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

// Decomp: CoDMPServer.c:409176
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

// Decomp: CoDMPServer.c:409225
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

// Decomp: CoDMPServer.c:409241
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

// Decomp: CoDMPServer.c:409263
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

// Decomp: CoDMPServer.c:409296
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

// Decomp: CoDMPServer.c:409318
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

// Decomp: CoDMPServer.c:409345
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
        if ( !strcmp(gScrAnimPub[0].xanim_lookup[1][treeIndex].anims->debugName, treeName) )
        {
            animTree = gScrAnimPub[0].xanim_lookup[1][treeIndex].anims;
            break;
        }
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

// Decomp: CoDMPServer.c:409400
void GScr_PrecacheTurret()
{
    char *turretInfo;

    if ( !level.initializing )
        Scr_Error("precacheTurret must be called before any wait statements in the level script\n", 0);
    turretInfo = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    G_GetWeaponIndexForName(turretInfo);
}

// Decomp: CoDMPServer.c:409411
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

// Decomp: CoDMPServer.c:409445
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

// Decomp: CoDMPServer.c:409479
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

// Decomp: CoDMPServer.c:409545
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

// Decomp: CoDMPServer.c:409589
void __cdecl ScrCmd_detachAll(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    G_EntDetachAll(ent);
}

// Decomp: CoDMPServer.c:409601
void __cdecl ScrCmd_GetAttachSize(scr_entref_t entref)
{
    gentity_s *ent;
    int i;

    ent = GetEntity(entref);
    for ( i = 0; i < 19 && ent->attachModelNames[i]; ++i )
        ;
    Scr_AddInt(i, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:409616
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

// Decomp: CoDMPServer.c:409634
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

// Decomp: CoDMPServer.c:409661
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

// Decomp: CoDMPServer.c:409677
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

// Decomp: CoDMPServer.c:409701
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

// Decomp: CoDMPServer.c:409722
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

// Decomp: CoDMPServer.c:409772
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

// Decomp: CoDMPServer.c:409822
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

// Decomp: CoDMPServer.c:409845
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

// Decomp: CoDMPServer.c:409870
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

// Decomp: CoDMPServer.c:409906
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

// Decomp: CoDMPServer.c:409916
void __cdecl ScrCmd_SetForceNoCull(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    ent->s.lerp.eFlags2 |= 0x4000000u;
}

// Decomp: CoDMPServer.c:409928
void __cdecl ScrCmd_SetInvisibleToAll(scr_entref_t entref)
{
    GetEntity(entref)->r.clientMask[0] = -1;
}

// Decomp: CoDMPServer.c:409938
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

// Decomp: CoDMPServer.c:409992
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

// Decomp: CoDMPServer.c:410019
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

// Decomp: CoDMPServer.c:410105
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

// Decomp: CoDMPServer.c:410185
void __cdecl ScrCmd_Unlink(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->client && (ent->client->ps.eFlags & 0x4000) != 0 )
        VEH_UnlinkPlayer(ent, 0, (char*)"ScrCmd_Unlink");
    else
        G_EntUnlink(ent);
}

// Decomp: CoDMPServer.c:410200
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

// Decomp: CoDMPServer.c:410233
void __cdecl ScrCmd_GetOrigin(scr_entref_t entref)
{
    float origin[3];
    gentity_s *ent;

    ent = GetEntity(entref);
    origin[0] = ent->r.currentOrigin[0];
    origin[1] = ent->r.currentOrigin[1];
    origin[2] = ent->r.currentOrigin[2];
    Scr_AddVector(origin, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:410249
void __cdecl ScrCmd_GetAngles(scr_entref_t entref)
{
    gentity_s *pSelf;

    pSelf = GetEntity(entref);
    Scr_AddVector(pSelf->r.currentAngles, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:410261
void __cdecl ScrCmd_GetMins(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_AddVector(ent->r.mins, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:410273
void __cdecl ScrCmd_GetMaxs(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_AddVector(ent->r.maxs, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:410285
void __cdecl ScrCmd_GetAbsMins(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_AddVector(ent->r.absmin, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:410297
void __cdecl ScrCmd_GetAbsMaxs(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_AddVector(ent->r.absmax, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:410309
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

// Decomp: CoDMPServer.c:410329
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

// Decomp: CoDMPServer.c:410351
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

// Decomp: CoDMPServer.c:410368
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

// Decomp: CoDMPServer.c:410388
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

// Decomp: CoDMPServer.c:410404
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

// Decomp: CoDMPServer.c:410527
void ScrCmd_SoundExists()
{
    snd_alias_list_t *Alias;
    char *soundName;

    soundName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    Alias = SND_FindAlias(soundName);
    Scr_AddBool(Alias != 0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:410538
void __cdecl ScrCmd_PlaySound(scr_entref_t entref)
{
    int NumParam;
    const char *fmtMsg;
    char *String;
    gentity_s *Entity;
    unsigned int AliasId;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
    {
        NumParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
        fmtMsg = va("playsound has %d parameters.    There should be exactly one.", NumParam);
        Scr_Error(fmtMsg, 0);
    }
    String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    AliasId = SND_FindAliasId(String);
    Entity = GetEntity(entref);
    G_PlaySoundAlias(Entity, AliasId, 0, 0);
}

// Decomp: CoDMPServer.c:410562
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

// Decomp: CoDMPServer.c:410589
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

// Decomp: CoDMPServer.c:410667
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

// Decomp: CoDMPServer.c:410732
gentity_s *__cdecl StartScriptPlayBattleChatterOnEnt(scr_entref_t entref)
{
    gentity_s *ent = GetEntity(entref);
    gentity_s *soundEnt = G_TempEntity(ent->r.currentOrigin, EV_SOUND_BATTLECHAT_ALIAS);
    soundEnt->r.svFlags |= 8u;
    soundEnt->s.loopSoundId = SND_FindAliasId(Scr_GetString(0, SCRIPTINSTANCE_SERVER));
    soundEnt->s.un3.item = SND_FindAliasId(Scr_GetString(1u, SCRIPTINSTANCE_SERVER));
    return soundEnt;
}

// Decomp: CoDMPServer.c:410755
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

// Decomp: CoDMPServer.c:410785
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

// Decomp: CoDMPServer.c:410806
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

// Decomp: CoDMPServer.c:410838
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

// Decomp: CoDMPServer.c:410870
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

// Decomp: CoDMPServer.c:410887
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

// Decomp: CoDMPServer.c:410907
void __cdecl ScrCmd_SetModel(scr_entref_t entref)
{
    char *modelName;
    gentity_s *pEnt;

    pEnt = GetEntity(entref);
    modelName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    SetModelInternal(pEnt, modelName);
}

// Decomp: CoDMPServer.c:410921
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

// Decomp: CoDMPServer.c:410955
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

// Decomp: CoDMPServer.c:410977
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

// Decomp: CoDMPServer.c:411013
void __cdecl ScrCmd_DoDamage(scr_entref_t entref)
{
    PROF_SCOPED("ScrCmd_DoDamage");

    const int numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParam < 2 || numParam > 8 )
    {
        Scr_Error("Usage: doDamage( <health>, <source position>, <attacker>, <inflictor>, <mod> )\n", 0);
        return;
    }

    const float damage = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    float source[3];
    Scr_GetVector(1u, source, SCRIPTINSTANCE_SERVER);

    gentity_s *attacker = 0;
    gentity_s *inflictor = 0;
    hitLocation_t hitLoc = HITLOC_HEAD;
    meansOfDeath_t mod = MOD_UNKNOWN;
    int dflags = 0;
    int weapon = -1;

    if ( numParam >= 3 )
        attacker = Scr_GetEntity(2u);
    if ( numParam >= 4 )
        inflictor = Scr_GetEntity(3u);
    if ( numParam >= 5 && !Scr_GetInt(4u, SCRIPTINSTANCE_SERVER) )
        hitLoc = HITLOC_NONE;
    if ( numParam >= 6 )
        mod = (meansOfDeath_t)G_MeansOfDeathFromScriptParam(5u);
    if ( numParam >= 7 )
        dflags = Scr_GetInt(6u, SCRIPTINSTANCE_SERVER);
    if ( numParam >= 8 )
        weapon = G_GetWeaponIndexForName(Scr_GetString(7u, SCRIPTINSTANCE_SERVER));

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

    if ( !ent->scr_vehicle && ent->destructible )
    {
        const meansOfDeath_t destructibleMod = mod ? mod : MOD_EXPLOSIVE;
        DestructibleRadiusDamage(ent, source, damage, 10.0, 400.0, destructibleMod, attacker);
        dflags |= 0x10u;
    }
    G_Damage(ent, inflictor, attacker, dir, source, (int)damage, dflags, mod, weapon, hitLoc, 0, 0, 0);
}

// Decomp: CoDMPServer.c:411189
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

// Decomp: CoDMPServer.c:411231
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

// Decomp: CoDMPServer.c:411283
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

// Decomp: CoDMPServer.c:411316
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

// Decomp: CoDMPServer.c:411362
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

// Decomp: CoDMPServer.c:411377
void __cdecl ScrCmd_Show(scr_entref_t entref)
{
    gentity_s *pEnt;

    pEnt = GetEntity(entref);
    pEnt->s.lerp.eFlags &= ~0x20u;
    if ( pEnt->client )
        pEnt->client->ps.eFlags &= ~0x20u;
    pEnt->r.clientMask[0] = 0;
}

// Decomp: CoDMPServer.c:411392
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

// Decomp: CoDMPServer.c:411410
void __cdecl ScrCmd_Ghost(scr_entref_t entref)
{
    gentity_s *pEnt;

    pEnt = GetEntity(entref);
    pEnt->s.lerp.eFlags |= 0x20u;
    if ( pEnt->client )
        pEnt->client->ps.eFlags |= 0x20u;
}

// Decomp: CoDMPServer.c:411424
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

// Decomp: CoDMPServer.c:411446
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

// Decomp: CoDMPServer.c:411463
void __cdecl GScr_StartFiring(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_GetTurretInfo(ent)->flags |= 4u;
}

// Decomp: CoDMPServer.c:411496
void __cdecl GScr_StopFiring(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_GetTurretInfo(ent)->flags &= ~4u;
}

// Decomp: CoDMPServer.c:411529
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

// Decomp: CoDMPServer.c:411553
void __cdecl GScr_StopShootTurret(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_GetTurretInfo(ent);
    ent->s.lerp.eFlags &= ~0x40u;
}

// Decomp: CoDMPServer.c:411573
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

// Decomp: CoDMPServer.c:411630
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

// Decomp: CoDMPServer.c:411662
void __cdecl GScr_SetTargetEntity(scr_entref_t entref)
{
    gentity_s *ent;
    gentity_s *targetEnt;

    ent = GetEntity(entref);
    targetEnt = Scr_GetEntity(0);
    Scr_GetTurretInfo(ent)->manualTarget.setEnt(targetEnt);
}

// Decomp: CoDMPServer.c:411695
void __cdecl GScr_SetAiSpread(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_GetTurretInfo(ent)->aiSpread = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:411726
void __cdecl GScr_SetPlayerSpread(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_GetTurretInfo(ent)->playerSpread = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:411757
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

// Decomp: CoDMPServer.c:411820
void __cdecl GScr_SetSuppressionTime(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_GetTurretInfo(ent)->suppressTime = (int)(Scr_GetFloat(0, SCRIPTINSTANCE_SERVER) * 1000.0);
}

// Decomp: CoDMPServer.c:411852
void __cdecl GScr_ClearTargetEntity(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_GetTurretInfo(ent)->manualTarget.setEnt(0);
}

// Decomp: CoDMPServer.c:411883
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

// Decomp: CoDMPServer.c:411934
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

// Decomp: CoDMPServer.c:411968
void __cdecl GScr_MakeTurretUsable(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_GetTurretInfo(ent)->flags |= 0x1000u;
}

// Decomp: CoDMPServer.c:411999
void __cdecl GScr_MakeTurretUnusable(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    Scr_GetTurretInfo(ent)->flags &= ~0x1000u;
}

// Decomp: CoDMPServer.c:412030
void __cdecl GScr_SetTurretAccuracy(scr_entref_t entref)
{
    Com_PrintWarning(24, "WARNING: Turret Accuracy no longer has any effect\n");
}

// Decomp: CoDMPServer.c:412036
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

// Decomp: CoDMPServer.c:412075
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

// Decomp: CoDMPServer.c:412097
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

// Decomp: CoDMPServer.c:412119
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

// Decomp: CoDMPServer.c:412149
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

// Decomp: CoDMPServer.c:412170
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

// Decomp: CoDMPServer.c:412228
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

// Decomp: CoDMPServer.c:412253
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

// Decomp: CoDMPServer.c:412298
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

// Decomp: CoDMPServer.c:412348
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

// Decomp: CoDMPServer.c:412367
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

// Decomp: CoDMPServer.c:412424
void __cdecl GScr_UseTriggerRequireLookAt(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->classname != scr_const.trigger_use && ent->classname != scr_const.trigger_radius_use )
        Scr_Error("The UseTriggerRequireLookAt command only works on trigger_use and trigger_radius_use entities.\n", 0);
    ent->trigger.requireLookAt = 1;
}

// Decomp: CoDMPServer.c:412438
void __cdecl GScr_IsMartyrdomGrenade(scr_entref_t entref)
{
    gentity_s *pEnt;

    pEnt = GetEntity(entref);
    Scr_AddBool((pEnt->flags & 0x8000) != 0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:412450
void __cdecl GScr_GetEntityNumber(scr_entref_t entref)
{
    gentity_s *pEnt;

    pEnt = GetEntity(entref);
    Scr_AddInt(pEnt->s.number, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:412462
void __cdecl GScr_EnableGrenadeTouchDamage(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->classname != scr_const.trigger_damage )
        Scr_Error("Currently on supported on damage triggers", 0);
    ent->flags |= 0x4000u;
}

// Decomp: CoDMPServer.c:412476
void __cdecl GScr_DisableGrenadeTouchDamage(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->classname != scr_const.trigger_damage )
        Scr_Error("Currently on supported on damage triggers", 0);
    ent->flags &= ~0x4000u;
}

// Decomp: CoDMPServer.c:412490
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

// Decomp: CoDMPServer.c:412525
void __cdecl GScr_EnableAimAssist(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->r.bmodel )
        Scr_Error("Currently only supported on entities with brush models", 0);
    ent->s.lerp.eFlags |= 0x800u;
}

// Decomp: CoDMPServer.c:412539
void __cdecl GScr_DisableAimAssist(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->r.bmodel )
        Scr_Error("Currently only supported on entities with brush models", 0);
    ent->s.lerp.eFlags &= ~0x800u;
}

// Decomp: CoDMPServer.c:412552
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

// Decomp: CoDMPServer.c:412567
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
    else
    {
        if ( stateString != scr_const.active )
        {
            *piStateIndex = OBJST_EMPTY;
            return 0;
        }
        *piStateIndex = OBJST_ACTIVE;
    }
    return 1;
}

// Decomp: CoDMPServer.c:412594
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

// Decomp: CoDMPServer.c:412609
void Scr_Objective_Add()
{
    const char *fmtMsg;
    char *slStr;
    const char *fmtMsg2;
    objectiveState_t state;
    objective_t *obj;
    int numParam;
    unsigned __int16 stateName;
    int objNum;

    numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( numParam < 2 )
        Scr_Error(
            "objective_add needs at least the first two parameters out of its parameter list of: index state [string] [position]\n",
            0);
    objNum = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)objNum >= 0x20 )
    {
        fmtMsg = va("index %i is an illegal objective index. Valid indexes are 0 to %i\n", objNum, 31);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    obj = &level.objectives[objNum];
    level.objectivesClientMask[objNum][0] = 0;
    level.objectivesClientMask[objNum][1] = 0;
    ClearObjective_OnEntity(obj);
    stateName = (unsigned __int16)Scr_GetConstString(1u, SCRIPTINSTANCE_SERVER);
    if ( !ObjectiveStateIndexFromString(&state, stateName) )
    {
        slStr = SL_ConvertToString(stateName, SCRIPTINSTANCE_SERVER);
        fmtMsg2 = va("Illegal objective state \"%s\". Valid states are \"empty\", \"invisible\", \"current\", \"active\"\n", slStr);
        Scr_ParamError(1u, fmtMsg2, SCRIPTINSTANCE_SERVER);
    }
    obj->state = state;
    if ( numParam >= 3 )
    {
        Scr_GetVector(2u, obj->origin, SCRIPTINSTANCE_SERVER);
        obj->origin[0] = (float)(int)obj->origin[0];
        obj->origin[1] = (float)(int)obj->origin[1];
        obj->origin[2] = (float)(int)obj->origin[2];
        obj->entNum = 1023;
        if ( numParam >= 4 )
            SetObjectiveIcon(obj, 3u);
    }
    obj->teamNum = 0;
}

// Decomp: CoDMPServer.c:412663
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

// Decomp: CoDMPServer.c:412677
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

// Decomp: CoDMPServer.c:412707
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

// Decomp: CoDMPServer.c:412728
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

// Decomp: CoDMPServer.c:412763
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

// Decomp: CoDMPServer.c:412778
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

// Decomp: CoDMPServer.c:412802
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

// Decomp: CoDMPServer.c:412826
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

// Decomp: CoDMPServer.c:412867
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

// Decomp: CoDMPServer.c:412900
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

// Decomp: CoDMPServer.c:412933
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

// Decomp: CoDMPServer.c:412952
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

// Decomp: CoDMPServer.c:412971
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

// Decomp: CoDMPServer.c:413016
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

// Decomp: CoDMPServer.c:413040
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
        slStr = SL_ConvertToString(team, SCRIPTINSTANCE_SERVER);
        fmtMsg2 = va("Illegal team string '%s'. Must be allies, axis, or none.", slStr);
        Scr_ParamError(1u, fmtMsg2, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:413078
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

// Decomp: CoDMPServer.c:413122
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

// Decomp: CoDMPServer.c:413217
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

// Decomp: CoDMPServer.c:413241
void GScr_WorldEntNumber()
{
    Scr_AddInt(1022, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:413248
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

// Decomp: CoDMPServer.c:413291
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

// Decomp: CoDMPServer.c:413327
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

// Decomp: CoDMPServer.c:413350
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

// Decomp: CoDMPServer.c:413378
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

// Decomp: CoDMPServer.c:413400
void __cdecl Scr_UpdateSpawnPoints()
{
    unsigned __int16 callback;

    if ( !g_scr_data.updatespawnpoints )
        return;
    callback = Scr_ExecThread(SCRIPTINSTANCE_SERVER, g_scr_data.updatespawnpoints, 0);
    Scr_FreeThread(callback, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:413409
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

// Decomp: CoDMPServer.c:413426
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

// Decomp: CoDMPServer.c:413455
void GScr_BoundsWouldTelefrag()
{
    float mins[3];
    float maxs[3];

    Scr_GetVector(0, mins, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, maxs, SCRIPTINSTANCE_SERVER);
    Scr_BoundsWouldTelefrag(mins, maxs);
}

// Decomp: CoDMPServer.c:413466
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

// Decomp: CoDMPServer.c:413487
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

// Decomp: CoDMPServer.c:413499
void GScr_getStartTime()
{
    Scr_AddInt(level.startTime, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:413506
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

// Decomp: CoDMPServer.c:413539
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

// Decomp: CoDMPServer.c:413557
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

// Decomp: CoDMPServer.c:413590
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

// Decomp: CoDMPServer.c:413610
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

// Decomp: CoDMPServer.c:413643
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

// Decomp: CoDMPServer.c:413663
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

// Decomp: CoDMPServer.c:413677
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

// Decomp: CoDMPServer.c:413691
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

// Decomp: CoDMPServer.c:413705
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

// Decomp: CoDMPServer.c:413721
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

// Decomp: CoDMPServer.c:413743
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

// Decomp: CoDMPServer.c:413756
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

// Decomp: CoDMPServer.c:413772
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

// Decomp: CoDMPServer.c:413786
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

// Decomp: CoDMPServer.c:413800
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

// Decomp: CoDMPServer.c:413823
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

// Decomp: CoDMPServer.c:413841
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

// Decomp: CoDMPServer.c:413859
void GScr_GetWeaponIndexFromName()
{
    char *weaponName;
    int weaponIndex;

    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    Scr_AddInt(weaponIndex, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:413870
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

// Decomp: CoDMPServer.c:413883
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

// Decomp: CoDMPServer.c:413896
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

// Decomp: CoDMPServer.c:413909
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

// Decomp: CoDMPServer.c:413922
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

// Decomp: CoDMPServer.c:413952
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

// Decomp: CoDMPServer.c:413980
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

// Decomp: CoDMPServer.c:414008
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

// Decomp: CoDMPServer.c:414029
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

// Decomp: CoDMPServer.c:414051
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

// Decomp: CoDMPServer.c:414071
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

// Decomp: CoDMPServer.c:414091
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

// Decomp: CoDMPServer.c:414124
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

// Decomp: CoDMPServer.c:414155
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

// Decomp: CoDMPServer.c:414246
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

// Decomp: CoDMPServer.c:414279
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

// Decomp: CoDMPServer.c:414310
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

// Decomp: CoDMPServer.c:414417
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

// Decomp: CoDMPServer.c:414438
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

// Decomp: CoDMPServer.c:414457
void Scr_RandomFloat()
{
    float max;
    float fMax;

    fMax = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    max = G_flrand(0.0, fMax);
    Scr_AddFloat(max, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:414468
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

// Decomp: CoDMPServer.c:414486
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

// Decomp: CoDMPServer.c:414504
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

// Decomp: CoDMPServer.c:414519
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

// Decomp: CoDMPServer.c:414534
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

// Decomp: CoDMPServer.c:414549
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

// Decomp: CoDMPServer.c:414564
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

// Decomp: CoDMPServer.c:414586
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

// Decomp: CoDMPServer.c:414608
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

// Decomp: CoDMPServer.c:414623
void GScr_abs()
{
    float Float;

    Float = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    Scr_AddFloat(fabs(Float), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:414633
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

// Decomp: CoDMPServer.c:414649
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

// Decomp: CoDMPServer.c:414665
void GScr_floor()
{
    float tempValue0;
    float Float;

    Float = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    tempValue0 = floor(Float);
    Scr_AddFloat(tempValue0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:414676
void GScr_ceil()
{
    float tempValue0;
    float Float;

    Float = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    tempValue0 = ceil(Float);
    Scr_AddFloat(tempValue0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:414687
void GScr_sqrt()
{
    float Float;

    Float = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    Scr_AddFloat(sqrtf(Float), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:414696
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

// Decomp: CoDMPServer.c:414731
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

// Decomp: CoDMPServer.c:414766
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

// Decomp: CoDMPServer.c:414804
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

// Decomp: CoDMPServer.c:414853
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

// Decomp: CoDMPServer.c:414866
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

// Decomp: CoDMPServer.c:414883
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

// Decomp: CoDMPServer.c:414896
void Scr_Length()
{
    float value;
    float v[3];

    Scr_GetVector(0, v, SCRIPTINSTANCE_SERVER);
    value = Abs(v);
    Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:414907
void Scr_LengthSquared()
{
    float v[3];

    Scr_GetVector(0, v, SCRIPTINSTANCE_SERVER);
    Scr_AddFloat((float)((float)(v[0] * v[0]) + (float)(v[1] * v[1])) + (float)(v[2] * v[2]), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:414916
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

// Decomp: CoDMPServer.c:414933
void Scr_VectorDot()
{
    float b[3];
    float a[3];

    Scr_GetVector(0, a, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, b, SCRIPTINSTANCE_SERVER);
    Scr_AddFloat((float)((float)(a[0] * b[0]) + (float)(a[1] * b[1])) + (float)(a[2] * b[2]), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:414944
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

// Decomp: CoDMPServer.c:414957
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

// Decomp: CoDMPServer.c:414973
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

// Decomp: CoDMPServer.c:414986
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

// Decomp: CoDMPServer.c:415003
void Scr_AnglesToUp()
{
    float angles[3];
    float up[3];

    Scr_GetVector(0, angles, SCRIPTINSTANCE_SERVER);
    AngleVectors(angles, 0, 0, up);
    Scr_AddVector(up, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:415014
void Scr_AnglesToRight()
{
    float right[3];
    float angles[3];

    Scr_GetVector(0, angles, SCRIPTINSTANCE_SERVER);
    AngleVectors(angles, 0, right, 0);
    Scr_AddVector(right, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:415025
void Scr_AnglesToForward()
{
    float forward[3];
    float angles[3];

    Scr_GetVector(0, angles, SCRIPTINSTANCE_SERVER);
    AngleVectors(angles, forward, 0, 0);
    Scr_AddVector(forward, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:415036
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

// Decomp: CoDMPServer.c:415055
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

// Decomp: CoDMPServer.c:415073
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

// Decomp: CoDMPServer.c:415091
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

// Decomp: CoDMPServer.c:415106
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

// Decomp: CoDMPServer.c:415119
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

// Decomp: CoDMPServer.c:415153
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

// Decomp: CoDMPServer.c:415178
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

// Decomp: CoDMPServer.c:415241
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

// Decomp: CoDMPServer.c:415283
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

// Decomp: CoDMPServer.c:415320
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

// Decomp: CoDMPServer.c:415345
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

// Decomp: CoDMPServer.c:415370
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
// Decomp: CoDMPServer.c:415380
void __cdecl GScr_StopBurning(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( ent->client )
        ent->client->ps.eFlags2 &= ~0x200000u;
    else
        ent->s.lerp.eFlags2 &= ~0x200000u;
}

// Decomp: CoDMPServer.c:415395
void __cdecl GScr_SpawnNapalmGroundFlame(scr_entref_t entref)
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

// Decomp: CoDMPServer.c:415443
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

// Decomp: CoDMPServer.c:415465
void __cdecl GScr_clearCenterPopups(scr_entref_t entref)
{
    const char *fmtMsg;
    int clientNum;

    clientNum = GetEntity(entref)->s.number;
    fmtMsg = va("%c %c", 91, 110);
    SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, fmtMsg);
}

// Decomp: CoDMPServer.c:415479
void __cdecl GScr_clearPopups(scr_entref_t entref)
{
    const char *fmtMsg;
    int clientNum;

    clientNum = GetEntity(entref)->s.number;
    fmtMsg = va("%c %c", 91, 105);
    SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, fmtMsg);
}

// Decomp: CoDMPServer.c:415493
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

// Decomp: CoDMPServer.c:415513
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

// Decomp: CoDMPServer.c:415539
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

// Decomp: CoDMPServer.c:415563
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

// Decomp: CoDMPServer.c:415587
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

// Decomp: CoDMPServer.c:415613
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

// Decomp: CoDMPServer.c:415656
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

// Decomp: CoDMPServer.c:415692
void __cdecl GScr_ClearEndGameComplete(scr_entref_t entref)
{
    const char *fmtMsg;
    int clientNum;

    clientNum = GetEntity(entref)->s.number;
    fmtMsg = va("%c %c", 91, 107);
    SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, fmtMsg);
}

// Decomp: CoDMPServer.c:415706
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

// Decomp: CoDMPServer.c:415729
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

// Decomp: CoDMPServer.c:415756
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

// Decomp: CoDMPServer.c:415808
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

// Decomp: CoDMPServer.c:415831
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

// Decomp: CoDMPServer.c:415853
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

// Decomp: CoDMPServer.c:415876
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

// Decomp: CoDMPServer.c:415920
void __cdecl GScr_GetWaterHeight()
{
    float pos[3];
    float height;

    Scr_GetVector(0, pos, SCRIPTINSTANCE_SERVER);
    height = CM_GetWaterHeight(pos, 200.0, -200.0);
    Scr_AddFloat(height, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:415933
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

// Decomp: CoDMPServer.c:415954
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

// Decomp: CoDMPServer.c:415977
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

// Decomp: CoDMPServer.c:415994
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

// Decomp: CoDMPServer.c:416009
void __cdecl Scr_ErrorOnDefaultAsset(XAssetType type, char *assetName)
{
    const char *XAssetTypeName;
    const char *fmtMsg;

    DB_FindXAssetHeader(type, assetName, 1, -1);
    if ( DB_IsXAssetDefault(type, assetName) )
    {
        XAssetTypeName = DB_GetXAssetTypeName(type);
        fmtMsg = va("precache %s '%s' failed", XAssetTypeName, assetName);
        Scr_NeverTerminalError(fmtMsg, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:416024
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

// Decomp: CoDMPServer.c:416044
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

// Decomp: CoDMPServer.c:416060
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

// Decomp: CoDMPServer.c:416073
void Scr_PrecacheString()
{
    char *stringName;

    if (!level.initializing)
        Scr_Error("precacheString must be called before any wait statements in the gametype or level script\n", 0);
    stringName = Scr_GetIString(0, SCRIPTINSTANCE_SERVER);
    if (*stringName)
        G_LocalizedStringIndex( stringName);
}

// Decomp: CoDMPServer.c:416086
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

// Decomp: CoDMPServer.c:416124
void GScr_RadiusDamage()
{
    GScr_RadiusDamageInternal(0);
}

// Decomp: CoDMPServer.c:416130
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

// Decomp: CoDMPServer.c:416163
void __cdecl GScr_EntityRadiusDamage(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    GScr_RadiusDamageInternal(ent);
}

// Decomp: CoDMPServer.c:416175
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

// Decomp: CoDMPServer.c:416194
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

// Decomp: CoDMPServer.c:416226
void GScr_SetPlayerIgnoreRadiusDamage()
{
    level.bPlayerIgnoreRadiusDamageLatched = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:416236
void __cdecl GScr_DamageConeTrace(scr_entref_t entref)
{
    GScr_DamageConeTraceInternal(entref, 8396819);
}

// Decomp: CoDMPServer.c:416246
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

// Decomp: CoDMPServer.c:416300
void __cdecl GScr_SightConeTrace(scr_entref_t entref)
{
    GScr_DamageConeTraceInternal(entref, 14337);
}

// Decomp: CoDMPServer.c:416310
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

// Decomp: CoDMPServer.c:416362
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

// Decomp: CoDMPServer.c:416391
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

// Decomp: CoDMPServer.c:416419
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

// Decomp: CoDMPServer.c:416456
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

// Decomp: CoDMPServer.c:416487
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

// Decomp: CoDMPServer.c:416522
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

// Decomp: CoDMPServer.c:416547
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

// Decomp: CoDMPServer.c:416572
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

// Decomp: CoDMPServer.c:416597
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

// Decomp: CoDMPServer.c:416622
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

// Decomp: CoDMPServer.c:416651
void __cdecl GScr_IsPoisoned(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_Error("USAGE: Must be called on a client\n", 0);
    Scr_AddBool(ent->client->ps.poisoned, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:416676
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

// Decomp: CoDMPServer.c:416714
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

// Decomp: CoDMPServer.c:416754
void GScr_GetNorthYaw()
{
    float value;
    char northYawString[32];

    SV_GetConfigstring(0x60Cu, northYawString, 32);
    value = atof(northYawString);
    Scr_AddFloat(value, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:416765
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

// Decomp: CoDMPServer.c:416783
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

// Decomp: CoDMPServer.c:416861
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

// Decomp: CoDMPServer.c:416919
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

// Decomp: CoDMPServer.c:416958
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
    if ( SV_DObjGetBoneIndex(ent, tag) < 0 )
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

// Decomp: CoDMPServer.c:417017
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

// Decomp: CoDMPServer.c:417091
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

// Decomp: CoDMPServer.c:417159
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

// Decomp: CoDMPServer.c:417198
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
    guy = SpawnActor(ent, targetname.stringValue, FORCE_SPAWN, noEnemyInfo == 0);
    if ( guy )
    {
        ent->item[0].clipAmmoCount = level.time;
        Scr_AddEntity(guy, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:417251
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

// Decomp: CoDMPServer.c:417303
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

// Decomp: CoDMPServer.c:417346
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

// Decomp: CoDMPServer.c:417370
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

// Decomp: CoDMPServer.c:417437
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

// Decomp: CoDMPServer.c:417464
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

// Decomp: CoDMPServer.c:417531
void __cdecl Scr_SetFog(
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
    G_setfog(fmtMsg3);
}

// Decomp: CoDMPServer.c:417573
void Scr_SetVolumetricFog()
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
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    9901,
                    0,
                    "%s\n\t(density) = %g",
                    "(density > 0 && density <= 1)",
                    density) )
    {
        __debugbreak();
    }
    if ( (tempValue1 < 0.0 || tempValue1 > 1.0)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
                    9902,
                    0,
                    "%s\n\t(heightDensity) = %g",
                    "(heightDensity >= 0 && heightDensity <= 1)",
                    tempValue1) )
    {
        __debugbreak();
    }
    Scr_SetFog(
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

// Decomp: CoDMPServer.c:417712
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

// Decomp: CoDMPServer.c:417725
void Scr_VisionSetNaked()
{
    char *fmtMsg;
    int NumParam;
    float tempValue2;
    int duration;
    char *name;

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
    fmtMsg = va("\"%s\" %i", name, duration);
    SV_SetConfigstring(1550, fmtMsg);
}

// Decomp: CoDMPServer.c:417753
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

// Decomp: CoDMPServer.c:417781
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

// Decomp: CoDMPServer.c:417808
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

// Decomp: CoDMPServer.c:417835
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

// Decomp: CoDMPServer.c:417864
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
// Decomp: CoDMPServer.c:417896
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

// Decomp: CoDMPServer.c:417911
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

// Decomp: CoDMPServer.c:417926
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

// Decomp: CoDMPServer.c:417935
void GScr_IsPlayerNumber()
{
    if ( Scr_GetInt(0, SCRIPTINSTANCE_SERVER) < 0x20 )
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:417947
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

// Decomp: CoDMPServer.c:417968
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

// Decomp: CoDMPServer.c:418009
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

// Decomp: CoDMPServer.c:418026
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

// Decomp: CoDMPServer.c:418045
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
        Scr_AddInt(level.teamScores[2], SCRIPTINSTANCE_SERVER);
    else
        Scr_AddInt(level.teamScores[1], SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:418067
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
        level.teamScores[2] = teamScore;
        fmtMsg2 = va("%c %i", 72, teamScore);
    }
    else
    {
        level.teamScores[1] = teamScore;
        fmtMsg2 = va("%c %i", 71, teamScore);
    }
    SV_GameSendServerCommand(-1, SV_CMD_CAN_IGNORE, fmtMsg2);
    level.bUpdateScoresForIntermission = 1;
}

// Decomp: CoDMPServer.c:418100
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

// Decomp: CoDMPServer.c:418120
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

// Decomp: CoDMPServer.c:418150
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

// Decomp: CoDMPServer.c:418184
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

// Decomp: CoDMPServer.c:418202
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

// Decomp: CoDMPServer.c:418215
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

// Decomp: CoDMPServer.c:418240
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

// Decomp: CoDMPServer.c:418290
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

// Decomp: CoDMPServer.c:418361
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

// Decomp: CoDMPServer.c:418378
void __cdecl GScr_GetTagOrigin(scr_entref_t entref)
{
    VariableUnion tagName;
    gentity_s *ent;

    ent = GetEntity(entref);
    tagName.intValue = Scr_GetConstLowercaseString(0, SCRIPTINSTANCE_SERVER);
    GScr_UpdateTagInternal(ent, tagName.stringValue, &level.cachedTagMat, 1);
    Scr_AddVector(level.cachedTagMat.tagMat[3], SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:418393
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

// Decomp: CoDMPServer.c:418449
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

// Decomp: CoDMPServer.c:418466
void __cdecl GScr_GetEntnum(scr_entref_t entref)
{
    Scr_AddInt(entref.entnum, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:418472
void __cdecl GScr_SetDepthOfField(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
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
    if ( dofNearBlur < 4.0 || dofNearBlur > 10.0 )
    {
        fmtMsg = va("near blur should be between %g and %g", 4.0, 10.0);
        Scr_ParamError(4u, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
    if ( dofFarBlur < 0.0 || dofFarBlur > dofNearBlur )
    {
        fmtMsg2 = va("far blur should be >= %g and <= near blur", 0.0);
        Scr_ParamError(5u, fmtMsg2, SCRIPTINSTANCE_SERVER);
    }
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

// Decomp: CoDMPServer.c:418542
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

// Decomp: CoDMPServer.c:418568
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

// Decomp: CoDMPServer.c:418600
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

// Decomp: CoDMPServer.c:418621
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

// Decomp: CoDMPServer.c:418637
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

// Decomp: CoDMPServer.c:418653
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

// Decomp: CoDMPServer.c:418669
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

// Decomp: CoDMPServer.c:418685
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

// Decomp: CoDMPServer.c:418732
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

// Decomp: CoDMPServer.c:418797
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

// Decomp: CoDMPServer.c:418817
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

// Decomp: CoDMPServer.c:418834
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

// Decomp: CoDMPServer.c:418862
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

// Decomp: CoDMPServer.c:418879
void GScr_KillServer()
{
    level.finished = 3;
    level.savepersist = 0;
    SV_KillLocalServer();
}

// Decomp: CoDMPServer.c:418888
void GScr_AddTestClient()
{
    gentity_s *ent;

    ent = SV_AddTestClient();
    if ( ent )
        Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:418898
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

// Decomp: CoDMPServer.c:418939
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

// Decomp: CoDMPServer.c:418969
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

// Decomp: CoDMPServer.c:419023
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

// Decomp: CoDMPServer.c:419175
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

// Decomp: CoDMPServer.c:419189
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

// Decomp: CoDMPServer.c:419204
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

// Decomp: CoDMPServer.c:419219
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
        fmtMsg = va("%i", level.voteTime);
        SV_SetConfigstring(15, fmtMsg);
        fmtMsg2 = va("%i", level.voteYes);
        SV_SetConfigstring(17, fmtMsg2);
        fmtMsg3 = va("%i", level.voteNo);
        SV_SetConfigstring(18, fmtMsg3);
    }
}

// Decomp: CoDMPServer.c:419240
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
        fmtMsg2 = va("%i", level.voteYes);
        SV_SetConfigstring(17, fmtMsg2);
        fmtMsg3 = va("%i", level.voteNo);
        SV_SetConfigstring(18, fmtMsg3);
    }
}

// Decomp: CoDMPServer.c:419260
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
        fmtMsg2 = va("%i", level.voteNo);
        SV_SetConfigstring(18, fmtMsg2);
    }
}

// Decomp: CoDMPServer.c:419277
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

// Decomp: CoDMPServer.c:419291
void GScr_KickPlayer()
{
#if 0 // KISAKTODO: find out why the scripts are trying to kick
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
#else
    if (Scr_GetNumParam(SCRIPTINSTANCE_SERVER))
    {
        Com_DPrintf(15, "[Kisak] Scripts attempted to kick player %i", Scr_GetInt(0, SCRIPTINSTANCE_SERVER));
    }
#endif
}

// Decomp: CoDMPServer.c:419324
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

// Decomp: CoDMPServer.c:419345
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

// Decomp: CoDMPServer.c:419361
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

// Decomp: CoDMPServer.c:419442
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

// Decomp: CoDMPServer.c:419489
void GScr_FPrintln()
{
    Scr_FPrint_internal(0);
}

// Decomp: CoDMPServer.c:419495
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

// Decomp: CoDMPServer.c:419540
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

// Decomp: CoDMPServer.c:419602
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

// Decomp: CoDMPServer.c:419666
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

// Decomp: CoDMP_rdBlackOps.map.c (Scr_IsSplitscreen @ 8262FE98)
void Scr_IsSplitscreen()
{
    if ( onlinegame->current.enabled || CL_LocalClient_GetActiveCount() <= 1 )
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:419680
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

// Decomp: CoDMPServer.c:419720
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

// Decomp: CoDMPServer.c:419735
void GScr_SetBeginForMatchRecording()
{
#ifdef KISAK_LIVE
    MatchRecordBegin();
#endif
}

// Decomp: CoDMPServer.c:419740
void GScr_GetAssignedTeam()
{
    Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:419747
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

// Decomp: CoDMPServer.c:419763
void __cdecl GScr_IsItemPurchased(scr_entref_t entref)
{
    bool IsItemLockedForChallenge;

    IsItemLockedForChallenge = GScr_IsItemLockedForChallenge(entref, 1);
    Scr_AddInt(!IsItemLockedForChallenge, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:419775
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

// Decomp: CoDMPServer.c:419819
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

// Decomp: CoDMPServer.c:419853
void __cdecl GScr_IsItemLocked(scr_entref_t entref)
{
    bool IsItemLockedForChallenge;

    IsItemLockedForChallenge = GScr_IsItemLockedForChallenge(entref, 0);
    Scr_AddInt(IsItemLockedForChallenge, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:419865
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

// Decomp: CoDMPServer.c:419879
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

// Decomp: CoDMPServer.c:419893
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


// Decomp: CoDMPServer.c:419924
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

// Decomp: CoDMPServer.c:419962
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

// Decomp: CoDMPServer.c:419975
void __cdecl GScr_GetLoadoutItemFromProfile(scr_entref_t entref)
{
    gentity_s *playerEnt;
    const char *className;
    const char *slotName;
    unsigned int item;
    const dvar_s *killstreakVar;
    bool isKillstreakSlot;
    int customKillstreakMode;

    playerEnt = GetEntity(entref);
    if ( !playerEnt->client )
        Scr_Error("getloadoutitemfromprofile: entity must be a player entity", 0);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 1 )
    {
        slotName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        className = 0;
    }
    else
    {
        className = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
        slotName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    }

    item = 0;
    isKillstreakSlot = I_stristr(slotName, "killstreak") != 0;
    customKillstreakMode = Dvar_GetInt(custom_killstreak_mode);
    if ( customKillstreakMode == 1 && isKillstreakSlot )
    {
        item = BG_UnlockablesGetItemIndexFromName("killstreak_null");
    }
    else if ( customKillstreakMode == 2 && isKillstreakSlot )
    {
        killstreakVar = Dvar_FindVar(va(
            "custom_killstreak_%c",
            slotName[strlen(slotName) - 1]));
        if ( killstreakVar && killstreakVar->current.string )
            item = (unsigned int)atoi(killstreakVar->current.string);
    }
    else if ( !Dvar_GetInt(ui_useCustomClassInfo) || isKillstreakSlot )
    {
        item = BG_UnlockablesGetEquippedItemInSlot(0, className, slotName);
    }
    else
    {
        item = GetCustomClassLoadoutItemForClassName(0, className, slotName);
    }

    if ( item == (unsigned int)-1 )
        item = 0;

    Scr_AddInt(item, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:420002
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
    Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
#endif
}

// Decomp: CoDMPServer.c:420110
void GScr_GetMaxActiveContracts()
{
    Scr_AddInt(3, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:420117
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

// Decomp: CoDMPServer.c:420147
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

// Decomp: CoDMPServer.c:420165
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

// Decomp: CoDMPServer.c:420183
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

// Decomp: CoDMPServer.c:420201
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

// Decomp: CoDMPServer.c:420219
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

// Decomp: CoDMPServer.c:420253
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

// Decomp: CoDMPServer.c:420271
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

// Decomp: CoDMPServer.c:420289
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

// Decomp: CoDMPServer.c:420310
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

// Decomp: CoDMPServer.c:420340
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

// Decomp: CoDMPServer.c:420377
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

// Decomp: CoDMPServer.c:420405
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

// Decomp: CoDMPServer.c:420442
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

// Decomp: CoDMPServer.c:420473
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

// Decomp: CoDMPServer.c:420504
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

// Decomp: CoDMPServer.c:420534
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

// Decomp: CoDMPServer.c:420549
void GScr_EnableOccluder()
{
    char *occluderName;
    int enable;

    occluderName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    enable = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    R_EnableOccluder(occluderName, enable != 0);
}

// Decomp: CoDMPServer.c:420560
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

// Decomp: CoDMPServer.c:420575
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

// Decomp: CoDMPServer.c:420590
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

// Decomp: CoDMPServer.c:420736
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

// Decomp: CoDMPServer.c:420771
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

// Decomp: CoDMPServer.c:420788
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

// Decomp: CoDMPServer.c:420803
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

// Decomp: CoDMPServer.c:420857
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

// Decomp: CoDMPServer.c:420896
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

// Decomp: CoDMPServer.c:420916
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

// Decomp: CoDMPServer.c:420953
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

// Decomp: CoDMPServer.c:420987
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

// Decomp: CoDMPServer.c:421009
void GScr_SetMapCenter()
{
    float mapCenter[3];

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("Expected 1 argument to setMapCenter()", 0);
    Scr_GetVector(0, mapCenter, SCRIPTINSTANCE_SERVER);
    SV_SetMapCenter(mapCenter);
}

// Decomp: CoDMPServer.c:421020
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

// Decomp: CoDMPServer.c:421036
void GScr_StartDemoRecording()
{
    if ( Demo_IsEnabled() )
    {
        if ( Demo_IsIdle() )
            Cbuf_AddText(0, "demo_startrecord\n");
    }
}

// Decomp: CoDMPServer.c:421046
void GScr_StopDemoRecording()
{
    if ( Demo_IsRecording() )
        Cbuf_AddText(0, "demo_stoprecord\n");
}

// Decomp: CoDMPServer.c:421053
void GScr_IsDemoRecording()
{
    bool IsRecording;

    IsRecording = Demo_IsRecording();
    Scr_AddBool(IsRecording, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:421062
void isDemoEnabled()
{
    bool IsEnabled;

    IsEnabled = Demo_IsEnabled();
    Scr_AddBool(IsEnabled, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:421071
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

// Decomp: CoDMPServer.c:421087
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

// Decomp: CoDMPServer.c:421103
void GScr_SetGameEndTime()
{
    VariableUnion varUnion;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("Expected 1 argument to setGameEndTime()", 0);
    varUnion.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    SV_SetGameEndTime(varUnion.intValue);
}

// Decomp: CoDMPServer.c:421114
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
// Decomp: CoDMPServer.c:421127
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

// Decomp: CoDMPServer.c:421168
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

// Decomp: CoDMPServer.c:421190
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
        level.teamHasSpyplane[2] = SpyplaneAvailable;
    }
    else if (team == scr_const.axis)
    {
        level.teamHasSpyplane[1] = SpyplaneAvailable;
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
        level.teamHasSpyplane[0] = SpyplaneAvailable;
    }
}

// Decomp: CoDMPServer.c:421239
void GScr_GetTeamSpyplane()
{
    char *slStr;
    const char *fmtMsg;
    unsigned __int16 team;

    team = (unsigned __int16)Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    if ( team == scr_const.allies )
    {
        Scr_AddInt(level.teamHasSpyplane[2], SCRIPTINSTANCE_SERVER);
    }
    else if ( team == scr_const.axis )
    {
        Scr_AddInt(level.teamHasSpyplane[1], SCRIPTINSTANCE_SERVER);
    }
    else if ( team == scr_const.none )
    {
        Scr_AddInt(level.teamHasSpyplane[0], SCRIPTINSTANCE_SERVER);
    }
    else
    {
        slStr = SL_ConvertToString(team, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("Illegal team string '%s'. Must be allies, axis, or none.", slStr);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:421267
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
        level.teamHasSatellite[2] = SatelliteAvailable;
    }
    else if (team == scr_const.axis)
    {
        level.teamHasSatellite[1] = SatelliteAvailable;
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
        level.teamHasSatellite[0] = SatelliteAvailable;
    }
}

// Decomp: CoDMPServer.c:421316
void GScr_GetTeamSatellite()
{
    char *slStr;
    const char *fmtMsg;
    unsigned __int16 team;

    team = (unsigned __int16)Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    if ( team == scr_const.allies )
    {
        Scr_AddInt(level.teamHasSatellite[2], SCRIPTINSTANCE_SERVER);
    }
    else if ( team == scr_const.axis )
    {
        Scr_AddInt(level.teamHasSatellite[1], SCRIPTINSTANCE_SERVER);
    }
    else if ( team == scr_const.none )
    {
        Scr_AddInt(level.teamHasSatellite[0], SCRIPTINSTANCE_SERVER);
    }
    else
    {
        slStr = SL_ConvertToString(team, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("Illegal team string '%s'. Must be allies, axis, or none.", slStr);
        Scr_ParamError(0, fmtMsg, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:421344
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

// Decomp: CoDMPServer.c:421361
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

// Decomp: CoDMPServer.c:421451
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

// Decomp: CoDMPServer.c:421600
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

// Decomp: CoDMPServer.c:421660
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

// Decomp: CoDMPServer.c:421682
void __cdecl GScr_IsOnLadder(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_Error("setstat: entity must be a player entity", 0);
    Scr_AddBool((ent->client->ps.pm_flags & 8) != 0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:421696
void __cdecl GScr_IsMantling(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_Error("setstat: entity must be a player entity", 0);
    Scr_AddBool((ent->client->ps.pm_flags & 4) != 0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:421710
void __cdecl GScr_StartDoorBreach(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_ObjectError("Not a player entity", SCRIPTINSTANCE_SERVER);
    ent->client->ps.pm_flags |= 0x1000000u;
}

// Decomp: CoDMPServer.c:421724
void __cdecl GScr_StopDoorBreach(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GetEntity(entref);
    if ( !ent->client )
        Scr_ObjectError("Not a player entity", SCRIPTINSTANCE_SERVER);
    ent->client->ps.pm_flags &= ~0x1000000u;
}

// Decomp: CoDMPServer.c:421738
void __cdecl GScr_GetLightColor(scr_entref_t entref)
{
    gentity_s *ent;
    float unpackedColor[4];

    ent = GScr_SetupLightEntity(entref);
    Byte4UnpackRgba((const unsigned __int8 *)&ent->s.lerp.u, unpackedColor);
    Scr_AddVector(unpackedColor, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:421752
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

// Decomp: CoDMPServer.c:421783
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

// Decomp: CoDMPServer.c:421801
void __cdecl GScr_GetLightIntensity(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GScr_SetupLightEntity(entref);
    Scr_AddFloat(ent->s.lerp.u.turret.gunAngles[1], SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:421813
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

// Decomp: CoDMPServer.c:421835
void __cdecl GScr_GetLightRadius(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GScr_SetupLightEntity(entref);
    Scr_AddFloat(ent->s.lerp.u.turret.gunAngles[2], SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:421847
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

// Decomp: CoDMPServer.c:421899
void __cdecl GScr_GetLightFovInner(scr_entref_t entref)
{
    gentity_s *ent = GScr_SetupLightEntity(entref);
    float innerCos = ent->s.lerp.u.primaryLight.cosHalfFovInner;
    float fov = acosf(innerCos) * 2.0f;

    Scr_AddFloat(fov, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:421916
void __cdecl GScr_GetLightFovOuter(scr_entref_t entref)
{
    gentity_s *ent = GScr_SetupLightEntity(entref);
    float outerCos = ent->s.lerp.u.primaryLight.cosHalfFovOuter;
    float fov = acosf(outerCos) * 2.0f;

    Scr_AddFloat(fov, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:421933
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

// Decomp: CoDMPServer.c:422029
void __cdecl GScr_GetLightExponent(scr_entref_t entref)
{
    gentity_s *ent;

    ent = GScr_SetupLightEntity(entref);
    Scr_AddInt(ent->s.lerp.u.primaryLight.colorAndExp[3], SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:422041
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

// Decomp: CoDMPServer.c:422057
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

// Decomp: CoDMPServer.c:422098
void __cdecl GScr_IsRagdoll(scr_entref_t entref)
{
    bool IsRagdollTrajectory;
    gentity_s *ent;

    ent = GetEntity(entref);
    IsRagdollTrajectory = Com_IsRagdollTrajectory(&ent->s.lerp.pos);
    Scr_AddInt(IsRagdollTrajectory, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:422112
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

// Decomp: CoDMPServer.c:422141
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

// Decomp: CoDMPServer.c:422189
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

// Decomp: CoDMPServer.c:422211
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

// Decomp: CoDMPServer.c:422252
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

// Decomp: CoDMPServer.c:422295
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

// Decomp: CoDMPServer.c:422342
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

// Decomp: CoDMPServer.c:422391
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

// Decomp: CoDMPServer.c:422433
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
        anim.index = ent->s.animState.state & 0xFBFF;
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

// Decomp: CoDMPServer.c:422461
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

// Decomp: CoDMPServer.c:749725
void __cdecl Scr_AddStruct()
{
    Scr_AddStruct(SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:750779
void __cdecl Scr_ResetTimeout()
{
    Scr_ResetTimeout(SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:422535
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

// Decomp: CoDMPServer.c:422573
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

// Decomp: CoDMPServer.c:422620
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

// Decomp: CoDMPServer.c:422628
void GScr_GetAITriggerFlags()
{
    Scr_AddInt(7, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:422635
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

// Decomp: CoDMPServer.c:422644
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

// Decomp: CoDMPServer.c:422660
void __cdecl GScr_GetMaxVehicles()
{
    Scr_AddInt(16, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:422667
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

// Decomp: CoDMPServer.c:422686
void __cdecl GScr_EnableAllDestructiblePieces()
{
    float zero_vec[3];
    gentity_s *tempent;

    EnableAllDestructiblePieces();
    memset(zero_vec, 0, sizeof(zero_vec));
    tempent = G_TempEntity(zero_vec, EV_DESTRUCTIBLE_DISABLE_PIECES);
    tempent->s.eventParm = 0;
}

// Decomp: CoDMPServer.c:422700
void __cdecl GScr_ClearSpawnPoints()
{
    SpawnSystem_ClearPoints();
}

// Decomp: CoDMPServer.c:422707
void __cdecl GScr_SetSpawnPointRandomVariation()
{
    float variation;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("USAGE: setspawnpointrandomvariation( <variation> )\n", 0);
    variation = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
    SpawnSystem_SetRandomVariation(variation);
}

// Decomp: CoDMPServer.c:422718
void __cdecl GScr_ClearSpawnPointsBaseWeight()
{
    VariableUnion team_mask;

    team_mask.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    SpawnSystem_ClearPointsBaseWeight(team_mask.intValue);
}

// Decomp: CoDMPServer.c:422727
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

// Decomp: CoDMPServer.c:422744
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

// Decomp: CoDMPServer.c:422831
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

// Decomp: CoDMPServer.c:422867
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

// Decomp: CoDMPServer.c:422888
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

// Decomp: CoDMPServer.c:422927
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

// Decomp: CoDMPServer.c:423036
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

// Decomp: CoDMPServer.c:423072
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

// Decomp: CoDMPServer.c:423167
void __cdecl GScr_RemoveInfluencer()
{
    unsigned int influencer_id;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("USAGE: removeinfluencer(<influencer id> )\n", 0);
    influencer_id = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( !SpawnSystem_RemoveInfluencer(influencer_id) )
        Scr_Error("Trying to remove influencer that does not exist\n", 0);
}

// Decomp: CoDMPServer.c:423179
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

// Decomp: CoDMPServer.c:423193
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

// Decomp: CoDMPServer.c:423207
void __cdecl GScr_SetDebugSideSwitch()
{
    int enabled;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("USAGE: setdebugsideswitch( <enabled> )\n", 0);
    enabled = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    SpawnSystem_DebugSideSwitch(enabled != 0);
}

// Decomp: CoDMPServer.c:423218
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

// Decomp: CoDMPServer.c:423284
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

// Decomp: CoDMPServer.c:423409
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

// Decomp: CoDMPServer.c:423580
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

// Decomp: CoDMPServer.c:423771
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

// Decomp: CoDMPServer.c:423957
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

// Decomp: CoDMPServer.c:423988
int __cdecl sort_scored_spawn_point_vectors_ascending(float *a, float *b)
{
    if ( a[1] > b[1] )
        return 1;
    if ( b[1] <= a[1] )
        return 0;
    return -1;
}

// Decomp: CoDMPServer.c:423998
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

// Decomp: CoDMPServer.c:424104
void GScr_GetNumGVRules()
{
    Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:424111
void GScr_GetGVRule()
{
    Scr_GameVariants_GetRule();
}

// Decomp: CoDMPServer.c:424117
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

// Decomp: CoDMPServer.c:424143
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

// Decomp: CoDMPServer.c:424169
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

// Decomp: CoDMPServer.c:424195
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

// Decomp: CoDMPServer.c:424221
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

// Decomp: CoDMPServer.c:424247
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

// Decomp: CoDMPServer.c:424281
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

// Decomp: CoDMPServer.c:424306
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

// Decomp: CoDMPServer.c:424331
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

// Decomp: CoDMPServer.c:424356
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

// Decomp: CoDOMPServer.c:424381
void GScr_PCServerUpdatePlaylist()
{
    SV_FetchWADDeferred();
}

// Decomp: CoDMPServer.c:424387
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

// Decomp: CoDMPServer.c:424423
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

// Decomp: CoDMPServer.c:424466
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

// Decomp: CoDMPServer.c:424513
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

// Decomp: CoDMPServer.c:424549
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

// Decomp: CoDMPServer.c:424560
void GScr_PixMarker()
{
    char *String;
    const char *fmtMsg;

    String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    fmtMsg = va("SCRIPT: %s", String);
    //PIXSetMarker(0, fmtMsg);
}

// Decomp: CoDMPServer.c:424584
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

// Decomp: CoDMPServer.c:424633
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

// Decomp: CoDMPServer.c:424674
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
            level.teamScores[i - 24] = CScr_GetColumnTypeByName(String);
            if ( !level.teamScores[i - 24] )
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

// Decomp: CoDMPServer.c:424704
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

// Decomp: CoDMPServer.c:424745
void GScr_IsPregameEnabled()
{
    if ( Pregame_isEnabled() )
        Scr_AddBool(1u, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddBool(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:424753
void GScr_ResetPregameData()
{
    Pregame_ResetData();
}

// Decomp: CoDMPServer.c:424760
void GScr_IsPregameGameStarted()
{
    if ( Pregame_isEnabled() && Pregame_GetState() == PREGAME_GAMESTARTED )
        Scr_AddBool(1u, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddBool(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:424768
void GScr_PregameStartGame()
{
    Pregame_StartGame();
}

// Decomp: CoDMPServer.c:407571
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

// Decomp: CoDMPServer.c:407595
void println_0()
{
    if (!g_NoScriptSpam->current.enabled)
    {
        print_0();
        Com_Printf(level.scriptPrintChannel, "\n");
    }
}

// Decomp: CoDMPServer.c:407212
void FUNCTION_NULLSUB()
{

}

// Decomp: CoDMPServer.c:408310
void assertCmd_0()
{
    if (!Scr_GetInt(0, SCRIPTINSTANCE_SERVER))
        Scr_Error("assert fail", 0);
}

// Decomp: CoDMPServer.c:408317
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

// Decomp: CoDMPServer.c:408331
void assertmsgCmd_0()
{
    char *String;
    char *fmtMsg;

    String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    fmtMsg = va("assert fail: %s", String);
    Scr_Error(fmtMsg, 0);
}

// Decomp: CoDMPServer.c:419692
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

BuiltinFunctionDef functions[] =
{
  { "createprintchannel", GScr_CreatePrintChannel, 1 },
  { "setprintchannel", GScr_printChannelSet, 1 },
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
  { "getnode", Scr_GetNode, 0 },
  { "getnodearray", Scr_GetNodeArray, 0 },
  { "getallnodes", Scr_GetAllNodes, 0 },
  { "getatrloaded", GScr_GetAnimTreesLoaded, 0 },
  { "findanimbyname", GScr_FindAnimByName, 0 },
  { "spawn", GScr_Spawn, 0 },
  { "spawncollision", GScr_SpawnCollision, 0 },
  { "spawntimedfx", GScr_SpawnTimedFX, 0 },
  { "spawnvehicle", GScr_SpawnVehicle, 0 },
  { "spawnplane", GScr_SpawnPlane, 0 },
  { "spawnturret", GScr_SpawnTurret, 0 },
  { "precacheturret", GScr_PrecacheTurret, 0 },
  { "spawnstruct", Scr_AddStruct, 0 },
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
  { "getdvar", GScr_GetDvar, 0 },
  { "getdvarint", GScr_GetDvarInt, 0 },
  { "getdvarfloat", GScr_GetDvarFloat, 0 },
  { "getdvarcolorred", GScr_GetDvarColorRed, 0 },
  { "getdvarcolorgreen", GScr_GetDvarColorGreen, 0 },
  { "getdvarcolorblue", GScr_GetDvarColorBlue, 0 },
  { "setdvar", GScr_SetDvar, 0 },
  { "gettime", GScr_GetTime, 0 },
  { "getentbynum", Scr_GetEntByNum, 1 },
  { "getweaponmodel", Scr_GetWeaponModel, 0 },
  { "getweaponstowedmodel", Scr_GetWeaponStowedModel, 0 },
  { "getanimlength", GScr_GetAnimLength, 0 },
  { "animhasnotetrack", GScr_AnimHasNotetrack, 0 },
  { "getnotetracktimes", GScr_GetNotetrackTimes, 0 },
  { "getbrushmodelcenter", GScr_GetBrushModelCenter, 0 },
  { "getattachmentindex", GScr_GetAttachmentIndex, 0 },
  // LWSS ADD FROM RETAIL BLOPS MP (LATEST FILES FROM STEAM REFERENCE NEWER FUNCTIONS THAT WERE ADDED LATER)
  { "getcorpsearray", GScr_GetCorpseArray, 0 },
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
  { "missile_createattractorent", Scr_MissileCreateAttractorEnt, 0 },
  { "missile_createattractororigin", Scr_MissileCreateAttractorOrigin, 0 },
  { "missile_createrepulsorent", Scr_MissileCreateRepulsorEnt, 0 },
  { "missile_createrepulsororigin", Scr_MissileCreateRepulsorOrigin, 0 },
  { "missile_deleteattractor", Scr_MissileDeleteAttractor, 0 },
  { "bullettrace", Scr_BulletTrace, 0 },
  { "bullettracepassed", Scr_BulletTracePassed, 0 },
  { "sighttracepassed", Scr_SightTracePassed, 0 },
  { "physicstrace", Scr_PhysicsTrace, 0 },
  { "playerphysicstrace", Scr_PlayerPhysicsTrace, 0 },
  // LWSS ADD FROM RETAIL BLOPS MP (LATEST FILES FROM STEAM REFERENCE NEWER FUNCTIONS THAT WERE ADDED LATER)
  { "playerbullettrace", Scr_PlayerBulletTrace, 0 },
  // LWSS END
  { "getmovedelta", GScr_GetMoveDelta, 0 },
  { "getangledelta", GScr_GetAngleDelta, 0 },
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
  { "float", GScr_CastFloat, 0 },
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
  { "distancesquared", Scr_DistanceSquared, 0 },
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
  { "absangleclamp180", Scr_AbsAngleClamp180, 0 },
  { "rotatepoint", Scr_RotatePoint, 0 },
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
  { "precachestring", Scr_PrecacheString, 0 },
  { "precacherumble", FUNCTION_NULLSUB, 0 },
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
  { "triggerfx", Scr_TriggerFX, 0 },
  { "physicsexplosionsphere", Scr_PhysicsExplosionSphere, 0 },
  { "physicsexplosioncylinder", Scr_PhysicsExplosionCylinder, 0 },
  { "physicsjolt", Scr_PhysicsRadiusJolt, 0 },
  { "createstreamerhint", Scr_CreateStreamerHint, 0 },
  { "setexpfog", Scr_SetExponentialFog, 0 },
  { "setvolfog", Scr_SetVolumetricFog, 0 },
  { "setculldist", Scr_SetCullDist, 0 },
  { "grenadeexplosioneffect", Scr_GrenadeExplosionEffect, 0 },
  { "magicbullet", GScr_MagicBullet, 0 },
  { "radiusdamage", GScr_RadiusDamage, 0 },
  { "setplayerignoreradiusdamage", GScr_SetPlayerIgnoreRadiusDamage, 0 },
  { "glassradiusdamage", GScr_GlassRadiusDamage, 0 },
  { "getnumparts", GScr_GetNumParts, 0 },
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
  { "weaponissemiauto", GScr_WeaponIsSemiAuto, 0 },
  { "weaponisboltaction", GScr_WeaponIsBoltAction, 0 },
  { "weapontype", GScr_WeaponType, 0 },
  { "weaponclass", GScr_WeaponClass, 0 },
  { "weaponmountable", GScr_WeaponIsMountable, 0 },
  { "weaponinventorytype", GScr_WeaponInventoryType, 0 },
  { "weaponstartammo", GScr_WeaponStartAmmo, 0 },
  { "weaponmaxammo", GScr_WeaponMaxAmmo, 0 },
  { "weaponaltweaponname", GScr_WeaponAltWeaponName, 0 },
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
  { "reviveobituary", GScr_ReviveObituary, 0 },
  { "adddemobookmark", GScr_AddDemoBookmark, 0 },
  { "positionwouldtelefrag", GScr_positionWouldTelefrag, 0 },
  { "boundswouldtelefrag", GScr_BoundsWouldTelefrag, 0 },
  { "recordusedspawnpoint", GScr_RecordUsedSpawnPoint, 0 },
  { "testspawnpoint", GScr_TestSpawnPoint, 0 },
  { "getstarttime", GScr_getStartTime, 0 },
  { "map_restart", GScr_MapRestart, 0 },
  { "exitlevel", GScr_ExitLevel, 0 },
  { "killserver", GScr_KillServer, 0 },
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
  { "pcserverupdateplaylist", GScr_PCServerUpdatePlaylist, 0 },
  { "kick", GScr_KickPlayer, 0 },
  { "ban", GScr_BanPlayer, 0 },
  { "map", GScr_LoadMap, 0 },
  { "playrumbleonposition", FUNCTION_NULLSUB, 0 },
  { "playrumblelooponposition", FUNCTION_NULLSUB, 0 },
  { "stopallrumbles", FUNCTION_NULLSUB, 0 },
  { "soundexists", ScrCmd_SoundExists, 0 },
  { "issplitscreen", GScr_GetAssignedTeam, 0 },
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
  { "getvehicletreadfxarray", GScr_GetVehicleTreadFXArray, 0 },
  { "disabledestructiblepieces", GScr_DisableDestructiblePieces, 0 },
  { "enablealldestructiblepieces", GScr_EnableAllDestructiblePieces, 0 },
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
  // LWSS ADD FROM RETAIL BLOPS MP (LATEST FILES FROM STEAM REFERENCE NEWER FUNCTIONS THAT WERE ADDED LATER)
  { "starthostmigration", FUNCTION_NULLSUB, 0 }, // (Stubbing these out because who cares)
  { "reportfilm", FUNCTION_NULLSUB, 0 },
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

// Decomp: CoDMPServer.c:424795
void __cdecl GScr_SetClientFlag(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;
    int flag;

    pSelf = GetEntity(entref);
    flag = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    // Decomp: CoDOMPServer.c:424806 — flag >= 0 && flag < 16
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

// Decomp: CoDMPServer.c:424821
void __cdecl GScr_ClearClientFlag(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;
    int flag;

    pSelf = GetEntity(entref);
    flag = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    // Decomp: CoDOMPServer.c:424806 — flag >= 0 && flag < 16
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

// Decomp: CoDMPServer.c:424847
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

// Decomp: CoDMPServer.c:424891
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

// Decomp: CoDMPServer.c:424952
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

// Decomp: CoDMPServer.c:424984
void __cdecl G_FlagAnimForUpdate(gentity_s *ent)
{
    if ( (ent->flags & 0x2000) == 0 )
        ent->flags |= 0x40000u;
}

// Decomp: CoDMPServer.c:424991
void __cdecl GScr_SetAnim(scr_entref_t entref)
{
    PROF_SCOPED("SetAnim");
    GScr_SetAnimInternal(entref, 1);
}

// Decomp: CoDMPServer.c:425005
void __cdecl GScr_SetAnimInternal(scr_entref_t entref, char flags)
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
                            0,
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
        error = XAnimSetGoalWeight(obj, anim.index, goalWeight, goalTime, rate, 0, notifyFlags, (flags & 2) != 0, cmdIndex);
    }
    if ( error )
        GScr_HandleAnimError(error);
    else
        G_FlagAnimForUpdate(ent);
}

// Decomp: CoDMPServer.c:425099
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

// Decomp: CoDMPServer.c:425118
double __cdecl GScr_GetOptionalFloat(unsigned int iParamIndex, float fDefault)
{
    if ( Scr_GetType(iParamIndex, SCRIPTINSTANCE_SERVER) )
        return Scr_GetFloat(iParamIndex, SCRIPTINSTANCE_SERVER);
    else
        return fDefault;
}

// Decomp: CoDMPServer.c:425130
void __cdecl G_SetAnimTree(gentity_s *ent, scr_animtree_t *animtree)
{
    XAnimTree_s *oldAnimTree;
    bool updateDObj;

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

// Decomp: CoDMPServer.c:425164
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
    if (method)
        return method;

    method = BuiltIn_GetMethod(pName, type);

    return method;
}

// Decomp: CoDMPServer.c:407212
static void METHOD_NULLSUB(scr_entref_t ref)
{

}

BuiltinMethodDef s_builtinMethods[] =
{
  { "attach", ScrCmd_attach, 0 },
  { "detach", ScrCmd_detach, 0 },
  { "detachall", ScrCmd_detachAll, 0 },
  { "getattachsize", ScrCmd_GetAttachSize, 0 },
  { "getattachmodelname", ScrCmd_GetAttachModelName, 0 },
  { "getattachtagname", ScrCmd_GetAttachTagName, 0 },
  { "getattachignorecollision", ScrCmd_GetAttachIgnoreCollision, 0 },
  { "getammocount", GScr_GetAmmoCount, 0 },
  { "getclanid", ScrCmd_GetClanId, 0 },
  { "getclanname", ScrCmd_GetClanName, 0 },
  { "hidepart", ScrCmd_hidepart, 0 },
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
  { "playerlinktodelta", ScrCmd_PlayerLinkToDelta, 0 },
  { "unlink", ScrCmd_Unlink, 0 },
  { "enablelinkto", ScrCmd_EnableLinkTo, 0 },
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
  { "stoploopsound", ScrCmd_StopLoopSound, 0 },
  { "playrumbleonentity", METHOD_NULLSUB, 0 },
  { "playrumblelooponentity", METHOD_NULLSUB, 0 },
  { "stoprumble", METHOD_NULLSUB, 0 },
  { "delete", ScrCmd_Delete, 0 },
  { "setmodel", ScrCmd_SetModel, 0 },
  { "setenemymodel", ScrCmd_SetEnemyModel, 0 },
  { "dodamage", ScrCmd_DoDamage, 0 },
  { "getnormalhealth", ScrCmd_GetNormalHealth, 0 },
  { "setnormalhealth", ScrCmd_SetNormalHealth, 0 },
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
  { "gettagorigin", GScr_GetTagOrigin, 0 },
  { "gettagangles", GScr_GetTagAngles, 0 },
  { "getentnum", GScr_GetEntnum, 1 },
  { "stopshellshock", GScr_StopShellShock, 0 },
  { "setdepthoffield", GScr_SetDepthOfField, 0 },
  { "setburn", GScr_SetBurn, 0 },
  { "setelectrified", GScr_SetElectrified, 0 },
  { "spawnnapalmgroundflame", GScr_SpawnNapalmGroundFlame, 0 },
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
  { "setrightarc", GScr_SetRightArc, 0 },
  { "setleftarc", GScr_SetLeftArc, 0 },
  { "settoparc", GScr_SetTopArc, 0 },
  { "setbottomarc", GScr_SetBottomArc, 0 },
  { "radiusdamage", GScr_EntityRadiusDamage, 0 },
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
  { "useanimtree", GScr_UseAnimTree, 0 },
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
  { "isonladder", GScr_IsOnLadder, 0 },
  { "ismantling", GScr_IsMantling, 0 },
  { "startdoorbreach", GScr_StartDoorBreach, 0 },
  { "stopdoorbreach", GScr_StopDoorBreach, 0 },
  { "startragdoll", GScr_StartRagdoll, 0 },
  { "isragdoll", GScr_IsRagdoll, 0 },
  { "launchragdoll", GScr_RagdollLaunch, 0 },
  { "launchvehicle", GScr_VehicleLaunch, 0 },
  { "giveachievement", GScr_GiveAchievement, 0 },
  { "setvehicleteam", GScr_SetTeam, 0 },
  { "setteam", GScr_SetTeam, 0 },
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
  { "spawnactor", ScrCmd_SpawnActor, 0 },
  { "getshootatpos", ScrCmd_GetShootAtPosition, 0 },
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
  { "istestclient", GScr_isTestClient, 0 }
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

// Decomp: CoDMPServer.c:425240
void __cdecl Scr_SetOrigin(gentity_s *ent, int)
{
    float org[3];

    Scr_GetVector(0, org, SCRIPTINSTANCE_SERVER);
    G_SetOrigin(ent, org);
    if ( ent->r.linked )
        SV_LinkEntity(ent);
}

// Decomp: CoDMPServer.c:425252
void __cdecl Scr_SetAngles(gentity_s *ent, int)
{
    float angles[3];

    Scr_GetVector(0, angles, SCRIPTINSTANCE_SERVER);
    G_SetAngle(ent, angles);
}

// Decomp: CoDMPServer.c:425261
void __cdecl Scr_SetExposureIndex(gentity_s *ent, int)
{
    ent->item[1].index = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:425267
void __cdecl Scr_SetExposureLerpToLighter(gentity_s *ent, int unused)
{
    ent->trigger.exposureLerpToLighter = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:425273
void __cdecl Scr_SetExposureLerpToDarker(gentity_s *ent, int unused)
{
    ent->trigger.exposureLerpToDarker = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:425279
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

// Decomp: CoDMPServer.c:425297
void __cdecl GScr_AddVector(float *vVec)
{
    if ( vVec )
        Scr_AddVector(vVec, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddUndefined(SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:425306
void __cdecl GScr_AddEntity(gentity_s *pEnt)
{
    if ( pEnt )
        Scr_AddEntity(pEnt, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddUndefined(SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:425314
void __cdecl Scr_ParseGameTypeList()
{
    if ( useFastFile->current.enabled )
        ((void (__cdecl *)(void (*)()))Scr_ParseGameTypeList_FastFile)(Scr_ParseGameTypeList_FastFile);
    else
        ((void (__cdecl *)(int (*)()))Scr_ParseGameTypeList_LoadObj)(Scr_ParseGameTypeList_LoadObj);
}

// Decomp: CoDMPServer.c:425327
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
    FileList = FS_GetFileList("maps/mp/gametypes", (char*)"gsc", FS_LIST_PURE_ONLY, listbuf, 4096);
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
            qpath = va("maps/mp/gametypes/%s.txt", src);
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
                    fmtMsg2 = va("maps/mp/gametypes/%s.txt", src);
                    Com_PrintWarning(24, "WARNING: GameType description file %s is too big to load.\n", fmtMsg2);
                }
                else
                {
                    fmtMsg = va("maps/mp/gametypes/%s.txt", src);
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

// Decomp: CoDMPServer.c:425407
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
    gametypesFile = DB_FindXAssetHeader(ASSET_TYPE_RAWFILE, (char*)"maps/mp/gametypes/_gametypes.txt", 1, -1).rawfile;
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
            fullname = va("maps/mp/gametypes/%s.txt", pszFileName->token);
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
                    fmtMsg2 = va("maps/mp/gametypes/%s.txt", pszFileName->token);
                    Com_PrintWarning(24, "WARNING: GameType description file %s is too big to load.\n", fmtMsg2);
                }
                else
                {
                    fmtMsg = va("maps/mp/gametypes/%s.txt", pszFileName->token);
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

// Decomp: CoDMPServer.c:425490
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

// Decomp: CoDMPServer.c:425503
bool __cdecl Scr_IsValidGameType(const char *pszGameType)
{
    return Scr_GetGameTypeNameForScript(pszGameType) != 0;
}

// Decomp: CoDMPServer.c:425509
void __cdecl Scr_LoadGameType()
{
    unsigned __int16 t;

    if ( !g_scr_data.gametype.main )
        return;
    t = Scr_ExecThread(SCRIPTINSTANCE_SERVER, g_scr_data.gametype.main, 0);
    Scr_FreeThread(t, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:425529
void __cdecl Scr_StartupGameType()
{
    unsigned __int16 callback;

    if ( !g_scr_data.gametype.startupgametype )
        return;
    callback = Scr_ExecThread(SCRIPTINSTANCE_SERVER, g_scr_data.gametype.startupgametype, 0);
    Scr_FreeThread(callback, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:425538
void __cdecl Scr_PlayerConnect(gentity_s *self)
{
    unsigned __int16 callback;

    if ( !g_scr_data.gametype.playerconnect )
        return;
    callback = Scr_ExecEntThread(self, g_scr_data.gametype.playerconnect, 0);
    Scr_FreeThread(callback, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:425547
void __cdecl Scr_PlayerDisconnect(gentity_s *self)
{
    unsigned __int16 callback;

    if ( !g_scr_data.gametype.playerdisconnect )
        return;
    callback = Scr_ExecEntThread(self, g_scr_data.gametype.playerdisconnect, 0);
    Scr_FreeThread(callback, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:425556
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

// Decomp: CoDMPServer.c:425582
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

// Decomp: CoDMPServer.c:425607
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
                int timeOffset)
{
    unsigned __int16 HitLocationString;
    char *weaponName;
    unsigned __int16 callback;

    if ( !g_scr_data.gametype.actordamage )
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
    callback = Scr_ExecEntThread(self, g_scr_data.gametype.actordamage, 0xAu);
    Scr_FreeThread(callback, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:425633
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

// Decomp: CoDMPServer.c:425657
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

// Decomp: CoDMPServer.c:425686
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

// Decomp: CoDMPServer.c:425715
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

// Decomp: CoDMPServer.c:425740
void __cdecl Scr_VoteCalled(gentity_s *self, char *command, char *param1, char *param2)
{
    Scr_AddString(param2, SCRIPTINSTANCE_SERVER);
    Scr_AddString(param1, SCRIPTINSTANCE_SERVER);
    Scr_AddString(command, SCRIPTINSTANCE_SERVER);
    Scr_Notify(self, scr_const.call_vote, 3u);
}

// Decomp: CoDMPServer.c:425749
void __cdecl Scr_PlayerVote(gentity_s *self, char *option)
{
    Scr_AddString(option, SCRIPTINSTANCE_SERVER);
    Scr_Notify(self, scr_const.vote, 1u);
}

// Decomp: CoDMPServer.c:425756
void __cdecl GScr_Shutdown()
{
    if ( level.cachedTagMat.name )
        Scr_SetString(&level.cachedTagMat.name, 0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:425763
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

// Decomp: CoDMPServer.c:425775
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
