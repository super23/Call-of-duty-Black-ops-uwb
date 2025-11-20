#pragma once

void __cdecl R_SetStreamsForBspSurface(GfxCmdBufPrimState *state, const srfTriangles_t *tris);
void __cdecl R_DrawBspDrawSurfsLit(
        const unsigned int *primDrawSurfPos,
        GfxCmdBufContext context,
        GfxCmdBufContext prepassContext);
void __cdecl R_DrawTrianglesLit(
        GfxTrianglesDrawStream *drawStream,
        GfxCmdBufPrimState *primState,
        GfxCmdBufPrimState *prepassPrimState);
void __cdecl R_DrawBspTris(GfxCmdBufPrimState *state, const srfTriangles_t *tris, unsigned int triCount);
int __cdecl R_ReadBspDrawSurfs(
        const unsigned int **primDrawSurfPos,
        const unsigned __int16 **list,
        unsigned int *count);
void __cdecl R_DrawBspDrawSurfsLitPreTess(const unsigned int *primDrawSurfPos, GfxCmdBufContext context);
void __cdecl R_DrawPreTessTris(
        GfxCmdBufPrimState *state,
        const srfTriangles_t *tris,
        unsigned int baseIndex,
        unsigned int triCount);
int __cdecl R_ReadBspPreTessDrawSurfs(
        GfxReadCmdBuf *cmdBuf,
        const GfxBspPreTessDrawSurf **list,
        unsigned int *count,
        unsigned int *baseIndex);
void __cdecl R_DrawBspDrawSurfsPreTess(const unsigned int *primDrawSurfPos, GfxCmdBufContext context);
void __cdecl R_DrawBspDrawSurfs(const unsigned int *primDrawSurfPos, GfxCmdBufState *state);
void __cdecl R_DrawTriangles(GfxTrianglesDrawStream *drawStream, GfxCmdBufPrimState *state);
