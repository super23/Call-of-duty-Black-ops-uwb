#pragma once

void __cdecl orthonormalize(phys_mat44 *mat);
void __thiscall pulse_sum_normal::set_object_vel(pulse_sum_normal *this, const phys_vec3 *object_vel);
void __thiscall pulse_sum_normal::set_object_col_pt(pulse_sum_normal *this, const phys_vec3 *object_col_pt);
void __thiscall pulse_sum_point::set_object_vel(pulse_sum_point *this, const phys_vec3 *object_vel);
void __thiscall pulse_sum_angular::set_object_vel(pulse_sum_angular *this, const phys_vec3 *object_vel);
void __thiscall pulse_sum_angular::set_object_col_pt(pulse_sum_angular *this, const phys_vec3 *object_col_pt);
rigid_body *__thiscall rigid_body::operator=(rigid_body *this, const rigid_body *__that);
void __cdecl nuge::tensor_transform_principle(const phys_vec3 *diag, const phys_mat44 *mat, phys_mat44 *tensor);
const phys_vec3 * rbint::inv_L@<eax>(
        int a1@<ebp>,
        const phys_vec3 *result,
        const rigid_body *rb,
        const phys_vec3 *t,
        float delta_t);
void __cdecl rbint::update_stability(rigid_body *const rb, float delta_t);
const phys_vec3 *__cdecl rbint::gtv(const phys_vec3 *result, rigid_body *const b, const phys_vec3 *r);
void __userpurge pulse_sum_normal::calc_abs(pulse_sum_normal *this@<ecx>, int a2@<ebp>, const phys_vec3 *b1_r_displace);
double  pulse_sum_normal::get_vel@<st0>(pulse_sum_normal *this@<ecx>, int a2@<ebp>);
double  pulse_sum_normal::get_last_vel@<st0>(pulse_sum_normal *this@<ecx>, int a2@<ebp>);
double  pulse_sum_normal::get_pos@<st0>(pulse_sum_normal *this@<ecx>, int a2@<ebp>);
void __userpurge pulse_sum_normal::set(
        pulse_sum_normal *this@<ecx>,
        int a2@<ebp>,
        rigid_body *const b1,
        const phys_vec3 *b1_r,
        rigid_body *const b2,
        const phys_vec3 *b2_r,
        const phys_vec3 *ud,
        pulse_sum_cache *const ps_cache,
        const phys_vec3 *b1_r_displace);
const phys_vec3 *__thiscall pulse_sum_normal::get_relative_velocity_change_dir(
        pulse_sum_normal *this,
        const phys_vec3 *result);
const phys_vec3 *__userpurge pulse_sum_normal::get_relative_velocity@<eax>(
        pulse_sum_normal *this@<ecx>,
        int a2@<ebp>,
        const phys_vec3 *result);
double __thiscall pulse_sum_normal::get_unclamped_pulse_sum(pulse_sum_normal *this);
const phys_vec3 *__userpurge pulse_sum_point::get_vel@<eax>(
        pulse_sum_point *this@<ecx>,
        int a2@<ebp>,
        const phys_vec3 *result);
const phys_vec3 *__userpurge pulse_sum_point::get_pos@<eax>(
        pulse_sum_point *this@<ecx>,
        int a2@<ebp>,
        const phys_vec3 *result);
double __thiscall pulse_sum_angular::get_vel(pulse_sum_angular *this);
double __thiscall pulse_sum_angular::get_pos(pulse_sum_angular *this);
void __userpurge pulse_sum_angular::set(
        pulse_sum_angular *this@<ecx>,
        int a2@<ebp>,
        rigid_body *const b1,
        const phys_vec3 *b1_r,
        rigid_body *const b2,
        const phys_vec3 *b2_r,
        const phys_vec3 *ud,
        pulse_sum_cache *const ps_cache);
const phys_vec3 *__userpurge pulse_sum_contact_point::get_vel@<eax>(
        pulse_sum_contact_point *this@<ecx>,
        float a2@<ebp>,
        const phys_vec3 *result,
        pulse_sum_contact *psc);
double __userpurge pulse_sum_contact_point::get_pos@<st0>(
        pulse_sum_contact_point *this@<ecx>,
        int a2@<ebp>,
        pulse_sum_contact *psc);
void __userpurge pulse_sum_point::set(
        pulse_sum_point *this@<ecx>,
        int a2@<ebp>,
        rigid_body *const b1,
        const phys_vec3 *b1_r,
        rigid_body *const b2,
        const phys_vec3 *b2_r,
        pulse_sum_cache *const ps_cache,
        float delta_t,
        bool is_spring,
        float spring_k,
        float damp_k);
double __userpurge pulse_sum_contact_point::get_impact_vel@<st0>(
        pulse_sum_contact_point *this@<ecx>,
        int a2@<ebp>,
        pulse_sum_contact *psc);
void __userpurge pulse_sum_contact::set(
        pulse_sum_contact *this@<ecx>,
        phys_vec3 *a2@<ebp>,
        rigid_body *const b1,
        rigid_body *const b2,
        contact_point_info *cpi,
        float delta_t);
void __cdecl constraint_solver_process(
        phys_transient_allocator *transient_buffer,
        physics_system *psys,
        float outside_delta_t);
void __thiscall pulse_sum_constraint_solver::solve_iterative(
        pulse_sum_constraint_solver *this,
        int max_iters,
        float max_error_sq);
void __thiscall pulse_sum_constraint_solver::add_urb(
        pulse_sum_constraint_solver *this,
        phys_inplace_avl_tree<user_rigid_body *,pulse_sum_constraint_solver::temp_user_rigid_body,pulse_sum_constraint_solver::temp_user_rigid_body::avl_tree_accessor> *turb_search_tree,
        phys_simple_link_list<pulse_sum_constraint_solver::temp_user_rigid_body> *list_turb,
        phys_simple_link_list<pulse_sum_constraint_solver::user_rigid_body_restore_info> *list_urbri,
        rigid_body_constraint *rbc);
int __cdecl phys_jq_constraint_solver_batch_function(jqBatch *pBatch);
double __thiscall pulse_sum_normal::get_objective(pulse_sum_normal *this);
void __thiscall pulse_sum_normal::apply(pulse_sum_normal *this, const float *s_);
double __thiscall pulse_sum_normal::clamp_pulse_sum(pulse_sum_normal *this, float ps);
void __thiscall pulse_sum_normal::SOLVER_apply_relaxation(
        pulse_sum_normal *this,
        float *error_sq,
        const bool add_error);
void __thiscall pulse_sum_normal::SOLVER_solver_prolog(pulse_sum_normal *this, float delta_t);
const phys_vec3 *__thiscall pulse_sum_point::get_objective(pulse_sum_point *this, const phys_vec3 *result);
void __thiscall pulse_sum_point::apply(pulse_sum_point *this, const phys_vec3 *s_);
void __thiscall pulse_sum_point::SOLVER_solver_intermediate(pulse_sum_point *this, float delta_t);
void __userpurge pulse_sum_point::SOLVER_apply_relaxation(pulse_sum_point *this@<ecx>, int a2@<ebp>, float *error_sq);
void  pulse_sum_angular::calc_abs(pulse_sum_angular *this@<ecx>, int a2@<ebp>);
double __thiscall pulse_sum_angular::get_objective(pulse_sum_angular *this);
void __thiscall pulse_sum_angular::apply(pulse_sum_angular *this, const float *s_);
double __thiscall pulse_sum_angular::clamp_pulse_sum(pulse_sum_angular *this, float ps);
void __thiscall pulse_sum_angular::SOLVER_apply_relaxation(pulse_sum_angular *this, float *error_sq);
void __thiscall pulse_sum_wheel::SOLVER_solver_intermediate(pulse_sum_wheel *this, float delta_t);
void __userpurge pulse_sum_contact_point::calc_abs(
        pulse_sum_contact_point *this@<ecx>,
        int a2@<ebp>,
        pulse_sum_contact *psc);
const phys_vec3 *__thiscall pulse_sum_contact_point::get_objective(
        pulse_sum_contact_point *this,
        const phys_vec3 *result,
        pulse_sum_contact *psc);
void __thiscall pulse_sum_contact_point::apply(
        pulse_sum_contact_point *this,
        pulse_sum_contact *psc,
        const phys_vec3 *s_);
void __cdecl rbint::setup_constraint(rigid_body *rb, pulse_sum_node *psn);
void __thiscall pulse_sum_constraint_solver::~pulse_sum_constraint_solver(pulse_sum_constraint_solver *this);
void __thiscall rigid_body::update_last_position(rigid_body *this);
user_rigid_body *__thiscall user_rigid_body::operator=(user_rigid_body *this, const user_rigid_body *__that);
void  rbint::substep(int a1@<ebp>, user_rigid_body *rb, float delta_t);
void __cdecl rbint::solver_prolog(rigid_body *rb, float delta_t);
// local variable allocation has failed, the output may be wrong!
void  rbint::euler_integrate_velocity(float a1@<ebp>, rigid_body *const rb, float delta_t);
void  rbint::euler_integrate_pos(int a1@<ebp>, rigid_body *const rb, float delta_t);
void __cdecl rbint::add_vel(rigid_body *rb, const phys_vec3 *t, const phys_vec3 *a);
void __thiscall rigid_body_constraint_contact::update_smallest_lambda(rigid_body_constraint_contact *this);
void __userpurge pulse_sum_point::calc_abs_and_setup(
        pulse_sum_point *this@<ecx>,
        int a2@<ebp>,
        float delta_t,
        bool is_spring,
        float spring_k,
        float damp_k);
void __userpurge pulse_sum_point::SOLVER_solver_prolog(pulse_sum_point *this@<ecx>, int a2@<ebp>, float delta_t);
bool __thiscall pulse_sum_wheel::pulse_chain_within_limits(pulse_sum_wheel *this);
char __thiscall pulse_sum_wheel::clamp_pulse_sum_pulse_chain(pulse_sum_wheel *this, float *ps1_, float *ps2_);
void __thiscall pulse_sum_wheel::addp_pulse_chain(pulse_sum_wheel *this);
void __thiscall pulse_sum_wheel::SOLVER_apply_relaxation(pulse_sum_wheel *this, float *error_sq);
void __thiscall pulse_sum_wheel::SOLVER_solver_prolog(pulse_sum_wheel *this, float delta_t);
void __thiscall pulse_sum_contact_point::clamp_f(pulse_sum_contact_point *this, pulse_sum_contact *psc);
void __userpurge pulse_sum_contact_point::SOLVER_solver_prolog(
        pulse_sum_contact_point *this@<ecx>,
        int a2@<ebp>,
        pulse_sum_contact *psc,
        pulse_sum_cache *m_pulse_sum_cache,
        float delta_t);
void __userpurge pulse_sum_contact_point::SOLVER_apply_relaxation(
        pulse_sum_contact_point *this@<ecx>,
        int a2@<ebp>,
        pulse_sum_contact *psc,
        float *error_sq);
void __userpurge pulse_sum_contact_point::setup_vel_uni_restitution(
        pulse_sum_contact_point *this@<ecx>,
        int a2@<ebp>,
        pulse_sum_contact *psc,
        float restitution_k,
        float max_restitution_v,
        float delta_t,
        float max_penalty_restitution_vel,
        float min_restitution_impact_vel_sq);
void __thiscall pulse_sum_contact::SOLVER_solver_intermediate(pulse_sum_contact *this, float delta_t);
pulse_sum_constraint_solver *__thiscall pulse_sum_constraint_solver::pulse_sum_constraint_solver(
        pulse_sum_constraint_solver *this);
void __thiscall pulse_sum_constraint_solver::solve_constraints(
        pulse_sum_constraint_solver *this,
        rigid_body *const head);
void __thiscall pulse_sum_constraint_solver::execute_constraint_solver(
        pulse_sum_constraint_solver *this,
        rigid_body *const head);
