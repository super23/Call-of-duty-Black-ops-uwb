#pragma once

parseInfo_t *__cdecl UI_GetMapRotationToken();
void __cdecl UI_RegisterDvars_PC();
const char *__cdecl UI_GetNetSources(unsigned int source);
void __cdecl UI_SelectCurrentGameType();
int __cdecl UI_GetListIndexFromMapIndex(int testMapIndex);
int __cdecl UI_MapCountByGameType();
void __cdecl UI_SelectCurrentMap(int localClientNum);
void __cdecl UI_UseAltColorPalette(char *buffer);
bool __cdecl ValidGamePadButtonIcon(unsigned int letter);
void __cdecl UI_RemoveServerFromDisplayList(int num);
char __cdecl UI_IsServerStatusDvar(const char *dvarname);
int __cdecl UI_GetServerStatusInfo(char *serverAddress, serverStatusInfo_s *info);
void __cdecl UI_SortServerStatusInfo(serverStatusInfo_s *info);
int __cdecl UI_GetServerStatusInfoScoreBoard(char *serverAddress, serverStatusInfo_s *info);
void __cdecl UI_SortServerStatusScoreBoardInfo(serverStatusInfo_s *info);
int __cdecl ScoreBar_CompareScores(const char **sv1, const char **sv2);
void __cdecl UI_RefreshServers();
void __cdecl UI_BuildFindPlayerList();
char *__cdecl stristr(char *str, char *charset);
char *__cdecl UI_SelectedMap(int index, int *actual);
int __cdecl UI_GameType_HandleKey(int flags, int key, int resetMap);
int __cdecl UI_NetGameType_HandleKey(int flags, int key);
bool __cdecl UI_IsMapActive(int mapIndex);
int UI_SelectFirstActiveMap();
void __cdecl UI_SelectListIndexForMapIndex(int mapIndex);
int __cdecl UI_JoinGameType_HandleKey(int flags, int key);
int __cdecl UI_JoinMod_HandleKey(int flags, int key);
void __cdecl UI_NetSource_UpdateDisplayList(int source);
int __cdecl UI_NetSource_HandleKey(int flags, int key);
int __cdecl UI_NetFilter_HandleKey(int flags, int key);
void __cdecl UI_DrawGameType(
        int contextIndex,
        const rectDef_s *rect,
        Font_s *font,
        float scale,
        const float *color,
        int textStyle);
void __cdecl UI_DrawNetGameType(
        int contextIndex,
        const rectDef_s *rect,
        Font_s *font,
        float scale,
        const float *color,
        int textStyle);
void __cdecl UI_DrawJoinGameType(
        int contextIndex,
        const rectDef_s *rect,
        Font_s *font,
        float scale,
        const float *color,
        int textStyle);
void __cdecl UI_DrawJoinMod(
        int contextIndex,
        const rectDef_s *rect,
        Font_s *font,
        float scale,
        const float *color,
        int textStyle);
void __cdecl UI_DrawNetFilter(
        int contextIndex,
        const rectDef_s *rect,
        Font_s *font,
        float scale,
        const float *color,
        int textStyle);
void __cdecl UI_DrawNetSource(
        int contextIndex,
        const rectDef_s *rect,
        Font_s *font,
        float scale,
        const float *color,
        int textStyle);
Material *__cdecl UI_GetLevelShot(int index);
void __cdecl UI_DrawMapPreview(int contextIndex, const rectDef_s *rect, const float *color);
bool __cdecl UI_LodMods_ShouldSkipMap(const char *dir);
void __cdecl UI_LoadMods();
void __cdecl UI_AcceptFriend();
void __cdecl UI_DeclineFriend();
void __cdecl UI_AcceptInvite();
void __cdecl UI_ClearMods();
