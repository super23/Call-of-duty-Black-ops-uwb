#include "ik_process.h"

void __cdecl IK_GeneratePreIKMatrices(IKState *ikState, bool isLocalBones)
{
  IKImport_GetBoneMatrixArray(ikState, isLocalBones);
  if ( !isLocalBones
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_process.cpp", 64, 0, "%s", "isLocalBones") )
  {
    __debugbreak();
  }
}

// local variable allocation has failed, the output may be wrong!
void  IK_GenerateIKXformMatrices(float *a1@<ebp>, IKState *ikState, bool isLocalBones)
{
  _BYTE v3[76]; // [esp+18h] [ebp-1ECh] OVERLAPPED BYREF
  int v4; // [esp+64h] [ebp-1A0h]
  int v5; // [esp+68h] [ebp-19Ch]
  int v6; // [esp+6Ch] [ebp-198h]
  int v7; // [esp+70h] [ebp-194h]
  __int64 v8; // [esp+74h] [ebp-190h]
  __int64 v9; // [esp+7Ch] [ebp-188h]
  __int64 v10; // [esp+84h] [ebp-180h]
  __int64 v11; // [esp+8Ch] [ebp-178h]
  __int64 v12; // [esp+94h] [ebp-170h]
  __int64 v13; // [esp+9Ch] [ebp-168h]
  int v14; // [esp+A4h] [ebp-160h]
  int v15; // [esp+A8h] [ebp-15Ch]
  float *v16; // [esp+ACh] [ebp-158h]
  float *v17; // [esp+B0h] [ebp-154h]
  int v18; // [esp+B4h] [ebp-150h]
  float v19; // [esp+B8h] [ebp-14Ch] BYREF
  float v20; // [esp+BCh] [ebp-148h]
  float v21; // [esp+C0h] [ebp-144h]
  float preModelInverseMat[4][4]; // [esp+C4h] [ebp-140h]
  float v23[5]; // [esp+104h] [ebp-100h] BYREF
  float v24[4]; // [esp+118h] [ebp-ECh] BYREF
  float v25; // [esp+128h] [ebp-DCh]
  float v26; // [esp+12Ch] [ebp-D8h]
  float v27; // [esp+130h] [ebp-D4h]
  int v28; // [esp+134h] [ebp-D0h]
  float v29; // [esp+138h] [ebp-CCh]
  float v30; // [esp+13Ch] [ebp-C8h]
  float v31; // [esp+140h] [ebp-C4h]
  int v32; // [esp+144h] [ebp-C0h]
  _BYTE v33[140]; // [esp+158h] [ebp-ACh] OVERLAPPED BYREF
  float *v34; // [esp+1E4h] [ebp-20h]
  float *v35; // [esp+1E8h] [ebp-1Ch]
  int i; // [esp+1ECh] [ebp-18h]
  float (*pPostModelMat)[4][4]; // [esp+1F0h] [ebp-14h]
  float (*xformMat)[4][4]; // [esp+1F8h] [ebp-Ch]
  float maxLayerLerp; // [esp+1FCh] [ebp-8h]
  float retaddr; // [esp+204h] [ebp+0h]

  xformMat = (float (*)[4][4])a1;
  maxLayerLerp = retaddr;
  *(float *)&pPostModelMat = IKImport_GetMaxLayerLerp(ikState);
  i = (int)ikState->matArrayXforms;
  v35 = (*ikState->matArrayPreIK)[0];
  v34 = (*ikState->matArrayPostIK)[0];
  *(unsigned int *)&v33[136] = 0;
  while ( *(int *)&v33[136] < 23 )
  {
    v33[135] = ikState->ikBoneToObjBone[*(unsigned int *)&v33[136]] != 161;
    if ( v33[135] )
    {
      if ( isLocalBones )
      {
        if ( v35 == &v19
          && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out") )
        {
          __debugbreak();
        }
        v19 = *v35;
        v20 = v35[4];
        v21 = v35[8];
        preModelInverseMat[0][0] = *(float *)&FLOAT_0_0;
        preModelInverseMat[0][1] = v35[1];
        preModelInverseMat[0][2] = v35[5];
        preModelInverseMat[0][3] = v35[9];
        preModelInverseMat[1][0] = *(float *)&FLOAT_0_0;
        preModelInverseMat[1][1] = v35[2];
        preModelInverseMat[1][2] = v35[6];
        preModelInverseMat[1][3] = v35[10];
        preModelInverseMat[2][0] = *(float *)&FLOAT_0_0;
        *(float *)i = (float)((float)(*v34 * v19) + (float)(v34[1] * preModelInverseMat[0][1]))
                    + (float)(v34[2] * preModelInverseMat[1][1]);
        *(float *)(i + 4) = (float)((float)(*v34 * v20) + (float)(v34[1] * preModelInverseMat[0][2]))
                          + (float)(v34[2] * preModelInverseMat[1][2]);
        *(float *)(i + 8) = (float)((float)(*v34 * v21) + (float)(v34[1] * preModelInverseMat[0][3]))
                          + (float)(v34[2] * preModelInverseMat[1][3]);
        *(unsigned int *)(i + 12) = *(unsigned int *)&FLOAT_0_0;
        *(float *)(i + 16) = (float)((float)(v34[4] * v19) + (float)(v34[5] * preModelInverseMat[0][1]))
                           + (float)(v34[6] * preModelInverseMat[1][1]);
        *(float *)(i + 20) = (float)((float)(v34[4] * v20) + (float)(v34[5] * preModelInverseMat[0][2]))
                           + (float)(v34[6] * preModelInverseMat[1][2]);
        *(float *)(i + 24) = (float)((float)(v34[4] * v21) + (float)(v34[5] * preModelInverseMat[0][3]))
                           + (float)(v34[6] * preModelInverseMat[1][3]);
        *(unsigned int *)(i + 28) = *(unsigned int *)&FLOAT_0_0;
        *(float *)(i + 32) = (float)((float)(v34[8] * v19) + (float)(v34[9] * preModelInverseMat[0][1]))
                           + (float)(v34[10] * preModelInverseMat[1][1]);
        *(float *)(i + 36) = (float)((float)(v34[8] * v20) + (float)(v34[9] * preModelInverseMat[0][2]))
                           + (float)(v34[10] * preModelInverseMat[1][2]);
        *(float *)(i + 40) = (float)((float)(v34[8] * v21) + (float)(v34[9] * preModelInverseMat[0][3]))
                           + (float)(v34[10] * preModelInverseMat[1][3]);
        *(unsigned int *)(i + 44) = *(unsigned int *)&FLOAT_0_0;
        v18 = i + 48;
        v17 = v35 + 12;
        v16 = v34 + 12;
        *(float *)(i + 48) = v34[12] - v35[12];
        *(float *)(v18 + 4) = v16[1] - v17[1];
        *(float *)(v18 + 8) = v16[2] - v17[2];
        *(float *)(i + 60) = FLOAT_1_0;
      }
      else
      {
        ikCalcBoneModelMatrix(ikState, *(int *)&v33[136], (*ikState->matArrayPreIK)[0], (float (*)[4])&v33[64]);
        ikCalcBoneModelMatrix(ikState, *(int *)&v33[136], (*ikState->matArrayPostIK)[0], (float (*)[4])v33);
        if ( &v33[64] == (_BYTE *)v24
          && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out") )
        {
          __debugbreak();
        }
        v24[0] = *(float *)&v33[64];
        v24[1] = *(float *)&v33[80];
        v24[2] = *(float *)&v33[96];
        v24[3] = *(float *)&FLOAT_0_0;
        v25 = *(float *)&v33[68];
        v26 = *(float *)&v33[84];
        v27 = *(float *)&v33[100];
        v28 = *(unsigned int *)&FLOAT_0_0;
        v29 = *(float *)&v33[72];
        v30 = *(float *)&v33[88];
        v31 = *(float *)&v33[104];
        v32 = *(unsigned int *)&FLOAT_0_0;
        *(float *)i = (float)((float)(*(float *)v33 * *(float *)&v33[64])
                            + (float)(*(float *)&v33[4] * *(float *)&v33[68]))
                    + (float)(*(float *)&v33[8] * *(float *)&v33[72]);
        *(float *)(i + 4) = (float)((float)(*(float *)v33 * v24[1]) + (float)(*(float *)&v33[4] * v26))
                          + (float)(*(float *)&v33[8] * v30);
        *(float *)(i + 8) = (float)((float)(*(float *)v33 * v24[2]) + (float)(*(float *)&v33[4] * v27))
                          + (float)(*(float *)&v33[8] * v31);
        *(unsigned int *)(i + 12) = *(unsigned int *)&FLOAT_0_0;
        *(float *)(i + 16) = (float)((float)(*(float *)&v33[16] * v24[0]) + (float)(*(float *)&v33[20] * v25))
                           + (float)(*(float *)&v33[24] * v29);
        *(float *)(i + 20) = (float)((float)(*(float *)&v33[16] * v24[1]) + (float)(*(float *)&v33[20] * v26))
                           + (float)(*(float *)&v33[24] * v30);
        *(float *)(i + 24) = (float)((float)(*(float *)&v33[16] * v24[2]) + (float)(*(float *)&v33[20] * v27))
                           + (float)(*(float *)&v33[24] * v31);
        *(unsigned int *)(i + 28) = *(unsigned int *)&FLOAT_0_0;
        *(float *)(i + 32) = (float)((float)(*(float *)&v33[32] * v24[0]) + (float)(*(float *)&v33[36] * v25))
                           + (float)(*(float *)&v33[40] * v29);
        *(float *)(i + 36) = (float)((float)(*(float *)&v33[32] * v24[1]) + (float)(*(float *)&v33[36] * v26))
                           + (float)(*(float *)&v33[40] * v30);
        *(float *)(i + 40) = (float)((float)(*(float *)&v33[32] * v24[2]) + (float)(*(float *)&v33[36] * v27))
                           + (float)(*(float *)&v33[40] * v31);
        *(unsigned int *)(i + 44) = *(unsigned int *)&FLOAT_0_0;
        LODWORD(v23[4]) = &v33[112];
        LODWORD(v23[3]) = &v33[48];
        v23[0] = *(float *)&v33[48] - *(float *)&v33[112];
        v23[1] = *(float *)&v33[52] - *(float *)&v33[116];
        v23[2] = *(float *)&v33[56] - *(float *)&v33[120];
        ikMatrixTransformVector34(v23, (const float (*)[4])v24, (float *)(i + 48));
        *(float *)(i + 60) = FLOAT_1_0;
      }
      if ( *(float *)&pPostModelMat < 1.0 )
      {
        v15 = i;
        v5 = *(unsigned int *)i;
        v6 = *(unsigned int *)(i + 4);
        v7 = *(unsigned int *)(i + 8);
        LODWORD(v8) = *(unsigned int *)(i + 12);
        v4 = i + 16;
        HIDWORD(v8) = *(unsigned int *)(i + 16);
        v9 = *(_QWORD *)(i + 20);
        LODWORD(v10) = *(unsigned int *)(i + 28);
        *(unsigned int *)&v3[72] = i + 32;
        HIDWORD(v10) = *(unsigned int *)(i + 32);
        v11 = *(_QWORD *)(i + 36);
        LODWORD(v12) = *(unsigned int *)(i + 44);
        *(unsigned int *)&v3[68] = i + 48;
        HIDWORD(v12) = *(unsigned int *)(i + 48);
        v13 = *(_QWORD *)(i + 52);
        v14 = *(unsigned int *)(i + 60);
        *(unsigned int *)v3 = v5;
        *(unsigned int *)&v3[4] = v6;
        *(unsigned int *)&v3[8] = v7;
        *(_QWORD *)&v3[12] = v8;
        *(_QWORD *)&v3[20] = v9;
        *(_QWORD *)&v3[28] = v10;
        *(_QWORD *)&v3[36] = v11;
        *(_QWORD *)&v3[44] = v12;
        *(_QWORD *)&v3[52] = v13;
        *(unsigned int *)&v3[60] = v14;
        ikMatrixLerp44((float (*)[4])ikIdentityMatrix44, (float (*)[4])v3, *(float *)&pPostModelMat, (float (*)[4])i);
      }
      ikNormalizedMatrixAssert_func((float (*)[4])i);
    }
    else
    {
      ikMatrixIdentity44((float (*)[4])i);
    }
    ++*(unsigned int *)&v33[136];
    i += 64;
    v35 += 16;
    v34 += 16;
  }
}

char __cdecl IK_HasAnimatedBones(IKState *ikState)
{
  int i; // [esp+4h] [ebp-4h]

  for ( i = 1; i < 23; ++i )
  {
    if ( ikState->ikBoneToObjBone[i] != 161 && IK_IsCalcBone(ikState, i) )
      return 1;
  }
  return 0;
}

void  IK_Process(int a1@<ebp>, IKState *ikState, bool isLocalBones)
{
  void *v3; // esp
  int v4; // [esp-1CC0h] [ebp-1CCCh] BYREF
  int v5; // [esp-1700h] [ebp-170Ch] BYREF
  int v6; // [esp-1140h] [ebp-114Ch] BYREF
  int v7; // [esp-B80h] [ebp-B8Ch] BYREF
  int v8; // [esp-5C0h] [ebp-5CCh] BYREF
  _BYTE v9[12]; // [esp+0h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]

  *(unsigned int *)v9 = a1;
  *(unsigned int *)&v9[4] = retaddr;
  v3 = alloca(7368);
  if ( ikState->matArrayPostIK
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_process.cpp",
          184,
          0,
          "%s",
          "!ikState->matArrayPostIK") )
  {
    __debugbreak();
  }
  ikState->matArrayPostIK = (float (*)[4][4])&v8;
  ikState->matArrayPreIK = (float (*)[4][4])&v7;
  ikState->matArrayXforms = (float (*)[4][4])&v6;
  ikState->matArrayCache = (float (*)[4][4])&v5;
  ikState->matArrayCachePre = (float (*)[4][4])&v4;
  ikState->cacheActive = 1;
  ikState->bHasActiveLayers = 1;
  if ( ikState->cacheActive )
  {
    memset((unsigned __int8 *)ikState->matArrayCache, 0, 0x5C0u);
    ikState->cacheActive = 0;
  }
  memset((unsigned __int8 *)ikState->matArrayCachePre, 0, 0x5C0u);
  if ( IK_HasAnimatedBones(ikState) )
  {
    ikState->modifiedIKBones = 0;
    IK_GeneratePreIKMatrices(ikState, isLocalBones);
    memcpy(ikState->matArrayPostIK, ikState->matArrayPreIK, 0x5C0u);
    if ( !ikState->bJointVarsValid )
      IK_GetJointVars((IKJointBones *)v9, ikState);
    IKImport_Profiler((int)v9, ikState);
    IK_ProcessLayers((int)v9, ikState);
    if ( ikState->bHasActiveLayers )
      IK_GenerateIKXformMatrices((float *)v9, ikState, isLocalBones);
    IKImport_ApplyIKToSkeleton(ikState, isLocalBones);
    if ( IKImport_GetVar_IK_Debug() == 2 )
      IKImport_DrawDebugSkeleton((int)v9, ikState);
  }
  ikState->matArrayPostIK = 0;
  ikState->matArrayPreIK = 0;
  ikState->matArrayXforms = 0;
  ikState->matArrayCache = 0;
  ikState->matArrayCachePre = 0;
}

