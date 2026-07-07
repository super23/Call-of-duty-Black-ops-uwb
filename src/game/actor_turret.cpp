#include "actor_turret.h"

#ifdef KISAK_SP
#include "actor_script_cmd.h"
#include "turret.h"
#include <game/g_spawn_sp.h>
#include <game/g_utils_sp.h>
#include <clientscript/cscr_vm.h>
#include <clientscript/scr_const.h>
#include <universal/assertive.h>

// Decomp: CoDSP_rdBlackOps.map.c (Actor_IsUsingTurret)
bool __cdecl Actor_IsUsingTurret(actor_s *self)
{
    if ( !self || !self->pTurret )
        return false;
    if ( !self->pTurret->r.ownerNum.isDefined() )
        return false;
    return self->pTurret->r.ownerNum.ent() == self->ent;
}

// Decomp: CoDSP_rdBlackOps.map.c (turret_canuse)
bool __cdecl turret_canuse(actor_s *actor, gentity_s *pTurret)
{
    if ( !actor || !actor->ent || !pTurret || !pTurret->pTurretInfo )
        return false;

    if ( Actor_IsUsingTurret(actor) )
        return actor->pTurret == pTurret;

    if ( pTurret->active )
        return false;

    return true;
}

// Decomp: CoDSP_rdBlackOps.map.c (Actor_UseTurret)
bool __cdecl Actor_UseTurret(actor_s *self, gentity_s *pTurret)
{
    if ( turret_canuse(self, pTurret) )
    {
        self->pTurret = pTurret;
        pTurret->flags |= 0x4000000u;
        return true;
    }

    self->pTurret = 0;
    return false;
}

// Decomp: CoDSP_rdBlackOps.map.c (Actor_DetachTurret)
void __cdecl Actor_DetachTurret(actor_s *self)
{
    gentity_s *pTurret;
    TurretInfo *turretInfo;
    tagInfo_s *tagInfo;

    iassert(self);
    iassert(self->sentient);
    iassert(Actor_IsUsingTurret(self));

    pTurret = self->pTurret;
    iassert(pTurret);
    iassert(pTurret->r.inuse);
    iassert(pTurret->active);
    iassert(pTurret->r.ownerNum.isDefined() && pTurret->r.ownerNum.ent() == self->ent);

    G_DeactivateTurret(pTurret);
    pTurret->r.ownerNum.setEnt(0);

    if ( G_EntIsLinkedTo(self->ent, pTurret) )
    {
        tagInfo = self->ent->tagInfo;
        if ( tagInfo && tagInfo->parent )
            G_EntLinkTo(self->ent, tagInfo->parent, tagInfo->name);
        else
            G_EntUnlink(self->ent);
    }

    turretInfo = pTurret->pTurretInfo;
    iassert(turretInfo);
    turretInfo->flags &= ~0x200u;
}

// Decomp: CoDSP_rdBlackOps.map.c (Actor_StopUseTurret)
void __cdecl Actor_StopUseTurret(actor_s *self)
{
    if ( !self )
        return;

    if ( Actor_IsUsingTurret(self) )
        Actor_DetachTurret(self);

    self->pTurret = 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (ActorCmd_StopUseTurret)
void __cdecl ActorCmd_StopUseTurret(scr_entref_t entref)
{
    Actor_StopUseTurret(Actor_Get(entref));
}

// Decomp: CoDSP_rdBlackOps.map.c (ActorCmd_UseTurret)
void __cdecl ActorCmd_UseTurret(scr_entref_t entref)
{
    actor_s *self;
    gentity_s *turretEnt;

    self = Actor_Get(entref);
    turretEnt = Scr_GetEntity(0);
    if ( !turretEnt->pTurretInfo )
        Scr_ParamError(0, "can only use a turret", SCRIPTINSTANCE_SERVER);
    Scr_AddBool(Actor_UseTurret(self, turretEnt), SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (ActorCmd_CanUseTurret)
void __cdecl ActorCmd_CanUseTurret(scr_entref_t entref)
{
    actor_s *self;
    gentity_s *turretEnt;

    self = Actor_Get(entref);
    turretEnt = Scr_GetEntity(0);
    if ( !turretEnt->pTurretInfo )
        Scr_ParamError(0, "can only use a turret", SCRIPTINSTANCE_SERVER);
    Scr_AddBool(turret_canuse(self, turretEnt), SCRIPTINSTANCE_SERVER);
}

#else

#include "actor_script_cmd.h"

bool __cdecl Actor_IsUsingTurret(actor_s *self)
{
    return false;
}

void __cdecl Actor_StopUseTurret(actor_s *self)
{
    (void)self;
}

void __cdecl Actor_DetachTurret(actor_s *self)
{
    (void)self;
}

bool __cdecl Actor_UseTurret(actor_s *self, gentity_s *pTurret)
{
    (void)self;
    (void)pTurret;
    return false;
}

bool __cdecl turret_canuse(actor_s *actor, gentity_s *pTurret)
{
    (void)actor;
    (void)pTurret;
    return false;
}

void __cdecl ActorCmd_StopUseTurret(scr_entref_t entref)
{
    (void)entref;
}

void __cdecl ActorCmd_UseTurret(scr_entref_t entref)
{
    (void)entref;
}

void __cdecl ActorCmd_CanUseTurret(scr_entref_t entref)
{
    (void)entref;
}

#endif
