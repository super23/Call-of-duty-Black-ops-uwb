#include "q_shared.h"

unsigned __int8 __cdecl ColorIndex(unsigned __int8 c)
{
  if ( (unsigned __int8)(c - 48) >= 0x11u )
    return 7;
  else
    return c - 48;
}

const char *__cdecl Com_GetFilenameSubString(const char *pathname)
{
  const char *last; // [esp+0h] [ebp-4h]

  last = pathname;
  while ( *pathname )
  {
    if ( *pathname == 47 || *pathname == 92 )
      last = pathname + 1;
    ++pathname;
  }
  return last;
}

const char *__cdecl Com_GetExtensionSubString(const char *filename)
{
  const char *substr; // [esp+0h] [ebp-4h]

  if ( !filename
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 201, 0, "%s", "filename") )
  {
    __debugbreak();
  }
  substr = 0;
  while ( *filename )
  {
    if ( *filename == 46 )
    {
      substr = filename;
    }
    else if ( *filename == 47 || *filename == 92 )
    {
      substr = 0;
    }
    ++filename;
  }
  if ( !substr )
    return filename;
  return substr;
}

void __cdecl Com_StripExtension(char *in, char *out)
{
  const char *extension; // [esp+0h] [ebp-4h]

  extension = Com_GetExtensionSubString(in);
  while ( in != extension )
    *out++ = *in++;
  *out = 0;
}

void __cdecl Com_DefaultExtension(char *path, unsigned int maxSize, const char *extension)
{
  char *src; // [esp+10h] [ebp-4Ch]
  char oldPath[68]; // [esp+14h] [ebp-48h] BYREF

  for ( src = &path[strlen(path) - 1]; *src != 47 && src != path; --src )
  {
    if ( *src == 46 )
      return;
  }
  I_strncpyz(oldPath, path, 64);
  Com_sprintf(path, maxSize, "%s%s", oldPath, extension);
}

__int16 __cdecl BigShort(__int16 l)
{
  return BigShort(l);
}

unsigned __int64 __cdecl LittleLong64(unsigned __int64 l)
{
  return LittleLong64(l);
}

int __cdecl ShortSwap(__int16 l)
{
  return HIBYTE(l) + ((unsigned __int8)l << 8);
}

__int16 __cdecl ShortNoSwap(__int16 l)
{
  return l;
}

int __cdecl LongSwap(int l)
{
  return HIBYTE(l) + (BYTE2(l) << 8) + (BYTE1(l) << 16) + ((unsigned __int8)l << 24);
}

unsigned __int64 __cdecl Long64Swap(unsigned __int64 l)
{
  return HIBYTE(l)
       + ((unsigned __int64)BYTE6(l) << 8)
       + ((unsigned __int64)BYTE5(l) << 16)
       + ((unsigned __int64)BYTE4(l) << 24)
       + ((unsigned __int64)BYTE3(l) << 32)
       + ((unsigned __int64)BYTE2(l) << 40)
       + ((unsigned __int64)BYTE1(l) << 48)
       + ((unsigned __int64)(unsigned __int8)l << 56);
}

unsigned __int64 __cdecl Long64NoSwap(unsigned __int64 ll)
{
  return ll;
}

double __cdecl FloatReadSwap(int n)
{
  FloatReadSwap::__l2::<unnamed_type_dat1> dat2; // [esp+4h] [ebp-4h]

  dat2.b[0] = HIBYTE(n);
  dat2.b[1] = BYTE2(n);
  dat2.b[2] = BYTE1(n);
  dat2.b[3] = n;
  return dat2.f;
}

double __cdecl FloatReadNoSwap(int n)
{
  return *(float *)&n;
}

FloatWriteSwap::__l2::<unnamed_type_dat1> __cdecl FloatWriteSwap(float f)
{
  FloatWriteSwap::__l2::<unnamed_type_dat1> dat2; // [esp+4h] [ebp-4h]

  dat2.b[0] = HIBYTE(f);
  dat2.b[1] = BYTE2(f);
  dat2.b[2] = BYTE1(f);
  dat2.b[3] = LOBYTE(f);
  return dat2;
}

void __cdecl Swap_InitLittleEndian()
{
  BigShort = (__int16 (__cdecl *)(__int16))ShortSwap;
  LittleShort = ShortNoSwap;
  BigLong = LongSwap;
  LittleLong = (int (__cdecl *)(int))Ragdoll_HandleBody;
  LittleLong64 = Long64NoSwap;
  LittleFloatRead = (float (__cdecl *)(int))FloatReadNoSwap;
  LittleFloatWrite = (int (__cdecl *)(float))Ragdoll_HandleBody;
}

void __cdecl Swap_InitBigEndian()
{
  BigShort = ShortNoSwap;
  LittleShort = (__int16 (__cdecl *)(__int16))ShortSwap;
  BigLong = (int (__cdecl *)(int))Ragdoll_HandleBody;
  LittleLong = LongSwap;
  LittleLong64 = Long64Swap;
  LittleFloatRead = (float (__cdecl *)(int))FloatReadSwap;
  LittleFloatWrite = (int (__cdecl *)(float))FloatWriteSwap;
}

void __cdecl Swap_Init()
{
  Swap_InitLittleEndian();
}

__int64 __cdecl I_atoi64(const char *str)
{
  return _atoi64(str);
}

bool __cdecl I_islower(int c)
{
  return c >= 97 && c <= 122;
}

bool __cdecl I_isupper(int c)
{
  return c >= 65 && c <= 90;
}

bool __cdecl I_isalpha(int c)
{
  return c >= 97 && c <= 122 || c >= 65 && c <= 90;
}

bool __cdecl I_isdigit(int c)
{
  return c >= 48 && c <= 57;
}

bool __cdecl I_isalnum(int c)
{
  return I_isalpha(c) || I_isdigit(c);
}

bool __cdecl I_isforfilename(int c)
{
  return I_isalnum(c) || c == 95 || c == 45;
}

void __cdecl I_strncpyz(char *dest, const char *src, int destsize)
{
  unsigned __int8 *q; // [esp+0h] [ebp-Ch]
  int n; // [esp+4h] [ebp-8h]
  const unsigned __int8 *p; // [esp+8h] [ebp-4h]

  if ( !src && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 586, 0, "%s", "src") )
    __debugbreak();
  if ( !dest && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 587, 0, "%s", "dest") )
    __debugbreak();
  if ( destsize < 1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp",
          588,
          0,
          "%s\n\t(destsize) = %i",
          "(destsize >= 1)",
          destsize) )
  {
    __debugbreak();
  }
  if ( dest )
  {
    if ( src )
    {
      p = (const unsigned __int8 *)(src - 1);
      q = (unsigned __int8 *)(dest - 1);
      n = destsize + 1;
      do
      {
        if ( !--n )
          break;
        *++q = *++p;
      }
      while ( *q );
      dest[destsize - 1] = 0;
    }
    else
    {
      *dest = 0;
    }
  }
}

int __cdecl I_strnicmp(const char *s0, const char *s1, int n)
{
  int c1; // [esp+0h] [ebp-8h]
  int c0; // [esp+4h] [ebp-4h]

  if ( !s0 && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 653, 0, "%s", "s0") )
    __debugbreak();
  if ( !s1 && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 654, 0, "%s", "s1") )
    __debugbreak();
  if ( !s0 || !s1 )
    return s1 - s0;
  do
  {
    c0 = *(unsigned __int8 *)s0;
    c1 = *(unsigned __int8 *)s1;
    ++s0;
    ++s1;
    if ( !n-- )
      return 0;
    if ( c0 != c1 )
    {
      if ( I_isupper(c0) )
        c0 += 32;
      if ( I_isupper(c1) )
        c1 += 32;
      if ( c0 != c1 )
        return 2 * (c0 >= c1) - 1;
    }
  }
  while ( c0 );
  return 0;
}

int __cdecl I_strncmp(const char *s0, const char *s1, int n)
{
  int c1; // [esp+0h] [ebp-8h]
  int c0; // [esp+4h] [ebp-4h]

  if ( !s0 && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 691, 0, "%s", "s0") )
    __debugbreak();
  if ( !s1 && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 692, 0, "%s", "s1") )
    __debugbreak();
  if ( !s0 || !s1 )
    return s1 - s0;
  do
  {
    c0 = *s0++;
    c1 = *s1++;
    if ( !n-- )
      return 0;
    if ( c0 != c1 )
      return 2 * (c0 >= c1) - 1;
  }
  while ( c0 );
  return 0;
}

const char *__cdecl I_stristr(const char *s0, const char *substr)
{
  int v3; // esi
  int s0Char; // [esp+4h] [ebp-8h]
  int substrChar; // [esp+8h] [ebp-4h]

  if ( !s0 && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 721, 0, "%s", "s0") )
    __debugbreak();
  if ( !substr
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 722, 0, "%s", "substr") )
  {
    __debugbreak();
  }
  if ( !s0 || !substr )
    return 0;
  for ( s0Char = 0; s0[s0Char]; ++s0Char )
  {
    substrChar = -1;
    do
    {
      if ( !substr[++substrChar] )
        return &s0[s0Char];
      v3 = tolower(s0[substrChar + s0Char]);
    }
    while ( v3 == tolower(substr[substrChar]) );
  }
  return 0;
}

int __cdecl I_stricmp(const char *s0, const char *s1)
{
  if ( !s0 && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 747, 0, "%s", "s0") )
    __debugbreak();
  if ( !s1 && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 748, 0, "%s", "s1") )
    __debugbreak();
  return I_strnicmp(s0, s1, 0x7FFFFFFF);
}

int __cdecl I_strcmp(const char *s0, const char *s1)
{
  if ( !s0 && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 756, 0, "%s", "s0") )
    __debugbreak();
  if ( !s1 && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 757, 0, "%s", "s1") )
    __debugbreak();
  return I_strncmp(s0, s1, 0x7FFFFFFF);
}

int __cdecl I_stricmpwild(const char *wild, const char *s)
{
  int v3; // esi
  char charWild; // [esp+7h] [ebp-9h]
  int delta; // [esp+8h] [ebp-8h]
  char charRef; // [esp+Fh] [ebp-1h]

  if ( !wild && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 768, 0, "%s", "wild") )
    __debugbreak();
  if ( !s && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 769, 0, "%s", "s") )
    __debugbreak();
  do
  {
    charWild = *wild++;
    if ( charWild == 42 )
    {
      if ( !*wild )
        return 0;
      if ( *s && !I_stricmpwild(wild - 1, s + 1) )
        return 0;
    }
    else
    {
      charRef = *s++;
      if ( charWild != charRef && charWild != 63 )
      {
        v3 = tolower(charWild);
        delta = v3 - tolower(charRef);
        if ( delta )
          return 2 * (delta >= 0) - 1;
      }
    }
  }
  while ( charWild );
  return 0;
}

char *__cdecl I_strlwr(char *s)
{
  char *iter; // [esp+0h] [ebp-4h]

  for ( iter = s; *iter; ++iter )
  {
    if ( I_isupper(*iter) )
      *iter += 32;
  }
  return s;
}

char *__cdecl I_strupr(char *s)
{
  char *iter; // [esp+0h] [ebp-4h]

  for ( iter = s; *iter; ++iter )
  {
    if ( I_islower(*iter) )
      *iter -= 32;
  }
  return s;
}

bool __cdecl I_iscsym(int c)
{
  if ( c >= 97 && c <= 122 )
    return 1;
  if ( c >= 65 && c <= 90 )
    return 1;
  if ( c < 48 || c > 57 )
    return c == 95;
  return 1;
}

void __cdecl I_strncat(char *dest, int size, const char *src)
{
  int destLen; // [esp+10h] [ebp-4h]

  if ( size == 4
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp",
          846,
          0,
          "%s",
          "size != sizeof( char * )") )
  {
    __debugbreak();
  }
  destLen = strlen(dest);
  if ( destLen >= size )
    Com_Error(ERR_FATAL, &byte_D0BE18);
  I_strncpyz(&dest[destLen], src, size - destLen);
}

int __cdecl I_DrawStrlen(const char *str)
{
  int count; // [esp+4h] [ebp-4h]

  count = 0;
  while ( *str )
  {
    if ( str && *str == 94 && str[1] && str[1] != 94 && str[1] >= 48 && str[1] <= 64 )
    {
      str += 2;
    }
    else
    {
      ++count;
      ++str;
    }
  }
  return count;
}

char *__cdecl I_CleanStr(char *string)
{
  char *d; // [esp+0h] [ebp-10h]
  char c; // [esp+7h] [ebp-9h]
  char *s; // [esp+8h] [ebp-8h]
  int keep_cleaning; // [esp+Ch] [ebp-4h]

  do
  {
    s = string;
    d = string;
    keep_cleaning = 0;
    while ( 1 )
    {
      c = *s;
      if ( !*s )
        break;
      if ( s && *s == 94 && s[1] && s[1] != 94 && s[1] >= 48 && s[1] <= 64 )
      {
        ++s;
        keep_cleaning = 1;
      }
      else if ( c >= 32 && c != 127 )
      {
        *d++ = c;
      }
      ++s;
    }
    *d = 0;
  }
  while ( keep_cleaning );
  return string;
}

unsigned __int8 __cdecl I_CleanChar(unsigned __int8 character)
{
  if ( character == 146 )
    return 39;
  else
    return character;
}

int Com_sprintf(char *dest, unsigned int size, const char *fmt, ...)
{
  int result; // eax
  va_list va; // [esp+1Ch] [ebp+14h] BYREF

  va_start(va, fmt);
  result = _vsnprintf(dest, size, fmt, va);
  dest[size - 1] = 0;
  return result;
}

int Com_sprintfPos(char *dest, int destSize, int *destPos, const char *fmt, ...)
{
  int len; // [esp+4h] [ebp-Ch]
  char *destMod; // [esp+8h] [ebp-8h]
  unsigned int destModSize; // [esp+Ch] [ebp-4h]
  va_list va; // [esp+28h] [ebp+18h] BYREF

  va_start(va, fmt);
  if ( *destPos >= destSize - 1 )
    return -1;
  destMod = &dest[*destPos];
  destModSize = destSize - *destPos;
  len = _vsnprintf(destMod, destModSize, fmt, va);
  destMod[destModSize - 1] = 0;
  if ( len == destModSize || len == -1 )
    *destPos = destSize - 1;
  else
    *destPos += len;
  return len;
}

bool __cdecl CanKeepStringPointer(char *string)
{
  va_info_t *info; // [esp+0h] [ebp-8h]
  char stack; // [esp+7h] [ebp-1h] BYREF

  if ( string >= &stack && string < (char *)&STACK[0x2007] )
    return 0;
  info = (va_info_t *)Sys_GetValue(1);
  return string < (char *)info || string > &info->va_string[3][1023];
}

char *__cdecl I_itoa(int value, char *buf, int bufsize)
{
  int v4; // [esp+0h] [ebp-Ch]
  unsigned int uvalue; // [esp+4h] [ebp-8h]
  char *p; // [esp+8h] [ebp-4h]

  p = buf;
  if ( bufsize < 12
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp",
          1004,
          0,
          "%s",
          "bufsize >= ITOA_BUFSIZE") )
  {
    __debugbreak();
  }
  if ( bufsize >= 12 )
  {
    if ( value >= 0 )
      v4 = value;
    else
      v4 = -value;
    uvalue = v4;
    do
    {
      *p++ = uvalue % 0xA + 48;
      uvalue /= 0xAu;
    }
    while ( uvalue );
    if ( value < 0 )
      *p++ = 45;
    *p = 0;
    strreverse(buf, p - 1);
    return buf;
  }
  else
  {
    *buf = 48;
    buf[1] = 0;
    return buf;
  }
}

void __cdecl strreverse(char *begin, char *end)
{
  char aux; // [esp+3h] [ebp-1h]

  while ( end > begin )
  {
    aux = *end;
    *end-- = *begin;
    *begin++ = aux;
  }
}

char *va(const char *format, ...)
{
  va_info_t *info; // [esp+0h] [ebp-14h]
  char *buf; // [esp+8h] [ebp-Ch]
  unsigned int len; // [esp+Ch] [ebp-8h]
  int index; // [esp+10h] [ebp-4h]
  va_list v6; // [esp+20h] [ebp+Ch] BYREF

  va_start(v6, format);
  info = (va_info_t *)Sys_GetValue(1);
  index = info->index;
  info->index = (index + 1) % 4;
  buf = info->va_string[index];
  len = _vsnprintf(buf, 0x400u, format, v6);
  buf[1023] = 0;
  if ( len >= 0x400 )
  {
    buf[1023] = 0;
    Com_Error(ERR_DROP, &byte_D0BE6C, buf);
  }
  return (char *)info + 1024 * index;
}

void __cdecl Com_InitThreadData(int threadContext)
{
  Sys_SetValue(1, &va_info[threadContext]);
  Sys_SetValue(2, g_com_error[threadContext]);
  Sys_SetValue(3, &g_traceThreadInfo[threadContext]);
  if ( threadContext == 1 )
    Sys_SetValue(4, &g_cmd_args[1]);
  else
    Sys_SetValue(4, g_cmd_args);
}

char *__cdecl Info_ValueForKey(char *s, char *key)
{
  char *v3; // [esp+0h] [ebp-2010h]
  char *v4; // [esp+0h] [ebp-2010h]
  char *v5; // [esp+4h] [ebp-200Ch]
  char s1[8196]; // [esp+8h] [ebp-2008h] BYREF
  const char *v7; // [esp+2018h] [ebp+8h]

  if ( !s || !key )
    return (char *)&toastPopupTitle;
  ++valueindex;
  valueindex %= 2;
  if ( *s == 92 )
    ++s;
  while ( 1 )
  {
    v3 = s1;
    while ( *s != 92 )
    {
      if ( !*s )
        return (char *)&toastPopupTitle;
      *v3++ = *s++;
      if ( v3 - s1 >= 0x2000 )
        Com_Error(ERR_DROP, &byte_D0BEA0, v3 - s1);
    }
    *v3 = 0;
    v7 = s + 1;
    if ( Sys_IsMainThread() )
    {
      v5 = value1[0][valueindex];
    }
    else if ( Sys_IsServerThread() )
    {
      v5 = (char *)((valueindex << 13) + 162725720);
    }
    else
    {
      if ( !Sys_IsRenderThread()
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp",
              1280,
              0,
              "%s",
              "Sys_IsRenderThread()") )
      {
        __debugbreak();
      }
      v5 = (char *)((valueindex << 13) + 162709336);
    }
    v4 = v5;
    while ( *v7 != 92 && *v7 )
    {
      *v4++ = *v7++;
      if ( v4 - v5 >= 0x2000 )
        Com_Error(ERR_DROP, &byte_D0BEA0, v4 - v5);
    }
    *v4 = 0;
    if ( !I_stricmp(key, s1) )
      return v5;
    if ( !*v7 )
      break;
    s = (char *)(v7 + 1);
  }
  return (char *)&toastPopupTitle;
}

void __cdecl Info_NextPair(const char **head, char *key, char *value)
{
  char *o; // [esp+0h] [ebp-8h]
  char *oa; // [esp+0h] [ebp-8h]
  const char *s; // [esp+4h] [ebp-4h]
  const char *sa; // [esp+4h] [ebp-4h]

  s = *head;
  if ( **head == 92 )
    ++s;
  *key = 0;
  *value = 0;
  o = key;
  while ( *s != 92 )
  {
    if ( !*s )
    {
      *o = 0;
      *head = s;
      return;
    }
    *o++ = *s++;
  }
  *o = 0;
  sa = s + 1;
  oa = value;
  while ( *sa != 92 && *sa )
    *oa++ = *sa++;
  *oa = 0;
  *head = sa;
}

void __cdecl Info_RemoveKey(char *s, char *key)
{
  int v2; // eax
  char v3; // al
  char *v4; // [esp+8h] [ebp-83Ch]
  char *v5; // [esp+Ch] [ebp-838h]
  char *o; // [esp+34h] [ebp-810h]
  char *oa; // [esp+34h] [ebp-810h]
  char *start; // [esp+38h] [ebp-80Ch]
  char pkey[1024]; // [esp+3Ch] [ebp-808h] BYREF
  char value[1024]; // [esp+43Ch] [ebp-408h] BYREF

  if ( strlen(s) >= 0x400 )
    Com_Error(ERR_DROP, &byte_D0BEC4);
  strchr((unsigned __int8 *)key, 0x5Cu);
  if ( !v2 )
  {
    while ( 1 )
    {
      start = s;
      if ( *s == 92 )
        ++s;
      o = pkey;
      while ( *s != 92 )
      {
        if ( !*s )
          return;
        *o++ = *s++;
      }
      *o = 0;
      ++s;
      oa = value;
      while ( *s != 92 && *s )
        *oa++ = *s++;
      *oa = 0;
      if ( !strcmp(key, pkey) )
        break;
      if ( !*s )
        return;
    }
    v5 = s;
    v4 = start;
    do
    {
      v3 = *v5;
      *v4++ = *v5++;
    }
    while ( v3 );
  }
}

void __cdecl Info_RemoveKey_Big(char *s, char *key)
{
  int v2; // eax
  char v3; // al
  char *v4; // [esp+8h] [ebp-403Ch]
  char *v5; // [esp+Ch] [ebp-4038h]
  char *v6; // [esp+34h] [ebp-4010h]
  char *v7; // [esp+34h] [ebp-4010h]
  char *v8; // [esp+38h] [ebp-400Ch]
  char v9[8192]; // [esp+3Ch] [ebp-4008h] BYREF
  char v10; // [esp+203Ch] [ebp-2008h] BYREF

  if ( strlen(s) >= 0x4000 )
    Com_Error(ERR_DROP, &byte_D0BEEC);
  strchr((unsigned __int8 *)key, 0x5Cu);
  if ( !v2 )
  {
    while ( 1 )
    {
      v8 = s;
      if ( *s == 92 )
        ++s;
      v6 = v9;
      while ( *s != 92 )
      {
        if ( !*s )
          return;
        *v6++ = *s++;
      }
      *v6 = 0;
      ++s;
      v7 = &v10;
      while ( *s != 92 && *s )
        *v7++ = *s++;
      *v7 = 0;
      if ( !strcmp(key, v9) )
        break;
      if ( !*s )
        return;
    }
    v5 = s;
    v4 = v8;
    do
    {
      v3 = *v5;
      *v4++ = *v5++;
    }
    while ( v3 );
  }
}

bool __cdecl Info_Validate(char *s)
{
  int v1; // eax
  int v3; // eax

  strchr((unsigned __int8 *)s, 0x22u);
  if ( v1 )
    return 0;
  strchr((unsigned __int8 *)s, 0x3Bu);
  return v3 == 0;
}

void __cdecl Info_SetValueForKey(char *s, char *key, const char *value)
{
  int v3; // eax
  const char *v4; // eax
  int v5; // eax
  const char *v6; // eax
  int v7; // eax
  const char *v8; // eax
  const char *v9; // eax
  int j; // [esp+54h] [ebp-818h]
  char c; // [esp+5Bh] [ebp-811h]
  char cleanValue[1028]; // [esp+5Ch] [ebp-810h] BYREF
  int len; // [esp+460h] [ebp-40Ch]
  char newi[1024]; // [esp+464h] [ebp-408h] BYREF
  int i; // [esp+868h] [ebp-4h]

  if ( !value && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 1508, 0, "%s", "value") )
    __debugbreak();
  if ( strlen(s) < 0x400 )
  {
    j = 0;
    for ( i = 0; i < 1023; ++i )
    {
      c = value[i];
      if ( !c )
        break;
      if ( c != 92 && c != 59 && c != 34 )
      {
        if ( j >= 1024
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp",
                1525,
                0,
                "%s",
                "j < MAX_INFO_STRING") )
        {
          __debugbreak();
        }
        cleanValue[j++] = c;
      }
    }
    if ( j >= 1024
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp",
            1530,
            0,
            "%s",
            "j < MAX_INFO_STRING") )
    {
      __debugbreak();
    }
    cleanValue[j] = 0;
    strchr((unsigned __int8 *)key, 0x5Cu);
    if ( v3 )
    {
      v4 = va("Can't use keys with a \\\nkey: '%s'\nvalue: '%s'", key, value);
      if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 1535, 0, v4) )
        __debugbreak();
      Com_Printf(16, &byte_D0C110, key, value);
    }
    else
    {
      strchr((unsigned __int8 *)key, 0x3Bu);
      if ( v5 )
      {
        v6 = va("Can't use keys with a semicolon\nkey: '%s'\nvalue: '%s'", key, value);
        if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 1542, 0, v6) )
          __debugbreak();
        Com_Printf(16, &byte_D0C0A0, key, value);
      }
      else
      {
        strchr((unsigned __int8 *)key, 0x22u);
        if ( v7 )
        {
          v8 = va("Can't use keys with a \"\nkey: '%s'\nvalue: '%s'", key, value);
          if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 1549, 0, v8) )
            __debugbreak();
          Com_Printf(16, &byte_D0C040, key, value);
        }
        else
        {
          Info_RemoveKey(s, key);
          if ( cleanValue[0] )
          {
            len = Com_sprintf(newi, 0x400u, "\\%s\\%s", key, cleanValue);
            if ( len > 0 )
            {
              if ( strlen(s) + &newi[strlen(newi) + 1] - &newi[1] <= 0x400 )
              {
                memcpy(&s[strlen(s)], newi, &newi[strlen(newi) + 1] - newi);
              }
              else
              {
                v9 = va("Info string length exceeded\nkey: '%s'\nvalue: '%s'\nInfo string:\n%s\n", key, value, s);
                if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 1569, 0, v9) )
                  __debugbreak();
                Com_Printf(16, &byte_D0BF18, key, value, s);
              }
            }
            else
            {
              if ( !Assert_MyHandler(
                      "C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp",
                      1562,
                      0,
                      "Info buffer length exceeded, not including key/value pair in response\n") )
                __debugbreak();
              Com_Printf(16, &byte_D0BFA8);
            }
          }
        }
      }
    }
  }
  else
  {
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp",
            1512,
            0,
            "Info_SetValueForKey: oversize infostring") )
      __debugbreak();
    Com_Printf(16, &byte_D0C184);
  }
}

void __cdecl Info_SetValueForKey_Big(char *s, char *key, const char *value)
{
  int v3; // eax
  int v4; // eax
  int v5; // eax
  int v6; // [esp+54h] [ebp-8018h]
  char v7; // [esp+5Bh] [ebp-8011h]
  char v8[16388]; // [esp+5Ch] [ebp-8010h] BYREF
  int v9; // [esp+4060h] [ebp-400Ch]
  char dest; // [esp+4064h] [ebp-4008h] BYREF
  _BYTE v11[3]; // [esp+4065h] [ebp-4007h] BYREF
  int i; // [esp+8068h] [ebp-4h]

  if ( !value && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 1594, 0, "%s", "value") )
    __debugbreak();
  if ( strlen(s) < 0x4000 )
  {
    v6 = 0;
    for ( i = 0; i < 0x3FFF; ++i )
    {
      v7 = value[i];
      if ( !v7 )
        break;
      if ( v7 != 92 && v7 != 59 && v7 != 34 )
      {
        if ( v6 >= 0x4000
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp",
                1610,
                0,
                "%s",
                "j < BIG_INFO_STRING") )
        {
          __debugbreak();
        }
        v8[v6++] = v7;
      }
    }
    if ( v6 >= 0x4000
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp",
            1615,
            0,
            "%s",
            "j < BIG_INFO_STRING") )
    {
      __debugbreak();
    }
    v8[v6] = 0;
    strchr((unsigned __int8 *)key, 0x5Cu);
    if ( v3 )
    {
      Com_Printf(16, &byte_D0C110, key, value);
    }
    else
    {
      strchr((unsigned __int8 *)key, 0x3Bu);
      if ( v4 )
      {
        Com_Printf(16, &byte_D0C0A0, key, value);
      }
      else
      {
        strchr((unsigned __int8 *)key, 0x22u);
        if ( v5 )
        {
          Com_Printf(16, &byte_D0C040, key, value);
        }
        else
        {
          Info_RemoveKey_Big(s, key);
          if ( v8[0] )
          {
            v9 = Com_sprintf(&dest, 0x4000u, "\\%s\\%s", key, v8);
            if ( v9 > 0 )
            {
              if ( strlen(s) + &v11[strlen(&dest)] - v11 <= 0x4000 )
                memcpy(&s[strlen(s)], &dest, &v11[strlen(&dest)] - &dest);
              else
                Com_Printf(16, &byte_D0C1E0, key, value, s);
            }
            else
            {
              Com_Printf(16, &byte_D0BFA8);
            }
          }
        }
      }
    }
  }
  else
  {
    Com_Printf(16, &byte_D0C184);
  }
}

bool __cdecl ParseConfigStringToStruct(
        unsigned __int8 *pStruct,
        const cspField_t *pFieldList,
        int iNumFields,
        char *pszBuffer,
        int iMaxFieldTypes,
        int (__cdecl *parseSpecialFieldType)(unsigned __int8 *, const char *, const int, const int),
        void (__cdecl *parseStrCpy)(unsigned __int8 *, const char *))
{
  char *pszKeyValue; // [esp+4h] [ebp-10h]
  char error; // [esp+Bh] [ebp-9h]
  int iField; // [esp+10h] [ebp-4h]

  error = 0;
  for ( iField = 0; iField < iNumFields; ++iField )
  {
    pszKeyValue = Info_ValueForKey(pszBuffer, (char *)pFieldList->szName);
    if ( *pszKeyValue )
      error |= !KeyValueToField(pStruct, pFieldList, pszKeyValue, iMaxFieldTypes, parseSpecialFieldType, parseStrCpy);
    ++pFieldList;
  }
  return iField == iNumFields && !error;
}

bool __cdecl KeyValueToField(
        unsigned __int8 *pStruct,
        const cspField_t *pField,
        char *pszKeyValue,
        int iMaxFieldTypes,
        int (__cdecl *parseSpecialFieldType)(unsigned __int8 *, const char *, const int, const int),
        void (__cdecl *parseStrcpy)(unsigned __int8 *, const char *))
{
  bool result; // eax
  const char *v7; // eax
  const char *v8; // eax
  float v9; // [esp+0h] [ebp-2110h]
  char name[256]; // [esp+8h] [ebp-2108h] BYREF
  char dest[8192]; // [esp+108h] [ebp-2008h] BYREF
  XModel *v12; // [esp+210Ch] [ebp-4h]

  if ( pField->iFieldType >= 15 )
  {
    if ( iMaxFieldTypes <= 0 || pField->iFieldType >= iMaxFieldTypes )
    {
      v8 = va("Bad field type %i\n", pField->iFieldType);
      if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 1785, 0, v8) )
        __debugbreak();
      Com_Error(ERR_DROP, &byte_D0C23C, pField->iFieldType);
      return 0;
    }
    else
    {
      if ( !parseSpecialFieldType
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp",
              1779,
              0,
              "%s",
              "parseSpecialFieldType != NULL") )
      {
        __debugbreak();
      }
      return parseSpecialFieldType(pStruct, pszKeyValue, pField->iFieldType, pField->iOffset) != 0;
    }
  }
  else
  {
    switch ( pField->iFieldType )
    {
      case 0:
        parseStrcpy(&pStruct[pField->iOffset], pszKeyValue);
        return 1;
      case 1:
        I_strncpyz((char *)&pStruct[pField->iOffset], pszKeyValue, 1024);
        return 1;
      case 2:
        I_strncpyz((char *)&pStruct[pField->iOffset], pszKeyValue, 64);
        return 1;
      case 3:
        I_strncpyz((char *)&pStruct[pField->iOffset], pszKeyValue, 256);
        return 1;
      case 4:
        *(unsigned int *)&pStruct[pField->iOffset] = atoi(pszKeyValue);
        return 1;
      case 5:
        pStruct[pField->iOffset] = atoi(pszKeyValue) != 0;
        return 1;
      case 6:
        *(unsigned int *)&pStruct[pField->iOffset] = atoi(pszKeyValue) != 0;
        return 1;
      case 7:
        v9 = atof(pszKeyValue);
        *(float *)&pStruct[pField->iOffset] = v9;
        return 1;
      case 8:
        *(unsigned int *)&pStruct[pField->iOffset] = (int)(atof(pszKeyValue) * 1000.0);
        return 1;
      case 9:
      case 0xB:
      case 0xC:
        return 1;
      case 0xA:
        I_strncpyz(dest, pszKeyValue, 0x2000);
        v12 = R_RegisterModel(dest);
        *(unsigned int *)&pStruct[pField->iOffset] = v12;
        if ( v12 )
          return 1;
        result = 0;
        break;
      case 0xD:
        I_strncpyz(name, pszKeyValue, 245);
        *(unsigned int *)&pStruct[pField->iOffset] = PhysPreset_Register(name);
        return 1;
      case 0xE:
        *(_WORD *)&pStruct[pField->iOffset] = SL_GetLowercaseString(pszKeyValue, 0, SCRIPTINSTANCE_SERVER);
        return 1;
      default:
        if ( pField->iFieldType >= 0 )
        {
          if ( !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp",
                  1774,
                  0,
                  "ParseConfigStringToStruct is out of sync with the csParseFieldType_t enum list\n") )
            __debugbreak();
        }
        else
        {
          v7 = va("Negative field type %i given to ParseConfigStringToStruct\n", pField->iFieldType);
          if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 1772, 0, v7) )
            __debugbreak();
        }
        return 1;
    }
  }
  return result;
}

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
        int (__cdecl *parseMergeSpecialCase)(const char *, char **, char *, int))
{
  char *v11; // eax
  char v12; // bl
  int i; // [esp+68h] [ebp-602Ch]
  char v15; // [esp+6Ch] [ebp-6028h] BYREF
  char v16; // [esp+2073h] [ebp-4021h]
  char v17; // [esp+2074h] [ebp-4020h] BYREF
  char *value; // [esp+4078h] [ebp-201Ch]
  char v19; // [esp+407Ch] [ebp-2018h] BYREF
  cspField_t *pField; // [esp+6080h] [ebp-14h]
  int v21; // [esp+6084h] [ebp-10h]
  char *dest; // [esp+6088h] [ebp-Ch] BYREF
  char *v23; // [esp+608Ch] [ebp-8h]
  char *v24; // [esp+6090h] [ebp-4h]

  dest = &v19;
  v23 = &v17;
  v24 = &v15;
  v16 = 0;
  v21 = 0;
  pField = pFieldList;
  while ( v21 < iNumFields )
  {
    for ( i = 0; i < 3; ++i )
    {
      v11 = Info_ValueForKey((char *)pszBuffer[i], (char *)pField->szName);
      I_strncpyz((&dest)[i], v11, 0x2000);
    }
    if ( !strcmp(v23, v24) )
    {
      value = v23;
    }
    else if ( strcmp(dest, v24) || !strcmp(dest, v23) )
    {
      if ( strcmp(dest, v23) || !strcmp(dest, v24) )
      {
        if ( !parseMergeSpecialCase(pField->szName, &dest, dest, 0x2000) )
        {
          Com_PrintError(
            1,
            "Could not merge field '%s' for merged asset '%s'.\nBase: %s -> %s\nLeft: %s -> %s\nRight: %s -> %s\n",
            pField->szName,
            mergedName,
            *sourceName,
            dest,
            sourceName[1],
            v23,
            sourceName[2],
            v24);
          v16 = 1;
        }
        value = dest;
      }
      else
      {
        value = v24;
      }
    }
    else
    {
      value = v23;
    }
    if ( *value )
    {
      Info_SetValueForKey_Big(pszMergedBuffer, (char *)pField->szName, value);
      v12 = v16;
      v16 = !KeyValueToField(pStruct, pField, value, iMaxFieldTypes, parseSpecialFieldType, parseStrCpy) | v12;
    }
    ++v21;
    ++pField;
  }
  return v21 == iNumFields && !v16;
}

double __cdecl GetLeanFraction(float fFrac)
{
  float v2; // [esp+0h] [ebp-4h]

  v2 = fabs(fFrac);
  return (2.0 - v2) * fFrac;
}

double __cdecl UnGetLeanFraction(float fFrac)
{
  float v2; // [esp+0h] [ebp-4h]

  if ( fFrac < 0.0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 1946, 0, "%s", "fFrac >= 0") )
  {
    __debugbreak();
  }
  if ( fFrac > 1.0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 1947, 0, "%s", "fFrac <= 1.f") )
  {
    __debugbreak();
  }
  v2 = sqrt(1.0 - fFrac);
  return 1.0 - v2;
}

void __cdecl AddLeanToPosition(float *position, float fViewYaw, float fLeanFrac, float fViewRoll, float fLeanDist)
{
  float fLean; // [esp+4h] [ebp-1Ch]
  float fLeana; // [esp+4h] [ebp-1Ch]
  float vRight[3]; // [esp+8h] [ebp-18h] BYREF
  float vAng[3]; // [esp+14h] [ebp-Ch] BYREF

  if ( fLeanFrac != 0.0 )
  {
    fLean = GetLeanFraction(fLeanFrac);
    vAng[0] = *(float *)&FLOAT_0_0;
    vAng[1] = fViewYaw;
    vAng[2] = fLean * fViewRoll;
    AngleVectors(vAng, 0, vRight, 0);
    fLeana = fLean * fLeanDist;
    *position = (float)(fLeana * vRight[0]) + *position;
    position[1] = (float)(fLeana * vRight[1]) + position[1];
    position[2] = (float)(fLeana * vRight[2]) + position[2];
  }
}

void __cdecl OrientationPosToWorldPos(const orientation_t *orient, const float *pos, float *out)
{
  if ( pos == out
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 1993, 0, "%s", "pos != out") )
  {
    __debugbreak();
  }
  *out = (float)((float)((float)(*pos * orient->axis[0][0]) + orient->origin[0]) + (float)(pos[1] * orient->axis[1][0]))
       + (float)(pos[2] * orient->axis[2][0]);
  out[1] = (float)((float)((float)(*pos * orient->axis[0][1]) + orient->origin[1]) + (float)(pos[1] * orient->axis[1][1]))
         + (float)(pos[2] * orient->axis[2][1]);
  out[2] = (float)((float)((float)(*pos * orient->axis[0][2]) + orient->origin[2]) + (float)(pos[1] * orient->axis[1][2]))
         + (float)(pos[2] * orient->axis[2][2]);
}

void __cdecl OrientationDirToWorldDir(const orientation_t *orient, const float *dir, float *out)
{
  if ( dir == out
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 2002, 0, "%s", "dir != out") )
  {
    __debugbreak();
  }
  *out = (float)((float)(*dir * orient->axis[0][0]) + (float)(dir[1] * orient->axis[1][0]))
       + (float)(dir[2] * orient->axis[2][0]);
  out[1] = (float)((float)(*dir * orient->axis[0][1]) + (float)(dir[1] * orient->axis[1][1]))
         + (float)(dir[2] * orient->axis[2][1]);
  out[2] = (float)((float)(*dir * orient->axis[0][2]) + (float)(dir[1] * orient->axis[1][2]))
         + (float)(dir[2] * orient->axis[2][2]);
}

void __cdecl OrientationConcatenate(const orientation_t *orFirst, const orientation_t *orSecond, orientation_t *out)
{
  if ( out == orFirst
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 2072, 0, "%s", "out != orFirst") )
  {
    __debugbreak();
  }
  if ( out == orSecond
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\q_shared.cpp", 2073, 0, "%s", "out != orSecond") )
  {
    __debugbreak();
  }
  OrientationDirToWorldDir(orSecond, orFirst->axis[0], out->axis[0]);
  OrientationDirToWorldDir(orSecond, orFirst->axis[1], out->axis[1]);
  OrientationDirToWorldDir(orSecond, orFirst->axis[2], out->axis[2]);
  OrientationPosToWorldPos(orSecond, orFirst->origin, out->origin);
}

void __cdecl OrientationInvert(const orientation_t *orient, orientation_t *out)
{
  float origin; // [esp+0h] [ebp-Ch]
  float origin_4; // [esp+4h] [ebp-8h]
  float origin_8; // [esp+8h] [ebp-4h]

  out->axis[0][0] = orient->axis[0][0];
  out->axis[0][1] = orient->axis[1][0];
  out->axis[0][2] = orient->axis[2][0];
  out->axis[1][0] = orient->axis[0][1];
  out->axis[1][1] = orient->axis[1][1];
  out->axis[1][2] = orient->axis[2][1];
  out->axis[2][0] = orient->axis[0][2];
  out->axis[2][1] = orient->axis[1][2];
  out->axis[2][2] = orient->axis[2][2];
  origin = 0.0 - orient->origin[0];
  origin_4 = 0.0 - orient->origin[1];
  origin_8 = 0.0 - orient->origin[2];
  out->origin[0] = (float)((float)(origin * out->axis[0][0]) + (float)(origin_4 * out->axis[1][0]))
                 + (float)(origin_8 * out->axis[2][0]);
  out->origin[1] = (float)((float)(origin * out->axis[0][1]) + (float)(origin_4 * out->axis[1][1]))
                 + (float)(origin_8 * out->axis[2][1]);
  out->origin[2] = (float)((float)(origin * out->axis[0][2]) + (float)(origin_4 * out->axis[1][2]))
                 + (float)(origin_8 * out->axis[2][2]);
}

bool __cdecl Com_IsLegacyXModelName(const char *name)
{
  return !I_strnicmp(name, "xmodel", 6) && (name[6] == 47 || name[6] == 92);
}

double __cdecl CMD_GetAnalogButtonValue(usercmd_s *cmd, unsigned int whichBit)
{
  int i; // [esp+0h] [ebp-10h]
  int val; // [esp+4h] [ebp-Ch]
  int numBits; // [esp+Ch] [ebp-4h]

  numBits = 1;
  if ( whichBit == 19 )
    numBits = 3;
  if ( numBits == 1 )
  {
    if ( bitarray<51>::testBit(&cmd->button_bits, whichBit) )
      return 1.0;
    else
      return 0.0;
  }
  else
  {
    val = 0;
    for ( i = 0; i < numBits; ++i )
    {
      if ( bitarray<51>::testBit(&cmd->button_bits, i + whichBit) )
        val |= 1 << i;
    }
    return (double)val / (double)((1 << numBits) - 1);
  }
}

