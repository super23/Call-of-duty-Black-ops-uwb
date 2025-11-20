#pragma once

bool __cdecl ModelIsFullySquashed(unsigned int drawInstIndex);
void __cdecl SetModelIsFullySquashed(unsigned int drawInstIndex);
double __cdecl GetFoliageSquashFromHash(unsigned int drawInstIndex);
void __cdecl R_FoliageSetPerFramePersistantForces(
        PerFrameFoliageInfo *frameInfo,
        const ClientFoliageInfo *clientFoliageInfo);
bool __cdecl GrassPersistForceIsActive(const GrassPersistForce *force);
void __cdecl R_FoliageInitModels(GfxWorld *gfxWorld);
void __cdecl R_MarkDynFoliageModels(GfxWorld *gfxWorld);
void __cdecl R_FoliageShutdown();
void __cdecl R_FoliageSetInitialBurnState();
double __cdecl R_GetWindBestStrength(
        const GrassWindForce *windForces,
        unsigned int numWindForces,
        const float *modelPlacement);
bool __cdecl R_StaticModelNeedsCharredTech(unsigned int modelDrawInstIndex);
void __cdecl R_FoliageNotifyBurn(float *loc, float rad, int *models, int models_count, bool instant);
bool __cdecl AABBOverlapsSphere(
        const float *boxMin,
        const float *boxMax,
        float sphereRadiusSquared,
        const float *sphereCenter);
void __cdecl AddNearMuzzleShakeForce(
        unsigned int id,
        const float *forcePos,
        const float *forwardVector,
        const FoliageShakeParams *params);
void __cdecl SetGrassPersistForceToActive(GrassPersistForce *force);
void __cdecl R_FoliageNotifyGrenadeExplosion(int localClientNum, float *loc);
void __cdecl R_FoliageNofityWeaponFiring(
        int localClientNum,
        unsigned int id,
        const float *loc,
        const float *forwardVector,
        bool isFlameThrower);
void __cdecl R_FoliageSetGrassWindForces(int clientIdx, float *pos, float radius, float strength);
void __cdecl DrawDebugAxis(const float *pos);
void __cdecl AddInstantForce(const GrassForce *force);
void __cdecl R_FoliageNotifyVehiclePosition(int localClientNum, const float *vehiclePos);
void __cdecl R_FoliageSetDefaultShaderConstants(GfxCmdBufSourceState *source);
void __cdecl R_SetCodeConstantFromVec4(GfxCmdBufSourceState *source, unsigned int constant, float *value);
void __cdecl R_DirtyCodeConstant(GfxCmdBufSourceState *source, unsigned int constant);
void __cdecl R_DynSModelInitGfxState(DynSModelGfxState *dynData);
int __cdecl dyn_smodel_drawstateCallback(jqBatch *batch);
void __cdecl R_CalcVisDynSModelDrawState(
        const unsigned __int16 *modelIndexList,
        unsigned int listBegin,
        unsigned int listEnd,
        const PerFrameFoliageInfo *frameInfo,
        DynSModelClientView *view,
        DynSModelGfxState *dynData,
        unsigned int persistForceIndexOffset,
        unsigned int instantForceIndexOffset,
        const ForceBoundsInfo *boundsInfo);
void __cdecl R_DynSModelWaitWorker();
void __cdecl R_DynSModelBuildClientView(
        const DynSModelClient *clientVis,
        DynSModelClientView *view,
        DynSModelGfxState *dynData);
void __cdecl R_FoliageSetStaticModelShaderConstants(
        GfxCmdBufContext *context,
        unsigned int smodelDrawIndex,
        const DynSModelClientView *smodelClientView,
        const DynSModelGfxState *smodelState);
int __cdecl R_UpdateCodeConstant(
        GfxCmdBufSourceState *source,
        unsigned int constant,
        float x,
        float y,
        float z,
        float w);
void __cdecl R_SetCodeConstant(GfxCmdBufSourceState *source, unsigned int constant, float x, float y, float z, float w);
int __cdecl R_UpdateCodeConstantFromVec4(GfxCmdBufSourceState *source, unsigned int constant, float *value);
void __cdecl R_GetGrassParmShaderConstant(float curSquashAmt, float *out);
void __cdecl R_DynSModelGetCounts(unsigned int *numDynSModelInMap, unsigned int *maxNumDynSModel);
void __thiscall ModelHashTable<ActiveModelNode,63,128>::Init(ModelHashTable<ActiveModelNode,63,128> *this);
char __thiscall ModelHashTable<ActiveModelNode,63,128>::Add(
        ModelHashTable<ActiveModelNode,63,128> *this,
        const ActiveModelNode *newNode);
ModelHashTable<ActiveModelNode,63,128> *__thiscall ModelHashTable<ActiveModelNode,63,128>::FindByKey(
        ModelHashTable<ActiveModelNode,63,128> *this,
        unsigned __int16 key);
ClientFoliageInfo *__thiscall ClientFoliageInfo::ClientFoliageInfo(ClientFoliageInfo *this);
