#pragma once

extern bool cin_skippable;

void __cdecl SCR_DrawCinematic();
void __cdecl SCR_StopCinematic();

#ifdef KISAK_SP
void __cdecl CL_BeginSPStartupIntroChain();
void __cdecl CL_FinishSPStartupIntro(int localClientNum);
bool __cdecl CL_IsSPStartupIntroActive();
#endif
