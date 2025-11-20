#pragma once

void __cdecl FixWindowsDesktop();
int __stdcall HideWindowCallback(HWND__ *hwnd, int lParam);
int __cdecl Assert_DoStackTrace(char *msg, int nIgnore, int type, int *context);
int __cdecl LoadMapFiles(char *msg);
void __cdecl LoadMapFilesForDir(const char *dir);
char __cdecl ParseMapFile(_iobuf *fp, unsigned int baseAddress, const char *mapName);
void __cdecl ParseError(const char *msg);
char __cdecl ReadLine(_iobuf *fp);
char __cdecl SkipLines(int lineCount, _iobuf *fp);
HINSTANCE__ *__cdecl GetModuleBase(char *name);
void __cdecl Assert_ResetAddressInfo();
void __cdecl Assert_SetMonkeyCallbackHandler(void (__cdecl *AssertCallbackFunc)(const char *));
bool Assert_MyHandler(char *filename, int line, int type, const char *fmt, ...);
bool CopyMessageToClipboard();
char __cdecl AssertNotify(int type, AssertOccurance occurance);
void __cdecl Assert_BuildAssertMessage(char *expr, char *filename, int line, int type, int skipLevels, char *message);
void __cdecl RefreshQuitOnErrorCondition();
bool __cdecl QuitOnError();
