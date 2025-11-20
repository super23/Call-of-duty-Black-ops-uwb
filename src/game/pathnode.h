#pragma once

void __cdecl Path_GetType(pathnode_t *node);
void __cdecl Scr_SetPathnodeField(unsigned int entnum, unsigned int offset);
void __cdecl Path_ReadOnly(int offset);
void __cdecl Scr_GetPathnodeField(unsigned int entnum, unsigned int offset);
void __cdecl Path_CallFunctionForNodes(scriptInstance_t inst, void (__cdecl *function)(scriptInstance_t, pathnode_t *));
void __cdecl GScr_SetDynamicPathnodeField(pathnode_t *node, unsigned int index);
void  parented_pathnode_list_update(generic_avl_map_node_t *a1@<ebp>, gentity_s *gent, const phys_mat44 *mat);
const pathnode_parent_t *__cdecl get_pathnode_parent(const pathnode_t *node);
void __cdecl G_UpdateTrackExtraNodes();
void __cdecl G_DropPathNodeToFloor(unsigned int nodeIndex);
// local variable allocation has failed, the output may be wrong!
void  node_droptofloor(cStaticModel_s *a1@<ebp>, pathnode_t *node);
void __cdecl setup_pathnode_parent(pathnode_t *node, int entnum, const float *origin_loc);
bool __cdecl is_moving_entity(gentity_s *gent);
void __cdecl G_InitPathBaseNode(pathbasenode_t *pbnode, const pathnode_t *pnode);
void __cdecl G_DropPathnodesToFloor();
void __cdecl G_SpawnPathnodeDynamic(SpawnVar *spawnVar);
void __cdecl G_ParsePathnodeScriptFields(pathnode_t *node, SpawnVar *spawnVar);
void __cdecl G_ParsePathnodeScriptField(const char *key, char *value, pathnode_t *node);
void __cdecl G_SetPathnodeScriptVariable(const char *key, char *value, pathnode_t *ent);
void __cdecl PathNode_UpdateStringField(
        const char *destKey,
        unsigned __int16 *destScrString,
        const char *key,
        char *value);
void __cdecl PathNode_UpdateFloatField(const char *destKey, float *destFloat, const char *key, const char *value);
void __cdecl PathNode_OriginMatches(const char *value, const float *nodeOrigin);
void __cdecl GScr_AddFieldsForPathnode();
void __cdecl Scr_FreePathnode(pathnode_t *node);
void __cdecl Scr_AddPathnode(pathnode_t *node);
void __cdecl Scr_GetNode();
void __cdecl Scr_GetNodeArray();
void __cdecl Scr_GetAllNodes();
pathnode_t *__cdecl Scr_GetPathnode(unsigned int index, scriptInstance_t inst);
bool __cdecl Path_CompareNodesIncreasing(const pathsort_t *ps1, const pathsort_t *ps2);
unsigned int __cdecl Path_ConvertNodeToIndex(const pathnode_t *node);
pathnode_t *__cdecl Path_ConvertIndexToNode(unsigned int index);
void __cdecl Path_Init(int restart);
void __cdecl Path_Shutdown();
void __cdecl PathNode_ClearStringField(unsigned __int16 *destScrString);
void __cdecl Path_NodesInCylinder_process(pathnode_t *pnode);
void __cdecl Path_MovingNodesInCylinder();
int __cdecl Path_NodesInCylinder(
        const float *origin,
        float maxDist,
        float maxHeight,
        pathsort_t *nodes,
        int maxNodes,
        int typeFlags);
void __cdecl Path_NodesInCylinder_r(pathnode_tree_t *tree);
int __cdecl Path_NodesInRadius(const float *origin, float maxDist, pathsort_t *nodes, int maxNodes, int typeFlags);
int __cdecl Path_IsDynamicBlockingEntity(gentity_s *ent);
bool __cdecl Path_IsBadPlaceLink(unsigned int nodeNumFrom, unsigned int nodeNumTo, team_t eTeam);
void __cdecl Path_AutoDisconnectPaths();
void __cdecl Path_PreSpawnInitPaths();
unsigned int Path_InitLinkCounts();
unsigned int Path_InitNodesDynamic();
void __cdecl Path_InitNodeDynamic(pathnode_t *loadNode);
void __cdecl Path_InitPaths();
int Path_InitLinkInfoArray();
void __cdecl Path_CheckSpawnExitNodesConnectivity();
void __cdecl Path_DrawDebugNoLinks(const pathnode_t *node, const float (*color)[4], int duration);
void __cdecl Path_DrawDebugNode(const float *cameraPos, const pathnode_t *node);
double __cdecl Path_GetDebugStringScale(const float *cameraPos, const float *origin);
void __cdecl Path_DrawDebugNodeBox(const pathnode_t *node);
void __cdecl Path_DrawDebugNearestNode(const float *vOrigin);
void __cdecl Path_DrawDebugFindPath(const float *vOrigin);
void __cdecl Path_DrawVisData();
void __cdecl Path_Update();
void __cdecl Path_DrawDebug();
void __cdecl Path_DrawDebugLink(const pathnode_t *node, int i, bool bShowAll);
void __cdecl Path_DrawDebugClaimedNodes(const float *origin, int numNodes);
bool __cdecl Path_NodesVisible(const pathnode_t *node0, const pathnode_t *node1);
int __cdecl NodeVisCacheEntry(int i, int j);
bool __cdecl Path_ExpandedNodeVisible(const pathnode_t *node0, const pathnode_t *node1);
int __cdecl ExpandedNodeVisCacheEntry(int i, int j);
pathnode_t *__cdecl Path_NearestNode(
        const float *vOrigin,
        pathsort_t *nodes,
        int typeFlags,
        float fMaxDist,
        int *returnCount,
        int maxNodes,
        nearestNodeHeightCheck heightCheck);
pathnode_t *__cdecl Path_NearestNodeNotCrossPlanes(
        const float *vOrigin,
        pathsort_t *nodes,
        int typeFlags,
        float fMaxDist,
        float (*vNormal)[2],
        float *fDist,
        int iPlaneCount,
        int *returnCount,
        int maxNodes,
        nearestNodeHeightCheck heightCheck);
bool __cdecl Path_CanClaimNode(const pathnode_t *node, sentient_s *claimer);
sentient_s *__cdecl Path_GetNodeOwner(const pathnode_t *node);
bool __cdecl Path_CanStealPriorityNode(const pathnode_t *node, sentient_s *claimer);
bool __cdecl Path_CanStealNode(const pathnode_t *node, sentient_s *claimer);
void __cdecl Path_ClaimNode(pathnode_t *node, sentient_s *claimer);
void __cdecl Path_ClaimNodeInternal(pathnode_t *node, sentient_s *claimer);
void __cdecl Path_MarkNodeOverlap(pathnode_t *node);
void __cdecl Path_RevokeClaim(pathnode_t *node, sentient_s *pNewClaimer);
void __cdecl Path_ForceClaimNode(pathnode_t *node, sentient_s *claimer);
void __cdecl Path_RelinquishNodeNow(sentient_s *claimer);
void __cdecl Path_RelinquishNodeSoon(sentient_s *claimer);
void __cdecl Path_RelinquishNode(sentient_s *claimer, int timeUntilRelinquished);
void __cdecl Path_MarkNodeInvalid(pathnode_t *node, team_t eTeam);
ai_stance_e __cdecl Path_AllowedStancesForNode(pathnode_t *node);
void __cdecl Path_ValidateNode(pathnode_t *node);
void __cdecl Path_ValidateAllNodes();
void __cdecl Path_CheckLinkLeaks();
void __cdecl Path_CheckUserCountLeaks();
void __cdecl Path_ConnectPathsForEntity(gentity_s *ent);
void __cdecl Path_ConnectPath(pathnode_t *node, int toNodeNum);
void __cdecl Path_DisconnectPathsForEntity(gentity_s *ent);
void __cdecl Path_DisconnectPath(gentity_s *ent, pathnode_t *node, pathlink_s *link);
void  Path_UpdateArcBadPlaceCount(float a1@<ebp>, badplace_arc_t *arc, int teamflags, int delta);
void __cdecl Path_UpdateBadPlaceCountForLink(pathlink_s *link, int teamflags, int delta);
void __cdecl Path_CheckForInwardLinks(
        pathnode_t *node,
        int currDepth,
        int maxDepth,
        float fRadiusSqrd,
        float fHeightSqrd,
        float fMaxRadiusSqrd,
        float fMaxHeightSqrd,
        float *centroid,
        badplace_arc_t *arc,
        float *side0,
        float *side1,
        int teamflags,
        int delta);
// local variable allocation has failed, the output may be wrong!
void  Path_UpdateLimitedDepthArcBadPlaceCount(
        float a1@<ebp>,
        badplace_arc_t *arc,
        int teamflags,
        int delta,
        int depth);
void __cdecl Path_UpdateBrushBadPlaceCount(gentity_s *brushEnt, int teamflags, int delta);
int __cdecl Path_IsNodeInArc(
        pathnode_t *pNode,
        const float *origin,
        float radius,
        float angle0,
        float angle1,
        float halfHeight);
bool __cdecl IsNodeEnabled(const pathnode_t *pNode);
pathnode_t *__cdecl G_FindPathNode(SpawnVar *spawnVar, nodeType type, int gameId);
void __cdecl G_ProcessPathnodeCommand(const RadiantCommand *command, SpawnVar *spawnVar);
void __cdecl G_ClearSelectedPathNode();
pathnode_parent_t *__thiscall phys_simple_allocator<pathnode_parent_t>::allocate(
        phys_simple_allocator<pathnode_parent_t> *this);
