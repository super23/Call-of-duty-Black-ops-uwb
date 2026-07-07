// GLUE — D3D preview adapter to src/gfx_d3d (R_Init, r_ed_*, Material_*).
// Decomp: CoDBORadiant.decompile.c — Radiant/GfxWrapper.cpp (assert line 180)
#pragma once

#ifndef KISAK_RADIANT_EDITOR
#error "src/radiant requires KISAK_RADIANT_EDITOR=1"
#endif

#include <stdbool.h>
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

// Decomp mapping (RAD_LINK_ENGINE):
//   sub_41B150 (QE_LoadProject)  -> Radiant_GfxInitAfterProject()
//   sub_51C110(HWND) / sub_51C000 -> Radiant_GfxAttachWindow()  [r_init.cpp attach path]
//   sub_41AEF0 / GfxWrapper:180    -> Radiant_Error on DX failure
//   sub_51C340(1) (ExitInstance)   -> Radiant_GfxShutdown()
//
// sub_51C340 is shutdown-only when gfx was initialized; it is not GfxWrapper startup.
// Viewport HWNDs (dword_27E184C etc.) come from CCamWnd/CXYWnd/CTexWnd children, not CMainFrame::m_hWnd.
//
// Default build (RAD_LINK_ENGINE off): stubs return immediately.

bool Radiant_GfxInit(void); // legacy early hook; prefer InitAfterProject once CMainFrame/viewports exist
bool Radiant_GfxInitAfterProject(void);
bool Radiant_GfxAttachWindow(HWND hwnd);
void Radiant_GfxShutdown(void);

#ifdef __cplusplus
}
#endif
