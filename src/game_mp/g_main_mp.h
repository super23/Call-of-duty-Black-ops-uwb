#pragma once

int __cdecl G_GetTime();
void __cdecl G_RegisterConnectPaths();
bool __cdecl G_OnlyConnectingPaths();
void __cdecl G_RegisterRegisterToolDvars();
int __cdecl G_GetSavePersist();
void __cdecl G_SetSavePersist(int savepersist);
double __cdecl G_GetFogOpaqueDistSqrd();
int __cdecl G_GetClientScore(unsigned int clientNum);
int __cdecl G_GetClientKills(unsigned int clientNum);
int __cdecl G_GetClientAssists(unsigned int clientNum);
int __cdecl G_GetClientRank(unsigned int clientNum);
int __cdecl G_GetClientPrestige(unsigned int clientNum);
team_t __cdecl G_GetClientTeam(unsigned int clientNum);
int __cdecl G_GetClientDeaths(unsigned int clientNum);
int __cdecl G_GetClientArchiveTime(unsigned int clientNum);
void __cdecl G_SetClientArchiveTime(unsigned int clientNum, int time);
MatchState *__cdecl G_GetMatchState();
clientState_s *__cdecl G_GetClientState(unsigned int clientNum);
gclient_s *__cdecl G_GetPlayerState(unsigned int clientNum);
int __cdecl G_GetClientSize();
void __cdecl G_FreeEntities(bool clearTargets);
unsigned __int8 *__cdecl Hunk_AllocXAnimServer(unsigned int size);
bool __cdecl G_ExitAfterConnectPaths();
bool __cdecl G_ExitAfterToolComplete();
bool __cdecl G_ExitOnComError(int code);
int __cdecl G_IsServerGameSystem(int clientNum);
unsigned __int16 __cdecl G_GetWeaponAttachBone(clientInfo_t *ci, weapType_t weapType, weapInventoryType_t invType);
void G_FreeAnimTreeInstances();
void __cdecl  G_InitGame(int levelTime, int randomSeed, int restart, int registerDvars);
const dvar_s *G_RegisterDvars();
void __cdecl G_CreateDObj(
        DObjModel_s *dobjModels,
        unsigned __int16 numModels,
        XAnimTree_s *tree,
        unsigned int handle,
        int unusedLocalClientNum);
DObj *__cdecl G_GetDObj(unsigned int handle, int unusedLocalClientNum);
void __cdecl G_SafeDObjFree(unsigned int handle, int unusedLocalClientNum);
XAnim_s *G_LoadAnimTreeInstances();
unsigned __int8 *__cdecl Hunk_AllocActorXAnimServer(unsigned int size);
void G_PrintAllFastFileErrors();
void __cdecl G_PrintFastFileErrors(char *fastfile);
unsigned __int16 __cdecl G_AttachWeapon(DObjModel_s *dobjModels, unsigned __int16 numModels, clientInfo_t *ci);
void __cdecl G_ShutdownGame(int freeScripts);
void __cdecl CalculateRanks();
int __cdecl SortRanks(int *a, int *b);
void __cdecl ExitLevel();
void G_LogPrintf(const char *fmt, ...);
void __cdecl CheckVote();
void __cdecl G_UpdateObjectiveToClients();
void __cdecl G_UpdateHudElemsToClients();
void __cdecl G_RunThink(gentity_s *ent);
void __cdecl DebugDumpAnims();
void __cdecl G_XAnimUpdateEnt(gentity_s *ent);
void __cdecl ScriptPump();
void __cdecl ShowEntityInfo();
void __cdecl G_UpdateIKPlayerClipTerrainTimeout(gentity_s *ent);
void __cdecl G_UpdateIKDisableTerrainMappingTimeout(gentity_s *ent);
void __cdecl G_UpdateTimedDamage(gentity_s *ent);
void __cdecl G_DebugTimedDamage();
void __cdecl G_UpdateClientLinkInfo(gentity_s *ent);
bool __cdecl ResolveParentClientMask(const gentity_s *entChild, gentity_s *entParent);
void  G_RunFrame(__m128 a1@<xmm0>, int levelTime);
void __cdecl G_ClientDoPerFrameNotifies(gentity_s *ent);
bool __cdecl DoPerFrameNotify(
        gentity_s *ent,
        bool isCurrently,
        bool wasPreviously,
        unsigned __int16 begin,
        unsigned __int16 end);
void __cdecl G_UpdateIKCulling(gentity_s *ent);
void __cdecl G_RunFrameForEntity(gentity_s *ent);
void  G_UpdateWeapons(__m128 a1@<xmm0>, gentity_s *ent);
int G_PopulateMatchState();
bool __cdecl G_IsEntWalkable(int localClientNum, int entityNum);
char __cdecl G_GetEntityOriginAngles(int localClientNum, int entityNum, float *origin, float *angles);
void __cdecl G_EntityLinkFromPMove(unsigned int entityNum, int parentEntityNum, int tagName);
void __cdecl G_AddDebugString(const float *xyz, const float *color, float scale, char *pszText, int duration);
gclient_s *__thiscall gclient_s::gclient_s(gclient_s *this);
void __cdecl G_SafeDObjFree(gentity_s *ent);
