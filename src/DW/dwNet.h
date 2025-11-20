#pragma once

void __cdecl dwRegisterRawInterceptor(bdPacketInterceptor *rawinterceptor);
int __cdecl dwRawSendTo(netadr_t *adr, unsigned __int8 *data, unsigned int size);
int __cdecl dwSendSysLogPacket(netadr_t *adr, void *data, unsigned int size);
void __cdecl dwNetStart(bool online);
void __thiscall bdNetStartParams::~bdNetStartParams(bdNetStartParams *this);
void __cdecl dwNetStop();
void __cdecl dwNetPump();
void dwCheckConnections();
void __cdecl dwHandleLostConnection(bdReference<bdAddrHandle> addrHandle);
void __cdecl dwClearAddrHandle(bdReference<bdRemoteTask> *countedAddrHandle);
void __cdecl dwCreateSecIDAndKey(bdSecurityID *id, bdSecurityKey *key);
char __cdecl dwRegisterSecIDAndKey(bdSecurityID *id, bdSecurityKey *key);
int __cdecl dwSendTo(unsigned int length, unsigned __int8 *data, netadr_t to);
int __cdecl dwRecvFrom(netadr_t *const from, unsigned __int8 *data, unsigned int size);
bool __cdecl dwRegisterAddrHandle(
        bdReference<bdAddrHandle> addrHandle,
        bdSecurityID *secID,
        unsigned int *index,
        bool *newHandle);
unsigned int __cdecl dwGetNumFreeAddrHandleSlots();
bool __cdecl dwCommonAddrToNetadr(netadr_t *const adr, bool *commonAddrBuf, bdCommonAddr *secID);
bdReference<bdAddrHandle> *__cdecl dwAddrHandleIndexToAddrHandle(
        bdReference<bdAddrHandle> *result,
        int addrHandleIndex);
bdReference<bdAddrHandle> *__cdecl dwNetadrToAddrHandle(bdReference<bdAddrHandle> *result, const netadr_t *const adr);
unsigned int __cdecl dwGetNumAddrHandles();
bool __cdecl dwCloseConnection(netadr_t *const adr);
int __cdecl dwGetLoopbackIndex();
bool __cdecl dwAddrIndexToAddrString(int addrHandleIndex, char *const str, unsigned int size);
int bdSnprintf(char *buf, unsigned int maxlen, const char *format, ...);
taskCompleteResults __cdecl dwGetAddrHandleConnectionTaskStatus(int addrHandleIndex);
void __thiscall bdArray<bdString>::clear(bdArray<bdString> *this);
void __thiscall bdArray<bdString>::pushBack(bdArray<bdString> *this, const bdString *value);
void __thiscall bdArray<bdInetAddr>::pushBack(bdArray<bdInetAddr> *this, const bdInetAddr *value);
void __thiscall bdArray<bdString>::increaseCapacity(bdArray<bdString> *this, unsigned int increase);
void __thiscall bdArray<bdInetAddr>::increaseCapacity(bdArray<bdInetAddr> *this, unsigned int increase);
void __thiscall bdArray<bdInetAddr>::clear(bdArray<bdInetAddr> *this);
bool __thiscall dwRawInterceptor::acceptPacket(
        dwRawInterceptor *this,
        bdSocket *socket,
        bdAddr addr,
        char *data,
        unsigned int size,
        unsigned __int8 type);
char __cdecl checkRconAddr(bdAddr fromAddr);
SecID *__thiscall SecID::SecID(SecID *this);
