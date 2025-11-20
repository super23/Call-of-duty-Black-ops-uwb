#pragma once

void __cdecl GenerateLightGridBasisDirs();
void __cdecl GatherIncidentEnergyInSpaceForLightFromDir(
        const float *energy,
        const float *lightFromDir,
        float (*incidentEnergy)[3]);
void __cdecl AddLightGridLightingForDir(float (*lightingForDir)[3], GfxDecodedLightGridColors *colors);
char __cdecl EvaluateHeroLightForGrid(
        const GfxHeroLight *light,
        const float *xyz,
        float *dirToLight,
        float *attenuatedColor);
void __cdecl R_AdjustLightColorSamples(GfxDecodedLightGridColors *colors);
void __cdecl R_AddHeroOnlyLightsToGridColors(GfxDecodedLightGridColors *packed, const float *heroPos);
GfxWorld *R_InitHeroLights();
