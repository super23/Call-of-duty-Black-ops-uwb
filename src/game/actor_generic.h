#pragma once

void __fastcall Actor_Generic_Suspend(actor_s *self, ai_state_t eNextState);
int __fastcall Actor_Generic_Resume(actor_s *self, ai_state_t ePrevState);
void __stdcall Actor_Generic_Pain(int iDamage, const float *vPoint, int iMod, const float *vDir, hitLocation_t hitLoc);
void __fastcall Actor_Generic_Touch(actor_s *self, gentity_s *pOther);
