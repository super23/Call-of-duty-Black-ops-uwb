#include "g_actor_prone.h"
#include "actor.h"
#include <universal/assertive.h>

#ifdef KISAK_SP
#include <bgame/bg_actor_prone.h>
#include <bgame/bg_misc.h>
#include <game/g_main_wrapper.h>
#include <qcommon/dobj_management.h>
#include <xanim/xanim.h>
#include <cmath>
#endif

void __cdecl G_InitActorProneInfo(actor_s *actor)
{
    if ( !actor && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_actor_prone.cpp", 65, 0, "%s", "actor") )
        __debugbreak();
    *(unsigned int *)&actor->ProneInfo.bCorpseOrientation = 0;
    actor->ProneInfo.iProneTime = 0;
    actor->ProneInfo.iProneTrans = 0;
    actor->ProneInfo.fBodyHeight = 0.0;
    actor->ProneInfo.fTorsoPitch = 0.0;
    actor->ProneInfo.fWaistPitch = 0.0;
#ifdef KISAK_SP
    actor->proneAnimInfo.animProneLow = 0;
    actor->proneAnimInfo.animProneLevel = 0;
    actor->proneAnimInfo.animProneHigh = 0;
    actor->proneAnimInfo.bProneAnimSetup = 0;
#endif
}

#ifdef KISAK_SP
// Decomp: CoDSP_rdBlackOps.map.c (G_ActorEnterProne)
void __cdecl G_ActorEnterProne(actor_s *actor, unsigned int iTransTime)
{
    actor_prone_info_s *pProneInfo;
    gentity_s *ent;
    float *currentOrigin;
    double actorProneFraction;

    pProneInfo = &actor->ProneInfo;
    if ( BG_ActorIsProne(pProneInfo, level.time) )
    {
        if ( pProneInfo->iProneTrans && iTransTime != (unsigned int)pProneInfo->iProneTrans )
        {
            actorProneFraction = BG_GetActorProneFraction(pProneInfo, level.time);
            if ( actorProneFraction < 1.0 )
            {
                if ( !pProneInfo->prone
                    && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_actor_prone.cpp",
                                75,
                                0,
                                "%s",
                                "actor->ProneInfo.prone") )
                {
                    __debugbreak();
                }
                pProneInfo->iProneTime = level.time - (int)((float)iTransTime * (float)actorProneFraction);
            }
            pProneInfo->iProneTrans = iTransTime;
        }
    }
    else
    {
        ent = actor->ent;
        currentOrigin = ent->r.currentOrigin;
        pProneInfo->bCorpseOrientation = 0;
        actor->ProneInfo.prone = 1;
        actor->ProneInfo.iProneTrans = iTransTime;
        actor->Physics.prone = 1;
        actor->ProneInfo.iProneTime = level.time;
        actor->bProneOK = BG_CheckProne(
            0,
            ent->s.number,
            currentOrigin,
            15.0f,
            48.0f,
            ent->r.currentAngles[1],
            &actor->ProneInfo.fTorsoPitch,
            &actor->ProneInfo.fWaistPitch,
            false,
            true,
            true,
            1,
            PCT_ACTOR,
            50.0f);
    }
    if ( !BG_ActorGoalIsProne(pProneInfo)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_actor_prone.cpp",
                    92,
                    0,
                    "%s",
                    "BG_ActorGoalIsProne( &actor->ProneInfo )") )
    {
        __debugbreak();
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (G_ActorExitProne)
void __cdecl G_ActorExitProne(actor_s *actor, unsigned int iTransTime)
{
    actor_prone_info_s *pProneInfo;
    int time;

    pProneInfo = &actor->ProneInfo;
    if ( BG_ActorIsProne(pProneInfo, level.time) )
    {
        if ( !pProneInfo->iProneTrans || iTransTime == (unsigned int)pProneInfo->iProneTrans )
        {
            time = level.time;
        }
        else
        {
            time = level.time - (int)((float)iTransTime * BG_GetActorProneFraction(pProneInfo, level.time));
        }
        actor->ProneInfo.iProneTime = time;
        if ( !actor->ProneInfo.prone
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_actor_prone.cpp",
                        113,
                        0,
                        "%s",
                        "actor->ProneInfo.prone") )
        {
            __debugbreak();
        }
        actor->ProneInfo.iProneTrans = -(int)iTransTime;
    }
    if ( BG_ActorGoalIsProne(pProneInfo)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_actor_prone.cpp",
                    117,
                    0,
                    "%s",
                    "!BG_ActorGoalIsProne( &actor->ProneInfo )") )
    {
        __debugbreak();
    }
}

// Decomp: CoDSP_rdBlackOps.map.c (Actor_UpdateProneInformation)
void __cdecl Actor_UpdateProneInformation(actor_s *self, int bDoProneCheck)
{
    double v7;
    double v8;
    double v9;
    double v10;
    double v11;
    double v12;
    double v13;
    DObj *serverDObj;

    if ( !self->proneAnimInfo.animProneLow
        || !self->proneAnimInfo.animProneLevel
        || !self->proneAnimInfo.animProneHigh )
    {
        return;
    }
    if ( bDoProneCheck )
    {
        self->bProneOK = BG_CheckProne(
            0,
            self->ent->s.number,
            self->ent->r.currentOrigin,
            15.0,
            48.0,
            self->ent->r.currentAngles[1],
            &self->ProneInfo.fTorsoPitch,
            &self->ProneInfo.fWaistPitch,
            true,
            true,
            true,
            1,
            PCT_ACTOR,
            50.0f);
    }
    else
    {
        self->fProneLastDiff = 360.0f;
    }
    v7 = (float)((self->ProneInfo.fWaistPitch - self->ProneInfo.fTorsoPitch) * 0.0027777778f);
    v8 = floor(v7 + 0.5);
    v9 = (float)((v7 - v8) * 360.0);
    if ( self->fProneLastDiff == (float)v9 )
        return;
    v10 = 0.0;
    v11 = 0.0;
    if ( v9 >= 0.0 )
    {
        v11 = (float)(self->fInvProneAnimHighPitch * v9);
        if ( v11 > 0.99f )
            v11 = 0.99f;
        v12 = 1.0;
        v13 = (float)(1.0 - v11);
    }
    else
    {
        v10 = (float)(self->fInvProneAnimLowPitch * v9);
        if ( v10 > 0.99f )
            v10 = 0.99f;
        v12 = 1.0;
        v13 = (float)(1.0 - v10);
    }
    serverDObj = Com_GetServerDObj(self->ent->s.number);
    XAnimSetGoalWeight(serverDObj, self->proneAnimInfo.animProneLow, (float)v10, 0.05f, (float)v12, 0, 0, 0, -1);
    XAnimSetGoalWeight(serverDObj, self->proneAnimInfo.animProneLevel, (float)v13, 0.05f, (float)v12, 0, 0, 0, -1);
    XAnimSetGoalWeight(serverDObj, self->proneAnimInfo.animProneHigh, (float)v11, 0.05f, (float)v12, 0, 0, 0, -1);
    self->fProneLastDiff = (float)v9;
}
#endif
