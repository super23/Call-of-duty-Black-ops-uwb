#include "actor_animscripted.h"

#include "actor_state.h"
#include "actor_orientation.h"
#include "actor_animapi.h"
#include "actor_sp.h"
#include <game/g_spawn_wrapper.h>

#include <clientscript/scr_const.h>
#include <game/sentient.h>

bool __fastcall Actor_CustomAnim_Start(actor_s *self, ai_state_t ePrevState)
{
    Scr_Notify(self->ent, scr_const.begin_custom_anim, 0);
    return true;
}

actor_think_result_t __fastcall Actor_CustomAnim_Think(actor_s *self)
{
    scr_animscript_t *pAnimScriptSpecific;

    pAnimScriptSpecific = &self->AnimScriptSpecific;
    if ( !self->AnimScriptSpecific.func )
    {
        Actor_PopState(self);
        return ACTOR_THINK_REPEAT;
    }

    self->pszDebugInfo = "animcustom";
    Actor_ClearKeepClaimedNode(self);
    Sentient_ClaimNode(self->sentient, 0);
    Actor_ClearPath(self);
    Actor_AnimSpecific(self, pAnimScriptSpecific, AI_ANIM_USE_BOTH_DELTAS, 1);
    if ( Actor_IsAnimScriptAlive(self) )
    {
        iassert(self->ent);
        Actor_PreThink(self);
        iassert(self->eAnimMode != AI_ANIM_UNKNOWN);
        Actor_UpdateOriginAndAngles(self);
        return ACTOR_THINK_DONE;
    }
    Actor_PopState(self);
    return ACTOR_THINK_REPEAT;
}
