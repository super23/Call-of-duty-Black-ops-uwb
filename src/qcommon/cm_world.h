#pragma once

void __cdecl CM_LinkWorld();
bool CM_ClearWorld();
void __cdecl CM_UnlinkEntity(svEntity_s *ent);
void __cdecl CM_LinkEntity(svEntity_s *ent, float *absmin, float *absmax, unsigned int clipHandle);
void __cdecl CM_AddEntityToNode(svEntity_s *ent, unsigned __int16 childNodeIndex);
void __cdecl CM_SortNode(unsigned __int16 nodeIndex, float *mins, float *maxs);
unsigned __int16 __cdecl CM_AllocWorldSector(float *mins, float *maxs);
void __cdecl CM_AddStaticModelToNode(cStaticModel_s *staticModel, unsigned __int16 childNodeIndex);
unsigned int CM_LinkAllStaticModels();
void __cdecl CM_LinkStaticModel(cStaticModel_s *staticModel);
int __cdecl CM_AreaEntities(const float *mins, const float *maxs, int *entityList, int maxcount, int contentmask);
void __cdecl CM_AreaEntities_r(unsigned int nodeIndex, areaParms_t *ap);
void __cdecl CM_PointTraceStaticModels(trace_t *results, const float *start, const float *end, int contentmask);
void __cdecl CM_PointTraceStaticModels_r(
        locTraceWork_t *tw,
        unsigned __int16 nodeIndex,
        const float *p1_,
        const float *p2,
        trace_t *trace);
int __cdecl CM_PointTraceStaticModelsComplete(const float *start, const float *end, int contentmask);
int __cdecl CM_PointTraceStaticModelsComplete_r(
        const staticmodeltrace_t *clip,
        unsigned __int16 nodeIndex,
        const float *p1_,
        const float *p2);
void __cdecl CM_ClipMoveToEntities(moveclip_t *clip, trace_t *trace);
void __cdecl CM_ClipMoveToEntities_r(
        const moveclip_t *clip,
        unsigned __int16 nodeIndex,
        const float *p1,
        const float *p2,
        trace_t *trace);
int __cdecl CM_ClipSightTraceToEntities(sightclip_t *clip);
int __cdecl CM_ClipSightTraceToEntities_r(
        const sightclip_t *clip,
        unsigned __int16 nodeIndex,
        const float *p1,
        const float *p2);
void __cdecl CM_PointTraceToEntities(pointtrace_t *clip, trace_t *trace, col_context_t *context);
void __cdecl CM_PointTraceToEntities_r(
        pointtrace_t *clip,
        unsigned __int16 nodeIndex,
        const float *p1,
        const float *p2,
        trace_t *trace,
        col_context_t *context);
int __cdecl CM_PointSightTraceToEntities(sightpointtrace_t *clip);
int __cdecl CM_PointSightTraceToEntities_r(
        const sightpointtrace_t *clip,
        unsigned __int16 nodeIndex,
        const float *p1,
        const float *p2);
