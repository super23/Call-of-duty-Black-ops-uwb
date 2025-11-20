#pragma once

void  R_DrawSunShadowMap(
        int a1@<ebp>,
        const GfxViewInfo *viewInfo,
        unsigned int partitionIndex,
        GfxCmdBuf *cmdBuf);
void __cdecl R_DrawSunShadowMapCallback(const GfxSunShadowPartition **userData, GfxCmdBufContext context);
