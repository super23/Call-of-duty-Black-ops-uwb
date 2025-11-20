#include "r_setstate_d3d.h"

void __cdecl R_FinishGpuFence()
{
  while ( LODWORD(dx.gpuSyncDelay)
       && (*(int (__stdcall **)(unsigned int, unsigned int, unsigned int, int))(*(unsigned int *)LODWORD(dx.gpuSyncDelay) + 28))(
            dx.gpuSyncDelay,
            0,
            0,
            1) == 1 )
    ;
}

