#include "ui_gameinfo_sp.h"
#include <ui/ui_shared.h>
#include <qcommon/common.h>
#include <universal/q_shared.h>
#include <universal/com_stringtable.h>
#include <clientscript/cscr_stringlist.h>
#include <stringed/stringed_hooks.h>

static const char *ui_browserMapNameTable[129];
const dvar_t *ui_browserMap;

// Decomp: CoDSP — SP campaign map table lives under maps/mapsTable.csv (not mp/mapsTable.csv).
void __cdecl UI_LoadMaps()
{
    const char *maxMapsStr;
    const char *packCountStr;
    const char *loadName;
    const char *displayName;
    const char *packType;
    const char *splitscreenStr;
    const char *dlcMapPackIndex;
    char tableName[24];
    char packIndex[4];
    char index[4];
    XAssetHeader table;
    int mapPackCount;
    int count;
    int i;
    int j;
    int mapPackIndex;

    I_strncpyz(tableName, "maps/mapsTable.csv", sizeof(tableName));
    StringTable_GetAsset(tableName, &table);
    if ( !table.stringTable )
    {
        Com_Printf(0, "UI_LoadMaps: missing %s\n", tableName);
        sharedUiInfo.mapCount = 0;
        Dvar_SetIntByName("ui_mapCount", 0);
        return;
    }

    memset((unsigned __int8 *)sharedUiInfo.mapList, 0, sizeof(sharedUiInfo.mapList));
    maxMapsStr = StringTable_Lookup(table.stringTable, 0, "maxnum_map", 1);
    sharedUiInfo.mapCount = maxMapsStr ? atoi(maxMapsStr) : 0;
    packCountStr = StringTable_Lookup(table.stringTable, 0, "mappack_count", 1);
    mapPackCount = packCountStr ? atoi(packCountStr) : 0;
    for ( i = 0; i < mapPackCount; ++i )
    {
        Com_sprintf(packIndex, sizeof(packIndex), "%i", i);
        loadName = StringTable_Lookup(table.stringTable, 1, packIndex, 0);
        if ( loadName )
            I_strncpyz(sharedUiInfo.contentPackList[i].mapPackName, loadName, 16);
    }

    count = 0;
    for ( j = 0; j < sharedUiInfo.mapCount; ++j )
    {
        Com_sprintf(index, sizeof(index), "%i", j);
        dlcMapPackIndex = StringTable_Lookup(table.stringTable, 5, index, 11);
        mapPackIndex = dlcMapPackIndex ? atoi(dlcMapPackIndex) : 0;
        loadName = StringTable_Lookup(table.stringTable, 5, index, 0);
        if ( !loadName )
            continue;
        I_strncpyz(sharedUiInfo.mapList[count].mapLoadName, loadName, 24);
        displayName = StringTable_Lookup(table.stringTable, 5, index, 3);
        if ( displayName )
            I_strncpyz(sharedUiInfo.mapList[count].mapName, displayName, 32);
        packType = StringTable_Lookup(table.stringTable, 1, dlcMapPackIndex, 0);
        if ( packType )
            I_strncpyz(sharedUiInfo.mapList[count].mapPackType, packType, 16);
        sharedUiInfo.mapList[count].mapPackTypeIndex = mapPackIndex;
        Com_sprintf(sharedUiInfo.mapList[count].mapNameCaps, 0x20u, "%s%s", sharedUiInfo.mapList[count].mapName, "_CAPS");
        splitscreenStr = StringTable_Lookup(table.stringTable, 5, index, 10);
        sharedUiInfo.mapList[count].splitscreen = splitscreenStr && !I_strcmp(splitscreenStr, "YES");
        sharedUiInfo.mapList[count].active = 1;
        ++count;
    }
    sharedUiInfo.mapCount = count;
    Dvar_SetIntByName("ui_mapCount", count);

    ui_browserMapNameTable[0] = UI_SafeTranslateString("MENU_ALL");
    for ( j = 0; j < sharedUiInfo.mapCount; ++j )
        ui_browserMapNameTable[j + 1] = UI_SafeTranslateString(sharedUiInfo.mapList[j].mapName);
    ui_browserMapNameTable[sharedUiInfo.mapCount + 1] = 0;
    ui_browserMap = _Dvar_RegisterEnum(
        "ui_browserMap",
        ui_browserMapNameTable,
        0,
        1u,
        "Map preferred in the server browser filter.");
}

// SP: no MP arena browser table.
void __cdecl UI_LoadArenas()
{
    // Campaign does not consume MP .arena metadata.
    // Keep map count dvar coherent with whatever UI_LoadMaps() last produced.
    Dvar_SetIntByName("ui_mapCount", sharedUiInfo.mapCount);
}

void __cdecl UI_LoadCustomMatchGameTypes()
{
    sharedUiInfo.numCustomMatchGameTypes = 0;
}
