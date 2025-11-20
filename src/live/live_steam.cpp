#include "live_steam.h"

void __cdecl SteamAPIDebugTextHook(int severity, const char *pchDebugText)
{
  if ( severity != 1 || I_strcmp(pchDebugText, "[S_API WARN] Invalid game server IP passed to InitiateGameConnection\n") )
    Com_PrintWarning(0, "Steam Warning sev %d: %s", severity, pchDebugText);
}

void __cdecl  LiveSteam_Quit(bool showErrorMessage)
{
  char *v1; // eax
  char *v2; // [esp-8h] [ebp-8h]

  Sys_HideSplashWindow();
  if ( showErrorMessage )
  {
    v2 = Win_LocalizeRef("WIN_ERROR");
    v1 = Win_LocalizeRef("WIN_NO_STEAM");
    MessageBoxA(0, v1, v2, 0x10u);
  }
  Sys_Quit();
}

void __cdecl LiveSteam_CheckAccess()
{
  ;
}

bool __cdecl LiveSteam_IsInitialized()
{
  return g_liveSteamInitialized;
}

char __cdecl LiveSteam_IsClientSignedInLocally()
{
  CSteamID *v0; // eax
  int v2; // [esp+0h] [ebp-10h]
  int v3; // [esp+0h] [ebp-10h]
  char v4; // [esp+0h] [ebp-10h]
  int v5; // [esp+4h] [ebp-Ch]
  _BYTE v6[8]; // [esp+8h] [ebp-8h] BYREF

  v4 = 0;
  if ( _SteamUser(v2) )
  {
    v5 = _SteamUser(v3);
    v0 = (CSteamID *)(*(int (__thiscall **)(int, _BYTE *))(*(unsigned int *)v5 + 8))(v5, v6);
    if ( CSteamID::IsValid(v0) )
      return 1;
  }
  return v4;
}

bool __cdecl LiveSteam_IsClientSignedInOnline()
{
  int v1; // eax
  int v2; // [esp+0h] [ebp-4h]

  if ( !LiveSteam_IsClientSignedInLocally() )
    return 0;
  v1 = _SteamUser(v2);
  return (*(int (__thiscall **)(int, int))(*(unsigned int *)v1 + 4))(v1, v1);
}

unsigned __int64 __cdecl LiveSteam_GetClientIDAsXUID()
{
  CSteamID *v1; // eax
  int v2; // eax
  int v3; // [esp+0h] [ebp-24h]
  int v4; // [esp+0h] [ebp-24h]
  int v5; // [esp+0h] [ebp-24h]
  int v6; // [esp+4h] [ebp-20h]
  _BYTE v7[8]; // [esp+14h] [ebp-10h] BYREF
  _BYTE v8[8]; // [esp+1Ch] [ebp-8h] BYREF

  if ( !LiveSteam_IsClientSignedInLocally() )
    return 0;
  if ( !_SteamUser(v3)
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\live\\live_steam.cpp", 141, 0, "%s", "SteamUser()") )
  {
    __debugbreak();
  }
  v6 = _SteamUser(v4);
  v1 = (CSteamID *)(*(int (__thiscall **)(int, _BYTE *))(*(unsigned int *)v6 + 8))(v6, v8);
  if ( !CSteamID::IsValid(v1)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\live\\live_steam.cpp",
          142,
          0,
          "%s",
          "SteamUser()->GetSteamID().IsValid()") )
  {
    __debugbreak();
  }
  v2 = _SteamUser(v5);
  return *(_QWORD *)(*(int (__thiscall **)(int, _BYTE *))(*(unsigned int *)v2 + 8))(v2, v7);
}

const char *__cdecl LiveSteam_GetCountryCode()
{
  int v1; // eax
  const char *loc; // [esp+4h] [ebp-4h]

  loc = Dvar_GetString("cl_forceloc");
  if ( loc && *loc )
    return loc;
  v1 = _SteamUtils();
  return (const char *)(*(int (__thiscall **)(int, int))(*(unsigned int *)v1 + 16))(v1, v1);
}

unsigned __int64 __cdecl LiveSteam_GetUid()
{
  int v1; // [esp+0h] [ebp-18h]
  _BYTE v2[8]; // [esp+10h] [ebp-8h] BYREF

  if ( !_SteamUser() )
    return 0;
  v1 = _SteamUser();
  return *(_QWORD *)(*(int (__thiscall **)(int, _BYTE *))(*(unsigned int *)v1 + 8))(v1, v2);
}

unsigned int __stdcall ShellExecuteThredProc(const char *lpParam)
{
  char currentDirectory[268]; // [esp+0h] [ebp-110h] BYREF

  GetCurrentDirectoryA(0x104u, currentDirectory);
  g_ShellExecuteErr = (int)ShellExecuteA(0, "open", lpParam, 0, 0, 1);
  _InterlockedExchange(&g_ShellExecuteInProgress, 0);
  return 0;
}

bool __cdecl LiveSteam_LaunchOtherApp(const char *cmd)
{
  const char *v1; // eax
  char *url; // [esp+0h] [ebp-Ch]
  int result; // [esp+4h] [ebp-8h]
  unsigned int dwThreadID; // [esp+8h] [ebp-4h] BYREF

  if ( !I_stricmp(cmd, "CoDSP_rd.exe") )
  {
    url = "steam://run/42700";
    goto LABEL_8;
  }
  if ( !I_stricmp(cmd, "CoDMP_rd.exe") )
  {
    url = "steam://run/42710";
LABEL_8:
    g_ShellExecuteInProgress = 1;
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ShellExecuteThredProc, url, 0, &dwThreadID);
    while ( g_ShellExecuteInProgress )
      NET_Sleep(0xAu);
    result = g_ShellExecuteErr;
    if ( g_ShellExecuteErr <= 32 )
      Com_Printf_NoFilter("Steam_LaunchOtherApp: ShellExecute return less than 32, '%d'\n", g_ShellExecuteErr);
    return result > 32;
  }
  Com_Printf_NoFilter("Steam_LaunchOtherApp: Don't know how to launch '%s'\n", cmd);
  v1 = va("Unknown exe '%s' to launch through Steam.\n", cmd);
  if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\live\\live_steam.cpp", 195, 0, v1) )
    __debugbreak();
  return 0;
}

void __cdecl LiveSteam_ShowStore()
{
  int v0; // eax
  int v1; // eax
  ISteamFriends *steamFriends; // [esp+4h] [ebp-4h]

  if ( g_liveSteamInitialized )
  {
    steamFriends = (ISteamFriends *)_SteamFriends();
    if ( steamFriends )
    {
      v0 = _SteamUtils();
      v1 = (*(int (__thiscall **)(int, int))(*(unsigned int *)v0 + 36))(v0, v0);
      steamFriends->ActivateGameOverlayToStore(steamFriends, v1);
    }
  }
}

void __cdecl LiveSteam_PopOverlayForSteamID(unsigned __int64 steamID)
{
  int v1; // [esp+0h] [ebp-Ch]
  CSteamID sID; // [esp+4h] [ebp-8h] BYREF

  CSteamID::CSteamID(&sID);
  sID.m_steamid.m_comp = (CSteamID::SteamID_t::SteamIDComponent_t)steamID;
  v1 = _SteamFriends();
  (*(void (__thiscall **)(int, const char *, unsigned int, unsigned int))(*(unsigned int *)v1 + 80))(
    v1,
    "steamid",
    *(unsigned int *)&sID.m_steamid.m_comp,
    *((unsigned int *)&sID.m_steamid.m_comp + 1));
}

void __cdecl LiveSteam_Init()
{
  LiveSteam_CreateSteamAppIdFile();
  g_liveSteamInitialized = 1;
  livesteam_sv_vac = _Dvar_RegisterBool("sv_vac", 1, 1u, "Enable VAC on this server");
}

void __cdecl LiveSteam_TestFriends_f()
{
  int i; // [esp+Ch] [ebp-54h]
  unsigned __int64 dwID; // [esp+10h] [ebp-50h]
  char buf[64]; // [esp+18h] [ebp-48h] BYREF
  int n; // [esp+5Ch] [ebp-4h]

  Com_Printf_NoFilter("STEAM: Firends Functionality Testing\n");
  n = LiveSteam_GetNumFriends();
  Com_Printf_NoFilter("STEAM: LiveSteam_GetNumFriends(): %d\n", n);
  while ( n-- )
  {
    LiveSteam_GetFriendPersonaName(n, buf, 64);
    Com_Printf_NoFilter("STEAM: LiveSteam_GetFriendPersonaName( %d ): %s\n", n, buf);
    i = LiveSteam_GetFriendPersonaStateByIndex(n);
    Com_Printf_NoFilter(
      "STEAM: LiveSteam_GetFriendPersonaStateByIndex( %d ): %s\n",
      n,
      &aOffline[20 * (i == 3) + 15 * (i == 2) + 8 * (i == 1)]);
    dwID = LiveSteam_GetFriendXuid(n);
    Com_Printf_NoFilter("STEAM: bdOnlineUserID is: %I64x\n", dwID);
  }
  n = LiveSteam_GetNumFriendsOnTheServer();
  Com_Printf_NoFilter("STEAM: LiveSteam_GetNumFriendsOnTheServer(): %d\n", n);
  while ( n-- )
  {
    LiveSteam_GetFriendOnTheServerPersonaName(n, buf, 64);
    Com_Printf_NoFilter("STEAM: LiveSteam_GetFriendOnTheServerPersonaName( %d ): %s\n", n, buf);
  }
}

_iobuf *LiveSteam_CreateSteamAppIdFile()
{
  _iobuf *result; // eax
  _iobuf *f; // [esp+0h] [ebp-4h]

  DeleteFileA("steam_appid.txt");
  result = fopen("noship_create_steam_appid.txt", "r");
  if ( result )
  {
    fclose(result);
    result = fopen("steam_appid.txt", "w");
    f = result;
    if ( result )
    {
      fprintf(result, "42710");
      return (_iobuf *)fclose(f);
    }
  }
  return result;
}

void __cdecl LiveSteam_Shutdown()
{
  if ( g_liveSteamInitialized )
  {
    LiveSteam_Server_Shutdown();
    LiveSteam_Achievements_Shutdown();
  }
  g_liveSteamInitialized = 0;
}

void __cdecl LiveSteam_CheckForP2PMessages()
{
  int v0; // edx
  unsigned __int8 messageBuffer[68]; // [esp+8h] [ebp-58h] BYREF
  CSteamID remoteID; // [esp+50h] [ebp-10h] BYREF
  int numMessages; // [esp+58h] [ebp-8h]
  unsigned int messagesize; // [esp+5Ch] [ebp-4h] BYREF

  numMessages = 0;
  messagesize = 0;
  CSteamID::CSteamID(&remoteID);
  while ( (unsigned __int8)LiveSteamP2P::IsDataAvailable(&s_SteamP2P, &messagesize) )
  {
    v0 = numMessages++;
    if ( v0 >= 10 )
      break;
    LiveSteamP2P::ReadPacket(&s_SteamP2P, messageBuffer, 0x40u, &messagesize, &remoteID);
    Live_DispatchP2PMessage(messageBuffer, messagesize, remoteID.m_steamid.m_unAll64Bits);
  }
}

void __cdecl LiveSteam_SendP2PMessage(unsigned __int64 uid, unsigned __int8 *payload, unsigned int payloadsize)
{
  LiveSteamP2P::SendPacketToSteamID(&s_SteamP2P, (CSteamID)uid, payload, payloadsize);
}

void __cdecl LiveSteam_Frame()
{
  LiveSteam_Achievements_Frame();
  LiveSteam_Server_Frame();
  Friends_Update();
  _SteamAPI_RunCallbacks();
}

void __cdecl LiveSteam_RunCallbacks()
{
  _SteamAPI_RunCallbacks();
}

char *__cdecl LiveSteam_GetClientPersonaName(bool shortName)
{
  int (__thiscall ***v1)(unsigned int, unsigned int); // eax
  const char *s; // [esp+4h] [ebp-4h]

  if ( !_SteamFriends() )
    return (char *)&toastPopupTitle;
  v1 = (int (__thiscall ***)(unsigned int, unsigned int))_SteamFriends();
  s = (const char *)(**v1)(v1, v1);
  if ( !shortName )
    return (char *)s;
  Com_sprintf(shorName, 0x40u, "%.16s", s);
  return shorName;
}

char __cdecl LiveSteam_DWUserNameFromSteamID(char *dwUserName)
{
  int v2; // [esp+0h] [ebp-18h]
  _BYTE v3[8]; // [esp+8h] [ebp-10h] BYREF
  unsigned __int64 steamid; // [esp+10h] [ebp-8h]

  if ( g_liveSteamInitialized )
  {
    v2 = _SteamUser();
    steamid = *(_QWORD *)(*(int (__thiscall **)(int, _BYTE *))(*(unsigned int *)v2 + 8))(v2, v3);
    sprintf_s(dwUserName, 0x40u, "%s%I64x", "steam", steamid);
    return 1;
  }
  else
  {
    *dwUserName = 0;
    return 0;
  }
}

char __cdecl LiveSteam_AuthRequestTicket(const void *authBlob, unsigned int authBlobSize)
{
  if ( !g_liveSteamInitialized )
    return 0;
  if ( (!authBlob || !authBlobSize)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\live\\live_steam.cpp",
          573,
          0,
          "%s",
          "authBlob && authBlobSize") )
  {
    __debugbreak();
  }
  LiveSteamClient::RequestEncryptedAppTicket(g_liveLocalSteamClient, authBlob, authBlobSize);
  return 1;
}

char __cdecl LiveSteam_AuthGetRequestedTicket(void *ticketBuf, unsigned int ticketBufSize, unsigned int *ticketSize)
{
  if ( g_liveLocalSteamClient->resultOnRequestEncryptedAppTicket == k_EResultOK )
    return LiveSteamClient::GetRetrievedEncryptedAppTicket(g_liveLocalSteamClient, ticketBuf, ticketBufSize, ticketSize);
  else
    return 0;
}

int __cdecl LiveSteam_Client_ConnectToSteamServer(unsigned __int64 serverID, void *authBlob, unsigned int bufferSize)
{
  unsigned int v4[6]; // [esp-14h] [ebp-24h] BYREF
  int v5; // [esp+4h] [ebp-Ch]
  unsigned int *v6; // [esp+8h] [ebp-8h]

  Com_DPrintf(14, "Steam CL_ConnectToSteamServer\n");
  v5 = _SteamUser(v4[5]);
  v6 = v4;
  return (*(int (__thiscall **)(int, void *, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, int))(*(unsigned int *)v5 + 12))(
           v5,
           authBlob,
           bufferSize,
           serverID,
           HIDWORD(serverID),
           0,
           0,
           1);
}

void __thiscall LiveSteam_Client_SteamDisconnect(void *this)
{
  _SteamUser(this);
  (*(void (__thiscall **)(unsigned int, unsigned int, unsigned int))(MEMORY[0] + 16))(0, 0, 0);
}

