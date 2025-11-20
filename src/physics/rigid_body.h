#pragma once

void __thiscall rigid_body::add_force(rigid_body *this, const phys_vec3 *force);
void __thiscall rigid_body::set_inertia(rigid_body *this, const phys_vec3 *inertia);
void __thiscall rigid_body::set_mass(rigid_body *this, float mass);
void __thiscall user_rigid_body::setPosition(user_rigid_body *this, const phys_mat44 *const dictator);
void __thiscall rigid_body::set(
        rigid_body *this,
        float mass,
        const phys_vec3 *inertia,
        const phys_mat44 *mat,
        const phys_vec3 *t_vel,
        const phys_vec3 *a_vel,
        int stable_min_contact_count);
void __thiscall rigid_body::add_force(
        rigid_body *this,
        const phys_vec3 *force,
        const phys_vec3 *point,
        float torque_mult);
void __thiscall user_rigid_body::set(user_rigid_body *this, const phys_mat44 *const dictator);
void __thiscall environment_rigid_body::set(environment_rigid_body *this);
