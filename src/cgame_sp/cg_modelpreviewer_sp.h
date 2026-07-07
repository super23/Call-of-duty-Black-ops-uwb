#pragma once

#include <universal/q_shared.h>

void TRACK_cg_modelpreviewer();
void CG_ModelPreviewerStepAnim_f();
char *CG_ModelPreviewerPauseAnim_f();
void CG_ModelPreviewerPauseAnim();
void CG_ModelPreviewerStepAnim(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, double a9, int a10, int a11, int a12);
void CG_ModelPreviewerDrawInfo();
void CG_ModelPreviewerEnumerateAssets(int a1, int a2, int a3, int a4, __int64 a5);
__int64 CG_ModelPreviewerRotateCamera(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, double a9, double a10, int a11, int a12, int a13, int a14);
void *CG_ModelPreviewerZoomCamera(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, double a9, double a10, int a11, int a12, int a13, int a14);
int CG_ModelPreviewerHandleGamepadEvents(unsigned int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, double a9, double a10, double a11, double a12, int a13, int a14, int a15, int a16, int a17, int a18, int a19, int a20);
int CG_ModelPreviewerHandleKeyEvents(unsigned int a1, int a2);
void *CG_ModelPreviewerUpdateView(float *a1, float *a2, float *a3, float *a4);
bool CG_ModelPreviewerNeedsVieworgInterpSkipped(unsigned int a1);
void CG_ModelPreviewerCreateDevGui(unsigned int a1);
void CG_ModelPreviewerDestroyDevGui();
float *CG_ModelPreviewerFrame(float *result);
