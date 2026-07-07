#pragma once
#include "actor.h"

bool __fastcall Actor_React_Start(actor_s *self, ai_state_t ePrevState);
void __fastcall Actor_React_Finish(actor_s *self, ai_state_t eNextState);
actor_think_result_t __fastcall Actor_React_Think(actor_s *self);
