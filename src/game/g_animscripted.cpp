#include "g_animscripted.h"

#include "g_main_sp.h"
#include "g_scr_main.h"
#include "g_utils_sp.h"
#include "actor_corpse.h"
#include "actor_state.h"

#include <clientscript/cscr_memorytree.h>
#include <clientscript/cscr_vm.h>
#include <clientscript/cscr_stringlist.h>
#include <clientscript/scr_const.h>
#include <qcommon/dobj_management.h>
#include <universal/com_math.h>
#include <universal/com_math_anglevectors.h>
#include <xanim/xanim.h>

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetCycleOriginOffset)
void __cdecl GScr_GetCycleOriginOffset()
{
    scr_anim_s anim;
    const XAnim_s *anims;
    float angles[3];
    float axis[3][3];
    float rot[3];
    float transStart[3];
    float transEnd[3];
    float delta[3];
    float result[3];

    Scr_GetVector(0, angles, SCRIPTINSTANCE_SERVER);
    anim = Scr_GetAnim(1u, 0, SCRIPTINSTANCE_SERVER);
    AnglesToAxis(angles, axis);
    anims = Scr_GetAnims(anim.tree, SCRIPTINSTANCE_SERVER);
    XAnimGetAbsDelta(anims, anim.index, rot, transStart, 0.0f);
    XAnimGetAbsDelta(anims, anim.index, rot, transEnd, 1.0f);
    delta[0] = transEnd[0] - transStart[0];
    delta[1] = transEnd[1] - transStart[1];
    delta[2] = transEnd[2] - transStart[2];
    MatrixTransformVector(delta, axis, result);
    Scr_AddVector(result, SCRIPTINSTANCE_SERVER);
}

static void __cdecl CalcDeltaOriginAndAngles(
    DObj *obj,
    unsigned int anim,
    const float (*matrix)[3],
    float *origin,
    float *angles)
{
    float rot[2];
    float trans[6];
    float localAxis[4][3];
    float resultAxis[4][3];
    double yaw;

    XAnimCalcAbsDelta(obj, anim, rot, trans);
    MatrixTransformVector43(trans, (const mat4x3&)matrix, origin);
    yaw = RotationToYaw(rot);
    YawToAxis(yaw, (mat3x3&)*localAxis);
    MatrixMultiply((const mat3x3&)*localAxis, (const mat3x3&)*matrix, (mat3x3&)*resultAxis);
    AxisToAngles((const mat3x3&)*resultAxis, angles);
}

static void __cdecl G_Animscripted_DeathPlant(
    gentity_s *ent,
    const XAnim_s *anims,
    unsigned int anim,
    float *origin,
    const float *angles)
{
    animscripted_s *scripted;
    int number;
    float fraction;
    int entNum;
    double traceLen;
    float traceStart[3];
    float traceEnd[3];
    float anglesV[3];
    float trans[3];
    float vOrigin[4];
    float transformedPos[3];
    float rot[4];
    trace_t trace;
    mat3x3 newMat;
    col_context_t context;

    scripted = ent->scripted;
    number = ent->s.number;

    traceStart[0] = origin[0];
    traceStart[1] = origin[1];
    traceStart[2] = origin[2] + 36.0f;

    traceEnd[0] = origin[0];
    traceEnd[1] = origin[1];
    traceEnd[2] = origin[2] - 18.0f;

    G_TraceCapsule(&trace, traceStart, actorMins, actorMaxs, traceEnd, number, 8519697, &context);

    if ( trace.allsolid || (fraction = trace.fraction, trace.fraction >= 1.0f) )
    {
        scripted->axis[3][0] = origin[0];
        scripted->axis[3][1] = origin[1];
        scripted->axis[3][2] = origin[2];
    }
    else
    {
        scripted->axis[3][0] = ((traceEnd[0] - traceStart[0]) * trace.fraction) + traceStart[0];
        scripted->axis[3][1] = ((traceEnd[1] - traceStart[1]) * fraction) + traceStart[1];
        scripted->axis[3][2] = (((traceEnd[2] - traceStart[2]) * fraction) + traceStart[2]);
    }
    scripted->fHeightOfs = 0.0f;
    anglesV[1] = angles[1];
    anglesV[2] = angles[2];
    anglesV[0] = 0.0f;
    AnglesToAxis(anglesV, scripted->axis);
    XAnimGetAbsDelta(anims, anim, rot, trans, 1.0f);
    MatrixTransformVector43(trans, scripted->axis, transformedPos);
    mat3x3 yawAxis;
    YawToAxis(RotationToYaw(rot), yawAxis);
    MatrixMultiply(yawAxis, (const mat3x3&)scripted->axis, newMat);
    AxisToAngles(newMat, anglesV);
    traceStart[0] = transformedPos[0];
    traceEnd[0] = transformedPos[0];
    entNum = ent->s.number;
    traceStart[1] = transformedPos[1];
    traceEnd[1] = transformedPos[1];
    traceLen = Vec3Length(trans) + 128.0f;
    traceStart[2] = (float)traceLen + transformedPos[2];
    traceEnd[2] = transformedPos[2] - (float)traceLen;
    G_TraceCapsule(&trace, traceStart, actorMins, actorMaxs, traceEnd, entNum, 8519697, &context);
    if ( trace.allsolid || trace.fraction >= 1.0f )
    {
        scripted->fEndPitch = 0.0f;
        scripted->fEndRoll = 0.0f;
    }
    else
    {
        vOrigin[0] = (float)((float)(traceEnd[0] - traceStart[0]) * trace.fraction) + traceStart[0];
        vOrigin[1] = (float)((float)(traceEnd[1] - traceStart[1]) * trace.fraction) + traceStart[1];
        vOrigin[2] = (float)((float)(traceEnd[2] - traceStart[2]) * trace.fraction) + traceStart[2];
        Actor_GetBodyPlantAngles(entNum, 8519697, vOrigin, anglesV[1], &scripted->fEndPitch, &scripted->fEndRoll, NULL);
    }
    if ( XAnimGetLength(anims, anim) >= 1.0f )
        scripted->fOrientLerp = 0.0f;
    else
        scripted->fOrientLerp = -1.0f;
}

static void __cdecl G_AnimScripted_ClearAnimWeights(
    DObj *obj,
    XAnimTree_s *pAnimTree,
    unsigned int root,
    actor_s *pActor)
{
    double weight;
    unsigned int lookAtAnim;
    double lookAtWeight;
    float blendTime;
    int cmdIndex;

    cmdIndex = 0;
    if ( !pActor || pActor->lookAtInfo.fLookAtTurnAngle == 0.0f || !pActor->lookAtInfo.bLookAtSetup )
    {
        blendTime = 0.2f;
        goto clear_root;
    }
    weight = XAnimGetWeight(pAnimTree, pActor->lookAtInfo.animLookAtStraight);
    lookAtWeight = XAnimGetWeight(pAnimTree, pActor->lookAtInfo.animLookAtLeft);
    if ( lookAtWeight > 0.0 )
    {
        lookAtAnim = pActor->lookAtInfo.animLookAtLeft;
    }
    else
    {
        lookAtWeight = XAnimGetWeight(pAnimTree, pActor->lookAtInfo.animLookAtRight);
        if ( lookAtWeight > 0.0 )
            lookAtAnim = pActor->lookAtInfo.animLookAtRight;
        else
            lookAtAnim = 0;
    }
    if ( lookAtAnim )
        XAnimSetGoalWeightKnob(obj, lookAtAnim, (float)lookAtWeight, 0.2f, 1.0f, 0, 0, 0, cmdIndex);
    XAnimSetCompleteGoalWeight(obj, pActor->lookAtInfo.animLookAtStraight, (float)weight, 0.2f, 1.0f, 0, 0, 0, cmdIndex);
    if ( !root )
        return;
    blendTime = 0.2f;
clear_root:
    XAnimClearTreeGoalWeightsStrict(pAnimTree, root, blendTime, cmdIndex);
}

void __cdecl G_Animscripted(
    gentity_s *ent,
    float *origin,
    const float *angles,
    unsigned int anim,
    unsigned int root,
    unsigned int notifyName,
    unsigned __int8 animMode)
{
    XAnimTree_s *pAnimTree;
    animscripted_s *scripted;
    const XAnim_s *anims;
    DObj *obj;
    bool isLooped;
    float rot[2];
    float transformedPos[4];
    float trans[4];
    float axisAngles[6];
    float localAxis[4][3];
    float resultAxis[4][3];
    double yaw;
    int cmdIndex;

    if ( (ent->flags & 0x2000) == 0 )
    {
        Scr_ObjectError(
            va(
                "entity (classname: '%s') does not currently support animscripted",
                SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER)),
            SCRIPTINSTANCE_SERVER);
    }
    pAnimTree = GScr_GetEntAnimTree(ent);
    iassert(pAnimTree);
    scripted = ent->scripted;
    if ( !scripted )
    {
        scripted = (animscripted_s *)MT_Alloc(sizeof(animscripted_s), 18, SCRIPTINSTANCE_SERVER);
        ent->scripted = scripted;
    }
    scripted->anim = anim;
    scripted->root = root;
    scripted->mode = animMode;
    scripted->bStarted = 0;
    anims = XAnimGetAnims(pAnimTree);
    if ( animMode == 1 )
        G_Animscripted_DeathPlant(ent, anims, anim, origin, angles);
    else
    {
        scripted->axis[3][0] = *origin;
        scripted->axis[3][1] = origin[1];
        scripted->axis[3][2] = origin[2];
        AnglesToAxis(angles, scripted->axis);
    }
    obj = Com_GetServerDObj(ent->s.number);
    G_AnimScripted_ClearAnimWeights(obj, pAnimTree, root, ent->actor);
    cmdIndex = 0;
    isLooped = XAnimIsLooped(anims, anim);
    XAnimSetCompleteGoalWeight(obj, anim, 1.0f, 0.2f, 1.0f, notifyName, 0, !isLooped, cmdIndex);
    G_FlagAnimForUpdate(ent);
    XAnimCalcAbsDelta(obj, anim, rot, trans);
    MatrixTransformVector43(trans, scripted->axis, transformedPos);
    yaw = RotationToYaw(rot);
    YawToAxis(yaw, (mat3x3&)*localAxis);
    MatrixMultiply((const mat3x3&)*localAxis, (const mat3x3&)*scripted->axis, (mat3x3&)*resultAxis);
    AxisToAngles((const mat3x3&)*resultAxis, axisAngles);
    scripted->originError[0] = ent->r.currentOrigin[0] - transformedPos[0];
    scripted->originError[1] = ent->r.currentOrigin[1] - transformedPos[1];
    scripted->originError[2] = ent->r.currentOrigin[2] - transformedPos[2];
    AnglesSubtract(ent->r.currentAngles, axisAngles, scripted->anglesError);
}

static void __cdecl G_ReduceOriginError(float *origin, float *originError, double maxChange)
{
    double lenSq;
    double invLen;
    float reducedOrigin[3];
    float oldOrigin[3];

    lenSq = (float)((float)(originError[2] * originError[2])
        + (float)((float)(*originError * *originError) + (float)(originError[1] * originError[1])));
    if ( lenSq == 0.0 )
        return;
    invLen = I_rsqrt(lenSq);
    if ( (float)((float)invLen * (float)maxChange) >= 1.0f )
    {
        *origin = *origin + *originError;
        origin[1] = origin[1] + originError[1];
        origin[2] = origin[2] + originError[2];
        *originError = 0.0f;
        originError[1] = 0.0f;
        originError[2] = 0.0f;
    }
    else
    {
        reducedOrigin[0] = (float)(*origin + *originError);
        reducedOrigin[1] = (float)(origin[1] + originError[1]);
        reducedOrigin[2] = (float)(origin[2] + originError[2]);
        oldOrigin[0] = *origin;
        oldOrigin[1] = origin[1];
        oldOrigin[2] = origin[2];
        *origin = (float)(*originError * (float)((float)invLen * (float)maxChange)) + *origin;
        origin[1] = (float)((float)((float)invLen * (float)maxChange) * originError[1]) + origin[1];
        origin[2] = (float)((float)((float)invLen * (float)maxChange) * originError[2]) + origin[2];
        *originError = reducedOrigin[0] - *origin;
        originError[1] = reducedOrigin[1] - origin[1];
        originError[2] = reducedOrigin[2] - origin[2];
    }
}

static void __cdecl G_ReduceAnglesError(float *angles, float *anglesError, double maxChange)
{
    float *error;
    int offset;
    int remaining;

    error = anglesError;
    offset = (char *)angles - (char *)anglesError;
    remaining = 3;
    do
    {
        if ( *error != 0.0f )
        {
            if ( *error <= (float)maxChange )
            {
                if ( *error >= (float)-maxChange )
                {
                    *(float *)((char *)error + offset) = *error + *(float *)((char *)error + offset);
                    *error = 0.0f;
                }
                else
                {
                    *error = *error + (float)maxChange;
                    *(float *)((char *)error + offset) = AngleNormalize360((float)(*(float *)((char *)error + offset) - (float)maxChange));
                }
            }
            else
            {
                *error = *error - (float)maxChange;
                *(float *)((char *)error + offset) = AngleNormalize360((float)((float)maxChange + *(float *)((char *)error + offset)));
            }
        }
        --remaining;
        ++error;
    } while ( remaining );
}

static void __cdecl G_AnimScripted_Think_DeathPlant(gentity_s *ent, XAnimTree_s *tree, float *origin, float *angles)
{
    animscripted_s *scripted;
    int number;
    float fOrientLerp;
    float traceStart[3];
    float traceEnd[3];
    float delta[3];
    trace_t trace;
    col_context_t context;

    scripted = ent->scripted;
    number = ent->s.number;

    Vec3Copy(origin, traceStart);
    Vec3Copy(origin, traceEnd);

    traceStart[2] = scripted->fHeightOfs + origin[2] + 18.0f;
    traceEnd[2] = (float)(scripted->fHeightOfs - (float)18.0) + (float)origin[2];

    G_TraceCapsule(&trace, traceStart, actorMins, actorMaxs, traceEnd, number, 8519697, &context);
    if ( !trace.allsolid && trace.fraction < 1.0f )
    {
        scripted->fHeightOfs = (((traceEnd[2] - traceStart[2]) * trace.fraction) + traceStart[2]) - origin[2];
        delta[0] = traceEnd[0] - traceStart[0];
        delta[1] = traceEnd[1] - traceStart[1];
        delta[2] = traceEnd[2] - traceStart[2];
        Vec3Mad(traceStart, trace.fraction, delta, origin);
    }
    if ( scripted->fOrientLerp >= 0.0f )
    {
        scripted->fOrientLerp = scripted->fOrientLerp + 0.05f;
        if ( scripted->fOrientLerp > 1.0f )
            scripted->fOrientLerp = 1.0f;
        fOrientLerp = scripted->fOrientLerp;
    }
    else
    {
        fOrientLerp = XAnimGetTime(tree, scripted->anim);
    }
    angles[0] = (scripted->fEndPitch * fOrientLerp) + angles[0];
    angles[2] = (scripted->fEndRoll * fOrientLerp) + angles[2];
}

static void __cdecl G_AnimScripted_UpdateEntityOriginAndAngles(gentity_s *ent, float *origin, const float *angles)
{
    float childAxis[12];
    float worldAxis[12];
    float parentRelAxis[6][3];

    if ( ent->tagInfo )
    {
        G_CalcTagParentRelAxis(ent, parentRelAxis);
        worldAxis[9] = *origin;
        worldAxis[10] = origin[1];
        worldAxis[11] = origin[2];
        AnglesToAxis(angles, (float (*)[3])worldAxis);
        MatrixMultiply43((const mat4x3&)worldAxis, (const mat4x3&)parentRelAxis, (mat4x3&)childAxis);
        ent->r.currentOrigin[0] = childAxis[9];
        ent->r.currentOrigin[1] = childAxis[10];
        ent->r.currentOrigin[2] = childAxis[11];
        AxisToAngles((const mat3x3&)childAxis, ent->r.currentAngles);
        G_CalcTagAxis(ent, 0);
    }
    else
    {
        ent->r.currentOrigin[0] = *origin;
        ent->r.currentOrigin[1] = origin[1];
        ent->r.currentOrigin[2] = origin[2];
        ent->r.currentAngles[0] = *angles;
        ent->r.currentAngles[1] = angles[1];
        ent->r.currentAngles[2] = angles[2];
    }
}

void __cdecl G_Animscripted_Think(gentity_s *ent)
{
    animscripted_s *scripted;
    XAnimTree_s *entAnimTree;
    DObj *obj;
    float angles[4];
    float origin[16];
    int cmdIndex;

    scripted = ent->scripted;
    if ( !scripted )
        return;
    entAnimTree = G_GetEntAnimTree(ent);
    if ( !entAnimTree || !scripted->anim )
    {
        MT_Free((unsigned char *)scripted, sizeof(animscripted_s), SCRIPTINSTANCE_SERVER);
        ent->scripted = 0;
        return;
    }
    obj = Com_GetServerDObj(ent->s.number);
    CalcDeltaOriginAndAngles(obj, scripted->anim, scripted->axis, origin, angles);
    if ( scripted->mode == 1 )
        G_AnimScripted_Think_DeathPlant(ent, entAnimTree, origin, angles);
    G_ReduceOriginError(origin, scripted->originError, 0.25);
    G_ReduceAnglesError(angles, scripted->anglesError, (float)(ent->angleLerpRate * 0.05f));
    G_AnimScripted_UpdateEntityOriginAndAngles(ent, origin, angles);
    if ( scripted->bStarted )
    {
        if ( XAnimHasFinished(entAnimTree, scripted->anim) )
        {
            cmdIndex = 0;
            XAnimSetCompleteGoalWeight(obj, scripted->anim, 1.0f, 0.2f, 1.0f, 0, 0, 0, cmdIndex);
            scripted->anim = 0;
        }
    }
    else
    {
        scripted->bStarted = 1;
    }
}

void __cdecl G_StopAnimScripted(gentity_s *ent, float blendTime)
{
    actor_s *actor;
    animscripted_s *scripted;
    XAnimTree_s *pAnimTree;
    DObj *obj;
    int cmdIndex;

    actor = ent->actor;
    if ( actor && actor->eSimulatedState[actor->simulatedStateLevel] == AIS_SCRIPTEDANIM )
        Actor_PopState(actor);
    scripted = ent->scripted;
    if ( !scripted )
        return;
    if ( scripted->anim )
    {
        pAnimTree = GScr_GetEntAnimTree(ent);
        iassert(pAnimTree);
        obj = Com_GetServerDObj(ent->s.number);
        if ( obj )
        {
            cmdIndex = 0;
            XAnimSetCompleteGoalWeight(obj, scripted->anim, 0.0f, blendTime, 1.0f, 0, 0, 0, cmdIndex);
        }
    }
    MT_Free((unsigned char *)scripted, sizeof(animscripted_s), SCRIPTINSTANCE_SERVER);
    ent->scripted = 0;
}
