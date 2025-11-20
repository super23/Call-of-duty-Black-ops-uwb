#pragma once

void __cdecl DDL_Buffer_WriteHeader(char *buffer, ddlBufferHeader_t *header);
void __cdecl DDL_Buffer_ReadHeader(char *buffer, ddlBufferHeader_t *header);
int __cdecl DDL_Buffer_ReadBits(char *buffer, int offset, int size, unsigned int *value);
int __cdecl DDL_Buffer_WriteBits(char *buffer, int offset, int size, unsigned int value);
int __cdecl DDL_Buffer_ClearBits(char *buffer, int offset, int size);
