#pragma once

HunkUser *__cdecl Hunk_UserDebugInit(
        unsigned int *buffer,
        int size,
        HU_ALLOCATION_SCHEME scheme,
        unsigned int flags,
        void *scheme_specific_data,
        const char *name,
        int type);
void __cdecl Hunk_UserDebugReset(HunkUser *_user);
void __cdecl Hunk_UserDebugDestroy(HunkUser *_user);
int __cdecl Hunk_UserDebugAlloc(HunkUser *_user, int size, int alignment);
void __cdecl Hunk_UserDebugFree(HunkUser *_user, unsigned int *ptr);
HunkUser *__cdecl Hunk_UserDefaultInit(
        unsigned __int8 *buffer,
        int size,
        HU_ALLOCATION_SCHEME scheme,
        unsigned int flags,
        void *scheme_specific_data,
        const char *name,
        int type);
void __cdecl Hunk_UserDefaultReset(HunkUser *_user);
void __cdecl Hunk_UserDefaultDestroy(HunkUser *_user);
int __cdecl Hunk_UserDefaultAlloc(HunkUser *_user, unsigned int size, int alignment, const char *name);
void __cdecl Hunk_UserDefaultFree(HunkUser *user, void *ptr);
void __cdecl Hunk_UserStartup();
void __cdecl Hunk_UserShutdown();
HunkUser *__cdecl Hunk_UserCreateFromBuffer(
        void *buffer,
        int size,
        HU_ALLOCATION_SCHEME scheme,
        unsigned int flags,
        void *scheme_specific_data,
        const char *name,
        int type);
HunkUser *__cdecl Hunk_UserCreate(
        int size,
        HU_ALLOCATION_SCHEME scheme,
        unsigned int flags,
        void *scheme_specific_data,
        const char *name,
        int type);
void *__cdecl Hunk_UserAlloc(HunkUser *user, int size, int alignment, const char *name);
void __cdecl Hunk_UserFree(HunkUser *user, void *ptr);
void __cdecl Hunk_UserReset(HunkUser *user);
void __cdecl Hunk_UserDestroy(HunkUser *user);
void __cdecl Hunk_UserSetPos(HunkUser *_user, const char **pos);
char *__cdecl Hunk_CopyString(HunkUser *user, const char *in);
