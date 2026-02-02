#pragma once
#include "rb_tess.h"
#include "r_state_utils.h"

void __cdecl R_PixStartNamedRenderTarget(unsigned __int8 renderTargetId);
void R_PixEndNamedRenderTarget();
void __cdecl R_SetTexFilter();
void __cdecl R_SetInitialContextState(IDirect3DDevice9 *device);
void __cdecl R_ChangeDepthHackNearClip(GfxCmdBufSourceState *source, float depthHackFlags);
void __cdecl R_DepthHackNearClipChanged(GfxCmdBufSourceState *source);
void __cdecl R_ChangeObjectPlacement(GfxCmdBufSourceState *source, const GfxScaledPlacement *placement);
void __cdecl R_ChangeObjectPlacement_Core(GfxCmdBufSourceState *source, const GfxScaledPlacement *placement);
void __cdecl R_ChangeObjectPlacementRemote(GfxCmdBufSourceState *source, const GfxScaledPlacement *remotePlacement);
GfxCmdBufSourceState *__cdecl R_GetCodeMatrix(
                GfxCmdBufSourceState *source,
                unsigned int sourceIndex,
                unsigned int firstRow);
void __cdecl R_DeriveCodeMatrix(GfxCmdBufSourceState *source, GfxCodeMatrices *activeMatrices, unsigned int baseIndex);
void __cdecl R_DeriveViewMatrix(GfxCmdBufSourceState *source);
// local variable allocation has failed, the output may be wrong!
void    R_DeriveWorldViewMatrix(GfxCmdBufSourceState *source);
void __cdecl R_DeriveProjectionMatrix(GfxCmdBufSourceState *source);
void __cdecl R_DeriveViewProjectionMatrix(GfxCmdBufSourceState *source);
// local variable allocation has failed, the output may be wrong!
void    R_DeriveWorldViewProjectionMatrix(GfxCmdBufSourceState *source);
void __cdecl R_DeriveShadowLookupMatrix(GfxCmdBufSourceState *source);
// local variable allocation has failed, the output may be wrong!
void    R_GenerateWorldOutdoorLookupMatrix(
                GfxCmdBufSourceState *source,
                float (*outMatrix)[4]);
const GfxImage *__cdecl R_GetTextureFromCode(
                GfxCmdBufSourceState *source,
                unsigned int codeTexture,
                unsigned __int8 *samplerState);
void __cdecl R_TextureFromCodeError(const GfxCmdBufContext context, unsigned int codeTexture);
const GfxImage *__cdecl R_OverrideGrayscaleImage(const dvar_s *dvar);
void __cdecl R_SetLightmap(GfxCmdBufContext context, unsigned int lmapIndex);
void __cdecl R_SetReflectionProbe(GfxCmdBufContext context, unsigned int reflectionProbeIndex);
void __cdecl R_ChangeDepthRange(GfxCmdBufState *state, GfxDepthRangeType depthRangeType);
void __cdecl R_HW_SetViewport(IDirect3DDevice9 *device, const GfxViewport *viewport, float nearValue, float farValue);
int __cdecl R_BeginMaterial(GfxCmdBufState *state, const Material *material, unsigned __int8 techType);
void __cdecl R_ChangeIndices(GfxCmdBufPrimState *state, IDirect3DIndexBuffer9 *ib);
void __cdecl R_ChangeStreamSource(
                GfxCmdBufPrimState *state,
                unsigned int streamIndex,
                IDirect3DVertexBuffer9 *vb,
                unsigned int vertexOffset,
                unsigned int vertexStride);
void __cdecl R_ClearAllStreamSources(GfxCmdBufPrimState *state);
void __cdecl R_DrawIndexedPrimitive(GfxCmdBufPrimState *state, const GfxDrawPrimArgs *args);
void __cdecl R_SetAlphaAntiAliasingState(IDirect3DDevice9 *device, __int16 stateBits0);
void __cdecl R_ChangeState_0(GfxCmdBufState *state, unsigned int stateBits0);
void __cdecl R_HW_SetAlphaTestEnable(IDirect3DDevice9 *device, __int16 stateBits0);
void __cdecl R_HW_SetColorMask(IDirect3DDevice9 *device, unsigned int stateBits0);
void __cdecl R_HW_SetCullFace(IDirect3DDevice9 *device, __int16 stateBits0);
void __cdecl R_HW_SetPolygonMode(IDirect3DDevice9 *device, signed int stateBits0);
void __cdecl R_HW_DisableBlend(IDirect3DDevice9 *device);
void __cdecl R_HW_SetBlend(
                IDirect3DDevice9 *device,
                bool blendWasEnabled,
                unsigned int changedBits,
                unsigned int stateBits0);
void __cdecl R_SetAlphaTestFunction(GfxCmdBufState *state, __int16 stateBits0);
void __cdecl R_ChangeState_1(GfxCmdBufState *state, unsigned int stateBits1);
void __cdecl R_HW_SetDepthWriteEnable(IDirect3DDevice9 *device, char stateBits1);
void __cdecl R_HW_SetDepthTestEnable(IDirect3DDevice9 *device, char stateBits1);
void __cdecl R_HW_SetDepthTestFunction(IDirect3DDevice9 *device, char stateBits1);
void __cdecl R_HW_EnableStencil(IDirect3DDevice9 *device);
void __cdecl R_HW_DisableStencil(IDirect3DDevice9 *device);
void __cdecl R_HW_SetFrontStencilOp(
                IDirect3DDevice9 *device,
                unsigned int stencilOpPass,
                unsigned int stencilOpFail,
                unsigned int stencilOpZFail);
void __cdecl R_HW_SetBackStencilOp(
                IDirect3DDevice9 *device,
                unsigned int stencilOpPass,
                unsigned int stencilOpFail,
                unsigned int stencilOpZFail);
void __cdecl R_HW_SetFrontStencilFunc(IDirect3DDevice9 *device, unsigned int stencilFunc);
void __cdecl R_HW_SetBackStencilFunc(IDirect3DDevice9 *device, unsigned int stencilFunc);
void __cdecl R_ForceSetPolygonOffset(IDirect3DDevice9 *device, char stateBits1);
void __cdecl R_HW_SetPolygonOffset(IDirect3DDevice9 *device, float scale, float bias);
void __cdecl R_SetTextureSamplerCodeImageRenderTarget(
                GfxCmdBufContext context,
                unsigned int samplerIndex,
                GfxCodeImageRenderTarget textureControl);
unsigned int __cdecl R_HW_SetSamplerState(
                IDirect3DDevice9 *device,
                unsigned int samplerIndex,
                unsigned int samplerState,
                unsigned int oldSamplerState);
void __cdecl R_SetSampler(
                GfxCmdBufContext context,
                unsigned int samplerIndex,
                unsigned __int8 samplerState,
                const GfxImage *image);
void __cdecl R_HW_SetSamplerTexture(IDirect3DDevice9 *device, unsigned int samplerIndex, const GfxTexture *texture);
void __cdecl R_SetSamplerState(GfxCmdBufState *state, unsigned int samplerIndex, unsigned __int8 samplerState);
void __cdecl R_TextureOverride(
                const GfxBackEndData *data,
                GfxCmdBufContext context,
                char modelIndex,
                int textureOverride);
void __cdecl R_GetViewport(GfxCmdBufSourceState *source, GfxViewport *outViewport);
void __cdecl R_SetViewport(GfxCmdBufState *state, const GfxViewport *viewport);
void __cdecl R_SetViewportStruct(GfxCmdBufSourceState *source, const GfxViewport *viewport);
void __cdecl R_SetScissorStruct(GfxCmdBufSourceState *source, const GfxViewport *scissor);
void __cdecl R_ClearScissorStruct(GfxCmdBufSourceState *source);
char __cdecl R_GetScissor(GfxCmdBufSourceState *source, GfxViewport *outScissor);
void __cdecl R_SetScissor(GfxCmdBufState *state, const GfxViewport *_scissor);
void __cdecl R_ClearScissor(GfxCmdBufState *state);
void __cdecl R_SetViewportValues(GfxCmdBufSourceState *source, int x, int y, int width, int height);
void __cdecl R_UpdateViewport(GfxCmdBufSourceState *source, GfxViewport *viewport);
void __cdecl R_DisableSampler(GfxCmdBufState *state, unsigned int samplerIndex);
void __cdecl R_HW_DisableSampler(IDirect3DDevice9 *device, unsigned int samplerIndex);
void __cdecl UpdateVPosToWorld(GfxCmdBufSourceState *source);
void __cdecl R_SetRenderTargetSize(GfxCmdBufSourceState *source, unsigned __int8 newTargetId);
GfxViewportBehavior __cdecl R_ViewportBehaviorForRenderTarget(unsigned __int8 renderTargetId);
void __cdecl R_SetRenderTarget(GfxCmdBufContext context, unsigned __int8 newTargetId);
void __cdecl R_HW_SetRenderTarget(GfxCmdBufState *state, unsigned __int8 newTargetId);
void __cdecl R_UpdateStatsTarget(GfxCmdBufContext context);
void __cdecl R_UnbindImage(GfxCmdBufState *state, const GfxImage *image);
void __cdecl R_ClearRenderTargetForMultiGpu(GfxCmdBufContext context, unsigned __int8 targetId);
void __cdecl R_ClearScreenInternal(
                IDirect3DDevice9 *device,
                unsigned __int8 whichToClear,
                const float *color,
                float depth,
                unsigned __int8 stencil,
                const GfxViewport *viewport);
void __cdecl Byte4PackPixelColor(const float *from, unsigned __int8 *to);
void __cdecl R_ClearScreen(
                IDirect3DDevice9 *device,
                unsigned __int8 whichToClear,
                const float *color,
                float depth,
                unsigned __int8 stencil,
                const GfxViewport *viewport);
void __cdecl R_SetMeshStream(GfxCmdBufState *state, GfxMeshData *mesh);
void __cdecl R_SetCompleteState(IDirect3DDevice9 *device, unsigned int *stateBits);
void __cdecl R_ForceSetBlendState(IDirect3DDevice9 *device, unsigned int stateBits0);
void __cdecl R_ForceSetStencilState(IDirect3DDevice9 *device, unsigned int stateBits1);
void __cdecl R_Set_Texture_SeeThruDecal(GfxCmdBufSourceState *source);
// bad sp value at call has been detected, the output may be wrong!
void    R_DrawCall(
                void (__cdecl *callback)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *),
                const void *userData,
                GfxCmdBufSourceState *source,
                const GfxViewInfo *viewInfo,
                GfxDrawSurfListInfo *info,
                const GfxViewParms *viewParms,
                GfxCmdBuf *cmdBufEA,
                GfxCmdBuf *prepassCmdBufEA);

inline void R_DrawCall(
    void(__cdecl *callback)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *),
    const void *userData,
    GfxCmdBufSourceState *source,
    const GfxViewInfo *viewInfo,
    const GfxDrawSurfListInfo *info, // inline version to de-const this arg :(
    const GfxViewParms *viewParms,
    GfxCmdBuf *cmdBufEA,
    GfxCmdBuf *prepassCmdBufEA)
{
    R_DrawCall(callback, userData, source, viewInfo, (GfxDrawSurfListInfo *)info, viewParms, cmdBufEA, prepassCmdBufEA); 
}

static const MaterialUpdateFrequency s_codeSamplerUpdateFreq[43] =
{
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_CUSTOM,
  MTL_UPDATE_CUSTOM,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_CUSTOM,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_CUSTOM,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_RARELY
};


static const MaterialUpdateFrequency s_codeConstUpdateFreq[] =
{
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_PRIM,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_PRIM,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_PER_PRIM,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_PER_PRIM,
  MTL_UPDATE_PER_PRIM,
  MTL_UPDATE_PER_PRIM,
  MTL_UPDATE_PER_PRIM,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_PRIM,
  MTL_UPDATE_PER_PRIM,
  MTL_UPDATE_PER_PRIM,
  MTL_UPDATE_PER_PRIM,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_PRIM,
  MTL_UPDATE_PER_PRIM,
  MTL_UPDATE_PER_PRIM,
  MTL_UPDATE_PER_PRIM,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_PRIM,
  MTL_UPDATE_PER_PRIM,
  MTL_UPDATE_PER_PRIM,
  MTL_UPDATE_PER_PRIM,
  MTL_UPDATE_PER_PRIM,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_CUSTOM,
  MTL_UPDATE_CUSTOM,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_CUSTOM,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_CUSTOM,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_RARELY,
  MTL_UPDATE_PER_OBJECT,
  MTL_UPDATE_RARELY
};