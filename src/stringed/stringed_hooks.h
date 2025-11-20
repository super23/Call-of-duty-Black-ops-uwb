#pragma once

int __cdecl SEH_GetCurrentLanguage();
void __cdecl SEH_InitLanguage();
void SEH_UpdateCurrentLanguage();
void __cdecl SEH_UpdateLanguageInfo();
int __cdecl SEH_VerifyLanguageSelection(int iLanguageSelection);
void __cdecl SEH_Init_StringEd();
void __cdecl SEH_Shutdown_StringEd();
int __cdecl SEH_StringEd_SetLanguageStrings(unsigned int iLanguage);
const char *__cdecl SE_GetString(char *psPackageAndStringReference);
const char *__cdecl SE_GetString_FastFile(char *psPackageAndStringReference);
const char *__cdecl SEH_StringEd_GetString(char *pszReference);
char *__cdecl SEH_SafeTranslateString(const char *pszReference);
int __cdecl SEH_ReplaceDirectiveInStringWithBinding(int localClientNum, char *translatedString, char *finalString);
char __cdecl SEH_ReplaceDirective(
        int localClientNum,
        unsigned int *searchPos,
        unsigned int *dstLen,
        char *dstString,
        unsigned int size);
void __cdecl SEH_GetBindingForDirective(int localClientNum, char *directive, char *result);
void __cdecl SEH_ParseDirective(char *directive, char *resultName, char *resultArg0);
char *__cdecl SEH_LocalizeTextMessage(const char *pszInputBuffer, const char *pszMessageType, msgLocErrType_t errType);
int __cdecl SEH_GetLocalizedTokenReference(
        char *token,
        char *reference,
        const char *messageType,
        msgLocErrType_t errType);
bool __cdecl Japanese_ValidShiftJISCode(unsigned int _iHi, unsigned int _iLo);
int __cdecl SEH_DecodeLetter(
        unsigned __int8 firstChar,
        unsigned __int8 secondChar,
        int *usedCount,
        int *pbIsTrailingPunctuation);
bool __cdecl Japanese_IsTrailingPunctuation(unsigned int uiCode);
unsigned int __cdecl SEH_ReadCharFromString(const char **text, int *isTrailingPunctuation);
int __cdecl Language_IsAsian();
int __cdecl SEH_PrintStrlen(const char *string);
const char *__cdecl SEH_GetLanguageName(unsigned int iLanguage);
const char *__cdecl SEH_GetLanguageNameAbbr(unsigned int iLanguage);
int __cdecl SEH_GetLanguageIndexForName(const char *pszLanguageName, int *piLanguageIndex);
EXT_List<LocalizeEntry> *__thiscall EXT_List<LocalizeEntry>::EXT_List<LocalizeEntry>(EXT_List<LocalizeEntry> *this);
void __thiscall EXT_List<LocalizeEntry>::~EXT_List<LocalizeEntry>(EXT_List<LocalizeEntry> *this);
void __thiscall EXT_List<LocalizeEntry>::Empty(EXT_List<LocalizeEntry> *this, bool DeleteData);
char __thiscall EXT_List<LocalizeEntry>::Remove(
        EXT_List<LocalizeEntry> *this,
        EXT_Link<LocalizeEntry> *Link,
        bool DeleteData);
