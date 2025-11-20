#pragma once

unsigned __int8 *__cdecl CM_Hunk_AllocXModel(unsigned int size);
XModel *__cdecl CM_XModelPrecache(char *name);
unsigned __int8 *__cdecl CM_Hunk_AllocXModelColl(unsigned int size);
void __cdecl CM_TraceStaticModel(
        cStaticModel_s *sm,
        trace_t *results,
        const float *start,
        const float *end,
        int contentmask);
bool __cdecl CM_TraceStaticModelComplete(cStaticModel_s *sm, const float *start, const float *end, int contentmask);
