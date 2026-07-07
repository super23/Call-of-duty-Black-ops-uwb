// SP link stubs for MP-only live/demo/UI symbols referenced from shared code.
#include <universal/dvar.h>
#include <universal/q_shared.h>
#include <live/live_sessions_win.h>
#include <live/live_stats.h>
#include <live/live_pcache.h>
#include <live/live_storage.h>
#include <demo/demo_version.h>
#include <qcommon/msg_mp.h>
#include <game_mp/g_spawnsystem_mp.h>
#include "g_main_sp.h"
#include <ui_sp/ui_gametype_custom_sp.h>
#include <ui/ui_shared.h>
#include <gfx_d3d/r_material.h>
#include <universal/com_math_anglevectors.h>
#include <algorithm>
#include <cmath>
#include <cstring>

double __cdecl G_crandom();

SessionData_s g_serverSession;
ddlDef_t *g_statsDDL;
unsigned __int64 s_selectedMetPlayerXUID;
unsigned __int64 s_selectedPlayerXUID;

namespace
{
    constexpr int SP_MAX_SPAWN_POINTS = 200;
    constexpr int SP_MAX_SPAWN_TEAMS = 3;
    constexpr int SP_ALL_TEAM_MASK = 7;
    constexpr float SP_LEGACY_SPAWN_POINT_RAISE_HEIGHT = 35.0f;

    SpawnPoint s_spSpawnPoints[SP_MAX_SPAWN_POINTS];
    SortedPointArray s_spSortedSpawnPoints[SP_MAX_SPAWN_TEAMS];
    int s_spSpawnPointCount = 0;
    float s_spSpawnRandomVariation = 0.0f;

    int SpawnSystem_FindPointByEntNum_SP(int entNum)
    {
        for ( int i = 0; i < s_spSpawnPointCount; ++i )
        {
            if ( s_spSpawnPoints[i].entNum == entNum )
                return i;
        }

        return -1;
    }

    void SpawnSystem_SetPointBaseWeight_SP(SpawnPoint *point, char teammask, float weight)
    {
        if ( !point )
            return;

        if ( (teammask & 1) != 0 )
            point->baseWeight[0] = weight;
        if ( (teammask & 2) != 0 )
            point->baseWeight[1] = weight;
        if ( (teammask & 4) != 0 )
            point->baseWeight[2] = weight;
    }
}

bool __cdecl Demo_GetAnalyzePrintDataType(int) { return false; }
bool __cdecl Demo_GetClipPausedState(void) { return false; }
bool __cdecl Demo_GetJumpTimeFlag(void) { return false; }
bool __cdecl Demo_GetSwitchedPlayersFlag(void) { return false; }
bool __cdecl Demo_IsClipModified(void) { return false; }
bool __cdecl Demo_IsClipPlaying(void) { return false; }
bool __cdecl Demo_IsClipPreviewRunning(void) { return false; }
bool __cdecl Demo_IsCompleted(void) { return false; }
bool __cdecl Demo_IsDemoHudHidden(void) { return false; }
bool __cdecl Demo_IsEnabled(void) { return false; }
bool __cdecl Demo_IsErrorDialogOpen(int) { return false; }
bool __cdecl Demo_IsGameHudHidden(void) { return false; }
bool __cdecl Demo_IsIdle(void) { return false; }
bool __cdecl Demo_IsMovieCamera(void) { return false; }
bool __cdecl Demo_IsPaused(void) { return false; }
bool __cdecl Demo_IsPlaybackInited(void) { return false; }
bool __cdecl Demo_IsPlaying(void) { return false; }
bool __cdecl Demo_IsRecording(void) { return false; }
bool __cdecl Demo_IsRenderingMovie(void) { return false; }
bool __cdecl Demo_IsSaveDialogOpen(int) { return false; }
bool __cdecl Demo_IsShutdownInProgress(void) { return false; }
bool __cdecl Demo_IsThirdPersonCamera(void) { return false; }
bool __cdecl Demo_ShouldBuildDemoSnapshot(void) { return false; }
bool __cdecl Demo_ShouldEntityBeVisibleToClient(int) { return false; }
bool __cdecl Demo_ShouldUpdateMovieCamera(int) { return false; }
bool __cdecl Demo_ShouldUpdateViewAngles(int) { return false; }
bool __cdecl Live_IsCombatTrainingMatch(void) { return false; }
bool __cdecl Live_IsPublicOnlineMatch(void) { return false; }
bool __cdecl Live_IsSignedIn(int) { return true; }
bool __cdecl Live_IsSignedInToLive(void) { return true; }
bool __cdecl Live_IsUserSignedInToDemonware(int) { return true; }
bool __cdecl Live_IsUserSignedInToLive(void) { return true; }
bool __cdecl Live_IsWagerMatch(void) { return false; }
bool __cdecl LiveContracts_IsContractLocked(int,int) { return false; }
bool __cdecl LiveStats_GetBasicTrainingState(unsigned char *) { return false; }
bool __cdecl LiveStats_GetChallengeInfo(struct challengeInfo_t * *,unsigned int,int) { return false; }
bool __cdecl LiveStats_GetChallengeInfoParam(enum sortedChallengeParams_t,unsigned int,char const * *,int *,int) { return false; }
bool __cdecl LiveStats_IsStableStatsBufferInitialized(int) { return false; }
bool __cdecl LiveStats_WritePrestigeToStats(int) { return false; }
bool __cdecl LiveSteam_LaunchOtherApp(char const *) { return false; }
bool __cdecl LiveStorage_DoWeHaveAllStats(int) { return true; }
bool __cdecl LiveStorage_DoWeHaveContracts(void) { return true; }
bool __cdecl LiveStorage_DoWeHavePlaylists(void) { return true; }
bool __cdecl LiveStorage_FetchingOnlineWAD(void) { return false; }
bool __cdecl LiveStorage_FirstTimeRunning(void) { return false; }
bool __cdecl LiveStorage_GetStatsWriteNeeded(int,enum statsLocation) { return false; }
bool __cdecl LiveStorage_IsTimeSynced(void) { return true; }
bool __cdecl PCache_TouchComponent(struct PCacheComponent *) { return false; }
bool __cdecl Session_IsHost(struct SessionData_s *,int clientNum)
{
    if ( IsDedicatedServer() )
        return false;
    if ( Demo_IsEnabled() )
        return clientNum == 1;
    return clientNum == 0;
}
bool __cdecl StringBeginsWith(char const *prefix,char const *name)
{
    if ( !prefix || !name )
        return false;

    const size_t prefixLen = std::strlen(prefix);
    return std::strncmp(name, prefix, prefixLen) == 0;
}
bool __cdecl UI_Project_OverrideCursorPos(int,int,struct itemDef_s *) { return false; }
char * __cdecl Clan_GetName(int) { return 0; }
char * __cdecl Demo_GetAuthor(void) { return 0; }
char * __cdecl Demo_GetDuration(void) { return 0; }
char * __cdecl Demo_GetSaveScreenDescription(char const *) { return 0; }
char * __cdecl Demo_GetSaveScreenName(char const *) { return 0; }
char * __cdecl Demo_GetTimeInfo(void) { return 0; }
char * __cdecl Demo_GetTitleDescription(void) { return 0; }
char * __cdecl Demo_GetTitleName(void) { return 0; }
char * __cdecl LB_FeederItemText(int,unsigned int,int,struct Material * *) { return 0; }
char * __cdecl Live_FileShare_GetFilterList(void) { return 0; }
char * __cdecl Live_FileShare_GetLocalizedUserTagFromIndex(int) { return 0; }
char * __cdecl Live_FileShare_PrivateSlotsText(int,int,unsigned int,int,struct Material * *) { return 0; }
char * __cdecl Live_FileShare_SearchResultsItemText(int,int,int,int,bool *) { return 0; }
char * __cdecl LiveContracts_GetContractDesc(int) { return 0; }
char * __cdecl LiveContracts_GetContractName(int) { return 0; }
char * __cdecl LiveContracts_GetContractStatName(int) { return 0; }
char * __cdecl LiveContracts_GetContractStatType(int) { return 0; }
char * __cdecl LiveContracts_GetExpirationType(int) { return 0; }
char * __cdecl LiveContracts_GetRequirementData(int,unsigned int) { return 0; }
char * __cdecl LiveContracts_GetRequirementType(int,unsigned int) { return 0; }
char * __cdecl LiveContracts_GetResetConditions(int) { return 0; }
char * __cdecl LiveStats_GetDStringStatFromForcedBase(int,struct ddlState_t *,char *) { return 0; }
char * __cdecl LiveStats_GetPersonalBestName(int,unsigned int) { return 0; }
char * __cdecl LiveStats_GetPersonalBestPrefix(int,unsigned int) { return 0; }
char * __cdecl LiveTicker_GetCurrentMessage(float,float,float,float,struct Font_s * const,float,float *,float *,float *) { return 0; }
char * __cdecl LiveTicker_GetCurrentMessageCategory(float,float,float,struct Font_s *,float,float *,float *,float *) { return 0; }
char * __cdecl UI_FeederItemText_CustomGametypes(int) { return 0; }
char __cdecl Demo_GetActivatedColorForButton(int,float (*)[4]) { return 0; }
char __cdecl Demo_IsClipRecording(void) { return 0; }
char __cdecl Demo_IsPauseMenuOpen(int) { return 0; }
char __cdecl Demo_ReadDemoMessage(int) { return 0; }
char __cdecl Friends_GetByID(int,unsigned __int64,struct FriendInfo *) { return 0; }
char __cdecl Friends_GetByIndex(int,int,int,struct FriendInfo *) { return 0; }
char __cdecl LB_UploadPlayerStats(int) { return 0; }
char __cdecl Live_AddFavourite_Ingame(unsigned __int64,unsigned __int64) { return 0; }
char __cdecl Live_FileShare_GetFeederData(int,int,int,char const *,char const * *,float *) { return 0; }
char __cdecl Live_FileShare_GetItemInfo(int,enum fileShareKeyIndex,int,enum fileShareInfoLocation,char const * *,float *) { return 0; }
char __cdecl Live_FileShare_GetMySlotInfo(int,int,char const *,char const * *,float *) { return 0; }
char __cdecl Live_FileShare_IsRated(int,unsigned __int64,unsigned char *) { return 0; }
char __cdecl Live_FileShare_Theater_GetDescription(char * *) { return 0; }
char __cdecl Live_FileShare_Theater_GetName(char * *) { return 0; }
char __cdecl Live_FileShare_Theater_GetType(int *) { return 0; }
char __cdecl Live_FileShareSearch_FeederColor(struct listBoxDef_s *,struct itemDef_s *,int,int,int,union DvarValue *) { return 0; }
char __cdecl Live_FileShareSearch_FeederItemEnabled(int) { return 0; }
char __cdecl Live_RequireUserToPlayOnline(void) { return 0; }
char __cdecl LiveStats_GetIntOtherPlayerStatByGameMode(int,int *,char const *,char const *) { return 0; }
char __cdecl LiveStats_GetIntOtherPlayerStatByKey(int,int *,enum playerStatsKeyIndex_t) { return 0; }
char __cdecl LiveStats_GetIntPlayerStat(int,int *,char const *) { return 0; }
char __cdecl LiveStats_GetIntPlayerStatByGameMode(int,int *,char const *,char const *) { return 0; }
char __cdecl LiveStats_GetIntPlayerStatByKey(int,int *,enum playerStatsKeyIndex_t) { return 0; }
char __cdecl LiveStats_GetIntPlayerStatFromBase(int *,char const *,char *) { return 0; }
char __cdecl LiveStats_MoveToProItemVersion(struct ddlState_t *,int,int) { return 0; }
char __cdecl LiveStats_SpendCurrency(int,int,enum pointsSpent_t,int) { return 0; }
char __cdecl PCache_GetCodpoints(int,unsigned __int64,int *) { return 0; }
char __cdecl PCache_GetRank(int,unsigned __int64,int *,int *) { return 0; }
char __cdecl SpawnSystem_AddPoint(unsigned int team,struct gentity_s *ent)
{
    if ( !ent || team >= SP_MAX_SPAWN_TEAMS )
        return 0;

    int index = SpawnSystem_FindPointByEntNum_SP(ent->s.number);
    if ( index == -1 )
    {
        if ( s_spSpawnPointCount + 1 >= SP_MAX_SPAWN_POINTS )
            return 0;

        SpawnPoint *point = &s_spSpawnPoints[s_spSpawnPointCount];
        std::memset(point, 0, sizeof(*point));
        point->origin[0] = ent->r.currentOrigin[0];
        point->origin[1] = ent->r.currentOrigin[1];
        point->origin[2] = ent->r.currentOrigin[2] + SP_LEGACY_SPAWN_POINT_RAISE_HEIGHT;
        point->yaw = ent->r.currentAngles[1];
        point->entNum = ent->s.number;
        point->orderNum = s_spSpawnPointCount;
        index = s_spSpawnPointCount++;
    }

    s_spSpawnPoints[index].teamMask |= 1 << team;
    return 1;
}
char __cdecl SpawnSystem_IsSpawnPointVisible(float *,float *,int,struct gentity_s *) { return 0; }
char __cdecl SV_CACValidate_SetIntStat(unsigned char *,char const *,unsigned int) { return 0; }
char __cdecl UI_Project_FeederDataExtended(int,int,int,struct listBoxDef_s *,char const *,char * *) { return 0; }
char __cdecl UI_Project_FeederItemColor(int,int,struct itemDef_s *,float,int,int,float *,bool) { return 0; }
char const * __cdecl Demo_GetKeyboardDefaultText(void) { return 0; }
char const * __cdecl Demo_GetKeyboardTitle(void) { return 0; }
char const * __cdecl Live_FileShare_GetPooledFileInfo(char const *) { return 0; }
char const * __cdecl Live_FileShare_PrivateSlotsIngameText(int,int,int,struct Material * *) { return 0; }
char const * __cdecl LiveCombatRecord_GetEquipmentUsageStringSuffix(char const *) { return 0; }
char const * __cdecl LiveCombatRecord_GetHitLocationImageRef(enum hitLocation_t) { return 0; }
char const * __cdecl LiveCombatRecord_GetPersonalBestStatNameByIndex(int) { return 0; }
char const * __cdecl LiveStats_GetPlayerStatStringByKey(enum playerStatsKeyIndex_t) { return 0; }
char const * __cdecl LiveStats_GetStatsMilestoneName(int,unsigned int) { return 0; }
char const * __cdecl LiveSteam_GetCountryCode(void) { return 0; }
double __cdecl Live_FileShare_LookupTotalVotes(unsigned __int64,struct bdVoteRankStatsInfo *,int) { return 0; }
double __cdecl LiveCombatRecord_GetSortedItemData(int,bool,enum sortedItemParams_t) { return 0; }
int __cdecl Demo_GetClipTime(void) { return 0; }
int __cdecl Demo_GetCurrentControllerConfig(void) { return 0; }
int __cdecl Demo_GetDemoClientIndex(void) { return 0; }
int __cdecl Demo_GetKeyboardTextSize(void) { return 0; }
int __cdecl Demo_GetKeyFrameForJumpBack(int,int) { return 0; }
int __cdecl Demo_GetKeyFrameForJumpForward(int,int) { return 0; }
int __cdecl Demo_GetKeyFrameTime(int) { return 0; }
int __cdecl Demo_GetNextDefaultBookmarkForPlayer(int,int,int *,int *,float (*)[4]) { return 0; }
int __cdecl Demo_GetSegmentCount(void) { return 0; }
int __cdecl Friends_GetCount(int,int) { return 0; }
int __cdecl LB_FeederCount(int) { return 0; }
int __cdecl Live_CountGuestsInUse(void) { return 0; }
int __cdecl Live_FileShare_GetMyPrivateSlotsCount(void) { return 0; }
int __cdecl Live_FileShare_GetOtherPrivateSlotsCount(void) { return 0; }
int __cdecl Live_FileShare_GetSearchResultsCount(void) { return 0; }
int __cdecl Live_GetInviteFriend(int) { return 0; }
int __cdecl Live_GetInvitesCount(void) { return 0; }
int __cdecl Live_GetMapCount(void) { return 0; }
int __cdecl Live_GetMapIndex(void) { return 0; }
int __cdecl Live_GetMapSource(int) { return 0; }
int __cdecl Live_GetTier(int) { return 0; }
int __cdecl LiveCombatRecord_GetSortedItemPositionByItemIndex(int,bool) { return 0; }
int __cdecl LiveContracts_GetActiveContractIndex(int,int) { return 0; }
int __cdecl LiveContracts_GetActiveContractProgress(int,unsigned int) { return 0; }
int __cdecl LiveContracts_GetActiveContractStatus(int,unsigned int) { return 0; }
int __cdecl LiveContracts_GetCombatTimeLeft(int,unsigned int) { return 0; }
int __cdecl LiveContracts_GetContractCost(int) { return 0; }
int __cdecl LiveContracts_GetContractRequiredCount(int) { return 0; }
int __cdecl LiveContracts_GetContractRewardCP(int) { return 0; }
int __cdecl LiveContracts_GetContractRewardXP(int) { return 0; }
int __cdecl LiveContracts_GetCooldownTime(int) { return 0; }
int __cdecl LiveContracts_GetExpirationData(int) { return 0; }
int __cdecl LiveContracts_GetNumActiveContracts(int) { return 0; }
int __cdecl LiveContracts_GetNumContractsInProgress(int) { return 0; }
int __cdecl LiveContracts_GetTimesPurchasable(int) { return 0; }
int __cdecl LiveContracts_GetTimeUntilCooledDown(int,int) { return 0; }
int __cdecl LiveContracts_GetTimeUntilNextCycle(void) { return 0; }
int __cdecl LiveContracts_GetUnlockLevel(int) { return 0; }
int __cdecl LiveContracts_SV_GetActiveContractProgress(unsigned int,unsigned int) { return 0; }
int __cdecl LiveContracts_SV_GetActiveContractStatus(unsigned int,unsigned int) { return 0; }
int __cdecl LiveContracts_SV_GetCombatTimePassed(unsigned int,unsigned int) { return 0; }
int __cdecl LiveCounter_CounterStringToID(char const *) { return 0; }
int __cdecl LiveCounter_GetCounterTotalValue(int) { return 0; }
int __cdecl LiveMeetPlayer_GetNoOfMetPlayers(int) { return 0; }
int __cdecl LiveStats_GetClanTagFeature(int) { return 0; }
int __cdecl LiveStats_GetCurrency(int) { return 0; }
int __cdecl LiveStats_GetPersonalBestDelta(int,unsigned int) { return 0; }
int __cdecl LiveStats_GetPersonalBestValue(int,unsigned int) { return 0; }
int __cdecl LiveStats_GetPrestige(int) { return 0; }
int __cdecl LiveStats_GetRank(int) { return 0; }
int __cdecl LiveStats_GetRecentlyUnlockedItemIndex(int,unsigned int) { return 0; }
int __cdecl LiveStats_GetStatsMilestoneItemIndex(int,unsigned int) { return 0; }
int __cdecl LiveStats_GetStatsMilestoneType(int,unsigned int) { return 0; }
int __cdecl LiveStats_GetTotalMatchesPlayed(int) { return 0; }
int __cdecl LiveStats_GetTotalMatchesPlayedByGameMode(int,char const *) { return 0; }
int __cdecl LiveSteam_Client_ConnectToSteamServer(unsigned __int64,void *,unsigned int) { return 0; }
int __cdecl LiveStorage_GetMatchesPlayed(char const *,int) { return 0; }
int __cdecl Session_FindRegisteredUser(struct SessionData_s const *,unsigned __int64) { return 0; }
int __cdecl SpawnSystem_AddSphereInfluencer(enum eInfluencerType,float *,float,float,enum eInfluencerScoreCurve,int,unsigned int,int)
{
    return 0;
}
int __cdecl SpawnSystem_GetSortedPointEntNum(unsigned int team,unsigned int index)
{
    if ( team >= SP_MAX_SPAWN_TEAMS )
        return 0;
    if ( index >= (unsigned int)s_spSortedSpawnPoints[team].count )
        return 0;

    const int pointIndex = s_spSortedSpawnPoints[team].spawnPointsByIndex[index];
    if ( pointIndex < 0 || pointIndex >= s_spSpawnPointCount )
        return 0;

    return s_spSpawnPoints[pointIndex].entNum;
}
int __cdecl SpawnSystem_UpdateSpawnPointsForPlayer(struct gentity_s *,unsigned int point_team,unsigned int influencer_team)
{
    return SpawnSystem_UpdateSpawnPointsForTeam(point_team, influencer_team);
}
int __cdecl SpawnSystem_UpdateSpawnPointsForTeam(unsigned int point_team,unsigned int)
{
    if ( point_team >= SP_MAX_SPAWN_TEAMS )
        return 0;

    SortedPointArray &sorted = s_spSortedSpawnPoints[point_team];
    sorted.count = 0;
    sorted.sortTime = level.time;

    for ( int i = 0; i < s_spSpawnPointCount; ++i )
    {
        SpawnPoint &point = s_spSpawnPoints[i];
        if ( ((1 << point_team) & point.teamMask) == 0 )
            continue;

        point.weight[point_team] = point.baseWeight[point_team];
        if ( s_spSpawnRandomVariation > 0.0f )
            point.weight[point_team] += G_crandom() * s_spSpawnRandomVariation;

        sorted.spawnPointsByIndex[sorted.count++] = i;
    }

    std::sort(
        sorted.spawnPointsByIndex,
        sorted.spawnPointsByIndex + sorted.count,
        [point_team](int lhs, int rhs)
        {
            return s_spSpawnPoints[lhs].weight[point_team] > s_spSpawnPoints[rhs].weight[point_team];
        });

    return sorted.count;
}
short __cdecl PCache_GetPlayerEmblemBackgroundID(int,unsigned __int64) { return 0; }
const dvar_t *bot_difficulty;
const dvar_t *bot_enemies;
const dvar_t *bot_friends;
const dvar_t *bot_tips;
const dvar_t *debugStats;
const dvar_t *demo_bookmarkEventThresholdTime;
const dvar_t *demo_controllerConfig;
const dvar_t *fshSelectFirstSlotRow;
const dvar_t *fshSelectLastSlotRow;
const dvar_t *fsOtherUserPrivateSlotCol;
const dvar_t *fsPrivateSlotCol;
const dvar_t *fsSelectedFileID;
const dvar_t *livesteam_sv_vac;
const dvar_t *ui_challengeGameMode;
const dvar_t *xblive_basictraining;
const dvar_t *xblive_basictraining_popup;
const dvar_t *xblive_clanmatch;
const dvar_t *xblive_matchEndingSoon;
const dvar_t *xblive_theater;
const dvar_t *xblive_wagermatch;
unsigned __int64 __cdecl Live_GetXuid(int) { return 0; }
unsigned __int64 __cdecl LiveSteam_GetUid(void) { return 0; }
unsigned int __cdecl LiveContracts_GetIndexForActiveContract(int,unsigned int) { return 0; }
unsigned int __cdecl LiveContracts_GetTimesPurchased(int,int) { return 0; }
unsigned int __cdecl LiveContracts_SV_GetIndexForActiveContract(unsigned int,unsigned int) { return 0; }
unsigned int __cdecl LiveGroups_GetCount(char *) { return 0; }
unsigned int __cdecl LiveGroups_GetCountByType(enum servertype_t) { return 0; }
unsigned int __cdecl LiveMeetPlayer_GetLastMetTime(int,int) { return 0; }
unsigned int __cdecl LiveStats_GetChallengeCount(int) { return 0; }
unsigned int __cdecl LiveStats_GetDDLIntStat(int,char const * *,int) { return 0; }
unsigned int __cdecl LiveStats_GetDInt64StatFromForcedBase(int,struct ddlState_t *,char *) { return 0; }
unsigned int __cdecl LiveStats_GetDIntStat(int,struct ddlState_t *) { return 0; }
unsigned int __cdecl LiveStats_GetDIntStatFromBase(int,struct ddlState_t *,struct persistentStats *) { return 0; }
unsigned int __cdecl LiveStats_GetDIntStatFromForcedBase(int,struct ddlState_t *,char *) { return 0; }
unsigned int __cdecl LiveStats_GetNumPersonalBests(int) { return 0; }
unsigned int __cdecl LiveStats_GetNumRecentlyUnlockedItems(int) { return 0; }
unsigned int __cdecl LiveStats_GetNumStatsMilestones(int) { return 0; }
unsigned int __cdecl LiveStats_GetStatsMilestoneTier(int,unsigned int) { return 0; }
unsigned int __cdecl LiveStats_GetXp(int) { return 0; }
unsigned int __cdecl LiveStorage_FileShare_GetDownloadingFileTotalSize(int) { return 0; }
unsigned int __cdecl LiveStorage_FileShare_GetUploadingFileTotalSize(int) { return 0; }
void __cdecl Demo_AddBookmark(int,int,int,int) {}
void __cdecl Demo_AllocatePlaybackMemory(unsigned int) {}
void __cdecl Demo_BuildDemoSnapshot(void) {}
void __cdecl Demo_ClientConnected(int) {}
void __cdecl Demo_DeallocatePlaybackMemory(void) {}
void __cdecl Demo_DrawMetaInformationScreen(int) {}
void __cdecl Demo_DrawTransitionScreen(int) {}
void __cdecl Demo_DrawVerticalLine(struct ScreenPlacement const *,float,float,float,float,int,int,float,float,float,float,float const *,struct Material *) {}
void __cdecl Demo_End(bool) {}
void __cdecl Demo_Frame(int) {}
void __cdecl Demo_GetConnectionTimeInfoForPlayer(int,int *,int *) {}
void __cdecl Demo_GetStartAndEndTime(int *,int *) {}
void __cdecl Demo_HandleInput(int,int,int,int) {}
void __cdecl Demo_InitFileHandlerSystem(void) {}
void __cdecl Demo_InitPlaybackData(int) {}
void __cdecl Demo_KeyboardComplete(int,char *,bool,char *) {}
void __cdecl Demo_LerpClipCameraValues(int) {}
void __cdecl Demo_Printf(int,char const *) {}
void __cdecl Demo_RecordPlayerSessionState(int,int) {}
void __cdecl Demo_RegisterCommands(void) {}
void __cdecl Demo_RegisterDvars(void) {}
void __cdecl Demo_RestoreUIStateAfterPreview(int) {}
void __cdecl Demo_SetDemoClientState(int) {}
void __cdecl Demo_SetIntermissionPoint(float *,float *) {}
void __cdecl Demo_SetJumpTimeFlag(bool) {}
void __cdecl Demo_SetSwitchedPlayersFlag(bool) {}
void __cdecl Demo_UpdateDesiredTime(int) {}
void __cdecl Demo_UpdateVisibilityBitsForCameraMode(int,int) {}
void __cdecl Demo_WriteClipCommandsWhenPaused(int) {}
void __cdecl Demo_WriteGamestateToBuffer(int,int,struct msg_t *,int) {}
void __cdecl LB_FeederSelection(int) {}
void __cdecl LB_ForceRefresh(void) {}
void __cdecl LB_GetSelectedPlayerInfo(int,struct LBSelectedPlayerInfo *) {}
void __cdecl LB_OnSelect(int) {}
void __cdecl LB_OverrideCursorPos(int,int *,int *) {}
void __cdecl Live_AcceptInvite(unsigned __int64) {}
void __cdecl Live_AcceptLastInvite_f(void) {}
void __cdecl Live_AddFavourite(unsigned __int64,unsigned __int64) {}
void __cdecl Live_AddRecentPlayers(unsigned __int64 *,char const * *,int) {}
void __cdecl Live_DeleteFavourite(unsigned __int64) {}
void __cdecl Live_FileShare_AvgSubmitRatingOwnerdraw(int,struct rectDef_s const *,struct itemDef_s const *,float const *) {}
void __cdecl Live_FileShare_CacheShutdown(void) {}
void __cdecl Live_FileShare_ClearSearchState(void) {}
void __cdecl Live_FileShare_FeederColor(struct listBoxDef_s *,struct itemDef_s *,int,unsigned int,int,enum fileShareBufferLocation,union DvarValue *) {}
void __cdecl Live_FileShare_MyRatingOwnerdraw(int,int,struct rectDef_s const *,struct itemDef_s const *,float const *) {}
void __cdecl Live_FileShare_PrivateRatingOwnerdraw(int,struct rectDef_s const *,struct itemDef_s const *,float const *) {}
void __cdecl Live_FileShare_RatingOwnerdraw(int,struct rectDef_s const *,struct itemDef_s const *,float const *) {}
void __cdecl Live_FileShare_SubmitRatingOwnerdraw(int,struct rectDef_s const *,struct itemDef_s const *,float const *) {}
void __cdecl Live_FileShareSearch_GetRatings(struct bdVoteRankStatsInfo * *,int *) {}
void __cdecl Live_GetFriendsOnServer(unsigned __int64,unsigned __int64 *,int *) {}
void __cdecl Live_GiveAchievement(int,char const *) {}
void __cdecl Live_GiveGamerPicture(int,char const *) {}
void __cdecl Live_JoinWagerFromInvite(void) {}
void __cdecl Live_SetPlaylistNum(int,int) {}
void __cdecl LiveContracts_SV_IncrementActiveContractProgress(unsigned int,unsigned int,int) {}
void __cdecl LiveContracts_SV_IncrementActiveContractTime(unsigned int,unsigned int,int) {}
void __cdecl LiveContracts_SV_ResetActiveContractProgress(unsigned int,unsigned int) {}
void __cdecl LiveCounter_IncrementCounterValue(int,__int64) {}
void __cdecl LiveCounter_IncrementCounterValueByName(char const *,unsigned __int64) {}
void __cdecl LiveMeetPlayer_SortMetPlayers(int,unsigned int) {}
void __cdecl LiveStats_CompareStatsVsStableBuffer(int) {}
void __cdecl LiveStats_MakeStableGlobalStatsBuffer(int) {}
void __cdecl LiveStats_MakeStableStatsBuffer(int) {}
void __cdecl LiveStats_SetStatChanged(int,char const *) {}
void __cdecl LiveSteam_CheckAccess(void) {}
void __cdecl LiveSteam_Client_SteamDisconnect(void) {}
void __cdecl LiveSteam_Frame(void) {}
void __cdecl LiveSteam_Init(void) {}
void __cdecl LiveSteam_Server_ClientSteamAuthentication(unsigned __int64,struct netadr_t,void *,unsigned int) {}
void __cdecl LiveSteam_Server_ClientSteamDisconnect(unsigned __int64) {}
void __cdecl LiveSteam_Server_Init(void) {}
void __cdecl LiveSteam_Server_Shutdown(void) {}
void __cdecl LiveSteam_Shutdown(void) {}
void __cdecl LiveStorage_DeleteGlobalStats(int) {}
void __cdecl LiveStorage_FetchPlaylistsLocal(int) {}
void __cdecl LiveStorage_FileShare_GetDownloadProgress(int,unsigned int *,float *) {}
void __cdecl LiveStorage_FileShare_GetUploadProgress(int,unsigned int *,float *) {}
void __cdecl LiveStorage_LocalSaveStats(int) {}
void __cdecl LiveStorage_ReadStatsIfDirChanged(void) {}
void __cdecl LiveStorage_SetFirstTimeRunning(bool) {}
void __cdecl LiveStorage_SetStatsWriteNeeded(int,bool,enum statsLocation) {}
void __cdecl LiveStorage_UploadStats(void) {}
void __cdecl LiveStorage_UploadStatsForController(void) {}
void __cdecl PC_InitSigninState(void) {}
void __cdecl PCache_GetProfileEmblem(struct PCachePublicProfile *,struct CompositeEmblemLayer *,int,short *) {}
void __cdecl PCache_InvalidatePlayerEmblem(int,unsigned __int64) {}
void __cdecl PCache_Lock(void) {}
void __cdecl PCache_NukeProfile(int) {}
void __cdecl PCache_SetProfileEmblem(int,struct CompositeEmblemLayer const *,int,short) {}
void __cdecl PCache_Unlock(void) {}
void __cdecl Session_DeleteSession(struct SessionData_s *) {}
void __cdecl Session_Init(void) {}
void __cdecl Session_StartHost(struct SessionData_s *,int,int,int) {}
void __cdecl SocketRouter_EmergencyFrame(void) {}
void __cdecl SV_CommitClientLeaderboards(void) {}
void __cdecl SV_DWReadClientCAC(struct client_t *) {}
void __cdecl SV_DWReadClientStats(struct client_t *) {}
void __cdecl SV_DWWriteClientStats(struct client_t *) {}
void __cdecl SV_FetchWADDeferred(void) {}
void __cdecl SV_UpdatePersonalBestsForClient(int) {}
void __cdecl UI_FeederSelection_CustomGametypes(int,float,int) {}
void __cdecl UI_OverrideCursorPos_CustomGametypes(int,struct listBoxDef_s *) {}
void __cdecl UI_Project_FeederItemOwnerDraw(int,int,float,struct rectDef_s const *,unsigned int) {}
void __cdecl UI_Project_FeederSelection(int,int,float,int) {}

persistentStats *__cdecl LiveStorage_GetStatsBuffer(int,statsLocation,bool) { return 0; }
ddlState_t *__cdecl LiveStats_GetRootDDLState(void) { return 0; }
ddlState_t *__cdecl LiveStats_GetCacDDLState(void) { return 0; }
ddlDef_t *__cdecl LiveStats_GetStatsDDL(void) { return 0; }
GfxImage *__cdecl PCache_GetPlayerEmblem(int,unsigned __int64) { return 0; }
void __cdecl SpawnSystem_SortPoints(void)
{
    std::sort(
        s_spSpawnPoints,
        s_spSpawnPoints + s_spSpawnPointCount,
        [](const SpawnPoint &lhs, const SpawnPoint &rhs)
        {
            return lhs.orderNum < rhs.orderNum;
        });
}
void __cdecl SpawnSystem_SetRandomVariation(float variation) { s_spSpawnRandomVariation = variation; }
void __cdecl SpawnSystem_ClearPoints(void)
{
    s_spSpawnPointCount = 0;
    std::memset(s_spSpawnPoints, 0, sizeof(s_spSpawnPoints));
    std::memset(s_spSortedSpawnPoints, 0, sizeof(s_spSortedSpawnPoints));
}
void __cdecl SpawnSystem_ClearPointsBaseWeight(int teammask)
{
    if ( !teammask )
        teammask = SP_ALL_TEAM_MASK;

    for ( int i = 0; i < s_spSpawnPointCount; ++i )
        SpawnSystem_SetPointBaseWeight_SP(&s_spSpawnPoints[i], (char)teammask, 0.0f);
}
void __cdecl SpawnSystem_SetPointsBaseWeight(int teammask,float *objective_position,float angle,float score)
{
    if ( !objective_position )
        return;

    if ( !teammask )
        teammask = SP_ALL_TEAM_MASK;

    const float angle_cos = std::cos(angle * 0.017453292f);
    for ( int i = 0; i < s_spSpawnPointCount; ++i )
    {
        SpawnPoint &point = s_spSpawnPoints[i];

        float forward[3];
        if ( point.entNum >= 0 && point.entNum < MAX_GENTITIES )
            AngleVectors(g_entities[point.entNum].r.currentAngles, forward, 0, 0);
        else
        {
            float axis[3][3];
            YawToAxis(point.yaw, axis);
            forward[0] = axis[0][0];
            forward[1] = axis[0][1];
            forward[2] = axis[0][2];
        }

        float direction[3];
        direction[0] = objective_position[0] - point.origin[0];
        direction[1] = objective_position[1] - point.origin[1];
        direction[2] = objective_position[2] - point.origin[2];
        Vec3Normalize(direction);

        const float dot = forward[0] * direction[0] + forward[1] * direction[1] + forward[2] * direction[2];
        SpawnSystem_SetPointBaseWeight_SP(&point, (char)teammask, dot < angle_cos ? 0.0f : score);
    }
}
struct _CustomClassDescription *__cdecl UI_Project_FeederItemText(int,int,struct itemDef_s *,float,enum itemGroup_t,int,struct Material **,float *,bool *) { return 0; }
struct XuidInfo *__cdecl LiveMeetPlayer_GetMetFriendInfo(struct XuidInfo *,int,int) { return 0; }
struct clipSegment *__cdecl Demo_GetSegmentInformation(int,const char *) { return 0; }
struct PCacheComponent *__cdecl PCache_GetComponent(int,unsigned __int64,unsigned int) { return 0; }
const struct NetFieldList *__cdecl Demo_GetNetFieldListForType(enum netFieldTypes_t) { return 0; }
const struct NetFieldList *__cdecl Demo_GetStateFieldListForEntityType(int) { return 0; }
