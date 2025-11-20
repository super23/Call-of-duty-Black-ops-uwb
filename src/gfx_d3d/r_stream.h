#pragma once

bool __cdecl R_StreamIsEnabled();
void __cdecl R_StreamPushDisable();
void __cdecl R_StreamPopDisable();
void __cdecl R_StreamSetInitData(int buffer_size);
void __cdecl R_StreamSetDefaultConfig(bool clear);
void __cdecl R_StreamSetUIConfig(bool clear);
double __cdecl R_Stream_GetProgress();
void __cdecl R_StreamUpdate_ReadTextures();
bool __cdecl R_StreamUpdate_ProcessFileCallbacks();
void __cdecl R_Stream_InvalidateRequest(pendingRequest *request);
void __cdecl R_StreamUpdate_SetupInitialImageList();
void __cdecl R_Stream_ForceLoadMaterial(Material *material, int part);
void __cdecl R_Stream_ForceLoadImage(GfxImage *image, int part);
void __cdecl R_Stream_ForceLoadModel(XModel *model, int part);
char __cdecl R_StreamTouchDObjAndCheck(const DObj *obj, int level);
void __cdecl R_StreamTouchMaterial(Material *material);
char __cdecl R_StreamTouchMaterialAndCheck(Material *material, int level);
void __cdecl R_StreamTouchImage(GfxImage *image);
bool __cdecl R_StreamTouchImageAndCheck(GfxImage *image, int level);
bool __cdecl R_StreamImageCheck(GfxImage *image, int level);
void __cdecl R_Stream_ResetHintEntities();
void __cdecl R_StreamInit();
void __cdecl R_StreamShutdown();
void __cdecl R_Stream_ReleaseImage(GfxImage *image, bool lock, bool delayDirty);
void __cdecl R_StreamSyncThenFlush(bool flushAll);
void R_Stream_Sync();
void __cdecl R_StreamPushSyncDisable();
void __cdecl R_StreamPopSyncDisable();
void __cdecl R_StreamUpdate_Idle();
void __cdecl R_StreamUpdate_CompletePreload(void (__cdecl *pumpfunc)());
char __cdecl R_StreamUpdate(const float *viewPos);
void __cdecl R_Stream_AddImagePartImportance(int imagePartIndex, unsigned int importance);
void __cdecl importance_swap_func(void **a, void **b);
bool __cdecl importance_compare_func(void *a, void *b);
void __cdecl importance_merge_sort(void **list, int list_count);
void __cdecl R_StreamUpdate_EndQuery();
unsigned intR_StreamUpdate_EndQuery_Internal();
char __cdecl R_StreamRequestImageAllocation(
        pendingRequest *request,
        GfxImage *image,
        bool highMip,
        int imagePart,
        float importance);
char __cdecl R_StreamRequestImageRead(pendingRequest *request);
char __cdecl R_StreamUpdate_FindImageAndOptimize(const float *viewPos);
void __cdecl R_StreamUpdate_AddInitialImages(unsigned int importance);
void __cdecl R_StreamUpdate_AddForcedImages(float forceImportance, float touchImportance);
void R_StreamUpdatePreventedMaterials();
void R_CheckHighmipAabbs();
void __cdecl R_CheckHighmipAabbs_r(GfxWorld *world, int aabbTreeNode, float *mins, float *maxs);
char __cdecl R_StreamUpdate_TryBeginQuery();
void R_StreamUpdateTouchedModels();
void __cdecl R_StreamUpdateForXModelTouched(const XModel *model);
void __cdecl R_StreamUpdateForcedModels();
void __cdecl R_StreamTouchImagesFromMaterial(const Material *remoteMaterial, unsigned int importance);
void __cdecl R_StreamUpdatePerClient(const float *viewPos);
void __cdecl R_StreamUpdateDynamicModels(
        const float *viewPos,
        float maxDistSq,
        unsigned int frame,
        float *distanceScale);
void __cdecl R_StreamUpdateForBModel(
        const float *viewPos,
        unsigned int frame,
        unsigned int surfId,
        const GfxBrushModel *bmodel,
        const float *origin,
        float maxDistSq,
        Material *altMaterial,
        bool isVisible,
        float *distanceScale);
double __cdecl PointDistSqFromBounds(const float *v, const float *mins, const float *maxs);
void __cdecl R_StreamUpdate_AddDynamicXModelDistance(
        const XModel *model,
        const float *viewPos,
        const float *origin,
        float scale,
        bool visible,
        float *distanceScale);
void __cdecl R_StreamUpdate_End();
void __cdecl R_Stream_UpdateStaticModelsCmd(char *data);
void __cdecl R_StreamUpdateStaticModel(
        int staticModelIndex,
        const float *viewPos,
        float maxDistSq,
        float *distanceScale);
void __cdecl R_StreamUpdate_AddXModelDistance(
        const XModel *model,
        const float *viewPos,
        const float *origin,
        float scale,
        bool visible,
        float *distanceScale);
void __cdecl R_Stream_UpdateStaticSurfacesCmd(char *data);
void __cdecl R_StreamUpdateWorldSurface(int surfId, const float *viewPos, float maxDistSq, float *distanceScale);
void __cdecl MultiplePointDistSqFromBounds(
        distance_data *distances,
        const float *v,
        const float *mip0mins,
        const float *mip0maxs,
        float himipRadiusSq,
        float distanceScale);
double __cdecl FastPointDistSqFromBounds(float4 mins, float4 maxs);
void __cdecl R_Stream_SortCmd(_BYTE *data);
void __cdecl R_StreamUpdate_EndQuerySort(bool diskOrder);
void __cdecl R_Stream_CombineCmd();
unsigned intR_StreamUpdate_CombineImportance();
void __cdecl R_StreamUpdateForXModel(const XModel *remoteModel, float distSq);
int __cdecl r_stream_update_staticmodelsCallback(jqBatch *batch);
int __cdecl r_stream_update_staticsurfacesCallback(jqBatch *batch);
int __cdecl r_stream_sortCallback(jqBatch *batch);
int __cdecl r_stream_combineCallback(jqBatch *batch);
int __cdecl r_stream_updateCallback(jqBatch *batch);
void __cdecl R_StreamUpdateStatic(const float *viewPos, float maxDistSq, float *distanceScale);
