#pragma once

void __cdecl MemFile_CommonInit(
        MemoryFile *memFile,
        int size,
        unsigned __int8 *buffer,
        bool errorOnOverflow,
        bool compress_enabled);
void __cdecl MemFile_InitForReading(MemoryFile *memFile, int size, unsigned __int8 *buffer, bool compress_enabled);
void __cdecl MemFile_InitForWriting(
        MemoryFile *memFile,
        int size,
        unsigned __int8 *buffer,
        bool errorOnOverflow,
        bool compress_enabled);
void __cdecl MemFile_StartSegment(MemoryFile *memFile, int index);
void __cdecl MemFile_EndSegment(MemoryFile *memFile);
void __cdecl MemFile_MoveToSegment(MemoryFile *memFile, int index);
void __cdecl MemFile_inflateInit(MemoryFile *memFile);
unsigned __int8 *__cdecl MemFile_GetSegmentAddess(MemoryFile *memFile, unsigned int index);
bool __cdecl MemFile_IsReading(MemoryFile *memFile);
void __cdecl MemFile_WriteDataFlushInternal(MemoryFile *memFile);
void __cdecl MemFile_WriteDataInternal(MemoryFile *memFile, unsigned int byteCount, unsigned __int8 *p);
char *__cdecl MemFile_ReadCString(MemoryFile *memFile);
void __cdecl MemFile_ReadData(MemoryFile *memFile, int byteCount, unsigned __int8 *p);
