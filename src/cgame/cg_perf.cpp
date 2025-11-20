#include "cg_perf.h"

void __cdecl CG_PerfInit()
{
  if ( !cg_perfInfo.initialized )
  {
    memset((unsigned __int8 *)&cg_perfInfo, 0, sizeof(cg_perfInfo));
    cg_perfInfo.frame.count = 32;
    cg_perfInfo.script.count = 10;
    cg_perfInfo.cscript.count = 10;
    cg_perfInfo.server.count = 20;
    cg_perfInfo.renderExec.count = 32;
    cg_perfInfo.renderSwap.count = 32;
    cg_perfInfo.initialized = 1;
  }
}

void __cdecl CG_PerfUpdate()
{
  int frameMS; // [esp+10h] [ebp-8h]
  unsigned intcurrentMS; // [esp+14h] [ebp-4h]

  CG_PerfInit();
  currentMS = Sys_Milliseconds();
  frameMS = currentMS - previousMS;
  previousMS = currentMS;
  UpdateData(&cg_perfInfo.frame, frameMS);
  UpdateData(&cg_perfInfo.server, (int)((double)gRunFrameTicks * msecPerRawTimerTick));
  UpdateData(&cg_perfInfo.script, gScrExecuteTime[0]);
  UpdateData(&cg_perfInfo.cscript, dword_A05AC7C);
  UpdateData(&cg_perfInfo.renderExec, rb_execCmdsMS);
  UpdateData(&cg_perfInfo.renderSwap, rb_swapMS);
}

void __cdecl UpdateData(CG_PerfData *data, int value)
{
  int v2; // eax
  int i; // [esp+10h] [ebp-4h]
  int ia; // [esp+10h] [ebp-4h]

  data->history[data->index % 32] = value;
  data->instant = value;
  data->min = 0x7FFFFFFF;
  data->max = 0;
  data->average = *(float *)&FLOAT_0_0;
  data->variance = *(float *)&FLOAT_0_0;
  data->total = 0;
  for ( i = 0; i < data->count; ++i )
  {
    v2 = (data->index - i) % 32;
    if ( v2 < 0 )
      break;
    data->total += data->history[v2];
    if ( data->min > data->history[v2] )
      data->min = data->history[v2];
    if ( data->max < data->history[v2] )
      data->max = data->history[v2];
  }
  data->average = (float)data->total / (float)data->count;
  for ( ia = 0; ia < data->count && (data->index - ia) % 32 >= 0; ++ia )
    data->variance = data->variance
                   + COERCE_FLOAT(
                       COERCE_UNSIGNED_INT((float)data->history[(data->index - ia) % 32] - data->average)
                     & _mask__AbsFloat_);
  data->variance = data->variance / (float)data->count;
  ++data->index;
}

