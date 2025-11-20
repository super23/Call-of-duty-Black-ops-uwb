#include "dwTasks.h"

void __cdecl dwClearTask()
{
  ;
}

int __cdecl dwGetTaskStatus(overlappedTask *const task)
{
  enum bdRemoteTask::bdStatus taskStatus; // [esp+8h] [ebp-8h]
  taskCompleteResults status; // [esp+Ch] [ebp-4h]

  status = TASK_ERROR;
  if ( !task && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\Dw\\dwTasks.cpp", 21, 0, "%s", "task") )
    __debugbreak();
  if ( task && task->overlappedIO.m_ptr )
  {
    taskStatus = bdRemoteTask::getStatus(task->overlappedIO.m_ptr);
    return dwTaskStatusConvert(taskStatus, BD_NO_ERROR);
  }
  return status;
}

