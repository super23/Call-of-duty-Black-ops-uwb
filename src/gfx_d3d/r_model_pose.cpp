#include "r_model_pose.h"
#include <xanim/xmodel.h>
#include <xanim/dobj_utils.h>
#include "r_dvars.h"
#include <cgame_mp/cg_pose_mp.h>
#include <EffectsCore/fx_marks.h>

float4 g_worldMins = { { -131072.0, -131072.0, -131072.0, 0.0 } }; // KISAKTODO: better spot
float4 g_worldMaxs = { { 131072.0, 131072.0, 131072.0, 0.0 } }; // KISAKTODO: better spot


// KISAKTODO: cleanup/refactor this gross func
// local variable allocation has failed, the output may be wrong!
DObjAnimMat * R_UpdateSceneEntBounds(
                GfxSceneEntity *sceneEnt,
                GfxSceneEntity **pLocalSceneEnt,
                const DObj **pObj,
                int waitForCullState)
{
    float v6; // xmm0_4
    float maxRadius; // [esp+24h] [ebp-614h]
    float maxRadiusSq; // [esp+28h] [ebp-610h]
    float v9; // [esp+2Ch] [ebp-60Ch]
    float radiusSqMins; // [esp+34h] [ebp-604h]
    unsigned __int8 *side; // [esp+40h] [ebp-5F8h]
    float *v12; // [esp+44h] [ebp-5F4h]
    int v13; // [esp+54h] [ebp-5E4h]
    int v14; // [esp+58h] [ebp-5E0h]
    int v15; // [esp+5Ch] [ebp-5DCh]
    int v16; // [esp+60h] [ebp-5D8h]
    int v17; // [esp+6Ch] [ebp-5CCh]
    int v18; // [esp+70h] [ebp-5C8h]
    int v19; // [esp+74h] [ebp-5C4h]
    __int64 v20; // [esp+88h] [ebp-5B0h]
    int v21; // [esp+98h] [ebp-5A0h]
    __int64 v22; // [esp+ACh] [ebp-58Ch]
    float v23; // [esp+B4h] [ebp-584h]
    float v24; // [esp+B8h] [ebp-580h]
    float v25; // [esp+D0h] [ebp-568h]
    float v26; // [esp+D4h] [ebp-564h]
    float v27; // [esp+D8h] [ebp-560h]
    float v28; // [esp+DCh] [ebp-55Ch]
    int v29; // [esp+104h] [ebp-534h]
    int v30; // [esp+108h] [ebp-530h]
    int v31; // [esp+10Ch] [ebp-52Ch]
    int v32; // [esp+110h] [ebp-528h]
    int v33; // [esp+148h] [ebp-4F0h]
    int v34; // [esp+14Ch] [ebp-4ECh]
    int v35; // [esp+150h] [ebp-4E8h]
    int v36; // [esp+154h] [ebp-4E4h]
    int v37; // [esp+1CCh] [ebp-46Ch]
    int v38; // [esp+1D0h] [ebp-468h]
    int v39; // [esp+1D4h] [ebp-464h]
    int v40; // [esp+204h] [ebp-434h]
    int v41; // [esp+208h] [ebp-430h]
    float v42; // [esp+2C8h] [ebp-370h]
    int v43; // [esp+2CCh] [ebp-36Ch]
    int v44; // [esp+2D0h] [ebp-368h]
    float v45; // [esp+2D8h] [ebp-360h]
    float v46; // [esp+2DCh] [ebp-35Ch]
    float v47; // [esp+2E0h] [ebp-358h]
    float *v48; // [esp+2E8h] [ebp-350h]
    float v49; // [esp+2ECh] [ebp-34Ch] BYREF
    float v50; // [esp+2F0h] [ebp-348h]
    XBoneInfo *boneInfo; // [esp+2F4h] [ebp-344h]
    DObjSkelMat boneAxis; // [esp+2F8h] [ebp-340h]
    DObjAnimMat *bone; // [esp+338h] [ebp-300h]
    int boneIndex; // [esp+33Ch] [ebp-2FCh] BYREF
    _BYTE boneInfoArray_632[12]; // [esp+5C0h] [ebp-78h] OVERLAPPED
    _BYTE v56[33]; // [esp+5CCh] [ebp-6Ch] OVERLAPPED BYREF
    int partBits[5]; // [esp+5F8h] [ebp-40h]
    int v58; // [esp+60Ch] [ebp-2Ch]
    int frustumPlaneCount; // [esp+610h] [ebp-28h]
    DpvsView *v60; // [esp+614h] [ebp-24h]
    bool v61; // [esp+61Bh] [ebp-1Dh]
    DObj *v62; // [esp+61Ch] [ebp-1Ch]
    DpvsView *dpvsView; // [esp+620h] [ebp-18h]
    volatile unsigned int v64; // [esp+624h] [ebp-14h] OVERLAPPED
    //GfxSceneEntity *localSceneEnt; // [esp+62Ch] [ebp-Ch]
    //unsigned int state; // [esp+630h] [ebp-8h]
    //unsigned int retaddr; // [esp+638h] [ebp+0h]

    //localSceneEnt = a1;
    //state = retaddr;
    if ( _InterlockedCompareExchange(&sceneEnt->cull.state, 1, 0) )
    {
        *pLocalSceneEnt = 0;
        if ( waitForCullState )
        {
            do
            {
                v64 = sceneEnt->cull.state;
                if ( !v64
                    && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_pose.cpp",
                                201,
                                0,
                                "%s\n\t(state) = %i",
                                "(state >= CULL_STATE_BOUNDED_PENDING)",
                                0) )
                {
                    __debugbreak();
                }
            }
            while ( v64 == 1 );
            if ( v64 == 4 )
            {
                return 0;
            }
            else
            {
                dpvsView = (DpvsView *)sceneEnt;
                *pLocalSceneEnt = sceneEnt;
                v62 = (DObj *)LODWORD(dpvsView->frustumPlanes[5].coeffs[2]);
                *pObj = v62;
                if ( !v62
                    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_pose.cpp", 212, 0, "%s", "obj") )
                {
                    __debugbreak();
                }
                return I_dmaGetDObjSkel(v62);
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        dpvsView = (DpvsView *)sceneEnt;
        *pLocalSceneEnt = sceneEnt;
        if ( !LODWORD(dpvsView->frustumPlanes[5].coeffs[2])
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_pose.cpp",
                        229,
                        0,
                        "%s",
                        "localSceneEnt->obj") )
        {
            __debugbreak();
        }
        v62 = (DObj *)LODWORD(dpvsView->frustumPlanes[5].coeffs[2]);
        *pObj = v62;
        if ( !v62 && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_pose.cpp", 232, 0, "%s", "obj") )
            __debugbreak();
        v61 = 0;
        if ( r_offscreenCasterLodScale->current.value > 1.0 )
        {
            v60 = dpvsGlob.views[scene.dpvs.localClientNum];
            if ( LODWORD(dpvsView->frustumPlanes[2].coeffs[0]) >= 2 )
            {
                frustumPlaneCount = v60->frustumPlaneCount;
                v58 = 0;
                partBits[4] = (int)v60->frustumPlanes;
                while ( v58 < frustumPlaneCount )
                {
                    partBits[3] = R_DpvsPlaneMaxSignedDistToBox(
                                                    (const DpvsPlane *)partBits[4],
                                                    &dpvsView->frustumPlanes[2].coeffs[1]) <= 0.0;
                    if ( partBits[3] )
                    {
                        partBits[2] = 1;
                        goto LABEL_28;
                    }
                    ++v58;
                    partBits[4] += 20;
                }
                partBits[2] = 0;
LABEL_28:
                if ( partBits[2] )
                    v61 = 1;
            }
        }
        DObjGetSurfaceData(
            v62,
            &dpvsView->frustumPlanes[1].coeffs[1],
            *(float *)dpvsView->frustumPlanes[1].side,
            v61,
            (char *)&dpvsView->frustumPlanes[3].coeffs[2]);
        if ( useFastFile->current.enabled || !DObjBad(v62) )
        {
            if ( v62->localModels )
                *(unsigned int *)&v56[28] = DObjGetSurfaces(v62, (int *)&v56[32], (const char *)&dpvsView->frustumPlanes[3].coeffs[2]);
            else
                *(unsigned int *)&v56[28] = 0;
            if ( !*(unsigned int *)&v56[28] )
                goto LABEL_37;
            if ( DObjIsRecorded(v62) && DObjGetRotTransArray(v62) )
            {
                *(unsigned int *)&v56[24] = (unsigned int)I_dmaGetDObjSkel(v62);
                DObjGetSetBones(v62, (int *)&v56[32]);
                DObjSkelSetSkel(v62, (int *)&v56[32]);
            }
            else
            {
                *(unsigned int *)&v56[24] = (unsigned int)R_DObjCalcPose((const GfxSceneEntity *)dpvsView, v62, (int *)&v56[32]);
            }
            if ( *(unsigned int *)&v56[24] )
            {
                if ( !DObjSkelAreBonesUpToDate(v62, (int *)&v56[32])
                    && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_pose.cpp",
                                328,
                                0,
                                "%s",
                                "DObjSkelAreBonesUpToDate( obj, partBits )") )
                {
                    __debugbreak();
                }
                *(_QWORD *)&boneInfoArray_632[4] = *(_QWORD *)g_worldMaxs.v;
                *(_QWORD *)v56 = *(_QWORD *)&g_worldMaxs.unitVec[2].packed;
                *(unsigned int *)boneInfoArray_632 = (unsigned int)&v56[8];
                *(float4 *)&v56[8] = g_worldMins;
                DObjGetBoneInfo(v62, (XBoneInfo **)&boneIndex);
                bone = (DObjAnimMat *)DObjNumBones(v62);
                boneAxis.origin[3] = -0.0;
                boneAxis.origin[2] = 0.0;
                while ( SLODWORD(boneAxis.origin[2]) < (int)bone )
                {
                    if ( (LODWORD(boneAxis.origin[3]) & *(unsigned int *)&v56[4 * (SLODWORD(boneAxis.origin[2]) >> 5) + 32]) != 0 )
                    {
                        LODWORD(boneAxis.origin[1]) = *(unsigned int *)&v56[24] + 32 * LODWORD(boneAxis.origin[2]);
                        ConvertQuatToSkelMat((const DObjAnimMat *)LODWORD(boneAxis.origin[1]), (DObjSkelMat *)&v49);
                        v48 = *(float **)(boneInfoArray_632 + 4 * LODWORD(boneAxis.origin[2]) + 636);
                        v42 = *v48;
                        v43 = *((unsigned int *)v48 + 1);
                        v44 = *((unsigned int *)v48 + 2);
                        v45 = v48[3];
                        v46 = v48[4];
                        v47 = v48[5];
                        if ( v49 >= 0.0 )
                            v41 = 0;
                        else
                            v41 = -1;
                        if ( v50 >= 0.0 )
                            v39 = 0;
                        else
                            v39 = -1;
                        if ( *(float *)&boneInfo >= 0.0 )
                            v38 = 0;
                        else
                            v38 = -1;
                        if ( boneAxis.axis[0][0] >= 0.0 )
                            v37 = 0;
                        else
                            v37 = -1;
                        if ( boneAxis.axis[0][1] >= 0.0 )
                            v36 = 0;
                        else
                            v36 = -1;
                        if ( boneAxis.axis[0][2] >= 0.0 )
                            v35 = 0;
                        else
                            v35 = -1;
                        if ( boneAxis.axis[0][3] >= 0.0 )
                            v34 = 0;
                        else
                            v34 = -1;
                        if ( boneAxis.axis[1][0] >= 0.0 )
                            v33 = 0;
                        else
                            v33 = -1;
                        if ( boneAxis.axis[1][1] >= 0.0 )
                            v32 = 0;
                        else
                            v32 = -1;
                        if ( boneAxis.axis[1][2] >= 0.0 )
                            v31 = 0;
                        else
                            v31 = -1;
                        if ( boneAxis.axis[1][3] >= 0.0 )
                            v30 = 0;
                        else
                            v30 = -1;
                        if ( boneAxis.axis[2][0] >= 0.0 )
                            v29 = 0;
                        else
                            v29 = -1;
                        v40 = v29;
                        v25 = (float)(COERCE_FLOAT(LODWORD(v47) & v32 | v44 & ~v32) * boneAxis.axis[1][1])
                                + (float)((float)(COERCE_FLOAT(LODWORD(v46) & v36 | v43 & ~v36) * boneAxis.axis[0][1])
                                                + (float)((float)(COERCE_FLOAT(LODWORD(v45) & v41 | LODWORD(v42) & ~v41) * v49)
                                                                + boneAxis.axis[2][1]));
                        v26 = (float)(COERCE_FLOAT(LODWORD(v47) & v31 | v44 & ~v31) * boneAxis.axis[1][2])
                                + (float)((float)(COERCE_FLOAT(LODWORD(v46) & v35 | v43 & ~v35) * boneAxis.axis[0][2])
                                                + (float)((float)(COERCE_FLOAT(LODWORD(v45) & v39 | LODWORD(v42) & ~v39) * v50)
                                                                + boneAxis.axis[2][2]));
                        v27 = (float)(COERCE_FLOAT(LODWORD(v47) & v30 | v44 & ~v30) * boneAxis.axis[1][3])
                                + (float)((float)(COERCE_FLOAT(LODWORD(v46) & v34 | v43 & ~v34) * boneAxis.axis[0][3])
                                                + (float)((float)(COERCE_FLOAT(LODWORD(v45) & v38 | LODWORD(v42) & ~v38) * *(float *)&boneInfo)
                                                                + boneAxis.axis[2][3]));
                        v28 = (float)(COERCE_FLOAT(LODWORD(v47) & v40 | v44 & ~v40) * boneAxis.axis[2][0])
                                + (float)((float)(COERCE_FLOAT(LODWORD(v46) & v33 | v43 & ~v33) * boneAxis.axis[1][0])
                                                + (float)((float)(COERCE_FLOAT(LODWORD(v45) & v37 | LODWORD(v42) & ~v37) * boneAxis.axis[0][0])
                                                                + boneAxis.origin[0]));
                        *(float *)&v22 = (float)(COERCE_FLOAT(v44 & v32 | LODWORD(v47) & ~v32) * boneAxis.axis[1][1])
                                                     + (float)((float)(COERCE_FLOAT(v43 & v36 | LODWORD(v46) & ~v36) * boneAxis.axis[0][1])
                                                                     + (float)((float)(COERCE_FLOAT(LODWORD(v42) & v41 | LODWORD(v45) & ~v41) * v49)
                                                                                     + boneAxis.axis[2][1]));
                        *((float *)&v22 + 1) = (float)(COERCE_FLOAT(v44 & v31 | LODWORD(v47) & ~v31) * boneAxis.axis[1][2])
                                                                 + (float)((float)(COERCE_FLOAT(v43 & v35 | LODWORD(v46) & ~v35) * boneAxis.axis[0][2])
                                                                                 + (float)((float)(COERCE_FLOAT(LODWORD(v42) & v39 | LODWORD(v45) & ~v39) * v50)
                                                                                                 + boneAxis.axis[2][2]));
                        v23 = (float)(COERCE_FLOAT(v44 & v30 | LODWORD(v47) & ~v30) * boneAxis.axis[1][3])
                                + (float)((float)(COERCE_FLOAT(v43 & v34 | LODWORD(v46) & ~v34) * boneAxis.axis[0][3])
                                                + (float)((float)(COERCE_FLOAT(LODWORD(v42) & v38 | LODWORD(v45) & ~v38) * *(float *)&boneInfo)
                                                                + boneAxis.axis[2][3]));
                        v24 = (float)(COERCE_FLOAT(v44 & v40 | LODWORD(v47) & ~v40) * boneAxis.axis[2][0])
                                + (float)((float)(COERCE_FLOAT(v43 & v33 | LODWORD(v46) & ~v33) * boneAxis.axis[1][0])
                                                + (float)((float)(COERCE_FLOAT(LODWORD(v42) & v37 | LODWORD(v45) & ~v37) * boneAxis.axis[0][0])
                                                                + boneAxis.origin[0]));
                        if ( *(float *)&boneInfoArray_632[4] <= v25 )
                            v21 = 0;
                        else
                            v21 = -1;
                        if ( *(float *)&boneInfoArray_632[8] <= v26 )
                            v19 = 0;
                        else
                            v19 = -1;
                        if ( *(float *)v56 <= v27 )
                            v18 = 0;
                        else
                            v18 = -1;
                        if ( *(float *)&v56[4] <= v28 )
                            v17 = 0;
                        else
                            v17 = -1;
                        *(unsigned int *)&boneInfoArray_632[4] = LODWORD(v25) & v21 | *(unsigned int *)&boneInfoArray_632[4] & ~v21;
                        *(unsigned int *)&boneInfoArray_632[8] = LODWORD(v26) & v19 | *(unsigned int *)&boneInfoArray_632[8] & ~v19;
                        *(unsigned int *)v56 = LODWORD(v27) & v18 | *(unsigned int *)v56 & ~v18;
                        *(unsigned int *)&v56[4] = LODWORD(v28) & v17 | *(unsigned int *)&v56[4] & ~v17;
                        if ( *(float *)&v56[8] <= *(float *)&v22 )
                            v16 = 0;
                        else
                            v16 = -1;
                        LODWORD(v20) = v16;
                        if ( *(float *)&v56[12] <= *((float *)&v22 + 1) )
                            v15 = 0;
                        else
                            v15 = -1;
                        HIDWORD(v20) = v15;
                        if ( *(float *)&v56[16] <= v23 )
                            v14 = 0;
                        else
                            v14 = -1;
                        if ( *(float *)&v56[20] <= v24 )
                            v13 = 0;
                        else
                            v13 = -1;
                        *(_QWORD *)&v56[8] = *(_QWORD *)&v56[8] & v20 | v22 & ~v20;
                        *(unsigned int *)&v56[16] = *(unsigned int *)&v56[16] & v14 | LODWORD(v23) & ~v14;
                        *(unsigned int *)&v56[20] = *(unsigned int *)&v56[20] & v13 | LODWORD(v24) & ~v13;
                    }
                    ++LODWORD(boneAxis.origin[2]);
                    LODWORD(boneAxis.origin[3]) = (LODWORD(boneAxis.origin[3]) << 31) | (LODWORD(boneAxis.origin[3]) >> 1);
                }
                v12 = &dpvsView->frustumPlanes[2].coeffs[1];
                dpvsView->frustumPlanes[2].coeffs[1] = *(float *)&boneInfoArray_632[4];
                v12[1] = *(float *)&boneInfoArray_632[8];
                v12[2] = *(float *)v56;
                side = dpvsView->frustumPlanes[2].side;
                *(float *)dpvsView->frustumPlanes[2].side = *(float *)&v56[8];
                *((float *)side + 1) = *(float *)&v56[12];
                *((float *)side + 2) = *(float *)&v56[16];
                if ( LODWORD(dpvsView->frustumPlanes[2].coeffs[0]) != 1
                    && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_pose.cpp",
                                353,
                                0,
                                "%s\n\t(localSceneEnt->cull.state) = %i",
                                "(localSceneEnt->cull.state == CULL_STATE_BOUNDED_PENDING)",
                                dpvsView->frustumPlanes[2].coeffs[0]) )
                {
                    __debugbreak();
                }
                if ( (float)((float)(v62->radius - 16.0) - 0.0) < 0.0 )
                    radiusSqMins = 0.0f;
                else
                    radiusSqMins = v62->radius - 16.0;
                v9 = v62->radius + 16.0;
                maxRadiusSq = Vec3DistanceSq(&dpvsView->frustumPlanes[1].coeffs[1], &dpvsView->frustumPlanes[2].coeffs[1]);
                maxRadius = Vec3DistanceSq(
                                            &dpvsView->frustumPlanes[1].coeffs[1],
                                            (const float *)dpvsView->frustumPlanes[2].side);
                if ( (float)(maxRadiusSq - maxRadius) < 0.0 )
                    v6 = maxRadius;
                else
                    v6 = maxRadiusSq;
                if ( (float)(radiusSqMins * radiusSqMins) > v6 || v6 > (float)(v9 * v9) )
                    *(float *)(LODWORD(dpvsView->frustumPlanes[5].coeffs[2]) + 88) = sqrtf(v6);
                LODWORD(dpvsView->frustumPlanes[2].coeffs[0]) = 2;
                return *(DObjAnimMat **)&v56[24];
            }
            else
            {
LABEL_37:
                R_SetNoDraw(sceneEnt);
                return 0;
            }
        }
        else
        {
            R_SetNoDraw(sceneEnt);
            return 0;
        }
    }
}

DObjAnimMat *__cdecl R_DObjCalcPose(const GfxSceneEntity *sceneEnt, DObj *obj, int *partBits)
{
    int i; // [esp+0h] [ebp-1Ch]
    int completePartBits[5]; // [esp+8h] [ebp-14h] BYREF

    if ( !sceneEnt
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_pose.cpp", 49, 0, "%s", "sceneEnt") )
    {
        __debugbreak();
    }
    if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_pose.cpp", 50, 0, "%s", "obj") )
        __debugbreak();
    for ( i = 0; i < 5; ++i )
        completePartBits[i] = partBits[i];
    return CG_DObjCalcPose(sceneEnt->info.pose, obj, completePartBits);
}

void __cdecl R_SetNoDraw(GfxSceneEntity *sceneEnt)
{
    if ( sceneEnt->cull.state != 1
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_pose.cpp",
                    71,
                    0,
                    "%s\n\t(sceneEnt->cull.state) = %i",
                    "(sceneEnt->cull.state == CULL_STATE_BOUNDED_PENDING)",
                    sceneEnt->cull.state) )
    {
        __debugbreak();
    }
    sceneEnt->cull.state = 4;
}

void __cdecl R_UpdateGfxEntityBoundsCmd(GfxSceneEntity **data)
{
    const DObj *obj; // [esp+0h] [ebp-10h] BYREF
    GfxSceneEntity *localSceneEnt; // [esp+4h] [ebp-Ch] BYREF
    GfxSceneEntity *sceneEnt; // [esp+8h] [ebp-8h]
    GfxSceneEntity **pSceneEnt; // [esp+Ch] [ebp-4h]
    int savedregs; // [esp+10h] [ebp+0h] BYREF

    localSceneEnt = 0;
    obj = 0;
    if ( !data && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_pose.cpp", 413, 0, "%s", "data") )
        __debugbreak();
    pSceneEnt = data;
    sceneEnt = *data;
    if ( R_UpdateSceneEntBounds(sceneEnt, &localSceneEnt, &obj, 0)
        && !localSceneEnt
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_model_pose.cpp", 423, 0, "%s", "localSceneEnt") )
    {
        __debugbreak();
    }
}

