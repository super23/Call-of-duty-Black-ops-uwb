#include "pregame_sp.h"
#include <cstring>

pregameStateGlobal pregameG;

// Decomp: SP/MP shared behavior (CoDMPServer.c:881426 baseline)
void __cdecl Pregame_SetState(pregameStateEnum s)
{
    pregameG.state = s;
}

// Decomp: SP/MP shared behavior (CoDMPServer.c:881432 baseline)
pregameStateEnum __cdecl Pregame_GetState()
{
    return pregameG.state;
}

// Decomp: SP behavior — pregame flow remains disabled.
bool __cdecl Pregame_isEnabled()
{
    return false;
}

// Decomp: SP/MP shared behavior (CoDMPServer.c:881426 baseline)
void __cdecl Pregame_StartPregame()
{
    Pregame_SetState(PREGAME_INPROGRESS);
}

// Decomp: SP/MP shared behavior (CoDMPServer.c:424768 baseline)
void __cdecl Pregame_StartGame()
{
    Pregame_SetState(PREGAME_GAMESTARTED);
}

// Decomp: SP/MP shared behavior (CoDMPServer.c:881450 baseline)
void __cdecl Pregame_Reset()
{
    Pregame_SetState(PREGAME_IDLE);
}

// Decomp: SP/MP shared behavior (CoDMPServer.c:424753 baseline)
void __cdecl Pregame_ResetData()
{
    memset(pregameG.clients, 0, sizeof(pregameG.clients));
}

// Decomp: SP/MP shared behavior (CoDMPServer.c:881463 baseline)
void __cdecl Pregame_ResetDataForClient(unsigned int clientNum)
{
    pregameClient *pregameClient;

    if ( clientNum >= 0x20 )
        return;

    pregameClient = &pregameG.clients[clientNum];
    pregameClient->pregame_team = TEAM_FREE;
    memset(pregameClient->pregame_class, 0, sizeof(pregameClient->pregame_class));
}

// Decomp: SP/MP shared behavior (CoDMPServer.c:881492 baseline)
bool __cdecl Pregame_ShouldLoadPregame()
{
    return Pregame_isEnabled() && Pregame_GetState() < PREGAME_GAMESTARTED;
}

// Decomp: SP behavior — no pregame dvars are registered.
void __cdecl Pregame_RegisterDvars()
{
}

// Decomp: SP/MP shared behavior (CoDMPServer.c:881510 baseline)
team_t __cdecl Pregame_GetAssignedPregameTeam(unsigned int clientNum)
{
    if ( clientNum >= 0x20 )
        return TEAM_FREE;

    return pregameG.clients[clientNum].pregame_team;
}

// Decomp: SP/MP shared behavior (CoDMPServer.c:881528 baseline)
void __cdecl Pregame_SetAssignedPregameClass(unsigned int clientNum, const char *pclass)
{
    if ( clientNum >= 0x20 || !pclass )
        return;

    std::strncpy(pregameG.clients[clientNum].pregame_class, pclass, sizeof(pregameG.clients[clientNum].pregame_class) - 1);
    pregameG.clients[clientNum].pregame_class[sizeof(pregameG.clients[clientNum].pregame_class) - 1] = '\0';
}

// Decomp: SP/MP shared behavior (CoDMPServer.c:881546 baseline)
void __cdecl Pregame_SetAssignedPregameTeam(unsigned int clientNum, team_t pteam)
{
    if ( clientNum >= 0x20 )
        return;

    pregameG.clients[clientNum].pregame_team = pteam;
}

// Decomp: SP/MP shared behavior (CoDMPServer.c:881564 baseline)
char *__cdecl Pregame_GetAssignedPregameClass(unsigned int clientNum)
{
    if ( clientNum >= 0x20 )
        return pregameG.clients[0].pregame_class;

    return pregameG.clients[clientNum].pregame_class;
}
