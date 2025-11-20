#pragma once

void __cdecl CL_GetClientState(int localClientNum, uiClientState_s *state);
bool __cdecl CL_AllowPopup(int localClientNum);
void __cdecl Key_KeynumToStringBuf(int keynum, char *buf, int buflen);
int __cdecl CL_GetClientName(int localClientNum, int index, char *buf, int size, bool addClanName);
int __cdecl CL_GetClientClanName(int localClientNum, int index, char *buf, int size);
int __cdecl CL_ShutdownUI();
void __cdecl CL_InitUI();
