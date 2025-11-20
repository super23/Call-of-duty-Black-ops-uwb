#pragma once

void __cdecl track_static_alloc_internal_simple(void *ptr, int size, const char *name, int memTrack);
void __cdecl track_addbasicinfo(meminfo_t *info, int type, int location, int size);
void __cdecl track_static_alloc_internal(void *ptr, int size, const char *name, int type);
void __cdecl track_flush_physical_alloc(const char *name, unsigned __int8 type);
TempMemInfo *__cdecl GetTempMemInfo(
        int permanent,
        const char *name,
        unsigned __int8 type,
        int usageType,
        TempMemInfo *tempMemInfoArray,
        int *tempMemInfoCount,
        bool add_if_missing);
void __cdecl track_set_max_memory_level();
void __cdecl track_z_alloc(int size, const char *name, int type, char *pos, int project, int overhead);
void __cdecl AddTempMemInfo(
        int size,
        int hunkSize,
        int permanent,
        const char *name,
        unsigned __int8 type,
        int location,
        int usageType,
        TempMemInfo *tempMemInfoArray,
        int *tempMemInfoCount);
void __cdecl CheckHighMemInfo(TempMemInfo *tempMemInfo, int hunkSize, int location);
void __cdecl track_z_free(int type, char *pos, int overhead);
void __cdecl RemoveTempMemInfo(
        int size,
        int permanent,
        const char *name,
        unsigned __int8 type,
        int location,
        int usageType,
        TempMemInfo *tempMemInfoArray,
        int *tempMemInfoCount);
void __cdecl CheckLowMemInfo(TempMemInfo *tempMemInfo, int location);
void __cdecl track_z_commit(int size, int type);
void __cdecl track_z_decommit(int size, int type);
void __cdecl track_physical_alloc(int size, const char *name, int type, int location);
void __cdecl track_temp_alloc(int size, int hunkSize, int permanent, const char *name);
void __cdecl track_temp_free(int size, int permanent, const char *name);
void __cdecl track_temp_clear(int permanent);
void __cdecl track_temp_high_alloc(int size, int hunkSize, int permanent, const char *name);
void __cdecl track_temp_high_clear(int permanent);
void __cdecl track_userhunk_freerange(void *low, unsigned int size);
void __cdecl track_userhunk_free(void *ptr);
void __cdecl track_userhunk_alloc(int size, int pos, const char *name, int type);
void __cdecl track_hunk_alloc(int size, int pos, const char *name, int type);
void __cdecl track_hunk_allocLow(int size, int pos, const char *name, int type);
void __cdecl track_set_hunk_size(int size);
void __cdecl track_hunk_ClearToMarkHigh(int mark);
void __cdecl track_hunk_ClearToMarkLow(int mark);
void __cdecl track_hunk_ClearToStart();
void track_temp_reset();
void track_temp_high_reset();
void __cdecl track_init();
void __cdecl track_getbasicinfo(meminfo_t *info);
void __cdecl track_addbasicmeminfo(meminfo_t *sum, meminfo_t *in);
void __cdecl track_shutdown(int project);
void __cdecl track_PrintInfo();
double __cdecl mb(int n);
int __cdecl mem_track_compare(const char *elem1, const char *elem2);
void __cdecl track_PrintAllInfo();
void bitarray<54>::bitarray<54>(bitarray<54> *this, unsigned int first, ...);
bool __thiscall bitarray<54>::testBit(bitarray<54> *this, unsigned int pos);
void __thiscall bitarray<54>::setBit(bitarray<54> *this, unsigned int pos);
