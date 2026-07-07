#include "g_cmds_mp.h"
#include "g_main_mp.h"
#include <server/sv_game.h>
#include <server_mp/sv_main_mp.h>
#include <server_mp/sv_init_mp.h>
#include <game/g_weapon.h>
#include <bgame/bg_misc.h>
#include "g_utils_mp.h"
#include <bgame/bg_weapons_ammo.h>
#include "g_client_script_cmd_mp.h"
#include "g_combat_mp.h"
#include <client_mp/g_client_mp.h>
#include "g_active_mp.h"
#include "g_spawn_mp.h"
#include <clientscript/scr_const.h>
#include "g_team_mp.h"
#include <game/g_load_utils.h>
#include <server_mp/sv_main_pc_mp.h>
#include <client_mp/sv_client_mp.h>
#include "g_misc_mp.h"
#include <clientscript/cscr_vm.h>
#include <game/actor_fields.h>

// Decomp: CoDMPServer.c:382848
int __cdecl CheatsOk(gentity_s *ent)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    bool bCreateFX;

    if ( g_cheats->current.enabled )
    {
        bCreateFX = Dvar_FindVar("createfx") != 0;
        if ( ent->health > 0 || bCreateFX )
        {
            return 1;
        }
        else
        {
            fmtMsg2 = va("%c \"GAME_MUSTBEALIVECOMMAND\"", 101);
            SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg2);
            return 0;
        }
    }
    else
    {
        fmtMsg = va("%c \"GAME_CHEATSNOTENABLED\"", 101);
        SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg);
        return 0;
    }
}

char line[1024];
// Decomp: CoDMPServer.c:382881
char *__cdecl ConcatArgs(int start)
{
    unsigned int tmp1; // kr00_4
    int c;
    int len;
    char arg[1028];

    len = 0;
    c = SV_Cmd_Argc();
    while ( start < c )
    {
        SV_Cmd_ArgvBuffer(start, arg, 1024);
        tmp1 = strlen(arg);
        if ( (int)(tmp1 + len) >= 1023 )
            break;
        memcpy((unsigned __int8 *)&line[len], (unsigned __int8 *)arg, tmp1);
        len += tmp1;
        if ( start != c - 1 )
            line[len++] = 32;
        ++start;
    }
    line[len] = 0;
    return line;
}

// Decomp: CoDMPServer.c:382907
int __cdecl SV_Cmd_Argc()
{
    if ( sv_cmd_args.nesting >= 8u
        && !Assert_MyHandler(
                    "c:\\projects_pc\\cod\\codsrc\\src\\game_mp\\../qcommon/cmd.h",
                    219,
                    0,
                    "sv_cmd_args.nesting doesn't index CMD_MAX_NESTING\n\t%i not in [0, %i)",
                    sv_cmd_args.nesting,
                    8) )
    {
        __debugbreak();
    }
    return sv_cmd_args.argc[sv_cmd_args.nesting];
}

// Decomp: CoDMPServer.c:382925
void __cdecl G_setfog(char *fogstring)
{
    float fDensity;
    float sunFogDir[3];
    float clr[3];
    float fFar;
    float sunFogColor[3];
    float fNear;
    int time;
    float sunAngEnd;
    float sunAngStart;
    float fogMaxOpacity;

    SV_SetConfigstring(10, fogstring);
    level.fFogOpaqueDist = FLT_MAX;
    level.fFogOpaqueDistSqrd = FLT_MAX;
    if ( sscanf(
                 fogstring,
                 "%f %f %f %f %f %f %i %f %f %f %f %f %f %f %f %f",
                 &fNear,
                 &fFar,
                 &fDensity,
                 clr,
                 &clr[1],
                 &clr[2],
                 &time,
                 sunFogColor,
                 &sunFogColor[1],
                 &sunFogColor[2],
                 sunFogDir,
                 &sunFogDir[1],
                 &sunFogDir[2],
                 &sunAngStart,
                 &sunAngEnd,
                 &fogMaxOpacity) == 16
        && fDensity >= 1.0 )
    {
        level.fFogOpaqueDist = (float)((float)(fFar - fNear) * 1.0) + fNear;
        level.fFogOpaqueDistSqrd = level.fFogOpaqueDist * level.fFogOpaqueDist;
    }
}

// Decomp: CoDMPServer.c:382975
void __cdecl Cmd_Give_f(gentity_s *ent)
{
    unsigned __int8 PlayerWeaponModel;
    int tmp2;
    unsigned int tmp3;
    int iClipSize;
    int tmp5;
    int i;
    int slot;
    gentity_s *it_ent;
    char *name;
    bool give_all;
    char *amt;
    const WeaponVariantDef *weapVariantDef;
    int amount;
    unsigned int weapIndex;
    int weapAlt;
    const WeaponDef *weapDef;
    const gitem_s *it;

    if ( CheatsOk(ent) )
    {
        amt = ConcatArgs(2);
        amount = atoi(amt);
        name = ConcatArgs(1);
        if ( name )
        {
            if ( strlen(name) )
            {
                if ( !(give_all = I_stricmp(name, "all") == 0) && I_strnicmp(name, "health", 6)
                    || (!amount ? (ent->health = ent->client->ps.stats[2]) : (ent->health += amount), give_all) )
                {
                    if ( !give_all && I_stricmp(name, "weapons") )
                        goto LABEL_51;
                    for ( weapIndex = 1; weapIndex < BG_GetNumWeapons(); ++weapIndex )
                    {
                        if ( BG_CanPlayerHaveWeapon(weapIndex) )
                        {
                            BG_TakePlayerWeapon(&ent->client->ps, weapIndex);
                            G_GivePlayerWeapon(&ent->client->ps, weapIndex, 0, 0);
                        }
                    }
                    if ( give_all )
                    {
LABEL_51:
                        if ( !give_all && I_strnicmp(name, "ammo", 4) )
                            goto LABEL_35;
                        if ( amount )
                        {
                            if ( ent->client->ps.weapon )
                            {
                                PlayerWeaponModel = BG_GetPlayerWeaponModel(&ent->client->ps, ent->client->ps.weapon);
                                Add_Ammo(ent, ent->client->ps.weapon, PlayerWeaponModel, amount, 1);
                            }
                        }
                        else
                        {
                            for ( slot = 0; slot < 15; ++slot )
                                Add_Ammo(ent, ent->client->ps.heldWeapons[slot].weapon, 0, 998, 1);
                        }
                        if ( give_all )
                        {
LABEL_35:
                            if ( I_strnicmp(name, "allammo", 7) )
                                goto LABEL_50;
                            if ( !amount )
                                goto LABEL_50;
                            for ( i = 0; i < 15; Add_Ammo(ent, ent->client->ps.heldWeapons[i++].weapon, 0, amount, 1) )
                                ;
                            if ( give_all )
                            {
LABEL_50:
                                if ( !give_all )
                                {
                                    level.initializing = 1;
                                    weapVariantDef = 0;
                                    weapAlt = 0;
                                    it = G_FindItem(name);
                                    if ( it )
                                    {
                                        it_ent = G_Spawn();
                                        it_ent->r.currentOrigin[0] = ent->r.currentOrigin[0];
                                        it_ent->r.currentOrigin[1] = ent->r.currentOrigin[1];
                                        it_ent->r.currentOrigin[2] = ent->r.currentOrigin[2];
                                        G_GetItemClassname(it, &it_ent->classname);
                                        G_SpawnItem(it_ent, it);
                                        it_ent->active = 1;
                                        if ( it->giType == IT_WEAPON )
                                        {
                                            weapAlt = it_ent->item[0].index % 2048;
                                            weapVariantDef = BG_GetWeaponVariantDef(weapAlt);
                                            weapDef = BG_GetWeaponDef(weapAlt);
                                            if ( weapDef->offhandClass == OFFHAND_CLASS_FLASH_GRENADE )
                                            {
                                                ent->client->ps.offhandSecondary = PLAYER_OFFHAND_SECONDARY_FLASH;
                                            }
                                            else if ( weapDef->offhandClass == OFFHAND_CLASS_SMOKE_GRENADE )
                                            {
                                                ent->client->ps.offhandSecondary = PLAYER_OFFHAND_SECONDARY_SMOKE;
                                            }
                                        }
                                        Touch_Item(it_ent, ent, 0);
                                        it_ent->active = 0;
                                        if ( it_ent->r.inuse )
                                            G_FreeEntity(it_ent);
                                        if ( it->giType == IT_WEAPON )
                                        {
                                            iClipSize = weapVariantDef->iClipSize;
                                            tmp2 = BG_ClipForWeapon(weapAlt);
                                            BG_SetAmmoInClip(&ent->client->ps, tmp2, iClipSize);
                                            tmp5 = BG_GetStartAmmo(weapAlt) - weapVariantDef->iClipSize;
                                            tmp3 = BG_AmmoForWeapon(weapAlt);
                                            BG_SetAmmoInPool(&ent->client->ps, tmp3, tmp5);
                                        }
                                        level.initializing = 0;
                                    }
                                    else
                                    {
                                        level.initializing = 0;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// Decomp: CoDMPServer.c:383108
void __cdecl Cmd_Take_f(gentity_s *ent)
{
    const WeaponVariantDef *weapVarDef;
    int tmp3;
    const WeaponVariantDef *weapVarDef2;
    int AmmoNotInClip;
    int tmp6;
    int j;
    int i;
    int slot;
    char *name;
    char *amt;
    int amount;
    unsigned int weapIndex;
    unsigned int weapAlt;
    unsigned int weapIndexb;
    BOOL take_all;

    if (CheatsOk(ent))
    {
        amt = ConcatArgs(2);
        amount = atoi(amt);
        name = ConcatArgs(1);
        if (name)
        {
            if (strlen(name))
            {
                take_all = I_stricmp(name, "all") == 0;
                if (!take_all)
                {
                    if (I_strnicmp(name, "health", 6))
                        goto LABEL_22;
                }
                if (amount)
                {
                    ent->health -= amount;
                    if (ent->health < 1)
                        ent->health = 1;
                }
                else
                {
                    ent->health = 1;
                }
                if (take_all)
                {
                LABEL_22:
                    if (!take_all && I_stricmp(name, "weapons"))
                        goto LABEL_35;
                    for (slot = 0; slot < 15; ++slot)
                    {
                        if (ent->client->ps.heldWeapons[slot].weapon)
                            BG_TakePlayerWeapon(&ent->client->ps, ent->client->ps.heldWeapons[slot].weapon);
                    }
                    if (ent->client->ps.weapon)
                    {
                        ent->client->ps.weapon = 0;
                        G_SelectWeaponIndex(ent - g_entities, 0);
                    }
                    if (take_all)
                    {
                    LABEL_35:
                        if (!take_all && I_strnicmp(name, "ammo", 4))
                            goto LABEL_37;
                        if (amount)
                        {
                            if (ent->client->ps.weapon)
                            {
                                weapIndex = ent->client->ps.weapon;
                                BG_AddAmmoToPool(&ent->client->ps, weapIndex, -amount);
                                if (BG_GetAmmoNotInClip(&ent->client->ps, weapIndex) < 0)
                                {
                                    AmmoNotInClip = BG_GetAmmoNotInClip(&ent->client->ps, weapIndex);
                                    weapVarDef = BG_GetWeaponVariantDef(weapIndex);
                                    BG_AddAmmoToClip(&ent->client->ps, weapVarDef->iClipIndex, AmmoNotInClip);
                                    BG_SetAmmoInPool(&ent->client->ps, weapIndex, 0);
                                }
                            }
                        }
                        else
                        {
                            for (i = 0; i < 15; ++i)
                            {
                                weapAlt = ent->client->ps.heldWeapons[i].weapon;
                                if (weapAlt)
                                {
                                    BG_SetAmmoInPool(&ent->client->ps, weapAlt, 0);
                                    tmp3 = BG_ClipForWeapon(weapAlt);
                                    BG_SetAmmoInClip(&ent->client->ps, tmp3, 0);
                                }
                            }
                        }
                        if (take_all)
                        {
                        LABEL_37:
                            if (!I_strnicmp(name, "allammo", 7) && amount)
                            {
                                for (j = 0; j < 15; ++j)
                                {
                                    weapIndexb = ent->client->ps.heldWeapons[j].weapon;
                                    if (weapIndexb)
                                    {
                                        BG_AddAmmoToPool(&ent->client->ps, weapIndexb, -amount);
                                        if (BG_GetAmmoNotInClip(&ent->client->ps, weapIndexb) < 0)
                                        {
                                            tmp6 = BG_GetAmmoNotInClip(&ent->client->ps, weapIndexb);
                                            weapVarDef2 = BG_GetWeaponVariantDef(weapIndexb);
                                            BG_AddAmmoToClip(&ent->client->ps, weapVarDef2->iClipIndex, tmp6);
                                            BG_SetAmmoInPool(&ent->client->ps, weapIndexb, 0);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// Decomp: CoDMPServer.c:383229
void __cdecl Cmd_God_f(gentity_s *ent)
{
    const char *fmtMsg;
    const char *tmp2;

    if ( CheatsOk(ent) )
    {
        ent->flags ^= 1u;
        if ( (ent->flags & 1) != 0 )
            tmp2 = "GAME_GODMODE_ON";
        else
            tmp2 = "GAME_GODMODE_OFF";
        fmtMsg = va("%c \"%s\"", 101, tmp2);
        SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg);
    }
}

// Decomp: CoDMPServer.c:383247
void __cdecl Cmd_DemiGod_f(gentity_s *ent)
{
    const char *fmtMsg;
    const char *tmp2;

    if ( CheatsOk(ent) )
    {
        ent->flags ^= 2u;
        if ( (ent->flags & 2) != 0 )
            tmp2 = "GAME_DEMI_GODMODE_ON";
        else
            tmp2 = "GAME_DEMI_GODMODE_OFF";
        fmtMsg = va("%c \"%s\"", 101, tmp2);
        SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg);
    }
}

// Decomp: CoDMPServer.c:383265
void __cdecl Cmd_Notarget_f(gentity_s *ent)
{
    const char *fmtMsg;

    if ( CheatsOk(ent) )
    {
        ent->flags ^= 4u;
        if ( (ent->flags & 4) != 0 )
            fmtMsg = va("%c \"%s\"", 101, "GAME_NOTARGETON");
        else
            fmtMsg = va("%c \"%s\"", 101, "GAME_NOTARGETOFF");
        SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg);
    }
}

// Decomp: CoDMPServer.c:383281
void __cdecl Cmd_Noclip_f(gentity_s *ent)
{
    const char *fmtMsg;

    if ( CheatsOk(ent) )
    {
        if ( (ent->client->flags & 1) != 0 )
        {
            ent->client->flags &= ~1u;
            fmtMsg = va("%c \"%s\"", 101, "GAME_NOCLIPOFF");
        }
        else
        {
            ent->client->flags |= 1u;
            fmtMsg = va("%c \"%s\"", 101, "GAME_NOCLIPON");
        }
        SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg);
    }
}

// Decomp: CoDMPServer.c:383302
void __cdecl Cmd_UFO_f(gentity_s *ent)
{
    const char *fmtMsg;

    if ( CheatsOk(ent) )
    {
        if ( (ent->client->flags & 2) != 0 )
        {
            ent->client->flags &= ~2u;
            fmtMsg = va("%c \"%s\"", 101, "GAME_UFOOFF");
        }
        else
        {
            ent->client->flags |= 2u;
            fmtMsg = va("%c \"%s\"", 101, "GAME_UFOON");
        }
        SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg);
    }
}

// Decomp: CoDMPServer.c:383323
void __cdecl Cmd_Kill_f(gentity_s *ent)
{
    if (!ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_cmds_mp.cpp", 666, 0, "%s", "ent->client"))
    {
        __debugbreak();
    }
    if (ent->client->sess.connected == CON_DISCONNECTED
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_cmds_mp.cpp",
            667,
            0,
            "%s",
            "ent->client->sess.connected != CON_DISCONNECTED"))
    {
        __debugbreak();
    }
    if (ent->client->sess.sessionState == SESS_STATE_PLAYING && CheatsOk(ent))
    {
        iassert(bgs == 0);
        bgs = &level_bgs;
        ent->flags &= 0xFFFFFFFC;
        ent->health = 0;
        ent->client->ps.stats[0] = 0;
        player_die(ent, ent, ent, 100000, 0xDu, 0, 0, HITLOC_NONE, 0);
        iassert(bgs == &level_bgs);
        bgs = 0;
    }
}

// Decomp: CoDMPServer.c:383383
void __cdecl StopFollowing(gentity_s *ent)
{
    gclient_s *client;
    float vAngles[3];
    col_context_t context;
    float vEnd[3];
    float vMins[3];
    trace_t trace;
    float vForward[3];
    float vPos[3];
    float vUp[3];
    float vMaxs[3];

    memset(&trace, 0, 16);
    //col_context_t::col_context_t(&context);
    client = ent->client;
    if ( !client && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_cmds_mp.cpp", 709, 0, "%s", "client") )
        __debugbreak();
    client->sess.forceSpectatorClient = -1;
    client->sess.killCamEntity = -1;
    client->sess.killCamTargetEntity = ent->s.number;
    client->spectatorClient = -1;
    if ( (client->ps.otherFlags & 2) != 0 )
    {
        client->ps.eFlags &= 0xFFFFBCFF;
        client->ps.viewlocked = PLAYERVIEWLOCK_NONE;
        client->ps.viewlocked_entNum = 1023;
        G_GetPlayerViewOrigin(&client->ps, vPos);
        BG_GetPlayerViewDirection(&client->ps, vForward, 0, vUp);
        vAngles[0] = client->ps.viewangles[0];
        vAngles[1] = client->ps.viewangles[1];
        vAngles[2] = client->ps.viewangles[2];
        vAngles[0] = vAngles[0] + 15.0;
        vEnd[0] = (float)(-40.0 * vForward[0]) + vPos[0];
        vEnd[1] = (float)(-40.0 * vForward[1]) + vPos[1];
        vEnd[2] = (float)(-40.0 * vForward[2]) + vPos[2];
        vEnd[0] = (float)(10.0 * vUp[0]) + vEnd[0];
        vEnd[1] = (float)(10.0 * vUp[1]) + vEnd[1];
        vEnd[2] = (float)(10.0 * vUp[2]) + vEnd[2];
        vMins[0] = -8.0f;
        vMins[1] = -8.0f;
        vMins[2] = -8.0f;
        vMaxs[0] = 8.0f;
        vMaxs[1] = 8.0f;
        vMaxs[2] = 8.0f;
        G_TraceCapsule(&trace, vPos, vMins, vMaxs, vEnd, 1023, 0x810011, &context);
        Vec3Lerp(vPos, vEnd, trace.fraction, vPos);
        AssignToSmallerType<unsigned char>(&client->ps.clientNum, ent - g_entities);
        client->ps.pm_flags &= 0xFFFEFFEF;
        client->ps.weapFlags &= ~0x40u;
        client->ps.otherFlags &= ~2u;
        client->ps.fWeaponPosFrac = 0.0f;
        G_SetOrigin(ent, vPos);
        client->ps.origin[0] = vPos[0];
        client->ps.origin[1] = vPos[1];
        client->ps.origin[2] = vPos[2];
        SetClientViewAngle(ent, vAngles);
        if ( !ent->tagInfo )
            ent->r.currentAngles[0] = 0.0f;
        client->ps.shellshockIndex = 0;
        client->ps.shellshockTime = 0;
        client->ps.shellshockDuration = 0;
        client->ps.predictableEventSequence = 0;
        client->ps.predictableEventSequenceOld = 0;
        client->ps.unpredictableEventSequence = 0;
        client->ps.unpredictableEventSequenceOld = 0;
        client->ps.predictableEvents[0] = 0;
        client->ps.predictableEvents[1] = 0;
        client->ps.predictableEvents[2] = 0;
        client->ps.predictableEvents[3] = 0;
        client->ps.unpredictableEvents[0] = 0;
        client->ps.unpredictableEvents[1] = 0;
        client->ps.unpredictableEvents[2] = 0;
        client->ps.unpredictableEvents[3] = 0;
    }
}

// Decomp: CoDMPServer.c:383508
int __cdecl Cmd_FollowCycle_f(gentity_s *ent, int dir)
{
    int tmp4;
    playerState_s tmp5;
    int health;
    int clientNum;
    int otherFlags;
    clientState_s tmp9;
    playerState_s *ps;

    ps = &tmp5;
    // LWSS: this is likely some 128bit aligned alloca
    //if (((unsigned __int8)ps & 0x7F) != 0
    //    && !Assert_MyHandler(
    //        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_cmds_mp.cpp",
    //        784,
    //        0,
    //        "%s",
    //        "( (unsigned int)ps & (128-1) ) == 0"))
    //{
    //    __debugbreak();
    //}
    if (dir != 1 && dir != -1)
        Com_Error(ERR_DROP, "Cmd_FollowCycle_f: bad dir %i", dir);
    if (!ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_cmds_mp.cpp", 792, 0, "%s", "ent->client"))
    {
        __debugbreak();
    }
    if (ent->client->sess.sessionState != SESS_STATE_SPECTATOR)
        return 0;
    if (ent->client->sess.forceSpectatorClient >= 0)
        return 0;
    clientNum = ent->client->spectatorClient;
    if (clientNum < 0)
        clientNum = 0;
    tmp4 = clientNum;
    do
    {
        clientNum += dir;
        if (clientNum >= level.maxclients)
            clientNum = 0;
        if (clientNum < 0)
            clientNum = level.maxclients - 1;
        if (SV_GetArchivedClientInfo(clientNum, &ent->client->sess.archiveTime, ps, &tmp9, 0, &health, &otherFlags))
        {
            if ((otherFlags & 4) == 0
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_cmds_mp.cpp",
                    822,
                    0,
                    "%s",
                    "otherFlags & POF_PLAYER"))
            {
                __debugbreak();
            }
            if (G_ClientCanSpectateTeamOrLocalPlayer(ent->client, &tmp9))
            {
                ent->client->spectatorClient = clientNum;
                ent->client->sess.sessionState = SESS_STATE_SPECTATOR;
                Scr_AddEntity(&g_entities[clientNum], SCRIPTINSTANCE_SERVER);
                Scr_Notify(ent, scr_const.spectator_cycle, 1u);
                return 1;
            }
        }
    } while (clientNum != tmp4);
    return 0;
}

// Decomp: CoDMPServer.c:383588
bool __cdecl G_IsPlaying(gentity_s *ent)
{
    if ( !ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_cmds_mp.cpp", 848, 0, "%s", "ent->client") )
    {
        __debugbreak();
    }
    if ( ent->client->sess.connected == CON_DISCONNECTED
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_cmds_mp.cpp",
                    849,
                    0,
                    "%s",
                    "ent->client->sess.connected != CON_DISCONNECTED") )
    {
        __debugbreak();
    }
    return ent->client->sess.sessionState == SESS_STATE_PLAYING;
}

// Decomp: CoDMPServer.c:383616
void __cdecl G_Say(gentity_s *ent, gentity_s *target, int mode, const char *chatText)
{
    const char *tmp4;
    int tmp5;
    int Guid;
    int tmp7;
    int number;
    int j;
    char cleanname[68];
    gentity_s *other;
    const char *pszTeamString;
    int color;
    char text[156];

    pszTeamString = "";
    if ( mode == 1 && ent->client->sess.cs.team != TEAM_AXIS )
        mode = ent->client->sess.cs.team == TEAM_ALLIES;
    tmp4 = CS_DisplayName(&ent->client->sess.cs, 3);
    I_strncpyz(cleanname, tmp4, 64);
    I_CleanStr(cleanname);
    if ( mode == 1 )
    {
        if ( ent->client->sess.cs.team != TEAM_AXIS
            && ent->client->sess.cs.team != TEAM_ALLIES
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_cmds_mp.cpp",
                        968,
                        0,
                        "%s",
                        "(ent->client->sess.cs.team == TEAM_AXIS) || (ent->client->sess.cs.team == TEAM_ALLIES)") )
        {
            __debugbreak();
        }
        number = ent->s.number;
        Guid = SV_GetGuid(ent->s.number);
        G_LogPrintf("sayteam;%d;%d;%s;%s\n", Guid, number, cleanname, chatText);
        if ( ent->client->sess.cs.team == TEAM_AXIS )
            pszTeamString = "GAME_AXIS";
        else
            pszTeamString = "GAME_ALLIES";
        color = 53;
    }
    else if ( mode == 2 )
    {
        color = 51;
    }
    else
    {
        tmp7 = ent->s.number;
        tmp5 = SV_GetGuid(ent->s.number);
        G_LogPrintf("say;%d;%d;%s;%s\n", tmp5, tmp7, cleanname, chatText);
        color = 55;
    }
    I_strncpyz(text, chatText, 150);
    if ( target )
    {
        G_SayTo(ent, target, mode, color, pszTeamString, cleanname, text);
    }
    else
    {
        Com_Printf(15, "%s%s\n", cleanname, text);
        for ( j = 0; j < level.maxclients; ++j )
        {
            other = &g_entities[j];
            G_SayTo(ent, other, mode, color, pszTeamString, cleanname, text);
        }
    }
}

// Decomp: CoDMPServer.c:383687
void __cdecl G_SayTo(
                gentity_s *ent,
                gentity_s *other,
                int mode,
                int color,
                const char *teamString,
                const char *cleanname,
                const char *message)
{
    const char *fmtMsg;
    team_t team;
    char szStateString[68];
    const char *team_color;
    char name[132];

    if ( other
        && other->r.inuse
        && other->client
        && other->client->sess.connected == CON_CONNECTED
        && (mode != 1 || OnSameTeam(ent, other))
        && (g_deadChat->current.enabled || G_IsPlaying(ent) || !G_IsPlaying(other)) )
    {
        team = ent->client->sess.cs.team;
        if ( team <= TEAM_FREE || team > TEAM_ALLIES )
        {
            team_color = "";
        }
        else if ( ent->client->sess.cs.team == other->client->sess.cs.team )
        {
            team_color = "^8";
        }
        else
        {
            team_color = "^9";
        }
        if ( ent->client->sess.cs.team == TEAM_SPECTATOR )
        {
            Com_sprintf(szStateString, 0x40u, "\x15(\x14GAME_SPECTATOR\x15)");
        }
        else if ( ent->client->sess.sessionState )
        {
            Com_sprintf(szStateString, 0x40u, "\x15%s(\x14GAME_DEAD\x15)", team_color);
        }
        else
        {
            Com_sprintf(szStateString, 0x40u, "\x15%s", team_color);
        }
        if ( mode == 1 )
        {
            Com_sprintf(name, 0x80u, "%s(\x14%s\x15)%s%s: ", szStateString, teamString, cleanname, "^7");
        }
        else if ( mode == 2 )
        {
            Com_sprintf(name, 0x80u, "%s[%s]%s: ", szStateString, cleanname, "^7");
        }
        else
        {
            Com_sprintf(name, 0x80u, "%s%s%s: ", szStateString, cleanname, "^7");
        }
        fmtMsg = va("%c \"\x15%s%c%c%s\"", (char)((mode == 1) + 104), name, 94, color, message);
        SV_GameSendServerCommand(other - g_entities, SV_CMD_CAN_IGNORE, fmtMsg);
    }
}

// Decomp: CoDMPServer.c:383745
void __cdecl Cmd_Where_f(gentity_s *ent)
{
    char *tmp1;
    const char *fmtMsg;

    tmp1 = vtos(ent->r.currentOrigin);
    fmtMsg = va("%c \"%s\"", 101, tmp1);
    SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg);
}

// Decomp: CoDMPServer.c:383756
void __cdecl Cmd_CallVote_f(gentity_s *ent)
{
    int LicenseType;
    const char *fmtMsg;
    const char *fmtMsg2;
    const char *fmtMsg3;
    const char *fmtMsg4;
    const char *fmtMsg5;
    char *tmp7;
    char *tmp8;
    char *tmp9;
    const char *fmtMsg6;
    const char *fmtMsg7;
    const char *fmtMsg8;
    const char *fmtMsg9;
    const char *fmtMsg10;
    const char *fmtMsg11;
    char *GameTypeNameForScript;
    char *tmp17;
    char *tmp18;
    const char *tmp19;
    const char *fmtMsg12;
    const char *fmtMsg13;
    const char *fmtMsg14;
    char *fmtMsg15;
    char *fmtMsg16;
    char *fmtMsg17;
    const char *tmp26;
    int j;
    int kicknum;
    const dvar_s *mapname;
    const char *playerName;
    char arg1[256];
    char arg2[256];
    char cleanName[68];
    int i;
    char arg3[260];

    if ( !g_allowVote->current.enabled || (LicenseType = SV_GetLicenseType(), SV_IsServerRanked(LicenseType)) )
    {
        fmtMsg = va("%c \"GAME_VOTINGNOTENABLED\"", 101);
        SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg);
        return;
    }
    if ( level.numConnectedClients < 2 )
    {
        fmtMsg2 = va("%c \"GAME_VOTINGNOTENOUGHPLAYERS\"", 101);
        SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg2);
        return;
    }
    if ( g_oldVoting->current.enabled )
    {
        if ( level.voteTime )
        {
            fmtMsg3 = va("%c \"GAME_VOTEALREADYINPROGRESS\"", 101);
            SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg3);
            return;
        }
        if ( ent->client->sess.voteCount >= 3 )
        {
            fmtMsg4 = va("%c \"GAME_MAXVOTESCALLED\"", 101);
            SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg4);
            return;
        }
        if ( ent->client->sess.cs.team == TEAM_SPECTATOR )
        {
            fmtMsg5 = va("%c \"GAME_NOSPECTATORCALLVOTE\"", 101);
            SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg5);
            return;
        }
    }
    SV_Cmd_ArgvBuffer(1, arg1, 256);
    SV_Cmd_ArgvBuffer(2, arg2, 256);
    SV_Cmd_ArgvBuffer(3, arg3, 256);
    tmp7 = strchr(arg1, 0x3Bu);
    if ( tmp7 )
        goto LABEL_85;
    tmp8 = strchr(arg2, 0x3Bu);
    if ( tmp8 )
        goto LABEL_85;
    tmp9 = strchr(arg3, 0x3Bu);
    if ( tmp9 )
        goto LABEL_85;
    if ( !g_oldVoting->current.enabled )
    {
        Scr_VoteCalled(ent, arg1, arg2, arg3);
        return;
    }
    if ( I_stricmp(arg1, "map_restart")
        && I_stricmp(arg1, "map_rotate")
        && I_stricmp(arg1, "typemap")
        && I_stricmp(arg1, "map")
        && I_stricmp(arg1, "g_gametype")
        && I_stricmp(arg1, "kick")
        && I_stricmp(arg1, "clientkick")
        && I_stricmp(arg1, "tempBanUser")
        && I_stricmp(arg1, "tempBanClient") )
    {
        fmtMsg6 = va("%c \"GAME_INVALIDVOTESTRING\"", 101);
        SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg6);
        const char aCGameVotecomma[] =
            "%c \"GAME_VOTECOMMANDSARE\x15 map_restart, map_rotate, map <mapname>, g_gametype <typename>, typemap <typename> <mapname>, kick <player>, clientkick <clientnum>, tempBanUser <player>, tempBanClient <clientNum>\"\x00\x00tempBanClient";


        fmtMsg7 = va(aCGameVotecomma, 101);
        SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg7);
        return;
    }
    if ( level.voteExecuteTime )
    {
        level.voteExecuteTime = 0;
        fmtMsg8 = va("%s\n", level.voteString);
        Cbuf_AddText(0, fmtMsg8);
    }
    if ( !I_stricmp(arg1, "typemap") )
    {
        if ( !Scr_IsValidGameType(arg2) )
        {
LABEL_32:
            fmtMsg9 = va("%c \"GAME_INVALIDGAMETYPE\"", 101);
            SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg9);
            return;
        }
        if ( !I_stricmp(arg2, g_gametype->current.string) )
            arg2[0] = 0;
        SV_Cmd_ArgvBuffer(3, arg3, 256);
        if ( !useFastFile->current.enabled && !SV_MapExists(arg3) )
        {
        LABEL_37:
            fmtMsg10 = va("%c \"\x15the server doesn't have that map\"\x00\x00%c \"GAME_INVALIDGAMETYPE\"", 101);
            SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg10);
            return;
        }
        mapname = _Dvar_RegisterString("mapname", (char *)"", 0x44u, "Current map name");
        if ( !I_stricmp(arg3, mapname->current.string) )
            arg3[0] = 0;
        if ( !arg2[0] && !arg3[0] )
        {
            fmtMsg11 = va("%c \"GAME_TYPEMAP_NOCHANGE\"", 101);
            SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg11);
            return;
        }
        if ( arg3[0] )
        {
            if ( arg2[0] )
                Com_sprintf(level.voteString, 0x400u, "g_gametype %s; map %s", arg2, arg3);
            else
                Com_sprintf(level.voteString, 0x400u, "map %s", arg3);
            if ( arg2[0] )
            {
                const char aGameVoteGamety_0[] =
                    "GAME_VOTE_GAMETYPE\x14%s\x15 - \x14GAME_VOTE_MAP\x15%s\x00\x00map %s";
                GameTypeNameForScript = Scr_GetGameTypeNameForScript(arg2);
                Com_sprintf(level.voteDisplayString, 0x400u, aGameVoteGamety_0, GameTypeNameForScript, arg3);
            }
            else
            {
                Com_sprintf(level.voteDisplayString, 0x400u, "GAME_VOTE_MAP", arg3);
            }
        }
        else
        {
            Com_sprintf(level.voteString, 0x400u, "g_gametype %s; map_restart", arg2);
            tmp17 = Scr_GetGameTypeNameForScript(arg2);
            Com_sprintf(level.voteDisplayString, 0x400u, "GAME_VOTE_GAMETYPE %s", tmp17);
        }
        goto LABEL_96;
    }
    if ( !I_stricmp(arg1, "g_gametype") )
    {
        if ( !Scr_IsValidGameType(arg2) )
            goto LABEL_32;
        Com_sprintf(level.voteString, 0x400u, "%s %s; map_restart", arg1, arg2);
        tmp18 = Scr_GetGameTypeNameForScript(arg2);
        Com_sprintf(level.voteDisplayString, 0x400u, "GAME_VOTE_GAMETYPE %s", tmp18);
        goto LABEL_96;
    }
    if ( !I_stricmp(arg1, "map_restart") )
    {
        Com_sprintf(level.voteString, 0x400u, "fast_restart");
        Com_sprintf(level.voteDisplayString, 0x400u, "GAME_VOTE_MAPRESTART");
        goto LABEL_96;
    }
    if ( !I_stricmp(arg1, "map_rotate") )
    {
        Com_sprintf(level.voteString, 0x400u, "%s", arg1);
        Com_sprintf(level.voteDisplayString, 0x400u, "GAME_VOTE_NEXTMAP");
LABEL_96:
        playerName = CS_DisplayName(&ent->client->sess.cs, 3);
        fmtMsg14 = va("%c GAME_CALLEDAVOTE %s", 101, playerName);
        SV_GameSendServerCommand(-1, SV_CMD_CAN_IGNORE, fmtMsg14);
        level.voteTime = level.time + 30000;
        level.voteYes = 1;
        level.voteNo = 0;
        for ( i = 0; i < level.maxclients; ++i )
            level.clients[i].ps.eFlags &= ~0x100000u;
        ent->client->ps.eFlags |= 0x100000u;
        fmtMsg15 = va("%i %i", level.voteTime, sv_serverId_value);
        SV_SetConfigstring(15, fmtMsg15);
        SV_SetConfigstring(16, level.voteDisplayString);
        fmtMsg16 = va("%i", level.voteYes);
        SV_SetConfigstring(17, fmtMsg16);
        fmtMsg17 = va("%i", level.voteNo);
        SV_SetConfigstring(18, fmtMsg17);
        return;
    }
    if ( !I_stricmp(arg1, "map") )
    {
        if ( !useFastFile->current.enabled && !SV_MapExists(arg2) )
            goto LABEL_37;
        Com_sprintf(level.voteString, 0x400u, "%s %s", arg1, arg2);
        Com_sprintf(level.voteDisplayString, 0x400u, "GAME_VOTE_MAP", arg2);
        goto LABEL_96;
    }
    if ( !I_stricmp(arg1, "kick")
        || !I_stricmp(arg1, "clientkick")
        || !I_stricmp(arg1, "tempBanUser")
        || !I_stricmp(arg1, "tempBanClient") )
    {
        kicknum = level.maxclients;
        if ( I_stricmp(arg1, "kick") && I_stricmp(arg1, "tempBanUser") )
        {
            kicknum = atoi(arg2);
            if ( (kicknum || !I_stricmp(arg2, "0"))
                && kicknum >= 0
                && kicknum < level.maxclients
                && level.clients[kicknum].sess.connected == CON_CONNECTED )
            {
                I_strncpyz(cleanName, level.clients[kicknum].sess.cs.name, 64);
                I_CleanStr(cleanName);
            }
            else
            {
                kicknum = level.maxclients;
            }
        }
        else
        {
            for ( j = 0; j < level.maxclients; ++j )
            {
                if ( level.clients[j].sess.connected == CON_CONNECTED )
                {
                    tmp19 = CS_DisplayName(&level.clients[j].sess.cs, 3);
                    I_strncpyz(cleanName, tmp19, 64);
                    I_CleanStr(cleanName);
                    if ( !I_stricmp(cleanName, arg2) )
                        kicknum = j;
                }
            }
        }
        if ( Demo_IsEnabled() && !kicknum )
        {
LABEL_85:
            fmtMsg12 = va("%c \"GAME_INVALIDVOTESTRING\"", 101);
            SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg12);
            return;
        }
        if ( kicknum == level.maxclients )
        {
            fmtMsg13 = va("%c \"GAME_CLIENTNOTONSERVER\"", 101);
            SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg13);
            return;
        }
        if ( arg1[0] == 116 || arg1[0] == 84 )
            Com_sprintf(level.voteString, 0x400u, "%s \"%d\"", "tempBanClient", kicknum);
        else
            Com_sprintf(level.voteString, 0x400u, "%s \"%d\"", "clientkick", kicknum);
        tmp26 = CS_DisplayName(&level.clients[kicknum].sess.cs, 3);
        Com_sprintf(level.voteDisplayString, 0x400u, "GAME_VOTE_KICK (%i)%s", kicknum, tmp26);
        goto LABEL_96;
    }
    if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_cmds_mp.cpp", 1283, 0, "unhandled callvote") )
        __debugbreak();
}

// Decomp: CoDMPServer.c:384033
void __cdecl Cmd_Vote_f(gentity_s *ent)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    const char *fmtMsg3;
    const char *fmtMsg4;
    char *fmtMsg5;
    char *fmtMsg6;
    char msg[68];

    if ( g_oldVoting->current.enabled )
    {
        if ( !level.voteTime )
        {
            fmtMsg = va("%c \"GAME_NOVOTEINPROGRESS\"", 101);
            SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg);
            return;
        }
        if ( (ent->client->ps.eFlags & 0x100000) != 0 )
        {
            fmtMsg2 = va("%c \"GAME_VOTEALREADYCAST\"", 101);
            SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg2);
            return;
        }
        if ( ent->client->sess.cs.team == TEAM_SPECTATOR )
        {
            fmtMsg3 = va("%c \"GAME_NOSPECTATORVOTE\"", 101);
            SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg3);
            return;
        }
        fmtMsg4 = va("%c \"GAME_VOTECAST\"", 101);
        SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg4);
        ent->client->ps.eFlags |= 0x100000u;
    }
    SV_Cmd_ArgvBuffer(1, msg, 64);
    if ( msg[0] == 121 || msg[1] == 89 || msg[1] == 49 )
    {
        if ( g_oldVoting->current.enabled )
        {
            fmtMsg5 = va("%i", ++level.voteYes);
            SV_SetConfigstring(17, fmtMsg5);
        }
        else
        {
            Scr_PlayerVote(ent, (char*)"yes");
        }
    }
    else if ( g_oldVoting->current.enabled )
    {
        fmtMsg6 = va("%i", ++level.voteNo);
        SV_SetConfigstring(18, fmtMsg6);
    }
    else
    {
        Scr_PlayerVote(ent, (char *)"no");
    }
}

// Decomp: CoDMPServer.c:384093
void __cdecl Cmd_SetViewpos_f(gentity_s *ent)
{
    const char *fmtMsg;
    const char *fmtMsg2;
    long double tmp3;
    int tmp4;
    char buffer[1024];
    float origin[3];
    float angles[3];
    int i;

    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_cmds_mp.cpp", 1387, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_cmds_mp.cpp", 1388, 0, "%s", "ent->client") )
    {
        __debugbreak();
    }
    if ( !g_cheats->current.enabled )
    {
        fmtMsg = va("%c \"GAME_CHEATSNOTENABLED\"", 101);
        SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg);
        return;
    }
    if ( SV_Cmd_Argc() < 4 || SV_Cmd_Argc() > 6 )
    {
        fmtMsg2 = va("%c GAME_USAGE : setviewpos x y z yaw", 101);
        SV_GameSendServerCommand(ent - g_entities, SV_CMD_CAN_IGNORE, fmtMsg2);
        return;
    }
    for ( i = 0; i < 3; ++i )
    {
        SV_Cmd_ArgvBuffer(i + 1, buffer, 1024);
        tmp3 = atof(buffer);
        origin[i] = tmp3;
    }
    origin[2] = origin[2] - ent->client->ps.viewHeightCurrent;
    memset(angles, 0, sizeof(angles));
    tmp4 = SV_Cmd_Argc();
    if ( tmp4 == 5 )
        goto LABEL_18;
    if ( tmp4 == 6 )
    {
        SV_Cmd_ArgvBuffer(5, buffer, 1024);
        angles[0] = atof(buffer);
LABEL_18:
        SV_Cmd_ArgvBuffer(4, buffer, 1024);
        angles[1] = atof(buffer);
    }
    TeleportPlayer(ent, origin, angles);
}

// Decomp: CoDMPServer.c:384167
void __cdecl Cmd_EntityCount_f()
{
    if ( g_cheats->current.enabled )
        Com_Printf(0, "entity count = %i\n", level.num_entities);
}

// Decomp: CoDMPServer.c:384174
void __cdecl Cmd_MenuResponse_f(gentity_s *pEnt)
{
    char szServerId[1024];
    char szMenuName[1028];
    int iMenuIndex;
    char szResponse[1028];

    iMenuIndex = -1;
    if ( SV_Cmd_Argc() == 4 )
    {
        SV_Cmd_ArgvBuffer(1, szServerId, 1024);
        if ( atoi(szServerId) != sv_serverId_value )
            return;
        SV_Cmd_ArgvBuffer(2, szMenuName, 1024);
        iMenuIndex = atoi(szMenuName);
        if ( (unsigned int)iMenuIndex < 0x20 )
            SV_GetConfigstring(iMenuIndex + 2548, szMenuName, 1024);
        SV_Cmd_ArgvBuffer(3, szResponse, 1024);
    }
    else
    {
        szMenuName[0] = 0;
        strcpy(szResponse, "bad");
    }
    Scr_AddString(szResponse, SCRIPTINSTANCE_SERVER);
    Scr_AddString(szMenuName, SCRIPTINSTANCE_SERVER);
    Scr_Notify(pEnt, scr_const.menuresponse, 2u);
}

// Decomp: CoDMPServer.c:384204
void __cdecl ClientCommand(int clientNum)
{
    char errMsg[68];
    gentity_s *ent;
    char cmd[1028];

    ent = &g_entities[clientNum];
    if ( ent->client )
    {
        SV_Cmd_ArgvBuffer(0, cmd, 1024);
        if ( I_stricmp(cmd, "say") )
        {
            if ( I_stricmp(cmd, "say_team") )
            {
                if ( ent->client->ps.pm_type != 5 )
                {
                    if ( I_stricmp(cmd, "mr") )
                    {
                        if ( I_stricmp(cmd, "give") )
                        {
                            if ( I_stricmp(cmd, "take") )
                            {
                                if ( I_stricmp(cmd, "god") )
                                {
                                    if ( I_stricmp(cmd, "demigod") )
                                    {
                                        if ( I_stricmp(cmd, "notarget") )
                                        {
                                            if ( I_stricmp(cmd, "noclip") )
                                            {
                                                if ( I_stricmp(cmd, "ufo") )
                                                {
                                                    if ( I_stricmp(cmd, "kill") )
                                                    {
                                                        if ( I_stricmp(cmd, "follownext") )
                                                        {
                                                            if ( I_stricmp(cmd, "followprev") )
                                                            {
                                                                if ( I_stricmp(cmd, "where") )
                                                                {
                                                                    if ( I_stricmp(cmd, "callvote") )
                                                                    {
                                                                        if ( I_stricmp(cmd, "vote") )
                                                                        {
                                                                            if ( I_stricmp(cmd, "setviewpos") )
                                                                            {
                                                                                if ( I_stricmp(cmd, "entitycount") )
                                                                                {
                                                                                    if ( I_stricmp(cmd, "printentities") )
                                                                                    {
                                                                                        if ( I_stricmp(cmd, "ai") || !CheatsOk(ent) )
                                                                                        {
                                                                                            if ( I_stricmp(cmd, "visionsetnaked") )
                                                                                            {
                                                                                                if ( I_stricmp(cmd, "visionsetnight") )
                                                                                                {
                                                                                                    memset(errMsg, 0, 64);
                                                                                                    if ( Com_sprintf(errMsg, 0x40u, "%c \"GAME_UNKNOWNCLIENTCOMMAND\x15%s\"", 101, cmd) < 0)
                                                                                                        memset(&errMsg[60], 46, 3);
                                                                                                    errMsg[63] = 0;
                                                                                                    SV_GameSendServerCommand(clientNum, SV_CMD_CAN_IGNORE, errMsg);
                                                                                                }
                                                                                                else
                                                                                                {
                                                                                                    Cmd_VisionSetNight_f();
                                                                                                }
                                                                                            }
                                                                                            else
                                                                                            {
                                                                                                Cmd_VisionSetNaked_f();
                                                                                            }
                                                                                        }
                                                                                        else
                                                                                        {
                                                                                            Cmd_AI_f();
                                                                                        }
                                                                                    }
                                                                                    else
                                                                                    {
                                                                                        Cmd_PrintEntities_f();
                                                                                    }
                                                                                }
                                                                                else
                                                                                {
                                                                                    Cmd_EntityCount_f();
                                                                                }
                                                                            }
                                                                            else
                                                                            {
                                                                                Cmd_SetViewpos_f(ent);
                                                                            }
                                                                        }
                                                                        else
                                                                        {
                                                                            Cmd_Vote_f(ent);
                                                                        }
                                                                    }
                                                                    else
                                                                    {
                                                                        Cmd_CallVote_f(ent);
                                                                    }
                                                                }
                                                                else
                                                                {
                                                                    Cmd_Where_f(ent);
                                                                }
                                                            }
                                                            else
                                                            {
                                                                Cmd_FollowCycle_f(ent, -1);
                                                            }
                                                        }
                                                        else
                                                        {
                                                            Cmd_FollowCycle_f(ent, 1);
                                                        }
                                                    }
                                                    else
                                                    {
                                                        Cmd_Kill_f(ent);
                                                    }
                                                }
                                                else
                                                {
                                                    Cmd_UFO_f(ent);
                                                }
                                            }
                                            else
                                            {
                                                Cmd_Noclip_f(ent);
                                            }
                                        }
                                        else
                                        {
                                            Cmd_Notarget_f(ent);
                                        }
                                    }
                                    else
                                    {
                                        Cmd_DemiGod_f(ent);
                                    }
                                }
                                else
                                {
                                    Cmd_God_f(ent);
                                }
                            }
                            else
                            {
                                Cmd_Take_f(ent);
                            }
                        }
                        else
                        {
                            Cmd_Give_f(ent);
                        }
                    }
                    else
                    {
                        Cmd_MenuResponse_f(ent);
                    }
                }
            }
            else
            {
                Cmd_Say_f(ent, 1, 0);
            }
        }
        else
        {
            Cmd_Say_f(ent, 0, 0);
        }
    }
}

// Decomp: CoDMPServer.c:384390
void __cdecl Cmd_Say_f(gentity_s *ent, int mode, int arg0)
{
    char *p;

    if ( SV_Cmd_Argc() >= 2 || arg0 )
    {
        if ( arg0 )
            p = ConcatArgs(0);
        else
            p = ConcatArgs(1);
        G_Say(ent, 0, mode, p);
    }
}

// Decomp: CoDMPServer.c:384405
void Cmd_PrintEntities_f()
{
    G_PrintEntities();
}

// Decomp: CoDMPServer.c:384411
void Cmd_VisionSetNaked_f()
{
    const char *tmp0;
    const char *tmp1;
    char *fmtMsg;
    float tmp3;
    int tmp4;
    float tmp5;
    int duration;

    duration = 1000;
    tmp4 = SV_Cmd_Argc();
    if ( tmp4 == 2 )
        goto LABEL_4;
    if ( tmp4 == 3 )
    {
        tmp0 = SV_Cmd_Argv(2);
        tmp3 = atof(tmp0);
        tmp5 = tmp3 * 1000.0;
        duration = (int)(tmp5 + 9.313225746154785e-10);
LABEL_4:
        tmp1 = SV_Cmd_Argv(1);
        fmtMsg = va("\"%s\" %i", tmp1, duration);
        SV_SetConfigstring(1550, fmtMsg);
        return;
    }
    Com_Printf(0, "USAGE: visionSetNaked <name> <duration>\n");
}

const char *__cdecl SV_Cmd_Argv(int argIndex)
{
    if ( sv_cmd_args.nesting >= 8u
        && !Assert_MyHandler(
                    "c:\\projects_pc\\cod\\codsrc\\src\\game_mp\\../qcommon/cmd.h",
                    238,
                    0,
                    "sv_cmd_args.nesting doesn't index CMD_MAX_NESTING\n\t%i not in [0, %i)",
                    sv_cmd_args.nesting,
                    8) )
    {
        __debugbreak();
    }
    if ( argIndex < 0
        && !Assert_MyHandler(
                    "c:\\projects_pc\\cod\\codsrc\\src\\game_mp\\../qcommon/cmd.h",
                    239,
                    0,
                    "%s\n\t(argIndex) = %i",
                    "(argIndex >= 0)",
                    argIndex) )
    {
        __debugbreak();
    }
    if ( argIndex >= sv_cmd_args.argc[sv_cmd_args.nesting] )
        return "";
    else
        return sv_cmd_args.argv[sv_cmd_args.nesting][argIndex];
}

// Decomp: CoDMPServer.c:384479
void Cmd_VisionSetNight_f()
{
    const char *tmp0;
    const char *tmp1;
    char *fmtMsg;
    float tmp3;
    int tmp4;
    float tmp5;
    int duration;

    duration = 1000;
    tmp4 = SV_Cmd_Argc();
    if ( tmp4 == 2 )
        goto LABEL_4;
    if ( tmp4 == 3 )
    {
        tmp0 = SV_Cmd_Argv(2);
        tmp3 = atof(tmp0);
        tmp5 = tmp3 * 1000.0;
        duration = (int)(tmp5 + 9.313225746154785e-10);
LABEL_4:
        tmp1 = SV_Cmd_Argv(1);
        fmtMsg = va("\"%s\" %i", tmp1, duration);
        SV_SetConfigstring(1551, fmtMsg);
        return;
    }
    Com_Printf(0, "USAGE: visionSetNight <name> <duration>\n");
}

