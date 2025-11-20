#pragma once

int __cdecl R_CellForPoint(const float *origin);
unsigned int __cdecl R_CalcReflectionProbeIndex(const float *origin);
unsigned int __cdecl R_FindNearestReflectionProbeInCell(
        const GfxWorldDraw *worldDraw,
        const GfxCell *cell,
        const float *origin);
unsigned int __cdecl R_FindNearestReflectionProbe(const GfxWorldDraw *worldDraw, const float *origin);
void __cdecl R_AllocStaticModels(GfxAabbTree *tree);
void __cdecl R_SortGfxAabbTree(GfxWorld *world, GfxAabbTree *tree);
int __cdecl R_SortGfxAabbTreeChildren(
        GfxWorld *world,
        float *mins,
        float *maxs,
        unsigned __int16 *staticModels,
        int staticModelCount);
int __cdecl CompareStaticModels(unsigned __int16 *smodel0, unsigned __int16 *smodel1);
void __cdecl R_FilterStaticModelIntoCells_r(
        GfxWorld *world,
        mnode_t *node,
        GfxStaticModelInst *smodelInst,
        const float *mins,
        const float *maxs);
void __cdecl R_AddStaticModelToCell(GfxWorld *world, GfxStaticModelInst *smodelInst, int cellIndex);
void __cdecl R_AddStaticModelToAabbTree_r(GfxWorld *world, GfxAabbTree *tree, int smodelIndex);
const char *__cdecl GetNameFromStaticModelInst(const GfxStaticModelInst *smodelInst);
void __cdecl R_SetStaticModelReflectionProbe(
        const GfxWorld *world,
        const GfxStaticModelInst *smodelInst,
        GfxStaticModelDrawInst *smodelDrawInst);
unsigned int __cdecl R_CalcReflectionProbeIndex(const GfxWorld *world, const float *origin);
int __cdecl R_CellForPoint(const GfxWorld *world, const float *origin);
void __cdecl R_CreateStaticModel(
        XModel *model,
        const float *origin,
        const float (*axis)[3],
        float scale,
        GfxStaticModelDrawInst *smodelDrawInst,
        GfxStaticModelInst *smodelInst,
        unsigned int staticModelFlags);
