#include "flame_class_fire.h"
#include "flame_class_stream.h"

#include <cstring>
#include "flame_physics.h"
#include <universal/assertive.h>
#include <cgame_mp/cg_local_mp.h>
#include <universal/com_math_anglevectors.h>
#include <cgame/cg_drawtools.h>

flameFire_t flameFire[2400];
flameFire_t *flameFireFree;
flameFire_t *flameFireUsed;
int g_FireCount;
int g_FireCountWaterMark;

void __cdecl Flame_Class_Fire_Init()
{
    memset(flameFire, 0, sizeof(flameFire));
    Flame_List_Init((flameGeneric_s*)flameFire, 100, 2400);
    flameFireFree = flameFire;
    flameFireUsed = 0;
    ++g_FireCount;
}

flameFire_t *__cdecl Flame_Class_Fire_Alloc()
{
    flameFire_t *freeFire;

    freeFire = flameFireFree;
    if ( !flameFireFree )
        return 0;
    Flame_List_Move_Global(&flameFireFree->gen, (flameGeneric_s **)&flameFireFree, (flameGeneric_s **)&flameFireUsed);
    if ( ++g_FireCount > g_FireCountWaterMark )
        g_FireCountWaterMark = g_FireCount;
    return freeFire;
}

void __cdecl Flame_Class_Fire_Free(bool is_server, flameFire_t *fire)
{
    if ( fire->flameRend )
        Flame_List_Sub_Local(&fire->gen, &fire->flameRend->fireList);
    Flame_List_Move_Global(&fire->gen, (flameGeneric_s **)&flameFireUsed, (flameGeneric_s **)&flameFireFree);
    --g_FireCount;
    Flame_Item_Init(&fire->gen, sizeof(flameFire_t));
}

void __cdecl Flame_Class_Fire_Age(bool is_server, int time)
{
    flameFire_t *next;
    flameFire_t *fire;

    if ( !is_server )
    {
        for ( fire = flameFireUsed; fire; fire = next )
        {
            next = (flameFire_t *)fire->gen.listGlobal.next;
            if ( fire->gen.age.lastUpdateTime < fire->gen.age.endTime )
            {
                if ( fire->gen.id < flame_freeze_id )
                {
                    fire->gen.phys.velocity[0] = 0.0f;
                    fire->gen.phys.velocity[1] = 0.0f;
                }
                Flame_Phys_Update_Item_Fire(&fire->gen, time);
            }
            if ( fire->gen.age.lastUpdateTime >= fire->gen.age.endTime )
                Flame_Class_Fire_Free(is_server, fire);
        }
        Flame_Phys_Update_Items(0);
    }
}

flameFire_t *__cdecl Flame_Class_Fire_Spawn(bool is_server, flameChunk_s *fromChunk, int curTime, float minLifeFrac)
{
    float speedScaleBase;
    float lifeFrac;
    float sidewaysOffset;
    flameFire_t *fire;
    float fwdVec[3];
    flameRender_s *flameRend;
    flameTable *flameVars;
    float speed;
    float lifetime;
    float sideVec[3];
    float upVec[3];

    flameRend = 0;
    upVec[0] = 0.0f;
    upVec[1] = 0.0f;
    upVec[2] = 1.0f;
    iassert(fromChunk);
    if ( is_server )
        return 0;
    if ( !fromChunk || !fromChunk->gen.stream )
        return 0;
    fire = Flame_Class_Fire_Alloc();
    if ( !fire )
        return 0;
    flameVars = fromChunk->gen.stream->flameVars;
    flameRend = fromChunk->gen.stream->renderList->flameRend;
    fire->gen.type = 2;
    fire->gen.stream = fromChunk->gen.stream;
    fire->gen.id = fromChunk->gen.id;
    iassert(fromChunk->gen.age.endTime != fromChunk->gen.age.startTime);
    if ( (float)((float)(fromChunk->gen.age.lastUpdateTime - fromChunk->gen.age.startTime)
                         / (float)(fromChunk->gen.age.endTime - fromChunk->gen.age.startTime)) <= minLifeFrac )
        lifeFrac = minLifeFrac;
    else
        lifeFrac = (float)(fromChunk->gen.age.lastUpdateTime - fromChunk->gen.age.startTime)
             / (float)(fromChunk->gen.age.endTime - fromChunk->gen.age.startTime);
    lifetime = Flame_Random(0) * flameVars->flameVar_fireLifeRand + flameVars->flameVar_fireLife;
    fire->gen.age.startTime = curTime - (int)(float)((float)(lifeFrac * lifetime) * 1000.0);
    fire->gen.age.endTime = fire->gen.age.startTime + (int)(float)(1000.0 * lifetime);
    fire->gen.age.lastUpdateTime = curTime;
    fire->gen.size.current = fromChunk->gen.size.current * flameVars->flameVar_fireStartSizeScale;
    fire->gen.size.rate = (float)((float)((float)(fromChunk->gen.size.current * flameVars->flameVar_fireEndSizeScale)
                                                                            + flameVars->flameVar_fireEndSizeAdd)
                                                            - fire->gen.size.current)
                                            / (float)((float)(1.0 - lifeFrac) * flameVars->flameVar_fireLife);
    if ( fire->gen.size.rate > 70.0 )
        fire->gen.size.rate = 70.0f;
    iassert(!IS_NAN(fire->gen.size.rate));
    fire->gen.phys.origin[0] = fromChunk->gen.phys.origin[0];
    fire->gen.phys.origin[1] = fromChunk->gen.phys.origin[1];
    fire->gen.phys.origin[2] = fromChunk->gen.phys.origin[2];
    speedScaleBase = 1.0f - flameVars->flameVar_fireSpeedScaleRand;
    speed = (Flame_Random(0) * flameVars->flameVar_fireSpeedScaleRand + speedScaleBase) * flameVars->flameVar_fireSpeedScale;
    fire->gen.phys.velocity[0] = speed * fromChunk->gen.phys.velocity[0];
    fire->gen.phys.velocity[1] = speed * fromChunk->gen.phys.velocity[1];
    fire->gen.phys.velocity[2] = speed * fromChunk->gen.phys.velocity[2];
    fwdVec[0] = fire->gen.phys.velocity[0];
    fwdVec[1] = fire->gen.phys.velocity[1];
    fwdVec[2] = 0.0f;
    speed = Vec3Normalize(fwdVec);
    Vec3Cross(fwdVec, upVec, sideVec);
    sidewaysOffset = Flame_CRandom(0) * speed * flameVars->flameVar_fireVelocityAddSideways;
    fire->gen.phys.velocity[0] = sidewaysOffset * sideVec[0] + fire->gen.phys.velocity[0];
    fire->gen.phys.velocity[1] = sidewaysOffset * sideVec[1] + fire->gen.phys.velocity[1];
    fire->gen.phys.velocity[2] = sidewaysOffset * sideVec[2] + fire->gen.phys.velocity[2];
    fire->gen.phys.velocity[2] = Flame_Random(0) * flameVars->flameVar_fireVelocityAddZRand
                                                         + flameVars->flameVar_fireVelocityAddZ
                                                         + fire->gen.phys.velocity[2];
    fire->gen.phys.invStartSpeed = Flame_CalcInvStartSpeed(
                                                                     fromChunk->gen.phys.invStartSpeed,
                                                                     flameVars->flameVar_fireSpeedScale);
    fire->gen.phys.gravity = flameVars->flameVar_fireGravity;
    fire->gen.phys.rotation = fromChunk->gen.phys.rotation;
    fire->gen.phys.rotVel = Flame_CRandom(0) * flameVars->flameVar_fireMaxRotVel;
    fire->flameRend = flameRend;
    Flame_List_Add_Local(&fire->gen, &fire->flameRend->fireList);
    return fire;
}

void __cdecl Flame_Class_Fire_Render_Item(int localClientNum, flameFire_t *fire)
{
    float halfSize;
    float negHalfSize;
    float pointOffsetRight;
    float pointOffsetDown;
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

    if ( flame_debug_render->current.integer > 0 )
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
        angles[2] = fire->gen.phys.rotation;
        AngleVectors(angles, fwd, right, down);
        halfSize = fire->gen.size.current * 0.5f;
        right[0] = halfSize * right[0];
        right[1] = halfSize * right[1];
        right[2] = halfSize * right[2];
        negHalfSize = fire->gen.size.current * -0.5f;
        down[0] = negHalfSize * down[0];
        down[1] = negHalfSize * down[1];
        down[2] = negHalfSize * down[2];
        start[0] = points[3][0] * right[0] + fire->gen.phys.origin[0];
        start[1] = points[3][0] * right[1] + fire->gen.phys.origin[1];
        start[2] = points[3][0] * right[2] + fire->gen.phys.origin[2];
        start[0] = points[3][1] * down[0] + start[0];
        start[1] = points[3][1] * down[1] + start[1];
        start[2] = points[3][1] * down[2] + start[2];
        for ( cornerIndex = 0; cornerIndex < 4; ++cornerIndex )
        {
            pointOffsetRight = points[cornerIndex][0];
            end[0] = pointOffsetRight * right[0] + fire->gen.phys.origin[0];
            end[1] = pointOffsetRight * right[1] + fire->gen.phys.origin[1];
            end[2] = pointOffsetRight * right[2] + fire->gen.phys.origin[2];
            pointOffsetDown = points[cornerIndex][1];
            end[0] = pointOffsetDown * down[0] + end[0];
            end[1] = pointOffsetDown * down[1] + end[1];
            end[2] = pointOffsetDown * down[2] + end[2];
            CG_DebugLine(start, end, colorYellow, 1, 1);
            start[0] = end[0];
            start[1] = end[1];
            start[2] = end[2];
        }
    }
}

void __cdecl Flame_Class_Fire_Render_All(int localClientNum)
{
    flameFire_t *fire;

    if ( flame_debug_render->current.integer > 0 )
    {
        for ( fire = flameFireUsed; fire; fire = (flameFire_t *)fire->gen.listGlobal.next )
            Flame_Class_Fire_Render_Item(localClientNum, fire);
    }
}

void __cdecl Flame_Class_Fire_Render_Local_List(int localClientNum, flameFire_t *start)
{
    flameFire_t *fire;
    cg_s *clientGlobals;
    flameGeneric_s *flameGenericList[2401];
    int numItems;

    clientGlobals = CG_GetLocalClientGlobals(localClientNum);
    iassert(clientGlobals);

    if ( (!start || start->gen.stream) && (!start || Flame_GetLocalClientSourceRange()) )
    {
        fire = start;
        numItems = 0;
        while ( numItems < 2400 && fire )
        {
            flameGenericList[numItems++] = &fire->gen;
            fire = (flameFire_t *)fire->gen.listLocal.next;
        }
        // CoDMPServer.c:704558
        if ( numItems > 1 )
            Flame_Render_Sprites(
                clientGlobals,
                start->gen.stream->flameVars->fire,
                flameGenericList,
                numItems);
    }
}
