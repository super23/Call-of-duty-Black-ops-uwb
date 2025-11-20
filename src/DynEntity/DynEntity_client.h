#pragma once

void __cdecl DynEntCl_RegisterDvars();
XModel *__cdecl DynEntCl_GetCurrentXModel(const DynEntityDef *dynEntDef, const DynEntityClient *dynEntClient);
bool __cdecl DynEntCl_IsHealthGone(const DynEntityDef *dynEntDef, const DynEntityClient *dynEntClient);
void __cdecl DynEntCl_LinkModel(unsigned __int16 dynEntId);
const DynEntityDef *__cdecl DynEnt_GetEntityDef(unsigned __int16 dynEntId, DynEntityDrawType drawType);
DynEntityClient *__cdecl DynEnt_GetClientEntity(unsigned __int16 dynEntId, DynEntityDrawType drawType);
void __cdecl DynEntCl_LinkBrush(unsigned __int16 dynEntId);
double __cdecl DynEntCl_UpdateBModelWorldBounds(const DynEntityDef *dynEntDef, const GfxPlacement *pose);
void __cdecl DynEntCl_UnlinkEntity(unsigned __int16 dynEntId, DynEntityCollType drawType);
void __cdecl DynEnt_DestroyPhysics(PhysWorld worldIndex, const DynEntityDef *dynEntDef, DynEntityClient *dynEntClient);
void __cdecl DynEntCl_Shutdown(int localClientNum);
void __cdecl DynEntCl_InitEntities(int localClientNum);
DynEntityColl *__cdecl DynEnt_GetEntityColl(DynEntityCollType collType, unsigned __int16 dynEntId);
void __cdecl DynEntCl_DestroyEntityModel(unsigned __int16 id);
void __cdecl DynEnt_FixupLightConstraints(int localClientNum, int entnum, int primaryLightIndex);
void __cdecl MatrixTransposeTransformVector(const float *in1, const float (*in2)[3], float *out);
void __cdecl DynEntCl_ProcessEntities(int localClientNum, int time);
void __cdecl DynEnt_UpdateLightConstraint(
        int localClientNum,
        const DynEntityDef *dynEntDef,
        const DynEntityPose *dynEntPose);
void __cdecl DynEnt_UpdateConstraints(int localClientNum, DynEntityClient *dynEntClient, const DynEntityDef *dynEntDef);
int __cdecl DynEnt_GetDamage(DynEntityClient *dynEntClient, const DynEntityDef *dynEntDef);
// local variable allocation has failed, the output may be wrong!
void  DynEnt_SetupConstraints(PhysConstraint *a1@<ebp>, const DynEntityDef *dynEntDef);
int __cdecl DynEnt_GetSurfaceType(const DynEntityDef *dynEntDef);
void __cdecl DynEnt_CalcPhysPreset(const DynEntityDef *dynEntDef);
int __cdecl DynEntCl_CreatePhysObj(
        const DynEntityDef *dynEntDef,
        DynEntityClient *dynEntClient,
        const GfxPlacement *pose);
broad_phase_info *__thiscall broad_phase_base::get_bpi(broad_phase_base *this);
broad_phase_group *__thiscall broad_phase_base::get_bpg(broad_phase_base *this);
void *__thiscall create_gjk_geom_collision_visitor::allocate(
        create_gjk_geom_collision_visitor *this,
        int size,
        int alignment,
        bool no_error);
char __thiscall create_gjk_geom_collision_visitor::query_create_prolog(gjk_collision_visitor *this, const void *geom);
void __thiscall gjk_geom_list_t::add_geom(gjk_geom_list_t *this, gjk_base_t *geom);
void __thiscall create_gjk_geom_collision_visitor::query_create_epilog_1(
        create_gjk_geom_collision_visitor *this,
        gjk_base_t *gjk_geom);
unsigned __int16 __cdecl DynEntCl_CreateEntityModel(
        XModel *model,
        const float *quat,
        const float *origin,
        const float *hitPos,
        const float *hitDir,
        int mod,
        PhysPreset *preset);
unsigned __int16 __cdecl DynEntCl_AddEntityModel();
void DynEntCl_CleanUpOldModels();
void __cdecl DynEntCl_PointTrace(const pointtrace_t *clip, trace_t *results);
void __cdecl DynEntCl_PointTrace_r(
        DynEntityCollType drawType,
        const pointtrace_t *clip,
        unsigned int sectorIndex,
        float *p1,
        float *p2,
        trace_t *results);
void __cdecl DynEntCl_AreaEntities_r(
        DynEntityCollType drawType,
        unsigned int sectorIndex,
        DynEntityAreaParms *areaParms);
unsigned __int16 __cdecl DynEntCl_AreaEntities(
        DynEntityDrawType drawType,
        const float *mins,
        const float *maxs,
        int contentMask,
        unsigned __int16 dynEntMaxCount,
        unsigned __int16 *dynEntList);
void __cdecl DynEntCl_TurnOffLight(const DynEntityDef *dynEntDef);
void __cdecl DynEntCl_Damage(
        int localClientNum,
        unsigned __int16 dynEntId,
        DynEntityCollType drawType,
        const float *hitPos,
        const float *hitDir,
        int damage);
void __cdecl DynEntCl_PlayEventFx(const FxEffectDef *def, const float *origin, const float (*axis)[3]);
unsigned int __cdecl DynEntCl_PlayBoltedFX(const FxEffectDef *fx, unsigned __int16 absDynEntId);
void __cdecl DynEntCl_RemoveFromFadeList(unsigned __int16 absDynEntId);
void __cdecl DynEntCl_SetFadeOut(unsigned __int16 dynEntId, DynEntityDrawType drawType);
void __cdecl DynEnt_UpdateFading(int currentTime);
double __cdecl DynEntCl_GetFadeTime(unsigned __int16 dynEntId, DynEntityDrawType drawType);
void __cdecl DynEntCl_SetBurning(unsigned __int16 dynEntId, DynEntityDrawType drawType, bool burning);
void DynEnt_ResetFading();
void DynEnt_ResetBurning();
void DynEnt_UpdateBurning();
void __cdecl DynEntCl_FlameDamage(
        int localClientNum,
        unsigned __int16 dynEntId,
        DynEntityCollType drawType,
        const float *hitPos,
        const float *hitVel,
        int damage);
void __cdecl DynEntCl_SetBurnFrac(unsigned __int16 dynEntId, DynEntityDrawType drawType, float frac);
double __cdecl DynEntCl_GetBurnTime(unsigned __int16 dynEntId, DynEntityDrawType drawType);
void __cdecl DynEntCl_MeleeEvent(int localClientNum, int attackerEntNum);
void __cdecl DynEntCl_EntityImpactEvent(
        const trace_t *trace,
        int localClientNum,
        int sourceEntityNum,
        const float *start,
        float *hitPos,
        bool isMelee);
void __cdecl DynEntCl_PlayImpactEffects(
        int localClientNum,
        int sourceEntityNum,
        unsigned int surfType,
        float *hitPos,
        const float *hitNormal);
char __cdecl DynEntCl_EventNeedsProcessed(int localClientNum, int sourceEntityNum);
char __cdecl DynEntCl_DynEntImpactEvent(
        int localClientNum,
        int sourceEntityNum,
        const float *start,
        const float *end,
        int damage,
        bool isMelee);
void __cdecl DynEntCl_ExplosionEvent(
        int localClientNum,
        bool isCylinder,
        float *origin,
        float innerRadius,
        float outerRadius,
        float *impulse,
        float inScale,
        int innerDamage,
        int outerDamage);
unsigned int __cdecl DynEntCl_GetClosestEntities(
        DynEntityCollType drawType,
        float *radiusMins,
        float *radiusMaxs,
        float *origin,
        unsigned __int16 *hitEnts,
        bool isCylinder);
bool __cdecl DynEntCl_CompareDynEntsForExplosion(const DynEntSortStruct *ent1, const DynEntSortStruct *ent2);
void __cdecl DynEntCl_DestroyEvent(
        int localClientNum,
        unsigned __int16 dynEntId,
        DynEntityCollType drawType,
        const float *hitPos,
        const float *hitDir);
void __cdecl CreateRopes(int localClientNum);
void __cdecl MapHitLocationToRagdollBoneName(hitLocation_t hitLoc, unsigned int *boneName);
const DynEntityDef *__cdecl DynEnt_GetEntityDef(unsigned __int16 absDynEntId);
DynEntityClient *__cdecl DynEnt_GetClientEntity(unsigned __int16 id);
