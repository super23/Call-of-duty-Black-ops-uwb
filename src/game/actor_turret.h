#pragma once

struct actor_s;
struct gentity_s;

bool __cdecl Actor_IsUsingTurret(actor_s *self);
void __cdecl Actor_StopUseTurret(actor_s *self);
void __cdecl Actor_DetachTurret(actor_s *self);
bool __cdecl Actor_UseTurret(actor_s *self, gentity_s *pTurret);
bool __cdecl turret_canuse(actor_s *actor, gentity_s *pTurret);
