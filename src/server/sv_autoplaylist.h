#pragma once

void __cdecl SV_AP_DumpTable();
bool __cdecl SV_AP_ParseControlFile(unsigned __int8 *controlFile);
int __cdecl SV_AP_PlaylistFromDistribution();
int __cdecl comparePlaylists(unsigned int *p1, unsigned int *p2);
void __cdecl SV_AP_GetControlFileComplete();
int __cdecl SV_AP_GetControlFileFailure();
TaskRecord *__cdecl SV_AP_GetControlFile();
void __cdecl SV_AP_GetControlFileName(char *buf, int buflen);
void __cdecl SV_SetGroupCountsComplete();
void __cdecl SV_GetGroupCountsComplete();
void __cdecl SV_GroupsFailure(TaskRecord *task);
void SV_GroupError(const char *fmt, ...);
TaskRecord *__cdecl SV_GetGroupCounts();
void __cdecl SV_Groups_SetGroupMembership(bool full);
void __cdecl SV_Groups_ParseGeos(const char *geoblob);
void __cdecl SV_AP_Start();
void __cdecl SV_AP_Frame();
bool __cdecl SV_AP_ServerIsFull();
dwFileTask *__thiscall dwFileTask::dwFileTask(dwFileTask *this);
