#pragma once

void R_RegisterSkyboxModel();
void __cdecl R_RegisterSkyboxModel(char *xmodelName);
void __cdecl R_ClearSkyboxModel();
void __cdecl R_AddSkyboxModel(const float *eyePos);
XModel *__cdecl R_MapGetSkyboxModel();
