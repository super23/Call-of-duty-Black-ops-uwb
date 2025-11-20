#include "ui_commands.h"

void __cdecl UI_SetClanName_f()
{
  char *String; // eax

  String = (char *)Dvar_GetString("ui_clantag_new");
  Dvar_SetStringByName("clanName", String);
  Dvar_SetStringByName("ui_clantag_new", (char *)&toastPopupTitle);
  CL_SanitizeClanName();
  Gamer//Profile_UpdateProfileFromDvars(0, PROFILE_WRITE_IF_CHANGED);
  dvar_modifiedFlags |= 2u;
}

