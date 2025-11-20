#include "xanim_calc.h"

void __cdecl XAnimSetTime(XAnimTree_s *tree, unsigned int animIndex, float time, unsigned __int16 cmdIndex)
{
  char *AnimDebugName; // eax
  const char *v5; // eax
  char *v6; // eax
  const char *v7; // eax
  bool bLoop; // [esp+8h] [ebp-28h]
  bool v9; // [esp+10h] [ebp-20h]
  float v10; // [esp+18h] [ebp-18h]
  float v11; // [esp+1Ch] [ebp-14h]
  XAnimInfo *info; // [esp+24h] [ebp-Ch]
  unsigned int infoIndex; // [esp+28h] [ebp-8h]
  const XAnimEntry *anim; // [esp+2Ch] [ebp-4h]

  if ( !tree && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim.cpp", 4807, 0, "%s", "tree") )
    __debugbreak();
  if ( !tree->anims
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim.cpp", 4808, 0, "%s", "tree->anims") )
  {
    __debugbreak();
  }
  if ( animIndex >= tree->anims->size
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim.cpp",
          4809,
          0,
          "%s\n\t(animIndex) = %i",
          "(animIndex < tree->anims->size)",
          animIndex) )
  {
    __debugbreak();
  }
  infoIndex = XAnimGetInfoIndex(tree, animIndex);
  if ( infoIndex )
  {
    anim = &tree->anims->entries[animIndex];
    if ( !XAnimHasTime(tree->anims, animIndex) )
    {
      AnimDebugName = XAnimGetAnimDebugName(tree->anims, animIndex);
      v5 = va("Anim name: '%s', time: %f", AnimDebugName, time);
      if ( !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim.cpp",
              4816,
              0,
              "%s\n\t%s",
              "XAnimHasTime( tree->anims, animIndex )",
              v5) )
        __debugbreak();
    }
    if ( Demo_IsPlaying() )
    {
      if ( (float)(time - 1.0) < 0.0 )
        v11 = time;
      else
        v11 = FLOAT_1_0;
      if ( (float)(0.0 - time) < 0.0 )
        v10 = v11;
      else
        v10 = *(float *)&FLOAT_0_0;
      time = v10;
    }
    if ( (time < 0.0 || time > 1.0)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim.cpp",
            4828,
            0,
            "%s\n\t(time) = %g",
            "(time >= 0.0f && time <= 1.0f)",
            time) )
    {
      __debugbreak();
    }
    if ( XAnimIsLooped(tree->anims, animIndex) )
      v9 = time < 1.0;
    else
      v9 = time <= 1.0;
    if ( !v9 )
    {
      bLoop = anim->parts->bLoop;
      v6 = XAnimGetAnimDebugName(tree->anims, animIndex);
      v7 = va("name: '%s', time: %f, parts->bLoop: %d", v6, time, bLoop);
      if ( !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim.cpp",
              4829,
              0,
              "%s\n\t%s",
              "XAnimIsLooped( tree->anims, animIndex ) ? (time < 1.0f) : (time <= 1.0f)",
              v7) )
        __debugbreak();
    }
    if ( infoIndex >= 0x1000
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim.cpp",
            4831,
            0,
            "%s\n\t(infoIndex) = %i",
            "(infoIndex && (infoIndex < 4096))",
            infoIndex) )
    {
      __debugbreak();
    }
    info = &g_xAnimInfo[infoIndex];
    if ( *(unsigned int *)(*((unsigned int *)NtCurrentTeb()->ThreadLocalStoragePointer + _tls_index) + 8) )
    {
      if ( *(unsigned int *)(*(unsigned int *)(*((unsigned int *)NtCurrentTeb()->ThreadLocalStoragePointer + _tls_index) + 8) + 184) )
        (*(void (__cdecl **)(unsigned int, unsigned int))(*(unsigned int *)(*((unsigned int *)NtCurrentTeb()->ThreadLocalStoragePointer
                                                          + _tls_index)
                                                        + 8)
                                            + 184))(
          cmdIndex,
          (unsigned __int16)infoIndex);
    }
    info->state.currentAnimTime = time;
    info->state.cycleCount = 0;
    info->state.oldTime = time;
    info->state.oldCycleCount = 0;
    info->notifyIndex = -1;
  }
}

void __cdecl XAnim_SetTime(float time, int frameCount, XAnimTime *animTime)
{
  animTime->time = time;
  animTime->frameFrac = (float)frameCount * time;
  animTime->frameIndex = (int)animTime->frameFrac;
}

void  DObjCalcAnim(int a1@<ebp>, const DObj *obj, int *partBits)
{
  void *v3; // esp
  const char *v4; // eax
  const char *v5; // eax
  float v6; // [esp+30h] [ebp-7904h]
  float v7; // [esp+34h] [ebp-7900h]
  float v8; // [esp+38h] [ebp-78FCh]
  __int16 *localQuats; // [esp+58h] [ebp-78DCh]
  int mm; // [esp+80h] [ebp-78B4h]
  int v11; // [esp+80h] [ebp-78B4h]
  XModel *v12; // [esp+84h] [ebp-78B0h]
  int kk; // [esp+88h] [ebp-78ACh]
  XModel **localModels; // [esp+8Ch] [ebp-78A8h]
  signed int v15; // [esp+90h] [ebp-78A4h]
  XAnimTree_s *localTree; // [esp+94h] [ebp-78A0h]
  int jj; // [esp+98h] [ebp-789Ch]
  int ii; // [esp+9Ch] [ebp-7898h]
  signed int n; // [esp+BCh] [ebp-7878h]
  DObjAnimMat *mat; // [esp+C0h] [ebp-7874h]
  DObjAnimMat *v21; // [esp+C0h] [ebp-7874h]
  char v22; // [esp+C7h] [ebp-786Dh]
  int m; // [esp+C8h] [ebp-786Ch]
  int k; // [esp+CCh] [ebp-7868h]
  int j; // [esp+D0h] [ebp-7864h]
  int i; // [esp+D4h] [ebp-7860h]
  DSkel *p_skel; // [esp+E0h] [ebp-7854h]
  XAnimInfo *AnimInfo; // [esp+E4h] [ebp-7850h]
  XAnimCalcAnimInfo v29; // [esp+E8h] [ebp-784Ch] BYREF
  int v30; // [esp+7928h] [ebp-Ch]
  void *v31; // [esp+792Ch] [ebp-8h]
  void *retaddr; // [esp+7934h] [ebp+0h]

  v30 = a1;
  v31 = retaddr;
  v3 = alloca(30988);
  XAnimCalcAnimInfo::XAnimCalcAnimInfo(&v29);
  if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp", 2161, 0, "%s", "obj") )
    __debugbreak();
  p_skel = &obj->skel;
  if ( obj == (const DObj *)-20
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp", 2164, 0, "%s", "skel") )
  {
    __debugbreak();
  }
  for ( i = 0; i < 5; ++i )
    v29.animPartBits.array[i] = partBits[i];
  for ( j = 0; j < 5; ++j )
    v29.animPartBits.array[j] = ~v29.animPartBits.array[j];
  for ( k = 0; k < 5; ++k )
    v29.animPartBits.array[k] |= p_skel->partBits.anim[k];
  v29.bLeftHandGripIKActive = 0;
  for ( m = 0; m < 5; ++m )
  {
    if ( v29.animPartBits.array[m] != -1 )
    {
      v22 = 0;
      goto LABEL_22;
    }
  }
  v22 = 1;
LABEL_22:
  if ( !v22 )
  {
    mat = obj->skel.mat;
    for ( n = 0; n < obj->numBones; ++n )
    {
      if ( bitarray<160>::testBit((bitarray<160> *)p_skel, n) )
      {
        if ( ((LODWORD(mat[n].quat[0]) & 0x7F800000) == 0x7F800000
           || (LODWORD(mat[n].quat[1]) & 0x7F800000) == 0x7F800000
           || (LODWORD(mat[n].quat[2]) & 0x7F800000) == 0x7F800000
           || (LODWORD(mat[n].quat[3]) & 0x7F800000) == 0x7F800000)
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp",
                2197,
                0,
                "%s",
                "!IS_NAN((mat[bone].quat)[0]) && !IS_NAN((mat[bone].quat)[1]) && !IS_NAN((mat[bone].quat)[2]) && !IS_NAN("
                "(mat[bone].quat)[3])") )
        {
          __debugbreak();
        }
        if ( ((LODWORD(mat[n].trans[0]) & 0x7F800000) == 0x7F800000
           || (LODWORD(mat[n].trans[1]) & 0x7F800000) == 0x7F800000
           || (LODWORD(mat[n].trans[2]) & 0x7F800000) == 0x7F800000)
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp",
                2198,
                0,
                "%s",
                "!IS_NAN((mat[bone].trans)[0]) && !IS_NAN((mat[bone].trans)[1]) && !IS_NAN((mat[bone].trans)[2])") )
        {
          __debugbreak();
        }
      }
    }
    for ( ii = 0; ii < 5; ++ii )
      p_skel->partBits.anim[ii] |= partBits[ii];
    for ( jj = 0; jj < 5; ++jj )
      v29.ignorePartBits.array[jj] = v29.animPartBits.array[jj];
    localTree = obj->localTree;
    if ( obj->localTree && localTree->children )
    {
      bitarray<160>::setBit(&v29.ignorePartBits, 0x9Fu);
      AnimInfo = GetAnimInfo(localTree->children);
      XAnimCalc(obj, AnimInfo, 1.0, 1, 0, &v29, 0, obj->skel.mat);
    }
    if ( v29.bLeftHandGripIKActive )
      obj->flags |= 0x10u;
    else
      obj->flags &= ~0x10u;
    v15 = 0;
    v21 = obj->skel.mat;
    localModels = obj->localModels;
    for ( kk = 0; kk < obj->numModels; ++kk )
    {
      v12 = localModels[kk];
      for ( mm = v12->numRootBones; mm; --mm )
      {
        if ( bitarray<160>::testBit(&v29.animPartBits, v15) )
        {
          if ( bitarray<160>::testBit((bitarray<160> *)p_skel, v15) )
          {
            if ( v15 >= obj->numBones )
            {
              v4 = va("model %d: '%s'", kk, localModels[kk]->name);
              if ( !Assert_MyHandler(
                      "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp",
                      2289,
                      0,
                      "%s\n\t%s",
                      "boneIndex < obj->numBones",
                      v4) )
                __debugbreak();
            }
            if ( (LODWORD(v21->quat[0]) & 0x7F800000) == 0x7F800000
              || (LODWORD(v21->quat[1]) & 0x7F800000) == 0x7F800000
              || (LODWORD(v21->quat[2]) & 0x7F800000) == 0x7F800000
              || (LODWORD(v21->quat[3]) & 0x7F800000) == 0x7F800000 )
            {
              v5 = va("model %d: '%s'", kk, localModels[kk]->name);
              if ( !Assert_MyHandler(
                      "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp",
                      2290,
                      0,
                      "%s\n\t%s",
                      "(!IS_NAN((mat->quat)[0]) && !IS_NAN((mat->quat)[1]) && !IS_NAN((mat->quat)[2]) && !IS_NAN((mat->quat)[3]))",
                      v5) )
                __debugbreak();
            }
            if ( ((LODWORD(v21->trans[0]) & 0x7F800000) == 0x7F800000
               || (LODWORD(v21->trans[1]) & 0x7F800000) == 0x7F800000
               || (LODWORD(v21->trans[2]) & 0x7F800000) == 0x7F800000)
              && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp",
                    2291,
                    0,
                    "%s",
                    "!IS_NAN((mat->trans)[0]) && !IS_NAN((mat->trans)[1]) && !IS_NAN((mat->trans)[2])") )
            {
              __debugbreak();
            }
          }
        }
        else
        {
          v21->quat[0] = *(float *)&FLOAT_0_0;
          v21->quat[1] = *(float *)&FLOAT_0_0;
          v21->quat[2] = *(float *)&FLOAT_0_0;
          v21->quat[3] = FLOAT_1_0;
          v21->trans[0] = *(float *)&FLOAT_0_0;
          v21->trans[1] = *(float *)&FLOAT_0_0;
          v21->trans[2] = *(float *)&FLOAT_0_0;
          v21->transWeight = *(float *)&FLOAT_0_0;
        }
        ++v21;
        ++v15;
      }
      localQuats = v12->localQuats;
      v11 = v12->numBones - v12->numRootBones;
      while ( v11 )
      {
        if ( bitarray<160>::testBit(&v29.animPartBits, v15) )
        {
          if ( bitarray<160>::testBit((bitarray<160> *)p_skel, v15) )
          {
            if ( ((LODWORD(v21->quat[0]) & 0x7F800000) == 0x7F800000
               || (LODWORD(v21->quat[1]) & 0x7F800000) == 0x7F800000
               || (LODWORD(v21->quat[2]) & 0x7F800000) == 0x7F800000
               || (LODWORD(v21->quat[3]) & 0x7F800000) == 0x7F800000)
              && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp",
                    2316,
                    0,
                    "%s",
                    "!IS_NAN((mat->quat)[0]) && !IS_NAN((mat->quat)[1]) && !IS_NAN((mat->quat)[2]) && !IS_NAN((mat->quat)[3])") )
            {
              __debugbreak();
            }
            if ( ((LODWORD(v21->trans[0]) & 0x7F800000) == 0x7F800000
               || (LODWORD(v21->trans[1]) & 0x7F800000) == 0x7F800000
               || (LODWORD(v21->trans[2]) & 0x7F800000) == 0x7F800000)
              && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp",
                    2317,
                    0,
                    "%s",
                    "!IS_NAN((mat->trans)[0]) && !IS_NAN((mat->trans)[1]) && !IS_NAN((mat->trans)[2])") )
            {
              __debugbreak();
            }
          }
        }
        else
        {
          v6 = 0.000030518509 * (float)localQuats[1];
          v7 = 0.000030518509 * (float)localQuats[2];
          v8 = 0.000030518509 * (float)localQuats[3];
          v21->quat[0] = 0.000030518509 * (float)*localQuats;
          v21->quat[1] = v6;
          v21->quat[2] = v7;
          v21->quat[3] = v8;
          v21->trans[0] = *(float *)&FLOAT_0_0;
          v21->trans[1] = *(float *)&FLOAT_0_0;
          v21->trans[2] = *(float *)&FLOAT_0_0;
          v21->transWeight = *(float *)&FLOAT_0_0;
        }
        --v11;
        ++v21;
        ++v15;
        localQuats += 4;
      }
    }
  }
}

void __cdecl XAnimCalc(
        const DObj *obj,
        XAnimInfo *info,
        float weightScale,
        bool bClear,
        bool bNormQuat,
        XAnimCalcAnimInfo *animInfo,
        int rotTransArrayIndex,
        DObjAnimMat *rotTransArray)
{
  XAnimInfo *firstInfo; // [esp+18h] [ebp-28h]
  XAnimInfo *secondInfo; // [esp+1Ch] [ebp-24h]
  XAnimInfo *secondInfoa; // [esp+1Ch] [ebp-24h]
  XAnimTree_s *tree; // [esp+20h] [ebp-20h]
  unsigned int secondInfoIndex; // [esp+24h] [ebp-1Ch]
  DObjAnimMat *calcBuffer; // [esp+28h] [ebp-18h]
  unsigned int firstInfoIndex; // [esp+2Ch] [ebp-14h]
  bool secondChildFound; // [esp+32h] [ebp-Eh]
  bool additiveChildExists; // [esp+33h] [ebp-Dh]
  int allocedCalcBuffer; // [esp+34h] [ebp-Ch]
  float weight; // [esp+38h] [ebp-8h]
  float weighta; // [esp+38h] [ebp-8h]
  float firstWeight; // [esp+3Ch] [ebp-4h]

  tree = obj->localTree;
  if ( !obj->localTree
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp", 1853, 0, "%s", "tree") )
  {
    __debugbreak();
  }
  if ( !tree->anims
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp", 1854, 0, "%s", "tree->anims") )
  {
    __debugbreak();
  }
  if ( info->animToModel )
  {
    if ( bClear )
      XAnimClearRotTransArray(obj, rotTransArray, animInfo);
    XAnimCalcLeaf(info, weightScale, rotTransArray, animInfo);
  }
  else
  {
    firstWeight = *(float *)&FLOAT_0_0;
    firstInfo = 0;
    for ( firstInfoIndex = info->children; firstInfoIndex; firstInfoIndex = firstInfo->next )
    {
      firstInfo = GetAnimInfo(firstInfoIndex);
      firstWeight = firstInfo->state.weight;
      if ( firstWeight != 0.0 )
      {
        if ( IsInfoAdditive(firstInfo) )
          firstInfoIndex = 0;
        break;
      }
    }
    if ( firstInfoIndex )
    {
      if ( !firstInfo
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp", 1940, 0, "%s", "firstInfo") )
      {
        __debugbreak();
      }
      secondChildFound = 0;
      calcBuffer = 0;
      allocedCalcBuffer = 0;
      for ( secondInfoIndex = firstInfo->next; secondInfoIndex; secondInfoIndex = secondInfo->next )
      {
        secondInfo = GetAnimInfo(secondInfoIndex);
        weight = secondInfo->state.weight;
        if ( weight != 0.0 )
        {
          if ( IsInfoAdditive(secondInfo) )
            break;
          if ( !secondChildFound )
          {
            secondChildFound = 1;
            if ( bClear )
            {
              calcBuffer = rotTransArray;
              XAnimCalc(obj, firstInfo, firstWeight, 1, 1, animInfo, rotTransArrayIndex, rotTransArray);
            }
            else
            {
              calcBuffer = XAnimGetCalcBuffer(animInfo, obj, &rotTransArrayIndex);
              if ( !calcBuffer )
                return;
              allocedCalcBuffer = 1;
              XAnimCalc(obj, firstInfo, firstWeight, 1, 1, animInfo, rotTransArrayIndex, calcBuffer);
            }
          }
          if ( !calcBuffer
            && !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp",
                  2002,
                  0,
                  "%s",
                  "calcBuffer") )
          {
            __debugbreak();
          }
          XAnimCalc(obj, secondInfo, weight, 0, 1, animInfo, rotTransArrayIndex, calcBuffer);
        }
      }
      if ( secondChildFound )
      {
        if ( !bNormQuat
          && !bClear
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp",
                2013,
                0,
                "%s",
                "bNormQuat || bClear") )
        {
          __debugbreak();
        }
        if ( bNormQuat )
        {
          if ( bClear )
            XAnimNormalizeRotScaleTransArray(obj->numBones, animInfo, weightScale, rotTransArray);
          else
            XAnimMadRotTransArray(obj->numBones, animInfo, weightScale, calcBuffer, rotTransArray);
        }
        else
        {
          XAnimScaleRotTransArray(obj->numBones, animInfo, rotTransArray);
        }
      }
      else
      {
        XAnimCalc(obj, firstInfo, weightScale, bClear, bNormQuat, animInfo, rotTransArrayIndex, rotTransArray);
      }
      if ( secondInfoIndex )
      {
        if ( allocedCalcBuffer || (calcBuffer = XAnimGetCalcBuffer(animInfo, obj, &rotTransArrayIndex)) != 0 )
        {
          additiveChildExists = 0;
          while ( secondInfoIndex )
          {
            secondInfoa = GetAnimInfo(secondInfoIndex);
            if ( !IsInfoAdditive(secondInfoa)
              && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp",
                    2079,
                    0,
                    "%s",
                    "IsInfoAdditive( secondInfo )") )
            {
              __debugbreak();
            }
            weighta = secondInfoa->state.weight;
            if ( weighta != 0.0 )
            {
              XAnimCalc(obj, secondInfoa, weighta, !additiveChildExists, 1, animInfo, rotTransArrayIndex, calcBuffer);
              additiveChildExists = 1;
            }
            secondInfoIndex = secondInfoa->next;
          }
          if ( additiveChildExists )
            XAnimApplyAdditives(rotTransArray, calcBuffer, weightScale, obj->numBones, animInfo);
        }
      }
    }
    else if ( bClear )
    {
      XAnimClearRotTransArray(obj, rotTransArray, animInfo);
    }
  }
}

bool __cdecl IsInfoAdditive(const XAnimInfo *info)
{
  if ( !info && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp", 196, 0, "%s", "info") )
    __debugbreak();
  return !info->animToModel && (info->animParent.flags & 0x10) != 0;
}

void __cdecl XAnimClearRotTransArray(const DObj *obj, DObjAnimMat *rotTransArray, XAnimCalcAnimInfo *info)
{
  signed int modelPartIndex; // [esp+4h] [ebp-4h]

  for ( modelPartIndex = 0; modelPartIndex < obj->numBones; ++modelPartIndex )
  {
    if ( !bitarray<160>::testBit(&info->ignorePartBits, modelPartIndex) )
    {
      rotTransArray->quat[0] = *(float *)&FLOAT_0_0;
      rotTransArray->quat[1] = *(float *)&FLOAT_0_0;
      rotTransArray->quat[2] = *(float *)&FLOAT_0_0;
      rotTransArray->quat[3] = *(float *)&FLOAT_0_0;
      rotTransArray->transWeight = *(float *)&FLOAT_0_0;
      rotTransArray->trans[0] = *(float *)&FLOAT_0_0;
      rotTransArray->trans[1] = *(float *)&FLOAT_0_0;
      rotTransArray->trans[2] = *(float *)&FLOAT_0_0;
    }
    ++rotTransArray;
  }
}

void __cdecl XAnimCalcLeaf(XAnimInfo *info, float weightScale, DObjAnimMat *rotTransArray, XAnimCalcAnimInfo *animInfo)
{
  unsigned __int16 animToModel; // [esp+18h] [ebp-8h]
  XAnimParts *parts; // [esp+1Ch] [ebp-4h]

  parts = info->parts;
  animToModel = info->animToModel;
  if ( !parts && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp", 1567, 0, "%s", "parts") )
    __debugbreak();
  if ( parts->bLeftHandGripIK )
    animInfo->bLeftHandGripIKActive = 1;
  XAnimCalcLeafInternal(
    parts,
    animToModel,
    info->state.currentAnimTime,
    weightScale,
    rotTransArray,
    &animInfo->animPartBits,
    &animInfo->ignorePartBits);
}

void __cdecl XAnimCalcLeafInternal(
        const XAnimParts *parts,
        unsigned __int16 animToModelIndex,
        float time,
        float weightScale,
        DObjAnimMat *rotTransArray,
        bitarray<160> *animPartBits,
        bitarray<160> *ignorePartBits)
{
  const char *v7; // eax
  bool v8; // [esp+14h] [ebp-14h]
  const XAnimToXModel *animToModel; // [esp+20h] [ebp-8h]
  int i; // [esp+24h] [ebp-4h]

  if ( !animToModelIndex
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp", 1481, 0, "%s", "animToModelIndex") )
  {
    __debugbreak();
  }
  animToModel = (const XAnimToXModel *)SL_ConvertToString(animToModelIndex, SCRIPTINSTANCE_SERVER);
  for ( i = 0; i < 5; ++i )
    animPartBits->array[i] |= animToModel->partBits.array[i] & ~ignorePartBits->array[i];
  if ( parts->bLoop )
    v8 = time < 1.0;
  else
    v8 = time <= 1.0;
  if ( !v8 )
  {
    v7 = va("time: %f, parts->bLoop: %d", time, parts->bLoop);
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp",
            1493,
            0,
            "%s\n\t%s",
            "parts->bLoop ? (time < 1.f) : (time <= 1.f)",
            v7) )
      __debugbreak();
  }
  if ( time != 1.0 && parts->numframes )
  {
    if ( parts->numframes >= 0x100u )
      XAnimCalcParts_unsigned_short_(parts, animToModel->boneIndex, time, weightScale, rotTransArray, ignorePartBits);
    else
      XAnimCalcParts_unsigned_char_(parts, animToModel->boneIndex, time, weightScale, rotTransArray, ignorePartBits);
  }
  else
  {
    if ( parts->bLoop
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp", 1504, 0, "%s", "!parts->bLoop") )
    {
      __debugbreak();
    }
    XAnimCalcNonLoopEnd(parts, animToModel->boneIndex, weightScale, rotTransArray, ignorePartBits);
  }
}

void __cdecl XAnimCalcNonLoopEnd(
        const XAnimParts *parts,
        const unsigned __int8 *animToModel,
        float weightScale,
        DObjAnimMat *rotTransArray,
        bitarray<160> *ignorePartBits)
{
  float *trans; // eax
  float *v6; // edx
  float *v7; // edx
  int *v8; // [esp+Ch] [ebp-198h]
  __int16 *v9; // [esp+10h] [ebp-194h]
  unsigned __int8 *v10; // [esp+14h] [ebp-190h]
  float v11; // [esp+2Ch] [ebp-178h]
  float v12; // [esp+34h] [ebp-170h]
  float v13; // [esp+3Ch] [ebp-168h]
  float scale; // [esp+48h] [ebp-15Ch]
  float v15; // [esp+64h] [ebp-140h]
  float v16; // [esp+68h] [ebp-13Ch]
  float v17; // [esp+6Ch] [ebp-138h]
  float v18; // [esp+9Ch] [ebp-108h]
  float v19; // [esp+A0h] [ebp-104h]
  float v20; // [esp+A4h] [ebp-100h]
  __int64 sizeVec; // [esp+CCh] [ebp-D8h]
  __int64 posVec; // [esp+F0h] [ebp-B4h]
  __int64 posVec_8; // [esp+F8h] [ebp-ACh]
  float v24; // [esp+120h] [ebp-84h] BYREF
  float v25; // [esp+124h] [ebp-80h]
  float v26; // [esp+128h] [ebp-7Ch]
  float v27; // [esp+12Ch] [ebp-78h]
  float v28[2]; // [esp+130h] [ebp-74h] BYREF
  float v29; // [esp+138h] [ebp-6Ch]
  float v30; // [esp+13Ch] [ebp-68h]
  float dir; // [esp+144h] [ebp-60h] BYREF
  float v32; // [esp+148h] [ebp-5Ch]
  float v33; // [esp+14Ch] [ebp-58h]
  float v34; // [esp+150h] [ebp-54h]
  __int16 *v35; // [esp+154h] [ebp-50h]
  float4 frameVec; // [esp+15Ch] [ebp-48h] BYREF
  const __int16 *rotLastFrame; // [esp+170h] [ebp-34h]
  int useSmallIndices; // [esp+174h] [ebp-30h]
  __int16 *dataShort; // [esp+178h] [ebp-2Ch]
  DObjAnimMat *totalRotTrans; // [esp+17Ch] [ebp-28h]
  unsigned int animPartIndex; // [esp+180h] [ebp-24h]
  unsigned __int8 *dataByte; // [esp+184h] [ebp-20h]
  int *randomDataInt; // [esp+188h] [ebp-1Ch]
  unsigned int size; // [esp+18Ch] [ebp-18h]
  int *dataInt; // [esp+190h] [ebp-14h]
  __int16 *randomDataShort; // [esp+194h] [ebp-10h]
  unsigned __int8 *randomDataByte; // [esp+198h] [ebp-Ch]
  unsigned int tableSize; // [esp+19Ch] [ebp-8h]
  int modelPartIndex; // [esp+1A0h] [ebp-4h]

  if ( parts->bLoop
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp", 420, 0, "%s", "!parts->bLoop") )
  {
    __debugbreak();
  }
  if ( parts->dataByte )
    v10 = parts->dataByte;
  else
    v10 = 0;
  dataByte = v10;
  if ( parts->dataShort )
    v9 = parts->dataShort;
  else
    v9 = 0;
  dataShort = v9;
  if ( parts->dataInt )
    v8 = parts->dataInt;
  else
    v8 = 0;
  dataInt = v8;
  randomDataByte = parts->randomDataByte;
  randomDataShort = parts->randomDataShort;
  randomDataInt = parts->randomDataInt;
  useSmallIndices = parts->numframes < 0x100u;
  animPartIndex = 0;
  size = parts->boneCount[0];
  while ( animPartIndex < size )
  {
    modelPartIndex = animToModel[animPartIndex];
    if ( (unsigned int)modelPartIndex >= 0xA0
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp",
            438,
            0,
            "modelPartIndex doesn't index DOBJ_MAX_PARTS\n\t%i not in [0, %i)",
            modelPartIndex,
            160) )
    {
      __debugbreak();
    }
    if ( !bitarray<160>::testBit(ignorePartBits, modelPartIndex) )
    {
      totalRotTrans = &rotTransArray[modelPartIndex];
      totalRotTrans->quat[3] = totalRotTrans->quat[3] + weightScale;
    }
    ++animPartIndex;
  }
  size += parts->boneCount[1];
  while ( animPartIndex < size )
  {
    modelPartIndex = animToModel[animPartIndex];
    if ( (unsigned int)modelPartIndex >= 0xA0
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp",
            589,
            0,
            "modelPartIndex doesn't index DOBJ_MAX_PARTS\n\t%i not in [0, %i)",
            modelPartIndex,
            160) )
    {
      __debugbreak();
    }
    tableSize = (unsigned __int16)*dataShort++;
    if ( useSmallIndices )
    {
      dataByte += tableSize + 1;
    }
    else if ( tableSize >= 0x40 )
    {
      dataShort += ((tableSize - 1) >> 8) + 2;
    }
    else
    {
      dataShort += tableSize + 1;
    }
    if ( !bitarray<160>::testBit(ignorePartBits, modelPartIndex) )
    {
      rotLastFrame = &randomDataShort[2 * tableSize];
      frameVec.u[0] = *(unsigned int *)&FLOAT_0_0;
      frameVec.u[1] = *(unsigned int *)&FLOAT_0_0;
      frameVec.v[2] = (float)*rotLastFrame * 0.000030518509;
      frameVec.v[3] = (float)rotLastFrame[1] * 0.000030518509;
      scale = weightScale;
      if ( (float)((float)((float)((float)(0.0 * rotTransArray[modelPartIndex].quat[0])
                                 + (float)(0.0 * rotTransArray[modelPartIndex].quat[1]))
                         + (float)(frameVec.v[2] * rotTransArray[modelPartIndex].quat[2]))
                 + (float)(frameVec.v[3] * rotTransArray[modelPartIndex].quat[3])) < 0.0 )
        LODWORD(scale) = LODWORD(weightScale) ^ _mask__NegFloat_;
      Vec4Mad(rotTransArray[modelPartIndex].quat, scale, frameVec.v, rotTransArray[modelPartIndex].quat);
    }
    ++animPartIndex;
    randomDataShort += 2 * tableSize + 2;
  }
  size += parts->boneCount[2];
  while ( animPartIndex < size )
  {
    modelPartIndex = animToModel[animPartIndex];
    if ( (unsigned int)modelPartIndex >= 0xA0
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp",
            625,
            0,
            "modelPartIndex doesn't index DOBJ_MAX_PARTS\n\t%i not in [0, %i)",
            modelPartIndex,
            160) )
    {
      __debugbreak();
    }
    tableSize = (unsigned __int16)*dataShort++;
    if ( useSmallIndices )
    {
      dataByte += tableSize + 1;
    }
    else if ( tableSize >= 0x40 )
    {
      dataShort += ((tableSize - 1) >> 8) + 2;
    }
    else
    {
      dataShort += tableSize + 1;
    }
    if ( !bitarray<160>::testBit(ignorePartBits, modelPartIndex) )
    {
      v35 = &randomDataShort[4 * tableSize];
      dir = (float)*v35 * 0.000030518509;
      v32 = (float)v35[1] * 0.000030518509;
      v33 = (float)v35[2] * 0.000030518509;
      v34 = (float)v35[3] * 0.000030518509;
      v13 = weightScale;
      if ( (float)((float)((float)((float)(dir * rotTransArray[modelPartIndex].quat[0])
                                 + (float)(v32 * rotTransArray[modelPartIndex].quat[1]))
                         + (float)(v33 * rotTransArray[modelPartIndex].quat[2]))
                 + (float)(v34 * rotTransArray[modelPartIndex].quat[3])) < 0.0 )
        LODWORD(v13) = LODWORD(weightScale) ^ _mask__NegFloat_;
      Vec4Mad(rotTransArray[modelPartIndex].quat, v13, &dir, rotTransArray[modelPartIndex].quat);
    }
    ++animPartIndex;
    randomDataShort += 4 * tableSize + 4;
  }
  size += parts->boneCount[3];
  while ( animPartIndex < size )
  {
    modelPartIndex = animToModel[animPartIndex];
    if ( (unsigned int)modelPartIndex >= 0xA0
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp",
            657,
            0,
            "modelPartIndex doesn't index DOBJ_MAX_PARTS\n\t%i not in [0, %i)",
            modelPartIndex,
            160) )
    {
      __debugbreak();
    }
    if ( !bitarray<160>::testBit(ignorePartBits, modelPartIndex) )
    {
      v28[0] = *(float *)&FLOAT_0_0;
      v28[1] = *(float *)&FLOAT_0_0;
      v29 = (float)*dataShort * 0.000030518509;
      v30 = (float)dataShort[1] * 0.000030518509;
      v12 = weightScale;
      if ( (float)((float)((float)((float)(0.0 * rotTransArray[modelPartIndex].quat[0])
                                 + (float)(0.0 * rotTransArray[modelPartIndex].quat[1]))
                         + (float)(v29 * rotTransArray[modelPartIndex].quat[2]))
                 + (float)(v30 * rotTransArray[modelPartIndex].quat[3])) < 0.0 )
        LODWORD(v12) = LODWORD(weightScale) ^ _mask__NegFloat_;
      Vec4Mad(rotTransArray[modelPartIndex].quat, v12, v28, rotTransArray[modelPartIndex].quat);
    }
    ++animPartIndex;
    dataShort += 2;
  }
  size += parts->boneCount[4];
  while ( animPartIndex < size )
  {
    modelPartIndex = animToModel[animPartIndex];
    if ( (unsigned int)modelPartIndex >= 0xA0
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp",
            683,
            0,
            "modelPartIndex doesn't index DOBJ_MAX_PARTS\n\t%i not in [0, %i)",
            modelPartIndex,
            160) )
    {
      __debugbreak();
    }
    if ( !bitarray<160>::testBit(ignorePartBits, modelPartIndex) )
    {
      v24 = (float)*dataShort * 0.000030518509;
      v25 = (float)dataShort[1] * 0.000030518509;
      v26 = (float)dataShort[2] * 0.000030518509;
      v27 = (float)dataShort[3] * 0.000030518509;
      v11 = weightScale;
      if ( (float)((float)((float)((float)(v24 * rotTransArray[modelPartIndex].quat[0])
                                 + (float)(v25 * rotTransArray[modelPartIndex].quat[1]))
                         + (float)(v26 * rotTransArray[modelPartIndex].quat[2]))
                 + (float)(v27 * rotTransArray[modelPartIndex].quat[3])) < 0.0 )
        LODWORD(v11) = LODWORD(weightScale) ^ _mask__NegFloat_;
      Vec4Mad(rotTransArray[modelPartIndex].quat, v11, &v24, rotTransArray[modelPartIndex].quat);
    }
    ++animPartIndex;
    dataShort += 4;
  }
  animPartIndex = 0;
  size = parts->boneCount[5];
  while ( animPartIndex < size )
  {
    modelPartIndex = animToModel[*dataByte++];
    if ( (unsigned int)modelPartIndex >= 0xA0
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp",
            720,
            0,
            "modelPartIndex doesn't index DOBJ_MAX_PARTS\n\t%i not in [0, %i)",
            modelPartIndex,
            160) )
    {
      __debugbreak();
    }
    tableSize = (unsigned __int16)*dataShort++;
    if ( useSmallIndices )
    {
      dataByte += tableSize + 1;
    }
    else if ( tableSize >= 0x40 )
    {
      dataShort += ((tableSize - 1) >> 8) + 2;
    }
    else
    {
      dataShort += tableSize + 1;
    }
    if ( !bitarray<160>::testBit(ignorePartBits, modelPartIndex) )
    {
      totalRotTrans = &rotTransArray[modelPartIndex];
      posVec = *(_QWORD *)totalRotTrans->trans;
      posVec_8 = *(_QWORD *)&totalRotTrans->trans[2];
      sizeVec = *(_QWORD *)(dataInt + 3);
      *(float *)&posVec = (float)(weightScale
                                * (float)((float)(*(float *)&sizeVec * (float)randomDataByte[3 * tableSize])
                                        + *(float *)dataInt))
                        + *(float *)&posVec;
      *((float *)&posVec + 1) = (float)(weightScale
                                      * (float)((float)(*((float *)&sizeVec + 1)
                                                      * (float)randomDataByte[3 * tableSize + 1])
                                              + *((float *)dataInt + 1)))
                              + *((float *)&posVec + 1);
      *(float *)&posVec_8 = (float)(weightScale
                                  * (float)((float)(*((float *)dataInt + 5) * (float)randomDataByte[3 * tableSize + 2])
                                          + *((float *)dataInt + 2)))
                          + *(float *)&posVec_8;
      trans = totalRotTrans->trans;
      *(_QWORD *)totalRotTrans->trans = posVec;
      *((unsigned int *)trans + 2) = posVec_8;
      trans[3] = (float)(weightScale * (float)((float)(0.0 * 0.0) + 0.0)) + *((float *)&posVec_8 + 1);
      totalRotTrans->transWeight = totalRotTrans->transWeight + weightScale;
    }
    ++animPartIndex;
    dataInt += 6;
    randomDataByte += 3 * tableSize + 3;
  }
  size += parts->boneCount[6];
  while ( animPartIndex < size )
  {
    modelPartIndex = animToModel[*dataByte++];
    if ( (unsigned int)modelPartIndex >= 0xA0
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp",
            761,
            0,
            "modelPartIndex doesn't index DOBJ_MAX_PARTS\n\t%i not in [0, %i)",
            modelPartIndex,
            160) )
    {
      __debugbreak();
    }
    tableSize = (unsigned __int16)*dataShort++;
    if ( useSmallIndices )
    {
      dataByte += tableSize + 1;
    }
    else if ( tableSize >= 0x40 )
    {
      dataShort += ((tableSize - 1) >> 8) + 2;
    }
    else
    {
      dataShort += tableSize + 1;
    }
    if ( !bitarray<160>::testBit(ignorePartBits, modelPartIndex) )
    {
      totalRotTrans = &rotTransArray[modelPartIndex];
      v18 = (float)(weightScale
                  * (float)((float)(*((float *)dataInt + 4) * (float)(unsigned __int16)randomDataShort[3 * tableSize + 1])
                          + *((float *)dataInt + 1)))
          + totalRotTrans->trans[1];
      v19 = (float)(weightScale
                  * (float)((float)(*((float *)dataInt + 5) * (float)(unsigned __int16)randomDataShort[3 * tableSize + 2])
                          + *((float *)dataInt + 2)))
          + totalRotTrans->trans[2];
      v20 = (float)(weightScale * (float)((float)(0.0 * 0.0) + 0.0)) + totalRotTrans->transWeight;
      v6 = totalRotTrans->trans;
      totalRotTrans->trans[0] = (float)(weightScale
                                      * (float)((float)(*((float *)dataInt + 3)
                                                      * (float)(unsigned __int16)randomDataShort[3 * tableSize])
                                              + *(float *)dataInt))
                              + totalRotTrans->trans[0];
      v6[1] = v18;
      v6[2] = v19;
      v6[3] = v20;
      totalRotTrans->transWeight = totalRotTrans->transWeight + weightScale;
    }
    ++animPartIndex;
    dataInt += 6;
    randomDataShort += 3 * tableSize + 3;
  }
  size += parts->boneCount[7];
  while ( animPartIndex < size )
  {
    modelPartIndex = animToModel[*dataByte];
    if ( (unsigned int)modelPartIndex >= 0xA0
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp",
            794,
            0,
            "modelPartIndex doesn't index DOBJ_MAX_PARTS\n\t%i not in [0, %i)",
            modelPartIndex,
            160) )
    {
      __debugbreak();
    }
    if ( !bitarray<160>::testBit(ignorePartBits, modelPartIndex) )
    {
      totalRotTrans = &rotTransArray[modelPartIndex];
      v15 = (float)(weightScale * *((float *)dataInt + 1)) + totalRotTrans->trans[1];
      v16 = (float)(weightScale * *((float *)dataInt + 2)) + totalRotTrans->trans[2];
      v17 = (float)(weightScale * 0.0) + totalRotTrans->transWeight;
      v7 = totalRotTrans->trans;
      totalRotTrans->trans[0] = (float)(weightScale * *(float *)dataInt) + totalRotTrans->trans[0];
      v7[1] = v15;
      v7[2] = v16;
      v7[3] = v17;
      totalRotTrans->transWeight = totalRotTrans->transWeight + weightScale;
    }
    ++animPartIndex;
    ++dataByte;
    dataInt += 3;
  }
  size += parts->boneCount[8];
  while ( animPartIndex < size )
  {
    modelPartIndex = animToModel[*dataByte];
    if ( (unsigned int)modelPartIndex >= 0xA0
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp",
            814,
            0,
            "modelPartIndex doesn't index DOBJ_MAX_PARTS\n\t%i not in [0, %i)",
            modelPartIndex,
            160) )
    {
      __debugbreak();
    }
    if ( !bitarray<160>::testBit(ignorePartBits, modelPartIndex) )
    {
      totalRotTrans = &rotTransArray[modelPartIndex];
      totalRotTrans->transWeight = totalRotTrans->transWeight + weightScale;
    }
    ++animPartIndex;
    ++dataByte;
  }
}

void __cdecl Vec4Mad(const float *start, float scale, const float *dir, float *result)
{
  *result = (float)(scale * *dir) + *start;
  result[1] = (float)(scale * dir[1]) + start[1];
  result[2] = (float)(scale * dir[2]) + start[2];
  result[3] = (float)(scale * dir[3]) + start[3];
}

DObjAnimMat *__cdecl XAnimGetCalcBuffer(XAnimCalcAnimInfo *info, const DObj *obj, int *rotTransArrayIndex)
{
  DObjAnimMat *calcBuffer; // [esp+0h] [ebp-8h]
  int index; // [esp+4h] [ebp-4h]

  calcBuffer = &info->rotTransArray[*rotTransArrayIndex];
  index = *rotTransArrayIndex + obj->numBones;
  if ( index <= 960 )
  {
    *rotTransArrayIndex = index;
    return calcBuffer;
  }
  else
  {
    Com_PrintWarning(19, "max calculation buffer exceeded\n");
    DObjDisplayAnim(obj, &toastPopupTitle);
    return 0;
  }
}

void __cdecl XAnimScaleRotTransArray(int numBones, const XAnimCalcAnimInfo *info, DObjAnimMat *rotTransArray)
{
  float r; // [esp+8h] [ebp-8h]
  signed int i; // [esp+Ch] [ebp-4h]

  for ( i = 0; i < numBones; ++i )
  {
    if ( !bitarray<160>::testBit(&info->ignorePartBits, i) && rotTransArray->transWeight != 0.0 )
    {
      r = 1.0 / rotTransArray->transWeight;
      rotTransArray->quat[0] = r * rotTransArray->quat[0];
      rotTransArray->quat[1] = r * rotTransArray->quat[1];
      rotTransArray->quat[2] = r * rotTransArray->quat[2];
      rotTransArray->quat[3] = r * rotTransArray->quat[3];
      rotTransArray->trans[0] = r * rotTransArray->trans[0];
      rotTransArray->trans[1] = r * rotTransArray->trans[1];
      rotTransArray->trans[2] = r * rotTransArray->trans[2];
    }
    ++rotTransArray;
  }
}

void __cdecl XAnimNormalizeRotScaleTransArray(
        int numBones,
        const XAnimCalcAnimInfo *info,
        float weightScale,
        DObjAnimMat *rotTransArray)
{
  float v4; // [esp+8h] [ebp-28h]
  float v5; // [esp+10h] [ebp-20h]
  int r; // [esp+28h] [ebp-8h]
  signed int i; // [esp+2Ch] [ebp-4h]

  for ( i = 0; i < numBones; ++i )
  {
    if ( !bitarray<160>::testBit(&info->ignorePartBits, i) )
    {
      *(float *)&r = Vec4LengthSq(rotTransArray->quat);
      if ( *(float *)&r != 0.0 )
      {
        v5 = I_rsqrt(r) * weightScale;
        rotTransArray->quat[0] = v5 * rotTransArray->quat[0];
        rotTransArray->quat[1] = v5 * rotTransArray->quat[1];
        rotTransArray->quat[2] = v5 * rotTransArray->quat[2];
        rotTransArray->quat[3] = v5 * rotTransArray->quat[3];
      }
      if ( rotTransArray->transWeight != 0.0 )
      {
        v4 = weightScale / rotTransArray->transWeight;
        rotTransArray->trans[0] = v4 * rotTransArray->trans[0];
        rotTransArray->trans[1] = v4 * rotTransArray->trans[1];
        rotTransArray->trans[2] = v4 * rotTransArray->trans[2];
        rotTransArray->transWeight = weightScale;
      }
    }
    ++rotTransArray;
  }
}

void __cdecl XAnimMadRotTransArray(
        int numBones,
        const XAnimCalcAnimInfo *info,
        float weightScale,
        const DObjAnimMat *rotTrans,
        DObjAnimMat *totalRotTrans)
{
  float v5; // [esp+Ch] [ebp-3Ch]
  float v6; // [esp+14h] [ebp-34h]
  int r; // [esp+2Ch] [ebp-1Ch]
  signed int i; // [esp+30h] [ebp-18h]
  float addQuat; // [esp+38h] [ebp-10h]
  float addQuat_4; // [esp+3Ch] [ebp-Ch]
  float addQuat_8; // [esp+40h] [ebp-8h]
  float addQuat_12; // [esp+44h] [ebp-4h]

  for ( i = 0; i < numBones; ++i )
  {
    if ( !bitarray<160>::testBit(&info->ignorePartBits, i) )
    {
      if ( (float)((float)((float)((float)(totalRotTrans->quat[0] * rotTrans->quat[0])
                                 + (float)(totalRotTrans->quat[1] * rotTrans->quat[1]))
                         + (float)(totalRotTrans->quat[2] * rotTrans->quat[2]))
                 + (float)(totalRotTrans->quat[3] * rotTrans->quat[3])) >= 0.0 )
      {
        addQuat = rotTrans->quat[0];
        addQuat_4 = rotTrans->quat[1];
        addQuat_8 = rotTrans->quat[2];
        addQuat_12 = rotTrans->quat[3];
      }
      else
      {
        LODWORD(addQuat) = LODWORD(rotTrans->quat[0]) ^ _mask__NegFloat_;
        LODWORD(addQuat_4) = LODWORD(rotTrans->quat[1]) ^ _mask__NegFloat_;
        LODWORD(addQuat_8) = LODWORD(rotTrans->quat[2]) ^ _mask__NegFloat_;
        LODWORD(addQuat_12) = LODWORD(rotTrans->quat[3]) ^ _mask__NegFloat_;
      }
      *(float *)&r = Vec4LengthSq(rotTrans->quat);
      if ( *(float *)&r != 0.0 )
      {
        v6 = I_rsqrt(r) * weightScale;
        totalRotTrans->quat[0] = (float)(v6 * addQuat) + totalRotTrans->quat[0];
        totalRotTrans->quat[1] = (float)(v6 * addQuat_4) + totalRotTrans->quat[1];
        totalRotTrans->quat[2] = (float)(v6 * addQuat_8) + totalRotTrans->quat[2];
        totalRotTrans->quat[3] = (float)(v6 * addQuat_12) + totalRotTrans->quat[3];
      }
      if ( rotTrans->transWeight != 0.0 )
      {
        v5 = weightScale / rotTrans->transWeight;
        totalRotTrans->trans[0] = (float)(v5 * rotTrans->trans[0]) + totalRotTrans->trans[0];
        totalRotTrans->trans[1] = (float)(v5 * rotTrans->trans[1]) + totalRotTrans->trans[1];
        totalRotTrans->trans[2] = (float)(v5 * rotTrans->trans[2]) + totalRotTrans->trans[2];
        totalRotTrans->transWeight = totalRotTrans->transWeight + weightScale;
      }
    }
    ++rotTrans;
    ++totalRotTrans;
  }
}

void __cdecl XAnimApplyAdditives(
        DObjAnimMat *rotTransArray,
        DObjAnimMat *additiveArray,
        float weight,
        int boneCount,
        XAnimCalcAnimInfo *info)
{
  DObjAnimMat *v5; // ecx
  float *trans; // [esp+0h] [ebp-40h]
  DObjAnimMat *v7; // [esp+18h] [ebp-28h]
  float r; // [esp+24h] [ebp-1Ch]
  float ra; // [esp+24h] [ebp-1Ch]
  float rb; // [esp+24h] [ebp-1Ch]
  signed int i; // [esp+28h] [ebp-18h]
  float rot[4]; // [esp+2Ch] [ebp-14h] BYREF
  const bitarray<160> *ignorePartBits; // [esp+3Ch] [ebp-4h]

  ignorePartBits = &info->ignorePartBits;
  for ( i = 0; i < boneCount; ++i )
  {
    if ( !bitarray<160>::testBit((bitarray<160> *)ignorePartBits, i) )
    {
      r = Vec4LengthSq(additiveArray[i].quat);
      if ( r != 0.0 )
      {
        ra = fsqrt(r);
        v7 = &additiveArray[i];
        v7->quat[0] = (float)(1.0 / ra) * v7->quat[0];
        v7->quat[1] = (float)(1.0 / ra) * v7->quat[1];
        v7->quat[2] = (float)(1.0 / ra) * v7->quat[2];
        v7->quat[3] = (float)(1.0 / ra) * v7->quat[3];
        rb = ra * weight;
        v7->quat[0] = rb * v7->quat[0];
        v7->quat[1] = rb * v7->quat[1];
        v7->quat[2] = rb * v7->quat[2];
        v7->quat[3] = (float)((float)(1.0 - rb) * 1.0) + (float)(v7->quat[3] * rb);
        QuatMultiply(v7->quat, rotTransArray[i].quat, rot);
        rotTransArray[i].quat[0] = rot[0];
        v5 = &rotTransArray[i];
        v5->quat[1] = rot[1];
        v5->quat[2] = rot[2];
        v5->quat[3] = rot[3];
      }
      trans = rotTransArray[i].trans;
      *trans = (float)(weight * additiveArray[i].trans[0]) + *trans;
      trans[1] = (float)(weight * additiveArray[i].trans[1]) + rotTransArray[i].trans[1];
      trans[2] = (float)(weight * additiveArray[i].trans[2]) + rotTransArray[i].trans[2];
    }
  }
}

XAnimCalcAnimInfo *__thiscall XAnimCalcAnimInfo::XAnimCalcAnimInfo(XAnimCalcAnimInfo *this)
{
  int j; // [esp+8h] [ebp-Ch]
  int i; // [esp+10h] [ebp-4h]

  for ( i = 0; i < 5; ++i )
    this->animPartBits.array[i] = 0;
  for ( j = 0; j < 5; ++j )
    this->ignorePartBits.array[j] = 0;
  return this;
}

void __cdecl XAnim_CalcDeltaForTime(const XAnimParts *anim, float time, float *rotDelta, float4 *posDelta)
{
  int frameCount; // [esp+20h] [ebp-8h]
  XAnimDeltaPart *animDelta; // [esp+24h] [ebp-4h]

  if ( (time < 0.0 || time > 1.0)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xanim_calc.cpp",
          2555,
          0,
          "time not in [0.0f, 1.0f]\n\t%g not in [%g, %g]",
          time,
          0.0,
          1.0) )
  {
    __debugbreak();
  }
  animDelta = anim->deltaPart;
  frameCount = anim->numframes;
  if ( time == 1.0 || !anim->numframes )
  {
    XAnim_CalcRotDeltaEntire(animDelta, rotDelta);
    XAnim_CalcPosDeltaEntire(animDelta, posDelta);
  }
  else if ( anim->numframes >= 0x100u )
  {
    XAnim_CalcRotDeltaDuring_unsigned_short_(animDelta, time, frameCount, rotDelta);
    XAnim_CalcPosDeltaDuring_unsigned_short_(animDelta, time, frameCount, posDelta);
  }
  else
  {
    XAnim_CalcRotDeltaDuring_unsigned_char_(animDelta, time, frameCount, rotDelta);
    XAnim_CalcPosDeltaDuring_unsigned_char_(animDelta, time, frameCount, posDelta);
  }
}

void __cdecl XAnim_CalcRotDeltaEntire(const XAnimDeltaPart *animDelta, float *rotDelta)
{
  XAnimDeltaPartQuat *rotFrameDeltas; // [esp+0h] [ebp-8h]
  const __int16 *rotDeltaLastFrame; // [esp+4h] [ebp-4h]

  if ( animDelta->quat )
  {
    rotFrameDeltas = animDelta->quat;
    if ( rotFrameDeltas->size )
      rotDeltaLastFrame = rotFrameDeltas->u.frames.frames[rotFrameDeltas->size];
    else
      rotDeltaLastFrame = (const __int16 *)&rotFrameDeltas->u;
    *rotDelta = (float)*rotDeltaLastFrame;
    rotDelta[1] = (float)rotDeltaLastFrame[1];
  }
  else
  {
    *rotDelta = *(float *)&FLOAT_0_0;
    rotDelta[1] = FLOAT_32767_0;
  }
}

void __cdecl XAnim_CalcPosDeltaEntire(const XAnimDeltaPart *animDelta, float4 *posDelta)
{
  int v2; // edx
  unsigned __int16 *v3; // [esp+8h] [ebp-44h]
  unsigned __int8 *v4; // [esp+Ch] [ebp-40h]
  __int64 sizeVec; // [esp+14h] [ebp-38h]
  float sizeVec_8; // [esp+1Ch] [ebp-30h]
  float lerp[4]; // [esp+24h] [ebp-28h]
  float minsVec[2]; // [esp+38h] [ebp-14h]
  XAnimPartTrans *posFrameDeltas; // [esp+48h] [ebp-4h]

  if ( animDelta->trans )
  {
    posFrameDeltas = animDelta->trans;
    if ( animDelta->trans->size )
    {
      if ( posFrameDeltas->smallTrans )
      {
        v4 = posFrameDeltas->u.frames.frames._1[posFrameDeltas->size];
        lerp[0] = (float)*v4;
        lerp[1] = (float)v4[1];
        v2 = v4[2];
      }
      else
      {
        v3 = (unsigned __int16 *)posFrameDeltas->u.frames.frames._1[2 * posFrameDeltas->size];
        lerp[0] = (float)*v3;
        lerp[1] = (float)v3[1];
        v2 = v3[2];
      }
      minsVec[0] = posFrameDeltas->u.frames.mins[1];
      minsVec[1] = posFrameDeltas->u.frames.mins[2];
      sizeVec = *(_QWORD *)&posFrameDeltas->u.frame0[3];
      sizeVec_8 = posFrameDeltas->u.frames.size[2];
      posDelta->v[0] = (float)(*(float *)&sizeVec * lerp[0]) + posFrameDeltas->u.frames.mins[0];
      posDelta->v[1] = (float)(*((float *)&sizeVec + 1) * lerp[1]) + minsVec[0];
      posDelta->v[2] = (float)(sizeVec_8 * (float)v2) + minsVec[1];
      posDelta->v[3] = (float)(0.0 * 0.0) + 0.0;
    }
    else
    {
      posDelta->v[0] = posFrameDeltas->u.frames.mins[0];
      posDelta->v[1] = posFrameDeltas->u.frames.mins[1];
      posDelta->v[2] = posFrameDeltas->u.frames.mins[2];
      posDelta->u[3] = *(unsigned int *)&FLOAT_0_0;
    }
  }
  else
  {
    posDelta->u[0] = *(unsigned int *)&FLOAT_0_0;
    posDelta->u[1] = *(unsigned int *)&FLOAT_0_0;
    posDelta->u[2] = *(unsigned int *)&FLOAT_0_0;
    posDelta->u[3] = *(unsigned int *)&FLOAT_0_0;
  }
}

bool __thiscall bitarray<160>::testBit(bitarray<160> *this, unsigned int pos)
{
  if ( pos >= 0xA0
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\universal\\../qcommon/bitarray.h",
          109,
          0,
          "%s",
          "pos < BIT_COUNT") )
  {
    __debugbreak();
  }
  return (this->array[pos >> 5] & (0x80000000 >> (pos & 0x1F))) != 0;
}

void __cdecl XAnimWeightedAccumLerpedTrans(
        const float4 *fromVec,
        const float4 *toVec,
        float keyFrameLerpFrac,
        float weightScale,
        int *dataInt,
        DObjAnimMat *totalRotTrans)
{
  float frameVec_4; // [esp+18h] [ebp-18h]
  float frameVec_8; // [esp+1Ch] [ebp-14h]
  float lerp[3]; // [esp+20h] [ebp-10h] BYREF
  const float *minsVec; // [esp+2Ch] [ebp-4h]

  Vec3Lerp(fromVec->v, toVec->v, keyFrameLerpFrac, lerp);
  minsVec = (const float *)dataInt;
  frameVec_4 = (float)(*((float *)dataInt + 4) * lerp[1]) + *((float *)dataInt + 1);
  frameVec_8 = (float)(*((float *)dataInt + 5) * lerp[2]) + *((float *)dataInt + 2);
  totalRotTrans->trans[0] = (float)(weightScale * (float)((float)(*((float *)dataInt + 3) * lerp[0]) + *(float *)dataInt))
                          + totalRotTrans->trans[0];
  totalRotTrans->trans[1] = (float)(weightScale * frameVec_4) + totalRotTrans->trans[1];
  totalRotTrans->trans[2] = (float)(weightScale * frameVec_8) + totalRotTrans->trans[2];
  totalRotTrans->transWeight = totalRotTrans->transWeight + weightScale;
}

void __cdecl XAnimWeightedAccumTrans(float weightScale, float *dataInt, DObjAnimMat *totalRotTrans)
{
  totalRotTrans->trans[0] = (float)(weightScale * *dataInt) + totalRotTrans->trans[0];
  totalRotTrans->trans[1] = (float)(weightScale * dataInt[1]) + totalRotTrans->trans[1];
  totalRotTrans->trans[2] = (float)(weightScale * dataInt[2]) + totalRotTrans->trans[2];
  totalRotTrans->transWeight = totalRotTrans->transWeight + weightScale;
}

void __cdecl Short2LerpAsVec2(const __int16 *from, const __int16 *to, float frac, float *out)
{
  *out = (float)*from + (float)((float)(*to - *from) * frac);
  out[1] = (float)from[1] + (float)((float)(to[1] - from[1]) * frac);
}

