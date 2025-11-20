#include "r_debug_alloc.h"

void __cdecl R_DebugAlloc(void **memPtr, int size, const char *name)
{
  if ( !memPtr
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_debug_alloc.cpp", 22, 0, "%s", "memPtr") )
  {
    __debugbreak();
  }
  if ( *memPtr
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_debug_alloc.cpp", 23, 0, "%s", "!*memPtr") )
  {
    __debugbreak();
  }
  if ( size <= 0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_debug_alloc.cpp", 24, 0, "%s", "size > 0") )
  {
    __debugbreak();
  }
  if ( (size & 3) != 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_debug_alloc.cpp",
          25,
          0,
          "%s",
          "(size & 0x3) == 0") )
  {
    __debugbreak();
  }
  *memPtr = Z_TryVirtualAlloc(size, name, 0);
  if ( !*memPtr )
    R_WarnOncePerFrame(R_WARN_DEBUG_ALLOC, name);
}

void __cdecl R_DebugFree(void **dataPtr)
{
  if ( *dataPtr )
  {
    Z_VirtualFree(*dataPtr, 0);
    *dataPtr = 0;
  }
}

