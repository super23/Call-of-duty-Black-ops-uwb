#pragma once

int __cdecl Playlist_GetCategoryIdByName(const char *name);
void __cdecl Playlist_ParsePlaylists(const char *buffer);
char *__cdecl Playlist_Strdup(const char *src);
char *__cdecl Playlist_Strdup_Newline(const char *src);
char *__cdecl Playlist_BeginRules();
char __cdecl Playlist_AppendRules(const char *base, const char *token);
int __cdecl ParseIntRange(const char *field, const char **pData, int min, int max);
char __cdecl Playlist_HasCategory(int playlistId, const char *category);
int __cdecl Playlist_GetPlaylistCount();
int __cdecl Playlist_GetCategoryCount();
int __cdecl Playlist_GetPlaylistCount(int category);
void __cdecl Playlist_ValidatePlaylistNum();
int __cdecl Playlist_GetPlaylistIdForNum(int controllerIndex, int num);
int __cdecl Playlist_GetPlaylistIdForNum(int controllerIndex, unsigned int categoryPlaylistNum, int categoryNum);
int __cdecl Playlist_GetPlaylistNumFromId(int controllerIndex, int id);
int __cdecl Playlist_GetCategoryIdForNum(int num);
int __cdecl Playlist_GetCategoryIdForPlaylistId(int num);
int __cdecl Playlist_GetNumEntries(unsigned int playlistId);
playlistEntry *__cdecl Playlist_GetEntry(unsigned int playlistId, int entry);
void __cdecl Playlist_ResetCategoryFilter();
void __cdecl Playlist_EnableCategoryInFilter(const char *filterName);
const char *__cdecl Playlist_GetCategoryLocalizedName(int controllerIndex, unsigned int categoryId);
const char *__cdecl Playlist_GetPlaylistName(int controllerIndex, unsigned int playlistId);
const char *__cdecl Playlist_GetPlaylistDescription(int controllerIndex, unsigned int playlistId);
const char *__cdecl Playlist_GetPlaylistIconName(int controllerIndex, unsigned int playlistId);
const char *__cdecl Playlist_GetCategoryIconName(int controllerIndex, unsigned int categoryId);
int __cdecl Playlist_IsLocked(
        int controllerIndex,
        unsigned int playlistId,
        int xp,
        int prestige,
        int gametypeBan,
        int partyCount);
int __cdecl Playlist_CategoryIsLocked(int controllerIndex, unsigned int categoryId, int xp, int prestige);
int __cdecl Playlist_GetMapPacksUsedByPlaylist(unsigned int playlistId);
bool __cdecl Playlist_DoWeHaveRequiredDLC(int controllerIndex, unsigned int playlistId);
const char *__cdecl Playlist_GetCategoryInternalName(int categoryId);
playlistGametype *__cdecl Playlist_FindGametype(const char *gametypeInternalName);
int __cdecl Playlist_GetGametypeCount();
const char *__cdecl Playlist_GetGametypeLocalizedName(unsigned int index);
const char *__cdecl Playlist_GetGametypeInternalName(unsigned int index);
int __cdecl Playlist_ChooseEntry(int localControllerIndex, int lastSelection);
void __cdecl Playlist_ResetWeights(int playlistId);
int __cdecl Playlist_CalcTotalWeights(int playlistId, int lastSelection);
int __cdecl Playlist_FindEntryByWeight(int playlistId, int selectedWeight, int lastSelection);
void __cdecl Playlist_MarkEntryPlayed(int playlistId, int entryNum);
void __cdecl Playlist_RunRules(int localControllerIndex, int entryNum);
int __cdecl Playlist_GetVersionNumber();
int __cdecl Playlist_GetUnlockXp(int localControllerIndex, unsigned int playlistId);
int __cdecl Playlist_GetUnlockPLevel(int localControllerIndex, unsigned int playlistId);
int __cdecl Playlist_GetMaxPartySize(int localControllerIndex, unsigned int playlistId);
int __cdecl Playlist_GetMinPartySize(int localControllerIndex, unsigned int playlistId);
int __cdecl Playlist_GetWagerBet(int localControllerIndex, unsigned int playlistId);
void __cdecl Playlist_SetPrevPlaylist();
void __cdecl Playlist_GetPrevPlaylist();
void __cdecl Playlist_SetPlaylistEntry(int num);
void __cdecl Playlist_SetSVMapRotation();
void __cdecl Playlist_SetSVMapRotationCurrent(int entryNum);
void __cdecl Playlist_SVMapRotate();
void __cdecl Playlist_Init();
const dvar_s *Playlist_RegisterDvars();
void __cdecl Playlist_Fetch_f();
void __cdecl Playlist_Pick_f();
int __cdecl Playlist_GetWagerAmount(int version, int index, int *wagerAmount);
