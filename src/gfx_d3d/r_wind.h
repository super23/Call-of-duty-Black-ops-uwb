#pragma once

void __cdecl R_InitWind();
void __cdecl R_SetWindDirection(float *dir, float speed);
void __cdecl R_SetVariantWindSpringAngle(unsigned int index, float angle);
void __cdecl R_SetVariantWindRustleAngle(unsigned int windIndex, float *angles);
void __cdecl R_WindStartBackend();
void __cdecl R_SetWindShaderConstants(GfxCmdBufSourceState *source);
void __cdecl RB_SetCustomWindConstants(GfxCmdBufSourceState *source, float *drawSurfListViewOrigin);
void __cdecl R_SetLocalWind(const float *position, float radius, const float *rustleAngle);
