#pragma once

LiveSteamClient *__thiscall LiveSteamClient::LiveSteamClient(LiveSteamClient *this);
void __thiscall LiveSteamClient::RequestEncryptedAppTicket(LiveSteamClient *this, const void *data, unsigned int size);
char __thiscall LiveSteamClient::GetRetrievedEncryptedAppTicket(
        LiveSteamClient *this,
        void *ticketBuf,
        unsigned int ticketBufSize,
        unsigned int *ticketSize);
void __thiscall LiveSteamClient::OnRequestEncryptedAppTicket(
        LiveSteamClient *this,
        EncryptedAppTicketResponse_t *pEncryptedAppTicketResponse,
        bool bIOFailure);
CCallResult<LiveSteamClient,EncryptedAppTicketResponse_t> *__thiscall CCallResult<LiveSteamClient,EncryptedAppTicketResponse_t>::CCallResult<LiveSteamClient,EncryptedAppTicketResponse_t>(
        CCallResult<LiveSteamClient,EncryptedAppTicketResponse_t> *this);
void __thiscall CCallResult<LiveSteamClient,EncryptedAppTicketResponse_t>::Run(
        CCallResult<LiveSteamClient,EncryptedAppTicketResponse_t> *this,
        EncryptedAppTicketResponse_t *pvParam);
void __thiscall CCallResult<LiveSteamClient,EncryptedAppTicketResponse_t>::Run(
        CCallResult<LiveSteamClient,EncryptedAppTicketResponse_t> *this,
        EncryptedAppTicketResponse_t *pvParam,
        bool bIOFailure,
        unsigned __int64 hSteamAPICall);
unsigned int __thiscall CCallResult<LiveSteamClient,EncryptedAppTicketResponse_t>::GetCallbackSizeBytes(
        gjk_double_sphere_t *this);
