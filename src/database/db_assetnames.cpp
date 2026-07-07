#include "db_assetnames.h"
#include "db_registry.h"

#include <gfx_d3d/r_material.h>

#include <xanim/xmodel.h>
#include <universal/q_shared.h>

#include <physics/physpreset_load_obj.h>
#include <physics/physconstraints_load_obj.h>
#include <glass/glass.h>
#include "db_load.h"
#include <bgame/bg_weapons.h>
#include <bgame/bg_local.h>
#include <gfx_d3d/r_bsp_load_obj.h>
#include <bgame/bg_emblems.h>
#include <qcommon/com_bsp.h>
#include <gfx_d3d/r_extracam.h>
#include <stringed/stringed_hooks.h>
#include <xanim/xanim.h>

namespace
{
    bool DB_IsValidXAssetType(int type)
    {
        return static_cast<unsigned int>(type) < ASSET_TYPE_COUNT;
    }
}

const char *(__cdecl *DB_XAssetGetNameHandler[ASSET_TYPE_COUNT])(const XAssetHeader *) =
{
  &DB_DDLGetName,
  &DB_DDLGetName,
  &DB_DDLGetName,
  &DB_DDLGetName,
  &DB_DDLGetName,
  &DB_DDLGetName,
  &DB_DDLGetName,
  &DB_DDLGetName,
  &DB_ImageGetName,
  &DB_DDLGetName,
  &DB_DDLGetName,
  &DB_DDLGetName,
  &DB_DDLGetName,
  &DB_DDLGetName,
  &DB_DDLGetName,
  &DB_DDLGetName,
  &DB_DDLGetName,
  &DB_DDLGetName,
  &DB_DDLGetName,
  NULL,
  &DB_DDLGetName,
  &DB_DDLGetName,
  &DB_DDLGetName,
  &DB_LocalizeEntryGetName,
  &DB_DDLGetName,
  NULL,
  NULL,
  &DB_DDLGetName,
  &DB_DDLGetName,
  &DB_DDLGetName,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  &DB_DDLGetName,
  &DB_DDLGetName,
  &DB_DDLGetName,
  &DB_DDLGetName,
  &DB_DDLGetName,
  &DB_DDLGetName,
  &DB_GetEmblemSetName
};
static_assert(ARRAY_COUNT(DB_XAssetGetNameHandler) == ASSET_TYPE_COUNT);

const char *g_assetNames[ASSET_TYPE_COUNT] =
{
  "xmodelpieces",
  "physpreset",
  "physconstraints",
  "destructibledef",
  "xanim",
  "xmodel",
  "material",
  "techset",
  "image",
  "sound",
  "sound_patch",
  "col_map_sp",
  "col_map_mp",
  "com_map",
  "game_map_sp",
  "game_map_mp",
  "map_ents",
  "gfx_map",
  "lightdef",
  "ui_map",
  "font",
  "menufile",
  "menu",
  "localize",
  "weapon",
  "weapondef",
  "weaponvariant",
  "snddriverglobals",
  "fx",
  "impactfx",
  "aitype",
  "mptype",
  "mpbody",
  "mphead",
  "character",
  "xmodelalias",
  "rawfile",
  "stringtable",
  "packindex",
  "xGlobals",
  "ddl",
  "glasses",
  "emblemset"
};
static_assert(ARRAY_COUNT(g_assetNames) == ASSET_TYPE_COUNT);

void(__cdecl *DB_XAssetSetNameHandler[ASSET_TYPE_COUNT])(XAssetHeader *, const char *) =
{
  &DB_DDLSetName,
  &DB_DDLSetName,
  &DB_DDLSetName,
  &DB_DDLSetName,
  &DB_DDLSetName,
  &DB_DDLSetName,
  &DB_DDLSetName,
  &DB_DDLSetName,
  (void(*)(XAssetHeader*, const char*))&DB_ImageSetName,
  &DB_DDLSetName,
  NULL,
  &DB_DDLSetName,
  &DB_DDLSetName,
  &DB_DDLSetName,
  &DB_DDLSetName,
  &DB_DDLSetName,
  &DB_DDLSetName,
  &DB_DDLSetName,
  &DB_DDLSetName,
  NULL,
  &DB_DDLSetName,
  &DB_DDLSetName,
  &DB_DDLSetName,
  &DB_LocalizeEntrySetName,
  &DB_DDLSetName,
  NULL,
  NULL,
  &DB_DDLSetName,
  &DB_DDLSetName,
  &DB_DDLSetName,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  &DB_DDLSetName,
  &DB_DDLSetName,
  &DB_DDLSetName,
  &DB_DDLSetName,
  &DB_DDLSetName,
  NULL,
  NULL
};
static_assert(ARRAY_COUNT(DB_XAssetSetNameHandler) == ASSET_TYPE_COUNT);

static_assert(sizeof(RawFile) == 12);
int __cdecl DB_SizeofXAsset_RawFile_()
{
    return 12;
}

static_assert(sizeof(PhysPreset) == 84);
int __cdecl DB_SizeofXAsset_PhysPreset_()
{
    return 84;
}

static_assert(sizeof(PhysConstraints) == 2696);
int __cdecl DB_SizeofXAsset_PhysConstraints_()
{
    return 2696;
}

static_assert(sizeof(DestructibleDef) == 24);
int __cdecl DB_SizeofXAsset_DestructibleDef_()
{
    return 24;
}

static_assert(sizeof(Font_s) == 24);
int __cdecl DB_SizeofXAsset_Font_s_()
{
    return 24;
}

static_assert(sizeof(XAnimParts) == 104);
int __cdecl DB_SizeofXAsset_XAnimParts_()
{
    return 104;
}

static_assert(sizeof(XModel) == 252);
int __cdecl DB_SizeofXAsset_XModel_()
{
    return 252;
}

// (for some reason the intellisense size is wrong unless it's right next to the struct definition)
int __cdecl DB_SizeofXAsset_Material_()
{
    return 192;
}

static_assert(sizeof(MaterialTechniqueSet) == 528);
int __cdecl DB_SizeofXAsset_MaterialTechniqueSet_()
{
    return 528;
}

static_assert(sizeof(SndDriverGlobals) == 52);
int __cdecl DB_SizeofXAsset_SndDriverGlobals_()
{
    return 52;
}

static_assert(sizeof(XGlobals) == 40);
int __cdecl DB_SizeofXAsset_XGlobals_()
{
    return 40;
}

static_assert(sizeof(StringTable) == 20);
int __cdecl DB_SizeofXAsset_StringTable_()
{
    return 20;
}

static_assert(sizeof(clipMap_t) == 332);
int __cdecl DB_SizeofXAsset_clipMap_t_()
{
    return 332;
}

static_assert(sizeof(ComWorld) == 64);
int __cdecl DB_SizeofXAsset_ComWorld_()
{
    return 64;
}

static_assert(sizeof(EmblemSet) == 44);
int __cdecl DB_SizeofXAsset_EmblemSet_()
{
    return 44;
}

static_assert(sizeof(GfxWorld) == 1084);
int __cdecl DB_SizeofXAsset_GfxWorld_()
{
    return 1084;
}

static_assert(sizeof(GfxLightDef) == 16);
int __cdecl DB_SizeofXAsset_GfxLightDef_()
{
    return 16;
}

static_assert(sizeof(menuDef_t) == 400);
int __cdecl DB_SizeofXAsset_menuDef_t_()
{
    return 400;
}

static_assert(sizeof(XAnimTree_s) == 8);

static_assert(sizeof(WeaponVariantDef) == 228);
int __cdecl DB_SizeofXAsset_WeaponVariantDef_()
{
    return 228;
}

static_assert(sizeof(FxEffectDef) == 60);
int __cdecl DB_SizeofXAsset_FxEffectDef_()
{
    return 60;
}

static_assert(sizeof(PackIndex) == 28);
int __cdecl DB_SizeofXAsset_PackIndex_()
{
    return 28;
}

static_assert(sizeof(Glasses) == 56);
int __cdecl DB_SizeofXAsset_Glasses_()
{
    return 56;
}

int(__cdecl *DB_GetXAssetSizeHandler[ASSET_TYPE_COUNT])() =
{
  &DB_SizeofXAsset_RawFile_,
  &DB_SizeofXAsset_PhysPreset_,
  &DB_SizeofXAsset_PhysConstraints_,
  &DB_SizeofXAsset_DestructibleDef_,
  DB_SizeofXAsset_XAnimParts_,
  &DB_SizeofXAsset_XModel_,
  &DB_SizeofXAsset_Material_,
  &DB_SizeofXAsset_MaterialTechniqueSet_,
  &DB_SizeofXAsset_SndDriverGlobals_,
  &DB_SizeofXAsset_XGlobals_,
  &DB_SizeofXAsset_StringTable_,
  &DB_SizeofXAsset_clipMap_t_,
  &DB_SizeofXAsset_clipMap_t_,
  &DB_SizeofXAsset_ComWorld_,
  &DB_SizeofXAsset_EmblemSet_,
  &DB_SizeofXAsset_EmblemSet_,
  &DB_SizeofXAsset_RawFile_,
  &DB_SizeofXAsset_GfxWorld_,
  &DB_SizeofXAsset_GfxLightDef_,
  NULL,
  &DB_SizeofXAsset_Font_s_,
  &DB_SizeofXAsset_RawFile_,
  &DB_SizeofXAsset_menuDef_t_,
  &XAnimTreeSize,
  &DB_SizeofXAsset_WeaponVariantDef_,
  NULL,
  NULL,
  &DB_SizeofXAsset_SndDriverGlobals_,
  &DB_SizeofXAsset_FxEffectDef_,
  &XAnimTreeSize,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  &DB_SizeofXAsset_RawFile_,
  &DB_SizeofXAsset_StringTable_,
  &DB_SizeofXAsset_PackIndex_,
  &DB_SizeofXAsset_XGlobals_,
  &XAnimTreeSize,
  &DB_SizeofXAsset_Glasses_,
  &DB_SizeofXAsset_EmblemSet_
};
static_assert(ARRAY_COUNT(DB_GetXAssetSizeHandler) == ASSET_TYPE_COUNT);





const char *__cdecl DB_ImageGetName(const XAssetHeader *header)
{
    return header->image->name;
}

void __cdecl DB_ImageSetName(XAssetHeader *header, const char *name)
{
    header->image->name = name;
}

const char *__cdecl DB_LocalizeEntryGetName(const XAssetHeader *header)
{
    return header->localize->name;
}

void __cdecl DB_LocalizeEntrySetName(XAssetHeader *header, const char *name)
{
    header->localize->name = name;
}

void __cdecl DB_DDLSetName(XAssetHeader *header, const char *name)
{
    header->xmodelPieces->name = name;
}

const char *__cdecl DB_DDLGetName(const XAssetHeader *header)
{
    return header->xmodelPieces->name;
}

const char *__cdecl DB_GetEmblemSetName(const XAssetHeader *header)
{
    return "emblemset";
}

const char *__cdecl DB_GetXAssetHeaderName(int type, const XAssetHeader *header)
{
    const char *name;

    if ( !DB_IsValidXAssetType(type)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_assetnames.cpp",
                    688,
                    0,
                    "%s",
                    "type >= 0 && type < ASSET_TYPE_COUNT") )
    {
        __debugbreak();
        return NULL;
    }
    if ( !header
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\database\\db_assetnames.cpp", 687, 0, "%s", "header") )
    {
        __debugbreak();
        return NULL;
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
        return NULL;
    }
    if ( !header->data
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\database\\db_assetnames.cpp", 689, 0, "%s", "header->data") )
    {
        __debugbreak();
        return NULL;
    }
    name = DB_XAssetGetNameHandler[type](header);
    if ( !name )
    {
        if ( !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_assetnames.cpp",
                        691,
                        0,
                        "%s\n\t%s",
                        "name",
                        va("Name \"%s\" not found for asset type %s\n", "<null>", g_assetNames[type])) )
        {
            __debugbreak();
        }
    }
    return name;
}

const char *__cdecl DB_GetXAssetName(const XAsset *asset)
{
    if ( !asset
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\database\\db_assetnames.cpp", 698, 0, "%s", "asset") )
    {
        __debugbreak();
        return NULL;
    }
    return DB_GetXAssetHeaderName(asset->type, &asset->header);
}

void __cdecl DB_SetXAssetName(XAsset *asset, const char *name)
{
    if ( !asset
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\database\\db_assetnames.cpp", 704, 0, "%s", "asset") )
    {
        __debugbreak();
        return;
    }
    if ( !DB_IsValidXAssetType(asset->type)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_assetnames.cpp",
                    705,
                    0,
                    "%s",
                    "asset->type >= 0 && asset->type < ASSET_TYPE_COUNT") )
    {
        __debugbreak();
        return;
    }
    if ( !DB_XAssetSetNameHandler[asset->type]
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_assetnames.cpp",
                    705,
                    0,
                    "%s",
                    "DB_XAssetSetNameHandler[asset->type]") )
    {
        __debugbreak();
        return;
    }
    DB_XAssetSetNameHandler[asset->type](&asset->header, name);
}

int __cdecl DB_GetXAssetTypeSize(int type)
{
    if ( !DB_IsValidXAssetType(type)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_assetnames.cpp",
                    711,
                    0,
                    "%s",
                    "type >= 0 && type < ASSET_TYPE_COUNT") )
    {
        __debugbreak();
        return 0;
    }
    if ( !DB_GetXAssetSizeHandler[type]
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_assetnames.cpp",
                    712,
                    0,
                    "%s",
                    "DB_GetXAssetSizeHandler[type]") )
    {
        __debugbreak();
        return 0;
    }
    return DB_GetXAssetSizeHandler[type]();
}

const char *__cdecl DB_GetXAssetTypeName(unsigned int type)
{
    if ( type >= ASSET_TYPE_COUNT
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\database\\db_assetnames.cpp",
                    718,
                    0,
                    "%s",
                    "type >= 0 && type < ASSET_TYPE_COUNT") )
    {
        __debugbreak();
        return NULL;
    }
    return g_assetNames[type];
}

