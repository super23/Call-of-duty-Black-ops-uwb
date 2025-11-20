#include "cscr_tempmemory.h"

void __cdecl TempMemoryReset(HunkUser *user)
{
  *(unsigned int *)(*((unsigned int *)NtCurrentTeb()->ThreadLocalStoragePointer + _tls_index) + 8304) = user;
}

char *__cdecl TempMallocAlignStrict(int len)
{
  return (char *)Hunk_UserAlloc(
                   *(HunkUser **)(*((unsigned int *)NtCurrentTeb()->ThreadLocalStoragePointer + _tls_index) + 8304),
                   len,
                   1,
                   0);
}

void __cdecl TempMemorySetPos(char *pos)
{
  Hunk_UserSetPos(
    *(HunkUser **)(*((unsigned int *)NtCurrentTeb()->ThreadLocalStoragePointer + _tls_index) + 8304),
    (const char **)pos);
}

