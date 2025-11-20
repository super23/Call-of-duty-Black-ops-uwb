#pragma once

void __cdecl R_ReleaseAndSetNULL<IDirect3DIndexBuffer9>(
        IDirect3DSurface9 *var,
        const char *fn,
        const char *filename,
        int line);
void *__cdecl R_LockVertexBuffer(
        IDirect3DVertexBuffer9 *handle,
        unsigned int offset,
        unsigned int bytes,
        unsigned int lockFlags);
void __cdecl R_UnlockVertexBuffer(IDirect3DVertexBuffer9 *handle);
void *__cdecl R_LockIndexBuffer(
        IDirect3DIndexBuffer9 *handle,
        unsigned int offset,
        unsigned int bytes,
        unsigned int lockFlags);
void __cdecl R_UnlockIndexBuffer(IDirect3DIndexBuffer9 *handle);
void *__cdecl R_AllocDynamicVertexBuffer(IDirect3DVertexBuffer9 **vb, int sizeInBytes);
void *__cdecl R_AllocStaticVertexBuffer(IDirect3DVertexBuffer9 **vb, int sizeInBytes);
void __cdecl R_FinishStaticVertexBuffer(IDirect3DVertexBuffer9 *vb);
void __cdecl R_FreeStaticVertexBuffer(IDirect3DVertexBuffer9 *vb);
void *__cdecl R_AllocDynamicIndexBuffer(IDirect3DIndexBuffer9 **ib, unsigned int sizeInBytes);
void *__cdecl R_AllocStaticIndexBuffer(IDirect3DIndexBuffer9 **ib, int sizeInBytes);
void __cdecl R_FinishStaticIndexBuffer(IDirect3DIndexBuffer9 *ib);
void __cdecl R_FreeStaticIndexBuffer(IDirect3DIndexBuffer9 *ib);
void __cdecl Load_VertexBuffer(IDirect3DVertexBuffer9 **vb, unsigned __int8 *bufferData, int sizeInBytes);
void __cdecl R_InitDynamicVertexBufferState(GfxVertexBufferState *vb, int bytes);
void __cdecl R_InitDynamicIndices(GfxDynamicIndices *ib, int indexCount);
void __cdecl R_CreateDynamicBuffers();
void __cdecl R_InitDynamicIndexBufferState(GfxIndexBufferState *ib, int indexCount);
void __cdecl R_DestroyDynamicBuffers();
void __cdecl R_ShutdownDynamicIndices(GfxDynamicIndices *ib);
void __cdecl R_CreateParticleCloudBuffer();
void __cdecl R_DestroyParticleCloudBuffer();
