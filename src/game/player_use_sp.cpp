#include "player_use_sp.h"
#include <clientscript/cscr_vm.h>
#include "g_spawn_sp.h"
#include <clientscript/scr_const.h>
#include <game/turret.h>
#include <client_sp/g_client_sp.h>
#include <bgame/bg_misc.h>
#include <qcommon/cm_world.h>
#include <server/sv_game.h>
#include <bgame/bg_perks.h>

#include <algorithm>
#include <game/g_team_sp.h>
#include <game/g_utils_wrapper.h>
#include <game/actor_script_cmd.h>
#include <bgame/bg_weapons_def.h>


// Decomp: CoDMPServer.c:453589
bool __cdecl compare_use(const useList_t &a, const useList_t &b)
{
    return b.score > a.score;
}

static const float turretScanRadiusSq = 16384.0;
// Decomp: CoDMPServer.c:452611
char __cdecl Player_IsPlayerUsingTurretNearby(gentity_s *ent)
{
    gentity_s *clEnt;
    int i;

    i = 0;
    clEnt = g_entities;
    while ( i < com_maxclients->current.integer )
    {
        if ( clEnt->client
            && clEnt->client->sess.connected == CON_CONNECTED
            && ((clEnt->client->ps.eFlags & 0x300) != 0
             || (clEnt->client->ps.weapFlags & 0x800) != 0
             || clEnt->client->ps.weaponstate == 35)
            && Vec3DistanceSq(ent->client->ps.origin, clEnt->client->ps.origin) <= turretScanRadiusSq )
        {
            return 1;
        }
        ++i;
        ++clEnt;
    }
    return 0;
}

// Decomp: CoDMPServer.c:452634
void __cdecl Player_UpdateActivate(gentity_s *ent)
{
    char useSucceeded;

    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp", 259, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp", 260, 0, "%s", "ent->client") )
    {
        __debugbreak();
    }
    ent->client->ps.weapFlags &= ~1u;
    ent->client->ps.weapFlags &= ~0x10000u;
    useSucceeded = 0;
    if ( ent->client->button_bits.testBit(0x32u) )
    {
        Player_ActivateDoubleTapCmd(ent);
    }
    else if ( (ent->client->useHoldEntity.isDefined() || ent->client->ps.mountAvailable)
                 &&  ent->client->oldbutton_bits.testBit(5u)
                 && !ent->client->button_bits.testBit(5u) )
    {
        ent->client->ps.weapFlags |= 1u;
        ent->client->ps.weapFlags |= 0x10000u;
    }
    else
    {
        if (   ent->client->latched_button_bits.testBit(3u)
            || ent->client->latched_button_bits.testBit(5u)
            || ent->client->latched_button_bits.testBit(4u) )
        {
            useSucceeded = Player_ActivateCmd(ent);
        }
        if ( ent->client->useHoldEntity.isDefined() || useSucceeded || ent->client->ps.mountAvailable )
        {
            if ( ent->client->button_bits.testBit(3u) || ent->client->button_bits.testBit(5u) )
                Player_ActivateHoldCmd(ent);
            ent->client->useButtonDone = 1;
        }
        else if ( ent->client->latched_button_bits.testBit(5u) )
        {
            ent->client->ps.weapFlags |= 1u;
            ent->client->ps.weapFlags |= 0x10000u;
        }
    }
}

// Decomp: CoDMPServer.c:452700
char __cdecl Player_ActivateCmd(gentity_s *ent)
{
    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp", 81, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->r.inuse
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp", 82, 0, "%s", "ent->r.inuse") )
    {
        __debugbreak();
    }
    if ( !ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp", 83, 0, "%s", "ent->client") )
    {
        __debugbreak();
    }
    if ( !Scr_IsSystemActive(1u, SCRIPTINSTANCE_SERVER) )
        return 0;
    ent->client->useHoldEntity.setEnt(0);
    if ( ent->active )
    {
        if ( (ent->client->ps.eFlags & 0x300) != 0 )
            ent->active = 2;
        else
            ent->active = 0;
        return 1;
    }
    else
    {
        if ( (ent->client->ps.eFlags & 0x14000) != 0 )
            ent->client->useButtonDone = 1;
        if ( (ent->client->ps.pm_flags & 4) != 0 )
        {
            return 1;
        }
        else if ( (ent->client->ps.pm_flags & 0x8000) != 0 )
        {
            return 1;
        }
        else if ( (ent->client->ps.pm_flags & 0x400000) != 0 )
        {
            return 1;
        }
        else if ( ent->client->ps.weaponstate < 20 || ent->client->ps.weaponstate > 25 )
        {
            if ( ent->client->ps.cursorHint || ent->client->ps.mountAvailable )
            {
                if ( ent->client->ps.cursorHintEntIndex != 1023 || ent->client->ps.mountAvailable )
                {
                    if ( ent->client->ps.cursorHintEntIndex != 1023 )
                    {
                        if ( !g_entities[ent->client->ps.cursorHintEntIndex].r.inuse
                            && !Assert_MyHandler(
                                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp",
                                        126,
                                        0,
                                        "%s",
                                        "g_entities[ ent->client->ps.cursorHintEntIndex ].r.inuse") )
                        {
                            __debugbreak();
                        }
                        ent->client->useHoldEntity.setEnt(&g_entities[ent->client->ps.cursorHintEntIndex]);
                    }
                    ent->client->useHoldTime = level.time;
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 1;
        }
    }
}

// Decomp: CoDMPServer.c:452807
void __cdecl Player_ActivateHoldCmd(gentity_s *ent)
{
    int useHoldEntNum;
    gentity_s *useEnt;

    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp", 170, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp", 171, 0, "%s", "ent->client") )
    {
        __debugbreak();
    }
    if ( Scr_IsSystemActive(1u, SCRIPTINSTANCE_SERVER)
        && (ent->client->useHoldEntity.isDefined() || ent->client->ps.mountAvailable)
        && level.time - ent->client->lastSpawnTime >= g_useholdspawndelay->current.integer
        && (!ent->client->button_bits.testBit(5u)
         || level.time - ent->client->useHoldTime >= g_useholdtime->current.integer) )
    {
        if ( ent->client->ps.mountAvailable )
        {
            if ( Mount_CanPlayerDeployTurret(&ent->client->ps) && !Player_IsPlayerUsingTurretNearby(ent) )
            {
                ent->client->ps.weapFlags |= 0x800u;
                BG_AddPredictableEventToPlayerstate(0x25u, 0, &ent->client->ps);
            }
        }
        else if ( ent->client->ps.weaponstate != 35
                     && ent->client->ps.weaponstate != 36
                     && ent->client->ps.weaponstate != 37
                     && (ent->client->ps.eFlags & 0x300) == 0
                     && (ent->client->ps.pm_flags & 4) == 0
                     && ent->client->ps.weaponstate != 1
                     && ent->client->ps.weaponstate != 2
                     && ent->client->ps.weaponstate != 3
                     && ent->client->ps.weaponstate != 4
                     && ent->client->ps.weaponstate != 5 )
        {
            useEnt = ent->client->useHoldEntity.ent();
            if ( useEnt->s.number != ent->client->useHoldEntity.entnum() )
            {
                useHoldEntNum = ent->client->useHoldEntity.entnum();
                if ( !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp",
                                205,
                                0,
                                "useEnt->s.number == (int)ent->client->useHoldEntity.entnum()\n\t%i, %i",
                                useEnt->s.number,
                                useHoldEntNum) )
                    __debugbreak();
            }
            if ( !useEnt->r.inuse
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp",
                            206,
                            0,
                            "%s\n\t(useEnt->s.number) = %i",
                            "(useEnt->r.inuse)",
                            useEnt->s.number) )
            {
                __debugbreak();
            }
            Player_UseEntity(ent, useEnt);
        }
    }
}

// Decomp: CoDMPServer.c:452891
void __cdecl Player_UseEntity(gentity_s *playerEnt, gentity_s *useEnt)
{
    void(__cdecl * touch)(gentity_s *, gentity_s *, int);
    void(__cdecl * use)(gentity_s *, gentity_s *, gentity_s *);

    if (!playerEnt
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp", 40, 0, "%s", "playerEnt"))
    {
        __debugbreak();
    }
    if (!playerEnt->client
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp",
            41,
            0,
            "%s",
            "playerEnt->client"))
    {
        __debugbreak();
    }
    if (!useEnt
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp", 42, 0, "%s", "useEnt"))
    {
        __debugbreak();
    }
    if (!useEnt->r.inuse
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp",
            44,
            0,
            "%s",
            "useEnt->r.inuse"))
    {
        __debugbreak();
    }
    if (useEnt->s.eType == 3)
    {
        Scr_AddEntity(playerEnt, SCRIPTINSTANCE_SERVER);
        Scr_Notify(useEnt, scr_const.touch, 1u);
        useEnt->active = 1;
        touch = entityHandlers[useEnt->handler].touch;
        if (touch)
            touch(useEnt, playerEnt, 0);
    }
    else if (useEnt->s.eType != 11 || G_IsTurretUsable(useEnt, playerEnt))
    {
        Scr_AddEntity(playerEnt, SCRIPTINSTANCE_SERVER);
        Scr_Notify(useEnt, scr_const.trigger, 1u);
        use = entityHandlers[useEnt->handler].use;
        if (use)
            use(useEnt, playerEnt, playerEnt);
    }
    playerEnt->client->useHoldEntity.setEnt(0);
}

// Decomp: CoDMPServer.c:452963
void __cdecl Player_ActivateDoubleTapCmd(gentity_s *ent)
{
    int useHoldEntNum;
    gentity_s *useEnt;

    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp", 220, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp", 221, 0, "%s", "ent->client") )
    {
        __debugbreak();
    }
    if ( Scr_IsSystemActive(1u, SCRIPTINSTANCE_SERVER)
        && ent->client->useHoldEntity.isDefined()
        && level.time - ent->client->lastSpawnTime >= g_useholdspawndelay->current.integer
        && ent->client->ps.weaponstate != 35
        && ent->client->ps.weaponstate != 36
        && ent->client->ps.weaponstate != 37
        && (ent->client->ps.eFlags & 0x300) == 0
        && (ent->client->ps.pm_flags & 4) == 0 )
    {
        useEnt = ent->client->useHoldEntity.ent();
        if ( useEnt->s.number != ent->client->useHoldEntity.entnum() )
        {
            useHoldEntNum = ent->client->useHoldEntity.entnum();
            if ( !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp",
                            244,
                            0,
                            "useEnt->s.number == (int)ent->client->useHoldEntity.entnum()\n\t%i, %i",
                            useEnt->s.number,
                            useHoldEntNum) )
                __debugbreak();
        }
        if ( !useEnt->r.inuse
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp",
                        245,
                        0,
                        "%s\n\t(useEnt->s.number) = %i",
                        "(useEnt->r.inuse)",
                        useEnt->s.number) )
        {
            __debugbreak();
        }
        Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
        Scr_Notify(useEnt, scr_const.trigger_use_doubletap, 1u);
    }
}

int Player_GetUseList(gentity_s *ent, useList_t *useList, int prevHintEntIndex);

// Decomp: CoDMPServer.c:453031
void __cdecl Player_UpdateCursorHints(gentity_s *ent)
{
    int perkTeamIndex;
    int IsVehicleUsable;
    int scale;
    int ItemCursorHint;
    int item;
    useList_t useList[1024];
    int useListCount;
    int useListIndex;
    playerState_s *ps;
    gentity_s *self;

    self = 0;
    if ( !ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp", 726, 0, "%s", "ent->client") )
    {
        __debugbreak();
    }
    ps = &ent->client->ps;
    ps->cursorHint = 0;
    if ( !BG_ThrowingBackGrenade(ps) )
        ps->throwBackGrenadeTimeLeft = 0;
    if ( ent->health > 0 )
    {
        if ( ent->active )
        {
            if ( (ps->eFlags & 0x300) != 0 )
                Player_SetTurretDropHint(ent);
            return;
        }
        if ( (ent->client->ps.pm_flags & 4) == 0
            && (ent->client->ps.pm_flags & 0x8000) == 0
            && (ent->client->ps.pm_flags & 0x400000) == 0
            && (ent->client->ps.pm_flags & 8) == 0
            && (ps->eFlags2 & 0x10000000) == 0
            && (ps->weapFlags & 0x100000) == 0
            && (ps->weaponstate < 20 || ps->weaponstate > 24) )
        {
            if ( !Player_IsPlayerUsingTurretNearby(ent)
                && (ent->client->ps.mountAvailable || ps->weaponstate == 35 || ps->weaponstate == 36 || ps->weaponstate == 37) )
            {
                ps->cursorHintEntIndex = 1023;
                return;
            }
            if ( ps->pm_type == 6 )
            {
                ps->cursorHintEntIndex = 1023;
                return;
            }
            if ( (ps->weapFlags & 0x400000) == 0
                && ((ps->locationSelectionInfo & 0xF) == 0xF || !ps->locationSelectionInfo)
                && (ps->eFlags & 0x4000) == 0
                && (ent->client->flags & 4) == 0
                && (ent->client->flags & 8) == 0 )
            {
                useListCount = Player_GetUseList(ent, useList, ps->cursorHintEntIndex);
                if ( useListCount )
                {
                    item = 0;
                    scale = -1;
                    useListIndex = 0;
                    while ( 1 )
                    {
                        if ( useListIndex >= useListCount )
                            return;
                        self = useList[useListIndex].ent;
                        if ( !self
                            && !Assert_MyHandler(
                                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp",
                                        841,
                                        0,
                                        "%s",
                                        "traceEnt") )
                        {
                            __debugbreak();
                        }
                        if ( !self->r.inuse
                            && !Assert_MyHandler(
                                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp",
                                        842,
                                        0,
                                        "%s",
                                        "traceEnt->r.inuse") )
                        {
                            __debugbreak();
                        }
                        switch ( self->s.eType )
                        {
                            case 0:
                                if ( self->classname != scr_const.trigger_use
                                    && self->classname != scr_const.trigger_use_touch
                                    && self->classname != scr_const.trigger_radius
                                    && self->classname != scr_const.trigger_radius_use )
                                {
                                    goto LABEL_85;
                                }
                                if ( self->team && self->team != ent->client->sess.cs.team
                                    || self->trigger.perk != 52 && !BG_HasPerk(ent->client->ps.perks, self->trigger.perk)
                                    || self->item[1].ammoCount != 1023 && self->item[1].ammoCount != ent->client->ps.clientNum
                                    || (self->r.clientMask[(int)ps->clientNum >> 5] & (1 << (ps->clientNum & 0x1F))) != 0 )
                                {
                                    goto LABEL_39;
                                }
                                item = self->s.un3.item;
                                if ( item && self->s.un1.scale != 255 )
                                    scale = self->s.un1.scale;
                                goto LABEL_85;
                            case 3:
                                if ( (0x800000 & self->s.lerp.eFlags2) != 0 )
                                    goto LABEL_39;
                                ItemCursorHint = Player_GetItemCursorHint(ent->client, self);
                                if ( !ItemCursorHint )
                                    goto LABEL_39;
                                item = ItemCursorHint;
                                goto LABEL_85;
                            case 4:
                                item = self->s.un3.item % 2048 + 7;
                                ps->throwBackGrenadeTimeLeft = self->nextthink - level.time;
                                goto LABEL_85;
                            case 6:
                                item = self->s.un3.item;
                                if ( item && self->s.un1.scale != 255 )
                                    scale = self->s.un1.scale;
                                if ( !self->s.un3.item )
                                    goto LABEL_85;
                                perkTeamIndex = (self->s.eventParm & 0xC000) >> 14;
                                if ( !perkTeamIndex || perkTeamIndex == ent->client->sess.cs.team )
                                {
                                    if ( (self->s.eventParm & 0x3FFF) != 0 && BG_HasPerk(ps->perks, (self->s.eventParm & 0x3FFF) >> 8) )
                                        scale = (unsigned __int8)self->s.eventParm;
LABEL_85:
                                    if ( item )
                                    {
                                        ps->cursorHintEntIndex = self->s.number;
                                        self->flags |= 0x200000u;
                                        ps->cursorHint = item;
                                        ps->cursorHintString = scale;
                                    }
                                    else
                                    {
                                        ps->cursorHintEntIndex = 1023;
                                    }
                                    if ( ps->cursorHintEntIndex != 1023
                                        && !g_entities[ps->cursorHintEntIndex].r.inuse
                                        && !Assert_MyHandler(
                                                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp",
                                                    961,
                                                    1,
                                                    "%s\n\t(ps->cursorHintEntIndex) = %i",
                                                    "(ps->cursorHintEntIndex == ((1<<10)-1) || g_entities[ps->cursorHintEntIndex].r.inuse)",
                                                    ps->cursorHintEntIndex) )
                                    {
                                        __debugbreak();
                                    }
                                    return;
                                }
                                item = 0;
                                scale = -1;
LABEL_39:
                                ++useListIndex;
                                break;
                            case 0xB:
                                if ( !G_IsTurretUsable(self, ent) )
                                    goto LABEL_39;
                                item = self->s.weapon + 7;
                                if ( *BG_GetWeaponDef(self->s.weapon)->szUseHintString )
                                    scale = BG_GetWeaponDef(self->s.weapon)->iUseHintStringIndex;
                                goto LABEL_85;
                            case 0xE:
                                IsVehicleUsable = G_IsVehicleUsable(self, ent);
                                if ( IsVehicleUsable == -1 )
                                    goto LABEL_39;
                                item = 1;
                                scale = IsVehicleUsable;
                                if ( !ent->client->disallowVehicleUsage )
                                    goto LABEL_85;
                                ps->cursorHintEntIndex = 1023;
                                ps->cursorHint = 1;
                                ps->cursorHintString = IsVehicleUsable;
                                goto LABEL_39;
                            default:
                                goto LABEL_39;
                        }
                    }
                }
            }
        }
    }
}

// Decomp: CoDMPServer.c:453236
int Player_GetUseList(gentity_s *ent, useList_t *useList, int prevHintEntIndex)
{
    int eType;
    int integer;
    float mgRadiusClamped;
    float value;
    float deltaX;
    float deltaY;
    float contactMins[3];
    BOOL hasEntContact;
    float contactMaxs[3];
    float throwbackInnerRadius;
    BOOL sameTeam;
    float heightDelta;
    gentity_s *ent1;
    float forwardDot;
    float v[3];
    int entityList[1025];
    int hitNum;
    float origin[3];
    float touchMaxX;
    float touchMaxY;
    float touchMaxZ;
    float baseScore;
    float mins[3];
    float distNorm;
    float useRadiusHoriz;
    float useRadiusHorizY;
    float useRadiusVert;
    
    float useRadius;
    gentity_s *gEnt;
    float forward[3];
    float dotScore;
    int useListCount;
    int blockedSightCount;
    float maxs[3];
    float pos[3];
    int blockedPickupCount;
    int areaEntCount;
    int i;
    playerState_s *ps;
    team_t team;
    float lookAtDotMin;
    float touchMinX;
    float touchMinY;
    float touchMinZ;
    gentity_s *ent2;

    baseScore = 256.0f;
    lookAtDotMin = 0.75999999f;
    blockedPickupCount = 0;
    if ((float)(192.0 - player_throwbackOuterRadius->current.value) < 0.0)
        value = player_throwbackOuterRadius->current.value;
    else
        value = 192.0f;
    useRadiusHoriz = value;
    if ((float)(value - player_MGUseRadius->current.value) < 0.0)
        mgRadiusClamped = player_MGUseRadius->current.value;
    else
        mgRadiusClamped = useRadiusHoriz;
    useRadiusHoriz = mgRadiusClamped;
    if ((float)(mgRadiusClamped - player_useRadius->current.value) < 0.0)
        integer = player_useRadius->current.integer;
    else
        integer = LODWORD(useRadiusHoriz);
    useRadiusHoriz = *(float *)&integer;
    useRadiusHorizY = *(float *)&integer;
    useRadiusVert = 96.0f;
    ps = &ent->client->ps;
    G_GetPlayerViewOrigin(ps, origin);
    BG_GetPlayerViewDirection(ps, forward, 0, 0);
    touchMinX = ps->origin[0] + -15.0;
    touchMinY = ps->origin[1] + -15.0;
    touchMinZ = ps->origin[2] + 0.0;
    touchMaxX = ps->origin[0] + 15.0;
    touchMaxY = ps->origin[1] + 15.0;
    touchMaxZ = ps->origin[2] + 70.0;
    mins[0] = origin[0] - useRadiusHoriz;
    mins[1] = origin[1] - useRadiusHorizY;
    mins[2] = origin[2] - useRadiusVert;
    maxs[0] = origin[0] + useRadiusHoriz;
    maxs[1] = origin[1] + useRadiusHorizY;
    maxs[2] = origin[2] + useRadiusVert;
    areaEntCount = CM_AreaEntities(mins, maxs, entityList, 1024, 0x200000);
    useListCount = 0;
    for (i = 0; i < areaEntCount; ++i)
    {
        gEnt = &g_entities[entityList[i]];
        if (ent != gEnt && (gEnt->s.eType == 3 || (gEnt->r.contents & 0x200000) != 0))
        {
            if (gEnt->classname == scr_const.trigger_use_touch)
            {
                if (gEnt->r.absmin[0] <= touchMaxX
                    && touchMinX <= gEnt->r.absmax[0]
                    && gEnt->r.absmin[1] <= touchMaxY
                    && touchMinY <= gEnt->r.absmax[1]
                    && gEnt->r.absmin[2] <= touchMaxZ
                    && touchMinZ <= gEnt->r.absmax[2])
                {
                    if (SV_EntityContact(&touchMinX, &touchMaxX, gEnt))
                    {
                        useList[useListCount].score = -256.0f;
                        useList[useListCount++].ent = gEnt;
                    }
                }
            }
            else
            {
                if (gEnt->s.eType != 4)
                    goto LABEL_42;
                if (ps->waterlevel < 3 && BG_GetWeaponDef(gEnt->s.weapon)->bThrowBack && !gEnt->s.lerp.u.turret.ownerNum)
                {
                    if (gEnt->parent.isDefined())
                    {
                        //ent1 = EntHandle::ent(&gEnt->parent);
                        ent1 = gEnt->parent.ent();
                        sameTeam = OnSameTeam(ent1, ent);
                        if ((!sameTeam
                            || ent->client->sess.cs.team == TEAM_FREE
                            || ent1->s.clientNum == ent->s.clientNum
                            || BG_GetFriendlyFireStatus())
                            && !gEnt->tagInfo)
                        {
                            heightDelta = gEnt->r.currentOrigin[2] - ent->r.currentOrigin[2];
                            if (prevHintEntIndex == gEnt->s.number)
                                goto LABEL_41;
                            throwbackInnerRadius = player_throwbackInnerRadius->current.value;
                            if ((ps->perks[1] & 0x10) != 0)
                                throwbackInnerRadius = perk_throwbackInnerRadius->current.value;
                            deltaX = ent->r.currentOrigin[0] - gEnt->r.currentOrigin[0];
                            deltaY = ent->r.currentOrigin[1] - gEnt->r.currentOrigin[1];
                            if ((float)((float)(deltaX * deltaX) + (float)(deltaY * deltaY)) <= (float)(throwbackInnerRadius * throwbackInnerRadius))
                            {
                            LABEL_41:
                                if ((float)((float)((float)(gEnt->s.lerp.pos.trDelta[0] * gEnt->s.lerp.pos.trDelta[0])
                                    + (float)(gEnt->s.lerp.pos.trDelta[1] * gEnt->s.lerp.pos.trDelta[1]))
                                    + (float)(gEnt->s.lerp.pos.trDelta[2] * gEnt->s.lerp.pos.trDelta[2])) <= (float)(bg_maxGrenadeIndicatorSpeed->current.value * bg_maxGrenadeIndicatorSpeed->current.value))
                                {
                                LABEL_42:
                                    eType = gEnt->s.eType;
                                    switch (eType)
                                    {
                                    case 4:
                                        useRadius = player_throwbackOuterRadius->current.value;
                                        break;
                                    case 11:
                                        useRadius = player_MGUseRadius->current.value;
                                        break;
                                    case 14:
                                        useRadius = vehicle_useRadius->current.value;
                                        break;
                                    default:
                                        useRadius = player_useRadius->current.value;
                                        break;
                                    }
                                    pos[0] = gEnt->r.absmin[0] + gEnt->r.absmax[0];
                                    pos[1] = gEnt->r.absmin[1] + gEnt->r.absmax[1];
                                    pos[2] = gEnt->r.absmin[2] + gEnt->r.absmax[2];
                                    pos[0] *= 0.5;
                                    pos[1] *= 0.5;
                                    pos[2] *= 0.5;
                                    v[0] = pos[0] - origin[0];
                                    v[1] = pos[1] - origin[1];
                                    v[2] = pos[2] - origin[2];
                                    distNorm = Vec3Normalize(v);
                                    if (distNorm <= useRadius)
                                    {
                                        if ((forwardDot = (float)((float)(v[0] * forward[0]) + (float)(v[1] * forward[1])) + (float)(v[2] * forward[2]),
                                            gEnt->classname != scr_const.trigger_use)
                                            && gEnt->classname != scr_const.trigger_radius
                                            && gEnt->classname != scr_const.trigger_radius_use
                                            || !gEnt->trigger.requireLookAt
                                            || lookAtDotMin <= forwardDot)
                                        {
                                            if (gEnt->classname != scr_const.trigger_radius
                                                && gEnt->classname != scr_const.trigger_radius_use)
                                            {
                                                goto LABEL_61;
                                            }
                                            team = level_bgs.clientinfo[ent->client->ps.clientNum].team;
                                            if (gEnt->team != 255 && gEnt->team == team)
                                            {
                                                hasEntContact = 0;
                                                contactMins[0] = gEnt->r.currentOrigin[0] + gEnt->r.mins[0];
                                                contactMins[1] = gEnt->r.currentOrigin[1] + gEnt->r.mins[1];
                                                contactMins[2] = gEnt->r.currentOrigin[2] + gEnt->r.mins[2];
                                                contactMaxs[0] = gEnt->r.currentOrigin[0] + gEnt->r.maxs[0];
                                                contactMaxs[1] = gEnt->r.currentOrigin[1] + gEnt->r.maxs[1];
                                                contactMaxs[2] = gEnt->r.currentOrigin[2] + gEnt->r.maxs[2];
                                                ExpandBoundsToWidth(contactMins, contactMaxs);
                                                hasEntContact = SV_EntityContact(contactMins, contactMaxs, ent);
                                                if (hasEntContact)
                                                {
                                                LABEL_61:
                                                    dotScore = 1.0 - (float)((float)(forwardDot + 1.0) * 0.5);
                                                    useList[useListCount].score = dotScore * baseScore;
                                                    if ((gEnt->flags & 0x200) != 0)
                                                    {
                                                        useList[useListCount].score = useList[useListCount].score - (float)(baseScore * 0.55000001);
                                                    }
                                                    else
                                                    {
                                                        if (gEnt->s.eType == 4)
                                                            useList[useListCount].score = useList[useListCount].score - (float)(baseScore * 2.0);
                                                        if (gEnt->s.eType == 14)
                                                            useList[useListCount].score = useList[useListCount].score - (float)(baseScore * 2.0);
                                                        if (gEnt->classname == scr_const.trigger_radius
                                                            || gEnt->classname == scr_const.trigger_radius_use && gEnt->s.un3.item > 0)
                                                        {
                                                            useList[useListCount].score = useList[useListCount].score - (float)(baseScore * 1.5);
                                                        }
                                                        if (gEnt->classname == scr_const.trigger_use || gEnt->classname == scr_const.trigger_radius)
                                                            useList[useListCount].score = useList[useListCount].score - baseScore;
                                                        if (gEnt->s.eType == 6)
                                                            useList[useListCount].score = useList[useListCount].score - (float)(baseScore * 0.75);
                                                    }
                                                    if (gEnt->s.eType == 11)
                                                    {
                                                        if (G_IsTurretUsable(gEnt, ent))
                                                            useList[useListCount].score = useList[useListCount].score - (float)(baseScore * 0.5);
                                                        else
                                                            useList[useListCount].score = useList[useListCount].score + 10000.0;
                                                    }
                                                    if (gEnt->s.eType == 3
                                                        && (!BG_CanItemBeGrabbed(&gEnt->s, &ent->client->ps, 0)
                                                            || BG_PlayerWeaponBlockPickupWeapon(&ent->client->ps, ent->client->ps.weapon)))
                                                    {
                                                        useList[useListCount].score = useList[useListCount].score + 10000.0;
                                                        ++blockedPickupCount;
                                                    }
                                                    useList[useListCount].ent = gEnt;
                                                    useList[useListCount].score = useList[useListCount].score + distNorm;
                                                    ++useListCount;
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
    }
    std::sort(useList, useList + useListCount, compare_use);

    useListCount -= blockedPickupCount;
    blockedSightCount = 0;
    col_context_t context(17);

    //col_context_t::col_context_t(&context, 17);
    context.passEntityNum0 = ps->clientNum;
    context.passEntityNum1 = ps->clientNum;
    hitNum = -1;
    for (i = 0; i < useListCount; ++i)
    {
        ent2 = useList[i].ent;
        if (ent2->classname != scr_const.trigger_use_touch && ent2->classname != scr_const.trigger_radius)
        {
            pos[0] = ent2->r.absmin[0] + ent2->r.absmax[0];
            pos[1] = ent2->r.absmin[1] + ent2->r.absmax[1];
            pos[2] = ent2->r.absmin[2] + ent2->r.absmax[2];
            pos[0] *= 0.5;
            pos[1] *= 0.5;
            pos[2] *= 0.5;
            if (ent2->s.eType == 14)
                pos[2] += (float)(ent2->r.maxs[2] * 0.5);
            if (ent2->s.eType == 11)
                G_DObjGetWorldTagPos(ent2, scr_const.tag_aim, pos);
            if (ent2->classname != scr_const.trigger_radius_use || ent2->s.otherEntityNum == 1023)
                context.passEntityNum1 = ent2->s.number;
            else
                context.passEntityNum1 = ent2->s.otherEntityNum;
            if (!SV_SightTracePoint(&hitNum, origin, pos, &context))
            {
                useList[i].score = useList[i].score + 10000.0;
                ++blockedSightCount;
            }
        }
    }
    std::sort(useList, useList + useListCount, compare_use);
    return useListCount - blockedSightCount;
}

// Decomp: CoDMPServer.c:453564
bool __cdecl BG_PlayerWeaponBlockPickupWeapon(const playerState_s *ps, unsigned int weaponIndex)
{
    const PlayerHeldWeapon *heldWeapon;

    if ( !ps && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../bgame/bg_weapons.h", 394, 0, "%s", "ps") )
        __debugbreak();
    heldWeapon = BG_GetHeldWeaponConst(ps, weaponIndex);
    return heldWeapon && heldWeapon->blockWeaponPickup;
}

// Decomp: CoDMPServer.c:453595
int __cdecl Player_GetItemCursorHint(const gclient_s *client, const gentity_s *traceEnt)
{
    const WeaponDef *weapDefItem;
    const WeaponDef *weapDefPlayer;
    int index;

    if ( !traceEnt
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp", 654, 0, "%s", "traceEnt") )
    {
        __debugbreak();
    }
    if ( !client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp", 655, 0, "%s", "client") )
    {
        __debugbreak();
    }
    index = traceEnt->s.un3.item;
    if ( (unsigned int)index >= 0x800
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp",
                    659,
                    0,
                    "%s\n\t(index) = %i",
                    "((0 <= index) && (index < (( 1 << 11 ) )))",
                    index) )
    {
        __debugbreak();
    }
    //if ( bg_itemlist[index] != IT_WEAPON
    //    && !Assert_MyHandler(
    //                "C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp",
    //                662,
    //                0,
    //                "%s",
    //                "item->giType == IT_WEAPON") )
    //{
    //    __debugbreak();
    //}
    weapDefItem = BG_GetWeaponDef(index % 2048);
    weapDefPlayer = BG_GetWeaponDef(client->ps.weapon);
    if ( BG_PlayerHasWeapon(&client->ps, index % 2048) )
        return 0;
    if ( weapDefPlayer->inventoryType == WEAPINVENTORY_PRIMARY
        || weapDefPlayer->inventoryType == WEAPINVENTORY_ALTMODE
        || weapDefItem->inventoryType && weapDefItem->inventoryType != WEAPINVENTORY_ALTMODE
        || BG_PlayerWeaponCountPrimaryTypes(&client->ps) < 2 )
    {
        return index % 2048 + 7;
    }
    return 0;
}

// Decomp: CoDMPServer.c:453662
void __cdecl Player_SetTurretDropHint(gentity_s *ent)
{
    gentity_s *turret;
    gclient_s *ps;

    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp", 689, 0, "%s", "ent") )
        __debugbreak();
    if ( !ent->client
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp", 690, 0, "%s", "ent->client") )
    {
        __debugbreak();
    }
    if ( !ent->active
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp", 691, 0, "%s", "ent->active") )
    {
        __debugbreak();
    }
    ps = ent->client;
    if ( (ps->ps.eFlags & 0x300) == 0
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp",
                    695,
                    0,
                    "%s",
                    "ps->eFlags & EF_TURRET_ACTIVE") )
    {
        __debugbreak();
    }
    if ( ps->ps.viewlocked_entNum == 1023
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp",
                    696,
                    0,
                    "%s",
                    "ps->viewlocked_entNum != ENTITYNUM_NONE") )
    {
        __debugbreak();
    }
    turret = &level.gentities[ps->ps.viewlocked_entNum];
    if ( turret->s.eType != 11
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\player_use_sp.cpp",
                    699,
                    0,
                    "%s",
                    "turret->s.eType == ET_MG42") )
    {
        __debugbreak();
    }
    if ( *BG_GetWeaponDef(turret->s.weapon)->dropHintString )
    {
        ps->ps.cursorHintEntIndex = 1023;
        ps->ps.cursorHint = turret->s.weapon + 7;
        ps->ps.cursorHintString = BG_GetWeaponDef(turret->s.weapon)->dropHintStringIndex;
    }
}

