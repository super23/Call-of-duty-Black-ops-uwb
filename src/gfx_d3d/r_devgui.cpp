#include "r_devgui.h"

void __cdecl R_CreateDevGui()
{
  Cbuf_InsertText(0, "exec devgui_visibility");
}

