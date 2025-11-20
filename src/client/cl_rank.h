#pragma once

void __cdecl CL_BuildRankXPTable();
int __cdecl CL_GetMaxXP();
int __cdecl CL_GetMaxPrestige();
int __cdecl CL_GetRankForXp(int xp);
const char *__cdecl CL_GetRankData(int rank, rankTableColumns_t column);
void __cdecl CL_GetRankIcon(int rank, int prestige, Material **handle);
const char *__cdecl CL_GetRankIconName(int rank, int prestige);
