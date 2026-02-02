#pragma once
#include "r_dpvs.h"

struct DpvsStaticCellCmd // sizeof=0xC
{                                       // XREF: R_AddCellSurfacesAndCullGroupsInFrustumDelayed/r
    const DpvsPlane *planes;            // XREF: R_AddCellSurfacesAndCullGroupsInFrustumDelayed+F/w
    //$FE217278708CFE8B64B0580037FD6900 ___u1;
    // 00000000 union $FE217278708CFE8B64B0580037FD6900 // sizeof=0x4
    union 
    {                                       // XREF: R_AddCellSurfacesAndCullGroupsInFrustumDelayed+9/w
        const GfxCell *cell;
        const GfxCell *cell_local;
    };
                                        // XREF: R_AddCellSurfacesAndCullGroupsInFrustumDelayed+9/w
    unsigned __int8 planeCount;         // XREF: R_AddCellSurfacesAndCullGroupsInFrustumDelayed+15/w
    unsigned __int8 frustumPlaneCount;  // XREF: R_AddCellSurfacesAndCullGroupsInFrustumDelayed+1B/w
    unsigned __int16 viewIndex;         // XREF: R_AddCellSurfacesAndCullGroupsInFrustumDelayed+35/w
};

struct GfxSurface;
union GfxDrawSurf;

char __cdecl TestOccluders(const float (*bounds)[3], int numOccluders, float (*plane)[4]);
float (*__cdecl TestOccludersPartial(const float (*bounds)[3], int *numOccludersPtr, float (*plane)[4]))[4];
bool __cdecl R_CalcSurfaceNoDynamicShadow(unsigned int bits, GfxSurface *localSurfaces, GfxDrawSurf *localDrawSurfaces);
void __cdecl R_AddCellStaticSurfacesInFrustumCmd(DpvsStaticCellCmd *data);
int __cdecl R_AddCellStaticSurfacesInFrustum(DpvsStaticCellCmd *dpvsCell);
int __cdecl R_AddAabbTreeSurfacesInFrustum_r(
                const GfxAabbTree *tree,
                const DpvsClipPlanes *planes,
                int treeDepth,
                int numOccluders,
                float (*occluderPlanes)[4]);
double __cdecl R_DpvsPlaneMinSignedDistToBox(const DpvsPlane *plane, const float *minmax);
int __cdecl R_CullBoxLeaf(const float *minmax, DpvsClipPlanes *planes);
void __cdecl R_CopyClipPlanes(const DpvsPlane *in, DpvsPlane *out);
void __cdecl R_AddCellCullGroupsInFrustum(DpvsStaticCellCmd *dpvsCell);
void __cdecl R_AddSkySurfacesDpvs(const DpvsPlane *planes, int planeCount);
