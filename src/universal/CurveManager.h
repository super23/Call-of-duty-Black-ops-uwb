#pragma once

int __cdecl cCurveManager::GetFreeCurve();
void __cdecl cCurveManager::AddNodeToCurve(unsigned int curve, float *p);
void __cdecl cCurveManager::AddNodeToCurve(unsigned int curve, float *p, float t);
void __cdecl cCurveManager::SortCurve(unsigned int curve, float *p, bool is_increasing_sort_order);
void __cdecl cCurveManager::BuildCurve(unsigned int curve);
void __cdecl cCurveManager::FreeCurve(unsigned int curve);
void __cdecl cCurveManager::SetCurveBSpline(unsigned int curve);
void __cdecl cCurveManager::SetCurveDraw(unsigned int curve, const float *color);
void __cdecl cCurveManager::GetPos(unsigned int curve, float t, float *p);
double __cdecl cCurveManager::GetCurveLength(unsigned int curve);
