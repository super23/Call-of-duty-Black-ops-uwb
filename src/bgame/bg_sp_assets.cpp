#include "bg_sp_assets.h"

#ifdef KISAK_SP

#include <universal/com_stringtable.h>
#include <database/db_registry.h>
#include <qcommon/common.h>
#include <universal/com_loadutils.h>
#include <universal/com_memory.h>
#include <universal/q_parse.h>
#include <universal/q_shared.h>
#include <universal/com_files.h>
#include <database/database.h>

extern unsigned int g_playerAnimTypeNamesCount;
extern int g_playerAnimTypeIndex_revivee;
extern char *g_playerAnimTypeNames[32];

bool __cdecl BG_SP_TryGetStringTableAsset(
    const char *retailMpPath,
    const char *fallbackPath,
    const StringTable **outTable)
{
    const StringTable *table;
    const char *paths[3];
    int pathIndex;

    if ( !outTable )
        return false;
    *outTable = 0;
    paths[0] = retailMpPath;
    paths[1] = fallbackPath;
    paths[2] = 0;

    for ( pathIndex = 0; paths[pathIndex]; ++pathIndex )
    {
        if ( !paths[pathIndex][0] )
            continue;
        // SP: probe without ERR_DROP (db_registry KISAK_SP) — asset may be in common_zombie.ff under mp/ path.
        table = DB_FindXAssetHeader(ASSET_TYPE_STRINGTABLE, (char *)paths[pathIndex], 0, -1).stringTable;
        if ( table && table->columnCount && table->rowCount )
        {
            *outTable = table;
            return true;
        }
    }
    return false;
}

char *__cdecl BG_SP_LoadRawTextFileFirst(const char *retailMpPath, const char *fallbackPath)
{
    char *buf;
    RawFile *rawfile;

    if ( retailMpPath && retailMpPath[0] )
    {
        if ( useFastFile->current.enabled )
        {
            rawfile = DB_FindXAssetHeader(ASSET_TYPE_RAWFILE, (char *)retailMpPath, 0, -1).rawfile;
            if ( rawfile && rawfile->buffer && rawfile->len > 0 )
                return (char *)rawfile->buffer;
        }
        else
        {
            buf = Com_LoadRawTextFile_LoadObj(retailMpPath);
            if ( buf )
                return buf;
        }
    }
    if ( fallbackPath && fallbackPath[0] && (!retailMpPath || I_stricmp(fallbackPath, retailMpPath)) )
    {
        if ( useFastFile->current.enabled )
        {
            rawfile = DB_FindXAssetHeader(ASSET_TYPE_RAWFILE, (char *)fallbackPath, 0, -1).rawfile;
            if ( rawfile && rawfile->buffer && rawfile->len > 0 )
                return (char *)rawfile->buffer;
        }
        else
        {
            buf = Com_LoadRawTextFile_LoadObj(fallbackPath);
            if ( buf )
                return buf;
        }
    }
    return 0;
}

static bool BG_SP_TryLoadInfoStringPath(
    const char *fileName,
    const char *ident,
    char *loadBuffer,
    char **outParsed)
{
    unsigned int identLen;
    int fileHandle;
    int fileLen;
    RawFile *rawfile;
    const char *buffer;

    if ( !fileName || !fileName[0] || !ident || !outParsed )
        return false;
    identLen = strlen(ident);
    if ( useFastFile->current.enabled )
    {
        rawfile = DB_FindXAssetHeader(ASSET_TYPE_RAWFILE, (char *)fileName, 0, -1).rawfile;
        if ( !rawfile || !rawfile->buffer || rawfile->len <= identLen )
            return false;
        buffer = rawfile->buffer;
        if ( strncmp(buffer, ident, identLen) )
            return false;
        *outParsed = (char *)&buffer[identLen];
        return Info_Validate(*outParsed);
    }
    fileLen = FS_FOpenFileByMode((char *)fileName, &fileHandle, FS_READ);
    if ( fileLen < 0 )
        return false;
    FS_Read((unsigned __int8 *)loadBuffer, identLen, fileHandle);
    loadBuffer[identLen] = 0;
    if ( strncmp(loadBuffer, ident, identLen) )
    {
        FS_FCloseFile(fileHandle);
        return false;
    }
    if ( fileLen - (int)identLen >= 0x4000 )
    {
        FS_FCloseFile(fileHandle);
        return false;
    }
    FS_Read((unsigned __int8 *)loadBuffer, fileLen - identLen, fileHandle);
    loadBuffer[fileLen - identLen] = 0;
    FS_FCloseFile(fileHandle);
    *outParsed = loadBuffer;
    return Info_Validate(loadBuffer);
}

bool __cdecl BG_SP_TryLoadInfoStringFirst(
    const char *spPath,
    const char *mpPath,
    const char *ident,
    char *loadBuffer,
    char **outParsed)
{
    if ( !outParsed )
        return false;
    *outParsed = 0;
    if ( spPath && spPath[0] && BG_SP_TryLoadInfoStringPath(spPath, ident, loadBuffer, outParsed) )
        return true;
    if ( mpPath && mpPath[0] && (!spPath || I_stricmp(spPath, mpPath))
        && BG_SP_TryLoadInfoStringPath(mpPath, ident, loadBuffer, outParsed) )
        return true;
    return false;
}

void __cdecl BG_SP_LoadPlayerAnimTypesWithFallback()
{
    static const char *kDefaultAnimTypes[] = {
        "player",
        "other",
        "riotshield",
        "revivee",
        "turret",
        "tank",
        0
    };
    char *buf;
    const char *text_p;
    const char *token;
    int defaultIndex;

    g_playerAnimTypeNamesCount = 0;
    g_playerAnimTypeIndex_revivee = -1;

    buf = BG_SP_LoadRawTextFileFirst("mp/playeranimtypes.txt", "playeranimtypes.txt");
    if ( !buf )
    {
        Com_PrintWarning(
            16,
            "SP: missing mp/playeranimtypes.txt (CoDSP_rdBlackOps.map.c) — using builtin anim type list\n");
        for ( defaultIndex = 0; kDefaultAnimTypes[defaultIndex]; ++defaultIndex )
        {
            if ( g_playerAnimTypeNamesCount >= 0x20 )
                break;
            g_playerAnimTypeNames[g_playerAnimTypeNamesCount] = (char *)Hunk_Alloc(
                strlen(kDefaultAnimTypes[defaultIndex]) + 1,
                "BG_SP_LoadPlayerAnimTypesWithFallback",
                10);
            I_strncpyz(
                g_playerAnimTypeNames[g_playerAnimTypeNamesCount],
                kDefaultAnimTypes[defaultIndex],
                strlen(kDefaultAnimTypes[defaultIndex]) + 1);
            if ( !I_stricmp(kDefaultAnimTypes[defaultIndex], "revivee") )
                g_playerAnimTypeIndex_revivee = g_playerAnimTypeNamesCount;
            ++g_playerAnimTypeNamesCount;
        }
        return;
    }

    text_p = buf;
    Com_BeginParseSession("BG_AnimParseAnimScript");
    while ( 1 )
    {
        token = (const char *)Com_Parse(&text_p);
        if ( !token || !*token )
            break;
        if ( g_playerAnimTypeNamesCount >= 0x20 )
            Com_Error(ERR_DROP, "Player anim type array size exceeded");
        g_playerAnimTypeNames[g_playerAnimTypeNamesCount] = (char *)Hunk_Alloc(
            strlen(token) + 1,
            "BG_LoadPlayerAnimTypes",
            10);
        I_strncpyz(g_playerAnimTypeNames[g_playerAnimTypeNamesCount], token, strlen(token) + 1);
        if ( !I_stricmp(token, "revivee") )
            g_playerAnimTypeIndex_revivee = g_playerAnimTypeNamesCount;
        ++g_playerAnimTypeNamesCount;
    }
    Com_EndParseSession();
}

#endif
