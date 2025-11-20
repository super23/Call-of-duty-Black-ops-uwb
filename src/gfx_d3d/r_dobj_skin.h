#pragma once

void __cdecl R_FlagXModelAsSkinned(GfxSceneEntity *sceneEnt, unsigned int surfaceCount);
void __cdecl R_SkinSceneDObj(
        GfxSceneEntity *sceneEnt,
        GfxSceneEntity *localSceneEnt,
        const DObj *obj,
        DObjAnimMat *boneMatrix,
        int waitForCullState);
unsigned int  R_SkinSceneDObjModels@<eax>(
        int a1@<ebp>,
        GfxSceneEntity *sceneEnt,
        const DObj *obj,
        DObjAnimMat *boneMatrix);
int __cdecl R_AllocSkinnedCachedVerts(int vertCount, bool highPriority);
// local variable allocation has failed, the output may be wrong!
int  R_PreSkinXSurface@<eax>(
        GfxModelRigidSurface *a1@<ebp>,
        const DObj *obj,
        XSurface *surf,
        const GfxModelSurfaceInfo *surfaceInfo,
        unsigned int *numSkinnedVerts,
        GfxModelRigidSurface *surfPos);
void __cdecl QuatMultiplyInverse(const float *in1, const float *in2, float *out);
void __cdecl R_TransformSkelMat(const float *in1, const float4 *in2, float *out);
void __cdecl R_SkinGfxEntityCmd(GfxSceneEntity **data);
