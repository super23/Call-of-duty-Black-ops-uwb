#include "g_client_script_cmd_mp.h"
#include "g_main_mp.h"
#include <clientscript/cscr_vm.h>
#include <game/g_weapon.h>
#include <bgame/bg_weapons.h>
#include <bgame/bg_misc.h>
#include <server_mp/sv_main_mp.h>
#include <bgame/bg_weapons_util.h>
#include <bgame/bg_weapons_load_obj.h>
#include <server/sv_game.h>
#include <bgame/bg_unlockable_items.h>
#include <ui_mp/ui_gametype_custom_mp.h>
#include <ui_mp/ui_gametype_variants_mp.h>
#include <live/live_stats.h>
#include <client_mp/sv_client_mp.h>
#include <cgame/cg_weapon_options.h>
#include <clientscript/scr_const.h>
#include <server_mp/sv_bot_mp.h>
#include <bgame/bg_weapons_ammo.h>
#include <server/sv_world.h>
#include <client_mp/g_client_mp.h>
#include "g_spawn_mp.h"
#include <cgame_mp/cg_predict_mp.h>
#include "g_active_mp.h"
#include <devgui/devgui.h>
#include <client/cl_keys.h>
#include <qcommon/com_clients.h>
#include "g_utils_mp.h"
#include "g_combat_mp.h"
#include <universal/surfaceflags.h>
#include <game/bullet.h>
#include <game/actor_script_cmd.h>
#include <game/turret.h>
#include <game/g_player_corpse.h>
#include <qcommon/dobj_management.h>
#include "g_cmds_mp.h"
#include <live/live_sessions.h>
#include <server/sv_live_stats.h>
#include <bgame/bg_perks.h>
#include <server_mp/sv_init_mp.h>
#include <game/g_targets.h>
#include <turret/turret_placement.h>
#include <cgame/cg_scr_main.h>
#include <sound/snd_bank.h>
#include <live/live_sessions_win.h>

// Decomp: CoDMPServer.c:407212
static void __cdecl METHOD_NULLSUB(scr_entref_t entref)
{

}

// BE FUCKING MINDFUL OF WHITESPACE IN THE STRING (UNLIKE ME)
const BuiltinMethodDef methods[] =
{
  { "giveweapon", &PlayerCmd_giveWeapon, 0 },
  { "setblockweaponpickup", &PlayerCmd_setBlockWeaponPickup, 0 },
  { "calcweaponoptions", &PlayerCmd_calcWeaponOptions, 0 },
  { "calcplayeroptions", &PlayerCmd_calcPlayerOptions, 0 },
  { "setplayerrenderoptions", &PlayerCmd_setPlayerRenderOptions, 0 },
  { "nextplayerrenderoption", &PlayerCmd_NextPlayerRenderOption, 0 },
  { "sethighlighted", &PlayerCmd_SetHighlighted, 0 },
  { "takeweapon", &PlayerCmd_takeWeapon, 0 },
  { "isthrowinggrenade", &PlayerCmd_IsThrowingGrenade, 0 },
  // LWSS ADD FROM LATEST RETAIL MP BLOPS
  { "isfiring", &PlayerCmd_IsFiring, 0 },
  { "ismeleeing", &PlayerCmd_IsMeleeing, 0 },
  { "isswitchingweapons", &PlayerCmd_IsSwitchingWeapons, 0 },
  // LWSS END
  { "takeallweapons", &PlayerCmd_takeAllWeapons, 0 },
  // LWSS ADD FROM LATEST RETAIL MP BLOPS
  { "isinmovemode", &PlayerCmd_IsInMoveMode, 0 },
  // LWSS END
  { "getcurrentweapon", &PlayerCmd_getCurrentWeapon, 0 },
  { "getcurrentoffhand", &PlayerCmd_getCurrentOffhand, 0 },
  { "hasweapon", &PlayerCmd_hasWeapon, 0 },
  { "switchtoweapon", &PlayerCmd_switchToWeapon, 0 },
  { "switchtooffhand", &PlayerCmd_switchToOffhand, 0 },
  { "getlockonradius", &PlayerCmd_getLockOnRadius, 0 },
  { "getlockonspeed", &PlayerCmd_getLockOnSpeed, 0 },
  { "givestartammo", &PlayerCmd_giveStartAmmo, 0 },
  { "givemaxammo", &PlayerCmd_giveMaxAmmo, 0 },
  { "getfractionstartammo", &PlayerCmd_getFractionStartAmmo, 0 },
  { "getfractionmaxammo", &PlayerCmd_getFractionMaxAmmo, 0 },
  { "setorigin", &PlayerCmd_setOrigin, 0 },
  { "setvelocity", &PlayerCmd_SetVelocity, 0 },
  { "setplayerangles", &PlayerCmd_setAngles, 0 },
  { "getplayerangles", &PlayerCmd_getAngles, 0 },
  { "cameraactivate", &PlayerCmd_CameraActivate, 0 },
  { "camerasetposition", &PlayerCmd_CameraSetPosition, 0 },
  { "camerasetlookat", &PlayerCmd_CameraSetLookAt, 0 },
  { "usebuttonpressed", &PlayerCmd_useButtonPressed, 0 },
  { "changeseatbuttonpressed", &PlayerCmd_ChangeSeatButtonPressed, 0 },
  { "attackbuttonpressed", &PlayerCmd_attackButtonPressed, 0 },
  { "actionslotonebuttonpressed", &PlayerCmd_actionSlotOneButtonPressed, 0 },
  { "actionslottwobuttonpressed", &PlayerCmd_actionSlotTwoButtonPressed, 0 },
  { "actionslotthreebuttonpressed", &PlayerCmd_actionSlotThreeButtonPressed, 0 },
  { "actionslotfourbuttonpressed", &PlayerCmd_actionSlotFourButtonPressed, 0 },
  { "adsbuttonpressed", &PlayerCmd_adsButtonPressed, 0 },
  { "throwbuttonpressed", &PlayerCmd_throwButtonPressed, 0 },
  { "meleebuttonpressed", &PlayerCmd_meleeButtonPressed, 0 },
  { "jumpbuttonpressed", &PlayerCmd_jumpButtonPressed, 0 },
  { "fragbuttonpressed", &PlayerCmd_fragButtonPressed, 0 },
  {
    "secondaryoffhandbuttonpressed",
    &PlayerCmd_secondaryOffhandButtonPressed,
    0
  },
  { "playerads", &PlayerCmd_playerADS, 0 },
  { "pingplayer", &PlayerCmd_pingPlayer, 0 },
  { "setviewmodel", &PlayerCmd_SetViewmodel, 0 },
  { "getviewmodel", &PlayerCmd_GetViewmodel, 0 },
  { "setoffhandsecondaryclass", &PlayerCmd_setOffhandSecondaryClass, 0 },
  { "getoffhandsecondaryclass", &PlayerCmd_getOffhandSecondaryClass, 0 },
  { "setoffhandprimaryclass", &PlayerCmd_setOffhandPrimaryClass, 0 },
  { "beginlocationselection", &PlayerCmd_BeginLocationSelection, 0 },
  {
    "beginlocationairstrikeselection",
    &PlayerCmd_BeginLocationAirstrikeSelection,
    0
  },
  { "beginlocationmortarselection", &PlayerCmd_BeginLocationMortarSelection, 0 },
  {
    "beginlocationartilleryselection",
    &PlayerCmd_BeginLocationArtillerySelection,
    0
  },
  {
    "beginlocationcomlinkselection",
    &PlayerCmd_BeginLocationComlinkSelection,
    0
  },
  { "beginlocationnapalmselection", &PlayerCmd_BeginLocationNapalmSelection, 0 },
  { "endlocationselection", &PlayerCmd_EndLocationSelection, 0 },
  { "clientsyssetstate", &PlayerCmd_ClientSysSetState, 0 },
  { "weaponlockstart", &PlayerCmd_WeaponLockStart, 0 },
  { "weaponlockfinalize", &PlayerCmd_WeaponLockFinalize, 0 },
  { "weaponlockfree", &PlayerCmd_WeaponLockFree, 0 },
  { "weaponlocktargettooclose", &PlayerCmd_WeaponLockTargetTooClose, 0 },
  { "weaponlocknoclearance", &PlayerCmd_WeaponLockNoClearance, 0 },
  { "buttonpressed", &PlayerCmd_buttonPressedDEVONLY, 0 },
  { "giveweaponnextattachment", &PlayerCmd_giveWeaponNextAttachmentDEVONLY, 0 },
  { "givenextbaseweapon", &PlayerCmd_giveNextBaseWeaponDEVONLY, 0 },
  { "sayall", &PlayerCmd_SayAll, 0 },
  { "sayteam", &PlayerCmd_SayTeam, 0 },
  { "setspawnweapon", &PlayerCmd_setSpawnWeapon, 0 },
  { "dropitem", &PlayerCmd_dropItem, 0 },
  { "dropscavengeritem", &PlayerCmd_dropScavengerItem, 0 },
  { "hasagrenadepulledback", &PlayerCmd_hasAGrenadePulledBack, 0 },
  { "finishplayerdamage", &PlayerCmd_finishPlayerDamage, 0 },
  { "suicide", &PlayerCmd_Suicide, 0 },
  { "isinvehicle", &PlayerCmd_IsInVehicle, 0 },
  { "isremotecontrolling", &PlayerCmd_IsRemoteControlling, 0 },
  { "getvehicleoccupied", &PlayerCmd_GetVehicleOccupied, 0 },
  { "openmenu", &PlayerCmd_OpenMenu, 0 },
  { "openmenunomouse", &PlayerCmd_OpenMenuNoMouse, 0 },
  { "closemenu", &PlayerCmd_CloseMenu, 0 },
  { "closeingamemenu", &PlayerCmd_CloseInGameMenu, 0 },
  { "freezecontrols", &PlayerCmd_FreezeControls, 0 },
  // LWSS ADD FROM LATEST RETAIL MP BLOPS
  { "freezecontrolsallowlook", &PlayerCmd_FreezeControlsAllowLook, 0 },
  { "arecontrolsfrozen", &PlayerCmd_AreControlsFrozen, 0 },
  // LWSS END
  { "disableusability", &PlayerCmd_DisableUsability, 0 },
  { "enableusability", &PlayerCmd_EnableUsability, 0 },
  { "disableweapons", &PlayerCmd_DisableWeapons, 0 },
  { "enableweapons", &PlayerCmd_EnableWeapons, 0 },
  { "disabledeathstreak", &PlayerCmd_DisableDeathStreak, 0 },
  { "enabledeathstreak", &PlayerCmd_EnableDeathStreak, 0 },
  { "deathstreakactive", &PlayerCmd_DeathStreakActive, 0 },
  { "disableoffhandweapons", &PlayerCmd_DisableOffhandWeapons, 0 },
  { "enableoffhandweapons", &PlayerCmd_EnableOffhandWeapons, 0 },
  { "disableweaponcycling", &PlayerCmd_DisableWeaponCycling, 0 },
  { "enableweaponcycling", &PlayerCmd_EnableWeaponCycling, 0 },
  { "revive", &PlayerCmd_Revive, 0 },
  { "setlaststandprevweap", &PlayerCmd_SetLastStandPrevWeap, 0 },
  { "setreverb", &METHOD_NULLSUB, 0 },
  { "deactivatereverb", &METHOD_NULLSUB, 0 },
  { "setchannelvolumes", &METHOD_NULLSUB, 0 },
  { "deactivatechannelvolumes", &METHOD_NULLSUB, 0 },
  { "setweaponammoclip", &PlayerCmd_SetWeaponAmmoClip, 0 },
  { "setweaponammostock", &PlayerCmd_SetWeaponAmmoStock, 0 },
  { "getweaponammoclip", &PlayerCmd_GetWeaponAmmoClip, 0 },
  { "getweaponammostock", &PlayerCmd_GetWeaponAmmoStock, 0 },
  { "anyammoforweaponmodes", &PlayerCmd_AnyAmmoForWeaponModes, 0 },
  { "iprintln", &iclientprintln, 0 },
  { "iprintlnbold", &iclientprintlnbold, 0 },
  { "spawn", &PlayerCmd_spawn, 0 },
  { "setentertime", &PlayerCmd_setEnterTime, 0 },
  { "cloneplayer", &PlayerCmd_ClonePlayer, 0 },
  { "setclientdvar", &PlayerCmd_SetClientDvar, 0 },
  { "setclientuivisibilityflag", &PlayerCmd_SetClientUIVisibilityFlag, 0 },
  { "setclientdvars", &PlayerCmd_SetClientDvars, 0 },
  { "playclientairstrike", &PlayerCmd_PlayClientAirstrike, 0 },
  { "playclientnapalm", &PlayerCmd_PlayClientNapalm, 0 },
  { "setcurrentspectatorclient", &PlayerCmd_SetCurrentSpectatorClient, 0 },
  { "relinktoturret", &PlayerCmd_ReLinkToTurret, 0 },
  { "carryturret", &PlayerCmd_CarryTurret, 0 },
  { "stopcarryturret", &PlayerCmd_StopCarryTurret, 0 },
  { "iscarryingturret", &PlayerCmd_isCarryingTurret, 0 },
  { "canplayerplaceturret", &PlayerCmd_CanPlayerPlaceTurret, 0 },
  // LWSS ADD FROM LATEST RETAIL MP BLOPS
  { "setturrethint", &PlayerCmd_SetTurrentHint, 0 },
  // LWSS END
  { "linkguidedmissilecamera", &PlayerCmd_LinkGuidedMissileCamera, 0 },
  { "unlinkguidedmissilecamera", &PlayerCmd_UnlinkGuidedMissileCamera, 0 },
  { "playlocalsound", &ScrCmd_PlayLocalSound, 0 },
  { "stoplocalsound", &ScrCmd_StopLocalSound, 0 },
  { "istalking", &PlayerCmd_IsTalking, 0 },
  { "allowspectateteam", &PlayerCmd_AllowSpectateTeam, 0 },
  { "getguid", &PlayerCmd_GetGuid, 0 },
  { "getxuid", &PlayerCmd_GetXuid, 0 },
  { "ishost", &PlayerCmd_IsHost, 0 },
  { "islocaltohost", &PlayerCmd_IsLocalToHost, 0 },
  { "allowads", &PlayerCmd_AllowADS, 0 },
  { "allowjump", &PlayerCmd_AllowJump, 0 },
  { "allowsprint", &PlayerCmd_AllowSprint, 0 },
  { "setsprintduration", &PlayerCmd_SetSprintDuration, 0 },
  { "setsprintcooldown", &PlayerCmd_SetSprintCooldown, 0 },
  { "setspreadoverride", &PlayerCmd_SetSpreadOverride, 0 },
  { "resetspreadoverride", &PlayerCmd_ResetSpreadOverride, 0 },
  { "setactionslot", &PlayerCmd_SetActionSlot, 0 },
  { "getweaponslist", &PlayerCmd_GetWeaponsList, 0 },
  { "getweaponslistprimaries", &PlayerCmd_GetWeaponsListPrimaries, 0 },
  { "setperk", &PlayerCmd_SetPerk, 0 },
  { "hasperk", &PlayerCmd_HasPerk, 0 },
  { "getperks", &PlayerCmd_GetPerks, 0 },
  { "clearperks", &PlayerCmd_ClearPerks, 0 },
  { "unsetperk", &PlayerCmd_UnsetPerk, 0 },
  { "setrank", &PlayerCmd_SetRank, 0 },
  { "enableinvulnerability", &PlayerCmd_EnableInvulnerability, 0 },
  { "disableinvulnerability", &PlayerCmd_DisableInvulnerability, 0 },
  { "setscriptgoal", &PlayerCmd_BotSetScriptGoal, 0 },
  { "clearscriptgoal", &PlayerCmd_BotClearScriptGoal, 0 },
  { "setscriptenemy", &PlayerCmd_BotSetScriptEnemy, 0 },
  { "clearscriptenemy", &PlayerCmd_BotClearScriptEnemy, 0 },
  { "setattacker", &PlayerCmd_BotSetAttacker, 0 },
  { "pressusebutton", &PlayerCmd_BotPressUseButton, 0 },
  // LWSS ADD FROM LATEST RETAIL MP BLOPS
  { "pressattackbutton", &PlayerCmd_BotPressAttackButton, 0 },
  { "getlookaheaddist", &PlayerCmd_GetLookaheadDist, 0 },
  { "getlookaheaddir", &PlayerCmd_GetLookaheadDir, 0 },
  { "getthreat", &PlayerCmd_GetThreat, 0 },
  { "hasscriptgoal", &PlayerCmd_HasScriptGoal, 0 },
  // LWSS END
  { "issplitscreen", &PlayerCmd_IsSplitscreen, 0 },
  { "isplayeronsamemachine", &PlayerCmd_IsPlayerOnSameMachine, 0 }
};



// Decomp: CoDMPServer.c:374442
void __cdecl PlayerCmd_setBlockWeaponPickup(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    gentity_s *pSelf;
    char *weaponName;
    bool blockWeaponPickup;
    unsigned int weaponIndex;
    playerState_s *ps;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        156,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2 )
        Scr_Error("setBlockWeaponPickup( <weaponname>, <blockWeaponPickup> ) takes 2 parameters", 0);
    ps = &pSelf->client->ps;
    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    blockWeaponPickup = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER) != 0;
    if ( !BG_PlayerWeaponSetBlockPickupWeapon(ps, weaponIndex, blockWeaponPickup) )
    {
        fmtMsg2 = va("Cannot set blocked weapon pickup, player does not have %s", weaponName);
        Scr_Error(fmtMsg2, 0);
    }
}

// Decomp: CoDMPServer.c:374491
int __cdecl BG_PlayerWeaponSetBlockPickupWeapon(playerState_s *ps, unsigned int weaponIndex, bool blockPickupWeapon)
{
    PlayerHeldWeapon *heldWeapon;

    if ( !ps && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../bgame/bg_weapons.h", 406, 0, "%s", "ps") )
        __debugbreak();
    heldWeapon = BG_GetHeldWeapon(ps, weaponIndex);
    if ( !heldWeapon )
        return 0;
    heldWeapon->blockWeaponPickup = blockPickupWeapon;
    return 1;
}

// Decomp: CoDMPServer.c:374514
void __cdecl PlayerCmd_giveWeapon(scr_entref_t entref)
{
    const char *fmtMsg;
    int weaponModel;
    gentity_s *pSelf;
    char *weaponName;
    bool hadWeapon;
    char svcmd[68];
    renderOptions_s weaponOptions;
    int weaponIndex;
    const WeaponDef *weapDef;
    playerState_s *ps;

    weaponOptions.i = 0;
    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        186,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    if ( !pSelf->client
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    192,
                    0,
                    "%s",
                    "pSelf->client") )
    {
        __debugbreak();
    }
    ps = &pSelf->client->ps;
    hadWeapon = BG_PlayerHasWeapon(ps, weaponIndex);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 1 )
    {
        (weaponModel) = 0;
    }
    else
    {
        weapDef = BG_GetWeaponDef(weaponIndex);
        weaponModel = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
        if ( (unsigned int)weaponModel < 0x100 )
        {
            if ( !weapDef->gunXModel[weaponModel] )
                (weaponModel) = 0;
        }
        else
        {
            (weaponModel) = 0;
        }
        if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 2 )
            weaponOptions.i = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
    }
    if ( G_GivePlayerWeapon(&pSelf->client->ps, weaponIndex, weaponModel, weaponOptions) )
    {
        _snprintf(svcmd, 0x40u, "%c \"%i\"", 74, 1);
        svcmd[63] = 0;
        SV_GameSendServerCommand(pSelf - g_entities, SV_CMD_CAN_IGNORE, svcmd);
        G_InitializeAmmo(pSelf, weaponIndex, weaponModel, hadWeapon);
    }
}

// Decomp: CoDMPServer.c:374599
void __cdecl G_InitializeAmmo(gentity_s *pSelf, unsigned int weaponIndex, unsigned __int8 weaponModel, int hadWeapon)
{
    int numWeapons;
    int startWeapon;
    const WeaponVariantDef *weapVariantDef;
    int ammoGive;
    const WeaponDef *weapDef;

    startWeapon = weaponIndex;
    numWeapons = BG_GetNumWeapons();
    weapVariantDef = BG_GetWeaponVariantDef(weaponIndex);
    weapDef = BG_GetWeaponDef(weaponIndex);
    if ( !weapDef
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    116,
                    0,
                    "%s",
                    "weapDef") )
    {
        __debugbreak();
    }
    if ( !pSelf
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    118,
                    0,
                    "%s",
                    "pSelf") )
    {
        __debugbreak();
    }
    if ( !pSelf->client
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    119,
                    0,
                    "%s",
                    "pSelf->client") )
    {
        __debugbreak();
    }
    do
    {
        ammoGive = G_GetNeededStartAmmo(pSelf, weaponIndex);
        if ( ammoGive <= 0 )
        {
            if ( !hadWeapon )
                Fill_Clip(&pSelf->client->ps, weaponIndex);
        }
        else
        {
            Add_Ammo(pSelf, weaponIndex, weaponModel, ammoGive, hadWeapon == 0);
        }
        if ( !hadWeapon && weapDef->bDualWield )
            Fill_Clip(&pSelf->client->ps, weaponIndex);
        if ( weapDef->bDualWield )
            weaponIndex = weapDef->dualWieldWeaponIndex;
        else
            weaponIndex = weapVariantDef->altWeaponIndex;
        weapVariantDef = BG_GetWeaponVariantDef(weaponIndex);
        weapDef = BG_GetWeaponDef(weaponIndex);
        if ( !weapVariantDef
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        140,
                        0,
                        "%s",
                        "weapVariantDef") )
        {
            __debugbreak();
        }
        if ( !weapDef
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        141,
                        0,
                        "%s",
                        "weapDef") )
        {
            __debugbreak();
        }
        if ( --numWeapons < 0
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        144,
                        0,
                        "%s\n\t(weapVariantDef->szDisplayName) = %s",
                        "(numWeapons >= 0)",
                        weapVariantDef->szDisplayName) )
        {
            __debugbreak();
        }
    }
    while ( weaponIndex
             && weaponIndex != startWeapon
             && numWeapons >= 0
             && (BG_PlayerHasWeapon(&pSelf->client->ps, weaponIndex) || weapDef->bDualWield) );
}

// Decomp: CoDMPServer.c:374706
int __cdecl G_GetNeededStartAmmo(gentity_s *pSelf, unsigned int weaponIndex)
{
    int StartAmmo; // esi
    const WeaponVariantDef *heldWeapVarDef;
    int slot;
    const WeaponVariantDef *weapVarDef;
    int applicableOwnedAmmoa;
    int applicableOwnedAmmo;
    unsigned int heldWeapIndex;
    gclient_s *ps;

    if ( !pSelf
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp", 67, 0, "%s", "pSelf") )
    {
        __debugbreak();
    }
    if ( !pSelf->client
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    68,
                    0,
                    "%s",
                    "pSelf->client") )
    {
        __debugbreak();
    }
    weapVarDef = BG_GetWeaponVariantDef(weaponIndex);
    ps = pSelf->client;
    applicableOwnedAmmoa = BG_GetAmmoNotInClipForAmmoIndex(&ps->ps, weapVarDef->iAmmoIndex);
    applicableOwnedAmmo = applicableOwnedAmmoa + BG_GetAmmoInClipForWeaponDef(&ps->ps, weapVarDef);
    for ( slot = 0; slot < 15; ++slot )
    {
        heldWeapIndex = ps->ps.heldWeapons[slot].weapon;
        if ( heldWeapIndex )
        {
            heldWeapVarDef = BG_GetWeaponVariantDef(heldWeapIndex);
            if ( heldWeapVarDef->iAmmoIndex == weapVarDef->iAmmoIndex && weapVarDef != heldWeapVarDef )
            {
                StartAmmo = BG_GetStartAmmo(heldWeapIndex);
                applicableOwnedAmmo -= StartAmmo - BG_GetAmmoInClipForWeaponDef(&ps->ps, heldWeapVarDef);
            }
        }
    }
    if ( applicableOwnedAmmo < 0 )
        applicableOwnedAmmo = 0;
    return BG_GetStartAmmo(weaponIndex) - applicableOwnedAmmo;
}

// Decomp: CoDMPServer.c:374763
int __cdecl BG_GetStartAmmo(unsigned int weaponIndex)
{
    const WeaponDef *weapDef;

    weapDef = BG_GetWeaponDef(weaponIndex);
    if ( weapDef->ammoCountClipRelative )
        return weapDef->iStartAmmo * BG_GetClipSize(weaponIndex);
    else
        return weapDef->iStartAmmo;
}

// Decomp: CoDMPServer.c:374777
void __cdecl PlayerCmd_giveNextBaseWeaponDEVONLY(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        229,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( !pSelf->client
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    230,
                    0,
                    "%s",
                    "pSelf->client") )
    {
        __debugbreak();
    }
}

// Decomp: CoDMPServer.c:374823
void __cdecl WeaponComponentListAddAllPoints(WeaponComponentList *componentList)
{
    int point;
    int compIdx;
    WeaponComponentList tempList;

    memset((unsigned __int8 *)&tempList, 0, sizeof(tempList));
    memcpy(
        &tempList.components[tempList.numComponents++],
        componentList->components,
        sizeof(tempList.components[tempList.numComponents++]));
    for ( point = 1; point < 5; ++point )
    {
        tempList.components[tempList.numComponents].attachment = ATTACHMENT_NONE;
        tempList.components[tempList.numComponents].attachmentPoint = ATTACHMENT_POINT_NONE;
        for ( compIdx = 0; compIdx < componentList->numComponents; ++compIdx )
        {
            if ( componentList->components[compIdx].attachmentPoint == point )
            {
                memcpy(
                    &tempList.components[tempList.numComponents],
                    &componentList->components[compIdx],
                    sizeof(tempList.components[tempList.numComponents]));
                break;
            }
        }
        ++tempList.numComponents;
    }
    memcpy(
        &tempList.components[tempList.numComponents++],
        &componentList->components[componentList->numComponents - 1],
        sizeof(tempList.components[tempList.numComponents++]));
    if ( tempList.numComponents != 6
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    272,
                    0,
                    "%s",
                    "tempList.numComponents == 6") )
    {
        __debugbreak();
    }
    memcpy(componentList, &tempList, sizeof(WeaponComponentList));
}

// Decomp: CoDMPServer.c:374870
void __cdecl WeaponComponentListToName(const WeaponComponentList *componentList, char *name, int size)
{
    int compIdx;

    *name = 0;
    if ( componentList->numComponents )
    {
        I_strncat(name, size, componentList->components[0].name);
        for ( compIdx = 1; compIdx < componentList->numComponents; ++compIdx )
        {
            if ( componentList->components[compIdx].type == WEAPON_COMPONENT_UNKNOWN
                || componentList->components[compIdx].attachmentPoint && componentList->components[compIdx].attachment )
            {
                if ( componentList->components[compIdx].name[0] )
                {
                    I_strncat(name, size, "_");
                    I_strncat(name, size, componentList->components[compIdx].name);
                }
            }
        }
    }
}

// Decomp: CoDMPServer.c:374894
void __cdecl PlayerCmd_giveWeaponNextAttachmentDEVONLY(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *AttachmentName;
    eAttachment attachment;
    int itemAttachmentNum;
    int weaponIndex;
    int i;
    gentity_s *pSelf;
    char buffer[64];
    char *weaponName;
    int componentIndex;
    char svcmd[68];
    int weaponStatItemIndex;
    int originalWeapon;
    const char *attachPointString;
    WeaponComponentList weaponComponentList;
    int numAttachmentsForWeapon;
    eAttachment currentAttachment;
    int newAttachmentIndex;
    playerState_s *ps;
    const WeaponVariantDef *weaponVariantDef;
    eAttachmentPoint point;

    weaponName = buffer;
    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        314,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( !pSelf->client
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    315,
                    0,
                    "%s",
                    "pSelf->client") )
    {
        __debugbreak();
    }
    ps = &pSelf->client->ps;
    weaponVariantDef = BG_GetWeaponVariantDef(ps->weapon);
    originalWeapon = ps->weapon;
    weaponStatItemIndex = BG_UnlockablesGetItemIndexforWeapon(ps->weapon);
    if ( weaponStatItemIndex != -1 )
    {
        attachPointString = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        point = BG_GetAttachmentPointIndex(attachPointString);
        if ( point )
        {
            BG_WeaponNameToComponentList(weaponVariantDef->szInternalName, &weaponComponentList);
            BG_WeaponComponentListIdentify(&weaponComponentList);
            WeaponComponentListAddAllPoints(&weaponComponentList);
            componentIndex = point;
            currentAttachment = weaponComponentList.components[point].attachment;
            newAttachmentIndex = BG_UnlockablesGetItemAttachmentIndexForAttachmentPoint(
                                                         weaponStatItemIndex,
                                                         currentAttachment,
                                                         point);
            numAttachmentsForWeapon = BG_UnlockablesGetNumItemAttachmentsWithAttachPoint(weaponStatItemIndex, point);
            for ( i = 0; i < numAttachmentsForWeapon; ++i )
            {
                newAttachmentIndex = (newAttachmentIndex + 1) % numAttachmentsForWeapon;
                itemAttachmentNum = BG_UnlockablesGetItemAttachmentNumWithAttachPoint(
                                                            weaponStatItemIndex,
                                                            newAttachmentIndex,
                                                            point);
                attachment = BG_UnlockablesGetItemAttachment(weaponStatItemIndex, itemAttachmentNum);
                AttachmentName = BG_GetAttachmentName(attachment);
                I_strncpyz(weaponComponentList.components[componentIndex].name, AttachmentName, 32);
                weaponComponentList.components[componentIndex].attachment = attachment;
                WeaponComponentListToName(&weaponComponentList, weaponName, 64);
                weaponIndex = BG_GetWeaponIndexForName(weaponName);
                if ( weaponIndex )
                {
                    if ( G_GivePlayerWeapon(&pSelf->client->ps, weaponIndex, 0, 0) )
                    {
                        if ( originalWeapon != weaponIndex )
                            BG_TakePlayerWeapon(ps, originalWeapon);
                        _snprintf(svcmd, 0x40u, "%c \"%i\"", 74, 1);
                        svcmd[63] = 0;
                        SV_GameSendServerCommand(pSelf - g_entities, SV_CMD_CAN_IGNORE, svcmd);
                        G_InitializeAmmo(pSelf, weaponIndex, 0, 0);
                        G_SelectWeaponIndex(pSelf->s.number, weaponIndex);
                    }
                    return;
                }
            }
        }
    }
}

// Decomp: CoDMPServer.c:375008
void __cdecl PlayerCmd_calcWeaponOptions(scr_entref_t entref)
{
    const char *fmtMsg;
    VariableUnion varUnion;
    VariableUnion varUnion2;
    const char *CustomClassNameForInt;
    VariableUnion varUnion3;
    VariableUnion varUnion4;
    const char *tmp7;
    const char *tmp8;
    const char *tmp9;
    const char *tmp10;
    const char *tmp11;
    const char *tmp12;
    char tmp13;
    char tmp14;
    char tmp15;
    char tmp16;
    char tmp17;
    char StatFromPlayer;
    const char *ops[3];
    bool primary;
    gentity_s *pSelf;
    renderOptions_s weaponOptions;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        423,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( !pSelf->client
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    424,
                    0,
                    "%s",
                    "pSelf->client") )
    {
        __debugbreak();
    }
    if ( pSelf->s.number >= 32
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    425,
                    0,
                    "%s",
                    "pSelf->s.number < (1 << RENDER_OPTIONS_PLAYER_BITS)") )
    {
        __debugbreak();
    }
    weaponOptions.i = 0;
    weaponOptions.i = (((unsigned __int8)pSelf->s.number + 1) & 0x1F) << 21;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 2 )
    {
        varUnion.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        if ( !BG_UnlockablesGetCustomClassNameForInt((customClass_t)varUnion.intValue) )
            return;
        ops[0] = "cacLoadouts";
        varUnion2.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        CustomClassNameForInt = BG_UnlockablesGetCustomClassNameForInt((customClass_t)varUnion2.intValue);
        ops[1] = va("%s", CustomClassNameForInt);
        ops[2] = 0;
        if ( UI_Gametype_IsUsingCustom() && Dvar_GetInt(custom_class_mode) )
        {
            varUnion3.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
            ops[1] = va("%d", varUnion3.intValue + 1);
        }
        varUnion4.intValue = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
        primary = varUnion4.intValue == 0;
        if ( varUnion4.intValue )
            tmp12 = "secondarycamo";
        else
            tmp12 = "primarycamo";
        ops[2] = tmp12;
        StatFromPlayer = PlayerCmd_GetStatFromPlayer(pSelf->s.number, ops, 3);
        weaponOptions.i = StatFromPlayer & 0x3F | weaponOptions.i & 0xFFFFFFC0;
        if ( primary )
            tmp11 = "primarylens";
        else
            tmp11 = "secondarylens";
        ops[2] = tmp11;
        tmp17 = PlayerCmd_GetStatFromPlayer(pSelf->s.number, ops, 3);
        weaponOptions.i = ((tmp17 & 0xF) << 6) | weaponOptions.i & 0xFFFFFC3F;
        if ( primary )
            tmp10 = "primaryreticle";
        else
            tmp10 = "secondaryreticle";
        ops[2] = tmp10;
        tmp16 = PlayerCmd_GetStatFromPlayer(pSelf->s.number, ops, 3);
        weaponOptions.i = ((tmp16 & 0x3F) << 10) | weaponOptions.i & 0xFFFF03FF;
        if ( primary )
            tmp9 = "primaryreticlecolor";
        else
            tmp9 = "secondaryreticlecolor";
        ops[2] = tmp9;
        tmp15 = PlayerCmd_GetStatFromPlayer(pSelf->s.number, ops, 3);
        weaponOptions.i = ((tmp15 & 7) << 16) | weaponOptions.i & 0xFFF8FFFF;
        if ( primary )
            tmp8 = "primarytag";
        else
            tmp8 = "secondarytag";
        ops[2] = tmp8;
        tmp14 = PlayerCmd_GetStatFromPlayer(pSelf->s.number, ops, 3);
        weaponOptions.i = ((tmp14 & 1) << 20) | weaponOptions.i & 0xFFEFFFFF;
        if ( primary )
            tmp7 = "primaryemblem";
        else
            tmp7 = "secondaryemblem";
        ops[2] = tmp7;
        tmp13 = PlayerCmd_GetStatFromPlayer(pSelf->s.number, ops, 3);
        weaponOptions.i = ((tmp13 & 1) << 19) | weaponOptions.i & 0xFFF7FFFF;
    }
    else if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 2 )
    {
        Scr_Error(
            "calcWeaponOptions takes 5 arguments: <camo> <lens> <reticle> <tag> <emblem>\n"
            "                                         or 2 argument: <custom class slot> <primary weapon=0 | secondary weapon=1>",
            0);
    }
    else
    {
        weaponOptions.i = Scr_GetInt(0, SCRIPTINSTANCE_SERVER) & 0x3F | weaponOptions.i & 0xFFFFFFC0;
        weaponOptions.i = ((Scr_GetInt(1u, SCRIPTINSTANCE_SERVER) & 0xF) << 6) | weaponOptions.i & 0xFFFFFC3F;
        if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 2 )
        {
            weaponOptions.i = ((Scr_GetInt(2u, SCRIPTINSTANCE_SERVER) & 0x3F) << 10)
                                            | weaponOptions.i & 0xFFFF03FF;
            if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 3 )
            {
                weaponOptions.i = ((Scr_GetInt(3u, SCRIPTINSTANCE_SERVER) & 1) << 20) | weaponOptions.i & 0xFFEFFFFF;
                if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 4 )
                    weaponOptions.i = ((Scr_GetInt(4u, SCRIPTINSTANCE_SERVER) & 1) << 19)
                                                    | weaponOptions.i & 0xFFF7FFFF;
            }
        }
    }
    Scr_AddInt(weaponOptions.i, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:375167
unsigned int __cdecl PlayerCmd_GetStatFromPlayer(unsigned int clientNum, const char **operands, int nOperands)
{
    int i;
    ddlState_t searchState;
    int statVal;

    if ( UI_Gametype_IsUsingCustom() && Dvar_GetInt(custom_class_mode) )
        return GetCustomClassLoadoutItemForClassName(0, operands[1], operands[2]);
    searchState = *LiveStats_GetRootDDLState();
    for ( i = 0; i < nOperands; ++i )
    {
        if ( searchState.member && searchState.member->arraySize > 1 && searchState.member->enumIndex == -1 )
        {
            DDL_PrintError("DDL: array index (integer) expected. Received a string instead.");
            return -1;
        }
        if ( !DDL_MoveToName(&searchState, &searchState, operands[i]) )
        {
            DDL_PrintError("DDL: Could not find member name.");
            return -1;
        }
    }
    if ( !searchState.member
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    408,
                    0,
                    "%s",
                    "searchState.member") )
    {
        __debugbreak();
    }
    statVal = 0;
    if ( searchState.member )
        return SV_GetClientDIntStat(clientNum, &searchState);
    return statVal;
}

// Decomp: CoDMPServer.c:375215
void __cdecl PlayerCmd_calcPlayerOptions(scr_entref_t entref)
{
    const char *fmtMsg;
    VariableUnion varUnion;
    VariableUnion varUnion2;
    const char *CustomClassNameForInt;
    VariableUnion varUnion3;
    unsigned __int8 StatFromPlayer;
    const char *ops[3];
    gentity_s *pSelf;
    renderOptions_s weaponOptions;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        487,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( !pSelf->client
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    488,
                    0,
                    "%s",
                    "pSelf->client") )
    {
        __debugbreak();
    }
    weaponOptions.i = 0;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        varUnion.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        if ( !BG_UnlockablesGetCustomClassNameForInt((customClass_t)varUnion.intValue) )
            return;
        ops[0] = "cacLoadouts";
        varUnion2.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        CustomClassNameForInt = BG_UnlockablesGetCustomClassNameForInt((customClass_t)varUnion2.intValue);
        ops[1] = va("%s", CustomClassNameForInt);
        ops[2] = 0;
        if ( UI_Gametype_IsUsingCustom() )
        {
            if ( Dvar_GetInt(custom_class_mode) )
            {
                varUnion3.intValue = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
                ops[1] = va("%d", varUnion3.intValue + 1);
            }
        }
        ops[2] = "facepaintPattern";
        StatFromPlayer = PlayerCmd_GetStatFromPlayer(pSelf->s.number, ops, 3);
        weaponOptions.i = (StatFromPlayer << 26) | weaponOptions.i & 0x3FFFFFF;
    }
    else if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 1 )
    {
        Scr_Error(
            "calcPlayerOptions takes 2 arguments: <facepaintPattern> <facepaintColor>\n"
            "                                                        or 1 argument: <custom class slot>",
            0);
    }
    else
    {
        weaponOptions.i = (Scr_GetInt(0, SCRIPTINSTANCE_SERVER) << 26) | weaponOptions.i & 0x3FFFFFF;
    }
    Scr_AddInt(weaponOptions.i, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:375302
void __cdecl PlayerCmd_setPlayerRenderOptions(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        535,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( !pSelf->client
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    536,
                    0,
                    "%s",
                    "pSelf->client") )
    {
        __debugbreak();
    }
    pSelf->client->ps.renderOptions.i = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:375347
void __cdecl PlayerCmd_NextPlayerRenderOption(scr_entref_t entref)
{
    const char *fmtMsg;
    char *String;
    char *tmp3;
    char *tmp4;
    char *tmp5;
    char *tmp6;
    char *tmp7;
    char *tmp8;
    char *tmp9;
    const char *fmtMsg2;
    char tmp11;
    int tmp12;
    int tmp13;
    int tmp14;
    char tmp15;
    int tmp16;
    int tmp17;
    int j;
    renderOptions_s *p_renderOptions;
    int k;
    renderOptions_s *p_options;
    int m;
    renderOptions_s *tmp23;
    int n;
    renderOptions_s *tmp25;
    int ii;
    renderOptions_s *tmp27;
    int jj;
    renderOptions_s *tmp29;
    int i;
    renderOptions_s *renderOptions;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        564,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( !pSelf->client
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    565,
                    0,
                    "%s",
                    "pSelf->client") )
    {
        __debugbreak();
    }
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 2 )
    {
        if ( CG_IsValidRenderOption(pSelf->client->ps.renderOptions) )
        {
            String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
            if ( I_stricmp(String, "camo") )
            {
                tmp3 = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
                if ( I_stricmp(tmp3, "lens") )
                {
                    tmp4 = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
                    if ( I_stricmp(tmp4, "reticle") )
                    {
                        tmp5 = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
                        if ( I_stricmp(tmp5, "reticle_color") )
                        {
                            tmp6 = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
                            if ( I_stricmp(tmp6, "emblem") )
                            {
                                tmp7 = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
                                if ( I_stricmp(tmp7, "tag") )
                                {
                                    tmp8 = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
                                    if ( I_stricmp(tmp8, "facepaint_pattern") )
                                    {
                                        tmp9 = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
                                        fmtMsg2 = va("NextPlayerRenderOption: Unknown option name: '%s'", tmp9);
                                        Scr_Error(fmtMsg2, 0);
                                    }
                                    else
                                    {
                                        p_renderOptions = &pSelf->client->ps.renderOptions;
                                        for ( j = 0; j < 64; ++j )
                                        {
                                            tmp11 = (unsigned __int8)HIBYTE(p_renderOptions->i) >> 2;
                                            p_renderOptions->i = (((tmp11 + (Scr_GetInt(1u, SCRIPTINSTANCE_SERVER) != 1 ? -1 : 1))
                                                                                     & 0x3F) << 26)
                                                                                 | p_renderOptions->i & 0x3FFFFFF;
                                            if ( CG_IsValidRenderOption((renderOptions_s)p_renderOptions->i) )
                                                break;
                                        }
                                    }
                                }
                                else
                                {
                                    p_options = &BG_GetHeldWeapon(&pSelf->client->ps, pSelf->client->ps.weapon)->options;
                                    for ( k = 0; k < 64; ++k )
                                    {
                                        tmp12 = (p_options->i >> 20) & 1;
                                        p_options->i = ((((_BYTE)tmp12 + (Scr_GetInt(1u, SCRIPTINSTANCE_SERVER) != 1 ? -1 : 1)) & 1) << 20)
                                                                 | p_options->i & 0xFFEFFFFF;
                                        if ( CG_IsValidRenderOption((renderOptions_s)p_options->i) )
                                            break;
                                    }
                                    if ( ((p_options->i >> 21) & 0x1F) == 0 )
                                        p_options->i = ((((unsigned __int8)pSelf->s.number + 1) & 0x1F) << 21) | p_options->i & 0xFC1FFFFF;
                                }
                            }
                            else
                            {
                                tmp23 = &BG_GetHeldWeapon(&pSelf->client->ps, pSelf->client->ps.weapon)->options;
                                for ( m = 0; m < 64; ++m )
                                {
                                    tmp13 = (tmp23->i >> 19) & 1;
                                    tmp23->i = ((((_BYTE)tmp13 + (Scr_GetInt(1u, SCRIPTINSTANCE_SERVER) != 1 ? -1 : 1)) & 1) << 19)
                                                 | tmp23->i & 0xFFF7FFFF;
                                    if ( CG_IsValidRenderOption((renderOptions_s)tmp23->i) )
                                        break;
                                }
                                if ( ((tmp23->i >> 21) & 0x1F) == 0 )
                                    tmp23->i = ((((unsigned __int8)pSelf->s.number + 1) & 0x1F) << 21) | tmp23->i & 0xFC1FFFFF;
                            }
                        }
                        else
                        {
                            tmp25 = &BG_GetHeldWeapon(&pSelf->client->ps, pSelf->client->ps.weapon)->options;
                            for ( n = 0; n < 64; ++n )
                            {
                                tmp14 = HIWORD(tmp25->i) & 7;
                                tmp25->i = ((((_BYTE)tmp14 + (Scr_GetInt(1u, SCRIPTINSTANCE_SERVER) != 1 ? -1 : 1)) & 7) << 16)
                                             | tmp25->i & 0xFFF8FFFF;
                                if ( CG_IsValidRenderOption((renderOptions_s)tmp25->i) )
                                    break;
                            }
                        }
                    }
                    else
                    {
                        tmp27 = &BG_GetHeldWeapon(&pSelf->client->ps, pSelf->client->ps.weapon)->options;
                        for ( ii = 0; ii < 64; ++ii )
                        {
                            tmp15 = (unsigned __int8)HIBYTE(LOWORD(tmp27->i)) >> 2;
                            tmp27->i = (((tmp15 + (Scr_GetInt(1u, SCRIPTINSTANCE_SERVER) != 1 ? -1 : 1)) & 0x3F) << 10)
                                         | tmp27->i & 0xFFFF03FF;
                            if ( CG_IsValidRenderOption((renderOptions_s)tmp27->i) )
                                break;
                        }
                    }
                }
                else
                {
                    tmp29 = &BG_GetHeldWeapon(&pSelf->client->ps, pSelf->client->ps.weapon)->options;
                    for ( jj = 0; jj < 64; ++jj )
                    {
                        tmp16 = (tmp29->i >> 6) & 0xF;
                        tmp29->i = ((((_BYTE)tmp16 + (Scr_GetInt(1u, SCRIPTINSTANCE_SERVER) != 1 ? -1 : 1)) & 0xF) << 6)
                                     | tmp29->i & 0xFFFFFC3F;
                        if ( CG_IsValidRenderOption((renderOptions_s)tmp29->i) )
                            break;
                    }
                }
            }
            else
            {
                renderOptions = &BG_GetHeldWeapon(&pSelf->client->ps, pSelf->client->ps.weapon)->options;
                for ( i = 0; i < 64; ++i )
                {
                    tmp17 = renderOptions->i & 0x3F;
                    renderOptions->i = ((_BYTE)tmp17 + (Scr_GetInt(1u, SCRIPTINSTANCE_SERVER) != 1 ? -1 : 1)) & 0x3F
                                                     | renderOptions->i & 0xFFFFFFC0;
                    if ( CG_IsValidRenderOption((renderOptions_s)renderOptions->i) )
                        break;
                }
            }
        }
        else
        {
            Com_Printf(0, "NextPlayerRenderOption: player's render options is invalid\n");
        }
    }
    else
    {
        Scr_Error("NextPlayerRenderOption takes 2 arguments: <optionName> <forward>", 0);
    }
}

// Decomp: CoDMPServer.c:375547
void __cdecl PlayerCmd_SetHighlighted(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        624,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( !pSelf->client
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    625,
                    0,
                    "%s",
                    "pSelf->client") )
    {
        __debugbreak();
    }
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("SetHighlighted takes 1 argument: <true/false>", 0);
}

// LWSS ADD
void PlayerCmd_IsFiring(scr_entref_t entref)
{
    gentity_s *pSelf;

    if (entref.classnum)
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        iassert(entref.entnum < MAX_GENTITIES);
        pSelf = &g_entities[entref.entnum];
        if (!pSelf->client)
        {
            Scr_ObjectError(va("entity %i is not a player", entref.entnum), SCRIPTINSTANCE_SERVER);
        }
    }

    iassert(pSelf);
    iassert(pSelf->client);

    weaponstate_t weapState = pSelf->client->ps.weaponstate;

    if (weapState == WEAPON_FIRING
        || weapState == WEAPON_RECHAMBERING
        || weapState == WEAPON_RELOADING_RIGHT
        || weapState == WEAPON_CONT_FIRE_OUT
        || weapState == WEAPON_MELEE_FIRE
        || weapState == WEAPON_MELEE_END
        || weapState == WEAPON_OFFHAND_INIT)
    {
        return Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        return Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

void PlayerCmd_IsInMoveMode(scr_entref_t entref)
{
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        iassert(entref.entnum < MAX_GENTITIES);
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
            Scr_ObjectError(va("entity %i is not a player", entref.entnum), SCRIPTINSTANCE_SERVER);
    }

    Scr_AddInt((pSelf->client->ps.pm_flags & 2) != 0, SCRIPTINSTANCE_SERVER);
}

void PlayerCmd_IsMeleeing(scr_entref_t entref)
{
    gentity_s *pSelf;

    if (entref.classnum)
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        iassert(entref.entnum < MAX_GENTITIES);
        pSelf = &g_entities[entref.entnum];
        if (!pSelf->client)
        {
            Scr_ObjectError(va("entity %i is not a player", entref.entnum), SCRIPTINSTANCE_SERVER);
        }
    }

    iassert(pSelf);
    iassert(pSelf->client);

    weaponstate_t weapState = pSelf->client->ps.weaponstate;

    if (weapState == WEAPON_MELEE_FIRE || weapState == WEAPON_MELEE_END || weapState == WEAPON_OFFHAND_INIT)
        return Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    else
        return Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
}

void PlayerCmd_IsSwitchingWeapons(scr_entref_t entref)
{
    gentity_s *pSelf;

    if (entref.classnum)
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        iassert(entref.entnum < MAX_GENTITIES);
        pSelf = &g_entities[entref.entnum];
        if (!pSelf->client)
        {
            Scr_ObjectError(va("entity %i is not a player", entref.entnum), SCRIPTINSTANCE_SERVER);
        }
    }

    iassert(pSelf);
    iassert(pSelf->client);

    weaponstate_t weapState = pSelf->client->ps.weaponstate;

    if (weapState == WEAPON_RAISING 
        || weapState == WEAPON_RAISING_ALTSWITCH 
        || weapState == WEAPON_DROPPING 
        || weapState == WEAPON_DROPPING_QUICK
        || weapState == WEAPON_DROPPING_ALTSWITCH)
        return Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    else
        return Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
}
// LWSS END

// Decomp: CoDMPServer.c:375593
void __cdecl PlayerCmd_IsThrowingGrenade(scr_entref_t entref)
{
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        iassert(entref.entnum < MAX_GENTITIES);
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            Scr_ObjectError(va("entity %i is not a player", entref.entnum), SCRIPTINSTANCE_SERVER);
        }
    }

    iassert(pSelf);
    iassert(pSelf->client);

    Scr_AddBool(pSelf->client->ps.weaponstate >= WEAPON_OFFHAND_INIT && pSelf->client->ps.weaponstate <= WEAPON_OFFHAND_END, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:375651
void __cdecl PlayerCmd_takeWeapon(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;
    unsigned int iWeaponIndex;
    char *pszWeaponName;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        665,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pszWeaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = G_GetWeaponIndexForName(pszWeaponName);
    BG_TakePlayerWeapon(&pSelf->client->ps, iWeaponIndex);
}

// Decomp: CoDMPServer.c:375689
void __cdecl PlayerCmd_takeAllWeapons(scr_entref_t entref)
{
    const char *fmtMsg;
    int slot;
    gentity_s *pSelf;
    unsigned int weapIndex;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        678,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pSelf->client->ps.weapon = 0;
    for ( slot = 0; slot < 15; ++slot )
    {
        weapIndex = pSelf->client->ps.heldWeapons[slot].weapon;
        if ( weapIndex )
            BG_TakePlayerWeapon(&pSelf->client->ps, weapIndex);
    }
}

// Decomp: CoDMPServer.c:375731
void __cdecl PlayerCmd_getCurrentWeapon(scr_entref_t entref)
{
    const char *fmtMsg;
    char *tmp2;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        710,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( ClientPlaying(pSelf) && pSelf->client->ps.weapon )
    {
        tmp2 = (char *)BG_WeaponName(pSelf->client->ps.weapon);
        Scr_AddString(tmp2, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddString("none", SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:375783
bool __cdecl ClientPlaying(gentity_s *pSelf)
{
    if ( !pSelf->client
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    698,
                    0,
                    "%s",
                    "pSelf->client") )
    {
        __debugbreak();
    }
    if ( pSelf->client->sess.connected == CON_DISCONNECTED
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    699,
                    0,
                    "%s",
                    "pSelf->client->sess.connected != CON_DISCONNECTED") )
    {
        __debugbreak();
    }
    return pSelf->client->sess.sessionState == SESS_STATE_PLAYING;
}

// Decomp: CoDMPServer.c:375811
void __cdecl PlayerCmd_getCurrentOffhand(scr_entref_t entref)
{
    const char *fmtMsg;
    char *tmp2;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        733,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( ClientPlaying(pSelf) && pSelf->client->ps.offHandIndex > 0 )
    {
        tmp2 = (char *)BG_WeaponName(pSelf->client->ps.offHandIndex);
        Scr_AddString(tmp2, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddString("none", SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:375861
void __cdecl PlayerCmd_setOffhandSecondaryClass(scr_entref_t entref)
{
    const char *fmtMsg;
    OffhandClass offhandClass;
    gentity_s *pSelf;
    unsigned int iWeaponIndex;
    char *pszWeaponName;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        768,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pszWeaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = BG_FindWeaponIndexForName(pszWeaponName);
    offhandClass = BG_GetWeaponDef(iWeaponIndex)->offhandClass;
    if ( offhandClass == OFFHAND_CLASS_SMOKE_GRENADE )
    {
        pSelf->client->ps.offhandSecondary = PLAYER_OFFHAND_SECONDARY_SMOKE;
    }
    else if ( offhandClass == OFFHAND_CLASS_FLASH_GRENADE )
    {
        pSelf->client->ps.offhandSecondary = PLAYER_OFFHAND_SECONDARY_FLASH;
    }
    else
    {
        Scr_Error("Invalid class to set Secondary offhand to.\n", 0);
    }
}

// Decomp: CoDMPServer.c:375912
void __cdecl PlayerCmd_setOffhandPrimaryClass(scr_entref_t entref)
{
    const char *fmtMsg;
    OffhandClass offhandClass;
    gentity_s *pSelf;
    unsigned int iWeaponIndex;
    char *pszWeaponName;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        804,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pszWeaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = BG_FindWeaponIndexForName(pszWeaponName);
    offhandClass = BG_GetWeaponDef(iWeaponIndex)->offhandClass;
    if ( offhandClass == OFFHAND_CLASS_FRAG_GRENADE )
    {
        pSelf->client->ps.offhandPrimary = PLAYER_OFFHAND_PRIMARY_FRAG;
        Scr_AddString("frag", SCRIPTINSTANCE_SERVER);
    }
    else if ( offhandClass == OFFHAND_CLASS_GEAR )
    {
        pSelf->client->ps.offhandPrimary = PLAYER_OFFHAND_PRIMARY_GEAR;
        Scr_AddString("gear", SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_Error("Invalid class to set Primary offhand to.\n", 0);
    }
}

// Decomp: CoDMPServer.c:375965
void __cdecl PlayerCmd_getOffhandSecondaryClass(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        827,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( pSelf->client->ps.offhandSecondary == PLAYER_OFFHAND_SECONDARY_FLASH )
    {
        Scr_AddConstString(scr_const.flash, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        if ( pSelf->client->ps.offhandSecondary )
        {
            if ( !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                            835,
                            0,
                            "%s",
                            "pSelf->client->ps.offhandSecondary == PLAYER_OFFHAND_SECONDARY_SMOKE") )
                __debugbreak();
        }
        Scr_AddConstString(scr_const.smoke, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:376017
void __cdecl PlayerCmd_hasWeapon(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;
    unsigned int iWeaponIndex;
    char *pszWeaponName;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        846,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pszWeaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = BG_FindWeaponIndexForName(pszWeaponName);
    if ( iWeaponIndex && BG_PlayerHasWeapon(&pSelf->client->ps, iWeaponIndex) )
        Scr_AddBool(1u, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddBool(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:376058
void __cdecl PlayerCmd_switchToWeapon(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    const client_t *tmp3;
    gentity_s *pSelf;
    int iWeaponIndex;
    char *pszWeaponName;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        864,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pszWeaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = G_GetWeaponIndexForName(pszWeaponName);
    if ( !iWeaponIndex )
    {
        fmtMsg2 = va("unknown weapon '%s'", pszWeaponName);
        Scr_ParamError(0, fmtMsg2, SCRIPTINSTANCE_SERVER);
    }
    tmp3 = &svs.clients[pSelf->s.number];
    if ( tmp3->bIsTestClient )
    {
        if ( BG_PlayerHasWeapon(&pSelf->client->ps, iWeaponIndex) )
        {
            SV_BotSwitchWeapon(tmp3, iWeaponIndex);
            Scr_AddBool(1u, SCRIPTINSTANCE_SERVER);
            return;
        }
    }
    else if ( BG_PlayerHasWeapon(&pSelf->client->ps, iWeaponIndex) )
    {
        G_SelectWeaponIndex(entref.entnum, iWeaponIndex);
        Scr_AddBool(1u, SCRIPTINSTANCE_SERVER);
        return;
    }
    Scr_AddBool(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:376124
void __cdecl PlayerCmd_switchToOffhand(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    gentity_s *pSelf;
    unsigned int iWeaponIndex;
    char *pszWeaponName;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        904,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pszWeaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = G_GetWeaponIndexForName(pszWeaponName);
    if ( !iWeaponIndex )
    {
        fmtMsg2 = va("unknown weapon '%s'", pszWeaponName);
        Scr_ParamError(0, fmtMsg2, SCRIPTINSTANCE_SERVER);
    }
    if ( BG_PlayerHasWeapon(&pSelf->client->ps, iWeaponIndex) )
    {
        G_SetEquippedOffHand(entref.entnum, iWeaponIndex);
        Scr_AddBool(1u, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddBool(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:376176
void __cdecl PlayerCmd_getLockOnRadius(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;
    const WeaponDef *weapDef;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        934,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    weapDef = BG_GetWeaponDef(pSelf->client->ps.weapon);
    Scr_AddInt(weapDef->lockOnRadius, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:376212
void __cdecl PlayerCmd_getLockOnSpeed(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;
    const WeaponDef *weapDef;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        954,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    weapDef = BG_GetWeaponDef(pSelf->client->ps.weapon);
    Scr_AddInt(weapDef->lockOnSpeed, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:376248
void __cdecl PlayerCmd_giveStartAmmo(scr_entref_t entref)
{
    const char *fmtMsg;
    unsigned __int8 PlayerWeaponModel;
    gentity_s *pSelf;
    unsigned int iWeaponIndex;
    char *pszWeaponName;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        967,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pszWeaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = G_GetWeaponIndexForName(pszWeaponName);
    if ( BG_PlayerHasWeapon(&pSelf->client->ps, iWeaponIndex) )
    {
        PlayerWeaponModel = BG_GetPlayerWeaponModel(&pSelf->client->ps, iWeaponIndex);
        G_InitializeAmmo(pSelf, iWeaponIndex, PlayerWeaponModel, 0);
    }
}

// Decomp: CoDMPServer.c:376291
void __cdecl PlayerCmd_giveMaxAmmo(scr_entref_t entref)
{
    const char *fmtMsg;
    unsigned __int8 PlayerWeaponModel;
    gentity_s *pSelf;
    char *weaponName;
    int maxWeaponAmmo;
    unsigned int weaponIndex;
    int ammoGive;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        985,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    weaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weaponIndex = G_GetWeaponIndexForName(weaponName);
    if ( BG_PlayerHasWeapon(&pSelf->client->ps, weaponIndex) )
    {
        maxWeaponAmmo = BG_GetAmmoPlayerMax(&pSelf->client->ps, weaponIndex, 0);
        BG_GetWeaponDef(weaponIndex);
        ammoGive = maxWeaponAmmo - BG_GetAmmoNotInClip(&pSelf->client->ps, weaponIndex);
        if ( ammoGive > 0 )
        {
            PlayerWeaponModel = BG_GetPlayerWeaponModel(&pSelf->client->ps, weaponIndex);
            Add_Ammo(pSelf, weaponIndex, PlayerWeaponModel, ammoGive, 0);
        }
    }
}

// Decomp: CoDMPServer.c:376342
void __cdecl PlayerCmd_getFractionStartAmmo(scr_entref_t entref)
{
    const char *fmtMsg;
    int startAmmo;
    gentity_s *pSelf;
    unsigned int iWeaponIndex;
    char *pszWeaponName;
    float fAmmoFrac;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1006,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pszWeaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = G_GetWeaponIndexForName(pszWeaponName);
    if ( BG_PlayerHasWeapon(&pSelf->client->ps, iWeaponIndex)
        && (startAmmo = BG_GetStartAmmo(iWeaponIndex), startAmmo >= 1) )
    {
        if ( BG_GetAmmoNotInClip(&pSelf->client->ps, iWeaponIndex) >= 1 )
        {
            fAmmoFrac = (float)BG_GetAmmoNotInClip(&pSelf->client->ps, iWeaponIndex) / (float)startAmmo;
            Scr_AddFloat(fAmmoFrac, SCRIPTINSTANCE_SERVER);
        }
        else
        {
            Scr_AddFloat(0.0, SCRIPTINSTANCE_SERVER);
        }
    }
    else
    {
        Scr_AddFloat(1.0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:376405
void __cdecl PlayerCmd_getFractionMaxAmmo(scr_entref_t entref)
{
    const char *fmtMsg;
    int maxAmmo;
    gentity_s *pSelf;
    unsigned int iWeaponIndex;
    char *pszWeaponName;
    float fAmmoFrac;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1038,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pszWeaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = G_GetWeaponIndexForName(pszWeaponName);
    if ( BG_PlayerHasWeapon(&pSelf->client->ps, iWeaponIndex) && (maxAmmo = BG_GetMaxAmmo(iWeaponIndex), maxAmmo >= 1) )
    {
        if ( BG_GetAmmoNotInClip(&pSelf->client->ps, iWeaponIndex) >= 1 )
        {
            fAmmoFrac = (float)BG_GetAmmoNotInClip(&pSelf->client->ps, iWeaponIndex) / (float)maxAmmo;
            Scr_AddFloat(fAmmoFrac, SCRIPTINSTANCE_SERVER);
        }
        else
        {
            Scr_AddFloat(0.0, SCRIPTINSTANCE_SERVER);
        }
    }
    else
    {
        Scr_AddFloat(1.0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:376468
void __cdecl PlayerCmd_setOrigin(scr_entref_t entref)
{
    const char *fmtMsg;
    float *tmp2;
    float *origin;
    gentity_s *pSelf;
    float vNewOrigin[3];

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1068,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    Scr_GetVector(0, vNewOrigin, SCRIPTINSTANCE_SERVER);
    SV_UnlinkEntity(pSelf);
    origin = pSelf->client->ps.origin;
    *origin = vNewOrigin[0];
    origin[1] = vNewOrigin[1];
    origin[2] = vNewOrigin[2];
    pSelf->client->ps.origin[2] = pSelf->client->ps.origin[2] + 1.0;
    pSelf->client->ps.eFlags ^= 2u;
    BG_PlayerStateToEntityState(&pSelf->client->ps, &pSelf->s, 1, 1u);
    tmp2 = pSelf->client->ps.origin;
    pSelf->r.currentOrigin[0] = *tmp2;
    pSelf->r.currentOrigin[1] = tmp2[1];
    pSelf->r.currentOrigin[2] = tmp2[2];
    SV_LinkEntity(pSelf);
}

// Decomp: CoDMPServer.c:376521
void __cdecl PlayerCmd_SetVelocity(scr_entref_t entref)
{
    const char *fmtMsg;
    float *velocity;
    gentity_s *pSelf;
    float vel[3];

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1094,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    Scr_GetVector(0, vel, SCRIPTINSTANCE_SERVER);
    velocity = pSelf->client->ps.velocity;
    *velocity = vel[0];
    velocity[1] = vel[1];
    velocity[2] = vel[2];
}

// Decomp: CoDMPServer.c:376563
void __cdecl PlayerCmd_setAngles(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;
    float angles[3];

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1117,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    Scr_GetVector(0, angles, SCRIPTINSTANCE_SERVER);
    SetClientViewAngle(pSelf, angles);
    if ( !pSelf->tagInfo )
        pSelf->r.currentAngles[0] = 0.0f;
}

// Decomp: CoDMPServer.c:376601
void __cdecl PlayerCmd_getAngles(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1129,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    Scr_AddVector(pSelf->client->ps.viewangles, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:376635
void __cdecl PlayerCmd_CameraActivate(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    gentity_s *pSelf;
    bool flags;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1150,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    flags = Scr_GetInt(0, SCRIPTINSTANCE_SERVER) != 0;
    fmtMsg2 = va("%c %i", 41, flags);
    SV_GameSendServerCommand(pSelf->s.clientNum, SV_CMD_RELIABLE, fmtMsg2);
}

// Decomp: CoDMPServer.c:376675
void __cdecl PlayerCmd_CameraSetPosition(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    const char *fmtMsg3;
    const gentity_s *ent;
    float origin[3];
    float angles[3];
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1179,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) == 4 )
    {
        Scr_GetVector(0, origin, SCRIPTINSTANCE_SERVER);
        memset(angles, 0, sizeof(angles));
        if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 1 && Scr_GetType(1u, SCRIPTINSTANCE_SERVER) == 4 )
            Scr_GetVector(1u, angles, SCRIPTINSTANCE_SERVER);
        fmtMsg2 = va("%c %i %f %f %f %f %f %f", 41, 2, origin[0], origin[1], origin[2], angles[0], angles[1], angles[2]);
        SV_GameSendServerCommand(pSelf->s.clientNum, SV_CMD_RELIABLE, fmtMsg2);
    }
    else
    {
        if ( (Scr_GetType(0, SCRIPTINSTANCE_SERVER) != 1 || Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) != 19)
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1198,
                        0,
                        "%s",
                        "Scr_GetType( 0 ) == VAR_POINTER && Scr_GetPointerType( 0 ) == VAR_ENTITY") )
        {
            __debugbreak();
        }
        ent = Scr_GetEntity(0);
        fmtMsg3 = va("%c %i %i", 41, 4, ent->s.number);
        SV_GameSendServerCommand(pSelf->s.clientNum, SV_CMD_RELIABLE, fmtMsg3);
    }
}

// Decomp: CoDMPServer.c:376741
void __cdecl PlayerCmd_CameraSetLookAt(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    const char *fmtMsg3;
    const char *fmtMsg4;
    const gentity_s *ent;
    float origin[3];
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1222,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) == 4 )
        {
            Scr_GetVector(0, origin, SCRIPTINSTANCE_SERVER);
            fmtMsg2 = va("%c %i %f %f %f", 41, 8, origin[0], origin[1], origin[2]);
            SV_GameSendServerCommand(pSelf->s.clientNum, SV_CMD_RELIABLE, fmtMsg2);
        }
        else
        {
            if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) != 1
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                            1235,
                            0,
                            "%s",
                            "Scr_GetType( 0 ) == VAR_POINTER") )
            {
                __debugbreak();
            }
            if ( Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) != 19
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                            1236,
                            0,
                            "%s",
                            "Scr_GetPointerType( 0 ) == VAR_ENTITY") )
            {
                __debugbreak();
            }
            ent = Scr_GetEntity(0);
            fmtMsg3 = va("%c %i %i", 41, 16, ent->s.number);
            SV_GameSendServerCommand(pSelf->s.clientNum, SV_CMD_RELIABLE, fmtMsg3);
        }
    }
    else
    {
        fmtMsg4 = va("%c %i", 41, 32);
        SV_GameSendServerCommand(pSelf->s.clientNum, SV_CMD_RELIABLE, fmtMsg4);
    }
}

// Decomp: CoDMPServer.c:376821
void __cdecl PlayerCmd_Revive(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1268,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pmove_t pm(&g_pmove[pSelf->client->ps.clientNum]);

    //pmove_t::pmove_t(&pm, &g_pmove[pSelf->client->ps.clientNum]);
    pSelf->client->lastStand = 0;
    pSelf->client->revive = 0;
    pSelf->client->ps.pm_type = 8;
    pSelf->health = pSelf->client->sess.maxHealth;
    pSelf->client->damage_blood = 0;

    if ( pm.cmd.button_bits.testBit(9u) )
    {
        BG_AnimScriptEvent(&g_pmove[pSelf->client->ps.clientNum], ANIM_ET_LASTSTAND_TO_CROUCH, 0, 1);
    }
    else if ( pm.cmd.button_bits.testBit(8u) )
    {
        BG_AnimScriptEvent(&g_pmove[pSelf->client->ps.clientNum], ANIM_ET_LASTSTAND_TO_PRONE, 0, 1);
    }
    else
    {
        BG_AnimScriptEvent(&g_pmove[pSelf->client->ps.clientNum], ANIM_ET_LASTSTAND_TO_STAND, 0, 1);
    }
    AssignToSmallerType<unsigned short>(&pSelf->client->ps.lastStandPrevWeapon, 0);
    pSelf->client->ps.damageTimer = 0;
    pSelf->client->ps.damageDuration = 0;
    pSelf->client->damage_fromWorld = 0;
}

// Decomp: CoDMPServer.c:376985
void __cdecl PlayerCmd_useButtonPressed(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1307,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( (pSelf->client->ps.pm_flags & 0x400000) == 0
        && (pSelf->client->button_bitsSinceLastFrame.testBit(3u)
         || pSelf->client->button_bits.testBit(3u)
         || pSelf->client->button_bitsSinceLastFrame.testBit(5u)
         || pSelf->client->button_bits.testBit(5u)) )
    {
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:377033
void __cdecl PlayerCmd_ChangeSeatButtonPressed(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1334,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if (   pSelf->client->button_bitsSinceLastFrame.testBit(0x1Cu)
        || pSelf->client->button_bits.testBit(0x1Cu) )
    {
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:377075
void __cdecl PlayerCmd_attackButtonPressed(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1354,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if (   pSelf->client->button_bitsSinceLastFrame.testBit(0)
        || pSelf->client->button_bits.testBit(0) )
    {
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:377117
void __cdecl PlayerCmd_actionSlotOneButtonPressed(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1375,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if (   pSelf->client->button_bitsSinceLastFrame.testBit(0x2Du)
        || pSelf->client->button_bits.testBit(0x2Du) )
    {
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:377159
void __cdecl PlayerCmd_actionSlotTwoButtonPressed(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1396,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( pSelf->client->button_bitsSinceLastFrame.testBit(0x2E)
        || pSelf->client->button_bits.testBit(0x2E) )
    {
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:377201
void __cdecl PlayerCmd_actionSlotThreeButtonPressed(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1417,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( pSelf->client->button_bitsSinceLastFrame.testBit(0x2F)
        || pSelf->client->button_bits.testBit(0x2F) )
    {
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:377243
void __cdecl PlayerCmd_actionSlotFourButtonPressed(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1438,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( pSelf->client->button_bitsSinceLastFrame.testBit(0x30)
        || pSelf->client->button_bits.testBit(0x30) )
    {
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:377285
void __cdecl PlayerCmd_adsButtonPressed(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1459,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( pSelf->client->button_bitsSinceLastFrame.testBit(0xB)
        || pSelf->client->button_bits.testBit(0xB) )
    {
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:377327
void __cdecl PlayerCmd_throwButtonPressed(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1480,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( pSelf->client->button_bitsSinceLastFrame.testBit(0x18)
        || pSelf->client->button_bits.testBit(0x18) )
    {
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:377369
void __cdecl PlayerCmd_meleeButtonPressed(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1501,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( pSelf->client->button_bitsSinceLastFrame.testBit(2)
        || pSelf->client->button_bits.testBit(2) )
    {
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:377411
void __cdecl PlayerCmd_jumpButtonPressed(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1522,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( pSelf->client->button_bitsSinceLastFrame.testBit(0xA)
        || pSelf->client->button_bits.testBit(0xA) )
    {
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:377453
void __cdecl PlayerCmd_fragButtonPressed(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1543,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( pSelf->client->button_bitsSinceLastFrame.testBit(0xE)
        || pSelf->client->button_bits.testBit(0xE) )
    {
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:377495
void __cdecl PlayerCmd_secondaryOffhandButtonPressed(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1565,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( pSelf->client->button_bitsSinceLastFrame.testBit(0xF)
        || pSelf->client->button_bits.testBit(0xF) )
    {
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:377537
void __cdecl PlayerCmd_buttonPressedDEVONLY(scr_entref_t entref)
{
    const char *fmtMsg;
    int LocalClientNum;
    char *button;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1595,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        if ( !g_entities[entref.entnum].client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( DevGui_IsActive() )
        goto LABEL_15;
    button = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( !button || !*button )
        Scr_ParamError(0, "usage: buttonPressed(<button name>)", SCRIPTINSTANCE_SERVER);
    LocalClientNum = Com_ControllerIndex_GetLocalClientNum(0);
    if ( !CL_IsKeyPressed(LocalClientNum, button) )
LABEL_15:
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:377584
void __cdecl PlayerCmd_playerADS(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1633,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    Scr_AddFloat(pSelf->client->ps.fWeaponPosFrac, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:377618
void __cdecl PlayerCmd_pingPlayer(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1651,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pSelf->client->ps.eFlags |= 0x400000u;
    pSelf->client->compassPingTime = level.time + 3000;
}

// Decomp: CoDMPServer.c:377653
void __cdecl PlayerCmd_SetViewmodel(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;
    char *modelName;
    int modelIndex;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1663,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    modelName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( !modelName || !*modelName )
        Scr_ParamError(0, "usage: setviewmodel(<model name>)", SCRIPTINSTANCE_SERVER);
    modelIndex = G_ModelIndex(modelName);
    if ( modelIndex != (unsigned __int16)modelIndex
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    1671,
                    0,
                    "%s",
                    "modelIndex == (modelNameIndex_t) modelIndex") )
    {
        __debugbreak();
    }
    pSelf->client->sess.viewmodelIndex = modelIndex;
}

// Decomp: CoDMPServer.c:377704
void __cdecl PlayerCmd_GetViewmodel(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;
    unsigned int modelName;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1691,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    modelName = G_ModelName(pSelf->client->sess.viewmodelIndex);
    Scr_AddConstString(modelName, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:377740
void __cdecl PlayerCmd_setSpawnWeapon(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;
    int iWeaponIndex;
    char *pszWeaponName;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1713,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pszWeaponName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = G_GetWeaponIndexForName(pszWeaponName);
    if ( BG_IsWeaponValid(&pSelf->client->ps, iWeaponIndex) )
    {
        AssignToSmallerType<unsigned short>(&pSelf->client->ps.weapon, iWeaponIndex);
        pSelf->client->ps.weaponstate = WEAPON_READY;
        BG_PlayerSetEverHadWeapon(&pSelf->client->ps, iWeaponIndex, 1);
        G_SelectWeaponIndex(entref.entnum, iWeaponIndex);
    }
}

// Decomp: CoDMPServer.c:377784
void __cdecl PlayerCmd_dropItem(scr_entref_t entref)
{
    const char *fmtMsg;
    unsigned __int8 PlayerWeaponModel;
    gentity_s *pSelf;
    unsigned int iWeaponIndex;
    const gitem_s *pItem;
    unsigned int dropTag;
    gentity_s *pDroppedItem;
    gentity_s *pDroppedItema;
    char *pszItemName;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1749,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pszItemName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    iWeaponIndex = G_GetWeaponIndexForName(pszItemName);
    if ( iWeaponIndex )
    {
        if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) <= 1 )
            dropTag = scr_const.tag_weapon_right;
        else
            dropTag = Scr_GetConstLowercaseString(1u, SCRIPTINSTANCE_SERVER);
        PlayerWeaponModel = BG_GetPlayerWeaponModel(&pSelf->client->ps, iWeaponIndex);
        pDroppedItem = Drop_Weapon(pSelf, iWeaponIndex, PlayerWeaponModel, dropTag);
        GScr_AddEntity(pDroppedItem);
    }
    else
    {
        pItem = G_FindItem(pszItemName);
        if ( pItem )
        {
            pDroppedItema = Drop_Item(pSelf, pItem, 0.0, 0);
            GScr_AddEntity(pDroppedItema);
        }
        else
        {
            GScr_AddEntity(0);
        }
    }
}

// Decomp: CoDMPServer.c:377849
void __cdecl PlayerCmd_dropScavengerItem(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;
    const gitem_s *pItem;
    gentity_s *pDroppedItem;
    char *pszItemName;

    pDroppedItem = 0;
    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1794,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pszItemName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    pItem = G_FindItem(pszItemName);
    if ( pItem )
    {
        pDroppedItem = Drop_Item(pSelf, pItem, 0.0, 0);
        pDroppedItem->s.lerp.eFlags2 |= 0x800000;
    }
    GScr_AddEntity(pDroppedItem);
}

// Decomp: CoDMPServer.c:377894
void __cdecl PlayerCmd_hasAGrenadePulledBack(scr_entref_t entref)
{
    const char *fmtMsg;
    int weaponstate;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1820,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( !pSelf->client )
        Scr_Error("Entity is not a player", 0);
    weaponstate = pSelf->client->ps.weaponstate;
    if ( weaponstate < 20 || weaponstate > 24 )
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:377935
void __cdecl PlayerCmd_BotSetScriptGoal(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    const client_t *tmp3;
    gentity_s *pSelf;
    float vGoal[3];
    float goalRadius;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1860,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    tmp3 = &svs.clients[pSelf->s.number];
    if ( tmp3->bIsTestClient )
    {
        Scr_GetVector(0, vGoal, SCRIPTINSTANCE_SERVER);
        goalRadius = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
        SV_BotSetScriptGoal(tmp3, vGoal, goalRadius);
    }
    else
    {
        fmtMsg2 = va("Illegal call to SetScriptGoal(). Player '%s' is not a bot.", pSelf->client->sess.cs.name);
        Scr_Error(fmtMsg2, 0);
    }
}

// Decomp: CoDMPServer.c:377984
void __cdecl PlayerCmd_BotClearScriptGoal(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    const client_t *tmp3;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1889,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    tmp3 = &svs.clients[pSelf->s.number];
    if ( tmp3->bIsTestClient )
    {
        SV_BotClearScriptGoal(tmp3);
    }
    else
    {
        fmtMsg2 = va("Illegal call to ClearScriptGoal(). Player '%s' is not a bot.", pSelf->client->sess.cs.name);
        Scr_Error(fmtMsg2, 0);
    }
}

// Decomp: CoDMPServer.c:378029
void __cdecl PlayerCmd_BotSetScriptEnemy(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    const client_t *tmp3;
    gentity_s *pSelf;
    gentity_s *ent;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1917,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    tmp3 = &svs.clients[pSelf->s.number];
    if ( tmp3->bIsTestClient )
    {
        ent = Scr_GetEntity(0);
        SV_BotSetScriptEnemy(tmp3, ent);
    }
    else
    {
        fmtMsg2 = va("Illegal call to SetScriptEnemy(). Player '%s' is not a bot.", pSelf->client->sess.cs.name);
        Scr_Error(fmtMsg2, 0);
    }
}

// Decomp: CoDMPServer.c:378076
void __cdecl PlayerCmd_BotClearScriptEnemy(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    const client_t *tmp3;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1944,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    tmp3 = &svs.clients[pSelf->s.number];
    if ( tmp3->bIsTestClient )
    {
        SV_BotClearScriptEnemy(tmp3);
    }
    else
    {
        fmtMsg2 = va("Illegal call to ClearScriptEnemy(). Player '%s' is not a bot.", pSelf->client->sess.cs.name);
        Scr_Error(fmtMsg2, 0);
    }
}

// Decomp: CoDMPServer.c:378121
void __cdecl PlayerCmd_BotSetAttacker(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    const client_t *tmp3;
    gentity_s *pSelf;
    gentity_s *ent;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        1972,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    tmp3 = &svs.clients[pSelf->s.number];
    if ( tmp3->bIsTestClient )
    {
        ent = Scr_GetEntity(0);
        SV_BotSetAttacker(tmp3, ent);
    }
    else
    {
        fmtMsg2 = va("Illegal call to SetAttacker(). Player '%s' is not a bot.", pSelf->client->sess.cs.name);
        Scr_Error(fmtMsg2, 0);
    }
}

// Decomp: CoDMPServer.c:378168
void __cdecl PlayerCmd_BotPressUseButton(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    const client_t *tmp3;
    gentity_s *pSelf;
    float time;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        2002,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    tmp3 = &svs.clients[pSelf->s.number];
    if ( tmp3->bIsTestClient )
    {
        time = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
        SV_BotPressUseButton(tmp3, (int)(float)(time * 1000.0));
    }
    else
    {
        fmtMsg2 = va("Illegal call to PressUseButton(). Player '%s' is not a bot.", pSelf->client->sess.cs.name);
        Scr_Error(fmtMsg2, 0);
    }
}

// Decomp: CoDMPServer.c:378216
void __cdecl PlayerCmd_finishPlayerDamage(scr_entref_t entref)
{
    const char *fmtMsg;
    char *String;
    unsigned __int16 floatValue; // ax
    char *tmp4;
    unsigned __int8 tmp5;
    const WeaponDef *WeaponDef;
    int tmp7; // esi
    gentity_s *tmp8;
    float *tmp9;
    float *damage_from;
    gentity_s *turret;
    gentity_s *tent;
    gentity_s *attacker;
    int damage;
    meansOfDeath_t mod;
    gentity_s *pSelf;
    float *dir;
    float localdir[3];
    float vDir[3];
    float vPoint[3];
    float player_yaw;
    gentity_s *tempBulletHitEntity;
    int iWeapon;
    int psTimeOffset;
    int dflags;
    int iSurfType;
    pmove_t *pm;
    float flinchYawDir;
    gentity_s *inflictor;
    hitLocation_t hitLoc;
    void (__cdecl *die)(gentity_s *, gentity_s *, gentity_s *, int, int, const int, const float *, const hitLocation_t, int);
    void (__cdecl *pain)(gentity_s *, gentity_s *, int, const float *, const int, const float *, const hitLocation_t, const int);
    float max_damage_time;
    const float *point;
    float time_per_point;

    inflictor = &g_entities[1022];
    attacker = &g_entities[1022];
    dir = 0;
    point = 0;
    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        2138,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    tempBulletHitEntity = 0;
    damage = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
    if ( damage > 0 )
    {
        pm = &g_pmove[pSelf->client->ps.clientNum];
        if ( pm->ps )
        {
            if ( Scr_GetType(0, SCRIPTINSTANCE_SERVER) && Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) == 19 )
                inflictor = Scr_GetEntity(0);
            if ( Scr_GetType(1u, SCRIPTINSTANCE_SERVER) && Scr_GetPointerType(1u, SCRIPTINSTANCE_SERVER) == 19 )
                attacker = Scr_GetEntity(1u);
            dflags = Scr_GetInt(3u, SCRIPTINSTANCE_SERVER);
            mod = (meansOfDeath_t)G_MeansOfDeathFromScriptParam(4u);
            if ( pSelf->client->lastStand
                && mod != MOD_MELEE
                && G_GetClientState(pSelf->s.number)->lastStandStartTime > level.time )
            {
                pSelf->health = 1;
                return;
            }
            String = Scr_GetString(5u, SCRIPTINSTANCE_SERVER);
            iWeapon = G_GetWeaponIndexForName(String);
            if ( Scr_GetType(6u, SCRIPTINSTANCE_SERVER) )
            {
                Scr_GetVector(6u, vPoint, SCRIPTINSTANCE_SERVER);
                point = vPoint;
            }
            if ( Scr_GetType(7u, SCRIPTINSTANCE_SERVER) )
            {
                Scr_GetVector(7u, vDir, SCRIPTINSTANCE_SERVER);
                dir = vDir;
            }
            floatValue = (unsigned __int16)Scr_GetConstString(8u, SCRIPTINSTANCE_SERVER);
            hitLoc = (hitLocation_t)G_GetHitLocationIndexFromString(floatValue);
            psTimeOffset = Scr_GetInt(9u, SCRIPTINSTANCE_SERVER);
            tmp4 = Scr_GetString(0xAu, SCRIPTINSTANCE_SERVER);
            iSurfType = Com_SurfaceTypeFromName(tmp4);
            if ( iSurfType == -1 )
                iSurfType = 7;
            if ( pSelf->client->ps.pm_type == 9 )
            {
                Com_Printf(15, "Trying to do damage to a client that is already dead");
                return;
            }
            if ( dir )
                Vec3NormalizeTo(dir, localdir);
            else
                memset(localdir, 0, sizeof(localdir));
            if ( (pSelf->flags & 1) != 0 )
            {
                ApplyKnockBack(pSelf, damage, localdir, dflags, 1);
                return;
            }
            if ( (pSelf->client->ps.eFlags & 0x4000) != 0
                && (pSelf->client->ps.eFlags2 & 0x10000000) == 0
                && pSelf->client->ps.vehiclePos >= 1
                && pSelf->client->ps.vehiclePos <= 4
                && (mod == MOD_PROJECTILE_SPLASH || mod == MOD_GRENADE_SPLASH) )
            {
                damage = (int)(float)((float)damage * vehGunnerSplashDamage->current.value);
            }
            if ( iWeapon && BG_GetWeaponDef(iWeapon)->weapType == WEAPTYPE_BULLET && IsBulletImpactMOD(mod) )
            {
                tempBulletHitEntity = G_TempEntity(vPoint, EV_BULLET_HIT);
                AssignToSmallerType<unsigned short>(&tempBulletHitEntity->s.weapon, iWeapon);
                tmp5 = DirToByte(localdir);
                tempBulletHitEntity->s.eventParm = tmp5;
                tempBulletHitEntity->s.un1.scale = hitLoc == HITLOC_HEAD;
                tempBulletHitEntity->s.un1.scale |= 4u;
                tempBulletHitEntity->s.surfType = iSurfType;
                AssignToSmallerType<short>(&tempBulletHitEntity->s.otherEntityNum, attacker->s.number);
                tempBulletHitEntity->s.index.brushmodel = scr_playerdamage_boneindex;
                tempBulletHitEntity->s.groundEntityNum = entref.entnum;
                if ( tempBulletHitEntity->r.clientMask[0]
                    && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                                2226,
                                0,
                                "%s",
                                "tempBulletHitEntity->r.clientMask[ 0 ] == 0") )
                {
                    __debugbreak();
                }
                tempBulletHitEntity->r.clientMask[(int)pSelf->client->ps.clientNum >> 5] |= 1 << (pSelf->client->ps.clientNum
                                                                                                                                                                                & 0x1F);
                WeaponDef = BG_GetWeaponDef(iWeapon);
                tent = G_TempEntity(vPoint, (entity_event_t)(WeaponDef->bRifleBullet + EV_BULLET_HIT_CLIENT_SMALL));
                AssignToSmallerType<unsigned short>(&tent->s.weapon, iWeapon);
                tent->s.surfType = iSurfType;
                AssignToSmallerType<short>(&tent->s.otherEntityNum, attacker->s.number);
                tent->s.clientNum = pSelf->client->ps.clientNum;
                tent->r.clientMask[0] = -1;
                tent->r.clientMask[(int)pSelf->client->ps.clientNum >> 5] &= ~(1 << (pSelf->client->ps.clientNum & 0x1F));
            }
            pSelf->client->damage_blood += damage;
            if ( dir )
            {
                damage_from = pSelf->client->damage_from;
                *damage_from = localdir[0];
                damage_from[1] = localdir[1];
                damage_from[2] = localdir[2];
                pSelf->client->damage_fromWorld = 0;
            }
            else
            {
                tmp9 = pSelf->client->damage_from;
                *tmp9 = pSelf->r.currentOrigin[0];
                tmp9[1] = pSelf->r.currentOrigin[1];
                tmp9[2] = pSelf->r.currentOrigin[2];
                pSelf->client->damage_fromWorld = 1;
            }
            if ( (pSelf->flags & 2) != 0 && pSelf->health - damage <= 0 )
                damage = pSelf->health - 1;
            time_per_point = player_dmgtimer_timePerPoint->current.value;
            max_damage_time = player_dmgtimer_maxTime->current.value;
            pSelf->client->ps.damageTimer += (int)(float)((float)damage * time_per_point);
            if ( dir )
            {
                flinchYawDir = vectoyaw(dir);
                player_yaw = pSelf->client->ps.viewangles[1];
                if ( player_yaw < 0.0 )
                    player_yaw = player_yaw + 360.0;
                flinchYawDir = flinchYawDir - (float)(int)player_yaw;
                if ( flinchYawDir < 0.0 )
                    flinchYawDir = flinchYawDir + 360.0;
            }
            else
            {
                flinchYawDir = 0.0f;
            }
            if ( flinchYawDir < 315.0 && flinchYawDir >= 45.0 )
            {
                if ( flinchYawDir < 135.0 || flinchYawDir >= 225.0 )
                {
                    if ( flinchYawDir < 45.0 || flinchYawDir >= 135.0 )
                        pSelf->client->ps.dmgDirection = 3;
                    else
                        pSelf->client->ps.dmgDirection = 4;
                }
                else
                {
                    pSelf->client->ps.dmgDirection = 1;
                }
            }
            else
            {
                pSelf->client->ps.dmgDirection = 2;
            }
            switch ( mod )
            {
                case MOD_BURNED:
                    pSelf->client->ps.dmgType = 1;
                    break;
                case MOD_GRENADE:
                case MOD_GRENADE_SPLASH:
                case MOD_PROJECTILE:
                case MOD_PROJECTILE_SPLASH:
                case MOD_EXPLOSIVE:
                    pSelf->client->ps.dmgType = 4;
                    break;
                case MOD_MELEE:
                case MOD_BAYONET:
                    pSelf->client->ps.dmgType = 2;
                    break;
                case MOD_HEAD_SHOT:
                    pSelf->client->ps.dmgType = 3;
                    break;
                case MOD_SUICIDE:
                    pSelf->client->ps.dmgType = 5;
                    break;
                default:
                    pSelf->client->ps.dmgType = 0;
                    break;
            }
            if ( (float)pSelf->client->ps.damageTimer > max_damage_time )
                pSelf->client->ps.damageTimer = (int)max_damage_time;
            pSelf->client->ps.damageDuration = pSelf->client->ps.damageTimer;
            pSelf->health -= damage;

            {
                PROF_SCOPED("damage notify");
                Scr_AddEntity(attacker, SCRIPTINSTANCE_SERVER);
                Scr_AddInt(damage, SCRIPTINSTANCE_SERVER);
                Scr_Notify(pSelf, scr_const.damage, 2u);
            }
            
            if (!entityHandlers[pSelf->handler].die)
                Com_Printf(1, "No die handler for player entity type %i", pSelf->handler);
            if ( pSelf->health > 0 )
            {
                PROF_SCOPED("pain");
                ApplyKnockBack(pSelf, damage, localdir, dflags, 0);
                pain = entityHandlers[pSelf->handler].pain;
                if ( pain )
                    pain(pSelf, attacker, damage, point, mod, localdir, hitLoc, iWeapon);
                goto LABEL_130;
            }
            if ( !pSelf->client->lastStand && (pSelf->client->ps.perks[1] & 0x20) != 0 && !pSelf->client->ps.waterlevel )
            {
                PROF_SCOPED("enter last stand");
                if ( (pSelf->client->ps.pm_flags & 2) != 0 )
                {
                    BG_AnimScriptEvent(&g_pmove[pSelf->client->ps.clientNum], ANIM_ET_CROUCH_TO_LASTSTAND, 0, 1);
                }
                else if ( (pSelf->client->ps.pm_flags & 1) != 0 )
                {
                    BG_AnimScriptEvent(&g_pmove[pSelf->client->ps.clientNum], ANIM_ET_PRONE_TO_LASTSTAND, 0, 1);
                }
                else
                {
                    BG_AnimScriptEvent(&g_pmove[pSelf->client->ps.clientNum], ANIM_ET_STAND_TO_LASTSTAND, 0, 1);
                }
                pSelf->client->lastStand = 1;
                tmp7 = level.time + 500;
                G_GetClientState(pSelf->s.number)->lastStandStartTime = tmp7;
                if ( (pSelf->client->ps.eFlags & 0x300) != 0
                    || pSelf->client->ps.weaponstate == 35
                    || pSelf->client->ps.weaponstate == 36
                    || pSelf->client->ps.weaponstate == 37 )
                {
                    turret = &g_entities[pSelf->client->ps.viewlocked_entNum];
                    if ( turret->s.eType == 11 )
                        G_ClientStopUsingTurret(turret);
                    pSelf->client->ps.weaponstate = WEAPON_READY;
                }
                ForceGrenadeThrow(&pSelf->client->ps);
                Scr_PlayerLastStand(pSelf, inflictor, attacker, damage, mod, iWeapon, localdir, hitLoc, psTimeOffset);
            }
            if ( pSelf->health > 0 )
                goto LABEL_130;
            {
                PROF_SCOPED("die");
                if (tempBulletHitEntity)
                    tempBulletHitEntity->s.un1.scale |= 2u;
                if (pSelf->health <= 0xFFFFFC18)
                    pSelf->health = -999;
                ApplyKnockBack(pSelf, damage, localdir, dflags, 1);
                if ((pSelf->client->ps.eFlags & 0x4000) != 0)
                {
                    if (pSelf->r.ownerNum.isDefined())
                    {
                        tmp8 = pSelf->r.ownerNum.ent();
                        if (G_GetVehicleSeatPlayerOccupies(tmp8, pSelf) > 0)
                        {
                            Scr_AddBool(0, SCRIPTINSTANCE_SERVER);
                            Scr_Notify(pSelf, scr_const.vehicle_death, 1u);
                        }
                    }
                }
                die = entityHandlers[pSelf->handler].die;
                if (die)
                    die(pSelf, inflictor, attacker, damage, mod, iWeapon, localdir, hitLoc, psTimeOffset);
            }

            if ( pSelf->r.inuse )
            {
LABEL_130:
                if ( !pSelf->r.inuse
                    && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                                2490,
                                0,
                                "%s",
                                "pSelf->r.inuse") )
                {
                    __debugbreak();
                }
                pSelf->client->ps.stats[0] = pSelf->health;
            }
        }
    }
}

// Decomp: CoDMPServer.c:378603
bool __cdecl IsBulletImpactMOD(meansOfDeath_t mod)
{
    if ( (unsigned int)mod >= MOD_NUM
        && !Assert_MyHandler(
                    "c:\\projects_pc\\cod\\codsrc\\src\\game\\../bgame/bg_public.h",
                    1924,
                    0,
                    "mod doesn't index MOD_NUM\n\t%i not in [0, %i)",
                    mod,
                    21) )
    {
        __debugbreak();
    }
    return mod == MOD_PISTOL_BULLET || mod == MOD_RIFLE_BULLET || mod == MOD_HEAD_SHOT;
}

// Decomp: CoDMPServer.c:378621
void __cdecl ForceGrenadeThrow(playerState_s *ps)
{
    const WeaponDef *weapDef;

    weapDef = BG_GetWeaponDef(ps->offHandIndex);
    if ( ps->weaponstate == WEAPON_OFFHAND_INIT || ps->weaponstate == WEAPON_OFFHAND_PREPARE || ps->weaponstate == WEAPON_OFFHAND_START)
    {
        ps->weaponstate = WEAPON_OFFHAND_HOLD;
        ps->weaponTime = weapDef->iFireTime;
        ps->weaponDelay = weapDef->iFireDelay;
        ps->weapFlags |= 2u;
        PM_StartWeaponAnim(ps, 2, 0);
    }
}

// Decomp: CoDMPServer.c:378637
void __cdecl ApplyKnockBack(gentity_s *pSelf, int damage, float *localdir, char dflags, bool allow_in_air)
{
    float *velocity;
    float tmp6;
    int knockback;
    int t;
    float knockbackMod;
    float kvel_4;
    float kvel_8;

    if ( (pSelf->flags & 8) == 0
        && (dflags & 4) == 0
        && (allow_in_air || (pSelf->client->ps.pm_flags & 0x4000) == 0 && pSelf->s.groundEntityNum != 1023) )
    {
        knockbackMod = 0.3f;
        if ( (pSelf->client->ps.pm_flags & 1) != 0 )
        {
            knockbackMod = 0.02f;
        }
        else if ( (pSelf->client->ps.pm_flags & 2) != 0 )
        {
            knockbackMod = 0.15000001f;
        }
        knockback = (int)(float)((float)damage * knockbackMod);
        if ( knockback > 60 )
            knockback = 60;
        if ( knockback )
        {
            if ( (pSelf->client->ps.eFlags & 0x300) == 0 )
            {
                tmp6 = (float)((float)knockback * g_knockback->current.value) / 250.0;
                kvel_4 = tmp6 * localdir[1];
                kvel_8 = tmp6 * localdir[2];
                velocity = pSelf->client->ps.velocity;
                *velocity = *velocity + (float)(tmp6 * *localdir);
                velocity[1] = velocity[1] + kvel_4;
                velocity[2] = velocity[2] + kvel_8;
                if ( !pSelf->client->ps.pm_time )
                {
                    t = 2 * knockback;
                    if ( 2 * knockback < 50 )
                        t = 50;
                    if ( t > 200 )
                        t = 200;
                    pSelf->client->ps.pm_time = t;
                    pSelf->client->ps.pm_flags |= 0x100u;
                }
            }
        }
    }
}

// Decomp: CoDMPServer.c:378695
void __cdecl PlayerCmd_Suicide(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        2507,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pSelf->flags &= 0xFFFFFFFC;
    pSelf->health = 0;
    pSelf->client->ps.stats[0] = 0;
    player_die(pSelf, pSelf, pSelf, 100000, 0xDu, 0, 0, HITLOC_NONE, 0);
}

// LWSS ADD — bot methods added in later TU (not in 1.0 CoDMPServer methods table); implemented for mod script compat.
void PlayerCmd_BotPressAttackButton(scr_entref_t entref)
{
    gentity_s* pSelf;

    if (entref.classnum)
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        bcassert(entref.entnum, 0x400u);
        pSelf = &g_entities[entref.entnum];
        if (!pSelf->client)
        {
            char* error1 = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(error1, SCRIPTINSTANCE_SERVER);
        }
    }

    client_t* pClient = &svs.clients[pSelf->s.number];
    if (pClient->bIsTestClient)
    {
        SV_BotPressAttackButton(pClient);
    }
    else
    {
        char* error2 = va("Illegal call to PressAttackButton(). Player '%s' is not a bot.", pSelf->client->sess.cs.name);
        Scr_Error(error2, 0);
    }
}

void PlayerCmd_GetLookaheadDist(scr_entref_t entref)
{
    gentity_s* pSelf;

    if (entref.classnum)
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        bcassert(entref.entnum, 0x400u);
        pSelf = &g_entities[entref.entnum];
        if (!pSelf->client)
        {
            char* error1 = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(error1, SCRIPTINSTANCE_SERVER);
        }
    }

    client_t* pClient = &svs.clients[pSelf->s.number];
    if (pClient->bIsTestClient)
    {
        float lookaheadDist = SV_BotGetLookaheadDist(pClient);
        Scr_AddFloat(lookaheadDist, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        char* error2 = va("Illegal call to GetLookaheadDist(). Player '%s' is not a bot.", pSelf->client->sess.cs.name);
        Scr_Error(error2, 0);
    }
}

void PlayerCmd_GetLookaheadDir(scr_entref_t entref)
{
    gentity_s* pSelf;

    if (entref.classnum)
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        bcassert(entref.entnum, 0x400u);
        pSelf = &g_entities[entref.entnum];
        if (!pSelf->client)
        {
            char* error1 = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(error1, SCRIPTINSTANCE_SERVER);
        }
    }

    client_t* pClient = &svs.clients[pSelf->s.number];
    if (pClient->bIsTestClient)
    {
        float lookaheadDir[3];
        if (SV_BotGetLookaheadDir(pClient, lookaheadDir))
            return Scr_AddVector(lookaheadDir, SCRIPTINSTANCE_SERVER);
        else
            return Scr_AddUndefined(SCRIPTINSTANCE_SERVER);
    }
    else
    {
        char* error2 = va("Illegal call to GetLookaheadDir(). Player '%s' is not a bot.", pSelf->client->sess.cs.name);
        Scr_Error(error2, 0);
    }
}

void PlayerCmd_GetThreat(scr_entref_t entref)
{
    gentity_s* pSelf;

    if (entref.classnum)
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        bcassert(entref.entnum, 0x400u);
        pSelf = &g_entities[entref.entnum];
        if (!pSelf->client)
        {
            char* error1 = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(error1, SCRIPTINSTANCE_SERVER);
        }
    }

    client_t* pClient = &svs.clients[pSelf->s.number];
    if (pClient->bIsTestClient)
    {
        gentity_s* pThreat = SV_BotGetThreat(pClient);
        if(pThreat)
            Scr_AddEntity(pThreat, SCRIPTINSTANCE_SERVER);
        else
            Scr_AddUndefined(SCRIPTINSTANCE_SERVER);
    }
    else
    {
        char* error2 = va("Illegal call to GetThreat(). Player '%s' is not a bot.", pSelf->client->sess.cs.name);
        Scr_Error(error2, 0);
    }
}

void PlayerCmd_HasScriptGoal(scr_entref_t entref)
{
    gentity_s* pSelf;

    if (entref.classnum)
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        bcassert(entref.entnum, 0x400u);
        pSelf = &g_entities[entref.entnum];
        if (!pSelf->client)
        {
            char* error1 = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(error1, SCRIPTINSTANCE_SERVER);
        }
    }

    client_t* pClient = &svs.clients[pSelf->s.number];
    if (pClient->bIsTestClient)
    {
        int hasScriptGoal = (int)SV_BotHasScriptGoal(pClient);
        return Scr_AddInt(hasScriptGoal, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        char* error2 = va("Illegal call to HasScriptGoal(). Player '%s' is not a bot.", pSelf->client->sess.cs.name);
        Scr_Error(error2, 0);
    }
}
// LWSS END 

// Decomp: CoDMPServer.c:378732
void __cdecl PlayerCmd_IsSplitscreen(scr_entref_t entref)
{
    const char *fmtMsg;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        2527,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        if ( !g_entities[entref.entnum].client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:378763
void __cdecl PlayerCmd_IsPlayerOnSameMachine(scr_entref_t entref)
{
    const char *fmtMsg;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        2571,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        if ( !g_entities[entref.entnum].client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( !Scr_GetType(0, SCRIPTINSTANCE_SERVER) || Scr_GetPointerType(0, SCRIPTINSTANCE_SERVER) != 19 )
        Scr_Error("IsPlayerOnSameMachine: Player must be passed in.", 0);
    if ( !Scr_GetEntity(0)->client )
        Scr_Error("IsPlayerOnSameMachine: Player must be passed in.", 0);
    Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:378798
void __cdecl PlayerCmd_IsInVehicle(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        2617,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( (pSelf->client->ps.eFlags & 0x4000) != 0 )
        Scr_AddBool(1u, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddBool(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:378835
void __cdecl PlayerCmd_IsRemoteControlling(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        2639,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( (pSelf->client->ps.eFlags2 & 0x10000000) != 0 )
        Scr_AddBool(1u, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddBool(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:378872
void __cdecl PlayerCmd_GetVehicleOccupied(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    gentity_s *tmp3;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        2663,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( (pSelf->client->ps.eFlags & 0x4000) != 0 )
    {
        if ( pSelf->r.ownerNum.isDefined() )
        {
            tmp3 = pSelf->r.ownerNum.ent();
            Scr_AddEntity(tmp3, SCRIPTINSTANCE_SERVER);
        }
        else
        {
            fmtMsg2 = va("Player %s is attached to a vehicle but has no owner", pSelf->client->sess.cs.name);
            Scr_Error(fmtMsg2, 0);
        }
    }
}

// Decomp: CoDMPServer.c:378920
void __cdecl PlayerCmd_OpenMenu(scr_entref_t entref)
{
    const char *fmtMsg;
    char *String;
    gentity_s *pSelf;
    unsigned int iMenuIndex;
    char svcmd[68];

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        2683,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( pSelf->client->sess.connected == CON_CONNECTED )
    {
        String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        iMenuIndex = GScr_GetScriptMenuIndex(String);
        if ( iMenuIndex >= 0x20
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        2689,
                        0,
                        "%s",
                        "(iMenuIndex >= 0) && (iMenuIndex < MAX_SCRIPT_MENUS)") )
        {
            __debugbreak();
        }
        _snprintf(svcmd, 0x40u, "%c %i", 116, iMenuIndex);
        svcmd[63] = 0;
        SV_GameSendServerCommand(entref.entnum, SV_CMD_RELIABLE, svcmd);
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:378980
void __cdecl PlayerCmd_OpenMenuNoMouse(scr_entref_t entref)
{
    const char *fmtMsg;
    char *String;
    gentity_s *pSelf;
    unsigned int iMenuIndex;
    char svcmd[68];

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        2707,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( pSelf->client->sess.connected == CON_CONNECTED )
    {
        String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        iMenuIndex = GScr_GetScriptMenuIndex(String);
        if ( iMenuIndex >= 0x20
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        2713,
                        0,
                        "%s",
                        "(iMenuIndex >= 0) && (iMenuIndex < MAX_SCRIPT_MENUS)") )
        {
            __debugbreak();
        }
        _snprintf(svcmd, 0x40u, "%c %i 1", 116, iMenuIndex);
        svcmd[63] = 0;
        SV_GameSendServerCommand(entref.entnum, SV_CMD_RELIABLE, svcmd);
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:379040
void __cdecl PlayerCmd_CloseMenu(scr_entref_t entref)
{
    const char *fmtMsg;
    char svcmd[68];

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        2731,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        if ( !g_entities[entref.entnum].client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    _snprintf(svcmd, 0x40u, "%c", 117);
    svcmd[63] = 0;
    SV_GameSendServerCommand(entref.entnum, SV_CMD_RELIABLE, svcmd);
}

// Decomp: CoDMPServer.c:379074
void __cdecl PlayerCmd_CloseInGameMenu(scr_entref_t entref)
{
    const char *fmtMsg;
    char svcmd[32];

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        2754,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        if ( !g_entities[entref.entnum].client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    _snprintf(svcmd, 0x20u, "%c", 76);
    svcmd[31] = 0;
    SV_GameSendServerCommand(entref.entnum, SV_CMD_RELIABLE, svcmd);
}

// Decomp: CoDMPServer.c:379108
void __cdecl PlayerCmd_SetWeaponAmmoClip(scr_entref_t entref)
{
    const char *fmtMsg;
    int ammoCount;
    gentity_s *pSelf;
    char *weapName;
    int clipIndex;
    unsigned int weapIndex;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        2789,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    weapName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    ammoCount = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    weapIndex = G_GetWeaponIndexForName(weapName);
    if ( weapIndex )
    {
        clipIndex = BG_ClipForWeapon(weapIndex);
        if ( clipIndex )
        {
            if ( ammoCount >= 0 )
            {
                if ( ammoCount > BG_GetClipSize(weapIndex) )
                    ammoCount = BG_GetClipSize(weapIndex);
                BG_SetAmmoInClip(&pSelf->client->ps, clipIndex, ammoCount);
            }
            else
            {
                BG_SetAmmoInClip(&pSelf->client->ps, clipIndex, 0);
            }
            if ( BG_GetWeaponDef(weapIndex)->bDualWield )
                SetWeaponAmmoClipDualWield(&pSelf->client->ps);
        }
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:379171
void __cdecl SetWeaponAmmoClipDualWield(playerState_s *ps)
{
    int ammoCount;
    int clipIndex;
    const WeaponDef *weapDef;

    weapDef = BG_GetWeaponDef(ps->weapon);
    if ( weapDef->dualWieldWeaponIndex )
    {
        clipIndex = BG_ClipForWeapon(weapDef->dualWieldWeaponIndex);
        if ( clipIndex )
        {
            ammoCount = BG_GetClipSize(weapDef->dualWieldWeaponIndex);
            BG_SetAmmoInClip(ps, clipIndex, ammoCount);
        }
    }
}

// Decomp: CoDMPServer.c:379190
void __cdecl PlayerCmd_SetWeaponAmmoStock(scr_entref_t entref)
{
    const char *fmtMsg;
    int tmp2;
    int ClipSize;
    int tmp4;
    int maxAmmo;
    int clipIdx;
    gentity_s *pSelf;
    char *weapName;
    unsigned int weapIdx;
    playerState_s *ps;
    int newAmmoCnt;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        2825,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    weapName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    newAmmoCnt = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    weapIdx = G_GetWeaponIndexForName(weapName);
    ps = &pSelf->client->ps;
    if ( weapIdx )
    {
        if ( BG_WeaponIsClipOnly(weapIdx) )
        {
            clipIdx = BG_ClipForWeapon(weapIdx);
            if ( clipIdx )
            {
                ClipSize = BG_GetClipSize(weapIdx);
                if ( newAmmoCnt < ClipSize )
                    tmp4 = newAmmoCnt;
                else
                    tmp4 = ClipSize;
                if ( tmp4 > 0 )
                    BG_SetAmmoInClip(ps, clipIdx, tmp4);
                else
                    BG_SetAmmoInClip(ps, clipIdx, 0);
            }
        }
        else if ( BG_AmmoForWeapon(weapIdx) )
        {
            maxAmmo = BG_GetAmmoPlayerMax(ps, weapIdx, 0);
            if ( newAmmoCnt < maxAmmo )
                tmp2 = newAmmoCnt;
            else
                tmp2 = maxAmmo;
            if ( tmp2 > 0 )
                BG_SetAmmoInPool(ps, weapIdx, tmp2);
            else
                BG_SetAmmoInPool(ps, weapIdx, 0);
        }
    }
}

// Decomp: CoDMPServer.c:379267
void __cdecl PlayerCmd_SetCurrentSpectatorClient(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        2859,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pSelf->client->spectatorClient = Scr_GetEntity(0)->s.clientNum;
}

// Decomp: CoDMPServer.c:379301
void __cdecl PlayerCmd_GetWeaponAmmoClip(scr_entref_t entref)
{
    const char *fmtMsg;
    int AmmoInClip;
    gentity_s *pSelf;
    char *weapName;
    unsigned int weapIdx;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        2870,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    weapName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weapIdx = G_GetWeaponIndexForName(weapName);
    if ( weapIdx )
    {
        AmmoInClip = BG_GetAmmoInClip(&pSelf->client->ps, weapIdx);
        Scr_AddInt(AmmoInClip, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:379348
void __cdecl PlayerCmd_GetWeaponAmmoStock(scr_entref_t entref)
{
    const char *fmtMsg;
    int AmmoInClip;
    gentity_s *pSelf;
    char *weapName;
    unsigned int weapIdx;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        2889,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    weapName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weapIdx = G_GetWeaponIndexForName(weapName);
    if ( weapIdx )
    {
        if ( BG_WeaponIsClipOnly(weapIdx) )
            AmmoInClip = BG_GetAmmoInClip(&pSelf->client->ps, weapIdx);
        else
            AmmoInClip = BG_GetAmmoNotInClip(&pSelf->client->ps, weapIdx);
        Scr_AddInt(AmmoInClip, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:379398
void __cdecl PlayerCmd_AnyAmmoForWeaponModes(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;
    char *weapName;
    unsigned int weapIdx;
    unsigned int altWeapIdx;
    int totalAmmo;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        2917,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    weapName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weapIdx = G_GetWeaponIndexForName(weapName);
    Scr_VerifyWeaponIndex(weapIdx, weapName);
    totalAmmo = BG_WeaponAmmo(&pSelf->client->ps, weapIdx);
    altWeapIdx = BG_GetWeaponVariantDef(weapIdx)->altWeaponIndex;
    if ( altWeapIdx )
        totalAmmo += BG_WeaponAmmo(&pSelf->client->ps, altWeapIdx);
    if ( totalAmmo )
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:379446
void __cdecl iclientprintln(scr_entref_t entref)
{
    const char *fmtMsg;
    char svcmd[32];

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        2941,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        if ( !g_entities[entref.entnum].client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    _snprintf(svcmd, 0x20u, "%c", 102);
    svcmd[31] = 0;
    Scr_MakeGameMessage(entref.entnum, svcmd);
}

// Decomp: CoDMPServer.c:379480
void __cdecl iclientprintlnbold(scr_entref_t entref)
{
    const char *fmtMsg;
    char svcmd[32];

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        2953,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        if ( !g_entities[entref.entnum].client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    _snprintf(svcmd, 0x20u, "%c", 103);
    svcmd[31] = 0;
    Scr_MakeGameMessage(entref.entnum, svcmd);
}

// Decomp: CoDMPServer.c:379514
void __cdecl PlayerCmd_spawn(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;
    float spawn_angles[3];
    float spawn_origin[3];

    PROF_SCOPED("PlayerCmd_spawn");

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        2967,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    Scr_GetVector(0, spawn_origin, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(1u, spawn_angles, SCRIPTINSTANCE_SERVER);
    ClientSpawn(pSelf, spawn_origin, spawn_angles);
#ifdef KISAK_LIVE // lol lets invent another ifdef name
    if ( onlinegame->current.enabled
        && com_sv_running->current.enabled
        && pSelf->client->sess.sessionState == SESS_STATE_PLAYING )
    {
        MatchRecordSpawn(pSelf->client);
    }
#endif
}

// Decomp: CoDMPServer.c:379562
void __cdecl PlayerCmd_setEnterTime(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        2991,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pSelf->client->sess.enterTime = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:379596
void __cdecl BodyEnd(gentity_s *ent)
{
    ent->s.lerp.eFlags &= ~0x10u;
    ent->r.contents = 0x4000000;
    ent->r.svFlags = 0;
}

// Decomp: CoDMPServer.c:379604
void __cdecl PlayerCmd_ClonePlayer(scr_entref_t entref)
{
    const char *fmtMsg;
    DObj *ServerDObj;
    gclient_s *client;
    gentity_s *pSelf;
    const DObj *dobj;
    XAnimTree_s *tree;
    gentity_s *body;
    int deathAnimDuration;
    int corpseIndex;
    corpseInfo_t *corpseInfo;
    int axis;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3027,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    deathAnimDuration = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    client = pSelf->client;
    if ( !client
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    3032,
                    0,
                    "%s",
                    "client") )
    {
        __debugbreak();
    }
    if ( client->sess.connected == CON_DISCONNECTED
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    3033,
                    0,
                    "%s",
                    "client->sess.connected != CON_DISCONNECTED") )
    {
        __debugbreak();
    }
    body = G_SpawnPlayerClone();
    body->s.clientNum = client->ps.clientNum;
    body->s.lerp.eFlags = body->s.lerp.eFlags & 2 | client->ps.eFlags & 0xFFFFFFFD | 0x40010;
    if ( (client->ps.eFlags2 & 0x200000) != 0 )
        body->s.lerp.eFlags2 |= 0x200000u;
    G_SetOrigin(body, client->ps.origin);
    G_SetAngle(body, pSelf->r.currentAngles);
    body->s.lerp.pos.trType = 6;
    body->s.lerp.pos.trTime = level.time;
    body->s.lerp.pos.trDelta[0] = client->ps.velocity[0];
    body->s.lerp.pos.trDelta[1] = client->ps.velocity[1];
    body->s.lerp.pos.trDelta[2] = client->ps.velocity[2];
    if ( ((LODWORD(body->s.lerp.pos.trDelta[0]) & 0x7F800000) == 0x7F800000
         || (LODWORD(body->s.lerp.pos.trDelta[1]) & 0x7F800000) == 0x7F800000
         || (LODWORD(body->s.lerp.pos.trDelta[2]) & 0x7F800000) == 0x7F800000)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    3049,
                    0,
                    "%s",
                    "!IS_NAN((body->s.lerp.pos.trDelta)[0]) && !IS_NAN((body->s.lerp.pos.trDelta)[1]) && !IS_NAN((body->s.lerp.pos.trDelta)[2])") )
    {
        __debugbreak();
    }
    body->s.eType = ET_PLAYER_CORPSE;
    body->physicsObject = 1;
    body->model = pSelf->model;
    body->s.renderOptions.i = pSelf->s.renderOptions.i;
    dobj = Com_GetServerDObj(client->ps.clientNum);
    tree = DObjGetTree(dobj);
    body->s.lerp.u.player.stowedWeapon = client->ps.stowedWeapon;
    for ( axis = 0; axis < 2; ++axis )
    {
        if ( body->s.lerp.pos.trDelta[axis] > g_clonePlayerMaxVelocity->current.value )
            body->s.lerp.pos.trDelta[axis] = g_clonePlayerMaxVelocity->current.value;
    }
    if ( ((LODWORD(body->s.lerp.pos.trDelta[0]) & 0x7F800000) == 0x7F800000
         || (LODWORD(body->s.lerp.pos.trDelta[1]) & 0x7F800000) == 0x7F800000
         || (LODWORD(body->s.lerp.pos.trDelta[2]) & 0x7F800000) == 0x7F800000)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    3068,
                    0,
                    "%s",
                    "!IS_NAN((body->s.lerp.pos.trDelta)[0]) && !IS_NAN((body->s.lerp.pos.trDelta)[1]) && !IS_NAN((body->s.lerp.pos.trDelta)[2])") )
    {
        __debugbreak();
    }
    Com_Printf(14, "Cloning Player entnum %i %i\n", body->s.clientNum, level.time);
    body->item[0].ammoCount = level.time;
    corpseIndex = G_GetFreePlayerCorpseIndex();
    corpseInfo = &g_scr_data.playerCorpseInfo[corpseIndex];
    corpseInfo->entnum = body->s.number;
    corpseInfo->time = level.time;
    corpseInfo->falling = 1;
    if ( client->ps.clientNum >= 0x20u
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    3077,
                    0,
                    "client->ps.clientNum doesn't index MAX_CLIENTS\n\t%i not in [0, %i)",
                    client->ps.clientNum,
                    32) )
    {
        __debugbreak();
    }
    memcpy(&corpseInfo->ci, &level_bgs.clientinfo[client->ps.clientNum], sizeof(corpseInfo->ci));
    corpseInfo->ci.pXAnimTree = corpseInfo->tree;
    client->ps.corpseIndex = corpseIndex;
    XAnimCloneAnimTree(tree, corpseInfo->tree);
    ServerDObj = Com_GetServerDObj(body->s.number);
    BG_UpdatePlayerDObj(-1, ServerDObj, &body->s, &corpseInfo->ci, 0);
    body->s.groundEntityNum = 1023;
    if ( body->r.svFlags
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    3087,
                    0,
                    "%s",
                    "!body->r.svFlags") )
    {
        __debugbreak();
    }
    body->r.svFlags = 2;
    body->r.mins[0] = pSelf->r.mins[0];
    body->r.mins[1] = pSelf->r.mins[1];
    body->r.mins[2] = pSelf->r.mins[2];
    body->r.maxs[0] = pSelf->r.maxs[0];
    body->r.maxs[1] = pSelf->r.maxs[1];
    body->r.maxs[2] = pSelf->r.maxs[2];
    body->r.absmin[0] = pSelf->r.absmin[0];
    body->r.absmin[1] = pSelf->r.absmin[1];
    body->r.absmin[2] = pSelf->r.absmin[2];
    body->r.absmax[0] = pSelf->r.absmax[0];
    body->r.absmax[1] = pSelf->r.absmax[1];
    body->r.absmax[2] = pSelf->r.absmax[2];
    body->s.animState.state = client->ps.legsAnim;
    body->s.anim.torsoAnim = client->ps.torsoAnim;
    body->clipmask = 65537;
    body->r.contents = 67125248;
    SV_LinkEntity(body);
    body->nextthink = deathAnimDuration + level.time;
    body->handler = 16;
    GScr_AddEntity(body);
}

// Decomp: CoDMPServer.c:379795
void __cdecl PlayerCmd_SetClientDvar(scr_entref_t entref)
{
    const char *fmtMsg;
    int NumParam;
    const char *fmtMsg2;
    char tmp4;
    const char *fmtMsg3;
    char *pszDvar;
    char *pszText;
    int i;
    char szString[1024];
    char szOutString[1024];
    char *pCh;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3122,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        if ( !g_entities[entref.entnum].client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pszDvar = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    if ( Scr_GetType(1u, SCRIPTINSTANCE_SERVER) == 3 )
    {
        NumParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
        Scr_ConstructMessageString(1, NumParam - 1, "Client Dvar Value", szString, 0x400u);
        pszText = szString;
    }
    else
    {
        pszText = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    }
    strlen(pszText);
    if ( Dvar_IsValidName(pszDvar) )
    {
        pCh = szOutString;
        memset((unsigned __int8 *)szOutString, 0, sizeof(szOutString));
        for ( i = 0; i < 1023 && pszText[i]; ++i )
        {
            tmp4 = I_CleanChar(pszText[i]);
            *pCh = tmp4;
            if ( *pCh == 34 )
                *pCh = 39;
            ++pCh;
        }
        fmtMsg3 = va("%c %s \"%s\"", 118, pszDvar, szOutString);
        SV_GameSendServerCommand(entref.entnum, SV_CMD_RELIABLE, fmtMsg3);
    }
    else
    {
        fmtMsg2 = va("Dvar %s has an invalid dvar name", pszDvar);
        Scr_Error(fmtMsg2, 0);
    }
}

// Decomp: CoDMPServer.c:379868
void __cdecl PlayerCmd_SetClientUIVisibilityFlag(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    int tmp3;
    gentity_s *pSelf;
    char *flagName;
    clientState_s *clientState;
    char flagBit;
    int value;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3163,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 2 )
    {
        flagName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        value = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
        if ( I_stricmp("hud_visible", flagName) )
        {
            if ( I_stricmp("g_compassShowEnemies", flagName) )
            {
                if ( I_stricmp("radar_client", flagName) )
                {
                    if ( I_stricmp("killcam_nemesis", flagName) )
                    {
                        fmtMsg2 = va("PlayerCmd_SetClientFlag: Couldn't find a matching bit for flag name : %s ", flagName);
                        Scr_Error(fmtMsg2, 0);
                        return;
                    }
                    flagBit = 3;
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
        }
        else
        {
            flagBit = 0;
        }
        clientState = G_GetClientState(pSelf->s.number);
        if ( clientState )
        {
            if ( value )
                tmp3 = clientState->clientUIVisibilityFlags | (1 << flagBit);
            else
                tmp3 = clientState->clientUIVisibilityFlags & ~(1 << flagBit);
            clientState->clientUIVisibilityFlags = tmp3;
        }
    }
}

// Decomp: CoDMPServer.c:379972
void __cdecl PlayerCmd_SetClientDvars(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    unsigned __int8 *c;
    char finalString[1024];
    char tempString[1024];
    const char *dvarName;
    unsigned int i;
    const char *value;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3225,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        if ( !g_entities[entref.entnum].client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) % 2u )
        Scr_Error(
            "Not enough parameters to setclientdvar() - must be an even number of parameters (dvar, value, dvar, value, etc.)\n",
            0);
    strcpy(finalString, "v");
    for ( i = 0; i < Scr_GetNumParam(SCRIPTINSTANCE_SERVER); i += 2 )
    {
        dvarName = Scr_GetString(i, SCRIPTINSTANCE_SERVER);
        value = Scr_GetString(i + 1, SCRIPTINSTANCE_SERVER);
        if ( !Dvar_IsValidName(dvarName) )
        {
            fmtMsg2 = va("Dvar %s has an invalid dvar name", dvarName);
            Scr_Error(fmtMsg2, 0);
            return;
        }
        I_strncpyz(tempString, value, 1024);
        for ( c = (unsigned __int8 *)tempString; *c; ++c )
        {
            *c = I_CleanChar(*c);
            if ( *c == 34 )
                *c = 39;
        }
        I_strncat(finalString, 1024, " ");
        I_strncat(finalString, 1024, dvarName);
        I_strncat(finalString, 1024, " \"");
        I_strncat(finalString, 1024, tempString);
        I_strncat(finalString, 1024, "\"");
    }
    SV_GameSendServerCommand(entref.entnum, SV_CMD_RELIABLE, finalString);
}

// Decomp: CoDMPServer.c:380040
void __cdecl PlayerCmd_PlayClientAirstrike(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    char *teamName;
    char *team;
    unsigned int owner;
    unsigned int yaw;
    int height;
    float position[3];
    const char *exitType;

    exitType = "straight";
    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3282,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        if ( !g_entities[entref.entnum].client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    Scr_GetVector(0, position, SCRIPTINSTANCE_SERVER);
    yaw = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    teamName = Scr_GetString(2u, SCRIPTINSTANCE_SERVER);
    team = Scr_GetString(3u, SCRIPTINSTANCE_SERVER);
    owner = Scr_GetInt(4u, SCRIPTINSTANCE_SERVER);
    exitType = Scr_GetString(5u, SCRIPTINSTANCE_SERVER);
    height = Scr_GetInt(6u, SCRIPTINSTANCE_SERVER);
    fmtMsg2 = va(
                 "%c %f %f %f %i %c %c %c %i %c %i %i",
                 82,
                 position[0],
                 position[1],
                 position[2],
                 yaw,
                 97,
                 *teamName,
                 team[1],
                 owner,
                 *exitType,
                 level.time,
                 height);
    SV_GameSendServerCommand(-1, SV_CMD_RELIABLE, fmtMsg2);
}

// Decomp: CoDMPServer.c:380102
void __cdecl PlayerCmd_PlayClientNapalm(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    char *teamName;
    char *team;
    unsigned int owner;
    unsigned int yaw;
    int height;
    float position[3];
    const char *exitType;

    exitType = "left";
    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3319,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        if ( !g_entities[entref.entnum].client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    Scr_GetVector(0, position, SCRIPTINSTANCE_SERVER);
    yaw = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    teamName = Scr_GetString(2u, SCRIPTINSTANCE_SERVER);
    team = Scr_GetString(3u, SCRIPTINSTANCE_SERVER);
    owner = Scr_GetInt(4u, SCRIPTINSTANCE_SERVER);
    exitType = Scr_GetString(5u, SCRIPTINSTANCE_SERVER);
    height = Scr_GetInt(6u, SCRIPTINSTANCE_SERVER);
    fmtMsg2 = va(
                 "%c %f %f %f %i %c %c %c %i %c %i %i",
                 82,
                 position[0],
                 position[1],
                 position[2],
                 yaw,
                 110,
                 *teamName,
                 team[1],
                 owner,
                 *exitType,
                 level.time,
                 height);
    SV_GameSendServerCommand(-1, SV_CMD_RELIABLE, fmtMsg2);
}

// Decomp: CoDMPServer.c:380164
void __cdecl PlayerCmd_IsTalking(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;
    int elapsedTime;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3348,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    elapsedTime = level.time - pSelf->client->lastVoiceTime;
    if ( elapsedTime < 0 || elapsedTime >= g_voiceChatTalkingDuration->current.integer )
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
}

// LWSS ADD
void PlayerCmd_FreezeControlsAllowLook(scr_entref_t entref)
{
    gentity_s *pSelf;

    if (entref.classnum)
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        iassert(entref.entnum < MAX_GENTITIES);

        pSelf = &g_entities[entref.entnum];
        if (!pSelf->client)
        {
            Scr_ObjectError(va("entity %i is not a player", entref.entnum), SCRIPTINSTANCE_SERVER);
        }
    }
    // BlackOpsMP.retail.c:48520-48521 (unk_A9BA24 / loc_82CD40), sub_831750 @ ~895272:
    // toggles gclient_s::flags bit 0x100. ClientEndFrame maps to pm_flags 0x10; retail PM ~813233
    // and client input ~874080 block pitch/yaw (name is not "allow mouse look" on zipline).
    if (Scr_GetInt(0, SCRIPTINSTANCE_SERVER))
        pSelf->client->flags |= 0x100u;
    else
        pSelf->client->flags &= ~0x100u;
}

void PlayerCmd_AreControlsFrozen(scr_entref_t entref)
{
    gentity_s *pSelf;

    if (entref.classnum)
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        iassert(entref.entnum < MAX_GENTITIES);

        pSelf = &g_entities[entref.entnum];
        if (!pSelf->client)
        {
            Scr_ObjectError(va("entity %i is not a player", entref.entnum), SCRIPTINSTANCE_SERVER);
        }
    }

    iassert(pSelf);
    iassert(pSelf->client);

    if (pSelf->client->flags & 4)
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
}
// LWSS END

// Decomp: CoDMPServer.c:380203
void __cdecl PlayerCmd_FreezeControls(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3360,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( Scr_GetInt(0, SCRIPTINSTANCE_SERVER) )
        pSelf->client->flags |= 4u;
    else
        pSelf->client->flags &= ~4u;
}

// Decomp: CoDMPServer.c:380240
void __cdecl PlayerCmd_DisableUsability(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3371,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pSelf->client->flags |= 8u;
}

// Decomp: CoDMPServer.c:380274
void __cdecl PlayerCmd_EnableUsability(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3378,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pSelf->client->flags &= ~8u;
}

// Decomp: CoDMPServer.c:380308
void __cdecl PlayerCmd_DisableWeapons(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;
    playerState_s *ps;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3384,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    ps = &pSelf->client->ps;
    ps->weapFlags |= 0x80u;
    if ( (ps->pm_flags & 0x400000) != 0 )
    {
        ps->pm_flags &= ~0x400000u;
        ps->jumpTime = 0;
        ps->velocity[0] = 0.0f;
        ps->velocity[1] = 0.0f;
        ps->velocity[2] = 0.0f;
        ps->lastDtpEnd = ps->commandTime;
        ps->sprintState.sprintButtonUpRequired = 0;
    }
}

// Decomp: CoDMPServer.c:380356
void __cdecl PlayerCmd_EnableWeapons(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3404,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pSelf->client->ps.weapFlags &= ~0x80u;
}

// Decomp: CoDMPServer.c:380390
void __cdecl PlayerCmd_DisableDeathStreak(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3410,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pSelf->client->ps.weapFlags &= ~0x40000u;
}

// Decomp: CoDMPServer.c:380424
void __cdecl PlayerCmd_EnableDeathStreak(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3417,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pSelf->client->ps.weapFlags |= 0x40000u;
}

// Decomp: CoDMPServer.c:380458
void __cdecl PlayerCmd_DeathStreakActive(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3423,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    Scr_AddInt((pSelf->client->ps.weapFlags & 0x40000) != 0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:380492
void __cdecl PlayerCmd_DisableOffhandWeapons(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3440,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pSelf->client->ps.weapFlags |= 0x100u;
}

// Decomp: CoDMPServer.c:380526
void __cdecl PlayerCmd_EnableOffhandWeapons(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3456,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pSelf->client->ps.weapFlags &= ~0x100u;
}

// Decomp: CoDMPServer.c:380560
void __cdecl PlayerCmd_DisableWeaponCycling(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3472,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pSelf->client->ps.weapFlags |= 0x200u;
}

// Decomp: CoDMPServer.c:380594
void __cdecl PlayerCmd_EnableWeaponCycling(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3489,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pSelf->client->ps.weapFlags &= ~0x200u;
}

// Decomp: CoDMPServer.c:380628
void __cdecl ScrCmd_PlayLocalSound(scr_entref_t entref)
{
    const char *fmtMsg;
    char *String;
    int AliasId;
    char svcmd[68];

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3531,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        if ( !g_entities[entref.entnum].client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    String = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    AliasId = SND_FindAliasId(String);
    _snprintf(svcmd, 0x40u, "%c %i", 115, AliasId);
    svcmd[63] = 0;
    SV_GameSendServerCommand(entref.entnum, SV_CMD_CAN_IGNORE, svcmd);
}

// Decomp: CoDMPServer.c:380667
void __cdecl ScrCmd_StopLocalSound(scr_entref_t entref)
{
    const char *fmtMsg;
    char *pszSoundName;
    char svcmd[64];
    unsigned int soundIndex;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3546,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        if ( !g_entities[entref.entnum].client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pszSoundName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    soundIndex = SND_FindAliasId(pszSoundName);
    _snprintf(svcmd, 0x40u, "%c %i", 107, soundIndex);
    svcmd[63] = 0;
    SV_GameSendServerCommand(entref.entnum, SV_CMD_CAN_IGNORE, svcmd);
}

// Decomp: CoDMPServer.c:380705
void __cdecl PlayerCmd_SayAll(scr_entref_t entref)
{
    const char *fmtMsg;
    int NumParam;
    gentity_s *pSelf;
    char szString[1028];

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3573,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    NumParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    Scr_ConstructMessageString(0, NumParam - 1, "Client Chat Message", &szString[1], 0x3FFu);
    szString[0] = 20;
    G_Say(pSelf, 0, 0, szString);
}

// Decomp: CoDMPServer.c:380744
void __cdecl PlayerCmd_SayTeam(scr_entref_t entref)
{
    const char *fmtMsg;
    int NumParam;
    gentity_s *pSelf;
    char szString[1028];

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3598,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    NumParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    Scr_ConstructMessageString(0, NumParam - 1, "Client Chat Message", &szString[1], 0x3FFu);
    szString[0] = 20;
    G_Say(pSelf, 0, 1, szString);
}

// Decomp: CoDMPServer.c:380783
void __cdecl PlayerCmd_AllowADS(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3612,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        if ( Scr_GetInt(0, SCRIPTINSTANCE_SERVER) )
        {
            pSelf->client->ps.weapFlags &= ~0x20u;
        }
        else
        {
            pSelf->client->ps.weapFlags |= 0x20u;
            PM_ExitAimDownSight(&pSelf->client->ps);
        }
    }
    else
    {
        Scr_Error("USAGE: <player> allowads( <boolean> )\n", 0);
    }
}

// Decomp: CoDMPServer.c:380832
void __cdecl PlayerCmd_AllowJump(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3634,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( Scr_GetInt(0, SCRIPTINSTANCE_SERVER) )
        pSelf->client->ps.pm_flags &= ~0x80000u;
    else
        pSelf->client->ps.pm_flags |= 0x80000u;
}

// Decomp: CoDMPServer.c:380869
void __cdecl PlayerCmd_AllowSprint(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3645,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( Scr_GetInt(0, SCRIPTINSTANCE_SERVER) )
        pSelf->client->ps.pm_flags &= ~0x40000u;
    else
        pSelf->client->ps.pm_flags |= 0x40000u;
}

// Decomp: CoDMPServer.c:380906
void __cdecl PlayerCmd_SetSprintDuration(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    gentity_s *pSelf;
    float duration;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3657,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        duration = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
        if ( duration >= 0.0 )
        {
            if ( duration < 16383.0 )
            {
                pSelf->client->ps.sprintState.sprintDuration = (int)(float)(duration * 1000.0);
            }
            else
            {
                fmtMsg2 = va("setsprintduration: duration must be < %d", 0x3FFF);
                Scr_ParamError(0, fmtMsg2, SCRIPTINSTANCE_SERVER);
            }
        }
        else
        {
            Scr_ParamError(0, "setsprintduration: duration must be >= 0", SCRIPTINSTANCE_SERVER);
        }
    }
    else
    {
        Scr_Error("USAGE: <player> setsprintduration( <duration> )\n", 0);
    }
}

// Decomp: CoDMPServer.c:380966
void __cdecl PlayerCmd_SetSprintCooldown(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    gentity_s *pSelf;
    float duration;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3685,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        duration = Scr_GetFloat(0, SCRIPTINSTANCE_SERVER);
        if ( duration >= 0.0 )
        {
            if ( duration < 16383.0 )
            {
                pSelf->client->ps.sprintState.sprintCooldown = (int)(float)(duration * 1000.0);
            }
            else
            {
                fmtMsg2 = va("setsprintcooldown: duration must be < %d", 0x3FFF);
                Scr_ParamError(0, fmtMsg2, SCRIPTINSTANCE_SERVER);
            }
        }
        else
        {
            Scr_ParamError(0, "setsprintcooldown: duration must be >= 0", SCRIPTINSTANCE_SERVER);
        }
    }
    else
    {
        Scr_Error("USAGE: <player> setsprintcooldown( <duration> )\n", 0);
    }
}

// Decomp: CoDMPServer.c:381026
void __cdecl PlayerCmd_SetSpreadOverride(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    gentity_s *pSelf;
    int value;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3713,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        value = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
        if ( value > 0 )
        {
            if ( value < 64 )
            {
                pSelf->client->ps.spreadOverride = value;
                pSelf->client->ps.spreadOverrideState = 2;
            }
            else
            {
                fmtMsg2 = va("setspreadoverride: spread must be < %d", 64);
                Scr_ParamError(0, fmtMsg2, SCRIPTINSTANCE_SERVER);
            }
        }
        else
        {
            Scr_ParamError(0, "setspreadoverride: spread must be > 0", SCRIPTINSTANCE_SERVER);
        }
    }
    else
    {
        Scr_Error("USAGE: <player> setspreadoverride( <spread> )\n", 0);
    }
}

// Decomp: CoDMPServer.c:381086
void __cdecl PlayerCmd_ResetSpreadOverride(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3741,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pSelf->client->ps.spreadOverrideState = 1;
    pSelf->client->ps.aimSpreadScale = 255.0f;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        Scr_Error("USAGE: <player> resetspreadoverride()\n", 0);
}

// Decomp: CoDMPServer.c:381124
void __cdecl PlayerCmd_AllowSpectateTeam(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;
    unsigned __int16 teamString;
    int teamBit;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3769,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    teamString = (unsigned __int16)Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    teamBit = 0;
    if ( teamString == scr_const.axis )
    {
        teamBit = 2;
    }
    else if ( teamString == scr_const.allies )
    {
        teamBit = 4;
    }
    else if ( teamString == scr_const.none )
    {
        teamBit = 1;
    }
    else if ( teamString == scr_const.freelook )
    {
        teamBit = 16;
    }
    else if ( teamString == scr_const.localplayers )
    {
        teamBit = 32;
    }
    else
    {
        Scr_ParamError(
            0,
            "team must be \"axis\", \"allies\", \"none\", \"localplayers\", or \"freelook\"",
            SCRIPTINSTANCE_SERVER);
    }
    if ( Scr_GetInt(1u, SCRIPTINSTANCE_SERVER) )
        pSelf->client->sess.noSpectate &= ~teamBit;
    else
        pSelf->client->sess.noSpectate |= teamBit;
}

// Decomp: CoDMPServer.c:381189
void __cdecl PlayerCmd_GetGuid(scr_entref_t entref)
{
    const char *fmtMsg;
    int Guid;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3798,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        if ( !g_entities[entref.entnum].client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        Scr_Error("USAGE: self getGuid()\n", 0);
    Guid = SV_GetGuid(entref.entnum);
    Scr_AddInt(Guid, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:381224
void __cdecl PlayerCmd_GetXuid(scr_entref_t entref)
{
    const char *fmtMsg;
    unsigned __int64 PlayerXuid; // rax
    gentity_s *pSelf;
    char svcmd[132];

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3813,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) && Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error("USAGE: self getXuid()\n", 0);
    if ( pSelf->client )
    {
        PlayerXuid = SV_GetPlayerXuid(pSelf->client->sess.cs.clientIndex);
        _snprintf(svcmd, 0x80u, "%llx", PlayerXuid);
        svcmd[127] = 0;
        Scr_AddString(svcmd, SCRIPTINSTANCE_SERVER);
    }
    else
    {
        Scr_AddString("0", SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:381272
void __cdecl PlayerCmd_IsHost(scr_entref_t entref)
{
    char *fmtMsg;
    gentity_s *pSelf;

    if (entref.classnum)
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if (entref.entnum >= 0x400u
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                3865,
                0,
                "%s",
                "entref.entnum < MAX_GENTITIES"))
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if (!pSelf->client)
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }

    if (Scr_GetNumParam(SCRIPTINSTANCE_SERVER))
        Scr_Error("USAGE: self IsHost()\n", 0);

    if (Session_IsHost(&g_serverSession, pSelf->client->sess.cs.clientIndex))
        Scr_AddInt(1, SCRIPTINSTANCE_SERVER);
    else
        Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:381313
void __cdecl PlayerCmd_IsLocalToHost(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3887,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
        Scr_Error("USAGE: self IsLocalToHost()\n", 0);
    if ( pSelf->client->sess.cs.clientIndex >= 0x20u
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    3891,
                    0,
                    "pSelf->client->sess.cs.clientIndex doesn't index MAX_CLIENTS\n\t%i not in [0, %i)",
                    pSelf->client->sess.cs.clientIndex,
                    32) )
    {
        __debugbreak();
    }
    Scr_AddInt(0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:381364
void __cdecl PlayerCmd_BeginLocationSelection(scr_entref_t entref)
{
    PlayerCmd_BeginLocationTypeSelection(entref, LOC_SEL_NONE);
}

// Decomp: CoDMPServer.c:381374
void __cdecl PlayerCmd_BeginLocationTypeSelection(scr_entref_t entref, locSel_t selectionType)
{
    const char *fmtMsg;
    gentity_s *pSelf;
    float radius;
    float radiusa;
    int locSelIndex;
    char *locSelName;
    unsigned int radiusBits;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3928,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( !pSelf->client
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    3930,
                    0,
                    "%s",
                    "pSelf->client") )
    {
        __debugbreak();
    }
    if ( pSelf->client->sess.connected == CON_DISCONNECTED
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    3931,
                    0,
                    "%s",
                    "pSelf->client->sess.connected != CON_DISCONNECTED") )
    {
        __debugbreak();
    }
    locSelName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    locSelIndex = GScr_GetLocSelIndex(locSelName);
    if ( (locSelIndex < 1 || locSelIndex > 16)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    3935,
                    0,
                    "locSelIndex not in [1, MAX_LOC_SEL_MTLS + 1]\n\t%i not in [%i, %i]",
                    locSelIndex,
                    1,
                    16) )
    {
        __debugbreak();
    }
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) < 2 )
    {
        radiusa = 0.15000001f;
    }
    else
    {
        radius = Scr_GetFloat(1u, SCRIPTINSTANCE_SERVER);
        if ( radius <= 0.0 )
            Scr_ParamError(1u, "Radius of location selector must be greater than zero\n", SCRIPTINSTANCE_SERVER);
        if ( level.compassMapWorldSize[1] <= 0.0 || radius <= 0.0 )
            radiusa = radius / 1000.0;
        else
            radiusa = radius / level.compassMapWorldSize[1];
        if ( 1.0 <= 0.0
            && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.h", 730, 0, "%s", "min < max") )
        {
            __debugbreak();
        }
        if ( radiusa >= 0.0 )
        {
            if ( radiusa > 1.0 )
                radiusa = 1.0f;
        }
        else
        {
            radiusa = 0.0f;
        }
    }
    pSelf->client->ps.locationSelectionType = selectionType;
    radiusBits = (int)((float)(radiusa * 15.0) + 9.313225746154785e-10);
    if ( radiusBits >= 0x10
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    3960,
                    0,
                    "radiusBits doesn't index (1 << LOC_SEL_RADIUS_BITS)\n\t%i not in [0, %i)",
                    radiusBits,
                    16) )
    {
        __debugbreak();
    }
    pSelf->client->ps.locationSelectionInfo = locSelIndex | (16 * radiusBits);
}

// Decomp: CoDMPServer.c:381502
void __cdecl PlayerCmd_BeginLocationAirstrikeSelection(scr_entref_t entref)
{
    PlayerCmd_BeginLocationTypeSelection(entref, LOC_SEL_AIRSTRIKE);
}

// Decomp: CoDMPServer.c:381512
void __cdecl PlayerCmd_BeginLocationMortarSelection(scr_entref_t entref)
{
    PlayerCmd_BeginLocationTypeSelection(entref, LOC_SEL_MORTAR);
}

// Decomp: CoDMPServer.c:381522
void __cdecl PlayerCmd_BeginLocationArtillerySelection(scr_entref_t entref)
{
    PlayerCmd_BeginLocationTypeSelection(entref, LOC_SEL_ARTILLERY);
}

// Decomp: CoDMPServer.c:381532
void __cdecl PlayerCmd_BeginLocationComlinkSelection(scr_entref_t entref)
{
    PlayerCmd_BeginLocationTypeSelection(entref, LOC_SEL_COMLINK);
}

// Decomp: CoDMPServer.c:381542
void __cdecl PlayerCmd_BeginLocationNapalmSelection(scr_entref_t entref)
{
    PlayerCmd_BeginLocationTypeSelection(entref, LOC_SEL_NAPALM);
}

// Decomp: CoDMPServer.c:381552
void __cdecl PlayerCmd_EndLocationSelection(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        3998,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( !pSelf->client
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    4000,
                    0,
                    "%s",
                    "pSelf->client") )
    {
        __debugbreak();
    }
    if ( pSelf->client->sess.connected == CON_DISCONNECTED
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    4001,
                    0,
                    "%s",
                    "pSelf->client->sess.connected != CON_DISCONNECTED") )
    {
        __debugbreak();
    }
    pSelf->client->ps.locationSelectionInfo = 0;
}

// Decomp: CoDMPServer.c:381608
void __cdecl PlayerCmd_SetActionSlot(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    const char *fmtMsg3;
    int weaponIdx;
    char *str;
    char *stra;
    gentity_s *pSelf;
    int slot;
    int slota;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        4012,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    slot = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( slot >= 1 && slot <= 4 )
    {
        slota = slot - 1;
        str = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
        if ( I_stricmp(str, "weapon") )
        {
            if ( I_stricmp(str, "altmode") )
            {
                if ( I_stricmp(str, "nightvision") )
                {
                    if ( I_stricmp(str, "") )
                        Scr_Error("Invalid option: expected \"weapon\", \"altweapon\", or \"nightvision\".\n", 0);
                    else
                        pSelf->client->ps.actionSlotType[slota] = ACTIONSLOTTYPE_DONOTHING;
                }
                else
                {
                    pSelf->client->ps.actionSlotType[slota] = ACTIONSLOTTYPE_NIGHTVISION;
                }
            }
            else
            {
                pSelf->client->ps.actionSlotType[slota] = ACTIONSLOTTYPE_ALTWEAPONTOGGLE;
            }
        }
        else
        {
            stra = Scr_GetString(2u, SCRIPTINSTANCE_SERVER);
            weaponIdx = BG_FindWeaponIndexForName(stra);
            if ( weaponIdx )
            {
                pSelf->client->ps.actionSlotType[slota] = ACTIONSLOTTYPE_SPECIFYWEAPON;
                pSelf->client->ps.actionSlotParam[slota].specifyWeapon.index = weaponIdx;
            }
            else
            {
                fmtMsg3 = va("Unknown weapon name \"%s\".\n", stra);
                Scr_ParamError(2u, fmtMsg3, SCRIPTINSTANCE_SERVER);
            }
        }
    }
    else
    {
        fmtMsg2 = va("Invalid slot (%i) given, expecting 1 - %i\n", slot, 4);
        Scr_Error(fmtMsg2, 0);
    }
}

// Decomp: CoDMPServer.c:381695
void __cdecl PlayerCmd_GetWeaponsList(scr_entref_t entref)
{
    const char *fmtMsg;
    char *tmp2;
    int slot;
    gentity_s *pSelf;
    unsigned int weapIndex;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        4063,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for ( slot = 0; slot < 15; ++slot )
    {
        weapIndex = pSelf->client->ps.heldWeapons[slot].weapon;
        if ( weapIndex )
        {
            if ( BG_GetWeaponDef(weapIndex)->inventoryType != WEAPINVENTORY_ALTMODE )
            {
                tmp2 = (char *)BG_WeaponName(weapIndex);
                Scr_AddString(tmp2, SCRIPTINSTANCE_SERVER);
                Scr_AddArray(SCRIPTINSTANCE_SERVER);
            }
        }
    }
}

// Decomp: CoDMPServer.c:381745
void __cdecl PlayerCmd_GetWeaponsListPrimaries(scr_entref_t entref)
{
    const char *fmtMsg;
    char *tmp2;
    int slot;
    gentity_s *pSelf;
    unsigned int weapIndex;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        4090,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for ( slot = 0; slot < 15; ++slot )
    {
        weapIndex = pSelf->client->ps.heldWeapons[slot].weapon;
        if ( weapIndex )
        {
            if ( BG_GetWeaponDef(weapIndex)->inventoryType == WEAPINVENTORY_PRIMARY )
            {
                tmp2 = (char *)BG_WeaponName(weapIndex);
                Scr_AddString(tmp2, SCRIPTINSTANCE_SERVER);
                Scr_AddArray(SCRIPTINSTANCE_SERVER);
            }
        }
    }
}

// Decomp: CoDMPServer.c:381795
void __cdecl PlayerCmd_SetPerk(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    gentity_s *pSelf;
    char *perkName;
    unsigned int perkIndex;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        4129,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    perkName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    perkIndex = BG_GetPerkIndexForName(perkName);
    if ( perkIndex == 52 )
    {
        fmtMsg2 = va("Unknown perk: %s\n", perkName);
        Scr_Error(fmtMsg2, 0);
    }
    BG_SetPerk(pSelf->client->ps.perks, perkIndex);
    BG_SetPerk(pSelf->client->sess.cs.perks, perkIndex);
}

// Decomp: CoDMPServer.c:381840
void __cdecl BG_SetPerk(unsigned int *perks, unsigned int perkIndex)
{
    unsigned int tmp2;

    if ( !perks
        && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../bgame/bg_perks.h", 145, 0, "%s", "perks") )
    {
        __debugbreak();
    }
    if ( perkIndex >= 0x34
        && !Assert_MyHandler(
                    "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../bgame/bg_perks.h",
                    146,
                    0,
                    "perkIndex doesn't index PERK_COUNT\n\t%i not in [0, %i)",
                    perkIndex,
                    52) )
    {
        __debugbreak();
    }
    tmp2 = perks[1] | ((unsigned __int64)(1LL << perkIndex) >> 32);
    *perks |= 1LL << perkIndex;
    perks[1] = tmp2;
}

// Decomp: CoDMPServer.c:381873
void __cdecl PlayerCmd_HasPerk(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    bool HasPerk;
    gentity_s *pSelf;
    char *perkName;
    unsigned int perkIndex;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        4159,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    perkName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    perkIndex = BG_GetPerkIndexForName(perkName);
    if ( perkIndex == 52 )
    {
        fmtMsg2 = va("Unknown perk: %s\n", perkName);
        Scr_Error(fmtMsg2, 0);
    }
    HasPerk = BG_HasPerk(pSelf->client->ps.perks, perkIndex);
    Scr_AddBool(HasPerk, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:381919
void __cdecl PlayerCmd_GetPerks(scr_entref_t entref)
{
    const char *fmtMsg;
    char *PerkNameForIndex;
    gentity_s *pSelf;
    unsigned int perkIndex;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        4184,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for ( perkIndex = 0; perkIndex < 0x34; ++perkIndex )
    {
        if ( BG_HasPerk(pSelf->client->ps.perks, perkIndex) )
        {
            PerkNameForIndex = (char *)BG_GetPerkNameForIndex(perkIndex);
            Scr_AddString(PerkNameForIndex, SCRIPTINSTANCE_SERVER);
            Scr_AddArray(SCRIPTINSTANCE_SERVER);
        }
    }
}

// Decomp: CoDMPServer.c:381964
void __cdecl PlayerCmd_UnsetPerk(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    gentity_s *pSelf;
    char *perkName;
    unsigned int perkIndex;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        4214,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    perkName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    perkIndex = BG_GetPerkIndexForName(perkName);
    if ( perkIndex == 52 )
    {
        fmtMsg2 = va("Unknown perk: %s\n", perkName);
        Scr_Error(fmtMsg2, 0);
    }
    BG_UnsetPerk(pSelf->client->ps.perks, perkIndex);
    BG_UnsetPerk(pSelf->client->sess.cs.perks, perkIndex);
}

// Decomp: CoDMPServer.c:382009
void __cdecl BG_UnsetPerk(unsigned int *perks, unsigned int perkIndex)
{
    unsigned int tmp2;

    if ( !perks
        && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../bgame/bg_perks.h", 160, 0, "%s", "perks") )
    {
        __debugbreak();
    }
    if ( perkIndex >= 0x34
        && !Assert_MyHandler(
                    "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../bgame/bg_perks.h",
                    161,
                    0,
                    "perkIndex doesn't index PERK_COUNT\n\t%i not in [0, %i)",
                    perkIndex,
                    52) )
    {
        __debugbreak();
    }
    tmp2 = perks[1] & ~((unsigned __int64)(1LL << perkIndex) >> 32);
    *perks &= ~(unsigned int)(1LL << perkIndex);
    perks[1] = tmp2;
}

// Decomp: CoDMPServer.c:382042
void __cdecl PlayerCmd_ClearPerks(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        4240,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    BG_ClearPerks(pSelf->client->ps.perks);
    BG_ClearPerks(pSelf->client->sess.cs.perks);
}

// Decomp: CoDMPServer.c:382077
void __cdecl BG_ClearPerks(unsigned int *perks)
{
    if ( !perks
        && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../bgame/bg_perks.h", 175, 0, "%s", "perks") )
    {
        __debugbreak();
    }
    *perks = 0;
    perks[1] = 0;
}

// Decomp: CoDMPServer.c:382095
void __cdecl PlayerCmd_EnableInvulnerability(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        4261,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pSelf->client->ps.otherFlags |= 1u;
}

// Decomp: CoDMPServer.c:382129
void __cdecl PlayerCmd_DisableInvulnerability(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        4278,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pSelf->client->ps.otherFlags &= ~1u;
}

// Decomp: CoDMPServer.c:382163
void __cdecl PlayerCmd_SetRank(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    const char *fmtMsg3;
    gentity_s *pSelf;
    int prestige;
    int rank;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        4296,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    rank = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)rank < 0x100 )
    {
        pSelf->client->sess.cs.rank = rank;
        if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) >= 2 )
        {
            prestige = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
            if ( (unsigned int)prestige < 0x100 )
            {
                pSelf->client->sess.cs.prestige = prestige;
            }
            else
            {
                fmtMsg3 = va("'%i' is an illegal prestige value.    Must be less than %i.\n", prestige, 256);
                Scr_Error(fmtMsg3, 0);
            }
        }
    }
    else
    {
        fmtMsg2 = va("'%i' is an illegal rank value.    Must be less than %i.\n", rank, 256);
        Scr_Error(fmtMsg2, 0);
    }
}

// Decomp: CoDMPServer.c:382223
void __cdecl PlayerCmd_SetLastStandPrevWeap(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;
    char *weapName;
    int weapIndex;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        4339,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    weapName = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
    weapIndex = G_GetWeaponIndexForName(weapName);
    AssignToSmallerType<unsigned short>(&pSelf->client->ps.lastStandPrevWeapon, weapIndex);
}

// Decomp: CoDMPServer.c:382261
void __cdecl PlayerCmd_ClientSysSetState(scr_entref_t entref)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    const char *fmtMsg3;
    const char *fmtMsg4;
    unsigned int j;
    gentity_s *pSelf;
    char str[1028];
    const char *pNewState;
    char szConfigString[1024];
    int i;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        4371,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    i = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( (unsigned int)i <= 8 )
    {
        SV_GetConfigstring(i + 1538, szConfigString, 1024);
        if ( szConfigString[0] )
        {
            szConfigString[0] = 0;
            str[0] = 0;
            pNewState = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
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
            fmtMsg4 = va("%c %i %s", 57, i, szConfigString);
            SV_GameSendServerCommand(pSelf->client->ps.clientNum, SV_CMD_RELIABLE, fmtMsg4);
        }
        else
        {
            fmtMsg3 = va("ClientSysSetState - state index (%i) unregistered.    Use ClientSysRegister first.", i);
            Scr_Error(fmtMsg3, 1);
        }
    }
    else
    {
        fmtMsg2 = va("ClientSysSetState - state index (%i) out of bounds (0 - %i)", i, 8);
        Scr_Error(fmtMsg2, 1);
    }
}

// Decomp: CoDMPServer.c:382335
void __cdecl PlayerCmd_WeaponLockStart(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *Entity;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        4432,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        Entity = Scr_GetEntity(0);
        pSelf->client->ps.weapLockFlags |= 1u;
        pSelf->client->ps.weapLockFlags &= ~2u;
        pSelf->client->ps.weapLockedEntnum = Entity->s.number;
        pSelf->client->ps.weapLockFlags &= ~4u;
        pSelf->client->ps.weapLockFlags &= ~8u;
    }
    else
    {
        Scr_Error("Incorrect number of parameters.\n", 0);
    }
}

// Decomp: CoDMPServer.c:382382
void __cdecl PlayerCmd_WeaponLockFinalize(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *Entity;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        4464,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        Entity = Scr_GetEntity(0);
        pSelf->client->ps.weapLockFlags |= 1u;
        pSelf->client->ps.weapLockFlags |= 2u;
        pSelf->client->ps.weapLockedEntnum = Entity->s.number;
        if ( G_TargetAttackProfileTop(Entity) )
            pSelf->client->ps.weapLockFlags |= 4u;
        else
            pSelf->client->ps.weapLockFlags |= 8u;
    }
    else
    {
        Scr_Error("Incorrect number of parameters.\n", 0);
    }
}

// Decomp: CoDMPServer.c:382431
void __cdecl PlayerCmd_WeaponLockFree(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        4498,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pSelf->client->ps.weapLockFlags &= ~1u;
    pSelf->client->ps.weapLockFlags &= ~2u;
    pSelf->client->ps.weapLockedEntnum = 1023;
    pSelf->client->ps.weapLockFlags &= ~4u;
    pSelf->client->ps.weapLockFlags &= ~8u;
}

// Decomp: CoDMPServer.c:382469
void __cdecl PlayerCmd_WeaponLockTargetTooClose(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        4521,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( Scr_GetInt(0, SCRIPTINSTANCE_SERVER) )
        pSelf->client->ps.weapLockFlags |= 0x10u;
    else
        pSelf->client->ps.weapLockFlags &= ~0x10u;
}

// Decomp: CoDMPServer.c:382506
void __cdecl PlayerCmd_WeaponLockNoClearance(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        4543,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( Scr_GetInt(0, SCRIPTINSTANCE_SERVER) )
        pSelf->client->ps.weapLockFlags |= 0x20u;
    else
        pSelf->client->ps.weapLockFlags &= ~0x20u;
}

// Decomp: CoDMPServer.c:382543
void __cdecl PlayerCmd_ReLinkToTurret(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        4564,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pSelf->client->ps.viewlocked_entNum = Scr_GetEntity(0)->s.number;
    pSelf->client->ps.eFlags |= 0x300u;
}

// Decomp: CoDMPServer.c:382578
void __cdecl PlayerCmd_CarryTurret(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;
    float anglesOffset[3];
    float offset[3];
    gentity_s *ent;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        4586,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    ent = Scr_GetEntity(0);
    Scr_GetVector(1u, offset, SCRIPTINSTANCE_SERVER);
    Scr_GetVector(2u, anglesOffset, SCRIPTINSTANCE_SERVER);
    ent->r.svFlags &= ~4u;
    ent->r.contents = 0;
    pSelf->client->ps.weapFlags |= 0x400000u;
    G_EntLinkToWithOffset(ent, pSelf, 0, offset, anglesOffset);
}

// Decomp: CoDMPServer.c:382621
void __cdecl PlayerCmd_StopCarryTurret(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;
    float origin[3];
    float angles[3];
    gentity_s *ent;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        4618,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    ent = Scr_GetEntity(0);
    G_EntUnlink(ent);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 1 )
    {
        Scr_GetVector(1u, origin, SCRIPTINSTANCE_SERVER);
        G_SetOrigin(ent, origin);
    }
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_SERVER) > 2 )
    {
        Scr_GetVector(2u, angles, SCRIPTINSTANCE_SERVER);
        G_SetAngle(ent, angles);
    }
    ent->r.svFlags |= 4u;
    ent->r.contents &= ~1u;
    ent->r.contents = 227008;
    pSelf->client->ps.weapFlags &= ~0x400000u;
}

// Decomp: CoDMPServer.c:382673
void __cdecl PlayerCmd_LinkGuidedMissileCamera(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        4658,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pSelf->client->ps.eFlags2 |= 0x10000000u;
    pSelf->client->ps.eFlags2 |= 0x40000u;
}

// Decomp: CoDMPServer.c:382708
void __cdecl PlayerCmd_UnlinkGuidedMissileCamera(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        4678,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    // BlackOpsMP.retail.c:48646 (unlinkguidedmissilecamera / loc_51EDD0): clears eFlags2 guided bits only.
    pSelf->client->ps.eFlags2 &= ~0x10000000u;
    pSelf->client->ps.eFlags2 &= ~0x40000u;
}

// Decomp: CoDMPServer.c:382743
void __cdecl PlayerCmd_isCarryingTurret(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        4698,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    pSelf->client->ps.bCarryingTurret = Scr_GetInt(0, SCRIPTINSTANCE_SERVER) != 0;
}

// LWSS ADD — BlackOpsMP.retail.c:48640 setturrethint / loc_82E5E0 (retail MP extension).
// Client HUD reads ps.weapFlags 0x400000 / 0x800000 (cg_newDraw_mp.cpp / sub_7D9FC0).
void PlayerCmd_SetTurrentHint(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;
    int hintState;
    playerState_s *ps;

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        8382,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_Error(SCRIPTINSTANCE_SERVER, "Usage: player setTurretHint( <hintState> );", 0);
    ps = &pSelf->client->ps;
    hintState = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    ps->weapFlags &= ~0x80000u;
    switch ( hintState )
    {
    case 1:
        ps->weapFlags |= 0x400000u;
        ps->weapFlags &= ~0x800000u;
        break;
    case 2:
    case 3:
        ps->weapFlags |= 0x800000u;
        ps->weapFlags &= ~0x400000u;
        break;
    default:
        ps->weapFlags &= 0xFF3FFFFFu;
        break;
    }
}
// LWSS END

// Decomp: CoDMPServer.c:382777
void __cdecl PlayerCmd_CanPlayerPlaceTurret(scr_entref_t entref)
{
    const char *fmtMsg;
    gentity_s *pSelf;
    unsigned int result;
    float origin[3];
    float angles[3];

    if ( entref.classnum )
    {
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                        4718,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        pSelf = &g_entities[entref.entnum];
        if ( !pSelf->client )
        {
            fmtMsg = va("entity %i is not a player", entref.entnum);
            Scr_ObjectError(fmtMsg, SCRIPTINSTANCE_SERVER);
        }
    }
    if ( !pSelf->client
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_client_script_cmd_mp.cpp",
                    4720,
                    0,
                    "%s",
                    "pSelf->client") )
    {
        __debugbreak();
    }
    result = Turret_PlaceTurret_Server(&pSelf->client->ps, origin, angles);
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    Scr_AddBool(result, SCRIPTINSTANCE_SERVER);
    Scr_AddArrayStringIndexed(scr_const.result, SCRIPTINSTANCE_SERVER);
    Scr_AddVector(origin, SCRIPTINSTANCE_SERVER);
    Scr_AddArrayStringIndexed(scr_const.origin, SCRIPTINSTANCE_SERVER);
    Scr_AddVector(angles, SCRIPTINSTANCE_SERVER);
    Scr_AddArrayStringIndexed(scr_const.angles, SCRIPTINSTANCE_SERVER);
}

void (__cdecl *__cdecl Player_GetMethod(const char **pName))(scr_entref_t)
{
    unsigned int i;

    for ( i = 0; i < ARRAY_COUNT(methods); ++i )
    {
        if ( !strcmp(*pName, methods[i].actionString) )
        {
            *pName = methods[i].actionString;
            return methods[i].actionFunc;
        }
    }
    return 0;
}

