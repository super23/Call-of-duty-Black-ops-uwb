#pragma once

#ifdef KISAK_SP

struct StringTable;

// Decomp: CoDSP_rdBlackOps.map.c / Unlinker-verified retail zones — SP+zombie still reference mp/*.csv paths
// but assets may live in common_zombie.ff; try retail path then unprefixed before ERR_DROP.
// Info tables: code_post_gfx.ff has info/ai_lochit_dmgtable + info/bullet_penetration_sp (not mp_ variants).
bool __cdecl BG_SP_TryGetStringTableAsset(
    const char *retailMpPath,
    const char *fallbackPath,
    const StringTable **outTable);

char *__cdecl BG_SP_LoadRawTextFileFirst(const char *retailMpPath, const char *fallbackPath);

// Decomp: CoDSP_rdBlackOps.map.c — SP info/*.txt tables use unprefixed paths; Zombies may ship mp_ variants in common_zombie.ff.
bool __cdecl BG_SP_TryLoadInfoStringFirst(
    const char *spPath,
    const char *mpPath,
    const char *ident,
    char *loadBuffer,
    char **outParsed);

// Decomp: CoDSP_rdBlackOps.map.c — minimal player anim types when mp/playeranimtypes.txt absent from zone.
void __cdecl BG_SP_LoadPlayerAnimTypesWithFallback();

#endif
