#include "cl_mapload_cin_sp.h"
#include <client/cl_cin.h>
#include <client_sp/cl_main_sp.h>
#include <ui_sp/ui_main_sp.h>
#include <gfx_d3d/r_cinematic.h>
#include <qcommon/common.h>
#include <qcommon/cmd.h>
#include <database/db_registry.h>
#include <universal/com_files.h>
#include <universal/q_parse.h>
#include <universal/dvar.h>
#include <game/g_main.h>
#include <sound/snd_public_async.h>

extern const dvar_t *com_skipMovies;
extern const dvar_t *com_introPlayed;

// CoDSP_rdBlackOps.map.c:828EA248 / BlackOps.singleplayer.c @951342
struct MovieScriptOpInfo
{
    int opcode;
    int hasName;
    int stackPop;
    int stackPush;
    const char *name;
};

static const MovieScriptOpInfo s_movieToPlayScriptOpInfo[] =
{
    { 0, 1, 2, 1, "+" },
    { 1, 1, 2, 1, "-" },
    { 2, 1, 2, 1, "*" },
    { 3, 1, 2, 1, ">" },
    { 4, 1, 2, 1, "<" },
    { 5, 1, 2, 1, "==" },
    { 6, 1, 2, 1, "strcmp" },
    { 7, 1, 2, 1, "strcat" },
    { 8, 1, 1, 1, "!" },
    { 9, 1, 0, 1, "dup" },
    { 10, 1, 0, 0, 0 },
    { 11, 1, 2, 2, "swap" },
    { 12, 1, 1, 1, "getdvar" },
    { 13, 1, 0, 1, "getmapname" },
    { 14, 1, 1, 0, "if" },
    { 15, 1, 0, 0, "then" },
    { 16, 1, 1, 0, "play" },
    { 17, 0, 0, 1, 0 },
    { 0, 0, 0, 0, 0 },
};

#define CINEMATIC_PLAYBACK_MAPLOAD 4u

static const MovieScriptOpInfo *CL_MapLoading_FindScriptOp(const char *token)
{
    int i;

    for ( i = 0; s_movieToPlayScriptOpInfo[i].name; ++i )
    {
        if ( !strcmp(token, s_movieToPlayScriptOpInfo[i].name) )
            return &s_movieToPlayScriptOpInfo[i];
    }
    return &s_movieToPlayScriptOpInfo[ARRAY_COUNT(s_movieToPlayScriptOpInfo) - 2];
}

// BlackOps.singleplayer.c @8825B0 (cin_levels VM) / CoDSP @828EA248 when video/cin_levels.txt is missing.
// Retail resolves movies via cin_levels.txt (getmapname + getdvar com_introPlayed + play rules).
// Kisak fallback mirrors the frontend branch only:
//   treyarch.bik              — first frontend load only (com_introPlayed == 0)
//   so_narrative1_frontend_load.bik — reload / return to frontend.ff (com_introPlayed != 0)
// frontend.bik is NOT the map-load bink; it is used elsewhere (e.g. ui_playCoastOutroMovie / script playbink).
static void CL_MapLoading_FallbackMovie(const char *mapName, char *outMovieName)
{
    if ( !I_stricmp(mapName, "frontend") || Com_IsMenuLevel(mapName) )
    {
        if ( com_introPlayed && com_introPlayed->current.enabled )
            I_strncpyz(outMovieName, "so_narrative1_frontend_load", 256);
        else
            I_strncpyz(outMovieName, "treyarch", 256);
    }
    else
    {
        Com_sprintf(outMovieName, 256, "%s_load", mapName ? mapName : "default");
    }
}

static bool CL_MapLoading_CalcMovieToPlayInternal(const char *scriptText, const char *mapName, char *outMovieName)
{
    char stack[32][64];
    char concatBuf[64];
    const char *parseText;
    parseInfo_t *parseInfo;
    const char *token;
    int stackDepth;
    int skipDepth;
    int skipFlag;
    const MovieScriptOpInfo *op;

    outMovieName[0] = 0;
    stackDepth = 0;
    skipDepth = 0;
    skipFlag = 0;
    parseText = scriptText;
    Com_BeginParseSession("video/cin_levels.txt");

    do
    {
        parseInfo = Com_Parse(&parseText);
        token = parseInfo->token;
        if ( !token[0] )
            break;

        if ( skipDepth > 0 )
        {
            if ( !strcmp(token, "then") )
            {
                --skipDepth;
                skipFlag = skipDepth;
            }
            else if ( !strcmp(token, "if") )
            {
                ++skipDepth;
                skipFlag = skipDepth;
            }
            continue;
        }

        op = CL_MapLoading_FindScriptOp(token);

        if ( stackDepth < op->stackPop )
        {
            Com_PrintWarning(1, "cin_levels.txt: stack underflow on '%s'\n", token);
            Com_EndParseSession();
            return false;
        }
        if ( op->stackPush + stackDepth - op->stackPop >= 32 )
        {
            Com_PrintWarning(1, "cin_levels.txt: stack overflow on '%s' (depth %d)\n", token, stackDepth);
            Com_EndParseSession();
            return false;
        }

        switch ( op->opcode )
        {
        case 0:
            Com_sprintf(stack[stackDepth - 2], 64, "%i",
                atoi(stack[stackDepth - 2]) + atoi(stack[stackDepth - 1]));
            break;
        case 1:
            Com_sprintf(stack[stackDepth - 2], 64, "%i",
                atoi(stack[stackDepth - 2]) - atoi(stack[stackDepth - 1]));
            break;
        case 2:
            Com_sprintf(stack[stackDepth - 2], 64, "%i",
                atoi(stack[stackDepth - 2]) * atoi(stack[stackDepth - 1]));
            break;
        case 3:
            Com_sprintf(stack[stackDepth - 2], 64, "%i",
                atoi(stack[stackDepth - 2]) > atoi(stack[stackDepth - 1]) ? 1 : 0);
            break;
        case 4:
            Com_sprintf(stack[stackDepth - 2], 64, "%i",
                atoi(stack[stackDepth - 2]) < atoi(stack[stackDepth - 1]) ? 1 : 0);
            break;
        case 5:
            Com_sprintf(stack[stackDepth - 2], 64, "%i",
                atoi(stack[stackDepth - 2]) == atoi(stack[stackDepth - 1]) ? 1 : 0);
            break;
        case 6:
            Com_sprintf(stack[stackDepth - 2], 64, "%i",
                strcmp(stack[stackDepth - 2], stack[stackDepth - 1]));
            break;
        case 7:
            Com_sprintf(concatBuf, 64, "%s%s", stack[stackDepth - 2], stack[stackDepth - 1]);
            I_strncpyz(stack[stackDepth - 2], concatBuf, 64);
            break;
        case 8:
            Com_sprintf(stack[stackDepth - 1], 64, "%i", atoi(stack[stackDepth - 1]) == 0 ? 1 : 0);
            break;
        case 9:
            I_strncpyz(stack[stackDepth], stack[stackDepth - 1], 64);
            break;
        case 10:
        case 15:
            break;
        case 11:
            I_strncpyz(concatBuf, stack[stackDepth - 2], 64);
            I_strncpyz(stack[stackDepth - 2], stack[stackDepth - 1], 64);
            I_strncpyz(stack[stackDepth - 1], concatBuf, 64);
            break;
        case 12:
            I_strncpyz(stack[stackDepth - 1], Dvar_GetVariantString(stack[stackDepth - 1]), 64);
            break;
        case 13:
            I_strncpyz(stack[stackDepth], mapName ? mapName : "", 64);
            break;
        case 14:
            if ( !atoi(stack[stackDepth - 1]) )
                skipFlag = 1;
            break;
        case 16:
            if ( stack[stackDepth - 1][0] )
                I_strncpyz(outMovieName, stack[stackDepth - 1], 256);
            break;
        case 17:
            I_strncpyz(stack[stackDepth], token, 64);
            break;
        default:
            Com_PrintWarning(1, "cin_levels.txt: unknown opcode %d\n", op->opcode);
            Com_EndParseSession();
            return false;
        }

        stackDepth += op->stackPush - op->stackPop;
        skipDepth = skipFlag;
    }
    while ( !outMovieName[0] );

    Com_EndParseSession();

    if ( skipDepth > 0 )
    {
        Com_PrintWarning(1, "cin_levels.txt: unterminated if\n");
        return false;
    }

    return outMovieName[0] != 0;
}

void __cdecl CL_MapLoading_CalcMovieToPlay(const char *scriptText, const char *mapName, char *outMovieName)
{
    if ( !CL_MapLoading_CalcMovieToPlayInternal(scriptText, mapName, outMovieName) )
    {
        Com_PrintWarning(1, "cin_levels.txt VM failed for map '%s', using fallback movie\n", mapName);
        CL_MapLoading_FallbackMovie(mapName, outMovieName);
    }
}

void __cdecl CL_MapLoading_CalcMovieToPlay_FastFile(const char *mapName, char *outMovieName)
{
    RawFile *rawfile;

    outMovieName[0] = 0;
    rawfile = DB_FindXAssetHeader(ASSET_TYPE_RAWFILE, (char *)"video/cin_levels.txt", 1, -1).rawfile;
    if ( !rawfile || !rawfile->buffer || !rawfile->buffer[0] )
    {
        Com_PrintWarning(1, "Could not open video/cin_levels.txt, using fallback movie\n");
        CL_MapLoading_FallbackMovie(mapName, outMovieName);
        return;
    }
    CL_MapLoading_CalcMovieToPlay(rawfile->buffer, mapName, outMovieName);
}

void __cdecl CL_MapLoading_StartCinematic(const char *mapName)
{
    char movieName[256];
    char nextMovieName[256];

    CL_MapLoading_CalcMovieToPlay_FastFile(mapName, movieName);
    if ( !movieName[0] )
    {
        Com_Printf(16, "SP map load: no cin_levels rule for '%s', using default.bik\n", mapName);
        I_strncpyz(movieName, "default", sizeof(movieName));
    }

    if ( !I_stricmp(movieName, "spinner") )
    {
        Dvar_SetBool((dvar_s *)long_blocking_call, 1);
        return;
    }

    cin_skippable = false;

    // BlackOps.singleplayer.c @882B00 / CoDSP @828EA138 — first frontend boot: treyarch.bik then
    // cin_levels.txt post-intro movie (so_narrative1_frontend_load.bik). Temporarily set com_introPlayed
    // so the VM takes the reload branch while computing the queued follow-up.
    if ( ( !I_stricmp(mapName, "frontend") || Com_IsMenuLevel(mapName) )
        && com_introPlayed
        && !com_introPlayed->current.enabled )
    {
        Dvar_SetBool((dvar_s *)com_introPlayed, 1);
        CL_MapLoading_CalcMovieToPlay_FastFile(mapName, nextMovieName);
        Dvar_SetBool((dvar_s *)com_introPlayed, 0);
        if ( nextMovieName[0]
            && I_stricmp(nextMovieName, "spinner")
            && I_stricmp(nextMovieName, movieName) )
        {
            R_Cinematic_QueueNextPlayback(nextMovieName, CINEMATIC_PLAYBACK_MAPLOAD);
        }
    }

    Com_Printf(16, "SP map load cinematic: %s.bik (map '%s')\n", movieName, mapName);
    R_Cinematic_StartPlayback_Internal(movieName, CINEMATIC_PLAYBACK_MAPLOAD, 0);
}

static void CL_PlayFrontendMovie()
{
    CL_MapLoading_StartCinematic("frontend");
}

void __cdecl CL_MapLoadingKickLoadMovie(const char *mapName, char savegame)
{
    (void)savegame;

    if ( G_ExitAfterToolComplete() )
        return;
    if ( com_skipMovies && com_skipMovies->current.enabled )
        return;

    if ( Com_IsMenuLevel(mapName) )
        CL_PlayFrontendMovie();
    else
        CL_MapLoading_StartCinematic(mapName);

    SND_FadeOut();
}
