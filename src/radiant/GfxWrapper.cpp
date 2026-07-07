// GLUE — thin init/shutdown adapter between CoDBORadiant and src/gfx_d3d.
// Decomp: GfxWrapper.cpp:180 (sub_41AEF0 fatal), sub_41B150 init, sub_51C340 shutdown, sub_51C110 attach.
//
// ENGINE_LINK (RAD_LINK_ENGINE=ON) --------------------------------------------
// Project load (sub_41B150 / QE_LoadProject):
//   R_Init() -> R_Register/R_InitGraphicsApi; editor dvars via R_Register inside R_Init
//   Radiant_GfxAttachWindow per viewport HWND (cam/xy/tex/z + console hwnd in decomp)
//   Material_Load for fonts/qerfont, white_tools, $opaque, $additive (sub_519B30/52A510)
// Shutdown (ExitInstance sub_51C340(1)):
//   R_Shutdown(1) — full decomp chain (images/materials/device) still to mirror in EDITOR_PORT
// Viewport draw: r_ed_scene.cpp / r_ed_vertbuf.cpp not in tree yet (manifest GLUE only).
// -----------------------------------------------------------------------------
#include "GfxWrapper.h"

#include "radiant.h"

#ifdef RAD_LINK_ENGINE
#include "gfx_d3d/r_init.h"
#include "gfx_d3d/r_material_load_obj.h"
#endif

static bool Radiant_GfxLoadEditorMaterials(void)
{
#ifdef RAD_LINK_ENGINE
    if (!Material_Load("fonts/qerfont", 7))
        return false;
    if (!Material_Load("white_tools", 0))
        return false;
    if (!Material_Load("$opaque", 0))
        return false;
    if (!Material_Load("$additive", 0))
        return false;
#endif
    return true;
}

bool Radiant_GfxInit(void)
{
#ifdef RAD_LINK_ENGINE
    // Decomp does not call Gfx init from InitInstance; kept for RadiantApp until CMainFrame + project load port.
    R_Init();
    return Material_Load("$default", 0) != nullptr;
#else
    return true;
#endif
}

bool Radiant_GfxInitAfterProject(void)
{
#ifdef RAD_LINK_ENGINE
    R_Init();
    return Radiant_GfxLoadEditorMaterials();
#else
    return true;
#endif
}

bool Radiant_GfxAttachWindow(HWND hwnd)
{
#ifdef RAD_LINK_ENGINE
    (void)hwnd;
    // TODO EDITOR_PORT: GfxWindowParms + R_CreateDevice/R_InitHardware path (decomp sub_51C110 -> sub_51C000)
    return hwnd != nullptr;
#else
    (void)hwnd;
    return true;
#endif
}

void Radiant_GfxShutdown(void)
{
#ifdef RAD_LINK_ENGINE
    R_Shutdown(1);
#endif
}
