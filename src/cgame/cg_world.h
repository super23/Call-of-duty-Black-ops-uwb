#pragma once

int __cdecl CG_GetEntityBModelContents(const centity_s *cent);
void  CG_GetEntityBModelBounds(
        int a1@<ebp>,
        const centity_s *cent,
        float *mins,
        float *maxs,
        float *absMins,
        float *absMaxs);
void  phys_calc_world_aabb(
        float a1@<ebp>,
        const phys_vec3 *local_center,
        const phys_vec3 *local_half_aabb_dims,
        const phys_mat44 *local_to_world_xform,
        phys_vec3 *aabb_min,
        phys_vec3 *aabb_max);
double __cdecl phys_dot(const phys_vec3 *a, const phys_vec3 *b);
phys_vec3 *__cdecl phys_AbsValue(phys_vec3 *result, const phys_vec3 *a);
const phys_vec3 * phys_full_multiply@<eax>(
        int a1@<ebp>,
        const phys_vec3 *result,
        const phys_mat44 *mat,
        const phys_vec3 *v);
void __cdecl phys_transpose(phys_mat44 *dest, const phys_mat44 *source);
void __cdecl CG_GetEntityDobjBounds(
        const centity_s *cent,
        const DObj *dobj,
        float *mins,
        float *maxs,
        float *absMins,
        float *absMaxs);
unsigned int __cdecl CG_GetEntityDObjContents(const centity_s *cent, const DObj *dobj);
bool __cdecl CG_EntityNeedsLinked(int localClientNum, unsigned int entIndex);
void __cdecl CG_UnlinkEntity(int localClientNum, unsigned int entIndex);
void __cdecl CG_CalcWorldBounds(centity_s *cent, DObj *dobj);
void __cdecl CG_LinkEntity(int localClientNum, unsigned int entIndex);
void __cdecl CG_PointTraceToEntity(const pointtrace_t *clip, unsigned int entIndex, trace_t *results);
void __cdecl CG_PointTraceToEntities(const pointtrace_t *clip, trace_t *results);
void __cdecl CG_PointTraceToEntities_r(
        const pointtrace_t *clip,
        unsigned __int16 sectorIndex,
        const float *p1,
        const float *p2,
        trace_t *results);
void __cdecl CG_TraceCapsule(
        trace_t *results,
        const float *start,
        const float *mins,
        const float *maxs,
        const float *end,
        int passEntityNum,
        int contentMask,
        col_context_t *context);
void __cdecl CG_ClipMoveToEntities(const moveclip_t *clip, trace_t *results);
void __cdecl CG_ClipMoveToEntities_r(
        const moveclip_t *clip,
        unsigned __int16 sectorIndex,
        const float *p1,
        const float *p2,
        trace_t *results);
void __cdecl CG_ClipMoveToEntity(const moveclip_t *clip, int entIndex, trace_t *results);
void __cdecl CG_LocationalTrace(
        trace_t *results,
        const float *start,
        const float *end,
        int passEntityNum,
        int contentMask,
        bool checkRopes,
        col_context_t *context);
void __cdecl CG_TracePoint(
        trace_t *results,
        const float *start,
        const float *end,
        int passEntityNum,
        int contentMask,
        bool locational,
        bool staticModels,
        bool checkRopes,
        col_context_t *context);
void __cdecl CG_AreaEntities_r(unsigned __int16 sectorIndex, CEntityAreaParms *areaParms);
char __cdecl CG_EntityInArea(unsigned int entIndex, const float *mins, const float *maxs, int contentMask);
int __cdecl CG_AreaEntities(const float *mins, const float *maxs, int *entityList, int maxEntities, int contentMask);
bool __cdecl CG_SightTracePointInternal(int *hitNum, const float *start, const float *end, int mask, trace_t *trace);
traceWork_t *__thiscall traceWork_t::traceWork_t(traceWork_t *this);
bool __cdecl CG_SightTracePoint(int *hitNum, const float *start, const float *end, int mask, trace_t *trace);
int __cdecl cmpr(float *e0, float *e1);
int __cdecl CG_GetEntityBoneInfo(int entID, int boneIndex, float *bonePos, float (*boneAxis)[3], char **boneName);
float *__thiscall phys_vec3::operator[]<int>(phys_vec3 *this, unsigned int i);
const float *__thiscall phys_vec3::operator[]<int>(phys_vec3 *this, unsigned int i);
