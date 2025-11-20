#pragma once

void __cdecl setup_gjkcc_input(pmove_t *pm, gjkcc_input_t *gjkcc_in);
void __cdecl AIPhys_AddTouchEnt(actor_physics_t *pPhys, int entityNum);
void __cdecl setup_gjkcc_input(actor_physics_t *pPhys, gjkcc_input_t *gjkcc_in);
void __cdecl ai_physics_trace(
        trace_t *trace,
        float *start,
        float *mins,
        float *maxs,
        float *end,
        int passEntityNum,
        int contentmask,
        actor_physics_t *pPhys);
void __cdecl setup_ai_gjk_slide_move_input(actor_physics_t *pPhys, ai_gjk_slide_move_input_t *input, int gravity);
void __cdecl AIPhys_FoliageSounds(actor_physics_t *pPhys);
bool __cdecl Actor_Physics_1(actor_physics_t *pPhys);
bool __cdecl AIPhys_AirMove(actor_physics_t *pPhys);
void __cdecl AIPhys_ClipVelocity(const float *in, const float *normal, bool isWalkable, float *out, float overbounce);
bool __cdecl AIPhys_StepSlideMove(actor_physics_t *pPhys, int gravity, int zonly);
int __cdecl AIPhys_SlideMove(actor_physics_t *pPhys, int gravity, int zonly);
void __thiscall ai_gjk_slide_move_input_t::custom_process(ai_gjk_slide_move_input_t *this, gjk_trace_output_t *gto);
actor_physics_t *__thiscall actor_physics_t::operator=(actor_physics_t *this, const actor_physics_t *__that);
colgeom_visitor_inlined_t<200> *__thiscall colgeom_visitor_inlined_t<200>::operator=(
        colgeom_visitor_inlined_t<200> *this,
        const colgeom_visitor_inlined_t<200> *__that);
colgeom_visitor_t *__thiscall colgeom_visitor_t::operator=(colgeom_visitor_t *this, const colgeom_visitor_t *__that);
bool __cdecl AIPhys_WalkMove(actor_physics_t *pPhys);
bool __cdecl AIPhys_ZOnlyPhysicsMove(actor_physics_t *pPhys);
void __cdecl AIPhys_NoClipMove(actor_physics_t *pPhys);
void __cdecl AIPhys_NoGravityMove(actor_physics_t *pPhys);
void __cdecl AIPhys_GroundTrace(actor_physics_t *pPhys);
int __cdecl Actor_Physics(actor_physics_t *pPhys);
int __cdecl Actor_Physics_z(actor_physics_t *pPhys);
void __cdecl Actor_PostPhysics(actor_physics_t *pPhys);
void __cdecl AIPhys_Footsteps(actor_physics_t *pPhys);
colgeom_visitor_inlined_t<200> *__thiscall colgeom_visitor_inlined_t<200>::colgeom_visitor_inlined_t<200>(
        colgeom_visitor_inlined_t<200> *this);
