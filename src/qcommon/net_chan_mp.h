#pragma once

char *__cdecl NET_AdrToString(netadr_t a);
char *__cdecl NET_AdrToStringDW(netadr_t a);
void __cdecl NetProf_PrepProfiling(netProfileInfo_t *prof);
void __cdecl NetProf_AddPacket(netProfileStream_t *pProfStream, int iSize, int bFragment);
void __cdecl NetProf_NewSendPacket(netchan_t *pChan, int iSize, int bFragment);
void __cdecl NetProf_NewRecievePacket(netchan_t *pChan, int iSize, int bFragment);
void __cdecl NetProf_UpdateStatistics(netProfileStream_t *pStream);
void __cdecl Net_DisplayProfile(int localClientNum);
void __cdecl Netchan_Init(__int16 port);
void __cdecl Net_Dump//Profile_f();
void __cdecl Net_GetQPort_f();
void __cdecl Net_SetQPort_f();
void __cdecl Netchan_Setup(
        netsrc_t sock,
        netchan_t *chan,
        netadr_t adr,
        int qport,
        char *outgoingBuffer,
        int outgoingBufferSize,
        char *incomingBuffer,
        int incomingBufferSize);
int __cdecl Netchan_TransmitFragment(
        netchan_t *chan,
        int fragmentLength,
        int fragmentIndex,
        unsigned __int8 maxFragmentIndex);
bool __cdecl Netchan_TransmitNextFragment(netchan_t *chan);
bool __cdecl Netchan_Transmit(netchan_t *chan, int length, char *data, bool reliable_fragments);
int __cdecl Netchan_Process(netchan_t *chan, msg_t *msg);
int __cdecl NET_CompareBaseAdrSigned(netadr_t *a, netadr_t *b);
bool __cdecl NET_CompareBaseAdr(netadr_t a, netadr_t b);
int __cdecl NET_CompareAdrSigned(netadr_t *a, netadr_t *b);
bool __cdecl NET_CompareAdr(netadr_t a, netadr_t b);
bool __cdecl NET_IsLocalAddress(netadr_t adr);
void __cdecl NET_SetNetAdrLoopbackIP(netadr_t *src);
int __cdecl NET_GetClientPacket(netadr_t *net_from, msg_t *net_message);
int __cdecl NET_GetLoopPacket_Real(netsrc_t sock, netadr_t *net_from, msg_t *net_message);
int __cdecl NET_GetLoopPacket(netsrc_t sock, netadr_t *net_from, msg_t *net_message);
void __cdecl NET_SendLoopPacket(netsrc_t sock, unsigned int length, unsigned __int8 *data, netadr_t to);
void __cdecl NET_DeferPacketToClient(netadr_t *net_from, msg_t *net_message);
char __cdecl NET_GetDeferredClientPacket(netadr_t *net_from, msg_t *net_message);
char __cdecl NET_SendPacket(netsrc_t sock, unsigned int length, unsigned __int8 *data, netadr_t to);
bool __cdecl NET_OutOfBandPrint(netsrc_t sock, netadr_t adr, const char *data);
bool __cdecl NET_OutOfBandData(netsrc_t sock, netadr_t adr, const unsigned __int8 *format, int len);
bool __cdecl NET_OutOfBandVoiceData(netsrc_t sock, netadr_t adr, unsigned __int8 *format, unsigned int len);
int __cdecl NET_StringToAdr(const char *s, netadr_t *a);
void __cdecl Int64ToString(__int64 int64, char *str);
__int64 __cdecl StringToInt64(const char *str);
void __cdecl XUIDToString(unsigned __int64 *xuid, char *str);
void __cdecl StringToXUID(const char *str, unsigned __int64 *xuid);
