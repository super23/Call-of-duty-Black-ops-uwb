#include "bg_collectibles_sp.h"

#include <qcommon/cmd.h>
#include <string.h>
#include <universal/assertive.h>
#include <universal/dvar.h>

// Decomp: CoDSP_rdBlackOps.map.c — bg_collectibles_coop.cpp

const dvar_s *bg_collectibles;

static char s_defaultCollectiblesStr[kCollectibleStringLen + 2];
static bool s_defaultCollectiblesInitialized;

// Decomp: CoDSP_rdBlackOps.map.c @831174D0 — g_CollectiblesGroupMap (15 entries)
static const CollectibleGroupMap g_CollectiblesGroupMap[kCollectibleGroupMapCount] =
{
    { "pentagon", 1, 3 },
    { "soa", 4, 6 },
    { "vorkuta", 7, 9 },
    { "exec", 10, 12 },
    { "khe_sanh", 13, 15 },
    { "defiant", 16, 18 },
    { "fullacide", 19, 21 },
    { "da_nang", 22, 24 },
    { "sog", 25, 27 },
    { "hue", 28, 30 },
    { "rebirth", 31, 32 },
    { "river", 33, 34 },
    { "wmd", 35, 36 },
    { "underlord", 37, 38 },
    { "revenge", 39, 41 },
};

// Decomp: CoDSP_rdBlackOps.map.c:822C9088 (BG_GetCollectibleGroupValue)
int __cdecl BG_GetCollectibleGroupValue(unsigned __int8 *collectibleData)
{
    int collectedCount;
    int groupIndex;
    int collectibleIndex;

    if ( !collectibleData )
        return 0;

    collectedCount = 0;
    for ( groupIndex = 0; groupIndex < kCollectibleGroupMapCount; ++groupIndex )
    {
        if ( !I_stricmp((const char *)g_CollectiblesGroupMap[groupIndex].groupName, (const char *)collectibleData) )
        {
            for ( collectibleIndex = g_CollectiblesGroupMap[groupIndex].startIndex; ; ++collectibleIndex )
            {
                if ( collectibleIndex > g_CollectiblesGroupMap[groupIndex].endIndex )
                    break;
                if ( BG_HasCollectible(collectibleIndex, 0, 0, collectibleIndex, 0, 0, 0) )
                {
                    ++collectedCount;
                }
            }
        }
    }
    return collectedCount;
}

// Decomp: CoDSP_rdBlackOps.map.c:822C9178 (BG_HasCollectible)
bool __cdecl BG_HasCollectible(int clientNum, int mapIndex, int groupIndex, int itemIndex, int a5, int a6, __int64 a7)
{
    const char *collectiblesStr;

    (void)mapIndex;
    (void)groupIndex;
    (void)itemIndex;
    (void)a5;
    (void)a6;
    (void)a7;

    iassert(clientNum <= kMaxCollectibleIndex && clientNum > 0);

    if ( !bg_collectibles )
        return false;

    collectiblesStr = bg_collectibles->current.string;
    if ( !collectiblesStr )
        return false;

    return collectiblesStr[clientNum - 1] != '0';
}

// Decomp: CoDSP_rdBlackOps.map.c:822C9218 (BG_SetCollectible)
int __cdecl BG_SetCollectible(int clientNum, int mapIndex, int groupIndex, int itemIndex, __int64 a5)
{
    char collectiblesStr[kCollectibleStringLen + 2];

    (void)mapIndex;
    (void)groupIndex;
    (void)itemIndex;
    (void)a5;

    iassert(clientNum <= kMaxCollectibleIndex && clientNum > 0);
    iassert(bg_collectibles);

    I_strncpyz(collectiblesStr, bg_collectibles->current.string, sizeof(collectiblesStr));
    collectiblesStr[clientNum - 1] = '1';
    Dvar_SetString((dvar_s *)bg_collectibles, collectiblesStr);
    Cbuf_InsertText(0, "updategamerprofile\n");
    return 0;
}

// Decomp: CoDSP_rdBlackOps.map.c:822C92E8 (BG_UnsetCollectible)
int __cdecl BG_UnsetCollectible(int clientNum, int mapIndex, int groupIndex, int itemIndex, __int64 a5)
{
    char collectiblesStr[kCollectibleStringLen + 2];

    (void)mapIndex;
    (void)groupIndex;
    (void)itemIndex;
    (void)a5;

    iassert(clientNum <= kMaxCollectibleIndex && clientNum > 0);
    iassert(bg_collectibles);

    I_strncpyz(collectiblesStr, bg_collectibles->current.string, sizeof(collectiblesStr));
    collectiblesStr[clientNum - 1] = '0';
    Dvar_SetString((dvar_s *)bg_collectibles, collectiblesStr);
    return 0;
}

// Decomp: CoDSP_rdBlackOps.map.c:822C93A8 (BG_GiveAllCollectible)
int __cdecl BG_GiveAllCollectible(int clientNum, int mapIndex, int groupIndex, int itemIndex, __int64 a5)
{
    int result;
    int collectibleIndex;

    (void)clientNum;

    result = 0;
    for ( collectibleIndex = 1; collectibleIndex < 42; ++collectibleIndex )
    {
        result = BG_SetCollectible(collectibleIndex, mapIndex, groupIndex, itemIndex, a5);
    }
    return result;
}

// Decomp: CoDSP_rdBlackOps.map.c:822C93F8 (BG_ClearCollectibles)
void BG_ClearCollectibles()
{
    char *defaultCollectiblesStr;

    defaultCollectiblesStr = BG_GetDefaulCollectiblesStr();
    Dvar_SetString((dvar_s *)bg_collectibles, defaultCollectiblesStr);
}

// Decomp: CoDSP_rdBlackOps.map.c:822C9428 (BG_GetDefaulCollectiblesStr)
char *BG_GetDefaulCollectiblesStr()
{
    if ( !s_defaultCollectiblesInitialized )
    {
        memset(s_defaultCollectiblesStr, '0', kCollectibleStringLen);
        s_defaultCollectiblesStr[kCollectibleStringLen] = 0;
        s_defaultCollectiblesInitialized = true;
    }
    return s_defaultCollectiblesStr;
}

// Decomp: CoDSP_rdBlackOps.map.c:822C9480 (BG_Collectibles_RegisterDvars)
const dvar_s *BG_Collectibles_RegisterDvars()
{
    char *defaultCollectiblesStr;
    const dvar_s *result;

    defaultCollectiblesStr = BG_GetDefaulCollectiblesStr();
    result = _Dvar_RegisterString(
        "bg_collectibles",
        defaultCollectiblesStr,
        1u,
        "Collectibles save string");
    bg_collectibles = result;
    return result;
}
