#pragma once
#include <ui_sp/ui_gametype_custom_sp.h>

// SP: gametype variant dvars referenced by shared bgame/cgame code.
extern const dvar_t *ui_gv_reloadSpeedModifier;

void UI_GametypeVariants_RegisterDvars_SP();
void __cdecl Scr_GameVariants_GetRule();
