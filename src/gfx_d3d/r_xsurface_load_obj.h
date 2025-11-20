#pragma once

void __cdecl XModelReadSurface(XModel *model, unsigned __int8 **pos, void *(__cdecl *Alloc)(int), XSurface *surface);
void __cdecl ReadBlend(XSurface *surface, int *partBits, XBlendLoadInfo *blend, unsigned __int8 **pos);
void __cdecl XModelReadSurface_BuildCollisionTree(
        XSurface *surface,
        unsigned int vertListIndex,
        void *(__cdecl *Alloc)(int));
