#pragma once
#include <clientscript/cscr_variable.h>
#include <bgame/bg_public.h>
#include <qcommon/common.h>

// Helicopter script entities (ET_HELICOPTER) — handlers in g_scr_helicopter.cpp.
// Physics/tuning dvars live in g_helicopter1.cpp (Helicopter_RegisterDvars).
// Height-lock volumes are registered in g_vehicle_path.cpp (heli_height_lock_patches).

void CMD_Heli_FreeHelicopter(scr_entref_t entref);
void CMD_Heli_SetDamageStage(scr_entref_t entref);
void CMD_Heli_SetHeliHeightLock(scr_entref_t entref);
void CMD_Heli_IsInsideHeliHeightLock(scr_entref_t entref);
void (*Helicopter_GetMethod(const char **pName))(scr_entref_t);
void G_SpawnHelicopter(gentity_s *ent, gentity_s *owner, char *vehicleInfoName, char *modelName);
void Heli_InitFirstThink(gentity_s *heliEnt);

void Helicopter_Pain(
    gentity_s *heliEnt,
    gentity_s *pAttacker,
    int damage,
    const float *point,
    const int mod,
    const float *dir,
    const hitLocation_t __formal,
    const int __formal2);

void Helicopter_Die(
    gentity_s *heliEnt,
    gentity_s *pInflictor,
    gentity_s *pAttacker,
    int damage,
    int mod,
    const int weapon,
    const float *dir,
    const hitLocation_t __formal,
    int __formal2);

void Helicopter_Controller(const gentity_s *heliEnt, int *partBits);
void Helicopter_Think(gentity_s *heliEnt);

extern heli_height_lock_patches_t heli_height_lock_patches[32];
