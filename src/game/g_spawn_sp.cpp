#include "g_spawn_sp.h"
#include <game/g_load_utils.h>
#include <clientscript/cscr_vm.h>
#include <game/g_utils_sp.h>
#include <clientscript/cscr_stringlist.h>
#include <server_sp/sv_init_sp.h>
#include <game/pathnode_load_obj.h>
#include <game/actor.h>
#include <game/teams.h>
#include <game/actor_wrapper.h>
#include <clientscript/scr_const.h>
#include <game/g_mover.h>
#include "g_trigger_sp.h"
#include "g_misc_sp.h"
#include "game/turret.h"
#include <game/g_scr_mover.h>
#include <bgame/bg_misc.h>
#include <game/g_weapon.h>
#include <game/g_client_fields.h>
#include <cgame/cg_hudelem.h>
#include <bgame/bg_weapons_def.h>
#include "g_scr_main.h"
#include "g_vehicle_path.h"
#include <universal/com_math_anglevectors.h>
#include <qcommon/cm_load.h>
#include <qcommon/cm_world.h>
#include <xanim/xmodel.h>
#include <dynentity/DynEntity_client.h>

struct ent_field_t // sizeof=0x14
{                                       // XREF: .rdata:fields_1/r
    const char *name;
    int ofs;
    int size[1];
    fieldtype_t type;
    void (__cdecl *callback)(gentity_s *, int);
};

#define ENTFIELD_MAX 15

static void G_ParseTeamField(gentity_s *ent, char *value)
{
    if ( !I_stricmp(value, "allies") )
    {
        ent->team = 2;
        if ( ent->sentient )
            ent->sentient->eTeam = TEAM_ALLIES;
    }
    else if ( !I_stricmp(value, "axis") )
    {
        ent->team = 1;
        if ( ent->sentient )
            ent->sentient->eTeam = TEAM_AXIS;
    }
    else
    {
        ent->team = 0;
        if ( ent->sentient )
            ent->sentient->eTeam = TEAM_FREE;
    }
}

const ent_field_t fields_1[16] =
{
  { "classname", 356, { 2 }, F_STRING, Scr_ReadOnlyField },
  { "origin", 292, { 12 }, F_VECTOR, Scr_SetOrigin },
  { "model", 348, { 2 }, F_MODEL, Scr_ReadOnlyField },
  { "spawnflags", 368, { 4 }, F_INT, Scr_ReadOnlyField },
  { "target", 358, { 2 }, F_STRING, NULL },
  { "targetname", 360, { 2 }, F_STRING, NULL },
  { "script_noteworthy", 362, { 2 }, F_STRING, NULL },
  { "count", 532, { 4 }, F_INT, NULL },
  { "health", 404, { 4 }, F_INT, Scr_SetHealth },
  { "dmg", 412, { 4 }, F_INT, NULL },
  { "angles", 304, { 12 }, F_VECTOR, Scr_SetAngles },
  { "birthtime", 748, { 4 }, F_INT, Scr_ReadOnlyField },
  { "index", 560, { 4 }, F_INT, Scr_SetExposureIndex },
  { "lerp_to_lighter", 564, { 4 }, F_FLOAT, Scr_SetExposureLerpToLighter },
  { "lerp_to_darker", 568, { 4 }, F_FLOAT, Scr_SetExposureLerpToDarker },
  { NULL, 0, { 0 }, F_INT, NULL }
};

const SpawnFuncEntry s_bspOnlySpawns[15] =
{
  { "trigger_use", trigger_use_touch },
  { "trigger_use_touch", trigger_use_touch },
  { "trigger_multiple", SP_trigger_multiple },
  { "trigger_disk", SP_trigger_disk },
  { "trigger_hurt", SP_trigger_hurt },
  { "trigger_once", SP_trigger_once },
  { "trigger_damage", SP_trigger_damage },
  { "trigger_lookat", SP_trigger_lookat },
  { "trigger_ik_playerclip_terrain", SP_trigger_ik_playerclip_terrain },
  { "light", SP_light },
  { "misc_mg42", SP_turret },
  { "misc_turret", SP_turret },
  { "script_brushmodel", SP_script_brushmodel },
  { "script_struct", G_FreeEntityWrapper },
  { "script_vehicle", SP_script_vehicle }
};

// KISAK FIX (mp_area51 apple_damage_think): script Spawn("trigger_damage", ...) uses G_CallSpawnEntity, not BSP-only spawns.
// Retail allows trigger_damage here; was missing (only in s_bspOnlySpawns). Revert: remove trigger_damage line and change [9] back to [8].
const SpawnFuncEntry s_bspOrDynamicSpawns[9] =
{
  { "info_notnull", SP_info_notnull },
  { "info_notnull_big", SP_info_notnull },
  { "trigger_radius", SP_trigger_radius },
  { "trigger_radius_use", SP_trigger_radius_use },
  { "trigger_damage", SP_trigger_damage },
  { "script_model", SP_script_model },
  { "script_origin", SP_script_origin },
  { "script_vehicle_collmap", SP_script_vehicle_collmap },
  { "sound_blend", SP_sound_blend }
};




// Decomp: CoDSP_rdBlackOps.map.c (G_SpawnFloat)
int __cdecl G_SpawnFloat(const SpawnVar *spawnVar, const char *key, const char *defaultString, float *out)
{
    int present;
    const char *s;

    present = G_SpawnString(spawnVar, key, defaultString, &s);
    *out = atof(s);
    return present;
}

// Decomp: CoDSP_rdBlackOps.map.c (G_SpawnInt)
int __cdecl G_SpawnInt(const SpawnVar *spawnVar, const char *key, const char *defaultString, int *out)
{
    int present;
    const char *s;

    present = G_SpawnString(spawnVar, key, defaultString, &s);
    *out = atoi(s);
    return present;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_ReadOnlyField)
void __cdecl Scr_ReadOnlyField(gentity_s *ent, int)
{
    Scr_Error("Tried to set a read only entity field", 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (G_FreeEntityWrapper)
void __cdecl G_FreeEntityWrapper(gentity_s *ent, SpawnVar *v)
{
    G_FreeEntity(ent);
}

// Decomp: CoDSP_rdBlackOps.map.c (G_SetEntityScriptVariableInternal)
unsigned int __cdecl G_SetEntityScriptVariableInternal(const char *key, char *value)
{
    int tmp3;
    const char *fmtMsg;
    float tmp5;
    unsigned int index;
    int type;
    float vec[3];

    index = Scr_FindField(key, &type, SCRIPTINSTANCE_SERVER);
    if ( !index )
        return 0;
    switch ( type )
    {
        case 2:
            Scr_AddString(value, SCRIPTINSTANCE_SERVER);
            break;
        case 4:
            memset(vec, 0, sizeof(vec));
            sscanf(value, "%f %f %f", vec, &vec[1], &vec[2]);
            Scr_AddVector(vec, SCRIPTINSTANCE_SERVER);
            break;
        case 5:
            tmp5 = atof(value);
            Scr_AddFloat(tmp5, SCRIPTINSTANCE_SERVER);
            break;
        case 6:
            tmp3 = atoi(value);
            Scr_AddInt(tmp3, SCRIPTINSTANCE_SERVER);
            break;
        default:
            fmtMsg = va("G_SetEntityScriptVariableInternal: bad case %d", type);
            if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp", 210, 0, fmtMsg) )
                __debugbreak();
            break;
    }
    return index;
}

// Decomp: CoDSP_rdBlackOps.map.c (G_ParseEntityFields)
void __cdecl G_ParseEntityFields(const SpawnVar *spawnVar, gentity_s *ent, int radiant_update)
{
    int i;

    if ( !spawnVar->spawnVarsValid
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                    316,
                    0,
                    "%s",
                    "spawnVar.spawnVarsValid") )
    {
        __debugbreak();
    }
    for ( i = 0; i < spawnVar->numSpawnVars; ++i )
        G_ParseEntityField(spawnVar->spawnVars[i][0], spawnVar->spawnVars[i][1], ent, radiant_update);
    G_SetOrigin(ent, ent->r.currentOrigin);
    G_SetAngle(ent, ent->r.currentAngles);
}

// Decomp: CoDSP_rdBlackOps.map.c (G_ParseEntityField)
void __cdecl G_ParseEntityField(const char *key, char *value, gentity_s *ent, int radiant_update)
{
    float tmp4;
    const ent_field_t *f;
    float vec[3];
    int modelIndex;

    if ( !I_stricmp(key, "team") )
    {
        G_ParseTeamField(ent, value);
        return;
    }

    for ( f = fields_1; ; ++f )
    {
        if ( !f->name )
        {
            G_SetEntityScriptVariable(key, value, ent);
            return;
        }
        if ( !I_stricmp(f->name, key) )
            break;
    }
    switch ( f->type )
    {
        case F_INT:
            *(int *)((char *)&ent->s.number + f->ofs) = atoi(value);
            break;
        case F_FLOAT:
            tmp4 = atof(value);
            *(float *)((char *)&ent->s.number + f->ofs) = tmp4;
            break;
        case F_STRING:
            Scr_SetStringLiveUpdateSafe((unsigned __int16 *)((char *)ent + f->ofs), value, SCRIPTINSTANCE_SERVER);
            break;
        case F_VECTOR:
            memset(vec, 0, sizeof(vec));
            sscanf(value, "%f %f %f", vec, &vec[1], &vec[2]);
            *(float *)((char *)&ent->s.number + f->ofs) = vec[0];
            *(float *)((char *)&ent->s.lerp.eFlags + f->ofs) = vec[1];
            *(float *)((char *)&ent->s.lerp.eFlags2 + f->ofs) = vec[2];
            break;
        case F_MODEL:
            if ( *value == 42 )
            {
                modelIndex = atoi(value + 1);
                if ( modelIndex != (unsigned __int16)modelIndex
                    && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                                282,
                                0,
                                "%s",
                                "modelIndex == (modelNameIndex_t)modelIndex") )
                {
                    __debugbreak();
                }
                ent->s.index.brushmodel = modelIndex;
            }
            else
            {
                if ( Com_IsLegacyXModelName(value) )
                    value += 7;
                if ( radiant_update )
                    G_SetModelIfLoaded(ent, value);
                else
                    G_SetModel(ent, value);
            }
            break;
        default:
            return;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (G_SetEntityScriptVariable)
void __cdecl G_SetEntityScriptVariable(const char *key, char *value, gentity_s *ent)
{
    unsigned int index;

    index = G_SetEntityScriptVariableInternal(key, value);
    if ( index )
        GScr_SetDynamicEntityField(ent, index);
}

// Decomp: CoDSP_rdBlackOps.map.c (G_SpawnStruct)
unsigned int __cdecl G_SpawnStruct(SpawnVar *spawnVar)
{
    unsigned int index;
    int i;
    const char *scriptLabel;
    unsigned int structId;

    if ( !spawnVar->spawnVarsValid
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                    339,
                    0,
                    "%s",
                    "spawnVar.spawnVarsValid") )
    {
        __debugbreak();
    }
    G_SpawnString(spawnVar, "script_label", "", &scriptLabel);
    if ( !I_stricmp(scriptLabel, "random") || !I_stricmp(scriptLabel, "looper") || !I_stricmp(scriptLabel, "line_emitter") )
        return 0;
    if ( !g_scr_data.createstruct
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                    348,
                    0,
                    "%s",
                    "g_scr_data.createstruct") )
    {
        __debugbreak();
    }
    Scr_AddExecThread(SCRIPTINSTANCE_SERVER, g_scr_data.createstruct, 0);
    structId = Scr_GetObject(0, SCRIPTINSTANCE_SERVER);
    for ( i = 0; i < spawnVar->numSpawnVars; ++i )
    {
        index = G_SetEntityScriptVariableInternal(spawnVar->spawnVars[i][0], spawnVar->spawnVars[i][1]);
        if ( index )
            Scr_SetStructField(structId, index, SCRIPTINSTANCE_SERVER);
    }
    return structId;
}

// Decomp: CoDSP_rdBlackOps.map.c (G_DuplicateEntityFields)
void __cdecl G_DuplicateEntityFields(gentity_s *dest, const gentity_s *source)
{
    float *tmp2;
    float *tmp3;
    const ent_field_t *f;

    for ( f = fields_1; f->name; ++f )
    {
        switch ( f->type )
        {
            case F_INT:
                *(int *)((char *)&dest->s.number + f->ofs) = *(int *)((char *)&source->s.number + f->ofs);
                break;
            case F_FLOAT:
                *(int *)((char *)&dest->s.number + f->ofs) = *(int *)((char *)&source->s.number + f->ofs);
                break;
            case F_STRING:
                Scr_SetString(
                    (unsigned __int16 *)((char *)dest + f->ofs),
                    *(unsigned __int16 *)((char *)&source->s.number + f->ofs),
                    SCRIPTINSTANCE_SERVER);
                break;
            case F_VECTOR:
                tmp2 = (float *)((char *)&dest->s.number + f->ofs);
                tmp3 = (float *)((char *)&source->s.number + f->ofs);
                *tmp2 = *tmp3;
                tmp2[1] = tmp3[1];
                tmp2[2] = tmp3[2];
                break;
            case F_MODEL:
                *(_WORD *)((char *)&dest->s.number + f->ofs) = *(_WORD *)((char *)&source->s.number + f->ofs);
                break;
            default:
                continue;
        }
    }
    dest->team = source->team;
}

// Decomp: CoDSP_rdBlackOps.map.c (G_DuplicateScriptFields)
void __cdecl G_DuplicateScriptFields(gentity_s *dest, const gentity_s *source)
{
    if ( dest->s.number != dest - g_entities
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                    412,
                    0,
                    "%s",
                    "dest->s.number == dest - g_entities") )
    {
        __debugbreak();
    }
    if ( source->s.number != source - g_entities
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                    413,
                    0,
                    "%s",
                    "source->s.number == source - g_entities") )
    {
        __debugbreak();
    }
    Scr_CopyEntityNum(source->s.number, dest->s.number, 0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (IsClientOnlyEntity)
bool __cdecl IsClientOnlyEntity(const SpawnVar *spawnVar)
{
    bool result;
    char *out;
    const char *modelName;
    const char *clientSide;
    DestructibleDef *ddef;
    const char *destructibledefname;
    bool bClientSide;
    const char *classname;
    unsigned int flags;
    const char *spawnFlags;

    G_SpawnString(spawnVar, "classname", "", &classname);
    G_SpawnString(spawnVar, "spawnflags", "", &spawnFlags);
    flags = atoi(spawnFlags);
    bClientSide = (flags & 2) != 0;
    if ( strcmp("script_model", classname) )
        goto LABEL_15;
    G_SpawnString(spawnVar, "destructibledef", 0, &destructibledefname);
    if ( destructibledefname )
    {
        ddef = DestructibleDefPrecache(destructibledefname);
        if ( ddef && ddef->clientOnly )
            return 1;
        goto LABEL_15;
    }
    if ( (clientSide = 0, G_SpawnString(spawnVar, "clientside", 0, &clientSide), !bClientSide) && !clientSide
        || !bClientSide && atof(clientSide) != 1.0 )
    {
LABEL_15:
        result = !strcmp("script_origin", classname)
                    && ((out = 0, G_SpawnString(spawnVar, "clientside", 0, (const char **)&out), bClientSide) || out)
                    && (bClientSide || atof(out) == 1.0);
        return result;
    }
    G_SpawnString(spawnVar, "model", 0, &modelName);
    if ( modelName && *modelName )
        G_ModelIndex((char *)modelName);
    return 1;
}

// Decomp: CoDSP_rdBlackOps.map.c (G_SetupDestructible)
void __cdecl G_SetupDestructible(gentity_s *ent, char *destructibleName)
{
    char szConfigString[1024];
    int iDestID;

    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp", 533, 0, "%s", "ent") )
        __debugbreak();
    if ( !destructibleName
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp", 534, 0, "%s", "destructibleName") )
    {
        __debugbreak();
    }
    for ( iDestID = 0; iDestID < 64; ++iDestID )
    {
        SV_GetConfigstring(iDestID + 3180, szConfigString, 1024);
        if ( !szConfigString[0] )
        {
            SV_SetConfigstring(iDestID + 3180, destructibleName);
            break;
        }
        if ( !I_stricmp(szConfigString, destructibleName) )
            break;
    }
    if ( iDestID >= 64
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                    552,
                    0,
                    "%s",
                    "iDestID < MAX_DESTRUCTIBLE_DEFS") )
    {
        __debugbreak();
    }
    AssignToSmallerType<unsigned char>(&ent->s.un1.scale, iDestID);
    ent->s.lerp.eFlags |= 0x20000u;
}

// Decomp: CoDSP_rdBlackOps.map.c (G_SetupDestructible)
void __cdecl G_SetupDestructible(gentity_s *ent, SpawnVar *spawnVar)
{
    const char *destructibleName;

    destructibleName = 0;
    G_SpawnString(spawnVar, "destructibledef", 0, &destructibleName);
    if ( destructibleName )
        G_SetupDestructible(ent, (char *)destructibleName);
}

// Decomp: CoDSP_rdBlackOps.map.c (G_CallSpawn)
void __cdecl G_CallSpawn(SpawnVar *spawnVar)
{
    const gitem_s *item;
    void (__cdecl *spawnFunc)(gentity_s *, SpawnVar *);
    const char *classname;
    gentity_s *ent;
    int radiantLiveUpdate;

    if ( !spawnVar->spawnVarsValid
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                    584,
                    1,
                    "%s",
                    "spawnVar->spawnVarsValid") )
    {
        __debugbreak();
    }
    radiantLiveUpdate = !level.spawnVar.spawnVarsValid;
    G_SpawnString(spawnVar, "classname", "", &classname);
    if ( classname )
    {
        if ( strncmp(classname, "dyn_", 4u) )
        {
            if ( !strncmp(classname, "node_", 5u) )
            {
                if ( !useFastFile->current.enabled )
                    G_SpawnPathnodeStatic(spawnVar, classname);
                G_SpawnPathnodeDynamic(spawnVar);
            }
            else if ( !strncmp(classname, "actor_", 6u) )
            {
                ent = G_Spawn();
                G_ParseEntityFields(spawnVar, ent, radiantLiveUpdate);
                SP_actor(ent, spawnVar);
            }
            else if ( !strcmp("info_vehicle_node", classname) )
            {
                SP_info_vehicle_node(spawnVar, 0, 0);
            }
            else if ( !strcmp("info_vehicle_node_rotate", classname) )
            {
                SP_info_vehicle_node(spawnVar, 1, 0);
            }
            else
            {
                item = G_GetItemForClassname(classname);
                if ( item )
                {
                    ent = G_Spawn();
                    G_ParseEntityFields(spawnVar, ent, radiantLiveUpdate);
                    G_SpawnItem(ent, item);
                }
                else if ( !IsClientOnlyEntity(spawnVar)
                             && strcmp("occlusion_override", classname)
                             && strcmp("undertow_volume", classname)
                             && strcmp("rope", classname) )
                {
                    if ( !strcmp("heli_height_lock", classname) )
                    {
                        G_SpawnHeliHeightLock(spawnVar);
                    }
                    else if ( strcmp("glass", classname) )
                    {
                        spawnFunc = G_FindSpawnFunc(classname, s_bspOrDynamicSpawns, 9);
                        if ( !spawnFunc && level.spawnVar.spawnVarsValid )
                            spawnFunc = G_FindSpawnFunc(classname, s_bspOnlySpawns, 15);
                        if ( (char *)spawnFunc != (char *)G_FreeEntityWrapper )
                        {
                            ent = G_Spawn();
                            G_ParseEntityFields(spawnVar, ent, radiantLiveUpdate);
                            G_SetupDestructible(ent, spawnVar);
                            if ( spawnFunc )
                                spawnFunc(ent, spawnVar);
                        }
                    }
                }
            }
        }
    }
    else
    {
        Com_Printf(15, "G_CallSpawn: NULL classname\n");
    }
}

const gitem_s *__cdecl G_GetItemForClassname(const char *classname)
{
    unsigned int weapIndex;

    if ( g_connectpaths->current.integer )
        return 0;
    if ( strncmp(classname, "weapon_", 7u) )
        return 0;
    weapIndex = G_GetWeaponIndexForName((char *)classname + 7);
    if ( !weapIndex )
        return 0;
    BG_GetWeaponDef(weapIndex);
    return BG_FindItemForWeapon(weapIndex);
}

void (__cdecl *__cdecl G_FindSpawnFunc(
                const char *classname,
                const SpawnFuncEntry *spawnFuncArray,
                int spawnFuncCount))(gentity_s *, SpawnVar *)
{
    int spawnFuncIter;

    for ( spawnFuncIter = 0; spawnFuncIter < spawnFuncCount; ++spawnFuncIter )
    {
        if ( !strcmp(classname, spawnFuncArray[spawnFuncIter].classname) )
            return spawnFuncArray[spawnFuncIter].callback;
    }
    return 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (G_CallSpawnEntity)
// KISAK FIX: use SpawnVar* spawn callback (retail) and pass spawnVar=0 so SP_* reads extra spawn() args from script VM.
// Revert: restore void (*spawnFunc)(gentity_s*) cast and ((void (*)(gentity_s*, unsigned int))spawnFunc)(ent, 0).
int __cdecl G_CallSpawnEntity(gentity_s *ent)
{
    const gitem_s *item;
    void (__cdecl *spawnFunc)(gentity_s *, SpawnVar *);
    char *classname;

    if ( level.spawnVar.spawnVarsValid
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                    705,
                    1,
                    "%s",
                    "!level.spawnVar.spawnVarsValid") )
    {
        __debugbreak();
    }
    if ( ent->classname )
    {
        classname = SL_ConvertToString(ent->classname, SCRIPTINSTANCE_SERVER);
        item = G_GetItemForClassname(classname);
        if ( item )
        {
            G_SpawnItem(ent, item);
            return 1;
        }
        else
        {
            spawnFunc = G_FindSpawnFunc(classname, s_bspOrDynamicSpawns, 9);
            if ( spawnFunc )
            {
                if ( spawnFunc == G_FreeEntityWrapper
                    && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                                729,
                                0,
                                "%s",
                                "spawnFunc != G_FreeEntityWrapper") )
                {
                    __debugbreak();
                }
                spawnFunc(ent, 0); // spawnVar null => SP_* uses Scr_GetFloat from script spawn() args
                if ( !ent->r.inuse
                    && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                                731,
                                0,
                                "%s",
                                "ent->r.inuse") )
                {
                    __debugbreak();
                }
                return 1;
            }
            else
            {
                Com_Printf(15, "%s cannot be spawned dynamically\n", classname);
                return 0;
            }
        }
    }
    else
    {
        Com_Printf(15, "G_CallSpawnEntity: NULL classname\n");
        return 0;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_AddFieldsForEntity)
void __cdecl GScr_AddFieldsForEntity()
{
    const ent_field_t *f;

    for ( f = fields_1; f->name; ++f )
    {
        if ( ((f - fields_1) & 0xC000) != 0
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                        747,
                        0,
                        "%s",
                        "((f - fields) & ENTFIELD_MASK) == ENTFIELD_ENTITY") )
        {
            __debugbreak();
        }
        if ( f - fields_1 != (unsigned __int16)(f - fields_1)
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                        748,
                        0,
                        "%s",
                        "(f - fields) == (unsigned short)( f - fields )") )
        {
            __debugbreak();
        }
        Scr_AddClassField(0, (char *)f->name, (unsigned __int16)(f - fields_1), SCRIPTINSTANCE_SERVER);
    }
    GScr_AddFieldsForActor();
    GScr_AddFieldsForSentient();
    GScr_AddFieldsForClient();
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_AddFieldsForRadiant)
void __cdecl GScr_AddFieldsForRadiant()
{
    Scr_AddFields("radiant", (char*)"txt", SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetGenericField)
void __cdecl GScr_SetGenericField(unsigned __int8 *b, fieldtype_t type, int ofs, unsigned int whichbits)
{
    gentity_s *ent;
    gentity_s *enta;
    gentity_s *entb;

    switch ( type )
    {
        case F_ACTOR:
            ent = Scr_GetEntityAllowNull(0, SCRIPTINSTANCE_SERVER);
            if ( ent )
                *(unsigned int *)&b[ofs] = (unsigned int)ent->actor;
            else
                *(unsigned int *)&b[ofs] = 0;
            break;
        case F_SENTIENT:
            enta = Scr_GetEntityAllowNull(0, SCRIPTINSTANCE_SERVER);
            if ( enta )
                *(unsigned int *)&b[ofs] = (unsigned int)enta->sentient;
            else
                *(unsigned int *)&b[ofs] = 0;
            break;
        case F_SENTIENTHANDLE:
            entb = Scr_GetEntityAllowNull(0, SCRIPTINSTANCE_SERVER);
            if ( entb )
                ((SentientHandle *)&b[ofs])->setSentient(entb->sentient);
            else
                ((SentientHandle *)&b[ofs])->setSentient(0);
            break;
        case F_PATHNODE:
            *(unsigned int *)&b[ofs] = (unsigned int)Scr_GetPathnode(0, SCRIPTINSTANCE_SERVER);
            break;
        default:
            Scr_SetGenericField(b, type, ofs, SCRIPTINSTANCE_SERVER, whichbits);
            break;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetObjectField)
int __cdecl Scr_SetObjectField(unsigned int classnum, unsigned int entnum, unsigned int offset)
{
    const char *fmtMsg;
    const char *fmtMsg2;

    switch ( classnum )
    {
        case 0u:
            return Scr_SetEntityField(entnum, offset);
        case 1u:
            Scr_SetHudElemField(entnum, offset);
            return 1;
        case 2u:
            Scr_SetPathnodeField(entnum, offset);
            return 1;
        case 3u:
            GScr_SetVehicleNodeField(entnum, offset);
            return 1;
        default:
            fmtMsg2 = va("bad class num %u", classnum);
            if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp", 894, 0, fmtMsg2) )
                __debugbreak();
            return 1;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_SetEntityField)
int __cdecl Scr_SetEntityField(unsigned int entnum, unsigned int offset)
{
    const ent_field_t *f;
    gentity_s *ent;

    if ( entnum >= 0x400
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                    781,
                    0,
                    "%s",
                    "(unsigned)entnum < MAX_GENTITIES") )
    {
        __debugbreak();
    }
    ent = &g_entities[entnum];
    if ( !ent->r.inuse
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                    784,
                    0,
                    "%s\n\t(ent->s.number) = %i",
                    "(ent->r.inuse)",
                    ent->s.number) )
    {
        __debugbreak();
    }
    switch ( offset & 0xC000 )
    {
        case 0x8000u:
            if ( ent->actor )
            {
                Scr_SetActorField(ent->actor, offset & 0xFFFF3FFF);
                return 1;
            }
            else
            {
                return 0;
            }
        case 0x4000u:
            if ( ent->sentient )
            {
                Scr_SetSentientField(ent->sentient, offset & 0xFFFF3FFF);
                return 1;
            }
            else
            {
                return 0;
            }
        case 0xC000u:
            if ( ent->client )
            {
                Scr_SetClientField(ent->client, offset & 0xFFFF3FFF);
                return 1;
            }
            else
            {
                return 0;
            }
        default:
            if ( offset >= ENTFIELD_MAX
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                            815,
                            0,
                            "%s",
                            "(unsigned)offset < ARRAY_COUNT( fields ) - 1") )
            {
                __debugbreak();
            }
            f = &fields_1[offset];
            if ( f->callback )
                f->callback(ent, offset);
            else
                GScr_SetGenericField((unsigned __int8 *)ent, f->type, f->ofs, 0);
            return 1;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetEntityField)
void __cdecl Scr_GetEntityField(unsigned int entnum, unsigned int offset)
{
    gentity_s *ent;

    if ( entnum >= 0x400
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                    911,
                    0,
                    "%s",
                    "(unsigned)entnum < MAX_GENTITIES") )
    {
        __debugbreak();
    }
    ent = &g_entities[entnum];
    if ( !ent->r.inuse
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                    914,
                    0,
                    "%s\n\t(ent->s.number) = %i",
                    "(ent->r.inuse)",
                    ent->s.number) )
    {
        __debugbreak();
    }
    switch ( offset & 0xC000 )
    {
        case 0x8000u:
            if ( ent->actor )
                Scr_GetActorField(ent->actor, offset & 0xFFFF3FFF);
            break;
        case 0x4000u:
            if ( ent->sentient )
                Scr_GetSentientField(ent->sentient, offset & 0xFFFF3FFF);
            break;
        case 0xC000u:
            if ( ent->client )
                Scr_GetClientField(ent->client, offset & 0xFFFF3FFF);
            break;
        default:
            if ( offset >= ENTFIELD_MAX
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                            942,
                            0,
                            "%s",
                            "(unsigned)offset < ARRAY_COUNT( fields ) - 1") )
            {
                __debugbreak();
            }
            GScr_GetGenericField((unsigned __int8 *)ent, fields_1[offset].type, fields_1[offset].ofs, 0);
            break;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_GetGenericField)
void __cdecl GScr_GetGenericField(unsigned __int8 *b, fieldtype_t type, int ofs, unsigned int whichbits)
{
    gentity_s *tmp4;
    sentient_s *tmp5;
    unsigned int modelNameStr;
    unsigned int id;

    switch ( type )
    {
        case F_ENTITY:
            if ( *(unsigned int *)&b[ofs] )
                Scr_AddEntity(*(gentity_s **)&b[ofs], SCRIPTINSTANCE_SERVER);
            break;
        case F_ENTHANDLE:
            if ( ((EntHandle *)&b[ofs])->isDefined() )
            {
                tmp4 = ((EntHandle *)&b[ofs])->ent();
                Scr_AddEntity(tmp4, SCRIPTINSTANCE_SERVER);
            }
            break;
        case F_ACTOR:
            if ( *(unsigned int *)&b[ofs] )
                Scr_AddEntity(**(gentity_s ***)&b[ofs], SCRIPTINSTANCE_SERVER);
            break;
        case F_SENTIENT:
            if ( *(unsigned int *)&b[ofs] )
                Scr_AddEntity(**(gentity_s ***)&b[ofs], SCRIPTINSTANCE_SERVER);
            break;
        case F_SENTIENTHANDLE:
            if ( ((SentientHandle *)&b[ofs])->isDefined() )
            {
                tmp5 = ((SentientHandle *)&b[ofs])->sentient();
                Scr_AddEntity(tmp5->ent, SCRIPTINSTANCE_SERVER);
            }
            break;
        case F_PATHNODE:
            if ( *(unsigned int *)&b[ofs] )
                Scr_AddPathnode(*(pathnode_t **)&b[ofs]);
            break;
        case F_OBJECT:
            id = *(unsigned int *)&b[ofs];
            if ( id )
                Scr_AddObject(id, SCRIPTINSTANCE_SERVER);
            break;
        case F_MODEL:
            modelNameStr = G_ModelName(b[ofs]);
            Scr_AddConstString(modelNameStr, SCRIPTINSTANCE_SERVER);
            break;
        default:
            Scr_GetGenericField(b, type, ofs, SCRIPTINSTANCE_SERVER, whichbits);
            break;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetObjectField)
void __cdecl Scr_GetObjectField(unsigned int classnum, unsigned int entnum, unsigned int offset)
{
    const char *fmtMsg;

    switch ( classnum )
    {
        case 0u:
            Scr_GetEntityField(entnum, offset);
            break;
        case 1u:
            Scr_GetHudElemField(entnum, offset);
            break;
        case 2u:
            Scr_GetPathnodeField(entnum, offset);
            break;
        case 3u:
            GScr_GetVehicleNodeField(entnum, offset);
            break;
        default:
            fmtMsg = va("bad class num %u", classnum);
            if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp", 1023, 0, fmtMsg) )
                __debugbreak();
            break;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_FreeEntityConstStrings)
void __cdecl Scr_FreeEntityConstStrings(gentity_s *pEnt)
{
    const ent_field_t *f;
    int i;

    for ( f = fields_1; f->name; ++f )
    {
        if ( f->type == F_STRING )
            Scr_SetString((unsigned __int16 *)((char *)pEnt + f->ofs), 0, SCRIPTINSTANCE_SERVER);
    }
    for ( i = 0; i < 19; ++i )
    {
        pEnt->attachModelNames[i] = 0;
        Scr_SetString(&pEnt->attachTagNames[i], 0, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_FreeEntity)
void __cdecl Scr_FreeEntity(gentity_s *ent)
{
    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp", 1065, 0, "%s", "ent") )
        __debugbreak();
    if ( ent->s.number != ent - g_entities
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                    1066,
                    0,
                    "ent->s.number == ent - g_entities\n\t%i, %i",
                    ent->s.number,
                    ent - g_entities) )
    {
        __debugbreak();
    }
    if ( !ent->r.inuse
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                    1067,
                    0,
                    "%s\n\t(ent->s.number) = %i",
                    "(ent->r.inuse)",
                    ent->s.number) )
    {
        __debugbreak();
    }
    Scr_FreeEntityConstStrings(ent);
    Scr_FreeEntityNum(ent->s.number, 0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_AddEntity)
void __cdecl Scr_AddEntity(gentity_s *ent, scriptInstance_t inst)
{
    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp", 1076, 0, "%s", "ent") )
        __debugbreak();
    if ( ent->s.number != ent - g_entities
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                    1077,
                    0,
                    "ent->s.number == ent - g_entities\n\t%i, %i",
                    ent->s.number,
                    ent - g_entities) )
    {
        __debugbreak();
    }
    if ( !ent->r.inuse
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                    1078,
                    0,
                    "%s\n\t(ent->s.number) = %i",
                    "(ent->r.inuse)",
                    ent->s.number) )
    {
        __debugbreak();
    }
    Scr_AddEntityNum(ent->s.number, 0, inst, 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetEntityAllowNull)
gentity_s *__cdecl Scr_GetEntityAllowNull(unsigned int index, scriptInstance_t inst)
{
    scr_entref_t tmp3;
    scr_entref_t tmp4;
    scr_entref_t entref;

    if ( !Scr_GetType(index, inst) )
        return 0;
    //tmp4 = *Scr_GetEntityRef(&tmp3, index, inst);
    tmp4 = Scr_GetEntityRef(index, inst);
    entref = tmp4;
    if ( tmp4.classnum )
        return 0;
    if ( entref.entnum >= 0x400u
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                    1101,
                    0,
                    "%s",
                    "entref.entnum < MAX_GENTITIES") )
    {
        __debugbreak();
    }
    return &g_entities[entref.entnum];
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetEntity)
gentity_s *__cdecl Scr_GetEntity(unsigned int index)
{
    scr_entref_t tmp2;
    scr_entref_t tmp3;
    scr_entref_t entref;

    //tmp3 = *Scr_GetEntityRef(&tmp2, index, SCRIPTINSTANCE_SERVER);
    tmp3 = Scr_GetEntityRef(index, SCRIPTINSTANCE_SERVER);
    entref = tmp3;
    if ( tmp3.classnum )
    {
        Scr_ParamError(index, "not an entity", SCRIPTINSTANCE_SERVER);
        return 0;
    }
    else
    {
        if ( entref.entnum >= 0x400u
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                        1120,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        return &g_entities[entref.entnum];
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_FreeHudElem)
void __cdecl Scr_FreeHudElem(game_hudelem_s *hud)
{
    if ( !hud && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp", 1137, 0, "%s", "hud") )
        __debugbreak();
    if ( (unsigned int)(hud - g_hudelems) >= 0x400
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                    1138,
                    0,
                    "hud - g_hudelems doesn't index MAX_HUDELEMS_TOTAL\n\t%i not in [0, %i)",
                    hud - g_hudelems,
                    1024) )
    {
        __debugbreak();
    }
    if ( !hud->elem.type
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                    1139,
                    0,
                    "%s",
                    "hud->elem.type != HE_TYPE_FREE") )
    {
        __debugbreak();
    }
    Scr_NotifyNum(hud - g_hudelems, 1u, scr_const.death, 0);
    Scr_FreeHudElemConstStrings(hud);
    Scr_FreeEntityNum(hud - g_hudelems, 1u, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_AddHudElem)
void __cdecl Scr_AddHudElem(game_hudelem_s *hud)
{
    if ( !hud && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp", 1158, 0, "%s", "hud") )
        __debugbreak();
    if ( (unsigned int)(hud - g_hudelems) >= 0x400
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                    1159,
                    0,
                    "hud - g_hudelems doesn't index MAX_HUDELEMS_TOTAL\n\t%i not in [0, %i)",
                    hud - g_hudelems,
                    1024) )
    {
        __debugbreak();
    }
    if ( !hud->elem.type
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                    1160,
                    0,
                    "%s",
                    "hud->elem.type != HE_TYPE_FREE") )
    {
        __debugbreak();
    }
    Scr_AddEntityNum(hud - g_hudelems, 1u, SCRIPTINSTANCE_SERVER, 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_ExecEntThread)
unsigned __int16 __cdecl Scr_ExecEntThread(gentity_s *ent, int handle, unsigned int paramcount)
{
    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp", 1193, 0, "%s", "ent") )
        __debugbreak();
    if ( ent->s.number != ent - g_entities
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                    1194,
                    0,
                    "ent->s.number == ent - g_entities\n\t%i, %i",
                    ent->s.number,
                    ent - g_entities) )
    {
        __debugbreak();
    }
    if ( !ent->r.inuse
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                    1195,
                    0,
                    "%s\n\t(ent->s.number) = %i",
                    "(ent->r.inuse)",
                    ent->s.number) )
    {
        __debugbreak();
    }
    if ( !handle )
        return 0;
    return Scr_ExecEntThreadNum(SCRIPTINSTANCE_SERVER, ent->s.number, 0, handle, paramcount, 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_Notify)
void __cdecl Scr_Notify(gentity_s *ent, unsigned __int16 stringValue, unsigned int paramcount)
{
    char *slStr;
    const char *fmtMsg;

    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp", 1223, 0, "%s", "ent") )
        __debugbreak();
    if ( ent->s.number != ent - g_entities )
    {
        slStr = SL_ConvertToString(stringValue, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("s.number = %i, index = %i, stringValue = %s", ent->s.number, ent - g_entities, slStr);
        if ( !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                        1224,
                        0,
                        "%s\n\t%s",
                        "ent->s.number == ent - g_entities",
                        fmtMsg) )
            __debugbreak();
    }
    if ( !ent->r.inuse
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                    1225,
                    0,
                    "%s\n\t(ent->s.number) = %i",
                    "(ent->r.inuse)",
                    ent->s.number) )
    {
        __debugbreak();
    }
    Scr_NotifyNum(ent->s.number, 0, stringValue, paramcount);
}

void __cdecl GScr_NotifyLevel(unsigned int stringValue, unsigned int paramcount)
{
    scriptInstance_t inst;
    VariableValue *startTop;
    unsigned int paramcounta;
    int type;

    inst = SCRIPTINSTANCE_SERVER;
    if ( !gScrVarPub[inst].levelId )
        return;
    startTop = &gScrVmPub[inst].top[-(int)paramcount];
    paramcounta = gScrVmPub[inst].inparamcount - paramcount;
    type = startTop->type;
    startTop->type = 8;
    gScrVmPub[inst].inparamcount = 0;
    VM_Notify(inst, gScrVarPub[inst].levelId, stringValue, gScrVmPub[inst].top);
    startTop->type = type;
    while ( gScrVmPub[inst].top != startTop )
    {
        RemoveRefToValue(inst, gScrVmPub[inst].top->type, gScrVmPub[inst].top->u);
        --gScrVmPub[inst].top;
    }
    gScrVmPub[inst].inparamcount = paramcounta;
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetEnt)
void __cdecl Scr_GetEnt()
{
    gentity_s *result;
    unsigned __int16 name;
    int offset;
    char *key;
    gentity_s *ent;
    int i;
    unsigned __int16 value;

    name = (unsigned __int16)Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    key = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
    offset = Scr_GetOffset(0, key, SCRIPTINSTANCE_SERVER);
    if ( offset >= 0 )
    {
        if ( offset >= 15
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                        1255,
                        0,
                        "%s",
                        "offset >= 0 && offset < static_cast<int>( ARRAY_COUNT( fields ) - 1 )") )
        {
            __debugbreak();
        }
        if ( fields_1[offset].type == F_STRING )
        {
            result = 0;
            i = 0;
            ent = g_entities;
            while ( i < level.num_entities )
            {
                if ( ent->r.inuse )
                {
                    value = *(_WORD *)((char *)&ent->s.number + fields_1[offset].ofs);
                    if ( value )
                    {
                        if ( value == name )
                        {
                            if ( result )
                                Scr_Error("getent used with more than one entity", 0);
                            result = ent;
                        }
                    }
                }
                ++i;
                ++ent;
            }
            if ( result )
                Scr_AddEntity(result, SCRIPTINSTANCE_SERVER);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetEntArray)
void __cdecl Scr_GetEntArray()
{
    unsigned __int16 name;
    int offset;
    char *key;
    gentity_s *ent;
    gentity_s *enta;
    int i;
    int ia;
    unsigned __int16 value;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) )
    {
        name = (unsigned __int16)Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
        key = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
        offset = Scr_GetOffset(0, key, SCRIPTINSTANCE_SERVER);
        if ( offset >= 0 )
        {
            if ( offset >= 15
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                            1321,
                            0,
                            "%s",
                            "offset >= 0 && offset < static_cast<int>( ARRAY_COUNT( fields ) - 1 )") )
            {
                __debugbreak();
            }
            if ( fields_1[offset].type == F_STRING )
            {
                Scr_MakeArray(SCRIPTINSTANCE_SERVER);
                ia = 0;
                enta = g_entities;
                while ( ia < level.num_entities )
                {
                    if ( enta->r.inuse )
                    {
                        value = *(_WORD *)((char *)&enta->s.number + fields_1[offset].ofs);
                        if ( value )
                        {
                            if ( value == name )
                            {
                                Scr_AddEntity(enta, SCRIPTINSTANCE_SERVER);
                                Scr_AddArray(SCRIPTINSTANCE_SERVER);
                            }
                        }
                    }
                    ++ia;
                    ++enta;
                }
            }
        }
    }
    else
    {
        Scr_MakeArray(SCRIPTINSTANCE_SERVER);
        i = 0;
        ent = g_entities;
        while ( i < level.num_entities )
        {
            if ( ent->r.inuse )
            {
                Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
                Scr_AddArray(SCRIPTINSTANCE_SERVER);
            }
            ++i;
            ++ent;
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetDynModels)
void __cdecl Scr_GetDynModels()
{
    unsigned int i;
    XModel *model;

    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for ( i = 0; i < cm.dynEntCount[0]; ++i )
    {
        model = cm.dynEntDefList[0][i].xModel;
        if ( model )
        {
            Scr_AddString(model->name, SCRIPTINSTANCE_SERVER);
            Scr_AddArray(SCRIPTINSTANCE_SERVER);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetMiscModels)
void __cdecl Scr_GetMiscModels()
{
    unsigned int i;
    XModel *model;

    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for ( i = 0; i < cm.numStaticModels; ++i )
    {
        model = cm.staticModelList[i].xmodel;
        if ( model )
        {
            Scr_AddString(model->name, SCRIPTINSTANCE_SERVER);
            Scr_AddArray(SCRIPTINSTANCE_SERVER);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_GetDestructibleDefs)
void __cdecl Scr_GetDestructibleDefs()
{
    int i;
    gentity_s *ent;

    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    for ( i = 0; i < level.num_entities; ++i )
    {
        ent = &g_entities[i];
        if ( ent->r.inuse && ent->destructible && ent->destructible->ddef && ent->destructible->ddef->name )
        {
            Scr_AddString(ent->destructible->ddef->name, SCRIPTINSTANCE_SERVER);
            Scr_AddArray(SCRIPTINSTANCE_SERVER);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Scr_EntSearch)
void __cdecl Scr_EntSearch()
{
    unsigned int numParam;
    int mask;
    float origin[3];
    float radius;
    float dir[3];
    float cosThreshold;
    float mins[3];
    float maxs[3];
    int entityList[1024];
    int areaCount;
    int i;
    gentity_s *ent;
    float delta[3];
    float dist;

    numParam = Scr_GetNumParam(SCRIPTINSTANCE_SERVER);
    if ( !numParam )
    {
        Scr_MakeArray(SCRIPTINSTANCE_SERVER);
        for ( i = 0; i < level.num_entities; ++i )
        {
            ent = &g_entities[i];
            if ( ent->r.inuse )
            {
                Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
                Scr_AddArray(SCRIPTINSTANCE_SERVER);
            }
        }
        return;
    }
    mask = Scr_GetInt(0, SCRIPTINSTANCE_SERVER);
    if ( numParam <= 1 )
    {
        Scr_MakeArray(SCRIPTINSTANCE_SERVER);
        for ( i = 0; i < level.num_entities; ++i )
        {
            ent = &g_entities[i];
            if ( ent->r.inuse && (ent->r.contents & mask) )
            {
                Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
                Scr_AddArray(SCRIPTINSTANCE_SERVER);
            }
        }
        return;
    }
    if ( numParam < 3 )
    {
        Scr_ParamError(1u, "Cannot specify an origin without a radius", SCRIPTINSTANCE_SERVER);
        return;
    }
    Scr_GetVector(1u, origin, SCRIPTINSTANCE_SERVER);
    radius = Scr_GetFloat(2u, SCRIPTINSTANCE_SERVER);
    if ( radius <= 0.0f )
    {
        Scr_ParamError(2u, "Range should be greater than 0", SCRIPTINSTANCE_SERVER);
        return;
    }
    dir[0] = 1.0f;
    dir[1] = 0.0f;
    dir[2] = 0.0f;
    cosThreshold = -1.0f;
    if ( numParam > 3 )
    {
        float fov;
        float halfFov;

        if ( numParam < 5 )
        {
            Scr_ParamError(3u, "Cannot specify an direction without a FOV", SCRIPTINSTANCE_SERVER);
            return;
        }
        Scr_GetVector(3u, dir, SCRIPTINSTANCE_SERVER);
        fov = Scr_GetFloat(4u, SCRIPTINSTANCE_SERVER);
        if ( fov <= 0.0f )
        {
            Scr_ParamError(4u, "FOV should be greater than 0", SCRIPTINSTANCE_SERVER);
            return;
        }
        if ( fov > 360.0f )
            fov = 360.0f;
        halfFov = fov * 0.5f;
        if ( halfFov >= 179.9f )
            cosThreshold = -1.0f;
        else
            cosThreshold = cosf(halfFov * 0.017453292f);
        if ( Vec3Normalize(dir) == 0.0f )
        {
            Scr_ParamError(3u, "Direction length must be greater than 0", SCRIPTINSTANCE_SERVER);
            return;
        }
    }
    Scr_MakeArray(SCRIPTINSTANCE_SERVER);
    mins[0] = origin[0] - radius;
    mins[1] = origin[1] - radius;
    mins[2] = origin[2] - radius;
    maxs[0] = origin[0] + radius;
    maxs[1] = origin[1] + radius;
    maxs[2] = origin[2] + radius;
    areaCount = CM_AreaEntities(mins, maxs, entityList, 1024, mask);
    for ( i = 0; i < areaCount; ++i )
    {
        ent = &g_entities[entityList[i]];
        delta[0] = ent->r.currentOrigin[0] - origin[0];
        delta[1] = ent->r.currentOrigin[1] - origin[1];
        delta[2] = ent->r.currentOrigin[2] - origin[2];
        dist = Vec3Normalize(delta);
        if ( dist < radius && (dir[0] * delta[0] + dir[1] * delta[1] + dir[2] * delta[2]) >= cosThreshold )
        {
            Scr_AddEntity(ent, SCRIPTINSTANCE_SERVER);
            Scr_AddArray(SCRIPTINSTANCE_SERVER);
        }
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (GScr_SetDynamicEntityField)
void __cdecl GScr_SetDynamicEntityField(gentity_s *ent, unsigned int index)
{
    Scr_SetDynamicEntityField(ent->s.number, 0, index, SCRIPTINSTANCE_SERVER, 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (SP_worldspawn)
void __cdecl SP_worldspawn(SpawnVar *spawnVar)
{
    char *fmtMsg;
    float tmp2;
    float value;
    float tmp4;
    const char *s;

    G_SpawnString(spawnVar, "classname", "", &s);
    if ( I_stricmp(s, "worldspawn") )
        Com_Error(ERR_DROP, "SP_worldspawn: The first entity isn't 'worldspawn'");
    SV_SetConfigstring(2, (char*)"cod");
    G_SpawnString(spawnVar, "ambienttrack", "", &s);
    if ( *s )
    {
        fmtMsg = va("n\\%s", s);
        SV_SetConfigstring(1547, fmtMsg);
    }
    else
    {
        SV_SetConfigstring(1547, (char *)"");
    }
    G_SpawnString(spawnVar, "message", "", &s);
    SV_SetConfigstring(4, (char *)s);
    SV_SetConfigstring(11, (char *)g_motd->current.integer);
    G_SpawnString(spawnVar, "gravity", "800", &s);
    if ( !bg_gravity
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp", 1381, 0, "%s", "bg_gravity") )
    {
        __debugbreak();
    }
    value = atof(s);
    Dvar_SetFloat((dvar_s *)bg_gravity, value);
    G_SpawnString(spawnVar, "northyaw", "", &s);
    if ( *s )
    {
        SV_SetConfigstring(1548, (char *)s);
        tmp2 = atof(s);
        tmp4 = tmp2 * 0.017453292;
        level.compassNorth[0] = cos(tmp4);
        level.compassNorth[1] = sin(tmp4);
    }
    else
    {
        SV_SetConfigstring(1548, (char*)"0");
        level.compassNorth[0] = 1.0f;
        level.compassNorth[1] = 0.0f;
    }
    G_SpawnString(spawnVar, "ambient", "0", &s);
    const float ambient = (float)atof(s);
    G_SpawnString(spawnVar, "diffuseFraction", "0", &s);
    const float diffuseFraction = (float)atof(s);
    G_SpawnString(spawnVar, "sunLight", "1", &s);
    const float sunLight = (float)atof(s);
    G_SpawnString(spawnVar, "sunColor", "1 1 1", &s);
    float sunColor[3];
    sscanf(s, "%g %g %g", &sunColor[0], &sunColor[1], &sunColor[2]);
    ColorNormalize(sunColor, sunColor);
    float sunScale = (sunLight - ambient) * (1.0f - diffuseFraction);
    if ( sunScale < 0.0f )
        sunScale = 0.0f;
    g_mapSunLight[0] = sunScale * sunColor[0];
    g_mapSunLight[1] = sunScale * sunColor[1];
    g_mapSunLight[2] = sunScale * sunColor[2];
    G_SpawnString(spawnVar, "sunDirection", "0 0 0", &s);
    float sunAngles[3];
    sscanf(s, "%g %g %g", &sunAngles[0], &sunAngles[1], &sunAngles[2]);
    AngleVectors(sunAngles, g_mapSunDirection, 0, 0);

    G_SpawnString(spawnVar, "spawnflags", "0", &s);
    g_entities[1022].spawnflags = atoi(s);
    g_entities[1022].s.number = 1022;
    Scr_SetString(&g_entities[1022].classname, scr_const.worldspawn, SCRIPTINSTANCE_SERVER);
    g_entities[1022].r.inuse = 1;
    if ( g_entities[1022].r.ownerNum.isDefined() )
    {
        if ( !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                        1403,
                        0,
                        "%s",
                        "!g_entities[ENTITYNUM_WORLD].r.ownerNum.isDefined()") )
            __debugbreak();
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (G_SpawnEntitiesFromString)
void __cdecl G_SpawnEntitiesFromString()
{
    if ( !G_ParseSpawnVars(&level.spawnVar) )
        Com_Error(ERR_DROP, "SpawnEntities: no entities");
    SP_worldspawn(&level.spawnVar);
    while ( G_ParseSpawnVars(&level.spawnVar) )
        G_CallSpawn(&level.spawnVar);
    G_ResetEntityParsePoint();
}

// Decomp: CoDSP_rdBlackOps.map.c (G_LoadStructs)
void __cdecl G_LoadStructs()
{
    unsigned __int16 hThread;
    const char *classname;

    if ( !g_scr_data.initstructs
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\\g_spawn_sp.cpp",
                    1443,
                    0,
                    "%s",
                    "g_scr_data.initstructs") )
    {
        __debugbreak();
    }
    hThread = Scr_ExecThread(SCRIPTINSTANCE_SERVER, g_scr_data.initstructs, 0);
    Scr_FreeThread(hThread, SCRIPTINSTANCE_SERVER);
    while ( G_ParseSpawnVars(&level.spawnVar) )
    {
        G_SpawnString(&level.spawnVar, "classname", "", &classname);
        if ( !strcmp("script_struct", classname) )
            G_SpawnStruct(&level.spawnVar);
    }
    G_ResetEntityParsePoint();
}

