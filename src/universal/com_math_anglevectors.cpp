#include "com_math_anglevectors.h"

void __cdecl AngleVectors(const float *angles, float *forward, float *right, float *up)
{
  float cy; // [esp+18h] [ebp-1Ch]
  float angle; // [esp+1Ch] [ebp-18h]
  float anglea; // [esp+1Ch] [ebp-18h]
  float angleb; // [esp+1Ch] [ebp-18h]
  float sr; // [esp+20h] [ebp-14h]
  float v9; // [esp+24h] [ebp-10h]
  float cr; // [esp+28h] [ebp-Ch]
  float cp; // [esp+2Ch] [ebp-8h]
  float sy; // [esp+30h] [ebp-4h]

  angle = angles[1] * 0.017453292;
  cy = cos(angle);
  sy = sin(angle);
  anglea = *angles * 0.017453292;
  cp = cos(anglea);
  v9 = sin(anglea);
  if ( forward )
  {
    *forward = cp * cy;
    forward[1] = cp * sy;
    *((unsigned int *)forward + 2) = LODWORD(v9) ^ _mask__NegFloat_;
  }
  if ( right || up )
  {
    angleb = angles[2] * 0.017453292;
    cr = cos(angleb);
    sr = sin(angleb);
    if ( right )
    {
      *right = (float)((float)((float)(-1.0 * sr) * v9) * cy)
             + (float)((float)(-1.0 * cr) * COERCE_FLOAT(LODWORD(sy) ^ _mask__NegFloat_));
      right[1] = (float)((float)((float)(-1.0 * sr) * v9) * sy) + (float)((float)(-1.0 * cr) * cy);
      right[2] = (float)(-1.0 * sr) * cp;
    }
    if ( up )
    {
      *up = (float)((float)(cr * v9) * cy)
          + (float)(COERCE_FLOAT(LODWORD(sr) ^ _mask__NegFloat_) * COERCE_FLOAT(LODWORD(sy) ^ _mask__NegFloat_));
      up[1] = (float)((float)(cr * v9) * sy) + (float)(COERCE_FLOAT(LODWORD(sr) ^ _mask__NegFloat_) * cy);
      up[2] = cr * cp;
    }
  }
}

void __cdecl AnglesToAxis(const float *angles, float (*axis)[3])
{
  float cy; // [esp+18h] [ebp-1Ch]
  float angle; // [esp+1Ch] [ebp-18h]
  float anglea; // [esp+1Ch] [ebp-18h]
  float angleb; // [esp+1Ch] [ebp-18h]
  float sr; // [esp+20h] [ebp-14h]
  float v7; // [esp+24h] [ebp-10h]
  float cr; // [esp+28h] [ebp-Ch]
  float cp; // [esp+2Ch] [ebp-8h]
  float sy; // [esp+30h] [ebp-4h]

  angle = angles[1] * 0.017453292;
  cy = cos(angle);
  sy = sin(angle);
  anglea = *angles * 0.017453292;
  cp = cos(anglea);
  v7 = sin(anglea);
  (*axis)[0] = cp * cy;
  (*axis)[1] = cp * sy;
  LODWORD((*axis)[2]) = LODWORD(v7) ^ _mask__NegFloat_;
  angleb = angles[2] * 0.017453292;
  cr = cos(angleb);
  sr = sin(angleb);
  (*axis)[3] = (float)((float)(sr * v7) * cy) + (float)(COERCE_FLOAT(LODWORD(sy) ^ _mask__NegFloat_) * cr);
  (*axis)[4] = (float)((float)(sr * v7) * sy) + (float)(cr * cy);
  (*axis)[5] = sr * cp;
  (*axis)[6] = (float)((float)(cr * v7) * cy)
             + (float)(COERCE_FLOAT(LODWORD(sr) ^ _mask__NegFloat_) * COERCE_FLOAT(LODWORD(sy) ^ _mask__NegFloat_));
  (*axis)[7] = (float)((float)(cr * v7) * sy) + (float)(COERCE_FLOAT(LODWORD(sr) ^ _mask__NegFloat_) * cy);
  (*axis)[8] = cr * cp;
}

void __cdecl AnglesToQuat(const float *angles, float *quat)
{
  float axis[3][3]; // [esp+0h] [ebp-24h] BYREF

  AnglesToAxis(angles, axis);
  AxisToQuat(axis, quat);
}

void __cdecl QuatToAxis(const float *quat, float (*axis)[3])
{
  float yy; // [esp+0h] [ebp-38h]
  float yya; // [esp+0h] [ebp-38h]
  float xy; // [esp+8h] [ebp-30h]
  float zz; // [esp+Ch] [ebp-2Ch]
  float zza; // [esp+Ch] [ebp-2Ch]
  float zw; // [esp+10h] [ebp-28h]
  float scaledX; // [esp+14h] [ebp-24h]
  float scaledY; // [esp+18h] [ebp-20h]
  float yw; // [esp+1Ch] [ebp-1Ch]
  float xz; // [esp+20h] [ebp-18h]
  float yz; // [esp+24h] [ebp-14h]
  float xx; // [esp+28h] [ebp-10h]
  float xxa; // [esp+28h] [ebp-10h]
  float xw; // [esp+2Ch] [ebp-Ch]
  float magSqr; // [esp+34h] [ebp-4h]

  xx = *quat * *quat;
  yy = quat[1] * quat[1];
  zz = quat[2] * quat[2];
  magSqr = (float)((float)(xx + yy) + zz) + (float)(quat[3] * quat[3]);
  if ( magSqr <= 0.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math_anglevectors.cpp",
          139,
          0,
          "%s",
          "magSqr > 0.0f") )
  {
    __debugbreak();
  }
  xxa = xx * (float)(2.0 / magSqr);
  yya = yy * (float)(2.0 / magSqr);
  zza = zz * (float)(2.0 / magSqr);
  scaledX = (float)(2.0 / magSqr) * *quat;
  xy = scaledX * quat[1];
  xz = scaledX * quat[2];
  xw = scaledX * quat[3];
  scaledY = (float)(2.0 / magSqr) * quat[1];
  yz = scaledY * quat[2];
  yw = scaledY * quat[3];
  zw = (float)((float)(2.0 / magSqr) * quat[2]) * quat[3];
  (*axis)[0] = 1.0 - (float)(yya + zza);
  (*axis)[1] = xy + zw;
  (*axis)[2] = xz - yw;
  (*axis)[3] = xy - zw;
  (*axis)[4] = 1.0 - (float)(xxa + zza);
  (*axis)[5] = yz + xw;
  (*axis)[6] = xz + yw;
  (*axis)[7] = yz - xw;
  (*axis)[8] = 1.0 - (float)(xxa + yya);
}

void __cdecl AxisToSignedAngles(const float (*axis)[3], float *angles)
{
  float v2; // [esp+0h] [ebp-38h]
  float rad; // [esp+18h] [ebp-20h]
  float rada; // [esp+18h] [ebp-20h]
  float right[3]; // [esp+1Ch] [ebp-1Ch] BYREF
  float temp; // [esp+28h] [ebp-10h]
  float pitch; // [esp+2Ch] [ebp-Ch]
  float fCos; // [esp+30h] [ebp-8h]
  float fSin; // [esp+34h] [ebp-4h]

  vectosignedangles((const float *)axis, angles);
  right[0] = (*axis)[3];
  right[1] = (*axis)[4];
  right[2] = (*axis)[5];
  rad = COERCE_FLOAT(*((unsigned int *)angles + 1) ^ _mask__NegFloat_) * 0.017453292;
  fCos = cos(rad);
  fSin = sin(rad);
  temp = (float)(fCos * right[0]) - (float)(fSin * right[1]);
  right[1] = (float)(fSin * right[0]) + (float)(fCos * right[1]);
  rada = COERCE_FLOAT(*(unsigned int *)angles ^ _mask__NegFloat_) * 0.017453292;
  fCos = cos(rada);
  fSin = sin(rada);
  right[0] = (float)(fSin * right[2]) + (float)(fCos * temp);
  right[2] = (float)(fCos * right[2]) - (float)(fSin * temp);
  pitch = vectosignedpitch(right);
  if ( right[1] >= 0.0 )
  {
    *((unsigned int *)angles + 2) = LODWORD(pitch) ^ _mask__NegFloat_;
  }
  else
  {
    if ( pitch >= 0.0 )
      v2 = FLOAT_N180_0;
    else
      v2 = FLOAT_180_0;
    angles[2] = pitch + v2;
  }
}

void __cdecl vectosignedangles(const float *vec, float *angles)
{
  double v2; // xmm0_8
  double v3; // xmm0_8
  long double v4; // [esp-10h] [ebp-2Ch]
  long double v5; // [esp-10h] [ebp-2Ch]
  long double v6; // [esp-8h] [ebp-24h]
  long double v7; // [esp-8h] [ebp-24h]
  float v8; // [esp+4h] [ebp-18h]
  float v9; // [esp+8h] [ebp-14h]
  float v10; // [esp+Ch] [ebp-10h]

  if ( vec[1] == 0.0 && *vec == 0.0 )
  {
    v10 = *(float *)&FLOAT_0_0;
    if ( COERCE_FLOAT(*((unsigned int *)vec + 2) ^ _mask__NegFloat_) < 0.0 )
      v9 = FLOAT_N90_0;
    else
      v9 = FLOAT_90_0;
    v8 = v9;
  }
  else
  {
    *((float *)&v6 + 1) = *vec;
    v2 = vec[1];
    __libm_sse2_atan2(v4, v6);
    *(float *)&v2 = v2;
    v10 = (float)(*(float *)&v2 * 180.0) / 3.1415927;
    *(float *)&v7 = (float)(*vec * *vec) + (float)(vec[1] * vec[1]);
    HIDWORD(v5) = fsqrt(*(float *)&v7);
    *(float *)&v5 = vec[2];
    v3 = *(float *)&v5;
    __libm_sse2_atan2(v5, v7);
    *(float *)&v3 = v3;
    v8 = (float)(*(float *)&v3 * -180.0) / 3.1415927;
  }
  *angles = v8;
  angles[1] = v10;
  angles[2] = *(float *)&FLOAT_0_0;
}

double __cdecl vectosignedpitch(const float *vec)
{
  double v1; // xmm0_8
  long double v3; // [esp-1Ch] [ebp-2Ch]
  long double v4; // [esp-14h] [ebp-24h]

  if ( vec[1] == 0.0 && *vec == 0.0 )
  {
    if ( COERCE_FLOAT(*((unsigned int *)vec + 2) ^ _mask__NegFloat_) < 0.0 )
      return FLOAT_N90_0;
    else
      return FLOAT_90_0;
  }
  else
  {
    *((float *)&v4 + 1) = vec[2];
    v1 = *((float *)&v4 + 1);
    __libm_sse2_atan2(v3, v4);
    *(float *)&v1 = v1;
    return (float)((float)(*(float *)&v1 * -180.0) / 3.1415927);
  }
}

