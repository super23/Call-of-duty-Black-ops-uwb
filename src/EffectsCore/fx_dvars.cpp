#include "fx_dvars.h"

void __cdecl FX_RegisterDvars()
{
  fx_enable = _Dvar_RegisterBool("fx_enable", 1, 0x80u, "Toggles all effects processing");
  fx_draw = _Dvar_RegisterBool("fx_draw", 1, 0x80u, "Toggles drawing of effects after processing");
  fx_cull_elem_spawn = _Dvar_RegisterBool("fx_cull_elem_spawn", 1, 0, "Culls effect elems for spawning");
  fx_cull_elem_draw = _Dvar_RegisterBool("fx_cull_elem_draw", 1, 0, "Culls effect elems for drawing");
  fx_cull_effect_spawn = _Dvar_RegisterBool("fx_cull_effect_spawn", 0, 0, "Culls entire effects for spawning");
  fx_marks = _Dvar_RegisterBool("fx_marks", 1, 1u, "Toggles whether bullet hits leave marks");
  fx_marks_smodels = _Dvar_RegisterBool(
                       "fx_marks_smodels",
                       1,
                       1u,
                       "Toggles whether bullet hits leave marks on static models");
  fx_marks_ents = _Dvar_RegisterBool("fx_marks_ents", 1, 1u, "Toggles whether bullet hits leave marks on entities");
  fx_freeze = _Dvar_RegisterBool("fx_freeze", 0, 0x80u, "Freeze effects");
  fx_debugBolt = _Dvar_RegisterFloat("fx_debugBolt", 0.0, 0.0, 100.0, 0x80u, "Debug effects bolt");
  fx_count = _Dvar_RegisterBool("fx_count", 0, 0x80u, "Debug effects count");
  fx_visMinTraceDist = _Dvar_RegisterFloat(
                         "fx_visMinTraceDist",
                         80.0,
                         0.0,
                         1000.0,
                         0x80u,
                         "Minimum visibility trace size");
  fx_profile = _Dvar_RegisterInt(
                 "fx_profile",
                 0,
                 0,
                 1,
                 0x80u,
                 "Turn on FX profiling (specify which local client, with '1' being the first.)");
  fx_mark_profile = _Dvar_RegisterInt(
                      "fx_mark_profile",
                      0,
                      0,
                      1,
                      0x80u,
                      "Turn on FX profiling for marks (specify which local client, with '1' being the first.)");
  fx_drawClouds = _Dvar_RegisterBool("fx_drawClouds", 1, 0x80u, "Toggles the drawing of particle clouds");
  fx_priority_debug = _Dvar_RegisterInt(
                        "fx_priority_debug",
                        0,
                        0,
                        1,
                        0x80u,
                        "Turn on priority debug (specify which local client, with '1' being the first.)");
  fx_seethru_parallax = _Dvar_RegisterBool(
                          "fx_seethru_parallax",
                          1,
                          0x80u,
                          "Toggles the drawing of parallax see-through decals");
  fx_priority_enable = _Dvar_RegisterBool("fx_priority_enable", 0, 0x80u, "Turn on priority");
  fx_draw_weapon_tags = _Dvar_RegisterBool("fx_draw_weapon_tags", 0, 0x80u, "Render weapon tag axis");
  fx_marks_draw_impact_axis = _Dvar_RegisterBool(
                                "fx_marks_draw_impact_axis",
                                0,
                                0x80u,
                                "Render impact mark axis for debugging");
  fx_marks_debug_text = _Dvar_RegisterBool(
                          "fx_marks_debug_text",
                          0,
                          0x80u,
                          "Show addition marks related debugging text");
  fx_marks_range = _Dvar_RegisterFloat(
                     "fx_marks_range",
                     1500.0,
                     0.0,
                     100000.0,
                     0x1000u,
                     "marks will be made only within this range to camera");
}

void __cdecl FX_CreateDevGui()
{
  if ( !fxDevGuiCreated )
  {
    Cbuf_InsertText(0, "exec devgui_fx");
    fxDevGuiCreated = 1;
  }
}

