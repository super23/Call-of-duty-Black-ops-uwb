#include "CurveManager.h"
#include <win32/win_common.h>
#include "assertive.h"
#include <universal/com_math.h>
#include <bgame/bg_public.h>

// really dont see much point to this class, guys

cCurve cCurveManager::mCurves[4];
int cCurveManagerLLlevelNotifyFunc;

int __cdecl cCurveManager::GetFreeCurve()
{
    int i; // [esp+4h] [ebp-4h]

    Sys_EnterCriticalSection(CRITSECT_CURVEALLOC);
    for ( i = 0; i < 4; ++i )
    {
        if ( !cCurveManager::mCurves[i].mActive )
        {
            cCurveManager::mCurves[i].mActive = 1;
            Sys_LeaveCriticalSection(CRITSECT_CURVEALLOC);
            return i;
        }
    }
    Sys_LeaveCriticalSection(CRITSECT_CURVEALLOC);
    if ( !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    94,
                    0,
                    "CurveManager - No unused curves available.") )
        __debugbreak();
    return -1;
}

void __cdecl cCurveManager::AddNodeToCurve(unsigned int curve, float *p)
{
    if ( curve >= 4
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    101,
                    0,
                    "%s",
                    "curve >= 0 && curve < MAX_CURVES") )
    {
        __debugbreak();
    }
    if ( !cCurveManager::mCurves[curve].mActive
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    102,
                    0,
                    "%s",
                    "mCurves[curve].IsActive()") )
    {
        __debugbreak();
    }
    //cCurve::AddNode(&cCurveManager::mCurves[curve], p);
    mCurves[curve].AddNode(p);
}

void __cdecl cCurveManager::AddNodeToCurve(unsigned int curve, float *p, float t)
{
    if ( curve >= 4
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    109,
                    0,
                    "%s",
                    "curve >= 0 && curve < MAX_CURVES") )
    {
        __debugbreak();
    }
    if ( !cCurveManager::mCurves[curve].mActive
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    110,
                    0,
                    "%s",
                    "mCurves[curve].IsActive()") )
    {
        __debugbreak();
    }
    //cCurve::AddNode(&cCurveManager::mCurves[curve], p, t);
    mCurves[curve].AddNode(p, t);
}

void __cdecl cCurveManager::SortCurve(unsigned int curve, float *p, bool is_increasing_sort_order)
{
    if ( curve >= 4
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    117,
                    0,
                    "%s",
                    "curve >= 0 && curve < MAX_CURVES") )
    {
        __debugbreak();
    }
    if ( !cCurveManager::mCurves[curve].mActive
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    118,
                    0,
                    "%s",
                    "mCurves[curve].IsActive()") )
    {
        __debugbreak();
    }
    //cCurve::Sort(&cCurveManager::mCurves[curve], p, is_increasing_sort_order);
    mCurves[curve].Sort(p, is_increasing_sort_order);
}

void __cdecl cCurveManager::BuildCurve(unsigned int curve)
{
    if ( curve >= 4
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    125,
                    0,
                    "%s",
                    "curve >= 0 && curve < MAX_CURVES") )
    {
        __debugbreak();
    }
    if ( !cCurveManager::mCurves[curve].mActive
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    126,
                    0,
                    "%s",
                    "mCurves[curve].IsActive()") )
    {
        __debugbreak();
    }
    //cCurve::Build(&cCurveManager::mCurves[curve]);
    mCurves[curve].Build();
}

void __cdecl cCurveManager::StartCurve(unsigned int curve)
{
    if ( curve >= 4
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    141,
                    0,
                    "%s",
                    "curve >= 0 && curve < MAX_CURVES") )
    {
        __debugbreak();
    }
    if ( !cCurveManager::mCurves[curve].mActive
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    142,
                    0,
                    "%s",
                    "mCurves[curve].IsActive()") )
    {
        __debugbreak();
    }
    mCurves[curve].mPaused = false;
}

void __cdecl cCurveManager::ResetCurve(unsigned int curve)
{
    if ( curve >= 4
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    0xCD,
                    0,
                    "%s",
                    "curve >= 0 && curve < MAX_CURVES") )
    {
        __debugbreak();
    }
    if ( !cCurveManager::mCurves[curve].mActive
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    0xCE,
                    0,
                    "%s",
                    "mCurves[curve].IsActive()") )
    {
        __debugbreak();
    }
    cCurveManager::mCurves[curve].ResetCurve();
}

void __cdecl cCurveManager::StopCurve(unsigned int curve)
{
    if ( curve >= 4
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    0xBD,
                    0,
                    "%s",
                    "curve >= 0 && curve < MAX_CURVES") )
    {
        __debugbreak();
    }
    if ( !cCurveManager::mCurves[curve].mActive
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    0xBE,
                    0,
                    "%s",
                    "mCurves[curve].IsActive()") )
    {
        __debugbreak();
    }
    mCurves[curve].mPaused = true;
}

void __cdecl cCurveManager::FreeCurve(unsigned int curve)
{
    if ( curve >= 4
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    133,
                    0,
                    "%s",
                    "curve >= 0 && curve < MAX_CURVES") )
    {
        __debugbreak();
    }
    if ( !cCurveManager::mCurves[curve].mActive
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    134,
                    0,
                    "%s",
                    "mCurves[curve].IsActive()") )
    {
        __debugbreak();
    }
    //cCurve::Reset(&cCurveManager::mCurves[curve]);
    mCurves[curve].Reset();
}

static void __cdecl cCurveManager_ValidateCurve(unsigned int curve)
{
    if ( curve >= 4
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    165,
                    0,
                    "%s",
                    "curve >= 0 && curve < MAX_CURVES") )
    {
        __debugbreak();
    }
    if ( !cCurveManager::mCurves[curve].mActive
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    166,
                    0,
                    "%s",
                    "mCurves[curve].IsActive()") )
    {
        __debugbreak();
    }
}

void __cdecl cCurveManager::SetCurveBSpline(unsigned int curve)
{
    cCurveManager_ValidateCurve(curve);
    mCurves[curve].mCurveType = cCurve::eCurveType::CURVE_BSPLINE;
}

void __cdecl cCurveManager::SetCurveRounded(unsigned int curve)
{
    cCurveManager_ValidateCurve(curve);
    mCurves[curve].mCurveType = cCurve::eCurveType::CURVE_RNS;
}

void __cdecl cCurveManager::SetCurveSmooth(unsigned int curve)
{
    cCurveManager_ValidateCurve(curve);
    mCurves[curve].mCurveType = cCurve::eCurveType::CURVE_SNS;
}

void __cdecl cCurveManager::SetCurveSpeed(unsigned int curve, float speed)
{
    cCurveManager_ValidateCurve(curve);
    mCurves[curve].mSpeed = speed;
}

void __cdecl cCurveManager::SetNotifyEnt(unsigned int curve, int entnum)
{
    cCurveManager_ValidateCurve(curve);
    mCurves[curve].mNotifyLevel = false;
    mCurves[curve].mNotifyEntNum = entnum;
}

void __cdecl cCurveManager::SetNotifyLevel(unsigned int curve)
{
    cCurveManager_ValidateCurve(curve);
    mCurves[curve].mNotifyLevel = true;
    mCurves[curve].mNotifyEntNum = 959;
}

void __cdecl cCurveManager::SetCameraEnt(unsigned int curve, int entnum)
{
    cCurveManager_ValidateCurve(curve);
    mCurves[curve].mCameraEntNum = entnum;
}

void __cdecl cCurveManager::SetCurveDraw(unsigned int curve, const float *color)
{
    if ( curve >= 4
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    173,
                    0,
                    "%s",
                    "curve >= 0 && curve < MAX_CURVES") )
    {
        __debugbreak();
    }
    if ( !cCurveManager::mCurves[curve].mActive
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    174,
                    0,
                    "%s",
                    "mCurves[curve].IsActive()") )
    {
        __debugbreak();
    }
    //cCurve::SetDraw(&cCurveManager::mCurves[curve], color);
    mCurves[curve].SetDraw(color);
}

void __cdecl cCurveManager::GetPos(unsigned int curve, float t, float *p)
{
    if ( curve >= 4
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    277,
                    0,
                    "%s",
                    "curve >= 0 && curve < MAX_CURVES") )
    {
        __debugbreak();
    }
    if ( !cCurveManager::mCurves[curve].mActive
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    278,
                    0,
                    "%s",
                    "mCurves[curve].IsActive()") )
    {
        __debugbreak();
    }
    //cCurve::GetPos(&cCurveManager::mCurves[curve], t, p);
    mCurves[curve].GetPos(t, p);

    nanassertvec3(p);
}

void __cdecl cCurveManager::GetCurClientPos(unsigned int curve, float *pos)
{
    if ( curve >= 4
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    285,
                    0,
                    "%s",
                    "curve >= 0 && curve < MAX_CURVES") )
    {
        __debugbreak();
    }
    if ( !cCurveManager::mCurves[curve].mActive
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    286,
                    0,
                    "%s",
                    "mCurves[curve].IsActive()") )
    {
        __debugbreak();
    }
    mCurves[curve].GetCurClientPos(pos);
    nanassertvec3(pos);
}

void __cdecl cCurveManager::GetCurServerPos(unsigned int curve, float *pos)
{
    if ( curve >= 4
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    293,
                    0,
                    "%s",
                    "curve >= 0 && curve < MAX_CURVES") )
    {
        __debugbreak();
    }
    if ( !cCurveManager::mCurves[curve].mActive
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    294,
                    0,
                    "%s",
                    "mCurves[curve].IsActive()") )
    {
        __debugbreak();
    }
    mCurves[curve].GetCurServerPos(pos);
    nanassertvec3(pos);
}

double __cdecl cCurveManager::GetCurveLength(unsigned int curve)
{
    if ( curve >= 4
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    287,
                    0,
                    "%s",
                    "curve >= 0 && curve < MAX_CURVES") )
    {
        __debugbreak();
    }
    if ( !cCurveManager::mCurves[curve].mActive
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\universal\\CurveManager.cpp",
                    288,
                    0,
                    "%s",
                    "mCurves[curve].IsActive()") )
    {
        __debugbreak();
    }
    //return cCurve::GetLength(&cCurveManager::mCurves[curve]);

    return mCurves[curve].GetLength();
}

