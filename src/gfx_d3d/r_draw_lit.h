#pragma once

// local variable allocation has failed, the output may be wrong!
void  R_SetDrawLitConstants(
        int a1@<ebp>,
        GfxCmdBufSourceState *source,
        const GfxViewInfo *viewInfo,
        const GfxViewParms *viewParms);
void  R_DrawCloakHDR(int a1@<ebp>, const GfxViewInfo *viewInfo, GfxCmdBuf *cmdBuf, CloakPhaseID phase);
void __cdecl R_DrawCloakPrePassCallbackHDR(
        const GfxViewInfo *userData,
        GfxCmdBufContext context,
        GfxCmdBufContext prepassContext);
void __cdecl R_DrawCloakPostEmissiveCallbackHDR(
        const GfxViewInfo *userData,
        GfxCmdBufContext context,
        GfxCmdBufContext prepassContext);
void  R_DrawLit(
        int a1@<ebp>,
        const GfxViewInfo *viewInfo,
        GfxCmdBuf *cmdBuf,
        GfxCmdBuf *prepassCmdBuf,
        LitPhaseID phase);
void __cdecl R_DrawLitCallback(char *userData, GfxCmdBufContext context, GfxCmdBufContext prepassContext);
void __cdecl R_DrawLitPostResolveCallback(char *userData, GfxCmdBufContext context, GfxCmdBufContext prepassContext);
void  R_DrawDecal(int a1@<ebp>, const GfxViewInfo *viewInfo, GfxCmdBuf *cmdBuf, GfxCmdBuf *prepassCmdBuf);
void __cdecl R_DrawDecalCallback(char *userData, GfxCmdBufContext context, GfxCmdBufContext prepassContext);
