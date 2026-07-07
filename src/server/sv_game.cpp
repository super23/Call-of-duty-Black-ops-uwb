#include "sv_game.h"
#include <bgame/bg_local.h>
#ifdef KISAK_SP
#include <server_sp/sv_init_sp.h>
#include <server_sp/sv_main_sp.h>
#include <client_sp/sv_client_sp.h>
#include <qcommon/common.h>
#else
#include <server_mp/sv_main_mp.h>
#include <client_mp/sv_client_mp.h>
#include <server_mp/sv_init_mp.h>
#endif
#include <qcommon/cm_load.h>
#include <qcommon/dobj_management.h>
#include "sv_world.h"
#include <qcommon/cm_test.h>
#ifdef KISAK_SP
#include <game/g_main.h>
#else
#include <game_mp/g_main_mp.h>
#endif
#include <qcommon/dvar_cmds.h>
#include <universal/com_memory.h>
#include <xanim/dobj_utils.h>
#include <xanim/xmodel_utils.h>
#include <universal/com_math_anglevectors.h>
#include <cgame/cg_drawtools.h>
#include <server_mp/sv_ccmds_mp.h>
#include <universal/com_files.h>
#include <qcommon/com_bsp_load_obj.h>
#include <client/cl_debugdata.h>
#include <qcommon/threads.h>
#include <game/g_load_utils.h>
#include <win32/win_main.h>
#include <win32/win_shared.h>
#include <game/g_svcmds.h>

// Unit-cube edge pairs (12 edges x 2 verts) for XModelDebugBoxes wireframe.
static int s_xmodelDebugBoxVerts[24][3] =
{
  { 0, 0, 0 },
  { 1, 0, 0 },
  { 0, 0, 0 },
  { 0, 1, 0 },
  { 1, 1, 0 },
  { 1, 0, 0 },
  { 1, 1, 0 },
  { 0, 1, 0 },
  { 0, 0, 1 },
  { 1, 0, 1 },
  { 0, 0, 1 },
  { 0, 1, 1 },
  { 1, 1, 1 },
  { 1, 0, 1 },
  { 1, 1, 1 },
  { 0, 1, 1 },
  { 0, 0, 0 },
  { 0, 0, 1 },
  { 1, 0, 0 },
  { 1, 0, 1 },
  { 0, 1, 0 },
  { 0, 1, 1 },
  { 1, 1, 0 },
  { 1, 1, 1 }
};

char *g_sv_skel_memory_start;
char g_sv_skel_memory[262144];

playerState_s *__cdecl SV_GameClientNum(int num)
{
    return (playerState_s *)((char *)sv.gameClients + num * sv.gameClientSize);
}

svEntity_s *__cdecl SV_SvEntityForGentity(const gentity_s *gEnt)
{
    if ( !gEnt || gEnt->s.number >= 1024 )
    {
        if ( gEnt )
            Com_Error(ERR_DROP, "SV_SvEntityForGentity: bad gEnt %d", gEnt->s.number);
        else
            Com_Error(ERR_DROP, "SV_SvEntityForGentity: bad gEnt %d", 0);
    }

    return &sv.svEntities[gEnt->s.number];
}

gentity_s *__cdecl SV_GentityNum(int num)
{
    return (gentity_s *)((char *)sv.gentities + num * sv.gentitySize);
}

gentity_s *__cdecl SV_GEntityForSvEntity(svEntity_s *svEnt)
{
    return SV_GentityNum(svEnt - sv.svEntities);
}

void __cdecl SV_GameSendServerCommand(int clientNum, svscmd_type type, const char *text)
{
    if ( clientNum == -1 )
    {
        SV_SendServerCommand(0, type, "%s", text);
    }
    else
    {
        if ( (com_maxclients->current.integer < 1 || com_maxclients->current.integer > 32)
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\server\\sv_game.cpp",
                        152,
                        0,
                        "%s\n\t(com_maxclients->current.integer) = %i",
                        "(com_maxclients->current.integer >= 1 && com_maxclients->current.integer <= 32)",
                        com_maxclients->current.integer) )
        {
            __debugbreak();
        }
        if ( clientNum >= 0 && clientNum < com_maxclients->current.integer )
            SV_SendServerCommand(&svs.clients[clientNum], type, "%s", text);
    }
}

void __cdecl SV_GameDropClient(int clientNum, const char *reason)
{
    if ( (com_maxclients->current.integer < 1 || com_maxclients->current.integer > 32)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\server\\sv_game.cpp",
                    164,
                    0,
                    "%s\n\t(com_maxclients->current.integer) = %i",
                    "(com_maxclients->current.integer >= 1 && com_maxclients->current.integer <= 32)",
                    com_maxclients->current.integer) )
    {
        __debugbreak();
    }
    if ( clientNum >= 0 && clientNum < com_maxclients->current.integer )
        SV_DropClient(&svs.clients[clientNum], reason, 1, 1);
}

void __cdecl SV_SetMapCenter(float *mapCenter)
{
    char *configString;

    svs.mapCenter[0] = *mapCenter;
    svs.mapCenter[1] = mapCenter[1];
    svs.mapCenter[2] = mapCenter[2];
    // Match retail: truncate to whole units before broadcasting on CS_MAP_CENTER.
    svs.mapCenter[0] = (float)(int)svs.mapCenter[0];
    svs.mapCenter[1] = (float)(int)svs.mapCenter[1];
    svs.mapCenter[2] = (float)(int)svs.mapCenter[2];
    configString = va("%g %g %g", svs.mapCenter[0], svs.mapCenter[1], svs.mapCenter[2]);
    SV_SetConfigstring(13, configString);
}

float (*__cdecl SV_GetMapCenter())[3]
{
    return (float (*)[3])svs.mapCenter;
}

void __cdecl SV_SetGameEndTime(int gameEndTime)
{
    char *configString;
    char lastGameEndTime[12];

    SV_GetConfigstring(0xCu, lastGameEndTime, 12);
    // Avoid spamming CS_GAME_END_TIME when the value only drifts slightly.
    if ( (int)abs(atoi(lastGameEndTime) - gameEndTime) > 500 )
    {
        configString = va("%i", gameEndTime);
        SV_SetConfigstring(12, configString);
    }
}

void __cdecl SV_SetTimeScale(float endTimeScale, int endTime)
{
    char *configString;

    configString = va("%g %d", endTimeScale, endTime);
    SV_SetConfigstring(14, configString);
}

char __cdecl SV_SetBrushModel(gentity_s *ent)
{
    DObj *obj;
    float mins[3];
    float dobjMaxs[3];
    float maxs[3];
    float dobjMins[3];

    if ( !ent->r.inuse
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\server\\sv_game.cpp", 213, 0, "%s", "ent->r.inuse") )
    {
        __debugbreak();
    }
    if ( !CM_ClipHandleIsValid(ent->s.index.brushmodel) )
        return 0;
    CM_ModelBounds(ent->s.index.brushmodel, mins, maxs);
    ent->r.mins[0] = mins[0];
    ent->r.mins[1] = mins[1];
    ent->r.mins[2] = mins[2];
    ent->r.maxs[0] = maxs[0];
    ent->r.maxs[1] = maxs[1];
    ent->r.maxs[2] = maxs[2];
    ent->r.bmodel = 1;
    ent->r.contents = CM_ContentsOfModel(ent->s.index.brushmodel);
    obj = Com_GetServerDObj(ent->s.number);
    if ( obj )
    {
        if ( (ent->r.svFlags & 4) != 0 )
        {
            DObjCalcBounds(obj, dobjMins, dobjMaxs);
            Vec3Min(ent->r.mins, dobjMins, ent->r.mins);
            Vec3Max(ent->r.maxs, dobjMaxs, ent->r.maxs);
            ent->r.contents |= DObjGetContents(obj);
        }
    }
    SV_LinkEntity(ent);
    return 1;
}

bool __cdecl SV_inSnapshot(const float *origin, int iEntityNum)
{
    int clientCluster;
    float fogOpaqueDistSqrd;
    svEntity_s *svEnt;
    int cluster;
    unsigned int leafnum;
    gentity_s *ent;
    int clusterIndex;
    unsigned __int8 *pvs;

    ent = SV_GentityNum(iEntityNum);
    if ( !ent->r.linked )
        return 0;
    if ( ent->r.broadcastTime )
        return 1;
    if ( (ent->r.svFlags & 1) != 0 )
        return 0;
    if ( (ent->r.svFlags & 0x18) != 0 )
        return 1;
    svEnt = SV_SvEntityForGentity(ent);
    leafnum = CM_PointLeafnum(origin);
    if ( !svEnt->numClusters )
        return 0;
    clientCluster = CM_LeafCluster(leafnum);
    pvs = CM_ClusterPVS(clientCluster);
    cluster = 0;
    for ( clusterIndex = 0; clusterIndex < svEnt->numClusters; ++clusterIndex )
    {
        cluster = svEnt->clusternums[clusterIndex];
        if ( ((1 << (cluster & 7)) & pvs[cluster >> 3]) != 0 )
            break;
    }
    // If no explicit cluster hit, walk the span up to lastCluster (portal/flood fill).
    if ( clusterIndex == svEnt->numClusters )
    {
        if ( !svEnt->lastCluster )
            return 0;
        while ( cluster <= svEnt->lastCluster && ((1 << (cluster & 7)) & pvs[cluster >> 3]) == 0 )
            ++cluster;
        if ( cluster == svEnt->lastCluster )
            return 0;
    }
    fogOpaqueDistSqrd = G_GetFogOpaqueDistSqrd();
    if ( fogOpaqueDistSqrd == 3.4028235e38 )
        return 1;
    return !BoxDistSqrdExceeds(ent->r.absmin, ent->r.absmax, origin, fogOpaqueDistSqrd);
}

bool __cdecl SV_EntityContact(const float *mins, const float *maxs, const gentity_s *gEnt)
{
    unsigned int clipHandle;
    float cylinderRadius;
    float diskRadius;
    trace_t trace;
    float traceCenter[2];
    float distSq;
    float radiusSq;

    memset(&trace, 0, 16);
    if ( (gEnt->r.svFlags & 0x60) != 0 )
    {
        // SVF_CYLINDER / SVF_DISK: cheap 2D overlap tests for oversized touch volumes.
        if ( (gEnt->r.svFlags & 0x20) != 0 )
        {
            if ( gEnt->r.mins[2] != 0.0
                && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\server\\sv_game.cpp", 336, 0, "%s", "!gEnt->r.mins[2]") )
            {
                __debugbreak();
            }
            if ( gEnt->r.currentOrigin[2] >= maxs[2] )
                return 0;
            if ( mins[2] >= (float)(gEnt->r.currentOrigin[2] + gEnt->r.maxs[2]) )
                return 0;
            traceCenter[0] = 0.5f * (*mins + *maxs);
            traceCenter[1] = 0.5f * (mins[1] + maxs[1]);
            cylinderRadius = (float)(*maxs - traceCenter[0]) + gEnt->r.maxs[0];
            distSq = (float)((traceCenter[0] - gEnt->r.currentOrigin[0]) * (traceCenter[0] - gEnt->r.currentOrigin[0]))
                   + (float)((traceCenter[1] - gEnt->r.currentOrigin[1]) * (float)(traceCenter[1] - gEnt->r.currentOrigin[1]));
            return cylinderRadius * cylinderRadius > distSq;
        }
        if ( (gEnt->r.svFlags & 0x40) == 0
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\server\\sv_game.cpp",
                        349,
                        0,
                        "%s",
                        "gEnt->r.svFlags & SVF_DISK") )
        {
            __debugbreak();
        }
        traceCenter[0] = 0.5f * (*mins + *maxs);
        traceCenter[1] = 0.5f * (mins[1] + maxs[1]);
        diskRadius = (float)((float)(*maxs - traceCenter[0]) + gEnt->r.maxs[0]) - 64.0f;
        distSq = (float)((traceCenter[0] - gEnt->r.currentOrigin[0]) * (traceCenter[0] - gEnt->r.currentOrigin[0]))
               + (float)((traceCenter[1] - gEnt->r.currentOrigin[1]) * (traceCenter[1] - gEnt->r.currentOrigin[1]));
        radiusSq = diskRadius * diskRadius;
        return distSq >= radiusSq;
    }
    clipHandle = SV_ClipHandleForEntity(gEnt);
    CM_TransformedBoxTraceExternal(
        &trace,
        vec3_origin,
        vec3_origin,
        mins,
        maxs,
        clipHandle,
        -1,
        gEnt->r.currentOrigin,
        gEnt->r.currentAngles);
    return trace.startsolid;
}

void __cdecl SV_GetServerinfo(char *buffer, int bufferSize)
{
    const char *infoString;

    if ( bufferSize < 1 )
        Com_Error(ERR_DROP, "SV_GetServerinfo: bufferSize == %i", bufferSize);
    infoString = Dvar_InfoString(0, 4);
    I_strncpyz(buffer, infoString, bufferSize);
}

void __cdecl SV_LocateGameData(
                gentity_s *gEnts,
                int numGEntities,
                int sizeofGEntity_t,
                playerState_s *clients,
                int sizeofGameClient)
{
    sv.gentities = gEnts;
    sv.gentitySize = sizeofGEntity_t;
    sv.num_entities = numGEntities;
    sv.gameClients = clients;
    sv.gameClientSize = sizeofGameClient;
}

void __cdecl SV_GetUsercmd(int clientNum, usercmd_s *cmd)
{
    if ( clientNum < 0
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\server\\sv_game.cpp", 534, 0, "%s", "clientNum >= 0") )
    {
        __debugbreak();
    }
    if ( (com_maxclients->current.integer < 1 || com_maxclients->current.integer > 32)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\server\\sv_game.cpp",
                    535,
                    0,
                    "%s\n\t(com_maxclients->current.integer) = %i",
                    "(com_maxclients->current.integer >= 1 && com_maxclients->current.integer <= 32)",
                    com_maxclients->current.integer) )
    {
        __debugbreak();
    }
    if ( clientNum >= com_maxclients->current.integer
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\server\\sv_game.cpp",
                    536,
                    0,
                    "%s",
                    "clientNum < com_maxclients->current.integer") )
    {
        __debugbreak();
    }
    memcpy(cmd, &svs.clients[clientNum].lastUsercmd, sizeof(usercmd_s));
}

XModel *__cdecl SV_XModelGet(char *name)
{
    return XModelPrecache(
                     name,
                     (void *(__cdecl *)(int))SV_AllocXModelPrecache,
                     (void *(__cdecl *)(int))SV_AllocXModelPrecacheColl);
}

unsigned __int8 *__cdecl SV_AllocXModelPrecache(unsigned int size)
{
    return Hunk_Alloc(size, "SV_AllocXModelPrecache", 22);
}

unsigned __int8 *__cdecl SV_AllocXModelPrecacheColl(unsigned int size)
{
    return Hunk_Alloc(size, "SV_AllocXModelPrecacheColl", 28);
}

void __cdecl SV_DObjDumpInfo(gentity_s *ent)
{
    const DObj *obj;

    if ( com_developer->current.integer )
    {
        obj = Com_GetServerDObj(ent->s.number);
        if ( obj )
            DObjDumpInfo(obj);
        else
            Com_Printf(15, "no model.\n");
    }
}

void __cdecl SV_ResetSkeletonCache()
{
    // Never use timestamp 0; DObjSkelExists treats it as invalid.
    if ( !++sv.skelTimeStamp )
        sv.skelTimeStamp = 1;
    g_sv_skel_memory_start = (char *)((unsigned int)&g_sv_skel_memory[15] & 0xFFFFFFF0);
    sv.skelMemPos = 0;
}

bool __cdecl SV_DObjCreateSkelForBone(DObj *obj, int boneIndex)
{
    char *buf;
    unsigned int len;

    if (DObjSkelExists(obj, sv.skelTimeStamp))
        return DObjSkelIsBoneUpToDate(obj, boneIndex);
    len = DObjGetAllocSkelSize(obj);
    buf = SV_AllocSkelMemory(len);
    DObjCreateSkel(obj, buf, sv.skelTimeStamp);
    return 0;
}

char *__cdecl SV_AllocSkelMemory(unsigned int size)
{
    char *alloc;
    unsigned int alignedSize;
    static int s_skelMemWarnTimeStamp;

    if (!size && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\server\\sv_game.cpp", 628, 0, "%s", "size"))
        __debugbreak();
    alignedSize = (size + 15) & 0xFFFFFFF0;
    if (alignedSize > 0x3FFF0
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\server\\sv_game.cpp",
            631,
            0,
            "%s",
            "size <= sizeof( g_sv_skel_memory ) - SKEL_MEM_ALIGNMENT"))
    {
        __debugbreak();
    }
    if (!g_sv_skel_memory_start
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\server\\sv_game.cpp",
            633,
            0,
            "%s",
            "g_sv_skel_memory_start"))
    {
        __debugbreak();
    }
    while ( 1 )
    {
        alloc = &g_sv_skel_memory_start[sv.skelMemPos];
        sv.skelMemPos += alignedSize;
        if ( sv.skelMemPos <= 262128 )
            break;
        if ( s_skelMemWarnTimeStamp != sv.skelTimeStamp )
        {
            s_skelMemWarnTimeStamp = sv.skelTimeStamp;
            Com_PrintWarning(15, "WARNING: SV_SKEL_MEMORY_SIZE exceeded\n");
        }
        SV_ResetSkeletonCache();
    }
    if ( !alloc && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\server\\sv_game.cpp", 641, 0, "%s", "alloc") )
        __debugbreak();
    return alloc;
}

int __cdecl SV_DObjCreateSkelForBones(DObj *obj, int *partBits)
{
    char *buf;
    unsigned int len;

    if (DObjSkelExists(obj, sv.skelTimeStamp))
        return DObjSkelAreBonesUpToDate(obj, partBits);
    len = DObjGetAllocSkelSize(obj);
    buf = SV_AllocSkelMemory(len);
    DObjCreateSkel(obj, buf, sv.skelTimeStamp);
    return 0;
}

void __cdecl SV_DObjUpdateServerTime(gentity_s *ent, float dtime, int bNotify)
{
    DObj *obj;

    obj = Com_GetServerDObj(ent->s.number);
    if ( obj )
        DObjUpdateServerInfo(obj, dtime, bNotify);
}

void __cdecl SV_DObjInitServerTime(gentity_s *ent, float dtime)
{
    DObj *obj;

    obj = Com_GetServerDObj(ent->s.number);
    if ( obj )
        DObjInitServerTime(obj, dtime);
}

int __cdecl SV_DObjGetBoneIndex(const gentity_s *ent, unsigned int boneName)
{
    const DObj *obj;
    unsigned __int8 index;

    obj = Com_GetServerDObj(ent->s.number);
    if ( !obj )
        return -1;
    index = -2;
    if ( DObjGetBoneIndex(obj, boneName, &index, -1) )
        return index;
    else
        return -1;
}

DObjAnimMat *__cdecl SV_DObjGetMatrixArray(const gentity_s *ent)
{
    const DObj *obj;

    obj = Com_GetServerDObj(ent->s.number);
    if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\server\\sv_game.cpp", 783, 0, "%s", "obj") )
        __debugbreak();
    return DObjGetRotTransArray(obj);
}

void __cdecl SV_DObjDisplayAnim(gentity_s *ent, const char *header)
{
    const DObj *obj;

    obj = Com_GetServerDObj(ent->s.number);
    if ( obj )
        DObjDisplayAnim(obj, header);
}

DObjAnimMat *__cdecl SV_DObjGetRotTransArray(const gentity_s *ent)
{
    const DObj *obj;

    obj = Com_GetServerDObj(ent->s.number);
    if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\server\\sv_game.cpp", 806, 0, "%s", "obj") )
        __debugbreak();
    return DObjGetRotTransArray(obj);
}

int __cdecl SV_DObjSetRotTransIndex(const gentity_s *ent, int *partBits, int boneIndex)
{
    const DObj *obj;

    obj = Com_GetServerDObj(ent->s.number);
    if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\server\\sv_game.cpp", 817, 0, "%s", "obj") )
        __debugbreak();
    return DObjSetRotTransIndex(obj, partBits, boneIndex);
}

void __cdecl SV_DObjGetBounds(gentity_s *ent, float *mins, float *maxs)
{
    const DObj *obj;

    obj = Com_GetServerDObj(ent->s.number);
    if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\server\\sv_game.cpp", 839, 0, "%s", "obj") )
        __debugbreak();
    DObjGetBounds(obj, mins, maxs);
}

XAnimTree_s *__cdecl SV_DObjGetTree(gentity_s *ent)
{
    const DObj *obj;

    obj = Com_GetServerDObj(ent->s.number);
    if ( obj )
        return DObjGetTree(obj);
    else
        return 0;
}

void __cdecl SV_XModelDebugBoxes(gentity_s *ent, const float *color, int *partBits, int duration)
{
    DObjAnimMat *boneMatrix;
    unsigned int edgeIndex;
    XBoneInfo *boneInfoArray[160];
    int numBones;
    int boneIndex;
    DObj *obj;
    float start[3];
    float end[3];
    int bonesInModel;
    float boneAxis[4][3];
    int localBoneIndex;
    int (*boxVertPtr)[3];
    float org[3];
    XBoneInfo *boneInfo;
    float axis[3][3];
    float vec[3];
    int modelCount;
    int modelIndex;

    obj = Com_GetServerDObj(ent->s.number);
    if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\server\\sv_game.cpp", 887, 0, "%s", "obj") )
        __debugbreak();
    numBones = DObjNumBones(obj);
    if ( numBones > 160
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\server\\sv_game.cpp",
                    890,
                    0,
                    "%s",
                    "numBones <= DOBJ_MAX_PARTS") )
    {
        __debugbreak();
    }
    DObjGetBoneInfo(obj, boneInfoArray);
    boneMatrix = DObjGetRotTransArray(obj);
    AnglesToAxis(ent->r.currentAngles, axis);
    modelCount = DObjGetNumModels(obj);
    boneIndex = 0;
    for ( modelIndex = 0; modelIndex < modelCount; ++modelIndex )
    {
        bonesInModel = XModelNumBones(DObjGetModel(obj, modelIndex));
        if ( DObjIgnoreCollision(obj, modelIndex) )
        {
            boneIndex += bonesInModel;
            boneMatrix += bonesInModel;
        }
        else
        {
            localBoneIndex = 0;
            while ( localBoneIndex < bonesInModel )
            {
                if ( !partBits || (partBits[boneIndex >> 5] & (0x80000000 >> (boneIndex & 0x1F))) != 0 )
                {
                    boneInfo = boneInfoArray[boneIndex];
                    boxVertPtr = s_xmodelDebugBoxVerts;
                    ConvertQuatToMat(boneMatrix, boneAxis);
                    boneAxis[3][0] = boneMatrix->trans[0];
                    boneAxis[3][1] = boneMatrix->trans[1];
                    boneAxis[3][2] = boneMatrix->trans[2];
                    for ( edgeIndex = 0; edgeIndex < 12; ++edgeIndex )
                    {
                        org[0] = boneInfo->bounds[(*boxVertPtr)[0]][0];
                        org[1] = boneInfo->bounds[(*boxVertPtr)[1]][1];
                        org[2] = boneInfo->bounds[(*boxVertPtr)[2]][2];
                        MatrixTransformVector43(org, boneAxis, vec);
                        MatrixTransformVector(vec, axis, start);
                        Vec3Add(start, ent->r.currentOrigin, start);
                        org[0] = boneInfo->bounds[(*++boxVertPtr)[0]][0];
                        org[1] = boneInfo->bounds[(*boxVertPtr)[1]][1];
                        org[2] = boneInfo->bounds[(*boxVertPtr)[2]][2];
                        MatrixTransformVector43(org, boneAxis, vec);
                        MatrixTransformVector(vec, axis, end);
                        Vec3Add(end, ent->r.currentOrigin, end);
                        ++boxVertPtr;
                        CL_AddDebugLine(start, end, color, 0, duration);
                    }
                }
                ++localBoneIndex;
                ++boneMatrix;
                ++boneIndex;
            }
        }
    }
}

bool __cdecl SV_MapExists(char *name)
{
    char fullpath[256];
    const char *basename;

    basename = SV_GetMapBaseName(name);
    Com_GetBspFilename(fullpath, 0x100u, basename);
    return FS_ReadFile(fullpath, 0) >= 0;
}

bool __cdecl SV_DObjExists(gentity_s *ent)
{
    return Com_GetServerDObj(ent->s.number) != 0;
}

void __cdecl SV_track_shutdown()
{
    //track_shutdown(2);
}

void __cdecl SV_SavePaths(unsigned __int8 *buf, unsigned int size)
{
    Com_SaveLump(LUMP_PATHCONNECTIONS, buf, size, COM_SAVE_LUMP_AND_REOPEN);
    sv.checksum = Com_GetBspChecksum();
}

int __cdecl SV_GetGuid(int clientNum)
{
    if ( !com_maxclients
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\server\\sv_game.cpp", 1001, 0, "%s", "com_maxclients") )
    {
        __debugbreak();
    }
    if ( clientNum >= 0 && clientNum < com_maxclients->current.integer )
        return svs.clients[clientNum].guid;
    else
        return 0;
}

int __cdecl SV_GetClientPing(unsigned int clientNum)
{
    if ( clientNum >= com_maxclients->current.integer
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\server\\sv_game.cpp",
                    1010,
                    0,
                    "clientNum doesn't index com_maxclients->current.integer\n\t%i not in [0, %i)",
                    clientNum,
                    com_maxclients->current.integer) )
    {
        __debugbreak();
    }
    return svs.clients[clientNum].ping;
}

bool __cdecl SV_IsLocalClient(unsigned int clientNum)
{
    if ( clientNum >= com_maxclients->current.integer
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\server\\sv_game.cpp",
                    1018,
                    0,
                    "clientNum doesn't index com_maxclients->current.integer\n\t%i not in [0, %i)",
                    clientNum,
                    com_maxclients->current.integer) )
    {
        __debugbreak();
    }
    return NET_IsLocalAddress(svs.clients[clientNum].header.netchan.remoteAddress);
}

void __cdecl SV_ShutdownGameProgs()
{
    Com_SyncThreads();
    sv.state = SS_DEAD;
    if ( gameInitialized )
    {
        SV_ShutdownGameVM(1);
        gameInitialized = 0;
    }
}

void __cdecl SV_SetGametype()
{
    char gametype[64];
    char *ch;

#ifdef KISAK_SP
    // Decomp: BlackOps.singleplayer.c @ 600235 — g_gametype default "cmp" (off_A30458).
    if ( !sv_gametype )
        sv_gametype = _Dvar_RegisterString("g_gametype", "cmp", 0x24u, "Current game type");
#else
    _Dvar_RegisterString("g_gametype", "tdm", 0x24u, "Game Type");
#endif
    if ( com_sv_running->current.enabled && G_GetSavePersist() )
        I_strncpyz(gametype, sv.gametype, 64);
#ifdef KISAK_SP
    else if ( sv_mapname && sv_mapname->current.string[0] )
    {
        if ( I_stristr(sv_mapname->current.string, "zombie") )
            I_strncpyz(gametype, "zombie", 64);
        else if ( Com_IsMenuLevel(sv_mapname->current.string) )
            I_strncpyz(gametype, "solo", 64);
        else
            I_strncpyz(gametype, "cmp", 64);
    }
#endif
    else
        I_strncpyz(gametype, sv_gametype->current.string, 64);
    for ( ch = gametype; *ch; ++ch )
        *ch = tolower(*ch);
    if ( !Scr_IsValidGameType(gametype) )
    {
#ifdef KISAK_SP
        Com_Printf(15, "g_gametype %s is not a valid gametype, defaulting to cmp\n", gametype);
        strcpy(gametype, "cmp");
#else
        Com_Printf(15, "g_gametype %s is not a valid gametype, defaulting to dm\n", gametype);
        strcpy(gametype, "dm");
#endif
    }
    Dvar_SetString((dvar_s *)sv_gametype, gametype);
}

void __cdecl SV_ShutdownGameVM(int clearScripts)
{
    if ( !Sys_IsMainThread()
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\server\\sv_game.cpp", 1295, 0, "%s", "Sys_IsMainThread()") )
    {
        __debugbreak();
    }
    PROF_SCOPED("SV_ShutdownGameVM");
    G_ShutdownGame(clearScripts);
}

void __cdecl SV_RestartGameProgs(int savepersist)
{
    Com_SyncThreads();
    SV_ShutdownGameVM(0);
    com_fixedConsolePosition = 0;
    SV_InitGameVM(1, savepersist == 0);
}

void __cdecl SV_InitGameVM(int restart, int registerDvars)
{
    int clientIndex;

    PROF_SCOPED("SV_InitGameVM");

    G_ResetEntityParsePoint();
    SV_ResetSkeletonCache();

    if ( (com_maxclients->current.integer < 1 || com_maxclients->current.integer > 32)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\server\\sv_game.cpp",
                    1217,
                    0,
                    "%s\n\t(com_maxclients->current.integer) = %i",
                    "(com_maxclients->current.integer >= 1 && com_maxclients->current.integer <= 32)",
                    com_maxclients->current.integer) )
    {
        __debugbreak();
    }

    for ( clientIndex = 0; clientIndex < com_maxclients->current.integer; ++clientIndex )
        svs.clients[clientIndex].gentity = 0;

    Sys_LoadingKeepAlive();
    G_InitGame(svs.time, Sys_MillisecondsRaw(), restart, registerDvars);

    // Dedicated servers: copy gametype script sprint limit into the live player dvar.
    if ( IsDedicatedServer() )
    {
        float sprintTime = Dvar_GetFloat("scr_player_sprintTime");
        Dvar_SetFromStringByNameFromSource("player_sprintTime", va("%f", sprintTime), DVAR_SOURCE_SCRIPT, 0);
    }

    SV_SetConfigstring(3, va("%i", sv_serverId_value));
    Sys_LoadingKeepAlive();

    if (IsDedicatedServer())
    {
        Com_DvarDump(6, NULL);
    }
}

void __cdecl SV_InitGameProgs(int savepersist)
{
    gameInitialized = 1;
    SV_InitGameVM(0, savepersist == 0);
}

int __cdecl SV_GameCommand()
{
    if ( sv.state == SS_GAME )
        return ConsoleCommand();
    else
        return 0;
}

const char *__cdecl SV_Archived_Dvar_GetVariantString(const char *dvarName)
{
    return Dvar_GetVariantString(dvarName);
}

const char *__cdecl SV_Archived_Dvar_GetVariantString(int dvarHash)
{
    return Dvar_GetVariantString(dvarHash);
}

