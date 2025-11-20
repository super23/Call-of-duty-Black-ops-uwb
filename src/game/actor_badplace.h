#pragma once

void __cdecl Path_UpdateBadPlaceCount(badplace_t *place, int delta);
void __cdecl Path_UpdateBadPlaces();
void __cdecl Path_FreeBadPlace(int index);
int __cdecl Path_FindBadPlace(unsigned int name);
void __cdecl Path_MakeBadPlaceEx(unsigned int name, int duration, int teamflags, int type, badplace_parms_t *parms);
badplace_t *__cdecl Path_AllocBadPlace(unsigned int name, int duration);
void __cdecl Path_DrawBadPlaces();
void __cdecl Path_DrawBadPlace(badplace_t *place);
void __cdecl Path_InitBadPlaces();
void __cdecl Path_ShutdownBadPlaces();
bool __cdecl Actor_IsInAnyBadPlace(actor_s *self);
actor_s *Actor_BadPlace_UpdateFleeingActors();
char __fastcall Actor_BadPlace_Flee_Start(actor_s *self, ai_state_t ePrevState);
char __cdecl Actor_BadPlace_AttemptEscape(actor_s *self);
double __cdecl Actor_BadPlace_GetMaximumFleeRadius();
int __cdecl Actor_BadPlace_FindSafeNodeOutsideBadPlace(actor_s *self, pathsort_t *potentialNodes, float maxFleeDist);
int __cdecl Path_IsCoverNode(const pathnode_t *node);
char __cdecl Actor_BadPlace_HasPotentialNodeDuplicates(
        pathsort_t *potentialNodes,
        int potentialNodeCount,
        pathnode_t *checkNode);
bool __cdecl Actor_BadPlace_IsNodeInAnyBadPlace(pathnode_t *node);
pathnode_t *__cdecl Actor_BadPlace_FindSafeNodeAlongPath(actor_s *self);
int __fastcall Actor_BadPlace_Flee_Think(actor_s *self);
void Actor_BadPlace_Flee_Finish();
