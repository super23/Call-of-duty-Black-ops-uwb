#include "actor_animapi.h"
#include <clientscript/cscr_variable.h>
#include <clientscript/cscr_vm.h>
#include <game/g_spawn_wrapper.h>
#include <clientscript/scr_const.h>
#include "actor_orientation.h"
#include <clientscript/cscr_stringlist.h>
#include <game/actor_wrapper.h>

#ifdef KISAK_SP
AnimScriptList *g_animScriptTable[4];
#else
AnimScriptList *g_animScriptTable[1];
#endif

void __fastcall Actor_InitAnim(actor_s *self)
{
    if ( self->AnimScriptHandle
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\actor_animapi.cpp",
                    24,
                    0,
                    "%s",
                    "self->AnimScriptHandle == 0") )
    {
        __debugbreak();
    }
    if ( self->pAnimScriptFunc
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\actor_animapi.cpp",
                    25,
                    0,
                    "%s",
                    "self->pAnimScriptFunc == NULL") )
    {
        __debugbreak();
    }
    if ( self->eAnimMode )
    {
        if ( !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\actor_animapi.cpp",
                        26,
                        0,
                        "%s",
                        "self->eAnimMode == AI_ANIM_UNKNOWN") )
            __debugbreak();
    }
}

bool __fastcall Actor_IsAnimScriptAlive(actor_s *self)
{
    return self->AnimScriptHandle && Scr_IsThreadAlive(self->AnimScriptHandle, SCRIPTINSTANCE_SERVER);
}

void __fastcall Actor_KillAnimScript(actor_s *self)
{
    if ( !Scr_IsSystemActive(1u, SCRIPTINSTANCE_SERVER)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\actor_animapi.cpp",
                    56,
                    0,
                    "%s",
                    "Scr_IsSystemActive( SCR_SYS_GAME )") )
    {
        __debugbreak();
    }
#ifdef KISAK_SP
    scr_animscript_t *prevScript;

    prevScript = self->pAnimScriptFunc;
#endif
    self->pAnimScriptFunc = 0;
#ifdef KISAK_SP
    // Decomp: CoDSP — only clear per-actor negotiation/custom script when that script was active.
    // Do not key off AnimScriptSpecific.name alone: MP obstacle negotiation sets name+func on
    // push while pAnimScriptFunc still points at dog_move; clearing here breaks Actor_AnimSpecific.
    if ( prevScript == &self->AnimScriptSpecific )
    {
        self->AnimScriptSpecific.func = 0;
        Scr_SetString(&self->AnimScriptSpecific.name, 0, SCRIPTINSTANCE_SERVER);
    }
#endif
    if ( self->AnimScriptHandle )
    {
        Scr_Notify(self->ent, scr_const.killanimscript, 0);
        Scr_FreeThread(self->AnimScriptHandle, SCRIPTINSTANCE_SERVER);
        self->AnimScriptHandle = 0;
        self->arrivalInfo.animscriptOverrideRunTo = 0;
    }
}

void __fastcall Actor_SetAnimScript(
                actor_s *self,
                scr_animscript_t *pAnimScriptFunc,
                unsigned __int8 moveMode,
                ai_animmode_t animMode,
                scriptAnimAIFunctionTypes_t animScript)
{
    PROF_SCOPED("setanimscript");

    iassert(self);
    iassert(pAnimScriptFunc);

    if ( self->pAnimScriptFunc == pAnimScriptFunc )
    {
        if ( self->eScriptSetAnimMode )
            self->eAnimMode = self->eScriptSetAnimMode;
        else
            self->eAnimMode = animMode;
        if ( self->moveMode == moveMode )
        {
            return;
        }
        else
        {
            self->moveMode = moveMode;
            Scr_Notify(self->ent, scr_const.movemode, 0);
        }
        return;
    }
    Scr_DecTime(SCRIPTINSTANCE_SERVER);
    Actor_KillAnimScript(self);
    Scr_RunCurrentThreads(SCRIPTINSTANCE_SERVER);
    Actor_ClearScriptOrient(self);
    self->bGrenadeTossValid = 0;
    self->safeToChangeScript = 1;
    self->eScriptSetAnimMode = AI_ANIM_UNKNOWN;
    self->ent->flags &= ~0x1000u;
    Scr_SetString(&self->scriptState, 0, SCRIPTINSTANCE_SERVER);
    Scr_SetString(&self->stateChangeReason, 0, SCRIPTINSTANCE_SERVER);
    self->pAnimScriptFunc = pAnimScriptFunc;
    self->eAnimMode = animMode;
    self->moveMode = moveMode;
    self->pushable = 1;
    Actor_ClearKeepClaimedNode(self);
    self->AnimScriptHandle = Scr_ExecEntThread(self->ent, pAnimScriptFunc->func, 0);
    if ( !self->AnimScriptHandle
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\game\\actor_animapi.cpp",
                    157,
                    0,
                    "%s",
                    "self->AnimScriptHandle") )
    {
        __debugbreak();
    }
    G_XAnimUpdateEnt(self->ent);
    Scr_IncTime(SCRIPTINSTANCE_SERVER);
}

void __fastcall Actor_AnimStop(actor_s *self, scr_animscript_t *pAnimScriptFunc)
{
    if ( !pAnimScriptFunc
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\actor_animapi.cpp", 194, 0, "%s", "pAnimScriptFunc") )
    {
        __debugbreak();
    }
    Actor_CheckCollisions(self);
    if ( self->pCloseEnt.isDefined() )
    {
        Actor_AnimMoveAway(self, pAnimScriptFunc);
    }
    else
    {
        if ( !pAnimScriptFunc
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\actor_animapi.cpp",
                        200,
                        0,
                        "%s",
                        "pAnimScriptFunc") )
        {
            __debugbreak();
        }
        Actor_SetAnimScript(self, pAnimScriptFunc, 0, AI_ANIM_MOVE_CODE, AI_ANIM_FUNCTION_STOP);
        self->bUseGoalWeight = 0;
    }
}

void __fastcall Actor_AnimMoveAway(actor_s *self, scr_animscript_t *pAnimScriptFunc)
{
    if ( !pAnimScriptFunc
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\actor_animapi.cpp", 176, 0, "%s", "pAnimScriptFunc") )
    {
        __debugbreak();
    }
    if ( (float)((float)(self->Physics.vVelocity[0] * self->Physics.vVelocity[0])
                         + (float)(self->Physics.vVelocity[1] * self->Physics.vVelocity[1])) >= 1.0 )
        Actor_SetAnimScript(self, &g_animScriptTable[self->species]->move, 2u, AI_ANIM_MOVE_CODE, AI_ANIM_FUNCTION_MOVE);
    else
        Actor_SetAnimScript(self, pAnimScriptFunc, 1u, AI_ANIM_MOVE_CODE, AI_ANIM_FUNCTION_MOVE);
    self->bUseGoalWeight = 0;
}

scr_animscript_t *__fastcall Actor_GetStopAnim(actor_s *self)
{
    return &g_animScriptTable[self->species]->stop;
}

void __fastcall Actor_AnimTryWalk(actor_s *self)
{
    scr_animscript_t *StopAnim; // eax

    if ( self->pCloseEnt.isDefined() )
    {
        Actor_AnimMoveAway(self, &g_animScriptTable[self->species]->stop);
    }
    else
    {
        if ( !Actor_HasPath(self)
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\actor_animapi.cpp",
                        283,
                        0,
                        "%s",
                        "Actor_HasPath( self )") )
        {
            __debugbreak();
        }
        if ( self->Path.iPathEndTime && self->Path.iPathEndTime - level.time < 200 )
        {
            StopAnim = Actor_GetStopAnim(self);
            Actor_SetAnimScript(self, StopAnim, 1u, AI_ANIM_MOVE_CODE, AI_ANIM_FUNCTION_MOVE);
        }
        else
        {
            Actor_SetAnimScript(self, &g_animScriptTable[self->species]->move, 2u, AI_ANIM_MOVE_CODE, AI_ANIM_FUNCTION_MOVE);
        }
        self->bUseGoalWeight = 0;
    }
}

void __fastcall Actor_AnimTryRun(actor_s *self)
{
    scr_animscript_t *StopAnim; // eax

    if ( self->pCloseEnt.isDefined() )
    {
        Actor_AnimMoveAway(self, &g_animScriptTable[self->species]->stop);
    }
    else
    {
        if ( !Actor_HasPath(self)
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\game\\actor_animapi.cpp",
                        328,
                        0,
                        "%s",
                        "Actor_HasPath( self )") )
        {
            __debugbreak();
        }
        if ( self->Path.iPathEndTime && self->Path.iPathEndTime - level.time <= 200 )
        {
            StopAnim = Actor_GetStopAnim(self);
            Actor_SetAnimScript(self, StopAnim, 1u, AI_ANIM_MOVE_CODE, AI_ANIM_FUNCTION_MOVE);
        }
        else
        {
            Actor_SetAnimScript(self, &g_animScriptTable[self->species]->move, 3u, AI_ANIM_MOVE_CODE, AI_ANIM_FUNCTION_MOVE);
        }
        self->bUseGoalWeight = 0;
    }
}

void __fastcall Actor_AnimPain(actor_s *self)
{
    Actor_SetAnimScript(self, &g_animScriptTable[self->species]->pain, 0, AI_ANIM_USE_BOTH_DELTAS, AI_ANIM_FUNCTION_PAIN);
    self->bUseGoalWeight = 0;
}

void __fastcall Actor_AnimReact(actor_s *self)
{
    Actor_SetAnimScript(self, &g_animScriptTable[self->species]->react, 0, AI_ANIM_USE_BOTH_DELTAS, AI_ANIM_FUNCTION_REACT);
    self->bUseGoalWeight = 0;
}

void __fastcall Actor_AnimDeath(actor_s *self)
{
    if ( self->eScriptSetAnimMode )
        Actor_SetAnimScript(
            self,
            &g_animScriptTable[self->species]->death,
            0,
            self->eScriptSetAnimMode,
            AI_ANIM_FUNCTION_DEATH);
    else
        Actor_SetAnimScript(
            self,
            &g_animScriptTable[self->species]->death,
            0,
            AI_ANIM_USE_BOTH_DELTAS,
            AI_ANIM_FUNCTION_DEATH);
    self->bUseGoalWeight = 0;
}

void __fastcall Actor_AnimSpecific(actor_s *self, scr_animscript_t *func, ai_animmode_t eAnimMode, bool bUseGoalWeight)
{
    if ( !func && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\actor_animapi.cpp", 457, 0, "%s", "func") )
        __debugbreak();
    if ( self->eScriptSetAnimMode )
        Actor_SetAnimScript(self, func, 0, self->eScriptSetAnimMode, AI_ANIM_FUNCTION_STOP);
    else
        Actor_SetAnimScript(self, func, 0, eAnimMode, AI_ANIM_FUNCTION_STOP);
    self->bUseGoalWeight = bUseGoalWeight;
}

void __fastcall Actor_AnimScripted(actor_s *self)
{
    Actor_SetAnimScript(self, &g_animScriptTable[self->species]->scripted, 0, AI_ANIM_USE_BOTH_DELTAS, AI_ANIM_FUNCTION_STOP);
    self->bUseGoalWeight = 1;
}

void __stdcall Actor_AnimSetCompleteGoalWeight(
                XAnimTree_s *tree,
                unsigned int animIndex,
                float goalWeight,
                float goalTime,
                float rate,
                unsigned int notifyName,
                unsigned int notifyType,
                int bRestart)
{
    ;
}

void __stdcall Actor_AnimClearGoalWeight(unsigned int animIndex, float blendTime)
{
    ;
}

