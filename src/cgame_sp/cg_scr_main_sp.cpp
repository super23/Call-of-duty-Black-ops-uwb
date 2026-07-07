#include "cg_scr_main_sp.h"
#include <clientscript/cscr_variable.h>
#include <clientscript/cscr_vm.h>
#include <cgame/cg_camerashake.h>
#include <EffectsCore/fx_system.h>
#include <cgame/cg_scr_main.h>
#include <qcommon/dobj_management.h>
#include <clientscript/cscr_stringlist.h>
#include <cgame/cg_event.h>
#include <demo/demo_playback.h>
#include "cg_main_sp.h"
#include "cg_ents_sp.h"
#include <bgame/bg_perks.h>
#include "cg_vehicles_sp.h"
#include <bgame/bg_dog.h>
#include <bgame/bg_mantle.h>
#include <bgame/bg_weapons.h>
#include <bgame/bg_weapons_ammo.h>
#include <client/splitscreen.h>
#include <cgame/cg_compass.h>
#include <xanim/dobj_utils.h>
#include <xanim/xmodel.h>
#include <client_sp/cl_cgame_sp.h>
#include <clientscript/scr_const.h>
#include "cg_animscripted_sp.h"
#include <universal/surfaceflags.h>
#include <gfx_d3d/r_fog.h>
#include "cg_servercmds_sp.h"
#include <bgame/bg_weapons_def.h>
#include <DynEntity/DynEntity_client.h>
#include <cgame/cg_spawn.h>
#include <qcommon/common.h>
#include <qcommon/cm_trace.h>
#include <stringed/stringed_hooks.h>
#include <universal/com_math_anglevectors.h>
#include "cg_draw_sp.h"
#include <gfx_d3d/r_shader_constant_set.h>
#include <win32/win_gamerprofile.h>
#include <qcommon/cmd.h>
#include <qcommon/com_clients.h>
#include <client_sp/cl_input_sp.h>
#include <cgame/cg_weapons.h>
#include <client/cl_main.h>
#include "cg_predict_sp.h"
#include <DynEntity/DynEntity_load_obj.h>
#include <win32/win_gamerprofile.h>
#include <cstddef>
#include <bgame/bg_collectibles_sp.h>
#include <bgame/bg_local.h>
#include <physics/destructible.h>
#include "cg_view_sp.h"
#include "cg_scoreboard_sp.h"
#include <gfx_d3d/r_dvars.h>
#include <bgame/bg_weapons_def.h>
#include <clientscript/cscr_main.h>
#include <gfx_d3d/r_scene.h>
#include <gfx_d3d/r_water_sim.h>
#include <live/live_win.h>
#include <qcommon/common.h>
#include <cstring>
#include "cg_vehicles_sp.h"

struct ClientVolFogBackup
{
    int valid;
    float start;
    float r;
    float g;
    float b;
    float density;
    float heightDensity;
    float baseHeight;
    float fogColorScale;
    float sunColR;
    float sunColG;
    float sunColB;
    float sunDirX;
    float sunDirY;
    float sunDirZ;
    float sunStartAng;
    float sunEndAng;
    float maxFogOpacity;
    int transitionMsec;
};

static ClientVolFogBackup s_clientVolFog;
static bool s_useAlternateAimParams[MAX_LOCAL_CLIENTS];

enum DynEntFieldEntityPart : __int32
{
    DYNENT_FIELD_CLIENT = 0,
    DYNENT_FIELD_POSE = 1,
    DYNENT_FIELD_DEF = 2,
};

struct dynent_field_s
{
    const char *name;
    fieldtype_t type;
    int ofs;
    DynEntFieldEntityPart entityPart;
    void (__cdecl *setter)(unsigned __int16 absDynEntId, const dynent_field_s *pField);
    void (__cdecl *getter)(unsigned __int16 absDynEntId, const dynent_field_s *pField);
};

static void __cdecl CScr_SP_RegAddFieldsForEntity();
static void __cdecl CScr_SP_RegAddFieldsForRadiant();
static void __cdecl CScr_SP_RegFreeScripts();
static void __cdecl CScr_SP_RegLoadLevel();
static void __cdecl CScr_SP_RegLoadStructs();
static void __cdecl CScr_SP_RegFPrintInternal();
static void __cdecl CScr_SP_RegPlayRumbleOnPosition();
static void __cdecl CScr_SP_RegSetAnimInternal(scr_entref_t entref);
static void __cdecl CScr_SP_RegSetAnimKnobInternal(scr_entref_t entref);
static void __cdecl CScr_SP_RegSetAnimKnobAllInternal(scr_entref_t entref);
static centity_s *__cdecl CScr_SP_GetMethodEntity(scr_entref_t entref);

void __cdecl CScr_PlayWeaponDeathEffects();
void __cdecl CScr_ForceGameModeMappings();
void __cdecl CScrCmd_HasEyes(scr_entref_t entref);
void __cdecl CScrCmd_SetBlur(scr_entref_t entref);
void __cdecl CScrCmd_ClientFlag(scr_entref_t entref);
void __cdecl CScrCmd_GetNormalizedCameraMovement(scr_entref_t entref);
void __cdecl CScrCmd_GetNormalizedMovement(scr_entref_t entref);
void __cdecl CScrCmd_HideViewModel(scr_entref_t entref);
void __cdecl CScrCmd_LinkToCamera(scr_entref_t entref);
void __cdecl CScrCmd_PlayRumbleOnEntity(scr_entref_t entref);
void __cdecl CScrCmd_PlayRumbleLoopOnEntity(scr_entref_t entref);
void __cdecl CScrCmd_StopRumble(scr_entref_t entref);
void __cdecl CScrCmd_PlayRumbleOnEntity_Internal(scr_entref_t entref, int cmd);
void __cdecl CScr_SetWatchStyle(scr_entref_t entref);
void __cdecl CScr_PlayRumbleOnPosition();
void __cdecl CScrCmd_FireWeapon(scr_entref_t entref);
void __cdecl CScrCmd_ShowViewModel(scr_entref_t entref);
void __cdecl CScrCmd_Swimming(scr_entref_t entref);
void __cdecl CScrCmd_GetOrigin(scr_entref_t entref);
void __cdecl CScrCmd_GetAngles(scr_entref_t entref);
void __cdecl CScrCmd_GetEye(scr_entref_t entref);
void __cdecl CScrCmd_GetEyeApprox(scr_entref_t entref);
void __cdecl CPlayerCmd_getAngles(scr_entref_t entref);
void __cdecl CPlayerCmd_setAngles(scr_entref_t entref);
void __cdecl CPlayerCmd_GetVelocity(scr_entref_t entref);
void __cdecl CScr_GetWaterHeight();
void __cdecl CScrCmd_UnlinkAllFromCamera(scr_entref_t entref);
void __cdecl CScrCmd_UnlinkFromCamera(scr_entref_t entref);
void __cdecl CScrCmd_MapShaderConstant(scr_entref_t entref);
void __cdecl CScrCmd_SetShaderConstant(scr_entref_t entref);
void __cdecl CScrCmd_PlayerSetGroundReferenceEnt(scr_entref_t entref);
void __cdecl CScr_AddFieldsForDynEntity();
void __cdecl CScr_ClearAlternateAimParams();
void __cdecl CScr_UseAlternateAimParams();
static void __cdecl CScr_ClearAlternateAimParams_Method(scr_entref_t entref);
static void __cdecl CScr_UseAlternateAimParams_Method(scr_entref_t entref);
static void __cdecl CScr_IsSpectating_Method(scr_entref_t entref);
static void CScr_EvalSpectating(int localClientNum, bool useOptionalThirdPersonCheck, int optionalThirdPersonFlag);
void __cdecl CScr_UpdateDvarsFromProfile();
void __cdecl CScr_UpdateGamerProfile();
void __cdecl CScr_SP_GetFunctionProjectSpecific();
void __cdecl CScr_SP_GetMethodProjectSpecific();
void __cdecl CScr_SP_GetDynEntityField();
void __cdecl CScr_SP_GetFunction();
void __cdecl CScr_SP_GetMethod();
void __cdecl CScr_SP_ExecEntThread();
void __cdecl CScr_GetDynEnt();
void __cdecl CScr_GetDynEntArray();
void __cdecl CScr_SP_GetEntAnimTree();
void __cdecl CScr_SP_GetMeansOfDeathConstString();
void __cdecl CScr_GetLocalClientHealth();
void __cdecl CScr_GetLocalClientMaxHealth();
void __cdecl CScr_GetPersistentProfileVar();
void __cdecl CScr_SetPersistentProfileVar();
void __cdecl CScr_SetClientDvar();
void __cdecl CScr_HasCollectible();
void __cdecl CScr_SetCollectible();
void __cdecl CScr_SP_PlayWeaponDamageEffects();
void __cdecl CScr_ResetSunLight();
void __cdecl CScr_SetSunLight();
void __cdecl CScr_UncrunchArray();
void __cdecl CScr_VisionSetDamage();
void __cdecl CScr_VisionSetUnderwater();
void __cdecl CScr_VisionSetNaked();
void __cdecl CScr_GetVisionSetNaked();
void __cdecl CScr_WaterPlop();
void __cdecl CScr_GiveGamerPicture();
void __cdecl CScr_GetLinkedEnt(scr_entref_t entref);
void __cdecl CScr_GetDynEntityField(unsigned __int16 absDynEntId, int clientNum, unsigned int offset);

// CoDSP_rdBlackOps.map.c coop CSC wrappers (cg_scr_main_coop.obj)
static void __cdecl CScr_SP_GetEntityField();
static void __cdecl CScr_SP_SetEntityField();
static void __cdecl CScr_SP_GetObjectField();
static void __cdecl CScr_SP_SetObjectField();
static void __cdecl CScr_SP_SetDynEntityField();
static void __cdecl CScr_SP_GetDynEntStringField();
static void __cdecl CScr_SP_SetCustomDynEntFieldReadOnly();
static void __cdecl CScr_SP_Notify();
static void __cdecl CScr_SP_UpdateTagInternal();
static void __cdecl CScr_SP_FxParamError();
static void __cdecl CScr_SP_SetFxAngles();
static void __cdecl CScr_SP_ReadOnly();
static void __cdecl CScr_SP_GlassSmash();
static void __cdecl CScr_SP_HandleAnimError();
static void __cdecl CScr_SP_DestructibleCallback();
static void __cdecl CScr_SP_GetModel();
static void __cdecl CScr_SP_GetType();
static void __cdecl CScr_SP_GetSpecies();
static void __cdecl CScr_SP_GetIsDog();
static void __cdecl CScr_SP_GetVehicleClass();
static void __cdecl CScr_SP_GetVehicleTreadFx();
static void __cdecl CScr_SP_GetWeaponName();
static void __cdecl CScr_SP_PlayerFootstepEvent();
static void __cdecl CScr_SP_PlayerJumpEvent();
static void __cdecl CScr_SP_PlayerLandEvent();
static void __cdecl CScr_SP_PlayerFoliageEvent();
void __cdecl CScr_PlayAIFootstepSound(int localClientNum, centity_s *cent, int isLeftFoot, bool isSpeciesDog);
static XModel *__cdecl CG_GetXModel(const char *name, int localClientNum);
static void __cdecl Scr_SetCustomDynEntFieldReadOnly(unsigned __int16 absDynEntId, const dynent_field_s *pField);
int __cdecl CScr_SetDynEntityField(unsigned __int16 absDynEntId, int clientNum, unsigned int offset);
void __cdecl CScr_ExtraCamConfig();
void __cdecl CScr_PlayFXOnDynEnt();
void __cdecl CScr_ShowScores();
void __cdecl CScr_HideScores();
void __cdecl CScr_NotifyLevel();
void __cdecl CScr_EnableWaterSheetFX();
void __cdecl CScr_StartWaterSheetFX();
void __cdecl CScr_StopWaterSheetFX();
void __cdecl CScr_SetWaterSheetFXRate();
void __cdecl CScr_SetVolumetricFog();
void __cdecl CScr_SetVolumetricFogForClient();
void __cdecl CScr_SetWaterFog();
void __cdecl CScr_StopExtraCam();
void __cdecl CScr_SetExtraCamFov();
void __cdecl CScr_IsExtraCam(scr_entref_t entref);
void __cdecl CScr_SpawnDynEnt();
void __cdecl CScr_LaunchDynEnt();
void __cdecl CScr_SetDynEntEnabled();
void __cdecl CScr_EntGetsWeaponFireNotification(scr_entref_t entref);
void __cdecl CScr_EntYawOverridesLinkYaw(scr_entref_t entref);

static const dvar_s *s_missionGlobals[8];
static bool s_missionGlobalsRegistered;
static int s_playWeaponDamageEffects;
static int s_playWeaponDeathEffects;
static int s_activateExploderCallback;
static int s_deactivateExploderCallback;
static int s_zombieEyeCallback;

static void CScr_PlayWeaponDeathEffectsInternal(
    unsigned int localClientNum,
    int entnum,
    unsigned int weaponIndex,
    int mod)
{
    centity_s *ent;
    unsigned __int16 thread;

    if ( !s_playWeaponDeathEffects )
        return;
    ent = CG_GetEntity(localClientNum, entnum);
    if ( !ent || !weaponIndex )
        return;
    if ( weaponIndex >= BG_GetNumWeapons() )
        Com_Error(ERR_DROP, "PlayWeaponDeathEffects called with unknown weapon index %u", weaponIndex);
    Scr_AddInt(mod, SCRIPTINSTANCE_CLIENT);
    Scr_AddString((char *)BG_WeaponName(weaponIndex), SCRIPTINSTANCE_CLIENT);
    Scr_AddInt(localClientNum, SCRIPTINSTANCE_CLIENT);
    thread = CScr_ExecEntThread(ent, s_playWeaponDeathEffects, 3u);
    Scr_FreeThread(thread, SCRIPTINSTANCE_CLIENT);
}

static void CScr_SP_RegisterMissionGlobals()
{
    char name[32];
    int i;

    if ( s_missionGlobalsRegistered )
        return;
    s_missionGlobalsRegistered = true;
    for ( i = 0; i < 8; ++i )
    {
        Com_sprintf(name, sizeof(name), "mg%d", i);
        s_missionGlobals[i] = _Dvar_RegisterInt(
            name,
            0,
            0,
            255,
            0x4001u,
            "Used by script for keeping track of mission globals");
    }
}

static unsigned __int16 DynEnt_GetDynEntityByName(unsigned __int16 targetname)
{
    unsigned int drawType;
    unsigned __int16 j;
    unsigned __int16 count;
    const DynEntityDef *def;

    for ( drawType = 0; drawType < 2; ++drawType )
    {
        count = DynEnt_GetEntityCount((DynEntityCollType)drawType);
        for ( j = 0; j < count; ++j )
        {
            def = DynEnt_GetEntityDef(j, (DynEntityDrawType)drawType);
            if ( def && def->targetname == targetname )
                return DynEnt_GetClientAbsId(j, (DynEntityDrawType)drawType);
        }
    }
    return 0xFFFF;
}

static void SCR_DynEnt_GetDynEntityArray(unsigned __int16 targetname)
{
    unsigned int drawType;
    unsigned __int16 j;
    unsigned __int16 count;
    const DynEntityDef *def;
    unsigned __int16 absId;

    Scr_MakeArray(SCRIPTINSTANCE_CLIENT);
    for ( drawType = 0; drawType < 2; ++drawType )
    {
        count = DynEnt_GetEntityCount((DynEntityCollType)drawType);
        for ( j = 0; j < count; ++j )
        {
            def = DynEnt_GetEntityDef(j, (DynEntityDrawType)drawType);
            if ( !def )
                continue;
            if ( targetname != 0xFFFF && def->targetname != targetname )
                continue;
            absId = DynEnt_GetClientAbsId(j, (DynEntityDrawType)drawType);
            Scr_AddEntityNum(absId, 4, SCRIPTINSTANCE_CLIENT, 0);
            Scr_AddArray(SCRIPTINSTANCE_CLIENT);
        }
    }
}

static int GetAttachedEntNum(int localClientNum, centity_s *cent)
{
    if ( !cent )
        return 1023;
    if ( cent->nextState.eType == ET_SCRIPTMOVER )
        return cent->nextState.lerp.u.scriptMover.attachedEntNum;
    if ( cent->linkInfo )
        return cent->linkInfo->linkEnt;
    (void)localClientNum;
    return 1023;
}

static void CScr_PlayWeaponDamageEffectsInternal(
    unsigned int localClientNum,
    int entnum,
    unsigned int weaponIndex,
    int mod)
{
    centity_s *ent;
    unsigned __int16 thread;

    if ( !s_playWeaponDamageEffects )
        return;
    ent = CG_GetEntity(localClientNum, entnum);
    if ( !ent || !weaponIndex )
        return;
    if ( weaponIndex >= BG_GetNumWeapons() )
        Com_Error(ERR_DROP, "PlayWeaponDamageEffects called with unknown weapon index %u", weaponIndex);
    Scr_AddInt(mod, SCRIPTINSTANCE_CLIENT);
    Scr_AddString((char *)BG_WeaponName(weaponIndex), SCRIPTINSTANCE_CLIENT);
    Scr_AddInt(localClientNum, SCRIPTINSTANCE_CLIENT);
    thread = CScr_ExecEntThread(ent, s_playWeaponDamageEffects, 3u);
    Scr_FreeThread(thread, SCRIPTINSTANCE_CLIENT);
}

unsigned __int16 *footTags[4] =
{
    &scr_const.j_palm_ri,
    &scr_const.j_palm_le,
    &scr_const.j_ball_ri,
    &scr_const.j_palm_ri
};

BuiltinFunctionDef client_project_functions[] =
{
  { "getgridfrompos", &CScr_GetGridFromPos, 0 },
  { "compassscale", &CScr_CompassScale, 0 },
  { "resetcompassscale", &CScr_ResetCompassScale, 0 },
  { "isdemoplaying", &CScr_IsDemoPlaying, 0 },
  { "isspectating", &CScr_IsSpectating, 0 },
  { "getlocalplayerteam", &CScr_GetLocalPlayerTeam, 0 },
  { "playfxontag", &CScr_PlayFXOnTag, 0 },
  { "playviewmodelfx", &CScr_PlayViewmodelFX, 0 },
  { "spawnfx", &CScr_SpawnFX, 0 },
  { "deletefx", &CScr_DeleteFX, 0 },
  { "triggerfx", &CScr_TriggerFX, 0 },
  { "getanimlength", &CScr_GetAnimLength, 0 },
  { "animateui", &CScr_AnimateUI, 0 },
  { "showui", &CScr_ShowUI, 0 },
  { "getcurrentweapon", &CScr_GetCurrentWeapon, 0 },
  { "getcurrentweaponincludingmelee", &CScr_GetCurrentWeaponIncludingMelee, 0 },
  { "hasweapon", &CScr_HasWeapon, 0 },
  { "gettotalammo", &CScr_GetTotalAmmo, 0 },
  { "setlocalradarenabled", &CScr_SetLocalRadarEnabled, 0 },
  { "setlocalradarposition", &CScr_SetLocalRadarPosition, 0 },
  { "setextracamentity", &CScr_SetExtraCamEntity, 0 },
  { "setextracamactive", &CScr_SetExtraCamActive, 0 },
  { "getextracamstatic", &CScr_GetExtraCamStatic, 0 },
  { "setextracamstatic", &CScr_SetExtraCamStatic, 0 },
  { "setextracamorigin", &CScr_SetExtraCamOrigin, 0 },
  { "setextracamangles", &CScr_SetExtraCamAngles, 0 },
  { "iscameraspiketoggled", &CScr_IsCameraSpikeToggled, 0 },
  // LWSS ADD FROM BLOPS MP RETAIL (LATEST)
  { "setclientvolumetricfog", &CScr_SetClientVolumetricFog, 0 },
  { "setclientdvar", &CScr_SetClientDvar, 0 },
  { "switchtoservervolumetricfog", &CScr_SwitchToServerVolumetricFog, 0 },
  { "switchtoclientvolumetricfog", &CScr_SwitchToClientVolumetricFog, 0 },
  { "isinhelicopter", &CScr_IsInHelicopter, 0 },
  { "isads", &CScr_IsADS, 0 },
  { "isonturret", &CScr_IsOnTurret, 0 },
  { "isthrowinggrenade", &CScr_IsThrowingGrenade, 0 },
  { "ismeleeing", &CScr_IsMeleeing, 0 },
  { "getweaponammoclip", &CScr_GetWeaponAmmoClip, 0 },
  { "createdynentandlaunch", &CScr_CreateDynEntAndLaunch, 0 },
  { "playweapondeatheffects", &CScr_PlayWeaponDeathEffects, 0 },
  { "forcegamemodemappings", &CScr_ForceGameModeMappings, 0 },
  { "postloadscripts", &CScr_PostLoadScripts, 0 },
  // cg_scr_main.obj / cg_scr_main_coop.obj registrations (CoDSP_rdBlackOps.map.c)
  { "addfieldsforentity", &CScr_SP_RegAddFieldsForEntity, 0 },
  { "addfieldsforradiant", &CScr_SP_RegAddFieldsForRadiant, 0 },
  { "addfieldsfordynentity", &CScr_AddFieldsForDynEntity, 0 },
  { "fprint_internal", &CScr_SP_RegFPrintInternal, 0 },
  { "freescripts", &CScr_SP_RegFreeScripts, 0 },
  { "loadlevel", &CScr_SP_RegLoadLevel, 0 },
  { "loadstructs", &CScr_SP_RegLoadStructs, 0 },
  { "playrumbleonposition_internal", &CScr_SP_RegPlayRumbleOnPosition, 0 },
  { "setclientscripts", reinterpret_cast<void(__cdecl *)()>(CScr_SetClientScripts), 0 },
  { "clearalternateaimparams", &CScr_ClearAlternateAimParams, 0 },
  { "usealternateaimparams", &CScr_UseAlternateAimParams, 0 },
  { "updatedvarsfromprofile", &CScr_UpdateDvarsFromProfile, 0 },
  { "updategamerprofile", &CScr_UpdateGamerProfile, 0 },
  { "spawnfxinternal", &CScr_SpawnFX, 0 },
  { "setuniqueclientscripts", reinterpret_cast<void(__cdecl *)()>(CScr_SetUniqueClientScripts), 0 },
  { "getfunctionprojectspecific", &CScr_SP_GetFunctionProjectSpecific, 0 },
  { "getmethodprojectspecific", &CScr_SP_GetMethodProjectSpecific, 0 },
  { "getdynentityfield", &CScr_SP_GetDynEntityField, 0 },
  { "setdynentityfield", &CScr_SP_SetDynEntityField, 0 },
  { "getdynentstringfield", &CScr_SP_GetDynEntStringField, 0 },
  { "setcustomdynentfieldreadonly", &CScr_SP_SetCustomDynEntFieldReadOnly, 0 },
  { "getfunction", &CScr_SP_GetFunction, 0 },
  { "getmethod", &CScr_SP_GetMethod, 0 },
  { "getentityfield", &CScr_SP_GetEntityField, 0 },
  { "getobjectfield", &CScr_SP_GetObjectField, 0 },
  { "setentityfield", &CScr_SP_SetEntityField, 0 },
  { "setobjectfield", &CScr_SP_SetObjectField, 0 },
  { "getfakeentity", &CScr_SpawnFakeEnt, 0 },
  { "freefakeentity", &CScr_DeleteFakeEnt, 0 },
  { "notify", &CScr_SP_Notify, 0 },
  { "playfxinternal", &CScr_PlayFX, 0 },
  { "updatetaginternal", &CScr_SP_UpdateTagInternal, 0 },
  // retail alias keys
  { "castfloat", &CScr_CastFloat, 0 },
  { "castint", &CScr_CastInt, 0 },
  { "getsoundname", &CScr_GetAliasName, 0 },
  { "getaliasname", &CScr_GetAliasName, 0 },
  { "getsoundcount", &CScr_GetAliasCount, 0 },
  { "getaliascount", &CScr_GetAliasCount, 0 },
  { "issoundloop", &CScr_IsAliasLoop, 0 },
  { "isaliasloop", &CScr_IsAliasLoop, 0 },
  // SP coop CSC globals (CoDSP_rdBlackOps.map.c / cg_scr_main_coop.obj)
  { "execentthread", &CScr_SP_ExecEntThread, 0 },
  { "getdynent", &CScr_GetDynEnt, 0 },
  { "getdynentarray", &CScr_GetDynEntArray, 0 },
  { "getentanimtree", &CScr_SP_GetEntAnimTree, 0 },
  { "destructiblecallback", &CScr_SP_DestructibleCallback, 0 },
  { "enablewatersheetfx", &CScr_EnableWaterSheetFX, 0 },
  { "extracamconfig", &CScr_ExtraCamConfig, 0 },
  { "fxparamerror", &CScr_SP_FxParamError, 0 },
  { "getgenericent", &CScr_GetEnt, 0 },
  { "getisdog", &CScr_SP_GetIsDog, 0 },
  { "getlocalclienthealth", &CScr_GetLocalClientHealth, 0 },
  { "getlocalclientmaxhealth", &CScr_GetLocalClientMaxHealth, 0 },
  { "getmeansofdeathconststring", &CScr_SP_GetMeansOfDeathConstString, 0 },
  { "getmodel", &CScr_SP_GetModel, 0 },
  { "getpersistentprofilevar", &CScr_GetPersistentProfileVar, 0 },
  { "getspecies", &CScr_SP_GetSpecies, 0 },
  { "gettype", &CScr_SP_GetType, 0 },
  { "getvehicleclass", &CScr_SP_GetVehicleClass, 0 },
  { "getvehicletreadfx", &CScr_SP_GetVehicleTreadFx, 0 },
  { "getvisionsetnaked", &CScr_GetVisionSetNaked, 0 },
  { "getweaponname", &CScr_SP_GetWeaponName, 0 },
  { "givegamerpicture", &CScr_GiveGamerPicture, 0 },
  { "glasssmash", &CScr_SP_GlassSmash, 0 },
  { "handleanimerror", &CScr_SP_HandleAnimError, 0 },
  { "hascollectible", &CScr_HasCollectible, 0 },
  { "hidescores", &CScr_HideScores, 0 },
  { "launchdynent", &CScr_LaunchDynEnt, 0 },
  { "notifylevel", &CScr_NotifyLevel, 0 },
  { "playfxondynent", &CScr_PlayFXOnDynEnt, 0 },
  { "playweapondamageeffects", &CScr_SP_PlayWeaponDamageEffects, 0 },
  { "playerfoliageevent", &CScr_SP_PlayerFoliageEvent, 0 },
  { "playerfootstepevent", &CScr_SP_PlayerFootstepEvent, 0 },
  { "playerjumpevent", &CScr_SP_PlayerJumpEvent, 0 },
  { "playerlandevent", &CScr_SP_PlayerLandEvent, 0 },
  { "readonly", &CScr_SP_ReadOnly, 0 },
  { "resetsunlight", &CScr_ResetSunLight, 0 },
  { "setcollectible", &CScr_SetCollectible, 0 },
  { "setdynentenabled", &CScr_SetDynEntEnabled, 0 },
  { "setextracamfov", &CScr_SetExtraCamFov, 0 },
  { "setfxangles", &CScr_SP_SetFxAngles, 0 },
  { "setlevelscript", reinterpret_cast<void(__cdecl *)()>(CScr_SetLevelScript), 0 },
  { "setpersistentprofilevar", &CScr_SetPersistentProfileVar, 0 },
  { "setsunlight", &CScr_SetSunLight, 0 },
  { "setvolfog", &CScr_SetVolumetricFog, 0 },
  { "setvolfogforclient", &CScr_SetVolumetricFogForClient, 0 },
  { "setvolumetricfog", &CScr_SetVolumetricFog, 0 },
  { "setvolumetricfogforclient", &CScr_SetVolumetricFogForClient, 0 },
  { "setwaterfog", &CScr_SetWaterFog, 0 },
  { "setwatersheetfxrate", &CScr_SetWaterSheetFXRate, 0 },
  { "showscores", &CScr_ShowScores, 0 },
  { "spawndynent", &CScr_SpawnDynEnt, 0 },
  { "startwatersheetfx", &CScr_StartWaterSheetFX, 0 },
  { "stopextracam", &CScr_StopExtraCam, 0 },
  { "stopwatersheetfx", &CScr_StopWaterSheetFX, 0 },
  { "structcontents", &CScr_StructInfo, 0 },
  { "uncruncharray", &CScr_UncrunchArray, 0 },
  { "visionsetdamage", &CScr_VisionSetDamage, 0 },
  { "visionsetunderwater", &CScr_VisionSetUnderwater, 0 },
  { "visionsetnaked", &CScr_VisionSetNaked, 0 },
  { "waterplop", &CScr_WaterPlop, 0 },
  { "getwaterheight", &CScr_GetWaterHeight, 0 },
  // LWSS END
};

// LWSS: Looks congruent to retail blops MP
const BuiltinMethodDef client_project_methods[72] =
{
  { "getorigin", &CScrCmd_GetOrigin, 0 },
  { "getangles", &CScrCmd_GetAngles, 0 },
  { "getplayerangles", &CPlayerCmd_getAngles, 0 },
  { "setplayerangles", &CPlayerCmd_setAngles, 0 },
  { "getvelocity", &CPlayerCmd_GetVelocity, 0 },
  { "geteye", &CScrCmd_GetEye, 0 },
  { "geteyeapprox", &CScrCmd_GetEyeApprox, 0 },
  { "gettagorigin", &CScr_GetTagOrigin, 0 },
  { "gettagangles", &CScr_GetTagAngles, 0 },
  { "gettagforwardvector", &CScr_GetTagForwardVector, 0 },
  { "getinkillcam", &CScr_GetInKillcam, 0 },
  { "getowner", &CScrCmd_GetOwner, 0 },
  { "getanimstate", &CScr_GetAnimState, 0 },
  { "getanimstatecategory", &CScr_GetAnimStateCategory, 0 },
  { "getvehiclehealth", &CScr_GetVehicleHealth, 0 },
  { "getlefttreadhealth", &CScr_GetLeftTreadHealth, 0 },
  { "getrighttreadhealth", &CScr_GetRightTreadHealth, 0 },
  { "gethelidamagestate", &CScr_GetHeliDamageState, 0 },
  { "isburning", &CScrCmd_IsBurning, 0 },
  { "hasperk", &CPlayerCmd_HasPerk, 0 },
  { "getstance", &CScr_GetStance, 0 },
  { "shellshock", &CScrCmd_ShellShock, 0 },
  { "earthquake", &CScrCmd_Earthquake, 0 },
  { "setenemyglobalscrambler", &CScr_SetEnemyGlobalScrambler, 0 },
  { "setenemyscrambleramount", &CScr_SetEnemyScramblerAmount, 0 },
  { "getenemyscrambleramount", &CScr_GetEnemyScramblerAmount, 0 },
  { "isscrambled", &CScr_IsScrambled, 0 },
  { "setfriendlyscrambleramount", &CScr_SetFriendlyScramblerAmount, 0 },
  { "getfriendlyscrambleramount", &CScr_GetFriendlyScramblerAmount, 0 },
  { "addfriendlyscrambler", &CScr_AddFriendlyScrambler, 0 },
  { "clearnearestenemyscrambler", &CScr_ClearNearestEnemyScrambler, 0 },
  { "setnearestenemyscrambler", &CScr_SetNearestEnemyScrambler, 0 },
  { "removefriendlyscrambler", &CScr_RemoveFriendlyScrambler, 0 },
  { "removeallfriendlyscramblers", &CScr_RemoveAllFriendlyScramblers, 0 },
  { "hastacticalmaskoverlay", CScr_HasTacticalMaskOverlay, 0 },
  { "setflagasaway", &CScr_SetFlagAsAway, 0 },
  { "getparententity", &CScr_GetParentEntity, 0 },
  { "getlocalclientnumber", &CScrCmd_GetLocalClientNumber, 0 },
  { "haseyes", &CScrCmd_HasEyes, 0 },
  { "setblur", &CScrCmd_SetBlur, 0 },
  { "playersetgroundreferenceent", &CScrCmd_PlayerSetGroundReferenceEnt, 0 },
  { "mapshaderconstant", &CScrCmd_MapShaderConstant, 0 },
  { "setshaderconstant", &CScrCmd_SetShaderConstant, 0 },
  { "clientflag", &CScrCmd_ClientFlag, 0 },
  { "isextracam", &CScr_IsExtraCam, 0 },
  { "entgetsweaponfirenotification", &CScr_EntGetsWeaponFireNotification, 0 },
  { "entyawoverrideslinkyaw", &CScr_EntYawOverridesLinkYaw, 0 },
  { "getnormalizedcameramovement", &CScrCmd_GetNormalizedCameraMovement, 0 },
  { "getnormalizedmovement", &CScrCmd_GetNormalizedMovement, 0 },
  { "hideviewmodel", &CScrCmd_HideViewModel, 0 },
  { "linktocamera", &CScrCmd_LinkToCamera, 0 },
  { "playrumbleonentity", &CScrCmd_PlayRumbleOnEntity, 0 },
  { "playrumblelooponentity", &CScrCmd_PlayRumbleLoopOnEntity, 0 },
  { "stoprumble", &CScrCmd_StopRumble, 0 },
  { "fireweapon", &CScrCmd_FireWeapon, 0 },
  { "playrumbleonentity_internal", &CScrCmd_PlayRumbleOnEntity, 0 },
  { "showviewmodel", &CScrCmd_ShowViewModel, 0 },
  { "swimming", &CScrCmd_Swimming, 0 },
  { "unlinkallfromcamera", &CScrCmd_UnlinkAllFromCamera, 0 },
  { "unlinkfromcamera", &CScrCmd_UnlinkFromCamera, 0 },
  { "setphysicsgravity", &CScr_SetPhysicsGravity, 0 },
  { "clearphysicsgravity", &CScr_ClearPhysicsGravity, 0 },
  { "setaniminternal", &CScr_SP_RegSetAnimInternal, 0 },
  { "setanimknobinternal", &CScr_SP_RegSetAnimKnobInternal, 0 },
  { "setanimknoballinternal", &CScr_SP_RegSetAnimKnobAllInternal, 0 },
  { "getlinkedent", &CScr_GetLinkedEnt, 0 },
  { "playsoundonentity", &CScr_PlaySoundOnEntity, 0 },
  { "playloopsoundonentity", &CScr_PlayLoopSoundOnEntity, 0 },
  { "stoploopsoundonentity", &CScr_StopLoopSoundOnEntity, 0 },
  { "clearalternateaimparams", &CScr_ClearAlternateAimParams_Method, 0 },
  { "usealternateaimparams", &CScr_UseAlternateAimParams_Method, 0 },
  { "isspectating", &CScr_IsSpectating_Method, 0 }
};

cached_tag_mat_t cg_cachedTagMat;
cscr_mp_data_t cg_scr_sp_data;

//void __cdecl CScrCmd_Earthquake(scr_entref_t entref)
//{
//    float Float; // [esp+20h] [ebp-1Ch]
//    float source[3]; // [esp+24h] [ebp-18h] BYREF
//    int duration; // [esp+30h] [ebp-Ch]
//    float radius; // [esp+34h] [ebp-8h]
//    float scale; // [esp+38h] [ebp-4h]
//
//    scale = Scr_GetFloat(0, SCRIPTINSTANCE_CLIENT);
//    Float = Scr_GetFloat(1u, SCRIPTINSTANCE_CLIENT);
//    duration = (int)((float)(Float * 1000.0) + 9.313225746154785e-10);
//    Scr_GetVector(2u, source, SCRIPTINSTANCE_CLIENT);
//    radius = Scr_GetFloat(3u, SCRIPTINSTANCE_CLIENT);
//    CG_StartShakeCamera(entref.client, scale, duration, source, radius);
//}

unsigned int __cdecl CScr_SpawnFXInternal(int localClientNum, int fxId, float (*axis)[3], float *pos, int time)
{
    const FxEffectDef *fxDef; // [esp+0h] [ebp-8h]

    fxDef = CG_GetLocalClientStaticGlobals(localClientNum)->fxs[fxId];
    if ( !fxDef
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp", 91, 0, "%s", "fxDef") )
    {
        __debugbreak();
    }
    return FX_SpawnOrientedEffect(localClientNum, fxDef, time, pos, axis, 0x3FFu);
}

void CScr_DeleteFX()
{
    int localClientNum; // [esp+8h] [ebp-Ch]
    int intFxPtr; // [esp+Ch] [ebp-8h]

    localClientNum = CScr_GetLocalClientNum(0);
    intFxPtr = Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT);
    if ( (unsigned int)localClientNum >= 2
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                    118,
                    0,
                    "localClientNum not in [0, MAX_LOCAL_CLIENTS]\n\t%i not in [%i, %i]",
                    localClientNum,
                    0,
                    1) )
    {
        __debugbreak();
    }
    FX_ThroughWithEffect(localClientNum, intFxPtr, 1);
}

// Retail: cg_scr_main_coop.obj CScr_TriggerFX — restarts ET_FX entities created via newFx.
void CScr_TriggerFX()
{
    scr_entref_t entref;
    centity_s *cent;
    cg_s *cgameGlob;
    int localClientNum;
    float delayMs;

    if ( !Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) || Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) > 2 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "Incorrect number of parameters", 0);
    entref = Scr_GetEntityRef(0, SCRIPTINSTANCE_CLIENT);
    if ( entref.classnum )
        Scr_ParamError(0, "entity wasn't created with 'newFx'", SCRIPTINSTANCE_CLIENT);
    localClientNum = entref.client;
    cent = CG_GetEntity(localClientNum, entref.entnum);
    if ( !cent || cent->nextState.eType != ET_FX )
        Scr_ParamError(0, "entity wasn't created with 'newFx'", SCRIPTINSTANCE_CLIENT);
    cgameGlob = CG_GetLocalClientGlobals(localClientNum);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) == 2 )
    {
        delayMs = Scr_GetFloat(1u, SCRIPTINSTANCE_CLIENT) * 1000.0f;
        cent->nextState.time2 = (int)(delayMs + 9.313225746154785e-10);
    }
    else
    {
        cent->nextState.time2 = cgameGlob->time;
    }
}

void CScr_SpawnFX()
{
    unsigned int v0; // [esp+0h] [ebp-94h]
    unsigned int v1; // [esp+10h] [ebp-84h]
    unsigned int value; // [esp+20h] [ebp-74h]
    float Float; // [esp+34h] [ebp-60h]
    char *error; // [esp+38h] [ebp-5Ch]
    float pos[3]; // [esp+44h] [ebp-50h] BYREF
    int iTime; // [esp+50h] [ebp-44h]
    int localClientNum; // [esp+54h] [ebp-40h]
    int numParams; // [esp+58h] [ebp-3Ch]
    float angles[3]; // [esp+5Ch] [ebp-38h] BYREF
    int fxId; // [esp+68h] [ebp-2Ch]
    float axis[3][3]; // [esp+6Ch] [ebp-28h] BYREF
    float vecLength; // [esp+90h] [ebp-4h]

    numParams = Scr_GetNumParam(SCRIPTINSTANCE_CLIENT);
    if ( numParams < 4 || numParams > 6 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "Incorrect number of parameters", 0);
    localClientNum = CScr_GetLocalClientNum(0);
    if ( (unsigned int)localClientNum >= 2
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                    161,
                    0,
                    "localClientNum not in [0, MAX_LOCAL_CLIENTS]\n\t%i not in [%i, %i]",
                    localClientNum,
                    0,
                    1) )
    {
        __debugbreak();
    }
    fxId = Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT);
    if ( fxId <= 0 || fxId >= 196 )
    {
        error = va("CScr_PlayFX: invalid effect id %d", fxId);
        Scr_Error(SCRIPTINSTANCE_CLIENT, error, 0);
    }
    Scr_GetVector(2u, pos, SCRIPTINSTANCE_CLIENT);
    Float = Scr_GetFloat(3u, SCRIPTINSTANCE_CLIENT);
    iTime = (int)((float)(Float * 1000.0) + 9.313225746154785e-10);
    if ( numParams == 4 )
    {
        CScr_SetFxAngles(0, axis, angles);
        value = CScr_SpawnFXInternal(localClientNum, fxId, axis, pos, iTime);
        Scr_AddInt(value, SCRIPTINSTANCE_CLIENT);
    }
    else
    {
        if ( numParams != 5
            && numParams != 6
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                        179,
                        1,
                        "%s\n\t(numParams) = %i",
                        "(numParams == 5 || numParams == 6)",
                        numParams) )
        {
            __debugbreak();
        }
        Scr_GetVector(4u, axis[0], SCRIPTINSTANCE_CLIENT);
        vecLength = Vec3Normalize(axis[0]);
        if ( vecLength == 0.0 )
            CScr_FxParamError(localClientNum, 4u, "playFx called with (0 0 0) forward direction", fxId);
        if ( numParams == 5 )
        {
            CScr_SetFxAngles(1u, axis, angles);
            v1 = CScr_SpawnFXInternal(localClientNum, fxId, axis, pos, iTime);
            Scr_AddInt(v1, SCRIPTINSTANCE_CLIENT);
        }
        else
        {
            if ( numParams != 6
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                            193,
                            1,
                            "%s\n\t(numParams) = %i",
                            "(numParams == 6)",
                            numParams) )
            {
                __debugbreak();
            }
            Scr_GetVector(5u, axis[2], SCRIPTINSTANCE_CLIENT);
            vecLength = Vec3Normalize(axis[2]);
            if ( vecLength == 0.0 )
                CScr_FxParamError(localClientNum, 5u, "playFx called with (0 0 0) up direction", fxId);
            CScr_SetFxAngles(2u, axis, angles);
            v0 = CScr_SpawnFXInternal(localClientNum, fxId, axis, pos, iTime);
            Scr_AddInt(v0, SCRIPTINSTANCE_CLIENT);
        }
    }
}

// BlackOpsMP.retail.c sub_7EE150 @ 850142-850188
void CScr_PlayFXOnTag()
{
    scr_entref_t entref;
    unsigned int tagName;
    unsigned int effectHandle;
    int localClientNum;
    const FxEffectDef *fxDef;
    int numParams;
    const char *name;
    cgs_t *cgs;
    int fxId;

    numParams = Scr_GetNumParam(SCRIPTINSTANCE_CLIENT);
    if ( numParams != 4 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "Incorrect number of parameters for playfxontag", 0);
    localClientNum = CScr_GetLocalClientNum(0);
    fxId = Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT);
    entref = Scr_GetEntityRef(2, SCRIPTINSTANCE_CLIENT);
    if ( !Com_GetClientDObj(entref.entnum, localClientNum) )
    {
        Scr_Error(SCRIPTINSTANCE_CLIENT, va(
            "CScr_PlayFX: invalid entity for local client %i, either not in the snapshot for the client or dobj does not exist yet",
            localClientNum), 0);
    }
    if ( fxId <= 0 || fxId >= 196 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, va("CScr_PlayFX: invalid effect id %d", fxId), 0);
    cgs = CG_GetLocalClientStaticGlobals(localClientNum);
    fxDef = cgs->fxs[fxId];
    if ( !fxDef
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp", 251, 0, "%s", "fxDef") )
    {
        __debugbreak();
    }
    tagName = Scr_GetConstLowercaseString(3u, SCRIPTINSTANCE_CLIENT);
    name = SL_ConvertToString(tagName, SCRIPTINSTANCE_CLIENT);
    tagName = SL_FindLowercaseString(name, SCRIPTINSTANCE_SERVER);
    effectHandle = CG_PlayBoltedEffect(localClientNum, fxDef, entref.entnum, tagName);
    Scr_AddInt(effectHandle, SCRIPTINSTANCE_CLIENT);
}

void CScr_PlayViewmodelFX()
{
    cg_s *LocalClientGlobals; // eax
    unsigned int v1; // eax
    const char *v2; // eax
    int v3; // eax
    unsigned int v4; // [esp-8h] [ebp-48h]
    char *v5; // [esp-4h] [ebp-44h]
    unsigned int v6; // [esp-4h] [ebp-44h]
    char *error; // [esp+0h] [ebp-40h]
    unsigned int handle; // [esp+20h] [ebp-20h]
    unsigned __int8 boneIndex; // [esp+27h] [ebp-19h] BYREF
    int weaponNum; // [esp+28h] [ebp-18h]
    int nparams; // [esp+2Ch] [ebp-14h]
    int localClientNum; // [esp+30h] [ebp-10h]
    const cgs_t *cgs; // [esp+34h] [ebp-Ch]
    int fxId; // [esp+38h] [ebp-8h]
    int realTagName; // [esp+3Ch] [ebp-4h]

    nparams = Scr_GetNumParam(SCRIPTINSTANCE_CLIENT);
    if ( nparams != 3 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "PlayViewmodelFX() called with wrong params.\n", 0);
    localClientNum = CScr_GetLocalClientNum(0);
    if ( (unsigned int)localClientNum >= 2
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                    275,
                    0,
                    "localClientNum not in [0, MAX_LOCAL_CLIENTS]\n\t%i not in [%i, %i]",
                    localClientNum,
                    0,
                    1) )
    {
        __debugbreak();
    }
    fxId = Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT);
    if ( (unsigned int)fxId > 0xC4
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                    278,
                    0,
                    "fxId not in [0, MAX_EFFECT_NAMES]\n\t%i not in [%i, %i]",
                    fxId,
                    0,
                    196) )
    {
        __debugbreak();
    }
    realTagName = CScr_GetConstServerString(2u);
    if ( !realTagName )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "PlayViewmodelFX(): unable to find viewmodel tag.\n", 0);
    cgs = CG_GetLocalClientStaticGlobals(localClientNum);
    if ( !cgs && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp", 287, 0, "%s", "cgs") )
        __debugbreak();
    LocalClientGlobals = CG_GetLocalClientGlobals(localClientNum);
    weaponNum = BG_GetViewmodelWeaponIndex(&LocalClientGlobals->predictedPlayerState);
    boneIndex = -2;
    v4 = realTagName;
    v1 = CG_WeaponDObjHandle(localClientNum);
    if ( !CG_GetBoneIndex(localClientNum, v1, v4, &boneIndex) )
    {
        v5 = SL_ConvertToString(realTagName, SCRIPTINSTANCE_SERVER);
        v2 = BG_WeaponName(weaponNum);
        error = va("PlayViewmodelFX(): viewmodel weapon '%s', does not have bone '%s'", v2, v5);
        Scr_Error(SCRIPTINSTANCE_CLIENT, error, 0);
    }
    v6 = realTagName;
    v3 = CG_WeaponDObjHandle(localClientNum);
    handle = CG_PlayBoltedEffect(localClientNum, cgs->fxs[fxId], v3, v6);
    Scr_AddInt(handle, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_IsDemoPlaying()
{
    if ( Demo_IsPlaying() )
        Scr_AddInt(1, SCRIPTINSTANCE_CLIENT);
    else
        Scr_AddInt(0, SCRIPTINSTANCE_CLIENT);
}

static void CScr_EvalSpectating(int localClientNum, bool useOptionalThirdPersonCheck, int optionalThirdPersonFlag)
{
    cg_s *cgameGlob;

    if ( (unsigned int)localClientNum >= MAX_LOCAL_CLIENTS )
        return;
    cgameGlob = CG_GetLocalClientGlobals(localClientNum);
    if ( !cgameGlob
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp", 343, 0, "%s", "cgameGlob") )
    {
        __debugbreak();
    }
    if ( cgameGlob == (cg_s *)-263324
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp", 346, 0, "%s", "ps") )
    {
        __debugbreak();
    }
    if ( Demo_IsPlaying() )
    {
        if ( Demo_IsMovieCamera() || Demo_IsThirdPersonCamera() )
        {
            Scr_AddInt(1, SCRIPTINSTANCE_CLIENT);
            return;
        }
    }
    else if ( (!useOptionalThirdPersonCheck
                    || !optionalThirdPersonFlag
                    || cgameGlob->renderingThirdPerson
                    || (cgameGlob->predictedPlayerState.otherFlags & 2) == 0)
                 && (cgameGlob->predictedPlayerState.otherFlags & 0x1A) != 0 )
    {
        Scr_AddInt(1, SCRIPTINSTANCE_CLIENT);
        return;
    }
    Scr_AddInt(0, SCRIPTINSTANCE_CLIENT);
}

static void __cdecl CScr_IsSpectating_Method(scr_entref_t entref)
{
    if ( entref.classnum )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "not an entity", 0);
    CScr_EvalSpectating(entref.client, false, 0);
}

void __cdecl CScr_IsSpectating()
{
    int localClientNum;
    int optionalThirdPersonFlag;

    localClientNum = CScr_GetLocalClientNum(0);
    optionalThirdPersonFlag = 0;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) > 1 )
        optionalThirdPersonFlag = Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT);
    CScr_EvalSpectating(localClientNum, Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) > 1, optionalThirdPersonFlag);
}

void __cdecl CScrCmd_IsBurning(scr_entref_t entref)
{
    centity_s *pSelf; // [esp+Ch] [ebp-Ch]
    unsigned int isBurning; // [esp+14h] [ebp-4h]

    if ( entref.classnum )
    {
        Scr_Error(SCRIPTINSTANCE_CLIENT, "not an entity", 0);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x600u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                        389,
                        0,
                        "%s",
                        "entref.entnum < MAX_LOCAL_CENTITIES") )
        {
            __debugbreak();
        }
        if ( CG_GetClientNumForLocalClient(entref.client) == entref.entnum )
            pSelf = &CG_GetLocalClientGlobals(entref.client)->predictedPlayerEntity;
        else
            pSelf = CG_GetEntity(entref.client, entref.entnum);
        if ( entref.entnum >= 0x400u )
            CG_GetFakeEntity(entref.client, entref.entnum);
    }
    isBurning = 0;
    if ( (pSelf->currentState.eFlags2 & 0x200000) != 0 || ((*((unsigned int *)pSelf + 201) >> 5) & 1) != 0 )
        isBurning = 1;
    Scr_AddBool(isBurning, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CPlayerCmd_HasPerk(scr_entref_t entref)
{
    unsigned int value; // [esp+0h] [ebp-18h]
    char *error; // [esp+8h] [ebp-10h]
    char *perkName; // [esp+Ch] [ebp-Ch]
    cg_s *cGameGlob; // [esp+10h] [ebp-8h]
    unsigned int perkIndex; // [esp+14h] [ebp-4h]

    if ( entref.entnum >= 0x20u )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "hasperk() can only be called on players", 0);
    perkName = Scr_GetString(0, SCRIPTINSTANCE_CLIENT);
    perkIndex = BG_GetPerkIndexForName(perkName);
    if ( perkIndex == BG_PERK_INVALID )
    {
        error = va("Unknown perk: %s\n", perkName);
        Scr_Error(SCRIPTINSTANCE_CLIENT, error, 0);
    }
    cGameGlob = CG_GetLocalClientGlobals(0);
    value = BG_HasPerk(cGameGlob->bgs.clientinfo[entref.entnum].perks, perkIndex);
    Scr_AddBool(value, SCRIPTINSTANCE_CLIENT);
}

static playerState_s *__cdecl CScr_SP_GetPlayerState(scr_entref_t entref)
{
    cg_s *cg;
    const char *fmtMsg;

    if ( entref.classnum )
    {
        Scr_Error(SCRIPTINSTANCE_CLIENT, "not an entity", 0);
        return 0;
    }
    if ( entref.entnum >= 0x20u )
    {
        fmtMsg = va("entity %i is not a player", entref.entnum);
        Scr_Error(SCRIPTINSTANCE_CLIENT, fmtMsg, 0);
        return 0;
    }
    cg = CG_GetLocalClientGlobals(entref.client);
    if ( CG_GetClientNumForLocalClient(entref.client) == entref.entnum )
        return &cg->predictedPlayerState;
    if ( cg->snap && cg->snap->ps.clientNum == entref.entnum )
        return &cg->snap->ps;
    fmtMsg = va("entity %i is not a player", entref.entnum);
    Scr_Error(SCRIPTINSTANCE_CLIENT, fmtMsg, 0);
    return 0;
}

void __cdecl CPlayerCmd_getAngles(scr_entref_t entref)
{
    playerState_s *ps;

    ps = CScr_SP_GetPlayerState(entref);
    if ( ps )
        Scr_AddVector(ps->viewangles, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CPlayerCmd_setAngles(scr_entref_t entref)
{
    playerState_s *ps;
    float angles[3];

    ps = CScr_SP_GetPlayerState(entref);
    if ( !ps )
        return;
    Scr_GetVector(0, angles, SCRIPTINSTANCE_CLIENT);
    ps->viewangles[0] = angles[0];
    ps->viewangles[1] = angles[1];
    ps->viewangles[2] = angles[2];
}

void __cdecl CPlayerCmd_GetVelocity(scr_entref_t entref)
{
    playerState_s *ps;

    ps = CScr_SP_GetPlayerState(entref);
    if ( ps )
        Scr_AddVector(ps->velocity, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_GetVehicleHealth(scr_entref_t entref)
{
    float value; // [esp+8h] [ebp-18h]
    centity_s *pSelf; // [esp+18h] [ebp-8h]

    if ( entref.classnum )
    {
        Scr_Error(SCRIPTINSTANCE_CLIENT, "not an entity", 0);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x600u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                        445,
                        0,
                        "%s",
                        "entref.entnum < MAX_LOCAL_CENTITIES") )
        {
            __debugbreak();
        }
        if ( CG_GetClientNumForLocalClient(entref.client) == entref.entnum )
            pSelf = &CG_GetLocalClientGlobals(entref.client)->predictedPlayerEntity;
        else
            pSelf = CG_GetEntity(entref.client, entref.entnum);
        if ( entref.entnum >= 0x400u )
            CG_GetFakeEntity(entref.client, entref.entnum);
    }
    if ( pSelf->nextState.eType != 14 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "GetVehicleHealth() can only be called on vehicles", 0);
    value = CG_VehGetHealthPercentageEntity(pSelf);
    Scr_AddFloat(value, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_GetLeftTreadHealth(scr_entref_t entref)
{
    float value; // [esp+8h] [ebp-18h]
    centity_s *pSelf; // [esp+18h] [ebp-8h]

    if ( entref.classnum )
    {
        Scr_Error(SCRIPTINSTANCE_CLIENT, "not an entity", 0);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x600u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                        470,
                        0,
                        "%s",
                        "entref.entnum < MAX_LOCAL_CENTITIES") )
        {
            __debugbreak();
        }
        if ( CG_GetClientNumForLocalClient(entref.client) == entref.entnum )
            pSelf = &CG_GetLocalClientGlobals(entref.client)->predictedPlayerEntity;
        else
            pSelf = CG_GetEntity(entref.client, entref.entnum);
        if ( entref.entnum >= 0x400u )
            CG_GetFakeEntity(entref.client, entref.entnum);
    }
    if ( pSelf->nextState.eType != 14 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "GetLeftTreadHealth() can only be called on vehicles", 0);
    value = CG_VehGetHealthPercentageLeftTread(pSelf);
    Scr_AddFloat(value, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_GetHeliDamageState(scr_entref_t entref)
{
    centity_s *pSelf; // [esp+10h] [ebp-8h]

    if ( entref.classnum )
    {
        Scr_Error(SCRIPTINSTANCE_CLIENT, "not an entity", 0);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x600u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                        495,
                        0,
                        "%s",
                        "entref.entnum < MAX_LOCAL_CENTITIES") )
        {
            __debugbreak();
        }
        if ( CG_GetClientNumForLocalClient(entref.client) == entref.entnum )
            pSelf = &CG_GetLocalClientGlobals(entref.client)->predictedPlayerEntity;
        else
            pSelf = CG_GetEntity(entref.client, entref.entnum);
        if ( entref.entnum >= 0x400u )
            CG_GetFakeEntity(entref.client, entref.entnum);
    }
    if ( pSelf->nextState.eType != 12 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "GetHeliDamageState() can only be called on helicopters", 0);
    Scr_AddInt(pSelf->nextState.un1.scale, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_GetRightTreadHealth(scr_entref_t entref)
{
    float value; // [esp+8h] [ebp-18h]
    centity_s *pSelf; // [esp+18h] [ebp-8h]

    if ( entref.classnum )
    {
        Scr_Error(SCRIPTINSTANCE_CLIENT, "not an entity", 0);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x600u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                        520,
                        0,
                        "%s",
                        "entref.entnum < MAX_LOCAL_CENTITIES") )
        {
            __debugbreak();
        }
        if ( CG_GetClientNumForLocalClient(entref.client) == entref.entnum )
            pSelf = &CG_GetLocalClientGlobals(entref.client)->predictedPlayerEntity;
        else
            pSelf = CG_GetEntity(entref.client, entref.entnum);
        if ( entref.entnum >= 0x400u )
            CG_GetFakeEntity(entref.client, entref.entnum);
    }
    if ( pSelf->nextState.eType != 14 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "GetRightTreadHealth() can only be called on vehicles", 0);
    value = CG_VehGetHealthPercentageRightTread(pSelf);
    Scr_AddFloat(value, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_GetInKillcam(scr_entref_t entref)
{
    cg_s *cGameGlob; // [esp+1Ch] [ebp-Ch]
    VariableUnion localClientNum; // [esp+20h] [ebp-8h]

    if ( entref.classnum )
    {
        Scr_Error(SCRIPTINSTANCE_CLIENT, "not an entity", 0);
    }
    else
    {
        if ( entref.entnum >= 0x600u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                        546,
                        0,
                        "%s",
                        "entref.entnum < MAX_LOCAL_CENTITIES") )
        {
            __debugbreak();
        }
        if ( CG_GetClientNumForLocalClient(entref.client) == entref.entnum )
            CG_GetLocalClientGlobals(entref.client);
        else
            CG_GetEntity(entref.client, entref.entnum);
        if ( entref.entnum >= 0x400u )
            CG_GetFakeEntity(entref.client, entref.entnum);
    }
    localClientNum.intValue = CScr_GetLocalClientNum(0);
    cGameGlob = CG_GetLocalClientGlobals(localClientNum.intValue);
    Scr_AddInt(cGameGlob->inKillCam, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_GetAnimState(scr_entref_t entref)
{
    char *value; // [esp+0h] [ebp-18h]
    centity_s *pSelf; // [esp+10h] [ebp-8h]

    if ( entref.classnum )
    {
        Scr_Error(SCRIPTINSTANCE_CLIENT, "not an entity", 0);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x600u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                        569,
                        0,
                        "%s",
                        "entref.entnum < MAX_LOCAL_CENTITIES") )
        {
            __debugbreak();
        }
        if ( CG_GetClientNumForLocalClient(entref.client) == entref.entnum )
            pSelf = &CG_GetLocalClientGlobals(entref.client)->predictedPlayerEntity;
        else
            pSelf = CG_GetEntity(entref.client, entref.entnum);
        if ( entref.entnum >= 0x400u )
            CG_GetFakeEntity(entref.client, entref.entnum);
    }
    if ( pSelf->nextState.eType != 17 && pSelf->nextState.eType != 19 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "GetAnimState() can only be called on actors", 0);
    value = BG_Actor_GetAnimStateName(pSelf->nextState.animState.state);
    Scr_AddString(value, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_GetAnimStateCategory(scr_entref_t entref)
{
    char *value; // [esp+0h] [ebp-18h]
    centity_s *pSelf; // [esp+10h] [ebp-8h]

    if ( entref.classnum )
    {
        Scr_Error(SCRIPTINSTANCE_CLIENT, "not an entity", 0);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x600u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                        593,
                        0,
                        "%s",
                        "entref.entnum < MAX_LOCAL_CENTITIES") )
        {
            __debugbreak();
        }
        if ( CG_GetClientNumForLocalClient(entref.client) == entref.entnum )
            pSelf = &CG_GetLocalClientGlobals(entref.client)->predictedPlayerEntity;
        else
            pSelf = CG_GetEntity(entref.client, entref.entnum);
        if ( entref.entnum >= 0x400u )
            CG_GetFakeEntity(entref.client, entref.entnum);
    }
    if ( pSelf->nextState.eType != 17 && pSelf->nextState.eType != 19 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "GetAnimStateCategory() can only be called on actors", 0);
    value = BG_Actor_GetAnimStateCategoryName(pSelf->nextState.animState.state);
    Scr_AddString(value, SCRIPTINSTANCE_CLIENT);
}

void CScr_GetTotalAmmo()
{
    char *weaponName; // [esp+10h] [ebp-1Ch]
    int localClientNum; // [esp+18h] [ebp-14h]
    int ammoStock; // [esp+1Ch] [ebp-10h]
    int roundsInClip; // [esp+20h] [ebp-Ch]
    unsigned int weapIdx; // [esp+24h] [ebp-8h]
    const playerState_s *ps; // [esp+28h] [ebp-4h]

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) == 2 )
    {
        localClientNum = CScr_GetLocalClientNum(0);
        if ( !CG_GetLocalClientGlobals(localClientNum)
            && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp", 625, 0, "%s", "cgameGlob") )
        {
            __debugbreak();
        }
        weaponName = Scr_GetString(1u, SCRIPTINSTANCE_CLIENT);
        if ( !weaponName
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                        627,
                        0,
                        "%s",
                        "weaponName") )
        {
            __debugbreak();
        }
        ps = CG_GetPredictedPlayerState(localClientNum);
        weapIdx = BG_FindWeaponIndexForName(weaponName);
        roundsInClip = BG_GetAmmoInClip(ps, weapIdx);
        ammoStock = BG_GetTotalAmmoReserve(ps, weapIdx);
        Scr_AddInt(ammoStock + roundsInClip, SCRIPTINSTANCE_CLIENT);
    }
    else
    {
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "Incorrect number of parameters passed to GetTotalAmmo()\nUSAGE: GetTotalAmmo( <localClientNum>, <weaponName> )\n",
            0);
    }
}

void CScr_GetCurrentWeapon()
{
    const cg_s *cgameGlob; // [esp+Ch] [ebp-Ch]
    VariableUnion localClientNum; // [esp+10h] [ebp-8h]
    const WeaponVariantDef *varDef; // [esp+14h] [ebp-4h]

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) == 1 )
    {
        localClientNum.intValue = CScr_GetLocalClientNum(0);
        cgameGlob = CG_GetLocalClientGlobals(localClientNum.intValue);
        if ( !cgameGlob
            && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp", 661, 0, "%s", "cgameGlob") )
        {
            __debugbreak();
        }
        varDef = BG_GetWeaponVariantDef(cgameGlob->predictedPlayerState.weapon);
        if ( !varDef
            && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp", 665, 0, "%s", "varDef") )
        {
            __debugbreak();
        }
        if ( !varDef->szInternalName
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                        666,
                        0,
                        "%s",
                        "varDef->szInternalName") )
        {
            __debugbreak();
        }
        Scr_AddString((char *)varDef->szInternalName, SCRIPTINSTANCE_CLIENT);
    }
    else
    {
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "Incorrect number of parameters passed to GetCurrentWeapon()\nUSAGE: GetCurrentWeapon( <localClientNum> )\n",
            0);
    }
}

void CScr_GetCurrentWeaponIncludingMelee()
{
    const cg_s *cgameGlob; // [esp+Ch] [ebp-14h]
    unsigned __int16 weapon; // [esp+10h] [ebp-10h]
    VariableUnion localClientNum; // [esp+14h] [ebp-Ch]
    const WeaponVariantDef *varDef; // [esp+18h] [ebp-8h]

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) == 1 )
    {
        localClientNum.intValue = CScr_GetLocalClientNum(0);
        cgameGlob = CG_GetLocalClientGlobals(localClientNum.intValue);
        if ( !cgameGlob
            && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp", 698, 0, "%s", "cgameGlob") )
        {
            __debugbreak();
        }
        if ( cgameGlob->predictedPlayerState.weaponstate == 17
            || cgameGlob->predictedPlayerState.weaponstate == 18
            || cgameGlob->predictedPlayerState.weaponstate == 19 )
        {
            weapon = cgameGlob->predictedPlayerState.meleeWeapon;
        }
        else
        {
            weapon = cgameGlob->predictedPlayerState.weapon;
        }
        varDef = BG_GetWeaponVariantDef(weapon);
        if ( !varDef
            && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp", 708, 0, "%s", "varDef") )
        {
            __debugbreak();
        }
        if ( !varDef->szInternalName
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                        709,
                        0,
                        "%s",
                        "varDef->szInternalName") )
        {
            __debugbreak();
        }
        Scr_AddString((char *)varDef->szInternalName, SCRIPTINSTANCE_CLIENT);
    }
    else
    {
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "Incorrect number of parameters passed to GetCurrentWeapon()\nUSAGE: GetCurrentWeapon( <localClientNum> )\n",
            0);
    }
}

void CScr_HasWeapon()
{
    char *weaponName; // [esp+14h] [ebp-10h]
    const cg_s *cgameGlob; // [esp+18h] [ebp-Ch]
    VariableUnion localClientNum; // [esp+1Ch] [ebp-8h]
    unsigned int weapIdx; // [esp+20h] [ebp-4h]

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) == 2 )
    {
        localClientNum.intValue = CScr_GetLocalClientNum(0);
        cgameGlob = CG_GetLocalClientGlobals(localClientNum.intValue);
        weaponName = Scr_GetString(1u, SCRIPTINSTANCE_CLIENT);
        if ( !weaponName
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                        739,
                        0,
                        "%s",
                        "weaponName") )
        {
            __debugbreak();
        }
        if ( !cgameGlob
            && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp", 740, 0, "%s", "cgameGlob") )
        {
            __debugbreak();
        }
        weapIdx = BG_FindWeaponIndexForName(weaponName);
        if ( weapIdx && BG_PlayerHasWeapon(&cgameGlob->predictedPlayerState, weapIdx) )
            Scr_AddBool(1u, SCRIPTINSTANCE_CLIENT);
        else
            Scr_AddBool(0, SCRIPTINSTANCE_CLIENT);
    }
    else
    {
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "Incorrect number of parameters passed to HasWeapon()\nUSAGE: HasWeapon( <localClientNum>, <weaponName> )\n",
            0);
    }
}

void CScr_SetLocalRadarEnabled()
{
    cg_s *cgameGlob; // [esp+Ch] [ebp-8h]
    VariableUnion localClientNum; // [esp+10h] [ebp-4h]

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) == 2 )
    {
        localClientNum.intValue = CScr_GetLocalClientNum(0);
        cgameGlob = CG_GetLocalClientGlobals(localClientNum.intValue);
        if ( Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT) )
            cgameGlob->hasLocalRadar = 1;
        else
            cgameGlob->hasLocalRadar = 0;
    }
    else
    {
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "Incorrect number of parameters passed to SetLocalRadarEnabled()\n"
            "USAGE: SetLocalRadarEnabled( <localClientNum>, <enabled> )\n",
            0);
    }
}

void CScr_SetLocalRadarPosition()
{
    float origin[3]; // [esp+Ch] [ebp-14h] BYREF
    cg_s *cgameGlob; // [esp+18h] [ebp-8h]
    int localClientNum; // [esp+1Ch] [ebp-4h]

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) == 2 )
    {
        localClientNum = CScr_GetLocalClientNum(0);
        cgameGlob = CG_GetLocalClientGlobals(localClientNum);
        Scr_GetVector(1u, origin, SCRIPTINSTANCE_CLIENT);
        *(_QWORD *)cgameGlob->localRadarPos = *(_QWORD *)origin;
    }
    else
    {
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "Incorrect number of parameters passed to SetLocalRadarPosition()\n"
            "USAGE: SetLocalRadarPosition( <localClientNum>, <position> )\n",
            0);
    }
}

void CScr_SetExtraCamEntity()
{
    scr_entref_t v0; // [esp+0h] [ebp-38h] BYREF
    int v1; // [esp+Ah] [ebp-2Eh]
    cg_s *cgameGlob; // [esp+28h] [ebp-10h]
    int localClientNum; // [esp+2Ch] [ebp-Ch]

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) == 2 )
    {
        localClientNum = CScr_GetLocalClientNum(0);
        cgameGlob = CG_GetLocalClientGlobals(localClientNum);
        if ( Scr_GetType(1u, SCRIPTINSTANCE_CLIENT) )
        {
            //v1 = *(unsigned int *)&Scr_GetEntityRef(&v0, 1u, SCRIPTINSTANCE_CLIENT)->entnum;
            v1 = Scr_GetEntityRef(1u, SCRIPTINSTANCE_CLIENT).entnum;
            cgameGlob->extraCamEntity = (unsigned __int16)v1;
        }
        else
        {
            cgameGlob->extraCamEntity = 1023;
        }
    }
    else
    {
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "Incorrect number of parameters passed to SetExtraCamEntity()\nUSAGE: SetExtraCam( <localClientNum>, <entity> )\n",
            0);
    }
}

void CScr_SetExtraCamActive()
{
    cg_s *cgameGlob; // [esp+8h] [ebp-Ch]
    VariableUnion localClientNum; // [esp+Ch] [ebp-8h]

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) == 2 )
    {
        if ( CL_LocalClient_GetActiveCount() <= 1 )
        {
            localClientNum.intValue = CScr_GetLocalClientNum(0);
            cgameGlob = CG_GetLocalClientGlobals(localClientNum.intValue);
            if ( Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT) )
                cgameGlob->extraCamActive = 1;
            else
                cgameGlob->extraCamActive = 0;
        }
    }
    else
    {
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "Incorrect number of parameters passed to SetExtraCamActive()\nUSAGE: SetExtraCam( <localClientNum>, <active> )\n",
            0);
    }
}

void CScr_GetExtraCamStatic()
{
    VariableUnion localClientNum; // [esp+Ch] [ebp-4h]

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) == 1 )
    {
        localClientNum.intValue = CScr_GetLocalClientNum(0);
        if ( CG_GetLocalClientGlobals(localClientNum.intValue)->extraCamStatic )
            Scr_AddInt(1, SCRIPTINSTANCE_CLIENT);
        else
            Scr_AddInt(0, SCRIPTINSTANCE_CLIENT);
    }
    else
    {
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "Incorrect number of parameters passed to GetExtraCamStatic()\nUSAGE: GetExtraCamStatic( <localClientNum> )\n",
            0);
    }
}

void CScr_SetExtraCamStatic()
{
    cg_s *cgameGlob; // [esp+8h] [ebp-Ch]
    VariableUnion localClientNum; // [esp+Ch] [ebp-8h]

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) == 2 )
    {
        if ( CL_LocalClient_GetActiveCount() <= 1 )
        {
            localClientNum.intValue = CScr_GetLocalClientNum(0);
            cgameGlob = CG_GetLocalClientGlobals(localClientNum.intValue);
            if ( Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT) )
                cgameGlob->extraCamStatic = 1;
            else
                cgameGlob->extraCamStatic = 0;
        }
    }
    else
    {
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "Incorrect number of parameters passed to SetExtraCamStatic()\n"
            "USAGE: SetExtraCamStatic( <localClientNum>, <active> )\n",
            0);
    }
}

void CScr_SetExtraCamOrigin()
{
    float *extraCamOrigin; // [esp+0h] [ebp-20h]
    float origin[3]; // [esp+Ch] [ebp-14h] BYREF
    cg_s *cgameGlob; // [esp+18h] [ebp-8h]
    int localClientNum; // [esp+1Ch] [ebp-4h]

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) == 2 )
    {
        localClientNum = CScr_GetLocalClientNum(0);
        cgameGlob = CG_GetLocalClientGlobals(localClientNum);
        Scr_GetVector(1u, origin, SCRIPTINSTANCE_CLIENT);
        extraCamOrigin = cgameGlob->extraCamOrigin;
        cgameGlob->extraCamOrigin[0] = origin[0];
        extraCamOrigin[1] = origin[1];
        extraCamOrigin[2] = origin[2];
    }
    else
    {
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "Incorrect number of parameters passed to SetExtraCamOrigin()\n"
            "USAGE: SetExtraCamOrigin( <localClientNum>, <origin> )\n",
            0);
    }
}

void CScr_SetExtraCamAngles()
{
    cg_s *cgameGlob; // [esp+Ch] [ebp-14h]
    VariableUnion localClientNum; // [esp+10h] [ebp-10h]
    float angles[3]; // [esp+14h] [ebp-Ch] BYREF

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) == 2 )
    {
        localClientNum.intValue = CScr_GetLocalClientNum(0);
        cgameGlob = CG_GetLocalClientGlobals(localClientNum.intValue);
        Scr_GetVector(1u, angles, SCRIPTINSTANCE_CLIENT);
        cgameGlob->extraCamAngles[0] = angles[0];
        cgameGlob->extraCamAngles[1] = angles[1];
        cgameGlob->extraCamAngles[2] = angles[2];
    }
    else
    {
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "Incorrect number of parameters passed to SetExtraCamAngles()\n"
            "USAGE: SetExtraCamAngles( <localClientNum>, <angles> )\n",
            0);
    }
}

void CScr_IsCameraSpikeToggled()
{
    VariableUnion localClientNum; // [esp+Ch] [ebp-4h]

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) == 1 )
    {
        localClientNum.intValue = CScr_GetLocalClientNum(0);
        if ( (CG_GetLocalClientGlobals(localClientNum.intValue)->predictedPlayerState.weapFlags & 0x200000) != 0 )
            Scr_AddInt(1, SCRIPTINSTANCE_CLIENT);
        else
            Scr_AddInt(0, SCRIPTINSTANCE_CLIENT);
    }
    else
    {
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "Incorrect number of parameters passed to IsCameraSpikeToggled\nUSAGE: IsCameraSpikeToggled( <localClientNum> )\n",
            0);
    }
}

// Client volumetric fog — congruent with retail BlackOpsMP sub_7EF1F0 / setClientVolFog
// and Scr_SetVolumetricFog (server) → R_SetFogFromServer + R_SwitchFog path in CG_ParseFog.

void CScr_SetClientVolumetricFog()
{
    float v0;
    float v1;
    float sunDirY;
    float green;
    float startDist;
    float baseHeight;
    float blue;
    float sunStartAng;
    float red;
    float fogColorScale;
    float maxFogOpacity;
    float density;
    float time;
    float sunColorB;
    float halfwayDist;
    float sunDirZ;
    float halfwayHeight;
    float sunStopAng;
    float sunColorR;
    float sunColorG;
    float sunDirX;
    int lcn;
    cg_s *cgameGlob;

    sunColorR = 0.5f;
    sunColorG = 0.5f;
    sunColorB = 0.5f;
    sunDirX = 1.0f;
    sunDirY = 0.0f;
    sunDirZ = 0.0f;
    sunStartAng = 0.0f;
    sunStopAng = 0.0f;
    maxFogOpacity = 1.0f;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 8 && Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 18 )
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "Incorrect number of parameters\n"
            "USAGE: setClientVolFog(<startDist>, <halfwayDist>, <halfwayHeight>, <baseHeight>, <red>, <green>, <blue>, <transiti"
            "on time>)\n"
            "OR:     setClientVolFog(<startDist>, <halfwayDist>, <halfwayHeight>, <baseHeight>, <red>, <green>, <blue>, <fogColor"
            "Scale>, <sunFogRed>, <sunFogGreen>, <sunFogBlue>, <sunFogDirX>, <sunFogDirY>, <sunFogDirZ>, <sunFogStartAng>, <sunF"
            "ogEndAng>, <transition time>, <max fog opacity>)\n",
            0);
    startDist = Scr_GetFloat(0, SCRIPTINSTANCE_CLIENT);
    if ( startDist < 0.0 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "setClientVolFog: startDist must be greater or equal to 0", 0);
    halfwayDist = Scr_GetFloat(1u, SCRIPTINSTANCE_CLIENT);
    if ( halfwayDist <= 0.0 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "setClientVolFog: halfwayDist must be greater than 0", 0);
    halfwayHeight = Scr_GetFloat(2u, SCRIPTINSTANCE_CLIENT);
    if ( halfwayHeight < 0.0 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "setClientVolFog: halfwayHeight must be greater or equal to 0", 0);
    baseHeight = Scr_GetFloat(3u, SCRIPTINSTANCE_CLIENT);
    density = 1.0f / halfwayDist;
    if ( halfwayHeight < 1.0f )
        v1 = 0.0f;
    else
        v1 = 1.0f / halfwayHeight;
    red = Scr_GetFloat(4u, SCRIPTINSTANCE_CLIENT);
    green = Scr_GetFloat(5u, SCRIPTINSTANCE_CLIENT);
    blue = Scr_GetFloat(6u, SCRIPTINSTANCE_CLIENT);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) == 18 )
    {
        fogColorScale = Scr_GetFloat(7u, SCRIPTINSTANCE_CLIENT);
        sunColorR = Scr_GetFloat(8u, SCRIPTINSTANCE_CLIENT);
        sunColorG = Scr_GetFloat(9u, SCRIPTINSTANCE_CLIENT);
        sunColorB = Scr_GetFloat(0xAu, SCRIPTINSTANCE_CLIENT);
        sunDirX = Scr_GetFloat(0xBu, SCRIPTINSTANCE_CLIENT);
        sunDirY = Scr_GetFloat(0xCu, SCRIPTINSTANCE_CLIENT);
        sunDirZ = Scr_GetFloat(0xDu, SCRIPTINSTANCE_CLIENT);
        sunStartAng = Scr_GetFloat(0xEu, SCRIPTINSTANCE_CLIENT);
        sunStopAng = Scr_GetFloat(0xFu, SCRIPTINSTANCE_CLIENT);
        time = Scr_GetFloat(0x10u, SCRIPTINSTANCE_CLIENT);
        maxFogOpacity = Scr_GetFloat(0x11u, SCRIPTINSTANCE_CLIENT);
    }
    else
    {
        Com_Printf(1, "setClientVolFog: Old syntax used. Please update script.\n");
        if ( green <= red )
            v0 = red;
        else
            v0 = green;
        fogColorScale = v0;
        if ( blue > v0 )
            fogColorScale = blue;
        red = red * (float)(1.0 / fogColorScale);
        green = green * (float)(1.0 / fogColorScale);
        blue = blue * (float)(1.0 / fogColorScale);
        time = Scr_GetFloat(7u, SCRIPTINSTANCE_CLIENT);
    }
    if ( (density <= 0.0 || density > 1.0)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                    0,
                    0,
                    "%s\n\t(density) = %g",
                    "(density > 0 && density <= 1)",
                    density) )
    {
        __debugbreak();
    }
    if ( (v1 < 0.0 || v1 > 1.0)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                    0,
                    0,
                    "%s\n\t(heightDensity) = %g",
                    "(heightDensity >= 0 && heightDensity <= 1)",
                    v1) )
    {
        __debugbreak();
    }
    // setClientVolFog (retail) has no localClientNum; first script arg is startDist (float).
    lcn = 0;
    cgameGlob = CG_GetLocalClientGlobals(lcn);
    R_SetFogFromServer(
        lcn,
        startDist,
        red,
        green,
        blue,
        density,
        v1,
        baseHeight,
        fogColorScale,
        sunColorR,
        sunColorG,
        sunColorB,
        sunDirX,
        sunDirY,
        sunDirZ,
        sunStartAng,
        sunStopAng,
        maxFogOpacity);
    s_clientVolFog.valid = 1;
    s_clientVolFog.start = startDist;
    s_clientVolFog.r = red;
    s_clientVolFog.g = green;
    s_clientVolFog.b = blue;
    s_clientVolFog.density = density;
    s_clientVolFog.heightDensity = v1;
    s_clientVolFog.baseHeight = baseHeight;
    s_clientVolFog.fogColorScale = fogColorScale;
    s_clientVolFog.sunColR = sunColorR;
    s_clientVolFog.sunColG = sunColorG;
    s_clientVolFog.sunColB = sunColorB;
    s_clientVolFog.sunDirX = sunDirX;
    s_clientVolFog.sunDirY = sunDirY;
    s_clientVolFog.sunDirZ = sunDirZ;
    s_clientVolFog.sunStartAng = sunStartAng;
    s_clientVolFog.sunEndAng = sunStopAng;
    s_clientVolFog.maxFogOpacity = maxFogOpacity;
    s_clientVolFog.transitionMsec = (int)(float)(time * 1000.0f + 9.313225746154785e-10);
    R_SwitchFog(lcn, 1u, cgameGlob->time, s_clientVolFog.transitionMsec);
}

void CScr_SwitchToServerVolumetricFog()
{
    int lcn;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 0 )
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "USAGE: switchToServerVolFog()\n",
            0);
    lcn = 0;
    CG_ParseFog(lcn);
}

void CScr_SwitchToClientVolumetricFog()
{
    int lcn;
    cg_s *cgameGlob;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 0 )
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "USAGE: switchToClientVolFog()\n",
            0);
    if ( !s_clientVolFog.valid )
        return;
    lcn = 0;
    cgameGlob = CG_GetLocalClientGlobals(lcn);
    R_SetFogFromServer(
        lcn,
        s_clientVolFog.start,
        s_clientVolFog.r,
        s_clientVolFog.g,
        s_clientVolFog.b,
        s_clientVolFog.density,
        s_clientVolFog.heightDensity,
        s_clientVolFog.baseHeight,
        s_clientVolFog.fogColorScale,
        s_clientVolFog.sunColR,
        s_clientVolFog.sunColG,
        s_clientVolFog.sunColB,
        s_clientVolFog.sunDirX,
        s_clientVolFog.sunDirY,
        s_clientVolFog.sunDirZ,
        s_clientVolFog.sunStartAng,
        s_clientVolFog.sunEndAng,
        s_clientVolFog.maxFogOpacity);
    R_SwitchFog(lcn, 1u, cgameGlob->time, s_clientVolFog.transitionMsec);
}

void CScr_IsInHelicopter()
{
    int lcn;
    cg_s *cg;
    centity_s *cent;
    const WeaponVariantDef *wv;
    int ge;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 0 )
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "USAGE: isInHelicopter()\n",
            0);
    lcn = 0;
    cg = CG_GetLocalClientGlobals(lcn);
    wv = BG_GetWeaponVariantDef(cg->predictedPlayerState.weapon);
    if ( wv && !I_stricmp(wv->szInternalName, "helicopter_gunner_mp") )
    {
        Scr_AddInt(1, SCRIPTINSTANCE_CLIENT);
        return;
    }
    ge = cg->predictedPlayerState.groundEntityNum;
    if ( ge != 1023 && ge != 1022 && (unsigned int)ge < 1024u )
    {
        cent = CG_GetEntity(lcn, ge);
        if ( cent && cent->nextState.eType == ET_HELICOPTER )
        {
            Scr_AddInt(1, SCRIPTINSTANCE_CLIENT);
            return;
        }
    }
    Scr_AddInt(0, SCRIPTINSTANCE_CLIENT);
}

// Decomp: CoDSP_rdBlackOps.map.c (CScr_IsADS) @ 6845049
void CScr_IsADS()
{
    unsigned int localClientNum;
    const playerState_s *ps;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 1 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "IsADS() called with wrong params.\n", 0);
    localClientNum = CScr_GetLocalClientNum(0);
    if ( (unsigned int)localClientNum >= 2
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_scr_main_sp.cpp",
                    1038,
                    0,
                    "localClientNum not in [0, MAX_LOCAL_CLIENTS]\n\t%i not in [%i, %i]",
                    localClientNum,
                    0,
                    1) )
    {
        __debugbreak();
    }
    if ( !CL_IsLocalClientInGame(localClientNum)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_scr_main_sp.cpp",
                    1039,
                    0,
                    "%s",
                    "CL_IsLocalClientInGame( localClientNum ) == qtrue") )
    {
        __debugbreak();
    }
    ps = CG_GetPredictedPlayerState(localClientNum);
    Scr_AddBool(ps->fWeaponPosFrac > 0.0f, SCRIPTINSTANCE_CLIENT);
}

// Decomp: CoDSP_rdBlackOps.map.c (CScrCmd_GetWeaponAmmoClip) @ 6845137
void CScr_GetWeaponAmmoClip()
{
    unsigned int localClientNum;
    const playerState_s *ps;
    char *weaponName;
    unsigned int weapIdx;
    int ammoInClip;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 2 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "GetWeaponAmmoClip() called with wrong params.\n", 0);
    localClientNum = CScr_GetLocalClientNum(0);
    if ( (unsigned int)localClientNum >= 2
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_scr_main_sp.cpp",
                    1068,
                    0,
                    "localClientNum not in [0, MAX_LOCAL_CLIENTS]\n\t%i not in [%i, %i]",
                    localClientNum,
                    0,
                    1) )
    {
        __debugbreak();
    }
    if ( !CL_IsLocalClientInGame(localClientNum)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_scr_main_sp.cpp",
                    1069,
                    0,
                    "%s",
                    "CL_IsLocalClientInGame( localClientNum ) == qtrue") )
    {
        __debugbreak();
    }
    weaponName = Scr_GetString(1u, SCRIPTINSTANCE_CLIENT);
    weapIdx = BG_FindWeaponIndexForName(weaponName);
    ps = CG_GetPredictedPlayerState(localClientNum);
    ammoInClip = BG_GetAmmoInClip(ps, weapIdx);
    Scr_AddInt(ammoInClip, SCRIPTINSTANCE_CLIENT);
}

// Decomp: CoDSP_rdBlackOps.map.c (CScrCmd_IsThrowingGrenade) @ 6845230
void CScr_IsThrowingGrenade()
{
    unsigned int localClientNum;
    const playerState_s *ps;
    bool throwing;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 1 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "IsThrowingGrenade() called with wrong params.\n", 0);
    localClientNum = CScr_GetLocalClientNum(0);
    if ( (unsigned int)localClientNum >= 2
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_scr_main_sp.cpp",
                    1099,
                    0,
                    "localClientNum not in [0, MAX_LOCAL_CLIENTS]\n\t%i not in [%i, %i]",
                    localClientNum,
                    0,
                    1) )
    {
        __debugbreak();
    }
    if ( !CL_IsLocalClientInGame(localClientNum)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_scr_main_sp.cpp",
                    1100,
                    0,
                    "%s",
                    "CL_IsLocalClientInGame( localClientNum ) == qtrue") )
    {
        __debugbreak();
    }
    ps = CG_GetPredictedPlayerState(localClientNum);
    throwing = ps->weaponstate >= WEAPON_OFFHAND_INIT && ps->weaponstate <= WEAPON_OFFHAND_END;
    Scr_AddBool(throwing, SCRIPTINSTANCE_CLIENT);
}

// Decomp: CoDSP_rdBlackOps.map.c (CScrCmd_IsMeleeing) @ 6845319
void CScr_IsMeleeing()
{
    unsigned int localClientNum;
    const playerState_s *ps;
    bool meleeing;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 1 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "IsMeleeing() called with wrong params.\n", 0);
    localClientNum = CScr_GetLocalClientNum(0);
    if ( (unsigned int)localClientNum >= 2
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_scr_main_sp.cpp",
                    1130,
                    0,
                    "localClientNum not in [0, MAX_LOCAL_CLIENTS]\n\t%i not in [%i, %i]",
                    localClientNum,
                    0,
                    1) )
    {
        __debugbreak();
    }
    if ( !CL_IsLocalClientInGame(localClientNum)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_scr_main_sp.cpp",
                    1131,
                    0,
                    "%s",
                    "CL_IsLocalClientInGame( localClientNum ) == qtrue") )
    {
        __debugbreak();
    }
    ps = CG_GetPredictedPlayerState(localClientNum);
    meleeing = ps->weaponstate == WEAPON_MELEE_FIRE
        || ps->weaponstate == WEAPON_MELEE_END
        || ps->weaponstate == WEAPON_OFFHAND_INIT;
    Scr_AddBool(meleeing, SCRIPTINSTANCE_CLIENT);
}

// Decomp: CoDSP_rdBlackOps.map.c (CScrCmd_IsOnTurret) @ 6845503
void CScr_IsOnTurret()
{
    unsigned int localClientNum;
    const playerState_s *ps;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 1 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "isonturret() called with wrong params.\n", 0);
    localClientNum = CScr_GetLocalClientNum(0);
    if ( (unsigned int)localClientNum >= 2
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_scr_main_sp.cpp",
                    1161,
                    0,
                    "localClientNum not in [0, MAX_LOCAL_CLIENTS]\n\t%i not in [%i, %i]",
                    localClientNum,
                    0,
                    1) )
    {
        __debugbreak();
    }
    if ( !CL_IsLocalClientInGame(localClientNum)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_scr_main_sp.cpp",
                    1162,
                    0,
                    "%s",
                    "CL_IsLocalClientInGame( localClientNum ) == qtrue") )
    {
        __debugbreak();
    }
    ps = CG_GetPredictedPlayerState(localClientNum);
    Scr_AddBool((ps->eFlags & 0x300) != 0, SCRIPTINSTANCE_CLIENT);
}

// Decomp: CoDSP_rdBlackOps.map.c (CScr_CreateDynEntAndLaunch) @ 6847017
void CScr_CreateDynEntAndLaunch()
{
    int localClientNum;
    char *modelName;
    int modelIndex;
    cgs_t *cgs;
    XModel *model;
    float pos[3];
    float angles[3];
    float hitpos[3];
    float force[3];
    float quat[4];
    int fxId;
    int mature;
    unsigned __int16 dynEntId;
    const FxEffectDef *fxDef;

    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) < 6 )
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "CreateDynEntAndLaunch called with invalid params. CreateDynEntAndLaunch( <local client num>, <model>, <pos>, "
            "<angles>, <hitpos>, <force>, <fx>, <mature> )",
            0);
    fxId = 0;
    mature = 0;
    localClientNum = Scr_GetInt(0, SCRIPTINSTANCE_CLIENT);
    modelName = Scr_GetString(1u, SCRIPTINSTANCE_CLIENT);
    modelIndex = CG_GetModelIndex(modelName, localClientNum);
    if ( modelIndex < 0 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, va("model '%s' not precached\n", modelName), 0);
    Scr_GetVector(2u, pos, SCRIPTINSTANCE_CLIENT);
    Scr_GetVector(3u, angles, SCRIPTINSTANCE_CLIENT);
    Scr_GetVector(4u, hitpos, SCRIPTINSTANCE_CLIENT);
    Scr_GetVector(5u, force, SCRIPTINSTANCE_CLIENT);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) > 6 )
        fxId = Scr_GetInt(6u, SCRIPTINSTANCE_CLIENT);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) > 7 )
        mature = Scr_GetInt(7u, SCRIPTINSTANCE_CLIENT);
    if ( !mature
        || loc_language->current.integer != 3
            && ((loc_language->current.integer != 11 && loc_language->current.integer != 13)
                || zombiemode->current.enabled) )
    {
        cgs = CG_GetLocalClientStaticGlobals(localClientNum);
        model = cgs->gameModels[modelIndex];
        AnglesToQuat(angles, quat);
        dynEntId = DynEntCl_CreateEntityModel(model, quat, pos, hitpos, force, 0, 0);
        if ( fxId )
        {
            fxDef = cgs->fxs[fxId];
            if ( !fxDef
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\cg_scr_main_sp.cpp",
                            1230,
                            0,
                            "%s",
                            "fxDef") )
            {
                __debugbreak();
            }
            DynEntCl_PlayBoltedFX(fxDef, dynEntId);
        }
    }
}

void CScr_GetGridFromPos()
{
    VariableUnion v0; // eax
    float pos[3]; // [esp+8h] [ebp-14h] BYREF
    char gridName[4]; // [esp+14h] [ebp-8h] BYREF
    int argc; // [esp+18h] [ebp-4h]

    argc = Scr_GetNumParam(SCRIPTINSTANCE_CLIENT);
    if ( argc != 2 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "CGScr_GetGridByPos( <clientNum> <pos> ) takes 2 parameters", 0);
    Scr_GetVector(1u, pos, SCRIPTINSTANCE_CLIENT);
    v0.intValue = CScr_GetLocalClientNum(0);
    CG_GetGridFromPos(v0.intValue, pos, gridName);
    Scr_AddString(gridName, SCRIPTINSTANCE_CLIENT);
}

void CScr_CompassScale()
{
    VariableUnion duration; // [esp+0h] [ebp-Ch]
    VariableUnion difference; // [esp+4h] [ebp-8h]

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 2 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "CScr_CompassScale( <diff> <dur> ) takes 2 parameters", 0);
    difference.intValue = Scr_GetInt(0, SCRIPTINSTANCE_CLIENT);
    duration.intValue = Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT);
    Dvar_SetInt((dvar_s *)compassScaleDiff, difference.intValue);
    Dvar_SetInt((dvar_s *)compassScaleDuration, duration.intValue);
}

void CScr_ResetCompassScale()
{
    VariableUnion duration; // [esp+0h] [ebp-8h]

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 1 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "CScr_ResetCompassScale( <dur> ) takes 1 parameter", 0);
    duration.intValue = Scr_GetInt(0, SCRIPTINSTANCE_CLIENT);
    Dvar_SetBool((dvar_s *)compassScaleReset, 1);
    Dvar_SetInt((dvar_s *)compassScaleDuration, duration.intValue);
}

void __cdecl CScr_GetLocalPlayerTeam()
{
    cg_s *cGameGlob; // [esp+8h] [ebp-Ch]
    int localClientNum; // [esp+10h] [ebp-4h]

    localClientNum = 0;
    if ( CL_LocalClient_IsActive(localClientNum) )
    {
        cGameGlob = CG_GetLocalClientGlobals(localClientNum);
        CScr_AddTeamName(cGameGlob->bgs.clientinfo[cGameGlob->clientNum].team);
    }
    else
    {
        Scr_AddUndefined(SCRIPTINSTANCE_CLIENT);
    }
}

void __cdecl CScr_AddTeamName(team_t team)
{
    switch ( team )
    {
        case TEAM_FREE:
            Scr_AddString("free", SCRIPTINSTANCE_CLIENT);
            break;
        case TEAM_AXIS:
            Scr_AddString("axis", SCRIPTINSTANCE_CLIENT);
            break;
        case TEAM_ALLIES:
            Scr_AddString("allies", SCRIPTINSTANCE_CLIENT);
            break;
        case TEAM_SPECTATOR:
            Scr_AddString("spectator", SCRIPTINSTANCE_CLIENT);
            break;
        default:
            return;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (CScrCmd_GetLocalClientNumber)
void __cdecl CScrCmd_GetLocalClientNumber(scr_entref_t entref)
{
    int i;
    cg_s *cgameGlob;

    if ( !CG_IsLocalPlayer(entref.entnum) )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "You can only call GetLocalClientNumber on players.", 0);
    for ( i = 0; i < 4; ++i )
    {
        if ( CL_LocalClient_IsActive(i) )
        {
            cgameGlob = CG_GetLocalClientGlobals(i);
            if ( cgameGlob->nextSnap && cgameGlob->nextSnap->ps.clientNum == entref.entnum )
            {
                Scr_AddInt(i, SCRIPTINSTANCE_CLIENT);
                return;
            }
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (CScrCmd_HasEyes ~8294AD00)
// Map: CoDSP_rd.map VA 0x8294AD00 (cg_scr_main_coop.obj)
void __cdecl CScrCmd_HasEyes(scr_entref_t entref)
{
    centity_s *cent;

    cent = CG_GetEntity(entref.client, entref.entnum);
    if ( (cent->nextState.lerp.eFlags & 0x20000) != 0 )
        Scr_AddInt(1, SCRIPTINSTANCE_CLIENT);
    else
        Scr_AddInt(0, SCRIPTINSTANCE_CLIENT);
}

// Decomp: CoDSP_rdBlackOps.map.c (CScrCmd_SetBlur ~8294B3C0)
// Map: CoDSP_rd.map VA 0x8294B3C0 (cg_scr_main_coop.obj)
void __cdecl CScrCmd_SetBlur(scr_entref_t entref)
{
    int blurTimeMs;
    float blurValue;
    int i;
    cg_s *cgameGlob;

    blurValue = Scr_GetFloat(0, SCRIPTINSTANCE_CLIENT);
    blurTimeMs = (int)(float)(Scr_GetFloat(1u, SCRIPTINSTANCE_CLIENT) * 1000.0f);
    if ( blurTimeMs < 0 )
        Scr_ParamError(1u, "Time must be positive", SCRIPTINSTANCE_CLIENT);
    if ( blurValue < 0.0f )
        Scr_ParamError(0, "Blur value must be greater than 0", SCRIPTINSTANCE_CLIENT);
    if ( !CG_IsLocalPlayer(entref.entnum) )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "You can only call GetLocalClientNumber on players.", 0);
    for ( i = 0; i < 4; ++i )
    {
        if ( CL_LocalClient_IsActive(i) )
        {
            cgameGlob = CG_GetLocalClientGlobals(i);
            if ( cgameGlob->nextSnap && cgameGlob->nextSnap->ps.clientNum == entref.entnum )
            {
                iassert(blurTimeMs >= 0);
                iassert(blurValue >= 0.0f);
                CG_Blur(i, blurTimeMs, 0, entref.entnum, blurValue);
                return;
            }
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (CScrCmd_PlayerSetGroundReferenceEnt ~82949688)
// Map: CoDSP_rd.map VA 0x82949688 (cg_scr_main_coop.obj)
void __cdecl CScrCmd_PlayerSetGroundReferenceEnt(scr_entref_t entref)
{
    centity_s *cent;
    cg_s *cgameGlob;
    int refEntNum;

    (void)entref;
    cent = CG_GetEntity(entref.client, entref.entnum);
    if ( !cent->nextState.clientNum && cent->nextState.eType != ET_PLAYER )
        Scr_ObjectError("not a player entity", SCRIPTINSTANCE_CLIENT);
    cgameGlob = CG_GetLocalClientGlobals(entref.client);
    if ( Scr_GetType(0, SCRIPTINSTANCE_CLIENT) )
    {
        if ( Scr_GetType(0, SCRIPTINSTANCE_CLIENT) != 1 || Scr_GetPointerType(0, SCRIPTINSTANCE_CLIENT) != 19 )
            Scr_ParamError(0, "not an entity", SCRIPTINSTANCE_CLIENT);
        refEntNum = Scr_GetEntityRef(0, SCRIPTINSTANCE_CLIENT).entnum;
    }
    else
    {
        refEntNum = ENTITYNUM_NONE;
    }
    cgameGlob->predictedPlayerState.groundEntityNum = refEntNum;
}

void __cdecl CScr_GetDynEnt()
{
    unsigned __int16 targetname;
    unsigned __int16 absId;
    const char *name;

    targetname = (unsigned __int16)CScr_GetConstServerString(0);
    absId = DynEnt_GetDynEntityByName(targetname);
    if ( absId != 0xFFFF )
    {
        Scr_AddEntityNum(absId, 4, SCRIPTINSTANCE_CLIENT, 0);
        return;
    }
    name = SL_ConvertToString(targetname, SCRIPTINSTANCE_CLIENT);
    Scr_Error(SCRIPTINSTANCE_CLIENT, va("getdynent, failed to find dyn ent with targetname %s.", name), 0);
}

void __cdecl CScr_GetDynEntArray()
{
    unsigned __int16 targetname;

    targetname = 0xFFFF;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) )
        targetname = (unsigned __int16)CScr_GetConstServerString(0);
    SCR_DynEnt_GetDynEntityArray(targetname);
}

void __cdecl CScr_GetLinkedEnt(scr_entref_t entref)
{
    centity_s *cent;
    int attachedEntNum;

    cent = CScr_SP_GetMethodEntity(entref);
    if ( !cent || !cent->nextValid )
    {
        Scr_AddUndefined(SCRIPTINSTANCE_CLIENT);
        return;
    }
    attachedEntNum = GetAttachedEntNum(entref.client, cent);
    if ( attachedEntNum == 1023 )
    {
        Scr_AddUndefined(SCRIPTINSTANCE_CLIENT);
        return;
    }
    CScr_AddEntity(CG_GetEntity(entref.client, attachedEntNum), entref.client);
}

void __cdecl CScr_GetLocalClientHealth()
{
    int localClientNum;
    const playerState_s *ps;

    localClientNum = CScr_GetLocalClientNum(0);
    if ( (unsigned int)localClientNum >= MAX_LOCAL_CLIENTS || !CL_IsLocalClientInGame(localClientNum) )
        return;
    ps = CG_GetPredictedPlayerState(localClientNum);
    Scr_AddInt(ps->stats[0], SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_GetLocalClientMaxHealth()
{
    int localClientNum;
    const playerState_s *ps;

    localClientNum = CScr_GetLocalClientNum(0);
    if ( (unsigned int)localClientNum >= MAX_LOCAL_CLIENTS || !CL_IsLocalClientInGame(localClientNum) )
        return;
    ps = CG_GetPredictedPlayerState(localClientNum);
    Scr_AddInt(ps->stats[2], SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_GetPersistentProfileVar()
{
    unsigned int index;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 1 )
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "GetPersistentProfileVar() called with wrong params.\n",
            0);
    index = Scr_GetInt(0, SCRIPTINSTANCE_CLIENT);
    if ( index >= 8 )
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "GetPersistentProfileVar() called with wrong param range. Index must be 0 to MISSION_GLOBAL_MAX_DVARS.\n",
            0);
    CScr_SP_RegisterMissionGlobals();
    if ( s_missionGlobals[index] )
        Scr_AddInt(s_missionGlobals[index]->current.integer, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_SetPersistentProfileVar()
{
    unsigned int index;
    unsigned int value;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 2 )
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "SetPersistentProfileVar() called with wrong params.\n",
            0);
    index = Scr_GetInt(0, SCRIPTINSTANCE_CLIENT);
    value = Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT);
    if ( index >= 8 )
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "SetPersistentProfileVar() called with wrong param range. Index must be 0 to MISSION_GLOBAL_MAX_DVARS.\n",
            0);
    if ( value >= 0x100 )
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "SetPersistentProfileVar() called with wrong param range. Value must be 0 to 255.\n",
            0);
    CScr_SP_RegisterMissionGlobals();
    if ( s_missionGlobals[index] )
        Dvar_SetInt((dvar_s *)s_missionGlobals[index], value);
}

// Decomp: CoDSP_rdBlackOps.map.c @ 82946650
void __cdecl CScr_SetClientDvar()
{
    char *dvarName;
    char *value;

    dvarName = Scr_GetString(0, SCRIPTINSTANCE_CLIENT);
    if ( !dvarName || !*dvarName )
        Scr_ParamError(0, "SetClientDvar: unknown dvar name", SCRIPTINSTANCE_CLIENT);
    value = Scr_GetString(1u, SCRIPTINSTANCE_CLIENT);
    if ( !value || !*value )
        Scr_ParamError(1u, "SetClientDvar: unknown dvar value", SCRIPTINSTANCE_CLIENT);
    Dvar_SetFromStringByName(dvarName, value);
}

void __cdecl CScr_HasCollectible()
{
    int collectibleIndex;
    bool hasCollectible;

    collectibleIndex = Scr_GetInt(0, SCRIPTINSTANCE_CLIENT);
    hasCollectible = BG_HasCollectible(collectibleIndex, 0, 0, 0, 0, 0, 0);
    Scr_AddBool(hasCollectible, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_SetCollectible()
{
    int collectibleIndex;

    collectibleIndex = Scr_GetInt(0, SCRIPTINSTANCE_CLIENT);
    BG_SetCollectible(collectibleIndex, 0, 0, 0, 0);
}

void __cdecl CScr_SP_GetMeansOfDeathConstString()
{
    int mod;

    mod = Scr_GetInt(0, SCRIPTINSTANCE_CLIENT);
    Scr_AddConstString(CScr_GetMeansOfDeathConstString(mod), SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_SP_GetEntAnimTree()
{
    scr_entref_t entref;
    centity_s *cent;
    XAnimTree_s *tree;

    entref = Scr_GetEntityRef(0, SCRIPTINSTANCE_CLIENT);
    cent = CScr_SP_GetMethodEntity(entref);
    if ( !cent )
    {
        Scr_AddUndefined(SCRIPTINSTANCE_CLIENT);
        return;
    }
    tree = CScr_GetEntAnimTree(cent);
    Scr_AddInt((int)tree, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_SP_ExecEntThread()
{
    scr_entref_t entref;
    centity_s *cent;
    int handle;
    unsigned int paramcount;
    unsigned __int16 thread;

    entref = Scr_GetEntityRef(0, SCRIPTINSTANCE_CLIENT);
    cent = CScr_SP_GetMethodEntity(entref);
    handle = Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT);
    paramcount = Scr_GetNumParam(SCRIPTINSTANCE_CLIENT);
    if ( paramcount < 2 )
        paramcount = 0;
    else
        paramcount -= 2;
    thread = CScr_ExecEntThread(cent, handle, paramcount);
    Scr_AddInt(thread, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_SP_PlayWeaponDamageEffects()
{
    unsigned int localClientNum;
    int entnum;
    unsigned int weaponIndex;
    int mod;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 4 )
        return;
    localClientNum = Scr_GetInt(0, SCRIPTINSTANCE_CLIENT);
    entnum = Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT);
    weaponIndex = Scr_GetInt(2u, SCRIPTINSTANCE_CLIENT);
    mod = Scr_GetInt(3u, SCRIPTINSTANCE_CLIENT);
    CScr_PlayWeaponDamageEffectsInternal(localClientNum, entnum, weaponIndex, mod);
}

void __cdecl CScr_UncrunchArray()
{
    const char *stringValue;
    unsigned int i;
    int j;
    char c;

    stringValue = Scr_GetString(0, SCRIPTINSTANCE_CLIENT);
    Scr_MakeArray(SCRIPTINSTANCE_CLIENT);
    for ( i = 0; i < strlen(stringValue); ++i )
    {
        c = stringValue[i] - 33;
        for ( j = 0; j < 6; ++j )
        {
            if ( (c & (1 << j)) != 0 )
            {
                Scr_AddInt((i << 6) + (1 << j), SCRIPTINSTANCE_CLIENT);
                Scr_AddArray(SCRIPTINSTANCE_CLIENT);
            }
        }
    }
}

void __cdecl CScr_WaterPlop()
{
    int localClientNum;
    float pos[3];
    unsigned int size;
    float height;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 4 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "waterplop() called with wrong params.\n", 0);
    localClientNum = CScr_GetLocalClientNum(0);
    Scr_GetVector(1u, pos, SCRIPTINSTANCE_CLIENT);
    size = Scr_GetInt(2u, SCRIPTINSTANCE_CLIENT);
    height = Scr_GetFloat(3u, SCRIPTINSTANCE_CLIENT);
    if ( CL_LocalClient_IsFirstActive(localClientNum) )
        R_WaterSimulationNotifyExplosionEx(pos, size, height);
}

void __cdecl CScr_SetSunLight()
{
    float diffuse[3];
    float specular[3];

    diffuse[0] = Scr_GetFloat(0, SCRIPTINSTANCE_CLIENT);
    diffuse[1] = Scr_GetFloat(1u, SCRIPTINSTANCE_CLIENT);
    diffuse[2] = Scr_GetFloat(2u, SCRIPTINSTANCE_CLIENT);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) == 6 )
    {
        specular[0] = Scr_GetFloat(3u, SCRIPTINSTANCE_CLIENT);
        specular[1] = Scr_GetFloat(4u, SCRIPTINSTANCE_CLIENT);
        specular[2] = Scr_GetFloat(5u, SCRIPTINSTANCE_CLIENT);
    }
    else
    {
        specular[0] = diffuse[0];
        specular[1] = diffuse[1];
        specular[2] = diffuse[2];
    }
    R_SetSunLightOverride(diffuse, specular);
}

void __cdecl CScr_ResetSunLight()
{
    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "Incorrect number of parameters\n", 0);
    R_ResetSunLightOverride();
}

// Decomp: CoDSP_rdBlackOps.map.c (CScr_GetVisionSetNaked ~829463A8)
void __cdecl CScr_GetVisionSetNaked()
{
    int localClientNum;
    cg_s *cg;

    localClientNum = Scr_GetInt(0, SCRIPTINSTANCE_CLIENT);
    cg = CG_GetLocalClientGlobals(localClientNum);
    Scr_AddString(cg->visionNameNaked, SCRIPTINSTANCE_CLIENT);
}

// Decomp: BlackOps.singleplayer.c sub_8948E0
void __cdecl CScr_VisionSetNaked()
{
    int localClientNum;
    cg_s *cg;
    const char *visionName;
    int duration;
    float transition;

    duration = 1000;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 2 && Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 3 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "VisionSetNaked() called with wrong params.\n", 0);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) == 3 )
    {
        transition = Scr_GetFloat(2u, SCRIPTINSTANCE_CLIENT);
        duration = (int)(transition * 1000.0f + 9.313225746154785e-10);
    }
    localClientNum = CScr_GetLocalClientNum(0);
    if ( (unsigned int)localClientNum >= MAX_LOCAL_CLIENTS || !CL_IsLocalClientInGame(localClientNum) )
        return;
    visionName = Scr_GetString(1u, SCRIPTINSTANCE_CLIENT);
    if ( CG_VisionSetStartLerp_To(
             localClientNum,
             VISIONSETMODE_NAKED,
             VISIONSETLERP_TO_SMOOTH,
             visionName,
             duration) )
    {
        cg = CG_GetLocalClientGlobals(localClientNum);
        I_strncpyz(cg->visionNameNaked, visionName, 64);
    }
}

void __cdecl CScr_VisionSetUnderwater()
{
    int localClientNum;
    cg_s *cg;
    const char *visionName;
    int duration;
    float transition;

    duration = 1000;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 2 && Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 3 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "visionsetunderwater() called with wrong params.\n", 0);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) == 3 )
    {
        transition = Scr_GetFloat(2u, SCRIPTINSTANCE_CLIENT);
        duration = (int)floorf(transition * 1000.0f + 0.5f);
    }
    localClientNum = CScr_GetLocalClientNum(0);
    if ( (unsigned int)localClientNum >= MAX_LOCAL_CLIENTS || !CL_IsLocalClientInGame(localClientNum) )
        return;
    visionName = Scr_GetString(1u, SCRIPTINSTANCE_CLIENT);
    cg = CG_GetLocalClientGlobals(localClientNum);
    if ( CG_VisionSetStartLerp_To(
             localClientNum,
             VISIONSETMODE_UNDERWATER,
             VISIONSETLERP_CUSTOM_VALUE,
             visionName,
             duration) )
        I_strncpyz(cg->visionNameUnderwater, visionName, 64);
}

void __cdecl CScr_VisionSetDamage()
{
    int localClientNum;
    cg_s *cg;
    int enable;
    const char *visionName;
    int duration;
    float transition;

    duration = 1000;
    localClientNum = CScr_GetLocalClientNum(0);
    cg = CG_GetLocalClientGlobals(localClientNum);
    enable = Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT);
    visionName = Scr_GetString(2u, SCRIPTINSTANCE_CLIENT);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 3 && Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 4 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "VisionSetDamage() called with wrong params.\n", 0);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) == 4 )
    {
        transition = Scr_GetFloat(3u, SCRIPTINSTANCE_CLIENT);
        duration = (int)floorf(transition * 1000.0f + 0.5f);
    }
    if ( (unsigned int)localClientNum >= MAX_LOCAL_CLIENTS || !CL_IsLocalClientInGame(localClientNum) )
        return;
    if ( enable )
    {
        cg->visionDamageEndTime = 0x7FFFFFFF;
        I_strncpyz(cg->visionNameDamageTarget, visionName, 64);
        CG_VisionSetStartLerp_To(
            localClientNum,
            VISIONSETMODE_DAMAGE,
            VISIONSETLERP_CUSTOM_VALUE,
            cg->visionNameDamage,
            duration);
    }
    else
    {
        cg->visionDamageEndTime = cg->time + duration;
        I_strncpyz(cg->visionNameDamageTarget, cg->visionNameNaked, 64);
        CG_VisionSetStartLerp_To(
            localClientNum,
            VISIONSETMODE_DAMAGE,
            VISIONSETLERP_CUSTOM_VALUE,
            cg->visionNameNaked,
            duration);
    }
}

void __cdecl CScr_GiveGamerPicture()
{
    int localClientNum;
    int controllerIndex;
    const char *pictureName;

    localClientNum = CScr_GetLocalClientNum(0);
    if ( (unsigned int)localClientNum >= MAX_LOCAL_CLIENTS || !CL_IsLocalClientInGame(localClientNum) )
        return;
    pictureName = Scr_GetString(1u, SCRIPTINSTANCE_CLIENT);
    if ( !pictureName || !*pictureName )
        Scr_ParamError(1u, "GiveGamerPicture: unknown picture name", SCRIPTINSTANCE_CLIENT);
    controllerIndex = Com_LocalClient_GetControllerIndex(localClientNum);
    Live_GiveGamerPicture(controllerIndex, pictureName);
}

void __cdecl CScr_SP_GetFunction()
{
    const char *name;
    const char *namePtr;
    int type;

    name = Scr_GetString(0, SCRIPTINSTANCE_CLIENT);
    namePtr = name;
    if ( CScr_GetFunction(&namePtr, &type) )
        Scr_AddInt(1, SCRIPTINSTANCE_CLIENT);
    else
        Scr_AddUndefined(SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_SP_GetMethod()
{
    const char *name;
    const char *namePtr;
    int type;

    name = Scr_GetString(0, SCRIPTINSTANCE_CLIENT);
    namePtr = name;
    if ( CScr_GetMethod(&namePtr, &type) )
        Scr_AddInt(1, SCRIPTINSTANCE_CLIENT);
    else
        Scr_AddUndefined(SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_SP_GetFunctionProjectSpecific()
{
    const char *name;
    const char *namePtr;
    int type;

    name = Scr_GetString(0, SCRIPTINSTANCE_CLIENT);
    namePtr = name;
    if ( CScr_GetFunctionProjectSpecific(&namePtr, &type) )
        Scr_AddInt(1, SCRIPTINSTANCE_CLIENT);
    else
        Scr_AddUndefined(SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_SP_GetMethodProjectSpecific()
{
    const char *name;
    const char *namePtr;
    int type;

    name = Scr_GetString(0, SCRIPTINSTANCE_CLIENT);
    namePtr = name;
    if ( CScr_GetMethodProjectSpecific(&namePtr, &type) )
        Scr_AddInt(1, SCRIPTINSTANCE_CLIENT);
    else
        Scr_AddUndefined(SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_SP_GetDynEntityField()
{
    unsigned __int16 absDynEntId;
    int clientNum;
    unsigned int offset;

    absDynEntId = (unsigned __int16)Scr_GetInt(0, SCRIPTINSTANCE_CLIENT);
    clientNum = Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT);
    offset = Scr_GetInt(2u, SCRIPTINSTANCE_CLIENT);
    CScr_GetDynEntityField(absDynEntId, clientNum, offset);
}

#define EXTRA_CAM_ENT_NONE 959

static unsigned int *__cdecl CScr_GetCentCoopFlags(centity_s *cent)
{
    return (unsigned int *)((unsigned char *)cent + 772);
}

static void __cdecl CScr_SP_ApplyVolFog(
    int lcn,
    float startDist,
    float halfwayDist,
    float halfwayHeight,
    float baseHeight,
    float red,
    float green,
    float blue,
    float fogColorScale,
    float sunColorR,
    float sunColorG,
    float sunColorB,
    float sunDirX,
    float sunDirY,
    float sunDirZ,
    float sunStartAng,
    float sunStopAng,
    float maxFogOpacity,
    float transitionTime)
{
    float density;
    float heightDensity;
    cg_s *cgameGlob;

    density = 1.0f / halfwayDist;
    heightDensity = halfwayHeight < 1.0f ? 0.0f : 1.0f / halfwayHeight;
    cgameGlob = CG_GetLocalClientGlobals(lcn);
    R_SetFogFromServer(
        lcn,
        startDist,
        red,
        green,
        blue,
        density,
        heightDensity,
        baseHeight,
        fogColorScale,
        sunColorR,
        sunColorG,
        sunColorB,
        sunDirX,
        sunDirY,
        sunDirZ,
        sunStartAng,
        sunStopAng,
        maxFogOpacity);
    R_SwitchFog(lcn, 1u, cgameGlob->time, (int)(transitionTime * 1000.0f));
}

static void __cdecl CScr_SP_ParseVolFogParams(
    unsigned int firstParam,
    float *startDist,
    float *halfwayDist,
    float *halfwayHeight,
    float *baseHeight,
    float *red,
    float *green,
    float *blue,
    float *fogColorScale,
    float *sunColorR,
    float *sunColorG,
    float *sunColorB,
    float *sunDirX,
    float *sunDirY,
    float *sunDirZ,
    float *sunStartAng,
    float *sunStopAng,
    float *maxFogOpacity,
    float *transitionTime)
{
    unsigned int paramCount;
    float v0;

    paramCount = Scr_GetNumParam(SCRIPTINSTANCE_CLIENT);
    if ( paramCount != 8 && paramCount != 18 && paramCount != 9 && paramCount != 19 )
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "Incorrect number of parameters\n"
            "USAGE: setVolFog(<startDist>, <halfwayDist>, <halfwayHeight>, <baseHeight>, <red>, <green>, <blue>, "
            "<transition time>)\n",
            0);
    *startDist = Scr_GetFloat(firstParam, SCRIPTINSTANCE_CLIENT);
    if ( *startDist < 0.0f )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "setVolFog: startDist must be greater or equal to 0", 0);
    *halfwayDist = Scr_GetFloat(firstParam + 1, SCRIPTINSTANCE_CLIENT);
    if ( *halfwayDist <= 0.0f )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "setVolFog: halfwayDist must be greater than 0", 0);
    *halfwayHeight = Scr_GetFloat(firstParam + 2, SCRIPTINSTANCE_CLIENT);
    if ( *halfwayHeight < 0.0f )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "setVolFog: halfwayHeight must be greater or equal to 0", 0);
    *baseHeight = Scr_GetFloat(firstParam + 3, SCRIPTINSTANCE_CLIENT);
    *red = Scr_GetFloat(firstParam + 4, SCRIPTINSTANCE_CLIENT);
    *green = Scr_GetFloat(firstParam + 5, SCRIPTINSTANCE_CLIENT);
    *blue = Scr_GetFloat(firstParam + 6, SCRIPTINSTANCE_CLIENT);
    *sunColorR = 0.0f;
    *sunColorG = 0.0f;
    *sunColorB = 0.0f;
    *sunDirX = 0.0f;
    *sunDirY = 0.0f;
    *sunDirZ = 0.0f;
    *sunStartAng = 0.0f;
    *sunStopAng = 0.0f;
    *maxFogOpacity = 1.0f;
    if ( paramCount == 18 || paramCount == 19 )
    {
        *fogColorScale = Scr_GetFloat(firstParam + 7, SCRIPTINSTANCE_CLIENT);
        *sunColorR = Scr_GetFloat(firstParam + 8, SCRIPTINSTANCE_CLIENT);
        *sunColorG = Scr_GetFloat(firstParam + 9, SCRIPTINSTANCE_CLIENT);
        *sunColorB = Scr_GetFloat(firstParam + 10, SCRIPTINSTANCE_CLIENT);
        *sunDirX = Scr_GetFloat(firstParam + 11, SCRIPTINSTANCE_CLIENT);
        *sunDirY = Scr_GetFloat(firstParam + 12, SCRIPTINSTANCE_CLIENT);
        *sunDirZ = Scr_GetFloat(firstParam + 13, SCRIPTINSTANCE_CLIENT);
        *sunStartAng = Scr_GetFloat(firstParam + 14, SCRIPTINSTANCE_CLIENT);
        *sunStopAng = Scr_GetFloat(firstParam + 15, SCRIPTINSTANCE_CLIENT);
        *transitionTime = Scr_GetFloat(firstParam + 16, SCRIPTINSTANCE_CLIENT);
        *maxFogOpacity = Scr_GetFloat(firstParam + 17, SCRIPTINSTANCE_CLIENT);
    }
    else
    {
        if ( *green <= *red )
            v0 = *red;
        else
            v0 = *green;
        *fogColorScale = v0;
        if ( *blue > v0 )
            *fogColorScale = *blue;
        if ( *fogColorScale > 0.0f )
        {
            *red *= 1.0f / *fogColorScale;
            *green *= 1.0f / *fogColorScale;
            *blue *= 1.0f / *fogColorScale;
        }
        *transitionTime = Scr_GetFloat(firstParam + 7, SCRIPTINSTANCE_CLIENT);
    }
}

static void __cdecl CScr_SP_GetEntityField()
{
    unsigned int entnum;
    int clientNum;
    unsigned int offset;

    entnum = (unsigned int)Scr_GetInt(0, SCRIPTINSTANCE_CLIENT);
    clientNum = Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT);
    offset = (unsigned int)Scr_GetInt(2u, SCRIPTINSTANCE_CLIENT);
    CScr_GetEntityField(entnum, clientNum, offset);
}

static void __cdecl CScr_SP_SetEntityField()
{
    unsigned int entnum;
    int clientNum;
    unsigned int offset;

    entnum = (unsigned int)Scr_GetInt(0, SCRIPTINSTANCE_CLIENT);
    clientNum = Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT);
    offset = (unsigned int)Scr_GetInt(2u, SCRIPTINSTANCE_CLIENT);
    CScr_SetEntityField(entnum, clientNum, offset);
}

static void __cdecl CScr_SP_GetObjectField()
{
    unsigned int classnum;
    unsigned int entnum;
    int clientNum;
    unsigned int offset;

    classnum = (unsigned int)Scr_GetInt(0, SCRIPTINSTANCE_CLIENT);
    entnum = (unsigned int)Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT);
    clientNum = Scr_GetInt(2u, SCRIPTINSTANCE_CLIENT);
    offset = (unsigned int)Scr_GetInt(3u, SCRIPTINSTANCE_CLIENT);
    CScr_GetObjectField(classnum, entnum, clientNum, offset);
}

static void __cdecl CScr_SP_SetObjectField()
{
    unsigned int classnum;
    unsigned int entnum;
    int clientNum;
    unsigned int offset;

    classnum = (unsigned int)Scr_GetInt(0, SCRIPTINSTANCE_CLIENT);
    entnum = (unsigned int)Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT);
    clientNum = Scr_GetInt(2u, SCRIPTINSTANCE_CLIENT);
    offset = (unsigned int)Scr_GetInt(3u, SCRIPTINSTANCE_CLIENT);
    CScr_SetObjectField(classnum, entnum, clientNum, offset);
}

static void __cdecl CScr_SP_SetDynEntityField()
{
    unsigned __int16 absDynEntId;
    int clientNum;
    unsigned int offset;

    absDynEntId = (unsigned __int16)Scr_GetInt(0, SCRIPTINSTANCE_CLIENT);
    clientNum = Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT);
    offset = (unsigned int)Scr_GetInt(2u, SCRIPTINSTANCE_CLIENT);
    CScr_SetDynEntityField(absDynEntId, clientNum, offset);
}

static void __cdecl CScr_SP_GetDynEntStringField()
{
    CScr_SP_GetDynEntityField();
}

static void __cdecl CScr_SP_Notify()
{
    scr_entref_t entref;
    centity_s *cent;
    unsigned __int16 stringValue;
    unsigned int paramcount;

    entref = Scr_GetEntityRef(0, SCRIPTINSTANCE_CLIENT);
    cent = CScr_SP_GetMethodEntity(entref);
    stringValue = Scr_GetConstString(1u, SCRIPTINSTANCE_CLIENT);
    paramcount = Scr_GetNumParam(SCRIPTINSTANCE_CLIENT);
    if ( paramcount > 2 )
        paramcount -= 2;
    else
        paramcount = 0;
    CScr_Notify(entref.client, cent, stringValue, paramcount);
}

static void __cdecl CScr_SP_UpdateTagInternal()
{
    scr_entref_t entref;
    centity_s *cent;
    unsigned int tagName;
    int result;

    entref = Scr_GetEntityRef(0, SCRIPTINSTANCE_CLIENT);
    cent = CScr_SP_GetMethodEntity(entref);
    tagName = Scr_GetConstLowercaseString(1u, SCRIPTINSTANCE_CLIENT);
    result = CScr_UpdateTagInternal(cent, tagName, &cg_cachedTagMat);
    Scr_AddInt(result, SCRIPTINSTANCE_CLIENT);
}

static void __cdecl CScr_SP_FxParamError()
{
    int localClientNum;
    unsigned int paramIndex;
    const char *errorString;
    int fxId;

    localClientNum = CScr_GetLocalClientNum(0);
    paramIndex = (unsigned int)Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT);
    errorString = Scr_GetString(2u, SCRIPTINSTANCE_CLIENT);
    fxId = Scr_GetInt(3u, SCRIPTINSTANCE_CLIENT);
    CScr_FxParamError(localClientNum, paramIndex, errorString, fxId);
}

static void __cdecl CScr_SP_SetFxAngles()
{
    unsigned int axisCount;
    float axis[3][3];
    float angles[3];

    axisCount = (unsigned int)Scr_GetInt(0, SCRIPTINSTANCE_CLIENT);
    Scr_GetVector(1u, axis[0], SCRIPTINSTANCE_CLIENT);
    if ( axisCount > 1 )
        Scr_GetVector(2u, axis[1], SCRIPTINSTANCE_CLIENT);
    if ( axisCount > 2 )
        Scr_GetVector(3u, axis[2], SCRIPTINSTANCE_CLIENT);
    CScr_SetFxAngles(axisCount, axis, angles);
    Scr_AddVector(angles, SCRIPTINSTANCE_CLIENT);
}

static void __cdecl CScr_SP_ReadOnly()
{
    Scr_Error(SCRIPTINSTANCE_CLIENT, "Tried to set a read only entity field", 0);
}

static void __cdecl CScr_SP_GlassSmash()
{
    float pos[3];
    float dir[3];

    Scr_GetVector(0, pos, SCRIPTINSTANCE_CLIENT);
    Scr_GetVector(1u, dir, SCRIPTINSTANCE_CLIENT);
    CScr_GlassSmash(pos, dir);
}

static void __cdecl CScr_SP_HandleAnimError()
{
    CScr_HandleAnimError(Scr_GetInt(0, SCRIPTINSTANCE_CLIENT));
}

static void __cdecl CScr_SP_DestructibleCallback()
{
    scr_entref_t entref;
    centity_s *cent;

    entref = Scr_GetEntityRef(0, SCRIPTINSTANCE_CLIENT);
    cent = CScr_SP_GetMethodEntity(entref);
    CScr_DestructibleCallback(
        cent,
        Scr_GetConstString(1u, SCRIPTINSTANCE_CLIENT),
        Scr_GetInt(2u, SCRIPTINSTANCE_CLIENT),
        Scr_GetFloat(3u, SCRIPTINSTANCE_CLIENT),
        Scr_GetInt(4u, SCRIPTINSTANCE_CLIENT));
}

static void __cdecl CScr_SP_GetCentFieldByIndex(unsigned int fieldIndex)
{
    unsigned int entnum;
    int clientNum;

    entnum = (unsigned int)Scr_GetInt(0, SCRIPTINSTANCE_CLIENT);
    clientNum = Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT);
    CScr_GetEntityField(entnum, clientNum, fieldIndex);
}

static void __cdecl CScr_SP_GetModel()
{
    CScr_SP_GetCentFieldByIndex(7u);
}

static void __cdecl CScr_SP_GetType()
{
    CScr_SP_GetCentFieldByIndex(6u);
}

static void __cdecl CScr_SP_GetSpecies()
{
    CScr_SP_GetCentFieldByIndex(4u);
}

static void __cdecl CScr_SP_GetIsDog()
{
    CScr_SP_GetCentFieldByIndex(5u);
}

static void __cdecl CScr_SP_GetVehicleClass()
{
    CScr_SP_GetCentFieldByIndex(9u);
}

static void __cdecl CScr_SP_GetVehicleTreadFx()
{
    CScr_SP_GetCentFieldByIndex(10u);
}

static void __cdecl CScr_SP_GetWeaponName()
{
    CScr_SP_GetCentFieldByIndex(2u);
}

static void __cdecl CScr_SP_PlayerFootstepEvent()
{
    scr_entref_t entref;

    entref = Scr_GetEntityRef(1u, SCRIPTINSTANCE_CLIENT);
    CScr_PlayerFootstepEvent(
        Scr_GetInt(0, SCRIPTINSTANCE_CLIENT),
        CScr_SP_GetMethodEntity(entref),
        Scr_GetInt(2u, SCRIPTINSTANCE_CLIENT) != 0,
        Scr_GetInt(3u, SCRIPTINSTANCE_CLIENT),
        Scr_GetInt(4u, SCRIPTINSTANCE_CLIENT),
        Scr_GetInt(5u, SCRIPTINSTANCE_CLIENT) != 0);
}

static void __cdecl CScr_SP_PlayerJumpEvent()
{
    scr_entref_t entref;

    entref = Scr_GetEntityRef(1u, SCRIPTINSTANCE_CLIENT);
    CScr_PlayerJumpEvent(
        Scr_GetInt(0, SCRIPTINSTANCE_CLIENT),
        CScr_SP_GetMethodEntity(entref),
        Scr_GetInt(2u, SCRIPTINSTANCE_CLIENT) != 0,
        Scr_GetInt(3u, SCRIPTINSTANCE_CLIENT),
        Scr_GetInt(4u, SCRIPTINSTANCE_CLIENT) != 0);
}

static void __cdecl CScr_SP_PlayerLandEvent()
{
    scr_entref_t entref;

    entref = Scr_GetEntityRef(1u, SCRIPTINSTANCE_CLIENT);
    CScr_PlayerLandEvent(
        Scr_GetInt(0, SCRIPTINSTANCE_CLIENT),
        CScr_SP_GetMethodEntity(entref),
        Scr_GetInt(2u, SCRIPTINSTANCE_CLIENT) != 0,
        Scr_GetInt(3u, SCRIPTINSTANCE_CLIENT),
        Scr_GetInt(4u, SCRIPTINSTANCE_CLIENT) != 0,
        Scr_GetInt(5u, SCRIPTINSTANCE_CLIENT) != 0);
}

static void __cdecl CScr_SP_PlayerFoliageEvent()
{
    scr_entref_t entref;

    entref = Scr_GetEntityRef(1u, SCRIPTINSTANCE_CLIENT);
    CScr_PlayerFoliageEvent(
        Scr_GetInt(0, SCRIPTINSTANCE_CLIENT),
        CScr_SP_GetMethodEntity(entref),
        Scr_GetInt(2u, SCRIPTINSTANCE_CLIENT) != 0,
        Scr_GetInt(3u, SCRIPTINSTANCE_CLIENT) != 0);
}

void __cdecl CScr_PlayAIFootstepSound(int localClientNum, centity_s *cent, int isLeftFoot, bool isSpeciesDog)
{
    DObj *obj;
    cg_s *cgameGlob;
    float footOrigin[3];
    float traceStart[3];
    float traceEnd[3];
    float axis[3][3];
    unsigned int paramCount;
    unsigned __int16 thread;
    unsigned int tagName;
    int surfType;

    if ( !cg_scr_sp_data.playAIFootstep || !cent )
        return;
    obj = Com_GetClientDObj(cent->nextState.clientNum, localClientNum);
    cgameGlob = CG_GetLocalClientGlobals(localClientNum);
    if ( !obj || !cgameGlob )
        return;
    if ( Vec3DistanceSq(cgameGlob->refdef.vieworg, cent->pose.origin) >= 262144.0f )
        return;
    tagName = isLeftFoot ? scr_const.j_ball_le : scr_const.j_ball_ri;
    if ( !CG_DObjGetWorldTagMatrix(&cent->pose, obj, tagName, axis, footOrigin) )
    {
        footOrigin[0] = cent->pose.origin[0];
        footOrigin[1] = cent->pose.origin[1];
        footOrigin[2] = cent->pose.origin[2];
    }
    traceStart[0] = footOrigin[0];
    traceStart[1] = footOrigin[1];
    traceStart[2] = footOrigin[2] + 12.0f;
    traceEnd[0] = footOrigin[0];
    traceEnd[1] = footOrigin[1];
    traceEnd[2] = footOrigin[2] - 12.0f;
    surfType = (CM_TracePointDown(traceStart, traceEnd, 42074129, 0x3F00000, footOrigin, 0, 0) & 0x3F00000) >> 20;
    paramCount = 5;
    if ( isSpeciesDog )
    {
        Scr_AddBool(1u, SCRIPTINSTANCE_CLIENT);
        paramCount = 6;
    }
    if ( (cent->currentState.eFlags2 & 0x200000) != 0 || ((*CScr_GetCentCoopFlags(cent) >> 26) & 1) != 0 )
        Scr_AddInt(1, SCRIPTINSTANCE_CLIENT);
    else
        Scr_AddInt(0, SCRIPTINSTANCE_CLIENT);
    Scr_AddString((char *)Com_SurfaceTypeToName(surfType), SCRIPTINSTANCE_CLIENT);
    Scr_AddVector(footOrigin, SCRIPTINSTANCE_CLIENT);
    CScr_AddEntity(cent, (unsigned __int16)localClientNum);
    Scr_AddInt(localClientNum, SCRIPTINSTANCE_CLIENT);
    thread = Scr_ExecThread(SCRIPTINSTANCE_CLIENT, cg_scr_sp_data.playAIFootstep, paramCount);
    Scr_FreeThread(thread, SCRIPTINSTANCE_CLIENT);
}
// Decomp: CoDSP_rdBlackOps.map.c @ 823CE660
void __cdecl CScr_ExtraCamConfig()
{
    Scr_Error(SCRIPTINSTANCE_CLIENT, "extracamconfig is deprecated", 0);
}

// Decomp: CoDSP_rdBlackOps.map.c @ 82945BA8
void __cdecl CScr_PlayFXOnDynEnt()
{
    int fxId;
    scr_entref_t entref;
    const FxEffectDef *fxDef;
    unsigned int effectHandle;

    fxId = Scr_GetInt(0, SCRIPTINSTANCE_CLIENT);
    entref = Scr_GetEntityRef(1u, SCRIPTINSTANCE_CLIENT);
    if ( entref.classnum != 4 )
        Scr_ParamError(1u, "PlayFXOnDynEnt param isn't a DYN entity.", SCRIPTINSTANCE_CLIENT);
    if ( fxId <= 0 || fxId >= 196 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, va("CScr_PlayFX: invalid effect id %d", fxId), 0);
    fxDef = CG_GetLocalClientStaticGlobals(0)->fxs[fxId];
    if ( !fxDef )
        Com_Error(ERR_DROP, "fxDef");
    effectHandle = DynEntCl_PlayBoltedFX(fxDef, entref.entnum);
    Scr_AddInt(effectHandle, SCRIPTINSTANCE_CLIENT);
}

// Decomp: CoDSP_rdBlackOps.map.c @ 82945DC8 / 82945E38
void __cdecl CScr_ShowScores()
{
    int localClientNum;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 1 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "ShowScores() called with wrong params.\n", 0);
    localClientNum = CScr_GetLocalClientNum(0);
    if ( !CG_IsScoreboardDisplayed(localClientNum) )
        Cbuf_AddText(localClientNum, "togglescores\n");
}

void __cdecl CScr_HideScores()
{
    int localClientNum;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 1 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "HideScores() called with wrong params.\n", 0);
    localClientNum = CScr_GetLocalClientNum(0);
    if ( CG_IsScoreboardDisplayed(localClientNum) )
        Cbuf_AddText(localClientNum, "togglescores\n");
}

// Decomp: CoDSP_rdBlackOps.map.c @ 8294CE80
void __cdecl CScr_NotifyLevel()
{
    const char *eventName;
    unsigned __int16 thread;

    eventName = Scr_GetString(0, SCRIPTINSTANCE_CLIENT);
    Scr_AddString(eventName, SCRIPTINSTANCE_CLIENT);
    thread = Scr_ExecThread(SCRIPTINSTANCE_CLIENT, cg_scr_data.levelnotify, 1u);
    Scr_FreeThread(thread, SCRIPTINSTANCE_CLIENT);
}

// Decomp: CoDSP_rdBlackOps.map.c @ 82946028 / 82945F10 / 82945FA8
void __cdecl CScr_EnableWaterSheetFX()
{
    Dvar_SetBoolByName("r_waterSheetingFX_enable", Scr_GetInt(0, SCRIPTINSTANCE_CLIENT) != 0);
}

void __cdecl CScr_StartWaterSheetFX()
{
    int localClientNum;
    int durationMsec;

    localClientNum = CScr_GetLocalClientNum(0);
    durationMsec = Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT);
    CG_StartWaterSheetFX(localClientNum, durationMsec);
}

void __cdecl CScr_StopWaterSheetFX()
{
    CG_StopWaterSheetFX(CScr_GetLocalClientNum(0));
}

void __cdecl CScr_SetWaterSheetFXRate()
{
    cg_s *cgameGlob;

    cgameGlob = CG_GetLocalClientGlobals(CScr_GetLocalClientNum(0));
    cgameGlob->refdef.waterSheetingFx.frameRate = Scr_GetFloat(1u, SCRIPTINSTANCE_CLIENT);
}

// Decomp: CoDSP_rdBlackOps.map.c @ 82948798
void __cdecl CScr_IsExtraCam(scr_entref_t entref)
{
    cg_s *cgameGlob;
    int localClientNum;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 1 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "isextracam() called with wrong params.\n", 0);
    localClientNum = CScr_GetLocalClientNum(0);
    cgameGlob = CG_GetLocalClientGlobals(localClientNum);
    if ( cgameGlob->cameraData.extraCamEntNum != EXTRA_CAM_ENT_NONE )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "There can be only one extra camera active in the level.", 0);
    cgameGlob->cameraData.extraCamEntNum = entref.entnum;
}

// Decomp: CoDSP_rdBlackOps.map.c @ 82948838 / 829488D8
void __cdecl CScr_StopExtraCam()
{
    cg_s *cgameGlob;
    int localClientNum;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 1 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "CScr_StopExtraCam() called with wrong params.\n", 0);
    localClientNum = CScr_GetLocalClientNum(0);
    cgameGlob = CG_GetLocalClientGlobals(localClientNum);
    if ( cgameGlob->cameraData.extraCamEntNum != EXTRA_CAM_ENT_NONE )
    {
        cgameGlob->cameraData.extraCamEntNum = EXTRA_CAM_ENT_NONE;
        cgameGlob->cameraData.extraCamFov = 65.0f;
    }
}

void __cdecl CScr_SetExtraCamFov()
{
    cg_s *cgameGlob;
    float fov;
    int localClientNum;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 2 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "CScr_SetExtraCamFov() called with wrong params.\n", 0);
    fov = Scr_GetFloat(1u, SCRIPTINSTANCE_CLIENT);
    iassert(fov >= 1.0f && fov <= 160.0f);
    localClientNum = CScr_GetLocalClientNum(0);
    cgameGlob = CG_GetLocalClientGlobals(localClientNum);
    if ( cgameGlob->cameraData.extraCamEntNum == EXTRA_CAM_ENT_NONE )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "CScr_SetExtraCamFov(), There is no extra cam active in the level.\n", 0);
    cgameGlob->cameraData.extraCamFov = fov;
}

// Decomp: CoDSP_rdBlackOps.map.c @ 82948B90 / 82948D20 / 82949138
void __cdecl CScr_SpawnDynEnt()
{
    const char *modelName;
    XModel *model;
    float pos[3];
    float angles[3];
    float quat[4];
    float hitpos[3];
    float force[3];
    unsigned __int16 dynEntId;
    unsigned __int16 absId;

    modelName = Scr_GetString(0, SCRIPTINSTANCE_CLIENT);
    model = CG_GetXModel(modelName, 0);
    if ( !model )
        Scr_ParamError(0, va("spawndynent, failed to find model %s.", modelName), SCRIPTINSTANCE_CLIENT);
    if ( !model->physPreset )
        Scr_ParamError(
            0,
            va(
                "spawndynent, failed to spawn %s dyn ent because no physPreset is specified in the xmodel GDT.",
                modelName),
            SCRIPTINSTANCE_CLIENT);
    Scr_GetVector(1u, pos, SCRIPTINSTANCE_CLIENT);
    Scr_GetVector(2u, angles, SCRIPTINSTANCE_CLIENT);
    hitpos[0] = hitpos[1] = hitpos[2] = 0.0f;
    force[0] = force[1] = force[2] = 0.0f;
    AnglesToQuat(angles, quat);
    dynEntId = DynEntCl_CreateEntityModel(model, quat, pos, hitpos, force, 0, 0);
    absId = DynEnt_GetClientAbsId(dynEntId, DYNENT_DRAW_MODEL);
    Scr_AddEntityNum(absId, 4, SCRIPTINSTANCE_CLIENT, 0);
}

void __cdecl CScr_LaunchDynEnt()
{
    scr_entref_t entref;
    float force[3];
    float hitpos[3];
    const float *hitPosPtr;

    entref = Scr_GetEntityRef(0, SCRIPTINSTANCE_CLIENT);
    Scr_GetVector(1u, force, SCRIPTINSTANCE_CLIENT);
    hitPosPtr = 0;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) > 2 )
    {
        Scr_GetVector(2u, hitpos, SCRIPTINSTANCE_CLIENT);
        hitPosPtr = hitpos;
    }
    DynEntCl_Launch(entref.entnum, force, hitPosPtr);
}

void __cdecl CScr_SetDynEntEnabled()
{
    scr_entref_t entref;
    bool enabled;

    entref = Scr_GetEntityRef(0, SCRIPTINSTANCE_CLIENT);
    enabled = false;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) > 1 )
        enabled = Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT) != 0;
    DynEntCl_Enable(entref.entnum, enabled);
}

// Decomp: CoDSP_rdBlackOps.map.c @ 8294BE28 / 8294BF68
void __cdecl CScr_EntYawOverridesLinkYaw(scr_entref_t entref)
{
    centity_s *cent;
    unsigned int flag;
    unsigned int *coopFlags;

    cent = CScr_SP_GetMethodEntity(entref);
    flag = Scr_GetInt(0, SCRIPTINSTANCE_CLIENT) != 0 ? 0x200u : 0;
    coopFlags = CScr_GetCentCoopFlags(cent);
    *coopFlags = *coopFlags & 0xFFFFFDFF | flag;
}

void __cdecl CScr_EntGetsWeaponFireNotification(scr_entref_t entref)
{
    centity_s *cent;
    unsigned int flag;
    unsigned int *coopFlags;

    cent = CScr_SP_GetMethodEntity(entref);
    flag = Scr_GetInt(0, SCRIPTINSTANCE_CLIENT) != 0 ? 0x100u : 0;
    coopFlags = CScr_GetCentCoopFlags(cent);
    *coopFlags = *coopFlags & 0xFFFFFEFF | flag;
}

// Decomp: CoDSP_rdBlackOps.map.c @ 829483A0
void __cdecl CScr_SetWaterFog()
{
    cg_s *cgameGlob;
    WaterFogDef *wf;
    unsigned int paramCount;
    float startDist;
    float halfwayDist;
    float halfwayHeight;
    float baseHeight;
    float red;
    float green;
    float blue;

    paramCount = Scr_GetNumParam(SCRIPTINSTANCE_CLIENT);
    if ( paramCount != 7 && paramCount != 17 )
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "Incorrect number of parameters\n"
            "USAGE: SetWaterFog(<startDist>, <halfwayDist>, <halfwayHeight>, <baseHeight>, <red>, <green>, <blue>)\n",
            0);
    startDist = Scr_GetFloat(0, SCRIPTINSTANCE_CLIENT);
    halfwayDist = Scr_GetFloat(1u, SCRIPTINSTANCE_CLIENT);
    if ( halfwayDist <= 0.0f )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "SetWaterFog: halfwayDist must be greater than 0", 0);
    halfwayHeight = Scr_GetFloat(2u, SCRIPTINSTANCE_CLIENT);
    if ( halfwayHeight < 0.0f )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "SetWaterFog: halfwayHeight must be greater or equal to 0", 0);
    baseHeight = Scr_GetFloat(3u, SCRIPTINSTANCE_CLIENT);
    red = Scr_GetFloat(4u, SCRIPTINSTANCE_CLIENT);
    green = Scr_GetFloat(5u, SCRIPTINSTANCE_CLIENT);
    blue = Scr_GetFloat(6u, SCRIPTINSTANCE_CLIENT);
    cgameGlob = CG_GetLocalClientGlobals(0);
    wf = &cgameGlob->refdef.waterFog;
    wf->startTime = 0;
    wf->finishTime = 0;
    wf->color[0] = red;
    wf->color[1] = green;
    wf->color[2] = blue;
    wf->color[3] = 1.0f;
    wf->fogStart = startDist;
    wf->density = 1.0f / halfwayDist;
    wf->heightDensity = halfwayHeight < 1.0f ? 0.0f : 1.0f / halfwayHeight;
    wf->baseHeight = baseHeight;
    if ( paramCount == 7 )
    {
        Com_Printf(1, "setWaterFog: Old syntax used. Please update script.\n");
        wf->sunFogColor[0] = 0.0f;
        wf->sunFogColor[1] = 0.0f;
        wf->sunFogColor[2] = 0.0f;
        wf->sunFogDir[0] = 0.0f;
        wf->sunFogDir[1] = 0.0f;
        wf->sunFogDir[2] = 0.0f;
        wf->sunFogStartAng = 0.0f;
        wf->sunFogEndAng = 0.0f;
    }
    else
    {
        wf->color[3] = Scr_GetFloat(7u, SCRIPTINSTANCE_CLIENT);
        wf->sunFogColor[0] = Scr_GetFloat(8u, SCRIPTINSTANCE_CLIENT);
        wf->sunFogColor[1] = Scr_GetFloat(9u, SCRIPTINSTANCE_CLIENT);
        wf->sunFogColor[2] = Scr_GetFloat(10u, SCRIPTINSTANCE_CLIENT);
        wf->sunFogDir[0] = Scr_GetFloat(11u, SCRIPTINSTANCE_CLIENT);
        wf->sunFogDir[1] = Scr_GetFloat(12u, SCRIPTINSTANCE_CLIENT);
        wf->sunFogDir[2] = Scr_GetFloat(13u, SCRIPTINSTANCE_CLIENT);
        wf->sunFogStartAng = Scr_GetFloat(14u, SCRIPTINSTANCE_CLIENT);
        wf->sunFogEndAng = Scr_GetFloat(15u, SCRIPTINSTANCE_CLIENT);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c @ 82947A00 / 82947ED0
void __cdecl CScr_SetVolumetricFog()
{
    float startDist;
    float halfwayDist;
    float halfwayHeight;
    float baseHeight;
    float red;
    float green;
    float blue;
    float fogColorScale;
    float sunColorR;
    float sunColorG;
    float sunColorB;
    float sunDirX;
    float sunDirY;
    float sunDirZ;
    float sunStartAng;
    float sunStopAng;
    float maxFogOpacity;
    float transitionTime;
    unsigned int i;

    CScr_SP_ParseVolFogParams(
        0,
        &startDist,
        &halfwayDist,
        &halfwayHeight,
        &baseHeight,
        &red,
        &green,
        &blue,
        &fogColorScale,
        &sunColorR,
        &sunColorG,
        &sunColorB,
        &sunDirX,
        &sunDirY,
        &sunDirZ,
        &sunStartAng,
        &sunStopAng,
        &maxFogOpacity,
        &transitionTime);
    for ( i = 0; i < MAX_LOCAL_CLIENTS; ++i )
    {
        if ( CL_LocalClient_IsActive(i) )
            CScr_SP_ApplyVolFog(
                i,
                startDist,
                halfwayDist,
                halfwayHeight,
                baseHeight,
                red,
                green,
                blue,
                fogColorScale,
                sunColorR,
                sunColorG,
                sunColorB,
                sunDirX,
                sunDirY,
                sunDirZ,
                sunStartAng,
                sunStopAng,
                maxFogOpacity,
                transitionTime);
    }
}

void __cdecl CScr_SetVolumetricFogForClient()
{
    float startDist;
    float halfwayDist;
    float halfwayHeight;
    float baseHeight;
    float red;
    float green;
    float blue;
    float fogColorScale;
    float sunColorR;
    float sunColorG;
    float sunColorB;
    float sunDirX;
    float sunDirY;
    float sunDirZ;
    float sunStartAng;
    float sunStopAng;
    float maxFogOpacity;
    float transitionTime;
    int localClientNum;

    localClientNum = Scr_GetInt(0, SCRIPTINSTANCE_CLIENT);
    if ( localClientNum >= MAX_LOCAL_CLIENTS || !CL_LocalClient_IsActive(localClientNum) )
        return;
    CScr_SP_ParseVolFogParams(
        1u,
        &startDist,
        &halfwayDist,
        &halfwayHeight,
        &baseHeight,
        &red,
        &green,
        &blue,
        &fogColorScale,
        &sunColorR,
        &sunColorG,
        &sunColorB,
        &sunDirX,
        &sunDirY,
        &sunDirZ,
        &sunStartAng,
        &sunStopAng,
        &maxFogOpacity,
        &transitionTime);
    CScr_SP_ApplyVolFog(
        localClientNum,
        startDist,
        halfwayDist,
        halfwayHeight,
        baseHeight,
        red,
        green,
        blue,
        fogColorScale,
        sunColorR,
        sunColorG,
        sunColorB,
        sunDirX,
        sunDirY,
        sunDirZ,
        sunStartAng,
        sunStopAng,
        maxFogOpacity,
        transitionTime);
}

static XModel *__cdecl CG_GetXModel(const char *name, int localClientNum)
{
    int modelIndex;
    cgs_t *cgs;

    modelIndex = CG_GetModelIndex(name, localClientNum);
    if ( modelIndex < 0 )
        return 0;
    cgs = CG_GetLocalClientStaticGlobals(localClientNum);
    return cgs->gameModels[modelIndex];
}

// Decomp: CoDSP_rdBlackOps.map.c (CScr_PlayWeaponDeathEffects ~8294CED8)
// Map: CoDSP_rd.map VA 0x8294CED8 (cg_scr_main_coop.obj)
void __cdecl CScr_PlayWeaponDeathEffects()
{
    unsigned int localClientNum;
    int entnum;
    unsigned int weaponIndex;
    int mod;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 4 )
        return;
    localClientNum = Scr_GetInt(0, SCRIPTINSTANCE_CLIENT);
    entnum = Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT);
    weaponIndex = Scr_GetInt(2u, SCRIPTINSTANCE_CLIENT);
    mod = Scr_GetInt(3u, SCRIPTINSTANCE_CLIENT);
    CScr_PlayWeaponDeathEffectsInternal(localClientNum, entnum, weaponIndex, mod);
}

void __cdecl CG_DoPlayWeaponDeathEffects(unsigned int localClientNum, int entnum, unsigned int weaponIndex, int mod)
{
    CScr_PlayWeaponDeathEffectsInternal(localClientNum, entnum, weaponIndex, mod);
}

void __cdecl CG_DoPlayWeaponDamageEffects(unsigned int localClientNum, int entnum, unsigned int weaponIndex, int mod)
{
    CScr_PlayWeaponDamageEffectsInternal(localClientNum, entnum, weaponIndex, mod);
}

// Decomp: CoDSP_rdBlackOps.map.c (CScr_ForceGameModeMappings ~823C72F0)
// Map: CoDSP_rd.map VA 0x823C72F0 (cg_scr_main.obj)
void __cdecl CScr_ForceGameModeMappings()
{
    int localClientNum;
    const char *modeName;
    int controllerIndex;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 2 )
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "Incorrect number of parameters passed to ForceGameModeMappings()\n"
            "USAGE: ForceGameModeMappings( <localClientNum>, <modeName>)\n",
            0);
    localClientNum = CScr_GetLocalClientNum(0);
    modeName = Scr_GetString(1u, SCRIPTINSTANCE_CLIENT);
    if ( !I_stricmp(modeName, "default") )
    {
        controllerIndex = Com_LocalClient_GetControllerIndex(localClientNum);
        GamerProfile_ExecControllerBindings(controllerIndex);
        CL_ClearKeys(localClientNum);
        return;
    }
    if ( !I_stricmp(modeName, "zombietron") )
    {
        controllerIndex = Com_LocalClient_GetControllerIndex(localClientNum);
        Cmd_ExecuteSingleCommand(localClientNum, controllerIndex, (char *)"exec thumbstick_default.cfg\n");
        controllerIndex = Com_LocalClient_GetControllerIndex(localClientNum);
        Cmd_ExecuteSingleCommand(localClientNum, controllerIndex, (char *)"exec buttons_default.cfg\n");
        controllerIndex = Com_LocalClient_GetControllerIndex(localClientNum);
        Cmd_ExecuteSingleCommand(localClientNum, controllerIndex, (char *)"exec buttons_zombietron\n");
        CL_ClearKeys(localClientNum);
    }
}

static centity_s *__cdecl CScr_SP_GetMethodEntity(scr_entref_t entref)
{
    if ( entref.classnum )
    {
        Scr_Error(SCRIPTINSTANCE_CLIENT, "not an entity", 0);
        return 0;
    }
    if ( entref.entnum >= 0x600u )
    {
        if ( !Assert_MyHandler(
                    "C:\\projects\\cod\\codsrc\\src\\cgame_sp\\cg_scr_main_sp.cpp",
                    0,
                    0,
                    "%s",
                    "entref.entnum < MAX_LOCAL_CENTITIES") )
            __debugbreak();
    }
    if ( CG_GetClientNumForLocalClient(entref.client) == entref.entnum )
        return &CG_GetLocalClientGlobals(entref.client)->predictedPlayerEntity;
    return CG_GetEntity(entref.client, entref.entnum);
}

// Decomp: CoDSP_rdBlackOps.map.c (CScr_MapShaderConstant ~82949800)
// Map: CoDSP_rd.map VA 0x82949800 (cg_scr_main_coop.obj)
void __cdecl CScrCmd_MapShaderConstant(scr_entref_t entref)
{
    unsigned int paramCount;
    int localClientNum;
    unsigned int index;
    const char *constantName;
    float values[4];
    centity_s *cent;
    cg_s *cgameGlob;
    ShaderConstantSet *constantSet;

    if ( entref.classnum )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "not an entity", 0);
    paramCount = Scr_GetNumParam(SCRIPTINSTANCE_CLIENT);
    if ( paramCount < 3 || paramCount > 7 )
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "USAGE: ent mapshaderconstant( <localClientNum>, <index>, <constant name>)\n",
            0);
    localClientNum = CScr_GetLocalClientNum(0);
    index = Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT);
    constantName = Scr_GetString(2u, SCRIPTINSTANCE_CLIENT);
    values[0] = 0.0f;
    values[1] = 0.0f;
    values[2] = 0.0f;
    values[3] = 0.0f;
    if ( paramCount >= 4 )
        values[0] = Scr_GetFloat(3u, SCRIPTINSTANCE_CLIENT);
    if ( paramCount >= 5 )
        values[1] = Scr_GetFloat(4u, SCRIPTINSTANCE_CLIENT);
    if ( paramCount >= 6 )
        values[2] = Scr_GetFloat(5u, SCRIPTINSTANCE_CLIENT);
    if ( paramCount >= 7 )
        values[3] = Scr_GetFloat(6u, SCRIPTINSTANCE_CLIENT);
    cent = CScr_SP_GetMethodEntity(entref);
    if ( !cent )
    {
        Scr_AddInt(0, SCRIPTINSTANCE_CLIENT);
        return;
    }
    constantSet = &cent->pose.constantSet;
    if ( !R_MapShaderConstantSet(constantSet, index, constantName) )
    {
        Scr_AddInt(0, SCRIPTINSTANCE_CLIENT);
        return;
    }
    R_SetShaderConstantSetValue(constantSet, index, values);
    Scr_AddInt(1, SCRIPTINSTANCE_CLIENT);
    cgameGlob = CG_GetLocalClientGlobals(localClientNum);
    if ( cgameGlob->predictedPlayerEntity.nextValid
        && cgameGlob->predictedPlayerEntity.nextState.number == entref.entnum )
    {
        constantSet = &cgameGlob->predictedPlayerEntity.pose.constantSet;
        if ( R_MapShaderConstantSet(constantSet, index, constantName) )
            R_SetShaderConstantSetValue(constantSet, index, values);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (CScr_SetShaderConstant ~82949B08)
// Map: CoDSP_rd.map VA 0x82949B08 (cg_scr_main_coop.obj)
void __cdecl CScrCmd_SetShaderConstant(scr_entref_t entref)
{
    int localClientNum;
    unsigned int index;
    float values[4];
    centity_s *cent;
    cg_s *cgameGlob;

    if ( entref.classnum )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "not an entity", 0);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 6 )
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "USAGE: ent setshaderconstant( <localClientNum>, <index>, <x>, <y>, <z>, <w>)\n",
            0);
    localClientNum = CScr_GetLocalClientNum(0);
    index = Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT);
    values[0] = Scr_GetFloat(2u, SCRIPTINSTANCE_CLIENT);
    values[1] = Scr_GetFloat(3u, SCRIPTINSTANCE_CLIENT);
    values[2] = Scr_GetFloat(4u, SCRIPTINSTANCE_CLIENT);
    values[3] = Scr_GetFloat(5u, SCRIPTINSTANCE_CLIENT);
    cent = CScr_SP_GetMethodEntity(entref);
    if ( !cent )
        return;
    R_SetShaderConstantSetValue(&cent->pose.constantSet, index, values);
    cgameGlob = CG_GetLocalClientGlobals(localClientNum);
    if ( cgameGlob->predictedPlayerEntity.nextValid
        && cgameGlob->predictedPlayerEntity.nextState.number == entref.entnum )
    {
        R_SetShaderConstantSetValue(
            &cgameGlob->predictedPlayerEntity.pose.constantSet,
            index,
            values);
    }
}

static void __cdecl CScr_SP_RegAddFieldsForEntity()
{
    CScr_AddFieldsForEntity();
}

static void __cdecl CScr_SP_RegAddFieldsForRadiant()
{
    CScr_AddFieldsForRadiant();
}

static void __cdecl CScr_SP_RegFreeScripts()
{
    CScr_FreeScripts();
}

static void __cdecl CScr_SP_RegLoadLevel()
{
    CScr_LoadLevel();
}

static void __cdecl CScr_SP_RegLoadStructs()
{
    CScr_LoadStructs();
}

static void __cdecl CScr_SP_RegFPrintInternal()
{
    CScr_FPrint_internal(0);
}

static void __cdecl CScr_SP_RegPlayRumbleOnPosition()
{
    int localClientNum;
    const char *rumbleName;
    const char *configString;
    unsigned int i;
    int rumbleIndex;
    float pos[3];

    localClientNum = CScr_GetLocalClientNum(0);
    rumbleName = Scr_GetString(1u, SCRIPTINSTANCE_CLIENT);
    rumbleIndex = 0;
    for ( i = 0; i < 32; ++i )
    {
        configString = CL_GetConfigString(i + 1520);
        if ( configString && !I_stricmp(configString, rumbleName) )
        {
            rumbleIndex = (int)i;
            break;
        }
    }
    if ( !rumbleIndex )
    {
        Scr_ParamError(1u, va("unknown rumble name '%s'", rumbleName), SCRIPTINSTANCE_CLIENT);
        return;
    }
    Scr_GetVector(2u, pos, SCRIPTINSTANCE_CLIENT);
    (void)localClientNum;
    (void)rumbleIndex;
    (void)pos;
}

static void __cdecl CScr_SP_RegSetAnimInternal(scr_entref_t entref)
{
    CScr_SetAnimInternal(entref, 0, 0);
}

static void __cdecl CScr_SP_RegSetAnimKnobInternal(scr_entref_t entref)
{
    CScr_SetAnimKnobInternal(entref, 0, 0);
}

static void __cdecl CScr_SP_RegSetAnimKnobAllInternal(scr_entref_t entref)
{
    CScr_SetAnimKnobAllInternal(entref, 0, 0);
}

static void __cdecl Scr_SetCustomDynEntFieldReadOnly(unsigned __int16 absDynEntId, const dynent_field_s *pField)
{
    char *error;

    (void)absDynEntId;
    error = va("dynent field field %s is read-only", pField->name);
    Scr_Error(SCRIPTINSTANCE_CLIENT, error, 0);
}

static void __cdecl Scr_GetDynEntStringField(unsigned __int16 absDynEntId, const dynent_field_s *pField)
{
    const DynEntityDef *def;
    const char *value;

    def = DynEnt_GetEntityDef(absDynEntId);
    if ( !def )
        return;
    if ( *(unsigned __int16 *)((const char *)def + pField->ofs) )
    {
        value = SL_ConvertToString(*(unsigned __int16 *)((const char *)def + pField->ofs), SCRIPTINSTANCE_CLIENT);
        Scr_AddString((char *)value, SCRIPTINSTANCE_CLIENT);
    }
}

static const dynent_field_s dynent_fields[] =
{
  { "origin", F_VECTOR, offsetof(DynEntityPose, pose.origin), DYNENT_FIELD_POSE, Scr_SetCustomDynEntFieldReadOnly, 0 },
  { "angles", F_VECTOR, offsetof(DynEntityPose, pose.quat), DYNENT_FIELD_POSE, Scr_SetCustomDynEntFieldReadOnly, 0 },
  { "health", F_INT, offsetof(DynEntityClient, health), DYNENT_FIELD_CLIENT, 0, 0 },
  { "targetname", F_STRING, offsetof(DynEntityDef, targetname), DYNENT_FIELD_DEF, Scr_SetCustomDynEntFieldReadOnly, Scr_GetDynEntStringField },
  { 0, F_INT, 0, DYNENT_FIELD_CLIENT, 0, 0 }
};

static void *__cdecl CScr_GetDynEntFieldBase(unsigned __int16 absDynEntId, DynEntFieldEntityPart entityPart)
{
    if ( entityPart == DYNENT_FIELD_POSE )
        return (void *)DynEnt_GetClientPose(absDynEntId);
    if ( entityPart == DYNENT_FIELD_DEF )
        return (void *)DynEnt_GetEntityDef(absDynEntId);
    return (void *)DynEnt_GetClientEntity(absDynEntId);
}

void __cdecl CScr_GetDynEntityField(unsigned __int16 absDynEntId, int clientNum, unsigned int offset)
{
    const dynent_field_s *f;
    void *base;

    (void)clientNum;
    if ( !DynEnt_Valid(absDynEntId) )
        return;
    if ( offset >= 4 )
        return;
    f = &dynent_fields[offset];
    base = CScr_GetDynEntFieldBase(absDynEntId, f->entityPart);
    if ( !base )
        return;
    if ( f->getter )
        f->getter(absDynEntId, f);
    else
        Scr_GetGenericField((unsigned __int8 *)base, f->type, f->ofs, SCRIPTINSTANCE_CLIENT, 0);
}

int __cdecl CScr_SetDynEntityField(unsigned __int16 absDynEntId, int clientNum, unsigned int offset)
{
    const dynent_field_s *f;
    void *base;

    (void)clientNum;
    if ( !DynEnt_Valid(absDynEntId) )
        return 0;
    if ( offset >= 4 )
        return 0;
    f = &dynent_fields[offset];
    if ( f->setter )
    {
        f->setter(absDynEntId, f);
        return 1;
    }
    base = CScr_GetDynEntFieldBase(absDynEntId, f->entityPart);
    if ( !base )
        return 0;
    Scr_SetGenericField((unsigned __int8 *)base, f->type, f->ofs, SCRIPTINSTANCE_CLIENT, 0);
    return 1;
}

static void __cdecl CScr_SP_SetCustomDynEntFieldReadOnly()
{
    unsigned __int16 absDynEntId;
    unsigned int offset;

    absDynEntId = (unsigned __int16)Scr_GetInt(0, SCRIPTINSTANCE_CLIENT);
    offset = (unsigned int)Scr_GetInt(2u, SCRIPTINSTANCE_CLIENT);
    if ( offset >= 4 )
        Scr_ParamError(2u, "bad dynent field offset", SCRIPTINSTANCE_CLIENT);
    Scr_SetCustomDynEntFieldReadOnly(absDynEntId, &dynent_fields[offset]);
}

void __cdecl CScr_AddFieldsForDynEntity()
{
    const dynent_field_s *f;

    for ( f = dynent_fields; f->name; ++f )
        Scr_AddClassField(4, (char *)f->name, (unsigned __int16)(f - dynent_fields), SCRIPTINSTANCE_CLIENT);
}

// Decomp: CoDSP_rdBlackOps.map.c (CScr_UpdateGamerProfile @ 82946878)
void __cdecl CScr_UpdateGamerProfile()
{
    int localClientNum;
    int controllerIndex;

    localClientNum = CScr_GetLocalClientNum(0);
    if ( (unsigned int)localClientNum >= MAX_LOCAL_CLIENTS )
        return;
    if ( !CL_IsLocalClientInGame(localClientNum) )
        return;
    controllerIndex = Com_LocalClient_GetControllerIndex(localClientNum);
    GamerProfile_UpdateProfileFromDvars(controllerIndex, PROFILE_WRITE_IF_CHANGED);
}

// Decomp: CoDSP_rdBlackOps.map.c (CScr_UpdateDvarsFromProfile @ 82946948)
void __cdecl CScr_UpdateDvarsFromProfile()
{
    int localClientNum;
    int controllerIndex;

    localClientNum = CScr_GetLocalClientNum(0);
    if ( (unsigned int)localClientNum >= MAX_LOCAL_CLIENTS )
        return;
    if ( !CL_IsLocalClientInGame(localClientNum) )
        return;
    controllerIndex = Com_LocalClient_GetControllerIndex(localClientNum);
    GamerProfile_UpdateDvarsFromProfile(controllerIndex);
}

static bool __cdecl CScr_SetAlternateAimForEnt(int localClientNum, unsigned int entnum, bool enabled)
{
    cg_s *cgameGlob;
    int i;

    if ( !zombiemode->current.enabled )
        return false;
    for ( i = 0; i < MAX_LOCAL_CLIENTS; ++i )
    {
        if ( !CL_LocalClient_IsActive(i) )
            continue;
        cgameGlob = CG_GetLocalClientGlobals(i);
        if ( !cgameGlob->snap )
            continue;
        if ( cgameGlob->clientNum == (int)entnum )
        {
            s_useAlternateAimParams[i] = enabled;
            return true;
        }
    }
    (void)localClientNum;
    return false;
}

static void CScr_ApplyAlternateAimParams(scr_entref_t entref, bool enabled)
{
    bool applied;

    if ( !CG_IsLocalPlayer(entref.entnum) )
    {
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            enabled ? "You can only call UseAlternateAim on players." : "You can only call ClearAlternateAim on players.",
            0);
    }
    applied = CScr_SetAlternateAimForEnt(entref.client, entref.entnum, enabled);
    if ( zombiemode->current.enabled && !applied )
    {
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            enabled ? "UseAlternateAimParams had no effect..." : "ClearAlternateAimParams had no effect...",
            0);
    }
}

static void __cdecl CScr_UseAlternateAimParams_Method(scr_entref_t entref)
{
    CScr_ApplyAlternateAimParams(entref, true);
}

static void __cdecl CScr_ClearAlternateAimParams_Method(scr_entref_t entref)
{
    CScr_ApplyAlternateAimParams(entref, false);
}

// Decomp: CoDSP_rdBlackOps.map.c (CScr_UseAlternateAimParams @ 8294B1B8)
void __cdecl CScr_UseAlternateAimParams()
{
    CScr_ApplyAlternateAimParams(Scr_GetEntityRef(0, SCRIPTINSTANCE_CLIENT), true);
}

// Decomp: CoDSP_rdBlackOps.map.c (CScr_ClearAlternateAimParams @ 8294B2B8)
void __cdecl CScr_ClearAlternateAimParams()
{
    CScr_ApplyAlternateAimParams(Scr_GetEntityRef(0, SCRIPTINSTANCE_CLIENT), false);
}

// Decomp: CoDSP_rdBlackOps.map.c (cg_scr_main_coop.obj shims)
void __cdecl CScrCmd_ClientFlag(scr_entref_t entref)
{
    (void)entref;
    (void)Scr_GetNumParam(SCRIPTINSTANCE_CLIENT);
}

// Decomp: CoDSP_rdBlackOps.map.c (8294A8D8 CScrCmd_GetNormalizedCameraMovement)
void __cdecl CScrCmd_GetNormalizedCameraMovement(scr_entref_t entref)
{
    const usercmd_s *cmd;
    float move[3];

    if ( entref.classnum )
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_CLIENT);
    cmd = &cg_pmove[entref.client].cmd;
    move[0] = (float)cmd->pitchmove / -127.0f;
    move[1] = (float)cmd->yawmove / -127.0f;
    move[2] = 0.0f;
    Scr_AddVector(move, SCRIPTINSTANCE_CLIENT);
}

// Decomp: CoDSP_rdBlackOps.map.c (8294A810 CScrCmd_GetNormalizedMovement)
void __cdecl CScrCmd_GetNormalizedMovement(scr_entref_t entref)
{
    const usercmd_s *cmd;
    float move[3];

    if ( entref.classnum )
        Scr_ObjectError("not an entity", SCRIPTINSTANCE_CLIENT);
    cmd = &cg_pmove[entref.client].cmd;
    move[0] = (float)cmd->forwardmove / 127.0f;
    move[1] = (float)cmd->rightmove / 127.0f;
    move[2] = 0.0f;
    Scr_AddVector(move, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScrCmd_HideViewModel(scr_entref_t entref)
{
    cg_s *cgameGlob;

    cgameGlob = CG_GetLocalClientGlobals(entref.client);
    cgameGlob->hideViewModel = 1;
    PM_ResetWeaponState(&cgameGlob->predictedPlayerState);
}

void __cdecl CScrCmd_LinkToCamera(scr_entref_t entref)
{
    (void)entref;
}

static int s_scriptWatchStyle[MAX_LOCAL_CLIENTS];

// Decomp: CoDSP_rdBlackOps.map.c (CScrCmd_PlayRumbleOnEntity @ 823D3798)
void __cdecl CScrCmd_PlayRumbleOnEntity(scr_entref_t entref)
{
    CScrCmd_PlayRumbleOnEntity_Internal(entref, 95);
}

// Decomp: CoDSP_rdBlackOps.map.c (CScrCmd_PlayRumbleLoopOnEntity @ 823D3A38)
void __cdecl CScrCmd_PlayRumbleLoopOnEntity(scr_entref_t entref)
{
    CScrCmd_PlayRumbleOnEntity_Internal(entref, 97);
}

// Decomp: CoDSP_rdBlackOps.map.c (CScrCmd_StopRumble @ 823D3A78)
void __cdecl CScrCmd_StopRumble(scr_entref_t entref)
{
    (void)entref;
    (void)Scr_GetString(0, SCRIPTINSTANCE_CLIENT);
}

// Decomp: CoDSP_rdBlackOps.map.c (CScr_SetWatchStyle @ 823D9558)
void __cdecl CScr_SetWatchStyle(scr_entref_t entref)
{
    int style;
    int clamped;

    style = Scr_GetInt(0, SCRIPTINSTANCE_CLIENT);
    clamped = style > 0 ? style : 0;
    if ( clamped > 2 )
        clamped = 2;
    if ( entref.client >= 0 && entref.client < MAX_LOCAL_CLIENTS )
        s_scriptWatchStyle[entref.client] = clamped;
}

// Decomp: CoDSP_rdBlackOps.map.c (CScr_PlayRumbleOnPosition @ 823CFA30)
void __cdecl CScr_PlayRumbleOnPosition()
{
    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 3 )
        Scr_ParamError(0, "PlayRumbleOnPosition [local client number] [rumble name] [pos]", SCRIPTINSTANCE_CLIENT);
    CScr_SP_RegPlayRumbleOnPosition();
}

void __cdecl CScrCmd_PlayRumbleOnEntity_Internal(scr_entref_t entref, int cmd)
{
    centity_s *cent;
    int localClientNum;
    unsigned int i;

    (void)cmd;
    if ( entref.classnum )
    {
        Scr_Error(SCRIPTINSTANCE_CLIENT, "not an entity", 0);
        return;
    }
    if ( entref.entnum >= MAX_LOCAL_CENTITIES )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "entref.entnum < MAX_LOCAL_CENTITIES", 0);
    if ( CG_GetClientNumForLocalClient(entref.client) == entref.entnum )
        cent = &CG_GetLocalClientGlobals(entref.client)->predictedPlayerEntity;
    else
        cent = CG_GetEntity(entref.client, entref.entnum);
    if ( !cent->nextValid )
        return;
    (void)Scr_GetString(0, SCRIPTINSTANCE_CLIENT);
    for ( i = 0; i < MAX_LOCAL_CLIENTS; ++i )
    {
        if ( CL_LocalClient_IsActive(i) )
        {
            localClientNum = i;
            (void)localClientNum;
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (CScrCmd_FireWeapon @ 829493D0)
void __cdecl CScrCmd_FireWeapon(scr_entref_t entref)
{
    centity_s *cent;
    cg_s *cgameGlob;
    unsigned int weaponIndex;
    unsigned int tagName;
    int event;
    unsigned int i;
    int filterClient;
    bool hasFilter;
    const char *weaponName;
    VariableUnion tagString;

    if ( entref.classnum )
    {
        Scr_Error(SCRIPTINSTANCE_CLIENT, "not an entity", 0);
        return;
    }
    if ( entref.entnum >= MAX_LOCAL_CENTITIES )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "entref.entnum < MAX_LOCAL_CENTITIES", 0);
    if ( CG_GetClientNumForLocalClient(entref.client) == entref.entnum )
        cent = &CG_GetLocalClientGlobals(entref.client)->predictedPlayerEntity;
    else
        cent = CG_GetEntity(entref.client, entref.entnum);
    if ( !cent->nextValid )
        return;
    if ( !Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) )
    {
        for ( i = 0; i < MAX_LOCAL_CLIENTS; ++i )
        {
            if ( !CL_LocalClient_IsActive(i) )
                continue;
            cent = CG_GetEntity(i, entref.entnum);
            if ( !cent->nextValid || (cent->nextState.lerp.eFlags & 0x40000000) == 0 )
                continue;
            cgameGlob = CG_GetLocalClientGlobals(i);
            CG_FireWeapon(i, cent, EV_FIRE_WEAPON, scr_const.tag_flash, 0, &cgameGlob->predictedPlayerState, 0);
        }
        return;
    }
    if ( Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 2 && Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) != 3 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "Incorrect number of params for 'FireWeapon'.  Either 2,3 or none.", 0);
    weaponName = Scr_GetString(0, SCRIPTINSTANCE_CLIENT);
    weaponIndex = BG_GetWeaponIndexForName(weaponName);
    tagString.intValue = Scr_GetConstString(1u, SCRIPTINSTANCE_CLIENT);
    if ( tagString.stringValue == scr_const.tag_flash )
    {
        event = EV_FIRE_WEAPON;
        tagName = scr_const.tag_flash;
    }
    else
    {
        event = EV_FIRE_WEAPON_LEFT;
        tagName = scr_const.tag_flash_2;
    }
    hasFilter = Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) == 3;
    filterClient = hasFilter ? Scr_GetInt(2u, SCRIPTINSTANCE_CLIENT) : 0;
    if ( hasFilter && filterClient > 4 )
        Scr_Error(SCRIPTINSTANCE_CLIENT, "Out-of-range client number specified for param 3 of 'FireWeapon'.", 0);
    for ( i = 0; i < MAX_LOCAL_CLIENTS; ++i )
    {
        if ( hasFilter && (int)i != filterClient )
            continue;
        if ( !CL_LocalClient_IsActive(i) )
            continue;
        cent = CG_GetEntity(i, entref.entnum);
        if ( !cent->nextValid || (cent->nextState.lerp.eFlags & 0x40000000) == 0 )
            continue;
        cgameGlob = CG_GetLocalClientGlobals(i);
        CG_FireWeapon(i, cent, event, tagName, weaponIndex, &cgameGlob->predictedPlayerState, 0);
    }
}

void __cdecl CScrCmd_ShowViewModel(scr_entref_t entref)
{
    cg_s *cgameGlob;

    cgameGlob = CG_GetLocalClientGlobals(entref.client);
    cgameGlob->hideViewModel = 0;
    PM_ResetWeaponState(&cgameGlob->predictedPlayerState);
}

void __cdecl CScrCmd_Swimming(scr_entref_t entref)
{
    (void)entref;
    Scr_AddBool(0, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScrCmd_GetOrigin(scr_entref_t entref)
{
    centity_s *cent;

    cent = CScr_SP_GetMethodEntity(entref);
    if ( !cent )
        return;
    Scr_AddVector(cent->pose.origin, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScrCmd_GetAngles(scr_entref_t entref)
{
    cg_s *cg;

    cg = CG_GetLocalClientGlobals(entref.client);
    Scr_AddVector(cg->refdefViewAngles, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScrCmd_GetEye(scr_entref_t entref)
{
    cg_s *cg;
    float eye[3];
    float waterHeight;
    float delta;

    cg = CG_GetLocalClientGlobals(entref.client);
    eye[0] = cg->refdef.vieworg[0];
    eye[1] = cg->refdef.vieworg[1];
    eye[2] = cg->refdef.vieworg[2];
    waterHeight = CM_GetWaterHeight(eye, 200.0, -200.0);
    delta = eye[2] - waterHeight;
    if ( fabs(delta) < cg_cameraWaterClip->current.value )
    {
        if ( delta <= 0.0 )
            eye[2] = waterHeight - cg_cameraWaterClip->current.value;
        else
            eye[2] = waterHeight + cg_cameraWaterClip->current.value;
    }
    Scr_AddVector(eye, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScrCmd_GetEyeApprox(scr_entref_t entref)
{
    cg_s *cg;
    float eye[3];

    cg = CG_GetLocalClientGlobals(entref.client);
    eye[0] = cg->predictedPlayerState.origin[0];
    eye[1] = cg->predictedPlayerState.origin[1];
    eye[2] = cg->predictedPlayerState.origin[2] + cg->predictedPlayerState.viewHeightCurrent;
    Scr_AddVector(eye, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_GetWaterHeight()
{
    float pos[3];
    float height;

    Scr_GetVector(0, pos, SCRIPTINSTANCE_CLIENT);
    height = CM_GetWaterHeight(pos, 200.0, -200.0);
    Scr_AddFloat(height, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScrCmd_UnlinkAllFromCamera(scr_entref_t entref)
{
    (void)entref;
}

void __cdecl CScrCmd_UnlinkFromCamera(scr_entref_t entref)
{
    (void)entref;
}

void(__cdecl *__cdecl CScr_GetFunctionProjectSpecific(const char **pName, int *type))()
{
    unsigned int i; // [esp+18h] [ebp-4h]

    for (i = 0; i < ARRAY_COUNT(client_project_functions); ++i)
    {
        if ( !client_project_functions[i].actionString )
            break;
        if (!strcmp(*pName, client_project_functions[i].actionString))
        {
            *pName = client_project_functions[i].actionString;
            *type = client_project_functions[i].type;
            return client_project_functions[i].actionFunc;
        }
    }
    return 0;
}

// CoDMPServer.c CScrCmd_GetOwner @ 240352-240386
void __cdecl CScrCmd_GetOwner(scr_entref_t entref)
{
    centity_s *pOwner;
    int localClientNum;
    centity_s *pSelf;
    int ownerEntNum;

    if ( entref.classnum )
    {
        Scr_Error(SCRIPTINSTANCE_CLIENT, "not an entity", 0);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x600u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                        1227,
                        0,
                        "%s",
                        "entref.entnum < MAX_LOCAL_CENTITIES") )
        {
            __debugbreak();
        }
        if ( CG_GetClientNumForLocalClient(entref.client) == entref.entnum )
            pSelf = &CG_GetLocalClientGlobals(entref.client)->predictedPlayerEntity;
        else
            pSelf = CG_GetEntity(entref.client, entref.entnum);
        if ( entref.entnum >= 0x400u )
            CG_GetFakeEntity(entref.client, entref.entnum);
    }
    localClientNum = CScr_GetLocalClientNum(0);
    // BlackOpsMP.retail.c:144413-144422 — undefined if cent invalid or owner index out of range
    if ( !pSelf->nextValid )
    {
        Scr_AddUndefined(SCRIPTINSTANCE_CLIENT);
        return;
    }
    ownerEntNum = (int)(unsigned __int8)pSelf->nextState.faction.iHeadIconTeam >> 2;
    if ( (unsigned int)ownerEntNum >= 0x400u )
    {
        Scr_AddUndefined(SCRIPTINSTANCE_CLIENT);
        return;
    }
    pOwner = CG_GetEntity(localClientNum, ownerEntNum);
    CScr_AddEntity(pOwner, localClientNum);
}

void __cdecl CScr_GetTagOrigin(scr_entref_t entref)
{
    VariableUnion v1; // eax
    unsigned int tagName; // [esp+Ch] [ebp-10h]
    centity_s *pSelf; // [esp+10h] [ebp-Ch]
    char *name; // [esp+14h] [ebp-8h]

    if ( entref.classnum )
    {
        Scr_Error(SCRIPTINSTANCE_CLIENT, "not an entity", 0);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x600u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                        1288,
                        0,
                        "%s",
                        "entref.entnum < MAX_LOCAL_CENTITIES") )
        {
            __debugbreak();
        }
        if ( CG_GetClientNumForLocalClient(entref.client) == entref.entnum )
            pSelf = &CG_GetLocalClientGlobals(entref.client)->predictedPlayerEntity;
        else
            pSelf = CG_GetEntity(entref.client, entref.entnum);
        if ( entref.entnum >= 0x400u )
            CG_GetFakeEntity(entref.client, entref.entnum);
    }
    v1.intValue = Scr_GetConstLowercaseString(0, SCRIPTINSTANCE_CLIENT);
    name = SL_ConvertToString(v1.stringValue, SCRIPTINSTANCE_CLIENT);
    tagName = SL_FindLowercaseString(name, SCRIPTINSTANCE_SERVER);
    CScr_UpdateTagInternal(pSelf, tagName, &cg_cachedTagMat);
    Scr_AddVector(cg_cachedTagMat.tagMat[3], SCRIPTINSTANCE_CLIENT);
}

int __cdecl CScr_UpdateTagInternal(centity_s *ent, unsigned int tagName, cached_tag_mat_t *cachedTag)
{
    char *v4; // eax
    cg_s *LocalClientGlobals; // eax
    const char *name; // [esp-4h] [ebp-18h]
    char *error; // [esp+0h] [ebp-14h]
    DObj *obj; // [esp+Ch] [ebp-8h]
    cg_s *cgameGlob; // [esp+10h] [ebp-4h]

    if ( !ent
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp", 1236, 0, "%s", "ent") )
    {
        __debugbreak();
    }
    cgameGlob = CG_GetLocalClientGlobals(ent->pose.localClientNum);
    if ( ent->nextState.number != cachedTag->entnum || cgameGlob->time != cachedTag->time || tagName != cachedTag->name )
    {
        obj = Com_GetClientDObj(ent->nextState.number, ent->pose.localClientNum);
        if ( !obj )
        {
            Scr_ObjectError("entity has no model defined", SCRIPTINSTANCE_CLIENT);
            return 0;
        }
        if ( !CG_DObjGetWorldTagMatrix(&ent->pose, obj, tagName, cachedTag->tagMat, cachedTag->tagMat[3]) )
        {
            name = DObjGetModel(obj, 0)->name;
            v4 = SL_ConvertToString(tagName, SCRIPTINSTANCE_CLIENT);
            error = va("tag '%s' does not exist in model '%s' (or any attached submodels)", v4, name);
            Scr_Error(SCRIPTINSTANCE_CLIENT, error, 0);
            return 0;
        }
        LocalClientGlobals = CG_GetLocalClientGlobals(ent->pose.localClientNum);
        cachedTag->entnum = ent->nextState.number;
        cachedTag->time = LocalClientGlobals->time;
        cachedTag->name = tagName;
    }
    return 1;
}

void __cdecl CScr_GetTagAngles(scr_entref_t entref)
{
    VariableUnion v1; // eax
    unsigned int tagName; // [esp+Ch] [ebp-1Ch]
    centity_s *pSelf; // [esp+10h] [ebp-18h]
    char *name; // [esp+14h] [ebp-14h]
    float angles[3]; // [esp+18h] [ebp-10h] BYREF
    fake_centity_s *pFake; // [esp+24h] [ebp-4h]

    if ( entref.classnum )
    {
        Scr_Error(SCRIPTINSTANCE_CLIENT, "not an entity", 0);
        pSelf = 0;
        pFake = 0;
    }
    else
    {
        if ( entref.entnum >= 0x600u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                        1314,
                        0,
                        "%s",
                        "entref.entnum < MAX_LOCAL_CENTITIES") )
        {
            __debugbreak();
        }
        if ( CG_GetClientNumForLocalClient(entref.client) == entref.entnum )
            pSelf = &CG_GetLocalClientGlobals(entref.client)->predictedPlayerEntity;
        else
            pSelf = CG_GetEntity(entref.client, entref.entnum);
        if ( entref.entnum < 0x400u )
            pFake = 0;
        else
            pFake = CG_GetFakeEntity(entref.client, entref.entnum);
    }
    v1.intValue = Scr_GetConstLowercaseString(0, SCRIPTINSTANCE_CLIENT);
    name = SL_ConvertToString(v1.stringValue, SCRIPTINSTANCE_CLIENT);
    tagName = SL_FindLowercaseString(name, SCRIPTINSTANCE_SERVER);
    CScr_UpdateTagInternal(pSelf, tagName, &cg_cachedTagMat);
    AxisToAngles(cg_cachedTagMat.tagMat, angles);
    Scr_AddVector(angles, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_GetTagForwardVector(scr_entref_t entref)
{
    VariableUnion v1;
    unsigned int tagName;
    centity_s *pSelf;
    char *name;
    float forward[3];

    if ( entref.classnum )
    {
        Scr_Error(SCRIPTINSTANCE_CLIENT, "not an entity", 0);
        return;
    }
    if ( entref.entnum >= 0x600u
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                    1314,
                    0,
                    "%s",
                    "entref.entnum < MAX_LOCAL_CENTITIES") )
    {
        __debugbreak();
    }
    if ( CG_GetClientNumForLocalClient(entref.client) == entref.entnum )
        pSelf = &CG_GetLocalClientGlobals(entref.client)->predictedPlayerEntity;
    else
        pSelf = CG_GetEntity(entref.client, entref.entnum);
    if ( entref.entnum >= 0x400u )
        CG_GetFakeEntity(entref.client, entref.entnum);
    v1.intValue = Scr_GetConstLowercaseString(0, SCRIPTINSTANCE_CLIENT);
    name = SL_ConvertToString(v1.stringValue, SCRIPTINSTANCE_CLIENT);
    tagName = SL_FindLowercaseString(name, SCRIPTINSTANCE_SERVER);
    CScr_UpdateTagInternal(pSelf, tagName, &cg_cachedTagMat);
    forward[0] = cg_cachedTagMat.tagMat[0][0];
    forward[1] = cg_cachedTagMat.tagMat[0][1];
    forward[2] = cg_cachedTagMat.tagMat[0][2];
    Scr_AddVector(forward, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScrCmd_ShellShock(scr_entref_t entref)
{
    shellshock_parms_t *ShellshockParms; // eax
    int time; // [esp+0h] [ebp-50h]
    char *v3; // [esp+8h] [ebp-48h]
    char *error; // [esp+Ch] [ebp-44h]
    float Float; // [esp+20h] [ebp-30h]
    int duration; // [esp+34h] [ebp-1Ch]
    char *shock; // [esp+38h] [ebp-18h]
    cg_s *cgameGlob; // [esp+3Ch] [ebp-14h]
    int localClientNum; // [esp+44h] [ebp-Ch]
    char *configString; // [esp+48h] [ebp-8h]
    signed int id; // [esp+4Ch] [ebp-4h]

    CG_GetEntity(entref.client, entref.entnum);
    if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) < 3 )
        Scr_Error(
            SCRIPTINSTANCE_CLIENT,
            "USAGE: <player> shellshock( <local client number>, <shellshockname>, <duration>)\n",
            0);
    localClientNum = CScr_GetLocalClientNum(0);
    shock = Scr_GetString(1u, SCRIPTINSTANCE_CLIENT);
    Float = Scr_GetFloat(2u, SCRIPTINSTANCE_CLIENT);
    duration = (int)((float)(1000.0 * Float) + 9.313225746154785e-10);
    if ( (unsigned int)duration > 0xEA60 )
    {
        error = va("duration %g should be >= 0 and <= 60", (float)((float)duration * 0.001));
        Scr_ParamError(1u, error, SCRIPTINSTANCE_CLIENT);
    }
    cgameGlob = CG_GetLocalClientGlobals(localClientNum);
    for ( id = 1; id < 16; ++id )
    {
        configString = CL_GetConfigString(id + 2532);
        if ( !I_stricmp(configString, shock) )
        {
            time = cgameGlob->time;
            ShellshockParms = BG_GetShellshockParms(id);
            CG_StartShellShock(cgameGlob, ShellshockParms, time, duration);
            return;
        }
    }
    v3 = va(
                 "shellshock '%s' was not precached.    You need to precache it in the server script before calling it on the client.\n",
                 shock);
    Scr_Error(SCRIPTINSTANCE_CLIENT, v3, 0);
}

void __cdecl CScr_SetEnemyGlobalScrambler(scr_entref_t entref)
{
    cg_s *cgameGlob; // [esp+0h] [ebp-8h]

    cgameGlob = CG_GetLocalClientGlobals(entref.client);
    cgameGlob->globalScramblerActive = Scr_GetInt(0, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_SetEnemyScramblerAmount(scr_entref_t entref)
{
    float alphaAmount; // [esp+0h] [ebp-8h]
    cg_s *cgameGlob; // [esp+4h] [ebp-4h]

    cgameGlob = CG_GetLocalClientGlobals(entref.client);
    alphaAmount = Scr_GetFloat(0, SCRIPTINSTANCE_CLIENT);
    if ( alphaAmount < 0.0 || alphaAmount > 1.0 )
        Scr_Error("float value must be between 0 and 1", 0);
    cgameGlob->scramblerEnemyAlpha = alphaAmount;
}

void __cdecl CScr_SetFriendlyScramblerAmount(scr_entref_t entref)
{
    float alphaAmount; // [esp+0h] [ebp-8h]
    cg_s *cgameGlob; // [esp+4h] [ebp-4h]

    cgameGlob = CG_GetLocalClientGlobals(entref.client);
    alphaAmount = Scr_GetFloat(0, SCRIPTINSTANCE_CLIENT);
    if ( alphaAmount < 0.0 || alphaAmount > 1.0 )
        Scr_Error("float value must be between 0 and 1", 0);
    cgameGlob->scramblerFriendlyAlpha = alphaAmount;
}

void __cdecl CScr_GetFriendlyScramblerAmount(scr_entref_t entref)
{
    float value; // [esp+8h] [ebp-8h]

    value = CG_GetLocalClientGlobals(entref.client)->scramblerFriendlyAlpha;
    Scr_AddFloat(value, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_GetEnemyScramblerAmount(scr_entref_t entref)
{
    float value; // [esp+8h] [ebp-8h]

    value = CG_GetLocalClientGlobals(entref.client)->scramblerEnemyAlpha;
    Scr_AddFloat(value, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_IsScrambled(scr_entref_t entref)
{
    unsigned int IsScrambled; // [esp+4h] [ebp-4h]

    IsScrambled = CG_GetLocalClientGlobals(entref.client)->scramblerEnemyAlpha > 0.01;
    Scr_AddBool(IsScrambled, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_SetNearestEnemyScrambler(scr_entref_t entref)
{
    scr_entref_t v1; // [esp+8h] [ebp-2Ch] BYREF
    scr_entref_t v2; // [esp+12h] [ebp-22h]
    scr_entref_t v3; // [esp+18h] [ebp-1Ch]
    scr_entref_t v4; // [esp+22h] [ebp-12h]
    centity_s *cent; // [esp+28h] [ebp-Ch]
    scr_entref_t scramblerEntity; // [esp+2Ch] [ebp-8h]

    //v2 = *Scr_GetEntityRef(&v1, 0, SCRIPTINSTANCE_CLIENT);
    v2 = Scr_GetEntityRef(0, SCRIPTINSTANCE_CLIENT);
    v3 = v2;
    v4 = v2;
    scramblerEntity = v2;
    cent = CG_GetEntity(v2.client, v2.entnum);
    CG_AddEnemyScrambler(entref.client, cent);
}

void __cdecl CScr_ClearNearestEnemyScrambler(scr_entref_t entref)
{
    CG_ClearNearestEnemyScrambler(entref.client);
}

void __cdecl CScr_AddFriendlyScrambler(scr_entref_t entref)
{
    VariableUnion scramblerHandle; // [esp+Ch] [ebp-Ch]
    float scramblerY; // [esp+10h] [ebp-8h]
    float scramblerX; // [esp+14h] [ebp-4h]

    scramblerX = Scr_GetFloat(0, SCRIPTINSTANCE_CLIENT);
    scramblerY = Scr_GetFloat(1u, SCRIPTINSTANCE_CLIENT);
    scramblerHandle.intValue = Scr_GetInt(2u, SCRIPTINSTANCE_CLIENT);
    CG_AddFriendlyScrambler(entref.client, scramblerX, scramblerY, scramblerHandle.intValue);
}

void __cdecl CScr_RemoveFriendlyScrambler(scr_entref_t entref)
{
    VariableUnion scramblerHandle; // [esp+0h] [ebp-4h]

    scramblerHandle.intValue = Scr_GetInt(0, SCRIPTINSTANCE_CLIENT);
    CG_RemoveFriendlyScrambler(entref.client, scramblerHandle.intValue);
}

void __cdecl CScr_RemoveAllFriendlyScramblers(scr_entref_t entref)
{
    CG_RemoveAllFriendlyScramblers(entref.client);
}

void __cdecl CScr_HasTacticalMaskOverlay(scr_entref_t entref)
{
    if ( CG_IsShowingZombieMap() )
        Scr_AddInt(1, SCRIPTINSTANCE_CLIENT);
    else
        Scr_AddInt(0, SCRIPTINSTANCE_CLIENT);
}

void __cdecl CScr_GetStance(scr_entref_t entref)
{
    centity_s *pSelf; // [esp+Ch] [ebp-8h]

    if ( entref.classnum )
    {
        Scr_Error(SCRIPTINSTANCE_CLIENT, "not an entity", 0);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x600u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                        1663,
                        0,
                        "%s",
                        "entref.entnum < MAX_LOCAL_CENTITIES") )
        {
            __debugbreak();
        }
        if ( CG_GetClientNumForLocalClient(entref.client) == entref.entnum )
            pSelf = &CG_GetLocalClientGlobals(entref.client)->predictedPlayerEntity;
        else
            pSelf = CG_GetEntity(entref.client, entref.entnum);
        if ( entref.entnum >= 0x400u )
            CG_GetFakeEntity(entref.client, entref.entnum);
    }
    if ( ((*((unsigned int *)pSelf + 201) >> 1) & 1) == 0 )
        Scr_Error("GetStance can not be used on a player that is not in the snapshot.", 0);
    if ( pSelf->nextState.eType == 1 || pSelf->nextState.eType == 5 )
    {
        if ( (pSelf->nextState.lerp.eFlags & 8) != 0 )
        {
            Scr_AddConstString(cscr_const.prone, SCRIPTINSTANCE_CLIENT);
        }
        else if ( (pSelf->nextState.lerp.eFlags & 4) != 0 )
        {
            Scr_AddConstString(cscr_const.crouch, SCRIPTINSTANCE_CLIENT);
        }
        else
        {
            Scr_AddConstString(cscr_const.stand, SCRIPTINSTANCE_CLIENT);
        }
    }
    else
    {
        Scr_Error("GetStance is only defined for players.", 0);
    }
}

void __cdecl CScr_SetFlagAsAway(scr_entref_t entref)
{
    centity_s *pSelf; // [esp+14h] [ebp-18h]
    team_t team; // [esp+18h] [ebp-14h]
    cg_s *cgameGlob; // [esp+1Ch] [ebp-10h]
    VariableUnion localClientNum; // [esp+20h] [ebp-Ch]
    int away; // [esp+28h] [ebp-4h]

    if ( entref.classnum )
    {
        Scr_Error(SCRIPTINSTANCE_CLIENT, "not an entity", 0);
        pSelf = 0;
    }
    else
    {
        if ( entref.entnum >= 0x600u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                        1702,
                        0,
                        "%s",
                        "entref.entnum < MAX_LOCAL_CENTITIES") )
        {
            __debugbreak();
        }
        if ( CG_GetClientNumForLocalClient(entref.client) == entref.entnum )
            pSelf = &CG_GetLocalClientGlobals(entref.client)->predictedPlayerEntity;
        else
            pSelf = CG_GetEntity(entref.client, entref.entnum);
        if ( entref.entnum >= 0x400u )
            CG_GetFakeEntity(entref.client, entref.entnum);
    }
    if ( ((*((unsigned int *)pSelf + 201) >> 1) & 1) != 0 )
    {
        if ( (unsigned int)Scr_GetNumParam(SCRIPTINSTANCE_CLIENT) >= 2 )
        {
            localClientNum.intValue = CScr_GetLocalClientNum(0);
            away = Scr_GetInt(1u, SCRIPTINSTANCE_CLIENT);
            cgameGlob = CG_GetLocalClientGlobals(localClientNum.intValue);
            team = (team_t)(pSelf->nextState.faction.iHeadIconTeam & 3);
            if ( team == TEAM_ALLIES )
            {
                cgameGlob->alliesFlagAway = away != 0;
            }
            else if ( team == TEAM_AXIS )
            {
                cgameGlob->axisFlagAway = away != 0;
            }
        }
        else
        {
            Scr_Error(SCRIPTINSTANCE_CLIENT, "Not enough parameters supplied to SetFlagAsAway", 0);
        }
    }
    else
    {
        Scr_Error(SCRIPTINSTANCE_CLIENT, "SetFlagAsAway must be called on a valid entity", 0);
    }
}

void __cdecl CScr_GetParentEntity(scr_entref_t entref)
{
    centity_s *cent; // [esp+8h] [ebp-8h]
    centity_s *parent; // [esp+Ch] [ebp-4h]

    cent = CG_GetEntity(entref.client, entref.entnum);
    parent = CG_EntGetLinkToParent(entref.client, cent);
    if ( parent )
        CScr_AddEntity(parent, entref.client);
    else
        Scr_AddUndefined(SCRIPTINSTANCE_CLIENT);
}

void (__cdecl *__cdecl CScr_GetMethodProjectSpecific(const char **pName, int *type))(scr_entref_t)
{
    unsigned int i; // [esp+18h] [ebp-4h]

    for ( i = 0; i < ARRAY_COUNT(client_project_methods); ++i )
    {
        if ( !strcmp(*pName, client_project_methods[i].actionString) )
        {
            *pName = client_project_methods[i].actionString;
            *type = client_project_methods[i].type;
            return client_project_methods[i].actionFunc;
        }
    }
    return 0;
}

void __cdecl CScr_SetUniqueClientScripts(ScriptFunctions *functions)
{
    // Decomp: BlackOps.singleplayer.c sub_408EE0 — postload consumes load-phase handles in this order.
    cg_scr_data.delete_ = CScr_SetScriptAndLabel(functions, "codescripts/delete", "main", 1);
    cg_scr_data.initstructs = CScr_SetScriptAndLabel(functions, "codescripts/struct", "initstructs", 1);
    cg_scr_data.createstruct = CScr_SetScriptAndLabel(functions, "codescripts/struct", "createstruct", 1);
    cg_scr_data.findstruct = CScr_SetScriptAndLabel(functions, "codescripts/struct", "findstruct", 1);
    cg_scr_data.clientsysstatechange = CScr_SetScriptAndLabel(functions, "clientscripts/_callbacks", "statechange", 1);
    cg_scr_data.maprestart = CScr_SetScriptAndLabel(functions, "clientscripts/_callbacks", "maprestart", 1);
    cg_scr_data.localclientconnect = CScr_SetScriptAndLabel(functions, "clientscripts/_callbacks", "localclientconnect", 1);
    cg_scr_data.localclientdisconnect = CScr_SetScriptAndLabel(functions, "clientscripts/_callbacks", "localclientdisconnect", 1);
    cg_scr_data.entityspawned = CScr_SetScriptAndLabel(functions, "clientscripts/_callbacks", "entityspawned", 1);
    cg_scr_data.playerspawned = CScr_SetScriptAndLabel(functions, "clientscripts/mp/_callbacks", "playerspawned", 1);
    cg_scr_data.scriptmodelspawned = CScr_SetScriptAndLabel(functions, "clientscripts/_callbacks", "scriptmodelspawned", 1);
    cg_scr_data.clientFlagCB = CScr_SetScriptAndLabel(functions, "clientscripts/_callbacks", "client_flag_callback", 0);
    cg_scr_data.clientFlagAsValCB = CScr_SetScriptAndLabel(functions, "clientscripts/_callbacks", "client_flagasval_callback", 0);
    cg_scr_data.destructible_callback = CScr_SetScriptAndLabel(
        functions, "clientscripts/_destructible", "CodeCallback_DestructibleEvent", 0);
    cg_scr_data.playerFootstep = CScr_SetScriptAndLabel(
        functions, "clientscripts/mp/_callbacks", "CodeCallback_PlayerFootstep", 1);
    cg_scr_data.playerJump = CScr_SetScriptAndLabel(
        functions, "clientscripts/mp/_callbacks", "CodeCallback_PlayerJump", 1);
    cg_scr_data.playerLand = CScr_SetScriptAndLabel(
        functions, "clientscripts/mp/_callbacks", "CodeCallback_PlayerLand", 1);
    cg_scr_data.playerFoliage = CScr_SetScriptAndLabel(
        functions, "clientscripts/mp/_callbacks", "CodeCallback_PlayerFoliage", 1);
    cg_scr_sp_data.playAIFootstep = CScr_SetScriptAndLabel(functions, "clientscripts/_footsteps", "playAIFootstep", 1);
    s_activateExploderCallback = CScr_SetScriptAndLabel(
        functions, "clientscripts/_callbacks", "callback_activate_exploder", 1);
    s_deactivateExploderCallback = CScr_SetScriptAndLabel(
        functions, "clientscripts/_callbacks", "callback_deactivate_exploder", 1);
    cg_scr_data.levelnotify = CScr_SetScriptAndLabel(functions, "clientscripts/_callbacks", "level_notify", 1);
    cg_scr_data.dogSoundNotify = CScr_SetScriptAndLabel(functions, "clientscripts/_callbacks", "sound_notify", 0);
    if ( zombietron && zombietron->current.enabled )
        s_zombieEyeCallback = CScr_SetScriptAndLabel(
            functions, "clientscripts/_zombietron", "zombie_eye_callback", 0);
    else
        s_zombieEyeCallback = CScr_SetScriptAndLabel(
            functions, "clientscripts/_zombiemode", "zombie_eye_callback", 0);
    s_playWeaponDeathEffects = CScr_SetScriptAndLabel(
        functions, "clientscripts/_callbacks", "CodeCallback_PlayWeaponDeathEffects", 0);
    s_playWeaponDamageEffects = CScr_SetScriptAndLabel(
        functions, "clientscripts/_callbacks", "CodeCallback_PlayWeaponDamageEffects", 0);
    cg_scr_data.airsupport = CScr_SetScriptAndLabel(functions, "clientscripts/_callbacks", "airsupport", 1);
    cg_scr_data.entityshutdownCB = CScr_SetScriptAndLabel(
        functions, "clientscripts/_callbacks", "entityshutdown_callback", 1);
    cg_scr_data.glassSmash = CScr_SetScriptAndLabel(functions, "clientscripts/mp/_callbacks", "glass_smash", 0);
    cg_scr_data.gibEvent = CScr_SetScriptAndLabel(functions, "clientscripts/_callbacks", "CodeCallback_GibEvent", 0);
}

void __cdecl CG_SendSwimNotify(int localClientNum, unsigned int clientNum, int start)
{
    unsigned __int16 swimming_begin; // [esp+2h] [ebp-2h]

    if ( start )
        swimming_begin = cscr_const.swimming_begin;
    else
        swimming_begin = cscr_const.swimming_end;
    CScr_NotifyNum(localClientNum, clientNum, 0, swimming_begin, 0);
}

void __cdecl CScr_GetEntityByIndex(centity_s *cent, const cent_field_s *pField)
{
    centity_s *Entity; // eax
    unsigned int index; // [esp+10h] [ebp-4h]

    if ( !cent
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp", 1867, 0, "%s", "cent") )
    {
        __debugbreak();
    }
    if ( !pField
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp", 1868, 0, "%s", "pField") )
    {
        __debugbreak();
    }
    index = GetField((const int *)((char *)cent + pField->ofs), pField->size[0]);
    if ( index < 0x3FE )
    {
        Entity = CG_GetEntity(cent->pose.localClientNum, index);
        CScr_AddEntity(Entity, cent->pose.localClientNum);
    }
}

int __cdecl GetField(const int *i, int size)
{
    switch ( size )
    {
        case 1:
            return *(char *)i;
        case 2:
            return *(__int16 *)i;
        case 4:
            return *i;
    }
    if ( !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                    1861,
                    0,
                    "unknown field size") )
        __debugbreak();
    return 0;
}

void __cdecl CScr_GetTeamName(centity_s *cent, const cent_field_s *pField)
{
    team_t team; // [esp+0h] [ebp-4h]

    if ( !cent
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp", 1902, 0, "%s", "cent") )
    {
        __debugbreak();
    }
    if ( !pField
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp", 1903, 0, "%s", "pField") )
    {
        __debugbreak();
    }
    team = GetTeam(cent);
    CScr_AddTeamName(team);
}

team_t __cdecl GetTeam(centity_s *cent)
{
    cg_s *cgameGlob; // [esp+0h] [ebp-8h]
    unsigned int localClientNum; // [esp+4h] [ebp-4h]

    if ( !cent
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp", 1882, 0, "%s", "cent") )
    {
        __debugbreak();
    }
    if ( cent->nextState.eType != 1 )
        return (team_t)(cent->nextState.faction.iHeadIconTeam & 3);
    localClientNum = RETURN_ZERO32();
    if ( localClientNum >= 2
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                    1888,
                    0,
                    "localClientNum not in [0, MAX_LOCAL_CLIENTS]\n\t%i not in [%i, %i]",
                    localClientNum,
                    0,
                    1) )
    {
        __debugbreak();
    }
    cgameGlob = CG_GetLocalClientGlobals(localClientNum);
    if ( cgameGlob->bgs.clientinfo[cent->nextState.clientNum].infoValid )
        return cgameGlob->bgs.clientinfo[cent->nextState.clientNum].team;
    else
        return TEAM_BAD;
}

unsigned __int16 __cdecl CScr_GetFootTag(eFoot foot)
{
    if ( (unsigned int)foot >= FOOTSTEP_COUNT
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\cgame\\\cg_scr_main_sp.cpp",
                    1923,
                    0,
                    "foot doesn't index FOOTSTEP_COUNT\n\t%i not in [0, %i)",
                    foot,
                    4) )
    {
        __debugbreak();
    }
    return *footTags[foot];
}

float footprintGroundTraceUp = 15.0f;
float footprintGroundTraceDown = 15.0f;

void __cdecl CScr_PlayDogstepSound(int localClientNum, centity_s *cent, eFoot foot)
{
    unsigned __int16 FootTag; // ax
    char *value; // [esp+0h] [ebp-68h]
    unsigned __int16 t; // [esp+14h] [ebp-54h]
    DObj *obj; // [esp+18h] [ebp-50h]
    float start[3]; // [esp+1Ch] [ebp-4Ch] BYREF
    float end[3]; // [esp+28h] [ebp-40h] BYREF
    int surfType; // [esp+34h] [ebp-34h]
    float footMatrix[4][3]; // [esp+38h] [ebp-30h] BYREF

    if ( cg_scr_sp_data.dogstep )
    {
        obj = Com_GetClientDObj(cent->nextState.number, localClientNum);
        if ( !obj
            || (FootTag = CScr_GetFootTag(foot),
                    !CG_DObjGetWorldTagMatrix(&cent->pose, obj, FootTag, footMatrix, footMatrix[3])) )
        {
            footMatrix[3][0] = cent->pose.origin[0];
            footMatrix[3][1] = cent->pose.origin[1];
            footMatrix[3][2] = cent->pose.origin[2];
        }
        *(_QWORD *)start = *(_QWORD *)&footMatrix[3][0];
        start[2] = footMatrix[3][2] + footprintGroundTraceUp;
        *(_QWORD *)end = *(_QWORD *)&footMatrix[3][0];
        end[2] = footMatrix[3][2] - footprintGroundTraceDown;
        surfType = (CM_TracePointDown(start, end, 2065, 0x3F00000, footMatrix[3], 0, 0) & 0x3F00000) >> 20;
        if ( (cent->currentState.eFlags2 & 0x200000) != 0 || ((*((unsigned int *)cent + 201) >> 5) & 1) != 0 )
            Scr_AddInt(1, SCRIPTINSTANCE_CLIENT);
        else
            Scr_AddInt(0, SCRIPTINSTANCE_CLIENT);
        value = (char *)Com_SurfaceTypeToName(surfType);
        Scr_AddString(value, SCRIPTINSTANCE_CLIENT);
        Scr_AddVector(footMatrix[3], SCRIPTINSTANCE_CLIENT);
        CScr_AddEntity(cent, localClientNum);
        Scr_AddInt(localClientNum, SCRIPTINSTANCE_CLIENT);
        t = Scr_ExecThread(SCRIPTINSTANCE_CLIENT, cg_scr_sp_data.dogstep, 5u);
        Scr_FreeThread(t, SCRIPTINSTANCE_CLIENT);
    }
}

