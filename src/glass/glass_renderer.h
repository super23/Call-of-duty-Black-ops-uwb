#pragma once
#include <universal/dvar.h>
#include <gfx_d3d/r_material.h>

#include "glass_shard.h"
#include "glass_allocator.h"

struct GlassClient;
struct Glasses;

// Interlocked lock used around the deferred glass action queue (CRITSECT_GLASS_ACTIONS).
struct GlassLock // sizeof=0x4
{
    volatile unsigned int lock;
};

// Client-side glass shard simulation, maintenance, and vertex generation.
struct GlassRenderer // sizeof=0x4DF8
{
    // Deferred events queued from gameplay / FX and drained on the glass update thread.
    enum ActionType_t : __int32
    {
        SHATTER     = 0x0,
        TRACE_POINT = 0x1,
        EXPLOSION   = 0x2,
        MELEE       = 0x3,
        MAX_ACTIONS = 0x64, // action-type sentinel (not the ring capacity)
    };

    struct Action // sizeof=0x38
    {
        ActionType_t type;
        union // sizeof=0x34
        {
            struct // sizeof=0x20
            {
                GlassClient *glass;
                bool gameState;
                char pad[3];
                float hitPos[3];
                float hitDir[3];
            } shatter;
            struct // sizeof=0x18
            {
                float p0[3];
                float p1[3];
            } trace;
            struct // sizeof=0x1C
            {
                float origin[3];
                float damageInner;
                float damageOuter;
                float radius;
                int mod;
            } explosion;
            struct // sizeof=0x34
            {
                float eyePos[3];
                float forward[3];
                float right[3];
                float up[3];
                float range;
            } melee;
        };
    };

    // std::list of shard pointers sorted by GlassShard::Outline::Area (see Insert / InsertReverse).
    struct SortedShardsList : std::list<GlassShard *, SmallAllocatorTemplate<GlassShard *>> // sizeof=0x1C
    {
        SortedShardsList() = default;

        explicit SortedShardsList(SmallAllocator *shardListAllocator)
            : std::list<GlassShard *, SmallAllocatorTemplate<GlassShard *>>(
                SmallAllocatorTemplate<GlassShard *>(shardListAllocator))
        {
        }

        void *operator new(size_t listByteSize, SmallAllocator * /*shardListAllocator*/)
        {
            return GlassesClient::Allocate(
                listByteSize,
                "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_renderer.cpp",
                72);
        }

        void *operator new(size_t listByteSize)
        {
            return GlassesClient::Allocate(
                listByteSize,
                "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_renderer.cpp",
                72);
        }

        void operator delete(void *listMemory)
        {
            GlassesClient::Free((char *)listMemory);
        }

        // Insert ascending by outline area (smallest shards first).
        void Insert(GlassShard *shard);
        // Insert descending by outline area (largest shards first).
        void InsertReverse(GlassShard *shard);
    };

    void *operator new(size_t size);
    void operator delete(void *ptr);

    const dvar_s *cullShards;
    const dvar_s *drawDebug;
    const dvar_s *drawBBox;
    const dvar_s *drawShardOutline;
    const dvar_s *pickShards;
    const dvar_s *debugSplit;
    const dvar_s *freezeShards;
    const dvar_s *broom;

    // Per-frame counters for PrintHwm / maintenance (current and previous frame).
    struct GlassRendererStat // sizeof=0x10
    {
        int numMovingShards;
        int numVisGroups;
        int numVisShards;
        int numOOMGroups;
    } stat;
    GlassRendererStat prevStat;

    int minFreeVertsMemory;
    int minFreeIndicesMemory;
    int minFreeShardsMemory;
    int maxCrashShards;
    unsigned int numShatters;
    char padAfterNumShatters[4];
    unsigned __int64 shatterTimer;
    unsigned int numSplits;
    char padAfterNumSplits[4];
    unsigned __int64 splitTimer;
    unsigned __int64 triangulateTimer;
    unsigned int genVertsCount;
    char padAfterGenVertsCount[4];
    unsigned __int64 genVertsTimer;
    const dvar_s *maxShardSplit;
    const dvar_s *shardShatterSizeLimitScale;
    const dvar_s *defragMemory;
    const dvar_s *defragIndices;
    const dvar_s *freeBuffersDelay;
    const dvar_s *smpGlass;
    const dvar_s *shardSplitDir;
    const dvar_s *shardSplitDir2;
    const dvar_s *shardEdgeSize;
    const dvar_s *forceMultiplier;
    const dvar_s *forceOriginMult;
    const dvar_s *forceAttenuation;
    const dvar_s *extraVelocity;
    const dvar_s *doMaintenance;
    const dvar_s *lowLodDist;
    const dvar_s *maxShardLife;
    const dvar_s *timeUntilDropRange;
    const dvar_s *narrowShardRatio;
    const dvar_s *shatterFxMaxDist;
    const dvar_s *shatterFxMinEdgeLength;
    unsigned int frame;
    int timeLastUpdate;
    float deltaTime;
    GlassLock rendererLock;
    SmallAllocator smallAllocator;
    Allocator shardMemoryAllocator;
    FixedSizeAllocator<ShardGroup *> *groupsAllocator;
    FixedSizeAllocator<GlassShard *> *shardsAllocator;
    FixedSizeAllocator<GlassPhysics *> *physicsAllocator;
    SortedShardsList *colidingShards;   // retail spelling: shards eligible for collision eviction
    SortedShardsList *tempShardsList;   // scratch list for maintenance / visibility passes
    Material *usedMaterials[32];
    unsigned int numUsedMaterials;
    float allBBoxMin[3];
    float allBBoxMax[3];
    float coordScale[3];
    GlassShard *groupChanges[2048];
    int numGroupChanges;
    int maxNumGroupChanges;
    GlassShardMeshVertex *vertexList[22];
    Action actions[200];                // SPSC ring; slot index = index % 0xC8
    volatile unsigned int actionInputIndex;
    volatile unsigned int actionOutputIndex;

    GlassRenderer(const Glasses *glasses);
    ~GlassRenderer();

    void __thiscall Reset();
    void __thiscall InitShardMeshVertexLists();
    void __thiscall RemoveGlassShards(unsigned int glassIndex);
    GlassShard *__thiscall AllocShard();
    void __thiscall FreeShard(GlassShard *shard);
    GlassPhysics *__thiscall AllocPhysics();
    void __thiscall FreePhysics(GlassPhysics *phys);
    void __thiscall RemovePhysicsShards();
    void __thiscall AddGroupChange(GlassShard *shard);
    void __thiscall DoGroupChanges();
    void __thiscall Update(int threadId);
    void __thiscall GenerateVerts(
        int localClientNum,
        unsigned int viewIndex,
        unsigned int threadId);
    void __thiscall ExplosionEvent(
        const float *origin,
        float damageInner,
        float damageOuter,
        float radius,
        int mod);
    int __thiscall TracePoint(float *p0, const float *p1);
    void __thiscall Melee(
        float *eyePos,
        const float *forward,
        const float *right,
        const float *up,
        float range);
    unsigned int __thiscall CalcPackedPos(const float *pos);
    GlassPhysics *__thiscall GetShardGroup(const float *pos, const GlassDef *glassDef);
    GlassPhysics *__thiscall GetShardGroup(
        unsigned int packedPos,
        const GlassDef *glassDef);
    void __thiscall FreeShardGroup(ShardGroup *grp);
    Allocator::Memory **__thiscall AllocateShardMemory(
        unsigned int size,
        GlassShard *shard);
    void __thiscall FreeShardMemory(unsigned int *ptr);
    unsigned int __thiscall ShardMemorySize(unsigned int *ptr);
    bool __thiscall AddColidingShard(GlassShard *shard);
    void __thiscall RemoveColidingShard(GlassShard *shard);
    GlassShard *__thiscall GetSmallestColidingShard();
    bool __thiscall IsVisible(const float *minmax, unsigned int localClientNum);
    void __thiscall AddShatterAction(
        GlassClient *glass,
        const float *hitPos,
        const float *hitDir,
        bool gameState);
    void __thiscall AddExplosionAction(
        const float *origin,
        float damageInner,
        float damageOuter,
        float radius,
        int mod);
    void __thiscall AddTraceAction(const float *p0, const float *p1);
    void __thiscall AddMeleeAction(
        const float *eyePos,
        const float *forward,
        const float *right,
        const float *up,
        float range);
    void __thiscall ExecuteActions();
    void __thiscall StartMaintenance();
    void __thiscall GetSmallestShards(
        unsigned int maxShards,
        bool checkVisible,
        bool isVisible);
    void __thiscall GetLargestShards(
        unsigned int maxShards,
        bool checkVisible,
        bool isVisible);
    void __thiscall AddUsedShardMaterial(Material *material);
    void __thiscall DoMaintenance();
    static void __cdecl CrashGlassCmd();
    void __thiscall StartCrashGlass();
    void __thiscall CrashGlass();
    static void __cdecl PrintHwmCmd();
    void __thiscall PrintHwm();
    void __thiscall Broom();
    void __thiscall DrawDebug();
    void __thiscall BeginUpdate();
};
