#include "ui_server.h"

void __cdecl UI_ClearDisplayedServers()
{
  *(unsigned int *)&sharedUiInfo.gap0[81128] = 0;
  *(unsigned int *)&sharedUiInfo.gap0[81136] = 0;
  *(unsigned int *)&sharedUiInfo.gap0[81132] = LAN_GetServerCount(ui_netSource->current.integer);
  memset(&sharedUiInfo.serverStatus[82200], 0, 0x100u);
}

void __cdecl UI_BuildServerDisplayList(int localClientNum, uiInfo_s *uiInfo, int force)
{
  const char *String; // eax
  char *v4; // eax
  char *v5; // eax
  char *v6; // eax
  char *v7; // eax
  char *v8; // eax
  char *v9; // eax
  char *v10; // eax
  char *v11; // eax
  char *v12; // eax
  char *v13; // eax
  char *v14; // eax
  char *v15; // eax
  char *v16; // eax
  char *v17; // eax
  const char *v18; // [esp-4h] [ebp-460h]
  const char *v19; // [esp-4h] [ebp-460h]
  char v20; // [esp+13h] [ebp-449h]
  _BYTE *v21; // [esp+18h] [ebp-444h]
  char *v22; // [esp+1Ch] [ebp-440h]
  int filter; // [esp+30h] [ebp-42Ch] BYREF
  const char *mod; // [esp+38h] [ebp-424h]
  char info[1024]; // [esp+3Ch] [ebp-420h] BYREF
  int ping; // [esp+440h] [ebp-1Ch]
  int maxClients; // [esp+444h] [ebp-18h]
  int len; // [esp+448h] [ebp-14h]
  int dirty; // [esp+44Ch] [ebp-10h]
  int i; // [esp+450h] [ebp-Ch]
  int clients; // [esp+454h] [ebp-8h]
  int count; // [esp+458h] [ebp-4h]

  if ( force || uiInfo->uiDC.realTime > *(int *)&sharedUiInfo.gap0[81140] )
  {
    if ( force == 2 )
      force = 0;
    String = Dvar_GetString("cl_motdString");
    I_strncpyz(&sharedUiInfo.serverStatus[81176], String, 1024);
    len = strlen(&sharedUiInfo.serverStatus[81176]);
    if ( !len )
    {
      v4 = UI_SafeTranslateString("EXE_COD_MULTIPLAYER");
      v22 = va("%s - %s", v4, "1.0");
      v21 = &sharedUiInfo.serverStatus[81176];
      do
      {
        v20 = *v22;
        *v21++ = *v22++;
      }
      while ( v20 );
      len = strlen(&sharedUiInfo.serverStatus[81176]);
    }
    if ( len != *(unsigned int *)&sharedUiInfo.gap0[81152] )
    {
      *(unsigned int *)&sharedUiInfo.gap0[81152] = len;
      *(unsigned int *)&sharedUiInfo.gap0[81156] = -1;
    }
    if ( force )
    {
      numclean = 0;
      UI_ClearDisplayedServers();
      if ( *(int *)&sharedUiInfo.gap0[1124] >= 0 )
        Menu_SetFeederSelection(localClientNum, &uiInfo->uiDC, 0, 2, 0, 0);
      LAN_MarkServerDirty(ui_netSource->current.integer, -1, 1u);
    }
    count = LAN_GetServerCount(ui_netSource->current.integer);
    if ( LAN_WaitServerResponse() )
    {
      UI_ClearDisplayedServers();
      *(unsigned int *)&sharedUiInfo.gap0[81140] = uiInfo->uiDC.realTime + 500;
    }
    else
    {
      UI_ServersSort(*(unsigned int *)&sharedUiInfo.gap0[1108] | 0x100000000LL);
      dirty = 0;
      for ( i = 0; i < count; ++i )
      {
        if ( LAN_ServerIsDirty(ui_netSource->current.integer, i) )
        {
          dirty = 1;
          ping = LAN_GetServerPing(ui_netSource->current.integer, i);
          if ( ping > 0 )
          {
            LAN_GetServerInfo(ui_netSource->current.integer, i, info, 1024);
            v5 = Info_ValueForKey(info, "clients");
            clients = atoi(v5);
            *(unsigned int *)&sharedUiInfo.gap0[81136] += clients;
            v6 = Info_ValueForKey(info, "addr");
            if ( !I_strnicmp(v6, "000.000.000.000", 15) )
              goto LABEL_64;
            if ( ping <= 0 )
              goto LABEL_65;
            if ( !ui_browserShowEmpty->current.enabled && !clients )
              goto LABEL_64;
            if ( !ui_browserShowFull->current.enabled )
            {
              v7 = Info_ValueForKey(info, "com_maxclients");
              maxClients = atoi(v7);
              if ( clients == maxClients )
                goto LABEL_64;
            }
            if ( ui_browserShowPassword->current.integer >= 0 )
            {
              v8 = Info_ValueForKey(info, "pswrd");
              if ( atoi(v8) != ui_browserShowPassword->current.integer )
                goto LABEL_64;
            }
            if ( ui_browserShowPure->current.enabled )
            {
              v9 = Info_ValueForKey(info, "pure");
              if ( !atoi(v9) )
                goto LABEL_64;
            }
            if ( ui_browserMod->current.integer > -2 )
            {
              mod = Info_ValueForKey(info, "game");
              if ( ui_browserMod->current.integer == -1 )
              {
                if ( *mod )
                  goto LABEL_64;
              }
              if ( ui_browserMod->current.integer >= 0 )
              {
                if ( ui_browserMod->current.integer >= 0x40u
                  && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\ui\\ui_server.cpp",
                        317,
                        0,
                        "ui_browserMod->current.integer doesn't index MAX_MODS\n\t%i not in [0, %i)",
                        ui_browserMod->current.integer,
                        64) )
                {
                  __debugbreak();
                }
                if ( I_stricmp(
                       mod + 5,
                       (const char *)sharedUiInfo.serverHardwareIconList[2 * ui_browserMod->current.integer + 9]) )
                {
                  goto LABEL_64;
                }
              }
            }
            if ( ui_browserFriendlyfire->current.integer >= 0 )
            {
              v10 = Info_ValueForKey(info, "ff");
              if ( atoi(v10) != ui_browserFriendlyfire->current.integer )
                goto LABEL_64;
            }
            if ( ui_browserKillcam->current.integer >= 0 )
            {
              v11 = Info_ValueForKey(info, "kc");
              if ( atoi(v11) != ui_browserKillcam->current.integer )
                goto LABEL_64;
            }
            if ( ui_browserHardcore->current.integer >= 0 )
            {
              v12 = Info_ValueForKey(info, "hc");
              if ( atoi(v12) != ui_browserHardcore->current.integer )
                goto LABEL_64;
            }
            if ( (*(_BYTE *)ui_browserGameMode->current.integer == 48
               || (v13 = Info_ValueForKey(info, "gametype"), !I_stricmp(ui_browserGameMode->current.string, v13)))
              && (ui_browserMap->current.integer <= 0
               || ui_browserMap->current.integer >= sharedUiInfo.joinGameTypes[31].basictraining + 1
               || (v14 = Info_ValueForKey(info, "mapname"),
                   !I_stricmp(
                      &sharedUiInfo.joinGameTypes[29].gameTypeNameCaps[304 * ui_browserMap->current.integer],
                      v14)))
              && (ui_browserVoiceChat->current.integer < 0
               || (v15 = Info_ValueForKey(info, "voice"), atoi(v15) == ui_browserVoiceChat->current.integer))
              && (!sharedUiInfo.joinGameTypes[ui_joinGameType->current.integer].gameType[8]
               || (v18 = (const char *)&sharedUiInfo.gameTypeMapCount[29 * ui_joinGameType->current.integer + 32],
                   v16 = Info_ValueForKey(info, "gametype"),
                   !I_stricmp(v16, v18)))
              && (ui_serverFilterType <= 0
               || (v19 = UI_GetServerFilter((const char **)&filter, ui_serverFilterType)[1],
                   v17 = Info_ValueForKey(info, "game"),
                   !I_stricmp(v17, v19))) )
            {
LABEL_65:
              if ( ui_netSource->current.integer == 3 )
                UI_RemoveServerFromDisplayList(i);
              UI_BinaryServerInsertion(i, 0);
              if ( ping > 0 )
              {
                LAN_MarkServerDirty(ui_netSource->current.integer, i, 0);
                ++numclean;
              }
            }
            else
            {
LABEL_64:
              LAN_MarkServerDirty(ui_netSource->current.integer, i, 0);
            }
          }
        }
      }
      if ( ui_netSource->current.integer == 4 && *(unsigned int *)&sharedUiInfo.gap0[1108] == 11 )
        UI_ServersSort(0x10000000BLL);
      *(unsigned int *)&sharedUiInfo.gap0[1100] = uiInfo->uiDC.realTime;
    }
  }
}

int __cdecl UI_BinaryServerInsertion(int num, int nameNum)
{
  int offset; // [esp+0h] [ebp-10h]
  int len; // [esp+4h] [ebp-Ch]
  int res; // [esp+8h] [ebp-8h]
  int mid; // [esp+Ch] [ebp-4h]

  len = *(unsigned int *)&sharedUiInfo.gap0[81128];
  mid = *(unsigned int *)&sharedUiInfo.gap0[81128];
  offset = 0;
  res = 0;
  while ( mid > 0 )
  {
    mid = len >> 1;
    if ( *(unsigned int *)&sharedUiInfo.gap0[1108] == 11 )
      res = LAN_CompareServers(
              ui_netSource->current.integer,
              11,
              *(int *)&sharedUiInfo.gap0[1112],
              nameNum,
              (unsigned __int8)sharedUiInfo.serverStatus[mid + 82200 + offset]);
    else
      res = LAN_CompareServers(
              ui_netSource->current.integer,
              *(int *)&sharedUiInfo.gap0[1108],
              *(int *)&sharedUiInfo.gap0[1112],
              num,
              *(unsigned int *)&sharedUiInfo.gap0[4 * mid + 1128 + 4 * offset]);
    if ( *(unsigned int *)&sharedUiInfo.gap0[1108] == 11 )
    {
      if ( res != -LAN_CompareServers(
                     ui_netSource->current.integer,
                     11,
                     *(int *)&sharedUiInfo.gap0[1112],
                     (unsigned __int8)sharedUiInfo.serverStatus[mid + 82200 + offset],
                     nameNum)
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\ui\\ui_server.cpp",
              137,
              0,
              "%s",
              "res == -LAN_CompareServers( ui_netSource->current.integer, sharedUiInfo.serverStatus.sortKey, sharedUiInfo"
              ".serverStatus.sortDir, sharedUiInfo.serverStatus.displayServersFriendNameInd[offset + mid], nameNum )") )
      {
        __debugbreak();
      }
    }
    else if ( res != -LAN_CompareServers(
                        ui_netSource->current.integer,
                        *(int *)&sharedUiInfo.gap0[1108],
                        *(int *)&sharedUiInfo.gap0[1112],
                        *(unsigned int *)&sharedUiInfo.gap0[4 * mid + 1128 + 4 * offset],
                        num)
           && !Assert_MyHandler(
                 "C:\\projects_pc\\cod\\codsrc\\src\\ui\\ui_server.cpp",
                 141,
                 0,
                 "%s",
                 "res == -LAN_CompareServers( ui_netSource->current.integer, sharedUiInfo.serverStatus.sortKey, sharedUiI"
                 "nfo.serverStatus.sortDir, sharedUiInfo.serverStatus.displayServers[offset + mid], num )") )
    {
      __debugbreak();
    }
    if ( !res )
    {
      UI_InsertServerIntoDisplayList(num, mid + offset, nameNum);
      return mid + offset;
    }
    if ( res > 0 )
      offset += mid;
    len -= mid;
  }
  if ( res > 0 )
    ++offset;
  UI_InsertServerIntoDisplayList(num, offset, nameNum);
  return offset;
}

void __cdecl UI_InsertServerIntoDisplayList(int num, int position, int nameNum)
{
  int i; // [esp+0h] [ebp-8h]
  int res; // [esp+4h] [ebp-4h]
  int resa; // [esp+4h] [ebp-4h]
  int resb; // [esp+4h] [ebp-4h]
  int resc; // [esp+4h] [ebp-4h]

  if ( position < 0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ui\\ui_server.cpp", 55, 0, "%s", "position >= 0") )
  {
    __debugbreak();
  }
  if ( *(unsigned int *)&sharedUiInfo.gap0[1108] == 11 )
  {
    if ( position < *(int *)&sharedUiInfo.gap0[81128] )
    {
      res = LAN_CompareServers(
              ui_netSource->current.integer,
              *(int *)&sharedUiInfo.gap0[1108],
              *(int *)&sharedUiInfo.gap0[1112],
              nameNum,
              (unsigned __int8)sharedUiInfo.serverStatus[position + 82200]);
      if ( res != -LAN_CompareServers(
                     ui_netSource->current.integer,
                     *(int *)&sharedUiInfo.gap0[1108],
                     *(int *)&sharedUiInfo.gap0[1112],
                     (unsigned __int8)sharedUiInfo.serverStatus[position + 82200],
                     nameNum)
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\ui\\ui_server.cpp",
              63,
              0,
              "%s",
              "res == -LAN_CompareServers( ui_netSource->current.integer, sharedUiInfo.serverStatus.sortKey, sharedUiInfo"
              ".serverStatus.sortDir, sharedUiInfo.serverStatus.displayServersFriendNameInd[position], nameNum )") )
      {
        __debugbreak();
      }
      if ( res > 0 && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ui\\ui_server.cpp", 64, 0, "%s", "res <= 0") )
        __debugbreak();
    }
    if ( position > 0 )
    {
      resa = LAN_CompareServers(
               ui_netSource->current.integer,
               *(int *)&sharedUiInfo.gap0[1108],
               *(int *)&sharedUiInfo.gap0[1112],
               nameNum,
               (unsigned __int8)sharedUiInfo.serverStatus[position + 82199]);
      if ( resa != -LAN_CompareServers(
                      ui_netSource->current.integer,
                      *(int *)&sharedUiInfo.gap0[1108],
                      *(int *)&sharedUiInfo.gap0[1112],
                      (unsigned __int8)sharedUiInfo.serverStatus[position + 82199],
                      nameNum)
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\ui\\ui_server.cpp",
              69,
              0,
              "%s",
              "res == -LAN_CompareServers( ui_netSource->current.integer, sharedUiInfo.serverStatus.sortKey, sharedUiInfo"
              ".serverStatus.sortDir, sharedUiInfo.serverStatus.displayServersFriendNameInd[position - 1], nameNum )") )
      {
        __debugbreak();
      }
      if ( resa < 0
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ui\\ui_server.cpp", 70, 0, "%s", "res >= 0") )
      {
        __debugbreak();
      }
    }
  }
  else
  {
    if ( position < *(int *)&sharedUiInfo.gap0[81128] )
    {
      resb = LAN_CompareServers(
               ui_netSource->current.integer,
               *(int *)&sharedUiInfo.gap0[1108],
               *(int *)&sharedUiInfo.gap0[1112],
               num,
               *(unsigned int *)&sharedUiInfo.gap0[4 * position + 1128]);
      if ( resb != -LAN_CompareServers(
                      ui_netSource->current.integer,
                      *(int *)&sharedUiInfo.gap0[1108],
                      *(int *)&sharedUiInfo.gap0[1112],
                      *(unsigned int *)&sharedUiInfo.gap0[4 * position + 1128],
                      num)
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\ui\\ui_server.cpp",
              79,
              0,
              "%s",
              "res == -LAN_CompareServers( ui_netSource->current.integer, sharedUiInfo.serverStatus.sortKey, sharedUiInfo"
              ".serverStatus.sortDir, sharedUiInfo.serverStatus.displayServers[position], num )") )
      {
        __debugbreak();
      }
      if ( resb > 0
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ui\\ui_server.cpp", 80, 0, "%s", "res <= 0") )
      {
        __debugbreak();
      }
    }
    if ( position > 0 )
    {
      resc = LAN_CompareServers(
               ui_netSource->current.integer,
               *(int *)&sharedUiInfo.gap0[1108],
               *(int *)&sharedUiInfo.gap0[1112],
               num,
               *(unsigned int *)&sharedUiInfo.gap0[4 * position + 1124]);
      if ( resc != -LAN_CompareServers(
                      ui_netSource->current.integer,
                      *(int *)&sharedUiInfo.gap0[1108],
                      *(int *)&sharedUiInfo.gap0[1112],
                      *(unsigned int *)&sharedUiInfo.gap0[4 * position + 1124],
                      num)
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\ui\\ui_server.cpp",
              85,
              0,
              "%s",
              "res == -LAN_CompareServers( ui_netSource->current.integer, sharedUiInfo.serverStatus.sortKey, sharedUiInfo"
              ".serverStatus.sortDir, sharedUiInfo.serverStatus.displayServers[position - 1], num )") )
      {
        __debugbreak();
      }
      if ( resc < 0
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ui\\ui_server.cpp", 86, 0, "%s", "res >= 0") )
      {
        __debugbreak();
      }
    }
  }
  if ( position >= 0 && position <= *(int *)&sharedUiInfo.gap0[81128] )
  {
    if ( position <= *(int *)&sharedUiInfo.gap0[1124] && *(unsigned int *)&sharedUiInfo.gap0[81128] )
      ++*(unsigned int *)&sharedUiInfo.gap0[1124];
    for ( i = ++*(unsigned int *)&sharedUiInfo.gap0[81128]; i > position; --i )
      *(unsigned int *)&sharedUiInfo.gap0[4 * i + 1128] = *(unsigned int *)&sharedUiInfo.gap0[4 * i + 1124];
    *(unsigned int *)&sharedUiInfo.gap0[4 * position + 1128] = num;
  }
}

void __cdecl UI_BuildServerStatus(int localClientNum, uiInfo_s *uiInfo, int force)
{
  if ( !uiInfo->nextFindPlayerRefresh )
  {
    if ( force )
    {
      Menu_SetFeederSelection(localClientNum, &uiInfo->uiDC, 0, 13, 0, 0);
      sharedUiInfo.serverStatusInfoScoreBoard.lines[2][5] = 0;
      LAN_GetServerStatus(0, 0, 0);
    }
    else if ( !*(unsigned int *)sharedUiInfo.pendingServerStatus.server[1].adrstr
           || *(int *)sharedUiInfo.pendingServerStatus.server[1].adrstr > uiInfo->uiDC.realTime )
    {
      return;
    }
    UI_UpdateDisplayServers(localClientNum, uiInfo);
    if ( *(int *)&sharedUiInfo.gap0[1124] >= 0
      && *(int *)&sharedUiInfo.gap0[1124] <= *(int *)&sharedUiInfo.gap0[81128]
      && *(unsigned int *)&sharedUiInfo.gap0[81128] )
    {
      if ( UI_GetServerStatusInfo(
             (char *)&sharedUiInfo.serverStatusInfo.lines[2][4],
             (serverStatusInfo_s *)&sharedUiInfo.serverStatusInfo.lines[2][6]) )
      {
        *(unsigned int *)sharedUiInfo.pendingServerStatus.server[1].adrstr = 0;
        UI_GetServerStatusInfo(&sharedUiInfo.serverStatus[82456], 0);
      }
      else
      {
        *(unsigned int *)sharedUiInfo.pendingServerStatus.server[1].adrstr = uiInfo->uiDC.realTime + 500;
      }
    }
  }
}

void __cdecl UI_BuildServerStatusScoreBoard(int localClientNum, uiInfo_s *uiInfo, int force)
{
  if ( force )
  {
    Menu_SetFeederSelection(localClientNum, &uiInfo->uiDC, 0, 91, 0, 0);
    sharedUiInfo.pendingServerStatus.server[0].valid = 0;
    LAN_GetServerStatusScoreBoard(0, 0, 0);
  }
  else if ( !*(unsigned int *)&sharedUiInfo.pendingServerStatus.server[1].adrstr[4]
         || *(int *)&sharedUiInfo.pendingServerStatus.server[1].adrstr[4] > uiInfo->uiDC.realTime )
  {
    return;
  }
  UI_UpdateDisplayServers(localClientNum, uiInfo);
  if ( *(int *)&sharedUiInfo.gap0[1124] >= 0
    && *(int *)&sharedUiInfo.gap0[1124] <= *(int *)&sharedUiInfo.gap0[81128]
    && *(unsigned int *)&sharedUiInfo.gap0[81128] )
  {
    if ( UI_GetServerStatusInfoScoreBoard(
           (char *)&sharedUiInfo.serverStatusInfo.lines[2][4],
           (serverStatusInfo_s *)&sharedUiInfo.serverStatusInfoScoreBoard.lines[2][6]) )
    {
      tries = 5;
      *(unsigned int *)&sharedUiInfo.pendingServerStatus.server[1].adrstr[4] = 0;
      UI_GetServerStatusInfoScoreBoard(&sharedUiInfo.serverStatus[82456], 0);
    }
    else if ( tries <= 0 )
    {
      tries = 5;
      *(unsigned int *)&sharedUiInfo.pendingServerStatus.server[1].adrstr[4] = 0;
    }
    else
    {
      *(unsigned int *)&sharedUiInfo.pendingServerStatus.server[1].adrstr[4] = uiInfo->uiDC.realTime + 500;
      --tries;
    }
  }
}

void __cdecl UI_DoServerRefresh(int localClientNum, uiInfo_s *uiInfo)
{
  bool wait; // [esp+0h] [ebp-4h]

  if ( *(unsigned int *)&sharedUiInfo.gap0[1120] )
  {
    wait = LAN_WaitServerResponse() != 0;
    if ( uiInfo->uiDC.realTime >= *(int *)&sharedUiInfo.gap0[1100] || !wait )
    {
      UI_UpdateDisplayServers(localClientNum, uiInfo);
      if ( LAN_UpdateDirtyPings(0, ui_netSource->current.unsignedInt) )
      {
        *(unsigned int *)&sharedUiInfo.gap0[1100] = uiInfo->uiDC.realTime + 2000;
        UI_BuildServerDisplayList(0, uiInfo, 2);
        UI_UpdateDisplayServers(0, uiInfo);
      }
      else if ( !wait )
      {
        UI_BuildServerDisplayList(localClientNum, uiInfo, 2);
        UI_StopServerRefresh();
      }
      UI_BuildServerDisplayList(localClientNum, uiInfo, 0);
    }
  }
}

void __cdecl UI_StartServerRefresh(int localClientNum, int contextIndex, int full)
{
  const char *MonthAbbrev; // eax
  char *v4; // eax
  char *v5; // eax
  char *v6; // eax
  int tm_mday; // [esp-10h] [ebp-58h]
  int v8; // [esp-Ch] [ebp-54h]
  int tm_hour; // [esp-8h] [ebp-50h]
  int tm_min; // [esp-4h] [ebp-4Ch]
  int controllerIndex; // [esp+0h] [ebp-48h]
  uiInfo_s *uiInfo; // [esp+4h] [ebp-44h]
  qtime_s q; // [esp+8h] [ebp-40h] BYREF
  char dvarName[24]; // [esp+2Ch] [ebp-1Ch] BYREF

  uiInfo = UI_UIContext_GetInfo(contextIndex);
  if ( localClientNum == -1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ui\\ui_server.cpp",
          619,
          0,
          "%s",
          "localClientNum != INVALID_CLIENT_NUMBER") )
  {
    __debugbreak();
  }
  controllerIndex = Com_LocalClient_GetControllerIndex(localClientNum);
  if ( controllerIndex == -1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ui\\ui_server.cpp",
          621,
          0,
          "%s",
          "controllerIndex != INVALID_CONTROLLER_PORT") )
  {
    __debugbreak();
  }
  Com_RealTime(&q, 1);
  _snprintf(dvarName, 0x18u, "ui_lastServerRefresh_%i", ui_netSource->current.integer);
  tm_min = q.tm_min;
  tm_hour = q.tm_hour;
  v8 = q.tm_year + 1900;
  tm_mday = q.tm_mday;
  MonthAbbrev = UI_GetMonthAbbrev(q.tm_mon);
  v4 = UI_SafeTranslateString(MonthAbbrev);
  v5 = va("%s %i, %i   %i:%02i", v4, tm_mday, v8, tm_hour, tm_min);
  Dvar_SetStringByName(dvarName, v5);
  if ( full )
  {
    *(_QWORD *)&sharedUiInfo.gap0[1120] = 1;
    *(unsigned int *)&sharedUiInfo.gap0[81140] = uiInfo->uiDC.realTime + 1000;
    UI_ClearDisplayedServers();
    LAN_MarkServerDirty(ui_netSource->current.integer, -1, 1u);
    LAN_ResetPings(ui_netSource->current.integer);
    *(unsigned int *)&sharedUiInfo.gap0[1100] = uiInfo->uiDC.realTime + 5000;
    cls.pingUpdateSource = ui_netSource->current.integer;
    v6 = va("findservers %i\n", ui_netSource->current.integer);
    Cmd_ExecuteSingleCommand(localClientNum, controllerIndex, v6);
  }
  else
  {
    UI_UpdatePendingPings(uiInfo);
  }
}

void __cdecl UI_StopServerRefresh()
{
  int count; // [esp+0h] [ebp-4h]

  if ( *(unsigned int *)&sharedUiInfo.gap0[1120] )
  {
    *(unsigned int *)&sharedUiInfo.gap0[1120] = 0;
    Com_Printf(
      13,
      "%d servers listed in browser with %d players.\n",
      *(unsigned int *)&sharedUiInfo.gap0[81128],
      *(unsigned int *)&sharedUiInfo.gap0[81136]);
    count = LAN_GetServerCount(ui_netSource->current.integer);
    if ( count - *(unsigned int *)&sharedUiInfo.gap0[81128] > 0 )
      Com_Printf(
        13,
        "%d servers not listed (filtered out by game browser settings)\n",
        count - *(unsigned int *)&sharedUiInfo.gap0[81128]);
  }
}

void __cdecl UI_ServersSort(__int64 column)
{
  if ( column != *(unsigned int *)&sharedUiInfo.gap0[1108] )
  {
    *(unsigned int *)&sharedUiInfo.gap0[1108] = column;
    qsort(
      &sharedUiInfo.gap0[1128],
      *(unsigned int *)&sharedUiInfo.gap0[81128],
      4u,
      (int (__cdecl *)(const void *, const void *))UI_ServersQsortCompare);
  }
}

int __cdecl UI_ServersQsortCompare(int *arg1, int *arg2)
{
  return LAN_CompareServers(
           ui_netSource->current.integer,
           *(int *)&sharedUiInfo.gap0[1108],
           *(int *)&sharedUiInfo.gap0[1112],
           *arg1,
           *arg2);
}

void __cdecl UI_AddServerToFavoritesList(const char *pszName, const char *pszAddress)
{
  char *v2; // eax
  char *v3; // eax
  char *v4; // eax
  char *v5; // eax
  char *v6; // eax
  char *v7; // eax
  const char *v8; // eax
  int res; // [esp+20h] [ebp-4h]

  if ( strlen(pszName) )
  {
    if ( strlen(pszAddress) )
    {
      res = LAN_AddServer(3, pszName, pszAddress);
      if ( res )
      {
        if ( res == -1 )
        {
          v5 = UI_SafeTranslateString("EXE_FAVORITELISTFULL");
          Com_Printf(13, "%s\n", v5);
          Dvar_SetStringByName("ui_favorite_message", "@EXE_FAVORITELISTFULL");
        }
        else if ( res == -2 )
        {
          v6 = UI_SafeTranslateString("EXE_BADSERVERADDRESS");
          Com_Printf(13, "%s\n", v6);
          Dvar_SetStringByName("ui_favorite_message", "@EXE_BADSERVERADDRESS");
        }
        else
        {
          v7 = UI_SafeTranslateString("EXE_FAVORITEADDED");
          v8 = va("%s\n", v7);
          Com_Printf(13, v8, pszAddress);
          Dvar_SetStringByName("ui_favorite_message", "@EXE_FAVORITEADDED");
        }
      }
      else
      {
        v4 = UI_SafeTranslateString("EXE_FAVORITEINLIST");
        Com_Printf(13, "%s\n", v4);
        Dvar_SetStringByName("ui_favorite_message", "@EXE_FAVORITEINLIST");
      }
    }
    else
    {
      v3 = UI_SafeTranslateString("EXE_FAVORITEADDRESSEMPTY");
      Com_Printf(13, "%s\n", v3);
      Dvar_SetStringByName("ui_favorite_message", "@EXE_FAVORITEADDRESSEMPTY");
    }
  }
  else
  {
    v2 = UI_SafeTranslateString("EXE_FAVORITENAMEEMPTY");
    Com_Printf(13, "%s\n", v2);
    Dvar_SetStringByName("ui_favorite_message", "@EXE_FAVORITENAMEEMPTY");
  }
}

void __cdecl UI_AddServerToFavoritesList(unsigned __int64 uid)
{
  Live_AddFavourite(0, uid);
}

void __cdecl UI_RemoveServerFromFavoritesList()
{
  char *v0; // eax
  unsigned __int64 uid; // [esp+0h] [ebp-410h] BYREF
  char buff[1028]; // [esp+8h] [ebp-408h] BYREF

  if ( *(int *)&sharedUiInfo.gap0[1124] >= 0 && *(int *)&sharedUiInfo.gap0[1124] < *(int *)&sharedUiInfo.gap0[81128] )
  {
    LAN_GetServerInfo(
      ui_netSource->current.integer,
      *(unsigned int *)&sharedUiInfo.gap0[4 * *(unsigned int *)&sharedUiInfo.gap0[1124] + 1128],
      buff,
      1024);
    v0 = Info_ValueForKey(buff, "dwuserid");
    StringToXUID(v0, &uid);
    Live_DeleteFavourite(uid);
  }
}

// local variable allocation has failed, the output may be wrong!
void __cdecl UI_DrawServerRefreshDate(
        int contextIndex,
        rectDef_s *rect,
        Font_s *font,
        float scale,
        float *color,
        int textStyle)
{
  double v6; // xmm0_8
  const char *v7; // eax
  const char *v8; // eax
  const char *VariantString; // eax
  long double v10; // [esp+1Ch] [ebp-48Ch] OVERLAPPED BYREF
  int offset; // [esp+64h] [ebp-444h]
  char hackString[1028]; // [esp+68h] [ebp-440h] BYREF
  int replaceCounts[2]; // [esp+470h] [ebp-38h] BYREF
  const char *first; // [esp+478h] [ebp-30h]
  float lowLight[4]; // [esp+47Ch] [ebp-2Ch] BYREF
  float newColor[4]; // [esp+48Ch] [ebp-1Ch] BYREF
  uiInfo_s *uiInfo; // [esp+49Ch] [ebp-Ch]
  const char *string; // [esp+4A0h] [ebp-8h]
  int serverCount; // [esp+4A4h] [ebp-4h]

  if ( *(unsigned int *)&sharedUiInfo.gap0[1120] )
  {
    lowLight[0] = 0.80000001 * *color;
    lowLight[1] = 0.80000001 * color[1];
    lowLight[2] = 0.80000001 * color[2];
    lowLight[3] = 0.80000001 * color[3];
    uiInfo = UI_UIContext_GetInfo(contextIndex);
    *(float *)&v10 = (float)(uiInfo->uiDC.realTime / 75);
    v6 = *(float *)&v10;
    __libm_sse2_sin(v10);
    *(float *)&v6 = v6;
    LerpColor(color, lowLight, newColor, (float)(*(float *)&v6 * 0.5) + 0.5);
    if ( LAN_WaitServerResponse() )
    {
      if ( Dvar_GetBool("dw_loggedin") )
        string = UI_SafeTranslateString("EXE_WAITINGFORMASTERSERVERRESPONSE");
      else
        string = UI_SafeTranslateString("PLATFORM_NOT_ONLINE");
    }
    else
    {
      serverCount = LAN_GetServerCount(ui_netSource->current.integer);
      string = UI_SafeTranslateString("EXE_GETTINGINFOFORSERVERS");
      I_strncpyz(hackString, string, 1024);
      strstr((unsigned __int8 *)string, "&&1");
      first = v7;
      if ( v7 )
      {
        offset = first - string + 3;
        hackString[offset++] = 47;
        hackString[offset++] = 38;
        hackString[offset++] = 38;
        hackString[offset++] = 50;
        I_strncpyz(&hackString[offset], &string[offset - 4], 1024 - offset);
      }
      replaceCounts[0] = cls.pingedServerCount;
      replaceCounts[1] = serverCount;
      string = UI_ReplaceConversionInts(hackString, 2, replaceCounts);
    }
    UI_DrawText(
      &scrPlaceView[contextIndex],
      (char *)string,
      0x7FFFFFFF,
      font,
      rect->x,
      rect->y,
      rect->horzAlign,
      rect->vertAlign,
      scale,
      newColor,
      textStyle);
  }
  else
  {
    v8 = va("ui_lastServerRefresh_%i", ui_netSource->current.integer);
    VariantString = Dvar_GetVariantString(v8);
    I_strncpyz((char *)&v10 + 4, VariantString, 64);
    string = UI_SafeTranslateString("EXE_REFRESHTIME");
    string = UI_ReplaceConversionString((char *)string, (const char *)&v10 + 4);
    UI_DrawText(
      &scrPlaceView[contextIndex],
      (char *)string,
      0x7FFFFFFF,
      font,
      rect->x,
      rect->y,
      rect->horzAlign,
      rect->vertAlign,
      scale,
      color,
      textStyle);
  }
}

void __cdecl UI_UpdateServerToolTips(int localClientNum, listBoxDef_s *listPtr)
{
  char *v2; // eax
  char *v3; // eax
  char *v4; // eax
  char *v5; // eax
  char *v6; // eax
  char *v7; // eax
  char *v8; // eax
  char *v9; // eax
  char *v10; // eax
  float deltaX; // [esp+20h] [ebp-50h]
  int iswager; // [esp+24h] [ebp-4Ch]
  float v13; // [esp+2Ch] [ebp-44h]
  uiInfo_s *uiInfo; // [esp+30h] [ebp-40h]
  itemDef_s *serversListItem; // [esp+34h] [ebp-3Ch]
  signed int time; // [esp+38h] [ebp-38h]
  bool showMap; // [esp+3Fh] [ebp-31h]
  char *toolTipText; // [esp+40h] [ebp-30h]
  int i; // [esp+44h] [ebp-2Ch]
  int ia; // [esp+44h] [ebp-2Ch]
  menuDef_t *serverBrowserMenu; // [esp+48h] [ebp-28h]
  rectDef_s *columnRect; // [esp+4Ch] [ebp-24h]
  rectDef_s rect; // [esp+54h] [ebp-1Ch] BYREF
  float dy; // [esp+6Ch] [ebp-4h]

  time = Sys_Milliseconds();
  serverBrowserMenu = 0;
  serversListItem = 0;
  uiInfo = UI_GetInfo(localClientNum);
  toolTipText = 0;
  showMap = 0;
  if ( !uiInfo && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ui\\ui_server.cpp", 889, 0, "%s", "uiInfo") )
    __debugbreak();
  if ( !listPtr && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ui\\ui_server.cpp", 890, 0, "%s", "listPtr") )
    __debugbreak();
  v13 = uiInfo->uiDC.cursor.x - uiInfo->uiDC.prevCursor.x;
  dy = uiInfo->uiDC.cursor.y - uiInfo->uiDC.prevCursor.y;
  if ( COERCE_FLOAT(LODWORD(v13) & _mask__AbsFloat_) <= 0.001 && COERCE_FLOAT(LODWORD(dy) & _mask__AbsFloat_) <= 0.001 )
  {
    if ( ui_showToolTipTime )
    {
      if ( time >= ui_showToolTipTime )
      {
        ui_toolAllowTimer = 0;
        ui_showToolTipTime = 0;
        if ( listPtr->mousePos >= 0 && listPtr->mousePos < *(int *)&sharedUiInfo.gap0[81128] )
        {
          for ( i = uiInfo->uiDC.openMenuCount - 1; i >= 0 && !serversListItem; --i )
          {
            serverBrowserMenu = uiInfo->uiDC.menuStack[i].menu;
            if ( serverBrowserMenu )
              serversListItem = Menu_FindItemByName(serverBrowserMenu, "browser");
          }
          if ( serversListItem
            && Rect_ContainsPoint(
                 uiInfo->uiDC.contextIndex,
                 &serversListItem->window.rect,
                 uiInfo->uiDC.cursor.x,
                 uiInfo->uiDC.cursor.y) )
          {
            for ( ia = 0; ia < ui_toolTipCount; ++ia )
            {
              columnRect = &listPtr->columnInfo[ui_toolTipColumns[ia]].rect;
              rect.x = serversListItem->window.rect.x + columnRect->x;
              rect.y = (float)((float)(listPtr->mousePos - listPtr->startPos[uiInfo->uiDC.contextIndex])
                             * listPtr->elementHeight)
                     + serversListItem->window.rect.y;
              rect.w = columnRect->w;
              rect.h = columnRect->h;
              rect.horzAlign = serversListItem->window.rect.horzAlign;
              rect.vertAlign = serversListItem->window.rect.vertAlign;
              if ( Rect_ContainsPoint(uiInfo->uiDC.contextIndex, &rect, uiInfo->uiDC.cursor.x, uiInfo->uiDC.cursor.y) )
              {
                LAN_GetServerInfo(
                  ui_netSource->current.integer,
                  *(unsigned int *)&sharedUiInfo.gap0[4 * listPtr->mousePos + 1128],
                  info_8,
                  1024);
                switch ( ui_toolTipColumns[ia] )
                {
                  case 0:
                    v2 = Info_ValueForKey(info_8, "wager");
                    iswager = atoi(v2);
                    v3 = Info_ValueForKey(info_8, "licensetype");
                    switch ( atoi(v3) )
                    {
                      case 2:
                        if ( iswager )
                          v5 = UI_SafeTranslateString("PLATFORM_WAGER_MATCH");
                        else
                          v5 = UI_SafeTranslateString("PLATFORM_RANKED_SERVER");
                        toolTipText = v5;
                        break;
                      case 4:
                        if ( iswager )
                          v4 = UI_SafeTranslateString("PLATFORM_OFFICIAL_WAGER_MATCH");
                        else
                          v4 = UI_SafeTranslateString("PLATFORM_OFFICIAL_RANKED_SERVER");
                        toolTipText = v4;
                        break;
                      case 5:
                        toolTipText = UI_SafeTranslateString("PLATFORM_LISTEN_SERVER");
                        break;
                      default:
                        toolTipText = UI_SafeTranslateString("PLATFORM_UNRANKED_SERVER");
                        break;
                    }
                    break;
                  case 1:
                    v6 = Info_ValueForKey(info_8, "pb");
                    if ( atoi(v6) )
                      v7 = UI_SafeTranslateString("PLATFORM_VAC_SERVER");
                    else
                      v7 = UI_SafeTranslateString("PLATFORM_NON_VAC");
                    toolTipText = v7;
                    break;
                  case 2:
                    v8 = Info_ValueForKey(info_8, "pswrd");
                    if ( atoi(v8) )
                      v9 = UI_SafeTranslateString("PLATFORM_PASSWORD_PROTECTED");
                    else
                      v9 = UI_SafeTranslateString("PLATFORM_PUBLIC");
                    toolTipText = v9;
                    break;
                  case 3:
                  case 5:
                    if ( ui_netSource->current.integer == 4 )
                    {
                      toolTipText = Info_ValueForKey(info_8, "friendsOnServer");
                      if ( !toolTipText || !*toolTipText )
                        toolTipText = (char *)&toastPopupTitle;
                    }
                    break;
                  case 4:
                    showMap = 1;
                    toolTipText = Info_ValueForKey(info_8, "mapname");
                    break;
                  case 6:
                    if ( Info_ValueForKey(info_8, "gametype") && *Info_ValueForKey(info_8, "gametype") )
                    {
                      v10 = Info_ValueForKey(info_8, "gametype");
                      toolTipText = UI_GetGameTypeDisplayName(v10);
                    }
                    break;
                  case 7:
                    toolTipText = Info_ValueForKey(info_8, "geolocation");
                    if ( !toolTipText || !*toolTipText )
                    {
                      toolTipText = Info_ValueForKey(info_8, "countrycode");
                      if ( !toolTipText || !*toolTipText )
                        toolTipText = UI_SafeTranslateString("PLATFORM_GEO_NOT_SPECIFIED");
                    }
                    break;
                  default:
                    break;
                }
                if ( showMap )
                {
                  UI_UpdateContextItem(
                    &uiInfo->uiDC,
                    serverBrowserMenu->window.name,
                    "server_browser_maptip_back",
                    10.0,
                    10.0);
                  UI_UpdateContextItem(
                    &uiInfo->uiDC,
                    serverBrowserMenu->window.name,
                    "server_browser_maptip_map",
                    10.0,
                    10.0);
                  Dvar_SetBoolIfChanged((dvar_s *)ui_browserShowMapTip, 1);
                }
                else
                {
                  deltaX = 2.6666667
                         / (float)(*(float *)&dword_2D9E6C0[30 * uiInfo->uiDC.contextIndex]
                                 / *(float *)&dword_2D9E6C4[30 * uiInfo->uiDC.contextIndex]);
                  UI_UpdateContextItem(
                    &uiInfo->uiDC,
                    serverBrowserMenu->window.name,
                    "server_browser_tooltip_back",
                    10.0,
                    10.0);
                  UI_UpdateContextItem(
                    &uiInfo->uiDC,
                    serverBrowserMenu->window.name,
                    "server_browser_tooltip_content",
                    deltaX + 10.0,
                    10.0);
                  if ( toolTipText )
                    Dvar_SetBoolIfChanged((dvar_s *)ui_browserShowToolTip, 1);
                  else
                    Dvar_SetBoolIfChanged((dvar_s *)ui_browserShowToolTip, 0);
                }
                if ( toolTipText )
                  Dvar_SetString((dvar_s *)ui_browserToolTip, toolTipText);
                return;
              }
            }
          }
        }
      }
    }
    else if ( ui_toolAllowTimer )
    {
      ui_showToolTipTime = time + 500;
    }
  }
  else
  {
    Dvar_SetBoolIfChanged((dvar_s *)ui_browserShowToolTip, 0);
    Dvar_SetBoolIfChanged((dvar_s *)ui_browserShowMapTip, 0);
    ui_showToolTipTime = 0;
    ui_toolAllowTimer = 1;
  }
}

