#pragma once

void __cdecl Helicopter_RegisterDvars();
void __cdecl HELI_CancelAIMove(gentity_s *ent);
void __cdecl VEH_CheckForPredictedCrash(gentity_s *ent);
void __cdecl VEH_CheckForCrash(gentity_s *ent, float *newPosition);
char __cdecl VEH_HandleHeliLockHeight(gentity_s *ent);
void __cdecl VEH_UpdateClientChopper(gentity_s *ent);
void __cdecl HELI_CalcAccel(gentity_s *ent, char *move, float *bodyAccel, float *rotAccel, float *viewangles);
void __cdecl HELI_CmdScale(char *move, float *outFracs);
void __cdecl HELI_UpdateJitter(VehicleJitter *jitter);
void __cdecl HELI_SoftenCollisions(gentity_s *ent, float *worldAccel);
bool __cdecl VEH_TestSlideMove(gentity_s *ent, float *outPos);
double __cdecl Vehicle_GetFakeLift(float *planeForward, float *planeUp);
double __cdecl Vehicle_AdjustRollForInputDeadZone(float rollPercent);
void __cdecl Vehicle_AddRotate(vehicle_physic_t *phys, float fRoll, float fPitch, float fYaw);
void __cdecl VEH_UpdateClientPlane(gentity_s *ent);
