#pragma once

// CoDSP_rdBlackOps.map.c:828EAC80 CL_MapLoadingKickLoadMovie, :828EA138 CL_MapLoading_StartCinematic,
// :828EA1B8 CL_MapLoading_CalcMovieToPlay_FastFile / :828EA248 CL_MapLoading_CalcMovieToPlay

void __cdecl CL_MapLoadingKickLoadMovie(const char *mapName, char savegame);
void __cdecl CL_MapLoading_StartCinematic(const char *mapName);
void __cdecl CL_MapLoading_CalcMovieToPlay_FastFile(const char *mapName, char *outMovieName);
void __cdecl CL_MapLoading_CalcMovieToPlay(const char *scriptText, const char *mapName, char *outMovieName);
