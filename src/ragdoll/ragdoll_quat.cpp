#include "ragdoll_quat.h"

void __cdecl Ragdoll_QuatMul(const float *qa, const float *qb, float *dest)
{
  float w0; // [esp+0h] [ebp-8h]
  float w1; // [esp+4h] [ebp-4h]

  if ( (qa == dest || qb == dest)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_quat.cpp",
          21,
          0,
          "%s",
          "qa != dest && qb != dest") )
  {
    __debugbreak();
  }
  w0 = qa[3];
  w1 = qb[3];
  *dest = (float)((float)((float)(w0 * *qb) + (float)(w1 * *qa)) + (float)(qa[1] * qb[2])) - (float)(qa[2] * qb[1]);
  dest[1] = (float)((float)((float)(w0 * qb[1]) + (float)(w1 * qa[1])) + (float)(qa[2] * *qb)) - (float)(*qa * qb[2]);
  dest[2] = (float)((float)((float)(w0 * qb[2]) + (float)(w1 * qa[2])) + (float)(*qa * qb[1])) - (float)(qa[1] * *qb);
  dest[3] = (float)((float)((float)(w0 * w1) - (float)(*qa * *qb)) - (float)(qa[1] * qb[1])) - (float)(qa[2] * qb[2]);
}

void __cdecl Ragdoll_QuatMulInvSecond(const float *qa, const float *qb, float *dest)
{
  float qinv[4]; // [esp+0h] [ebp-10h] BYREF

  Ragdoll_QuatConjugate(qb, qinv);
  Ragdoll_QuatMul(qa, qinv, dest);
}

void __cdecl Ragdoll_QuatConjugate(const float *src, float *dest)
{
  *(unsigned int *)dest = *(unsigned int *)src ^ _mask__NegFloat_;
  *((unsigned int *)dest + 1) = *((unsigned int *)src + 1) ^ _mask__NegFloat_;
  *((unsigned int *)dest + 2) = *((unsigned int *)src + 2) ^ _mask__NegFloat_;
  dest[3] = src[3];
}

void __cdecl Ragdoll_QuatInverse(const float *src, float *dest)
{
  const char *v2; // eax

  if ( !Vec4IsNormalized(src) )
  {
    v2 = va("%g %g %g %g", *src, src[1], src[2], src[3]);
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_quat.cpp",
            91,
            0,
            "%s\n\t%s",
            "Vec4IsNormalized( src )",
            v2) )
      __debugbreak();
  }
  *(unsigned int *)dest = *(unsigned int *)src ^ _mask__NegFloat_;
  *((unsigned int *)dest + 1) = *((unsigned int *)src + 1) ^ _mask__NegFloat_;
  *((unsigned int *)dest + 2) = *((unsigned int *)src + 2) ^ _mask__NegFloat_;
  dest[3] = src[3];
}

void __cdecl Ragdoll_QuatPointRotate(const float *p, const float *q, float *dest)
{
  float tmp0[4]; // [esp+0h] [ebp-40h] BYREF
  float qp[4]; // [esp+10h] [ebp-30h] BYREF
  float tmp1[4]; // [esp+20h] [ebp-20h] BYREF
  float qInv[4]; // [esp+30h] [ebp-10h] BYREF

  qp[0] = *p;
  qp[1] = p[1];
  qp[2] = p[2];
  qp[3] = *(float *)&FLOAT_0_0;
  Ragdoll_QuatInverse(q, qInv);
  Ragdoll_QuatMul(q, qInv, tmp0);
  Ragdoll_QuatMul(tmp0, qp, tmp1);
  *dest = tmp1[0];
  dest[1] = tmp1[1];
  dest[2] = tmp1[2];
}

void __cdecl Ragdoll_Mat33ToQuat(const float (*axis)[3], float *quat)
{
  float v2; // xmm0_4
  float trace; // [esp+Ch] [ebp-8h]
  float s; // [esp+10h] [ebp-4h]
  float sa; // [esp+10h] [ebp-4h]
  float sb; // [esp+10h] [ebp-4h]
  float sc; // [esp+10h] [ebp-4h]

  trace = (float)((float)((*axis)[0] + (float)(*axis)[4]) + (float)(*axis)[8]) + 1.0;
  if ( trace <= 0.000001 )
  {
    if ( (*axis)[0] <= (*axis)[4] || (*axis)[0] <= (*axis)[8] )
    {
      if ( (*axis)[4] <= (*axis)[8] )
      {
        sc = fsqrt((float)((float)((float)(*axis)[8] + 1.0) - (*axis)[0]) - (float)(*axis)[4]) * 2.0;
        *quat = (float)((float)(*axis)[2] + (float)(*axis)[6]) / sc;
        quat[1] = (float)((float)(*axis)[5] + (float)(*axis)[7]) / sc;
        quat[2] = 0.25 * sc;
        v2 = (float)((float)(*axis)[1] - (float)(*axis)[3]) / sc;
      }
      else
      {
        sb = fsqrt((float)((float)((float)(*axis)[4] + 1.0) - (*axis)[0]) - (float)(*axis)[8]) * 2.0;
        *quat = (float)((float)(*axis)[1] + (float)(*axis)[3]) / sb;
        quat[1] = 0.25 * sb;
        quat[2] = (float)((float)(*axis)[5] + (float)(*axis)[7]) / sb;
        v2 = (float)((float)(*axis)[2] - (float)(*axis)[6]) / sb;
      }
      quat[3] = v2;
    }
    else
    {
      sa = fsqrt((float)((float)((*axis)[0] + 1.0) - (float)(*axis)[4]) - (float)(*axis)[8]) * 2.0;
      *quat = 0.25 * sa;
      quat[1] = (float)((float)(*axis)[1] + (float)(*axis)[3]) / sa;
      quat[2] = (float)((float)(*axis)[2] + (float)(*axis)[6]) / sa;
      quat[3] = (float)((float)(*axis)[5] - (float)(*axis)[7]) / sa;
    }
  }
  else
  {
    s = 0.5 / fsqrt(trace);
    *quat = (float)((float)(*axis)[7] - (float)(*axis)[5]) * s;
    quat[1] = (float)((float)(*axis)[2] - (float)(*axis)[6]) * s;
    quat[2] = (float)((float)(*axis)[3] - (float)(*axis)[1]) * s;
    quat[3] = 0.25 / s;
  }
}

void __cdecl Ragdoll_QuatToAxisAngle(const float *quat, float *axisAngle)
{
  long double v2; // [esp+4h] [ebp-1Ch]
  float v3; // [esp+4h] [ebp-1Ch]
  float v4; // [esp+Ch] [ebp-14h]

  v4 = Q_acos(quat[3]);
  __libm_sse2_sin(v2);
  if ( COERCE_FLOAT(LODWORD(v4) & _mask__AbsFloat_) <= 0.000001 )
  {
    *axisAngle = *(float *)&FLOAT_0_0;
    axisAngle[1] = *(float *)&FLOAT_0_0;
    axisAngle[2] = *(float *)&FLOAT_0_0;
  }
  else
  {
    v3 = (float)(2.0 * v4) / v4;
    *axisAngle = *quat * v3;
    axisAngle[1] = quat[1] * v3;
    axisAngle[2] = quat[2] * v3;
  }
}

void __cdecl Ragdoll_QuatLerp(const float *qa, const float *qb, float t, float *dest)
{
  Vec4Lerp(qa, qb, t, dest);
  Vec4Normalize(dest);
}

