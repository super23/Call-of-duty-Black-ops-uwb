#pragma once

void __cdecl StatMon_Warning(int type, int duration, char *materialName);
void __cdecl StatMon_GetStatsArray(const statmonitor_s **array, int *count);
void __cdecl StatMon_Reset();
