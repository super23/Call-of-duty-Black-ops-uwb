#pragma once

int __cdecl G_GetPlayerCorpseIndex(gentity_s *ent, const char *error_msg);
int __cdecl G_GetFreePlayerCorpseIndex();
void __cdecl PlayerCorpse_Free(gentity_s *ent);
void __cdecl G_RunCorpseMove(gentity_s *ent);
void __cdecl G_BounceCorpse(gentity_s *ent, corpseInfo_t *corpseInfo, trace_t *trace, float *endpos);
char __cdecl G_GetAnimDeltaForCorpse(gentity_s *ent, float *originChange);
void __cdecl G_RunCorpse(gentity_s *ent);
void __cdecl G_RunCorpseAnimate(gentity_s *ent);
