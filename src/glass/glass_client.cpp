#include "glass_client.h"
#include "glass_renderer.h"
#include <universal/com_memory.h>

#include <new>
#include <cgame/cg_sound.h>
#include <EffectsCore/fx_marks.h>
#include <cgame/cg_scr_main.h>
#include <EffectsCore/fx_system.h>
#include "glass_load_obj.h"
#include <client/splitscreen.h>
#include <bgame/bg_misc.h>
#include <universal/com_math.h>

GlassesClient *clGlasses;

StackAllocator GlassesClient::allocator;

// CoDMPServer @ 009955A0
void __cdecl GlassesClient::InitAllocator(Glasses *glasses)
{
    GlassesClient::allocator.memory = glasses->workMemory;
    GlassesClient::allocator.size = glasses->workMemorySize;
    GlassesClient::allocator.pos = 0;
}

// CoDMPServer @ 009955E0
unsigned int *__cdecl GlassesClient::Allocate(int size, const char *file, int line)
{
    const char *label = va("Glass work memory %d %s(%d)", size, file, line);
    return Z_Malloc(size, label, 9);
}

// CoDMPServer @ 00995620
void __cdecl GlassesClient::Free(char *ptr)
{
    Z_Free(ptr, 9);
}

// CoDMPServer @ 00995640
unsigned int __cdecl GlassesClient::GetFreeMem()
{
    return GlassesClient::allocator.size - GlassesClient::allocator.pos;
}

// CoDMPServer @ 00995650
void *GlassesClient::operator new(size_t size)
{
    return GlassesClient::Allocate(size, "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_client.cpp", 69);
}

// CoDMPServer @ (operator delete, glass_client.cpp)
void GlassesClient::operator delete(void *ptr)
{
    GlassesClient::Free((char *)ptr);
}

// CoDMPServer @ 00995670
GlassesClient::GlassesClient(const Glasses *glss)
{
    this->numGlasses = glss->numGlasses;
    this->glasses = (GlassClient *)GlassesClient::Allocate(
        12 * this->numGlasses,
        "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_client.cpp",
        80);
    this->renderer = new GlassRenderer(glss);
    this->lastPreShatter = 0;
    for (unsigned int glassIndex = 0; glassIndex < this->numGlasses; ++glassIndex)
        this->glasses[glassIndex].Init(&glss->glasses[glassIndex]);
}

// CoDMPServer @ 00995730
GlassesClient::~GlassesClient()
{
    GlassesClient::Free((char *)this->glasses);
    if (this->renderer)
        delete this->renderer;
}

// CoDMPServer @ 00995790
const GlassClient *GlassesClient::GetGlass(int idx)
{
    return &this->glasses[idx];
}

// CoDMPServer @ 009957B0
void __thiscall GlassesClient::Reset()
{
    for (unsigned int glassIndex = 0; glassIndex < this->numGlasses; ++glassIndex)
        this->glasses[glassIndex].Reset();
}

// CoDMPServer @ 009957F0
void __thiscall GlassesClient::Update(int localClientNum)
{
    PROF_SCOPED("GlassesClient.Update");
    for (unsigned int glassIndex = 0; glassIndex < this->numGlasses; ++glassIndex)
        this->glasses[glassIndex].Update(localClientNum);
}

// CoDMPServer @ 00995870 | BlackOpsMP.retail sub_5D0990
void __thiscall GlassesClient::ParseSnapshot(int localClientNum, msg_t *msg, bool gameState)
{
    float hitDir[3];
    float hitPos[3];
    unsigned int glassIndex;
    unsigned int changeIndex;
    unsigned int numChanges;
    unsigned int state;
    unsigned int glassCount;
    unsigned int endMarker;

    if (MSG_ReadBit(msg) == 1)
    {
        if (MSG_ReadBit(msg) == 1)
        {
            glassCount = MSG_ReadShort(msg);
            if (glassCount != this->numGlasses)
            {
                Com_PrintError(
                    14,
                    "GlassesClient::ParseSnapshot - number of glasses mismatch: %d != %d\n",
                    glassCount,
                    this->numGlasses);
                MSG_Discard(msg);
                return;
            }
            for (glassIndex = 0; glassIndex < this->numGlasses; ++glassIndex)
            {
                state = MSG_ReadBits(msg, 2u);
                if (state == GlassState::SHATTERED)
                {
                    hitPos[0] = MSG_ReadFloat(msg);
                    hitPos[1] = MSG_ReadFloat(msg);
                    hitPos[2] = MSG_ReadFloat(msg);
                    hitDir[0] = MSG_ReadFloat(msg);
                    hitDir[1] = MSG_ReadFloat(msg);
                    hitDir[2] = MSG_ReadFloat(msg);
                }
                this->glasses[glassIndex].SetState(
                    localClientNum,
                    (GlassState::State)state,
                    hitPos,
                    hitDir,
                    gameState);
            }
        }
        else
        {
            numChanges = MSG_ReadShort(msg);
            for (changeIndex = 0; changeIndex < numChanges; ++changeIndex)
            {
                glassIndex = MSG_ReadBits(msg, 0xAu);
                if (glassIndex >= this->numGlasses)
                {
                    Com_PrintError(
                        14,
                        "GlassesClient::ParseSnapshot - glass index out of range: %d .= %d\n",
                        glassIndex,
                        this->numGlasses);
                    MSG_Discard(msg);
                    return;
                }
                state = MSG_ReadBits(msg, 2u);
                if (state == GlassState::SHATTERED)
                {
                    hitPos[0] = MSG_ReadFloat(msg);
                    hitPos[1] = MSG_ReadFloat(msg);
                    hitPos[2] = MSG_ReadFloat(msg);
                    hitDir[0] = MSG_ReadFloat(msg);
                    hitDir[1] = MSG_ReadFloat(msg);
                    hitDir[2] = MSG_ReadFloat(msg);
                }
                this->glasses[glassIndex].SetState(
                    localClientNum,
                    (GlassState::State)state,
                    hitPos,
                    hitDir,
                    gameState);
            }
        }
    }
    endMarker = MSG_ReadShort(msg);
    if (endMarker != 30154)
    {
        Com_PrintError(14, "GlassesClient::ParseSnapshot - unexpected end of message: %x\n", endMarker);
        MSG_Discard(msg);
    }
}

// CoDMPServer @ 00995B00
void __thiscall GlassesClient::WriteDemoSnapshot(msg_t *msg)
{
    PROF_SCOPED("GlassesClient.WriteDemoSnapshot");
    MSG_WriteBit1(msg);
    MSG_WriteBit1(msg);
    MSG_WriteShort(msg, this->numGlasses);
    for (unsigned int glassIndex = 0; glassIndex < this->numGlasses; ++glassIndex)
    {
        GlassClient *glass = &this->glasses[glassIndex];
        if ((glass->state.val.i & 0xF) == GlassState::INVALID
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_client.cpp",
                203,
                0,
                "%s",
                "glass->state.GetState() != GlassState::INVALID"))
        {
            __debugbreak();
        }
        MSG_WriteBits(msg, glass->state.val.i & 0xF, 2u);
        if ((glass->state.val.i & 0xF) == GlassState::SHATTERED)
        {
            MSG_WriteFloat(msg, 0.0);
            MSG_WriteFloat(msg, 0.0);
            MSG_WriteFloat(msg, 0.0);
            MSG_WriteFloat(msg, 0.0);
            MSG_WriteFloat(msg, 0.0);
            MSG_WriteFloat(msg, 0.0);
        }
    }
    MSG_WriteShort(msg, 30154);
}

// CoDMPServer @ 00995C80
void __thiscall GlassesClient::PreShatterNext()
{
    PROF_SCOPED("GlassesClient.PreShatterNext");

    for (unsigned int attempt = 0; attempt < this->numGlasses; ++attempt)
    {
        GlassClient *glass = &this->glasses[this->lastPreShatter++];
        if (this->lastPreShatter >= this->numGlasses)
            this->lastPreShatter = 0;
        if (glass->PreShatter())
            return;
    }
}

// CoDMPServer @ 00995D60
void __thiscall GlassClient::Init(const Glass *gls)
{
    this->state.val.i = this->state.val.i & 0xFFFFFFF0 | GlassState::INVALID;
    this->glass = gls;
    this->outlines = 0;
    GfxBrushModel *bmodel = R_GetBrushModel(this->glass->brushModel);
    if (!bmodel
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_client.cpp",
            273,
            0,
            "%s",
            "bmodel"))
    {
        __debugbreak();
    }
    bmodel->writable.mip1radiusSq = 1000000.0f;
    bmodel->writable.mins[0] = gls->absmin[0];
    bmodel->writable.mins[1] = gls->absmin[1];
    bmodel->writable.mins[2] = gls->absmin[2];
    bmodel->writable.maxs[0] = gls->absmax[0];
    bmodel->writable.maxs[1] = gls->absmax[1];
    bmodel->writable.maxs[2] = gls->absmax[2];
}

// CoDMPServer @ 00995E60
void __thiscall GlassClient::Reset()
{
    if (this->outlines)
    {
        clGlasses->renderer->FreeShardMemory((unsigned int *)this->outlines);
        this->outlines = 0;
    }
}

// CoDMPServer @ 00995EA0
void __thiscall GlassClient::SetState(
    int localClientNum,
    GlassState::State newState,
    const float *hitPos,
    float *hitDir,
    bool gameState)
{
    if (newState != (this->state.val.i & 0xF))
    {
        this->state.val.i = newState & 0xF | this->state.val.i & 0xFFFFFFF0;
        if (newState)
        {
            if (newState == GlassState::CRACKED)
            {
                clGlasses->renderer->RemoveGlassShards(this->glass->index);
                GlassClient::SetBrushMaterial(GlassState::CRACKED);
                if (!gameState)
                {
                    CG_PlaySound(
                        localClientNum,
                        (float *)this->glass->origin,
                        0,
                        0,
                        1.0,
                        (char *)this->glass->glassDef->crackSound);
                }
            }
            else if (newState == GlassState::SHATTERED)
            {
                clGlasses->renderer->AddShatterAction(this, hitPos, hitDir, gameState);
                CScr_GlassSmash((float *)this->glass->origin, hitDir);
                if (!gameState)
                {
                    const char *shatterSound;
                    if ((hitDir[0] * hitDir[0] + hitDir[1] * hitDir[1] + hitDir[2] * hitDir[2]) == 0.0f)
                        shatterSound = this->glass->glassDef->autoShatterShound;
                    else
                        shatterSound = this->glass->glassDef->shatterShound;
                    CG_PlaySound(
                        localClientNum,
                        (float *)this->glass->origin,
                        0,
                        0,
                        1.0,
                        (char *)shatterSound);
                    if (this->glass->glassDef->shatterEffect)
                        GlassClient::PlayShatterFX(localClientNum, hitPos, hitDir);
                }
                FX_MarkGlassDetachAll(localClientNum, this->glass->index);
            }
            else
            {
                if (!Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_client.cpp",
                    328,
                    0,
                    va("wrong glass state: %d", newState)))
                {
                    __debugbreak();
                }
            }
        }
        else
        {
            clGlasses->renderer->RemoveGlassShards(this->glass->index);
            GlassClient::SetBrushMaterial(GlassState::PRISTINE);
        }
    }
}

// CoDMPServer @ 009960D0
void __thiscall GlassClient::PlayShatterFX(
    int localClientNum,
    const float *hitPos,
    const float *hitDir)
{
    const int maxOutlineEdges = 32;
    const int maxEffects = 10;
    OutlineEdge outlineEdges[32];
    float axis[3][3];
    float effectPos[3];
    float offsetAlongEdge;
    float offsetAcrossEdge;
    int effectsOnEdge;
    int effectStep;
    int totalEffects;
    int outlineEdgeIndex;
    int effectIndex;

    if (this->glass->numOutlineVerts >= maxOutlineEdges
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_client.cpp",
            362,
            0,
            "%s",
            "glass->numOutlineVerts < MAX_EDGES"))
    {
        __debugbreak();
    }

    for (outlineEdgeIndex = 0; outlineEdgeIndex < this->glass->numOutlineVerts; ++outlineEdgeIndex)
    {
        const unsigned int nextVertIndex = (outlineEdgeIndex == this->glass->numOutlineVerts - 1)
            ? 0u
            : outlineEdgeIndex + 1;
        const float *vertStart = this->glass->outline[outlineEdgeIndex];
        const float *vertEnd = this->glass->outline[nextVertIndex];
        ray2_t *edgeRay = &outlineEdges[outlineEdgeIndex].ray;

        outlineEdges[outlineEdgeIndex].index = outlineEdgeIndex;
        edgeRay->origin[0] = vertStart[0];
        edgeRay->origin[1] = vertStart[1];
        edgeRay->dir[0] = vertEnd[0] - vertStart[0];
        edgeRay->dir[1] = vertEnd[1] - vertStart[1];
        edgeRay->len = Vec2Normalize(edgeRay->dir);
    }

    qsort(
        outlineEdges,
        this->glass->numOutlineVerts,
        sizeof(OutlineEdge),
        (int(__cdecl *)(const void *, const void *))compareOutlineEdges);

    const float hitDirLenSq = hitDir[0] * hitDir[0] + hitDir[1] * hitDir[1] + hitDir[2] * hitDir[2];
    if (hitDirLenSq == 0.0f)
    {
        if (irand(0, 1))
            Vec3Copy(this->glass->outlineAxis[2], axis[0]);
        else
        {
            axis[0][0] = -this->glass->outlineAxis[2][0];
            axis[0][1] = -this->glass->outlineAxis[2][1];
            axis[0][2] = -this->glass->outlineAxis[2][2];
        }
    }
    else
    {
        const float normalDot = hitDir[0] * this->glass->outlineAxis[2][0]
            + hitDir[1] * this->glass->outlineAxis[2][1]
            + hitDir[2] * this->glass->outlineAxis[2][2];
        if (normalDot <= 0.0f)
        {
            axis[0][0] = -this->glass->outlineAxis[2][0];
            axis[0][1] = -this->glass->outlineAxis[2][1];
            axis[0][2] = -this->glass->outlineAxis[2][2];
        }
        else
            Vec3Copy(this->glass->outlineAxis[2], axis[0]);
    }

    Vec3Copy(this->glass->outlineAxis[1], axis[1]);
    Vec3Cross(axis[0], axis[1], axis[2]);

    outlineEdgeIndex = 0;
    totalEffects = 0;
    while (outlineEdgeIndex < this->glass->numOutlineVerts
        && totalEffects < maxEffects
        && clGlasses->renderer->shatterFxMinEdgeLength->current.value <= outlineEdges[outlineEdgeIndex].ray.len)
    {
        effectsOnEdge = (int)(outlineEdges[outlineEdgeIndex].ray.len
                / clGlasses->renderer->shatterFxMaxDist->current.value
            + 0.5f);
        if (effectsOnEdge > 3)
            effectsOnEdge = 3;
        if (effectsOnEdge < 1)
            effectsOnEdge = 1;

        effectStep = outlineEdges[outlineEdgeIndex].ray.len / (float)effectsOnEdge;
        for (effectIndex = 0; effectIndex < effectsOnEdge; ++effectIndex)
        {
            offsetAlongEdge = ((float)effectIndex + 0.5f) * effectStep;
            offsetAcrossEdge = offsetAlongEdge * outlineEdges[outlineEdgeIndex].ray.dir[1]
                + outlineEdges[outlineEdgeIndex].ray.origin[1];
            const float alongCoord = offsetAlongEdge * outlineEdges[outlineEdgeIndex].ray.dir[0]
                + outlineEdges[outlineEdgeIndex].ray.origin[0];
            const float *outlineU = this->glass->outlineAxis[0];
            const float *outlineOrigin = this->glass->outlineOrigin;
            const float *outlineV = this->glass->outlineAxis[1];

            effectPos[0] = alongCoord * outlineU[0] + outlineOrigin[0];
            effectPos[1] = alongCoord * outlineU[1] + outlineOrigin[1];
            effectPos[2] = alongCoord * outlineU[2] + outlineOrigin[2];
            effectPos[0] = offsetAcrossEdge * outlineV[0] + effectPos[0];
            effectPos[1] = offsetAcrossEdge * outlineV[1] + effectPos[1];
            effectPos[2] = offsetAcrossEdge * outlineV[2] + effectPos[2];

            cg_s *cgame = CG_GetLocalClientGlobals(localClientNum);
            FX_PlayOrientedEffect(
                localClientNum,
                this->glass->glassDef->shatterEffect,
                cgame->time,
                effectPos,
                axis);
            ++totalEffects;
        }
        ++outlineEdgeIndex;
    }
}

// CoDMPServer @ 00996870 — qsort comparator; longer edges sort first (descending ray.len)
int __cdecl compareOutlineEdges(float *elemA, float *elemB)
{
    const OutlineEdge *edgeA = reinterpret_cast<const OutlineEdge *>(elemA);
    const OutlineEdge *edgeB = reinterpret_cast<const OutlineEdge *>(elemB);

    if (edgeA->ray.len <= edgeB->ray.len)
        return 1;
    return -1;
}

// CoDMPServer @ 009968B0
void __thiscall GlassClient::Update(int localClientNum)
{
    if ((this->state.val.i & 0xF) == GlassState::INVALID
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_client.cpp",
            417,
            0,
            "%s",
            "state.GetState() != GlassState::INVALID"))
    {
        __debugbreak();
    }
    if ((this->state.val.i & 0xF) != GlassState::SHATTERED)
    {
        Material *drawMaterial;
        if (this->state.val.i & 0xF)
            drawMaterial = this->glass->glassDef->pristineMaterial;
        else
            drawMaterial = this->glass->glassDef->crackedMaterial;
        const GfxBrushModel *brushModel = R_GetBrushModel(this->glass->brushModel);
        R_AddGlassBrushToScene(brushModel, this->glass->origin, this->glass->angles, drawMaterial);
    }
}

// CoDMPServer @ 00996960
void __thiscall GlassClient::SetBrushMaterial(GlassState::State state)
{
    if ((this->state.val.i & 0xF) >= GlassState::SHATTERED
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_client.cpp",
            480,
            0,
            "%s",
            "GetState() == GlassState::PRISTINE || GetState() == GlassState::CRACKED"))
    {
        __debugbreak();
    }
    const GfxBrushModel *brushModel = R_GetBrushModel(this->glass->brushModel);
    GfxSurface *surface = &rgp.world->dpvs.surfaces[brushModel->startSurfIndex];
    Material *surfaceMaterial;
    if (this->state.val.i & 0xF)
        surfaceMaterial = this->glass->glassDef->crackedMaterial;
    else
        surfaceMaterial = this->glass->glassDef->pristineMaterial;
    for (unsigned int surfaceIndex = 0; surfaceIndex < brushModel->surfaceCount; ++surfaceIndex)
    {
        surface->material = surfaceMaterial;
        ++surface;
    }
}

// CoDMPServer @ 00996A40
void __thiscall GlassClient::Shatter(const float *pos, const float *dir)
{
    const GlassShard *newShards[400];
    float glassExtent;
    GlassShard *shard;

    PROF_SCOPED("GlassClient.Shatter");

    shard = clGlasses->renderer->AllocShard();
    if (!shard)
        return;

    if (!shard->Create(this->glass))
    {
        clGlasses->renderer->FreeShard(shard);
        return;
    }

    glassExtent = shard->outline.Extent();
    if (this->outlines)
    {
        PROF_SCOPED("GlassClient.Shatter.outlines");
        const int numNewShards = this->outlines->InitShards(shard, newShards, 400);
        shard->Remove(static_cast<GlassShard::RemoveReason>(8), false);
        clGlasses->renderer->FreeShardMemory((unsigned int *)this->outlines);
        this->outlines = 0;
        if (numNewShards > 0)
            ((GlassShard *)newShards[0])->InitPhysics(newShards, numNewShards, glassExtent, pos, dir);
    }
    else
    {
        const int numNewShards = shard->Shatter(newShards, 400);
        if (!shard->group
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_client.cpp",
                570,
                0,
                "%s",
                "shard->group"))
        {
            __debugbreak();
        }
        if (shard->group)
            shard->InitPhysics(newShards, numNewShards, glassExtent, pos, dir);
        else
        {
            for (int shardIndex = 0; shardIndex < numNewShards; ++shardIndex)
                ((GlassShard *)newShards[shardIndex])->Remove(static_cast<GlassShard::RemoveReason>(8), false);
        }
    }
}

// CoDMPServer @ 00996D00
char __thiscall GlassClient::PreShatter()
{
    const GlassShard *newShards[400];
    GlassShard *shard;

    if ((this->state.val.i & 0xF) == GlassState::SHATTERED || this->outlines)
        return 0;

    PROF_SCOPED("GlassClient.PreShatter");

    shard = clGlasses->renderer->AllocShard();
    if (!shard)
        return 1;

    if (shard->Create(this->glass))
    {
        const int numNewShards = shard->Shatter(newShards, 400);
        GlassRenderer *renderer = clGlasses->renderer;
        const unsigned int outlineBytes = GlassClient::Outlines::CalcMemorySize(newShards, numNewShards);
        Allocator::Memory **outlineBuffer = renderer->AllocateShardMemory(outlineBytes, NULL);
        if (outlineBuffer)
        {
            GlassShard *baseShard = clGlasses->renderer->AllocShard();
            if (baseShard && baseShard->Create(this->glass))
            {
                new (outlineBuffer) GlassClient::Outlines(baseShard, newShards, numNewShards);
                this->outlines = (GlassClient::Outlines *)outlineBuffer;
            }
            if (baseShard)
                baseShard->Remove(static_cast<GlassShard::RemoveReason>(8), false);
        }
        for (int shardIndex = 0; shardIndex < numNewShards; ++shardIndex)
            ((GlassShard *)newShards[shardIndex])->Remove(static_cast<GlassShard::RemoveReason>(8), false);
    }
    else
        clGlasses->renderer->FreeShard(shard);

    return 1;
}

// CoDMPServer @ 00996F80
int __cdecl GlassClient::Outlines::CalcMemorySize(const GlassShard **shards, int numShards)
{
    int numVerts = 0;
    for (int shardIndex = 0; shardIndex < numShards; ++shardIndex)
        numVerts += shards[shardIndex]->outline.numVerts;
    return 12 * numShards + 8 * numVerts + 8;
}

// CoDMPServer @ 00996FE0
GlassClient::Outlines::Outlines(
    const GlassShard * /*baseShard*/,
    const GlassShard **shards,
    int numShards)
{
    this->numOutlines = numShards;
    this->outlines = (GlassClient::Outlines::Outline *)&this[1];
    float *vertStorage = (float *)&this->outlines[this->numOutlines].numVerts;
    for (int shardIndex = 0; shardIndex < numShards; ++shardIndex)
    {
        this->outlines[shardIndex].numVerts = shards[shardIndex]->outline.numVerts;
        this->outlines[shardIndex].verts = (float (*)[2])vertStorage;
        this->outlines[shardIndex].edgeFlags = 0;
        for (unsigned int vertIndex = 0; vertIndex < this->outlines[shardIndex].numVerts; ++vertIndex)
        {
            float *outlineVert = this->outlines[shardIndex].verts[vertIndex];
            const GlassShard::Outline::Vertex *shardVert = &shards[shardIndex]->outline.verts[vertIndex];
            outlineVert[0] = shardVert->edge.origin[0];
            outlineVert[1] = shardVert->edge.origin[1];
            if (shards[shardIndex]->outline.verts[vertIndex].isOriginalEdge)
                this->outlines[shardIndex].edgeFlags |= 1u << vertIndex;
        }
        vertStorage += 2 * this->outlines[shardIndex].numVerts;
    }
}

// CoDMPServer @ 00997160
int __thiscall GlassClient::Outlines::InitShards(
    const GlassShard *baseShard,
    const GlassShard **shards,
    int maxNewShards)
{
    PROF_SCOPED("InitShards");

    int numNewShards = 0;
    for (unsigned int outlineIndex = 0; outlineIndex < this->numOutlines; ++outlineIndex)
    {
        TempOutline newOutline;
        newOutline.verts = newOutline.v;
        newOutline.numVerts = 0;
        newOutline.maxVerts = 64;
        newOutline.isClosed = 0;
        for (unsigned int vertIndex = 0; vertIndex < this->outlines[outlineIndex].numVerts; ++vertIndex)
        {
            const bool isOriginalEdge = (this->outlines[outlineIndex].edgeFlags & (1u << vertIndex)) != 0;
            newOutline.Add(this->outlines[outlineIndex].verts[vertIndex])->isOriginalEdge = isOriginalEdge;
        }
        newOutline.CloseOutline();
        float offset[2];
        newOutline.Recenter(false, offset);

        GlassShard *newShard = clGlasses->renderer->AllocShard();
        shards[numNewShards] = newShard;
        if (!shards[numNewShards])
            break;
        if (!((GlassShard *)shards[numNewShards])->Init(baseShard, &newOutline, offset))
        {
            clGlasses->renderer->FreeShard((GlassShard *)shards[numNewShards]);
            break;
        }
        ShardGroup *group = (ShardGroup *)clGlasses->renderer->GetShardGroup(
            shards[numNewShards]->origin,
            baseShard->group->glassDef);
        if (!group)
        {
            clGlasses->renderer->FreeShard((GlassShard *)shards[numNewShards]);
            break;
        }
        group->Add((GlassShard *)shards[numNewShards++]);
    }
    return numNewShards;
}

// CoDMPServer @ 009973D0
void __cdecl GlassCl_AllocateMemory()
{
    if (!clGlasses)
    {
        Glasses *glasses = GetGlasses();
        if (glasses && glasses->numGlasses)
        {
            GlassesClient::InitAllocator(glasses);
            clGlasses = new GlassesClient(glasses);
        }
    }
}

// CoDMPServer @ 00997440
void __cdecl GlassCl_FreeMemory()
{
    if (clGlasses)
    {
        delete clGlasses;
        clGlasses = 0;
    }
}

// CoDMPServer @ 009974A0
void __cdecl GlassCl_Reset(int localClientNum)
{
    if (CL_LocalClient_IsFirstActive(localClientNum) && clGlasses)
    {
        clGlasses->Reset();
        clGlasses->renderer->Reset();
    }
}

// CoDMPServer @ 009974F0
void __cdecl GlassCl_Update(int localClientNum)
{
    if (clGlasses)
        clGlasses->Update(localClientNum);
}

// CoDMPServer @ 00997510
void __cdecl GlassCl_ReadGameState(int localClientNum, msg_t *msg)
{
    if (clGlasses)
        clGlasses->ParseSnapshot(localClientNum, msg, 1);
}

// CoDMPServer @ 00997540
void __cdecl GlassCl_ParseSnapshot(int localClientNum, msg_t *msg)
{
    if (clGlasses)
        clGlasses->ParseSnapshot(localClientNum, msg, 0);
}

// CoDMPServer @ 00997570
void __cdecl GlassCl_WriteDemoSnapshot(msg_t *msg)
{
    if (clGlasses)
        clGlasses->WriteDemoSnapshot(msg);
}

// CoDMPServer @ 00997590
void __cdecl GlassCl_WaitUpdate()
{
    PROF_SCOPED("GlassCl_WaitUpdate");
    if (clGlasses && clGlasses->renderer->smpGlass->current.enabled)
        clGlasses->renderer->BeginUpdate();
}

// CoDMPServer @ 009975E0 — retail calls NitrousVehicle::damage; PC build is a no-op
void __cdecl GlassCl_BeginGenerateVerts(unsigned int localClientNum, unsigned int viewIndex)
{
    (void)localClientNum;
    (void)viewIndex;
}

// CoDMPServer @ 00997630
void __cdecl GlassCl_WaitGenerateVerts()
{
    if (clGlasses && clGlasses->renderer->smpGlass->current.enabled)
    {
        PROF_SCOPED("GlassCl_WaitGenerateVerts");
        clGlasses->renderer->BeginUpdate();
    }
}

// CoDMPServer @ 009976B0
void __cdecl GlassCl_GenerateVerts(int localClientNum, unsigned int viewIndex)
{
    if (clGlasses && !clGlasses->renderer->smpGlass->current.enabled)
    {
        if (CL_LocalClient_IsFirstActive(localClientNum) && !viewIndex)
            clGlasses->renderer->Update(0);
        clGlasses->renderer->GenerateVerts(localClientNum, viewIndex, 0);
    }
}

// CoDMPServer @ 00997730
void __cdecl GlassCl_ExplosionEvent(
    int localClientNum,
    const float *origin,
    float damageInner,
    float damageOuter,
    float radius,
    int mod)
{
    if (clGlasses && CL_LocalClient_IsFirstActive(localClientNum))
        clGlasses->renderer->AddExplosionAction(origin, damageInner, damageOuter, radius, mod);
}

// CoDMPServer @ 00997790
void __cdecl GlassCl_TracePoint(const pointtrace_t *clip, trace_t *results)
{
    if (clGlasses)
    {
        float traceEnd[3];
        clGlasses->TracePoint(clip, results);
        Vec3Lerp(clip->extents.start.vec.v, clip->extents.end.vec.v, results->fraction, traceEnd);
        clGlasses->renderer->AddTraceAction(clip->extents.start.vec.v, traceEnd);
    }
}

// CoDMPServer @ 00997800
void __thiscall GlassesClient::TracePoint(const pointtrace_t *clip, trace_t *results)
{
    PROF_SCOPED("GlassesClient.TracePoint");

    float traceMins[3];
    float traceMaxs[3];
    const Glass *areaGlasses[128];

    Vec3Min(clip->extents.start.vec.v, clip->extents.end.vec.v, traceMins);
    Vec3Max(clip->extents.start.vec.v, clip->extents.end.vec.v, traceMaxs);
    const unsigned int glassCount = GlassesClient::AreaGlasses(traceMins, traceMaxs, areaGlasses, 0x80u);
    for (unsigned int glassIndex = 0; glassIndex < glassCount; ++glassIndex)
    {
        const Glass *glass = areaGlasses[glassIndex];
        const float oldFraction = results->fraction;
        CM_TransformedBoxTrace(
            results,
            clip->extents.start.vec.v,
            clip->extents.end.vec.v,
            vec3_origin,
            vec3_origin,
            glass->brushModel,
            clip->contentmask,
            glass->origin,
            glass->angles);
        if (results->fraction < oldFraction)
        {
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
            results->hitId = glass->index;
        }
    }
}

// CoDMPServer @ 009979C0
void __cdecl GlassCl_MeleeEvent(int localClientNum, int attackerEntNum)
{
    if (clGlasses && CL_LocalClient_IsFirstActive(localClientNum))
    {
        const centity_s *attacker = CG_GetEntity(localClientNum, attackerEntNum);
        if (attacker->nextState.weapon)
        {
            float eyePos[3];
            float right[3];
            float forward[3];
            float up[3];
            CG_CalcEyePoint(localClientNum, attackerEntNum, eyePos);
            CG_GetViewDirection(localClientNum, attackerEntNum, forward, right, up);
            const int meleeRange = BG_IsBayonetWeapon(attacker->nextState.weapon)
                ? player_bayonetRange->current.integer
                : player_meleeRange->current.integer;
            clGlasses->renderer->AddMeleeAction(eyePos, forward, right, up, *(float *)&meleeRange);
        }
    }
}

// CoDMPServer @ 00997AD0
void __cdecl GlassCl_DrawDebug()
{
    if (clGlasses)
        clGlasses->renderer->DrawDebug();
}

// CoDMPServer @ 00997B00
unsigned int __cdecl GlassCl_AreaGlasses(
    const float *mins,
    const float *maxs,
    const Glass **glasses,
    unsigned int maxGlasses)
{
    if (clGlasses)
        return clGlasses->AreaGlasses(mins, maxs, glasses, maxGlasses);
    return 0;
}

// CoDMPServer @ 00997B30
unsigned int __thiscall GlassesClient::AreaGlasses(
    const float *mins,
    const float *maxs,
    const Glass **glss,
    unsigned int maxGlasses)
{
    unsigned int num = 0;
    for (unsigned int glassIndex = 0; glassIndex < clGlasses->numGlasses && num < maxGlasses; ++glassIndex)
    {
        const GlassClient *glass = &this->glasses[glassIndex];
        if ((glass->state.val.i & 0xF) != GlassState::SHATTERED
            && maxs[0] >= glass->glass->absmin[0]
            && maxs[1] >= glass->glass->absmin[1]
            && maxs[2] >= glass->glass->absmin[2]
            && glass->glass->absmax[0] >= mins[0]
            && glass->glass->absmax[1] >= mins[1]
            && glass->glass->absmax[2] >= mins[2])
        {
            glss[num++] = glass->glass;
        }
    }
    return num;
}

// CoDMPServer @ 00997C50
void __cdecl GlassCl_ClipMoveTrace(const moveclip_t *clip, trace_t *results)
{
    if (clGlasses && (clip->contentmask & 0x10) != 0)
        clGlasses->ClipMoveTrace(clip, results);
}

// CoDMPServer @ 00997C80
void __thiscall GlassesClient::ClipMoveTrace(const moveclip_t *clip, trace_t *results)
{
    PROF_SCOPED("GlassesClient.ClipMoveTrace");

    float traceMins[3];
    float traceMaxs[3];
    const Glass *areaGlasses[128];

    for (int axisIndex = 0; axisIndex < 3; ++axisIndex)
    {
        if (clip->extents.end.vec.v[axisIndex] - clip->extents.start.vec.v[axisIndex] < 0.0f)
            traceMins[axisIndex] = clip->extents.end.vec.v[axisIndex];
        else
            traceMins[axisIndex] = clip->extents.start.vec.v[axisIndex];

        if (clip->extents.start.vec.v[axisIndex] + clip->mins[axisIndex] - traceMins[axisIndex] < 0.0f)
            traceMins[axisIndex] = clip->extents.start.vec.v[axisIndex] + clip->mins[axisIndex];
        else
            traceMins[axisIndex] = traceMins[axisIndex];

        if (clip->extents.end.vec.v[axisIndex] + clip->mins[axisIndex] - traceMins[axisIndex] < 0.0f)
            traceMins[axisIndex] = clip->extents.end.vec.v[axisIndex] + clip->mins[axisIndex];
        else
            traceMins[axisIndex] = traceMins[axisIndex];

        if (clip->extents.start.vec.v[axisIndex] - clip->extents.end.vec.v[axisIndex] < 0.0f)
            traceMaxs[axisIndex] = clip->extents.end.vec.v[axisIndex];
        else
            traceMaxs[axisIndex] = clip->extents.start.vec.v[axisIndex];

        if (traceMaxs[axisIndex] - (clip->extents.start.vec.v[axisIndex] + clip->maxs[axisIndex]) < 0.0f)
            traceMaxs[axisIndex] = clip->extents.start.vec.v[axisIndex] + clip->maxs[axisIndex];
        else
            traceMaxs[axisIndex] = traceMaxs[axisIndex];

        if (traceMaxs[axisIndex] - (clip->extents.end.vec.v[axisIndex] + clip->maxs[axisIndex]) < 0.0f)
            traceMaxs[axisIndex] = clip->extents.end.vec.v[axisIndex] + clip->maxs[axisIndex];
        else
            traceMaxs[axisIndex] = traceMaxs[axisIndex];
    }

    const unsigned int glassCount = GlassesClient::AreaGlasses(traceMins, traceMaxs, areaGlasses, 0x80u);
    for (unsigned int glassIndex = 0; glassIndex < glassCount; ++glassIndex)
    {
        const Glass *glass = areaGlasses[glassIndex];
        const float oldFraction = results->fraction;
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
            results->hitId = glass->index;
        }
    }
}
