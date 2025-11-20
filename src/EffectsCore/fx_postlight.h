#pragma once

void __cdecl FX_PostLight_GenerateVerts(FxPostLightInfo *postLightInfoAddr, FxSystem *system);
void __cdecl FX_PostLight_Begin();
void __cdecl FX_PostLight_Add(FxPostLight *postLight);
FxPostLightInfo *__cdecl FX_PostLight_GetInfo();
