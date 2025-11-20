#include "win_workercmds.h"

int __cdecl nuge_physicsCallback(jqBatch *batch)
{
  char *Name; // [esp+8h] [ebp-14h]
  unsigned int *realbatch; // [esp+18h] [ebp-4h]

  realbatch = jqLockData(batch);
  Name = va("nuge_physics: %s", *(const char **)realbatch[3]);
  PIXBeginNamedEvent(-1, Name);
  (*(void (__cdecl **)(unsigned int *))(realbatch[3] + 8))(realbatch);
  jqUnlockData(batch);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  return 0;
}

void __cdecl IW_task_manager_add_batch(jqBatch *ptr)
{
  if ( !ptr->Module
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\win32\\win_workercmds.cpp", 51, 0, "%s", "ptr->Module") )
  {
    __debugbreak();
  }
  Sys_AddWorkerCmdInternal(&nuge_physicsWorkerCmd, (unsigned __int8 *)ptr, 0);
}

void __cdecl IW_task_manager_flush()
{
  Sys_WaitWorkerCmdInternal(&nuge_physicsWorkerCmd);
}

void __cdecl R_InitWorkerCmds()
{
  jqQueue *WorkerQueue; // eax
  jqQueue *v1; // eax
  jqBatch batch; // [esp+0h] [ebp-80h] BYREF

  jqInit();
  jqEnableWorkers(0xCu);
  jqSetBatchDataHeapSize(0x4000u, 0x10u);
  jqStart();
  jqBatch::jqBatch(&batch);
  batch.Module = &`anonymous namespace'::setup_worker_threadsModule;
  batch.ParamData[0] = 0;
  WorkerQueue = jqGetWorkerQueue(4);
  jqAddBatch(&batch, WorkerQueue);
  batch.ParamData[0] = 1;
  v1 = jqGetWorkerQueue(8);
  jqAddBatch(&batch, v1);
  jqFlush(0, 0);
  Sys_InitWorkerThreadContext();
  Sys_WorkerCmdInit();
}

void __cdecl R_InitWorkerThreads()
{
  R_InitWorkerCmds();
}

