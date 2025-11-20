#pragma once

const GfxImage *__cdecl WeaponOptions::GetImageFromTable(const StringTable *table, int row, int column);
void __thiscall WeaponOptions::InitWeaponOptionTextures(
        WeaponOptions *this,
        const char *tableName,
        const char *label,
        int labelCol,
        int textureCol,
        const GfxImage **overrides,
        unsigned int num);
void __thiscall WeaponOptions::InitWeaponOptionMaterials(
        WeaponOptions *this,
        const char *tableName,
        const char *label,
        int labelCol,
        int materialCol,
        Material **overrides,
        unsigned int num);
void __thiscall WeaponOptions::InitWeaponOptionColors(
        WeaponOptions *this,
        const char *tableName,
        const char *label,
        int labelCol,
        int colorsCol,
        float (*colors)[4]);
void __thiscall WeaponOptions::InitWeaponOptions(WeaponOptions *this);
void __cdecl GC_InitWeaponOptionsDvars();
unsigned int __thiscall WeaponOptions::FindCamoOverrideIndex(
        WeaponOptions *this,
        const GfxImage *base,
        unsigned int defaultIndex);
void __thiscall WeaponOptions::WeaponOverride::AddToGold(
        WeaponOptions::WeaponOverride *this,
        const char *matName,
        bool gold);
void __thiscall WeaponOptions::WeaponOverride::Init(
        WeaponOptions::WeaponOverride *this,
        const WeaponVariantDef *weapVarDef);
bool __thiscall WeaponOptions::WeaponOverride::ToGold::Init(
        WeaponOptions::WeaponOverride::ToGold *this,
        const char *matName);
int __thiscall WeaponOptions::WeaponOverride::ToGold::Set(
        WeaponOptions::WeaponOverride::ToGold *this,
        int textureOverrideIndex,
        unsigned __int16 modelMask,
        const WeaponOptions::WeaponOverride::ToGold *gold);
const WeaponOptions::WeaponOverride *__thiscall WeaponOptions::GetWeaponOverrides(
        WeaponOptions *this,
        const WeaponVariantDef *weapVarDef);
int __cdecl GetShaderConstant(ShaderConstantSet *constantSet, const char *name);
int __thiscall WeaponOptions::SetupWeaponOptionsRender(
        WeaponOptions *this,
        int localClientNum,
        const float *origin,
        const WeaponVariantDef *weapVarDef,
        renderOptions_s weaponOptions,
        unsigned __int16 modelMask,
        ShaderConstantSet *constantSet,
        const char *name,
        int textureOverrideIndex);
bool __thiscall WeaponOptions::IsValidRenderOption(WeaponOptions *this, renderOptions_s renderOptions);
char __thiscall WeaponOptions::GetWeaponOptionReticleColor(
        WeaponOptions *this,
        unsigned int reticleColorIndex,
        float *outColor);
char __thiscall WeaponOptions::GetWeaponOptionLensColor(
        WeaponOptions *this,
        unsigned int reticleColorIndex,
        float *outColor);
void __cdecl GC_InitWeaponOptions();
int __cdecl CG_SetupWeaponOptionsRender(
        int localClientNum,
        const float *origin,
        const WeaponVariantDef *weaponVarDef,
        renderOptions_s renderOptions,
        ShaderConstantSet *constantSet,
        const char *name);
int __cdecl CG_SetupWeaponOptionsRender(
        int localClientNum,
        const float *origin,
        const WeaponVariantDef *wvd,
        renderOptions_s wo,
        unsigned __int16 modelMask,
        ShaderConstantSet *constantSet,
        const char *name,
        int textureOverrideIndex);
bool __cdecl CG_IsValidRenderOption(renderOptions_s renderOptions);
char __cdecl CG_GetWeaponOptionReticleColor(unsigned int reticleColorIndex, float *outColor);
char __cdecl CG_GetWeaponOptionLensColor(unsigned int reticleColorIndex, float *outColor);
