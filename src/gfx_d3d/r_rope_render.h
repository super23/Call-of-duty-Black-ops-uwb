#pragma once

void __cdecl R_Rope_GenerateVerts_Camera(RopeGenerateVertsCmd *cmd);
void __cdecl R_Rope_GenerateVerts_Internal(
        const RopeGenerateVertsCmd *cmd,
        char sceneIndex,
        unsigned int drawsurfRegion,
        bool forceLowResLOD);
void __cdecl R_Rope_ClearAll();
void __cdecl R_Rope_Add(const VisualRope *vropeIn);
RopeRenderInfo *__cdecl R_Rope_GetInfo();
unsigned __int8 __cdecl R_Rope_CalcVisibility(const float *minmax, unsigned int localClientNum);
const Material *__cdecl R_Rope_GetMaterialFromGfxWorld();
