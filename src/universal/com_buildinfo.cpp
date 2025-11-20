#include "com_buildinfo.h"

char *__cdecl Com_GetBuildVersion()
{
  if ( CHANGELIST_NUMBER )
    sprintf(buf, "%s.%s.%d CL(%d) %s %s", "7", "0", BUILD_NUMBER, CHANGELIST_NUMBER, BUILD_MACHINE, BUILD_TIME);
  else
    sprintf(buf, "%s.%s.%d %s %s", "7", "0", BUILD_NUMBER, BUILD_MACHINE, BUILD_TIME);
  return buf;
}

int __cdecl Com_GetBuildNumber()
{
  return BUILD_NUMBER;
}

const char *__cdecl Com_GetBuildDisplayName()
{
  return "Call of Duty Multiplayer - Release";
}

const char *__cdecl Com_GetBuildOfficialNameR()
{
  char *Language; // eax

  Language = Win_GetLanguage();
  _stricmp(Language, "japanese");
  return aCallOfDuty_1;
}

const char *__cdecl Com_GetBuildDisplayNameR()
{
  char *Language; // eax

  Language = Win_GetLanguage();
  if ( !_stricmp(Language, "japanese") )
    return "Call of Duty(R) Multiplayer - Release";
  else
    return aCallOfDuty;
}

const char *__cdecl Com_GetBuildName()
{
  return "COD_T5_R MP";
}

int __cdecl Com_GetChangelist()
{
  return CHANGELIST_NUMBER;
}

