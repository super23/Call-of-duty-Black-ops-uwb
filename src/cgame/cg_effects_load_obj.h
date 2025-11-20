#pragma once

FxImpactTable *__cdecl CG_RegisterImpactEffects(const char *mapname);
FxImpactTable *__cdecl CG_RegisterImpactEffects_LoadObj(const char *mapname);
int __cdecl CG_RegisterImpactEffects_NonFlesh(
        const char **szEffectFile,
        const char *pszTypeName,
        const FxEffectDef **fx);
int __cdecl CG_RegisterImpactEffects_Flesh(
        const char **szEffectFile,
        const char *pszTypeName,
        const FxEffectDef **fx,
        char *defaultEffectName);
const char *__cdecl CG_FleshTypeToName(unsigned int fleshTypeId);
void __cdecl CG_RegisterImpactEffectsForDir(char *dir, EffectFile *effectFile, char *listbuf);
char *__cdecl CG_ParseImpactEffects(
        const char *filename,
        const char *buf,
        int iTypeCount,
        const char *const *pszTypeName,
        EffectFile *effectFile);
int __cdecl compare_impact_files(const char **pe0, const char **pe1);
FxImpactTable *__cdecl CG_RegisterImpactEffects_FastFile();
