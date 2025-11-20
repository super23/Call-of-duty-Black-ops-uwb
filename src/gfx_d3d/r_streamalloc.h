#pragma once

void __cdecl R_StreamAlloc_InitTempImages();
GfxImage *__cdecl R_StreamAlloc_SetupTempImage(
        _D3DFORMAT format,
        bool linear,
        unsigned __int16 width,
        unsigned __int16 height);
void __cdecl R_StreamAlloc_ReleaseTempImage(GfxImage *image);
void __cdecl R_StreamAlloc_Lock();
void __cdecl R_StreamAlloc_Unlock();
char __cdecl R_StreamAlloc_CanAllocate(int size, float importance, GfxImage **unloadImage);
char __cdecl R_StreamAlloc_FreeImageByImportance(int size, float importance, GfxImage **unloadImage);
unsigned __int8 *__cdecl R_StreamAlloc_FreeImage(GfxImage *image, int imagePart, bool dirty, int *freedSize);
void __cdecl R_StreamAlloc_Flush();
