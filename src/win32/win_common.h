#pragma once

void __cdecl Sys_Mkdir(const char *path);
char *__cdecl Sys_Cwd();
const char *__cdecl Sys_DefaultCDPath();
char *__cdecl Sys_DefaultInstallPath();
HunkUser **__cdecl Sys_ListFiles(char *directory, char *extension, char *filter, int *numfiles, int wantsubs);
void __cdecl Sys_ListFilteredFiles(
        HunkUser *user,
        const char *basedir,
        const char *subdirs,
        const char *filter,
        char **list,
        int *numfiles);
bool __cdecl HasFileExtension(const char *name, const char *extension);
int __cdecl Sys_DirectoryHasContents(const char *directory);
void __cdecl Sys_InitializeCriticalSections();
void __cdecl Sys_EnterCriticalSection(CriticalSection critSect);
bool __cdecl Sys_TryEnterCriticalSection(CriticalSection critSect);
void __cdecl Sys_LeaveCriticalSection(CriticalSection critSect);
