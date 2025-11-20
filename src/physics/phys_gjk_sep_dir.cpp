#include "phys_gjk_sep_dir.h"

const phys_vec3 * gjk_sep_dir::comp_sep_dir@<eax>(
        int a1@<ebp>,
        float *a2@<edi>,
        float *a3@<esi>,
        const phys_vec3 *result,
        const phys_gjk_input *m_pcd,
        phys_gjk_info *m_gjk_info)
{
  int v6; // edi
  const phys_gjk_geom *gjk_cg2; // ecx
  const phys_vec3 *v8; // eax
  double y; // st7
  double v10; // st6
  double z; // st5
  double v12; // st4
  double w; // st3
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
  float v30; // ecx
  const phys_vec3 *v31; // eax
  phys_vec3 *v32; // ecx
  float v33; // edx
  float x; // ecx
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
  float v47; // [esp+234h] [ebp-ACh] BYREF
  float v48; // [esp+238h] [ebp-A8h]
  float v49; // [esp+23Ch] [ebp-A4h]
  phys_vec3 m_b_loc_vert; // [esp+240h] [ebp-A0h]
  float v51; // [esp+254h] [ebp-8Ch]
  float v52; // [esp+258h] [ebp-88h]
  float v53; // [esp+25Ch] [ebp-84h]
  float v54; // [esp+264h] [ebp-7Ch]
  float v55; // [esp+268h] [ebp-78h]
  float v56; // [esp+26Ch] [ebp-74h]
  phys_vec3 m_b_vert; // [esp+270h] [ebp-70h] BYREF
  phys_vec3 m_a_vert; // [esp+280h] [ebp-60h]
  phys_vec3 m_mesh_b_center; // [esp+290h] [ebp-50h]
  phys_vec3 m_mesh_a_center; // [esp+2A0h] [ebp-40h]
  float v61; // [esp+2B0h] [ebp-30h]
  float v62; // [esp+2B4h] [ebp-2Ch]
  phys_vec3 *best_normal; // [esp+2B8h] [ebp-28h]
  float best_dist_sq; // [esp+2BCh] [ebp-24h]
  phys_vec3 dir; // [esp+2C0h] [ebp-20h]
  float v66; // [esp+2D0h] [ebp-10h]
  int v67; // [esp+2D4h] [ebp-Ch]
  void *v68; // [esp+2D8h] [ebp-8h]
  void *retaddr; // [esp+2E0h] [ebp+0h]

  v67 = a1;
  v68 = retaddr;
  m_mesh_b_center.y = 0.0;
  m_mesh_b_center.z = 0.0;
  m_mesh_b_center.w = 0.0;
  v6 = 0;
  m_a_vert.y = 0.0;
  m_a_vert.z = 0.0;
  m_a_vert.w = 0.0;
  v62 = 0.0;
  *(float *)&best_normal = 0.0;
  best_dist_sq = 0.0;
  do
  {
    ((void (__thiscall *)(const phys_gjk_geom *, const phys_vec3 *, float *, int *, float *, float *, float *))m_pcd->gjk_cg1->support)(
      m_pcd->gjk_cg1,
      &m_list_support_dir[v6],
      &m_b_vert.y,
      &v43,
      a2,
      a3,
      p_y);
    gjk_cg2 = m_pcd->gjk_cg2;
    m_b_loc_vert.y = -*(float *)(v6 * 16 + 14786112);
    p_y = &m_vert_list[11].y;
    m_b_loc_vert.z = -*(float *)(v6 * 16 + 14786116);
    a3 = &v47;
    a2 = &v44;
    m_b_loc_vert.w = -*(float *)(v6 * 16 + 14786120);
    v44 = m_gjk_info->cg2_to_cg1_xform.x.z * m_b_loc_vert.w
        + m_gjk_info->cg2_to_cg1_xform.x.y * m_b_loc_vert.z
        + m_b_loc_vert.y * m_gjk_info->cg2_to_cg1_xform.x.x;
    v45 = m_gjk_info->cg2_to_cg1_xform.y.x * m_b_loc_vert.y
        + m_gjk_info->cg2_to_cg1_xform.y.y * m_b_loc_vert.z
        + m_b_loc_vert.w * m_gjk_info->cg2_to_cg1_xform.y.z;
    v46 = m_b_loc_vert.w * m_gjk_info->cg2_to_cg1_xform.z.z
        + m_b_loc_vert.z * m_gjk_info->cg2_to_cg1_xform.z.y
        + m_b_loc_vert.y * m_gjk_info->cg2_to_cg1_xform.z.x;
    ((void (__thiscall *)(const phys_gjk_geom *))gjk_cg2->support)(gjk_cg2);
    v8 = phys_multiply(&v42, &m_gjk_info->cg2_to_cg1_xform, (const phys_vec3 *)&v47);
    ++v6;
    v54 = v8->x + m_gjk_info->cg2_to_cg1_xform.w.x;
    v55 = v8->y + m_gjk_info->cg2_to_cg1_xform.w.y;
    v56 = v8->z + m_gjk_info->cg2_to_cg1_xform.w.z;
    y = m_b_vert.y;
    v10 = v54;
    v51 = m_b_vert.y - v54;
    z = m_b_vert.z;
    v12 = v55;
    v52 = m_b_vert.z - v55;
    w = m_b_vert.w;
    v14 = v56;
    v53 = m_b_vert.w - v56;
    m_normal_list[v6 + 18].y = v51;
    m_normal_list[v6 + 18].z = v52;
    m_normal_list[v6 + 18].w = v53;
    m_mesh_b_center.y = y + m_mesh_b_center.y;
    m_mesh_b_center.z = z + m_mesh_b_center.z;
    m_mesh_b_center.w = w + m_mesh_b_center.w;
    m_a_vert.y = v10 + m_a_vert.y;
    m_a_vert.z = v12 + m_a_vert.z;
    m_a_vert.w = v14 + m_a_vert.w;
    v62 = m_normal_list[v6 + 18].y + v62;
    *(float *)&best_normal = m_normal_list[v6 + 18].z + *(float *)&best_normal;
    best_dist_sq = best_dist_sq + m_normal_list[v6 + 18].w;
  }
  while ( v6 < 12 );
  m_mesh_a_center.w = 0.0;
  v15 = m_list_triangle;
  v16 = (float *)&v39;
  m_mesh_b_center.y = m_mesh_b_center.y * 0.0833333358168602;
  m_mesh_b_center.z = m_mesh_b_center.z * 0.0833333358168602;
  m_mesh_b_center.w = m_mesh_b_center.w * 0.0833333358168602;
  m_a_vert.y = m_a_vert.y * 0.0833333358168602;
  m_a_vert.z = m_a_vert.z * 0.0833333358168602;
  m_a_vert.w = m_a_vert.w * 0.0833333358168602;
  v62 = v62 * 0.0833333358168602;
  *(float *)&best_normal = *(float *)&best_normal * 0.0833333358168602;
  best_dist_sq = 0.0833333358168602 * best_dist_sq;
  v61 = 0.0;
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
    v54 = *v21 - *v17;
    v55 = v21[1] - v17[1];
    v56 = v21[2] - v17[2];
    m_b_loc_vert.y = v55 * v53 - v56 * v52;
    m_b_loc_vert.z = v56 * v51 - v53 * v54;
    m_b_loc_vert.w = v52 * v54 - v55 * v51;
    v24 = m_b_loc_vert.y;
    *(v16 - 2) = m_b_loc_vert.y;
    v25 = m_b_loc_vert.z;
    *(v16 - 1) = m_b_loc_vert.z;
    v26 = m_b_loc_vert.w;
    *v16 = m_b_loc_vert.w;
    m_b_vert.y = *v17 - v62;
    m_b_vert.z = v17[1] - *(float *)&best_normal;
    m_b_vert.w = v17[2] - best_dist_sq;
    v66 = m_b_vert.y * v24 + m_b_vert.z * v25 + m_b_vert.w * v26;
    if ( v66 < 0.0 )
    {
      v44 = -v24;
      v45 = -v25;
      v46 = -v26;
      *v23 = v44;
      *(v16 - 1) = v45;
      *v16 = v46;
    }
    v66 = *(v16 - 1) * *(v16 - 1) + *v23 * *v23 + *v16 * *v16;
    v27 = v66;
    if ( v66 < 0.0000000099999991 )
    {
      v30 = m_mesh_a_center.w;
    }
    else
    {
      v47 = -*v17;
      v48 = -v17[1];
      v49 = -v17[2];
      v66 = v47 * *v23 + *(v16 - 1) * v48 + *v16 * v49;
      v28 = v66;
      v66 = v66 * v66;
      v66 = v66 / v27;
      v29 = v66;
      if ( v28 < 0.0 )
        v29 = -v29;
      v30 = m_mesh_a_center.w;
      v66 = v29;
      if ( !LODWORD(m_mesh_a_center.w) || v61 < (double)v66 )
      {
        v61 = v66;
        LODWORD(m_mesh_a_center.w) = v16 - 2;
        LODWORD(v30) = v16 - 2;
      }
    }
    v15 += 3;
    v16 += 4;
  }
  while ( v15 < (const int *)"bad separation dir.\n" );
  if ( v30 == 0.0 )
  {
    v62 = m_mesh_b_center.y - m_a_vert.y;
    *(float *)&best_normal = m_mesh_b_center.z - m_a_vert.z;
    best_dist_sq = m_mesh_b_center.w - m_a_vert.w;
    v61 = v62 * v62 + *(float *)&best_normal * *(float *)&best_normal + best_dist_sq * best_dist_sq;
    if ( v61 < 0.0000000099999991 )
    {
      tlWarning("bad separation dir.\n");
      v31 = result;
      v35 = PHYS_X_VEC_13.y;
      result->x = PHYS_X_VEC_13.x;
      v33 = PHYS_X_VEC_13.z;
      result->y = v35;
      x = PHYS_X_VEC_13.w;
    }
    else
    {
      v31 = result;
      v32 = best_normal;
      result->x = v62;
      v33 = best_dist_sq;
      LODWORD(result->y) = v32;
      x = dir.x;
    }
    result->z = v33;
    result->w = x;
  }
  else
  {
    v61 = *(float *)(LODWORD(v30) + 4) * *(float *)(LODWORD(v30) + 4)
        + *(float *)LODWORD(v30) * *(float *)LODWORD(v30)
        + *(float *)(LODWORD(v30) + 8) * *(float *)(LODWORD(v30) + 8);
    if ( v61 < 0.0000000099999991 )
    {
      if ( _tlAssert(
             "source/phys_gjk_sep_dir.cpp",
             258,
             "AbsSquared(*best_normal) >= phys_sqr(GJK_MIN_SUPPORT_DIR_LENGTH)",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
      v30 = m_mesh_a_center.w;
    }
    v31 = result;
    result->x = -*(float *)LODWORD(v30);
    result->y = -*(float *)(LODWORD(v30) + 4);
    result->z = -*(float *)(LODWORD(v30) + 8);
  }
  return v31;
}

double __cdecl gjk_sep_dir::dist_seg(
        const phys_vec3 *v0,
        const phys_vec3 *v1,
        const phys_vec3 *mesh_center,
        phys_vec3 *direction)
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

