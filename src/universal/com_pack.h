#pragma once

PackedUnitVec __cdecl Vec3PackUnitVec(const float *unitVec);
void __cdecl Vec3UnpackUnitVec(PackedUnitVec in, float *out);
int __cdecl Vec4PackQuat(const float *in);
int __cdecl FloatPack9S(float unitFloat);
int __cdecl FloatPack10S(float unitFloat);
PackedTexCoords __cdecl Vec2PackTexCoords(const float *in);
void __cdecl Vec2UnpackTexCoords(PackedTexCoords in, float *out);
