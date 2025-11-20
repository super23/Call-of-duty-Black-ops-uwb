#include "mem_userhunk.h"

HunkUser *__cdecl Hunk_UserDebugInit(
        unsigned int *buffer,
        int size,
        HU_ALLOCATION_SCHEME scheme,
        unsigned int flags,
        void *scheme_specific_data,
        const char *name,
        int type)
{
  if ( scheme != HU_SCHEME_DEBUG
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          109,
          0,
          "%s",
          "scheme==HU_SCHEME_DEBUG") )
  {
    __debugbreak();
  }
  if ( !Sys_IsMainThread()
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          110,
          0,
          "%s",
          "Sys_IsMainThread()") )
  {
    __debugbreak();
  }
  *buffer = 0;
  buffer[1] = 0;
  buffer[2] = 0;
  buffer[3] = 0;
  buffer[4] = 0;
  buffer[4] = Hunk_FirstFitInit(buffer + 5, size - 20, HU_SCHEME_FIRSTFIT, flags, 0, name, type);
  *buffer = scheme;
  buffer[1] = flags;
  buffer[2] = name;
  buffer[3] = type;
  return (HunkUser *)buffer;
}

void __cdecl Hunk_UserDebugReset(HunkUser *_user)
{
  Hunk_FirstFitReset((HunkUser *)_user[1].scheme);
}

void __cdecl Hunk_UserDebugDestroy(HunkUser *_user)
{
  if ( _user->scheme != HU_SCHEME_DEBUG
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          136,
          0,
          "%s",
          "user->hunkUser.scheme==HU_SCHEME_DEBUG") )
  {
    __debugbreak();
  }
  if ( !Sys_IsMainThread()
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          137,
          0,
          "%s",
          "Sys_IsMainThread()") )
  {
    __debugbreak();
  }
  Z_VirtualFree(_user, _user->type);
}

int __cdecl Hunk_UserDebugAlloc(HunkUser *_user, int size, int alignment)
{
  if ( !_user
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp", 155, 0, "%s", "user") )
  {
    __debugbreak();
  }
  if ( _user->scheme != HU_SCHEME_DEBUG
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          156,
          0,
          "%s",
          "user->hunkUser.scheme==HU_SCHEME_DEBUG") )
  {
    __debugbreak();
  }
  if ( !Sys_IsMainThread()
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          157,
          0,
          "%s",
          "Sys_IsMainThread()") )
  {
    __debugbreak();
  }
  if ( (alignment & (alignment - 1)) != 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          158,
          0,
          "%s",
          "!(alignment & (alignment - 1))") )
  {
    __debugbreak();
  }
  if ( alignment > 4096
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          159,
          0,
          "%s",
          "alignment <= HUNK_MAX_ALIGNMENT") )
  {
    __debugbreak();
  }
  return Hunk_FirstFitAlloc((HunkUser *)_user[1].scheme, size, alignment);
}

void __cdecl Hunk_UserDebugFree(HunkUser *_user, unsigned int *ptr)
{
  if ( _user->scheme != HU_SCHEME_DEBUG
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          168,
          0,
          "%s",
          "_user->scheme==HU_SCHEME_DEBUG") )
  {
    __debugbreak();
  }
  if ( !Sys_IsMainThread()
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          169,
          0,
          "%s",
          "Sys_IsMainThread()") )
  {
    __debugbreak();
  }
  Hunk_FirstFitFree((HunkUser *)_user[1].scheme, ptr);
}

HunkUser *__cdecl Hunk_UserDefaultInit(
        unsigned __int8 *buffer,
        int size,
        HU_ALLOCATION_SCHEME scheme,
        unsigned int flags,
        void *scheme_specific_data,
        const char *name,
        int type)
{
  unsigned int v7; // ecx

  if ( scheme
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          179,
          0,
          "%s",
          "scheme==HU_SCHEME_DEFAULT") )
  {
    __debugbreak();
  }
  memset(buffer, 0, 0x2Cu);
  *((unsigned int *)buffer + 7) = &buffer[size];
  *((unsigned int *)buffer + 8) = buffer + 40;
  *((unsigned int *)buffer + 8) = (*((unsigned int *)buffer + 8) + 31) & 0xFFFFFFE0;
  if ( (*((unsigned int *)buffer + 8) & 0x1F) != 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          187,
          0,
          "%s\n\t(user->pos) = %i",
          "(!(user->pos & 31))",
          *((unsigned int *)buffer + 8)) )
  {
    __debugbreak();
  }
  if ( (flags & 2) != 0 )
    v7 = size + *((unsigned int *)buffer + 8);
  else
    v7 = (*((unsigned int *)buffer + 8) + 4095) & 0xFFFFF000;
  *((unsigned int *)buffer + 9) = v7;
  *((unsigned int *)buffer + 6) = size;
  *((unsigned int *)buffer + 4) = buffer;
  *(unsigned int *)buffer = scheme;
  *((unsigned int *)buffer + 1) = flags;
  *((unsigned int *)buffer + 2) = name;
  *((unsigned int *)buffer + 3) = type;
  if ( *((unsigned int *)buffer + 5)
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp", 200, 0, "%s", "!user->next") )
  {
    __debugbreak();
  }
  return (HunkUser *)buffer;
}

void __cdecl Hunk_UserDefaultReset(HunkUser *_user)
{
  char *pos; // [esp+0h] [ebp-8h]

  if ( _user->scheme
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          210,
          0,
          "%s",
          "user->hunkUser.scheme==HU_SCHEME_DEFAULT") )
  {
    __debugbreak();
  }
  if ( _user[1].flags )
  {
    Hunk_UserDefaultDestroy((HunkUser *)_user[1].flags);
    _user[1].scheme = (HU_ALLOCATION_SCHEME)_user;
    _user[1].flags = 0;
  }
  if ( (_user->flags & 2) != 0 )
  {
    _user[2].scheme = (HU_ALLOCATION_SCHEME)&_user[2].name;
    _user[2].flags = (unsigned int)&_user[1].name[_user[2].scheme];
  }
  else
  {
    pos = (char *)(((unsigned int)&_user[258].flags + 3) & 0xFFFFF000);
    if ( pos != (char *)((_user[2].scheme + 4095) & 0xFFFFF000) )
    {
      if ( _user[2].scheme - (int)pos <= 0
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
              229,
              0,
              "%s",
              "user->pos - pos > 0") )
      {
        __debugbreak();
      }
      Z_VirtualDecommit(pos, _user[2].scheme - (unsigned int)pos, _user->type);
    }
    _user[2].scheme = (HU_ALLOCATION_SCHEME)&_user[2].name;
    _user[2].flags = (_user[2].scheme + 4095) & 0xFFFFF000;
    memset((unsigned __int8 *)&_user[2].name, 0, 0xFD8u);
  }
}

void __cdecl Hunk_UserDefaultDestroy(HunkUser *_user)
{
  HunkUserDefault *current; // [esp+4h] [ebp-8h]
  HunkUserDefault *newCurrent; // [esp+8h] [ebp-4h]

  if ( _user->scheme
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          251,
          0,
          "%s",
          "user->hunkUser.scheme==HU_SCHEME_DEFAULT") )
  {
    __debugbreak();
  }
  if ( (_user->flags & 8) != 0 )
    track_userhunk_freerange(&_user[2].name, (unsigned int)_user[1].name);
  if ( (_user->flags & 2) == 0 )
  {
    for ( current = (HunkUserDefault *)_user[1].flags; current; current = newCurrent )
    {
      newCurrent = current->next;
      Z_VirtualFree(current, _user->type);
    }
    Z_VirtualFree(_user, _user->type);
  }
}

int __cdecl Hunk_UserDefaultAlloc(HunkUser *_user, unsigned int size, int alignment, const char *name)
{
  const char *v4; // eax
  signed int range; // [esp+4h] [ebp-1Ch]
  int pos; // [esp+Ch] [ebp-14h]
  int result; // [esp+10h] [ebp-10h]
  HunkUserDefault *current; // [esp+18h] [ebp-8h]
  HunkUserDefault *newCurrent; // [esp+1Ch] [ebp-4h]
  int alignmenta; // [esp+30h] [ebp+10h]

  if ( !_user
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp", 288, 0, "%s", "user") )
  {
    __debugbreak();
  }
  if ( _user->scheme
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          289,
          0,
          "%s",
          "user->hunkUser.scheme==HU_SCHEME_DEFAULT") )
  {
    __debugbreak();
  }
  if ( (const char *)size > _user[1].name - 40 )
  {
    v4 = va("size: %d, maxSize: %d", size, _user[1].name - 40);
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
            290,
            0,
            "%s\n\t%s",
            "static_cast< uint >( size ) <= user->maxSize - offsetof( HunkUserDefault, buf )",
            v4) )
      __debugbreak();
  }
  if ( (alignment & (alignment - 1)) != 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          291,
          0,
          "%s",
          "!(alignment & (alignment - 1))") )
  {
    __debugbreak();
  }
  if ( alignment > 4096
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          292,
          0,
          "%s",
          "alignment <= HUNK_MAX_ALIGNMENT") )
  {
    __debugbreak();
  }
  alignmenta = alignment - 1;
  for ( current = (HunkUserDefault *)_user[1].scheme; ; current = newCurrent )
  {
    pos = current->pos;
    result = pos % (alignmenta + 1) ? ~alignmenta & (alignmenta + pos) : current->pos;
    if ( (signed int)(size + result) <= current->end )
      break;
    if ( (_user->flags & 4) != 0 || (_user->flags & 2) != 0 )
      Com_Error(ERR_FATAL, "Hunk_UserAlloc: out of memory");
    newCurrent = (HunkUserDefault *)Hunk_UserCreate(
                                      (int)_user[1].name,
                                      _user->scheme,
                                      _user->flags,
                                      0,
                                      _user->name,
                                      _user->type);
    _user[1].scheme = (HU_ALLOCATION_SCHEME)newCurrent;
    current->next = newCurrent;
  }
  current->pos = size + result;
  if ( current->pos > current->locked )
  {
    range = ((current->pos + 4096) & 0xFFFFF000) - current->locked;
    if ( range <= 0
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp", 323, 0, "%s", "range > 0") )
    {
      __debugbreak();
    }
    if ( range % 4096
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
            324,
            0,
            "%s",
            "range % PAGE_SIZE == 0") )
    {
      __debugbreak();
    }
    Z_VirtualCommit((char *)current->locked, range, _user->type);
    current->locked += range;
  }
  if ( (_user->flags & 8) != 0 )
  {
    if ( name )
      track_userhunk_alloc(size, result, name, _user->type);
    else
      track_userhunk_alloc(size, result, _user->name, _user->type);
  }
  return result;
}

void __cdecl Hunk_UserDefaultFree(HunkUser *user, void *ptr)
{
  if ( user->scheme
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          341,
          0,
          "%s",
          "user->scheme==HU_SCHEME_DEFAULT") )
  {
    __debugbreak();
  }
  if ( (user->flags & 8) != 0 )
    track_userhunk_free(ptr);
}

void __cdecl Hunk_UserStartup()
{
  if ( g_DebugHunkUser
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          358,
          0,
          "%s",
          "!g_DebugHunkUser") )
  {
    __debugbreak();
  }
  g_DebugHunkUser = Hunk_UserCreate(
                      (int)&cls.unrankedServers[232].city[46],
                      HU_SCHEME_DEBUG,
                      0,
                      0,
                      "Hunk_InitDebugMemory",
                      0);
}

void __cdecl Hunk_UserShutdown()
{
  if ( !g_DebugHunkUser
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          366,
          0,
          "%s",
          "g_DebugHunkUser") )
  {
    __debugbreak();
  }
  Hunk_UserDestroy(g_DebugHunkUser);
  g_DebugHunkUser = 0;
}

HunkUser *__cdecl Hunk_UserCreateFromBuffer(
        void *buffer,
        int size,
        HU_ALLOCATION_SCHEME scheme,
        unsigned int flags,
        void *scheme_specific_data,
        const char *name,
        int type)
{
  if ( scheme == HU_SCHEME_DEBUG
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          374,
          0,
          "%s",
          "scheme!=HU_SCHEME_DEBUG") )
  {
    __debugbreak();
  }
  return g_HunkUserAllocationSchemeMap[scheme].Init(buffer, size, scheme, flags | 2, scheme_specific_data, name, type);
}

HunkUser *__cdecl Hunk_UserCreate(
        int size,
        HU_ALLOCATION_SCHEME scheme,
        unsigned int flags,
        void *scheme_specific_data,
        const char *name,
        int type)
{
  char *buffer; // [esp+0h] [ebp-4h]

  if ( (flags & 2) != 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          381,
          0,
          "%s",
          "(flags & HF_FROMBUFFER)==0") )
  {
    __debugbreak();
  }
  if ( (unsigned int)scheme >= HU_SCHEME_FIRSTFIT
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          382,
          0,
          "%s",
          "scheme == HU_SCHEME_DEFAULT || scheme == HU_SCHEME_DEBUG") )
  {
    __debugbreak();
  }
  if ( size % 4096
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          383,
          0,
          "%s\n\t(size) = %i",
          "(!(size % (4 * 1024)))",
          size) )
  {
    __debugbreak();
  }
  buffer = (char *)Z_VirtualReserve(size);
  if ( scheme == HU_SCHEME_DEBUG )
  {
    Z_VirtualCommit(buffer, size, type);
    return off_E0F544(buffer, size, HU_SCHEME_DEBUG, flags | 2, scheme_specific_data, name, type);
  }
  else
  {
    Z_VirtualCommit(buffer, 44, type);
    return g_HunkUserAllocationSchemeMap[scheme].Init(buffer, size, scheme, flags, scheme_specific_data, name, type);
  }
}

void *__cdecl Hunk_UserAlloc(HunkUser *user, int size, int alignment, const char *name)
{
  return (*(&off_E0F53C + 5 * user->scheme))(user, size, alignment, name);
}

void __cdecl Hunk_UserFree(HunkUser *user, void *ptr)
{
  (*(&off_E0F540 + 5 * user->scheme))(user, ptr);
}

void __cdecl Hunk_UserReset(HunkUser *user)
{
  off_E0F534[5 * user->scheme](user);
}

void __cdecl Hunk_UserDestroy(HunkUser *user)
{
  (*(&off_E0F538 + 5 * user->scheme))(user);
}

void __cdecl Hunk_UserSetPos(HunkUser *_user, const char **pos)
{
  if ( _user->scheme
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          446,
          0,
          "%s",
          "_user->scheme==HU_SCHEME_DEFAULT") )
  {
    __debugbreak();
  }
  if ( (_user->flags & 4) == 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          447,
          0,
          "%s",
          "(user->hunkUser.flags & HF_FIXEDSIZE)!=0") )
  {
    __debugbreak();
  }
  if ( pos < &_user[2].name
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          448,
          0,
          "%s",
          "pos >= user->buf") )
  {
    __debugbreak();
  }
  if ( (int)pos > _user[2].scheme
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\mem_userhunk.cpp",
          449,
          0,
          "%s",
          "(psize_int)pos <= user->pos") )
  {
    __debugbreak();
  }
  _user[2].scheme = (HU_ALLOCATION_SCHEME)pos;
}

char *__cdecl Hunk_CopyString(HunkUser *user, const char *in)
{
  char v3; // [esp+3h] [ebp-21h]
  char *v4; // [esp+8h] [ebp-1Ch]
  const char *v5; // [esp+Ch] [ebp-18h]
  char *out; // [esp+20h] [ebp-4h]

  out = (char *)Hunk_UserAlloc(user, strlen(in) + 1, 1, 0);
  v5 = in;
  v4 = out;
  do
  {
    v3 = *v5;
    *v4++ = *v5++;
  }
  while ( v3 );
  return out;
}

