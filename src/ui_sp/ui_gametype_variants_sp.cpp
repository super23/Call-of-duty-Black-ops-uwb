#include "ui_gametype_variants_sp.h"
#include <universal/dvar.h>

const dvar_t *ui_gv_reloadSpeedModifier;

void UI_GametypeVariants_RegisterDvars_SP()
{
    ui_gv_reloadSpeedModifier = _Dvar_RegisterFloat(
        "ui_gv_reloadSpeedModifier",
        0.0f,
        0.0f,
        4.0f,
        0,
        "Adjusts the reload speed");
}

// SP: gametype variant rules are MP UI; script hook is a no-op.
void __cdecl Scr_GameVariants_GetRule()
{
}
