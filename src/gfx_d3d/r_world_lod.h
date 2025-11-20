#pragma once

void __thiscall LodChainState::Init(LodChainState *this, const GfxWorldLodChain *lodChain, int localClientNum);
void __thiscall LodChainState::UpdateLevel(LodChainState *this, int newLevel, int localClientNum);
void __thiscall LodChainState::UpdateFade(LodChainState *this, float dt, int localClientNum);
void __cdecl SetFadeForSortedSurfaces(float val, int lodInfoIndex, int localClientNum);
void __cdecl R_WorldLod_Init();
void __cdecl R_WorldLod_FrameUpdate(float curTime, float *camPos, int localClientNum);
// local variable allocation has failed, the output may be wrong!
void  UpdateChain(int a1@<ebp>, int index, const float *inputCamPos, float dt, int localClientNum);
double __cdecl math::Abs(const math::Dir3 *_v);
