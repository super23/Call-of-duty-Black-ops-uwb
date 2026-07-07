#include "glass_renderer.h"
#include "glass_client.h"

#include <gfx_d3d/r_drawsurf.h>
#include <gfx_d3d/r_model_lighting.h>
#include <client/splitscreen.h>
#include <client/client.h>
#include <bgame/bg_misc.h>
#include <gfx_d3d/r_stream.h>
#include <cgame/cg_drawtools.h>
#include <tl/tl_system.h>
#include <client/cl_debugdata.h>
#include <gfx_d3d/r_model_lod.h>
#include <gfx_d3d/r_primarylights.h>
#include <universal/com_workercmds.h>


cmd_function_s MemInfoCmd_VAR;
cmd_function_s CrashGlassCmd_VAR;
cmd_function_s PrintHwmCmd_VAR;

float MIN_SHARD_GROUP_VOLUME_SIZE = 32.0;


// Decomp: CoDMPServer.c:855735
void *GlassRenderer::operator new(size_t size)
{
    return GlassesClient::Allocate(
        size,
        "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_renderer.cpp",
        72);
}

// Decomp: CoDMPServer.c:855741
void GlassRenderer::operator delete(void *ptr)
{
    GlassesClient::Free((char *)ptr);
}

// Decomp: CoDMPServer.c:855747
GlassRenderer::GlassRenderer(const Glasses *glasses)
{
    static const char *const kSourceFile = "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_renderer.cpp";

    float minGridSize[3];
    float gridSize[3];
    float allExtent[3];

    rendererLock.lock = 0;
    smallAllocator.memory = 0;

    unsigned int *const smallAllocMem = GlassesClient::Allocate(
        16 * glasses->smallAllocatorBlocks,
        kSourceFile,
        70);
    smallAllocator.Init(smallAllocMem, 0x10u, glasses->smallAllocatorBlocks);

    colidingShards = new SortedShardsList(&smallAllocator);
    tempShardsList = new SortedShardsList(&smallAllocator);

    ShardGroup *const groupMem = (ShardGroup *)GlassesClient::Allocate(
        sizeof(ShardGroup) * glasses->maxGroups,
        kSourceFile,
        75);
    groupsAllocator = new FixedSizeAllocator<ShardGroup *>(groupMem, glasses->maxGroups, &smallAllocator);

    GlassShard *const shardMem = (GlassShard *)GlassesClient::Allocate(
        sizeof(GlassShard) * glasses->maxShards,
        kSourceFile,
        77);
    shardsAllocator = new FixedSizeAllocator<GlassShard *>(shardMem, glasses->maxShards, &smallAllocator);

    GlassPhysics *const physicsMem = (GlassPhysics *)GlassesClient::Allocate(
        sizeof(GlassPhysics) * glasses->maxPhysics,
        kSourceFile,
        79);
    physicsAllocator = new FixedSizeAllocator<GlassPhysics *>(physicsMem, glasses->maxPhysics, &smallAllocator);

    unsigned int *const shardWorkMem = GlassesClient::Allocate(glasses->shardMemorySize, kSourceFile, 81);
    shardMemoryAllocator.Init(shardWorkMem, glasses->shardMemorySize);

    InitShardMeshVertexLists();
    R_InitGlassRenderBuffers(glasses->numIndices, glasses->numVerts, 32);
    Com_Printf(0, "GlassRenderer init: unused memory: %d\n", GlassesClient::GetFreeMem());
    Cmd_AddCommandInternal("glassMemInfo", BLOPS_NULLSUB, &MemInfoCmd_VAR);
    Cmd_AddCommandInternal("crashGlass", GlassRenderer::CrashGlassCmd, &CrashGlassCmd_VAR);
    Cmd_AddCommandInternal("glassHWM", GlassRenderer::PrintHwmCmd, &PrintHwmCmd_VAR);
    this->drawBBox = _Dvar_RegisterBool("drawGlassBBox", 0, 0x80u, "Draw glass bounding box");
    this->drawDebug = _Dvar_RegisterBool("drawGlassDebug", 0, 0x80u, "Draw glass client debug");
    this->cullShards = _Dvar_RegisterBool("cullGlassShards", 1, 0x80u, "Enable glass shards culling (draw all if false)");
    this->drawShardOutline = _Dvar_RegisterInt(
                                                         "drawShardOutline",
                                                         0,
                                                         0,
                                                         2,
                                                         0x80u,
                                                         "draw shard outline (0-off, 1-on, 2-w/text)");
    this->pickShards = _Dvar_RegisterBool("pickShards", 0, 0x80u, "pick a shard");
    this->debugSplit = _Dvar_RegisterBool("debugSplit", 0, 0x80u, "print shard split debug info");
    this->freezeShards = _Dvar_RegisterBool("freezeShards", 0, 0x80u, "Freeze the shards after the shatter");
    this->broom = _Dvar_RegisterBool("glassBroom", 0, 0x80u, "Enable the 'broom'");
    this->maxShardSplit = _Dvar_RegisterInt(
                                                    "maxShardSplit",
                                                    75,
                                                    0,
                                                    400,
                                                    0x1000u,
                                                    "Max number of shards that a glass can be split into");
    this->shardShatterSizeLimitScale = _Dvar_RegisterFloat(
                                                                             "shardShatterSizeLimitScale",
                                                                             0.2,
                                                                             0.0099999998,
                                                                             1.0,
                                                                             0,
                                                                             "Scale of maxShardSize & minShardSize when shattering a shard.");
    this->defragMemory = _Dvar_RegisterBool("defragGlassMemory", 1, 0x80u, "Enable glass memory defragmentation");
    this->defragIndices = _Dvar_RegisterBool("defragGlassIndices", 1, 0x80u, "Enable glass memory defragmentation");
    this->smpGlass = _Dvar_RegisterBool("smpGlass", 0, 0x80u, "Update & generate glass verts in a worker thread");
    this->doMaintenance = _Dvar_RegisterBool("doMaintenance", 1, 0x80u, "Do cleanup of the glass");
    this->shardSplitDir = _Dvar_RegisterVec2(
                                                    "shardSplitDir",
                                                    (30.0),
                                                    (60.0),
                                                    0.0,
                                                    180.0,
                                                    0,
                                                    "Angle range of the first edge of the shard");
    this->shardSplitDir2 = _Dvar_RegisterVec2(
                                                     "shardSplitDir2",
                                                     (10.0),
                                                     (30.0),
                                                     0.0,
                                                     180.0,
                                                     0,
                                                     "Angle range of the next edges of the shard");
    this->shardEdgeSize = _Dvar_RegisterVec2(
                                                    "shardEdgeSize",
                                                    (0.02),
                                                    (0.15000001),
                                                    0.0,
                                                    0.30000001,
                                                    0,
                                                    "Range of the new shard edge length as a fraction of the total shard edges lengths");
    this->forceMultiplier = _Dvar_RegisterFloat(
                                                        "glassForceMultiplier",
                                                        500.0,
                                                        10.0,
                                                        100000.0,
                                                        0,
                                                        "Multiplier of the force to apply to shards.");
    this->forceOriginMult = _Dvar_RegisterFloat(
                                                        "glassForceOriginMult",
                                                        0.5,
                                                        0.0,
                                                        10.0,
                                                        0,
                                                        "Multiplier of the distance of the origin of the shards spread point - the smaller it is, clo"
                                                        "ser to the glass then the shards will spread to the sides. The farther it is the shards will"
                                                        " fly in the same direction.");
    this->forceAttenuation = _Dvar_RegisterFloat(
                                                         "glassForceAttenuation",
                                                         1.0,
                                                         0.0,
                                                         10.0,
                                                         0,
                                                         "Attenuation of the force as the shard is farther from the hit position.");
    this->extraVelocity = _Dvar_RegisterFloat(
                                                    "glassExtraVelocity",
                                                    4.0,
                                                    0.0,
                                                    100.0,
                                                    0,
                                                    "Additional random extra angular velocity to the shards when shattering, and extra linear veloc"
                                                    "ity when cracked glass times out.");
    this->freeBuffersDelay = _Dvar_RegisterInt(
                                                         "freeBuffersDelay",
                                                         1,
                                                         0,
                                                         10,
                                                         0,
                                                         "Frames delay until vertex & index buffers are freed");
    this->lowLodDist = _Dvar_RegisterFloat(
                                             "glassLowLodDist",
                                             750.0,
                                             10.0,
                                             100000.0,
                                             0,
                                             "Adjusted (by fov) distance to switch to low glass lod");
    this->maxShardLife = _Dvar_RegisterFloat(
                                                 "glassMaxShardLife",
                                                 3.0,
                                                 0.0,
                                                 1000.0,
                                                 0,
                                                 "Maximum life time of a shard that is contolled by the glass physics");
    this->timeUntilDropRange = _Dvar_RegisterVec2(
                                                             "timeUntilDropRange",
                                                             (0.25),
                                                             (1.0),
                                                             0.0,
                                                             10.0,
                                                             0,
                                                             "Range of how much time a glass shard will stay on the frame before falling");
    this->narrowShardRatio = _Dvar_RegisterFloat(
                                                         "narrowShardRatio",
                                                         3.0,
                                                         1.0,
                                                         1000.0,
                                                         0,
                                                         "minimum ratio between a shard's local bbox edges or between the shard's bbox area vs the sh"
                                                         "ard's area to cause the shard to be rejected");
    this->shatterFxMaxDist = _Dvar_RegisterFloat(
                                                         "shatterFxMaxDist",
                                                         55.0,
                                                         1.0,
                                                         1000.0,
                                                         0,
                                                         "Distance between FX spawning points along the window edges");
    this->shatterFxMinEdgeLength = _Dvar_RegisterFloat(
                                                                     "shatterFxMinEdgeLength",
                                                                     20.0,
                                                                     1.0,
                                                                     1000.0,
                                                                     0,
                                                                     "Minimum length that the window edge has to be to have a shatter fx spawned on it.");
    this->allBBoxMin[0] = FLT_MAX;
    this->allBBoxMin[1] = FLT_MAX;
    this->allBBoxMin[2] = FLT_MAX;
    this->allBBoxMax[0] = -FLT_MAX;
    this->allBBoxMax[1] = -FLT_MAX;
    this->allBBoxMax[2] = -FLT_MAX;
    for (unsigned int glassIndex = 0; glassIndex < glasses->numGlasses; ++glassIndex)
    {
        Vec3Min(allBBoxMin, glasses->glasses[glassIndex].absmin, allBBoxMin);
        Vec3Max(allBBoxMax, glasses->glasses[glassIndex].absmax, allBBoxMax);
    }
    allExtent[0] = this->allBBoxMax[0] - this->allBBoxMin[0];
    allExtent[1] = this->allBBoxMax[1] - this->allBBoxMin[1];
    allExtent[2] = this->allBBoxMax[2] - this->allBBoxMin[2];
    this->allBBoxMin[0] = (float)(-0.5 * allExtent[0]) + this->allBBoxMin[0];
    this->allBBoxMin[1] = (float)(-0.5 * allExtent[1]) + this->allBBoxMin[1];
    this->allBBoxMin[2] = (float)(-0.5 * allExtent[2]) + this->allBBoxMin[2];
    this->allBBoxMax[0] = (float)(0.5 * allExtent[0]) + this->allBBoxMax[0];
    this->allBBoxMax[1] = (float)(0.5 * allExtent[1]) + this->allBBoxMax[1];
    this->allBBoxMax[2] = (float)(0.5 * allExtent[2]) + this->allBBoxMax[2];
    Vec3Max(this->allBBoxMin, s_world.mins, this->allBBoxMin);
    Vec3Min(this->allBBoxMax, s_world.maxs, this->allBBoxMax);
    allExtent[0] = this->allBBoxMax[0] - this->allBBoxMin[0];
    allExtent[1] = this->allBBoxMax[1] - this->allBBoxMin[1];
    allExtent[2] = this->allBBoxMax[2] - this->allBBoxMin[2];
    if ( !strcmp(ui_mapname->current.string, "mp_mountain") )
        MIN_SHARD_GROUP_VOLUME_SIZE = 128.0f;
    else
        MIN_SHARD_GROUP_VOLUME_SIZE = 32.0f;
    constexpr float kGridScaleXY = 0.00048828125f;
    constexpr float kGridScaleZ = 0.0009765625f;
    gridSize[0] = 1.1f * (allExtent[0] * kGridScaleXY);
    gridSize[1] = 1.1f * (allExtent[1] * kGridScaleXY);
    gridSize[2] = 1.1f * (allExtent[2] * kGridScaleZ);
    minGridSize[0] = MIN_SHARD_GROUP_VOLUME_SIZE;
    minGridSize[1] = MIN_SHARD_GROUP_VOLUME_SIZE;
    minGridSize[2] = MIN_SHARD_GROUP_VOLUME_SIZE;
    Vec3Max(gridSize, minGridSize, gridSize);
    this->coordScale[0] = 1.0 / gridSize[0];
    this->coordScale[1] = 1.0 / gridSize[1];
    this->coordScale[2] = 1.0 / gridSize[2];
    Reset();
}

// Decomp: CoDMPServer.c:856132
GlassRenderer::~GlassRenderer()
{
    GlassesClient::Free((char *)this->groupsAllocator->memory);
    GlassesClient::Free((char *)this->groupsAllocator);
    GlassesClient::Free((char *)this->shardsAllocator->memory);
    GlassesClient::Free((char *)this->shardsAllocator);
    GlassesClient::Free((char *)this->physicsAllocator->memory);
    GlassesClient::Free((char *)this->physicsAllocator);
    GlassesClient::Free((char *)this->smallAllocator.memory);
    GlassesClient::Free((char *)this->colidingShards);
    GlassesClient::Free((char *)this->tempShardsList);
    GlassesClient::Free((char *)this->shardMemoryAllocator.buffer);
    for (unsigned int vertListIndex = 0; vertListIndex < 22; ++vertListIndex)
        GlassesClient::Free((char *)vertexList[vertListIndex]);
    R_FreeGlassRenderBuffers();
}

// Decomp: CoDMPServer.c:856154
void GlassRenderer::Reset()
{
    // Acquire spin lock
    while (rendererLock.lock ||
        _InterlockedCompareExchange(&rendererLock.lock, 1, 0))
    {
        /* spin */
    }

    // Reset all shard groups
    for (ShardGroup *group : groupsAllocator->used)
    {
        group->Reset();
    }

    // Free all allocator memory
    groupsAllocator->FreeAll();
    shardsAllocator->FreeAll();
    shardMemoryAllocator.FreeAll();

    // Reset counters and indices
    numGroupChanges = 0;
    maxNumGroupChanges = 0;
    actionInputIndex = 0;
    actionOutputIndex = 0;
    numUsedMaterials = 0;
    numShatters = 0;
    numSplits = 0;
    genVertsCount = 0;
    maxCrashShards = 0;

    // Clear action buffer
    memset(actions, 0, sizeof(actions));

    // Reset timers
    shatterTimer = 0;
    splitTimer = 0;
    triangulateTimer = 0;
    genVertsTimer = 0;

    // Reset global shard removal stats
    for (unsigned int reasonIndex = 0; reasonIndex < GlassShard::NUM_REMOVE_REASONS; ++reasonIndex)
        GlassShard::removeReasonsCount[reasonIndex] = 0;

    // Release lock
    rendererLock.lock = 0;
}

// Decomp: CoDMPServer.c:856242
void GlassRenderer::InitShardMeshVertexLists()
{
    static const char *const kSourceFile = "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_renderer.cpp";

    for (unsigned int vertListIndex = 0; vertListIndex < 22; ++vertListIndex)
    {
        const unsigned int numVerts = vertListIndex + 1;
        if (numVerts <= 2)
        {
            vertexList[vertListIndex] = 0;
        }
        else
        {
            vertexList[vertListIndex] = (GlassShardMeshVertex *)GlassesClient::Allocate(
                12 * numVerts,
                kSourceFile,
                271);
            GlassShard::Mesh::InitVertexList(numVerts, vertexList[vertListIndex]);
        }
    }
}

// Decomp: CoDMPServer.c:856269
void GlassRenderer::RemoveGlassShards(unsigned int glassIndex)
{
    // Acquire spin lock
    while (rendererLock.lock ||
           _InterlockedCompareExchange(&rendererLock.lock, 1, 0))
    {
        /* spin */
    }

    // Remove shards from all active groups
    for (ShardGroup* group : groupsAllocator->used)
    {
        group->RemoveGlassShards(glassIndex);
    }

    // Apply pending structural changes
    DoGroupChanges();

    // Release lock
    rendererLock.lock = 0;
}

// Decomp: CoDMPServer.c:856324
GlassShard *GlassRenderer::AllocShard()
{
    GlassShard *shard = shardsAllocator->Allocate();
    if (shard)
        shard->Init();
    return shard;
}

// Decomp: CoDMPServer.c:856337
void GlassRenderer::FreeShard(GlassShard *shard)
{
    if ( shard->group
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_renderer.cpp", 305, 0, "%s", "!shard->group") )
    {
        __debugbreak();
    }
    shard->Destroy();
    shardsAllocator->Free(shard);
}

// Decomp: CoDMPServer.c:856361
GlassPhysics *GlassRenderer::AllocPhysics()
{
    return this->physicsAllocator->Allocate();
}

// Decomp: CoDMPServer.c:856367
void GlassRenderer::FreePhysics(GlassPhysics *phys)
{
    this->physicsAllocator->Free(phys);
}


// Decomp: CoDMPServer.c:856373
void GlassRenderer::RemovePhysicsShards()
{
    // First, process any pending group changes
    DoGroupChanges();

    // Clear the temporary shards list
    tempShardsList->clear();

    // Iterate over all shard groups
    for (std::list<ShardGroup *, SmallAllocatorTemplate<ShardGroup *> >::iterator grpIt = groupsAllocator->used.begin();
        grpIt != groupsAllocator->used.end();
        ++grpIt)
    {
        ShardGroup &grp = **grpIt;

        // Iterate over all shards in this group
        for (GlassShard *shard = grp.head; shard; shard = shard->groupNext)
        {
            if (shard->glassPhysics)
            {
                tempShardsList->Insert(shard);
            }
        }
    }

    // Now remove all shards in the tempShardsList
    while (!tempShardsList->empty())
    {
        GlassShard *shard = tempShardsList->front();
        tempShardsList->pop_front();

        shard->Remove(GlassShard::RemoveReason::NUM_REMOVE_REASONS, 0);
    }
}

// Decomp: CoDMPServer.c:856456
void GlassRenderer::AddGroupChange(GlassShard *shard)
{
    if ( !shard->inGroupChange )
    {
        if ( this->numGroupChanges >= 0x800u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_renderer.cpp",
                        345,
                        0,
                        "%s",
                        "numGroupChanges < ARRAY_COUNT(groupChanges)") )
        {
            __debugbreak();
        }
        if ( this->numGroupChanges < 0x800u )
        {
            this->groupChanges[this->numGroupChanges++] = shard;
            shard->inGroupChange = 1;
        }
    }
}

// Decomp: CoDMPServer.c:856483
void GlassRenderer::DoGroupChanges()
{
    for (unsigned int changeIndex = 0; changeIndex < numGroupChanges; ++changeIndex)
        groupChanges[changeIndex]->ChangeGroup();

    if (maxNumGroupChanges < numGroupChanges)
        maxNumGroupChanges = numGroupChanges;

    numGroupChanges = 0;
}


// Decomp: CoDMPServer.c:856501
void GlassRenderer::Update(int threadId)
{
    iassert(threadId == 0);

    PROF_SCOPED("Glasses Update");


    CrashGlass();
    Broom();

    // Acquire renderer spin lock
    while (_InterlockedCompareExchange(&rendererLock.lock, 1, 0))
        ;

    if (!threadId)
    {
        ++frame;
        deltaTime = 1.0f / 60.0f; // 0.016666668f

        cg_s *cg = CG_GetLocalClientGlobals(0);
        if (cg && cg->snap)
        {
            for (unsigned int localClientIndex = 0; localClientIndex < 1; ++localClientIndex)
            {
                if (!CL_LocalClient_IsActive(localClientIndex))
                    continue;
                if (CL_GetLocalClientConnectionState(localClientIndex) < 4)
                    continue;

                cg_s *cgameGlob = CG_GetLocalClientGlobals(localClientIndex);
                if (!cgameGlob)
                    return;

                const int now = cgameGlob->physicsTime;
                float dt = (float)(now - timeLastUpdate) * 0.001f;
                dt = fmaxf(0.0f, fminf(dt, 0.1f));

                deltaTime = dt;
                timeLastUpdate = now;
                break;
            }
        }

        DoGroupChanges();
        ExecuteActions();

        // prevStat = stat (4 fields copied individually)
        prevStat.numMovingShards = stat.numMovingShards;
        prevStat.numVisGroups = stat.numVisGroups;
        prevStat.numVisShards = stat.numVisShards;
        prevStat.numOOMGroups = stat.numOOMGroups;

        stat.numMovingShards = 0;
        stat.numVisGroups = 0;
        stat.numVisShards = 0;
        stat.numOOMGroups = 0;
    }

    {
        PROF_SCOPED("ShardGroup.Update");

        for (ShardGroup *grp : groupsAllocator->used)
            grp->Update(deltaTime);
    }

    if (!threadId)
        DoGroupChanges();

    rendererLock.lock = 0;
}


// Decomp: CoDMPServer.c:856649
void GlassRenderer::GenerateVerts(int localClientNum, unsigned int viewIndex, unsigned int threadId)
{
    if (threadId && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_renderer.cpp", 472, 0, "threadId == 0"))
        __debugbreak();


    char *Name = va("Glass.GenVerts(c=%d v=%d t=%d)", localClientNum, viewIndex, threadId);
    PROF_SCOPED_RUNTIME_NAME(Name);


    // Acquire renderer lock
    while (_InterlockedCompareExchange(&rendererLock.lock, 1, 0))
        ;


    // Determine if this is the first view
    bool firstView = CL_LocalClient_IsFirstActive(localClientNum) && (viewIndex == 0);
    if (firstView)
        numUsedMaterials = 0;


    // Iterate all shard groups
    for (std::list<ShardGroup *, SmallAllocatorTemplate<ShardGroup *> >::iterator it = groupsAllocator->used.begin();
        it != groupsAllocator->used.end();
        ++it)
    {
        ShardGroup *grp = *it;
        grp->GenerateVerts(firstView, localClientNum);
    }

    // Release renderer lock
    rendererLock.lock = 0;

    // Start maintenance tasks
    StartMaintenance();
}


// Decomp: CoDMPServer.c:856731
void GlassRenderer::ExplosionEvent(
    const float *origin,
    float damageInner,
    float damageOuter,
    float radius,
    int mod)
{
    PROF_SCOPED("GlassRenderer.ExplosionEvent");

    // Iterate over all shard groups
    for (std::list<ShardGroup *, SmallAllocatorTemplate<ShardGroup *> >::iterator it = groupsAllocator->used.begin();
        it != groupsAllocator->used.end();
        ++it)
    {
        ShardGroup *grp = *it;
        grp->ExplosionEvent(origin, damageInner, damageOuter, radius, mod);
    }
}


// Decomp: CoDMPServer.c:856778
int GlassRenderer::TracePoint(float *p0, const float *p1)
{
    int numHits = 0;

    // Iterate over all ShardGroups
    for (ShardGroup *grp : groupsAllocator->used)
    {
        numHits += grp->TracePoint(p0, p1);
    }

    return numHits;
}


// Decomp: CoDMPServer.c:856832
void GlassRenderer::Melee(
    float *eyePos,
    const float *forward,
    const float *right,
    const float *up,
    float range)
{
    for (unsigned int traceIndex = 0; traceIndex < 5; ++traceIndex)
    {
        float end[3];
        end[0] = range * forward[0] + eyePos[0];
        end[1] = range * forward[1] + eyePos[1];
        end[2] = range * forward[2] + eyePos[2];

        const float widthOffset = player_meleeWidth->current.value * traceOffsets_0[traceIndex][0];
        end[0] += widthOffset * right[0];
        end[1] += widthOffset * right[1];
        end[2] += widthOffset * right[2];

        const float heightOffset = player_meleeHeight->current.value * traceOffsets_0[traceIndex][1];
        end[0] += heightOffset * up[0];
        end[1] += heightOffset * up[1];
        end[2] += heightOffset * up[2];

        if (TracePoint(eyePos, end) > 0)
            break;
    }
}

// Decomp: CoDMPServer.c:856862
unsigned int GlassRenderer::CalcPackedPos(const float *pos)
{
    float p[3];

    Vec3Max(pos, allBBoxMin, p);
    Vec3Min(p, this->allBBoxMax, p);
    p[0] = p[0] - this->allBBoxMin[0];
    p[1] = p[1] - this->allBBoxMin[1];
    p[2] = p[2] - this->allBBoxMin[2];
    p[0] = p[0] * this->coordScale[0];
    p[1] = p[1] * this->coordScale[1];
    p[2] = p[2] * this->coordScale[2];
    if ( (unsigned int)(__int64)p[0] >= 0x800
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_renderer.cpp",
                    627,
                    0,
                    "%s\n\t(p[0]) = %g",
                    "(uint(p[0]) < (1<<COORD_SCALE_BITS_X))",
                    p[0]) )
    {
        __debugbreak();
    }
    if ( (unsigned int)(__int64)p[1] >= 0x800
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_renderer.cpp",
                    628,
                    0,
                    "%s\n\t(p[1]) = %g",
                    "(uint(p[1]) < (1<<COORD_SCALE_BITS_Y))",
                    p[1]) )
    {
        __debugbreak();
    }
    if ( (unsigned int)(__int64)p[2] >= 0x400
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_renderer.cpp",
                    629,
                    0,
                    "%s\n\t(p[2]) = %g",
                    "(uint(p[2]) < (1<<COORD_SCALE_BITS_Z))",
                    p[2]) )
    {
        __debugbreak();
    }
    return ((__int64)p[2] << 22) | ((__int64)p[1] << 11) | (__int64)p[0];
}

// Decomp: CoDMPServer.c:856918
GlassPhysics *GlassRenderer::GetShardGroup(const float *pos, const GlassDef *glassDef)
{
    return GetShardGroup(CalcPackedPos(pos), glassDef);
}

// Decomp: CoDMPServer.c:856929
GlassPhysics *GlassRenderer::GetShardGroup(
    unsigned int packedPos,
    const GlassDef *glassDef)
{
    // First, check if a group already exists for this position and GlassDef
    for (ShardGroup *grp : groupsAllocator->used)
    {
        if (grp->packedPos == packedPos && grp->glassDef == glassDef)
        {
            return (GlassPhysics *)grp;
        }
    }

    ShardGroup *grp = groupsAllocator->Allocate();
    if (!grp)
        return nullptr;


    // Initialize the new group
    grp->Init(packedPos, glassDef);


    return (GlassPhysics *)grp;
}

// Decomp: CoDMPServer.c:856337
void GlassRenderer::FreeShardGroup(ShardGroup *grp)
{
    this->groupsAllocator->Free(grp);
}

// Decomp: CoDMPServer.c:856989
Allocator::Memory **GlassRenderer::AllocateShardMemory(
                unsigned int size,
                GlassShard *shard)
{
    return shardMemoryAllocator.Allocate(size, shard);
}

// Decomp: CoDMPServer.c:856337
void GlassRenderer::FreeShardMemory(unsigned int *ptr)
{
    this->shardMemoryAllocator.Free(ptr);
}

// Decomp: CoDMPServer.c:857001
unsigned int GlassRenderer::ShardMemorySize(unsigned int *ptr)
{
    return this->shardMemoryAllocator.GetMemorySize(ptr);
}


// Decomp: CoDMPServer.c:857007
void GlassRenderer::SortedShardsList::Insert(GlassShard *shard)
{
    const float shardArea = shard->outline.Area();

    // If the list is empty, just add it
    if (empty())
    {
        push_back(shard);
        return;
    }

    // Compare with the back element's area
    const float backArea = back()->outline.Area();

    if (shardArea <= backArea)
    {
        // Insert somewhere in the middle (or at the end)
        for (auto it = begin(); it != end(); ++it)
        {
            const float curArea = (*it)->outline.Area();

            if (curArea > shardArea)
            {
                insert(it, shard);
                return;
            }
        }

        push_back(shard);
    }
    else
    {
        push_front(shard);
    }
}


// Decomp: CoDMPServer.c:857007
void GlassRenderer::SortedShardsList::InsertReverse(GlassShard *shard)
{
    const float shardArea = shard->outline.Area();


    // If the list is empty, just add it
    if (empty())
    {
        push_back(shard);
        return;
    }


    // Compare with the back element's area
    const float backArea = back()->outline.Area();


    if (shardArea >= backArea)
    {
        // Insert somewhere in the middle (or at the end)
        for (auto it = begin(); it != end(); ++it)
        {
            const float curArea = (*it)->outline.Area();


            if (shardArea > curArea)
            {
                insert(it, shard);
                return;
            }
        }

        push_back(shard);
    }
    else
    {
        push_front(shard);
    }
}

// Decomp: CoDMPServer.c:857227
bool GlassRenderer::AddColidingShard(GlassShard *shard)
{
    this->colidingShards->Insert(shard);
    //return this->colidingShards->_Mysize > 0x64;
    return this->colidingShards->size() > 100;
}

// Decomp: CoDMPServer.c:857237
void GlassRenderer::RemoveColidingShard(GlassShard *shard)
{
    this->colidingShards->remove(shard);
    //    (GlassPhysics **)&shard);
}

// Decomp: CoDMPServer.c:857246
GlassShard *GlassRenderer::GetSmallestColidingShard()
{
    if (colidingShards->empty())
        return nullptr;
    return colidingShards->front();
}

// Decomp: CoDMPServer.c:857252
bool GlassRenderer::IsVisible(const float *minmax, unsigned int localClientNum)
{
    return !this->cullShards->current.enabled || R_CullBoxCurDpvs_SceneSelect(minmax, localClientNum, 0) == 0;
}

// Decomp: CoDMPServer.c:857264
void GlassRenderer::AddShatterAction(
                GlassClient *glass,
                const float *hitPos,
                const float *hitDir,
                bool gameState)
{
    GlassRenderer::Action *action;

    Sys_EnterCriticalSection(CRITSECT_GLASS_ACTIONS);
    action = &this->actions[this->actionInputIndex % 0xC8u];
    action->type = SHATTER;
    action->shatter.glass = glass;
    action->shatter.gameState = gameState;
    action->shatter.hitPos[0] = *hitPos;
    action->shatter.hitPos[1] = hitPos[1];
    action->shatter.hitPos[2] = hitPos[2];
    action->shatter.hitDir[0] = *hitDir;
    action->shatter.hitDir[1] = hitDir[1];
    action->shatter.hitDir[2] = hitDir[2];
    _InterlockedExchangeAdd(&this->actionInputIndex, 1u);
    Sys_LeaveCriticalSection(CRITSECT_GLASS_ACTIONS);
}

// Decomp: CoDMPServer.c:857290
void GlassRenderer::AddExplosionAction(
                const float *origin,
                float damageInner,
                float damageOuter,
                float radius,
                int mod)
{
    GlassRenderer::Action *action;

    Sys_EnterCriticalSection(CRITSECT_GLASS_ACTIONS);
    action = &this->actions[this->actionInputIndex % 0xC8u];
    action->type = EXPLOSION;
    action->trace.p0[0] = *origin;
    action->trace.p0[1] = origin[1];
    action->shatter.hitPos[0] = origin[2];
    action->shatter.hitPos[1] = damageInner;
    action->shatter.hitPos[2] = damageOuter;
    action->shatter.hitDir[0] = radius;
    action->explosion.mod = mod;
    _InterlockedExchangeAdd(&this->actionInputIndex, 1u);
    Sys_LeaveCriticalSection(CRITSECT_GLASS_ACTIONS);
}

// Decomp: CoDMPServer.c:857313
void GlassRenderer::AddTraceAction(const float *p0, const float *p1)
{
    GlassRenderer::Action *action;

    Sys_EnterCriticalSection(CRITSECT_GLASS_ACTIONS);
    action = &this->actions[this->actionInputIndex % 0xC8u];
    action->type = TRACE_POINT;
    action->trace.p0[0] = *p0;
    action->trace.p0[1] = p0[1];
    action->shatter.hitPos[0] = p0[2];
    action->shatter.hitPos[1] = *p1;
    action->shatter.hitPos[2] = p1[1];
    action->shatter.hitDir[0] = p1[2];
    _InterlockedExchangeAdd(&this->actionInputIndex, 1u);
    Sys_LeaveCriticalSection(CRITSECT_GLASS_ACTIONS);
}

// Decomp: CoDMPServer.c:857337
void GlassRenderer::AddMeleeAction(
                const float *eyePos,
                const float *forward,
                const float *right,
                const float *up,
                float range)
{
    GlassRenderer::Action *action;

    Sys_EnterCriticalSection(CRITSECT_GLASS_ACTIONS);
    action = &this->actions[this->actionInputIndex % 0xC8u];
    action->type = MELEE;
    action->trace.p0[0] = *eyePos;
    action->trace.p0[1] = eyePos[1];
    action->shatter.hitPos[0] = eyePos[2];
    action->shatter.hitPos[1] = *forward;
    action->shatter.hitPos[2] = forward[1];
    action->shatter.hitDir[0] = forward[2];
    action->shatter.hitDir[1] = *right;
    action->shatter.hitDir[2] = right[1];
    action->melee.right[2] = right[2];
    action->melee.up[0] = *up;
    action->melee.up[1] = up[1];
    action->melee.up[2] = up[2];
    action->melee.range = range;
    _InterlockedExchangeAdd(&this->actionInputIndex, 1u);
    Sys_LeaveCriticalSection(CRITSECT_GLASS_ACTIONS);
}

// Decomp: CoDMPServer.c:857372
void GlassRenderer::ExecuteActions()
{
    GlassRenderer::Action *action;

    PROF_SCOPED("GlassRenderer.ExecuteActions");

    while ( this->actionOutputIndex != this->actionInputIndex )
    {
        action = &this->actions[this->actionOutputIndex % 0xC8u];
        switch ( action->type )
        {
            case SHATTER:
                action->shatter.glass->Shatter(action->shatter.hitPos, action->shatter.hitDir);
                if ( action->shatter.gameState )
                    GlassRenderer::RemovePhysicsShards();
                break;
            case TRACE_POINT:
                GlassRenderer::TracePoint(action->trace.p0, action->trace.p1);
                break;
            case EXPLOSION:
                this->ExplosionEvent(action->explosion.origin, 
                    action->explosion.damageInner, 
                    action->explosion.damageOuter, 
                    action->explosion.radius, 
                    action->explosion.mod);
                break;
            case MELEE:
                this->Melee(action->melee.eyePos, action->melee.forward, action->melee.right, action->melee.up, action->melee.range);
                break;
            default:
                if ( !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_renderer.cpp",
                                894,
                                0,
                                "Unknown glass action command") )
                    __debugbreak();
                break;
        }
        _InterlockedExchangeAdd(&this->actionOutputIndex, 1u);
        GlassRenderer::DoGroupChanges();
    }
}

// Decomp: CoDMPServer.c:857439
void GlassRenderer::StartMaintenance()
{
    GlassRenderer::DoMaintenance();
}


// Decomp: CoDMPServer.c:857445
void GlassRenderer::GetSmallestShards(unsigned int maxShards, bool checkVisible, bool isVisible)
{
    // Clear the temporary shard list
    tempShardsList->clear();

    // Iterate over all shard groups
    for (ShardGroup *grp : groupsAllocator->used)
    {
        // Skip invisible groups if requested
        if (checkVisible && grp->visible != isVisible)
            continue;

        // Iterate over all shards in the group
        for (GlassShard *shard = grp->head; shard; shard = shard->groupNext)
        {
            // Insert the shard into the sorted temporary list
            tempShardsList->Insert(shard);

            if (tempShardsList->size() > maxShards)
                tempShardsList->pop_back();
        }
    }
}


// Decomp: CoDMPServer.c:857504
void GlassRenderer::GetLargestShards(unsigned int maxShards, bool checkVisible, bool isVisible)
{
    // Clear the temporary list first
    tempShardsList->clear();


    // Iterate over all shard groups
    for (ShardGroup *grp : groupsAllocator->used)
    {
        // Skip this group if visibility filtering is enabled and it doesn't match
        if (checkVisible && grp->visible != isVisible)
            continue;


        // Iterate over all shards in the group
        for (GlassShard *shard = grp->head; shard; shard = shard->groupNext)
        {
            // Insert shard into tempShardsList in descending order (largest first)
            tempShardsList->InsertReverse(shard);


            if (tempShardsList->size() > maxShards)
                tempShardsList->pop_back();
        }
    }
}


// Decomp: CoDMPServer.c:857563
void GlassRenderer::AddUsedShardMaterial(Material *material)
{
    for (unsigned int matIndex = 0; matIndex < numUsedMaterials; ++matIndex)
    {
        if (usedMaterials[matIndex] == material)
            return;
    }
    if ( this->numUsedMaterials >= 0x20 )
    {
        if ( !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_renderer.cpp",
                        978,
                        0,
                        "GlassRenderer::usedMaterials is too small") )
            __debugbreak();
    }
    else
    {
        R_StreamTouchMaterial(material);
        this->usedMaterials[this->numUsedMaterials++] = material;
    }
}


// Decomp: CoDMPServer.c:857719
void Sys_WaitInterlockedCompareExchange(volatile unsigned int *destination, int value, int comperand)
{
    do
    {
        while (*destination != comperand)
            ;
    } while (_InterlockedCompareExchange(destination, value, comperand) != comperand);
}

// Decomp: CoDMPServer.c:857592
void GlassRenderer::DoMaintenance()
{
    // Acquire renderer lock
    Sys_WaitInterlockedCompareExchange(&rendererLock.lock, 1, 0);


    if (!doMaintenance->current.enabled)
    {
        rendererLock.lock = 0;
        return;
    }


    double totalShards = static_cast<double>(shardsAllocator->used.size() + shardsAllocator->free.size());
    double usedFraction = static_cast<double>(shardsAllocator->used.size()) / totalShards;


    const double maxFraction = 0.9; // 90%
    if (usedFraction <= maxFraction)
    {
        // Too much shard memory reserved
        size_t totalMemory = reinterpret_cast<char *>(shardMemoryAllocator.tail) - reinterpret_cast<char *>(shardMemoryAllocator.head);
        float freeFraction = static_cast<float>(shardMemoryAllocator.GetFree()) / static_cast<float>(totalMemory);


        if ((1.0f - maxFraction) > freeFraction)
        {
            // Free some smallest shards to reclaim memory
            GetSmallestShards(0x32u, true, false);


            int targetFree = static_cast<int>(totalMemory * (1.0f - static_cast<float>(maxFraction)));


            for (GlassShard *shard : *tempShardsList)
            {
                if (shardMemoryAllocator.GetFree() >= targetFree)
                    break;


                shard->Remove(GlassShard::RemoveReason::REMOVE_OUT_OF_SHARD_MEMORY, true);
            }


            tempShardsList->clear();
        }
    }
    else
    {
        // Too many shards allocated, remove excess
        int numToFree = static_cast<int>(shardsAllocator->used.size() - static_cast<int>(totalShards * maxFraction - 5.0));


        GetSmallestShards(numToFree, true, false);


        for (GlassShard *shard : *tempShardsList)
        {
            shard->Remove(GlassShard::RemoveReason::REMOVE_OUT_OF_SHARDS, true);
        }


        tempShardsList->clear();
    }


    // Defrag shard memory
    shardMemoryAllocator.Defrag(GlassShard_Defrag, 10);


    // Pre-shatter step
    clGlasses->PreShatterNext();


    // Release renderer lock
    rendererLock.lock = 0;
}

// Decomp: CoDMPServer.c:857736
void GlassRenderer::CrashGlassCmd()
{
    clGlasses->renderer->StartCrashGlass();
}

// Decomp: CoDMPServer.c:857742
void GlassRenderer::StartCrashGlass()
{
    if (Cmd_Argc() <= 1)
    {
        maxCrashShards = 0;
        return;
    }

    int requested = atoi(Cmd_Argv(1));
    maxCrashShards = (requested > 100) ? 100 : requested;
}


// Decomp: CoDMPServer.c:857767
void GlassRenderer::CrashGlass()
{
    if (maxCrashShards == 0)
        return;


    // Clear temp list and get largest shards
    tempShardsList->clear();
    GetLargestShards(maxCrashShards, false, false);


    for (GlassShard *shard : *tempShardsList)
    {
        if (!shard->CanSplit(false))
            continue;

        GlassPhysics *phys = shard->glassPhysics;
        if (!phys)
            continue;

        const float r = flrand(-1.0f, 1.0f);
        const float *invFromW = &phys->m_inv_inertia.w;
        const float *matFromY = &phys->m_mat.w.y;

        const float delta[3] = {
            10.0f * invFromW[0] + r * matFromY[0],
            10.0f * invFromW[1] + r * matFromY[1],
            10.0f * invFromW[2] + r * matFromY[2],
        };

        const float *tvelFromZ = &phys->m_t_vel.z;
        const float p1[3] = {
            tvelFromZ[0] + delta[0],
            tvelFromZ[1] + delta[1],
            tvelFromZ[2] + delta[2],
        };
        const float p2[3] = {
            tvelFromZ[0] - delta[0],
            tvelFromZ[1] - delta[1],
            tvelFromZ[2] - delta[2],
        };

        AddTraceAction(p1, p2);
    }

    tempShardsList->clear();
}

// Decomp: CoDMPServer.c:857849
void GlassRenderer::PrintHwmCmd()
{
    clGlasses->renderer->PrintHwm();
}

// Decomp: CoDMPServer.c:857855
void GlassRenderer::PrintHwm()
{
    Com_Printf(0, "glasses:                                %7d\n", clGlasses->numGlasses);
    Com_Printf(
        0,
        "glass_smallAllocator:     %7d [%5.1f%%]\n",
        this->smallAllocator.maxUsed,
        (double)this->smallAllocator.maxUsed * 100.0 / (double)this->smallAllocator.numBlocks);
    Com_Printf(
        0,
        "glass_groupsAllocator:    %7d [%5.1f%%]\n",
        this->groupsAllocator->maxUsed,
        (double)this->groupsAllocator->maxUsed
    * 100.0
    / (double)(this->groupsAllocator->free.size() + this->groupsAllocator->used.size()));
    Com_Printf(
        0,
        "glass_shardsAllocator:    %7d [%5.1f%%]\n",
        this->shardsAllocator->maxUsed,
        (double)this->shardsAllocator->maxUsed
    * 100.0
    / (double)(this->shardsAllocator->free.size() + this->shardsAllocator->used.size()));
    Com_Printf(
        0,
        "glass_physicsAllocator: %7d [%5.1f%%]\n",
        this->physicsAllocator->maxUsed,
        (double)this->physicsAllocator->maxUsed
    * 100.0
    / (double)(this->physicsAllocator->free.size() + this->physicsAllocator->used.size()));
    Com_Printf(
        0,
        "glass_sharedMemory:         %7d [%5.1f%%]\n",
        this->shardMemoryAllocator.maxUsed,
        (float)((float)((float)this->shardMemoryAllocator.maxUsed * 100.0)
                    / (float)((char *)this->shardMemoryAllocator.tail - (char *)this->shardMemoryAllocator.head)));
    Com_Printf(0, "sizeof(GlassClient)         %7d\n", 12);
    Com_Printf(0, "sizeof(GlassRenderer)     %7d\n", 19960);
    Com_Printf(0, "sizeof(ShardGroup)            %7d\n", 84);
    Com_Printf(0, "sizeof(GlassShard)            %7d\n", 144);
    Com_Printf(0, "sizeof(GlassPhysics)        %7d\n", 160);
}


// Decomp: CoDMPServer.c:857912
void GlassRenderer::Broom()
{
    if (!broom->current.enabled)
        return;


    constexpr float DISTANCE = 150.0f;
    constexpr float RADIUS = 120.0f;
    constexpr float FORCE_SCALE = 0.05f;


    const cg_s *cgameGlob = CG_GetLocalClientGlobals(0);


    // Compute broom origin in front of player
    float origin[2] = {
    cgameGlob->refdef.vieworg[0] + DISTANCE * cgameGlob->refdef.viewaxis[2][0],
    cgameGlob->refdef.vieworg[1] + DISTANCE * cgameGlob->refdef.viewaxis[2][1]
    };


    // Broom direction
    float dir[2] = { cgameGlob->refdef.viewaxis[2][0], cgameGlob->refdef.viewaxis[2][1] };
    Vec2Normalize(dir);


    // Loop over all shards
    for (GlassShard *shrd : shardsAllocator->used)
    {
        if (!shrd->group)
            continue;


        float dx = shrd->origin[0] - origin[0];
        float dy = shrd->origin[1] - origin[1];


        // Check if within broom radius
        if ((dx * dx + dy * dy) <= (RADIUS * RADIUS))
        {
            float d2[2] = { -dx, -dy }; // direction from shard to origin
            Vec2Normalize(d2);


            // Apply only if shard roughly in broom direction
            float dot = dir[0] * d2[0] + dir[1] * d2[1];
            if (dot > 0.0f)
            {
                float force[3] = { FORCE_SCALE * d2[0], FORCE_SCALE * d2[1], 0.0f };
                shrd->InitPhysics(1, shrd->origin, force, 0.0f, 0.0f);
            }
        }
    }
}


// Decomp: CoDMPServer.c:858006
void GlassRenderer::DrawDebug()
{
    if (drawDebug && drawDebug->current.enabled)
    {
        float x = 0.0f, y = 0.0f;

        // Display general shard stats
        char *s = va(
            "glasses:%d shards:%d(free:%d) moving:%d(%d) vis:%d idx/vtx=%.1f",
            clGlasses->numGlasses,
            (int)shardsAllocator->used.size(),
            (int)shardsAllocator->free.size(),
            prevStat.numMovingShards,
            (int)colidingShards->size(),
            prevStat.numVisShards,
            0.0f);

        y = (float)CG_DrawDevString(scrPlaceView, 0.0f, 0.0f, 0.75f, 0.75f, s, colorWhite, 5, cgMedia.smallDevFont);

        // Compute timings
        float genV = genVertsCount ? (float)(genVertsTimer * 1e6 / tlPcTicksPerMS / genVertsCount) : 0.0f;
        float splitUs = numSplits ? (float)(splitTimer * 1000.0 / tlPcTicksPerMS / numSplits) : 0.0f;
        float triangPct = splitTimer ? (float)(triangulateTimer * 100.0 / splitTimer) : 0.0f;
        float shatterMs = numShatters ? (float)(shatterTimer / tlPcTicksPerMS / numShatters) : 0.0f;

        // Memory per shard
        unsigned int memPerShard = shardsAllocator->used.size()
            ? (unsigned int)(((char *)shardMemoryAllocator.tail - (char *)shardMemoryAllocator.head - shardMemoryAllocator.GetFree()) / shardsAllocator->used.size())
            : 0;

        s = va("Shard: bytes:%d shatter:%.2fms triang=%.1f%% split=%.1fus genV=%.1fns", memPerShard, shatterMs, triangPct, splitUs, genV);
        y = (float)CG_DrawDevString(scrPlaceView, x, y, 0.75f, 0.75f, s, colorWhite, 5, cgMedia.smallDevFont);

        // Shard removes
        s = va(
            "ShardRemoves: bottom:%d shards:%d shardMem:%d vtx:%d phys:%d init:%d",
            GlassShard::removeReasonsCount[0],
            GlassShard::removeReasonsCount[1],
            GlassShard::removeReasonsCount[3],
            GlassShard::removeReasonsCount[2],
            GlassShard::removeReasonsCount[4],

            GlassShard::removeReasonsCount[6]);
        y = (float)CG_DrawDevString(scrPlaceView, x, y, 0.75f, 0.75f, s, colorWhite, 5, cgMedia.smallDevFont);

        // Shard group info
        s = va(
            "ShardGroups: num:%d (free:%d) vis:%d oom:%d maxChng:%d",
            (int)groupsAllocator->used.size(),
            (int)groupsAllocator->free.size(),
            prevStat.numVisGroups,
            prevStat.numOOMGroups,
            maxNumGroupChanges);
        y = (float)CG_DrawDevString(scrPlaceView, x, y, 0.75f, 0.75f, s, colorWhite, 5, cgMedia.smallDevFont);

        // Small allocator
        s = va("Allocator: used:%d(%d) free:%d", smallAllocator.numUsed, smallAllocator.maxUsed, smallAllocator.numBlocks - smallAllocator.numUsed);
        y = (float)CG_DrawDevString(scrPlaceView, x, y, 0.75f, 0.75f, s, colorWhite, 5, cgMedia.smallDevFont);

        // Shard memory
        int totalMem = (char *)shardMemoryAllocator.tail - (char *)shardMemoryAllocator.head;
        float freePct = totalMem ? (float)(shardMemoryAllocator.GetFree() * 100) / totalMem : 0.0f;
        float minPct = totalMem ? (float)(minFreeShardsMemory * 100) / totalMem : 0.0f;
        int largestFree = shardMemoryAllocator.GetLargestFree();

        s = va("shards: sz:%d free:%d(%.0f%%) largest:%d min:%d(%.0f%%)", totalMem, shardMemoryAllocator.GetFree(), freePct, largestFree, minFreeShardsMemory, minPct);
        y = (float)CG_DrawDevString(scrPlaceView, x, y, 0.75f, 0.75f, s, colorWhite, 5, cgMedia.smallDevFont);
    }

    if (drawBBox && drawBBox->current.enabled)
    {
        // Draw all shard group boxes
        for (ShardGroup *grp : groupsAllocator->used)
        {
            if (!grp)
                continue;

            CL_AddDebugBox(vec3_origin, grp->worldBBoxMin, grp->worldBBoxMax, 0.0f, colorWhite, 1, 1);
        }

        // Draw all glasses boxes
        for (unsigned int glassIndex = 0; glassIndex < clGlasses->numGlasses; ++glassIndex)
        {
            const Glass *glass = clGlasses->GetGlass(glassIndex)->glass;
            CL_AddDebugBox(vec3_origin, glass->absmin, glass->absmax, 0.0f, colorBlue, 1, 1);
        }
    }
}

// Decomp: CoDMPServer.c:17657 (idb stub)
void GlassRenderer::BeginUpdate()
{
    ;
}

// Decomp: CoDMPServer.c:858187
void ShardGroup::Init(unsigned int p, const GlassDef *gd)
{
    this->packedPos = p;
    this->glassDef = gd;
    this->head = 0;
    this->lightingHandle = 0;
    this->invalidBBox = 1;
    this->renderIndices = 0;
    this->numShards = 0;
}

// Decomp: CoDMPServer.c:858199
void ShardGroup::Reset()
{
    GlassShard *shard;

    for ( shard = this->head; shard; shard = shard->groupNext )
    {
        if (shard->physObjId || shard->glassPhysics)
            shard->DestroyPhysicsObj();
    }
    this->head = 0;
}

// Decomp: CoDMPServer.c:858216
void ShardGroup::Add(GlassShard *shard)
{
    if ( shard->group
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_renderer.cpp", 1460, 0, "%s", "!shard->group") )
    {
        __debugbreak();
    }
    this->invalidBBox = 1;
    shard->group = this;
    shard->groupNext = this->head;
    this->head = shard;
    ++this->numShards;
}

// Decomp: CoDMPServer.c:858240
void ShardGroup::Remove(GlassShard *shard)
{
    GlassShard *s;

    if ( shard->group != this
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_renderer.cpp",
                    1471,
                    0,
                    "%s",
                    "shard->group == this") )
    {
        __debugbreak();
    }
    --this->numShards;
    this->invalidBBox = 1;
    if ( this->head == shard )
    {
        this->head = shard->groupNext;
        shard->group = 0;
        shard->groupNext = 0;
    }
    else
    {
        for ( s = this->head; s; s = s->groupNext )
        {
            if ( s->groupNext == shard )
            {
                s->groupNext = shard->groupNext;
                shard->group = 0;
                shard->groupNext = 0;
                break;
            }
        }
    }
    if ( shard->group
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_renderer.cpp", 1495, 0, "%s", "!shard->group") )
    {
        __debugbreak();
    }
    if ( !this->head )
    {
        if ( this->numShards )
        {
            if ( !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_renderer.cpp",
                            1499,
                            0,
                            "%s\n\t(numShards) = %i",
                            "(numShards == 0)",
                            this->numShards) )
                __debugbreak();
        }
        ShardGroup::FreeRenderMemory();
        clGlasses->renderer->FreeShardGroup(this);
    }
}

// Decomp: CoDMPServer.c:858309
void ShardGroup::RemoveGlassShards(unsigned int glassIndex)
{
    GlassShard *shard;

    for ( shard = this->head; shard; shard = shard->groupNext )
    {
        if (shard->glassIndex == glassIndex)
            shard->Remove(GlassShard::RemoveReason::REMOVE_ROLLBACK_TIME, true);
    }
}

// Decomp: CoDMPServer.c:858321
void ShardGroup::UpdateBBox()
{
    float *worldBBoxMax;
    float *worldBBoxMin;
    GlassShard *s;
    bool foundPhysical;

    if ( this->head )
    {
        worldBBoxMin = this->head->worldBBoxMin;
        this->worldBBoxMin[0] = *worldBBoxMin;
        this->worldBBoxMin[1] = worldBBoxMin[1];
        this->worldBBoxMin[2] = worldBBoxMin[2];
        worldBBoxMax = this->head->worldBBoxMax;
        this->worldBBoxMax[0] = *worldBBoxMax;
        this->worldBBoxMax[1] = worldBBoxMax[1];
        this->worldBBoxMax[2] = worldBBoxMax[2];
        foundPhysical = this->head->glassPhysics == 0;
        for ( s = this->head->groupNext; s; s = s->groupNext )
        {
            if ( s->glassPhysics )
            {
                if ( !foundPhysical )
                {
                    Vec3Min(this->worldBBoxMin, s->worldBBoxMin, this->worldBBoxMin);
                    Vec3Max(this->worldBBoxMax, s->worldBBoxMax, this->worldBBoxMax);
                }
            }
            else if ( foundPhysical )
            {
                Vec3Min(this->worldBBoxMin, s->worldBBoxMin, this->worldBBoxMin);
                Vec3Max(this->worldBBoxMax, s->worldBBoxMax, this->worldBBoxMax);
            }
            else
            {
                this->worldBBoxMin[0] = s->worldBBoxMin[0];
                this->worldBBoxMin[1] = s->worldBBoxMin[1];
                this->worldBBoxMin[2] = s->worldBBoxMin[2];
                this->worldBBoxMax[0] = s->worldBBoxMax[0];
                this->worldBBoxMax[1] = s->worldBBoxMax[1];
                this->worldBBoxMax[2] = s->worldBBoxMax[2];
                foundPhysical = 1;
            }
        }
        this->origin[0] = (float)(this->worldBBoxMin[0] + this->worldBBoxMax[0]) * 0.5;
        this->origin[1] = (float)(this->worldBBoxMin[1] + this->worldBBoxMax[1]) * 0.5;
        this->origin[2] = (float)(this->worldBBoxMin[2] + this->worldBBoxMax[2]) * 0.5;
        this->extent[0] = this->worldBBoxMax[0] - this->worldBBoxMin[0];
        this->extent[1] = this->worldBBoxMax[1] - this->worldBBoxMin[1];
        this->extent[2] = this->worldBBoxMax[2] - this->worldBBoxMin[2];
        this->extent[0] = 0.5 * this->extent[0];
        this->extent[1] = 0.5 * this->extent[1];
        this->extent[2] = 0.5 * this->extent[2];
    }
    this->invalidBBox = 0;
}

// Decomp: CoDMPServer.c:858396
void ShardGroup::Update(float deltaTime)
{
    GlassShard *shard;

    if ( !this->visible )
        ShardGroup::FreeRenderMemory();
    this->visible = 0;
    for ( shard = this->head; shard; shard = shard->groupNext )
    {
        if ( shard->physObjId || shard->glassPhysics || shard->delayedDrop )
        {
            shard->Update(deltaTime);
            ++clGlasses->renderer->stat.numMovingShards;
            this->invalidBBox = 1;
            ShardGroup::FreeRenderMemory();
        }
    }
}

// Decomp: CoDMPServer.c:858420
void ShardGroup::GenerateVerts(bool firstView, unsigned int localClientNum)
{
    unsigned __int8 numVertsLow;
    unsigned __int8 numIndicesLow;
    unsigned __int8 numVerts;
    unsigned __int8 numIndices;
    GlassShard *outlineShard;
    GlassShard *genShard;
    GlassShard *shard;
    unsigned __int16 vertsBaseIndex;
    unsigned __int16 vertsIndex;
    GfxPackedVertex *verts;
    unsigned __int16 *indices;
    GlassTimer timer;
    GfxPackedVertex *baseVerts;
    bool needHighLod;
    float dist;
    unsigned int primaryLightIndex;
    unsigned int lightHandle;

    if ( firstView && this->invalidBBox )
        ShardGroup::UpdateBBox();

    if ( clGlasses->renderer->IsVisible(this->worldBBoxMin, localClientNum) )
    {
        this->visible = 1;
        dist = R_GetAdjustedLodDist(Vec3Distance(origin, rg.viewOrg), XMODEL_LOD_RAMP_RIGID);
        needHighLod = clGlasses->renderer->lowLodDist->current.value > dist;
        if ( needHighLod && !this->highLod && this->renderIndices )
            ShardGroup::FreeRenderMemory();
        if ( !this->renderIndices )
        {
            PROF_SCOPED("ShardGroup.GenerateVerts");

            this->highLod = needHighLod;
            ShardGroup::FreeRenderMemory();
            this->numIndices = 0;
            this->numVerts = 0;
            for ( shard = this->head; shard; shard = shard->groupNext )
            {
                if ( this->highLod )
                    numIndices = shard->mesh.numIndices;
                else
                    numIndices = shard->mesh.numIndicesLow;
                this->numIndices += numIndices;
                if ( this->highLod )
                    numVerts = shard->mesh.numVerts;
                else
                    numVerts = shard->mesh.numVertsLow;
                this->numVerts += numVerts;
            }
            this->renderIndices = (unsigned __int16 *)ShardGroup::AllocateIndices(this->numIndices);
            baseVerts = ShardGroup::AllocateVerts(this->numVerts, &vertsBaseIndex);
            if ( !this->renderIndices || !baseVerts )
            {
                ++clGlasses->renderer->stat.numOOMGroups;
                ShardGroup::FreeRenderMemory();
                baseVerts = 0;
                this->renderIndices = 0;
                return;
            }
            indices = this->renderIndices;
            verts = baseVerts;
            vertsIndex = vertsBaseIndex;
            timer.counter = &clGlasses->renderer->genVertsTimer;
            timer.start = tlPcGetTick().QuadPart;
            clGlasses->renderer->genVertsCount += this->numVerts;
            for (genShard = head; genShard; genShard = genShard->groupNext)
            {
                genShard->GenerateVerts(highLod, verts, vertsIndex, indices);
                if (highLod)
                    numIndicesLow = genShard->mesh.numIndices;
                else
                    numIndicesLow = genShard->mesh.numIndicesLow;
                indices += numIndicesLow;
                if (highLod)
                    numVertsLow = genShard->mesh.numVerts;
                else
                    numVertsLow = genShard->mesh.numVertsLow;
                verts += numVertsLow;
                if (highLod)
                    numVerts = genShard->mesh.numVerts;
                else
                    numVerts = genShard->mesh.numVertsLow;
                vertsIndex += numVerts;
            }
            //*timer.counter += tlPcGetTick().QuadPart - timer.start;
            *timer.counter += tlPcGetTick().QuadPart - timer.start;
        }
        if ( !this->renderIndices
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_renderer.cpp",
                        1662,
                        0,
                        "%s",
                        "renderIndices") )
        {
            __debugbreak();
        }
        primaryLightIndex = R_GetNonSunPrimaryLightForBox(0, this->origin, this->extent);
        if ( !primaryLightIndex )
            primaryLightIndex = rgp.world->sunPrimaryLightIndex;
        //lightHandle = R_AllocModelLighting_PrimaryLight(
        //                                this->origin,
        //                                primaryLightIndex,
        //                                0,
        //                                &this->lightingHandle,
        //                                &this->lightingInfo);

        lightHandle = R_AllocModelLighting( // LWSS HACK: increase lighting tolerance here
            this->origin,
            256.0f,           // 16-unit movement before re-alloc; light grid is ~32 units
            0,
            &this->lightingHandle,
            R_PassbackPrimaryLightCallback,
            &primaryLightIndex,
            &this->lightingInfo);

        //Sys_AssistAndWaitWorkerCmdInternal(&r_model_lightingWorkerCmd);

        if ( lightHandle )
        {
            R_AddGlassDrawSurf(
                this->glassDef->shardMaterial,
                this->renderIndices,
                this->numIndices,
                lightHandle,
                primaryLightIndex,
                this->lightingInfo.reflectionProbeIndex,
                9u);
            clGlasses->renderer->AddUsedShardMaterial(this->glassDef->shardMaterial);
        }
        clGlasses->renderer->stat.numVisShards += this->numShards;
        ++clGlasses->renderer->stat.numVisGroups;
        if ( clGlasses->renderer->drawShardOutline->current.integer > 0 )
        {
            for (outlineShard = head; outlineShard; outlineShard = outlineShard->groupNext)
                outlineShard->DrawOutline();
        }
        ShardGroup::FreeRenderMemory();
    }
}

// Decomp: CoDMPServer.c:858568
void ShardGroup::FreeRenderMemory()
{
    this->renderIndices = 0;
}

// Decomp: CoDMPServer.c:858574
r_double_index_t *ShardGroup::AllocateIndices(int numIndices)
{
    r_double_index_t *baseIndices;

    if ( R_ReserveCodeMeshIndices(numIndices, &baseIndices) )
        return baseIndices;
    else
        return 0;
}

// Decomp: CoDMPServer.c:858587
GfxPackedVertex *ShardGroup::AllocateVerts(int numVerts, unsigned __int16 *vertsBaseIndex)
{
    if ( R_ReserveCodeMeshVerts(numVerts, vertsBaseIndex) )
        return R_GetCodeMeshVerts(*vertsBaseIndex);
    else
        return 0;
}

// Decomp: CoDMPServer.c:858599
void ShardGroup::ExplosionEvent(
                const float *origin,
                float damageInner,
                float damageOuter,
                float radius,
                int mod)
{
    GlassShard *shard;

    if ( this->worldBBoxMax[0] >= (float)(*origin - radius)
        && this->worldBBoxMax[1] >= (float)(origin[1] - radius)
        && this->worldBBoxMax[2] >= (float)(origin[2] - radius)
        && (float)(*origin + radius) >= this->worldBBoxMin[0]
        && (float)(origin[1] + radius) >= this->worldBBoxMin[1]
        && (float)(origin[2] + radius) >= this->worldBBoxMin[2] )
    {
        for ( shard = this->head; shard; shard = shard->groupNext )
        {
            if ( !shard->physObjId && !shard->glassPhysics )
                shard->ExplosionEvent(origin, damageInner, damageOuter, radius, mod);
        }
    }
}

// Decomp: CoDMPServer.c:858623
int ShardGroup::TracePoint(float *p0, const float *p1)
{
    GlassShard *shard;
    float dir[3];
    float length;
    float mins[3];
    float maxs[3];
    int numHits;

    numHits = 0;
    Vec3Min(p0, p1, mins);
    Vec3Max(p0, p1, maxs);
    if ( this->worldBBoxMax[0] >= mins[0]
        && this->worldBBoxMax[1] >= mins[1]
        && this->worldBBoxMax[2] >= mins[2]
        && maxs[0] >= this->worldBBoxMin[0]
        && maxs[1] >= this->worldBBoxMin[1]
        && maxs[2] >= this->worldBBoxMin[2] )
    {
        PROF_SCOPED("ShardGroup.TracePoint");

        dir[0] = *p1 - *p0;
        dir[1] = p1[1] - p0[1];
        dir[2] = p1[2] - p0[2];
        length = Vec3Normalize(dir);
        for ( shard = this->head; shard; shard = shard->groupNext )
        {
            if ( !shard->physObjId && !shard->glassPhysics )
            {
                if ( shard->TracePoint(p0, p1, dir, length, mins, maxs) )
                    ++numHits;
            }
        }
    }
    return numHits;
}


