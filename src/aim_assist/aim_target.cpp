#include "aim_target.h"

#include <cstring>
#include <xanim/dobj.h>
#include <qcommon/dobj_management.h>
#include <bgame/bg_local.h>
#include <cgame_mp/cg_ents_mp.h>
#include <clientscript/cscr_stringlist.h>
#include <qcommon/common.h>
#include <qcommon/cmd.h>
#include <universal/dvar.h>
#include <cgame_mp/cg_local_mp.h>
#include <demo/demo_common.h>
#include <gfx_d3d/r_dpvs.h>
#include <cgame_mp/cg_players_mp.h>
#include <universal/com_math.h>
#include <clientscript/scr_const.h>
#include <universal/com_math_anglevectors.h>
#include <bgame/bg_animconditions.h>
#include <bgame/bg_misc.h>
#include <cgame_mp/cg_main_mp.h>
#include <qcommon/cm_load.h>
#include <qcommon/cm_trace.h>
#include <qcommon/cm_test.h>
#include <EffectsCore/fx_system.h>
#include <cgame/cg_world.h>

static const float dog_length = 20.0f;

AimTargetGlob atGlobArray[1];

const dvar_s *aim_target_sentient_radius;
const dvar_s *aim_target_sentient_half_height;
const dvar_s *aim_target_frustum_expand_on_screen;
const dvar_s *aim_target_frustum_expand_fast_updates;
const dvar_s *aim_target_frustum_min_distance;
const dvar_s *aim_target_aim_tag_fast_update_interval;
const dvar_s *aim_target_aim_tag_slow_update_interval;

// Decomp: CoDMPServer.c:115868
AimTargetGlob *__cdecl AimTarget_GetGlobArray(int localClientNum)
{
    return &atGlobArray[localClientNum];
}

// Decomp: CoDMPServer.c:115880
void __cdecl AimTarget_Init(int localClientNum)
{
    AimTargetGlob *atGlob = AimTarget_GetGlobArray(localClientNum);

    memset(atGlob, 0, 0xC04u);
    AimTarget_RegisterDvars();
    Cbuf_InsertText(0, "exec devgui_aimassist\n");
}

// Decomp: CoDMPServer.c:115891
void AimTarget_RegisterDvars()
{
    aim_target_sentient_radius = _Dvar_RegisterFloat(
                                                                 "aim_target_sentient_radius",
                                                                 10.0,
                                                                 0.0,
                                                                 128.0,
                                                                 0x80u,
                                                                 "The radius used to calculate target bounds for a sentient(actor or player)");
    aim_target_sentient_half_height = _Dvar_RegisterFloat(
                                                                            "aim_target_sentient_half_height",
                                                                            32.0,
                                                                            0.0,
                                                                            128.0,
                                                                            0x80u,
                                                                            "The radius used to calculate frustum target center for a sentient(actor or player)");
    aim_target_frustum_expand_on_screen = _Dvar_RegisterFloat(
                                                                                    "aim_target_frustum_expand_on_screen",
                                                                                    -30.0,
                                                                                    -1000.0,
                                                                                    1000.0,
                                                                                    0x80u,
                                                                                    "The frustum expansion to determine if a target is on screen.    Positive is inwards.");
    aim_target_frustum_expand_fast_updates = _Dvar_RegisterFloat(
                                                                                         "aim_target_frustum_expand_fast_updates",
                                                                                         40.0,
                                                                                         -1000.0,
                                                                                         1000.0,
                                                                                         0x80u,
                                                                                         "The frustum expansion to determine if a target is in the fast interval tag "
                                                                                         "update zone.    Positive is inwards.");
    aim_target_frustum_min_distance = _Dvar_RegisterFloat(
                                                                            "aim_target_frustum_min_distance",
                                                                            256.0,
                                                                            0.0,
                                                                            10000.0,
                                                                            0x80u,
                                                                            "If target is inside this distance frustum culling is not applied.");
    aim_target_aim_tag_fast_update_interval = _Dvar_RegisterInt(
                                                                                            "aim_target_aim_tag_fast_update_interval",
                                                                                            59,
                                                                                            1,
                                                                                            10000,
                                                                                            0x80u,
                                                                                            "How often the aim target tag is updated instead of pulled from cache insid"
                                                                                            "e of the \"fast\" zone.");
    aim_target_aim_tag_slow_update_interval = _Dvar_RegisterInt(
                         "aim_target_aim_tag_slow_update_interval",
                         131,
                         1,
                         10000,
                         0x80u,
                         "How often the aim target tag is updated instead of pulled from cache inside of the \"fast\" zone.");
}

// Decomp: CoDMPServer.c:115954
void __cdecl expandMins(float *mins, float *point)
{
    if ( *mins > *point )
        *mins = *point;
    if ( mins[1] > point[1] )
        mins[1] = point[1];
    if ( mins[2] > point[2] )
        mins[2] = point[2];
}

// Decomp: CoDMPServer.c:115965
void __cdecl expandMaxs(float *maxs, float *point)
{
    if ( *point > *maxs )
        *maxs = *point;
    if ( point[1] > maxs[1] )
        maxs[1] = point[1];
    if ( point[2] > maxs[2] )
        maxs[2] = point[2];
}

// Decomp: CoDMPServer.c:115976
void __cdecl AimTarget_ProcessEntityInternal(int localClientNum, const centity_s *ent)
{
    const cg_s *cgameGlob;
    AimTarget target;
    unsigned int visBone;

    PROF_SCOPED("AimTarget_ProcessEntity");

    cgameGlob = CG_GetLocalClientGlobals(localClientNum);
    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp", 902, 0, "%s", "ent") )
        __debugbreak();
    if ( ((*((unsigned int *)ent + 201) >> 1) & 1) == 0
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp",
                    903,
                    0,
                    "%s",
                    "ent->nextValid") )
    {
        __debugbreak();
    }
    if ( ent->nextState.number == cgameGlob->predictedPlayerState.clientNum
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp",
                    905,
                    0,
                    "%s",
                    "ent->nextState.number != cgameGlob->predictedPlayerState.clientNum") )
    {
        __debugbreak();
    }
    if ( Demo_IsPlaying() )
    {
            return;
    }
    if ( !AimTarget_PlayerInValidState(&cgameGlob->predictedPlayerState) )
    {
            return;
    }
    if ( ent->nextState.eType == 1 || ent->nextState.eType == 17 )
    {
        visBone = scr_const.aim_vis_bone;
    }
    else
    {
        if ( (ent->nextState.lerp.eFlags & 0x800) == 0
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp",
                        922,
                        0,
                        "%s",
                        "ent->nextState.lerp.eFlags & EF_AIM_ASSIST") )
        {
            __debugbreak();
        }
        visBone = 0;
    }
    if ( !AimTarget_IsTargetValid(cgameGlob, ent) )
    {
            return;
    }
    if ((ent->nextState.lerp.eFlags2 & 0x1000000) != 0) 
    {
            return;
    }
    if ( ent->pose.eType == 17 && (ent->nextState.lerp.eFlags & 0x40000) != 0 )
    {
            return;
    }
    else if ( player_topDownCamMode->current.integer || AimTarget_IsTargetVisible(localClientNum, ent, visBone) )
    {
        AimTarget_CreateTarget(localClientNum, ent, &target);
        return;
    }
}

// Decomp: CoDMPServer.c:116092
bool __cdecl AimTarget_IsTargetValid(const cg_s *cgameGlob, const centity_s *targetEnt)
{
    const clientInfo_t *playerInfo;
    team_t playerInfoTeam;
    const clientInfo_t *localTargetInfo;
    float toTarget[3];
    float top[3];
    float radius;
    const playerState_s *predictedPlayerState;
    float viewForward[3];
    float dot;

    if ( !cgameGlob
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp", 582, 0, "%s", "cgameGlob") )
    {
        __debugbreak();
    }
    if ( !targetEnt
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp", 583, 0, "%s", "targetEnt") )
    {
        __debugbreak();
    }
    if ( ((*((unsigned int *)targetEnt + 201) >> 1) & 1) == 0
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp",
                    584,
                    0,
                    "%s",
                    "targetEnt->nextValid") )
    {
        __debugbreak();
    }
    if ( targetEnt->nextState.number == cgameGlob->predictedPlayerState.clientNum
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp",
                    586,
                    0,
                    "%s",
                    "targetEnt->nextState.number != cgameGlob->predictedPlayerState.clientNum") )
    {
        __debugbreak();
    }
    predictedPlayerState = &cgameGlob->predictedPlayerState;
    if ( targetEnt->nextState.eType == 1 )
    {
        if ( (targetEnt->nextState.lerp.eFlags & 0x40000) != 0
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp",
                        596,
                        0,
                        "%s",
                        "!(targetEnt->nextState.lerp.eFlags & EF_DEAD)") )
        {
            __debugbreak();
        }
        if ( targetEnt->nextState.clientNum >= 0x20u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp",
                        598,
                        0,
                        "targetEnt->nextState.clientNum doesn't index MAX_CLIENTS\n\t%i not in [0, %i)",
                        targetEnt->nextState.clientNum,
                        32) )
        {
            __debugbreak();
        }
        localTargetInfo = &cgameGlob->bgs.clientinfo[targetEnt->nextState.clientNum];
        if ( predictedPlayerState->clientNum >= 0x20u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp",
                        604,
                        0,
                        "predictedPlayerState->clientNum doesn't index MAX_CLIENTS\n\t%i not in [0, %i)",
                        predictedPlayerState->clientNum,
                        32) )
        {
            __debugbreak();
        }
        playerInfo = &cgameGlob->bgs.clientinfo[predictedPlayerState->clientNum];
        if ( !localTargetInfo->infoValid || !localTargetInfo->model[0] )
            return 0;
        if ( !Com_GetClientDObj(targetEnt->nextState.number, targetEnt->pose.localClientNum)
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp",
                        613,
                        0,
                        "%s",
                        "Com_GetClientDObj( targetEnt->nextState.number, targetEnt->pose.localClientNum )") )
        {
            __debugbreak();
        }
        playerInfoTeam = playerInfo->team;
        if ( localTargetInfo->team == playerInfoTeam && playerInfoTeam )
            return 0;
    }
    else if ( targetEnt->nextState.eType == 17 )
    {
        if ( predictedPlayerState->clientNum >= 0x20u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp",
                        627,
                        0,
                        "predictedPlayerState->clientNum doesn't index MAX_CLIENTS\n\t%i not in [0, %i)",
                        predictedPlayerState->clientNum,
                        32) )
        {
            __debugbreak();
        }
        if ( targetEnt->nextState.lerp.u.actor.team == cgameGlob->bgs.clientinfo[predictedPlayerState->clientNum].team )
            return 0;
    }
    else if ( (targetEnt->nextState.lerp.eFlags & 0x800) == 0 )
    {
        return 0;
    }
    toTarget[0] = targetEnt->pose.origin[0] - predictedPlayerState->origin[0];
    toTarget[1] = targetEnt->pose.origin[1] - predictedPlayerState->origin[1];
    toTarget[2] = targetEnt->pose.origin[2] - predictedPlayerState->origin[2];
    if ( player_topDownCamMode->current.integer > 0 )
        return 1;
    *(_QWORD *)viewForward = *(_QWORD *)&cgameGlob->refdef.viewaxis[0][0];
    viewForward[2] = cgameGlob->refdef.viewaxis[0][2];
    radius = AimTarget_GetTargetRadius(targetEnt);
    dot = (float)((float)((float)(viewForward[0] * toTarget[0]) + (float)(viewForward[1] * toTarget[1]))
                            + (float)(viewForward[2] * toTarget[2]))
            + radius;
    if ( dot < 0.0 )
        return 0;
    if ( aim_target_frustum_min_distance->current.value > Vec3DistanceSq(
                                                                                                                    predictedPlayerState->origin,
                                                                                                                    targetEnt->pose.origin) )
        return 1;
    if ( !CG_PlayerInViewFrustum(targetEnt->pose.localClientNum, targetEnt) )
        return 0;
    AimTarget_GetTargetTop(targetEnt, top);
    return R_CullPoint(cgameGlob->localClientNum, top, aim_target_frustum_expand_on_screen->current.value) == 0;
}

// Decomp: CoDMPServer.c:116264
double __cdecl AimTarget_GetTargetRadius(const centity_s *targetEnt)
{
    float mins[3];
    float maxs[3];

    if ( !targetEnt
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp", 519, 0, "%s", "targetEnt") )
    {
        __debugbreak();
    }
    if ( targetEnt->nextState.eType == 1 || targetEnt->nextState.eType == 17 )
        return aim_target_sentient_radius->current.value;
    AimTarget_GetTargetBounds(targetEnt, mins, maxs);
    return (float)RadiusFromBounds(mins, maxs);
}

// Decomp: CoDMPServer.c:116416 — internal tag lookup (distinct from AimTarget_GetTagPos in aim_assist.cpp)
void __cdecl AimTarget_GetTagPos_0(const centity_s *ent, unsigned int tagName, float *pos)
{
    DObj *dobj;
    bool wasPrevTarget;
    int tagUpdateInterval;

    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp", 314, 0, "%s", "ent") )
        __debugbreak();
    if ( !pos && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp", 315, 0, "%s", "pos") )
        __debugbreak();
    if ( ent->nextState.eType == 1 )
    {
        wasPrevTarget = AimAssist_IsPrevTargetEntity(ent->pose.localClientNum, ent->nextState.number);
        tagUpdateInterval = AimTarget_GetTagUpdateInterval(ent);
        if ( !CachedTag_GetTagPos(ent, tagName, pos, tagUpdateInterval, wasPrevTarget) )
        {
            const char *tagNameStr = SL_ConvertToString(tagName, SCRIPTINSTANCE_SERVER);
            Com_Error(ERR_DROP, "AimTarget_GetTagPos: Cannot find tag [%s] on entity\n", tagNameStr);
        }
    }
    else
    {
        dobj = Com_GetClientDObj(ent->nextState.number, ent->pose.localClientNum);
        if ( !dobj
            && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp", 333, 0, "%s", "dobj") )
        {
            __debugbreak();
        }
        if ( !CG_DObjGetWorldTagPos(&ent->pose, dobj, tagName, pos) )
        {
            const char *tagNameStr = SL_ConvertToString(tagName, SCRIPTINSTANCE_SERVER);
            Com_Error(ERR_DROP, "AimTarget_GetTagPos: Cannot find tag [%s] on entity\n", tagNameStr);
        }
    }
}

// Decomp: CoDMPServer.c:116287
void __cdecl AimTarget_GetTargetBounds(const centity_s *targetEnt, float *mins, float *maxs)
{
    const DObj *dobj;
    float right[3];
    float forward[3];
    float left[3];
    float tail[3];
    float highBonePos[3];

    if ( !targetEnt
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp", 427, 0, "%s", "targetEnt") )
    {
        __debugbreak();
    }
    if ( !mins && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp", 428, 0, "%s", "mins") )
        __debugbreak();
    if ( !maxs && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp", 429, 0, "%s", "maxs") )
        __debugbreak();
    if ( targetEnt->nextState.eType == 17 )
    {
        const float sentientRadius = aim_target_sentient_radius->current.value;
        const float tailScale = -(sentientRadius + dog_length);
        float headForward[3];

        AimTarget_GetTagPos_0(targetEnt, scr_const.aim_highest_bone, highBonePos);
        AngleVectors(targetEnt->pose.angles, forward, right, 0);
        right[0] = sentientRadius * right[0];
        right[1] = sentientRadius * right[1];
        right[2] = sentientRadius * right[2];
        left[0] = -right[0];
        left[1] = -right[1];
        left[2] = -right[2];
        headForward[0] = sentientRadius * forward[0];
        headForward[1] = sentientRadius * forward[1];
        headForward[2] = sentientRadius * forward[2];
        tail[0] = tailScale * forward[0];
        tail[1] = tailScale * forward[1];
        tail[2] = tailScale * forward[2];
        *mins = headForward[0];
        mins[1] = headForward[1];
        mins[2] = headForward[2];
        expandMins(mins, tail);
        expandMins(mins, right);
        expandMins(mins, left);
        *maxs = headForward[0];
        maxs[1] = headForward[1];
        maxs[2] = headForward[2];
        expandMaxs(maxs, tail);
        expandMaxs(maxs, right);
        expandMaxs(maxs, left);
        maxs[2] = (float)(highBonePos[2] - targetEnt->pose.origin[2]) + maxs[2];
    }
    else if ( targetEnt->nextState.eType != 1 )
    {
        if ( targetEnt->nextState.solid == 0xFFFFFF )
        {
            CM_ModelBounds(targetEnt->nextState.index.brushmodel, mins, maxs);
        }
        else
        {
            dobj = Com_GetClientDObj(targetEnt->nextState.number, targetEnt->pose.localClientNum);
            if ( dobj )
            {
                DObjGetBounds(dobj, mins, maxs);
            }
            else
            {
                *mins = -1.0f;
                mins[1] = -1.0f;
                mins[2] = -1.0f;
                *maxs = 1.0f;
                maxs[1] = 1.0f;
                maxs[2] = 1.0f;
            }
        }
    }
    else
    {
        const float sentientRadius = aim_target_sentient_radius->current.value;

        AimTarget_GetTagPos_0(targetEnt, scr_const.aim_highest_bone, highBonePos);
        mins[0] = -sentientRadius;
        mins[1] = -sentientRadius;
        mins[2] = 0.0f;
        maxs[0] = sentientRadius;
        maxs[1] = sentientRadius;
        maxs[2] = highBonePos[2] - targetEnt->pose.origin[2];
    }
}

// Decomp: CoDMPServer.c:116479
int __cdecl AimTarget_GetTagUpdateInterval(const centity_s *ent)
{
    const cg_s *cgameGlob;
    float center[3];

    cgameGlob = CG_GetLocalClientGlobals(ent->pose.localClientNum);
    center[0] = ent->pose.origin[0];
    center[1] = ent->pose.origin[1];
    center[2] = ent->pose.origin[2] + aim_target_sentient_half_height->current.value;
    if ( (double)aim_target_frustum_min_distance->current.value > Vec3DistanceSq(
                                                                                                                                    cgameGlob->predictedPlayerState.origin,
                                                                                                                                    ent->pose.origin) )
        return aim_target_aim_tag_fast_update_interval->current.integer;
    if ( R_CullPoint(ent->pose.localClientNum, center, aim_target_frustum_expand_fast_updates->current.value) )
        return aim_target_aim_tag_slow_update_interval->current.integer;
    return aim_target_aim_tag_fast_update_interval->current.integer;
}

// Decomp: CoDMPServer.c:116505
void __cdecl AimTarget_GetTargetTop(const centity_s *targetEnt, float *top)
{
    float mins[3];
    float maxs[3];

    if ( !targetEnt
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp", 558, 0, "%s", "targetEnt") )
    {
        __debugbreak();
    }
    if ( !top && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp", 559, 0, "%s", "top") )
        __debugbreak();
    AimTarget_GetTargetBounds(targetEnt, mins, maxs);
    *top = mins[0] + maxs[0];
    top[1] = mins[1] + maxs[1];
    top[2] = mins[2] + maxs[2];
    *top = 0.5 * *top;
    top[1] = 0.5 * top[1];
    top[2] = 0.5 * top[2];
    top[2] = maxs[2];
    *top = targetEnt->pose.origin[0] + *top;
    top[1] = targetEnt->pose.origin[1] + top[1];
    top[2] = targetEnt->pose.origin[2] + top[2];
}

// Decomp: CoDMPServer.c:116551
bool __cdecl AimTarget_IsTargetVisible(int localClientNum, const centity_s *targetEnt, unsigned int visBone)
{
    col_context_t context;
    float endPos[3];
    const cg_s *cgameGlob;
    const playerState_s *predictedPlayerState;
    trace_t trace;
    float playerEyePos[3];
    float targetEyePos[3];

    memset(&trace, 0, 16);
    if ( !targetEnt
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp", 736, 0, "%s", "targetEnt") )
    {
        __debugbreak();
    }
    if ( visBone )
        AimTarget_GetTagPos_0(targetEnt, visBone, targetEyePos);
    else
        AimTarget_GetTargetCenter(targetEnt, targetEyePos);
    cgameGlob = CG_GetLocalClientGlobals(localClientNum);
    playerEyePos[0] = cgameGlob->refdef.vieworg[0];
    playerEyePos[1] = cgameGlob->refdef.vieworg[1];
    playerEyePos[2] = cgameGlob->refdef.vieworg[2];
    predictedPlayerState = &cgameGlob->predictedPlayerState;
    ////col_context_t::col_context_t(&context);
    CG_TraceCapsule(
        &trace,
        playerEyePos,
        vec3_origin,
        vec3_origin,
        targetEyePos,
        predictedPlayerState->clientNum,
        8400899,
        &context);
    if ( trace.fraction != 1.0 && Trace_GetEntityHitId(&trace) != targetEnt->nextState.number )
    {
        if (targetEnt->nextState.solid != 0xFFFFFF)
            return 0;
        Vec3Lerp(playerEyePos, targetEyePos, trace.fraction, endPos);
        if ( !CM_TransformedPointContents(
                        endPos,
                        targetEnt->nextState.index.brushmodel,
                        targetEnt->pose.origin,
                        targetEnt->pose.angles) )
            return 0;
    }
    return FX_ClientVisibilityTest(localClientNum, playerEyePos, targetEyePos);
}

// Decomp: CoDMPServer.c:116621
void __cdecl AimTarget_GetTargetCenter(const centity_s *targetEnt, float *center)
{
    float mins[3];
    float maxs[3];

    if ( !targetEnt
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp", 539, 0, "%s", "targetEnt") )
    {
        __debugbreak();
    }
    if ( !center
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp", 540, 0, "%s", "center") )
    {
        __debugbreak();
    }
    AimTarget_GetTargetBounds(targetEnt, mins, maxs);
    *center = mins[0] + maxs[0];
    center[1] = mins[1] + maxs[1];
    center[2] = mins[2] + maxs[2];
    *center = 0.5 * *center;
    center[1] = 0.5 * center[1];
    center[2] = 0.5 * center[2];
    *center = targetEnt->pose.origin[0] + *center;
    center[1] = targetEnt->pose.origin[1] + center[1];
    center[2] = targetEnt->pose.origin[2] + center[2];
}

// Decomp: CoDMPServer.c:116666
void __cdecl AimTarget_CreateTarget(int localClientNum, const centity_s *targetEnt, AimTarget *target)
{
    int snapServerTime;
    const cg_s *cgameGlob;
    int nextSnapServerTime;
    float currentPos[3];
    float nextPos[3];
    float deltaTime;

    if ( !targetEnt
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp", 805, 0, "%s", "targetEnt") )
    {
        __debugbreak();
    }
    if ( !target
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp", 806, 0, "%s", "target") )
    {
        __debugbreak();
    }
    cgameGlob = CG_GetLocalClientGlobals(localClientNum);
    target->entIndex = targetEnt->nextState.number;
    target->worldDistSqr = Vec3DistanceSq(cgameGlob->predictedPlayerState.origin, targetEnt->pose.origin);
    AimTarget_GetTargetBounds(targetEnt, target->mins, target->maxs);
    snapServerTime = cgameGlob->snap->serverTime;
    nextSnapServerTime = cgameGlob->nextSnap->serverTime;
    deltaTime = (float)(nextSnapServerTime - snapServerTime) * 0.001;
    if ( deltaTime <= 0.0 )
    {
        target->velocity[0] = 0.0f;
        target->velocity[1] = 0.0f;
        target->velocity[2] = 0.0f;
        target->heightChange = 0.0f;
    }
    else
    {
        BG_EvaluateTrajectory(&targetEnt->currentState.pos, snapServerTime, currentPos);
        BG_EvaluateTrajectory(&targetEnt->nextState.lerp.pos, nextSnapServerTime, nextPos);
        const float invDeltaTime = 1.0f / deltaTime;

        target->velocity[0] = nextPos[0] - currentPos[0];
        target->velocity[1] = nextPos[1] - currentPos[1];
        target->velocity[2] = nextPos[2] - currentPos[2];
        target->velocity[0] = invDeltaTime * target->velocity[0];
        target->velocity[1] = invDeltaTime * target->velocity[1];
        target->velocity[2] = invDeltaTime * target->velocity[2];
    }
    AimTarget_AddTargetToList(localClientNum, target);
}

// Decomp: CoDMPServer.c:116740
void __cdecl AimTarget_AddTargetToList(int localClientNum, const AimTarget *target)
{
    int targetIndex;
    int insertIndex;
    AimTargetGlob *atGlob;
    int searchEndIndex;

    if ( !target
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp", 239, 0, "%s", "target") )
    {
        __debugbreak();
    }
    atGlob = AimTarget_GetGlobArray(localClientNum);
    for ( targetIndex = 0; targetIndex < atGlob->targetCount; ++targetIndex )
    {
        if ( target->entIndex == atGlob->targets[targetIndex].entIndex
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp",
                        251,
                        0,
                        "%s",
                        "target->entIndex != atGlob->targets[targetIndex].entIndex") )
        {
            __debugbreak();
        }
    }
    insertIndex = 0;
    searchEndIndex = atGlob->targetCount;
    while ( insertIndex < searchEndIndex )
    {
        const int midIndex = (searchEndIndex + insertIndex) / 2;

        if ( AimTarget_CompareTargets(target, &atGlob->targets[midIndex]) <= 0 )
            insertIndex = midIndex + 1;
        else
            searchEndIndex = midIndex;
    }
    if ( insertIndex < 64 )
    {
        if ( atGlob->targetCount == 64 )
            --atGlob->targetCount;
        memmove(
            (unsigned __int8 *)&atGlob->targets[insertIndex + 1],
            (unsigned __int8 *)&atGlob->targets[insertIndex],
            48 * (atGlob->targetCount - insertIndex));
        memcpy(&atGlob->targets[insertIndex], target, sizeof(atGlob->targets[insertIndex]));
        ++atGlob->targetCount;
    }
}

// Decomp: CoDMPServer.c:116793
int __cdecl AimTarget_CompareTargets(const AimTarget *targetA, const AimTarget *targetB)
{
    if ( !targetA
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp", 216, 0, "%s", "targetA") )
    {
        __debugbreak();
    }
    if ( !targetB
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp", 217, 0, "%s", "targetB") )
    {
        __debugbreak();
    }
    if ( targetB->worldDistSqr > targetA->worldDistSqr )
        return 1;
    if ( targetA->worldDistSqr <= targetB->worldDistSqr )
        return 0;
    return -1;
}

// Decomp: CoDMPServer.c:116825
bool __cdecl AimTarget_PlayerInValidState(const playerState_s *ps)
{
    if ( !ps && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp", 865, 0, "%s", "ps") )
        __debugbreak();
    if ( (ps->otherFlags & 2) != 0 )
        return false;
    switch ( ps->pm_type )
    {
        case 2:
        case 3:
        case 4:
        case 5:
        case 9:
        case 0xA:
            return false;
        default:
            return true;
    }
}

// Decomp: CoDMPServer.c:116860
void __cdecl AimTarget_UpdateClientTargets(int localClientNum)
{
    int entityType;
    cg_s *cgameGlob;
    const centity_s *cent;
    int snapEntIndex;

    if ( localClientNum
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp",
                    1078,
                    0,
                    "localClientNum doesn't index MAX_LOCAL_CLIENTS\n\t%i not in [0, %i)",
                    localClientNum,
                    1) )
    {
        __debugbreak();
    }
    cgameGlob = CG_GetLocalClientGlobals((int)localClientNum);
    if ( !cgameGlob->nextSnap
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp",
                    1082,
                    0,
                    "%s",
                    "cgameGlob->nextSnap") )
    {
        __debugbreak();
    }
    AimTarget_ClearTargetList((int)localClientNum);
    //BLOPS_NULLSUB();
    for ( snapEntIndex = 0; snapEntIndex < cgameGlob->nextSnap->numEntities; ++snapEntIndex )
    {
        cent = CG_GetEntity((int)localClientNum, cgameGlob->nextSnap->entities[snapEntIndex].number);
        if ( (cent->nextState.lerp.eFlags & 0x20) == 0
            && ((*((unsigned int *)cent + 201) >> 1) & 1) != 0
            && cgameGlob->predictedPlayerState.clientNum != cent->nextState.number )
        {
            bool processTarget = false;

            entityType = cent->nextState.eType;
            if ( entityType == 1 )
            {
                if ( (cent->nextState.lerp.eFlags & 0x40000) == 0 )
                    processTarget = true;
            }
            else if ( entityType == 6 )
            {
                if ( (cent->nextState.lerp.eFlags & 0x800) != 0 )
                    processTarget = true;
            }
            else if ( entityType == 17 && (cent->nextState.lerp.eFlags & 0x40000) == 0 )
            {
                processTarget = true;
            }
            if ( processTarget )
                AimTarget_ProcessEntity((int)localClientNum, cent);
        }
    }

    {
        PROF_SCOPED("aim assist epilog");
        //BLOPS_NULLSUB();
    }
}

// Decomp: CoDMPServer.c:116927
void __cdecl AimTarget_ClearTargetList(int localClientNum)
{
    AimTarget_GetGlobArray(localClientNum)->targetCount = 0;
}

// Decomp: CoDMPServer.c:116933
void __cdecl AimTarget_ProcessEntity(int localClientNum, const centity_s *ent)
{
    AimTarget_ProcessEntityInternal(localClientNum, ent);
}

// Decomp: CoDMPServer.c:116939
void __cdecl AimTarget_GetClientTargetList(int localClientNum, AimTargetGlob **targetList, int *targetCount)
{
    AimTargetGlob *atGlob;

    if ( !targetList
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp", 1145, 0, "%s", "targetList") )
    {
        __debugbreak();
    }
    if ( !targetCount
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\aim_assist\\aim_target.cpp", 1146, 0, "%s", "targetCount") )
    {
        __debugbreak();
    }
    atGlob = AimTarget_GetGlobArray(localClientNum);
    *targetList = atGlob;
    *targetCount = atGlob->targetCount;
}

