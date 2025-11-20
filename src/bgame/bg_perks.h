#pragma once

unsigned int __cdecl BG_GetPerkIndexForName(const char *perkName);
const char *__cdecl BG_GetPerkNameForIndex(unsigned int perkIndex);
void __cdecl Perks_RegisterDvars();
