#include "fx_archive.h"

FxEffectContainer *__cdecl FX_EffectFromHandle(FxSystem *system, unsigned __int16 handle)
{
  const char *v2; // eax

  if ( !system
    && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\effectscore\\fx_system.h", 399, 0, "%s", "system") )
  {
    __debugbreak();
  }
  if ( handle >= 0xD000u || handle % 0x34u )
  {
    v2 = va("%p %i", system->effects, handle);
    if ( !Assert_MyHandler(
            "c:\\projects_pc\\cod\\codsrc\\src\\effectscore\\fx_system.h",
            400,
            0,
            "%s\n\t%s",
            "handle < FX_EFFECT_LIMIT * sizeof( FxEffectContainer ) / FxEffect::HANDLE_SCALE && handle % (sizeof( FxEffec"
            "tContainer ) / FxEffect::HANDLE_SCALE) == 0",
            v2) )
      __debugbreak();
  }
  return (FxEffectContainer *)((char *)system->effects + 4 * handle);
}

FxPool<FxElem,FxElemContainer> *__cdecl FX_ElemFromHandle(FxSystem *system, unsigned __int16 handle)
{
  if ( !system
    && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\effectscore\\fx_system.h", 479, 0, "%s", "system") )
  {
    __debugbreak();
  }
  return FX_PoolFromHandle_Generic<FxElem,FxElemContainer,2048>(system->elems, handle);
}

FxElemVisuals __cdecl FX_GetElemVisuals(const FxElemDef *elemDef, int randomSeed)
{
  if ( !elemDef->visualCount
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\effectscore\\fx_draw.h",
          92,
          0,
          "%s\n\t(elemDef->visualCount) = %i",
          "(elemDef->visualCount > 0)",
          elemDef->visualCount) )
  {
    __debugbreak();
  }
  if ( elemDef->visualCount == 1 )
    return elemDef->visuals.instance;
  else
    return (FxElemVisuals)elemDef->visuals.markArray->materials[(elemDef->visualCount
                                                               * LOWORD(fx_randomTable[randomSeed + 21])) >> 16];
}

void __cdecl MemFile_WriteCString(MemoryFile *memFile, char *string)
{
  if ( !string
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\effectscore\\../universal/memfile.h",
          144,
          0,
          "%s",
          "string") )
  {
    __debugbreak();
  }
  MemFile_WriteData(memFile, strlen(string) + 1, (unsigned __int8 *)string);
}

FxPool<FxElem,FxElemContainer> *__cdecl FX_PoolFromHandle_Generic<FxElem,FxElemContainer,2048>(
        FxPool<FxElem,FxElemContainer> *poolArray,
        unsigned __int16 handle)
{
  const char *v2; // eax

  if ( handle >= 0x6000u || handle % 0xCu )
  {
    v2 = va("%p %i size=%d limit=%d handlescale=%d", poolArray, handle, 48, 2048, 4);
    if ( !Assert_MyHandler(
            "c:\\projects_pc\\cod\\codsrc\\src\\effectscore\\fx_system.h",
            465,
            0,
            "%s\n\t%s",
            "handle < LIMIT * sizeof( CONTAINER_TYPE ) / ITEM_TYPE::HANDLE_SCALE && handle % (sizeof( CONTAINER_TYPE ) / "
            "ITEM_TYPE::HANDLE_SCALE) == 0",
            v2) )
      __debugbreak();
  }
  return (FxPool<FxElem,FxElemContainer> *)((char *)poolArray + 4 * handle);
}

