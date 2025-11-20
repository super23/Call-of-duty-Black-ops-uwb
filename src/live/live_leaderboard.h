#pragma once

bool __cdecl LB_NeedToReadLeaderBoard();
void __cdecl LB_ForceRefresh();
void __cdecl LB_InitStructure(LbStructure *const structure, int type);
void __cdecl LB_RegisterStat(LbStructure *const structure, LbStatEnum statType, bool isRating);
void __cdecl LB_RegisterStructure(LbLookup *const lookup, LbStructure structure, int type);
void __cdecl LB_InitGlobalStructure(LbGlobalStructure *const globalStructures, int lbIndex);
void __cdecl LB_RegisterGlobalStat(
        LbGlobalStructure *const globalStructures,
        GlobalLbStatEnum globalStatType,
        bool isRating);
void __cdecl LB_RegisterGlobalStructure(LbLookup *const lookup, LbGlobalStructure globalStructures, int lbIndex);
LbType __cdecl LB_GetLbIndex(LbType type, LbResetPeriod resetPeriod, bool isPrestigeLb, bool isHiddenLb);
char __cdecl LB_GetNumMatchesPlayedForPlayerFromLb(Leaderboard *lb, int controllerIndex);
char __cdecl LB_GetByRank(Leaderboard *lb, int controllerIndex, unsigned int topRank);
char __cdecl LB_GetByPlayer(Leaderboard *lb, int localClientNum);
char __cdecl LB_GetStatsByXuids(int controllerIndex, Leaderboard *lb);
char __cdecl LB_GetPlayerStats(int controllerIndex, Leaderboard *lb);
char __cdecl LB_GetFriends(Leaderboard *lb, int localClientNum);
char __cdecl LB_CalcOffset(int offset, Leaderboard *lb);
LeaderBoardResult<LeaderBoardRow<10>,100> *__cdecl LB_UpdateLeaderboard(int localClientNum, LbType lbType, int index);
int __cdecl LB_FeederCount(int localClientNum);
void __cdecl LB_FeederSelection(int index);
void __cdecl LB_OnSelect(int localClientNum);
void __cdecl LB_GetSelectedPlayerInfo(int localClientNum, LBSelectedPlayerInfo *info);
void __cdecl LB_OverrideCursorPos(int viewmax, int *cursorPos, int *startPos);
int __cdecl LB_GetXUserCol(LbLookup *g_lbLookup, LeaderBoardRow<10> *row, int type, LbStatEnum colId);
void __cdecl LB_GetRankIconForXp(LeaderBoardRow<10> *row, LbType type, Material **handle);
unsigned int __cdecl LB_CalcPlayerIndex(int localControllerIndex, Leaderboard *lb);
void __cdecl LB_SortXUserStatsByRank(Leaderboard *lb);
int __cdecl LB_CompareXUserStatsRowRanks(_QWORD *r1, _QWORD *r2);
bool __cdecl LB_MakeRow(
        int controllerIndex,
        const LbPlayerStat *const stats,
        int type,
        int lbIndex,
        LeaderBoardRow<10> *const row,
        bool statWriteTypeAdd);
bool __cdecl LB_MakeGlobalRow(
        int controllerIndex,
        const LbPlayerStat *const stats,
        int lbIndex,
        LeaderBoardRow<10> *const row);
void __cdecl LB_openDownloadingMenuCmd();
void __cdecl LB_ReportUser();
void __cdecl LB_IncrementEscrow();
char __cdecl LB_SetGametypeDvar();
taskCompleteResults __cdecl LB_ReadHiddenLeaderboardComplete(unsigned int slot);
void __cdecl LB_SetStatsForRow(int controllerIndex, const char *gameModePrefix, bool setMatchDelta);
taskCompleteResults __cdecl LB_ReadLeaderboardMatchesCountComplete(unsigned int slot);
taskCompleteResults __cdecl LB_ReadLeaderboardComplete(unsigned int slot);
void __cdecl LB_ClearPlayerStats(int localControllerIndex);
void __cdecl LB_SetStat(int localControllerIndex, int statId, int value);
void __cdecl LB_UpdateNeeded(int localControllerIndex);
void __cdecl LB_UploadPlayerGlobalStats(int localControllerIndex);
void __cdecl Lb_ReadHiddenLeaderboardEntry(
        int controllerIndex,
        LeaderBoardRow<10> *userStats,
        int lbIndex,
        LbTaskEnum task);
void __cdecl Lb_InitiateWeeklyAndMonthlyLbWrite(int controllerIndex, LbType type);
char __cdecl LB_UploadPlayerStats(int localControllerIndex);
taskCompleteResults __cdecl LB_UploadPlayerStatsComplete(unsigned int slot);
taskCompleteResults __cdecl LB_WriteStatsToLbComplete(unsigned int slot);
taskCompleteResults __cdecl LB_IncrementEscrowComplete(unsigned int slot);
taskCompleteResults __cdecl LB_ResolveEscrowComplete(unsigned int slot);
taskCompleteResults __cdecl LB_RetrieveEscrowBalanceComplete(unsigned int slot);
taskCompleteResults __cdecl LB_ClearEscrowComplete(int slot);
char *__cdecl LB_FeederItemText(int localClientNum, unsigned int index, int column, Material **material);
void __cdecl LB_ReadRowForUser(
        int controllerIndex,
        unsigned __int64 entityID,
        unsigned int leaderboardID,
        int taskType,
        LeaderBoardRow<10> *userStats);
void __cdecl LB_WriteRows(int controllerIndex, bdStatsInfo **statsInfo, unsigned int numStats, int taskType);
void __cdecl LB_InitAndRegisterStructure(LbStructure *const structure, int type);
void __cdecl LB_InitAndRegisterStructureForSpecialLeaderboards(
        LbGlobalStructure *const globalStructures,
        int lbIndex,
        int type);
void __cdecl LB_Init();
void __cdecl LB_CheckOngoingTasks();
void __cdecl LB_EndOngoingTasks();
void __thiscall LeaderBoardRow<10>::serialize(LeaderBoardRow<10> *this, bdByteBuffer *buffer);
bool __thiscall LeaderBoardRow<10>::deserialize(LeaderBoardRow<10> *this, bdReference<bdByteBuffer> buffer);
LbGlob *__thiscall LbGlob::LbGlob(LbGlob *this);
Leaderboard *__thiscall Leaderboard::Leaderboard(Leaderboard *this);
LeaderBoardResult<LeaderBoardRow<10>,100> *__thiscall LeaderBoardResult<LeaderBoardRow<10>,100>::LeaderBoardResult<LeaderBoardRow<10>,100>(
        LeaderBoardResult<LeaderBoardRow<10>,100> *this);
void __thiscall LeaderBoardResult<LeaderBoardRow<10>,100>::~LeaderBoardResult<LeaderBoardRow<10>,100>(
        LeaderBoardResult<LeaderBoardRow<10>,100> *this);
void __thiscall LbGlob::~LbGlob(LbGlob *this);
void __thiscall Leaderboard::~Leaderboard(Leaderboard *this);
