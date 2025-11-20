#pragma once

void __cdecl MT_DumpTree(scriptInstance_t inst);
int __cdecl MT_GetSubTreeSize(scriptInstance_t inst, int nodeNum);
const char *__cdecl MT_NodeInfoString(scriptInstance_t inst, unsigned int nodeNum);
int __cdecl Scr_GetStringUsage(scriptInstance_t inst);
void __cdecl MT_Init(scriptInstance_t inst);
void __cdecl MT_InitBits(scriptInstance_t inst);
void __cdecl MT_AddMemoryNode(scriptInstance_t inst, int newNode, unsigned int size);
int __cdecl MT_GetScore(scriptInstance_t inst, int num);
unsigned __int16 __cdecl MT_AllocIndex(scriptInstance_t inst, int numBytes, int type);
void __cdecl MT_RemoveHeadMemoryNode(scriptInstance_t inst, unsigned int size);
void __cdecl MT_Error(scriptInstance_t inst, const char *funcName, int numBytes);
int __cdecl MT_GetSize(scriptInstance_t inst, int numBytes);
void __cdecl MT_FreeIndex(scriptInstance_t inst, unsigned int nodeNum, int numBytes);
char __cdecl MT_RemoveMemoryNode(scriptInstance_t inst, int oldNode, unsigned int size);
char *__cdecl MT_Alloc(int numBytes, int type, scriptInstance_t inst);
void __cdecl MT_Free(_BYTE *p, int numBytes, scriptInstance_t inst);
bool __cdecl MT_Realloc(scriptInstance_t inst, int oldNumBytes, int newNumbytes);
