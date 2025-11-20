#pragma once

unsigned int __thiscall CCallback<CSteamAchievements,UserAchievementStored_t,0>::GetCallbackSizeBytes(
        LeaderBoardRow<10> *this);
CSteamAchievements *__thiscall CSteamAchievements::CSteamAchievements(
        CSteamAchievements *this,
        Achievement_t *Achievements,
        int NumAchievements);
void __thiscall CSteamAchievements::~CSteamAchievements(CSteamAchievements *this);
bool __thiscall CSteamAchievements::RequestStats(CSteamAchievements *this);
void __thiscall CSteamAchievements::SetAchievement(CSteamAchievements *this, const char *ID);
void __thiscall CSteamAchievements::StoreStats(CSteamAchievements *this);
void __thiscall CSteamAchievements::ResetStats(CSteamAchievements *this);
void __thiscall CSteamAchievements::OnUserStatsReceived(CSteamAchievements *this, UserStatsReceived_t *pCallback);
void __thiscall CSteamAchievements::OnUserStatsStored(CSteamAchievements *this, UserStatsStored_t *pCallback);
void __thiscall CSteamAchievements::OnAchievementStored(CSteamAchievements *this, UserAchievementStored_t *pCallback);
void __cdecl LiveSteam_Achievements_Init();
void __cdecl LiveSteam_GiveAchievement_f();
void __cdecl LiveSteam_ResetAchievement_f();
void __cdecl LiveSteam_Achievements_Shutdown();
void __cdecl LiveSteam_Achievements_Frame();
void __cdecl LiveSteam_GiveAchievement(const char *achievementName);
int __thiscall CCallback<CSteamAchievements,UserStatsReceived_t,0>::GetCallbackSizeBytes(
        CCallback<CSteamAchievements,UserStatsReceived_t,0> *this);
int __thiscall CCallback<CSteamAchievements,UserStatsStored_t,0>::GetCallbackSizeBytes(
        CCallback<CSteamAchievements,UserStatsStored_t,0> *this);
void __thiscall CCallback<CSteamAchievements,UserStatsReceived_t,0>::Register(
        CCallback<CSteamAchievements,UserStatsReceived_t,0> *this,
        CSteamAchievements *pObj,
        void (__thiscall *func)(CSteamAchievements *this, UserStatsReceived_t *));
void __thiscall CCallback<CSteamAchievements,UserStatsStored_t,0>::Register(
        CCallback<CSteamAchievements,UserStatsStored_t,0> *this,
        CSteamAchievements *pObj,
        void (__thiscall *func)(CSteamAchievements *this, UserStatsStored_t *));
void __thiscall CCallback<CSteamAchievements,UserAchievementStored_t,0>::Register(
        CCallback<CSteamAchievements,UserAchievementStored_t,0> *this,
        CSteamAchievements *pObj,
        void (__thiscall *func)(CSteamAchievements *this, UserAchievementStored_t *));
void __thiscall CCallback<CSteamAchievements,UserStatsStored_t,0>::Run(
        CCallback<LiveSteamP2P,P2PSessionRequest_t,0> *this,
        P2PSessionRequest_t *pvParam);
