#pragma once

void __cdecl Turret_PlaceTurret_RegisterDvars();
int __cdecl Turret_PlaceTurret_Internal(
        turret_handler handler,
        int clientNum,
        unsigned int moverEntNum,
        const float *moverOrigin,
        const float *viewOrigin,
        const float *viewAngles,
        float *outOrigin,
        float *outAngles);
void __cdecl Turret_PlaceTurret_Trace(
        turret_handler handler,
        trace_t *results,
        const float *start,
        const float *mins,
        const float *maxs,
        const float *end,
        int passEntityNum,
        int contentmask,
        col_context_t *context);
bool __cdecl Turret_PlaceTurret_UpdateFooting(
        turret_handler handler,
        int moverEntNum,
        trace_t *trace,
        float *outOrigin,
        float (*outAxis)[3]);
int __cdecl Turret_PlaceTurret_Client(int localClientNum, const entityState_s *es, float *outOrigin, float *outAngles);
int __cdecl Turret_PlaceTurret_Server(playerState_s *ps, float *outOrigin, float *outAngles);
