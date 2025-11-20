#pragma once

bool __cdecl XModelBad(const XModel *model);
void __cdecl XModelReleaseResources(XModel *model);
void __cdecl XModelPartsFree(XModelPartsLoad *modelParts);
XModelPartsLoad *__cdecl XModelPartsFindData(const char *name);
void __cdecl XModelPartsSetData(const char *name, XModelPartsLoad *modelParts, void *(__cdecl *Alloc)(int));
XModelSurfs *__cdecl XModelSurfsFindData(const char *name);
void __cdecl XModelSurfsSetData(const char *name, XModelSurfs *modelSurfs, void *(__cdecl *Alloc)(int));
XModel *__cdecl XModelPrecache(char *name, void *(__cdecl *Alloc)(int), void *(__cdecl *AllocColl)(int));
XModel *__cdecl XModelPrecache_LoadObj(char *name, void *(__cdecl *Alloc)(int), void *(__cdecl *AllocColl)(int));
XModel *__cdecl XModelDefaultModel(const char *name, void *(__cdecl *Alloc)(int));
XModel *__cdecl XModelCreateDefault(void *(__cdecl *Alloc)(int));
void __cdecl XModelMakeDefault(XModel *model);
XModelPartsLoad *__cdecl XModelCreateDefaultParts();
XModel *__cdecl XModelPrecache_FastFile(const char *name);
unsigned __int16 *__cdecl XModelBoneNames(XModel *model);
double __cdecl XModelGetRadius(const XModel *model);
void __cdecl XModelGetBounds(const XModel *model, float *mins, float *maxs);
int __cdecl XModelGetMemUsage(const XModel *model);
void __cdecl XModelRenderString(const float *pos, char *string);
int __cdecl XModelTraceLine(
        const XModel *model,
        trace_t *results,
        const float *localStart,
        const float *localEnd,
        int contentmask);
int __cdecl XModelTraceLineAnimated(
        const DObj *obj,
        unsigned int modelIndex,
        int baseBoneIndex,
        trace_t *results,
        const DObjAnimMat *boneMtxList,
        float *localStart,
        float *localEnd,
        int contentmask);
void __cdecl ConvertQuatToInverseMat(const DObjAnimMat *mat, float (*axis)[3]);
void __cdecl XModelTraceLineAnimatedPartBits(
        const DObj *obj,
        unsigned int modelIndex,
        int baseBoneIndex,
        int contentmask,
        int *partBits);
char __cdecl XSurfaceVisitTrianglesInAabb(
        const XSurface *surface,
        unsigned int vertListIndex,
        const float *aabbMins,
        const float *aabbMaxs,
        bool (__cdecl *visitorFunc)(void *, const unsigned __int8 **, const unsigned __int8 **),
        void *visitorContext);
void __cdecl XSurfaceVisitTrianglesInAabb_ConvertAabb(
        const XSurfaceCollisionTree *tree,
        const float *aabbMins,
        const float *aabbMaxs,
        int *mins,
        int *maxs);
void __cdecl PrefetchArrayIndexed(char *mem, unsigned int elementCount, unsigned int index, unsigned int elementSize);
int __cdecl XSurfaceVisitTrianglesInAabb_ProcessVertices(XSurfaceGetTriCandidatesLocals *locals);
char __cdecl XSurfaceVisitTrianglesInAabb_ProcessTriangles(XSurfaceGetTriCandidatesLocals *locals);
char __cdecl XSurfaceVisitTrianglesInAabb_ProcessLeaf(XSurfaceGetTriCandidatesLocals *locals);
char __cdecl XSurfaceVisitTrianglesInAabb_ProcessNode(XSurfaceGetTriCandidatesLocals *locals);
