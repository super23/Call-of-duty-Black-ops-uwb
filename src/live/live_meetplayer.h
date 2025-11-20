#pragma once

void __cdecl LiveMeetPlayer_DownloadMetPlayersList(int localControllerIndex);
void __cdecl LiveMeetPlayer_ClearUploadFlags(int controllerIndex);
bool __cdecl LiveMeetPlayer_UploadMetPlayerListNeeded(int controllerIndex);
void __cdecl LiveMeetPlayer_ResetData(int controllerIndex);
void __cdecl LiveMeetPlayer_CheckDownloadedVersionAndResetData(int localControllerIndex);
void __cdecl LiveMeetPlayer_UploadMetPlayerList(int localControllerIndex);
void __cdecl LiveMeetPlayer_Init();
int __cdecl LiveMeetPlayer_GetXUIDPosInMetPlayerList(int localControllerIndex, unsigned __int64 currentPlayerXuid);
int __cdecl LiveMeetPlayer_GetNoOfMetPlayers(int localControllerIndex);
void __cdecl LiveMeetPlayer_SortMetPlayers(int localControllerIndex, unsigned int noOfPlayers);
int __cdecl LiveMeetPlayer_MetPlayerCompare(_QWORD *arg0, _QWORD *arg1);
void __cdecl LiveMeetPlayer_UpdateMeetPlayerList(
        int localControllerIndex,
        unsigned __int64 playeruid,
        const char *playername);
void __cdecl LiveMeetPlayer_SetCurrentTime(_FILETIME *currentTime);
char __cdecl LiveMeetPlayer_AddPlayerAtSlot(
        int localControllerIndex,
        unsigned __int64 playeruid,
        const char *playername,
        int slot);
unsigned int __cdecl LiveMeetPlayer_GetLastMetTime(int localControllerIndex, int index);
XuidInfo *__cdecl LiveMeetPlayer_GetMetFriendInfo(XuidInfo *result, int localControllerIndex, int index);
char __cdecl LiveMeetPlayer_GetRecentPlayerInfoByID(
        int controllerIndex,
        unsigned __int64 id,
        XuidInfo *recentMetPlayerInfo);
unsigned int __cdecl LiveMeetPlayer_GetRecentPlayerIndexByID(int controllerIndex, unsigned __int64 id);
void __cdecl LiveMeetPlayer_UpdatePlayerSession(unsigned __int64 playerUid, unsigned __int64 sessionUid);
int __cdecl LiveMeetPlayer_GetPlayerSessionByID(unsigned __int64 playerUid);
