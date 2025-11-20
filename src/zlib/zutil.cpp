#include "zutil.h"

unsigned __int8 *__cdecl zcalloc(unsigned __int8 *opaque, unsigned int items, unsigned int size)
{
  return (unsigned __int8 *)Z_Malloc(size * items, "zcalloc", 11);
}

void __cdecl zcfree(unsigned __int8 *opaque, unsigned __int8 *ptr)
{
  Z_Free((char *)ptr, 11);
}

