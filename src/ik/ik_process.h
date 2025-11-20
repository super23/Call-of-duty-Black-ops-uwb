#pragma once

void __cdecl IK_GeneratePreIKMatrices(IKState *ikState, bool isLocalBones);
// local variable allocation has failed, the output may be wrong!
void  IK_GenerateIKXformMatrices(float *a1@<ebp>, IKState *ikState, bool isLocalBones);
char __cdecl IK_HasAnimatedBones(IKState *ikState);
void  IK_Process(int a1@<ebp>, IKState *ikState, bool isLocalBones);
