#pragma once

void __cdecl R_RegisterSunDvars();
void __cdecl R_SetSunFromDvars(sunflare_t *sun);
void __cdecl R_LoadSunThroughDvars(const char *sunName, sunflare_t *sun);
unsigned int __cdecl R_GetSunDvarCount();
void __cdecl R_Cmd_LoadSun();
void __cdecl R_Cmd_SaveSun();
void __cdecl R_SaveSunFromDvars(const char *sunName);
void __cdecl R_FlushSun();
