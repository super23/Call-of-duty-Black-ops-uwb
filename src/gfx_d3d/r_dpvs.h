#pragma once

void __cdecl R_FrustumClipPlanes(
        const GfxMatrix *viewProjMtx,
        const float (*sidePlanes)[4],
        int sidePlaneCount,
        DpvsPlane *frustumPlanes);
void __cdecl R_SetDpvsPlaneSides(DpvsPlane *plane);
int __cdecl R_CellForPoint(const float *origin);
unsigned int __cdecl R_CalcReflectionProbeIndex(const float *origin);
unsigned int __cdecl R_FindNearestReflectionProbeInCell(
        const GfxWorldDraw *worldDraw,
        const GfxCell *cell,
        const float *origin);
unsigned int __cdecl R_FindNearestReflectionProbe(const GfxWorldDraw *worldDraw, const float *origin);
unsigned int __cdecl R_FindProbeFromVolume(const GfxWorldDraw *worldDraw, const float *origin);
void __cdecl R_AddAllSceneEntSurfacesCamera(const GfxViewInfo *viewInfo);
void __cdecl LaunchDobjLightingJobs(
        const GfxViewInfo *viewInfo,
        unsigned int sceneEntCount,
        unsigned __int8 *sceneEntVisData,
        GfxLightingInfo *lightingInfo);
void __cdecl FinishDobjLighting(
        unsigned int sceneEntCount,
        unsigned __int8 *sceneEntVisData,
        GfxLightingInfo *lightingInfo,
        GfxDrawSurf **drawSurfs,
        GfxDrawSurf **lastDrawSurfs,
        const GfxLight *visibleLights,
        int visibleLightCount,
        bool isMissileCamera);
void __cdecl LaunchModelLightingJobs(
        const GfxViewInfo *viewInfo,
        unsigned int sceneEntCount,
        unsigned __int8 *sceneEntVisData,
        GfxLightingInfo *lightingInfo);
void __cdecl FinishModelLighting(
        unsigned int sceneEntCount,
        unsigned __int8 *sceneEntVisData,
        GfxLightingInfo *lightingInfo,
        GfxDrawSurf **drawSurfs,
        GfxDrawSurf **lastDrawSurfs,
        const GfxLight *visibleLights,
        int visibleLightCount);
void __cdecl LaunchDynEntLightingJobs(
        const GfxViewInfo *viewInfo,
        unsigned int sceneEntCount,
        unsigned __int8 *sceneEntVisData,
        GfxLightingInfo *lightingInfo);
void __cdecl FinishDynEntLighting(
        unsigned int sceneEntCount,
        unsigned __int8 *sceneEntVisData,
        GfxLightingInfo *lightingInfo,
        GfxDrawSurf **drawSurfs,
        GfxDrawSurf **lastDrawSurfs,
        const GfxLight *visibleLights,
        int visibleLightCount);
void __cdecl R_AddAllSceneEntSurfacesSunShadow();
void __cdecl R_AddAllSceneEntSurfacesRangeSunShadow(unsigned int partitionIndex);
void __cdecl R_AddAllSceneEntSurfacesSpotShadow(
        const GfxViewInfo *viewInfo,
        unsigned int spotShadowIndex,
        unsigned int primaryLightIndex);
void __cdecl R_AddSceneDObj(unsigned int entnum, unsigned int viewIndex);
void __cdecl R_DrawAllSceneEnt(const GfxViewInfo *viewInfo);
int __cdecl R_DrawBModel(
        BModelDrawInfo *bmodelInfo,
        const GfxBrushModel *bmodel,
        const GfxPlacement *placement,
        const ShaderConstantSet *constSet);
void __cdecl R_DrawAllDynEnt(const GfxViewInfo *viewInfo);
void __cdecl R_UnfilterEntFromCells(int localClientNum, unsigned int entnum);
void __cdecl R_UnfilterDynEntFromCells(unsigned int dynEntId, DynEntityDrawType drawType);
void __cdecl R_FilterDObjIntoCells(unsigned int localClientNum, unsigned int entnum, float *origin, float radius);
void __cdecl R_FilterEntIntoCells_r(FilterEntInfo *entInfo, mnode_t *node, const float *mins, const float *maxs);
void __cdecl R_FilterBModelIntoCells(unsigned int localClientNum, unsigned int entnum, GfxBrushModel *bmodel);
void __cdecl R_FilterDynEntIntoCells(unsigned int dynEntId, DynEntityDrawType drawType, float *mins, float *maxs);
void __cdecl R_FilterDynEntIntoCells_r(
        mnode_t *node,
        unsigned int dynEntIndex,
        DynEntityDrawType drawType,
        const float *mins,
        const float *maxs);
void __cdecl R_FilterXModelIntoScene(
        const XModel *model,
        const GfxScaledPlacement *placement,
        unsigned __int16 renderFxFlags,
        unsigned __int16 *cachedLightingHandle,
        float lightToleranceSq);
int __cdecl R_CullSphereDpvs(const float *origin, float radius, const DpvsPlane *planes, int planeCount);
void __cdecl R_InitialEntityCulling();
double __cdecl R_DpvsPlaneMaxSignedDistToBox(const DpvsPlane *plane, const float *minmax);
void __cdecl R_GetStaticModels(const float *mins, float *maxs, int *models, int *models_count, int max_models);
void __cdecl R_GetStaticModelsAabb(
        const GfxAabbTree *tree,
        const float *mins,
        const float *maxs,
        int *models,
        int *models_count,
        int max_models);
void __cdecl R_AddCellDynBrushSurfacesInFrustumCmd(const DpvsPlane **data);
void __cdecl R_CullDynBrushInCell(unsigned int cellIndex, DpvsPlane *planes, int planeCount);
unsigned int __cdecl R_SetVisData(unsigned int viewIndex);
void __cdecl R_GenerateShadowMapCasterCells();
void __cdecl R_VisitPortalsNoFrustum(const GfxCell *cell);
unsigned int __cdecl R_PortalClipPlanesNoFrustum(
        DpvsPlane *planes,
        unsigned int vertexCount,
        const float (*winding)[3]);
void __cdecl R_GetSidePlaneNormals(const float (*winding)[3], unsigned int vertexCount, float (*normals)[3]);
GfxPortal *__cdecl R_NextQueuedPortal();
void R_AssertValidQueue();
void __cdecl R_FreeHullPoints(GfxHullPointsPool *hullPoints);
void __cdecl R_VisitPortalsForCellNoFrustum(
        const GfxCell *cell,
        GfxPortal *parentPortal,
        const DpvsPlane *parentPlane,
        const DpvsPlane *planes,
        int planeCount,
        signed int recursionDepth);
void __cdecl R_EnqueuePortal(GfxPortal *portal);
double __cdecl R_FurthestPointOnWinding(const float (*points)[3], int pointCount, const DpvsPlane *plane);
bool __cdecl R_ShouldSkipPortal(const GfxPortal *portal, const DpvsPlane *planes, int planeCount);
double __cdecl R_DpvsPlaneDistToEye(const DpvsPlane *plane);
char __cdecl R_PortalBehindAnyPlane(const GfxPortal *portal, const DpvsPlane *planes, int planeCount);
char __cdecl R_PortalBehindPlane(const GfxPortal *portal, const DpvsPlane *plane);
char __cdecl R_ChopPortalAndAddHullPointsNoFrustum(
        GfxPortal *portal,
        const DpvsPlane *parentPlane,
        const DpvsPlane *planes,
        int planeCount);
void __cdecl R_AddVertToPortalHullPoints(GfxPortal *portal, const float *v);
GfxHullPointsPool *__cdecl R_AllocHullPoints();
int __cdecl R_ChopPortal(
        const GfxPortal *portal,
        const DpvsPlane *parentPlane,
        const DpvsPlane *planes,
        int planeCount,
        float (*v)[128][3],
        const float (**finalVerts)[3]);
const float (*__cdecl R_ChopPortalWinding(
        const float (*vertsIn)[3],
        int *vertexCount,
        const DpvsPlane *plane,
        float (*vertsOut)[3]))[3];
void __cdecl R_SetAncestorListStatus(GfxPortal *portal, bool isAncestor);
void __cdecl R_AddWorldSurfacesFrustumOnly(unsigned int viewIndex);
void __cdecl R_AddCellSurfacesAndCullGroupsInFrustumDelayed(
        const GfxCell *cell,
        const DpvsPlane *planes,
        unsigned __int8 planeCount,
        unsigned __int8 frustumPlaneCount);
void __cdecl R_ShowCull(const float *viewOrigin);
void __cdecl R_InitSceneData(int localClientNum);
void __cdecl DynEntCl_InitFilter();
void __cdecl R_InitSceneBuffers();
void __cdecl R_ShutdownSceneBuffers();
void __cdecl R_ClearDpvsScene();
void __cdecl R_ClearDpvsScene_For_ExtraCam();
bool __cdecl R_CullDynamicSpotLightInCameraView();
void __cdecl R_CullDynamicPointLightsInCameraView();
void __cdecl PointFrom3Planes(float *a, float *b, float *c, float *r);
void __cdecl R_SetupWorldSurfacesDpvs(const GfxViewParms *viewParms, unsigned int renderCullFlags);
void __cdecl R_EnableOccluder(const char *name, bool enable);
void __cdecl R_SetViewFrustumPlanes(GfxViewInfo *viewInfo);
void __cdecl R_AddWorldSurfacesDpvs(const GfxViewParms *viewParms, int cameraCellIndex);
void __cdecl R_AddWorldSurfacesPortalWalk(int cameraCellIndex);
void __cdecl R_SetCellVisible(const GfxCell *cell);
unsigned int __cdecl R_CellIsForcedInvisible(const GfxCell *cell);
void __cdecl R_VisitPortals(const GfxCell *cell, const DpvsPlane *parentPlane, DpvsPlane *planes, int planeCount);
unsigned int __cdecl R_PortalClipPlanes(
        DpvsPlane *planes,
        unsigned int vertexCount,
        const float (*winding)[3],
        DpvsClipChildren *clipChildren);
double __cdecl R_NearestPointOnWinding(const float (*points)[3], int pointCount, const DpvsPlane *plane);
void __cdecl R_ProjectPortal(
        int vertexCount,
        const float (*winding)[3],
        float *mins,
        float *maxs,
        DpvsClipChildren *clipChildren);
char *__cdecl R_PortalAssertMsg();
unsigned int __cdecl R_AddBevelPlanes(
        DpvsPlane *planes,
        unsigned int vertexCount,
        const float (*windingNormals)[3],
        float *mins,
        float *maxs,
        DpvsForceBevels forceBevels);
void __cdecl R_UnprojectPoint(const float *projected, float *unprojected);
void __cdecl R_VisitPortalsForCell(
        const GfxCell *cell,
        GfxPortal *parentPortal,
        const DpvsPlane *parentPlane,
        DpvsPlane *planes,
        unsigned int maxPlaneCount,
        unsigned int planeCount,
        int frustumPlaneCount,
        signed int recursionDepth,
        DpvsClipChildren clipChildren);
char __cdecl R_ChopPortalAndAddHullPoints(
        GfxPortal *portal,
        const DpvsPlane *parentPlane,
        const DpvsPlane *planes,
        int planeCount);
void __cdecl R_VisitAllFurtherCells(
        const GfxCell *cell,
        const DpvsPlane *parentPlane,
        const DpvsPlane *planes,
        int planeCount,
        unsigned __int8 frustumPlaneCount);
int __cdecl R_GetFurtherCellList_r(
        const GfxCell *cell,
        const DpvsPlane *parentPlane,
        const DpvsPlane *planes,
        int planeCount,
        float (*v)[128][3],
        const GfxCell **list,
        int count);
char __cdecl R_IsCellInList(const GfxCell *cell, const GfxCell **list, int count);
int __cdecl R_AddCellToList(const GfxCell *cell, const GfxCell **list, int count);
void __cdecl R_SetupShadowSurfacesDpvs(
        const GfxViewParms *viewParms,
        const float (*sidePlanes)[4],
        unsigned int sidePlaneCount,
        int partitionIndex);
double __cdecl R_GetFarPlaneDist();
void __cdecl R_SetCullDist(float dist);
int __cdecl R_CullBoxCurDpvs_SceneSelect(const float *minmax, unsigned int viewIndex, unsigned int sceneIndex);
int __cdecl R_CullBoxCurDpvs(const float *minmax, unsigned int viewIndex);
int __cdecl R_CullPoint(int localClient, const float *p0, float cutoff);
int __cdecl R_CullPoint(const float *point, float cutoff);
int __cdecl R_CullLine(int localClient, const float *p0, const float *p1, float cutoff);
unsigned int __cdecl R_ExtraCam_SaveDpvsData(int localClientNum, unsigned __int8 *buffer, unsigned int bufferByteSize);
void __cdecl R_ExtraCam_RestoreDpvsData(int localClientNum, unsigned __int8 *buffer);
void __cdecl R_PerMap_DpvsGlobInit();
unsigned int __cdecl R_CalcReflectionProbeIndex(const GfxWorld *world, const float *origin);
int __cdecl R_CellForPoint(const GfxWorld *world, const float *origin);
