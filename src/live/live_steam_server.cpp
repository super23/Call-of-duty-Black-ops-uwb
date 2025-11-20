#include "live_steam_server.h"

unsigned int __thiscall CCallback<LiveSteamServer,GSPolicyResponse_t,1>::GetCallbackSizeBytes(gjk_aabb_t *this)
{
  return 1;
}

int __thiscall CCallback<LiveSteamServer,GSClientApprove_t,1>::GetCallbackSizeBytes(
        CCallback<LiveSteamP2P,P2PSessionRequest_t,0> *this)
{
  return 8;
}

LiveSteamServer *__thiscall LiveSteamServer::LiveSteamServer(LiveSteamServer *this)
{
  this->currentMapDlcAppID = 0;
  this->m_CallbackSteamServersConnected.__vftable = (CCallback<LiveSteamServer,SteamServersConnected_t,1>_vtbl *)&CCallbackBase::`vftable';
  this->m_CallbackSteamServersConnected.m_nCallbackFlags = 0;
  this->m_CallbackSteamServersConnected.m_iCallback = 0;
  this->m_CallbackSteamServersConnected.__vftable = (CCallback<LiveSteamServer,SteamServersConnected_t,1>_vtbl *)&CCallback<LiveSteamServer,SteamServersConnected_t,1>::`vftable';
  this->m_CallbackSteamServersConnected.m_pObj = this;
  this->m_CallbackSteamServersConnected.m_Func = LiveSteamServer::OnSteamServersConnected;
  if ( this && LiveSteamServer::OnSteamServersConnected )
    CCallback<LiveSteamServer,SteamServersConnected_t,1>::Register(
      &this->m_CallbackSteamServersConnected,
      this,
      LiveSteamServer::OnSteamServersConnected);
  this->m_CallbackSteamServersDisconnected.__vftable = (CCallback<LiveSteamServer,SteamServersDisconnected_t,1>_vtbl *)&CCallbackBase::`vftable';
  this->m_CallbackSteamServersDisconnected.m_nCallbackFlags = 0;
  this->m_CallbackSteamServersDisconnected.m_iCallback = 0;
  this->m_CallbackSteamServersDisconnected.__vftable = (CCallback<LiveSteamServer,SteamServersDisconnected_t,1>_vtbl *)&CCallback<LiveSteamServer,SteamServersDisconnected_t,1>::`vftable';
  this->m_CallbackSteamServersDisconnected.m_pObj = this;
  this->m_CallbackSteamServersDisconnected.m_Func = LiveSteamServer::OnSteamServersDisconnected;
  if ( this && LiveSteamServer::OnSteamServersDisconnected )
    CCallback<LiveSteamServer,SteamServersDisconnected_t,1>::Register(
      &this->m_CallbackSteamServersDisconnected,
      this,
      LiveSteamServer::OnSteamServersDisconnected);
  this->m_CallbackPolicyResponse.__vftable = (CCallback<LiveSteamServer,GSPolicyResponse_t,1>_vtbl *)&CCallbackBase::`vftable';
  this->m_CallbackPolicyResponse.m_nCallbackFlags = 0;
  this->m_CallbackPolicyResponse.m_iCallback = 0;
  this->m_CallbackPolicyResponse.__vftable = (CCallback<LiveSteamServer,GSPolicyResponse_t,1>_vtbl *)&CCallback<LiveSteamServer,GSPolicyResponse_t,1>::`vftable';
  this->m_CallbackPolicyResponse.m_pObj = this;
  this->m_CallbackPolicyResponse.m_Func = LiveSteamServer::OnPolicyResponse;
  if ( this && LiveSteamServer::OnPolicyResponse )
    CCallback<LiveSteamServer,GSPolicyResponse_t,1>::Register(
      &this->m_CallbackPolicyResponse,
      this,
      LiveSteamServer::OnPolicyResponse);
  this->m_CallbackGSClientApprove.__vftable = (CCallback<LiveSteamServer,GSClientApprove_t,1>_vtbl *)&CCallbackBase::`vftable';
  this->m_CallbackGSClientApprove.m_nCallbackFlags = 0;
  this->m_CallbackGSClientApprove.m_iCallback = 0;
  this->m_CallbackGSClientApprove.__vftable = (CCallback<LiveSteamServer,GSClientApprove_t,1>_vtbl *)&CCallback<LiveSteamServer,GSClientApprove_t,1>::`vftable';
  this->m_CallbackGSClientApprove.m_pObj = this;
  this->m_CallbackGSClientApprove.m_Func = LiveSteamServer::OnGSClientApprove;
  if ( this && LiveSteamServer::OnGSClientApprove )
    CCallback<LiveSteamServer,GSClientApprove_t,1>::Register(
      &this->m_CallbackGSClientApprove,
      this,
      LiveSteamServer::OnGSClientApprove);
  this->m_CallbackGSClientDeny.__vftable = (CCallback<LiveSteamServer,GSClientDeny_t,1>_vtbl *)&CCallbackBase::`vftable';
  this->m_CallbackGSClientDeny.m_nCallbackFlags = 0;
  this->m_CallbackGSClientDeny.m_iCallback = 0;
  this->m_CallbackGSClientDeny.__vftable = (CCallback<LiveSteamServer,GSClientDeny_t,1>_vtbl *)&CCallback<LiveSteamServer,GSClientDeny_t,1>::`vftable';
  this->m_CallbackGSClientDeny.m_pObj = this;
  this->m_CallbackGSClientDeny.m_Func = LiveSteamServer::OnGSClientDeny;
  if ( this && LiveSteamServer::OnGSClientDeny )
    CCallback<LiveSteamServer,GSClientDeny_t,1>::Register(
      &this->m_CallbackGSClientDeny,
      this,
      LiveSteamServer::OnGSClientDeny);
  this->m_CallbackGSClientKick.__vftable = (CCallback<LiveSteamServer,GSClientKick_t,1>_vtbl *)&CCallbackBase::`vftable';
  this->m_CallbackGSClientKick.m_nCallbackFlags = 0;
  this->m_CallbackGSClientKick.m_iCallback = 0;
  this->m_CallbackGSClientKick.__vftable = (CCallback<LiveSteamServer,GSClientKick_t,1>_vtbl *)&CCallback<LiveSteamServer,GSClientKick_t,1>::`vftable';
  this->m_CallbackGSClientKick.m_pObj = this;
  this->m_CallbackGSClientKick.m_Func = LiveSteamServer::OnGSClientKick;
  if ( this && LiveSteamServer::OnGSClientKick )
    CCallback<LiveSteamServer,GSClientKick_t,1>::Register(
      &this->m_CallbackGSClientKick,
      this,
      LiveSteamServer::OnGSClientKick);
  this->serverIP = 0;
  this->serverActive = 0;
  return this;
}

void __thiscall LiveSteamServer::~LiveSteamServer(LiveSteamServer *this)
{
  this->m_CallbackGSClientKick.__vftable = (CCallback<LiveSteamServer,GSClientKick_t,1>_vtbl *)&CCallback<LiveSteamServer,GSClientKick_t,1>::`vftable';
  if ( (this->m_CallbackGSClientKick.m_nCallbackFlags & 1) != 0 )
    _SteamAPI_UnregisterCallback(&this->m_CallbackGSClientKick);
  this->m_CallbackGSClientDeny.__vftable = (CCallback<LiveSteamServer,GSClientDeny_t,1>_vtbl *)&CCallback<LiveSteamServer,GSClientDeny_t,1>::`vftable';
  if ( (this->m_CallbackGSClientDeny.m_nCallbackFlags & 1) != 0 )
    _SteamAPI_UnregisterCallback(&this->m_CallbackGSClientDeny);
  this->m_CallbackGSClientApprove.__vftable = (CCallback<LiveSteamServer,GSClientApprove_t,1>_vtbl *)&CCallback<LiveSteamServer,GSClientApprove_t,1>::`vftable';
  if ( (this->m_CallbackGSClientApprove.m_nCallbackFlags & 1) != 0 )
    _SteamAPI_UnregisterCallback(&this->m_CallbackGSClientApprove);
  this->m_CallbackPolicyResponse.__vftable = (CCallback<LiveSteamServer,GSPolicyResponse_t,1>_vtbl *)&CCallback<LiveSteamServer,GSPolicyResponse_t,1>::`vftable';
  if ( (this->m_CallbackPolicyResponse.m_nCallbackFlags & 1) != 0 )
    _SteamAPI_UnregisterCallback(&this->m_CallbackPolicyResponse);
  this->m_CallbackSteamServersDisconnected.__vftable = (CCallback<LiveSteamServer,SteamServersDisconnected_t,1>_vtbl *)&CCallback<LiveSteamServer,SteamServersDisconnected_t,1>::`vftable';
  if ( (this->m_CallbackSteamServersDisconnected.m_nCallbackFlags & 1) != 0 )
    _SteamAPI_UnregisterCallback(&this->m_CallbackSteamServersDisconnected);
  this->m_CallbackSteamServersConnected.__vftable = (CCallback<LiveSteamServer,SteamServersConnected_t,1>_vtbl *)&CCallback<LiveSteamServer,SteamServersConnected_t,1>::`vftable';
  if ( (this->m_CallbackSteamServersConnected.m_nCallbackFlags & 1) != 0 )
    _SteamAPI_UnregisterCallback(&this->m_CallbackSteamServersConnected);
}

void __thiscall LiveSteamServer::RunFrame(LiveSteamServer *this)
{
  char *v1; // eax
  const char *v2; // eax
  const char *v3; // eax
  int v4; // [esp+0h] [ebp-30h]
  unsigned __int64 serverSteamID; // [esp+8h] [ebp-28h]
  _BYTE v7[8]; // [esp+14h] [ebp-1Ch] BYREF
  client_t *client; // [esp+1Ch] [ebp-14h]
  unsigned int RUN_INTERVAL; // [esp+20h] [ebp-10h]
  unsigned int currentTime; // [esp+24h] [ebp-Ch]
  int i; // [esp+28h] [ebp-8h]
  int MAX_STEAM_AUTH_FAIL_COUNT; // [esp+2Ch] [ebp-4h]

  MAX_STEAM_AUTH_FAIL_COUNT = 4;
  RUN_INTERVAL = 10000;
  if ( this->serverActive )
  {
    currentTime = Sys_Milliseconds();
    if ( currentTime - this->lastRunTime >= 0x2710 )
    {
      this->lastRunTime = currentTime;
      for ( i = 0; i < com_maxclients->current.integer; ++i )
      {
        client = &svs.clients[i];
        if ( client->header.state >= 5 && !client->bIsTestClient && !client->bIsDemoClient && !client->steamAuthorized )
        {
          if ( ++client->steamAuthFailCount == 4 )
          {
            v1 = va("Kicking '%s' <%llx> in runframe, unable to Steam authorize\n", client->name, client->steamID);
            Com_PrintError(15, v1);
            v2 = va("Kicking '%s' <%llx> in runframe, unable to Steam authorize\n", client->name, client->steamID);
            SV_SysLog_LogMessage(0, v2);
            v3 = va("clientkick %d PLATFORM_STEAM_CONNECT_FAIL\n", i);
            Cbuf_AddText(0, v3);
            client->steamAuthFailCount = 0;
          }
          else
          {
            Com_DPrintf(
              0,
              "Sending steam auth request to client <%llx>, fail count is now %i\n",
              client->steamID,
              client->steamAuthFailCount);
            v4 = _SteamGameServer();
            serverSteamID = *(_QWORD *)(*(int (__thiscall **)(int, _BYTE *))(*(unsigned int *)v4 + 16))(v4, v7);
            SV_SteamAuthClientRequest(client->header.netchan.remoteAddress, serverSteamID);
          }
        }
      }
    }
  }
}

void __thiscall LiveSteamServer::OnSteamServersConnected(LiveSteamServer *this, SteamServersConnected_t *pLogonSuccess)
{
  bdAddr *SecurityKey; // eax
  bdAddr *Address; // eax
  bdAddr *v4; // eax
  bdReference<bdCommonAddr> localAddr; // [esp+1Ch] [ebp-8h] BYREF
  unsigned int n; // [esp+20h] [ebp-4h] BYREF

  Com_Printf(23, "STEAM: SteamServer connected to Steam successfully\n");
  dwGetLocalCommonAddr(&localAddr);
  SecurityKey = (bdAddr *)bdGameInfo::getSecurityKey((bdGameInfo *)localAddr.m_ptr);
  Address = bdAddr::getAddress(SecurityKey);
  bdInetAddr::serialize(&Address->m_address, (unsigned __int8 *)&this->serverIP, 4u, 0, &n);
  v4 = (bdAddr *)bdGameInfo::getSecurityKey((bdGameInfo *)localAddr.m_ptr);
  bdAddr::getPort(v4);
  bdReference<bdRemoteTask>::~bdReference<bdRemoteTask>(&localAddr);
}

void __thiscall LiveSteamServer::OnSteamServersDisconnected(
        LiveSteamServer *this,
        SteamServersDisconnected_t *pLoggedOff)
{
  int i; // [esp+4h] [ebp-8h]
  client_t *client; // [esp+8h] [ebp-4h]

  this->serverActive = 0;
  Com_PrintError(15, "STEAM: SteamServer got logged out of Steam\n");
  SV_SysLog_LogMessage(0, "STEAM: SteamServer got logged out of Steam\n");
  for ( i = 0; i < com_maxclients->current.integer; ++i )
  {
    client = &svs.clients[i];
    if ( client->header.state >= 5 && !client->bIsTestClient && !client->bIsDemoClient && !client->steamAuthorized )
      client->steamAuthFailCount = 0;
  }
}

void __thiscall LiveSteamServer::OnPolicyResponse(LiveSteamServer *this, GSPolicyResponse_t *pPolicyResponse)
{
  int v2; // eax

  this->serverActive = 1;
  this->lastRunTime = 0;
  v2 = _SteamGameServer();
  if ( (*(unsigned __int8 (__thiscall **)(int, int))(*(unsigned int *)v2 + 12))(v2, v2) )
    Com_Printf(23, "STEAM: SteamServer is VAC Secure!\n");
  else
    Com_Printf(23, "STEAM: SteamServer is not VAC Secure!\n");
}

void __thiscall LiveSteamServer::OnGSClientApprove(LiveSteamServer *this, GSClientApprove_t *pGSClientApprove)
{
  const char *v2; // eax
  char *v3; // eax
  const char *v4; // eax
  const char *v5; // eax
  int v6; // [esp+4h] [ebp-30h]
  int v7; // [esp+8h] [ebp-2Ch]
  int v8; // [esp+Ch] [ebp-28h]
  client_t *client; // [esp+2Ch] [ebp-8h]
  int i; // [esp+30h] [ebp-4h]

  if ( _SteamFriends() )
  {
    v8 = _SteamFriends();
    v2 = (const char *)(*(int (__thiscall **)(int, unsigned int, unsigned int))(*(unsigned int *)v8 + 28))(
                         v8,
                         *(unsigned int *)&pGSClientApprove->m_SteamID.m_steamid.m_comp,
                         *((unsigned int *)&pGSClientApprove->m_SteamID.m_steamid.m_comp + 1));
    Com_PrintWarning(15, "STEAM: Approved client with SteamPersonaName: %s\n", v2);
  }
  else
  {
    Com_PrintWarning(
      15,
      "STEAM: Approved client with SteamID: %I64d\n",
      pGSClientApprove->m_SteamID.m_steamid.m_unAll64Bits);
  }
  for ( i = 0; i < com_maxclients->current.integer; ++i )
  {
    client = &svs.clients[i];
    if ( client->header.state >= 3 )
    {
      if ( this->currentMapDlcAppID )
      {
        v7 = _SteamGameServer();
        if ( (*(int (__thiscall **)(int, unsigned int, unsigned int, unsigned int))(*(unsigned int *)v7 + 72))(
               v7,
               *(unsigned int *)&pGSClientApprove->m_SteamID.m_steamid.m_comp,
               *((unsigned int *)&pGSClientApprove->m_SteamID.m_steamid.m_comp + 1),
               this->currentMapDlcAppID) )
        {
          v3 = va("kicking client %s, no license\n", client->name);
          Com_PrintError(15, v3);
          v4 = va("kicking client %s, no license\n", client->name);
          SV_SysLog_LogMessage(0, v4);
          KickClientFromSteamGameServer(pGSClientApprove->m_SteamID, k_EDenyNoLicense);
          return;
        }
        if ( _SteamFriends() )
        {
          v6 = _SteamFriends();
          v5 = (const char *)(*(int (__thiscall **)(int, unsigned int, unsigned int))(*(unsigned int *)v6 + 28))(
                               v6,
                               *(unsigned int *)&pGSClientApprove->m_SteamID.m_steamid.m_comp,
                               *((unsigned int *)&pGSClientApprove->m_SteamID.m_steamid.m_comp + 1));
          Com_PrintWarning(15, "STEAM: Approved client for DLC, SteamPersonaName: %s\n", v5);
        }
        else
        {
          Com_PrintWarning(
            15,
            "STEAM: Approved client DLC, SteamID: %I64d\n",
            pGSClientApprove->m_SteamID.m_steamid.m_unAll64Bits);
        }
      }
      if ( LODWORD(client->steamID) == pGSClientApprove->m_SteamID.m_steamid.m_comp
        && HIDWORD(client->steamID) == *((unsigned int *)&pGSClientApprove->m_SteamID.m_steamid.m_comp + 1) )
      {
        client->steamAuthorized = 1;
        return;
      }
    }
  }
}

void __cdecl KickClientFromSteamGameServer(CSteamID clientID, EDenyReason reason)
{
  char *v2; // eax
  const char *v3; // eax
  const char *v4; // eax
  const char *v5; // eax
  const char *v6; // eax
  int v7; // [esp+4h] [ebp-10h]
  client_t *client; // [esp+Ch] [ebp-8h]
  int i; // [esp+10h] [ebp-4h]

  if ( reason != k_EDenySteamValidationStalled )
  {
    for ( i = 0; i < com_maxclients->current.integer; ++i )
    {
      client = &svs.clients[i];
      if ( client->header.state >= 3 && client->steamID == clientID.m_steamid.m_comp )
      {
        v7 = _SteamGameServer();
        (*(void (__thiscall **)(int, unsigned int, unsigned int))(*(unsigned int *)v7 + 28))(
          v7,
          *(unsigned int *)&clientID.m_steamid.m_comp,
          *((unsigned int *)&clientID.m_steamid.m_comp + 1));
        v2 = va(
               "STEAM: kicking '%s' <%llx> for reason: %d in KickClientFromSteamGameServer\n",
               client->name,
               client->steamID,
               reason);
        Com_PrintError(15, v2);
        v3 = va(
               "STEAM: kicking '%s' <%llx> for reason: %d in KickClientFromSteamGameServer\n",
               client->name,
               client->steamID,
               reason);
        SV_SysLog_LogMessage(0, v3);
        switch ( reason )
        {
          case k_EDenyNotLoggedOn:
          case k_EDenyNoLicense:
          case k_EDenyLoggedInElseWhere:
            v4 = va("clientkick %d PLATFORM_STEAM_AUTH_DENIED\n", i);
            Cbuf_AddText(0, v4);
            break;
          case k_EDenyCheater:
            v5 = va("tempBanClient %d PLATFORM_STEAM_KICK_CHEAT\n", i);
            Cbuf_AddText(0, v5);
            break;
          default:
            v6 = va("clientkick %d PLATFORM_STEAM_CONNECT_FAIL\n", i);
            Cbuf_AddText(0, v6);
            break;
        }
        return;
      }
    }
  }
}

void __thiscall LiveSteamServer::OnGSClientDeny(LiveSteamServer *this, GSClientDeny_t *pGSClientDeny)
{
  char *v2; // eax
  const char *v3; // eax

  v2 = va(
         "STEAM: Denying <%llx> for reason: %d\n",
         pGSClientDeny->m_SteamID.m_steamid.m_unAll64Bits,
         pGSClientDeny->m_eDenyReason);
  Com_PrintError(15, v2);
  v3 = va(
         "STEAM: Denying <%llx> for reason: %d\n",
         pGSClientDeny->m_SteamID.m_steamid.m_unAll64Bits,
         pGSClientDeny->m_eDenyReason);
  SV_SysLog_LogMessage(0, v3);
  KickClientFromSteamGameServer(pGSClientDeny->m_SteamID, pGSClientDeny->m_eDenyReason);
}

void __thiscall LiveSteamServer::OnGSClientKick(LiveSteamServer *this, GSClientKick_t *pGSClientKick)
{
  Com_PrintError(
    0,
    "Steam kicking <%llx> for reason: %d\n",
    pGSClientKick->m_SteamID.m_steamid.m_unAll64Bits,
    pGSClientKick->m_eDenyReason);
  KickClientFromSteamGameServer(pGSClientKick->m_SteamID, pGSClientKick->m_eDenyReason);
}

void __cdecl LiveSteam_Server_Init()
{
  LiveSteamServer *v0; // [esp+0h] [ebp-18h]
  void (__thiscall ***v1)(unsigned int, unsigned int); // [esp+4h] [ebp-14h]
  LiveSteamServer *v2; // [esp+8h] [ebp-10h]

  if ( LiveSteam_IsInitialized() && !g_liveSteamServer )
  {
    if ( !(unsigned __int8)_SteamGameServer_Init(0, 8766, 27015, 0, 27016, 3, &toastPopupTitle, "1.0.0.0") )
      Com_PrintError(0, "ERROR: STEAM: SteamGameServer_Init call failed\n");
    if ( _SteamMasterServerUpdater() )
    {
      v1 = (void (__thiscall ***)(unsigned int, unsigned int))_SteamMasterServerUpdater();
      (**v1)(v1, 0);
    }
    v2 = (LiveSteamServer *)operator new(0x88u);
    if ( v2 )
      v0 = LiveSteamServer::LiveSteamServer(v2);
    else
      v0 = 0;
    g_liveSteamServer = v0;
  }
}

void __cdecl LiveSteam_Server_Shutdown()
{
  int v0; // [esp+4h] [ebp-10h]
  void (__thiscall ***v1)(unsigned int, unsigned int); // [esp+8h] [ebp-Ch]
  LiveSteamServer *v2; // [esp+10h] [ebp-4h]

  if ( g_liveSteamServer )
  {
    v1 = (void (__thiscall ***)(unsigned int, unsigned int))_SteamMasterServerUpdater();
    (**v1)(v1, 0);
    v0 = _SteamGameServer();
    (*(void (__thiscall **)(int))(*(unsigned int *)v0 + 4))(v0);
    _SteamGameServer_Shutdown();
    v2 = g_liveSteamServer;
    if ( g_liveSteamServer )
    {
      LiveSteamServer::~LiveSteamServer(g_liveSteamServer);
      operator delete(v2);
    }
    g_liveSteamServer = 0;
  }
}

void __cdecl LiveSteam_Server_Frame()
{
  if ( g_liveSteamServer )
    LiveSteamServer::RunFrame(g_liveSteamServer);
}

void __cdecl LiveSteam_Server_RunCallbacks()
{
  _SteamGameServer_RunCallbacks();
}

void __cdecl LiveSteam_Server_ClientSteamAuthentication(
        unsigned __int64 clientSteamID,
        netadr_t netAddr,
        void *authBlob,
        unsigned int authBlobSize)
{
  int v4; // [esp+0h] [ebp-18h]
  unsigned int clientIP; // [esp+8h] [ebp-10h]
  CSteamID steamID; // [esp+Ch] [ebp-Ch] BYREF

  CSteamID::CSteamID(&steamID);
  if ( g_liveSteamServer )
  {
    if ( netAddr.type == NA_LOOPBACK )
    {
      clientIP = g_liveSteamServer->serverIP;
    }
    else
    {
      if ( netAddr.type != NA_IP )
        return;
      clientIP = _byteswap_ulong(*(unsigned int *)netAddr.ip);
    }
    v4 = _SteamGameServer();
    if ( (*(unsigned __int8 (__thiscall **)(int, unsigned int, void *, unsigned int, CSteamID *))(*(unsigned int *)v4 + 20))(
           v4,
           clientIP,
           authBlob,
           authBlobSize,
           &steamID) )
    {
      if ( clientSteamID != steamID.m_steamid.m_comp
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\live\\live_steam_server.cpp",
              431,
              0,
              "%s",
              "clientSteamID == steamID.ConvertToUint64()") )
      {
        __debugbreak();
      }
    }
    else
    {
      Com_PrintError(
        15,
        "ERROR: STEAM: Dev print - Unable to connect user %llu from IP: %u.%u.%u.%u:%u to steam server\n",
        clientSteamID,
        netAddr.ip[0],
        netAddr.ip[1],
        netAddr.ip[2],
        netAddr.ip[3],
        netAddr.port);
    }
  }
}

void __cdecl LiveSteam_Server_ClientSteamDisconnect(unsigned __int64 clientSteamID)
{
  int v1; // eax
  CSteamID clientID; // [esp+4h] [ebp-8h] BYREF

  if ( g_liveSteamServer )
  {
    clientID.m_steamid.m_comp = (CSteamID::SteamID_t::SteamIDComponent_t)clientSteamID;
    if ( CSteamID::IsValid(&clientID) )
    {
      v1 = _SteamGameServer();
      (*(void (__thiscall **)(int, unsigned int, unsigned int))(*(unsigned int *)v1 + 28))(
        v1,
        *(unsigned int *)&clientID.m_steamid.m_comp,
        *((unsigned int *)&clientID.m_steamid.m_comp + 1));
    }
  }
}

void __thiscall CCallback<LiveSteamServer,SteamServersDisconnected_t,1>::Run(
        CCallback<LiveSteamP2P,P2PSessionRequest_t,0> *this,
        P2PSessionRequest_t *pvParam,
        bool __formal,
        unsigned __int64 a4)
{
  this->m_Func(this->m_pObj, pvParam);
}

int __thiscall CCallback<LiveSteamServer,GSClientDeny_t,1>::GetCallbackSizeBytes(
        CCallback<LiveSteamServer,GSClientDeny_t,1> *this)
{
  return 140;
}

int __thiscall CCallback<LiveSteamServer,GSClientKick_t,1>::GetCallbackSizeBytes(
        CCallback<LiveSteamServer,GSClientKick_t,1> *this)
{
  return 12;
}

void __thiscall CCallback<LiveSteamServer,SteamServersConnected_t,1>::Register(
        CCallback<LiveSteamServer,SteamServersConnected_t,1> *this,
        LiveSteamServer *pObj,
        void (__thiscall *func)(LiveSteamServer *this, SteamServersConnected_t *))
{
  if ( pObj && func )
  {
    if ( (this->m_nCallbackFlags & 1) != 0 )
      _SteamAPI_UnregisterCallback(this);
    this->m_nCallbackFlags |= 2u;
    this->m_pObj = pObj;
    this->m_Func = func;
    _SteamAPI_RegisterCallback(this, 101);
  }
}

void __thiscall CCallback<LiveSteamServer,SteamServersDisconnected_t,1>::Register(
        CCallback<LiveSteamServer,SteamServersDisconnected_t,1> *this,
        LiveSteamServer *pObj,
        void (__thiscall *func)(LiveSteamServer *this, SteamServersDisconnected_t *))
{
  if ( pObj && func )
  {
    if ( (this->m_nCallbackFlags & 1) != 0 )
      _SteamAPI_UnregisterCallback(this);
    this->m_nCallbackFlags |= 2u;
    this->m_pObj = pObj;
    this->m_Func = func;
    _SteamAPI_RegisterCallback(this, 103);
  }
}

void __thiscall CCallback<LiveSteamServer,GSPolicyResponse_t,1>::Register(
        CCallback<LiveSteamServer,GSPolicyResponse_t,1> *this,
        LiveSteamServer *pObj,
        void (__thiscall *func)(LiveSteamServer *this, GSPolicyResponse_t *))
{
  if ( pObj && func )
  {
    if ( (this->m_nCallbackFlags & 1) != 0 )
      _SteamAPI_UnregisterCallback(this);
    this->m_nCallbackFlags |= 2u;
    this->m_pObj = pObj;
    this->m_Func = func;
    _SteamAPI_RegisterCallback(this, 115);
  }
}

void __thiscall CCallback<LiveSteamServer,GSClientApprove_t,1>::Register(
        CCallback<LiveSteamServer,GSClientApprove_t,1> *this,
        LiveSteamServer *pObj,
        void (__thiscall *func)(LiveSteamServer *this, GSClientApprove_t *))
{
  if ( pObj && func )
  {
    if ( (this->m_nCallbackFlags & 1) != 0 )
      _SteamAPI_UnregisterCallback(this);
    this->m_nCallbackFlags |= 2u;
    this->m_pObj = pObj;
    this->m_Func = func;
    _SteamAPI_RegisterCallback(this, 201);
  }
}

void __thiscall CCallback<LiveSteamServer,GSClientDeny_t,1>::Register(
        CCallback<LiveSteamServer,GSClientDeny_t,1> *this,
        LiveSteamServer *pObj,
        void (__thiscall *func)(LiveSteamServer *this, GSClientDeny_t *))
{
  if ( pObj && func )
  {
    if ( (this->m_nCallbackFlags & 1) != 0 )
      _SteamAPI_UnregisterCallback(this);
    this->m_nCallbackFlags |= 2u;
    this->m_pObj = pObj;
    this->m_Func = func;
    _SteamAPI_RegisterCallback(this, 202);
  }
}

void __thiscall CCallback<LiveSteamServer,GSClientKick_t,1>::Register(
        CCallback<LiveSteamServer,GSClientKick_t,1> *this,
        LiveSteamServer *pObj,
        void (__thiscall *func)(LiveSteamServer *this, GSClientKick_t *))
{
  if ( pObj && func )
  {
    if ( (this->m_nCallbackFlags & 1) != 0 )
      _SteamAPI_UnregisterCallback(this);
    this->m_nCallbackFlags |= 2u;
    this->m_pObj = pObj;
    this->m_Func = func;
    _SteamAPI_RegisterCallback(this, 203);
  }
}

