#include "flame_class_drips.h"
#include "flame_class_stream.h"
#include <cstring>
#include <universal/assertive.h>
#include <cgame_mp/cg_local_mp.h>
#include <universal/com_math_anglevectors.h>
#include <cgame/cg_drawtools.h>

flameDrips_t flameDrips[250];
flameDrips_t *flameDripsFree;
flameDrips_t *flameDripsUsed;

flameDrips_t sv_flameDrips[250];
flameDrips_t *sv_flameDripsFree;
flameDrips_t *sv_flameDripsUsed;

int g_DripsCount;
int g_DripsCountWaterMark;

void __cdecl Flame_Class_Drips_Init()
{
    memset(flameDrips, 0, sizeof(flameDrips));
    Flame_List_Init((flameGeneric_s *)flameDrips, 100, 250);
    flameDripsFree = flameDrips;
    flameDripsUsed = 0;

    memset(sv_flameDrips, 0, sizeof(sv_flameDrips));
    Flame_List_Init((flameGeneric_s*)sv_flameDrips, 100, 250);
    sv_flameDripsFree = sv_flameDrips;
    sv_flameDripsUsed = 0;

    ++g_DripsCount;
}

flameDrips_t *__cdecl Flame_Class_Drips_Alloc(bool is_server)
{
    flameDrips_t *freeDrips;

    if ( is_server )
        freeDrips = sv_flameDripsFree;
    else
        freeDrips = flameDripsFree;
    if ( !freeDrips )
        return 0;
    if ( is_server )
    {
        Flame_List_Move_Global(
            &freeDrips->gen,
            (flameGeneric_s **)&sv_flameDripsFree,
            (flameGeneric_s **)&sv_flameDripsUsed);
    }
    else
    {
        Flame_List_Move_Global(&freeDrips->gen, (flameGeneric_s **)&flameDripsFree, (flameGeneric_s **)&flameDripsUsed);
        if ( ++g_DripsCount > g_DripsCountWaterMark )
            g_DripsCountWaterMark = g_DripsCount;
    }
    return freeDrips;
}

void __cdecl Flame_Class_Drips_Free(bool is_server, flameDrips_t *drip)
{
    if ( drip->flameRend )
        Flame_List_Sub_Local(&drip->gen, &drip->flameRend->dripsList);
    if ( is_server )
    {
        Flame_List_Move_Global(&drip->gen, (flameGeneric_s **)&sv_flameDripsUsed, (flameGeneric_s **)&sv_flameDripsFree);
    }
    else
    {
        Flame_List_Move_Global(&drip->gen, (flameGeneric_s **)&flameDripsUsed, (flameGeneric_s **)&flameDripsFree);
        --g_DripsCount;
    }
    Flame_Item_Init(&drip->gen, sizeof(flameDrips_t));
}

void __cdecl Flame_Class_Drips_Free_If_Stream(bool is_server, flameStream_s *stream)
{
    flameDrips_t *next;
    flameDrips_t *drip;

    if ( is_server )
        drip = sv_flameDripsUsed;
    else
        drip = flameDripsUsed;
    while ( drip )
    {
        next = (flameDrips_t *)drip->gen.listGlobal.next;
        if ( drip->gen.stream == stream )
            Flame_Class_Drips_Free(is_server, drip);
        drip = next;
    }
}

void __cdecl Flame_Class_Drips_Age(bool is_server, int time)
{
    phys_static_array<flameGeneric_s *,1000> *flamesArray;
    flameDrips_t *next;
    flameDrips_t *drip;

    if ( is_server )
        drip = sv_flameDripsUsed;
    else
        drip = flameDripsUsed;
    while ( drip )
    {
        next = (flameDrips_t *)drip->gen.listGlobal.next;
        if ( drip->gen.age.lastUpdateTime < drip->gen.age.endTime )
        {
            if ( is_server )
                flamesArray = &sv_flames;
            else
                flamesArray = &cl_flames;
            Flame_Phys_Update_Item_Drip(&drip->gen, time, flamesArray);
        }
        if ( drip->gen.age.lastUpdateTime >= drip->gen.age.endTime )
            Flame_Class_Drips_Free(is_server, drip);
        drip = next;
    }
    Flame_Phys_Update_Items(is_server);
}

flameDrips_t *__cdecl Flame_Class_Drips_Spawn(bool is_server, flameChunk_s *fromChunk, int curTime, float minLifeFrac)
{
    float speedScaleBase;
    float lifeFrac;
    float sidewaysScale;
    flameDrips_t *drip;
    flameRender_s *flameRend;
    flameTable *flameVars;
    float speed;
    float lifetime;
    float velY;
    float velX;

    flameRend = 0;
    iassert(fromChunk);
    if ( !fromChunk || !fromChunk->gen.stream )
        return 0;
    drip = Flame_Class_Drips_Alloc(is_server);
    if ( !drip )
        return 0;
    flameVars = fromChunk->gen.stream->flameVars;
    if ( !is_server )
        flameRend = fromChunk->gen.stream->renderList->flameRend;
    drip->gen.type = 4;
    drip->gen.stream = fromChunk->gen.stream;
    if ( (float)((float)(fromChunk->gen.age.lastUpdateTime - fromChunk->gen.age.startTime)
                         / (float)(fromChunk->gen.age.endTime - fromChunk->gen.age.startTime)) <= minLifeFrac )
        lifeFrac = minLifeFrac;
    else
        lifeFrac = (float)(fromChunk->gen.age.lastUpdateTime - fromChunk->gen.age.startTime)
             / (float)(fromChunk->gen.age.endTime - fromChunk->gen.age.startTime);
    lifetime = Flame_Random(is_server) * flameVars->flameVar_dripsLifeRand + flameVars->flameVar_dripsLife;
    drip->gen.age.startTime = curTime - (int)(float)((float)(lifeFrac * lifetime) * 1000.0);
    drip->gen.age.endTime = drip->gen.age.startTime + (int)(float)(1000.0 * lifetime);
    drip->gen.age.lastUpdateTime = curTime;
    drip->gen.size.current = fromChunk->gen.size.current * flameVars->flameVar_dripsStartSizeScale;
    drip->gen.size.rate = 1.0f;
    drip->gen.phys.origin[0] = fromChunk->gen.phys.origin[0];
    drip->gen.phys.origin[1] = fromChunk->gen.phys.origin[1];
    drip->gen.phys.origin[2] = fromChunk->gen.phys.origin[2];
    speedScaleBase = 1.0f - flameVars->flameVar_dripsSpeedScaleRand;
    speed = (Flame_Random(is_server) * flameVars->flameVar_dripsSpeedScaleRand + speedScaleBase) * flameVars->flameVar_dripsSpeedScale;
    drip->gen.phys.velocity[0] = speed * fromChunk->gen.phys.velocity[0];
    drip->gen.phys.velocity[1] = speed * fromChunk->gen.phys.velocity[1];
    drip->gen.phys.velocity[2] = speed * fromChunk->gen.phys.velocity[2];
    velY = drip->gen.phys.velocity[1];
    velX = drip->gen.phys.velocity[0];
    sidewaysScale = Flame_CRandom(is_server) * flameVars->flameVar_dripsVelocityAddSideways;
    drip->gen.phys.velocity[0] = sidewaysScale * velY + drip->gen.phys.velocity[0];
    drip->gen.phys.velocity[1] = sidewaysScale * velX + drip->gen.phys.velocity[1];
    drip->gen.phys.velocity[2] = Flame_Random(is_server) * flameVars->flameVar_dripsVelocityAddZRand
                                                         + flameVars->flameVar_dripsVelocityAddZ
                                                         + drip->gen.phys.velocity[2];
    drip->gen.phys.invStartSpeed = Flame_CalcInvStartSpeed(
                                                                     fromChunk->gen.phys.invStartSpeed,
                                                                     flameVars->flameVar_dripsSpeedScale);
    drip->gen.phys.gravity = flameVars->flameVar_dripsGravity;
    drip->gen.phys.rotation = fromChunk->gen.phys.rotation;
    drip->gen.phys.rotVel = Flame_CRandom(is_server) * flameVars->flameVar_dripsMaxRotVel;
    drip->flameRend = flameRend;
    if ( !is_server )
        Flame_List_Add_Local(&drip->gen, &drip->flameRend->dripsList);
    return drip;
}

void __cdecl Flame_Class_Drips_Render_Item(int localClientNum, flameDrips_t *drip)
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
        angles[2] = drip->gen.phys.rotation;
        AngleVectors(angles, fwd, right, down);
        halfSize = drip->gen.size.current * 0.5f;
        right[0] = halfSize * right[0];
        right[1] = halfSize * right[1];
        right[2] = halfSize * right[2];
        negHalfSize = drip->gen.size.current * -0.5f;
        down[0] = negHalfSize * down[0];
        down[1] = negHalfSize * down[1];
        down[2] = negHalfSize * down[2];
        start[0] = points[3][0] * right[0] + drip->gen.phys.origin[0];
        start[1] = points[3][0] * right[1] + drip->gen.phys.origin[1];
        start[2] = points[3][0] * right[2] + drip->gen.phys.origin[2];
        start[0] = points[3][1] * down[0] + start[0];
        start[1] = points[3][1] * down[1] + start[1];
        start[2] = points[3][1] * down[2] + start[2];
        for ( cornerIndex = 0; cornerIndex < 4; ++cornerIndex )
        {
            pointOffsetRight = points[cornerIndex][0];
            end[0] = pointOffsetRight * right[0] + drip->gen.phys.origin[0];
            end[1] = pointOffsetRight * right[1] + drip->gen.phys.origin[1];
            end[2] = pointOffsetRight * right[2] + drip->gen.phys.origin[2];
            pointOffsetDown = points[cornerIndex][1];
            end[0] = pointOffsetDown * down[0] + end[0];
            end[1] = pointOffsetDown * down[1] + end[1];
            end[2] = pointOffsetDown * down[2] + end[2];
            CG_DebugLine(start, end, colorMagenta, 1, 1);
            start[0] = end[0];
            start[1] = end[1];
            start[2] = end[2];
        }
    }
}

void __cdecl Flame_Class_Drips_Render_All(int localClientNum)
{
    flameDrips_t *drip;

    if ( flame_debug_render->current.integer > 0 )
    {
        for ( drip = flameDripsUsed; drip; drip = (flameDrips_t *)drip->gen.listGlobal.next )
            Flame_Class_Drips_Render_Item(localClientNum, drip);
    }
}

void __cdecl Flame_Class_Drips_Render_Local_List(int localClientNum, flameDrips_t *start)
{
    flameDrips_t *drip;
    cg_s *clientGlobals;
    flameGeneric_s *renderList[250];
    int count;

    clientGlobals = CG_GetLocalClientGlobals(localClientNum);
    iassert(clientGlobals);

    if ( (!start || start->gen.stream) && (!start || Flame_GetLocalClientSourceRange()) )
    {
        drip = start;
        count = 0;
        while ( count < 250 && drip )
        {
            renderList[count++] = &drip->gen;
            drip = (flameDrips_t *)drip->gen.listLocal.next;
        }
        // CoDMPServer.c:704207
        if ( count > 1 )
            Flame_Render_Sprites(
                clientGlobals,
                start->gen.stream->flameVars->drips,
                renderList,
                count);
    }
}
