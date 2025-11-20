#pragma once

void __cdecl Phys_InitSoundAliases();
unsigned int __cdecl Phys_GetSoundAliasIndex(Phys_SurfaceType type1, Phys_SurfaceType type2);
void __cdecl Phys_EffectsInit();
void __cdecl Phys_ReEvalPriority(PhysObjUserData *userData);
void __cdecl Phys_EffectsProcess();
unsigned int __thiscall rigid_body::is_user_rigid_body(rigid_body *this);
Phys_SurfaceType __cdecl Phys_RemmapSurfaceType(int stype);
unsigned int __cdecl Phys_GetSoundAliasForImpact(Phys_SurfaceType type1, Phys_SurfaceType type2);
bool __cdecl Phys_CanPlayEffect(PhysObjUserData *userData);
