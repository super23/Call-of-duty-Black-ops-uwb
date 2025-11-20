#pragma once

void __cdecl CG_PerturbCamera(cg_s *cgameGlob);
int __cdecl CG_DrawShellShockSavedScreenBlendBlurred(
        int localClientNum,
        const shellshock_parms_t *parms,
        int start,
        int duration);
void __cdecl SaveScreenToBuffer(int localClientNum);
int __cdecl CG_DrawShellShockSavedScreenBlendFlashed(
        int localClientNum,
        const shellshock_parms_t *parms,
        int start,
        int duration);
double __cdecl BlendSmooth(float percent);
void __cdecl EndShellShock(int localClientNum);
void __cdecl EndShellShockSound(int localClientNum);
void __cdecl EndShellShockLookControl(int localClientNum);
void __cdecl EndShellShockCamera(int localClientNum);
void __cdecl EndShellShockScreen(int localClientNum);
void __cdecl CG_UpdateShellShock(int localClientNum, const shellshock_parms_t *parms, int start, int duration);
void __cdecl UpdateShellShockSound(int localClientNum, const shellshock_parms_t *parms, int time, int duration);
void __cdecl UpdateShellShockLookControl(int localClientNum, const shellshock_parms_t *parms, int time, int duration);
void __cdecl UpdateShellShockCamera(int localClientNum, const shellshock_parms_t *parms, int time, int duration);
double __cdecl CubicInterpolate(float t, float x0, float x1, float x2, float x3);
void __cdecl UpdateShellShockVisionSet(int localClientNum, const shellshock_parms_t *parms, int time, int duration);
void __cdecl CG_StartShellShock(cg_s *cgameGlob, const shellshock_parms_t *parms, int start, int duration);
bool __cdecl CG_Flashbanged(int localClientNum);
bool __cdecl CG_Flared(int localClientNum);
