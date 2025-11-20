#pragma once

double __fastcall Path_GetPathDir(float *delta, const float *vFrom, const float *vTo);
pathnode_t *__fastcall Path_GetNegotiationNode(const path_t *pPath);
void __fastcall Path_IncrementNodeUserCount(path_t *pPath);
void __fastcall Path_AddTrimmedAmount(path_t *pPath, const float *vStartPos);
double __fastcall Path_GetDistToPathSegment(const float *vStartPos, const pathpoint_t *pt);
pathnode_t *__fastcall Path_FindCloseNode(
        team_t eTeam,
        pathnode_t *pNodeFrom,
        const float *vGoalPos,
        int bAllowNegotiationLinks);
int __fastcall Path_FindPath(
        path_t *pPath,
        team_t eTeam,
        const float *vStartPos,
        const float *vGoalPos,
        int bAllowNegotiationLinks);
int __fastcall Path_FindPathFrom(
        path_t *pPath,
        team_t eTeam,
        pathnode_t *pNodeFrom,
        const float *vStartPos,
        const float *vGoalPos,
        int bAllowNegotiationLinks,
        int bIgnoreBadplaces);
int __fastcall Path_FindPathFromTo(
        path_t *pPath,
        team_t eTeam,
        pathnode_t *pNodeFrom,
        const float *vStartPos,
        pathnode_t *pNodeTo,
        const float *vGoalPos,
        int bAllowNegotiationLinks,
        int bIgnoreBadplaces);
int __fastcall Path_FindPathWithWidth(
        path_t *pPath,
        team_t eTeam,
        const float *vStartPos,
        const float *vGoalPos,
        int bAllowNegotiationLinks,
        float width,
        float *perp);
int __fastcall Path_FindPathFromToWithWidth(
        path_t *pPath,
        team_t eTeam,
        pathnode_t *pNodeFrom,
        const float *vStartPos,
        pathnode_t *pNodeTo,
        const float *vGoalPos,
        int bAllowNegotiationLinks,
        float width,
        float *perp);
int __fastcall Path_FindPathFromNotCrossPlanes(
        path_t *pPath,
        team_t eTeam,
        pathnode_t *pNodeFrom,
        const float *vStartPos,
        const float *vGoalPos,
        float (*vNormal)[2],
        float *fDist,
        int iPlaneCount,
        int bAllowNegotiationLinks,
        int bIgnoreBadplaces);
int __fastcall Path_FindPathFromToNotCrossPlanes(
        path_t *pPath,
        team_t eTeam,
        pathnode_t *pNodeFrom,
        const float *vStartPos,
        pathnode_t *pNodeTo,
        const float *vGoalPos,
        float (*vNormal)[2],
        float *fDist,
        int iPlaneCount,
        int bAllowNegotiationLinks,
        int bIgnoreBadplaces);
char __thiscall CustomSearchInfo_FindPathNotCrossPlanes::IgnoreNode(
        CustomSearchInfo_FindPathNotCrossPlanes *this,
        pathnode_t *pNode);
int __fastcall Path_GeneratePath(
        path_t *pPath,
        team_t eTeam,
        const float *vStartPos,
        const float *vGoalPos,
        pathnode_t *pNodeFrom,
        pathnode_t *pNodeTo,
        int bIncludeGoalPos,
        int bAllowNegotiationLinks);
void __fastcall Path_TransferLookahead(path_t *pPath, const float *vStartPos);
void __fastcall Path_SetLookaheadToStart(path_t *pPath, const float *vStartPos, int bTrimAmount);
int __fastcall Path_FindPathInCylinderWithLOS(
        path_t *pPath,
        team_t eTeam,
        const float *vStartPos,
        const float *vGoalPos,
        const actor_goal_s *goal,
        float fWithinDistSqrd,
        int bAllowNegotiationLinks);
const pathnode_t *__fastcall Path_FindFacingNode(sentient_s *pSelf, sentient_s *pOther, sentient_info_t *pInfo);
int __fastcall Path_FindPathGetCloseAsPossible(
        path_t *pPath,
        team_t eTeam,
        pathnode_t *pNodeFrom,
        const float *vStartPos,
        pathnode_t *pNodeTo,
        const float *vGoalPos,
        int bAllowNegotiationLinks);
int __fastcall Path_ClipToGoal(path_t *pPath, const actor_goal_s *goal);
void __cdecl Path_TrimLastNodes(path_t *pPath, int iNodeCount, bool bMaintainGoalPos);
void __fastcall Path_DecrementNodeUserCount(path_t *pPath);
void __fastcall Path_BeginTrim(path_t *pPath, path_trim_t *pTrim);
int __fastcall Path_TrimToSeePoint(
        path_t *pPath,
        path_trim_t *pTrim,
        actor_s *pActor,
        float fMaxDistSqrd,
        int iIgnoreEntityNum,
        const float *vPoint);
void __fastcall Path_Begin(path_t *pPath);
void __fastcall Path_Clear(path_t *pPath);
bool __fastcall Path_Exists(const path_t *pPath);
bool __fastcall Path_CompleteLookahead(const path_t *pPath);
bool __fastcall Path_AttemptedCompleteLookahead(const path_t *pPath);
bool __fastcall Path_UsesObstacleNegotiation(const path_t *pPath);
bool __fastcall Path_HasNegotiationNode(const path_t *path);
bool __fastcall Path_AllowsObstacleNegotiation(const path_t *pPath);
void __fastcall Path_GetObstacleNegotiationScript(const path_t *pPath, scr_animscript_t *animscript);
bool __fastcall Path_NeedsReevaluation(const path_t *pPath);
int __fastcall Path_DistanceGreaterThan(path_t *pPath, float fDist);
bool __fastcall Path_FailedLookahead(path_t *pPath);
int __fastcall Path_PredictionTraceCheckForEntities(
        float *vStartPos,
        float *vEndPos,
        const int *entities,
        int entityCount,
        int entityIgnore,
        int mask,
        float *vTraceEndPos);
bool __fastcall Path_PredictionTrace(
        float *vStartPos,
        float *vEndPos,
        int entityIgnore,
        int mask,
        float *vTraceEndPos,
        float stepheight,
        int allowStartSolid);
bool __fastcall Path_LookaheadPredictionTrace(path_t *pPath, float *vStartPos, float *vEndPos);
int __fastcall Path_IsTrimmed(path_t *pPath);
int __fastcall Path_GetForwardStartPos(path_t *pPath, const float *vStartPos, float *vForwardStartPos);
void __fastcall Path_UpdateLookahead_NonCodeMove(path_t *pPath, const float *vPrevPos, const float *vStartPos);
void __fastcall Path_RemoveCompletedPathPoints(path_t *pPath, __int16 pathPointIndex);
void __fastcall Path_UpdateLookahead(
        path_t *pPath,
        const float *vStartPos,
        int bReduceLookaheadAmount,
        int bTrimAmount,
        int bAllowBacktrack,
        int bAllowRestore);
void __fastcall Path_SubtractTrimmedAmount(path_t *pPath, const float *vStartPos);
void __fastcall Path_TrimCompletedPath(path_t *pPath, const float *vStartPos);
void __fastcall Path_BacktrackCompletedPath(path_t *pPath, const float *vStartPos);
void __fastcall Path_CalcLookahead(
        path_t *pPath,
        const float *vStartPos,
        int bReduceLookaheadAmount,
        int bAllowRestore);
void __fastcall Path_UpdateLookaheadAmount(
        path_t *pPath,
        float *vStartPos,
        float *vLookaheadPos,
        int bReduceLookaheadAmount,
        float dist,
        int lookaheadNextNode,
        float maxLookaheadAmountIfReduce,
        int bAllowRestore);
void __fastcall Path_ReduceLookaheadAmount(path_t *pPath, float maxLookaheadAmountIfReduce);
void __fastcall Path_IncreaseLookaheadAmount(path_t *pPath);
void __fastcall Path_CalcLookahead_Completed(
        path_t *pPath,
        const float *vStartPos,
        int bReduceLookaheadAmount,
        float totalArea,
        int bAllowRestore);
void __fastcall PathCalcLookahead_CheckMinLookaheadNodes(path_t *pPath, const pathpoint_t *pt, int currentNode);
void __fastcall Path_UpdateForwardLookahead(path_t *pPath, const float *vStartPos);
void __fastcall Path_UpdateForwardLookahead_IncompletePath(
        path_t *pPath,
        const pathpoint_t *pt,
        const float *vForwardStartPos,
        float area,
        float height);
void __fastcall Path_DebugDraw(path_t *pPath, float *vStartPos, int bDrawLookahead, int entId);
bool __fastcall Path_WithinApproxDist(path_t *pPath, float checkDist);
ai_stance_e __fastcall Path_AllowedStancesForPath(path_t *pPath);
bool __cdecl Path_IsPathStanceNode(const pathnode_t *node);
char __fastcall Path_AttemptDodge(
        path_t *pPath,
        const float *vOrg,
        const float *vDodgeStart,
        const float *vDodgeEnd,
        int startIndex,
        const int *entities,
        int entityCount,
        int entityIgnore,
        int mask,
        int bCheckLookahead);
void __cdecl Path_CheckNodeCountForDodge(path_t *pPath, int numNeeded, pathpoint_t **pt, int *startIndex);
void __cdecl Path_DodgeDrawRaisedLine(const float *start, const float *end, const float *color);
bool __fastcall Path_MayFaceEnemy(path_t *pPath, float *vEnemyDir, float *vOrg);
void __fastcall Path_TrimToBadPlaceLink(path_t *pPath, team_t eTeam);
char __thiscall CustomSearchInfo_FindCloseNode::IgnoreNode(CustomSearchInfo_FindCloseNode *this, pathnode_t *pNode);
double __thiscall CustomSearchInfo_FindPath::EvaluateHeuristic(
        CustomSearchInfo_FindPath *this,
        pathnode_t *pSuccessor,
        const float *vGoalPos);
double __userpurge CustomSearchInfo_FindPathWithWidth::EvaluateHeuristic@<st0>(
        CustomSearchInfo_FindPathWithWidth *this@<ecx>,
        float a2@<ebp>,
        pathnode_t *pSuccessor,
        const float *vGoalPos);
double __thiscall CustomSearchInfo_FindPathWithLOS::EvaluateHeuristic(
        CustomSearchInfo_FindPathWithLOS *this,
        pathnode_t *pSuccessor,
        const float *vGoalPos);
double __thiscall CustomSearchInfo_FindPathClosestPossible::EvaluateHeuristic(
        CustomSearchInfo_FindPathClosestPossible *this,
        pathnode_t *pSuccessor,
        const float *vGoalPos);
