#pragma once

int __cdecl LocalizeName::GetType();
char *__cdecl LocalizeStringName::GetName();
void __thiscall CStringEdPackage::Clear(CStringEdPackage *this);
char *__thiscall CStringEdPackage::Filename_WithoutExt(CStringEdPackage *this, const char *psFilename);
char *__thiscall CStringEdPackage::Filename_WithoutPath(CStringEdPackage *this, const char *psFilename);
void __thiscall CStringEdPackage::SetupNewFileParse(CStringEdPackage *this, const char *psFileName);
int __thiscall CStringEdPackage::CheckLineForKeyword(
        CStringEdPackage *this,
        const char *psKeyword,
        const char **psLine);
char __thiscall CStringEdPackage::IsStringFormatCorrect(CStringEdPackage *this, char *string);
std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > *__thiscall CStringEdPackage::ConvertCRLiterals_Read(
        CStringEdPackage *this,
        std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > *result,
        std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > string);
void __thiscall CStringEdPackage::REMKill(CStringEdPackage *this, char *psBuffer);
int __thiscall CStringEdPackage::ReadLine(CStringEdPackage *this, const char **psParsePos, char *psDest);
std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > *__thiscall CStringEdPackage::InsideQuotes(
        CStringEdPackage *this,
        std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > *result,
        const char *psLine);
const char *__thiscall CStringEdPackage::ParseLine(CStringEdPackage *this, char *psLine, bool forceEnglish);
std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *__thiscall CStringEdPackage::GetCurrentReference_ParseOnly(
        CStringEdPackage *this);
void __thiscall CStringEdPackage::AddEntry(CStringEdPackage *this, const char *psLocalReference);
void __thiscall CStringEdPackage::SetString(
        CStringEdPackage *this,
        const char *psLocalReference,
        const char *psNewString,
        int bSentenceIsEnglish);
char *__cdecl SE_Load(char *psFileName, bool forceEnglish);
std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> >::_Bxty *__cdecl SE_GetString_LoadObj(
        const char *psPackageAndStringReference);
void __cdecl SE_NewLanguage();
void __cdecl SE_Init();
CStringEdPackage *__thiscall CStringEdPackage::CStringEdPackage(CStringEdPackage *this);
void __cdecl SE_ShutDown();
void __thiscall CStringEdPackage::~CStringEdPackage(CStringEdPackage *this);
char *__cdecl SE_LoadLanguage(bool forceEnglish);
char *__cdecl SE_GetFoundFile(std::string *strResult);
