#pragma once

HunkUser *__cdecl Hunk_FirstFitInit(
        unsigned int *buffer,
        unsigned int size,
        HU_ALLOCATION_SCHEME scheme,
        unsigned int flags,
        void *scheme_specific_data,
        const char *name,
        int type);
void __cdecl Hunk_FirstFitReset(HunkUser *_user);
void __cdecl Hunk_FirstFitDestroy(HunkUser *_user);
int __cdecl Hunk_FirstFitAlloc(HunkUser *_user, int size, int alignment);
void __cdecl Hunk_FirstFitFree(HunkUser *_user, unsigned int *ptr);
