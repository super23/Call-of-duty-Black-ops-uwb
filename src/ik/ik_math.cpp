#include "ik_math.h"

void __cdecl ikMatrixIdentity44(float (*out)[4])
{
  if ( !out && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp", 221, 0, "%s", "out") )
    __debugbreak();
  memcpy(out, ikIdentityMatrix44, 0x40u);
}

void __cdecl ikMatrixSet44(float (*out)[4], const float *origin, const float (*axis)[3], float scale)
{
  (*out)[0] = (*axis)[0] * scale;
  (*out)[1] = (float)(*axis)[1] * scale;
  (*out)[2] = (float)(*axis)[2] * scale;
  (*out)[3] = 0.0f;
  (*out)[4] = (float)(*axis)[3] * scale;
  (*out)[5] = (float)(*axis)[4] * scale;
  (*out)[6] = (float)(*axis)[5] * scale;
  (*out)[7] = 0.0f;
  (*out)[8] = (float)(*axis)[6] * scale;
  (*out)[9] = (float)(*axis)[7] * scale;
  (*out)[10] = (float)(*axis)[8] * scale;
  (*out)[11] = 0.0f;
  (*out)[12] = *origin;
  (*out)[13] = origin[1];
  (*out)[14] = origin[2];
  (*out)[15] = 1.0f;
}

void __cdecl ikMatrixTransformVector34(const float *in1, const float (*in2)[4], float *out)
{
  if ( in1 == out && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp", 550, 0, "%s", "in1 != out") )
    __debugbreak();
  *out = (float)((float)(*in1 * (*in2)[0]) + (float)(in1[1] * (float)(*in2)[4])) + (float)(in1[2] * (float)(*in2)[8]);
  out[1] = (float)((float)(*in1 * (float)(*in2)[1]) + (float)(in1[1] * (float)(*in2)[5]))
         + (float)(in1[2] * (float)(*in2)[9]);
  out[2] = (float)((float)(*in1 * (float)(*in2)[2]) + (float)(in1[1] * (float)(*in2)[6]))
         + (float)(in1[2] * (float)(*in2)[10]);
}

void __cdecl ikQuatTransToMatrix44(float *quat, const float *trans, float (*out)[4])
{
  float yy; // [esp+0h] [ebp-48h]
  float xy; // [esp+8h] [ebp-40h]
  float z; // [esp+Ch] [ebp-3Ch]
  float zz; // [esp+10h] [ebp-38h]
  float zw; // [esp+14h] [ebp-34h]
  float yw; // [esp+20h] [ebp-28h]
  float xz; // [esp+24h] [ebp-24h]
  float yz; // [esp+28h] [ebp-20h]
  float xx; // [esp+2Ch] [ebp-1Ch]
  float xw; // [esp+30h] [ebp-18h]
  float magSqr; // [esp+38h] [ebp-10h]
  float x; // [esp+3Ch] [ebp-Ch]
  float y; // [esp+40h] [ebp-8h]
  float w; // [esp+44h] [ebp-4h]

  x = *quat;
  y = quat[1];
  z = quat[2];
  w = quat[3];
  magSqr = (float)((float)((float)(x * x) + (float)(y * y)) + (float)(z * z)) + (float)(w * w);
  if ( magSqr <= 0.0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp", 600, 0, "%s", "magSqr > 0.0f") )
  {
    __debugbreak();
  }
  xx = (float)(x * x) * (float)(2.0 / magSqr);
  yy = (float)(y * y) * (float)(2.0 / magSqr);
  zz = (float)(z * z) * (float)(2.0 / magSqr);
  xy = (float)((float)(2.0 / magSqr) * x) * y;
  xz = (float)((float)(2.0 / magSqr) * x) * z;
  xw = (float)((float)(2.0 / magSqr) * x) * w;
  yz = (float)((float)(2.0 / magSqr) * y) * z;
  yw = (float)((float)(2.0 / magSqr) * y) * w;
  zw = (float)((float)(2.0 / magSqr) * z) * w;
  (*out)[0] = 1.0 - (float)(yy + zz);
  (*out)[1] = xy + zw;
  (*out)[2] = xz - yw;
  (*out)[3] = 0.0f;
  (*out)[4] = xy - zw;
  (*out)[5] = 1.0 - (float)(xx + zz);
  (*out)[6] = yz + xw;
  (*out)[7] = 0.0f;
  (*out)[8] = xz + yw;
  (*out)[9] = yz - xw;
  (*out)[10] = 1.0 - (float)(xx + yy);
  (*out)[11] = 0.0f;
  (*out)[12] = *trans;
  (*out)[13] = trans[1];
  (*out)[14] = trans[2];
  (*out)[15] = 1.0f;
}

void __cdecl ikMatrix44ToQuatTrans(float (*mat)[4], float *quat, float *trans)
{
  float axis[3][3]; // [esp+14h] [ebp-24h] BYREF

  *(_QWORD *)&axis[0][0] = *(_QWORD *)&(*mat)[0];
  axis[0][2] = (*mat)[2];
  axis[1][0] = (*mat)[4];
  axis[1][1] = (*mat)[5];
  axis[1][2] = (*mat)[6];
  axis[2][0] = (*mat)[8];
  axis[2][1] = (*mat)[9];
  axis[2][2] = (*mat)[10];
  ikAxisToQuat(axis, quat);
  *trans = (*mat)[12];
  trans[1] = (*mat)[13];
  trans[2] = (*mat)[14];
}

void __cdecl ikAxisToQuat(const float (*mat)[3], float *out)
{
  float *v2; // [esp+8h] [ebp-50h]
  float invLength; // [esp+Ch] [ebp-4Ch]
  float test[4][4]; // [esp+10h] [ebp-48h] BYREF
  int best; // [esp+50h] [ebp-8h]
  float testSizeSq; // [esp+54h] [ebp-4h]

  test[0][0] = (float)(*mat)[5] - (float)(*mat)[7];
  test[0][1] = (float)(*mat)[6] - (float)(*mat)[2];
  test[0][2] = (float)(*mat)[1] - (float)(*mat)[3];
  test[0][3] = (float)((float)((*mat)[0] + (float)(*mat)[4]) + (float)(*mat)[8]) + 1.0;
  testSizeSq = Vec4LengthSq(test[0]);
  if ( testSizeSq < 1.0 )
  {
    test[1][0] = (float)(*mat)[6] + (float)(*mat)[2];
    test[1][1] = (float)(*mat)[7] + (float)(*mat)[5];
    test[1][2] = (float)((float)((float)(*mat)[8] - (float)(*mat)[4]) - (*mat)[0]) + 1.0;
    test[1][3] = test[0][2];
    testSizeSq = Vec4LengthSq(test[1]);
    if ( testSizeSq < 1.0 )
    {
      test[2][0] = (float)((float)((*mat)[0] - (float)(*mat)[4]) - (float)(*mat)[8]) + 1.0;
      test[2][1] = (float)(*mat)[3] + (float)(*mat)[1];
      *(_QWORD *)&test[2][2] = __PAIR64__(LODWORD(test[0][0]), LODWORD(test[1][0]));
      testSizeSq = Vec4LengthSq(test[2]);
      if ( testSizeSq < 1.0 )
      {
        test[3][0] = test[2][1];
        test[3][1] = (float)((float)((float)(*mat)[4] - (*mat)[0]) - (float)(*mat)[8]) + 1.0;
        *(_QWORD *)&test[3][2] = __PAIR64__(LODWORD(test[0][1]), LODWORD(test[1][1]));
        testSizeSq = Vec4LengthSq(test[3]);
        if ( testSizeSq < 1.0
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp",
                704,
                0,
                "%s\n\t(testSizeSq) = %g",
                "(testSizeSq >= 1.0f)",
                testSizeSq) )
        {
          __debugbreak();
        }
        best = 3;
      }
      else
      {
        best = 2;
      }
    }
    else
    {
      best = 1;
    }
  }
  else
  {
    best = 0;
  }
  if ( testSizeSq == 0.0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp", 709, 0, "%s", "testSizeSq") )
  {
    __debugbreak();
  }
  invLength = 1.0 / sqrtf(testSizeSq);
  v2 = test[best];
  *out = invLength * *v2;
  out[1] = invLength * v2[1];
  out[2] = invLength * v2[2];
  out[3] = invLength * v2[3];
}

void __cdecl ikMat44Set(float (*mat)[4], const float *v1, const float *v2, const float *v3, const float *pos)
{
  (*mat)[0] = *v1;
  (*mat)[1] = v1[1];
  (*mat)[2] = v1[2];
  (*mat)[4] = *v2;
  (*mat)[5] = v2[1];
  (*mat)[6] = v2[2];
  (*mat)[8] = *v3;
  (*mat)[9] = v3[1];
  (*mat)[10] = v3[2];
  (*mat)[12] = *pos;
  (*mat)[13] = pos[1];
  (*mat)[14] = pos[2];
  (*mat)[3] = 0.0f;
  (*mat)[7] = 0.0f;
  (*mat)[11] = 0.0f;
  (*mat)[15] = 1.0f;
}

// local variable allocation has failed, the output may be wrong!
void  ikCalcBoneModelMatrix_r(
        float *a1@<ebp>,
        IKState *ikState,
        int boneNum,
        float *inMatArray,
        float (*out)[4])
{
  float v5; // [esp-218h] [ebp-3D8h]
  float v6; // [esp-214h] [ebp-3D4h]
  float v7; // [esp-210h] [ebp-3D0h]
  float v8; // [esp-20Ch] [ebp-3CCh]
  float v9; // [esp-208h] [ebp-3C8h]
  float v10; // [esp-204h] [ebp-3C4h]
  float v11; // [esp-200h] [ebp-3C0h]
  float v12; // [esp-1FCh] [ebp-3BCh]
  float v13; // [esp-1F8h] [ebp-3B8h]
  float v14; // [esp-1F4h] [ebp-3B4h]
  float v15; // [esp-1F0h] [ebp-3B0h]
  float v16; // [esp-1ECh] [ebp-3ACh]
  float v17; // [esp-1E8h] [ebp-3A8h]
  float v18; // [esp-1E4h] [ebp-3A4h]
  float v19; // [esp-1E0h] [ebp-3A0h]
  float *v20; // [esp-1D4h] [ebp-394h]
  float v21; // [esp-1B0h] [ebp-370h]
  float v22; // [esp-1ACh] [ebp-36Ch]
  float v23; // [esp-1A8h] [ebp-368h]
  float v24; // [esp-1A4h] [ebp-364h]
  float v25; // [esp-1A0h] [ebp-360h]
  float v26; // [esp-19Ch] [ebp-35Ch]
  float v27; // [esp-198h] [ebp-358h]
  float v28; // [esp-194h] [ebp-354h]
  float v29; // [esp-190h] [ebp-350h]
  float v30; // [esp-18Ch] [ebp-34Ch]
  float v31; // [esp-188h] [ebp-348h]
  float v32; // [esp-184h] [ebp-344h]
  float v33; // [esp-180h] [ebp-340h]
  float v34; // [esp-17Ch] [ebp-33Ch]
  float v35; // [esp-178h] [ebp-338h]
  float *v36; // [esp-16Ch] [ebp-32Ch]
  float v37; // [esp-148h] [ebp-308h]
  float v38; // [esp-144h] [ebp-304h]
  float v39; // [esp-140h] [ebp-300h]
  float v40; // [esp-13Ch] [ebp-2FCh]
  float v41; // [esp-138h] [ebp-2F8h]
  float v42; // [esp-134h] [ebp-2F4h]
  float v43; // [esp-130h] [ebp-2F0h]
  float v44; // [esp-12Ch] [ebp-2ECh]
  float v45; // [esp-128h] [ebp-2E8h]
  float v46; // [esp-124h] [ebp-2E4h]
  float v47; // [esp-120h] [ebp-2E0h]
  float v48; // [esp-11Ch] [ebp-2DCh]
  float v49; // [esp-118h] [ebp-2D8h]
  float v50; // [esp-114h] [ebp-2D4h]
  float v51; // [esp-110h] [ebp-2D0h]
  float *v52; // [esp-108h] [ebp-2C8h]
  float v53; // [esp-F0h] [ebp-2B0h]
  float v54; // [esp-ECh] [ebp-2ACh]
  float v55; // [esp-E8h] [ebp-2A8h]
  float v56; // [esp-E4h] [ebp-2A4h]
  float v57; // [esp-E0h] [ebp-2A0h]
  float v58; // [esp-DCh] [ebp-29Ch]
  float v59; // [esp-D8h] [ebp-298h]
  float v60; // [esp-D4h] [ebp-294h]
  float v61; // [esp-D0h] [ebp-290h]
  float v62; // [esp-CCh] [ebp-28Ch]
  float v63; // [esp-C8h] [ebp-288h]
  float v64; // [esp-C4h] [ebp-284h]
  float v65; // [esp-C0h] [ebp-280h]
  float v66; // [esp-BCh] [ebp-27Ch]
  float v67; // [esp-B8h] [ebp-278h]
  float v68; // [esp-50h] [ebp-210h]
  float v69; // [esp-4Ch] [ebp-20Ch]
  float v70; // [esp-48h] [ebp-208h]
  float v71; // [esp-44h] [ebp-204h]
  float v72; // [esp-40h] [ebp-200h]
  float v73; // [esp-3Ch] [ebp-1FCh]
  float v74; // [esp-38h] [ebp-1F8h]
  float v75; // [esp-34h] [ebp-1F4h]
  float v76; // [esp-30h] [ebp-1F0h]
  float v77; // [esp-2Ch] [ebp-1ECh]
  float v78; // [esp-28h] [ebp-1E8h]
  float v79; // [esp-24h] [ebp-1E4h]
  float v80; // [esp-20h] [ebp-1E0h]
  float v81; // [esp-1Ch] [ebp-1DCh]
  float v82; // [esp-18h] [ebp-1D8h]
  float *v83; // [esp-10h] [ebp-1D0h]
  _BYTE v84[64]; // [esp-Ch] [ebp-1CCh] OVERLAPPED BYREF
  float v85; // [esp+50h] [ebp-170h]
  float v86; // [esp+54h] [ebp-16Ch]
  float v87; // [esp+58h] [ebp-168h]
  float v88; // [esp+5Ch] [ebp-164h]
  float v89; // [esp+60h] [ebp-160h]
  float v90; // [esp+64h] [ebp-15Ch]
  float v91; // [esp+68h] [ebp-158h]
  float v92; // [esp+6Ch] [ebp-154h]
  float v93; // [esp+70h] [ebp-150h]
  float v94; // [esp+74h] [ebp-14Ch]
  float v95; // [esp+78h] [ebp-148h]
  float v96; // [esp+7Ch] [ebp-144h]
  float v97; // [esp+80h] [ebp-140h]
  float v98; // [esp+84h] [ebp-13Ch]
  float v99; // [esp+88h] [ebp-138h]
  float *v100; // [esp+90h] [ebp-130h]
  float *v101; // [esp+94h] [ebp-12Ch]
  float *v102; // [esp+98h] [ebp-128h]
  float v103; // [esp+9Ch] [ebp-124h]
  float v104; // [esp+A0h] [ebp-120h]
  float v105; // [esp+A4h] [ebp-11Ch]
  float v106; // [esp+A8h] [ebp-118h]
  float v107; // [esp+ACh] [ebp-114h]
  float v108; // [esp+B0h] [ebp-110h]
  float v109; // [esp+B4h] [ebp-10Ch]
  float v110; // [esp+B8h] [ebp-108h]
  float v111; // [esp+BCh] [ebp-104h]
  float v112; // [esp+C0h] [ebp-100h]
  float v113; // [esp+C4h] [ebp-FCh]
  float v114; // [esp+C8h] [ebp-F8h]
  float v115; // [esp+CCh] [ebp-F4h]
  float v116; // [esp+D0h] [ebp-F0h]
  float v117; // [esp+D4h] [ebp-ECh]
  float v118; // [esp+D8h] [ebp-E8h]
  float *v119; // [esp+DCh] [ebp-E4h]
  float *v120; // [esp+E0h] [ebp-E0h]
  float *v121; // [esp+E4h] [ebp-DCh]
  float *v122; // [esp+E8h] [ebp-D8h]
  float v123; // [esp+ECh] [ebp-D4h]
  float v124; // [esp+F0h] [ebp-D0h]
  float v125; // [esp+F4h] [ebp-CCh]
  float v126; // [esp+F8h] [ebp-C8h]
  float v127; // [esp+FCh] [ebp-C4h]
  float v128; // [esp+100h] [ebp-C0h]
  float v129; // [esp+104h] [ebp-BCh]
  float v130; // [esp+108h] [ebp-B8h]
  float v131; // [esp+10Ch] [ebp-B4h]
  float v132; // [esp+110h] [ebp-B0h]
  float v133; // [esp+114h] [ebp-ACh]
  float v134; // [esp+118h] [ebp-A8h]
  float v135; // [esp+11Ch] [ebp-A4h]
  float v136; // [esp+120h] [ebp-A0h]
  float v137; // [esp+124h] [ebp-9Ch]
  float v138; // [esp+128h] [ebp-98h]
  float *v139; // [esp+130h] [ebp-90h]
  float *v140; // [esp+134h] [ebp-8Ch]
  float *v141; // [esp+138h] [ebp-88h]
  IKBoneNames v142; // [esp+13Ch] [ebp-84h]
  unsigned int *v143; // [esp+150h] [ebp-70h]
  unsigned int *v144; // [esp+154h] [ebp-6Ch]
  unsigned int *v145; // [esp+158h] [ebp-68h]
  float v146; // [esp+15Ch] [ebp-64h]
  float v147; // [esp+160h] [ebp-60h]
  float v148; // [esp+164h] [ebp-5Ch]
  float v149; // [esp+168h] [ebp-58h]
  float v150; // [esp+16Ch] [ebp-54h]
  float v151; // [esp+170h] [ebp-50h]
  float v152; // [esp+174h] [ebp-4Ch]
  float v153; // [esp+178h] [ebp-48h]
  float v154; // [esp+17Ch] [ebp-44h]
  float v155; // [esp+180h] [ebp-40h]
  float v156; // [esp+184h] [ebp-3Ch]
  float v157; // [esp+188h] [ebp-38h]
  float v158; // [esp+18Ch] [ebp-34h]
  float v159; // [esp+190h] [ebp-30h]
  float v160; // [esp+194h] [ebp-2Ch]
  float v161; // [esp+198h] [ebp-28h]
  float *v162; // [esp+1A0h] [ebp-20h]
  float *v163; // [esp+1A4h] [ebp-1Ch]
  float *v164; // [esp+1A8h] [ebp-18h]
  float *v165; // [esp+1ACh] [ebp-14h]
  float (*cache)[4][4]; // [esp+1B4h] [ebp-Ch]
  float (*matArray)[4][4]; // [esp+1B8h] [ebp-8h]
  float *retaddr; // [esp+1C0h] [ebp+0h]

  cache = (float (*)[4][4])a1;
  matArray = (float (*)[4][4])retaddr;
  v165 = inMatArray;
  v164 = 0;
  if ( inMatArray == (float *)ikState->matArrayPostIK )
  {
    v164 = (*ikState->matArrayCache)[0];
  }
  else if ( inMatArray == (float *)ikState->matArrayPreIK )
  {
    v164 = (*ikState->matArrayCachePre)[0];
  }
  if ( !v164 || v164[16 * boneNum + 15] == 0.0 )
  {
    v142 = ikState->localIkSystem->ikBoneParents[boneNum];
    if ( v142 == IKBONE_NONE )
    {
      v36 = &v165[16 * boneNum];
      v21 = v36[1];
      v22 = v36[2];
      v23 = v36[3];
      v24 = v36[4];
      v25 = v36[5];
      v26 = v36[6];
      v27 = v36[7];
      v28 = v36[8];
      v29 = v36[9];
      v30 = v36[10];
      v31 = v36[11];
      v32 = v36[12];
      v33 = v36[13];
      v34 = v36[14];
      v35 = v36[15];
      (*out)[0] = *v36;
      (*out)[1] = v21;
      (*out)[2] = v22;
      (*out)[3] = v23;
      (*out)[4] = v24;
      (*out)[5] = v25;
      (*out)[6] = v26;
      (*out)[7] = v27;
      (*out)[8] = v28;
      (*out)[9] = v29;
      (*out)[10] = v30;
      (*out)[11] = v31;
      (*out)[12] = v32;
      (*out)[13] = v33;
      (*out)[14] = v34;
      (*out)[15] = v35;
      ikNormalizedMatrixAssert_func(out);
      if ( v164 )
      {
        v20 = &v164[16 * boneNum];
        v5 = (*out)[1];
        v6 = (*out)[2];
        v7 = (*out)[3];
        v8 = (*out)[4];
        v9 = (*out)[5];
        v10 = (*out)[6];
        v11 = (*out)[7];
        v12 = (*out)[8];
        v13 = (*out)[9];
        v14 = (*out)[10];
        v15 = (*out)[11];
        v16 = (*out)[12];
        v17 = (*out)[13];
        v18 = (*out)[14];
        v19 = (*out)[15];
        *v20 = (*out)[0];
        v20[1] = v5;
        v20[2] = v6;
        v20[3] = v7;
        v20[4] = v8;
        v20[5] = v9;
        v20[6] = v10;
        v20[7] = v11;
        v20[8] = v12;
        v20[9] = v13;
        v20[10] = v14;
        v20[11] = v15;
        v20[12] = v16;
        v20[13] = v17;
        v20[14] = v18;
        v20[15] = v19;
        ikState->cacheActive = 1;
      }
    }
    else if ( !v164 || v164[16 * v142 + 15] == 0.0 )
    {
      ikCalcBoneModelMatrix_r(ikState, v142, inMatArray, (float (*)[4])v84);
      ikNormalizedMatrixAssert_func((float (*)[4])v84);
      v83 = &v165[16 * boneNum];
      v68 = v83[1];
      v69 = v83[2];
      v70 = v83[3];
      v71 = v83[4];
      v72 = v83[5];
      v73 = v83[6];
      v74 = v83[7];
      v75 = v83[8];
      v76 = v83[9];
      v77 = v83[10];
      v78 = v83[11];
      v79 = v83[12];
      v80 = v83[13];
      v81 = v83[14];
      v82 = v83[15];
      v53 = (float)((float)((float)(*v83 * *(float *)&v84[4]) + (float)(v68 * *(float *)&v84[20]))
                  + (float)(v69 * *(float *)&v84[36]))
          + (float)(v70 * *(float *)&v84[52]);
      v54 = (float)((float)((float)(*v83 * *(float *)&v84[8]) + (float)(v68 * *(float *)&v84[24]))
                  + (float)(v69 * *(float *)&v84[40]))
          + (float)(v70 * *(float *)&v84[56]);
      v55 = (float)((float)((float)(*v83 * *(float *)&v84[12]) + (float)(v68 * *(float *)&v84[28]))
                  + (float)(v69 * *(float *)&v84[44]))
          + (float)(v70 * *(float *)&v84[60]);
      v56 = (float)((float)((float)(v71 * *(float *)v84) + (float)(v72 * *(float *)&v84[16]))
                  + (float)(v73 * *(float *)&v84[32]))
          + (float)(v74 * *(float *)&v84[48]);
      v57 = (float)((float)((float)(v71 * *(float *)&v84[4]) + (float)(v72 * *(float *)&v84[20]))
                  + (float)(v73 * *(float *)&v84[36]))
          + (float)(v74 * *(float *)&v84[52]);
      v58 = (float)((float)((float)(v71 * *(float *)&v84[8]) + (float)(v72 * *(float *)&v84[24]))
                  + (float)(v73 * *(float *)&v84[40]))
          + (float)(v74 * *(float *)&v84[56]);
      v59 = (float)((float)((float)(v71 * *(float *)&v84[12]) + (float)(v72 * *(float *)&v84[28]))
                  + (float)(v73 * *(float *)&v84[44]))
          + (float)(v74 * *(float *)&v84[60]);
      v60 = (float)((float)((float)(v75 * *(float *)v84) + (float)(v76 * *(float *)&v84[16]))
                  + (float)(v77 * *(float *)&v84[32]))
          + (float)(v78 * *(float *)&v84[48]);
      v61 = (float)((float)((float)(v75 * *(float *)&v84[4]) + (float)(v76 * *(float *)&v84[20]))
                  + (float)(v77 * *(float *)&v84[36]))
          + (float)(v78 * *(float *)&v84[52]);
      v62 = (float)((float)((float)(v75 * *(float *)&v84[8]) + (float)(v76 * *(float *)&v84[24]))
                  + (float)(v77 * *(float *)&v84[40]))
          + (float)(v78 * *(float *)&v84[56]);
      v63 = (float)((float)((float)(v75 * *(float *)&v84[12]) + (float)(v76 * *(float *)&v84[28]))
                  + (float)(v77 * *(float *)&v84[44]))
          + (float)(v78 * *(float *)&v84[60]);
      v64 = (float)((float)((float)(v79 * *(float *)v84) + (float)(v80 * *(float *)&v84[16]))
                  + (float)(v81 * *(float *)&v84[32]))
          + (float)(v82 * *(float *)&v84[48]);
      v65 = (float)((float)((float)(v79 * *(float *)&v84[4]) + (float)(v80 * *(float *)&v84[20]))
                  + (float)(v81 * *(float *)&v84[36]))
          + (float)(v82 * *(float *)&v84[52]);
      v66 = (float)((float)((float)(v79 * *(float *)&v84[8]) + (float)(v80 * *(float *)&v84[24]))
                  + (float)(v81 * *(float *)&v84[40]))
          + (float)(v82 * *(float *)&v84[56]);
      v67 = (float)((float)((float)(v79 * *(float *)&v84[12]) + (float)(v80 * *(float *)&v84[28]))
                  + (float)(v81 * *(float *)&v84[44]))
          + (float)(v82 * *(float *)&v84[60]);
      (*out)[0] = (float)((float)((float)(*v83 * *(float *)v84) + (float)(v68 * *(float *)&v84[16]))
                        + (float)(v69 * *(float *)&v84[32]))
                + (float)(v70 * *(float *)&v84[48]);
      (*out)[1] = v53;
      (*out)[2] = v54;
      (*out)[3] = v55;
      (*out)[4] = v56;
      (*out)[5] = v57;
      (*out)[6] = v58;
      (*out)[7] = v59;
      (*out)[8] = v60;
      (*out)[9] = v61;
      (*out)[10] = v62;
      (*out)[11] = v63;
      (*out)[12] = v64;
      (*out)[13] = v65;
      (*out)[14] = v66;
      (*out)[15] = v67;
      ikNormalizedMatrixAssert_func(out);
      if ( v164 )
      {
        v52 = &v164[16 * boneNum];
        v37 = (*out)[1];
        v38 = (*out)[2];
        v39 = (*out)[3];
        v40 = (*out)[4];
        v41 = (*out)[5];
        v42 = (*out)[6];
        v43 = (*out)[7];
        v44 = (*out)[8];
        v45 = (*out)[9];
        v46 = (*out)[10];
        v47 = (*out)[11];
        v48 = (*out)[12];
        v49 = (*out)[13];
        v50 = (*out)[14];
        v51 = (*out)[15];
        *v52 = (*out)[0];
        v52[1] = v37;
        v52[2] = v38;
        v52[3] = v39;
        v52[4] = v40;
        v52[5] = v41;
        v52[6] = v42;
        v52[7] = v43;
        v52[8] = v44;
        v52[9] = v45;
        v52[10] = v46;
        v52[11] = v47;
        v52[12] = v48;
        v52[13] = v49;
        v52[14] = v50;
        v52[15] = v51;
        ikState->cacheActive = 1;
      }
    }
    else
    {
      ikNormalizedMatrixAssert_func((float (*)[4])&v164[16 * v142]);
      ikNormalizedMatrixAssert_func((float (*)[4])&v165[16 * boneNum]);
      v141 = &v164[16 * v142];
      v140 = &v165[16 * boneNum];
      v139 = v140;
      v123 = *v140;
      v124 = v140[1];
      v125 = v140[2];
      v126 = v140[3];
      v122 = v140 + 4;
      v127 = v140[4];
      v128 = v140[5];
      v129 = v140[6];
      v130 = v140[7];
      v121 = v140 + 8;
      v131 = v140[8];
      v132 = v140[9];
      v133 = v140[10];
      v134 = v140[11];
      v120 = v140 + 12;
      v135 = v140[12];
      v136 = v140[13];
      v137 = v140[14];
      v138 = v140[15];
      v119 = v141;
      v103 = *v141;
      v104 = v141[1];
      v105 = v141[2];
      v106 = v141[3];
      v102 = v141 + 4;
      v107 = v141[4];
      v108 = v141[5];
      v109 = v141[6];
      v110 = v141[7];
      v101 = v141 + 8;
      v111 = v141[8];
      v112 = v141[9];
      v113 = v141[10];
      v114 = v141[11];
      v100 = v141 + 12;
      v115 = v141[12];
      v116 = v141[13];
      v117 = v141[14];
      v118 = v141[15];
      v85 = (float)((float)((float)(v123 * v104) + (float)(v124 * v108)) + (float)(v125 * v112)) + (float)(v126 * v116);
      v86 = (float)((float)((float)(v123 * v105) + (float)(v124 * v109)) + (float)(v125 * v113)) + (float)(v126 * v117);
      v87 = (float)((float)((float)(v123 * v106) + (float)(v124 * v110)) + (float)(v125 * v114)) + (float)(v126 * v118);
      v88 = (float)((float)((float)(v127 * v103) + (float)(v128 * v107)) + (float)(v129 * v111)) + (float)(v130 * v115);
      v89 = (float)((float)((float)(v127 * v104) + (float)(v128 * v108)) + (float)(v129 * v112)) + (float)(v130 * v116);
      v90 = (float)((float)((float)(v127 * v105) + (float)(v128 * v109)) + (float)(v129 * v113)) + (float)(v130 * v117);
      v91 = (float)((float)((float)(v127 * v106) + (float)(v128 * v110)) + (float)(v129 * v114)) + (float)(v130 * v118);
      v92 = (float)((float)((float)(v131 * v103) + (float)(v132 * v107)) + (float)(v133 * v111)) + (float)(v134 * v115);
      v93 = (float)((float)((float)(v131 * v104) + (float)(v132 * v108)) + (float)(v133 * v112)) + (float)(v134 * v116);
      v94 = (float)((float)((float)(v131 * v105) + (float)(v132 * v109)) + (float)(v133 * v113)) + (float)(v134 * v117);
      v95 = (float)((float)((float)(v131 * v106) + (float)(v132 * v110)) + (float)(v133 * v114)) + (float)(v134 * v118);
      v96 = (float)((float)((float)(v135 * v103) + (float)(v136 * v107)) + (float)(v137 * v111)) + (float)(v138 * v115);
      v97 = (float)((float)((float)(v135 * v104) + (float)(v136 * v108)) + (float)(v137 * v112)) + (float)(v138 * v116);
      v98 = (float)((float)((float)(v135 * v105) + (float)(v136 * v109)) + (float)(v137 * v113)) + (float)(v138 * v117);
      v99 = (float)((float)((float)(v135 * v106) + (float)(v136 * v110)) + (float)(v137 * v114)) + (float)(v138 * v118);
      (*out)[0] = (float)((float)((float)(v123 * v103) + (float)(v124 * v107)) + (float)(v125 * v111))
                + (float)(v126 * v115);
      (*out)[1] = v85;
      (*out)[2] = v86;
      (*out)[3] = v87;
      (*out)[4] = v88;
      (*out)[5] = v89;
      (*out)[6] = v90;
      (*out)[7] = v91;
      (*out)[8] = v92;
      (*out)[9] = v93;
      (*out)[10] = v94;
      (*out)[11] = v95;
      (*out)[12] = v96;
      (*out)[13] = v97;
      (*out)[14] = v98;
      (*out)[15] = v99;
    }
  }
  else
  {
    ikNormalizedMatrixAssert_func((float (*)[4])&v164[16 * boneNum]);
    v163 = &v164[16 * boneNum];
    v162 = v163;
    v146 = *v163;
    v147 = v163[1];
    v148 = v163[2];
    v149 = v163[3];
    v145 = v163 + 4;
    v150 = v163[4];
    v151 = v163[5];
    v152 = v163[6];
    v153 = v163[7];
    v144 = v163 + 8;
    v154 = v163[8];
    v155 = v163[9];
    v156 = v163[10];
    v157 = v163[11];
    v143 = v163 + 12;
    v158 = v163[12];
    v159 = v163[13];
    v160 = v163[14];
    v161 = v163[15];
    (*out)[0] = v146;
    (*out)[1] = v147;
    (*out)[2] = v148;
    (*out)[3] = v149;
    (*out)[4] = v150;
    (*out)[5] = v151;
    (*out)[6] = v152;
    (*out)[7] = v153;
    (*out)[8] = v154;
    (*out)[9] = v155;
    (*out)[10] = v156;
    (*out)[11] = v157;
    (*out)[12] = v158;
    (*out)[13] = v159;
    (*out)[14] = v160;
    (*out)[15] = v161;
  }
}

void __cdecl ikCalcBoneModelMatrix(IKState *ikState, int boneNum, float *inMatArray, float (*out)[4])
{
  int savedregs; // [esp+0h] [ebp+0h] BYREF

  ikCalcBoneModelMatrix_r((float *)&savedregs, ikState, boneNum, inMatArray, out);
}

void __cdecl ikSolve2D(
        const float *BaseJoint,
        const float *ModelTarget,
        float UpperIKc,
        float LowerIKc,
        float UpperIKInvc,
        float LowerIKInvc,
        float *ModelTargetDir,
        float *SinUpper,
        float *CosUpper,
        float *SinLower,
        float *CosLower)
{
  float v11; // [esp+8h] [ebp-1Ch]
  float v12; // [esp+Ch] [ebp-18h]
  float v13; // [esp+10h] [ebp-14h]
  float v14; // [esp+14h] [ebp-10h]
  float tmp_C; // [esp+1Ch] [ebp-8h]

  *ModelTargetDir = *ModelTarget - *BaseJoint;
  ModelTargetDir[1] = ModelTarget[1] - BaseJoint[1];
  ModelTargetDir[2] = ModelTarget[2] - BaseJoint[2];
  tmp_C = sqrtf(
            (float)((float)(*ModelTargetDir * *ModelTargetDir) + (float)(ModelTargetDir[1] * ModelTargetDir[1]))
          + (float)(ModelTargetDir[2] * ModelTargetDir[2]));
  *ModelTargetDir = (float)(1.0 / tmp_C) * *ModelTargetDir;
  ModelTargetDir[1] = (float)(1.0 / tmp_C) * ModelTargetDir[1];
  ModelTargetDir[2] = (float)(1.0 / tmp_C) * ModelTargetDir[2];
  *CosUpper = (float)(UpperIKInvc * (float)(1.0 / tmp_C)) + (float)(UpperIKc * tmp_C);
  *CosLower = (float)(LowerIKInvc * (float)(1.0 / tmp_C)) + (float)(LowerIKc * tmp_C);
  v14 = *CosUpper;
  if ( 1.0 <= -1.0
    && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.h", 730, 0, "%s", "min < max") )
  {
    __debugbreak();
  }
  if ( v14 >= -1.0 )
  {
    if ( v14 <= 1.0 )
      v13 = v14;
    else
      v13 = 1.0f;
  }
  else
  {
    v13 = -1.0f;
  }
  *CosUpper = v13;
  v12 = *CosLower;
  if ( 1.0 <= -1.0
    && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.h", 730, 0, "%s", "min < max") )
  {
    __debugbreak();
  }
  if ( v12 >= -1.0 )
  {
    if ( v12 <= 1.0 )
      v11 = v12;
    else
      v11 = 1.0f;
  }
  else
  {
    v11 = -1.0f;
  }
  *CosLower = v11;
  *SinUpper = sqrtf(1.0 - (float)(*CosUpper * *CosUpper));
  *SinLower = sqrtf(1.0 - (float)(*CosLower * *CosLower));
}

// local variable allocation has failed, the output may be wrong!
void  ikMap2DTo3D(
        float a1@<ebp>,
        float UpperLimbLength,
        float SinUpper,
        float CosUpper,
        float SinLower,
        float CosLower,
        const float *ModelBaseJoint,
        const float *ModelTargetDir,
        const float *ModelMidJointDir,
        float sinTwist,
        float cosTwist,
        float (*UpperLocalToModel)[4],
        float (*LowerLocalToModel)[4])
{
  float v13; // xmm0_4
  float v14; // [esp-F8h] [ebp-2D8h]
  float v15; // [esp-F4h] [ebp-2D4h]
  float v16; // [esp-F0h] [ebp-2D0h]
  float v17; // [esp-ECh] [ebp-2CCh]
  float v18; // [esp-E8h] [ebp-2C8h]
  float v19; // [esp-E4h] [ebp-2C4h]
  float v20; // [esp-E0h] [ebp-2C0h]
  float v21; // [esp-DCh] [ebp-2BCh]
  float v22; // [esp-D8h] [ebp-2B8h]
  float v23; // [esp-D4h] [ebp-2B4h]
  float v24; // [esp-D0h] [ebp-2B0h]
  float v25; // [esp-CCh] [ebp-2ACh]
  float v26; // [esp-C8h] [ebp-2A8h]
  float v27; // [esp-C4h] [ebp-2A4h]
  float v28; // [esp-58h] [ebp-238h]
  float v29; // [esp-54h] [ebp-234h]
  float v30; // [esp-28h] [ebp-208h]
  float v31; // [esp-24h] [ebp-204h]
  float v32; // [esp+48h] [ebp-198h]
  float v33; // [esp+4Ch] [ebp-194h]
  float v34; // [esp+50h] [ebp-190h]
  float v35; // [esp+54h] [ebp-18Ch]
  float v36; // [esp+58h] [ebp-188h]
  float v37; // [esp+5Ch] [ebp-184h]
  float v38; // [esp+60h] [ebp-180h]
  float v39; // [esp+64h] [ebp-17Ch]
  float v40; // [esp+68h] [ebp-178h]
  float v41; // [esp+6Ch] [ebp-174h]
  float v42; // [esp+70h] [ebp-170h]
  float v43; // [esp+74h] [ebp-16Ch]
  float v44; // [esp+78h] [ebp-168h]
  float v45; // [esp+7Ch] [ebp-164h]
  float v46; // [esp+80h] [ebp-160h]
  _BYTE UpperRot_52[69]; // [esp+174h] [ebp-6Ch] OVERLAPPED BYREF
  float Up[3]; // [esp+1C4h] [ebp-1Ch] BYREF
  float MidJointAxis[4]; // [esp+1D0h] [ebp-10h]
  float retaddr; // [esp+1E0h] [ebp+0h]

  MidJointAxis[1] = a1;
  MidJointAxis[2] = retaddr;
  Vec3Cross(ModelTargetDir, ModelMidJointDir, Up);
  if ( Vec3Normalize(Up) < 0.0099999998 )
  {
    Up[0] = 0.0f;
    Up[1] = 0.0f;
    Up[2] = 1.0f;
  }
  Vec3Cross(Up, ModelTargetDir, (float *)&UpperRot_52[68]);
  ikMat44Set((float (*)[4])UpperRot_52, ModelTargetDir, (const float *)&UpperRot_52[68], Up, ModelBaseJoint);
  ikNormalizedMatrixAssert_func((float (*)[4])UpperRot_52);
  v32 = (float)((float)((float)(CosUpper * *(float *)&UpperRot_52[4])
                      + (float)((float)(SinUpper * cosTwist) * *(float *)&UpperRot_52[20]))
              + (float)((float)(SinUpper * sinTwist) * *(float *)&UpperRot_52[36]))
      + (float)(0.0 * *(float *)&UpperRot_52[52]);
  v33 = (float)((float)((float)(CosUpper * *(float *)&UpperRot_52[8])
                      + (float)((float)(SinUpper * cosTwist) * *(float *)&UpperRot_52[24]))
              + (float)((float)(SinUpper * sinTwist) * *(float *)&UpperRot_52[40]))
      + (float)(0.0 * *(float *)&UpperRot_52[56]);
  v34 = (float)((float)((float)(CosUpper * *(float *)&UpperRot_52[12])
                      + (float)((float)(SinUpper * cosTwist) * *(float *)&UpperRot_52[28]))
              + (float)((float)(SinUpper * sinTwist) * *(float *)&UpperRot_52[44]))
      + (float)(0.0 * *(float *)&UpperRot_52[60]);
  v35 = (float)((float)((float)(COERCE_FLOAT(LODWORD(SinUpper) ^ _mask__NegFloat_) * *(float *)UpperRot_52)
                      + (float)((float)(CosUpper * cosTwist) * *(float *)&UpperRot_52[16]))
              + (float)((float)(CosUpper * sinTwist) * *(float *)&UpperRot_52[32]))
      + (float)(0.0 * *(float *)&UpperRot_52[48]);
  v36 = (float)((float)((float)(COERCE_FLOAT(LODWORD(SinUpper) ^ _mask__NegFloat_) * *(float *)&UpperRot_52[4])
                      + (float)((float)(CosUpper * cosTwist) * *(float *)&UpperRot_52[20]))
              + (float)((float)(CosUpper * sinTwist) * *(float *)&UpperRot_52[36]))
      + (float)(0.0 * *(float *)&UpperRot_52[52]);
  v37 = (float)((float)((float)(COERCE_FLOAT(LODWORD(SinUpper) ^ _mask__NegFloat_) * *(float *)&UpperRot_52[8])
                      + (float)((float)(CosUpper * cosTwist) * *(float *)&UpperRot_52[24]))
              + (float)((float)(CosUpper * sinTwist) * *(float *)&UpperRot_52[40]))
      + (float)(0.0 * *(float *)&UpperRot_52[56]);
  v38 = (float)((float)((float)(COERCE_FLOAT(LODWORD(SinUpper) ^ _mask__NegFloat_) * *(float *)&UpperRot_52[12])
                      + (float)((float)(CosUpper * cosTwist) * *(float *)&UpperRot_52[28]))
              + (float)((float)(CosUpper * sinTwist) * *(float *)&UpperRot_52[44]))
      + (float)(0.0 * *(float *)&UpperRot_52[60]);
  v39 = (float)((float)((float)(0.0 * *(float *)UpperRot_52)
                      + (float)(COERCE_FLOAT(LODWORD(sinTwist) ^ _mask__NegFloat_) * *(float *)&UpperRot_52[16]))
              + (float)(cosTwist * *(float *)&UpperRot_52[32]))
      + (float)(0.0 * *(float *)&UpperRot_52[48]);
  v40 = (float)((float)((float)(0.0 * *(float *)&UpperRot_52[4])
                      + (float)(COERCE_FLOAT(LODWORD(sinTwist) ^ _mask__NegFloat_) * *(float *)&UpperRot_52[20]))
              + (float)(cosTwist * *(float *)&UpperRot_52[36]))
      + (float)(0.0 * *(float *)&UpperRot_52[52]);
  v41 = (float)((float)((float)(0.0 * *(float *)&UpperRot_52[8])
                      + (float)(COERCE_FLOAT(LODWORD(sinTwist) ^ _mask__NegFloat_) * *(float *)&UpperRot_52[24]))
              + (float)(cosTwist * *(float *)&UpperRot_52[40]))
      + (float)(0.0 * *(float *)&UpperRot_52[56]);
  v42 = (float)((float)((float)(0.0 * *(float *)&UpperRot_52[12])
                      + (float)(COERCE_FLOAT(LODWORD(sinTwist) ^ _mask__NegFloat_) * *(float *)&UpperRot_52[28]))
              + (float)(cosTwist * *(float *)&UpperRot_52[44]))
      + (float)(0.0 * *(float *)&UpperRot_52[60]);
  v43 = (float)((float)((float)(0.0 * *(float *)UpperRot_52) + (float)(0.0 * *(float *)&UpperRot_52[16]))
              + (float)(0.0 * *(float *)&UpperRot_52[32]))
      + (float)(1.0 * *(float *)&UpperRot_52[48]);
  v44 = (float)((float)((float)(0.0 * *(float *)&UpperRot_52[4]) + (float)(0.0 * *(float *)&UpperRot_52[20]))
              + (float)(0.0 * *(float *)&UpperRot_52[36]))
      + (float)(1.0 * *(float *)&UpperRot_52[52]);
  v45 = (float)((float)((float)(0.0 * *(float *)&UpperRot_52[8]) + (float)(0.0 * *(float *)&UpperRot_52[24]))
              + (float)(0.0 * *(float *)&UpperRot_52[40]))
      + (float)(1.0 * *(float *)&UpperRot_52[56]);
  v46 = (float)((float)((float)(0.0 * *(float *)&UpperRot_52[12]) + (float)(0.0 * *(float *)&UpperRot_52[28]))
              + (float)(0.0 * *(float *)&UpperRot_52[44]))
      + (float)(1.0 * *(float *)&UpperRot_52[60]);
  (*UpperLocalToModel)[0] = (float)((float)((float)(CosUpper * *(float *)UpperRot_52)
                                          + (float)((float)(SinUpper * cosTwist) * *(float *)&UpperRot_52[16]))
                                  + (float)((float)(SinUpper * sinTwist) * *(float *)&UpperRot_52[32]))
                          + (float)(0.0 * *(float *)&UpperRot_52[48]);
  (*UpperLocalToModel)[1] = v32;
  (*UpperLocalToModel)[2] = v33;
  (*UpperLocalToModel)[3] = v34;
  (*UpperLocalToModel)[4] = v35;
  (*UpperLocalToModel)[5] = v36;
  (*UpperLocalToModel)[6] = v37;
  (*UpperLocalToModel)[7] = v38;
  (*UpperLocalToModel)[8] = v39;
  (*UpperLocalToModel)[9] = v40;
  (*UpperLocalToModel)[10] = v41;
  (*UpperLocalToModel)[11] = v42;
  (*UpperLocalToModel)[12] = v43;
  (*UpperLocalToModel)[13] = v44;
  (*UpperLocalToModel)[14] = v45;
  (*UpperLocalToModel)[15] = v46;
  v28 = COERCE_FLOAT(LODWORD(SinLower) ^ _mask__NegFloat_) * cosTwist;
  v29 = COERCE_FLOAT(LODWORD(SinLower) ^ _mask__NegFloat_) * sinTwist;
  v30 = (float)(UpperLimbLength * SinUpper) * cosTwist;
  v31 = (float)(UpperLimbLength * SinUpper) * sinTwist;
  v14 = (float)((float)((float)(CosLower * *(float *)&UpperRot_52[4]) + (float)(v28 * *(float *)&UpperRot_52[20]))
              + (float)(v29 * *(float *)&UpperRot_52[36]))
      + (float)(0.0 * *(float *)&UpperRot_52[52]);
  v15 = (float)((float)((float)(CosLower * *(float *)&UpperRot_52[8]) + (float)(v28 * *(float *)&UpperRot_52[24]))
              + (float)(v29 * *(float *)&UpperRot_52[40]))
      + (float)(0.0 * *(float *)&UpperRot_52[56]);
  v16 = (float)((float)((float)(CosLower * *(float *)&UpperRot_52[12]) + (float)(v28 * *(float *)&UpperRot_52[28]))
              + (float)(v29 * *(float *)&UpperRot_52[44]))
      + (float)(0.0 * *(float *)&UpperRot_52[60]);
  v17 = (float)((float)((float)(SinLower * *(float *)UpperRot_52)
                      + (float)((float)(CosLower * cosTwist) * *(float *)&UpperRot_52[16]))
              + (float)((float)(CosLower * sinTwist) * *(float *)&UpperRot_52[32]))
      + (float)(0.0 * *(float *)&UpperRot_52[48]);
  v18 = (float)((float)((float)(SinLower * *(float *)&UpperRot_52[4])
                      + (float)((float)(CosLower * cosTwist) * *(float *)&UpperRot_52[20]))
              + (float)((float)(CosLower * sinTwist) * *(float *)&UpperRot_52[36]))
      + (float)(0.0 * *(float *)&UpperRot_52[52]);
  v19 = (float)((float)((float)(SinLower * *(float *)&UpperRot_52[8])
                      + (float)((float)(CosLower * cosTwist) * *(float *)&UpperRot_52[24]))
              + (float)((float)(CosLower * sinTwist) * *(float *)&UpperRot_52[40]))
      + (float)(0.0 * *(float *)&UpperRot_52[56]);
  v20 = (float)((float)((float)(SinLower * *(float *)&UpperRot_52[12])
                      + (float)((float)(CosLower * cosTwist) * *(float *)&UpperRot_52[28]))
              + (float)((float)(CosLower * sinTwist) * *(float *)&UpperRot_52[44]))
      + (float)(0.0 * *(float *)&UpperRot_52[60]);
  v21 = (float)((float)((float)(0.0 * *(float *)UpperRot_52)
                      + (float)(COERCE_FLOAT(LODWORD(sinTwist) ^ _mask__NegFloat_) * *(float *)&UpperRot_52[16]))
              + (float)(cosTwist * *(float *)&UpperRot_52[32]))
      + (float)(0.0 * *(float *)&UpperRot_52[48]);
  v22 = (float)((float)((float)(0.0 * *(float *)&UpperRot_52[4])
                      + (float)(COERCE_FLOAT(LODWORD(sinTwist) ^ _mask__NegFloat_) * *(float *)&UpperRot_52[20]))
              + (float)(cosTwist * *(float *)&UpperRot_52[36]))
      + (float)(0.0 * *(float *)&UpperRot_52[52]);
  v23 = (float)((float)((float)(0.0 * *(float *)&UpperRot_52[8])
                      + (float)(COERCE_FLOAT(LODWORD(sinTwist) ^ _mask__NegFloat_) * *(float *)&UpperRot_52[24]))
              + (float)(cosTwist * *(float *)&UpperRot_52[40]))
      + (float)(0.0 * *(float *)&UpperRot_52[56]);
  v24 = (float)((float)((float)(0.0 * *(float *)&UpperRot_52[12])
                      + (float)(COERCE_FLOAT(LODWORD(sinTwist) ^ _mask__NegFloat_) * *(float *)&UpperRot_52[28]))
              + (float)(cosTwist * *(float *)&UpperRot_52[44]))
      + (float)(0.0 * *(float *)&UpperRot_52[60]);
  v25 = (float)((float)((float)((float)(UpperLimbLength * CosUpper) * *(float *)UpperRot_52)
                      + (float)(v30 * *(float *)&UpperRot_52[16]))
              + (float)(v31 * *(float *)&UpperRot_52[32]))
      + (float)(1.0 * *(float *)&UpperRot_52[48]);
  v26 = (float)((float)((float)((float)(UpperLimbLength * CosUpper) * *(float *)&UpperRot_52[4])
                      + (float)(v30 * *(float *)&UpperRot_52[20]))
              + (float)(v31 * *(float *)&UpperRot_52[36]))
      + (float)(1.0 * *(float *)&UpperRot_52[52]);
  v27 = (float)((float)((float)((float)(UpperLimbLength * CosUpper) * *(float *)&UpperRot_52[8])
                      + (float)(v30 * *(float *)&UpperRot_52[24]))
              + (float)(v31 * *(float *)&UpperRot_52[40]))
      + (float)(1.0 * *(float *)&UpperRot_52[56]);
  v13 = (float)((float)((float)((float)(UpperLimbLength * CosUpper) * *(float *)&UpperRot_52[12])
                      + (float)(v30 * *(float *)&UpperRot_52[28]))
              + (float)(v31 * *(float *)&UpperRot_52[44]))
      + (float)(1.0 * *(float *)&UpperRot_52[60]);
  (*LowerLocalToModel)[0] = (float)((float)((float)(CosLower * *(float *)UpperRot_52)
                                          + (float)(v28 * *(float *)&UpperRot_52[16]))
                                  + (float)(v29 * *(float *)&UpperRot_52[32]))
                          + (float)(0.0 * *(float *)&UpperRot_52[48]);
  (*LowerLocalToModel)[1] = v14;
  (*LowerLocalToModel)[2] = v15;
  (*LowerLocalToModel)[3] = v16;
  (*LowerLocalToModel)[4] = v17;
  (*LowerLocalToModel)[5] = v18;
  (*LowerLocalToModel)[6] = v19;
  (*LowerLocalToModel)[7] = v20;
  (*LowerLocalToModel)[8] = v21;
  (*LowerLocalToModel)[9] = v22;
  (*LowerLocalToModel)[10] = v23;
  (*LowerLocalToModel)[11] = v24;
  (*LowerLocalToModel)[12] = v25;
  (*LowerLocalToModel)[13] = v26;
  (*LowerLocalToModel)[14] = v27;
  (*LowerLocalToModel)[15] = v13;
}

// local variable allocation has failed, the output may be wrong!
void  ikSolveLegJoint(
        int a1@<ebp>,
        IKState *ikState,
        IKJointBones *jointBones,
        IKJointVars *jointVars,
        float *footPos,
        float (*outParentMat)[4],
        float (*outJointMat)[4],
        bool flipHack)
{
  __int64 v8; // [esp+BCh] [ebp-230h] BYREF
  int v9; // [esp+C4h] [ebp-228h]
  float basePos[4]; // [esp+C8h] [ebp-224h] BYREF
  float modelMidJointDir[3]; // [esp+D8h] [ebp-214h] BYREF
  float modelTargetDir[3]; // [esp+E4h] [ebp-208h] BYREF
  float sinUpper; // [esp+F0h] [ebp-1FCh] BYREF
  float *cosUpper; // [esp+F4h] [ebp-1F8h]
  float sinLower; // [esp+F8h] [ebp-1F4h]
  float cosLower; // [esp+FCh] [ebp-1F0h]
  float v17; // [esp+100h] [ebp-1ECh] BYREF
  float v18; // [esp+104h] [ebp-1E8h]
  float footDist; // [esp+108h] [ebp-1E4h]
  _BYTE footVec[80]; // [esp+10Ch] [ebp-1E0h] OVERLAPPED BYREF
  _BYTE *v21; // [esp+15Ch] [ebp-190h]
  _BYTE v22[76]; // [esp+160h] [ebp-18Ch] OVERLAPPED BYREF
  float v23; // [esp+1ACh] [ebp-140h]
  float v24; // [esp+1B0h] [ebp-13Ch]
  float v25; // [esp+1B4h] [ebp-138h]
  float v26; // [esp+1B8h] [ebp-134h]
  float v27; // [esp+1BCh] [ebp-130h]
  float v28; // [esp+1C0h] [ebp-12Ch]
  float v29; // [esp+1C4h] [ebp-128h]
  float v30; // [esp+1C8h] [ebp-124h]
  float v31; // [esp+1CCh] [ebp-120h]
  float v32; // [esp+1D0h] [ebp-11Ch]
  float v33; // [esp+1D4h] [ebp-118h]
  float v34; // [esp+1D8h] [ebp-114h]
  float v35; // [esp+1DCh] [ebp-110h]
  float v36; // [esp+1E0h] [ebp-10Ch]
  float v37; // [esp+1E4h] [ebp-108h]
  float *v38; // [esp+1ECh] [ebp-100h]
  float *v39; // [esp+1F0h] [ebp-FCh]
  float *v40; // [esp+1F4h] [ebp-F8h]
  float v41; // [esp+1F8h] [ebp-F4h]
  float v42; // [esp+1FCh] [ebp-F0h]
  float v43; // [esp+200h] [ebp-ECh]
  float v44; // [esp+204h] [ebp-E8h]
  float v45; // [esp+208h] [ebp-E4h]
  float v46; // [esp+20Ch] [ebp-E0h]
  float v47; // [esp+210h] [ebp-DCh]
  float v48; // [esp+214h] [ebp-D8h]
  float v49; // [esp+218h] [ebp-D4h]
  float v50; // [esp+21Ch] [ebp-D0h]
  float v51; // [esp+220h] [ebp-CCh]
  float v52; // [esp+224h] [ebp-C8h]
  float v53; // [esp+228h] [ebp-C4h]
  float v54; // [esp+22Ch] [ebp-C0h]
  float v55; // [esp+230h] [ebp-BCh]
  float v56; // [esp+234h] [ebp-B8h]
  float *v57; // [esp+238h] [ebp-B4h]
  _BYTE *v58; // [esp+23Ch] [ebp-B0h]
  _BYTE *v59; // [esp+240h] [ebp-ACh]
  _BYTE *v60; // [esp+244h] [ebp-A8h]
  int v61; // [esp+248h] [ebp-A4h]
  int v62; // [esp+24Ch] [ebp-A0h]
  int v63; // [esp+250h] [ebp-9Ch]
  __int64 v64; // [esp+254h] [ebp-98h]
  __int64 v65; // [esp+25Ch] [ebp-90h]
  __int64 v66; // [esp+264h] [ebp-88h]
  __int64 v67; // [esp+26Ch] [ebp-80h]
  __int64 v68; // [esp+274h] [ebp-78h]
  __int64 v69; // [esp+27Ch] [ebp-70h]
  int v70; // [esp+284h] [ebp-68h]
  _BYTE *v71; // [esp+288h] [ebp-64h]
  float *v72; // [esp+28Ch] [ebp-60h]
  _BYTE v73[76]; // [esp+290h] [ebp-5Ch] OVERLAPPED BYREF
  unsigned int v74[3]; // [esp+2E0h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+2ECh] [ebp+0h]

  v74[0] = a1;
  v74[1] = retaddr;
  v73[75] = ikState->ikBoneToObjBone[jointBones->childBone] != 161;
  if ( v73[75] )
  {
    ikCalcBoneModelMatrix(ikState, jointBones->parentBone, (*ikState->matArrayPostIK)[0], (float (*)[4])v73);
    v72 = ikState->matArrayPostIK[jointBones->jointBone][0];
    v71 = v73;
    v61 = *(unsigned int *)v73;
    v62 = *(unsigned int *)&v73[4];
    v63 = *(unsigned int *)&v73[8];
    v64 = *(_QWORD *)&v73[12];
    v60 = &v73[16];
    v65 = *(_QWORD *)&v73[20];
    v66 = *(_QWORD *)&v73[28];
    v59 = &v73[32];
    v67 = *(_QWORD *)&v73[36];
    v68 = *(_QWORD *)&v73[44];
    v58 = &v73[48];
    v69 = *(_QWORD *)&v73[52];
    v70 = *(unsigned int *)&v73[60];
    v57 = v72;
    v41 = *v72;
    v42 = v72[1];
    v43 = v72[2];
    v44 = v72[3];
    v40 = v72 + 4;
    v45 = v72[4];
    v46 = v72[5];
    v47 = v72[6];
    v48 = v72[7];
    v39 = v72 + 8;
    v49 = v72[8];
    v50 = v72[9];
    v51 = v72[10];
    v52 = v72[11];
    v38 = v72 + 12;
    v53 = v72[12];
    v54 = v72[13];
    v55 = v72[14];
    v56 = v72[15];
    *(float *)&v22[72] = (float)((float)((float)(*(float *)v73 * v41) + (float)(*(float *)&v73[4] * v45))
                               + (float)(*(float *)&v73[8] * v49))
                       + (float)(*(float *)&v73[12] * v53);
    v23 = (float)((float)((float)(*(float *)v73 * v42) + (float)(*(float *)&v73[4] * v46))
                + (float)(*(float *)&v73[8] * v50))
        + (float)(*(float *)&v73[12] * v54);
    v24 = (float)((float)((float)(*(float *)v73 * v43) + (float)(*(float *)&v73[4] * v47))
                + (float)(*(float *)&v73[8] * v51))
        + (float)(*(float *)&v73[12] * v55);
    v25 = (float)((float)((float)(*(float *)v73 * v44) + (float)(*(float *)&v73[4] * v48))
                + (float)(*(float *)&v73[8] * v52))
        + (float)(*(float *)&v73[12] * v56);
    v26 = (float)((float)((float)(*(float *)&v73[16] * v41) + (float)(*(float *)&v73[20] * v45))
                + (float)(*(float *)&v73[24] * v49))
        + (float)(*(float *)&v73[28] * v53);
    v27 = (float)((float)((float)(*(float *)&v73[16] * v42) + (float)(*(float *)&v73[20] * v46))
                + (float)(*(float *)&v73[24] * v50))
        + (float)(*(float *)&v73[28] * v54);
    v28 = (float)((float)((float)(*(float *)&v73[16] * v43) + (float)(*(float *)&v73[20] * v47))
                + (float)(*(float *)&v73[24] * v51))
        + (float)(*(float *)&v73[28] * v55);
    v29 = (float)((float)((float)(*(float *)&v73[16] * v44) + (float)(*(float *)&v73[20] * v48))
                + (float)(*(float *)&v73[24] * v52))
        + (float)(*(float *)&v73[28] * v56);
    v30 = (float)((float)((float)(*(float *)&v73[32] * v41) + (float)(*(float *)&v73[36] * v45))
                + (float)(*(float *)&v73[40] * v49))
        + (float)(*(float *)&v73[44] * v53);
    v31 = (float)((float)((float)(*(float *)&v73[32] * v42) + (float)(*(float *)&v73[36] * v46))
                + (float)(*(float *)&v73[40] * v50))
        + (float)(*(float *)&v73[44] * v54);
    v32 = (float)((float)((float)(*(float *)&v73[32] * v43) + (float)(*(float *)&v73[36] * v47))
                + (float)(*(float *)&v73[40] * v51))
        + (float)(*(float *)&v73[44] * v55);
    v33 = (float)((float)((float)(*(float *)&v73[32] * v44) + (float)(*(float *)&v73[36] * v48))
                + (float)(*(float *)&v73[40] * v52))
        + (float)(*(float *)&v73[44] * v56);
    v34 = (float)((float)((float)(*(float *)&v73[48] * v41) + (float)(*(float *)&v73[52] * v45))
                + (float)(*(float *)&v73[56] * v49))
        + (float)(*(float *)&v73[60] * v53);
    v35 = (float)((float)((float)(*(float *)&v73[48] * v42) + (float)(*(float *)&v73[52] * v46))
                + (float)(*(float *)&v73[56] * v50))
        + (float)(*(float *)&v73[60] * v54);
    v36 = (float)((float)((float)(*(float *)&v73[48] * v43) + (float)(*(float *)&v73[52] * v47))
                + (float)(*(float *)&v73[56] * v51))
        + (float)(*(float *)&v73[60] * v55);
    v37 = (float)((float)((float)(*(float *)&v73[48] * v44) + (float)(*(float *)&v73[52] * v48))
                + (float)(*(float *)&v73[56] * v52))
        + (float)(*(float *)&v73[60] * v56);
    v21 = v22;
    *(float *)v22 = *(float *)&v22[72];
    *(float *)&v22[4] = v23;
    *(float *)&v22[8] = v24;
    *(float *)&v22[12] = v25;
    *(unsigned int *)&footVec[76] = &v22[16];
    *(float *)&v22[16] = v26;
    *(float *)&v22[20] = v27;
    *(float *)&v22[24] = v28;
    *(float *)&v22[28] = v29;
    *(unsigned int *)&footVec[72] = &v22[32];
    *(float *)&v22[32] = v30;
    *(float *)&v22[36] = v31;
    *(float *)&v22[40] = v32;
    *(float *)&v22[44] = v33;
    *(unsigned int *)&footVec[68] = &v22[48];
    *(float *)&v22[48] = v34;
    *(float *)&v22[52] = v35;
    *(float *)&v22[56] = v36;
    *(float *)&v22[60] = v37;
    ikCalcBoneModelMatrix(ikState, jointBones->childBone, (*ikState->matArrayPreIK)[0], (float (*)[4])&footVec[4]);
    if ( flipHack )
    {
      IK_FlipHack((float (*)[4])v73);
      IK_FlipHack((float (*)[4])v22);
      IK_FlipHack((float (*)[4])&footVec[4]);
    }
    *(unsigned int *)footVec = &v73[48];
    v17 = *footPos - *(float *)&v73[48];
    v18 = footPos[1] - *(float *)&v73[52];
    footDist = footPos[2] - *(float *)&v73[56];
    cosLower = Vec3Normalize(&v17);
    if ( cosLower > (float)((float)(jointVars->UpperLength + jointVars->LowerLength) - fMaxFootReachSubtract) )
    {
      sinLower = (float)(jointVars->UpperLength + jointVars->LowerLength) - fMaxFootReachSubtract;
      cosUpper = (float *)&v73[48];
      *footPos = (float)(sinLower * v17) + *(float *)&v73[48];
      footPos[1] = (float)(sinLower * v18) + cosUpper[1];
      footPos[2] = (float)(sinLower * footDist) + cosUpper[2];
    }
    ikSolve2D(
      (const float *)&v73[48],
      footPos,
      jointVars->UpperIKc,
      jointVars->LowerIKc,
      jointVars->UpperIKInvc,
      jointVars->LowerIKInvc,
      modelMidJointDir,
      modelTargetDir,
      &modelTargetDir[1],
      &modelTargetDir[2],
      &sinUpper);
    Vec3Cross((const float *)&v73[32], modelMidJointDir, &basePos[1]);
    LODWORD(basePos[0]) = &v73[48];
    v8 = *(_QWORD *)&v73[48];
    v9 = *(unsigned int *)&v73[56];
    ikMap2DTo3D(
      COERCE_FLOAT(v74),
      jointVars->UpperLength,
      modelTargetDir[0],
      modelTargetDir[1],
      modelTargetDir[2],
      sinUpper,
      (const float *)&v8,
      modelMidJointDir,
      &basePos[1],
      0.0,
      1.0,
      outParentMat,
      outJointMat);
    if ( ((LODWORD((*outParentMat)[0]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outParentMat)[1]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outParentMat)[2]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outParentMat)[3]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp",
            925,
            0,
            "%s",
            "!IS_NAN((outParentMat[0])[0]) && !IS_NAN((outParentMat[0])[1]) && !IS_NAN((outParentMat[0])[2]) && !IS_NAN(("
            "outParentMat[0])[3])") )
    {
      __debugbreak();
    }
    if ( ((LODWORD((*outParentMat)[4]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outParentMat)[5]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outParentMat)[6]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outParentMat)[7]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp",
            926,
            0,
            "%s",
            "!IS_NAN((outParentMat[1])[0]) && !IS_NAN((outParentMat[1])[1]) && !IS_NAN((outParentMat[1])[2]) && !IS_NAN(("
            "outParentMat[1])[3])") )
    {
      __debugbreak();
    }
    if ( ((LODWORD((*outParentMat)[8]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outParentMat)[9]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outParentMat)[10]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outParentMat)[11]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp",
            927,
            0,
            "%s",
            "!IS_NAN((outParentMat[2])[0]) && !IS_NAN((outParentMat[2])[1]) && !IS_NAN((outParentMat[2])[2]) && !IS_NAN(("
            "outParentMat[2])[3])") )
    {
      __debugbreak();
    }
    if ( ((LODWORD((*outParentMat)[12]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outParentMat)[13]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outParentMat)[14]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outParentMat)[15]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp",
            928,
            0,
            "%s",
            "!IS_NAN((outParentMat[3])[0]) && !IS_NAN((outParentMat[3])[1]) && !IS_NAN((outParentMat[3])[2]) && !IS_NAN(("
            "outParentMat[3])[3])") )
    {
      __debugbreak();
    }
    if ( ((LODWORD((*outJointMat)[0]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outJointMat)[1]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outJointMat)[2]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outJointMat)[3]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp",
            930,
            0,
            "%s",
            "!IS_NAN((outJointMat[0])[0]) && !IS_NAN((outJointMat[0])[1]) && !IS_NAN((outJointMat[0])[2]) && !IS_NAN((out"
            "JointMat[0])[3])") )
    {
      __debugbreak();
    }
    if ( ((LODWORD((*outJointMat)[4]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outJointMat)[5]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outJointMat)[6]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outJointMat)[7]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp",
            931,
            0,
            "%s",
            "!IS_NAN((outJointMat[1])[0]) && !IS_NAN((outJointMat[1])[1]) && !IS_NAN((outJointMat[1])[2]) && !IS_NAN((out"
            "JointMat[1])[3])") )
    {
      __debugbreak();
    }
    if ( ((LODWORD((*outJointMat)[8]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outJointMat)[9]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outJointMat)[10]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outJointMat)[11]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp",
            932,
            0,
            "%s",
            "!IS_NAN((outJointMat[2])[0]) && !IS_NAN((outJointMat[2])[1]) && !IS_NAN((outJointMat[2])[2]) && !IS_NAN((out"
            "JointMat[2])[3])") )
    {
      __debugbreak();
    }
    if ( ((LODWORD((*outJointMat)[12]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outJointMat)[13]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outJointMat)[14]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outJointMat)[15]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp",
            933,
            0,
            "%s",
            "!IS_NAN((outJointMat[3])[0]) && !IS_NAN((outJointMat[3])[1]) && !IS_NAN((outJointMat[3])[2]) && !IS_NAN((out"
            "JointMat[3])[3])") )
    {
      __debugbreak();
    }
    ikNormalizedMatrixAssert_func(outParentMat);
    ikNormalizedMatrixAssert_func(outJointMat);
    if ( flipHack )
    {
      IK_FlipHack(outParentMat);
      IK_FlipHack(outJointMat);
    }
  }
}

// local variable allocation has failed, the output may be wrong!
void  ikSolveArmJoint(
        int a1@<ebp>,
        IKState *ikState,
        IKJointBones *jointBones,
        IKJointVars *jointVars,
        float *handPos,
        float (*outParentMat)[4],
        float (*outJointMat)[4],
        bool flipHack)
{
  __int64 v8; // [esp+B4h] [ebp-238h] BYREF
  int v9; // [esp+BCh] [ebp-230h]
  float basePos[4]; // [esp+C0h] [ebp-22Ch] BYREF
  float modelMidJointDir[3]; // [esp+D0h] [ebp-21Ch] BYREF
  float modelTargetDir[3]; // [esp+E4h] [ebp-208h] BYREF
  float sinUpper; // [esp+F0h] [ebp-1FCh] BYREF
  float *cosUpper; // [esp+F4h] [ebp-1F8h]
  float sinLower; // [esp+F8h] [ebp-1F4h]
  float cosLower; // [esp+FCh] [ebp-1F0h]
  float v17; // [esp+100h] [ebp-1ECh] BYREF
  float v18; // [esp+104h] [ebp-1E8h]
  float handDist; // [esp+108h] [ebp-1E4h]
  _BYTE handVec[80]; // [esp+10Ch] [ebp-1E0h] OVERLAPPED BYREF
  _BYTE *v21; // [esp+15Ch] [ebp-190h]
  _BYTE v22[76]; // [esp+160h] [ebp-18Ch] OVERLAPPED BYREF
  float v23; // [esp+1ACh] [ebp-140h]
  float v24; // [esp+1B0h] [ebp-13Ch]
  float v25; // [esp+1B4h] [ebp-138h]
  float v26; // [esp+1B8h] [ebp-134h]
  float v27; // [esp+1BCh] [ebp-130h]
  float v28; // [esp+1C0h] [ebp-12Ch]
  float v29; // [esp+1C4h] [ebp-128h]
  float v30; // [esp+1C8h] [ebp-124h]
  float v31; // [esp+1CCh] [ebp-120h]
  float v32; // [esp+1D0h] [ebp-11Ch]
  float v33; // [esp+1D4h] [ebp-118h]
  float v34; // [esp+1D8h] [ebp-114h]
  float v35; // [esp+1DCh] [ebp-110h]
  float v36; // [esp+1E0h] [ebp-10Ch]
  float v37; // [esp+1E4h] [ebp-108h]
  float *v38; // [esp+1ECh] [ebp-100h]
  float *v39; // [esp+1F0h] [ebp-FCh]
  float *v40; // [esp+1F4h] [ebp-F8h]
  float v41; // [esp+1F8h] [ebp-F4h]
  float v42; // [esp+1FCh] [ebp-F0h]
  float v43; // [esp+200h] [ebp-ECh]
  float v44; // [esp+204h] [ebp-E8h]
  float v45; // [esp+208h] [ebp-E4h]
  float v46; // [esp+20Ch] [ebp-E0h]
  float v47; // [esp+210h] [ebp-DCh]
  float v48; // [esp+214h] [ebp-D8h]
  float v49; // [esp+218h] [ebp-D4h]
  float v50; // [esp+21Ch] [ebp-D0h]
  float v51; // [esp+220h] [ebp-CCh]
  float v52; // [esp+224h] [ebp-C8h]
  float v53; // [esp+228h] [ebp-C4h]
  float v54; // [esp+22Ch] [ebp-C0h]
  float v55; // [esp+230h] [ebp-BCh]
  float v56; // [esp+234h] [ebp-B8h]
  float *v57; // [esp+238h] [ebp-B4h]
  _BYTE *v58; // [esp+23Ch] [ebp-B0h]
  _BYTE *v59; // [esp+240h] [ebp-ACh]
  _BYTE *v60; // [esp+244h] [ebp-A8h]
  int v61; // [esp+248h] [ebp-A4h]
  int v62; // [esp+24Ch] [ebp-A0h]
  int v63; // [esp+250h] [ebp-9Ch]
  __int64 v64; // [esp+254h] [ebp-98h]
  __int64 v65; // [esp+25Ch] [ebp-90h]
  __int64 v66; // [esp+264h] [ebp-88h]
  __int64 v67; // [esp+26Ch] [ebp-80h]
  __int64 v68; // [esp+274h] [ebp-78h]
  __int64 v69; // [esp+27Ch] [ebp-70h]
  int v70; // [esp+284h] [ebp-68h]
  _BYTE *v71; // [esp+288h] [ebp-64h]
  float *v72; // [esp+28Ch] [ebp-60h]
  _BYTE v73[76]; // [esp+290h] [ebp-5Ch] OVERLAPPED BYREF
  unsigned int v74[3]; // [esp+2E0h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+2ECh] [ebp+0h]

  v74[0] = a1;
  v74[1] = retaddr;
  v73[75] = ikState->ikBoneToObjBone[jointBones->childBone] != 161;
  if ( v73[75] )
  {
    ikCalcBoneModelMatrix(ikState, jointBones->parentBone, (*ikState->matArrayPostIK)[0], (float (*)[4])v73);
    v72 = ikState->matArrayPostIK[jointBones->jointBone][0];
    v71 = v73;
    v61 = *(unsigned int *)v73;
    v62 = *(unsigned int *)&v73[4];
    v63 = *(unsigned int *)&v73[8];
    v64 = *(_QWORD *)&v73[12];
    v60 = &v73[16];
    v65 = *(_QWORD *)&v73[20];
    v66 = *(_QWORD *)&v73[28];
    v59 = &v73[32];
    v67 = *(_QWORD *)&v73[36];
    v68 = *(_QWORD *)&v73[44];
    v58 = &v73[48];
    v69 = *(_QWORD *)&v73[52];
    v70 = *(unsigned int *)&v73[60];
    v57 = v72;
    v41 = *v72;
    v42 = v72[1];
    v43 = v72[2];
    v44 = v72[3];
    v40 = v72 + 4;
    v45 = v72[4];
    v46 = v72[5];
    v47 = v72[6];
    v48 = v72[7];
    v39 = v72 + 8;
    v49 = v72[8];
    v50 = v72[9];
    v51 = v72[10];
    v52 = v72[11];
    v38 = v72 + 12;
    v53 = v72[12];
    v54 = v72[13];
    v55 = v72[14];
    v56 = v72[15];
    *(float *)&v22[72] = (float)((float)((float)(*(float *)v73 * v41) + (float)(*(float *)&v73[4] * v45))
                               + (float)(*(float *)&v73[8] * v49))
                       + (float)(*(float *)&v73[12] * v53);
    v23 = (float)((float)((float)(*(float *)v73 * v42) + (float)(*(float *)&v73[4] * v46))
                + (float)(*(float *)&v73[8] * v50))
        + (float)(*(float *)&v73[12] * v54);
    v24 = (float)((float)((float)(*(float *)v73 * v43) + (float)(*(float *)&v73[4] * v47))
                + (float)(*(float *)&v73[8] * v51))
        + (float)(*(float *)&v73[12] * v55);
    v25 = (float)((float)((float)(*(float *)v73 * v44) + (float)(*(float *)&v73[4] * v48))
                + (float)(*(float *)&v73[8] * v52))
        + (float)(*(float *)&v73[12] * v56);
    v26 = (float)((float)((float)(*(float *)&v73[16] * v41) + (float)(*(float *)&v73[20] * v45))
                + (float)(*(float *)&v73[24] * v49))
        + (float)(*(float *)&v73[28] * v53);
    v27 = (float)((float)((float)(*(float *)&v73[16] * v42) + (float)(*(float *)&v73[20] * v46))
                + (float)(*(float *)&v73[24] * v50))
        + (float)(*(float *)&v73[28] * v54);
    v28 = (float)((float)((float)(*(float *)&v73[16] * v43) + (float)(*(float *)&v73[20] * v47))
                + (float)(*(float *)&v73[24] * v51))
        + (float)(*(float *)&v73[28] * v55);
    v29 = (float)((float)((float)(*(float *)&v73[16] * v44) + (float)(*(float *)&v73[20] * v48))
                + (float)(*(float *)&v73[24] * v52))
        + (float)(*(float *)&v73[28] * v56);
    v30 = (float)((float)((float)(*(float *)&v73[32] * v41) + (float)(*(float *)&v73[36] * v45))
                + (float)(*(float *)&v73[40] * v49))
        + (float)(*(float *)&v73[44] * v53);
    v31 = (float)((float)((float)(*(float *)&v73[32] * v42) + (float)(*(float *)&v73[36] * v46))
                + (float)(*(float *)&v73[40] * v50))
        + (float)(*(float *)&v73[44] * v54);
    v32 = (float)((float)((float)(*(float *)&v73[32] * v43) + (float)(*(float *)&v73[36] * v47))
                + (float)(*(float *)&v73[40] * v51))
        + (float)(*(float *)&v73[44] * v55);
    v33 = (float)((float)((float)(*(float *)&v73[32] * v44) + (float)(*(float *)&v73[36] * v48))
                + (float)(*(float *)&v73[40] * v52))
        + (float)(*(float *)&v73[44] * v56);
    v34 = (float)((float)((float)(*(float *)&v73[48] * v41) + (float)(*(float *)&v73[52] * v45))
                + (float)(*(float *)&v73[56] * v49))
        + (float)(*(float *)&v73[60] * v53);
    v35 = (float)((float)((float)(*(float *)&v73[48] * v42) + (float)(*(float *)&v73[52] * v46))
                + (float)(*(float *)&v73[56] * v50))
        + (float)(*(float *)&v73[60] * v54);
    v36 = (float)((float)((float)(*(float *)&v73[48] * v43) + (float)(*(float *)&v73[52] * v47))
                + (float)(*(float *)&v73[56] * v51))
        + (float)(*(float *)&v73[60] * v55);
    v37 = (float)((float)((float)(*(float *)&v73[48] * v44) + (float)(*(float *)&v73[52] * v48))
                + (float)(*(float *)&v73[56] * v52))
        + (float)(*(float *)&v73[60] * v56);
    v21 = v22;
    *(float *)v22 = *(float *)&v22[72];
    *(float *)&v22[4] = v23;
    *(float *)&v22[8] = v24;
    *(float *)&v22[12] = v25;
    *(unsigned int *)&handVec[76] = &v22[16];
    *(float *)&v22[16] = v26;
    *(float *)&v22[20] = v27;
    *(float *)&v22[24] = v28;
    *(float *)&v22[28] = v29;
    *(unsigned int *)&handVec[72] = &v22[32];
    *(float *)&v22[32] = v30;
    *(float *)&v22[36] = v31;
    *(float *)&v22[40] = v32;
    *(float *)&v22[44] = v33;
    *(unsigned int *)&handVec[68] = &v22[48];
    *(float *)&v22[48] = v34;
    *(float *)&v22[52] = v35;
    *(float *)&v22[56] = v36;
    *(float *)&v22[60] = v37;
    ikCalcBoneModelMatrix(ikState, jointBones->childBone, (*ikState->matArrayPreIK)[0], (float (*)[4])&handVec[4]);
    if ( flipHack )
    {
      IK_FlipHack((float (*)[4])v73);
      IK_FlipHack((float (*)[4])v22);
      IK_FlipHack((float (*)[4])&handVec[4]);
    }
    *(unsigned int *)handVec = &v73[48];
    v17 = *handPos - *(float *)&v73[48];
    v18 = handPos[1] - *(float *)&v73[52];
    handDist = handPos[2] - *(float *)&v73[56];
    cosLower = Vec3Normalize(&v17);
    if ( cosLower > (float)((float)(jointVars->UpperLength + jointVars->LowerLength) - fMaxHandReachSubtract) )
    {
      sinLower = (float)(jointVars->UpperLength + jointVars->LowerLength) - fMaxHandReachSubtract;
      cosUpper = (float *)&v73[48];
      *handPos = (float)(sinLower * v17) + *(float *)&v73[48];
      handPos[1] = (float)(sinLower * v18) + cosUpper[1];
      handPos[2] = (float)(sinLower * handDist) + cosUpper[2];
    }
    ikSolve2D(
      (const float *)&v73[48],
      handPos,
      jointVars->UpperIKc,
      jointVars->LowerIKc,
      jointVars->UpperIKInvc,
      jointVars->LowerIKInvc,
      &modelMidJointDir[2],
      modelTargetDir,
      &modelTargetDir[1],
      &modelTargetDir[2],
      &sinUpper);
    modelMidJointDir[1] = 0.0f;
    modelMidJointDir[0] = 1.0f;
    Vec3Cross((const float *)&v73[32], &modelMidJointDir[2], &basePos[1]);
    LODWORD(basePos[0]) = &v73[48];
    v8 = *(_QWORD *)&v73[48];
    v9 = *(unsigned int *)&v73[56];
    ikMap2DTo3D(
      COERCE_FLOAT(v74),
      jointVars->UpperLength,
      modelTargetDir[0],
      modelTargetDir[1],
      modelTargetDir[2],
      sinUpper,
      (const float *)&v8,
      &modelMidJointDir[2],
      &basePos[1],
      modelMidJointDir[1],
      modelMidJointDir[0],
      outParentMat,
      outJointMat);
    if ( ((LODWORD((*outParentMat)[0]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outParentMat)[1]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outParentMat)[2]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outParentMat)[3]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp",
            995,
            0,
            "%s",
            "!IS_NAN((outParentMat[0])[0]) && !IS_NAN((outParentMat[0])[1]) && !IS_NAN((outParentMat[0])[2]) && !IS_NAN(("
            "outParentMat[0])[3])") )
    {
      __debugbreak();
    }
    if ( ((LODWORD((*outParentMat)[4]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outParentMat)[5]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outParentMat)[6]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outParentMat)[7]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp",
            996,
            0,
            "%s",
            "!IS_NAN((outParentMat[1])[0]) && !IS_NAN((outParentMat[1])[1]) && !IS_NAN((outParentMat[1])[2]) && !IS_NAN(("
            "outParentMat[1])[3])") )
    {
      __debugbreak();
    }
    if ( ((LODWORD((*outParentMat)[8]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outParentMat)[9]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outParentMat)[10]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outParentMat)[11]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp",
            997,
            0,
            "%s",
            "!IS_NAN((outParentMat[2])[0]) && !IS_NAN((outParentMat[2])[1]) && !IS_NAN((outParentMat[2])[2]) && !IS_NAN(("
            "outParentMat[2])[3])") )
    {
      __debugbreak();
    }
    if ( ((LODWORD((*outParentMat)[12]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outParentMat)[13]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outParentMat)[14]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outParentMat)[15]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp",
            998,
            0,
            "%s",
            "!IS_NAN((outParentMat[3])[0]) && !IS_NAN((outParentMat[3])[1]) && !IS_NAN((outParentMat[3])[2]) && !IS_NAN(("
            "outParentMat[3])[3])") )
    {
      __debugbreak();
    }
    if ( ((LODWORD((*outJointMat)[0]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outJointMat)[1]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outJointMat)[2]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outJointMat)[3]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp",
            1000,
            0,
            "%s",
            "!IS_NAN((outJointMat[0])[0]) && !IS_NAN((outJointMat[0])[1]) && !IS_NAN((outJointMat[0])[2]) && !IS_NAN((out"
            "JointMat[0])[3])") )
    {
      __debugbreak();
    }
    if ( ((LODWORD((*outJointMat)[4]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outJointMat)[5]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outJointMat)[6]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outJointMat)[7]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp",
            1001,
            0,
            "%s",
            "!IS_NAN((outJointMat[1])[0]) && !IS_NAN((outJointMat[1])[1]) && !IS_NAN((outJointMat[1])[2]) && !IS_NAN((out"
            "JointMat[1])[3])") )
    {
      __debugbreak();
    }
    if ( ((LODWORD((*outJointMat)[8]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outJointMat)[9]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outJointMat)[10]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outJointMat)[11]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp",
            1002,
            0,
            "%s",
            "!IS_NAN((outJointMat[2])[0]) && !IS_NAN((outJointMat[2])[1]) && !IS_NAN((outJointMat[2])[2]) && !IS_NAN((out"
            "JointMat[2])[3])") )
    {
      __debugbreak();
    }
    if ( ((LODWORD((*outJointMat)[12]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outJointMat)[13]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outJointMat)[14]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*outJointMat)[15]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp",
            1003,
            0,
            "%s",
            "!IS_NAN((outJointMat[3])[0]) && !IS_NAN((outJointMat[3])[1]) && !IS_NAN((outJointMat[3])[2]) && !IS_NAN((out"
            "JointMat[3])[3])") )
    {
      __debugbreak();
    }
    ikNormalizedMatrixAssert_func(outParentMat);
    ikNormalizedMatrixAssert_func(outJointMat);
    if ( flipHack )
    {
      IK_FlipHack(outParentMat);
      IK_FlipHack(outJointMat);
    }
  }
}

void __cdecl IK_FlipHack(float (*mat)[4])
{
  int i; // [esp+0h] [ebp-4h]

  for ( i = 0; i < 2; ++i )
  {
    LODWORD((*mat)[4 * i]) ^= _mask__NegFloat_;
    LODWORD((*mat)[4 * i + 1]) ^= _mask__NegFloat_;
    LODWORD((*mat)[4 * i + 2]) ^= _mask__NegFloat_;
  }
}

void __cdecl ikNormalizedMatrixAssert_func(float (*mat)[4])
{
  const char *v1; // eax
  const char *v2; // eax
  const char *v3; // eax
  float v4; // xmm2_4
  const char *v5; // eax
  int j; // [esp+5Ch] [ebp-Ch]
  int ja; // [esp+5Ch] [ebp-Ch]
  float len; // [esp+60h] [ebp-8h]

  if ( ikSystem.dvars[14] && ikSystem.dvars[14]->current.enabled )
  {
    if ( ((LODWORD((*mat)[0]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*mat)[1]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*mat)[2]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*mat)[3]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp",
            1049,
            0,
            "%s",
            "!IS_NAN((mat[0])[0]) && !IS_NAN((mat[0])[1]) && !IS_NAN((mat[0])[2]) && !IS_NAN((mat[0])[3])") )
    {
      __debugbreak();
    }
    if ( ((LODWORD((*mat)[4]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*mat)[5]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*mat)[6]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*mat)[7]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp",
            1050,
            0,
            "%s",
            "!IS_NAN((mat[1])[0]) && !IS_NAN((mat[1])[1]) && !IS_NAN((mat[1])[2]) && !IS_NAN((mat[1])[3])") )
    {
      __debugbreak();
    }
    if ( ((LODWORD((*mat)[8]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*mat)[9]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*mat)[10]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*mat)[11]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp",
            1051,
            0,
            "%s",
            "!IS_NAN((mat[2])[0]) && !IS_NAN((mat[2])[1]) && !IS_NAN((mat[2])[2]) && !IS_NAN((mat[2])[3])") )
    {
      __debugbreak();
    }
    if ( ((LODWORD((*mat)[12]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*mat)[13]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*mat)[14]) & 0x7F800000) == 0x7F800000
       || (LODWORD((*mat)[15]) & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp",
            1052,
            0,
            "%s",
            "!IS_NAN((mat[3])[0]) && !IS_NAN((mat[3])[1]) && !IS_NAN((mat[3])[2]) && !IS_NAN((mat[3])[3])") )
    {
      __debugbreak();
    }
    for ( j = 0; j < 3; ++j )
    {
      if ( (*mat)[4 * j + 3] != 0.0 )
      {
        v1 = va("mat[%i][3] == %f", j, (*mat)[4 * j + 3]);
        if ( !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp",
                1057,
                0,
                "%s\n\t%s",
                "mat[j][3] == 0.0f",
                v1) )
          __debugbreak();
      }
      len = Abs(&(*mat)[4 * j]);
      if ( fabs(1.0 - len) >= 0.0020000001 )
      {
        v2 = va("len == %f", len);
        if ( !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp",
                1059,
                0,
                "%s\n\t%s",
                "fabs( 1.0f - len ) < IK_SANITYCHECKACCURACY",
                v2) )
          __debugbreak();
      }
    }
    if ( fabs(1.0 - (*mat)[15]) >= 0.0020000001 )
    {
      v3 = va("mat[3][3] == %f", (*mat)[15]);
      if ( !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp",
              1062,
              0,
              "%s\n\t%s",
              "fabs( 1.0f - mat[3][3] ) < IK_SANITYCHECKACCURACY",
              v3) )
        __debugbreak();
    }
    for ( ja = 0; ja < 3; ++ja )
    {
      if ( ja >= 2 )
        v4 = (float)((float)((*mat)[4 * ja] * (*mat)[0]) + (float)((*mat)[4 * ja + 1] * (*mat)[1]))
           + (float)((*mat)[4 * ja + 2] * (*mat)[2]);
      else
        v4 = (float)((float)((*mat)[4 * ja] * (*mat)[4 * ja + 4]) + (float)((*mat)[4 * ja + 1] * (*mat)[4 * ja + 5]))
           + (float)((*mat)[4 * ja + 2] * (*mat)[4 * ja + 6]);
      if ( fabs(v4) >= 0.0020000001 )
      {
        v5 = va("dot == %f", v4);
        if ( !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.cpp",
                1076,
                0,
                "%s\n\t%s",
                "fabs( dot ) < IK_SANITYCHECKACCURACY",
                v5) )
          __debugbreak();
      }
    }
  }
}

// local variable allocation has failed, the output may be wrong!
void  ikRotateBone(float *a1@<ebp>, IKState *ikState, IKBoneNames boneName, float *rot, bool local)
{
  __int64 v5; // [esp+34h] [ebp-24Ch]
  __int64 v6; // [esp+3Ch] [ebp-244h]
  __int64 v7; // [esp+44h] [ebp-23Ch]
  __int64 v8; // [esp+4Ch] [ebp-234h]
  __int64 v9; // [esp+54h] [ebp-22Ch]
  __int64 v10; // [esp+5Ch] [ebp-224h]
  __int64 v11; // [esp+64h] [ebp-21Ch]
  __int64 v12; // [esp+94h] [ebp-1ECh]
  __int64 v13; // [esp+9Ch] [ebp-1E4h]
  __int64 v14; // [esp+A4h] [ebp-1DCh]
  __int64 v15; // [esp+ACh] [ebp-1D4h]
  __int64 v16; // [esp+B4h] [ebp-1CCh]
  __int64 v17; // [esp+BCh] [ebp-1C4h]
  __int64 v18; // [esp+C4h] [ebp-1BCh]
  float v19[5]; // [esp+D0h] [ebp-1B0h] BYREF
  float v20[4]; // [esp+E4h] [ebp-19Ch] BYREF
  float v21; // [esp+F4h] [ebp-18Ch]
  float v22; // [esp+F8h] [ebp-188h]
  float v23; // [esp+FCh] [ebp-184h]
  int v24; // [esp+100h] [ebp-180h]
  float v25; // [esp+104h] [ebp-17Ch]
  float v26; // [esp+108h] [ebp-178h]
  float v27; // [esp+10Ch] [ebp-174h]
  int v28; // [esp+110h] [ebp-170h]
  _BYTE *v29; // [esp+12Ch] [ebp-154h]
  _BYTE *v30; // [esp+130h] [ebp-150h]
  float v31; // [esp+134h] [ebp-14Ch]
  float v32; // [esp+138h] [ebp-148h]
  float v33; // [esp+13Ch] [ebp-144h]
  _BYTE workMat2[224]; // [esp+140h] [ebp-140h] OVERLAPPED BYREF
  float v35[3]; // [esp+240h] [ebp-40h] BYREF
  float axis[3][3]; // [esp+24Ch] [ebp-34h]
  float (*boneMat)[4][4]; // [esp+274h] [ebp-Ch]
  void *v38; // [esp+278h] [ebp-8h] OVERLAPPED
  void *retaddr; // [esp+280h] [ebp+0h]

  boneMat = (float (*)[4][4])a1;
  v38 = retaddr;
  HIBYTE(axis[2][2]) = 1;
  if ( IK_IsCalcBone(ikState, boneName) )
  {
    LODWORD(axis[2][1]) = ikState->matArrayPostIK[boneName];
    LODWORD(axis[2][0]) = boneName - 10;
    switch ( boneName )
    {
      case IKBONE_LHAND:
        LODWORD(axis[2][1]) = ikState->handMats;
        break;
      case IKBONE_RHAND:
        LODWORD(axis[2][1]) = ikState->handMats[1];
        break;
      case IKBONE_LFOOT:
        LODWORD(axis[2][1]) = ikState->footMats;
        break;
      case IKBONE_RFOOT:
        LODWORD(axis[2][1]) = ikState->footMats[1];
        break;
      default:
        HIBYTE(axis[2][2]) = 0;
        break;
    }
    AnglesToAxis(rot, (float (*)[3])v35);
    ikMatrixSet44((float (*)[4])&workMat2[180], vec3_origin, (const float (*)[3])v35, 1.0);
    if ( local )
    {
      v5 = *(_QWORD *)(LODWORD(axis[2][1]) + 8);
      v6 = *(_QWORD *)(LODWORD(axis[2][1]) + 16);
      v7 = *(_QWORD *)(LODWORD(axis[2][1]) + 24);
      v8 = *(_QWORD *)(LODWORD(axis[2][1]) + 32);
      v9 = *(_QWORD *)(LODWORD(axis[2][1]) + 40);
      v10 = *(_QWORD *)(LODWORD(axis[2][1]) + 48);
      v11 = *(_QWORD *)(LODWORD(axis[2][1]) + 56);
      *(_QWORD *)&workMat2[52] = *(_QWORD *)LODWORD(axis[2][1]);
      *(_QWORD *)&workMat2[60] = v5;
      *(_QWORD *)&workMat2[68] = v6;
      *(_QWORD *)&workMat2[76] = v7;
      *(_QWORD *)&workMat2[84] = v8;
      *(_QWORD *)&workMat2[92] = v9;
      *(_QWORD *)&workMat2[100] = v10;
      *(_QWORD *)&workMat2[108] = v11;
      *(float *)LODWORD(axis[2][1]) = (float)((float)(*(float *)&workMat2[52] * *(float *)&workMat2[180])
                                            + (float)(*(float *)&workMat2[56] * *(float *)&workMat2[196]))
                                    + (float)(*(float *)&v5 * *(float *)&workMat2[212]);
      *(float *)(LODWORD(axis[2][1]) + 4) = (float)((float)(*(float *)&workMat2[52] * *(float *)&workMat2[184])
                                                  + (float)(*(float *)&workMat2[56] * *(float *)&workMat2[200]))
                                          + (float)(*(float *)&workMat2[60] * *(float *)&workMat2[216]);
      *(float *)(LODWORD(axis[2][1]) + 8) = (float)((float)(*(float *)&workMat2[52] * *(float *)&workMat2[188])
                                                  + (float)(*(float *)&workMat2[56] * *(float *)&workMat2[204]))
                                          + (float)(*(float *)&workMat2[60] * *(float *)&workMat2[220]);
      *(unsigned int *)(LODWORD(axis[2][1]) + 12) = 0;
      *(float *)(LODWORD(axis[2][1]) + 16) = (float)((float)(*(float *)&workMat2[68] * *(float *)&workMat2[180])
                                                   + (float)(*(float *)&workMat2[72] * *(float *)&workMat2[196]))
                                           + (float)(*(float *)&workMat2[76] * *(float *)&workMat2[212]);
      *(float *)(LODWORD(axis[2][1]) + 20) = (float)((float)(*(float *)&workMat2[68] * *(float *)&workMat2[184])
                                                   + (float)(*(float *)&workMat2[72] * *(float *)&workMat2[200]))
                                           + (float)(*(float *)&workMat2[76] * *(float *)&workMat2[216]);
      *(float *)(LODWORD(axis[2][1]) + 24) = (float)((float)(*(float *)&workMat2[68] * *(float *)&workMat2[188])
                                                   + (float)(*(float *)&workMat2[72] * *(float *)&workMat2[204]))
                                           + (float)(*(float *)&workMat2[76] * *(float *)&workMat2[220]);
      *(unsigned int *)(LODWORD(axis[2][1]) + 28) = 0;
      *(float *)(LODWORD(axis[2][1]) + 32) = (float)((float)(*(float *)&workMat2[84] * *(float *)&workMat2[180])
                                                   + (float)(*(float *)&workMat2[88] * *(float *)&workMat2[196]))
                                           + (float)(*(float *)&workMat2[92] * *(float *)&workMat2[212]);
      *(float *)(LODWORD(axis[2][1]) + 36) = (float)((float)(*(float *)&workMat2[84] * *(float *)&workMat2[184])
                                                   + (float)(*(float *)&workMat2[88] * *(float *)&workMat2[200]))
                                           + (float)(*(float *)&workMat2[92] * *(float *)&workMat2[216]);
      *(float *)(LODWORD(axis[2][1]) + 40) = (float)((float)(*(float *)&workMat2[84] * *(float *)&workMat2[188])
                                                   + (float)(*(float *)&workMat2[88] * *(float *)&workMat2[204]))
                                           + (float)(*(float *)&workMat2[92] * *(float *)&workMat2[220]);
      *(unsigned int *)(LODWORD(axis[2][1]) + 44) = 0;
    }
    else
    {
      ikCalcBoneModelMatrix(
        ikState,
        IKBoneParents[boneName],
        (*ikState->matArrayPostIK)[0],
        (float (*)[4])&workMat2[116]);
      if ( HIBYTE(axis[2][2]) )
      {
        v12 = *(_QWORD *)(LODWORD(axis[2][1]) + 8);
        v13 = *(_QWORD *)(LODWORD(axis[2][1]) + 16);
        v14 = *(_QWORD *)(LODWORD(axis[2][1]) + 24);
        v15 = *(_QWORD *)(LODWORD(axis[2][1]) + 32);
        v16 = *(_QWORD *)(LODWORD(axis[2][1]) + 40);
        v17 = *(_QWORD *)(LODWORD(axis[2][1]) + 48);
        v18 = *(_QWORD *)(LODWORD(axis[2][1]) + 56);
        *(_QWORD *)&workMat2[52] = *(_QWORD *)LODWORD(axis[2][1]);
        *(_QWORD *)&workMat2[60] = v12;
        *(_QWORD *)&workMat2[68] = v13;
        *(_QWORD *)&workMat2[76] = v14;
        *(_QWORD *)&workMat2[84] = v15;
        *(_QWORD *)&workMat2[92] = v16;
        *(_QWORD *)&workMat2[100] = v17;
        *(_QWORD *)&workMat2[108] = v18;
        *(float *)LODWORD(axis[2][1]) = (float)((float)(*(float *)&workMat2[52] * *(float *)&workMat2[180])
                                              + (float)(*(float *)&workMat2[56] * *(float *)&workMat2[196]))
                                      + (float)(*(float *)&v12 * *(float *)&workMat2[212]);
        *(float *)(LODWORD(axis[2][1]) + 4) = (float)((float)(*(float *)&workMat2[52] * *(float *)&workMat2[184])
                                                    + (float)(*(float *)&workMat2[56] * *(float *)&workMat2[200]))
                                            + (float)(*(float *)&workMat2[60] * *(float *)&workMat2[216]);
        *(float *)(LODWORD(axis[2][1]) + 8) = (float)((float)(*(float *)&workMat2[52] * *(float *)&workMat2[188])
                                                    + (float)(*(float *)&workMat2[56] * *(float *)&workMat2[204]))
                                            + (float)(*(float *)&workMat2[60] * *(float *)&workMat2[220]);
        *(unsigned int *)(LODWORD(axis[2][1]) + 12) = 0;
        *(float *)(LODWORD(axis[2][1]) + 16) = (float)((float)(*(float *)&workMat2[68] * *(float *)&workMat2[180])
                                                     + (float)(*(float *)&workMat2[72] * *(float *)&workMat2[196]))
                                             + (float)(*(float *)&workMat2[76] * *(float *)&workMat2[212]);
        *(float *)(LODWORD(axis[2][1]) + 20) = (float)((float)(*(float *)&workMat2[68] * *(float *)&workMat2[184])
                                                     + (float)(*(float *)&workMat2[72] * *(float *)&workMat2[200]))
                                             + (float)(*(float *)&workMat2[76] * *(float *)&workMat2[216]);
        *(float *)(LODWORD(axis[2][1]) + 24) = (float)((float)(*(float *)&workMat2[68] * *(float *)&workMat2[188])
                                                     + (float)(*(float *)&workMat2[72] * *(float *)&workMat2[204]))
                                             + (float)(*(float *)&workMat2[76] * *(float *)&workMat2[220]);
        *(unsigned int *)(LODWORD(axis[2][1]) + 28) = 0;
        *(float *)(LODWORD(axis[2][1]) + 32) = (float)((float)(*(float *)&workMat2[84] * *(float *)&workMat2[180])
                                                     + (float)(*(float *)&workMat2[88] * *(float *)&workMat2[196]))
                                             + (float)(*(float *)&workMat2[92] * *(float *)&workMat2[212]);
        *(float *)(LODWORD(axis[2][1]) + 36) = (float)((float)(*(float *)&workMat2[84] * *(float *)&workMat2[184])
                                                     + (float)(*(float *)&workMat2[88] * *(float *)&workMat2[200]))
                                             + (float)(*(float *)&workMat2[92] * *(float *)&workMat2[216]);
        *(float *)(LODWORD(axis[2][1]) + 40) = (float)((float)(*(float *)&workMat2[84] * *(float *)&workMat2[188])
                                                     + (float)(*(float *)&workMat2[88] * *(float *)&workMat2[204]))
                                             + (float)(*(float *)&workMat2[92] * *(float *)&workMat2[220]);
        *(unsigned int *)(LODWORD(axis[2][1]) + 44) = 0;
      }
      else
      {
        ikCalcBoneModelMatrix(ikState, boneName, (*ikState->matArrayPostIK)[0], (float (*)[4])&workMat2[52]);
        v31 = (float)((float)(*(float *)&workMat2[52] * *(float *)&workMat2[180])
                    + (float)(*(float *)&workMat2[56] * *(float *)&workMat2[196]))
            + (float)(*(float *)&workMat2[60] * *(float *)&workMat2[212]);
        v32 = (float)((float)(*(float *)&workMat2[52] * *(float *)&workMat2[184])
                    + (float)(*(float *)&workMat2[56] * *(float *)&workMat2[200]))
            + (float)(*(float *)&workMat2[60] * *(float *)&workMat2[216]);
        v33 = (float)((float)(*(float *)&workMat2[52] * *(float *)&workMat2[188])
                    + (float)(*(float *)&workMat2[56] * *(float *)&workMat2[204]))
            + (float)(*(float *)&workMat2[60] * *(float *)&workMat2[220]);
        *(unsigned int *)workMat2 = 0;
        *(float *)&workMat2[4] = (float)((float)(*(float *)&workMat2[68] * *(float *)&workMat2[180])
                                       + (float)(*(float *)&workMat2[72] * *(float *)&workMat2[196]))
                               + (float)(*(float *)&workMat2[76] * *(float *)&workMat2[212]);
        *(float *)&workMat2[8] = (float)((float)(*(float *)&workMat2[68] * *(float *)&workMat2[184])
                                       + (float)(*(float *)&workMat2[72] * *(float *)&workMat2[200]))
                               + (float)(*(float *)&workMat2[76] * *(float *)&workMat2[216]);
        *(float *)&workMat2[12] = (float)((float)(*(float *)&workMat2[68] * *(float *)&workMat2[188])
                                        + (float)(*(float *)&workMat2[72] * *(float *)&workMat2[204]))
                                + (float)(*(float *)&workMat2[76] * *(float *)&workMat2[220]);
        *(unsigned int *)&workMat2[16] = 0;
        *(float *)&workMat2[20] = (float)((float)(*(float *)&workMat2[84] * *(float *)&workMat2[180])
                                        + (float)(*(float *)&workMat2[88] * *(float *)&workMat2[196]))
                                + (float)(*(float *)&workMat2[92] * *(float *)&workMat2[212]);
        *(float *)&workMat2[24] = (float)((float)(*(float *)&workMat2[84] * *(float *)&workMat2[184])
                                        + (float)(*(float *)&workMat2[88] * *(float *)&workMat2[200]))
                                + (float)(*(float *)&workMat2[92] * *(float *)&workMat2[216]);
        *(float *)&workMat2[28] = (float)((float)(*(float *)&workMat2[84] * *(float *)&workMat2[188])
                                        + (float)(*(float *)&workMat2[88] * *(float *)&workMat2[204]))
                                + (float)(*(float *)&workMat2[92] * *(float *)&workMat2[220]);
        *(unsigned int *)&workMat2[32] = 0;
        v30 = &workMat2[36];
        v29 = &workMat2[100];
        *(float *)&workMat2[36] = *(float *)&workMat2[100];
        *(float *)&workMat2[40] = *(float *)&workMat2[104];
        *(float *)&workMat2[44] = *(float *)&workMat2[108];
        *(float *)&workMat2[48] = *(float *)&workMat2[112];
        if ( &workMat2[116] == (_BYTE *)v20
          && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ik\\ik_math.h", 165, 0, "%s", "in != out") )
        {
          __debugbreak();
        }
        v20[0] = *(float *)&workMat2[116];
        v20[1] = *(float *)&workMat2[132];
        v20[2] = *(float *)&workMat2[148];
        v20[3] = 0.0f;
        v21 = *(float *)&workMat2[120];
        v22 = *(float *)&workMat2[136];
        v23 = *(float *)&workMat2[152];
        v24 = 0;
        v25 = *(float *)&workMat2[124];
        v26 = *(float *)&workMat2[140];
        v27 = *(float *)&workMat2[156];
        v28 = 0;
        *(float *)LODWORD(axis[2][1]) = (float)((float)(v31 * *(float *)&workMat2[116])
                                              + (float)(v32 * *(float *)&workMat2[120]))
                                      + (float)(v33 * *(float *)&workMat2[124]);
        *(float *)(LODWORD(axis[2][1]) + 4) = (float)((float)(v31 * v20[1]) + (float)(v32 * v22)) + (float)(v33 * v26);
        *(float *)(LODWORD(axis[2][1]) + 8) = (float)((float)(v31 * v20[2]) + (float)(v32 * v23)) + (float)(v33 * v27);
        *(unsigned int *)(LODWORD(axis[2][1]) + 12) = 0;
        *(float *)(LODWORD(axis[2][1]) + 16) = (float)((float)(*(float *)&workMat2[4] * v20[0])
                                                     + (float)(*(float *)&workMat2[8] * v21))
                                             + (float)(*(float *)&workMat2[12] * v25);
        *(float *)(LODWORD(axis[2][1]) + 20) = (float)((float)(*(float *)&workMat2[4] * v20[1])
                                                     + (float)(*(float *)&workMat2[8] * v22))
                                             + (float)(*(float *)&workMat2[12] * v26);
        *(float *)(LODWORD(axis[2][1]) + 24) = (float)((float)(*(float *)&workMat2[4] * v20[2])
                                                     + (float)(*(float *)&workMat2[8] * v23))
                                             + (float)(*(float *)&workMat2[12] * v27);
        *(unsigned int *)(LODWORD(axis[2][1]) + 28) = 0;
        *(float *)(LODWORD(axis[2][1]) + 32) = (float)((float)(*(float *)&workMat2[20] * v20[0])
                                                     + (float)(*(float *)&workMat2[24] * v21))
                                             + (float)(*(float *)&workMat2[28] * v25);
        *(float *)(LODWORD(axis[2][1]) + 36) = (float)((float)(*(float *)&workMat2[20] * v20[1])
                                                     + (float)(*(float *)&workMat2[24] * v22))
                                             + (float)(*(float *)&workMat2[28] * v26);
        *(float *)(LODWORD(axis[2][1]) + 40) = (float)((float)(*(float *)&workMat2[20] * v20[2])
                                                     + (float)(*(float *)&workMat2[24] * v23))
                                             + (float)(*(float *)&workMat2[28] * v27);
        *(unsigned int *)(LODWORD(axis[2][1]) + 44) = 0;
        LODWORD(v19[4]) = &workMat2[164];
        LODWORD(v19[3]) = &workMat2[36];
        v19[0] = *(float *)&workMat2[36] - *(float *)&workMat2[164];
        v19[1] = *(float *)&workMat2[40] - *(float *)&workMat2[168];
        v19[2] = *(float *)&workMat2[44] - *(float *)&workMat2[172];
        ikMatrixTransformVector34(v19, (const float (*)[4])v20, (float *)(LODWORD(axis[2][1]) + 48));
        *(float *)(LODWORD(axis[2][1]) + 60) = 1.0f;
      }
    }
    ikState->bHasActiveLayers = 1;
    if ( ikState->cacheActive )
    {
      memset((unsigned __int8 *)ikState->matArrayCache, 0, 0x5C0u);
      ikState->cacheActive = 0;
    }
    if ( boneName != IKBONE_NONE )
      ikState->modifiedIKBones |= 1 << boneName;
  }
}

void __cdecl ikMatrixLerp44(float (*from)[4], float (*to)[4], float lerp, float (*res)[4])
{
  float v4; // [esp+8h] [ebp-98h]
  float v5; // [esp+14h] [ebp-8Ch]
  float v6; // [esp+20h] [ebp-80h]
  float aFrom[3]; // [esp+4Ch] [ebp-54h] BYREF
  float pLerped[3]; // [esp+58h] [ebp-48h] BYREF
  float aLerped[3]; // [esp+64h] [ebp-3Ch] BYREF
  float axis[3][3]; // [esp+70h] [ebp-30h] BYREF
  float aTo[3]; // [esp+94h] [ebp-Ch] BYREF

  *(_QWORD *)&axis[0][0] = *(_QWORD *)&(*from)[0];
  axis[0][2] = (*from)[2];
  axis[1][0] = (*from)[4];
  axis[1][1] = (*from)[5];
  axis[1][2] = (*from)[6];
  axis[2][0] = (*from)[8];
  axis[2][1] = (*from)[9];
  axis[2][2] = (*from)[10];
  AxisToAngles(axis, aFrom);
  *(_QWORD *)&axis[0][0] = *(_QWORD *)&(*to)[0];
  axis[0][2] = (*to)[2];
  axis[1][0] = (*to)[4];
  axis[1][1] = (*to)[5];
  axis[1][2] = (*to)[6];
  axis[2][0] = (*to)[8];
  axis[2][1] = (*to)[9];
  axis[2][2] = (*to)[10];
  AxisToAngles(axis, aTo);
  v6 = aFrom[0];
  aLerped[0] = AngleNormalize180(aTo[0] - aFrom[0]) * lerp + v6;
  v5 = aFrom[1];
  aLerped[1] = AngleNormalize180(aTo[1] - aFrom[1]) * lerp + v5;
  v4 = aFrom[2];
  aLerped[2] = AngleNormalize180(aTo[2] - aFrom[2]) * lerp + v4;
  AnglesToAxis(aLerped, axis);
  Vec3Lerp(&(*from)[12], &(*to)[12], lerp, pLerped);
  ikMatrixSet44(res, pLerped, axis, 1.0);
}

