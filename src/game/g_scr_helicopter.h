#pragma once

void __cdecl CMD_Heli_FreeHelicopter(scr_entref_t entref);
void __cdecl CMD_Heli_SetDamageStage(scr_entref_t entref);
void __cdecl CMD_Heli_SetHeliHeightLock(scr_entref_t entref);
void __cdecl CMD_Heli_IsInsideHeliHeightLock(scr_entref_t entref);
void (__cdecl *__cdecl Helicopter_GetMethod(const char **pName))(scr_entref_t);
void __cdecl G_SpawnHelicopter(gentity_s *ent, gentity_s *owner, char *vehicleInfoName, char *modelName);
void __cdecl Heli_InitFirstThink(gentity_s *pSelf);
void __cdecl Helicopter_Pain(
        gentity_s *pSelf,
        gentity_s *pAttacker,
        int damage,
        const float *point,
        int mod,
        const float *dir);
void __cdecl Helicopter_Die(
        gentity_s *pSelf,
        gentity_s *pInflictor,
        gentity_s *pAttacker,
        int damage,
        int mod,
        int weapon,
        const float *dir);
void __cdecl Helicopter_Controller(const gentity_s *pSelf, int *partBits);
void __cdecl Helicopter_Think(gentity_s *ent);
