#pragma once

netadr_t *__cdecl CL_GetLastRconAddress();
void __cdecl CL_SetServerInfo(serverInfo_t *server, char *info, __int16 ping);
void __cdecl CL_ServerInfoPacket(bdSecurityID *secID, msg_t *msg, int time);
void __cdecl CL_Connect(serverInfo_t *server);
void __cdecl CL_ConnectHackDW();
void __cdecl CL_Connect_f();
void __cdecl CL_PC_SignInLive();
void __cdecl CL_InitServerInfo(serverInfo_t *server, netadr_t adr);
int __cdecl CL_RawPingSetupBuffer(
        unsigned __int8 *buffer,
        int buffersize,
        unsigned __int8 opcode,
        const bdSecurityID *secID);
void __cdecl CL_RawPingServer(serverInfo_t *server, unsigned __int8 opcode);
void __cdecl CL_ServersResponsePacket(MatchMakingInfo *mminfo, int numResults, bool geo);
void __cdecl CL_FindServers_f();
void __cdecl CL_RconInit();
void __cdecl CL_Rcon_f();
void CL_RconLogin();
void CL_RconLogout();
void CL_RconHost();
serverStatusInfoResponse_s *__cdecl CL_GetServerStatus(bdSecurityID *secID);
serverStatusInfoResponse_s *__cdecl CL_GetServerStatusScoreBoard(bdSecurityID *secID);
int __cdecl CL_ServerStatus(char *serversecurityID, char *serverStatusString, int maxLen);
int __cdecl CL_ServerStatusScoreBoard(char *serversecurityID, char *serverStatusString, int maxLen);
void __cdecl CL_ServerStatusScoreBoardResponse(msg_t *msg, bdSecurityID *secID);
void __cdecl CL_ServerStatusResponse(msg_t *msg, bdSecurityID *secID);
void __cdecl CL_ResetPlayerMuting(unsigned int muteClientIndex);
void __cdecl CL_MutePlayer(int localClientNum, unsigned int muteClientIndex);
bool __cdecl CL_IsPlayerMuted(int localClientNum, unsigned int muteClientIndex);
void __cdecl CL_ClearMutedList();
void __cdecl CL_FinishMotdDownload();
void __cdecl CL_WWWDownload();
void __cdecl CL_Platform_RegisterCommands();
void __cdecl CL_PC_RequireLiveSignin();
void __cdecl CL_LanSessions_f();
void __cdecl CL_LanConnect_f();
void __cdecl CL_Prestige_f();
void __cdecl CL_RequestCACValidateSuccess();
void __cdecl CL_RequestCACValidateFailure();
void __cdecl CL_CACValidateHandleNACK(unsigned __int64 uid);
char __cdecl CL_CACValidateServerMatches(unsigned __int64 uid);
void __cdecl CL_CACValidateHandleOK(unsigned __int64 uid);
void __cdecl CL_CACValidateHandleBad(unsigned __int64 uid);
char __cdecl CL_RequestCACValidate(unsigned __int64 serverId);
void __cdecl CL_CACValidateRequest_f();
bool __cdecl CL_CACValidateInProgress();
char __cdecl CL_PrestigeRequest();
bool __cdecl CL_CACValidate_IsTimedOut();
void __cdecl CL_CACValidate_Frame();
void __cdecl CL_QuickMatchConnect_f();
bool __cdecl CL_QuickMatch_InProgress();
void __cdecl CL_QuickMatch_FindSessionsSuccess(TaskRecord *task);
void __cdecl CL_QuickMatch_FindSessionsFailure();
void __cdecl CL_QuickMatch_Start(
        unsigned int servertype,
        const char *mapname,
        unsigned int playlist,
        int minPlayers,
        int maxPlayers,
        int maxPing);
void __cdecl CL_QuickWager_Start();
void __cdecl CL_QuickMatch_f();
void __cdecl CL_QuickWager_f();
void __cdecl CL_QuickMatch_InitDvars();
void __cdecl CL_QuickMatch_Init();
bool __cdecl CL_QuickMatch_ServerMatches(MatchMakingInfo *server);
void __cdecl CL_QuickMatch_PingServers();
bool __cdecl CL_QuickMatch_GoodSessionFound();
void __cdecl CL_QuickMatch_PingResponse(bdSecurityID *secID, msg_t *msg);
char __cdecl CL_QuickMatch_ChooseSession();
int __cdecl CL_QuickMatch_CompareServers(unsigned int *sv1, unsigned int *sv2);
void __cdecl CL_QuickMatch_Frame();
bool __cdecl CL_QuickMatch_ShouldChooseSession();
char *__cdecl CL_LongNameForShortName(const char *shortname);
const bdReference<bdCommonAddr> *__thiscall bdReference<bdCommonAddr>::operator=(
        bdReference<bdCommonAddr> *this,
        const bdReference<bdCommonAddr> *other);
void __thiscall bdReference<bdRemoteTask>::~bdReference<bdRemoteTask>(bdReference<bdCommonAddr> *this);
void __thiscall dwQoSMultiProbeListener::~dwQoSMultiProbeListener(dwQoSMultiProbeListener *this);
void __thiscall bdReference<bdCommonAddr>::operator=(bdReference<bdRemoteTask> *this, bdRemoteTask *p);
void __cdecl CL_PC_SignIn();
