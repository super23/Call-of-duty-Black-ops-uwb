#include "ik.h"

void __cdecl IK_AllocateLocalClientMemory(void *ikStatesBuf, int localClientIndex)
{
  dword_9C3D758[localClientIndex] = (int)ikStatesBuf;
}

void __cdecl IK_InitSystem()
{
  int i; // [esp+4h] [ebp-4h]

  IKImport_InitVars(&ikSystem);
  IKImport_GetBoneStrings(ikSystem.boneStrings);
  for ( i = 0; i < 23; ++i )
    ikSystem.ikBoneParents[i] = IKBoneParents[i];
  ikSystem.joints[0].childBone = IKBONE_LHAND;
  ikSystem.joints[0].jointBone = IKBONE_LFOREARM;
  ikSystem.joints[0].parentBone = IKBONE_LUPPERARM;
  ikSystem.joints[0].baseBone = IKBONE_LCLAVICLE;
  ikSystem.joints[1].childBone = IKBONE_RHAND;
  ikSystem.joints[1].jointBone = IKBONE_RFOREARM;
  ikSystem.joints[1].parentBone = IKBONE_RUPPERARM;
  ikSystem.joints[1].baseBone = IKBONE_RCLAVICLE;
  ikSystem.joints[2].childBone = IKBONE_LFOOT;
  ikSystem.joints[2].jointBone = IKBONE_LCALF;
  ikSystem.joints[2].parentBone = IKBONE_LTHIGH;
  ikSystem.joints[2].baseBone = IKBONE_PELVIS;
  ikSystem.joints[3].childBone = IKBONE_RFOOT;
  ikSystem.joints[3].jointBone = IKBONE_RCALF;
  ikSystem.joints[3].parentBone = IKBONE_RTHIGH;
  ikSystem.joints[3].baseBone = IKBONE_PELVIS;
  ikSystem.inViewer = 0;
  ikSystem.bInitialized = 1;
}

IKState *__cdecl IK_GetIKStateForEntity(unsigned int entityNum, unsigned __int8 *model, bool bScanAll)
{
  int actorNum; // [esp+0h] [ebp-Ch]
  int modelLocalClientIndex; // [esp+4h] [ebp-8h]
  int modelLocalClientIndexa; // [esp+4h] [ebp-8h]
  IKState *ikState; // [esp+8h] [ebp-4h]
  IKState *ikStatea; // [esp+8h] [ebp-4h]

  if ( entityNum >= 0x400
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik.cpp",
          389,
          0,
          "%s",
          "entityNum >= 0 && entityNum <= IK_STATE_ENTITY_LOOKUP_MAX") )
  {
    __debugbreak();
  }
  if ( !model )
    return 0;
  if ( entityNum == 1023 )
    return 0;
  ikState = IKImport_FetchDObjIKState(model);
  if ( !ikState )
  {
LABEL_17:
    modelLocalClientIndexa = IKImport_GetLocalClientIndexForModel(model);
    if ( !dword_9C3D758[modelLocalClientIndexa] )
      return 0;
    if ( (int)entityNum >= 32 )
    {
      actorNum = IKImport_GetActorNum(model).actorNum;
      if ( (unsigned int)actorNum >= 0x10
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik.cpp",
              442,
              0,
              "%s",
              "actorNum >= 0 && actorNum < MAX_ACTORS") )
      {
        __debugbreak();
      }
      ikStatea = (IKState *)(dword_9C3D758[modelLocalClientIndexa] + 3680 * (actorNum + 32));
    }
    else
    {
      ikStatea = (IKState *)(dword_9C3D758[modelLocalClientIndexa] + 3680 * entityNum);
    }
    if ( ikStatea->inUse )
    {
      ikStatea->model = model;
      IK_InitializeIKState(ikStatea);
      IKImport_SetDObjIKState((DObj *)model, ikStatea);
      return ikStatea;
    }
    else
    {
      if ( bScanAll )
        return IK_CreateIKForEntity(entityNum, model);
      return 0;
    }
  }
  modelLocalClientIndex = IKImport_GetLocalClientIndexForModel(model);
  if ( ikState->inUse )
  {
    if ( ikState->entityNum == entityNum && ikState->localClientIndex == modelLocalClientIndex )
      return ikState;
    ikState->inUse = 0;
    IKImport_SetDObjIKState((DObj *)model, 0);
    goto LABEL_17;
  }
  if ( IKImport_IsMainThread() )
    return IK_CreateIKForEntity(entityNum, model);
  return 0;
}

void __cdecl IK_InitializeIKState(IKState *ikState)
{
  IKBoneNames i; // [esp+10h] [ebp-4h]

  memset(ikState->objBoneToIKBone, 0, sizeof(ikState->objBoneToIKBone));
  for ( i = IKBONE_FIRST; i < IKBONE_COUNT; ++i )
  {
    if ( IKImport_GetBoneLookups(ikState, i) )
    {
      if ( ikState->objBoneToIKBone[ikState->ikBoneToObjBone[i]] != (unsigned __int8)(i + 1) )
        IKImport_DebugPrint(
          "IK_InitializeIKState() - IK bones don't match.\n"
          "i = %i\n"
          "ikState->ikBoneToObjBone[i] = %i\n"
          "ikState->objBoneToIKBone[ikState->ikBoneToObjBone[i]] = %i (should be %i)\n",
          i,
          ikState->ikBoneToObjBone[i],
          ikState->objBoneToIKBone[ikState->ikBoneToObjBone[i]],
          i + 1);
      if ( ikState->objBoneToIKBone[ikState->ikBoneToObjBone[i]] != (unsigned __int8)(i + 1)
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik.cpp",
              577,
              0,
              "%s",
              "ikState->objBoneToIKBone[ikState->ikBoneToObjBone[i]] == (byte)(i + 1)") )
      {
        __debugbreak();
      }
    }
    else
    {
      if ( ikEssentialBones[i] )
      {
        ikState->isIKModel = 0;
        return;
      }
      ikState->ikBoneToObjBone[i] = -95;
    }
  }
  ikState->isIKModel = 1;
  ikState->lastFootTraceNormals[0][0] = *(float *)&FLOAT_0_0;
  ikState->lastFootTraceNormals[0][1] = *(float *)&FLOAT_0_0;
  ikState->lastFootTraceNormals[0][2] = FLOAT_1_0;
  ikState->lastFootTraceNormals[1][0] = *(float *)&FLOAT_0_0;
  ikState->lastFootTraceNormals[1][1] = *(float *)&FLOAT_0_0;
  ikState->lastFootTraceNormals[1][2] = FLOAT_1_0;
  ikState->lastFootTraceGoalNormals[0][0] = *(float *)&FLOAT_0_0;
  ikState->lastFootTraceGoalNormals[0][1] = *(float *)&FLOAT_0_0;
  ikState->lastFootTraceGoalNormals[0][2] = FLOAT_1_0;
  ikState->lastFootTraceGoalNormals[1][0] = *(float *)&FLOAT_0_0;
  ikState->lastFootTraceGoalNormals[1][1] = *(float *)&FLOAT_0_0;
  ikState->lastFootTraceGoalNormals[1][2] = FLOAT_1_0;
  ikState->leftHandLastProximityLerp = *(float *)&FLOAT_0_0;
  IKImport_InitCollisionCache(ikState);
}

IKState *__cdecl IK_CreateIKForEntity(int entityNum, unsigned __int8 *model)
{
  IKState *ikState; // [esp+0h] [ebp-4h]

  Sys_EnterCriticalSection(CRITSECT_IK);
  ikState = IK_FindFreeIKState(entityNum, model);
  if ( !ikState
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik.cpp",
          608,
          0,
          "%s\n\t%s",
          "ikState",
          "no free IK slots") )
  {
    __debugbreak();
  }
  ikState->inUse = 1;
  ikState->entityNum = entityNum;
  ikState->model = model;
  ikState->bJointVarsValid = 0;
  memset((unsigned __int8 *)ikState->lastFrameMS, 0, sizeof(ikState->lastFrameMS));
  ikState->isServer = IKImport_IsServerModel(model);
  ikState->localClientIndex = IKImport_GetLocalClientIndexForModel(model);
  ikState->localIkSystem = &ikSystem;
  IK_InitializeIKState(ikState);
  ikState->timeMS = 1000;
  IKImport_SetDObjIKState((DObj *)model, ikState);
  Sys_LeaveCriticalSection(CRITSECT_IK);
  return ikState;
}

IKState *__cdecl IK_FindFreeIKState(int entityNum, unsigned __int8 *model)
{
  int listNum; // [esp+4h] [ebp-4h]

  listNum = IKImport_GetLocalClientIndexForModel(model) + 1;
  if ( entityNum < 32 )
    return &ikStates[listNum][entityNum];
  Com_Error(ERR_DROP, "IK_FindFreeIKState() - entityNum is not a client: %i", entityNum);
  return 0;
}

// local variable allocation has failed, the output may be wrong!
void  IK_GetJointVars(IKJointBones *a1@<ebp>, IKState *ikState)
{
  float v2; // [esp-8h] [ebp-138h]
  float footHeightFromGround; // [esp+0h] [ebp-130h]
  _BYTE posParent_4[76]; // [esp+24h] [ebp-10Ch] OVERLAPPED BYREF
  _BYTE posJoint[80]; // [esp+70h] [ebp-C0h] OVERLAPPED BYREF
  _BYTE posChild[64]; // [esp+C0h] [ebp-70h] OVERLAPPED BYREF
  IKJointVars *v8; // [esp+114h] [ebp-1Ch]
  int *joints; // [esp+118h] [ebp-18h]
  int v10; // [esp+11Ch] [ebp-14h]
  IKJointVars *jointVars; // [esp+120h] [ebp-10h]
  IKJointBones *joint; // [esp+124h] [ebp-Ch]
  int i; // [esp+128h] [ebp-8h]
  int retaddr; // [esp+130h] [ebp+0h]

  joint = a1;
  i = retaddr;
  HIBYTE(jointVars) = 1;
  v10 = 0;
  joints = (int *)ikState->localIkSystem->joints;
  v8 = ikState->jointVars;
  while ( v10 < 4 )
  {
    if ( !v8->bValid && *joints )
    {
      if ( ikState->ikBoneToObjBone[*joints] != 161 )
      {
        ikCalcBoneModelMatrix(ikState, *joints, (*ikState->matArrayPostIK)[0], (float (*)[4])&posChild[4]);
        *(unsigned int *)posChild = &posChild[52];
        *(_QWORD *)&posJoint[68] = *(_QWORD *)&posChild[52];
        *(unsigned int *)&posJoint[76] = *(unsigned int *)&posChild[60];
        ikCalcBoneModelMatrix(ikState, joints[1], (*ikState->matArrayPostIK)[0], (float (*)[4])&posJoint[4]);
        *(unsigned int *)posJoint = &posJoint[52];
        *(_QWORD *)&posParent_4[64] = *(_QWORD *)&posJoint[52];
        *(unsigned int *)&posParent_4[72] = *(unsigned int *)&posJoint[60];
        ikCalcBoneModelMatrix(ikState, joints[2], (*ikState->matArrayPostIK)[0], (float (*)[4])posParent_4);
        footHeightFromGround = fsqrt(
                                 (float)((float)((float)(*(float *)&posJoint[68] - *(float *)&posParent_4[64])
                                               * (float)(*(float *)&posJoint[68] - *(float *)&posParent_4[64]))
                                       + (float)((float)(*(float *)&posJoint[72] - *(float *)&posParent_4[68])
                                               * (float)(*(float *)&posJoint[72] - *(float *)&posParent_4[68])))
                               + (float)((float)(*(float *)&posJoint[76] - *(float *)&posParent_4[72])
                                       * (float)(*(float *)&posJoint[76] - *(float *)&posParent_4[72])));
        v2 = fsqrt(
               (float)((float)((float)(*(float *)&posParent_4[64] - *(float *)&posParent_4[48])
                             * (float)(*(float *)&posParent_4[64] - *(float *)&posParent_4[48]))
                     + (float)((float)(*(float *)&posParent_4[68] - *(float *)&posParent_4[52])
                             * (float)(*(float *)&posParent_4[68] - *(float *)&posParent_4[52])))
             + (float)((float)(*(float *)&posParent_4[72] - *(float *)&posParent_4[56])
                     * (float)(*(float *)&posParent_4[72] - *(float *)&posParent_4[56])));
        if ( v10 < 2 || (float)(v2 + footHeightFromGround) <= 40.0 && (float)(v2 + footHeightFromGround) >= 18.0 )
        {
          v8->UpperLength = v2;
          v8->UpperIKc = 1.0 / (float)(2.0 * v2);
          v8->UpperIKInvc = (float)((float)(v2 * v2) - (float)(footHeightFromGround * footHeightFromGround))
                          / (float)(2.0 * v2);
          v8->LowerLength = footHeightFromGround;
          v8->LowerIKc = 1.0 / (float)(2.0 * footHeightFromGround);
          v8->LowerIKInvc = (float)((float)(footHeightFromGround * footHeightFromGround) - (float)(v2 * v2))
                          / (float)(2.0 * footHeightFromGround);
          if ( v10 >= 2 )
            ikState->footHeightFromGround = FLOAT_5_8000002;
          v8->bValid = 1;
        }
        else
        {
          HIBYTE(jointVars) = 0;
        }
      }
      else
      {
        HIBYTE(jointVars) = 0;
      }
    }
    ++v10;
    joints += 4;
    ++v8;
  }
  if ( HIBYTE(jointVars) )
    ikState->bJointVarsValid = 1;
}

bool __cdecl IK_IsCalcBone(IKState *ikState, int ikBoneIndex)
{
  return (ikState->objBoneCalcBits[(int)ikState->ikBoneToObjBone[ikBoneIndex] >> 5]
        & (0x80000000 >> (ikState->ikBoneToObjBone[ikBoneIndex] & 0x1F))) != 0;
}

void __cdecl IK_UpdateTimeAll(int time, int localClientIndex, bool inViewer)
{
  IKState *ikState; // [esp+0h] [ebp-8h]
  int i; // [esp+4h] [ebp-4h]

  if ( IKImport_GetVar_IK_Enable() && dword_9C3D758[localClientIndex] )
  {
    i = 0;
    ikState = (IKState *)dword_9C3D758[localClientIndex];
    while ( i < 32 )
    {
      if ( ikState->inUse )
      {
        if ( ikState->localClientIndex != localClientIndex
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik.cpp",
                772,
                0,
                "%s",
                "ikState->localClientIndex == localClientIndex") )
        {
          __debugbreak();
        }
        ikState->timeMS = time;
      }
      ++i;
      ++ikState;
    }
    ikSystem.systemFrameNumber = IKImport_GetFrameNum();
    ikSystem.inViewer = inViewer;
  }
}

void __cdecl IK_ResetTime()
{
  int j; // [esp+0h] [ebp-14h]
  int k; // [esp+4h] [ebp-10h]
  IKState *ikState; // [esp+8h] [ebp-Ch]
  int i; // [esp+10h] [ebp-4h]

  if ( IKImport_GetVar_IK_Enable() )
  {
    for ( j = 0; j < 2; ++j )
    {
      if ( ikStates[j] )
      {
        i = 0;
        ikState = ikStates[j];
        while ( i < 32 )
        {
          if ( ikState && ikState->inUse )
          {
            ikState->timeMS = -9999;
            ikState->lastUpdateLayerLerpsTime = ikState->timeMS;
            ikState->leftHandChangeTime = 0;
            for ( k = 0; k < 21; ++k )
            {
              ikState->lerpInfo[k].lastUpdateTime = ikState->timeMS;
              ikState->lastFrameMS[k] = ikState->timeMS;
            }
          }
          ++i;
          ++ikState;
        }
      }
    }
    ikSystem.systemFrameNumber = -1;
  }
}

void __cdecl IK_UpdateDvarValues(IKState *ikState)
{
  DvarValue *p_current; // eax
  DvarValue *v2; // ecx
  int i; // [esp+0h] [ebp-4h]

  if ( ikSystem.dvars[0] )
  {
    for ( i = 0; i < 23; ++i )
    {
      p_current = &ikSystem.dvars[i]->current;
      v2 = &ikState->dvarValues[i];
      v2->integer = p_current->integer;
      v2->vector[1] = p_current->vector[1];
      v2->vector[2] = p_current->vector[2];
      v2->vector[3] = p_current->vector[3];
    }
  }
  else
  {
    memset((unsigned __int8 *)ikState->dvarValues, 0, sizeof(ikState->dvarValues));
  }
}

void __cdecl IK_UpdateLayerLerps(IKState *ikState)
{
  IKLayerNames i; // [esp+4h] [ebp-4h]

  if ( ikState->lastUpdateLayerLerpsTime != ikState->timeMS )
  {
    IKImport_GetVelocity(ikState, ikState->velocity);
    IKImport_SetWeaponOffsetType(ikState);
    IKImport_UpdateWeaponClass(ikState);
    IK_UpdateDvarValues(ikState);
    ikState->bHasActiveLayers = 0;
    for ( i = IKLAYER_CROUCH_ACTIVE; i < IKLAYER_COUNT; ++i )
    {
      if ( IKImport_GetLayerLerp(ikState, i, ikLayerLerpTimes[i]) > 0.0 )
      {
        if ( ikLayerRealLayers[i] )
          ikState->bHasActiveLayers = 1;
      }
    }
    ikState->lastUpdateLayerLerpsTime = ikState->timeMS;
  }
}

bool __cdecl IK_GetStatus(IKState *ikState, IKEntityStatusFunctions statusFunc)
{
  return (ikState->statusFlags & (1 << statusFunc)) != 0;
}

void __cdecl IK_CheckSolverLayers(IKState *ikState)
{
  IKLayerNames i; // [esp+4h] [ebp-4h]

  ikState->bHasLegSolverLayer = 0;
  ikState->bHasArmSolverLayer = 0;
  for ( i = IKLAYER_CROUCH_ACTIVE; i < IKLAYER_COUNT; ++i )
  {
    if ( IKImport_GetLayerLerp(ikState, i, ikLayerLerpTimes[i]) != 0.0 )
    {
      if ( ikLegSolverLayers[i] )
        ikState->bHasLegSolverLayer = 1;
      if ( ikArmSolverLayers[i] )
        ikState->bHasArmSolverLayer = 1;
    }
  }
}

void __cdecl IK_UpdateStatusFunctions(IKState *ikState)
{
  int i; // [esp+4h] [ebp-4h]
  int savedregs; // [esp+8h] [ebp+0h] BYREF

  ikState->hasControllers = IKImport_GetBoneControllers(ikState, ikState->controllers[0]);
  if ( ikState->lastUpdateStatusFlags != ikState->timeMS )
  {
    ikState->statusFlags = 0;
    for ( i = 0; i < 10; ++i )
    {
      if ( ikStatusFunctions[i](ikState) )
        ikState->statusFlags |= 1 << i;
    }
    ikState->gunnerCrouch = IKImport_GetGunnerCrouch(ikState);
    if ( IKImport_GetLayerLerp(ikState, IKLAYER_TERRAIN_MAPPING, flt_E12704) > 0.0 )
      IKImport_UpdateCollisionCache(ikState);
    BG_EvalVehicleName();
    IKImport_GetEntityXform((gentity_s *)&savedregs, ikState);
    IK_CheckSolverLayers(ikState);
    ikState->lastUpdateStatusFlags = ikState->timeMS;
  }
}

void __cdecl IK_UpdateEntity(unsigned int entityNum, unsigned __int8 *model, bool isLocalBones, int *calcBoneBits)
{
  IKState *ikState; // [esp+10h] [ebp-4h]
  int savedregs; // [esp+14h] [ebp+0h] BYREF

  if ( isLocalBones )
  {
    PIXBeginNamedEvent(-1, "IK - update bones");
    ikState = IK_GetIKStateForEntity(entityNum, model, 0);
    if ( !ikState )
    {
      if ( g_DXDeviceThread != GetCurrentThreadId() )
        return;
      goto LABEL_19;
    }
    if ( (!ikState->localIkSystem || !ikState->localIkSystem->bInitialized)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik.cpp",
            999,
            0,
            "%s",
            "ikState->ikSystem && ikState->ikSystem->bInitialized") )
    {
      __debugbreak();
    }
    if ( ikState->isIKModel )
    {
      if ( ikState->frameUsed < ikState->localIkSystem->systemFrameNumber
        || ikState->frameUsed > ikState->localIkSystem->systemFrameNumber + 5 )
      {
        ikState->frameUsed = ikState->localIkSystem->systemFrameNumber;
      }
      ikState->objBoneCalcBits[0] = *calcBoneBits;
      ikState->objBoneCalcBits[1] = calcBoneBits[1];
      ikState->objBoneCalcBits[2] = calcBoneBits[2];
      ikState->objBoneCalcBits[3] = calcBoneBits[3];
      if ( IK_IsCalcBone(ikState, 1) )
      {
        IKImport_CalcAnimatedIKBonesBits(ikState, model);
        IK_UpdateLayerLerps(ikState);
        IK_UpdateStatusFunctions(ikState);
        if ( IKImport_CheckIKPartBits(ikState) )
        {
          if ( !ikState->timeMS )
            ikState->timeMS = 10000;
          IKImport_GetEntityXform((gentity_s *)&savedregs, ikState);
          IK_Process((int)&savedregs, ikState, isLocalBones);
        }
      }
    }
    if ( GetCurrentThreadId() == g_DXDeviceThread )
LABEL_19:
      D3DPERF_EndEvent();
  }
}

