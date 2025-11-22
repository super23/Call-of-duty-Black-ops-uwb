#include "ik_import.h"

void __cdecl IKImport_InitVars(IKSystem *ikSystem)
{
  ikSystem->localClipMap = (unsigned __int8 *)CM_GetClipMap();
  ikSystem->dvars[0] = _Dvar_RegisterBool("ik_enable", 1, 0x4080u, "Turn on/off IK");
  ikSystem->dvars[1] = _Dvar_RegisterInt(
                         "ik_debug",
                         0,
                         0,
                         2,
                         0x4080u,
                         "Set to one of the IK debug values in ik_types.h");
  ikSystem->dvars[2] = _Dvar_RegisterBool("ik_pelvis_test", 0, 0x4080u, "Turn on/off IK pelvis test");
  ikSystem->dvars[3] = _Dvar_RegisterBool("ik_foot_test", 0, 0x4080u, "Turn on/off IK foot test");
  ikSystem->dvars[4] = _Dvar_RegisterBool("ik_hand_test", 0, 0x4080u, "Turn on/off IK hand test");
  ikSystem->dvars[5] = _Dvar_RegisterBool("ik_left_hand_lerp_test", 0, 0x4080u, "Turn on/off left-hand lerp test");
  ikSystem->dvars[6] = _Dvar_RegisterBool("ik_right_hand_lerp_test", 0, 0x4080u, "Turn on/off right-hand lerp test");
  ikSystem->dvars[7] = _Dvar_RegisterBool("ik_enable_player_hand", 1, 0x4080u, "Turn on/off player hand IK");
  ikSystem->dvars[8] = _Dvar_RegisterBool("ik_enable_player_terrain", 1, 0x4080u, "Turn on/off player terrain IK");
  ikSystem->dvars[9] = _Dvar_RegisterBool("ik_enable_ai_hand", 0, 0x4080u, "Turn on/off AI hand IK");
  ikSystem->dvars[10] = _Dvar_RegisterInt(
                          "ik_enable_ai_terrain",
                          2,
                          0,
                          2,
                          0x4080u,
                          "Turn on/off AI terrain IK (0 = OFF, 1 = All, 2 = Heros)");
  ikSystem->dvars[11] = _Dvar_RegisterInt(
                          "ik_ai_hand_tuning",
                          0,
                          0,
                          2,
                          0x4080u,
                          "Turn on/off AI hand IK tuning. 1 = Use anim flag. 2 = Force on for all anims.");
  ikSystem->dvars[12] = _Dvar_RegisterVec3(
                          "ik_ai_hand_offset_vec",
                          COERCE_UNSIGNED_INT(0.0),
                          COERCE_UNSIGNED_INT(0.0),
                          COERCE_UNSIGNED_INT(0.0),
                          -10.0,
                          10.0,
                          0x4080u,
                          "Offset vector for hand tuning");
  ikSystem->dvars[13] = _Dvar_RegisterVec3(
                          "ik_ai_hand_rotation_vec",
                          COERCE_UNSIGNED_INT(0.0),
                          COERCE_UNSIGNED_INT(0.0),
                          COERCE_UNSIGNED_INT(0.0),
                          -180.0,
                          180.0,
                          0x4080u,
                          "Rotation vector for hand tuning");
  ikSystem->dvars[14] = _Dvar_RegisterBool(
                          "ik_paranoid_matrix_checks",
                          0,
                          0x4080u,
                          "Turn on/off THOROUGH matrix validity checks");
  ikSystem->dvars[15] = _Dvar_RegisterFloat(
                          "ik_actor_tracking_scale",
                          1.0,
                          0.0,
                          1.0,
                          0x4080u,
                          "Adjust the amount of rotation that is applied during head/eye tracking");
  ikSystem->dvars[16] = _Dvar_RegisterFloat(
                          "ik_pitch_limit_thresh",
                          10.0,
                          0.0,
                          180.0,
                          0x1000u,
                          "Pitch angle at which headtracking starts to be constrained");
  ikSystem->dvars[17] = _Dvar_RegisterFloat(
                          "ik_pitch_limit_max",
                          60.0,
                          0.0,
                          180.0,
                          0x1000u,
                          "Pitch angle at which headtracking is entirely constrained");
  ikSystem->dvars[18] = _Dvar_RegisterFloat(
                          "ik_roll_limit_thresh",
                          30.0,
                          0.0,
                          180.0,
                          0x1000u,
                          "Roll angle at which headtracking starts to be constrained");
  ikSystem->dvars[19] = _Dvar_RegisterFloat(
                          "ik_roll_limit_max",
                          100.0,
                          0.0,
                          180.0,
                          0x1000u,
                          "Roll angle at which headtracking is entirely constrained");
  ikSystem->dvars[20] = _Dvar_RegisterFloat(
                          "ik_yaw_limit_thresh",
                          10.0,
                          0.0,
                          180.0,
                          0x1000u,
                          "Yaw angle at which headtracking starts to be constrained");
  ikSystem->dvars[21] = _Dvar_RegisterFloat(
                          "ik_yaw_limit_max",
                          90.0,
                          0.0,
                          180.0,
                          0x1000u,
                          "Yaw angle at which headtracking is entirely constrained");
  ikSystem->dvars[22] = _Dvar_RegisterBool(
                          "ik_dvar_lookatentity_notorso",
                          0,
                          0x1000u,
                          "No torso deformation in lookatentity transforms");
}

bool __cdecl IKImport_GetVar_IK_Enable()
{
  return ikSystem.dvars[0] && ikSystem.dvars[0]->current.enabled;
}

bool __cdecl IKImport_GetVar_IK_Pelvis_Test()
{
  return ikSystem.dvars[2]->current.enabled;
}

bool __cdecl IKImport_GetVar_IK_Foot_Test()
{
  return ikSystem.dvars[3]->current.enabled;
}

bool __cdecl IKImport_GetVar_IK_Hand_Test()
{
  return ikSystem.dvars[4]->current.enabled;
}

bool __cdecl IKImport_GetVar_IK_Left_Hand_Lerp_Test()
{
  return ikSystem.dvars[5]->current.enabled;
}

bool __cdecl IKImport_GetVar_IK_Right_Hand_Lerp_Test()
{
  return ikSystem.dvars[6]->current.enabled;
}

int __cdecl IKImport_GetVar_IK_Debug()
{
  return ikSystem.dvars[1]->current.integer;
}

int __cdecl IKImport_GetVar_IK_Hand_Tuning()
{
  return ikSystem.dvars[11]->current.integer;
}

void __cdecl IKImport_GetVar_IK_Hand_Offset_Vec(float *offset)
{
  DvarValue *p_current; // [esp+0h] [ebp-4h]

  p_current = &ikSystem.dvars[12]->current;
  *offset = ikSystem.dvars[12]->current.value;
  offset[1] = p_current->vector[1];
  offset[2] = p_current->vector[2];
}

void __cdecl IKImport_GetVar_IK_Hand_Rotation_Vec(float *rotation)
{
  DvarValue *p_current; // [esp+0h] [ebp-4h]

  p_current = &ikSystem.dvars[13]->current;
  *rotation = ikSystem.dvars[13]->current.value;
  rotation[1] = p_current->vector[1];
  rotation[2] = p_current->vector[2];
}

int __cdecl IKImport_GetVar_IK_Hand_Tuning(IKState *ikState)
{
  return ikState->dvarValues[11].integer;
}

void __cdecl IKImport_GetBoneStrings(char **outBoneStrings)
{
  int i; // [esp+0h] [ebp-4h]
  int ia; // [esp+0h] [ebp-4h]

  for ( i = 0; i < 23; ++i )
  {
    ikImportBoneStringVars[i] = Scr_AllocString((char *)ikImportBoneStrings[i], 1, SCRIPTINSTANCE_SERVER);
    outBoneStrings[i] = (char *)ikImportBoneStrings[i];
  }
  for ( ia = 0; ia < 3; ++ia )
    ikImportMiscBoneStringVars[ia] = Scr_AllocString((char *)ikImportMiscBoneStrings[ia], 1, SCRIPTINSTANCE_SERVER);
  ikSystem.ikImportBoneStringVars = ikImportBoneStringVars;
  ikSystem.ikImportMiscBoneStringVars = ikImportMiscBoneStringVars;
}

char __cdecl IKImport_GetBoneLookups(IKState *ikState, IKBoneNames ikBoneName)
{
  unsigned __int8 lookup; // [esp+3h] [ebp-9h] BYREF
  DObj *obj; // [esp+4h] [ebp-8h]
  int result; // [esp+8h] [ebp-4h]

  if ( !ikState->model
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 379, 0, "%s", "ikState->model") )
  {
    __debugbreak();
  }
  obj = (DObj *)ikState->model;
  lookup = -2;
  result = DObjGetBoneIndex(obj, ikImportBoneStringVars[ikBoneName], &lookup, 0);
  if ( !result || lookup >= (int)(*obj->localModels)->numBones )
    return 0;
  if ( lookup >= 0xA0u
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp",
          392,
          0,
          "%s",
          "lookup < tl_align( IK_MAX_MODEL_BONES, 4 )") )
  {
    __debugbreak();
  }
  ikState->objBoneToIKBone[lookup] = ikBoneName + 1;
  ikState->ikBoneToObjBone[ikBoneName] = lookup;
  return 1;
}

char __cdecl IKImport_CalcAnimatedIKBonesBits(IKState *ikState, unsigned __int8 *model)
{
  int ikBoneIndex; // [esp+4h] [ebp-8h]

  if ( !model && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 413, 0, "%s", "obj") )
    __debugbreak();
  ikState->animatedIKBonesBits[0] = 0;
  for ( ikBoneIndex = 0; ikBoneIndex < 23; ++ikBoneIndex )
  {
    if ( (*(unsigned int *)&model[4 * ((int)ikState->ikBoneToObjBone[ikBoneIndex] >> 5) + 20]
        & (0x80000000 >> (ikState->ikBoneToObjBone[ikBoneIndex] & 0x1F))) != 0 )
      ikState->animatedIKBonesBits[ikBoneIndex >> 5] |= 0x80000000 >> (ikBoneIndex & 0x1F);
  }
  return 1;
}

void __cdecl IKImport_SetIKPartBits(DObj *obj, int *partBits)
{
  IKState *ikState; // [esp+0h] [ebp-8h]
  unsigned int i; // [esp+4h] [ebp-4h]

  if ( IKImport_GetVar_IK_Enable() )
  {
    if ( !ikSystem.bInitialized )
      IK_InitSystem();
    ikState = IK_GetIKStateForEntity(obj->entnum - 1, (unsigned __int8 *)obj, 0);
    if ( ikState )
      goto LABEL_17;
    ikState = IK_CreateIKForEntity(obj->entnum - 1, (unsigned __int8 *)obj);
    if ( !ikState
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp",
            504,
            0,
            "%s\n\t%s",
            "ikState",
            "no free IK slots") )
    {
      __debugbreak();
    }
    if ( ikState )
    {
LABEL_17:
      if ( ikState->isIKModel )
      {
        for ( i = 0; i < 6; ++i )
        {
          if ( ikState->ikBoneToObjBone[minBoneExtents[i]] < (int)(*obj->localModels)->numBones )
            IKImport_RecurseSetIKPartBits(obj, ikState->ikBoneToObjBone[minBoneExtents[i]], partBits);
        }
      }
    }
  }
}

void __cdecl IKImport_RecurseSetIKPartBits(const DObj *obj, int boneIndex, int *partBits)
{
  int newBoneIndex; // [esp+0h] [ebp-10h]
  unsigned int bit; // [esp+4h] [ebp-Ch]
  int highBoneIndex; // [esp+8h] [ebp-8h]

  bit = 0x80000000 >> (boneIndex & 0x1F);
  highBoneIndex = boneIndex >> 5;
  if ( (boneIndex >> 5 < 0 || highBoneIndex >= 5)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp",
          442,
          0,
          "%s",
          "highBoneIndex >= 0 && highBoneIndex < DOBJ_MAX_PART_BITS") )
  {
    __debugbreak();
  }
  if ( (bit & partBits[highBoneIndex]) == 0 )
  {
    partBits[highBoneIndex] |= bit;
    newBoneIndex = boneIndex - (*obj->localModels)->numRootBones;
    if ( newBoneIndex >= 0 )
      IKImport_RecurseSetIKPartBits(obj, boneIndex - (*obj->localModels)->localParentList[newBoneIndex], partBits);
  }
}

char __cdecl IKImport_CheckIKPartBits(IKState *ikState)
{
  unsigned int i; // [esp+4h] [ebp-4h]

  if ( !ikState->isIKModel )
    return 0;
  for ( i = 0; i < 6; ++i )
  {
    if ( ikEssentialBones[minBoneExtents[i]] && ikState->ikBoneToObjBone[minBoneExtents[i]] == 161 )
      return 0;
  }
  return 1;
}

void __cdecl IKImport_AccumulateParentObjBones(
        IKState *ikState,
        const unsigned __int8 *parentList,
        int numRootBones,
        float *transArray,
        int objBoneIndex,
        DObjAnimMat *matArray,
        DObjAnimMat *localMat)
{
  int v7; // eax
  float v8; // [esp+34h] [ebp-30h]
  float v9; // [esp+50h] [ebp-14h]
  float *trans; // [esp+58h] [ebp-Ch]
  float *transa; // [esp+58h] [ebp-Ch]
  int parentObjBoneIndex; // [esp+60h] [ebp-4h]

  if ( objBoneIndex >= numRootBones )
  {
    v7 = parentList[objBoneIndex - numRootBones];
    parentObjBoneIndex = objBoneIndex - v7;
    if ( objBoneIndex - v7 == objBoneIndex
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp",
            564,
            0,
            "%s\n\t(parentObjBoneIndex) = %i",
            "(parentObjBoneIndex != objBoneIndex)",
            parentObjBoneIndex) )
    {
      __debugbreak();
    }
    if ( ikState->objBoneToIKBone[parentObjBoneIndex] - 1 < 0 )
    {
      QuatMultiplyEquals(matArray[parentObjBoneIndex].quat, localMat->quat);
      v8 = Vec4LengthSq(localMat->quat);
      if ( v8 == 0.0 )
      {
        localMat->quat[3] = FLOAT_1_0;
        localMat->transWeight = FLOAT_2_0;
      }
      else
      {
        localMat->transWeight = 2.0 / v8;
      }
      transa = &transArray[3 * (objBoneIndex - numRootBones)];
      localMat->trans[0] = localMat->trans[0] + *transa;
      localMat->trans[1] = localMat->trans[1] + transa[1];
      localMat->trans[2] = localMat->trans[2] + transa[2];
      MatrixTransformVectorQuatTransEquals(&matArray[parentObjBoneIndex], localMat->trans);
      IKImport_AccumulateParentObjBones(
        ikState,
        parentList,
        numRootBones,
        transArray,
        parentObjBoneIndex,
        matArray,
        localMat);
    }
    else
    {
      v9 = Vec4LengthSq(localMat->quat);
      if ( v9 == 0.0 )
      {
        localMat->quat[3] = FLOAT_1_0;
        localMat->transWeight = FLOAT_2_0;
      }
      else
      {
        localMat->transWeight = 2.0 / v9;
      }
      trans = &transArray[3 * (objBoneIndex - numRootBones)];
      localMat->trans[0] = localMat->trans[0] + *trans;
      localMat->trans[1] = localMat->trans[1] + trans[1];
      localMat->trans[2] = localMat->trans[2] + trans[2];
    }
  }
}

void __cdecl IKImport_GetBoneMatrixArrayModelBones()
{
  if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 656, 0, "%s", "0") )
    __debugbreak();
}

void __cdecl IKImport_GetBoneMatrixArrayLocalBones(IKState *ikState)
{
  float v1; // [esp+28h] [ebp-4Ch]
  unsigned __int8 *obj; // [esp+3Ch] [ebp-38h]
  unsigned __int8 *boneLookup; // [esp+40h] [ebp-34h]
  int objBoneIndex; // [esp+44h] [ebp-30h]
  int objBoneIndexa; // [esp+44h] [ebp-30h]
  DObjAnimMat *thisMat; // [esp+48h] [ebp-2Ch]
  DObjAnimMat *thisMata; // [esp+48h] [ebp-2Ch]
  int i; // [esp+4Ch] [ebp-28h]
  DObjAnimMat *matArray; // [esp+50h] [ebp-24h]
  DObjAnimMat localMat; // [esp+54h] [ebp-20h] BYREF

  obj = ikState->model;
  if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 672, 0, "%s", "obj") )
    __debugbreak();
  matArray = (DObjAnimMat *)*((unsigned int *)obj + 21);
  if ( !matArray && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 675, 0, "%s", "matArray") )
    __debugbreak();
  ikState->rootOffset[0] = matArray->trans[0];
  ikState->rootOffset[1] = matArray->trans[1];
  ikState->rootOffset[2] = matArray->trans[2];
  matArray->trans[0] = *(float *)&FLOAT_0_0;
  matArray->trans[1] = *(float *)&FLOAT_0_0;
  matArray->trans[2] = *(float *)&FLOAT_0_0;
  ikState->rootQuat[0] = matArray->quat[0];
  ikState->rootQuat[1] = matArray->quat[1];
  ikState->rootQuat[2] = matArray->quat[2];
  ikState->rootQuat[3] = matArray->quat[3];
  matArray->quat[0] = *(float *)&FLOAT_0_0;
  matArray->quat[1] = *(float *)&FLOAT_0_0;
  matArray->quat[2] = *(float *)&FLOAT_0_0;
  matArray->quat[3] = FLOAT_1_0;
  thisMat = matArray;
  for ( objBoneIndex = 0; objBoneIndex < *(unsigned __int8 *)(**((unsigned int **)obj + 30) + 4); ++objBoneIndex )
  {
    if ( (*(unsigned int *)&obj[4 * (objBoneIndex >> 5) + 20] & (0x80000000 >> (objBoneIndex & 0x1F))) != 0 )
    {
      v1 = Vec4LengthSq(thisMat->quat);
      if ( v1 == 0.0 )
      {
        thisMat->quat[3] = FLOAT_1_0;
        thisMat->transWeight = FLOAT_2_0;
      }
      else
      {
        thisMat->transWeight = 2.0 / v1;
      }
    }
    ++thisMat;
  }
  i = 0;
  boneLookup = ikState->ikBoneToObjBone;
  while ( i < 23 )
  {
    objBoneIndexa = *boneLookup;
    if ( ikState->ikBoneToObjBone[i] != 161 )
    {
      thisMata = &matArray[objBoneIndexa];
      if ( ((LODWORD(thisMata->quat[0]) & 0x7F800000) == 0x7F800000
         || (LODWORD(thisMata->quat[1]) & 0x7F800000) == 0x7F800000
         || (LODWORD(thisMata->quat[2]) & 0x7F800000) == 0x7F800000
         || (LODWORD(thisMata->quat[3]) & 0x7F800000) == 0x7F800000)
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp",
              704,
              0,
              "%s",
              "!IS_NAN((thisMat->quat)[0]) && !IS_NAN((thisMat->quat)[1]) && !IS_NAN((thisMat->quat)[2]) && !IS_NAN((thisMat->quat)[3])") )
      {
        __debugbreak();
      }
      if ( ((LODWORD(thisMata->trans[0]) & 0x7F800000) == 0x7F800000
         || (LODWORD(thisMata->trans[1]) & 0x7F800000) == 0x7F800000
         || (LODWORD(thisMata->trans[2]) & 0x7F800000) == 0x7F800000)
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp",
              705,
              0,
              "%s",
              "!IS_NAN((thisMat->trans)[0]) && !IS_NAN((thisMat->trans)[1]) && !IS_NAN((thisMat->trans)[2])") )
      {
        __debugbreak();
      }
      memcpy(&localMat, thisMata, sizeof(localMat));
      IKImport_AccumulateParentObjBones(
        ikState,
        *(const unsigned __int8 **)(**((unsigned int **)obj + 30) + 12),
        *(unsigned __int8 *)(**((unsigned int **)obj + 30) + 5),
        *(float **)(**((unsigned int **)obj + 30) + 20),
        objBoneIndexa,
        matArray,
        &localMat);
      ikQuatTransToMatrix44(localMat.quat, localMat.trans, ikState->matArrayPreIK[i]);
      ikNormalizedMatrixAssert_func(ikState->matArrayPreIK[i]);
    }
    ++i;
    ++boneLookup;
  }
}

void __cdecl IKImport_GetBoneMatrixArray(IKState *ikState, bool isLocalBones)
{
  if ( !isLocalBones
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 725, 0, "%s", "isLocalBones") )
  {
    __debugbreak();
  }
  if ( isLocalBones )
    IKImport_GetBoneMatrixArrayLocalBones(ikState);
  else
    IKImport_GetBoneMatrixArrayModelBones();
}

void __cdecl IKImport_ApplyIKToSkeletonModelBones()
{
  if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 895, 0, "%s", "0") )
    __debugbreak();
}

// local variable allocation has failed, the output may be wrong!
void  IKImport_ApplyIKToSkeletonLocalBones(DObjAnimMat *a1@<ebp>, IKState *ikState)
{
  _BYTE v2[76]; // [esp-Ch] [ebp-24Ch] OVERLAPPED BYREF
  float v3; // [esp+40h] [ebp-200h]
  float v4; // [esp+44h] [ebp-1FCh]
  float v5; // [esp+48h] [ebp-1F8h]
  float v6; // [esp+4Ch] [ebp-1F4h]
  float v7; // [esp+50h] [ebp-1F0h]
  float v8; // [esp+54h] [ebp-1ECh]
  float v9; // [esp+58h] [ebp-1E8h]
  float v10; // [esp+5Ch] [ebp-1E4h]
  float v11; // [esp+60h] [ebp-1E0h]
  float v12; // [esp+64h] [ebp-1DCh]
  float v13; // [esp+68h] [ebp-1D8h]
  float v14; // [esp+6Ch] [ebp-1D4h]
  float v15; // [esp+70h] [ebp-1D0h]
  float v16; // [esp+74h] [ebp-1CCh]
  float v17; // [esp+78h] [ebp-1C8h]
  _BYTE *v18; // [esp+80h] [ebp-1C0h]
  _BYTE *v19; // [esp+84h] [ebp-1BCh]
  _BYTE *v20; // [esp+88h] [ebp-1B8h]
  __int64 v21; // [esp+8Ch] [ebp-1B4h]
  int v22; // [esp+94h] [ebp-1ACh]
  __int64 v23; // [esp+98h] [ebp-1A8h]
  __int64 v24; // [esp+A0h] [ebp-1A0h]
  __int64 v25; // [esp+A8h] [ebp-198h]
  __int64 v26; // [esp+B0h] [ebp-190h]
  __int64 v27; // [esp+B8h] [ebp-188h]
  __int64 v28; // [esp+C0h] [ebp-180h]
  int v29; // [esp+C8h] [ebp-178h]
  _BYTE *v30; // [esp+CCh] [ebp-174h]
  _BYTE *v31; // [esp+D0h] [ebp-170h]
  _BYTE *v32; // [esp+D4h] [ebp-16Ch]
  _BYTE *v33; // [esp+D8h] [ebp-168h]
  int v34; // [esp+DCh] [ebp-164h]
  int v35; // [esp+E0h] [ebp-160h]
  int v36; // [esp+E4h] [ebp-15Ch]
  __int64 v37; // [esp+E8h] [ebp-158h]
  __int64 v38; // [esp+F0h] [ebp-150h]
  __int64 v39; // [esp+F8h] [ebp-148h]
  __int64 v40; // [esp+100h] [ebp-140h]
  __int64 v41; // [esp+108h] [ebp-138h]
  __int64 v42; // [esp+110h] [ebp-130h]
  int v43; // [esp+118h] [ebp-128h]
  _BYTE *v44; // [esp+120h] [ebp-120h]
  _BYTE v45[140]; // [esp+124h] [ebp-11Ch] OVERLAPPED BYREF
  int v46; // [esp+1B0h] [ebp-90h]
  float v47; // [esp+1B4h] [ebp-8Ch]
  float v48; // [esp+1B8h] [ebp-88h]
  DObjAnimMat *v49; // [esp+1BCh] [ebp-84h]
  float *v50; // [esp+1C0h] [ebp-80h]
  float *trans; // [esp+1C4h] [ebp-7Ch]
  float *v52; // [esp+1C8h] [ebp-78h]
  float v53[3]; // [esp+1CCh] [ebp-74h] BYREF
  float axis[3][3]; // [esp+1D8h] [ebp-68h] BYREF
  DObjAnimMat *thisMat; // [esp+1FCh] [ebp-44h]
  unsigned __int8 *ikBoneToObjBone; // [esp+200h] [ebp-40h]
  int objBoneIndex; // [esp+204h] [ebp-3Ch]
  float (*thisIKMat)[4][4]; // [esp+208h] [ebp-38h] BYREF
  unsigned __int8 *boneLookup; // [esp+20Ch] [ebp-34h]
  int i; // [esp+210h] [ebp-30h]
  DObjAnimMat animMatXform; // [esp+214h] [ebp-2Ch] BYREF
  DObjAnimMat *matArray; // [esp+234h] [ebp-Ch]
  DObj *obj; // [esp+238h] [ebp-8h]
  DObj *retaddr; // [esp+240h] [ebp+0h]

  matArray = a1;
  obj = retaddr;
  LODWORD(animMatXform.trans[2]) = ikState->model;
  if ( !LODWORD(animMatXform.trans[2])
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 915, 0, "%s", "obj") )
  {
    __debugbreak();
  }
  animMatXform.trans[1] = *(float *)(LODWORD(animMatXform.trans[2]) + 84);
  if ( !LODWORD(animMatXform.trans[1])
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 918, 0, "%s", "matArray") )
  {
    __debugbreak();
  }
  memset(&animMatXform.quat[1], 0, 12);
  if ( ikState->bHasActiveLayers )
  {
    objBoneIndex = 0;
    ikBoneToObjBone = ikState->ikBoneToObjBone;
    thisMat = (DObjAnimMat *)ikState->matArrayXforms;
    while ( objBoneIndex < 23 )
    {
      LODWORD(axis[2][2]) = *ikBoneToObjBone;
      HIBYTE(axis[2][1]) = ikState->ikBoneToObjBone[objBoneIndex] != 161;
      if ( HIBYTE(axis[2][1]) && (ikState->modifiedIKBones & (1 << objBoneIndex)) != 0 )
      {
        LODWORD(axis[2][0]) = LODWORD(animMatXform.trans[1]) + 32 * LODWORD(axis[2][2]);
        if ( objBoneIndex > 0 )
        {
          v53[0] = thisMat->quat[0];
          v53[1] = thisMat->quat[1];
          v53[2] = thisMat->quat[2];
          v52 = axis[0];
          trans = thisMat->trans;
          axis[0][0] = thisMat->trans[0];
          axis[0][1] = thisMat->trans[1];
          axis[0][2] = thisMat->trans[2];
          v50 = axis[1];
          v49 = thisMat + 1;
          axis[1][0] = thisMat[1].quat[0];
          axis[1][1] = thisMat[1].quat[1];
          axis[1][2] = thisMat[1].quat[2];
          ikAxisToQuat((const float (*)[3])v53, (float *)&thisIKMat);
          v48 = Vec4LengthSq((const float *)&thisIKMat);
          if ( v48 == 0.0 )
          {
            animMatXform.quat[0] = FLOAT_1_0;
            animMatXform.trans[0] = FLOAT_2_0;
          }
          else
          {
            animMatXform.trans[0] = 2.0 / v48;
          }
          QuatMultiplyEquals((const float *)LODWORD(axis[2][0]), (float *)&thisIKMat);
          *(unsigned int *)LODWORD(axis[2][0]) = thisIKMat;
          *(unsigned int *)(LODWORD(axis[2][0]) + 4) = boneLookup;
          *(unsigned int *)(LODWORD(axis[2][0]) + 8) = i;
          *(float *)(LODWORD(axis[2][0]) + 12) = animMatXform.quat[0];
          v47 = Vec4LengthSq((const float *)LODWORD(axis[2][0]));
          if ( v47 == 0.0 )
          {
            *(float *)(LODWORD(axis[2][0]) + 12) = FLOAT_1_0;
            *(float *)(LODWORD(axis[2][0]) + 28) = FLOAT_2_0;
          }
          else
          {
            *(float *)(LODWORD(axis[2][0]) + 28) = 2.0 / v47;
          }
        }
        v46 = LODWORD(axis[2][0]) + 16;
        *(unsigned int *)&v45[136] = (char *)thisMat + 48;
        *(unsigned int *)&v45[132] = LODWORD(axis[2][0]) + 16;
        *(float *)(LODWORD(axis[2][0]) + 16) = *(float *)(LODWORD(axis[2][0]) + 16) + thisMat[1].trans[0];
        *(float *)(v46 + 4) = *(float *)(*(unsigned int *)&v45[132] + 4) + *(float *)(*(unsigned int *)&v45[136] + 4);
        *(float *)(v46 + 8) = *(float *)(*(unsigned int *)&v45[132] + 8) + *(float *)(*(unsigned int *)&v45[136] + 8);
      }
      ++objBoneIndex;
      ++ikBoneToObjBone;
      thisMat += 2;
    }
  }
  ikQuatTransToMatrix44(ikState->rootQuat, ikState->rootOffset, (float (*)[4])&v45[64]);
  ikQuatTransToMatrix44(
    (float *)LODWORD(animMatXform.trans[1]),
    (const float *)(LODWORD(animMatXform.trans[1]) + 16),
    (float (*)[4])v45);
  v44 = v45;
  v34 = *(unsigned int *)v45;
  v35 = *(unsigned int *)&v45[4];
  v36 = *(unsigned int *)&v45[8];
  v37 = *(_QWORD *)&v45[12];
  v33 = &v45[16];
  v38 = *(_QWORD *)&v45[20];
  v39 = *(_QWORD *)&v45[28];
  v32 = &v45[32];
  v40 = *(_QWORD *)&v45[36];
  v41 = *(_QWORD *)&v45[44];
  v31 = &v45[48];
  v42 = *(_QWORD *)&v45[52];
  v43 = *(unsigned int *)&v45[60];
  v30 = &v45[64];
  v21 = *(_QWORD *)&v45[64];
  v22 = *(unsigned int *)&v45[72];
  v23 = *(_QWORD *)&v45[76];
  v20 = &v45[80];
  v24 = *(_QWORD *)&v45[84];
  v25 = *(_QWORD *)&v45[92];
  v19 = &v45[96];
  v26 = *(_QWORD *)&v45[100];
  v27 = *(_QWORD *)&v45[108];
  v18 = &v45[112];
  v28 = *(_QWORD *)&v45[116];
  v29 = *(unsigned int *)&v45[124];
  *(float *)&v2[72] = (float)((float)((float)(*(float *)v45 * *(float *)&v45[64])
                                    + (float)(*(float *)&v45[4] * *(float *)&v45[80]))
                            + (float)(*(float *)&v45[8] * *(float *)&v45[96]))
                    + (float)(*(float *)&v45[12] * *(float *)&v45[112]);
  v3 = (float)((float)((float)(*(float *)v45 * *(float *)&v45[68]) + (float)(*(float *)&v45[4] * *(float *)&v45[84]))
             + (float)(*(float *)&v45[8] * *(float *)&v45[100]))
     + (float)(*(float *)&v45[12] * *(float *)&v45[116]);
  v4 = (float)((float)((float)(*(float *)v45 * *(float *)&v45[72]) + (float)(*(float *)&v45[4] * *(float *)&v45[88]))
             + (float)(*(float *)&v45[8] * *(float *)&v45[104]))
     + (float)(*(float *)&v45[12] * *(float *)&v45[120]);
  v5 = (float)((float)((float)(*(float *)v45 * *(float *)&v45[76]) + (float)(*(float *)&v45[4] * *(float *)&v45[92]))
             + (float)(*(float *)&v45[8] * *(float *)&v45[108]))
     + (float)(*(float *)&v45[12] * *(float *)&v45[124]);
  v6 = (float)((float)((float)(*(float *)&v45[16] * *(float *)&v45[64])
                     + (float)(*(float *)&v45[20] * *(float *)&v45[80]))
             + (float)(*(float *)&v45[24] * *(float *)&v45[96]))
     + (float)(*(float *)&v45[28] * *(float *)&v45[112]);
  v7 = (float)((float)((float)(*(float *)&v45[16] * *(float *)&v45[68])
                     + (float)(*(float *)&v45[20] * *(float *)&v45[84]))
             + (float)(*(float *)&v45[24] * *(float *)&v45[100]))
     + (float)(*(float *)&v45[28] * *(float *)&v45[116]);
  v8 = (float)((float)((float)(*(float *)&v45[16] * *(float *)&v45[72])
                     + (float)(*(float *)&v45[20] * *(float *)&v45[88]))
             + (float)(*(float *)&v45[24] * *(float *)&v45[104]))
     + (float)(*(float *)&v45[28] * *(float *)&v45[120]);
  v9 = (float)((float)((float)(*(float *)&v45[16] * *(float *)&v45[76])
                     + (float)(*(float *)&v45[20] * *(float *)&v45[92]))
             + (float)(*(float *)&v45[24] * *(float *)&v45[108]))
     + (float)(*(float *)&v45[28] * *(float *)&v45[124]);
  v10 = (float)((float)((float)(*(float *)&v45[32] * *(float *)&v45[64])
                      + (float)(*(float *)&v45[36] * *(float *)&v45[80]))
              + (float)(*(float *)&v45[40] * *(float *)&v45[96]))
      + (float)(*(float *)&v45[44] * *(float *)&v45[112]);
  v11 = (float)((float)((float)(*(float *)&v45[32] * *(float *)&v45[68])
                      + (float)(*(float *)&v45[36] * *(float *)&v45[84]))
              + (float)(*(float *)&v45[40] * *(float *)&v45[100]))
      + (float)(*(float *)&v45[44] * *(float *)&v45[116]);
  v12 = (float)((float)((float)(*(float *)&v45[32] * *(float *)&v45[72])
                      + (float)(*(float *)&v45[36] * *(float *)&v45[88]))
              + (float)(*(float *)&v45[40] * *(float *)&v45[104]))
      + (float)(*(float *)&v45[44] * *(float *)&v45[120]);
  v13 = (float)((float)((float)(*(float *)&v45[32] * *(float *)&v45[76])
                      + (float)(*(float *)&v45[36] * *(float *)&v45[92]))
              + (float)(*(float *)&v45[40] * *(float *)&v45[108]))
      + (float)(*(float *)&v45[44] * *(float *)&v45[124]);
  v14 = (float)((float)((float)(*(float *)&v45[48] * *(float *)&v45[64])
                      + (float)(*(float *)&v45[52] * *(float *)&v45[80]))
              + (float)(*(float *)&v45[56] * *(float *)&v45[96]))
      + (float)(*(float *)&v45[60] * *(float *)&v45[112]);
  v15 = (float)((float)((float)(*(float *)&v45[48] * *(float *)&v45[68])
                      + (float)(*(float *)&v45[52] * *(float *)&v45[84]))
              + (float)(*(float *)&v45[56] * *(float *)&v45[100]))
      + (float)(*(float *)&v45[60] * *(float *)&v45[116]);
  v16 = (float)((float)((float)(*(float *)&v45[48] * *(float *)&v45[72])
                      + (float)(*(float *)&v45[52] * *(float *)&v45[88]))
              + (float)(*(float *)&v45[56] * *(float *)&v45[104]))
      + (float)(*(float *)&v45[60] * *(float *)&v45[120]);
  v17 = (float)((float)((float)(*(float *)&v45[48] * *(float *)&v45[76])
                      + (float)(*(float *)&v45[52] * *(float *)&v45[92]))
              + (float)(*(float *)&v45[56] * *(float *)&v45[108]))
      + (float)(*(float *)&v45[60] * *(float *)&v45[124]);
  *(float *)v2 = *(float *)&v2[72];
  *(float *)&v2[4] = v3;
  *(float *)&v2[8] = v4;
  *(float *)&v2[12] = v5;
  *(float *)&v2[16] = v6;
  *(float *)&v2[20] = v7;
  *(float *)&v2[24] = v8;
  *(float *)&v2[28] = v9;
  *(float *)&v2[32] = v10;
  *(float *)&v2[36] = v11;
  *(float *)&v2[40] = v12;
  *(float *)&v2[44] = v13;
  *(float *)&v2[48] = v14;
  *(float *)&v2[52] = v15;
  *(float *)&v2[56] = v16;
  *(float *)&v2[60] = v17;
  ikMatrix44ToQuatTrans(
    (float (*)[4])v2,
    (float *)LODWORD(animMatXform.trans[1]),
    (float *)(LODWORD(animMatXform.trans[1]) + 16));
}

void __cdecl IKImport_ApplyIKToSkeleton(IKState *ikState, bool isLocalBones)
{
  int savedregs; // [esp+0h] [ebp+0h] BYREF

  if ( isLocalBones )
    IKImport_ApplyIKToSkeletonLocalBones((DObjAnimMat *)&savedregs, ikState);
  else
    IKImport_ApplyIKToSkeletonModelBones();
}

IKState *__cdecl IKImport_FetchDObjIKState(const unsigned __int8 *model)
{
  if ( !model && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 1017, 0, "%s", "obj") )
    __debugbreak();
  return (IKState *)*((unsigned int *)model + 29);
}

void __cdecl IKImport_SetDObjIKState(DObj *model, IKState *ikState)
{
  if ( !model && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 1041, 0, "%s", "obj") )
    __debugbreak();
  DObjSetIKState(model, (unsigned __int8 *)ikState);
}

bool __cdecl IKImport_IsIKEntity_Internal(unsigned __int8 *model)
{
  int LocalClientIndex; // eax
  centity_s *cent; // [esp+10h] [ebp-14h]
  gentity_s *ent; // [esp+14h] [ebp-10h]
  int entityNum; // [esp+1Ch] [ebp-8h]
  IKState *ikState; // [esp+20h] [ebp-4h]

  if ( !model && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 1105, 0, "%s", "obj") )
    __debugbreak();
  entityNum = *((unsigned __int16 *)model + 3) - 1;
  if ( entityNum > 1023 )
    return 0;
  ikState = 0;
  if ( !ikSystem.bInitialized )
    IK_InitSystem();
  if ( IKImport_IsValidEntityType(model) )
  {
    ikState = IK_GetIKStateForEntity(entityNum, model, 0);
    if ( ikState )
    {
      if ( ikState->lastIsIKEntityFrameNum == IKImport_GetFrameNum() )
        return DObjHasFlag((const DObj *)model, 8u);
      ikState->lastIsIKEntityFrameNum = IKImport_GetFrameNum();
    }
  }
  DObjSetFlag((DObj *)model, 8u, 0);
  if ( !IKImport_GetVar_IK_Enable() )
    return 0;
  if ( !dword_9C3D758[DObjGetLocalClientIndex((const DObj *)model)] )
    return 0;
  if ( ikState && !ikState->isIKModel )
    return 0;
  if ( !*((unsigned int *)model + 30) || !model[9] || !**((unsigned int **)model + 30) )
    return 0;
  if ( entityNum < 0 )
    return 0;
  if ( DObjIsServer((const DObj *)model) )
  {
    ent = &g_entities[entityNum];
    if ( ent->s.eType != 1 )
      return 0;
    if ( ent->client && ent->client->sess.connected != CON_CONNECTED )
      return 0;
  }
  else
  {
    if ( ikState )
    {
      cent = CG_GetEntity(ikState->localClientIndex, entityNum);
    }
    else
    {
      LocalClientIndex = DObjGetLocalClientIndex((const DObj *)model);
      cent = CG_GetEntity(LocalClientIndex, entityNum);
    }
    if ( !cent )
      return 0;
    if ( ((*((unsigned int *)cent + 201) >> 1) & 1) == 0 )
      return 0;
    if ( cent->pose.isRagdoll )
      return 0;
    if ( (cent->nextState.lerp.eFlags & 0x40000) != 0 )
      return 0;
    if ( cent->nextState.eType != 1 )
      return 0;
  }
  if ( !ikState )
  {
    ikState = IK_GetIKStateForEntity(entityNum, model, 1);
    if ( !ikState
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp",
            1246,
            0,
            "%s\n\t%s",
            "ikState",
            "IKImport_IsIKEntity(): no free IK slots. This is fatal.") )
    {
      __debugbreak();
    }
  }
  if ( !ikState || !ikState->isIKModel )
    return 0;
  IK_UpdateLayerLerps(ikState);
  if ( !ikState->bHasActiveLayers )
    return 0;
  IK_UpdateStatusFunctions(ikState);
  DObjSetFlag((DObj *)model, 8u, 1);
  return 1;
}

bool __cdecl IKImport_IsIKEntity(unsigned __int8 *model)
{
  bool IsIKEntity_Internal; // [esp+9h] [ebp-3h]

  //PIXBeginNamedEvent(-1, "IKImport_IsIKEntity");
  IsIKEntity_Internal = IKImport_IsIKEntity_Internal(model);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  return IsIKEntity_Internal;
}

bool __cdecl IKImport_BypassTerrainMapping(IKState *ikState)
{
  unsigned __int8 *obj; // [esp+0h] [ebp-8h]
  int entityNum; // [esp+4h] [ebp-4h]

  obj = ikState->model;
  if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 1300, 0, "%s", "obj") )
    __debugbreak();
  entityNum = *((unsigned __int16 *)obj + 3) - 1;
  if ( (unsigned int)entityNum >= 0x400 )
    return 1;
  if ( entityNum < 32 )
    return !ikSystem.dvars[8]->current.enabled;
  if ( ikSystem.dvars[10]->current.integer )
    return IKImport_GetIKPriority() + 1 <= ikSystem.dvars[10]->current.integer;
  return 1;
}

char __cdecl IKImport_ApplyTerrainMapping(IKState *ikState)
{
  int LocalClientIndex; // eax
  int v3; // eax
  int groundEntityNum; // [esp-4h] [ebp-34h]
  int v5; // [esp+0h] [ebp-30h]
  int eType; // [esp+4h] [ebp-2Ch]
  centity_s *centGround; // [esp+18h] [ebp-18h]
  centity_s *cent; // [esp+1Ch] [ebp-14h]
  gentity_s *groundEnt; // [esp+20h] [ebp-10h]
  gentity_s *ent; // [esp+24h] [ebp-Ch]
  unsigned __int8 *obj; // [esp+28h] [ebp-8h]
  unsigned int entityNum; // [esp+2Ch] [ebp-4h]

  obj = ikState->model;
  if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 1336, 0, "%s", "obj") )
    __debugbreak();
  entityNum = *((unsigned __int16 *)obj + 3) - 1;
  if ( entityNum >= 0x400 )
    return 0;
  if ( ikState->isServer )
  {
    ent = &g_entities[entityNum];
    if ( (ent->s.lerp.eFlags & 0x44000) != 0 )
      return 0;
    if ( ent->s.groundEntityNum == 1023 )
    {
      if ( (ent->s.lerp.eFlags & 0x300) == 0 )
        return 0;
    }
    else if ( ent->s.groundEntityNum != 1022 )
    {
      groundEnt = &g_entities[ent->s.groundEntityNum];
      if ( !groundEnt->active )
        return 0;
      eType = groundEnt->s.eType;
      if ( eType == 6 || eType == 14 )
        return 0;
    }
  }
  else
  {
    LocalClientIndex = IKImport_GetLocalClientIndex(ikState);
    cent = CG_GetEntity(LocalClientIndex, entityNum);
    if ( ((*((unsigned int *)cent + 201) >> 1) & 1) == 0 )
      return 0;
    if ( (cent->nextState.lerp.eFlags & 0x4C000) != 0 )
      return 0;
    if ( cent->nextState.groundEntityNum == 1023 )
    {
      if ( (cent->nextState.lerp.eFlags & 0x300) == 0 )
        return 0;
    }
    else if ( cent->nextState.groundEntityNum != 1022 )
    {
      groundEntityNum = cent->nextState.groundEntityNum;
      v3 = IKImport_GetLocalClientIndex(ikState);
      centGround = CG_GetEntity(v3, groundEntityNum);
      if ( ((*((unsigned int *)centGround + 201) >> 1) & 1) == 0 )
        return 0;
      v5 = centGround->nextState.eType;
      if ( v5 == 6 || v5 == 14 )
        return 0;
    }
  }
  return 1;
}

bool __cdecl IKImport_ApplyLeftHandIK(IKState *ikState)
{
  unsigned __int8 *obj; // [esp+8h] [ebp-Ch]
  int entityNum; // [esp+Ch] [ebp-8h]
  centity_s *cent; // [esp+10h] [ebp-4h]

  obj = ikState->model;
  if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 1433, 0, "%s", "obj") )
    __debugbreak();
  entityNum = *((unsigned __int16 *)obj + 3) - 1;
  if ( (unsigned int)entityNum >= 0x400 )
    return 0;
  if ( ikState->isServer )
    return 0;
  if ( (char)obj[112] == -1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp",
          1443,
          0,
          "%s",
          "obj->localClientIndex != INVALID_CLIENT_NUMBER") )
  {
    __debugbreak();
  }
  cent = CG_GetEntity((char)obj[112], entityNum);
  if ( ((*((unsigned int *)cent + 201) >> 1) & 1) == 0 )
    return 0;
  if ( IKImport_GetVar_IK_Left_Hand_Lerp_Test() )
    return 1;
  if ( entityNum >= 32 )
  {
    if ( !ikState->dvarValues[9].enabled )
      return 0;
  }
  else if ( !ikState->dvarValues[7].enabled )
  {
    return 0;
  }
  return (cent->nextState.lerp.eFlags & 0x40000) == 0 && (*((unsigned int *)cent + 201) & 1) != 0;
}

bool __cdecl IKImport_ApplyRightHandIK(IKState *ikState)
{
  unsigned __int8 lookup; // [esp+3h] [ebp-Dh] BYREF
  DObj *obj; // [esp+4h] [ebp-Ch]
  int result; // [esp+8h] [ebp-8h]
  int entityNum; // [esp+Ch] [ebp-4h]

  obj = (DObj *)ikState->model;
  if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 1494, 0, "%s", "obj") )
    __debugbreak();
  entityNum = obj->entnum - 1;
  if ( (unsigned int)entityNum >= 0x400 )
    return 0;
  if ( IKImport_GetVar_IK_Right_Hand_Lerp_Test() )
    return ikState->timeMS % 3000 < 1500;
  if ( entityNum >= 32 )
  {
    if ( !ikState->dvarValues[9].enabled )
      return 0;
  }
  else if ( !ikState->dvarValues[7].enabled )
  {
    return 0;
  }
  lookup = (*obj->localModels)->numBones;
  result = DObjGetBoneIndex(obj, ikImportMiscBoneStringVars[1], &lookup, -1);
  return result != 0;
}

// local variable allocation has failed, the output may be wrong!
void  IKImport_GetEntityXform(gentity_s *a1@<ebp>, IKState *ikState)
{
  int v2; // eax
  int LocalClientIndex; // eax
  int v4; // eax
  int entityNum; // edx
  int angle; // [esp+0h] [ebp-200h]
  __int64 v7; // [esp+18h] [ebp-1E8h]
  float v8; // [esp+20h] [ebp-1E0h]
  __int64 v9; // [esp+24h] [ebp-1DCh]
  __int64 v10; // [esp+2Ch] [ebp-1D4h]
  __int64 v11; // [esp+34h] [ebp-1CCh]
  __int64 v12; // [esp+3Ch] [ebp-1C4h]
  __int64 v13; // [esp+44h] [ebp-1BCh]
  __int64 v14; // [esp+4Ch] [ebp-1B4h]
  __int64 v15; // [esp+64h] [ebp-19Ch]
  __int64 v16; // [esp+6Ch] [ebp-194h]
  __int64 v17; // [esp+74h] [ebp-18Ch]
  __int64 v18; // [esp+7Ch] [ebp-184h]
  __int64 v19; // [esp+84h] [ebp-17Ch]
  __int64 v20; // [esp+8Ch] [ebp-174h]
  __int64 v21; // [esp+94h] [ebp-16Ch]
  __int64 v22; // [esp+9Ch] [ebp-164h]
  _BYTE v23[64]; // [esp+104h] [ebp-FCh] OVERLAPPED BYREF
  float *v24; // [esp+150h] [ebp-B0h]
  float *v25; // [esp+154h] [ebp-ACh]
  float *v26; // [esp+158h] [ebp-A8h]
  float *v27; // [esp+15Ch] [ebp-A4h]
  float *v28; // [esp+160h] [ebp-A0h]
  float *v29; // [esp+164h] [ebp-9Ch]
  float v30[3]; // [esp+168h] [ebp-98h] BYREF
  float axis[3][3]; // [esp+174h] [ebp-8Ch] BYREF
  float *v32; // [esp+198h] [ebp-68h]
  int v33; // [esp+19Ch] [ebp-64h]
  float *v34; // [esp+1A0h] [ebp-60h]
  float *origin; // [esp+1A4h] [ebp-5Ch]
  clientInfo_t *ci; // [esp+1A8h] [ebp-58h]
  centity_s *Entity; // [esp+1ACh] [ebp-54h]
  float *v38; // [esp+1B0h] [ebp-50h]
  float *v39; // [esp+1B4h] [ebp-4Ch]
  centity_s *cent; // [esp+1B8h] [ebp-48h]
  float *v41; // [esp+1BCh] [ebp-44h]
  float *v42; // [esp+1C0h] [ebp-40h]
  cg_s *LocalClientGlobals; // [esp+1C4h] [ebp-3Ch]
  float *viewangles; // [esp+1C8h] [ebp-38h]
  float *v45; // [esp+1CCh] [ebp-34h]
  cg_s *cgameGlob; // [esp+1D0h] [ebp-30h] BYREF
  float v47; // [esp+1D4h] [ebp-2Ch]
  float v48; // [esp+1D8h] [ebp-28h]
  float angles[3]; // [esp+1DCh] [ebp-24h]
  gentity_s *v50; // [esp+1E8h] [ebp-18h]
  char v51; // [esp+1EFh] [ebp-11h]
  gentity_s *ent; // [esp+1F4h] [ebp-Ch]
  void *v53; // [esp+1F8h] [ebp-8h] OVERLAPPED
  void *retaddr; // [esp+200h] [ebp+0h]

  ent = a1;
  v53 = retaddr;
  if ( ikState->entityNum >= 0x400u
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp",
          1537,
          0,
          "%s",
          "ikState->entityNum >= 0 && ikState->entityNum < MAX_GENTITIES") )
  {
    __debugbreak();
  }
  v51 = 0;
  if ( ikState->isServer )
  {
    v50 = &g_entities[ikState->entityNum];
    if ( !v50->r.inuse )
      return;
    LODWORD(angles[2]) = ikState->entityXform[3];
    LODWORD(angles[1]) = v50->r.currentOrigin;
    ikState->entityXform[3][0] = v50->r.currentOrigin[0];
    *(float *)(LODWORD(angles[2]) + 4) = *(float *)(LODWORD(angles[1]) + 4);
    *(float *)(LODWORD(angles[2]) + 8) = *(float *)(LODWORD(angles[1]) + 8);
    LODWORD(angles[0]) = v50->r.currentAngles;
    cgameGlob = (cg_s *)LODWORD(v50->r.currentAngles[0]);
    v47 = v50->r.currentAngles[1];
    v48 = v50->r.currentAngles[2];
    if ( v50->client )
    {
      v45 = ikState->angles;
      viewangles = v50->client->ps.viewangles;
      ikState->angles[0] = *viewangles;
      v45[1] = viewangles[1];
      v45[2] = viewangles[2];
    }
    v51 = 1;
LABEL_16:
    if ( !v51 )
    {
      angle = ikState->entityNum;
      LocalClientIndex = IKImport_GetLocalClientIndex(ikState);
      Entity = CG_GetEntity(LocalClientIndex, angle);
      if ( !Entity )
      {
        if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 1585, 0, "%s", "cent") )
          __debugbreak();
        return;
      }
      ci = (clientInfo_t *)ikState->entityXform[3];
      origin = Entity->pose.origin;
      ikState->entityXform[3][0] = Entity->pose.origin[0];
      ci->nextValid = (int)origin[1];
      ci->clientNum = (int)origin[2];
      v34 = Entity->pose.angles;
      cgameGlob = (cg_s *)LODWORD(Entity->pose.angles[0]);
      v47 = Entity->pose.angles[1];
      v48 = Entity->pose.angles[2];
      if ( ikState->entityNum < 32 )
      {
        v4 = IKImport_GetLocalClientIndex(ikState);
        LocalClientGlobals = CG_GetLocalClientGlobals(v4);
        if ( !LocalClientGlobals
          && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 1596, 0, "%s", "cgameGlob") )
        {
          __debugbreak();
        }
        if ( LocalClientGlobals )
        {
          entityNum = ikState->entityNum;
          v33 = (int)&LocalClientGlobals->bgs.clientinfo[entityNum];
          if ( (cg_s *)((char *)LocalClientGlobals + entityNum * 1480) == (cg_s *)-389096
            && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 1601, 0, "%s", "ci") )
          {
            __debugbreak();
          }
          if ( v33 && *(unsigned int *)v33 )
          {
            v32 = ikState->angles;
            LODWORD(axis[2][2]) = v33 + 1100;
            ikState->angles[0] = *(float *)(v33 + 1100);
            v32[1] = *(float *)(LODWORD(axis[2][2]) + 4);
            v32[2] = *(float *)(LODWORD(axis[2][2]) + 8);
          }
        }
      }
    }
    ikState->angles[0] = AngleNormalize180(ikState->angles[0]);
    LODWORD(axis[2][1]) = ikState->origin;
    LODWORD(axis[2][0]) = ikState->entityXform[3];
    ikState->origin[0] = ikState->entityXform[3][0];
    *(float *)(LODWORD(axis[2][1]) + 4) = *(float *)(LODWORD(axis[2][0]) + 4);
    *(float *)(LODWORD(axis[2][1]) + 8) = *(float *)(LODWORD(axis[2][0]) + 8);
    cgameGlob = *(cg_s **)&FLOAT_0_0;
    v48 = *(float *)&FLOAT_0_0;
    AnglesToAxis((const float *)&cgameGlob, (float (*)[3])v30);
    v29 = ikState->entityXform[0];
    ikState->entityXform[0][0] = v30[0];
    v29[1] = v30[1];
    v29[2] = v30[2];
    v28 = ikState->entityXform[1];
    v27 = axis[0];
    ikState->entityXform[1][0] = axis[0][0];
    v28[1] = v27[1];
    v28[2] = v27[2];
    v26 = ikState->entityXform[2];
    v25 = axis[1];
    ikState->entityXform[2][0] = axis[1][0];
    v26[1] = v25[1];
    v26[2] = v25[2];
    v24 = ikState->entityXform[0];
    ikState->entityXform[0][3] = *(float *)&FLOAT_0_0;
    v24[7] = *(float *)&FLOAT_0_0;
    v24[11] = *(float *)&FLOAT_0_0;
    v24[15] = FLOAT_1_0;
    AnglesToAxis(ikState->controller_origin_angles, (float (*)[3])v30);
    ikMatrixSet44((float (*)[4])v23, vec3_origin, (const float (*)[3])v30, 1.0);
    v15 = *(_QWORD *)&ikState->entityXform[0][0];
    v16 = *(_QWORD *)&ikState->entityXform[0][2];
    v17 = *(_QWORD *)&ikState->entityXform[1][0];
    v18 = *(_QWORD *)&ikState->entityXform[1][2];
    v19 = *(_QWORD *)&ikState->entityXform[2][0];
    v20 = *(_QWORD *)&ikState->entityXform[2][2];
    v21 = *(_QWORD *)&ikState->entityXform[3][0];
    v22 = *(_QWORD *)&ikState->entityXform[3][2];
    *(float *)&v7 = (float)((float)((float)(*(float *)v23 * *((float *)&v15 + 1))
                                  + (float)(*(float *)&v23[4] * *((float *)&v17 + 1)))
                          + (float)(*(float *)&v23[8] * *((float *)&v19 + 1)))
                  + (float)(*(float *)&v23[12] * *((float *)&v21 + 1));
    *((float *)&v7 + 1) = (float)((float)((float)(*(float *)v23 * *(float *)&v16)
                                        + (float)(*(float *)&v23[4] * *(float *)&v18))
                                + (float)(*(float *)&v23[8] * *(float *)&v20))
                        + (float)(*(float *)&v23[12] * *(float *)&v22);
    v8 = (float)((float)((float)(*(float *)v23 * *((float *)&v16 + 1))
                       + (float)(*(float *)&v23[4] * *((float *)&v18 + 1)))
               + (float)(*(float *)&v23[8] * *((float *)&v20 + 1)))
       + (float)(*(float *)&v23[12] * *((float *)&v22 + 1));
    *(float *)&v9 = (float)((float)((float)(*(float *)&v23[16] * *(float *)&v15)
                                  + (float)(*(float *)&v23[20] * *(float *)&v17))
                          + (float)(*(float *)&v23[24] * *(float *)&v19))
                  + (float)(*(float *)&v23[28] * *(float *)&v21);
    *((float *)&v9 + 1) = (float)((float)((float)(*(float *)&v23[16] * *((float *)&v15 + 1))
                                        + (float)(*(float *)&v23[20] * *((float *)&v17 + 1)))
                                + (float)(*(float *)&v23[24] * *((float *)&v19 + 1)))
                        + (float)(*(float *)&v23[28] * *((float *)&v21 + 1));
    *(float *)&v10 = (float)((float)((float)(*(float *)&v23[16] * *(float *)&v16)
                                   + (float)(*(float *)&v23[20] * *(float *)&v18))
                           + (float)(*(float *)&v23[24] * *(float *)&v20))
                   + (float)(*(float *)&v23[28] * *(float *)&v22);
    *((float *)&v10 + 1) = (float)((float)((float)(*(float *)&v23[16] * *((float *)&v16 + 1))
                                         + (float)(*(float *)&v23[20] * *((float *)&v18 + 1)))
                                 + (float)(*(float *)&v23[24] * *((float *)&v20 + 1)))
                         + (float)(*(float *)&v23[28] * *((float *)&v22 + 1));
    *(float *)&v11 = (float)((float)((float)(*(float *)&v23[32] * *(float *)&v15)
                                   + (float)(*(float *)&v23[36] * *(float *)&v17))
                           + (float)(*(float *)&v23[40] * *(float *)&v19))
                   + (float)(*(float *)&v23[44] * *(float *)&v21);
    *((float *)&v11 + 1) = (float)((float)((float)(*(float *)&v23[32] * *((float *)&v15 + 1))
                                         + (float)(*(float *)&v23[36] * *((float *)&v17 + 1)))
                                 + (float)(*(float *)&v23[40] * *((float *)&v19 + 1)))
                         + (float)(*(float *)&v23[44] * *((float *)&v21 + 1));
    *(float *)&v12 = (float)((float)((float)(*(float *)&v23[32] * *(float *)&v16)
                                   + (float)(*(float *)&v23[36] * *(float *)&v18))
                           + (float)(*(float *)&v23[40] * *(float *)&v20))
                   + (float)(*(float *)&v23[44] * *(float *)&v22);
    *((float *)&v12 + 1) = (float)((float)((float)(*(float *)&v23[32] * *((float *)&v16 + 1))
                                         + (float)(*(float *)&v23[36] * *((float *)&v18 + 1)))
                                 + (float)(*(float *)&v23[40] * *((float *)&v20 + 1)))
                         + (float)(*(float *)&v23[44] * *((float *)&v22 + 1));
    *(float *)&v13 = (float)((float)((float)(*(float *)&v23[48] * *(float *)&v15)
                                   + (float)(*(float *)&v23[52] * *(float *)&v17))
                           + (float)(*(float *)&v23[56] * *(float *)&v19))
                   + (float)(*(float *)&v23[60] * *(float *)&v21);
    *((float *)&v13 + 1) = (float)((float)((float)(*(float *)&v23[48] * *((float *)&v15 + 1))
                                         + (float)(*(float *)&v23[52] * *((float *)&v17 + 1)))
                                 + (float)(*(float *)&v23[56] * *((float *)&v19 + 1)))
                         + (float)(*(float *)&v23[60] * *((float *)&v21 + 1));
    *(float *)&v14 = (float)((float)((float)(*(float *)&v23[48] * *(float *)&v16)
                                   + (float)(*(float *)&v23[52] * *(float *)&v18))
                           + (float)(*(float *)&v23[56] * *(float *)&v20))
                   + (float)(*(float *)&v23[60] * *(float *)&v22);
    *((float *)&v14 + 1) = (float)((float)((float)(*(float *)&v23[48] * *((float *)&v16 + 1))
                                         + (float)(*(float *)&v23[52] * *((float *)&v18 + 1)))
                                 + (float)(*(float *)&v23[56] * *((float *)&v20 + 1)))
                         + (float)(*(float *)&v23[60] * *((float *)&v22 + 1));
    ikState->entityLegsXform[0][0] = (float)((float)((float)(*(float *)v23 * *(float *)&v15)
                                                   + (float)(*(float *)&v23[4] * *(float *)&v17))
                                           + (float)(*(float *)&v23[8] * *(float *)&v19))
                                   + (float)(*(float *)&v23[12] * *(float *)&v21);
    *(_QWORD *)&ikState->entityLegsXform[0][1] = v7;
    ikState->entityLegsXform[0][3] = v8;
    *(_QWORD *)&ikState->entityLegsXform[1][0] = v9;
    *(_QWORD *)&ikState->entityLegsXform[1][2] = v10;
    *(_QWORD *)&ikState->entityLegsXform[2][0] = v11;
    *(_QWORD *)&ikState->entityLegsXform[2][2] = v12;
    *(_QWORD *)&ikState->entityLegsXform[3][0] = v13;
    *(_QWORD *)&ikState->entityLegsXform[3][2] = v14;
    return;
  }
  if ( !IKImport_IsLocalClient(ikState) )
    goto LABEL_16;
  v2 = IKImport_GetLocalClientIndex(ikState);
  LocalClientGlobals = CG_GetLocalClientGlobals(v2);
  if ( !LocalClientGlobals
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 1567, 0, "%s", "cgameGlob") )
  {
    __debugbreak();
  }
  if ( LocalClientGlobals )
  {
    if ( LocalClientGlobals->clientNum == ikState->entityNum )
    {
      v42 = LocalClientGlobals->predictedPlayerEntity.pose.angles;
      cgameGlob = (cg_s *)LODWORD(LocalClientGlobals->predictedPlayerEntity.pose.angles[0]);
      v47 = LocalClientGlobals->predictedPlayerEntity.pose.angles[1];
      v48 = LocalClientGlobals->predictedPlayerEntity.pose.angles[2];
      v41 = ikState->entityXform[3];
      cent = (centity_s *)LocalClientGlobals->predictedPlayerEntity.pose.origin;
      ikState->entityXform[3][0] = LocalClientGlobals->predictedPlayerEntity.pose.origin[0];
      v41[1] = *(float *)&cent->pose.localClientNum;
      v41[2] = *(float *)&cent->pose.cullIn;
      v39 = ikState->angles;
      v38 = LocalClientGlobals->predictedPlayerState.viewangles;
      ikState->angles[0] = LocalClientGlobals->predictedPlayerState.viewangles[0];
      v39[1] = v38[1];
      v39[2] = v38[2];
      v51 = 1;
    }
    goto LABEL_16;
  }
}

void __cdecl IKImport_UpdateCollisionCache(IKState *ikState)
{
  float absmin[3]; // [esp+8h] [ebp-54h] BYREF
  int contents_mask_playerclip; // [esp+14h] [ebp-48h]
  int contents_mask; // [esp+18h] [ebp-44h]
  LerpEntityState *lerpEntity; // [esp+1Ch] [ebp-40h]
  float entityBoundsMin[3]; // [esp+20h] [ebp-3Ch]
  float expand_vec[3]; // [esp+2Ch] [ebp-30h] BYREF
  float absmax[3]; // [esp+38h] [ebp-24h] BYREF
  float entityBoundsMax[3]; // [esp+44h] [ebp-18h]
  IKCollisionCache *collCache; // [esp+50h] [ebp-Ch]
  float expandRange; // [esp+54h] [ebp-8h]
  int contents_mask_no_playerclip; // [esp+58h] [ebp-4h]

  contents_mask_no_playerclip = (int)&loc_82000C + 5;
  contents_mask_playerclip = (int)&loc_830011;
  collCache = &ikState->collisionCache;
  expandRange = FLOAT_32_0;
  expand_vec[0] = FLOAT_32_0;
  expand_vec[1] = FLOAT_32_0;
  expand_vec[2] = FLOAT_32_0;
  entityBoundsMin[0] = FLOAT_N20_0;
  entityBoundsMin[1] = FLOAT_N20_0;
  entityBoundsMin[2] = FLOAT_N10_0;
  entityBoundsMax[0] = FLOAT_20_0;
  entityBoundsMax[1] = FLOAT_20_0;
  entityBoundsMax[2] = FLOAT_40_0;
  absmin[0] = ikState->origin[0] + -20.0;
  absmin[1] = ikState->origin[1] + -20.0;
  absmin[2] = ikState->origin[2] + -10.0;
  absmax[0] = ikState->origin[0] + 20.0;
  absmax[1] = ikState->origin[1] + 20.0;
  absmax[2] = ikState->origin[2] + 40.0;
  lerpEntity = IKImport_GetLerpEntityState(ikState, -1);
  if ( forcePlayerClip || !ikState->lerpInfo[3].goalState || (lerpEntity->eFlags2 & 0x10000) != 0 )
    contents_mask = (int)&loc_830011;
  else
    contents_mask = (int)&loc_82000C + 5;
  if ( contents_mask != collCache->proximity_data.m_mask )
  {
    ikState->footTraceCollisionHint[0] = 0;
    ikState->footTraceCollisionHint[1] = 0;
    ikState->footTraceCollisionHint[2] = 0;
    ikState->footTraceCollisionHint[3] = 0;
  }
  collCache->proximity_data.update(&collCache->proximity_data, absmin, absmax, contents_mask, expand_vec);
}

bool __cdecl IKImport_TraceBox(
        IKState *ikState,
        float *startPos,
        float *endPos,
        float boxRadius,
        float *tracePos,
        float *outNormal,
        int hintIndexListItem,
        float *outTraceFraction,
        bool bIgnoreStartSolid)
{
  col_context_t context; // [esp+18h] [ebp-68h] BYREF
  trace_t tr; // [esp+40h] [ebp-40h] BYREF
  IKCollisionCache *collCache; // [esp+7Ch] [ebp-4h]

  memset(&tr, 0, 16);
  col_context_t::col_context_t(&context);
  //PIXBeginNamedEvent(-1, "IK Trace");
  collCache = &ikState->collisionCache;
  context.prims = ikState->collisionCache.proximity_data.prims;
  context.nprims = ikState->collisionCache.proximity_data.nprims;
  if ( boxRadius == 0.0 )
  {
    if ( hintIndexListItem >= 0 )
      trace_point_vs_env(
        &tr,
        startPos,
        endPos,
        &context,
        &ikState->footTraceCollisionHint[hintIndexListItem],
        ikState->localIkSystem->localClipMap);
    else
      trace_point_vs_env(&tr, startPos, endPos, &context, &collCache->hitIndex, ikState->localIkSystem->localClipMap);
  }
  else if ( hintIndexListItem >= 0 )
  {
    trace_sphere_vs_env(
      &tr,
      startPos,
      endPos,
      boxRadius,
      &context,
      &ikState->footTraceCollisionHint[hintIndexListItem],
      ikState->localIkSystem->localClipMap);
  }
  else
  {
    trace_sphere_vs_env(
      &tr,
      startPos,
      endPos,
      boxRadius,
      &context,
      &collCache->hitIndex,
      ikState->localIkSystem->localClipMap);
  }
  Vec3Lerp(startPos, endPos, tr.fraction, tracePos);
  *(_QWORD *)outNormal = *(_QWORD *)tr.normal.vec.v;
  outNormal[2] = tr.normal.vec.v[2];
  if ( outTraceFraction )
    *outTraceFraction = tr.fraction;
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  return tr.fraction < 1.0 && (!bIgnoreStartSolid || tr.fraction > 0.001);
}

void __cdecl IKImport_DrawAxisOrigin(float (*mat44)[4], float colorScale)
{
  float green[4]; // [esp+14h] [ebp-3Ch] BYREF
  float blue[4]; // [esp+24h] [ebp-2Ch] BYREF
  float red[4]; // [esp+34h] [ebp-1Ch] BYREF
  float end[3]; // [esp+44h] [ebp-Ch] BYREF

  red[0] = colorScale * 1.0;
  red[1] = colorScale * 0.0;
  red[2] = colorScale * 0.0;
  red[3] = colorScale * 1.0;
  green[0] = colorScale * 0.0;
  green[1] = colorScale * 1.0;
  green[2] = colorScale * 0.0;
  green[3] = colorScale * 1.0;
  blue[0] = colorScale * 0.0;
  blue[1] = colorScale * 0.0;
  blue[2] = colorScale * 1.0;
  blue[3] = colorScale * 1.0;
  end[0] = (float)(5.0 * (*mat44)[0]) + (*mat44)[12];
  end[1] = (float)(5.0 * (*mat44)[1]) + (*mat44)[13];
  end[2] = (float)(5.0 * (*mat44)[2]) + (*mat44)[14];
  CG_DebugLine(&(*mat44)[12], end, red, 0, 1);
  end[0] = (float)(5.0 * (*mat44)[4]) + (*mat44)[12];
  end[1] = (float)(5.0 * (*mat44)[5]) + (*mat44)[13];
  end[2] = (float)(5.0 * (*mat44)[6]) + (*mat44)[14];
  CG_DebugLine(&(*mat44)[12], end, green, 0, 1);
  end[0] = (float)(5.0 * (*mat44)[8]) + (*mat44)[12];
  end[1] = (float)(5.0 * (*mat44)[9]) + (*mat44)[13];
  end[2] = (float)(5.0 * (*mat44)[10]) + (*mat44)[14];
  CG_DebugLine(&(*mat44)[12], end, blue, 0, 1);
}

void __cdecl IKImport_DrawLine(float *start, float *end, int colorType)
{
  float color[4]; // [esp+4h] [ebp-10h] BYREF

  switch ( colorType )
  {
    case 0:
      color[0] = colorRed[0];
      color[1] = 0.0;
      color[2] = 0.0;
      color[3] = 1.0;
      break;
    case 1:
      color[0] = colorGreen[0];
      color[1] = 1.0;
      color[2] = 0.0;
      color[3] = 1.0;
      break;
    case 2:
      color[0] = colorBlue[0];
      color[1] = 0.0;
      color[2] = 1.0;
      color[3] = 1.0;
      break;
    case 3:
      color[0] = colorLtCyan[0];
      color[1] = 0.75;
      color[2] = 0.75;
      color[3] = 1.0;
      break;
    default:
      break;
  }
  CG_DebugLine(start, end, color, 0, 1);
}

void IKImport_DebugPrint(const char *fmt, ...)
{
  char msg[256]; // [esp+4h] [ebp-108h] BYREF
  int maxDebugPrintMsg; // [esp+108h] [ebp-4h]
  va_list va; // [esp+118h] [ebp+Ch] BYREF

  va_start(va, fmt);
  maxDebugPrintMsg = 256;
  _vsnprintf(msg, 0x100u, fmt, va);
  msg[255] = 0;
  Com_Printf(0, msg);
}

clientInfo_t *__cdecl IKImport_GetClientInfo(IKState *ikState)
{
  int LocalClientIndex; // eax
  clientInfo_t *ci; // [esp+4h] [ebp-4h]

  if ( ikState->entityNum >= 32 )
    return 0;
  if ( ikState->isServer )
  {
    ci = &level_bgs.clientinfo[ikState->entityNum];
  }
  else
  {
    LocalClientIndex = IKImport_GetLocalClientIndex(ikState);
    ci = &CG_GetLocalClientGlobals(LocalClientIndex)->bgs.clientinfo[ikState->entityNum];
  }
  if ( ci->infoValid )
    return ci;
  else
    return 0;
}

// local variable allocation has failed, the output may be wrong!
void  IKImport_DrawDebugSkeleton(int a1@<ebp>, IKState *ikState)
{
  _BYTE v2[76]; // [esp+14h] [ebp-ACh] OVERLAPPED BYREF
  _BYTE *v3; // [esp+60h] [ebp-60h]
  _BYTE v4[76]; // [esp+64h] [ebp-5Ch] OVERLAPPED BYREF
  int i; // [esp+B0h] [ebp-10h]
  int v6; // [esp+B4h] [ebp-Ch]
  void *v7; // [esp+B8h] [ebp-8h]
  void *retaddr; // [esp+C0h] [ebp+0h]

  v6 = a1;
  v7 = retaddr;
  if ( !ikState->entityNum )
  {
    for ( i = 0; i < 23; ++i )
    {
      v4[75] = ikState->ikBoneToObjBone[i] != 161;
      if ( !v4[75] )
        return;
    }
    for ( i = 0; i < 23; ++i )
    {
      ikCalcBoneModelMatrix(ikState, i, (*ikState->matArrayPostIK)[0], (float (*)[4])v4);
      v3 = &v4[48];
      *(unsigned int *)&v2[72] = ikState->entityXform[3];
      *(unsigned int *)&v2[68] = &v4[48];
      *(float *)&v4[48] = *(float *)&v4[48] + ikState->entityXform[3][0];
      *(float *)&v4[52] = *(float *)&v4[52] + ikState->entityXform[3][1];
      *(float *)&v4[56] = *(float *)&v4[56] + ikState->entityXform[3][2];
      IKImport_DrawAxisOrigin((float (*)[4])v4, 1.0);
      if ( ikSystem.ikBoneParents[i] != IKBONE_NONE )
      {
        ikCalcBoneModelMatrix(ikState, ikSystem.ikBoneParents[i], (*ikState->matArrayPostIK)[0], (float (*)[4])v2);
        *(float *)&v2[48] = *(float *)&v2[48] + ikState->entityXform[3][0];
        *(float *)&v2[52] = *(float *)&v2[52] + ikState->entityXform[3][1];
        *(float *)&v2[56] = *(float *)&v2[56] + ikState->entityXform[3][2];
        IKImport_DrawLine((float *)&v4[48], (float *)&v2[48], 3);
      }
    }
  }
}

int __cdecl IKImport_GetFrameNum()
{
  return com_frameNumber;
}

bool __cdecl IKImport_IsMainThread()
{
  return Sys_IsMainThread();
}

int __cdecl IKImport_GetLocalClientIndex(IKState *ikState)
{
  return DObjGetLocalClientIndex((const DObj *)ikState->model);
}

int __cdecl IKImport_GetLocalClientIndexForModel(unsigned __int8 *model)
{
  return DObjGetLocalClientIndex((const DObj *)model);
}

bool __cdecl IKImport_IsServerModel(unsigned __int8 *model)
{
  return DObjIsServer((const DObj *)model);
}

bool __cdecl IKImport_IsLocalClient(IKState *ikState)
{
  return DObjIsLocalPlayer((const DObj *)ikState->model);
}

bool __cdecl IKImport_IsLeftHandGripIKActive(IKState *ikState)
{
  return DObjIsLeftHandGripIKActive((const DObj *)ikState->model);
}

bool __cdecl IKImport_IsClient(IKState *ikState)
{
  return ikState->entityNum < 32;
}

LerpEntityState *__cdecl IKImport_GetLerpEntityState(IKState *ikState, int inEntityNum)
{
  int LocalClientIndex; // eax
  int v4; // eax
  cg_s *cgameGlob; // [esp+8h] [ebp-10h]
  centity_s *cent; // [esp+Ch] [ebp-Ch]
  int entityNum; // [esp+14h] [ebp-4h]

  entityNum = ikState->entityNum;
  if ( inEntityNum >= 0 )
    entityNum = inEntityNum;
  if ( ikState->isServer )
    return &g_entities[entityNum].s.lerp;
  LocalClientIndex = IKImport_GetLocalClientIndex(ikState);
  cgameGlob = CG_GetLocalClientGlobals(LocalClientIndex);
  if ( cgameGlob->clientNum == entityNum )
  {
    cent = &cgameGlob->predictedPlayerEntity;
  }
  else
  {
    v4 = IKImport_GetLocalClientIndex(ikState);
    cent = CG_GetEntity(v4, entityNum);
  }
  return &cent->nextState.lerp;
}

gentity_s *__cdecl IKImport_GetEntityState(IKState *ikState, int inEntityNum)
{
  int LocalClientIndex; // eax
  int v4; // eax
  cg_s *cgameGlob; // [esp+8h] [ebp-10h]
  centity_s *cent; // [esp+Ch] [ebp-Ch]
  int entityNum; // [esp+14h] [ebp-4h]

  entityNum = ikState->entityNum;
  if ( inEntityNum >= 0 )
    entityNum = inEntityNum;
  if ( ikState->isServer )
    return &g_entities[entityNum];
  LocalClientIndex = IKImport_GetLocalClientIndex(ikState);
  cgameGlob = CG_GetLocalClientGlobals(LocalClientIndex);
  if ( cgameGlob->clientNum == entityNum )
  {
    cent = &cgameGlob->predictedPlayerEntity;
  }
  else
  {
    v4 = IKImport_GetLocalClientIndex(ikState);
    cent = CG_GetEntity(v4, entityNum);
  }
  return (gentity_s *)&cent->nextState;
}

char __cdecl IKImport_ConditionLayerTest(
        bool condition,
        IKLayerNames thisLayer,
        const IKLayerNames *layers,
        int arraySize)
{
  int i; // [esp+0h] [ebp-4h]

  for ( i = 0; i < arraySize; ++i )
  {
    if ( thisLayer == layers[i] )
      return 1;
  }
  return 0;
}

unsigned int __cdecl IKImport_GetWeaponIndex(IKState *ikState)
{
  int v2; // eax
  int LocalClientIndex; // eax
  cg_s *LocalClientGlobals; // [esp+0h] [ebp-14h]
  clientInfo_t *ci; // [esp+8h] [ebp-Ch]
  gentity_s *ent; // [esp+10h] [ebp-4h]

  if ( ikState->isServer )
  {
    ent = &g_entities[ikState->entityNum];
    if ( ent->client && ent->client->sess.connected == CON_CONNECTED )
    {
      if ( (ent->client->ps.weapFlags & 2) != 0 )
        return ent->client->ps.offHandIndex;
      else
        return ent->client->ps.weapon;
    }
    else
    {
      return 0;
    }
  }
  else if ( ikState->entityNum < 32 )
  {
    if ( IKImport_IsLocalClient(ikState) )
    {
      LocalClientIndex = IKImport_GetLocalClientIndex(ikState);
      LocalClientGlobals = CG_GetLocalClientGlobals(LocalClientIndex);
      if ( !LocalClientGlobals
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 2249, 0, "%s", "cgameGlob") )
      {
        __debugbreak();
      }
      if ( (LocalClientGlobals->predictedPlayerState.weapFlags & 2) != 0
        && LocalClientGlobals->predictedPlayerState.offHandIndex >= 0 )
      {
        return LocalClientGlobals->predictedPlayerState.offHandIndex;
      }
      else
      {
        return LocalClientGlobals->predictedPlayerState.weapon;
      }
    }
    else
    {
      v2 = IKImport_GetLocalClientIndex(ikState);
      if ( !CG_GetLocalClientGlobals(v2)
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 2236, 0, "%s", "cgameGlob") )
      {
        __debugbreak();
      }
      ci = IKImport_GetClientInfo(ikState);
      if ( ci && ci->infoValid )
        return ci->iDObjWeapon;
      else
        return 0;
    }
  }
  else
  {
    return IKImport_GetEntityState(ikState, -1)->s.weapon;
  }
}

const WeaponVariantDef *__cdecl IKImport_GetWeaponVariantDef(IKState *ikState)
{
  unsigned int WeaponIndex; // eax

  WeaponIndex = IKImport_GetWeaponIndex(ikState);
  return BG_GetWeaponVariantDef(WeaponIndex);
}

const WeaponDef *__cdecl IKImport_GetWeaponDef(IKState *ikState)
{
  unsigned int WeaponIndex; // eax

  WeaponIndex = IKImport_GetWeaponIndex(ikState);
  return BG_GetWeaponDef(WeaponIndex);
}

bool __cdecl IKImport_IsVehicleRemoteControl(IKState *ikState)
{
  gentity_s *EntityState; // [esp+4h] [ebp-8h]
  gentity_s *ent; // [esp+8h] [ebp-4h]

  if ( ikState->isServer )
  {
    ent = &g_entities[ikState->entityNum];
    return ent->client
        && ent->client->sess.connected == CON_CONNECTED
        && (ent->client->ps.eFlags & 0x4000) != 0
        && (ent->client->ps.eFlags2 & 0x10000000) != 0;
  }
  else
  {
    EntityState = IKImport_GetEntityState(ikState, -1);
    return (EntityState->s.lerp.eFlags & 0x4000) != 0 && (EntityState->s.lerp.eFlags2 & 0x10000000) != 0;
  }
}

double __cdecl IKImport_GetLayerLerp(IKState *ikState, IKLayerNames layerName, float layerLerpTime)
{
  double LayerLerp; // st7
  char v5; // al
  char v6; // al
  float v7; // [esp+4h] [ebp-1C0h]
  float v8; // [esp+8h] [ebp-1BCh]
  bool v9; // [esp+10h] [ebp-1B4h]
  int v10; // [esp+14h] [ebp-1B0h]
  bool v11; // [esp+18h] [ebp-1ACh]
  int v12; // [esp+1Ch] [ebp-1A8h]
  bool v13; // [esp+20h] [ebp-1A4h]
  int v14; // [esp+24h] [ebp-1A0h]
  bool v15; // [esp+28h] [ebp-19Ch]
  bool v16; // [esp+2Ch] [ebp-198h]
  bool enabled; // [esp+33h] [ebp-191h]
  bool v18; // [esp+34h] [ebp-190h]
  bool v19; // [esp+38h] [ebp-18Ch]
  bool v20; // [esp+3Ch] [ebp-188h]
  bool v21; // [esp+40h] [ebp-184h]
  bool v22; // [esp+44h] [ebp-180h]
  bool v23; // [esp+48h] [ebp-17Ch]
  bool v24; // [esp+4Ch] [ebp-178h]
  bool v25; // [esp+50h] [ebp-174h]
  bool v26; // [esp+54h] [ebp-170h]
  bool v27; // [esp+58h] [ebp-16Ch]
  bool v28; // [esp+5Ch] [ebp-168h]
  bool v29; // [esp+64h] [ebp-160h]
  bool v30; // [esp+6Ch] [ebp-158h]
  bool v31; // [esp+70h] [ebp-154h]
  bool v32; // [esp+78h] [ebp-14Ch]
  bool v33; // [esp+7Ch] [ebp-148h]
  bool v34; // [esp+84h] [ebp-140h]
  bool v35; // [esp+88h] [ebp-13Ch]
  bool v36; // [esp+90h] [ebp-134h]
  bool v37; // [esp+94h] [ebp-130h]
  bool v38; // [esp+9Ch] [ebp-128h]
  bool v39; // [esp+A0h] [ebp-124h]
  bool v40; // [esp+A8h] [ebp-11Ch]
  bool v41; // [esp+B0h] [ebp-114h]
  bool v42; // [esp+B8h] [ebp-10Ch]
  float v43; // [esp+C0h] [ebp-104h]
  float v44; // [esp+C4h] [ebp-100h]
  float lerp; // [esp+E4h] [ebp-E0h]
  float v46; // [esp+E8h] [ebp-DCh]
  float v47; // [esp+14Ch] [ebp-78h]
  float v48; // [esp+150h] [ebp-74h]
  float v49; // [esp+154h] [ebp-70h]
  float *offset; // [esp+158h] [ebp-6Ch]
  float *rotation; // [esp+15Ch] [ebp-68h]
  float frameTime; // [esp+164h] [ebp-60h]
  IKLayerNames turretIKLayers[4]; // [esp+168h] [ebp-5Ch] BYREF
  ikLerpInfo_t *li; // [esp+178h] [ebp-4Ch]
  IKLayerNames mantleIKLayers[1]; // [esp+17Ch] [ebp-48h] BYREF
  LerpEntityState *lerpEntity; // [esp+180h] [ebp-44h]
  IKLayerNames disableIKLayers[2]; // [esp+184h] [ebp-40h] BYREF
  IKLayerNames proneIKLayers[2]; // [esp+18Ch] [ebp-38h] BYREF
  IKLayerNames frontEndIKLayers[2]; // [esp+194h] [ebp-30h] BYREF
  const WeaponVariantDef *weapVariantDef; // [esp+19Ch] [ebp-28h]
  bool bOrTest; // [esp+1A3h] [ebp-21h]
  IKLayerNames actorIKLayers[3]; // [esp+1A4h] [ebp-20h] BYREF
  bool bOn; // [esp+1B3h] [ebp-11h]
  float lerpScale; // [esp+1B4h] [ebp-10h]
  const WeaponDef *weapDef; // [esp+1B8h] [ebp-Ch]
  IKLayerNames vehicleIKLayers[2]; // [esp+1BCh] [ebp-8h] BYREF

  lerpScale = FLOAT_1_0;
  if ( layerLerpTime < 0.0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 2328, 0, "%s", "layerLerpTime >= 0.f") )
  {
    __debugbreak();
  }
  li = &ikState->lerpInfo[layerName];
  if ( ikLayerRealLayers[layerName] )
  {
    switch ( layerName )
    {
      case IKLAYER_LEFT_HAND_ON_GUN:
      case IKLAYER_RIGHT_HAND_ON_GUN:
      case IKLAYER_PLAYER_CONTROLLERS:
      case IKLAYER_LOOK_AT_ENTITY:
        break;
      default:
        LayerLerp = IKImport_GetLayerLerp(ikState, IKLAYER_PRONE_SLOW_FADE_ACTIVE);
        v48 = LayerLerp + LayerLerp;
        v49 = IKImport_GetLayerLerp(ikState, IKLAYER_PRONE_ACTIVE);
        if ( (float)(v48 - v49) < 0.0 )
          v44 = v49;
        else
          v44 = v48;
        if ( (float)(v44 - 1.0) < 0.0 )
          v47 = v44;
        else
          v47 = FLOAT_1_0;
        if ( (float)(0.0 - v44) < 0.0 )
          v43 = v47;
        else
          v43 = *(float *)&FLOAT_0_0;
        lerpScale = (float)(1.0 - v43) * lerpScale;
        break;
    }
  }
  if ( li->lastUpdateTime == ikState->timeMS )
    return IKImport_SmoothLerpAdjust(li->lerp * lerpScale);
  frameTime = (float)(ikState->timeMS - li->lastUpdateTime) * 0.001;
  if ( frameTime < 0.0 )
    frameTime = *(float *)&FLOAT_0_0;
  li->lastUpdateTime = ikState->timeMS;
  lerpEntity = IKImport_GetLerpEntityState(ikState, -1);
  weapVariantDef = 0;
  weapDef = 0;
  bOn = 1;
  bOrTest = 0;
  if ( layerName == IKLAYER_VEHICLE_GUNNER_OFFSET || layerName == IKLAYER_TURRET_OFFSET )
  {
    bOn = 0;
    bOrTest = 1;
  }
  disableIKLayers[0] = IKLAYER_LOOK_AT_ENTITY;
  disableIKLayers[1] = IKLAYER_COUNT;
  vehicleIKLayers[0] = IKLAYER_VEHICLE_GUNNER_OFFSET;
  vehicleIKLayers[1] = IKLAYER_COUNT;
  turretIKLayers[0] = IKLAYER_TERRAIN_MAPPING;
  turretIKLayers[1] = IKLAYER_TURRET_FIRING;
  turretIKLayers[2] = IKLAYER_TURRET_OFFSET;
  turretIKLayers[3] = IKLAYER_COUNT;
  proneIKLayers[0] = IKLAYER_PLAYER_CONTROLLERS;
  proneIKLayers[1] = IKLAYER_COUNT;
  mantleIKLayers[0] = IKLAYER_COUNT;
  actorIKLayers[0] = IKLAYER_TERRAIN_MAPPING;
  actorIKLayers[1] = IKLAYER_LOOK_AT_ENTITY;
  actorIKLayers[2] = IKLAYER_COUNT;
  frontEndIKLayers[0] = IKLAYER_LEFT_HAND_ON_GUN;
  frontEndIKLayers[1] = IKLAYER_COUNT;
  if ( bOrTest )
  {
    v42 = bOn
       || (lerpEntity->eFlags & 0x20000) != 0
       && IKImport_ConditionLayerTest((lerpEntity->eFlags & 0x20000) != 0, layerName, disableIKLayers, 2);
    bOn = v42;
  }
  else
  {
    v41 = bOn
       && ((lerpEntity->eFlags & 0x20000) == 0
        || IKImport_ConditionLayerTest((lerpEntity->eFlags & 0x20000) != 0, layerName, disableIKLayers, 2));
    bOn = v41;
  }
  if ( bOrTest )
  {
    v40 = 1;
    if ( !bOn )
    {
      if ( (lerpEntity->eFlags & 0x4000) == 0
        || (lerpEntity->eFlags & 0x10000) != 0
        || IKImport_IsVehicleRemoteControl(ikState)
        || ((lerpEntity->eFlags & 0x4000) == 0
         || (lerpEntity->eFlags & 0x10000) != 0
         || IKImport_IsVehicleRemoteControl(ikState)
          ? (v5 = IKImport_ConditionLayerTest(0, layerName, vehicleIKLayers, 2))
          : (v5 = IKImport_ConditionLayerTest(1, layerName, vehicleIKLayers, 2)),
            !v5) )
      {
        v40 = 0;
      }
    }
    bOn = v40;
  }
  else
  {
    v39 = 0;
    if ( bOn )
    {
      if ( (lerpEntity->eFlags & 0x4000) == 0
        || (lerpEntity->eFlags & 0x10000) != 0
        || IKImport_IsVehicleRemoteControl(ikState)
        || ((lerpEntity->eFlags & 0x4000) == 0
         || (lerpEntity->eFlags & 0x10000) != 0
         || IKImport_IsVehicleRemoteControl(ikState)
          ? (v6 = IKImport_ConditionLayerTest(0, layerName, vehicleIKLayers, 2))
          : (v6 = IKImport_ConditionLayerTest(1, layerName, vehicleIKLayers, 2)),
            v6) )
      {
        v39 = 1;
      }
    }
    bOn = v39;
  }
  if ( bOrTest )
  {
    v38 = bOn
       || (lerpEntity->eFlags & 0x300) != 0
       && IKImport_ConditionLayerTest((lerpEntity->eFlags & 0x300) != 0, layerName, turretIKLayers, 4);
    bOn = v38;
  }
  else
  {
    v37 = bOn
       && ((lerpEntity->eFlags & 0x300) == 0
        || IKImport_ConditionLayerTest((lerpEntity->eFlags & 0x300) != 0, layerName, turretIKLayers, 4));
    bOn = v37;
  }
  if ( bOrTest )
  {
    v36 = bOn
       || (lerpEntity->eFlags & 8) != 0
       && IKImport_ConditionLayerTest((lerpEntity->eFlags & 8) != 0, layerName, proneIKLayers, 2);
    bOn = v36;
  }
  else
  {
    v35 = bOn
       && ((lerpEntity->eFlags & 8) == 0
        || IKImport_ConditionLayerTest((lerpEntity->eFlags & 8) != 0, layerName, proneIKLayers, 2));
    bOn = v35;
  }
  if ( bOrTest )
  {
    v34 = bOn
       || (lerpEntity->eFlags & 0x8000) != 0
       && IKImport_ConditionLayerTest((lerpEntity->eFlags & 0x8000) != 0, layerName, mantleIKLayers, 1);
    bOn = v34;
  }
  else
  {
    v33 = bOn
       && ((lerpEntity->eFlags & 0x8000) == 0
        || IKImport_ConditionLayerTest((lerpEntity->eFlags & 0x8000) != 0, layerName, mantleIKLayers, 1));
    bOn = v33;
  }
  if ( bOrTest )
  {
    v32 = bOn
       || ikState->entityNum >= 32 && IKImport_ConditionLayerTest(ikState->entityNum >= 32, layerName, actorIKLayers, 3);
    bOn = v32;
  }
  else
  {
    v31 = bOn
       && (ikState->entityNum < 32 || IKImport_ConditionLayerTest(ikState->entityNum >= 32, layerName, actorIKLayers, 3));
    bOn = v31;
  }
  if ( bOrTest )
  {
    v30 = bOn || ikSystem.inViewer && IKImport_ConditionLayerTest(ikSystem.inViewer, layerName, frontEndIKLayers, 2);
    bOn = v30;
  }
  else
  {
    v29 = bOn && (!ikSystem.inViewer || IKImport_ConditionLayerTest(ikSystem.inViewer, layerName, frontEndIKLayers, 2));
    bOn = v29;
  }
  switch ( layerName )
  {
    case IKLAYER_CROUCH_ACTIVE:
      bOn = (lerpEntity->eFlags & 4) != 0;
      break;
    case IKLAYER_PRONE_ACTIVE:
    case IKLAYER_PRONE_SLOW_FADE_ACTIVE:
      bOn = (lerpEntity->eFlags & 8) != 0;
      break;
    case IKLAYER_NOT_MOVING:
    case IKLAYER_NOT_MOVING_MED:
    case IKLAYER_NOT_MOVING_FAST:
      v26 = ikState->lastMovedTime < ikState->timeMS - notMovedTime
         && ikState->lastNotMovedTime > ikState->lastMovedTime;
      bOn = v26;
      break;
    case IKLAYER_MINIGUN_ACTIVE:
      bOn = ikState->weaponOffsetType == IKWEAPOFS_MINIGUN;
      break;
    case IKLAYER_SHOTGUN_ACTIVE:
      bOn = ikState->weaponClass == IKWEAPCLASS_SHOTGUN;
      break;
    case IKLAYER_PELVIS_TEST:
      v19 = bOn && IKImport_GetVar_IK_Pelvis_Test();
      bOn = v19;
      break;
    case IKLAYER_TERRAIN_MAPPING:
      v23 = bOn && (lerpEntity->eFlags2 & 0x100000) == 0;
      bOn = v23;
      v22 = v23 && IKImport_ApplyTerrainMapping(ikState);
      bOn = v22;
      v21 = v22 && !IKImport_BypassTerrainMapping(ikState);
      bOn = v21;
      v20 = v21 && ((lerpEntity->eFlags & 0x300) != 0 || !IKImport_IsSpectating(ikState));
      bOn = v20;
      break;
    case IKLAYER_HAND_TEST:
      v18 = bOn && IKImport_GetVar_IK_Hand_Test();
      bOn = v18;
      break;
    case IKLAYER_LEFT_HAND_ON_GUN:
      if ( ikSystem.inViewer )
      {
        bOn = 1;
        layerLerpTime = *(float *)&FLOAT_0_0;
      }
      else
      {
        if ( IKImport_IsClient(ikState) )
          enabled = ikState->dvarValues[7].enabled;
        else
          enabled = ikState->dvarValues[9].enabled;
        bOn = enabled;
        v16 = enabled && (IKImport_IsLeftHandGripIKActive(ikState) || IKImport_GetVar_IK_Hand_Tuning(ikState) == 2);
        bOn = v16;
      }
      if ( bOn )
      {
        weapVariantDef = IKImport_GetWeaponVariantDef(ikState);
        if ( weapVariantDef )
        {
          offset = weapVariantDef->ikLeftHandOffset;
          rotation = weapVariantDef->ikLeftHandRotation;
          if ( IKImport_IsProne(ikState) )
          {
            v15 = weapVariantDef->ikLeftHandProneOffset[0] == 0.0
               && weapVariantDef->ikLeftHandProneOffset[1] == 0.0
               && weapVariantDef->ikLeftHandProneOffset[2] == 0.0;
            if ( !v15
              || (weapVariantDef->ikLeftHandProneRotation[0] != 0.0
               || weapVariantDef->ikLeftHandProneRotation[1] != 0.0
               || weapVariantDef->ikLeftHandProneRotation[2] != 0.0
                ? (v14 = 0)
                : (v14 = 1),
                  !v14) )
            {
              offset = weapVariantDef->ikLeftHandProneOffset;
              rotation = weapVariantDef->ikLeftHandProneRotation;
            }
          }
          if ( ikSystem.inViewer )
          {
            v13 = weapVariantDef->ikLeftHandUiViewerOffset[0] == 0.0
               && weapVariantDef->ikLeftHandUiViewerOffset[1] == 0.0
               && weapVariantDef->ikLeftHandUiViewerOffset[2] == 0.0;
            if ( !v13
              || (weapVariantDef->ikLeftHandUiViewerRotation[0] != 0.0
               || weapVariantDef->ikLeftHandUiViewerRotation[1] != 0.0
               || weapVariantDef->ikLeftHandUiViewerRotation[2] != 0.0
                ? (v12 = 0)
                : (v12 = 1),
                  !v12) )
            {
              offset = weapVariantDef->ikLeftHandUiViewerOffset;
              rotation = weapVariantDef->ikLeftHandUiViewerRotation;
            }
          }
          v11 = *offset == ikState->leftHandOffset[0]
             && offset[1] == ikState->leftHandOffset[1]
             && offset[2] == ikState->leftHandOffset[2];
          if ( !v11
            || (*rotation != ikState->leftHandRotation[0]
             || rotation[1] != ikState->leftHandRotation[1]
             || rotation[2] != ikState->leftHandRotation[2]
              ? (v10 = 0)
              : (v10 = 1),
                !v10) )
          {
            if ( ikState->leftHandChangeTime )
            {
              ikState->leftHandOffsetPrev[0] = ikState->leftHandOffset[0];
              ikState->leftHandOffsetPrev[1] = ikState->leftHandOffset[1];
              ikState->leftHandOffsetPrev[2] = ikState->leftHandOffset[2];
              ikState->leftHandRotationPrev[0] = ikState->leftHandRotation[0];
              ikState->leftHandRotationPrev[1] = ikState->leftHandRotation[1];
              ikState->leftHandRotationPrev[2] = ikState->leftHandRotation[2];
              ikState->leftHandOffset[0] = *offset;
              ikState->leftHandOffset[1] = offset[1];
              ikState->leftHandOffset[2] = offset[2];
              ikState->leftHandRotation[0] = *rotation;
              ikState->leftHandRotation[1] = rotation[1];
              ikState->leftHandRotation[2] = rotation[2];
              ikState->leftHandChangeTime = ikState->timeMS;
            }
            else
            {
              ikState->leftHandOffset[0] = *offset;
              ikState->leftHandOffset[1] = offset[1];
              ikState->leftHandOffset[2] = offset[2];
              ikState->leftHandRotation[0] = *rotation;
              ikState->leftHandRotation[1] = rotation[1];
              ikState->leftHandRotation[2] = rotation[2];
              ikState->leftHandOffsetPrev[0] = ikState->leftHandOffset[0];
              ikState->leftHandOffsetPrev[1] = ikState->leftHandOffset[1];
              ikState->leftHandOffsetPrev[2] = ikState->leftHandOffset[2];
              ikState->leftHandRotationPrev[0] = ikState->leftHandRotation[0];
              ikState->leftHandRotationPrev[1] = ikState->leftHandRotation[1];
              ikState->leftHandRotationPrev[2] = ikState->leftHandRotation[2];
              ikState->leftHandChangeTime = 1;
            }
          }
        }
        else
        {
          bOn = 0;
        }
      }
      break;
    case IKLAYER_RIGHT_HAND_ON_GUN:
      bOn = 0;
      break;
    case IKLAYER_PLAYER_PITCH:
      v28 = bOn && IKImport_IsClient(ikState);
      bOn = v28;
      if ( v28 )
      {
        weapDef = IKImport_GetWeaponDef(ikState);
        if ( !weapDef
          && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 2472, 0, "%s", "weapDef") )
        {
          __debugbreak();
        }
        v27 = bOn && weapDef->weapClass != WEAPCLASS_GRENADE;
        bOn = v27;
      }
      break;
    case IKLAYER_TURRET_FIRING:
      v24 = bOn && IKImport_IsMountedTurret(ikState) && IKImport_IsFiring(ikState);
      bOn = v24;
      break;
    case IKLAYER_PLAYER_CONTROLLERS:
      v25 = bOn && IKImport_IsClient(ikState);
      bOn = v25;
      break;
    case IKLAYER_LOOK_AT_ENTITY:
      v9 = bOn && !IKImport_IsClient(ikState);
      bOn = v9;
      if ( v9 )
      {
        BG_EvalVehicleName();
        bOn = ikState->actorLookAtEntityScale > 0.0;
        layerLerpTime = FLOAT_N1_0;
        li->lerp = ikState->actorLookAtEntityScale;
      }
      break;
    default:
      break;
  }
  if ( Com_IsRagdollTrajectory(&lerpEntity->pos) )
  {
    layerLerpTime = FLOAT_0_1;
    bOn = 0;
  }
  if ( layerName == offLayer )
    bOn = 0;
  if ( bOn && !IKImport_GetVar_IK_Enable() )
    bOn = 0;
  if ( layerLerpTime <= 0.0 )
  {
    if ( layerLerpTime == 0.0 )
    {
      if ( bOn )
        v7 = FLOAT_1_0;
      else
        v7 = *(float *)&FLOAT_0_0;
      li->lerp = v7;
    }
  }
  else
  {
    if ( bOn )
      li->lerp = (float)(frameTime / layerLerpTime) + li->lerp;
    else
      li->lerp = li->lerp - (float)(frameTime / layerLerpTime);
    lerp = li->lerp;
    if ( (float)(lerp - 1.0) < 0.0 )
      v46 = li->lerp;
    else
      v46 = FLOAT_1_0;
    if ( (float)(0.0 - lerp) < 0.0 )
      v8 = v46;
    else
      v8 = *(float *)&FLOAT_0_0;
    li->lerp = v8;
  }
  li->goalState = bOn;
  return IKImport_SmoothLerpAdjust(li->lerp * lerpScale);
}

double __cdecl IKImport_SmoothLerpAdjust(float in)
{
  float v2; // [esp+Ch] [ebp-Ch]

  if ( in == 0.0 || in == 1.0 )
    return in;
  v2 = sin((float)((float)(in * 3.1415927) - 1.5707964));
  return (float)((float)(v2 + 1.0) / 2.0);
}

double __cdecl IKImport_GetLayerLerp(IKState *ikState, IKLayerNames layerIndex)
{
  return IKImport_GetLayerLerp(ikState, layerIndex, ikLayerLerpTimes[layerIndex]);
}

double __cdecl IKImport_GetGunnerCrouch(IKState *ikState)
{
  int LocalClientIndex; // eax
  int entityNum; // edx
  cg_s *cgameGlob; // [esp+0h] [ebp-Ch]

  if ( ikState->isServer )
    return IKImport_GetLerpEntityState(ikState, -1)->u.turret.gunAngles[0];
  LocalClientIndex = IKImport_GetLocalClientIndex(ikState);
  cgameGlob = CG_GetLocalClientGlobals(LocalClientIndex);
  if ( !cgameGlob
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 2680, 0, "%s", "cgameGlob") )
  {
    __debugbreak();
  }
  if ( cgameGlob
    && (entityNum = ikState->entityNum, (cg_s *)((char *)cgameGlob + entityNum * 1480) != (cg_s *)-389096)
    && cgameGlob->bgs.clientinfo[entityNum].infoValid )
  {
    return cgameGlob->bgs.clientinfo[entityNum].lerpLean;
  }
  else
  {
    return 0.0;
  }
}

bool __cdecl IKImport_IsMountedTurret(IKState *ikState)
{
  return (IKImport_GetLerpEntityState(ikState, -1)->eFlags & 0x300) != 0;
}

bool __cdecl IKImport_IsFiring(IKState *ikState)
{
  gentity_s *EntityState; // [esp+0h] [ebp-Ch]
  LerpEntityState *lerpEntity; // [esp+8h] [ebp-4h]

  lerpEntity = IKImport_GetLerpEntityState(ikState, -1);
  if ( (lerpEntity->eFlags & 0x300) == 0 )
    return (lerpEntity->eFlags & 0x40) != 0;
  EntityState = IKImport_GetEntityState(ikState, -1);
  return (IKImport_GetLerpEntityState(ikState, EntityState->s.otherEntityNum)->eFlags & 0x40) != 0;
}

bool __cdecl IKImport_IsCrouching(IKState *ikState)
{
  return (IKImport_GetLerpEntityState(ikState, -1)->eFlags & 4) != 0;
}

bool __cdecl IKImport_IsProne(IKState *ikState)
{
  return (IKImport_GetLerpEntityState(ikState, -1)->eFlags & 8) != 0;
}

char __cdecl IKImport_GetBoneControllers(IKState *ikState, float *outAngles)
{
  int LocalClientIndex; // eax
  float *v4; // [esp+8h] [ebp-18h]
  float *v5; // [esp+Ch] [ebp-14h]
  int i; // [esp+10h] [ebp-10h]
  cg_s *cgameGlob; // [esp+14h] [ebp-Ch]
  clientControllers_t *control; // [esp+18h] [ebp-8h]
  clientInfo_t *ci; // [esp+1Ch] [ebp-4h]

  if ( ikState->entityNum >= 32 )
    return 0;
  if ( ikState->isServer )
  {
    ci = &level_bgs.clientinfo[ikState->entityNum];
    if ( !ci->infoValid )
      return 0;
    control = &ci->control;
  }
  else
  {
    LocalClientIndex = IKImport_GetLocalClientIndex(ikState);
    cgameGlob = CG_GetLocalClientGlobals(LocalClientIndex);
    if ( !cgameGlob )
      return 0;
    if ( !cgameGlob->bgs.clientinfo[ikState->entityNum].infoValid )
      return 0;
    control = &cgameGlob->bgs.clientinfo[ikState->entityNum].control;
  }
  for ( i = 0; i < 6; ++i )
  {
    v4 = &outAngles[3 * i];
    v5 = control->angles[i];
    *v4 = *v5;
    v4[1] = v5[1];
    v4[2] = v5[2];
  }
  ikState->controller_origin_angles[0] = control->tag_origin_angles[0];
  ikState->controller_origin_angles[1] = control->tag_origin_angles[1];
  ikState->controller_origin_angles[2] = control->tag_origin_angles[2];
  return 1;
}

void __cdecl IKImport_GetVelocity(IKState *ikState, float *velocity)
{
  float *v2; // edx
  int LocalClientIndex; // eax
  int v4; // eax
  int entityNum; // [esp-4h] [ebp-2Ch]
  cg_s *cgameGlob; // [esp+18h] [ebp-10h]
  centity_s *cent; // [esp+1Ch] [ebp-Ch]
  int snapTime; // [esp+20h] [ebp-8h]
  gentity_s *ent; // [esp+24h] [ebp-4h]

  *velocity = *(float *)&FLOAT_0_0;
  velocity[1] = *(float *)&FLOAT_0_0;
  velocity[2] = *(float *)&FLOAT_0_0;
  if ( ikState->isServer )
  {
    ent = &g_entities[ikState->entityNum];
    if ( ikState->entityNum < 32 && ent->client && ent->client->sess.connected == CON_CONNECTED )
    {
      v2 = ent->client->ps.velocity;
      *velocity = *v2;
      velocity[1] = v2[1];
    }
  }
  else
  {
    entityNum = ikState->entityNum;
    LocalClientIndex = IKImport_GetLocalClientIndex(ikState);
    cent = CG_GetEntity(LocalClientIndex, entityNum);
    if ( ((*((unsigned int *)cent + 201) >> 1) & 1) != 0 )
    {
      v4 = IKImport_GetLocalClientIndex(ikState);
      cgameGlob = CG_GetLocalClientGlobals(v4);
      if ( cgameGlob )
      {
        if ( cgameGlob->localClientNum >= 0 )
        {
          if ( cgameGlob->snap )
          {
            if ( cgameGlob->nextSnap )
            {
              *velocity = cent->nextState.lerp.pos.trBase[0] - cent->currentState.pos.trBase[0];
              velocity[1] = cent->nextState.lerp.pos.trBase[1] - cent->currentState.pos.trBase[1];
              snapTime = cgameGlob->nextSnap->serverTime - cgameGlob->snap->serverTime;
              if ( snapTime > 0 )
              {
                *velocity = (float)(1.0 / (float)((float)snapTime * 0.001)) * *velocity;
                velocity[1] = (float)(1.0 / (float)((float)snapTime * 0.001)) * velocity[1];
              }
            }
          }
        }
      }
    }
  }
}

bool __cdecl IKImport_IsSpectating(IKState *ikState)
{
  int LocalClientIndex; // eax
  cg_s *cgameGlob; // [esp+0h] [ebp-4h]

  if ( ikState->isServer )
    return 0;
  LocalClientIndex = IKImport_GetLocalClientIndex(ikState);
  cgameGlob = CG_GetLocalClientGlobals(LocalClientIndex);
  if ( !cgameGlob
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 2901, 0, "%s", "cgameGlob") )
  {
    __debugbreak();
  }
  return (cgameGlob->predictedPlayerState.pm_type == 4 || (cgameGlob->predictedPlayerState.otherFlags & 2) != 0)
      && ikState->entityNum == cgameGlob->predictedPlayerState.clientNum;
}

char __cdecl IKImport_IsMoving(IKState *ikState)
{
  float velocity[3]; // [esp+4h] [ebp-Ch] BYREF

  IKImport_GetVelocity(ikState, velocity);
  if ( Abs(velocity) >= 1.0 )
  {
    ikState->lastMovedTime = ikState->timeMS;
    return 1;
  }
  else
  {
    ikState->lastNotMovedTime = ikState->timeMS;
    return 0;
  }
}

int __cdecl IKImport_GetIKPriority()
{
  return 10;
}

void __cdecl IKImport_SetWeaponOffsetType(IKState *ikState)
{
  weapType_t weapType; // [esp+0h] [ebp-Ch]
  weapClass_t weapClass; // [esp+4h] [ebp-8h]
  const WeaponDef *weapDef; // [esp+8h] [ebp-4h]

  weapDef = IKImport_GetWeaponDef(ikState);
  ikState->weaponOffsetType = IKWEAPOFS_DEFAULT;
  if ( weapDef && weapDef->inventoryType != WEAPINVENTORY_ALTMODE )
  {
    weapClass = weapDef->weapClass;
    if ( weapClass == WEAPCLASS_MG )
    {
      ikState->weaponOffsetType = IKWEAPOFS_MINIGUN;
    }
    else if ( weapClass == WEAPCLASS_PISTOL )
    {
      ikState->weaponOffsetType = IKWEAPOFS_PISTOL;
    }
    else
    {
      weapType = weapDef->weapType;
      if ( weapType == WEAPTYPE_GRENADE )
      {
        if ( weapDef->offhandClass )
          ikState->weaponOffsetType = IKWEAPOFS_GRENADE;
      }
      else if ( weapType == WEAPTYPE_PROJECTILE && weapDef->weapClass != WEAPCLASS_GRENADE )
      {
        ikState->weaponOffsetType = IKWEAPOFS_SHOULDER;
      }
    }
  }
}

void __cdecl IKImport_UpdateWeaponClass(IKState *ikState)
{
  ikState->weaponClass = IKImport_GetWeaponDef(ikState)->weapClass == WEAPCLASS_SPREAD;
}

LerpEntityStateActor::<unnamed_type_index> __cdecl IKImport_GetActorNum(unsigned __int8 *model)
{
  int LocalClientIndex; // eax
  int v2; // eax
  cg_s *cgameGlob; // [esp+8h] [ebp-18h]
  centity_s *cent; // [esp+Ch] [ebp-14h]
  int entityNum; // [esp+18h] [ebp-8h]
  LerpEntityState *lerpEntity; // [esp+1Ch] [ebp-4h]

  if ( !model && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 3030, 0, "%s", "obj") )
    __debugbreak();
  entityNum = *((unsigned __int16 *)model + 3) - 1;
  if ( DObjIsServer((const DObj *)model) )
  {
    if ( g_entities[entityNum].s.eType != 17
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp",
            3039,
            0,
            "%s",
            "ent->s.eType == ET_ACTOR") )
    {
      __debugbreak();
    }
    lerpEntity = &g_entities[entityNum].s.lerp;
  }
  else
  {
    LocalClientIndex = DObjGetLocalClientIndex((const DObj *)model);
    cgameGlob = CG_GetLocalClientGlobals(LocalClientIndex);
    if ( cgameGlob->clientNum == entityNum )
    {
      cent = &cgameGlob->predictedPlayerEntity;
    }
    else
    {
      v2 = DObjGetLocalClientIndex((const DObj *)model);
      cent = CG_GetEntity(v2, entityNum);
    }
    if ( cent->nextState.eType != 17
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp",
            3059,
            0,
            "%s",
            "cent->nextState.eType == ET_ACTOR") )
    {
      __debugbreak();
    }
    lerpEntity = &cent->nextState.lerp;
  }
  return lerpEntity->u.actor.index;
}

bool __cdecl IKImport_IsValidEntityType(const unsigned __int8 *model)
{
  int LocalClientIndex; // eax
  int v3; // eax
  cg_s *cgameGlob; // [esp+8h] [ebp-14h]
  centity_s *cent; // [esp+Ch] [ebp-10h]
  int entityNum; // [esp+18h] [ebp-4h]

  if ( !model && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_import.cpp", 3076, 0, "%s", "obj") )
    __debugbreak();
  entityNum = *((unsigned __int16 *)model + 3) - 1;
  if ( DObjIsServer((const DObj *)model) )
    return g_entities[entityNum].s.eType == 1;
  LocalClientIndex = DObjGetLocalClientIndex((const DObj *)model);
  cgameGlob = CG_GetLocalClientGlobals(LocalClientIndex);
  if ( cgameGlob->clientNum == entityNum )
  {
    cent = &cgameGlob->predictedPlayerEntity;
  }
  else
  {
    v3 = DObjGetLocalClientIndex((const DObj *)model);
    cent = CG_GetEntity(v3, entityNum);
  }
  return cent->nextState.eType == 1;
}

void __cdecl IKImport_GetPitchWeaponOffset(IKState *ikState, bool bIsPitchDirectionUp, float *weapOfsOut)
{
  float fraction; // [esp+0h] [ebp-208h]
  float fractiona; // [esp+0h] [ebp-208h]
  float fractionb; // [esp+0h] [ebp-208h]
  float fractionc; // [esp+0h] [ebp-208h]
  float weapOfsUpMoving[5][2][3]; // [esp+8h] [ebp-200h] BYREF
  float weapOfsDnIdle[5][2][3]; // [esp+80h] [ebp-188h] BYREF
  float weapOfsDnMoving[5][2][3]; // [esp+F8h] [ebp-110h] BYREF
  float weapOfsMoving[3]; // [esp+170h] [ebp-98h] BYREF
  float weapOfsIdle[3]; // [esp+17Ch] [ebp-8Ch] BYREF
  float weapOfsUpIdle[5][2][3]; // [esp+188h] [ebp-80h] BYREF
  IKWeaponOffsetType offsetType; // [esp+204h] [ebp-4h]

  *(_QWORD *)&weapOfsDnIdle[0][0][0] = __PAIR64__(LODWORD(FLOAT_N3_0), *(unsigned int *)&FLOAT_0_0);
  *(_QWORD *)&weapOfsDnIdle[0][0][2] = __PAIR64__(LODWORD(FLOAT_N2_0), LODWORD(FLOAT_7_0));
  *(_QWORD *)&weapOfsDnIdle[0][1][1] = __PAIR64__(LODWORD(FLOAT_10_0), LODWORD(FLOAT_N5_0));
  weapOfsDnIdle[1][0][0] = *(float *)&FLOAT_0_0;
  weapOfsDnIdle[1][0][1] = *(float *)&FLOAT_0_0;
  *(_QWORD *)&weapOfsDnIdle[1][0][2] = __PAIR64__(LODWORD(FLOAT_N5_0), *(unsigned int *)&FLOAT_0_0);
  *(_QWORD *)&weapOfsDnIdle[1][1][1] = __PAIR64__(LODWORD(FLOAT_5_0), LODWORD(FLOAT_N5_0));
  memset((void *)weapOfsDnIdle[2], 0, sizeof(const float[2][3]));
  *(_QWORD *)&weapOfsDnIdle[3][0][0] = __PAIR64__(LODWORD(FLOAT_N6_0), LODWORD(FLOAT_6_0));
  *(_QWORD *)&weapOfsDnIdle[3][0][2] = __PAIR64__(LODWORD(FLOAT_4_0), LODWORD(FLOAT_10_0));
  *(_QWORD *)&weapOfsDnIdle[3][1][1] = __PAIR64__(LODWORD(FLOAT_4_0), LODWORD(FLOAT_N8_0));
  memset((void *)weapOfsDnIdle[4], 0, sizeof(const float[2][3]));
  *(_QWORD *)&weapOfsUpIdle[0][0][0] = __PAIR64__(*(unsigned int *)&FLOAT_0_0, LODWORD(FLOAT_N4_0));
  *(_QWORD *)&weapOfsUpIdle[0][0][2] = __PAIR64__(LODWORD(FLOAT_N7_0), LODWORD(FLOAT_N2_0));
  *(_QWORD *)&weapOfsUpIdle[0][1][1] = __PAIR64__(LODWORD(FLOAT_N4_0), *(unsigned int *)&FLOAT_0_0);
  *(_QWORD *)&weapOfsUpIdle[1][0][0] = __PAIR64__(*(unsigned int *)&FLOAT_0_0, LODWORD(FLOAT_N3_0));
  *(_QWORD *)&weapOfsUpIdle[1][0][2] = __PAIR64__(LODWORD(FLOAT_N7_0), LODWORD(FLOAT_N2_0));
  *(_QWORD *)&weapOfsUpIdle[1][1][1] = __PAIR64__(LODWORD(FLOAT_N4_0), *(unsigned int *)&FLOAT_0_0);
  memset((void *)weapOfsUpIdle[2], 0, 72);
  memset((void *)weapOfsDnMoving, 0, 16);
  *(_QWORD *)&weapOfsDnMoving[0][1][1] = __PAIR64__(LODWORD(FLOAT_8_0), *(unsigned int *)&FLOAT_0_0);
  memset((void *)weapOfsDnMoving[1], 0, sizeof(const float[2][3]));
  *(_QWORD *)&weapOfsDnMoving[2][0][0] = __PAIR64__(*(unsigned int *)&FLOAT_0_0, LODWORD(FLOAT_4_0));
  *(_QWORD *)&weapOfsDnMoving[2][0][2] = __PAIR64__(LODWORD(FLOAT_4_0), *(unsigned int *)&FLOAT_0_0);
  weapOfsDnMoving[2][1][1] = *(float *)&FLOAT_0_0;
  weapOfsDnMoving[2][1][2] = *(float *)&FLOAT_0_0;
  *(_QWORD *)&weapOfsDnMoving[3][0][0] = __PAIR64__(LODWORD(FLOAT_N2_0), *(unsigned int *)&FLOAT_0_0);
  *(_QWORD *)&weapOfsDnMoving[3][0][2] = __PAIR64__(LODWORD(FLOAT_8_0), *(unsigned int *)&FLOAT_0_0);
  *(_QWORD *)&weapOfsDnMoving[3][1][1] = __PAIR64__(*(unsigned int *)&FLOAT_0_0, LODWORD(FLOAT_N4_0));
  memset((void *)weapOfsDnMoving[4], 0, sizeof(const float[2][3]));
  memset((void *)weapOfsUpMoving, 0, 24);
  *(_QWORD *)&weapOfsUpMoving[1][0][0] = __PAIR64__(*(unsigned int *)&FLOAT_0_0, LODWORD(FLOAT_N3_0));
  *(_QWORD *)&weapOfsUpMoving[1][0][2] = __PAIR64__(*(unsigned int *)&FLOAT_0_0, LODWORD(FLOAT_N2_0));
  weapOfsUpMoving[1][1][1] = *(float *)&FLOAT_0_0;
  weapOfsUpMoving[1][1][2] = *(float *)&FLOAT_0_0;
  *(_QWORD *)&weapOfsUpMoving[2][0][0] = __PAIR64__(*(unsigned int *)&FLOAT_0_0, LODWORD(FLOAT_4_0));
  *(_QWORD *)&weapOfsUpMoving[2][0][2] = __PAIR64__(LODWORD(FLOAT_4_0), *(unsigned int *)&FLOAT_0_0);
  weapOfsUpMoving[2][1][1] = *(float *)&FLOAT_0_0;
  weapOfsUpMoving[2][1][2] = *(float *)&FLOAT_0_0;
  memset((void *)weapOfsUpMoving[3], 0, 48);
  offsetType = ikState->weaponOffsetType;
  fraction = IKImport_GetLayerLerp(ikState, IKLAYER_CROUCH_ACTIVE);
  if ( bIsPitchDirectionUp )
  {
    Vec3Lerp(weapOfsUpIdle[offsetType][0], weapOfsUpIdle[offsetType][1], fraction, weapOfsIdle);
    fractiona = IKImport_GetLayerLerp(ikState, IKLAYER_CROUCH_ACTIVE);
    Vec3Lerp(weapOfsUpMoving[offsetType][0], weapOfsUpMoving[offsetType][1], fractiona, weapOfsMoving);
  }
  else
  {
    Vec3Lerp(weapOfsDnIdle[offsetType][0], weapOfsDnIdle[offsetType][1], fraction, weapOfsIdle);
    fractionb = IKImport_GetLayerLerp(ikState, IKLAYER_CROUCH_ACTIVE);
    Vec3Lerp(weapOfsDnMoving[offsetType][0], weapOfsDnMoving[offsetType][1], fractionb, weapOfsMoving);
  }
  fractionc = IKImport_GetLayerLerp(ikState, IKLAYER_NOT_MOVING_MED);
  Vec3Lerp(weapOfsMoving, weapOfsIdle, fractionc, weapOfsOut);
}

void __cdecl IKImport_InitCollisionCache(IKState *ikState)
{
  if ( (_S1_11 & 1) == 0 )
  {
    _S1_11 |= 1u;
    colgeom_visitor_t::colgeom_visitor_t(&dummy_2);
    dummy_2.__vftable = (colgeom_visitor_inlined_t<200>_vtbl *)&colgeom_visitor_inlined_t<200>::`vftable';
    colgeom_visitor_inlined_t<500>::reset(&dummy_2);
    atexit(IKImport_InitCollisionCache_::_2_::_dynamic_atexit_destructor_for__dummy__);
  }
  ikState->collisionCache.proximity_data.__vftable = dummy_2.__vftable;
  colgeom_visitor_inlined_t<500>::reset(&ikState->collisionCache.proximity_data);
  ikState->collisionCache.hitIndex = 0;
}

// local variable allocation has failed, the output may be wrong!
void  IKImport_Profiler(int a1@<ebp>, IKState *ikState)
{
  float v2; // [esp-4h] [ebp-C0h] BYREF
  int timed; // [esp+0h] [ebp-BCh]
  int time2; // [esp+4h] [ebp-B8h]
  float *v5; // [esp+8h] [ebp-B4h]
  float *v6; // [esp+Ch] [ebp-B0h]
  float v7; // [esp+10h] [ebp-ACh]
  float v8; // [esp+14h] [ebp-A8h]
  float v9; // [esp+18h] [ebp-A4h]
  float out[17]; // [esp+1Ch] [ebp-A0h] OVERLAPPED BYREF
  float v11; // [esp+60h] [ebp-5Ch]
  float v12; // [esp+64h] [ebp-58h]
  float v13; // [esp+68h] [ebp-54h]
  int v14; // [esp+6Ch] [ebp-50h]
  float v15; // [esp+70h] [ebp-4Ch]
  float v16; // [esp+74h] [ebp-48h]
  float v17; // [esp+78h] [ebp-44h]
  int v18; // [esp+7Ch] [ebp-40h]
  float *v19; // [esp+9Ch] [ebp-20h]
  float *v20; // [esp+A0h] [ebp-1Ch]
  int m; // [esp+A4h] [ebp-18h]
  int k; // [esp+A8h] [ebp-14h]
  unsigned intv23; // [esp+ACh] [ebp-10h]
  int j; // [esp+B0h] [ebp-Ch]
  int i; // [esp+B4h] [ebp-8h]
  int retaddr; // [esp+BCh] [ebp+0h]

  j = a1;
  i = retaddr;
  if ( bEnabled )
  {
    bEnabled = 0;
    v23 = Sys_Milliseconds();
    for ( k = 0; k < 10000; ++k )
    {
      for ( m = 1; m < 23; ++m )
      {
        v20 = ikState->matArrayPostIK[m - 1][0];
        v19 = ikState->matArrayPostIK[m][0];
        if ( v20 == &out[13]
          && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out") )
        {
          __debugbreak();
        }
        out[13] = *v20;
        out[14] = v20[4];
        out[15] = v20[8];
        out[16] = *(float *)&FLOAT_0_0;
        v11 = v20[1];
        v12 = v20[5];
        v13 = v20[9];
        v14 = *(unsigned int *)&FLOAT_0_0;
        v15 = v20[2];
        v16 = v20[6];
        v17 = v20[10];
        v18 = *(unsigned int *)&FLOAT_0_0;
        v7 = (float)((float)(*v19 * out[13]) + (float)(v19[1] * v11)) + (float)(v19[2] * v15);
        v8 = (float)((float)(*v19 * out[14]) + (float)(v19[1] * v12)) + (float)(v19[2] * v16);
        v9 = (float)((float)(*v19 * out[15]) + (float)(v19[1] * v13)) + (float)(v19[2] * v17);
        out[0] = *(float *)&FLOAT_0_0;
        out[1] = (float)((float)(v19[4] * out[13]) + (float)(v19[5] * v11)) + (float)(v19[6] * v15);
        out[2] = (float)((float)(v19[4] * out[14]) + (float)(v19[5] * v12)) + (float)(v19[6] * v16);
        out[3] = (float)((float)(v19[4] * out[15]) + (float)(v19[5] * v13)) + (float)(v19[6] * v17);
        out[4] = *(float *)&FLOAT_0_0;
        out[5] = (float)((float)(v19[8] * out[13]) + (float)(v19[9] * v11)) + (float)(v19[10] * v15);
        out[6] = (float)((float)(v19[8] * out[14]) + (float)(v19[9] * v12)) + (float)(v19[10] * v16);
        out[7] = (float)((float)(v19[8] * out[15]) + (float)(v19[9] * v13)) + (float)(v19[10] * v17);
        out[8] = *(float *)&FLOAT_0_0;
        v6 = v20 + 12;
        v5 = v19 + 12;
        v2 = v19[12] - v20[12];
        *(float *)&timed = v19[13] - v20[13];
        *(float *)&time2 = v19[14] - v20[14];
        ikMatrixTransformVector34(&v2, (const float (*)[4])&out[13], &out[9]);
        out[12] = FLOAT_1_0;
      }
    }
    Sys_Milliseconds();
  }
}

double __cdecl IKImport_GetMaxLayerLerp(IKState *ikState)
{
  float max; // [esp+0h] [ebp-8h]
  IKLayerNames i; // [esp+4h] [ebp-4h]

  max = *(float *)&FLOAT_0_0;
  for ( i = IKLAYER_CROUCH_ACTIVE; i < IKLAYER_COUNT; ++i )
  {
    if ( ikLayerRealLayers[i] )
    {
      if ( IKImport_GetLayerLerp(ikState, i) > max )
        max = ikState->lerpInfo[i].lerp;
    }
  }
  return max;
}

