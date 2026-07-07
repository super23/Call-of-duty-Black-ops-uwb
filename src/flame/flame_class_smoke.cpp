#include "flame_class_smoke.h"
#include "flame_class_stream.h"

#include <cstring>
#include "flame_system.h"
#include <universal/assertive.h>
#include <cgame_mp/cg_local_mp.h>
#include <universal/com_math_anglevectors.h>
#include <cgame/cg_drawtools.h>


flameSmoke_t flameSmoke[250];
flameSmoke_t *flameSmokeFree;
flameSmoke_t *flameSmokeUsed;
int g_SmokeCount;

void __cdecl Flame_Class_Smoke_Init()
{
    memset(flameSmoke, 0, sizeof(flameSmoke));
    Flame_List_Init((flameGeneric_s*)flameSmoke, 116, 250);
    flameSmokeFree = flameSmoke;
    flameSmokeUsed = 0;
    ++g_SmokeCount;
}

flameSmoke_t *__cdecl Flame_Class_Smoke_Alloc()
{
    flameSmoke_t *freeSmoke;

    freeSmoke = flameSmokeFree;
    if ( !flameSmokeFree )
        return 0;
    Flame_List_Move_Global(&flameSmokeFree->gen, (flameGeneric_s **)&flameSmokeFree, (flameGeneric_s **)&flameSmokeUsed);
    ++g_SmokeCount;
    return freeSmoke;
}

void __cdecl Flame_Class_Smoke_Free(flameSmoke_t *smoke)
{
    Flame_List_Move_Global(&smoke->gen, (flameGeneric_s **)&flameSmokeUsed, (flameGeneric_s **)&flameSmokeFree);
    Flame_Item_Init(&smoke->gen, sizeof(flameSmoke_t));
    --g_SmokeCount;
}

void __cdecl Flame_Class_Smoke_Age(bool is_server, int time)
{
    flameSmoke_t *smoke;
    flameSmoke_t *nextSmoke;

    if ( !is_server )
    {
        for ( smoke = flameSmokeUsed; smoke; smoke = nextSmoke )
        {
            nextSmoke = (flameSmoke_t *)smoke->gen.listGlobal.next;
            if ( smoke->gen.age.lastUpdateTime >= smoke->gen.age.endTime )
            {
                Flame_Class_Smoke_Free(smoke);
            }
            else
            {
                if ( smoke->gen.id < flame_freeze_id )
                {
                    smoke->gen.phys.velocity[0] = 0.0f;
                    smoke->gen.phys.velocity[1] = 0.0f;
                }
                Flame_Phys_Update_Item_Smoke(&smoke->gen, time);
                if ( smoke->gen.age.lastUpdateTime > smoke->gen.age.endTime )
                    smoke->gen.age.lastUpdateTime = smoke->gen.age.endTime;
            }
        }
        Flame_Phys_Update_Items(0);
    }
}

flameSmoke_t *__cdecl Flame_Class_Smoke_Spawn(flameChunk_s *fromChunk, int curTime)
{
    float smokeSpeedScale;
    int durationMs;
    flameTable *flameVars;
    flameSmoke_t *smoke;

    if ( !fromChunk || !fromChunk->gen.stream )
        return 0;
    smoke = Flame_Class_Smoke_Alloc();
    if ( !smoke )
        return 0;
    flameVars = fromChunk->gen.stream->flameVars;
    durationMs = (int)((Flame_Random(0) * flameVars->flameVar_smokeLifeRand + flameVars->flameVar_smokeLife) * 1000.0);
    smoke->gen.type = 3;
    smoke->gen.stream = fromChunk->gen.stream;
    smoke->gen.id = fromChunk->gen.id;
    smoke->gen.age.startTime = curTime;
    smoke->gen.age.endTime = durationMs + curTime;
    smoke->gen.age.lastUpdateTime = curTime;
    smoke->gen.size.current = fromChunk->gen.size.current + flameVars->flameVar_smokeStartSizeAdd;
    smoke->gen.size.rate = (float)((float)(fromChunk->gen.size.current + flameVars->flameVar_smokeEndSizeAdd)
                                                             - smoke->gen.size.current)
                                             / flameVars->flameVar_smokeLife;
    smoke->gen.phys.origin[0] = fromChunk->gen.phys.origin[0];
    smoke->gen.phys.origin[1] = fromChunk->gen.phys.origin[1];
    smoke->gen.phys.origin[2] = fromChunk->gen.phys.origin[2];
    smoke->gen.phys.origin[0] = Flame_CRandom(0) * flameVars->flameVar_smokeOriginOffset + smoke->gen.phys.origin[0];
    smoke->gen.phys.origin[1] = Flame_CRandom(0) * flameVars->flameVar_smokeOriginOffset + smoke->gen.phys.origin[1];
    smoke->gen.phys.origin[2] = (float)((float)(fromChunk->gen.size.current * flameVars->flameVar_smokeOriginSizeOfsZScale)
                                                                        + flameVars->flameVar_smokeOriginOfsZ)
                                                        + smoke->gen.phys.origin[2];
    smokeSpeedScale = flameVars->flameVar_smokeSpeedScale;
    smoke->gen.phys.velocity[0] = smokeSpeedScale * fromChunk->gen.phys.velocity[0];
    smoke->gen.phys.velocity[1] = smokeSpeedScale * fromChunk->gen.phys.velocity[1];
    smoke->gen.phys.velocity[2] = smokeSpeedScale * fromChunk->gen.phys.velocity[2];
    smoke->gen.phys.velocity[2] = smoke->gen.phys.velocity[2] + flameVars->flameVar_smokeVelocityAddZ;
    smoke->gen.phys.invStartSpeed = Flame_CalcInvStartSpeed(
                                                                        fromChunk->gen.phys.invStartSpeed,
                                                                        flameVars->flameVar_smokeSpeedScale);
    smoke->gen.phys.gravity = flameVars->flameVar_smokeGravity;
    smoke->gen.phys.rotation = Flame_CRandom(0) * flameVars->flameVar_smokeMaxRotation;
    smoke->gen.phys.rotVel = Flame_CRandom(0) * flameVars->flameVar_smokeMaxRotVel;
    smoke->smokeFadein = flameVars->flameVar_smokeFadein;
    smoke->smokeFadeout = flameVars->flameVar_smokeFadeout;
    smoke->smokeMaxAlpha = flameVars->flameVar_smokeMaxAlpha;
    smoke->smokeBrightness = flameVars->flameVar_smokeBrightness;
    smoke->material = flameVars->smoke;
    return smoke;
}

void __cdecl Flame_Class_Smoke_Render_Item(int localClientNum, flameSmoke_t *smoke)
{
    float halfSize;
    float negHalfSize;
    float pointOffsetRight;
    float pointOffsetDown;
    float alpha;
    float fwd[3];
    float points[4][2];
    float right[3];
    float start[3];
    float end[3];
    float angles[3];
    float down[3];
    int cornerIndex;
    cg_s *clientGlobals;

    clientGlobals = CG_GetLocalClientGlobals(localClientNum);
    iassert(clientGlobals);

    if ( flame_debug_render->current.integer <= 0 )
    {
        alpha = 1.0f;
        if ( (float)((float)smoke->gen.age.startTime
                             + (float)((float)(smoke->gen.age.endTime - smoke->gen.age.startTime) * smoke->smokeFadein)) <= (float)smoke->gen.age.lastUpdateTime )
        {
            if ( (float)smoke->gen.age.lastUpdateTime > (float)((float)smoke->gen.age.startTime
                                                                                                                + (float)((float)(smoke->gen.age.endTime
                                                                                                                                                - smoke->gen.age.startTime)
                                                                                                                                * smoke->smokeFadeout)) )
                alpha = 1.0
                            - (float)((float)((float)((float)(smoke->gen.age.lastUpdateTime - smoke->gen.age.startTime)
                                                                            / (float)(smoke->gen.age.endTime - smoke->gen.age.startTime))
                                                            - smoke->smokeFadeout)
                                            / (float)(1.0 - smoke->smokeFadeout));
        }
        else
        {
            alpha = (float)((float)(smoke->gen.age.lastUpdateTime - smoke->gen.age.startTime)
                                        / (float)(smoke->gen.age.endTime - smoke->gen.age.startTime))
                        / smoke->smokeFadein;
        }
        // CoDMPServer.c:704689-704760 — alpha fade; frame=-1 lets Flame_Render_Sprite derive atlas lifeFrac.
        // smoke->material is typically mc/mtl_fx_3d_clouds_fly from flameTable (smoke field).
        Flame_Render_Sprite(
            clientGlobals,
            smoke->material,
            &smoke->gen,
            alpha * smoke->smokeMaxAlpha,
            -1.0f,
            (int)(float)(255.0 * smoke->smokeBrightness));
    }
    else
    {
        points[0][0] = -1.0f;
        points[0][1] = -1.0f;

        points[1][0] = -1.0f;
        points[1][1] = 1.0f;

        points[2][0] = 1.0f;
        points[2][1] = 1.0f;

        points[3][0] = 1.0f;
        points[3][1] = -1.0f;

        AxisToAngles(clientGlobals->refdef.viewaxis, angles);
        angles[2] = smoke->gen.phys.rotation;
        AngleVectors(angles, fwd, right, down);
        halfSize = smoke->gen.size.current * 0.5f;
        right[0] = halfSize * right[0];
        right[1] = halfSize * right[1];
        right[2] = halfSize * right[2];
        negHalfSize = smoke->gen.size.current * -0.5f;
        down[0] = negHalfSize * down[0];
        down[1] = negHalfSize * down[1];
        down[2] = negHalfSize * down[2];
        start[0] = points[3][0] * right[0] + smoke->gen.phys.origin[0];
        start[1] = points[3][0] * right[1] + smoke->gen.phys.origin[1];
        start[2] = points[3][0] * right[2] + smoke->gen.phys.origin[2];
        start[0] = points[3][1] * down[0] + start[0];
        start[1] = points[3][1] * down[1] + start[1];
        start[2] = points[3][1] * down[2] + start[2];
        for ( cornerIndex = 0; cornerIndex < 4; ++cornerIndex )
        {
            pointOffsetRight = points[cornerIndex][0];
            end[0] = pointOffsetRight * right[0] + smoke->gen.phys.origin[0];
            end[1] = pointOffsetRight * right[1] + smoke->gen.phys.origin[1];
            end[2] = pointOffsetRight * right[2] + smoke->gen.phys.origin[2];
            pointOffsetDown = points[cornerIndex][1];
            end[0] = pointOffsetDown * down[0] + end[0];
            end[1] = pointOffsetDown * down[1] + end[1];
            end[2] = pointOffsetDown * down[2] + end[2];
            CG_DebugLine(start, end, colorMdGrey, 1, 1);
            start[0] = end[0];
            start[1] = end[1];
            start[2] = end[2];
        }
    }
}

void __cdecl Flame_Class_Smoke_Render_All(int localClientNum)
{
    flameSmoke_t *smoke;

    // Walk to the tail, then render back-to-front through the global list.
    for ( smoke = flameSmokeUsed; smoke && smoke->gen.listGlobal.next; smoke = (flameSmoke_t *)smoke->gen.listGlobal.next )
        ;
    while ( smoke )
    {
        Flame_Class_Smoke_Render_Item(localClientNum, smoke);
        smoke = (flameSmoke_t *)smoke->gen.listGlobal.prev;
    }
}

