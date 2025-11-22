#include "com_math.h"

#include <stdlib.h>
#include "assertive.h"
#include <cmath>

double __cdecl random()
{
  return (double)rand() / 32768.0;
}

double __cdecl crandom()
{
  return random() * 2.0 - 1.0;
}

void __cdecl GaussianRandom(float *f0, float *f1)
{
    float v2; // [esp+0h] [ebp-1Ch]
    float v3; // [esp+4h] [ebp-18h]
    float v4; // [esp+8h] [ebp-14h]
    float x; // [esp+10h] [ebp-Ch]
    float y; // [esp+14h] [ebp-8h]
    float w; // [esp+18h] [ebp-4h]

    iassert(f0);
    iassert(f1);

    do
    {
        x = crandom();
        y = crandom();
        w = x * x + y * y;
    } while (w > 1.0);
    v4 = log(w);
    v3 = v4 * -2.0 / w;
    v2 = sqrt(v3);
    *f0 = x * v2;
    *f1 = y * v2;
}

unsigned int __cdecl RandWithSeed(int *seed)
{
  *seed = 1103515245 * *seed + 12345;
  return *seed / 0x10000 % 0x8000u;
}

double __cdecl randomhash(int value)
{
  return (double)(int)RandWithSeed(&value) / 32768.0;
}

void __cdecl PointInCircleFromUniformDeviates(float radiusDeviate, float yawDeviate, float *point)
{
  long double v3; // st7
  float r; // [esp+8h] [ebp-14h]
  float sinYaw; // [esp+14h] [ebp-8h]
  float cosYaw; // [esp+18h] [ebp-4h]

  r = fsqrt(radiusDeviate);
  v3 = (float)(6.2831855 * yawDeviate);
  cosYaw = cos(v3);
  sinYaw = sin(v3);
  *point = r * cosYaw;
  point[1] = r * sinYaw;
}

double __cdecl LinearTrack(float tgt, float cur, float rate, float deltaTime)
{
  float err; // [esp+4h] [ebp-8h]
  float step; // [esp+8h] [ebp-4h]

  err = tgt - cur;
  if ( (float)(tgt - cur) <= 0.0 )
    step = COERCE_FLOAT(LODWORD(rate) ^ _mask__NegFloat_) * deltaTime;
  else
    step = rate * deltaTime;
  if ( COERCE_FLOAT(LODWORD(err) & _mask__AbsFloat_) <= 0.001 )
    return tgt;
  if ( COERCE_FLOAT(LODWORD(step) & _mask__AbsFloat_) <= COERCE_FLOAT(LODWORD(err) & _mask__AbsFloat_) )
    return cur + step;
  return tgt;
}

double __cdecl LinearTrackAngle(float tgt, float cur, float rate, float deltaTime)
{
  float angle; // [esp+14h] [ebp-4h]

  while ( (float)(tgt - cur) > 180.0 )
    tgt = tgt - 360.0;
  while ( (float)(tgt - cur) < -180.0 )
    tgt = tgt + 360.0;
  angle = LinearTrack(tgt, cur, rate, deltaTime);
  return AngleNormalize180(angle);
}

double __cdecl DiffTrack(float tgt, float cur, float rate, float deltaTime)
{
  float step; // [esp+8h] [ebp-4h]

  step = (float)(rate * (float)(tgt - cur)) * deltaTime;
  if ( COERCE_FLOAT(COERCE_UNSIGNED_INT(tgt - cur) & _mask__AbsFloat_) <= 0.001 )
    return tgt;
  if ( COERCE_FLOAT(LODWORD(step) & _mask__AbsFloat_) <= COERCE_FLOAT(COERCE_UNSIGNED_INT(tgt - cur) & _mask__AbsFloat_) )
    return cur + step;
  return tgt;
}

double __cdecl DiffTrackAngle(float tgt, float cur, float rate, float deltaTime)
{
  float angle; // [esp+14h] [ebp-4h]

  while ( (float)(tgt - cur) > 180.0 )
    tgt = tgt - 360.0;
  while ( (float)(tgt - cur) < -180.0 )
    tgt = tgt + 360.0;
  angle = DiffTrack(tgt, cur, rate, deltaTime);
  return AngleNormalize180(angle);
}

double __cdecl GraphGetValueFromFraction(int knotCount, const float (*knots)[2], float fraction)
{
  float result; // [esp+Ch] [ebp-Ch]
  float adjustedFrac; // [esp+10h] [ebp-8h]
  int knotIndex; // [esp+14h] [ebp-4h]

  result = -1.0f;
  if ( !knots && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 467, 0, "%s", "knots") )
    __debugbreak();
  if ( knotCount < 2
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
          468,
          0,
          "%s\n\t(knotCount) = %i",
          "(knotCount >= 2)",
          knotCount) )
  {
    __debugbreak();
  }
  if ( (fraction < 0.0 || fraction > 1.0)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
          469,
          0,
          "%s\n\t(fraction) = %g",
          "(fraction >= 0.0f && fraction <= 1.0f)",
          fraction) )
  {
    __debugbreak();
  }
  if ( (*knots)[2 * knotCount - 2] != 1.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
          470,
          0,
          "%s\n\t(knots[knotCount - 1][0]) = %g",
          "(knots[knotCount - 1][0] == 1.0f)",
          (*knots)[2 * knotCount - 2]) )
  {
    __debugbreak();
  }
  for ( knotIndex = 1; knotIndex < knotCount; ++knotIndex )
  {
    if ( (*knots)[2 * knotIndex] >= fraction )
    {
      adjustedFrac = (float)(fraction - (float)(*knots)[2 * knotIndex - 2])
                   / (float)((float)(*knots)[2 * knotIndex] - (float)(*knots)[2 * knotIndex - 2]);
      if ( (adjustedFrac < 0.0 || adjustedFrac > 1.0)
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
              478,
              0,
              "%s\n\t(adjustedFrac) = %g",
              "(adjustedFrac >= 0.0f && adjustedFrac <= 1.0f)",
              adjustedFrac) )
      {
        __debugbreak();
      }
      if ( ((*knots)[2 * knotIndex - 1] < 0.0 || (*knots)[2 * knotIndex - 1] > 1.0)
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
              479,
              0,
              "%s\n\t(knots[knotIndex - 1][1]) = %g",
              "(knots[knotIndex - 1][1] >= 0.0f && knots[knotIndex - 1][1] <= 1.0f)",
              (*knots)[2 * knotIndex - 1]) )
      {
        __debugbreak();
      }
      if ( ((*knots)[2 * knotIndex + 1] < 0.0 || (*knots)[2 * knotIndex + 1] > 1.0)
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
              480,
              0,
              "%s\n\t(knots[knotIndex][1]) = %g",
              "(knots[knotIndex][1] >= 0.0f && knots[knotIndex][1] <= 1.0f)",
              (*knots)[2 * knotIndex + 1]) )
      {
        __debugbreak();
      }
      result = (float)((float)((float)(*knots)[2 * knotIndex + 1] - (float)(*knots)[2 * knotIndex - 1]) * adjustedFrac)
             + (float)(*knots)[2 * knotIndex - 1];
      break;
    }
  }
  if ( (result < 0.0 || result > 1.0)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
          487,
          0,
          "%s\n\t(result) = %g",
          "(result >= 0.0f && result <= 1.0f)",
          result) )
  {
    __debugbreak();
  }
  return result;
}

double __cdecl Q_acos(float c)
{
  long double angle; // [esp+0h] [ebp-4h]

  __libm_sse2_acos(angle);
  if ( c > 3.1415927 )
    return 3.1415927;
  if ( c >= -3.1415927 )
    return c;
  return 3.1415927;
}

char __cdecl ClampChar(int i)
{
  if ( i < -128 )
    return 0x80;
  if ( i <= 127 )
    return i;
  return 127;
}

__int16 __cdecl ClampShort(int i)
{
  if ( i < -32768 )
    return 0x8000;
  if ( i <= 0x7FFF )
    return i;
  return 0x7FFF;
}

unsigned __int8 __cdecl DirToByte(const float *dir)
{
  unsigned __int8 best; // [esp+Ah] [ebp-6h]
  unsigned __int8 i; // [esp+Bh] [ebp-5h]
  float bestd; // [esp+Ch] [ebp-4h]

  if ( !dir )
    return 0;
  bestd = 0.0f;
  best = 0;
  for ( i = 0; i < 0xA2u; ++i )
  {
    if ( (float)((float)((float)(*dir * bytedirs[i][0]) + (float)(dir[1] * bytedirs[i][1]))
               + (float)(dir[2] * bytedirs[i][2])) > bestd )
    {
      bestd = (float)((float)(*dir * bytedirs[i][0]) + (float)(dir[1] * bytedirs[i][1]))
            + (float)(dir[2] * bytedirs[i][2]);
      best = i;
    }
  }
  return best;
}

void __cdecl ByteToDir(unsigned int b, float *dir)
{
  float *v2; // ecx

  if ( b < 0xA2 )
  {
    *dir = bytedirs[b][0];
    v2 = bytedirs[b];
    dir[1] = v2[1];
    dir[2] = v2[2];
  }
  else
  {
    *dir = vec3_origin[0];
    dir[1] = 0.0;
    dir[2] = 0.0;
  }
}

int __cdecl VecNCompareCustomEpsilon(const float *v0, const float *v1, float epsilon, int coordCount)
{
  int i; // [esp+4h] [ebp-4h]

  for ( i = 0; i < coordCount; ++i )
  {
    if ( (float)((float)(v0[i] - v1[i]) * (float)(v0[i] - v1[i])) > (float)(epsilon * epsilon) )
      return 0;
  }
  return 1;
}

void __cdecl Vec3ProjectionCoords(const float *dir, int *xCoord, int *yCoord)
{
  float dirSq; // [esp+0h] [ebp-Ch]
  float dirSq_4; // [esp+4h] [ebp-8h]
  float dirSq_8; // [esp+8h] [ebp-4h]

  dirSq = *dir * *dir;
  dirSq_4 = dir[1] * dir[1];
  dirSq_8 = dir[2] * dir[2];
  if ( dirSq_8 < dirSq || dirSq_8 < dirSq_4 )
  {
    if ( dirSq_4 < dirSq || dirSq_4 < dirSq_8 )
    {
      if ( *dir <= 0.0 )
      {
        *xCoord = 2;
        *yCoord = 1;
      }
      else
      {
        *xCoord = 1;
        *yCoord = 2;
      }
    }
    else if ( dir[1] <= 0.0 )
    {
      *xCoord = 0;
      *yCoord = 2;
    }
    else
    {
      *xCoord = 2;
      *yCoord = 0;
    }
  }
  else if ( dir[2] <= 0.0 )
  {
    *xCoord = 1;
    *yCoord = 0;
  }
  else
  {
    *xCoord = 0;
    *yCoord = 1;
  }
}

void __cdecl RotatePointAroundVector(float *dst, const float *dir, const float *point, float degrees)
{
  float m[3][3]; // [esp+1Ch] [ebp-E0h] BYREF
  float rad; // [esp+40h] [ebp-BCh]
  float vr[3]; // [esp+44h] [ebp-B8h] BYREF
  float zrot[3][3]; // [esp+50h] [ebp-ACh] BYREF
  float rot[3][3]; // [esp+74h] [ebp-88h] BYREF
  float vf[3]; // [esp+98h] [ebp-64h] BYREF
  float vup[3]; // [esp+A4h] [ebp-58h] BYREF
  int i; // [esp+B0h] [ebp-4Ch]
  float tmpmat[3][3]; // [esp+B4h] [ebp-48h] BYREF
  float im[3][3]; // [esp+D8h] [ebp-24h] BYREF

  if ( *dir == 0.0
    && dir[1] == 0.0
    && dir[2] == 0.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
          733,
          0,
          "%s",
          "dir[0] || dir[1] || dir[2]") )
  {
    __debugbreak();
  }
  *(_QWORD *)vf = *(_QWORD *)dir;
  vf[2] = dir[2];
  PerpendicularVector(dir, vr);
  Vec3Cross(vr, vf, vup);
  *(_QWORD *)&m[0][0] = __PAIR64__(LODWORD(vup[0]), LODWORD(vr[0]));
  *(_QWORD *)&m[1][0] = __PAIR64__(LODWORD(vup[1]), LODWORD(vr[1]));
  *(_QWORD *)&m[2][0] = __PAIR64__(LODWORD(vup[2]), LODWORD(vr[2]));
  m[1][2] = vf[1];
  m[0][2] = vf[0];
  m[2][2] = vf[2];
  memcpy(im, m, sizeof(im));
  *(_QWORD *)&im[0][1] = *(_QWORD *)&vr[1];
  im[1][0] = vup[0];
  im[1][2] = vup[2];
  *(_QWORD *)&im[2][0] = *(_QWORD *)vf;
  zrot[0][2] = 0.0;
  zrot[1][0] = 0.0;
  *(_QWORD *)&zrot[2][0] = 0;
  zrot[2][2] = 1.0f;
  *(_QWORD *)&zrot[1][1] = LODWORD(1.0f);
  *(_QWORD *)&zrot[0][0] = LODWORD(1.0f);
  rad = degrees * 0.017453292;
  if ( (COERCE_UNSIGNED_INT(degrees * 0.017453292) & 0x7F800000) == 0x7F800000
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 768, 0, "%s", "!IS_NAN(rad)") )
  {
    __debugbreak();
  }
  zrot[0][0] = cos(rad);
  zrot[0][1] = sin(rad);
  if ( (LODWORD(zrot[0][1]) & 0x7F800000) == 0x7F800000
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
          772,
          0,
          "%s",
          "!IS_NAN(zrot[0][1])") )
  {
    __debugbreak();
  }
  if ( (LODWORD(zrot[0][0]) & 0x7F800000) == 0x7F800000
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
          773,
          0,
          "%s",
          "!IS_NAN(zrot[0][0])") )
  {
    __debugbreak();
  }
  LODWORD(zrot[1][0]) = LODWORD(zrot[0][1]) ^ _mask__NegFloat_;
  zrot[1][1] = zrot[0][0];
  MatrixMultiply(m, zrot, tmpmat);
  MatrixMultiply(tmpmat, im, rot);
  for ( i = 0; i < 3; ++i )
    dst[i] = (float)((float)(rot[i][0] * *point) + (float)(rot[i][1] * point[1])) + (float)(rot[i][2] * point[2]);
}

void __cdecl Vec3Basis_RightHanded(const float *forward, float *left, float *up)
{
  PerpendicularVector(forward, up);
  Vec3Cross(up, forward, left);
}

double __cdecl vectoyaw(const float *vec)
{
  double v1; // xmm0_8
  long double v3; // [esp+0h] [ebp-10h]
  float v4; // [esp+0h] [ebp-10h]
  long double v5; // [esp+8h] [ebp-8h]
  float yawa; // [esp+Ch] [ebp-4h]

  if ( vec[1] == 0.0 && *vec == 0.0 )
  {
    return 0.0f;
  }
  else
  {
    *((float *)&v3 + 1) = vec[1];
    *(float *)&v5 = *vec;
    v1 = *((float *)&v3 + 1);
    __libm_sse2_atan2(v3, v5);
    *(float *)&v1 = v1;
    yawa = (float)(*(float *)&v1 * 180.0) / 3.1415927;
    if ( yawa < 0.0 )
      v4 = 360.0f;
    else
      v4 = 0.0f;
    return (float)(yawa + v4);
  }
}

double __cdecl vectosignedyaw(const float *vec)
{
  double v1; // xmm0_8
  long double v3; // [esp+0h] [ebp-Ch]
  long double yaw; // [esp+8h] [ebp-4h]
  float yawa; // [esp+8h] [ebp-4h]

  if ( vec[1] == 0.0 && *vec == 0.0 )
  {
    return 0.0f;
  }
  else
  {
    *(float *)&v3 = vec[1];
    *((float *)&v3 + 1) = *vec;
    v1 = *(float *)&v3;
    __libm_sse2_atan2(v3, yaw);
    *(float *)&v1 = v1;
    yawa = (float)(*(float *)&v1 * 180.0) / 3.1415927;
    if ( yawa < -180.0
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 859, 0, "%s", "yaw >= -180") )
    {
      __debugbreak();
    }
    if ( yawa > 180.0
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 860, 0, "%s", "yaw <= 180") )
    {
      __debugbreak();
    }
  }
  return yawa;
}

double __cdecl vectopitch(const float *vec)
{
  double v1; // xmm0_8
  long double v3; // [esp-1Ch] [ebp-2Ch]
  long double v4; // [esp-14h] [ebp-24h]
  float v5; // [esp-14h] [ebp-24h]
  float v7; // [esp-4h] [ebp-14h]

  if ( vec[1] == 0.0 && *vec == 0.0 )
  {
    if ( COERCE_FLOAT(*((unsigned int *)vec + 2) ^ _mask__NegFloat_) < 0.0 )
      return 270.0f;
    else
      return 90.0f;
  }
  else
  {
    *((float *)&v4 + 1) = vec[2];
    v1 = *((float *)&v4 + 1);
    __libm_sse2_atan2(v3, v4);
    *(float *)&v1 = v1;
    v7 = (float)(*(float *)&v1 * -180.0) / 3.1415927;
    if ( v7 < 0.0 )
      v5 = 360.0f;
    else
      v5 = 0.0f;
    return (float)(v7 + v5);
  }
}

void __cdecl vectoangles(const float *vec, float *angles)
{
  double v2; // xmm0_8
  double v3; // xmm0_8
  long double v4; // [esp-1Ch] [ebp-3Ch]
  long double v5; // [esp-1Ch] [ebp-3Ch]
  long double v6; // [esp-14h] [ebp-34h]
  long double v7; // [esp-14h] [ebp-34h]
  float v8; // [esp-14h] [ebp-34h]
  float v9; // [esp-4h] [ebp-24h]
  float v10; // [esp+8h] [ebp-18h]
  float v11; // [esp+8h] [ebp-18h]
  float v12; // [esp+Ch] [ebp-14h]
  float v13; // [esp+10h] [ebp-10h]
  float v14; // [esp+10h] [ebp-10h]

  if ( vec[1] == 0.0 && *vec == 0.0 )
  {
    v13 = 0.0f;
    if ( COERCE_FLOAT(*((unsigned int *)vec + 2) ^ _mask__NegFloat_) < 0.0 )
      v12 = 270.0f;
    else
      v12 = 90.0f;
    v10 = v12;
  }
  else
  {
    v2 = vec[1];
    __libm_sse2_atan2(v4, v6);
    *(float *)&v2 = v2;
    v14 = (float)(*(float *)&v2 * 180.0) / 3.1415927;
    if ( v14 < 0.0 )
      v9 = 360.0f;
    else
      v9 = 0.0f;
    v13 = v14 + v9;
    *((float *)&v7 + 1) = vec[2];
    v3 = *((float *)&v7 + 1);
    __libm_sse2_atan2(v5, v7);
    *(float *)&v3 = v3;
    v11 = (float)(*(float *)&v3 * -180.0) / 3.1415927;
    if ( v11 < 0.0 )
      v8 = 360.0f;
    else
      v8 = 0.0f;
    v10 = v11 + v8;
  }
  *angles = v10;
  angles[1] = v13;
  angles[2] = 0.0f;
}

void __cdecl UnitQuatToAngles(const float *quat, float *angles)
{
  float axis[3][3]; // [esp+0h] [ebp-24h] BYREF

  UnitQuatToAxis(quat, axis);
  AxisToAngles(axis, angles);
}

void __cdecl YawVectors(float yaw, float *forward, float *right)
{
  long double v3; // st7
  float cy; // [esp+8h] [ebp-Ch]
  float sy; // [esp+10h] [ebp-4h]

  v3 = (float)(yaw * 0.017453292);
  cy = cos(v3);
  sy = sin(v3);
  if ( forward )
  {
    *forward = cy;
    forward[1] = sy;
    forward[2] = 0.0f;
  }
  if ( right )
  {
    *right = sy;
    *((unsigned int *)right + 1) = LODWORD(cy) ^ _mask__NegFloat_;
    right[2] = 0.0f;
  }
}

void __cdecl YawVectors2D(float yaw, float *forward, float *right)
{
  long double v3; // st7
  float cy; // [esp+8h] [ebp-Ch]
  float sy; // [esp+10h] [ebp-4h]

  v3 = (float)(yaw * 0.017453292);
  cy = cos(v3);
  sy = sin(v3);
  if ( forward )
  {
    *forward = cy;
    forward[1] = sy;
  }
  if ( right )
  {
    *right = sy;
    *((unsigned int *)right + 1) = LODWORD(cy) ^ _mask__NegFloat_;
  }
}

void __cdecl PerpendicularVector(const float *src, float *dst)
{
  double v2; // st7
  const char *v3; // eax
  int pos; // [esp+38h] [ebp-14h]
  float d; // [esp+3Ch] [ebp-10h]
  float srcSq[3]; // [esp+40h] [ebp-Ch]

  if ( !Vec3IsNormalized(src) )
  {
    v2 = Abs(src);
    v3 = va("(%g %g %g) len %g", *src, src[1], src[2], v2);
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
            992,
            0,
            "%s\n\t%s",
            "Vec3IsNormalized( src )",
            v3) )
      __debugbreak();
  }
  srcSq[0] = *src * *src;
  srcSq[1] = src[1] * src[1];
  srcSq[2] = src[2] * src[2];
  pos = srcSq[0] > srcSq[1];
  if ( srcSq[pos] > srcSq[2] )
    pos = 2;
  LODWORD(d) = LODWORD(src[pos]) ^ _mask__NegFloat_;
  *dst = d * *src;
  dst[1] = d * src[1];
  dst[2] = d * src[2];
  dst[pos] = dst[pos] + 1.0;
  Vec3Normalize(dst);
}

double __cdecl PointToLineDistSq(const float *point, const float *start, const float *end)
{
  float segDot; // [esp+10h] [ebp-20h]
  float seg; // [esp+14h] [ebp-1Ch]
  float seg_4; // [esp+18h] [ebp-18h]
  float seg_8; // [esp+1Ch] [ebp-14h]
  float ptToLine; // [esp+20h] [ebp-10h]
  float ptToLine_4; // [esp+24h] [ebp-Ch]
  float ptToLine_8; // [esp+28h] [ebp-8h]
  float ptToLineDot; // [esp+2Ch] [ebp-4h]

  seg = *end - *start;
  seg_4 = end[1] - start[1];
  seg_8 = end[2] - start[2];
  ptToLine = *point - *start;
  ptToLine_4 = point[1] - start[1];
  ptToLine_8 = point[2] - start[2];
  segDot = (float)((float)(seg * seg) + (float)(seg_4 * seg_4)) + (float)(seg_8 * seg_8);
  if ( segDot == 0.0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 1082, 0, "%s", "segDot") )
  {
    __debugbreak();
  }
  ptToLineDot = (float)((float)(ptToLine * seg) + (float)(ptToLine_4 * seg_4)) + (float)(ptToLine_8 * seg_8);
  return (float)((float)(COERCE_FLOAT(COERCE_UNSIGNED_INT(ptToLineDot / segDot) ^ _mask__NegFloat_) * seg_8) + ptToLine_8)
       * (float)((float)(COERCE_FLOAT(COERCE_UNSIGNED_INT(ptToLineDot / segDot) ^ _mask__NegFloat_) * seg_8) + ptToLine_8)
       + (float)((float)(COERCE_FLOAT(COERCE_UNSIGNED_INT(ptToLineDot / segDot) ^ _mask__NegFloat_) * seg_4) + ptToLine_4)
       * (float)((float)(COERCE_FLOAT(COERCE_UNSIGNED_INT(ptToLineDot / segDot) ^ _mask__NegFloat_) * seg_4) + ptToLine_4)
       + (float)((float)(COERCE_FLOAT(COERCE_UNSIGNED_INT(ptToLineDot / segDot) ^ _mask__NegFloat_) * seg) + ptToLine)
       * (float)((float)(COERCE_FLOAT(COERCE_UNSIGNED_INT(ptToLineDot / segDot) ^ _mask__NegFloat_) * seg) + ptToLine);
}

double __cdecl PointToLineDistSq2D(const float *point, const float *start, const float *end)
{
  float segDot; // [esp+Ch] [ebp-18h]
  float seg; // [esp+10h] [ebp-14h]
  float seg_4; // [esp+14h] [ebp-10h]
  float ptToLine; // [esp+18h] [ebp-Ch]
  float ptToLine_4; // [esp+1Ch] [ebp-8h]
  float ptToLineDot; // [esp+20h] [ebp-4h]

  seg = *end - *start;
  seg_4 = end[1] - start[1];
  ptToLine = *point - *start;
  ptToLine_4 = point[1] - start[1];
  segDot = (float)(seg * seg) + (float)(seg_4 * seg_4);
  if ( segDot == 0.0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 1108, 0, "%s", "segDot") )
  {
    __debugbreak();
  }
  ptToLineDot = (float)(ptToLine * seg) + (float)(ptToLine_4 * seg_4);
  return (float)((float)(COERCE_FLOAT(COERCE_UNSIGNED_INT(ptToLineDot / segDot) ^ _mask__NegFloat_) * seg_4) + ptToLine_4)
       * (float)((float)(COERCE_FLOAT(COERCE_UNSIGNED_INT(ptToLineDot / segDot) ^ _mask__NegFloat_) * seg_4) + ptToLine_4)
       + (float)((float)(COERCE_FLOAT(COERCE_UNSIGNED_INT(ptToLineDot / segDot) ^ _mask__NegFloat_) * seg) + ptToLine)
       * (float)((float)(COERCE_FLOAT(COERCE_UNSIGNED_INT(ptToLineDot / segDot) ^ _mask__NegFloat_) * seg) + ptToLine);
}

int __cdecl BoxInPlanes(const float (*planes)[4], unsigned int numPlanes, const float *mins, const float *maxs)
{
  float v5; // [esp+0h] [ebp-2Ch]
  float v6; // [esp+4h] [ebp-28h]
  float v7; // [esp+8h] [ebp-24h]
  float v8; // [esp+Ch] [ebp-20h]
  float v9; // [esp+10h] [ebp-1Ch]
  float v10; // [esp+14h] [ebp-18h]
  float v11; // [esp+18h] [ebp-14h]
  float v12; // [esp+1Ch] [ebp-10h]
  const float *plane; // [esp+20h] [ebp-Ch]
  unsigned int planeIndex; // [esp+28h] [ebp-4h]

  for ( planeIndex = 0; planeIndex < numPlanes; ++planeIndex )
  {
    plane = &(*planes)[4 * planeIndex];
    if ( (float)((float)((float)((float)(*plane * *mins) + (float)(plane[1] * mins[1])) + (float)(plane[2] * mins[2]))
               + plane[3]) < 0.0 )
      v12 = 1.0f;
    else
      v12 = 0.0f;
    if ( (float)((float)((float)((float)(*plane * *mins) + (float)(plane[1] * mins[1])) + (float)(plane[2] * maxs[2]))
               + plane[3]) < 0.0 )
      v11 = 1.0f;
    else
      v11 = 0.0f;
    if ( (float)((float)((float)((float)(*plane * *mins) + (float)(plane[1] * maxs[1])) + (float)(plane[2] * mins[2]))
               + plane[3]) < 0.0 )
      v10 = 1.0f;
    else
      v10 = 0.0f;
    if ( (float)((float)((float)((float)(*plane * *mins) + (float)(plane[1] * maxs[1])) + (float)(plane[2] * maxs[2]))
               + plane[3]) < 0.0 )
      v9 = 1.0f;
    else
      v9 = 0.0f;
    if ( (float)((float)((float)((float)(*plane * *maxs) + (float)(plane[1] * mins[1])) + (float)(plane[2] * mins[2]))
               + plane[3]) < 0.0 )
      v8 = 1.0f;
    else
      v8 = 0.0f;
    if ( (float)((float)((float)((float)(*plane * *maxs) + (float)(plane[1] * mins[1])) + (float)(plane[2] * maxs[2]))
               + plane[3]) < 0.0 )
      v7 = 1.0f;
    else
      v7 = 0.0f;
    if ( (float)((float)((float)((float)(*plane * *maxs) + (float)(plane[1] * maxs[1])) + (float)(plane[2] * mins[2]))
               + plane[3]) < 0.0 )
      v6 = 1.0f;
    else
      v6 = 0.0f;
    if ( (float)((float)((float)((float)(*plane * *maxs) + (float)(plane[1] * maxs[1])) + (float)(plane[2] * maxs[2]))
               + plane[3]) < 0.0 )
      v5 = 1.0f;
    else
      v5 = 0.0f;
    if ( (float)((float)((float)((float)((float)((float)((float)((float)(v12 + 0.0) + v11) + v10) + v9) + v8) + v7) + v6)
               + v5) == 0.0 )
      return 0;
  }
  return 1;
}

int __cdecl SphereInPlanes(const float (*planes)[4], unsigned int numPlanes, const float *center, float radius)
{
  unsigned int planeIndex; // [esp+4h] [ebp-4h]

  for ( planeIndex = 0; planeIndex < numPlanes; ++planeIndex )
  {
    if ( (float)((float)((float)((float)((float)((float)(*planes)[4 * planeIndex] * *center)
                                       + (float)((float)(*planes)[4 * planeIndex + 1] * center[1]))
                               + (float)((float)(*planes)[4 * planeIndex + 2] * center[2]))
                       + (float)(*planes)[4 * planeIndex + 3])
               - radius) > 0.0 )
      return 0;
  }
  return 1;
}

void __cdecl MatrixIdentity33(float (*out)[3])
{
  if ( !out && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 1227, 0, "%s", "out") )
    __debugbreak();
  (*out)[0] = 0.0;
  (*out)[1] = 0.0;
  (*out)[2] = 0.0;
  (*out)[3] = 0.0;
  (*out)[4] = 0.0;
  (*out)[5] = 0.0;
  (*out)[6] = 0.0;
  (*out)[7] = 0.0;
  (*out)[8] = 0.0;
  (*out)[0] = 1.0f;
  (*out)[4] = 1.0f;
  (*out)[8] = 1.0f;
}

void __cdecl MatrixIdentity44(float (*out)[4])
{
  if ( !out && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 1236, 0, "%s", "out") )
    __debugbreak();
  memcpy(out, identityMatrix44, 0x40u);
}

void __cdecl MatrixMultiply(const float (*in1)[3], const float (*in2)[3], float (*out)[3])
{
  (*out)[0] = (float)((float)((*in1)[0] * (*in2)[0]) + (float)((float)(*in1)[1] * (float)(*in2)[3]))
            + (float)((float)(*in1)[2] * (float)(*in2)[6]);
  (*out)[1] = (float)((float)((*in1)[0] * (float)(*in2)[1]) + (float)((float)(*in1)[1] * (float)(*in2)[4]))
            + (float)((float)(*in1)[2] * (float)(*in2)[7]);
  (*out)[2] = (float)((float)((*in1)[0] * (float)(*in2)[2]) + (float)((float)(*in1)[1] * (float)(*in2)[5]))
            + (float)((float)(*in1)[2] * (float)(*in2)[8]);
  (*out)[3] = (float)((float)((float)(*in1)[3] * (*in2)[0]) + (float)((float)(*in1)[4] * (float)(*in2)[3]))
            + (float)((float)(*in1)[5] * (float)(*in2)[6]);
  (*out)[4] = (float)((float)((float)(*in1)[3] * (float)(*in2)[1]) + (float)((float)(*in1)[4] * (float)(*in2)[4]))
            + (float)((float)(*in1)[5] * (float)(*in2)[7]);
  (*out)[5] = (float)((float)((float)(*in1)[3] * (float)(*in2)[2]) + (float)((float)(*in1)[4] * (float)(*in2)[5]))
            + (float)((float)(*in1)[5] * (float)(*in2)[8]);
  (*out)[6] = (float)((float)((float)(*in1)[6] * (*in2)[0]) + (float)((float)(*in1)[7] * (float)(*in2)[3]))
            + (float)((float)(*in1)[8] * (float)(*in2)[6]);
  (*out)[7] = (float)((float)((float)(*in1)[6] * (float)(*in2)[1]) + (float)((float)(*in1)[7] * (float)(*in2)[4]))
            + (float)((float)(*in1)[8] * (float)(*in2)[7]);
  (*out)[8] = (float)((float)((float)(*in1)[6] * (float)(*in2)[2]) + (float)((float)(*in1)[7] * (float)(*in2)[5]))
            + (float)((float)(*in1)[8] * (float)(*in2)[8]);
}

void __cdecl MatrixVecMultiply(const float (*mulMat)[3], const float *mulVec, float *solution)
{
  *solution = (float)((float)((*mulMat)[0] * *mulVec) + (float)((float)(*mulMat)[1] * mulVec[1]))
            + (float)((float)(*mulMat)[2] * mulVec[2]);
  solution[1] = (float)((float)((float)(*mulMat)[3] * *mulVec) + (float)((float)(*mulMat)[4] * mulVec[1]))
              + (float)((float)(*mulMat)[5] * mulVec[2]);
  solution[2] = (float)((float)((float)(*mulMat)[6] * *mulVec) + (float)((float)(*mulMat)[7] * mulVec[1]))
              + (float)((float)(*mulMat)[8] * mulVec[2]);
}

void __cdecl MatrixVecMultiply(const float (*mulMat)[4], const float *mulVec, float *solution)
{
  *solution = (float)((float)((float)((*mulMat)[0] * *mulVec) + (float)((float)(*mulMat)[1] * mulVec[1]))
                    + (float)((float)(*mulMat)[2] * mulVec[2]))
            + (float)((float)(*mulMat)[3] * mulVec[3]);
  solution[1] = (float)((float)((float)((float)(*mulMat)[4] * *mulVec) + (float)((float)(*mulMat)[5] * mulVec[1]))
                      + (float)((float)(*mulMat)[6] * mulVec[2]))
              + (float)((float)(*mulMat)[7] * mulVec[3]);
  solution[2] = (float)((float)((float)((float)(*mulMat)[8] * *mulVec) + (float)((float)(*mulMat)[9] * mulVec[1]))
                      + (float)((float)(*mulMat)[10] * mulVec[2]))
              + (float)((float)(*mulMat)[11] * mulVec[3]);
  solution[3] = (float)((float)((float)((float)(*mulMat)[12] * *mulVec) + (float)((float)(*mulMat)[13] * mulVec[1]))
                      + (float)((float)(*mulMat)[14] * mulVec[2]))
              + (float)((float)(*mulMat)[15] * mulVec[3]);
}

void __cdecl MatrixVecMultiplyProject(const float (*mulMat)[4], const float *mulVec, float *solution)
{
  float ret[4]; // [esp+0h] [ebp-24h] BYREF
  float mul[4]; // [esp+10h] [ebp-14h] BYREF
  float invW; // [esp+20h] [ebp-4h]

  mul[0] = *mulVec;
  mul[1] = mulVec[1];
  mul[2] = mulVec[2];
  mul[3] = 1.0f;
  MatrixVecMultiply(mulMat, mul, ret);
  invW = 1.0 / ret[3];
  *solution = ret[0] * (float)(1.0 / ret[3]);
  solution[1] = ret[1] * invW;
  solution[2] = ret[2] * invW;
}

void __cdecl MatrixMultiplyEquals(const float (*in)[3], float (*out)[3])
{
  __int64 temp; // [esp+8h] [ebp-18h]
  float temp_8; // [esp+10h] [ebp-10h]
  float temp_12; // [esp+14h] [ebp-Ch]
  float temp_16; // [esp+18h] [ebp-8h]
  float temp_20; // [esp+1Ch] [ebp-4h]

  *(float *)&temp = (float)((float)((*in)[0] * (*out)[0]) + (float)((float)(*in)[1] * (*out)[3]))
                  + (float)((float)(*in)[2] * (*out)[6]);
  *((float *)&temp + 1) = (float)((float)((*in)[0] * (*out)[1]) + (float)((float)(*in)[1] * (*out)[4]))
                        + (float)((float)(*in)[2] * (*out)[7]);
  temp_8 = (float)((float)((*in)[0] * (*out)[2]) + (float)((float)(*in)[1] * (*out)[5]))
         + (float)((float)(*in)[2] * (*out)[8]);
  temp_12 = (float)((float)((float)(*in)[3] * (*out)[0]) + (float)((float)(*in)[4] * (*out)[3]))
          + (float)((float)(*in)[5] * (*out)[6]);
  temp_16 = (float)((float)((float)(*in)[3] * (*out)[1]) + (float)((float)(*in)[4] * (*out)[4]))
          + (float)((float)(*in)[5] * (*out)[7]);
  temp_20 = (float)((float)((float)(*in)[3] * (*out)[2]) + (float)((float)(*in)[4] * (*out)[5]))
          + (float)((float)(*in)[5] * (*out)[8]);
  (*out)[6] = (float)((float)((float)(*in)[6] * (*out)[0]) + (float)((float)(*in)[7] * (*out)[3]))
            + (float)((float)(*in)[8] * (*out)[6]);
  (*out)[7] = (float)((float)((float)(*in)[6] * (*out)[1]) + (float)((float)(*in)[7] * (*out)[4]))
            + (float)((float)(*in)[8] * (*out)[7]);
  (*out)[8] = (float)((float)((float)(*in)[6] * (*out)[2]) + (float)((float)(*in)[7] * (*out)[5]))
            + (float)((float)(*in)[8] * (*out)[8]);
  *(_QWORD *)&(*out)[0] = temp;
  (*out)[2] = temp_8;
  (*out)[3] = temp_12;
  (*out)[4] = temp_16;
  (*out)[5] = temp_20;
}

void __cdecl MatrixMultiply43(const float (*in1)[3], const float (*in2)[3], float (*out)[3])
{
  if ( in1 == out
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 1449, 0, "%s", "in1 != out") )
  {
    __debugbreak();
  }
  if ( in2 == out
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 1450, 0, "%s", "in2 != out") )
  {
    __debugbreak();
  }
  (*out)[0] = (float)((float)((*in1)[0] * (*in2)[0]) + (float)((float)(*in1)[1] * (float)(*in2)[3]))
            + (float)((float)(*in1)[2] * (float)(*in2)[6]);
  (*out)[3] = (float)((float)((float)(*in1)[3] * (*in2)[0]) + (float)((float)(*in1)[4] * (float)(*in2)[3]))
            + (float)((float)(*in1)[5] * (float)(*in2)[6]);
  (*out)[6] = (float)((float)((float)(*in1)[6] * (*in2)[0]) + (float)((float)(*in1)[7] * (float)(*in2)[3]))
            + (float)((float)(*in1)[8] * (float)(*in2)[6]);
  (*out)[1] = (float)((float)((*in1)[0] * (float)(*in2)[1]) + (float)((float)(*in1)[1] * (float)(*in2)[4]))
            + (float)((float)(*in1)[2] * (float)(*in2)[7]);
  (*out)[4] = (float)((float)((float)(*in1)[3] * (float)(*in2)[1]) + (float)((float)(*in1)[4] * (float)(*in2)[4]))
            + (float)((float)(*in1)[5] * (float)(*in2)[7]);
  (*out)[7] = (float)((float)((float)(*in1)[6] * (float)(*in2)[1]) + (float)((float)(*in1)[7] * (float)(*in2)[4]))
            + (float)((float)(*in1)[8] * (float)(*in2)[7]);
  (*out)[2] = (float)((float)((*in1)[0] * (float)(*in2)[2]) + (float)((float)(*in1)[1] * (float)(*in2)[5]))
            + (float)((float)(*in1)[2] * (float)(*in2)[8]);
  (*out)[5] = (float)((float)((float)(*in1)[3] * (float)(*in2)[2]) + (float)((float)(*in1)[4] * (float)(*in2)[5]))
            + (float)((float)(*in1)[5] * (float)(*in2)[8]);
  (*out)[8] = (float)((float)((float)(*in1)[6] * (float)(*in2)[2]) + (float)((float)(*in1)[7] * (float)(*in2)[5]))
            + (float)((float)(*in1)[8] * (float)(*in2)[8]);
  (*out)[9] = (float)((float)((float)((float)(*in1)[9] * (*in2)[0]) + (float)((float)(*in1)[10] * (float)(*in2)[3]))
                    + (float)((float)(*in1)[11] * (float)(*in2)[6]))
            + (float)(*in2)[9];
  (*out)[10] = (float)((float)((float)((float)(*in1)[9] * (float)(*in2)[1])
                             + (float)((float)(*in1)[10] * (float)(*in2)[4]))
                     + (float)((float)(*in1)[11] * (float)(*in2)[7]))
             + (float)(*in2)[10];
  (*out)[11] = (float)((float)((float)((float)(*in1)[9] * (float)(*in2)[2])
                             + (float)((float)(*in1)[10] * (float)(*in2)[5]))
                     + (float)((float)(*in1)[11] * (float)(*in2)[8]))
             + (float)(*in2)[11];
}

void __cdecl MatrixMultiply44(const float (*in1)[4], const float (*in2)[4], float (*out)[4])
{
  if ( in1 == out
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 1488, 0, "%s", "in1 != out") )
  {
    __debugbreak();
  }
  if ( in2 == out
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 1489, 0, "%s", "in2 != out") )
  {
    __debugbreak();
  }
  (*out)[0] = (float)((float)((float)((*in1)[0] * (*in2)[0]) + (float)((float)(*in1)[1] * (float)(*in2)[4]))
                    + (float)((float)(*in1)[2] * (float)(*in2)[8]))
            + (float)((float)(*in1)[3] * (float)(*in2)[12]);
  (*out)[1] = (float)((float)((float)((*in1)[0] * (float)(*in2)[1]) + (float)((float)(*in1)[1] * (float)(*in2)[5]))
                    + (float)((float)(*in1)[2] * (float)(*in2)[9]))
            + (float)((float)(*in1)[3] * (float)(*in2)[13]);
  (*out)[2] = (float)((float)((float)((*in1)[0] * (float)(*in2)[2]) + (float)((float)(*in1)[1] * (float)(*in2)[6]))
                    + (float)((float)(*in1)[2] * (float)(*in2)[10]))
            + (float)((float)(*in1)[3] * (float)(*in2)[14]);
  (*out)[3] = (float)((float)((float)((*in1)[0] * (float)(*in2)[3]) + (float)((float)(*in1)[1] * (float)(*in2)[7]))
                    + (float)((float)(*in1)[2] * (float)(*in2)[11]))
            + (float)((float)(*in1)[3] * (float)(*in2)[15]);
  (*out)[4] = (float)((float)((float)((float)(*in1)[4] * (*in2)[0]) + (float)((float)(*in1)[5] * (float)(*in2)[4]))
                    + (float)((float)(*in1)[6] * (float)(*in2)[8]))
            + (float)((float)(*in1)[7] * (float)(*in2)[12]);
  (*out)[5] = (float)((float)((float)((float)(*in1)[4] * (float)(*in2)[1]) + (float)((float)(*in1)[5] * (float)(*in2)[5]))
                    + (float)((float)(*in1)[6] * (float)(*in2)[9]))
            + (float)((float)(*in1)[7] * (float)(*in2)[13]);
  (*out)[6] = (float)((float)((float)((float)(*in1)[4] * (float)(*in2)[2]) + (float)((float)(*in1)[5] * (float)(*in2)[6]))
                    + (float)((float)(*in1)[6] * (float)(*in2)[10]))
            + (float)((float)(*in1)[7] * (float)(*in2)[14]);
  (*out)[7] = (float)((float)((float)((float)(*in1)[4] * (float)(*in2)[3]) + (float)((float)(*in1)[5] * (float)(*in2)[7]))
                    + (float)((float)(*in1)[6] * (float)(*in2)[11]))
            + (float)((float)(*in1)[7] * (float)(*in2)[15]);
  (*out)[8] = (float)((float)((float)((float)(*in1)[8] * (*in2)[0]) + (float)((float)(*in1)[9] * (float)(*in2)[4]))
                    + (float)((float)(*in1)[10] * (float)(*in2)[8]))
            + (float)((float)(*in1)[11] * (float)(*in2)[12]);
  (*out)[9] = (float)((float)((float)((float)(*in1)[8] * (float)(*in2)[1]) + (float)((float)(*in1)[9] * (float)(*in2)[5]))
                    + (float)((float)(*in1)[10] * (float)(*in2)[9]))
            + (float)((float)(*in1)[11] * (float)(*in2)[13]);
  (*out)[10] = (float)((float)((float)((float)(*in1)[8] * (float)(*in2)[2])
                             + (float)((float)(*in1)[9] * (float)(*in2)[6]))
                     + (float)((float)(*in1)[10] * (float)(*in2)[10]))
             + (float)((float)(*in1)[11] * (float)(*in2)[14]);
  (*out)[11] = (float)((float)((float)((float)(*in1)[8] * (float)(*in2)[3])
                             + (float)((float)(*in1)[9] * (float)(*in2)[7]))
                     + (float)((float)(*in1)[10] * (float)(*in2)[11]))
             + (float)((float)(*in1)[11] * (float)(*in2)[15]);
  (*out)[12] = (float)((float)((float)((float)(*in1)[12] * (*in2)[0]) + (float)((float)(*in1)[13] * (float)(*in2)[4]))
                     + (float)((float)(*in1)[14] * (float)(*in2)[8]))
             + (float)((float)(*in1)[15] * (float)(*in2)[12]);
  (*out)[13] = (float)((float)((float)((float)(*in1)[12] * (float)(*in2)[1])
                             + (float)((float)(*in1)[13] * (float)(*in2)[5]))
                     + (float)((float)(*in1)[14] * (float)(*in2)[9]))
             + (float)((float)(*in1)[15] * (float)(*in2)[13]);
  (*out)[14] = (float)((float)((float)((float)(*in1)[12] * (float)(*in2)[2])
                             + (float)((float)(*in1)[13] * (float)(*in2)[6]))
                     + (float)((float)(*in1)[14] * (float)(*in2)[10]))
             + (float)((float)(*in1)[15] * (float)(*in2)[14]);
  (*out)[15] = (float)((float)((float)((float)(*in1)[12] * (float)(*in2)[3])
                             + (float)((float)(*in1)[13] * (float)(*in2)[7]))
                     + (float)((float)(*in1)[14] * (float)(*in2)[11]))
             + (float)((float)(*in1)[15] * (float)(*in2)[15]);
}

void __cdecl MatrixTranspose(const float (*in)[3], float (*out)[3])
{
  if ( in == out
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 1520, 0, "%s", "in != out") )
  {
    __debugbreak();
  }
  (*out)[0] = (*in)[0];
  (*out)[1] = (*in)[3];
  (*out)[2] = (*in)[6];
  (*out)[3] = (*in)[1];
  (*out)[4] = (*in)[4];
  (*out)[5] = (*in)[7];
  (*out)[6] = (*in)[2];
  (*out)[7] = (*in)[5];
  (*out)[8] = (*in)[8];
}

void __cdecl MatrixTranspose44(const float *in, float *out)
{
  if ( in == out
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 1548, 0, "%s", "in != out") )
  {
    __debugbreak();
  }
  *out = *in;
  out[1] = in[4];
  out[2] = in[8];
  out[3] = in[12];
  out[4] = in[1];
  out[5] = in[5];
  out[6] = in[9];
  out[7] = in[13];
  out[8] = in[2];
  out[9] = in[6];
  out[10] = in[10];
  out[11] = in[14];
  out[12] = in[3];
  out[13] = in[7];
  out[14] = in[11];
  out[15] = in[15];
}

void __cdecl MatrixInverse(const float (*in)[3], float (*out)[3])
{
  float det; // [esp+0h] [ebp-4h]
  float deta; // [esp+0h] [ebp-4h]

  if ( in == out
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 1578, 0, "%s", "in != out") )
  {
    __debugbreak();
  }
  det = (float)((float)((float)((float)((float)(*in)[8] * (float)(*in)[4]) - (float)((float)(*in)[7] * (float)(*in)[5]))
                      * (*in)[0])
              - (float)((float)((float)((float)(*in)[8] * (float)(*in)[1]) - (float)((float)(*in)[7] * (float)(*in)[2]))
                      * (float)(*in)[3]))
      + (float)((float)((float)((float)(*in)[5] * (float)(*in)[1]) - (float)((float)(*in)[4] * (float)(*in)[2]))
              * (float)(*in)[6]);
  if ( det == 0.0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 1582, 0, "%s", "det") )
  {
    __debugbreak();
  }
  deta = 1.0 / det;
  (*out)[0] = (float)((float)((float)(*in)[8] * (float)(*in)[4]) - (float)((float)(*in)[7] * (float)(*in)[5])) * deta;
  (*out)[1] = COERCE_FLOAT(
                COERCE_UNSIGNED_INT((float)((float)(*in)[8] * (float)(*in)[1]) - (float)((float)(*in)[7]
                                                                                       * (float)(*in)[2]))
              ^ _mask__NegFloat_)
            * deta;
  (*out)[2] = (float)((float)((float)(*in)[5] * (float)(*in)[1]) - (float)((float)(*in)[4] * (float)(*in)[2])) * deta;
  (*out)[3] = COERCE_FLOAT(
                COERCE_UNSIGNED_INT((float)((float)(*in)[8] * (float)(*in)[3]) - (float)((float)(*in)[6]
                                                                                       * (float)(*in)[5]))
              ^ _mask__NegFloat_)
            * deta;
  (*out)[4] = (float)((float)((float)(*in)[8] * (*in)[0]) - (float)((float)(*in)[6] * (float)(*in)[2])) * deta;
  (*out)[5] = COERCE_FLOAT(
                COERCE_UNSIGNED_INT((float)((float)(*in)[5] * (*in)[0]) - (float)((float)(*in)[3] * (float)(*in)[2]))
              ^ _mask__NegFloat_)
            * deta;
  (*out)[6] = (float)((float)((float)(*in)[7] * (float)(*in)[3]) - (float)((float)(*in)[6] * (float)(*in)[4])) * deta;
  (*out)[7] = COERCE_FLOAT(
                COERCE_UNSIGNED_INT((float)((float)(*in)[7] * (*in)[0]) - (float)((float)(*in)[6] * (float)(*in)[1]))
              ^ _mask__NegFloat_)
            * deta;
  (*out)[8] = (float)((float)((float)(*in)[4] * (*in)[0]) - (float)((float)(*in)[3] * (float)(*in)[1])) * deta;
}

void __cdecl MatrixInverseOrthogonal43(const float (*in)[3], float (*out)[3])
{
  float origin[3]; // [esp+4h] [ebp-Ch] BYREF

  MatrixTranspose(in, out);
  origin[0] = 0.0 - (float)(*in)[9];
  origin[1] = 0.0 - (float)(*in)[10];
  origin[2] = 0.0 - (float)(*in)[11];
  MatrixTransformVector(origin, out, &(*out)[9]);
}

void __cdecl MatrixInverse44(const float *mat, float *dst)
{
  float src[16]; // [esp+0h] [ebp-78h]
  float det; // [esp+40h] [ebp-38h]
  float tmp[12]; // [esp+44h] [ebp-34h]
  int i; // [esp+74h] [ebp-4h]

  if ( mat == dst
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 1640, 0, "%s", "mat != dst") )
  {
    __debugbreak();
  }
  for ( i = 0; i < 4; ++i )
  {
    src[i] = mat[4 * i];
    src[i + 4] = mat[4 * i + 1];
    src[i + 8] = mat[4 * i + 2];
    src[i + 12] = mat[4 * i + 3];
  }
  tmp[0] = src[10] * src[15];
  tmp[1] = src[11] * src[14];
  tmp[2] = src[9] * src[15];
  tmp[3] = src[11] * src[13];
  tmp[4] = src[9] * src[14];
  tmp[5] = src[10] * src[13];
  tmp[6] = src[8] * src[15];
  tmp[7] = src[11] * src[12];
  tmp[8] = src[8] * src[14];
  tmp[9] = src[10] * src[12];
  tmp[10] = src[8] * src[13];
  tmp[11] = src[9] * src[12];
  *dst = (float)((float)((float)(src[10] * src[15]) * src[5]) + (float)((float)(src[11] * src[13]) * src[6]))
       + (float)((float)(src[9] * src[14]) * src[7]);
  *dst = *dst - (float)((float)((float)(tmp[1] * src[5]) + (float)(tmp[2] * src[6])) + (float)(tmp[5] * src[7]));
  dst[1] = (float)((float)(tmp[1] * src[4]) + (float)(tmp[6] * src[6])) + (float)(tmp[9] * src[7]);
  dst[1] = dst[1] - (float)((float)((float)(tmp[0] * src[4]) + (float)(tmp[7] * src[6])) + (float)(tmp[8] * src[7]));
  dst[2] = (float)((float)(tmp[2] * src[4]) + (float)(tmp[7] * src[5])) + (float)(tmp[10] * src[7]);
  dst[2] = dst[2] - (float)((float)((float)(tmp[3] * src[4]) + (float)(tmp[6] * src[5])) + (float)(tmp[11] * src[7]));
  dst[3] = (float)((float)(tmp[5] * src[4]) + (float)(tmp[8] * src[5])) + (float)(tmp[11] * src[6]);
  dst[3] = dst[3] - (float)((float)((float)(tmp[4] * src[4]) + (float)(tmp[9] * src[5])) + (float)(tmp[10] * src[6]));
  dst[4] = (float)((float)(tmp[1] * src[1]) + (float)(tmp[2] * src[2])) + (float)(tmp[5] * src[3]);
  dst[4] = dst[4] - (float)((float)((float)(tmp[0] * src[1]) + (float)(tmp[3] * src[2])) + (float)(tmp[4] * src[3]));
  dst[5] = (float)((float)(tmp[0] * src[0]) + (float)(tmp[7] * src[2])) + (float)(tmp[8] * src[3]);
  dst[5] = dst[5] - (float)((float)((float)(tmp[1] * src[0]) + (float)(tmp[6] * src[2])) + (float)(tmp[9] * src[3]));
  dst[6] = (float)((float)(tmp[3] * src[0]) + (float)(tmp[6] * src[1])) + (float)(tmp[11] * src[3]);
  dst[6] = dst[6] - (float)((float)((float)(tmp[2] * src[0]) + (float)(tmp[7] * src[1])) + (float)(tmp[10] * src[3]));
  dst[7] = (float)((float)(tmp[4] * src[0]) + (float)(tmp[9] * src[1])) + (float)(tmp[10] * src[2]);
  dst[7] = dst[7] - (float)((float)((float)(tmp[5] * src[0]) + (float)(tmp[8] * src[1])) + (float)(tmp[11] * src[2]));
  tmp[0] = src[2] * src[7];
  tmp[1] = src[3] * src[6];
  tmp[2] = src[1] * src[7];
  tmp[3] = src[3] * src[5];
  tmp[4] = src[1] * src[6];
  tmp[5] = src[2] * src[5];
  tmp[6] = src[0] * src[7];
  tmp[7] = src[3] * src[4];
  tmp[8] = src[0] * src[6];
  tmp[9] = src[2] * src[4];
  tmp[10] = src[0] * src[5];
  tmp[11] = src[1] * src[4];
  dst[8] = (float)((float)((float)(src[2] * src[7]) * src[13]) + (float)((float)(src[3] * src[5]) * src[14]))
         + (float)((float)(src[1] * src[6]) * src[15]);
  dst[8] = dst[8] - (float)((float)((float)(tmp[1] * src[13]) + (float)(tmp[2] * src[14])) + (float)(tmp[5] * src[15]));
  dst[9] = (float)((float)(tmp[1] * src[12]) + (float)(tmp[6] * src[14])) + (float)(tmp[9] * src[15]);
  dst[9] = dst[9] - (float)((float)((float)(tmp[0] * src[12]) + (float)(tmp[7] * src[14])) + (float)(tmp[8] * src[15]));
  dst[10] = (float)((float)(tmp[2] * src[12]) + (float)(tmp[7] * src[13])) + (float)(tmp[10] * src[15]);
  dst[10] = dst[10]
          - (float)((float)((float)(tmp[3] * src[12]) + (float)(tmp[6] * src[13])) + (float)(tmp[11] * src[15]));
  dst[11] = (float)((float)(tmp[5] * src[12]) + (float)(tmp[8] * src[13])) + (float)(tmp[11] * src[14]);
  dst[11] = dst[11]
          - (float)((float)((float)(tmp[4] * src[12]) + (float)(tmp[9] * src[13])) + (float)(tmp[10] * src[14]));
  dst[12] = (float)((float)(tmp[2] * src[10]) + (float)(tmp[5] * src[11])) + (float)(tmp[1] * src[9]);
  dst[12] = dst[12] - (float)((float)((float)(tmp[4] * src[11]) + (float)(tmp[0] * src[9])) + (float)(tmp[3] * src[10]));
  dst[13] = (float)((float)(tmp[8] * src[11]) + (float)(tmp[0] * src[8])) + (float)(tmp[7] * src[10]);
  dst[13] = dst[13] - (float)((float)((float)(tmp[6] * src[10]) + (float)(tmp[9] * src[11])) + (float)(tmp[1] * src[8]));
  dst[14] = (float)((float)(tmp[6] * src[9]) + (float)(tmp[11] * src[11])) + (float)(tmp[3] * src[8]);
  dst[14] = dst[14] - (float)((float)((float)(tmp[10] * src[11]) + (float)(tmp[2] * src[8])) + (float)(tmp[7] * src[9]));
  dst[15] = (float)((float)(tmp[10] * src[10]) + (float)(tmp[4] * src[8])) + (float)(tmp[9] * src[9]);
  dst[15] = dst[15] - (float)((float)((float)(tmp[8] * src[9]) + (float)(tmp[11] * src[10])) + (float)(tmp[5] * src[8]));
  det = (float)((float)((float)(src[0] * *dst) + (float)(src[1] * dst[1])) + (float)(src[2] * dst[2]))
      + (float)(src[3] * dst[3]);
  if ( det == 0.0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 1715, 0, "%s", "det") )
  {
    __debugbreak();
  }
  det = 1.0 / det;
  for ( i = 0; i < 16; ++i )
    dst[i] = dst[i] * det;
}

void __cdecl MatrixTransformVector44(const float *vec, const float (*mat)[4], float *out)
{
  if ( vec == out
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 1742, 0, "%s", "vec != out") )
  {
    __debugbreak();
  }
  *out = (float)((float)((float)(*vec * (*mat)[0]) + (float)(vec[1] * (float)(*mat)[4]))
               + (float)(vec[2] * (float)(*mat)[8]))
       + (float)(vec[3] * (float)(*mat)[12]);
  out[1] = (float)((float)((float)(*vec * (float)(*mat)[1]) + (float)(vec[1] * (float)(*mat)[5]))
                 + (float)(vec[2] * (float)(*mat)[9]))
         + (float)(vec[3] * (float)(*mat)[13]);
  out[2] = (float)((float)((float)(*vec * (float)(*mat)[2]) + (float)(vec[1] * (float)(*mat)[6]))
                 + (float)(vec[2] * (float)(*mat)[10]))
         + (float)(vec[3] * (float)(*mat)[14]);
  out[3] = (float)((float)((float)(*vec * (float)(*mat)[3]) + (float)(vec[1] * (float)(*mat)[7]))
                 + (float)(vec[2] * (float)(*mat)[11]))
         + (float)(vec[3] * (float)(*mat)[15]);
}

void __cdecl MatrixTransformVector43(const float *in1, const float (*in2)[3], float *out)
{
  if ( in1 == out
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 1752, 0, "%s", "in1 != out") )
  {
    __debugbreak();
  }
  *out = (float)((float)((float)(*in1 * (*in2)[0]) + (float)(in1[1] * (float)(*in2)[3]))
               + (float)(in1[2] * (float)(*in2)[6]))
       + (float)(*in2)[9];
  out[1] = (float)((float)((float)(*in1 * (float)(*in2)[1]) + (float)(in1[1] * (float)(*in2)[4]))
                 + (float)(in1[2] * (float)(*in2)[7]))
         + (float)(*in2)[10];
  out[2] = (float)((float)((float)(*in1 * (float)(*in2)[2]) + (float)(in1[1] * (float)(*in2)[5]))
                 + (float)(in1[2] * (float)(*in2)[8]))
         + (float)(*in2)[11];
}

void __cdecl MatrixTransposeTransformVector43(const float *in1, const float (*in2)[3], float *out)
{
  float temp; // [esp+4h] [ebp-Ch]
  float temp_4; // [esp+8h] [ebp-8h]
  float temp_8; // [esp+Ch] [ebp-4h]

  if ( in1 == out
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 1762, 0, "%s", "in1 != out") )
  {
    __debugbreak();
  }
  temp = *in1 - (float)(*in2)[9];
  temp_4 = in1[1] - (float)(*in2)[10];
  temp_8 = in1[2] - (float)(*in2)[11];
  *out = (float)((float)((*in2)[0] * temp) + (float)((float)(*in2)[1] * temp_4)) + (float)((float)(*in2)[2] * temp_8);
  out[1] = (float)((float)((float)(*in2)[3] * temp) + (float)((float)(*in2)[4] * temp_4))
         + (float)((float)(*in2)[5] * temp_8);
  out[2] = (float)((float)((float)(*in2)[6] * temp) + (float)((float)(*in2)[7] * temp_4))
         + (float)((float)(*in2)[8] * temp_8);
}

void __cdecl MatrixTransformVector43Equals(float *out, const float *in)
{
  float temp; // [esp+0h] [ebp-8h]
  float temp_4; // [esp+4h] [ebp-4h]

  temp = (float)((float)((float)(*out * *in) + (float)(out[1] * in[3])) + (float)(out[2] * in[6])) + in[9];
  temp_4 = (float)((float)((float)(*out * in[1]) + (float)(out[1] * in[4])) + (float)(out[2] * in[7])) + in[10];
  out[2] = (float)((float)((float)(*out * in[2]) + (float)(out[1] * in[5])) + (float)(out[2] * in[8])) + in[11];
  *out = temp;
  out[1] = temp_4;
}

void __cdecl VectorAngleMultiply(float *vec, float angle)
{
  long double v2; // st7
  float temp; // [esp+Ch] [ebp-Ch]
  float x; // [esp+10h] [ebp-8h]
  float y; // [esp+14h] [ebp-4h]

  v2 = (float)(angle * 0.017453292);
  x = cos(v2);
  y = sin(v2);
  temp = (float)(*vec * x) - (float)(vec[1] * y);
  vec[1] = (float)(vec[1] * x) + (float)(*vec * y);
  *vec = temp;
}

void __cdecl UnitQuatToAxis(const float *quat, float (*axis)[3])
{
  const char *v2; // eax
  float yy; // [esp+24h] [ebp-30h]
  float xy; // [esp+28h] [ebp-2Ch]
  float scaledZ; // [esp+2Ch] [ebp-28h]
  float zz; // [esp+30h] [ebp-24h]
  float zw; // [esp+34h] [ebp-20h]
  float scaledX; // [esp+38h] [ebp-1Ch]
  float scaledY; // [esp+3Ch] [ebp-18h]
  float yw; // [esp+40h] [ebp-14h]
  float xz; // [esp+44h] [ebp-10h]
  float yz; // [esp+48h] [ebp-Ch]
  float xx; // [esp+4Ch] [ebp-8h]
  float xw; // [esp+50h] [ebp-4h]

  if ( !Vec4IsNormalized(quat) )
  {
    v2 = va("%g %g %g %g", *quat, quat[1], quat[2], quat[3]);
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
            1799,
            0,
            "%s\n\t%s",
            "Vec4IsNormalized( quat )",
            v2) )
      __debugbreak();
  }
  scaledX = *quat + *quat;
  xx = scaledX * *quat;
  xy = scaledX * quat[1];
  xz = scaledX * quat[2];
  xw = scaledX * quat[3];
  scaledY = quat[1] + quat[1];
  yy = scaledY * quat[1];
  yz = scaledY * quat[2];
  yw = scaledY * quat[3];
  scaledZ = quat[2] + quat[2];
  zw = scaledZ * quat[3];
  zz = scaledZ * quat[2];
  (*axis)[0] = 1.0 - (float)(yy + zz);
  (*axis)[1] = xy + zw;
  (*axis)[2] = xz - yw;
  (*axis)[3] = xy - zw;
  (*axis)[4] = 1.0 - (float)(xx + zz);
  (*axis)[5] = yz + xw;
  (*axis)[6] = xz + yw;
  (*axis)[7] = yz - xw;
  (*axis)[8] = 1.0 - (float)(xx + yy);
}

void __cdecl UnitQuatToForward(const float *quat, float *forward)
{
  const char *v2; // eax

  if ( !Vec4IsNormalized(quat) )
  {
    v2 = va("%g %g %g %g", *quat, quat[1], quat[2], quat[3]);
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
            1831,
            0,
            "%s\n\t%s",
            "Vec4IsNormalized( quat )",
            v2) )
      __debugbreak();
  }
  *forward = 1.0 - (float)((float)((float)(quat[1] * quat[1]) + (float)(quat[2] * quat[2])) * 2.0);
  forward[1] = (float)((float)(*quat * quat[1]) + (float)(quat[2] * quat[3])) * 2.0;
  forward[2] = (float)((float)(*quat * quat[2]) - (float)(quat[1] * quat[3])) * 2.0;
}

void __cdecl QuatSlerp(const float *from, const float *to, float frac, float *result)
{
  double v4; // xmm0_8
  double v5; // xmm0_8
  float v6; // xmm1_4
  long double theta; // [esp+0h] [ebp-18h]
  long double thetaa; // [esp+0h] [ebp-18h]
  long double thetab; // [esp+0h] [ebp-18h]
  long double thetac; // [esp+0h] [ebp-18h]
  float sinTheta; // [esp+4h] [ebp-14h]
  float scaleTo; // [esp+8h] [ebp-10h]
  float scaleFrom; // [esp+Ch] [ebp-Ch]
  float dot; // [esp+10h] [ebp-8h]
  bool dotWasNeg; // [esp+17h] [ebp-1h]

  dot = (float)((float)((float)(*from * *to) + (float)(from[1] * to[1])) + (float)(from[2] * to[2]))
      + (float)(from[3] * to[3]);
  if ( dot >= 0.0 )
  {
    dotWasNeg = 0;
  }
  else
  {
    dotWasNeg = 1;
    dot = dot * -1.0;
  }
  if ( dot <= 0.94999999 )
  {
    __libm_sse2_acos(theta);
    *(float *)&thetaa = dot;
    __libm_sse2_sin(thetaa);
    *((float *)&thetab + 1) = dot;
    v4 = (float)((float)(1.0 - frac) * *(float *)&thetab);
    __libm_sse2_sin(thetab);
    *(float *)&v4 = v4;
    scaleFrom = *(float *)&v4 / *((float *)&thetac + 1);
    v5 = (float)(frac * *(float *)&thetac);
    __libm_sse2_sin(thetac);
    *(float *)&v5 = v5;
    scaleTo = *(float *)&v5 / sinTheta;
  }
  else
  {
    scaleFrom = 1.0 - frac;
    scaleTo = frac;
  }
  if ( dotWasNeg )
  {
    *result = (float)(scaleFrom * *from) + (float)((float)(scaleTo * *to) * -1.0);
    result[1] = (float)(scaleFrom * from[1]) + (float)((float)(scaleTo * to[1]) * -1.0);
    result[2] = (float)(scaleFrom * from[2]) + (float)((float)(scaleTo * to[2]) * -1.0);
    v6 = (float)(scaleTo * to[3]) * -1.0;
  }
  else
  {
    *result = (float)(scaleFrom * *from) + (float)(scaleTo * *to);
    result[1] = (float)(scaleFrom * from[1]) + (float)(scaleTo * to[1]);
    result[2] = (float)(scaleFrom * from[2]) + (float)(scaleTo * to[2]);
    v6 = scaleTo * to[3];
  }
  result[3] = (float)(scaleFrom * from[3]) + v6;
}

double __cdecl RotationToYaw(const float *rot)
{
  double v1; // xmm0_8
  long double var10; // [esp+0h] [ebp-10h]
  float r; // [esp+4h] [ebp-Ch]
  long double yaw; // [esp+8h] [ebp-8h]

  *((float *)&yaw + 1) = *rot * *rot;
  r = (float)(rot[1] * rot[1]) + *((float *)&yaw + 1);
  if ( r == 0.0 && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 1960, 0, "%s", "r") )
    __debugbreak();
  *((float *)&var10 + 1) = 2.0 / r;
  *(float *)&var10 = (float)(*rot * rot[1]) * *((float *)&var10 + 1);
  v1 = *(float *)&var10;
  __libm_sse2_atan2(var10, yaw);
  *(float *)&v1 = v1;
  return (float)(*(float *)&v1 * 57.295776);
}

void __cdecl MatrixRotationZ(float (*mat)[3], float degree)
{
  long double v2; // st7
  float c; // [esp+Ch] [ebp-8h]
  float s; // [esp+10h] [ebp-4h]

  v2 = (float)(degree * 0.017453292);
  c = cos(v2);
  s = sin(v2);
  (*mat)[0] = c;
  LODWORD((*mat)[1]) = LODWORD(s) ^ _mask__NegFloat_;
  (*mat)[2] = 0.0f;
  (*mat)[3] = s;
  (*mat)[4] = c;
  (*mat)[5] = 0.0f;
  (*mat)[6] = 0.0f;
  (*mat)[7] = 0.0f;
  (*mat)[8] = 1.0f;
}

void __cdecl FinitePerspectiveMatrix(float tanHalfFovX, float tanHalfFovY, float zNear, float zFar, float (*mtx)[4])
{
  if ( !mtx && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 2037, 0, "%s", "mtx") )
    __debugbreak();
  if ( zNear <= 0.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
          2038,
          0,
          "zNear > 0.0f\n\t%g, %g",
          zNear,
          0.0) )
  {
    __debugbreak();
  }
  if ( zFar <= zNear
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
          2039,
          0,
          "zFar > zNear\n\t%g, %g",
          zFar,
          zNear) )
  {
    __debugbreak();
  }
  memset((unsigned __int8 *)mtx, 0, 0x40u);
  (*mtx)[0] = 1.0 / tanHalfFovX;
  (*mtx)[5] = 1.0 / tanHalfFovY;
  (*mtx)[10] = COERCE_FLOAT(LODWORD(zFar) ^ _mask__NegFloat_) / (float)(zNear - zFar);
  (*mtx)[11] = 1.0f;
  (*mtx)[14] = (float)(zNear * zFar) / (float)(zNear - zFar);
}

// local variable allocation has failed, the output may be wrong!
void  SpotLightViewMatrix(const float *direction, float rotation, float (*mtx)[4])
{
  long double v4; // [esp-14h] [ebp-CCh]
  long double v5; // [esp-14h] [ebp-CCh]
  _BYTE v6[128]; // [esp-4h] [ebp-BCh] OVERLAPPED BYREF
  float v7; // [esp+88h] [ebp-30h] BYREF
  int v8; // [esp+8Ch] [ebp-2Ch]
  int v9; // [esp+90h] [ebp-28h]
  float axis[3][3]; // [esp+94h] [ebp-24h] BYREF
  unsigned int retaddr; // [esp+B8h] [ebp+0h]

  *(_QWORD *)&axis[2][0] = __PAIR64__(retaddr, a1);
  LODWORD(v7) = *(unsigned int *)direction ^ _mask__NegFloat_;
  v8 = *((unsigned int *)direction + 1) ^ _mask__NegFloat_;
  v9 = *((unsigned int *)direction + 2) ^ _mask__NegFloat_;
  PerpendicularVector(&v7, axis[1]);
  Vec3Cross(axis[1], &v7, axis[0]);
  *(unsigned int *)&v6[64] = LODWORD(axis[0][0]) ^ _mask__NegFloat_;
  *(unsigned int *)&v6[80] = LODWORD(axis[0][1]) ^ _mask__NegFloat_;
  *(unsigned int *)&v6[96] = LODWORD(axis[0][2]) ^ _mask__NegFloat_;
  *(float *)&v6[84] = axis[1][1];
  *(float *)&v6[68] = axis[1][0];
  *(float *)&v6[100] = axis[1][2];
  *(float *)&v6[72] = v7;
  *(unsigned int *)&v6[88] = v8;
  *(unsigned int *)&v6[104] = v9;
  *(unsigned int *)&v6[76] = 0;
  *(unsigned int *)&v6[92] = 0;
  memset(&v6[108], 0, 16);
  *(float *)&v6[124] = 1.0f;
  memset(v6, 0, 0x40u);
  __libm_sse2_sin(v4);
  __libm_sse2_cos(v5);
  *(float *)v6 = rotation;
  *(unsigned int *)&v6[4] = LODWORD(rotation) ^ _mask__NegFloat_;
  *(float *)&v6[16] = rotation;
  *(float *)&v6[20] = rotation;
  *(float *)&v6[40] = 1.0f;
  *(float *)&v6[60] = 1.0f;
  MatrixMultiply44((const float (*)[4])&v6[64], (const float (*)[4])v6, mtx);
}

// local variable allocation has failed, the output may be wrong!
void  SpotLightViewMatrixDir3(
        const float *dirx,
        const float *diry,
        const float *dirz,
        float (*mtx)[4])
{
  double v5; // xmm0_8
  long double v6; // [esp-14h] [ebp-CCh]
  long double v7; // [esp-14h] [ebp-CCh]
  _BYTE v8[140]; // [esp-4h] [ebp-BCh] OVERLAPPED BYREF
  int v9; // [esp+88h] [ebp-30h]
  int v10; // [esp+8Ch] [ebp-2Ch]
  float axis[3][3]; // [esp+90h] [ebp-28h] BYREF
  float retaddr; // [esp+B8h] [ebp+0h]

  axis[2][2] = retaddr;
  *(_QWORD *)&axis[2][0] = __PAIR64__(a1, LODWORD(FLOAT_3_1415927));
  *(float *)&v8[136] = *dirx;
  v9 = *((unsigned int *)dirx + 1);
  v10 = *((unsigned int *)dirx + 2);
  *(unsigned int *)&v8[132] = axis;
  axis[0][0] = *diry;
  axis[0][1] = diry[1];
  axis[0][2] = diry[2];
  *(unsigned int *)&v8[128] = axis[1];
  axis[1][0] = *dirz;
  axis[1][1] = dirz[1];
  axis[1][2] = dirz[2];
  *(float *)&v8[80] = axis[0][1];
  *(float *)&v8[64] = axis[0][0];
  *(float *)&v8[96] = axis[0][2];
  *(float *)&v8[84] = axis[1][1];
  *(float *)&v8[68] = axis[1][0];
  *(float *)&v8[100] = axis[1][2];
  *(unsigned int *)&v8[72] = *(unsigned int *)&v8[136];
  *(unsigned int *)&v8[88] = v9;
  *(unsigned int *)&v8[104] = v10;
  *(unsigned int *)&v8[76] = 0;
  *(unsigned int *)&v8[92] = 0;
  memset(&v8[108], 0, 16);
  *(float *)&v8[124] = 1.0f;
  memset(v8, 0, 0x40u);
  __libm_sse2_sin(v6);
  v5 = axis[2][0];
  __libm_sse2_cos(v7);
  *(float *)&v5 = v5;
  *(unsigned int *)v8 = LODWORD(v5);
  *(unsigned int *)&v8[4] = COERCE_UNSIGNED_INT(3.1415927) ^ _mask__NegFloat_;
  *(float *)&v8[16] = 3.1415927;
  *(unsigned int *)&v8[20] = LODWORD(v5);
  *(float *)&v8[40] = 1.0f;
  *(float *)&v8[60] = 1.0f;
  MatrixMultiply44((const float (*)[4])&v8[64], (const float (*)[4])v8, mtx);
}

void __cdecl SpotLightProjectionMatrix(float cosFov, float zNear, float zFar, float (*mtx)[4])
{
  float v4; // [esp+0h] [ebp-10h]
  float Q; // [esp+8h] [ebp-8h]
  float cotanFov; // [esp+Ch] [ebp-4h]

  memset((unsigned __int8 *)mtx, 0, 0x40u);
  if ( zNear >= 0.001 )
    v4 = zNear;
  else
    v4 = 0.001f;
  Q = zFar / (float)(zFar - v4);
  cotanFov = 1.0 / (float)(fsqrt(1.0 - (float)(cosFov * cosFov)) / cosFov);
  (*mtx)[0] = cotanFov;
  (*mtx)[5] = cotanFov;
  (*mtx)[10] = Q;
  (*mtx)[11] = 1.0f;
  (*mtx)[14] = COERCE_FLOAT(LODWORD(Q) ^ _mask__NegFloat_) * v4;
}

void __cdecl InfinitePerspectiveMatrix(float tanHalfFovX, float tanHalfFovY, float zNear, float (*mtx)[4])
{
  if ( !mtx && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 2304, 0, "%s", "mtx") )
    __debugbreak();
  if ( zNear <= 0.0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 2305, 0, "%s", "zNear > 0") )
  {
    __debugbreak();
  }
  memset((unsigned __int8 *)mtx, 0, 0x40u);
  (*mtx)[0] = 0.99951172 / tanHalfFovX;
  (*mtx)[5] = 0.99951172 / tanHalfFovY;
  (*mtx)[10] = FLOAT_0_99951172;
  (*mtx)[11] = 1.0f;
  (*mtx)[14] = COERCE_FLOAT(LODWORD(zNear) ^ _mask__NegFloat_) * 0.99951172;
}

void __cdecl MatrixForViewer(const float *origin, const float (*axis)[3], float (*mtx)[4])
{
  if ( !mtx && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 2353, 0, "%s", "mtx") )
    __debugbreak();
  if ( !origin
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 2354, 0, "%s", "origin") )
  {
    __debugbreak();
  }
  if ( !axis && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 2355, 0, "%s", "axis") )
    __debugbreak();
  LODWORD((*mtx)[0]) = LODWORD((*axis)[3]) ^ _mask__NegFloat_;
  LODWORD((*mtx)[4]) = LODWORD((*axis)[4]) ^ _mask__NegFloat_;
  LODWORD((*mtx)[8]) = LODWORD((*axis)[5]) ^ _mask__NegFloat_;
  LODWORD((*mtx)[12]) = COERCE_UNSIGNED_INT(
                          (float)((float)(*origin * (*mtx)[0]) + (float)(origin[1] * (*mtx)[4]))
                        + (float)(origin[2] * (*mtx)[8]))
                      ^ _mask__NegFloat_;
  (*mtx)[1] = (*axis)[6];
  (*mtx)[5] = (*axis)[7];
  (*mtx)[9] = (*axis)[8];
  LODWORD((*mtx)[13]) = COERCE_UNSIGNED_INT(
                          (float)((float)(*origin * (*mtx)[1]) + (float)(origin[1] * (*mtx)[5]))
                        + (float)(origin[2] * (*mtx)[9]))
                      ^ _mask__NegFloat_;
  (*mtx)[2] = (*axis)[0];
  (*mtx)[6] = (*axis)[1];
  (*mtx)[10] = (*axis)[2];
  LODWORD((*mtx)[14]) = COERCE_UNSIGNED_INT(
                          (float)((float)(*origin * (*mtx)[2]) + (float)(origin[1] * (*mtx)[6]))
                        + (float)(origin[2] * (*mtx)[10]))
                      ^ _mask__NegFloat_;
  (*mtx)[3] = 0.0f;
  (*mtx)[7] = 0.0f;
  (*mtx)[11] = 0.0f;
  (*mtx)[15] = 1.0f;
}

void __cdecl AnglesSubtract(const float *v1, const float *v2, float *v3)
{
  *v3 = AngleNormalize180(*v1 - *v2);
  v3[1] = AngleNormalize180(v1[1] - v2[1]);
  v3[2] = AngleNormalize180(v1[2] - v2[2]);
}

double __cdecl AngleNormalize360(float angle)
{
  float v3; // [esp+Ch] [ebp-10h]
  float result; // [esp+14h] [ebp-8h]

  v3 = floor((float)(angle * 0.0027777778));
  result = ((float)(angle * 0.0027777778) - v3) * 360.0;
  if ( (float)(result - 360.0) < 0.0 )
    return (float)(((float)(angle * 0.0027777778) - v3) * 360.0);
  else
    return (float)(result - 360.0);
}

double __cdecl AngleDelta(float angle1, float angle2)
{
  return AngleNormalize180(angle1 - angle2);
}

double __cdecl RadiusFromBounds(const float *mins, const float *maxs)
{
  float v4; // [esp+4h] [ebp-4h]

  v4 = RadiusFromBoundsSq(mins, maxs);
  return (float)sqrt(v4);
}

double __cdecl RadiusFromBounds2D(const float *mins, const float *maxs)
{
  float v4; // [esp+4h] [ebp-4h]

  v4 = RadiusFromBounds2DSq(mins, maxs);
  return (float)sqrt(v4);
}

double __cdecl RadiusFromBoundsSq(const float *mins, const float *maxs)
{
  float v3; // [esp+0h] [ebp-24h]
  int i; // [esp+10h] [ebp-14h]
  float corner[3]; // [esp+14h] [ebp-10h]
  float a; // [esp+20h] [ebp-4h]

  for ( i = 0; i < 3; ++i )
  {
    LODWORD(a) = LODWORD(mins[i]) & _mask__AbsFloat_;
    if ( a <= COERCE_FLOAT(LODWORD(maxs[i]) & _mask__AbsFloat_) )
      LODWORD(v3) = LODWORD(maxs[i]) & _mask__AbsFloat_;
    else
      v3 = a;
    corner[i] = v3;
  }
  return corner[2] * corner[2] + corner[1] * corner[1] + corner[0] * corner[0];
}

double __cdecl RadiusFromBounds2DSq(const float *mins, const float *maxs)
{
  float v3; // [esp+0h] [ebp-20h]
  int i; // [esp+10h] [ebp-10h]
  float corner[2]; // [esp+14h] [ebp-Ch]
  float a; // [esp+1Ch] [ebp-4h]

  for ( i = 0; i < 2; ++i )
  {
    LODWORD(a) = LODWORD(mins[i]) & _mask__AbsFloat_;
    if ( a <= COERCE_FLOAT(LODWORD(maxs[i]) & _mask__AbsFloat_) )
      LODWORD(v3) = LODWORD(maxs[i]) & _mask__AbsFloat_;
    else
      v3 = a;
    corner[i] = v3;
  }
  return corner[1] * corner[1] + corner[0] * corner[0];
}

void __cdecl ExtendBounds(float *mins, float *maxs, const float *offset)
{
  if ( *offset <= 0.0 )
    *mins = *mins + *offset;
  else
    *maxs = *maxs + *offset;
  if ( offset[1] <= 0.0 )
    mins[1] = mins[1] + offset[1];
  else
    maxs[1] = maxs[1] + offset[1];
  if ( offset[2] <= 0.0 )
    mins[2] = mins[2] + offset[2];
  else
    maxs[2] = maxs[2] + offset[2];
}

void __cdecl ExpandBoundsToWidth(float *mins, float *maxs)
{
  float v2; // [esp+0h] [ebp-18h]
  float diff; // [esp+4h] [ebp-14h]
  float size_8; // [esp+14h] [ebp-4h]

  if ( *maxs < *mins
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
          2489,
          0,
          "%s",
          "maxs[0] >= mins[0]") )
  {
    __debugbreak();
  }
  if ( maxs[1] < mins[1]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
          2490,
          0,
          "%s",
          "maxs[1] >= mins[1]") )
  {
    __debugbreak();
  }
  if ( maxs[2] < mins[2]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
          2491,
          0,
          "%s",
          "maxs[2] >= mins[2]") )
  {
    __debugbreak();
  }
  size_8 = maxs[2] - mins[2];
  if ( (float)((float)(*maxs - *mins) - (float)(maxs[1] - mins[1])) < 0.0 )
    v2 = maxs[1] - mins[1];
  else
    v2 = *maxs - *mins;
  if ( v2 > size_8 )
  {
    diff = (float)(v2 - size_8) * 0.5;
    mins[2] = mins[2] - diff;
    maxs[2] = maxs[2] + diff;
  }
}

void __cdecl ClearBounds(float *mins, float *maxs)
{
  *mins = 131072.0f;
  mins[1] = 131072.0f;
  mins[2] = 131072.0f;
  *maxs = FLOAT_N131072_0;
  maxs[1] = FLOAT_N131072_0;
  maxs[2] = FLOAT_N131072_0;
}

bool __cdecl IsClearedBounds(const float *mins, const float *maxs)
{
  if ( *mins != 131072.0 || mins[1] != 131072.0 || mins[2] != 131072.0 )
    return 0;
  return *maxs == -131072.0 && maxs[1] == -131072.0 && maxs[2] == -131072.0;
}

void __cdecl ClearBounds2D(float *mins, float *maxs)
{
  *mins = 131072.0f;
  mins[1] = 131072.0f;
  *maxs = FLOAT_N131072_0;
  maxs[1] = FLOAT_N131072_0;
}

void __cdecl AddPointToBounds(const float *v, float *mins, float *maxs)
{
  if ( *mins > *v )
    *mins = *v;
  if ( *v > *maxs )
    *maxs = *v;
  if ( mins[1] > v[1] )
    mins[1] = v[1];
  if ( v[1] > maxs[1] )
    maxs[1] = v[1];
  if ( mins[2] > v[2] )
    mins[2] = v[2];
  if ( v[2] > maxs[2] )
    maxs[2] = v[2];
}

void __cdecl AddPointToBounds2D(const float *v, float *mins, float *maxs)
{
  if ( *mins > *v )
    *mins = *v;
  if ( *v > *maxs )
    *maxs = *v;
  if ( mins[1] > v[1] )
    mins[1] = v[1];
  if ( v[1] > maxs[1] )
    maxs[1] = v[1];
}

bool __cdecl PointInBounds(const float *v, const float *mins, const float *maxs)
{
  if ( !v && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 2593, 0, "%s", "v") )
    __debugbreak();
  if ( !mins && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 2594, 0, "%s", "mins") )
    __debugbreak();
  if ( !maxs && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 2595, 0, "%s", "maxs") )
    __debugbreak();
  if ( *mins > *v || *v > *maxs )
    return 0;
  if ( mins[1] > v[1] || v[1] > maxs[1] )
    return 0;
  return mins[2] <= v[2] && v[2] <= maxs[2];
}

bool __cdecl PointInBounds2D(const float *v, const float *mins, const float *maxs)
{
  if ( !v && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 2609, 0, "%s", "v") )
    __debugbreak();
  if ( !mins && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 2610, 0, "%s", "mins") )
    __debugbreak();
  if ( !maxs && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 2611, 0, "%s", "maxs") )
    __debugbreak();
  if ( *mins > *v || *v > *maxs )
    return 0;
  return mins[1] <= v[1] && v[1] <= maxs[1];
}

bool __cdecl BoundsOverlap(const float *mins0, const float *maxs0, const float *mins1, const float *maxs1)
{
  if ( *mins0 > *maxs1 || *mins1 > *maxs0 )
    return 0;
  if ( mins0[1] > maxs1[1] || mins1[1] > maxs0[1] )
    return 0;
  return mins0[2] <= maxs1[2] && mins1[2] <= maxs0[2];
}

void __cdecl ExpandBounds(const float *addedmins, const float *addedmaxs, float *mins, float *maxs)
{
  if ( *mins > *addedmins )
    *mins = *addedmins;
  if ( *addedmaxs > *maxs )
    *maxs = *addedmaxs;
  if ( mins[1] > addedmins[1] )
    mins[1] = addedmins[1];
  if ( addedmaxs[1] > maxs[1] )
    maxs[1] = addedmaxs[1];
  if ( mins[2] > addedmins[2] )
    mins[2] = addedmins[2];
  if ( addedmaxs[2] > maxs[2] )
    maxs[2] = addedmaxs[2];
}

void __cdecl ExpandBounds2D(const float *addedmins, const float *addedmaxs, float *mins, float *maxs)
{
  if ( *mins > *addedmins )
    *mins = *addedmins;
  if ( *addedmaxs > *maxs )
    *maxs = *addedmaxs;
  if ( mins[1] > addedmins[1] )
    mins[1] = addedmins[1];
  if ( addedmaxs[1] > maxs[1] )
    maxs[1] = addedmaxs[1];
}

void __cdecl GetRotatedBounds(
        const float (*baseBounds)[3],
        const float *origin,
        const float (*axis)[3],
        float (*rotatedBounds)[3])
{
  int axisIndex; // [esp+10h] [ebp-8h]
  int offset; // [esp+14h] [ebp-4h]
  int offseta; // [esp+14h] [ebp-4h]
  int offsetb; // [esp+14h] [ebp-4h]

  for ( axisIndex = 0; axisIndex < 3; ++axisIndex )
  {
    (*rotatedBounds)[axisIndex] = origin[axisIndex];
    (*rotatedBounds)[axisIndex + 3] = origin[axisIndex];
    offset = (*axis)[axisIndex] >= 0.0 ? 0 : 0xC;
    (*rotatedBounds)[axisIndex] = (float)(*(float *)((char *)*baseBounds + offset) * (float)(*axis)[axisIndex])
                                + (*rotatedBounds)[axisIndex];
    (*rotatedBounds)[axisIndex + 3] = (float)(*(float *)((char *)&(*baseBounds)[3] - offset) * (float)(*axis)[axisIndex])
                                    + (*rotatedBounds)[axisIndex + 3];
    offseta = (*axis)[axisIndex + 3] >= 0.0 ? 0 : 0xC;
    (*rotatedBounds)[axisIndex] = (float)(*(float *)((char *)&(*baseBounds)[1] + offseta) * (float)(*axis)[axisIndex + 3])
                                + (*rotatedBounds)[axisIndex];
    (*rotatedBounds)[axisIndex + 3] = (float)(*(float *)((char *)&(*baseBounds)[4] - offseta)
                                            * (float)(*axis)[axisIndex + 3])
                                    + (*rotatedBounds)[axisIndex + 3];
    offsetb = (*axis)[axisIndex + 6] >= 0.0 ? 0 : 0xC;
    (*rotatedBounds)[axisIndex] = (float)(*(float *)((char *)&(*baseBounds)[2] + offsetb) * (float)(*axis)[axisIndex + 6])
                                + (*rotatedBounds)[axisIndex];
    (*rotatedBounds)[axisIndex + 3] = (float)(*(float *)((char *)&(*baseBounds)[5] - offsetb)
                                            * (float)(*axis)[axisIndex + 6])
                                    + (*rotatedBounds)[axisIndex + 3];
  }
}

void __cdecl AxisClear(float (*axis)[3])
{
  (*axis)[0] = 1.0f;
  (*axis)[1] = 0.0f;
  (*axis)[2] = 0.0f;
  (*axis)[3] = 0.0f;
  (*axis)[4] = 1.0f;
  (*axis)[5] = 0.0f;
  (*axis)[6] = 0.0f;
  (*axis)[7] = 0.0f;
  (*axis)[8] = 1.0f;
}

void __cdecl AxisTranspose(const float (*in)[3], float (*out)[3])
{
  if ( in == out
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 2750, 0, "%s", "in != out") )
  {
    __debugbreak();
  }
  (*out)[0] = (*in)[0];
  (*out)[1] = (*in)[3];
  (*out)[2] = (*in)[6];
  (*out)[3] = (*in)[1];
  (*out)[4] = (*in)[4];
  (*out)[5] = (*in)[7];
  (*out)[6] = (*in)[2];
  (*out)[7] = (*in)[5];
  (*out)[8] = (*in)[8];
}

void __cdecl AxisTransformVec3(const float (*axes)[3], const float *vec, float *out)
{
  *out = (float)((float)(*vec * (*axes)[0]) + (float)(vec[1] * (float)(*axes)[3])) + (float)(vec[2] * (float)(*axes)[6]);
  out[1] = (float)((float)(*vec * (float)(*axes)[1]) + (float)(vec[1] * (float)(*axes)[4]))
         + (float)(vec[2] * (float)(*axes)[7]);
  out[2] = (float)((float)(*vec * (float)(*axes)[2]) + (float)(vec[1] * (float)(*axes)[5]))
         + (float)(vec[2] * (float)(*axes)[8]);
}

void __cdecl YawToAxis(float yaw, float (*axis)[3])
{
  float right[3]; // [esp+10h] [ebp-Ch] BYREF

  YawVectors(yaw, (float *)axis, right);
  (*axis)[6] = 0.0f;
  (*axis)[7] = 0.0f;
  (*axis)[8] = 1.0f;
  (*axis)[3] = 0.0 - right[0];
  (*axis)[4] = 0.0 - right[1];
  (*axis)[5] = 0.0 - right[2];
}

void __cdecl AxisToAngles(const float (*axis)[3], float *angles)
{
  float v2; // [esp+0h] [ebp-38h]
  float rad; // [esp+18h] [ebp-20h]
  float rada; // [esp+18h] [ebp-20h]
  float right[3]; // [esp+1Ch] [ebp-1Ch] BYREF
  float temp; // [esp+28h] [ebp-10h]
  float pitch; // [esp+2Ch] [ebp-Ch]
  float fCos; // [esp+30h] [ebp-8h]
  float fSin; // [esp+34h] [ebp-4h]

  vectoangles((const float *)axis, angles);
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
      v2 = -180.0f;
    else
      v2 = 180.0f;
    angles[2] = pitch + v2;
  }
}

void __cdecl Axis4ToAngles(const float (*axis)[4], float *angles)
{
  float v2; // [esp+0h] [ebp-3Ch]
  float rad; // [esp+18h] [ebp-24h]
  float rada; // [esp+18h] [ebp-24h]
  float right[4]; // [esp+1Ch] [ebp-20h] BYREF
  float temp; // [esp+2Ch] [ebp-10h]
  float pitch; // [esp+30h] [ebp-Ch]
  float fCos; // [esp+34h] [ebp-8h]
  float fSin; // [esp+38h] [ebp-4h]

  vectoangles((const float *)axis, angles);
  right[0] = (*axis)[4];
  right[1] = (*axis)[5];
  right[2] = (*axis)[6];
  right[3] = (*axis)[7];
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
      v2 = -180.0f;
    else
      v2 = 180.0f;
    angles[2] = pitch + v2;
  }
}

int __cdecl IntersectPlanes(const float **plane, float *xyz)
{
  float v3; // xmm0_4
  float v4; // xmm0_4
  float v5; // xmm0_4
  double determinant; // [esp+8h] [ebp-20h]

  determinant = (float)((float)((float)(plane[1][1] * plane[2][2]) - (float)(plane[2][1] * plane[1][2])) * **plane)
              + (float)((float)((float)(plane[2][1] * (*plane)[2]) - (float)((*plane)[1] * plane[2][2])) * *plane[1])
              + (float)((float)((float)((*plane)[1] * plane[1][2]) - (float)(plane[1][1] * (*plane)[2])) * *plane[2]);
  if ( COERCE_DOUBLE(*(_QWORD *)&determinant & _mask__AbsDouble_) < 0.001000000047497451 )
    return 0;
  v3 = ((float)((float)((float)(plane[1][1] * plane[2][2]) - (float)(plane[2][1] * plane[1][2])) * (*plane)[3])
      + (float)((float)((float)(plane[2][1] * (*plane)[2]) - (float)((*plane)[1] * plane[2][2])) * plane[1][3])
      + (float)((float)((float)((*plane)[1] * plane[1][2]) - (float)(plane[1][1] * (*plane)[2])) * plane[2][3]))
     * (1.0
      / determinant);
  *xyz = v3;
  v4 = ((float)((float)((float)(plane[1][2] * *plane[2]) - (float)(plane[2][2] * *plane[1])) * (*plane)[3])
      + (float)((float)((float)(plane[2][2] * **plane) - (float)((*plane)[2] * *plane[2])) * plane[1][3])
      + (float)((float)((float)((*plane)[2] * *plane[1]) - (float)(plane[1][2] * **plane)) * plane[2][3]))
     * (1.0
      / determinant);
  xyz[1] = v4;
  v5 = ((float)((float)((float)(*plane[1] * plane[2][1]) - (float)(*plane[2] * plane[1][1])) * (*plane)[3])
      + (float)((float)((float)(*plane[2] * (*plane)[1]) - (float)(**plane * plane[2][1])) * plane[1][3])
      + (float)((float)((float)(**plane * plane[1][1]) - (float)(*plane[1] * (*plane)[1])) * plane[2][3]))
     * (1.0
      / determinant);
  xyz[2] = v5;
  return 1;
}

void __cdecl SnapPointToIntersectingPlanes(const float **planes, float *xyz, float snapGrid, float snapEpsilon)
{
  float snapped[3]; // [esp+34h] [ebp-28h]
  float baseError; // [esp+40h] [ebp-1Ch]
  float maxBaseError; // [esp+44h] [ebp-18h]
  float snapError; // [esp+48h] [ebp-14h]
  float maxSnapError; // [esp+4Ch] [ebp-10h]
  float rounded; // [esp+50h] [ebp-Ch]
  int axis; // [esp+54h] [ebp-8h]
  int planeIndex; // [esp+58h] [ebp-4h]

  for ( axis = 0; axis < 3; ++axis )
  {
    rounded = (float)(int)((float)(xyz[axis] / snapGrid) + 9.313225746154785e-10) * snapGrid;
    if ( snapEpsilon <= COERCE_FLOAT(COERCE_UNSIGNED_INT(rounded - xyz[axis]) & _mask__AbsFloat_) )
      snapped[axis] = xyz[axis];
    else
      snapped[axis] = rounded;
  }
  if ( snapped[0] != *xyz || snapped[1] != xyz[1] || snapped[2] != xyz[2] )
  {
    maxSnapError = 0.0f;
    maxBaseError = snapEpsilon;
    for ( planeIndex = 0; planeIndex < 3; ++planeIndex )
    {
      LODWORD(snapError) = COERCE_UNSIGNED_INT(
                             (float)((float)((float)(*planes[planeIndex] * snapped[0])
                                           + (float)(planes[planeIndex][1] * snapped[1]))
                                   + (float)(planes[planeIndex][2] * snapped[2]))
                           - planes[planeIndex][3])
                         & _mask__AbsFloat_;
      if ( snapError > maxSnapError )
        maxSnapError = snapError;
      LODWORD(baseError) = COERCE_UNSIGNED_INT(
                             (float)((float)((float)(*planes[planeIndex] * *xyz)
                                           + (float)(planes[planeIndex][1] * xyz[1]))
                                   + (float)(planes[planeIndex][2] * xyz[2]))
                           - planes[planeIndex][3])
                         & _mask__AbsFloat_;
      if ( baseError > maxBaseError )
        maxBaseError = baseError;
    }
    if ( maxBaseError > maxSnapError )
    {
      *xyz = snapped[0];
      xyz[1] = snapped[1];
      xyz[2] = snapped[2];
    }
  }
}

int __cdecl ProjectedWindingContainsCoplanarPoint(
        const float (*verts)[3],
        int vertCount,
        int x,
        int y,
        const float *point)
{
  int vertIndex; // [esp+10h] [ebp-10h]
  int vertIndexPrev; // [esp+14h] [ebp-Ch]

  vertIndexPrev = vertCount - 1;
  for ( vertIndex = 0; vertIndex < vertCount; ++vertIndex )
  {
    if ( (float)((float)((float)(point[x] - (float)(*verts)[3 * vertIndexPrev + x])
                       * (float)((float)(*verts)[3 * vertIndex + y] - (float)(*verts)[3 * vertIndexPrev + y]))
               + (float)((float)(point[y] - (float)(*verts)[3 * vertIndexPrev + y])
                       * (float)((float)(*verts)[3 * vertIndexPrev + x] - (float)(*verts)[3 * vertIndex + x]))) < 0.0 )
      return 0;
    vertIndexPrev = vertIndex;
  }
  return 1;
}

int __cdecl PlaneFromPoints(float *plane, const float *v0, const float *v1, const float *v2)
{
  float v2_v0[3]; // [esp+8h] [ebp-20h] BYREF
  float v1_v0[3]; // [esp+14h] [ebp-14h] BYREF
  float length; // [esp+20h] [ebp-8h]
  float lengthSq; // [esp+24h] [ebp-4h]

  v1_v0[0] = *v1 - *v0;
  v1_v0[1] = v1[1] - v0[1];
  v1_v0[2] = v1[2] - v0[2];
  v2_v0[0] = *v2 - *v0;
  v2_v0[1] = v2[1] - v0[1];
  v2_v0[2] = v2[2] - v0[2];
  Vec3Cross(v2_v0, v1_v0, plane);
  lengthSq = (float)((float)(*plane * *plane) + (float)(plane[1] * plane[1])) + (float)(plane[2] * plane[2]);
  if ( lengthSq >= 2.0 )
    goto LABEL_7;
  if ( lengthSq == 0.0 )
    return 0;
  if ( (float)((float)((float)((float)((float)(v2_v0[0] * v2_v0[0]) + (float)(v2_v0[1] * v2_v0[1]))
                             + (float)(v2_v0[2] * v2_v0[2]))
                     * (float)((float)((float)(v1_v0[0] * v1_v0[0]) + (float)(v1_v0[1] * v1_v0[1]))
                             + (float)(v1_v0[2] * v1_v0[2])))
             * 0.0000010000001) >= lengthSq )
  {
    v1_v0[0] = *v2 - *v1;
    v1_v0[1] = v2[1] - v1[1];
    v1_v0[2] = v2[2] - v1[2];
    v2_v0[0] = *v0 - *v1;
    v2_v0[1] = v0[1] - v1[1];
    v2_v0[2] = v0[2] - v1[2];
    Vec3Cross(v2_v0, v1_v0, plane);
    if ( (float)((float)((float)((float)((float)(v2_v0[0] * v2_v0[0]) + (float)(v2_v0[1] * v2_v0[1]))
                               + (float)(v2_v0[2] * v2_v0[2]))
                       * (float)((float)((float)(v1_v0[0] * v1_v0[0]) + (float)(v1_v0[1] * v1_v0[1]))
                               + (float)(v1_v0[2] * v1_v0[2])))
               * 0.0000010000001) >= lengthSq )
      return 0;
  }
LABEL_7:
  length = fsqrt(lengthSq);
  *plane = *plane / length;
  plane[1] = plane[1] / length;
  plane[2] = plane[2] / length;
  plane[3] = (float)((float)(*v0 * *plane) + (float)(v0[1] * plane[1])) + (float)(v0[2] * plane[2]);
  return 1;
}

void __cdecl ProjectPointOnPlane(const float *p, const float *normal, float *dst)
{
  double v3; // st7
  const char *v4; // eax
  float d; // [esp+28h] [ebp-4h]

  if ( !Vec3IsNormalized(normal) )
  {
    v3 = Abs(normal);
    v4 = va("(%g %g %g) len %g", *normal, normal[1], normal[2], v3);
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
            3138,
            0,
            "%s\n\t%s",
            "Vec3IsNormalized( normal )",
            v4) )
      __debugbreak();
  }
  LODWORD(d) = COERCE_UNSIGNED_INT((float)((float)(*normal * *p) + (float)(normal[1] * p[1])) + (float)(normal[2] * p[2]))
             ^ _mask__NegFloat_;
  *dst = (float)(d * *normal) + *p;
  dst[1] = (float)(d * normal[1]) + p[1];
  dst[2] = (float)(d * normal[2]) + p[2];
}

void __cdecl SetPlaneSignbits(cplane_s *out)
{
  int j; // [esp+0h] [ebp-8h]
  unsigned __int8 bits; // [esp+7h] [ebp-1h]

  bits = 0;
  for ( j = 0; j < 3; ++j )
  {
    if ( out->normal[j] < 0.0 )
      bits |= 1 << j;
  }
  out->signbits = bits;
}

int __cdecl BoxOnPlaneSide(
        const float *emins,
        const float *emaxs,
        const cplane_s *p,
        const cplane_s *pa,
        float a5,
        float a6,
        float a7)
{
  int signbits; // eax

  if ( bops_initialized != 1 )
  {
    bops_initialized = 1;
    Ljmptab[0] = (int)&Lcase0;
    dword_99DBDC8 = (int)&Lcase1;
    dword_99DBDCC = (int)&Lcase2;
    dword_99DBDD0 = (int)&Lcase3;
    dword_99DBDD4 = (int)&Lcase4;
    dword_99DBDD8 = (int)&Lcase5;
    dword_99DBDDC = (int)&Lcase6;
    dword_99DBDE0 = (int)&Lcase7;
  }
  signbits = p->signbits;
  if ( (unsigned __int8)signbits < 8u )
    __asm { jmp     Ljmptab[eax*4] }
  __debugbreak();
  if ( !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
          3450,
          1,
          "BoxOnPlaneSide: invalid signbits for plane") )
    __debugbreak();
  __debugbreak();
  __debugbreak();
  __debugbreak();
  __debugbreak();
  __debugbreak();
  __debugbreak();
  __debugbreak();
  __debugbreak();
  return IsPosInsideArc(emins, *(float *)&emaxs, p->normal, *(float *)&pa, a5, a6, a7);
}

int __cdecl IsPosInsideArc(
        const float *pos,
        float posRadius,
        const float *arcOrigin,
        float arcRadius,
        float arcAngle0,
        float arcAngle1,
        float arcHalfHeight)
{
  float originDelta[3]; // [esp+10h] [ebp-18h] BYREF
  float distDelta; // [esp+1Ch] [ebp-Ch]
  float posYaw; // [esp+20h] [ebp-8h]
  float adjustedDistDelta; // [esp+24h] [ebp-4h]

  if ( !pos && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 3472, 0, "%s", "pos") )
    __debugbreak();
  if ( !arcOrigin
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 3473, 0, "%s", "arcOrigin") )
  {
    __debugbreak();
  }
  originDelta[0] = *pos - *arcOrigin;
  originDelta[1] = pos[1] - arcOrigin[1];
  originDelta[2] = pos[2] - arcOrigin[2];
  distDelta = Vec2Normalize(originDelta);
  adjustedDistDelta = distDelta - posRadius;
  if ( (float)((float)(distDelta - posRadius) * (float)(distDelta - posRadius)) > (float)(arcRadius * arcRadius) )
    return 0;
  if ( (float)(arcOrigin[2] - arcHalfHeight) > pos[2] || pos[2] > (float)(arcOrigin[2] + arcHalfHeight) )
    return 0;
  posYaw = vectoyaw(originDelta);
  posYaw = AngleNormalize360(posYaw);
  if ( arcAngle1 <= arcAngle0 )
  {
    if ( arcAngle1 > posYaw || posYaw > arcAngle0 )
      return 1;
  }
  else if ( arcAngle1 > posYaw && posYaw > arcAngle0 )
  {
    return 1;
  }
  return 0;
}

bool __cdecl BoxDistSqrdExceeds(const float *absmin, const float *absmax, const float *org, float fogOpaqueDistSqrd)
{
  float v5; // [esp+4h] [ebp-2Ch]
  float minsSqrd; // [esp+8h] [ebp-28h]
  float mins[3]; // [esp+Ch] [ebp-24h]
  float total; // [esp+18h] [ebp-18h]
  float maxs[3]; // [esp+1Ch] [ebp-14h]
  int i; // [esp+28h] [ebp-8h]
  float maxsSqrd; // [esp+2Ch] [ebp-4h]

  mins[0] = *absmin - *org;
  mins[1] = absmin[1] - org[1];
  mins[2] = absmin[2] - org[2];
  maxs[0] = *absmax - *org;
  maxs[1] = absmax[1] - org[1];
  maxs[2] = absmax[2] - org[2];
  total = 0.0f;
  for ( i = 0; i < 3; ++i )
  {
    if ( (float)(mins[i] * maxs[i]) > 0.0 )
    {
      minsSqrd = mins[i] * mins[i];
      maxsSqrd = maxs[i] * maxs[i];
      if ( minsSqrd <= maxsSqrd )
        v5 = minsSqrd;
      else
        v5 = maxsSqrd;
      total = total + v5;
    }
  }
  return total > fogOpaqueDistSqrd;
}

double __cdecl Q_rint(float in)
{
  return (float)floor(in + 0.5);
}

double __cdecl ColorNormalize(float *in, float *out)
{
  float max; // [esp+0h] [ebp-8h]

  max = *in;
  if ( in[1] > *in )
    max = in[1];
  if ( in[2] > max )
    max = in[2];
  if ( max == 0.0 )
  {
    out[2] = 1.0f;
    out[1] = 1.0f;
    *out = 1.0f;
    return 0.0;
  }
  else
  {
    *out = (float)(1.0 / max) * *in;
    out[1] = (float)(1.0 / max) * in[1];
    out[2] = (float)(1.0 / max) * in[2];
    return max;
  }
}

void __cdecl ColorSRGBtoLinear(const float *in, float *out)
{
  double v2; // xmm0_8
  double v3; // xmm0_8
  double v4; // xmm0_8
  long double v5; // [esp+0h] [ebp-18h]
  float v6; // [esp+0h] [ebp-18h]
  long double v7; // [esp+8h] [ebp-10h]

  if ( *in <= 0.040449999 )
  {
    *(float *)&v7 = *in / 12.92;
  }
  else
  {
    v2 = (float)((float)(*in + 0.055) / 1.0549999);
    __libm_sse2_pow(v5, v7);
    *(float *)&v2 = v2;
    LODWORD(v7) = LODWORD(v2);
  }
  *out = *(float *)&v7;
  if ( in[1] <= 0.040449999 )
  {
    *((float *)&v5 + 1) = in[1] / 12.92;
  }
  else
  {
    v3 = (float)((float)(in[1] + 0.055) / 1.0549999);
    __libm_sse2_pow(v5, v7);
    *(float *)&v3 = v3;
    HIDWORD(v5) = LODWORD(v3);
  }
  out[1] = *((float *)&v5 + 1);
  if ( in[2] <= 0.040449999 )
  {
    v6 = in[2] / 12.92;
  }
  else
  {
    *((float *)&v7 + 1) = (float)(in[2] + 0.055) / 1.0549999;
    v4 = *((float *)&v7 + 1);
    __libm_sse2_pow(v5, v7);
    *(float *)&v4 = v4;
    v6 = *(float *)&v4;
  }
  out[2] = v6;
}

double __cdecl PitchForYawOnNormal(float fYaw, const float *normal)
{
  float v3; // [esp+Ch] [ebp-14h]
  float forward[3]; // [esp+14h] [ebp-Ch] BYREF

  if ( *normal == 0.0
    && normal[1] == 0.0
    && normal[2] == 0.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
          3675,
          0,
          "%s",
          "normal[0] || normal[1] || normal[2]") )
  {
    __debugbreak();
  }
  YawVectors(fYaw, forward, 0);
  if ( normal[2] == 0.0 )
    return 270.0;
  v3 = atan((float)((float)((float)(*normal * forward[0]) + (float)(normal[1] * forward[1])) / normal[2]));
  return v3 * 180.0 / 3.1415927;
}

void __cdecl NearestPitchAndYawOnPlane(const float *angles, const float *normal, float *result)
{
  float projected[3]; // [esp+0h] [ebp-18h] BYREF
  float forward[3]; // [esp+Ch] [ebp-Ch] BYREF

  if ( *normal == 0.0
    && normal[1] == 0.0
    && normal[2] == 0.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
          3705,
          0,
          "%s",
          "normal[0] || normal[1] || normal[2]") )
  {
    __debugbreak();
  }
  AngleVectors(angles, forward, 0, 0);
  ProjectPointOnPlane(forward, normal, projected);
  vectoangles(projected, result);
}

void __cdecl Rand_Init(int seed)
{
  holdrand_0 = seed;
}

double __cdecl flrand(float min, float max)
{
  float result; // [esp+8h] [ebp-4h]

  holdrand_0 = 214013 * holdrand_0 + 2531011;
  result = (float)(holdrand_0 >> 17);
  return (float)((float)((float)((float)(max - min) * result) / 32768.0) + min);
}

int __cdecl irand(int min, int max)
{
  holdrand_0 = 214013 * holdrand_0 + 2531011;
  return (((holdrand_0 >> 17) * (__int64)(max - min)) >> 15) + min;
}

void __cdecl AxisToQuat(const float (*mat)[3], float *out)
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
                "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
                3801,
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
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp", 3806, 0, "%s", "testSizeSq") )
  {
    __debugbreak();
  }
  invLength = 1.0 / fsqrt(testSizeSq);
  v2 = test[best];
  *out = invLength * *v2;
  out[1] = invLength * v2[1];
  out[2] = invLength * v2[2];
  out[3] = invLength * v2[3];
}

void __cdecl QuatLerp(const float *qa, const float *qb, float frac, float *out)
{
  if ( (float)((float)((float)((float)(*qa * *qb) + (float)(qa[1] * qb[1])) + (float)(qa[2] * qb[2]))
             + (float)(qa[3] * qb[3])) < 0.0 )
  {
    *(unsigned int *)out = *(unsigned int *)qb ^ _mask__NegFloat_;
    *((unsigned int *)out + 1) = *((unsigned int *)qb + 1) ^ _mask__NegFloat_;
    *((unsigned int *)out + 2) = *((unsigned int *)qb + 2) ^ _mask__NegFloat_;
    *((unsigned int *)out + 3) = *((unsigned int *)qb + 3) ^ _mask__NegFloat_;
    Vec4Lerp(qa, out, frac, out);
  }
  else
  {
    Vec4Lerp(qa, qb, frac, out);
  }
}

bool __cdecl CullBoxFromCone(
        const float *coneOrg,
        const float *coneDir,
        float cosHalfFov,
        const float *boxCenter,
        const float *boxHalfSize)
{
  float v6; // [esp+0h] [ebp-64h]
  float v7; // [esp+4h] [ebp-60h]
  float v8; // [esp+8h] [ebp-5Ch]
  float dist; // [esp+2Ch] [ebp-38h]
  float deltaMid; // [esp+30h] [ebp-34h]
  float deltaMid_4; // [esp+34h] [ebp-30h]
  float deltaMid_8; // [esp+38h] [ebp-2Ch]
  float sinHalfFovSq; // [esp+3Ch] [ebp-28h]
  float perpendicular; // [esp+44h] [ebp-20h]
  float perpendicular_4; // [esp+48h] [ebp-1Ch]
  float perpendicular_8; // [esp+4Ch] [ebp-18h]
  float scale; // [esp+50h] [ebp-14h]
  float perpLenSq; // [esp+54h] [ebp-10h]
  float farCorner; // [esp+58h] [ebp-Ch]
  float farCorner_4; // [esp+5Ch] [ebp-8h]
  float farCorner_8; // [esp+60h] [ebp-4h]

  if ( cosHalfFov < 0.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
          3926,
          0,
          "%s",
          "cosHalfFov >= 0.0f") )
  {
    __debugbreak();
  }
  deltaMid = *boxCenter - *coneOrg;
  deltaMid_4 = boxCenter[1] - coneOrg[1];
  deltaMid_8 = boxCenter[2] - coneOrg[2];
  if ( *coneDir < 0.0 )
    v8 = -1.0f;
  else
    v8 = 1.0f;
  farCorner = deltaMid - (float)(*boxHalfSize * v8);
  if ( coneDir[1] < 0.0 )
    v7 = -1.0f;
  else
    v7 = 1.0f;
  farCorner_4 = deltaMid_4 - (float)(boxHalfSize[1] * v7);
  if ( coneDir[2] < 0.0 )
    v6 = -1.0f;
  else
    v6 = 1.0f;
  farCorner_8 = deltaMid_8 - (float)(boxHalfSize[2] * v6);
  dist = (float)((float)(farCorner * *coneDir) + (float)(farCorner_4 * coneDir[1])) + (float)(farCorner_8 * coneDir[2]);
  if ( dist >= 0.0 )
    return 1;
  perpendicular = (float)(COERCE_FLOAT(LODWORD(dist) ^ _mask__NegFloat_) * *coneDir) + farCorner;
  perpendicular_4 = (float)(COERCE_FLOAT(LODWORD(dist) ^ _mask__NegFloat_) * coneDir[1]) + farCorner_4;
  perpendicular_8 = (float)(COERCE_FLOAT(LODWORD(dist) ^ _mask__NegFloat_) * coneDir[2]) + farCorner_8;
  perpLenSq = (float)((float)(perpendicular * perpendicular) + (float)(perpendicular_4 * perpendicular_4))
            + (float)(perpendicular_8 * perpendicular_8);
  sinHalfFovSq = 1.0 - (float)(cosHalfFov * cosHalfFov);
  if ( (float)((float)(dist * dist) * sinHalfFovSq) >= (float)(perpLenSq * (float)(cosHalfFov * cosHalfFov)) )
    return 0;
  scale = cosHalfFov / fsqrt(sinHalfFovSq * perpLenSq);
  return (float)((float)((float)((float)((float)((float)((float)((float)(scale * perpendicular) + *coneDir) * deltaMid)
                                               + (float)((float)((float)(scale * perpendicular_4) + coneDir[1])
                                                       * deltaMid_4))
                                       + (float)((float)((float)(scale * perpendicular_8) + coneDir[2]) * deltaMid_8))
                               - COERCE_FLOAT(
                                   COERCE_UNSIGNED_INT(*boxHalfSize * (float)((float)(scale * perpendicular) + *coneDir))
                                 & _mask__AbsFloat_))
                       - COERCE_FLOAT(
                           COERCE_UNSIGNED_INT(boxHalfSize[1] * (float)((float)(scale * perpendicular_4) + coneDir[1]))
                         & _mask__AbsFloat_))
               - COERCE_FLOAT(
                   COERCE_UNSIGNED_INT(boxHalfSize[2] * (float)((float)(scale * perpendicular_8) + coneDir[2]))
                 & _mask__AbsFloat_)) >= 0.0;
}

bool __cdecl CullBoxFromSphere(const float *sphereOrg, float radius, const float *boxCenter, const float *boxHalfSize)
{
  float v5; // [esp+0h] [ebp-3Ch]
  float v6; // [esp+4h] [ebp-38h]
  float v7; // [esp+8h] [ebp-34h]

  if ( (float)((float)(COERCE_FLOAT(COERCE_UNSIGNED_INT(*sphereOrg - *boxCenter) & _mask__AbsFloat_) - *boxHalfSize)
             - 0.0) < 0.0 )
    v7 = 0.0f;
  else
    v7 = COERCE_FLOAT(COERCE_UNSIGNED_INT(*sphereOrg - *boxCenter) & _mask__AbsFloat_) - *boxHalfSize;
  if ( (float)((float)(COERCE_FLOAT(COERCE_UNSIGNED_INT(sphereOrg[1] - boxCenter[1]) & _mask__AbsFloat_) - boxHalfSize[1])
             - 0.0) < 0.0 )
    v6 = 0.0f;
  else
    v6 = COERCE_FLOAT(COERCE_UNSIGNED_INT(sphereOrg[1] - boxCenter[1]) & _mask__AbsFloat_) - boxHalfSize[1];
  if ( (float)((float)(COERCE_FLOAT(COERCE_UNSIGNED_INT(sphereOrg[2] - boxCenter[2]) & _mask__AbsFloat_) - boxHalfSize[2])
             - 0.0) < 0.0 )
    v5 = 0.0f;
  else
    v5 = COERCE_FLOAT(COERCE_UNSIGNED_INT(sphereOrg[2] - boxCenter[2]) & _mask__AbsFloat_) - boxHalfSize[2];
  return (float)((float)((float)(v7 * v7) + (float)(v6 * v6)) + (float)(v5 * v5)) > (float)(radius * radius);
}

bool __cdecl CullBoxFromConicSectionOfSphere(
        const float *coneOrg,
        const float *coneDir,
        float cosHalfFov,
        float radius,
        const float *boxCenter,
        const float *boxHalfSize)
{
  float v7; // [esp+0h] [ebp-A0h]
  float v8; // [esp+4h] [ebp-9Ch]
  float v9; // [esp+8h] [ebp-98h]
  float v10; // [esp+Ch] [ebp-94h]
  float v11; // [esp+10h] [ebp-90h]
  float v12; // [esp+14h] [ebp-8Ch]
  float dist; // [esp+5Ch] [ebp-44h]
  float deltaMid; // [esp+60h] [ebp-40h]
  float deltaMid_4; // [esp+64h] [ebp-3Ch]
  float deltaMid_8; // [esp+68h] [ebp-38h]
  float sinHalfFovSq; // [esp+6Ch] [ebp-34h]
  float perpendicular; // [esp+80h] [ebp-20h]
  float perpendicular_4; // [esp+84h] [ebp-1Ch]
  float perpendicular_8; // [esp+88h] [ebp-18h]
  float scale; // [esp+8Ch] [ebp-14h]
  float perpLenSq; // [esp+90h] [ebp-10h]
  float farCorner; // [esp+94h] [ebp-Ch]
  float farCorner_4; // [esp+98h] [ebp-8h]
  float farCorner_8; // [esp+9Ch] [ebp-4h]

  if ( cosHalfFov < 0.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
          3999,
          0,
          "%s",
          "cosHalfFov >= 0.0f") )
  {
    __debugbreak();
  }
  deltaMid = *boxCenter - *coneOrg;
  deltaMid_4 = boxCenter[1] - coneOrg[1];
  deltaMid_8 = boxCenter[2] - coneOrg[2];
  if ( (float)((float)(COERCE_FLOAT(LODWORD(deltaMid) & _mask__AbsFloat_) - *boxHalfSize) - 0.0) < 0.0 )
    v12 = 0.0f;
  else
    v12 = COERCE_FLOAT(LODWORD(deltaMid) & _mask__AbsFloat_) - *boxHalfSize;
  if ( (float)((float)(COERCE_FLOAT(LODWORD(deltaMid_4) & _mask__AbsFloat_) - boxHalfSize[1]) - 0.0) < 0.0 )
    v11 = 0.0f;
  else
    v11 = COERCE_FLOAT(LODWORD(deltaMid_4) & _mask__AbsFloat_) - boxHalfSize[1];
  if ( (float)((float)(COERCE_FLOAT(LODWORD(deltaMid_8) & _mask__AbsFloat_) - boxHalfSize[2]) - 0.0) < 0.0 )
    v10 = 0.0f;
  else
    v10 = COERCE_FLOAT(LODWORD(deltaMid_8) & _mask__AbsFloat_) - boxHalfSize[2];
  if ( (float)((float)((float)(v12 * v12) + (float)(v11 * v11)) + (float)(v10 * v10)) > (float)(radius * radius) )
    return 1;
  if ( *coneDir < 0.0 )
    v9 = -1.0f;
  else
    v9 = 1.0f;
  farCorner = deltaMid - (float)(*boxHalfSize * v9);
  if ( coneDir[1] < 0.0 )
    v8 = -1.0f;
  else
    v8 = 1.0f;
  farCorner_4 = deltaMid_4 - (float)(boxHalfSize[1] * v8);
  if ( coneDir[2] < 0.0 )
    v7 = -1.0f;
  else
    v7 = 1.0f;
  farCorner_8 = deltaMid_8 - (float)(boxHalfSize[2] * v7);
  dist = (float)((float)(farCorner * *coneDir) + (float)(farCorner_4 * coneDir[1])) + (float)(farCorner_8 * coneDir[2]);
  if ( dist >= 0.0 )
    return 1;
  perpendicular = (float)(COERCE_FLOAT(LODWORD(dist) ^ _mask__NegFloat_) * *coneDir) + farCorner;
  perpendicular_4 = (float)(COERCE_FLOAT(LODWORD(dist) ^ _mask__NegFloat_) * coneDir[1]) + farCorner_4;
  perpendicular_8 = (float)(COERCE_FLOAT(LODWORD(dist) ^ _mask__NegFloat_) * coneDir[2]) + farCorner_8;
  perpLenSq = (float)((float)(perpendicular * perpendicular) + (float)(perpendicular_4 * perpendicular_4))
            + (float)(perpendicular_8 * perpendicular_8);
  sinHalfFovSq = 1.0 - (float)(cosHalfFov * cosHalfFov);
  if ( (float)((float)(dist * dist) * sinHalfFovSq) >= (float)(perpLenSq * (float)(cosHalfFov * cosHalfFov)) )
    return 0;
  scale = cosHalfFov / fsqrt(sinHalfFovSq * perpLenSq);
  return (float)((float)((float)((float)((float)((float)((float)((float)(scale * perpendicular) + *coneDir) * deltaMid)
                                               + (float)((float)((float)(scale * perpendicular_4) + coneDir[1])
                                                       * deltaMid_4))
                                       + (float)((float)((float)(scale * perpendicular_8) + coneDir[2]) * deltaMid_8))
                               - COERCE_FLOAT(
                                   COERCE_UNSIGNED_INT(*boxHalfSize * (float)((float)(scale * perpendicular) + *coneDir))
                                 & _mask__AbsFloat_))
                       - COERCE_FLOAT(
                           COERCE_UNSIGNED_INT(boxHalfSize[1] * (float)((float)(scale * perpendicular_4) + coneDir[1]))
                         & _mask__AbsFloat_))
               - COERCE_FLOAT(
                   COERCE_UNSIGNED_INT(boxHalfSize[2] * (float)((float)(scale * perpendicular_8) + coneDir[2]))
                 & _mask__AbsFloat_)) >= 0.0;
}

bool __cdecl CullSphereFromCone(
        const float *coneOrg,
        const float *coneDir,
        float cosHalfFov,
        const float *sphereCenter,
        float radius)
{
  float delta; // [esp+8h] [ebp-28h]
  float delta_4; // [esp+Ch] [ebp-24h]
  float delta_8; // [esp+10h] [ebp-20h]
  float dist; // [esp+14h] [ebp-1Ch]
  float discriminant; // [esp+18h] [ebp-18h]
  float perpendicular_4; // [esp+24h] [ebp-Ch]
  float perpendicular_8; // [esp+28h] [ebp-8h]

  if ( cosHalfFov < 0.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\com_math.cpp",
          4061,
          0,
          "%s",
          "cosHalfFov >= 0.0f") )
  {
    __debugbreak();
  }
  delta = *sphereCenter - *coneOrg;
  delta_4 = sphereCenter[1] - coneOrg[1];
  delta_8 = sphereCenter[2] - coneOrg[2];
  dist = (float)((float)(delta * *coneDir) + (float)(delta_4 * coneDir[1])) + (float)(delta_8 * coneDir[2]);
  if ( dist >= radius )
    return 1;
  perpendicular_4 = (float)(COERCE_FLOAT(LODWORD(dist) ^ _mask__NegFloat_) * coneDir[1]) + delta_4;
  perpendicular_8 = (float)(COERCE_FLOAT(LODWORD(dist) ^ _mask__NegFloat_) * coneDir[2]) + delta_8;
  discriminant = (float)(fsqrt(1.0 - (float)(cosHalfFov * cosHalfFov)) * dist) - radius;
  return (float)((float)((float)((float)((float)((float)(COERCE_FLOAT(LODWORD(dist) ^ _mask__NegFloat_) * *coneDir)
                                               + delta)
                                       * (float)((float)(COERCE_FLOAT(LODWORD(dist) ^ _mask__NegFloat_) * *coneDir)
                                               + delta))
                               + (float)(perpendicular_4 * perpendicular_4))
                       + (float)(perpendicular_8 * perpendicular_8))
               * (float)(cosHalfFov * cosHalfFov)) >= (float)(discriminant * discriminant);
}

void __cdecl colorTempMatrix(float (*finalMatrix)[4], float colorTemp)
{
  float v2; // [esp+2E8h] [ebp-148h]
  float v3; // [esp+2F0h] [ebp-140h]
  float v4; // [esp+2F8h] [ebp-138h]
  float v5; // [esp+300h] [ebp-130h]
  float v6; // [esp+308h] [ebp-128h]
  float v7; // [esp+310h] [ebp-120h]
  float v8; // [esp+318h] [ebp-118h]
  float v9; // [esp+320h] [ebp-110h]
  float v10; // [esp+328h] [ebp-108h]
  float v11[16]; // [esp+330h] [ebp-100h] BYREF
  float in1[32]; // [esp+370h] [ebp-C0h] BYREF
  float v13[4]; // [esp+3F0h] [ebp-40h] BYREF
  float out[4]; // [esp+400h] [ebp-30h] BYREF
  float vec[4]; // [esp+410h] [ebp-20h] BYREF
  float XYZ[4]; // [esp+420h] [ebp-10h] BYREF

  colorTempToXYZ(6500.0, XYZ);
  colorTempToXYZ(colorTemp, vec);
  MatrixTransformVector44(XYZ, bradfordMA, out);
  MatrixTransformVector44(vec, bradfordMA, v13);
  in1[16] = v13[0] / out[0];
  memset(&in1[17], 0, 16);
  in1[21] = v13[1] / out[1];
  memset(&in1[22], 0, 16);
  in1[26] = v13[2] / out[2];
  memset(&in1[27], 0, 16);
  in1[31] = 1.0f;
  MatrixMultiply44(bradfordMA, (const float (*)[4])&in1[16], (float (*)[4])in1);
  MatrixMultiply44((const float (*)[4])in1, bradfordMI, (float (*)[4])v11);
  MatrixMultiply44(sRGBtoXYZ, (const float (*)[4])v11, (float (*)[4])in1);
  MatrixMultiply44((const float (*)[4])in1, XYZtosRGB, finalMatrix);
  (*finalMatrix)[0] = (*finalMatrix)[0] + 0.00048828125;
  v10 = floor((float)((*finalMatrix)[0] * 1024.0));
  (*finalMatrix)[0] = v10 / 1024.0;
  (*finalMatrix)[1] = (*finalMatrix)[1] + 0.00048828125;
  v9 = floor((float)((*finalMatrix)[1] * 1024.0));
  (*finalMatrix)[1] = v9 / 1024.0;
  (*finalMatrix)[2] = (*finalMatrix)[2] + 0.00048828125;
  v8 = floor((float)((*finalMatrix)[2] * 1024.0));
  (*finalMatrix)[2] = v8 / 1024.0;
  (*finalMatrix)[4] = (*finalMatrix)[4] + 0.00048828125;
  v7 = floor((float)((*finalMatrix)[4] * 1024.0));
  (*finalMatrix)[4] = v7 / 1024.0;
  (*finalMatrix)[5] = (*finalMatrix)[5] + 0.00048828125;
  v6 = floor((float)((*finalMatrix)[5] * 1024.0));
  (*finalMatrix)[5] = v6 / 1024.0;
  (*finalMatrix)[6] = (*finalMatrix)[6] + 0.00048828125;
  v5 = floor((float)((*finalMatrix)[6] * 1024.0));
  (*finalMatrix)[6] = v5 / 1024.0;
  (*finalMatrix)[8] = (*finalMatrix)[8] + 0.00048828125;
  v4 = floor((float)((*finalMatrix)[8] * 1024.0));
  (*finalMatrix)[8] = v4 / 1024.0;
  (*finalMatrix)[9] = (*finalMatrix)[9] + 0.00048828125;
  v3 = floor((float)((*finalMatrix)[9] * 1024.0));
  (*finalMatrix)[9] = v3 / 1024.0;
  (*finalMatrix)[10] = (*finalMatrix)[10] + 0.00048828125;
  v2 = floor((float)((*finalMatrix)[10] * 1024.0));
  (*finalMatrix)[10] = v2 / 1024.0;
}

void __cdecl colorTempToXYZ(float colorTemp, float *XYZ)
{
  float v2; // xmm0_4
  float v3; // xmm0_4
  double v4; // [esp+0h] [ebp-50h]
  double v5; // [esp+8h] [ebp-48h]
  double yd; // [esp+18h] [ebp-38h]
  double xd; // [esp+20h] [ebp-30h]
  double t3; // [esp+28h] [ebp-28h]
  double t2; // [esp+30h] [ebp-20h]
  double xd2; // [esp+38h] [ebp-18h]
  double xd3; // [esp+40h] [ebp-10h]
  float x; // [esp+48h] [ebp-8h]

  if ( colorTemp >= 1650.0 )
    v5 = colorTemp;
  else
    v5 = DOUBLE_1650_0;
  if ( v5 <= 25000.0 )
    v4 = v5;
  else
    v4 = DOUBLE_25000_0;
  t2 = v4 * v4;
  t3 = v4 * v4 * v4;
  if ( v4 >= 4000.0 )
  {
    if ( v4 >= 7000.0 )
      xd = -2006400000.0 / t3 + 1901800.0 / t2 + 247.48 / v4 + 0.23704;
    else
      xd = -4607000000.0 / t3 + 2967800.0 / t2 + 99.11 / v4 + 0.244063;
  }
  else
  {
    xd = -266123900.0 / t3 - 234358.0 / t2 + 877.6956 / v4 + 0.17991;
  }
  xd2 = xd * xd;
  xd3 = xd * xd * xd;
  if ( v4 >= 4000.0 )
  {
    yd = -3.0 * xd2 + 2.87 * xd - 0.275;
  }
  else if ( xd > 0.50338 || v4 <= 2222.0 )
  {
    yd = -1.1063814 * xd3 - 1.3481102 * xd2 + 2.18555832 * xd - 0.20219683;
  }
  else
  {
    yd = -0.9549476 * xd3 - 1.37418593 * xd2 + 2.09137015 * xd - 0.16748867;
  }
  v2 = xd;
  x = v2;
  v3 = yd;
  *XYZ = x;
  XYZ[1] = v3;
  XYZ[2] = (float)(1.0 - x) - v3;
  XYZ[3] = 1.0f;
}

void __cdecl colorHueMatrix(float (*finalMatrix)[4], float hue)
{
  long double v2; // [esp+3ACh] [ebp-160h]
  long double v3; // [esp+3ACh] [ebp-160h]
  float v4; // [esp+3B4h] [ebp-158h]
  float v5; // [esp+3BCh] [ebp-150h]
  float v6; // [esp+3C4h] [ebp-148h]
  float v7; // [esp+3CCh] [ebp-140h]
  float v8; // [esp+3D4h] [ebp-138h]
  float v9; // [esp+3DCh] [ebp-130h]
  float v10; // [esp+3E4h] [ebp-128h]
  float v11; // [esp+3ECh] [ebp-120h]
  float v12; // [esp+3F4h] [ebp-118h]
  float out[68]; // [esp+3FCh] [ebp-110h] BYREF

  out[52] = FLOAT_0_81649655;
  out[53] = 0.0f;
  out[54] = 0.5f3451085;
  out[55] = 0.0f;
  out[56] = FLOAT_N0_40824828;
  out[57] = FLOAT_0_70710665;
  out[58] = 1.0f555116;
  out[59] = 0.0f;
  out[60] = FLOAT_N0_40824828;
  out[61] = FLOAT_N0_70710677;
  out[62] = 0.1f4204822;
  memset(&out[63], 0, 16);
  out[67] = 1.0f;
  out[36] = FLOAT_0_84678853;
  out[37] = FLOAT_N0_37795621;
  out[38] = FLOAT_N0_37795624;
  out[39] = 0.0f;
  out[40] = -0.37292805f;
  out[41] = 0.33417869f;
  out[42] = -1.0f800347;
  out[43] = 0.0f;
  out[44] = 0.5f773502;
  out[45] = 0.5f773502;
  out[46] = 0.5f773502;
  memset(&out[47], 0, 16);
  out[51] = 1.0f;
  __libm_sse2_sin(v2);
  out[35] = hue * 0.017453199;
  __libm_sse2_cos(v3);
  out[34] = hue * 0.017453199;
  out[16] = hue * 0.017453199;
  out[17] = hue * 0.017453199;
  out[18] = 0.0f;
  out[19] = 0.0f;
  LODWORD(out[20]) = COERCE_UNSIGNED_INT(hue * 0.017453199) ^ _mask__NegFloat_;
  out[21] = hue * 0.017453199;
  memset(&out[22], 0, 16);
  out[26] = 1.0f;
  memset(&out[27], 0, 16);
  out[31] = 1.0f;
  MatrixMultiply44((const float (*)[4])&out[52], (const float (*)[4])&out[16], (float (*)[4])out);
  MatrixMultiply44((const float (*)[4])out, (const float (*)[4])&out[36], finalMatrix);
  (*finalMatrix)[0] = (*finalMatrix)[0] + 0.00048828125;
  v12 = floor((float)((*finalMatrix)[0] * 1024.0));
  (*finalMatrix)[0] = v12 / 1024.0;
  (*finalMatrix)[1] = (*finalMatrix)[1] + 0.00048828125;
  v11 = floor((float)((*finalMatrix)[1] * 1024.0));
  (*finalMatrix)[1] = v11 / 1024.0;
  (*finalMatrix)[2] = (*finalMatrix)[2] + 0.00048828125;
  v10 = floor((float)((*finalMatrix)[2] * 1024.0));
  (*finalMatrix)[2] = v10 / 1024.0;
  (*finalMatrix)[4] = (*finalMatrix)[4] + 0.00048828125;
  v9 = floor((float)((*finalMatrix)[4] * 1024.0));
  (*finalMatrix)[4] = v9 / 1024.0;
  (*finalMatrix)[5] = (*finalMatrix)[5] + 0.00048828125;
  v8 = floor((float)((*finalMatrix)[5] * 1024.0));
  (*finalMatrix)[5] = v8 / 1024.0;
  (*finalMatrix)[6] = (*finalMatrix)[6] + 0.00048828125;
  v7 = floor((float)((*finalMatrix)[6] * 1024.0));
  (*finalMatrix)[6] = v7 / 1024.0;
  (*finalMatrix)[8] = (*finalMatrix)[8] + 0.00048828125;
  v6 = floor((float)((*finalMatrix)[8] * 1024.0));
  (*finalMatrix)[8] = v6 / 1024.0;
  (*finalMatrix)[9] = (*finalMatrix)[9] + 0.00048828125;
  v5 = floor((float)((*finalMatrix)[9] * 1024.0));
  (*finalMatrix)[9] = v5 / 1024.0;
  (*finalMatrix)[10] = (*finalMatrix)[10] + 0.00048828125;
  v4 = floor((float)((*finalMatrix)[10] * 1024.0));
  (*finalMatrix)[10] = v4 / 1024.0;
}

void __cdecl colorSaturationMatrix(float (*finalMatrix)[4], float saturation)
{
  float r; // [esp+0h] [ebp-Ch]
  float g; // [esp+4h] [ebp-8h]

  r = (float)(1.0 - saturation) * 0.25;
  g = (float)(1.0 - saturation) * 0.5;
  (*finalMatrix)[0] = r + saturation;
  (*finalMatrix)[1] = r;
  (*finalMatrix)[2] = r;
  (*finalMatrix)[3] = 0.0f;
  (*finalMatrix)[4] = g;
  (*finalMatrix)[5] = g + saturation;
  (*finalMatrix)[6] = g;
  (*finalMatrix)[7] = 0.0f;
  (*finalMatrix)[8] = r;
  (*finalMatrix)[9] = r;
  (*finalMatrix)[10] = r + saturation;
  (*finalMatrix)[15] = 0.0f;
  (*finalMatrix)[12] = 0.0f;
  (*finalMatrix)[13] = 0.0f;
  (*finalMatrix)[14] = 0.0f;
  (*finalMatrix)[15] = 1.0f;
}

void __cdecl AxisCopy(const float (*in)[3], float (*out)[3])
{
  (*out)[0] = (*in)[0];
  (*out)[1] = (*in)[1];
  (*out)[2] = (*in)[2];
  (*out)[3] = (*in)[3];
  (*out)[4] = (*in)[4];
  (*out)[5] = (*in)[5];
  (*out)[6] = (*in)[6];
  (*out)[7] = (*in)[7];
  (*out)[8] = (*in)[8];
}

void __cdecl Vec3Lerp(const float *start, const float *end, float fraction, float *endpos)
{
    *endpos = (float)((float)(*end - *start) * fraction) + *start;
    endpos[1] = (float)((float)(end[1] - start[1]) * fraction) + start[1];
    endpos[2] = (float)((float)(end[2] - start[2]) * fraction) + start[2];
}

float __cdecl Vec3DistanceSq(const float *p1, const float *p2)
{
    float v_4; // [esp+4h] [ebp-8h]
    float v_8; // [esp+8h] [ebp-4h]

    v_4 = p2[1] - p1[1];
    v_8 = p2[2] - p1[2];
    return v_8 * v_8 + v_4 * v_4 + (float)(*p2 - *p1) * (float)(*p2 - *p1);
}
