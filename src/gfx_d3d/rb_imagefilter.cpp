#include "rb_imagefilter.h"

void __cdecl RB_GaussianFilterImage(float radius, unsigned __int8 srcRenderTargetId, unsigned __int8 dstRenderTargetId)
{
  float radiusY; // [esp+30h] [ebp-13ACh] BYREF
  int srcWidth; // [esp+34h] [ebp-13A8h]
  float radiusX; // [esp+38h] [ebp-13A4h] BYREF
  int dstWidth; // [esp+3Ch] [ebp-13A0h]
  int srcHeight; // [esp+44h] [ebp-1398h]
  int dstHeight; // [esp+48h] [ebp-1394h]
  GfxImageFilter filter; // [esp+4Ch] [ebp-1390h] BYREF

  //PIXBeginNamedEvent(-1, "RB_GaussianFilterImage");
  RB_VirtualToSceneRadius(radius, &radiusX, &radiusY);
  srcWidth = (unsigned __int16)word_B50E83C[10 * srcRenderTargetId];
  srcHeight = (unsigned __int16)word_B50E83E[10 * srcRenderTargetId];
  dstWidth = (unsigned __int16)word_B50E83C[10 * dstRenderTargetId];
  dstHeight = (unsigned __int16)word_B50E83E[10 * dstRenderTargetId];
  filter.sourceImage = gfxRenderTargets[srcRenderTargetId].image;
  filter.finalTarget = dstRenderTargetId;
  filter.passCount = RB_GenerateGaussianFilterChain(
                       radiusX,
                       radiusY,
                       srcWidth,
                       srcHeight,
                       dstWidth,
                       dstHeight,
                       32,
                       filter.passes);
  if ( filter.passCount )
  {
    RB_FilterImage(&filter);
    if ( GetCurrentThreadId() != g_DXDeviceThread )
      return;
  }
  else if ( g_DXDeviceThread != GetCurrentThreadId() )
  {
    return;
  }
  D3DPERF_EndEvent();
}

void __cdecl RB_VirtualToSceneRadius(float radius, float *radiusX, float *radiusY)
{
  if ( !radiusX
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_imagefilter.cpp", 64, 0, "%s", "radiusX") )
  {
    __debugbreak();
  }
  if ( !radiusY
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_imagefilter.cpp", 65, 0, "%s", "radiusY") )
  {
    __debugbreak();
  }
  *radiusY = (double)vidConfig.sceneHeight * radius / 480.0;
  *radiusX = *radiusY * vidConfig.aspectRatioScenePixel;
}

int __cdecl RB_GenerateGaussianFilterChain(
        float radiusX,
        float radiusY,
        int srcWidth,
        int srcHeight,
        int dstWidth,
        int dstHeight,
        int passLimit,
        GfxImageFilterPass *filterPass)
{
  float v9; // [esp+18h] [ebp-24h]
  int dstRes[2]; // [esp+24h] [ebp-18h] BYREF
  float passRadius; // [esp+2Ch] [ebp-10h]
  int passRes; // [esp+30h] [ebp-Ch]
  int passAxis; // [esp+34h] [ebp-8h]
  int passCount; // [esp+38h] [ebp-4h]

  dstRes[0] = dstWidth;
  dstRes[1] = dstHeight;
  passCount = 0;
  if ( srcWidth != dstWidth || srcHeight != dstHeight )
  {
    if ( (float)(radiusY - radiusX) < 0.0 )
      v9 = radiusY;
    else
      v9 = radiusX;
    passRadius = v9;
    if ( v9 > 1.3895605 )
      passRadius = FLOAT_1_3895605;
    radiusX = (float)(fsqrt(
                        COERCE_FLOAT(
                          COERCE_UNSIGNED_INT((float)(radiusX * radiusX) - (float)(passRadius * passRadius))
                        & _mask__AbsFloat_))
                    * (float)dstWidth)
            / (float)srcWidth;
    radiusY = (float)(fsqrt(
                        COERCE_FLOAT(
                          COERCE_UNSIGNED_INT((float)(radiusY * radiusY) - (float)(passRadius * passRadius))
                        & _mask__AbsFloat_))
                    * (float)dstHeight)
            / (float)srcHeight;
    if ( passCount >= passLimit
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_imagefilter.cpp",
            241,
            0,
            "%s",
            "passCount < passLimit") )
    {
      __debugbreak();
    }
    RB_GenerateGaussianFilter2D(passRadius, srcWidth, srcHeight, dstWidth, dstHeight, &filterPass[passCount++]);
  }
  while ( passCount < 32 && (radiusX >= 0.32950512 || radiusY >= 0.32950512) )
  {
    if ( COERCE_FLOAT(COERCE_UNSIGNED_INT(radiusX - radiusY) & _mask__AbsFloat_) < 0.32950512 )
    {
      passRadius = (float)(radiusX + radiusY) * 0.5;
      if ( passRadius <= 1.3895605 )
      {
        if ( passCount >= passLimit
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_imagefilter.cpp",
                253,
                0,
                "%s",
                "passCount < passLimit") )
        {
          __debugbreak();
        }
        RB_GenerateGaussianFilter2D(passRadius, dstWidth, dstHeight, dstWidth, dstHeight, &filterPass[passCount++]);
        break;
      }
    }
    if ( radiusX <= radiusY )
    {
      if ( radiusY > 6.4977503 )
      {
        passRadius = FLOAT_6_4977503;
        radiusY = fsqrt((float)(radiusY * radiusY) - 42.22076);
      }
      else
      {
        passRadius = radiusY;
        radiusY = 0.0f;
      }
      passAxis = 1;
      passRes = dstHeight;
    }
    else
    {
      if ( radiusX >= 6.4977503 )
      {
        passRadius = FLOAT_6_4977503;
        radiusX = fsqrt((float)(radiusX * radiusX) - 42.22076);
      }
      else
      {
        passRadius = radiusX;
        radiusX = 0.0f;
      }
      passAxis = 0;
      passRes = dstWidth;
    }
    if ( passCount >= passLimit
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_imagefilter.cpp",
            291,
            0,
            "%s",
            "passCount < passLimit") )
    {
      __debugbreak();
    }
    RB_GenerateGaussianFilter1D(passRadius, dstRes, passAxis, &filterPass[passCount++]);
  }
  if ( passCount > passLimit
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_imagefilter.cpp",
          296,
          1,
          "%s",
          "passCount <= passLimit") )
  {
    __debugbreak();
  }
  return passCount;
}

void __cdecl RB_GenerateGaussianFilter1D(float radius, int *res, int axis, GfxImageFilterPass *filterPass)
{
  float tapWeights[8]; // [esp+18h] [ebp-48h] BYREF
  int tapHalfCount; // [esp+38h] [ebp-28h]
  int tapIndex; // [esp+3Ch] [ebp-24h]
  float tapOffsets[8]; // [esp+40h] [ebp-20h] BYREF

  tapHalfCount = RB_GaussianFilterPoints1D(radius, res[axis], res[axis], 8, tapOffsets, tapWeights);
  filterPass->tapHalfCount = RB_PickSymmetricFilterMaterial(tapHalfCount, &filterPass->material);
  for ( tapIndex = 0; tapIndex < 8; ++tapIndex )
  {
    filterPass->tapOffsetsAndWeights[tapIndex][axis] = tapOffsets[tapIndex];
    filterPass->tapOffsetsAndWeights[tapIndex][1 - axis] = 0.0f;
    filterPass->tapOffsetsAndWeights[tapIndex][2] = 0.0f;
    filterPass->tapOffsetsAndWeights[tapIndex][3] = tapWeights[tapIndex];
  }
  filterPass->srcWidth = 1.0f;
  filterPass->srcHeight = 1.0f;
  filterPass->dstWidth = *res;
  filterPass->dstHeight = res[1];
}

int __cdecl RB_PickSymmetricFilterMaterial(int halfTapCount, const Material **material)
{
  if ( (halfTapCount <= 0 || halfTapCount > 8)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_imagefilter.cpp",
          82,
          0,
          "%s\n\t(halfTapCount) = %i",
          "(halfTapCount > 0 && halfTapCount <= 8)",
          halfTapCount) )
  {
    __debugbreak();
  }
  *material = (const Material *)*((unsigned int *)&rgp.poisonFXMaterial + halfTapCount);
  return halfTapCount;
}

int __cdecl RB_GaussianFilterPoints1D(
        float pixels,
        int srcRes,
        int dstRes,
        int tapLimit,
        float *tapOffsets,
        float *tapWeights)
{
  float v6; // xmm0_4
  long double v8; // [esp+8h] [ebp-40h]
  long double v9; // [esp+8h] [ebp-40h]
  int tapHalfCount; // [esp+1Ch] [ebp-2Ch]
  int tapIndex; // [esp+20h] [ebp-28h]
  int tapIndexa; // [esp+20h] [ebp-28h]
  float offset; // [esp+28h] [ebp-20h]
  float totalWeight; // [esp+2Ch] [ebp-1Ch]
  float gaussianExponent; // [esp+30h] [ebp-18h]
  float weight; // [esp+34h] [ebp-14h]
  float weight_4; // [esp+38h] [ebp-10h]
  float sample; // [esp+40h] [ebp-8h]
  float sample_4; // [esp+44h] [ebp-4h]

  if ( pixels <= 0.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_imagefilter.cpp",
          99,
          0,
          "%s\n\t(pixels) = %g",
          "(pixels > 0)",
          pixels) )
  {
    __debugbreak();
  }
  if ( dstRes <= 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_imagefilter.cpp",
          100,
          0,
          "%s\n\t(dstRes) = %i",
          "(dstRes > 0)",
          dstRes) )
  {
    __debugbreak();
  }
  if ( srcRes < dstRes
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_imagefilter.cpp",
          101,
          0,
          "srcRes >= dstRes\n\t%i, %i",
          srcRes,
          dstRes) )
  {
    __debugbreak();
  }
  *((float *)&v8 + 1) = (float)srcRes / (float)dstRes;
  if ( ((int)(*((float *)&v8 + 1) + 9.313225746154785e-10) & 1) != 0 )
    LODWORD(v8) = 0;
  else
    *(float *)&v8 = 0.5f;
  offset = *(float *)&v8;
  gaussianExponent = -0.5 / (float)(pixels * pixels);
  totalWeight = 0.0f;
  for ( tapIndex = 0; tapIndex < tapLimit; ++tapIndex )
  {
    sample = (float)(2 * tapIndex) + offset;
    sample_4 = (float)(2 * tapIndex + 1) + offset;
    __libm_sse2_exp(v8);
    weight = (float)(gaussianExponent * sample) * sample;
    __libm_sse2_exp(v9);
    weight_4 = (float)(gaussianExponent * sample_4) * sample_4;
    if ( !tapIndex && offset == 0.0 )
      weight = weight * 0.5;
    tapWeights[tapIndex] = weight + weight_4;
    if ( tapWeights[tapIndex] == 0.0 )
      v6 = (float)((float)(sample + sample_4) * 0.5) / (float)srcRes;
    else
      v6 = (float)((float)(sample * weight) + (float)(sample_4 * weight_4))
         / (float)((float)srcRes * tapWeights[tapIndex]);
    tapOffsets[tapIndex] = v6;
    totalWeight = totalWeight + tapWeights[tapIndex];
  }
  if ( totalWeight > 0.001 )
  {
    if ( totalWeight <= 0.0
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_imagefilter.cpp",
            136,
            0,
            "%s\n\t(totalWeight) = %g",
            "(totalWeight > 0)",
            totalWeight) )
    {
      __debugbreak();
    }
    tapHalfCount = tapLimit;
    for ( tapIndexa = tapLimit - 1; tapIndexa >= 0; --tapIndexa )
    {
      tapWeights[tapIndexa] = tapWeights[tapIndexa] * (float)(0.5 / totalWeight);
      if ( tapWeights[tapIndexa] < 0.0099999998 )
        tapHalfCount = tapIndexa + 1;
    }
  }
  else
  {
    *tapWeights = 0.5f;
    return 1;
  }
  return tapHalfCount;
}

void __cdecl RB_GenerateGaussianFilter2D(
        float radius,
        int srcWidth,
        int srcHeight,
        int dstWidth,
        int dstHeight,
        GfxImageFilterPass *filterPass)
{
  float tapOffsetsY[2]; // [esp+18h] [ebp-2Ch] BYREF
  float tapWeightsX[2]; // [esp+20h] [ebp-24h] BYREF
  int tapIndex; // [esp+28h] [ebp-1Ch]
  float tapOffsetsX[2]; // [esp+2Ch] [ebp-18h] BYREF
  int x; // [esp+34h] [ebp-10h]
  int y; // [esp+38h] [ebp-Ch]
  float tapWeightsY[2]; // [esp+3Ch] [ebp-8h] BYREF

  RB_GaussianFilterPoints1D(radius, srcWidth, dstWidth, 2, tapOffsetsX, tapWeightsX);
  RB_GaussianFilterPoints1D(radius, srcHeight, dstHeight, 2, tapOffsetsY, tapWeightsY);
  tapIndex = 0;
  for ( y = 0; y < 2; ++y )
  {
    for ( x = 0; x < 2; ++x )
    {
      LODWORD(filterPass->tapOffsetsAndWeights[2 * tapIndex][0]) = LODWORD(tapOffsetsX[x]) ^ _mask__NegFloat_;
      filterPass->tapOffsetsAndWeights[2 * tapIndex][1] = tapOffsetsY[y];
      filterPass->tapOffsetsAndWeights[2 * tapIndex][2] = 0.0f;
      filterPass->tapOffsetsAndWeights[2 * tapIndex][3] = tapWeightsX[x] * tapWeightsY[y];
      filterPass->tapOffsetsAndWeights[2 * tapIndex + 1][0] = tapOffsetsX[x];
      filterPass->tapOffsetsAndWeights[2 * tapIndex + 1][1] = tapOffsetsY[y];
      filterPass->tapOffsetsAndWeights[2 * tapIndex + 1][2] = 0.0f;
      filterPass->tapOffsetsAndWeights[2 * tapIndex++ + 1][3] = tapWeightsX[x] * tapWeightsY[y];
    }
  }
  if ( 2 * tapIndex != 8
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_imagefilter.cpp",
          206,
          1,
          "%s\n\t(tapIndex) = %i",
          "(tapIndex * 2 == 8)",
          tapIndex) )
  {
    __debugbreak();
  }
  filterPass->tapHalfCount = RB_PickSymmetricFilterMaterial(2 * tapIndex, &filterPass->material);
  filterPass->srcWidth = 1.0f;
  filterPass->srcHeight = 1.0f;
  filterPass->dstWidth = dstWidth;
  filterPass->dstHeight = dstHeight;
}

void __cdecl RB_FilterImage(GfxImageFilter *filter)
{
  int passIndex; // [esp+2Ch] [ebp-Ch]

  if ( !filter
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_imagefilter.cpp", 394, 0, "%s", "filter") )
  {
    __debugbreak();
  }
  if ( filter->passCount <= 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_imagefilter.cpp",
          395,
          0,
          "%s\n\t(filter->passCount) = %i",
          "(filter->passCount > 0)",
          filter->passCount) )
  {
    __debugbreak();
  }
  if ( !filter->sourceImage
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_imagefilter.cpp",
          396,
          0,
          "%s",
          "filter->sourceImage") )
  {
    __debugbreak();
  }
  if ( tess.indexCount )
    RB_EndTessSurface();
  for ( passIndex = 0; passIndex < filter->passCount; ++passIndex )
  {
    RB_FilterPingPong(filter, passIndex);
    RB_SetupFilterPass(&filter->passes[passIndex]);
    R_Set2D(&gfxCmdBufSourceState);
    RB_DrawStretchPic(
      filter->passes[passIndex].material,
      0.0,
      0.0,
      (float)dword_B473FD0,
      (float)dword_B473FD4,
      0.0,
      0.0,
      filter->passes[passIndex].srcWidth,
      filter->passes[passIndex].srcHeight,
      0xFFFFFFFF,
      GFX_PRIM_STATS_CODE);
    RB_EndTessSurface();
  }
  BLOPS_NULLSUB();
}

void __cdecl RB_SetupFilterPass(const GfxImageFilterPass *filterPass)
{
  int constIndex; // [esp+4h] [ebp-4h]

  if ( filterPass->tapHalfCount >= 9u
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_imagefilter.cpp",
          339,
          0,
          "filterPass->tapHalfCount doesn't index ARRAY_COUNT( filterPass->tapOffsetsAndWeights ) + 1\n"
          "\t%i not in [0, %i)",
          filterPass->tapHalfCount,
          9) )
  {
    __debugbreak();
  }
  for ( constIndex = 0; constIndex < filterPass->tapHalfCount; ++constIndex )
    R_SetCodeConstantFromVec4(&gfxCmdBufSourceState, constIndex + 36, filterPass->tapOffsetsAndWeights[constIndex]);
}

void __cdecl RB_FilterPingPong(const GfxImageFilter *filter, int passIndex)
{
  unsigned __int8 finalTarget; // [esp+3h] [ebp-11h]
  GfxImage *image; // [esp+4h] [ebp-10h]
  unsigned int pingpong; // [esp+8h] [ebp-Ch]

  pingpong = passIndex & 1;
  if ( passIndex )
    image = gfxRenderTargets[9 - pingpong].image;
  else
    image = filter->sourceImage;
  R_SetCodeImageTexture(&gfxCmdBufSourceState, 8u, image);
  if ( passIndex == filter->passCount - 1 )
    finalTarget = filter->finalTarget;
  else
    finalTarget = pingpong + 8;
  R_SetRenderTargetSize(&gfxCmdBufSourceState, finalTarget);
  R_SetRenderTarget(gfxCmdBufContext, finalTarget);
  R_ClearRenderTargetForMultiGpu(gfxCmdBufContext, finalTarget);
}

