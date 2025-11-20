#pragma once

void __cdecl R_RegisterOutdoorImage(GfxWorld *world, const float *outdoorMin, const float *outdoorMax);
void __cdecl Outdoor_ApplyBoundingBox(const float *outdoorMin, const float *outdoorMax);
int Outdoor_UpdateTransforms();
void __cdecl Outdoor_SetRendererOutdoorLookupMatrix(GfxWorld *world);
void __cdecl R_GenerateOutdoorImage(GfxImage *outdoorImage);
void Outdoor_TempHunkFreePic();
void Outdoor_ComputeTexels();
double __cdecl Outdoor_TraceHeightInWorld(float worldX, float worldY);
void __cdecl Outdoor_WriteToPic(float zWorld, unsigned __int8 *outByte);
int __cdecl Outdoor_TransformToTextureClamped(int dimension, float inWorld);
