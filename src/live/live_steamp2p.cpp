#include "live_steamp2p.h"

LiveSteamP2P *__thiscall LiveSteamP2P::LiveSteamP2P(LiveSteamP2P *this)
{
  this->m_OnSessionRequest.__vftable = (CCallback<LiveSteamP2P,P2PSessionRequest_t,0>_vtbl *)&CCallbackBase::`vftable';
  this->m_OnSessionRequest.m_nCallbackFlags = 0;
  this->m_OnSessionRequest.m_iCallback = 0;
  this->m_OnSessionRequest.__vftable = (CCallback<LiveSteamP2P,P2PSessionRequest_t,0>_vtbl *)&CCallback<LiveSteamP2P,P2PSessionRequest_t,0>::`vftable';
  this->m_OnSessionRequest.m_pObj = this;
  this->m_OnSessionRequest.m_Func = LiveSteamP2P::OnSessionRequest;
  if ( this && LiveSteamP2P::OnSessionRequest )
    CCallback<LiveSteamP2P,P2PSessionRequest_t,0>::Register(
      &this->m_OnSessionRequest,
      this,
      LiveSteamP2P::OnSessionRequest);
  return this;
}

void __thiscall LiveSteamP2P::SendPacketToSteamID(
        LiveSteamP2P *this,
        CSteamID remoteID,
        const void *data,
        unsigned int datasize)
{
  void (__thiscall ***v4)(unsigned int, unsigned int, unsigned int, const void *, unsigned int, int, unsigned int); // [esp+0h] [ebp-8h]

  v4 = (void (__thiscall ***)(unsigned int, unsigned int, unsigned int, const void *, unsigned int, int, unsigned int))_SteamNetworking();
  (**v4)(v4, *(unsigned int *)&remoteID.m_steamid.m_comp, *((unsigned int *)&remoteID.m_steamid.m_comp + 1), data, datasize, 2, 0);
}

int __thiscall LiveSteamP2P::IsDataAvailable(LiveSteamP2P *this, unsigned int *messageSize)
{
  int v3; // [esp+0h] [ebp-8h]

  v3 = _SteamNetworking();
  return (*(int (__thiscall **)(int, unsigned int *, unsigned int))(*(unsigned int *)v3 + 4))(v3, messageSize, 0);
}

int __thiscall LiveSteamP2P::ReadPacket(
        LiveSteamP2P *this,
        void *buf,
        unsigned int bufsize,
        unsigned int *msgsize,
        CSteamID *remoteID)
{
  int v6; // [esp+0h] [ebp-8h]

  v6 = _SteamNetworking();
  return (*(int (__thiscall **)(int, void *, unsigned int, unsigned int *, CSteamID *, unsigned int))(*(unsigned int *)v6 + 8))(
           v6,
           buf,
           bufsize,
           msgsize,
           remoteID,
           0);
}

void __thiscall LiveSteamP2P::OnSessionRequest(LiveSteamP2P *this, P2PSessionRequest_t *sessionRequest)
{
  int v2; // [esp+0h] [ebp-8h]

  Com_DPrintf(0, "Received session request from %llu\n", sessionRequest->m_steamIDRemote.m_steamid.m_unAll64Bits);
  v2 = _SteamNetworking();
  (*(void (__thiscall **)(int, unsigned int, unsigned int))(*(unsigned int *)v2 + 12))(
    v2,
    *(unsigned int *)&sessionRequest->m_steamIDRemote.m_steamid.m_comp,
    *((unsigned int *)&sessionRequest->m_steamIDRemote.m_steamid.m_comp + 1));
}

void __thiscall CCallback<LiveSteamP2P,P2PSessionRequest_t,0>::Register(
        CCallback<LiveSteamP2P,P2PSessionRequest_t,0> *this,
        LiveSteamP2P *pObj,
        void (__thiscall *func)(LiveSteamP2P *this, P2PSessionRequest_t *))
{
  if ( pObj && func )
  {
    if ( (this->m_nCallbackFlags & 1) != 0 )
      _SteamAPI_UnregisterCallback(this);
    this->m_pObj = pObj;
    this->m_Func = func;
    _SteamAPI_RegisterCallback(this, 1202);
  }
}

