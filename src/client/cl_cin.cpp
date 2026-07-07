#include "cl_cin.h"
#include <gfx_d3d/r_cinematic.h>
#include <gfx_d3d/r_init.h>
#include <gfx_d3d/r_rendercmds.h>
#include <gfx_d3d/r_font.h>
#include <universal/dvar.h>
#include <universal/q_shared.h>
#include <qcommon/common.h>
#ifdef KISAK_SP
#include <server_sp/sv_init_sp.h>
#include <client_sp/cl_cgame_sp.h>
#include <client_sp/cl_main_sp.h>
#include <ui_sp/ui_main_sp.h>
#include <ui/ui_shared.h>
#else
#include <server_mp/sv_init_mp.h>
#include <client_mp/cl_cgame_mp.h>
#endif
#include "client.h"
#include <qcommon/cmd.h>
#include <universal/com_files.h>
#include <win32/win_main.h>
#include <sound/snd_public_async.h>

bool cin_skippable = true;

#ifdef KISAK_SP
#include <ui_sp/ui_main_sp.h>
#include <database/db_file_load.h>

static void CL_UpdateCinematicSkippable()
{
    connstate_t state;

    state = CL_GetLocalClientConnectionState(0);
    if ( (state == CA_LOADING || state == CA_PRIMED) && R_Cinematic_IsStarted() )
        cin_skippable = DB_GetLoadedFraction() >= 1.0;
    else
        cin_skippable = true;
}

// CoDSP_rdBlackOps.map.c:6371718 — startup intros run via "map frontend" + CL_MapLoadingKickLoadMovie, not a separate chain.
void __cdecl CL_BeginSPStartupIntroChain()
{
}

bool __cdecl CL_IsSPStartupIntroActive()
{
    return R_Cinematic_IsPending();
}

void __cdecl CL_FinishSPStartupIntro(int localClientNum)
{
    (void)localClientNum;
    if ( com_introPlayed )
        Dvar_SetBool((dvar_s *)com_introPlayed, 1);
    if ( com_startupIntroPlayed )
        Dvar_SetBool((dvar_s *)com_startupIntroPlayed, 1);
}

// Retail briefing/connect overlay during map-load bink (CA_LOADING + CG_DrawInformation).
// CoDSP byte_8311D444 (cin_skippable) gates SCR_StopCinematic; draw matches PC "LEFT MOUSE skip".
static void SCR_DrawCinematicSkipHint()
{
    static const float actionColor[4] = { 0.55f, 0.58f, 0.22f, 1.0f };
    static const float skipColor[4] = { 0.45f, 0.45f, 0.45f, 1.0f };
    char actionText[16];
    char skipText[8];
    Font_s *font;
    float scale;
    float actionWidth;
    float skipWidth;
    float totalWidth;
    float x;
    float y;
    int screenWidth;
    int screenHeight;
    float aspect;

    if ( !cin_skippable || !R_Cinematic_IsStarted() )
        return;

    I_strncpyz(actionText, "LEFT MOUSE", sizeof(actionText));
    I_strncpyz(skipText, " skip", sizeof(skipText));
    font = UI_GetFontHandle(&scrPlaceFull, 0, 0.5f);
    scale = R_NormalizedTextScale(font, 0.35f);
    actionWidth = (float)UI_TextWidth(actionText, 0, font, scale);
    skipWidth = (float)UI_TextWidth(skipText, 0, font, scale);
    totalWidth = actionWidth + skipWidth;
    CL_GetScreenDimensions(&screenWidth, &screenHeight, &aspect);
    x = ((float)screenWidth - totalWidth) * 0.5f;
    y = (float)screenHeight * 0.88f;
    R_AddCmdDrawText(actionText, 0x7FFFFFFF, font, x, y, scale, scale, 0.0f, actionColor, 0);
    R_AddCmdDrawText(skipText, 0x7FFFFFFF, font, x + actionWidth, y, scale, scale, 0.0f, skipColor, 0);
}
#endif

void __cdecl SCR_DrawCinematic()
{
    float color[4];
    float w;
    float h;

    if ( R_Cinematic_IsNextReady() )
        R_Cinematic_StartNextPlayback();
    if ( R_Cinematic_IsStarted() )
    {
        R_Cinematic_UpdateFrame(0);
        if ( rgp.cinematicMaterial )
        {
            color[0] = 1.0f;
            color[1] = 1.0f;
            color[2] = 1.0f;
            color[3] = 1.0f;
            w = (float)cls.vidConfig.displayWidth;
            h = (float)cls.vidConfig.displayHeight;
            R_AddCmdDrawStretchPic(0.0f, 0.0f, w, h, 0.0f, 0.0f, 1.0f, 1.0f, color, rgp.cinematicMaterial);
#ifdef KISAK_SP
            SCR_DrawCinematicSkipHint();
#endif
        }
    }
    if ( R_Cinematic_IsFinished() )
    {
        if ( Dvar_GetBool("com_movieIsPlaying") )
            Dvar_SetBool((dvar_s *)com_movieIsPlaying, 0);
#ifdef KISAK_SP
        if ( !R_Cinematic_IsNextReady() )
        {
            Dvar_SetBool((dvar_s *)long_blocking_call, 0);
            SCR_StopCinematic();
        }
#else
        SCR_StopCinematic();
#endif
    }
    if ( R_Cinematic_IsStarted() && !Dvar_GetBool("com_movieIsPlaying") )
        Dvar_SetBool((dvar_s *)com_movieIsPlaying, 1);
}

void __cdecl SCR_StopCinematic()
{
    const char *v0; // eax
    int clientNum; // [esp+0h] [ebp-8h]
    bool oneClientWasChanged; // [esp+7h] [ebp-1h]

#ifdef KISAK_SP
    CL_UpdateCinematicSkippable();
#endif
    if ( cin_skippable || R_Cinematic_IsFinished() )
    {
        R_Cinematic_StopPlayback();
#ifdef KISAK_SP
        if ( CL_GetLocalClientConnectionState(0) == CA_CINEMATIC )
            CL_FinishSPStartupIntro(0);
#endif
        oneClientWasChanged = 0;
        for ( clientNum = 0; clientNum < 1; ++clientNum )
        {
            if ( CL_GetLocalClientConnectionState(clientNum) == 1 || CL_GetLocalClientConnectionState(clientNum) == 2 )
            {
                oneClientWasChanged = 1;
                CL_SetLocalClientConnectionState(clientNum, CA_DISCONNECTED);
            }
        }
        if ( oneClientWasChanged && *(_BYTE *)nextmap->current.integer )
        {
            v0 = va("%s\n", nextmap->current.string);
            Cbuf_AddText(0, v0);
            Dvar_SetString((dvar_s *)nextmap, "");
        }
    }
}
