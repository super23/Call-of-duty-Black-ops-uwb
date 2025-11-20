#pragma once

unsigned int __thiscall CCallback<LiveSteamServer,GSPolicyResponse_t,1>::GetCallbackSizeBytes(gjk_aabb_t *this);
int __thiscall CCallback<LiveSteamServer,GSClientApprove_t,1>::GetCallbackSizeBytes(
        CCallback<LiveSteamP2P,P2PSessionRequest_t,0> *this);
LiveSteamServer *__thiscall LiveSteamServer::LiveSteamServer(LiveSteamServer *this);
void __thiscall LiveSteamServer::~LiveSteamServer(LiveSteamServer *this);
void __thiscall LiveSteamServer::RunFrame(LiveSteamServer *this);
void __thiscall LiveSteamServer::OnSteamServersConnected(LiveSteamServer *this, SteamServersConnected_t *pLogonSuccess);
void __thiscall LiveSteamServer::OnSteamServersDisconnected(
        LiveSteamServer *this,
        SteamServersDisconnected_t *pLoggedOff);
void __thiscall LiveSteamServer::OnPolicyResponse(LiveSteamServer *this, GSPolicyResponse_t *pPolicyResponse);
void __thiscall LiveSteamServer::OnGSClientApprove(LiveSteamServer *this, GSClientApprove_t *pGSClientApprove);
void __cdecl KickClientFromSteamGameServer(CSteamID clientID, EDenyReason reason);
void __thiscall LiveSteamServer::OnGSClientDeny(LiveSteamServer *this, GSClientDeny_t *pGSClientDeny);
void __thiscall LiveSteamServer::OnGSClientKick(LiveSteamServer *this, GSClientKick_t *pGSClientKick);
void __cdecl LiveSteam_Server_Init();
void __cdecl LiveSteam_Server_Shutdown();
void __cdecl LiveSteam_Server_Frame();
void __cdecl LiveSteam_Server_RunCallbacks();
void __cdecl LiveSteam_Server_ClientSteamAuthentication(
        unsigned __int64 clientSteamID,
        netadr_t netAddr,
        void *authBlob,
        unsigned int authBlobSize);
void __cdecl LiveSteam_Server_ClientSteamDisconnect(unsigned __int64 clientSteamID);
void __thiscall CCallback<LiveSteamServer,SteamServersDisconnected_t,1>::Run(
        CCallback<LiveSteamP2P,P2PSessionRequest_t,0> *this,
        P2PSessionRequest_t *pvParam,
        bool __formal,
        unsigned __int64 a4);
int __thiscall CCallback<LiveSteamServer,GSClientDeny_t,1>::GetCallbackSizeBytes(
        CCallback<LiveSteamServer,GSClientDeny_t,1> *this);
int __thiscall CCallback<LiveSteamServer,GSClientKick_t,1>::GetCallbackSizeBytes(
        CCallback<LiveSteamServer,GSClientKick_t,1> *this);
void __thiscall CCallback<LiveSteamServer,SteamServersConnected_t,1>::Register(
        CCallback<LiveSteamServer,SteamServersConnected_t,1> *this,
        LiveSteamServer *pObj,
        void (__thiscall *func)(LiveSteamServer *this, SteamServersConnected_t *));
void __thiscall CCallback<LiveSteamServer,SteamServersDisconnected_t,1>::Register(
        CCallback<LiveSteamServer,SteamServersDisconnected_t,1> *this,
        LiveSteamServer *pObj,
        void (__thiscall *func)(LiveSteamServer *this, SteamServersDisconnected_t *));
void __thiscall CCallback<LiveSteamServer,GSPolicyResponse_t,1>::Register(
        CCallback<LiveSteamServer,GSPolicyResponse_t,1> *this,
        LiveSteamServer *pObj,
        void (__thiscall *func)(LiveSteamServer *this, GSPolicyResponse_t *));
void __thiscall CCallback<LiveSteamServer,GSClientApprove_t,1>::Register(
        CCallback<LiveSteamServer,GSClientApprove_t,1> *this,
        LiveSteamServer *pObj,
        void (__thiscall *func)(LiveSteamServer *this, GSClientApprove_t *));
void __thiscall CCallback<LiveSteamServer,GSClientDeny_t,1>::Register(
        CCallback<LiveSteamServer,GSClientDeny_t,1> *this,
        LiveSteamServer *pObj,
        void (__thiscall *func)(LiveSteamServer *this, GSClientDeny_t *));
void __thiscall CCallback<LiveSteamServer,GSClientKick_t,1>::Register(
        CCallback<LiveSteamServer,GSClientKick_t,1> *this,
        LiveSteamServer *pObj,
        void (__thiscall *func)(LiveSteamServer *this, GSClientKick_t *));
