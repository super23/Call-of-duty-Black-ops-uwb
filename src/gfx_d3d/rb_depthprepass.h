#pragma once

void  R_DepthPrepass(
        int a1@<ebp>,
        unsigned __int8 renderTargetId,
        const GfxViewInfo *viewInfo,
        GfxCmdBuf *cmdBuf);
void __cdecl R_DepthPrepassCallback(const GfxViewInfo *userData, GfxCmdBufContext context);
