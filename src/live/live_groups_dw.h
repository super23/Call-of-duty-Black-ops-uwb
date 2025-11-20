#pragma once

void __cdecl LiveGroups_Init();
void __cdecl LiveGroups_SetGroupsComplete(TaskRecord *task);
void __cdecl LiveGroups_SetGroups(int localControllerIndex);
void __cdecl LiveGroups_GetGroupCountsComplete(TaskRecord *task);
void __cdecl LiveGroups_GetCounts(int localControllerIndex);
char __cdecl LiveGroups_GetGroupID(char *path, int *offset, GroupSet **gs);
void __cdecl LiveGroups_JoinGroup(int localControllerIndex, char *path);
void __cdecl LiveGroups_LeaveGroup(int localControllerIndex, char *path);
unsigned int __cdecl LiveGroups_GetCount(char *path);
unsigned int __cdecl LiveGroups_GetCountByType(servertype_t type);
void __cdecl LiveGroups_RegisterPlayer(int localControllerIndex);
void __cdecl LiveGroups_Update(int localControllerIndex);
void __cdecl LiveGroups_JoinGroup_f();
void __cdecl LiveGroups_LeaveGroup_f();
void __cdecl LiveGroups_Dump_f();
