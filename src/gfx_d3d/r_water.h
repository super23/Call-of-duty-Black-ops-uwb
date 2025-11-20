#pragma once

void __cdecl R_UploadWaterTextureInternal(water_t **data);
void __cdecl WaterFrequenciesAtTime(complex_s *H, const water_t *water, float t);
void __cdecl WaterAmplitudesFromFrequencies(complex_s *H, const water_t *water);
void __cdecl TransposeArray(complex_s *H, unsigned int M);
void __cdecl WaterPixelsFromAmplitudes(GfxColor *pixels, complex_s *H, const water_t *water);
void __cdecl GenerateMipMaps(_D3DFORMAT format, unsigned __int8 *pixels, water_t *water);
void __cdecl R_UploadWaterTexture(water_t *water, float floatTime);
void __cdecl R_InitWater();
void __cdecl Load_PicmipWater(water_t **waterRef);
