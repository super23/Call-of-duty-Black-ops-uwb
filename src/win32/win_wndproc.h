#pragma once

void __cdecl Sys_SetBlockSystemHotkeys(int block);
LRESULT __stdcall LowLevelKeyboardProc(int nCode, WPARAM wParam, unsigned int *lParam);
bool __cdecl Sys_AllowVidRestart();
void __cdecl Sys_UpdateHotkeyBlock();
int __stdcall MainWndProc(HWND__ *hWnd, UINT uMsg, signed int wParam, LPARAM lParam);
void __cdecl VID_AppActivate(unsigned int activeState, int minimize);
unsigned __int8 __cdecl MapKey(int key, unsigned int wParam);
unsigned int __cdecl AdustKeyForNumericKeypad(unsigned int key, unsigned int wParam, unsigned int extended);
bool __cdecl IsNumLockAffectedVK(unsigned int wParam);
