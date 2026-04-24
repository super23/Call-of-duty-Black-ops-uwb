#include "phys_gjk_sep_dir.h"
#include <tl/tl_system.h>
#include <universal/com_math.h>

// m_ kinda implies it's a member, but not here it seems
const int m_list_triangle[60] =
{
  10,
  1,
  4,
  8,
  11,
  6,
  0,
  10,
  4,
  2,
  8,
  6,
  0,
  2,
  10,
  2,
  0,
  8,
  9,
  1,
  10,
  9,
  2,
  6,
  2,
  9,
  10,
  1,
  7,
  4,
  5,
  11,
  8,
  0,
  5,
  8,
  5,
  7,
  11,
  5,
  0,
  4,
  7,
  5,
  4,
  7,
  3,
  11,
  11,
  3,
  6,
  3,
  9,
  6,
  9,
  3,
  1,
  3,
  7,
  1
};

const phys_vec3 m_list_support_dir[12] =
{
  { -0.51530403, 0.44512299, -0.73234397, 0.0 },
  { -0.222601, 0.27597901, 0.93503201, 0.0 },
  { 0.497603, 0.66555297, -0.55626398, 0.0 },
  { 0.515302, -0.445124, 0.73234397, 0.0 },
  { -0.96041399, 0.204353, 0.189327, 0.0 },
  { -0.67850202, -0.56101602, -0.474231, 0.0 },
  { 0.96041399, -0.204353, -0.189327, 0.0 },
  { -0.49760401, -0.66555297, 0.55626398, 0.0 },
  { 0.222601, -0.27597699, -0.93503201, 0.0 },
  { 0.67850399, 0.56101501, 0.474231, 0.0 },
  { -0.23354, 0.96241403, 0.13862801, 0.0 },
  { 0.23354, -0.96241403, -0.13862801, 0.0 }
};


#if 0
const phys_vec3 * gjk_sep_dir::comp_sep_dir(
                phys_vec3 *result,
                const phys_gjk_input *m_pcd,
                phys_gjk_info *m_gjk_info)
{
    int v6; // edi
    const phys_gjk_geom *gjk_cg2; // ecx
    const phys_vec3 *v8; // eax
    double x; // st7
    double v10; // st6
    double y; // st5
    double v12; // st4
    double z; // st3
    double v14; // st2
    const int *v15; // esi
    float *v16; // edx
    float *v17; // ecx
    int v18; // edi
    double v19; // st6
    float *v20; // edi
    float *v21; // eax
    double v22; // st6
    float *v23; // edi
    double v24; // st6
    double v25; // st5
    double v26; // st4
    double v27; // st5
    double v28; // st4
    double v29; // st5
    phys_vec3 *v30; // ecx
    const phys_vec3 *v31; // eax
    float v32; // ecx
    float v33; // edx
    float w; // ecx
    float v35; // ecx
    float *p_y; // [esp-14h] [ebp-2F4h]
    int v39; // [esp-4h] [ebp-2E4h] BYREF
    phys_vec3 m_normal_list[20]; // [esp+0h] [ebp-2E0h] BYREF
    phys_vec3 m_vert_list[12]; // [esp+140h] [ebp-1A0h] BYREF
    phys_vec3 v42; // [esp+204h] [ebp-DCh] BYREF
    int v43; // [esp+214h] [ebp-CCh] BYREF
    float v44; // [esp+224h] [ebp-BCh] BYREF
    float v45; // [esp+228h] [ebp-B8h]
    float v46; // [esp+22Ch] [ebp-B4h]
    phys_vec3 m_b_loc_vert; // [esp+234h] [ebp-ACh] BYREF
    float v48; // [esp+244h] [ebp-9Ch]
    float v49; // [esp+248h] [ebp-98h]
    float v50; // [esp+24Ch] [ebp-94h]
    float v51; // [esp+254h] [ebp-8Ch]
    float v52; // [esp+258h] [ebp-88h]
    float v53; // [esp+25Ch] [ebp-84h]
    phys_vec3 m_b_vert; // [esp+264h] [ebp-7Ch]
    phys_vec3 m_a_vert; // [esp+274h] [ebp-6Ch] BYREF
    phys_vec3 m_mesh_b_center; // [esp+284h] [ebp-5Ch]
    phys_vec3 m_mesh_a_center; // [esp+294h] [ebp-4Ch]
    phys_vec3 *best_normal; // [esp+2ACh] [ebp-34h]
    float best_dist_sq; // [esp+2B0h] [ebp-30h]
    phys_vec3 dir; // [esp+2B4h] [ebp-2Ch]
    float dotp; // [esp+2D0h] [ebp-10h]
    //_UNKNOWN *v62; // [esp+2D4h] [ebp-Ch]
    //const phys_vec3 *resulta; // [esp+2D8h] [ebp-8h]
    //phys_gjk_info *m_gjk_infoa; // [esp+2E0h] [ebp+0h]
    //
    //v62 = a1;
    //resulta = &m_gjk_infoa->cg2_to_cg1_xform.x;
    m_mesh_a_center.x = 0.0;
    m_mesh_a_center.y = 0.0;
    m_mesh_a_center.z = 0.0;
    v6 = 0;
    m_mesh_b_center.x = 0.0;
    m_mesh_b_center.y = 0.0;
    m_mesh_b_center.z = 0.0;
    dir.x = 0.0;
    dir.y = 0.0;
    dir.z = 0.0;
    do
    {
        //((void(__thiscall *)(const phys_gjk_geom *, const phys_vec3 *, phys_vec3 *, int *, float *, phys_vec3 *, float *))m_pcd->gjk_cg1->support)(
        //    m_pcd->gjk_cg1,
        //    &m_list_support_dir[v6],
        //    &m_a_vert,
        //    &v43,
        //    a2,
        //    a3,
        //    p_y);

        m_pcd->gjk_cg1->support(&m_list_support_dir[v6], &m_a_vert, &v43);

        gjk_cg2 = m_pcd->gjk_cg2;
        v48 = -*(float *)(v6 * 16 + 14786112);
        p_y = &m_vert_list[11].y;
        v49 = -*(float *)(v6 * 16 + 14786116);
        a3 = &m_b_loc_vert;
        a2 = &v44;
        v50 = -*(float *)(v6 * 16 + 14786120);
        v44 = m_gjk_info->cg2_to_cg1_xform.x.z * v50
            + m_gjk_info->cg2_to_cg1_xform.x.y * v49
            + v48 * m_gjk_info->cg2_to_cg1_xform.x.x;
        v45 = m_gjk_info->cg2_to_cg1_xform.y.x * v48
            + m_gjk_info->cg2_to_cg1_xform.y.y * v49
            + v50 * m_gjk_info->cg2_to_cg1_xform.y.z;
        v46 = v50 * m_gjk_info->cg2_to_cg1_xform.z.z
            + v49 * m_gjk_info->cg2_to_cg1_xform.z.y
            + v48 * m_gjk_info->cg2_to_cg1_xform.z.x;

        ((void(__thiscall *)(const phys_gjk_geom *))gjk_cg2->support)(gjk_cg2);

        v8 = phys_multiply(&v42, &m_gjk_info->cg2_to_cg1_xform, &m_b_loc_vert);
        ++v6;
        m_b_vert.x = v8->x + m_gjk_info->cg2_to_cg1_xform.w.x;
        m_b_vert.y = v8->y + m_gjk_info->cg2_to_cg1_xform.w.y;
        m_b_vert.z = v8->z + m_gjk_info->cg2_to_cg1_xform.w.z;
        x = m_a_vert.x;
        v10 = m_b_vert.x;
        v51 = m_a_vert.x - m_b_vert.x;
        y = m_a_vert.y;
        v12 = m_b_vert.y;
        v52 = m_a_vert.y - m_b_vert.y;
        z = m_a_vert.z;
        v14 = m_b_vert.z;
        v53 = m_a_vert.z - m_b_vert.z;
        m_normal_list[v6 + 18].y = v51;
        m_normal_list[v6 + 18].z = v52;
        m_normal_list[v6 + 18].w = v53;
        m_mesh_a_center.x = x + m_mesh_a_center.x;
        m_mesh_a_center.y = y + m_mesh_a_center.y;
        m_mesh_a_center.z = z + m_mesh_a_center.z;
        m_mesh_b_center.x = v10 + m_mesh_b_center.x;
        m_mesh_b_center.y = v12 + m_mesh_b_center.y;
        m_mesh_b_center.z = v14 + m_mesh_b_center.z;
        dir.x = m_normal_list[v6 + 18].y + dir.x;
        dir.y = m_normal_list[v6 + 18].z + dir.y;
        dir.z = dir.z + m_normal_list[v6 + 18].w;
    } while (v6 < 12);
    best_normal = 0;
    v15 = m_list_triangle;
    v16 = (float *)&v39;
    m_mesh_a_center.x = m_mesh_a_center.x * 0.0833333358168602;
    m_mesh_a_center.y = m_mesh_a_center.y * 0.0833333358168602;
    m_mesh_a_center.z = m_mesh_a_center.z * 0.0833333358168602;
    m_mesh_b_center.x = m_mesh_b_center.x * 0.0833333358168602;
    m_mesh_b_center.y = m_mesh_b_center.y * 0.0833333358168602;
    m_mesh_b_center.z = m_mesh_b_center.z * 0.0833333358168602;
    dir.x = dir.x * 0.0833333358168602;
    dir.y = dir.y * 0.0833333358168602;
    dir.z = 0.0833333358168602 * dir.z;
    best_dist_sq = 0.0;
    do
    {
        v17 = &m_normal_list[*v15 + 19].y;
        v18 = v15[2];
        v19 = m_normal_list[v18 + 19].y;
        v20 = &m_normal_list[v18 + 19].y;
        v21 = &m_normal_list[v15[1] + 19].y;
        v51 = v19 - *v17;
        v52 = v20[1] - v17[1];
        v22 = v20[2];
        v23 = v16 - 2;
        v53 = v22 - v17[2];
        m_b_vert.x = *v21 - *v17;
        m_b_vert.y = v21[1] - v17[1];
        m_b_vert.z = v21[2] - v17[2];
        v48 = m_b_vert.y * v53 - m_b_vert.z * v52;
        v49 = m_b_vert.z * v51 - v53 * m_b_vert.x;
        v50 = v52 * m_b_vert.x - m_b_vert.y * v51;
        v24 = v48;
        *(v16 - 2) = v48;
        v25 = v49;
        *(v16 - 1) = v49;
        v26 = v50;
        *v16 = v50;
        m_a_vert.x = *v17 - dir.x;
        m_a_vert.y = v17[1] - dir.y;
        m_a_vert.z = v17[2] - dir.z;
        dotp = m_a_vert.x * v24 + m_a_vert.y * v25 + m_a_vert.z * v26;
        if (dotp < 0.0)
        {
            v44 = -v24;
            v45 = -v25;
            v46 = -v26;
            *v23 = v44;
            *(v16 - 1) = v45;
            *v16 = v46;
        }
        dotp = *(v16 - 1) * *(v16 - 1) + *v23 * *v23 + *v16 * *v16;
        v27 = dotp;
        if (dotp < 0.0000000099999991)
        {
            v30 = best_normal;
        }
        else
        {
            m_b_loc_vert.x = -*v17;
            m_b_loc_vert.y = -v17[1];
            m_b_loc_vert.z = -v17[2];
            dotp = m_b_loc_vert.x * *v23 + *(v16 - 1) * m_b_loc_vert.y + *v16 * m_b_loc_vert.z;
            v28 = dotp;
            dotp = dotp * dotp;
            dotp = dotp / v27;
            v29 = dotp;
            if (v28 < 0.0)
                v29 = -v29;
            v30 = best_normal;
            dotp = v29;
            if (!best_normal || best_dist_sq < (double)dotp)
            {
                best_dist_sq = dotp;
                best_normal = (phys_vec3 *)(v16 - 2);
                v30 = (phys_vec3 *)(v16 - 2);
            }
        }
        v15 += 3;
        v16 += 4;
    } while (v15 < (const int *)"bad separation dir.\n");
    if (v30)
    {
        best_dist_sq = v30->y * v30->y + v30->x * v30->x + v30->z * v30->z;
        if (best_dist_sq < 0.0000000099999991)
        {
            if (_tlAssert(
                "source/phys_gjk_sep_dir.cpp",
                258,
                "AbsSquared(*best_normal) >= phys_sqr(GJK_MIN_SUPPORT_DIR_LENGTH)",
                ""))
            {
                __debugbreak();
            }
            v30 = best_normal;
        }
        result->x = -v30->x;
        result->y = -v30->y;
        result->z = -v30->z;
        return result;
    }
    else
    {
        dir.x = m_mesh_a_center.x - m_mesh_b_center.x;
        dir.y = m_mesh_a_center.y - m_mesh_b_center.y;
        dir.z = m_mesh_a_center.z - m_mesh_b_center.z;
        best_dist_sq = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
        if (best_dist_sq < 0.0000000099999991)
        {
            tlWarning("bad separation dir.\n");
            v31 = result;
            v35 = PHYS_X_VEC.y;
            result->x = PHYS_X_VEC.x;
            v33 = PHYS_X_VEC.z;
            result->y = v35;
            w = PHYS_X_VEC.w;
        }
        else
        {
            v31 = result;
            v32 = dir.y;
            result->x = dir.x;
            v33 = dir.z;
            result->y = v32;
            w = dir.w;
        }
        result->z = v33;
        result->w = w;
    }
    return v31;
}
#else // aislop 2
const phys_vec3 *gjk_sep_dir::comp_sep_dir(
    phys_vec3 *result,
    const phys_gjk_input *input,
    phys_gjk_info *info)
{
    // --- Phase 1: Sample support points in 12 directions ---
    // For each support direction, get support point on cg1 (in cg1 local space)
    // and support point on cg2 (transformed into cg1 local space)
    // then compute the Minkowski difference vertex: cg1_vert - cg2_vert_in_cg1_space

    phys_vec3 m_vert_list[12];   // Minkowski difference verts (cg1 - cg2 in cg1 space)
    phys_vec3 m_normal_list[20]; // Face normals computed in phase 2

    phys_vec3 mesh_a_center = { 0, 0, 0, 0 }; // accumulator for cg2 verts in cg1 space
    phys_vec3 mesh_b_center = { 0, 0, 0, 0 }; // accumulator for cg1 verts
    phys_vec3 diff_sum = { 0, 0, 0, 0 }; // accumulator for Minkowski diff verts

    for (int i = 0; i < 12; ++i)
    {
        // Get support point on cg1 in cg1 local space
        phys_vec3 cg1_support_ind;
        phys_vec3 cg1_support_vert;
       
        input->gjk_cg1->support(
            &m_list_support_dir[i],
            &cg1_support_vert,
            &cg1_support_ind);

        iassert(cg1_support_vert.x == cg1_support_vert.x &&
            cg1_support_vert.y == cg1_support_vert.y &&
            cg1_support_vert.z == cg1_support_vert.z); // NaN check

        // Transform support direction into cg2 local space (negate for Minkowski)
        phys_vec3 cg2_dir_local;
        cg2_dir_local.x = -(m_list_support_dir[i].x);
        cg2_dir_local.y = -(m_list_support_dir[i].y);
        cg2_dir_local.z = -(m_list_support_dir[i].z);

        // Transform negated direction into cg2 space using the rotation part of cg2_to_cg1
        phys_vec3 cg2_dir_in_cg2;
        cg2_dir_in_cg2.x = info->cg2_to_cg1_xform.x.x * cg2_dir_local.x
            + info->cg2_to_cg1_xform.x.y * cg2_dir_local.y
            + info->cg2_to_cg1_xform.x.z * cg2_dir_local.z;
        cg2_dir_in_cg2.y = info->cg2_to_cg1_xform.y.x * cg2_dir_local.x
            + info->cg2_to_cg1_xform.y.y * cg2_dir_local.y
            + info->cg2_to_cg1_xform.y.z * cg2_dir_local.z;
        cg2_dir_in_cg2.z = info->cg2_to_cg1_xform.z.x * cg2_dir_local.x
            + info->cg2_to_cg1_xform.z.y * cg2_dir_local.y
            + info->cg2_to_cg1_xform.z.z * cg2_dir_local.z;

        // Get support point on cg2 in cg2 local space
        phys_vec3 cg2_support_ind;
        phys_vec3 cg2_support_vert_local;
        input->gjk_cg2->support(
            &cg2_dir_in_cg2,
            &cg2_support_vert_local,
            &cg2_support_ind);

        iassert(cg2_support_vert_local.x == cg2_support_vert_local.x &&
            cg2_support_vert_local.y == cg2_support_vert_local.y &&
            cg2_support_vert_local.z == cg2_support_vert_local.z); // NaN check

        // Transform cg2 support vert into cg1 space (rotation only)
        phys_vec3 cg2_rotated;
        phys_multiply(&cg2_rotated, &info->cg2_to_cg1_xform, &cg2_support_vert_local);

        // Add translation (w column of cg2_to_cg1)
        phys_vec3 cg2_support_in_cg1;
        cg2_support_in_cg1.x = cg2_rotated.x + info->cg2_to_cg1_xform.w.x;
        cg2_support_in_cg1.y = cg2_rotated.y + info->cg2_to_cg1_xform.w.y;
        cg2_support_in_cg1.z = cg2_rotated.z + info->cg2_to_cg1_xform.w.z;

        // Minkowski difference: cg1_vert - cg2_vert_in_cg1
        m_vert_list[i].x = cg1_support_vert.x - cg2_support_in_cg1.x;
        m_vert_list[i].y = cg1_support_vert.y - cg2_support_in_cg1.y;
        m_vert_list[i].z = cg1_support_vert.z - cg2_support_in_cg1.z;

        // Accumulate centroids
        mesh_b_center.x += cg1_support_vert.x;
        mesh_b_center.y += cg1_support_vert.y;
        mesh_b_center.z += cg1_support_vert.z;

        mesh_a_center.x += cg2_support_in_cg1.x;
        mesh_a_center.y += cg2_support_in_cg1.y;
        mesh_a_center.z += cg2_support_in_cg1.z;

        diff_sum.x += m_vert_list[i].x;
        diff_sum.y += m_vert_list[i].y;
        diff_sum.z += m_vert_list[i].z;
    }

    // Average to get centroids
    const float inv12 = 1.0f / 12.0f;
    mesh_b_center.x *= inv12;
    mesh_b_center.y *= inv12;
    mesh_b_center.z *= inv12;
    mesh_a_center.x *= inv12;
    mesh_a_center.y *= inv12;
    mesh_a_center.z *= inv12;

    // Centroid of Minkowski difference vertices
    phys_vec3 mink_center;
    mink_center.x = diff_sum.x * inv12;
    mink_center.y = diff_sum.y * inv12;
    mink_center.z = diff_sum.z * inv12;

    // --- Phase 2: Find best separating face normal ---
    // Iterate over icosahedron triangle faces, compute face normals,
    // orient them outward, and find the one with the best signed distance
    // from the origin — this is the initial separation direction for GJK

    float       best_dist_sq = 0.0f;
    phys_vec3 *best_normal = nullptr;

    // m_list_triangle: triples of indices into m_vert_list
    // Loop ends when pointer reaches string "bad separation dir.\n"
    for (const int *tri = m_list_triangle;
        tri < (const int *)"bad separation dir.\n";
        tri += 3)
    {
        const phys_vec3 *v0 = &m_vert_list[tri[0]];
        const phys_vec3 *v1 = &m_vert_list[tri[1]];
        const phys_vec3 *v2 = &m_vert_list[tri[2]];

        // Edge vectors from v0
        phys_vec3 e1 = { v2->x - v0->x, v2->y - v0->y, v2->z - v0->z };
        phys_vec3 e2 = { v1->x - v0->x, v1->y - v0->y, v1->z - v0->z };

        // Face normal via cross product e1 x e2
        phys_vec3 normal;
        normal.x = e1.y * e2.z - e1.z * e2.y;  // wait, cross(e2,e1)?
        normal.y = e1.z * e2.x - e1.x * e2.z;
        normal.z = e1.x * e2.y - e1.y * e2.x;

        // Vector from mink_center to v0
        phys_vec3 to_v0;
        to_v0.x = v0->x - mink_center.x;
        to_v0.y = v0->y - mink_center.y;
        to_v0.z = v0->z - mink_center.z;

        // Ensure normal points away from centroid (outward)
        float dot_center = to_v0.x * normal.x + to_v0.y * normal.y + to_v0.z * normal.z;
        if (dot_center < 0.0f)
        {
            normal.x = -normal.x;
            normal.y = -normal.y;
            normal.z = -normal.z;
        }

        float normal_len_sq = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;

        if (normal_len_sq < 1e-8f)
        {
            // Degenerate face, skip
            continue;
        }

        // Signed distance from origin to face plane, scaled by normal length
        // dist = (-v0 . normal) / |normal|, then stored as dist^2 * sign(dist)
        phys_vec3 neg_v0 = { -v0->x, -v0->y, -v0->z };
        float dot_origin = neg_v0.x * normal.x + neg_v0.y * normal.y + neg_v0.z * normal.z;
        float dist_sq_signed = (dot_origin * dot_origin) / normal_len_sq;
        if (dot_origin < 0.0f)
            dist_sq_signed = -dist_sq_signed;

        // Keep face with maximum signed distance (most separated)
        if (!best_normal || best_dist_sq < dist_sq_signed)
        {
            best_dist_sq = dist_sq_signed;
            best_normal = &normal; // NOTE: this stores stack address — see below
        }
    }

    // --- Phase 3: Output result ---

    if (!best_normal)
    {
        // No valid face found — fall back to vector between shape centers
        phys_vec3 center_diff;
        center_diff.x = mesh_a_center.x - mesh_b_center.x;
        center_diff.y = mesh_a_center.y - mesh_b_center.y;
        center_diff.z = mesh_a_center.z - mesh_b_center.z;

        float len_sq = center_diff.x * center_diff.x
            + center_diff.y * center_diff.y
            + center_diff.z * center_diff.z;

        if (len_sq < 1e-8f)
        {
            tlWarning("bad separation dir.\n");
            *result = PHYS_X_VEC; // fallback: X axis
            return result;
        }

        result->x = center_diff.x;
        result->y = center_diff.y;
        result->z = center_diff.z;
        return result;
    }

    // Validate the best normal isn't degenerate
    float best_len_sq = best_normal->x * best_normal->x
        + best_normal->y * best_normal->y
        + best_normal->z * best_normal->z;

    if (best_len_sq < 1e-8f)
    {
        iassert(!"AbsSquared(*best_normal) >= phys_sqr(GJK_MIN_SUPPORT_DIR_LENGTH)");
        // best_normal stays as whatever it was — original code falls through
    }

    // Negate: we want the direction pointing FROM cg2 TOWARD cg1
    result->x = -best_normal->x;
    result->y = -best_normal->y;
    result->z = -best_normal->z;
    return result;
}
#endif

double gjk_sep_dir::dist_seg(const phys_vec3 *v0, const phys_vec3 *v1, const phys_vec3 *mesh_center, phys_vec3 *direction)
{
    double v4; // st7
    double v5; // st5
    double v6; // st6
    double v7; // st3
    double v8; // st7
    double v9; // rtt
    double v10; // st6
    phys_vec3 *v11; // ecx
    double z; // st6
    float v14; // [esp-2Ch] [ebp-4Ch]
    float v15; // [esp-2Ch] [ebp-4Ch]
    float v16; // [esp-2Ch] [ebp-4Ch]
    float v17; // [esp-28h] [ebp-48h]
    float v18; // [esp-28h] [ebp-48h]
    float v19; // [esp-28h] [ebp-48h]
    float v20; // [esp-24h] [ebp-44h]
    float v21; // [esp-24h] [ebp-44h]
    float v22; // [esp-24h] [ebp-44h]
    float v23; // [esp-1Ch] [ebp-3Ch]
    float v24; // [esp-18h] [ebp-38h]
    float v25; // [esp-14h] [ebp-34h]
    float v26; // [esp-Ch] [ebp-2Ch]
    float v27; // [esp-Ch] [ebp-2Ch]
    float v28; // [esp-Ch] [ebp-2Ch]
    float v29; // [esp-8h] [ebp-28h]
    float v30; // [esp-8h] [ebp-28h]
    float v31; // [esp-8h] [ebp-28h]
    float v32; // [esp-4h] [ebp-24h]
    float v33; // [esp-4h] [ebp-24h]
    float v34; // [esp-4h] [ebp-24h]
    float dif_v1_v0_12; // [esp+Ch] [ebp-14h]
    float v36; // [esp+10h] [ebp-10h]
    float v37; // [esp+10h] [ebp-10h]
    float v38; // [esp+10h] [ebp-10h]
    float v39; // [esp+10h] [ebp-10h]
    float v40; // [esp+10h] [ebp-10h]
    float v41; // [esp+10h] [ebp-10h]
    float v42; // [esp+10h] [ebp-10h]

    v26 = v1->x - v0->x;
    v29 = v1->y - v0->y;
    v32 = v1->z - v0->z;
    v4 = v26;
    v5 = v29;
    v6 = v32;
    v36 = v32 * v32 + v26 * v26 + v29 * v29;
    if ( v36 <= 0.001 )
    {
        v11 = direction;
        v8 = 0.0;
        direction->x = v1->x;
        direction->y = v1->y;
        z = v1->z;
    }
    else
    {
        v27 = -v0->x;
        v30 = -v0->y;
        v33 = -v0->z;
        v7 = v4 * v27;
        v8 = 0.0;
        v9 = v36;
        v37 = v6 * v33 + v5 * v30 + v7;
        v38 = v37 / v9;
        v10 = v38;
        if ( v38 >= 0.0 )
        {
            if ( v10 > 1.0 )
                v10 = 1.0;
            v39 = v10;
        }
        else
        {
            v39 = 0.0;
        }
        v23 = v1->x * v39;
        v24 = v39 * v1->y;
        v25 = v1->z * v39;
        v40 = 1.0 - v39;
        v28 = v0->x * v40;
        v31 = v40 * v0->y;
        v11 = direction;
        v34 = v40 * v0->z;
        v14 = v28 + v23;
        v17 = v31 + v24;
        v20 = v34 + v25;
        direction->x = v14;
        direction->y = v17;
        z = v20;
    }
    v11->z = z;
    v41 = v11->y * v11->y + v11->x * v11->x + v11->z * v11->z;
    v42 = -v41;
    v15 = v11->x - mesh_center->x;
    v18 = v11->y - mesh_center->y;
    v21 = v11->z - mesh_center->z;
    dif_v1_v0_12 = v11->y * v18 + v15 * v11->x + v11->z * v21;
    if ( v8 > dif_v1_v0_12 )
    {
        v16 = -v11->x;
        v19 = -v11->y;
        v22 = -v11->z;
        v11->x = v16;
        v11->y = v19;
        v11->z = v22;
        return (float)-v42;
    }
    return v42;
}

