#include "bg_dog.h"

int __cdecl BG_Actor_GetAnimStateIndex(unsigned __int16 animStateName)
{
  char *v2; // eax
  const char *v3; // eax
  int i; // [esp+0h] [ebp-4h]

  for ( i = 0; i < 47; ++i )
  {
    if ( *s_animStateNames[i].name == animStateName )
      return i;
  }
  if ( i != 47 )
  {
    v2 = SL_ConvertToString(animStateName, SCRIPTINSTANCE_SERVER);
    v3 = va("Actor_GetAnimStateIndex: Could not find anim state %s", v2);
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_dog.cpp",
            104,
            0,
            "%s\n\t%s",
            "i == ACTOR_ANIMATION_COUNT",
            v3) )
      __debugbreak();
  }
  return 0;
}

char *__cdecl BG_Actor_GetAnimStateName(int animStateIndex)
{
  if ( (animStateIndex & 0xFFFFFFBF) >= 0x2F
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_dog.cpp",
          125,
          0,
          "animStateIndex & ~ANIM_STATE_TOGGLEBIT doesn't index ARRAY_COUNT( s_animStateNames )\n\t%i not in [0, %i)",
          animStateIndex & 0xFFFFFFBF,
          47) )
  {
    __debugbreak();
  }
  return SL_ConvertToString(*s_animStateNames[animStateIndex & 0xFFFFFFBF].name, SCRIPTINSTANCE_SERVER);
}

char *__cdecl BG_Actor_GetAnimStateCategoryName(int animStateIndex)
{
  if ( (animStateIndex & 0xFFFFFFBF) >= 0x2F
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_dog.cpp",
          132,
          0,
          "animStateIndex & ~ANIM_STATE_TOGGLEBIT doesn't index ARRAY_COUNT( s_animStateNames )\n\t%i not in [0, %i)",
          animStateIndex & 0xFFFFFFBF,
          47) )
  {
    __debugbreak();
  }
  return SL_ConvertToString(
           *s_animStateCategories[s_animStateNames[animStateIndex & 0xFFFFFFBF].category],
           SCRIPTINSTANCE_SERVER);
}

void __cdecl Dog_RegisterDvars()
{
  dog_debug = _Dvar_RegisterBool("dog_debug", 1, 0x180u, "MP dog debugging");
  dog_turn90_angle = _Dvar_RegisterFloat(
                       "dog_turn90_angle",
                       70.0,
                       0.0,
                       360.0,
                       0x80u,
                       "If the dog needs to turn at this angle or greater, he'll execute the 90-degree turn animation");
  dog_turn180_angle = _Dvar_RegisterFloat(
                        "dog_turn180_angle",
                        135.0,
                        0.0,
                        360.0,
                        0x80u,
                        "If the dog needs to turn at this angle or greater, he'll execute the 180-degree turn animation");
  dog_turn_min_goal_dist = _Dvar_RegisterFloat(
                             "dog_turn_min_goal_dist",
                             40.0,
                             0.0,
                             1024.0,
                             0x80u,
                             "If the dog is at this distance or less to it's goal, it won't execute any turn animations");
}

void __cdecl BG_Dog_UpdateAnimationState(int localClientNum, const entityState_s *es, actorInfo_t *actorInfo)
{
  animInfo_t *lf; // [esp+8h] [ebp-Ch]
  int newAnimationState; // [esp+10h] [ebp-4h]

  lf = &actorInfo->animInfo;
  newAnimationState = es->un2.animState.state;
  if ( ApplyNewAnimState(actorInfo->pXAnimTree, &actorInfo->animInfo.lerp, newAnimationState) )
  {
    BG_Dog_SetNewAnimationState(localClientNum, &actorInfo->animInfo, &lf->lerp, newAnimationState, es);
  }
  else if ( lf->lerp.bAnimating
         && (*(int (__cdecl **)(int, int))(*(unsigned int *)(*((unsigned int *)NtCurrentTeb()->ThreadLocalStoragePointer + _tls_index)
                                                     + 8)
                                         + 160))(
              es->number,
              localClientNum) )
  {
    if ( dword_C6011C[4 * (newAnimationState & 0xFFFFFFBF)] )
      ((void (__cdecl *)(int, const entityState_s *))dword_C6011C[4 * (newAnimationState & 0xFFFFFFBF)])(
        localClientNum,
        es);
  }
}

void __cdecl BG_Dog_SetNewAnimationState(
        int localClientNum,
        animInfo_t *animInfo,
        actorLerpFrame_t *lf,
        int newAnimationState,
        const entityState_s *es)
{
  BG_CheckThread();
  if ( !*(unsigned int *)(*((unsigned int *)NtCurrentTeb()->ThreadLocalStoragePointer + _tls_index) + 8)
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_dog.cpp", 172, 0, "%s", "bgs") )
  {
    __debugbreak();
  }
  if ( AIAnimationFuncTable[newAnimationState & 0xFFFFFFBF].pfnStart )
  {
    AIAnimationFuncTable[newAnimationState & 0xFFFFFFBF].pfnStart(
      localClientNum,
      es,
      (ActorAnimStates)(lf->animationState & 0xFFFFFFBF));
    lf->bAnimating = 1;
    lf->animationState = newAnimationState;
  }
}

bool __cdecl ApplyNewAnimState(XAnimTree_s *pAnimTree, actorLerpFrame_t *lf, int newAnimationState)
{
  if ( newAnimationState != lf->animationState )
    return 1;
  if ( (newAnimationState & 0xFFFFFFBF) == 0 )
    return 0;
  if ( !lf->bAnimating )
    return 1;
  return pAnimTree && !XAnimTreeHasInfo(pAnimTree);
}

void __cdecl BG_Dog_SetAnimState(entityState_s *es, int animState)
{
  es->un2.animState.state = animState | es->un2.animState.state & 0x40 ^ 0x40;
}

