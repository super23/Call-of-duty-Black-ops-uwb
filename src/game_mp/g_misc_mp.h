#pragma once

void __cdecl misc_EntInfo(gentity_s *self, float *source);
void __cdecl EntinfoPosAndScale(gentity_s *self, float *source, float *pos, float *textScale, float *dist);
void __cdecl EntInfo_Vehicle(gentity_s *self, float *source);
void __cdecl EntInfo_Item(gentity_s *self, float *source);
double __cdecl G_GetEntInfoScale();
void __cdecl SP_info_notnull(gentity_s *self);
void __cdecl SP_light(gentity_s *self, SpawnVar *spawnVar);
void __cdecl TeleportPlayer(gentity_s *player, float *origin, float *angles);
void __cdecl ScrNotify_FaceEvent(gentity_s *self, unsigned __int16 face_event);
