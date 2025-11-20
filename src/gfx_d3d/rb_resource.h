#pragma once

void __cdecl RB_Resource_Lock();
void __cdecl RB_Resource_Unlock();
void __cdecl RB_Resource_CreateTexture(
        GfxImage *image,
        void *imageBuffer,
        int mipCount,
        int flags,
        _D3DFORMAT imageFormat);
r_resource_action *__cdecl RB_Resource_AllocEntry();
void __cdecl RB_Resource_Release(IUnknown *resource);
void __cdecl RB_Resource_ReloadTexture(GfxImage *image, void *data);
void __cdecl RB_Resource_Callback(void (__cdecl *callback)());
void __cdecl RB_Resource_CallbackParam(void (__cdecl *callback)(void *), void *data);
void __cdecl RB_Resource_CreateVertexDeclaration(
        _D3DVERTEXELEMENT9 *elements,
        IDirect3DVertexDeclaration9 **declaration);
void __cdecl RB_Resource_CreateVertexShader(unsigned int *function, IDirect3DVertexShader9 **shader);
void __cdecl RB_Resource_CreatePixelShader(unsigned int *function, IDirect3DPixelShader9 **shader);
void __cdecl RB_Resource_LoadVertexBuffer(IDirect3DVertexBuffer9 **vb, void *bufferData, int sizeInBytes);
void __cdecl RB_Resource_Update(int msec);
unsigned __int8 *RB_Resource_Update_Internal();
void __cdecl RB_Resource_Flush();
