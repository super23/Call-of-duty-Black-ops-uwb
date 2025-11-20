#pragma once

bool __cdecl SV_VoiceEnabled();
void __cdecl SV_SendClientVoiceData(client_t *client);
void __cdecl SV_WriteVoiceDataToClient(client_t *client, msg_t *msg);
bool __cdecl SV_ClientWantsVoiceData(unsigned int clientNum);
bool __cdecl SV_ClientHasClientMuted(unsigned int listener, unsigned int talker);
void __cdecl SV_QueueVoicePacket(int talkerNum, int clientNum, VoicePacket_t *voicePacket);
void __cdecl SV_UserVoice(client_t *cl, msg_t *msg);
void __cdecl SV_PreGameUserVoice(client_t *cl, msg_t *msg);
