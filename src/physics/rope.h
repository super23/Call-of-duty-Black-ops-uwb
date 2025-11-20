#pragma once

bool __cdecl Rope_IsValid(int rope_index);
void __cdecl Rope_SetFlag(int rope_index, int flags, int onoff);
void __cdecl Rope_Reset();
void __cdecl Rope_Reset(int rope_index);
void __cdecl Rope_SetMaterial(int rope_index, Material *const material);
int __cdecl Rope_GetNumParticles(int rope_index);
void __cdecl Rope_AddAnchor(
        int rope_index,
        char particle_index,
        const float *p,
        int entity_index,
        int bone_name_hash,
        rope_constraint_e type);
void __cdecl Rope_Activate(int rope_index);
void __cdecl Rope_AddDynEntAnchor(int rope_index, char particle_index, const float *p, int entity_index);
void __cdecl Rope_AddEntAnchor(
        int rope_index,
        char particle_index,
        const float *p,
        int entity_index,
        int bone_name_hash);
void __cdecl Rope_RemoveAnchor(int rope_index, int particle_index);
void __cdecl Rope_Create(
        int rope_index,
        const float *start,
        const float *end,
        float length,
        float scale,
        float width,
        float force_scale,
        int static_rope);
void __cdecl Rope_Create(int rope_index);
void __cdecl Rope_Init(
        int rope_index,
        int npars,
        const float *p0,
        const float *p1,
        float rope_len,
        float rope_width,
        float scale,
        float force_scale,
        int static_rope);
void __cdecl Rope_AddParticle(int rope_index, const float *pos);
void __cdecl Rope_UpdateInternal(const RopeUpdateCmdData *cmd);
void __cdecl Rope_Tick(const RopeUpdateCmdData *cmd, int rope_index, float dt, bool force_update);
void __cdecl Rope_ApplyPhysics(int rope_index, float dt);
void __cdecl Rope_ApplyConstraints(int rope_index, bool forward);
void __cdecl Rope_ApplyContactConstraint(int rope_index, int contact_index);
void __cdecl Rope_ApplyConstraint(int rope_index, int constraint_index);
void __cdecl Rope_UpdateVisible(int rope_index);
void __cdecl Rope_BuildCurve(const RopeUpdateCmdData *cmd, int rope_index);
// local variable allocation has failed, the output may be wrong!
void  Rope_Update(int a1@<ebp>, int localClientNum, int curtime);
void __cdecl Rope_Render(unsigned int localClientNum);
void __cdecl Rope_DebugDraw(int rope_index);
void __cdecl Rope_Draw(int rope_index, unsigned int localClientNum);
void __cdecl Rope_Settle(int rope_index);
void __cdecl Rope_ExplosionEvent(
        const float *origin,
        float innerRadius,
        float outerRadius,
        int innerDamage,
        int outerDamage);
void __cdecl Rope_ExplosionEvent(
        int rope_index,
        const float *origin,
        float innerRadius,
        float outerRadius,
        int innerDamage,
        int outerDamage);
double __cdecl PointToBoxDistSq(const float *pt, const float *mins, const float *maxs);
int __cdecl trace_point_through_sphere(
        const float *p,
        const float *ud,
        const float *ctr,
        float r,
        float *t,
        float *hitp);
void __cdecl Rope_Trace(const float *p0, const float *p1);
void __cdecl Rope_CollideWorld(int rope_index);
void __cdecl Rope_CollideEntitiesHelper(int rope_index, float *origin);
void __cdecl Rope_CollideEntities(int rope_index);
