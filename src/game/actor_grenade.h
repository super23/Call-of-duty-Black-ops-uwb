#pragma once

#include "actor.h"

// Decomp: CoDSPBlackOps.map.c — actor_grenade.cpp

void __cdecl SP_info_grenade_hint(gentity_s *self);
void __fastcall Actor_Grenade_GetTossFromPosition(actor_s *self, float *tossPos);
bool __fastcall Actor_Grenade_GetTossPositions(actor_s *self, float (*tossPositions)[3], int *count);
void __cdecl Actor_Grenade_GetTossPositionsPos(const float *from, const float *target, float *landPos, unsigned int weaponIndex);
bool __fastcall Actor_Grenade_GetTossPositionsFromHints(actor_s *self, float (*tossPositions)[3], int *count);
bool __fastcall Actor_Grenade_IsValidTrajectory(
    actor_s *self,
    const float *from,
    const float *vel,
    const float *land);
bool __fastcall Actor_Grenade_CheckMaximumEnergyToss(
    actor_s *self,
    const float *from,
    const float *land,
    float *tossVel,
    bool lowArc);
bool __fastcall Actor_Grenade_CheckInfiniteEnergyToss(
    actor_s *self,
    const float *from,
    const float *land,
    float *tossVel);
bool __fastcall Actor_Grenade_CheckMinimumEnergyToss(
    actor_s *self,
    const float *from,
    const float *land,
    float *tossVel);
bool __fastcall Actor_Grenade_CheckGrenadeHintToss(
    actor_s *self,
    const float *from,
    const float *land,
    float *tossVel);
bool __fastcall Actor_Grenade_CheckToss(actor_s *self, const float *targetPos);
bool __fastcall Actor_Grenade_CheckTossPos(actor_s *self, const float *targetPos, float *tossVel);
bool __fastcall Actor_Grenade_CheckTossPosFrom(
    actor_s *self,
    const float *tossFrom,
    const float *targetPos,
    float *tossVel);
#ifdef KISAK_SP
bool __fastcall Actor_CheckGrenadeLaunch(actor_s *self, const float *standPos, const float *handOffset);
bool __fastcall Actor_GrenadeLauncher_CheckPos(
    actor_s *self,
    const float *standPos,
    const float *handOffset,
    const float *targetPos,
    float *posOut,
    float *velOut,
    float launchSpeed);
#endif
bool __fastcall Actor_Grenade_IsSafeTarget(actor_s *self, gentity_s *target);
bool __fastcall Actor_Grenade_IsPointSafe(actor_s *self, const float *point);
bool __fastcall Actor_IsAwareOfGrenade(actor_s *self, gentity_s *grenade);
void __fastcall Actor_GrenadePing(actor_s *self, gentity_s *grenade);
void __fastcall Actor_PredictGrenadeLandPos(gentity_s *grenade);
bool __fastcall Actor_Grenade_ShouldIgnore(actor_s *self, gentity_s *grenade);
void __fastcall Actor_Grenade_ReevaluateResponse(actor_s *self);
bool __fastcall Actor_Grenade_AttemptEscape(actor_s *self, bool forceRepath);
bool __fastcall Actor_Grenade_EscapePlane(
    actor_s *self,
    float *planeNormal,
    float *planePoint,
    gentity_s *grenade);
bool __fastcall Actor_Grenade_AttemptReturn(actor_s *self);
bool __fastcall Actor_Grenade_CheckDesperateToss(actor_s *self);
bool __fastcall Actor_Grenade_AttemptReturnTo(actor_s *self, const float *returnPos);
void __fastcall Actor_Grenade_GetPickupPos(gentity_s *grenade, float *pickupPos);
void __fastcall Actor_DissociateGrenade(actor_s *self);
void __fastcall Actor_Grenade_Attach(actor_s *self, gentity_s *grenade);
void __fastcall Actor_Grenade_Detach(actor_s *self);
void __fastcall Actor_Grenade_DropIfHeld(actor_s *self);
bool __fastcall Actor_Grenade_InActorHands(gentity_s *grenade);
bool __fastcall Actor_Grenade_Start(actor_s *self, ai_state_t ePrevState);
void __fastcall Actor_Grenade_DecideResponse(actor_s *self);
void __fastcall Actor_Grenade_Finish(actor_s *self, ai_state_t eNextState);
void __fastcall Actor_Grenade_Suspend(actor_s *self, ai_state_t eNextState);
bool __fastcall Actor_Grenade_Resume(actor_s *self, ai_state_t ePrevState);
actor_think_result_t __fastcall Actor_Grenade_Think(actor_s *self);
void __fastcall Actor_Grenade_Cower(actor_s *self);
void __fastcall Actor_Grenade_Combat(actor_s *self);
void __fastcall Actor_Grenade_CoverAttack(actor_s *self);
void __fastcall Actor_Grenade_Flee(actor_s *self);
void __fastcall Actor_Grenade_TakeCover(actor_s *self);
bool __fastcall Actor_Grenade_ThrowBack(actor_s *self);
bool __fastcall Actor_Grenade_Acquire(actor_s *self);
