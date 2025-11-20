#include "live_steam_client.h"

LiveSteamClient *__thiscall LiveSteamClient::LiveSteamClient(LiveSteamClient *this)
{
  this->resultOnRequestEncryptedAppTicket = 0;
  CCallResult<LiveSteamClient,EncryptedAppTicketResponse_t>::CCallResult<LiveSteamClient,EncryptedAppTicketResponse_t>(&this->m_SteamCallResultEncryptedAppTicket);
  return this;
}

void __thiscall LiveSteamClient::RequestEncryptedAppTicket(LiveSteamClient *this, const void *data, unsigned int size)
{
  int v3; // [esp+0h] [ebp-18h]
  int v4; // [esp+4h] [ebp-14h]
  __int64 hSteamAPICall; // [esp+10h] [ebp-8h]

  this->resultOnRequestEncryptedAppTicket = 0;
  v4 = _SteamUser(v3);
  hSteamAPICall = ((__int64 (__thiscall *)(int, const void *, unsigned int))*(unsigned int *)(*(unsigned int *)v4 + 76))(
                    v4,
                    data,
                    size);
  if ( this->m_SteamCallResultEncryptedAppTicket.m_hAPICall )
    _SteamAPI_UnregisterCallResult(
      &this->m_SteamCallResultEncryptedAppTicket,
      this->m_SteamCallResultEncryptedAppTicket.m_hAPICall,
      HIDWORD(this->m_SteamCallResultEncryptedAppTicket.m_hAPICall));
  this->m_SteamCallResultEncryptedAppTicket.m_hAPICall = hSteamAPICall;
  this->m_SteamCallResultEncryptedAppTicket.m_pObj = this;
  this->m_SteamCallResultEncryptedAppTicket.m_Func = LiveSteamClient::OnRequestEncryptedAppTicket;
  if ( hSteamAPICall )
    _SteamAPI_RegisterCallResult(&this->m_SteamCallResultEncryptedAppTicket, hSteamAPICall, HIDWORD(hSteamAPICall));
}

char __thiscall LiveSteamClient::GetRetrievedEncryptedAppTicket(
        LiveSteamClient *this,
        void *ticketBuf,
        unsigned int ticketBufSize,
        unsigned int *ticketSize)
{
  int v5; // [esp+0h] [ebp-Ch]
  int v6; // [esp+0h] [ebp-Ch]
  char result; // [esp+Bh] [ebp-1h]

  if ( (this->resultOnRequestEncryptedAppTicket != k_EResultOK || !ticketBufSize)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\live\\live_steam_client.cpp",
          26,
          0,
          "%s",
          "resultOnRequestEncryptedAppTicket == k_EResultOK && ticketBufSize") )
  {
    __debugbreak();
  }
  v6 = _SteamUser(v5);
  result = (*(int (__thiscall **)(int, void *, unsigned int, unsigned int *))(*(unsigned int *)v6 + 80))(
             v6,
             ticketBuf,
             ticketBufSize,
             ticketSize);
  Com_DPrintf(30, "STEAM: Retrieved ticket from Steam, sending to DemonWare\n");
  return result;
}

void __thiscall LiveSteamClient::OnRequestEncryptedAppTicket(
        LiveSteamClient *this,
        EncryptedAppTicketResponse_t *pEncryptedAppTicketResponse,
        bool bIOFailure)
{
  EResult result; // [esp+4h] [ebp-4h]

  if ( !bIOFailure )
  {
    result = pEncryptedAppTicketResponse->m_eResult;
    if ( pEncryptedAppTicketResponse->m_eResult == k_EResultOK )
    {
      dwLogonSeAcquiredSteamTicket();
    }
    else
    {
      switch ( result )
      {
        case k_EResultLimitExceeded:
          Com_DPrintf(30, "STEAM: Calling RequestEncryptedAppTicket more than once per minute returns this error\n");
          break;
        case k_EResultDuplicateRequest:
          Com_DPrintf(
            30,
            "STEAM: Calling RequestEncryptedAppTicket while there is already a pending request results in this error\n");
          break;
        case k_EResultNoConnection:
          Com_DPrintf(
            30,
            "STEAM: Calling RequestEncryptedAppTicket while not connected to steam results in this error\n");
          break;
      }
    }
    this->resultOnRequestEncryptedAppTicket = result;
  }
}

CCallResult<LiveSteamClient,EncryptedAppTicketResponse_t> *__thiscall CCallResult<LiveSteamClient,EncryptedAppTicketResponse_t>::CCallResult<LiveSteamClient,EncryptedAppTicketResponse_t>(
        CCallResult<LiveSteamClient,EncryptedAppTicketResponse_t> *this)
{
  this->__vftable = (CCallResult<LiveSteamClient,EncryptedAppTicketResponse_t>_vtbl *)&CCallbackBase::`vftable';
  this->m_nCallbackFlags = 0;
  this->m_iCallback = 0;
  this->__vftable = (CCallResult<LiveSteamClient,EncryptedAppTicketResponse_t>_vtbl *)&CCallResult<LiveSteamClient,EncryptedAppTicketResponse_t>::`vftable';
  this->m_hAPICall = 0;
  this->m_pObj = 0;
  this->m_Func = 0;
  this->m_iCallback = 154;
  return this;
}

void __thiscall CCallResult<LiveSteamClient,EncryptedAppTicketResponse_t>::Run(
        CCallResult<LiveSteamClient,EncryptedAppTicketResponse_t> *this,
        EncryptedAppTicketResponse_t *pvParam)
{
  this->m_hAPICall = 0;
  this->m_Func(this->m_pObj, pvParam, 0);
}

void __thiscall CCallResult<LiveSteamClient,EncryptedAppTicketResponse_t>::Run(
        CCallResult<LiveSteamClient,EncryptedAppTicketResponse_t> *this,
        EncryptedAppTicketResponse_t *pvParam,
        bool bIOFailure,
        unsigned __int64 hSteamAPICall)
{
  if ( hSteamAPICall == this->m_hAPICall )
  {
    this->m_hAPICall = 0;
    this->m_Func(this->m_pObj, pvParam, bIOFailure);
  }
}

unsigned int __thiscall CCallResult<LiveSteamClient,EncryptedAppTicketResponse_t>::GetCallbackSizeBytes(
        gjk_double_sphere_t *this)
{
  return 4;
}

