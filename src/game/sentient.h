#pragma once

sentient_s *__cdecl Sentient_Alloc();
void __cdecl Sentient_Free(sentient_s *sentient);
void __cdecl Sentient_Dissociate(sentient_s *pSentient);
void __cdecl Sentient_DissociateSentient(sentient_s *self, sentient_s *other);
void __fastcall Sentient_GetOrigin(const sentient_s *self, float *vOriginOut);
void __fastcall Sentient_GetVelocity(const sentient_s *self, float *vVelOut);
void __fastcall Sentient_GetEyePosition(const sentient_s *self, float *vEyePosOut);
void __fastcall Sentient_GetThirdPersonEyePosition(const sentient_s *self, float *vEyePosOut);
void __fastcall Sentient_GetDebugEyePosition(const sentient_s *self, float *vEyePosOut);
pathnode_t *__fastcall Sentient_NearestNode(sentient_s *self);
pathnode_t *__fastcall Sentient_NearestNodeSuppressed(
        sentient_s *self,
        float (*vNormal)[2],
        float *fDist,
        int iPlaneCount);
void __fastcall Sentient_InvalidateNearestNode(sentient_s *self);
void __fastcall Sentient_SetEnemy(sentient_s *self, gentity_s *enemy, int bNotify);
sentient_s *__fastcall Sentient_FirstSentient(int iTeamFlags);
sentient_s *__fastcall Sentient_NextSentient(sentient_s *pPrevSentient, int iTeamFlags);
const char *__fastcall Sentient_NameForTeam(unsigned int eTeam);
void __fastcall Sentient_SetTeam(sentient_s *self, team_t eTeam);
void __fastcall Sentient_ClaimNode(sentient_s *self, pathnode_t *node);
void __fastcall Sentient_NodeClaimRevoked(sentient_s *self, pathnode_t *node);
void __fastcall Sentient_StealClaimNode(sentient_s *self, sentient_s *other);
void __fastcall Sentient_BanNearNodes(sentient_s *self);
void __cdecl G_InitSentients();
bool __fastcall Sentient_NearestNodeDirty(sentient_s *self, bool originChanged);
