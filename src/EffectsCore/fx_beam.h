#pragma once

// local variable allocation has failed, the output may be wrong!
void __cdecl FX_Beam_GenerateVerts(FxGenerateVertsCmd *cmd);
void __cdecl CreateClipMatrix(vector4 *clipMtx, const float *vieworg, const float (*viewaxis)[3]);
void __cdecl Float4x4ForViewer(vector4 *mtx, const float *origin3, const float (*axis3)[3]);
void __cdecl Float4x4InfinitePerspectiveMatrix(vector4 *mtx, float tanHalfFovX, float tanHalfFovY, float zNear);
// local variable allocation has failed, the output may be wrong!
char  FX_GenerateBeam_GetFlatDelta@<al>(
        unsigned int a1@<ebp>,
        const vector4 *clipMtx,
        const vector4 *invClipMtx,
        const float4 *beamWorldBegin,
        const float4 *beamWorldEnd,
        float4 *outFlatDelta);
bool __cdecl Vec4HomogenousClipBothZ(float4 *pt0, float4 *pt1);
bool __cdecl Vec4HomogenousClipZW(float4 *pt0, float4 *pt1, const float4 *coeffZW);
double __cdecl I_fres(float val);
void __cdecl FX_Beam_Begin();
void __cdecl FX_Beam_Add(FxBeam *beam);
FxBeamInfo *__cdecl FX_Beam_GetInfo();
