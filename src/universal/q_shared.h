#pragma once

// this is the define for determining if we have an asm version of a C function
#if (defined _M_IX86 || defined __i386__) && !defined __sun__  && !defined __LCC__
#define id386	1
#else
#define id386	0
#endif

// for windows fastcall option
#define	QDECL

//======================= WIN32 DEFINES =================================
#ifdef WIN32

#define	MAC_STATIC

#undef QDECL
#define	QDECL	__cdecl

// buildstring will be incorporated into the version string
#ifdef NDEBUG
#ifdef _M_IX86
#define	CPUSTRING	"win-x86"
#elif defined _M_ALPHA
#define	CPUSTRING	"win-AXP"
#endif
#else
#ifdef _M_IX86
#define	CPUSTRING	"win-x86-debug"
#elif defined _M_ALPHA
#define	CPUSTRING	"win-AXP-debug"
#endif
#endif

#define ID_INLINE __inline 

int __cdecl ShortSwap(__int16 l);
int __cdecl LongSwap(int l);

static ID_INLINE short BigShort(short l) { return ShortSwap(l); }
#define LittleShort
static ID_INLINE int BigLong(int l) { return LongSwap(l); }
#define LittleLong
//static ID_INLINE float BigFloat(const float *l) { FloatSwap(l); }
#define LittleFloat

#define	PATH_SEP '\\'

#endif // WIN32


#define MAX_LOCAL_CENTITIES 0x600

//=============================================
void I_strncat(char *dest, int size, const char *src);
void I_strncpyz(char *dest, const char *src, int destsize);
int I_stricmp(const char *s0, const char *s1);
int I_strnicmp(const char *s0, const char *s1, int n);
const char *__cdecl I_stristr(const char *s0, const char *substr);

bool I_islower(int c);
bool I_isupper(int c);
bool I_isalpha(int c);

bool I_iscsym(int c);

bool __cdecl I_isdigit(int c);
bool __cdecl I_isalnum(int c);
bool __cdecl I_isforfilename(int c);
int __cdecl I_strncmp(const char *s0, const char *s1, int n);
int __cdecl I_strcmp(const char *s0, const char *s1);
int __cdecl I_stricmpwild(const char *wild, const char *s);
char *__cdecl I_strlwr(char *s);
char *__cdecl I_strupr(char *s);
int __cdecl I_DrawStrlen(const char *str);
char *__cdecl I_CleanStr(char *string);
unsigned __int8 __cdecl I_CleanChar(unsigned __int8 character);

// lwss: fcking winblows!
#undef MAKEWORD
#undef MAKELONG
#undef LOWORD
#undef HIWORD
#undef LOBYTE
#undef HIBYTE

#if defined(__GNUC__)
typedef          long long ll;
typedef unsigned long long ull;
#define __int64 long long
#define __int32 int
#define __int16 short
#define __int8  char
#define MAKELL(num) num ## LL
#define FMT_64 "ll"
#elif defined(_MSC_VER)
typedef          __int64 ll;
typedef unsigned __int64 ull;
#define MAKELL(num) num ## i64
#define FMT_64 "I64"
#elif defined (__BORLANDC__)
typedef          __int64 ll;
typedef unsigned __int64 ull;
#define MAKELL(num) num ## i64
#define FMT_64 "L"
#else
#error "unknown compiler"
#endif
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

typedef   signed char   int8; // LWSS: slight changes here made to conform with steam api...
typedef   signed char   sint8;
typedef unsigned char   uint8;
typedef          short  int16;
typedef   signed short  sint16;
typedef unsigned short  uint16;
typedef __int32				int32;
typedef unsigned __int32 uint32;
typedef signed long			sint32;
typedef ll              int64;
typedef ll              sint64;
typedef ull             uint64;

// Partially defined types. They are used when the decompiler does not know
// anything about the type except its size.
#define _BYTE  uint8
#define _WORD  uint16
#define _DWORD uint32
#define _QWORD uint64
#if !defined(_MSC_VER)
#define _LONGLONG __int128
#endif

#ifndef M_PI
#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h
#endif

#define M_PI_HALF (M_PI / 2.0) // LWSS ADD

// Some convenience macros to make partial accesses nicer
#define LAST_IND(x,part_type)    (sizeof(x)/sizeof(part_type) - 1)
#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN
#  define LOW_IND(x,part_type)   LAST_IND(x,part_type)
#  define HIGH_IND(x,part_type)  0
#else
#  define HIGH_IND(x,part_type)  LAST_IND(x,part_type)
#  define LOW_IND(x,part_type)   0
#endif
// first unsigned macros:
#define BYTEn(x, n)   (*((_BYTE*)&(x)+n))
#define WORDn(x, n)   (*((_WORD*)&(x)+n))
#define DWORDn(x, n)  (*((_DWORD*)&(x)+n))

#define LOBYTE(x)  BYTEn(x,LOW_IND(x,_BYTE))
#define LOWORD(x)  WORDn(x,LOW_IND(x,_WORD))
#define LODWORD(x) DWORDn(x,LOW_IND(x,_DWORD))
#define HIBYTE(x)  BYTEn(x,HIGH_IND(x,_BYTE))
#define HIWORD(x)  WORDn(x,HIGH_IND(x,_WORD))
#define HIDWORD(x) DWORDn(x,HIGH_IND(x,_DWORD))
#define BYTE1(x)   BYTEn(x,  1)         // byte 1 (counting from 0)
#define BYTE2(x)   BYTEn(x,  2)
#define BYTE3(x)   BYTEn(x,  3)
#define BYTE4(x)   BYTEn(x,  4)
#define BYTE5(x)   BYTEn(x,  5)
#define BYTE6(x)   BYTEn(x,  6)
#define BYTE7(x)   BYTEn(x,  7)
#define BYTE8(x)   BYTEn(x,  8)
#define BYTE9(x)   BYTEn(x,  9)
#define BYTE10(x)  BYTEn(x, 10)
#define BYTE11(x)  BYTEn(x, 11)
#define BYTE12(x)  BYTEn(x, 12)
#define BYTE13(x)  BYTEn(x, 13)
#define BYTE14(x)  BYTEn(x, 14)
#define BYTE15(x)  BYTEn(x, 15)
#define WORD1(x)   WORDn(x,  1)
#define WORD2(x)   WORDn(x,  2)         // third word of the object, unsigned
#define WORD3(x)   WORDn(x,  3)
#define WORD4(x)   WORDn(x,  4)
#define WORD5(x)   WORDn(x,  5)
#define WORD6(x)   WORDn(x,  6)
#define WORD7(x)   WORDn(x,  7)

// now signed macros (the same but with sign extension)
#define SBYTEn(x, n)   (*((int8*)&(x)+n))
#define SWORDn(x, n)   (*((int16*)&(x)+n))
#define SDWORDn(x, n)  (*((int32*)&(x)+n))

#define SLOBYTE(x)  SBYTEn(x,LOW_IND(x,int8))
#define SLOWORD(x)  SWORDn(x,LOW_IND(x,int16))
#define SLODWORD(x) SDWORDn(x,LOW_IND(x,int32))
#define SHIBYTE(x)  SBYTEn(x,HIGH_IND(x,int8))
#define SHIWORD(x)  SWORDn(x,HIGH_IND(x,int16))
#define SHIDWORD(x) SDWORDn(x,HIGH_IND(x,int32))
#define SBYTE1(x)   SBYTEn(x,  1)
#define SBYTE2(x)   SBYTEn(x,  2)
#define SBYTE3(x)   SBYTEn(x,  3)
#define SBYTE4(x)   SBYTEn(x,  4)
#define SBYTE5(x)   SBYTEn(x,  5)
#define SBYTE6(x)   SBYTEn(x,  6)
#define SBYTE7(x)   SBYTEn(x,  7)
#define SBYTE8(x)   SBYTEn(x,  8)
#define SBYTE9(x)   SBYTEn(x,  9)
#define SBYTE10(x)  SBYTEn(x, 10)
#define SBYTE11(x)  SBYTEn(x, 11)
#define SBYTE12(x)  SBYTEn(x, 12)
#define SBYTE13(x)  SBYTEn(x, 13)
#define SBYTE14(x)  SBYTEn(x, 14)
#define SBYTE15(x)  SBYTEn(x, 15)
#define SWORD1(x)   SWORDn(x,  1)
#define SWORD2(x)   SWORDn(x,  2)
#define SWORD3(x)   SWORDn(x,  3)
#define SWORD4(x)   SWORDn(x,  4)
#define SWORD5(x)   SWORDn(x,  5)
#define SWORD6(x)   SWORDn(x,  6)
#define SWORD7(x)   SWORDn(x,  7)

// Generate a pair of operands. S stands for 'signed'
#define __SPAIR16__(high, low)  (((int16)  (high) <<  8) | (uint8) (low))
#define __SPAIR32__(high, low)  (((int32)  (high) << 16) | (uint16)(low))
#define __SPAIR64__(high, low)  (((int64)  (high) << 32) | (uint32)(low))
#define __SPAIR128__(high, low) (((int128) (high) << 64) | (uint64)(low))
#define __PAIR16__(high, low)   (((uint16) (high) <<  8) | (uint8) (low))
#define __PAIR32__(high, low)   (((uint32) (high) << 16) | (uint16)(low))
#define __PAIR64__(high, low)   (((uint64) (high) << 32) | (uint32)(low))
#define __PAIR128__(high, low)  (((uint128)(high) << 64) | (uint64)(low))

// rotate left
template<class T> T __ROL__(T value, int count)
{
    const uint nbits = sizeof(T) * 8;

    if (count > 0)
    {
        count %= nbits;
        T high = value >> (nbits - count);
        if (T(-1) < 0) // signed value
            high &= ~((T(-1) << count));
        value <<= count;
        value |= high;
    }
    else
    {
        count = -count % nbits;
        T low = value << (nbits - count);
        value >>= count;
        value |= low;
    }
    return value;
}

inline uint8  __ROL1__(uint8  value, int count) { return __ROL__((uint8)value, count); }
inline uint16 __ROL2__(uint16 value, int count) { return __ROL__((uint16)value, count); }
inline uint32 __ROL4__(uint32 value, int count) { return __ROL__((uint32)value, count); }
inline uint64 __ROL8__(uint64 value, int count) { return __ROL__((uint64)value, count); }
inline uint8  __ROR1__(uint8  value, int count) { return __ROL__((uint8)value, -count); }
inline uint16 __ROR2__(uint16 value, int count) { return __ROL__((uint16)value, -count); }
inline uint32 __ROR4__(uint32 value, int count) { return __ROL__((uint32)value, -count); }
inline uint64 __ROR8__(uint64 value, int count) { return __ROL__((uint64)value, -count); }


//=============================================
void Com_Memset(void *dest, const int val, const size_t count);
void Com_Memcpy(void *dest, const void *src, const size_t count);

char *QDECL va(const char *format, ...);


//=============================================

enum dvarType_t : __int32
{                                       // XREF: dvar_s/r
                                         // ?Dvar_ValueInDomain@@YA_NW4dvarType_t@@TDvarValue@@TDvarLimits@@@Z/r ...
     DVAR_TYPE_BOOL = 0x0,
     DVAR_TYPE_FLOAT = 0x1,
     DVAR_TYPE_FLOAT_2 = 0x2,
     DVAR_TYPE_FLOAT_3 = 0x3,
     DVAR_TYPE_FLOAT_4 = 0x4,
     DVAR_TYPE_INT = 0x5,
     DVAR_TYPE_ENUM = 0x6,
     DVAR_TYPE_STRING = 0x7,
     DVAR_TYPE_COLOR = 0x8,
     DVAR_TYPE_INT64 = 0x9,
     DVAR_TYPE_LINEAR_COLOR_RGB = 0xA,
     DVAR_TYPE_COLOR_XYZ = 0xB,
     DVAR_TYPE_COUNT = 0xC,
};

union DvarValue // sizeof=0x10
{                                       // XREF: CL_CommitDvarToProfiles+58/w
                                        // CL_CommitDvarToProfiles+5E/w ...
    bool enabled;
    int integer;
    unsigned int unsignedInt;
    __int64 integer64;
    unsigned __int64 unsignedInt64;
    float value;
    float vector[4];
    const char *string;
    unsigned __int8 color[4];
};

struct DvarLimits_enum // sizeof=0x8
{                                       // XREF: DvarLimits/r
    int stringCount;
    const char **strings;
};

struct DvarLimits_int // sizeof=0x8
{                                       // XREF: DvarLimits/r
    int min;
    int max;
};

struct DvarLimits_int64 // sizeof=0x10
{                                       // XREF: DvarLimits/r
    __int64 min;
    __int64 max;
};

struct DvarLimits_float // sizeof=0x8
{                                       // XREF: DvarLimits/r
    float min;
    float max;
};

struct DvarLimits_vector // sizeof=0x8
{                                       // XREF: DvarLimits/r
    float min;
    float max;
};

union DvarLimits // sizeof=0x10
{                                       // XREF: Dvar_ValueInDomain(dvarType_t,DvarValue,DvarLimits):$LN17_36/r
                                        // Dvar_ValueInDomain(dvarType_t,DvarValue,DvarLimits)+61/r ...
    DvarLimits_enum enumeration;
    DvarLimits_int integer;
    DvarLimits_int64 integer64;
    DvarLimits_float value;
    DvarLimits_vector vector;
};

struct __declspec(align(8)) dvar_s // sizeof=0x70
{                                       // XREF: .data:dvarPool/r
                                        // dvar_t/r
    const char *name;
    const char *description;
    int hash;
    unsigned int flags;                 // XREF: Dvar_ResetScriptInfo(void)+4A/r
                                        // Dvar_ResetScriptInfo(void)+5C/w
    dvarType_t type;
    bool modified;
    bool loadedFromSaveGame;
    // padding byte
    // padding byte
    DvarValue current;
    DvarValue latched;
    DvarValue reset;
    DvarValue saved;
    DvarLimits domain;
    dvar_s *hashNext;
    // padding byte
    // padding byte
    // padding byte
    // padding byte
};

union FloatWriteSwap_union // sizeof=0x4
{                                       // ...
    float f;
    int n;
    unsigned __int8 b[4];
};


struct cspField_t // sizeof=0xC
{                                       // XREF: .data:weaponDefFields/r
                                        // .data:destructiblePieceFields/r ...
    const char *szName;                 // XREF: BG_ParsePenetrationDepthTable+13D/w
                                        // G_ParseHitLocDmgTable(void)+5D/w ...
    int iOffset;                        // XREF: BG_ParsePenetrationDepthTable+156/w
                                        // G_ParseHitLocDmgTable(void)+70/w
    int iFieldType;                     // XREF: BG_ParsePenetrationDepthTable+166/w
                                        // G_ParseHitLocDmgTable(void)+7D/w
};

struct orientation_t // sizeof=0x30
{                                       // XREF: .rdata:orientation_t const orIdentity/r
                                        // snd_listener/r ...
    float origin[3];                    // XREF: Weapon_DrawAxisOrigin(orientation_t,float)+EB/r
                                        // Weapon_DrawAxisOrigin(orientation_t,float)+102/r ...
    float axis[3][3];                   // XREF: CG_Laser_Add(centity_s *,DObj *,cpose_t *,float const * const,LaserOwnerEnum)+50/o
                                        // CG_UpdateHelicopterKillCam+152/o ...
};

struct trajectory_t // sizeof=0x24
{                                       // XREF: LerpEntityState/r
                                        // LerpEntityState/r ...
    unsigned __int8 trType;             // XREF: CScriptMover_SetupMoveSpeed+6AD/w
                                        // CScr_LaunchRagdoll+147/w ...
    // padding byte
    // padding byte
    // padding byte
    int trTime;                         // XREF: CScriptMover_SetupMoveSpeed+6C9/w
                                        // CScr_LaunchRagdoll+14E/w ...
    int trDuration;                     // XREF: CScriptMover_SetupMoveSpeed+6DF/w
                                        // CScriptMover_SetupMoveSpeed+7E0/r ...
    float trBase[3];                    // XREF: CScriptMover_SetupMoveSpeed+6E9/w
                                        // CScriptMover_SetupMoveSpeed+6F6/w ...
    float trDelta[3];                   // XREF: CScriptMover_SetupMoveSpeed+70F/w
                                        // CScriptMover_SetupMoveSpeed+71C/w ...
};


struct usercmd_s;

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
FloatWriteSwap_union __cdecl FloatWriteSwap(float f);
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
