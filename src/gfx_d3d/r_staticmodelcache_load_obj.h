#pragma once

void __cdecl R_AssignSModelCacheResources(GfxWorld *world);
unsigned __int8 __cdecl R_AssignSModelCacheIndex(
        char smcAllocBits,
        unsigned int maxEntryCount,
        unsigned int *smcUseCount);
bool __cdecl R_CompareSModelStats_Score(GfxSModelSurfStats *s0, GfxSModelSurfStats *s1);
unsigned int __cdecl R_GetEntryCount(GfxSModelSurfStats *stats);
unsigned int __cdecl R_OptimalSModelResourceStats(GfxWorld *world, GfxSModelSurfStats *stats, unsigned int statLimit);
unsigned int __cdecl R_AddSModelListStats(
        const GfxStaticModelDrawInst **drawInstArray,
        unsigned int drawInstCount,
        GfxSModelSurfStats *stats,
        unsigned int statsCount,
        unsigned int statsLimit);
int __cdecl R_GetSModelCacheAllocBits(const XModel *model, unsigned int lod);
unsigned int __cdecl R_MaxModelsInDistRange(
        const GfxStaticModelDrawInst **drawInstArray,
        unsigned int drawInstCount,
        const float *mins,
        const float *maxs,
        float distMin,
        float distMax);
bool __cdecl R_CompareSModels_Model(const GfxStaticModelDrawInst *s0, const GfxStaticModelDrawInst *s1);
