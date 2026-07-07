#pragma once

struct StreamDelayInfo
{
    const void *ptr;
    int size;
};

void __cdecl Load_Stream(bool atStreamStart, unsigned __int8 *ptr, int size);
void __cdecl Load_DelayStream();
void __cdecl DB_ConvertOffsetToAlias(unsigned int *data);
void __cdecl DB_ConvertOffsetToPointer(unsigned int *data);
void __cdecl Load_XStringCustom(const char **str);
void __cdecl Load_TempStringCustom(const char **str);
