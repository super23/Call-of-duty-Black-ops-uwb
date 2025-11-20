#pragma once

void __cdecl R_DrawStaticModelSurfLit(
        const unsigned int *primDrawSurfPos,
        GfxCmdBufContext context,
        GfxCmdBufContext prepassContext,
        const GfxDrawSurfListInfo *info);
int __cdecl R_GetNextStaticModelSurf(GfxStaticModelDrawStream *drawStream, XSurface **outSurf);
void __cdecl R_DrawStaticModelSurf(
        const unsigned int *primDrawSurfPos,
        GfxCmdBufContext context,
        const GfxDrawSurfListInfo *info);
void __cdecl R_DrawStaticModelDrawSurfNonOptimized(GfxStaticModelDrawStream *drawStream, GfxCmdBufContext context);
void __cdecl R_SetStaticModelVertexBuffer(GfxCmdBufPrimState *primState, XSurface *xsurf);
void __cdecl R_SetStaticModelIndexBuffer(GfxCmdBufPrimState *primState, XSurface *xsurf);
void __cdecl R_SetStaticModelPrimArgs(const XSurface *xsurf, GfxDrawPrimArgs *args);
void __cdecl R_SetStaticModelVertexPrimArgs(const XSurface *xsurf, GfxDrawPrimArgs *args);
void __cdecl R_DrawStaticModelDrawSurfPlacement(
        const GfxStaticModelDrawInst *smodelDrawInst,
        GfxCmdBufSourceState *source);
void __cdecl R_DrawStaticModelDrawSurfLightingNonOptimized(
        GfxStaticModelDrawStream *drawStream,
        GfxCmdBufContext context);
void __cdecl R_DrawStaticModelSkinnedSurfLit(
        const unsigned int *primDrawSurfPos,
        GfxCmdBufContext context,
        const GfxDrawSurfListInfo *info);
void __cdecl R_DrawStaticModelsSkinnedDrawSurfLighting(GfxStaticModelDrawStream *drawStream, GfxCmdBufContext context);
void __cdecl R_DrawStaticModelSkinnedDrawSurfLighting(
        int smodelIndex,
        const GfxStaticModelDrawInst *smodelDrawInst,
        unsigned __int16 lightingHandle,
        GfxDrawPrimArgs *args,
        GfxCmdBufContext context,
        const DynSModelClientView *dynSModelView,
        const DynSModelGfxState *dynSModelState);
void __cdecl R_SetStaticModelSkinnedPrimArgs(GfxCmdBufPrimState *state, const XSurface *xsurf, GfxDrawPrimArgs *args);
void __cdecl R_DrawStaticModelSkinnedSurf(
        const unsigned int *primDrawSurfPos,
        GfxCmdBufContext context,
        const GfxDrawSurfListInfo *info);
void __cdecl R_DrawStaticModelsSkinnedDrawSurf(GfxStaticModelDrawStream *drawStream, GfxCmdBufContext context);
void __cdecl R_DrawStaticModelSkinnedDrawSurf(
        int smodelIndex,
        const GfxStaticModelDrawInst *smodelDrawInst,
        GfxDrawPrimArgs *args,
        GfxCmdBufContext context,
        const DynSModelClientView *dynSModelView,
        const DynSModelGfxState *dynSModelState);
void __cdecl R_DrawStaticModelCachedSurfLit(const unsigned int *primDrawSurfPos, GfxCmdBufContext context);
int __cdecl R_GetNextStaticModelCachedSurf(GfxStaticModelDrawStream *drawStream);
void __cdecl R_DrawStaticModelsCachedDrawSurfLighting(GfxStaticModelDrawStream *drawStream, GfxCmdBufContext context);
XSurface *__cdecl R_GetCurrentStaticModelCachedSurf(
        GfxStaticModelDrawStream *drawStream,
        unsigned int *reflectionProbeIndex);
void __cdecl R_SetStaticModelCachedPrimArgs(const XSurface *xsurf, GfxDrawPrimArgs *args);
void __cdecl R_SetStaticModelCachedBuffer(GfxCmdBufState *state, unsigned int cachedIndex);
void __cdecl R_DrawStaticModelCachedSurf(const unsigned int *primDrawSurfPos, GfxCmdBufContext context);
void __cdecl R_DrawStaticModelsCachedDrawSurf(GfxStaticModelDrawStream *drawStream, GfxCmdBufContext context);
void __cdecl R_DrawStaticModelPreTessSurfLit(const unsigned int *primDrawSurfPos, GfxCmdBufContext context);
int __cdecl R_ReadStaticModelPreTessDrawSurf(
        GfxReadCmdBuf *readCmdBuf,
        GfxStaticModelPreTessSurf *pretessSurf,
        unsigned int *firstIndex,
        unsigned int *count);
void __cdecl R_DrawStaticModelsPreTessDrawSurfLighting(
        GfxStaticModelPreTessSurf pretessSurf,
        unsigned int firstIndex,
        unsigned int count,
        GfxCmdBufContext context);
const GfxStaticModelDrawInst *__cdecl R_SetupCachedSModelSurface(
        GfxCmdBufState *state,
        unsigned int cachedIndex,
        unsigned int lod,
        unsigned int surfIndex,
        unsigned int count,
        GfxDrawPrimArgs *args,
        unsigned int *baseIndex);
void __cdecl R_DrawStaticModelPreTessSurf(const unsigned int *primDrawSurfPos, GfxCmdBufContext context);
void __cdecl R_DrawStaticModelsPreTessDrawSurf(
        GfxStaticModelPreTessSurf pretessSurf,
        unsigned int firstIndex,
        unsigned int count,
        GfxCmdBufContext context);
