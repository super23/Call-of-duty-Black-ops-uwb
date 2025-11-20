#include "r_hw_nvidia.h"

bool __cdecl NV_UseShadowNullColorRenderTarget()
{
  return dx.nvInitialized;
}

