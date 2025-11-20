#pragma once

void __fastcall Actor_SetDesiredLookAngles(ai_orient_t *pOrient, float fPitch, float fYaw);
void __fastcall Actor_SetDesiredBodyAngle(ai_orient_t *pOrient, float fAngle);
void __fastcall Actor_SetDesiredAngles(ai_orient_t *pOrient, float fPitch, float fYaw);
void __fastcall Actor_SetLookAngles(actor_s *self, float fPitch, float fYaw);
void __fastcall Actor_SetBodyAngle(actor_s *self, float fAngle);
void __fastcall Actor_ChangeAngles(actor_s *self, float fPitch, float fYaw);
void __fastcall Actor_UpdateLookAngles(actor_s *self);
void __cdecl CalcYaw(float dt, float *yaw, float *yawVeloc, float yawAccel);
void __cdecl StepYaw(float dt, float *yaw, float *yawVeloc, float targetYaw, float targetVeloc);
void __fastcall Actor_UpdateBodyAngle(actor_s *self);
void __fastcall Actor_FaceVector(ai_orient_t *pOrient, const float *v);
void __fastcall Actor_SetAnglesToLikelyEnemyPath(actor_s *self);
char __fastcall Actor_GetAnglesToLikelyEnemyPath(actor_s *self);
void __fastcall Actor_FaceLikelyEnemyPath(actor_s *self, ai_orient_t *pOrient);
void __fastcall Actor_FaceEnemy(actor_s *self, ai_orient_t *pOrient);
void __fastcall Actor_DecideOrientation(actor_s *self);
void __fastcall Actor_FaceMotion(actor_s *self, ai_orient_t *pOrient);
void __fastcall Actor_FaceEnemyOrMotion(actor_s *self, ai_orient_t *pOrient);
void __fastcall Actor_FaceEnemyOrMotionSidestep(actor_s *self, ai_orient_t *pOrient);
void __fastcall Actor_SetOrientMode(actor_s *self, ai_orient_mode_t eMode);
void __fastcall Actor_ClearScriptOrient(actor_s *self);
