#include "rb_spotshadow.h"
#include "r_state.h"
#include "rb_backend.h"
#include "rb_pixelcost.h"
#include "r_cmdbuf.h"
#include "r_dvars.h"
#include "r_foliage.h"
#include "r_state_utils.h"
#include "r_wind.h"
#include "rb_draw3d.h"
#include "rb_shade.h"
#include "rb_sunshadow.h"

void __cdecl R_DrawSpotShadowMapCallback(const GfxSpotShadow **userData, GfxCmdBufContext context)
{
    const GfxDrawSurfListInfo *drawList; // [esp+Ch] [ebp-8h]
    const GfxSpotShadow *spotShadow; // [esp+10h] [ebp-4h]

    spotShadow = *userData;
    drawList = (const GfxDrawSurfListInfo *)userData[1];
    R_SetRenderTarget(context, (*userData)->renderTargetId);
    if ( spotShadow->clearScreen )
        R_ClearScreen(context.state->prim.device, 3u, shadowmapClearColor, 1.0, 0, 0);
    if ( !gfxMetrics.hasHardwareShadowmap )
        R_DrawQuadMesh(context, rgp.shadowClearMaterial, spotShadow->clearMesh);
    R_DrawSurfs(context, 0, drawList);
}

void __cdecl RB_SpotShadowMaps(const GfxBackEndData *data, const GfxViewInfo *viewInfo)
{
    GfxCmdBuf cmdBuf; // [esp+1Ch] [ebp-4h] BYREF
    int savedregs; // [esp+20h] [ebp+0h] BYREF

    //PIXBeginNamedEvent(-1, "RB_SpotShadowMaps");
    if ( pixelCostMode )
    {
        //if ( GetCurrentThreadId() == g_DXDeviceThread )
            //D3DPERF_EndEvent();
    }
    else
    {
        if ( !data
            && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_spotshadow.cpp", 302, 0, "%s", "data") )
        {
            __debugbreak();
        }
        R_InitContext(data, &cmdBuf);
        //PIXBeginNamedEvent(-1, "Spot Shadow Maps");
        R_DrawSpotShadowMapArray(viewInfo, &cmdBuf);
        //if ( GetCurrentThreadId() == g_DXDeviceThread )
            //D3DPERF_EndEvent();
        //if ( GetCurrentThreadId() == g_DXDeviceThread )
            //D3DPERF_EndEvent();
    }
}

void    R_DrawSpotShadowMapArray(const GfxViewInfo *viewInfo, GfxCmdBuf *cmdBuf)
{
    void *v3; // esp
    unsigned int v4[2]; // [esp+18h] [ebp-1AC4h] BYREF
    const GfxDrawSurfListInfo *v5; // [esp+20h] [ebp-1ABCh]
    unsigned int i; // [esp+28h] [ebp-1AB4h]
    const GfxBackEndData *data; // [esp+2Ch] [ebp-1AB0h]
    GfxCmdBufSourceState state; // [esp+30h] [ebp-1AACh] BYREF

    //R_InitCmdBufSourceState(&state, &viewInfo->input, 0);
    //R_SetWindShaderConstants(&state);
    //data = viewInfo->input.data;
    //for ( i = 0; i < data->spotShadowCount; ++i )
    //{
    //    v6 = &data->spotShadows[i];
    //    v5 = &viewInfo->drawList[i + 10];
    //    R_SetRenderTargetSize(&state, data->spotShadows[i].renderTargetId);
    //    R_UpdateCodeConstant(
    //        &state,
    //        0x14u,
    //        sm_polygonOffsetBias->current.value * 0.25,
    //        sm_polygonOffsetScale->current.value,
    //        0.0,
    //        0.0);
    //    R_SetViewportValues(&state, v6->viewport.x, v6->viewport.y, v6->viewport.width, v6->viewport.height);
    //    v4[0] = v6;
    //    v4[1] = v5;
    //    R_DrawCall(
    //        (void (__cdecl *)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *))R_DrawSpotShadowMapCallback,
    //        v4,
    //        &state,
    //        viewInfo,
    //        v5,
    //        &v6->shadowViewParms,
    //        cmdBuf,
    //        0);
    //}

    R_InitCmdBufSourceState(&state, &viewInfo->input, 0);
    R_SetWindShaderConstants(&state);

    for (int i = 0; i < viewInfo->input.data->spotShadowCount; i++)
    {
        const GfxSpotShadow *spotShadow = &viewInfo->input.data->spotShadows[i];

        R_SetRenderTargetSize(&state, spotShadow->renderTargetId);
        R_UpdateCodeConstant(
            &state,
            CONST_SRC_CODE_SHADOWMAP_POLYGON_OFFSET,
            sm_polygonOffsetBias->current.value * 0.25,
            sm_polygonOffsetScale->current.value,
            0.0,
            0.0);
        R_SetViewportValues(&state, spotShadow->viewport.x, spotShadow->viewport.y, spotShadow->viewport.width, spotShadow->viewport.height);

        R_DrawCall(
            (void(__cdecl *)(const void *, GfxCmdBufSourceState *, GfxCmdBufState *, GfxCmdBufSourceState *, GfxCmdBufState *))R_DrawSpotShadowMapCallback,
            spotShadow,
            &state,
            viewInfo,
            &viewInfo->drawList[i + 10],
            &spotShadow->shadowViewParms,
            cmdBuf,
            0);
    }


}

void __cdecl RB_DrawSpotShadowOverlay()
{
    float x; // [esp+0h] [ebp-6Ch]
    float t0; // [esp+40h] [ebp-2Ch]
    unsigned int spotShadowIndex; // [esp+60h] [ebp-Ch]
    float h; // [esp+64h] [ebp-8h]

    if (!backEndData->viewInfoCount
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_spotshadow.cpp",
            375,
            0,
            "%s",
            "backEndData->viewInfoCount > 0"))
    {
        __debugbreak();
    }
    if (backEndData->spotShadowCount)
    {
        h = (double)vidConfig.displayHeight * 0.25;
        gfxCmdBufSourceState.input.codeImageSamplerStates[8] = 97;
        R_SetCodeImageTexture(&gfxCmdBufSourceState, 8u, gfxRenderTargets[15].image);
        for (spotShadowIndex = 0; spotShadowIndex < backEndData->spotShadowCount; ++spotShadowIndex)
        {
            t0 = (double)spotShadowIndex * 0.25;
            RB_SetSpotShadowOverlayScaleAndBias(&backEndData->spotShadows[spotShadowIndex]);
            x = (double)spotShadowIndex * (h + 2.0) + 4.0;
            RB_DrawStretchPic(
                rgp.shadowOverlayMaterial,
                x,
                4.0,
                h,
                h,
                0.0,
                t0,
                1.0,
                t0 + 0.25,
                0xFFFFFFFF,
                GFX_PRIM_STATS_HUD);
            RB_EndTessSurface();
        }
        gfxCmdBufSourceState.input.codeImageSamplerStates[8] = 98;
    }
}


void __cdecl RB_SetSpotShadowOverlayScaleAndBias(const GfxSpotShadow *spotShadow)
{
    float nearDepth; // [esp+20h] [ebp-1Ch] BYREF
    unsigned __int8 shadowmapBuildTechType; // [esp+27h] [ebp-15h]
    float zNear; // [esp+28h] [ebp-14h]
    float zFar; // [esp+30h] [ebp-Ch]
    float farDepth; // [esp+38h] [ebp-4h] BYREF

    RB_GetShadowOverlayDepthBounds(&nearDepth, &farDepth);
    zNear = (-(spotShadow->shadowViewParms.projectionMatrix.m[3][2])) / spotShadow->shadowViewParms.projectionMatrix.m[2][2];
    zFar = spotShadow->light->radius;
    shadowmapBuildTechType = gfxMetrics.shadowmapBuildTechType;
    if (gfxMetrics.shadowmapBuildTechType == 3)
        R_UpdateCodeConstant(
            &gfxCmdBufSourceState,
            CONST_SRC_CODE_FILTER_TAP_0,
            1.0 / (float)((float)(zFar - zNear) * (float)(farDepth - nearDepth)),
            (-(1.0 / (float)((float)(zFar - zNear) * (float)(farDepth - nearDepth))))
            * (float)((float)((float)(zFar - zNear) * nearDepth) + zNear),
            1.0,
            1.0);
    else
        R_UpdateCodeConstant(
            &gfxCmdBufSourceState,
            CONST_SRC_CODE_FILTER_TAP_0,
            1.0 / (float)(farDepth - nearDepth),
            (-(1.0 / (float)(farDepth - nearDepth))) * nearDepth,
            zNear,
            zFar);
}

