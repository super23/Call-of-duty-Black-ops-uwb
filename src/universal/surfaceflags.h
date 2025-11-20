#pragma once

int __cdecl Com_SurfaceTypeFromName(const char *name);
const char *__cdecl Com_SurfaceTypeToName(int iTypeIndex);
bool __cdecl Com_SurfaceBurns(int iTypeIndex);
bool __cdecl Com_SurfaceFloats(int iTypeIndex);
double __cdecl Com_SurfaceDensity(int iTypeIndex);
double __cdecl Com_SurfaceFrictionScale(int iTypeIndex);
double __cdecl Com_SurfaceBounceScale(int iTypeIndex);
