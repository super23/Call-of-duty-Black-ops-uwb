#include "fx_random.h"

void __cdecl FX_RandomDir(int seed, float *dir)
{
  long double v2; // st7
  double v3; // st7
  const char *v4; // eax
  float height; // [esp+38h] [ebp-14h]
  float sinYaw; // [esp+40h] [ebp-Ch]
  float cosYaw; // [esp+44h] [ebp-8h]
  float radiusAtHeight; // [esp+48h] [ebp-4h]

  height = (float)(*(float *)&dword_CAEB08[seed] * 2.0) - 1.0;
  if ( (height < -1.0 || height > 1.0)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\EffectsCore\\fx_random.cpp",
          189,
          0,
          "%s\n\t(height) = %g",
          "(height >= -1.0f && height <= 1.0f)",
          height) )
  {
    __debugbreak();
  }
  radiusAtHeight = fsqrt(1.0 - (float)(height * height));
  v2 = (float)(*(float *)&dword_CAEB04[seed] * 6.2831855);
  cosYaw = cos(v2);
  sinYaw = sin(v2);
  *dir = radiusAtHeight * cosYaw;
  dir[1] = radiusAtHeight * sinYaw;
  dir[2] = height;
  if ( !Vec3IsNormalized(dir) )
  {
    v3 = Abs(dir);
    v4 = va("(%g %g %g) len %g", *dir, dir[1], dir[2], v3);
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\EffectsCore\\fx_random.cpp",
            199,
            0,
            "%s\n\t%s",
            "Vec3IsNormalized( dir )",
            v4) )
      __debugbreak();
  }
}

