#include "phys_broad_phase.h"

void __cdecl create_broad_phase_info(rigid_body *body)
{
  unsigned int v1; // eax
  unsigned int geom_id; // eax
  gjk_base_t **p_m_first_geom; // [esp+4h] [ebp-2Ch]
  gjk_base_t **v4; // [esp+10h] [ebp-20h]
  broad_phase_info *broad_phase_info; // [esp+18h] [ebp-18h]
  gjk_base_t *gjk_geom; // [esp+1Ch] [ebp-14h]
  broad_phase_info *bpi; // [esp+20h] [ebp-10h]
  broad_phase_group *bpg; // [esp+24h] [ebp-Ch]
  gjk_base_t *geom; // [esp+28h] [ebp-8h]
  PhysObjUserData *userData; // [esp+2Ch] [ebp-4h]

  userData = (PhysObjUserData *)body->m_userdata;
  if ( gjk_geom_list_t::get_geom_count(&userData->m_gjk_geom_list) <= 1 )
  {
    if ( gjk_geom_list_t::get_geom_count(&userData->m_gjk_geom_list) != 1
      && _tlAssert(
           "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
           429,
           "userData->m_gjk_geom_list.get_geom_count() == 1",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    p_m_first_geom = &userData->m_gjk_geom_list.m_first_geom;
    if ( userData->m_gjk_geom_list.m_geom_count < 0
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
           1035,
           "m_geom_count >= 0",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    gjk_geom = *p_m_first_geom;
    if ( !*p_m_first_geom
      && _tlAssert("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp", 431, "gjk_geom", &toastPopupTitle) )
    {
      __debugbreak();
    }
    broad_phase_info = create_broad_phase_info();
    geom_id = gjk_base_t::get_geom_id(gjk_geom);
    broad_phase_info::set(
      broad_phase_info,
      body,
      &body->m_mat,
      &userData->cg2w,
      &userData->cg2rb,
      gjk_geom,
      geom_id,
      1,
      31,
      0,
      0x1C7u);
    userData->m_bpb = broad_phase_info;
    if ( rigid_body::get_flag(body, 0x20u) )
      broad_phase_info->m_env_collision_flags &= ~1u;
  }
  else
  {
    bpg = create_broad_phase_group();
    broad_phase_group::set(bpg);
    v4 = &userData->m_gjk_geom_list.m_first_geom;
    if ( userData->m_gjk_geom_list.m_geom_count < 0
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
           1035,
           "m_geom_count >= 0",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    geom = *v4;
    if ( !*v4 && _tlAssert("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp", 417, "geom", &toastPopupTitle) )
      __debugbreak();
    while ( geom )
    {
      bpi = create_broad_phase_info();
      v1 = gjk_base_t::get_geom_id(geom);
      broad_phase_info::set(bpi, body, &body->m_mat, &userData->cg2w, &userData->cg2rb, geom, v1, 1, 31, 0, 0x1C7u);
      broad_phase_group::add_bpi(bpg, bpi);
      geom = geom->m_next_geom;
    }
    userData->m_bpb = bpg;
  }
  environment_collision_list_add(userData->m_bpb);
  aasap_list_add(userData->m_bpb);
}

void __cdecl destroy_broad_phase_info(rigid_body *body)
{
  broad_phase_info *bpi; // eax
  broad_phase_group *bpg; // eax
  broad_phase_group *v3; // eax
  PhysObjUserData *userData; // [esp+8h] [ebp-4h]

  userData = (PhysObjUserData *)body->m_userdata;
  if ( userData->m_bpb )
  {
    aasap_list_remove(userData->m_bpb);
    environment_collision_list_remove(userData->m_bpb);
    if ( (userData->m_bpb->m_flags & 1) != 0 )
    {
      bpi = broad_phase_base::get_bpi(userData->m_bpb);
      destroy_broad_phase_info(bpi);
    }
    else
    {
      if ( (userData->m_bpb->m_flags & 2) == 0 )
      {
        if ( _tlAssert(
               "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_main.cpp",
               458,
               "userData->m_bpb->is_bpg()",
               &toastPopupTitle) )
        {
          __debugbreak();
        }
      }
      bpg = broad_phase_base::get_bpg(userData->m_bpb);
      destroy_broad_phase_info_list(bpg->m_list_bpi_head);
      v3 = broad_phase_base::get_bpg(userData->m_bpb);
      destroy_broad_phase_group(v3);
    }
    userData->m_bpb = 0;
  }
}

jqBatchGroup *__thiscall jqBatchGroup::jqBatchGroup(jqBatchGroup *this)
{
  jqBatchGroup *result; // eax

  result = this;
  this->BatchCount = 0;
  return result;
}

void  comp_trace_volume(
        int a1@<ebp>,
        const phys_vec3 *aabb1_min,
        const phys_vec3 *aabb1_max,
        const phys_vec3 *aabb2_min,
        const phys_vec3 *aabb2_max,
        phys_vec3 *p1,
        phys_vec3 *p2,
        phys_vec3 *half_dims)
{
  const phys_vec3 *v8; // eax
  const phys_vec3 *v9; // eax
  const phys_vec3 *v10; // eax
  const phys_vec3 *v11; // eax
  const phys_vec3 *v12; // eax
  float v13; // [esp-Ch] [ebp-4Ch] BYREF
  float v14; // [esp-8h] [ebp-48h]
  float v15; // [esp-4h] [ebp-44h]
  phys_vec3 cmin; // [esp+0h] [ebp-40h] BYREF
  phys_vec3 cmax; // [esp+10h] [ebp-30h] BYREF
  phys_vec3 total_aabb_max; // [esp+20h] [ebp-20h]
  int v19; // [esp+34h] [ebp-Ch]
  void *v20; // [esp+38h] [ebp-8h]
  void *retaddr; // [esp+40h] [ebp+0h]

  v19 = a1;
  v20 = retaddr;
  cmin.y = aabb1_max->x - aabb1_min->x;
  cmin.z = aabb1_max->y - aabb1_min->y;
  cmin.w = aabb1_max->z - aabb1_min->z;
  v13 = aabb2_max->x - aabb2_min->x;
  v14 = aabb2_max->y - aabb2_min->y;
  v15 = aabb2_max->z - aabb2_min->z;
  v8 = phys_max((phys_vec3 *)&cmax.y, (phys_vec3 *)&cmin.y, (const phys_vec3 *)&v13);
  total_aabb_max.y = v8->x * 0.5;
  total_aabb_max.z = v8->y * 0.5;
  total_aabb_max.w = 0.5 * v8->z;
  half_dims->x = total_aabb_max.y;
  half_dims->y = total_aabb_max.z;
  half_dims->z = total_aabb_max.w;
  phys_min((phys_vec3 *)&cmin.y, aabb1_min, aabb2_min);
  phys_max((phys_vec3 *)&cmax.y, aabb1_max, aabb2_max);
  v13 = cmin.y + total_aabb_max.y;
  v14 = cmin.z + total_aabb_max.z;
  v15 = cmin.w + total_aabb_max.w;
  cmin.y = cmax.y - total_aabb_max.y;
  cmin.z = cmax.z - total_aabb_max.z;
  cmin.w = cmax.w - total_aabb_max.w;
  total_aabb_max.y = aabb1_min->x + aabb1_max->x;
  total_aabb_max.z = aabb1_min->y + aabb1_max->y;
  total_aabb_max.w = aabb1_min->z + aabb1_max->z;
  cmax.y = total_aabb_max.y * 0.5;
  cmax.z = total_aabb_max.z * 0.5;
  cmax.w = 0.5 * total_aabb_max.w;
  p1->x = cmax.y;
  p1->y = cmax.z;
  p1->z = cmax.w;
  v9 = phys_min((phys_vec3 *)&cmax.y, p1, (phys_vec3 *)&cmin.y);
  p1->x = v9->x;
  p1->y = v9->y;
  p1->z = v9->z;
  v10 = phys_max((phys_vec3 *)&cmax.y, p1, (const phys_vec3 *)&v13);
  p1->x = v10->x;
  p1->y = v10->y;
  p1->z = v10->z;
  cmax.y = aabb2_min->x + aabb2_max->x;
  cmax.z = aabb2_min->y + aabb2_max->y;
  cmax.w = aabb2_min->z + aabb2_max->z;
  total_aabb_max.y = cmax.y * 0.5;
  total_aabb_max.z = cmax.z * 0.5;
  total_aabb_max.w = 0.5 * cmax.w;
  p2->x = total_aabb_max.y;
  p2->y = total_aabb_max.z;
  p2->z = total_aabb_max.w;
  v11 = phys_min((phys_vec3 *)&cmax.y, p2, (phys_vec3 *)&cmin.y);
  p2->x = v11->x;
  p2->y = v11->y;
  p2->z = v11->z;
  v12 = phys_max((phys_vec3 *)&cmin.y, p2, (const phys_vec3 *)&v13);
  p2->x = v12->x;
  p2->y = v12->y;
  p2->z = v12->z;
}

void __userpurge broad_phase_base::get_aabb(broad_phase_base *this@<ecx>, int a2@<ebp>, phys_vec3 *aabb)
{
  const phys_vec3 *v4; // eax
  const phys_vec3 *v5; // eax
  phys_vec3 v6; // [esp-20h] [ebp-2Ch] BYREF
  phys_vec3 v7; // [esp-10h] [ebp-1Ch] BYREF
  int v8; // [esp+0h] [ebp-Ch]
  void *v9; // [esp+4h] [ebp-8h]
  void *retaddr; // [esp+Ch] [ebp+0h]

  v8 = a2;
  v9 = retaddr;
  v7.x = this->m_trace_translation.x + this->m_trace_aabb_min_whace.x;
  v7.y = this->m_trace_translation.y + this->m_trace_aabb_min_whace.y;
  v7.z = this->m_trace_translation.z + this->m_trace_aabb_min_whace.z;
  v4 = phys_min(&v6, &this->m_trace_aabb_min_whace, &v7);
  aabb->x = v4->x;
  aabb->y = v4->y;
  aabb->z = v4->z;
  v7.x = this->m_trace_aabb_max_whace.x + this->m_trace_translation.x;
  v7.y = this->m_trace_aabb_max_whace.y + this->m_trace_translation.y;
  v7.z = this->m_trace_aabb_max_whace.z + this->m_trace_translation.z;
  v5 = phys_max(&v6, &this->m_trace_aabb_max_whace, &v7);
  aabb[1].x = v5->x;
  aabb[1].y = v5->y;
  aabb[1].z = v5->z;
}

void __cdecl setup_gjk_input_from_pcp(phys_gjk_input *pgi, phys_collision_pair *pcp)
{
  broad_phase_info *m_bpi1; // eax
  broad_phase_info *m_bpi2; // ecx
  const phys_gjk_geom *m_gjk_geom; // ebx
  double v7; // st7
  phys_gjk_cache_info *m_gjk_ci; // edi
  float v9; // [esp+Ch] [ebp-Ch]
  float v10; // [esp+10h] [ebp-8h]
  const phys_mat44 *m_cg_to_world_xform; // [esp+14h] [ebp-4h]
  phys_gjk_input *pgia; // [esp+20h] [ebp+8h]
  float pcpa; // [esp+24h] [ebp+Ch]
  phys_collision_pair *pcpb; // [esp+24h] [ebp+Ch]

  if ( pcp->m_hit_time < 0.0 || pcp->m_hit_time > 1.0 )
  {
    if ( _tlAssert(
           "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_collision.h",
           11,
           "pcp->m_hit_time >= 0.0f && pcp->m_hit_time <= 1.0f",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
  }
  m_bpi1 = pcp->m_bpi1;
  m_bpi2 = pcp->m_bpi2;
  pcpa = pcp->m_hit_time;
  pgi->m_cg1_translation.x = m_bpi1->m_trace_translation.x;
  pgi->m_cg1_translation.y = m_bpi1->m_trace_translation.y;
  pgi->m_cg1_translation.z = m_bpi1->m_trace_translation.z;
  pgi->m_cg2_translation.x = m_bpi2->m_trace_translation.x;
  pgi->m_cg2_translation.y = m_bpi2->m_trace_translation.y;
  pgi->m_cg2_translation.z = m_bpi2->m_trace_translation.z;
  pgi->m_start_time = pcpa;
  pgi->m_end_time = 1.0;
  m_gjk_geom = m_bpi2->m_gjk_geom;
  pgia = (phys_gjk_input *)m_bpi1->m_cg_to_world_xform;
  m_cg_to_world_xform = m_bpi2->m_cg_to_world_xform;
  pcpb = (phys_collision_pair *)m_bpi1->m_gjk_geom;
  v10 = ((double (__thiscall *)(phys_collision_pair *))pcpb->m_next_link[1].m_next_link)(pcpb) + 0.3400000035762787;
  v7 = ((double (__thiscall *)(const phys_gjk_geom *))m_gjk_geom->get_geom_radius)(m_gjk_geom);
  m_gjk_ci = pcp->m_gjk_ci;
  v9 = v7 + 0.3400000035762787;
  pgi->cg1_radius = v10;
  pgi->gjk_ci = m_gjk_ci;
  pgi->gjk_cg2 = m_gjk_geom;
  pgi->cg2_radius = v9;
  pgi->gjk_cg1 = (const phys_gjk_geom *)pcpb;
  pgi->cg1_to_world_xform = (const phys_mat44 *)pgia;
  pgi->cg2_to_world_xform = m_cg_to_world_xform;
}

void __userpurge phys_wheel_collide_info::collision_process(
        phys_wheel_collide_info *this@<ecx>,
        int a2@<ebp>,
        int a3@<edi>,
        int a4@<esi>,
        broad_phase_info *bpi)
{
  const phys_mat44 *m_cg_to_world_xform; // edi
  const phys_gjk_geom *m_gjk_geom; // ecx
  _BYTE v10[12]; // [esp+1Ch] [ebp-4Ch] BYREF
  phys_vec3 ray_pos_loc; // [esp+28h] [ebp-40h] BYREF
  phys_vec3 hitn; // [esp+38h] [ebp-30h] BYREF
  phys_vec3 ray_dir_loc; // [esp+48h] [ebp-20h]
  float v14; // [esp+58h] [ebp-10h] BYREF
  unsigned int v15[2]; // [esp+5Ch] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+68h] [ebp+0h]

  v15[0] = a2;
  v15[1] = retaddr;
  m_cg_to_world_xform = bpi->m_cg_to_world_xform;
  phys_full_inv_multiply((int)v15, (phys_vec3 *)v10, m_cg_to_world_xform, &this->m_ray_pos);
  v14 = m_cg_to_world_xform->y.y * this->m_ray_dir.y
      + m_cg_to_world_xform->y.x * this->m_ray_dir.x
      + m_cg_to_world_xform->y.z * this->m_ray_dir.z;
  ray_dir_loc.w = m_cg_to_world_xform->z.y * this->m_ray_dir.y
                + m_cg_to_world_xform->z.x * this->m_ray_dir.x
                + m_cg_to_world_xform->z.z * this->m_ray_dir.z;
  m_gjk_geom = bpi->m_gjk_geom;
  hitn.y = m_cg_to_world_xform->x.y * this->m_ray_dir.y
         + this->m_ray_dir.x * m_cg_to_world_xform->x.x
         + m_cg_to_world_xform->x.z * this->m_ray_dir.z;
  hitn.z = v14;
  hitn.w = ray_dir_loc.w;
  if ( ((unsigned __int8 (__thiscall *)(const phys_gjk_geom *, _BYTE *, float *, float, float *, float *, int, int))m_gjk_geom->ray_cast)(
         m_gjk_geom,
         v10,
         &hitn.y,
         this->m_hit_t,
         &v14,
         &ray_pos_loc.y,
         a3,
         a4) )
  {
    if ( this->m_hit_t < (double)v14 )
    {
      if ( _tlAssert("source/phys_broad_phase.cpp", 41, "hit_t <= m_hit_t", &toastPopupTitle) )
        __debugbreak();
    }
    this->m_hit_t = v14;
    this->m_hitn.x = ray_pos_loc.y;
    this->m_hitn.y = ray_pos_loc.z;
    this->m_hitn.z = ray_pos_loc.w;
    this->m_hit_bpi = bpi;
  }
}

void __userpurge phys_wheel_collide_info::collision_epilog(
        phys_wheel_collide_info *this@<ecx>,
        int a2@<ebp>,
        rigid_body_constraint_wheel *rbc_wheel)
{
  phys_vec3 *p_m_hitn; // esi
  double v5; // st7
  const phys_vec3 *v6; // eax
  double z; // st7
  broad_phase_info *m_hit_bpi; // edi
  const phys_mat44 *m_rb_to_world_xform; // [esp-30h] [ebp-70h]
  _BYTE v10[12]; // [esp-Ch] [ebp-4Ch] BYREF
  phys_vec3 b2_hitn_loc; // [esp+0h] [ebp-40h] BYREF
  float v12; // [esp+14h] [ebp-2Ch] BYREF
  float v13; // [esp+18h] [ebp-28h]
  float v14; // [esp+1Ch] [ebp-24h]
  float m_hit_t; // [esp+30h] [ebp-10h]
  unsigned int v16[2]; // [esp+34h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+40h] [ebp+0h]

  v16[0] = a2;
  v16[1] = retaddr;
  rigid_body_constraint_wheel::set_no_collision(rbc_wheel);
  if ( this->m_hit_bpi )
  {
    p_m_hitn = &this->m_hitn;
    m_hit_t = this->m_hitn.y * this->m_hitn.y + this->m_hitn.x * this->m_hitn.x + this->m_hitn.z * this->m_hitn.z;
    m_hit_t = sqrt(m_hit_t);
    if ( m_hit_t > 0.000001 )
    {
      m_hit_t = 1.0 / m_hit_t;
      v5 = m_hit_t;
      p_m_hitn->x = p_m_hitn->x * m_hit_t;
      this->m_hitn.y = this->m_hitn.y * v5;
      this->m_hitn.z = v5 * this->m_hitn.z;
      v6 = phys_multiply((phys_vec3 *)&b2_hitn_loc.y, this->m_hit_bpi->m_cg_to_world_xform, &this->m_hitn);
      p_m_hitn->x = v6->x;
      this->m_hitn.y = v6->y;
      this->m_hitn.z = v6->z;
      m_hit_t = this->m_hit_t;
      v12 = m_hit_t * this->m_ray_dir.x;
      v13 = this->m_ray_dir.y * m_hit_t;
      v14 = m_hit_t * this->m_ray_dir.z;
      b2_hitn_loc.y = this->m_ray_pos.x + v12;
      b2_hitn_loc.z = this->m_ray_pos.y + v13;
      z = this->m_ray_pos.z;
      m_hit_bpi = this->m_hit_bpi;
      m_rb_to_world_xform = m_hit_bpi->m_rb_to_world_xform;
      b2_hitn_loc.w = z + v14;
      phys_full_inv_multiply((int)v16, (phys_vec3 *)&v12, m_rb_to_world_xform, (phys_vec3 *)&b2_hitn_loc.y);
      phys_inv_multiply((const phys_vec3 *)v10, m_hit_bpi->m_rb_to_world_xform, p_m_hitn);
      rigid_body_constraint_wheel::set_collision(
        rbc_wheel,
        m_hit_bpi->m_rb,
        (const phys_vec3 *)&v12,
        (const phys_vec3 *)v10);
    }
  }
}

void __thiscall axis_aligned_sweep_and_prune::sap_node::init(
        axis_aligned_sweep_and_prune::sap_node *this,
        broad_phase_base *bpb,
        axis_aligned_sweep_and_prune::axis_element **xlist,
        axis_aligned_sweep_and_prune::axis_element **ylist,
        axis_aligned_sweep_and_prune::axis_element **zlist)
{
  this->m_bpb = bpb;
  this->m_updated = 0;
  *(_QWORD *)&this->m_ae1[0][0].m_node = (unsigned int)this;
  this->m_ae1[0][1].m_node = this;
  this->m_ae1[0][1].m_min_max = 1;
  this->m_ae1[0][0].m_next = &this->m_ae1[0][1];
  this->m_ae1[0][1].m_next = *xlist;
  *xlist = this->m_ae1[0];
  *(_QWORD *)&this->m_ae1[1][0].m_node = (unsigned int)this;
  this->m_ae1[1][1].m_node = this;
  this->m_ae1[1][1].m_min_max = 1;
  this->m_ae1[1][0].m_next = &this->m_ae1[1][1];
  this->m_ae1[1][1].m_next = *ylist;
  *ylist = this->m_ae1[1];
  *(_QWORD *)&this->m_ae1[2][0].m_node = (unsigned int)this;
  this->m_ae1[2][1].m_node = this;
  this->m_ae1[2][1].m_min_max = 1;
  this->m_ae1[2][0].m_next = &this->m_ae1[2][1];
  this->m_ae1[2][1].m_next = *zlist;
  *zlist = this->m_ae1[2];
}

bool __thiscall axis_aligned_sweep_and_prune::are_overlapping(
        axis_aligned_sweep_and_prune *this,
        axis_aligned_sweep_and_prune::sap_node *n1,
        axis_aligned_sweep_and_prune::sap_node *n2)
{
  return n1->m_ae1[0][0].m_ae_list_index < n2->m_ae1[0][1].m_ae_list_index
      && n2->m_ae1[0][0].m_ae_list_index < n1->m_ae1[0][1].m_ae_list_index
      && n1->m_ae1[1][0].m_ae_list_index < n2->m_ae1[1][1].m_ae_list_index
      && n2->m_ae1[1][0].m_ae_list_index < n1->m_ae1[1][1].m_ae_list_index
      && n1->m_ae1[2][0].m_ae_list_index < n2->m_ae1[2][1].m_ae_list_index
      && n2->m_ae1[2][0].m_ae_list_index < n1->m_ae1[2][1].m_ae_list_index;
}

void __thiscall axis_aligned_sweep_and_prune::remove(
        axis_aligned_sweep_and_prune *this,
        axis_aligned_sweep_and_prune::axis_element **axis_list,
        axis_aligned_sweep_and_prune::sap_node *node)
{
  axis_aligned_sweep_and_prune::axis_element **p_m_next; // ecx
  axis_aligned_sweep_and_prune::axis_element *v4; // eax
  int v5; // edx

  p_m_next = axis_list;
  v4 = *axis_list;
  v5 = 0;
  if ( !*axis_list )
    goto LABEL_12;
  do
  {
    if ( v5 >= 2 )
      break;
    if ( v4->m_node == node )
    {
      *p_m_next = v4->m_next;
      ++v5;
    }
    else
    {
      p_m_next = &v4->m_next;
    }
    v4 = *p_m_next;
  }
  while ( *p_m_next );
  if ( v5 != 2 )
  {
LABEL_12:
    if ( _tlAssert("source/phys_broad_phase.cpp", 515, "count == 2", &toastPopupTitle) )
      __debugbreak();
  }
}

void __cdecl init_bpeqi(broad_phase_environment_query_input *bpeqi, broad_phase_base *bpb)
{
  float v2; // [esp-10h] [ebp-1Ch]
  float v3; // [esp-10h] [ebp-1Ch]
  float v4; // [esp-Ch] [ebp-18h]
  float v5; // [esp-Ch] [ebp-18h]
  float v6; // [esp-8h] [ebp-14h]
  float v7; // [esp-8h] [ebp-14h]

  v2 = bpb->m_trace_aabb_min_whace.x - 0.5099999904632568;
  v4 = bpb->m_trace_aabb_min_whace.y - 0.5099999904632568;
  v6 = bpb->m_trace_aabb_min_whace.z - 0.5099999904632568;
  bpeqi->trace_aabb_min_wace.x = v2;
  bpeqi->trace_aabb_min_wace.y = v4;
  bpeqi->trace_aabb_min_wace.z = v6;
  v3 = bpb->m_trace_aabb_max_whace.x + 0.5099999904632568;
  v5 = bpb->m_trace_aabb_max_whace.y + 0.5099999904632568;
  v7 = bpb->m_trace_aabb_max_whace.z + 0.5099999904632568;
  bpeqi->trace_aabb_max_wace.x = v3;
  bpeqi->trace_aabb_max_wace.y = v5;
  bpeqi->trace_aabb_max_wace.z = v7;
  bpeqi->trace_translation.x = bpb->m_trace_translation.x;
  bpeqi->trace_translation.y = bpb->m_trace_translation.y;
  bpeqi->trace_translation.z = bpb->m_trace_translation.z;
  bpeqi->env_collision_flags = bpb->m_env_collision_flags;
}

void __cdecl check_terrain_query_params(broad_phase_base *bpb)
{
  float ntranslation_sq; // [esp+8h] [ebp+8h]

  ntranslation_sq = bpb->m_trace_translation.y * bpb->m_trace_translation.y
                  + bpb->m_trace_translation.x * bpb->m_trace_translation.x
                  + bpb->m_trace_translation.z * bpb->m_trace_translation.z;
  if ( ntranslation_sq > 260100.0 )
  {
    if ( (pai_check_terrain_query_params.m_hits_total_count < pai_check_terrain_query_params.m_max_hits_total
       || !pai_check_terrain_query_params.m_max_hits_total)
      && pai_check_terrain_query_params.m_hits_frame_count < pai_check_terrain_query_params.m_max_hits_per_frame )
    {
      if ( pai_check_terrain_query_params.m_use_warnings_only )
      {
        PHYS_WARNING(
          "source/phys_broad_phase.cpp",
          636,
          "ntranslation_sq <= MAX_TRANSLATION_SQ",
          "terrain query translation too large.");
      }
      else if ( ntranslation_sq > 260100.0 )
      {
        if ( _tlAssert(
               "source/phys_broad_phase.cpp",
               636,
               "ntranslation_sq <= MAX_TRANSLATION_SQ",
               "terrain query translation too large.") )
        {
          __debugbreak();
        }
      }
    }
    _InterlockedExchangeAdd(&pai_check_terrain_query_params.m_hits_total_count, 1u);
    _InterlockedExchangeAdd(&pai_check_terrain_query_params.m_hits_frame_count, 1u);
  }
}

void  calc_largest_vel_sq(int a1@<ebp>, broad_phase_info *bpi)
{
  rigid_body *m_rb; // edi
  float *p_x; // esi
  double v4; // st7
  double v5; // st7
  double v6; // st6
  phys_vec3 *v7; // eax
  phys_vec3 *v8; // eax
  const phys_vec3 *v9; // eax
  float *p_m_largest_vel_sq; // edx
  double v11; // st7
  float w; // ecx
  phys_vec3 v13; // [esp-4Ch] [ebp-BCh] BYREF
  phys_vec3 v14; // [esp-3Ch] [ebp-ACh] BYREF
  int v15; // [esp-2Ch] [ebp-9Ch] BYREF
  phys_vec3 v16; // [esp-1Ch] [ebp-8Ch] BYREF
  _BYTE v17[12]; // [esp-Ch] [ebp-7Ch] BYREF
  phys_vec3 dir_loc; // [esp+0h] [ebp-70h] BYREF
  float v19; // [esp+20h] [ebp-50h]
  float y; // [esp+24h] [ebp-4Ch] BYREF
  float z; // [esp+28h] [ebp-48h]
  float t_vel_sq; // [esp+2Ch] [ebp-44h]
  phys_vec3 dir; // [esp+30h] [ebp-40h] BYREF
  float v24; // [esp+44h] [ebp-2Ch] BYREF
  float v25; // [esp+48h] [ebp-28h]
  float v26; // [esp+4Ch] [ebp-24h]
  phys_vec3 support_pt; // [esp+50h] [ebp-20h]
  float v28; // [esp+60h] [ebp-10h]
  int v29; // [esp+64h] [ebp-Ch] BYREF
  float lvs; // [esp+68h] [ebp-8h]
  float retaddr; // [esp+70h] [ebp+0h]

  v29 = a1;
  lvs = retaddr;
  if ( (bpi->m_flags & 4) != 0 && _tlAssert("source/phys_broad_phase.cpp", 692, "!bpi->is_bpi_env()", &toastPopupTitle) )
    __debugbreak();
  m_rb = bpi->m_rb;
  if ( (m_rb->m_flags & 0x30) != 0
    && _tlAssert(
         "source/phys_broad_phase.cpp",
         694,
         "!rb->get_flag(rigid_body::FLAG_ENVIRONMENT_RIGID_BODY|rigid_body::FLAG_USER_RIGID_BODY)",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  p_x = &m_rb->m_a_vel.x;
  support_pt.w = m_rb->m_a_vel.y * m_rb->m_a_vel.y
               + m_rb->m_a_vel.x * m_rb->m_a_vel.x
               + m_rb->m_a_vel.z * m_rb->m_a_vel.z;
  v19 = m_rb->m_t_vel.y * m_rb->m_t_vel.y + m_rb->m_t_vel.x * m_rb->m_t_vel.x + m_rb->m_t_vel.z * m_rb->m_t_vel.z;
  v4 = 0.0000010000001;
  if ( support_pt.w >= 0.0000010000001 )
  {
    y = m_rb->m_t_vel.y * m_rb->m_a_vel.z - m_rb->m_t_vel.z * m_rb->m_a_vel.y;
    z = m_rb->m_a_vel.x * m_rb->m_t_vel.z - m_rb->m_t_vel.x * m_rb->m_a_vel.z;
    t_vel_sq = m_rb->m_t_vel.x * m_rb->m_a_vel.y - m_rb->m_a_vel.x * m_rb->m_t_vel.y;
    v28 = z * z + y * y + t_vel_sq * t_vel_sq;
    if ( v28 < 0.0000010000001 )
    {
      dir.y = m_rb->m_a_vel.y * 0.0 - m_rb->m_a_vel.z * 0.0;
      v6 = 0.0 * *p_x;
      dir.z = m_rb->m_a_vel.z - v6;
      dir.w = v6 - m_rb->m_a_vel.y;
      v24 = m_rb->m_t_vel.x + dir.y;
      v25 = m_rb->m_t_vel.y + dir.z;
      v26 = m_rb->m_t_vel.z + dir.w;
      dir.y = m_rb->m_a_vel.z * v25 - v26 * m_rb->m_a_vel.y;
      dir.z = v26 * *p_x - m_rb->m_a_vel.z * v24;
      dir.w = v24 * m_rb->m_a_vel.y - v25 * m_rb->m_a_vel.x;
      y = dir.y;
      z = dir.z;
      t_vel_sq = dir.w;
      v28 = dir.w * dir.w + dir.z * dir.z + dir.y * dir.y;
      if ( v28 < 0.0000010000001 )
      {
        v24 = 0.0;
        v25 = 2.0;
        v26 = 0.0;
        v7 = phys_cross(&v14, &m_rb->m_a_vel, (const phys_vec3 *)&v24);
        dir.y = m_rb->m_t_vel.x + v7->x;
        dir.z = m_rb->m_t_vel.y + v7->y;
        dir.w = m_rb->m_t_vel.z + v7->z;
        v8 = phys_cross(&v16, (phys_vec3 *)&dir.y, &m_rb->m_a_vel);
        y = v8->x;
        z = v8->y;
        t_vel_sq = v8->z;
        v28 = z * z + y * y + t_vel_sq * t_vel_sq;
        v4 = 0.0000010000001;
      }
    }
    if ( v4 > v28 )
    {
      v5 = v19 + support_pt.w;
    }
    else
    {
      phys_inv_multiply((const phys_vec3 *)v17, bpi->m_cg_to_world_xform, (const phys_vec3 *)&y);
      bpi->m_gjk_geom->support(
        (phys_gjk_geom *)bpi->m_gjk_geom,
        (const phys_vec3 *)v17,
        (phys_vec3 *)&dir_loc.y,
        (phys_vec3 *)&v15);
      LODWORD(support_pt.w) = &bpi->m_rb_to_world_xform->w;
      v9 = phys_full_multiply((int)&v29, &v13, bpi->m_cg_to_world_xform, (phys_vec3 *)&dir_loc.y);
      v24 = v9->x - *(float *)LODWORD(support_pt.w);
      v25 = v9->y - *(float *)(LODWORD(support_pt.w) + 4);
      v26 = v9->z - *(float *)(LODWORD(support_pt.w) + 8);
      dir.y = m_rb->m_a_vel.y * v26 - v25 * m_rb->m_a_vel.z;
      dir.z = v24 * m_rb->m_a_vel.z - v26 * m_rb->m_a_vel.x;
      dir.w = v25 * m_rb->m_a_vel.x - v24 * m_rb->m_a_vel.y;
      v24 = dir.y + m_rb->m_t_vel.x;
      v25 = m_rb->m_t_vel.y + dir.z;
      v26 = m_rb->m_t_vel.z + dir.w;
      v5 = v25 * v25 + v24 * v24 + v26 * v26;
    }
  }
  else
  {
    v5 = v19;
  }
  v28 = v5;
  p_m_largest_vel_sq = &m_rb->m_largest_vel_sq;
  v11 = v28;
  do
  {
    support_pt.w = *p_m_largest_vel_sq;
    if ( support_pt.w > v11 )
      break;
    w = support_pt.w;
  }
  while ( _InterlockedCompareExchange(
            (volatile signed __int32 *)p_m_largest_vel_sq,
            SLODWORD(v28),
            SLODWORD(support_pt.w)) != LODWORD(w) );
}

void __cdecl broad_phase_reset_buffer()
{
  phys_transient_allocator::reset(&G_BPM->g_collision_memory_buffer);
}

broad_phase_memory_info *__thiscall broad_phase_memory_info::broad_phase_memory_info(broad_phase_memory_info *this)
{
  broad_phase_memory_info *result; // eax

  result = this;
  this->m_max_num_gjk_ci = 0;
  this->m_max_num_sap_active_pairs = 0;
  this->m_max_num_surface_types = 0;
  return result;
}

int  bp_env_jq_batch_function1@<eax>(int a1@<ebp>, jqBatch *pBatch)
{
  signed __int32 i; // eax
  float **Input; // esi
  float *v4; // ecx
  double v5; // st7
  double v6; // st7
  double v7; // st7
  float *v8; // ecx
  double v9; // st7
  bool v10; // c0
  bool v11; // c3
  double w; // st7
  double v13; // st7
  bool v14; // c0
  bool v15; // c3
  double z; // st7
  double v17; // st7
  bool v18; // c0
  bool v19; // c3
  double y; // st7
  float v22; // [esp-Ch] [ebp-6Ch] BYREF
  float v23; // [esp-8h] [ebp-68h]
  float v24; // [esp-4h] [ebp-64h]
  broad_phase_base bpb; // [esp+0h] [ebp-60h] BYREF
  float v26; // [esp+50h] [ebp-10h]
  unsigned int v27[3]; // [esp+54h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+60h] [ebp+0h]

  v27[0] = a1;
  v27[1] = retaddr;
  v22 = 1.0e30;
  v23 = 1.0e30;
  v24 = 1.0e30;
  bpb.m_trace_aabb_min_whace.y = -1.0e30;
  bpb.m_trace_aabb_min_whace.z = -1.0e30;
  bpb.m_trace_aabb_min_whace.w = -1.0e30;
  for ( i = _InterlockedExchangeAdd(&g_bpb_list_index, 1u);
        i < g_bpb_list_max_index;
        i = _InterlockedExchangeAdd(&g_bpb_list_index, 1u) )
  {
    process_cluster_environment_collision_prolog(
      (int)v27,
      (broad_phase_info *)g_bpb_ptr_list[i],
      (broad_phase_base *)&v22);
  }
  Input = (float **)pBatch->Input;
  tlAtomicMutex::Lock(&g_prolog_task_mutex);
  v4 = *Input;
  v5 = v24;
  if ( v24 >= (double)(*Input)[2] )
    v5 = v4[2];
  *(float *)&bpb.m_my_collision_type_flags = v5;
  v6 = v23;
  if ( v23 >= (double)v4[1] )
    v6 = v4[1];
  *((float *)&bpb.m_my_collision_type_flags + 1) = v6;
  v7 = v22;
  if ( v22 >= (double)*v4 )
    v7 = *v4;
  v26 = v7;
  *v4 = v26;
  v4[1] = *((float *)&bpb.m_my_collision_type_flags + 1);
  v4[2] = *(float *)&bpb.m_my_collision_type_flags;
  v8 = Input[1];
  v9 = v8[2];
  v10 = bpb.m_trace_aabb_min_whace.w < v9;
  v11 = bpb.m_trace_aabb_min_whace.w == v9;
  w = bpb.m_trace_aabb_min_whace.w;
  if ( v10 || v11 )
    w = v8[2];
  v26 = w;
  v13 = v8[1];
  v14 = bpb.m_trace_aabb_min_whace.z < v13;
  v15 = bpb.m_trace_aabb_min_whace.z == v13;
  z = bpb.m_trace_aabb_min_whace.z;
  if ( v14 || v15 )
    z = v8[1];
  *((float *)&bpb.m_my_collision_type_flags + 1) = z;
  v17 = *v8;
  v18 = bpb.m_trace_aabb_min_whace.y < v17;
  v19 = bpb.m_trace_aabb_min_whace.y == v17;
  y = bpb.m_trace_aabb_min_whace.y;
  if ( v18 || v19 )
    y = *v8;
  *(float *)&bpb.m_my_collision_type_flags = y;
  *v8 = *(float *)&bpb.m_my_collision_type_flags;
  v8[1] = *((float *)&bpb.m_my_collision_type_flags + 1);
  v8[2] = v26;
  if ( !--g_prolog_task_mutex.LockCount )
  {
    bpb.m_my_collision_type_flags = 0;
    InterlockedExchange((volatile int *)&bpb.m_my_collision_type_flags, 0);
    g_prolog_task_mutex.ThreadId = 0;
  }
  return 0;
}

void __cdecl merge_sort_bpb(broad_phase_base **list, int list_count)
{
  broad_phase_base *v2; // eax
  broad_phase_base **v3; // ebx
  broad_phase_base **i; // esi
  broad_phase_base *v5; // eax
  broad_phase_base **j; // edi
  broad_phase_base *v7; // eax
  broad_phase_base **last; // [esp+8h] [ebp-4h]

  if ( list_count >= 3 )
  {
    merge_sort_bpb(list, list_count / 2);
    merge_sort_bpb(&list[list_count / 2], list_count - list_count / 2);
    v3 = &list[list_count / 2];
    last = &list[list_count];
    for ( i = v3 - 1; v3 < last; ++v3 )
    {
      if ( !compare_bpb(*v3, *i) )
        break;
      v5 = *i;
      *i = *v3;
      *v3 = v5;
      for ( j = i - 1; i > list; --j )
      {
        if ( !compare_bpb(*i, *j) )
          break;
        v7 = *j;
        *j = *i;
        *i = v7;
        i = j;
      }
      i = v3;
    }
  }
  else if ( list_count == 2 )
  {
    if ( compare_bpb(list[1], *list) )
    {
      v2 = *list;
      *list = list[1];
      list[1] = v2;
    }
  }
}

void  broad_phase_process_object_environment_collision(int a1@<ebp>, bpi_environment_collision_info *eci)
{
  bpi_environment_collision_info *v2; // esi
  float v3; // eax
  broad_phase_base *m_bpb_i_start; // edi
  float v5; // ecx
  int m_bpb_count; // eax
  int v7; // eax
  broad_phase_base **v8; // edi
  int v9; // ecx
  broad_phase_base **v10; // edx
  broad_phase_base **i; // eax
  broad_phase_base *v12; // edi
  int v13; // eax
  broad_phase_base *x_low; // eax
  float y; // ecx
  float z; // edx
  int v17; // esi
  double v18; // st7
  double v19; // st7
  float w; // eax
  unsigned int v21; // ecx
  volatile unsigned int y_low; // edx
  double v23; // st7
  float v24; // eax
  float v25; // ecx
  broad_phase_base **m_env_collision_flags; // edx
  double v27; // st7
  double v28; // st7
  double v29; // st7
  broad_phase_base *m_list_bpb_next; // esi
  double v31; // st7
  double v32; // st7
  double v33; // st7
  double v34; // st7
  double v35; // st7
  double v36; // st7
  double v37; // st7
  double v38; // st6
  double v39; // st5
  double v40; // st4
  double v41; // st3
  double v42; // st2
  broad_phase_base **v43; // eax
  unsigned int v44; // edx
  double v45; // st1
  double v46; // st1
  broad_phase_base *v47; // edx
  unsigned int v48; // eax
  unsigned int v49; // ecx
  broad_phase_base *v50; // eax
  int v51; // esi
  double v52; // st7
  double v53; // st6
  double v54; // st5
  double v55; // st4
  double v56; // st3
  double v57; // st2
  broad_phase_base **v58; // ecx
  int v59; // [esp-40h] [ebp-180h]
  phys_vec3 v60; // [esp-1Ch] [ebp-15Ch] BYREF
  float v61; // [esp-Ch] [ebp-14Ch] BYREF
  float v62; // [esp-8h] [ebp-148h]
  float v63; // [esp-4h] [ebp-144h]
  phys_vec3 aabb2_max_new; // [esp+0h] [ebp-140h] BYREF
  phys_vec3 p2; // [esp+10h] [ebp-130h] BYREF
  phys_vec3 aabb2_min_new; // [esp+20h] [ebp-120h] BYREF
  phys_vec3 half_dims; // [esp+30h] [ebp-110h] BYREF
  float v68; // [esp+44h] [ebp-FCh] BYREF
  float v69; // [esp+48h] [ebp-F8h]
  float v70; // [esp+4Ch] [ebp-F4h]
  phys_vec3 p1; // [esp+50h] [ebp-F0h] BYREF
  float *p_y; // [esp+60h] [ebp-E0h]
  float v73; // [esp+64h] [ebp-DCh] BYREF
  broad_phase_prolog_task_input bppti; // [esp+68h] [ebp-D8h]
  phys_vec3 bp_aabb_min; // [esp+70h] [ebp-D0h] BYREF
  phys_vec3 bp_aabb_max; // [esp+80h] [ebp-C0h] BYREF
  phys_vec3 aabb2_max; // [esp+90h] [ebp-B0h] BYREF
  phys_vec3 aabb2_min; // [esp+A0h] [ebp-A0h] BYREF
  broad_phase_base *v79; // [esp+B0h] [ebp-90h]
  float bpb_i_aabb_min; // [esp+B4h] [ebp-8Ch]
  phys_transient_allocator transient_buffer; // [esp+B8h] [ebp-88h] BYREF
  phys_vec3 aabb1_max; // [esp+D0h] [ebp-70h] BYREF
  phys_vec3 aabb1_min; // [esp+E0h] [ebp-60h]
  int v84; // [esp+F0h] [ebp-50h]
  volatile unsigned int v85; // [esp+F4h] [ebp-4Ch]
  unsigned int v86; // [esp+F8h] [ebp-48h]
  int bpb_cluster_list_count; // [esp+FCh] [ebp-44h]
  float v88; // [esp+100h] [ebp-40h]
  float v89; // [esp+104h] [ebp-3Ch]
  broad_phase_base *bpb_cluster_list; // [esp+108h] [ebp-38h]
  broad_phase_base **bpb_ptr_list; // [esp+10Ch] [ebp-34h]
  broad_phase_base **bpb_ptr_cur; // [esp+110h] [ebp-30h]
  float v93; // [esp+114h] [ebp-2Ch]
  broad_phase_base *bpb_cluster_head; // [esp+118h] [ebp-28h]
  unsigned int env_collision_flags; // [esp+11Ch] [ebp-24h]
  float x; // [esp+120h] [ebp-20h]
  int *v97; // [esp+124h] [ebp-1Ch]
  _EXCEPTION_REGISTRATION_RECORD *ExceptionList; // [esp+128h] [ebp-18h]
  _UNKNOWN **v99; // [esp+12Ch] [ebp-14h]
  int v100; // [esp+130h] [ebp-10h]
  unsigned int v101[2]; // [esp+134h] [ebp-Ch] BYREF
  int v102; // [esp+13Ch] [ebp-4h] BYREF
  _UNKNOWN *retaddr; // [esp+140h] [ebp+0h]

  v101[0] = a1;
  v101[1] = retaddr;
  v100 = -1;
  v99 = &_ehhandler__broad_phase_process_object_environment_collision__YAXAAVbpi_environment_collision_info___Z;
  ExceptionList = NtCurrentTeb()->NtTib.ExceptionList;
  v97 = &v102;
  v2 = eci;
  if ( eci->m_bpb_count <= 0 && _tlAssert("source/phys_broad_phase.cpp", 1005, "eci.m_bpb_count > 0", &toastPopupTitle) )
    __debugbreak();
  aabb2_min.w = 0.0;
  v79 = 0;
  bpb_i_aabb_min = 0.0;
  transient_buffer.m_first_block = 0;
  transient_buffer.m_cur = (char *)1;
  transient_buffer.m_end = 0;
  v59 = 4 * eci->m_bpb_count;
  v100 = 0;
  v3 = COERCE_FLOAT(
         phys_transient_allocator::allocate(
           (phys_transient_allocator *)&aabb2_min.w,
           v59,
           4,
           0,
           "phys_transient_allocator out of memory."));
  m_bpb_i_start = eci->m_bpb_i_start;
  v5 = v3;
  v88 = v3;
  v89 = v3;
  if ( m_bpb_i_start != eci->m_bpb_i_end )
  {
    *(float *)&bpb_cluster_list = 0.0;
    do
    {
      if ( (int)bpb_cluster_list >> 2 >= eci->m_bpb_count )
      {
        if ( _tlAssert(
               "source/phys_broad_phase.cpp",
               1020,
               "bpb_ptr_cur - bpb_ptr_list < eci.m_bpb_count",
               &toastPopupTitle) )
        {
          __debugbreak();
        }
        v3 = v89;
      }
      bpb_cluster_list = (broad_phase_base *)((char *)bpb_cluster_list + 4);
      *(unsigned int *)LODWORD(v3) = m_bpb_i_start;
      m_bpb_i_start = m_bpb_i_start->m_list_bpb_next;
      LODWORD(v3) += 4;
      v89 = v3;
    }
    while ( m_bpb_i_start != eci->m_bpb_i_end );
    v5 = v88;
  }
  if ( (LODWORD(v3) - LODWORD(v5)) >> 2 != eci->m_bpb_count
    && _tlAssert("source/phys_broad_phase.cpp", 1027, "bpb_ptr_cur - bpb_ptr_list == eci.m_bpb_count", &toastPopupTitle) )
  {
    __debugbreak();
  }
  g_bpb_ptr_list = (broad_phase_base **)LODWORD(v88);
  v73 = 1.0e30;
  g_bpb_list_index = 0;
  *(float *)&bppti.m_aabb_min = 1.0e30;
  m_bpb_count = eci->m_bpb_count;
  *(float *)&bppti.m_aabb_max = 1.0e30;
  g_bpb_list_max_index = m_bpb_count;
  v7 = eci->m_bpb_count;
  bp_aabb_min.y = -1.0e30;
  bp_aabb_min.z = -1.0e30;
  LODWORD(p1.w) = &v73;
  bp_aabb_min.w = -1.0e30;
  p_y = &bp_aabb_min.y;
  phys_task_manager_process(&bp_env_jq_module1Module, &p1.w, v7);
  phys_task_manager_flush();
  *(float *)&v84 = bp_aabb_min.y - v73;
  *(float *)&bpb_ptr_cur = bp_aabb_min.z - *(float *)&bppti.m_aabb_min;
  *(float *)&bpb_cluster_list_count = bp_aabb_min.w - *(float *)&bppti.m_aabb_max;
  if ( *(float *)&bpb_ptr_cur > (double)*(float *)&v84 )
  {
    g_bpb_cluster_sort_axis = 1;
    if ( *(float *)&bpb_cluster_list_count <= (double)*(float *)&bpb_ptr_cur )
      goto LABEL_20;
  }
  else if ( *(float *)&bpb_cluster_list_count <= (double)*(float *)&v84 )
  {
    g_bpb_cluster_sort_axis = 0;
    goto LABEL_20;
  }
  g_bpb_cluster_sort_axis = 2;
LABEL_20:
  v8 = (broad_phase_base **)LODWORD(v88);
  merge_sort_bpb((broad_phase_base **)LODWORD(v88), eci->m_bpb_count);
  G_BPM->g_list_bpb = *v8;
  v9 = eci->m_bpb_count;
  eci->m_bpb_i_start = *v8;
  v10 = &v8[v9];
  for ( i = v8; i < v10; ++i )
    (*i)->m_list_bpb_next = i[1];
  (*(i - 1))->m_list_bpb_next = eci->m_bpb_i_end;
  v12 = eci->m_bpb_i_start;
  v13 = 0;
  *(float *)&bpb_cluster_list_count = 0.0;
  *(float *)&v84 = 0.0;
  if ( v12 != eci->m_bpb_i_end )
  {
    while ( (v12->m_flags & 0x10) != 0 )
    {
LABEL_73:
      v12 = v12->m_list_bpb_next;
      if ( v12 == v2->m_bpb_i_end )
      {
        v13 = v84;
        goto LABEL_75;
      }
    }
    if ( v12->m_list_bpb_cluster_next
      && _tlAssert("source/phys_broad_phase.cpp", 1093, "bpb_i->get_bpb_cluster_next() == NULL", &toastPopupTitle) )
    {
      __debugbreak();
    }
    v12->m_flags |= 0x10u;
    x_low = (broad_phase_base *)LODWORD(v12->m_trace_aabb_min_whace.x);
    y = v12->m_trace_aabb_min_whace.y;
    z = v12->m_trace_aabb_min_whace.z;
    *(float *)&bpb_ptr_cur = v12->m_trace_aabb_min_whace.x + v12->m_trace_translation.x;
    v17 = g_bpb_cluster_sort_axis;
    v18 = v12->m_trace_translation.y;
    LODWORD(aabb1_max.y) = x_low;
    v19 = v18 + v12->m_trace_aabb_min_whace.y;
    w = v12->m_trace_aabb_min_whace.w;
    aabb1_max.z = y;
    v21 = LODWORD(v12->m_trace_aabb_max_whace.x);
    *(float *)&bpb_ptr_list = v19;
    aabb1_max.w = z;
    y_low = LODWORD(v12->m_trace_aabb_max_whace.y);
    v23 = v12->m_trace_translation.z + v12->m_trace_aabb_min_whace.z;
    aabb1_min.x = w;
    v24 = v12->m_trace_aabb_max_whace.z;
    transient_buffer.m_total_memory_allocated = v21;
    x = v23;
    v25 = v12->m_trace_aabb_max_whace.w;
    transient_buffer.m_mutex.m_count = y_low;
    aabb2_max.y = *(float *)&bpb_ptr_cur;
    m_env_collision_flags = (broad_phase_base **)v12->m_env_collision_flags;
    *(float *)&transient_buffer.m_slot_pool = v24;
    aabb2_max.z = *(float *)&bpb_ptr_list;
    aabb1_max.x = v25;
    aabb2_max.w = x;
    *(float *)&bpb_ptr_cur = v12->m_trace_aabb_max_whace.x + v12->m_trace_translation.x;
    *(float *)&bpb_ptr_list = v12->m_trace_aabb_max_whace.y + v12->m_trace_translation.y;
    x = v12->m_trace_aabb_max_whace.z + v12->m_trace_translation.z;
    v27 = *(float *)&bpb_ptr_cur;
    bpb_ptr_cur = m_env_collision_flags;
    bp_aabb_max.y = v27;
    v28 = *(float *)&bpb_ptr_list;
    bpb_ptr_list = (broad_phase_base **)v12;
    bp_aabb_max.z = v28;
    bp_aabb_max.w = x;
    if ( (unsigned int)g_bpb_cluster_sort_axis > 2
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
           32,
           "i >= 0 && i < 3",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    v29 = *(&v12->m_trace_aabb_min_whace.x + v17);
    m_list_bpb_next = v12->m_list_bpb_next;
    for ( aabb2_min.z = v29; m_list_bpb_next != eci->m_bpb_i_end; m_list_bpb_next = m_list_bpb_next->m_list_bpb_next )
    {
      x = *(float *)&g_bpb_cluster_sort_axis;
      if ( (unsigned int)g_bpb_cluster_sort_axis > 2
        && _tlAssert(
             "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
             32,
             "i >= 0 && i < 3",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
      x = *(&m_list_bpb_next->m_trace_aabb_min_whace.x + LODWORD(x));
      if ( x - aabb2_min.z > 136.0 )
        break;
      if ( (m_list_bpb_next->m_flags & 0x10) == 0 )
      {
        if ( m_list_bpb_next->m_list_bpb_cluster_next
          && _tlAssert("source/phys_broad_phase.cpp", 1108, "bpb_j->get_bpb_cluster_next() == NULL", &toastPopupTitle) )
        {
          __debugbreak();
        }
        x = m_list_bpb_next->m_trace_aabb_min_whace.z;
        v31 = aabb1_max.w;
        if ( x < (double)aabb1_max.w )
          v31 = m_list_bpb_next->m_trace_aabb_min_whace.z;
        v89 = v31;
        x = m_list_bpb_next->m_trace_aabb_min_whace.y;
        v32 = aabb1_max.z;
        if ( x < (double)aabb1_max.z )
          v32 = m_list_bpb_next->m_trace_aabb_min_whace.y;
        *(float *)&bpb_cluster_list = v32;
        x = m_list_bpb_next->m_trace_aabb_min_whace.x;
        v33 = aabb1_max.y;
        if ( x < (double)aabb1_max.y )
          v33 = m_list_bpb_next->m_trace_aabb_min_whace.x;
        v88 = v33;
        x = m_list_bpb_next->m_trace_aabb_max_whace.z;
        v34 = *(float *)&transient_buffer.m_slot_pool;
        if ( x > (double)*(float *)&transient_buffer.m_slot_pool )
          v34 = m_list_bpb_next->m_trace_aabb_max_whace.z;
        x = v34;
        v85 = LODWORD(m_list_bpb_next->m_trace_aabb_max_whace.y);
        v35 = *(float *)&transient_buffer.m_mutex.m_count;
        if ( *(float *)&v85 > (double)*(float *)&transient_buffer.m_mutex.m_count )
          v35 = m_list_bpb_next->m_trace_aabb_max_whace.y;
        *(float *)&v85 = v35;
        v86 = LODWORD(m_list_bpb_next->m_trace_aabb_max_whace.x);
        v36 = *(float *)&transient_buffer.m_total_memory_allocated;
        if ( *(float *)&v86 > (double)*(float *)&transient_buffer.m_total_memory_allocated )
          v36 = m_list_bpb_next->m_trace_aabb_max_whace.x;
        *(float *)&v86 = v36;
        *(float *)&env_collision_flags = *(float *)&v86 - v88;
        v93 = *(float *)&v85 - *(float *)&bpb_cluster_list;
        *(float *)&bpb_cluster_head = x - v89;
        if ( *(float *)&env_collision_flags <= 136.0 && v93 <= 136.0 && *(float *)&bpb_cluster_head <= 136.0 )
        {
          *(float *)&bpb_cluster_head = m_list_bpb_next->m_trace_aabb_min_whace.x
                                      + m_list_bpb_next->m_trace_translation.x;
          v93 = m_list_bpb_next->m_trace_translation.y + m_list_bpb_next->m_trace_aabb_min_whace.y;
          *(float *)&env_collision_flags = m_list_bpb_next->m_trace_translation.z
                                         + m_list_bpb_next->m_trace_aabb_min_whace.z;
          v60.x = *(float *)&bpb_cluster_head;
          v60.y = v93;
          v60.z = *(float *)&env_collision_flags;
          phys_min((phys_vec3 *)&p2.y, (phys_vec3 *)&aabb2_max.y, &v60);
          *(float *)&bpb_cluster_head = m_list_bpb_next->m_trace_aabb_max_whace.x
                                      + m_list_bpb_next->m_trace_translation.x;
          v93 = m_list_bpb_next->m_trace_aabb_max_whace.y + m_list_bpb_next->m_trace_translation.y;
          *(float *)&env_collision_flags = m_list_bpb_next->m_trace_aabb_max_whace.z
                                         + m_list_bpb_next->m_trace_translation.z;
          half_dims.y = *(float *)&bpb_cluster_head;
          half_dims.z = v93;
          half_dims.w = *(float *)&env_collision_flags;
          phys_max((const phys_vec3 *)&v61, (phys_vec3 *)&bp_aabb_max.y, (phys_vec3 *)&half_dims.y);
          v37 = v61;
          v38 = p2.y;
          *(float *)&bpb_cluster_head = v61 - p2.y;
          v39 = v62;
          v40 = p2.z;
          v93 = v62 - p2.z;
          v41 = v63;
          v42 = p2.w;
          *(float *)&env_collision_flags = v63 - p2.w;
          if ( *(float *)&bpb_cluster_head <= 136.0 && v93 <= 136.0 && *(float *)&env_collision_flags <= 136.0 )
          {
            v43 = bpb_ptr_list;
            aabb1_max.y = v88;
            v44 = m_list_bpb_next->m_env_collision_flags;
            v45 = *(float *)&bpb_cluster_list;
            m_list_bpb_next->m_flags |= 0x10u;
            aabb1_max.z = v45;
            bpb_ptr_cur = (broad_phase_base **)(v44 | (unsigned int)bpb_ptr_cur);
            v46 = v89;
            m_list_bpb_next->m_list_bpb_cluster_next = (broad_phase_base *)v43;
            aabb1_max.w = v46;
            bpb_ptr_list = (broad_phase_base **)m_list_bpb_next;
            transient_buffer.m_total_memory_allocated = v86;
            transient_buffer.m_mutex.m_count = v85;
            *(float *)&transient_buffer.m_slot_pool = x;
            aabb2_max.y = v38;
            aabb2_max.z = v40;
            aabb2_max.w = v42;
            bp_aabb_max.y = v37;
            bp_aabb_max.z = v39;
            bp_aabb_max.w = v41;
          }
        }
      }
    }
    transient_allocator_update_largest_size(80);
    v47 = v79;
    v48 = ((unsigned int)&v79->m_trace_aabb_min_whace.w + 3) & 0xFFFFFFF0;
    if ( v48 + 80 <= LODWORD(bpb_i_aabb_min) )
    {
      v79 = (broad_phase_base *)(v48 + 80);
      v49 = ((unsigned int)&v47->m_trace_aabb_min_whace.w + 3) & 0xFFFFFFF0;
      if ( v48 )
        goto LABEL_70;
    }
    phys_transient_allocator::resize((phys_transient_allocator *)&aabb2_min.w);
    v50 = (broad_phase_base *)(((unsigned int)&v79->m_trace_aabb_min_whace.w + 3) & 0xFFFFFFF0);
    if ( (unsigned int)&v50[1] <= LODWORD(bpb_i_aabb_min) )
    {
      v79 = v50 + 1;
      x = *(float *)&v50;
      if ( *(float *)&v50 != 0.0 )
      {
LABEL_69:
        *(float *)&v49 = x;
LABEL_70:
        v51 = v49;
LABEL_72:
        ++v84;
        *(float *)(v51 + 52) = *(float *)&bpb_cluster_list_count;
        bpb_cluster_list_count = v51;
        comp_trace_volume(
          (int)v101,
          (phys_vec3 *)&aabb1_max.y,
          (const phys_vec3 *)&transient_buffer.m_total_memory_allocated,
          (phys_vec3 *)&aabb2_max.y,
          (phys_vec3 *)&bp_aabb_max.y,
          (phys_vec3 *)&v68,
          (phys_vec3 *)&aabb2_max_new.y,
          (phys_vec3 *)&aabb2_min_new.y);
        v52 = v68;
        v53 = aabb2_min_new.y;
        *(float *)&bpb_cluster_head = v68 - aabb2_min_new.y;
        v54 = v69;
        v55 = aabb2_min_new.z;
        v93 = v69 - aabb2_min_new.z;
        v56 = v70;
        v57 = aabb2_min_new.w;
        *(float *)&env_collision_flags = v70 - aabb2_min_new.w;
        *(float *)v51 = *(float *)&bpb_cluster_head;
        *(float *)(v51 + 4) = v93;
        *(float *)(v51 + 8) = *(float *)&env_collision_flags;
        *(float *)&bpb_cluster_head = v53 + v52;
        v93 = v55 + v54;
        *(float *)&env_collision_flags = v57 + v56;
        *(float *)(v51 + 16) = *(float *)&bpb_cluster_head;
        *(float *)(v51 + 20) = v93;
        *(float *)(v51 + 24) = *(float *)&env_collision_flags;
        *(float *)&bpb_cluster_head = aabb2_max_new.y - v52;
        v93 = aabb2_max_new.z - v54;
        *(float *)&env_collision_flags = aabb2_max_new.w - v56;
        *(float *)(v51 + 32) = *(float *)&bpb_cluster_head;
        *(float *)(v51 + 36) = v93;
        *(float *)(v51 + 40) = *(float *)&env_collision_flags;
        v58 = bpb_ptr_cur;
        *(float *)(v51 + 56) = *(float *)&bpb_ptr_list;
        *(unsigned int *)(v51 + 68) = v58;
        v2 = eci;
        goto LABEL_73;
      }
    }
    else
    {
      x = 0.0;
    }
    if ( _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_transient_allocator.h",
           79,
           "ptr",
           "transient allocation too large, increase block_size.") )
    {
      __debugbreak();
    }
    if ( _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_transient_allocator.h",
           81,
           "ptr || no_error",
           "phys_transient_allocator out of memory.") )
    {
      __debugbreak();
    }
    if ( x == 0.0 )
    {
      *(float *)&v51 = 0.0;
      goto LABEL_72;
    }
    goto LABEL_69;
  }
LABEL_75:
  g_bpb_list_cur = (broad_phase_base *)bpb_cluster_list_count;
  g_bpb_list_index = 0;
  g_bpb_list_max_index = v13;
  g_thread_id = 0;
  phys_task_manager_process(&bp_env_jq_module2Module, 0, v13);
  phys_task_manager_flush();
  phys_transient_allocator::reset((phys_transient_allocator *)&aabb2_min.w);
  v100 = -1;
  if ( LODWORD(aabb2_min.w) )
  {
    if ( _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_transient_allocator.h",
           69,
           "m_first_block == NULL",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
  }
}

void __cdecl broad_phase_process_object_environment_collision()
{
  int g_list_bpb_count; // eax
  int v1; // eax
  bpi_environment_collision_info eci; // [esp+0h] [ebp-10h] BYREF
  int savedregs; // [esp+10h] [ebp+0h] BYREF

  g_list_bpb_count = G_BPM->g_list_bpb_count;
  eci.m_bpb_i_start = G_BPM->g_list_bpb;
  eci.m_bpb_i_end = 0;
  eci.m_bpb_count = g_list_bpb_count;
  for ( eci.m_bpb_last_count = g_list_bpb_count; eci.m_bpb_count; eci.m_bpb_last_count = v1 )
  {
    broad_phase_process_object_environment_collision((int)&savedregs, &eci);
    eci.m_bpb_i_end = eci.m_bpb_i_start;
    v1 = G_BPM->g_list_bpb_count;
    eci.m_bpb_i_start = G_BPM->g_list_bpb;
    eci.m_bpb_count = v1 - eci.m_bpb_last_count;
  }
}

void __thiscall axis_aligned_sweep_and_prune::swap(
        axis_aligned_sweep_and_prune *this,
        axis_aligned_sweep_and_prune::axis_element **a1_ptr,
        axis_aligned_sweep_and_prune::axis_element **a2_ptr)
{
  axis_aligned_sweep_and_prune::axis_element *v3; // esi
  axis_aligned_sweep_and_prune::axis_element *v4; // edi
  axis_aligned_sweep_and_prune::sap_node *m_node; // ebx
  bool (__cdecl *m_should_collide_callback)(const broad_phase_base *, const broad_phase_base *); // eax

  v3 = *a1_ptr;
  v4 = *a2_ptr;
  if ( (*a1_ptr)->m_node == (*a2_ptr)->m_node
    && _tlAssert("source/phys_broad_phase.cpp", 570, "a1->m_node != a2->m_node", &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( v3->m_ae_list_index + 1 != v4->m_ae_list_index
    && _tlAssert("source/phys_broad_phase.cpp", 571, "a1->m_ae_list_index + 1 == a2->m_ae_list_index", &toastPopupTitle) )
  {
    __debugbreak();
  }
  ++v3->m_ae_list_index;
  --v4->m_ae_list_index;
  if ( v3->m_min_max == 1 && !v4->m_min_max )
  {
    m_node = v4->m_node;
    if ( axis_aligned_sweep_and_prune::are_overlapping(this, v3->m_node, v4->m_node) )
    {
      m_should_collide_callback = this->m_should_collide_callback;
      if ( !m_should_collide_callback || m_should_collide_callback(v3->m_node->m_bpb, m_node->m_bpb) )
        axis_aligned_sweep_and_prune::add_active_pair(this, v3->m_node, v4->m_node);
    }
  }
  *a1_ptr = v4;
  *a2_ptr = v3;
}

void __thiscall axis_aligned_sweep_and_prune::merge_sort(
        axis_aligned_sweep_and_prune *this,
        axis_aligned_sweep_and_prune::axis_element **list,
        int list_count)
{
  axis_aligned_sweep_and_prune *v3; // esi
  axis_aligned_sweep_and_prune::axis_element **v4; // ebx
  axis_aligned_sweep_and_prune::axis_element **v5; // edi
  axis_aligned_sweep_and_prune::axis_element **v6; // ecx
  axis_aligned_sweep_and_prune::axis_element **i; // esi
  axis_aligned_sweep_and_prune::axis_element **last; // [esp+8h] [ebp-8h]

  v3 = this;
  if ( list_count >= 3 )
  {
    axis_aligned_sweep_and_prune::merge_sort(this, list, list_count / 2);
    axis_aligned_sweep_and_prune::merge_sort(v3, &list[list_count / 2], list_count - list_count / 2);
    v4 = &list[list_count / 2];
    last = &list[list_count];
    v5 = v4 - 1;
    if ( v4 < last )
    {
      while ( (*v5)->m_val > (double)(*v4)->m_val )
      {
        axis_aligned_sweep_and_prune::swap(v3, v5, v4);
        v6 = v5;
        for ( i = v5 - 1; v6 > list; --i )
        {
          if ( (*i)->m_val <= (double)(*v6)->m_val )
            break;
          axis_aligned_sweep_and_prune::swap(this, i, v6);
          v6 = i;
        }
        v5 = v4++;
        if ( v4 >= last )
          break;
        v3 = this;
      }
    }
  }
  else if ( list_count == 2 && (*list)->m_val > (double)list[1]->m_val )
  {
    axis_aligned_sweep_and_prune::swap(this, list, list + 1);
  }
}

void __cdecl aasap_list_add(broad_phase_base *bpb)
{
  axis_aligned_sweep_and_prune::create_sap_node(g_axis_aligned_sweep_and_prune, bpb);
}

void __cdecl aasap_list_remove(broad_phase_base *bpb)
{
  axis_aligned_sweep_and_prune::destroy_sap_node(g_axis_aligned_sweep_and_prune, bpb);
}

void  do_initial_tunnel_test(
        float a1@<ebp>,
        broad_phase_group *bpg,
        const broad_phase_environement_query_results *bpeqr)
{
  broad_phase_info *m_list_bpi_head; // esi
  const phys_gjk_geom *m_gjk_geom; // ecx
  phys_gjk_geom_vtbl *v5; // eax
  double v6; // st7
  broad_phase_base_list::node *v7; // eax
  broad_phase_base *m_bpb; // edi
  int v9; // [esp+14h] [ebp-43Ch] BYREF
  phys_gjk_info gjk_info; // [esp+20h] [ebp-430h] BYREF
  phys_gjk_input pgi; // [esp+3C0h] [ebp-90h] BYREF
  unsigned int v12[3]; // [esp+418h] [ebp-38h] BYREF
  phys_collision_pair pcp; // [esp+424h] [ebp-2Ch]
  float reduced_radius; // [esp+438h] [ebp-18h] BYREF
  broad_phase_base_list::node *last_bpi_env_iter; // [esp+43Ch] [ebp-14h]
  float lambda; // [esp+440h] [ebp-10h]
  float hit_time; // [esp+444h] [ebp-Ch] BYREF
  broad_phase_base_list::node *bpi_env_iter; // [esp+448h] [ebp-8h]
  broad_phase_base_list::node *retaddr; // [esp+450h] [ebp+0h]

  hit_time = a1;
  bpi_env_iter = retaddr;
  if ( (bpg->m_flags & 0x200) == 0
    && _tlAssert(
         "source/phys_broad_phase.cpp",
         753,
         "bpg->get_flag(broad_phase_group::FLAG_DO_INITIAL_TUNNEL_TEST)",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  bpg->m_flags &= ~0x200u;
  m_list_bpi_head = bpg->m_list_bpi_head;
  *(float *)&pcp.m_next_link = -1.0;
  for ( reduced_radius = 0.0; m_list_bpi_head; m_list_bpi_head = (broad_phase_info *)m_list_bpi_head->m_list_bpb_next )
  {
    m_gjk_geom = m_list_bpi_head->m_gjk_geom;
    v5 = m_gjk_geom->__vftable;
    lambda = 1.0;
    v6 = ((double (__thiscall *)(const phys_gjk_geom *))v5->get_geom_radius)(m_gjk_geom);
    v7 = *bpeqr->m_list_bpi_env.m_list_cur;
    last_bpi_env_iter = bpeqr->m_list_bpi_env.m_list;
    *(float *)&pcp.m_bpi2 = v6 * 0.8999999761581421 + 0.3400000035762787;
    LODWORD(pcp.m_hit_time) = v7;
    if ( last_bpi_env_iter != v7 )
    {
      do
      {
        m_bpb = last_bpi_env_iter->m_bpb;
        if ( (last_bpi_env_iter->m_bpb->m_env_collision_flags & m_list_bpi_head->m_env_collision_flags) != 0 )
        {
          pgi.m_sep_thresh = m_list_bpi_head->m_trace_translation.x - m_bpb->m_trace_translation.x;
          *(float *)&pgi.m_intersection_test_only = m_list_bpi_head->m_trace_translation.y
                                                  - m_bpb->m_trace_translation.y;
          *(float *)(&pgi.m_continuous_collision + 3) = m_list_bpi_head->m_trace_translation.z
                                                      - m_bpb->m_trace_translation.z;
          if ( phys_are_potentially_colliding(
                 &m_list_bpi_head->m_trace_aabb_min_whace,
                 &m_list_bpi_head->m_trace_aabb_max_whace,
                 (const phys_vec3 *)&pgi.m_sep_thresh,
                 &m_bpb->m_trace_aabb_min_whace,
                 &m_bpb->m_trace_aabb_max_whace,
                 &reduced_radius) )
          {
            *(float *)&pcp.m_next_link = reduced_radius;
            v12[1] = m_list_bpi_head;
            v12[2] = m_bpb;
            pcp.m_bpi1 = 0;
            setup_gjk_input_from_pcp((phys_gjk_input *)&gjk_info.m_set_list[15].m_candidate, (phys_collision_pair *)v12);
            pgi.cg1_to_world_xform = (const phys_mat44 *)pcp.m_bpi2;
            *(float *)&pgi.gjk_ci = 1.02;
            LOWORD(pgi.cg1_radius) = 256;
            if ( phys_gjk_info::phys_collide_do_gjk_collide(
                   (phys_gjk_info *)&v9,
                   (int)&hit_time,
                   (const phys_gjk_input *)&gjk_info.m_set_list[15].m_candidate) )
            {
              *(float *)&pcp.m_gjk_ci = gjk_info.cg1_cinfo_loc.m_p2.z * gjk_info.cg2_to_cg1_xform.w.z
                                      + gjk_info.cg1_cinfo_loc.m_p2.y * gjk_info.cg2_to_cg1_xform.w.y
                                      + gjk_info.cg1_cinfo_loc.m_p2.w * gjk_info.cg2_to_cg1_xform.w.w;
              if ( *(float *)&pcp.m_gjk_ci < 0.0 && lambda > (double)gjk_info.m_cg1_relative_translation_loc.y )
                lambda = gjk_info.m_cg1_relative_translation_loc.y;
            }
          }
        }
        last_bpi_env_iter = last_bpi_env_iter->m_next;
      }
      while ( last_bpi_env_iter != (broad_phase_base_list::node *)LODWORD(pcp.m_hit_time) );
    }
    rigid_body::adjust_col_moved_vec(m_list_bpi_head->m_rb, lambda);
    broad_phase_info::collision_prolog(m_list_bpi_head);
  }
}

broad_phase_info *__cdecl create_broad_phase_info()
{
  phys_free_list<broad_phase_info> *p_g_list_broad_phase_info; // edi
  phys_free_list<broad_phase_info>::T_internal *v1; // eax
  phys_free_list<broad_phase_info>::T_internal *v2; // esi

  p_g_list_broad_phase_info = &G_BPM->g_list_broad_phase_info;
  v1 = (phys_free_list<broad_phase_info>::T_internal *)PMM_ALLOC(0x90u, 0x10u);
  v2 = v1;
  if ( v1 )
  {
    v1->m_prev_T_internal = &p_g_list_broad_phase_info->m_dummy_head;
    v1->m_next_T_internal = p_g_list_broad_phase_info->m_dummy_head.m_next_T_internal;
    p_g_list_broad_phase_info->m_dummy_head.m_next_T_internal->m_prev_T_internal = v1;
    ++p_g_list_broad_phase_info->m_list_count;
    p_g_list_broad_phase_info->m_dummy_head.m_next_T_internal = v1;
    phys_free_list<broad_phase_info>::debug_add(p_g_list_broad_phase_info, v1);
    return &v2->m_data;
  }
  else
  {
    if ( _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
           470,
           "no_error",
           "phys_free_list error: out of memory.") )
    {
      __debugbreak();
    }
    return 0;
  }
}

void __cdecl destroy_broad_phase_info(broad_phase_info *bpi)
{
  phys_free_list<broad_phase_info> *p_g_list_broad_phase_info; // edi

  environment_collision_list_remove(bpi);
  axis_aligned_sweep_and_prune::destroy_sap_node(g_axis_aligned_sweep_and_prune, bpi);
  p_g_list_broad_phase_info = &G_BPM->g_list_broad_phase_info;
  if ( bpi )
  {
    PMM_VALIDATE((char *)&bpi[-1].m_gjk_geom, 0x90u, 0x10u);
    phys_free_list<broad_phase_info>::remove(
      p_g_list_broad_phase_info,
      (phys_free_list<broad_phase_info>::T_internal *)&bpi[-1].m_gjk_geom);
  }
}

void __cdecl destroy_broad_phase_info_list(broad_phase_info *list_bpi)
{
  broad_phase_info *v1; // esi
  broad_phase_info *m_list_bpb_next; // ebx
  char *p_m_gjk_geom; // esi
  phys_free_list<broad_phase_info> *p_g_list_broad_phase_info; // edi
  unsigned int *v5; // eax
  int v6; // ecx

  v1 = list_bpi;
  if ( list_bpi )
  {
    do
    {
      if ( (v1->m_flags & 0x20) != 0
        && _tlAssert(
             "source/phys_broad_phase.cpp",
             130,
             "!bpi->get_flag(broad_phase_base::FLAG_ON_ENV_LIST)",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
      if ( v1->m_sap_node && _tlAssert("source/phys_broad_phase.cpp", 131, "bpi->m_sap_node == NULL", &toastPopupTitle) )
        __debugbreak();
      m_list_bpb_next = (broad_phase_info *)v1->m_list_bpb_next;
      p_m_gjk_geom = (char *)&v1[-1].m_gjk_geom;
      p_g_list_broad_phase_info = &G_BPM->g_list_broad_phase_info;
      PMM_VALIDATE(p_m_gjk_geom, 0x90u, 0x10u);
      if ( !p_m_gjk_geom )
      {
        if ( _tlAssert("c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h", 477, "data", &toastPopupTitle) )
          __debugbreak();
      }
      --p_g_list_broad_phase_info->m_list_count;
      phys_free_list<broad_phase_info>::debug_remove(
        p_g_list_broad_phase_info,
        (phys_free_list<broad_phase_info>::T_internal *)p_m_gjk_geom);
      v5 = (unsigned int *)*((unsigned int *)p_m_gjk_geom + 1);
      v6 = *(unsigned int *)p_m_gjk_geom;
      *(unsigned int *)(v6 + 4) = v5;
      *v5 = v6;
      PMM_FREE((unsigned __int8 *)p_m_gjk_geom, 0x90u, 0x10u);
      v1 = m_list_bpb_next;
    }
    while ( m_list_bpb_next );
  }
}

broad_phase_collision_pair *__cdecl create_broad_phase_collision_pair()
{
  phys_free_list<broad_phase_collision_pair> *p_g_list_broad_phase_collision_pair; // edi
  phys_free_list<broad_phase_collision_pair>::T_internal *v1; // eax
  phys_free_list<broad_phase_collision_pair>::T_internal *v2; // esi

  p_g_list_broad_phase_collision_pair = &G_BPM->g_list_broad_phase_collision_pair;
  v1 = (phys_free_list<broad_phase_collision_pair>::T_internal *)PMM_ALLOC(0x18u, 4u);
  v2 = v1;
  if ( v1 )
  {
    v1->m_prev_T_internal = &p_g_list_broad_phase_collision_pair->m_dummy_head;
    v1->m_next_T_internal = p_g_list_broad_phase_collision_pair->m_dummy_head.m_next_T_internal;
    p_g_list_broad_phase_collision_pair->m_dummy_head.m_next_T_internal->m_prev_T_internal = v1;
    ++p_g_list_broad_phase_collision_pair->m_list_count;
    p_g_list_broad_phase_collision_pair->m_dummy_head.m_next_T_internal = v1;
    phys_free_list<broad_phase_collision_pair>::debug_add(p_g_list_broad_phase_collision_pair, v1);
    return &v2->m_data;
  }
  else
  {
    if ( _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
           470,
           "no_error",
           "phys_free_list error: out of memory.") )
    {
      __debugbreak();
    }
    return 0;
  }
}

void __cdecl destroy_broad_phase_collision_pair(broad_phase_collision_pair *bpcp)
{
  phys_free_list<broad_phase_collision_pair> *p_g_list_broad_phase_collision_pair; // edi

  p_g_list_broad_phase_collision_pair = &G_BPM->g_list_broad_phase_collision_pair;
  if ( bpcp )
  {
    PMM_VALIDATE((char *)&bpcp[-1].m_bpi2, 0x18u, 4u);
    phys_free_list<broad_phase_collision_pair>::remove(
      p_g_list_broad_phase_collision_pair,
      (phys_free_list<broad_phase_collision_pair>::T_internal *)&bpcp[-1].m_bpi2);
  }
}

void __cdecl destroy_broad_phase_collision_pair_list(broad_phase_collision_pair *list_bpcp)
{
  broad_phase_collision_pair *v1; // eax
  broad_phase_collision_pair *m_next_bpcp; // ebx
  phys_free_list<broad_phase_collision_pair>::T_internal *p_m_bpi2; // esi
  phys_free_list<broad_phase_collision_pair> *p_g_list_broad_phase_collision_pair; // edi
  phys_free_list<broad_phase_collision_pair>::T_internal_base *m_next_T_internal; // eax
  phys_free_list<broad_phase_collision_pair>::T_internal_base *m_prev_T_internal; // ecx

  v1 = list_bpcp;
  if ( list_bpcp )
  {
    do
    {
      m_next_bpcp = v1->m_next_bpcp;
      p_m_bpi2 = (phys_free_list<broad_phase_collision_pair>::T_internal *)&v1[-1].m_bpi2;
      p_g_list_broad_phase_collision_pair = &G_BPM->g_list_broad_phase_collision_pair;
      PMM_VALIDATE((char *)&v1[-1].m_bpi2, 0x18u, 4u);
      if ( !p_m_bpi2 )
      {
        if ( _tlAssert("c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h", 477, "data", &toastPopupTitle) )
          __debugbreak();
      }
      --p_g_list_broad_phase_collision_pair->m_list_count;
      phys_free_list<broad_phase_collision_pair>::debug_remove(p_g_list_broad_phase_collision_pair, p_m_bpi2);
      m_next_T_internal = p_m_bpi2->m_next_T_internal;
      m_prev_T_internal = p_m_bpi2->m_prev_T_internal;
      m_prev_T_internal->m_next_T_internal = m_next_T_internal;
      m_next_T_internal->m_prev_T_internal = m_prev_T_internal;
      PMM_FREE((unsigned __int8 *)p_m_bpi2, 0x18u, 4u);
      v1 = m_next_bpcp;
    }
    while ( m_next_bpcp );
  }
}

broad_phase_group *__cdecl create_broad_phase_group()
{
  phys_free_list<broad_phase_group> *p_g_list_broad_phase_group; // edi
  phys_free_list<broad_phase_group>::T_internal *v1; // eax
  phys_free_list<broad_phase_group>::T_internal *v2; // esi

  p_g_list_broad_phase_group = &G_BPM->g_list_broad_phase_group;
  v1 = (phys_free_list<broad_phase_group>::T_internal *)PMM_ALLOC(0x80u, 0x10u);
  v2 = v1;
  if ( v1 )
  {
    v1->m_prev_T_internal = &p_g_list_broad_phase_group->m_dummy_head;
    v1->m_next_T_internal = p_g_list_broad_phase_group->m_dummy_head.m_next_T_internal;
    p_g_list_broad_phase_group->m_dummy_head.m_next_T_internal->m_prev_T_internal = v1;
    ++p_g_list_broad_phase_group->m_list_count;
    p_g_list_broad_phase_group->m_dummy_head.m_next_T_internal = v1;
    phys_free_list<broad_phase_group>::debug_add(p_g_list_broad_phase_group, v1);
    return &v2->m_data;
  }
  else
  {
    if ( _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
           470,
           "no_error",
           "phys_free_list error: out of memory.") )
    {
      __debugbreak();
    }
    return 0;
  }
}

void __cdecl destroy_broad_phase_group(broad_phase_group *bpg)
{
  phys_free_list<broad_phase_group> *p_g_list_broad_phase_group; // edi

  environment_collision_list_remove(bpg);
  axis_aligned_sweep_and_prune::destroy_sap_node(g_axis_aligned_sweep_and_prune, bpg);
  p_g_list_broad_phase_group = &G_BPM->g_list_broad_phase_group;
  if ( bpg )
  {
    PMM_VALIDATE((char *)&bpg[-1].m_list_bpi_head, 0x80u, 0x10u);
    phys_free_list<broad_phase_group>::remove(
      p_g_list_broad_phase_group,
      (phys_free_list<broad_phase_group>::T_internal *)&bpg[-1].m_list_bpi_head);
  }
}

char  bpi_do_gjk_intersect@<al>(
        phys_gjk_cache_info *a1@<ebp>,
        broad_phase_info *p1,
        broad_phase_info *p2,
        phys_collision_pair *hit_time)
{
  float v4; // ecx
  int v6; // [esp-Ch] [ebp-41Ch] BYREF
  phys_gjk_info g_phys_gjk_info; // [esp+0h] [ebp-410h] BYREF
  phys_gjk_input pgi; // [esp+3A0h] [ebp-70h] BYREF
  broad_phase_info *v9; // [esp+3F0h] [ebp-20h]
  phys_collision_pair pcp; // [esp+3F4h] [ebp-1Ch] BYREF
  unsigned int id1; // [esp+408h] [ebp-8h]
  unsigned int retaddr; // [esp+410h] [ebp+0h]

  pcp.m_gjk_ci = a1;
  id1 = retaddr;
  if ( (*(float *)&hit_time < 0.0 || *(float *)&hit_time > 1.0)
    && _tlAssert("source/phys_broad_phase.cpp", 81, "hit_time >= 0.0f && hit_time <= 1.0f", &toastPopupTitle) )
  {
    __debugbreak();
  }
  v4 = *(float *)&p2->m_gjk_geom_id;
  pcp.m_bpi2 = (broad_phase_info *)p1->m_gjk_geom_id;
  pcp.m_hit_time = v4;
  if ( p1 == p2 && _tlAssert("source/phys_broad_phase.cpp", 86, "p1 && p2 && p1 != p2", &toastPopupTitle) )
    __debugbreak();
  if ( pcp.m_bpi2 == (broad_phase_info *)LODWORD(pcp.m_hit_time)
    && _tlAssert("source/phys_broad_phase.cpp", 87, "id1 != id2", &toastPopupTitle) )
  {
    __debugbreak();
  }
  pcp.m_next_link = hit_time;
  *(unsigned int *)(&pgi.m_continuous_collision + 3) = p1;
  v9 = p2;
  pcp.m_bpi1 = (broad_phase_info *)phys_heap_gjk_cache_system_avl_tree::get_gjk_cache_info_mutex(
                                     &G_BPM->g_phys_gjk_cache_system,
                                     (unsigned int)pcp.m_bpi2,
                                     LODWORD(pcp.m_hit_time),
                                     &G_BPM->g_bp_gjk_cache_mutex,
                                     1);
  setup_gjk_input_from_pcp(
    (phys_gjk_input *)&g_phys_gjk_info.m_set_list[15].m_candidate,
    (phys_collision_pair *)&pgi.m_intersection_test_only);
  *(float *)&pgi.gjk_ci = 1.02;
  LOWORD(pgi.cg1_radius) = 257;
  return phys_gjk_info::phys_collide_do_gjk_collide(
           (phys_gjk_info *)&v6,
           (int)&pcp.m_gjk_ci,
           (const phys_gjk_input *)&g_phys_gjk_info.m_set_list[15].m_candidate);
}

void __cdecl collide_bpi_environment(float bpi, broad_phase_base_list::node *bpeqr)
{
  broad_phase_info *v2; // ebx
  broad_phase_base_list::node *m_bpb; // eax
  broad_phase_base_list::node *v4; // esi
  broad_phase_base_list::node *m_next; // edi
  broad_phase_base_list::node *last_bpi_env_iter; // [esp+14h] [ebp-4h]
  int savedregs; // [esp+18h] [ebp+0h] BYREF

  v2 = (broad_phase_info *)LODWORD(bpi);
  if ( ((int)bpeqr[2].m_bpb & *(unsigned int *)(LODWORD(bpi) + 68)) != 0 )
  {
    m_bpb = (broad_phase_base_list::node *)bpeqr->m_next->m_bpb;
    bpeqr = (broad_phase_base_list::node *)bpeqr->m_bpb;
    for ( last_bpi_env_iter = m_bpb; bpeqr != last_bpi_env_iter; bpeqr = bpeqr->m_next )
    {
      v4 = (broad_phase_base_list::node *)bpeqr->m_bpb;
      bpi = 0.0;
      if ( ((int)v4[8].m_next & v2->m_env_collision_flags) != 0
        && phys_are_potentially_colliding_whace<broad_phase_info,broad_phase_info>(
             COERCE_FLOAT(&savedregs),
             v2,
             (broad_phase_info *)v4,
             &bpi) )
      {
        if ( SLOBYTE(v4[6].m_bpb) >= 0 )
        {
          add_collision_pair_mutex(v2, (broad_phase_info *)v4, bpi, 0);
        }
        else
        {
          m_next = v4[7].m_next;
          if ( !COERCE_UNSIGNED_INT8_THISCALL_BROAD_PHASE_BASE_LIST_NODE_(m_next->m_bpb->m_trace_aabb_min_whace.x)(m_next)
            && bpi_do_gjk_intersect(
                 (phys_gjk_cache_info *)&savedregs,
                 v2,
                 (broad_phase_info *)v4,
                 (phys_collision_pair *)LODWORD(bpi)) )
          {
            tlAtomicMutex::Lock(&G_BPM->g_bp_auto_activate_mutex);
            if ( !COERCE_UNSIGNED_INT8_THISCALL_BROAD_PHASE_BASE_LIST_NODE_(m_next->m_bpb->m_trace_aabb_min_whace.x)(m_next) )
              ((void (__thiscall *)(broad_phase_base_list::node *, broad_phase_info *))LODWORD(m_next->m_bpb->m_trace_aabb_min_whace.y))(
                m_next,
                v2);
            tlAtomicMutex::Unlock(&G_BPM->g_bp_auto_activate_mutex);
          }
        }
      }
    }
  }
}

void  collide_bpg_environment(
        broad_phase_base_list::node *a1@<ebp>,
        broad_phase_info *bpg,
        const broad_phase_environement_query_results *bpeqr)
{
  broad_phase_info *v3; // edi
  broad_phase_base_list::node *v4; // ecx
  broad_phase_base *m_bpb; // esi
  broad_phase_info *m_rb; // edi
  phys_wheel_collide_info *m_sap_node; // ecx
  broad_phase_memory *v8; // eax
  phys_wheel_collide_info *v9; // eax
  phys_wheel_collide_info *m_cg_to_rb_xform; // edi
  phys_wheel_collide_info *p_z; // eax
  phys_vec3 v12; // [esp+18h] [ebp-4Ch] BYREF
  phys_vec3 v13; // [esp+28h] [ebp-3Ch] BYREF
  broad_phase_base_list::node *i; // [esp+40h] [ebp-24h]
  phys_wheel_collide_info *v15; // [esp+44h] [ebp-20h]
  int v16; // [esp+48h] [ebp-1Ch] BYREF
  broad_phase_base_list::node *last_bpi_env_iter; // [esp+4Ch] [ebp-18h]
  phys_wheel_collide_info *last_wci; // [esp+50h] [ebp-14h]
  float v19; // [esp+54h] [ebp-10h] BYREF
  broad_phase_base_list::node *bpi_env_iter; // [esp+58h] [ebp-Ch] BYREF
  phys_auto_activate_callback *aac; // [esp+5Ch] [ebp-8h]
  phys_auto_activate_callback *retaddr; // [esp+64h] [ebp+0h]

  bpi_env_iter = a1;
  aac = retaddr;
  v19 = 0.0;
  v3 = bpg;
  if ( (bpeqr->m_env_collision_flags & bpg->m_env_collision_flags) != 0 )
  {
    if ( (bpg->m_flags & 0x200) != 0 )
      do_initial_tunnel_test(COERCE_FLOAT(&bpi_env_iter), (broad_phase_group *)bpg, bpeqr);
    v4 = *bpeqr->m_list_bpi_env.m_list_cur;
    last_bpi_env_iter = bpeqr->m_list_bpi_env.m_list;
    for ( i = v4; last_bpi_env_iter != i; last_bpi_env_iter = last_bpi_env_iter->m_next )
    {
      m_bpb = last_bpi_env_iter->m_bpb;
      if ( (last_bpi_env_iter->m_bpb->m_env_collision_flags & v3->m_env_collision_flags) != 0
        && phys_are_potentially_colliding_whace<broad_phase_info,broad_phase_info>(
             COERCE_FLOAT(&bpi_env_iter),
             v3,
             (broad_phase_info *)m_bpb,
             &v19) )
      {
        m_rb = (broad_phase_info *)v3->m_rb;
        if ( SLOBYTE(m_bpb->m_flags) >= 0 )
        {
          for ( ; m_rb; m_rb = (broad_phase_info *)m_rb->m_list_bpb_next )
          {
            if ( (m_rb->m_env_collision_flags & m_bpb->m_env_collision_flags) != 0 )
            {
              v13.x = m_rb->m_trace_translation.x - m_bpb->m_trace_translation.x;
              v13.y = m_rb->m_trace_translation.y - m_bpb->m_trace_translation.y;
              v13.z = m_rb->m_trace_translation.z - m_bpb->m_trace_translation.z;
              if ( phys_are_potentially_colliding(
                     &m_rb->m_trace_aabb_min_whace,
                     &m_rb->m_trace_aabb_max_whace,
                     &v13,
                     &m_bpb->m_trace_aabb_min_whace,
                     &m_bpb->m_trace_aabb_max_whace,
                     &v19) )
              {
                add_collision_pair_mutex(m_rb, (broad_phase_info *)m_bpb, v19, 0);
              }
            }
          }
          if ( bpg->m_cg_to_world_xform )
          {
            if ( !bpg->m_cg_to_rb_xform
              && _tlAssert("source/phys_broad_phase.cpp", 835, "bpg->m_list_wci", &toastPopupTitle) )
            {
              __debugbreak();
            }
            m_cg_to_rb_xform = (phys_wheel_collide_info *)bpg->m_cg_to_rb_xform;
            v15 = &m_cg_to_rb_xform[LODWORD(bpg->m_cg_to_world_xform->w.y)];
            if ( m_cg_to_rb_xform != v15 )
            {
              p_z = (phys_wheel_collide_info *)&m_cg_to_rb_xform->m_ray_dir.z;
              for ( last_wci = (phys_wheel_collide_info *)&m_cg_to_rb_xform->m_ray_dir.z; ; p_z = last_wci )
              {
                v12.x = *((float *)&p_z[-1].m_hit_bpi + 1) - m_bpb->m_trace_translation.x;
                v12.y = *((float *)&p_z[-1].m_hit_bpi + 2) - m_bpb->m_trace_translation.y;
                v12.z = p_z->m_ray_pos.x - m_bpb->m_trace_translation.z;
                if ( phys_are_potentially_colliding(
                       &m_cg_to_rb_xform->m_ray_pos,
                       &m_cg_to_rb_xform->m_ray_pos,
                       &v12,
                       &m_bpb->m_trace_aabb_min_whace,
                       &m_bpb->m_trace_aabb_max_whace,
                       &v19) )
                {
                  phys_wheel_collide_info::collision_process(
                    m_cg_to_rb_xform,
                    (int)&bpi_env_iter,
                    (int)m_cg_to_rb_xform,
                    (int)m_bpb,
                    (broad_phase_info *)m_bpb);
                }
                ++last_wci;
                if ( ++m_cg_to_rb_xform == v15 )
                  break;
              }
            }
          }
        }
        else
        {
          m_sap_node = (phys_wheel_collide_info *)m_bpb->m_sap_node;
          last_wci = m_sap_node;
          if ( m_rb )
          {
            while ( !(*(unsigned __int8 (__thiscall **)(phys_wheel_collide_info *))LODWORD(m_sap_node->m_ray_pos.x))(m_sap_node) )
            {
              if ( (m_rb->m_env_collision_flags & m_bpb->m_env_collision_flags) != 0
                && phys_are_potentially_colliding_whace<broad_phase_info,broad_phase_info>(
                     COERCE_FLOAT(&bpi_env_iter),
                     m_rb,
                     (broad_phase_info *)m_bpb,
                     &v19)
                && bpi_do_gjk_intersect(
                     (phys_gjk_cache_info *)&bpi_env_iter,
                     m_rb,
                     (broad_phase_info *)m_bpb,
                     (phys_collision_pair *)LODWORD(v19)) )
              {
                tlAtomicMutex::Lock(&G_BPM->g_bp_auto_activate_mutex);
                if ( !(*(unsigned __int8 (__thiscall **)(phys_wheel_collide_info *))LODWORD(last_wci->m_ray_pos.x))(last_wci) )
                  (*(void (__thiscall **)(phys_wheel_collide_info *, broad_phase_info *))(LODWORD(last_wci->m_ray_pos.x)
                                                                                        + 4))(
                    last_wci,
                    m_rb);
                v8 = G_BPM;
                --G_BPM->g_bp_auto_activate_mutex.LockCount;
                v15 = (phys_wheel_collide_info *)v8;
                if ( !v8->g_bp_auto_activate_mutex.LockCount )
                {
                  v16 = 0;
                  InterlockedExchange(&v16, 0);
                  v9 = v15;
                  v15->m_ray_pos.x = 0.0;
                  v9->m_ray_pos.y = 0.0;
                }
              }
              m_rb = (broad_phase_info *)m_rb->m_list_bpb_next;
              if ( !m_rb )
                break;
              m_sap_node = last_wci;
            }
          }
        }
        v3 = bpg;
      }
    }
  }
  broad_phase_group::collision_epilog((broad_phase_group *)v3);
}

int  bp_env_jq_batch_function2@<eax>(int a1@<ebp>)
{
  broad_phase_base_list::node *v1; // eax
  broad_phase_base *v2; // esi
  _BYTE v4[12]; // [esp-Ch] [ebp-6Ch] BYREF
  broad_phase_base_list::node v5; // [esp+40h] [ebp-20h] BYREF
  int v6; // [esp+48h] [ebp-18h]
  broad_phase_environement_query_results bpeqr; // [esp+4Ch] [ebp-14h]
  int retaddr; // [esp+60h] [ebp+0h]

  bpeqr.m_list_bpi_env_count = a1;
  bpeqr.m_thread_id = retaddr;
  v1 = (broad_phase_base_list::node *)_InterlockedExchangeAdd(&g_thread_id, 1u);
  v2 = g_bpb_list_cur;
  v5.m_bpb = 0;
  v5.m_next = &v5;
  for ( bpeqr.m_list_bpi_env.m_list = v1; g_bpb_list_cur; v2 = g_bpb_list_cur )
  {
    if ( (broad_phase_base *)_InterlockedCompareExchange(
                               (volatile signed __int32 *)&g_bpb_list_cur,
                               (signed __int32)v2->m_list_bpb_next,
                               (signed __int32)v2) == v2 )
    {
      check_terrain_query_params(v2);
      v5.m_next = &v5;
      v6 = 0;
      bpeqr.m_list_bpi_env.m_list_cur = 0;
      init_bpeqi((broad_phase_environment_query_input *)v4, v2);
      G_BPM->g_broad_phase_terrain_query_callback->query(
        G_BPM->g_broad_phase_terrain_query_callback,
        (const broad_phase_environment_query_input *)v4,
        (broad_phase_environement_query_results *)&v5);
      process_cluster_environment_collision(v2, &v5);
    }
  }
  return 0;
}

void  broad_phase_process(float a1@<ebp>)
{
  broad_phase_memory *v1; // eax
  phys_link_list<phys_collision_pair> *p_g_list_phys_collide_data; // eax
  broad_phase_base *i; // esi
  axis_aligned_sweep_and_prune::sap_node *m_sap_node; // ecx
  broad_phase_memory *v5; // edx
  phys_collision_pair *j; // esi
  int m_total_memory_allocated; // eax
  phys_link_list<phys_collision_pair> *v8; // eax

  v1 = G_BPM;
  G_BPM->m_list_bpi_env = 0;
  v1->m_bpi_env_count = 0;
  v1->m_bpg_env_count = 0;
  v1->m_bpg_env_bpi_count = 0;
  v1->m_bpi_env_no_database_count = 0;
  if ( tlScratchpadLocked
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\base\\include\\tl_system.h",
         215,
         "!tlScratchpadLocked",
         "Scratchpad is already locked!") )
  {
    __debugbreak();
  }
  tlScratchpadLocked = 1;
  if ( G_BPM->g_collision_memory_buffer.m_cur
    && _tlAssert("source/phys_broad_phase.cpp", 1220, "G_BPM->g_collision_memory_buffer.is_empty()", &toastPopupTitle) )
  {
    __debugbreak();
  }
  p_g_list_phys_collide_data = &G_BPM->g_list_phys_collide_data;
  G_BPM->g_list_phys_collide_data.m_first = 0;
  p_g_list_phys_collide_data->m_last_next_ptr = &p_g_list_phys_collide_data->m_first;
  p_g_list_phys_collide_data->m_alloc_count = 0;
  broad_phase_process_object_environment_collision();
  broad_phase_process_collision_pairs(a1);
  for ( i = G_BPM->g_list_bpb; i; i = i->m_list_bpb_next )
  {
    m_sap_node = (axis_aligned_sweep_and_prune::sap_node *)i->m_sap_node;
    if ( m_sap_node )
      axis_aligned_sweep_and_prune::sap_node::update_ae_val(m_sap_node, a1);
  }
  axis_aligned_sweep_and_prune::process(g_axis_aligned_sweep_and_prune);
  v5 = G_BPM;
  for ( j = G_BPM->g_list_phys_collide_data.m_first; j; j = j->m_next_link )
  {
    if ( !j->m_gjk_ci )
    {
      j->m_gjk_ci = phys_heap_gjk_cache_system_avl_tree::get_gjk_cache_info(
                      &v5->g_phys_gjk_cache_system,
                      j->m_bpi1->m_gjk_geom_id,
                      j->m_bpi2->m_gjk_geom_id,
                      1);
      v5 = G_BPM;
    }
  }
  m_total_memory_allocated = v5->g_collision_memory_buffer.m_total_memory_allocated;
  if ( v5->m_memory_high_water > m_total_memory_allocated )
    m_total_memory_allocated = v5->m_memory_high_water;
  v5->m_memory_high_water = m_total_memory_allocated;
  process_list_do_gjk_collide_and_contact_manifold(&G_BPM->g_list_phys_collide_data);
  v8 = &G_BPM->g_list_phys_collide_data;
  G_BPM->g_list_phys_collide_data.m_first = 0;
  v8->m_last_next_ptr = &v8->m_first;
  v8->m_alloc_count = 0;
  if ( !tlScratchpadLocked
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\base\\include\\tl_system.h",
         221,
         "tlScratchpadLocked",
         "Scratchpad is already unlocked!") )
  {
    __debugbreak();
  }
  tlScratchpadLocked = 0;
  phys_heap_gjk_cache_system_avl_tree::update_cache(&G_BPM->g_phys_gjk_cache_system);
  bpei_database_t::update_database(&G_BPM->g_bpei_database);
}

void __cdecl broad_phase_system_init(
        const broad_phase_memory_info *bpmi,
        bool (__cdecl *should_collide_callback)(const broad_phase_base *, const broad_phase_base *))
{
  axis_aligned_sweep_and_prune *v2; // eax
  broad_phase_memory *v3; // esi
  phys_free_list<broad_phase_info> *p_g_list_broad_phase_info; // ecx

  break_here_39 = 1;
  G_BPM = broad_phase_memory::allocate_buffer(bpmi);
  v2 = (axis_aligned_sweep_and_prune *)PMM_PERM_ALLOCATE(0x28u, 4u);
  if ( v2 )
  {
    v2->m_sap_node_allocator.m_count = 0;
    v2->m_active_pair_allocator.m_count = 0;
  }
  else
  {
    v2 = 0;
  }
  v3 = G_BPM;
  g_axis_aligned_sweep_and_prune = v2;
  p_g_list_broad_phase_info = &G_BPM->g_list_broad_phase_info;
  G_BPM->g_broad_phase_terrain_query_callback = 0;
  v3->g_list_bpb = 0;
  v3->g_list_bpb_count = 0;
  phys_free_list<broad_phase_info>::remove_all(p_g_list_broad_phase_info);
  phys_free_list<rigid_body_constraint_distance>::remove_all(&v3->g_list_broad_phase_group);
  phys_free_list<broad_phase_collision_pair>::remove_all(&v3->g_list_broad_phase_collision_pair);
  bpei_database_t::purge_database(&v3->g_bpei_database);
  v3->m_memory_high_water = 0;
  axis_aligned_sweep_and_prune::init_system(g_axis_aligned_sweep_and_prune, bpmi->m_max_num_sap_active_pairs);
  g_axis_aligned_sweep_and_prune->m_should_collide_callback = should_collide_callback;
}

void __cdecl broad_phase_system_shutdown()
{
  broad_phase_memory::~broad_phase_memory(G_BPM);
  G_BPM = 0;
  g_axis_aligned_sweep_and_prune = 0;
}

void __thiscall phys_free_list<broad_phase_info>::debug_remove(
        phys_free_list<broad_phase_info> *this,
        phys_free_list<broad_phase_info>::T_internal *T_i)
{
  unsigned int m_ptr_list_index; // eax

  m_ptr_list_index = T_i->m_ptr_list_index;
  if ( m_ptr_list_index != -1 )
  {
    if ( m_ptr_list_index >= 0x100
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
           421,
           "T_i->m_ptr_list_index >= 0 && T_i->m_ptr_list_index < PTR_LIST_SIZE",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    if ( this->m_ptr_list[T_i->m_ptr_list_index] != &T_i->m_data )
    {
      if ( _tlAssert(
             "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
             422,
             "m_ptr_list[T_i->m_ptr_list_index] == &T_i->m_data",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
    }
    LODWORD(this->m_ptr_list[--this->m_ptr_list_count][1].m_trace_aabb_min_whace.x) = T_i->m_ptr_list_index;
    this->m_ptr_list[T_i->m_ptr_list_index] = this->m_ptr_list[this->m_ptr_list_count];
  }
}

void __thiscall phys_free_list<broad_phase_group>::debug_remove(
        phys_free_list<broad_phase_group> *this,
        phys_free_list<broad_phase_group>::T_internal *T_i)
{
  unsigned int m_ptr_list_index; // eax

  m_ptr_list_index = T_i->m_ptr_list_index;
  if ( m_ptr_list_index != -1 )
  {
    if ( m_ptr_list_index >= 0x100
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
           421,
           "T_i->m_ptr_list_index >= 0 && T_i->m_ptr_list_index < PTR_LIST_SIZE",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    if ( this->m_ptr_list[T_i->m_ptr_list_index] != &T_i->m_data )
    {
      if ( _tlAssert(
             "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
             422,
             "m_ptr_list[T_i->m_ptr_list_index] == &T_i->m_data",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
    }
    LODWORD(this->m_ptr_list[--this->m_ptr_list_count][1].m_trace_aabb_min_whace.x) = T_i->m_ptr_list_index;
    this->m_ptr_list[T_i->m_ptr_list_index] = this->m_ptr_list[this->m_ptr_list_count];
  }
}

void __thiscall phys_free_list<broad_phase_collision_pair>::debug_remove(
        phys_free_list<broad_phase_collision_pair> *this,
        phys_free_list<broad_phase_collision_pair>::T_internal *T_i)
{
  unsigned int m_ptr_list_index; // eax

  m_ptr_list_index = T_i->m_ptr_list_index;
  if ( m_ptr_list_index != -1 )
  {
    if ( m_ptr_list_index >= 0x100
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
           421,
           "T_i->m_ptr_list_index >= 0 && T_i->m_ptr_list_index < PTR_LIST_SIZE",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    if ( this->m_ptr_list[T_i->m_ptr_list_index] != &T_i->m_data )
    {
      if ( _tlAssert(
             "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
             422,
             "m_ptr_list[T_i->m_ptr_list_index] == &T_i->m_data",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
    }
    this->m_ptr_list[--this->m_ptr_list_count][1].m_bpi1 = (broad_phase_info *)T_i->m_ptr_list_index;
    this->m_ptr_list[T_i->m_ptr_list_index] = this->m_ptr_list[this->m_ptr_list_count];
  }
}

void __thiscall tlAtomicReadWriteMutex::~tlAtomicReadWriteMutex(tlAtomicReadWriteMutex *this)
{
  this->ThisPtr = 0;
}

void __thiscall rigid_body::adjust_col_moved_vec(rigid_body *this, float lambda)
{
  double v3; // st7
  float v4; // [esp-30h] [ebp-3Ch]
  float v5; // [esp-2Ch] [ebp-38h]
  float v6; // [esp-28h] [ebp-34h]
  float v7; // [esp-20h] [ebp-2Ch]
  float v8; // [esp-1Ch] [ebp-28h]
  float v9; // [esp-18h] [ebp-24h]
  float v10; // [esp-4h] [ebp-10h]
  float v11; // [esp-4h] [ebp-10h]
  float v12; // [esp-4h] [ebp-10h]
  float v13; // [esp-4h] [ebp-10h]
  float v14; // [esp-4h] [ebp-10h]
  float v15; // [esp-4h] [ebp-10h]
  float v16; // [esp-4h] [ebp-10h]

  v7 = this->m_moved_vec.x * lambda;
  v8 = this->m_moved_vec.y * lambda;
  v9 = this->m_moved_vec.z * lambda;
  v4 = this->m_mat.w.x + v7;
  v5 = this->m_mat.w.y + v8;
  v6 = this->m_mat.w.z + v9;
  this->m_mat.w.x = v4;
  this->m_mat.w.y = v5;
  this->m_mat.w.z = v6;
  v10 = 1.0 - lambda;
  this->m_moved_vec.x = v10 * this->m_moved_vec.x;
  this->m_moved_vec.y = this->m_moved_vec.y * v10;
  this->m_moved_vec.z = v10 * this->m_moved_vec.z;
  v3 = 100000.0;
  v11 = fabs(this->m_moved_vec.x);
  if ( v11 > 100000.0
    || (v12 = fabs(this->m_moved_vec.y), v12 > 100000.0)
    || (v13 = fabs(this->m_moved_vec.z), v13 > 100000.0) )
  {
    phys_exec_debug_callback(this);
    v3 = 100000.0;
  }
  v14 = fabs(this->m_mat.w.x);
  if ( v14 > v3 || (v15 = fabs(this->m_mat.w.y), v15 > v3) || (v16 = fabs(this->m_mat.w.z), v3 < v16) )
    phys_exec_debug_callback(this);
}

void  broad_phase_group::collision_prolog(broad_phase_group *this@<ecx>, int a2@<ebp>)
{
  broad_phase_group *v2; // edi
  bool v3; // zf
  broad_phase_info *m_list_bpi_head; // eax
  double v5; // st7
  float y; // edx
  float z; // ecx
  float x; // ecx
  double v9; // st7
  float w; // edx
  float v11; // ecx
  double v12; // st7
  double v13; // st7
  float v14; // edx
  unsigned int m_env_collision_flags; // ecx
  double v16; // st7
  double v17; // st7
  broad_phase_info *m_list_bpb_next; // esi
  double v19; // st7
  double v20; // st7
  double v21; // st7
  double v22; // st7
  double v23; // st7
  double v24; // st7
  double v25; // st7
  double v26; // st7
  double v27; // st7
  double v28; // st7
  double v29; // st7
  double v30; // st7
  unsigned int v31; // edx
  double v32; // st7
  rb_vehicle_model *m_rbvm; // eax
  phys_wheel_collide_info *v34; // eax
  int m_buffer; // esi
  int v36; // eax
  int v37; // esi
  float v38; // edi
  float *v39; // edi
  double v40; // st7
  double v41; // st7
  double v42; // st7
  double v43; // st7
  double v44; // st7
  double v45; // st7
  double v46; // st7
  double v47; // st7
  double v48; // st7
  double v49; // st7
  double v50; // st7
  double v51; // st7
  bool v52; // cc
  double v53; // st7
  double v54; // st6
  double v55; // st5
  double v56; // st4
  double v57; // st3
  double v58; // st2
  double v59; // st1
  double v60; // st6
  double v61; // st1
  double v62; // st4
  double v63; // st1
  double v64; // st2
  float v65; // [esp-Ch] [ebp-BCh] BYREF
  float v66; // [esp-8h] [ebp-B8h]
  float v67; // [esp-4h] [ebp-B4h]
  phys_vec3 p2; // [esp+0h] [ebp-B0h] BYREF
  phys_vec3 half_dims; // [esp+10h] [ebp-A0h]
  broad_phase_group *v70; // [esp+20h] [ebp-90h]
  const phys_mat44 *rb_mat; // [esp+24h] [ebp-8Ch] BYREF
  int wheel_count; // [esp+28h] [ebp-88h]
  float v73; // [esp+2Ch] [ebp-84h]
  phys_vec3 aabb2_min; // [esp+30h] [ebp-80h] BYREF
  phys_vec3 aabb2_max; // [esp+40h] [ebp-70h] BYREF
  phys_vec3 p1; // [esp+50h] [ebp-60h] BYREF
  phys_vec3 aabb1_max; // [esp+60h] [ebp-50h] BYREF
  phys_vec3 aabb1_min; // [esp+70h] [ebp-40h]
  float v79; // [esp+90h] [ebp-20h]
  float v80; // [esp+94h] [ebp-1Ch]
  float v81; // [esp+98h] [ebp-18h]
  float v82; // [esp+9Ch] [ebp-14h]
  float v83; // [esp+A0h] [ebp-10h]
  int v84; // [esp+A4h] [ebp-Ch] BYREF
  int i; // [esp+A8h] [ebp-8h]
  int retaddr; // [esp+B0h] [ebp+0h]

  v84 = a2;
  i = retaddr;
  v2 = this;
  v3 = this->m_list_bpi_head == 0;
  v70 = this;
  if ( v3
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_broad_phase_inline.h",
         184,
         "m_list_bpi_head",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  broad_phase_info::collision_prolog(v2->m_list_bpi_head);
  m_list_bpi_head = v2->m_list_bpi_head;
  v5 = m_list_bpi_head->m_trace_translation.x + m_list_bpi_head->m_trace_aabb_min_whace.x;
  y = m_list_bpi_head->m_trace_aabb_min_whace.y;
  aabb1_max.y = m_list_bpi_head->m_trace_aabb_min_whace.x;
  z = m_list_bpi_head->m_trace_aabb_min_whace.z;
  *(float *)&rb_mat = v5;
  aabb1_max.w = z;
  x = m_list_bpi_head->m_trace_aabb_max_whace.x;
  v9 = m_list_bpi_head->m_trace_translation.y + m_list_bpi_head->m_trace_aabb_min_whace.y;
  aabb1_max.z = y;
  w = m_list_bpi_head->m_trace_aabb_min_whace.w;
  p1.y = x;
  v11 = m_list_bpi_head->m_trace_aabb_max_whace.z;
  *(float *)&wheel_count = v9;
  v12 = m_list_bpi_head->m_trace_translation.z;
  aabb1_min.x = w;
  v13 = v12 + m_list_bpi_head->m_trace_aabb_min_whace.z;
  v14 = m_list_bpi_head->m_trace_aabb_max_whace.y;
  p1.w = v11;
  m_env_collision_flags = m_list_bpi_head->m_env_collision_flags;
  v73 = v13;
  p1.z = v14;
  v16 = m_list_bpi_head->m_trace_aabb_max_whace.x + m_list_bpi_head->m_trace_translation.x;
  aabb1_max.x = m_list_bpi_head->m_trace_aabb_max_whace.w;
  aabb2_min.y = v16;
  aabb2_min.z = m_list_bpi_head->m_trace_aabb_max_whace.y + m_list_bpi_head->m_trace_translation.y;
  v17 = m_list_bpi_head->m_trace_aabb_max_whace.z + m_list_bpi_head->m_trace_translation.z;
  v2->m_env_collision_flags = m_env_collision_flags;
  m_list_bpb_next = (broad_phase_info *)m_list_bpi_head->m_list_bpb_next;
  for ( aabb2_min.w = v17; m_list_bpb_next; aabb2_min.w = v82 )
  {
    broad_phase_info::collision_prolog(m_list_bpb_next);
    v19 = aabb1_max.w;
    if ( m_list_bpb_next->m_trace_aabb_min_whace.z < (double)aabb1_max.w )
      v19 = m_list_bpb_next->m_trace_aabb_min_whace.z;
    v83 = v19;
    v20 = aabb1_max.z;
    if ( m_list_bpb_next->m_trace_aabb_min_whace.y < (double)aabb1_max.z )
      v20 = m_list_bpb_next->m_trace_aabb_min_whace.y;
    v82 = v20;
    v21 = aabb1_max.y;
    if ( m_list_bpb_next->m_trace_aabb_min_whace.x < (double)aabb1_max.y )
      v21 = m_list_bpb_next->m_trace_aabb_min_whace.x;
    v81 = v21;
    aabb1_max.y = v81;
    aabb1_max.z = v82;
    aabb1_max.w = v83;
    v22 = p1.w;
    if ( m_list_bpb_next->m_trace_aabb_max_whace.z > (double)p1.w )
      v22 = m_list_bpb_next->m_trace_aabb_max_whace.z;
    v82 = v22;
    v23 = p1.z;
    if ( m_list_bpb_next->m_trace_aabb_max_whace.y > (double)p1.z )
      v23 = m_list_bpb_next->m_trace_aabb_max_whace.y;
    v81 = v23;
    v24 = p1.y;
    if ( m_list_bpb_next->m_trace_aabb_max_whace.x > (double)p1.y )
      v24 = m_list_bpb_next->m_trace_aabb_max_whace.x;
    v83 = v24;
    p1.y = v83;
    p1.z = v81;
    p1.w = v82;
    aabb1_min.y = m_list_bpb_next->m_trace_aabb_max_whace.x + m_list_bpb_next->m_trace_translation.x;
    aabb1_min.z = m_list_bpb_next->m_trace_aabb_max_whace.y + m_list_bpb_next->m_trace_translation.y;
    aabb1_min.w = m_list_bpb_next->m_trace_aabb_max_whace.z + m_list_bpb_next->m_trace_translation.z;
    aabb2_max.y = m_list_bpb_next->m_trace_aabb_min_whace.x + m_list_bpb_next->m_trace_translation.x;
    aabb2_max.z = m_list_bpb_next->m_trace_translation.y + m_list_bpb_next->m_trace_aabb_min_whace.y;
    aabb2_max.w = m_list_bpb_next->m_trace_translation.z + m_list_bpb_next->m_trace_aabb_min_whace.z;
    v25 = v73;
    if ( aabb2_max.w < (double)v73 )
      v25 = aabb2_max.w;
    v82 = v25;
    v26 = *(float *)&wheel_count;
    if ( aabb2_max.z < (double)*(float *)&wheel_count )
      v26 = aabb2_max.z;
    v81 = v26;
    v27 = *(float *)&rb_mat;
    if ( aabb2_max.y < (double)*(float *)&rb_mat )
      v27 = aabb2_max.y;
    v83 = v27;
    *(float *)&rb_mat = v83;
    *(float *)&wheel_count = v81;
    v73 = v82;
    v28 = aabb2_min.w;
    if ( aabb1_min.w > (double)aabb2_min.w )
      v28 = aabb1_min.w;
    v82 = v28;
    v29 = aabb2_min.z;
    if ( aabb1_min.z > (double)aabb2_min.z )
      v29 = aabb1_min.z;
    v81 = v29;
    v30 = aabb2_min.y;
    if ( aabb1_min.y > (double)aabb2_min.y )
      v30 = aabb1_min.y;
    v31 = m_list_bpb_next->m_env_collision_flags;
    v83 = v30;
    v32 = v83;
    v2->m_env_collision_flags |= v31;
    m_list_bpb_next = (broad_phase_info *)m_list_bpb_next->m_list_bpb_next;
    aabb2_min.y = v32;
    aabb2_min.z = v81;
  }
  m_rbvm = v2->m_rbvm;
  if ( m_rbvm )
  {
    LODWORD(half_dims.w) = m_rbvm->m_wheels.m_alloc_count;
    v34 = (phys_wheel_collide_info *)phys_transient_allocator::mt_allocate(
                                       &G_BPM->g_collision_memory_buffer,
                                       LODWORD(half_dims.w) << 6,
                                       16,
                                       0,
                                       "broad phase collision out of memory.");
    m_buffer = (int)v2->m_rbvm->m_wheels.m_buffer;
    v2->m_list_wci = v34;
    if ( *(int *)(m_buffer + 20) <= 0
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_array_base.inc",
           118,
           "i >= 0 && i < m_alloc_count",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    LODWORD(half_dims.z) = ***(unsigned int ***)(m_buffer + 16) + 48;
    v36 = 0;
    v82 = 0.0;
    if ( SLODWORD(half_dims.w) > 0 )
    {
      v81 = 0.0;
      while ( 1 )
      {
        v37 = (int)v2->m_list_wci + LODWORD(v81);
        LODWORD(v38) = v2->m_rbvm->m_wheels.m_buffer;
        v80 = v38;
        if ( (v36 < 0 || v36 >= *(unsigned int *)(LODWORD(v38) + 20))
          && _tlAssert(
               "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_array_base.inc",
               118,
               "i >= 0 && i < m_alloc_count",
               &toastPopupTitle) )
        {
          __debugbreak();
        }
        v39 = (float *)(v37 + 16);
        rigid_body_constraint_wheel::get_wheel_collide_segment(
          *(rigid_body_constraint_wheel **)(*(unsigned int *)(LODWORD(v80) + 16) + 4 * LODWORD(v82)),
          (int)&v84,
          (const phys_mat44 *)LODWORD(half_dims.z),
          (phys_vec3 *const)v37,
          (phys_vec3 *const)(v37 + 16));
        *(float *)(v37 + 16) = *(float *)(v37 + 16) - *(float *)v37;
        *(float *)(v37 + 20) = *(float *)(v37 + 20) - *(float *)(v37 + 4);
        *(float *)(v37 + 24) = *(float *)(v37 + 24) - *(float *)(v37 + 8);
        *(unsigned int *)(v37 + 52) = 0;
        *(float *)(v37 + 48) = 1.0;
        v40 = aabb1_max.w;
        if ( *(float *)(v37 + 8) < (double)aabb1_max.w )
          v40 = *(float *)(v37 + 8);
        v80 = v40;
        v41 = aabb1_max.z;
        if ( *(float *)(v37 + 4) < (double)aabb1_max.z )
          v41 = *(float *)(v37 + 4);
        v79 = v41;
        v42 = aabb1_max.y;
        if ( *(float *)v37 < (double)aabb1_max.y )
          v42 = *(float *)v37;
        v83 = v42;
        aabb1_max.y = v83;
        aabb1_max.z = v79;
        aabb1_max.w = v80;
        v43 = p1.w;
        if ( *(float *)(v37 + 8) > (double)p1.w )
          v43 = *(float *)(v37 + 8);
        v83 = v43;
        v44 = p1.z;
        if ( *(float *)(v37 + 4) > (double)p1.z )
          v44 = *(float *)(v37 + 4);
        v79 = v44;
        v45 = p1.y;
        if ( *(float *)v37 > (double)p1.y )
          v45 = *(float *)v37;
        v80 = v45;
        p1.y = v80;
        p1.z = v79;
        p1.w = v83;
        aabb2_max.y = *(float *)v37 + *v39;
        aabb2_max.z = *(float *)(v37 + 20) + *(float *)(v37 + 4);
        aabb2_max.w = *(float *)(v37 + 24) + *(float *)(v37 + 8);
        aabb1_min.y = *(float *)v37 + *v39;
        aabb1_min.z = *(float *)(v37 + 20) + *(float *)(v37 + 4);
        aabb1_min.w = *(float *)(v37 + 24) + *(float *)(v37 + 8);
        v46 = v73;
        if ( aabb1_min.w < (double)v73 )
          v46 = aabb1_min.w;
        v83 = v46;
        v47 = *(float *)&wheel_count;
        if ( aabb1_min.z < (double)*(float *)&wheel_count )
          v47 = aabb1_min.z;
        v79 = v47;
        v48 = *(float *)&rb_mat;
        if ( aabb1_min.y < (double)*(float *)&rb_mat )
          v48 = aabb1_min.y;
        v80 = v48;
        *(float *)&rb_mat = v80;
        *(float *)&wheel_count = v79;
        v73 = v83;
        v49 = aabb2_min.w;
        if ( aabb2_max.w > (double)aabb2_min.w )
          v49 = aabb2_max.w;
        v83 = v49;
        v50 = aabb2_min.z;
        if ( aabb2_max.z > (double)aabb2_min.z )
          v50 = aabb2_max.z;
        v79 = v50;
        v51 = aabb2_min.y;
        if ( aabb2_max.y > (double)aabb2_min.y )
          v51 = aabb2_max.y;
        v80 = v51;
        LODWORD(v81) += 64;
        v2 = v70;
        aabb2_min.y = v80;
        v52 = LODWORD(v82) + 1 < SLODWORD(half_dims.w);
        aabb2_min.z = v79;
        ++LODWORD(v82);
        aabb2_min.w = v83;
        if ( !v52 )
          break;
        v36 = LODWORD(v82);
      }
    }
  }
  comp_trace_volume(
    (int)&v84,
    (phys_vec3 *)&aabb1_max.y,
    (phys_vec3 *)&p1.y,
    (const phys_vec3 *)&rb_mat,
    (phys_vec3 *)&aabb2_min.y,
    (phys_vec3 *)&aabb2_max.y,
    (phys_vec3 *)&v65,
    (phys_vec3 *)&p2.y);
  v53 = aabb2_max.y;
  v54 = p2.y;
  aabb1_min.y = aabb2_max.y - p2.y;
  v55 = aabb2_max.z;
  v56 = p2.z;
  aabb1_min.z = aabb2_max.z - p2.z;
  v57 = aabb2_max.w;
  v58 = p2.w;
  aabb1_min.w = aabb2_max.w - p2.w;
  v2->m_trace_aabb_min_whace.x = aabb1_min.y;
  v2->m_trace_aabb_min_whace.y = aabb1_min.z;
  v2->m_trace_aabb_min_whace.z = aabb1_min.w;
  v59 = v54 + v65;
  v60 = v65;
  aabb1_min.y = v59;
  v61 = v56 + v66;
  v62 = v66;
  aabb1_min.z = v61;
  v63 = v58 + v67;
  v64 = v67;
  aabb1_min.w = v63;
  v2->m_trace_aabb_max_whace.x = aabb1_min.y;
  v2->m_trace_aabb_max_whace.y = aabb1_min.z;
  v2->m_trace_aabb_max_whace.z = aabb1_min.w;
  aabb1_min.y = v60 - v53;
  aabb1_min.z = v62 - v55;
  aabb1_min.w = v64 - v57;
  v2->m_trace_translation.x = aabb1_min.y;
  v2->m_trace_translation.y = aabb1_min.z;
  v2->m_trace_translation.z = aabb1_min.w;
}

void __thiscall broad_phase_group::collision_epilog(broad_phase_group *this)
{
  broad_phase_group *v1; // edi
  int v2; // esi
  phys_wheel_collide_info *v3; // ebx
  int m_buffer; // edi
  int wheel_count; // [esp+8h] [ebp-Ch]
  int v7; // [esp+10h] [ebp-4h]
  int savedregs; // [esp+14h] [ebp+0h] BYREF

  v1 = this;
  v2 = 0;
  if ( this->m_rbvm )
  {
    if ( !this->m_list_wci && _tlAssert("source/phys_broad_phase.cpp", 69, "m_list_wci", &toastPopupTitle) )
      __debugbreak();
    wheel_count = v1->m_rbvm->m_wheels.m_alloc_count;
    if ( wheel_count > 0 )
    {
      v7 = 0;
      while ( 1 )
      {
        v3 = &v1->m_list_wci[v7];
        m_buffer = (int)v1->m_rbvm->m_wheels.m_buffer;
        if ( v2 < 0 || v2 >= *(unsigned int *)(m_buffer + 20) )
        {
          if ( _tlAssert(
                 "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_array_base.inc",
                 118,
                 "i >= 0 && i < m_alloc_count",
                 &toastPopupTitle) )
          {
            __debugbreak();
          }
        }
        phys_wheel_collide_info::collision_epilog(
          v3,
          (int)&savedregs,
          *(rigid_body_constraint_wheel **)(*(unsigned int *)(m_buffer + 16) + 4 * v2));
        ++v7;
        if ( ++v2 >= wheel_count )
          break;
        v1 = this;
      }
    }
  }
}

void  axis_aligned_sweep_and_prune::sap_node::update_ae_val(
        axis_aligned_sweep_and_prune::sap_node *this@<ecx>,
        float a2@<ebp>)
{
  broad_phase_base *m_bpb; // ecx
  float v4[3]; // [esp-Ch] [ebp-2Ch] BYREF
  phys_vec3 aabb[2]; // [esp+0h] [ebp-20h] BYREF
  float retaddr; // [esp+20h] [ebp+0h]

  aabb[1].y = a2;
  aabb[1].z = retaddr;
  if ( this->m_updated && _tlAssert("source/phys_broad_phase.cpp", 225, "m_updated == 0", &toastPopupTitle) )
    __debugbreak();
  m_bpb = this->m_bpb;
  this->m_updated = 1;
  broad_phase_base::get_aabb(m_bpb, (int)&aabb[1].y, (phys_vec3 *)v4);
  this->m_ae1[0][0].m_val = v4[0];
  this->m_ae1[0][1].m_val = aabb[0].y;
  this->m_ae1[1][0].m_val = v4[1];
  this->m_ae1[1][1].m_val = aabb[0].z;
  this->m_ae1[2][0].m_val = v4[2];
  this->m_ae1[2][1].m_val = aabb[0].w;
}

void __thiscall axis_aligned_sweep_and_prune::init_system(axis_aligned_sweep_and_prune *this, int max_num_active_pairs)
{
  if ( this->m_active_pair_allocator.m_count
    && _tlAssert("source/phys_broad_phase.cpp", 347, "m_active_pair_allocator.get_count() == 0", &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( this->m_sap_node_allocator.m_count
    && _tlAssert("source/phys_broad_phase.cpp", 348, "m_sap_node_allocator.get_count() == 0", &toastPopupTitle) )
  {
    __debugbreak();
  }
  this->m_x_list = 0;
  this->m_y_list = 0;
  this->m_z_list = 0;
  this->m_list_bpi_bpi = 0;
  this->m_list_bpi_bpg = 0;
  this->m_list_bpg_bpg = 0;
  this->m_max_num_active_pairs = max_num_active_pairs;
}

void  process_cluster_environment_collision_prolog(
        int a1@<ebp>,
        broad_phase_info *bpb,
        broad_phase_base *info)
{
  unsigned int m_flags; // eax
  const phys_vec3 *v4; // eax
  const phys_vec3 *v5; // eax
  phys_vec3 v6; // [esp-10h] [ebp-1Ch] BYREF
  unsigned int v7[3]; // [esp+0h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]

  v7[0] = a1;
  v7[1] = retaddr;
  m_flags = bpb->m_flags;
  if ( (m_flags & 1) != 0 )
  {
    broad_phase_info::collision_prolog(bpb);
  }
  else
  {
    if ( (m_flags & 2) == 0
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_broad_phase_base.h",
           100,
           "is_bpg()",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    broad_phase_group::collision_prolog((broad_phase_group *)bpb, (int)v7);
  }
  bpb->m_flags &= ~0x10u;
  bpb->m_list_bpb_cluster_next = 0;
  v4 = phys_min(&v6, &info->m_trace_aabb_min_whace, &bpb->m_trace_aabb_min_whace);
  info->m_trace_aabb_min_whace.x = v4->x;
  info->m_trace_aabb_min_whace.y = v4->y;
  info->m_trace_aabb_min_whace.z = v4->z;
  v5 = phys_max(&v6, &info->m_trace_aabb_max_whace, &bpb->m_trace_aabb_max_whace);
  info->m_trace_aabb_max_whace.x = v5->x;
  info->m_trace_aabb_max_whace.y = v5->y;
  info->m_trace_aabb_max_whace.z = v5->z;
}

bool __cdecl compare_bpb(broad_phase_base *bpb1, broad_phase_base *bpb2)
{
  unsigned int v2; // eax
  int v3; // esi
  double v4; // st7
  unsigned int v5; // esi
  float bpb1a; // [esp+Ch] [ebp+8h]

  v2 = g_bpb_cluster_sort_axis;
  v3 = g_bpb_cluster_sort_axis;
  if ( (unsigned int)g_bpb_cluster_sort_axis > 2 )
  {
    if ( _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
           32,
           "i >= 0 && i < 3",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    v2 = g_bpb_cluster_sort_axis;
  }
  v4 = *(&bpb1->m_trace_aabb_min_whace.x + v3);
  v5 = v2;
  if ( v2 > 2
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
         32,
         "i >= 0 && i < 3",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  bpb1a = v4;
  return *(&bpb2->m_trace_aabb_min_whace.x + v5) > (double)bpb1a;
}

void __thiscall phys_link_list<phys_collision_pair>::add_mt(
        phys_link_list<phys_collision_pair> *this,
        phys_collision_pair *p)
{
  volatile signed __int32 *p_m_last_next_ptr; // esi
  unsigned int *v4; // ecx

  p_m_last_next_ptr = (volatile signed __int32 *)&this->m_last_next_ptr;
  if ( !this->m_last_next_ptr
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         239,
         "m_last_next_ptr",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  p->m_next_link = 0;
  _InterlockedExchangeAdd(&this->m_alloc_count, 1u);
  do
    v4 = (unsigned int *)*p_m_last_next_ptr;
  while ( (unsigned int *)_InterlockedCompareExchange(p_m_last_next_ptr, (signed __int32)p, *p_m_last_next_ptr) != v4 );
  *v4 = p;
}

void __thiscall phys_free_list<broad_phase_info>::debug_add(
        phys_free_list<broad_phase_info> *this,
        phys_free_list<broad_phase_info>::T_internal *T_i)
{
  int m_list_count_high_water; // eax
  int m_ptr_list_count; // eax

  m_list_count_high_water = this->m_list_count_high_water;
  if ( m_list_count_high_water <= this->m_list_count )
    m_list_count_high_water = this->m_list_count;
  this->m_list_count_high_water = m_list_count_high_water;
  m_ptr_list_count = this->m_ptr_list_count;
  if ( m_ptr_list_count >= 256 )
  {
    T_i->m_ptr_list_index = -1;
  }
  else
  {
    T_i->m_ptr_list_index = m_ptr_list_count;
    this->m_ptr_list[this->m_ptr_list_count++] = &T_i->m_data;
  }
}

void __thiscall phys_free_list<broad_phase_info>::remove(
        phys_free_list<broad_phase_info> *this,
        phys_free_list<broad_phase_info>::T_internal *data)
{
  phys_free_list<broad_phase_info>::T_internal_base *m_next_T_internal; // eax
  phys_free_list<broad_phase_info>::T_internal_base *m_prev_T_internal; // ecx

  if ( !data
    && _tlAssert("c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h", 477, "data", &toastPopupTitle) )
  {
    __debugbreak();
  }
  --this->m_list_count;
  phys_free_list<broad_phase_info>::debug_remove(this, data);
  m_next_T_internal = data->m_next_T_internal;
  m_prev_T_internal = data->m_prev_T_internal;
  m_prev_T_internal->m_next_T_internal = m_next_T_internal;
  m_next_T_internal->m_prev_T_internal = m_prev_T_internal;
  PMM_FREE((unsigned __int8 *)data, 0x90u, 0x10u);
}

void __thiscall phys_free_list<broad_phase_group>::debug_add(
        phys_free_list<broad_phase_group> *this,
        phys_free_list<broad_phase_group>::T_internal *T_i)
{
  int m_list_count_high_water; // eax
  int m_ptr_list_count; // eax

  m_list_count_high_water = this->m_list_count_high_water;
  if ( m_list_count_high_water <= this->m_list_count )
    m_list_count_high_water = this->m_list_count;
  this->m_list_count_high_water = m_list_count_high_water;
  m_ptr_list_count = this->m_ptr_list_count;
  if ( m_ptr_list_count >= 256 )
  {
    T_i->m_ptr_list_index = -1;
  }
  else
  {
    T_i->m_ptr_list_index = m_ptr_list_count;
    this->m_ptr_list[this->m_ptr_list_count++] = &T_i->m_data;
  }
}

void __thiscall phys_free_list<broad_phase_group>::remove(
        phys_free_list<broad_phase_group> *this,
        phys_free_list<broad_phase_group>::T_internal *data)
{
  phys_free_list<broad_phase_group>::T_internal_base *m_next_T_internal; // eax
  phys_free_list<broad_phase_group>::T_internal_base *m_prev_T_internal; // ecx

  if ( !data
    && _tlAssert("c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h", 477, "data", &toastPopupTitle) )
  {
    __debugbreak();
  }
  --this->m_list_count;
  phys_free_list<broad_phase_group>::debug_remove(this, data);
  m_next_T_internal = data->m_next_T_internal;
  m_prev_T_internal = data->m_prev_T_internal;
  m_prev_T_internal->m_next_T_internal = m_next_T_internal;
  m_next_T_internal->m_prev_T_internal = m_prev_T_internal;
  PMM_FREE((unsigned __int8 *)data, 0x80u, 0x10u);
}

void __thiscall phys_free_list<broad_phase_collision_pair>::debug_add(
        phys_free_list<broad_phase_collision_pair> *this,
        phys_free_list<broad_phase_collision_pair>::T_internal *T_i)
{
  int m_list_count_high_water; // eax
  int m_ptr_list_count; // eax

  m_list_count_high_water = this->m_list_count_high_water;
  if ( m_list_count_high_water <= this->m_list_count )
    m_list_count_high_water = this->m_list_count;
  this->m_list_count_high_water = m_list_count_high_water;
  m_ptr_list_count = this->m_ptr_list_count;
  if ( m_ptr_list_count >= 256 )
  {
    T_i->m_ptr_list_index = -1;
  }
  else
  {
    T_i->m_ptr_list_index = m_ptr_list_count;
    this->m_ptr_list[this->m_ptr_list_count++] = &T_i->m_data;
  }
}

void __thiscall phys_free_list<broad_phase_collision_pair>::remove(
        phys_free_list<broad_phase_collision_pair> *this,
        phys_free_list<broad_phase_collision_pair>::T_internal *data)
{
  phys_free_list<broad_phase_collision_pair>::T_internal_base *m_next_T_internal; // eax
  phys_free_list<broad_phase_collision_pair>::T_internal_base *m_prev_T_internal; // ecx

  if ( !data
    && _tlAssert("c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h", 477, "data", &toastPopupTitle) )
  {
    __debugbreak();
  }
  --this->m_list_count;
  phys_free_list<broad_phase_collision_pair>::debug_remove(this, data);
  m_next_T_internal = data->m_next_T_internal;
  m_prev_T_internal = data->m_prev_T_internal;
  m_prev_T_internal->m_next_T_internal = m_next_T_internal;
  m_next_T_internal->m_prev_T_internal = m_prev_T_internal;
  PMM_FREE((unsigned __int8 *)data, 0x18u, 4u);
}

char  phys_are_potentially_colliding_whace<broad_phase_info,broad_phase_info>@<al>(
        float a1@<ebp>,
        broad_phase_info *p1,
        broad_phase_info *p2,
        float *hit_time)
{
  float v5[3]; // [esp-Ch] [ebp-1Ch] BYREF
  phys_vec3 trans; // [esp+0h] [ebp-10h]
  float retaddr; // [esp+10h] [ebp+0h]

  trans.y = a1;
  trans.z = retaddr;
  v5[0] = p1->m_trace_translation.x - p2->m_trace_translation.x;
  v5[1] = p1->m_trace_translation.y - p2->m_trace_translation.y;
  v5[2] = p1->m_trace_translation.z - p2->m_trace_translation.z;
  return phys_are_potentially_colliding(
           &p1->m_trace_aabb_min_whace,
           &p1->m_trace_aabb_max_whace,
           (const phys_vec3 *)v5,
           &p2->m_trace_aabb_min_whace,
           &p2->m_trace_aabb_max_whace,
           hit_time);
}

void __cdecl add_collision_pair(
        broad_phase_info *bpi1,
        broad_phase_info *bpi2,
        float hit_time,
        phys_gjk_cache_info *gjk_ci)
{
  float *v4; // eax
  float *v5; // esi
  phys_link_list<phys_collision_pair> *p_g_list_phys_collide_data; // edi
  phys_collision_pair **m_last_next_ptr; // eax

  if ( (hit_time < 0.0 || hit_time > 1.0)
    && _tlAssert("source/phys_broad_phase.cpp", 11, "hit_time >= 0.0f && hit_time <= 1.0f", &toastPopupTitle) )
  {
    __debugbreak();
  }
  v4 = (float *)phys_transient_allocator::allocate(
                  &G_BPM->g_collision_memory_buffer,
                  20,
                  4,
                  0,
                  "phys_transient_allocator out of memory.");
  if ( v4 )
  {
    v5 = v4;
    v4[3] = -1.0;
  }
  else
  {
    v5 = 0;
  }
  v5[3] = hit_time;
  *((unsigned int *)v5 + 1) = bpi1;
  *((unsigned int *)v5 + 2) = bpi2;
  *((unsigned int *)v5 + 4) = gjk_ci;
  p_g_list_phys_collide_data = &G_BPM->g_list_phys_collide_data;
  if ( !G_BPM->g_list_phys_collide_data.m_last_next_ptr
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         230,
         "m_last_next_ptr",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  *v5 = 0.0;
  m_last_next_ptr = p_g_list_phys_collide_data->m_last_next_ptr;
  ++p_g_list_phys_collide_data->m_alloc_count;
  *m_last_next_ptr = (phys_collision_pair *)v5;
  p_g_list_phys_collide_data->m_last_next_ptr = (phys_collision_pair **)v5;
}

void __cdecl add_collision_pair_mutex(
        broad_phase_info *bpi1,
        broad_phase_info *bpi2,
        float hit_time,
        phys_gjk_cache_info *gjk_ci)
{
  phys_collision_pair *v4; // eax

  if ( (hit_time < 0.0 || hit_time > 1.0)
    && _tlAssert("source/phys_broad_phase.cpp", 23, "hit_time >= 0.0f && hit_time <= 1.0f", &toastPopupTitle) )
  {
    __debugbreak();
  }
  v4 = (phys_collision_pair *)phys_transient_allocator::mt_allocate(
                                &G_BPM->g_collision_memory_buffer,
                                20,
                                4,
                                0,
                                "broad phase collision out of memory.");
  if ( v4 )
    v4->m_hit_time = -1.0;
  else
    v4 = 0;
  v4->m_hit_time = hit_time;
  v4->m_bpi1 = bpi1;
  v4->m_gjk_ci = gjk_ci;
  v4->m_bpi2 = bpi2;
  phys_link_list<phys_collision_pair>::add_mt(&G_BPM->g_list_phys_collide_data, v4);
}

void __thiscall axis_aligned_sweep_and_prune::add_active_pair(
        axis_aligned_sweep_and_prune *this,
        axis_aligned_sweep_and_prune::sap_node *p1,
        axis_aligned_sweep_and_prune::sap_node *p2)
{
  axis_aligned_sweep_and_prune::active_pair *v4; // eax

  if ( this->m_active_pair_allocator.m_count >= this->m_max_num_active_pairs )
  {
    if ( (pai_max_num_sap_active_pair.m_hits_total_count < pai_max_num_sap_active_pair.m_max_hits_total
       || !pai_max_num_sap_active_pair.m_max_hits_total)
      && pai_max_num_sap_active_pair.m_hits_frame_count < pai_max_num_sap_active_pair.m_max_hits_per_frame )
    {
      if ( pai_max_num_sap_active_pair.m_use_warnings_only )
      {
        PHYS_WARNING(
          "source/phys_broad_phase.cpp",
          309,
          "m_active_pair_allocator.get_count() < m_max_num_active_pairs",
          "max num sap active pairs reached.");
      }
      else if ( _tlAssert(
                  "source/phys_broad_phase.cpp",
                  309,
                  "m_active_pair_allocator.get_count() < m_max_num_active_pairs",
                  "max num sap active pairs reached.") )
      {
        __debugbreak();
      }
    }
    _InterlockedExchangeAdd(&pai_max_num_sap_active_pair.m_hits_total_count, 1u);
    _InterlockedExchangeAdd(&pai_max_num_sap_active_pair.m_hits_frame_count, 1u);
  }
  if ( this->m_active_pair_allocator.m_count < this->m_max_num_active_pairs )
  {
    v4 = (axis_aligned_sweep_and_prune::active_pair *)PMM_ALLOC(0x10u, 4u);
    if ( v4 )
    {
      ++this->m_active_pair_allocator.m_count;
      v4->m_p1 = p1;
      v4->m_p2 = p2;
      v4->m_gjk_ci = 0;
      if ( (p1->m_bpb->m_flags & 1) != 0 )
      {
        if ( (p2->m_bpb->m_flags & 1) != 0 )
        {
          v4->m_next = this->m_list_bpi_bpi;
          this->m_list_bpi_bpi = v4;
        }
        else
        {
          v4->m_next = this->m_list_bpi_bpg;
          this->m_list_bpi_bpg = v4;
        }
      }
      else if ( (p2->m_bpb->m_flags & 1) != 0 )
      {
        v4->m_p1 = p2;
        v4->m_p2 = p1;
        v4->m_gjk_ci = 0;
        v4->m_next = this->m_list_bpi_bpg;
        this->m_list_bpi_bpg = v4;
      }
      else
      {
        v4->m_next = this->m_list_bpg_bpg;
        this->m_list_bpg_bpg = v4;
      }
    }
    else
    {
      if ( (pai_create_sap_active_pair.m_hits_total_count < pai_create_sap_active_pair.m_max_hits_total
         || !pai_create_sap_active_pair.m_max_hits_total)
        && pai_create_sap_active_pair.m_hits_frame_count < pai_create_sap_active_pair.m_max_hits_per_frame )
      {
        if ( pai_create_sap_active_pair.m_use_warnings_only )
        {
          PHYS_WARNING("source/phys_broad_phase.cpp", 313, "ap", "list sap active pairs out of memory.");
        }
        else if ( _tlAssert("source/phys_broad_phase.cpp", 313, "ap", "list sap active pairs out of memory.") )
        {
          __debugbreak();
        }
      }
      _InterlockedExchangeAdd(&pai_create_sap_active_pair.m_hits_total_count, 1u);
      _InterlockedExchangeAdd(&pai_create_sap_active_pair.m_hits_frame_count, 1u);
    }
  }
}

void __thiscall axis_aligned_sweep_and_prune::create_sap_node(
        axis_aligned_sweep_and_prune *this,
        broad_phase_base *bpb)
{
  char *v3; // eax
  axis_aligned_sweep_and_prune::sap_node *v4; // edi

  if ( bpb->m_sap_node && _tlAssert("source/phys_broad_phase.cpp", 496, "bpb->m_sap_node == NULL", &toastPopupTitle) )
    __debugbreak();
  v3 = PMM_ALLOC(0x80u, 4u);
  if ( v3 )
  {
    ++this->m_sap_node_allocator.m_count;
    v4 = (axis_aligned_sweep_and_prune::sap_node *)v3;
  }
  else
  {
    v4 = 0;
  }
  axis_aligned_sweep_and_prune::sap_node::init(v4, bpb, &this->m_x_list, &this->m_y_list, &this->m_z_list);
  bpb->m_sap_node = v4;
}

void __thiscall axis_aligned_sweep_and_prune::remove(
        axis_aligned_sweep_and_prune *this,
        axis_aligned_sweep_and_prune::active_pair **list_ap,
        axis_aligned_sweep_and_prune::sap_node *node)
{
  int i; // esi
  char *v4; // edi
  phys_heap_gjk_cache_system_avl_tree *p_g_phys_gjk_cache_system; // ebx

  for ( i = (int)*list_ap; *list_ap; i = (int)*list_ap )
  {
    if ( *(axis_aligned_sweep_and_prune::sap_node **)i == node
      || *(axis_aligned_sweep_and_prune::sap_node **)(i + 4) == node )
    {
      *list_ap = *(axis_aligned_sweep_and_prune::active_pair **)(i + 8);
      v4 = *(char **)(i + 12);
      p_g_phys_gjk_cache_system = &G_BPM->g_phys_gjk_cache_system;
      if ( v4 )
      {
        PMM_VALIDATE(v4, 0x90u, 0x10u);
        --p_g_phys_gjk_cache_system->m_list_phys_gjk_cache_info_internal.m_count;
        PMM_FREE((unsigned __int8 *)v4, 0x90u, 0x10u);
      }
      PMM_VALIDATE((char *)i, 0x10u, 4u);
      --this->m_active_pair_allocator.m_count;
      PMM_FREE((unsigned __int8 *)i, 0x10u, 4u);
    }
    else
    {
      list_ap = (axis_aligned_sweep_and_prune::active_pair **)(i + 8);
    }
  }
}

void __thiscall axis_aligned_sweep_and_prune::destroy_sap_node(
        axis_aligned_sweep_and_prune *this,
        broad_phase_base *bpb)
{
  axis_aligned_sweep_and_prune::sap_node *m_sap_node; // edi

  m_sap_node = (axis_aligned_sweep_and_prune::sap_node *)bpb->m_sap_node;
  if ( m_sap_node )
  {
    axis_aligned_sweep_and_prune::remove(
      this,
      &this->m_list_bpi_bpi,
      (axis_aligned_sweep_and_prune::sap_node *)bpb->m_sap_node);
    axis_aligned_sweep_and_prune::remove(this, &this->m_list_bpi_bpg, m_sap_node);
    axis_aligned_sweep_and_prune::remove(this, &this->m_list_bpg_bpg, m_sap_node);
    axis_aligned_sweep_and_prune::remove(this, &this->m_x_list, m_sap_node);
    axis_aligned_sweep_and_prune::remove(this, &this->m_y_list, m_sap_node);
    axis_aligned_sweep_and_prune::remove(this, &this->m_z_list, m_sap_node);
    PMM_VALIDATE((char *)m_sap_node, 0x80u, 4u);
    --this->m_sap_node_allocator.m_count;
    PMM_FREE((unsigned __int8 *)m_sap_node, 0x80u, 4u);
    bpb->m_sap_node = 0;
  }
}

void  broad_phase_process_collision_pairs(float a1@<ebp>)
{
  phys_free_list<broad_phase_collision_pair>::T_internal_base *m_next_T_internal; // esi
  phys_free_list<broad_phase_collision_pair> *i; // edi
  const phys_vec3 *m_prev_T_internal; // ecx
  const phys_vec3 *v4; // eax
  phys_vec3 v5; // [esp+18h] [ebp-2Ch] BYREF
  float v6[3]; // [esp+34h] [ebp-10h] BYREF
  float retaddr; // [esp+44h] [ebp+0h]

  v6[1] = a1;
  v6[2] = retaddr;
  v6[0] = 0.0;
  m_next_T_internal = G_BPM->g_list_broad_phase_collision_pair.m_dummy_head.m_next_T_internal;
  for ( i = &G_BPM->g_list_broad_phase_collision_pair;
        i != (phys_free_list<broad_phase_collision_pair> *)m_next_T_internal;
        m_next_T_internal = m_next_T_internal->m_next_T_internal )
  {
    m_prev_T_internal = (const phys_vec3 *)m_next_T_internal[1].m_prev_T_internal;
    v4 = (const phys_vec3 *)m_next_T_internal[1].m_next_T_internal;
    v5.x = m_prev_T_internal[2].x - v4[2].x;
    v5.y = m_prev_T_internal[2].y - v4[2].y;
    v5.z = m_prev_T_internal[2].z - v4[2].z;
    if ( phys_are_potentially_colliding(m_prev_T_internal, m_prev_T_internal + 1, &v5, v4, v4 + 1, v6) )
      add_collision_pair(
        (broad_phase_info *)m_next_T_internal[1].m_prev_T_internal,
        (broad_phase_info *)m_next_T_internal[1].m_next_T_internal,
        v6[0],
        0);
  }
}

void __thiscall phys_free_list<broad_phase_info>::remove_all(phys_free_list<broad_phase_info> *this)
{
  phys_free_list<broad_phase_info>::T_internal *m_next_T_internal; // edi
  phys_free_list<broad_phase_info>::T_internal_base *v3; // eax
  phys_free_list<broad_phase_info>::T_internal_base *m_prev_T_internal; // ecx

  while ( (phys_free_list<broad_phase_info> *)this->m_dummy_head.m_next_T_internal != this )
  {
    m_next_T_internal = (phys_free_list<broad_phase_info>::T_internal *)this->m_dummy_head.m_next_T_internal;
    if ( !m_next_T_internal )
    {
      if ( _tlAssert("c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h", 477, "data", &toastPopupTitle) )
        __debugbreak();
    }
    --this->m_list_count;
    phys_free_list<broad_phase_info>::debug_remove(this, m_next_T_internal);
    v3 = m_next_T_internal->m_next_T_internal;
    m_prev_T_internal = m_next_T_internal->m_prev_T_internal;
    m_prev_T_internal->m_next_T_internal = v3;
    v3->m_prev_T_internal = m_prev_T_internal;
    PMM_FREE((unsigned __int8 *)m_next_T_internal, 0x90u, 0x10u);
  }
}

void __thiscall phys_free_list<broad_phase_collision_pair>::remove_all(
        phys_free_list<broad_phase_collision_pair> *this)
{
  phys_free_list<broad_phase_collision_pair>::T_internal *m_next_T_internal; // edi
  phys_free_list<broad_phase_collision_pair>::T_internal_base *v3; // eax
  phys_free_list<broad_phase_collision_pair>::T_internal_base *m_prev_T_internal; // ecx

  while ( (phys_free_list<broad_phase_collision_pair> *)this->m_dummy_head.m_next_T_internal != this )
  {
    m_next_T_internal = (phys_free_list<broad_phase_collision_pair>::T_internal *)this->m_dummy_head.m_next_T_internal;
    if ( !m_next_T_internal )
    {
      if ( _tlAssert("c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h", 477, "data", &toastPopupTitle) )
        __debugbreak();
    }
    --this->m_list_count;
    phys_free_list<broad_phase_collision_pair>::debug_remove(this, m_next_T_internal);
    v3 = m_next_T_internal->m_next_T_internal;
    m_prev_T_internal = m_next_T_internal->m_prev_T_internal;
    m_prev_T_internal->m_next_T_internal = v3;
    v3->m_prev_T_internal = m_prev_T_internal;
    PMM_FREE((unsigned __int8 *)m_next_T_internal, 0x18u, 4u);
  }
}

// attributes: thunk
void __thiscall phys_heap_gjk_cache_system_avl_tree::~phys_heap_gjk_cache_system_avl_tree(
        phys_heap_gjk_cache_system_avl_tree *this)
{
  phys_heap_gjk_cache_system_avl_tree::shutdown(this);
}

// attributes: thunk
void __thiscall bpei_database_t::~bpei_database_t(bpei_database_t *this)
{
  bpei_database_t::purge_database(this);
}

void __thiscall phys_free_list<broad_phase_info>::~phys_free_list<broad_phase_info>(
        phys_free_list<broad_phase_info> *this)
{
  phys_free_list<broad_phase_info>::remove_all(this);
  if ( this->m_list_count
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         448,
         "m_list_count == 0",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( (phys_free_list<broad_phase_info> *)this->m_dummy_head.m_next_T_internal != this
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         449,
         "m_dummy_head.m_next_T_internal == &m_dummy_head",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( (phys_free_list<broad_phase_info> *)this->m_dummy_head.m_prev_T_internal != this
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         450,
         "m_dummy_head.m_prev_T_internal == &m_dummy_head",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  this->m_list_count_high_water = 0;
  this->m_ptr_list_count = 0;
}

void __thiscall phys_free_list<broad_phase_group>::~phys_free_list<broad_phase_group>(
        phys_free_list<broad_phase_group> *this)
{
  phys_free_list<rigid_body_constraint_distance>::remove_all(this);
  if ( this->m_list_count
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         448,
         "m_list_count == 0",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( (phys_free_list<broad_phase_group> *)this->m_dummy_head.m_next_T_internal != this
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         449,
         "m_dummy_head.m_next_T_internal == &m_dummy_head",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( (phys_free_list<broad_phase_group> *)this->m_dummy_head.m_prev_T_internal != this
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         450,
         "m_dummy_head.m_prev_T_internal == &m_dummy_head",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  this->m_list_count_high_water = 0;
  this->m_ptr_list_count = 0;
}

void __thiscall phys_free_list<broad_phase_collision_pair>::~phys_free_list<broad_phase_collision_pair>(
        phys_free_list<broad_phase_collision_pair> *this)
{
  phys_free_list<broad_phase_collision_pair>::remove_all(this);
  if ( this->m_list_count
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         448,
         "m_list_count == 0",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( (phys_free_list<broad_phase_collision_pair> *)this->m_dummy_head.m_next_T_internal != this
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         449,
         "m_dummy_head.m_next_T_internal == &m_dummy_head",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( (phys_free_list<broad_phase_collision_pair> *)this->m_dummy_head.m_prev_T_internal != this
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         450,
         "m_dummy_head.m_prev_T_internal == &m_dummy_head",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  this->m_list_count_high_water = 0;
  this->m_ptr_list_count = 0;
}

phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *__thiscall phys_heap_gjk_cache_system_avl_tree::get_gjk_cache_info_mutex(
        phys_heap_gjk_cache_system_avl_tree *this,
        unsigned int id1,
        unsigned int id2,
        tlAtomicReadWriteMutex *query_mutex,
        bool __formal)
{
  unsigned int v5; // esi
  unsigned int v6; // edi
  bool v8; // cc
  phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *v10; // eax
  phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *v11; // esi
  phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *v13; // eax
  unsigned int m_id2; // ecx
  phys_gjk_geom_id_pair_key key; // [esp+Ch] [ebp-8h] BYREF
  int savedregs; // [esp+14h] [ebp+0h] BYREF
  bool swapped; // [esp+1Ch] [ebp+8h]

  v5 = id1;
  v6 = id2;
  v8 = id1 <= id2;
  if ( id1 == id2 )
  {
    if ( _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_gjk_cache_system.h",
           347,
           "id1 != id2",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    v8 = id1 <= id2;
  }
  if ( v8 )
  {
    swapped = 0;
  }
  else
  {
    v5 = id2;
    swapped = 1;
    v6 = id1;
  }
  if ( v5 >= v6
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_gjk_cache_system.h",
         24,
         "id1 < id2",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  key.m_id1 = v5;
  key.m_id2 = v6;
  tlAtomicReadWriteMutex::ReadLock(query_mutex);
  v10 = phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::find(
          &this->m_search_tree,
          &key);
  v11 = v10;
  _InterlockedExchangeAdd(&query_mutex->ThisPtr->ReadLockCount, 0xFFFFFFFF);
  if ( v10 )
  {
    phys_gjk_cache_info::update_swapped(v10, (int)&savedregs, swapped);
    return v11;
  }
  if ( this->m_list_phys_gjk_cache_info_internal.m_count >= this->m_max_num_gjk_ci )
  {
    if ( (pai_gjk_cache_system_max_num_gjk_ci.m_hits_total_count < pai_gjk_cache_system_max_num_gjk_ci.m_max_hits_total
       || !pai_gjk_cache_system_max_num_gjk_ci.m_max_hits_total)
      && pai_gjk_cache_system_max_num_gjk_ci.m_hits_frame_count < pai_gjk_cache_system_max_num_gjk_ci.m_max_hits_per_frame )
    {
      if ( pai_gjk_cache_system_max_num_gjk_ci.m_use_warnings_only )
      {
        PHYS_WARNING(
          "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_gjk_cache_system.h",
          368,
          "m_list_phys_gjk_cache_info_internal.get_count() < m_max_num_gjk_ci",
          "max num gjk_ci reached.");
      }
      else if ( _tlAssert(
                  "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_gjk_cache_system.h",
                  368,
                  "m_list_phys_gjk_cache_info_internal.get_count() < m_max_num_gjk_ci",
                  "max num gjk_ci reached.") )
      {
        __debugbreak();
      }
    }
    _InterlockedExchangeAdd(&pai_gjk_cache_system_max_num_gjk_ci.m_hits_total_count, 1u);
    _InterlockedExchangeAdd(&pai_gjk_cache_system_max_num_gjk_ci.m_hits_frame_count, 1u);
  }
  if ( this->m_list_phys_gjk_cache_info_internal.m_count >= this->m_max_num_gjk_ci )
    return v11;
  tlAtomicReadWriteMutex::WriteLock(query_mutex);
  if ( phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::find(
         &this->m_search_tree,
         &key)
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_gjk_cache_system.h",
         372,
         "m_search_tree.find(key) == NULL",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  v13 = (phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *)PMM_ALLOC(0x90u, 0x10u);
  v11 = v13;
  if ( v13 )
  {
    ++this->m_list_phys_gjk_cache_info_internal.m_count;
    m_id2 = key.m_id2;
    v13->m_key.m_id1 = key.m_id1;
    v13->m_key.m_id2 = m_id2;
    v13->m_flags = 0;
    v13->m_flags = swapped ? 2 : 0;
    phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::add(
      &this->m_search_tree,
      &key,
      v13);
    v11->m_next_gjk_ci = this->m_list_head;
    this->m_list_head = v11;
    tlAtomicReadWriteMutex::WriteUnlock(query_mutex);
    return v11;
  }
  if ( (pai_gjk_cache_system_create_gjk_ci.m_hits_total_count < pai_gjk_cache_system_create_gjk_ci.m_max_hits_total
     || !pai_gjk_cache_system_create_gjk_ci.m_max_hits_total)
    && pai_gjk_cache_system_create_gjk_ci.m_hits_frame_count < pai_gjk_cache_system_create_gjk_ci.m_max_hits_per_frame )
  {
    if ( pai_gjk_cache_system_create_gjk_ci.m_use_warnings_only )
    {
      PHYS_WARNING(
        "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_gjk_cache_system.h",
        374,
        "gjk_ci",
        "gjk_ci memory allocation failed.");
    }
    else if ( _tlAssert(
                "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_gjk_cache_system.h",
                374,
                "gjk_ci",
                "gjk_ci memory allocation failed.") )
    {
      __debugbreak();
    }
  }
  _InterlockedExchangeAdd(&pai_gjk_cache_system_create_gjk_ci.m_hits_total_count, 1u);
  _InterlockedExchangeAdd(&pai_gjk_cache_system_create_gjk_ci.m_hits_frame_count, 1u);
  tlAtomicReadWriteMutex::WriteUnlock(query_mutex);
  return 0;
}

broad_phase_memory *__thiscall broad_phase_memory::broad_phase_memory(broad_phase_memory *this)
{
  broad_phase_memory *result; // eax

  result = this;
  this->g_bp_auto_activate_mutex.ThisPtr = &this->g_bp_auto_activate_mutex;
  this->g_bp_auto_activate_mutex.ThreadId = 0;
  this->g_bp_auto_activate_mutex.LockCount = 0;
  this->g_bp_gjk_cache_mutex.WriteThreadId = 0;
  this->g_bp_gjk_cache_mutex.ReadLockCount = 0;
  this->g_bp_gjk_cache_mutex.WriteLockCount = 0;
  this->g_bp_gjk_cache_mutex.ThisPtr = &this->g_bp_gjk_cache_mutex;
  this->g_phys_gjk_cache_system.m_list_phys_gjk_cache_info_internal.m_count = 0;
  this->g_phys_gjk_cache_system.m_search_tree.m_tree_root = 0;
  this->g_list_broad_phase_info.m_list_count = 0;
  this->g_list_broad_phase_info.m_list_count_high_water = 0;
  this->g_list_broad_phase_info.m_ptr_list_count = 0;
  this->g_list_broad_phase_info.m_dummy_head.m_next_T_internal = &this->g_list_broad_phase_info.m_dummy_head;
  this->g_list_broad_phase_info.m_dummy_head.m_prev_T_internal = &this->g_list_broad_phase_info.m_dummy_head;
  this->g_list_broad_phase_group.m_list_count = 0;
  this->g_list_broad_phase_group.m_list_count_high_water = 0;
  this->g_list_broad_phase_group.m_ptr_list_count = 0;
  this->g_list_broad_phase_group.m_dummy_head.m_next_T_internal = &this->g_list_broad_phase_group.m_dummy_head;
  this->g_list_broad_phase_group.m_dummy_head.m_prev_T_internal = &this->g_list_broad_phase_group.m_dummy_head;
  this->g_list_broad_phase_collision_pair.m_list_count = 0;
  this->g_list_broad_phase_collision_pair.m_list_count_high_water = 0;
  this->g_list_broad_phase_collision_pair.m_ptr_list_count = 0;
  this->g_list_broad_phase_collision_pair.m_dummy_head.m_next_T_internal = &this->g_list_broad_phase_collision_pair.m_dummy_head;
  this->g_list_broad_phase_collision_pair.m_dummy_head.m_prev_T_internal = &this->g_list_broad_phase_collision_pair.m_dummy_head;
  this->g_bpei_database.m_bpei_map.m_tree_root = 0;
  this->g_bpei_database.m_bpei_allocator.m_count = 0;
  this->g_bpei_database.m_mutex.m_count = 1;
  this->g_bpei_database.m_bpei_list = 0;
  this->g_list_phys_collide_data.m_first = 0;
  this->g_list_phys_collide_data.m_alloc_count = 0;
  this->g_list_phys_collide_data.m_last_next_ptr = &this->g_list_phys_collide_data.m_first;
  this->g_collision_memory_buffer.m_first_block = 0;
  this->g_collision_memory_buffer.m_cur = 0;
  this->g_collision_memory_buffer.m_end = 0;
  this->g_collision_memory_buffer.m_total_memory_allocated = 0;
  this->g_collision_memory_buffer.m_mutex.m_count = 1;
  this->g_collision_memory_buffer.m_slot_pool = 0;
  return result;
}

void  axis_aligned_sweep_and_prune::process_active_pair_list(
        axis_aligned_sweep_and_prune *this@<ecx>,
        axis_aligned_sweep_and_prune::active_pair **a2@<ebp>)
{
  axis_aligned_sweep_and_prune::active_pair **p_m_list_bpi_bpi; // edx
  axis_aligned_sweep_and_prune::active_pair *m_list_bpi_bpi; // edi
  axis_aligned_sweep_and_prune::sap_node *m_p1; // eax
  axis_aligned_sweep_and_prune::sap_node *m_p2; // ecx
  broad_phase_base *m_bpb; // eax
  bool v7; // zf
  broad_phase_info **v8; // eax
  broad_phase_info *v9; // esi
  phys_heap_gjk_cache_system_avl_tree *v10; // esi
  char *v11; // eax
  char *m_gjk_ci; // esi
  axis_aligned_sweep_and_prune::active_pair *m_list_bpi_bpg; // esi
  axis_aligned_sweep_and_prune::active_pair **p_m_list_bpi_bpg; // edx
  axis_aligned_sweep_and_prune::sap_node *v15; // eax
  axis_aligned_sweep_and_prune::sap_node *v16; // ecx
  broad_phase_info *v17; // edi
  axis_aligned_sweep_and_prune::sap_node *v18; // eax
  broad_phase_base *v19; // esi
  broad_phase_info *i; // esi
  char *v21; // edi
  axis_aligned_sweep_and_prune::active_pair *m_list_bpg_bpg; // edi
  axis_aligned_sweep_and_prune::active_pair **p_m_list_bpg_bpg; // edx
  axis_aligned_sweep_and_prune::sap_node *v24; // eax
  axis_aligned_sweep_and_prune::sap_node *v25; // ecx
  broad_phase_base *v26; // esi
  broad_phase_info **v27; // eax
  broad_phase_info *v28; // edi
  broad_phase_info *j; // esi
  broad_phase_info *k; // edi
  char *v31; // esi
  phys_vec3 v32; // [esp+18h] [ebp-4Ch] BYREF
  phys_vec3 v33; // [esp+28h] [ebp-3Ch] BYREF
  phys_heap_gjk_cache_system_avl_tree *p_g_phys_gjk_cache_system; // [esp+44h] [ebp-20h]
  broad_phase_info *v35; // [esp+48h] [ebp-1Ch]
  axis_aligned_sweep_and_prune::active_pair **p_m_next; // [esp+4Ch] [ebp-18h]
  axis_aligned_sweep_and_prune *v37; // [esp+50h] [ebp-14h]
  float v38; // [esp+54h] [ebp-10h] BYREF
  axis_aligned_sweep_and_prune::active_pair **ap_i; // [esp+58h] [ebp-Ch]
  void *v40; // [esp+5Ch] [ebp-8h]
  void *retaddr; // [esp+64h] [ebp+0h]

  ap_i = a2;
  v40 = retaddr;
  p_m_list_bpi_bpi = &this->m_list_bpi_bpi;
  m_list_bpi_bpi = this->m_list_bpi_bpi;
  v37 = this;
  p_m_next = &this->m_list_bpi_bpi;
  if ( m_list_bpi_bpi )
  {
    while ( 1 )
    {
      m_p1 = m_list_bpi_bpi->m_p1;
      m_p2 = m_list_bpi_bpi->m_p2;
      if ( m_list_bpi_bpi->m_p1->m_ae1[0][0].m_ae_list_index >= m_p2->m_ae1[0][1].m_ae_list_index
        || m_p2->m_ae1[0][0].m_ae_list_index >= m_p1->m_ae1[0][1].m_ae_list_index
        || m_p1->m_ae1[1][0].m_ae_list_index >= m_p2->m_ae1[1][1].m_ae_list_index
        || m_p2->m_ae1[1][0].m_ae_list_index >= m_p1->m_ae1[1][1].m_ae_list_index
        || m_p1->m_ae1[2][0].m_ae_list_index >= m_p2->m_ae1[2][1].m_ae_list_index
        || m_p2->m_ae1[2][0].m_ae_list_index >= m_p1->m_ae1[2][1].m_ae_list_index )
      {
        *p_m_list_bpi_bpi = m_list_bpi_bpi->m_next;
        m_gjk_ci = (char *)m_list_bpi_bpi->m_gjk_ci;
        p_g_phys_gjk_cache_system = &G_BPM->g_phys_gjk_cache_system;
        if ( m_gjk_ci )
        {
          PMM_VALIDATE(m_gjk_ci, 0x90u, 0x10u);
          --p_g_phys_gjk_cache_system->m_list_phys_gjk_cache_info_internal.m_count;
          PMM_FREE((unsigned __int8 *)m_gjk_ci, 0x90u, 0x10u);
        }
        PMM_VALIDATE((char *)m_list_bpi_bpi, 0x10u, 4u);
        --v37->m_active_pair_allocator.m_count;
        PMM_FREE((unsigned __int8 *)m_list_bpi_bpi, 0x10u, 4u);
      }
      else
      {
        m_bpb = m_p1->m_bpb;
        v7 = (m_bpb->m_flags & 1) == 0;
        p_m_next = &m_list_bpi_bpi->m_next;
        v35 = (broad_phase_info *)m_bpb;
        if ( v7
          && _tlAssert(
               "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_broad_phase_base.h",
               99,
               "is_bpi()",
               &toastPopupTitle) )
        {
          __debugbreak();
        }
        v8 = (broad_phase_info **)m_list_bpi_bpi->m_p2;
        v9 = *v8;
        v7 = ((*v8)->m_flags & 1) == 0;
        p_g_phys_gjk_cache_system = (phys_heap_gjk_cache_system_avl_tree *)*v8;
        if ( v7
          && _tlAssert(
               "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_broad_phase_base.h",
               99,
               "is_bpi()",
               &toastPopupTitle) )
        {
          __debugbreak();
        }
        v33.x = v35->m_trace_translation.x - v9->m_trace_translation.x;
        v33.y = v35->m_trace_translation.y - v9->m_trace_translation.y;
        v33.z = v35->m_trace_translation.z - v9->m_trace_translation.z;
        if ( phys_are_potentially_colliding(
               &v35->m_trace_aabb_min_whace,
               &v35->m_trace_aabb_max_whace,
               &v33,
               &v9->m_trace_aabb_min_whace,
               &v9->m_trace_aabb_max_whace,
               &v38) )
        {
          if ( !m_list_bpi_bpi->m_gjk_ci )
          {
            v10 = &G_BPM->g_phys_gjk_cache_system;
            v11 = PMM_ALLOC(0x90u, 0x10u);
            if ( v11 )
            {
              ++v10->m_list_phys_gjk_cache_info_internal.m_count;
              *((unsigned int *)v11 + 29) = 1;
              *((unsigned int *)v11 + 30) = 2;
              *((unsigned int *)v11 + 31) = 0;
            }
            else
            {
              v11 = 0;
            }
            v9 = (broad_phase_info *)p_g_phys_gjk_cache_system;
            m_list_bpi_bpi->m_gjk_ci = (phys_gjk_cache_info *)v11;
          }
          add_collision_pair(v35, v9, v38, m_list_bpi_bpi->m_gjk_ci);
        }
      }
      m_list_bpi_bpi = *p_m_next;
      if ( !*p_m_next )
        break;
      p_m_list_bpi_bpi = p_m_next;
    }
    this = v37;
  }
  m_list_bpi_bpg = this->m_list_bpi_bpg;
  p_m_list_bpi_bpg = &this->m_list_bpi_bpg;
  p_m_next = &this->m_list_bpi_bpg;
  if ( m_list_bpi_bpg )
  {
    while ( 1 )
    {
      v15 = m_list_bpi_bpg->m_p1;
      v16 = m_list_bpi_bpg->m_p2;
      if ( m_list_bpi_bpg->m_p1->m_ae1[0][0].m_ae_list_index >= v16->m_ae1[0][1].m_ae_list_index
        || v16->m_ae1[0][0].m_ae_list_index >= v15->m_ae1[0][1].m_ae_list_index
        || v15->m_ae1[1][0].m_ae_list_index >= v16->m_ae1[1][1].m_ae_list_index
        || v16->m_ae1[1][0].m_ae_list_index >= v15->m_ae1[1][1].m_ae_list_index
        || v15->m_ae1[2][0].m_ae_list_index >= v16->m_ae1[2][1].m_ae_list_index
        || v16->m_ae1[2][0].m_ae_list_index >= v15->m_ae1[2][1].m_ae_list_index )
      {
        *p_m_list_bpi_bpg = m_list_bpi_bpg->m_next;
        v21 = (char *)m_list_bpi_bpg->m_gjk_ci;
        p_g_phys_gjk_cache_system = &G_BPM->g_phys_gjk_cache_system;
        if ( v21 )
        {
          PMM_VALIDATE(v21, 0x90u, 0x10u);
          --p_g_phys_gjk_cache_system->m_list_phys_gjk_cache_info_internal.m_count;
          PMM_FREE((unsigned __int8 *)v21, 0x90u, 0x10u);
        }
        PMM_VALIDATE((char *)m_list_bpi_bpg, 0x10u, 4u);
        --v37->m_active_pair_allocator.m_count;
        PMM_FREE((unsigned __int8 *)m_list_bpi_bpg, 0x10u, 4u);
      }
      else
      {
        v17 = (broad_phase_info *)v15->m_bpb;
        v7 = (v15->m_bpb->m_flags & 1) == 0;
        p_m_next = &m_list_bpi_bpg->m_next;
        if ( v7
          && _tlAssert(
               "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_broad_phase_base.h",
               99,
               "is_bpi()",
               &toastPopupTitle) )
        {
          __debugbreak();
        }
        v18 = m_list_bpi_bpg->m_p2;
        v19 = v18->m_bpb;
        if ( (v18->m_bpb->m_flags & 2) == 0
          && _tlAssert(
               "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_broad_phase_base.h",
               100,
               "is_bpg()",
               &toastPopupTitle) )
        {
          __debugbreak();
        }
        v33.x = v17->m_trace_translation.x - v19->m_trace_translation.x;
        v33.y = v17->m_trace_translation.y - v19->m_trace_translation.y;
        v33.z = v17->m_trace_translation.z - v19->m_trace_translation.z;
        if ( phys_are_potentially_colliding(
               &v17->m_trace_aabb_min_whace,
               &v17->m_trace_aabb_max_whace,
               &v33,
               &v19->m_trace_aabb_min_whace,
               &v19->m_trace_aabb_max_whace,
               &v38) )
        {
          for ( i = (broad_phase_info *)LODWORD(v19[1].m_trace_aabb_min_whace.x);
                i;
                i = (broad_phase_info *)i->m_list_bpb_next )
          {
            v32.x = v17->m_trace_translation.x - i->m_trace_translation.x;
            v32.y = v17->m_trace_translation.y - i->m_trace_translation.y;
            v32.z = v17->m_trace_translation.z - i->m_trace_translation.z;
            if ( phys_are_potentially_colliding(
                   &v17->m_trace_aabb_min_whace,
                   &v17->m_trace_aabb_max_whace,
                   &v32,
                   &i->m_trace_aabb_min_whace,
                   &i->m_trace_aabb_max_whace,
                   &v38) )
            {
              add_collision_pair(v17, i, v38, 0);
            }
          }
        }
      }
      m_list_bpi_bpg = *p_m_next;
      if ( !*p_m_next )
        break;
      p_m_list_bpi_bpg = p_m_next;
    }
    this = v37;
  }
  m_list_bpg_bpg = this->m_list_bpg_bpg;
  p_m_list_bpg_bpg = &this->m_list_bpg_bpg;
  p_m_next = &this->m_list_bpg_bpg;
  if ( m_list_bpg_bpg )
  {
    while ( 1 )
    {
      v24 = m_list_bpg_bpg->m_p1;
      v25 = m_list_bpg_bpg->m_p2;
      if ( m_list_bpg_bpg->m_p1->m_ae1[0][0].m_ae_list_index >= v25->m_ae1[0][1].m_ae_list_index
        || v25->m_ae1[0][0].m_ae_list_index >= v24->m_ae1[0][1].m_ae_list_index
        || v24->m_ae1[1][0].m_ae_list_index >= v25->m_ae1[1][1].m_ae_list_index
        || v25->m_ae1[1][0].m_ae_list_index >= v24->m_ae1[1][1].m_ae_list_index
        || v24->m_ae1[2][0].m_ae_list_index >= v25->m_ae1[2][1].m_ae_list_index
        || v25->m_ae1[2][0].m_ae_list_index >= v24->m_ae1[2][1].m_ae_list_index )
      {
        *p_m_list_bpg_bpg = m_list_bpg_bpg->m_next;
        v31 = (char *)m_list_bpg_bpg->m_gjk_ci;
        p_g_phys_gjk_cache_system = &G_BPM->g_phys_gjk_cache_system;
        if ( v31 )
        {
          PMM_VALIDATE(v31, 0x90u, 0x10u);
          --p_g_phys_gjk_cache_system->m_list_phys_gjk_cache_info_internal.m_count;
          PMM_FREE((unsigned __int8 *)v31, 0x90u, 0x10u);
        }
        PMM_VALIDATE((char *)m_list_bpg_bpg, 0x10u, 4u);
        --v37->m_active_pair_allocator.m_count;
        PMM_FREE((unsigned __int8 *)m_list_bpg_bpg, 0x10u, 4u);
      }
      else
      {
        v26 = v24->m_bpb;
        v7 = (v24->m_bpb->m_flags & 2) == 0;
        p_m_next = &m_list_bpg_bpg->m_next;
        if ( v7
          && _tlAssert(
               "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_broad_phase_base.h",
               100,
               "is_bpg()",
               &toastPopupTitle) )
        {
          __debugbreak();
        }
        v27 = (broad_phase_info **)m_list_bpg_bpg->m_p2;
        v28 = *v27;
        v7 = ((*v27)->m_flags & 2) == 0;
        v35 = *v27;
        if ( v7
          && _tlAssert(
               "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_broad_phase_base.h",
               100,
               "is_bpg()",
               &toastPopupTitle) )
        {
          __debugbreak();
        }
        for ( j = (broad_phase_info *)LODWORD(v26[1].m_trace_aabb_min_whace.x);
              j;
              j = (broad_phase_info *)j->m_list_bpb_next )
        {
          v32.x = j->m_trace_translation.x - v28->m_trace_translation.x;
          v32.y = j->m_trace_translation.y - v28->m_trace_translation.y;
          v32.z = j->m_trace_translation.z - v28->m_trace_translation.z;
          if ( phys_are_potentially_colliding(
                 &j->m_trace_aabb_min_whace,
                 &j->m_trace_aabb_max_whace,
                 &v32,
                 &v28->m_trace_aabb_min_whace,
                 &v28->m_trace_aabb_max_whace,
                 &v38) )
          {
            for ( k = (broad_phase_info *)v28->m_rb; k; k = (broad_phase_info *)k->m_list_bpb_next )
            {
              v33.x = j->m_trace_translation.x - k->m_trace_translation.x;
              v33.y = j->m_trace_translation.y - k->m_trace_translation.y;
              v33.z = j->m_trace_translation.z - k->m_trace_translation.z;
              if ( phys_are_potentially_colliding(
                     &j->m_trace_aabb_min_whace,
                     &j->m_trace_aabb_max_whace,
                     &v33,
                     &k->m_trace_aabb_min_whace,
                     &k->m_trace_aabb_max_whace,
                     &v38) )
              {
                add_collision_pair(j, k, v38, 0);
              }
            }
            v28 = v35;
          }
        }
      }
      m_list_bpg_bpg = *p_m_next;
      if ( !*p_m_next )
        break;
      p_m_list_bpg_bpg = p_m_next;
    }
  }
}

void __thiscall axis_aligned_sweep_and_prune::process(axis_aligned_sweep_and_prune *this)
{
  bool v1; // sf
  int v2; // esi
  axis_aligned_sweep_and_prune::axis_element **v3; // ebx
  axis_aligned_sweep_and_prune::axis_element **v4; // eax
  axis_aligned_sweep_and_prune::axis_element *m_y_list; // edx
  axis_aligned_sweep_and_prune::axis_element *m_x_list; // edi
  axis_aligned_sweep_and_prune::axis_element *m_z_list; // ecx
  axis_aligned_sweep_and_prune::axis_element **v8; // eax
  int v9; // esi
  axis_aligned_sweep_and_prune::axis_element **v10; // eax
  axis_aligned_sweep_and_prune::axis_element *v11; // ecx
  axis_aligned_sweep_and_prune::axis_element **v12; // edx
  axis_aligned_sweep_and_prune::axis_element *m_next; // edx
  axis_aligned_sweep_and_prune::axis_element *v14; // ecx
  axis_aligned_sweep_and_prune::axis_element **v15; // edx
  axis_aligned_sweep_and_prune::axis_element *v16; // ecx
  int v17; // esi
  axis_aligned_sweep_and_prune::axis_element **v18; // edi
  axis_aligned_sweep_and_prune *v19; // esi
  axis_aligned_sweep_and_prune::axis_element **v20; // ecx
  axis_aligned_sweep_and_prune::axis_element **v21; // edx
  axis_aligned_sweep_and_prune::axis_element **i; // eax
  axis_aligned_sweep_and_prune::axis_element *v23; // edi
  axis_aligned_sweep_and_prune::axis_element *v24; // ebx
  int v25; // [esp-4h] [ebp-60h]
  phys_transient_allocator transient_buffer; // [esp+10h] [ebp-4Ch] BYREF
  axis_aligned_sweep_and_prune::axis_element **zlist; // [esp+28h] [ebp-34h]
  int axis_element_count; // [esp+2Ch] [ebp-30h]
  axis_aligned_sweep_and_prune::axis_element **ylist; // [esp+30h] [ebp-2Ch]
  axis_aligned_sweep_and_prune::axis_element **zl_i; // [esp+34h] [ebp-28h]
  axis_aligned_sweep_and_prune::axis_element **yl_i; // [esp+38h] [ebp-24h]
  axis_aligned_sweep_and_prune::axis_element **xl_i; // [esp+3Ch] [ebp-20h]
  axis_aligned_sweep_and_prune::axis_element **xl_i_last; // [esp+40h] [ebp-1Ch]
  axis_aligned_sweep_and_prune::axis_element *zcur; // [esp+44h] [ebp-18h]
  axis_aligned_sweep_and_prune::axis_element *ycur; // [esp+48h] [ebp-14h]
  axis_aligned_sweep_and_prune *v36; // [esp+4Ch] [ebp-10h]
  int v37; // [esp+58h] [ebp-4h]
  axis_aligned_sweep_and_prune::active_pair *savedregs; // [esp+5Ch] [ebp+0h] BYREF

  v36 = this;
  v1 = (this->m_sap_node_allocator.m_count & 0x40000000) != 0;
  axis_element_count = 2 * this->m_sap_node_allocator.m_count;
  if ( !v1 && axis_element_count != 0 )
  {
    memset(&transient_buffer, 0, 16);
    transient_buffer.m_mutex.m_count = 1;
    transient_buffer.m_slot_pool = 0;
    v2 = 4 * axis_element_count;
    v37 = 0;
    v3 = (axis_aligned_sweep_and_prune::axis_element **)phys_transient_allocator::allocate(
                                                          &transient_buffer,
                                                          4 * axis_element_count,
                                                          4,
                                                          0,
                                                          "phys_transient_allocator out of memory.");
    ylist = (axis_aligned_sweep_and_prune::axis_element **)phys_transient_allocator::allocate(
                                                             &transient_buffer,
                                                             v2,
                                                             4,
                                                             0,
                                                             "phys_transient_allocator out of memory.");
    v4 = (axis_aligned_sweep_and_prune::axis_element **)phys_transient_allocator::allocate(
                                                          &transient_buffer,
                                                          v2,
                                                          4,
                                                          0,
                                                          "phys_transient_allocator out of memory.");
    m_y_list = v36->m_y_list;
    m_x_list = v36->m_x_list;
    m_z_list = v36->m_z_list;
    zlist = v4;
    zl_i = v4;
    v8 = &v3[v2 / 4u];
    ycur = m_y_list;
    v9 = 0;
    zcur = m_z_list;
    xl_i = v3;
    yl_i = ylist;
    xl_i_last = v8;
    if ( v3 != v8 )
    {
      do
      {
        if ( m_x_list->m_node->m_updated != 1
          && _tlAssert("source/phys_broad_phase.cpp", 451, "xcur->m_node->m_updated == 1", &toastPopupTitle) )
        {
          __debugbreak();
        }
        if ( ycur->m_node->m_updated != 1
          && _tlAssert("source/phys_broad_phase.cpp", 452, "ycur->m_node->m_updated == 1", &toastPopupTitle) )
        {
          __debugbreak();
        }
        if ( zcur->m_node->m_updated != 1
          && _tlAssert("source/phys_broad_phase.cpp", 453, "zcur->m_node->m_updated == 1", &toastPopupTitle) )
        {
          __debugbreak();
        }
        v10 = xl_i;
        v11 = ycur;
        v12 = yl_i;
        *xl_i = m_x_list;
        m_x_list->m_ae_list_index = v9;
        m_x_list = m_x_list->m_next;
        *v12 = v11;
        yl_i = v12 + 1;
        m_next = v11->m_next;
        v11->m_ae_list_index = v9;
        v14 = zcur;
        ycur = m_next;
        v15 = zl_i;
        *zl_i = zcur;
        v14->m_ae_list_index = v9;
        v16 = v14->m_next;
        ++v9;
        xl_i = v10 + 1;
        zl_i = v15 + 1;
        zcur = v16;
      }
      while ( v10 + 1 != xl_i_last );
    }
    if ( (m_x_list || ycur || zcur)
      && _tlAssert("source/phys_broad_phase.cpp", 459, "xcur == NULL && ycur == NULL && zcur == NULL", &toastPopupTitle) )
    {
      __debugbreak();
    }
    v17 = axis_element_count;
    axis_aligned_sweep_and_prune::merge_sort(v36, v3, axis_element_count);
    v18 = ylist;
    v25 = v17;
    v19 = v36;
    axis_aligned_sweep_and_prune::merge_sort(v36, ylist, v25);
    axis_aligned_sweep_and_prune::merge_sort(v19, zlist, axis_element_count);
    v19->m_x_list = *v3;
    v20 = zlist;
    v19->m_y_list = *v18;
    v19->m_z_list = *v20;
    v21 = v3;
    for ( i = v18; v21 != xl_i_last; v23->m_next = v24 )
    {
      (*v21)->m_node->m_updated = 0;
      (*i)->m_node->m_updated = 0;
      (*v20)->m_node->m_updated = 0;
      (*v21)->m_next = v21[1];
      (*i)->m_next = i[1];
      v23 = *v20;
      v24 = v20[1];
      ++i;
      ++v20;
      ++v21;
    }
    (*(v21 - 1))->m_next = 0;
    (*(i - 1))->m_next = 0;
    (*(v20 - 1))->m_next = 0;
    phys_transient_allocator::reset(&transient_buffer);
    v37 = -1;
    if ( transient_buffer.m_first_block
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_transient_allocator.h",
           69,
           "m_first_block == NULL",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
  }
  axis_aligned_sweep_and_prune::process_active_pair_list(v36, &savedregs);
}

void __cdecl process_cluster_environment_collision(broad_phase_base *bpb, broad_phase_base_list::node *bpeqr)
{
  broad_phase_base *i; // esi
  unsigned int m_flags; // eax
  int savedregs; // [esp+4h] [ebp+0h] BYREF

  if ( (!bpb || !bpb->m_list_bpb_cluster_next)
    && _tlAssert("source/phys_broad_phase.cpp", 888, "bpb && bpb->get_bpb_cluster_next()", &toastPopupTitle) )
  {
    __debugbreak();
  }
  for ( i = bpb->m_list_bpb_cluster_next; i; i = i->m_list_bpb_cluster_next )
  {
    if ( (i->m_flags & 0x10) == 0
      && _tlAssert(
           "source/phys_broad_phase.cpp",
           891,
           "bpb->get_flag(broad_phase_base::FLAG_IS_IN_CLUSTER)",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    m_flags = i->m_flags;
    if ( (m_flags & 1) != 0 )
    {
      collide_bpi_environment(*(float *)&i, bpeqr);
    }
    else
    {
      if ( (m_flags & 2) == 0 )
      {
        if ( _tlAssert(
               "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_broad_phase_base.h",
               100,
               "is_bpg()",
               &toastPopupTitle) )
        {
          __debugbreak();
        }
      }
      collide_bpg_environment(
        (broad_phase_base_list::node *)&savedregs,
        (broad_phase_info *)i,
        (const broad_phase_environement_query_results *)bpeqr);
    }
  }
}

void __thiscall broad_phase_memory::~broad_phase_memory(broad_phase_memory *this)
{
  if ( this->g_collision_memory_buffer.m_first_block
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_transient_allocator.h",
         69,
         "m_first_block == NULL",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  bpei_database_t::purge_database(&this->g_bpei_database);
  phys_free_list<broad_phase_collision_pair>::~phys_free_list<broad_phase_collision_pair>(&this->g_list_broad_phase_collision_pair);
  phys_free_list<broad_phase_group>::~phys_free_list<broad_phase_group>(&this->g_list_broad_phase_group);
  phys_free_list<broad_phase_info>::~phys_free_list<broad_phase_info>(&this->g_list_broad_phase_info);
  phys_heap_gjk_cache_system_avl_tree::shutdown(&this->g_phys_gjk_cache_system);
  this->g_bp_gjk_cache_mutex.ThisPtr = 0;
  LODWORD(this->g_bp_auto_activate_mutex.ThreadId) = 0;
  HIDWORD(this->g_bp_auto_activate_mutex.ThreadId) = 0;
  this->g_bp_auto_activate_mutex.ThisPtr = 0;
}

broad_phase_memory *__cdecl broad_phase_memory::allocate_buffer(const broad_phase_memory_info *bpmi)
{
  broad_phase_memory *v1; // eax
  broad_phase_memory *v2; // esi
  int v3; // ecx
  int m_max_num_surface_types; // ecx
  int v5; // eax
  phys_surface_type_info *v6; // eax
  int v7; // ecx
  bool v8; // cc
  int v9; // edx
  phys_surface_type_info *g_surface_type_info_database; // eax
  phys_surface_type_info *v11; // eax

  v1 = (broad_phase_memory *)PMM_PERM_ALLOCATE(0xCD8u, 8u);
  if ( v1 )
    v2 = broad_phase_memory::broad_phase_memory(v1);
  else
    v2 = 0;
  v3 = bpmi->m_max_num_gjk_ci + bpmi->m_max_num_sap_active_pairs;
  v2->g_phys_gjk_cache_system.m_list_head = 0;
  v2->g_phys_gjk_cache_system.m_max_num_gjk_ci = v3;
  m_max_num_surface_types = bpmi->m_max_num_surface_types;
  v5 = m_max_num_surface_types * (m_max_num_surface_types + 1) / 2;
  v2->m_max_num_surface_types = m_max_num_surface_types;
  v2->m_max_num_surface_type_infos = v5;
  if ( v5 <= 0 )
  {
    v2->g_surface_type_info_database = 0;
    return v2;
  }
  v6 = (phys_surface_type_info *)PMM_PERM_ALLOCATE(16 * v5, 4u);
  v7 = 0;
  v8 = v2->m_max_num_surface_type_infos <= 0;
  v2->g_surface_type_info_database = v6;
  if ( v8 )
    return v2;
  v9 = 0;
  do
  {
    g_surface_type_info_database = v2->g_surface_type_info_database;
    g_surface_type_info_database[v9].m_friction_coef = 0.0;
    v11 = &g_surface_type_info_database[v9];
    ++v7;
    v11->m_bounce_coef = 0.0;
    v11->m_solver_priority = 0;
    v11->m_no_overflow_error = 0;
    ++v9;
  }
  while ( v7 < v2->m_max_num_surface_type_infos );
  return v2;
}

