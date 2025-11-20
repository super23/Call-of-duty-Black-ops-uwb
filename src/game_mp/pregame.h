#pragma once

void __cdecl Pregame_SetState(pregameStateEnum s);
pregameStateEnum __cdecl Pregame_GetState();
bool __cdecl Pregame_isEnabled();
void __cdecl Pregame_StartPregame();
void __cdecl Pregame_StartGame();
void __cdecl Pregame_Reset();
void __cdecl Pregame_ResetData();
void __cdecl Pregame_ResetDataForClient(unsigned int clientNum);
bool __cdecl Pregame_ShouldLoadPregame();
void __cdecl Pregame_RegisterDvars();
team_t __cdecl Pregame_GetAssignedPregameTeam(unsigned int clientNum);
void __cdecl Pregame_SetAssignedPregameClass(unsigned int clientNum, const char *pclass);
void __cdecl Pregame_SetAssignedPregameTeam(unsigned int clientNum, team_t pteam);
char *__cdecl Pregame_GetAssignedPregameClass(unsigned int clientNum);
