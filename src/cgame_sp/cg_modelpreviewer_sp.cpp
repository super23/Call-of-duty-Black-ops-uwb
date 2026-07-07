#include "cg_modelpreviewer_sp.h"

#include <universal/physicalmemory.h>
#include <qcommon/common.h>

namespace
{
struct ModelPreviewerState
{
    bool devGuiActive;
    bool animPaused;
    bool stepRequested;
    float yawDelta;
    float pitchDelta;
    float zoom;
};

ModelPreviewerState s_modelPreviewerState = { false, false, false, 0.0f, 0.0f, 0.0f };

static float CG_ModelPreviewerClampZoom(float zoom)
{
    if (zoom < -500.0f)
        return -500.0f;
    if (zoom > 500.0f)
        return 500.0f;
    return zoom;
}
}

// Decomp: CoDSP_rdBlackOps.map.c:5558706
void TRACK_cg_modelpreviewer()
{
    PMem_BeginAlloc("cg_modelpreviewer", 1u, TRACK_RENDERER_MISC);
}

// BlackOps SP fallback: keep previewer callable without devgui side effects.
void CG_ModelPreviewerStepAnim_f()
{
    CG_ModelPreviewerStepAnim(0, 0, 0, 0, 0, 0, 0, 0, 0.0, 0, 0, 0);
}

char *CG_ModelPreviewerPauseAnim_f()
{
    CG_ModelPreviewerPauseAnim();
    return (char *)"";
}

void CG_ModelPreviewerPauseAnim()
{
    if (!s_modelPreviewerState.devGuiActive)
        return;

    s_modelPreviewerState.animPaused = !s_modelPreviewerState.animPaused;
}

void CG_ModelPreviewerStepAnim(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, double a9, int a10, int a11, int a12)
{
    (void)a1; (void)a2; (void)a3; (void)a4; (void)a5; (void)a6;
    (void)a7; (void)a8; (void)a9; (void)a10; (void)a11; (void)a12;
    if (!s_modelPreviewerState.devGuiActive)
        return;

    s_modelPreviewerState.animPaused = true;
    s_modelPreviewerState.stepRequested = true;
}

void CG_ModelPreviewerDrawInfo()
{
    if (!s_modelPreviewerState.devGuiActive)
        return;

    Com_Printf(
        14,
        "ModelPreviewer: paused=%d step=%d zoom=%0.2f yaw=%0.2f pitch=%0.2f\n",
        s_modelPreviewerState.animPaused ? 1 : 0,
        s_modelPreviewerState.stepRequested ? 1 : 0,
        s_modelPreviewerState.zoom,
        s_modelPreviewerState.yawDelta,
        s_modelPreviewerState.pitchDelta);
}

void CG_ModelPreviewerEnumerateAssets(int a1, int a2, int a3, int a4, __int64 a5)
{
    (void)a1; (void)a2; (void)a3; (void)a4; (void)a5;
}

__int64 CG_ModelPreviewerRotateCamera(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, double a9, double a10, int a11, int a12, int a13, int a14)
{
    (void)a1; (void)a2; (void)a3; (void)a4; (void)a5; (void)a6; (void)a7; (void)a8;
    (void)a11; (void)a12; (void)a13; (void)a14;
    if (!s_modelPreviewerState.devGuiActive)
        return 0;

    s_modelPreviewerState.yawDelta += (float)a9;
    s_modelPreviewerState.pitchDelta += (float)a10;
    return s_modelPreviewerState.devGuiActive ? 1 : 0;
}

void *CG_ModelPreviewerZoomCamera(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, double a9, double a10, int a11, int a12, int a13, int a14)
{
    (void)a1; (void)a2; (void)a3; (void)a4; (void)a5; (void)a6; (void)a7; (void)a8;
    (void)a10; (void)a11; (void)a12; (void)a13; (void)a14;
    if (!s_modelPreviewerState.devGuiActive)
        return &s_modelPreviewerState;

    s_modelPreviewerState.zoom = CG_ModelPreviewerClampZoom(s_modelPreviewerState.zoom + (float)a9);
    return &s_modelPreviewerState;
}

int CG_ModelPreviewerHandleGamepadEvents(unsigned int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, double a9, double a10, double a11, double a12, int a13, int a14, int a15, int a16, int a17, int a18, int a19, int a20)
{
    (void)a2; (void)a3; (void)a4; (void)a5; (void)a6; (void)a7; (void)a8;
    (void)a13; (void)a14; (void)a15; (void)a16; (void)a17; (void)a18; (void)a19; (void)a20;
    if (!s_modelPreviewerState.devGuiActive)
        return 0;

    CG_ModelPreviewerRotateCamera(a1, 0, 0, 0, 0, 0, 0, 0, a9, a10, 0, 0, 0, 0);
    CG_ModelPreviewerZoomCamera(a1, 0, 0, 0, 0, 0, 0, 0, a11 + a12, 0.0, 0, 0, 0, 0);
    return 1;
}

int CG_ModelPreviewerHandleKeyEvents(unsigned int a1, int a2)
{
    (void)a1;
    if (!s_modelPreviewerState.devGuiActive)
        return 0;

    if (a2 == ' ')
    {
        CG_ModelPreviewerPauseAnim();
        return 1;
    }

    if (a2 == '.')
    {
        CG_ModelPreviewerStepAnim_f();
        return 1;
    }

    return 0;
}

void *CG_ModelPreviewerUpdateView(float *a1, float *a2, float *a3, float *a4)
{
    (void)a2; (void)a4;
    if (a3)
        a3[0] += s_modelPreviewerState.zoom;
    return a1;
}

bool CG_ModelPreviewerNeedsVieworgInterpSkipped(unsigned int a1)
{
    (void)a1;
    return s_modelPreviewerState.devGuiActive && (s_modelPreviewerState.animPaused || s_modelPreviewerState.stepRequested);
}

void CG_ModelPreviewerCreateDevGui(unsigned int a1)
{
    s_modelPreviewerState.devGuiActive = true;
    s_modelPreviewerState.animPaused = false;
    s_modelPreviewerState.stepRequested = false;
    s_modelPreviewerState.yawDelta = 0.0f;
    s_modelPreviewerState.pitchDelta = 0.0f;
    s_modelPreviewerState.zoom = 0.0f;
}

void CG_ModelPreviewerDestroyDevGui()
{
    s_modelPreviewerState.devGuiActive = false;
    s_modelPreviewerState.animPaused = false;
    s_modelPreviewerState.stepRequested = false;
    s_modelPreviewerState.yawDelta = 0.0f;
    s_modelPreviewerState.pitchDelta = 0.0f;
    s_modelPreviewerState.zoom = 0.0f;
}

// Decomp: CoDSP_rdBlackOps.map.c:5565663
float *CG_ModelPreviewerFrame(float *result)
{
    if (s_modelPreviewerState.stepRequested)
    {
        s_modelPreviewerState.stepRequested = false;
    }

    return result;
}
