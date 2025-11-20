#include "r_drawsurf.h"

void __cdecl R_SortDrawSurfs(GfxDrawSurf *drawSurfList, int surfCount)
{
  qsortArray_GfxSortDrawSurfsInterface_GfxDrawSurf_(drawSurfList, surfCount);
}

void __cdecl R_ReverseSortDrawSurfs(GfxDrawSurf *drawSurfList, int surfCount)
{
  qsortArray_GfxReverseSortDrawSurfsInterface_GfxDrawSurf_(drawSurfList, surfCount);
}

void __cdecl R_SortWorldSurfaces()
{
  GfxDrawSurf v0; // [esp+Ch] [ebp-28h]
  unsigned int surfIndex; // [esp+18h] [ebp-1Ch]
  unsigned int worldSurfCount; // [esp+28h] [ebp-Ch]
  GfxSurface *worldSurfArray; // [esp+30h] [ebp-4h]

  if ( !rgp.world
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp", 550, 0, "%s", "rgp.world") )
  {
    __debugbreak();
  }
  if ( rgp.world->models->startSurfIndex
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
          551,
          0,
          "%s\n\t(rgp.world->models[0].startSurfIndex) = %i",
          "(rgp.world->models[0].startSurfIndex == 0)",
          rgp.world->models->startSurfIndex) )
  {
    __debugbreak();
  }
  worldSurfArray = rgp.world->dpvs.surfaces;
  worldSurfCount = rgp.world->models->surfaceCount;
  if ( worldSurfCount )
    memset((unsigned __int8 *)rgp.world->dpvs.surfaceCastsSunShadow, 0, 4 * ((worldSurfCount - 1) >> 5) + 4);
  if ( worldSurfArray != rgp.world->dpvs.surfaces
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
          561,
          1,
          "%s",
          "worldSurfArray == rgp.world->dpvs.surfaces") )
  {
    __debugbreak();
  }
  if ( worldSurfCount != rgp.world->models->surfaceCount
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
          562,
          1,
          "%s",
          "worldSurfCount == rgp.world->models[0].surfaceCount") )
  {
    __debugbreak();
  }
  for ( surfIndex = 0; surfIndex < worldSurfCount; ++surfIndex )
  {
    v0.fields = R_GetWorldDrawSurf(&worldSurfArray[surfIndex]).fields;
    rgp.world->dpvs.surfaceMaterials[surfIndex] = v0;
    if ( (v0.packed & 0x100000) != 0 && (worldSurfArray[surfIndex].flags & 1) != 0 )
      rgp.world->dpvs.surfaceCastsSunShadow[surfIndex >> 5] |= 1 << (surfIndex & 0x1F);
  }
  R_SetPrimaryLightShadowSurfaces();
}

GfxWorld *R_SetPrimaryLightShadowSurfaces()
{
  GfxWorld *result; // eax
  unsigned int surfIndex; // [esp+8h] [ebp-14h]
  unsigned int primaryLightIndex; // [esp+18h] [ebp-4h]

  for ( primaryLightIndex = 0; primaryLightIndex < rgp.world->primaryLightCount; ++primaryLightIndex )
    rgp.world->shadowGeom[primaryLightIndex].surfaceCount = 0;
  for ( surfIndex = 0; ; ++surfIndex )
  {
    result = rgp.world;
    if ( surfIndex >= rgp.world->models->surfaceCount )
      break;
    if ( (*(unsigned int *)&rgp.world->dpvs.surfaceMaterials[surfIndex].fields & 0x100000) != 0 )
      R_ForEachPrimaryLightAffectingSurface(
        rgp.world,
        &rgp.world->dpvs.surfaces[surfIndex],
        surfIndex,
        R_AddShadowSurfaceToPrimaryLight);
  }
  return result;
}

GfxDrawSurf __cdecl R_GetWorldDrawSurf(GfxSurface *worldSurf)
{
  Material *material; // ecx
  GfxDrawSurf drawSurf; // [esp+14h] [ebp-8h]

  material = worldSurf->material;
  *(unsigned int *)&drawSurf.fields = material->info.drawSurf.fields;
  HIDWORD(drawSurf.packed) = (worldSurf->primaryLightIndex << 11) | HIDWORD(material->info.drawSurf.packed) & 0xFFF807FF;
  if ( (unsigned __int8)(drawSurf.packed >> 43) != worldSurf->primaryLightIndex
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
          535,
          1,
          "drawSurf.fields.primaryLightIndex == worldSurf->primaryLightIndex\n\t%i, %i",
          (unsigned __int8)(drawSurf.packed >> 43),
          worldSurf->primaryLightIndex) )
  {
    __debugbreak();
  }
  return drawSurf;
}

void __cdecl R_AddCodeMeshDrawSurf(
        Material *material,
        r_double_index_t *indices,
        unsigned int indexCount,
        unsigned int argOffset,
        unsigned int argCount,
        const char *fxName,
        unsigned int region)
{
  int packed_high; // ecx
  unsigned int v8; // edx
  int MaterialSortKey; // [esp+20h] [ebp-24h]
  FxCodeMeshData *localCodeMesh; // [esp+30h] [ebp-14h]
  GfxDrawSurf *drawSurf; // [esp+34h] [ebp-10h]
  unsigned int codeMeshIndex; // [esp+40h] [ebp-4h]
  unsigned int regiona; // [esp+64h] [ebp+20h]

  if ( !indexCount
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp", 635, 0, "%s", "indexCount") )
  {
    __debugbreak();
  }
  if ( !g_processCodeMesh
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
          636,
          0,
          "%s",
          "g_processCodeMesh") )
  {
    __debugbreak();
  }
  if ( rgp.sortedMaterials[(material->info.drawSurf.packed >> 31) & 0xFFF] != material
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
          639,
          0,
          "%s",
          "rgp.sortedMaterials[material->info.drawSurf.fields.materialSortedIndex] == material") )
  {
    __debugbreak();
  }
  if ( Material_GetTechnique(material, gfxDrawMethod.emissiveTechType) && (material->info.gameFlags & 2) == 0 )
  {
    codeMeshIndex = _InterlockedExchangeAdd(&frontEndDataOut->codeMeshCount, 1u);
    if ( codeMeshIndex < 0x800 )
    {
      localCodeMesh = &frontEndDataOut->codeMeshes[codeMeshIndex];
      localCodeMesh->triCount = indexCount / 3;
      localCodeMesh->indices = (unsigned __int16 *)indices;
      if ( !argCount
        && argOffset
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
              680,
              0,
              "%s",
              "argCount != 0 || argOffset == 0") )
      {
        __debugbreak();
      }
      if ( argOffset >= 0x100
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
              681,
              0,
              "%s\n\t(argOffset) = %i",
              "(argOffset >= 0 && argOffset < 256)",
              argOffset) )
      {
        __debugbreak();
      }
      localCodeMesh->argCount = argCount;
      if ( localCodeMesh->argCount != argCount
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
              683,
              0,
              "%s\n\t(argCount) = %i",
              "(localCodeMesh->argCount == argCount)",
              argCount) )
      {
        __debugbreak();
      }
      localCodeMesh->argOffset = argOffset;
      if ( localCodeMesh->argOffset != argOffset
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
              685,
              0,
              "%s\n\t(argOffset) = %i",
              "(localCodeMesh->argOffset == argOffset)",
              argOffset) )
      {
        __debugbreak();
      }
      MaterialSortKey = R_GetMaterialSortKey(material);
      regiona = (MaterialSortKey != 24 ? 0 : 2) + region + (MaterialSortKey == 48);
      drawSurf = R_AllocFxDrawSurf(regiona);
      if ( drawSurf )
      {
        drawSurf->fields = R_GetMaterialInfoPacked(material).fields;
        packed_high = HIDWORD(drawSurf->packed);
        *(unsigned int *)&drawSurf->fields = (unsigned __int16)codeMeshIndex | *(unsigned int *)&drawSurf->fields & 0xFFFF0000;
        HIDWORD(drawSurf->packed) = packed_high;
        v8 = (unsigned int)&loc_580000 | HIDWORD(drawSurf->packed) & 0xFF87FFFF;
        *(unsigned int *)&drawSurf->fields = drawSurf->fields;
        HIDWORD(drawSurf->packed) = v8;
        *(unsigned int *)&drawSurf->fields = drawSurf->fields;
        HIDWORD(drawSurf->packed) = HIDWORD(drawSurf->packed);
        if ( ((drawSurf->packed >> 55) & 3) != 2
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
                713,
                1,
                "%s",
                "localDrawSurf->fields.prepass == MTL_PREPASS_NONE") )
        {
          __debugbreak();
        }
        if ( regiona != 13
          && regiona != 16
          && drawSurf != scene.drawSurfs[regiona]
          && ((drawSurf->packed >> 58) & 0x3F) < ((drawSurf[-1].packed >> 58) & 0x3F)
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
                715,
                1,
                "%s\n\t(region) = %i",
                "((region == DRAW_SURF_FX_CAMERA_EMISSIVE) || (region == DRAW_SURF_FX_REFLECTED_EMISSIVE) || (drawSurf =="
                " scene.drawSurfs[region]) || (drawSurf->fields.primarySortKey >= (drawSurf - 1)->fields.primarySortKey))",
                regiona) )
        {
          __debugbreak();
        }
      }
    }
    else
    {
      R_WarnOncePerFrame(R_WARN_GFX_CODE_MESH_LIMIT);
    }
  }
  else
  {
    if ( (!r_fullbright || !r_lightConflicts)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
            645,
            0,
            "%s",
            "r_fullbright && r_lightConflicts") )
    {
      __debugbreak();
    }
    if ( !r_fullbright->current.enabled && !r_lightConflicts->current.enabled )
      R_WarnOncePerFrame(R_WARN_NONEMISSIVE_FX_MATERIAL, material->info.name, fxName);
  }
}

GfxDrawSurf *__cdecl R_AllocFxDrawSurf(unsigned int region)
{
  signed int maxCount; // [esp+4h] [ebp-8h]
  int drawSurfCount; // [esp+8h] [ebp-4h]

  drawSurfCount = _InterlockedExchangeAdd((volatile signed __int32 *)(4 * region + 177797148), 1u);
  maxCount = scene.maxDrawSurfCount[region];
  if ( drawSurfCount < maxCount )
    return &scene.drawSurfs[region][drawSurfCount];
  _InterlockedExchangeAdd((volatile signed __int32 *)(4 * region + 177797148), 0xFFFFFFFF);
  R_WarnOncePerFrame(R_WARN_MAX_FX_DRAWSURFS, maxCount, region);
  return 0;
}

unsigned __int8 __cdecl R_GetMaterialSortKey(const Material *material)
{
  return material->info.sortKey;
}

GfxDrawSurf __cdecl R_GetMaterialInfoPacked(const Material *material)
{
  return material->info.drawSurf;
}

void __cdecl R_AddRopeCodeMeshDrawSurf(
        const Material *material,
        r_double_index_t *indices,
        unsigned int indexCount,
        unsigned int argOffset,
        unsigned int argCount,
        unsigned __int8 primaryLightIndex,
        unsigned int drawsurfRegion)
{
  const char *v7; // eax
  int packed_high; // ecx
  unsigned int v9; // edx
  unsigned int v10; // eax
  unsigned int v11; // ecx
  FxCodeMeshData *localCodeMesh; // [esp+8h] [ebp-14h]
  GfxDrawSurf *drawSurf; // [esp+Ch] [ebp-10h]
  unsigned int codeMeshIndex; // [esp+18h] [ebp-4h]

  if ( !indexCount
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp", 732, 0, "%s", "indexCount") )
  {
    __debugbreak();
  }
  if ( !g_processCodeMesh
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
          733,
          0,
          "%s",
          "g_processCodeMesh") )
  {
    __debugbreak();
  }
  if ( rgp.sortedMaterials[(material->info.drawSurf.packed >> 31) & 0xFFF] != material )
  {
    v7 = va(
           "index = 0x%08x, sortedMaterials = 0x%08x, material = 0x%08x\n",
           (unsigned int)(material->info.drawSurf.packed >> 31) & 0xFFF,
           0,
           rgp.sortedMaterials[(material->info.drawSurf.packed >> 31) & 0xFFF]);
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
            740,
            0,
            "%s\n\t%s",
            "rgp.sortedMaterials[material->info.drawSurf.fields.materialSortedIndex] == material",
            v7) )
      __debugbreak();
  }
  codeMeshIndex = _InterlockedExchangeAdd(&frontEndDataOut->codeMeshCount, 1u);
  if ( codeMeshIndex < 0x800 )
  {
    localCodeMesh = &frontEndDataOut->codeMeshes[codeMeshIndex];
    localCodeMesh->triCount = indexCount / 3;
    localCodeMesh->indices = (unsigned __int16 *)indices;
    if ( !argCount
      && argOffset
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
            771,
            0,
            "%s",
            "argCount != 0 || argOffset == 0") )
    {
      __debugbreak();
    }
    if ( argOffset >= 0x100
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
            772,
            0,
            "%s\n\t(argOffset) = %i",
            "(argOffset >= 0 && argOffset < 256)",
            argOffset) )
    {
      __debugbreak();
    }
    localCodeMesh->argCount = argCount;
    if ( localCodeMesh->argCount != argCount
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
            774,
            0,
            "%s\n\t(argCount) = %i",
            "(localCodeMesh->argCount == argCount)",
            argCount) )
    {
      __debugbreak();
    }
    localCodeMesh->argOffset = argOffset;
    if ( localCodeMesh->argOffset != argOffset
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
            776,
            0,
            "%s\n\t(argOffset) = %i",
            "(localCodeMesh->argOffset == argOffset)",
            argOffset) )
    {
      __debugbreak();
    }
    drawSurf = R_AllocFxDrawSurf(drawsurfRegion);
    if ( drawSurf )
    {
      drawSurf->fields = R_GetMaterialInfoPacked(material).fields;
      packed_high = HIDWORD(drawSurf->packed);
      *(unsigned int *)&drawSurf->fields = (unsigned __int16)codeMeshIndex | *(unsigned int *)&drawSurf->fields & 0xFFFF0000;
      HIDWORD(drawSurf->packed) = packed_high;
      v9 = (unsigned int)&off_700000 | HIDWORD(drawSurf->packed) & 0xFF87FFFF;
      *(unsigned int *)&drawSurf->fields = drawSurf->fields;
      HIDWORD(drawSurf->packed) = v9;
      v10 = (unsigned int)&cls.rankedServers[711].game[35] | HIDWORD(drawSurf->packed) & 0xFE7FFFFF;
      *(unsigned int *)&drawSurf->fields = drawSurf->fields;
      HIDWORD(drawSurf->packed) = v10;
      v11 = (primaryLightIndex << 11) | HIDWORD(drawSurf->packed) & 0xFFF807FF;
      *(unsigned int *)&drawSurf->fields = drawSurf->fields;
      HIDWORD(drawSurf->packed) = v11;
      *(unsigned int *)&drawSurf->fields = drawSurf->fields;
      HIDWORD(drawSurf->packed) = HIDWORD(drawSurf->packed);
    }
  }
  else
  {
    R_WarnOncePerFrame(R_WARN_GFX_CODE_MESH_LIMIT);
  }
}

void __cdecl R_AddGlassDrawSurf(
        Material *material,
        unsigned __int16 *indices,
        unsigned int indexCount,
        unsigned int lightHandle,
        unsigned __int8 primaryLightIndex,
        char reflectionProbeIndex,
        unsigned int drawsurfRegion)
{
  const char *v7; // eax
  int packed_high; // ecx
  unsigned int v9; // edx
  unsigned int v10; // eax
  unsigned int v11; // ecx
  unsigned __int64 v12; // rax
  int v13; // ecx
  FxCodeMeshData *localCodeMesh; // [esp+8h] [ebp-14h]
  GfxDrawSurf *drawSurf; // [esp+Ch] [ebp-10h]
  unsigned int codeMeshIndex; // [esp+18h] [ebp-4h]

  if ( !indexCount
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp", 820, 0, "%s", "indexCount") )
  {
    __debugbreak();
  }
  if ( rgp.sortedMaterials[(material->info.drawSurf.packed >> 31) & 0xFFF] != material )
  {
    v7 = va(
           "index = 0x%08x, sortedMaterials = 0x%08x, material = 0x%08x\n",
           (unsigned int)(material->info.drawSurf.packed >> 31) & 0xFFF,
           0,
           rgp.sortedMaterials[(material->info.drawSurf.packed >> 31) & 0xFFF]);
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
            827,
            0,
            "%s\n\t%s",
            "rgp.sortedMaterials[material->info.drawSurf.fields.materialSortedIndex] == material",
            v7) )
      __debugbreak();
  }
  codeMeshIndex = _InterlockedExchangeAdd(&frontEndDataOut->codeMeshCount, 1u);
  if ( codeMeshIndex < 0x800 )
  {
    localCodeMesh = &frontEndDataOut->codeMeshes[codeMeshIndex];
    localCodeMesh->triCount = indexCount / 3;
    localCodeMesh->indices = indices;
    localCodeMesh->argCount = 0;
    localCodeMesh->argOffset = 0;
    localCodeMesh->lightHandle = lightHandle;
    drawSurf = R_AllocFxDrawSurf(drawsurfRegion);
    if ( drawSurf )
    {
      drawSurf->fields = R_GetMaterialInfoPacked(material).fields;
      packed_high = HIDWORD(drawSurf->packed);
      *(unsigned int *)&drawSurf->fields = (unsigned __int16)codeMeshIndex | *(unsigned int *)&drawSurf->fields & 0xFFFF0000;
      HIDWORD(drawSurf->packed) = packed_high;
      v9 = (unsigned int)&loc_780000 | HIDWORD(drawSurf->packed);
      *(unsigned int *)&drawSurf->fields = drawSurf->fields;
      HIDWORD(drawSurf->packed) = v9;
      v10 = (unsigned int)&cls.rankedServers[711].game[35] | HIDWORD(drawSurf->packed) & 0xFE7FFFFF;
      *(unsigned int *)&drawSurf->fields = drawSurf->fields;
      HIDWORD(drawSurf->packed) = v10;
      v11 = (primaryLightIndex << 11) | HIDWORD(drawSurf->packed) & 0xFFF807FF;
      *(unsigned int *)&drawSurf->fields = drawSurf->fields;
      HIDWORD(drawSurf->packed) = v11;
      v12 = (unsigned __int64)(reflectionProbeIndex & 7) << 25;
      v13 = HIDWORD(v12) | HIDWORD(drawSurf->packed);
      *(unsigned int *)&drawSurf->fields = v12 | *(unsigned int *)&drawSurf->fields & 0xF1FFFFFF;
      HIDWORD(drawSurf->packed) = v13;
      *(unsigned int *)&drawSurf->fields = drawSurf->fields;
      HIDWORD(drawSurf->packed) = HIDWORD(drawSurf->packed);
    }
  }
  else
  {
    R_WarnOncePerFrame(R_WARN_GFX_CODE_MESH_LIMIT);
  }
}

void __cdecl R_AddMarkMeshDrawSurf(
        Material *material,
        const GfxMarkContext *context,
        unsigned __int16 *indices,
        unsigned int indexCount,
        const float *hitNormal,
        char visLightsMask)
{
  int packed_high; // ecx
  unsigned __int64 v7; // rax
  int v8; // ecx
  unsigned __int64 v9; // rax
  int v10; // ecx
  unsigned int v11; // ecx
  unsigned __int64 v12; // rax
  int v13; // ecx
  unsigned __int64 v14; // rax
  int v15; // ecx
  bool isForSeeThruBulletHoleDecal; // [esp+47h] [ebp-15h]
  GfxDrawSurf *drawSurf; // [esp+48h] [ebp-14h]
  int region; // [esp+50h] [ebp-Ch]
  unsigned int markMeshIndex; // [esp+54h] [ebp-8h]
  FxMarkMeshData *markMesh; // [esp+58h] [ebp-4h]

  if ( !g_processMarkMesh
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
          902,
          0,
          "%s",
          "g_processMarkMesh") )
  {
    __debugbreak();
  }
  if ( rgp.sortedMaterials[(material->info.drawSurf.packed >> 31) & 0xFFF] != material
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
          903,
          0,
          "%s",
          "rgp.sortedMaterials[material->info.drawSurf.fields.materialSortedIndex] == material") )
  {
    __debugbreak();
  }
  if ( Material_GetTechnique(material, gfxDrawMethod.litTechType[12][0][0][0])
    && ((isForSeeThruBulletHoleDecal = Material_IsForSeeThruBulletHoleDecal(material)) || material->info.sortKey >= 0x18u) )
  {
    markMeshIndex = _InterlockedExchangeAdd(&frontEndDataOut->markMeshCount, 1u);
    if ( markMeshIndex < 0x600 )
    {
      markMesh = &frontEndDataOut->markMeshes[markMeshIndex];
      markMesh->triCount = indexCount / 3;
      markMesh->indices = indices;
      markMesh->modelIndex = context->modelIndex;
      markMesh->modelTypeAndSurf = context->modelTypeAndSurf;
      markMesh->normal[0] = *hitNormal;
      markMesh->normal[1] = hitNormal[1];
      markMesh->normal[2] = hitNormal[2];
      region = (material->info.sortKey == 48) + (material->info.sortKey != 24 ? 0 : 2) + 6;
      drawSurf = R_AllocFxDrawSurf(region);
      if ( drawSurf )
      {
        drawSurf->fields = (GfxDrawSurfFields)material->info.drawSurf;
        packed_high = HIDWORD(drawSurf->packed);
        *(unsigned int *)&drawSurf->fields = (unsigned __int16)markMeshIndex | *(unsigned int *)&drawSurf->fields & 0xFFFF0000;
        HIDWORD(drawSurf->packed) = packed_high;
        v7 = (unsigned __int64)(context->lmapIndex & 0x1F) << 20;
        v8 = HIDWORD(v7) | HIDWORD(drawSurf->packed);
        *(unsigned int *)&drawSurf->fields = v7 | *(unsigned int *)&drawSurf->fields & 0xFE0FFFFF;
        HIDWORD(drawSurf->packed) = v8;
        HIDWORD(v7) = (unsigned int)&off_600000 | HIDWORD(drawSurf->packed) & 0xFF87FFFF;
        *(unsigned int *)&drawSurf->fields = drawSurf->fields;
        HIDWORD(drawSurf->packed) = HIDWORD(v7);
        LODWORD(v7) = (unsigned int)&cls.rankedServers[711].game[35] | HIDWORD(drawSurf->packed) & 0xFE7FFFFF;
        *(unsigned int *)&drawSurf->fields = drawSurf->fields;
        HIDWORD(drawSurf->packed) = v7;
        v9 = (unsigned __int64)(context->reflectionProbeIndex & 7) << 25;
        v10 = HIDWORD(v9) | HIDWORD(drawSurf->packed);
        *(unsigned int *)&drawSurf->fields = v9 | *(unsigned int *)&drawSurf->fields & 0xF1FFFFFF;
        HIDWORD(drawSurf->packed) = v10;
        v11 = (context->primaryLightIndex << 11) | HIDWORD(drawSurf->packed) & 0xFFF807FF;
        *(unsigned int *)&drawSurf->fields = drawSurf->fields;
        HIDWORD(drawSurf->packed) = v11;
        v12 = (unsigned __int64)(visLightsMask & 1) << 29;
        v13 = HIDWORD(v12) | HIDWORD(drawSurf->packed);
        *(unsigned int *)&drawSurf->fields = v12 | *(unsigned int *)&drawSurf->fields & 0xDFFFFFFF;
        HIDWORD(drawSurf->packed) = v13;
        v14 = (unsigned __int64)((visLightsMask & 2) != 0) << 30;
        v15 = HIDWORD(v14) | HIDWORD(drawSurf->packed);
        *(unsigned int *)&drawSurf->fields = v14 | *(unsigned int *)&drawSurf->fields & 0xBFFFFFFF;
        HIDWORD(drawSurf->packed) = v15;
        if ( !isForSeeThruBulletHoleDecal
          && ((drawSurf->packed >> 55) & 3) != 2
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
                968,
                1,
                "%s",
                "drawSurf->fields.prepass == MTL_PREPASS_NONE") )
        {
          __debugbreak();
        }
        if ( ((drawSurf->packed >> 25) & 7) != context->reflectionProbeIndex
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
                970,
                1,
                "drawSurf->fields.reflectionProbeIndex == context->reflectionProbeIndex\n\t%i, %i",
                (unsigned int)(drawSurf->packed >> 25) & 7,
                context->reflectionProbeIndex) )
        {
          __debugbreak();
        }
        if ( region != 6
          && drawSurf != scene.drawSurfs[region]
          && ((drawSurf->packed >> 58) & 0x3F) < ((drawSurf[-1].packed >> 58) & 0x3F)
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
                971,
                1,
                "%s\n\t(region) = %i",
                "((region == DRAW_SURF_FX_CAMERA_LIT) || (drawSurf == scene.drawSurfs[region]) || (drawSurf->fields.prima"
                "rySortKey >= (drawSurf - 1)->fields.primarySortKey))",
                region) )
        {
          __debugbreak();
        }
      }
    }
    else
    {
      R_WarnOncePerFrame(R_WARN_GFX_MARK_MESH_LIMIT);
    }
  }
  else
  {
    R_WarnOncePerFrame(R_WARN_NONLIGHTMAP_MARK_MATERIAL, material->info.name);
  }
}

char __cdecl R_AddParticleCloudDrawSurf(volatile unsigned int cloudIndex, Material *material)
{
  int packed_high; // ecx
  unsigned int v4; // edx
  int MaterialSortKey; // [esp+20h] [ebp-18h]
  GfxDrawSurf *drawSurf; // [esp+2Ch] [ebp-Ch]
  int region; // [esp+30h] [ebp-8h]

  if ( cloudIndex >= frontEndDataOut->cloudCount
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
          1010,
          0,
          "cloudIndex doesn't index frontEndDataOut->cloudCount\n\t%i not in [0, %i)",
          cloudIndex,
          frontEndDataOut->cloudCount) )
  {
    __debugbreak();
  }
  if ( rgp.sortedMaterials[(material->info.drawSurf.packed >> 31) & 0xFFF] != material
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
          1011,
          0,
          "%s",
          "rgp.sortedMaterials[material->info.drawSurf.fields.materialSortedIndex] == material") )
  {
    __debugbreak();
  }
  if ( Material_GetTechnique(material, gfxDrawMethod.emissiveTechType) )
  {
    MaterialSortKey = R_GetMaterialSortKey(material);
    region = (MaterialSortKey == 48) + (MaterialSortKey != 24 ? 0 : 2) + 13;
    drawSurf = R_AllocFxDrawSurf(region);
    if ( drawSurf )
    {
      drawSurf->fields = R_GetMaterialInfoPacked(material).fields;
      packed_high = HIDWORD(drawSurf->packed);
      *(unsigned int *)&drawSurf->fields = (unsigned __int16)cloudIndex | *(unsigned int *)&drawSurf->fields & 0xFFFF0000;
      HIDWORD(drawSurf->packed) = packed_high;
      v4 = (unsigned int)&loc_680000 | HIDWORD(drawSurf->packed) & 0xFF87FFFF;
      *(unsigned int *)&drawSurf->fields = drawSurf->fields;
      HIDWORD(drawSurf->packed) = v4;
      *(unsigned int *)&drawSurf->fields = drawSurf->fields;
      HIDWORD(drawSurf->packed) = HIDWORD(drawSurf->packed);
      if ( ((drawSurf->packed >> 55) & 3) != 2
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
              1033,
              1,
              "%s",
              "localDrawSurf->fields.prepass == MTL_PREPASS_NONE") )
      {
        __debugbreak();
      }
      if ( (MaterialSortKey == 48) + (MaterialSortKey != 24 ? 0 : 2)
        && drawSurf != scene.drawSurfs[region]
        && ((drawSurf->packed >> 58) & 0x3F) < ((drawSurf[-1].packed >> 58) & 0x3F)
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
              1035,
              1,
              "%s\n\t(region) = %i",
              "((region == DRAW_SURF_FX_CAMERA_EMISSIVE) || (drawSurf == scene.drawSurfs[region]) || (drawSurf->fields.pr"
              "imarySortKey >= (drawSurf - 1)->fields.primarySortKey))",
              region) )
      {
        __debugbreak();
      }
      return 1;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    R_WarnOncePerFrame(R_WARN_NONEMISSIVE_FX_MATERIAL, material->info.name, "UNKNOWN-FX");
    return 0;
  }
}

void __cdecl R_BeginCodeMeshVerts()
{
  if ( g_processCodeMesh
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
          1069,
          0,
          "%s",
          "!g_processCodeMesh") )
  {
    __debugbreak();
  }
  g_processCodeMesh = 1;
}

void __cdecl R_EndCodeMeshVerts()
{
  g_processCodeMesh = 0;
}

void __cdecl R_BeginMarkMeshVerts()
{
  if ( g_processMarkMesh
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
          1120,
          0,
          "%s",
          "!g_processMarkMesh") )
  {
    __debugbreak();
  }
  g_processMarkMesh = 1;
}

void __cdecl R_EndMarkMeshVerts()
{
  g_processMarkMesh = 0;
}

char __cdecl R_ReserveCodeMeshIndices(int indexCount, r_double_index_t **indicesOut)
{
  if ( !g_processCodeMesh
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
          1160,
          0,
          "%s",
          "g_processCodeMesh") )
  {
    __debugbreak();
  }
  if ( R_ReserveMeshIndices(frontEndDataOut->codeMeshPtr, indexCount, indicesOut) )
    return 1;
  R_WarnOncePerFrame(R_WARN_MAX_CODE_INDS);
  return 0;
}

char __cdecl R_ReserveCodeMeshVerts(int vertCount, unsigned __int16 *baseVertex)
{
  if ( !g_processCodeMesh
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
          1211,
          0,
          "%s",
          "g_processCodeMesh") )
  {
    __debugbreak();
  }
  if ( R_ReserveMeshVerts(frontEndDataOut->codeMeshPtr, vertCount, baseVertex) )
    return 1;
  R_WarnOncePerFrame(R_WARN_MAX_CODE_VERTS);
  return 0;
}

char __cdecl R_ReserveCodeMeshArgs(int argCount, unsigned int *argOffsetOut)
{
  volatile int oldArgCount; // [esp+8h] [ebp-4h]

  if ( !g_processCodeMesh
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
          1235,
          0,
          "%s",
          "g_processCodeMesh") )
  {
    __debugbreak();
  }
  if ( argCount < 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
          1236,
          0,
          "%s\n\t(argCount) = %i",
          "(argCount >= 0)",
          argCount) )
  {
    __debugbreak();
  }
  if ( !argOffsetOut
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp", 1237, 0, "%s", "argOffsetOut") )
  {
    __debugbreak();
  }
  do
  {
    oldArgCount = frontEndDataOut->codeMeshArgsCount;
    if ( (unsigned int)(argCount + oldArgCount) >= 0x100 )
    {
      R_WarnOncePerFrame(R_WARN_MAX_CODE_ARGS);
      return 0;
    }
  }
  while ( _InterlockedCompareExchange(&frontEndDataOut->codeMeshArgsCount, argCount + oldArgCount, oldArgCount) != oldArgCount );
  *argOffsetOut = oldArgCount;
  return 1;
}

char __cdecl R_ReserveMarkMeshIndices(int indexCount, r_double_index_t **indicesOut)
{
  if ( !g_processMarkMesh
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
          1258,
          0,
          "%s",
          "g_processMarkMesh") )
  {
    __debugbreak();
  }
  if ( R_ReserveMeshIndices(&frontEndDataOut->markMesh, indexCount, indicesOut) )
    return 1;
  R_WarnOncePerFrame(R_WARN_MAX_MARK_INDS);
  return 0;
}

char __cdecl R_ReserveMarkMeshVerts(int vertCount, unsigned __int16 *baseVertex)
{
  if ( !g_processMarkMesh
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
          1269,
          0,
          "%s",
          "g_processMarkMesh") )
  {
    __debugbreak();
  }
  if ( R_ReserveMeshVerts(&frontEndDataOut->markMesh, vertCount, baseVertex) )
    return 1;
  R_WarnOncePerFrame(R_WARN_MAX_MARK_VERTS);
  return 0;
}

float (*__cdecl R_GetCodeMeshArgs(unsigned int argOffset))[4]
{
  if ( !g_processCodeMesh
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
          1281,
          0,
          "%s",
          "g_processCodeMesh") )
  {
    __debugbreak();
  }
  return (float (*)[4])frontEndDataOut->codeMeshArgs[argOffset];
}

GfxPackedVertex *__cdecl R_GetCodeMeshVerts(unsigned __int16 baseVertex)
{
  if ( !g_processCodeMesh
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
          1287,
          0,
          "%s",
          "g_processCodeMesh") )
  {
    __debugbreak();
  }
  return (GfxPackedVertex *)R_GetMeshVerts(frontEndDataOut->codeMeshPtr, baseVertex);
}

GfxWorldVertex *__cdecl R_GetMarkMeshVerts(unsigned __int16 baseVertex)
{
  if ( !g_processMarkMesh
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_drawsurf.cpp",
          1297,
          0,
          "%s",
          "g_processMarkMesh") )
  {
    __debugbreak();
  }
  return (GfxWorldVertex *)R_GetMeshVerts(&frontEndDataOut->markMesh, baseVertex);
}

