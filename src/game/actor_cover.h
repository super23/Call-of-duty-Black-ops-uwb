#pragma once

#include "actor.h"
#include "pathnode.h"

// Decomp: CoDSPBlackOps.map.c — actor_cover.cpp

struct CoverRange // sizeof=0x1C
{
    float distSq;
    float minAngle;
    float maxAngle;
    float minNodeAngle;
    float maxNodeAngle;
    float minOffset;
    float maxOffset;
};

struct CoverMetric
{
    pathnode_t *node;
    float score;
    float distSq;
};

bool __fastcall Actor_Cover_IsWithinAngle(
    const float *angles,
    const float *nodeAngles,
    const CoverRange *range,
    float minOffset,
    float maxOffset);
bool __fastcall Actor_Cover_IsAngleSweet(const float *angles, const float *nodeAngles, const CoverRange *range);
bool __fastcall Actor_Cover_IsAngleFlanked(const float *angles, const float *nodeAngles, const CoverRange *range);
bool __fastcall Actor_Cover_IsAngleValid(const float *angles, const float *nodeAngles, const CoverRange *range);
bool __fastcall Actor_Cover_NodeRangeValid(const CoverRange *rangeOut, const float *nodeAngles, const CoverRange *range);
void __fastcall Actor_Cover_InitRange(CoverRange *rangeOut, const pathnode_t *node);
scr_animscript_t *__cdecl Actor_Cover_PickAttackScript(actor_s *self, pathnode_t *node);
scr_animscript_t *__fastcall Actor_Cover_GetAttackScript(actor_s *self, pathnode_t *node);
bool __fastcall Actor_Cover_CheckWithEnemy(actor_s *self, pathnode_t *node, sentient_s *enemy);
float __fastcall Actor_Cover_GetNodeMetric(actor_s *self, pathnode_t *node, sentient_s *enemy, const CoverRange *range);
float __fastcall Actor_Cover_ScoreOnEngagement(actor_s *self, pathnode_t *node, sentient_s *enemy, const CoverRange *range);
float __fastcall Actor_Cover_ScoreOnNodeAngle(actor_s *self, pathnode_t *node, sentient_s *enemy, const CoverRange *range);
float __fastcall Actor_Cover_ScoreOnTargetDir(actor_s *self, pathnode_t *node, sentient_s *enemy);
float __fastcall Actor_Cover_ScoreOnPriority(pathnode_t *node);
float __fastcall Actor_Cover_ScoreOnPlayerLOS(actor_s *self, pathnode_t *node);
float __fastcall Actor_Cover_ScoreOnVisibility(actor_s *self, pathnode_t *node, sentient_s *enemy);
float __fastcall Actor_Cover_ScoreOnCoverType(pathnode_t *node);
float __fastcall Actor_Cover_ScoreOnFlanking(actor_s *self, pathnode_t *node, sentient_s *enemy);
float __fastcall Actor_Cover_GetNodeDistMetric(actor_s *self, pathnode_t *node);
float __fastcall Actor_Cover_ScoreOnDistance(actor_s *self, pathnode_t *node, sentient_s *enemy);
bool __fastcall Actor_Cover_IsValidReacquire(actor_s *self, pathnode_t *node);
bool __fastcall Actor_Cover_IsValidCover(actor_s *self, pathnode_t *node, sentient_s *enemy, const CoverRange *range);
bool __fastcall Actor_Cover_IsValidCoverDir(actor_s *self, pathnode_t *node, const float *dir);
pathnode_t *__fastcall Actor_Cover_FindCoverFromPoint(
    actor_s *self,
    const float *fromPoint,
    sentient_s *enemy,
    const CoverRange *range);
float __fastcall Actor_Cover_FromPoint_GetNodeMetric(
    actor_s *self,
    pathnode_t *node,
    const float *fromPoint,
    sentient_s *enemy,
    const CoverRange *range);
float __cdecl Actor_Cover_MinHeightAtCover(pathnode_t *node);
pathnode_t *__fastcall Actor_Cover_FindBestCover(actor_s *self, sentient_s *enemy);
pathnode_t *__fastcall Actor_Cover_FindBestCoverList(
    actor_s *self,
    sentient_s *enemy,
    pathnode_t **nodeList,
    int nodeCount);
pathnode_t *__fastcall Actor_Cover_FindBestCoverListInList(
    actor_s *self,
    sentient_s *enemy,
    pathnode_t **nodeList,
    int nodeCount);
pathnode_t *__fastcall Actor_Cover_FindCoverNode(actor_s *self, sentient_s *enemy);
pathnode_t *__fastcall Actor_Cover_GetCoverNode(actor_s *self, sentient_s *enemy);
bool __fastcall Actor_Cover_UseCoverNode(actor_s *self, pathnode_t *node);
void __fastcall Actor_CheckCoverLeave(actor_s *self);
#ifdef KISAK_SP
bool __fastcall Actor_CheckCoverExitPosWithPath(actor_s *self, const float *exitPos);
#endif
void __fastcall Actor_CheckCoverApproach(actor_s *self);
void __fastcall Actor_CoverApproachNotify(actor_s *self);
bool __fastcall Actor_CoverArrival_Start(actor_s *self, ai_state_t ePrevState);
bool __fastcall Actor_CoverArrival_Resume(actor_s *self, ai_state_t ePrevState);
actor_think_result_t __fastcall Actor_CoverArrival_Think(actor_s *self);
