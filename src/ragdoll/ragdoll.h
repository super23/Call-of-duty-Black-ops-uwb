#pragma once

void __thiscall phys_free_list<RagdollBody>::remove(
        phys_free_list<RagdollBody> *this,
        phys_free_list<RagdollBody>::T_internal *data);
void __cdecl Ragdoll_DebugDraw();
bool __cdecl Ragdoll_BodyInUse(RagdollBody *body);
RagdollDef *__cdecl Ragdoll_BodyDef(RagdollBody *body);
DObj *__cdecl Ragdoll_BodyDObj(RagdollBody *body);
centity_s *__cdecl Ragdoll_BodyPose(RagdollBody *body);
void __cdecl Ragdoll_BodyRootOrigin(RagdollBody *body, float *origin);
bool __cdecl Ragdoll_BodyPoseValid(RagdollBody *body);
void __cdecl Ragdoll_GetRootOrigin(const cpose_t *ragdollHandle, float *origin);
int __cdecl Ragdoll_CountPhysicsBodies();
bool __cdecl Ragdoll_BodyHasPhysics(RagdollBody *body);
const cpose_t *__cdecl Ragdoll_CreateRagdollForDObj(
        int localClientNum,
        int ragdollDef,
        phys_free_list<RagdollBody>::T_internal_base *dobj,
        bool reset,
        bool share);
RagdollBody *__cdecl Ragdoll_GetUnusedBody();
void __cdecl Ragdoll_InitBody(RagdollBody *body);
phys_free_list<RagdollBody>::T_internal_base *__cdecl Ragdoll_ReferenceDObjBody(
        phys_free_list<RagdollBody>::T_internal_base *dobj);
char __cdecl Ragdoll_BindDef(unsigned int ragdollDef);
bool __cdecl Ragdoll_ValidateDef(unsigned int ragdollDef);
void __cdecl Ragdoll_Remove(const cpose_t *ragdoll);
void __cdecl Ragdoll_FreeBody(const cpose_t *ragdollBody);
void __cdecl Ragdoll_InitDvars();
void __cdecl Ragdoll_Register();
void __cdecl Ragdoll_Init();
void __cdecl Ragdoll_Shutdown();
RagdollBody *__thiscall phys_free_list<RagdollBody>::add(
        phys_free_list<RagdollBody> *this,
        int no_error,
        const char *error_msg);
void __thiscall phys_free_list<RagdollBody>::remove(phys_free_list<RagdollBody> *this, RagdollBody *data_);
void __thiscall phys_free_list<RagdollBody>::debug_add(
        phys_free_list<RagdollBody> *this,
        phys_free_list<RagdollBody>::T_internal *T_i);
