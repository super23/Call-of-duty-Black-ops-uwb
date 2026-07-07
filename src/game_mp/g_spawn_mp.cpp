#include "g_spawn_mp.h"
#include <game/g_load_utils.h>
#include <clientscript/cscr_vm.h>
#include "g_utils_mp.h"
#include <clientscript/cscr_stringlist.h>
#include <server_mp/sv_init_mp.h>
#include <game/pathnode_load_obj.h>
#include "actor_mp.h"
#include <clientscript/scr_const.h>
#include <game/g_mover.h>
#include "g_trigger_mp.h"
#include "g_misc_mp.h"
#include "game/turret.h"
#include <game/g_scr_mover.h>
#include <bgame/bg_misc.h>
#include <game/g_weapon.h>
#include <game/g_client_fields.h>
#include <cgame/cg_hudelem.h>
#include <bgame/bg_weapons_def.h>
#include <game/g_vehicle_path.h>

struct ent_field_t // sizeof=0x14
{                                       // XREF: .rdata:fields_1/r
    const char *name;
    int ofs;
    int size[1];
    fieldtype_t type;
    void (__cdecl *callback)(gentity_s *, int);
};

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
// Retail allows trigger_damage here; was missing (only in s_bspOnlySpawns). Revert: remove trigger_damage line and change [8] back to [7].
const SpawnFuncEntry s_bspOrDynamicSpawns[8] =
{
  { "info_notnull", SP_info_notnull },
  { "info_notnull_big", SP_info_notnull },
  { "trigger_radius", SP_trigger_radius },
  { "trigger_radius_use", SP_trigger_radius_use },
  { "trigger_damage", SP_trigger_damage },
  { "script_model", SP_script_model },
  { "script_origin", SP_script_origin },
  { "script_vehicle_collmap", SP_script_vehicle_collmap }
};




// Decomp: CoDMPServer.c:440355
int __cdecl G_SpawnFloat(const SpawnVar *spawnVar, const char *key, const char *defaultString, float *out)
{
    int present;
    const char *s;

    present = G_SpawnString(spawnVar, key, defaultString, &s);
    *out = atof(s);
    return present;
}

// Decomp: CoDMPServer.c:440366
int __cdecl G_SpawnInt(const SpawnVar *spawnVar, const char *key, const char *defaultString, int *out)
{
    int present;
    const char *s;

    present = G_SpawnString(spawnVar, key, defaultString, &s);
    *out = atoi(s);
    return present;
}

// Decomp: CoDMPServer.c:440377
void __cdecl Scr_ReadOnlyField(gentity_s *ent, int)
{
    Scr_Error("Tried to set a read only entity field", 0);
}

// Decomp: CoDMPServer.c:440383
void __cdecl G_FreeEntityWrapper(gentity_s *ent, SpawnVar *v)
{
    G_FreeEntity(ent);
}

// Decomp: CoDMPServer.c:440389
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
            if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp", 210, 0, fmtMsg) )
                __debugbreak();
            break;
    }
    return index;
}

// Decomp: CoDMPServer.c:440438
void __cdecl G_ParseEntityFields(const SpawnVar *spawnVar, gentity_s *ent, int radiant_update)
{
    int i;

    if ( !spawnVar->spawnVarsValid
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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

// Decomp: CoDMPServer.c:440460
void __cdecl G_ParseEntityField(const char *key, char *value, gentity_s *ent, int radiant_update)
{
    float tmp4;
    const ent_field_t *f;
    float vec[3];
    int modelIndex;

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
                                "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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

// Decomp: CoDMPServer.c:440533
void __cdecl G_SetEntityScriptVariable(const char *key, char *value, gentity_s *ent)
{
    unsigned int index;

    index = G_SetEntityScriptVariableInternal(key, value);
    if ( index )
        GScr_SetDynamicEntityField(ent, index);
}

// Decomp: CoDMPServer.c:440543
unsigned int __cdecl G_SpawnStruct(SpawnVar *spawnVar)
{
    unsigned int index;
    int i;
    const char *scriptLabel;
    unsigned int structId;

    if ( !spawnVar->spawnVarsValid
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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

// Decomp: CoDMPServer.c:440591
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
}

// Decomp: CoDMPServer.c:440634
void __cdecl G_DuplicateScriptFields(gentity_s *dest, const gentity_s *source)
{
    if ( dest->s.number != dest - g_entities
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
                    412,
                    0,
                    "%s",
                    "dest->s.number == dest - g_entities") )
    {
        __debugbreak();
    }
    if ( source->s.number != source - g_entities
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
                    413,
                    0,
                    "%s",
                    "source->s.number == source - g_entities") )
    {
        __debugbreak();
    }
    Scr_CopyEntityNum(source->s.number, dest->s.number, 0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:440662
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

// Decomp: CoDMPServer.c:440830
void __cdecl G_SetupDestructible(gentity_s *ent, char *destructibleName)
{
    char szConfigString[1024];
    int iDestID;

    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp", 533, 0, "%s", "ent") )
        __debugbreak();
    if ( !destructibleName
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp", 534, 0, "%s", "destructibleName") )
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
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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

// Decomp: CoDMPServer.c:440830
void __cdecl G_SetupDestructible(gentity_s *ent, SpawnVar *spawnVar)
{
    const char *destructibleName;

    destructibleName = 0;
    G_SpawnString(spawnVar, "destructibledef", 0, &destructibleName);
    if ( destructibleName )
        G_SetupDestructible(ent, (char *)destructibleName);
}

// Decomp: CoDMPServer.c:440841
void __cdecl G_CallSpawn(SpawnVar *spawnVar)
{
    const gitem_s *item;
    void (__cdecl *spawnFunc)(gentity_s *, SpawnVar *);
    const char *classname;
    gentity_s *ent;
    int radiantLiveUpdate;

    if ( !spawnVar->spawnVarsValid
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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
                        spawnFunc = G_FindSpawnFunc(classname, s_bspOrDynamicSpawns, 8);
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

// Decomp: CoDMPServer.c:440958
// KISAK FIX: use SpawnVar* spawn callback (retail) and pass spawnVar=0 so SP_* reads extra spawn() args from script VM.
// Revert: restore void (*spawnFunc)(gentity_s*) cast and ((void (*)(gentity_s*, unsigned int))spawnFunc)(ent, 0).
int __cdecl G_CallSpawnEntity(gentity_s *ent)
{
    const gitem_s *item;
    void (__cdecl *spawnFunc)(gentity_s *, SpawnVar *);
    char *classname;

    if ( level.spawnVar.spawnVarsValid
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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
            spawnFunc = G_FindSpawnFunc(classname, s_bspOrDynamicSpawns, 8);
            if ( spawnFunc )
            {
                if ( spawnFunc == G_FreeEntityWrapper
                    && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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
                                "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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

// Decomp: CoDMPServer.c:441031
void __cdecl GScr_AddFieldsForEntity()
{
    const ent_field_t *f;

    for ( f = fields_1; f->name; ++f )
    {
        if ( ((f - fields_1) & 0xC000) != 0
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
                        747,
                        0,
                        "%s",
                        "((f - fields) & ENTFIELD_MASK) == ENTFIELD_ENTITY") )
        {
            __debugbreak();
        }
        if ( f - fields_1 != (unsigned __int16)(f - fields_1)
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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

// Decomp: CoDMPServer.c:441067
void __cdecl GScr_AddFieldsForRadiant()
{
    Scr_AddFields("radiant", (char*)"txt", SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDMPServer.c:441074
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

// Decomp: CoDMPServer.c:441113
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
            if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp", 894, 0, fmtMsg2) )
                __debugbreak();
            return 1;
    }
}

// Decomp: CoDMPServer.c:441147
int __cdecl Scr_SetEntityField(unsigned int entnum, unsigned int offset)
{
    const ent_field_t *f;
    gentity_s *ent;

    if ( entnum >= 0x400
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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
            if ( offset >= 0xF
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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

// Decomp: CoDMPServer.c:441236
void __cdecl Scr_GetEntityField(unsigned int entnum, unsigned int offset)
{
    gentity_s *ent;

    if ( entnum >= 0x400
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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
            if ( offset >= 0xF
                && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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

// Decomp: CoDMPServer.c:441296
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

// Decomp: CoDMPServer.c:441351
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
            if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp", 1023, 0, fmtMsg) )
                __debugbreak();
            break;
    }
}

// Decomp: CoDMPServer.c:441383
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

// Decomp: CoDMPServer.c:441401
void __cdecl Scr_FreeEntity(gentity_s *ent)
{
    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp", 1065, 0, "%s", "ent") )
        __debugbreak();
    if ( ent->s.number != ent - g_entities
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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

// Decomp: CoDMPServer.c:441443
void __cdecl Scr_AddEntity(gentity_s *ent, scriptInstance_t inst)
{
    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp", 1076, 0, "%s", "ent") )
        __debugbreak();
    if ( ent->s.number != ent - g_entities
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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

// Decomp: CoDMPServer.c:441484
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
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
                    1101,
                    0,
                    "%s",
                    "entref.entnum < MAX_GENTITIES") )
    {
        __debugbreak();
    }
    return &g_entities[entref.entnum];
}

// Decomp: CoDMPServer.c:441519
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
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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

// Decomp: CoDMPServer.c:441558
void __cdecl Scr_FreeHudElem(game_hudelem_s *hud)
{
    if ( !hud && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp", 1137, 0, "%s", "hud") )
        __debugbreak();
    if ( (unsigned int)(hud - g_hudelems) >= 0x400
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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

// Decomp: CoDMPServer.c:441600
void __cdecl Scr_AddHudElem(game_hudelem_s *hud)
{
    if ( !hud && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp", 1158, 0, "%s", "hud") )
        __debugbreak();
    if ( (unsigned int)(hud - g_hudelems) >= 0x400
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
                    1160,
                    0,
                    "%s",
                    "hud->elem.type != HE_TYPE_FREE") )
    {
        __debugbreak();
    }
    Scr_AddEntityNum(hud - g_hudelems, 1u, SCRIPTINSTANCE_SERVER, 0);
}

// Decomp: CoDMPServer.c:441640
unsigned __int16 __cdecl Scr_ExecEntThread(gentity_s *ent, int handle, unsigned int paramcount)
{
    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp", 1193, 0, "%s", "ent") )
        __debugbreak();
    if ( ent->s.number != ent - g_entities
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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

// Decomp: CoDMPServer.c:441681
void __cdecl Scr_Notify(gentity_s *ent, unsigned __int16 stringValue, unsigned int paramcount)
{
    char *slStr;
    const char *fmtMsg;

    if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp", 1223, 0, "%s", "ent") )
        __debugbreak();
    if ( ent->s.number != ent - g_entities )
    {
        slStr = SL_ConvertToString(stringValue, SCRIPTINSTANCE_SERVER);
        fmtMsg = va("s.number = %i, index = %i, stringValue = %s", ent->s.number, ent - g_entities, slStr);
        if ( !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
                        1224,
                        0,
                        "%s\n\t%s",
                        "ent->s.number == ent - g_entities",
                        fmtMsg) )
            __debugbreak();
    }
    if ( !ent->r.inuse
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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

// Decomp: CoDMPServer.c:441727
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
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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

// Decomp: CoDMPServer.c:441785
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
                            "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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

// Decomp: CoDMPServer.c:441860
void __cdecl GScr_SetDynamicEntityField(gentity_s *ent, unsigned int index)
{
    Scr_SetDynamicEntityField(ent->s.number, 0, index, SCRIPTINSTANCE_SERVER, 0);
}

// Decomp: CoDMPServer.c:441866
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
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp", 1381, 0, "%s", "bg_gravity") )
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
    G_SpawnString(spawnVar, "spawnflags", "0", &s);
    g_entities[1022].spawnflags = atoi(s);
    g_entities[1022].s.number = 1022;
    Scr_SetString(&g_entities[1022].classname, scr_const.worldspawn, SCRIPTINSTANCE_SERVER);
    g_entities[1022].r.inuse = 1;
    if ( g_entities[1022].r.ownerNum.isDefined() )
    {
        if ( !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
                        1403,
                        0,
                        "%s",
                        "!g_entities[ENTITYNUM_WORLD].r.ownerNum.isDefined()") )
            __debugbreak();
    }
}

// Decomp: CoDMPServer.c:441939
void __cdecl G_SpawnEntitiesFromString()
{
    if ( !G_ParseSpawnVars(&level.spawnVar) )
        Com_Error(ERR_DROP, "SpawnEntities: no entities");
    SP_worldspawn(&level.spawnVar);
    while ( G_ParseSpawnVars(&level.spawnVar) )
        G_CallSpawn(&level.spawnVar);
    G_ResetEntityParsePoint();
}

// Decomp: CoDMPServer.c:441950
void __cdecl G_LoadStructs()
{
    unsigned __int16 hThread;
    const char *classname;

    if ( !g_scr_data.initstructs
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_spawn_mp.cpp",
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

