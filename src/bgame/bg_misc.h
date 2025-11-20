#pragma once

void __cdecl BG_RegisterDvars();
char *__cdecl BG_GetEntityTypeName(int eType);
const gitem_s *__cdecl BG_FindItemForWeapon(unsigned int weapon);
const gitem_s *__cdecl G_FindItem(char *pickupName);
bool __cdecl BG_PlayerTouchesItem(const playerState_s *ps, const entityState_s *item, int atTime);
bool __cdecl BG_PlayerCanPickUpWeaponType(const WeaponDef *weapDef, const playerState_s *ps);
bool __cdecl BG_CanItemBeGrabbed(const entityState_s *ent, const playerState_s *ps, int touched);
char __cdecl WeaponEntCanBeGrabbed(
        const entityState_s *weaponEntState,
        const playerState_s *ps,
        int touched,
        unsigned int weapIdx);
bool __cdecl BG_PlayerHasWeapon(const playerState_s *ps, unsigned int weaponIndex);
const PlayerHeldWeapon *__cdecl BG_GetHeldWeaponConst(const playerState_s *ps, unsigned int weaponIndex);
int __cdecl BG_GetHeldWeaponSlot(const playerState_s *ps, unsigned int weaponIndex);
char __cdecl HaveRoomForAmmo(const playerState_s *ps, unsigned int weaponIndex);
bool __cdecl BG_PlayerHasRoomForEntAllAmmoTypes(const entityState_s *ent, const playerState_s *ps);
void __cdecl BG_EvaluateTrajectory(const trajectory_t *tr, int atTime, float *result);
double __cdecl Vec3NormalizeTo(const float *v, float *out);
void __cdecl BG_EvaluateTrajectoryDelta(const trajectory_t *tr, int atTime, float *result);
bool __cdecl BG_ValidateOrigin(const float *pos, char xyBits, char zBits, const float *mapCenter);
bool __cdecl BG_ValidateOriginValue(float val, char bits, float mapCenterValue);
void __cdecl BG_AddPredictableEventToPlayerstate(unsigned int newEvent, unsigned int eventParm, playerState_s *ps);
void __cdecl BG_AddUnpredictableEventToPlayerstate(unsigned int newEvent, unsigned int eventParm, playerState_s *ps);
void __cdecl BG_AddEntityStateEvent(unsigned int newEvent, unsigned int eventParm, entityState_s *es, int eventTime);
void __cdecl BG_PlayerToEntitySetFlags(playerState_s *ps, entityState_s *s);
void __cdecl BG_PlayerToEntitySetPitchAngles(playerState_s *ps, entityState_s *s);
void __cdecl BG_PlayerStateToEntityState(playerState_s *ps, entityState_s *s, int snap, unsigned __int8 handler);
void __cdecl BG_PlayerToEntityProcessEvents(playerState_s *ps, entityState_s *s, unsigned __int8 handler);
void __cdecl BG_PlayerToEntityProcessEvents_Internal(
        entityState_s *s,
        unsigned __int8 handler,
        __int16 *eventSequence,
        __int16 *oldEventSequence,
        int *events,
        unsigned int *eventParms,
        unsigned int eventTime);
char __cdecl BG_PlayerToEntityShouldAddEvent(int event);
void __cdecl BG_PlayerToEntitySetMisc(playerState_s *ps, entityState_s *s);
void __thiscall renderOptions_s::CopyWeaponOptions(renderOptions_s *this, const renderOptions_s *o);
unsigned __int8 __cdecl BG_GetPlayerWeaponModel(const playerState_s *ps, unsigned int weaponIndex);
renderOptions_s __cdecl BG_PlayerWeaponOptions(const playerState_s *ps, unsigned int weaponIndex);
void __cdecl BG_PlayerToEntitySetTrajectory(playerState_s *ps, entityState_s *s, int snap);
// local variable allocation has failed, the output may be wrong!
char  BG_CheckProne@<al>(
        cStaticModel_s *a1@<ebp>,
        const playerState_s *ps,
        int passEntityNum,
        const float *vPos,
        float fSize,
        float fHeight,
        float fYaw,
        float *pfTorsoPitch,
        float *pfWaistPitch,
        bool isAlreadyProne,
        bool isOnGround,
        bool groundIsWalkable,
        unsigned __int8 handler,
        proneCheckType_t proneCheckType,
        float prone_feet_dist);
void __cdecl Phys_Vec3ToNitrousVec(float *inVector, phys_vec3 *outVector);
void __cdecl BG_GetPlayerViewOrigin(const playerState_s *ps, float *origin, int time);
void __cdecl BG_GetPlayerViewDirection(const playerState_s *ps, float *forward, float *right, float *up);
void __cdecl BG_LerpHudColors(const hudelem_s *elem, int time, hudelem_color_t *toColor);
int __cdecl BG_LoadShellShockDvars(const char *name);
void __cdecl BG_SetShellShockParmsFromDvars(shellshock_parms_t *parms);
int __cdecl BG_SaveShellShockDvars(const char *name);
shellshock_parms_t *__cdecl BG_GetShellshockParms(unsigned int index);
bool __cdecl BG_IsShellshockAnim(const playerState_s *ps);
void __cdecl BG_CreateXAnim(XAnim_s *anims, unsigned int animIndex, char *name);
void __cdecl BG_CheckThread();
int __cdecl BG_GetMaxSprintTime(const playerState_s *ps);
bool __cdecl BG_ClipMoveToDobj(const entityState_s *es);
int __cdecl BG_GetFriendlyFireStatus();
const char *__cdecl BG_DisplayName(const clientInfo_t *ci, int type);
void __cdecl BG_ClipCameraToHeliPatch(float *origin, float *oldpos, float *velocity, float (*breachPos)[3]);
const phys_vec3 *__cdecl Phys_Vec3ToNitrousVec(const phys_vec3 *result, float *inVector);
