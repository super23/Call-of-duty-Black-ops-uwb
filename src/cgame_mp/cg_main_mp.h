#pragma once

unsigned __int8 *__cdecl Hunk_AllocXAnimCreate(unsigned int size);
int __cdecl CG_GetClientNumForLocalClient(int localClientNum);
bool __cdecl CG_IsRagdollTrajectory(const trajectory_t *trajectory);
void __cdecl CG_SetupSplitscreenDvars();
void __cdecl CG_RegisterDvars();
int __cdecl CG_AllocateClientMemory_SizeRequired(int maxLocalClients);
void __cdecl CG_AllocateClientMemory(HunkUser *hunk, int maxLocalClients);
void __cdecl CG_FreeClientMemory(HunkUser *hunk, int maxLocalClients);
void __cdecl CG_GetDObjOrientation(int localClientNum, int dobjHandle, float (*axis)[3], float *origin);
const playerState_s *__cdecl CG_GetPredictedPlayerState(int localClientNum);
void __cdecl CG_GameMessage(int localClientNum, const char *msg);
void __cdecl CG_BoldGameMessage(int localClientNum, const char *msg, int duration);
bool __cdecl CG_IsVehicleMayhemGameType();
void __cdecl CG_RegisterSounds();
void __cdecl CG_RegisterSurfaceTypeSounds(const char *pszType, unsigned int *sound);
void __cdecl CG_RegisterImpactTypeSounds(const char *pszType, unsigned int *sound);
void __cdecl CG_PlayBattleChatter(
        int localClientNum,
        int entitynum,
        float *origin,
        unsigned int firstSoundAlias,
        int secondSoundAlias);
void __cdecl CG_CheckBattleChatter();
void __cdecl CG_RestartSmokeGrenades(int localClientNum);
void __cdecl CG_InitVote(int localClientNum);
unsigned __int16 __cdecl CG_GetWeaponAttachBone(clientInfo_t *ci, weapType_t weapType, weapInventoryType_t invType);
void __cdecl CGScr_LoadGameTypeScript(scriptInstance_t inst, const char *gametype, ScriptFunctions *functions);
char __cdecl CGScr_LoadScriptAndLabel(
        scriptInstance_t inst,
        const char *filename,
        const char *label,
        ScriptFunctions *functions);
void __cdecl CGScr_LoadScripts(const char *mapname, const char *gametype, ScriptFunctions *functions);
void __cdecl CGScr_LoadLevelScript(scriptInstance_t inst, const char *mapname, ScriptFunctions *functions);
void __cdecl CGScr_LoadClientScripts(const char *mapname, ScriptFunctions *functions);
void __cdecl CGScr_LoadClientScriptsAndAnims();
void __cdecl CG_InitScreenDimensions(int localClientNum);
int __cdecl CG_GetClientNum(int localClientNum);
bool __cdecl CG_IsMature();
char __cdecl CG_GetEntityOriginAngles(int localClientNum, int entityNum, float *origin, float *angles);
unsigned __int16 __cdecl CG_GetVehicleTypeString(int clientNum, int entityNum);
int __cdecl CachedTag_GetCachedTagPos(
        const centity_s *ent,
        cached_client_tag_t *cachedTag,
        unsigned int tagName,
        float *pos,
        int updateInterval,
        bool forceUpdate);
int __cdecl CachedTag_UpdateTagInternal(
        const centity_s *ent,
        cached_client_tag_t *cachedTag,
        unsigned int tagName,
        int updateInterval,
        bool forceUpdate);
void __cdecl CachedTag_CalcTagPos(const centity_s *ent, cached_client_tag_t *cachedTag, float *pos);
int __cdecl CachedTag_NoCache_GetTagPos(const centity_s *ent, unsigned int tagName, float *pos);
int __cdecl CachedTag_GetTagPos(
        const centity_s *ent,
        unsigned int tagName,
        float *pos,
        int updateInterval,
        bool forceUpdate);
void __cdecl CG_InitClientEntityCaches(int localClientNum);
void __cdecl CG_FreeClientEntityCaches(int localClientNum);
void __cdecl CG_Init(int localClientNum, int serverMessageNum, int serverCommandSequence, int clientNum);
clientConnection_t *__cdecl CL_GetLocalClientConnection(int localClientNum);
void __cdecl CG_RegisterGraphics(int localClientNum, const char *mapname);
void __cdecl CG_LoadHudMenu(int localClientNum);
const rectDef_s *__cdecl Window_GetRect(const windowDef_t *w);
unsigned __int16 __cdecl CG_AttachWeapon(DObjModel_s *dobjModels, unsigned __int16 numModels, clientInfo_t *ci);
void __cdecl CG_CreateDObj(
        DObjModel_s *dobjModels,
        unsigned __int16 numModels,
        XAnimTree_s *tree,
        int handle,
        int localClientNum,
        clientInfo_t *ci);
DObj *__cdecl CG_GetDObj(unsigned int handle, int localClientNum);
void __cdecl CG_InitEntities(int localClientNum);
void __cdecl CG_InitViewDimensions(int localClientNum);
void __cdecl CG_InitDof(GfxDepthOfField *dof);
int CGScr_LoadScriptsAndAnims();
void __cdecl CG_LoadAnimTrees(int localClientNum, cg_s *cgameGlob, const char *mapname, bool loading_scripts);
void __cdecl CG_LoadAnimTreeInstances(int localClientNum);
void __cdecl CG_SetupGameInformation(int localClientNum);
void __cdecl CG_Shutdown(int localClientNum);
void __cdecl CG_FreeAnimTreeInstances(int localClientNum);
void __cdecl CG_ShutdownOnceForAllClients();
void __cdecl CG_ProcessTriggerDebug(centity_s *ent, trigger_info_t *trigger_info);
int __cdecl CG_NotifyTriggers();
void __cdecl CG_Trigger(centity_s *self, centity_s *other);
void __cdecl CG_multi_trigger(centity_s *ent);
void __cdecl CG_Touch_Multi(centity_s *self, centity_s *other);
unsigned __int8 *__cdecl Hunk_AllocXAnimClient(unsigned int size);
