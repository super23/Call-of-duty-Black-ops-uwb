#include "actor_react.h"

#include "actor_animapi.h"
#include "actor_events.h"
#include "actor_orientation.h"
#include "actor_sp.h"
#include "actor_state.h"
#include <clientscript/scr_const.h>
#include <game/actor_wrapper.h>
#include <game/g_main_wrapper.h>
#include <game/g_spawn_wrapper.h>

bool __fastcall Actor_React_Start(actor_s *self, ai_state_t ePrevState)
{
    iassert(self);
    self->iPainTime = level.time;
    Actor_ClearPath(self);
    Actor_AnimReact(self);
    Scr_Notify(self->ent, scr_const.react, 0);
    return true;
}

void __fastcall Actor_React_Finish(actor_s *self, ai_state_t eNextState)
{
    iassert(self);
    iassert(self->ent);
    if ( self->sentient->lastAttacker.isDefined() )
    {
        Actor_BroadcastTeamEvent(self->sentient, AI_EV_REACT);
        self->sentient->lastAttacker.setEnt(0);
    }
}

actor_think_result_t __fastcall Actor_React_Think(actor_s *self)
{
    PROF_SCOPED("reactthink");

    iassert(self);
    iassert(self->ent);
    self->pszDebugInfo = "react";
    if ( level.time <= self->iPainTime || Actor_IsAnimScriptAlive(self) )
    {
        if ( self->sentient->lastAttacker.isDefined() && level.time - self->iPainTime >= 500 )
        {
            Actor_BroadcastTeamEvent(self->sentient, AI_EV_REACT);
            self->sentient->lastAttacker.setEnt(0);
        }
        Actor_PreThink(self);
        Actor_SetOrientMode(self, AI_ORIENT_DONT_CHANGE);
        Actor_AnimReact(self);
        Actor_PostThink(self);
        return ACTOR_THINK_DONE;
    }

    Actor_PopState(self);
    return ACTOR_THINK_REPEAT;
}
