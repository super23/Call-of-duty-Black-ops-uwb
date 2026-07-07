#pragma once
#include "curve.h"

struct cCurveManager // sizeof=0x0
{
    static int __cdecl GetFreeCurve();

    static void __cdecl AddNodeToCurve(unsigned int curve, float *p);
    static void __cdecl AddNodeToCurve(unsigned int curve, float *p, float t);
    static void __cdecl SortCurve(unsigned int curve, float *p, bool is_increasing_sort_order);
    static void __cdecl BuildCurve(unsigned int curve);
    static void __cdecl StartCurve(unsigned int curve);
    static void __cdecl ResetCurve(unsigned int curve);
    static void __cdecl StopCurve(unsigned int curve);
    static void __cdecl FreeCurve(unsigned int curve);
    static void __cdecl SetCurveBSpline(unsigned int curve);
    static void __cdecl SetCurveRounded(unsigned int curve);
    static void __cdecl SetCurveSmooth(unsigned int curve);
    static void __cdecl SetCurveSpeed(unsigned int curve, float speed);
    static void __cdecl SetNotifyEnt(unsigned int curve, int entnum);
    static void __cdecl SetNotifyLevel(unsigned int curve);
    static void __cdecl SetCameraEnt(unsigned int curve, int entnum);
    static void __cdecl SetCurveDraw(unsigned int curve, const float *color);
    static void __cdecl GetPos(unsigned int curve, float t, float *p);
    static void __cdecl GetCurClientPos(unsigned int curve, float *pos);
    static void __cdecl GetCurServerPos(unsigned int curve, float *pos);
    static double __cdecl GetCurveLength(unsigned int curve);

    static cCurve mCurves[4];
    static int levelNotifyFunc;
};