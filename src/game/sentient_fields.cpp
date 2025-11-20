#include "sentient_fields.h"

void __cdecl SentientScr_ReadOnly(sentient_s *pSelf, const sentient_fields_s *pField)
{
  const char *v2; // eax

  if ( !pSelf && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\sentient_fields.cpp", 67, 0, "%s", "pSelf") )
    __debugbreak();
  v2 = va("sentient property '%s' is read-only", pField->name);
  Scr_Error(v2, 0);
}

void __cdecl SentientScr_SetTeam(sentient_s *pSelf)
{
  const char *v1; // eax
  char *pszTeam; // [esp+0h] [ebp-4h]

  if ( !pSelf && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\sentient_fields.cpp", 79, 0, "%s", "pSelf") )
    __debugbreak();
  pszTeam = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
  if ( I_stricmp(pszTeam, "axis") )
  {
    if ( I_stricmp(pszTeam, "allies") )
    {
      if ( I_stricmp(pszTeam, "spectator") )
      {
        if ( I_stricmp(pszTeam, "free") )
        {
          v1 = va("unknown team '%s', should be axis, allies, or neutral\n", pszTeam);
          Scr_Error(v1, 0);
        }
        else
        {
          Sentient_SetTeam(pSelf, TEAM_FREE);
        }
      }
      else
      {
        Sentient_SetTeam(pSelf, TEAM_SPECTATOR);
      }
    }
    else
    {
      Sentient_SetTeam(pSelf, TEAM_ALLIES);
    }
  }
  else
  {
    Sentient_SetTeam(pSelf, TEAM_AXIS);
  }
}

void __cdecl SentientScr_GetTeam(sentient_s *pSelf)
{
  if ( !pSelf
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\sentient_fields.cpp", 108, 0, "%s", "pSelf") )
  {
    __debugbreak();
  }
  if ( pSelf->eTeam >= (unsigned int)TEAM_NUM_TEAMS
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\sentient_fields.cpp",
          110,
          0,
          "%s",
          "pSelf->eTeam >= TEAM_FREE && pSelf->eTeam < TEAM_NUM_TEAMS") )
  {
    __debugbreak();
  }
  switch ( pSelf->eTeam )
  {
    case TEAM_FREE:
      Scr_AddString("free", SCRIPTINSTANCE_SERVER);
      break;
    case TEAM_AXIS:
      Scr_AddString("axis", SCRIPTINSTANCE_SERVER);
      break;
    case TEAM_ALLIES:
      Scr_AddString("allies", SCRIPTINSTANCE_SERVER);
      break;
    case TEAM_SPECTATOR:
      Scr_AddString("spectator", SCRIPTINSTANCE_SERVER);
      break;
    default:
      if ( !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\game\\sentient_fields.cpp",
              139,
              0,
              "SentientScr_GetTeam: default case (shouldn't happen") )
        __debugbreak();
      break;
  }
}

void __cdecl GScr_AddFieldsForSentient()
{
  const sentient_fields_s *f; // [esp+4h] [ebp-4h]

  for ( f = fields_2; f->name; ++f )
  {
    if ( ((f - fields_2) & 0xC000) != 0
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\sentient_fields.cpp",
            158,
            0,
            "%s",
            "!((f - fields) & ENTFIELD_MASK)") )
    {
      __debugbreak();
    }
    if ( f - fields_2 != (unsigned __int16)(f - fields_2)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\sentient_fields.cpp",
            159,
            0,
            "%s",
            "(f - fields) == (unsigned short)( f - fields )") )
    {
      __debugbreak();
    }
    Scr_AddClassField(0, (char *)f->name, (unsigned __int16)(f - fields_2) | 0x4000, SCRIPTINSTANCE_SERVER);
  }
}

void __cdecl Scr_SetSentientField(sentient_s *sentient, unsigned int offset)
{
  const sentient_fields_s *f; // [esp+0h] [ebp-4h]

  if ( !sentient
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\sentient_fields.cpp", 175, 0, "%s", "sentient") )
  {
    __debugbreak();
  }
  if ( offset >= 0x13
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\sentient_fields.cpp",
          176,
          0,
          "%s",
          "(unsigned)offset < ARRAY_COUNT( fields ) - 1") )
  {
    __debugbreak();
  }
  f = &fields_2[offset];
  if ( f->setter )
    f->setter(sentient, f);
  else
    GScr_SetGenericField((unsigned __int8 *)sentient, f->type, f->ofs, 0);
}

void __cdecl Scr_GetSentientField(sentient_s *sentient, unsigned int offset)
{
  const sentient_fields_s *f; // [esp+0h] [ebp-4h]

  if ( !sentient
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\sentient_fields.cpp", 199, 0, "%s", "sentient") )
  {
    __debugbreak();
  }
  if ( offset >= 0x13
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\sentient_fields.cpp",
          200,
          0,
          "%s",
          "(unsigned)offset < ARRAY_COUNT( fields ) - 1") )
  {
    __debugbreak();
  }
  f = &fields_2[offset];
  if ( f->getter )
    f->getter(sentient, f);
  else
    GScr_GetGenericField((unsigned __int8 *)sentient, f->type, f->ofs, 0);
}

