#pragma once

void __cdecl Dtp_RegisterDvars();
int __cdecl Dtp_CalcSurfaceType(pmove_t *pm, pml_t *pml);
bool __cdecl Dtp_IsSliding(const playerState_s *ps);
void __cdecl Dtp_AdjustDamage(const playerState_s *ps, float fallHeight, int *damage);
void __cdecl Dtp_Interrupt(pmove_t *pm);
void __cdecl Dtp_CheckForEnd(pmove_t *pm);
void __cdecl Dtp_End(pmove_t *pm);
double __cdecl Dtp_ReduceFriction(pmove_t *pm, const playerState_s *ps);
void __cdecl Dtp_AirMove(playerState_s *ps, const pmove_t *pm);
bool __cdecl Dtp_CanMove(pmove_t *pm);
int __cdecl Dtp_Update(pmove_t *pm, pml_t *pml);
void __cdecl Dtp_Start(pmove_t *pm, pml_t *pml);
bool __cdecl Dtp_IsDtp(const pmove_t *pm, const playerState_s *ps);
