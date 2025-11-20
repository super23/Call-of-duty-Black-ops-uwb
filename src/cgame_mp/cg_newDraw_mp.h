#pragma once

void __cdecl CG_AntiBurnInHUD_RegisterDvars();
bool __cdecl CG_ShouldDrawHud(int localClientNum);
double __cdecl CG_FadeHudMenu(int localClientNum, const dvar_s *fadeDvar, int displayStartTime, int duration);
bool __cdecl CG_CheckPlayerForLowAmmoSpecific(const cg_s *cgameGlob, unsigned int weapIndex);
bool __cdecl CG_CheckPlayerForLowAmmo(const cg_s *cgameGlob);
bool __cdecl CG_CheckPlayerForLowClipSpecific(const cg_s *cgameGlob, unsigned int weapIndex);
bool __cdecl CG_CheckPlayerForLowClip(const cg_s *cgameGlob);
double __cdecl CG_CalcPlayerHealth(const playerState_s *ps);
void __cdecl CG_ResetLowHealthOverlay(cg_s *cgameGlob);
void __cdecl CG_ResetDirectionalDamageIndicators(int localClientNum, int time);
int __cdecl CG_ServerMaterialName(int localClientNum, int index, char *materialName, unsigned int maxLen);
Material *__cdecl CG_ObjectiveIcon(int localClientNum, int icon, int type);
char *__cdecl CG_ScriptMainMenu(int localClientNum);
void __cdecl CG_OwnerDraw(
        int localClientNum,
        rectDef_s parentRect,
        float x,
        float y,
        float w,
        float h,
        int horzAlign,
        int vertAlign,
        int ownerDraw,
        int ownerDrawFlags,
        float *color,
        Material *material);
clientUIActive_t *__cdecl CL_GetLocalClientUIGlobals(int localClientNum);
void __cdecl CG_DrawPlayerAmmoBackdrop(
        int localClientNum,
        const rectDef_s *rect,
        const float *color,
        Material *material);
void __cdecl CG_DrawPlayerHeatValue(
        int localClientNum,
        const rectDef_s *rect,
        float *color,
        bool drawVehicleTurretVersion);
double __cdecl BG_PlayerWeaponHeat(const playerState_s *ps, unsigned int weaponIndex);
void __cdecl CG_DrawPlayerFuelAmmoValue(int localClientNum, const rectDef_s *rect, float *color);
void __cdecl CG_DrawPlayerReloadValue(int localClientNum, const rectDef_s *rect, float *color, Material *material);
void __cdecl CG_DrawPlayerReloadedValue(int localClientNum, const rectDef_s *rect, float *color, Material *material);
void __cdecl CG_DrawPlayerSprintBack(int localClientNum, const rectDef_s *rect, Material *material, float *color);
void __cdecl CG_DrawPlayerSprintMeter(int localClientNum, const rectDef_s *rect, Material *material, float *color);
void __cdecl CG_CalcPlayerSprintColor(const cg_s *cgameGlob, const playerState_s *ps, float *color);
void __cdecl CG_DrawPlayerBarHealth(int localClientNum, const rectDef_s *rect, Material *material, float *color);
void __cdecl CG_DrawPlayerBarHealthBack(int localClientNum, const rectDef_s *rect, Material *material, float *color);
void __cdecl CG_DrawPlayerLowHealthOverlay(int localClientNum, const rectDef_s *rect, Material *material, float *color);
double __cdecl CG_FadeLowHealthOverlay(const cg_s *cgameGlob);
void __cdecl CG_PulseLowHealthOverlay(cg_s *cgameGlob, float healthRatio);
void __cdecl CG_DrawPlayerDirectionalHitIndicator(
        int localClientNum,
        const rectDef_s *rect,
        Material *material,
        float *color);
void __cdecl draw_tank_body_component(
        int localClientNum,
        const rectDef_s *rect,
        Material *material,
        const float *color,
        e_tank_hud_component component);
void __cdecl get_tank_seat_tint_color(const centity_s *cent, unsigned int seat_index, float *rgba);
void __cdecl get_tank_hud_health_tint_color(float health_fraction, float *rgba, float black_when_dead);
void __cdecl draw_tank_turret_component(
        int localClientNum,
        const rectDef_s *rect,
        Material *material,
        const float *color,
        e_tank_hud_component component);
void __cdecl CG_DrawWarMomentumProgress(
        int localClientNum,
        const rectDef_s *rect,
        float *color,
        Material *material,
        int ownerDraw);
void __cdecl CG_DrawWarMomentumMultiplier(int localClientNum, const rectDef_s *rect, float *color, Material *material);
void __cdecl CG_DrawWarMomentumMultiplierDetail(
        int localClientNum,
        const rectDef_s *rect,
        float *color,
        Material *material);
void __cdecl CG_DrawWarMomentumMultiplierBlitzkrieg(
        int localClientNum,
        const rectDef_s *rect,
        float *color,
        Material *material);
void __cdecl CG_DrawLifeCounter(int localClientNum, rectDef_s *rect, float *color, bool alive);
void __cdecl CG_DrawWristWatch(
        int localClientNum,
        const rectDef_s *parentRect,
        const rectDef_s *rect,
        Material *material,
        float *color);
void __cdecl CG_DrawDemoControls(int localClientNum, const rectDef_s *rect, Material *material, float *color);
void  CG_DrawPlayerImageSwing(
        float a1@<ebp>,
        int localClientNum,
        const rectDef_s *rect,
        const rectDef_s *parentRect,
        Material *material,
        float *color);
void __cdecl CG_DrawPlayerTargetHighlights(int localClientNum, const rectDef_s *rect, Material *material, float *color);
char __cdecl ShouldDrawPlayerTargetHighlights(int localClientNum, const cg_s *cgameGlob);
void __cdecl CG_DrawPlayerTargetHighlightsFriendly(
        int localClientNum,
        const rectDef_s *rect,
        Material *material,
        float *color);
void __cdecl CG_DrawGuidedMissileFuel(int localClientNum, const rectDef_s *rect, Material *material);
void __cdecl CG_OwnerDrawText(
        int localClientNum,
        rectDef_s parentRect,
        float x,
        float y,
        float w,
        float h,
        int horzAlign,
        int vertAlign,
        float text_x,
        float text_y,
        int ownerDraw,
        int ownerDrawFlags,
        int align,
        Font_s *font,
        float scale,
        float *color,
        Material *material,
        int textStyle,
        int textAlignMode,
        itemDef_s *item,
        char *text);
void __cdecl CG_DrawPlayerAmmoValue(
        int localClientNum,
        const rectDef_s *rect,
        Font_s *font,
        float scale,
        float *color,
        Material *material,
        int textStyle);
void __cdecl CG_DrawPlayerWeaponName(
        int localClientNum,
        const rectDef_s *rect,
        Font_s *font,
        float scale,
        float *color,
        int textStyle);
void __cdecl CG_DrawPlayerWeaponNameBack(
        int localClientNum,
        const rectDef_s *rect,
        Font_s *font,
        float scale,
        const float *color,
        Material *material);
void  CG_DrawPlayerStance(
        int a1@<ebp>,
        int localClientNum,
        const rectDef_s *rect,
        const float *color,
        Font_s *font,
        float scale,
        int textStyle);
void __cdecl CG_DrawStanceIcon(
        int localClientNum,
        const rectDef_s *rect,
        float *drawColor,
        float x,
        float y,
        float fadeAlpha);
void __cdecl CG_DrawStanceHintPrints(
        int localClientNum,
        const rectDef_s *rect,
        float x,
        const float *color,
        float fadeAlpha,
        Font_s *font,
        float scale,
        int textStyle);
void  CG_DrawCursorhint(
        cg_s *a1@<ebp>,
        int localClientNum,
        const rectDef_s *rect,
        Font_s *font,
        float fontscale,
        float *color,
        int textStyle);
void __cdecl CG_UpdateCursorHints(cg_s *cgameGlob);
const char *__cdecl CG_GetWeaponUseString(int localClientNum, const char **secondaryString);
const char *__cdecl CG_GetUseString(int localClientNum);
void __cdecl CG_DrawHoldBreathHint(
        int localClientNum,
        const rectDef_s *rect,
        Font_s *font,
        float fontscale,
        int textStyle);
void __cdecl CG_DrawHoldBreathZoomHint(
        int localClientNum,
        const rectDef_s *rect,
        Font_s *font,
        float fontscale,
        int textStyle);
void __cdecl CG_DrawRCBombHints(
        int localClientNum,
        const rectDef_s *rect,
        Font_s *font,
        float fontscale,
        int textStyle,
        int hintType);
void __cdecl CG_DrawGuidedMissileBoostHint(
        int localClientNum,
        const rectDef_s *rect,
        Font_s *font,
        float fontscale,
        int textStyle);
void __cdecl CG_DrawGuidedMissileDetonateHint(
        int localClientNum,
        const rectDef_s *rect,
        Font_s *font,
        float fontscale,
        int textStyle);
void __cdecl CG_DrawGuidedMissileInfo(
        int localClientNum,
        const rectDef_s *rect,
        Font_s *font,
        float fontscale,
        int textStyle,
        eGuidedMissileInfo infoType);
double __cdecl CG_GetClosestTargetToGuidedMissile(int localClientNum);
double __cdecl CG_GetDistanceGuidedMissileAndOwner(int localClientNum);
void __cdecl CG_DrawTurretPlaceHint(
        int localClientNum,
        const rectDef_s *rect,
        Font_s *font,
        float fontscale,
        const float *color,
        int textStyle);
void __cdecl CG_DrawMantleHint(
        int localClientNum,
        const rectDef_s *rect,
        Font_s *font,
        float fontscale,
        const float *color,
        int textStyle);
void __cdecl CG_DrawInvalidCmdHint(
        int localClientNum,
        const rectDef_s *rect,
        Font_s *font,
        float fontscale,
        float *color,
        int textStyle);
void __cdecl CG_DrawTalkerNum(
        int localClientNum,
        int num,
        rectDef_s *rect,
        Font_s *font,
        float *color,
        float textScale,
        int style);
void __cdecl CG_DrawWarText(
        const cg_s *cgameGlob,
        rectDef_s *rect,
        Font_s *font,
        float fontscale,
        float *color,
        int textStyle,
        float text_x,
        float text_y);
