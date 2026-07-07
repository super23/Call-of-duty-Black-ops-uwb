#pragma once
// Glass shard physics, outline, mesh, and shard-group types.
// Retail: CoDMPServer.h structs 2672–2677, 2711, 35785–35799; bodies CoDMPServer.c ~854000+ (allocator)
// and ~857000+ / ~861000+ / ~862000+ / ~863000+ (GlassShard, helpers).

#include <universal/com_math.h>
#include <physics/phys_local.h>
#include <gfx_d3d/r_material.h>
#include <gfx_d3d/fxprimitives.h>
#include <gfx_d3d/r_dpvs.h>
#include <gfx_d3d/r_model_lighting.h>

struct GfxPackedVertex;

// CoDMPServer.h:2673 — 2D ray/segment in outline plane (origin + dir + length).
struct ray2_t // sizeof=0x14
{
    float origin[2];
    float dir[2];
    float len;
};

// CoDMPServer.h:2677 — rigid-body state for falling shards (align 16).
struct __declspec(align(16)) GlassPhysics // sizeof=0xA0
{
    phys_mat44 m_mat;
    phys_vec3 m_inv_inertia;
    phys_vec3 m_t_vel;
    phys_vec3 m_a_vel;
    phys_vec3 m_force_sum;
    phys_vec3 m_torque_sum;
    float m_inv_mass;
    char pad[12];

    void CreateAxis(
        float *position,
        const float (*axis)[3],
        float *tVel,
        float *aVel,
        float mass,
        float *localBBoxMin,
        float *localBBoxMax);
    void AddForce(float *worldPos, float *impulse);
    void SetVelocity(float *t_vel);
    void SetAngularVelocity(float *a_vel);
    void Step(float deltaTime);
    void tensor_transform_principle(
        const phys_vec3 *diag,
        const phys_mat44 *mat,
        phys_mat44 *tensor);
    void IntegrateVelocity(float deltaTime);
    void IntegratePos(float deltaTime);
    void GetPosition(float *position, float (*axis)[3]);
};
static_assert(sizeof(GlassPhysics) == 160);

struct GlassDef;

// CoDMPServer.h:2678 — render/physics bucket for shards sharing a glass pane.
struct ShardGroup // sizeof=0x54
{
    unsigned int packedPos;
    const GlassDef *glassDef;
    struct GlassShard *head;
    float worldBBoxMin[3];
    float worldBBoxMax[3];
    float origin[3];
    float extent[3];
    bool invalidBBox;
    bool visible;
    bool highLod;
    char pad; // align __int16 fields
    __int16 numShards;
    __int16 numIndices;
    __int16 numVerts;
    unsigned __int16 lightingHandle;
    GfxLightingInfo lightingInfo;
    unsigned __int16 *renderIndices;

    void __thiscall Init(unsigned int packedPos, const GlassDef *glassDef);
    void __thiscall Reset();
    void __thiscall Add(GlassShard *shard);
    void __thiscall Remove(GlassShard *shard);
    void __thiscall RemoveGlassShards(unsigned int glassIndex);
    void __thiscall UpdateBBox();
    void __thiscall Update(float deltaTime);
    void __thiscall GenerateVerts(bool firstView, unsigned int localClientNum);
    void __thiscall FreeRenderMemory();
    r_double_index_t *__thiscall AllocateIndices(int numIndices);
    GfxPackedVertex *__thiscall AllocateVerts(int numVerts, unsigned __int16 *vertsBaseIndex);
    void __thiscall ExplosionEvent(
        const float *origin,
        float damageInner,
        float damageOuter,
        float radius,
        int mod);
    int __thiscall TracePoint(float *p0, const float *p1);
};
static_assert(sizeof(ShardGroup) == 84);

// CoDMPServer.h:2711
struct GlassShardMeshVertex // sizeof=0x2
{
    unsigned __int8 pos;
    unsigned __int8 norm;
};

// CoDMPServer.h:2672
struct GlassShard // sizeof=0x90
{
    // CoDMPServer.h:7030
    enum RemoveReason : int
    {
        REMOVE_HIT_BOTTOM           = 0,
        REMOVE_OUT_OF_SHARDS        = 1,
        REMOVE_OUT_OF_VERTEX_MEMORY = 2,
        REMOVE_OUT_OF_SHARD_MEMORY  = 3,
        REMOVE_OUT_OF_PHYSICS       = 4,
        REMOVE_ROLLBACK_TIME        = 5,
        NUM_REMOVE_REASONS          = 6,
        REMOVE_DONT_TRACK           = 7,
        // Retail passes (GlassShard::RemoveReason)8 without a named enum entry — CoDMPServer.c:855067
        // after Outlines::InitShards replaces the alloc shard with outline-derived shards.
        REMOVE_SHATTER_REPLACED     = 8,
        KISAK_I_HAVE_NO_CLUE_WHY    = REMOVE_SHATTER_REPLACED, // legacy alias (glass_client.cpp)
        NUM_REMOVE_REASON_STATS     = 9,
        KISAK_TOTAL                 = NUM_REMOVE_REASON_STATS,
    };

    // CoDMPServer.h:7043
    enum SplitFailReason : int
    {
        SPLIT_FAIL_START_POS  = 0,
        SPLIT_FAIL_START_DIR  = 1,
        SPLIT_FAIL_INIT       = 2,
        SPLIT_FAIL_VERTS      = 3,
        SPLIT_FAIL_AREA       = 4,
        SPLIT_FAIL_MEMORY     = 5,
        SPLIT_FAIL_ANGLE      = 6,
        SPLIT_FAIL_NARROW     = 7,
        NUM_SPLIT_FAIL_REASONS = 8,
    };

    struct Outline;

    // CoDMPServer.h:35785 — ear-clipping triangulation workspace (256 index bytes).
    struct Triangles // sizeof=0x108
    {
        const struct Outline *outline;
        unsigned __int8 triangleIndices[256];
        unsigned int nIndices;

        Triangles(const struct GlassShard::Outline *outline);
        double CalcCross(int vertIdx0, int vertIdx1, int vertIdx2);
        char AddTri(
            unsigned __int8 vertIdx0,
            unsigned __int8 vertIdx1,
            unsigned __int8 vertIdx2);
        char Triangulate();
    };

    struct Outline
    {
        // CoDMPServer.h:35793
        struct EdgeDistance // sizeof=0x14
        {
            unsigned int edgeIndex;
            float edgeParam;
            float dist;
            float closestPoint[2];
        };

        // CoDMPServer.h:2674
        struct Vertex // sizeof=0x18
        {
            ray2_t edge;
            bool isOriginalEdge;
            char pad[3];

            void operator=(const GlassShard::Outline::Vertex *other);
        };
        static_assert(sizeof(Outline::Vertex) == 24);

        GlassShard::Outline::Vertex *verts;
        float length;
        float area;
        unsigned __int8 numVerts;
        unsigned __int8 maxVerts;
        bool isClosed;
        char pad;

        void MarkAllEdge(bool isEdge);

        int Init(Vertex *vertStorage, unsigned __int8 numVerts);
        int SetPointers(unsigned __int8 *ptr);
        GlassShard::Outline &operator=(const GlassShard::Outline &other);
        double Length() const;

        // CoDMPServer.c:857107 @ 0099A940 — cached shoelace area; requires closed outline.
        inline float Area() const
        {
            iassert(isClosed);
            return area;
        }

        Vertex *Add(const float *pos);
        Vertex *Add(float *dir, float len);

        char CloseOutline();

        void GetBBox(float *mins, float *maxs);
        double Extent();
        void Recenter(bool flip, float *offset);
        bool HasNarrowAngle();
        bool IsNarrow();
        void Reverse();
        int GetNumIntersections(
            const float *rayOrigin,
            const float *rayDir,
            float *nearestDist,
            int *nearestEdge) const;
        char DoesIntersect(
            const float *rayOrigin,
            const float *rayDir,
            float rayLen,
            float padding);
        void GetNearestDistances(
            const float *point,
            EdgeDistance *dists,
            int numDists);
        void Verify() const;

        static void Defrag(GlassShard *shard);
        void Defrag();
    };

    // CoDMPServer.h:2676
    struct __declspec(align(4)) Mesh // sizeof=0x10
    {
        PackedUnitVec *normArray;
        unsigned __int8 *indices;
        unsigned __int8 numNorm;
        unsigned __int8 numVerts;
        unsigned __int8 numVertsLow;
        unsigned __int8 numIndices;
        unsigned __int8 numIndicesLow;
        char pad[3];

        void Clear();
        void SetTriangles(
            unsigned __int8 *ptr,
            unsigned __int8 *triangles,
            unsigned int numTriIdxs);
        int SetPointers(PackedUnitVec *ptr);
        void Init(
            const GlassShard::Outline *outline,
            float thickness,
            const float *tangent);
        static void __cdecl InitVertexList(unsigned __int8 numOutlineVerts, GlassShardMeshVertex *verts);
        static unsigned int __cdecl GetMemorySize(unsigned int numOutlineVerts);
    };

    GlassShard *groupNext;
    unsigned __int8 *memoryPtr;
    unsigned __int16 memorySize;
    unsigned __int16 glassIndex;
    GlassShard::Outline outline;
    float thickness;
    float uvScale;
    GlassShard::Mesh mesh;
    float axis[3][3];
    float origin[3];
    int physObjId;
    GlassPhysics *glassPhysics;
    float timeUntilAction;
    float worldBBoxMin[3];
    float worldBBoxMax[3];
    ShardGroup *group;
    bool isOriginalEdge;
    bool remove;
    bool inGroupChange;
    bool delayedDrop;

    void Init();
    void Destroy();
    double EdgeRatio();
    void UnEdge();
    char Create(const Glass *glass);
    bool CanSplit(float maxShardSize, float minShardSize) const;
    bool CanSplit(bool shatter) const;
    int Shatter(const GlassShard **newShards, int maxNewShards);
    void InitPhysics(
        const GlassShard **newShards,
        int numNewShards,
        float glassExtent,
        const float *pos,
        const float *dir);
    void InitMesh();
    char AllocateMemory(
        unsigned int numHullVerts,
        const Triangles *triangles);
    void FreeMemory();
    void Defrag();
    void ToWorldPos(float *localPos, float *worldPos, bool is3D);
    void ToWorldDir(float *localDir, float *worldDir, bool is3D);
    void ToLocal(float *pos, float *dir, float *localPos, float *localDir);
    bool Intersect(float *pos, float *dir, float *hitPoint);
    void ExplosionEvent(
        const float *expOrigin,
        float damageInner,
        float damageOuter,
        float radius,
        int mod);
    char TracePoint(
        float *p0,
        const float *p1,
        float *dir,
        float length,
        const float *mins,
        float *maxs);
    void GetLocalBBox(float *mins, float *maxs);
    void UpdateBBox();
    void Update(float deltaTime);
    void ChangeGroup();
    void Remove(RemoveReason reason, bool delay);
    void GenerateVerts(
        bool highLod,
        GfxPackedVertex *baseVerts,
        unsigned __int16 vertsBaseIndex,
        unsigned __int16 *idxOut);
    int Split(
        const GlassShard **newShards,
        float minShardSize,
        unsigned int startEdge,
        float startEdgeParam) const;
    int Chip(
        const Outline::EdgeDistance *dist,
        const float *hitPoint,
        const GlassShard **newShards,
        float minShardSize);
    int InitSplitShards(
        Outline *outline1,
        Outline *outline2,
        const GlassShard **newShards,
        float minShardSize) const;
    char Init(
        const GlassShard *other,
        const GlassShard::Outline *newOutline,
        const float *const offset);
    double GetMass();
    bool InitPhysicsObj(bool enableCollisions);
    void DestroyPhysicsObj();
    void DisableCollisions();
    void AddForce(float *pos, const float *forceIn);
    void SetVelocity(float *t_vel, float *a_vel);
    bool LeaveOnEdge(float stickiness);
    bool IsOnBottomEdge();
    void Recenter();
    char InitPhysics(
        bool enableCollisions,
        const float *hitPos,
        const float *hitDir,
        float glassSize,
        float stickiness);
    void DrawOutline();

    static int splitFailCount[NUM_SPLIT_FAIL_REASONS]; // CoDMPServer.c:110742
    static int lastFreeMemorySize;
    static int removeReasonsCount[KISAK_TOTAL]; // retail [7] @ CoDMPServer.c:110748; +1 for reason 8
};
static_assert(sizeof(GlassShard) == 144);

void GlassShard_Defrag(void *ptr);

// CoDMPServer.h — stack outline buffer for GlassShard::Create (64 verts × 24 bytes + outline header).
struct TempOutline : GlassShard::Outline // sizeof=0x610
{
    GlassShard::Outline::Vertex v[64]; // stack verts; glass_shard.cpp sets outline.verts = v
};

// CoDMPServer — edge ray used by GlassClient::PlayShatterFX.
struct OutlineEdge // sizeof=0x18
{
    int index;
    ray2_t ray;
};

// CoDMPServer.c:862437 @ 009A6C90 — point-in-triangle test (2D, outline plane).
bool __cdecl IsInside(const float *vert0, const float *vert1, const float *vert2, const float *point);

// CoDMPServer.c:861856 @ 009A5030 — parametric position of point on segment [segStart, segEnd].
double __cdecl GetSegmentParam(const float *segStart, const float *segEnd, const float *point);

// CoDMPServer.c:861871 @ 009A50C0
bool __cdecl Vec2IntersectSegments(
    const float *segA0,
    const float *segA1,
    const float *segB0,
    const float *segB1,
    float *intersectionOut);

// CoDMPServer.c:861892 @ 009A5170 (retail spelling preserved)
char __cdecl Vec2IntesectLines(
    const float *lineA0,
    const float *lineA1,
    const float *lineB0,
    const float *lineB1,
    float *intersectionOut);

int compareShards(const void *shardA, const void *shardB);
