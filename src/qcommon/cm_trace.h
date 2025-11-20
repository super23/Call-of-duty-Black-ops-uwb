#pragma once

void __cdecl RotatePoint(const float *v, const float *q, float *out);
col_context_t *__thiscall col_context_t::col_context_t(col_context_t *this);
col_context_t *__thiscall col_context_t::col_context_t(col_context_t *this, int _mask);
void __thiscall col_context_t::init_locational(col_context_t *this, int ent0);
void __thiscall col_context_t::init_locational(col_context_t *this, int ent0, int ent1);
unsigned __int16 __cdecl Trace_GetEntityHitId(const trace_t *trace);
unsigned __int16 __cdecl Trace_GetDynEntHitId(const trace_t *trace, DynEntityDrawType *drawType);
int __cdecl Trace_GetGlassHitId(const trace_t *trace);
void __cdecl CM_GetTraceThreadInfo(TraceThreadInfo *threadInfo);
clipMap_t *__cdecl CM_GetClipMap();
unsigned int __cdecl CM_TempBoxModel(const float *mins, const float *maxs, int contents);
void __cdecl CM_GetBox(cbrush_t **box_brush, cmodel_t **box_model, PhysGeomList ***geoms);
unsigned int __cdecl CM_TempBrushModel(PhysGeomList *geoms);
bool __cdecl CM_ClipHandleIsValid(unsigned int handle);
cmodel_t *__cdecl CM_ClipHandleToModel(unsigned int handle);
int __cdecl CM_ContentsOfModel(unsigned int handle);
void __cdecl CM_Trace(
        trace_t *results,
        const float *start,
        const float *end,
        const float *mins,
        const float *maxs,
        unsigned int model,
        int brushmask,
        col_context_t *context);
void __cdecl CM_TestInLeaf(traceWork_t *tw, cLeaf_s *leaf, trace_t *trace);
bool __cdecl CM_TestInLeafBrushNode(traceWork_t *tw, cLeaf_s *leaf, trace_t *trace);
void __cdecl CM_TestInLeafBrushNode_r(const traceWork_t *tw, cLeafBrushNode_s *node, trace_t *trace);
void __cdecl CM_TestBoxInBrush(const traceWork_t *tw, const cbrush_t *brush, trace_t *trace);
void __cdecl CM_TestCapsuleInCapsule(const traceWork_t *tw, trace_t *trace);
void __cdecl CM_PositionTest(traceWork_t *tw, trace_t *trace, col_context_t *context);
void __cdecl CM_TraceThroughLeaf(const traceWork_t *tw, cLeaf_s *leaf, trace_t *trace);
bool __cdecl CM_TraceThroughLeafBrushNode(const traceWork_t *tw, cLeaf_s *leaf, trace_t *trace);
void __cdecl CM_TraceThroughLeafBrushNode_r(
        const traceWork_t *tw,
        cLeafBrushNode_s *node,
        const float *p1_,
        const float *p2,
        trace_t *trace);
void __cdecl CM_TraceThroughBrush(const traceWork_t *tw, const cbrush_t *brush, trace_t *trace);
void __cdecl CM_TestInTempBrush(const traceWork_t *tw, trace_t *trace);
void __cdecl CM_TraceThroughTempBrush(const traceWork_t *tw, trace_t *trace);
void __cdecl CM_TraceCapsuleThroughCapsule(const traceWork_t *tw, trace_t *trace);
int __cdecl CM_TraceSphereThroughSphere(
        const traceWork_t *tw,
        const float *vStart,
        const float *vEnd,
        const float *vStationary,
        float radius,
        trace_t *trace);
int __cdecl CM_TraceCylinderThroughCylinder(
        const traceWork_t *tw,
        const float *vStationary,
        float fStationaryHalfHeight,
        float radius,
        trace_t *trace);
void __cdecl CM_TraceThroughTree(const traceWork_t *tw, int num, const float *p1_, const float *p2, trace_t *trace);
void __cdecl CM_TraceThroughPrimitives(
        const traceWork_t *tw,
        const float *p1_,
        const float *p2,
        trace_t *trace,
        col_context_t *context);
void __cdecl CM_SetAxialCullOnly(traceWork_t *tw);
void __cdecl CM_BoxTrace(
        trace_t *results,
        const float *start,
        const float *end,
        const float *mins,
        const float *maxs,
        int brushmask,
        col_context_t *context);
void __cdecl CM_TransformedBoxTraceRotated(
        trace_t *results,
        const float *start,
        const float *end,
        const float *mins,
        const float *maxs,
        unsigned int model,
        int brushmask,
        const float *origin,
        const float (*matrix)[3]);
void __cdecl TransposeMatrix(const float (*matrix)[3], float (*transpose)[3]);
void __cdecl RotatePoint(float *point, const float (*mat)[3]);
void __cdecl CM_TransformedBoxTrace(
        trace_t *results,
        const float *start,
        const float *end,
        const float *mins,
        const float *maxs,
        unsigned int model,
        int brushmask,
        const float *origin,
        const float *angles);
void __cdecl CM_TransformedBoxTraceExternal(
        trace_t *results,
        const float *start,
        const float *end,
        const float *mins,
        const float *maxs,
        unsigned int model,
        int brushmask,
        const float *origin,
        const float *angles);
int __cdecl CM_SightTraceThroughBrush(const traceWork_t *tw, const cbrush_t *brush, int brushNum, trace_t *trace);
int __cdecl CM_SightTracePointThroughBrush(const TraceExtents *extents, const cbrush_t *brush);
int __cdecl CM_SightTraceThroughTree(const traceWork_t *tw, int num, const float *p1_, const float *p2, trace_t *trace);
int __cdecl CM_SightTraceThroughLeaf(const traceWork_t *tw, const cLeaf_s *leaf, trace_t *trace);
int __cdecl CM_SightTraceThroughLeafBrushNode(const traceWork_t *tw, const cLeaf_s *leaf, trace_t *trace);
int __cdecl CM_SightTraceThroughLeafBrushNode_r(
        const traceWork_t *tw,
        cLeafBrushNode_s *remoteNode,
        const float *p1_,
        const float *p2,
        trace_t *trace);
int __cdecl CM_BoxSightTrace(
        int oldHitNum,
        const float *start,
        const float *end,
        const float *mins,
        const float *maxs,
        unsigned int model,
        int brushmask);
int __cdecl CM_SightTraceThroughTempBrush(const traceWork_t *tw, trace_t *trace);
int __cdecl CM_SightTraceCapsuleThroughCapsule(const traceWork_t *tw, trace_t *trace);
bool __cdecl CM_SightTraceSphereThroughSphere(
        const traceWork_t *tw,
        const float *vStart,
        const float *vEnd,
        const float *vStationary,
        float radius,
        trace_t *trace);
bool __cdecl CM_SightTraceCylinderThroughCylinder(
        const traceWork_t *tw,
        const float *vStationary,
        float fStationaryHalfHeight,
        float radius,
        trace_t *trace);
int __cdecl CM_SightTracePoint(int oldHitNum, const float *start, const float *end, col_context_t *context);
int __cdecl CM_TransformedBoxSightTrace(
        int hitNum,
        const float *start,
        const float *end,
        const float *mins,
        const float *maxs,
        unsigned int model,
        int brushmask,
        const float *origin,
        const float *angles);
int __cdecl CM_TracePointDown(
        const float *start,
        const float *end,
        int contentmask,
        int surf_type,
        float *endpos,
        float *fraction,
        float *normal);
double __cdecl CM_GetWaterHeight(const float *pos, float z_up, float z_down);
void __cdecl calc_closest(const float *v0, const float *v1, const float *v2, const float *p, float *res);
void __cdecl Vec3Mad(const float *start, float scale, const float *dir, float *result);
void __cdecl Vec3ScaleMad(float scale0, const float *dir0, float scale1, const float *dir1, float *result);
bool __cdecl is_inside(const float *a, const float *b, const float *n, const float *p);
bool __cdecl point_in_triangle(const float *p, const float *v0, const float *v1, const float *v2, const float *n);
char __cdecl trace_sphere_through_triangle(
        const float *c0,
        const float *dir,
        float *t,
        float r,
        const float *v0,
        const float *v1,
        const float *v2,
        const float *n);
void __cdecl trace_sphere_through_partition(
        const float *c,
        const float *dir,
        float *t,
        float r,
        const CollisionAabbTree *tree,
        float *hitn,
        int *sflags);
void __cdecl trace_sphere_through_brush(
        const float *c0,
        const float *dir,
        float *t,
        float r,
        const cbrush_t *brush,
        float *hitn,
        int *sflags);
void __cdecl trace_sphere_through_brush(
        const float *c0,
        const float *dir,
        float *t,
        float r,
        const cbrush_t *brush,
        float *hitn,
        int *sflags);
void __cdecl trace_point_through_brush(
        const float *c0,
        const float *dir,
        float *t,
        const cbrush_t *brush,
        float *hitn,
        int *sflags);
char __cdecl trace_point_through_triangle(
        const float *p0,
        const float *u0,
        const float *v0,
        const float *v1,
        const float *v2,
        float cur_t,
        float *t);
void __cdecl trace_sphere_through_partition(
        const float *p,
        const float *dir,
        float *t,
        float r,
        const CollisionPartition *partition,
        float *hitn,
        int *hint,
        clipMap_t *pClipMap);
void __cdecl trace_point_through_partition(
        const float *p,
        const float *dir,
        float *t,
        const CollisionPartition *partition,
        float *hitn,
        int *hint,
        clipMap_t *pClipMap);
void __cdecl trace_point_vs_env(
        trace_t *trace,
        const float *start,
        const float *end,
        col_context_t *context,
        int *index_hint,
        clipMap_t *pClipMap);
void __cdecl trace_point_vs_env(
        trace_t *trace,
        const float *start,
        const float *end,
        col_context_t *context,
        int *index_hint);
void __cdecl trace_point_vs_env(
        trace_t *trace,
        const float *start,
        const float *end,
        col_context_t *context,
        int *index_hint,
        unsigned __int8 *pClipMap);
void __cdecl trace_sphere_vs_env(
        trace_t *trace,
        const float *start,
        const float *end,
        float radius,
        col_context_t *context,
        int *index_hint,
        clipMap_t *pClipMap);
void __cdecl trace_sphere_vs_env(
        trace_t *trace,
        const float *start,
        const float *end,
        float radius,
        col_context_t *context,
        int *index_hint,
        unsigned __int8 *pClipMap);
int __cdecl collide_segment_brush(const float *p0, const float *p1, const cbrush_t *brush);
bool __cdecl collide_segment_triangle(
        const float *p0,
        const float *u0,
        const float *v0,
        const float *v1,
        const float *v2);
int __cdecl collide_segment(const float *p0, const float *p1, col_context_t *context);
char __cdecl CM_TestPointInBrushModel(const float *point, unsigned int brushmodel, const float *trans);
char __cdecl CM_GetWaterForce(const float *pt, float *dir, float *force);
char __cdecl CM_GetHeliHeight(const float *pt, float checkdist, float *result);
