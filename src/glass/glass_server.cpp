#include "glass_server.h"
#include <game_mp/g_main_mp.h>
#include <game_mp/g_utils_mp.h>
#include "glass_load_obj.h"
#include <server_mp/sv_main_mp.h>
#include <universal/com_math_anglevectors.h>
#include <qcommon/cm_tracebox.h>
#include <game_mp/g_spawn_mp.h>
#include <clientscript/scr_const.h>
#include <server/sv_game.h>
#include <physics/phys_main.h>
#include <flame/flame_system.h>
#include <universal/com_math.h>

GlassesServer svGlasses;

static const float glassPredictTime = 0.15f;

static bool GlassModIsInstantBreak(int mod)
{
    return mod == 3 || mod == 5 || mod == 7 || mod == 8 || mod == 17 || mod == 16;
}

// Decomp: CoDMPServer.c:859672
void __thiscall GlassServer::SetState(GlassState::State st, float *pos, float *dir)
{
    if (st != (this->state.val.i & 0xF))
    {
        this->stateChangeTime = level.time;
        if (st == GlassState::State::CRACKED)
        {
            this->crackedDamageRate = G_flrand(
                svGlasses.crackedDamageRateRange->current.value,
                svGlasses.crackedDamageRateRange->current.vector[1]);
        }
        else if (st == GlassState::State::SHATTERED)
        {
            Scr_GlassSmash(pos, dir);
            this->hitPos[0] = pos[0];
            this->hitPos[1] = pos[1];
            this->hitPos[2] = pos[2];
            this->hitDir[0] = dir[0];
            this->hitDir[1] = dir[1];
            this->hitDir[2] = dir[2];
        }
        this->state.val.i = (st & 0xF) | (this->state.val.i & 0xFFFFFFF0);
    }
}

cmd_function_s ShatterAllCmd_VAR;
cmd_function_s ResetAllCmd_VAR;

// Decomp: CoDMPServer.c:859705
void __thiscall GlassesServer::Init()
{
    unsigned int glassIndex;
    Glasses *mapGlasses;

    if (!this->inited)
    {
        Cmd_AddCommandInternal("shatterAllGlass", GlassesServer::ShatterAllCmd, &ShatterAllCmd_VAR);
        Cmd_AddCommandInternal("resetGlass", GlassesServer::ResetAllCmd, &ResetAllCmd_VAR);
        this->inited = true;
        this->damageMultiplier = _Dvar_RegisterFloat(
            "glassDamageMultiplier",
            1.0f,
            0.1f,
            10.0f,
            0,
            "Multiplier the damage applied to the glass (server).");
        this->crackedDamageRateRange = _Dvar_RegisterVec2(
            "glassCrackedDamageRateRange",
            1.0f,
            10.0f,
            0.0f,
            10000.0f,
            0,
            "Range of the rate a cracked glass looses health points");
    }
    if (!this->numGlasses)
    {
        mapGlasses = GetGlasses();
        if (mapGlasses)
        {
            if (mapGlasses->numGlasses >= 0x3E8
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_server.cpp",
                    136,
                    0,
                    "%s",
                    "glss->numGlasses < MAX_GLASSES"))
            {
                __debugbreak();
            }
            this->numGlasses = mapGlasses->numGlasses;
            for (glassIndex = 0; glassIndex < this->numGlasses; ++glassIndex)
            {
                this->glasses[glassIndex].glass = &mapGlasses->glasses[glassIndex];
                this->glasses[glassIndex].state.val.i = (16 * (glassIndex & 0xFFF))
                    | (this->glasses[glassIndex].state.val.i & 0xFFFF000F);
                this->glasses[glassIndex].SetState(GlassState::State::PRISTINE, 0, 0);
                this->glasses[glassIndex].health = (float)this->glasses[glassIndex].glass->glassDef->maxHealth;
                this->glasses[glassIndex].stateChangeTime = 0;
            }
        }
        else
        {
            this->numGlasses = 0;
        }
    }
    this->nextShatterTime = 0;
}

// Decomp: CoDMPServer.c:859949
void __thiscall GlassesServer::Shutdown()
{
    this->numGlasses = 0;
}

// Decomp: CoDMPServer.c:859773
void __thiscall GlassesServer::Update()
{
    float yaw;
    float pitch;
    float angles[3];
    float axis[3][3];
    unsigned int glassId;
    int toShatter;
    unsigned int glassIndex;
    int nonShatteredCount;
    unsigned int crackedIndex;

    if (!this->numGlasses)
    {
        return;
    }

    PROF_SCOPED("GlassesServer.Update");

    for (crackedIndex = 0; crackedIndex < this->numGlasses; ++crackedIndex)
    {
        if ((this->glasses[crackedIndex].state.val.i & 0xF) == 1)
        {
            this->glasses[crackedIndex].health -= this->glasses[crackedIndex].crackedDamageRate
                * this->damageMultiplier->current.value
                * (float)level.frametime
                * 0.001f;
            if (this->glasses[crackedIndex].health < 0.0f)
            {
                this->glasses[crackedIndex].SetState(
                    GlassState::State::SHATTERED,
                    (float *)vec3_origin,
                    (float *)vec3_origin);
            }
        }
    }
    while (this->nextShatterTime > 0 && svs.time > this->nextShatterTime)
    {
        nonShatteredCount = 0;
        for (glassIndex = 0; glassIndex < this->numGlasses; ++glassIndex)
        {
            if ((this->glasses[glassIndex].state.val.i & 0xF) != 2)
            {
                ++nonShatteredCount;
            }
        }
        if (nonShatteredCount <= 0)
        {
            this->nextShatterTime = 0;
        }
        else
        {
            toShatter = G_irand(0, nonShatteredCount);
            for (glassId = 0; glassId < this->numGlasses; ++glassId)
            {
                if ((this->glasses[glassId].state.val.i & 0xF) != 2)
                {
                    if (!toShatter--)
                    {
                        yaw = G_flrand(0.0f, 360.0f);
                        pitch = G_flrand(-90.0f, 90.0f);
                        angles[0] = yaw;
                        angles[1] = pitch;
                        angles[2] = 0.0f;
                        AnglesToAxis(angles, axis);
                        if (G_flrand(0.0f, 1.0f) <= 0.5f)
                        {
                            GlassSv_Damage(
                                glassId,
                                1,
                                2,
                                this->glasses[glassId].glass->origin,
                                axis[0]);
                        }
                        else
                        {
                            GlassSv_Damage(
                                glassId,
                                1,
                                3,
                                this->glasses[glassId].glass->origin,
                                axis[0]);
                        }
                        break;
                    }
                }
            }
            this->nextShatterTime += this->shatterIntervalsMS;
        }
    }
}

// Decomp: CoDMPServer.c:859858
void __cdecl GlassesServer::ShatterAllCmd()
{
    svGlasses.ShatterAll();
}

// Decomp: CoDMPServer.c:859864
void __thiscall GlassesServer::ShatterAll()
{
    const char *intervalArg;
    int shatterIntervalMs;
    float yaw;
    float pitch;
    float angles[3];
    float axis[3][3];
    unsigned int glassIndex;
    int shatterCount;
    const int maxShatters = 100;

    if (Cmd_Argc() <= 1)
    {
        shatterCount = 0;
        for (glassIndex = 0; glassIndex < this->numGlasses; ++glassIndex)
        {
            if ((this->glasses[glassIndex].state.val.i & 0xF) != 2)
            {
                yaw = G_flrand(0.0f, 360.0f);
                pitch = G_flrand(-90.0f, 90.0f);
                angles[0] = yaw;
                angles[1] = pitch;
                angles[2] = 0.0f;
                AnglesToAxis(angles, axis);
                GlassSv_Damage(
                    glassIndex,
                    1,
                    3,
                    g_entities[glassIndex].s.lerp.pos.trBase,
                    axis[0]);
                if (++shatterCount == maxShatters)
                {
                    break;
                }
            }
        }
    }
    else
    {
        intervalArg = Cmd_Argv(1);
        shatterIntervalMs = atoi(intervalArg);
        if (shatterIntervalMs < 10)
        {
            shatterIntervalMs = 10;
        }
        this->shatterIntervalsMS = shatterIntervalMs;
        this->nextShatterTime = svs.time;
    }
}

// Decomp: CoDMPServer.c:859918
void __cdecl GlassesServer::ResetAllCmd()
{
    svGlasses.ResetAll();
}

// Decomp: CoDMPServer.c:859924
void __thiscall GlassesServer::ResetAll()
{
    unsigned int glassIndex;

    for (glassIndex = 0; glassIndex < this->numGlasses; ++glassIndex)
    {
        if ((this->glasses[glassIndex].state.val.i & 0xF) != 0)
        {
            this->glasses[glassIndex].SetState(GlassState::State::PRISTINE, 0, 0);
            this->glasses[glassIndex].health = (float)this->glasses[glassIndex].glass->glassDef->maxHealth;
        }
    }
}

// Decomp: CoDMPServer.c:859941
void __cdecl GlassSv_Init()
{
    svGlasses.Init();
}

// Decomp: CoDMPServer.c:859947
void __cdecl GlassSv_Shutdown()
{
    svGlasses.Shutdown();
}

// Decomp: CoDMPServer.c:859953
void __cdecl GlassSv_Update()
{
    svGlasses.Update();
}

// Decomp: CoDMPServer.c:859959
unsigned int __cdecl GlassSv_AreaGlasses(
    const float *mins,
    const float *maxs,
    const Glass **glasses,
    unsigned int maxGlasses)
{
    const GlassServer *glassServer;
    unsigned int glassIndex;
    unsigned int num;

    num = 0;
    if (!svGlasses.numGlasses)
    {
        return num;
    }

    for (glassIndex = 0; glassIndex < svGlasses.numGlasses && num < maxGlasses; ++glassIndex)
    {
        glassServer = &svGlasses.glasses[glassIndex];
        if ((glassServer->state.val.i & 0xF) != 2
            && maxs[0] >= glassServer->glass->absmin[0]
            && maxs[1] >= glassServer->glass->absmin[1]
            && maxs[2] >= glassServer->glass->absmin[2]
            && glassServer->glass->absmax[0] >= mins[0]
            && glassServer->glass->absmax[1] >= mins[1]
            && glassServer->glass->absmax[2] >= mins[2])
        {
            glasses[num++] = glassServer->glass;
        }
    }
    return num;
}

// Decomp: CoDMPServer.c:859989
void __cdecl GlassSv_ClipMoveTrace(const moveclip_t *clip, trace_t *results)
{
    float oldFraction;
    float clipMax;
    float clipMin;
    unsigned __int16 glassIndex;
    const Glass *glass;
    unsigned int areaGlassIndex;
    int axisIndex;
    float mins[3];
    const Glass *areaGlasses[128];
    float maxs[3];
    unsigned int numAreaGlasses;

    if ((clip->contentmask & 0x10) == 0 || !svGlasses.numGlasses)
    {
        return;
    }

    PROF_SCOPED("GlassSv_ClipMoveTrace");

    for (axisIndex = 0; axisIndex < 3; ++axisIndex)
    {
        if (clip->extents.end.vec.v[axisIndex] - clip->extents.start.vec.v[axisIndex] < 0.0f)
        {
            mins[axisIndex] = clip->extents.end.vec.v[axisIndex];
        }
        else
        {
            mins[axisIndex] = clip->extents.start.vec.v[axisIndex];
        }
        clipMin = clip->extents.start.vec.v[axisIndex] + clip->mins[axisIndex];
        if (clipMin - mins[axisIndex] < 0.0f)
        {
            mins[axisIndex] = clipMin;
        }
        else
        {
            mins[axisIndex] = mins[axisIndex];
        }
        clipMin = clip->extents.end.vec.v[axisIndex] + clip->mins[axisIndex];
        if (clipMin - mins[axisIndex] < 0.0f)
        {
            mins[axisIndex] = clipMin;
        }
        if (clip->extents.start.vec.v[axisIndex] - clip->extents.end.vec.v[axisIndex] < 0.0f)
        {
            maxs[axisIndex] = clip->extents.end.vec.v[axisIndex];
        }
        else
        {
            maxs[axisIndex] = clip->extents.start.vec.v[axisIndex];
        }
        clipMax = clip->extents.start.vec.v[axisIndex] + clip->maxs[axisIndex];
        if (maxs[axisIndex] - clipMax < 0.0f)
        {
            maxs[axisIndex] = clipMax;
        }
        clipMax = clip->extents.end.vec.v[axisIndex] + clip->maxs[axisIndex];
        if (maxs[axisIndex] - clipMax < 0.0f)
        {
            maxs[axisIndex] = clipMax;
        }
    }
    numAreaGlasses = GlassSv_AreaGlasses(mins, maxs, areaGlasses, 0x80u);
    for (areaGlassIndex = 0; areaGlassIndex < numAreaGlasses; ++areaGlassIndex)
    {
        glass = areaGlasses[areaGlassIndex];
        oldFraction = results->fraction;
        CM_TransformedBoxTrace(
            results,
            clip->extents.start.vec.v,
            clip->extents.end.vec.v,
            clip->mins,
            clip->maxs,
            glass->brushModel,
            clip->contentmask,
            glass->origin,
            glass->angles);
        if (results->fraction < oldFraction)
        {
            glassIndex = glass->index;
            if (!results
                && !Assert_MyHandler(
                    "c:\\projects_pc\\cod\\codsrc\\src\\physics\\../qcommon/cm_public.h",
                    175,
                    0,
                    "%s",
                    "trace"))
            {
                __debugbreak();
            }
            results->hitType = TRACE_HITTYPE_GLASS;
            results->hitId = glassIndex;
        }
    }
}

// Decomp: CoDMPServer.c:860084
void __cdecl GlassSv_PointTrace(const pointtrace_t *clip, trace_t *results)
{
    float oldFraction;
    const GlassServer *glassServer;
    unsigned int glassIndex;

    if ((clip->contentmask & 0x10) == 0 || !svGlasses.numGlasses)
    {
        return;
    }

    PROF_SCOPED("GlassSv_PointTrace");

    for (glassIndex = 0; glassIndex < svGlasses.numGlasses; ++glassIndex)
    {
        glassServer = &svGlasses.glasses[glassIndex];
        if ((glassServer->state.val.i & 0xF) == 2)
        {
            continue;
        }
        if (!intersect_extents_aabb(&clip->extents, glassServer->glass->absmin, glassServer->glass->absmax, results->fraction))
        {
            continue;
        }
        oldFraction = results->fraction;
        CM_TransformedBoxTrace(
            results,
            clip->extents.start.vec.v,
            clip->extents.end.vec.v,
            vec3_origin,
            vec3_origin,
            glassServer->glass->brushModel,
            clip->contentmask,
            glassServer->glass->origin,
            glassServer->glass->angles);
        if (results->fraction < oldFraction)
        {
            results->modelIndex = 0;
            results->partName = 0;
            results->boneIndex = 0;
            results->partGroup = 0;
            if (!results
                && !Assert_MyHandler(
                    "c:\\projects_pc\\cod\\codsrc\\src\\physics\\../qcommon/cm_public.h",
                    175,
                    0,
                    "%s",
                    "trace"))
            {
                __debugbreak();
            }
            results->hitType = TRACE_HITTYPE_GLASS;
            results->hitId = glassIndex;
            results->cflags = 16;
        }
    }
}

// Decomp: CoDMPServer.c:860143
void __cdecl GlassSv_Damage(unsigned int glassId, int damage, int mod, float *pos, float *dir)
{
    GlassServer *glass;
    const float minDamage = 5.0f * svGlasses.damageMultiplier->current.value;

    PROF_SCOPED("GlassSv_Damage");

    if (glassId >= svGlasses.numGlasses
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_server.cpp",
            646,
            0,
            "%s",
            "glassId < svGlasses.NumGlasses()"))
    {
        __debugbreak();
    }
    glass = &svGlasses.glasses[glassId];
    if (glass->health <= 0.0f)
    {
        return;
    }
    if ((float)damage < minDamage && !GlassModIsInstantBreak(mod))
    {
        return;
    }
    if (GlassModIsInstantBreak(mod))
    {
        glass->health = 0.0f;
    }
    else
    {
        glass->health -= (float)(int)((float)damage * svGlasses.damageMultiplier->current.value);
    }
    if (glass->health <= 0.0f)
    {
        glass->SetState(GlassState::State::SHATTERED, pos, dir);
    }
    else
    {
        glass->SetState(GlassState::State::CRACKED, 0, 0);
    }
}

// Decomp: CoDMPServer.c:860187
void __cdecl GlassSv_RadiusDamage(
    float *origin,
    float radius,
    float coneAngleCos,
    float *coneDirection,
    float innerDamage,
    float outerDamage,
    int mod)
{
    const float *glassOrigin;
    float distSqr;
    float damage;
    float dirScale;
    float dir[3];
    float dist;
    unsigned int glassIndex;
    float radiusMaxs[3];
    const unsigned int maxClosests = 100;
    float radiusSqr;
    float radiusMins[3];
    unsigned int num;
    const Glass *closest[100];
    const float radiusPad = 1.4142135f * radius;

    if (!svGlasses.numGlasses)
    {
        return;
    }

    PROF_SCOPED("GlassSv_RadiusDamage");

    radiusSqr = radius * radius;
    radiusMins[0] = origin[0] - radiusPad;
    radiusMins[1] = origin[1] - radiusPad;
    radiusMins[2] = origin[2] - radiusPad;
    radiusMaxs[0] = origin[0] + radiusPad;
    radiusMaxs[1] = origin[1] + radiusPad;
    radiusMaxs[2] = origin[2] + radiusPad;
    num = GlassSv_AreaGlasses(radiusMins, radiusMaxs, closest, maxClosests);
    for (glassIndex = 0; glassIndex < num; ++glassIndex)
    {
        glassOrigin = closest[glassIndex]->origin;
        dir[0] = glassOrigin[0] - origin[0];
        dir[1] = glassOrigin[1] - origin[1];
        dir[2] = glassOrigin[2] - origin[2];
        distSqr = dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2];
        if (distSqr >= radiusSqr)
        {
            continue;
        }
        dist = sqrtf(distSqr);
        if (dist >= 0.0099999998f)
        {
            const float invDist = 1.0f / dist;
            dir[0] *= invDist;
            dir[1] *= invDist;
            dir[2] *= invDist;
        }
        else
        {
            dir[0] = 0.0f;
            dir[1] = 0.0f;
            dir[2] = 1.0f;
        }
        if (coneDirection
            && dist > 0.0099999998f
            && coneAngleCos > dir[0] * coneDirection[0] + dir[1] * coneDirection[1] + dir[2] * coneDirection[2])
        {
            continue;
        }
        damage = (innerDamage - outerDamage) * (1.0f - dist / radius) + outerDamage;
        dirScale = damage / (float)closest[glassIndex]->glassDef->maxHealth + 1.0f;
        dir[0] *= dirScale;
        dir[1] *= dirScale;
        dir[2] *= dirScale;
        GlassSv_Damage(closest[glassIndex]->index, (int)damage, mod, origin, dir);
    }
}

// Decomp: CoDMPServer.c:860269
void __cdecl GlassSv_Touch(unsigned int glassId, gentity_s *other)
{
    float physVel[3];
    float angVel[3];
    float actorVel[3];
    float playerVel[3];
    const vehicle_physic_t *phys;
    float dir[3];
    const scr_vehicle_s *veh;

    if (other->s.eType == 14)
    {
        veh = other->scr_vehicle;
        phys = &veh->phys;
        Vec3NormalizeTo(veh->phys.vel, dir);
        GlassSv_Damage(glassId, 5000, 16, (float *)phys->origin, dir);
    }
    else if (other->s.eType == 1 && (other->client->ps.pm_flags & 0x400000) != 0)
    {
        Vec3NormalizeTo(other->client->ps.velocity, playerVel);
        GlassSv_Damage(glassId, 5000, 16, other->client->ps.origin, playerVel);
        Scr_Notify(other, scr_const.dtp_through_glass, 0);
    }
    else if (other->actor && Flame_GetLocalClientSourceRange())
    {
        Vec3NormalizeTo(other->actor->Physics.vVelocity, actorVel);
        GlassSv_Damage(glassId, 5000, 16, other->actor->Physics.vOrigin, actorVel);
    }
    else if (other->s.eType == 6 && other->physObjId)
    {
        Phys_ObjGetVelocities(other->physObjId, physVel, angVel);
        Vec3NormalizeTo(physVel, physVel);
        GlassSv_Damage(glassId, 5000, 16, other->r.currentOrigin, physVel);
    }
}

// Decomp: CoDMPServer.c:860311
void __cdecl GlassSv_PredictTouch(gentity_s *other)
{
    const Glass *glass;
    float invAxis[4][3];
    float localVel[3];
    float localOrigin[3];
    float contactMins[3];
    float contactMaxs[3];
    float velOffset[3];
    float predictedMins[3];
    float predictedMaxs[3];
    float tvel[3];
    float avel[3];
    unsigned int glassIndex;
    const Glass *areaGlasses[1025];
    unsigned int numAreaGlasses;

    iassert(other);
    if (!svGlasses.numGlasses)
    {
        return;
    }

    if (other->physObjId)
    {
        Phys_ObjGetVelocities(other->physObjId, tvel, avel);
    }
    else if (other->scr_vehicle)
    {
        tvel[0] = other->scr_vehicle->phys.vel[0];
        tvel[1] = other->scr_vehicle->phys.vel[1];
        tvel[2] = other->scr_vehicle->phys.vel[2];
    }
    else
    {
        if (!other->actor || !Flame_GetLocalClientSourceRange())
        {
            return;
        }
        tvel[0] = other->actor->Physics.vVelocity[0];
        tvel[1] = other->actor->Physics.vVelocity[1];
        tvel[2] = other->actor->Physics.vVelocity[2];
    }

    PROF_SCOPED("GlassSv_PredictTouch");

    predictedMaxs[0] = other->r.absmax[0];
    predictedMaxs[1] = other->r.absmax[1];
    predictedMaxs[2] = other->r.absmax[2];
    predictedMins[0] = other->r.absmin[0];
    predictedMins[1] = other->r.absmin[1];
    predictedMins[2] = other->r.absmin[2];
    velOffset[0] = glassPredictTime * tvel[0];
    velOffset[1] = glassPredictTime * tvel[1];
    velOffset[2] = glassPredictTime * tvel[2];
    predictedMins[0] += velOffset[0];
    predictedMins[1] += velOffset[1];
    predictedMins[2] += velOffset[2];
    predictedMaxs[0] += velOffset[0];
    predictedMaxs[1] += velOffset[1];
    predictedMaxs[2] += velOffset[2];
    numAreaGlasses = GlassSv_AreaGlasses(predictedMins, predictedMaxs, areaGlasses, 0x400u);
    for (glassIndex = 0; glassIndex < numAreaGlasses; ++glassIndex)
    {
        glass = areaGlasses[glassIndex];
        contactMins[0] = glass->absmin[0];
        contactMins[1] = glass->absmin[1];
        contactMins[2] = glass->absmin[2];
        contactMaxs[0] = glass->absmax[0];
        contactMaxs[1] = glass->absmax[1];
        contactMaxs[2] = glass->absmax[2];
        ExpandBoundsToWidth(&contactMins[0], &contactMaxs[0]);
        contactMins[0] -= velOffset[0];
        contactMins[1] -= velOffset[1];
        contactMins[2] -= velOffset[2];
        contactMaxs[0] -= velOffset[0];
        contactMaxs[1] -= velOffset[1];
        contactMaxs[2] -= velOffset[2];
        if (!SV_EntityContact(contactMins, contactMaxs, other))
        {
            continue;
        }

        MatrixInverseOrthogonal43(glass->outlineAxis, invAxis);
        localVel[0] = tvel[0] * invAxis[0][0] + tvel[1] * invAxis[1][0] + tvel[2] * invAxis[2][0];
        localVel[1] = tvel[0] * invAxis[0][1] + tvel[1] * invAxis[1][1] + tvel[2] * invAxis[2][1];
        localVel[2] = tvel[0] * invAxis[0][2] + tvel[1] * invAxis[1][2] + tvel[2] * invAxis[2][2];

        localOrigin[0] = other->r.currentOrigin[0] * invAxis[0][0]
            + other->r.currentOrigin[1] * invAxis[1][0]
            + other->r.currentOrigin[2] * invAxis[2][0]
            + invAxis[3][0];
        localOrigin[1] = other->r.currentOrigin[0] * invAxis[0][1]
            + other->r.currentOrigin[1] * invAxis[1][1]
            + other->r.currentOrigin[2] * invAxis[2][1]
            + invAxis[3][1];
        localOrigin[2] = other->r.currentOrigin[0] * invAxis[0][2]
            + other->r.currentOrigin[1] * invAxis[1][2]
            + other->r.currentOrigin[2] * invAxis[2][2]
            + invAxis[3][2];

        if (localOrigin[2] * localVel[2] < 0.0f && fabs(localVel[2]) > 100.0f)
        {
            GlassSv_Touch(glass->index, other);
        }
    }
}

// Decomp: CoDMPServer.c:860455
void __thiscall GlassesServer::WriteSnapshotToClient(msg_t *msg, int sinceTime)
{
    unsigned int glassIndex;
    unsigned int snapshotGlassIndex;
    unsigned int numPositions;
    unsigned int numShattered;
    bool sendFull;
    unsigned int numChanges;
    unsigned int numGlasses;

    numGlasses = this->numGlasses;
    if (!numGlasses)
    {
        return;
    }

    PROF_SCOPED("GlassesServer::WriteSnapshotToClient");

    sendFull = true;
    numChanges = 0;
    if (sinceTime > 0)
    {
        numPositions = 0;
        numShattered = 0;
        for (glassIndex = 0; glassIndex < numGlasses; ++glassIndex)
        {
            if ((this->glasses[glassIndex].state.val.i & 0xF) == 2)
            {
                ++numShattered;
            }
            if (this->glasses[glassIndex].stateChangeTime >= sinceTime)
            {
                ++numChanges;
                if ((this->glasses[glassIndex].state.val.i & 0xF) == 2)
                {
                    ++numPositions;
                }
            }
        }
        if (!numChanges)
        {
            MSG_WriteBit0(msg);
            MSG_WriteShort(msg, 30154);
            return;
        }
        if (192 * numShattered + 2 * numGlasses > 192 * numPositions + 12 * numChanges)
        {
            sendFull = false;
        }
    }
    MSG_WriteBit1(msg);
    if (sendFull)
    {
        MSG_WriteBit1(msg);
        MSG_WriteShort(msg, numGlasses);
        for (snapshotGlassIndex = 0; snapshotGlassIndex < numGlasses; ++snapshotGlassIndex)
        {
            MSG_WriteBits(msg, this->glasses[snapshotGlassIndex].state.val.i & 0xF, 2u);
            if ((this->glasses[snapshotGlassIndex].state.val.i & 0xF) == 2)
            {
                MSG_WriteFloat(msg, this->glasses[snapshotGlassIndex].hitPos[0]);
                MSG_WriteFloat(msg, this->glasses[snapshotGlassIndex].hitPos[1]);
                MSG_WriteFloat(msg, this->glasses[snapshotGlassIndex].hitPos[2]);
                MSG_WriteFloat(msg, this->glasses[snapshotGlassIndex].hitDir[0]);
                MSG_WriteFloat(msg, this->glasses[snapshotGlassIndex].hitDir[1]);
                MSG_WriteFloat(msg, this->glasses[snapshotGlassIndex].hitDir[2]);
            }
        }
    }
    else
    {
        MSG_WriteBit0(msg);
        MSG_WriteShort(msg, numChanges);
        for (glassIndex = 0; glassIndex < numGlasses; ++glassIndex)
        {
            if (this->glasses[glassIndex].stateChangeTime < sinceTime)
            {
                continue;
            }
            MSG_WriteBits(msg, glassIndex, 10u);
            MSG_WriteBits(msg, this->glasses[glassIndex].state.val.i & 0xF, 2u);
            if ((this->glasses[glassIndex].state.val.i & 0xF) == 2)
            {
                MSG_WriteFloat(msg, this->glasses[glassIndex].hitPos[0]);
                MSG_WriteFloat(msg, this->glasses[glassIndex].hitPos[1]);
                MSG_WriteFloat(msg, this->glasses[glassIndex].hitPos[2]);
                MSG_WriteFloat(msg, this->glasses[glassIndex].hitDir[0]);
                MSG_WriteFloat(msg, this->glasses[glassIndex].hitDir[1]);
                MSG_WriteFloat(msg, this->glasses[glassIndex].hitDir[2]);
            }
        }
    }
    MSG_WriteShort(msg, 30154);
}

// Decomp: CoDMPServer.c:860550
void __cdecl GlassSv_WriteSnapshotToClient(msg_t *msg, int sinceTime)
{
    svGlasses.WriteSnapshotToClient(msg, sinceTime);
}

// Decomp: CoDMPServer.c:860556
void __cdecl GlassSv_WriteGameState(msg_t *msg)
{
    GlassSv_WriteSnapshotToClient(msg, 0);
}
