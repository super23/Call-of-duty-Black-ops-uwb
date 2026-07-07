#pragma once

struct actor_s;

void __cdecl G_InitActorProneInfo(actor_s *actor);

#ifdef KISAK_SP
void __cdecl G_ActorEnterProne(actor_s *actor, unsigned int iTransTime);
void __cdecl G_ActorExitProne(actor_s *actor, unsigned int iTransTime);
void __cdecl Actor_UpdateProneInformation(actor_s *self, int bDoProneCheck);
#endif
