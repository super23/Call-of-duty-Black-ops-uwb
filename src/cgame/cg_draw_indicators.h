#pragma once

void __cdecl CG_DrawFlashDamage(const cg_s *cgameGlob);
void __cdecl CG_DrawDamageDirectionIndicators(int localClientNum);
void __cdecl CG_ClearHudGrenades();
double __cdecl CG_AddHudGrenade_PositionCheck(cg_s *cgameGlob, const centity_s *grenadeEnt, const WeaponDef *weapDef);
void __cdecl CG_GrenadeIndicator_GetPlayerPosition(const cg_s *cgameGlob, float *playerOrigin);
void __cdecl CG_AddHudGrenade(cg_s *cgameGlob, const centity_s *grenadeEnt);
void __cdecl CG_DrawGrenadeIndicators(int localClientNum);
void __cdecl CG_DrawWarningPointer(
        int localClientNum,
        float centerX,
        float centerY,
        const float *grenadeOffset,
        const float *color,
        float radiusOffset);
void __cdecl CG_DrawWarningIcon(
        int localClientNum,
        float centerX,
        float centerY,
        const float *grenadeOffset,
        const float *color,
        Material *material,
        float radiusOffset);
