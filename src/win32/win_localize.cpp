#include "win_localize.h"

int __cdecl Win_InitLocalization()
{
  signed int size; // [esp+0h] [ebp-10h]
  int sizea; // [esp+0h] [ebp-10h]
  _iobuf *fp; // [esp+4h] [ebp-Ch]
  int i; // [esp+8h] [ebp-8h]
  int lang; // [esp+Ch] [ebp-4h] BYREF

  localization.language = 0;
  localization.strings = 0;
  fp = FS_FileOpenReadText("localization.txt");
  if ( !fp )
    return 0;
  size = FS_FileGetFileSize(fp);
  if ( size >= 4096
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\win32\\win_localize.cpp",
          39,
          0,
          "%s",
          "size < LANGUAGE_BUF_SIZE") )
  {
    __debugbreak();
  }
  localization.language = language_buffer;
  sizea = FS_FileRead(language_buffer, size, fp);
  FS_FileClose(fp);
  if ( sizea )
  {
    localization.language[sizea] = 0;
    lang = 0;
    for ( i = 0; localization.language[i]; ++i )
    {
      if ( localization.language[i] == 10 )
      {
        localization.language[i] = 0;
        localization.strings = &localization.language[i + 1];
        SEH_GetLanguageIndexForName(localization.language, &lang);
        return lang;
      }
    }
    return lang;
  }
  else
  {
    localization.language = 0;
    return 0;
  }
}

void __cdecl Win_ShutdownLocalization()
{
  localization.language = 0;
  localization.strings = 0;
}

char *__cdecl Win_LocalizeRef(const char *ref)
{
  const char *v1; // eax
  const char *v3; // eax
  const char *strings; // [esp+14h] [ebp-Ch] BYREF
  int useRef; // [esp+18h] [ebp-8h]
  const char *token; // [esp+1Ch] [ebp-4h]

  Com_BeginParseSession("localization");
  strings = localization.strings;
  do
  {
    token = (const char *)Com_Parse(&strings);
    if ( !*token )
    {
      Com_EndParseSession();
      v1 = va("unlocalized: %s", ref);
      if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\win32\\win_localize.cpp", 112, 0, v1) )
        __debugbreak();
      return Win_CopyLocalizationString(ref);
    }
    useRef = strcmp(token, ref) == 0;
    token = (const char *)Com_Parse(&strings);
    if ( !*token )
    {
      Com_EndParseSession();
      v3 = va("missing value: %s", ref);
      if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\win32\\win_localize.cpp", 121, 0, v3) )
        __debugbreak();
      return Win_CopyLocalizationString(ref);
    }
  }
  while ( !useRef );
  Com_EndParseSession();
  return Win_CopyLocalizationString(token);
}

char *__cdecl Win_CopyLocalizationString(const char *string)
{
  return va("%s", string);
}

char *__cdecl Win_GetLanguage()
{
  if ( !localization.language
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\win32\\win_localize.cpp",
          140,
          0,
          "%s",
          "localization.language") )
  {
    __debugbreak();
  }
  return localization.language;
}

