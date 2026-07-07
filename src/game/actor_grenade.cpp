#include "actor_grenade.h"

#include "actor_cover.h"
#include "actor_animapi.h"
#include "actor_orientation.h"
#include "actor_state.h"
#include "actor_team_move.h"
#include "bullet.h"
#include "enthandle.h"
#include "g_missile.h"
#include "sentient.h"

#include <bgame/bg_misc.h>
#include <bgame/bg_weapons_def.h>
#include <clientscript/cscr_vm.h>
#include <clientscript/scr_const.h>
#include <game/actor_wrapper.h>
#include <game/g_main_wrapper.h>
#include <game/g_spawn_wrapper.h>
#include <game/g_utils_wrapper.h>
#include <qcommon/cm_trace.h>
#include <universal/q_shared.h>
#include <server/sv_world.h>
#include <universal/assertive.h>
#include <xanim/xmodel_utils.h>
#include <cmath>

// Decomp: CoDSPBlackOps.map.c

void __cdecl SP_info_grenade_hint(gentity_s *self)
{
    iassert(self);
    self->s.eType = ET_GENERAL;
}

void __fastcall Actor_Grenade_GetTossFromPosition(actor_s *self, float *tossPos)
{
    iassert(self);
    iassert(self->ent);
    iassert(tossPos);
    tossPos[0] = self->ent->r.currentOrigin[0];
    tossPos[1] = self->ent->r.currentOrigin[1];
    tossPos[2] = self->ent->r.currentOrigin[2] + 64.0f;
}

bool __fastcall Actor_Grenade_GetTossPositions(actor_s *self, float (*tossPositions)[3], int *count)
{
    iassert(self);
    iassert(tossPositions);
    iassert(count);
    Actor_Grenade_GetTossFromPosition(self, tossPositions[0]);
    *count = 1;
    return true;
}

// Decomp: CoDSP_rdBlackOps.map.c (8253FB08 Actor_Grenade_GetTossPositions)
void __cdecl Actor_Grenade_GetTossPositionsPos(const float *from, const float *target, float *landPos, unsigned int weaponIndex)
{
    const WeaponDef *weapDef;
    float traceStart[3];
    float traceEnd[3];
    trace_t trace;
    unsigned int surfaceIndex;
    float blend;
    float t;

    iassert(from);
    iassert(target);
    iassert(landPos);
    weapDef = BG_GetWeaponDef(weaponIndex);
    if ( !weapDef )
    {
        landPos[0] = target[0];
        landPos[1] = target[1];
        landPos[2] = target[2];
        return;
    }
    if ( weapDef->stickiness != WEAPSTICKINESS_NONE )
    {
        landPos[0] = target[0];
        landPos[1] = target[1];
        landPos[2] = target[2];
        return;
    }
    traceStart[0] = target[0];
    traceStart[1] = target[1];
    traceStart[2] = target[2] + 1.0f;
    traceEnd[0] = target[0];
    traceEnd[1] = target[1];
    traceEnd[2] = target[2] - 1.0f;
    G_MissileTrace(&trace, traceStart, traceEnd, ENTITYNUM_NONE, 2065, weaponIndex);
    if ( trace.fraction == 1.0f )
        surfaceIndex = 0;
    else
        surfaceIndex = (trace.sflags & 0x3F00000) >> 20;
    if ( weapDef->locationDamageMultipliers )
        blend = weapDef->locationDamageMultipliers[surfaceIndex];
    else
        blend = 0.9397f;
    t = 1.0f - blend;
    landPos[0] = from[0] + t * (target[0] - from[0]);
    landPos[1] = from[1] + t * (target[1] - from[1]);
    landPos[2] = from[2] + t * (target[2] - from[2]);
}

bool __fastcall Actor_Grenade_GetTossPositionsFromHints(actor_s *self, float (*tossPositions)[3], int *count)
{
    return Actor_Grenade_GetTossPositions(self, tossPositions, count);
}

static void Actor_Grenade_TrajectoryPos(
    const float *from,
    const float *vel,
    float gravity,
    float time,
    float *out)
{
    out[0] = from[0] + vel[0] * time;
    out[1] = from[1] + vel[1] * time;
    out[2] = from[2] + vel[2] * time + 0.5f * gravity * time * time;
}

static bool Actor_Grenade_SightTraceClear(
    actor_s *self,
    const float *start,
    const float *end)
{
    int hitNum;
    float mins[3] = { -1.0f, -1.0f, -1.0f };
    float maxs[3] = { 1.0f, 1.0f, 1.0f };

    hitNum = 0;
    col_context_t context(42004625);
    context.passEntityNum0 = self->ent->s.number;
    SV_SightTraceCapsule(&hitNum, start, mins, maxs, end, &context);
    return hitNum == 0;
}

bool __fastcall Actor_Grenade_IsValidTrajectory(
    actor_s *self,
    const float *from,
    const float *vel,
    const float *land)
{
    float gravity;
    float tApex;
    float tHalfFar;
    float totalTime;
    float predictedPos[3];
    float nearPos[3];
    float midPos[3];
    float farPos[3];
    trace_t trace;
    float mins[3] = { -1.0f, -1.0f, -1.0f };
    float maxs[3] = { 1.0f, 1.0f, 1.0f };
    int hitEntId;

    iassert(self);
    iassert(self->ent);
    iassert(from);
    iassert(vel);
    iassert(land);
    gravity = bg_gravity->current.value;
    if ( gravity <= 0.0f )
        return false;
    if ( vel[2] <= 0.0f )
    {
        float discriminant = 2.0f * gravity * (from[2] - land[2]) + vel[2] * vel[2];
        if ( discriminant <= 0.0f )
            return false;
        tApex = 0.0f;
        tHalfFar = (sqrtf(discriminant) + vel[2]) / gravity * 0.5f;
    }
    else
    {
        float discriminant;
        tApex = vel[2] / gravity;
        discriminant = 2.0f * (from[2] - land[2]) / gravity + tApex * tApex;
        if ( discriminant <= 0.0f )
            return false;
        tHalfFar = sqrtf(discriminant) * 0.5f;
    }
    totalTime = tHalfFar + tApex + tHalfFar;
    Actor_Grenade_TrajectoryPos(from, vel, gravity, totalTime, predictedPos);
    if ( fabsf(predictedPos[0] - land[0]) > 0.1f || fabsf(predictedPos[1] - land[1]) > 0.1f )
        return false;
    if ( tApex > 0.0f )
    {
        Actor_Grenade_TrajectoryPos(from, vel, gravity, tHalfFar, nearPos);
        if ( !Actor_Grenade_SightTraceClear(self, from, nearPos) )
            return false;
        Actor_Grenade_TrajectoryPos(from, vel, gravity, tHalfFar + tApex, midPos);
        if ( !Actor_Grenade_SightTraceClear(self, nearPos, midPos) )
            return false;
    }
    else
    {
        midPos[0] = from[0];
        midPos[1] = from[1];
        midPos[2] = from[2];
    }
    Actor_Grenade_TrajectoryPos(from, vel, gravity, totalTime - tHalfFar, farPos);
    if ( !Actor_Grenade_SightTraceClear(self, midPos, farPos) )
        return false;
    col_context_t context(42004625);
    context.passEntityNum0 = self->ent->s.number;
    G_TraceCapsule(&trace, farPos, mins, maxs, land, self->ent->s.number, 42004625, &context);
    if ( trace.fraction == 1.0f )
        return true;
    hitEntId = Trace_GetEntityHitId(&trace);
    if ( hitEntId >= 0 && hitEntId < MAX_GENTITIES && g_entities[hitEntId].r.inuse && g_entities[hitEntId].sentient )
        return g_entities[hitEntId].sentient->eTeam == Sentient_EnemyTeam(self->sentient->eTeam);
    {
        float hitPos[3];
        float missDelta[3];
        hitPos[0] = farPos[0] + (land[0] - farPos[0]) * trace.fraction;
        hitPos[1] = farPos[1] + (land[1] - farPos[1]) * trace.fraction;
        hitPos[2] = farPos[2] + (land[2] - farPos[2]) * trace.fraction;
        Vec3Sub(hitPos, land, missDelta);
        return Vec3LengthSq(missDelta) <= Square(30.0f);
    }
}

bool __fastcall Actor_Grenade_CheckMaximumEnergyToss(
    actor_s *self,
    const float *from,
    const float *land,
    float *tossVel,
    bool lowArc)
{
    float delta[3];
    float horizDistSq;
    float energyTerm;
    float discriminant;
    float totalTime;
    float gravity;

    iassert(self);
    iassert(from);
    iassert(land);
    iassert(tossVel);
    gravity = bg_gravity->current.value;
    if ( gravity <= 0.0f )
        return false;
    delta[0] = land[0] - from[0];
    delta[1] = land[1] - from[1];
    delta[2] = land[2] - from[2];
    horizDistSq = delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2];
    if ( horizDistSq < 1.0f )
        return false;
    energyTerm = -(gravity * delta[2] - 810000.0f);
    discriminant = energyTerm * energyTerm
        - gravity * gravity * horizDistSq;
    if ( discriminant < 0.0f )
        return false;
    if ( lowArc )
        totalTime = (energyTerm - sqrtf(discriminant)) / gravity;
    else
        totalTime = (energyTerm + sqrtf(discriminant)) / gravity;
    if ( totalTime == 0.0f )
        return false;
    tossVel[0] = delta[0] / totalTime;
    tossVel[1] = delta[1] / totalTime;
    tossVel[2] = delta[2] / totalTime + 0.5f * gravity * totalTime;
    return Actor_Grenade_IsValidTrajectory(self, from, tossVel, land);
}

bool __fastcall Actor_Grenade_CheckInfiniteEnergyToss(
    actor_s *self,
    const float *from,
    const float *land,
    float *tossVel)
{
    float delta[3];
    float totalDistSq;
    float totalTime;
    float gravity;

    iassert(self);
    iassert(from);
    iassert(land);
    iassert(tossVel);
    gravity = bg_gravity->current.value;
    if ( gravity <= 0.0f )
        return false;
    delta[0] = land[0] - from[0];
    delta[1] = land[1] - from[1];
    delta[2] = land[2] - from[2];
    totalDistSq = delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2];
    if ( totalDistSq <= 0.0f )
        return false;
    totalTime = sqrtf((sqrtf(totalDistSq) * gravity) * 2.0f) / gravity;
    if ( totalTime == 0.0f )
        return false;
    tossVel[0] = delta[0] / totalTime;
    tossVel[1] = delta[1] / totalTime;
    tossVel[2] = delta[2] / totalTime + 0.5f * gravity * totalTime;
    return Actor_Grenade_IsValidTrajectory(self, from, tossVel, land);
}

bool __fastcall Actor_Grenade_CheckMinimumEnergyToss(
    actor_s *self,
    const float *from,
    const float *land,
    float *tossVel)
{
    float delta[3];
    float totalDist;
    float totalTime;
    float gravity;

    iassert(self);
    iassert(from);
    iassert(land);
    iassert(tossVel);
    gravity = bg_gravity->current.value;
    if ( gravity <= 0.0f )
        return false;
    delta[0] = land[0] - from[0];
    delta[1] = land[1] - from[1];
    delta[2] = land[2] - from[2];
    totalDist = Vec3Length(delta);
    if ( totalDist <= 0.0f )
        return false;
    if ( (delta[2] + totalDist) * gravity > 810000.0f )
        return false;
    totalTime = sqrtf((totalDist / gravity) * 2.0f);
    if ( totalTime == 0.0f )
        return false;
    tossVel[0] = delta[0] / totalTime;
    tossVel[1] = delta[1] / totalTime;
    tossVel[2] = delta[2] / totalTime + 0.5f * gravity * totalTime;
    return Actor_Grenade_IsValidTrajectory(self, from, tossVel, land);
}

bool __fastcall Actor_Grenade_CheckGrenadeHintToss(
    actor_s *self,
    const float *from,
    const float *land,
    float *tossVel)
{
    return Actor_Grenade_CheckMaximumEnergyToss(self, from, land, tossVel, false);
}

bool __fastcall Actor_Grenade_CheckTossPos(actor_s *self, const float *targetPos, float *tossVel)
{
    float tossFrom[3];

    Actor_Grenade_GetTossFromPosition(self, tossFrom);
    return Actor_Grenade_CheckTossPosFrom(self, tossFrom, targetPos, tossVel);
}

bool __fastcall Actor_Grenade_CheckTossPosFrom(
    actor_s *self,
    const float *tossFrom,
    const float *targetPos,
    float *tossVel)
{
    float landPos[3];
    trace_t trace;
    bool success;

    iassert(self);
    iassert(self->sentient);
    iassert(tossFrom);
    iassert(targetPos);
    iassert(tossVel);
    if ( bg_gravity->current.value <= 0.0f )
        return false;
    if ( self->pGrenade.isDefined() )
        return false;
    landPos[0] = targetPos[0];
    landPos[1] = targetPos[1];
    landPos[2] = targetPos[2];
    if ( !Actor_Grenade_IsPointSafe(self, landPos) )
        return false;
    G_MissileTrace(&trace, tossFrom, landPos, ENTITYNUM_NONE, 2065, self->iGrenadeWeaponIndex);
    if ( trace.fraction < 1.0f )
    {
        Vec3Lerp(tossFrom, landPos, trace.fraction, landPos);
    }
    if ( Vec3DistanceSq(tossFrom, landPos) == 0.0f )
        return false;
    success = Actor_Grenade_CheckMaximumEnergyToss(self, tossFrom, landPos, tossVel, false);
    if ( !success )
        success = Actor_Grenade_CheckMaximumEnergyToss(self, tossFrom, landPos, tossVel, true);
    if ( success )
    {
        self->vGrenadeTossVel[0] = tossVel[0];
        self->vGrenadeTossVel[1] = tossVel[1];
        self->vGrenadeTossVel[2] = tossVel[2];
        self->vGrenadeTossPos[0] = tossFrom[0];
        self->vGrenadeTossPos[1] = tossFrom[1];
        self->vGrenadeTossPos[2] = tossFrom[2];
        self->vGrenadeTargetPos[0] = landPos[0];
        self->vGrenadeTargetPos[1] = landPos[1];
        self->vGrenadeTargetPos[2] = landPos[2];
        self->bGrenadeTossValid = 1;
        self->bGrenadeTargetValid = 1;
    }
    return success;
}

#ifdef KISAK_SP
// Decomp: CoDSP_rdBlackOps.map.c (Actor_GrenadeLauncher_CheckPos ~82544E64)
bool __fastcall Actor_GrenadeLauncher_CheckPos(
    actor_s *self,
    const float *standPos,
    const float *handOffset,
    const float *targetPos,
    float *posOut,
    float *velOut,
    float launchSpeed)
{
    float gravity;
    float delta[3];
    float horizDistSq;
    float energyTerm;
    float discriminant;
    float totalTime;
    float launchFrom[3];

    iassert(self);
    iassert(standPos);
    iassert(targetPos);
    iassert(posOut);
    iassert(velOut);
    gravity = bg_gravity->current.value;
    if ( gravity <= 0.0f )
        return false;
    if ( handOffset )
    {
        launchFrom[0] = standPos[0] + handOffset[0];
        launchFrom[1] = standPos[1] + handOffset[1];
        launchFrom[2] = standPos[2] + handOffset[2];
    }
    else
    {
        launchFrom[0] = standPos[0];
        launchFrom[1] = standPos[1];
        launchFrom[2] = standPos[2];
    }
    posOut[0] = launchFrom[0];
    posOut[1] = launchFrom[1];
    posOut[2] = launchFrom[2];
    delta[0] = targetPos[0] - launchFrom[0];
    delta[1] = targetPos[1] - launchFrom[1];
    delta[2] = targetPos[2] - launchFrom[2];
    energyTerm = launchSpeed * launchSpeed - gravity * delta[2];
    horizDistSq = delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2];
    discriminant = energyTerm * energyTerm - horizDistSq * gravity * gravity;
    if ( discriminant < 0.0f )
        return false;
    totalTime = (sqrtf(2.0f * (energyTerm - sqrtf(discriminant)))) / gravity;
    if ( totalTime == 0.0f )
        return false;
    velOut[0] = delta[0] / totalTime;
    velOut[1] = delta[1] / totalTime;
    velOut[2] = delta[2] / totalTime + 0.5f * gravity * totalTime;
    return Actor_Grenade_IsValidTrajectory(self, launchFrom, velOut, targetPos);
}

// Decomp: CoDSP_rdBlackOps.map.c (Actor_CheckGrenadeLaunch ~8256D2B0)
bool __fastcall Actor_CheckGrenadeLaunch(actor_s *self, const float *standPos, const float *handOffset)
{
    gentity_s *ent;
    const WeaponDef *weapDef;
    float launchSpeed;

    iassert(self);
    iassert(self->ent);
    iassert(standPos);
    ent = self->ent;
    if ( !ent->s.weapon )
        Scr_Error(va("checkgrenadelaunch: invalid weapon for entity %d", ent->s.number), 0);
    weapDef = BG_GetWeaponDef(ent->s.weapon);
    if ( !weapDef )
        Scr_Error(va("checkgrenadelaunch: invalid weapon for entity %d", ent->s.number), 0);
    launchSpeed = (float)weapDef->iProjectileSpeed;
    if ( launchSpeed <= 0.0f )
        Scr_Error("checkgrenadelaunch: grenade launcher speed must be > 0", 0);
    self->bGrenadeTossValid = Actor_GrenadeLauncher_CheckPos(
        self,
        standPos,
        handOffset,
        self->vGrenadeTargetPos,
        self->vGrenadeTossPos,
        self->vGrenadeTossVel,
        launchSpeed);
    return self->bGrenadeTossValid != 0;
}
#endif

bool __fastcall Actor_Grenade_CheckToss(actor_s *self, const float *targetPos)
{
    sentient_s *enemy;
    float predictedPos[3];
    float velocity[3];
    float tossVel[3];

    iassert(self);
    iassert(self->sentient);
    iassert(targetPos);
    enemy = Actor_GetTargetSentient(self);
    if ( !enemy || bg_gravity->current.value <= 0.0f )
        return false;
    if ( self->pGrenade.isDefined() )
        return false;
    Sentient_GetOrigin(enemy, predictedPos);
    Sentient_GetVelocity(enemy, velocity);
    predictedPos[0] += velocity[0];
    predictedPos[1] += velocity[1];
    predictedPos[2] += velocity[2];
    self->vGrenadeTargetPos[0] = predictedPos[0];
    self->vGrenadeTargetPos[1] = predictedPos[1];
    self->vGrenadeTargetPos[2] = predictedPos[2];
    self->bGrenadeTargetValid = 1;
    return Actor_Grenade_CheckTossPos(self, predictedPos, tossVel);
}

bool __fastcall Actor_Grenade_IsSafeTarget(actor_s *self, gentity_s *target)
{
    float targetPos[3];

    iassert(self);
    if ( !target )
        return true;
    targetPos[0] = target->r.currentOrigin[0];
    targetPos[1] = target->r.currentOrigin[1];
    targetPos[2] = target->r.currentOrigin[2];
    return Actor_Grenade_IsPointSafe(self, targetPos);
}

bool __fastcall Actor_Grenade_IsPointSafe(actor_s *self, const float *point)
{
    gentity_s *grenadeEnt;
    float landPos[3];

    iassert(self);
    iassert(point);
    if ( !self->pGrenade.isDefined() )
        return true;
    grenadeEnt = self->pGrenade.ent();
    if ( !grenadeEnt )
        return true;
    MissileTrajectory(grenadeEnt, landPos);
    return Vec3Distance(landPos, point) > 128.0f;
}

bool __fastcall Actor_IsAwareOfGrenade(actor_s *self, gentity_s *grenade)
{
    float awareness;

    iassert(self);
    iassert(grenade);
    awareness = self->grenadeAwareness;
    if ( awareness <= 0.0f )
        awareness = 0.5f;
    return G_random() < awareness;
}

void __fastcall Actor_PredictGrenadeLandPos(gentity_s *grenade)
{
    iassert(grenade);
    MissileTrajectory(grenade, grenade->s.lerp.pos.trBase);
}

bool __fastcall Actor_Grenade_ShouldIgnore(actor_s *self, gentity_s *grenade)
{
    gentity_s *owner;

    iassert(self);
    iassert(grenade);
    owner = grenade->r.ownerNum.ent();
    if ( owner && owner->client && self->sentient && owner->sentient == self->sentient )
        return true;
    if ( self->eState[self->stateLevel] == AIS_GRENADE_RESPONSE )
        return true;
    return !Actor_IsAwareOfGrenade(self, grenade);
}

void __fastcall Actor_GrenadePing(actor_s *self, gentity_s *grenade)
{
    float actorPos[3];
    gentity_s *owner;

    iassert(self);
    iassert(grenade);
    if ( self->bThrowbackGrenades )
        return;
    if ( self->eState[self->stateLevel] == AIS_GRENADE_RESPONSE
        && self->eSubState[self->stateLevel] == STATE_GRENADE_THROWBACK )
    {
        return;
    }
    if ( self->pGrenade.isDefined() )
        return;
    if ( BG_GetWeaponDef(grenade->s.weapon)->offhandClass != OFFHAND_CLASS_FRAG_GRENADE
        && !Actor_IsAwareOfGrenade(self, grenade) )
    {
        return;
    }
    self->pGrenade.setEnt(grenade);
    owner = grenade->parent.ent();
    if ( owner && owner->sentient )
    {
        if ( owner->sentient->eTeam == Sentient_EnemyTeam(self->sentient->eTeam) )
        {
            Scr_AddEntity(grenade, SCRIPTINSTANCE_SERVER);
            Scr_Notify(self->ent, scr_const.grenade, 1u);
        }
    }
    Actor_PredictGrenadeLandPos(grenade);
    Sentient_GetOrigin(self->sentient, actorPos);
    if ( Actor_Grenade_ShouldIgnore(self, grenade) || Actor_Grenade_IsPointSafe(self, actorPos) )
        self->pGrenade.setEnt(nullptr);
}

void __fastcall Actor_DissociateGrenade(actor_s *self)
{
    iassert(self);
    if ( self->pGrenade.isDefined() )
        EntHandleDissociate(self->pGrenade.ent());
    self->pGrenade.setEnt(nullptr);
}

void __fastcall Actor_Grenade_Attach(actor_s *self, gentity_s *grenade)
{
    const WeaponDef *weapDef;
    const char *modelName;

    iassert(self);
    iassert(grenade);
    iassert(self->pGrenade.isDefined());
    weapDef = BG_GetWeaponDef(grenade->s.weapon);
    iassert(weapDef);
    modelName = XModelGetName(weapDef->worldModel[grenade->s.weaponModel]);
    G_EntDetach(self->ent, (char *)modelName, scr_const.tag_inhand);
    G_EntAttach(self->ent, (char *)modelName, scr_const.tag_inhand, 0);
    if ( self->vGrenadeTossVel[0] != 0.0f || self->vGrenadeTossVel[1] != 0.0f )
        Actor_FaceVector(&self->CodeOrient, self->vGrenadeTossVel);
    Actor_SetOrientMode(self, AI_ORIENT_DONT_CHANGE);
    grenade->r.linked = 0;
    grenade->s.lerp.eFlags |= 0x10u;
    grenade->parent.setEnt(self->ent);
}

void __fastcall Actor_Grenade_Detach(actor_s *self)
{
    gentity_s *grenade;
    const WeaponDef *weapDef;
    const char *modelName;

    iassert(self);
    if ( !self->pGrenade.isDefined() )
        return;
    grenade = self->pGrenade.ent();
    iassert(grenade);
    weapDef = BG_GetWeaponDef(grenade->s.weapon);
    modelName = XModelGetName(weapDef->worldModel[grenade->s.weaponModel]);
    G_EntDetach(self->ent, (char *)modelName, scr_const.tag_inhand);
    grenade->s.lerp.eFlags &= ~0x10u;
    grenade->parent.setEnt(0);
}

void __fastcall Actor_Grenade_DropIfHeld(actor_s *self)
{
    gentity_s *grenade;

    iassert(self);
    if ( !self->pGrenade.isDefined() )
        return;
    grenade = self->pGrenade.ent();
    if ( !grenade || grenade->r.ownerNum.entnum() != self->ent->s.number )
        return;
    if ( Actor_Grenade_InActorHands(grenade) )
    {
        Actor_Grenade_Detach(self);
        G_InitGrenadeEntity(self->ent, grenade);
        G_InitGrenadeMovement(grenade, grenade->r.currentOrigin, vec3_origin, 1, WEAPROTATE_GRENADE_ROTATE);
        SV_LinkEntity(grenade);
    }
    grenade->r.ownerNum.setEnt(0);
}

bool __fastcall Actor_Grenade_InActorHands(gentity_s *grenade)
{
    iassert(grenade);
    return grenade->parent.isDefined();
}

void __fastcall Actor_Grenade_DecideResponse(actor_s *self)
{
    gentity_s *grenade;

    iassert(self);
    iassert(self->pGrenade.isDefined());
    iassert(self->sentient);
    grenade = self->pGrenade.ent();
    if ( grenade->r.ownerNum.isDefined() )
    {
        Actor_Grenade_AttemptEscape(self, false);
        return;
    }
    if ( self->bThrowbackGrenades )
    {
        Actor_Grenade_AttemptEscape(self, false);
        return;
    }
    if ( grenade->s.lerp.apos.trBase[0] != 0.0f )
    {
        Actor_Grenade_AttemptEscape(self, false);
        return;
    }
    if ( !Actor_Grenade_AttemptReturn(self) )
        Actor_Grenade_AttemptEscape(self, false);
}

bool __fastcall Actor_Grenade_Start(actor_s *self, ai_state_t ePrevState)
{
    (void)ePrevState;
    iassert(self);
    if ( !self->pGrenade.isDefined() )
        return false;
    Actor_Grenade_DecideResponse(self);
    iassert(self->eSubState[self->stateLevel] >= STATE_GRENADE_FLEE);
    return true;
}

void __fastcall Actor_Grenade_Finish(actor_s *self, ai_state_t eNextState)
{
    (void)eNextState;
    iassert(self);
    Actor_Grenade_DropIfHeld(self);
    self->pGrenade.setEnt(nullptr);
}

void __fastcall Actor_Grenade_Suspend(actor_s *self, ai_state_t eNextState)
{
    Actor_Grenade_Finish(self, eNextState);
}

bool __fastcall Actor_Grenade_Resume(actor_s *self, ai_state_t ePrevState)
{
    (void)ePrevState;
    iassert(self);
    if ( !self->pGrenade.isDefined() )
        return false;
    return self->bThrowbackGrenades != 0;
}

void __fastcall Actor_Grenade_Cower(actor_s *self)
{
    iassert(self);
    Actor_SetOrientMode(self, AI_ORIENT_DONT_CHANGE);
    Actor_AnimStop(self, &g_animScriptTable[self->species]->grenade_cower);
}

void __fastcall Actor_Grenade_Combat(actor_s *self)
{
    iassert(self);
    Actor_SetOrientMode(self, AI_ORIENT_TO_ENEMY);
    Actor_SetAnimScript(
        self,
        &g_animScriptTable[self->species]->combat,
        0,
        AI_ANIM_USE_BOTH_DELTAS,
        AI_ANIM_FUNCTION_COMBAT);
}

void __fastcall Actor_Grenade_CoverAttack(actor_s *self)
{
    Actor_Grenade_Combat(self);
}

void __fastcall Actor_Grenade_TakeCover(actor_s *self)
{
    pathnode_t *coverNode;

    iassert(self);
    coverNode = Actor_Cover_FindCoverNode(self, Actor_GetTargetSentient(self));
    if ( coverNode && Actor_Cover_UseCoverNode(self, coverNode) )
        Actor_SetSubState(self, STATE_GRENADE_TAKECOVER);
    else
    {
        Actor_SetSubState(self, STATE_GRENADE_COWER);
        Actor_Grenade_Cower(self);
    }
}

void __fastcall Actor_Grenade_Flee(actor_s *self)
{
    iassert(self);
    if ( Actor_HasPath(self) )
    {
        if ( self->pGrenade.isDefined() && !self->pCloseEnt.isDefined() )
            self->pCloseEnt.setEnt(self->pGrenade.ent());
        Actor_SetOrientMode(self, AI_ORIENT_TO_ENEMY_OR_MOTION);
        Actor_TeamMoveBlockedClear(self);
        Actor_MoveAlongPathWithTeam(self, true, false, false);
    }
    else if ( Actor_Grenade_IsPointSafe(self, self->ent->r.currentOrigin) )
    {
        Actor_SetSubState(self, STATE_GRENADE_COMBAT);
        Actor_Grenade_Combat(self);
    }
    else
    {
        Actor_SetSubState(self, STATE_GRENADE_COWER);
        Actor_Grenade_Cower(self);
    }
}

bool __fastcall Actor_Grenade_ThrowBack(actor_s *self)
{
    iassert(self);
    if ( !self->bThrowbackGrenades || !self->pGrenade.isDefined() )
        return false;
    Actor_SetSubState(self, STATE_GRENADE_THROWBACK);
    Actor_SetAnimScript(
        self,
        &g_animScriptTable[self->species]->grenade_return_throw,
        0,
        AI_ANIM_USE_BOTH_DELTAS,
        AI_ANIM_FUNCTION_COMBAT);
    return Actor_IsAnimScriptAlive(self);
}

bool __fastcall Actor_Grenade_Acquire(actor_s *self)
{
    gentity_s *grenade;

    iassert(self);
    if ( !self->pGrenade.isDefined() )
        return false;
    grenade = self->pGrenade.ent();
    if ( !grenade )
        return false;
    Actor_Grenade_Attach(self, grenade);
    Actor_SetSubState(self, STATE_GRENADE_ACQUIRE);
    return Actor_IsAnimScriptAlive(self);
}

void __fastcall Actor_Grenade_ReevaluateResponse(actor_s *self)
{
    iassert(self);
    if ( self->pGrenade.isDefined() )
        Actor_Grenade_DecideResponse(self);
}

bool __fastcall Actor_Grenade_AttemptEscape(actor_s *self, bool forceRepath)
{
    iassert(self);
    if ( forceRepath )
        Actor_ClearPath(self);
    Actor_SetSubState(self, STATE_GRENADE_FLEE);
    Actor_Grenade_Flee(self);
    return true;
}

bool __fastcall Actor_Grenade_EscapePlane(
    actor_s *self,
    float *planeNormal,
    float *planePoint,
    gentity_s *grenade)
{
    iassert(self);
    iassert(planeNormal);
    iassert(planePoint);
    iassert(grenade);
    if ( !self->pGrenade.isDefined() || self->pGrenade.ent() != grenade )
        return false;
    return Actor_Grenade_AttemptEscape(self, true);
}

bool __fastcall Actor_Grenade_AttemptReturn(actor_s *self)
{
    iassert(self);
    if ( !self->bThrowbackGrenades )
        return false;
    return Actor_Grenade_ThrowBack(self);
}

bool __fastcall Actor_Grenade_CheckDesperateToss(actor_s *self)
{
    float targetPos[3];

    iassert(self);
    if ( !Actor_GetTargetEntity(self) )
        return false;
    Actor_GetTargetPosition(self, targetPos);
    return Actor_Grenade_CheckToss(self, targetPos);
}

bool __fastcall Actor_Grenade_AttemptReturnTo(actor_s *self, const float *returnPos)
{
    float tossVel[3];

    iassert(self);
    iassert(returnPos);
    if ( !self->bThrowbackGrenades )
        return false;
    if ( !Actor_Grenade_CheckTossPos(self, returnPos, tossVel) )
        return false;
    return Actor_Grenade_ThrowBack(self);
}

void __fastcall Actor_Grenade_GetPickupPos(gentity_s *grenade, float *pickupPos)
{
    iassert(grenade);
    iassert(pickupPos);
    pickupPos[0] = grenade->r.currentOrigin[0];
    pickupPos[1] = grenade->r.currentOrigin[1];
    pickupPos[2] = grenade->r.currentOrigin[2];
}

actor_think_result_t __fastcall Actor_Grenade_Think(actor_s *self)
{
    ai_substate_t subState;

    iassert(self);
    if ( !self->pGrenade.isDefined() || self->bThrowbackGrenades )
    {
        Actor_SetState(self, AIS_EXPOSED);
        return ACTOR_THINK_DONE;
    }
    Actor_PreThink(self);
    subState = self->eSubState[self->stateLevel];
    if ( subState == STATE_GRENADE_FLEE || subState == STATE_GRENADE_ACQUIRE )
    {
        if ( !self->sentient->pClaimedNode )
            Actor_Grenade_AttemptEscape(self, true);
    }
    switch ( subState )
    {
    case STATE_GRENADE_TAKECOVER:
        self->pszDebugInfo = "grenade_takecover";
        Actor_Grenade_TakeCover(self);
        break;
    case STATE_GRENADE_COWER:
        self->pszDebugInfo = "grenade_cower";
        Actor_Grenade_Cower(self);
        break;
    case STATE_GRENADE_COMBAT:
        self->pszDebugInfo = "grenade_combat";
        Actor_Grenade_Combat(self);
        break;
    case STATE_GRENADE_COVERATTACK:
        self->pszDebugInfo = "grenade_coverattack";
        Actor_Grenade_CoverAttack(self);
        break;
    case STATE_GRENADE_ACQUIRE:
        self->pszDebugInfo = "grenade_acquire";
        if ( Actor_Grenade_Acquire(self) )
            return ACTOR_THINK_REPEAT;
        break;
    case STATE_GRENADE_FLEE:
        self->pszDebugInfo = "grenade_flee";
        Actor_Grenade_Flee(self);
        break;
    default:
        self->pszDebugInfo = "grenade_throwback";
        if ( Actor_Grenade_ThrowBack(self) )
            return ACTOR_THINK_REPEAT;
        break;
    }
    Actor_PostThink(self);
    return ACTOR_THINK_REPEAT;
}
