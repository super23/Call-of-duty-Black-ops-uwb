#pragma once

const phys_vec3 *__cdecl phys_v2_to_v3_multiply(const phys_vec3 *result, const phys_mat44 *m, const phys_vec2 *v);
void  displace_contact_p(
        int a1@<ebp>,
        contact_manifold_mesh_point **mp,
        const phys_vec2 *d,
        const phys_mat44 *contact_mat);
double __thiscall phys_contact_manifold::compute_convex_poly_area(phys_contact_manifold *this);
double __thiscall phys_contact_manifold::compute_convex_poly_perimeter(phys_contact_manifold *this);
void __cdecl feature_qsort(contact_manifold_mesh_point **list, int list_count);
void __cdecl convex_poly_qsort(contact_manifold_mesh_point **list, int list_count);
void __thiscall phys_contact_manifold::generate_convex_poly_internal(phys_contact_manifold *this);
void __thiscall phys_contact_manifold_process::intersect_poly_segment(
        phys_contact_manifold_process *this,
        phys_contact_manifold *cman,
        const phys_vec2 *p0,
        const phys_vec2 *p1);
bool __thiscall phys_contact_manifold_process::find_bottom(
        phys_contact_manifold_process *this,
        phys_contact_manifold_process::bridge *b,
        contact_manifold_mesh_point **left_cman,
        phys_contact_manifold_process::isect_info *right_cman);
void __thiscall phys_contact_manifold::comp_feature_normal(phys_contact_manifold *this);
void __thiscall phys_contact_manifold::generate_convex_poly(phys_contact_manifold *this, const phys_mat44 *contact_mat);
void __thiscall phys_contact_manifold_process::intersect_poly_poly(phys_contact_manifold_process *this);
bool __cdecl phys_contact_manifold::rht(const phys_vec2 *e1, const phys_vec2 *e2, float min_length2, float min_sin_sq);
void __thiscall phys_contact_manifold_process::isect_info::init(
        phys_contact_manifold_process::isect_info *this,
        phys_contact_manifold *cman);
void __thiscall phys_contact_manifold_process::isect_info::update(phys_contact_manifold_process::isect_info *this);
char *__thiscall phys_memory_heap::fast_align_start(phys_memory_heap *this, int alignment, const char *error_msg);
