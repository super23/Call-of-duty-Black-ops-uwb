#pragma once

bool __cdecl SV_IsConnectedToDW();
void __cdecl dwInit();
void __cdecl dwInitForController(int localControllerIndex);
void __cdecl DWDisconnectLobby(bdLobbyService *lobby);
void __cdecl dw_disconnect(int controllerIndex);
int __cdecl dwGetLogOnStatus(int localControllerIndex);
void __cdecl dwLogOnStart(int controllerIndex);
void __cdecl dwLogonSeAcquiredSteamTicket();
bool __cdecl SV_DedicatedReadCDKey(char *cdkey, int cdkeysize);
void __cdecl DW_DedicatedLogonStart(int controllerIndex);
void __cdecl DW_DedicatedLogonComplete(int controllerIndex);
const char *__cdecl dwSelectLSGAddress();
void __cdecl CL_GetSecretSauce(char *secretsauce);
void __cdecl dwLogOnComplete();
void __cdecl dwDNSCleanup(int localControllerIndex);
void __cdecl dwLobbyPump(int localControllerIndex);
void __cdecl r_PumpDemonware();
bool __cdecl dwDNSInit(char *authAddr, int controllerIndex);
int __cdecl dwDNSPump(int localControllerIndex, bdInetAddr *authAddr, bdInetAddr *lsgAddr);
void __cdecl DWDedicatedLogon();
void __cdecl DWDedicatedLobbyPump();
bool __thiscall Com_CanWriteLeaderboards(void *this);
bool __cdecl isOfficialServer(int licensetype);
bool __cdecl isOfficialRentedServer(int licensetype);
