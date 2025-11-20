#pragma once

void __cdecl G_ProcessIPBans();
void __cdecl AddIP(char *str);
int __cdecl StringToFilter(char *s, ipFilter_s *f);
void UpdateIPBans();
void __cdecl Svcmd_AddIP_f();
void __cdecl Svcmd_RemoveIP_f();
void __cdecl Svcmd_EntityList_f();
int __cdecl ConsoleCommand();
