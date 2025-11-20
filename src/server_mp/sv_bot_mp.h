#pragma once

void __cdecl SV_BotSwitchWeapon(const client_t *bot, int weapIndex);
void __cdecl SV_BotSetScriptGoal(const client_t *bot, const float *goal, float goalRadius);
void __cdecl SV_BotClearScriptGoal(const client_t *bot);
void __cdecl SV_BotSetScriptEnemy(const client_t *bot, const gentity_s *enemy);
void __cdecl SV_BotClearScriptEnemy(const client_t *bot);
void __cdecl Bot_ClearThreat(bot_info_t *botInfo);
void __cdecl SV_BotPressUseButton(const client_t *bot, int msec);
void __cdecl SV_BotSetAttacker(const client_t *bot, const gentity_s *attacker);
void __cdecl SV_BotThink(client_t *bot, usercmd_s *cmd);
int __cdecl Com_HashString(const char *fname, int len);
void __cdecl Bot_UpdateTimedAction(int *timedAction);
void __cdecl Bot_UpdateSight(bot_info_t *botInfo, const client_t *bot);
void __cdecl Bot_UpdateThreat(bot_info_t *botInfo, const client_t *bot);
bool __cdecl Bot_IsEnemyHuman(const gentity_s *enemy);
bool __cdecl Bot_IsInLastStand(const client_t *bot);
bool __cdecl Bot_IsFacingEnemy(const client_t *bot, const gentity_s *enemy, float *dot);
char __cdecl Bot_IsInRangeOfEnemy(const client_t *bot, const gentity_s *enemy, float *distSq);
void __cdecl Bot_UpdateBestThreat(bot_info_t *botInfo, bot_threat_t *threats, int numThreats);
bool __cdecl Bot_ThreatSightCheck(const client_t *bot, const gentity_s *enemy, float fovDot);
void __cdecl Bot_UpdateWeapon(const bot_info_t *botInfo, const client_t *bot, usercmd_s *cmd);
void __cdecl Bot_UpdateMovement(bot_info_t *botInfo, const client_t *bot, usercmd_s *cmd);
void __cdecl Bot_RandomInput(bot_info_t *botInfo, usercmd_s *cmd);
void __cdecl Bot_GetStrafeInput(const client_t *bot, bot_info_t *botInfo, usercmd_s *cmd);
void __cdecl Bot_RandomStrafe(const playerState_s *ps, usercmd_s *cmd);
void __cdecl Bot_UpdatePath(bot_info_t *botInfo, const client_t *bot);
char __cdecl Bot_IsMovingFoward(const bot_info_t *botInfo, const client_t *bot, const playerState_s *ps);
bool __cdecl Bot_IsThrowingGrenade(const bot_info_t *botInfo, const client_t *bot);
char __cdecl Bot_PointInGoal(const float *point, const path_t *path, float goalRadius);
bool __cdecl Bot_GetPathToGoal(const client_t *bot, const float *goal, path_t *path, bool allowMantle);
char __cdecl Bot_PathValid(const gentity_s *bot, const path_t *path);
char __cdecl Bot_IsAtNegotiationNode(bot_info_t *botInfo, const client_t *bot, path_t *path);
char __cdecl Bot_UpdateScriptGoal(bot_info_t *botInfo, const client_t *bot);
char __cdecl Bot_UpdateScriptEnemy(bot_info_t *botInfo, const client_t *bot);
void __cdecl Bot_PredictPosition(const gentity_s *ent, const playerState_s *ps, float *vPos);
void __cdecl Bot_UpdateDirection(bot_info_t *botInfo, const client_t *bot, usercmd_s *cmd);
double __cdecl Bot_UpdatePitch(float currentPitch, float targetPitch, bool forceSlow);
double __cdecl Bot_UpdateYaw(float currentYaw, float targetYaw, bool forceSlow, bot_info_t *botInfo);
void __cdecl Bot_UpdateMantle(bot_info_t *botInfo, const client_t *bot, usercmd_s *cmd);
void __cdecl Bot_UpdateSpeed(bot_info_t *botInfo, const client_t *bot, usercmd_s *cmd);
void __cdecl Bot_StrafeOnPath(const playerState_s *ps, const path_t *path, usercmd_s *cmd);
void __cdecl Vec3Rotate(const float *in, const float (*matrix)[3], float *out);
void __cdecl Bot_SetTimedAction(
        unsigned int button,
        int *timedAction,
        const dvar_s *minTime,
        const dvar_s *maxTime,
        usercmd_s *cmd,
        const usercmd_s *lastCmd);
void __cdecl Bot_UpdateStance(bot_info_t *botInfo, const client_t *bot, usercmd_s *cmd);
void __cdecl Bot_UpdateSprint(bot_info_t *botInfo, const client_t *bot, usercmd_s *cmd);
bool __cdecl Bot_IsFlashbanged(const client_t *bot);
void __cdecl Bot_UpdateReload(bot_info_t *botInfo, const client_t *bot, usercmd_s *cmd);
void __cdecl Bot_UpdateGlassSmash(const client_t *bot, usercmd_s *cmd);
void __cdecl Bot_UpdateCombat(bot_info_t *botInfo, const client_t *bot, usercmd_s *cmd);
char __cdecl Bot_UpdateMelee(bot_info_t *botInfo, const client_t *bot, usercmd_s *cmd);
char __cdecl Bot_UpdateGrenadeThrow(bot_info_t *botInfo, const client_t *bot, usercmd_s *cmd);
char __cdecl Bot_GrenadeSightTrace(const client_t *bot, float projectileSpeed, float throwPitch);
char __cdecl Bot_GrenadeInRange(
        float range,
        float height,
        const float *angles,
        const WeaponDef *weapDef,
        float *outPitch);
char __cdecl Bot_GrenadePickType(const bot_info_t *botInfo, const playerState_s *ps, OffhandSlot *slot);
char __cdecl Bot_ShouldThrowGrenade(bot_info_t *botInfo, const client_t *bot);
void __cdecl Bot_Clear(bot_info_t *botInfo);
void __cdecl SV_BotRegisterDvars();
void __cdecl Bot_DrawDebug(const ScreenPlacement *scrPlace);
void __cdecl Bot_DrawString(const ScreenPlacement *scrPlace, char *text, float x, float *y, const float *color);
void __cdecl SV_DrawBotPath();
void __cdecl Bot_DrawPath(const client_t *bot);
void __cdecl SV_DrawBotThreat();
void  Bot_DrawThreat(const gentity_s *a1@<ebp>, const client_t *bot);
const char *__cdecl SV_BotNameRandom();
char __cdecl Friends_IsInCurrentSession(const char *name);
char __cdecl SV_BotNameRandomFriend(int controller, char *buf, int bufLen);
client_t *__cdecl Bot_NameUsed(const char *name);
void __cdecl SV_BotNameUpdate(const char *name);
