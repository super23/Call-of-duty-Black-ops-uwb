#include <game_mp/g_spawnsystem_mp.h>
#include "g_main_sp.h"
#include <cstring>

static const int SP_MAX_SPAWN_INFLUENCERS = 328;

static SpawnInfluencer s_spSpawnInfluencers[SP_MAX_SPAWN_INFLUENCERS];
static SpawnInfluencerPreset s_spSpawnInfluencerPresets[SP_MAX_SPAWN_INFLUENCERS];

static int SpawnSystem_FindFreeInfluencer_SP()
{
    for ( int i = 0; i < SP_MAX_SPAWN_INFLUENCERS; ++i )
    {
        if ( !s_spSpawnInfluencers[i].used )
            return i;
    }
    return -1;
}

int __cdecl SpawnSystem_AddCylinderInfluencer(
    eInfluencerType type,
    float *origin,
    float *forward,
    float *up,
    float radius,
    float axis_length,
    float score,
    eInfluencerScoreCurve score_curve,
    int teamMask,
    unsigned int entNum,
    int timeout)
{
    int index;
    SpawnInfluencer *influencer;
    SpawnInfluencerPreset *preset;

    if ( !origin || !forward || !up || radius <= 0.0f )
        return -1;

    index = SpawnSystem_FindFreeInfluencer_SP();
    if ( index < 0 )
        return -1;

    influencer = &s_spSpawnInfluencers[index];
    preset = &s_spSpawnInfluencerPresets[index];

    std::memset(influencer, 0, sizeof(*influencer));
    std::memset(preset, 0, sizeof(*preset));

    preset->index = index;
    preset->type = type;
    preset->shape = INFLUENCER_SHAPE_CYLINDER;
    preset->radius = radius;
    preset->originalLength = axis_length;
    preset->score = score;
    preset->score_curve = score_curve;

    influencer->used = true;
    influencer->active = true;
    influencer->teamMask = teamMask;
    influencer->preset = preset;
    influencer->length = axis_length;
    influencer->boundingRadius = radius + (0.5f * axis_length);
    influencer->origin[0] = origin[0];
    influencer->origin[1] = origin[1];
    influencer->origin[2] = origin[2];
    influencer->forward[0] = forward[0];
    influencer->forward[1] = forward[1];
    influencer->forward[2] = forward[2];
    influencer->up[0] = up[0];
    influencer->up[1] = up[1];
    influencer->up[2] = up[2];

    if ( entNum < MAX_GENTITIES )
        influencer->entity.setEnt(&g_entities[entNum]);
    else
        influencer->entity.setEnt(0);

    if ( timeout > 0 )
        influencer->expireTime = level.time + timeout;
    else
        influencer->expireTime = 0;

    return index;
}

char __cdecl SpawnSystem_RemoveInfluencer(unsigned int influencer_index)
{
    if ( influencer_index >= SP_MAX_SPAWN_INFLUENCERS )
        return 0;
    if ( !s_spSpawnInfluencers[influencer_index].used )
        return 0;

    std::memset(&s_spSpawnInfluencers[influencer_index], 0, sizeof(SpawnInfluencer));
    std::memset(&s_spSpawnInfluencerPresets[influencer_index], 0, sizeof(SpawnInfluencerPreset));
    return 1;
}

char __cdecl SpawnSystem_EnableInfluencer(unsigned int influencer_index, bool enabled)
{
    if ( influencer_index >= SP_MAX_SPAWN_INFLUENCERS )
        return 0;
    if ( !s_spSpawnInfluencers[influencer_index].used )
        return 0;

    s_spSpawnInfluencers[influencer_index].active = enabled;
    return 1;
}

char __cdecl SpawnSystem_SetInfluencerTeamMask(unsigned int influencer_index, int team_mask)
{
    if ( influencer_index >= SP_MAX_SPAWN_INFLUENCERS )
        return 0;
    if ( !s_spSpawnInfluencers[influencer_index].used )
        return 0;

    s_spSpawnInfluencers[influencer_index].teamMask = team_mask;
    return 1;
}

void __cdecl SpawnSystem_ClearEntityInfluencers(gentity_s *ent)
{
    int i;

    if ( !ent )
        return;

    for ( i = 0; i < SP_MAX_SPAWN_INFLUENCERS; ++i )
    {
        if ( s_spSpawnInfluencers[i].used
            && s_spSpawnInfluencers[i].entity.isDefined()
            && s_spSpawnInfluencers[i].entity.entnum() == (unsigned int)ent->s.number )
        {
            SpawnSystem_RemoveInfluencer(i);
        }
    }
}

void __cdecl QuatMultiply(const float *in1, const float *in2, float *out)
{
    out[0] = in1[3] * in2[0] + in1[0] * in2[3] + in1[1] * in2[2] - in1[2] * in2[1];
    out[1] = in1[3] * in2[1] + in1[1] * in2[3] + in1[2] * in2[0] - in1[0] * in2[2];
    out[2] = in1[3] * in2[2] + in1[2] * in2[3] + in1[0] * in2[1] - in1[1] * in2[0];
    out[3] = in1[3] * in2[3] - in1[0] * in2[0] - in1[1] * in2[1] - in1[2] * in2[2];
}
