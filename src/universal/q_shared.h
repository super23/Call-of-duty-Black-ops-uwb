#pragma once

unsigned __int8 __cdecl ColorIndex(unsigned __int8 c);
const char *__cdecl Com_GetFilenameSubString(const char *pathname);
const char *__cdecl Com_GetExtensionSubString(const char *filename);
void __cdecl Com_StripExtension(char *in, char *out);
void __cdecl Com_DefaultExtension(char *path, unsigned int maxSize, const char *extension);
__int16 __cdecl BigShort(__int16 l);
unsigned __int64 __cdecl LittleLong64(unsigned __int64 l);
int __cdecl ShortSwap(__int16 l);
__int16 __cdecl ShortNoSwap(__int16 l);
int __cdecl LongSwap(int l);
unsigned __int64 __cdecl Long64Swap(unsigned __int64 l);
unsigned __int64 __cdecl Long64NoSwap(unsigned __int64 ll);
double __cdecl FloatReadSwap(int n);
double __cdecl FloatReadNoSwap(int n);
FloatWriteSwap::__l2::<unnamed_type_dat1> __cdecl FloatWriteSwap(float f);
void __cdecl Swap_InitLittleEndian();
void __cdecl Swap_InitBigEndian();
void __cdecl Swap_Init();
__int64 __cdecl I_atoi64(const char *str);
bool __cdecl I_islower(int c);
bool __cdecl I_isupper(int c);
bool __cdecl I_isalpha(int c);
bool __cdecl I_isdigit(int c);
bool __cdecl I_isalnum(int c);
bool __cdecl I_isforfilename(int c);
void __cdecl I_strncpyz(char *dest, const char *src, int destsize);
int __cdecl I_strnicmp(const char *s0, const char *s1, int n);
int __cdecl I_strncmp(const char *s0, const char *s1, int n);
const char *__cdecl I_stristr(const char *s0, const char *substr);
int __cdecl I_stricmp(const char *s0, const char *s1);
int __cdecl I_strcmp(const char *s0, const char *s1);
int __cdecl I_stricmpwild(const char *wild, const char *s);
char *__cdecl I_strlwr(char *s);
char *__cdecl I_strupr(char *s);
bool __cdecl I_iscsym(int c);
void __cdecl I_strncat(char *dest, int size, const char *src);
int __cdecl I_DrawStrlen(const char *str);
char *__cdecl I_CleanStr(char *string);
unsigned __int8 __cdecl I_CleanChar(unsigned __int8 character);
int Com_sprintf(char *dest, unsigned int size, const char *fmt, ...);
int Com_sprintfPos(char *dest, int destSize, int *destPos, const char *fmt, ...);
bool __cdecl CanKeepStringPointer(char *string);
char *__cdecl I_itoa(int value, char *buf, int bufsize);
void __cdecl strreverse(char *begin, char *end);
char *va(const char *format, ...);
void __cdecl Com_InitThreadData(int threadContext);
char *__cdecl Info_ValueForKey(char *s, char *key);
void __cdecl Info_NextPair(const char **head, char *key, char *value);
void __cdecl Info_RemoveKey(char *s, char *key);
void __cdecl Info_RemoveKey_Big(char *s, char *key);
bool __cdecl Info_Validate(char *s);
void __cdecl Info_SetValueForKey(char *s, char *key, const char *value);
void __cdecl Info_SetValueForKey_Big(char *s, char *key, const char *value);
bool __cdecl ParseConfigStringToStruct(
        unsigned __int8 *pStruct,
        const cspField_t *pFieldList,
        int iNumFields,
        char *pszBuffer,
        int iMaxFieldTypes,
        int (__cdecl *parseSpecialFieldType)(unsigned __int8 *, const char *, const int, const int),
        void (__cdecl *parseStrCpy)(unsigned __int8 *, const char *));
bool __cdecl KeyValueToField(
        unsigned __int8 *pStruct,
        const cspField_t *pField,
        char *pszKeyValue,
        int iMaxFieldTypes,
        int (__cdecl *parseSpecialFieldType)(unsigned __int8 *, const char *, const int, const int),
        void (__cdecl *parseStrcpy)(unsigned __int8 *, const char *));
bool __cdecl ParseConfigStringToStructMerged(
        unsigned __int8 *pStruct,
        cspField_t *pFieldList,
        int iNumFields,
        const char *mergedName,
        const char **pszBuffer,
        const char **sourceName,
        char *pszMergedBuffer,
        int iMaxFieldTypes,
        int (__cdecl *parseSpecialFieldType)(unsigned __int8 *, const char *, const int, const int),
        void (__cdecl *parseStrCpy)(unsigned __int8 *, const char *),
        int (__cdecl *parseMergeSpecialCase)(const char *, char **, char *, int));
double __cdecl GetLeanFraction(float fFrac);
double __cdecl UnGetLeanFraction(float fFrac);
void __cdecl AddLeanToPosition(float *position, float fViewYaw, float fLeanFrac, float fViewRoll, float fLeanDist);
void __cdecl OrientationPosToWorldPos(const orientation_t *orient, const float *pos, float *out);
void __cdecl OrientationDirToWorldDir(const orientation_t *orient, const float *dir, float *out);
void __cdecl OrientationConcatenate(const orientation_t *orFirst, const orientation_t *orSecond, orientation_t *out);
void __cdecl OrientationInvert(const orientation_t *orient, orientation_t *out);
bool __cdecl Com_IsLegacyXModelName(const char *name);
double __cdecl CMD_GetAnalogButtonValue(usercmd_s *cmd, unsigned int whichBit);
