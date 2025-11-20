#pragma once

void __cdecl R_GetPixelLiteralConsts(
        const Material *mtl,
        const MaterialPass *pass,
        GfxShaderConstantBlock *pixelLiteralConsts);
void __cdecl R_RegisterShaderConst(unsigned int dest, const float *value, GfxShaderConstantBlock *consts);
int __cdecl R_ComparePixelConsts(const Material **material, const MaterialPass **pass);
