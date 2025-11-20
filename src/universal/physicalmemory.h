#pragma once

void __cdecl PMem_Init();
void __cdecl PMem_InitPhysicalMemory(
        PhysicalMemory *pmem,
        const char *name,
        unsigned __int8 *memory,
        unsigned int memorySize);
void __cdecl PMem_BeginAlloc(const char *name, unsigned int allocType, EMemTrack memTrack);
void __cdecl PMem_BeginAllocInPrim(PhysicalMemoryPrim *prim, const char *name, EMemTrack memTrack);
void __cdecl PMem_EndAlloc(const char *name, unsigned int allocType);
void __cdecl PMem_EndAllocInPrim(PhysicalMemoryPrim *prim, const char *name);
void __cdecl PMem_Free(const char *name);
void __cdecl PMem_FreeInPrim(PhysicalMemoryPrim *prim, const char *name, int location);
void __cdecl PMem_FreeIndex(PhysicalMemoryPrim *prim, unsigned int allocIndex, int location);
int __cdecl PMem_GetOverAllocatedSize();
unsigned __int8 *__cdecl _PMem_AllocNamed(
        unsigned int size,
        unsigned int alignment,
        signed int type,
        unsigned int allocType,
        const char *name,
        EMemTrack memTrack);
unsigned __int8 *__cdecl _PMem_Alloc(
        unsigned int size,
        unsigned int alignment,
        unsigned int type,
        unsigned int allocType,
        EMemTrack memTrack,
        const char *file,
        int lineNum);
