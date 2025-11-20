#pragma once

void __cdecl BG_Player_DoControllers(const CEntPlayerInfo *player, const DObj *obj, int *partBits);
void __cdecl CG_Destructible_DoControllers(const cpose_t *pose, const DObj *obj, int *partBits);
void __cdecl CG_Actor_DoControllers(const cpose_t *pose, const DObj *obj, int *partBits);
void __cdecl CG_DoControllers(const cpose_t *pose, const DObj *obj, int *partBits);
void __cdecl CG_Player_DoControllers(const cpose_t *pose, const DObj *obj, int *partBits);
void  CG_Vehicle_DoControllers(void *a1@<ebp>, const cpose_t *pose, const DObj *obj, int *partBits);
void __cdecl CG_mg42_DoControllers(const cpose_t *pose, const DObj *obj, int *partBits);
void __cdecl CG_DoBaseOriginController(const cpose_t *pose, const DObj *obj, int *setPartBits);
double __cdecl Vec4LengthSq(const float *v);
void __cdecl MatrixTransformVectorQuatTrans(const float *in, const DObjAnimMat *mat, float *out);
void __cdecl QuatMultiplyEquals(const float *in, float *inout);
DObjAnimMat *__cdecl CG_DObjCalcPose(const cpose_t *remote_pose, DObj *obj, int *partBits);
