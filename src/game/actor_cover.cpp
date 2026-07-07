#include "actor_cover.h"

#include "actor_animapi.h"
#include "actor_badplace.h"
#include "actor_navigation.h"
#include "actor_senses.h"
#include "sentient.h"
#include "actor_threat.h"
#include <clientscript/cscr_vm.h>
#include <clientscript/scr_const.h>
#include <game/g_bsp.h>
#include "actor_orientation.h"
#include "actor_state.h"
#include "actor_exposed.h"
#include "teams.h"

#include <game/actor_wrapper.h>
#include <game/g_main_wrapper.h>
#include <game/g_spawn_wrapper.h>
#include <universal/assertive.h>

// Decomp: CoDSPBlackOps.map.c

static float Actor_Cover_NormalizeAngle(float angle)
{
    while ( angle > 180.0f )
        angle -= 360.0f;
    while ( angle < -180.0f )
        angle += 360.0f;
    return angle;
}

bool __fastcall Actor_Cover_IsWithinAngle(
    const float *angles,
    const float *nodeAngles,
    const CoverRange *range,
    float minOffset,
    float maxOffset)
{
    float delta;

    iassert(angles);
    iassert(nodeAngles);
    iassert(range);
    delta = Actor_Cover_NormalizeAngle(angles[1] - nodeAngles[1]);
    return delta >= range->minAngle + minOffset && delta <= range->maxAngle + maxOffset;
}

bool __fastcall Actor_Cover_IsAngleSweet(const float *angles, const float *nodeAngles, const CoverRange *range)
{
    iassert(range);
    return Actor_Cover_IsWithinAngle(angles, nodeAngles, range, range->minNodeAngle, range->maxNodeAngle);
}

bool __fastcall Actor_Cover_IsAngleFlanked(const float *angles, const float *nodeAngles, const CoverRange *range)
{
    float delta;

    iassert(angles);
    iassert(nodeAngles);
    iassert(range);
    delta = Actor_Cover_NormalizeAngle(angles[1] - nodeAngles[1]);
    return delta < range->minNodeAngle || delta > range->maxNodeAngle;
}

bool __fastcall Actor_Cover_IsAngleValid(const float *angles, const float *nodeAngles, const CoverRange *range)
{
    float delta;

    iassert(angles);
    iassert(nodeAngles);
    iassert(range);
    delta = Actor_Cover_NormalizeAngle(angles[1] - nodeAngles[1]);
    return delta >= range->minAngle && delta <= range->maxAngle;
}

bool __fastcall Actor_Cover_NodeRangeValid(const CoverRange *rangeOut, const float *nodeAngles, const CoverRange *range)
{
    float angles[3];

    iassert(rangeOut);
    iassert(nodeAngles);
    iassert(range);
    angles[0] = 0.0f;
    angles[1] = rangeOut->minAngle;
    angles[2] = 0.0f;
    return Actor_Cover_IsAngleValid(angles, nodeAngles, range);
}

void __fastcall Actor_Cover_InitRange(CoverRange *rangeOut, const pathnode_t *node)
{
    iassert(rangeOut);
    iassert(node);
    switch ( node->constant.type )
    {
    case NODE_COVER_CROUCH:
    case NODE_COVER_CROUCH_WINDOW:
    case NODE_COVER_STAND:
    case NODE_COVER_PILLAR:
        rangeOut->minAngle = -45.0f;
        rangeOut->maxAngle = 45.0f;
        rangeOut->minNodeAngle = -35.0f;
        rangeOut->maxNodeAngle = 35.0f;
        rangeOut->minOffset = -25.0f;
        rangeOut->maxOffset = 25.0f;
        rangeOut->distSq = 81225.0f;
        break;
    case NODE_COVER_PRONE:
        rangeOut->minAngle = -30.0f;
        rangeOut->maxAngle = 10.0f;
        rangeOut->minNodeAngle = -20.0f;
        rangeOut->maxNodeAngle = 5.0f;
        rangeOut->minOffset = -15.0f;
        rangeOut->maxOffset = 1.0f;
        rangeOut->distSq = 81225.0f;
        break;
    case NODE_COVER_LEFT:
    case NODE_COVER_RIGHT:
        rangeOut->minAngle = -30.0f;
        rangeOut->maxAngle = 30.0f;
        rangeOut->minNodeAngle = -20.0f;
        rangeOut->maxNodeAngle = 20.0f;
        rangeOut->minOffset = -15.0f;
        rangeOut->maxOffset = 15.0f;
        rangeOut->distSq = 640000.0f;
        break;
    case NODE_COVER_WIDE_LEFT:
    case NODE_COVER_WIDE_RIGHT:
        rangeOut->minAngle = -12.0f;
        rangeOut->maxAngle = 60.0f;
        rangeOut->minNodeAngle = -5.0f;
        rangeOut->maxNodeAngle = 35.0f;
        rangeOut->minOffset = 1.0f;
        rangeOut->maxOffset = 10.0f;
        rangeOut->distSq = 81225.0f;
        break;
    default:
        rangeOut->minAngle = -55.0f;
        rangeOut->maxAngle = 55.0f;
        rangeOut->minNodeAngle = -35.0f;
        rangeOut->maxNodeAngle = 35.0f;
        rangeOut->minOffset = -25.0f;
        rangeOut->maxOffset = 25.0f;
        rangeOut->distSq = 0.0f;
        break;
    }
}

scr_animscript_t *__cdecl Actor_Cover_PickAttackScript(actor_s *self, pathnode_t *node)
{
    return Actor_Cover_GetAttackScript(self, node);
}

scr_animscript_t *__fastcall Actor_Cover_GetAttackScript(actor_s *self, pathnode_t *node)
{
    AnimScriptList *scripts;

    iassert(self);
    iassert(node);
    scripts = g_animScriptTable[self->species];
    switch ( node->constant.type )
    {
    case NODE_COVER_CROUCH:
    case NODE_COVER_CROUCH_WINDOW:
        return &scripts->cover_crouch;
    case NODE_COVER_PRONE:
        return &scripts->cover_prone;
    case NODE_COVER_LEFT:
    case NODE_COVER_WIDE_LEFT:
        return &scripts->cover_left;
    case NODE_COVER_RIGHT:
    case NODE_COVER_WIDE_RIGHT:
        return &scripts->cover_right;
    default:
        return &scripts->cover_stand;
    }
}

bool __fastcall Actor_Cover_CheckWithEnemy(actor_s *self, pathnode_t *node, sentient_s *enemy)
{
    float enemyPos[3];
    float dir[3];
    float angles[3];
    float nodeAngles[3];
    CoverRange range;

    iassert(self);
    iassert(node);
    if ( !enemy )
        return true;
    Sentient_GetOrigin(enemy, enemyPos);
    Vec3Sub(enemyPos, node->constant.vOrigin, dir);
    vectoangles(dir, angles);
    nodeAngles[0] = 0.0f;
    nodeAngles[1] = node->constant.fAngle;
    nodeAngles[2] = 0.0f;
    Actor_Cover_InitRange(&range, node);
    return Actor_Cover_IsAngleValid(angles, nodeAngles, &range);
}

float __fastcall Actor_Cover_GetNodeDistMetric(actor_s *self, pathnode_t *node)
{
    float actorPos[3];
    float nodePos[3];

    iassert(self);
    iassert(node);
    Sentient_GetOrigin(self->sentient, actorPos);
    nodePos[0] = node->constant.vOrigin[0];
    nodePos[1] = node->constant.vOrigin[1];
    nodePos[2] = node->constant.vOrigin[2];
    return Vec3DistanceSq(actorPos, nodePos);
}

float __fastcall Actor_Cover_ScoreOnDistance(actor_s *self, pathnode_t *node, sentient_s *enemy)
{
    float distSq;

    (void)enemy;
    iassert(self);
    iassert(node);
    distSq = Vec3DistanceSq(node->constant.vOrigin, self->ent->r.currentOrigin);
    if ( distSq > 1440000.0f )
        distSq = 1440000.0f;
    if ( distSq < 0.0f )
        distSq = 0.0f;
    return 1.0f - (distSq / 1440000.0f);
}

float __fastcall Actor_Cover_ScoreOnPriority(pathnode_t *node)
{
    iassert(node);
    return (float)node->constant.type;
}

float __fastcall Actor_Cover_ScoreOnPlayerLOS(actor_s *self, pathnode_t *node)
{
    iassert(self);
    iassert(node);
    iassert(self->sentient);
    if ( node->dynamic.inPlayerLOSTime >= level.time && self->sentient->eTeam == TEAM_ALLIES )
        return 0.0f;
    return 1.0f;
}

float __fastcall Actor_Cover_ScoreOnVisibility(actor_s *self, pathnode_t *node, sentient_s *enemy)
{
    pathnode_t *enemyNode;

    (void)enemy;
    iassert(self);
    iassert(node);
    enemy = Actor_GetTargetSentient(self);
    if ( enemy
        && (enemyNode = Sentient_NearestNode(enemy)) != nullptr
        && !Path_NodesVisible(node, enemyNode) )
    {
        return 0.0f;
    }
    return 1.0f;
}

float __fastcall Actor_Cover_ScoreOnCoverType(pathnode_t *node)
{
    iassert(node);
    return Path_IsCoverNode(node) ? 1.0f : 0.0f;
}

float __fastcall Actor_Cover_ScoreOnFlanking(actor_s *self, pathnode_t *node, sentient_s *enemy)
{
    gentity_s *targetEnt;
    float targetPos[3];

    (void)enemy;
    iassert(self);
    iassert(node);
    targetEnt = Actor_GetTargetEntity(self);
    if ( !targetEnt )
        return 0.0f;
    targetPos[0] = targetEnt->r.currentOrigin[0];
    targetPos[1] = targetEnt->r.currentOrigin[1];
    targetPos[2] = targetEnt->r.currentOrigin[2];
    if ( IsSolidProtectingTgtFromPt(targetPos, node->constant.vOrigin, 45.0f, 32.0f, 4.0f) )
        return 0.0f;
    return 1.0f;
}

float __fastcall Actor_Cover_ScoreOnTargetDir(actor_s *self, pathnode_t *node, sentient_s *enemy)
{
    gentity_s *targetEnt;
    float actorToNode[2];
    float targetToNode[2];
    float actorLen;
    float targetLen;

    (void)enemy;
    iassert(self);
    iassert(node);
    targetEnt = Actor_GetTargetEntity(self);
    if ( !targetEnt )
        return 0.0f;
    if ( Actor_PointNearNode(self->ent->r.currentOrigin, node) )
        return 1.0f;
    actorToNode[0] = node->constant.vOrigin[0] - self->ent->r.currentOrigin[0];
    actorToNode[1] = node->constant.vOrigin[1] - self->ent->r.currentOrigin[1];
    targetToNode[0] = node->constant.vOrigin[0] - targetEnt->r.currentOrigin[0];
    targetToNode[1] = node->constant.vOrigin[1] - targetEnt->r.currentOrigin[1];
    actorLen = Vec2Length(actorToNode);
    targetLen = Vec2Length(targetToNode);
    if ( actorLen <= 0.0f || targetLen <= 0.0f )
        return 0.0f;
    return ((actorToNode[0] / actorLen) * (targetToNode[0] / targetLen)
            + (actorToNode[1] / actorLen) * (targetToNode[1] / targetLen)
            + 1.0f)
        * 0.5f;
}

static bool Actor_Cover_IsAngleValidFromPoint(
    const float *point,
    pathnode_t *node,
    const CoverRange *range)
{
    float dir[3];
    float angles[3];
    float nodeAngles[3];

    Vec3Sub(point, node->constant.vOrigin, dir);
    vectoangles(dir, angles);
    nodeAngles[0] = 0.0f;
    nodeAngles[1] = node->constant.fAngle;
    nodeAngles[2] = 0.0f;
    return Actor_Cover_IsAngleValid(angles, nodeAngles, range);
}

float __fastcall Actor_Cover_ScoreOnNodeAngle(actor_s *self, pathnode_t *node, sentient_s *enemy, const CoverRange *range)
{
    float threatDir[2];
    float threatPoint[3];
    gentity_s *targetEnt;
    bool hasPotentialThreat;
    CoverRange localRange;

    (void)enemy;
    iassert(self);
    iassert(node);
    if ( !range )
    {
        Actor_Cover_InitRange(&localRange, node);
        range = &localRange;
    }
    hasPotentialThreat = Actor_GetPotentialThreat(&self->potentialThreat, threatDir) != 0;
    targetEnt = Actor_GetTargetEntity(self);
    if ( targetEnt && (Actor_CanSeeEnemy(self) || !hasPotentialThreat) )
    {
        if ( Actor_Cover_IsAngleValidFromPoint(targetEnt->r.currentOrigin, node, range) )
            return 1.0f;
    }
    else if ( hasPotentialThreat )
    {
        threatPoint[0] = node->constant.vOrigin[0] + threatDir[0];
        threatPoint[1] = node->constant.vOrigin[1] + threatDir[1];
        threatPoint[2] = node->constant.vOrigin[2];
        if ( Actor_Cover_IsAngleValidFromPoint(threatPoint, node, range) )
            return 1.0f;
    }
    return 0.0f;
}

static void Actor_Cover_GetEngageRange(
    const actor_s *self,
    float *engageMinDist,
    float *engageMinFalloffDist,
    float *engageMaxDist,
    float *engageMaxFalloffDist)
{
    float fightDist;
    float maxDist;

    fightDist = self->pathEnemyFightDist > 0.0f ? self->pathEnemyFightDist : 512.0f;
    maxDist = self->pathEnemyLookahead > fightDist ? self->pathEnemyLookahead : fightDist * 2.0f;
    *engageMinFalloffDist = fightDist * 0.5f;
    *engageMinDist = fightDist;
    *engageMaxDist = maxDist;
    *engageMaxFalloffDist = maxDist * 1.5f;
}

float __fastcall Actor_Cover_ScoreOnEngagement(actor_s *self, pathnode_t *node, sentient_s *enemy, const CoverRange *range)
{
    gentity_s *targetEnt;
    float engageMinDist;
    float engageMinFalloffDist;
    float engageMaxDist;
    float engageMaxFalloffDist;
    float delta[3];
    float dist;
    float score;

    (void)range;
    (void)enemy;
    iassert(self);
    iassert(node);
    targetEnt = Actor_GetTargetEntity(self);
    if ( !targetEnt )
        return 0.0f;
    Actor_Cover_GetEngageRange(self, &engageMinDist, &engageMinFalloffDist, &engageMaxDist, &engageMaxFalloffDist);
    delta[0] = targetEnt->r.currentOrigin[0] - node->constant.vOrigin[0];
    delta[1] = targetEnt->r.currentOrigin[1] - node->constant.vOrigin[1];
    delta[2] = targetEnt->r.currentOrigin[2] - node->constant.vOrigin[2];
    dist = Vec3Length(delta);
    if ( dist < engageMinFalloffDist || dist > engageMaxFalloffDist )
        return 0.0f;
    if ( dist < engageMinDist )
    {
        if ( dist >= engageMinFalloffDist )
        {
            score = 1.0f - (engageMinDist - dist) / (engageMinDist - engageMinFalloffDist);
            iassert(score >= 0.0f && score <= 1.0f);
            return score;
        }
        return 0.0f;
    }
    if ( dist <= engageMaxDist )
        return 1.0f;
    if ( dist > engageMaxFalloffDist )
        return 0.0f;
    score = (engageMaxFalloffDist - dist) / (engageMaxFalloffDist - engageMaxDist);
    iassert(score >= 0.0f && score <= 1.0f);
    return score;
}

float __fastcall Actor_Cover_GetNodeMetric(actor_s *self, pathnode_t *node, sentient_s *enemy, const CoverRange *range)
{
    CoverRange localRange;
    float metric;
    float nodeAngleScore;

    iassert(self);
    iassert(self->sentient);
    iassert(node);
    if ( !range )
    {
        Actor_Cover_InitRange(&localRange, node);
        range = &localRange;
    }
    nodeAngleScore = Actor_Cover_ScoreOnNodeAngle(self, node, enemy, range);
    metric = Actor_Cover_GetNodeDistMetric(self, node);
    metric += Actor_Cover_ScoreOnEngagement(self, node, enemy, range);
    metric += Actor_Cover_ScoreOnVisibility(self, node, enemy);
    metric += Actor_Cover_ScoreOnCoverType(node);
    metric += nodeAngleScore;
    metric += Actor_Cover_ScoreOnTargetDir(self, node, enemy);
    metric += Actor_Cover_ScoreOnPlayerLOS(self, node);
    metric += Actor_Cover_ScoreOnPriority(node);
    if ( nodeAngleScore > 0.0f )
        metric += Actor_Cover_ScoreOnFlanking(self, node, enemy);
    return metric;
}

bool __fastcall Actor_Cover_IsValidReacquire(actor_s *self, pathnode_t *node)
{
    iassert(self);
    iassert(node);
    return Path_IsCoverNode(node) && Path_IsValidClaimNode(node);
}

bool __fastcall Actor_Cover_IsValidCoverDir(actor_s *self, pathnode_t *node, const float *dir)
{
    float angles[3];
    float nodeAngles[3];
    CoverRange range;

    (void)self;
    iassert(node);
    iassert(dir);
    Actor_Cover_InitRange(&range, node);
    vectoangles(dir, angles);
    nodeAngles[0] = 0.0f;
    nodeAngles[1] = node->constant.fAngle;
    nodeAngles[2] = 0.0f;
    return Actor_Cover_IsAngleValid(angles, nodeAngles, &range);
}

bool __fastcall Actor_Cover_IsValidCover(actor_s *self, pathnode_t *node, sentient_s *enemy, const CoverRange *range)
{
    CoverRange localRange;

    iassert(self);
    iassert(node);
    if ( !Path_IsCoverNode(node) )
        return false;
    if ( !range )
    {
        Actor_Cover_InitRange(&localRange, node);
        range = &localRange;
    }
    if ( enemy && !Actor_Cover_CheckWithEnemy(self, node, enemy) )
        return false;
    return Actor_Cover_GetNodeMetric(self, node, enemy, range) > 0.0f;
}

float __fastcall Actor_Cover_FromPoint_GetNodeMetric(
    actor_s *self,
    pathnode_t *node,
    const float *fromPoint,
    sentient_s *enemy,
    const CoverRange *range)
{
    float savedOrigin[3];
    float metric;

    iassert(self);
    iassert(node);
    iassert(fromPoint);
    Sentient_GetOrigin(self->sentient, savedOrigin);
    self->sentient->ent->r.currentOrigin[0] = fromPoint[0];
    self->sentient->ent->r.currentOrigin[1] = fromPoint[1];
    self->sentient->ent->r.currentOrigin[2] = fromPoint[2];
    metric = Actor_Cover_GetNodeMetric(self, node, enemy, range);
    self->sentient->ent->r.currentOrigin[0] = savedOrigin[0];
    self->sentient->ent->r.currentOrigin[1] = savedOrigin[1];
    self->sentient->ent->r.currentOrigin[2] = savedOrigin[2];
    return metric;
}

pathnode_t *__fastcall Actor_Cover_FindCoverFromPoint(
    actor_s *self,
    const float *fromPoint,
    sentient_s *enemy,
    const CoverRange *range)
{
    pathnode_t *bestNode = nullptr;
    float bestScore = -999999.0f;
    int i;

    iassert(self);
    iassert(fromPoint);
    for ( i = 0; i < gameWorldCurrent->path.nodeCount; ++i )
    {
        pathnode_t *node = &gameWorldCurrent->path.nodes[i];
        float score;

        if ( !Path_IsCoverNode(node) )
            continue;
        score = Actor_Cover_FromPoint_GetNodeMetric(self, node, fromPoint, enemy, range);
        if ( score > bestScore )
        {
            bestScore = score;
            bestNode = node;
        }
    }
    return bestNode;
}

float __cdecl Actor_Cover_MinHeightAtCover(pathnode_t *node)
{
    iassert(node);
    return node->constant.vOrigin[2];
}

pathnode_t *__fastcall Actor_Cover_FindBestCoverListInList(
    actor_s *self,
    sentient_s *enemy,
    pathnode_t **nodeList,
    int nodeCount)
{
    pathnode_t *bestNode = nullptr;
    float bestScore = -999999.0f;
    int i;

    iassert(self);
    iassert(nodeList);
    for ( i = 0; i < nodeCount; ++i )
    {
        float score = Actor_Cover_GetNodeMetric(self, nodeList[i], enemy, nullptr);
        if ( score > bestScore )
        {
            bestScore = score;
            bestNode = nodeList[i];
        }
    }
    return bestNode;
}

pathnode_t *__fastcall Actor_Cover_FindBestCoverList(
    actor_s *self,
    sentient_s *enemy,
    pathnode_t **nodeList,
    int nodeCount)
{
    return Actor_Cover_FindBestCoverListInList(self, enemy, nodeList, nodeCount);
}

pathnode_t *__fastcall Actor_Cover_FindBestCover(actor_s *self, sentient_s *enemy)
{
    return Actor_Cover_FindCoverNode(self, enemy);
}

pathnode_t *__fastcall Actor_Cover_FindCoverNode(actor_s *self, sentient_s *enemy)
{
    iassert(self);
    if ( self->iPotentialCoverNodeCount > 0 )
        return Actor_Cover_FindBestCoverList(
            self,
            enemy,
            self->pPotentialCoverNode,
            self->iPotentialCoverNodeCount);
    return Sentient_NearestNode(self->sentient);
}

pathnode_t *__fastcall Actor_Cover_GetCoverNode(actor_s *self, sentient_s *enemy)
{
    pathnode_t *node;

    iassert(self);
    node = Actor_Cover_FindCoverNode(self, enemy);
    if ( node && Actor_Cover_IsValidCover(self, node, enemy, nullptr) )
        return node;
    return nullptr;
}

bool __fastcall Actor_Cover_UseCoverNode(actor_s *self, pathnode_t *node)
{
    scr_animscript_t *attackScript;

    iassert(self);
    iassert(self->sentient);
    iassert(node);
    attackScript = Actor_Cover_GetAttackScript(self, node);
    if ( !attackScript )
        return false;
    Actor_SetAnimScript(self, attackScript, 0, AI_ANIM_USE_BOTH_DELTAS, AI_ANIM_FUNCTION_STOP);
    self->sentient->pClaimedNode = node;
    return true;
}

void __fastcall Actor_CheckCoverLeave(actor_s *self)
{
    pathnode_t *node;

    iassert(self);
    node = self->sentient->pClaimedNode;
    if ( !node )
        return;
    if ( !Actor_IsDoingCover(self) )
        self->sentient->pClaimedNode = nullptr;
}

#ifdef KISAK_SP
// Decomp: CoDSP_rdBlackOps.map.c (Actor_CheckCoverLeave ~82569240 path variant)
bool __fastcall Actor_CheckCoverExitPosWithPath(actor_s *self, const float *exitPos)
{
    float actorOrigin[2];
    float delta[2];
    float pathDelta[2];
    float dot;
    int negativeDots;
    int pathLen;
    int startIndex;
    int i;
    float distSq;

    iassert(self);
    iassert(self->ent);
    iassert(exitPos);
    if ( !Actor_HasPath(self) )
        return false;

    actorOrigin[0] = self->ent->r.currentOrigin[0];
    actorOrigin[1] = self->ent->r.currentOrigin[1];
    delta[0] = exitPos[0] - actorOrigin[0];
    delta[1] = exitPos[1] - actorOrigin[1];
    if ( delta[0] * delta[0] + delta[1] * delta[1] >= Square(128.0f) )
        return false;

    pathLen = self->Path.wPathLen;
    startIndex = self->TrimInfo.iIndex;
    if ( pathLen - 3 < startIndex )
        startIndex = pathLen - 3;

    negativeDots = 0;
    for ( i = pathLen - 2; ; --i )
    {
        if ( i < startIndex )
            return true;

        pathDelta[0] = self->Path.pts[i].vOrigPoint[0] - actorOrigin[0];
        pathDelta[1] = self->Path.pts[i].vOrigPoint[1] - actorOrigin[1];
        dot = pathDelta[0] * delta[0] + pathDelta[1] * delta[1];
        if ( dot < 0.0f && ++negativeDots > 1 )
            return false;

        distSq = pathDelta[0] * pathDelta[0] + pathDelta[1] * pathDelta[1];
        if ( distSq > Square(128.0f) )
            break;
    }

    return i != pathLen - 2 || negativeDots == 0;
}
#endif

void __fastcall Actor_CheckCoverApproach(actor_s *self)
{
    iassert(self);
    if ( self->eState[self->stateLevel] == AIS_COVERARRIVAL && !Actor_IsAnimScriptAlive(self) )
        Actor_SetState(self, AIS_EXPOSED);
}

void __fastcall Actor_CoverApproachNotify(actor_s *self)
{
    iassert(self);
    if ( self->arrivalInfo.arrivalNotifyRequested )
    {
        Scr_Notify(self->ent, scr_const.runto_arrived, 0);
        self->arrivalInfo.arrivalNotifyRequested = 0;
    }
}

bool __fastcall Actor_CoverArrival_Start(actor_s *self, ai_state_t ePrevState)
{
    (void)ePrevState;
    iassert(self);
    Actor_SetOrientMode(self, AI_ORIENT_DONT_CHANGE);
    self->eAnimMode = AI_ANIM_USE_BOTH_DELTAS;
    self->bUseGoalWeight = true;
    return true;
}

bool __fastcall Actor_CoverArrival_Resume(actor_s *self, ai_state_t ePrevState)
{
    return Actor_CoverArrival_Start(self, ePrevState);
}

actor_think_result_t __fastcall Actor_CoverArrival_Think(actor_s *self)
{
    iassert(self);
    iassert(self->bUseGoalWeight);
    Actor_SetAnimScript(
        self,
        &g_animScriptTable[self->species]->cover_arrival,
        0,
        self->eAnimMode,
        AI_ANIM_FUNCTION_MOVE);
    self->safeToChangeScript = false;
    if ( Actor_IsAnimScriptAlive(self) )
    {
        self->pszDebugInfo = "cover_arrival";
        Actor_PreThink(self);
        Actor_UpdateOriginAndAngles(self);
        return ACTOR_THINK_REPEAT;
    }
    Actor_SetState(self, AIS_EXPOSED);
    Actor_SetAnimScript(self, Actor_GetStopAnim(self), 0, AI_ANIM_MOVE_CODE, AI_ANIM_FUNCTION_STOP);
    Actor_ClearPath(self);
    return ACTOR_THINK_DONE;
}
