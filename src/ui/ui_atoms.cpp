#include "ui_atoms.h"

void __cdecl UI_DrawHandlePic(
        const ScreenPlacement *scrPlace,
        float x,
        float y,
        float w,
        float h,
        int horzAlign,
        int vertAlign,
        const float *color,
        Material *material)
{
  float t0; // [esp+30h] [ebp-10h]
  float t1; // [esp+34h] [ebp-Ch]
  float s1; // [esp+38h] [ebp-8h]
  float s0; // [esp+3Ch] [ebp-4h]

  if ( w >= 0.0 )
  {
    s0 = *(float *)&FLOAT_0_0;
    s1 = FLOAT_1_0;
  }
  else
  {
    LODWORD(w) ^= _mask__NegFloat_;
    s0 = FLOAT_1_0;
    s1 = *(float *)&FLOAT_0_0;
  }
  if ( h >= 0.0 )
  {
    t0 = *(float *)&FLOAT_0_0;
    t1 = FLOAT_1_0;
  }
  else
  {
    LODWORD(h) ^= _mask__NegFloat_;
    t0 = FLOAT_1_0;
    t1 = *(float *)&FLOAT_0_0;
  }
  CL_DrawStretchPic(scrPlace, x, y, w, h, horzAlign, vertAlign, s0, t0, s1, t1, color, material);
}

void __cdecl UI_DrawHandlePicRotated(
        const ScreenPlacement *scrPlace,
        float x,
        float y,
        float w,
        float h,
        int horzAlign,
        int vertAlign,
        const float *color,
        float angle,
        Material *material)
{
  float t0; // [esp+2Ch] [ebp-10h]
  float t1; // [esp+30h] [ebp-Ch]
  float s1; // [esp+34h] [ebp-8h]
  float s0; // [esp+38h] [ebp-4h]

  if ( w >= 0.0 )
  {
    s0 = *(float *)&FLOAT_0_0;
    s1 = FLOAT_1_0;
  }
  else
  {
    LODWORD(w) ^= _mask__NegFloat_;
    s0 = FLOAT_1_0;
    s1 = *(float *)&FLOAT_0_0;
  }
  if ( h >= 0.0 )
  {
    t0 = *(float *)&FLOAT_0_0;
    t1 = FLOAT_1_0;
  }
  else
  {
    LODWORD(h) ^= _mask__NegFloat_;
    t0 = FLOAT_1_0;
    t1 = *(float *)&FLOAT_0_0;
  }
  ScrPlace_ApplyRect(scrPlace, &x, &y, &w, &h, horzAlign, vertAlign);
  CL_DrawStretchPicPhysicalRotateXY(x, y, w, h, s0, t0, s1, t1, angle, color, material);
}

void __cdecl UI_DrawHandlePicFramed(
        const ScreenPlacement *scrPlace,
        float x,
        float y,
        float w,
        float h,
        int horzAlign,
        int vertAlign,
        float frameSize,
        float frameTexSize,
        int frameSides,
        const float *color,
        Material *material)
{
  float thicknessW; // [esp+28h] [ebp-8h]
  float thicknessH; // [esp+2Ch] [ebp-4h]

  ScrPlace_ApplyRect(scrPlace, &x, &y, &w, &h, horzAlign, vertAlign);
  thicknessW = ScrPlace_ApplyW(scrPlace, frameSize, horzAlign);
  thicknessH = ScrPlace_ApplyH(scrPlace, frameSize, vertAlign);
  CL_DrawFramedPicPhysical(
    x,
    y,
    w,
    h,
    frameSize,
    (float)(thicknessH / thicknessW) * frameSize,
    frameTexSize,
    frameSides,
    color,
    material);
}

void __cdecl UI_DrawLoadBar(
        const ScreenPlacement *scrPlace,
        float x,
        float y,
        float w,
        float h,
        int horzAlign,
        int vertAlign,
        const float *color,
        Material *material)
{
  double v9; // st7
  float percentDone; // [esp+3Ch] [ebp-10h]

  if ( useFastFile->current.enabled )
    v9 = ((double (__cdecl *)(double (__cdecl *)()))UI_LoadBarProgress_FastFile)(UI_LoadBarProgress_FastFile);
  else
    v9 = ((double (__cdecl *)(double (__cdecl *)()))UI_LoadBarProgress_LoadObj)(UI_LoadBarProgress_LoadObj);
  percentDone = v9;
  CL_DrawStretchPic(
    scrPlace,
    x,
    y,
    w * percentDone,
    h,
    horzAlign,
    vertAlign,
    0.0,
    0.0,
    percentDone,
    1.0,
    color,
    material);
}

double __cdecl UI_LoadBarProgress_LoadObj()
{
  float v2; // [esp+4h] [ebp-8h]
  float v3; // [esp+8h] [ebp-4h]

  if ( com_expectedHunkUsage <= 0 )
    return 0.0;
  v2 = (float)com_expectedHunkUsage;
  v3 = (float)Hunk_Used() / v2;
  if ( (float)(v3 - 1.0) < 0.0 )
    return v3;
  else
    return FLOAT_1_0;
}

double __cdecl UI_LoadBarProgress_FastFile()
{
  return DB_GetLoadedFraction();
}

void __cdecl UI_FillRectPhysical(float x, float y, float width, float height, const float *color)
{
  if ( sharedUiInfo.assets.whiteMaterial )
    CL_DrawStretchPicPhysical(x, y, width, height, 0.0, 0.0, 0.0, 0.0, color, sharedUiInfo.assets.whiteMaterial);
}

void __cdecl UI_FillRect(
        const ScreenPlacement *scrPlace,
        float x,
        float y,
        float width,
        float height,
        int horzAlign,
        int vertAlign,
        const float *color)
{
  if ( sharedUiInfo.assets.whiteMaterial )
    CL_DrawStretchPic(
      scrPlace,
      x,
      y,
      width,
      height,
      horzAlign,
      vertAlign,
      0.0,
      0.0,
      0.0,
      0.0,
      color,
      sharedUiInfo.assets.whiteMaterial);
}

void __cdecl UI_DrawSpinner(
        const ScreenPlacement *scrPlace,
        float x,
        float y,
        float width,
        float height,
        int horzAlign,
        int vertAlign,
        const float *color)
{
  CL_DrawSpinner(scrPlace, x, y, width, height, horzAlign, vertAlign, color);
}

void __cdecl UI_DrawSpinnerLoadbar(
        const ScreenPlacement *scrPlace,
        float x,
        float y,
        float width,
        float height,
        int horzAlign,
        int vertAlign,
        const float *color)
{
  double v8; // st7
  float percentDone; // [esp+28h] [ebp-4h]

  if ( useFastFile->current.enabled )
    v8 = ((double (__cdecl *)(double (__cdecl *)()))UI_LoadBarProgress_FastFile)(UI_LoadBarProgress_FastFile);
  else
    v8 = ((double (__cdecl *)(double (__cdecl *)()))UI_LoadBarProgress_LoadObj)(UI_LoadBarProgress_LoadObj);
  percentDone = v8;
  CL_DrawSpinnerLoadbar(scrPlace, x, y, width, height, horzAlign, vertAlign, color, percentDone);
}

