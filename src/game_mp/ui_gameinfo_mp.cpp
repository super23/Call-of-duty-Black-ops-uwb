#include "ui_gameinfo_mp.h"

int __cdecl UI_ParseInfos(const char *buf, int max, char **infos)
{
  unsigned __int8 *v3; // eax
  char v5; // [esp+3h] [ebp-865h]
  char *v6; // [esp+8h] [ebp-860h]
  char *v7; // [esp+Ch] [ebp-85Ch]
  unsigned int v8; // [esp+10h] [ebp-858h]
  char info[1024]; // [esp+58h] [ebp-810h] BYREF
  char key[1028]; // [esp+458h] [ebp-410h] BYREF
  const char *token; // [esp+860h] [ebp-8h]
  int count; // [esp+864h] [ebp-4h]

  count = 0;
  while ( 1 )
  {
    token = (const char *)Com_Parse(&buf);
    if ( !*token )
      return count;
    if ( strcmp(token, "{") )
    {
      Com_Printf(13, "Missing { in info file\n");
      return count;
    }
    if ( count == max )
    {
      Com_Printf(13, "Max infos exceeded\n");
      return count;
    }
    info[0] = 0;
    while ( 1 )
    {
      token = (const char *)Com_Parse(&buf);
      if ( !*token )
        break;
      if ( !strcmp(token, "}") )
        goto LABEL_14;
      I_strncpyz(key, token, 1024);
      token = (const char *)Com_ParseOnLine(&buf);
      if ( !*token )
        token = "<NULL>";
      Info_SetValueForKey(info, key, token);
    }
    Com_Printf(13, "Unexpected end of info file\n");
LABEL_14:
    v8 = strlen(va("%d", 128));
    v3 = UI_Alloc(strlen(info) + v8 + 6, 1);
    infos[count] = (char *)v3;
    if ( infos[count] )
    {
      v7 = info;
      v6 = infos[count];
      do
      {
        v5 = *v7;
        *v6++ = *v7++;
      }
      while ( v5 );
      ++count;
    }
  }
}

void __cdecl UI_LoadArenas()
{
  char *v0; // eax
  char *v1; // eax
  char *v2; // eax
  char *v3; // eax
  const char *v4; // eax
  bool v5; // [esp+0h] [ebp-2034h]
  char v6; // [esp+4h] [ebp-2030h]
  char *data_p; // [esp+201Ch] [ebp-18h] BYREF
  char *v9; // [esp+2020h] [ebp-14h]
  int j; // [esp+2024h] [ebp-10h]
  int i; // [esp+2028h] [ebp-Ch]
  char ModArenas; // [esp+202Fh] [ebp-5h]
  char *s0; // [esp+2030h] [ebp-4h]

  sharedUiInfo.joinGameTypes[31].basictraining = 0;
  ui_numArenas = 0;
  ModArenas = 0;
  if ( fs_gameDirVar && *(_BYTE *)fs_gameDirVar->current.integer )
    ModArenas = UI_LoadModArenas();
  if ( !ModArenas || (!fs_gameDirVar || !*(_BYTE *)fs_gameDirVar->current.integer ? (v6 = 0) : (v6 = 1), !v6) )
    UI_LoadArenasFromFile();
  v5 = fs_gameDirVar && *(_BYTE *)fs_gameDirVar->current.integer;
  if ( v5 && !ModArenas )
  {
    UI_LoadModsMap(FFD_MOD_DIR);
    UI_LoadModsMap(FFD_USER_MAP);
  }
  ui_browserMapNameTable[0] = UI_SafeTranslateString("MENU_ALL");
  for ( i = 0; i < ui_numArenas; ++i )
  {
    v0 = Info_ValueForKey(ui_arenaInfos[i], "map");
    I_strncpyz(&sharedUiInfo.mapList[sharedUiInfo.joinGameTypes[31].basictraining].mapName[28], v0, 24);
    v1 = Info_ValueForKey(ui_arenaInfos[i], "longname");
    I_strncpyz(&sharedUiInfo.joinGameTypes[32].gameType[304 * sharedUiInfo.joinGameTypes[31].basictraining], v1, 32);
    v2 = Info_ValueForKey(ui_arenaInfos[i], "loadname");
    I_strncpyz(&sharedUiInfo.mapList[sharedUiInfo.joinGameTypes[31].basictraining].mapLoadName[20], v2, 32);
    sharedUiInfo.mapList[sharedUiInfo.joinGameTypes[31].basictraining].timeToBeat[31] = 0;
    v3 = Info_ValueForKey(ui_arenaInfos[i], "loadscreen");
    I_strncpyz((char *)&sharedUiInfo.mapList[sharedUiInfo.joinGameTypes[31].basictraining].mapPackTypeIndex, v3, 42);
    v9 = Info_ValueForKey(ui_arenaInfos[i], "gametype");
    if ( !v9 )
      goto LABEL_35;
    if ( *v9 )
    {
      *(unsigned int *)sharedUiInfo.contentPackList[19 * sharedUiInfo.joinGameTypes[31].basictraining - 2422].mapPackName = 0;
      v4 = va(".arena files : %s", &sharedUiInfo.mapList[sharedUiInfo.joinGameTypes[31].basictraining].mapName[28]);
      Com_BeginParseSession(v4);
      data_p = v9;
      while ( 1 )
      {
        s0 = (char *)Com_Parse((const char **)&data_p);
        if ( !s0 || !*s0 )
          break;
        for ( j = 0; j < sharedUiInfo.playerClientNums[31]; ++j )
        {
          if ( !I_stricmp(s0, (const char *)&sharedUiInfo.playerClientNums[29 * j + 32]) )
            *(unsigned int *)sharedUiInfo.contentPackList[19 * sharedUiInfo.joinGameTypes[31].basictraining - 2422].mapPackName |= 1 << j;
        }
      }
      Com_EndParseSession();
    }
    else
    {
LABEL_35:
      *(unsigned int *)sharedUiInfo.contentPackList[19 * sharedUiInfo.joinGameTypes[31].basictraining - 2422].mapPackName = -1;
    }
    dword_98A4D5C[sharedUiInfo.joinGameTypes[31].basictraining] = (int)UI_SafeTranslateString(&sharedUiInfo.joinGameTypes[32].gameType[304 * sharedUiInfo.joinGameTypes[31].basictraining]);
    if ( ++sharedUiInfo.joinGameTypes[31].basictraining >= 128 )
      break;
  }
  dword_98A4D5C[sharedUiInfo.joinGameTypes[31].basictraining] = 0;
  ui_browserMap = _Dvar_RegisterEnum(
                    "ui_browserMap",
                    ui_browserMapNameTable,
                    0,
                    1u,
                    "Map preferred in the server browser filter.");
}

char __cdecl UI_LoadModArenas()
{
  int len; // [esp+0h] [ebp-2010h]
  unsigned __int8 buffer[8192]; // [esp+8h] [ebp-2008h] BYREF
  int file; // [esp+200Ch] [ebp-4h] BYREF

  len = FS_FOpenFileRead("mod.arena", &file);
  if ( file )
  {
    if ( len )
    {
      if ( len <= 0x2000 )
      {
        FS_Read(buffer, len, file);
        FS_FCloseFile(file);
        ui_numArenas = UI_ParseInfos((const char *)buffer, 128 - ui_numArenas, (char **)(4 * ui_numArenas + 160058192));
        return 1;
      }
      else
      {
        Com_PrintWarning(13, "Customized arena file size is too big to load > %d: %s\n", 0x2000, "mod.arena");
        return 0;
      }
    }
    else
    {
      Com_PrintWarning(13, "Customized mod.arena file is empty\n", "mod.arena");
      return 0;
    }
  }
  else
  {
    Com_PrintWarning(13, "Customized arena file not found: %s\n", "mod.arena");
    return 0;
  }
}

void __cdecl UI_LoadModsMap(FF_DIR source)
{
  char v1; // [esp+3h] [ebp-5A9h]
  char *v2; // [esp+8h] [ebp-5A4h]
  char *v3; // [esp+Ch] [ebp-5A0h]
  unsigned int v4; // [esp+10h] [ebp-59Ch]
  const char *v5; // [esp+40h] [ebp-56Ch]
  const char *string; // [esp+44h] [ebp-568h]
  int i; // [esp+68h] [ebp-544h]
  char ainfo[1028]; // [esp+6Ch] [ebp-540h] BYREF
  char *name; // [esp+470h] [ebp-13Ch]
  int nameLength; // [esp+474h] [ebp-138h]
  bool IsDupName; // [esp+47Bh] [ebp-131h]
  int id; // [esp+47Ch] [ebp-130h]
  const char *loadSuffix; // [esp+480h] [ebp-12Ch]
  int loadSuffixLength; // [esp+484h] [ebp-128h]
  const char *baseDir; // [esp+488h] [ebp-124h]
  char netpath[256]; // [esp+48Ch] [ebp-120h] BYREF
  const char *basePath; // [esp+590h] [ebp-1Ch]
  int isMod; // [esp+594h] [ebp-18h]
  const char *localizedPrefix; // [esp+598h] [ebp-14h]
  int numSysFiles; // [esp+59Ch] [ebp-10h] BYREF
  int localizedPrefixLength; // [esp+5A0h] [ebp-Ch]
  const char *filter; // [esp+5A4h] [ebp-8h]
  char **sysFiles; // [esp+5A8h] [ebp-4h]

  if ( source != FFD_MOD_DIR && source != FFD_USER_MAP )
    return;
  isMod = source == FFD_MOD_DIR;
  loadSuffix = "_load";
  loadSuffixLength = strlen("_load");
  localizedPrefix = "localized_";
  localizedPrefixLength = strlen("localized_");
  basePath = fs_homepath->current.string;
  if ( source == FFD_MOD_DIR )
    string = fs_gameDirVar->current.string;
  else
    string = "usermaps";
  baseDir = string;
  if ( isMod )
    v5 = "ff";
  else
    v5 = "/";
  filter = v5;
  Com_sprintf(netpath, 0x100u, "%s/%s", basePath, baseDir);
  sysFiles = (char **)Sys_ListFiles(netpath, (char *)filter, 0, &numSysFiles, 0);
  for ( id = 0; id < numSysFiles && ui_numArenas < 128; ++id )
  {
    name = sysFiles[id];
    nameLength = strlen(name);
    if ( nameLength <= 3 )
      continue;
    if ( isMod )
    {
      nameLength -= 3;
      name[nameLength] = 0;
    }
    else if ( !DB_FileExists(name, source) )
    {
      continue;
    }
    if ( I_stricmp(name, "mod")
      && I_stricmp(name, "common_mp")
      && I_stricmp(name, "code_post_gfx_mp")
      && I_stricmp(name, "ui_mp")
      && !I_strnicmp(name, "mp_", 3)
      && (nameLength < loadSuffixLength || I_stricmp(&name[nameLength - loadSuffixLength], loadSuffix))
      && (nameLength < localizedPrefixLength || I_stricmp(&name[nameLength - localizedPrefixLength], localizedPrefix)) )
    {
      IsDupName = 0;
      Com_sprintf(ainfo, 0x400u, "\\longname\\%s\\loadname\\", name);
      for ( i = 0; i < ui_numArenas; ++i )
      {
        if ( I_stristr(ui_arenaInfos[i], ainfo) )
        {
          IsDupName = 1;
          break;
        }
      }
      if ( !IsDupName )
      {
        Com_sprintf(
          ainfo,
          0x400u,
          "map\\%s\\loadscreen\\loadscreen_mp_temp\\longname\\%s\\loadname\\%s\\gametype\\dm tdm sd sab dom koth twar war"
          " sur ctf vdm vtdm mini",
          name,
          name,
          name);
        v4 = strlen(va("%d", 128));
        ui_arenaInfos[ui_numArenas] = (char *)UI_Alloc(strlen(ainfo) + v4 + 6, 1);
        v3 = ainfo;
        v2 = ui_arenaInfos[ui_numArenas];
        do
        {
          v1 = *v3;
          *v2++ = *v3++;
        }
        while ( v1 );
        ++ui_numArenas;
      }
    }
  }
  if ( ui_numArenas >= 128 )
    Com_Printf(13, "Warning: Max number of arenas loaded: %d\n", 128);
  FS_FreeFileList((const char **)sysFiles);
}

int UI_LoadArenasFromFile()
{
  if ( useFastFile->current.enabled )
    return ((int (__cdecl *)(void (*)()))UI_LoadArenasFromFile_FastFile)(UI_LoadArenasFromFile_FastFile);
  else
    return ((int (__cdecl *)(const char *(*)()))UI_LoadArenasFromFile_LoadObj)(UI_LoadArenasFromFile_LoadObj);
}

const char *UI_LoadArenasFromFile_LoadObj()
{
  const char *result; // eax
  const char *v1; // [esp+14h] [ebp-24A4h]
  char string[132]; // [esp+18h] [ebp-24A0h] BYREF
  char *v3; // [esp+9Ch] [ebp-241Ch]
  char listbuf[1024]; // [esp+A0h] [ebp-2418h] BYREF
  char buffer[8196]; // [esp+4A0h] [ebp-2018h] BYREF
  int len; // [esp+24A8h] [ebp-10h]
  int f; // [esp+24ACh] [ebp-Ch] BYREF
  int v8; // [esp+24B0h] [ebp-8h]
  unsigned int v9; // [esp+24B4h] [ebp-4h]

  ui_numArenas = 0;
  result = (const char *)FS_GetFileList("mp", "arena", FS_LIST_PURE_ONLY, listbuf, 1024);
  v1 = result;
  v3 = listbuf;
  v8 = 0;
  while ( v8 < (int)v1 )
  {
    v9 = strlen(v3);
    sprintf(string, "%s/%s", "mp", v3);
    len = FS_FOpenFileByMode(string, &f, FS_READ);
    if ( f )
    {
      if ( len < 0x2000 )
      {
        FS_Read((unsigned __int8 *)buffer, len, f);
        buffer[len] = 0;
        FS_FCloseFile(f);
        ui_numArenas += UI_ParseInfos(buffer, 128 - ui_numArenas, (char **)(4 * ui_numArenas + 160058192));
      }
      else
      {
        Com_PrintError(13, "file too large: %s is %i, max allowed is %i", string, len, 0x2000);
        FS_FCloseFile(f);
      }
    }
    else
    {
      Com_PrintError(13, "file not found: %s\n", string);
    }
    ++v8;
    result = &v3[v9 + 1];
    v3 = (char *)result;
  }
  return result;
}

void UI_LoadArenasFromFile_FastFile()
{
  RawFile *rawfile; // [esp+8h] [ebp-8h]

  rawfile = DB_FindXAssetHeader(ASSET_TYPE_RAWFILE, "mp/mpmaps.arena", 1, -1).rawfile;
  if ( rawfile )
    ui_numArenas = UI_ParseInfos(rawfile->buffer, 128 - ui_numArenas, (char **)(4 * ui_numArenas + 160058192));
  else
    Com_PrintError(13, "file not found: %s\n", "mp/mpmaps.arena");
}

void __cdecl UI_LoadMaps()
{
  const char *v0; // eax
  const char *v1; // eax
  const char *v2; // eax
  const char *v3; // eax
  const char *v4; // eax
  const char *v5; // eax
  const char *v6; // eax
  int j; // [esp+0h] [ebp-3Ch]
  int i; // [esp+4h] [ebp-38h]
  char tableName[20]; // [esp+8h] [ebp-34h] BYREF
  const char *dlcMapPackIndex; // [esp+20h] [ebp-1Ch]
  const StringTable *table; // [esp+24h] [ebp-18h] BYREF
  char packIndex[4]; // [esp+28h] [ebp-14h] BYREF
  int mapPackIndex; // [esp+2Ch] [ebp-10h]
  char index[4]; // [esp+30h] [ebp-Ch] BYREF
  int mapPackCount; // [esp+34h] [ebp-8h]
  int count; // [esp+38h] [ebp-4h]

  I_strncpyz(tableName, "mp/mapsTable.csv", 18);
  StringTable_GetAsset(tableName, (XAssetHeader *)&table);
  if ( !table
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ui_mp\\ui_gameinfo_mp.cpp", 434, 0, "%s", "table") )
  {
    __debugbreak();
  }
  memset((unsigned __int8 *)&sharedUiInfo.mapCount, 0, 0x9800u);
  v0 = StringTable_Lookup(table, 0, "maxnum_map", 1);
  sharedUiInfo.joinGameTypes[31].basictraining = atoi(v0);
  v1 = StringTable_Lookup(table, 0, "mappack_count", 1);
  mapPackCount = atoi(v1);
  for ( i = 0; i < mapPackCount; ++i )
  {
    Com_sprintf(packIndex, 4u, "%i", i);
    v2 = StringTable_Lookup(table, 1, packIndex, 0);
    I_strncpyz((char *)&sharedUiInfo.mapList[127].active + 16 * i, v2, 16);
  }
  count = 0;
  for ( j = 0; j < sharedUiInfo.joinGameTypes[31].basictraining; ++j )
  {
    Com_sprintf(index, 4u, "%i", j);
    dlcMapPackIndex = StringTable_Lookup(table, 5, index, 11);
    mapPackIndex = atoi(dlcMapPackIndex);
    v3 = StringTable_Lookup(table, 5, index, 0);
    I_strncpyz(&sharedUiInfo.mapList[count].mapName[28], v3, 24);
    v4 = StringTable_Lookup(table, 5, index, 3);
    I_strncpyz(&sharedUiInfo.joinGameTypes[32].gameType[304 * count], v4, 32);
    v5 = StringTable_Lookup(table, 1, dlcMapPackIndex, 0);
    I_strncpyz((char *)&sharedUiInfo.mapList[count].splitscreen, v5, 16);
    *(unsigned int *)&sharedUiInfo.mapList[count].mapPackType[12] = mapPackIndex;
    Com_sprintf(
      &sharedUiInfo.mapList[count].mapLoadName[20],
      0x20u,
      "%s%s",
      &sharedUiInfo.joinGameTypes[32].gameType[304 * count],
      "_CAPS");
    v6 = StringTable_Lookup(table, 5, index, 10);
    if ( I_strcmp(v6, "YES") )
      *(unsigned int *)&sharedUiInfo.mapList[count].mapNameCaps[28] = 0;
    else
      *(unsigned int *)&sharedUiInfo.mapList[count].mapNameCaps[28] = 1;
    ++count;
  }
  sharedUiInfo.joinGameTypes[31].basictraining = count;
  Dvar_SetIntByName("ui_mapCount", count);
}

void __cdecl UI_LoadCustomMatchGameTypes()
{
  const char *v0; // eax
  const char *v1; // eax
  const char *v2; // eax
  const char *v3; // eax
  const char *v4; // eax
  int i; // [esp+0h] [ebp-28h]
  char tableName[24]; // [esp+4h] [ebp-24h] BYREF
  const StringTable *table; // [esp+20h] [ebp-8h] BYREF
  char index[4]; // [esp+24h] [ebp-4h] BYREF

  I_strncpyz(tableName, "mp/gametypesTable.csv", 23);
  StringTable_GetAsset(tableName, (XAssetHeader *)&table);
  if ( !table
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ui_mp\\ui_gameinfo_mp.cpp", 486, 0, "%s", "table") )
  {
    __debugbreak();
  }
  v0 = StringTable_Lookup(table, 0, "maxnum_gametype", 1);
  sharedUiInfo.customGameTypes[31].splitscreen = atoi(v0);
  for ( i = 0; i < sharedUiInfo.customGameTypes[31].splitscreen; ++i )
  {
    Com_sprintf(index, 4u, "%i", i);
    v1 = StringTable_Lookup(table, 4, index, 0);
    I_strncpyz(&sharedUiInfo.customGameTypes[32].gameType[116 * i], v1, 12);
    v2 = StringTable_Lookup(table, 4, index, 1);
    I_strncpyz((char *)&sharedUiInfo.gameTypeMapCount[29 * i - 926], v2, 32);
    v3 = StringTable_Lookup(table, 4, index, 5);
    if ( I_strcmp(v3, "YES") )
      sharedUiInfo.gameTypeMapCount[29 * i - 902] = 0;
    else
      sharedUiInfo.gameTypeMapCount[29 * i - 902] = 1;
    v4 = StringTable_Lookup(table, 4, index, 6);
    if ( I_strcmp(v4, "YES") )
      sharedUiInfo.gameTypeMapCount[29 * i - 901] = 0;
    else
      sharedUiInfo.gameTypeMapCount[29 * i - 901] = 1;
  }
}

