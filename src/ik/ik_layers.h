#pragma once

// local variable allocation has failed, the output may be wrong!
void  IK_Layer_TerrainMapping(float a1@<ebp>, IKState *ikState);
// local variable allocation has failed, the output may be wrong!
void  IK_Layer_ApplyFootIK(
        int a1@<ebp>,
        IKState *ikState,
        IKJointBones *jointBones,
        IKJointVars *jointVars,
        float (*childMat)[4]);
// local variable allocation has failed, the output may be wrong!
void  IK_Layer_ApplyHandIK(
        int a1@<ebp>,
        IKState *ikState,
        IKJointBones *jointBones,
        IKJointVars *jointVars,
        float (*childMat)[4]);
// local variable allocation has failed, the output may be wrong!
void  IK_Layer_LeftHandOnGun(float a1@<ebp>, IKState *ikState);
void __cdecl IK_Layer_PlayerControllers(IKState *ikState);
// local variable allocation has failed, the output may be wrong!
void  IK_Layer_PlayerPitch(float a1@<ebp>, IKState *ikState, bool preControllers);
// local variable allocation has failed, the output may be wrong!
void  IK_Layer_PreventHeadClip(float a1@<ebp>, IKState *ikState);
// local variable allocation has failed, the output may be wrong!
void  IK_ProcessLayers(int a1@<ebp>, IKState *ikState);
