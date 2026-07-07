#pragma once

#include "glass.h"

struct GlassShard;
struct GlassRenderer;
struct msg_t;
struct trace_t;
struct pointtrace_t;
struct moveclip_t;

struct GlassClient
{
    struct Outlines
    {
        struct Outline
        {
            unsigned int numVerts;
            unsigned int edgeFlags;
            float (*verts)[2];
        };

        unsigned int numOutlines;
        Outline *outlines;

        Outlines(const GlassShard *baseShard, const GlassShard **shards, int numShards);

        int InitShards(const GlassShard *baseShard, const GlassShard **shards, int maxNewShards);

        static int CalcMemorySize(const GlassShard **shards, int numShards);
    };

    const Glass *glass;
    GlassState state;
    Outlines *outlines;

    void Init(const Glass *gls);
    void Reset();
    void SetState(
        int localClientNum,
        GlassState::State newState,
        const float *hitPos,
        float *hitDir,
        bool gameState);
    void PlayShatterFX(int localClientNum, const float *hitPos, const float *hitDir);
    void Update(int localClientNum);
    void SetBrushMaterial(GlassState::State state);
    void Shatter(const float *pos, const float *dir);
    char PreShatter();
};

struct StackAllocator
{
    unsigned char *memory;
    unsigned int size;
    unsigned int pos;
};

struct GlassesClient
{
    GlassClient *glasses;
    unsigned int numGlasses;
    GlassRenderer *renderer;
    unsigned int lastPreShatter;

    void *operator new(size_t size);
    void operator delete(void *ptr);

    GlassesClient(const Glasses *glss);
    ~GlassesClient();

    static StackAllocator allocator;

    static void InitAllocator(Glasses *glasses);
    static unsigned int *Allocate(int size, const char *file, int line);
    static void Free(char *ptr);
    static unsigned int GetFreeMem();

    const GlassClient *GetGlass(int idx);
    void Reset();
    void Update(int localClientNum);
    void ParseSnapshot(int localClientNum, msg_t *msg, bool gameState);
    void WriteDemoSnapshot(msg_t *msg);
    void PreShatterNext();
    void TracePoint(const pointtrace_t *clip, trace_t *results);
    unsigned int AreaGlasses(
        const float *mins,
        const float *maxs,
        const Glass **glss,
        unsigned int maxGlasses);
    void ClipMoveTrace(const moveclip_t *clip, trace_t *results);
};

int compareOutlineEdges(float *s1, float *s2);

void GlassCl_AllocateMemory();
void GlassCl_FreeMemory();
void GlassCl_Reset(int localClientNum);
void GlassCl_Update(int localClientNum);
void GlassCl_ReadGameState(int localClientNum, msg_t *msg);
void GlassCl_ParseSnapshot(int localClientNum, msg_t *msg);
void GlassCl_WriteDemoSnapshot(msg_t *msg);
void GlassCl_WaitUpdate();
void GlassCl_BeginGenerateVerts(unsigned int localClientNum, unsigned int viewIndex);
void GlassCl_WaitGenerateVerts();
void GlassCl_GenerateVerts(int localClientNum, unsigned int viewIndex);
void GlassCl_ExplosionEvent(
    int localClientNum,
    const float *origin,
    float damageInner,
    float damageOuter,
    float radius,
    int mod);
void GlassCl_TracePoint(const pointtrace_t *clip, trace_t *results);
void GlassCl_MeleeEvent(int localClientNum, int attackerEntNum);
void GlassCl_DrawDebug();
unsigned int GlassCl_AreaGlasses(
    const float *mins,
    const float *maxs,
    const Glass **glasses,
    unsigned int maxGlasses);
void GlassCl_ClipMoveTrace(const moveclip_t *clip, trace_t *results);

extern GlassesClient *clGlasses;
