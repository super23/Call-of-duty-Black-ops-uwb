#pragma once

void __cdecl BG_Wind_RegisterDvars();
void __cdecl DisplayWindDebug();
void __cdecl SeedVariantWind(
        VariantWind *wind,
        int i,
        int start_time,
        const float *seedWindVector,
        float windLeafScale);
void __cdecl BG_SeedVarintWinds(unsigned int variant_seed, int start_time);
void  BG_UpdateVariantWind(
        int a1@<esi>,
        VariantWind *wind,
        int variantIndex,
        const float *seedWindVector,
        int cur_time);
void  CG_UpdateWind(int a1@<ebp>, int a2@<esi>, int cur_time);
void __cdecl BG_GetVariantWindVector(int which, float *out);
void __cdecl BG_GetGlobalWind(float *out);
void __cdecl CG_EnableLocalWindSource(int entnum);
double __cdecl CG_GetLocalWindSourceScale(int localClientNum, float *pos);
