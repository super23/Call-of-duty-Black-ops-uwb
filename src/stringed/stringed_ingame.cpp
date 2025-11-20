#include "stringed_ingame.h"

int __cdecl LocalizeName::GetType()
{
  return 34;
}

char *__cdecl LocalizeStringName::GetName()
{
  return "LocalizeString";
}

void __thiscall CStringEdPackage::Clear(CStringEdPackage *this)
{
  std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::less<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>>,Allocator<std::pair<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>>,mapStringEntriesName_t>,0>>::clear(&this->m_StringEntries);
  this->m_bEndMarkerFound_ParseOnly = 0;
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::assign(
    &this->m_strCurrentEntryRef_ParseOnly,
    &toastPopupTitle,
    strlen(&toastPopupTitle));
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::assign(
    &this->m_strCurrentEntryEnglish_ParseOnly,
    &toastPopupTitle,
    strlen(&toastPopupTitle));
}

char *__thiscall CStringEdPackage::Filename_WithoutExt(CStringEdPackage *this, const char *psFilename)
{
  char *v2; // eax
  char *v3; // eax
  unsigned int v4; // eax
  char v6; // [esp+3h] [ebp-1Dh]
  char *v7; // [esp+8h] [ebp-18h]
  char *p2; // [esp+18h] [ebp-8h]
  char *p; // [esp+1Ch] [ebp-4h]

  v7 = sString;
  do
  {
    v6 = *psFilename;
    *v7++ = *psFilename++;
  }
  while ( v6 );
  strrchr((unsigned __int8 *)sString, '.');
  p = v2;
  strrchr((unsigned __int8 *)sString, '\\');
  p2 = v3;
  strrchr((unsigned __int8 *)sString, '/');
  if ( p && (!p2 || p > p2) && (!v4 || (unsigned int)p > v4) )
    *p = 0;
  return sString;
}

char *__thiscall CStringEdPackage::Filename_WithoutPath(CStringEdPackage *this, const char *psFilename)
{
  char v2; // dl
  char *v4; // [esp+8h] [ebp-10h]
  const char *v5; // [esp+Ch] [ebp-Ch]
  const char *psCopyPos; // [esp+14h] [ebp-4h]

  psCopyPos = psFilename;
  while ( *psFilename )
  {
    if ( *psFilename == 47 || *psFilename == 92 )
      psCopyPos = psFilename + 1;
    ++psFilename;
  }
  v5 = psCopyPos;
  v4 = sString_0;
  do
  {
    v2 = *v5;
    *v4++ = *v5++;
  }
  while ( v2 );
  return sString_0;
}

void __thiscall CStringEdPackage::SetupNewFileParse(CStringEdPackage *this, const char *psFileName)
{
  const char *v2; // eax
  char v3; // al
  char *v4; // [esp+18h] [ebp-94h]
  char *v5; // [esp+1Ch] [ebp-90h]
  char sString[68]; // [esp+64h] [ebp-48h] BYREF

  v2 = CStringEdPackage::Filename_WithoutExt(this, psFileName);
  v5 = CStringEdPackage::Filename_WithoutPath(this, v2);
  v4 = sString;
  do
  {
    v3 = *v5;
    *v4++ = *v5++;
  }
  while ( v3 );
  I_strupr(sString);
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::assign(
    &this->m_strCurrentFileRef_ParseOnly,
    sString,
    strlen(sString));
}

int __thiscall CStringEdPackage::CheckLineForKeyword(
        CStringEdPackage *this,
        const char *psKeyword,
        const char **psLine)
{
  if ( I_strnicmp(psKeyword, *psLine, strlen(psKeyword)) )
    return 0;
  for ( *psLine += strlen(psKeyword); **psLine == 9 || **psLine == 32; ++*psLine )
    ;
  return 1;
}

char __thiscall CStringEdPackage::IsStringFormatCorrect(CStringEdPackage *this, char *string)
{
  const char *v2; // eax
  const char *v4; // eax
  CStringEdPackage *thisa; // [esp+0h] [ebp-1Ch]
  int argIndex; // [esp+4h] [ebp-18h]
  const char *convString; // [esp+8h] [ebp-14h]
  bool args[12]; // [esp+Ch] [ebp-10h] BYREF

  thisa = this;
  memset(args, 0, 9);
  strstr((unsigned __int8 *)string, AMPERSAND_2X);
  for ( convString = v2; convString; convString = v4 )
  {
    convString += 2;
    if ( !isdigit(*convString) )
      return 0;
    argIndex = *convString - 48;
    if ( args[--argIndex] )
      return 0;
    args[argIndex] = 1;
    strstr((unsigned __int8 *)++convString, AMPERSAND_2X);
  }
  return 1;
}

std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > *__thiscall CStringEdPackage::ConvertCRLiterals_Read(
        CStringEdPackage *this,
        std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > *result,
        std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > string)
{
  unsigned __int64 v4; // [esp-8h] [ebp-114h]
  unsigned __int64 v5; // [esp-8h] [ebp-114h]
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *v6; // [esp+8h] [ebp-104h]
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *v7; // [esp+20h] [ebp-ECh]
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *Ptr; // [esp+24h] [ebp-E8h]
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *p_Bx; // [esp+38h] [ebp-D4h]
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > str; // [esp+BCh] [ebp-50h] BYREF
  bool validButton; // [esp+DBh] [ebp-31h]
  int loc; // [esp+DCh] [ebp-30h]
  char buttonName[32]; // [esp+E0h] [ebp-2Ch] BYREF
  int nameLen; // [esp+100h] [ebp-Ch]
  int i; // [esp+104h] [ebp-8h]

  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(&str, 0, 0);
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::assign(
    &str,
    &string,
    0,
    std::string::npos);
  while ( 1 )
  {
    HIDWORD(v4) = strlen("\\n");
    LODWORD(v4) = 0;
    loc = std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::find(&str, "\\n", v4);
    if ( loc == -1 )
      break;
    if ( loc > str._Mysize )
      _invalid_parameter_noinfo();
    if ( str._Myres < 0x10 )
      p_Bx = &str._Bx;
    else
      p_Bx = (std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *)str._Bx._Ptr;
    p_Bx->_Buf[loc] = 10;
    std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::erase(
      (std::string *)&str,
      loc + 1,
      1u);
  }
  while ( 1 )
  {
    HIDWORD(v5) = strlen("&&\"");
    LODWORD(v5) = 0;
    loc = std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::find(&str, "&&\"", v5);
    if ( loc == -1 )
      break;
    validButton = 0;
    nameLen = 0;
    i = loc + 3;
    if ( loc + 3 > str._Mysize )
      _invalid_parameter_noinfo();
    if ( str._Myres < 0x10 )
      Ptr = &str._Bx;
    else
      Ptr = (std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *)str._Bx._Ptr;
    for ( buttonName[nameLen] = Ptr->_Buf[i]; buttonName[nameLen] != 34; buttonName[nameLen] = v7->_Buf[i] )
    {
      ++i;
      ++nameLen;
      if ( i > str._Mysize )
        _invalid_parameter_noinfo();
      if ( str._Myres < 0x10 )
        v7 = &str._Bx;
      else
        v7 = (std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *)str._Bx._Ptr;
    }
    buttonName[nameLen] = 0;
    for ( i = 0; btnMapping[i].btnString; ++i )
    {
      if ( !strcmp(buttonName, btnMapping[i].btnString) )
      {
        if ( loc > str._Mysize )
          _invalid_parameter_noinfo();
        if ( str._Myres < 0x10 )
          v6 = &str._Bx;
        else
          v6 = (std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *)str._Bx._Ptr;
        v6->_Buf[loc] = dword_E0B904[2 * i];
        std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::erase(
          (std::string *)&str,
          loc + 1,
          nameLen + 3);
        validButton = 1;
        break;
      }
    }
    if ( !validButton )
    {
      if ( str._Myres < 0x10 )
        Com_PrintError(
          16,
          "ERROR: Localized string '%s' has an invalid button-to-font mapping: '%s'\n",
          str._Bx._Buf,
          buttonName);
      else
        Com_PrintError(
          16,
          "ERROR: Localized string '%s' has an invalid button-to-font mapping: '%s'\n",
          str._Bx._Ptr,
          buttonName);
      break;
    }
    if ( dword_E0B904[2 * i] != 26 )
    {
      if ( string._Myres < 0x10 )
        Com_PrintWarning(
          16,
          "WARNING: btnMapping in localized string '%s' will not work with button binds\n",
          string._Bx._Buf);
      else
        Com_PrintWarning(
          16,
          "WARNING: btnMapping in localized string '%s' will not work with button binds\n",
          string._Bx._Ptr);
    }
  }
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(result, 0, 0);
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::assign(
    result,
    &str,
    0,
    std::string::npos);
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(&str, 1, 0);
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(&string, 1, 0);
  return result;
}

void __thiscall CStringEdPackage::REMKill(CStringEdPackage *this, char *psBuffer)
{
  _BYTE *v2; // eax
  int iWhiteSpaceScanPos; // [esp+14h] [ebp-18h]
  int i; // [esp+18h] [ebp-14h]
  int iDoubleQuoteCount; // [esp+1Ch] [ebp-10h]
  int iDoubleQuotesSoFar; // [esp+20h] [ebp-Ch]

  for ( iDoubleQuotesSoFar = 0; ; iDoubleQuotesSoFar = iDoubleQuoteCount )
  {
    strstr((unsigned __int8 *)psBuffer, "//");
    if ( !v2 )
      break;
    iDoubleQuoteCount = iDoubleQuotesSoFar;
    for ( i = 0; i < v2 - psBuffer; ++i )
    {
      if ( psBuffer[i] == 34 )
        ++iDoubleQuoteCount;
    }
    if ( (iDoubleQuoteCount & 1) == 0 )
    {
      *v2 = 0;
      if ( *psBuffer )
      {
        for ( iWhiteSpaceScanPos = strlen(psBuffer) - 1;
              iWhiteSpaceScanPos >= 0 && isspace(psBuffer[iWhiteSpaceScanPos]);
              --iWhiteSpaceScanPos )
        {
          psBuffer[iWhiteSpaceScanPos] = 0;
        }
      }
      return;
    }
    psBuffer = v2 + 1;
  }
}

int __thiscall CStringEdPackage::ReadLine(CStringEdPackage *this, const char **psParsePos, char *psDest)
{
  int v3; // eax
  int v4; // eax
  unsigned __int8 v5; // cl
  char *v7; // [esp+28h] [ebp-18h]
  unsigned __int8 *v8; // [esp+2Ch] [ebp-14h]
  int iWhiteSpaceScanPos; // [esp+34h] [ebp-Ch]
  unsigned int iCharsToCopy; // [esp+38h] [ebp-8h]

  if ( !**psParsePos )
    return 0;
  strchr((unsigned __int8 *)*psParsePos, '\n');
  if ( v3 )
  {
    iCharsToCopy = v3 - (unsigned int)*psParsePos;
    strncpy((unsigned __int8 *)psDest, (unsigned __int8 *)*psParsePos, iCharsToCopy);
    psDest[iCharsToCopy] = 0;
    for ( *psParsePos += iCharsToCopy; **psParsePos; ++*psParsePos )
    {
      strchr("\r\n", **psParsePos);
      if ( !v4 )
        break;
    }
  }
  else
  {
    v8 = (unsigned __int8 *)*psParsePos;
    v7 = psDest;
    do
    {
      v5 = *v8;
      *v7++ = *v8++;
    }
    while ( v5 );
    *psParsePos += strlen(*psParsePos);
  }
  if ( *psDest )
  {
    for ( iWhiteSpaceScanPos = strlen(psDest) - 1;
          iWhiteSpaceScanPos >= 0 && isspace(psDest[iWhiteSpaceScanPos]);
          --iWhiteSpaceScanPos )
    {
      psDest[iWhiteSpaceScanPos] = 0;
    }
    CStringEdPackage::REMKill(this, psDest);
  }
  return 1;
}

std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > *__thiscall CStringEdPackage::InsideQuotes(
        CStringEdPackage *this,
        std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > *result,
        const char *psLine)
{
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *v4; // [esp+10h] [ebp-11Ch]
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *v5; // [esp+14h] [ebp-118h]
  unsigned int v6; // [esp+18h] [ebp-114h]
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *Ptr; // [esp+28h] [ebp-104h]
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *p_Bx; // [esp+3Ch] [ebp-F0h]
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *v9; // [esp+40h] [ebp-ECh]
  unsigned int v10; // [esp+44h] [ebp-E8h]
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *v11; // [esp+54h] [ebp-D8h]
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *v12; // [esp+58h] [ebp-D4h]
  unsigned int v13; // [esp+5Ch] [ebp-D0h]
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *v14; // [esp+6Ch] [ebp-C0h]
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > str; // [esp+10Ch] [ebp-20h] BYREF

  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(&str, 0, 0);
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::assign(
    &str,
    &toastPopupTitle,
    strlen(&toastPopupTitle));
  while ( *psLine == 32 || *psLine == 9 )
    ++psLine;
  if ( *psLine == 34 )
    ++psLine;
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::assign(
    &str,
    psLine,
    strlen(psLine));
  if ( *psLine )
  {
    while ( 1 )
    {
      v14 = str._Myres < 0x10
          ? &str._Bx
          : (std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *)str._Bx._Ptr;
      v13 = strlen(v14->_Buf);
      v12 = str._Myres < 0x10
          ? &str._Bx
          : (std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *)str._Bx._Ptr;
      if ( v12->_Buf[v13 - 1] != 32 )
      {
        v11 = str._Myres < 0x10
            ? &str._Bx
            : (std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *)str._Bx._Ptr;
        v10 = strlen(v11->_Buf);
        v9 = str._Myres < 0x10
           ? &str._Bx
           : (std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *)str._Bx._Ptr;
        if ( v9->_Buf[v10 - 1] != 9 )
          break;
      }
      if ( str._Myres < 0x10 )
        p_Bx = &str._Bx;
      else
        p_Bx = (std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *)str._Bx._Ptr;
      std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::erase(
        (std::string *)&str,
        strlen(p_Bx->_Buf) - 1,
        1u);
    }
    if ( str._Myres < 0x10 )
      Ptr = &str._Bx;
    else
      Ptr = (std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *)str._Bx._Ptr;
    v6 = strlen(Ptr->_Buf);
    if ( str._Myres < 0x10 )
      v5 = &str._Bx;
    else
      v5 = (std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *)str._Bx._Ptr;
    if ( v5->_Buf[v6 - 1] == 34 )
    {
      if ( str._Myres < 0x10 )
        v4 = &str._Bx;
      else
        v4 = (std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *)str._Bx._Ptr;
      std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::erase(
        (std::string *)&str,
        strlen(v4->_Buf) - 1,
        1u);
    }
  }
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(result, 0, 0);
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::assign(
    result,
    &str,
    0,
    std::string::npos);
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(&str, 1, 0);
  return result;
}

const char *__thiscall CStringEdPackage::ParseLine(CStringEdPackage *this, char *psLine, bool forceEnglish)
{
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > v4; // [esp-1Ch] [ebp-4F8h] BYREF
  char *psNewString; // [esp+0h] [ebp-4DCh]
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *p_Bx; // [esp+4h] [ebp-4D8h]
  char *string; // [esp+8h] [ebp-4D4h]
  int v8; // [esp+Ch] [ebp-4D0h]
  char *v9; // [esp+14h] [ebp-4C8h]
  char *v10; // [esp+18h] [ebp-4C4h]
  char *psLocalReference; // [esp+1Ch] [ebp-4C0h]
  char *nptr; // [esp+20h] [ebp-4BCh]
  CStringEdPackage *thisa; // [esp+24h] [ebp-4B8h]
  int bSentenceIsEnglish; // [esp+68h] [ebp-474h]
  char sThisLanguage[1028]; // [esp+6Ch] [ebp-470h] BYREF
  const char *psWordEnd; // [esp+470h] [ebp-6Ch]
  int iCharsToCopy; // [esp+474h] [ebp-68h]
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > sentence; // [esp+478h] [ebp-64h] BYREF
  const char *psReference; // [esp+494h] [ebp-48h]
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > localReference; // [esp+498h] [ebp-44h] BYREF
  int iVersionNumber; // [esp+4B4h] [ebp-28h]
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > versionNumber; // [esp+4B8h] [ebp-24h] BYREF
  const char *psErrorMessage; // [esp+4D8h] [ebp-4h]

  thisa = this;
  psErrorMessage = 0;
  if ( psLine )
  {
    if ( CStringEdPackage::CheckLineForKeyword(thisa, "VERSION", (const char **)&psLine) )
    {
      CStringEdPackage::InsideQuotes(thisa, &versionNumber, psLine);
      if ( versionNumber._Myres < 0x10 )
        nptr = versionNumber._Bx._Buf;
      else
        nptr = versionNumber._Bx._Ptr;
      iVersionNumber = atoi(nptr);
      if ( iVersionNumber != 1 )
        psErrorMessage = va("Unexpected version number %d, expecting %d!", iVersionNumber, 1);
      std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(&versionNumber, 1, 0);
    }
    else if ( !CStringEdPackage::CheckLineForKeyword(thisa, "CONFIG", (const char **)&psLine)
           && !CStringEdPackage::CheckLineForKeyword(thisa, "FILENOTES", (const char **)&psLine)
           && !CStringEdPackage::CheckLineForKeyword(thisa, "NOTES", (const char **)&psLine)
           && !CStringEdPackage::CheckLineForKeyword(thisa, "FLAGS", (const char **)&psLine) )
    {
      if ( CStringEdPackage::CheckLineForKeyword(thisa, "REFERENCE", (const char **)&psLine) )
      {
        CStringEdPackage::InsideQuotes(thisa, &localReference, psLine);
        if ( localReference._Myres < 0x10 )
          psLocalReference = localReference._Bx._Buf;
        else
          psLocalReference = localReference._Bx._Ptr;
        CStringEdPackage::AddEntry(thisa, psLocalReference);
        std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(&localReference, 1, 0);
      }
      else if ( CStringEdPackage::CheckLineForKeyword(thisa, "ENDMARKER", (const char **)&psLine) )
      {
        thisa->m_bEndMarkerFound_ParseOnly = 1;
      }
      else if ( I_strnicmp("LANG_", psLine, 5) )
      {
        return va("Unknown keyword at linestart: \"%s\"", psLine);
      }
      else
      {
        psReference = (const char *)CStringEdPackage::GetCurrentReference_ParseOnly(thisa);
        if ( *psReference )
        {
          psLine += 5;
          for ( psWordEnd = psLine; *psWordEnd && *psWordEnd != 32 && *psWordEnd != 9; ++psWordEnd )
            ;
          memset(sThisLanguage, 0, 1024);
          iCharsToCopy = psWordEnd - psLine;
          if ( (unsigned int)(psWordEnd - psLine) > 0x3FF )
            iCharsToCopy = 1023;
          strncpy((unsigned __int8 *)sThisLanguage, (unsigned __int8 *)psLine, iCharsToCopy);
          v10 = sThisLanguage;
          v9 = &sThisLanguage[1];
          v10 += strlen(v10) + 1;
          v8 = v10 - &sThisLanguage[1];
          psLine += v10 - &sThisLanguage[1];
          CStringEdPackage::InsideQuotes(thisa, &v4, psLine);
          CStringEdPackage::ConvertCRLiterals_Read(thisa, &sentence, v4);
          if ( sentence._Myres < 0x10 )
            string = sentence._Bx._Buf;
          else
            string = sentence._Bx._Ptr;
          if ( !CStringEdPackage::IsStringFormatCorrect(thisa, string) )
          {
            if ( sentence._Myres < 0x10 )
              p_Bx = &sentence._Bx;
            else
              p_Bx = (std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *)sentence._Bx._Ptr;
            psErrorMessage = va("Illegal string format \"%s\"", p_Bx->_Buf);
          }
          bSentenceIsEnglish = I_stricmp(sThisLanguage, "english") == 0;
          if ( !psErrorMessage && (bSentenceIsEnglish || !forceEnglish) )
          {
            if ( sentence._Myres < 0x10 )
              psNewString = sentence._Bx._Buf;
            else
              psNewString = sentence._Bx._Ptr;
            CStringEdPackage::SetString(thisa, psReference, psNewString, bSentenceIsEnglish);
          }
          std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(&sentence, 1, 0);
        }
        else
        {
          return "Error parsing file: Unexpected \"LANG_\"";
        }
      }
    }
  }
  return psErrorMessage;
}

std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *__thiscall CStringEdPackage::GetCurrentReference_ParseOnly(
        CStringEdPackage *this)
{
  if ( this->m_strCurrentEntryRef_ParseOnly._Myres < 0x10 )
    return &this->m_strCurrentEntryRef_ParseOnly._Bx;
  else
    return (std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *)this->m_strCurrentEntryRef_ParseOnly._Bx._Ptr;
}

void __thiscall CStringEdPackage::AddEntry(CStringEdPackage *this, const char *psLocalReference)
{
  char *v2; // eax
  char *v3; // eax
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > *v5; // [esp+68h] [ebp-140h]
  char *v6; // [esp+F0h] [ebp-B8h]
  std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::less<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > >,Allocator<std::pair<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > >,mapStringEntriesName_t>,0> >::_Node *Myhead; // [esp+104h] [ebp-A4h]
  char *_Ptr; // [esp+134h] [ebp-74h]
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > v9; // [esp+140h] [ebp-68h] BYREF
  std::_Aux_cont *Myownedaux; // [esp+15Ch] [ebp-4Ch]
  std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::less<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > >,Allocator<std::pair<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > >,mapStringEntriesName_t>,0> >::_Node *v11; // [esp+160h] [ebp-48h]
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > _Keyval; // [esp+164h] [ebp-44h] BYREF
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > SE_Entry; // [esp+180h] [ebp-28h] BYREF
  std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::less<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > >,Allocator<std::pair<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > >,mapStringEntriesName_t>,0> >::iterator itEntry; // [esp+1A0h] [ebp-8h] BYREF

  if ( this->m_strCurrentFileRef_ParseOnly._Myres < 0x10 )
    v2 = va("%s_%s", this->m_strCurrentFileRef_ParseOnly._Bx._Buf, psLocalReference);
  else
    v2 = va("%s_%s", this->m_strCurrentFileRef_ParseOnly._Bx._Ptr, psLocalReference);
  _Ptr = v2;
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(&_Keyval, 0, 0);
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::assign(
    &_Keyval,
    _Ptr,
    strlen(_Ptr));
  std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::less<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>>,Allocator<std::pair<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>>,mapStringEntriesName_t>,0>>::find(
    &this->m_StringEntries,
    &itEntry,
    &_Keyval);
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(&_Keyval, 1, 0);
  Myhead = this->m_StringEntries._Myhead;
  Myownedaux = 0;
  v11 = Myhead;
  if ( this == (CStringEdPackage *)-88 )
    _invalid_parameter_noinfo();
  Myownedaux = this->m_StringEntries._Myownedaux;
  if ( !itEntry._Myaux || itEntry._Myaux != Myownedaux )
    _invalid_parameter_noinfo();
  if ( itEntry._Ptr == v11 )
  {
    std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(&SE_Entry, 0, 0);
    if ( this->m_strCurrentFileRef_ParseOnly._Myres < 0x10 )
      v3 = va("%s_%s", this->m_strCurrentFileRef_ParseOnly._Bx._Buf, psLocalReference);
    else
      v3 = va("%s_%s", this->m_strCurrentFileRef_ParseOnly._Bx._Ptr, psLocalReference);
    v6 = v3;
    std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(&v9, 0, 0);
    std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::assign(&v9, v6, strlen(v6));
    v5 = std::map<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::less<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>>,Allocator<std::pair<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>>,mapStringEntriesName_t>>::operator[](
           &this->m_StringEntries,
           &v9);
    std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::assign(
      v5,
      &SE_Entry,
      0,
      std::string::npos);
    std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(&v9, 1, 0);
    std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(&SE_Entry, 1, 0);
  }
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::assign(
    &this->m_strCurrentEntryRef_ParseOnly,
    psLocalReference,
    strlen(psLocalReference));
}

void __thiscall CStringEdPackage::SetString(
        CStringEdPackage *this,
        const char *psLocalReference,
        const char *psNewString,
        int bSentenceIsEnglish)
{
  char *v4; // eax
  std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::less<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > >,Allocator<std::pair<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > >,mapStringEntriesName_t>,0> >::_Node *Myhead; // [esp+8Ch] [ebp-6Ch]
  char *_Ptr; // [esp+BCh] [ebp-3Ch]
  std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::less<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > >,Allocator<std::pair<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > >,mapStringEntriesName_t>,0> >::const_iterator _Right; // [esp+C8h] [ebp-30h] BYREF
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > _Keyval; // [esp+D0h] [ebp-28h] BYREF
  std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::less<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > >,Allocator<std::pair<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > >,mapStringEntriesName_t>,0> >::iterator itEntry; // [esp+ECh] [ebp-Ch] BYREF
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > *Entry; // [esp+F4h] [ebp-4h]

  if ( this->m_strCurrentFileRef_ParseOnly._Myres < 0x10 )
    v4 = va("%s_%s", this->m_strCurrentFileRef_ParseOnly._Bx._Buf, psLocalReference);
  else
    v4 = va("%s_%s", this->m_strCurrentFileRef_ParseOnly._Bx._Ptr, psLocalReference);
  _Ptr = v4;
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(&_Keyval, 0, 0);
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::assign(
    &_Keyval,
    _Ptr,
    strlen(_Ptr));
  std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::less<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>>,Allocator<std::pair<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>>,mapStringEntriesName_t>,0>>::find(
    &this->m_StringEntries,
    &itEntry,
    &_Keyval);
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(&_Keyval, 1, 0);
  Myhead = this->m_StringEntries._Myhead;
  _Right._Myaux = 0;
  _Right._Ptr = Myhead;
  if ( this == (CStringEdPackage *)-88 )
    _invalid_parameter_noinfo();
  _Right._Myaux = this->m_StringEntries._Myownedaux;
  if ( !std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::less<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>>,Allocator<std::pair<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>>,mapStringEntriesName_t>,0>>::const_iterator::operator!=(
          &itEntry,
          &_Right)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\stringed\\stringed_ingame.cpp",
          727,
          0,
          "%s",
          "itEntry != m_StringEntries.end()") )
  {
    __debugbreak();
  }
  Entry = &std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::less<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>>,Allocator<std::pair<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>>,mapStringEntriesName_t>,0>>::const_iterator::operator*(&itEntry)->second;
  if ( bSentenceIsEnglish )
  {
    std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::assign(
      Entry,
      psNewString,
      strlen(psNewString));
    std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::assign(
      &this->m_strCurrentEntryEnglish_ParseOnly,
      psNewString,
      strlen(psNewString));
  }
  else if ( I_stricmp(psNewString, "#same") )
  {
    std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::assign(
      Entry,
      psNewString,
      strlen(psNewString));
  }
  else
  {
    std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::assign(
      Entry,
      &this->m_strCurrentEntryEnglish_ParseOnly,
      0,
      std::string::npos);
  }
}

char *__cdecl SE_Load(char *psFileName, bool forceEnglish)
{
  char *psParsePos; // [esp+1Ch] [ebp-4014h] BYREF
  char psDest[16388]; // [esp+20h] [ebp-4010h] BYREF
  const char *v5; // [esp+4028h] [ebp-8h]
  unsigned __int8 *psLoadedFile; // [esp+402Ch] [ebp-4h]

  v5 = 0;
  psLoadedFile = SE_LoadFileData(psFileName);
  if ( !psLoadedFile )
    return va("Unable to load \"%s\"!", psFileName);
  psParsePos = (char *)psLoadedFile;
  if ( *(unsigned int *)psLoadedFile == unk_CF1720 || *(unsigned int *)psLoadedFile == unk_CF1718 )
  {
    psParsePos += 4;
  }
  else if ( *(_WORD *)psLoadedFile == unk_CF1714 || *(_WORD *)psLoadedFile == unk_CF1710 )
  {
    psParsePos += 2;
  }
  else if ( !memcmp(psLoadedFile, &off_CF170C, 3u) )
  {
    psParsePos += 3;
  }
  CStringEdPackage::SetupNewFileParse(TheStringPackage, psFileName);
  while ( !v5 && CStringEdPackage::ReadLine(TheStringPackage, (const char **)&psParsePos, psDest) )
  {
    if ( &psDest[strlen(psDest) + 1] != &psDest[1] )
      v5 = CStringEdPackage::ParseLine(TheStringPackage, psDest, forceEnglish);
  }
  if ( v5 )
    v5 = va("%s in %s", v5, psFileName);
  SE_FreeFileDataAfterLoad(psLoadedFile);
  if ( !v5 && !TheStringPackage->m_bEndMarkerFound_ParseOnly )
    return va("Truncated file, failed to find \"%s\" at file end in file %s", "ENDMARKER", psFileName);
  return (char *)v5;
}

std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *__cdecl SE_GetString_LoadObj(
        const char *psPackageAndStringReference)
{
  const std::pair<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > const ,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > > *v1; // eax
  std::map<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::less<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > >,Allocator<std::pair<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > >,mapStringEntriesName_t> > *p_m_StringEntries; // [esp+18h] [ebp-88h]
  std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::less<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > >,Allocator<std::pair<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > >,mapStringEntriesName_t>,0> >::_Node *Myhead; // [esp+1Ch] [ebp-84h]
  std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::less<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > >,Allocator<std::pair<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > >,mapStringEntriesName_t>,0> >::const_iterator _Right; // [esp+70h] [ebp-30h] BYREF
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > _Keyval; // [esp+78h] [ebp-28h] BYREF
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > *Entry; // [esp+94h] [ebp-Ch]
  std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::less<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > >,Allocator<std::pair<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > >,mapStringEntriesName_t>,0> >::iterator itEntry; // [esp+98h] [ebp-8h] BYREF

  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(&_Keyval, 0, 0);
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::assign(
    &_Keyval,
    psPackageAndStringReference,
    strlen(psPackageAndStringReference));
  std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::less<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>>,Allocator<std::pair<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>>,mapStringEntriesName_t>,0>>::find(
    &TheStringPackage->m_StringEntries,
    &itEntry,
    &_Keyval);
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(&_Keyval, 1, 0);
  p_m_StringEntries = &TheStringPackage->m_StringEntries;
  Myhead = TheStringPackage->m_StringEntries._Myhead;
  _Right._Myaux = 0;
  _Right._Ptr = Myhead;
  if ( TheStringPackage == (CStringEdPackage *)-88 )
    _invalid_parameter_noinfo();
  _Right._Myaux = p_m_StringEntries->_Myownedaux;
  if ( !std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::less<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>>,Allocator<std::pair<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>>,mapStringEntriesName_t>,0>>::const_iterator::operator!=(
          &itEntry,
          &_Right) )
    return 0;
  v1 = std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::less<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>>,Allocator<std::pair<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>>,mapStringEntriesName_t>,0>>::const_iterator::operator*(&itEntry);
  Entry = &v1->second;
  if ( v1->second._Myres < 0x10 )
    return &Entry->_Bx;
  else
    return (std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *)Entry->_Bx._Ptr;
}

void __cdecl SE_NewLanguage()
{
  if ( !TheStringPackage
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\stringed\\stringed_ingame.cpp",
          858,
          0,
          "%s",
          "TheStringPackage") )
  {
    __debugbreak();
  }
  CStringEdPackage::Clear(TheStringPackage);
}

void __cdecl SE_Init()
{
  CStringEdPackage *v0; // [esp+0h] [ebp-68h]
  CStringEdPackage *v1; // [esp+64h] [ebp-4h]

  if ( TheStringPackage
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\stringed\\stringed_ingame.cpp",
          870,
          0,
          "%s",
          "!TheStringPackage") )
  {
    __debugbreak();
  }
  v1 = (CStringEdPackage *)Z_Malloc(120, "CStringEdPackage", 34);
  if ( v1 )
    v0 = CStringEdPackage::CStringEdPackage(v1);
  else
    v0 = 0;
  TheStringPackage = v0;
  CStringEdPackage::Clear(v0);
}

CStringEdPackage *__thiscall CStringEdPackage::CStringEdPackage(CStringEdPackage *this)
{
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(
    &this->m_strCurrentEntryRef_ParseOnly,
    0,
    0);
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(
    &this->m_strCurrentEntryEnglish_ParseOnly,
    0,
    0);
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(
    &this->m_strCurrentFileRef_ParseOnly,
    0,
    0);
  std::map<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::less<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>>,Allocator<std::pair<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>>,mapStringEntriesName_t>>::map<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::less<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>>,Allocator<std::pair<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>>,mapStringEntriesName_t>>(&this->m_StringEntries);
  return this;
}

void __cdecl SE_ShutDown()
{
  CStringEdPackage *v0; // [esp+44h] [ebp-4h]

  if ( TheStringPackage )
  {
    CStringEdPackage::Clear(TheStringPackage);
    v0 = TheStringPackage;
    if ( TheStringPackage )
    {
      CStringEdPackage::~CStringEdPackage(TheStringPackage);
      Z_Free((char *)v0, 34);
    }
    TheStringPackage = 0;
  }
}

void __thiscall CStringEdPackage::~CStringEdPackage(CStringEdPackage *this)
{
  int Type; // eax
  std::map<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::less<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > >,Allocator<std::pair<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > >,mapStringEntriesName_t> > *p_m_StringEntries; // [esp+40h] [ebp-44h]
  char *ptr; // [esp+44h] [ebp-40h]

  p_m_StringEntries = &this->m_StringEntries;
  std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::less<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>>,Allocator<std::pair<std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>,std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>>,mapStringEntriesName_t>,0>>::_Tidy(&this->m_StringEntries);
  ptr = (char *)p_m_StringEntries->_Myownedaux;
  Type = LocalizeName::GetType();
  Z_Free(ptr, Type);
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(
    &this->m_strCurrentFileRef_ParseOnly,
    1,
    0);
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(
    &this->m_strCurrentEntryEnglish_ParseOnly,
    1,
    0);
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(
    &this->m_strCurrentEntryRef_ParseOnly,
    1,
    0);
}

char *__cdecl SE_LoadLanguage(bool forceEnglish)
{
  char *psErrorMessage; // [esp+30h] [ebp-28h]
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > strResults; // [esp+34h] [ebp-24h] BYREF
  const char *p; // [esp+54h] [ebp-4h]

  psErrorMessage = 0;
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(&strResults, 0, 0);
  SE_NewLanguage();
  SE_BuildFileList("localizedstrings", &strResults);
  while ( 1 )
  {
    p = SE_GetFoundFile((std::string *)&strResults);
    if ( !p || psErrorMessage )
      break;
    psErrorMessage = SE_Load((char *)p, forceEnglish);
  }
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::_Tidy(&strResults, 1, 0);
  return psErrorMessage;
}

char *__cdecl SE_GetFoundFile(std::string *strResult)
{
  _BYTE *v2; // eax
  std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *p_Bx; // [esp+14h] [ebp-38h]

  if ( strResult->_Myres < 0x10 )
    p_Bx = (std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *)&strResult->_Bx;
  else
    p_Bx = (std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *)strResult->_Bx._Ptr;
  if ( !strlen(p_Bx->_Buf) )
    return 0;
  if ( strResult->_Myres < 0x10 )
    strncpy((unsigned __int8 *)sTemp, (unsigned __int8 *)&strResult->_Bx, 0x3Fu);
  else
    strncpy((unsigned __int8 *)sTemp, (unsigned __int8 *)strResult->_Bx._Ptr, 0x3Fu);
  sTemp[63] = 0;
  strchr((unsigned __int8 *)sTemp, 0x3Bu);
  if ( v2 )
  {
    *v2 = 0;
    std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::erase(
      strResult,
      0,
      v2 - sTemp + 1);
  }
  else
  {
    std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName>>::erase(
      strResult,
      0,
      std::string::npos);
  }
  return sTemp;
}

