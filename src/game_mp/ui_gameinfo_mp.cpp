#include "ui_gameinfo_mp.h"
#include <universal/q_parse.h>
#include <ui/ui_utils.h>
#include <universal/com_files.h>
#include <qcommon/common.h>
#include <universal/com_stringtable.h>

char *ui_arenaInfos[128];
int ui_numArenas;
const char *ui_browserMapNameTable[129];

const dvar_t *ui_browserMap;

// Decomp: CoDMPServer.c:534015
int __cdecl UI_ParseInfos(const char *buf, int max, char **infos)
{
    unsigned __int8 *tmp3;
    char ch;
    char *idx;
    char *idx2;
    unsigned int tmp8;
    char info[1024];
    char key[1028];
    const char *token;
    int count;

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
        tmp8 = strlen(va("%d", 128));
        tmp3 = UI_Alloc(strlen(info) + tmp8 + 6, 1);
        infos[count] = (char *)tmp3;
        if ( infos[count] )
        {
            idx2 = info;
            idx = infos[count];
            do
            {
                ch = *idx2;
                *idx++ = *idx2++;
            }
            while ( ch );
            ++count;
        }
    }
}

// Decomp: CoDMPServer.c:534078
void __cdecl UI_LoadArenas()
{
    char *tmp1;
    char *tmp2;
    char *tmp3;
    char *tmp4;
    char *fmtMsg;
    bool tmp6;
    char tmp7;
    char *data_p;
    char *tmp10;
    int j;
    int i;
    char ModArenas;
    char *s0;

    sharedUiInfo.mapCount = 0;
    ui_numArenas = 0;
    ModArenas = 0;
    if (fs_gameDirVar && *(_BYTE *)fs_gameDirVar->current.integer)
        ModArenas = UI_LoadModArenas();
    if (!ModArenas || (!fs_gameDirVar || !*(_BYTE *)fs_gameDirVar->current.integer ? (tmp7 = 0) : (tmp7 = 1), !tmp7))
        UI_LoadArenasFromFile();
    tmp6 = fs_gameDirVar && *(_BYTE *)fs_gameDirVar->current.integer;
    if (tmp6 && !ModArenas)
    {
        UI_LoadModsMap(FFD_MOD_DIR);
        UI_LoadModsMap(FFD_USER_MAP);
    }
    ui_browserMapNameTable[0] = UI_SafeTranslateString("MENU_ALL");
    for (i = 0; i < ui_numArenas; ++i)
    {
        tmp1 = Info_ValueForKey(ui_arenaInfos[i], "map");
        I_strncpyz(sharedUiInfo.mapList[sharedUiInfo.mapCount].mapLoadName, tmp1, 24);
        tmp2 = Info_ValueForKey(ui_arenaInfos[i], "longname");
        I_strncpyz(sharedUiInfo.mapList[sharedUiInfo.mapCount].mapName, tmp2, 32);
        tmp3 = Info_ValueForKey(ui_arenaInfos[i], "loadname");
        I_strncpyz(sharedUiInfo.mapList[sharedUiInfo.mapCount].mapNameCaps, tmp3, 32);
        sharedUiInfo.mapList[sharedUiInfo.mapCount].levelShot = 0;
        tmp4 = Info_ValueForKey(ui_arenaInfos[i], "loadscreen");
        I_strncpyz(sharedUiInfo.mapList[sharedUiInfo.mapCount].imageName, tmp4, 42);
        tmp10 = Info_ValueForKey(ui_arenaInfos[i], "gametype");
        if (!tmp10)
            goto LABEL_35;
        if (*tmp10)
        {
            sharedUiInfo.mapList[sharedUiInfo.mapCount].typeBits = 0;
            fmtMsg = va(".arena files : %s", sharedUiInfo.mapList[sharedUiInfo.mapCount].mapLoadName);
            Com_BeginParseSession(fmtMsg);
            data_p = tmp10;
            while (1)
            {
                s0 = (char *)Com_Parse((const char **)&data_p);
                if (!s0 || !*s0)
                    break;
                for (j = 0; j < sharedUiInfo.numGameTypes; ++j)
                {
                    if (!I_stricmp(s0, sharedUiInfo.gameTypes[j].gameType))
                        sharedUiInfo.mapList[sharedUiInfo.mapCount].typeBits |= 1 << j;
                }
            }
            Com_EndParseSession();
        }
        else
        {
        LABEL_35:
            sharedUiInfo.mapList[sharedUiInfo.mapCount].typeBits = -1;
        }
        ui_browserMapNameTable[sharedUiInfo.mapCount + 1] = UI_SafeTranslateString(sharedUiInfo.mapList[sharedUiInfo.mapCount].mapName);
        if (++sharedUiInfo.mapCount >= 128)
            break;
    }
    ui_browserMapNameTable[sharedUiInfo.mapCount + 1] = 0;
    ui_browserMap = _Dvar_RegisterEnum(
        "ui_browserMap",
        ui_browserMapNameTable,
        0,
        1u,
        "Map preferred in the server browser filter.");
}

// Decomp: CoDMPServer.c:534166
char __cdecl UI_LoadModArenas()
{
    int len;
    unsigned __int8 buffer[8192];
    int file;

    len = FS_FOpenFileRead("mod.arena", &file);
    if ( file )
    {
        if ( len )
        {
            if ( len <= 0x2000 )
            {
                FS_Read(buffer, len, file);
                FS_FCloseFile(file);
                ui_numArenas = UI_ParseInfos((const char *)buffer, 128 - ui_numArenas, &ui_arenaInfos[ui_numArenas]);
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

// Decomp: CoDMPServer.c:534206
void __cdecl UI_LoadModsMap(FF_DIR source)
{
    char ch;
    char *idx;
    char *idx2;
    unsigned int tmp4;
    const char *tmp5;
    const char *string;
    int i;
    char ainfo[1028];
    char *name;
    int nameLength;
    bool IsDupName;
    int id;
    const char *loadSuffix;
    int loadSuffixLength;
    const char *baseDir;
    char netpath[256];
    const char *basePath;
    int isMod;
    const char *localizedPrefix;
    int numSysFiles;
    int localizedPrefixLength;
    const char *filter;
    char **sysFiles;

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
        tmp5 = "ff";
    else
        tmp5 = "/";
    filter = tmp5;
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
                tmp4 = strlen(va("%d", 128));
                ui_arenaInfos[ui_numArenas] = (char *)UI_Alloc(strlen(ainfo) + tmp4 + 6, 1);
                idx2 = ainfo;
                idx = ui_arenaInfos[ui_numArenas];
                do
                {
                    ch = *idx2;
                    *idx++ = *idx2++;
                }
                while ( ch );
                ++ui_numArenas;
            }
        }
    }
    if ( ui_numArenas >= 128 )
        Com_Printf(13, "Warning: Max number of arenas loaded: %d\n", 128);
    FS_FreeFileList((const char **)sysFiles);
}

// Decomp: CoDMPServer.c:534315
int UI_LoadArenasFromFile()
{
    if ( useFastFile->current.enabled )
        return ((int (__cdecl *)(void (*)()))UI_LoadArenasFromFile_FastFile)(UI_LoadArenasFromFile_FastFile);
    else
        return ((int (__cdecl *)(const char *(*)()))UI_LoadArenasFromFile_LoadObj)(UI_LoadArenasFromFile_LoadObj);
}

const char *UI_LoadArenasFromFile_LoadObj()
{
    const char *result;
    const char *tmp1;
    char string[132];
    char *tmp3;
    char listbuf[1024];
    char buffer[8196];
    int len;
    int f;
    int idx;
    unsigned int tmp9;

    ui_numArenas = 0;
    result = (const char *)FS_GetFileList("mp", (char*)"arena", FS_LIST_PURE_ONLY, listbuf, 1024);
    tmp1 = result;
    tmp3 = listbuf;
    idx = 0;
    while ( idx < (int)tmp1 )
    {
        tmp9 = strlen(tmp3);
        sprintf(string, "%s/%s", "mp", tmp3);
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
        ++idx;
        result = &tmp3[tmp9 + 1];
        tmp3 = (char *)result;
    }
    return result;
}

// Decomp: CoDMPServer.c:534379
void UI_LoadArenasFromFile_FastFile()
{
    RawFile *rawfile;

    rawfile = DB_FindXAssetHeader(ASSET_TYPE_RAWFILE, (char *)"mp/mpmaps.arena", 1, -1).rawfile;
    if ( rawfile )
        ui_numArenas = UI_ParseInfos(rawfile->buffer, 128 - ui_numArenas, &ui_arenaInfos[ui_numArenas]);
    else
        Com_PrintError(13, "file not found: %s\n", "mp/mpmaps.arena");
}

// Decomp: CoDMPServer.c:534391
void __cdecl UI_LoadMaps()
{
    const char *tmp0;
    const char *tmp1;
    const char *tmp2;
    const char *tmp3;
    const char *tmp4;
    const char *tmp5;
    const char *tmp6;
    int j;
    int i;
    char tableName[20];
    const char *dlcMapPackIndex;
    XAssetHeader table;
    char packIndex[4];
    int mapPackIndex;
    char index[4];
    int mapPackCount;
    int count;

    I_strncpyz(tableName, "mp/mapsTable.csv", 18);
    StringTable_GetAsset(tableName, &table);
    if (!table.xmodelPieces
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ui_mp\\ui_gameinfo_mp.cpp", 434, 0, "%s", "table"))
    {
        __debugbreak();
    }
    memset((unsigned __int8 *)sharedUiInfo.mapList, 0, sizeof(sharedUiInfo.mapList));
    tmp0 = StringTable_Lookup(table.stringTable, 0, "maxnum_map", 1);
    sharedUiInfo.mapCount = atoi(tmp0);
    tmp1 = StringTable_Lookup(table.stringTable, 0, "mappack_count", 1);
    mapPackCount = atoi(tmp1);
    for (i = 0; i < mapPackCount; ++i)
    {
        Com_sprintf(packIndex, 4u, "%i", i);
        tmp2 = StringTable_Lookup(table.stringTable, 1, packIndex, 0);
        I_strncpyz(sharedUiInfo.contentPackList[i].mapPackName, tmp2, 16);
    }
    count = 0;
    for (j = 0; j < sharedUiInfo.mapCount; ++j)
    {
        Com_sprintf(index, 4u, "%i", j);
        dlcMapPackIndex = StringTable_Lookup(table.stringTable, 5, index, 11);
        mapPackIndex = atoi(dlcMapPackIndex);
        tmp3 = StringTable_Lookup(table.stringTable, 5, index, 0);
        I_strncpyz(sharedUiInfo.mapList[count].mapLoadName, tmp3, 24);
        tmp4 = StringTable_Lookup(table.stringTable, 5, index, 3);
        I_strncpyz(sharedUiInfo.mapList[count].mapName, tmp4, 32);
        tmp5 = StringTable_Lookup(table.stringTable, 1, dlcMapPackIndex, 0);
        I_strncpyz(sharedUiInfo.mapList[count].mapPackType, tmp5, 16);
        sharedUiInfo.mapList[count].mapPackTypeIndex = mapPackIndex;
        Com_sprintf(sharedUiInfo.mapList[count].mapNameCaps, 0x20u, "%s%s", sharedUiInfo.mapList[count].mapName, "_CAPS");
        tmp6 = StringTable_Lookup(table.stringTable, 5, index, 10);
        if (I_strcmp(tmp6, "YES"))
            sharedUiInfo.mapList[count].splitscreen = 0;
        else
            sharedUiInfo.mapList[count].splitscreen = 1;
        ++count;
    }
    sharedUiInfo.mapCount = count;
    Dvar_SetIntByName("ui_mapCount", count);
}

// Decomp: CoDMPServer.c:534461
void __cdecl UI_LoadCustomMatchGameTypes()
{
    const char *tmp0;
    const char *tmp1;
    const char *tmp2;
    const char *tmp3;
    const char *tmp4;
    int i;
    char tableName[24];
    const StringTable *table;
    char index[4];

    I_strncpyz(tableName, "mp/gametypesTable.csv", 23);
    StringTable_GetAsset(tableName, (XAssetHeader *)&table);
    if ( !table
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ui_mp\\ui_gameinfo_mp.cpp", 486, 0, "%s", "table") )
    {
        __debugbreak();
    }
    tmp0 = StringTable_Lookup(table, 0, "maxnum_gametype", 1);
    sharedUiInfo.numCustomMatchGameTypes = atoi(tmp0);
    for ( i = 0; i < sharedUiInfo.numCustomMatchGameTypes; ++i )
    {
        Com_sprintf(index, 4u, "%i", i);
        tmp1 = StringTable_Lookup(table, 4, index, 0);
        I_strncpyz(sharedUiInfo.customMatchGameTypes[i].gameType, tmp1, 12);
        tmp2 = StringTable_Lookup(table, 4, index, 1);
        I_strncpyz((char *)sharedUiInfo.customMatchGameTypes[i].gameTypeName, tmp2, 32);
        tmp3 = StringTable_Lookup(table, 4, index, 5);
        if ( I_strcmp(tmp3, "YES") )
            sharedUiInfo.customMatchGameTypes[i].splitscreen = 0;
        else
            sharedUiInfo.customMatchGameTypes[i].splitscreen = 1;
        tmp4 = StringTable_Lookup(table, 4, index, 6);
        if ( I_strcmp(tmp4, "YES") )
            sharedUiInfo.customMatchGameTypes[i].basictraining = 0;
        else
            sharedUiInfo.customMatchGameTypes[i].basictraining = 1;
    }
}

