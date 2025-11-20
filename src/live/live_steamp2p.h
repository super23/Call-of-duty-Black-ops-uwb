#pragma once

LiveSteamP2P *__thiscall LiveSteamP2P::LiveSteamP2P(LiveSteamP2P *this);
void __thiscall LiveSteamP2P::SendPacketToSteamID(
        LiveSteamP2P *this,
        CSteamID remoteID,
        const void *data,
        unsigned int datasize);
int __thiscall LiveSteamP2P::IsDataAvailable(LiveSteamP2P *this, unsigned int *messageSize);
int __thiscall LiveSteamP2P::ReadPacket(
        LiveSteamP2P *this,
        void *buf,
        unsigned int bufsize,
        unsigned int *msgsize,
        CSteamID *remoteID);
void __thiscall LiveSteamP2P::OnSessionRequest(LiveSteamP2P *this, P2PSessionRequest_t *sessionRequest);
void __thiscall CCallback<LiveSteamP2P,P2PSessionRequest_t,0>::Register(
        CCallback<LiveSteamP2P,P2PSessionRequest_t,0> *this,
        LiveSteamP2P *pObj,
        void (__thiscall *func)(LiveSteamP2P *this, P2PSessionRequest_t *));
