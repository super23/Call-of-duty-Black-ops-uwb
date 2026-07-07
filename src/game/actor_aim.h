#pragma once
#include "actor.h"
#include <bgame/bg_weapons.h>

void __cdecl Actor_FillWeaponParms(actor_s *self, weaponParms *wp);
void __cdecl Actor_InitWeaponParms(actor_s *self, unsigned int weapon, weaponParms *wp);
gentity_s *__cdecl Actor_Melee(actor_s *self, const float *direction);
void __fastcall Actor_StopShoot(actor_s *self);
void __fastcall Actor_Shoot(actor_s *self, const float *optionalTarget, int addEvents, float accuracyMod);
#ifdef KISAK_SP
// Decomp: CoDSP_rdBlackOps.map.c (Actor_GetPlayerSightAccuracy ~82521588)
// Map: CoDSP_rd.map VA 0x82521588 (actor_aim.obj)
float __fastcall Actor_GetPlayerSightAccuracy(actor_s *self, sentient_s *targetSentient);
void __fastcall Actor_ShootBlank(actor_s *self);
#endif
