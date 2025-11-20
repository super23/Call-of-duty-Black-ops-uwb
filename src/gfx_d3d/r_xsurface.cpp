#include "r_xsurface.h"

int __cdecl XSurfaceGetNumVerts(const XSurface *surface)
{
  if ( !surface
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_xsurface.cpp", 37, 0, "%s", "surface") )
  {
    __debugbreak();
  }
  return surface->vertCount;
}

int __cdecl XSurfaceGetNumTris(const XSurface *surface)
{
  if ( !surface
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_xsurface.cpp", 43, 0, "%s", "surface") )
  {
    __debugbreak();
  }
  return surface->triCount;
}

void __cdecl XSurfaceGetVerts(const XSurface *surf, float *pVert, float *pTexCoord, float *pNormal)
{
  PackedTexCoords *packedTexCoord; // [esp+0h] [ebp-1Ch]
  int render_count; // [esp+4h] [ebp-18h]
  const PackedUnitVec *packedNormal; // [esp+8h] [ebp-14h]
  GfxPackedVertex *verts0; // [esp+Ch] [ebp-10h]

  verts0 = surf->verts0;
  packedNormal = &verts0->normal;
  packedTexCoord = &verts0->texCoord;
  for ( render_count = surf->vertCount; render_count; --render_count )
  {
    if ( pNormal )
    {
      Vec3UnpackUnitVec((const PackedUnitVec)packedNormal->packed, pNormal);
      packedNormal += 8;
      pNormal += 3;
    }
    if ( pTexCoord )
    {
      Vec2UnpackTexCoords((PackedTexCoords)packedTexCoord->packed, pTexCoord);
      packedTexCoord += 8;
      pTexCoord += 2;
    }
    *pVert = verts0->xyz[0];
    pVert[1] = verts0->xyz[1];
    pVert[2] = verts0->xyz[2];
    ++verts0;
    pVert += 3;
  }
}

void __cdecl XSurfaceTransfer(
        const XVertexBuffer *surfVerts,
        GfxPackedVertex *verts0,
        GfxPackedVertex *verts1,
        int vertCount)
{
  float texCoordAv[2]; // [esp+0h] [ebp-10h] BYREF
  int vertIndex; // [esp+8h] [ebp-8h]
  const XVertexInfo_s *v; // [esp+Ch] [ebp-4h]

  if ( !vertCount
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_xsurface.cpp", 277, 0, "%s", "vertCount") )
  {
    __debugbreak();
  }
  v = &surfVerts->v;
  XSurfaceTransferGetTexCoordRange(&surfVerts->v, vertCount, texCoordAv);
  for ( vertIndex = 0; vertIndex < vertCount; ++vertIndex )
  {
    XSurfaceTransfer_Position_GfxPackedVertex_(&verts0[vertIndex], v);
    XSurfaceTransfer_BinormalSign_GfxPackedVertex_(&verts0[vertIndex], v);
    XSurfaceTransfer_NormalTangent_GfxPackedVertex_(&verts1[vertIndex], v);
    XSurfaceTransfer_Texcoord_GfxPackedVertex_(&verts1[vertIndex], v, texCoordAv);
    XSurfaceTransfer_Color_GfxPackedVertex_(&verts1[vertIndex], v);
    v = (const XVertexInfo_s *)((char *)v + 4 * v->numWeights + 64);
  }
}

void __cdecl XSurfaceTransferGetTexCoordRange(const XVertexInfo_s *v, int vertCount, float *texCoordAv)
{
  float v3; // [esp+8h] [ebp-1Ch]
  float v4; // [esp+Ch] [ebp-18h]
  int vertIndex; // [esp+1Ch] [ebp-8h]
  char texCoordUnitRange; // [esp+20h] [ebp-4h]
  char texCoordUnitRange_1; // [esp+21h] [ebp-3h]

  *texCoordAv = *(float *)&FLOAT_0_0;
  texCoordAv[1] = *(float *)&FLOAT_0_0;
  texCoordUnitRange = 1;
  texCoordUnitRange_1 = 1;
  for ( vertIndex = 0; vertIndex < vertCount; ++vertIndex )
  {
    *texCoordAv = *texCoordAv + v->texCoordX;
    texCoordAv[1] = texCoordAv[1] + v->texCoordY;
    if ( v->texCoordX < 0.0 || v->texCoordX > 1.0 )
      texCoordUnitRange = 0;
    if ( v->texCoordY < 0.0 || v->texCoordY > 1.0 )
      texCoordUnitRange_1 = 0;
  }
  *texCoordAv = (float)(1.0 / (float)vertCount) * *texCoordAv;
  texCoordAv[1] = (float)(1.0 / (float)vertCount) * texCoordAv[1];
  if ( texCoordUnitRange )
  {
    *texCoordAv = *(float *)&FLOAT_0_0;
  }
  else
  {
    v4 = floor((float)(*texCoordAv + 0.5));
    *texCoordAv = v4;
  }
  if ( texCoordUnitRange_1 )
  {
    texCoordAv[1] = *(float *)&FLOAT_0_0;
  }
  else
  {
    v3 = floor((float)(texCoordAv[1] + 0.5));
    texCoordAv[1] = v3;
  }
}

