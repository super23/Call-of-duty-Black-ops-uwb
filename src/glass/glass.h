#pragma once

#include <gfx_d3d/r_material.h>
#include <gfx_d3d/fxprimitives.h>

// Packed pane state: 4-bit GlassState::State plus 12-bit pane id (see GlassServer snapshot writes).
struct GlassState
{
    enum State : int
    {
        PRISTINE = 0,
        CRACKED = 1,
        SHATTERED = 2,
        INVALID = 3,
    };

    union
    {
        unsigned int i;
        struct
        {
            unsigned int state : 4;
            unsigned int id : 12;
        };
    } val;
};

static_assert(sizeof(GlassState) == 4);

// Glass material / shard parameters (fastfile asset, 60 bytes).
struct GlassDef
{
    const char *name;
    int maxHealth;
    float thickness;
    float minShardSize;
    float maxShardSize;
    float shardLifeProbablility; // retail asset key spelling
    int maxShards;
    Material *pristineMaterial;
    Material *crackedMaterial;
    Material *shardMaterial;
    const char *crackSound;
    const char *shatterShound;     // retail asset key spelling
    const char *autoShatterShound; // retail asset key spelling
    const FxEffectDef *crackEffect;
    const FxEffectDef *shatterEffect;
};

static_assert(sizeof(GlassDef) == 0x3C);

// One placed breakable pane in a map (read-only glasses asset, 124 bytes).
struct Glass
{
    GlassDef *glassDef;
    unsigned int index;
    unsigned int brushModel;
    float origin[3];
    float angles[3];
    float absmin[3];
    float absmax[3];
    bool isPlanar;
    char numOutlineVerts;
    char pad[2];
    float (*outline)[2];
    float outlineAxis[3][3];
    float outlineOrigin[3];
    float uvScale;
    float thickness;
};

static_assert(sizeof(Glass) == 0x7C);

// Glasses asset: pane array plus client work-memory and renderer limits.
struct Glasses
{
    const char *name;
    unsigned int numGlasses;
    Glass *glasses;
    unsigned __int8 *workMemory;
    unsigned int workMemorySize;
    unsigned int smallAllocatorBlocks;
    unsigned int maxGroups;
    unsigned int maxShards;
    unsigned int maxPhysics;
    unsigned int shardMemorySize;
    unsigned int maxFreeCmd;
    unsigned int numSlots;
    unsigned int numVerts;
    unsigned int numIndices;
};

static_assert(sizeof(Glasses) == 0x38);

// Scoped PC-tick accumulator for renderer profiling (split / triangulate / gen-verts stats).
struct __declspec(align(8)) GlassTimer
{
    unsigned __int64 start;
    unsigned __int64 *counter;
};

static_assert(sizeof(GlassTimer) == 0x10);
