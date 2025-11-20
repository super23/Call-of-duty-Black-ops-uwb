#pragma once

void __cdecl SetClientViewAngle(gentity_s *ent, const float *angle);
void __cdecl G_GetPlayerViewOrigin_Internal(const playerState_s *ps, float *origin);
void __cdecl G_GetPlayerViewOrigin(const playerState_s *ps, float *origin);
void __cdecl G_GetPlayerBodyViewOrigin(const playerState_s *ps, float *origin);
void __cdecl G_GetPlayerViewDirection(const gentity_s *ent, float *forward, float *right, float *up);
void __cdecl ClientUserinfoChanged(unsigned int clientNum);
void __cdecl ClientCleanName(const char *in, char *out, int outSize);
const char *__cdecl ClientConnect(unsigned int clientNum, unsigned int scriptPersId);
void __cdecl ClientClearFields(gclient_s *client);
void __cdecl ClientBegin(unsigned int clientNum);
void __cdecl ClientSpawn(gentity_s *ent, const float *spawn_origin, const float *spawn_angles);
clientSession_t *__thiscall clientSession_t::clientSession_t(clientSession_t *this);
void __cdecl ClientDisconnect(unsigned int clientNum);
unsigned int __cdecl G_GetNonPVSPlayerInfo(gentity_s *pSelf, float *vPosition, int iLastUpdateEnt);
void __cdecl G_BroadcastVoice(gentity_s *talker, VoicePacket_t *voicePacket);
