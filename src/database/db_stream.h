#pragma once

void __cdecl DB_InitStreams(XBlock *blocks);
void __cdecl DB_PushStreamPos(unsigned int index);
void __cdecl DB_SetStreamIndex(unsigned int index);
void __cdecl DB_PopStreamPos();
unsigned __int8 *__cdecl DB_GetStreamPos();
unsigned __int8 *__cdecl DB_AllocStreamPos(int alignment);
void __cdecl DB_IncStreamPos(int size);
const void **__cdecl DB_InsertPointer();
