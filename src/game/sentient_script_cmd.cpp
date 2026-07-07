#include "sentient_script_cmd.h"
#include <game/g_main_wrapper.h>
#include <clientscript/cscr_vm.h>
#include <clientscript/cscr_stringlist.h>
#include <cgame/cg_scr_main.h>
#include <game/g_spawn_wrapper.h>
#include "actor_threat.h"
#include "bullet.h"
#include "actor.h"
#include "actor_sp.h"

static const BuiltinMethodDef s_sentientMethods[] =
{
    { "getclosestenemysqdist", SentientCmd_GetClosestEnemySqDist, 0 },
    { "getenemysqdist", SentientCmd_GetEnemySqDist, 0 },
    { "getthreatbias", SentientCmd_GetThreatBias, 0 },
    { "getthreatbiasgroup", SentientCmd_GetThreatBiasGroup, 0 },
    { "isnotarget", SentientCmd_IsNoTarget, 0 },
    { "setthreatbiasgroup", SentientCmd_SetThreatBiasGroup, 0 },
};

// Decomp: CoDSP_rdBlackOps.map.c (Sentient_Get ~6239756)
sentient_s *__fastcall Sentient_Get(scr_entref_t entref)
{
    gentity_s *ent;

    if ( !entref.classnum )
    {
        if ( entref.entnum >= MAX_GENTITIES
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\sentient_script_cmd.cpp",
                        0x2Fu,
                        0,
                        "%s",
                        "entref.entnum < MAX_GENTITIES") )
        {
            __debugbreak();
        }
        ent = &g_entities[entref.entnum];
        if ( ent->sentient )
            return ent->sentient;
    }
    Scr_ObjectError("not a sentient", SCRIPTINSTANCE_SERVER);
    return 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (Sentient_GetMethod ~6240304)
void(__cdecl *__cdecl Sentient_GetMethod(const char **pName))(scr_entref_t)
{
    unsigned int i;

    for ( i = 0; i < ARRAY_COUNT(s_sentientMethods); ++i )
    {
        if ( !strcmp(*pName, s_sentientMethods[i].actionString) )
        {
            *pName = s_sentientMethods[i].actionString;
            return s_sentientMethods[i].actionFunc;
        }
    }
    return 0;
}

// Decomp: CoDSP_rdBlackOps.map.c (SentientCmd_GetClosestEnemySqDist ~6239820)
void __cdecl SentientCmd_GetClosestEnemySqDist(scr_entref_t entref)
{
    sentient_s *self;
    actor_s *selfActor;
    float minDistSq;
    float myOrigin[3];
    int teamFlags;

    self = Sentient_Get(entref);
    selfActor = self->ent->actor;
    minDistSq = 100000000.0f;
    teamFlags = 1 << Sentient_EnemyTeam(self->eTeam);
    if ( teamFlags )
    {
        Sentient_GetOrigin(self, myOrigin);
        for ( sentient_s *enemy = Sentient_FirstSentient(teamFlags); enemy; enemy = Sentient_NextSentient(enemy, teamFlags) )
        {
            sentient_info_t *info;

            if ( (enemy->ent->flags & 4) != 0 )
                continue;
            if ( Actor_CheckIgnore(self, enemy) )
                continue;
            if ( selfActor )
            {
                info = &selfActor->sentientInfo[enemy - level.sentients];
                if ( info->lastKnownPosTime <= 0 )
                    continue;
            }
            float enemyOrigin[3];
            Sentient_GetOrigin(enemy, enemyOrigin);
            float distSq = Vec3DistanceSq(myOrigin, enemyOrigin);
            if ( distSq < minDistSq )
                minDistSq = distSq;
        }
        Scr_AddFloat(minDistSq, SCRIPTINSTANCE_SERVER);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (SentientCmd_GetEnemySqDist ~826864E0)
void __cdecl SentientCmd_GetEnemySqDist(scr_entref_t entref)
{
    (void)entref;
    Scr_Error("GetEnemySqDist is depricated, use GetClosestEnemySqDist.\n", 0);
}

// Decomp: CoDSP_rdBlackOps.map.c (SentientCmd_GetThreatBias ~6239949)
void __cdecl SentientCmd_GetThreatBias(scr_entref_t entref)
{
    unsigned int groupFor;
    unsigned int groupAgainst;
    int groupForIndex;
    int groupAgainstIndex;
    const char *groupNameStr;
    int threatBias;

    (void)entref;
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2 )
        Scr_ParamError(0u, "getthreatbias [group for] [group against]", SCRIPTINSTANCE_SERVER);
    groupFor = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
    groupForIndex = Actor_FindThreatBiasGroupIndex(groupFor);
    groupAgainst = Scr_GetConstString(1u, SCRIPTINSTANCE_SERVER);
    groupAgainstIndex = Actor_FindThreatBiasGroupIndex(groupAgainst);
    if ( groupForIndex >= 0 )
    {
        if ( groupAgainstIndex >= 0 )
        {
            threatBias = Actor_GetThreatBias(groupForIndex, groupAgainstIndex);
            Scr_AddInt(threatBias, SCRIPTINSTANCE_SERVER);
        }
        else
        {
            groupNameStr = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
            Scr_Error(va("Invalid threat bias group '%s'.\n", groupNameStr), 0);
        }
    }
    else
    {
        groupNameStr = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        Scr_Error(va("Invalid threat bias group '%s'.\n", groupNameStr), 0);
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (SentientCmd_GetThreatBiasGroup ~6240280)
void __cdecl SentientCmd_GetThreatBiasGroup(scr_entref_t entref)
{
    sentient_s *self;
    const char *groupName;

    self = Sentient_Get(entref);
    if ( self->iThreatBiasGroupIndex <= 0 )
    {
        Scr_AddString("", SCRIPTINSTANCE_SERVER);
        return;
    }
    groupName = SL_ConvertToString(g_threatBias.groupName[self->iThreatBiasGroupIndex], SCRIPTINSTANCE_SERVER);
    Scr_AddString(groupName, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (SentientCmd_IsNoTarget ~82686D78)
void __cdecl SentientCmd_IsNoTarget(scr_entref_t entref)
{
    sentient_s *self;

    self = Sentient_Get(entref);
    Scr_AddBool((self->ent->flags & 4) != 0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (SentientCmd_SetThreatBiasGroup ~82686C30)
void __cdecl SentientCmd_SetThreatBiasGroup(scr_entref_t entref)
{
    sentient_s *self;
    unsigned int groupName;
    int groupIndex;
    const char *groupNameStr;

    self = Sentient_Get(entref);
    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) == 1 )
    {
        groupName = Scr_GetConstString(0, SCRIPTINSTANCE_SERVER);
        groupIndex = Actor_FindThreatBiasGroupIndex(groupName);
        if ( groupIndex >= 0 )
            self->iThreatBiasGroupIndex = groupIndex;
        else
        {
            groupNameStr = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
            Scr_Error(va("Invalid threat bias group '%s'.\n", groupNameStr), 0);
        }
    }
    else
    {
        self->iThreatBiasGroupIndex = 0;
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (SentientCmd_SetThreatBias ~826869B8)
void __cdecl SentientCmd_SetThreatBias()
{
    int groupForIndex;
    int groupAgainstIndex;
    int threat;
    const char *groupNameStr;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 3 )
        Scr_ParamError(0u, "setthreatbias [threatener] [threatened] [threat]", SCRIPTINSTANCE_SERVER);
    groupForIndex = Actor_FindThreatBiasGroupIndex(Scr_GetConstString(0, SCRIPTINSTANCE_SERVER));
    groupAgainstIndex = Actor_FindThreatBiasGroupIndex(Scr_GetConstString(1u, SCRIPTINSTANCE_SERVER));
    threat = Scr_GetInt(2u, SCRIPTINSTANCE_SERVER);
    if ( groupForIndex < 0 )
    {
        groupNameStr = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        Scr_Error(va("Invalid threat bias group '%s'.\n", groupNameStr), 0);
    }
    if ( groupAgainstIndex < 0 )
    {
        groupNameStr = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
        Scr_Error(va("Invalid threat bias group '%s'.\n", groupNameStr), 0);
    }
    Actor_SetThreatBias(groupForIndex, groupAgainstIndex, threat);
}

// Decomp: CoDSP_rdBlackOps.map.c (SentientCmd_SetThreatBiasAgainstAll ~82686AA8)
void __cdecl SentientCmd_SetThreatBiasAgainstAll()
{
    int groupIndex;
    int threat;
    const char *groupNameStr;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2 )
        Scr_ParamError(0u, "setthreatbiasagainstall [group] [threat]", SCRIPTINSTANCE_SERVER);
    groupIndex = Actor_FindThreatBiasGroupIndex(Scr_GetConstString(0, SCRIPTINSTANCE_SERVER));
    threat = Scr_GetInt(1u, SCRIPTINSTANCE_SERVER);
    if ( groupIndex < 0 )
    {
        groupNameStr = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        Scr_Error(va("Invalid threat bias group '%s'.\n", groupNameStr), 0);
    }
    Actor_SetThreatBiasEntireGroup(groupIndex, threat);
}

// Decomp: CoDSP_rdBlackOps.map.c (SentientCmd_ThreatBiasGroupExists ~82686858)
void __cdecl SentientCmd_ThreatBiasGroupExists()
{
    int groupIndex;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 1 )
        Scr_ParamError(0u, "threatbiasgroupexists [name]", SCRIPTINSTANCE_SERVER);
    groupIndex = Actor_FindThreatBiasGroupIndex(Scr_GetConstString(0, SCRIPTINSTANCE_SERVER));
    Scr_AddBool(groupIndex >= 0, SCRIPTINSTANCE_SERVER);
}

// Decomp: CoDSP_rdBlackOps.map.c (SentientCmd_SetIgnoreMeGroup ~82686B50)
void __cdecl SentientCmd_SetIgnoreMeGroup()
{
    int groupForIndex;
    int groupAgainstIndex;
    const char *groupNameStr;

    if ( Scr_GetNumParam(SCRIPTINSTANCE_SERVER) != 2 )
        Scr_ParamError(0u, "setignoremegroup [group for] [group ignoring]", SCRIPTINSTANCE_SERVER);
    groupForIndex = Actor_FindThreatBiasGroupIndex(Scr_GetConstString(0, SCRIPTINSTANCE_SERVER));
    groupAgainstIndex = Actor_FindThreatBiasGroupIndex(Scr_GetConstString(1u, SCRIPTINSTANCE_SERVER));
    if ( groupForIndex < 0 )
    {
        groupNameStr = Scr_GetString(0, SCRIPTINSTANCE_SERVER);
        Scr_Error(va("Invalid threat bias group '%s'.\n", groupNameStr), 0);
    }
    if ( groupAgainstIndex < 0 )
    {
        groupNameStr = Scr_GetString(1u, SCRIPTINSTANCE_SERVER);
        Scr_Error(va("Invalid threat bias group '%s'.\n", groupNameStr), 0);
    }
    Actor_SetIgnoreMeGroup(groupForIndex, groupAgainstIndex);
}
