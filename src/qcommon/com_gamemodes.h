#pragma once

bool __cdecl Com_GameMode_IsOnlineGame();
bool __cdecl Com_GameMode_IsPrivateOnlineGame();
bool __cdecl Com_GameMode_IsPublicOnlineGame();
bool __cdecl Com_GameMode_IsGameMode(eGameModes mode);
char ValidateGameModes();
void __cdecl Com_GameMode_ResetGameModes();
