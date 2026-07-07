#include "actor_aim.h"
#include "actor_senses.h"
#include "g_weapon.h"
#include <clientscript/cscr_stringlist.h>
#include <clientscript/cscr_vm.h>
#include <game/g_main_wrapper.h>
#include <game/actor_wrapper.h>
#include "bullet.h"
#include <bgame/bg_misc.h>
#include <bgame/bg_weapons_def.h>
#include <game/g_utils_wrapper.h>
#include "actor_events.h"
#include "actor_turret.h"

#ifdef KISAK_SP
#include "actor_sp.h"

// Decomp: CoDSP_rdBlackOps.map.c (Actor_GetPlayerSightAccuracy ~82521588)
// Map: CoDSP_rd.map VA 0x82521588 (actor_aim.obj)
float __fastcall Actor_GetPlayerSightAccuracy(actor_s *self, sentient_s *targetSentient)
{
    float accuracy;
    float enemyEye[3];
    float playerViewPos[3];
    float actorEye[3];
    float delta[3];
    float checkPos[3];
    gentity_s *playerEnt;
    float *playerOrigin;
    float dx;
    float dy;
    bool inPlayerCone;

    iassert(self);
    iassert(targetSentient);
    iassert(targetSentient->ent);
    iassert(targetSentient->ent->client);

    accuracy = 0.0f;
    Sentient_GetEyePosition(targetSentient, enemyEye);
    playerEnt = targetSentient->ent;
    playerViewPos[0] = playerEnt->client->ps.origin[0];
    playerViewPos[1] = playerEnt->client->ps.origin[1];
    playerViewPos[2] = playerEnt->client->ps.origin[2];
    delta[0] = enemyEye[0] - playerViewPos[0];
    delta[1] = enemyEye[1] - playerViewPos[1];
    delta[2] = enemyEye[2] - playerViewPos[2];

    if ( !Actor_IsUsingTurret(self) )
    {
        Actor_GetEyePosition(self, actorEye);
        checkPos[0] = (enemyEye[0] + playerViewPos[0]) * 0.5f;
        checkPos[1] = (enemyEye[1] + playerViewPos[1]) * 0.5f;
        checkPos[2] = (enemyEye[2] + playerViewPos[2]) * 0.5f;
    }

    if ( Actor_CanSeePointEx(self, enemyEye, self->fovDot, self->fMaxSightDistSqrd, playerEnt->s.number) )
        accuracy += 10.0f;

    checkPos[0] = playerViewPos[0] + delta[0] * 0.75f;
    checkPos[1] = playerViewPos[1] + delta[1] * 0.75f;
    checkPos[2] = playerViewPos[2] + delta[2] * 0.75f;
    if ( Actor_CanSeePointEx(self, checkPos, self->fovDot, self->fMaxSightDistSqrd, playerEnt->s.number) )
        accuracy += 30.0f;

    checkPos[0] = playerViewPos[0] + delta[0] * 0.5f;
    checkPos[1] = playerViewPos[1] + delta[1] * 0.5f;
    checkPos[2] = playerViewPos[2] + delta[2] * 0.5f;
    if ( Actor_CanSeePointEx(self, checkPos, self->fovDot, self->fMaxSightDistSqrd, playerEnt->s.number) )
        accuracy += 30.0f;

    checkPos[0] = playerViewPos[0] + delta[0] * 0.25f;
    checkPos[1] = playerViewPos[1] + delta[1] * 0.25f;
    checkPos[2] = playerViewPos[2] + delta[2] * 0.25f;
    if ( Actor_CanSeePointEx(self, checkPos, self->fovDot, self->fMaxSightDistSqrd, playerEnt->s.number) )
        accuracy += 30.0f;

    accuracy *= 0.01f;

    playerOrigin = targetSentient->ent->r.currentOrigin;
    dx = playerOrigin[0] - playerViewPos[0];
    dy = playerOrigin[1] - playerViewPos[1];
    inPlayerCone = (dx * dx + dy * dy) < 65536.0f;
    if ( accuracy < 0.75f && inPlayerCone )
        accuracy = 0.75f;
    else if ( accuracy < 0.1f )
        accuracy = 0.1f;

    if ( accuracy < 0.0f )
        accuracy = 0.0f;
    if ( accuracy > 1.0f )
        accuracy = 1.0f;
    return accuracy;
}

#endif // KISAK_SP

void __cdecl Actor_FillWeaponParms(actor_s *self, weaponParms *wp)
{
    if ( !self && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\actor_aim.cpp", 437, 0, "%s", "self") )
        __debugbreak();
    if ( !wp && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\actor_aim.cpp", 438, 0, "%s", "wp") )
        __debugbreak();
    if ( ((LODWORD(self->vLookForward[0]) & 0x7F800000) == 0x7F800000
         || (LODWORD(self->vLookForward[1]) & 0x7F800000) == 0x7F800000
         || (LODWORD(self->vLookForward[2]) & 0x7F800000) == 0x7F800000)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\actor_aim.cpp",
                    440,
                    0,
                    "%s",
                    "!IS_NAN((self->vLookForward)[0]) && !IS_NAN((self->vLookForward)[1]) && !IS_NAN((self->vLookForward)[2])") )
    {
        __debugbreak();
    }
    if ( ((LODWORD(self->vLookRight[0]) & 0x7F800000) == 0x7F800000
         || (LODWORD(self->vLookRight[1]) & 0x7F800000) == 0x7F800000
         || (LODWORD(self->vLookRight[2]) & 0x7F800000) == 0x7F800000)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\actor_aim.cpp",
                    441,
                    0,
                    "%s",
                    "!IS_NAN((self->vLookRight)[0]) && !IS_NAN((self->vLookRight)[1]) && !IS_NAN((self->vLookRight)[2])") )
    {
        __debugbreak();
    }
    if ( ((LODWORD(self->vLookUp[0]) & 0x7F800000) == 0x7F800000
         || (LODWORD(self->vLookUp[1]) & 0x7F800000) == 0x7F800000
         || (LODWORD(self->vLookUp[2]) & 0x7F800000) == 0x7F800000)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\actor_aim.cpp",
                    442,
                    0,
                    "%s",
                    "!IS_NAN((self->vLookUp)[0]) && !IS_NAN((self->vLookUp)[1]) && !IS_NAN((self->vLookUp)[2])") )
    {
        __debugbreak();
    }
    wp->forward[0] = self->vLookForward[0];
    wp->forward[1] = self->vLookForward[1];
    wp->forward[2] = self->vLookForward[2];
    wp->right[0] = self->vLookRight[0];
    wp->right[1] = self->vLookRight[1];
    wp->right[2] = self->vLookRight[2];
    wp->up[0] = self->vLookUp[0];
    wp->up[1] = self->vLookUp[1];
    wp->up[2] = self->vLookUp[2];
    if ( Actor_GetMuzzleInfo(self, wp->muzzleTrace, wp->gunForward) )
    {
        wp->forward[0] = wp->gunForward[0];
        wp->forward[1] = wp->gunForward[1];
        wp->forward[2] = wp->gunForward[2];
    }
    else
    {
        Actor_GetEyePosition(self, wp->muzzleTrace);
        wp->gunForward[0] = wp->forward[0];
        wp->gunForward[1] = wp->forward[1];
        wp->gunForward[2] = wp->forward[2];
    }
    if ( ((LODWORD(wp->muzzleTrace[0]) & 0x7F800000) == 0x7F800000
         || (LODWORD(wp->muzzleTrace[1]) & 0x7F800000) == 0x7F800000
         || (LODWORD(wp->muzzleTrace[2]) & 0x7F800000) == 0x7F800000)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\actor_aim.cpp",
                    458,
                    0,
                    "%s",
                    "!IS_NAN((wp->muzzleTrace)[0]) && !IS_NAN((wp->muzzleTrace)[1]) && !IS_NAN((wp->muzzleTrace)[2])") )
    {
        __debugbreak();
    }
    if ( ((LODWORD(wp->gunForward[0]) & 0x7F800000) == 0x7F800000
         || (LODWORD(wp->gunForward[1]) & 0x7F800000) == 0x7F800000
         || (LODWORD(wp->gunForward[2]) & 0x7F800000) == 0x7F800000)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\actor_aim.cpp",
                    459,
                    0,
                    "%s",
                    "!IS_NAN((wp->gunForward)[0]) && !IS_NAN((wp->gunForward)[1]) && !IS_NAN((wp->gunForward)[2])") )
    {
        __debugbreak();
    }
}

void __cdecl Actor_InitWeaponParms(actor_s *self, unsigned int weapon, weaponParms *wp)
{
    Actor_FillWeaponParms(self, wp);
    Weapon_SetWeaponParamsWeapon(wp, weapon);
}

gentity_s *__cdecl Actor_Melee(actor_s *self, const float *direction)
{
    char *v2; // eax
    float v4; // [esp+10h] [ebp-8Ch]
    float v5; // [esp+2Ch] [ebp-70h]
    float enemyEye[3]; // [esp+38h] [ebp-64h] BYREF
    int weapon; // [esp+44h] [ebp-58h]
    sentient_s *enemy; // [esp+48h] [ebp-54h]
    weaponParms wp; // [esp+4Ch] [ebp-50h] BYREF
    gentity_s *ent; // [esp+98h] [ebp-4h]

    if ( !self && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\actor_aim.cpp", 955, 0, "%s", "self") )
        __debugbreak();
    if ( !self && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\actor_aim.cpp", 961, 0, "%s", "self") )
        __debugbreak();
    ent = self->ent;
    v2 = SL_ConvertToString(self->weaponName, SCRIPTINSTANCE_SERVER);
    weapon = G_GetWeaponIndexForName(v2);
    if ( self->lastShotTime == level.time )
    {
        Com_PrintError(
            18,
            "ERROR: Attempt for same actor (entnum %d) to shoot/melee more than once in a frame.\n",
            ent->s.number);
        return 0;
    }
    else
    {
        self->lastShotTime = level.time;
        enemy = Actor_GetTargetSentient(self);
        Actor_InitWeaponParms(self, weapon, &wp);
        Actor_GetEyePosition(self, wp.muzzleTrace);
        if ( enemy )
        {
            Sentient_GetThirdPersonEyePosition(enemy, enemyEye);
            wp.forward[0] = enemyEye[0] - wp.muzzleTrace[0];
            wp.forward[1] = enemyEye[1] - wp.muzzleTrace[1];
            wp.forward[2] = enemyEye[2] - wp.muzzleTrace[2];
            if ( direction )
            {
                v4 = Vec2Length(direction);
                v5 = Vec2Length(wp.forward) / v4;
                wp.forward[0] = v5 * *direction;
                wp.forward[1] = v5 * direction[1];
            }
            Vec3Normalize(wp.forward);
        }
        else if ( direction )
        {
            wp.forward[0] = *direction;
            wp.forward[1] = direction[1];
            wp.forward[2] = direction[2];
            Vec3Normalize(wp.forward);
        }
        ent->s.weapon = weapon;
        if ( ent->s.weapon != weapon
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\actor_aim.cpp",
                        1014,
                        0,
                        "%s",
                        "ent->s.weapon == weapon") )
        {
            __debugbreak();
        }
        return Weapon_Melee(
                         self->ent,
                         &wp,
                         ai_meleeRange->current.value,
                         ai_meleeWidth->current.value,
                         ai_meleeHeight->current.value,
                         level.time);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Actor_StopShoot ~5918109)
void __fastcall Actor_StopShoot(actor_s *self)
{
    iassert(self);
    iassert(self->ent);
    self->ent->s.lerp.eFlags &= ~0x40u;
}

// Decomp: CoDSP_rdBlackOps.map.c (Actor_ShootNoEnemy ~5917582)
static void Actor_ShootNoEnemy(weaponParms *wp)
{
    wp->forward[0] = wp->gunForward[0];
    wp->forward[1] = wp->gunForward[1];
    wp->forward[2] = wp->gunForward[2];
}

// Decomp: CoDSP_rdBlackOps.map.c (Actor_ShootPos ~5917590)
static void Actor_ShootPos(weaponParms *wp, const float *targetPos)
{
    wp->forward[0] = targetPos[0] - wp->muzzleTrace[0];
    wp->forward[1] = targetPos[1] - wp->muzzleTrace[1];
    wp->forward[2] = targetPos[2] - wp->muzzleTrace[2];
    Vec3Normalize(wp->forward);
}

// Decomp: CoDSP_rdBlackOps.map.c (Actor_GetFinalAccuracy ~5915784)
static float Actor_GetFinalAccuracy(actor_s *self, float accuracyMod)
{
    sentient_s *enemy;
    float accuracy;

    enemy = Actor_GetTargetSentient(self);
    accuracy = self->accuracy * accuracyMod * self->playerSightAccuracy;
    if ( enemy && enemy->ent && enemy->ent->client )
        accuracy = accuracy * enemy->attackerAccuracy;
    if ( accuracy < 0.0f )
        accuracy = 0.0f;
    if ( accuracy > 1.0f )
        accuracy = 1.0f;
    self->debugLastAccuracy = accuracy;
    return accuracy;
}

#ifdef KISAK_SP
// Decomp: CoDSP_rdBlackOps.map.c (Actor_ShootBlank ~82524E70)
void __fastcall Actor_ShootBlank(actor_s *self)
{
    gentity_s *ent;
    unsigned int weapon;
    weaponParms wp;
    const WeaponDef *weapDef;
    const char *weaponNameStr;

    iassert(self);
    iassert(self->ent);
    ent = self->ent;
    if ( self->lastShotTime == level.time )
    {
        Com_PrintError(
            18,
            "ERROR: Attempt for same actor (entnum %d) to shoot/melee more than once in a frame.\n",
            ent->s.number);
        return;
    }
    self->lastShotTime = level.time;
    weaponNameStr = SL_ConvertToString(self->weaponName, SCRIPTINSTANCE_SERVER);
    weapon = G_GetWeaponIndexForName((char *)weaponNameStr);
    weapDef = BG_GetWeaponDef(weapon);
    if ( !weapDef || weapDef->weapType != WEAPTYPE_BULLET )
    {
        Scr_Error(
            va("ShootBlank() only works with bullet weapons.  Using weapon [%s]", BG_WeaponName(weapon)),
            0);
        return;
    }
    Actor_InitWeaponParms(self, weapon, &wp);
    Actor_ShootNoEnemy(&wp);
    ent->s.weapon = weapon;
    iassert(ent->s.weapon == weapon);
    iassert(weapDef->weapType == WEAPTYPE_BULLET);
    G_AddEvent(ent, EV_FIRE_WEAPON, 0);
}
#endif

// Decomp: CoDSP_rdBlackOps.map.c (Actor_Shoot ~5915270)
void __fastcall Actor_Shoot(actor_s *self, const float *optionalTarget, int addEvents, float accuracyMod)
{
    gentity_s *ent;
    unsigned int weapon;
    weaponParms wp;
    gentity_s *targetEnt;
    const char *weaponNameStr;
    const WeaponDef *weapDef;

    iassert(self);
    iassert(self->ent);
    ent = self->ent;
    if ( self->lastShotTime == level.time )
    {
        Com_PrintError(
            18,
            "ERROR: Attempt for same actor (entnum %d) to shoot/melee more than once in a frame.\n",
            ent->s.number);
        return;
    }
    self->lastShotTime = level.time;
    weaponNameStr = SL_ConvertToString(self->weaponName, SCRIPTINSTANCE_SERVER);
    weapon = G_GetWeaponIndexForName((char *)weaponNameStr);
    Actor_InitWeaponParms(self, weapon, &wp);
    targetEnt = Actor_GetTargetEntity(self);
    if ( optionalTarget )
        Actor_ShootPos(&wp, optionalTarget);
    else if ( targetEnt )
    {
        float targetPos[3];

        if ( targetEnt->sentient )
            Sentient_GetThirdPersonEyePosition(targetEnt->sentient, targetPos);
        else
            G_EntityCentroid(targetEnt, targetPos);
        Actor_ShootPos(&wp, targetPos);
        if ( Actor_GetFinalAccuracy(self, accuracyMod) > G_random() )
            ++self->hitCount;
        else
            ++self->missCount;
    }
    else
        Actor_ShootNoEnemy(&wp);

    ent->s.weapon = weapon;
    ent->s.lerp.eFlags |= 0x40u;
    weapDef = BG_GetWeaponDef(weapon);
    if ( weapDef->weapType == WEAPTYPE_BULLET )
    {
        Bullet_Fire(ent, 0.0f, &wp, ent, level.time);
        if ( addEvents )
            G_AddEvent(ent, EV_FIRE_WEAPON, 0);
    }
}

