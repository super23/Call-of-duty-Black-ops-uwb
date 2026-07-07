// Vehicle spline paths and path-graph routing for script vehicles.
// Splines: G_SetupSplinePaths links nodes via target/target2.
// Graph: G_ConnectVehiclePaths builds s_node_links for A* in VP_FindPath.
#include "g_vehicle_path.h"
#include <clientscript/cscr_stringlist.h>
#include <cfloat>
#include <cstring>
#include "g_debug.h"
#include <cgame/cg_drawtools.h>
#include <universal/com_math_anglevectors.h>
#include "g_scr_helicopter.h"
#include <clientscript/cscr_vm.h>
#include <game/g_spawn_wrapper.h>
#include "g_load_utils.h"
#include "sentient_fields.h"

void __cdecl GScr_SetGenericField(unsigned __int8 *b, fieldtype_t type, int ofs, unsigned int whichbits);
void __cdecl GScr_GetGenericField(unsigned __int8 *b, fieldtype_t type, int ofs, unsigned int whichbits);

static const float s_invalidAngles[3] = { 3.1415927, 3.1415927, 3.1415927 };

vn_field_t vn_fields[12] =
{
  { "targetname", 0, { 2 }, F_STRING, false },
  { "target", 2, { 2 }, F_STRING, false },
  { "target2", 4, { 2 }, F_STRING, false },
  { "script_linkname", 6, { 2 }, F_STRING, false },
  { "script_noteworthy", 8, { 2 }, F_STRING, false },
  { "origin", 24, { 12 }, F_VECTOR, true },
  { "angles", 36, { 12 }, F_VECTOR, false },
  { "speed", 16, { 4 }, F_FLOAT, false },
  { "radius", 52, { 4 }, F_FLOAT, false },
  { "lookahead", 20, { 4 }, F_FLOAT, false },
  { "spawnflags", 12, { 4 }, F_INT, false },
  { NULL, 0, { 0 }, F_INT, false }
};
int num_heli_height_lock_patches;
__int16 s_numVehicleNodeLinks;
vehicle_node_t s_nodes[2000];
__int16 s_numNodes;
vehicle_path_node_link_t s_node_links[2000];
bool pathsInitialized;
vehicle_custom_path_t gCustomPaths[10];
int s_newDebugLine = 1;
vehicle_node_t *g_radiant_selected_node;
float s_dir[3];
float s_end[3];
float s_start[3];

// Decomp: CoDMPServer.c:608992  RVA: 0x007C7430  Retail: N/A
void __cdecl VP_ResetLinks()
{
    s_numVehicleNodeLinks = 0;
}

// Decomp: CoDMPServer.c:609002  RVA: 0x007C7440  Retail: N/A
__int16 __cdecl VP_GetNextFreePathNode()
{
    if ( s_numVehicleNodeLinks >= 2000
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\g_vehicle_path.cpp",
                    52,
                    0,
                    "%s",
                    "s_numVehicleNodeLinks < VEHICLE_MAX_NODE_LINKS") )
    {
        __debugbreak();
    }
    return s_numVehicleNodeLinks++;
}

// Decomp: CoDMPServer.c:609019  RVA: 0x007C7490  Retail: N/A
void __cdecl VP_ClearNode(vehicle_node_t *node)
{
    Scr_SetString(&node->name, 0, SCRIPTINSTANCE_SERVER);
    Scr_SetString(&node->target, 0, SCRIPTINSTANCE_SERVER);
    Scr_SetString(&node->target2, 0, SCRIPTINSTANCE_SERVER);
    Scr_SetString(&node->script_linkname, 0, SCRIPTINSTANCE_SERVER);
    Scr_SetString(&node->script_noteworthy, 0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:609029  RVA: 0x007C7500  Retail: N/A
__int16 __cdecl VP_GetNodeIndex(unsigned __int16 name, float *origin)
{
    vehicle_node_t *node;
    __int16 i;
    if ( !name )
        return -1;
    for ( i = 0; i < s_numNodes; ++i )
    {
        node = &s_nodes[i];
        if ( node->name == name )
        {
            if ( !origin )
                return i;
            if ( node->origin[0] == *origin && node->origin[1] == origin[1] && node->origin[2] == origin[2] )
                return i;
        }
    }
    return -1;
}

// Nearest VEH_NODE_FLAG_PATH_NODE, biased by node radius (splineNode.length).
__int16 __cdecl VP_FindClosestPathNode(const float *pos)
{
    double distSq;
    vehicle_node_t *node;
    float closestDist2;
    __int16 closestNode;
    float adjustedDist2;
    __int16 i;
    closestNode = -1;
    closestDist2 = FLT_MAX;
    for ( i = 0; i < s_numNodes; ++i )
    {
        distSq = Vec3DistanceSq(pos, s_nodes[i].origin);
        node = &s_nodes[i];
        if ( (node->flags & 2) != 0 )
        {
            adjustedDist2 = distSq - node->splineNode.length * node->splineNode.length;
            if ( closestDist2 > adjustedDist2 )
            {
                closestDist2 = adjustedDist2;
                closestNode = i;
            }
        }
    }
    return closestNode;
}

// Decomp: CoDMPServer.c:609083  RVA: 0x007C7690  Retail: N/A
vehicle_custom_path_t *__cdecl VP_GetFreeCustomPath()
{
    int j;
    int i;
    if ( !pathsInitialized )
    {
        pathsInitialized = 1;
        for ( i = 0; i < 10; ++i )
            gCustomPaths[i].inUse = 0;
    }
    for ( j = 0; j < 10; ++j )
    {
        if ( !gCustomPaths[j].inUse )
        {
            gCustomPaths[j].inUse = 1;
            return &gCustomPaths[j];
        }
    }
    return 0;
}

// Decomp: CoDMPServer.c:609106  RVA: 0x007C7730  Retail: N/A
void __cdecl VP_FreeCustomPath(vehicle_custom_path_t *path)
{
    if ( !path
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\g_vehicle_path.cpp", 1409, 0, "%s", "path") )
    {
        __debugbreak();
    }
    path->inUse = 0;
}

// Reverse A* from endNode to startNode; fills customPath with pathOrder/pathLinkIdx.
int __cdecl VP_FindPath(float *start, float *end, vehicle_pathpos_t *vpp)
{
    double goalHeuristic;
    const char *assertMsg;
    double endDistSq;
    float *goalOrigin;
    float *endNodeOrigin;
    __int16 neighborIdx;
    bool shouldUpdateNeighbor;
    float newPathCost;
    __int16 linkIdx;
    __int16 lastLinkIdx;
    __int16 linkSearchIdx;
    __int16 linkCount;
    __int16 firstLinkIdx;
    __int16 walkNodeIdx;
    vehicle_custom_path_t *path;
    bool extendToEndNode;
    int pathOrderCount;
    __int16 queueSwapTemp;
    __int16 queueNodeIdx;
    int queueSortIdx;
    __int16 outgoingLinkCount;
    float bestQueueCost;
    __int16 currentNodeIdx;
    __int16 firstLinkBaseIdx;
    __int16 pathQueue[128];
    __int16 pathQueueCount;
    _WORD searchState[16000];
    __int16 endNodeIdx;
    __int16 startNodeIdx;
    startNodeIdx = VP_FindClosestPathNode(start);
    endNodeIdx = VP_FindClosestPathNode(end);
    if ( endNodeIdx < 0 || startNodeIdx < 0 )
        return 0;
    memset((unsigned __int8 *)searchState, 0, sizeof(searchState));
    pathQueueCount = 0;
    *(unsigned int *)&searchState[8 * endNodeIdx + 2] = 0;
    goalHeuristic = Vec3Distance(s_nodes[endNodeIdx].origin, s_nodes[startNodeIdx].origin);
    *(float *)&searchState[8 * endNodeIdx + 6] = goalHeuristic;
    *(float *)&searchState[8 * endNodeIdx + 4] = *(float *)&searchState[8 * endNodeIdx + 6];
    searchState[8 * endNodeIdx + 1] |= 1u;
    pathQueue[pathQueueCount++] = endNodeIdx;
    while ( 1 )
    {
        if ( pathQueueCount <= 0 )
            return 0;
        currentNodeIdx = pathQueue[--pathQueueCount];
        bestQueueCost = *(float *)&searchState[8 * currentNodeIdx + 4];
        for ( queueSortIdx = pathQueueCount - 1; queueSortIdx >= 0; --queueSortIdx )
        {
            queueNodeIdx = pathQueue[queueSortIdx];
            if ( bestQueueCost > *(float *)&searchState[8 * queueNodeIdx + 4] )
            {
                bestQueueCost = *(float *)&searchState[8 * queueNodeIdx + 4];
                currentNodeIdx = pathQueue[queueSortIdx];
                queueSwapTemp = pathQueue[pathQueueCount];
                pathQueue[pathQueueCount] = queueNodeIdx;
                pathQueue[queueSortIdx] = queueSwapTemp;
            }
        }
        searchState[8 * currentNodeIdx + 1] |= 2u;
        if ( currentNodeIdx == startNodeIdx )
            break;
        firstLinkBaseIdx = s_nodes[currentNodeIdx].splineNode.prevIdx;
        outgoingLinkCount = s_nodes[currentNodeIdx].splineNode.nextIdx;
        for ( linkIdx = 0; linkIdx < outgoingLinkCount; ++linkIdx )
        {
            neighborIdx = s_node_links[(__int16)(linkIdx + firstLinkBaseIdx)].nextIdx;
            if ( (s_nodes[neighborIdx].flags & 0x80000) == 0 && (searchState[8 * neighborIdx + 1] & 2) == 0 )
            {
                newPathCost = *(float *)&searchState[8 * currentNodeIdx + 2] + s_node_links[linkIdx + firstLinkBaseIdx].length;
                if ( (searchState[8 * neighborIdx + 1] & 1) != 0 )
                {
                    shouldUpdateNeighbor = *(float *)&searchState[8 * neighborIdx + 2] > newPathCost;
                }
                else
                {
                    if ( pathQueueCount >= 128
                        && !Assert_MyHandler(
                                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\g_vehicle_path.cpp",
                                    1574,
                                    0,
                                    "%s",
                                    "pathQueueCount < MAX_CUSTOM_PATH_LENGTH") )
                    {
                        __debugbreak();
                    }
                    searchState[8 * neighborIdx + 1] |= 1u;
                    shouldUpdateNeighbor = 1;
                    pathQueue[pathQueueCount++] = neighborIdx;
                }
                if ( shouldUpdateNeighbor )
                {
                    searchState[8 * neighborIdx] = currentNodeIdx;
                    *(float *)&searchState[8 * neighborIdx + 2] = newPathCost;
                    *(float *)&searchState[8 * neighborIdx + 6] = Vec3Distance(s_nodes[neighborIdx].origin, s_nodes[startNodeIdx].origin);
                    *(float *)&searchState[8 * neighborIdx + 4] = *(float *)&searchState[8 * neighborIdx + 2] + *(float *)&searchState[8 * neighborIdx + 6];
                }
            }
        }
    }
    if ( vpp->customPath )
        path = vpp->customPath;
    else
        path = VP_GetFreeCustomPath();
    if ( !path )
        return 0;
    walkNodeIdx = startNodeIdx;
    pathOrderCount = 0;
    while ( walkNodeIdx != endNodeIdx )
    {
        if ( pathOrderCount >= 128 )
        {
            assertMsg = va("Vehicle path length over %d nodes long\n", 128);
            if ( !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\universal\\g_vehicle_path.cpp",
                            1495,
                            0,
                            "%s\n\t%s",
                            "i < MAX_CUSTOM_PATH_LENGTH",
                            assertMsg) )
                __debugbreak();
        }
        path->pathOrder[pathOrderCount] = walkNodeIdx;
        firstLinkIdx = s_nodes[walkNodeIdx].splineNode.prevIdx;
        linkCount = s_nodes[walkNodeIdx].splineNode.nextIdx;
        walkNodeIdx = searchState[8 * walkNodeIdx];
        for ( linkSearchIdx = 0; linkSearchIdx < linkCount; ++linkSearchIdx )
        {
            if ( s_node_links[(__int16)(linkSearchIdx + firstLinkIdx)].nextIdx == walkNodeIdx )
                path->pathLinkIdx[pathOrderCount] = linkSearchIdx + firstLinkIdx;
        }
        ++pathOrderCount;
    }
    extendToEndNode = 1;
    if ( pathOrderCount )
    {
        endDistSq = Vec3DistanceSq(s_nodes[endNodeIdx].origin, end);
        if ( s_nodes[endNodeIdx].splineNode.length * s_nodes[endNodeIdx].splineNode.length <= endDistSq )
        {
            endNodeOrigin = s_nodes[endNodeIdx].origin;
            vpp->customGoalDir[0] = *end - *endNodeOrigin;
            vpp->customGoalDir[1] = end[1] - endNodeOrigin[1];
            vpp->customGoalDir[2] = end[2] - endNodeOrigin[2];
            vpp->customGoalLength = Vec3Normalize(vpp->customGoalDir);
            lastLinkIdx = path->pathLinkIdx[pathOrderCount - 1];
            if ( s_node_links[lastLinkIdx].nextIdx != endNodeIdx
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\universal\\g_vehicle_path.cpp",
                            1529,
                            0,
                            "%s",
                            "s_node_links[linkIdx].nextIdx == endNode") )
            {
                __debugbreak();
            }
            if ( (float)((float)((float)(vpp->customGoalDir[0] * s_node_links[lastLinkIdx].dir[0])
                                                 + (float)(vpp->customGoalDir[1] * s_node_links[lastLinkIdx].dir[1]))
                                 + (float)(vpp->customGoalDir[2] * s_node_links[lastLinkIdx].dir[2])) < -0.30000001 )
                extendToEndNode = 0;
        }
        else
        {
            extendToEndNode = 0;
        }
        if ( extendToEndNode )
        {
            path->pathOrder[pathOrderCount] = endNodeIdx;
            path->pathOrder[pathOrderCount + 1] = -1;
        }
        else
        {
            if ( pathOrderCount > 0 )
                endNodeIdx = path->pathOrder[pathOrderCount - 1];
            path->pathOrder[pathOrderCount] = -1;
        }
        G_VehSetUpPathPos(vpp, path->pathOrder[0]);
        vpp->nodeIdx = 0;
        vpp->customPath = path;
        goalOrigin = s_nodes[endNodeIdx].origin;
        vpp->customGoalDir[0] = *end - *goalOrigin;
        vpp->customGoalDir[1] = end[1] - goalOrigin[1];
        vpp->customGoalDir[2] = end[2] - goalOrigin[2];
        vpp->customGoalLength = Vec3Normalize(vpp->customGoalDir);
        return 1;
    }
    else
    {
        VP_FreeCustomPath(path);
        vpp->customPath = 0;
        return 0;
    }
}

// Decomp: CoDMPServer.c:609336  RVA: 0x007C8150  Retail: N/A
void __cdecl VP_DrawCustomPath(vehicle_custom_path_t *customPath)
{
    __int16 nodeIdx;
    __int16 nextNodeIdx;
    int i;
    if ( customPath )
    {
        for ( i = 1; i < 128; ++i )
        {
            nodeIdx = customPath->pathOrder[i - 1];
            if ( nodeIdx < 0 )
                break;
            nextNodeIdx = customPath->pathOrder[i];
            if ( nextNodeIdx < 0 )
                break;
            G_DebugLine(s_nodes[nodeIdx].origin, s_nodes[nextNodeIdx].origin, colorGreen, 1);
        }
    }
}

// Decomp: CoDMPServer.c:609358  RVA: 0x007C8200  Retail: N/A
void __cdecl VP_DrawPath(const vehicle_pathpos_t *vpp)
{
    vehicle_node_t *node;
    float mins[3];
    float maxs[3];
    float k_boxColor2[4];
    float k_boxColor1[4];
    int arrowCount;
    int done;
    vehicle_pathpos_t nextVPP;
    vehicle_pathpos_t prevVPP;
    __int16 loopNode;
    int looped;
    int arrowTime;
    int count;
    arrowCount = 0;
    arrowTime = 1000;
    done = 0;
    count = 0;
    loopNode = -1;
    memcpy(&prevVPP, vpp, sizeof(prevVPP));
    memcpy(&nextVPP, vpp, sizeof(nextVPP));
    s_newDebugLine = 1;
    while ( !done && count <= 50000 )
    {
        ++count;
        if ( prevVPP.nodeIdx != vpp->nodeIdx )
            loopNode = vpp->nodeIdx;
        memcpy(&prevVPP, &nextVPP, sizeof(prevVPP));
        looped = G_VehUpdatePathPos(&nextVPP, loopNode);
        if ( nextVPP.endOfPath || looped )
            done = 1;
        VP_AddDebugLine(prevVPP.origin, nextVPP.origin, done);
        if ( arrowCount <= arrowTime )
        {
            arrowCount += 50;
        }
        else
        {
            VP_DebugArrow(nextVPP.origin, nextVPP.angles);
            arrowCount = 0;
        }
    }
    k_boxColor1[0] = 0.0f;
    k_boxColor1[1] = 1.0f;
    k_boxColor1[2] = 0.0f;
    k_boxColor1[3] = 1.0f;
    k_boxColor2[0] = 0.0f;
    k_boxColor2[1] = 0.0f;
    k_boxColor2[2] = 1.0f;
    k_boxColor2[3] = 1.0f;
    count = 0;
    for ( node = &s_nodes[vpp->nodeIdx];
                count < s_numNodes && vpp->nodeIdx != node->splineNode.nextIdx;
                node = &s_nodes[node->splineNode.nextIdx] )
    {
        ++count;
        mins[0] = -8.0f;
        mins[1] = -8.0f;
        mins[2] = -8.0f;
        maxs[0] = 8.0f;
        maxs[1] = 8.0f;
        maxs[2] = 8.0f;
        if ( node == &s_nodes[vpp->nodeIdx] || g_radiant_selected_node == node )
            CG_DebugBox(node->origin, mins, maxs, node->angles[1], k_boxColor1, 1, 0);
        else
            CG_DebugBox(node->origin, mins, maxs, node->angles[1], k_boxColor2, 1, 0);
        if ( node->splineNode.nextIdx < 0 || node->splineNode.nextIdx == vpp->nodeIdx )
            break;
    }
}

// Decomp: CoDMPServer.c:609451  RVA: 0x007C8550  Retail: N/A
void __cdecl VP_AddDebugLine(const float *start, const float *end, int forceDraw)
{
    float k_lineColor[4];
    float dir[3];
    dir[0] = *end - *start;
    dir[1] = end[1] - start[1];
    dir[2] = end[2] - start[2];
    Vec3Normalize(dir);
    if (s_newDebugLine)
    {
        s_start[0] = *start;
        s_start[1] = start[1];
        s_start[2] = start[2];
        s_end[0] = *end;
        s_end[1] = end[1];
        s_end[2] = end[2];
        s_dir[0] = dir[0];
        s_dir[1] = dir[1];
        s_dir[2] = dir[2];
        s_newDebugLine = 0;
    }
    else if ((float)((float)((float)(dir[0] * s_dir[0]) + (float)(dir[1] * s_dir[1])) + (float)(dir[2] * s_dir[2])) < 0.99989998
        || forceDraw)
    {
        k_lineColor[0] = 1.0f;
        k_lineColor[1] = 0.0f;
        k_lineColor[2] = 0.0f;
        k_lineColor[3] = 1.0f;
        G_DebugLine(s_start, s_end, k_lineColor, 1);
        s_start[0] = *start;
        s_start[1] = start[1];
        s_start[2] = start[2];
        s_end[0] = *end;
        s_end[1] = end[1];
        s_end[2] = end[2];
        s_dir[0] = dir[0];
        s_dir[1] = dir[1];
        s_dir[2] = dir[2];
    }
    else
    {
        s_end[0] = *end;
        s_end[1] = end[1];
        s_end[2] = end[2];
    }
}

// Draw a heading arrow at pos (debug only).
void __cdecl VP_DebugArrow(const float *pos, const float *angles)
{
    float pts[5][3];
    float k_lineColor[4];
    float scale;
    int i;
    float axis[4][3];
    float origPts[5][3];
    scale = 80.0f;
    //*(_QWORD *)&origPts[0][0] = __PAIR64__(0, LODWORD(0.5f));
    //origPts[0][0] = 0.5f;
    //origPts[0][1] = 0.0f;
    //origPts[0][2] = 0.0f;
    //*(_QWORD *)&origPts[1][0] = __PAIR64__(LODWORD(FLOAT_N0_40000001), LODWORD(-0.5f));
    //origPts[1][2] = 0.0f;
    //*(_QWORD *)&origPts[2][0] = __PAIR64__(LODWORD(0.4f), LODWORD(-0.5f));
    //origPts[2][2] = 0.0f;
    //*(_QWORD *)&origPts[3][0] = __PAIR64__(0, LODWORD(-0.5f));
    //origPts[3][2] = 0.4f;
    //*(_QWORD *)&origPts[4][0] = __PAIR64__(0, LODWORD(-0.5f));
    //origPts[4][2] = 0.0f;
    // (ai assisted)
    origPts[0][0] = 0.5f;
    origPts[0][1] = 0.0f;
    origPts[0][2] = 0.0f;
    origPts[1][0] = -0.5f;
    origPts[1][1] = -0.4f;
    origPts[1][2] = 0.0f;
    origPts[2][0] = -0.5f;
    origPts[2][1] = 0.4f;
    origPts[2][2] = 0.0f;
    origPts[3][0] = -0.5f;
    origPts[3][1] = 0.0f;
    origPts[3][2] = 0.4f;
    origPts[4][0] = -0.5f;
    origPts[4][1] = 0.0f;
    AnglesToAxis(angles, axis);
    axis[3][0] = *pos;
    axis[3][1] = pos[1];
    axis[3][2] = pos[2];
    for ( i = 0; i < 5; ++i )
    {
        pts[i][0] = scale * origPts[i][0];
        pts[i][1] = scale * origPts[i][1];
        pts[i][2] = scale * origPts[i][2];
        MatrixTransformVector43(pts[i], axis, pts[i]);
    }
    k_lineColor[0] = 1.0f;
    k_lineColor[1] = 0.0f;
    k_lineColor[2] = 0.0f;
    k_lineColor[3] = 1.0f;
    G_DebugLine(pts[0], pts[1], k_lineColor, 1);
    G_DebugLine(pts[1], pts[2], k_lineColor, 1);
    G_DebugLine(pts[2], pts[0], k_lineColor, 1);
    G_DebugLine(pts[3], pts[4], k_lineColor, 1);
    G_DebugLine(pts[4], pos, k_lineColor, 1);
    G_DebugLine(pos, pts[3], k_lineColor, 1);
}

// Decomp: CoDMPServer.c:609595  RVA: 0x007C8AC0  Retail: N/A
void __cdecl G_InitVehiclePaths()
{
    s_numNodes = 0;
    s_numVehicleNodeLinks = 0;
}

// Decomp: CoDMPServer.c:609606  RVA: 0x007C8AE0  Retail: N/A
void __cdecl G_FreeVehiclePaths()
{
    vehicle_node_t *node;
    __int16 i;
    for ( i = 0; i < s_numNodes; ++i )
    {
        node = &s_nodes[i];
        Scr_FreeEntityNum(node->index, 3u, SCRIPTINSTANCE_SERVER);
        VP_ClearNode(node);
    }
    s_numNodes = 0;
    s_numVehicleNodeLinks = 0;
    for ( i = 0; i < num_heli_height_lock_patches; ++i )
            Scr_SetString(&heli_height_lock_patches[i].targetname, 0, SCRIPTINSTANCE_SERVER);
    num_heli_height_lock_patches = 0;
}

// Decomp: CoDMPServer.c:609634  RVA: 0x007C8BA0  Retail: N/A
void __cdecl VP_ResetNode(vehicle_node_t *node)
{
    if ( (node->flags & 0x20000) == 0 )
        node->speed = -1.0f;
    if ( (node->flags & 0x40000) == 0 )
        node->lookAhead = -1.0f;
}

// Decomp: CoDMPServer.c:609644  RVA: 0x007C8BF0  Retail: N/A
void __cdecl G_SetupSplinePaths()
{
    float *nextOrigin;
    vehicle_node_t *node;
    __int16 j;
    __int16 i;
    for ( i = 0; i < s_numNodes; ++i )
    {
        node = &s_nodes[i];
        VP_ResetNode(node);
        if ( (node->flags & 2) == 0 )
        {
            if ( node->target )
                node->splineNode.nextIdx = VP_GetNodeIndex(node->target, 0);
            for ( j = 0; j < s_numNodes; ++j )
            {
                if ( node->name && i != j && node->name == s_nodes[j].target )
                {
                    node->splineNode.prevIdx = j;
                    break;
                }
            }
            if ( node->splineNode.nextIdx == i )
                node->splineNode.nextIdx = -1;
            if ( node->splineNode.prevIdx == i )
                node->splineNode.prevIdx = -1;
        }
    }
    for ( i = 0; i < s_numNodes; ++i )
    {
        node = &s_nodes[i];
        if ( (node->flags & 2) == 0 && node->splineNode.nextIdx >= 0 )
        {
            nextOrigin = s_nodes[node->splineNode.nextIdx].origin;
            node->splineNode.dir[0] = *nextOrigin - node->origin[0];
            node->splineNode.dir[1] = nextOrigin[1] - node->origin[1];
            node->splineNode.dir[2] = nextOrigin[2] - node->origin[2];
            node->splineNode.length = Vec3Normalize(node->splineNode.dir);
            if ( (node->flags & 0x10000) == 0 )
                vectoangles(node->splineNode.dir, node->angles);
        }
    }
    for ( i = 0; i < s_numNodes; ++i )
    {
        node = &s_nodes[i];
        if ( (node->flags & 2) == 0 )
        {
            node->speed = VP_CalcNodeSpeed(i);
            node->lookAhead = VP_CalcNodeLookAhead(i);
            if ( node->speed < 0.0 )
                Com_Error(ERR_DROP, "Vehicle path node at( %f, %f, %f ) has negative speed", node->origin[0], node->origin[1], node->origin[2]);
            if ( (node->flags & 0x10000) != 0 )
                VP_CalcNodeAngles(i, node->angles);
            node->angles[0] = AngleNormalize180(node->angles[0]);
            node->angles[1] = AngleNormalize180(node->angles[1]);
            node->angles[2] = AngleNormalize180(node->angles[2]);
            if ( node->speed <= 0.0 || node->lookAhead <= 0.0 )
                node->splineNode.nextIdx = -1;
            if ( node->splineNode.nextIdx < 0 )
            {
                if ( node->speed <= 0.0 )
                    node->speed = 1.0f;
                if ( node->lookAhead <= 0.0 )
                    node->lookAhead = 1.0f;
            }
        }
    }
}

// Decomp: CoDMPServer.c:609726  RVA: 0x007C8FB0  Retail: N/A
double __cdecl VP_CalcNodeSpeed(__int16 nodeIdx)
{
    vehicle_node_t *node;
    float prevDist;
    float nextDist;
    float nextSpeed;
    vehicle_node_t *walkNode;
    float prevSpeed;
    __int16 count;
    node = &s_nodes[nodeIdx];
    if ( (node->flags & 0x20000) != 0 )
        return node->speed;
    nextDist = 0.0f;
    prevDist = 0.0f;
    nextSpeed = -1.0f;
    prevSpeed = -1.0f;
    if ( node->splineNode.prevIdx >= 0 )
    {
        count = 0;
        walkNode = &s_nodes[node->splineNode.prevIdx];
        while ( count < s_numNodes )
        {
            ++count;
            prevDist = prevDist + walkNode->splineNode.length;
            if ( walkNode->speed >= 0.0 )
            {
                prevSpeed = walkNode->speed;
                break;
            }
            if ( walkNode->splineNode.prevIdx < 0 || walkNode->splineNode.prevIdx == nodeIdx )
                break;
            walkNode = &s_nodes[walkNode->splineNode.prevIdx];
        }
    }
    count = 0;
    walkNode = &s_nodes[nodeIdx];
    while ( count < s_numNodes )
    {
        ++count;
        if ( walkNode->speed >= 0.0 )
        {
            nextSpeed = walkNode->speed;
            break;
        }
        if ( walkNode->splineNode.nextIdx < 0 || walkNode->splineNode.nextIdx == nodeIdx )
            break;
        nextDist = nextDist + walkNode->splineNode.length;
        walkNode = &s_nodes[walkNode->splineNode.nextIdx];
    }
    if ( prevSpeed < 0.0 && nextSpeed < 0.0 )
        return 0.0;
    if ( prevSpeed < 0.0 )
        return nextSpeed;
    if ( nextSpeed < 0.0 )
        return prevSpeed;
    if ( (float)(prevDist + nextDist) > 0.0 )
        return (float)((float)((float)(nextSpeed - prevSpeed) * (float)(prevDist / (float)(prevDist + nextDist))) + prevSpeed);
    return 0.0;
}

// Decomp: CoDMPServer.c:609791  RVA: 0x007C91F0  Retail: N/A
double __cdecl VP_CalcNodeLookAhead(__int16 nodeIdx)
{
    vehicle_node_t *node;
    float prevDist;
    float nextDist;
    vehicle_node_t *walkNode;
    float prevLook;
    float nextLook;
    __int16 count;
    node = &s_nodes[nodeIdx];
    if ( node->lookAhead >= 0.0 )
        return node->lookAhead;
    nextDist = 0.0f;
    prevDist = 0.0f;
    nextLook = -1.0f;
    prevLook = -1.0f;
    if ( node->splineNode.prevIdx >= 0 )
    {
        count = 0;
        walkNode = &s_nodes[node->splineNode.prevIdx];
        while ( count < s_numNodes )
        {
            ++count;
            prevDist = prevDist + walkNode->splineNode.length;
            if ( walkNode->lookAhead > 0.0 )
            {
                prevLook = walkNode->lookAhead;
                break;
            }
            if ( walkNode->splineNode.prevIdx < 0 || walkNode->splineNode.prevIdx == nodeIdx )
                break;
            walkNode = &s_nodes[walkNode->splineNode.prevIdx];
        }
    }
    count = 0;
    walkNode = &s_nodes[nodeIdx];
    while ( count < s_numNodes )
    {
        ++count;
        if ( walkNode->lookAhead > 0.0 )
        {
            nextLook = walkNode->lookAhead;
            break;
        }
        if ( walkNode->splineNode.nextIdx < 0 || walkNode->splineNode.nextIdx == nodeIdx )
            break;
        nextDist = nextDist + walkNode->splineNode.length;
        walkNode = &s_nodes[walkNode->splineNode.nextIdx];
    }
    if ( prevLook < 0.0 && nextLook < 0.0 )
        return 0.0;
    if ( prevLook < 0.0 )
        return nextLook;
    if ( nextLook < 0.0 )
        return prevLook;
    if ( (float)(prevDist + nextDist) > 0.0 )
        return (float)((float)((float)(nextLook - prevLook) * (float)(prevDist / (float)(prevDist + nextDist))) + prevLook);
    return 0.0;
}

// Decomp: CoDMPServer.c:609856  RVA: 0x007C9430  Retail: N/A
void __cdecl VP_CalcNodeAngles(__int16 nodeIdx, float *angles)
{
    vehicle_node_t *node;
    float prevDist;
    float nextDist;
    float totalDist;
    float nextAnglePitch;
    float nextAngleYaw;
    float nextAngleRoll;
    vehicle_node_t *walkNode;
    float prevAnglePitch;
    float prevAngleYaw;
    float prevAngleRoll;
    __int16 count;
    node = &s_nodes[nodeIdx];
    if ( node->angles[0] != s_invalidAngles[0]
        || s_nodes[nodeIdx].angles[1] != s_invalidAngles[1]
        || s_nodes[nodeIdx].angles[2] != s_invalidAngles[2] )
    {
        *angles = node->angles[0];
        angles[1] = node->angles[1];
        angles[2] = node->angles[2];
        return;
    }
    nextDist = 0.0f;
    prevDist = 0.0f;
    prevAnglePitch = s_invalidAngles[0];
    prevAngleYaw = s_invalidAngles[1];
    prevAngleRoll = s_invalidAngles[2];
    nextAnglePitch = s_invalidAngles[0];
    nextAngleYaw = s_invalidAngles[1];
    nextAngleRoll = s_invalidAngles[2];
    if ( node->splineNode.prevIdx >= 0 )
    {
        count = 0;
        walkNode = &s_nodes[node->splineNode.prevIdx];
        while ( count < s_numNodes )
        {
            ++count;
            prevDist = prevDist + walkNode->splineNode.length;
            if ( walkNode->angles[0] != s_invalidAngles[0]
                || walkNode->angles[1] != s_invalidAngles[1]
                || walkNode->angles[2] != s_invalidAngles[2] )
            {
                prevAnglePitch = walkNode->angles[0];
                prevAngleYaw = walkNode->angles[1];
                prevAngleRoll = walkNode->angles[2];
                break;
            }
            if ( walkNode->splineNode.prevIdx < 0 || walkNode->splineNode.prevIdx == nodeIdx )
                break;
            walkNode = &s_nodes[walkNode->splineNode.prevIdx];
        }
    }
    count = 0;
    walkNode = &s_nodes[nodeIdx];
    while ( count < s_numNodes )
    {
        ++count;
        if ( walkNode->angles[0] != s_invalidAngles[0]
            || walkNode->angles[1] != s_invalidAngles[1]
            || walkNode->angles[2] != s_invalidAngles[2] )
        {
            nextAnglePitch = walkNode->angles[0];
            nextAngleYaw = walkNode->angles[1];
            nextAngleRoll = walkNode->angles[2];
            break;
        }
        if ( walkNode->splineNode.nextIdx < 0 || walkNode->splineNode.nextIdx == nodeIdx )
            break;
        nextDist = nextDist + walkNode->splineNode.length;
        walkNode = &s_nodes[walkNode->splineNode.nextIdx];
    }
    if ( prevAnglePitch == s_invalidAngles[0]
        && prevAngleYaw == s_invalidAngles[1]
        && prevAngleRoll == s_invalidAngles[2]
        && nextAnglePitch == s_invalidAngles[0]
        && nextAngleYaw == s_invalidAngles[1]
        && nextAngleRoll == s_invalidAngles[2] )
    {
        goto LABEL_47;
    }
    if ( prevAnglePitch == s_invalidAngles[0] && prevAngleYaw == s_invalidAngles[1] && prevAngleRoll == s_invalidAngles[2] )
    {
        *angles = nextAnglePitch;
        angles[1] = nextAngleYaw;
        angles[2] = nextAngleRoll;
    }
    else if ( nextAnglePitch == s_invalidAngles[0] && nextAngleYaw == s_invalidAngles[1] && nextAngleRoll == s_invalidAngles[2] )
    {
        *angles = prevAnglePitch;
        angles[1] = prevAngleYaw;
        angles[2] = prevAngleRoll;
    }
    else
    {
        totalDist = prevDist + nextDist;
        if ( (float)(prevDist + nextDist) <= 0.0 )
        {
LABEL_47:
            *angles = 0.0f;
            angles[1] = 0.0f;
            angles[2] = 0.0f;
            return;
        }
        *angles = AngleNormalize180(nextAnglePitch - prevAnglePitch) * (float)(prevDist / totalDist) + prevAnglePitch;
        angles[1] = AngleNormalize180(nextAngleYaw - prevAngleYaw) * (float)(prevDist / totalDist) + prevAngleYaw;
        angles[2] = AngleNormalize180(nextAngleRoll - prevAngleRoll) * (float)(prevDist / totalDist) + prevAngleRoll;
    }
}

// Decomp: CoDMPServer.c:609994  RVA: 0x007C9A50  Retail: N/A
void __cdecl G_ConnectVehiclePaths()
{
    vehicle_path_node_link_t *reverseLink;
    vehicle_path_node_link_t *link;
    vehicle_node_t *node;
    __int16 j;
    __int16 linkIdx;
    __int16 reverseLinkIdx;
    vehicle_node_t *other;
    __int16 i;
    VP_ResetLinks();
    for ( i = 0; i < s_numNodes; ++i )
    {
        node = &s_nodes[i];
        if ( (node->flags & 2) != 0 )
        {
            VP_ResetNode(node);
            node->splineNode.nextIdx = 0;
            if ( node->speed <= 0.0 )
                node->speed = 100.0 * 17.6;
            if ( node->splineNode.length <= 0.0 )
                node->splineNode.length = 200.0f;
            if ( node->lookAhead <= 0.0 )
                node->lookAhead = (float)(node->splineNode.length * 2.0) - 50.0;
            if ( node->lookAhead <= 0.0 )
                node->lookAhead = 200.0f;
            for ( j = 0; j < s_numNodes; ++j )
            {
                other = &s_nodes[j];
                if ( (other->flags & 2) != 0 && other != node )
                {
                    if ( other->name && (node->target == other->name || node->target2 == other->name) )
                    {
                        linkIdx = VP_GetNextFreePathNode();
                        if ( !node->splineNode.nextIdx )
                            node->splineNode.prevIdx = linkIdx;
                        ++node->splineNode.nextIdx;
                        link = &s_node_links[linkIdx];
                        link->nextIdx = other->index;
                        link->length = Vec3Distance(node->origin, other->origin);
                        link->dir[0] = other->origin[0] - node->origin[0];
                        link->dir[1] = other->origin[1] - node->origin[1];
                        link->dir[2] = other->origin[2] - node->origin[2];
                        Vec3Normalize(link->dir);
                    }
                    if ( node->name && (other->target == node->name || other->target2 == node->name) )
                    {
                        reverseLinkIdx = VP_GetNextFreePathNode();
                        if ( !node->splineNode.nextIdx )
                            node->splineNode.prevIdx = reverseLinkIdx;
                        ++node->splineNode.nextIdx;
                        reverseLink = &s_node_links[reverseLinkIdx];
                        reverseLink->nextIdx = other->index;
                        reverseLink->length = Vec3Distance(node->origin, other->origin);
                        reverseLink->dir[0] = other->origin[0] - node->origin[0];
                        reverseLink->dir[1] = other->origin[1] - node->origin[1];
                        reverseLink->dir[2] = other->origin[2] - node->origin[2];
                        Vec3Normalize(reverseLink->dir);
                    }
                }
            }
        }
    }
}

// Decomp: CoDMPServer.c:610072  RVA: 0x007C9DD0  Retail: N/A
void __cdecl G_SetupVehiclePaths()
{
    G_SetupSplinePaths();
    G_ConnectVehiclePaths();
}

// Decomp: CoDMPServer.c:610079  RVA: 0x007C9DE0  Retail: N/A
void __cdecl VP_ZeroNode(vehicle_node_t *node)
{
    node->name = 0;
    node->target = 0;
    node->target2 = 0;
    node->script_linkname = 0;
    node->script_noteworthy = 0;
}

// Decomp: CoDMPServer.c:610089  RVA: 0x007C9E20  Retail: N/A
void __cdecl G_VehInitPathPos(vehicle_pathpos_t *vpp)
{
    vpp->lastNodeIdx = -1;
    vpp->nodeIdx = -1;
    vpp->endOfPath = 0;
    vpp->frac = 0.0f;
    vpp->speed = 0.0f;
    vpp->lookAhead = 0.0f;
    vpp->slide = 0.0f;
    vpp->customPath = 0;
    vpp->origin[0] = 0.0f;
    vpp->origin[1] = 0.0f;
    vpp->origin[2] = 0.0f;
    vpp->angles[0] = 0.0f;
    vpp->angles[1] = 0.0f;
    vpp->angles[2] = 0.0f;
    vpp->lookPos[0] = 0.0f;
    vpp->lookPos[1] = 0.0f;
    vpp->lookPos[2] = 0.0f;
    VP_InitNode(vpp->switchNode, -1);
    VP_InitNode(&vpp->switchNode[1], -1);
}

// Decomp: CoDMPServer.c:610120  RVA: 0x007C9F60  Retail: N/A
void __cdecl VP_InitNode(vehicle_node_t *node, __int16 nodeIdx)
{
    VP_ZeroNode(node);
    node->index = nodeIdx;
    node->flags = 0;
    node->speed = -1.0f;
    node->lookAhead = -1.0f;
    node->origin[0] = 0.0f;
    node->origin[1] = 0.0f;
    node->origin[2] = 0.0f;
    node->splineNode.dir[0] = 0.0f;
    node->splineNode.dir[1] = 0.0f;
    node->splineNode.dir[2] = 0.0f;
    node->angles[0] = s_invalidAngles[0];
    node->angles[1] = s_invalidAngles[1];
    node->angles[2] = s_invalidAngles[2];
    node->splineNode.length = 0.0f;
    node->splineNode.nextIdx = -1;
    node->splineNode.prevIdx = -1;
}

// Decomp: CoDMPServer.c:610152  RVA: 0x007CA080  Retail: N/A
void __cdecl G_VehFreePathPos(vehicle_pathpos_t *vpp)
{
    VP_ClearNode(vpp->switchNode);
    VP_ClearNode(&vpp->switchNode[1]);
}

// Decomp: CoDMPServer.c:610159  RVA: 0x007CA0B0  Retail: N/A
void __cdecl G_VehSetUpPathPos(vehicle_pathpos_t *vpp, __int16 nodeIdx)
{
    float slideBlend;
    vehicle_node_t *node;
    node = &s_nodes[nodeIdx];
    vpp->lastNodeIdx = nodeIdx;
    vpp->nodeIdx = nodeIdx;
    vpp->endOfPath = 0;
    vpp->frac = 0.0f;
    vpp->speed = node->speed;
    vpp->lookAhead = node->lookAhead;
    if ( (node->flags & 0x10000) != 0 )
        slideBlend = 1.0f;
    else
        slideBlend = 0.0f;
    vpp->slide = slideBlend;
    vpp->flags = 0;
    vpp->customPath = 0;
    vpp->origin[0] = node->origin[0];
    vpp->origin[1] = node->origin[1];
    vpp->origin[2] = node->origin[2];
    vpp->angles[0] = node->angles[0];
    vpp->angles[1] = node->angles[1];
    vpp->angles[2] = node->angles[2];
    vpp->lookPos[0] = node->origin[0];
    vpp->lookPos[1] = node->origin[1];
    vpp->lookPos[2] = node->origin[2];
    VP_InitNode(vpp->switchNode, -1);
    VP_InitNode(&vpp->switchNode[1], -1);
}

// Decomp: CoDMPServer.c:610198  RVA: 0x007CA220  Retail: N/A
int __cdecl G_VehUpdatePathPos(vehicle_pathpos_t *vpp, __int16 testNode)
{
    float moveStep;
    float dist;
    float lookDir[3];
    int test;
    test = 0;
    if ( vpp->endOfPath )
        return 0;
    VP_BeginSwitchNode(vpp);
    if ( vpp->customPath )
        VP_GetLookAheadXYZCustom(vpp, vpp->lookPos);
    else
        VP_GetLookAheadXYZ(vpp, vpp->lookPos);
    lookDir[0] = vpp->lookPos[0] - vpp->origin[0];
    lookDir[1] = vpp->lookPos[1] - vpp->origin[1];
    lookDir[2] = vpp->lookPos[2] - vpp->origin[2];
    dist = Vec3Normalize(lookDir);
    if ( dist <= 0.0 )
    {
        vpp->endOfPath = 1;
    }
    else
    {
        vectoangles(lookDir, vpp->angles);
        vpp->angles[0] = AngleNormalize180(vpp->angles[0]);
        vpp->angles[1] = AngleNormalize180(vpp->angles[1]);
        vpp->angles[2] = AngleNormalize180(vpp->angles[2]);
        moveStep = vpp->speed * 0.050000001;
        vpp->origin[0] = (float)(moveStep * lookDir[0]) + vpp->origin[0];
        vpp->origin[1] = (float)(moveStep * lookDir[1]) + vpp->origin[1];
        vpp->origin[2] = (float)(moveStep * lookDir[2]) + vpp->origin[2];
        if ( vpp->customPath )
            test = VP_UpdatePathPosCustom(vpp, lookDir, testNode);
        else
            test = VP_UpdatePathPos(vpp, lookDir, testNode);
        VP_GetAngles(vpp, vpp->angles);
    }
    VP_EndSwitchNode(vpp);
    return test;
}

// Decomp: CoDMPServer.c:610247  RVA: 0x007CA430  Retail: N/A
void __cdecl VP_GetAngles(const vehicle_pathpos_t *vpp, float *angles)
{
    double angleDelta;
    double normalizedAngle;
    float startAngle;
    float frac;
    vehicle_node_t *node;
    float endAngles[3];
    __int16 nodeIdx;
    vehicle_node_t *next;
    __int16 nextNodeIdx;
    float startAngles[3];
    int i;
    nodeIdx = VPP_GetCurrentNode(vpp, vpp->nodeIdx);
    node = &s_nodes[nodeIdx];
    if ( (node->flags & 2) != 0 )
        nextNodeIdx = VPP_GetCurrentNode(vpp, vpp->nodeIdx + 1);
    else
        nextNodeIdx = node->splineNode.nextIdx;
    if ( nextNodeIdx >= 0 )
    {
        next = &s_nodes[nextNodeIdx];
        if ( (node->flags & 0x10000) != 0 || (next->flags & 0x10000) != 0 )
        {
            if ( (node->flags & 0x10000) != 0 && (next->flags & 0x10000) != 0 )
            {
                startAngles[0] = node->angles[0];
                startAngles[1] = node->angles[1];
                startAngles[2] = node->angles[2];
                endAngles[0] = next->angles[0];
                endAngles[1] = next->angles[1];
                endAngles[2] = next->angles[2];
            }
            else if ( (node->flags & 0x10000) != 0 || (next->flags & 0x10000) == 0 )
            {
                startAngles[0] = node->angles[0];
                startAngles[1] = node->angles[1];
                startAngles[2] = node->angles[2];
                endAngles[0] = *angles;
                endAngles[1] = angles[1];
                endAngles[2] = angles[2];
            }
            else
            {
                startAngles[0] = *angles;
                startAngles[1] = angles[1];
                startAngles[2] = angles[2];
                endAngles[0] = next->angles[0];
                endAngles[1] = next->angles[1];
                endAngles[2] = next->angles[2];
            }
            for ( i = 0; i < 3; ++i )
            {
                startAngle = startAngles[i];
                frac = vpp->frac;
                angleDelta = AngleNormalize180(endAngles[i] - startAngle);
                angles[i] = angleDelta * frac + startAngle;
                normalizedAngle = AngleNormalize180(angles[i]);
                angles[i] = normalizedAngle;
            }
        }
    }
    else if ( (node->flags & 0x10000) != 0 )
    {
        *angles = node->angles[0];
        angles[1] = node->angles[1];
        angles[2] = node->angles[2];
    }
}

// Decomp: CoDMPServer.c:610323  RVA: 0x007CA6E0  Retail: N/A
__int16 __cdecl VPP_GetCurrentNode(const vehicle_pathpos_t *vpp, __int16 currentNode)
{
    if ( vpp->customPath )
        return vpp->customPath->pathOrder[currentNode];
    else
        return currentNode;
}

// Decomp: CoDMPServer.c:610335  RVA: 0x007CA710  Retail: N/A
void __cdecl VP_GetLookAheadXYZCustom(const vehicle_pathpos_t *vpp, float *lookXYZ)
{
    vehicle_node_t *node;
    __int16 nodeIdx;
    vehicle_path_node_link_t *link;
    float dist;
    __int16 pathIdx;
    float length;
    pathIdx = vpp->nodeIdx;
    node = &s_nodes[vpp->customPath->pathOrder[vpp->nodeIdx]];
    link = &s_node_links[vpp->customPath->pathLinkIdx[vpp->nodeIdx]];
    dist = (float)(vpp->frac * link->length) + vpp->lookAhead;
    while ( 1 )
    {
        length = link->length;
        nodeIdx = vpp->customPath->pathOrder[++pathIdx];
        if ( nodeIdx < 0 || length == 0.0 )
            break;
        if ( length > dist )
        {
            *lookXYZ = (float)(dist * link->dir[0]) + node->origin[0];
            lookXYZ[1] = (float)(dist * link->dir[1]) + node->origin[1];
            lookXYZ[2] = (float)(dist * link->dir[2]) + node->origin[2];
            return;
        }
        dist = dist - length;
        node = &s_nodes[nodeIdx];
        link = &s_node_links[vpp->customPath->pathLinkIdx[pathIdx]];
    }
    if ( dist < 0.0 )
        dist = 0.0f;
    *lookXYZ = (float)(dist * vpp->customGoalDir[0]) + node->origin[0];
    lookXYZ[1] = (float)(dist * vpp->customGoalDir[1]) + node->origin[1];
    lookXYZ[2] = (float)(dist * vpp->customGoalDir[2]) + node->origin[2];
}

// Decomp: CoDMPServer.c:610373  RVA: 0x007CA930  Retail: N/A
void __cdecl VP_GetLookAheadXYZ(const vehicle_pathpos_t *vpp, float *lookXYZ)
{
    vehicle_node_t *node;
    float dist;
    __int16 count;
    node = &s_nodes[vpp->nodeIdx];
    dist = (float)(vpp->frac * node->splineNode.length) + (float)(vpp->lookAhead * vpp->speed);
    count = 0;
    while ( count < s_numNodes )
    {
        ++count;
        if ( node->splineNode.nextIdx < 0 || node->splineNode.length == 0.0 )
        {
            dist = 0.0f;
            break;
        }
        if ( node->splineNode.length > dist )
            break;
        dist = dist - node->splineNode.length;
        node = &s_nodes[node->splineNode.nextIdx];
    }
    *lookXYZ = (float)(dist * node->splineNode.dir[0]) + node->origin[0];
    lookXYZ[1] = (float)(dist * node->splineNode.dir[1]) + node->origin[1];
    lookXYZ[2] = (float)(dist * node->splineNode.dir[2]) + node->origin[2];
}

// Decomp: CoDMPServer.c:610401  RVA: 0x007CAA70  Retail: N/A
int __cdecl VP_UpdatePathPosCustom(vehicle_pathpos_t *vpp, const float *dir, __int16 nodeTest)
{
    float clampedFrac;
    float cappedFrac;
    float bisectorNormal[3];
    float distAlongPrevEdge;
    __int16 nodeIdx;
    float frac;
    float segmentFrac;
    __int16 linkIdx;
    vehicle_path_node_link_t *nextLink;
    int test;
    vehicle_path_node_link_t *currentLink;
    float vppOffsetFromCNode[3];
    float distAlongBisector;
    __int16 nextNodeIdx;
    __int16 pathIdx;
    __int16 prevLinkIdx;
    vehicle_node_t *currentNode;
    vehicle_node_t *nextNode;
    float vppToNextNode[3];
    pathIdx = vpp->nodeIdx;
    nodeIdx = vpp->customPath->pathOrder[vpp->nodeIdx];
    linkIdx = vpp->customPath->pathLinkIdx[vpp->nodeIdx];
    test = 0;
    while ( 1 )
    {
        while ( 1 )
        {
            currentNode = &s_nodes[nodeIdx];
            currentLink = &s_node_links[linkIdx];
            nextNodeIdx = vpp->customPath->pathOrder[pathIdx + 1];
            nextNode = &s_nodes[nextNodeIdx];
            nextLink = &s_node_links[vpp->customPath->pathLinkIdx[pathIdx + 1]];
            if ( (nextNode->flags & 0x80000) != 0 )
                nextNodeIdx = -1;
            if ( pathIdx <= 0 )
                prevLinkIdx = linkIdx;
            else
                prevLinkIdx = vpp->customPath->pathLinkIdx[pathIdx - 1];
            if ( nodeIdx == nodeTest )
                test = 1;
            if ( nextNodeIdx < 0 || currentLink->length == 0.0 )
            {
                frac = 0.0f;
                goto LABEL_30;
            }
            bisectorNormal[0] = 0.5 * (float)(currentLink->dir[0] + nextLink->dir[0]);
            bisectorNormal[1] = 0.5 * (float)(currentLink->dir[1] + nextLink->dir[1]);
            bisectorNormal[2] = 0.5 * (float)(currentLink->dir[2] + nextLink->dir[2]);
            vppOffsetFromCNode[0] = vpp->origin[0] - currentNode->origin[0];
            vppOffsetFromCNode[1] = vpp->origin[1] - currentNode->origin[1];
            vppOffsetFromCNode[2] = vpp->origin[2] - currentNode->origin[2];
            distAlongPrevEdge = (float)((float)(vppOffsetFromCNode[0] * (float)(0.5 * (float)(s_node_links[prevLinkIdx].dir[0] + currentLink->dir[0])))
                                 + (float)(vppOffsetFromCNode[1] * (float)(0.5 * (float)(s_node_links[prevLinkIdx].dir[1] + currentLink->dir[1]))))
                 + (float)(vppOffsetFromCNode[2] * (float)(0.5 * (float)(s_node_links[prevLinkIdx].dir[2] + currentLink->dir[2])));
            vppToNextNode[0] = nextNode->origin[0] - vpp->origin[0];
            vppToNextNode[1] = nextNode->origin[1] - vpp->origin[1];
            vppToNextNode[2] = nextNode->origin[2] - vpp->origin[2];
            distAlongBisector = (float)((float)(vppToNextNode[0] * bisectorNormal[0]) + (float)(vppToNextNode[1] * bisectorNormal[1]))
                 + (float)(vppToNextNode[2] * bisectorNormal[2]);
            if ( distAlongPrevEdge == 0.0 && distAlongBisector == 0.0 )
            {
                frac = 0.0f;
                goto LABEL_30;
            }
            if ( distAlongBisector >= 0.0 )
                break;
            ++pathIdx;
            nodeIdx = nextNodeIdx;
        }
        if ( distAlongPrevEdge >= 0.0 )
            break;
        if ( (float)((float)((float)(currentLink->dir[0] * vppOffsetFromCNode[0]) + (float)(currentLink->dir[1] * vppOffsetFromCNode[1]))
                             + (float)(currentLink->dir[2] * vppOffsetFromCNode[2])) < 0.0 )
        {
            frac = 0.0f;
            goto LABEL_30;
        }
        segmentFrac = 1.0 - Abs(vppToNextNode) / currentLink->length;
        if ( (float)(segmentFrac - 1.0) < 0.0 )
            cappedFrac = segmentFrac;
        else
            cappedFrac = 1.0f;
        if ( (float)(0.0 - segmentFrac) < 0.0 )
            clampedFrac = cappedFrac;
        else
            clampedFrac = 0.0f;
        frac = clampedFrac;
        if ( clampedFrac <= 0.94999999 )
            goto LABEL_30;
        ++pathIdx;
        nodeIdx = nextNodeIdx;
    }
    frac = distAlongPrevEdge / (float)(distAlongPrevEdge + distAlongBisector);
LABEL_30:
    vpp->endOfPath = nextNodeIdx < 0;
    vpp->nodeIdx = pathIdx;
    vpp->frac = frac;
    vpp->speed = VP_GetSpeed(vpp);
    vpp->lookAhead = VP_GetLookAhead(vpp);
    vpp->slide = VP_GetSlide(vpp);
    return test;
}

// Decomp: CoDMPServer.c:610516  RVA: 0x007CB040  Retail: N/A
double __cdecl VP_GetSpeed(const vehicle_pathpos_t *vpp)
{
    vehicle_node_t *node;
    __int16 nodeIdx;
    __int16 nextNodeIdx;
    if ( vpp->customPath )
    {
        nodeIdx = vpp->customPath->pathOrder[vpp->nodeIdx];
        nextNodeIdx = vpp->customPath->pathOrder[vpp->nodeIdx + 1];
    }
    else
    {
        nodeIdx = vpp->nodeIdx;
        nextNodeIdx = s_nodes[vpp->nodeIdx].splineNode.nextIdx;
    }
    node = &s_nodes[nodeIdx];
    if ( nextNodeIdx >= 0 )
        return (float)((float)((float)(s_nodes[nextNodeIdx].speed - node->speed) * vpp->frac) + node->speed);
    else
        return node->speed;
}

// Decomp: CoDMPServer.c:610542  RVA: 0x007CB100  Retail: N/A
double __cdecl VP_GetLookAhead(const vehicle_pathpos_t *vpp)
{
    vehicle_node_t *node;
    __int16 nodeIdx;
    __int16 nextNodeIdx;
    if ( vpp->customPath )
    {
        nodeIdx = vpp->customPath->pathOrder[vpp->nodeIdx];
        nextNodeIdx = vpp->customPath->pathOrder[vpp->nodeIdx + 1];
    }
    else
    {
        nodeIdx = vpp->nodeIdx;
        nextNodeIdx = s_nodes[vpp->nodeIdx].splineNode.nextIdx;
    }
    node = &s_nodes[nodeIdx];
    // Decomp: CoDMPServer.c:610560 used s_nodes[nodeIdx].splineNode.nextIdx; use nextNodeIdx (path successor) like VP_GetSpeed.
    if ( nextNodeIdx >= 0 )
        return (float)((float)((float)(s_nodes[nextNodeIdx].lookAhead - node->lookAhead) * vpp->frac) + node->lookAhead);
    else
        return node->lookAhead;
}

// Decomp: CoDMPServer.c:610568  RVA: 0x007CB1C0  Retail: N/A
double __cdecl VP_GetSlide(const vehicle_pathpos_t *vpp)
{
    vehicle_node_t *node;
    __int16 nodeIdx;
    vehicle_node_t *next;
    __int16 nextNodeIdx;
    if ( vpp->customPath )
    {
        nodeIdx = vpp->customPath->pathOrder[vpp->nodeIdx];
        nextNodeIdx = vpp->customPath->pathOrder[vpp->nodeIdx + 1];
    }
    else
    {
        nodeIdx = vpp->nodeIdx;
        nextNodeIdx = s_nodes[vpp->nodeIdx].splineNode.nextIdx;
    }
    node = &s_nodes[nodeIdx];
    if ( nextNodeIdx >= 0 )
    {
        next = &s_nodes[nextNodeIdx];
        if ( (node->flags & 0x10000) != 0 && (next->flags & 0x10000) != 0 )
        {
            return 1.0;
        }
        else if ( (node->flags & 0x10000) != 0 || (next->flags & 0x10000) == 0 )
        {
            if ( (node->flags & 0x10000) == 0 || (next->flags & 0x10000) != 0 )
                return 0.0;
            else
                return 1.0 - vpp->frac;
        }
        else
        {
            return vpp->frac;
        }
    }
    else if ( (node->flags & 0x10000) != 0 )
    {
        return 1.0f;
    }
    else
    {
        return 0.0f;
    }
}

static const float tightTurnThreshold = 0.866;
// Decomp: CoDMPServer.c:610619  RVA: 0x007CB2F0  Retail: N/A
int __cdecl VP_UpdatePathPos(vehicle_pathpos_t *vpp, const float *dir, __int16 nodeTest)
{
    double distToNextNode;
    float clampedFrac;
    float cappedFrac;
    float bisectorNormal[3];
    vehicle_node_t *prevNode;
    float entryPlaneNormal[3];
    float angleDiff;
    float distAlongEntryPlane;
    __int16 nodeIdx;
    float frac;
    __int16 lastNodeIdx;
    int test;
    float vppOffsetFromCurrent[3];
    float distAlongBisector;
    vehicle_node_t *currentNode;
    vehicle_node_t *nextNode;
    float vppToNextNode[3];
    __int16 count;
    nodeIdx = vpp->nodeIdx;
    lastNodeIdx = vpp->lastNodeIdx;
    currentNode = &s_nodes[nodeIdx];
    frac = vpp->frac;
    test = 0;
    count = 0;
    while ( count < s_numNodes )
    {
        ++count;
        currentNode = &s_nodes[nodeIdx];
        if ( nodeIdx == nodeTest )
            test = 1;
        if ( currentNode->splineNode.nextIdx < 0 || currentNode->splineNode.length == 0.0 )
        {
            frac = 0.0f;
            break;
        }
        nextNode = &s_nodes[currentNode->splineNode.nextIdx];
        prevNode = &s_nodes[lastNodeIdx];
        bisectorNormal[0] = currentNode->splineNode.dir[0] + nextNode->splineNode.dir[0];
        bisectorNormal[1] = currentNode->splineNode.dir[1] + nextNode->splineNode.dir[1];
        bisectorNormal[2] = currentNode->splineNode.dir[2] + nextNode->splineNode.dir[2];
        Vec3Normalize(bisectorNormal);
        angleDiff = (float)((float)(prevNode->splineNode.dir[0] * currentNode->splineNode.dir[0])
                                            + (float)(prevNode->splineNode.dir[1] * currentNode->splineNode.dir[1]))
                            + (float)(prevNode->splineNode.dir[2] * currentNode->splineNode.dir[2]);
        if ( (-(tightTurnThreshold)) <= angleDiff )
        {
            entryPlaneNormal[0] = prevNode->splineNode.dir[0] + currentNode->splineNode.dir[0];
            entryPlaneNormal[1] = prevNode->splineNode.dir[1] + currentNode->splineNode.dir[1];
            entryPlaneNormal[2] = prevNode->splineNode.dir[2] + currentNode->splineNode.dir[2];
            Vec3Normalize(entryPlaneNormal);
        }
        else
        {
            entryPlaneNormal[0] = currentNode->splineNode.dir[0];
            entryPlaneNormal[1] = currentNode->splineNode.dir[1];
            entryPlaneNormal[2] = currentNode->splineNode.dir[2];
        }
        vppOffsetFromCurrent[0] = vpp->origin[0] - currentNode->origin[0];
        vppOffsetFromCurrent[1] = vpp->origin[1] - currentNode->origin[1];
        vppOffsetFromCurrent[2] = vpp->origin[2] - currentNode->origin[2];
        distAlongEntryPlane = (float)((float)(vppOffsetFromCurrent[0] * entryPlaneNormal[0]) + (float)(vppOffsetFromCurrent[1] * entryPlaneNormal[1]))
             + (float)(vppOffsetFromCurrent[2] * entryPlaneNormal[2]);
        vppToNextNode[0] = nextNode->origin[0] - vpp->origin[0];
        vppToNextNode[1] = nextNode->origin[1] - vpp->origin[1];
        vppToNextNode[2] = nextNode->origin[2] - vpp->origin[2];
        distAlongBisector = (float)((float)(vppToNextNode[0] * bisectorNormal[0]) + (float)(vppToNextNode[1] * bisectorNormal[1]))
             + (float)(vppToNextNode[2] * bisectorNormal[2]);
        if ( distAlongBisector >= 0.0 )
        {
            if ( distAlongEntryPlane >= 0.0 )
            {
                frac = distAlongEntryPlane / (float)(distAlongEntryPlane + distAlongBisector);
                break;
            }
            distAlongEntryPlane = (float)((float)(currentNode->splineNode.dir[0] * vppOffsetFromCurrent[0]) + (float)(currentNode->splineNode.dir[1] * vppOffsetFromCurrent[1]))
                 + (float)(currentNode->splineNode.dir[2] * vppOffsetFromCurrent[2]);
            if ( distAlongEntryPlane < 0.0 )
            {
                frac = 0.0f;
                break;
            }
            distToNextNode = Abs(vppToNextNode);
            frac = 1.0 - distToNextNode / currentNode->splineNode.length;
            if ( (float)(frac - 1.0) < 0.0 )
                cappedFrac = frac;
            else
                cappedFrac = 1.0f;
            if ( (float)(0.0 - frac) < 0.0 )
                clampedFrac = cappedFrac;
            else
                clampedFrac = 0.0f;
            frac = clampedFrac;
            if ( clampedFrac <= 0.89999998 )
                break;
            lastNodeIdx = nodeIdx;
            nodeIdx = currentNode->splineNode.nextIdx;
        }
        else
        {
            lastNodeIdx = nodeIdx;
            nodeIdx = currentNode->splineNode.nextIdx;
        }
    }
    vpp->endOfPath = currentNode->splineNode.nextIdx < 0;
    vpp->lastNodeIdx = lastNodeIdx;
    vpp->nodeIdx = nodeIdx;
    vpp->frac = frac;
    vpp->speed = VP_GetSpeed(vpp);
    vpp->lookAhead = VP_GetLookAhead(vpp);
    vpp->slide = VP_GetSlide(vpp);
    return test;
}

// Decomp: CoDMPServer.c:610743  RVA: 0x007CB850  Retail: N/A
void __cdecl VP_BeginSwitchNode(const vehicle_pathpos_t *vpp)
{
    __int16 nodeIdx;
    nodeIdx = VP_GetNodeIndex(vpp->switchNode[0].name, 0);
    if ( nodeIdx >= 0 )
        VP_CopyNode(vpp->switchNode, &s_nodes[nodeIdx]);
}

// Decomp: CoDMPServer.c:610753  RVA: 0x007CB8A0  Retail: N/A
void __cdecl VP_CopyNode(const vehicle_node_t *src, vehicle_node_t *dst)
{
    Scr_SetString(&dst->name, src->name, SCRIPTINSTANCE_SERVER);
    Scr_SetString(&dst->target, src->target, SCRIPTINSTANCE_SERVER);
    Scr_SetString(&dst->target2, src->target2, SCRIPTINSTANCE_SERVER);
    Scr_SetString(&dst->script_linkname, src->script_linkname, SCRIPTINSTANCE_SERVER);
    Scr_SetString(&dst->script_noteworthy, src->script_noteworthy, SCRIPTINSTANCE_SERVER);
    dst->index = src->index;
    dst->flags = src->flags;
    dst->speed = src->speed;
    dst->lookAhead = src->lookAhead;
    dst->origin[0] = src->origin[0];
    dst->origin[1] = src->origin[1];
    dst->origin[2] = src->origin[2];
    dst->splineNode.dir[0] = src->splineNode.dir[0];
    dst->splineNode.dir[1] = src->splineNode.dir[1];
    dst->splineNode.dir[2] = src->splineNode.dir[2];
    dst->angles[0] = src->angles[0];
    dst->angles[1] = src->angles[1];
    dst->angles[2] = src->angles[2];
    dst->splineNode.length = src->splineNode.length;
    *(unsigned int *)&dst->splineNode.nextIdx = *(unsigned int *)&src->splineNode.nextIdx;
}

// Decomp: CoDMPServer.c:610785  RVA: 0x007CBA20  Retail: N/A
void __cdecl VP_EndSwitchNode(const vehicle_pathpos_t *vpp)
{
    __int16 nodeIdx;
    nodeIdx = VP_GetNodeIndex(vpp->switchNode[0].name, 0);
    if ( nodeIdx >= 0 )
        VP_CopyNode(&vpp->switchNode[1], &s_nodes[nodeIdx]);
}

// Decomp: CoDMPServer.c:610795  RVA: 0x007CBA70  Retail: N/A
void __cdecl G_VehSetSwitchNode(vehicle_pathpos_t *vpp, __int16 srcNodeIdx, __int16 dstNodeIdx)
{
    vehicle_node_t *srcNode;
    vehicle_node_t *dstNode;
    G_VehFreePathPos(vpp);
    VP_InitNode(vpp->switchNode, -1);
    VP_InitNode(&vpp->switchNode[1], -1);
    if ( srcNodeIdx >= 0 && dstNodeIdx >= 0 )
    {
        if ( vpp->nodeIdx == srcNodeIdx )
            vpp->frac = 0.0f;
        srcNode = &s_nodes[srcNodeIdx];
        dstNode = &s_nodes[dstNodeIdx];
        VP_CopyNode(srcNode, vpp->switchNode);
        VP_CopyNode(srcNode, &vpp->switchNode[1]);
        vpp->switchNode[0].splineNode.nextIdx = dstNodeIdx;
        vpp->switchNode[0].splineNode.dir[0] = dstNode->origin[0] - srcNode->origin[0];
        vpp->switchNode[0].splineNode.dir[1] = dstNode->origin[1] - srcNode->origin[1];
        vpp->switchNode[0].splineNode.dir[2] = dstNode->origin[2] - srcNode->origin[2];
        vpp->switchNode[0].splineNode.length = Vec3Normalize(vpp->switchNode[0].splineNode.dir);
    }
}

// Decomp: CoDMPServer.c:610828  RVA: 0x007CBBE0  Retail: N/A
vehicle_node_t *__cdecl G_FindVehicleNode(const SpawnVar *spawnVar)
{
    vehicle_node_t *node;
    float dist2;
    int score;
    int i;
    int best_score;
    vehicle_node_t compare_node;
    vehicle_node_t *best_node;
    VP_InitNode(&compare_node, 0);
    VP_ParseFields(&compare_node, spawnVar, 0);
    best_score = 1;
    best_node = 0;
    for ( i = 0; i < s_numNodes; ++i )
    {
        score = 0;
        node = &s_nodes[i];
        dist2 = Vec3DistanceSq(node->origin, compare_node.origin);
        if ( dist2 < 100.0 )
        {
            score = 1;
            if ( dist2 < 0.5 )
                score = 4;
        }
        if ( node->name && node->name == compare_node.name )
            ++score;
        if ( node->target && node->target == compare_node.target )
            ++score;
        if ( node->target2 && node->target2 == compare_node.target2 )
            ++score;
        if ( node->script_linkname && node->script_linkname == compare_node.script_linkname )
            ++score;
        if ( node->script_noteworthy && node->script_noteworthy == compare_node.script_noteworthy )
            ++score;
        if ( score > best_score )
        {
            best_node = &s_nodes[i];
            best_score = score;
        }
    }
    return best_node;
}

// Decomp: CoDMPServer.c:610880  RVA: 0x007CBD60  Retail: N/A
void __cdecl VP_ParseFields(vehicle_node_t *node, const SpawnVar *spawnVar, bool setScriptVariable)
{
    int i;
    if ( !spawnVar->spawnVarsValid
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\g_vehicle_path.cpp",
                    339,
                    0,
                    "%s",
                    "spawnVar.spawnVarsValid") )
    {
        __debugbreak();
    }
    for ( i = 0; i < spawnVar->numSpawnVars; ++i )
        VP_ParseField(spawnVar->spawnVars[i][0], spawnVar->spawnVars[i][1], node, setScriptVariable);
}

// Decomp: CoDMPServer.c:610900  RVA: 0x007CBDE0  Retail: N/A
void __cdecl VP_ParseField(const char *key, char *value, vehicle_node_t *node, bool setScriptVariable)
{
    float floatValue;
    vn_field_t *f;
    float vec[3];
    for ( f = vn_fields; f->name; ++f )
    {
        if ( !I_stricmp(f->name, key) )
        {
            switch ( f->type )
            {
                case F_INT:
                    *(unsigned int *)((char *)&node->name + f->ofs) = atoi(value);
                    break;
                case F_SHORT:
                    *(unsigned __int16 *)((char *)&node->name + f->ofs) = atoi(value);
                    break;
                case F_BYTE:
                    *((_BYTE *)&node->name + f->ofs) = atoi(value);
                    break;
                case F_FLOAT:
                    floatValue = atof(value);
                    *(float *)((char *)&node->name + f->ofs) = floatValue;
                    break;
                case F_STRING:
                    Scr_SetStringLiveUpdateSafe((unsigned __int16 *)((char *)&node->name + f->ofs), value, SCRIPTINSTANCE_SERVER);
                    break;
                case F_VECTOR:
                    memset(vec, 0, sizeof(vec));
                    sscanf(value, "%f %f %f", vec, &vec[1], &vec[2]);
                    *(float *)((char *)&node->name + f->ofs) = vec[0];
                    *(float *)((char *)&node->target2 + f->ofs) = vec[1];
                    *(float *)((char *)&node->script_noteworthy + f->ofs) = vec[2];
                    break;
                case F_VECTORHACK:
                    memset(vec, 0, sizeof(vec));
                    sscanf(value, "%f %f %f", vec, &vec[1], &vec[2]);
                    *(float *)((char *)&node->name + f->ofs) = AngleNormalize360(vec[1]);
                    break;
                default:
                    return;
            }
            return;
        }
    }
    if ( setScriptVariable )
        VP_SetScriptVariable(key, value, node);
}

// Decomp: CoDMPServer.c:610957  RVA: 0x007CC020  Retail: N/A
void __cdecl VP_SetScriptVariable(const char *key, char *value, vehicle_node_t *node)
{
    int intValue;
    const char *assertMsg;
    float floatValue;
    unsigned int index;
    int type;
    index = Scr_FindField(key, &type, SCRIPTINSTANCE_SERVER);
    if ( index )
    {
        switch ( type )
        {
            case 2:
                Scr_AddString(value, SCRIPTINSTANCE_SERVER);
                break;
            case 5:
                floatValue = atof(value);
                Scr_AddFloat(floatValue, SCRIPTINSTANCE_SERVER);
                break;
            case 6:
                intValue = atoi(value);
                Scr_AddInt(intValue, SCRIPTINSTANCE_SERVER);
                break;
            default:
                assertMsg = va("VP_SetScriptVariable: bad case %d", type);
                if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\g_vehicle_path.cpp", 249, 0, assertMsg) )
                    __debugbreak();
                return;
        }
        Scr_SetDynamicEntityField(node->index, 3u, index, SCRIPTINSTANCE_SERVER, 0);
    }
}

// Decomp: CoDMPServer.c:610994  RVA: 0x007CC100  Retail: N/A
void __cdecl SP_info_vehicle_node(const SpawnVar *spawnVar, int rotated, int radiantLiveUpdate)
{
    vehicle_node_t *node;
    if ( s_numNodes >= 2000 )
        Com_Error(ERR_DROP, "Hit max vehicle path node count [%d]", 2000);
    node = &s_nodes[s_numNodes];
    VP_InitNode(node, s_numNodes);
    ++s_numNodes;
    VP_ParseFields(node, spawnVar, 1);
    if ( rotated )
        node->flags |= 0x10000u;
    if ( !node->name && !radiantLiveUpdate && (node->flags & 2) == 0 )
        Com_Error(ERR_DROP, "Vehicle spline path node( %f, %f, %f ) found with no name", node->origin[0], node->origin[1], node->origin[2]);
    if ( node->speed >= 0.0 )
    {
        node->flags |= 0x20000u;
        node->speed = node->speed * 17.6;
    }
    if ( node->lookAhead >= 0.0 )
        node->flags |= 0x40000u;
}

// Decomp: CoDMPServer.c:611018  RVA: 0x007CC250  Retail: N/A
void __cdecl G_ClearSelectedVehicleNode()
{
    g_radiant_selected_node = 0;
}

// Decomp: CoDMPServer.c:611024  RVA: 0x007CC260  Retail: N/A
void __cdecl G_ProcessVehicleNodeCommand(const RadiantCommand *command, SpawnVar *spawnVar)
{
    unsigned int newFlags;
    vehicle_node_t *node;
    RadiantCommandType commandType;
    char *classname;
    bool rotated;
    node = 0;
    commandType = command->type;
    classname = GetPairValue(spawnVar, "classname");
    rotated = I_strcmp("info_vehicle_node_rotate", classname) == 0;
    if ( commandType == RADIANT_COMMAND_CREATE )
    {
        SP_info_vehicle_node(spawnVar, rotated, 1);
        g_radiant_selected_node = &s_nodes[s_numNodes - 1];
        G_SetupVehiclePaths();
        Com_Printf(5, "Radiant Live Update: Created new vehicle node\n");
        G_AssignGameIdMapping(command->liveUpdateId, g_radiant_selected_node->index);
        AddSavedCommand(command);
    }
    else if ( commandType == RADIANT_COMMAND_DELETE )
    {
        g_radiant_selected_node = G_FindVehicleNode(spawnVar);
        if ( g_radiant_selected_node )
        {
            G_AssignGameIdMapping(command->liveUpdateId, g_radiant_selected_node->index);
            AddSavedCommand(command);
            VP_InitNode(g_radiant_selected_node, -1);
            g_radiant_selected_node = 0;
            G_SetupVehiclePaths();
        }
    }
    else
    {
        if ( commandType )
        {
            if ( commandType == RADIANT_COMMAND_UPDATE_SELECTED )
            {
                node = g_radiant_selected_node;
            }
            else if ( commandType == RADIANT_COMMAND_UPDATE )
            {
                node = G_FindVehicleNode(spawnVar);
            }
        }
        else
        {
            g_radiant_selected_node = G_FindVehicleNode(spawnVar);
            node = g_radiant_selected_node;
            if ( !g_radiant_selected_node )
                Com_Printf(5, "Radiant Live Update: Can't find vehicle node.    Maps out of sync (Radiant/Game), re-bsp!\n");
        }
        if ( node )
        {
            node->speed = -1.0f;
            node->lookAhead = -1.0f;
            node->flags = 0;
            VP_ParseFields(node, spawnVar, 1);
            if ( rotated )
                newFlags = node->flags | 0x10000;
            else
                newFlags = node->flags & 0xFFFEFFFF;
            node->flags = newFlags;
            if ( node->speed >= 0.0 )
            {
                node->flags |= 0x20000u;
                node->speed = node->speed * 17.6;
            }
            if ( node->lookAhead >= 0.0 )
                node->flags |= 0x40000u;
            G_SetupVehiclePaths();
            G_AssignGameIdMapping(command->liveUpdateId, node->index);
            AddSavedCommand(command);
        }
    }
}

// Decomp: CoDMPServer.c:611109  RVA: 0x007CC4E0  Retail: N/A
unsigned __int16 __cdecl GScr_GetVehicleNodeIndex(unsigned int index)
{
    scr_entref_t entref;
    entref = Scr_GetEntityRef(index, SCRIPTINSTANCE_SERVER);
    if ( entref.classnum == 3 )
    {
        if ( entref.entnum >= s_numNodes
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\universal\\g_vehicle_path.cpp",
                        2567,
                        0,
                        "%s",
                        "entref.entnum < s_numNodes") )
        {
            __debugbreak();
        }
        return entref.entnum;
    }
    else
    {
        Scr_ParamError(index, "Not a vehicle node", SCRIPTINSTANCE_SERVER);
        return -1;
    }
}

bool __cdecl GScr_VehicleNodeHasSpawnFlag(unsigned int entnum, unsigned int flag)
{
    if ( entnum >= s_numNodes )
        return false;
    return (s_nodes[entnum].flags & flag) != 0;
}

// Decomp: CoDMPServer.c:611148  RVA: 0x007CC580  Retail: N/A
void __cdecl GScr_AddFieldsForVehicleNode()
{
    vn_field_t *f;
    for ( f = vn_fields; f->name; ++f )
    {
        if ( f - vn_fields != (unsigned __int16)(f - vn_fields)
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\universal\\g_vehicle_path.cpp",
                        2586,
                        0,
                        "%s",
                        "(f - vn_fields) == (unsigned short)( f - vn_fields )") )
        {
            __debugbreak();
        }
        Scr_AddClassField(3u, (char*)f->name, (unsigned __int16)(f - vn_fields), SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDMPServer.c:611170  RVA: 0x007CC620  Retail: N/A
void __cdecl GScr_GetVehicleNodeField(unsigned int entnum, unsigned int offset)
{
    if ( offset >= 0xB
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\g_vehicle_path.cpp",
                    2601,
                    0,
                    "offset doesn't index ARRAY_COUNT( vn_fields ) - 1\n\t%i not in [0, %i)",
                    offset,
                    11) )
    {
        __debugbreak();
    }
    if ( entnum >= s_numNodes
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\g_vehicle_path.cpp",
                    2602,
                    0,
                    "entnum doesn't index s_numNodes\n\t%i not in [0, %i)",
                    entnum,
                    s_numNodes) )
    {
        __debugbreak();
    }
    GScr_GetGenericField((unsigned __int8 *)&s_nodes[entnum], vn_fields[offset].type, vn_fields[offset].ofs, 0);
}

// Decomp: CoDMPServer.c:611200  RVA: 0x007CC6D0  Retail: N/A
void __cdecl GScr_GetVehicleNode()
{
    vehicle_node_t *result;
    unsigned int name;
    int offset;
    char *key;
    vn_field_t *f;
    __int16 i;
    name = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    key = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    offset = Scr_GetOffset(3u, key, SCRIPTINSTANCE_SERVER);
    if ( offset >= 0 )
    {
        if ( (unsigned int)offset >= 0xB
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\universal\\g_vehicle_path.cpp",
                        2668,
                        0,
                        "offset doesn't index ARRAY_COUNT( vn_fields ) - 1\n\t%i not in [0, %i)",
                        offset,
                        11) )
        {
            __debugbreak();
        }
        f = &vn_fields[offset];
        if ( f->type != F_STRING )
            Scr_ParamError(1u, "key is not internally a string", SCRIPTINSTANCE_SERVER);
        result = 0;
        for ( i = 0; i < s_numNodes; ++i )
        {
            if ( *(unsigned __int16 *)((char *)&s_nodes[i].name + f->ofs)
                && *(unsigned __int16 *)((char *)&s_nodes[i].name + f->ofs) == name )
            {
                if ( result )
                    Scr_Error("GetVehicleNode used with more than one node", 0);
                result = &s_nodes[i];
            }
        }
        if ( result )
            Scr_AddEntityNum(result->index, 3u, SCRIPTINSTANCE_SERVER, 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetVehicleNodeField @ 8280E1B8)
void __cdecl GScr_SetVehicleNodeField(unsigned int entnum, unsigned int offset)
{
    vn_field_t *f;

    if ( offset >= 0xB
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\g_vehicle_path.cpp",
                    2621,
                    0,
                    "offset doesn't index ARRAY_COUNT( vn_fields ) - 1\n\t%i not in [0, %i)",
                    offset,
                    11) )
    {
        __debugbreak();
    }
    if ( entnum >= s_numNodes
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\g_vehicle_path.cpp",
                    2622,
                    0,
                    "entnum doesn't index s_numNodes\n\t%i not in [0, %i)",
                    entnum,
                    s_numNodes) )
    {
        __debugbreak();
    }
    f = &vn_fields[offset];
    if ( !f->writable )
        Scr_Error(va("vehicle node field is read-only: %s", f->name), 0);
    GScr_SetGenericField((unsigned __int8 *)&s_nodes[entnum], f->type, f->ofs, 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetVehicleNodeArray @ 8280E4B0)
void __cdecl GScr_GetVehicleNodeArray()
{
    unsigned int name;
    int offset;
    char *key;
    vn_field_t *f;
    __int16 i;

    name = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    key = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    offset = Scr_GetOffset(3u, key, SCRIPTINSTANCE_SERVER);
    if ( offset >= 0 )
    {
        if ( (unsigned int)offset >= 0xB
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\universal\\g_vehicle_path.cpp",
                        2738,
                        0,
                        "offset doesn't index ARRAY_COUNT( vn_fields ) - 1\n\t%i not in [0, %i)",
                        offset,
                        11) )
        {
            __debugbreak();
        }
        f = &vn_fields[offset];
        if ( f->type != F_STRING )
            Scr_ParamError(1u, "key is not internally a string", SCRIPTINSTANCE_SERVER);
        Scr_MakeArray(SCRIPTINSTANCE_SERVER);
        for ( i = 0; i < s_numNodes; ++i )
        {
            if ( *(unsigned __int16 *)((char *)&s_nodes[i].name + f->ofs)
                && *(unsigned __int16 *)((char *)&s_nodes[i].name + f->ofs) == name )
            {
                Scr_AddEntityNum(s_nodes[i].index, 3u, SCRIPTINSTANCE_SERVER, 0);
                Scr_AddArray(SCRIPTINSTANCE_SERVER);
            }
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetAllVehicleNodes @ 8280E648)
void __cdecl GScr_GetAllVehicleNodes()
{
    __int16 i;

    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for ( i = 0; i < s_numNodes; ++i )
    {
        Scr_AddEntityNum(s_nodes[i].index, 3u, SCRIPTINSTANCE_SERVER, 0);
        Scr_AddArray(SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetVehicleNodeEnabled @ 8280E6E0)
void __cdecl GScr_SetVehicleNodeEnabled()
{
    vehicle_node_t *node;
    unsigned __int16 nodeIdx;
    int enabled;

    nodeIdx = GScr_GetVehicleNodeIndex(0);
    enabled = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    if ( nodeIdx >= 0x8000u || (__int16)nodeIdx >= s_numNodes )
        Scr_Error("Invalid vehicle node passed into SetVehicleNodeEnabled", 0);
    node = &s_nodes[nodeIdx];
    if ( (node->flags & VEH_NODE_FLAG_PATH_NODE) == 0 )
        Scr_Error("SetVehicleNodeEnabled must be called on a vehicle path node with the path node flag enabled", 0);
    if ( enabled )
        node->flags &= ~VEH_NODE_FLAG_NO_VEHICLE;
    else
        node->flags |= VEH_NODE_FLAG_NO_VEHICLE;
}

// Decomp: CoDMPServer.c:611246  RVA: 0x007CC810  Retail: N/A
void __cdecl G_SpawnHeliHeightLock(SpawnVar *spawnVar)
{
    float *patchOrigin;
    const char *originString;
    int brushmodel;
    float origin[3];
    const char *modelString;
    const char *targetnameString;
    int modelIndex;
    if (num_heli_height_lock_patches <= 0)
    {
        if (num_heli_height_lock_patches >= 32
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\universal\\g_vehicle_path.cpp",
                2832,
                0,
                "%s",
                "num_heli_height_lock_patches < MAX_HELI_HEIGHT_LOCK_PATCHES"))
        {
            __debugbreak();
        }
        if (num_heli_height_lock_patches < 32)
        {
            G_SpawnString(spawnVar, "origin", 0, &originString);
            G_SpawnString(spawnVar, "model", 0, &modelString);
            G_SpawnString(spawnVar, "targetname", 0, &targetnameString);
            if (*modelString != 42
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\g_vehicle_path.cpp",
                    2842,
                    0,
                    "%s",
                    "modelString[0] == '*'"))
            {
                __debugbreak();
            }
            modelIndex = atoi(modelString + 1);
            if (modelIndex != (unsigned __int16)modelIndex
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\g_vehicle_path.cpp",
                    2846,
                    0,
                    "%s",
                    "modelIndex == (modelNameIndex_t)modelIndex"))
            {
                __debugbreak();
            }
            brushmodel = (unsigned __int16)modelIndex;
            memset(origin, 0, sizeof(origin));
            sscanf(originString, "%f %f %f", origin, &origin[1], &origin[2]);
            heli_height_lock_patches[num_heli_height_lock_patches].brushmodel = brushmodel;
            if (targetnameString)
                heli_height_lock_patches[num_heli_height_lock_patches].targetname = G_NewString(targetnameString);
            else
                heli_height_lock_patches[num_heli_height_lock_patches].targetname = 0;
            heli_height_lock_patches[num_heli_height_lock_patches].enabled = 1;
            patchOrigin = heli_height_lock_patches[num_heli_height_lock_patches].origin;
            *patchOrigin = origin[0];
            patchOrigin[1] = origin[1];
            patchOrigin[2] = origin[2];
            ++num_heli_height_lock_patches;
        }
    }
}
