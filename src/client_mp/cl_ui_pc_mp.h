#pragma once

int __cdecl LAN_AddServer(int source, const char *name, const char *address);
void __cdecl LAN_GetServerAddressString(int source, int n, char *buf, int buflen);
void __cdecl LAN_GetServerSecurityId(int source, int n, bdSecurityID *buf, int buflen);
void __cdecl LAN_ResetPings(int source);
int __cdecl LAN_GetServerCount(int source);
int __cdecl LAN_WaitServerResponse();
void __cdecl LAN_GetServerInfo(int source, int n, char *buf, int buflen);
int __cdecl LAN_GetServerPing(int source, int n);
serverInfo_t *__cdecl LAN_GetServerPtr(int source, int n);
void __cdecl LAN_CleanHostname(const char *hostName, char *cleanHostName);
int __cdecl LAN_CompareHostname(const char *hostName1, const char *hostName2);
int __cdecl LAN_CompareServers(int source, int sortKey, int sortDir, int s1, int s2);
void __cdecl LAN_MarkServerDirty(int source, int n, unsigned __int8 dirty);
int __cdecl LAN_ServerIsDirty(int source, int n);
int __cdecl LAN_UpdateDirtyPings(int localClientNum, unsigned int source);
int __cdecl LAN_GetServerStatus(char *serverSecurityID, char *serverStatus, int maxLen);
int __cdecl LAN_GetServerStatusScoreBoard(char *serverSecurityID, char *serverStatus, int maxLen);
void __cdecl CLUI_SetPbClStatus(int status);
