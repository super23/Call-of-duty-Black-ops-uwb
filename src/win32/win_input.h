#pragma once

void __cdecl IN_StartupGamepads();
void __cdecl IN_RecenterMouse();
bool __cdecl IN_IsForegroundWindow();
void __cdecl IN_ActivateMouse(int force);
void __cdecl IN_DeactivateMouse();
void IN_DeactivateWin32Mouse();
void __cdecl IN_StartupMouse();
void __cdecl IN_MouseEvent(int mstate);
void __cdecl IN_SetCursorPos(unsigned int x, unsigned int y);
void __cdecl IN_ShowSystemCursor(bool show);
void __cdecl IN_Startup();
void __cdecl IN_Shutdown();
void __cdecl IN_Init();
void __cdecl IN_Activate(int active);
void __cdecl IN_Frame();
void IN_GamepadsMove();
void IN_MouseMove();
void __cdecl IN_ClampMouseMove(tagPOINT *curPos);
