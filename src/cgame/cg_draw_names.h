#pragma once

void __cdecl CG_ClearOverheadFade();
void __cdecl CG_ClearPlayerDetails();
void __cdecl CG_InitPlayerDetails(int localClientNum, int targetClientNum);
void __cdecl CG_DrawOverheadNames(int localClientNum, const centity_s *cent, float alpha, int mode);
char __cdecl CG_CalcNamePosition(int localClientNum, float *origin, float *xOut, float *yOut);
void __cdecl CG_GetViewAxisProjections(const refdef_s *refdef, const float *worldPoint, float *projections);
void __cdecl CG_CalcVehicleNamePosition(int localClientNum, const centity_s *cent, float (*origin)[3]);
double __cdecl CG_FadeCrosshairNameAlpha(int time, int startMsec, int lastMsec, int fadeInMsec, int fadeOutMsec);
char __cdecl CG_CanSeeFriendlyHead(int localClientNum, const centity_s *cent);
bool __cdecl CG_TraceEntIsEntity(int traceEnt, const centity_s *cent);
void __cdecl CG_ScanForCrosshairEntity(int localClientNum);
void __cdecl CG_ScanForCrosshairEntityInternal(int localClientNum);
bool __cdecl CG_CheckIfDrivingRemoteControlVehicle(int localClientNum, unsigned __int16 EntId);
void __cdecl CG_DrawNames(int localClientNum);
int __cdecl compareEntityDist(float *v1, float *v2);
int __cdecl CG_GetTeamIndicator();
bool __cdecl ShouldDrawCrosshairNames(const cg_s *cgameGlob);
void __cdecl CG_DrawCrosshairNames(int localClientNum);
void __cdecl CG_AddDrawName(int localClientNum, int entnum, float alpha, int teamIndicator);
char __cdecl CG_IsValidCrosshairEntity(int localClientNum, const cg_s *cgameGlob, const centity_s *cent, int entNum);
bool __cdecl ShouldDrawFriendlyName(int localClientNum, const centity_s *cent, int entnum, int team);
int __cdecl GetVehicleDriverEntNum(const cg_s *cgameGlob, const centity_s *cent, int entnum);
void __cdecl CG_DrawFriendlyNames(int localClientNum);
void __cdecl CG_DrawVisibleNames(int localClientNum);
