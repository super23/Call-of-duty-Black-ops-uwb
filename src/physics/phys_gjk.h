#pragma once

phys_vec3 *__thiscall phys_vec3::operator*=(phys_vec3 *this, float d);
phys_vec3 *__thiscall phys_vec3::operator/=(phys_vec3 *this, const float d);
phys_mat44 *__thiscall phys_mat44::phys_mat44(
        phys_mat44 *this,
        const phys_vec3 *x_,
        const phys_vec3 *y_,
        const phys_vec3 *z_,
        const phys_vec3 *w_);
// local variable allocation has failed, the output may be wrong!
void  phys_full_inv_multiply_mat(
        int a1@<ebp>,
        phys_mat44 *dest,
        const phys_mat44 *left,
        const phys_mat44 *right);
void __cdecl get_simplex(
        phys_gjk_geom *cg1,
        phys_gjk_geom *cg2,
        phys_gjk_cache_info *gjk_ci,
        phys_vec3 *a_verts,
        phys_vec3 *a_inds,
        phys_vec3 *b_verts,
        phys_vec3 *b_inds,
        int *vert_count);
void __cdecl set_simplex(
        phys_gjk_geom *cg1,
        phys_gjk_geom *cg2,
        phys_gjk_cache_info *gjk_ci,
        const phys_vec3 *a_normal,
        const phys_vec3 *b_normal,
        const phys_vec3 *a_inds,
        const phys_vec3 *b_inds,
        int w_set);
int __thiscall phys_gjk_info::gjk_subalgorithm(phys_gjk_info *this, int w_set, int new_index);
int __userpurge phys_gjk_info::seed_simplex@<eax>(phys_gjk_info *this@<ecx>, int a2@<ebp>, int cached_vert_count);
int  backup1@<eax>(float a1@<ebp>, phys_gjk_info *gjk_info, int new_index, bool seed_simplex);
phys_gjk_info::gjk_retval_e __userpurge phys_gjk_info::gjk@<eax>(
        phys_gjk_info *this@<ecx>,
        float a2@<ebp>,
        float *a3@<edi>,
        const phys_vec3 *a4@<esi>,
        const phys_gjk_input *d,
        const phys_vec3 *initial_support_dir,
        bool in_separation_loop);
// local variable allocation has failed, the output may be wrong!
phys_gjk_info::gjk_retval_e __userpurge phys_gjk_info::gjk_ray_cast@<eax>(
        phys_gjk_info *this@<ecx>,
        float a2@<ebp>,
        float *a3@<edi>,
        phys_vec3 *a4@<esi>,
        const phys_gjk_input *d,
        const phys_vec3 *initial_support_dir,
        bool in_separation_loop);
phys_gjk_info::gjk_retval_e __userpurge phys_gjk_info::collide@<eax>(
        phys_gjk_info *this@<ecx>,
        int a2@<ebp>,
        const phys_gjk_input *d);
char __userpurge phys_gjk_info::phys_collide_do_gjk_collide@<al>(
        phys_gjk_info *this@<ecx>,
        int a2@<ebp>,
        const phys_gjk_input *d);
int __thiscall phys_gjk_info::compress_verts(phys_gjk_info *this, char w_set);
void __thiscall phys_gjk_info::comp_closest_points(phys_gjk_info *this, int w_set, phys_vec3 *a, phys_vec3 *b);
// bad sp value at call has been detected, the output may be wrong!
const phys_vec3 *__userpurge phys_gjk_info::get_initial_support_dir@<eax>(
        phys_gjk_info *this@<ecx>,
        int a2@<ebp>,
        phys_vec3 *result,
        const phys_gjk_input *d);
void __thiscall phys_gjk_info::gjk_cache_update_invalid(phys_gjk_info *this, const phys_gjk_input *d);
void __userpurge phys_gjk_info::gjk_cache_update_test_only_valid(
        phys_gjk_info *this@<ecx>,
        int a2@<ebp>,
        const phys_gjk_input *d);
void __userpurge phys_gjk_info::gjk_cache_update_colliding(
        phys_gjk_info *this@<ecx>,
        int a2@<ebp>,
        const phys_gjk_input *d);
void __thiscall phys_gjk_info::gjk_cache_update_test_only_penetrating(phys_gjk_info *this, const phys_gjk_input *d);
const phys_vec3 *__userpurge phys_gjk_geom::support_only@<eax>(
        phys_gjk_geom *this@<ecx>,
        int a2@<ebp>,
        int a3@<esi>,
        const phys_vec3 *result,
        const phys_mat44 *xform,
        const phys_vec3 *v);
// local variable allocation has failed, the output may be wrong!
void __userpurge phys_gjk_info::comp_v(phys_gjk_info *this@<ecx>, float a2@<ebp>, int w_set, phys_vec3 *v);
int __userpurge phys_gjk_info::init_gjk@<eax>(
        phys_gjk_info *this@<ecx>,
        int a2@<ebp>,
        const phys_gjk_input *d,
        const phys_vec3 *initial_support_dir,
        bool in_separation_loop);
