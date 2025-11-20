#pragma once

void __cdecl TempMemoryReset(HunkUser *user);
char *__cdecl TempMallocAlignStrict(int len);
void __cdecl TempMemorySetPos(char *pos);
