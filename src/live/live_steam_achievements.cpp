#include "live_steam_achievements.h"

unsigned int __thiscall CCallback<CSteamAchievements,UserAchievementStored_t,0>::GetCallbackSizeBytes(
        LeaderBoardRow<10> *this)
{
  return 152;
}

CSteamAchievements *__thiscall CSteamAchievements::CSteamAchievements(
        CSteamAchievements *this,
        Achievement_t *Achievements,
        int NumAchievements)
{
  int v3; // eax

  this->m_needStatsStored = 1;
  this->m_iAppID = 0;
  this->m_bInitialized = 0;
  this->m_CallbackUserStatsReceived.__vftable = (CCallback<CSteamAchievements,UserStatsReceived_t,0>_vtbl *)&CCallbackBase::`vftable';
  this->m_CallbackUserStatsReceived.m_nCallbackFlags = 0;
  this->m_CallbackUserStatsReceived.m_iCallback = 0;
  this->m_CallbackUserStatsReceived.__vftable = (CCallback<CSteamAchievements,UserStatsReceived_t,0>_vtbl *)&CCallback<CSteamAchievements,UserStatsReceived_t,0>::`vftable';
  this->m_CallbackUserStatsReceived.m_pObj = this;
  this->m_CallbackUserStatsReceived.m_Func = CSteamAchievements::OnUserStatsReceived;
  if ( this && CSteamAchievements::OnUserStatsReceived )
    CCallback<CSteamAchievements,UserStatsReceived_t,0>::Register(
      &this->m_CallbackUserStatsReceived,
      this,
      CSteamAchievements::OnUserStatsReceived);
  this->m_CallbackUserStatsStored.__vftable = (CCallback<CSteamAchievements,UserStatsStored_t,0>_vtbl *)&CCallbackBase::`vftable';
  this->m_CallbackUserStatsStored.m_nCallbackFlags = 0;
  this->m_CallbackUserStatsStored.m_iCallback = 0;
  this->m_CallbackUserStatsStored.__vftable = (CCallback<CSteamAchievements,UserStatsStored_t,0>_vtbl *)&CCallback<CSteamAchievements,UserStatsStored_t,0>::`vftable';
  this->m_CallbackUserStatsStored.m_pObj = this;
  this->m_CallbackUserStatsStored.m_Func = CSteamAchievements::OnUserStatsStored;
  if ( this && CSteamAchievements::OnUserStatsStored )
    CCallback<CSteamAchievements,UserStatsStored_t,0>::Register(
      &this->m_CallbackUserStatsStored,
      this,
      CSteamAchievements::OnUserStatsStored);
  this->m_CallbackAchievementStored.__vftable = (CCallback<CSteamAchievements,UserAchievementStored_t,0>_vtbl *)&CCallbackBase::`vftable';
  this->m_CallbackAchievementStored.m_nCallbackFlags = 0;
  this->m_CallbackAchievementStored.m_iCallback = 0;
  this->m_CallbackAchievementStored.__vftable = (CCallback<CSteamAchievements,UserAchievementStored_t,0>_vtbl *)&CCallback<CSteamAchievements,UserAchievementStored_t,0>::`vftable';
  this->m_CallbackAchievementStored.m_pObj = this;
  this->m_CallbackAchievementStored.m_Func = CSteamAchievements::OnAchievementStored;
  if ( this && CSteamAchievements::OnAchievementStored )
    CCallback<CSteamAchievements,UserAchievementStored_t,0>::Register(
      &this->m_CallbackAchievementStored,
      this,
      CSteamAchievements::OnAchievementStored);
  v3 = _SteamUtils();
  LODWORD(this->m_iAppID) = (*(int (__thiscall **)(int, int))(*(unsigned int *)v3 + 36))(v3, v3);
  HIDWORD(this->m_iAppID) = 0;
  this->m_pAchievements = Achievements;
  this->m_iNumAchievements = NumAchievements;
  CSteamAchievements::RequestStats(this);
  return this;
}

void __thiscall CSteamAchievements::~CSteamAchievements(CSteamAchievements *this)
{
  this->m_CallbackAchievementStored.__vftable = (CCallback<CSteamAchievements,UserAchievementStored_t,0>_vtbl *)&CCallback<CSteamAchievements,UserAchievementStored_t,0>::`vftable';
  if ( (this->m_CallbackAchievementStored.m_nCallbackFlags & 1) != 0 )
    _SteamAPI_UnregisterCallback(&this->m_CallbackAchievementStored);
  this->m_CallbackUserStatsStored.__vftable = (CCallback<CSteamAchievements,UserStatsStored_t,0>_vtbl *)&CCallback<CSteamAchievements,UserStatsStored_t,0>::`vftable';
  if ( (this->m_CallbackUserStatsStored.m_nCallbackFlags & 1) != 0 )
    _SteamAPI_UnregisterCallback(&this->m_CallbackUserStatsStored);
  this->m_CallbackUserStatsReceived.__vftable = (CCallback<CSteamAchievements,UserStatsReceived_t,0>_vtbl *)&CCallback<CSteamAchievements,UserStatsReceived_t,0>::`vftable';
  if ( (this->m_CallbackUserStatsReceived.m_nCallbackFlags & 1) != 0 )
    _SteamAPI_UnregisterCallback(&this->m_CallbackUserStatsReceived);
}

bool __thiscall CSteamAchievements::RequestStats(CSteamAchievements *this)
{
  int (__thiscall ***v2)(unsigned int, unsigned int); // eax
  int v3; // [esp+0h] [ebp-Ch]
  int v4; // [esp+0h] [ebp-Ch]
  int v5; // [esp+4h] [ebp-8h]

  if ( !_SteamUserStats() || !_SteamUser(v3) )
    return 0;
  v5 = _SteamUser(v4);
  if ( !(*(unsigned __int8 (__thiscall **)(int))(*(unsigned int *)v5 + 4))(v5) )
    return 0;
  v2 = (int (__thiscall ***)(unsigned int, unsigned int))_SteamUserStats();
  return (**v2)(v2, v2);
}

void __thiscall CSteamAchievements::SetAchievement(CSteamAchievements *this, const char *ID)
{
  int v2; // [esp+0h] [ebp-8h]

  if ( this->m_bInitialized )
  {
    v2 = _SteamUserStats();
    (*(void (__thiscall **)(int, const char *))(*(unsigned int *)v2 + 28))(v2, ID);
    this->m_needStatsStored = 1;
  }
}

void __thiscall CSteamAchievements::StoreStats(CSteamAchievements *this)
{
  int v1; // eax

  if ( this->m_needStatsStored )
  {
    v1 = _SteamUserStats();
    this->m_needStatsStored = (*(unsigned __int8 (__thiscall **)(int, int))(*(unsigned int *)v1 + 40))(v1, v1) == 0;
  }
}

void __thiscall CSteamAchievements::ResetStats(CSteamAchievements *this)
{
  int v1; // [esp+0h] [ebp-Ch]

  v1 = _SteamUserStats();
  if ( (*(unsigned __int8 (__thiscall **)(int, int))(*(unsigned int *)v1 + 76))(v1, 1) )
    Com_DPrintf(23, "STEAM: Resetting achievements\n");
  else
    Com_DPrintf(23, "STEAM: Unable to reset achievements\n");
  this->m_needStatsStored = 1;
}

void __thiscall CSteamAchievements::OnUserStatsReceived(CSteamAchievements *this, UserStatsReceived_t *pCallback)
{
  const char *v2; // eax
  const char *v3; // eax
  int v4; // [esp+0h] [ebp-20h]
  int v5; // [esp+4h] [ebp-1Ch]
  int v6; // [esp+8h] [ebp-18h]
  Achievement_t *ach; // [esp+18h] [ebp-8h]
  int iAch; // [esp+1Ch] [ebp-4h]

  if ( LODWORD(this->m_iAppID) == LODWORD(pCallback->m_nGameID)
    && HIDWORD(this->m_iAppID) == HIDWORD(pCallback->m_nGameID) )
  {
    if ( pCallback->m_eResult == k_EResultOK )
    {
      Com_DPrintf(23, "STEAM: Received stats and achievements from Steam\n");
      this->m_bInitialized = 1;
      for ( iAch = 0; iAch < this->m_iNumAchievements; ++iAch )
      {
        ach = &this->m_pAchievements[iAch];
        v6 = _SteamUserStats();
        (*(void (__thiscall **)(int, const char *, bool *))(*(unsigned int *)v6 + 24))(
          v6,
          ach->m_pchAchievementID,
          &ach->m_bAchieved);
        v5 = _SteamUserStats();
        v2 = (const char *)(*(int (__thiscall **)(int, const char *, const char *))(*(unsigned int *)v5 + 48))(
                             v5,
                             ach->m_pchAchievementID,
                             "name");
        _snprintf(ach->m_rgchName, 0x80u, "%s", v2);
        v4 = _SteamUserStats();
        v3 = (const char *)(*(int (__thiscall **)(int, const char *, const char *))(*(unsigned int *)v4 + 48))(
                             v4,
                             ach->m_pchAchievementID,
                             "desc");
        _snprintf(ach->m_rgchDescription, 0x100u, "%s", v3);
      }
    }
    else
    {
      Com_DPrintf(23, "STEAM: RequestStats - failed, %d\n", pCallback->m_eResult);
    }
  }
}

void __thiscall CSteamAchievements::OnUserStatsStored(CSteamAchievements *this, UserStatsStored_t *pCallback)
{
  if ( LODWORD(this->m_iAppID) == LODWORD(pCallback->m_nGameID)
    && HIDWORD(this->m_iAppID) == HIDWORD(pCallback->m_nGameID) )
  {
    if ( pCallback->m_eResult == k_EResultOK )
      Com_DPrintf(23, "STEAM: Stored stats for Steam\n");
    else
      Com_DPrintf(23, "STEAM: StatsStored - failed, %d\n", pCallback->m_eResult);
  }
}

void __thiscall CSteamAchievements::OnAchievementStored(CSteamAchievements *this, UserAchievementStored_t *pCallback)
{
  if ( LODWORD(this->m_iAppID) == LODWORD(pCallback->m_nGameID)
    && HIDWORD(this->m_iAppID) == HIDWORD(pCallback->m_nGameID) )
  {
    Com_DPrintf(23, "STEAM: Stored Achievement for Steam\n");
  }
}

void __cdecl LiveSteam_Achievements_Init()
{
  CSteamAchievements *v0; // [esp+0h] [ebp-Ch]
  CSteamAchievements *v1; // [esp+4h] [ebp-8h]

  v1 = (CSteamAchievements *)operator new(0x58u);
  if ( v1 )
    v0 = CSteamAchievements::CSteamAchievements(v1, g_Achievements, 3);
  else
    v0 = 0;
  g_liveSteamAchievements = v0;
  Cmd_AddCommandInternal("livesteam_giveachivement", LiveSteam_GiveAchievement_f, &LiveSteam_GiveAchievement_f_VAR);
  Cmd_AddCommandInternal("livesteam_resetachivement", LiveSteam_ResetAchievement_f, &LiveSteam_ResetAchievement_f_VAR);
}

void __cdecl LiveSteam_GiveAchievement_f()
{
  const char *v0; // eax
  const char *v1; // eax

  if ( Cmd_Argc() != 2 )
  {
    v0 = Cmd_Argv(0);
    Com_Printf(0, "Usage: %s <achievement name>\n", v0);
  }
  v1 = Cmd_Argv(1);
  LiveSteam_GiveAchievement(v1);
}

void __cdecl LiveSteam_ResetAchievement_f()
{
  CSteamAchievements::ResetStats(g_liveSteamAchievements);
}

void __cdecl LiveSteam_Achievements_Shutdown()
{
  CSteamAchievements *v0; // [esp+8h] [ebp-4h]

  if ( g_liveSteamAchievements )
  {
    v0 = g_liveSteamAchievements;
    CSteamAchievements::~CSteamAchievements(g_liveSteamAchievements);
    operator delete(v0);
  }
}

void __cdecl LiveSteam_Achievements_Frame()
{
  if ( g_liveSteamAchievements )
    CSteamAchievements::StoreStats(g_liveSteamAchievements);
}

void __cdecl LiveSteam_GiveAchievement(const char *achievementName)
{
  if ( g_liveSteamAchievements )
    CSteamAchievements::SetAchievement(g_liveSteamAchievements, achievementName);
}

int __thiscall CCallback<CSteamAchievements,UserStatsReceived_t,0>::GetCallbackSizeBytes(
        CCallback<CSteamAchievements,UserStatsReceived_t,0> *this)
{
  return 24;
}

int __thiscall CCallback<CSteamAchievements,UserStatsStored_t,0>::GetCallbackSizeBytes(
        CCallback<CSteamAchievements,UserStatsStored_t,0> *this)
{
  return 16;
}

void __thiscall CCallback<CSteamAchievements,UserStatsReceived_t,0>::Register(
        CCallback<CSteamAchievements,UserStatsReceived_t,0> *this,
        CSteamAchievements *pObj,
        void (__thiscall *func)(CSteamAchievements *this, UserStatsReceived_t *))
{
  if ( pObj && func )
  {
    if ( (this->m_nCallbackFlags & 1) != 0 )
      _SteamAPI_UnregisterCallback(this);
    this->m_pObj = pObj;
    this->m_Func = func;
    _SteamAPI_RegisterCallback(this, 1101);
  }
}

void __thiscall CCallback<CSteamAchievements,UserStatsStored_t,0>::Register(
        CCallback<CSteamAchievements,UserStatsStored_t,0> *this,
        CSteamAchievements *pObj,
        void (__thiscall *func)(CSteamAchievements *this, UserStatsStored_t *))
{
  if ( pObj && func )
  {
    if ( (this->m_nCallbackFlags & 1) != 0 )
      _SteamAPI_UnregisterCallback(this);
    this->m_pObj = pObj;
    this->m_Func = func;
    _SteamAPI_RegisterCallback(this, 1102);
  }
}

void __thiscall CCallback<CSteamAchievements,UserAchievementStored_t,0>::Register(
        CCallback<CSteamAchievements,UserAchievementStored_t,0> *this,
        CSteamAchievements *pObj,
        void (__thiscall *func)(CSteamAchievements *this, UserAchievementStored_t *))
{
  if ( pObj && func )
  {
    if ( (this->m_nCallbackFlags & 1) != 0 )
      _SteamAPI_UnregisterCallback(this);
    this->m_pObj = pObj;
    this->m_Func = func;
    _SteamAPI_RegisterCallback(this, 1103);
  }
}

void __thiscall CCallback<CSteamAchievements,UserStatsStored_t,0>::Run(
        CCallback<LiveSteamP2P,P2PSessionRequest_t,0> *this,
        P2PSessionRequest_t *pvParam)
{
  this->m_Func(this->m_pObj, pvParam);
}

