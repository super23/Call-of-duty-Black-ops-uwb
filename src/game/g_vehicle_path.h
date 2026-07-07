#pragma once

#include <qcommon/common.h>
#include <qcommon/radiant_remote.h>
#include "sentient_fields.h"

// ---------------------------------------------------------------------------
// Vehicle path node flags (spawnflags / runtime)
// ---------------------------------------------------------------------------
#define VEH_NODE_FLAG_PATH_NODE        0x00002   // graph vertex for VP_FindPath
#define VEH_NODE_FLAG_SLIDE_ANGLES     0x10000   // bank from node angles vs velocity heading
#define VEH_NODE_FLAG_SPEED_OVERRIDE   0x20000   // do not inherit speed from neighbors
#define VEH_NODE_FLAG_LOOK_OVERRIDE    0x40000   // do not inherit lookAhead from neighbors
#define VEH_NODE_FLAG_NO_VEHICLE       0x80000   // skip in custom-path graph expansion

#define VEHICLE_MAX_NODES              2000
#define VEHICLE_MAX_NODE_LINKS         2000
#define VEHICLE_MAX_CUSTOM_PATH_LEN    128

// Spline chain segment (G_SetupSplinePaths): target / target2 links, dir/length to next node.
struct vehicle_spline_node_t // sizeof=0x14
{
    __int16 nextIdx;
    __int16 prevIdx;
    float length;
    float dir[3];
};

// Alternate union view of the tail of vehicle_node_t (not used by current setup code).
struct vehicle_path_node_t // sizeof=0x8
{
    __int16 numLinks;
    __int16 firstLinkIndex;
    float radius;
};

// Directed edge in the path graph (G_ConnectVehiclePaths -> s_node_links[]).
// For graph nodes: splineNode.prevIdx = index of first link, splineNode.nextIdx = link count.
struct vehicle_path_node_link_t // sizeof=0x14
{
    __int16 nextIdx;
    // 2 bytes padding
    float length;
    float dir[3];
};

struct vehicle_node_t // sizeof=0x44
{
    unsigned __int16 name;
    unsigned __int16 target;
    unsigned __int16 target2;
    unsigned __int16 script_linkname;
    unsigned __int16 script_noteworthy;
    __int16 index;
    int flags;
    float speed;
    float lookAhead;
    float origin[3];
    float angles[3];
    union
    {
        vehicle_spline_node_t splineNode;
        vehicle_path_node_t pathNode;
    };
};

// Custom A* path: parallel node order and s_node_links indices.
struct __declspec(align(2)) vehicle_custom_path_t // sizeof=0x202
{
    __int16 pathOrder[128];
    __int16 pathLinkIdx[128];
    unsigned __int8 inUse;
    // 1 byte padding
};

// Runtime position along spline or custom path (first member of scr_vehicle_s::pathPos).
struct vehicle_pathpos_t // sizeof=0xDC
{
    __int16 nodeIdx;
    __int16 lastNodeIdx;
    __int16 endOfPath;
    // 2 bytes padding
    float frac;
    float speed;
    float lookAhead;
    float slide;
    float origin[3];
    float angles[3];
    float lookPos[3];
    vehicle_node_t switchNode[2];
    int flags;
    vehicle_custom_path_t *customPath;
    float customGoalLength;
    float customGoalDir[3];
};

struct __declspec(align(4)) vn_field_t // sizeof=0x14
{
    const char *name;
    int ofs;
    int size[1];
    fieldtype_t type;
    bool writable;
    // 3 bytes padding
};

void __cdecl VP_ResetLinks();
__int16 __cdecl VP_GetNextFreePathNode();
void __cdecl VP_ClearNode(vehicle_node_t *node);
__int16 __cdecl VP_GetNodeIndex(unsigned __int16 name, float *origin);
__int16 __cdecl VP_FindClosestPathNode(const float *pos);
vehicle_custom_path_t *__cdecl VP_GetFreeCustomPath();
void __cdecl VP_FreeCustomPath(vehicle_custom_path_t *path);
int __cdecl VP_FindPath(float *start, float *end, vehicle_pathpos_t *vpp);
void __cdecl VP_DrawCustomPath(vehicle_custom_path_t *customPath);
void __cdecl VP_DrawPath(const vehicle_pathpos_t *vpp);
void __cdecl VP_AddDebugLine(const float *start, const float *end, int forceDraw);
void __cdecl VP_DebugArrow(const float *pos, const float *angles);
void __cdecl G_InitVehiclePaths();
void __cdecl G_FreeVehiclePaths();
void __cdecl VP_ResetNode(vehicle_node_t *node);
void __cdecl G_SetupSplinePaths();
double __cdecl VP_CalcNodeSpeed(__int16 nodeIdx);
double __cdecl VP_CalcNodeLookAhead(__int16 nodeIdx);
void __cdecl VP_CalcNodeAngles(__int16 nodeIdx, float *angles);
void __cdecl G_ConnectVehiclePaths();
void __cdecl G_SetupVehiclePaths();
void __cdecl VP_ZeroNode(vehicle_node_t *node);
void __cdecl G_VehInitPathPos(vehicle_pathpos_t *vpp);
void __cdecl VP_InitNode(vehicle_node_t *node, __int16 nodeIdx);
void __cdecl G_VehFreePathPos(vehicle_pathpos_t *vpp);
void __cdecl G_VehSetUpPathPos(vehicle_pathpos_t *vpp, __int16 nodeIdx);
int __cdecl G_VehUpdatePathPos(vehicle_pathpos_t *vpp, __int16 testNode);
void __cdecl VP_GetAngles(const vehicle_pathpos_t *vpp, float *angles);
__int16 __cdecl VPP_GetCurrentNode(const vehicle_pathpos_t *vpp, __int16 currentNode);
void __cdecl VP_GetLookAheadXYZCustom(const vehicle_pathpos_t *vpp, float *lookXYZ);
void __cdecl VP_GetLookAheadXYZ(const vehicle_pathpos_t *vpp, float *lookXYZ);
int __cdecl VP_UpdatePathPosCustom(vehicle_pathpos_t *vpp, const float *dir, __int16 nodeTest);
double __cdecl VP_GetSpeed(const vehicle_pathpos_t *vpp);
double __cdecl VP_GetLookAhead(const vehicle_pathpos_t *vpp);
double __cdecl VP_GetSlide(const vehicle_pathpos_t *vpp);
int __cdecl VP_UpdatePathPos(vehicle_pathpos_t *vpp, const float *dir, __int16 nodeTest);
void __cdecl VP_BeginSwitchNode(const vehicle_pathpos_t *vpp);
void __cdecl VP_CopyNode(const vehicle_node_t *src, vehicle_node_t *dst);
void __cdecl VP_EndSwitchNode(const vehicle_pathpos_t *vpp);
void __cdecl G_VehSetSwitchNode(vehicle_pathpos_t *vpp, __int16 srcNodeIdx, __int16 dstNodeIdx);
vehicle_node_t *__cdecl G_FindVehicleNode(const SpawnVar *spawnVar);
void __cdecl VP_ParseFields(vehicle_node_t *node, const SpawnVar *spawnVar, bool setScriptVariable);
void __cdecl VP_ParseField(const char *key, char *value, vehicle_node_t *node, bool setScriptVariable);
void __cdecl VP_SetScriptVariable(const char *key, char *value, vehicle_node_t *node);
void __cdecl SP_info_vehicle_node(const SpawnVar *spawnVar, int rotated, int radiantLiveUpdate);
void __cdecl G_ClearSelectedVehicleNode();
void __cdecl G_ProcessVehicleNodeCommand(const RadiantCommand *command, SpawnVar *spawnVar);
unsigned __int16 __cdecl GScr_GetVehicleNodeIndex(unsigned int index);
void __cdecl GScr_AddFieldsForVehicleNode();
bool __cdecl GScr_VehicleNodeHasSpawnFlag(unsigned int entnum, unsigned int flag);
void __cdecl GScr_GetVehicleNodeField(unsigned int entnum, unsigned int offset);
void __cdecl GScr_SetVehicleNodeField(unsigned int entnum, unsigned int offset);
void __cdecl GScr_GetVehicleNode();
void __cdecl GScr_GetVehicleNodeArray();
void __cdecl GScr_GetAllVehicleNodes();
void __cdecl GScr_SetVehicleNodeEnabled();
void __cdecl G_SpawnHeliHeightLock(SpawnVar *spawnVar);

extern int num_heli_height_lock_patches;
