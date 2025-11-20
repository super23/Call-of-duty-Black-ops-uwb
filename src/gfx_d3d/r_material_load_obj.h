#pragma once

bool __cdecl Material_CachedShaderTextLess(const GfxCachedShaderText *cached0, const GfxCachedShaderText *cached1);
void __cdecl Material_FreeAll();
void Material_FreeAllLiterals();
void Material_FreeAllStrings();
void Material_FreeAllStateMaps();
MaterialTechniqueSet *__cdecl Material_LoadTechniqueSet(char *name);
bool __cdecl Material_UsingTechnique(unsigned int techType);
bool __cdecl Material_MatchToken(const char **text, const char *match);
MaterialTechnique *__cdecl Material_RegisterTechnique(char *name);
MaterialTechnique *__cdecl Material_FindTechnique(const char *name);
char __cdecl MaterialTechnique_FindHashLocation(const char *name, unsigned int *foundHashIndex);
void __cdecl Material_SetTechnique(const char *name, MaterialTechnique *technique);
MaterialTechnique *__cdecl Material_LoadTechnique(char *name);
char __cdecl Material_LoadPass(
        char **text,
        unsigned __int16 *techFlags,
        MaterialPass *pass,
        MaterialStateMap **stateMap);
bool __cdecl Material_LoadPassStateMap(const char **text, MaterialStateMap **stateMap);
MaterialStateMap *__cdecl Material_RegisterStateMap(char *name);
MaterialStateMap *__cdecl Material_FindStateMap(const char *name);
char __cdecl Material_HashStateMap(const char *name, unsigned int *foundHashIndex);
void __cdecl Material_SetStateMap(const char *name, MaterialStateMap *stateMap);
MaterialStateMap *__cdecl Material_LoadStateMap(char *name);
bool __cdecl Material_ParseStateMap(char **text, MaterialStateMap *stateMap);
char __cdecl Material_ParseRuleSet(
        char **text,
        char *ruleSetName,
        const MtlStateMapBitGroup *stateSet,
        MaterialStateMapRuleSet **ruleSet);
int __cdecl Material_ParseRuleSetCondition(const char **text, const char *token, MaterialStateMapRule *rule);
int __cdecl Material_ParseRuleSetConditionTest(const char **text, const char *token, MaterialStateMapRule *rule);
const MtlStateMapBitName *__cdecl Material_ParseValueForState(const char **text, const MtlStateMapBitName *bitNames);
bool __cdecl Material_ParseRuleSetValue(
        const char **text,
        const char *token,
        const MtlStateMapBitGroup *stateSet,
        MaterialStateMapRule *rule);
MaterialStateMapRuleSet *__cdecl Material_AssembleRuleSet(int ruleCount, const MaterialStateMapRule *rules);
char __cdecl Material_LoadPassVertexDecl(
        const char **text,
        ShaderVaryingDef *inputTable,
        unsigned int inputCount,
        MaterialPass *pass);
MaterialVertexDeclaration *__cdecl Material_AllocVertexDecl(
        MaterialStreamRouting *routingData,
        unsigned int streamCount,
        bool *existing);
int __cdecl Material_HashVertexDecl(const MaterialStreamRouting *routingData, int streamCount);
char __cdecl Material_StreamDestForName(const char **text, const char *destName, unsigned __int8 *dest);
bool __cdecl Material_ParseIndex(const char **text, int indexCount, int *index);
char __cdecl Material_StreamSourceForName(const char **text, const char *sourceName, unsigned __int8 *source);
char __cdecl Material_ResourceDestForStreamDest(
        unsigned __int8 streamDest,
        ShaderVaryingDef *inputTable,
        unsigned int inputCount,
        unsigned __int8 *resourceDest);
const char *__cdecl Material_NameForStreamDest(unsigned __int8 dest);
bool __cdecl Material_CheckUnspecifiedVertexInputs(const ShaderVaryingDef *inputTable, unsigned int inputCount);
char __cdecl Material_LoadPassVertexShader(
        const char **text,
        unsigned __int16 *techFlags,
        ShaderParameterSet *paramSet,
        MaterialPass *pass,
        unsigned int argLimit,
        unsigned int *argCount,
        MaterialShaderArgument *args);
MaterialVertexShader *__cdecl Material_RegisterVertexShader(char *shaderName);
MaterialVertexShader *__cdecl Material_LoadVertexShader(char *shaderName);
ID3DXBuffer *__cdecl Material_CompileShader(
        char *shaderName,
        MaterialShaderType shaderType,
        char *entryPoint,
        char *target);
unsigned int __cdecl Material_GenerateShaderString(
        GfxAssembledShaderText *prog,
        const char *shaderName,
        MaterialShaderType shaderType,
        char *shaderString,
        unsigned int sizeofShaderString);
void __cdecl Material_EmitShaderString(GfxAssembledShaderText *prog, const char *string);
int __cdecl Material_MemCopyAndReturnLines(char *destString, const char *srcString, int len);
void __cdecl Material_AddShaderFile(GfxAssembledShaderText *prog, const char *shaderFileName, unsigned int srcLine);
bool __cdecl Material_GenerateShaderString_r(
        GfxAssembledShaderText *prog,
        const char *shaderName,
        const char *file,
        unsigned int fileSize,
        bool isInLibDir);
void __cdecl Material_EmitShaderChar(GfxAssembledShaderText *prog, char ch);
bool __cdecl Material_IncludeShader(GfxAssembledShaderText *prog, char *includeName, bool isInLibDir);
char __cdecl Material_FindCachedShaderText(const char *filename, const char **data, unsigned int *byteCount);
void __cdecl Material_FileIncludeFileAndLineNumber(
        GfxAssembledShaderText *prog,
        char *errorMessage,
        char **fileName,
        unsigned int *lineNumber);
bool __cdecl Material_ParseLineNumber(char *errorMessage, unsigned int *lineNumber);
char __cdecl Material_FindCachedShaderDX(
        char *shaderText,
        unsigned int shaderTextLen,
        const char *entryPoint,
        const char *target,
        ID3DXBuffer **shader,
        const char *shaderName,
        int useUPDB);
char __cdecl Material_FindCachedShader(
        const char *shaderText,
        unsigned int shaderTextLen,
        const char *filename,
        void **cachedShader,
        unsigned int *shaderLen);
void Material_DeleteOldCachedShaders();
void __cdecl Material_DeleteOldFilesInDirectory(const char *dirname, unsigned __int16 daysOld);
void __cdecl Material_SubtractDays(_SYSTEMTIME *sysTime, unsigned __int16 daysOld);
void __cdecl Material_DeleteDirectory(const char *dirname);
char __cdecl Material_CopyTextToDXBuffer(unsigned __int8 *cachedShader, unsigned int shaderLen, ID3DXBuffer **shader);
void __cdecl Material_CacheShaderDX(
        char *shaderText,
        unsigned int shaderTextLen,
        const char *entryPoint,
        const char *target,
        ID3DXBuffer *shader,
        int useUPDB);
void __cdecl Material_CacheShader(
        const char *shaderText,
        unsigned int shaderTextLen,
        const char *filename,
        const void *shaderBuffer,
        unsigned int shaderLen);
void __cdecl Material_GetShaderTargetString(char *target, unsigned int maxChars, const char *prefix);
char __cdecl Material_GetVertexShaderHashIndex(const char *shaderName, unsigned int *foundHashIndex);
int __cdecl Material_ParseShaderVersion(const char **text);
char __cdecl Material_SetPassShaderArguments_DX(
        char **text,
        char *shaderName,
        MaterialShaderType shaderType,
        const unsigned int *program,
        unsigned __int16 *techFlags,
        ShaderParameterSet *paramSet,
        unsigned int argLimit,
        unsigned int *argCount,
        MaterialShaderArgument *args);
unsigned int __cdecl Material_PrepareToParseShaderArguments(
        _D3DXSHADER_CONSTANTTABLE *constantTable,
        ShaderUniformDef *paramTable);
unsigned int __cdecl R_SetParameterDefArray(
        _D3DXSHADER_CONSTANTTABLE *constantTable,
        unsigned int constantIndex,
        ShaderUniformDef *paramDef);
char *__cdecl BufferOffset(char *buffer, int offset);
char __cdecl Material_ParseShaderArguments(
        char **text,
        char *shaderName,
        MaterialShaderType shaderType,
        ShaderUniformDef *paramTable,
        unsigned int paramCount,
        unsigned __int16 *techFlags,
        unsigned int argLimit,
        unsigned int *argCount,
        MaterialShaderArgument *args);
bool __cdecl Material_ParseIndexRange(const char **text, unsigned int arrayCount, ShaderIndexRange *indexRange);
unsigned int __cdecl Material_ElemCountForParamName(
        const char *shaderName,
        const ShaderUniformDef *paramTable,
        unsigned int paramCount,
        const char *name,
        ShaderParamType *paramType);
bool __cdecl Material_ParseArgumentSource(
        MaterialShaderType shaderType,
        const char **text,
        const char *shaderName,
        ShaderParamType paramType,
        ShaderArgumentSource *argSource);
bool __cdecl Material_ParseSamplerSource(const char **text, ShaderArgumentSource *argSource);
const char *__cdecl Material_RegisterString(char *string);
bool __cdecl Material_CodeSamplerSource_r(
        const char **text,
        int offset,
        const CodeSamplerSource *sourceTable,
        ShaderArgumentSource *argSource);
char __cdecl Material_ParseArrayOffset(const char **text, int arrayCount, int arrayStride, int *offset);
bool __cdecl Material_ParseConstantSource(
        MaterialShaderType shaderType,
        const char **text,
        ShaderArgumentSource *argSource);
float *__cdecl Material_RegisterLiteral(float *literal);
char __cdecl Material_ParseLiteral(const char **text, const char *token, float *literal);
bool __cdecl Material_ParseVector(const char **text, int elemCount, float *vector);
bool __cdecl Material_ParseCodeConstantSource_r(
        MaterialShaderType shaderType,
        const char **text,
        int offset,
        const CodeConstantSource *sourceTable,
        ShaderArgumentSource *argSource);
bool __cdecl Material_DefaultArgumentSource(
        MaterialShaderType shaderType,
        const char *constantName,
        ShaderParamType paramType,
        const ShaderIndexRange *indexRange,
        ShaderArgumentSource *argSource);
bool __cdecl Material_DefaultSamplerSource(
        const char *constantName,
        const ShaderIndexRange *indexRange,
        ShaderArgumentSource *argSource);
char __cdecl Material_DefaultSamplerSourceFromTable(
        const char *constantName,
        const ShaderIndexRange *indexRange,
        const CodeSamplerSource *sourceTable,
        ShaderArgumentSource *argSource);
char __cdecl Material_DefaultIndexRange(
        const ShaderIndexRange *indexRangeRef,
        unsigned int arrayCount,
        ShaderIndexRange *indexRangeSet);
bool __cdecl Material_DefaultConstantSource(
        MaterialShaderType shaderType,
        const char *constantName,
        const ShaderIndexRange *indexRange,
        ShaderArgumentSource *argSource);
char __cdecl Material_DefaultConstantSourceFromTable(
        MaterialShaderType shaderType,
        const char *constantName,
        const ShaderIndexRange *indexRange,
        const CodeConstantSource *sourceTable,
        ShaderArgumentSource *argSource);
bool __cdecl Material_UnknownShaderworksConstantSource(
        MaterialShaderType shaderType,
        const char *constantName,
        const ShaderIndexRange *indexRange,
        ShaderArgumentSource *argSource);
char __cdecl Material_SetShaderArguments(
        unsigned int usedCount,
        MaterialShaderArgument *localArgs,
        unsigned int argLimit,
        unsigned int *argCount,
        MaterialShaderArgument *args);
int __cdecl Material_CompareShaderArgumentsForCombining(unsigned __int16 *e0, unsigned __int16 *e1);
unsigned int __cdecl Material_CombineShaderArguments(unsigned int usedCount, MaterialShaderArgument *localArgs);
char __cdecl Material_AttemptCombineShaderArguments(MaterialShaderArgument *arg0, const MaterialShaderArgument *arg1);
bool __cdecl Material_AddShaderArgument(
        const char *shaderName,
        MaterialShaderType shaderType,
        ShaderArgumentSource *argSource,
        const ShaderArgumentDest *argDest,
        ShaderUniformDef *paramTable,
        unsigned int paramCount,
        unsigned int *usedCount,
        MaterialShaderArgument *argTable,
        char (*registerUsage)[64]);
ShaderUniformDef *__cdecl Material_GetShaderArgumentDest(
        const char *paramName,
        unsigned int paramIndex,
        ShaderUniformDef *paramTable,
        unsigned int paramCount);
char __cdecl Material_AddShaderArgumentFromLiteral(
        const char *shaderName,
        const char *paramName,
        unsigned __int16 type,
        const float *literal,
        ShaderUniformDef *dest,
        MaterialShaderArgument *arg,
        char (*registerUsage)[64]);
char __cdecl MaterialAddShaderArgument(
        const char *shaderName,
        const char *paramName,
        MaterialShaderArgument *arg,
        char (*registerUsage)[64]);
char __cdecl Material_AddShaderArgumentFromCodeConst(
        const char *shaderName,
        const char *paramName,
        unsigned __int16 type,
        unsigned int codeIndex,
        __int16 offset,
        ShaderUniformDef *dest,
        MaterialShaderArgument *arg,
        char (*registerUsage)[64]);
void __cdecl Material_AddShaderArgumentFromCodeSampler(
        unsigned __int16 type,
        unsigned int codeSampler,
        ShaderUniformDef *dest,
        MaterialShaderArgument *arg);
char __cdecl Material_AddShaderArgumentFromMaterial(
        const char *shaderName,
        const char *paramName,
        unsigned __int16 type,
        char *name,
        ShaderUniformDef *dest,
        MaterialShaderArgument *arg,
        char (*registerUsage)[64]);
char __cdecl CodeConstIsOneOf(unsigned __int16 constCodeIndex, const unsigned __int16 *consts, int numConsts);
void __cdecl Material_SetVaryingParameterDef(const _D3DXSEMANTIC *semantic, ShaderVaryingDef *paramDef);
unsigned __int8 __cdecl Material_GetStreamDestForSemantic(const _D3DXSEMANTIC *semantic);
char __cdecl Material_LoadDeclTypes(const char **text, MaterialPass *pass);
char __cdecl Material_LoadPassPixelShader(
        const char **text,
        unsigned __int16 *techFlags,
        ShaderParameterSet *paramSet,
        MaterialPass *pass,
        unsigned int argLimit,
        unsigned int *argCount,
        MaterialShaderArgument *args);
MaterialPixelShader *__cdecl Material_RegisterPixelShader(char *shaderName);
MaterialPixelShader *__cdecl Material_LoadPixelShader(char *shaderName);
char __cdecl Material_GetPixelShaderHashIndex(const char *shaderName, unsigned int *foundHashIndex);
bool __cdecl Material_ValidateShaderLinkage(
        const ShaderVaryingDef *vertexOutputs,
        unsigned int vertexOutputCount,
        const ShaderVaryingDef *pixelInputs,
        unsigned int pixelInputCount);
char __cdecl Material_HasMatchingParameter(
        unsigned __int8 find,
        const ShaderVaryingDef *paramTable,
        unsigned int paramCount);
char __cdecl Material_HasMatchingParameter_BuggySdkWorkaround(
        unsigned __int8 find,
        const ShaderVaryingDef *paramTable,
        unsigned int paramCount);
int __cdecl Material_CompareShaderArgumentsForRuntime(
        const MaterialShaderArgument *e0,
        const MaterialShaderArgument *e1);
int __cdecl Material_GetArgUpdateFrequency(const MaterialShaderArgument *arg);
unsigned __int8 __cdecl Material_CountArgsWithUpdateFrequency(
        MaterialUpdateFrequency updateFreq,
        const MaterialShaderArgument *args,
        unsigned int argCount,
        unsigned int *firstArg);
char __cdecl Material_IgnoreTechnique(const char *name);
unsigned __int8 __cdecl Material_TechniqueTypeForName(const char *name);
bool __cdecl MaterialTexture_IsMatureContent(unsigned __int8 texSemantic, char *texImageName);
Material *__cdecl Material_Load(const char *assetName, int imageTrack);
Material *__cdecl Material_LoadLayered(const char *assetName);
Material *__cdecl Material_CreateLayered(
        const char *name,
        const Material **layerMtl,
        unsigned int layerCount,
        MaterialTechniqueSet *techSet);
void __cdecl Material_SetMaterialDrawRegion(Material *material);
unsigned int __cdecl Material_GetTechniqueSetDrawRegion(MaterialTechniqueSet *techniqueSet);
void __cdecl Material_SetStateBits(Material *material, unsigned int (*stateBitsTable)[2], unsigned int stateBitsCount);
void __cdecl Material_UpdateStateFlags(Material *mtl);
unsigned int __cdecl Material_GetCullFlags(Material *material);
unsigned int __cdecl Material_GetCullShadowFlags(Material *material);
int __cdecl Material_GetDecalFlags(const Material *mtl);
int __cdecl Material_GetWritesDepthFlags(const Material *mtl);
unsigned int __cdecl Material_GetUsesDepthBufferFlags(const Material *mtl);
unsigned int __cdecl Material_GetUsesStencilBufferFlags(const Material *mtl);
void __cdecl Material_UpdateTechniqueFlags(Material *material);
char __cdecl Material_HasConstant(const Material *mtl, unsigned int nameHash);
char __cdecl Material_Validate(const Material *material);
char __cdecl Material_ValidateTechnique(const Material *material, const MaterialTechnique *technique);
char __cdecl Material_ValidatePassArguments(
        const Material *mtl,
        const char *techniqueName,
        unsigned int argCount,
        const MaterialShaderArgument *args);
const char *__cdecl Material_StringFromHash(unsigned int hash);
char __cdecl Material_HasTexture(const Material *mtl, unsigned int nameHash);
int __cdecl CompareHashedMaterialTextures(unsigned int *e0, unsigned int *e1);
void __cdecl Material_AppendCharToConstName(char *name, char ch);
unsigned int __cdecl Material_CreateLayeredStateBitsTable(
        const Material **layerMtl,
        unsigned int layerCount,
        const MaterialTechniqueSet *techSet,
        unsigned __int8 *stateBitsEntry,
        unsigned int (*stateBitsTable)[2]);
unsigned __int8 __cdecl Material_AddStateBitsArrayToTable(
        const unsigned int (*stateBitsForPass)[2],
        unsigned int passCount,
        unsigned int (*stateBitsTable)[2],
        unsigned int *stateBitsCount);
void __cdecl Material_GetLayeredStateBits(
        const Material **layerMtl,
        unsigned int layerCount,
        unsigned int techType,
        unsigned int *stateBits);
bool __cdecl Material_HasNormalMap(const Material *mtl);
MaterialTechniqueSet *__cdecl Material_RegisterLayeredTechniqueSet(const Material **mtl, unsigned int layerCount);
unsigned int __cdecl Material_AppendTechniqueSetName(char *name, unsigned int nameLen, char *append, char lyrToken);
const LayeredTechniqueSetName *__cdecl Material_GetLayeredTechniqueSetName(const char *techSetName);
Material *__cdecl Material_LoadRaw(const MaterialRaw *mtlRaw, unsigned int materialType, int imageTrack);
water_t *__cdecl Material_RegisterWaterImage(const MaterialWaterDef *water);
void __cdecl Material_BuildStateBitsTable(Material *material, __int16 toolFlags, unsigned int *refStateBits);
void __cdecl Material_RemapStateBits(
        const Material *material,
        __int16 toolFlags,
        const MaterialStateMap *stateMap,
        const unsigned int *refStateBits,
        unsigned int *stateBitsOut);
void __cdecl Material_ApplyStateBitsRemapRuleSet(
        const Material *material,
        const MaterialStateMap *stateMap,
        unsigned int ruleSetIndex,
        const unsigned int *refStateBits,
        unsigned int *stateBitsOut);
bool __cdecl Material_FinishLoadingInstance(
        const MaterialRaw *mtlRaw,
        const char *techniqueSetVertDeclPrefix,
        MaterialTechniqueSet **techniqueSet,
        unsigned int materialType,
        int imageTrack);
bool __cdecl Material_FinishLoadingTexdef(
        const MaterialRaw *material,
        MaterialTextureDefRaw *texdef,
        unsigned int materialType,
        int imageTrack);
bool __cdecl Material_RegisterImage(
        const MaterialRaw *material,
        int imageNameOffset,
        unsigned __int8 semantic,
        int imageTrack);
bool __cdecl R_IsWorldMaterialType(unsigned int materialType);
int __cdecl CompareRawMaterialTextures(unsigned int *e0, unsigned int *e1);
void __cdecl Material_PreLoadAllShaderText();
void __cdecl Material_PreLoadSingleShaderText(const char *filename, const char *subdir, GfxCachedShaderText *cached);
void __cdecl Material_GetInfo(Material *handle, MaterialInfo *matInfo);
bool __cdecl Material_HasImpactMaskTech(const Material *material);
bool __cdecl Material_IsForSeeThruBulletHoleDecal(const Material *mat);
bool __cdecl Material_IsSeeThruParallaxDecal(const Material *mat);
void __cdecl Material_Sort();
void __cdecl Material_SortInternal(Material **sortedMaterials, unsigned int materialCount);
unsigned int __cdecl R_DrawSurfStandardPrepassSortKey(const Material *material);
bool __cdecl Material_Compare(const Material *mtl0, const Material *mtl1);
void __cdecl Material_GetVertexShaderName(char *dest, const MaterialPass *pass, int destsize);
int __cdecl Material_ComparePixelConsts(const Material *mtl0, const Material *mtl1, unsigned __int8 techType);
unsigned int __cdecl R_DrawSurfPrimarySortKey(const Material *material);
char __cdecl R_FindScriptableConstantSource_ByName(const char *name, unsigned __int8 *dest);
void __cdecl R_BuildScritableConstantArray(ScriptableConstant *array, int arraySize);
const char *__cdecl R_FindCodeConstantName_BySource(unsigned __int8 src);
const char *__cdecl R_FindScriptableConstantName_BySource(unsigned __int8 src);
