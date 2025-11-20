#pragma once

void __cdecl DynEntPieces_RegisterDvars();
void __cdecl DynEntPieces_AddDrawSurfs();
char __cdecl DynEntPieces_SpawnPhysicsModel(
        int localClientNum,
        const XModel *model,
        const float *offset,
        const float *origin,
        const float (*axis)[3],
        const float *hitPos,
        const float *hitDir,
        PhysPreset *physPreset);
PhysObjUserData *__cdecl DynEntPieces_SpawnPhysObj(
        const char *modelName,
        int stype,
        const float *mins,
        const float *maxs,
        const float *position,
        const float *quat,
        const float *velocity,
        float *angularVelocity,
        const PhysPreset *physPreset);
void __cdecl DynEntPieces_CalcForceDir(const float *hitDir, float spreadFraction, float *forceDir);
void __cdecl DynEntPieces_SpawnPieces(
        int localClientNum,
        const XModelPieces *pieces,
        const float *origin,
        const float (*axis)[3],
        const float *hitPos,
        const float *hitDir);
