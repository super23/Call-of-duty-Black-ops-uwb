#pragma once

#include <universal/assertive.h>

struct TaskRecord;

// Autoplaylist state machine (KISAK_LIVE): geo group counts + DW control CSV → playlist dvar.
// Retail: CoDOMPServer.c; driven from dwLogOn_pc (SV_AP_Frame) and live_storage_pub (geos WAD).

enum sv_apstate_t : __int32
{
    AP_START       = 0x0, // kick off parallel group-count + control-file fetches
    AP_GETTINGDATA = 0x1, // waiting on both async tasks
    AP_GETONE      = 0x2, // one task finished; waiting on the other
    AP_CHOOSE      = 0x3, // both done; parse CSV and pick playlist
    AP_SETTINGDATA = 0x4, // bdGroup::setGroups in flight after parse
    AP_SLEEPING    = 0x5, // idle; may promote to "full" group membership
    AP_ERROR       = 0x6, // unrecoverable (timeout / task start failure)
};

// Retail pre-increment: only legal in AP_GETTINGDATA → AP_GETONE → AP_CHOOSE.
inline sv_apstate_t &operator++(sv_apstate_t &state)
{
    if (state != AP_GETTINGDATA && state != AP_GETONE)
    {
        if (!Assert_MyHandler(
                "src/server/sv_autoplaylist.cpp",
                66,
                0,
                "Tried to increment s_apstate when not incrementable\n"))
        {
            __debugbreak();
        }
        state = AP_ERROR;
    }
    else
    {
        state = static_cast<sv_apstate_t>(static_cast<int>(state) + 1);
    }
    return state;
}

// --- Control file (geo-specific *.csv probabilities) ---

void __cdecl SV_AP_DumpTable();
bool __cdecl SV_AP_ParseControlFile(unsigned __int8 *controlFile);
int __cdecl SV_AP_PlaylistFromDistribution();

// qsort comparator for goaldiff_t[] (sorts by diff descending).
int __cdecl comparePlaylists(const void *elemA, const void *elemB);

void __cdecl SV_AP_GetControlFileComplete();
int __cdecl SV_AP_GetControlFileFailure(); // taskCompleteResults: returns TASK_ERROR (1)
TaskRecord *__cdecl SV_AP_GetControlFile();
void __cdecl SV_AP_GetControlFileName(char *buf, int buflen);

// --- Demonware bdGroup tasks ---

void __cdecl SV_SetGroupCountsComplete();
void __cdecl SV_GetGroupCountsComplete();
void __cdecl SV_GroupsFailure(TaskRecord *task);
void SV_GroupError(const char *fmt, ...);
TaskRecord *__cdecl SV_GetGroupCounts();
void __cdecl SV_Groups_SetGroupMembership(bool full);
void __cdecl SV_Groups_ParseGeos(const char *geoblob);

// --- Frame entry points ---

void __cdecl SV_AP_Start();
void __cdecl SV_AP_Frame();
bool __cdecl SV_AP_ServerIsFull();
