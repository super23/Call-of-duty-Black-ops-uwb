#pragma once

struct client_t;

// Demonware / live-storage helpers: map bd online user IDs (XUID) to connected clients.
// Signatures match CoDOMPServer.c (retail used _QWORD* for file-info payloads).

// CoDOMPServer.c @ 0x712460 — connected client slot for bdOnlineUserId, or -1.
int __cdecl SV_GetClientNumForBdOnlineUserID(unsigned __int64 bdOnlineUserId);

// CoDOMPServer.c @ 0x7124F0 — ownerID from dwFileOperationInfo (retail: *((_QWORD *)data + 34)).
// fileInfo is really dwFileOperationInfo* (see live_storage.h).
unsigned __int64 __cdecl SV_GetBdUidFromFileInfo(void *fileInfo);

// CoDOMPServer.c @ 0x712510 — resolve ownerID to client_t*, or nullptr if disconnected.
// fileInfo is really dwFileOperationInfo*; null fileInfo returns nullptr.
client_t *__cdecl SV_GetClientFromFileInfo(void *fileInfo);

// CoDOMPServer.c @ 0x7125E0 — svs.clients[clientNum].dw_userID.
unsigned __int64 __cdecl SV_GetPlayerXuid(unsigned int clientNum);
