#pragma once

void __cdecl GlassesClient::InitAllocator(Glasses *glasses);
unsigned int *__cdecl GlassesClient::Allocate(int size, const char *file, int line);
void __cdecl GlassesClient::Free(char *ptr);
unsigned int __cdecl GlassesClient::GetFreeMem();
GlassesClient *__thiscall GlassesClient::GlassesClient(GlassesClient *this, const Glasses *glss);
void __thiscall GlassesClient::~GlassesClient(GlassesClient *this);
const GlassClient *__thiscall GlassesClient::GetGlass(GlassesClient *this, int idx);
void __thiscall GlassesClient::Reset(GlassesClient *this);
void __thiscall GlassesClient::Update(GlassesClient *this, int localClientNum);
void __thiscall GlassesClient::ParseSnapshot(GlassesClient *this, int localClientNum, msg_t *msg, bool gameState);
void __thiscall GlassesClient::WriteDemoSnapshot(GlassesClient *this, msg_t *msg);
void __thiscall GlassesClient::PreShatterNext(GlassesClient *this);
void __thiscall GlassClient::Init(GlassClient *this, const Glass *gls);
void __thiscall GlassClient::Reset(GlassClient *this);
void __thiscall GlassClient::SetState(
        GlassClient *this,
        int localClientNum,
        GlassState::State newState,
        const float *hitPos,
        float *hitDir,
        bool gameState);
void __thiscall GlassClient::PlayShatterFX(
        GlassClient *this,
        int localClientNum,
        const float *hitPos,
        const float *hitDir);
int __cdecl compareOutlineEdges(float *s1, float *s2);
void __thiscall GlassClient::Update(GlassClient *this, int localClientNum);
void __thiscall GlassClient::SetBrushMaterial(GlassClient *this, GlassState::State state);
void __thiscall GlassClient::Shatter(GlassClient *this, const float *pos, const float *dir);
char __thiscall GlassClient::PreShatter(GlassClient *this);
int __cdecl GlassClient::Outlines::CalcMemorySize(GlassShard **shards, int numShards);
void __thiscall GlassClient::Outlines::Outlines(
        GlassClient::Outlines *this,
        const GlassShard *baseShard,
        GlassShard **shards,
        int numShards);
int __thiscall GlassClient::Outlines::InitShards(
        GlassClient::Outlines *this,
        const GlassShard *baseShard,
        GlassShard **shards,
        int maxNewShards);
void __cdecl GlassCl_AllocateMemory();
void __cdecl GlassCl_FreeMemory();
void __cdecl GlassCl_Reset(int localClientNum);
void __cdecl GlassCl_Update(int localClientNum);
void __cdecl GlassCl_ReadGameState(int localClientNum, msg_t *msg);
void __cdecl GlassCl_ParseSnapshot(int localClientNum, msg_t *msg);
void __cdecl GlassCl_WriteDemoSnapshot(msg_t *msg);
void __cdecl GlassCl_WaitUpdate();
void __cdecl GlassCl_BeginGenerateVerts(unsigned int localClientNum, unsigned int viewIndex);
void __cdecl GlassCl_WaitGenerateVerts();
void __cdecl GlassCl_GenerateVerts(int localClientNum, unsigned int viewIndex);
void __cdecl GlassCl_ExplosionEvent(
        int localClientNum,
        const float *origin,
        float damageInner,
        float damageOuter,
        float radius,
        int mod);
void __cdecl GlassCl_TracePoint(const pointtrace_t *clip, trace_t *results);
void __thiscall GlassesClient::TracePoint(GlassesClient *this, const pointtrace_t *clip, trace_t *results);
void __cdecl GlassCl_MeleeEvent(int localClientNum, int attackerEntNum);
void __cdecl GlassCl_DrawDebug();
unsigned int __cdecl GlassCl_AreaGlasses(
        const float *mins,
        const float *maxs,
        const Glass **glasses,
        unsigned int maxGlasses);
unsigned int __thiscall GlassesClient::AreaGlasses(
        GlassesClient *this,
        const float *mins,
        const float *maxs,
        const Glass **glss,
        unsigned int maxGlasses);
void __cdecl GlassCl_ClipMoveTrace(const moveclip_t *clip, trace_t *results);
void __thiscall GlassesClient::ClipMoveTrace(GlassesClient *this, const moveclip_t *clip, trace_t *results);
