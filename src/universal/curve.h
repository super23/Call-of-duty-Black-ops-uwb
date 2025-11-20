#pragma once

cCurve *__thiscall cCurve::cCurve(cCurve *this);
void __cdecl GetPositionOnCubic(
        float *startPos,
        float *startVel,
        float *endPos,
        float *endVel,
        float time,
        float *output);
void __cdecl GetPositionOnSpine(float *p1, float *p2, float *p3, float *p4, float time, float *output);
void __thiscall cCurve::Reset(cCurve *this);
void __thiscall cCurve::Reinit(cCurve *this);
void __thiscall cCurve::AddNode(cCurve *this, float *p);
void __thiscall cCurve::AddNode(cCurve *this, float *p, float timePeriod);
void __thiscall cCurve::GetPosition(cCurve *this, float time, float *pos);
void __thiscall cCurve::GetStartVelocity(cCurve *this, int index, float *vel);
void __thiscall cCurve::GetEndVelocity(cCurve *this, int index, float *vel);
int __cdecl cCurve::CurveSortCompare(const float *e1, const float *e2);
void __thiscall cCurve::Sort(cCurve *this, float *p, bool is_increasing_sort_order);
void __thiscall cCurve::BuildNonUniform(cCurve *this);
void __thiscall cCurve::BuildBSpline(cCurve *this);
void __thiscall cCurve::Build(cCurve *this);
void __thiscall cCurve::Smooth(cCurve *this);
void __thiscall cCurve::Constrain(cCurve *this);
void __thiscall cCurve::GetPos(cCurve *this, float t, float *p);
double __thiscall cCurve::GetLength(cCurve *this);
void __thiscall cCurve::SetDraw(cCurve *this, const float *userColor);
