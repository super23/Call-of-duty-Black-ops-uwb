#include "r_draw_sunshadow.h"
#include "r_state_utils.h"
#include "r_wind.h"
#include "r_state.h"
#include "r_foliage.h"
#include "r_dvars.h"
#include "rb_draw3d.h"
#include "rb_spotshadow.h"

void    R_DrawSunShadowMap(
                const GfxViewInfo *viewInfo,
                unsigned int partitionIndex,
                GfxCmdBuf *cmdBuf)
{
    void *v4; // esp
    float x; // [esp+20h] [ebp-1ABCh]
    const GfxBackEndData *data; // [esp+2Ch] [ebp-1AB0h]
    GfxCmdBufSourceState state; // [esp+30h] [ebp-1AACh] BYREF

    R_InitCmdBufSourceState(&state, &viewInfo->input, 0);
    R_SetWindShaderConstants(&state);
    R_SetRenderTargetSize(&state, 14u);

    data = viewInfo->input.data;
    x = data->sunShadow.partition[partitionIndex].shadowViewParms.projectionMatrix.m[2][2];

    R_UpdateCodeConstant(
        &state,
        CONST_SRC_CODE_SHADOWMAP_POLYGON_OFFSET,
        (sm_polygonOffsetBias->current.value * 0.25) * x,
        sm_polygonOffsetScale->current.value,
        0.0,
        0.0);
    R_SetViewportValues(&state, 0, partitionIndex * dx.sunShadowmapSize, dx.sunShadowmapSize, dx.sunShadowmapSize);
    R_DrawCall(
        (void(__cdecl *)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *))R_DrawSunShadowMapCallback, 
        &data->sunShadow.partition[partitionIndex],
        &state, 
        viewInfo, 
        (GfxDrawSurfListInfo*)&viewInfo->drawList[partitionIndex + 8], // de-const cast
        &data->sunShadow.partition[partitionIndex].shadowViewParms,
        cmdBuf, 
        0);
}

void __cdecl R_DrawSunShadowMapCallback(const GfxSunShadowPartition **userData, GfxCmdBufContext context)
{
    const GfxDrawSurfListInfo *drawList; // [esp+34h] [ebp-8h]
    const GfxSunShadowPartition *partition; // [esp+38h] [ebp-4h]

    partition = *userData;
    drawList = (const GfxDrawSurfListInfo *)userData[1];
    R_SetRenderTarget(context, 0xEu);
    if ( !partition->partitionIndex )
        R_ClearScreen(context.state->prim.device, 3u, shadowmapClearColor, 1.0, 0, 0);
    R_HW_EnableScissor(
        context.state->prim.device,
        partition->viewport.x,
        partition->viewport.y + partition->partitionIndex * (int)dx.singleSampleDepthStencilSurface,
        partition->viewport.width,
        partition->viewport.height);
    R_DrawSurfs(context, 0, drawList);
    R_HW_DisableScissor(context.state->prim.device);
}

