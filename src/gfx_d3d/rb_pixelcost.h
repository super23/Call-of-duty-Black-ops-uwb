#pragma once

const Material *__cdecl R_PixelCost_GetAccumulationMaterial(const Material *material);
void __cdecl R_PixelCost_BeginSurface(GfxCmdBufContext context);
void __cdecl R_PixelCost_SetConstant(GfxCmdBufSourceState *source, int cost);
int __cdecl RB_PixelCost_GetCostForRecordIndex(int recordIndex);
unsigned __int64 __cdecl R_PixelCost_PackedKeyForMaterial(const Material *material, unsigned __int8 techType);
bool __cdecl RB_PixelCost_DoesPrimMatch(unsigned __int64 packedKey);
void __cdecl RB_PixelCost_ResetPrim(unsigned __int64 packedKey);
void __cdecl R_PixelCost_EndSurface(GfxCmdBufContext context);
int RB_PixelCost_AccumulateMsec();
unsigned __int8 __cdecl RB_PixelCost_OverrideRenderTarget(unsigned __int8 targetId);
