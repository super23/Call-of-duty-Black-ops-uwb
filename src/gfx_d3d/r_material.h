#pragma once

void __cdecl Material_FreeTechniqueSet(XAssetHeader header);
unsigned __int8 *__cdecl Material_Alloc(unsigned int size);
void __cdecl Load_CreateMaterialPixelShader(GfxPixelShaderLoadDef *loadDef, MaterialPixelShader *mtlShader);
void __cdecl Load_CreateMaterialVertexShader(GfxVertexShaderLoadDef *loadDef, MaterialVertexShader *mtlShader);
void __cdecl Load_BuildVertexDecl(MaterialVertexDeclaration **mtlVertDecl);
IDirect3DVertexDeclaration9 *__cdecl Material_BuildVertexDecl(
        const MaterialStreamRouting *routingData,
        int streamCount,
        const stream_source_info_t *sourceTable);
MaterialTechniqueSet *__cdecl Material_FindTechniqueSet(const char *name, MtlTechSetNotFoundBehavior notFoundBehavior);
MaterialTechniqueSet *__cdecl Material_FindTechniqueSet_LoadObj(
        const char *name,
        MtlTechSetNotFoundBehavior notFoundBehavior);
char __cdecl MaterialTechniqueSet_FindHashLocation(const char *name, int *foundHashIndex);
MaterialTechniqueSet *__cdecl Material_FindTechniqueSet_FastFile(
        const char *name,
        MtlTechSetNotFoundBehavior notFoundBehavior);
void __cdecl Material_SetAlwaysUseDefaultMaterial(bool alwaysUseDefaultMaterial);
void __cdecl Material_PreventOverrideTechniqueGeneration();
MaterialTechniqueSet *__cdecl Material_RegisterTechniqueSet(char *name);
void __cdecl Material_SetTechniqueSet(const char *name, MaterialTechniqueSet *techniqueSet);
void __cdecl Material_ReleaseTechniqueSetResources(MaterialTechniqueSet *techniqueSet);
void __cdecl Material_ReleasePassResources(MaterialPass *pass);
void __cdecl Material_FreeAllTechniqueSets();
void __cdecl Material_DirtySort();
Material *__cdecl Material_Duplicate(Material *mtlCopy, char *name);
void __cdecl Material_Add(Material *material, unsigned __int16 hashIndex);
bool __cdecl Material_IsDefault(const Material *material);
void __cdecl Material_GetHashIndex(const char *name, unsigned __int16 *hashIndex, bool *exists);
Material *__cdecl Material_Register(char *name, int imageTrack);
Material *__cdecl Material_Register_LoadObj(char *name, int imageTrack);
Material *__cdecl Material_MakeDefault(char *name);
Material *__cdecl Material_Register_FastFile(const char *name);
Material *__cdecl Material_RegisterHandle(char *name, int imageTrack);
void __cdecl R_MaterialList_f();
void __cdecl R_GetMaterialList(XAssetHeader header, char *data);
int __cdecl R_GetMaterialMemory(Material *material);
bool __cdecl R_MaterialCompare(const MaterialMemory *material0, const MaterialMemory *material1);
void __cdecl R_MaterialEdit_f();
void __cdecl R_MaterialParameterEdit_f();
void __cdecl R_MaterialParameterTweak_f();
void __cdecl R_MaterialParameterTweakUpdate();
void __cdecl R_EnumTechniqueSets(void (__cdecl *func)(MaterialTechniqueSet *, void *), void *data);
void __cdecl R_EnumMaterials(void (__cdecl *func)(Material *, void *), void *data);
const char *__cdecl Material_GetName(Material *handle);
void __cdecl R_InitAssets_PostMapFastfileLoad();
void __cdecl Material_LoadPerMap(const PerMapMaterialTable *mtlTable, int mtlTableCount);
void __cdecl Material_Init();
void __cdecl Material_LoadBuiltIn(const BuiltInMaterialTable *mtlTable, int mtlTableCount);
void __cdecl Material_Shutdown();
void __cdecl Material_UpdatePicmipAll();
void __cdecl Material_UpdatePicmipSingle(XAssetHeader header);
void __cdecl Material_UpdatePicmipForTexdef(const MaterialTextureDef *texdef);
void __cdecl R_Cmd_ReloadMaterialTextures();
Material *__cdecl Material_Find(const char *name);
void __cdecl Material_ReloadTextures(const Material *material);
unsigned int __cdecl Material_LoadFile(const char *filename, int *file);
bool __cdecl IsValidMaterialHandle(Material *const handle);
int __cdecl GetCodeConst_GenericParam0();
int __cdecl GetCodeConst_GenericParam1();
int __cdecl GetCodeConst_GenericParam2();
int __cdecl GetCodeConst_GenericParam3();
int __cdecl GetCodeConst_GenericParamA();
int __cdecl GetCodeConst_GenericParamB();
int __cdecl GetCodeConst_GameTimeParam();
int __cdecl Material_GetTextureCount(Material *handle);
GfxImage *__cdecl Material_GetTexture(Material *handle, int index);
