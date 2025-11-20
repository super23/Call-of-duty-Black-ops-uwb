#pragma once

void  R_BoxSurfaces(
        int a1@<ebp>,
        const float *mins,
        const float *maxs,
        int (__cdecl **allowSurf)(int, void *),
        void *callbackContext,
        GfxSurface ***surfLists,
        unsigned int surfListSize,
        unsigned int *surfCounts,
        unsigned int listCount);
void __cdecl R_BoxSurfaces_r(
        mnode_t *node,
        const float *mins,
        const float *maxs,
        int (__cdecl **allowSurf)(int, void *),
        void *callbackContext,
        GfxSurface ***surfLists,
        unsigned int surfListSize,
        unsigned int *surfCounts,
        unsigned int listCount,
        unsigned __int8 *cellBits);
void __cdecl R_CellSurfaces(
        int cellIndex,
        const float *mins,
        const float *maxs,
        int (__cdecl *allowSurf)(int, void *),
        void *callbackContext,
        GfxSurface **surfList,
        unsigned int surfListSize,
        unsigned int *surfCount,
        unsigned __int8 *cellBits);
void __cdecl R_AABBTreeSurfaces_r(
        GfxAabbTree *tree,
        const float *mins,
        const float *maxs,
        int (__cdecl *allowSurf)(int, void *),
        void *callbackContext,
        GfxSurface **surfList,
        unsigned int surfListSize,
        unsigned int *surfCount);
void __cdecl R_AddSurfaceToList(GfxSurface *surf, GfxSurface **surfList, unsigned int *surfCount);
void __cdecl R_CellSurfacesTwoLists(
        int cellIndex,
        const float *mins,
        const float *maxs,
        int (__cdecl **allowSurf)(int, void *),
        void *callbackContext,
        GfxSurface ***surfLists,
        unsigned int surfListSize,
        unsigned int *surfCounts,
        unsigned __int8 *cellBits);
void __cdecl R_AABBTreeSurfacesTwoLists_r(
        GfxAabbTree *tree,
        const float *mins,
        const float *maxs,
        int (__cdecl **allowSurf)(int, void *),
        void *callbackContext,
        GfxSurface ***surfLists,
        unsigned int surfListSize,
        unsigned int *surfCounts);
int  R_BoxStaticModels@<eax>(
        int a1@<ebp>,
        const float *mins,
        const float *maxs,
        int (__cdecl *allowSModel)(int),
        unsigned __int16 *smodelList,
        int smodelListSize);
void __cdecl R_BoxStaticModels_r(
        mnode_t *node,
        const float *mins,
        const float *maxs,
        int (__cdecl *allowSModel)(int),
        unsigned __int16 *smodelList,
        int smodelListSize,
        int *smodelCount,
        unsigned __int8 *cellBits);
void __cdecl R_CellStaticModels(
        int cellIndex,
        const float *mins,
        const float *maxs,
        int (__cdecl *allowSModel)(int),
        unsigned __int16 *smodelList,
        int smodelListSize,
        int *smodelCount,
        unsigned __int8 *cellBits);
void __cdecl R_AABBTreeStaticModels_r(
        GfxAabbTree *tree,
        const float *mins,
        const float *maxs,
        int (__cdecl *allowSModel)(int),
        unsigned __int16 *smodelList,
        int smodelListSize,
        int *smodelCount);
void __cdecl R_AddStaticModelToList(int smodelIndex, unsigned __int16 *smodelList, int *smodelCount);
void __cdecl R_MarkUtil_GetDObjAnimMatAndHideParts(
        DObj *dobj,
        const cpose_t *pose,
        const DObjAnimMat **outBoneMtxList,
        unsigned int *outHidePartBits);
void __cdecl R_MarkFragments_Begin(
        MarkInfo *markInfo,
        MarkFragmentsAgainstEnum markAgainst,
        const float *origin,
        const float (*axis)[3],
        float radius,
        const float *viewOffset,
        Material *material);
void __cdecl R_GetMarkFragmentBounds(
        const float *origin,
        const float (*axis)[3],
        float radius,
        float *mins,
        float *maxs);
void __cdecl R_GetMarkFragmentClipPlanes(
        const float *origin,
        const float (*axis)[3],
        float radius,
        MarkClipPlaneSet *clipPlanes);
char __cdecl R_MarkFragments_AddDObj(MarkInfo *markInfo, DObj *dObj, cpose_t *pose, unsigned __int16 entityIndex);
char __cdecl R_MarkFragments_AddBModel(
        MarkInfo *markInfo,
        GfxBrushModel *brushModel,
        cpose_t *pose,
        unsigned __int16 entityIndex);
void __cdecl R_MarkFragments_Go(
        MarkInfo *markInfo,
        void (__cdecl *callback)(void *, int, FxMarkTri *, int, FxMarkPoint *, const float *, const float *, const float *, const bool),
        void *callbackContext,
        int maxTris,
        FxMarkTri *tris,
        int maxPoints,
        FxMarkPoint *points);
bool __cdecl R_MarkFragments_Brushes(MarkInfo *markInfo);
char __cdecl R_MarkFragments_WorldBrushes(MarkInfo *markInfo);
bool __cdecl R_AllowMarks(int surfIndex, const Material *markMaterialAsVoid);
bool __cdecl R_Mark_MaterialAllowsMarks(const Material *markReceiverMaterialHandle, const Material *markMaterialHandle);
char __cdecl R_MarkFragments_BrushSurface(
        MarkInfo *markInfo,
        GfxMarkContext *markContext,
        const MarkClipPlaneSet *clipPlanes,
        const float *markDir,
        const GfxSurface *surface,
        bool *anyMarks);
int __cdecl R_ChopWorldPolyBehindPlane(
        int inPointCount,
        FxWorldMarkPoint *inPoints,
        FxWorldMarkPoint *outPoints,
        const float *plane,
        float epsilon);
void __cdecl R_LerpModelMarkPoints(
        const FxModelMarkPoint *from,
        const FxModelMarkPoint *to,
        float lerp,
        FxModelMarkPoint *out);
bool __cdecl R_MarkFragment_IsTriangleRejected(
        const float *markNormal,
        const float *xyz0,
        const float *xyz1,
        const float *xyz2);
void __cdecl R_MarkFragment_SetupWorldClipPoints(
        const GfxWorldVertex *triVerts0,
        const unsigned __int16 *indices,
        FxWorldMarkPoint (*clipPoints)[14]);
char __cdecl R_MarkFragments_EntBrushes(MarkInfo *markInfo);
void __cdecl R_Mark_TransformClipPlanes(
        const MarkClipPlaneSet *inClipPlanes,
        float (*matrix)[3],
        MarkClipPlaneSet *outClipPlanes);
char __cdecl R_MarkFragments_Glass(MarkInfo *markInfo);
char __cdecl R_MarkFragments_Models(MarkInfo *markInfo);
char __cdecl R_MarkFragments_SceneDObjs(MarkInfo *markInfo);
char __cdecl R_MarkFragments_AnimatedXModel(
        MarkInfo *markInfo,
        const XModel *model,
        const unsigned int *hidePartBits,
        int boneIndex,
        const DObjAnimMat *boneMtxList,
        int boneCount,
        GfxMarkContext *markContext);
// local variable allocation has failed, the output may be wrong!
char  R_MarkFragments_AnimatedXModel_VertList@<al>(
        int a1@<ebp>,
        MarkInfo *markInfo,
        unsigned int vertListIndex,
        const DObjAnimMat *poseBone,
        const DObjAnimMat *baseBone,
        GfxMarkContext *markContext,
        XSurface *surface);
void __cdecl Vec3AddScalar(const float *a, float s, float *sum);
char __cdecl R_MarkFragments_StaticModels(MarkInfo *markInfo);
char __cdecl R_MarkFragments_EntirelyRigidXModel(
        MarkInfo *markInfo,
        const XModel *xmodel,
        const float (*modelAxis)[3],
        const float *modelOrigin,
        float modelScale,
        GfxMarkContext *markContext);
char __cdecl R_MarkFragments_XModelSurface_Basic(
        MarkInfo *markInfo,
        const XSurface *surface,
        const float (*modelAxis)[3],
        const float *modelOrigin,
        float modelScale,
        GfxMarkContext *markContext);
int __cdecl R_ChopPolyBehindPlane(
        int inPointCount,
        FxModelMarkPoint *inPoints,
        FxModelMarkPoint *outPoints,
        const float *plane);
