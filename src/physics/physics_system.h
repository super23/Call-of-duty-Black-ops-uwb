#pragma once

rigid_body_pair_key *__thiscall rigid_body_pair_key::rigid_body_pair_key(
        rigid_body_pair_key *this,
        rigid_body *const b1,
        rigid_body *const b2);
environment_rigid_body *__cdecl phys_sys::get_environment_rigid_body();
void __cdecl phys_sys::set_max_delta_t(float max_delta_t);
double __cdecl phys_sys::get_max_delta_t();
void __cdecl phys_sys::set_v_tol(int max_v_iters);
void __cdecl phys_sys::get_v_tol(int *max_v_iters);
void __cdecl phys_sys::set_vp_tol(int max_vp_iters);
void __cdecl phys_sys::get_vp_tol(int *max_vp_iters);
void __cdecl phys_sys::set_collision_callback(void (__cdecl *collision_callback)());
void __cdecl phys_sys::phys_frame_advance(float delta_t);
// attributes: thunk
void __cdecl phys_sys::phys_init();
// attributes: thunk
void __cdecl phys_sys::phys_shutdown();
int __cdecl phys_sys::get_rigid_body_alloc_count();
int __cdecl phys_sys::get_user_rigid_body_alloc_count();
int __cdecl phys_sys::get_rbc_point_alloc_count();
int __cdecl phys_sys::get_rbc_hinge_alloc_count();
int __cdecl phys_sys::get_rbc_dist_alloc_count();
int __cdecl phys_sys::get_rbc_ragdoll_alloc_count();
int __cdecl phys_sys::get_rbc_wheel_alloc_count();
int __cdecl phys_sys::get_rbc_angular_actuator_alloc_count();
int __cdecl phys_sys::get_rbc_upright_alloc_count();
int __cdecl phys_sys::get_rbc_custom_orientation_alloc_count();
int __cdecl phys_sys::get_rbc_custom_path_alloc_count();
int __cdecl phys_sys::get_rbc_contact_alloc_count();
phys_free_list<user_rigid_body>::T_internal_base *__cdecl phys_sys::get_user_rigid_body(
        const phys_mat44 *const dictactor);
rigid_body_constraint_contact *__cdecl phys_sys::get_rbc_contact(
        environment_rigid_body *b1,
        environment_rigid_body *b2);
void __cdecl phys_sys::fixup_wheel_constraints(rigid_body *const rb);
void __cdecl phys_sys::update_constraint_infos();
rigid_body *__cdecl phys_sys::create_rigid_body(int no_error);
rigid_body_constraint_point *__cdecl phys_sys::create_rbc_point(
        environment_rigid_body *b1,
        environment_rigid_body *b2,
        int no_error);
rigid_body_constraint_custom_orientation *__cdecl phys_sys::create_rbc_custom_orientation(
        environment_rigid_body *b1,
        environment_rigid_body *b2,
        int no_error);
rigid_body_constraint_contact *__cdecl phys_sys::create_rbc_contact(
        environment_rigid_body *b1,
        environment_rigid_body *b2,
        int no_error);
void __cdecl phys_sys::destroy(rigid_body_constraint_point *const rbc);
void __cdecl phys_sys::destroy(rigid_body_constraint_hinge *const rbc);
void __cdecl phys_sys::destroy(rigid_body_constraint_distance *const rbc);
void __cdecl phys_sys::destroy(rigid_body_constraint_ragdoll *const rbc);
void __cdecl phys_sys::destroy(rigid_body_constraint_wheel *const rbc);
void __cdecl phys_sys::destroy(rigid_body_constraint_angular_actuator *const rbc);
void __cdecl phys_sys::destroy(rigid_body_constraint_upright *const rbc);
void __cdecl phys_sys::destroy(rigid_body_constraint_custom_orientation *const rbc);
void __cdecl phys_sys::destroy(rigid_body_constraint_custom_path *const rbc);
void __cdecl phys_sys::destroy_all_rbc_point();
void __cdecl phys_sys::destroy_all_rbc_hinge();
void __cdecl phys_sys::destroy_all_rbc_dist();
void __cdecl phys_sys::destroy_all_rbc_ragdoll();
void __cdecl phys_sys::destroy_all_rbc_wheel();
void __cdecl phys_sys::destroy_all_rbc_angular_actuator();
void __cdecl phys_sys::destroy_all_rbc_upright();
void __cdecl phys_sys::destroy_all_rbc_custom_orientation();
void __cdecl phys_sys::destroy_all_rbc_custom_path();
void __cdecl phys_sys::destroy_all_unused_user_rigid_body();
user_rigid_body *__cdecl phys_sys::create_user_rigid_body(int no_error);
rigid_body_constraint_hinge *__cdecl phys_sys::create_rbc_hinge(
        environment_rigid_body *b1,
        environment_rigid_body *b2,
        int no_error);
rigid_body_constraint_distance *__cdecl phys_sys::create_rbc_dist(
        environment_rigid_body *b1,
        environment_rigid_body *b2,
        int no_error);
rigid_body_constraint_ragdoll *__cdecl phys_sys::create_rbc_ragdoll(
        environment_rigid_body *b1,
        environment_rigid_body *b2,
        int no_error);
rigid_body_constraint_wheel *__cdecl phys_sys::create_rbc_wheel(
        environment_rigid_body *b1,
        environment_rigid_body *b2,
        int no_error);
rigid_body_constraint_angular_actuator *__cdecl phys_sys::create_rbc_angular_actuator(
        environment_rigid_body *b1,
        environment_rigid_body *b2,
        int no_error);
rigid_body_constraint_upright *__cdecl phys_sys::create_rbc_upright(
        environment_rigid_body *b1,
        environment_rigid_body *b2,
        int no_error);
rigid_body_constraint_custom_path *__cdecl phys_sys::create_rbc_custom_path(
        environment_rigid_body *b1,
        environment_rigid_body *b2,
        int no_error);
void __cdecl phys_sys::destroy(rigid_body_constraint_contact *const rbc);
void __cdecl phys_sys::destroy_all_rigid_body();
void __cdecl phys_sys::destroy_all_user_rigid_body();
void __cdecl phys_sys::destroy_all_rbc_contact();
void __cdecl phys_sys::destroy_all_constraint(rigid_body *const rb);
void __cdecl phys_sys::destroy_all_constraint_with_user_rigid_body(rigid_body *const rb);
void __cdecl phys_sys::destroy(rigid_body *const rb);
void __cdecl phys_sys::destroy(user_rigid_body *const rb);
rigid_body_constraint_contact *__thiscall phys_inplace_avl_tree<rigid_body_pair_key,rigid_body_constraint_contact,rigid_body_constraint_contact::avl_tree_accessor>::find(
        phys_inplace_avl_tree<rigid_body_pair_key,rigid_body_constraint_contact,rigid_body_constraint_contact::avl_tree_accessor> *this,
        const rigid_body_pair_key *key);
void __thiscall phys_free_list<rigid_body_constraint_point>::debug_add(
        phys_free_list<rigid_body_constraint_point> *this,
        phys_free_list<rigid_body_constraint_point>::T_internal *T_i);
void __thiscall phys_free_list<rigid_body_constraint_ragdoll>::debug_add(
        phys_free_list<rigid_body_constraint_ragdoll> *this,
        phys_free_list<rigid_body_constraint_ragdoll>::T_internal *T_i);
void __thiscall phys_free_list<rigid_body_constraint_hinge>::debug_add(
        phys_free_list<rigid_body_constraint_wheel> *this,
        phys_free_list<rigid_body_constraint_wheel>::T_internal *T_i);
void __thiscall phys_free_list<rigid_body_constraint_custom_orientation>::debug_add(
        phys_free_list<rigid_body_constraint_custom_orientation> *this,
        phys_free_list<rigid_body_constraint_custom_orientation>::T_internal *T_i);
void __thiscall phys_free_list<rigid_body_constraint_custom_path>::debug_add(
        phys_free_list<rigid_body_constraint_custom_path> *this,
        phys_free_list<rigid_body_constraint_custom_path>::T_internal *T_i);
void __thiscall phys_free_list<rigid_body_constraint_angular_actuator>::debug_add(
        phys_free_list<rigid_body_constraint_angular_actuator> *this,
        phys_free_list<rigid_body_constraint_angular_actuator>::T_internal *T_i);
void __thiscall phys_free_list<rigid_body_constraint_upright>::debug_add(
        phys_free_list<rigid_body_constraint_upright> *this,
        phys_free_list<rigid_body_constraint_upright>::T_internal *T_i);
void __thiscall phys_free_list<user_rigid_body>::debug_add(
        phys_free_list<user_rigid_body> *this,
        phys_free_list<user_rigid_body>::T_internal *T_i);
void __thiscall phys_free_list<rigid_body>::debug_add(
        phys_free_list<rigid_body> *this,
        phys_free_list<rigid_body>::T_internal *T_i);
void __thiscall phys_free_list<rigid_body_constraint_contact>::debug_add(
        phys_free_list<rigid_body_constraint_contact> *this,
        phys_free_list<rigid_body_constraint_contact>::T_internal *T_i);
rigid_body_constraint_point *__thiscall phys_free_list<rigid_body_constraint_point>::add(
        phys_free_list<rigid_body_constraint_point> *this,
        int no_error,
        const char *error_msg);
rigid_body_constraint_custom_orientation *__thiscall phys_free_list<rigid_body_constraint_custom_orientation>::add(
        phys_free_list<rigid_body_constraint_custom_orientation> *this,
        int no_error,
        const char *error_msg);
void __thiscall phys_inplace_avl_tree<rigid_body_pair_key,rigid_body_constraint_contact,rigid_body_constraint_contact::avl_tree_accessor>::add(
        phys_inplace_avl_tree<rigid_body_pair_key,rigid_body_constraint_contact,rigid_body_constraint_contact::avl_tree_accessor> *this,
        const rigid_body_pair_key *key,
        rigid_body_constraint_contact *data);
rigid_body *__thiscall phys_free_list<rigid_body>::add(
        phys_free_list<rigid_body> *this,
        int no_error,
        const char *error_msg);
rigid_body_constraint_hinge *__thiscall phys_free_list<rigid_body_constraint_hinge>::add(
        phys_free_list<rigid_body_constraint_hinge> *this,
        int no_error,
        const char *error_msg);
rigid_body_constraint_distance *__thiscall phys_free_list<rigid_body_constraint_distance>::add(
        phys_free_list<rigid_body_constraint_distance> *this,
        int no_error,
        const char *error_msg);
rigid_body_constraint_ragdoll *__thiscall phys_free_list<rigid_body_constraint_ragdoll>::add(
        phys_free_list<rigid_body_constraint_ragdoll> *this,
        int no_error,
        const char *error_msg);
rigid_body_constraint_wheel *__thiscall phys_free_list<rigid_body_constraint_wheel>::add(
        phys_free_list<rigid_body_constraint_wheel> *this,
        int no_error,
        const char *error_msg);
rigid_body_constraint_custom_path *__thiscall phys_free_list<rigid_body_constraint_custom_path>::add(
        phys_free_list<rigid_body_constraint_custom_path> *this,
        int no_error,
        const char *error_msg);
rigid_body_constraint_angular_actuator *__thiscall phys_free_list<rigid_body_constraint_angular_actuator>::add(
        phys_free_list<rigid_body_constraint_angular_actuator> *this,
        int no_error,
        const char *error_msg);
rigid_body_constraint_upright *__thiscall phys_free_list<rigid_body_constraint_upright>::add(
        phys_free_list<rigid_body_constraint_upright> *this,
        int no_error,
        const char *error_msg);
rigid_body_constraint_contact *__cdecl create(
        phys_free_list<rigid_body_constraint_contact> *__formal,
        rigid_body_constraint_contact *a2,
        environment_rigid_body *b1,
        environment_rigid_body *b2,
        int no_error,
        const char *error_msg);
user_rigid_body *__thiscall phys_free_list<user_rigid_body>::add(
        phys_free_list<user_rigid_body> *this,
        int no_error,
        const char *error_msg);
