#include "rb_compositing.h"

void __cdecl RB_DrawComposites()
{
  const void *cmds; // [esp+14h] [ebp-4h]

  //PIXBeginNamedEvent(-1, "RB_DrawComposites");
  cmds = backEndData->compositingCmds;
  if ( cmds )
  {
    R_InitLocalCmdBufState(&gfxCmdBufState);
    R_SetRenderTargetSize(&gfxCmdBufSourceState, 0x18u);
    R_SetRenderTarget(gfxCmdBufContext, 0x18u);
    R_Set2D(&gfxCmdBufSourceState);
    RB_ExecuteRenderCommandsLoop(cmds, 0);
    memcpy(gfxCmdBufState.refSamplerState, gfxCmdBufState.refSamplerState, sizeof(gfxCmdBufState));
    if ( GetCurrentThreadId() != g_DXDeviceThread )
      return;
    goto LABEL_5;
  }
  if ( GetCurrentThreadId() == g_DXDeviceThread )
LABEL_5:
    D3DPERF_EndEvent();
}

