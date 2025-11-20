#include "live_friends_pc.h"

int __cdecl LiveSteam_GetNumFriends()
{
  return _GetNumFriends<4>();
}

int __cdecl LiveSteam_GetNumFriendsOnTheServer()
{
  return _GetNumFriends<16>();
}

unsigned __int64 __cdecl LiveSteam_GetFriendXuid(int index)
{
  int SortedFriendIndex; // eax

  SortedFriendIndex = _GetSortedFriendIndex(index);
  return _GetFriendXuid<4>(SortedFriendIndex);
}

void __cdecl LiveSteam_GetFriendPersonaName(int index, char *buf, int size)
{
  int SortedFriendIndex; // eax

  SortedFriendIndex = _GetSortedFriendIndex(index);
  _GetFriendPersonaName<4>(SortedFriendIndex, buf, size);
}

void __cdecl LiveSteam_GetFriendOnTheServerPersonaName(int index, char *buf, int size)
{
  _GetFriendPersonaName<16>(index, buf, size);
}

int __cdecl LiveSteam_GetFriendPersonaState(unsigned __int64 xuid)
{
  int v2; // [esp+0h] [ebp-10h]
  LiveSteam_PersonaState result; // [esp+Ch] [ebp-4h]

  result = LIVE_STEAM_PERSONA_STATE_OFFLINE;
  if ( _SteamFriends() )
  {
    v2 = _SteamFriends();
    return (*(int (__thiscall **)(int, unsigned int, unsigned int))(*(unsigned int *)v2 + 24))(v2, xuid, HIDWORD(xuid));
  }
  return result;
}

int __cdecl LiveSteam_GetFriendPersonaStateByIndex(int index)
{
  unsigned __int64 FriendXuid; // rax

  FriendXuid = LiveSteam_GetFriendXuid(index);
  return LiveSteam_GetFriendPersonaState(FriendXuid);
}

char __cdecl Friends_Update()
{
  g_sortedFriendsListDirty = 1;
  return 1;
}

int __cdecl Friends_GetCount(int __formal, int type)
{
  if ( !type )
    return LiveSteam_GetNumFriends();
  if ( type == 1 )
    return LiveMeetPlayer_GetNoOfMetPlayers(0);
  return 0;
}

char __cdecl Friends_GetByIndex(int controllerIndex, int filter, int index, FriendInfo *info)
{
  if ( index < 0 || index >= LiveSteam_GetNumFriends() || !info )
    return 0;
  info->steamid = LiveSteam_GetFriendXuid(index);
  info->dwid = info->steamid;
  switch ( LiveSteam_GetFriendPersonaStateByIndex(index) )
  {
    case 0:
      info->status = 8;
      break;
    case 1:
      info->status = 5;
      break;
    case 2:
      info->status = 4;
      break;
    case 3:
      info->status = 4;
      break;
    default:
      info->status = 8;
      break;
  }
  LiveSteam_GetFriendPersonaName(index, info->name, 32);
  info->presence[0] = 0;
  return 1;
}

char __cdecl Friends_GetByID(int controllerIndex, unsigned __int64 id, FriendInfo *info)
{
  if ( !info || !_GetFriendPersonaNameByXuid<4>(id, info->name, 32) )
    return 0;
  info->steamid = id;
  info->dwid = info->steamid;
  info->presence[0] = 0;
  switch ( LiveSteam_GetFriendPersonaState(id) )
  {
    case 0:
      info->status = 8;
      break;
    case 1:
      info->status = 5;
      break;
    case 2:
      info->status = 4;
      break;
    case 3:
      info->status = 4;
      break;
    default:
      info->status = 8;
      break;
  }
  return 1;
}

bool __cdecl Friend_GetByName(int controllerIndex, const char *friendName, unsigned __int64 *uid)
{
  int i; // [esp+0h] [ebp-BCh]
  char friendBuf[32]; // [esp+4h] [ebp-B8h] BYREF
  char tmpXuidStr[132]; // [esp+24h] [ebp-98h] BYREF
  unsigned __int64 tmpXuid; // [esp+ACh] [ebp-10h] BYREF
  bool retval; // [esp+BBh] [ebp-1h]

  retval = 0;
  if ( uid )
  {
    memset(friendBuf, 0, sizeof(friendBuf));
    for ( i = 0; i < _GetNumFriends<4>(); ++i )
    {
      _GetFriendPersonaName<4>(i, friendBuf, 32);
      if ( !I_stricmp(friendName, friendBuf) )
      {
        *uid = _GetFriendXuid<4>(i);
        return 1;
      }
      tmpXuid = _GetFriendXuid<4>(i);
      XUIDToString(&tmpXuid, tmpXuidStr);
      if ( !I_stricmp(friendName, friendBuf) )
      {
        *uid = tmpXuid;
        return 1;
      }
    }
  }
  return retval;
}

void __cdecl Friends_GetPeopleOnServer()
{
  char buf[68]; // [esp+0h] [ebp-50h] BYREF
  int i; // [esp+48h] [ebp-8h]
  int numPeople; // [esp+4Ch] [ebp-4h]

  numPeople = _GetNumFriends<16>();
  for ( i = 0; i < numPeople; ++i )
  {
    memset(buf, 0, 64);
    _GetFriendPersonaName<16>(i, buf, 64);
    Com_Printf(23, "%s\n", buf);
  }
}

void __cdecl Friend_PrintNamesOnServer_f()
{
  Friends_GetPeopleOnServer();
}

void __cdecl Friends_Init()
{
  Cmd_AddCommandInternal("getpeopleonserver", Friend_PrintNamesOnServer_f, &Friend_PrintNamesOnServer_f_VAR);
  g_sortedFriendsListDirty = 1;
}

void __cdecl Live_GetRecentPlayerName(unsigned __int64 uid, char *buf, int bufsize)
{
  _GetFriendPersonaNameByXuid<4>(uid, buf, bufsize);
}

CSteamID *__thiscall CSteamID::CSteamID(CSteamID *this)
{
  *(unsigned int *)&this->m_steamid.m_comp = 0;
  *((unsigned int *)&this->m_steamid.m_comp + 1) &= 0xFF0FFFFF;
  *((unsigned int *)&this->m_steamid.m_comp + 1) &= (unsigned int)&cls.rankedServers[711].game[34];
  *((unsigned int *)&this->m_steamid.m_comp + 1) &= 0xFFF00000;
  return this;
}

int __cdecl _GetSortedFriendIndex(int index)
{
  unsigned __int64 Friend; // rax
  int FriendPersonaState; // [esp+0h] [ebp-10h]
  int i; // [esp+4h] [ebp-Ch]
  int onlineInsertIndex; // [esp+8h] [ebp-8h]
  int offlineInsertIndex; // [esp+Ch] [ebp-4h]

  if ( g_sortedFriendsListDirty )
  {
    g_sortedNumFriends = LiveSteam_GetNumFriends();
    if ( g_sortedNumFriends > 256
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\live\\live_friends_pc.cpp",
            295,
            0,
            "%s",
            "g_sortedNumFriends <= MAX_FRIENDS") )
    {
      __debugbreak();
    }
    onlineInsertIndex = 0;
    offlineInsertIndex = g_sortedNumFriends - 1;
    for ( i = 0; i < g_sortedNumFriends; ++i )
    {
      Friend = _GetFriendXuid<4>(i);
      FriendPersonaState = LiveSteam_GetFriendPersonaState(Friend);
      if ( FriendPersonaState <= 0 || FriendPersonaState > 3 )
        g_sortedFriendsRedirect[offlineInsertIndex--] = i;
      else
        g_sortedFriendsRedirect[onlineInsertIndex++] = i;
    }
    g_sortedFriendsListDirty = 0;
  }
  return g_sortedFriendsRedirect[index];
}

int __cdecl _GetNumFriends<4>()
{
  int result; // [esp+0h] [ebp-8h]
  ISteamFriends *steamFriends; // [esp+4h] [ebp-4h]

  result = 0;
  steamFriends = (ISteamFriends *)_SteamFriends();
  if ( steamFriends )
    return steamFriends->GetFriendCount(steamFriends, 4);
  return result;
}

int __cdecl _GetNumFriends<16>()
{
  int result; // [esp+0h] [ebp-8h]
  ISteamFriends *steamFriends; // [esp+4h] [ebp-4h]

  result = 0;
  steamFriends = (ISteamFriends *)_SteamFriends();
  if ( steamFriends )
    return steamFriends->GetFriendCount(steamFriends, 16);
  return result;
}

unsigned __int64 __cdecl _GetFriendXuid<4>(int index)
{
  CSteamID steamID; // [esp+0h] [ebp-14h] BYREF
  ISteamFriends *steamFriends; // [esp+8h] [ebp-Ch]
  unsigned __int64 xuid; // [esp+Ch] [ebp-8h]

  xuid = 0;
  steamFriends = (ISteamFriends *)_SteamFriends();
  if ( steamFriends )
  {
    if ( index >= steamFriends->GetFriendCount(steamFriends, 4)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\live\\live_friends_pc.cpp",
            37,
            0,
            "%s",
            "index < steamFriends->GetFriendCount( k_EFriendFlagImmediate )") )
    {
      __debugbreak();
    }
    steamFriends->GetFriendByIndex(steamFriends, &steamID, index, 4);
    if ( !CSteamID::IsValid(&steamID)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\live\\live_friends_pc.cpp",
            39,
            0,
            "%s",
            "steamID.IsValid()") )
    {
      __debugbreak();
    }
    return steamID.m_steamid.m_unAll64Bits;
  }
  return xuid;
}

bool __thiscall CSteamID::IsValid(CSteamID *this)
{
  if ( ((*((unsigned int *)&this->m_steamid.m_comp + 1) >> 20) & 0xF) == 0
    || ((*((unsigned int *)&this->m_steamid.m_comp + 1) >> 20) & 0xFu) >= 0xB )
  {
    return 0;
  }
  if ( SHIDWORD(this->m_steamid.m_unAll64Bits) >> 24 <= 0 || SHIDWORD(this->m_steamid.m_unAll64Bits) >> 24 >= 6 )
    return 0;
  if ( ((*((unsigned int *)&this->m_steamid.m_comp + 1) >> 20) & 0xF) == 1
    && (!*(unsigned int *)&this->m_steamid.m_comp || (*((unsigned int *)&this->m_steamid.m_comp + 1) & 0xFFFFF) != 1) )
  {
    return 0;
  }
  return ((*((unsigned int *)&this->m_steamid.m_comp + 1) >> 20) & 0xF) != 7
      || *(unsigned int *)&this->m_steamid.m_comp && (*((unsigned int *)&this->m_steamid.m_comp + 1) & 0xFFFFF) == 0;
}

void __cdecl _GetFriendPersonaName<4>(int index, char *buf, int size)
{
  CSteamID steamID; // [esp+0h] [ebp-10h] BYREF
  const char *s; // [esp+8h] [ebp-8h]
  ISteamFriends *steamFriends; // [esp+Ch] [ebp-4h]

  steamFriends = (ISteamFriends *)_SteamFriends();
  if ( (!buf || !size)
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\live\\live_friends_pc.cpp", 50, 0, "%s", "buf && size") )
  {
    __debugbreak();
  }
  if ( steamFriends )
  {
    if ( index >= steamFriends->GetFriendCount(steamFriends, 4)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\live\\live_friends_pc.cpp",
            53,
            0,
            "%s",
            "index < steamFriends->GetFriendCount( friendType )") )
    {
      __debugbreak();
    }
    steamFriends->GetFriendByIndex(steamFriends, &steamID, index, 4);
    s = (const char *)((int (__thiscall *)(ISteamFriends *, unsigned int, unsigned int))steamFriends->GetFriendPersonaName)(
                        steamFriends,
                        *(unsigned int *)&steamID.m_steamid.m_comp,
                        *((unsigned int *)&steamID.m_steamid.m_comp + 1));
    Com_sprintf(buf, size, "%.16s", s);
  }
  else
  {
    *buf = 0;
  }
}

void __cdecl _GetFriendPersonaName<16>(int index, char *buf, int size)
{
  CSteamID steamID; // [esp+0h] [ebp-10h] BYREF
  const char *s; // [esp+8h] [ebp-8h]
  ISteamFriends *steamFriends; // [esp+Ch] [ebp-4h]

  steamFriends = (ISteamFriends *)_SteamFriends();
  if ( (!buf || !size)
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\live\\live_friends_pc.cpp", 50, 0, "%s", "buf && size") )
  {
    __debugbreak();
  }
  if ( steamFriends )
  {
    if ( index >= steamFriends->GetFriendCount(steamFriends, 16)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\live\\live_friends_pc.cpp",
            53,
            0,
            "%s",
            "index < steamFriends->GetFriendCount( friendType )") )
    {
      __debugbreak();
    }
    steamFriends->GetFriendByIndex(steamFriends, &steamID, index, 16);
    s = (const char *)((int (__thiscall *)(ISteamFriends *, unsigned int, unsigned int))steamFriends->GetFriendPersonaName)(
                        steamFriends,
                        *(unsigned int *)&steamID.m_steamid.m_comp,
                        *((unsigned int *)&steamID.m_steamid.m_comp + 1));
    Com_sprintf(buf, size, "%.16s", s);
  }
  else
  {
    *buf = 0;
  }
}

char __cdecl _GetFriendPersonaNameByXuid<4>(unsigned __int64 xuid, char *buf, int size)
{
  const char *s; // [esp+10h] [ebp-8h]
  ISteamFriends *steamFriends; // [esp+14h] [ebp-4h]

  steamFriends = (ISteamFriends *)_SteamFriends();
  if ( (!buf || !size)
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\live\\live_friends_pc.cpp", 68, 0, "%s", "buf && size") )
  {
    __debugbreak();
  }
  if ( steamFriends
    && ((int (__thiscall *)(ISteamFriends *, unsigned int, unsigned int))steamFriends->GetFriendRelationship)(
         steamFriends,
         xuid,
         HIDWORD(xuid)) )
  {
    s = (const char *)((int (__thiscall *)(ISteamFriends *, unsigned int, unsigned int))steamFriends->GetFriendPersonaName)(
                        steamFriends,
                        xuid,
                        HIDWORD(xuid));
    Com_sprintf(buf, size, "%.16s", s);
    return 1;
  }
  else
  {
    *buf = 0;
    return 0;
  }
}

