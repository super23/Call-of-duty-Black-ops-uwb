#pragma once

void __cdecl R_ModelList_f();
bool __cdecl R_ModelSort(XModel **model1, XModel **model2);
void __cdecl R_GetModelList(XAssetHeader header, XAssetHeader *data);
XModel *__cdecl R_RegisterModel(char *name);
unsigned __int8 *__cdecl Hunk_AllocXModelPrecache(unsigned int size);
unsigned __int8 *__cdecl Hunk_AllocXModelPrecacheColl(unsigned int size);
void __cdecl R_FinishLoadingModels();
void __cdecl R_XModelDebug(const DObj *obj, int *partBits);
void __cdecl R_XModelDebugBoxes(const DObj *obj, int *partBits);
void __cdecl R_XModelDebugAxes(const DObj *obj, int *partBits);
void __cdecl R_LockSkinnedCache();
void __cdecl R_UnlockSkinnedCache();
int  R_SkinXModel@<eax>(
        int a1@<ebp>,
        XModelDrawInfo *modelInfo,
        const XModel *model,
        const DObj *obj,
        const GfxPlacement *placement,
        float scale,
        __int16 gfxEntIndex,
        bool offscreen);
int __cdecl R_SkinAndBoundSceneEnt(GfxSceneEntity *sceneEnt);
void __cdecl R_ShutdownModels();
void __cdecl R_ReleaseAllModels();
void __cdecl R_ReleaseModel(XAssetHeader header);
void __cdecl R_OptimizeAllModels();
void __cdecl R_OptimizeModel(XAssetHeader header);
void __cdecl XSurfaceReleaseResources(XSurface *surf);
void __cdecl XSurfaceRenderString(const float *pos, char *string);
