#pragma once

GlassRenderer *__thiscall GlassRenderer::GlassRenderer(GlassRenderer *this, const Glasses *glasses);
void __thiscall GlassRenderer::~GlassRenderer(GlassRenderer *this);
void __thiscall GlassRenderer::Reset(GlassRenderer *this);
void __thiscall GlassRenderer::InitShardMeshVertexLists(GlassRenderer *this);
void __thiscall GlassRenderer::RemoveGlassShards(GlassRenderer *this, unsigned int glassIndex);
GlassShard *__thiscall GlassRenderer::AllocShard(GlassRenderer *this);
void __thiscall GlassRenderer::FreeShard(GlassRenderer *this, GlassShard *shard);
GlassPhysics *__thiscall GlassRenderer::AllocPhysics(GlassRenderer *this);
void __thiscall GlassRenderer::FreePhysics(GlassRenderer *this, GlassPhysics *phys);
void __thiscall GlassRenderer::RemovePhysicsShards(GlassRenderer *this);
void __thiscall GlassRenderer::AddGroupChange(GlassRenderer *this, GlassShard *shard);
void __thiscall GlassRenderer::DoGroupChanges(GlassRenderer *this);
void __thiscall GlassRenderer::Update(GlassRenderer *this, int threadId);
void __thiscall GlassRenderer::GenerateVerts(
        GlassRenderer *this,
        int localClientNum,
        unsigned int viewIndex,
        unsigned int threadId);
void __thiscall GlassRenderer::ExplosionEvent(
        GlassRenderer *this,
        const float *origin,
        float damageInner,
        float damageOuter,
        float radius,
        int mod);
int __thiscall GlassRenderer::TracePoint(GlassRenderer *this, float *p0, const float *p1);
void __thiscall GlassRenderer::Melee(
        GlassRenderer *this,
        float *eyePos,
        const float *forward,
        const float *right,
        const float *up,
        float range);
unsigned int __thiscall GlassRenderer::CalcPackedPos(GlassRenderer *this, const float *pos);
GlassPhysics *__thiscall GlassRenderer::GetShardGroup(GlassRenderer *this, const float *pos, const GlassDef *glassDef);
GlassPhysics *__thiscall GlassRenderer::GetShardGroup(
        GlassRenderer *this,
        unsigned int packedPos,
        const GlassDef *glassDef);
void __thiscall GlassRenderer::FreeShardGroup(GlassRenderer *this, GlassPhysics *grp);
Allocator::Memory **__thiscall GlassRenderer::AllocateShardMemory(
        GlassRenderer *this,
        unsigned int size,
        GlassShard *shard);
void __thiscall GlassRenderer::FreeShardMemory(GlassRenderer *this, unsigned int *ptr);
unsigned int __thiscall GlassRenderer::ShardMemorySize(GlassRenderer *this, unsigned int *ptr);
void __thiscall GlassRenderer::SortedShardsList::Insert(GlassRenderer::SortedShardsList *this, GlassShard *shard);
double __thiscall GlassShard::Outline::Area(GlassShard::Outline *this);
void __thiscall GlassRenderer::SortedShardsList::InsertReverse(
        GlassRenderer::SortedShardsList *this,
        GlassShard *shard);
bool __thiscall GlassRenderer::AddColidingShard(GlassRenderer *this, GlassShard *shard);
void __thiscall GlassRenderer::RemoveColidingShard(GlassRenderer *this, GlassShard *shard);
GlassPhysics *__thiscall GlassRenderer::GetSmallestColidingShard(GlassRenderer *this);
bool __thiscall GlassRenderer::IsVisible(GlassRenderer *this, const float *minmax, unsigned int localClientNum);
void __thiscall GlassRenderer::AddShatterAction(
        GlassRenderer *this,
        GlassClient *glass,
        const float *hitPos,
        const float *hitDir,
        bool gameState);
void __thiscall GlassRenderer::AddExplosionAction(
        GlassRenderer *this,
        const float *origin,
        float damageInner,
        float damageOuter,
        float radius,
        int mod);
void __thiscall GlassRenderer::AddTraceAction(GlassRenderer *this, const float *p0, const float *p1);
void __thiscall GlassRenderer::AddMeleeAction(
        GlassRenderer *this,
        const float *eyePos,
        const float *forward,
        const float *right,
        const float *up,
        float range);
void __thiscall GlassRenderer::ExecuteActions(GlassRenderer *this);
void __thiscall GlassRenderer::StartMaintenance(GlassRenderer *this);
void __thiscall GlassRenderer::GetSmallestShards(
        GlassRenderer *this,
        unsigned int maxShards,
        bool checkVisible,
        bool isVisible);
void __thiscall GlassRenderer::GetLargestShards(
        GlassRenderer *this,
        unsigned int maxShards,
        bool checkVisible,
        bool isVisible);
void __thiscall GlassRenderer::AddUsedShardMaterial(GlassRenderer *this, Material *material);
void __thiscall GlassRenderer::DoMaintenance(GlassRenderer *this);
void __cdecl Sys_WaitInterlockedCompareExchange(volatile int *destination, int value, int comperand);
void __cdecl GlassShard::Defrag(GlassShard *ptr);
void __cdecl GlassRenderer::CrashGlassCmd();
void __thiscall GlassRenderer::StartCrashGlass(GlassRenderer *this);
void __thiscall GlassRenderer::CrashGlass(GlassRenderer *this);
void __cdecl GlassRenderer::PrintHwmCmd();
void __thiscall GlassRenderer::PrintHwm(GlassRenderer *this);
void __thiscall GlassRenderer::Broom(GlassRenderer *this);
void __thiscall GlassRenderer::DrawDebug(GlassRenderer *this);
void __thiscall GlassRenderer::BeginUpdate(colgeom_visitor_t *this);
void __thiscall ShardGroup::Init(ShardGroup *this, unsigned int p, const GlassDef *gd);
void __thiscall ShardGroup::Reset(ShardGroup *this);
void __thiscall ShardGroup::Add(ShardGroup *this, GlassShard *shard);
void __thiscall ShardGroup::Remove(ShardGroup *this, GlassShard *shard);
void __thiscall ShardGroup::RemoveGlassShards(ShardGroup *this, unsigned int glassIndex);
void __thiscall ShardGroup::UpdateBBox(ShardGroup *this);
void __thiscall ShardGroup::Update(ShardGroup *this, float deltaTime);
void __thiscall ShardGroup::GenerateVerts(ShardGroup *this, bool firstView, unsigned int localClientNum);
void __thiscall ShardGroup::FreeRenderMemory(ShardGroup *this);
r_double_index_t *__thiscall ShardGroup::AllocateIndices(ShardGroup *this, int numIndices);
GfxPackedVertex *__thiscall ShardGroup::AllocateVerts(ShardGroup *this, int numVerts, unsigned __int16 *vertsBaseIndex);
void __thiscall ShardGroup::ExplosionEvent(
        ShardGroup *this,
        const float *origin,
        float damageInner,
        float damageOuter,
        float radius,
        int mod);
int __thiscall ShardGroup::TracePoint(ShardGroup *this, float *p0, const float *p1);
void __thiscall GlassShard::Defrag(GlassShard *this);
