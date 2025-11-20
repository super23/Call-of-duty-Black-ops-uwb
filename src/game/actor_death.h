#pragma once

char __fastcall Actor_Death_Start(actor_s *self, ai_state_t ePrevState);
int __fastcall Actor_Death_Think(actor_s *self);
void __fastcall Actor_Death_Cleanup(actor_s *self);
