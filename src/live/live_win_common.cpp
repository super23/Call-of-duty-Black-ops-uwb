#include "live_win_common.h"
#include <qcommon/com_clients.h>
#include "live_win.h"
#include <DW/dwNet.h>
#include "live_groups_dw.h"
#include <DW/MatchRecorder.h>
#include "live_leaderboard.h"
#include "live_sessions_win.h"
#include "live_news.h"
#include "live_counter.h"
#include "live_pcache.h"
#include "live_storage.h"
#include "live_storage_win.h"
#ifdef KISAK_SP
#include <client_sp/cl_main_pc_sp.h>
#else
#include <client_mp/cl_main_pc_mp.h>
#endif
#include <win32/win_shared.h>
#include "live.h"
#include <qcommon/common.h>
#include <universal/assertive.h>
#include <qcommon/threads.h>

#include <DW/dwLogOn_pc.h>
#include <DW/MatchMakingInfo_win32.h>
#include <qcommon/net_chan_mp.h>

// KISAK: forward declaration for our spoofed local sign-in (defined below).
static void LiveStorage_FakeSignInLocal(int controllerIndex);

const char *s_comErrorString;
bool s_shouldComError;

bool dw_disconnect_detected;

int sessiontimer;

extern const dvar_t *live_service; // KISAKTODO: remove these later
extern const dvar_t *dw_dupe_key;
char __cdecl Live_Frame_MP(int localControllerIndex)
{
#ifdef KISAK_LIVE
    bool v2; // [esp+7h] [ebp-21h]
    char *disconnectMessage; // [esp+Ch] [ebp-1Ch] BYREF
    int LogOnStatus; // [esp+10h] [ebp-18h]
    DWOnlineStatus newOnlineStatus; // [esp+14h] [ebp-14h]
    bool justSignedIn; // [esp+1Bh] [ebp-Dh]
    DWOnlineStatus onlineStatus; // [esp+1Ch] [ebp-Ch]
    bool online; // [esp+23h] [ebp-5h]
    int elapsed; // [esp+24h] [ebp-4h]

    elapsed = 0;
    if ( !Sys_IsMainThread()
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\live\\live_win_common.cpp",
                    1004,
                    0,
                    "%s",
                    "Sys_IsMainThread()") )
    {
        __debugbreak();
    }
    if ( !Sys_IsMainThread() )
        return 0;
    if ( s_shouldComError )
    {
        s_shouldComError = 0;
        Com_Error(ERR_DROP, s_comErrorString);
    }

    if ( !live_service->current.enabled )
        return 0;

    if ( dw_dupe_key->current.enabled )
        return 0;

    if ( dw_disconnect_detected )
    {
        Live_DemonwareDisconnectCleanup(0);
        return 0;
    }
    else
    {
        dw_disconnect_detected = 0;
        onlineStatus = (DWOnlineStatus)dwGetLogOnStatus(localControllerIndex);
        switch ( onlineStatus )
        {
            case DW_LIVE_ERROR:
                Live_DemonwareDisconnectCleanup(0);
                break;
            case DW_LIVE_DISCONNECTED:
                Com_DPrintf(16, "Connecting to Demonware...\n");
                Live_UpdateUiPopup("popup_connecting_dw", 0);
                dwLogOnStart(localControllerIndex);
                break;
            case DW_LIVE_CONNECTING:
                dwLogOnComplete();
                newOnlineStatus = (DWOnlineStatus)dwGetLogOnStatus(localControllerIndex);
                if ( newOnlineStatus == DW_LIVE_DISCONNECTED )
                {
                    Com_Printf(16, "Failed to log on.\n");
                    Live_UpdateUiPopup("popup_cannot_connect_to_dw", 0);
                    dw_disconnect_detected = 1;
                }
                break;
            case DW_LIVE_CONNECTED:
                elapsed = Sys_Milliseconds() - sessiontimer;
                sessiontimer = Sys_Milliseconds();
                dwLobbyPump(localControllerIndex);
                CL_CACValidate_Frame();
                PCache_Update(localControllerIndex);
                LiveCounter_Update(localControllerIndex);
                LiveNews_UpdateCountersTicker();
                LiveStorage_CheckOngoingSessionTasks();
                Live_CheckOngoingSessionTasks();
                LB_CheckOngoingTasks();
                LiveGroups_Update(localControllerIndex);
                Live_CheckOngoingMatchRecordingTasks();
                //if ( MatchMakingInfo::doUpdate(g_matchmakingInfo, elapsed, 0xBB8u, 0x2BF20u) )
                if ( g_matchmakingInfo->doUpdate(elapsed, 0xBB8u, 0x2BF20u) )
                    Session_Modify(0, &g_serverSession, 0, g_serverSession.publicSlots, g_serverSession.privateSlots);
                LogOnStatus = dwGetLogOnStatus(localControllerIndex);
                if ( LogOnStatus == 2 )
                    Com_Printf(16, "Lost connection to lobby server.\n");
                break;
            default:
                break;
        }
        online = dwGetLogOnStatus(localControllerIndex) == 4;
        dwNetStart(online);
        if ( online != Dvar_GetBool("dw_loggedin") )
            Dvar_SetBoolByName("dw_loggedin", online);
        v2 = dwGetLogOnStatus(localControllerIndex) == 4 && onlineStatus != DW_LIVE_CONNECTED;
        justSignedIn = v2;
        if ( v2 )
            Live_UserSignedInToLive(localControllerIndex, &disconnectMessage);
        if ( dwGetLogOnStatus(localControllerIndex) == 4 )
        {
            Live_FindSessionsPump();
            return 1;
        }
        else
        {
            return 0;
        }
    }
#else
    // KISAK: no DemonWare backend in this build. Instead of returning 0
    // forever (which leaves xblive_loggedin = 0, blocks the multiplayer
    // menus behind a "connecting to online service" popup, and prevents
    // LiveStorage_ReadStats from ever being invoked) we synthesise a
    // local sign-in on the first frame. From that point on the engine
    // believes it is signed in to Live, the stats system fires its
    // normal Read/Upload calls, and our local file persistence (see
    // LiveStorage_LocalSaveStats / LiveStorage_LocalLoadStats in
    // live_storage_win.cpp) takes care of rank-up / class / prestige
    // survival across restarts.
    static bool s_fakeSignedIn = false;
    if ( !s_fakeSignedIn )
    {
        s_fakeSignedIn = true;
        LiveStorage_FakeSignInLocal(localControllerIndex);
    }
    return 1;
#endif
}

// ---------------------------------------------------------------------------
// LiveStorage_FakeSignInLocal
// Stand-in for Live_UserSignedInToLive when no DemonWare backend is present.
// Runs once on the first Live_Frame_MP() and emulates just enough of the
// retail sign-in path that downstream code (stats, custom classes, prestige,
// CAC validation) believes a profile is signed in:
//   - flip xblive_loggedin / dw_loggedin on
//   - mark xenonUserData[0] as signed-in tier-none with a stable XUID
//   - call LiveStorage_NewUser (zeros the buffers + wires up fileOps)
//   - call LiveStorage_ReadStats which loads our local profile file
// ---------------------------------------------------------------------------
extern const dvar_t *xblive_loggedin;
extern XenonUserData xenonUserData[];

static void LiveStorage_FakeSignInLocal(int controllerIndex)
{
    if ( controllerIndex < 0 || controllerIndex >= 1 )
        return;
    if ( xenonUserData[controllerIndex].signinState >= 2 )
        return;

    xenonUserData[controllerIndex].signinState = 2;
    xenonUserData[controllerIndex].isGuestUser = 0;
    xenonUserData[controllerIndex].tier = USER_TIER_NONE;

    // Stable per-machine XUID. The exact value doesn't matter as long as it
    // stays the same across launches so the local stats file is keyed
    // consistently. We use 0xE0000000xxxxxxxx (Live unofficial range) with
    // the controller index in the low bits.
    unsigned __int64 xuid = 0xE000000000000001ull + (unsigned __int64)controllerIndex;
    LODWORD(xenonUserData[controllerIndex].xuid) = (unsigned int)xuid;
    HIDWORD(xenonUserData[controllerIndex].xuid) = (unsigned int)(xuid >> 32);
    XUIDToString(&xenonUserData[controllerIndex].xuid, xenonUserData[controllerIndex].xuidString);

    if ( xenonUserData[controllerIndex].gamertag[0] == 0 )
    {
        char gamerTagBuffer[32];
        if ( Live_UserGetName(controllerIndex, gamerTagBuffer, sizeof(gamerTagBuffer)) && gamerTagBuffer[0] )
            I_strncpyz(xenonUserData[controllerIndex].gamertag, gamerTagBuffer, 32);
        else
            I_strncpyz(xenonUserData[controllerIndex].gamertag, "Unknown Soldier", 32);
    }

    LiveStorage_NewUser(controllerIndex);
    Dvar_SetBoolByName("dw_loggedin", true);
    Dvar_SetBool((dvar_s *)xblive_loggedin, 1);
    Dvar_SetBoolByName("ui_ethernetLinkActive", true);

    LiveStorage_ReadStats(controllerIndex, 0, 0);
    LiveStorage_FetchPlaylistsLocal(controllerIndex);

    Com_Printf(
        16,
        "LiveStorage_FakeSignInLocal: signed-in controllerIndex=%d as '%s' (xuid=%s)\n",
        controllerIndex,
        xenonUserData[controllerIndex].gamertag,
        xenonUserData[controllerIndex].xuidString);
}

extern const dvar_t *dw_popup; // KISAKTODO: remove later
void __cdecl Live_UpdateUiPopup(const char *popupname, bool reset)
{
    int v2; // eax

    if ( cls.uiStarted )
    {
        Com_Printf(16, "Live_UpdateUiPopup: %s\n", popupname);
        if ( reset )
            v2 = I_stricmp(dw_popup->current.string, "");
        else
            v2 = I_stricmp(dw_popup->current.string, popupname);
        if ( v2 )
        {
            if ( reset )
                Dvar_SetString((dvar_s *)dw_popup, "");
            else
                Dvar_SetString((dvar_s *)dw_popup, popupname);
        }
    }
    else
    {
        Com_Printf(16, "%s\n", popupname);
    }
}

void __cdecl Live_ShowPlayerProfile(int localClientNum, unsigned __int64 playerXUID, char *playerName)
{
    int ControllerIndex; // eax
    __int64 v4; // rax
    char *v5; // eax
    uiInfo_s *uiInfo; // [esp+8h] [ebp-8h]
    menuDef_t *menu; // [esp+Ch] [ebp-4h]

    ControllerIndex = Com_LocalClient_GetControllerIndex(localClientNum);
    LODWORD(v4) = Live_GetXuid(ControllerIndex);
    if ( playerXUID && playerXUID != v4 )
    {
        v5 = va("%11d", (unsigned int)playerXUID);
        Dvar_SetStringByName("selectedPlayerXuid", v5);
        Dvar_SetStringByName("selectedFriendName", playerName);
        uiInfo = UI_GetInfo(localClientNum);
        menu = Menus_FindByName(&uiInfo->uiDC, "menu_playercard");
        if ( menu )
        {
            menu->openSlideDirection = 2;
            Menus_Open(localClientNum, &uiInfo->uiDC, menu);
        }
    }
}

void __cdecl SocketRouter_EmergencyFrame()
{
    Sys_IsMainThread();
}

