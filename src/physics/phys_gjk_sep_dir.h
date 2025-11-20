#pragma once

const phys_vec3 * gjk_sep_dir::comp_sep_dir@<eax>(
        int a1@<ebp>,
        float *a2@<edi>,
        float *a3@<esi>,
        const phys_vec3 *result,
        const phys_gjk_input *m_pcd,
        phys_gjk_info *m_gjk_info);
double __cdecl gjk_sep_dir::dist_seg(
        const phys_vec3 *v0,
        const phys_vec3 *v1,
        const phys_vec3 *mesh_center,
        phys_vec3 *direction);
