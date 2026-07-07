#pragma once
#include <clientscript/cscr_variable.h>
#include <game/teams.h>
#include "cg_ents_sp.h"

struct cscr_mp_data_t // sizeof=0x14
{                                       // XREF: .data:cscr_mp_data_t cg_scr_sp_data/r
    int dogstep;                        // XREF: CScr_SetUniqueClientScripts(ScriptFunctions *)+225/w
    int airsupport;                     // XREF: CScr_SetUniqueClientScripts(ScriptFunctions *)+27C/w
    int demo_jump;                      // XREF: CScr_SetUniqueClientScripts(ScriptFunctions *)+299/w
    int demo_playerSwitch;              // XREF: CScr_SetUniqueClientScripts(ScriptFunctions *)+2B6/w
    int playAIFootstep;                 // XREF: cg_scr_main_coop.obj CScr_PlayAIFootstepSound
};

struct cached_tag_mat_t;
struct cent_field_s;
struct centity_s;

void __cdecl CScrCmd_GetLocalClientNumber(scr_entref_t entref);
void __cdecl CScrCmd_Earthquake(scr_entref_t entref);
unsigned int __cdecl CScr_SpawnFXInternal(int localClientNum, int fxId, float (*axis)[3], float *pos, int time);
void CScr_DeleteFX();
void CScr_TriggerFX();
void CScr_SpawnFX();
void CScr_PlayFXOnTag();
void CScr_PlayViewmodelFX();
void __cdecl CScr_IsDemoPlaying();
void __cdecl CScr_IsSpectating();
void __cdecl CScrCmd_IsBurning(scr_entref_t entref);
void __cdecl CPlayerCmd_HasPerk(scr_entref_t entref);
void __cdecl CScr_GetVehicleHealth(scr_entref_t entref);
void __cdecl CScr_GetLeftTreadHealth(scr_entref_t entref);
void __cdecl CScr_GetHeliDamageState(scr_entref_t entref);
void __cdecl CScr_GetRightTreadHealth(scr_entref_t entref);
void __cdecl CScr_GetInKillcam(scr_entref_t entref);
void __cdecl CScr_GetAnimState(scr_entref_t entref);
void __cdecl CScr_GetAnimStateCategory(scr_entref_t entref);
void CScr_GetTotalAmmo();
void CScr_GetCurrentWeapon();
void CScr_GetCurrentWeaponIncludingMelee();
void CScr_HasWeapon();
void CScr_SetLocalRadarEnabled();
void CScr_SetLocalRadarPosition();
void CScr_SetExtraCamEntity();
void CScr_SetExtraCamActive();
void CScr_GetExtraCamStatic();
void CScr_SetExtraCamStatic();
void CScr_SetExtraCamOrigin();
void CScr_SetExtraCamAngles();
void CScr_IsCameraSpikeToggled();
// LWSS ADD
void CScr_SetClientVolumetricFog();
void CScr_SwitchToServerVolumetricFog();
void CScr_SwitchToClientVolumetricFog();
void CScr_IsInHelicopter();
void CScr_IsADS();
void CScr_IsOnTurret();
void CScr_IsThrowingGrenade();
void CScr_IsMeleeing();
void CScr_GetWeaponAmmoClip();
void CScr_CreateDynEntAndLaunch();
void __cdecl CG_DoPlayWeaponDeathEffects(unsigned int localClientNum, int entnum, unsigned int weaponIndex, int mod);
void __cdecl CG_DoPlayWeaponDamageEffects(unsigned int localClientNum, int entnum, unsigned int weaponIndex, int mod);
// LWSS END
void CScr_GetGridFromPos();
void CScr_CompassScale();
void CScr_ResetCompassScale();
void __cdecl CScr_GetLocalPlayerTeam();
void __cdecl CScr_AddTeamName(team_t team);
void (__cdecl *__cdecl CScr_GetFunctionProjectSpecific(const char **pName, int *type))();
void __cdecl CScrCmd_GetOwner(scr_entref_t entref);
void __cdecl CScr_GetTagOrigin(scr_entref_t entref);
int __cdecl CScr_UpdateTagInternal(centity_s *ent, unsigned int tagName, cached_tag_mat_t *cachedTag);
void __cdecl CScr_GetTagAngles(scr_entref_t entref);
void __cdecl CScr_GetTagForwardVector(scr_entref_t entref);
void __cdecl CScrCmd_ShellShock(scr_entref_t entref);
void __cdecl CScr_SetEnemyGlobalScrambler(scr_entref_t entref);
void __cdecl CScr_SetEnemyScramblerAmount(scr_entref_t entref);
void __cdecl CScr_SetFriendlyScramblerAmount(scr_entref_t entref);
void __cdecl CScr_GetFriendlyScramblerAmount(scr_entref_t entref);
void __cdecl CScr_GetEnemyScramblerAmount(scr_entref_t entref);
void __cdecl CScr_IsScrambled(scr_entref_t entref);
void __cdecl CScr_SetNearestEnemyScrambler(scr_entref_t entref);
void __cdecl CScr_ClearNearestEnemyScrambler(scr_entref_t entref);
void __cdecl CScr_AddFriendlyScrambler(scr_entref_t entref);
void __cdecl CScr_RemoveFriendlyScrambler(scr_entref_t entref);
void __cdecl CScr_RemoveAllFriendlyScramblers(scr_entref_t entref);
void __cdecl CScr_HasTacticalMaskOverlay(scr_entref_t entref);
void __cdecl CScr_GetStance(scr_entref_t entref);
void __cdecl CScr_SetFlagAsAway(scr_entref_t entref);
void __cdecl CScr_GetParentEntity(scr_entref_t entref);
void(__cdecl *__cdecl CScr_GetMethodProjectSpecific(const char **pName, int *type))(scr_entref_t);
void __cdecl CScr_SetUniqueClientScripts(ScriptFunctions *functions);
void __cdecl CScr_AddFieldsForDynEntity();
void __cdecl CScr_UpdateGamerProfile();
void __cdecl CScr_UpdateDvarsFromProfile();
void __cdecl CScr_UseAlternateAimParams();
void __cdecl CScr_ClearAlternateAimParams();
void __cdecl CG_SendSwimNotify(int localClientNum, unsigned int clientNum, int start);
void __cdecl CScr_GetEntityByIndex(centity_s *cent, const cent_field_s *pField);
int __cdecl GetField(const int *i, int size);
void __cdecl CScr_GetTeamName(centity_s *cent, const cent_field_s *pField);
team_t __cdecl GetTeam(centity_s *cent);
unsigned __int16 __cdecl CScr_GetFootTag(eFoot foot);
void __cdecl CScr_PlayDogstepSound(int localClientNum, centity_s *cent, eFoot foot);
void __cdecl CScr_PlayAIFootstepSound(int localClientNum, centity_s *cent, int isLeftFoot, bool isSpeciesDog);

void __cdecl CScrCmd_PlayRumbleOnEntity(scr_entref_t entref);
void __cdecl CScrCmd_PlayRumbleLoopOnEntity(scr_entref_t entref);
void __cdecl CScrCmd_StopRumble(scr_entref_t entref);
void __cdecl CScr_SetWatchStyle(scr_entref_t entref);
void __cdecl CScr_PlayRumbleOnPosition();
void __cdecl CScrCmd_FireWeapon(scr_entref_t entref);

extern cscr_mp_data_t cg_scr_sp_data;