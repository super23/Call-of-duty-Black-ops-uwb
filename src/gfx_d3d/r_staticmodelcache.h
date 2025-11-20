#pragma once

void __cdecl R_SkinCachedStaticModelCmd(SkinCachedStaticModelCmd *skinCmd);
static_model_leaf_t *__cdecl SMC_GetLeaf(unsigned int cacheIndex);
void __cdecl SetupTransformUnitVec(const float4 *mtx, int (*fixedMtx)[3]);
void __cdecl R_SkinXSurfaceStaticVerts(
        const float4 *useAxis,
        const int (*normAxis)[3],
        unsigned int baseVertIndex,
        unsigned int vertCount,
        const GfxPackedVertex *srcVertArray,
        unsigned int smodelIndex,
        GfxSModelCachedVertex *verts);
PackedUnitVec __cdecl LocalTransformUnitVec(PackedUnitVec in, const int (*fixedMtx)[3]);
static_model_leaf_t *__cdecl R_GetCachedSModelSurf(unsigned int cacheIndex);
unsigned __int16 __cdecl R_CacheStaticModelSurface(
        unsigned int smcIndex,
        unsigned int smodelIndex,
        const XModelLodInfo *lodInfo);
unsigned __int16 __cdecl SMC_Allocate(unsigned int smcIndex, unsigned int bitCount);
char __cdecl SMC_GetFreeBlockOfSize(unsigned int smcIndex, unsigned int listIndex);
char __cdecl SMC_ForceFreeBlock(unsigned int smcIndex);
void __cdecl SMC_FreeCachedSurface_r(
        static_model_tree_t *tree,
        static_model_leaf_t *leafs,
        int nodeIndex,
        int levelsToLeaf);
void __cdecl R_CacheStaticModelIndices(unsigned int smodelIndex, unsigned int lod, unsigned int cacheBaseVertIndex);
void __cdecl R_InitStaticModelCache();
void *R_AllocStaticModelCache();
unsigned int SMC_ClearCache();
void __cdecl R_FlushStaticModelCache();
void __cdecl R_ClearAllStaticModelCacheRefs();
void __cdecl R_UncacheStaticModel(unsigned int smodelIndex);
void __cdecl R_ShutdownStaticModelCache();
void __cdecl R_StaticModelCacheStats_f();
void __cdecl R_StaticModelCacheFlush_f();
