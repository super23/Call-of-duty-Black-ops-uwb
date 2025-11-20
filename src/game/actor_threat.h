#pragma once

bool __fastcall Actor_CheckIgnore(sentient_s *self, sentient_s *enemy);
void __cdecl Actor_InitThreatBiasGroups();
int __fastcall Actor_GetThreatBias(int groupSelf, int groupEnemy);
void __fastcall Actor_FlagEnemyUnattackable(actor_s *self);
bool __fastcall Actor_CaresAboutInfo(actor_s *self, sentient_s *pOther);
double __cdecl Sentient_GetScarinessForDistance(sentient_s *self, sentient_s *enemy, float fDist);
void __fastcall Actor_InitThreatUpdateInterval(actor_s *self);
void __fastcall Actor_UpdateThreat(actor_s *self);
void __cdecl DebugThreatStringSimple(const actor_s *self, gentity_s *enemy, char *string, const float *color);
int __fastcall Actor_UpdateSingleThreat(actor_s *self, sentient_s *enemy);
void __cdecl DebugResetThreatStrings(const actor_s *self);
void __cdecl DebugSetThreatString(ThreatDebugStringCategory category, int threat);
void __cdecl DebugSetThreatStringFromString(ThreatDebugStringCategory category, const char *string);
void __cdecl DebugThreatStringAll(const actor_s *self, sentient_s *enemy, int threat);
int __fastcall Actor_ThreatFromScariness(float fScariness);
int __fastcall Actor_ThreatFromDistance(float fDistance);
bool __fastcall Actor_CanBePathedTo(actor_s *self, sentient_s *enemy, int isCurrentEnemy);
bool __fastcall Actor_IsFullyAware(actor_s *self, sentient_s *enemy, int isCurrentEnemy);
void __cdecl DebugThreatNodes(
        const actor_s *self,
        sentient_s *enemy,
        pathnode_t *selfNode,
        pathnode_t *enemyNode,
        const float *color);
int __fastcall Actor_ThreatFromVisibilityAndAwareness(int isVisible, int isFullyAware, int friendlyTimingOut);
int __fastcall Actor_ThreatFromAttackerCount(actor_s *self, sentient_s *enemy, int isCurrentEnemy);
bool __fastcall Actor_ShouldIgnoreFromAttackerCount(actor_s *self, sentient_s *enemy, int isCurrentEnemy);
int __fastcall Actor_ThreatBonusForCurrentEnemy(
        int isCurrentEnemy,
        int isFullyAware,
        int friendlyTimingOut,
        int isPlayer,
        int isDamaged);
int __fastcall Actor_ThreatCoveringFire(actor_s *self, sentient_s *enemy);
int __fastcall Actor_ThreatFlashed(sentient_s *enemy);
void __fastcall Actor_IncrementThreatTime(actor_s *self);
void __fastcall Actor_SetPotentialThreat(potential_threat_t *self, float yaw);
void __fastcall Actor_ClearPotentialThreat(potential_threat_t *self);
void __cdecl Actor_PotentialThreat_Debug(actor_s *self);
