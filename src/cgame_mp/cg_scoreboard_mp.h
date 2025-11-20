#pragma once

const char *__cdecl CG_GetNameForScoreboardColumn(int localClientNum, unsigned int columnNumber);
void __cdecl CG_UpdateMatchScoreboard(int localClientNum);
void __cdecl CG_AddSBColumnToMatchScoreBoard(
        int localClientNum,
        listColumnTypes_t columnType,
        int columnWidth,
        const char *columnName,
        int alignment,
        scoreboardColumnType_t sbColumnType);
int __cdecl CG_GetMatchScoreboardClientCount(int localClientNum, team_t team);
int __cdecl CG_GetMatchscoreboardTeam(int localClientNum, int index);
int __cdecl CG_GetMatchScoreboardIndexForTeam(int localClientNum, int index, team_t team);
void __cdecl CG_GetInGamePlayerListIcon(int localClientNum, int column, int index, team_t team, Material **handle);
char *__cdecl CG_GetMatchScoreboardInfo(int localClientNum, int column, int index, team_t team);
char *__cdecl CG_GetColumnValueString(int localClientNum, const score_s *score, scoreboardColumnType_t columnType);
int __cdecl CG_GetColumnValue(int localClientNum, const score_s *score, scoreboardColumnType_t columnType);
const char *__cdecl CG_GetMatchInGamePlayerListInfo(
        int localClientNum,
        int column,
        int index,
        team_t team,
        Material **handle);
const char *__cdecl CG_IsInGamePlayerListPlayerTalking(int localClientNum, unsigned int clientNum, Material **handle);
char *__cdecl CG_GetMatchInGamePlayersInfo(
        int localClientNum,
        int column,
        unsigned int index,
        int feederType,
        Material **handle);
const char *__cdecl CG_GetMatchInGamePlayerXuid(int localClientNum, int index, team_t team);
char __cdecl CG_GetMatchInGamePlayersXUID(
        int localClientNum,
        unsigned int index,
        int feederType,
        unsigned __int64 *retXUID);
void __cdecl CG_GetMatchScoreboardRankIcon(int localClientNum, int index, team_t team, Material **handle);
int __cdecl CG_GetIndexIntoMatchScoreboard(int localClientNum, int controllerIndex);
int __cdecl CG_GetWagerPlaceForMatchScoreboard(int localClientNum, int matchScoreboardIndex);
const score_s *__cdecl GetClientScore(int localClientNum, int clientNum);
const score_s *__cdecl UI_GetOurClientScore(int localClientNum);
const score_s *__cdecl UI_GetScoreAtRank(int localClientNum, int rank);
char *__cdecl CG_GetGametypeDescription(int localClientNum);
char __cdecl CG_DrawScoreboard_GetTeamColorIndex(int team, int localClientNum);
int __cdecl CG_DrawScoreboard(int localClientNum);
void __cdecl CG_DrawScoreboard_Backdrop(int localClientNum, float alpha);
double __cdecl CG_BackdropWidth();
double __cdecl CG_BackdropLeft(int localClientNum);
double __cdecl CG_BackdropTop();
void __cdecl CG_DrawBackdropServerInfo(int localClientNum, float alpha);
void __cdecl CG_DrawScoreboard_ScoresList(int localClientNum, float alpha);
double __cdecl CG_DrawScoreboard_ListColumnHeaders(
        int localClientNum,
        const float *color,
        float y,
        float h,
        float listWidth);
void __cdecl CG_GetScoreboardInfo(int localClientNum, const listColumnInfo_t **colInfo, int *numFields);
int __cdecl CG_ScoreboardTotalLines(int localClientNum);
double __cdecl CG_DrawTeamOfClientScore(
        int localClientNum,
        const float *color,
        float y,
        int team,
        float listWidth,
        int *drawLine);
int __cdecl CG_CheckDrawScoreboardLine(int localClientNum, int *drawLine, float y, float lineHeight);
double __cdecl CG_DrawScoreboard_ListBanner(
        int localClientNum,
        const float *color,
        float y,
        float w,
        float h,
        int team,
        int *piDrawLine);
int __cdecl CG_GetLivePlayersOnTeam(int localClientNum, int team);
double __cdecl CG_DrawClientScore(
        int localClientNum,
        unsigned int clientNum,
        const float *color,
        float y,
        const score_s *score,
        float listWidth);
double __cdecl CalcXAdj(int align, float maxw, float w);
void __cdecl DrawListString(
        int localClientNum,
        char *string,
        float x,
        float y,
        float width,
        int alignment,
        Font_s *font,
        float scale,
        int style,
        const float *color);
void __cdecl CG_DrawClientPing(int localClientNum, int ping, float x, float y, float maxWidth, float maxHeight);
void __cdecl CG_DrawScrollbar(int localClientNum, float top);
void __cdecl CenterViewOnClient(int localClientNum);
bool __cdecl CG_IsScoreboardDisplayed(int localClientNum);
bool __cdecl CG_IsIntermission(int localClientNum);
void __cdecl CG_ScrollScoreboardUp(cg_s *cgameGlob);
void __cdecl CG_ScrollScoreboardDown(cg_s *cgameGlob);
void __cdecl CG_InitScoreboard();
void __cdecl CG_SetFocusScoreboardCmd();
void __cdecl CG_RegisterScoreboardDvars();
void __cdecl CG_RegisterScoreboardGraphics();
bool __cdecl Scoreboard_HandleInput(int localClientNum, int key);
bool __cdecl MuteError_HandleInput(int localClientNum);
int __cdecl CG_GetPlaceWithTiesForScore(int localClientNum, int score);
int __cdecl CG_GetKills(int localClientNum, const score_s *score);
int __cdecl CG_GetDeaths(int localClientNum, const score_s *score);
