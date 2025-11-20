#pragma once

void __fastcall Actor_InitAnim(actor_s *self);
bool __fastcall Actor_IsAnimScriptAlive(actor_s *self);
void __fastcall Actor_KillAnimScript(actor_s *self);
void __fastcall Actor_SetAnimScript(
        actor_s *self,
        scr_animscript_t *pAnimScriptFunc,
        unsigned __int8 moveMode,
        ai_animmode_t animMode,
        scriptAnimAIFunctionTypes_t animScript);
void __fastcall Actor_AnimStop(actor_s *self, scr_animscript_t *pAnimScriptFunc);
void __fastcall Actor_AnimMoveAway(actor_s *self, scr_animscript_t *pAnimScriptFunc);
scr_animscript_t *__fastcall Actor_GetStopAnim(actor_s *self);
void __fastcall Actor_AnimTryWalk(actor_s *self);
void __fastcall Actor_AnimTryRun(actor_s *self);
void __fastcall Actor_AnimPain(actor_s *self);
void __fastcall Actor_AnimDeath(actor_s *self);
void __fastcall Actor_AnimSpecific(actor_s *self, scr_animscript_t *func, ai_animmode_t eAnimMode, bool bUseGoalWeight);
void __stdcall Actor_AnimSetCompleteGoalWeight(
        XAnimTree_s *tree,
        unsigned int animIndex,
        float goalWeight,
        float goalTime,
        float rate,
        unsigned int notifyName,
        unsigned int notifyType,
        int bRestart);
void __stdcall Actor_AnimClearGoalWeight(unsigned int animIndex, float blendTime);
