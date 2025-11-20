#pragma once

void __thiscall phys_transient_allocator::~phys_transient_allocator(phys_transient_allocator *this);
void __cdecl rbint::take_last_step(user_rigid_body *rb);
void __cdecl IPN_merge(rigid_body *dest, rigid_body *source);
void __cdecl rigid_body_island_qsort(rigid_body **list, int list_count);
void __cdecl merge_sort(rigid_body_constraint_contact **list, int list_count);
void __cdecl IPN_verify_rigid_bodies(rigid_body *rb_partition_head);
void __thiscall physics_system::generate_partitions_and_stuff(
        physics_system *this,
        phys_transient_allocator *transient_buffer);
void __thiscall physics_system::time_step(physics_system *this, float outside_delta_t, bool last_step);
void __thiscall physics_system::frame_advance(physics_system *this, float delta_t);
physics_system *__thiscall physics_system::physics_system(physics_system *this);
physics_system *__cdecl physics_system::create_physics_system();
void __cdecl physics_system::initialize();
void __cdecl physics_system::destroy_physics_system(physics_system *psys);
void __cdecl physics_system::shutdown();
void __cdecl IPN_partition_process(const rigid_body_constraint *rbc, int *island_count);
void __thiscall phys_free_list<rigid_body_constraint_contact>::ptr_array_read(
        phys_free_list<rigid_body_constraint_contact> *this,
        rigid_body_constraint_contact **ptr_array,
        int ptr_array_size);
void __thiscall phys_free_list<rigid_body_constraint_contact>::debug_remove(
        phys_free_list<rigid_body_constraint_contact> *this,
        phys_free_list<rigid_body_constraint_contact>::T_internal *T_i);
void __thiscall phys_free_list<user_rigid_body>::debug_remove(
        phys_free_list<user_rigid_body> *this,
        phys_free_list<user_rigid_body>::T_internal *T_i);
void __thiscall phys_free_list<rigid_body>::debug_remove(
        phys_free_list<rigid_body> *this,
        phys_free_list<rigid_body>::T_internal *T_i);
void __cdecl SetIdentity(phys_mat44 *m);
void __thiscall rigid_body::swap_last_position(rigid_body *this);
void __cdecl rbint::prolog_frame_advance(user_rigid_body *rb, float delta_t);
void __cdecl rbint::collision_prolog(user_rigid_body *rb, float delta_t);
void  rbint::take_next_step(int a1@<ebp>, user_rigid_body *rb, float delta_t);
void __cdecl rbint::collision_prolog(rigid_body *const rb, float delta_t);
void __thiscall phys_free_list<rigid_body_constraint_contact>::ptr_array_write(
        phys_free_list<rigid_body_constraint_contact> *this,
        rigid_body_constraint_contact **ptr_array,
        int ptr_array_size);
void __thiscall phys_free_list<user_rigid_body>::remove(
        phys_free_list<user_rigid_body> *this,
        phys_free_list<user_rigid_body>::T_internal *data);
void __thiscall phys_free_list<rigid_body>::remove(
        phys_free_list<rigid_body> *this,
        phys_free_list<rigid_body>::T_internal *data);
void __thiscall phys_free_list<rigid_body_constraint_point>::debug_remove(
        phys_free_list<rigid_body_constraint_point> *this,
        phys_free_list<rigid_body_constraint_point>::T_internal *T_i);
void __thiscall phys_free_list<rigid_body_constraint_wheel>::debug_remove(
        phys_free_list<rigid_body_constraint_wheel> *this,
        phys_free_list<rigid_body_constraint_wheel>::T_internal *T_i);
void __thiscall phys_free_list<rigid_body_constraint_ragdoll>::debug_remove(
        phys_free_list<rigid_body_constraint_ragdoll> *this,
        phys_free_list<rigid_body_constraint_ragdoll>::T_internal *T_i);
void __thiscall phys_free_list<rigid_body_constraint_custom_orientation>::debug_remove(
        phys_free_list<rigid_body_constraint_custom_orientation> *this,
        phys_free_list<rigid_body_constraint_custom_orientation>::T_internal *T_i);
void __thiscall phys_free_list<rigid_body_constraint_custom_path>::debug_remove(
        phys_free_list<rigid_body_constraint_custom_path> *this,
        phys_free_list<rigid_body_constraint_custom_path>::T_internal *T_i);
void __thiscall phys_free_list<rigid_body_constraint_angular_actuator>::debug_remove(
        phys_free_list<rigid_body_constraint_angular_actuator> *this,
        phys_free_list<rigid_body_constraint_angular_actuator>::T_internal *T_i);
void __thiscall phys_free_list<rigid_body_constraint_upright>::debug_remove(
        phys_free_list<rigid_body_constraint_upright> *this,
        phys_free_list<rigid_body_constraint_upright>::T_internal *T_i);
void __thiscall physics_system::validate_member(physics_system *this, environment_rigid_body *rb);
void __thiscall physics_system::solver_priority_sort(physics_system *this, phys_transient_allocator *transient_buffer);
void __thiscall phys_free_list<user_rigid_body>::remove_all(phys_free_list<user_rigid_body> *this);
void __thiscall phys_free_list<rigid_body>::remove_all(phys_free_list<rigid_body> *this);
void __thiscall phys_free_list<rigid_body_constraint_contact>::remove(
        phys_free_list<rigid_body_constraint_contact> *this,
        phys_free_list<rigid_body_constraint_contact>::T_internal *data);
void __thiscall phys_free_list<rigid_body_constraint_contact>::remove_all(
        phys_free_list<rigid_body_constraint_contact> *this);
void __thiscall phys_free_list<rigid_body_constraint_point>::remove(
        phys_free_list<rigid_body_constraint_point> *this,
        phys_free_list<rigid_body_constraint_point>::T_internal *data);
void __thiscall phys_free_list<rigid_body_constraint_wheel>::remove(
        phys_free_list<rigid_body_constraint_wheel> *this,
        phys_free_list<rigid_body_constraint_wheel>::T_internal *data);
void __thiscall phys_free_list<rigid_body_constraint_ragdoll>::remove(
        phys_free_list<rigid_body_constraint_ragdoll> *this,
        phys_free_list<rigid_body_constraint_ragdoll>::T_internal *data);
void __thiscall phys_free_list<rigid_body_constraint_custom_orientation>::remove(
        phys_free_list<rigid_body_constraint_custom_orientation> *this,
        phys_free_list<rigid_body_constraint_custom_orientation>::T_internal *data);
void __thiscall phys_free_list<rigid_body_constraint_custom_path>::remove(
        phys_free_list<rigid_body_constraint_custom_path> *this,
        phys_free_list<rigid_body_constraint_custom_path>::T_internal *data);
void __thiscall phys_free_list<rigid_body_constraint_angular_actuator>::remove(
        phys_free_list<rigid_body_constraint_angular_actuator> *this,
        phys_free_list<rigid_body_constraint_angular_actuator>::T_internal *data);
void __thiscall phys_free_list<rigid_body_constraint_upright>::remove(
        phys_free_list<rigid_body_constraint_upright> *this,
        phys_free_list<rigid_body_constraint_upright>::T_internal *data);
void __thiscall phys_free_list<user_rigid_body>::~phys_free_list<user_rigid_body>(
        phys_free_list<user_rigid_body> *this);
void __thiscall phys_free_list<rigid_body>::~phys_free_list<rigid_body>(phys_free_list<rigid_body> *this);
void __thiscall phys_free_list<rigid_body_constraint_contact>::~phys_free_list<rigid_body_constraint_contact>(
        phys_free_list<rigid_body_constraint_contact> *this);
void __thiscall phys_free_list<rigid_body_constraint_point>::remove_all(
        phys_free_list<rigid_body_constraint_point> *this);
void __thiscall phys_free_list<rigid_body_constraint_ragdoll>::remove_all(
        phys_free_list<rigid_body_constraint_ragdoll> *this);
void __thiscall phys_free_list<rigid_body_constraint_wheel>::remove_all(
        phys_free_list<rigid_body_constraint_wheel> *this);
void __thiscall phys_free_list<rigid_body_constraint_custom_orientation>::remove_all(
        phys_free_list<rigid_body_constraint_custom_orientation> *this);
void __thiscall phys_free_list<rigid_body_constraint_custom_path>::remove_all(
        phys_free_list<rigid_body_constraint_custom_path> *this);
void __thiscall phys_free_list<rigid_body_constraint_angular_actuator>::remove_all(
        phys_free_list<rigid_body_constraint_angular_actuator> *this);
void __thiscall phys_free_list<rigid_body_constraint_upright>::remove_all(
        phys_free_list<rigid_body_constraint_upright> *this);
void __thiscall phys_free_list<rigid_body_constraint_point>::~phys_free_list<rigid_body_constraint_point>(
        phys_free_list<rigid_body_constraint_point> *this);
void __thiscall phys_free_list<rigid_body_constraint_wheel>::~phys_free_list<rigid_body_constraint_wheel>(
        phys_free_list<rigid_body_constraint_wheel> *this);
void __thiscall phys_free_list<rigid_body_constraint_ragdoll>::~phys_free_list<rigid_body_constraint_ragdoll>(
        phys_free_list<rigid_body_constraint_ragdoll> *this);
void __thiscall phys_free_list<rigid_body_constraint_custom_orientation>::~phys_free_list<rigid_body_constraint_custom_orientation>(
        phys_free_list<rigid_body_constraint_custom_orientation> *this);
void __thiscall phys_free_list<rigid_body_constraint_custom_path>::~phys_free_list<rigid_body_constraint_custom_path>(
        phys_free_list<rigid_body_constraint_custom_path> *this);
void __thiscall phys_free_list<rigid_body_constraint_angular_actuator>::~phys_free_list<rigid_body_constraint_angular_actuator>(
        phys_free_list<rigid_body_constraint_angular_actuator> *this);
void __thiscall phys_free_list<rigid_body_constraint_upright>::~phys_free_list<rigid_body_constraint_upright>(
        phys_free_list<rigid_body_constraint_upright> *this);
void __thiscall physics_system::~physics_system(physics_system *this);
void __thiscall phys_free_list<rigid_body_constraint_distance>::remove_all(phys_free_list<broad_phase_group> *this);
