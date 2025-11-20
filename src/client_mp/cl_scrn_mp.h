#pragma once

void __cdecl SCR_DrawSmallStringExt(int x, int y, char *string, const float *setColor);
void __cdecl SCR_Init();
void __cdecl CL_DrawScreen(int localClientNum);
double __cdecl CL_GetMenuBlurRadius(int localClientNum);
void __cdecl CL_UpdateUIVisibilityBits(int localClientNum);
void  SCR_UpdateScreen(int a1@<esi>);
void  SCR_UpdateFrame(int a1@<esi>);
int  CL_CGameRendering@<eax>(int a1@<esi>, int localClientNum);
int __cdecl Window_GetDynamicFlags(int contextIndex, const windowDef_t *w);
int __cdecl CL_GetDemoType();
void __cdecl SCR_DrawScreenField(int localClientNum, int refreshedUI);
void SCR_DrawDemoRecording();
void SCR_ClearScreen();
void SCR_DrawLoadingAndLogo();
char __cdecl SCR_ShouldSkipUpdateScreen();
void __cdecl SCR_UpdateLoadScreen();
void  CL_CubemapShot_f(int a1@<esi>);
void CL_CubemapShotUsage();
void  CL_TakeHiResShot(int a1@<esi>);
void  HiResScreenshot(int a1@<esi>, int tiles);
void  CL_TakeHiResShot2(int a1@<esi>);
void  CL_TakeHiResShot3(int a1@<esi>);
void  CL_TakeHiResShot4(int a1@<esi>);
void  CL_TakeHiResShot8(int a1@<esi>);
