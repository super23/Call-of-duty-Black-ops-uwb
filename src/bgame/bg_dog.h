#pragma once

int __cdecl BG_Actor_GetAnimStateIndex(unsigned __int16 animStateName);
char *__cdecl BG_Actor_GetAnimStateName(int animStateIndex);
char *__cdecl BG_Actor_GetAnimStateCategoryName(int animStateIndex);
void __cdecl Dog_RegisterDvars();
void __cdecl BG_Dog_UpdateAnimationState(int localClientNum, const entityState_s *es, actorInfo_t *actorInfo);
void __cdecl BG_Dog_SetNewAnimationState(
        int localClientNum,
        animInfo_t *animInfo,
        actorLerpFrame_t *lf,
        int newAnimationState,
        const entityState_s *es);
bool __cdecl ApplyNewAnimState(XAnimTree_s *pAnimTree, actorLerpFrame_t *lf, int newAnimationState);
void __cdecl BG_Dog_SetAnimState(entityState_s *es, int animState);
