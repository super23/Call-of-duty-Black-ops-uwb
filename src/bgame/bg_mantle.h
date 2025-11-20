#pragma once

void __cdecl Mantle_RegisterDvars();
void __cdecl Mantle_CreateAnims(void *(__cdecl *xanimAlloc)(int));
int __cdecl Trans_GetUpIndex(unsigned int transIndex);
int __cdecl Trans_GetOverIndex(unsigned int transIndex);
double __cdecl Trans_GetHeight(unsigned int transIndex);
void __cdecl Mantle_ShutdownAnims();
void __cdecl Mantle_Check(pmove_t *pm, pml_t *pml);
void __cdecl Mantle_DebugPrint(const char *msg);
void __cdecl Mount_CheckProne(pmove_t *pm);
char __cdecl Mantle_CheckLedge(pmove_t *pm, pml_t *pml, MantleResults *mresults, float height);
void __cdecl Mantle_CalcEndPos(pmove_t *pm, MantleResults *mresults);
void __cdecl Mantle_Start(pmove_t *pm, playerState_s *ps, MantleResults *mresults);
int __cdecl Mantle_GetUpLength(const MantleState *mstate);
int __cdecl Mantle_GetOverLength(const MantleState *mstate);
void __cdecl Mantle_GetAnimDelta(MantleState *mstate, int time, float *delta);
int __cdecl Mantle_FindTransition(float curHeight, float goalHeight);
void __cdecl Mantle_SetHaveWeapon(MantleState *mstate, const playerState_s *ps);
// local variable allocation has failed, the output may be wrong!
void  Mount_CheckLedge(cStaticModel_s *a1@<ebp>, pmove_t *pm, pml_t *pml, MantleResults *mresults);
char  Mantle_FindMantleSurface@<al>(int a1@<ebp>, pmove_t *pm, pml_t *pml, trace_t *trace, float *mantleDir);
void __cdecl Mantle_Move(pmove_t *pm, playerState_s *ps, pml_t *pml);
int __cdecl Mantle_GetAnim(MantleState *mstate);
void __cdecl Mantle_CapView(playerState_s *ps);
void __cdecl Mantle_ClearHint(playerState_s *ps);
bool __cdecl Mantle_IsWeaponInactive(const playerState_s *ps);
bool __cdecl Mantle_IsWeaponEquipped(const playerState_s *ps);
bool __cdecl Mantle_DoAnim(const playerState_s *ps);
int __cdecl Mantle_GetDuration(const playerState_s *ps);
double __cdecl Mantle_GetMantleFrac(const playerState_s *ps);
bool __cdecl Mount_CanPlayerDeployTurret(playerState_s *ps);
int __cdecl BG_GetAmmoInClip(const playerState_s *ps, unsigned int weaponIndex);
int __cdecl BG_GetAmmoInClipForWeaponDef(const playerState_s *ps, const WeaponVariantDef *weapVarDef);
