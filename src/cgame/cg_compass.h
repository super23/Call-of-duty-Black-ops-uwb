#pragma once

void __cdecl CG_CompassRegisterDvars();
char __cdecl CG_IsSelectingLocation(int localClientNum);
bool __cdecl CG_WorldPosToCompass(
        CompassType compassType,
        const cg_s *cgameGlob,
        const rectDef_s *mapRect,
        const float *north,
        const float *playerWorldPos,
        const float *in,
        float *out,
        float *outClipped);
char __cdecl CG_TextClippedToCompass(
        CompassType compassType,
        const cg_s *cgameGlob,
        const rectDef_s *mapRect,
        const float *north,
        const float *playerWorldPos,
        const float *in,
        float w,
        float h);
void __cdecl CG_CompassCalcDimensions(
        CompassType compassType,
        const cg_s *cgameGlob,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        float *x,
        float *y,
        float *w,
        float *h);
double __cdecl CG_FadeCompass(int localClientNum, int displayStartTime, CompassType compassType);
double __cdecl CG_FadeCompassIcons(int localClientNum, bool ignoreGlobalScrambler);
void __cdecl CG_CompassDrawPlayerBack(
        int localClientNum,
        CompassType compassType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        Material *material,
        float *color);
void __cdecl CG_DrawPlayerPopUps(
        int localClientNum,
        const rectDef_s *rect,
        Font_s *font,
        float scale,
        float *color,
        Material *material,
        int textStyle,
        itemDef_s *item,
        char *text,
        bool doSwing);
void __cdecl CG_DrawPlayerCOD7TypeWriter(
        int localClientNum,
        const rectDef_s *rect,
        Font_s *font,
        float scale,
        float *color,
        Material *material,
        int textStyle,
        itemDef_s *item,
        char *text);
void __cdecl CG_DrawPlayerRedationText(
        int localClientNum,
        const rectDef_s *rect,
        Font_s *font,
        float scale,
        float *color,
        Material *material,
        int textStyle,
        itemDef_s *item,
        char *text);
void __cdecl CG_DrawPlayerPopInText(
        int localClientNum,
        const rectDef_s *rect,
        Font_s *font,
        float scale,
        float *color,
        Material *material,
        int textStyle,
        itemDef_s *item,
        char *text);
void __cdecl CG_CompassDrawPlayerNorthCoord(
        int localClientNum,
        CompassType compassType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        Font_s *font,
        Material *material,
        float *color,
        int textStyle);
void __cdecl CG_CompassDrawPlayerEastCoord(
        int localClientNum,
        CompassType compassType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        Font_s *font,
        Material *material,
        float *color,
        int textStyle);
void __cdecl CG_CompassDrawPlayerNCoordScroll(
        int localClientNum,
        CompassType compassType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        Font_s *font,
        Material *material,
        float *color,
        int textStyle);
void __cdecl CG_CompassDrawPlayerECoordScroll(
        int localClientNum,
        CompassType compassType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        Font_s *font,
        Material *material,
        float *color,
        int textStyle);
void __cdecl CG_CompassDrawStatic(
        int localClientNum,
        CompassType compassType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        Material *material,
        float *color);
void __cdecl CG_UpdateEnemyScramblerAlpha(int localClientNum);
double __cdecl CG_GetScramblerEnemyAlpha(int localClientNum);
void __cdecl CG_CompassDrawScrambler(
        int localClientNum,
        CompassType compassType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        Material *material,
        float *color);
void __cdecl CG_CompassDrawRound(int localClientNum, const rectDef_s *rect, Material *material, float *color);
void __cdecl CG_CompassDrawPlayerMap(
        int localClientNum,
        CompassType compassType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        Material *material,
        float *color,
        bool grid);
void __cdecl CG_CompassDrawPlayerMapLocationSelector(
        int localClientNum,
        CompassType compassType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        Material *material,
        float *color);
void __cdecl CG_CompassDrawPlayerSelectedLocations(
        int localClientNum,
        CompassType compassType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        Material *material,
        float *color);
void __cdecl CG_CompassDrawPlayer(
        int localClientNum,
        jpeg_decompress_struct *compassType,
        const rectDef_s *parentRect,
        rectDef_s *rect,
        Material *material,
        float *color);
void __cdecl CG_CompassDrawBorder(
        int localClientNum,
        CompassType compassType,
        const rectDef_s *parentRect,
        rectDef_s *rect,
        Material *material,
        float *color);
void __cdecl CG_CompassUpYawVector(const cg_s *cgameGlob, float *result);
void  CG_CompassDrawTickertape(
        float a1@<ebp>,
        int localClientNum,
        CompassType compassType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        Material *material,
        const float *color,
        Font_s *textFont,
        float textScale,
        int textStyle,
        bool drawObjectives);
void __cdecl CalcCompassPointerSize(CompassType compassType, float *w, float *h);
void __cdecl DrawIconDistanceText(
        int localClientNum,
        float distance,
        float iconX,
        float iconY,
        float iconH,
        const rectDef_s *rect,
        const float *color,
        Font_s *textFont,
        float textScale,
        int textStyle);
double __cdecl CutFloat(float original);
void __cdecl CG_SetGridTable();
void __cdecl CG_ScaleCompass();
void __cdecl CG_SetGridOffsets(
        const float *east,
        const float *south,
        float *eastOffsetLetter,
        float *southOffsetLetter);
void __cdecl CG_CompassDrawGridPoints(
        int localClientNum,
        CompassType compassType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        const float *color,
        Font_s *textFont,
        float textScale,
        int textStyle);
void __cdecl CG_GetGridFromPos(int localClientNum, float *pos, char *gridName);
float __cdecl CG_GetHudAlphaCompass(int localClientNum);
void __cdecl CalcCompassFriendlySize(CompassType compassType, float *w, float *h);
void __cdecl CG_CompassDrawPlayerPointers_MiniMap(
        int localClientNum,
        CompassType compassType,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        Material *material,
        const float *color);
double __cdecl GetObjectiveFade(const rectDef_s *clipRect, float x, float y, float width, float height);
char __cdecl CalcCompassPointerSizeObjective(
        CompassType compassType,
        float *w,
        float *h,
        const objective_t *objective,
        const rectDef_s *mapRect);
bool __cdecl CG_IsShowingZombieMap();
