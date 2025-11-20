#include "r_adszscale.h"

void __cdecl R_StartADSZScaleFrame()
{
  ++activeAdsZScale;
}

void __cdecl R_SetADSZScale(unsigned int viewport, float pos)
{
  adsZScale[viewport][(activeAdsZScale + 1) % 2] = pos;
}

void __cdecl R_SetADSZScaleConstants(unsigned int viewport, GfxCmdBufSourceState *source)
{
  source->input.consts[172][0] = adsZScale[viewport][activeAdsZScale % 2];
  source->input.consts[172][1] = *(float *)&FLOAT_0_0;
  source->input.consts[172][2] = *(float *)&FLOAT_0_0;
  source->input.consts[172][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(source, 0xACu);
}

