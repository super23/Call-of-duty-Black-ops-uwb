#pragma once

void __cdecl IK_AllocateLocalClientMemory(void *ikStatesBuf, int localClientIndex);
void __cdecl IK_InitSystem();
IKState *__cdecl IK_GetIKStateForEntity(unsigned int entityNum, unsigned __int8 *model, bool bScanAll);
void __cdecl IK_InitializeIKState(IKState *ikState);
IKState *__cdecl IK_CreateIKForEntity(int entityNum, unsigned __int8 *model);
IKState *__cdecl IK_FindFreeIKState(int entityNum, unsigned __int8 *model);
// local variable allocation has failed, the output may be wrong!
void  IK_GetJointVars(IKJointBones *a1@<ebp>, IKState *ikState);
bool __cdecl IK_IsCalcBone(IKState *ikState, int ikBoneIndex);
void __cdecl IK_UpdateTimeAll(int time, int localClientIndex, bool inViewer);
void __cdecl IK_ResetTime();
void __cdecl IK_UpdateDvarValues(IKState *ikState);
void __cdecl IK_UpdateLayerLerps(IKState *ikState);
bool __cdecl IK_GetStatus(IKState *ikState, IKEntityStatusFunctions statusFunc);
void __cdecl IK_CheckSolverLayers(IKState *ikState);
void __cdecl IK_UpdateStatusFunctions(IKState *ikState);
void __cdecl IK_UpdateEntity(unsigned int entityNum, unsigned __int8 *model, bool isLocalBones, int *calcBoneBits);
