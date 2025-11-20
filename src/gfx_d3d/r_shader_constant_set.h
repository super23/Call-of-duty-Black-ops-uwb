#pragma once

void __cdecl R_InitShaderConstantSet(ShaderConstantSet *scs);
bool __cdecl R_ShaderConstantSetIsUsed(const ShaderConstantSet *scs);
char __cdecl R_MapShaderConstantSet(ShaderConstantSet *scs, unsigned int index, const char *constantName);
char __cdecl R_UnmapShaderConstantSet(ShaderConstantSet *scs, unsigned int index);
int __cdecl R_GetConstantIndex(const ShaderConstantSet *scs, const char *constantName);
int __cdecl R_GetFreeConstantIndex(const ShaderConstantSet *scs);
char __cdecl R_SetShaderConstantSetValue(ShaderConstantSet *scs, unsigned int index, float *v);
char __cdecl R_SetShaderConstantSetValue(
        ShaderConstantSet *scs,
        unsigned int index,
        float x,
        float y,
        float z,
        float w);
int __cdecl RB_ApplyShaderConstantSet(GfxCmdBufSourceState *gfxSourceState, const ShaderConstantSet *scs);
const ShaderConstantSet *__cdecl RB_ShaderConstantSetFromDrawSurf(
        const GfxBackEndData *backEndData,
        const GfxDrawSurf *drawSurf);
unsigned int __cdecl XModelDrawSurf_DecodeIndex(const GfxDrawSurf *drawSurf);
void __cdecl RB_ApplyShaderConstantSet(GfxCmdBufSourceState *gfxSourceState, const GfxDrawSurf *drawSurf);
bool __cdecl RB_DrawSurf_Uses_ShaderConstantSet(const GfxDrawSurf *drawSurf);
void __cdecl R_XModelDrawSurfEncodeShaderConstantSet(GfxDrawSurf *drawSurf, unsigned int constSetIndex);
void __cdecl XModelDrawSurf_EncodeIndex(GfxDrawSurf *drawSurf, char scsIndex);
bool __cdecl R_ShaderConstantSet_Sets_Charred(const ShaderConstantSet *scs);
int __cdecl R_ShaderConstantSet_CopyToFrontEndDataOut(const ShaderConstantSet *scs);
int __cdecl CopyShaderConstantSetToBackendData(const ShaderConstantSet *scs);
const ShaderConstantSet *__cdecl R_ShaderConstantSet_FromFrontEndDataOut(unsigned int constSetindex);
void __cdecl RB_SaveCurrentShaderConstantSetValues(
        ShaderConstantSet *destSet,
        GfxCmdBufSourceState *gfxSourceState,
        const ShaderConstantSet *srcSet);
ScopedShaderConstantSetUndo *__thiscall ScopedShaderConstantSetUndo::ScopedShaderConstantSetUndo(
        ScopedShaderConstantSetUndo *this,
        GfxCmdBufSourceState *sourceState,
        const ShaderConstantSet *cscEA);
void __thiscall ScopedShaderConstantSetUndo::~ScopedShaderConstantSetUndo(ScopedShaderConstantSetUndo *this);
void __cdecl R_ShaderConstantShowDebug(
        const float *eyePos,
        const float *objOrigin,
        float objRad,
        const ShaderConstantSet *scs);
