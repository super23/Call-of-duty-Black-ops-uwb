#pragma once

mover_info_t *__cdecl get_mover_info(gentity_s *ent);
mover_info_t *__cdecl create_mover_info(gentity_s *ent);
bool __cdecl entity_is_a_mover(int entnum);
void __userpurge mover_info_t::apply_rotation(mover_info_t *this@<ecx>, int a2@<ebp>, float *v);
void __userpurge mover_info_t::apply_rotation_translation(mover_info_t *this@<ecx>, int a2@<ebp>, float *v);
void __cdecl G_PushEntity(gentity_s *check, gentity_s *pusher, const float *move, const float *amove, mover_info_t *mi);
char __cdecl IsEntityLinkedToMe(gentity_s *self, gentity_s *other);
gentity_s *__cdecl GetEntity(const unsigned __int16 *targetname);
void  CreateConstraint(rigid_body *a1@<ebp>, PhysConstraint *constraint);
void __cdecl DeleteConstraint(phys_free_list<RagdollBody>::T_internal_base *rope_index);
void __cdecl G_MoverTeam(gentity_s *ent, mover_info_t *mi);
void __cdecl G_MoverPush(gentity_s *pusher, mover_info_t *mi);
void __cdecl G_CalcEntityPhysicsPositions(gentity_s *ent);
void __cdecl G_CreatePhysicsObject(gentity_s *ent);
void __cdecl G_UpdatePhysicsPose(gentity_s *ent);
void __cdecl G_MoverTeam_New(gentity_s *ent);
void __thiscall mover_info_t::init(mover_info_t *this, float *origin, const float *angles, bool do_collision);
void __thiscall mover_info_t::init(
        mover_info_t *this,
        float *origin,
        const float *angles,
        float *prev_origin,
        const float *prev_angles,
        bool do_collision);
void __thiscall mover_info_t::update(mover_info_t *this, float *origin, const float *angles);
void __cdecl G_RunMover(gentity_s *ent);
void __cdecl trigger_use_shared(gentity_s *self, SpawnVar *spawnVar);
void __cdecl trigger_use_touch(gentity_s *ent, SpawnVar *spawnVar);
mover_info_t *__thiscall phys_simple_allocator<mover_info_t>::allocate(phys_simple_allocator<mover_info_t> *this);
gentity_s *__cdecl GetEntity(scr_entref_t entref);
