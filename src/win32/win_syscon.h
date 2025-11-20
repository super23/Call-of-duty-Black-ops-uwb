#pragma once

LRESULT __stdcall InputLineWndProc(HWND__ *hWnd, UINT uMsg, HWND__ *wParam, LPARAM lParam);
void __cdecl Sys_CreateConsole(HINSTANCE__ *hInstance);
LRESULT __stdcall ConWndProc(HWND__ *hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
char *__cdecl Conbuf_CleanText(const char *source, char *target, int sizeofTarget);
void __cdecl Sys_DestroyConsole();
void __cdecl Sys_ShowConsole();
char *__cdecl Sys_ConsoleInput();
void __cdecl Conbuf_AppendText(char *pMsg);
void __cdecl Conbuf_AppendTextInMainThread(char *msg);
void __cdecl Sys_SetErrorText(const char *buf);
