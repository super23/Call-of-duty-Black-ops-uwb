#pragma once

void __cdecl CMod_LoadPlanes();
MapEnts *__cdecl MapEnts_VirtualLoad(char *name);
bool __cdecl MapEnts_CanPurgeEntity(
        const char *classname,
        void *userData,
        bool (__cdecl *HasKeyCallback)(void *, const char *));
MapEnts *__cdecl MapEnts_RealLoad(char *name);
MapEnts *__cdecl MapEnts_GetFromString(char *name, const char *entityString, int numEntityChars);
bool __cdecl CMod_HasSpawnString(const SpawnVar *userData, const char *key);
void __cdecl CM_LoadMapFromBsp(const char *name, bool usePvs);
void CMod_LoadMaterials();
unsigned int CMod_LoadNodes();
void CMod_LoadLeafSurfaces();
void CMod_LoadCollisionVerts();
void CMod_LoadCollisionTriangles();
void CMod_LoadPartitionIndices();
void CMod_LoadCollisionEdgeWalkable();
unsigned int CMod_LoadCollisionBorders();
unsigned int CMod_LoadCollisionPartitions();
MapEnts *CMod_LoadEntityString();
void CMod_LoadVisibility();
void __cdecl CMod_LoadBrushRelated(unsigned int version, bool usePvs);
unsigned int CMod_LoadSubmodels();
unsigned int CMod_LoadSubmodelBrushNodes();
void __cdecl CMod_PartionLeafBrushes(unsigned __int16 *leafBrushes, int numLeafBrushes, cLeaf_s *leaf);
cLeafBrushNode_s *__cdecl CMod_PartionLeafBrushes_r(
        unsigned __int16 *leafBrushes,
        int numLeafBrushes,
        const float *mins,
        const float *maxs);
cLeafBrushNode_s *__cdecl CMod_AllocLeafBrushNode();
double __cdecl CMod_GetPartitionScore(
        unsigned __int16 *leafBrushes,
        int numLeafBrushes,
        int axis,
        const float *mins,
        const float *maxs,
        float *dist);
int __cdecl CMod_GetLeafTerrainContents(cLeaf_s *leaf);
const unsigned __int8 *CMod_LoadBrushes();
void __cdecl CMod_LoadLeafs(bool usePvs);
void __cdecl CMod_LoadLeafs_Version14(bool usePvs);
cLeaf_s *CMod_LoadLeafBrushNodes();
unsigned int CMod_LoadLeafBrushNodes_Version14();
unsigned int CMod_LoadLeafBrushes();
unsigned int CMod_LoadCollisionAabbTrees();
unsigned __int16 *CM_InitBoxHull();
cplane_s *__cdecl CM_GetPlanes();
int __cdecl CM_GetPlaneCount();
char *__cdecl CM_EntityString();
