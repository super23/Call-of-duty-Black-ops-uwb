#pragma once

void __cdecl Session_ClearDWOverlappedTasks();
void __cdecl Live_FindSessionsStart(bool reset, int servertype);
void __cdecl Live_FindSessionsPump();
void __cdecl Session_QoSListenStart(SessionData_s *session);
void __cdecl Session_QoSListenStop(SessionData_s *session);
int __cdecl Session_FindRegisteredUser(const SessionData_s *session, unsigned __int64 xuid);
void __cdecl Session_EndGameSession(SessionData_s *session);
void __cdecl Session_DeleteSession(SessionData_s *session);
void __cdecl Session_DeleteHandle(bool *sessionHandle);
void __cdecl Session_UnregisterAllUsersFromVoice(SessionData_s *session);
int __cdecl Session_GetFreeSessionGraveYardSlot();
void __cdecl Session_StartHost(SessionData_s *session, int sessionFlags, int numPrivateSlots, int numPublicSlots);
int __cdecl Session_GetFreeCreateSessionSlot();
void __cdecl Session_Modify(
        int localControllerIndex,
        SessionData_s *session,
        int flags,
        int publicSlots,
        int privateSlots);
taskCompleteResults __cdecl Session_ModifyComplete(int slot);
void __cdecl Session_EveryoneLeaveSessionAsync(int localControllerIndex, SessionData_s *session);
char __cdecl Session_JoinInProgress();
bool __cdecl Session_SessionTasksInProgress(SessionData_s *session);
int __cdecl Session_StartSessionComplete(int slot);
int __cdecl Session_JoinSessionComplete(int slot);
void __cdecl Session_EndOngoingSessionTasks(SessionData_s *session);
taskCompleteResults __cdecl Session_StartHostComplete(int slot);
int __cdecl Session_EveryoneLeaveSessionComplete(int slot);
void __cdecl Live_FinishOngoingSessionJoinTasksForXUID(unsigned __int64 player);
void __cdecl Live_CheckOngoingSessionTasks();
void Session_ManageGraveYard();
void __cdecl Session_Init();
