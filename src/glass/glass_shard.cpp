#include "glass_shard.h"
#include "glass.h"
#include "glass_renderer.h"
#include <universal/q_shared.h>
#include <physics/phys_util.h>
#include <bgame/bg_slidemove.h>
#include <tl/tl_system.h>
#include "glass_client.h"
#include <physics/physpreset_load_obj.h>
#include <client/cl_debugdata.h>
#include <gfx_d3d/r_bsp.h>

float hitPos[3];

int GlassShard::splitFailCount[8];
int GlassShard::lastFreeMemorySize;
int GlassShard::removeReasonsCount[KISAK_TOTAL];

//void __cdecl GlassShard::Defrag(GlassShard *ptr)
//{
//    ptr->Defrag();
//}

// Decomp: CoDMPServer.c:857730
void GlassShard_Defrag(void *ptr) // changed from static member func
{
    GlassShard *shard = (GlassShard *)ptr;
    shard->Defrag();
}


// Decomp: CoDMPServer.c:860562
void GlassPhysics::CreateAxis(
    float *position,
    const float (*axis)[3],
    float *tVel,
    float *aVel,
    float mass,
    float *localBBoxMin,
    float *localBBoxMax)
{
    // --------------------------------------------------
    // Compute local bounding box dimensions
    // --------------------------------------------------
    phys_vec3 bbMin, bbMax;
    Phys_Vec3ToNitrousVec(localBBoxMin, &bbMin);
    Phys_Vec3ToNitrousVec(localBBoxMax, &bbMax);

    phys_vec3 dim;
    dim.x = bbMax.x - bbMin.x;
    dim.y = bbMax.y - bbMin.y;
    dim.z = bbMax.z - bbMin.z;

    // --------------------------------------------------
    // Compute inertia tensor (box)
    // --------------------------------------------------
    phys_vec3 inertia;
    float volume;

    nuge::calc_box_inertia(&dim, &inertia, &volume);

    float scale = mass / volume;
    inertia.x *= scale;
    inertia.y *= scale;
    inertia.z *= scale;

    // --------------------------------------------------
    // Build model-to-world transform
    // --------------------------------------------------
    phys_mat44 m2w = PHYS_IDENTITY_MATRIX;

    // rotation
    m2w.x.x = axis[0][0]; m2w.x.y = axis[0][1]; m2w.x.z = axis[0][2];
    m2w.y.x = axis[1][0]; m2w.y.y = axis[1][1]; m2w.y.z = axis[1][2];
    m2w.z.x = axis[2][0]; m2w.z.y = axis[2][1]; m2w.z.z = axis[2][2];

    // translation
    m2w.w.x = position[0];
    m2w.w.y = position[1];
    m2w.w.z = position[2];

    // --------------------------------------------------
    // Center-of-mass adjustment
    // --------------------------------------------------
    phys_vec3 center;
    center.x = 0.5f * (bbMin.x + bbMax.x);
    center.y = 0.5f * (bbMin.y + bbMax.y);
    center.z = 0.5f * (bbMin.z + bbMax.z);

    phys_mat44 cg2rb = PHYS_IDENTITY_MATRIX;
    cg2rb.w.x = -center.x;
    cg2rb.w.y = -center.y;
    cg2rb.w.z = -center.z;

    phys_mat44 rb2w;
    phys_full_inverse(&rb2w, &cg2rb);

    phys_mat44 finalMat;
    phys_full_multiply_mat(&finalMat, &m2w, &rb2w);

    // --------------------------------------------------
    // Store mass + inertia
    // --------------------------------------------------
    m_inv_mass = 1.0f / mass;

    //m_inv_inertia = PHYS_IDENTITY_MATRIX;
    m_inv_inertia.x = 1.0f / inertia.x;
    m_inv_inertia.y = 1.0f / inertia.y;
    m_inv_inertia.z = 1.0f / inertia.z;

    m_mat = finalMat;

    // --------------------------------------------------
    // Velocities
    // --------------------------------------------------
    m_t_vel.x = tVel[0];
    m_t_vel.y = tVel[1];
    m_t_vel.z = tVel[2];

    m_a_vel.x = aVel[0];
    m_a_vel.y = aVel[1];
    m_a_vel.z = aVel[2];

    // --------------------------------------------------
    // Clear accumulators
    // --------------------------------------------------
    m_force_sum = PHYS_ZERO_VEC;
    m_torque_sum = PHYS_ZERO_VEC;
}


// Decomp: CoDMPServer.c:860745
void GlassPhysics::AddForce(float *worldPos, float *impulse)
{
    phys_vec3 force;
    force.x = impulse[0] * 0.001f;
    force.y = impulse[1] * 0.001f;
    force.z = impulse[2] * 0.001f;

    iassert(Abs(force) < 1000000.0f);

    const float posX = worldPos[0];
    const float posY = worldPos[1];
    const float posZ = worldPos[2];
    const float posLenSq = posX * posX + posY * posY + posZ * posZ;

    if (posLenSq == 0.0f)
    {
        m_force_sum.x += force.x;
        m_force_sum.y += force.y;
        m_force_sum.z += force.z;
    }
    else
    {
        m_force_sum.x += force.x;
        m_force_sum.y += force.y;
        m_force_sum.z += force.z;

        phys_vec3 offset;
        offset.x = posX - m_mat.w.x;
        offset.y = posY - m_mat.w.y;
        offset.z = posZ - m_mat.w.z;

        phys_vec3 torque;
        phys_cross(&torque, &offset, &force);
        m_torque_sum.x += torque.x;
        m_torque_sum.y += torque.y;
        m_torque_sum.z += torque.z;
    }
}

// Decomp: CoDMPServer.c:860846
void __thiscall GlassPhysics::SetVelocity(float *t_vel)
{
    m_t_vel.x = t_vel[0];
    m_t_vel.y = t_vel[1];
    m_t_vel.z = t_vel[2];
}

// Decomp: CoDMPServer.c:860873
void __thiscall GlassPhysics::SetAngularVelocity(float *a_vel)
{
    m_a_vel.x = a_vel[0];
    m_a_vel.y = a_vel[1];
    m_a_vel.z = a_vel[2];
}

// Decomp: CoDMPServer.c:860888
void __thiscall GlassPhysics::Step(float deltaTime)
{
    GlassPhysics::IntegrateVelocity(deltaTime);
    GlassPhysics::IntegratePos(deltaTime);
    this->m_force_sum.x = PHYS_ZERO_VEC.x;
    this->m_force_sum.y = PHYS_ZERO_VEC.y;
    this->m_force_sum.z = PHYS_ZERO_VEC.z;
    this->m_torque_sum.x = PHYS_ZERO_VEC.x;
    this->m_torque_sum.y = PHYS_ZERO_VEC.y;
    this->m_torque_sum.z = PHYS_ZERO_VEC.z;
}


// Decomp: CoDMPServer.c:860909
void GlassPhysics::tensor_transform_principle(
    const phys_vec3 *diag,
    const phys_mat44 *mat,
    phys_mat44 *tensor)
{
    const float rot[3][3] = {
        { mat->x.x, mat->x.y, mat->x.z },
        { mat->y.x, mat->y.y, mat->y.z },
        { mat->z.x, mat->z.y, mat->z.z },
    };
    const float principalInertia[3] = { diag->x, diag->y, diag->z };

    // scaledInertia[row][col] = rot[row][col] * principalInertia[row]  (R * diag(I))
    const float scaledInertia00 = rot[0][0] * principalInertia[0];
    const float scaledInertia01 = rot[0][1] * principalInertia[0];
    const float scaledInertia02 = rot[0][2] * principalInertia[0];
    const float scaledInertia10 = rot[1][0] * principalInertia[1];
    const float scaledInertia11 = rot[1][1] * principalInertia[1];
    const float scaledInertia12 = rot[1][2] * principalInertia[1];
    const float scaledInertia20 = rot[2][0] * principalInertia[2];
    const float scaledInertia21 = rot[2][1] * principalInertia[2];
    const float scaledInertia22 = rot[2][2] * principalInertia[2];

    // tensor = scaledInertia * R^T
    tensor->x.x = scaledInertia00 * rot[0][0] + scaledInertia10 * rot[1][0] + scaledInertia20 * rot[2][0];
    tensor->x.y = scaledInertia00 * rot[0][1] + scaledInertia10 * rot[1][1] + scaledInertia20 * rot[2][1];
    tensor->x.z = scaledInertia00 * rot[0][2] + scaledInertia10 * rot[1][2] + scaledInertia20 * rot[2][2];

    tensor->y.x = scaledInertia01 * rot[0][0] + scaledInertia11 * rot[1][0] + scaledInertia21 * rot[2][0];
    tensor->y.y = scaledInertia01 * rot[0][1] + scaledInertia11 * rot[1][1] + scaledInertia21 * rot[2][1];
    tensor->y.z = scaledInertia01 * rot[0][2] + scaledInertia11 * rot[1][2] + scaledInertia21 * rot[2][2];

    tensor->z.x = scaledInertia02 * rot[0][0] + scaledInertia12 * rot[1][0] + scaledInertia22 * rot[2][0];
    tensor->z.y = scaledInertia02 * rot[0][1] + scaledInertia12 * rot[1][1] + scaledInertia22 * rot[2][1];
    tensor->z.z = scaledInertia02 * rot[0][2] + scaledInertia12 * rot[1][2] + scaledInertia22 * rot[2][2];
}


// Decomp: CoDMPServer.c:861067
void GlassPhysics::IntegrateVelocity(float deltaTime)
{
    // --------------------------------------------
    // Convert impulse-style accumulators to forces
    // --------------------------------------------
    const float invDt = 1.0f / deltaTime;

    m_force_sum.x *= invDt;
    m_force_sum.y *= invDt;
    m_force_sum.z *= invDt;

    m_torque_sum.x *= invDt;
    m_torque_sum.y *= invDt;
    m_torque_sum.z *= invDt;

    // --------------------------------------------
    // Gravity acceleration
    // --------------------------------------------
    phys_vec3 gravityDir;
    Phys_Vec3ToNitrousVec(&phys_gravity_dir->current.value, &gravityDir);

    const float gravityMag = phys_gravity->current.value;

    phys_vec3 gravityAcc;
    gravityAcc.x = gravityMag * gravityDir.x;
    gravityAcc.y = gravityMag * gravityDir.y;
    gravityAcc.z = gravityMag * gravityDir.z;

    // --------------------------------------------
    // Linear acceleration
    // --------------------------------------------
    phys_vec3 linAcc;
    linAcc.x = m_inv_mass * m_force_sum.x + gravityAcc.x;
    linAcc.y = m_inv_mass * m_force_sum.y + gravityAcc.y;
    linAcc.z = m_inv_mass * m_force_sum.z + gravityAcc.z;

    // Integrate linear velocity
    m_t_vel.x += linAcc.x * deltaTime;
    m_t_vel.y += linAcc.y * deltaTime;
    m_t_vel.z += linAcc.z * deltaTime;

    // --------------------------------------------
    // Angular acceleration (world space)
    // --------------------------------------------
    phys_mat44 worldInvInertia;
    tensor_transform_principle(&m_inv_inertia, &m_mat, &worldInvInertia);

    phys_vec3 angAcc;
    phys_multiply(&angAcc, &worldInvInertia, &m_torque_sum);

    // Integrate angular velocity
    m_a_vel.x += angAcc.x * deltaTime;
    m_a_vel.y += angAcc.y * deltaTime;
    m_a_vel.z += angAcc.z * deltaTime;

    // --------------------------------------------
    // Gyroscopic term
    // --------------------------------------------
    phys_vec3 omegaLocal;
    phys_inv_multiply(&omegaLocal, &m_mat, &m_a_vel);

    phys_vec3 angMomentumLocal;
    angMomentumLocal.x = omegaLocal.x / m_inv_inertia.x;
    angMomentumLocal.y = omegaLocal.y / m_inv_inertia.y;
    angMomentumLocal.z = omegaLocal.z / m_inv_inertia.z;

    phys_vec3 gyroLocal;
    phys_cross(&gyroLocal, &angMomentumLocal, &omegaLocal);

    phys_vec3 gyroWorld;
    gyroLocal.x *= m_inv_inertia.x;
    gyroLocal.y *= m_inv_inertia.y;
    gyroLocal.z *= m_inv_inertia.z;

    phys_multiply(&gyroWorld, &m_mat, &gyroLocal);

    m_a_vel.x += gyroWorld.x * deltaTime;
    m_a_vel.y += gyroWorld.y * deltaTime;
    m_a_vel.z += gyroWorld.z * deltaTime;

    // --------------------------------------------
    // Clamp angular velocity
    // --------------------------------------------
    float omegaSq = AbsSquared(&m_a_vel);
    const float maxOmega = 16.0f;

    if (omegaSq > maxOmega * maxOmega)
    {
        float scale = maxOmega / sqrtf(omegaSq);
        m_a_vel.x *= scale;
        m_a_vel.y *= scale;
        m_a_vel.z *= scale;
    }

    // --------------------------------------------
    // Linear drag
    // --------------------------------------------
    float linDrag = deltaTime * phys_dragLinear->current.value;
    if (linDrag > 0.9f) linDrag = 0.9f;

    const float linDamp = 1.0f - linDrag;

    m_t_vel.x *= linDamp;
    m_t_vel.y *= linDamp;
    m_t_vel.z *= linDamp;

    // --------------------------------------------
    // Angular drag
    // --------------------------------------------
    float angDrag = deltaTime * phys_dragAngular->current.value;
    if (angDrag > 0.9f) angDrag = 0.9f;

    const float angDamp = 1.0f - angDrag;

    m_a_vel.x *= angDamp;
    m_a_vel.y *= angDamp;
    m_a_vel.z *= angDamp;
}



// Decomp: CoDMPServer.c:861312
void GlassPhysics::IntegratePos(float deltaTime)
{
    m_mat.w.x += deltaTime * m_t_vel.x;
    m_mat.w.y += deltaTime * m_t_vel.y;
    m_mat.w.z += deltaTime * m_t_vel.z;

    phys_mat44 rotDelta;
    make_rotate(&rotDelta, &m_a_vel, deltaTime, 1000.0f);
    phys_multiply_mat(&m_mat, &rotDelta, &m_mat);
}

// Decomp: CoDMPServer.c:861349
void __thiscall GlassPhysics::GetPosition(float *position, float (*axis)[3])
{
    Phys_NitrousVecToVec3(&this->m_mat.w, position);
    Phys_NitrousMat44ToVec33(&this->m_mat, axis);
}

// Decomp: CoDMPServer.c:861359
int __thiscall GlassShard::Outline::Init(
                GlassShard::Outline::Vertex *ptr,
                unsigned __int8 num)
{
    this->verts = ptr;
    this->maxVerts = num;
    this->numVerts = 0;
    return 24 * this->maxVerts;
}

// Decomp: CoDMPServer.c:861368
int __thiscall GlassShard::Outline::SetPointers(unsigned __int8 *ptr)
{
    this->verts = (GlassShard::Outline::Vertex *)ptr;
    return 24 * this->numVerts;
}

// Decomp: CoDMPServer.c:861375
GlassShard::Outline &__thiscall GlassShard::Outline::operator=(const GlassShard::Outline &o)
{
    iassert(verts);
    iassert(o.verts);
    iassert(maxVerts >= o.numVerts);

    this->isClosed = o.isClosed;
    this->numVerts = o.numVerts;
    memcpy(this->verts, o.verts, sizeof(Outline::Vertex) * this->numVerts);
    this->length = o.Length();
    this->area = o.Area();
    GlassShard::Outline::Verify();
    return *this;
}

// Decomp: CoDMPServer.c:861423
double __thiscall GlassShard::Outline::Length() const
{
    if ( !this->isClosed
        && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.h", 145, 0, "%s", "isClosed") )
    {
        __debugbreak();
    }
    return this->length;
}

// Decomp: CoDMPServer.c:861443
GlassShard::Outline::Vertex *__thiscall GlassShard::Outline::Add(const float *pos)
{
    if (this->numVerts >= (int)this->maxVerts)
        return &this->verts[this->numVerts - 1];

    if (this->numVerts)
    {
        GlassShard::Outline::Vertex *prevVert = &this->verts[this->numVerts - 1];
        float *prevDir = prevVert->edge.dir;
        prevDir[0] = pos[0] - prevVert->edge.origin[0];
        prevDir[1] = pos[1] - prevVert->edge.origin[1];
        prevVert->edge.len = Vec2Normalize(prevDir);
    }

    GlassShard::Outline::Vertex *newVert = &this->verts[this->numVerts];
    newVert->edge.origin[0] = pos[0];
    newVert->edge.origin[1] = pos[1];
    newVert->edge.dir[0] = 0.0f;
    newVert->edge.dir[1] = 0.0f;
    newVert->edge.len = 0.0f;
    newVert->isOriginalEdge = 0;
    ++this->numVerts;
    return &this->verts[this->numVerts - 1];
}

// Decomp: CoDMPServer.c:861477
GlassShard::Outline::Vertex *__thiscall GlassShard::Outline::Add(float *dir, float len)
{
    iassert(this->numVerts > 0);

    if (this->numVerts >= (int)this->maxVerts)
        return &this->verts[this->numVerts - 1];

    GlassShard::Outline::Vertex *prevVert = &this->verts[this->numVerts - 1];
    prevVert->edge.dir[0] = dir[0];
    prevVert->edge.dir[1] = dir[1];
    prevVert->edge.len = len;

    GlassShard::Outline::Vertex *newVert = &this->verts[this->numVerts];
    newVert->edge.origin[0] = prevVert->edge.len * prevVert->edge.dir[0] + prevVert->edge.origin[0];
    newVert->edge.origin[1] = prevVert->edge.len * prevVert->edge.dir[1] + prevVert->edge.origin[1];
    newVert->edge.dir[0] = 0.0f;
    newVert->edge.dir[1] = 0.0f;
    newVert->edge.len = 0.0f;
    newVert->isOriginalEdge = 0;
    ++this->numVerts;
    return &this->verts[this->numVerts - 1];
}

// Decomp: CoDMPServer.c:861519
char __thiscall GlassShard::Outline::CloseOutline()
{
    iassert(this->numVerts > 2);

    GlassShard::Outline::Vertex *lastVert = &this->verts[this->numVerts - 1];
    float *closingDir = lastVert->edge.dir;
    closingDir[0] = this->verts[0].edge.origin[0] - lastVert->edge.origin[0];
    closingDir[1] = this->verts[0].edge.origin[1] - lastVert->edge.origin[1];
    lastVert->edge.len = Vec2Normalize(closingDir);

    unsigned int vertIndex = 0;
    while (vertIndex < this->numVerts)
    {
        if (this->verts[vertIndex].edge.len >= 0.0099999998f)
        {
            ++vertIndex;
            continue;
        }

        if (this->numVerts == 3)
            return 0;

        for (unsigned int shiftIndex = vertIndex; shiftIndex < this->numVerts - 1; ++shiftIndex)
            this->verts[shiftIndex] = this->verts[shiftIndex + 1];
        --this->numVerts;

        GlassShard::Outline::Vertex *curVert = &this->verts[vertIndex];
        const unsigned int nextVertIndex =
            ((signed int)-abs((int)vertIndex - (this->numVerts - 1)) >> 31) & (vertIndex + 1);
        GlassShard::Outline::Vertex *nextVert = &this->verts[nextVertIndex];
        float *edgeDir = curVert->edge.dir;
        edgeDir[0] = nextVert->edge.origin[0] - curVert->edge.origin[0];
        edgeDir[1] = nextVert->edge.origin[1] - curVert->edge.origin[1];
        curVert->edge.len = Vec2Normalize(edgeDir);
        vertIndex = 0;
    }

    this->length = 0.0f;
    this->area = 0.0f;
    for (unsigned int vertIndex = 0; vertIndex < this->numVerts; ++vertIndex)
    {
        const unsigned int nextVertIndex =
            ((signed int)-fabs((int)vertIndex - (this->numVerts - 1)) >> 31) & (vertIndex + 1);
        this->length += this->verts[vertIndex].edge.len;
        this->area += (this->verts[vertIndex].edge.origin[0] * this->verts[nextVertIndex].edge.origin[1]
                     - this->verts[vertIndex].edge.origin[1] * this->verts[nextVertIndex].edge.origin[0])
                    * 0.5f;
    }
    this->area = fabs(this->area);
    this->isClosed = 1;
    GlassShard::Outline::Verify();
    return 1;
}

// Decomp: CoDMPServer.c:861599
void __thiscall GlassShard::Outline::Vertex::operator=(const GlassShard::Outline::Vertex *other)
{
    *this = *other;
}

// Decomp: CoDMPServer.c:861613
void __thiscall GlassShard::Outline::GetBBox(float *mn, float *mx)
{
    *mn = this->verts[0].edge.origin[0];
    mn[1] = this->verts[0].edge.origin[1];
    *mx = this->verts[0].edge.origin[0];
    mx[1] = this->verts[0].edge.origin[1];

    for (unsigned int vertIndex = 1; vertIndex < this->numVerts; ++vertIndex)
    {
        const GlassShard::Outline::Vertex *vert = &this->verts[vertIndex];
        if (vert->edge.origin[0] < *mn)
            *mn = vert->edge.origin[0];
        if (vert->edge.origin[1] < mn[1])
            mn[1] = vert->edge.origin[1];
        if (*mx < vert->edge.origin[0])
            *mx = vert->edge.origin[0];
        if (*mx < vert->edge.origin[1])
            mx[1] = vert->edge.origin[1];
    }
}

// Decomp: CoDMPServer.c:861659
double __thiscall GlassShard::Outline::Extent()
{
    float mn[2];
    float mx[2];
    float extent[2];

    GlassShard::Outline::GetBBox(mn, mx);
    extent[0] = mx[0] - mn[0];
    extent[1] = mx[1] - mn[1];
    return Vec2Length(extent);
}

// Decomp: CoDMPServer.c:861675
void __thiscall GlassShard::Outline::Recenter(bool flip, float *offset)
{
    float mn[2];
    float mx[2];

    if (flip)
        GlassShard::Outline::Reverse();
    GlassShard::Outline::GetBBox(mn, mx);
    offset[0] = (mn[0] + mx[0]) * 0.5f;
    offset[1] = (mn[1] + mx[1]) * 0.5f;
    for (unsigned int vertIndex = 0; vertIndex < this->numVerts; ++vertIndex)
    {
        this->verts[vertIndex].edge.origin[0] -= offset[0];
        this->verts[vertIndex].edge.origin[1] -= offset[1];
    }
}

// Decomp: CoDMPServer.c:861703
bool __thiscall GlassShard::Outline::HasNarrowAngle()
{
    float minDot = 1.0f;
    for (unsigned int edgeIndex = 0; edgeIndex < this->numVerts; ++edgeIndex)
    {
        const unsigned int nextEdge = (edgeIndex + 1) % this->numVerts;
        const float dot = this->verts[edgeIndex].edge.dir[0] * this->verts[nextEdge].edge.dir[0]
                        + this->verts[edgeIndex].edge.dir[1] * this->verts[nextEdge].edge.dir[1];
        if (dot < minDot)
            minDot = dot;
    }
    return minDot < -0.89999998f;
}

// Decomp: CoDMPServer.c:861727
// Outline::Area() is inline in glass_shard.h (retail caches area in CloseOutline)
bool __thiscall GlassShard::Outline::IsNarrow()
{
    float mn[2];
    float mx[2];
    GlassShard::Outline::GetBBox(mn, mx);

    const float extentX = mx[0] - mn[0];
    const float extentY = mx[1] - mn[1];
    const float longExtent = (extentX < extentY) ? extentY : extentX;
    const float shortExtent = (extentY < extentX) ? extentY : extentX;

    if (longExtent / shortExtent > clGlasses->renderer->narrowShardRatio->current.value)
        return true;

    const float bboxArea = extentX * extentY;
    return bboxArea / this->area > clGlasses->renderer->narrowShardRatio->current.value;
}

// Decomp: CoDMPServer.c:861761
void __thiscall GlassShard::Outline::Reverse()
{
    GlassShard::Outline::Vertex tmpVerts[64];

    iassert(this->isClosed);

    for (unsigned int vertIndex = 0; vertIndex < this->numVerts; ++vertIndex)
    {
        const int reversedIndex = this->numVerts - 1 - vertIndex;
        const int prevReversedIndex = (reversedIndex <= 0) ? (this->numVerts - 1) : (reversedIndex - 1);
        GlassShard::Outline::Vertex *outVert = &tmpVerts[vertIndex];
        const GlassShard::Outline::Vertex *srcOriginVert = &this->verts[reversedIndex];
        const GlassShard::Outline::Vertex *srcDirVert = &this->verts[prevReversedIndex];

        outVert->edge.origin[0] = srcOriginVert->edge.origin[0];
        outVert->edge.origin[1] = srcOriginVert->edge.origin[1];
        outVert->edge.dir[0] = -srcDirVert->edge.dir[0];
        outVert->edge.dir[1] = -srcDirVert->edge.dir[1];
        outVert->edge.len = srcDirVert->edge.len;
        outVert->isOriginalEdge = srcDirVert->isOriginalEdge;
    }
    memcpy(this->verts, tmpVerts, 24 * this->numVerts);
    GlassShard::Outline::Verify();
}

// Decomp: CoDMPServer.c:861814
int __thiscall GlassShard::Outline::GetNumIntersections(
                const float *start,
                const float *dir,
                float *nearestDist,
                int *nearestedge) const
{
    const float testLength = 1000.0f;
    float end[2];
    end[0] = testLength * dir[0] + start[0];
    end[1] = testLength * dir[1] + start[1];

    int hitCount = 0;
    *nearestDist = testLength;

    for (unsigned int edgeIndex = 0; edgeIndex < this->numVerts; ++edgeIndex)
    {
        float hitPoint[2];
        const unsigned int nextVert = (edgeIndex + 1) % this->numVerts;
        if ( !Vec2IntersectSegments(
                 this->verts[edgeIndex].edge.origin,
                 this->verts[nextVert].edge.origin,
                 start,
                 end,
                 hitPoint) )
        {
            continue;
        }

        const float dist = GetSegmentParam(start, end, hitPoint) * testLength;
        if ( *nearestDist > dist )
        {
            *nearestDist = dist;
            *nearestedge = edgeIndex;
        }
        ++hitCount;
    }
    return hitCount;
}

// Decomp: CoDMPServer.c:861856
double __cdecl GetSegmentParam(const float *segStart, const float *segEnd, const float *point)
{
    const float deltaX = segEnd[0] - segStart[0];
    if (fabs(deltaX) < 0.0000099999997f)
        return (point[1] - segStart[1]) / (segEnd[1] - segStart[1]);
    return (point[0] - segStart[0]) / deltaX;
}

// Decomp: CoDMPServer.c:861871
bool __cdecl Vec2IntersectSegments(const float *a1, const float *a2, const float *b1, const float *b2, float *ret)
{
    if (!Vec2IntesectLines(a1, a2, b1, b2, ret))
        return false;

    const float tOnA = GetSegmentParam(a1, a2, ret);
    if (tOnA <= 0.0000099999997f || tOnA >= (1.0f - 0.0000099999997f))
        return false;

    const float tOnB = GetSegmentParam(b1, b2, ret);
    return tOnB > 0.0000099999997f && tOnB < (1.0f - 0.0000099999997f);
}

// Decomp: CoDMPServer.c:861892
char __cdecl Vec2IntesectLines(const float *a1, const float *a2, const float *b1, const float *b2, float *ret)
{
    const float det = (*a1 - *a2) * (b1[1] - b2[1]) - (a1[1] - a2[1]) * (*b1 - *b2);
    if ( fabs(det) <= 0.0000099999997 )
        return 0;
    *ret = (float)((float)((float)(*b1 - *b2) * (float)((float)(*a1 * a2[1]) - (float)(a1[1] * *a2)))
                             - (float)((float)(*a1 - *a2) * (float)((float)(*b1 * b2[1]) - (float)(b1[1] * *b2))))
             / det;
    ret[1] = (float)((float)((float)(b1[1] - b2[1]) * (float)((float)(*a1 * a2[1]) - (float)(a1[1] * *a2)))
                                 - (float)((float)(a1[1] - a2[1]) * (float)((float)(*b1 * b2[1]) - (float)(b1[1] * *b2))))
                 / det;
    return 1;
}

// Decomp: CoDMPServer.c:861910
char __thiscall GlassShard::Outline::DoesIntersect(
                const float *start,
                const float *dir,
                float len,
                float padding)
{
    float end[2];
    end[0] = len * dir[0] + start[0];
    end[1] = len * dir[1] + start[1];

    const unsigned int edgeCount = this->isClosed ? this->numVerts : (this->numVerts - 1);
    for (unsigned int edgeIndex = 0; edgeIndex < edgeCount; ++edgeIndex)
    {
        const GlassShard::Outline::Vertex *vert = &this->verts[edgeIndex];
        const float *edgeDir = vert->edge.dir;
        const float paddedLen = vert->edge.len + padding;
        float edgeStart[2];
        float edgeEnd[2];
        float hitPoint[2];

        edgeStart[0] = vert->edge.origin[0] - padding * edgeDir[0];
        edgeStart[1] = vert->edge.origin[1] - padding * edgeDir[1];
        edgeEnd[0] = vert->edge.origin[0] + paddedLen * edgeDir[0];
        edgeEnd[1] = vert->edge.origin[1] + paddedLen * edgeDir[1];
        if (Vec2IntersectSegments(edgeStart, edgeEnd, start, end, hitPoint))
            return 1;
    }
    return 0;
}

// Decomp: CoDMPServer.c:861956
void __thiscall GlassShard::Outline::GetNearestDistances(
                const float *p,
                GlassShard::Outline::EdgeDistance *dists,
                int nDists)
{
    for (int distSlotIndex = 0; distSlotIndex < nDists; ++distSlotIndex)
        dists[distSlotIndex].edgeIndex = -1;

    for (signed int edgeIndex = 0; edgeIndex < this->numVerts; ++edgeIndex)
    {
        const GlassShard::Outline::Vertex *vert = &this->verts[edgeIndex];
        const float len = vert->edge.len;
        const float lenSq = len * len;
        const float edgeParamRaw =
            (len * vert->edge.dir[0] * (p[0] - vert->edge.origin[0])
           + len * vert->edge.dir[1] * (p[1] - vert->edge.origin[1])) / lenSq;
        const float edgeParamClamped = (edgeParamRaw < 1.0f) ? edgeParamRaw : 1.0f;
        const float edgeParam = (edgeParamRaw > 0.0f) ? edgeParamClamped : 0.0f;
        const float alongEdge = edgeParam * len;
        float closestPoint[2];
        closestPoint[0] = alongEdge * vert->edge.dir[0] + vert->edge.origin[0];
        closestPoint[1] = alongEdge * vert->edge.dir[1] + vert->edge.origin[1];
        const float dx = p[0] - closestPoint[0];
        const float dy = p[1] - closestPoint[1];
        const float distSq = dx * dx + dy * dy;

        int insertIndex = 0;
        const int maxCompare = (nDists < edgeIndex) ? nDists : edgeIndex;
        for (; insertIndex < maxCompare; ++insertIndex)
        {
            if (dists[insertIndex].dist > distSq)
            {
                for (int shiftIndex = nDists - 1; shiftIndex > insertIndex; --shiftIndex)
                    dists[shiftIndex] = dists[shiftIndex - 1];
                break;
            }
        }

        if (insertIndex < nDists)
        {
            dists[insertIndex].edgeIndex = edgeIndex;
            dists[insertIndex].edgeParam = edgeParam;
            dists[insertIndex].dist = distSq;
            dists[insertIndex].closestPoint[0] = closestPoint[0];
            dists[insertIndex].closestPoint[1] = closestPoint[1];
        }
    }

    for (int distSlotIndex = 0; distSlotIndex < nDists; ++distSlotIndex)
    {
        iassert(dists[distSlotIndex].edgeIndex != (unsigned int)-1);
        dists[distSlotIndex].dist = sqrtf(dists[distSlotIndex].dist);
    }
}

// Decomp: CoDMPServer.c:862048
void __thiscall GlassShard::Outline::Verify() const
{
    if ( !this->isClosed
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 781, 0, "%s", "isClosed") )
    {
        __debugbreak();
    }
}

// Decomp: CoDMPServer.c:862064
void __thiscall GlassShard::Mesh::Clear()
{
    this->normArray = 0;
    this->indices = 0;
    *(unsigned int *)&this->numNorm = 0;
    *(unsigned int *)&this->numIndicesLow = 0;
}

// Decomp: CoDMPServer.c:862073
void __thiscall GlassShard::Mesh::SetTriangles(
                unsigned __int8 *ptr,
                unsigned __int8 *triangles,
                unsigned int numTriIdxs)
{
    GlassShard::Mesh::Clear();
    this->indices = ptr;
    memcpy(this->indices, triangles, numTriIdxs);
}

// Decomp: CoDMPServer.c:862084
int __thiscall GlassShard::Mesh::SetPointers(PackedUnitVec *ptr)
{
    this->normArray = ptr;
    this->indices = (unsigned __int8 *)&ptr[this->numNorm];
    return this->numIndices + 4 * this->numNorm;
}

// Decomp: CoDMPServer.c:862092
void __thiscall GlassShard::Mesh::Init(
                const GlassShard::Outline *outline,
                float thickness,
                const float *tangt)
{
    (void)tangt;
    if (this->normArray)
        return;

    const int numTriVerts = 3 * (outline->numVerts - 2);
    unsigned __int8 triangles[260];
    memcpy(triangles, this->indices, numTriVerts);

    unsigned __int8 *memPtr = this->indices;
    this->normArray = (PackedUnitVec *)memPtr;
    memPtr += 4 * outline->numVerts + 8;
    this->indices = memPtr;
    memPtr += 12 * outline->numVerts - 12;

    auto packSnormNormal = [](const float *n, PackedUnitVec *out)
    {
        out->array[0] = (int)(n[0] * 127.0f + 127.5f);
        out->array[1] = (int)(n[1] * 127.0f + 127.5f);
        out->array[2] = (int)(n[2] * 127.0f + 127.5f);
        out->array[3] = 63;
    };

    this->numNorm = 0;
    float frontNormal[3] = { 0.0f, 0.0f, 1.0f };
    packSnormNormal(frontNormal, &this->normArray[this->numNorm++]);
    float backNormal[3] = { 0.0f, 0.0f, -1.0f };
    packSnormNormal(backNormal, &this->normArray[this->numNorm++]);

    this->numIndices = 0;
    for (int triBase = 0; triBase < numTriVerts; triBase += 3)
    {
        this->indices[this->numIndices++] = 2 * triangles[triBase];
        this->indices[this->numIndices++] = 2 * triangles[triBase + 1];
        this->indices[this->numIndices++] = 2 * triangles[triBase + 2];
    }
    for (int triBase = 0; triBase < numTriVerts; triBase += 3)
    {
        this->indices[this->numIndices++] = 2 * triangles[triBase] + 1;
        this->indices[this->numIndices++] = 2 * triangles[triBase + 2] + 1;
        this->indices[this->numIndices++] = 2 * triangles[triBase + 1] + 1;
    }
    this->numIndicesLow = this->numIndices;
    this->numVerts = 2 * outline->numVerts;
    this->numVertsLow = this->numVerts;

    for (unsigned int vertIndex = 0; vertIndex < outline->numVerts; ++vertIndex)
    {
        const unsigned int nextVert = (vertIndex + 1) % outline->numVerts;
        const float x0 = outline->verts[vertIndex].edge.origin[0];
        const float y0 = outline->verts[vertIndex].edge.origin[1];
        const float x1 = outline->verts[nextVert].edge.origin[0];
        const float y1 = outline->verts[nextVert].edge.origin[1];

        float edgeA[3] = { x0 - x1, y0 - y1, 0.0f };
        float edgeB[3] = { 0.0f, 0.0f, -thickness };
        float normal[3];
        Vec3Cross(edgeB, edgeA, normal);
        Vec3Normalize(normal);
        packSnormNormal(normal, &this->normArray[this->numNorm]);

        this->indices[this->numIndices++] = this->numVerts;
        this->indices[this->numIndices++] = this->numVerts + 1;
        this->indices[this->numIndices++] = this->numVerts + 2;
        this->indices[this->numIndices++] = this->numVerts + 1;
        this->indices[this->numIndices++] = this->numVerts + 3;
        this->indices[this->numIndices++] = this->numVerts + 2;
        ++this->numNorm;
        this->numVerts += 4;
    }

    iassert(this->numNorm == outline->numVerts + 2);
    iassert(this->numVerts == 6 * outline->numVerts);
    iassert(this->numIndices == 12 * (outline->numVerts - 1));
}

// Decomp: CoDMPServer.c:862256
void __cdecl GlassShard::Mesh::InitVertexList(unsigned __int8 numOutlineVerts, GlassShardMeshVertex *verts)
{
    unsigned __int8 vertWrite = 0;
    for (unsigned int outlineVert = 0; outlineVert < numOutlineVerts; ++outlineVert)
    {
        verts[vertWrite].pos = vertWrite;
        verts[vertWrite].norm = 0;
        ++vertWrite;
        verts[vertWrite].pos = vertWrite;
        verts[vertWrite].norm = 1;
        ++vertWrite;
    }
    for (unsigned int edgeVert = 0; edgeVert < numOutlineVerts; ++edgeVert)
    {
        const unsigned int nextEdgeVert = (edgeVert + 1) % numOutlineVerts;
        verts[vertWrite].pos = 2 * edgeVert;
        verts[vertWrite].norm = edgeVert + 2;
        verts[vertWrite + 1].pos = 2 * edgeVert + 1;
        verts[vertWrite + 1].norm = edgeVert + 2;
        verts[vertWrite + 2].pos = 2 * nextEdgeVert;
        verts[vertWrite + 2].norm = edgeVert + 2;
        verts[vertWrite + 3].pos = 2 * nextEdgeVert + 1;
        verts[vertWrite + 3].norm = edgeVert + 2;
        vertWrite += 4;
    }
}

// Decomp: CoDMPServer.c:862290
unsigned int __cdecl GlassShard::Mesh::GetMemorySize(unsigned int numOutineVerts)
{
    return 12 * (numOutineVerts - 1) + 4 * numOutineVerts + 8;
}

// Decomp: CoDMPServer.c:862296
GlassShard::Triangles::Triangles(const GlassShard::Outline *ol)
{
    this->outline = ol;
    this->nIndices = 0;
}

// Decomp: CoDMPServer.c:862303
double __thiscall GlassShard::Triangles::CalcCross(int idx1, int idx2, int idx3)
{
    return (float)(this->outline->verts[idx2].edge.origin[0] - this->outline->verts[idx1].edge.origin[0])
             * (float)(this->outline->verts[idx3].edge.origin[1] - this->outline->verts[idx2].edge.origin[1])
             - (float)(this->outline->verts[idx2].edge.origin[1] - this->outline->verts[idx1].edge.origin[1])
             * (float)(this->outline->verts[idx3].edge.origin[0] - this->outline->verts[idx2].edge.origin[0]);
}

// Decomp: CoDMPServer.c:862312
char __thiscall GlassShard::Triangles::AddTri(
                unsigned __int8 idx0,
                unsigned __int8 idx1,
                unsigned __int8 idx2)
{
    if (this->nIndices + 3 >= 0x100)
        return 0;
    this->triangleIndices[this->nIndices++] = idx0;
    this->triangleIndices[this->nIndices++] = idx1;
    this->triangleIndices[this->nIndices++] = idx2;
    return 1;
}

// Decomp: CoDMPServer.c:862323
char __thiscall GlassShard::Triangles::Triangulate()
{
    unsigned __int64 *triangulateTimer = &clGlasses->renderer->triangulateTimer;
    const _LARGE_INTEGER timerStart = tlPcGetTick();

    int hullCount = this->outline->numVerts;
    int hullVtxIndex[64];
    for (int hullInit = 0; hullInit < hullCount; ++hullInit)
        hullVtxIndex[hullInit] = hullInit;

    while (hullCount > 3)
    {
        const int hullCountBefore = hullCount;
        for (int earIndex = 0; earIndex < hullCount; ++earIndex)
        {
            const int prevIdx = hullVtxIndex[(earIndex + hullCount - 1) % hullCount];
            const int currIdx = hullVtxIndex[earIndex];
            const int nextIdx = hullVtxIndex[(earIndex + 1) % hullCount];
            if (CalcCross(prevIdx, currIdx, nextIdx) >= 0.0)
                continue;

            bool pointInsideEar = false;
            for (int testIndex = 0; testIndex < hullCount - 3 && !pointInsideEar; ++testIndex)
            {
                const int testVert = hullVtxIndex[(earIndex + testIndex + 2) % hullCount];
                pointInsideEar = IsInside(
                    this->outline->verts[prevIdx].edge.origin,
                    this->outline->verts[currIdx].edge.origin,
                    this->outline->verts[nextIdx].edge.origin,
                    this->outline->verts[testVert].edge.origin);
            }
            if (pointInsideEar)
                continue;

            if (!AddTri(
                    (unsigned __int8)hullVtxIndex[(earIndex + hullCount - 1) % hullCount],
                    (unsigned __int8)hullVtxIndex[earIndex],
                    (unsigned __int8)hullVtxIndex[(earIndex + 1) % hullCount]))
            {
                *triangulateTimer += tlPcGetTick().QuadPart - timerStart.QuadPart;
                return 0;
            }
            for (int shiftIndex = earIndex; shiftIndex < hullCount - 1; ++shiftIndex)
                hullVtxIndex[shiftIndex] = hullVtxIndex[shiftIndex + 1];
            --hullCount;
            break;
        }
        if (hullCountBefore == hullCount)
        {
            *triangulateTimer += tlPcGetTick().QuadPart - timerStart.QuadPart;
            return 0;
        }
    }

    iassert(hullCount == 3);
    if (!AddTri((unsigned __int8)hullVtxIndex[0], (unsigned __int8)hullVtxIndex[1], (unsigned __int8)hullVtxIndex[2]))
    {
        *triangulateTimer += tlPcGetTick().QuadPart - timerStart.QuadPart;
        return 0;
    }
    *triangulateTimer += tlPcGetTick().QuadPart - timerStart.QuadPart;
    return 1;
}

// Decomp: CoDMPServer.c:862437
bool __cdecl IsInside(const float *triA, const float *triB, const float *triC, const float *testPt)
{
    return (triB[0] - triA[0]) * (testPt[1] - triA[1]) - (triB[1] - triA[1]) * (testPt[0] - triA[0]) <= 0.0f
        && (triC[0] - triB[0]) * (testPt[1] - triB[1]) - (triC[1] - triB[1]) * (testPt[0] - triB[0]) <= 0.0f
        && (triA[0] - triC[0]) * (testPt[1] - triC[1]) - (triA[1] - triC[1]) * (testPt[0] - triC[0]) <= 0.0f;
}

// Decomp: CoDMPServer.c:862455
void __thiscall GlassShard::Init()
{
    this->physObjId = 0;
    this->glassPhysics = 0;
    this->group = 0;
    this->remove = 0;
    this->inGroupChange = 0;
    this->memoryPtr = 0;
    this->delayedDrop = 0;
    this->worldBBoxMin[0] = -FLT_MAX;
    this->worldBBoxMin[1] = -FLT_MAX;
    this->worldBBoxMin[2] = -FLT_MAX;
    this->worldBBoxMax[0] = FLT_MAX;
    this->worldBBoxMax[1] = FLT_MAX;
    this->worldBBoxMax[2] = FLT_MAX;
}

// Decomp: CoDMPServer.c:862480
void __thiscall GlassShard::Destroy()
{
    GlassShard::DestroyPhysicsObj();
    GlassShard::FreeMemory();
    memset(this, 41, sizeof(GlassShard));
}

// Decomp: CoDMPServer.c:862491
double __thiscall GlassShard::EdgeRatio()
{
    float originalEdgeLen = 0.0f;
    for (unsigned int vertIndex = 0; vertIndex < this->outline.numVerts; ++vertIndex)
    {
        if (this->outline.verts[vertIndex].isOriginalEdge)
            originalEdgeLen += this->outline.verts[vertIndex].edge.len;
    }
    return originalEdgeLen / this->outline.Length();
}

// Decomp: CoDMPServer.c:862510
void __thiscall GlassShard::UnEdge()
{
    if ( this->isOriginalEdge )
    {
        this->isOriginalEdge = 0;
        //GlassShard::Outline::MarkAllEdge(&this->outline, 0);
        this->outline.MarkAllEdge(false);
    }
}

// Decomp: CoDMPServer.c:862520
void __thiscall GlassShard::Outline::MarkAllEdge(bool e)
{
    iassert(this->isClosed);
    for (unsigned int vertIndex = 0; vertIndex < this->numVerts; ++vertIndex)
        this->verts[vertIndex].isOriginalEdge = e;
}

// Decomp: CoDMPServer.c:862542
char __thiscall GlassShard::Create(const Glass *glass)
{
    ShardGroup *grp;
    TempOutline newOutline;

    if (glass->numOutlineVerts < 3u)
        return 0;
    newOutline.verts = newOutline.v;
    newOutline.numVerts = 0;
    newOutline.maxVerts = 64;
    newOutline.isClosed = 0;
    for (unsigned int outlineVert = 0; outlineVert < glass->numOutlineVerts; ++outlineVert)
        newOutline.Add(glass->outline[outlineVert]);
    newOutline.CloseOutline();
    newOutline.MarkAllEdge(glass->isPlanar);

    GlassShard::Triangles triangles(&newOutline);
    //GlassShard::Triangles::Triangles(&triangles, &newOutline);

    //if ( !GlassShard::Triangles::Triangulate(&triangles) )
    if ( !triangles.Triangulate() )
        return 0;
    //if ( !GlassShard::AllocateMemory(newOutline.numVerts, &triangles) )
    if ( !this->AllocateMemory(newOutline.numVerts, &triangles) )
        return 0;
    //GlassShard::Outline::operator=(&this->outline, &newOutline);
    this->outline = newOutline;
    this->thickness = glass->thickness;
    this->uvScale = glass->uvScale;
    this->glassIndex = glass->index;
    this->axis[0][0] = glass->outlineAxis[0][0];
    this->axis[0][1] = glass->outlineAxis[0][1];
    this->axis[0][2] = glass->outlineAxis[0][2];
    this->axis[1][0] = glass->outlineAxis[1][0];
    this->axis[1][1] = glass->outlineAxis[1][1];
    this->axis[1][2] = glass->outlineAxis[1][2];
    this->axis[2][0] = glass->outlineAxis[2][0];
    this->axis[2][1] = glass->outlineAxis[2][1];
    this->axis[2][2] = glass->outlineAxis[2][2];
    this->origin[0] = glass->outlineOrigin[0];
    this->origin[1] = glass->outlineOrigin[1];
    this->origin[2] = glass->outlineOrigin[2];
    this->isOriginalEdge = glass->isPlanar;
    //grp = (ShardGroup *)GlassRenderer::GetShardGroup(clGlasses->renderer, this->origin, glass->glassDef);
    grp = (ShardGroup *)clGlasses->renderer->GetShardGroup(this->origin, glass->glassDef);
    if ( !grp )
        return 0;
    GlassShard::UpdateBBox();
    //ShardGroup::Add(grp, this);
    grp->Add(this);
    return 1;
}

// Decomp: CoDMPServer.c:862604
bool __thiscall GlassShard::CanSplit(float maxShardSize, float minShardSize) const
{
    const float areaThreshold = (minShardSize * 2.5f < maxShardSize)
        ? minShardSize * 2.5f
        : maxShardSize;
    return this->outline.Area() > areaThreshold;
}

// Decomp: CoDMPServer.c:862617
bool GlassShard::CanSplit(bool shatter) const
{
    iassert(this->group);

    const float minScale = shatter
        ? 1.0f
        : clGlasses->renderer->shardShatterSizeLimitScale->current.value;
    return this->CanSplit(
        this->group->glassDef->maxShardSize,
        this->group->glassDef->minShardSize * minScale);
}

// Decomp: CoDMPServer.c:862646
int __thiscall GlassShard::Shatter(const GlassShard **newShards, int maxNewShards)
{
    PROF_SCOPED("GlassShard.Shatter");

    unsigned __int64 *shatterTimer = &clGlasses->renderer->shatterTimer;
    const _LARGE_INTEGER timerStart = tlPcGetTick();
    ++clGlasses->renderer->numShatters;
    iassert(this->group);

    const float glassSize = this->outline.Area();
    float maxShardSize = (glassSize / 10.0f < this->group->glassDef->maxShardSize)
        ? glassSize / 10.0f
        : this->group->glassDef->maxShardSize;
    float minShardSize = (maxShardSize / 5.0f < this->group->glassDef->minShardSize)
        ? maxShardSize / 5.0f
        : this->group->glassDef->minShardSize;

    *newShards = this;
    int numNewShards = 1;
    int maxSplits = clGlasses->renderer->maxShardSplit->current.integer;
    if (this->group->glassDef->maxShards > 0)
        maxSplits = this->group->glassDef->maxShards;
    const int splitBudget = (maxNewShards - 1 < maxSplits) ? maxNewShards - 1 : maxSplits;
    int maxSplitTries = 3 * splitBudget;
    int numTries = 0;
    GlassShard::splitFailCount[0] = 0;
    GlassShard::splitFailCount[1] = 0;
    GlassShard::splitFailCount[2] = 0;
    GlassShard::splitFailCount[3] = 0;
    GlassShard::splitFailCount[4] = 0;
    GlassShard::splitFailCount[5] = 0;
    GlassShard::splitFailCount[6] = 0;
    GlassShard::splitFailCount[7] = 0;

    {
        PROF_SCOPED("Split");
        while (numNewShards < splitBudget && (*newShards)->CanSplit(maxShardSize, minShardSize) && maxSplitTries > 0)
        {
            int numSplit = 0;
            for (int shardIndex = 0; shardIndex < numNewShards && !numSplit && newShards[shardIndex]->CanSplit(maxShardSize, minShardSize); ++shardIndex)
            {
                ++numTries;
                numSplit = newShards[shardIndex]->Split(&newShards[numNewShards], minShardSize, 0, -1.0);
                --maxSplitTries;
            }
            if (numSplit)
            {
                numNewShards += numSplit;
                qsort(newShards, numNewShards, 4u, compareShards);
                if (maxShardSize > newShards[0]->outline.Area())
                    break;
            }
        }
    }

    if (clGlasses->renderer->debugSplit->current.enabled)
    {
        int numNewVerts = 0;
        for (int shardIndex = 0; shardIndex < numNewShards; ++shardIndex)
            numNewVerts += newShards[shardIndex]->outline.numVerts;
        const float smallestArea = newShards[numNewShards - 1]->outline.Area();
        const float largestArea = newShards[0]->outline.Area();
        Com_Printf(
            0,
            "Shatter: original size=%.1f num tries=%d new shards=%d largest=%.1f smallest=%.1f numVerts=%d(%.1f)\n",
            glassSize,
            numTries,
            numNewShards,
            largestArea,
            smallestArea,
            numNewVerts,
            (float)numNewVerts / (float)numNewShards);
        Com_Printf(
            0,
            "         fail: sPos=%d sDir=%d init=%d verts=%d area=%d angle=%d narrow=%d mem=%d\n",
            GlassShard::splitFailCount[0],
            GlassShard::splitFailCount[1],
            GlassShard::splitFailCount[2],
            GlassShard::splitFailCount[3],
            GlassShard::splitFailCount[4],
            GlassShard::splitFailCount[6],
            GlassShard::splitFailCount[7],
            GlassShard::splitFailCount[5]);
    }
    *shatterTimer += tlPcGetTick().QuadPart - timerStart.QuadPart;

    return numNewShards;
}

// Decomp: CoDMPServer.c:862773
int compareShards(const void *s1, const void *s2)
{
    float area1 = (*(const GlassShard **)s1)->outline.area;
    float area2 = (*(const GlassShard **)s2)->outline.area;

    // Sort largest first (descending)
    if (area1 > area2) return -1;
    if (area1 < area2) return  1;
    return 0;
}

// Decomp: CoDMPServer.c:862792
void __thiscall GlassShard::InitPhysics(
                const GlassShard **newShards,
                int numNewShards,
                float glassExtent,
                const float *pos,
                const float *dir)
{
    if (!this->group)
    {
        for (int shardIndex = 0; shardIndex < numNewShards; ++shardIndex)
            ((GlassShard *)newShards[shardIndex])->Remove((GlassShard::RemoveReason)8, 0);
        return;
    }

    const float dirLenSq = dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2];
    const float lifeScale = (dirLenSq == 0.0f) ? 0.5f : 1.0f;
    const int numKeepShards = (int)(numNewShards * this->group->glassDef->shardLifeProbablility * lifeScale + 0.5f);

    PROF_SCOPED("InitPhysics");

    for (int shardIndex = 0; shardIndex < numNewShards; ++shardIndex)
    {
        GlassShard *shard = (GlassShard *)newShards[shardIndex];
        shard->InitMesh();
        if (clGlasses->renderer->freezeShards->current.enabled)
            shard->UpdateBBox();
        else if (!shard->InitPhysics(shardIndex < numKeepShards, pos, dir, glassExtent, 1.0))
            continue;

        if (!shard->group)
        {
            if (!this->group)
            {
                shard->Remove((GlassShard::RemoveReason)8, 0);
                continue;
            }
            this->group->Add(shard);
        }
        ShardGroup *group = shard->group;
        if (group->packedPos != clGlasses->renderer->CalcPackedPos(shard->origin))
            clGlasses->renderer->AddGroupChange(shard);
    }
}

// Decomp: CoDMPServer.c:862846
void __thiscall GlassShard::InitMesh()
{
    GlassShard::Recenter();
    //GlassShard::Mesh::Init(&this->mesh, &this->outline, this->thickness, this->axis[2]);
    this->mesh.Init(&this->outline, this->thickness, this->axis[2]);
}

// Decomp: CoDMPServer.c:862856
char __thiscall GlassShard::AllocateMemory(
                unsigned int nHull,
                const GlassShard::Triangles *triangles)
{
    __int16 MemorySize;
    unsigned __int8 *ptr;

    GlassShard::FreeMemory();
    if ( triangles )
        MemorySize = GlassShard::Mesh::GetMemorySize(nHull);
    else
        MemorySize = 0;
    this->memorySize = MemorySize + 24 * nHull;
    //this->memoryPtr = (unsigned __int8 *)GlassRenderer::AllocateShardMemory(clGlasses->renderer, this->memorySize, this);
    this->memoryPtr = (unsigned __int8 *)clGlasses->renderer->AllocateShardMemory(this->memorySize, this);
    if ( !this->memoryPtr )
        return 0;
    ptr = this->memoryPtr;
    if ( nHull )
    {
        if ( nHull >= 0x100
            && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 1667, 0, "%s", "nHull < 256") )
        {
            __debugbreak();
        }
        //ptr += GlassShard::Outline::Init(&this->outline, (GlassShard::Outline::Vertex *)ptr, nHull);
        ptr += this->outline.Init((GlassShard::Outline::Vertex *)ptr, nHull);
    }
    if ( triangles )
    {
        if ( triangles->nIndices != 3 * (nHull - 2)
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp",
                        1672,
                        0,
                        "%s",
                        "triangles->nIndices == (nHull-2)*3") )
        {
            __debugbreak();
        }
        //GlassShard::Mesh::SetTriangles(&this->mesh, ptr, triangles->triangleIndices, triangles->nIndices);
        this->mesh.SetTriangles(ptr, (unsigned char*)triangles->triangleIndices, triangles->nIndices);
    }
    return 1;
}

// Decomp: CoDMPServer.c:862907
void __thiscall GlassShard::FreeMemory()
{
    if ( this->memoryPtr )
    {
        //GlassShard::lastFreeMemorySize = GlassRenderer::ShardMemorySize(clGlasses->renderer, (unsigned int *)this->memoryPtr);
        GlassShard::lastFreeMemorySize = clGlasses->renderer->ShardMemorySize((unsigned int *)this->memoryPtr);
        //GlassRenderer::FreeShardMemory(clGlasses->renderer, (unsigned int *)this->memoryPtr);
        clGlasses->renderer->FreeShardMemory((unsigned int *)this->memoryPtr);
        this->memoryPtr = 0;
        this->outline.verts = 0;
        this->outline.numVerts = 0;
        this->outline.maxVerts = 0;
        this->outline.isClosed = 0;
        //GlassShard::Mesh::Clear(&this->mesh);
        this->mesh.Clear();
    }
}

// Decomp: CoDMPServer.c:862930
void __thiscall GlassShard::Defrag()
{
    unsigned __int8 backupBuffer[1024];

    iassert(this->memorySize < ARRAY_COUNT(backupBuffer));
    iassert(this->mesh.normArray);
    iassert(this->mesh.indices);

    memcpy(backupBuffer, this->memoryPtr, this->memorySize);
    clGlasses->renderer->FreeShardMemory((unsigned int *)this->memoryPtr);

    const int outlineBytes = 24 * this->outline.numVerts;
    iassert(this->memorySize == GlassShard::Mesh::GetMemorySize(this->outline.numVerts) + outlineBytes);

    this->memoryPtr = (unsigned __int8 *)clGlasses->renderer->AllocateShardMemory(this->memorySize, this);
    if (!this->memoryPtr)
    {
        clGlasses->renderer->FreeShard(this);
        return;
    }

    memcpy(this->memoryPtr, backupBuffer, this->memorySize);
    unsigned __int8 *ptr = this->memoryPtr;
    ptr += this->outline.SetPointers(ptr);
    ptr += this->mesh.SetPointers((PackedUnitVec *)ptr);
    iassert(ptr - this->memoryPtr == (int)this->memorySize);
}

// Decomp: CoDMPServer.c:863025
void __thiscall GlassShard::ToWorldPos(float *pLocal, float *pWorld, bool is3D)
{
    pWorld[0] = pLocal[0] * this->axis[0][0] + this->origin[0];
    pWorld[1] = pLocal[0] * this->axis[0][1] + this->origin[1];
    pWorld[2] = pLocal[0] * this->axis[0][2] + this->origin[2];
    pWorld[0] += pLocal[1] * this->axis[1][0];
    pWorld[1] += pLocal[1] * this->axis[1][1];
    pWorld[2] += pLocal[1] * this->axis[1][2];
    if (is3D)
    {
        pWorld[0] += pLocal[2] * this->axis[2][0];
        pWorld[1] += pLocal[2] * this->axis[2][1];
        pWorld[2] += pLocal[2] * this->axis[2][2];
    }
}

// Decomp: CoDMPServer.c:863049
void __thiscall GlassShard::ToWorldDir(float *dLocal, float *dWorld, bool is3D)
{
    dWorld[0] = dLocal[0] * this->axis[0][0];
    dWorld[1] = dLocal[0] * this->axis[0][1];
    dWorld[2] = dLocal[0] * this->axis[0][2];
    dWorld[0] += dLocal[1] * this->axis[1][0];
    dWorld[1] += dLocal[1] * this->axis[1][1];
    dWorld[2] += dLocal[1] * this->axis[1][2];
    if (is3D)
    {
        dWorld[0] += dLocal[2] * this->axis[2][0];
        dWorld[1] += dLocal[2] * this->axis[2][1];
        dWorld[2] += dLocal[2] * this->axis[2][2];
    }
}

// Decomp: CoDMPServer.c:863073
void __thiscall GlassShard::ToLocal(float *pos, float *dir, float *localPos, float *localDir)
{
    float invXform[4][3];
    MatrixInverseOrthogonal43(this->axis, invXform);

    if (localDir)
    {
        localDir[0] = dir[0] * invXform[0][0];
        localDir[1] = dir[0] * invXform[0][1];
        localDir[2] = dir[0] * invXform[0][2];
        localDir[0] += dir[1] * invXform[1][0];
        localDir[1] += dir[1] * invXform[1][1];
        localDir[2] += dir[1] * invXform[1][2];
        localDir[0] += dir[2] * invXform[2][0];
        localDir[1] += dir[2] * invXform[2][1];
        localDir[2] += dir[2] * invXform[2][2];
    }
    if (localPos)
    {
        localPos[0] = pos[0] * invXform[0][0] + invXform[3][0];
        localPos[1] = pos[0] * invXform[0][1] + invXform[3][1];
        localPos[2] = pos[0] * invXform[0][2] + invXform[3][2];
        localPos[0] += pos[1] * invXform[1][0];
        localPos[1] += pos[1] * invXform[1][1];
        localPos[2] += pos[1] * invXform[1][2];
        localPos[0] += pos[2] * invXform[2][0];
        localPos[1] += pos[2] * invXform[2][1];
        localPos[2] += pos[2] * invXform[2][2];
    }
}

// Decomp: CoDMPServer.c:863128
bool __thiscall GlassShard::Intersect(float *pos, float *dir, float *hitPoint)
{
    float testDir[2];
    float d;
    int edge;
    float localHit[3];
    float t;
    float localPos[3];
    float localDir[3];

    GlassShard::ToLocal(pos, dir, localPos, localDir);
    if ( fabs(localDir[2]) <= 0.0000099999997 )
        return 0;
    //t = COERCE_FLOAT(LODWORD(localPos[2]) ^ _mask__NegFloat_) / localDir[2];
    t = (-(localPos[2])) / localDir[2];
    if ( t < 0.0 )
        return 0;
    localHit[0] = (float)(t * localDir[0]) + localPos[0];
    localHit[1] = (float)(t * localDir[1]) + localPos[1];
    localHit[2] = (float)(t * localDir[2]) + localPos[2];
    *hitPoint = localHit[0];
    hitPoint[1] = localHit[1];
    testDir[0] = 1.0f;
    testDir[1] = 0.0f;
    //return GlassShard::Outline::GetNumIntersections(&this->outline, hitPoint, testDir, &d, &edge) % 2 == 1;
    return this->outline.GetNumIntersections(hitPoint, testDir, &d, &edge) % 2 == 1;
}

// Decomp: CoDMPServer.c:863166
void __thiscall GlassShard::ExplosionEvent(
                const float *expOrigin,
                float damageInner,
                float damageOuter,
                float radius,
                int mod)
{
    (void)mod;
    const float distSq = Vec3DistanceSq(expOrigin, this->origin);
    if (radius * radius <= distSq)
        return;

    const float dist = sqrtf(distSq);
    const float damage = (damageInner - damageOuter) * (1.0f - dist / radius) + damageOuter;
    float dir[3];
    dir[0] = this->origin[0] - expOrigin[0];
    dir[1] = this->origin[1] - expOrigin[1];
    dir[2] = this->origin[2] - expOrigin[2];
    Vec3Normalize(dir);

    if (this->InitPhysicsObj(true))
    {
        const float forceScale = damage * 0.01f * clGlasses->renderer->forceMultiplier->current.value;
        float force[3] = { forceScale * dir[0], forceScale * dir[1], forceScale * dir[2] };
        GlassShard::AddForce(this->origin, force);
    }
    if (this->isOriginalEdge)
        GlassShard::UnEdge();
}

// Decomp: CoDMPServer.c:863205
char __thiscall GlassShard::TracePoint(
                float *p0,
                const float *p1,
                float *dir,
                float length,
                const float *mins,
                float *maxs)
{
    (void)length;

    const float *bboxMax = this->worldBBoxMax;
    const float *bboxMin = this->worldBBoxMin;
    if (bboxMax[0] < mins[0] || bboxMax[1] < mins[1] || bboxMax[2] < mins[2]
        || maxs[0] < bboxMin[0] || maxs[1] < bboxMin[1] || maxs[2] < bboxMin[2])
        return 0;

    float segMin[3];
    float segMax[3];
    Vec3Min(p0, p1, segMin);
    Vec3Max(p0, p1, segMax);

    bool obbHit = false;
    if (bboxMax[0] >= segMin[0] && bboxMax[1] >= segMin[1] && bboxMax[2] >= segMin[2]
        && segMax[0] >= bboxMin[0] && segMax[1] >= bboxMin[1] && segMax[2] >= bboxMin[2])
    {
        float diff[3];
        float center[3];
        float halfExtents[3];
        float relStart[3];
        Vec3Sub(p1, p0, diff);
        Vec3Avg(bboxMax, bboxMin, center);
        Vec3Sub(bboxMax, bboxMin, halfExtents);
        Vec3Scale(halfExtents, 0.5f, halfExtents);
        Vec3Sub(p0, center, relStart);

        float absDiff[3];
        Vec3Abs(diff, absDiff);

        float permHalf[3] = { halfExtents[1], halfExtents[2], halfExtents[0] };
        float product[3];
        Vec3Mul(halfExtents, permHalf, product);

        float permHalf2[3] = { halfExtents[2], halfExtents[0], halfExtents[1] };
        float termB[3];
        Vec3Mul(permHalf2, absDiff, termB);

        float permDiff[3] = { diff[1], diff[2], diff[0] };
        float termA[3];
        Vec3Mul(relStart, permDiff, termA);

        float permRel[3] = { relStart[1], relStart[2], relStart[0] };
        float termC[3];
        Vec3Mul(permRel, diff, termC);

        float crossTerm[3];
        Vec3Sub(termA, termC, crossTerm);

        float absCross[3];
        Vec3Abs(crossTerm, absCross);

        float slabTest[3];
        Vec3Add(product, termB, slabTest);
        Vec3Sub(slabTest, absCross, slabTest);

        obbHit = slabTest[0] > 0.0f && slabTest[1] > 0.0f && slabTest[2] > 0.0f;
    }

    float hitPoint2d[2];
    if (!obbHit || !GlassShard::Intersect(p0, dir, hitPoint2d))
        return 0;

    float hitPoint3d[3];
    GlassShard::ToWorldPos(hitPoint2d, hitPoint3d, 0);
    const float glassExtent = this->outline.Extent();

    GlassShard *newShards[10];
    newShards[0] = this;
    memset(&newShards[1], 0, 36);
    int numNewShards = 1;
    const float minShardSize = this->group->glassDef->minShardSize
        * clGlasses->renderer->shardShatterSizeLimitScale->current.value;
    const float maxShardSize = this->group->glassDef->maxShardSize;

    memset(GlassShard::splitFailCount, 0, sizeof(GlassShard::splitFailCount));

    GlassShard::Outline::EdgeDistance dists[1];
    this->outline.GetNearestDistances(hitPoint2d, dists, 1);

    const float edgeDistRatio = 0.02f;
    const float outlineLen = this->outline.Length();
    if (dists[0].dist <= outlineLen * edgeDistRatio)
    {
        PROF_SCOPED("Split 1");
        hitPoint2d[0] = dists[0].closestPoint[0];
        hitPoint2d[1] = dists[0].closestPoint[1];
        GlassShard::ToWorldPos(hitPoint2d, hitPoint3d, 0);

        int numSplit = 0;
        for (int tryIndex = 0; tryIndex < 5 && !numSplit && newShards[0]->CanSplit(maxShardSize, minShardSize); ++tryIndex)
        {
            numSplit = newShards[0]->Split(
                (const GlassShard **)&newShards[numNewShards],
                minShardSize,
                dists[0].edgeIndex,
                dists[0].edgeParam);
        }
        numNewShards += numSplit;
    }
    else
    {
        PROF_SCOPED("Chip");
        if (GlassShard::Chip(dists, hitPoint2d, (const GlassShard **)&newShards[numNewShards], minShardSize))
            ++numNewShards;
    }

    for (int shardIndex = 0; shardIndex < numNewShards; ++shardIndex)
    {
        if (newShards[shardIndex]->EdgeRatio() < 0.15000001f)
            newShards[shardIndex]->UnEdge();
    }

    const float stickiness = 1.0f;
    if (numNewShards == 1)
    {
        if (newShards[0]->isOriginalEdge && !newShards[0]->LeaveOnEdge(stickiness))
            newShards[0]->UnEdge();
    }
    else
    {
        iassert(numNewShards == 2);
        GlassShard *fixedShard = 0;

        if (newShards[0]->isOriginalEdge && newShards[1]->isOriginalEdge)
        {
            const float area0 = newShards[0]->outline.Area();
            if (newShards[1]->outline.Area() <= area0)
            {
                newShards[1]->UnEdge();
                fixedShard = newShards[0];
            }
            else
            {
                newShards[0]->UnEdge();
                fixedShard = newShards[1];
            }
        }
        else if (newShards[0]->isOriginalEdge)
        {
            if (newShards[0]->LeaveOnEdge(stickiness))
                fixedShard = newShards[0];
            else
                newShards[0]->UnEdge();
        }
        else if (newShards[1]->isOriginalEdge)
        {
            if (newShards[1]->LeaveOnEdge(stickiness))
                fixedShard = newShards[1];
            else
                newShards[1]->UnEdge();
        }

        const float shardChipRatio = 3.0f;
        const float area0 = newShards[0]->outline.Area();
        if (newShards[1]->outline.Area() <= area0 * shardChipRatio)
        {
            if (newShards[0]->outline.Area() > newShards[1]->outline.Area() * shardChipRatio)
                fixedShard = newShards[0];
        }
        else
        {
            fixedShard = newShards[1];
        }

        PROF_SCOPED("Split 2");
        const int maxSplits = 10;
        int maxSplitTries = 30;
        float pos2d[3];
        while (numNewShards < maxSplits && newShards[0]->CanSplit(maxShardSize, minShardSize) && maxSplitTries > 0)
        {
            int numSplit = 0;
            for (int shardIndex = 0; shardIndex < numNewShards && !numSplit && newShards[shardIndex]->CanSplit(maxShardSize, minShardSize); ++shardIndex)
            {
                if (newShards[shardIndex] != fixedShard)
                {
                    newShards[shardIndex]->ToLocal(hitPoint3d, 0, pos2d, 0);
                    newShards[shardIndex]->outline.GetNearestDistances(pos2d, dists, 1);
                    numSplit = newShards[shardIndex]->Split(
                        (const GlassShard **)&newShards[numNewShards],
                        minShardSize,
                        dists[0].edgeIndex,
                        dists[0].edgeParam);
                }
                --maxSplitTries;
            }
            if (numSplit)
            {
                numNewShards += numSplit;
                qsort(newShards, numNewShards, 4u, compareShards);
            }
        }
    }

    if (clGlasses->renderer->debugSplit->current.enabled)
    {
        Com_Printf(
            0,
            "TracePoint: new shards=%d largest=%.1f smallest=%.1f\n",
            numNewShards,
            newShards[0]->outline.Area(),
            newShards[numNewShards - 1]->outline.Area());
        Com_Printf(
            0,
            "         fail: sPos=%d sDir=%d init=%d verts=%d area=%d angle=%d narrow=%d mem=%d\n",
            GlassShard::splitFailCount[0],
            GlassShard::splitFailCount[1],
            GlassShard::splitFailCount[2],
            GlassShard::splitFailCount[3],
            GlassShard::splitFailCount[4],
            GlassShard::splitFailCount[6],
            GlassShard::splitFailCount[7],
            GlassShard::splitFailCount[5]);
    }

    PROF_SCOPED("InitPhysics");
    for (int shardIndex = 0; shardIndex < numNewShards; ++shardIndex)
    {
        newShards[shardIndex]->InitMesh();
        if (newShards[shardIndex]->isOriginalEdge || clGlasses->renderer->freezeShards->current.enabled)
            newShards[shardIndex]->UpdateBBox();
        else if (!newShards[shardIndex]->InitPhysics(1, hitPoint3d, dir, glassExtent, 1.0))
            continue;

        if (!newShards[shardIndex]->group)
            this->group->Add(newShards[shardIndex]);
        ShardGroup *group = newShards[shardIndex]->group;
        if (group->packedPos != clGlasses->renderer->CalcPackedPos(newShards[shardIndex]->origin))
            clGlasses->renderer->AddGroupChange(newShards[shardIndex]);
    }
    return 1;
}

// Decomp: CoDMPServer.c:863610
void __thiscall GlassShard::GetLocalBBox(float *mins, float *maxs)
{
    mins[0] = maxs[0] = this->outline.verts[0].edge.origin[0];
    mins[1] = maxs[1] = this->outline.verts[0].edge.origin[1];
    mins[2] = maxs[2] = 0.0f;

    for (unsigned int vertIndex = 1; vertIndex < this->outline.numVerts; ++vertIndex)
    {
        float pos[3];
        pos[0] = this->outline.verts[vertIndex].edge.origin[0];
        pos[1] = this->outline.verts[vertIndex].edge.origin[1];
        pos[2] = 0.0f;
        Vec3Min(pos, mins, mins);
        Vec3Max(pos, maxs, maxs);
    }
    if ( this->thickness <= 0.0 )
        mins[2] = this->thickness;
    else
        maxs[2] = this->thickness;

    iassert(mins);
    iassert(maxs);
    iassert(maxs[0] >= mins[0]);
    iassert(maxs[1] >= mins[1]);
    iassert(maxs[2] >= mins[2]);
    nanassertvec3(mins);
    nanassertvec3(maxs);
}

// Decomp: CoDMPServer.c:863727
void __thiscall GlassShard::UpdateBBox()
{
    this->worldBBoxMin[0] = FLT_MAX;
    this->worldBBoxMin[1] = FLT_MAX;
    this->worldBBoxMin[2] = FLT_MAX;
    this->worldBBoxMax[0] = -FLT_MAX;
    this->worldBBoxMax[1] = -FLT_MAX;
    this->worldBBoxMax[2] = -FLT_MAX;

    const float offset[3] = {
        this->thickness * this->axis[2][0],
        this->thickness * this->axis[2][1],
        this->thickness * this->axis[2][2]
    };

    for (unsigned int vertIndex = 0; vertIndex < this->outline.numVerts; ++vertIndex)
    {
        const GlassShard::Outline::Vertex *vert = &this->outline.verts[vertIndex];
        float temp[3];
        temp[0] = vert->edge.origin[0] * this->axis[0][0] + this->origin[0];
        temp[1] = vert->edge.origin[0] * this->axis[0][1] + this->origin[1];
        temp[2] = vert->edge.origin[0] * this->axis[0][2] + this->origin[2];
        temp[0] += vert->edge.origin[1] * this->axis[1][0];
        temp[1] += vert->edge.origin[1] * this->axis[1][1];
        temp[2] += vert->edge.origin[1] * this->axis[1][2];
        Vec3Min(temp, this->worldBBoxMin, this->worldBBoxMin);
        Vec3Max(temp, this->worldBBoxMax, this->worldBBoxMax);
        temp[0] += offset[0];
        temp[1] += offset[1];
        temp[2] += offset[2];
        Vec3Min(temp, this->worldBBoxMin, this->worldBBoxMin);
        Vec3Max(temp, this->worldBBoxMax, this->worldBBoxMax);
    }
}

// Decomp: CoDMPServer.c:863781
void __thiscall GlassShard::Update(float deltaTime)
{
    iassert(this->physObjId || this->glassPhysics || this->delayedDrop);
    if ( this->delayedDrop )
    {
        this->timeUntilAction = this->timeUntilAction - deltaTime;
        if ( this->timeUntilAction < 0.0 )
            GlassShard::InitPhysics(1, vec3_origin, vec3_origin, 0.0, 0.0);
    }
    else
    {
        bool isStable;
        bool hitBottom;

        isStable = 0;
        hitBottom = 0;
        if ( this->glassPhysics )
        {
            if ( deltaTime > 0.0 )
            {
                this->glassPhysics->Step(deltaTime);
                this->glassPhysics->GetPosition(this->origin, this->axis);
                this->timeUntilAction = this->timeUntilAction - deltaTime;
                hitBottom = this->timeUntilAction < 0.0;
                isStable = 0;
            }
        }
        else
        {
            Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
            Sys_EnterCriticalSection(CRITSECT_PHYSICS);
            Phys_ObjGetPosition(this->physObjId, this->origin, this->axis);
            //isStable = rigid_body::is_stable(*(rigid_body **)this->physObjId) != 0;
            isStable = Phys_GetUserData(this->physObjId)->body->is_stable() != 0;
            Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
            Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
        }
        const bool removeHitBottom = hitBottom || s_world.mins[2] + 5.0f > this->origin[2];
        if ((isStable || removeHitBottom) && (GlassShard::DestroyPhysicsObj(), removeHitBottom))
        {
            GlassShard::Remove(REMOVE_HIT_BOTTOM, 1);
        }
        else
        {
            GlassShard::UpdateBBox();
            ShardGroup *group = this->group;
            if (group->packedPos != clGlasses->renderer->CalcPackedPos(this->origin))
                clGlasses->renderer->AddGroupChange(this);
        }
    }
}

// Decomp: CoDMPServer.c:863850
void __thiscall GlassShard::ChangeGroup()
{
    ShardGroup *grp;
    const GlassDef *glassDef;
    bool freeShard;

    freeShard = 1;
    if ( this->group )
    {
        glassDef = this->group->glassDef;
        this->group->Remove(this);
        if ( !this->remove )
        {
            grp = (ShardGroup *)clGlasses->renderer->GetShardGroup(this->origin, glassDef);
            if ( grp )
            {
                grp->Add(this);
                freeShard = 0;
            }
        }
    }
    this->inGroupChange = 0;
    if ( freeShard )
        clGlasses->renderer->FreeShard(this);
}

// Decomp: CoDMPServer.c:863879
void __thiscall GlassShard::Remove(GlassShard::RemoveReason reason, bool delay)
{
    //if ( !FixedSizeAllocator<GlassPhysics *>::IsValidUsedPtr(
    //                (FixedSizeAllocator<GlassPhysics *> *)clGlasses->renderer->shardsAllocator,
    //                (GlassPhysics *)this)
    //    && !Assert_MyHandler(
    //                "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp",
    //                2285,
    //                0,
    //                "%s",
    //                "clGlasses->Renderer()->IsValidUsedShardPtr( this )") )
    //{
    //    __debugbreak();
    //}
    if ( delay )
    {
        if ( !this->remove )
        {
            this->remove = 1;
            clGlasses->renderer->AddGroupChange(this);
            ++GlassShard::removeReasonsCount[reason];
        }
    }
    else
    {
        ++GlassShard::removeReasonsCount[reason];
        if ( this->group )
            this->group->Remove(this);
        clGlasses->renderer->FreeShard(this);
    }
}

// Decomp: CoDMPServer.c:863916
void __thiscall GlassShard::GenerateVerts(
                bool highLod,
                GfxPackedVertex *baseVerts,
                unsigned __int16 vertsBaseIndex,
                unsigned __int16 *idxOut)
{
    iassert(baseVerts);

    float positions[769 * 3];
    unsigned int packedUVs[256];

    const float thicknessX = this->thickness * this->axis[2][0];
    const float thicknessY = this->thickness * this->axis[2][1];
    const float thicknessZ = this->thickness * this->axis[2][2];

    float *posOut = positions;
    unsigned int *uvOut = packedUVs;

    for (unsigned int vertIndex = 0; vertIndex < this->outline.numVerts; ++vertIndex)
    {
        const GlassShard::Outline::Vertex *vert = &this->outline.verts[vertIndex];
        const float u = vert->edge.origin[0];
        const float v = vert->edge.origin[1];

        float wx = u * this->axis[0][0] + v * this->axis[1][0] + this->origin[0];
        float wy = u * this->axis[0][1] + v * this->axis[1][1] + this->origin[1];
        float wz = u * this->axis[0][2] + v * this->axis[1][2] + this->origin[2];

        posOut[0] = wx;
        posOut[1] = wy;
        posOut[2] = wz;
        posOut[3] = wx + thicknessX;
        posOut[4] = wy + thicknessY;
        posOut[5] = wz + thicknessZ;
        posOut += 6;

        const float scaledU = u * this->uvScale;
        const float scaledV = v * this->uvScale;
        int packedU = (int)((2 * *(unsigned int *)&scaledU) ^ 0x80000000) >> 14;
        int packedV = (int)((2 * *(unsigned int *)&scaledV) ^ 0x80000000) >> 14;
        packedU = (packedU < 0x3FFF) ? packedU : 0x3FFF;
        packedV = (packedV < 0x3FFF) ? packedV : 0x3FFF;
        packedU = (packedU > -16384) ? packedU : -16384;
        packedV = (packedV > -16384) ? packedV : -16384;
        packedU = (packedU & 0x3FFF) | ((*(int *)&scaledU >> 16) & 0xC000);
        packedV = (packedV & 0x3FFF) | ((*(int *)&scaledV >> 16) & 0xC000);
        uvOut[0] = packedV | (packedU << 16);
        uvOut[1] = uvOut[0];
        uvOut += 2;
    }

    unsigned int packedNormals[256];
    int numNorm = highLod ? this->mesh.numNorm : 2;

    for (int normIndex = 0; normIndex < numNorm; ++normIndex)
    {
        float localNorm[3];
        Vec3UnpackUnitVec(this->mesh.normArray[normIndex], localNorm);

        float wx = localNorm[0] * this->axis[0][0]
                 + localNorm[1] * this->axis[1][0]
                 + localNorm[2] * this->axis[2][0];
        float wy = localNorm[0] * this->axis[0][1]
                 + localNorm[1] * this->axis[1][1]
                 + localNorm[2] * this->axis[2][1];
        float wz = localNorm[0] * this->axis[0][2]
                 + localNorm[1] * this->axis[1][2]
                 + localNorm[2] * this->axis[2][2];

        packedNormals[normIndex] = ((unsigned char)(int)(wx * 127.0f + 127.5f))
                               | ((unsigned char)(int)(wy * 127.0f + 127.5f)) << 8
                               | ((unsigned char)(int)(wz * 127.0f + 127.5f)) << 16
                               | (63u << 24);
    }

    PackedUnitVec tangent;
    tangent.array[0] = (int)(this->axis[0][0] * 127.0f + 127.5f);
    tangent.array[1] = (int)(this->axis[0][1] * 127.0f + 127.5f);
    tangent.array[2] = (int)(this->axis[0][2] * 127.0f + 127.5f);
    tangent.array[3] = 63;

    const unsigned char *meshVerts =
        (const unsigned char *)clGlasses->renderer->vertexList[this->outline.numVerts - 1];
    int numVerts = highLod ? this->mesh.numVerts : this->mesh.numVertsLow;

    for (int vertIndex = 0; vertIndex < numVerts; ++vertIndex)
    {
        unsigned char posIdx = meshVerts[2 * vertIndex];
        unsigned char normIdx = meshVerts[2 * vertIndex + 1];
        const float *pos = &positions[3 * posIdx];

        baseVerts->xyz[0] = pos[0];
        baseVerts->xyz[1] = pos[1];
        baseVerts->xyz[2] = pos[2];
        baseVerts->normal.packed = packedNormals[normIdx];
        baseVerts->tangent = tangent;
        baseVerts->color.packed = 0xFFFFFFFF;
        baseVerts->binormalSign = 1.0f;
        baseVerts->texCoord.packed = packedUVs[posIdx];
        ++baseVerts;
    }

    const unsigned char *indices = this->mesh.indices;
    int numIndices = highLod ? this->mesh.numIndices : this->mesh.numIndicesLow;
    for (int idxIndex = 0; idxIndex < numIndices; ++idxIndex)
        *idxOut++ = vertsBaseIndex + *indices++;
}

// Decomp: CoDMPServer.c:864130
int GlassShard::Split(
                const GlassShard **newShards,
                float minShardSize,
                unsigned int startEdge,
                float startEdgeParam) const
{
    GlassTimer timer;
    timer.counter = &clGlasses->renderer->splitTimer;
    timer.start = tlPcGetTick().QuadPart;
    ++clGlasses->renderer->numSplits;

    const float outlineLen = this->outline.Length();
    const float minEdgeLength = outlineLen * clGlasses->renderer->shardEdgeSize->current.value;
    const float maxEdgeLength = outlineLen * clGlasses->renderer->shardEdgeSize->current.vector[1];

    bool dirSign = false;
    float newEdgeDist = -1.0f;
    int newEdgeIdx = -1;
    float newDir[2];

    const auto nextEdgeForward = [this](unsigned int edge) -> unsigned int
    {
        return (edge + 1 >= this->outline.numVerts) ? 0u : edge + 1;
    };
    const auto prevEdge = [this](unsigned int edge) -> unsigned int
    {
        return (edge == 0) ? this->outline.numVerts - 1 : edge - 1;
    };

    if (startEdgeParam >= 0.0f)
    {
        const GlassShard::Outline::Vertex &startVert = this->outline.verts[startEdge];
        const float *edgeDir = startVert.edge.dir;
        const float along = startVert.edge.len * startEdgeParam;
        float start[2] = {
            along * edgeDir[0] + startVert.edge.origin[0],
            along * edgeDir[1] + startVert.edge.origin[1]
        };
        for (int tryIndex = 0; tryIndex < 10; ++tryIndex)
        {
            dirSign = irand(0, 2) != 0;
            const float sign = dirSign ? 1.0f : -1.0f;
            newDir[0] = edgeDir[0];
            newDir[1] = edgeDir[1];
            VectorAngleMultiply(newDir, -flrand(30.0, 150.0) * sign);
            if (this->outline.GetNumIntersections(start, newDir, &newEdgeDist, &newEdgeIdx) % 2 == 1
                && newEdgeDist > minEdgeLength)
                break;
            newEdgeIdx = -1;
        }
    }
    else
    {
        startEdge = 64;
        startEdgeParam = 0.0f;
        for (int tryIndex = 0; tryIndex < 10 && startEdge >= 0x40; ++tryIndex)
        {
            float walkParam = flrand(0.0f, outlineLen);
            for (unsigned int edgeIndex = 0; edgeIndex < this->outline.numVerts; ++edgeIndex)
            {
                const float edgeLen = this->outline.verts[edgeIndex].edge.len;
                if (walkParam > edgeLen)
                {
                    walkParam -= edgeLen;
                    continue;
                }
                if (edgeLen / outlineLen > 0.025f)
                {
                    startEdge = edgeIndex;
                    float edgeT = walkParam / edgeLen;
                    if (edgeT > 0.75f)
                        edgeT = 0.75f;
                    if (edgeT < 0.25f)
                        edgeT = 0.25f;
                    startEdgeParam = edgeT;
                }
                break;
            }
            if (startEdge >= 0x40)
                continue;

            dirSign = irand(0, 2) != 0;
            const float sign = dirSign ? 1.0f : -1.0f;
            const GlassShard::Outline::Vertex &startVert = this->outline.verts[startEdge];
            newDir[0] = startVert.edge.dir[0];
            newDir[1] = startVert.edge.dir[1];
            const float firstAngle = flrand(
                clGlasses->renderer->shardSplitDir->current.value,
                clGlasses->renderer->shardSplitDir->current.vector[1]) * sign + 90.0f;
            VectorAngleMultiply(newDir, -firstAngle);

            const float along = startVert.edge.len * startEdgeParam;
            float pos[2] = {
                along * startVert.edge.dir[0] + startVert.edge.origin[0],
                along * startVert.edge.dir[1] + startVert.edge.origin[1]
            };
            if (this->outline.GetNumIntersections(pos, newDir, &newEdgeDist, &newEdgeIdx) % 2 == 1
                && newEdgeDist > minEdgeLength)
                break;
            newEdgeIdx = -1;
        }
    }

    if (newEdgeIdx < 0)
    {
        ++GlassShard::splitFailCount[0];
        *timer.counter += tlPcGetTick().QuadPart - timer.start;
        return 0;
    }

    TempOutline newOutline;
    TempOutline otherOutline;
    newOutline.verts = newOutline.v;
    newOutline.numVerts = 0;
    newOutline.maxVerts = 64;
    newOutline.isClosed = 0;
    otherOutline.verts = otherOutline.v;
    otherOutline.numVerts = 0;
    otherOutline.maxVerts = 64;
    otherOutline.isClosed = 0;

    const GlassShard::Outline::Vertex &startVert = this->outline.verts[startEdge];
    float splitEdgeDir[2];

    newOutline.Add(startVert.edge.origin)->isOriginalEdge = startVert.isOriginalEdge;
    splitEdgeDir[0] = startVert.edge.dir[0];
    splitEdgeDir[1] = startVert.edge.dir[1];
    newOutline.Add(splitEdgeDir, startVert.edge.len * startEdgeParam)->isOriginalEdge = 0;
    otherOutline.Add(newOutline.verts[1].edge.origin);

    while (true)
    {
        const float segmentLen = flrand(minEdgeLength, maxEdgeLength);
        if (minEdgeLength * 0.5f > newEdgeDist - segmentLen)
            break;

        newOutline.Add(newDir, segmentLen)->isOriginalEdge = 0;
        otherOutline.Add(newOutline.verts[newOutline.numVerts - 1].edge.origin)->isOriginalEdge = 0;

        bool foundDir = false;
        for (int angleTry = 0; angleTry < 5; ++angleTry)
        {
            float tryDir[2] = { newDir[0], newDir[1] };
            float tryAngle = flrand(
                clGlasses->renderer->shardSplitDir2->current.value,
                clGlasses->renderer->shardSplitDir2->current.vector[1]);
            tryAngle *= dirSign ? -1.0f : 1.0f;
            VectorAngleMultiply(tryDir, tryAngle);

            const float *lastOrigin = newOutline.verts[newOutline.numVerts - 1].edge.origin;
            if (!newOutline.DoesIntersect(lastOrigin, tryDir, 10000.0, minEdgeLength)
                && this->outline.GetNumIntersections(lastOrigin, tryDir, &newEdgeDist, &newEdgeIdx))
            {
                newDir[0] = tryDir[0];
                newDir[1] = tryDir[1];
                foundDir = true;
                break;
            }
        }
        dirSign = !dirSign;
        if (!foundDir)
        {
            ++GlassShard::splitFailCount[1];
            *timer.counter += tlPcGetTick().QuadPart - timer.start;
            return 0;
        }
    }

    newOutline.Add(newDir, newEdgeDist)->isOriginalEdge = this->outline.verts[newEdgeIdx].isOriginalEdge;
    otherOutline.Add(newOutline.verts[newOutline.numVerts - 1].edge.origin)->isOriginalEdge =
        this->outline.verts[newEdgeIdx].isOriginalEdge;

    for (unsigned int edgeIndex = nextEdgeForward((unsigned int)newEdgeIdx);
         edgeIndex != startEdge;
         edgeIndex = nextEdgeForward(edgeIndex))
    {
        newOutline.Add(this->outline.verts[edgeIndex].edge.origin)->isOriginalEdge =
            this->outline.verts[edgeIndex].isOriginalEdge;
    }

    if (!newOutline.CloseOutline())
    {
        *timer.counter += tlPcGetTick().QuadPart - timer.start;
        return 0;
    }

    for (unsigned int edgeIndex = (unsigned int)newEdgeIdx; edgeIndex != startEdge; edgeIndex = prevEdge(edgeIndex))
    {
        otherOutline.Add(this->outline.verts[edgeIndex].edge.origin)->isOriginalEdge =
            this->outline.verts[prevEdge(edgeIndex)].isOriginalEdge;
    }

    if (!otherOutline.CloseOutline())
    {
        *timer.counter += tlPcGetTick().QuadPart - timer.start;
        return 0;
    }

    const int inited = GlassShard::InitSplitShards(&newOutline, &otherOutline, newShards, minShardSize);
    *timer.counter += tlPcGetTick().QuadPart - timer.start;
    return inited;
}

// Decomp: CoDMPServer.c:864451
int GlassShard::Chip(
                const GlassShard::Outline::EdgeDistance *dist,
                const float *hitPoint,
                const GlassShard **newShards,
                float minShardSize)
{
    GlassTimer timer;
    timer.counter = &clGlasses->renderer->splitTimer;
    timer.start = tlPcGetTick().QuadPart;
    ++clGlasses->renderer->numSplits;

    TempOutline outline1;
    TempOutline outline2;
    outline1.verts = outline1.v;
    outline1.numVerts = 0;
    outline1.maxVerts = 64;
    outline1.isClosed = 0;
    outline2.verts = outline2.v;
    outline2.numVerts = 0;
    outline2.maxVerts = 64;
    outline2.isClosed = 0;

    const bool hitEdgeOriginal = this->outline.verts[dist->edgeIndex].isOriginalEdge;
    outline1.Add(this->outline.verts[dist->edgeIndex].edge.origin)->isOriginalEdge = hitEdgeOriginal;
    outline1.Add(dist->closestPoint)->isOriginalEdge = 0;
    outline2.Add(dist->closestPoint)->isOriginalEdge = 0;
    outline1.Add(hitPoint)->isOriginalEdge = 0;
    outline2.Add(hitPoint)->isOriginalEdge = 0;

    const float dirSign = (irand(0, 2) != 0) ? 1.0f : -1.0f;
    float newDir[2] = { outline1.verts[1].edge.dir[0], outline1.verts[1].edge.dir[1] };
    VectorAngleMultiply(newDir, flrand(30.0, 60.0) * dirSign);

    float newEdgeDist = -1.0f;
    int newEdgeIdx = -1;
    if (this->outline.GetNumIntersections(hitPoint, newDir, &newEdgeDist, &newEdgeIdx) < 1)
    {
        *timer.counter += tlPcGetTick().QuadPart - timer.start;
        return 0;
    }

    outline1.Add(newDir, newEdgeDist)->isOriginalEdge = this->outline.verts[newEdgeIdx].isOriginalEdge;
    outline2.Add(newDir, newEdgeDist)->isOriginalEdge = this->outline.verts[newEdgeIdx].isOriginalEdge;

    unsigned int edgeIndex = (unsigned int)(newEdgeIdx + 1);
    if (edgeIndex >= this->outline.numVerts)
        edgeIndex = 0;
    while (edgeIndex != dist->edgeIndex)
    {
        outline1.Add(this->outline.verts[edgeIndex].edge.origin)->isOriginalEdge =
            this->outline.verts[edgeIndex].isOriginalEdge;
        if (++edgeIndex >= this->outline.numVerts)
            edgeIndex = 0;
    }
    outline1.CloseOutline();

    for (edgeIndex = newEdgeIdx; edgeIndex != dist->edgeIndex; )
    {
        const unsigned int prevEdge = edgeIndex ? edgeIndex - 1 : this->outline.numVerts - 1;
        outline2.Add(this->outline.verts[edgeIndex].edge.origin)->isOriginalEdge =
            this->outline.verts[prevEdge].isOriginalEdge;
        edgeIndex = prevEdge;
    }
    outline2.CloseOutline();

    const int inited = GlassShard::InitSplitShards(&outline1, &outline2, newShards, minShardSize);
    *timer.counter += tlPcGetTick().QuadPart - timer.start;
    return inited;
}

// Decomp: CoDMPServer.c:864569
int GlassShard::InitSplitShards(
                GlassShard::Outline *outline1,
                GlassShard::Outline *outline2,
                const GlassShard **newShards,
                float minShardSize) const
{
    float otherOffset[2];
    float offset[2];

    if (outline1->numVerts < 3u || outline2->numVerts < 3u || outline1->numVerts > 0x16u || outline2->numVerts > 0x16u)
    {
        ++GlassShard::splitFailCount[3];
        return 0;
    }
    if (minShardSize > outline1->Area())
    {
        ++GlassShard::splitFailCount[4];
        return 0;
    }
    if (minShardSize > outline2->Area())
    {
        ++GlassShard::splitFailCount[4];
        return 0;
    }
    if (outline1->HasNarrowAngle() || outline2->HasNarrowAngle())
    {
        ++GlassShard::splitFailCount[6];
        return 0;
    }
    if (outline1->IsNarrow() || outline2->IsNarrow())
    {
        ++GlassShard::splitFailCount[7];
        return 0;
    }
    *newShards = clGlasses->renderer->AllocShard();
    if (*newShards)
    {
        otherOffset[0] = 0.0f;
        otherOffset[1] = 0.0f;
        outline2->Reverse();
        //if (GlassShard::Init(*newShards, a2, this, outline2, otherOffset))
        if (((GlassShard*)*newShards)->Init(this, outline2, otherOffset))
        {
            offset[0] = 0.0f;
            offset[1] = 0.0f;
            //if (GlassShard::Init(this, a2, this, outline1, offset))
            if (((GlassShard *)this)->Init(this, outline1, offset)) // KISAKTODO: sus
            {
                return 1;
            }
            else
            {
                clGlasses->renderer->FreeShard((GlassShard*)*newShards);
                //if (!GlassShard::Init(this, a2, this, outline2, otherOffset))
                if (!((GlassShard *)this)->Init(this, outline2, otherOffset))
                {
                    //if (!Assert_MyHandler(
                    //    a2,
                    //    "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp",
                    //    2851,
                    //    0,
                    //    "Out of shard memory"))
                    //    __debugbreak();
                    Com_Error(ERR_DROP, "Out of shard memory\n");
                }
                ++GlassShard::splitFailCount[2];
                return 0;
            }
        }
        else
        {
            ++GlassShard::splitFailCount[2];
            clGlasses->renderer->FreeShard((GlassShard*)*newShards);
            return 0;
        }
    }
    else
    {
        ++GlassShard::splitFailCount[5];
        return 0;
    }
}

// Decomp: CoDMPServer.c:864661
char __thiscall GlassShard::Init(
                const GlassShard *other,
                const GlassShard::Outline *newOutline, // KISAKTODO: convert to ref &
                const float * const offset)
{
    newOutline->Verify();
    GlassShard::FreeMemory();

    GlassShard::Triangles triangles(newOutline);

    //GlassShard::Triangles::Triangles(&triangles, newOutline);

    if ( !triangles.Triangulate() )
        return 0;
    if ( !GlassShard::AllocateMemory(newOutline->numVerts, &triangles) )
        return 0;

    //GlassShard::Outline::operator=(&this->outline, newOutline);
    this->outline = *newOutline;
    this->thickness = other->thickness;
    this->uvScale = other->uvScale;
    this->glassIndex = other->glassIndex;
    this->axis[0][0] = other->axis[0][0];
    this->axis[0][1] = other->axis[0][1];
    this->axis[0][2] = other->axis[0][2];
    this->axis[1][0] = other->axis[1][0];
    this->axis[1][1] = other->axis[1][1];
    this->axis[1][2] = other->axis[1][2];
    this->axis[2][0] = other->axis[2][0];
    this->axis[2][1] = other->axis[2][1];
    this->axis[2][2] = other->axis[2][2];
    this->origin[0] = other->origin[0];
    this->origin[1] = other->origin[1];
    this->origin[2] = other->origin[2];
    this->origin[0] += offset[0] * this->axis[0][0];
    this->origin[1] += offset[0] * this->axis[0][1];
    this->origin[2] += offset[0] * this->axis[0][2];
    this->origin[0] += offset[1] * this->axis[1][0];
    this->origin[1] += offset[1] * this->axis[1][1];
    this->origin[2] += offset[1] * this->axis[1][2];
    this->isOriginalEdge = 0;
    for (unsigned int vertIndex = 0; vertIndex < this->outline.numVerts && !this->isOriginalEdge; ++vertIndex)
        this->isOriginalEdge = this->outline.verts[vertIndex].isOriginalEdge;
    return 1;
}

// Decomp: CoDMPServer.c:864720
double __thiscall GlassShard::GetMass()
{
    const float volume = this->outline.Area() * fabs(this->thickness);
    if ( volume < 100.0 )
        return 0.001;
    if ( volume <= 1000.0 )
        return (0.0099999998 - 0.001) * ((volume - 100.0) / (1000.0 - 100.0)) + 0.001;
    return 0.0099999998;
}

// Decomp: CoDMPServer.c:864740
bool __thiscall GlassShard::InitPhysicsObj(bool enableCollisions)
{
    gjk_aabb_t *aabb_gjk_geom;
    float mass;
    GlassShard *other;
    PhysPreset physPreset;
    float localBBoxMin[3];
    float quat[4];
    float localBBoxMax[3];
    gjk_geom_list_t gjk_geom_list;

    this->timeUntilAction = clGlasses->renderer->maxShardLife->current.value;
    if ( this->physObjId || this->glassPhysics )
        return 1;
    //if ( enableCollisions && GlassRenderer::AddColidingShard(clGlasses->renderer, this) )
    if ( enableCollisions && clGlasses->renderer->AddColidingShard(this) )
    {
        //other = (GlassShard *)GlassRenderer::GetSmallestColidingShard(clGlasses->renderer);
        other = (GlassShard *)clGlasses->renderer->GetSmallestColidingShard();
        if ( other == this )
        {
            clGlasses->renderer->RemoveColidingShard(this);
            enableCollisions = 0;
        }
        else
        {
            other->DisableCollisions();
        }
    }
    GlassShard::GetLocalBBox(localBBoxMin, localBBoxMax);
    if ( enableCollisions )
    {
        Com_Memset((unsigned int *)&physPreset, 0, 84);
        physPreset.mass = GlassShard::GetMass();
        physPreset.friction = 0.5f;
        AxisToQuat(this->axis, quat);
        Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
        Sys_EnterCriticalSection(CRITSECT_PHYSICS);
        gjk_geom_list.m_first_geom = 0;
        gjk_geom_list.m_geom_count = 0;
        aabb_gjk_geom = create_aabb_gjk_geom(
                                            localBBoxMin,
                                            localBBoxMax,
                                            9,
                                            &g_empty_collision_visitor);
        //gjk_geom_list_t::add_geom(&gjk_geom_list, aabb_gjk_geom);
        gjk_geom_list.add_geom(aabb_gjk_geom);
        PhysObjUserData *obj = Phys_ObjCreate(1, this->origin, quat, vec3_origin, &physPreset, &gjk_geom_list, 1, -1);
        this->physObjId = (int)obj;

        broad_phase_base *bpb = obj->m_bpb;
        aasap_list_remove(bpb);
        bpb->m_env_collision_flags &= ~0x40u;
        bpb->m_my_collision_type_flags |= 0x100u;

        Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
        Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
        return 1;
    }
    else
    {
        this->glassPhysics = clGlasses->renderer->AllocPhysics();
        if ( this->glassPhysics )
        {
            mass = GlassShard::GetMass();
            this->glassPhysics->CreateAxis(
                this->origin,
                this->axis,
                (float *)vec3_origin,
                (float *)vec3_origin,
                mass,
                localBBoxMin,
                localBBoxMax);
        }
        return this->glassPhysics != 0;
    }
}

// Decomp: CoDMPServer.c:864838
void __thiscall GlassShard::DestroyPhysicsObj()
{
    if ( this->physObjId )
    {
        clGlasses->renderer->RemoveColidingShard(this);
        Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
        Sys_EnterCriticalSection(CRITSECT_PHYSICS);
        Phys_ObjDestroy(1, this->physObjId);
        Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
        Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
        this->physObjId = 0;
    }
    if ( this->glassPhysics )
    {
        clGlasses->renderer->FreePhysics(this->glassPhysics);
        this->glassPhysics = 0;
    }
}

// Decomp: CoDMPServer.c:864861
void __thiscall GlassShard::DisableCollisions()
{
    float mass;
    float localBBoxMin[3];
    float localBBoxMax[3];
    float aVel[3];
    float tVel[3];

    if ( !this->physObjId
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 3009, 0, "%s", "physObjId") )
    {
        __debugbreak();
    }
    if ( this->glassPhysics
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 3010, 0, "%s", "!glassPhysics") )
    {
        __debugbreak();
    }
    clGlasses->renderer->RemoveColidingShard(this);
    this->glassPhysics = clGlasses->renderer->AllocPhysics();
    if ( this->glassPhysics )
    {
        Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
        Sys_EnterCriticalSection(CRITSECT_PHYSICS);
        Phys_ObjGetVelocities(this->physObjId, tVel, aVel);
        Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
        Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
        GlassShard::GetLocalBBox(localBBoxMin, localBBoxMax);
        mass = GlassShard::GetMass();
        this->glassPhysics->CreateAxis(
            this->origin,
            this->axis,
            tVel,
            aVel,
            mass,
            localBBoxMin,
            localBBoxMax);
    }
    else
    {
        GlassShard::Remove(REMOVE_OUT_OF_PHYSICS, 1);
    }
    Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
    Sys_EnterCriticalSection(CRITSECT_PHYSICS);
    Phys_ObjDestroy(1, this->physObjId);
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
    this->physObjId = 0;
}

// Decomp: CoDMPServer.c:864929
void __thiscall GlassShard::AddForce(float *pos, const float *forceIn)
{
    float force[3];
    const float size = Vec3NormalizeTo(forceIn, force);
    if (size > 0.0f)
    {
        const float clampedSize = (size > 500000.0f) ? 500000.0f : size;
        force[0] *= clampedSize;
        force[1] *= clampedSize;
        force[2] *= clampedSize;
        if ( this->physObjId )
        {
            Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
            Sys_EnterCriticalSection(CRITSECT_PHYSICS);
            Phys_ObjAddForce(this->physObjId, pos, force, 0);
            Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
            Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
        }
        else
        {
            if ( !this->glassPhysics
                && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 3059, 0, "%s", "glassPhysics") )
            {
                __debugbreak();
            }
            this->glassPhysics->AddForce(pos, force);
        }
    }
}

// Decomp: CoDMPServer.c:864978
void __thiscall GlassShard::SetVelocity(float *t_vel, float *a_vel)
{
    if ( this->physObjId )
    {
        Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
        Sys_EnterCriticalSection(CRITSECT_PHYSICS);
        Phys_ObjSetVelocity(this->physObjId, t_vel);
        Phys_ObjSetAngularVelocity(this->physObjId, a_vel);
        Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
        Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
    }
    else
    {
        if ( !this->glassPhysics
            && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 3078, 0, "%s", "glassPhysics") )
        {
            __debugbreak();
        }
        this->glassPhysics->SetVelocity(t_vel);
        this->glassPhysics->SetAngularVelocity(a_vel);
    }
}

// Decomp: CoDMPServer.c:865012
bool __thiscall GlassShard::LeaveOnEdge(float stickiness)
{
    if ( !this->isOriginalEdge
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 3086, 0, "%s", "isOriginalEdge") )
    {
        __debugbreak();
    }
    if (stickiness > 0.0f)
    {
        const float prob = (GlassShard::EdgeRatio() - 0.15000001f) * stickiness / (0.34999999f - 0.15000001f);
        return prob > 0.0f && prob > random();
    }
    return false;
}

// Decomp: CoDMPServer.c:865042
bool __thiscall GlassShard::IsOnBottomEdge()
{
    iassert(this->isOriginalEdge);
    if (fabs(this->axis[2][2]) >= 0.01f)
        return false;

    int numOriginalEdges = 0;
    unsigned int bottomEdgeIdx = (unsigned int)-1;
    float wDir[3];

    for (unsigned int vertIndex = 0; vertIndex < this->outline.numVerts; ++vertIndex)
    {
        if (!this->outline.verts[vertIndex].isOriginalEdge)
            continue;
        if (++numOriginalEdges == 2)
            return false;
        GlassShard::ToWorldDir(this->outline.verts[vertIndex].edge.dir, wDir, 0);
        if (fabs(wDir[2]) < 0.01f)
            bottomEdgeIdx = vertIndex;
    }

    if (bottomEdgeIdx == (unsigned int)-1)
        return false;

    const unsigned int bottomEdgeNext = (bottomEdgeIdx + 1 >= this->outline.numVerts) ? 0u : bottomEdgeIdx + 1;
    float minEdgeZ = FLT_MAX;
    float minOtherZ = FLT_MAX;
    float wPos[3];

    for (unsigned int vertIndex = 0; vertIndex < this->outline.numVerts; ++vertIndex)
    {
        GlassShard::ToWorldPos(this->outline.verts[vertIndex].edge.origin, wPos, 0);
        if (vertIndex == bottomEdgeIdx || vertIndex == bottomEdgeNext)
        {
            if (wPos[2] < minEdgeZ)
                minEdgeZ = wPos[2];
        }
        else
        {
            if (wPos[2] < minOtherZ)
                minOtherZ = wPos[2];
        }
    }
    return minOtherZ > minEdgeZ;
}

// Decomp: CoDMPServer.c:865124
void __thiscall GlassShard::Recenter()
{
    float offset[2];
    this->outline.Recenter(0, offset);
    this->origin[0] += offset[0] * this->axis[0][0];
    this->origin[1] += offset[0] * this->axis[0][1];
    this->origin[2] += offset[0] * this->axis[0][2];
    this->origin[0] += offset[1] * this->axis[1][0];
    this->origin[1] += offset[1] * this->axis[1][1];
    this->origin[2] += offset[1] * this->axis[1][2];
}

// Decomp: CoDMPServer.c:865149
char __thiscall GlassShard::InitPhysics(
                bool enableCollisions,
                const float *hitPos,
                const float *hitDir,
                float glassSize,
                float stickiness)
{
    if (!this->outline.verts)
    {
        GlassShard::Remove(REMOVE_OUT_OF_SHARD_MEMORY, 1);
        return 0;
    }

    this->delayedDrop = 0;
    if (this->isOriginalEdge && stickiness > 0.0f && !GlassShard::IsOnBottomEdge())
    {
        GlassShard::UpdateBBox();
        if (!GlassShard::LeaveOnEdge(stickiness))
        {
            this->delayedDrop = 1;
            this->timeUntilAction = flrand(
                clGlasses->renderer->timeUntilDropRange->current.value,
                clGlasses->renderer->timeUntilDropRange->current.vector[1]);
        }
        return 1;
    }

    if (!GlassShard::InitPhysicsObj(enableCollisions))
    {
        GlassShard::Remove(REMOVE_OUT_OF_PHYSICS, 1);
        return 0;
    }

    float velocity[3];
    const float dirLengthSq = hitDir[0] * hitDir[0] + hitDir[1] * hitDir[1] + hitDir[2] * hitDir[2];

    if (dirLengthSq == 0.0f)
    {
        const float extraVel = clGlasses->renderer->extraVelocity->current.value;
        velocity[0] = flrand(-extraVel, extraVel);
        velocity[1] = flrand(-extraVel, extraVel);
        velocity[2] = flrand(-extraVel, extraVel);
    }
    else if (dirLengthSq <= 1.01f)
    {
        iassert(glassSize > 0.0f);
        float impulseDir[3] = {
            this->origin[0] - hitPos[0],
            this->origin[1] - hitPos[1],
            this->origin[2] - hitPos[2]
        };
        const float dist = Vec3Normalize(impulseDir);
        const float originMult = clGlasses->renderer->forceOriginMult->current.value;
        impulseDir[0] = originMult * impulseDir[0] + hitDir[0];
        impulseDir[1] = originMult * impulseDir[1] + hitDir[1];
        impulseDir[2] = originMult * impulseDir[2] + hitDir[2];

        float attenParam = clGlasses->renderer->forceAttenuation->current.value * dist / glassSize;
        if (attenParam > 1.0f)
            attenParam = 1.0f;
        if (attenParam < 0.25f)
            attenParam = 0.25f;
        const float distAtten = 1.0f - attenParam;

        const float forceMult = clGlasses->renderer->forceMultiplier->current.value;
        velocity[0] = forceMult * distAtten * impulseDir[0];
        velocity[1] = forceMult * distAtten * impulseDir[1];
        velocity[2] = forceMult * distAtten * impulseDir[2];
    }
    else
    {
        float impulseDir[3] = {
            this->origin[0] - hitPos[0],
            this->origin[1] - hitPos[1],
            this->origin[2] - hitPos[2]
        };
        Vec3Normalize(impulseDir);
        float strength = sqrtf(dirLengthSq) - 1.0f;
        float strengthScale = strength * 2.0f;
        if (strengthScale > 1.0f)
            strengthScale = 1.0f;
        if (strengthScale < 0.2f)
            strengthScale = 0.2f;
        const float forceMult = clGlasses->renderer->forceMultiplier->current.value;
        velocity[0] = forceMult * strengthScale * impulseDir[0];
        velocity[1] = forceMult * strengthScale * impulseDir[1];
        velocity[2] = forceMult * strengthScale * impulseDir[2];
    }

    const float extraAngVel = clGlasses->renderer->extraVelocity->current.value;
    float angVelocity[3] = {
        flrand(-extraAngVel, extraAngVel),
        flrand(-extraAngVel, extraAngVel),
        flrand(-extraAngVel, extraAngVel)
    };
    GlassShard::SetVelocity(velocity, angVelocity);
    GlassShard::UnEdge();
    return 1;
}

// Decomp: CoDMPServer.c:865308
void __thiscall GlassShard::DrawOutline()
{
    float worldVerts[64][3];

    for (unsigned int vertIndex = 0; vertIndex < this->outline.numVerts; ++vertIndex)
    {
        const float localX = this->outline.verts[vertIndex].edge.origin[0];
        const float localY = this->outline.verts[vertIndex].edge.origin[1];
        float temp[3];
        temp[0] = localX * this->axis[0][0] + this->origin[0];
        temp[1] = localX * this->axis[0][1] + this->origin[1];
        temp[2] = localX * this->axis[0][2] + this->origin[2];
        temp[0] += localY * this->axis[1][0];
        temp[1] += localY * this->axis[1][1];
        temp[2] += localY * this->axis[1][2];
        worldVerts[vertIndex][0] = temp[0];
        worldVerts[vertIndex][1] = temp[1];
        worldVerts[vertIndex][2] = temp[2];
    }

    for (unsigned int edgeIndex = 0; edgeIndex < this->outline.numVerts; ++edgeIndex)
    {
        float *color = this->outline.verts[edgeIndex].isOriginalEdge ? (float *)colorBlue : (float *)colorYellow;
        CL_AddDebugLine(
            worldVerts[edgeIndex],
            worldVerts[(edgeIndex + 1) % this->outline.numVerts],
            color,
            1,
            0);
    }

    if (clGlasses->renderer->drawShardOutline->current.integer <= 0)
    {
        if (clGlasses->renderer->pickShards->current.enabled)
        {
            const float edgePct = GlassShard::EdgeRatio() * 100.0f;
            float localBBoxMin[3];
            float localBBoxMax[3];
            GlassShard::GetLocalBBox(localBBoxMin, localBBoxMax);
            float bboxExtent[3] = {
                localBBoxMax[0] - localBBoxMin[0],
                localBBoxMax[1] - localBBoxMin[1],
                localBBoxMax[2] - localBBoxMin[2]
            };
            const float maxXY = (bboxExtent[0] > bboxExtent[1]) ? bboxExtent[0] : bboxExtent[1];
            const float minXY = (bboxExtent[0] < bboxExtent[1]) ? bboxExtent[0] : bboxExtent[1];
            const float area2bboxRatio = bboxExtent[0] * bboxExtent[1] / this->outline.Area();
            const char *label = va(
                "%d %.1f %.2f (%.2f %.2f) %d",
                this->outline.numVerts,
                this->outline.Area(),
                edgePct,
                maxXY / minXY,
                area2bboxRatio,
                this->outline.IsNarrow());
            CL_AddDebugString(this->origin, colorWhite, 0.25f, const_cast<char *>(label), 0);
            CL_AddDebugBox(this->axis, this->origin, localBBoxMin, localBBoxMax, colorBlue, 1, 0);
        }
    }
    else if (clGlasses->renderer->drawShardOutline->current.integer > 1)
    {
        const char *label = va("%d", this->outline.numVerts);
        CL_AddDebugString(this->origin, colorWhite, 0.5f, const_cast<char *>(label), 0);
    }
}