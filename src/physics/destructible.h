#pragma once

bool __cdecl hasLabel(DestructibleDef *ddef, unsigned __int16 label);
int __cdecl Destructible_GetPieceIndexForLabel(DestructibleDef *ddef, unsigned __int16 enableLabel);
int __cdecl Destructible_GetPieceMaxHealth(DestructibleDef *ddef, int pieceIndex);
int __cdecl Destructible_GetPieceHealth(Destructible *destructible, int pieceIndex);
void __cdecl EnableAllDestructiblePieces();
void __cdecl DisableDestructiblePiece(int label);
char __cdecl IsDestructiblePieceDisabled(int label);
void __cdecl CG_EnableAllDestructiblePieces();
void __cdecl CG_DisableDestructiblePiece(int localClientNum, int label);
char __cdecl CG_IsDestructiblePieceDisabled(int label);
int __cdecl CG_GetDestructibleIndex(int localClientNum, const Destructible *destructible);
Destructible *__cdecl CG_GetDestructibleByIndex(int localClientNum, int index);
Destructible *__cdecl Destructible_GetFreeDestructible();
Destructible *__cdecl Destructible_GetDestructible(int entNum);
void __cdecl Destructible_FreeDestructible(Destructible *destructible);
Destructible *__cdecl CG_GetFreeDestructible(int localClientNum, int entNum);
void __cdecl CG_FreeDestructible(int localClientNum, Destructible *destructible);
void __cdecl CG_InitDestructibles(int localClientNum);
void __cdecl G_InitDestructibles();
int __cdecl Destructible_GetNumStagesForPiece(DestructibleDef *ddef, int pieceIndex);
int __cdecl Destructible_GetShowBoneNameForPiece(Destructible *destructible, int pieceIndex);
int __cdecl Destructible_GetDestructibleStage(const DestructibleDef *ddef, int pieceIndex, int health);
void __cdecl Destructible_GetHideParts(Destructible *destructible, unsigned int *partBits);
int __cdecl Destructible_GetPieceParentBone(DestructibleDef *ddef, int pieceIndex);
PhysPreset *__cdecl Destructible_GetPiecePhysPreset(DestructibleDef *ddef, int pieceIndex);
unsigned int __cdecl DestructibleUpdate(gentity_s *ent, DObjModel_s *dobjModels, unsigned int numModels);
char __cdecl Destructible_GetPieceIndexFromBoneName(
        Destructible *destructible,
        int modelIndex,
        unsigned int boneName,
        unsigned __int8 *index);
int __cdecl CG_Destructible_GetModelIndexFromLabel();
int __cdecl GetHealthFromState(
        const LerpEntityStateDestructibleHit *state,
        DestructibleDef *ddef,
        unsigned __int8 pieceIndex);
void __cdecl DestructibleEventSetPieceState(gentity_s *self, LerpEntityStateDestructibleHit *state);
void __cdecl DestructibleExplosiveDamageEvent(
        gentity_s *self,
        const float *point,
        float radius,
        entityState_s::<unnamed_type_un1> mod);
void __cdecl DestructibleBulletDamageEvent(
        gentity_s *self,
        const float *point,
        const float *dir,
        entityState_s::<unnamed_type_un1> mod);
bool __cdecl DamagePiece(
        gentity_s *self,
        unsigned __int8 index,
        int damage,
        const float *point,
        const float *dir,
        int mod,
        bool damageChildren,
        int excludeChild,
        gentity_s *attacker,
        int recursiveDepth);
bool __cdecl Destructible_HasParent(const DestructibleDef *ddef, unsigned __int8 index);
double __cdecl DestructibleRadiusDamage(
        gentity_s *self,
        const float *point,
        float damgeInner,
        float damgeOuter,
        float radius,
        int mod,
        gentity_s *attacker);
int __cdecl DestructibleDamage(
        gentity_s *self,
        gentity_s *attacker,
        const float *dir,
        const float *point,
        int damage,
        int mod,
        unsigned int modelIndex,
        unsigned int partName);
void __cdecl CG_DestructibleClientOnlyCreate(centity_s *ent, DestructibleDef *ddef);
unsigned int __cdecl CG_DestructibleUpdate(
        centity_s *ent,
        DObjModel_s *dobjModels,
        unsigned int numModels,
        int localClientNum);
destructible_gamestate *__cdecl Destructible_FindGameState(int entityNum, int localClientNum);
char __cdecl Destructible_RemoveGameState(int entityNum, int localClientNum);
int __cdecl CG_SpawnPhysObjForBone(
        const centity_s *cent,
        int pieceIndex,
        unsigned int boneName,
        const float *hitp,
        const float *hitd,
        int mod);
unsigned __int16 __cdecl CG_DestructibleSpawnDynEnt(
        const centity_s *cent,
        XModel *model,
        float *origin,
        float (*mat)[3],
        const float *hitp,
        const float *hitd,
        int mod);
int __cdecl G_GetTagTransform(gentity_s *ent, DestructibleDef *ddef, int pieceIndex, float (*tagMat)[3]);
void __cdecl CG_DestructibleBreakPiece(
        centity_s *self,
        DestructiblePiece *piece,
        int pieceIndex,
        int stage,
        int nextStage,
        const float *hitp,
        const float *hitd,
        int mod);
unsigned int __cdecl CG_GetTagTransform(
        centity_s *self,
        DestructibleDef *ddef,
        int pieceIndex,
        float *tagOrigin,
        float (*tagMat)[3]);
void __cdecl CG_DestructibleSetBurn(
        int localClientNum,
        centity_s *self,
        Destructible *destructible,
        int pieceIndex,
        bool burn);
void __cdecl CG_DestructibleUpdateBurn(int localClientNum, Destructible *destructible);
void __cdecl CG_DestructibleUpdateXDoll(int localClientNum, Destructible *destructible);
double __cdecl CG_DestructibleGetBurnFrac(Destructible *destructible);
bool __cdecl CG_DestructibleIsClientOnly(Destructible *destructible);
void __cdecl CG_UpdateDestructibles(int localClientNum, int msec);
void __cdecl CG_DestructibleUpdateDObj(centity_s *self);
void __cdecl CG_DestructibleRadiusDamage(
        centity_s *self,
        const centity_s **point,
        float damgeInner,
        float damgeOuter,
        float radius,
        int mod,
        int lazyprocess);
void Destructible_DumpEventInfo();
char __cdecl CG_DamagePiece(
        int localClientNum,
        centity_s *self,
        unsigned __int8 index,
        int damage,
        const float *point,
        const float *dir,
        int mod,
        bool damageChildren,
        int recursiveDepth);
void __cdecl CG_NotifyDestructibleScripts(int localClientNum, centity_s *self, DestructibleStage *prev_stage);
void __cdecl CG_StoreDestructibleState(centity_s *cent, DestructibleState newState);
void __cdecl CG_RestartDestructibles(int localClientNum);
void __cdecl CG_DestructibleRewindToTime(int localClientNum, Destructible *destructible, int time);
void __cdecl CG_DestructibleHitEvent(int localClientNum, int event, const entityState_s *es, int lazyprocess);
void __cdecl CG_ProcessDestructibleEvents();
void __cdecl CG_DestructibleDamage(
        centity_s *self,
        const centity_s *attacker,
        const float *dir,
        const float *point,
        int damage,
        int mod,
        unsigned int modelIndex,
        unsigned int partName,
        int lazyprocess);
bool __cdecl CG_DestructibleDamage(
        centity_s *self,
        const float *dir,
        const float *point,
        signed int pieceIndex,
        int newHealth,
        int mod,
        bool updateDObj,
        bool applyForce);
unsigned __int8 *__cdecl CM_Hunk_AllocDestructible(unsigned int size);
DestructibleDef *__cdecl DestructibleDefPrecache(const char *name);
DestructibleDef *__cdecl DestructibleDefPrecache_LoadObj(const char *name);
DestructibleDef *__cdecl DestructibleDefPrecache_FastFile(const char *name);
int __cdecl Destructible_GetPieceCount(Destructible *obj);
unsigned int __cdecl Destructible_XDollActive(Destructible *obj);
int __cdecl Destructible_GetXDollHandle(Destructible *obj, int piece_index);
environment_rigid_body *__cdecl Destructible_GetRigidBody();
char __cdecl Destructible_NeedsGameStateUpdate(const Destructible *d);
void __cdecl Destructible_WriteGameState(msg_t *msg);
void __cdecl Destructible_ReadGameState(msg_t *msg, int localClientNum);
void __cdecl Demo_Destructible_WriteGameState(msg_t *msg, int localClientNum);
void __cdecl Demo_Destructible_ReadGameState(msg_t *msg, int localClientNum);
unsigned __int8 __cdecl CG_GetFreeDestructiblePose();
void __cdecl CG_FreeDestructiblePose(int localClientNum, unsigned __int8 destructiblePoseID);
DestructiblePose *__cdecl CG_GetDestructiblePose(__int16 index);
void __cdecl CG_DestructibleShake(
        int localClientNum,
        centity_s *cent,
        int pieceIndex,
        const float *dir,
        const float *hitp,
        int mod);
void __cdecl Vec3RotateTranspose(const float *in, const float (*matrix)[3], float *out);
void __cdecl CG_DestructibleUpdatePoses(int localClientNum, int msec);
void __cdecl Destructible_SetDebugRender(Destructible *destructible);
void __cdecl Destructible_SetDebugDamagePiece(int piece, int damage);
int __cdecl Destructible_IsDebugDamagePiece(int piece);
void __cdecl Destructible_DebugRender();
DestructibleDef *__cdecl Destructible_GetDDef(const centity_s *cent);
DestructibleDef *__cdecl Destructible_GetDDef(const Destructible *const obj);
void __cdecl Scr_DestructibleCallback(gentity_s *self, unsigned __int16 event, int piece, float time, int damage);
void __cdecl Scr_DestructibleCallback(gentity_s *self, unsigned __int16 event, char *notify, gentity_s *attacker);
void __cdecl CScr_DestructibleCallback(centity_s *self, unsigned __int16 event, int piece, float time, int damage);
Destructible_BonePose *__thiscall Destructible_BonePose::Destructible_BonePose(Destructible_BonePose *this);
