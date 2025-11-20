#include "statmonitor.h"

void __cdecl StatMon_Warning(int type, int duration, char *materialName)
{
  if ( com_statmon->current.enabled && !G_ExitAfterToolComplete() )
  {
    if ( (unsigned int)type >= 0xE
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\statmonitor.cpp",
            42,
            0,
            "type doesn't index ARRAY_COUNT( stats )\n\t%i not in [0, %i)",
            type,
            14) )
    {
      __debugbreak();
    }
    Sys_EnterCriticalSection(CRITSECT_STATMON);
    stats[type].endtime = duration + Sys_Milliseconds();
    if ( !stats[type].material && cls.rendererStarted )
      stats[type].material = Material_RegisterHandle(materialName, 1);
    if ( type >= statCount )
      statCount = type + 1;
    Sys_LeaveCriticalSection(CRITSECT_STATMON);
  }
}

void __cdecl StatMon_GetStatsArray(const statmonitor_s **array, int *count)
{
  *array = stats;
  *count = statCount;
}

void __cdecl StatMon_Reset()
{
  memset((unsigned __int8 *)stats, 0, sizeof(stats));
  statCount = 0;
}

