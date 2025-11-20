#pragma once

Phys_UnitQuaternion *__cdecl Phys_GetQuaternion(Phys_UnitQuaternion *result, const phys_mat44 *M);
void __cdecl nuge::get_ballistic_info(
        rigid_body *const *list_rigid_body,
        int rbodies_count,
        phys_vec3 *center_of_mass,
        phys_vec3 *total_momentum,
        float *total_mass);
void  nuge::apply_ballistic_target(
        float a1@<ebp>,
        rigid_body *const *list_rigid_body,
        int rbodies_count,
        const phys_vec3 *target,
        float *const dist_sq);
void  nuge::calc_velocities(
        float a1@<ebp>,
        const phys_mat44 *mat0,
        const phys_mat44 *mat1,
        float delta_t,
        phys_vec3 *t_vel,
        phys_vec3 *a_vel);
void  nuge::calc_velocities(
        int a1@<ebp>,
        const phys_mat44 *mat0,
        const phys_mat44 *mat1,
        const phys_vec3 *center_offset_loc,
        float delta_t,
        phys_vec3 *t_vel,
        phys_vec3 *a_vel);
void __cdecl nuge::calc_sphere_inertia(float radius, phys_vec3 *unit_inertia, float *volume);
void __cdecl nuge::calc_box_inertia(const phys_vec3 *dim, phys_vec3 *unit_inertia, float *volume);
void __cdecl nuge::calc_bound_sphere(const phys_vec3 *vert_list, int vert_count, float *radius, phys_vec3 *com);
void __cdecl nuge::calc_bound_box(const phys_vec3 *vert_list, int vert_count, phys_vec3 *dim, phys_vec3 *com);
