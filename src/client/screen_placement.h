#pragma once

double __cdecl ScrPlace_HiResGetScale();
void __cdecl ScrPlace_HiResApplyRect(float *x, float *y, float *w, float *h);
void __cdecl ScrPlace_SetupUI3DForFullscreen(ScreenPlacement *ui3DScrPlace, ScreenPlacement *fullScrPlace);
void __cdecl ScrPlace_SetupFloatViewport(
        ScreenPlacement *scrPlace,
        float viewportX,
        float viewportY,
        float viewportWidth,
        float viewportHeight);
void __cdecl ScrPlace_CalcSafeAreaOffsets(
        float viewportX,
        float viewportY,
        float viewportWidth,
        float viewportHeight,
        float horzAspectScale,
        float vertAspectScale,
        float safeAreaRatioHorz,
        float safeAreaRatioVert,
        float *realViewableMin,
        float *realViewableMax,
        float *virtualViewableMin,
        float *virtualViewableMax);
void __cdecl ScrPlace_SetupViewport(
        ScreenPlacement *scrPlace,
        int viewportX,
        int viewportY,
        int viewportWidth,
        int viewportHeight);
void __cdecl ScrPlace_SetupUnsafeViewport(
        ScreenPlacement *scrPlace,
        int viewportX,
        int viewportY,
        int viewportWidth,
        int viewportHeight);
double __cdecl ScrPlace_ApplyXinverse(const ScreenPlacement *scrPlace, float x, int horzAlign);
double __cdecl ScrPlace_ApplyYinverse(const ScreenPlacement *scrPlace, float y, int vertAlign);
double __cdecl ScrPlace_ApplyX(const ScreenPlacement *scrPlace, float x, int horzAlign);
double __cdecl ScrPlace_ApplyY(const ScreenPlacement *scrPlace, float y, int vertAlign);
double __cdecl ScrPlace_ApplyW(const ScreenPlacement *scrPlace, float w, int horzAlign);
double __cdecl ScrPlace_ApplyH(const ScreenPlacement *scrPlace, float h, int vertAlign);
void __cdecl ScrPlace_ApplyRect(
        const ScreenPlacement *scrPlace,
        float *x,
        float *y,
        float *w,
        float *h,
        int horzAlign,
        int vertAlign);
