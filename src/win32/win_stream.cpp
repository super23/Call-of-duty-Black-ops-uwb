#include "win_stream.h"

char __cdecl Stream_Init()
{
  if ( !g_streamReadThreadSpawned && !Sys_SpawnStreamThread((void (__cdecl *)(unsigned int))Stream_Thread) )
    return 0;
  g_streamReadThreadSpawned = 1;
  return 1;
}

void __cdecl  Stream_Thread(unsigned int threadContext)
{
  void *Value; // eax

  if ( threadContext != 14
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\win32\\win_stream.cpp",
          19,
          0,
          "threadContext == THREAD_CONTEXT_STREAM\n\t%i, %i",
          threadContext,
          14) )
  {
    __debugbreak();
  }
  do
    Value = Sys_GetValue(2);
  while ( _setjmp3(Value, 0) );
  while ( 1 )
  {
    Sys_StreamSleep();
    R_StreamUpdate_ReadTextures();
    Snd_StreamUpdate();
  }
}

