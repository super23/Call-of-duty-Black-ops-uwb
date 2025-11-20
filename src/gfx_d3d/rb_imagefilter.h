#pragma once

void __cdecl RB_GaussianFilterImage(float radius, unsigned __int8 srcRenderTargetId, unsigned __int8 dstRenderTargetId);
void __cdecl RB_VirtualToSceneRadius(float radius, float *radiusX, float *radiusY);
int __cdecl RB_GenerateGaussianFilterChain(
        float radiusX,
        float radiusY,
        int srcWidth,
        int srcHeight,
        int dstWidth,
        int dstHeight,
        int passLimit,
        GfxImageFilterPass *filterPass);
void __cdecl RB_GenerateGaussianFilter1D(float radius, int *res, int axis, GfxImageFilterPass *filterPass);
int __cdecl RB_PickSymmetricFilterMaterial(int halfTapCount, const Material **material);
int __cdecl RB_GaussianFilterPoints1D(
        float pixels,
        int srcRes,
        int dstRes,
        int tapLimit,
        float *tapOffsets,
        float *tapWeights);
void __cdecl RB_GenerateGaussianFilter2D(
        float radius,
        int srcWidth,
        int srcHeight,
        int dstWidth,
        int dstHeight,
        GfxImageFilterPass *filterPass);
void __cdecl RB_FilterImage(GfxImageFilter *filter);
void __cdecl RB_SetupFilterPass(const GfxImageFilterPass *filterPass);
void __cdecl RB_FilterPingPong(const GfxImageFilter *filter, int passIndex);
