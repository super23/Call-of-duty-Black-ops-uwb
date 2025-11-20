#pragma once

DynEntityCollSector *__cdecl DynEnt_GetCollSector(DynEntityCollType collType, unsigned int sectorIndex);
DynEntityCollWorld *__cdecl DynEnt_GetCollWorld(DynEntityCollType collType);
void __cdecl DynEnt_ClearCollWorld(DynEntityCollType collType);
void __cdecl DynEnt_UnlinkEntity(DynEntityCollType collType, unsigned __int16 dynEntId);
void __cdecl DynEnt_LinkEntity(
        DynEntityCollType collType,
        unsigned __int16 dynEntId,
        const float *absMins,
        const float *absMaxs);
void __cdecl DynEnt_AddToCollSector(
        DynEntityCollType collType,
        unsigned __int16 dynEntId,
        unsigned __int16 sectorIndex);
void __cdecl DynEnt_SortCollSector(
        DynEntityCollType collType,
        unsigned __int16 sectorIndex,
        const float *mins,
        const float *maxs);
unsigned __int16 __cdecl DynEnt_AllocCollSector(DynEntityCollType collType, const float *mins, const float *maxs);
int __cdecl DynEnt_GetContents(const DynEntityDef *dynEntDef);
void __cdecl DynEnt_GetLocalBounds(const DynEntityDef *dynEntDef, float *mins, float *maxs);
double __cdecl DynEnt_GetRadiusDistSqr(const DynEntityColl *dynEntColl, const float *origin);
bool __cdecl DynEnt_IsInCone(
        const DynEntityDef *dynEntDef,
        const GfxPlacement *pose,
        const float *origin,
        float coneAngleCos,
        const float *coneDir);
double __cdecl DynEnt_GetCylindricalRadiusDistSqr(const DynEntityColl *dynEntColl, const float *origin);
bool __cdecl DynEnt_EntityInArea(
        const DynEntityDef *dynEntDef,
        const DynEntityColl *dynEntColl,
        const float *mins,
        const float *maxs,
        int contentMask);
void __cdecl DynEnt_PointTraceToModel(
        const DynEntityDef *dynEntDef,
        const GfxPlacement *pose,
        const pointtrace_t *clip,
        trace_t *results);
void __cdecl DynEnt_PointTraceToBrush(
        const DynEntityDef *dynEntDef,
        const GfxPlacement *pose,
        const pointtrace_t *clip,
        trace_t *results);
