#pragma once

int __cdecl XSurfaceGetNumVerts(const XSurface *surface);
int __cdecl XSurfaceGetNumTris(const XSurface *surface);
void __cdecl XSurfaceGetVerts(const XSurface *surf, float *pVert, float *pTexCoord, float *pNormal);
void __cdecl XSurfaceTransfer(
        const XVertexBuffer *surfVerts,
        GfxPackedVertex *verts0,
        GfxPackedVertex *verts1,
        int vertCount);
void __cdecl XSurfaceTransferGetTexCoordRange(const XVertexInfo_s *v, int vertCount, float *texCoordAv);
