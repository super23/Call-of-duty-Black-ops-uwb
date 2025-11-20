#include "phys_collision.h"

void __thiscall contact_point_info::get_closest_psc(
        contact_point_info *this,
        const phys_vec3 *normal,
        const phys_vec3 *b1_r_loc,
        const phys_vec3 *b2_r_loc,
        float *closest_error,
        const contact_point_info::pulse_sum_cache_info **closest_psc)
{
  phys_vec3 *m_list_b1_r_loc; // edx
  phys_vec3 *m_list_b2_r_loc; // esi
  const phys_vec3 *v8; // edi
  double v9; // st7
  double v10; // st6
  float v11; // [esp-30h] [ebp-3Ch]
  float v12; // [esp-2Ch] [ebp-38h]
  float v13; // [esp-28h] [ebp-34h]
  float v14; // [esp-20h] [ebp-2Ch]
  float v15; // [esp-20h] [ebp-2Ch]
  float v16; // [esp-1Ch] [ebp-28h]
  float v17; // [esp-1Ch] [ebp-28h]
  float v18; // [esp-18h] [ebp-24h]
  float v19; // [esp-18h] [ebp-24h]
  float v20; // [esp-Ch] [ebp-18h]
  float v21; // [esp-Ch] [ebp-18h]
  float v22; // [esp-Ch] [ebp-18h]
  float v23; // [esp-Ch] [ebp-18h]
  int v24; // [esp-8h] [ebp-14h]
  int v25; // [esp-4h] [ebp-10h]

  m_list_b1_r_loc = this->m_list_b1_r_loc;
  m_list_b2_r_loc = this->m_list_b2_r_loc;
  v24 = 0;
  if ( this->m_point_pair_count > 0 )
  {
    v8 = b1_r_loc;
    v14 = normal->x - this->m_normal.x;
    v16 = normal->y - this->m_normal.y;
    v18 = normal->z - this->m_normal.z;
    v20 = v14 * v14 + v16 * v16 + v18 * v18;
    v9 = v20;
    v25 = 0;
    do
    {
      v15 = m_list_b1_r_loc->x - v8->x;
      v17 = m_list_b1_r_loc->y - v8->y;
      v19 = m_list_b1_r_loc->z - v8->z;
      v11 = m_list_b2_r_loc->x - b2_r_loc->x;
      v12 = m_list_b2_r_loc->y - b2_r_loc->y;
      v13 = m_list_b2_r_loc->z - b2_r_loc->z;
      v21 = v19 * v19 + v15 * v15 + v17 * v17;
      v10 = v21 + v9;
      v22 = v13 * v13 + v12 * v12 + v11 * v11;
      v23 = v10 + v22;
      if ( *closest_error > (double)v23 )
      {
        *closest_error = v23;
        *closest_psc = &this->m_list_pulse_sum_cache_info[v25];
        v8 = b1_r_loc;
      }
      ++v24;
      ++v25;
      ++m_list_b1_r_loc;
      ++m_list_b2_r_loc;
    }
    while ( v24 < this->m_point_pair_count );
  }
}

void __userpurge phys_contact_manifold::xform_and_translate_mesh_points(
        phys_contact_manifold *this@<ecx>,
        int a2@<ebp>,
        const phys_mat44 *xform,
        const phys_vec3 *translation)
{
  phys_contact_manifold *v4; // esi
  const phys_vec3 **m_list_sorted_mesh_point; // eax
  const phys_vec3 **v6; // ecx
  phys_vec3 *v7; // esi
  const phys_vec3 *v8; // eax
  double v9; // st7
  const phys_vec3 **v10; // eax
  const phys_vec3 *v11; // eax
  phys_vec3 v12; // [esp-40h] [ebp-4Ch] BYREF
  float v13; // [esp-30h] [ebp-3Ch]
  float v14; // [esp-2Ch] [ebp-38h]
  float v15; // [esp-28h] [ebp-34h]
  float v16; // [esp-20h] [ebp-2Ch]
  float v17; // [esp-1Ch] [ebp-28h]
  float v18; // [esp-18h] [ebp-24h]
  phys_contact_manifold *v19; // [esp-Ch] [ebp-18h]
  const phys_vec3 **v20; // [esp-8h] [ebp-14h]
  const phys_vec3 **v21; // [esp-4h] [ebp-10h]
  int v22; // [esp+0h] [ebp-Ch]
  contact_manifold_mesh_point **last_mp_i; // [esp+4h] [ebp-8h]
  contact_manifold_mesh_point **retaddr; // [esp+Ch] [ebp+0h]

  v22 = a2;
  last_mp_i = retaddr;
  v4 = this;
  m_list_sorted_mesh_point = (const phys_vec3 **)this->m_list_sorted_mesh_point;
  v6 = &m_list_sorted_mesh_point[this->m_list_mesh_point_count];
  v19 = v4;
  v20 = v6;
  v21 = m_list_sorted_mesh_point;
  if ( m_list_sorted_mesh_point != v6 )
  {
    while ( 1 )
    {
      v7 = (phys_vec3 *)*m_list_sorted_mesh_point;
      v8 = phys_multiply(&v12, xform, *m_list_sorted_mesh_point);
      v16 = v8->x + xform->w.x;
      v17 = v8->y + xform->w.y;
      v18 = v8->z + xform->w.z;
      v13 = translation->x + v16;
      v14 = translation->y + v17;
      v9 = translation->z + v18;
      v10 = ++v21;
      v15 = v9;
      v7->x = v13;
      v7->y = v14;
      v7->z = v15;
      if ( v10 == v20 )
        break;
      m_list_sorted_mesh_point = v21;
    }
    v4 = v19;
  }
  v11 = phys_multiply(&v12, xform, &v4->m_feature_normal);
  v4->m_feature_normal.x = v11->x;
  v4->m_feature_normal.y = v11->y;
  v4->m_feature_normal.z = v11->z;
}

double __cdecl phys_contact_manifold::get_STD_COMP_FEATURE_NORMAL_DISTANCE_EPS(float penetration_t)
{
  if ( penetration_t < 0.0 || penetration_t > 1.0 )
  {
    if ( _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_contact_manifold.h",
           270,
           "penetration_t >= 0.0f && penetration_t <= 1.0f",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
  }
  return (float)(penetration_t * 1.700000047683716 + 1.700000047683716);
}

double __cdecl phys_contact_manifold::get_STD_COMP_FEATURE_NORMAL_SIN_ANGULAR_EPS_SQ(float penetration_t)
{
  if ( penetration_t < 0.0 || penetration_t > 1.0 )
  {
    if ( _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_contact_manifold.h",
           278,
           "penetration_t >= 0.0f && penetration_t <= 1.0f",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
  }
  return (float)(penetration_t * 0.007589269621803396 + 0.000006853876129753189);
}

double __cdecl phys_contact_manifold::get_STD_GET_FEATURE_DISTANCE_EPS(float penetration_t)
{
  if ( penetration_t < 0.0 || penetration_t > 1.0 )
  {
    if ( _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_contact_manifold.h",
           288,
           "penetration_t >= 0.0f && penetration_t <= 1.0f",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
  }
  return (float)(penetration_t * 5.100000143051147 + 1.700000047683716);
}

double __cdecl phys_contact_manifold::get_STD_GET_FEATURE_SIN_ANGULAR_EPS_SQ(float penetration_t)
{
  if ( penetration_t < 0.0 || penetration_t > 1.0 )
  {
    if ( _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_contact_manifold.h",
           296,
           "penetration_t >= 0.0f && penetration_t <= 1.0f",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
  }
  return (float)(penetration_t * 0.4698463100939989 + 0.03015368990600109);
}

void __thiscall phys_contact_manifold_process::comp_contact_mat(
        phys_contact_manifold_process *this,
        const phys_vec3 *contact_normal)
{
  double v3; // st6
  double v4; // st7
  float v5; // [esp-1Ch] [ebp-3Ch]
  float v6; // [esp-1Ch] [ebp-3Ch]
  float v7; // [esp-1Ch] [ebp-3Ch]
  float v8; // [esp-18h] [ebp-38h]
  float v9; // [esp-18h] [ebp-38h]
  float v10; // [esp-18h] [ebp-38h]
  float v11; // [esp-14h] [ebp-34h]
  float v12; // [esp-14h] [ebp-34h]
  float v13; // [esp-14h] [ebp-34h]
  float v14; // [esp-Ch] [ebp-2Ch]
  float v15; // [esp-Ch] [ebp-2Ch]
  float v16; // [esp-8h] [ebp-28h]
  float v17; // [esp-8h] [ebp-28h]
  float v18; // [esp-4h] [ebp-24h]
  float v19; // [esp+10h] [ebp-10h]
  float y; // [esp+10h] [ebp-10h]
  float v21; // [esp+10h] [ebp-10h]
  float v22; // [esp+10h] [ebp-10h]
  float v23; // [esp+10h] [ebp-10h]

  v19 = fabs(contact_normal->x);
  if ( v19 >= 0.800000011920929 )
  {
    y = contact_normal->y;
    v15 = contact_normal->x * y;
    v17 = y * y;
    v18 = y * contact_normal->z;
    v4 = 0.0;
    v5 = 0.0 - v15;
    v3 = 1.0 - v17;
  }
  else
  {
    v14 = contact_normal->x * contact_normal->x;
    v16 = contact_normal->y * contact_normal->x;
    v18 = contact_normal->x * contact_normal->z;
    v5 = 1.0 - v14;
    v3 = 0.0 - v16;
    v4 = 0.0;
  }
  v8 = v3;
  v11 = v4 - v18;
  v21 = v8 * v8 + v5 * v5 + v11 * v11;
  v22 = sqrt(v21);
  if ( v22 <= 0.1000000014901161
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_contact_manifold.h",
         397,
         "nyrow > 0.1f",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  v23 = 1.0 / v22;
  v6 = v23 * v5;
  v9 = v8 * v23;
  v12 = v23 * v11;
  this->contact_mat.y.x = v6;
  this->contact_mat.y.y = v9;
  this->contact_mat.y.z = v12;
  v7 = contact_normal->z * this->contact_mat.y.y - contact_normal->y * this->contact_mat.y.z;
  v10 = contact_normal->x * this->contact_mat.y.z - contact_normal->z * this->contact_mat.y.x;
  v13 = this->contact_mat.y.x * contact_normal->y - contact_normal->x * this->contact_mat.y.y;
  this->contact_mat.x.x = v7;
  this->contact_mat.x.y = v10;
  this->contact_mat.x.z = v13;
  this->contact_mat.z.x = contact_normal->x;
  this->contact_mat.z.y = contact_normal->y;
  this->contact_mat.z.z = contact_normal->z;
}

rigid_body_constraint_contact *__cdecl avl_tree_find<rigid_body_pair_key,rigid_body_constraint_contact,rigid_body_constraint_contact::avl_tree_accessor>(
        rigid_body_constraint_contact *tree_root,
        const rigid_body_pair_key *key)
{
  rigid_body_constraint_contact *result; // eax
  rigid_body *m_b1; // edx
  rigid_body *v4; // ecx
  bool v5; // cf

  result = tree_root;
  if ( tree_root )
  {
    m_b1 = key->m_b1;
    do
    {
      if ( m_b1 == result->m_avl_key.m_b1 && key->m_b2 == result->m_avl_key.m_b2 )
        break;
      v4 = result->m_avl_key.m_b1;
      v5 = m_b1 < v4;
      if ( m_b1 == v4 )
        v5 = key->m_b2 < result->m_avl_key.m_b2;
      result = v5 ? result->m_avl_tree_node.m_left : result->m_avl_tree_node.m_right;
    }
    while ( result );
  }
  return result;
}

contact_point_info *__cdecl contact_point_info::create_cpi(
        int point_pair_count,
        bool no_error,
        phys_transient_allocator *allocator)
{
  unsigned int v3; // edi
  contact_point_info *result; // eax

  if ( point_pair_count <= 0
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\rbc_defs\\rbc_def_contact.h",
         159,
         "point_pair_count > 0",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  v3 = (32 * point_pair_count + 95) & 0xFFFFFFF0;
  result = (contact_point_info *)phys_transient_allocator::mt_allocate(
                                   allocator,
                                   v3 + ((12 * point_pair_count + 15) & 0xFFFFFFF0),
                                   16,
                                   1,
                                   "contact_point_info buffer overflow");
  if ( result )
  {
    result->m_fric_coef = -1.0;
    result->m_bounce_coef = -1.0;
    result->m_max_restitution_vel = -1.0;
    result->m_list_b1_r_loc = &result[1].m_normal;
    result->m_list_b2_r_loc = &result[1].m_normal + point_pair_count;
    result->m_list_pulse_sum_cache_info = (contact_point_info::pulse_sum_cache_info *)((char *)result + v3);
    result->m_point_pair_count = point_pair_count;
    result->m_flags = 0;
    result->m_rb2_entity = (const void *)-1;
  }
  else if ( !no_error )
  {
    if ( (pai_create_cpi.m_hits_total_count < pai_create_cpi.m_max_hits_total || !pai_create_cpi.m_max_hits_total)
      && pai_create_cpi.m_hits_frame_count < pai_create_cpi.m_max_hits_per_frame )
    {
      if ( pai_create_cpi.m_use_warnings_only )
      {
        PHYS_WARNING(
          "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\rbc_defs\\rbc_def_contact.h",
          173,
          "cpi || no_error",
          "contact_point_info buffer overflow");
      }
      else if ( _tlAssert(
                  "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\rbc_defs\\rbc_def_contact.h",
                  173,
                  "cpi || no_error",
                  "contact_point_info buffer overflow") )
      {
        __debugbreak();
      }
    }
    _InterlockedExchangeAdd(&pai_create_cpi.m_hits_total_count, 1u);
    _InterlockedExchangeAdd(&pai_create_cpi.m_hits_frame_count, 1u);
    return 0;
  }
  return result;
}

void __thiscall phys_contact_manifold::set_get_feature_params(
        phys_contact_manifold *this,
        const phys_vec3 *hitp,
        const phys_vec3 *hitn,
        float feature_distance_eps,
        float sin_feautre_angular_eps_sq)
{
  phys_memory_heap *m_allocator; // edi
  const char *v7; // ecx
  unsigned int v8; // eax

  this->m_feature_hitp.x = hitp->x;
  this->m_feature_hitp.y = hitp->y;
  this->m_feature_hitp.z = hitp->z;
  this->m_feature_hitn.x = hitn->x;
  this->m_feature_hitn.y = hitn->y;
  this->m_feature_hitn.z = hitn->z;
  m_allocator = this->m_allocator;
  this->m_feature_distance_eps = feature_distance_eps;
  this->m_sin_feautre_angular_eps_sq = sin_feautre_angular_eps_sq;
  v7 = g_contact_manifold_error_msg;
  v8 = (int)(m_allocator->m_buffer_cur + 15) & 0xFFFFFFF0;
  m_allocator->m_buffer_cur = (char *)v8;
  if ( (char *)v8 >= m_allocator->m_buffer_end
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         114,
         "m_buffer_cur < m_buffer_end",
         v7) )
  {
    __debugbreak();
  }
  this->m_list_mesh_point = (contact_manifold_mesh_point *)m_allocator->m_buffer_cur;
  this->m_list_mesh_point_count = 0;
  this->m_close_mesh_point_count = 0;
  this->m_list_sorted_mesh_point = 0;
  this->m_list_contact_point = 0;
}

void __cdecl contact_point_info::set_closest_cached_psc(
        contact_point_info *cached_cpi,
        const phys_vec3 *normal,
        const phys_vec3 *b1_r_loc,
        const phys_vec3 *b2_r_loc,
        contact_point_info::pulse_sum_cache_info *psc)
{
  contact_point_info *v5; // esi
  const contact_point_info::pulse_sum_cache_info *v6; // eax
  float closest_error; // [esp+4h] [ebp-8h] BYREF
  const contact_point_info::pulse_sum_cache_info *closest_psc; // [esp+8h] [ebp-4h] BYREF

  v5 = cached_cpi;
  closest_error = 10000000.0;
  closest_psc = 0;
  if ( !cached_cpi )
    goto LABEL_5;
  do
  {
    contact_point_info::get_closest_psc(v5, normal, b1_r_loc, b2_r_loc, &closest_error, &closest_psc);
    v5 = v5->m_next_link;
  }
  while ( v5 );
  v6 = closest_psc;
  if ( closest_psc )
  {
    psc->m_ps_cache_list[0].m_pulse_sum = closest_psc->m_ps_cache_list[0].m_pulse_sum;
    psc->m_ps_cache_list[1].m_pulse_sum = v6->m_ps_cache_list[1].m_pulse_sum;
    psc->m_ps_cache_list[2].m_pulse_sum = v6->m_ps_cache_list[2].m_pulse_sum;
  }
  else
  {
LABEL_5:
    if ( psc )
    {
      psc->m_ps_cache_list[0].m_pulse_sum = 0.0;
      psc->m_ps_cache_list[1].m_pulse_sum = 0.0;
      psc->m_ps_cache_list[2].m_pulse_sum = 0.0;
    }
  }
}

void __thiscall contact_point_info::set_closest_cached_psc(contact_point_info *this, contact_point_info *cached_cpi)
{
  contact_point_info::pulse_sum_cache_info *m_list_pulse_sum_cache_info; // esi
  phys_vec3 *m_list_b1_r_loc; // ebx
  contact_point_info::pulse_sum_cache_info *last_psci_i; // [esp+Ch] [ebp-8h]
  const phys_vec3 *b2_r_loc; // [esp+10h] [ebp-4h]

  m_list_pulse_sum_cache_info = this->m_list_pulse_sum_cache_info;
  m_list_b1_r_loc = this->m_list_b1_r_loc;
  b2_r_loc = this->m_list_b2_r_loc;
  for ( last_psci_i = &m_list_pulse_sum_cache_info[this->m_point_pair_count];
        m_list_pulse_sum_cache_info != last_psci_i;
        ++m_list_b1_r_loc )
  {
    contact_point_info::set_closest_cached_psc(
      cached_cpi,
      &this->m_normal,
      m_list_b1_r_loc,
      b2_r_loc++,
      m_list_pulse_sum_cache_info++);
  }
}

// bad sp value at call has been detected, the output may be wrong!
void __userpurge phys_contact_manifold_process::process(
        phys_contact_manifold_process *this@<ecx>,
        float a2@<ebp>,
        int a3@<edi>,
        int a4@<esi>,
        phys_collision_pair *pcp,
        phys_gjk_info *gjk_info)
{
  phys_contact_manifold_process *v6; // esi
  broad_phase_info *m_bpi1; // eax
  contact_manifold_mesh_point **m_cg_to_world_xform; // edi
  const phys_mat44 *m_cg_to_rb_xform; // eax
  const phys_mat44 *m_rb_to_world_xform; // edx
  phys_vec3 *p_m_n; // edi
  double v12; // st6
  float y; // edx
  double v14; // st6
  float x; // edx
  double v16; // st7
  float v17; // edx
  double v18; // st7
  float w; // edx
  double v20; // rt1
  double z; // st6
  double v22; // st5
  double v23; // st7
  phys_vec3 *v25; // eax
  int m_list_mesh_point_count; // eax
  int v27; // ecx
  double STD_COMP_FEATURE_NORMAL_SIN_ANGULAR_EPS_SQ; // st7
  phys_contact_manifold *v29; // ecx
  float v30; // ecx
  int v31; // eax
  int v32; // ecx
  int m_list_contact_point_count; // eax
  int v34; // ecx
  int m_contact_point_count; // eax
  contact_manifold_mesh_point **v36; // eax
  const phys_vec3 *v37; // eax
  contact_manifold_mesh_point *v38; // ecx
  double v39; // st7
  double v40; // st7
  float v41; // eax
  const phys_vec3 *v42; // eax
  float *p_x; // ecx
  rigid_body *v44; // ecx
  float v45; // edx
  int v46; // eax
  double v47; // st6
  rigid_body *v48; // ecx
  contact_manifold_mesh_point *p_m_next_node; // eax
  double v50; // st7
  rigid_body *m_list_isect_point; // edx
  double v52; // st7
  double v53; // st6
  float v54; // edx
  double v55; // st5
  float v56; // edx
  contact_manifold_mesh_point *p_y; // eax
  float *x_low; // eax
  double v59; // st7
  contact_manifold_mesh_point *v60; // eax
  unsigned int p_m_partition_size; // ecx
  float **v62; // eax
  rigid_body *v63; // edx
  float **v64; // edx
  float *v65; // ecx
  float *v66; // edx
  float *v67; // eax
  float v68; // ecx
  double v69; // st7
  phys_vec2 *p_m_contact_p; // edx
  double v71; // st7
  float v72; // eax
  contact_point_info *cpi; // eax
  float v74; // edx
  int v75; // ecx
  const phys_vec2 **v76; // eax
  rigid_body *v77; // eax
  double v78; // st6
  double v79; // st6
  double v80; // st2
  const phys_vec3 *v81; // eax
  double v82; // st7
  float v83; // eax
  double v84; // st5
  double v85; // st4
  double v86; // st2
  const phys_vec3 *v87; // eax
  double v88; // st7
  double v89; // st7
  phys_contact_manifold_process *v90; // ecx
  contact_manifold_mesh_point **v91; // eax
  double v92; // st7
  const phys_vec3 *v93; // eax
  float *v94; // ecx
  double v95; // st7
  contact_point_info *v96; // ecx
  contact_point_info *m_cpi; // ecx
  bool v98; // zf
  contact_manifold_mesh_point **v99; // ecx
  rigid_body *m_rb; // eax
  contact_point_info *v101; // edx
  const rigid_body_pair_key *v102; // eax
  rigid_body_constraint_contact *v103; // eax
  contact_point_info *v104; // ecx
  phys_vec3 *m_list_b1_r_loc; // edx
  phys_vec3 *m_list_b2_r_loc; // edi
  double v107; // st7
  contact_point_info *m_first; // eax
  phys_vec3 v111; // [esp+1Ch] [ebp-14Ch] BYREF
  phys_vec3 v112; // [esp+2Ch] [ebp-13Ch] BYREF
  phys_vec3 v113; // [esp+3Ch] [ebp-12Ch] BYREF
  float v114; // [esp+4Ch] [ebp-11Ch]
  float v115; // [esp+50h] [ebp-118h]
  float v116; // [esp+54h] [ebp-114h]
  phys_vec3 *v117; // [esp+60h] [ebp-108h]
  float v118; // [esp+64h] [ebp-104h]
  float v119; // [esp+68h] [ebp-100h]
  float v120; // [esp+6Ch] [ebp-FCh] BYREF
  float n2; // [esp+70h] [ebp-F8h]
  float n1; // [esp+74h] [ebp-F4h]
  float v123; // [esp+88h] [ebp-E0h]
  float v124; // [esp+8Ch] [ebp-DCh]
  float v125; // [esp+90h] [ebp-D8h]
  float dist_p1_p2_n; // [esp+94h] [ebp-D4h]
  phys_vec3 ip_3d_2; // [esp+98h] [ebp-D0h]
  phys_mat44 *v128; // [esp+A8h] [ebp-C0h]
  float v129; // [esp+ACh] [ebp-BCh]
  float v130; // [esp+B0h] [ebp-B8h]
  const phys_mat44 *cg1_to_rb1_xform; // [esp+B4h] [ebp-B4h]
  float v132; // [esp+B8h] [ebp-B0h]
  phys_contact_manifold_process *v133; // [esp+C8h] [ebp-A0h]
  float v134; // [esp+CCh] [ebp-9Ch] BYREF
  float v135; // [esp+D0h] [ebp-98h]
  contact_manifold_mesh_point **last_ip_i; // [esp+D4h] [ebp-94h]
  rigid_body *v137; // [esp+E8h] [ebp-80h]
  float v138; // [esp+ECh] [ebp-7Ch] BYREF
  float v139; // [esp+F0h] [ebp-78h]
  contact_manifold_mesh_point **last_mp_i; // [esp+F4h] [ebp-74h]
  phys_vec3 p2_displaced; // [esp+F8h] [ebp-70h]
  phys_vec3 cg1_relative_translation_loc; // [esp+108h] [ebp-60h] BYREF
  phys_vec3 ip_3d_; // [esp+118h] [ebp-50h]
  float *v144; // [esp+128h] [ebp-40h]
  float v145; // [esp+12Ch] [ebp-3Ch]
  int v146; // [esp+130h] [ebp-38h]
  contact_manifold_mesh_point *closest_mp; // [esp+134h] [ebp-34h]
  contact_manifold_mesh_point **MAX_MP_I; // [esp+138h] [ebp-30h]
  float feature_distance_eps; // [esp+13Ch] [ebp-2Ch]
  contact_manifold_mesh_point **cur_mp_i; // [esp+140h] [ebp-28h]
  rigid_body *rb1; // [esp+144h] [ebp-24h]
  float d1; // [esp+148h] [ebp-20h]
  float d2; // [esp+14Ch] [ebp-1Ch]
  float v154; // [esp+150h] [ebp-18h]
  float sin_feautre_angular_eps_sq; // [esp+154h] [ebp-14h]
  contact_manifold_mesh_point **smallest_area_mp_i; // [esp+158h] [ebp-10h]
  float dist; // [esp+15Ch] [ebp-Ch] BYREF
  contact_manifold_mesh_point **next_mp_i; // [esp+160h] [ebp-8h]
  contact_manifold_mesh_point **retaddr; // [esp+168h] [ebp+0h]

  dist = a2;
  next_mp_i = retaddr;
  v6 = this;
  v133 = this;
  this->cman1.m_list_mesh_point = 0;
  this->cman1.m_list_sorted_mesh_point = 0;
  this->cman1.m_list_contact_point = 0;
  this->cman2.m_list_mesh_point = 0;
  this->cman2.m_list_sorted_mesh_point = 0;
  this->cman2.m_list_contact_point = 0;
  rb1 = (rigid_body *)&this->cman1;
  this->m_list_isect_point = 0;
  LODWORD(d1) = &this->cman2;
  this->m_allocator.m_buffer_cur = this->m_allocator.m_buffer_start;
  m_bpi1 = pcp->m_bpi1;
  m_cg_to_world_xform = (contact_manifold_mesh_point **)m_bpi1->m_cg_to_world_xform;
  m_cg_to_rb_xform = m_bpi1->m_cg_to_rb_xform;
  this->m_cpi = 0;
  m_rb_to_world_xform = pcp->m_bpi2->m_rb_to_world_xform;
  v128 = (phys_mat44 *)m_cg_to_rb_xform;
  MAX_MP_I = m_cg_to_world_xform;
  LODWORD(ip_3d_.w) = &this->cg1_to_rb2_xform;
  phys_full_inv_multiply_mat(
    (int)&dist,
    &this->cg1_to_rb2_xform,
    m_rb_to_world_xform,
    (const phys_mat44 *)m_cg_to_world_xform);
  p_m_n = &gjk_info->cg1_cinfo_loc.m_n;
  v12 = gjk_info->cg1_cinfo_loc.m_n.x * 0.3400000035762787;
  cg1_relative_translation_loc.y = gjk_info->cg1_cinfo_loc.m_p1.x;
  y = gjk_info->cg1_cinfo_loc.m_p1.y;
  p2_displaced.y = v12;
  cg1_relative_translation_loc.z = y;
  v14 = gjk_info->cg1_cinfo_loc.m_n.y * 0.3400000035762787;
  cg1_relative_translation_loc.w = gjk_info->cg1_cinfo_loc.m_p1.z;
  ip_3d_.x = gjk_info->cg1_cinfo_loc.m_p1.w;
  x = gjk_info->cg1_cinfo_loc.m_p2.x;
  p2_displaced.z = v14;
  v16 = 0.3400000035762787 * gjk_info->cg1_cinfo_loc.m_n.z;
  v138 = x;
  v17 = gjk_info->cg1_cinfo_loc.m_p2.y;
  p2_displaced.w = v16;
  v139 = v17;
  v18 = gjk_info->cg1_cinfo_loc.m_p1.x;
  last_mp_i = (contact_manifold_mesh_point **)LODWORD(gjk_info->cg1_cinfo_loc.m_p2.z);
  w = gjk_info->cg1_cinfo_loc.m_p2.w;
  v117 = &gjk_info->cg1_cinfo_loc.m_n;
  v20 = p2_displaced.y;
  p2_displaced.x = w;
  gjk_info->cg1_cinfo_loc.m_p1.x = v18 + p2_displaced.y;
  z = p2_displaced.z;
  gjk_info->cg1_cinfo_loc.m_p1.y = p2_displaced.z + gjk_info->cg1_cinfo_loc.m_p1.y;
  v22 = p2_displaced.w;
  gjk_info->cg1_cinfo_loc.m_p1.z = gjk_info->cg1_cinfo_loc.m_p1.z + p2_displaced.w;
  gjk_info->cg1_cinfo_loc.m_p2.x = gjk_info->cg1_cinfo_loc.m_p2.x - v20;
  gjk_info->cg1_cinfo_loc.m_p2.y = gjk_info->cg1_cinfo_loc.m_p2.y - z;
  gjk_info->cg1_cinfo_loc.m_p2.z = gjk_info->cg1_cinfo_loc.m_p2.z - v22;
  p2_displaced.y = cg1_relative_translation_loc.y - v138;
  p2_displaced.z = cg1_relative_translation_loc.z - v139;
  p2_displaced.w = cg1_relative_translation_loc.w - *(float *)&last_mp_i;
  v123 = gjk_info->cg1_cinfo_loc.m_n.y * p2_displaced.z
       + gjk_info->cg1_cinfo_loc.m_n.x * p2_displaced.y
       + gjk_info->cg1_cinfo_loc.m_n.z * p2_displaced.w;
  *(float *)&smallest_area_mp_i = v123 / -10.20000076293945;
  v23 = 0.0;
  if ( *(float *)&smallest_area_mp_i >= 0.0 )
  {
    v23 = 1.0;
    if ( *(float *)&smallest_area_mp_i <= 1.0 )
      v23 = *(float *)&smallest_area_mp_i;
  }
  sin_feautre_angular_eps_sq = v23;
  feature_distance_eps = phys_contact_manifold::get_STD_GET_FEATURE_DISTANCE_EPS(sin_feautre_angular_eps_sq);
  *(float *)&cur_mp_i = phys_contact_manifold::get_STD_GET_FEATURE_SIN_ANGULAR_EPS_SQ(sin_feautre_angular_eps_sq);
  phys_contact_manifold::set_get_feature_params(
    (phys_contact_manifold *)rb1,
    &gjk_info->cg1_cinfo_loc.m_p1,
    p_m_n,
    feature_distance_eps,
    *(float *)&cur_mp_i);
  ((void (__thiscall *)(const phys_gjk_geom *, rigid_body *, int, int))pcp->m_bpi1->m_gjk_geom->get_feature)(
    pcp->m_bpi1->m_gjk_geom,
    rb1,
    a3,
    a4);
  p2_displaced.y = -p_m_n->x;
  p2_displaced.z = -gjk_info->cg1_cinfo_loc.m_n.y;
  p2_displaced.w = -gjk_info->cg1_cinfo_loc.m_n.z;
  v134 = gjk_info->cg2_to_cg1_xform.x.z * p2_displaced.w
       + gjk_info->cg2_to_cg1_xform.x.y * p2_displaced.z
       + p2_displaced.y * gjk_info->cg2_to_cg1_xform.x.x;
  v135 = gjk_info->cg2_to_cg1_xform.y.y * p2_displaced.z
       + p2_displaced.y * gjk_info->cg2_to_cg1_xform.y.x
       + gjk_info->cg2_to_cg1_xform.y.z * p2_displaced.w;
  *(float *)&last_ip_i = p2_displaced.w * gjk_info->cg2_to_cg1_xform.z.z
                       + p2_displaced.z * gjk_info->cg2_to_cg1_xform.z.y
                       + p2_displaced.y * gjk_info->cg2_to_cg1_xform.z.x;
  sin_feautre_angular_eps_sq = *(float *)&cur_mp_i;
  v154 = feature_distance_eps;
  d2 = COERCE_FLOAT(&v134);
  v25 = phys_full_inv_multiply(
          (int)&dist,
          (phys_vec3 *)&v120,
          &gjk_info->cg2_to_cg1_xform,
          &gjk_info->cg1_cinfo_loc.m_p2);
  phys_contact_manifold::set_get_feature_params(
    (phys_contact_manifold *)LODWORD(d1),
    v25,
    (const phys_vec3 *)LODWORD(d2),
    v154,
    sin_feautre_angular_eps_sq);
  pcp->m_bpi2->m_gjk_geom->get_feature((phys_gjk_geom *)pcp->m_bpi2->m_gjk_geom, (phys_contact_manifold *)LODWORD(d1));
  smallest_area_mp_i = (contact_manifold_mesh_point **)LODWORD(gjk_info->m_continuous_collision_lambda);
  p2_displaced.y = *(float *)&smallest_area_mp_i * gjk_info->m_cg1_relative_translation_loc.x;
  p2_displaced.z = gjk_info->m_cg1_relative_translation_loc.y * *(float *)&smallest_area_mp_i;
  m_list_mesh_point_count = v6->cman1.m_list_mesh_point_count;
  p2_displaced.w = *(float *)&smallest_area_mp_i * gjk_info->m_cg1_relative_translation_loc.z;
  if ( m_list_mesh_point_count < 2 )
    goto LABEL_17;
  v27 = v6->cman2.m_list_mesh_point_count;
  if ( v27 < 2 || m_list_mesh_point_count == 2 && v27 == 2 )
    goto LABEL_17;
  *(float *)&cur_mp_i = phys_contact_manifold::get_STD_COMP_FEATURE_NORMAL_DISTANCE_EPS(sin_feautre_angular_eps_sq);
  STD_COMP_FEATURE_NORMAL_SIN_ANGULAR_EPS_SQ = phys_contact_manifold::get_STD_COMP_FEATURE_NORMAL_SIN_ANGULAR_EPS_SQ(sin_feautre_angular_eps_sq);
  v29 = (phys_contact_manifold *)rb1;
  feature_distance_eps = STD_COMP_FEATURE_NORMAL_SIN_ANGULAR_EPS_SQ;
  rb1->m_mat.x.x = *(float *)&cur_mp_i;
  v29->m_sin_feautre_angular_eps_sq = feature_distance_eps;
  phys_contact_manifold::comp_feature_normal(v29);
  v30 = d1;
  *(float *)(LODWORD(d1) + 48) = *(float *)&cur_mp_i;
  *(float *)(LODWORD(v30) + 52) = feature_distance_eps;
  phys_contact_manifold::comp_feature_normal((phys_contact_manifold *)LODWORD(v30));
  v31 = v6->cman1.m_list_mesh_point_count;
  if ( v31 < 2 )
    goto LABEL_17;
  v32 = v6->cman2.m_list_mesh_point_count;
  if ( v32 < 2 || v31 == 2 && v32 == 2 )
    goto LABEL_17;
  phys_contact_manifold_process::comp_contact_mat(v6, p_m_n);
  phys_contact_manifold::generate_convex_poly((phys_contact_manifold *)rb1, &v6->contact_mat);
  v134 = -p2_displaced.y;
  v135 = -p2_displaced.z;
  *(float *)&last_ip_i = -p2_displaced.w;
  phys_contact_manifold::xform_and_translate_mesh_points(
    (phys_contact_manifold *)LODWORD(d1),
    (int)&dist,
    &gjk_info->cg2_to_cg1_xform,
    (const phys_vec3 *)&v134);
  phys_contact_manifold::generate_convex_poly((phys_contact_manifold *)LODWORD(d1), &v6->contact_mat);
  m_list_contact_point_count = v6->cman1.m_list_contact_point_count;
  if ( m_list_contact_point_count < 2 )
    goto LABEL_17;
  v34 = v6->cman2.m_list_contact_point_count;
  if ( v34 < 2 || m_list_contact_point_count == 2 && v34 == 2 )
    goto LABEL_17;
  phys_contact_manifold_process::intersect_poly_poly(v6);
  m_contact_point_count = v6->m_contact_point_count;
  if ( !m_contact_point_count )
  {
LABEL_17:
    *(float *)&v36 = COERCE_FLOAT(contact_point_info::create_cpi(1, 0, v6->m_cpi_allocator));
    smallest_area_mp_i = v36;
    v6->m_cpi = (contact_point_info *)v36;
    if ( *(float *)&v36 == 0.0 )
      return;
    v37 = phys_full_multiply((int)&dist, (const phys_vec3 *)&v120, v128, (phys_vec3 *)&cg1_relative_translation_loc.y);
    v38 = smallest_area_mp_i[9];
    v38->m_p.x = v37->x;
    v39 = v37->y;
    LODWORD(d1) = &v138;
    v38->m_p.y = v39;
    v40 = v37->z;
    v41 = ip_3d_.w;
    v38->m_p.z = v40;
    v42 = phys_full_multiply(
            (int)&dist,
            (const phys_vec3 *)&v120,
            (const phys_mat44 *)LODWORD(v41),
            (const phys_vec3 *)LODWORD(d1));
    p_x = &v6->m_cpi->m_list_b2_r_loc->x;
    *p_x = v42->x;
    p_x[1] = v42->y;
    p_x[2] = v42->z;
    v6->m_contact_point_count = 1;
    goto LABEL_69;
  }
  if ( m_contact_point_count <= 0
    && _tlAssert("source/phys_collision.cpp", 83, "m_contact_point_count > 0", &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( !v6->m_list_isect_point && _tlAssert("source/phys_collision.cpp", 84, "m_list_isect_point", &toastPopupTitle) )
    __debugbreak();
  v44 = rb1;
  v45 = d1;
  feature_distance_eps = gjk_info->cg1_cinfo_loc.m_n.y * v44->m_last_position.y
                       + v44->m_last_position.x * gjk_info->cg1_cinfo_loc.m_n.x
                       + gjk_info->cg1_cinfo_loc.m_n.z * v44->m_last_position.z;
  *(float *)&cur_mp_i = gjk_info->cg1_cinfo_loc.m_n.y * *(float *)(LODWORD(v45) + 4)
                      + gjk_info->cg1_cinfo_loc.m_n.x * *(float *)LODWORD(v45)
                      + gjk_info->cg1_cinfo_loc.m_n.z * *(float *)(LODWORD(d1) + 8);
  *(float *)&smallest_area_mp_i = fabs(feature_distance_eps);
  if ( *(float *)&smallest_area_mp_i <= 0.0000001000000011686097 )
  {
    if ( _tlAssert("source/phys_collision.cpp", 88, "fabsf(d1) > 0.0000001f", &toastPopupTitle) )
      __debugbreak();
    v45 = d1;
    v44 = rb1;
  }
  *(float *)&smallest_area_mp_i = fabs(*(float *)&cur_mp_i);
  if ( *(float *)&smallest_area_mp_i <= 0.0000001000000011686097 )
  {
    if ( _tlAssert("source/phys_collision.cpp", 89, "fabsf(d2) > 0.0000001f", &toastPopupTitle) )
      __debugbreak();
    v45 = d1;
    v44 = rb1;
  }
  v46 = v6->m_contact_point_count;
  v119 = v44->m_last_position.y * cg1_relative_translation_loc.z
       + v44->m_last_position.x * cg1_relative_translation_loc.y
       + v44->m_last_position.z * cg1_relative_translation_loc.w;
  v118 = *(float *)(LODWORD(v45) + 4) * v139
       + *(float *)LODWORD(v45) * v138
       + *(float *)(LODWORD(v45) + 8) * *(float *)&last_mp_i;
  if ( v46 > 5 )
  {
    v144 = 0;
    *(float *)&smallest_area_mp_i = 1.0 / feature_distance_eps;
    v138 = v44->m_last_position.x * *(float *)&smallest_area_mp_i;
    v139 = v44->m_last_position.y * *(float *)&smallest_area_mp_i;
    v47 = *(float *)&smallest_area_mp_i * v44->m_last_position.z;
    v48 = (rigid_body *)&v6->m_list_isect_point[v46];
    p_m_next_node = (contact_manifold_mesh_point *)&v48[-1].m_partition_node.m_next_node;
    *(float *)&last_mp_i = v47;
    v137 = v48;
    *(float *)&smallest_area_mp_i = 1.0 / *(float *)&cur_mp_i;
    cg1_relative_translation_loc.y = *(float *)LODWORD(v45) * *(float *)&smallest_area_mp_i;
    cg1_relative_translation_loc.z = *(float *)(LODWORD(v45) + 4) * *(float *)&smallest_area_mp_i;
    v50 = *(float *)&smallest_area_mp_i * *(float *)(LODWORD(v45) + 8);
    closest_mp = (contact_manifold_mesh_point *)&v48[-1].m_partition_node.m_partition_size;
    m_list_isect_point = (rigid_body *)v6->m_list_isect_point;
    cg1_relative_translation_loc.w = v50;
    sin_feautre_angular_eps_sq = *(float *)&m_list_isect_point;
    v129 = cg1_relative_translation_loc.y - v138;
    v130 = cg1_relative_translation_loc.z - v139;
    *(float *)&cg1_to_rb1_xform = cg1_relative_translation_loc.w - *(float *)&last_mp_i;
    v145 = v6->contact_mat.x.z * *(float *)&cg1_to_rb1_xform + v6->contact_mat.x.y * v130 + v129 * v6->contact_mat.x.x;
    *(float *)&v146 = *(float *)&cg1_to_rb1_xform * v6->contact_mat.y.z
                    + v130 * v6->contact_mat.y.y
                    + v129 * v6->contact_mat.y.x;
    *(float *)&smallest_area_mp_i = 10000000.0;
    if ( m_list_isect_point == v48 )
      goto LABEL_89;
    v52 = *(float *)&v146;
    v53 = v145;
    do
    {
      v54 = closest_mp->m_p.x;
      d2 = p_m_next_node->m_p.x;
      v55 = *(float *)(*(unsigned int *)LODWORD(sin_feautre_angular_eps_sq) + 16);
      LODWORD(v154) = *(unsigned int *)LODWORD(sin_feautre_angular_eps_sq) + 16;
      dist_p1_p2_n = v55 - *(float *)(LODWORD(d2) + 16);
      ip_3d_2.x = *(float *)(LODWORD(v154) + 4) - *(float *)(LODWORD(d2) + 20);
      v145 = *(float *)(LODWORD(v54) + 16) - *(float *)(LODWORD(d2) + 16);
      *(float *)&v146 = *(float *)(LODWORD(v54) + 20) - *(float *)(LODWORD(d2) + 20);
      v154 = ip_3d_2.x * v145 - *(float *)&v146 * dist_p1_p2_n;
      v154 = fabs(v154);
      *(float *)LODWORD(v54) = v154;
      v154 = *(float *)(LODWORD(v54) + 20) * v52 + v53 * *(float *)(LODWORD(v54) + 16);
      if ( *(float *)&smallest_area_mp_i > (double)v154 )
      {
        *(float *)&smallest_area_mp_i = v154;
        v144 = (float *)LODWORD(v54);
      }
      p_m_next_node = closest_mp;
      *(float *)&closest_mp = sin_feautre_angular_eps_sq;
      LODWORD(sin_feautre_angular_eps_sq) += 4;
    }
    while ( (rigid_body *)LODWORD(sin_feautre_angular_eps_sq) != v48 );
    if ( !v144 )
    {
LABEL_89:
      if ( _tlAssert("source/phys_collision.cpp", 115, "closest_mp", &toastPopupTitle) )
        __debugbreak();
      v48 = v137;
    }
    v146 = (int)(v6->m_list_isect_point + 5);
    while ( (unsigned int)v48 > v146 )
    {
      v56 = *(float *)&v6->m_list_isect_point;
      p_y = (contact_manifold_mesh_point *)(LODWORD(v56) + 4);
      d2 = v56;
      for ( closest_mp = (contact_manifold_mesh_point *)(LODWORD(v56) + 4);
            p_y != (contact_manifold_mesh_point *)v48;
            closest_mp = p_y )
      {
        x_low = (float *)LODWORD(p_y->m_p.x);
        v56 = d2;
        if ( x_low == v144 )
        {
          v60 = closest_mp;
        }
        else
        {
          v59 = *x_low;
          v60 = closest_mp;
          if ( **(float **)LODWORD(d2) > v59 )
          {
            v56 = *(float *)&closest_mp;
            d2 = *(float *)&closest_mp;
          }
        }
        p_y = (contact_manifold_mesh_point *)&v60->m_p.y;
      }
      p_m_partition_size = (unsigned int)&v48[-1].m_partition_node.m_partition_size;
      v137 = (rigid_body *)p_m_partition_size;
      v62 = (float **)p_m_partition_size;
      if ( (contact_manifold_mesh_point **)LODWORD(v56) > v6->m_list_isect_point )
        v62 = (float **)(LODWORD(v56) - 4);
      if ( LODWORD(v56) < p_m_partition_size )
        v63 = (rigid_body *)(LODWORD(v56) + 4);
      else
        v63 = (rigid_body *)v6->m_list_isect_point;
      sin_feautre_angular_eps_sq = *(float *)&v63;
      v64 = (float **)p_m_partition_size;
      if ( (contact_manifold_mesh_point **)v62 > v6->m_list_isect_point )
        v64 = v62 - 1;
      if ( LODWORD(sin_feautre_angular_eps_sq) < p_m_partition_size )
        v65 = (float *)(LODWORD(sin_feautre_angular_eps_sq) + 4);
      else
        v65 = (float *)v6->m_list_isect_point;
      v66 = *v64;
      v67 = *v62;
      smallest_area_mp_i = (contact_manifold_mesh_point **)v65;
      v68 = *(float *)LODWORD(sin_feautre_angular_eps_sq);
      *(float *)&cg1_to_rb1_xform = *(float *)(*(unsigned int *)LODWORD(sin_feautre_angular_eps_sq) + 16) - v66[4];
      v132 = *(float *)(LODWORD(v68) + 20) - v66[5];
      dist_p1_p2_n = v67[4] - v66[4];
      v69 = v67[5] - v66[5];
      p_m_contact_p = &(*smallest_area_mp_i)->m_contact_p;
      ip_3d_2.x = v69;
      v154 = v132 * dist_p1_p2_n - ip_3d_2.x * *(float *)&cg1_to_rb1_xform;
      v154 = fabs(v154);
      *v67 = v154;
      cg1_relative_translation_loc.w = p_m_contact_p->x - v67[4];
      ip_3d_.x = p_m_contact_p->y - v67[5];
      *(float *)&last_mp_i = *(float *)(LODWORD(v68) + 16) - v67[4];
      v71 = *(float *)(LODWORD(v68) + 20) - v67[5];
      v72 = d2;
      p2_displaced.x = v71;
      v154 = ip_3d_.x * *(float *)&last_mp_i - p2_displaced.x * cg1_relative_translation_loc.w;
      v154 = fabs(v154);
      *(float *)LODWORD(v68) = v154;
      v48 = v137;
      if ( LODWORD(v72) < (unsigned int)v137 )
      {
        memcpy(
          (void *)LODWORD(v72),
          (const void *)(LODWORD(v72) + 4),
          4 * ((((unsigned int)v137 - LODWORD(v72) - 1) >> 2) + 1));
        p_m_n = v117;
        v48 = v137;
        v6 = v133;
      }
    }
    v6->m_contact_point_count = 5;
  }
  cpi = contact_point_info::create_cpi(v6->m_contact_point_count, 0, v6->m_cpi_allocator);
  v6->m_cpi = cpi;
  if ( cpi )
  {
    v74 = *(float *)&cpi->m_list_b1_r_loc;
    v75 = v6->m_contact_point_count;
    smallest_area_mp_i = (contact_manifold_mesh_point **)cpi->m_list_b2_r_loc;
    v76 = (const phys_vec2 **)v6->m_list_isect_point;
    d2 = v74;
    v144 = (float *)v76;
    v133 = (phys_contact_manifold_process *)&v76[v75];
    if ( v76 != (const phys_vec2 **)v133 )
    {
      while ( 1 )
      {
        phys_v2_to_v3_multiply((phys_vec3 *)&cg1_relative_translation_loc.y, &v6->contact_mat, *v76 + 2);
        v77 = rb1;
        v78 = rb1->m_last_position.x;
        LODWORD(d1) = &v113;
        rb1 = (rigid_body *)v128;
        v79 = v78 * cg1_relative_translation_loc.y + cg1_relative_translation_loc.z * v77->m_last_position.y;
        v80 = cg1_relative_translation_loc.w * v77->m_last_position.z;
        *(float *)&cur_mp_i = COERCE_FLOAT(&v111);
        *(float *)&v146 = v79 + v80;
        *(float *)&v146 = (v119 - *(float *)&v146) / feature_distance_eps;
        v138 = *(float *)&v146 * p_m_n->x;
        v139 = p_m_n->y * *(float *)&v146;
        *(float *)&last_mp_i = *(float *)&v146 * p_m_n->z;
        v113.x = cg1_relative_translation_loc.y + v138;
        v113.y = cg1_relative_translation_loc.z + v139;
        v113.z = cg1_relative_translation_loc.w + *(float *)&last_mp_i;
        v81 = phys_multiply(&v111, v128, &v113);
        v129 = v81->x + v128->w.x;
        v130 = v81->y + v128->w.y;
        v82 = v81->z;
        v83 = d2;
        *(float *)&cg1_to_rb1_xform = v82 + v128->w.z;
        *(float *)LODWORD(d2) = v129;
        *(float *)(LODWORD(v83) + 4) = v130;
        *(float *)(LODWORD(v83) + 8) = *(float *)&cg1_to_rb1_xform;
        v124 = cg1_relative_translation_loc.y + p2_displaced.y;
        v125 = cg1_relative_translation_loc.z + p2_displaced.z;
        dist_p1_p2_n = cg1_relative_translation_loc.w + p2_displaced.w;
        v84 = v125 * *(float *)(LODWORD(d1) + 4);
        v85 = *(float *)LODWORD(d1);
        feature_distance_eps = COERCE_FLOAT(&v120);
        MAX_MP_I = (contact_manifold_mesh_point **)LODWORD(ip_3d_.w);
        v86 = dist_p1_p2_n * *(float *)(LODWORD(d1) + 8);
        *(float *)&closest_mp = COERCE_FLOAT(&v112);
        *(float *)&v146 = v85 * v124 + v84 + v86;
        *(float *)&v146 = (v118 - *(float *)&v146) / *(float *)&cur_mp_i;
        v114 = *(float *)&v146 * p_m_n->x;
        v115 = p_m_n->y * *(float *)&v146;
        v116 = *(float *)&v146 * p_m_n->z;
        v120 = v124 + v114;
        n2 = v125 + v115;
        n1 = dist_p1_p2_n + v116;
        v87 = phys_multiply(&v112, (const phys_mat44 *)LODWORD(ip_3d_.w), (const phys_vec3 *)&v120);
        v88 = v87->x + *(float *)(LODWORD(ip_3d_.w) + 48);
        LODWORD(d2) += 16;
        v134 = v88;
        v135 = v87->y + *(float *)(LODWORD(ip_3d_.w) + 52);
        v89 = v87->z + *(float *)(LODWORD(ip_3d_.w) + 56);
        v91 = smallest_area_mp_i + 4;
        *(float *)&last_ip_i = v89;
        v90 = (phys_contact_manifold_process *)++v144;
        smallest_area_mp_i = v91;
        *((float *)v91 - 4) = v134;
        *((float *)v91 - 3) = v135;
        *(v91 - 2) = (contact_manifold_mesh_point *)last_ip_i;
        if ( v90 == v133 )
          break;
        v76 = (const phys_vec2 **)v144;
      }
    }
LABEL_69:
    if ( !v6->m_cpi && _tlAssert("source/phys_collision.cpp", 155, "m_cpi", &toastPopupTitle) )
      __debugbreak();
    v120 = -p_m_n->x;
    sin_feautre_angular_eps_sq = COERCE_FLOAT(&v120);
    v92 = p_m_n->y;
    v154 = *(float *)&MAX_MP_I;
    n2 = -v92;
    d2 = COERCE_FLOAT(&v112);
    n1 = -p_m_n->z;
    v93 = phys_multiply(&v112, (const phys_mat44 *)MAX_MP_I, (const phys_vec3 *)&v120);
    v94 = &v6->m_cpi->m_normal.x;
    *v94 = v93->x;
    LODWORD(d1) = v94;
    v94[1] = v93->y;
    v94[2] = v93->z;
    PHYS_ASSERT_UNIT((const phys_vec3 *)LODWORD(d1));
    *(float *)&MAX_MP_I = p_m_n->y * gjk_info->m_cg1_relative_translation_loc.y
                        + p_m_n->x * gjk_info->m_cg1_relative_translation_loc.x
                        + p_m_n->z * gjk_info->m_cg1_relative_translation_loc.z;
    if ( *(float *)&MAX_MP_I >= -0.17 )
    {
      v6->m_cpi->m_translation_lambda = 1.0;
    }
    else
    {
      *(float *)&MAX_MP_I = -v123 / *(float *)&MAX_MP_I;
      v95 = 0.0;
      if ( *(float *)&MAX_MP_I < 0.0 || (v95 = *(float *)&MAX_MP_I, *(float *)&MAX_MP_I <= 1.0) )
      {
        m_cpi = v6->m_cpi;
        ip_3d_.w = v95;
        m_cpi->m_translation_lambda = ip_3d_.w;
      }
      else
      {
        v96 = v6->m_cpi;
        ip_3d_.w = 1.0;
        v96->m_translation_lambda = 1.0;
      }
    }
    set_cpi_params(v6->m_cpi, pcp);
    v98 = v6->m_list_cpi.m_last_next_ptr == 0;
    MAX_MP_I = (contact_manifold_mesh_point **)v6->m_cpi;
    if ( v98
      && _tlAssert(
           "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
           230,
           "m_last_next_ptr",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    v99 = MAX_MP_I;
    MAX_MP_I[12] = 0;
    ++v6->m_list_cpi.m_alloc_count;
    *v6->m_list_cpi.m_last_next_ptr = (contact_point_info *)v99;
    v6->m_list_cpi.m_last_next_ptr = (contact_point_info **)(v99 + 12);
    m_rb = pcp->m_bpi1->m_rb;
    v101 = v6->m_cpi;
    sin_feautre_angular_eps_sq = *(float *)&pcp->m_bpi2->m_rb;
    v154 = *(float *)&m_rb;
    MAX_MP_I = (contact_manifold_mesh_point **)m_rb;
    v101->m_pcp = pcp;
    v102 = rigid_body_pair_key::rigid_body_pair_key(
             (rigid_body_pair_key *)&cg1_relative_translation_loc.w,
             (rigid_body *const)LODWORD(v154),
             (rigid_body *const)LODWORD(sin_feautre_angular_eps_sq));
    v103 = avl_tree_find<rigid_body_pair_key,rigid_body_constraint_contact,rigid_body_constraint_contact::avl_tree_accessor>(
             v6->m_rbc_contact_search_tree_root,
             v102);
    v6->m_cpi->m_rbc_contact = v103;
    if ( v103 )
    {
      if ( (contact_manifold_mesh_point **)v103->b1 != MAX_MP_I )
      {
        v104 = v6->m_cpi;
        m_list_b1_r_loc = v104->m_list_b1_r_loc;
        m_list_b2_r_loc = v104->m_list_b2_r_loc;
        v134 = -v104->m_normal.x;
        v104->m_list_b1_r_loc = m_list_b2_r_loc;
        v107 = v104->m_normal.y;
        v104->m_list_b2_r_loc = m_list_b1_r_loc;
        v135 = -v107;
        *(float *)&last_ip_i = -v104->m_normal.z;
        v104->m_normal.x = v134;
        v104->m_normal.y = v135;
        v104->m_normal.z = *(float *)&last_ip_i;
      }
      m_first = v103->m_list_contact_point_info_buffer_2.m_first;
    }
    else
    {
      m_first = 0;
    }
    contact_point_info::set_closest_cached_psc(v6->m_cpi, m_first);
  }
}

