#include "db_assetnames.h"

XModelPiece *__cdecl DB_ImageGetName(const XAssetHeader *header)
{
  return header->xmodelPieces[3].pieces;
}

void __cdecl DB_ImageSetName(XAssetHeader *header, XModelPiece *name)
{
  header->xmodelPieces[3].pieces = name;
}

const char *__cdecl DB_LocalizeEntryGetName(const XAssetHeader *header)
{
  return (const char *)header->xmodelPieces->numpieces;
}

void __cdecl DB_LocalizeEntrySetName(XAssetHeader *header, const char *name)
{
  header->xmodelPieces->numpieces = (int)name;
}

void __cdecl DB_DDLSetname(XAssetHeader *header, const char *name)
{
  header->xmodelPieces->name = name;
}

const char *__cdecl DB_DDLGetName(const XAssetHeader *header)
{
  return header->xmodelPieces->name;
}

const char *__cdecl DB_GetEmblemSetName()
{
  return "emblemset";
}

const char *__cdecl DB_GetXAssetHeaderName(int type, const XAssetHeader *header)
{
  const char *v2; // eax
  const char *name; // [esp+0h] [ebp-4h]

  if ( !header
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\database\\db_assetnames.cpp", 687, 0, "%s", "header") )
  {
    __debugbreak();
  }
  if ( !DB_XAssetGetNameHandler[type]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_assetnames.cpp",
          688,
          0,
          "%s",
          "DB_XAssetGetNameHandler[type]") )
  {
    __debugbreak();
  }
  if ( !header->xmodelPieces
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\database\\db_assetnames.cpp", 689, 0, "%s", "header->data") )
  {
    __debugbreak();
  }
  name = DB_XAssetGetNameHandler[type](header);
  if ( !name )
  {
    v2 = va("Name \"%s\" not found for asset type %s\n", 0, g_assetNames[type]);
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_assetnames.cpp",
            691,
            0,
            "%s\n\t%s",
            "name",
            v2) )
      __debugbreak();
  }
  return name;
}

const char *__cdecl DB_GetXAssetName(const XAsset *asset)
{
  if ( !asset
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\database\\db_assetnames.cpp", 698, 0, "%s", "asset") )
  {
    __debugbreak();
  }
  return DB_GetXAssetHeaderName(asset->type, &asset->header);
}

void __cdecl DB_SetXAssetName(XAsset *asset, const char *name)
{
  if ( !DB_XAssetSetNameHandler[asset->type]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_assetnames.cpp",
          705,
          0,
          "%s",
          "DB_XAssetSetNameHandler[asset->type]") )
  {
    __debugbreak();
  }
  DB_XAssetSetNameHandler[asset->type](&asset->header, name);
}

int __cdecl DB_GetXAssetTypeSize(int type)
{
  if ( !DB_GetXAssetSizeHandler[type]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_assetnames.cpp",
          712,
          0,
          "%s",
          "DB_GetXAssetSizeHandler[type]") )
  {
    __debugbreak();
  }
  return DB_GetXAssetSizeHandler[type]();
}

const char *__cdecl DB_GetXAssetTypeName(unsigned int type)
{
  if ( type > 0x2A
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_assetnames.cpp",
          718,
          0,
          "%s",
          "type >= 0 && type < ASSET_TYPE_COUNT") )
  {
    __debugbreak();
  }
  return g_assetNames[type];
}

