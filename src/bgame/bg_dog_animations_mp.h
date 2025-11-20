#pragma once

XAnim_s *__cdecl Dog_GetAnims();
void __cdecl Dog_CreateAnims(void *(__cdecl *xanimAlloc)(int));
void __cdecl Dog_DebugPrint(const char *msg);
void __cdecl BG_Dog_CreateAnimChildGroup(
        unsigned int index,
        unsigned int childindex,
        const char **animNames,
        int count,
        unsigned int flags);
void __cdecl Dog_ShutdownAnims();
void __cdecl BG_Actor_FastForwardAnimState(int localClientNum, const entityState_s *es, actorInfo_t *actorInfo);
void __cdecl BG_Dog_Look_At(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Move_Start(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Move_Stop(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Move_Run(int localClientNum, const entityState_s *es, ActorAnimStates prevState);
void __cdecl BG_Dog_GetRunAnimWeights(float leanAmount, float *center, float *left, float *right);
void __cdecl BG_Dog_Init_Normal_Look_At(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Clear_Look_At(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Move_Run_Think(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Move_Walk(int localClientNum, const entityState_s *es, ActorAnimStates prevState);
void __cdecl BG_Dog_Move_Walk_Think(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Move_Turn_Left(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Clear_All_Turns(DObj *obj);
void __cdecl BG_Dog_Move_Turn_Right(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Move_Run_Turn_Left(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Move_Run_Turn_Right(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Move_Turn_Around_Left(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Move_Turn_Around_Right(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Move_Run_Turn_Around_Left(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Move_Run_Turn_Around_Right(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Stop_Idle(int localClientNum, const entityState_s *es, ActorAnimStates prevState);
void __cdecl BG_Dog_Stop_Init(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Init_Attack_Look_At(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Stop_AttackIdle(int localClientNum, const entityState_s *es, ActorAnimStates prevState);
void __cdecl BG_Dog_Stop_AttackIdle_Bark(int localClientNum, const entityState_s *es, ActorAnimStates prevState);
void __cdecl BG_Dog_Stop_AttackIdle_Growl(int localClientNum, const entityState_s *es, ActorAnimStates prevState);
void __cdecl BG_Dog_Pain_Main(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Pain_Front(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Pain_Back(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Pain_Left(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Pain_Right(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Run_Pain_Front(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Run_Pain_Back(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Run_Pain_Left(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Run_Pain_Right(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Death_Front(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Death_Back(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Death_Left(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Death_Right(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Death_Fast_Forward_Front(int localClientNum, const entityState_s *es, actorInfo_t *actorInfo);
void __cdecl BG_Dog_FastForwardAnim(unsigned int anim, actorInfo_t *actorInfo);
void __cdecl BG_Dog_Death_Fast_Forward_Back(int localClientNum, const entityState_s *es, actorInfo_t *actorInfo);
void __cdecl BG_Dog_Death_Fast_Forward_Left(int localClientNum, const entityState_s *es, actorInfo_t *actorInfo);
void __cdecl BG_Dog_Death_Fast_Forward_Right(int localClientNum, const entityState_s *es, actorInfo_t *actorInfo);
void __cdecl BG_Dog_Flashed(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Combat_AttackIdle(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Combat_AttackIdle_Bark(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Combat_AttackIdle_Growl(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Combat_Attack_Run(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Combat_Attack_Player_Close_Range(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Combat_Attack_Miss(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Combat_Attack_Miss_Left(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Combat_Attack_Miss_Right(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Clear_Traverse_Anims(int localClientNum, const entityState_s *es, ActorAnimStates prevState);
void __cdecl BG_Dog_Traverse_Jump_Up_40(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Traverse_Jump_Up_80(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Traverse_Jump_Down_40(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Traverse_Jump_Down_80(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Traverse_Wallhop(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Traverse_Window(int localClientNum, const entityState_s *es);
void __cdecl BG_Dog_Traverse_Through_Hole_42(int localClientNum, const entityState_s *es);
