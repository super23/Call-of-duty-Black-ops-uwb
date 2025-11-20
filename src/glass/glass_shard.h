#pragma once

void __cdecl GlassShard::Defrag(GlassShard *ptr);
void __userpurge GlassPhysics::CreateAxis(
        GlassPhysics *this@<ecx>,
        int a2@<ebp>,
        float *position,
        const float (*axis)[3],
        float *tVel,
        float *aVel,
        float mass,
        float *localBBoxMin,
        float *localBBoxMax);
void __userpurge GlassPhysics::AddForce(GlassPhysics *this@<ecx>, int a2@<ebp>, float *worldPos, float *impulse);
void __thiscall GlassPhysics::SetVelocity(GlassPhysics *this, float *t_vel);
void __thiscall GlassPhysics::SetAngularVelocity(GlassPhysics *this, float *a_vel);
void __thiscall GlassPhysics::Step(GlassPhysics *this, float deltaTime);
void __userpurge GlassPhysics::tensor_transform_principle(
        GlassPhysics *this@<ecx>,
        int a2@<ebp>,
        const phys_vec3 *diag,
        const phys_mat44 *mat,
        phys_mat44 *tensor);
void __userpurge GlassPhysics::IntegrateVelocity(GlassPhysics *this@<ecx>, int a2@<ebp>, float deltaTime);
phys_vec3 *__thiscall phys_vec3::operator+=(phys_vec3 *this, const phys_vec3 *v);
void __userpurge GlassPhysics::IntegratePos(GlassPhysics *this@<ecx>, int a2@<ebp>, float deltaTime);
void __thiscall GlassPhysics::GetPosition(GlassPhysics *this, float *position, float (*axis)[3]);
int __thiscall GlassShard::Outline::Init(
        GlassShard::Outline *this,
        GlassShard::Outline::Vertex *ptr,
        unsigned __int8 num);
int __thiscall GlassShard::Outline::SetPointers(GlassShard::Outline *this, unsigned __int8 *ptr);
GlassShard::Outline *__thiscall GlassShard::Outline::operator=(GlassShard::Outline *this, GlassShard::Outline *o);
double __thiscall GlassShard::Outline::Length(GlassShard::Outline *this);
GlassShard::Outline::Vertex *__thiscall GlassShard::Outline::Add(GlassShard::Outline *this, const float *pos);
GlassShard::Outline::Vertex *__thiscall GlassShard::Outline::Add(GlassShard::Outline *this, float *dir, float len);
char __thiscall GlassShard::Outline::CloseOutline(GlassShard::Outline *this);
void __thiscall GlassShard::Outline::Vertex::operator=(
        GlassShard::Outline::Vertex *this,
        const GlassShard::Outline::Vertex *other);
void __thiscall GlassShard::Outline::GetBBox(GlassShard::Outline *this, float *mn, float *mx);
double __thiscall GlassShard::Outline::Extent(GlassShard::Outline *this);
void __thiscall GlassShard::Outline::Recenter(GlassShard::Outline *this, bool flip, float *offset);
bool __thiscall GlassShard::Outline::HasNarrowAngle(GlassShard::Outline *this);
bool __thiscall GlassShard::Outline::IsNarrow(GlassShard::Outline *this);
void __thiscall GlassShard::Outline::Reverse(GlassShard::Outline *this);
int __thiscall GlassShard::Outline::GetNumIntersections(
        GlassShard::Outline *this,
        const float *start,
        const float *dir,
        float *nearestDist,
        int *nearestedge);
double __cdecl GetSegmentParam(const float *a1, const float *a2, const float *p);
bool __cdecl Vec2IntersectSegments(const float *a1, const float *a2, const float *b1, const float *b2, float *ret);
char __cdecl Vec2IntesectLines(const float *a1, const float *a2, const float *b1, const float *b2, float *ret);
char __thiscall GlassShard::Outline::DoesIntersect(
        GlassShard::Outline *this,
        const float *start,
        const float *dir,
        float len,
        float padding);
void __thiscall GlassShard::Outline::GetNearestDistances(
        GlassShard::Outline *this,
        const float *p,
        GlassShard::Outline::EdgeDistance *dists,
        int nDists);
void __thiscall GlassShard::Outline::Verify(GlassShard::Outline *this);
void __thiscall GlassShard::Mesh::Clear(GlassShard::Mesh *this);
void __thiscall GlassShard::Mesh::SetTriangles(
        GlassShard::Mesh *this,
        unsigned __int8 *ptr,
        unsigned __int8 *triangles,
        unsigned int numTriIdxs);
int __thiscall GlassShard::Mesh::SetPointers(GlassShard::Mesh *this, PackedUnitVec *ptr);
void __thiscall GlassShard::Mesh::Init(
        GlassShard::Mesh *this,
        const GlassShard::Outline *outline,
        float thickness,
        const float *tangt);
void __cdecl GlassShard::Mesh::InitVertexList(unsigned __int8 numOutlineVerts, GlassShardMeshVertex *verts);
unsigned int __cdecl GlassShard::Mesh::GetMemorySize(unsigned int numOutineVerts);
void __thiscall GlassShard::Triangles::Triangles(GlassShard::Triangles *this, const GlassShard::Outline *ol);
double __thiscall GlassShard::Triangles::CalcCross(GlassShard::Triangles *this, int idx1, int idx2, int idx3);
char __thiscall GlassShard::Triangles::AddTri(
        GlassShard::Triangles *this,
        unsigned __int8 v1,
        unsigned __int8 v2,
        unsigned __int8 v3);
char __thiscall GlassShard::Triangles::Triangulate(GlassShard::Triangles *this);
bool __cdecl IsInside(const float *v1, const float *v2, const float *v3, const float *p);
void __thiscall GlassShard::Init(GlassShard *this);
void __thiscall GlassShard::Destroy(GlassShard *this);
double __thiscall GlassShard::EdgeRatio(GlassShard *this);
void __thiscall GlassShard::UnEdge(GlassShard *this);
void __thiscall GlassShard::Outline::MarkAllEdge(GlassShard::Outline *this, bool e);
char __thiscall GlassShard::Create(GlassShard *this, const Glass *glass);
bool __thiscall GlassShard::CanSplit(GlassShard *this, float maxShardSize, float minShardSize);
bool __thiscall GlassShard::CanSplit(GlassShard *this, bool shatter);
int __thiscall GlassShard::Shatter(GlassShard *this, GlassShard **newShards, int maxNewShards);
int __cdecl compareShards(const void *s1, const GlassShard **s2);
void __thiscall GlassShard::InitPhysics(
        GlassShard *this,
        GlassShard **newShards,
        int numNewShards,
        float glassExtent,
        const float *pos,
        const float *dir);
void __thiscall GlassShard::InitMesh(GlassShard *this);
char __thiscall GlassShard::AllocateMemory(
        GlassShard *this,
        unsigned int nHull,
        const GlassShard::Triangles *triangles);
void __thiscall GlassShard::FreeMemory(GlassShard *this);
void __thiscall GlassShard::Defrag(GlassShard *this);
void __thiscall GlassShard::ToWorldPos(GlassShard *this, float *pLocal, float *pWorld, bool is3D);
void __thiscall GlassShard::ToWorldDir(GlassShard *this, float *dLocal, float *dWorld, bool is3D);
void __thiscall GlassShard::ToLocal(GlassShard *this, float *pos, float *dir, float *localPos, float *localDir);
bool __thiscall GlassShard::Intersect(GlassShard *this, float *pos, float *dir, float *hitPoint);
void __thiscall GlassShard::ExplosionEvent(
        GlassShard *this,
        const float *expOrigin,
        float damageInner,
        float damageOuter,
        float radius,
        int mod);
char __thiscall GlassShard::TracePoint(
        GlassShard *this,
        float *p0,
        const float *p1,
        float *dir,
        float length,
        const float *mins,
        float *maxs);
void __cdecl Vec3Avg(const float *a, const float *b, float *sum);
void __cdecl Vec3Mul(const float *a, const float *b, float *product);
void __thiscall GlassShard::GetLocalBBox(GlassShard *this, float *mins, float *maxs);
void __thiscall GlassShard::UpdateBBox(GlassShard *this);
void __thiscall GlassShard::Update(GlassShard *this, float deltaTime);
void __thiscall GlassShard::ChangeGroup(GlassShard *this);
void __thiscall GlassShard::Remove(GlassShard *this, GlassShard::RemoveReason reason, bool delay);
void __thiscall GlassShard::GenerateVerts(
        GlassShard *this,
        bool highLod,
        GfxPackedVertex *baseVerts,
        unsigned __int16 vertsBaseIndex,
        unsigned __int16 *idxOut);
int __thiscall GlassShard::Split(
        GlassShard *this,
        GlassShard **newShards,
        float minShardSize,
        unsigned int startEdge,
        float startEdgeParam);
int __thiscall GlassShard::Chip(
        GlassShard *this,
        const GlassShard::Outline::EdgeDistance *dist,
        const float *hitPoint,
        GlassShard **newShards,
        float minShardSize);
int __thiscall GlassShard::InitSplitShards(
        GlassShard *this,
        GlassShard::Outline *outline1,
        GlassShard::Outline *outline2,
        GlassShard **newShards,
        float minShardSize);
char __thiscall GlassShard::Init(
        GlassShard *this,
        const GlassShard *other,
        GlassShard::Outline *newOutline,
        float *offset);
double __thiscall GlassShard::GetMass(GlassShard *this);
bool __thiscall GlassShard::InitPhysicsObj(GlassShard *this, bool enableCollisions);
void __thiscall GlassShard::DestroyPhysicsObj(GlassShard *this);
void __thiscall GlassShard::DisableCollisions(GlassShard *this);
void __thiscall GlassShard::AddForce(GlassShard *this, float *pos, const float *forceIn);
void __thiscall GlassShard::SetVelocity(GlassShard *this, float *t_vel, float *a_vel);
bool __thiscall GlassShard::LeaveOnEdge(GlassShard *this, float stickiness);
bool __thiscall GlassShard::IsOnBottomEdge(GlassShard *this);
void __thiscall GlassShard::Recenter(GlassShard *this);
char __thiscall GlassShard::InitPhysics(
        GlassShard *this,
        bool enableCollisions,
        const float *hitPos,
        const float *hitDir,
        float glassSize,
        float stickiness);
void __thiscall GlassShard::DrawOutline(GlassShard *this);
