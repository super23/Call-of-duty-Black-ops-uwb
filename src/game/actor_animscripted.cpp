#include "actor_animscripted.h"

#include "g_animscripted.h"
#include "actor_state.h"
#include "actor_orientation.h"
#include "actor_animapi.h"
#include "actor_sp.h"

#include <clientscript/cscr_memorytree.h>
#include <clientscript/cscr_vm.h>
#include <clientscript/scr_const.h>
#include <game/sentient.h>
#include "g_spawn_sp.h"

bool __fastcall Actor_ScriptedAnim_Start(actor_s *self, ai_state_t ePrevState)
{
    return true;
}

void __fastcall Actor_ScriptedAnim_Finish(actor_s *self, ai_state_t eNextState)
{
    gentity_s *ent;

    ent = self->ent;
    iassert(ent);
    if ( ent->scripted )
    {
        MT_Free((unsigned char *)ent->scripted, sizeof(animscripted_s), SCRIPTINSTANCE_SERVER);
        ent->scripted = 0;
    }
}

actor_think_result_t __fastcall Actor_ScriptedAnim_Think(actor_s *self)
{
    gentity_s *ent;

    self->pszDebugInfo = "animscripted";
    Actor_ClearKeepClaimedNode(self);
    Sentient_ClaimNode(self->sentient, 0);
    Actor_ClearPath(self);
    Actor_AnimScripted(self);
    self->pushable = 0;

    if ( !Actor_IsAnimScriptAlive(self) )
    {
        iassert(self->eSimulatedState[self->simulatedStateLevel] == AIS_SCRIPTEDANIM || self->eState[self->stateLevel] == AIS_DEATH);
        if ( self->eSimulatedState[self->simulatedStateLevel] != AIS_SCRIPTEDANIM )
            return ACTOR_THINK_REPEAT;
    }

    ent = self->ent;
    iassert(ent);
    G_Animscripted_Think(ent);

    if ( !ent->scripted )
    {
        Actor_PopState(self);
        return ACTOR_THINK_REPEAT;
    }

    Actor_PreThink(self);
    Actor_SetDesiredAngles(&self->CodeOrient, ent->r.currentAngles[0], ent->r.currentAngles[1]);

    Vec3Clear(self->Physics.vVelocity);
    Vec3Clear(self->Physics.vWishDelta);

    return ACTOR_THINK_DONE;
}

