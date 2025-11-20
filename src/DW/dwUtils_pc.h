#pragma once

char __cdecl dwGetOnlineUserID(int controllerIndex, unsigned __int64 *userID);
char __cdecl dwGetOnlineUserName(int controllerIndex, char *buf, unsigned int bufsize);
void __cdecl dwXnaddrtonetadr(unsigned __int8 *xnaddr, netadr_t *privadr, netadr_t *pubadr);
unsigned int __cdecl SV_SendClientChallenge(unsigned int nonce, unsigned int challenge, unsigned __int64 userID);
void __cdecl CL_HandleSVDWChallenge(unsigned int nonce, unsigned int serverchallenge, unsigned int clientchallenge);
unsigned int __cdecl Com_GametypeToInt(const char *gametype);
void __cdecl Com_IntToGametype(char *gametype_out, unsigned int gametype_in);
