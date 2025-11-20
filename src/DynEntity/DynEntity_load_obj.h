#pragma once

const DynEntityDef *__cdecl DynEnt_GetEntityDef(unsigned __int16 dynEntId, DynEntityDrawType drawType);
DynEntityClient *__cdecl DynEnt_GetClientEntity(unsigned __int16 dynEntId, DynEntityDrawType drawType);
PhysPreset *__cdecl DynEnt_PhysPresetPrecache(const char *name);
unsigned __int8 *__cdecl DynEnt_AllocPhysPreset(int size);
XModelPieces *__cdecl DynEnt_XModelPiecesPrecache(const char *name);
unsigned __int8 *__cdecl DynEnt_AllocXModelPieces(int size);
void __cdecl DynEnt_LoadEntities();
char __cdecl DynEnt_IsValidClassName(const char *className);
void __cdecl DynEnt_GetEntityCountFromString(const char *entityString, int *entCount, int *constraintCount);
char __cdecl DynEnt_IsValidConstraintName(const char *className);
parseInfo_t *LoadRopeParams();
void __cdecl DynEnt_GetEntityConstraintParams(
        DynEntityConstraintCreateParams *constraintParams,
        PhysConstraint *constraints,
        int dynEntConstraintCount);
char __cdecl DynEnt_CreateConstraint(PhysConstraint *constraint, DynEntityConstraintCreateParams *params);
bool __cdecl FindConstraints(
        const char *targetname,
        const float *origin,
        const float *angles,
        AttachPointType attach_type,
        DynEntityConstraintCreateParams *constraintParams,
        int numConstraints,
        int *outConstraints);
char __cdecl DynEnt_Create(
        DynEntityDef *dynEntDef,
        const DynEntityCreateParams *params,
        DynEntityConstraintCreateParams *constraintParams,
        int numConstraints);
int __cdecl DynEnt_GetType(const char *typeName);
XModel *__cdecl DynEnt_XModelPrecache(char *modelName);
unsigned __int8 *__cdecl DynEnt_AllocXModel(int size);
unsigned __int8 *__cdecl DynEnt_AllocXModelColl(int size);
PhysPreset *__cdecl DynEnt_AutoCreatePhysPreset(const DynEntityDef *dynEntDef, const DynEntityCreateParams *params);
void __cdecl DynEnt_Strcpy(char **memberString, const char *keyValue);
int __cdecl DynEnt_CompareEntities(unsigned int *arg0, unsigned int *arg1);
unsigned __int8 *__cdecl DynEnt_Alloc(int count, int size);
void __cdecl GetAttachPointsInWorldSpace(PhysConstraint *constraint, float *p0, float *p1);
const DynEntityProps *__cdecl DynEnt_GetEntityProps(DynEntityType dynEntType);
unsigned __int16 __cdecl DynEnt_GetId(const DynEntityDef *dynEntDef, DynEntityDrawType drawType);
unsigned __int16 __cdecl DynEnt_GetAbsId(const DynEntityDef *dynEntDef);
bool __cdecl DynEnt_Valid(unsigned __int16 absDynEntId);
unsigned __int16 __cdecl DynEnt_GetEntityCount(DynEntityCollType collType);
const DynEntityDef *__cdecl DynEnt_GetEntityDef(unsigned __int16 absDynEntId);
const DynEntityPose *__cdecl DynEnt_GetClientPose(unsigned __int16 absDynEntId);
DynEntityPose *__cdecl DynEnt_GetClientModelPoseList();
DynEntityPose *__cdecl DynEnt_GetClientPose(unsigned __int16 dynEntId, DynEntityDrawType drawType);
DynEntityServer *__cdecl DynEnt_GetServerEntity(unsigned __int16 dynEntId, DynEntityDrawType drawType);
void __cdecl DynEnt_GetClientIdDrawType(unsigned __int16 absId, unsigned __int16 *outId, DynEntityDrawType *drawType);
DynEntityClient *__cdecl DynEnt_GetClientEntity(unsigned __int16 id);
unsigned __int16 __cdecl DynEnt_GetClientAbsId(unsigned __int16 dynEntId, DynEntityDrawType drawType);
int __cdecl DynEnt_GetXModelUsageCount(const XModel *xModel);
