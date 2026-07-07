#include "aim_assist.h"

#include <client/client_limits.h>
#include <cstring>
#include <qcommon/graph.h>
#include <qcommon/cmd.h>
#include <universal/dvar.h>
#include <client/screen_placement.h>
#include <bgame/bg_local.h>
#include <bgame/bg_weapons.h>
#include <cgame/cg_weapons.h>
#include <bgame/bg_weapons_def.h>
#include <bgame/bg_weapons_ammo.h>
#include <cgame_mp/cg_local_mp.h>
#include <demo/demo_common.h>
#include <universal/com_math_anglevectors.h>
#include "aim_target.h"
#include <universal/com_math.h>
#include <clientscript/scr_const.h>
#include <xanim/dobj.h>
#include <qcommon/dobj_management.h>
#include <cgame_mp/cg_ents_mp.h>
#include <clientscript/cscr_stringlist.h>
#include <qcommon/common.h>
#include <cgame_mp/cg_main_mp.h>
#include <cmath>
#include <xanim/xanim.h>
#include <client_mp/cl_cgame_mp.h>

AimAssistGlobals aaGlobArray[1];
GraphFloat aaInputGraph[4];

namespace
{

static float ClampFloat(float value, float minVal, float maxVal)
{
    if (value < minVal)
        return minVal;
    if (value > maxVal)
        return maxVal;
    return value;
}

static float AimAssist_ScaleRegionWidth(int localClientNum, float screenWidth, float regionPixels)
{
    return ScrPlace_HiResGetScale() * regionPixels * scrPlaceView[localClientNum].scaleVirtualToReal[0] / screenWidth;
}

static float AimAssist_ScaleRegionHeight(int localClientNum, float screenHeight, float regionPixels)
{
    return ScrPlace_HiResGetScale() * regionPixels * scrPlaceView[localClientNum].scaleVirtualToReal[1] / screenHeight;
}

} // namespace

const dvar_s *aim_aimAssistRangeScale;
const dvar_s *aim_autoAimRangeScale;
const dvar_s *aim_input_graph_enabled;
const dvar_s *aim_input_graph_debug;
const dvar_s *aim_input_graph_index;
const dvar_s *aim_turnrate_pitch;
const dvar_s *aim_turnrate_pitch_ads;
const dvar_s *aim_turnrate_yaw;
const dvar_s *aim_turnrate_yaw_ads;
const dvar_s *aim_accel_turnrate_enabled;
const dvar_s *aim_accel_turnrate_debug;
const dvar_s *aim_accel_turnrate_lerp;
const dvar_s *aim_slowdown_enabled;
const dvar_s *aim_slowdown_debug;
const dvar_s *aim_slowdown_region_width;
const dvar_s *aim_slowdown_region_height;
const dvar_s *aim_slowdown_region_extended_width;
const dvar_s *aim_slowdown_region_extended_height;
const dvar_s *aim_slowdown_pitch_scale;
const dvar_s *aim_slowdown_pitch_scale_ads;
const dvar_s *aim_slowdown_yaw_scale;
const dvar_s *aim_slowdown_yaw_scale_ads;
const dvar_s *aim_autoaim_lerp;
const dvar_s *aim_autoaim_enabled;
const dvar_s *aim_autoaim_debug;
const dvar_s *aim_autoaim_region_width;
const dvar_s *aim_autoaim_region_height;
const dvar_s *aim_automelee_enabled;
const dvar_s *aim_automelee_debug;
const dvar_s *aim_automelee_lerp;
const dvar_s *aim_automelee_region_width;
const dvar_s *aim_automelee_region_height;
const dvar_s *aim_automelee_range;
const dvar_s *aim_autobayonet_range;
const dvar_s *aim_view_sensitivity_override;
const dvar_s *aim_lockon_enabled;
const dvar_s *aim_lockon_debug;
const dvar_s *aim_lockon_deflection;
const dvar_s *aim_lockon_strength;
const dvar_s *aim_lockon_pitch_strength;
const dvar_s *aim_lockon_region_width;
const dvar_s *aim_lockon_region_height;
const dvar_s *aim_scale_view_axis;
const dvar_s *aim_assist_script_disable;
const dvar_s *aim_assist_min_target_distance;


void __cdecl AimAssist_Init(int localClientNum)
{
    char graphName[128];
    int graphIndex;

    memset((unsigned __int8 *)&aaGlobArray[localClientNum], 0, sizeof(AimAssistGlobals));
    AimAssist_RegisterDvars();
    for ( graphIndex = 0; graphIndex < 4; ++graphIndex )
    {
        Com_sprintf(graphName, 0x80u, "aim_assist/view_input_%d.graph", graphIndex);
        GraphFloat_Load(&aaInputGraph[graphIndex], graphName, 1.0);
    }
    Cbuf_InsertText(0, "exec devgui_aimassist\n");
    for ( graphIndex = 0; graphIndex < 4; ++graphIndex )
    {
        Com_sprintf(graphName, 0x80u, "Aim Assist/Input Graph:1/Graph %d", graphIndex);
        GraphFloat_CreateDevGui(&aaInputGraph[graphIndex], graphName);
    }
}

void AimAssist_RegisterDvars()
{
    aim_aimAssistRangeScale = _Dvar_RegisterFloat(
                                                            "aim_aimAssistRangeScale",
                                                            1.0,
                                                            0.0,
                                                            2.0,
                                                            0x1080u,
                                                            "Scales the weapon's aim assist range");
    aim_autoAimRangeScale = _Dvar_RegisterFloat(
                                                        "aim_autoAimRangeScale",
                                                        1.0,
                                                        0.0,
                                                        2.0,
                                                        0x1080u,
                                                        "Scales the weapon's auto aim range");
    aim_input_graph_enabled = _Dvar_RegisterBool(
                                                            "aim_input_graph_enabled",
                                                            1,
                                                            0x1080u,
                                                            "Use graph for adjusting view input");
    aim_input_graph_debug = _Dvar_RegisterBool("aim_input_graph_debug", 0, 0x1080u, "Debug the view input graphs");
    aim_input_graph_index = _Dvar_RegisterInt("aim_input_graph_index", 3, 0, 3, 0x1080u, "Which input graph to use");
    aim_turnrate_pitch = _Dvar_RegisterFloat(
                                                 "aim_turnrate_pitch",
                                                 90.0,
                                                 0.0,
                                                 1080.0,
                                                 0x1080u,
                                                 "The vertical turn rate for aim assist when firing from the hip");
    aim_turnrate_pitch_ads = _Dvar_RegisterFloat(
                                                         "aim_turnrate_pitch_ads",
                                                         55.0,
                                                         0.0,
                                                         1080.0,
                                                         0x1080u,
                                                         "The turn rate up and down for aim assist when aiming down the sight");
    aim_turnrate_yaw = _Dvar_RegisterFloat(
                                             "aim_turnrate_yaw",
                                             260.0,
                                             0.0,
                                             1080.0,
                                             0x1080u,
                                             "The horizontal turn rate for aim assist when firing from the hip");
    aim_turnrate_yaw_ads = _Dvar_RegisterFloat(
                                                     "aim_turnrate_yaw_ads",
                                                     90.0,
                                                     0.0,
                                                     1080.0,
                                                     0x1080u,
                                                     "The horizontal turn rate for aim assist when aiming down the sight");
    aim_accel_turnrate_enabled = _Dvar_RegisterBool(
                                                                 "aim_accel_turnrate_enabled",
                                                                 1,
                                                                 0x1080u,
                                                                 "Enable/disable acceleration of the turnrates");
    aim_accel_turnrate_debug = _Dvar_RegisterBool(
                                                             "aim_accel_turnrate_debug",
                                                             0,
                                                             0x1080u,
                                                             "Turn on debugging info for the acceleration");
    aim_accel_turnrate_lerp = _Dvar_RegisterFloat(
                                                            "aim_accel_turnrate_lerp",
                                                            1200.0,
                                                            0.0,
                                                            4000.0,
                                                            0x1080u,
                                                            "The acceleration of the turnrates");
    aim_slowdown_enabled = _Dvar_RegisterBool(
                                                     "aim_slowdown_enabled",
                                                     1,
                                                     0x1080u,
                                                     "Slowdown the turn rate when the cross hair passes over a target");
    aim_slowdown_debug = _Dvar_RegisterBool("aim_slowdown_debug", 0, 0x1080u, "Turn on debugging info for aim slowdown");
    aim_slowdown_region_width = _Dvar_RegisterFloat(
                                                                "aim_slowdown_region_width",
                                                                90.0,
                                                                0.0,
                                                                640.0,
                                                                0x1080u,
                                                                "The screen width of the aim slowdown region");
    aim_slowdown_region_height = _Dvar_RegisterFloat(
                                                                 "aim_slowdown_region_height",
                                                                 90.0,
                                                                 0.0,
                                                                 480.0,
                                                                 0x1080u,
                                                                 "The screen height of the aim assist slowdown region");
    aim_slowdown_region_extended_width = _Dvar_RegisterFloat(
                                                                                 "aim_slowdown_region_extended_width",
                                                                                 90.0,
                                                                                 0.0,
                                                                                 640.0,
                                                                                 0x1080u,
                                                                                 "The screen width of the extended aim slowdown region");
    aim_slowdown_region_extended_height = _Dvar_RegisterFloat(
                                                                                    "aim_slowdown_region_extended_height",
                                                                                    90.0,
                                                                                    0.0,
                                                                                    480.0,
                                                                                    0x1080u,
                                                                                    "The screen height of the extended aim assist slowdown region");
    aim_slowdown_pitch_scale = _Dvar_RegisterFloat(
                                                             "aim_slowdown_pitch_scale",
                                                             0.40000001,
                                                             0.0,
                                                             1.0,
                                                             0x1080u,
                                                             "The vertical aim assist slowdown ratio from the hip");
    aim_slowdown_pitch_scale_ads = _Dvar_RegisterFloat(
                                                                     "aim_slowdown_pitch_scale_ads",
                                                                     0.5,
                                                                     0.0,
                                                                     1.0,
                                                                     0x1080u,
                                                                     "The vertical aim assist slowdown ratio when aiming down the sight");
    aim_slowdown_yaw_scale = _Dvar_RegisterFloat(
                                                         "aim_slowdown_yaw_scale",
                                                         0.40000001,
                                                         0.0,
                                                         1.0,
                                                         0x1080u,
                                                         "The horizontal aim assist slowdown ratio from the hip");
    aim_slowdown_yaw_scale_ads = _Dvar_RegisterFloat(
                                                                 "aim_slowdown_yaw_scale_ads",
                                                                 0.5,
                                                                 0.0,
                                                                 1.0,
                                                                 0x1080u,
                                                                 "The horizontal aim assist slowdown ratio when aiming down the sight");
    aim_autoaim_lerp = _Dvar_RegisterFloat(
                                             "aim_autoaim_lerp",
                                             40.0,
                                             0.0,
                                             100.0,
                                             0x1080u,
                                             "The rate in degrees per second that the auto aim will converge to its target");
    aim_autoaim_enabled = _Dvar_RegisterBool("aim_autoaim_enabled", 0, 0x1080u, "Turn on auto aim");
    aim_autoaim_debug = _Dvar_RegisterBool("aim_autoaim_debug", 0, 0x1080u, "Turn on auto aim debugging");
    aim_autoaim_region_width = _Dvar_RegisterFloat(
                                                             "aim_autoaim_region_width",
                                                             160.0,
                                                             0.0,
                                                             640.0,
                                                             0x1080u,
                                                             "The width of the auto aim region in virtual screen coordinates (0 - 640)");
    aim_autoaim_region_height = _Dvar_RegisterFloat(
                                                                "aim_autoaim_region_height",
                                                                120.0,
                                                                0.0,
                                                                480.0,
                                                                0x1080u,
                                                                "The height of the auto aim region in virtual screen coordinates (0 - 480)");
    aim_automelee_enabled = _Dvar_RegisterBool("aim_automelee_enabled", 1, 0x1080u, "Turn on auto melee");
    aim_automelee_debug = _Dvar_RegisterBool("aim_automelee_debug", 0, 0x5080u, "Turn on auto melee debugging");
    aim_automelee_lerp = _Dvar_RegisterFloat(
                                                 "aim_automelee_lerp",
                                                 40.0,
                                                 0.0,
                                                 100.0,
                                                 0x1080u,
                                                 "The rate in degrees per second that the auto melee will converge to its target");
    aim_automelee_region_width = _Dvar_RegisterFloat(
                                                                 "aim_automelee_region_width",
                                                                 320.0,
                                                                 0.0,
                                                                 640.0,
                                                                 0x1080u,
                                                                 "The width of the auto melee region in virtual screen coordinates (0 - 640)");
    aim_automelee_region_height = _Dvar_RegisterFloat(
                                                                    "aim_automelee_region_height",
                                                                    240.0,
                                                                    0.0,
                                                                    480.0,
                                                                    0x1080u,
                                                                    "The height of the auto melee region in virtual screen coordinates (0 - 480)");
    aim_automelee_range = _Dvar_RegisterFloat(
                                                    "aim_automelee_range",
                                                    128.0,
                                                    0.0,
                                                    255.0,
                                                    0x1080u,
                                                    "The range of the auto melee");
    aim_autobayonet_range = _Dvar_RegisterFloat(
                                                        "aim_autobayonet_range",
                                                        200.0,
                                                        0.0,
                                                        255.0,
                                                        0x1080u,
                                                        "The range of the auto bayonet");
    aim_view_sensitivity_override = _Dvar_RegisterFloat(
                                                                        "aim_view_sensitivity_override",
                                                                        -1.0,
                                                                        -1.0,
                                                                        5.0,
                                                                        0x1000u,
                                                                        "override value for view sensitivity");
    aim_lockon_enabled = _Dvar_RegisterBool(
                                                 "aim_lockon_enabled",
                                                 0,
                                                 0x1080u,
                                                 "Aim lock on helps the player to stay on target");
    aim_lockon_debug = _Dvar_RegisterBool("aim_lockon_debug", 0, 0x1080u, "Turn on debugging info for aim lock on");
    aim_lockon_deflection = _Dvar_RegisterFloat(
                                                        "aim_lockon_deflection",
                                                        0.050000001,
                                                        0.0,
                                                        1.0,
                                                        0x1080u,
                                                        "The amount of stick deflection for the lockon to activate");
    aim_lockon_strength = _Dvar_RegisterFloat(
                                                    "aim_lockon_strength",
                                                    0.60000002,
                                                    0.0,
                                                    1.0,
                                                    0x1080u,
                                                    "The amount of aim assistance given by the target lock on (yaw)");
    aim_lockon_pitch_strength = _Dvar_RegisterFloat(
                                                                "aim_lockon_pitch_strength",
                                                                0.60000002,
                                                                0.0,
                                                                1.0,
                                                                0x1000u,
                                                                "The amount of aim assistance given by the target lock on (pitch)");
    aim_lockon_region_width = _Dvar_RegisterFloat(
                                                            "aim_lockon_region_width",
                                                            90.0,
                                                            0.0,
                                                            640.0,
                                                            0x1080u,
                                                            "The width of the auto aim region in virtual screen coordinates(0-640)");
    aim_lockon_region_height = _Dvar_RegisterFloat(
                                                             "aim_lockon_region_height",
                                                             90.0,
                                                             0.0,
                                                             480.0,
                                                             0x1080u,
                                                             "The height of the auto aim region in virtual screen coordinates(0-480)");
    aim_scale_view_axis = _Dvar_RegisterBool(
                                                    "aim_scale_view_axis",
                                                    1,
                                                    0x1080u,
                                                    "Scale the influence of each input axis so that the major axis has more influence on the control");
    aim_assist_script_disable = _Dvar_RegisterBool(
                                                                "aim_assist_script_disable",
                                                                0,
                                                                0x4000u,
                                                                "Override aim assist from script: set to true to disable code aim assist (i.e. for meatshield)");
    aim_assist_min_target_distance = _Dvar_RegisterFloat(
            "aim_assist_min_target_distance",
            10000.0,
            0.0,
            10000.0,
            0x5000u,
            "Aim assist will not work on targets beyond this distance");
}

void __cdecl AimAssist_Setup(int localClientNum, const playerState_s *ps)
{
    AimAssistGlobals *aaGlob;

    if ( localClientNum
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp",
                    501,
                    0,
                    "localClientNum doesn't index MAX_LOCAL_CLIENTS\n\t%i not in [0, %i)",
                    localClientNum,
                    1) )
    {
        __debugbreak();
    }
    if ( !ps && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 502, 0, "%s", "ps") )
        __debugbreak();
    aaGlob = &aaGlobArray[localClientNum];
    memset((unsigned __int8 *)aaGlob, 0, sizeof(AimAssistGlobals));
    aaGlob->initialized = 1;
    aaGlob->fovTurnRateScale = 1.0f;
    aaGlob->fovScaleInv = 1.0f;
    aaGlob->screenWidth = scrPlaceView[localClientNum].realViewportSize[0];
    aaGlob->screenHeight = scrPlaceView[localClientNum].realViewportSize[1];
    aaGlob->autoAimTargetEnt = 1023;
    aaGlob->autoMeleeTargetEnt = 1023;
    aaGlob->lockOnTargetEnt = 1023;
    aaGlob->aimSlowdownTargetEnt = 1023;
    aaGlob->autoAimActive = 0;
    aaGlob->autoAimHasRealTarget = 0;
    aaGlob->autoAimJustGotTarget = 0;
    aaGlob->aimSlowdownActive = 0;
    AimAssist_BackupPlayerState(localClientNum, ps);
}

void __cdecl AimAssist_BackupPlayerState(int localClientNum, const playerState_s *ps)
{
    unsigned int NumWeapons;
    AimAssistGlobals *aaGlob;

    if ( localClientNum
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp",
                    585,
                    0,
                    "localClientNum doesn't index MAX_LOCAL_CLIENTS\n\t%i not in [0, %i)",
                    localClientNum,
                    1) )
    {
        __debugbreak();
    }
    if ( !ps && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 586, 0, "%s", "ps") )
        __debugbreak();
    aaGlob = &aaGlobArray[localClientNum];
    aaGlob->ps.velocity[0] = ps->velocity[0];
    aaGlob->ps.velocity[1] = ps->velocity[1];
    aaGlob->ps.velocity[2] = ps->velocity[2];
    aaGlob->ps.viewangles[0] = ps->viewangles[0];
    aaGlob->ps.viewangles[1] = ps->viewangles[1];
    aaGlob->ps.viewangles[2] = ps->viewangles[2];
    aaGlob->ps.origin[0] = ps->origin[0];
    aaGlob->ps.origin[1] = ps->origin[1];
    aaGlob->ps.origin[2] = ps->origin[2];
    aaGlob->ps.eFlags = ps->eFlags;
    aaGlob->ps.linkFlags = ps->linkFlags;
    aaGlob->ps.pm_flags = ps->pm_flags;
    aaGlob->ps.weapFlags = ps->weapFlags;
    aaGlob->ps.weaponstate = ps->weaponstate;
    aaGlob->ps.fWeaponPosFrac = ps->fWeaponPosFrac;
    aaGlob->ps.viewHeightCurrent = ps->viewHeightCurrent;
    if ( (ps->eFlags & 0x4300) != 0 )
    {
        if ( ps->viewlocked_entNum == 1023
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp",
                        605,
                        0,
                        "%s",
                        "ps->viewlocked_entNum != ENTITYNUM_NONE") )
        {
            __debugbreak();
        }
        if ( (unsigned int)ps->viewlocked_entNum >= 0x400
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp",
                        606,
                        0,
                        "%s\n\t(ps->viewlocked_entNum) = %i",
                        "((ps->viewlocked_entNum >= 0) && (ps->viewlocked_entNum < (1<<10)))",
                        ps->viewlocked_entNum) )
        {
            __debugbreak();
        }
        aaGlob->ps.weapIndex = CG_GetEntity(localClientNum, ps->viewlocked_entNum)->nextState.weapon;
    }
    else
    {
        aaGlob->ps.weapIndex = BG_GetViewmodelWeaponIndex(ps);
    }
    aaGlob->ps.meleeWeapIndex = CG_GetPlayerMeleeWeapon(ps);
    if ( aaGlob->ps.weapIndex >= BG_GetNumWeapons() )
    {
        NumWeapons = BG_GetNumWeapons();
        if ( !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp",
                        616,
                        0,
                        "aaGlob->ps.weapIndex doesn't index BG_GetNumWeapons()\n\t%i not in [0, %i)",
                        aaGlob->ps.weapIndex,
                        NumWeapons) )
            __debugbreak();
    }
    aaGlob->ps.hasAmmo = BG_WeaponAmmo(ps, aaGlob->ps.weapIndex) > 0 || ps->throwBackGrenadeTimeLeft > 0;
    aaGlob->ps.isDualWielding = BG_GetWeaponDef(aaGlob->ps.weapIndex)->bDualWield;
    aaGlob->ps.targetAssistDisabled = AimAssist_PlayerDisabledAutoAim();
}

centity_s *__cdecl CG_GetEntity(int localClientNum, int entityIndex)
{
    localClientNum = Com_ClampLocalClientNum(localClientNum);
    if ( (unsigned int)entityIndex >= 0x600
        && !Assert_MyHandler(
                    "c:\\projects_pc\\cod\\codsrc\\src\\cgame\\../cgame_mp/cg_local_mp.h",
                    1898,
                    0,
                    "entityIndex doesn't index MAX_LOCAL_CENTITIES\n\t%i not in [0, %i)",
                    entityIndex,
                    1536) )
    {
        __debugbreak();
    }
    if ( entityIndex < 1024 )
        return &cg_entitiesArray[localClientNum][entityIndex];
    else
        return &cg_fakeEntitiesArray[512 * localClientNum - 1024 + entityIndex].cent;
}

char __cdecl AimAssist_PlayerDisabledAutoAim()
{
    Demo_IsPlaying();
    return 1;
}

// Decomp: CoDMPServer.c:113431
void __cdecl AimAssist_UpdateScreenTargets(
                int localClientNum,
                const float *viewOrg,
                const float *viewAngles,
                float tanHalfFovX,
                float tanHalfFovY)
{
    AimScreenTarget screenTarget;
    int targetCount;
    const AimTarget *target;
    const AimTarget *targetList;
    centity_s *cent;
    float entMtx[4][3];
    int targetIndex;
    float bounds[2][3];
    float clipBounds[2][3];
    AimAssistGlobals *aaGlob;

    aaGlob = &aaGlobArray[localClientNum];
    if ( aaGlob->initialized )
    {
        aaGlob->viewOrigin[0] = viewOrg[0];
        aaGlob->viewOrigin[1] = viewOrg[1];
        aaGlob->viewOrigin[2] = viewOrg[2];
        aaGlob->viewAngles[0] = viewAngles[0];
        aaGlob->viewAngles[1] = viewAngles[1];
        aaGlob->viewAngles[2] = viewAngles[2];
        AnglesToAxis(viewAngles, aaGlob->viewAxis);
        AimAssist_FovScale(aaGlob, tanHalfFovY);
        AimAssist_CreateScreenMatrix(aaGlob, tanHalfFovX, tanHalfFovY);
        aaGlob->screenTargetCount = 0;
        AimTarget_GetClientTargetList(localClientNum, (AimTargetGlob **)&targetList, &targetCount);
        for ( targetIndex = 0; targetIndex < targetCount; ++targetIndex )
        {
            target = &targetList[targetIndex];
            cent = CG_GetEntity(localClientNum, target->entIndex);
            if ( ((*((unsigned int *)cent + 201) >> 1) & 1) != 0
                && target->worldDistSqr <= (float)(aim_assist_min_target_distance->current.value
                                                                                 * aim_assist_min_target_distance->current.value) )
            {
                *(_QWORD *)&bounds[0][0] = *(_QWORD *)target->mins;
                bounds[0][2] = target->mins[2];
                bounds[1][0] = target->maxs[0];
                bounds[1][1] = target->maxs[1];
                bounds[1][2] = target->maxs[2];
                AnglesToAxis(cent->pose.angles, entMtx);
                entMtx[3][0] = cent->pose.origin[0];
                entMtx[3][1] = cent->pose.origin[1];
                entMtx[3][2] = cent->pose.origin[2];
                if ( AimAssist_ConvertToClipBounds(aaGlob, bounds, entMtx, clipBounds) )
                {
                    screenTarget.entIndex = target->entIndex;
                    *(_QWORD *)screenTarget.clipMins = *(_QWORD *)&clipBounds[0][0];
                    *(_QWORD *)screenTarget.clipMaxs = *(_QWORD *)&clipBounds[1][0];
                    screenTarget.velocity[0] = target->velocity[0];
                    screenTarget.velocity[1] = target->velocity[1];
                    screenTarget.velocity[2] = target->velocity[2];
                    if ( AimAssist_CalcAimPos(localClientNum, cent, target, screenTarget.aimPos) )
                    {
                        screenTarget.distSqr = target->worldDistSqr;
                        screenTarget.crosshairDistSqr = AimAssist_GetCrosshairDistSqr(clipBounds[0], clipBounds[1]);
                        AimAssist_AddToTargetList(aaGlob, &screenTarget);
                    }
                }
            }
        }
    }
}

// Decomp: CoDMPServer.c:113527
void __cdecl AimAssist_FovScale(AimAssistGlobals *aaGlob, float tanHalfFovY)
{
    constexpr float kBaseTanHalfFovY = 0.47780272f;
    constexpr float kWideScreenFovScale = 0.75f;
    const float tanHalfBaseFovY = tanf(DEG2RAD(cg_fov->current.value) * 0.5f) * kWideScreenFovScale;

    iassert(aaGlob);
    aaGlob->fovTurnRateScale = tanHalfFovY / kBaseTanHalfFovY;
    iassert(tanHalfBaseFovY != 0.0f);
    aaGlob->fovScaleInv = tanHalfBaseFovY / tanHalfFovY;
}

void    AimAssist_CreateScreenMatrix(
                AimAssistGlobals *aaGlob,
                float tanHalfFovX,
                float tanHalfFovY)
{
        float viewMtx[4][4];
        float projMtx[4][4];
        float screenMtx[4][4];

        iassert(aaGlob);

        MatrixForViewer(aaGlob->viewOrigin, aaGlob->viewAxis, viewMtx);
        InfinitePerspectiveMatrix(tanHalfFovX, tanHalfFovY, 1.0f, projMtx);
        MatrixMultiply44(viewMtx, projMtx, screenMtx);
        MatrixTranspose44(screenMtx, aaGlob->screenMtx);
        MatrixInverse44(aaGlob->screenMtx, aaGlob->invScreenMtx);
}

// Decomp: CoDMPServer.c:113600
char __cdecl AimAssist_ConvertToClipBounds(
                const AimAssistGlobals *aaGlob,
                const float (*bounds)[3],
                const float (*mtx)[3],
                float (*clipBounds)[3])
{
    float worldCorner[3];
    float clipCorner[3];
    int boundsCornerIndex;
    float worldCornerRotated[3];

    if ( !bounds
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 722, 0, "%s", "bounds") )
    {
        __debugbreak();
    }
    if ( !mtx && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 723, 0, "%s", "mtx") )
        __debugbreak();
    if ( !clipBounds
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 724, 0, "%s", "clipBounds") )
    {
        __debugbreak();
    }
    ClearBounds((float *)clipBounds, &(*clipBounds)[3]);
    for ( boundsCornerIndex = 0; boundsCornerIndex < 8; ++boundsCornerIndex )
    {
        worldCorner[0] = (*bounds)[3 * (boundsCornerIndex & 1)];
        worldCorner[1] = (*bounds)[3 * ((boundsCornerIndex >> 1) & 1) + 1];
        worldCorner[2] = (*bounds)[3 * ((boundsCornerIndex >> 2) & 1) + 2];
        MatrixTransformVector43(worldCorner, mtx, worldCornerRotated);
        if ( AimAssist_XfmWorldPointToClipSpace(aaGlob, worldCornerRotated, clipCorner) )
            AddPointToBounds(clipCorner, (float *)clipBounds, &(*clipBounds)[3]);
    }
    if ( (*clipBounds)[0] >= (*clipBounds)[3]
        || (*clipBounds)[1] >= (*clipBounds)[4]
        || (*clipBounds)[2] >= (*clipBounds)[5] )
    {
        return 0;
    }
    if ( (*clipBounds)[0] > 1.0 || (*clipBounds)[1] > 1.0 || (*clipBounds)[2] > 1.0 )
        return 0;
    if ( (*clipBounds)[3] < -1.0 || (*clipBounds)[4] < -1.0 || (*clipBounds)[5] < 0.0 )
        return 0;
    (*clipBounds)[0] = ClampFloat((*clipBounds)[0], -1.0f, 1.0f);
    (*clipBounds)[1] = ClampFloat((*clipBounds)[1], -1.0f, 1.0f);
    (*clipBounds)[2] = ClampFloat((*clipBounds)[2], 0.0f, 1.0f);
    (*clipBounds)[3] = ClampFloat((*clipBounds)[3], -1.0f, 1.0f);
    (*clipBounds)[4] = ClampFloat((*clipBounds)[4], -1.0f, 1.0f);
    (*clipBounds)[5] = ClampFloat((*clipBounds)[5], 0.0f, 1.0f);
    return 1;
}

char __cdecl AimAssist_XfmWorldPointToClipSpace(const AimAssistGlobals *aaGlob, float *in, float *out)
{
    float clipW;
    float clipY;
    float clipZ;
    float worldX;
    float worldY;
    float worldZ;

    if ( !aaGlob
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 689, 0, "%s", "aaGlob") )
    {
        __debugbreak();
    }
    if ( !in && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 690, 0, "%s", "in") )
        __debugbreak();
    if ( !out && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 691, 0, "%s", "out") )
        __debugbreak();
    worldX = in[0];
    worldY = in[1];
    worldZ = in[2];
    clipW = (float)((float)((float)(aaGlob->screenMtx[3][0] * worldX) + (float)(aaGlob->screenMtx[3][1] * worldY))
                    + (float)(aaGlob->screenMtx[3][2] * worldZ))
            + aaGlob->screenMtx[3][3];
    if ( clipW <= 0.0f )
        return 0;
    clipY = (float)((float)((float)(aaGlob->screenMtx[1][0] * worldX) + (float)(aaGlob->screenMtx[1][1] * worldY))
                    + (float)(aaGlob->screenMtx[1][2] * worldZ))
            + aaGlob->screenMtx[1][3];
    clipZ = (float)((float)((float)(aaGlob->screenMtx[2][0] * worldX) + (float)(aaGlob->screenMtx[2][1] * worldY))
                    + (float)(aaGlob->screenMtx[2][2] * worldZ))
            + aaGlob->screenMtx[2][3];
    *out = (float)((float)((float)(aaGlob->screenMtx[0][0] * worldX) + (float)(aaGlob->screenMtx[0][1] * worldY))
                   + (float)(aaGlob->screenMtx[0][2] * worldZ) + aaGlob->screenMtx[0][3])
           / clipW;
    out[1] = (clipY / clipW) * -1.0f;
    out[2] = clipZ / clipW;
    return 1;
}

double __cdecl AimAssist_GetCrosshairDistSqr(const float *clipMins, const float *clipMaxs)
{
    float clipCenterX;
    float clipCenterY;

    if ( !clipMins
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 766, 0, "%s", "clipMins") )
    {
        __debugbreak();
    }
    if ( !clipMaxs
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 767, 0, "%s", "clipMaxs") )
    {
        __debugbreak();
    }
    clipCenterX = 0.5f * (clipMins[0] + clipMaxs[0]);
    clipCenterY = 0.5f * (clipMins[1] + clipMaxs[1]);
    return clipCenterX * clipCenterX + clipCenterY * clipCenterY;
}

void __cdecl AimAssist_AddToTargetList(AimAssistGlobals *aaGlob, const AimScreenTarget *screenTarget)
{
    int insertIndex;
    int searchEndIndex;

    if ( !aaGlob
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 883, 0, "%s", "aaGlob") )
    {
        __debugbreak();
    }
    if ( !screenTarget
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 884, 0, "%s", "screenTarget") )
    {
        __debugbreak();
    }
    insertIndex = 0;
    searchEndIndex = aaGlob->screenTargetCount;
    while ( insertIndex < searchEndIndex )
    {
        const int midIndex = (searchEndIndex + insertIndex) / 2;

        if ( AimAssist_CompareTargets(screenTarget, &aaGlob->screenTargets[midIndex]) <= 0 )
            insertIndex = midIndex + 1;
        else
            searchEndIndex = midIndex;
    }
    if ( insertIndex < 64 )
    {
        if ( aaGlob->screenTargetCount == 64 )
            --aaGlob->screenTargetCount;
        memmove(
            (unsigned __int8 *)&aaGlob->screenTargets[insertIndex + 1],
            (unsigned __int8 *)&aaGlob->screenTargets[insertIndex],
            52 * (aaGlob->screenTargetCount - insertIndex));
        memcpy(&aaGlob->screenTargets[insertIndex], screenTarget, sizeof(aaGlob->screenTargets[insertIndex]));
        ++aaGlob->screenTargetCount;
    }
}

int __cdecl AimAssist_CompareTargets(const AimScreenTarget *screenTargetA, const AimScreenTarget *screenTargetB)
{
    if ( !screenTargetA
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 860, 0, "%s", "screenTargetA") )
    {
        __debugbreak();
    }
    if ( !screenTargetB
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 861, 0, "%s", "screenTargetB") )
    {
        __debugbreak();
    }
    if ( screenTargetB->crosshairDistSqr > screenTargetA->crosshairDistSqr )
        return 1;
    if ( screenTargetA->crosshairDistSqr <= screenTargetB->crosshairDistSqr )
        return 0;
    return -1;
}

int __cdecl AimAssist_CalcAimPos(
                int localClientNum,
                const centity_s *targetEnt,
                const AimTarget *target,
                float *aimPos)
{
    float boundsCenterY;
    float boundsCenterZ;

    if ( !targetEnt
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 941, 0, "%s", "targetEnt") )
    {
        __debugbreak();
    }
    if ( !aimPos
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 942, 0, "%s", "aimPos") )
    {
        __debugbreak();
    }
    if ( targetEnt->nextState.eType == 1 )
        return AimTarget_GetTagPos(localClientNum, targetEnt, scr_const.aim_bone, aimPos);
    if ( !target )
        return 0;
    boundsCenterY = (target->mins[1] + target->maxs[1]) * 0.5f;
    boundsCenterZ = (target->mins[2] + target->maxs[2]) * 0.5f;
    *aimPos = targetEnt->pose.origin[0] + (target->mins[0] + target->maxs[0]) * 0.5f;
    aimPos[1] = targetEnt->pose.origin[1] + boundsCenterY;
    aimPos[2] = targetEnt->pose.origin[2] + boundsCenterZ;
    return 1;
}

int __cdecl AimTarget_GetTagPos(int localClientNum, const centity_s *cent, unsigned int tagName, float *pos)
{
    const char *tagNameString;
    DObj *dobj;

    if ( !cent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 920, 0, "%s", "cent") )
        __debugbreak();
    if ( !pos && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 921, 0, "%s", "pos") )
        __debugbreak();
    dobj = Com_GetClientDObj(cent->nextState.number, localClientNum);
    if ( !dobj )
        return 0;
    if ( !CG_DObjGetWorldTagPos(&cent->pose, dobj, tagName, pos) )
    {
        tagNameString = SL_ConvertToString(tagName, SCRIPTINSTANCE_SERVER);
        Com_Error(ERR_DROP, "AimTarget_GetTagPos: Cannot find tag [%s] on entity\n", tagNameString);
    }
    return 1;
}

int __cdecl AimAssist_GetScreenTargetCount(int localClientNum)
{
    return aaGlobArray[localClientNum].screenTargetCount;
}

int __cdecl AimAssist_GetScreenTargetEntity(int localClientNum, unsigned int targetIndex)
{
    const AimAssistGlobals *aaGlob;

    aaGlob = &aaGlobArray[localClientNum];
    if ( targetIndex >= aaGlob->screenTargetCount
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp",
                    1067,
                    0,
                    "targetIndex doesn't index aaGlob->screenTargetCount\n\t%i not in [0, %i)",
                    targetIndex,
                    aaGlob->screenTargetCount) )
    {
        __debugbreak();
    }
    return aaGlob->screenTargets[targetIndex].entIndex;
}

void __cdecl AimAssist_ClearEntityReference(int localClientNum, int entIndex)
{
    AimAssistGlobals *aaGlob;

    aaGlob = &aaGlobArray[localClientNum];
    if ( aaGlob->autoAimTargetEnt == entIndex )
        aaGlob->autoAimTargetEnt = 1023;
    if ( aaGlob->autoMeleeTargetEnt == entIndex )
        aaGlob->autoMeleeTargetEnt = 1023;
    if ( aaGlob->lockOnTargetEnt == entIndex )
        aaGlob->lockOnTargetEnt = 1023;
    if ( aaGlob->aimSlowdownTargetEnt == entIndex )
        aaGlob->aimSlowdownTargetEnt = 1023;
}

bool __cdecl AimAssist_IsPrevTargetEntity(int localClientNum, int entIndex)
{
    AimAssistGlobals *aaGlob;

    aaGlob = &aaGlobArray[localClientNum];
    if ( aaGlob->aimSlowdownTargetEnt == entIndex )
        return 1;
    if ( aaGlob->autoMeleeTargetEnt == entIndex )
        return 1;
    if ( aaGlob->lockOnTargetEnt == entIndex )
        return 1;
    return aaGlob->autoAimTargetEnt == entIndex;
}

void __cdecl AimAssist_UpdateGamePadInput(const AimInput *input, AimOutput *output)
{
    int buttonBitsWordIndex;
    AimAssistGlobals *aaGlob;

    if ( !input
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 2330, 0, "%s", "input") )
    {
        __debugbreak();
    }
    if ( !output
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 2331, 0, "%s", "output") )
    {
        __debugbreak();
    }
    output->pitch = input->pitch;
    output->yaw = input->yaw;
    aaGlob = &aaGlobArray[input->localClientNum];
    if ( aaGlob->initialized )
    {
        AimAssist_UpdateTweakables(input->localClientNum);
        AimAssist_UpdateAdsLerp(input);
        AimAssist_ApplyTurnRates(input, output);
        if ( aaGlob->ps.weaponstate < 47 || aaGlob->ps.weaponstate > 49 )
        {
            AimAssist_ApplyAutoAim(input, output);
            AimAssist_ApplyAutoMelee(input, output);
            AimAssist_ApplyLockOn(input, output);
        }
        for ( buttonBitsWordIndex = 0; buttonBitsWordIndex < 2; ++buttonBitsWordIndex )
            aaGlob->prev_button_bits.array[buttonBitsWordIndex] = input->button_bits.array[buttonBitsWordIndex];
    }
}

// Decomp: CoDMPServer.c:114157
void __cdecl AimAssist_UpdateTweakables(int localClientNum)
{
    AimAssistGlobals *aaGlob;
    AimTweakables *tweaks;

    if ( localClientNum
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp",
                    311,
                    0,
                    "localClientNum doesn't index MAX_LOCAL_CLIENTS\n\t%i not in [0, %i)",
                    localClientNum,
                    1) )
    {
        __debugbreak();
    }
    aaGlob = &aaGlobArray[localClientNum];
    tweaks = &aaGlob->tweakables;
    if ( aaGlob->aimSlowdownActive )
    {
        tweaks->slowdownRegionWidth = AimAssist_ScaleRegionWidth(
            localClientNum,
            aaGlob->screenWidth,
            aim_slowdown_region_extended_width->current.value);
        tweaks->slowdownRegionHeight = AimAssist_ScaleRegionHeight(
            localClientNum,
            aaGlob->screenHeight,
            aim_slowdown_region_extended_height->current.value);
    }
    else
    {
        tweaks->slowdownRegionWidth = AimAssist_ScaleRegionWidth(
            localClientNum,
            aaGlob->screenWidth,
            aim_slowdown_region_width->current.value);
        tweaks->slowdownRegionHeight = AimAssist_ScaleRegionHeight(
            localClientNum,
            aaGlob->screenHeight,
            aim_slowdown_region_height->current.value);
    }
    if ( aaGlob->overrideSnapWidthAndLerp )
    {
        tweaks->autoAimRegionWidth = AimAssist_ScaleRegionWidth(
                                         localClientNum,
                                         aaGlob->screenWidth,
                                         aaGlob->overrideAutoaimWidthValue)
                                     * aaGlob->fovScaleInv;
    }
    else
    {
        tweaks->autoAimRegionWidth = AimAssist_ScaleRegionWidth(
                                         localClientNum,
                                         aaGlob->screenWidth,
                                         aim_autoaim_region_width->current.value)
                                     * aaGlob->fovScaleInv;
    }
    tweaks->autoAimRegionHeight = AimAssist_ScaleRegionHeight(
                                      localClientNum,
                                      aaGlob->screenHeight,
                                      aim_autoaim_region_height->current.value)
                                  * aaGlob->fovScaleInv;
    tweaks->autoMeleeRegionWidth = AimAssist_ScaleRegionWidth(
                                       localClientNum,
                                       aaGlob->screenWidth,
                                       aim_automelee_region_width->current.value)
                                   * aaGlob->fovScaleInv;
    tweaks->autoMeleeRegionHeight = AimAssist_ScaleRegionHeight(
                                        localClientNum,
                                        aaGlob->screenHeight,
                                        aim_automelee_region_height->current.value)
                                    * aaGlob->fovScaleInv;
    tweaks->lockOnRegionWidth = AimAssist_ScaleRegionWidth(
                                    localClientNum,
                                    aaGlob->screenWidth,
                                    aim_lockon_region_width->current.value);
    tweaks->lockOnRegionHeight = AimAssist_ScaleRegionHeight(
                                     localClientNum,
                                     aaGlob->screenHeight,
                                     aim_lockon_region_height->current.value);
}

void __cdecl AimAssist_UpdateAdsLerp(const AimInput *input)
{
    AimAssistGlobals *aaGlob;

    if ( !input
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 347, 0, "%s", "input") )
    {
        __debugbreak();
    }
    aaGlob = &aaGlobArray[input->localClientNum];
    if ( Demo_IsPlaying() )
    {
        aaGlob->adsLerp = 0.0f;
    }
    else
    {
        aaGlob->adsLerp = aaGlob->ps.fWeaponPosFrac;
        if ( (aaGlob->ps.eFlags & 0x300) != 0 )
        {
            if ( input->button_bits.testBit(11) )
                aaGlob->adsLerp = 1.0f;
        }
    }
}

// Decomp: CoDMPServer.c:114303
void __cdecl AimAssist_ApplyTurnRates(const AimInput *input, AimOutput *output)
{
    float adjustedYawAxis;
    float pitchTurnRate;
    float adjustedPitchAxis;
    float yawDelta;
    float pitchSign;
    float slowdownYawScale;
    float sensitivity;
    float yawTurnRate;
    float pitchDelta;
    float slowdownPitchScale;
    AimAssistGlobals *aaGlob;
    float yawSign;
    float turnRateTrackSpeed;

    slowdownPitchScale = 0.0f;
    slowdownYawScale = 0.0f;
    if ( !input
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1429, 0, "%s", "input") )
    {
        __debugbreak();
    }
    if ( !output
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1430, 0, "%s", "output") )
    {
        __debugbreak();
    }
    aaGlob = &aaGlobArray[input->localClientNum];
    if ( aaGlob->autoMeleeState == AMS_TARGETING )
    {
        adjustedPitchAxis = 0.0f;
        adjustedYawAxis = 0.0f;
        slowdownPitchScale = 1.0f;
        slowdownYawScale = 1.0f;
    }
    else
    {
        AimAssist_CalcAdjustedAxis(input, &adjustedPitchAxis, &adjustedYawAxis);
        AimAssist_CalcSlowdown(input, &slowdownPitchScale, &slowdownYawScale);
    }
    sensitivity = AimAssist_GetProfileSensitivity();
    pitchTurnRate = AimAssist_LerpDvars(aim_turnrate_pitch, aim_turnrate_pitch_ads, aaGlob->adsLerp);
    pitchTurnRate = (float)((float)(slowdownPitchScale * aaGlob->fovTurnRateScale) * sensitivity) * pitchTurnRate;
    yawTurnRate = AimAssist_LerpDvars(aim_turnrate_yaw, aim_turnrate_yaw_ads, aaGlob->adsLerp);
    yawTurnRate = (float)((float)(slowdownYawScale * aaGlob->fovTurnRateScale) * sensitivity) * yawTurnRate;
    if ( input->pitchMax != 0.0 && pitchTurnRate > input->pitchMax )
        pitchTurnRate = input->pitchMax;
    if ( input->yawMax != 0.0 && yawTurnRate > input->yawMax )
        yawTurnRate = input->yawMax;
    pitchSign = adjustedPitchAxis >= 0.0f ? 1.0f : -1.0f;
    yawSign = adjustedYawAxis >= 0.0f ? 1.0f : -1.0f;
    pitchDelta = fabs(adjustedPitchAxis) * pitchTurnRate;
    yawDelta = fabs(adjustedYawAxis) * yawTurnRate;
    if ( !aim_accel_turnrate_enabled->current.enabled || aim_assist_script_disable->current.enabled )
    {
        aaGlob->pitchDelta = pitchDelta;
        aaGlob->yawDelta = yawDelta;
    }
    else
    {
        turnRateTrackSpeed = aim_accel_turnrate_lerp->current.value * sensitivity;
        if ( pitchDelta <= aaGlob->pitchDelta )
        {
            aaGlob->pitchDelta = pitchDelta;
        }
        else
        {
            aaGlob->pitchDelta = LinearTrack(pitchDelta, aaGlob->pitchDelta, turnRateTrackSpeed, input->deltaTime);
        }
        if ( yawDelta <= aaGlob->yawDelta )
        {
            aaGlob->yawDelta = yawDelta;
        }
        else
        {
            aaGlob->yawDelta = LinearTrack(yawDelta, aaGlob->yawDelta, turnRateTrackSpeed, input->deltaTime);
        }
        if ( aim_accel_turnrate_debug->current.enabled )
        {
            Com_Printf(17, "Target Pit = %f, Current Pit = %f\n", pitchDelta, aaGlob->pitchDelta);
            Com_Printf(17, "Target Yaw = %f, Current Yaw = %f\n", yawDelta, aaGlob->yawDelta);
        }
    }
    output->pitch = (float)((float)(aaGlob->pitchDelta * input->deltaTime) * pitchSign) + output->pitch;
    output->yaw = (float)((float)(aaGlob->yawDelta * input->deltaTime) * yawSign) + output->yaw;
}

double __cdecl AimAssist_LerpDvars(const dvar_s *from, const dvar_s *to, float frac)
{
    if ( !from && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 277, 0, "%s", "from") )
        __debugbreak();
    if ( !to && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 278, 0, "%s", "to") )
        __debugbreak();
    if ( (frac < 0.0 || frac > 1.0)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp",
                    279,
                    0,
                    "%s",
                    "(frac >= 0.0f) && (frac <= 1.0f)") )
    {
        __debugbreak();
    }
    return (to->current.value - from->current.value) * frac + from->current.value;
}

// Decomp: CoDMPServer.c:114461
void __cdecl AimAssist_CalcAdjustedAxis(const AimInput *input, float *pitchAxis, float *yawAxis)
{
    float stickDeflection;
    float deflection;
    float absPitchAxis;
    float graphScale;
    float absYawAxis;

    if ( !input
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1206, 0, "%s", "input") )
    {
        __debugbreak();
    }
    if ( !pitchAxis
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1207, 0, "%s", "pitchAxis") )
    {
        __debugbreak();
    }
    if ( !yawAxis
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1208, 0, "%s", "yawAxis") )
    {
        __debugbreak();
    }
    if ( aim_input_graph_enabled->current.enabled )
    {
        deflection = sqrtf((float)(input->pitchAxis * input->pitchAxis) + (float)(input->yawAxis * input->yawAxis));
        stickDeflection = ClampFloat(deflection, 0.0f, 1.0f);
        if ( aim_input_graph_index->current.integer >= 4u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp",
                        1215,
                        0,
                        "aim_input_graph_index->current.integer doesn't index AIM_INPUT_GRAPH_COUNT\n\t%i not in [0, %i)",
                        aim_input_graph_index->current.integer,
                        4) )
        {
            __debugbreak();
        }
        graphScale = GraphFloat_GetValue(&aaInputGraph[aim_input_graph_index->current.integer], stickDeflection);
        *pitchAxis = input->pitchAxis * graphScale;
        *yawAxis = input->yawAxis * graphScale;
        if ( aim_input_graph_debug->current.enabled )
        {
            Com_Printf(17, "Input Pit = %f, Output Pit = %f\n", input->pitchAxis, *pitchAxis);
            Com_Printf(17, "Input Yaw = %f, Output Yaw = %f\n", input->yawAxis, *yawAxis);
        }
    }
    else
    {
        *pitchAxis = input->pitchAxis;
        *yawAxis = input->yawAxis;
    }
    if ( aim_scale_view_axis->current.enabled )
    {
        absPitchAxis = fabs(*pitchAxis);
        absYawAxis = fabs(*yawAxis);
        if ( absPitchAxis > 1.0
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp",
                        1239,
                        0,
                        "%s",
                        "absPitchAxis <= 1.0f") )
        {
            __debugbreak();
        }
        if ( absYawAxis > 1.0
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp",
                        1240,
                        0,
                        "%s",
                        "absYawAxis <= 1.0f") )
        {
            __debugbreak();
        }
        if ( absPitchAxis <= absYawAxis )
            *pitchAxis = (float)(1.0 - (float)(absYawAxis - absPitchAxis)) * *pitchAxis;
        else
            *yawAxis = (float)(1.0 - (float)(absPitchAxis - absYawAxis)) * *yawAxis;
    }
}

void __cdecl AimAssist_CalcSlowdown(const AimInput *input, float *pitchScale, float *yawScale)
{
    const AimScreenTarget *screenTarget;
    float aimAssistRange;
    AimAssistGlobals *aaGlob;

    if ( !input
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1334, 0, "%s", "input") )
    {
        __debugbreak();
    }
    if ( !pitchScale
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1335, 0, "%s", "pitchScale") )
    {
        __debugbreak();
    }
    if ( !yawScale
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1336, 0, "%s", "yawScale") )
    {
        __debugbreak();
    }
    aaGlob = &aaGlobArray[input->localClientNum];
    *pitchScale = 1.0f;
    *yawScale = 1.0f;
    aaGlob->aimSlowdownActive = 0;
    aaGlob->aimSlowdownTargetEnt = 1023;
    if ( AimAssist_IsSlowdownActive(&aaGlob->ps) && (aaGlob->ps.weaponstate < 47 || aaGlob->ps.weaponstate > 49) )
    {
        if ( !aaGlob->ps.weapIndex
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp",
                        1350,
                        0,
                        "%s",
                        "aaGlob->ps.weapIndex != WP_NONE") )
        {
            __debugbreak();
        }
        aimAssistRange = AimAssist_GetAimAssistRange(aaGlob->ps.weapIndex, aaGlob->adsLerp);
        screenTarget = AimAssist_GetBestTarget(
                                         aaGlob,
                                         aimAssistRange,
                                         aaGlob->tweakables.slowdownRegionWidth,
                                         aaGlob->tweakables.slowdownRegionHeight);
        if ( screenTarget )
        {
            *pitchScale = AimAssist_LerpDvars(aim_slowdown_pitch_scale, aim_slowdown_pitch_scale_ads, aaGlob->adsLerp);
            *yawScale = AimAssist_LerpDvars(aim_slowdown_yaw_scale, aim_slowdown_yaw_scale_ads, aaGlob->adsLerp);
            aaGlob->aimSlowdownActive = 1;
            aaGlob->aimSlowdownTargetEnt = screenTarget->entIndex;
        }
        if ( AimAssist_IsPlayerUsingOffhand(&aaGlob->ps) )
            *pitchScale = 1.0f;
        if ( aim_slowdown_debug->current.enabled )
        {
            Com_Printf(17, "Target slowdown pitch scale = %f\n", *pitchScale);
            Com_Printf(17, "Target slowdown yaw scale = %f\n", *yawScale);
        }
    }
}

double __cdecl AimAssist_GetAimAssistRange(unsigned int weapIndex, float adsLerp)
{
    const WeaponVariantDef *weapVariantDef;

    if ( !weapIndex
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp",
                    827,
                    0,
                    "%s",
                    "weapIndex != WP_NONE") )
    {
        __debugbreak();
    }
    if ( (adsLerp < 0.0 || adsLerp > 1.0)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp",
                    828,
                    0,
                    "adsLerp not in [0.0f, 1.0f]\n\t%g not in [%g, %g]",
                    adsLerp,
                    0.0,
                    1.0) )
    {
        __debugbreak();
    }
    weapVariantDef = BG_GetWeaponVariantDef(weapIndex);
    return (float)((float)((float)((float)(1.0 - adsLerp) * weapVariantDef->weapDef->aimAssistRange)
                                             + (float)(weapVariantDef->fAimAssistRangeAds * adsLerp))
                             * aim_aimAssistRangeScale->current.value);
}

static bool __cdecl AimAssist_DoBoundsIntersectCenterBox(
    const float *clipMins,
    const float *clipMaxs,
    float clipHalfWidth,
    float clipHalfHeight)
{
    iassert(clipMins);
    iassert(clipMaxs);

    return (clipHalfWidth >= (double)clipMins[0] && clipMaxs[0] >= -clipHalfWidth)
        && (clipHalfHeight >= (double)clipMins[1] && clipMaxs[1] >= -clipHalfHeight);
}

const AimScreenTarget *__cdecl AimAssist_GetBestTarget(
                const AimAssistGlobals *aaGlob,
                float range,
                float regionWidth,
                float regionHeight)
{
    int targetIndex;

    if ( !aaGlob
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1153, 0, "%s", "aaGlob") )
    {
        __debugbreak();
    }
    if ( range < 0.0
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp",
                    1154,
                    0,
                    "%s\n\t(range) = %g",
                    "((range >= 0.0f))",
                    range) )
    {
        __debugbreak();
    }
    for ( targetIndex = 0; targetIndex < aaGlob->screenTargetCount; ++targetIndex )
    {
        if ( aaGlob->screenTargets[targetIndex].distSqr <= (float)(range * range)
            && AimAssist_DoBoundsIntersectCenterBox(
                     aaGlob->screenTargets[targetIndex].clipMins,
                     aaGlob->screenTargets[targetIndex].clipMaxs,
                     regionWidth,
                     regionHeight) )
        {
            return &aaGlob->screenTargets[targetIndex];
        }
    }
    return 0;
}

bool __cdecl AimAssist_IsPlayerUsingOffhand(const AimAssistPlayerState *ps)
{
    if ( !ps && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1253, 0, "%s", "ps") )
        __debugbreak();
    if ( (ps->weapFlags & 2) == 0 )
        return 0;
    if ( ps->weapIndex )
        return BG_GetWeaponDef(ps->weapIndex)->impactType != IMPACT_TYPE_BLADE;
    return 0;
}

bool __cdecl AimAssist_IsSlowdownActive(const AimAssistPlayerState *ps)
{
    if ( !ps && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1276, 0, "%s", "ps") )
        __debugbreak();
    if ( aim_assist_script_disable->current.enabled )
        return 0;
    if ( !aim_slowdown_enabled->current.enabled )
        return 0;
    if ( ps->targetAssistDisabled )
        return 0;
    if ( !ps->weapIndex )
        return 0;
    if ( BG_GetWeaponDef(ps->weapIndex)->requireLockonToFire )
        return 0;
    if ( BG_GetWeaponDef(ps->weapIndex)->guidedMissileType == MISSILE_GUIDANCE_TVGUIDED )
        return 0;
    if ( (ps->pm_flags & 0x10000) != 0 )
        return 0;
    if ( (ps->eFlags & 0x300) != 0 )
        return 1;
    return ps->hasAmmo;
}

double __cdecl AimAssist_GetProfileSensitivity()
{
    float sensitivity;

    if ( aim_view_sensitivity_override->current.value > 0.0 )
        sensitivity = aim_view_sensitivity_override->current.value;
    else
        sensitivity = Dvar_GetFloat("input_viewSensitivity");
    if ( sensitivity < 0.001 )
        return 1.0f;
    return sensitivity;
}

void __cdecl AimAssist_ApplyAutoAim(const AimInput *input, AimOutput *output)
{
    const AimScreenTarget *screenTarget;
    float yawDelta;
    float newPitch;
    bool autoAimEnabled;
    float pitchDelta;
    AimAssistGlobals *aaGlob;
    float newYaw;
    float autoAimRange;

    if ( !input
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1646, 0, "%s", "input") )
    {
        __debugbreak();
    }
    if ( !output
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1647, 0, "%s", "output") )
    {
        __debugbreak();
    }
    aaGlob = &aaGlobArray[input->localClientNum];
    autoAimEnabled = AimAssist_PlayerDisabledAutoAim() == 0;
    BG_GetWeaponDef(aaGlob->ps.weapIndex);
    if ( aim_autoaim_enabled->current.enabled
        && !aim_assist_script_disable->current.enabled
        && input->button_bits.testBit(11)
        && autoAimEnabled
        && aaGlob->ps.weapIndex )
    {
        if ( !aaGlob->autoAimPressed && input->button_bits.testBit(11) )
        {
            if ( aaGlob->ps.weaponstate != 10
                && aaGlob->ps.weaponstate != 12
                && aaGlob->ps.weaponstate != 14
                && aaGlob->ps.weaponstate != 13
                && aaGlob->ps.weaponstate != 11
                && aaGlob->ps.weaponstate != 15
                && aaGlob->ps.weaponstate != 16 )
            {
                if ( BG_IsAimDownSightWeapon(aaGlob->ps.weapIndex) )
                {
                    autoAimRange = AimAssist_GetAutoAimRange(aaGlob->ps.weapIndex);
                    screenTarget = AimAssist_GetBestTarget(
                                                     aaGlob,
                                                     autoAimRange,
                                                     aaGlob->tweakables.autoAimRegionWidth,
                                                     aaGlob->tweakables.autoAimRegionHeight);
                    if ( screenTarget )
                    {
                        AimAssist_SetAutoAimTarget(aaGlob, screenTarget);
                        aaGlob->autoAimJustGotTarget = 1;
                    }
                }
            }
            aaGlob->autoAimPressed = 1;
        }
        if ( aaGlob->autoAimActive )
        {
            if ( (aaGlob->ps.eFlags & 0x300) == 0
                && aaGlob->adsLerp < 1.0
                && !AimAssist_UpdateAutoAimTarget(aaGlob)
                && !aaGlob->autoAimJustGotTarget )
            {
                goto LABEL_37;
            }
            newPitch = DiffTrackAngle(
                                     aaGlob->autoAimPitchTarget,
                                     aaGlob->autoAimPitch,
                                     aim_autoaim_lerp->current.value,
                                     input->deltaTime);
            newYaw = aaGlob->overrideSnapWidthAndLerp
                         ? DiffTrackAngle(
                                 aaGlob->autoAimYawTarget,
                                 aaGlob->autoAimYaw,
                                 aaGlob->overrideAutoaimLerpValue,
                                 input->deltaTime)
                         : DiffTrackAngle(
                                 aaGlob->autoAimYawTarget,
                                 aaGlob->autoAimYaw,
                                 aim_autoaim_lerp->current.value,
                                 input->deltaTime);
            pitchDelta = AngleNormalize180(newPitch - aaGlob->autoAimPitch);
            yawDelta = AngleNormalize180(newYaw - aaGlob->autoAimYaw);
            aaGlob->autoAimPitch = newPitch;
            aaGlob->autoAimYaw = newYaw;
            output->pitch = output->pitch + pitchDelta;
            output->yaw = output->yaw + yawDelta;
            if ( fabs(pitchDelta) < 0.001f
                && fabs(yawDelta) <= 0.001f
                && aaGlob->ps.fWeaponPosFrac == 0.0 )
            {
LABEL_37:
                AimAssist_ClearAutoAimTarget(aaGlob);
            }
        }
    }
    else
    {
        AimAssist_ClearAutoAimTarget(aaGlob);
        aaGlob->autoAimPressed = 0;
    }
}

double __cdecl AimAssist_GetAutoAimRange(unsigned int weapIndex)
{
    if ( !weapIndex
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp",
                    845,
                    0,
                    "%s",
                    "weapIndex != WP_NONE") )
    {
        __debugbreak();
    }
    return (float)(BG_GetWeaponDef(weapIndex)->autoAimRange * aim_autoAimRangeScale->current.value);
}

void __cdecl AimAssist_ClearAutoAimTarget(AimAssistGlobals *aaGlob)
{
    if ( !aaGlob
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1531, 0, "%s", "aaGlob") )
    {
        __debugbreak();
    }
    aaGlob->autoAimTargetEnt = 1023;
    aaGlob->autoAimActive = 0;
    aaGlob->autoAimPitch = 0.0f;
    aaGlob->autoAimPitchTarget = 0.0f;
    aaGlob->autoAimYaw = 0.0f;
    aaGlob->autoAimYawTarget = 0.0f;
    aaGlob->autoAimJustGotTarget = 0;
    aaGlob->autoAimHasRealTarget = 0;
}

char __cdecl AimAssist_UpdateAutoAimTarget(AimAssistGlobals *aaGlob)
{
    float targetDir[3];
    const AimScreenTarget *screenTarget;
    float targetAngles[3];

    if ( !aaGlob
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1556, 0, "%s", "aaGlob") )
    {
        __debugbreak();
    }
    if ( !aaGlob->autoAimActive
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp",
                    1557,
                    0,
                    "%s",
                    "aaGlob->autoAimActive") )
    {
        __debugbreak();
    }
    screenTarget = AimAssist_GetTargetFromEntity(aaGlob, aaGlob->autoAimTargetEnt);
    if ( !screenTarget )
        return 0;
    targetDir[0] = screenTarget->aimPos[0] - aaGlob->viewOrigin[0];
    targetDir[1] = screenTarget->aimPos[1] - aaGlob->viewOrigin[1];
    targetDir[2] = screenTarget->aimPos[2] - aaGlob->viewOrigin[2];
    vectoangles(targetDir, targetAngles);
    if ( aaGlob->overrideSnapWidthAndLerp )
    {
        aaGlob->autoAimPitch = aaGlob->viewAngles[0];
        aaGlob->autoAimYaw = aaGlob->viewAngles[1];
    }
    aaGlob->autoAimPitchTarget = targetAngles[0];
    aaGlob->autoAimYawTarget = targetAngles[1];
    return 1;
}

const AimScreenTarget *__cdecl AimAssist_GetTargetFromEntity(const AimAssistGlobals *aaGlob, int entIndex)
{
    int targetIndex;

    if ( !aaGlob
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1128, 0, "%s", "aaGlob") )
    {
        __debugbreak();
    }
    if ( entIndex == 1023 )
        return 0;
    for ( targetIndex = 0; targetIndex < aaGlob->screenTargetCount; ++targetIndex )
    {
        if ( aaGlob->screenTargets[targetIndex].entIndex == entIndex )
            return &aaGlob->screenTargets[targetIndex];
    }
    return 0;
}

void __cdecl AimAssist_SetAutoAimTarget(AimAssistGlobals *aaGlob, const AimScreenTarget *screenTarget)
{
    if ( !aaGlob
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1584, 0, "%s", "aaGlob") )
    {
        __debugbreak();
    }
    if ( !screenTarget
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1585, 0, "%s", "screenTarget") )
    {
        __debugbreak();
    }
    AimAssist_ClearAutoAimTarget(aaGlob);
    aaGlob->autoAimTargetEnt = screenTarget->entIndex;
    aaGlob->autoAimActive = 1;
    aaGlob->autoAimHasRealTarget = 1;
    aaGlob->autoAimPitch = aaGlob->viewAngles[0];
    aaGlob->autoAimYaw = aaGlob->viewAngles[1];
    AimAssist_UpdateAutoAimTarget(aaGlob);
}

void __cdecl AimAssist_ApplyLockOn(const AimInput *input, AimOutput *output)
{
    float arcLength;
    const AimScreenTarget *screenTarget;
    float pitchTurnRate;
    int prevTargetEnt;
    float aimAssistRange;
    AimAssistGlobals *aaGlob;

    if ( !input
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1739, 0, "%s", "input") )
    {
        __debugbreak();
    }
    if ( !output
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1740, 0, "%s", "output") )
    {
        __debugbreak();
    }
    aaGlob = &aaGlobArray[input->localClientNum];
    prevTargetEnt = aaGlob->lockOnTargetEnt;
    aaGlob->lockOnTargetEnt = 1023;
    if ( aim_lockon_enabled->current.enabled
        && !AimAssist_IsPlayerUsingOffhand(&aaGlob->ps)
        && !aim_assist_script_disable->current.enabled
        && !aaGlob->ps.targetAssistDisabled
        && !aaGlob->autoAimActive
        && aaGlob->autoMeleeState != AMS_TARGETING
        && BG_GetWeaponDef(aaGlob->ps.weapIndex)->guidedMissileType != MISSILE_GUIDANCE_TVGUIDED
        && !BG_GetWeaponDef(aaGlob->ps.weapIndex)->requireLockonToFire
        && (aim_lockon_deflection->current.value <= fabs(input->pitchAxis)
         || aim_lockon_deflection->current.value <= fabs(input->yawAxis)
         || aim_lockon_deflection->current.value <= fabs(input->rightAxis))
        && !CG_GetLocalClientGlobals(input->localClientNum)->cameraMode )
    {
        if ( aaGlob->ps.weapIndex )
        {
            aimAssistRange = AimAssist_GetAimAssistRange(aaGlob->ps.weapIndex, aaGlob->adsLerp);
            screenTarget = AimAssist_GetPrevOrBestTarget(
                                             aaGlob,
                                             aimAssistRange,
                                             aaGlob->tweakables.lockOnRegionWidth,
                                             aaGlob->tweakables.lockOnRegionHeight,
                                             prevTargetEnt);
        }
        else
        {
            screenTarget = 0;
        }
        if ( screenTarget && screenTarget->distSqr > 0.0 )
        {
            aaGlob->lockOnTargetEnt = screenTarget->entIndex;
            arcLength = sqrtf(screenTarget->distSqr) * 3.1415927;
            if ( arcLength <= 0.0
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp",
                            1789,
                            0,
                            "%s",
                            "arcLength > 0.0f") )
            {
                __debugbreak();
            }
            pitchTurnRate = (float)((float)((float)((float)((float)((float)(screenTarget->velocity[0] * aaGlob->viewAxis[2][0])
                                                                                                                        + (float)(screenTarget->velocity[1] * aaGlob->viewAxis[2][1]))
                                                                                                        + (float)(screenTarget->velocity[2] * aaGlob->viewAxis[2][2]))
                                                                                        - (float)((float)((float)(aaGlob->ps.velocity[0] * aaGlob->viewAxis[2][0])
                                                                                                                        + (float)(aaGlob->ps.velocity[1] * aaGlob->viewAxis[2][1]))
                                                                                                        + (float)(aaGlob->ps.velocity[2] * aaGlob->viewAxis[2][2])))
                                                                        / arcLength)
                                                        * 180.0)
                                        * aim_lockon_pitch_strength->current.value;
            output->yaw = (float)((float)((float)((float)((float)((float)((float)((float)(screenTarget->velocity[0]
                                                                                                                                                                    * aaGlob->viewAxis[1][0])
                                                                                                                                                    + (float)(screenTarget->velocity[1]
                                                                                                                                                                    * aaGlob->viewAxis[1][1]))
                                                                                                                                    + (float)(screenTarget->velocity[2]
                                                                                                                                                    * aaGlob->viewAxis[1][2]))
                                                                                                                    - (float)((float)((float)(aaGlob->ps.velocity[0]
                                                                                                                                                                    * aaGlob->viewAxis[1][0])
                                                                                                                                                    + (float)(aaGlob->ps.velocity[1]
                                                                                                                                                                    * aaGlob->viewAxis[1][1]))
                                                                                                                                    + (float)(aaGlob->ps.velocity[2]
                                                                                                                                                    * aaGlob->viewAxis[1][2])))
                                                                                                    / arcLength)
                                                                                    * 180.0)
                                                                    * aim_lockon_strength->current.value)
                                                    * input->deltaTime)
                                    + output->yaw;
            output->pitch = output->pitch - (float)(pitchTurnRate * input->deltaTime);
        }
    }
}

const AimScreenTarget *__cdecl AimAssist_GetPrevOrBestTarget(
                const AimAssistGlobals *aaGlob,
                float range,
                float regionWidth,
                float regionHeight,
                int prevTargetEnt)
{
    const AimScreenTarget *screenTarget;

    if ( !aaGlob
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1181, 0, "%s", "aaGlob") )
    {
        __debugbreak();
    }
    if ( range < 0.0
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp",
                    1182,
                    0,
                    "%s\n\t(range) = %g",
                    "((range >= 0.0f))",
                    range) )
    {
        __debugbreak();
    }
    screenTarget = AimAssist_GetTargetFromEntity(aaGlob, prevTargetEnt);
    if ( screenTarget
        && (float)(range * range) > screenTarget->distSqr
        && AimAssist_DoBoundsIntersectCenterBox(screenTarget->clipMins, screenTarget->clipMaxs, regionWidth, regionHeight) )
    {
        return screenTarget;
    }
    else
    {
        return AimAssist_GetBestTarget(aaGlob, range, regionWidth, regionHeight);
    }
}

static const float kMeleeChargeDistOffset = 32.0f;

void __cdecl AimAssist_ApplyAutoMelee(const AimInput *input, AimOutput *output)
{
    float targetDir[2];
    float meleeChargeDistance;
    const AimScreenTarget *screenTarget;
    float meleeAcquireRange;
    bool meleeButtonPressed;
    float yawDelta;
    float newPitch;
    float pitchDelta;
    bool isBayonet;
    AimAssistGlobals *aaGlob;
    float newYaw;
    const AimTweakables *tweaks;

    if ( !input
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1982, 0, "%s", "input") )
    {
        __debugbreak();
    }
    if ( !output
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1983, 0, "%s", "output") )
    {
        __debugbreak();
    }
    output->meleeChargeYaw = 0.0f;
    output->meleeChargeDist = 0;
    aaGlob = &aaGlobArray[input->localClientNum];
    tweaks = &aaGlob->tweakables;
    isBayonet = BG_IsBayonetWeapon(aaGlob->ps.weapIndex);
    if ( !aim_automelee_enabled->current.enabled || !aaGlob->ps.weapIndex )
    {
        AimAssist_ClearAutoMeleeTarget(aaGlob);
        return;
    }
    meleeButtonPressed = input->button_bits.testBit(2) && !aaGlob->prev_button_bits.testBit(2);
    if ( BG_GetWeaponDef(aaGlob->ps.weapIndex)->meleeChargeRange == 0.0 )
    {
        if ( isBayonet )
            meleeAcquireRange = aim_autobayonet_range->current.value;
        else
            meleeAcquireRange = aim_automelee_range->current.value;
    }
    else
    {
        meleeAcquireRange = BG_GetWeaponDef(aaGlob->ps.weapIndex)->meleeChargeRange;
    }
    if ( aaGlob->autoMeleeState != AMS_TARGETING && meleeButtonPressed )
    {
        screenTarget = AimAssist_GetTargetFromEntity(aaGlob, aaGlob->autoMeleeTargetEnt);
        if ( !screenTarget )
        {
            screenTarget = AimAssist_GetBestTarget(
                aaGlob,
                meleeAcquireRange,
                tweaks->autoMeleeRegionWidth,
                tweaks->autoMeleeRegionHeight);
            if ( !screenTarget )
            {
                AimAssist_ClearAutoMeleeTarget(aaGlob);
                return;
            }
        }
        AimAssist_SetAutoMeleeTarget(aaGlob, screenTarget);
        if ( (aaGlob->ps.pm_flags & 1) == 0 && screenTarget->distSqr > 0.0 )
        {
            meleeChargeDistance = sqrtf(screenTarget->distSqr) - kMeleeChargeDistOffset;
            if ( meleeChargeDistance > 0.0f )
            {
                if ( meleeChargeDistance > 255.0f )
                    meleeChargeDistance = 255.0f;
                targetDir[0] = screenTarget->aimPos[0] - aaGlob->viewOrigin[0];
                targetDir[1] = screenTarget->aimPos[1] - aaGlob->viewOrigin[1];
                output->meleeChargeYaw = vectoyaw(targetDir);
                output->meleeChargeDist = (int)meleeChargeDistance;
            }
        }
    }
    if ( aaGlob->autoMeleeState == AMS_TARGET_AQUIRED && aaGlob->ps.weaponstate == 17 )
        aaGlob->autoMeleeState = AMS_TARGETING;
    if ( aaGlob->autoMeleeState == AMS_TARGETING )
    {
        if ( aaGlob->ps.weaponstate != 18
            && (aaGlob->ps.weaponstate == 17 || aaGlob->ps.weaponstate == 18 || aaGlob->ps.weaponstate == 19)
            && AimAssist_UpdateAutoMeleeTarget(aaGlob, input->localClientNum) )
        {
            newPitch = DiffTrackAngle(
                                     aaGlob->autoMeleePitchTarget,
                                     aaGlob->autoMeleePitch,
                                     aim_automelee_lerp->current.value,
                                     input->deltaTime);
            newYaw = DiffTrackAngle(
                                 aaGlob->autoMeleeYawTarget,
                                 aaGlob->autoMeleeYaw,
                                 aim_automelee_lerp->current.value,
                                 input->deltaTime);
            pitchDelta = AngleNormalize180(newPitch - aaGlob->autoMeleePitch);
            yawDelta = AngleNormalize180(newYaw - aaGlob->autoMeleeYaw);
            aaGlob->autoMeleePitch = newPitch;
            aaGlob->autoMeleeYaw = newYaw;
            output->pitch = output->pitch + pitchDelta;
            output->yaw = output->yaw + yawDelta;
        }
        else
        {
            AimAssist_ClearAutoMeleeTarget(aaGlob);
        }
    }
}

void __cdecl AimAssist_ClearAutoMeleeTarget(AimAssistGlobals *aaGlob)
{
    if ( !aaGlob
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1813, 0, "%s", "aaGlob") )
    {
        __debugbreak();
    }
    aaGlob->autoMeleeState = AMS_NOT_ACTIVE;
    aaGlob->autoMeleeTargetEnt = 1023;
    aaGlob->autoMeleePitch = 0.0f;
    aaGlob->autoMeleePitchTarget = 0.0f;
    aaGlob->autoMeleeYaw = 0.0f;
    aaGlob->autoMeleeYawTarget = 0.0f;
}

char __cdecl AimAssist_UpdateAutoMeleeTarget(AimAssistGlobals *aaGlob, int localClientNum)
{
    centity_s *cent;
    float targetDir[3];
    const AimScreenTarget *screenTarget;
    float aimPos[3];
    float targetAngles[3];

    if ( !aaGlob
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1846, 0, "%s", "aaGlob") )
    {
        __debugbreak();
    }
    if ( aaGlob->autoMeleeState == AMS_NOT_ACTIVE
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp",
                    1847,
                    0,
                    "%s",
                    "aaGlob->autoMeleeState != AMS_NOT_ACTIVE") )
    {
        __debugbreak();
    }
    screenTarget = AimAssist_GetTargetFromEntity(aaGlob, aaGlob->autoMeleeTargetEnt);
    if ( !screenTarget && aaGlob->autoMeleeTargetEnt == 1023 )
        return 0;
    if ( screenTarget )
    {
        aimPos[0] = screenTarget->aimPos[0];
        aimPos[1] = screenTarget->aimPos[1];
        aimPos[2] = screenTarget->aimPos[2];
    }
    else
    {
        cent = CG_GetEntity(localClientNum, aaGlob->autoMeleeTargetEnt);
        if ( ((*((unsigned int *)cent + 201) >> 1) & 1) == 0 )
            return 0;
        if ( !AimAssist_CalcAimPos(localClientNum, cent, 0, aimPos) )
            return 0;
    }
    targetDir[0] = aimPos[0] - aaGlob->viewOrigin[0];
    targetDir[1] = aimPos[1] - aaGlob->viewOrigin[1];
    targetDir[2] = aimPos[2] - aaGlob->viewOrigin[2];
    vectoangles(targetDir, targetAngles);
    aaGlob->autoMeleePitchTarget = targetAngles[0];
    aaGlob->autoMeleeYawTarget = targetAngles[1];
    return 1;
}

void __cdecl AimAssist_SetAutoMeleeTarget(AimAssistGlobals *aaGlob, const AimScreenTarget *screenTarget)
{
    if ( !aaGlob
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1884, 0, "%s", "aaGlob") )
    {
        __debugbreak();
    }
    if ( !screenTarget
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 1885, 0, "%s", "screenTarget") )
    {
        __debugbreak();
    }
    aaGlob->autoMeleeTargetEnt = screenTarget->entIndex;
    aaGlob->autoMeleeState = AMS_TARGET_AQUIRED;
    aaGlob->autoMeleePitch = aaGlob->viewAngles[0];
    aaGlob->autoMeleePitchTarget = 0.0f;
    aaGlob->autoMeleeYaw = aaGlob->viewAngles[1];
    aaGlob->autoMeleeYawTarget = 0.0f;
}

void __cdecl AimAssist_UpdateMouseInput(const AimInput *input, AimOutput *output)
{
    int buttonBitsWordIndex;
    AimAssistGlobals *aaGlob;

    if ( !input
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 2378, 0, "%s", "input") )
    {
        __debugbreak();
    }
    if ( !output
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 2379, 0, "%s", "output") )
    {
        __debugbreak();
    }
    output->pitch = input->pitch;
    output->yaw = input->yaw;
    aaGlob = &aaGlobArray[input->localClientNum];
    if ( aaGlob->initialized )
    {
        AimAssist_UpdateTweakables(input->localClientNum);
        AimAssist_UpdateAdsLerp(input);
        AimAssist_ApplyAutoMelee(input, output);
        for ( buttonBitsWordIndex = 0; buttonBitsWordIndex < 2; ++buttonBitsWordIndex )
            aaGlob->prev_button_bits.array[buttonBitsWordIndex] = input->button_bits.array[buttonBitsWordIndex];
    }
}

// Decomp: CoDMPServer.c:115594
void __cdecl AimAssist_DrawDebugOverlay(int localClientNum)
{
    float green[4];
    float red[4];
    const AimAssistGlobals *aaGlob;
    const playerState_s *ps;
    const AimTweakables *tweaks;

    red[0] = 1.0f;
    red[1] = 0.0f;
    red[2] = 0.0f;
    red[3] = 0.25f;
    green[0] = 0.0f;
    green[1] = 1.0f;
    green[2] = 0.0f;
    green[3] = 0.25f;
    aaGlob = &aaGlobArray[localClientNum];
    if ( aaGlob->initialized )
    {
        tweaks = &aaGlob->tweakables;
        ps = &CG_GetLocalClientGlobals(localClientNum)->predictedPlayerState;
        if ( aim_slowdown_debug->current.enabled )
        {
            AimAssist_DrawTargets(localClientNum, ps, red);
            AimAssist_DrawCenterBox(aaGlob, tweaks->slowdownRegionWidth, tweaks->slowdownRegionHeight, green);
        }
        if ( aim_autoaim_debug->current.enabled )
        {
            AimAssist_DrawTargets(localClientNum, ps, red);
            AimAssist_DrawCenterBox(aaGlob, tweaks->autoAimRegionWidth, tweaks->autoAimRegionHeight, green);
        }
        if ( aim_automelee_debug->current.enabled )
        {
            AimAssist_DrawTargets(localClientNum, ps, red);
            AimAssist_DrawCenterBox(aaGlob, tweaks->autoMeleeRegionWidth, tweaks->autoMeleeRegionHeight, green);
        }
        if ( aim_lockon_debug->current.enabled )
        {
            AimAssist_DrawTargets(localClientNum, ps, red);
            AimAssist_DrawCenterBox(aaGlob, tweaks->lockOnRegionWidth, tweaks->lockOnRegionHeight, green);
        }
    }
}

void __cdecl AimAssist_DrawCenterBox(
                const AimAssistGlobals *aaGlob,
                float clipHalfWidth,
                float clipHalfHeight,
                const float *color)
{
    if ( !aaGlob
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 2420, 0, "%s", "aaGlob") )
    {
        __debugbreak();
    }
    if ( !color
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 2421, 0, "%s", "color") )
    {
        __debugbreak();
    }
    CL_DrawStretchPicPhysical(
        (float)((float)(aaGlob->screenWidth - (float)(clipHalfWidth * aaGlob->screenWidth)) * 0.5) + 0.0,
        (float)((float)(aaGlob->screenHeight - (float)(clipHalfHeight * aaGlob->screenHeight)) * 0.5) + 0.0,
        clipHalfWidth * aaGlob->screenWidth,
        clipHalfHeight * aaGlob->screenHeight,
        0.0,
        0.0,
        1.0,
        1.0,
        color,
        cgMedia.whiteMaterial);
}

// Decomp: CoDMPServer.c:115685
void __cdecl AimAssist_DrawTargets(int localClientNum, const playerState_s *ps, const float *color)
{
    float meleeDebugRange;
    float aimAssistRange;
    int targetIndex;
    const AimAssistGlobals *aaGlob;
    float targetBoxScreenLeft;
    float labelY;
    float autoAimRange;

    if ( !ps && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 2458, 0, "%s", "ps") )
        __debugbreak();
    if ( !color
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_assist.cpp", 2459, 0, "%s", "color") )
    {
        __debugbreak();
    }
    aaGlob = &aaGlobArray[localClientNum];
    if ( aaGlob->ps.weapIndex )
    {
        if ( BG_IsBayonetWeapon(aaGlob->ps.weapIndex) )
            meleeDebugRange = aim_autobayonet_range->current.value;
        else
            meleeDebugRange = aim_automelee_range->current.value;
        aimAssistRange = AimAssist_GetAimAssistRange(aaGlob->ps.weapIndex, aaGlob->adsLerp);
        autoAimRange = AimAssist_GetAutoAimRange(aaGlob->ps.weapIndex);
        for ( targetIndex = 0; targetIndex < aaGlob->screenTargetCount; ++targetIndex )
        {
            if ( (!aim_autoaim_debug->current.enabled
                 || aaGlob->screenTargets[targetIndex].distSqr <= (float)(autoAimRange * autoAimRange))
                && (!aim_automelee_debug->current.enabled
                 || aaGlob->screenTargets[targetIndex].distSqr <= (float)(meleeDebugRange * meleeDebugRange))
                && (!aim_slowdown_debug->current.enabled
                 || aaGlob->screenTargets[targetIndex].distSqr <= (float)(aimAssistRange * aimAssistRange))
                && (!aim_lockon_debug->current.enabled
                 || aaGlob->screenTargets[targetIndex].distSqr <= (float)(aimAssistRange * aimAssistRange)) )
            {
                targetBoxScreenLeft = (aaGlob->screenTargets[targetIndex].clipMins[0] + 1.0f) * (aaGlob->screenWidth * 0.5f);
                labelY = (aaGlob->screenTargets[targetIndex].clipMins[1] + 1.0f) * (aaGlob->screenHeight * 0.5f);
                CL_DrawStretchPicPhysical(
                    targetBoxScreenLeft,
                    labelY,
                    (aaGlob->screenTargets[targetIndex].clipMaxs[0] + 1.0f) * (aaGlob->screenWidth * 0.5f)
                - targetBoxScreenLeft,
                    (aaGlob->screenTargets[targetIndex].clipMaxs[1] + 1.0f) * (aaGlob->screenHeight * 0.5f)
                - labelY,
                    0.0,
                    0.0,
                    1.0,
                    1.0,
                    color,
                    cgMedia.whiteMaterial);
                CL_DrawText(
                    &scrPlaceView[localClientNum],
                    va("Pri: %i", targetIndex),
                    0x7FFFFFFF,
                    cgMedia.smallDevFont,
                    targetBoxScreenLeft,
                    labelY,
                    5,
                    5,
                    1.0,
                    1.0,
                    colorYellow,
                    0);
                labelY -= 10.0f;
                CL_DrawText(
                    &scrPlaceView[localClientNum],
                    va("Ent: %i", aaGlob->screenTargets[targetIndex].entIndex),
                    0x7FFFFFFF,
                    cgMedia.smallDevFont,
                    targetBoxScreenLeft,
                    labelY,
                    5,
                    5,
                    1.0,
                    1.0,
                    colorYellow,
                    0);
                labelY -= 10.0f;
                CL_DrawText(
                    &scrPlaceView[localClientNum],
                    va("Dist: %.2f", sqrtf(aaGlob->screenTargets[targetIndex].distSqr)),
                    0x7FFFFFFF,
                    cgMedia.smallDevFont,
                    targetBoxScreenLeft,
                    labelY,
                    5,
                    5,
                    1.0,
                    1.0,
                    colorWhite,
                    0);
                labelY -= 10.0f;
                CL_DrawText(
                    &scrPlaceView[localClientNum],
                    va("Speed: %.2f", Abs(aaGlob->screenTargets[targetIndex].velocity)),
                    0x7FFFFFFF,
                    cgMedia.smallDevFont,
                    targetBoxScreenLeft,
                    labelY,
                    5,
                    5,
                    1.0,
                    1.0,
                    colorWhite,
                    0);
                labelY -= 10.0f;
                CL_DrawText(
                    &scrPlaceView[localClientNum],
                    va("XHairDist: %.4f", sqrtf(aaGlob->screenTargets[targetIndex].crosshairDistSqr)),
                    0x7FFFFFFF,
                    cgMedia.smallDevFont,
                    targetBoxScreenLeft,
                    labelY,
                    5,
                    5,
                    1.0,
                    1.0,
                    colorWhite,
                    0);
            }
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c — SP-only top-down aim assist (no MP binary); safe no-op stubs
float *__cdecl TopDown_FindBestTarget(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, double a9, int a10, int a11, int a12, int a13, int a14)
{
    (void)a1; (void)a2; (void)a3; (void)a4; (void)a5; (void)a6; (void)a7; (void)a8; (void)a9;
    (void)a10; (void)a11; (void)a12; (void)a13; (void)a14;
    return nullptr;
}

// Decomp: CoDSP_rdBlackOps.map.c
int __cdecl TopDown_UpdateAngles(int a1, int a2, float *a3)
{
    (void)a1;
    (void)a2;
    if ( a3 )
    {
        a3[0] = 0.0f;
        a3[1] = 0.0f;
    }
    return 0;
}

// Decomp: CoDSP_rdBlackOps.map.c
const char *__cdecl TopDown_UpdateGamePadInput(int a1, float *a2)
{
    TopDown_UpdateAngles(0, a1, a2);
    return "";
}

// Decomp: CoDSP_rdBlackOps.map.c
void *__cdecl TopDown_RestoreState(void *result, float *a2)
{
    (void)a2;
    return result;
}

// Decomp: CoDSP_rdBlackOps.map.c
int __cdecl TopDown_UpdateMouseInput(int a1, float *a2)
{
    return TopDown_UpdateAngles(0, a1, a2);
}

// Decomp: CoDSP_rdBlackOps.map.c
void *__cdecl TopDown_SaveState(const void *a1, int a2)
{
    (void)a1;
    (void)a2;
    return nullptr;
}

// Decomp: CoDSP_rdBlackOps.map.c (AimAssist_SetAdsWidthAndLerp ~822B7AF0)
void __cdecl AimAssist_SetAdsWidthAndLerp(int localClientNum, float width, float lerp)
{
    AimAssistGlobals *aaGlob;

    aaGlob = &aaGlobArray[localClientNum];
    aaGlob->overrideAutoaimLerpValue = lerp;
    aaGlob->overrideAutoaimWidthValue = width;
    aaGlob->overrideSnapWidthAndLerp = true;
}

// Decomp: CoDSP_rdBlackOps.map.c (AimAssist_ResetAdsWidthAndLerp ~822B7B40)
void __cdecl AimAssist_ResetAdsWidthAndLerp(int localClientNum)
{
    AimAssistGlobals *aaGlob;

    aaGlob = &aaGlobArray[localClientNum];
    aaGlob->overrideSnapWidthAndLerp = false;
    aaGlob->overrideAutoaimLerpValue = 0.0f;
    aaGlob->overrideAutoaimWidthValue = 0.0f;
}

