#include "phys_gjk_collision_detection.h"

phys_vec3 *__cdecl phys_Unitize(phys_vec3 *result, const phys_vec3 *a)
{
  float v3; // [esp+4h] [ebp-14h]
  float v4; // [esp+8h] [ebp-10h]
  float na; // [esp+14h] [ebp-4h]

  na = Abs(&a->x);
  if ( na <= 0.0
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
         80,
         "na > 0.0f",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  v3 = a->y * (float)(1.0 / na);
  v4 = a->z * (float)(1.0 / na);
  result->x = a->x * (float)(1.0 / na);
  result->y = v3;
  result->z = v4;
  return result;
}

void __cdecl gjk_collision_epilog(bool is_server_thread)
{
  if ( is_server_thread )
    BG_EvalVehicleName();
}

phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *__cdecl get_gjk_cache_info(
        phys_heap_gjk_cache_system_avl_tree *gjk_cache,
        const gjk_base_t *cg1,
        const gjk_base_t *cg2)
{
  unsigned int v3; // eax
  unsigned int geom_id; // [esp-8h] [ebp-84h]

  if ( !gjk_cache || !g_use_gjk_cache )
    return 0;
  geom_id = gjk_base_t::get_geom_id(cg2);
  v3 = gjk_base_t::get_geom_id(cg1);
  return phys_heap_gjk_cache_system_avl_tree::get_gjk_cache_info(gjk_cache, v3, geom_id, 1);
}

phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *__thiscall phys_heap_gjk_cache_system_avl_tree::get_gjk_cache_info(
        phys_heap_gjk_cache_system_avl_tree *this,
        unsigned int id1,
        unsigned int id2,
        bool __formal)
{
  unsigned int m_id2; // eax
  phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *v5; // ecx
  unsigned int temp; // [esp+1B0h] [ebp-14h]
  phys_gjk_geom_id_pair_key key; // [esp+1B4h] [ebp-10h] BYREF
  bool swapped; // [esp+1BFh] [ebp-5h]
  phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *gjk_ci; // [esp+1C0h] [ebp-4h]
  int savedregs; // [esp+1C4h] [ebp+0h] BYREF

  if ( id1 == id2
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_gjk_cache_system.h",
         309,
         "id1 != id2",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( id1 <= id2 )
  {
    swapped = 0;
  }
  else
  {
    swapped = 1;
    temp = id1;
    id1 = id2;
    id2 = temp;
  }
  if ( id1 >= id2
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_gjk_cache_system.h",
         24,
         "id1 < id2",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  key.m_id1 = id1;
  key.m_id2 = id2;
  gjk_ci = phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::find(
             &this->m_search_tree,
             &key);
  if ( gjk_ci )
  {
    phys_gjk_cache_info::update_swapped(gjk_ci, (int)&savedregs, swapped);
  }
  else
  {
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
            328,
            "m_list_phys_gjk_cache_info_internal.get_count() < m_max_num_gjk_ci",
            "max num gjk_ci reached.");
        }
        else if ( this->m_list_phys_gjk_cache_info_internal.m_count >= this->m_max_num_gjk_ci
               && _tlAssert(
                    "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_gjk_cache_system.h",
                    328,
                    "m_list_phys_gjk_cache_info_internal.get_count() < m_max_num_gjk_ci",
                    "max num gjk_ci reached.") )
        {
          __debugbreak();
        }
      }
      _InterlockedExchangeAdd(&pai_gjk_cache_system_max_num_gjk_ci.m_hits_total_count, 1u);
      _InterlockedExchangeAdd(&pai_gjk_cache_system_max_num_gjk_ci.m_hits_frame_count, 1u);
    }
    if ( this->m_list_phys_gjk_cache_info_internal.m_count < this->m_max_num_gjk_ci )
    {
      gjk_ci = phys_simple_allocator<phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal>::allocate(&this->m_list_phys_gjk_cache_info_internal);
      if ( !gjk_ci )
      {
        if ( (pai_gjk_cache_system_create_gjk_ci.m_hits_total_count < pai_gjk_cache_system_create_gjk_ci.m_max_hits_total
           || !pai_gjk_cache_system_create_gjk_ci.m_max_hits_total)
          && pai_gjk_cache_system_create_gjk_ci.m_hits_frame_count < pai_gjk_cache_system_create_gjk_ci.m_max_hits_per_frame )
        {
          if ( pai_gjk_cache_system_create_gjk_ci.m_use_warnings_only )
          {
            PHYS_WARNING(
              "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_gjk_cache_system.h",
              332,
              "gjk_ci",
              "gjk_ci memory allocation failed.");
          }
          else if ( _tlAssert(
                      "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_gjk_cache_system.h",
                      332,
                      "gjk_ci",
                      "gjk_ci memory allocation failed.") )
          {
            __debugbreak();
          }
        }
        _InterlockedExchangeAdd(&pai_gjk_cache_system_create_gjk_ci.m_hits_total_count, 1u);
        _InterlockedExchangeAdd(&pai_gjk_cache_system_create_gjk_ci.m_hits_frame_count, 1u);
      }
      if ( gjk_ci )
      {
        m_id2 = key.m_id2;
        v5 = gjk_ci;
        gjk_ci->m_key.m_id1 = key.m_id1;
        v5->m_key.m_id2 = m_id2;
        gjk_ci->m_flags = 0;
        if ( swapped )
          gjk_ci->m_flags |= 2u;
        else
          gjk_ci->m_flags &= ~2u;
        phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::add(
          &this->m_search_tree,
          &key,
          gjk_ci);
        gjk_ci->m_next_gjk_ci = this->m_list_head;
        this->m_list_head = gjk_ci;
      }
    }
  }
  return gjk_ci;
}

// local variable allocation has failed, the output may be wrong!
void __userpurge phys_gjk_cache_info::update_swapped(phys_gjk_cache_info *this@<ecx>, int a2@<ebp>, bool swapped)
{
  _BYTE v3[60]; // [esp-Ch] [ebp-6Ch] OVERLAPPED BYREF
  int v4; // [esp+40h] [ebp-20h]
  int v5; // [esp+44h] [ebp-1Ch]
  float v6; // [esp+48h] [ebp-18h]
  unsigned int m_flags; // [esp+4Ch] [ebp-14h]
  phys_gjk_cache_info *v8; // [esp+50h] [ebp-10h]
  int v9; // [esp+54h] [ebp-Ch]
  void *v10; // [esp+58h] [ebp-8h]
  void *retaddr; // [esp+60h] [ebp+0h]

  v9 = a2;
  v10 = retaddr;
  v8 = this;
  if ( swapped != ((this->m_flags & 2) != 0) )
  {
    if ( swapped )
      v8->m_flags |= 2u;
    else
      v8->m_flags &= ~2u;
    m_flags = v8->m_flags;
    if ( (v8->m_flags & 4) != 0 )
    {
      LODWORD(v6) = LODWORD(v8->m_support_dir.x) ^ _mask__NegFloat_;
      v5 = LODWORD(v8->m_support_dir.y) ^ _mask__NegFloat_;
      v4 = LODWORD(v8->m_support_dir.z) ^ _mask__NegFloat_;
      *(float *)&v3[48] = v6;
      *(unsigned int *)&v3[52] = v5;
      *(unsigned int *)&v3[56] = v4;
      v8->m_support_dir.x = v6;
      v8->m_support_dir.y = *(float *)&v3[52];
      v8->m_support_dir.z = *(float *)&v3[56];
    }
    if ( (v8->m_flags & 8) != 0 )
    {
      memcpy(v3, &v8->m_support_a, 0x30u);
      cached_simplex_info::operator=(&v8->m_support_a, &v8->m_support_b);
      cached_simplex_info::operator=(&v8->m_support_b, (const cached_simplex_info *)v3);
    }
  }
}

cached_simplex_info *__thiscall cached_simplex_info::operator=(
        cached_simplex_info *this,
        const cached_simplex_info *__that)
{
  phys_vec3 *v2; // eax
  float *p_x; // [esp+8h] [ebp-8h]
  unsigned int _S1; // [esp+Ch] [ebp-4h]

  for ( _S1 = 0; _S1 < 3; ++_S1 )
  {
    p_x = &this->m_indices[_S1].x;
    *p_x = __that->m_indices[_S1].x;
    v2 = &__that->m_indices[_S1];
    p_x[1] = v2->y;
    p_x[2] = v2->z;
  }
  return this;
}

void __thiscall gjk_query_output::reset_cache(gjk_query_output *this)
{
  bpei_database_t::update_database(&this->m_bpei_database);
  this->m_ent_count = 0;
  this->m_geom_count = 0;
  this->m_list_geom_info.m_first = 0;
  this->m_list_geom_info.m_last_next_ptr = &this->m_list_geom_info.m_first;
  this->m_list_geom_info.m_alloc_count = 0;
  this->m_query_visitor_count = 0;
  this->m_gent_query_visitor_count = 0;
  this->m_cent_query_visitor_count = 0;
  this->m_dent_query_visitor_count = 0;
  phys_transient_allocator::reset(&this->m_allocator);
  this->m_total_list_geom_info = 0;
  this->m_cached_query_info.m_query_contents = 0;
  this->m_cached_query_info.m_query_flags = 0;
}

void __thiscall bpei_database_t::update_database(bpei_database_t *this)
{
  broad_phase_environment_info *bpei; // [esp+154h] [ebp-8h]
  broad_phase_environment_info **prev_next; // [esp+158h] [ebp-4h]

  prev_next = &this->m_bpei_list;
  for ( bpei = this->m_bpei_list; bpei; bpei = *prev_next )
  {
    if ( bpei->m_data )
    {
      prev_next = &bpei->m_next_bpei;
      bpei->m_data = 0;
    }
    else
    {
      *prev_next = bpei->m_next_bpei;
      phys_inplace_avl_tree<bpei_database_id,broad_phase_environment_info,broad_phase_environment_info::avl_tree_accessor>::remove(
        &this->m_bpei_map,
        &bpei->m_database_id);
      phys_simple_allocator<broad_phase_environment_info>::free(&this->m_bpei_allocator, bpei);
    }
  }
}

void __thiscall gjk_query_output::query_prolog(gjk_query_output *this, const gjk_query_input *input)
{
  int savedregs; // [esp+160h] [ebp+0h] BYREF

  this->m_list_geom_info.m_first = 0;
  this->m_list_geom_info.m_last_next_ptr = &this->m_list_geom_info.m_first;
  this->m_list_geom_info.m_alloc_count = 0;
  gjk_query_input::visit_skip_list(input, ++this->m_query_visitor_count);
  gjk_query_output::calc_query_aabb(this, (int)&savedregs, input);
  phys_transient_allocator::reset_to_state(&this->m_allocator, &this->m_allocator_state);
}

void __thiscall gjk_query_input::visit_skip_list(gjk_query_input *this, int query_visitor_count)
{
  gjk_geom_info_t *gi_i; // [esp+14h] [ebp-4h]

  for ( gi_i = this->m_geom_skip_list.m_first; gi_i; gi_i = gi_i->m_next_link )
  {
    if ( gi_i->m_query_visitor_count == query_visitor_count
      && !Assert_MyHandler(
            "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.h",
            182,
            0,
            "%s",
            "gi->m_query_visitor_count != query_visitor_count") )
    {
      __debugbreak();
    }
    gi_i->m_query_visitor_count = query_visitor_count;
  }
}

void __userpurge gjk_query_output::calc_query_aabb(
        gjk_query_output *this@<ecx>,
        int a2@<ebp>,
        const gjk_query_input *input)
{
  const phys_vec3 *v3; // eax
  phys_vec3 *v4; // ecx
  float v5; // [esp-13Ch] [ebp-148h]
  float v6; // [esp-138h] [ebp-144h]
  phys_vec3 v7; // [esp-120h] [ebp-12Ch] BYREF
  phys_vec3 *v8; // [esp-104h] [ebp-110h]
  phys_vec3 v9; // [esp-100h] [ebp-10Ch] BYREF
  float v10; // [esp-E8h] [ebp-F4h]
  float v11; // [esp-E4h] [ebp-F0h]
  float v12; // [esp-E0h] [ebp-ECh]
  phys_vec3 *v13; // [esp-DCh] [ebp-E8h]
  phys_vec3 *v14; // [esp-D8h] [ebp-E4h]
  phys_vec3 *v15; // [esp-D4h] [ebp-E0h]
  float v16; // [esp-D0h] [ebp-DCh]
  float v17; // [esp-CCh] [ebp-D8h]
  float v18; // [esp-C8h] [ebp-D4h]
  float v19; // [esp-C0h] [ebp-CCh]
  float v20; // [esp-BCh] [ebp-C8h]
  float v21; // [esp-B8h] [ebp-C4h]
  const phys_vec3 *v22; // [esp-B4h] [ebp-C0h]
  phys_vec3 v23; // [esp-B0h] [ebp-BCh] BYREF
  phys_vec3 *p_m_ac_eps_vec; // [esp-94h] [ebp-A0h]
  phys_vec3 v25; // [esp-90h] [ebp-9Ch] BYREF
  float v26; // [esp-78h] [ebp-84h]
  float v27; // [esp-74h] [ebp-80h]
  float v28; // [esp-70h] [ebp-7Ch]
  phys_vec3 *v29; // [esp-6Ch] [ebp-78h]
  phys_vec3 *p_m_cg_translation; // [esp-68h] [ebp-74h]
  phys_vec3 *p_m_query_aabb_max; // [esp-64h] [ebp-70h]
  float v32; // [esp-60h] [ebp-6Ch]
  float v33; // [esp-5Ch] [ebp-68h]
  float v34; // [esp-58h] [ebp-64h]
  float v35; // [esp-48h] [ebp-54h]
  float v36; // [esp-44h] [ebp-50h]
  float v37; // [esp-40h] [ebp-4Ch]
  phys_vec3 *v38; // [esp-3Ch] [ebp-48h]
  phys_vec3 *p_m_cg_aabb_max; // [esp-38h] [ebp-44h]
  phys_vec3 *p_m_query_aabb_min; // [esp-34h] [ebp-40h]
  float v41; // [esp-30h] [ebp-3Ch]
  float v42; // [esp-2Ch] [ebp-38h]
  float v43; // [esp-28h] [ebp-34h]
  float v44; // [esp-14h] [ebp-20h]
  float v45; // [esp-10h] [ebp-1Ch]
  float v46; // [esp-Ch] [ebp-18h]
  phys_vec3 *p_m_cg_position; // [esp-8h] [ebp-14h]
  gjk_query_output *v48; // [esp-4h] [ebp-10h]
  int v49; // [esp+0h] [ebp-Ch]
  void *v50; // [esp+4h] [ebp-8h]
  void *retaddr; // [esp+Ch] [ebp+0h]

  v49 = a2;
  v50 = retaddr;
  v48 = this;
  p_m_cg_position = &input->m_cg_position;
  v46 = input->m_cg_position.x + input->m_cg_aabb_min.x;
  v45 = input->m_cg_position.y + input->m_cg_aabb_min.y;
  v44 = input->m_cg_position.z + input->m_cg_aabb_min.z;
  v41 = v46;
  v42 = v45;
  v43 = v44;
  p_m_query_aabb_min = &this->m_query_aabb_min;
  this->m_query_aabb_min.x = v46;
  p_m_query_aabb_min->y = v42;
  p_m_query_aabb_min->z = v43;
  p_m_cg_aabb_max = &input->m_cg_aabb_max;
  v38 = &input->m_cg_position;
  v37 = input->m_cg_position.x + input->m_cg_aabb_max.x;
  v36 = input->m_cg_position.y + input->m_cg_aabb_max.y;
  v35 = input->m_cg_position.z + input->m_cg_aabb_max.z;
  v32 = v37;
  v33 = v36;
  v34 = v35;
  p_m_query_aabb_max = &v48->m_query_aabb_max;
  v48->m_query_aabb_max.x = v37;
  p_m_query_aabb_max->y = v33;
  p_m_query_aabb_max->z = v34;
  p_m_cg_translation = &input->m_cg_translation;
  v29 = &v48->m_query_aabb_min;
  v28 = v48->m_query_aabb_min.x + input->m_cg_translation.x;
  v27 = v48->m_query_aabb_min.y + input->m_cg_translation.y;
  v26 = v48->m_query_aabb_min.z + input->m_cg_translation.z;
  v25.x = v28;
  v25.y = v27;
  v25.z = v26;
  p_m_ac_eps_vec = &input->m_ac_eps_vec;
  v22 = phys_min(&v23, &v48->m_query_aabb_min, &v25);
  v21 = v22->x - p_m_ac_eps_vec->x;
  v20 = v22->y - p_m_ac_eps_vec->y;
  v19 = v22->z - p_m_ac_eps_vec->z;
  v16 = v21;
  v17 = v20;
  v18 = v19;
  v15 = &v48->m_query_aabb_min;
  v48->m_query_aabb_min.x = v21;
  v15->y = v17;
  v15->z = v18;
  v14 = &input->m_cg_translation;
  v13 = &v48->m_query_aabb_max;
  v12 = v48->m_query_aabb_max.x + input->m_cg_translation.x;
  v11 = v48->m_query_aabb_max.y + input->m_cg_translation.y;
  v10 = v48->m_query_aabb_max.z + input->m_cg_translation.z;
  v9.x = v12;
  v9.y = v11;
  v9.z = v10;
  v8 = &input->m_ac_eps_vec;
  v3 = phys_max(&v7, &v48->m_query_aabb_max, &v9);
  v5 = v3->y + v8->y;
  v6 = v3->z + v8->z;
  v4 = &v48->m_query_aabb_max;
  v48->m_query_aabb_max.x = v3->x + v8->x;
  v4->y = v5;
  v4->z = v6;
}

void __thiscall gjk_query_output::query_epilog(gjk_query_output *this)
{
  phys_transient_allocator::allocator_state v1; // [esp+18h] [ebp-10h]

  *(_QWORD *)&v1.m_first_block = *(_QWORD *)&this->m_allocator.m_first_block;
  *(_QWORD *)&v1.m_end = *(_QWORD *)&this->m_allocator.m_end;
  this->m_allocator_state = v1;
}

void *__thiscall gjk_query_output::allocate(gjk_query_output *this, int size, int alignment, bool no_error)
{
  return phys_transient_allocator::allocate(
           &this->m_allocator,
           size,
           alignment,
           no_error,
           "gjk player trace out of memory.");
}

void __thiscall gjk_query_output::get_local_query_aabb(
        gjk_query_output *this,
        float *local_query_aabb_min,
        float *local_query_aabb_max)
{
  Phys_NitrousVecToVec3(&this->m_local_query_aabb_min, local_query_aabb_min);
  Phys_NitrousVecToVec3(&this->m_local_query_aabb_max, local_query_aabb_max);
}

bool __thiscall gjk_query_output::query_create_prolog(gjk_query_output *this, const void *geom)
{
  if ( !geom
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
          458,
          0,
          "%s",
          "geom") )
  {
    __debugbreak();
  }
  if ( !this->m_local_ent_info
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
          459,
          0,
          "%s",
          "m_local_ent_info") )
  {
    __debugbreak();
  }
  if ( !gjk_entity_info_t::get_ent(this->m_local_ent_info)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
          460,
          0,
          "%s",
          "m_local_ent_info->get_ent()") )
  {
    __debugbreak();
  }
  this->m_local_database_id.m_id1 = (unsigned int)gjk_entity_info_t::get_ent(this->m_local_ent_info);
  this->m_local_database_id.m_id2 = (unsigned int)geom;
  this->m_local_bpei = bpei_database_t::get_bpei(&this->m_bpei_database, this->m_local_database_id);
  return this->m_local_bpei->m_data == 0;
}

broad_phase_environment_info *__thiscall bpei_database_t::get_bpei(bpei_database_t *this, bpei_database_id database_id)
{
  broad_phase_environment_info *bpei; // [esp+160h] [ebp-4h]

  bpei = phys_inplace_avl_tree<bpei_database_id,broad_phase_environment_info,broad_phase_environment_info::avl_tree_accessor>::find(
           &this->m_bpei_map,
           &database_id);
  if ( !bpei )
  {
    bpei = bpei_database_t::create_bpei(this, database_id);
    bpei->m_next_bpei = this->m_bpei_list;
    this->m_bpei_list = bpei;
  }
  if ( !bpei
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_broad_phase.h",
         257,
         "bpei",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  return bpei;
}

const void *__thiscall gjk_entity_info_t::get_ent(gjk_entity_info_t *this)
{
  if ( (this->m_ent_type == ET_NONE || !this->m_ent)
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.h",
          91,
          0,
          "%s",
          "m_ent_type != ET_NONE && m_ent != NULL") )
  {
    __debugbreak();
  }
  return this->m_ent;
}

void __thiscall gjk_query_output::query_create_epilog(gjk_query_output *this, gjk_base_t *gjk_geom)
{
  bool v2; // [esp+0h] [ebp-10h]

  if ( !gjk_geom
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
          472,
          0,
          "%s",
          "gjk_geom") )
  {
    __debugbreak();
  }
  if ( !this->m_local_bpei
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
          473,
          0,
          "%s",
          "m_local_bpei") )
  {
    __debugbreak();
  }
  if ( this->m_local_bpei->m_data
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
          474,
          0,
          "%s",
          "m_local_bpei->m_data == NULL") )
  {
    __debugbreak();
  }
  v2 = this->m_local_bpei->m_database_id.m_id1 == this->m_local_database_id.m_id1
    && this->m_local_bpei->m_database_id.m_id2 == this->m_local_database_id.m_id2;
  if ( !v2
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
          475,
          0,
          "%s",
          "m_local_bpei->m_database_id == m_local_database_id") )
  {
    __debugbreak();
  }
  gjk_base_t::set_geom_id_new(gjk_geom, this->m_local_bpei->m_gjk_geom_id);
  this->m_local_bpei->m_data = gjk_query_output::create_geom_info(this, gjk_geom, this->m_local_ent_info, 0, 0);
}

bool __userpurge gjk_query_output::query_create_prolog_1@<al>(
        gjk_query_output *this@<ecx>,
        int a2@<ebp>,
        float *local_aabb_min,
        float *local_aabb_max,
        const void *geom)
{
  _BYTE v6[12]; // [esp-Ch] [ebp-3Ch] BYREF
  phys_vec3 pv_local_aabb_max; // [esp+0h] [ebp-30h] BYREF
  gjk_query_output *v8; // [esp+20h] [ebp-10h]
  int v9; // [esp+24h] [ebp-Ch]
  void *v10; // [esp+28h] [ebp-8h]
  void *retaddr; // [esp+30h] [ebp+0h]

  v9 = a2;
  v10 = retaddr;
  v8 = this;
  if ( !this->m_local_ent_info
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
          484,
          0,
          "%s",
          "m_local_ent_info") )
  {
    __debugbreak();
  }
  if ( !gjk_entity_info_t::get_ent(v8->m_local_ent_info)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
          485,
          0,
          "%s",
          "m_local_ent_info->get_ent()") )
  {
    __debugbreak();
  }
  Phys_Vec3ToNitrousVec((phys_vec3 *)&pv_local_aabb_max.y, local_aabb_min);
  Phys_Vec3ToNitrousVec((const phys_vec3 *)v6, local_aabb_max);
  return phys_are_aabb_overlapping(
           &v8->m_local_query_aabb_min,
           &v8->m_local_query_aabb_max,
           (phys_vec3 *)&pv_local_aabb_max.y,
           (const phys_vec3 *)v6)
      && v8->query_create_prolog(v8, geom);
}

bool __cdecl phys_are_aabb_overlapping(
        const phys_vec3 *aabb1_min,
        const phys_vec3 *aabb1_max,
        const phys_vec3 *aabb2_min,
        const phys_vec3 *aabb2_max)
{
  return aabb2_max->x >= aabb1_min->x
      && aabb2_max->y >= aabb1_min->y
      && aabb2_max->z >= aabb1_min->z
      && aabb1_max->x >= aabb2_min->x
      && aabb1_max->y >= aabb2_min->y
      && aabb1_max->z >= aabb2_min->z;
}

broad_phase_environment_info *__thiscall gjk_query_output::get_ent_info(gjk_query_output *this, unsigned int ent_id)
{
  return bpei_database_t::get_bpei(&this->m_bpei_database, (bpei_database_id)ent_id);
}

void __thiscall gjk_query_output::set_local_query_info(
        gjk_query_output *this,
        const gjk_query_input *input,
        gjk_entity_info_t *ent_info)
{
  phys_calc_local_aabb(
    &this->m_query_aabb_min,
    &this->m_query_aabb_max,
    &ent_info->m_mat,
    &this->m_local_query_aabb_min,
    &this->m_local_query_aabb_max);
  this->m_local_query_input = input;
  this->m_local_ent_info = ent_info;
  if ( !gjk_entity_info_t::get_ent(ent_info)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
          509,
          0,
          "%s",
          "ent_info->get_ent()") )
  {
    __debugbreak();
  }
}

gjk_geom_info_t *__thiscall gjk_query_output::create_geom_info(
        gjk_query_output *this,
        gjk_base_t *cg,
        gjk_entity_info_t *ent_info,
        float *aabb_min,
        float *aabb_max)
{
  gjk_geom_info_t *v6; // [esp+0h] [ebp-3Ch]
  gjk_geom_info_t *v8; // [esp+34h] [ebp-8h]

  ++this->m_geom_count;
  v8 = (gjk_geom_info_t *)phys_transient_allocator::allocate(
                            &this->m_allocator,
                            64,
                            16,
                            0,
                            "phys_transient_allocator out of memory.");
  if ( v8 )
    v6 = v8;
  else
    v6 = 0;
  v6->m_cg = cg;
  v6->m_ent_info = ent_info;
  v6->m_query_visitor_count = -1;
  if ( aabb_min )
  {
    if ( !aabb_max
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
            521,
            0,
            "%s",
            "aabb_max") )
    {
      __debugbreak();
    }
    Phys_Vec3ToNitrousVec(aabb_min, &v6->m_aabb_min);
    Phys_Vec3ToNitrousVec(aabb_max, &v6->m_aabb_max);
  }
  else
  {
    gjk_geom_info_t::calc_aabb(v6);
  }
  v6->m_total_next_link = this->m_total_list_geom_info;
  this->m_total_list_geom_info = v6;
  return v6;
}

void __thiscall gjk_geom_info_t::calc_aabb(gjk_geom_info_t *this)
{
  gjk_entity_info_t *xform; // eax
  phys_vec3 *p_m_aabb_max; // [esp-4h] [ebp-10h]

  p_m_aabb_max = &this->m_aabb_max;
  xform = gjk_geom_info_t::get_xform(this);
  this->m_cg->calc_aabb(this->m_cg, (const phys_mat44 *)xform, (phys_vec3 *)this, p_m_aabb_max);
}

gjk_entity_info_t *__thiscall gjk_query_output::create_entity_info(gjk_query_output *this)
{
  unsigned int *v2; // [esp+0h] [ebp-34h]
  unsigned int *v3; // [esp+2Ch] [ebp-8h]

  ++this->m_ent_count;
  v3 = phys_transient_allocator::allocate(&this->m_allocator, 80, 16, 0, "phys_transient_allocator out of memory.");
  if ( v3 )
  {
    v3[16] = 4;
    v3[17] = 0;
    v2 = v3;
  }
  else
  {
    v2 = 0;
  }
  v2[18] = -1;
  return (gjk_entity_info_t *)v2;
}

void __thiscall gjk_query_output::add(
        gjk_query_output *this,
        const gjk_query_input *input,
        const CollisionPartition *partition,
        const CollisionAabbTree *tree)
{
  gjk_geom_info_t *geom_info; // eax
  float aabb_max[3]; // [esp+20h] [ebp-28h] BYREF
  gjk_base_t *cg; // [esp+2Ch] [ebp-1Ch]
  float aabb_min[3]; // [esp+30h] [ebp-18h] BYREF
  bpei_database_id database_id; // [esp+3Ch] [ebp-Ch]
  broad_phase_environment_info *bpei; // [esp+44h] [ebp-4h]

  database_id.m_id1 = (unsigned int)partition;
  database_id.m_id2 = 0;
  bpei = bpei_database_t::get_bpei(&this->m_bpei_database, (bpei_database_id)(unsigned int)partition);
  if ( !bpei->m_data )
  {
    cg = gjk_partition_t::create(tree, this);
    gjk_base_t::set_geom_id_new(cg, bpei->m_gjk_geom_id);
    aabb_min[0] = tree->origin[0] - tree->halfSize[0];
    aabb_min[1] = tree->origin[1] - tree->halfSize[1];
    aabb_min[2] = tree->origin[2] - tree->halfSize[2];
    aabb_max[0] = tree->origin[0] + tree->halfSize[0];
    aabb_max[1] = tree->origin[1] + tree->halfSize[1];
    aabb_max[2] = tree->origin[2] + tree->halfSize[2];
    geom_info = gjk_query_output::create_geom_info(this, cg, 0, aabb_min, aabb_max);
    bpei->m_data = geom_info;
  }
}

void __thiscall gjk_query_output::add(
        gjk_query_output *this,
        const gjk_query_input *input,
        const cbrush_t *brush,
        const float *query_mins,
        const float *query_maxs)
{
  gjk_brush_t *cg; // [esp+30h] [ebp-10h]
  broad_phase_environment_info *bpei; // [esp+3Ch] [ebp-4h]
  int savedregs; // [esp+40h] [ebp+0h] BYREF

  bpei = bpei_database_t::get_bpei(&this->m_bpei_database, (bpei_database_id)(unsigned int)brush);
  if ( !bpei->m_data )
  {
    cg = gjk_brush_t::create(
           COERCE_FLOAT(&savedregs),
           brush,
           (unsigned __int8)((int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20] & brush->axial_sflags[0][0]) >> 20),
           this);
    gjk_base_t::set_geom_id_new(cg, bpei->m_gjk_geom_id);
    bpei->m_data = gjk_query_output::create_geom_info(this, cg, 0, brush->mins, brush->maxs);
  }
}

// local variable allocation has failed, the output may be wrong!
void __userpurge gjk_query_output::add(
        gjk_query_output *this@<ecx>,
        broad_phase_environment_info *a2@<ebp>,
        const gjk_query_input *input,
        gentity_s *gent)
{
  gjk_entity_info_t *m_data; // [esp-Ch] [ebp-5Ch]
  float v5[3]; // [esp-8h] [ebp-58h] OVERLAPPED BYREF
  phys_vec3 *p_w; // [esp+1Ch] [ebp-34h]
  const phys_vec3 *v7; // [esp+20h] [ebp-30h]
  phys_vec3 v8; // [esp+24h] [ebp-2Ch] BYREF
  gjk_entity_info_t *entity_info; // [esp+34h] [ebp-1Ch]
  broad_phase_environment_info *ent_info; // [esp+38h] [ebp-18h]
  gentity_s *v11; // [esp+3Ch] [ebp-14h]
  gjk_query_output *v12; // [esp+40h] [ebp-10h]
  broad_phase_environment_info *bpei; // [esp+44h] [ebp-Ch]
  unsigned int gent_id; // [esp+48h] [ebp-8h]
  unsigned int retaddr; // [esp+50h] [ebp+0h]

  bpei = a2;
  gent_id = retaddr;
  v12 = this;
  v11 = gent;
  ent_info = gjk_query_output::get_ent_info(this, (unsigned int)gent);
  if ( !ent_info->m_data )
  {
    entity_info = gjk_query_output::create_entity_info(v12);
    entity_info->m_ent_type = ET_GENT;
    entity_info->m_ent = gent;
    if ( gent->s.eType == 17 || gent->s.eType == 1 )
    {
      phys_mat44::operator=(&entity_info->m_mat, &PHYS_IDENTITY_MATRIX_42);
      v7 = Phys_Vec3ToNitrousVec(&v8, gent->r.currentOrigin);
      p_w = &entity_info->m_mat.w;
      entity_info->m_mat.w.x = v7->x;
      p_w->y = v7->y;
      p_w->z = v7->z;
    }
    else
    {
      AnglesToAxis(gent->r.currentAngles, (float (*)[3])v5);
      Phys_AxisToNitrousMat((float (*)[3])v5, &entity_info->m_mat);
      Phys_Vec3ToNitrousVec(gent->r.currentOrigin, &entity_info->m_mat.w);
    }
    ent_info->m_data = entity_info;
  }
  if ( !ent_info->m_data
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
          590,
          0,
          "%s",
          "bpei->m_data") )
  {
    __debugbreak();
  }
  m_data = (gjk_entity_info_t *)ent_info->m_data;
  if ( *((unsigned int *)ent_info->m_data + 16)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
          592,
          0,
          "%s",
          "ent_info->is_gent()") )
  {
    __debugbreak();
  }
  if ( m_data->m_query_visitor_count != v12->m_gent_query_visitor_count )
  {
    m_data->m_query_visitor_count = v12->m_gent_query_visitor_count;
    gjk_query_output::set_local_query_info(v12, input, m_data);
    create_gjk_geom(gent, v12, 1, input->m_contents, 0);
  }
}

// local variable allocation has failed, the output may be wrong!
void __userpurge gjk_query_output::add(
        gjk_query_output *this@<ecx>,
        broad_phase_environment_info *a2@<ebp>,
        const gjk_query_input *input,
        centity_s *cent)
{
  gjk_entity_info_t *m_data; // [esp-8h] [ebp-5Ch]
  float v5[3]; // [esp-4h] [ebp-58h] OVERLAPPED BYREF
  phys_vec3 *p_w; // [esp+20h] [ebp-34h]
  const phys_vec3 *v7; // [esp+24h] [ebp-30h]
  phys_vec3 v8; // [esp+28h] [ebp-2Ch] BYREF
  gjk_entity_info_t *entity_info; // [esp+38h] [ebp-1Ch]
  broad_phase_environment_info *ent_info; // [esp+3Ch] [ebp-18h]
  centity_s *v11; // [esp+40h] [ebp-14h]
  gjk_query_output *v12; // [esp+44h] [ebp-10h]
  broad_phase_environment_info *bpei; // [esp+48h] [ebp-Ch]
  unsigned int cent_id; // [esp+4Ch] [ebp-8h]
  unsigned int retaddr; // [esp+54h] [ebp+0h]

  bpei = a2;
  cent_id = retaddr;
  v12 = this;
  v11 = cent;
  ent_info = gjk_query_output::get_ent_info(this, (unsigned int)cent);
  if ( !ent_info->m_data )
  {
    entity_info = gjk_query_output::create_entity_info(v12);
    entity_info->m_ent_type = ET_CENT;
    entity_info->m_ent = cent;
    if ( cent->nextState.eType == 17 || cent->nextState.eType == 1 )
    {
      phys_mat44::operator=(&entity_info->m_mat, &PHYS_IDENTITY_MATRIX_42);
      v7 = Phys_Vec3ToNitrousVec(&v8, cent->pose.origin);
      p_w = &entity_info->m_mat.w;
      entity_info->m_mat.w.x = v7->x;
      p_w->y = v7->y;
      p_w->z = v7->z;
    }
    else
    {
      AnglesToAxis(cent->pose.angles, (float (*)[3])v5);
      Phys_AxisToNitrousMat((float (*)[3])v5, &entity_info->m_mat);
      Phys_Vec3ToNitrousVec(cent->pose.origin, &entity_info->m_mat.w);
    }
    ent_info->m_data = entity_info;
  }
  if ( !ent_info->m_data
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
          619,
          0,
          "%s",
          "bpei->m_data") )
  {
    __debugbreak();
  }
  m_data = (gjk_entity_info_t *)ent_info->m_data;
  if ( *((unsigned int *)ent_info->m_data + 16) != 1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
          621,
          0,
          "%s",
          "ent_info->is_cent()") )
  {
    __debugbreak();
  }
  if ( m_data->m_query_visitor_count != v12->m_cent_query_visitor_count )
  {
    m_data->m_query_visitor_count = v12->m_cent_query_visitor_count;
    gjk_query_output::set_local_query_info(v12, input, m_data);
    create_gjk_geom(cgCollWorldLocalClientNum, cent, v12, 1, input->m_contents, 0, 0);
  }
}

void __thiscall gjk_query_output::add(gjk_query_output *this, const gjk_query_input *input, const Glass *glass)
{
  float axis[9]; // [esp+4h] [ebp-34h] BYREF
  phys_mat44 *outMat; // [esp+28h] [ebp-10h]
  broad_phase_environment_info *bpei; // [esp+2Ch] [ebp-Ch]
  gjk_entity_info_t *ent_info; // [esp+30h] [ebp-8h]
  unsigned int glass_id; // [esp+34h] [ebp-4h]

  glass_id = (unsigned int)glass;
  bpei = gjk_query_output::get_ent_info(this, (unsigned int)glass);
  if ( !bpei->m_data )
  {
    outMat = (phys_mat44 *)gjk_query_output::create_entity_info(this);
    LODWORD(outMat[1].x.x) = 3;
    LODWORD(outMat[1].x.y) = glass;
    AnglesToAxis(glass->angles, (float (*)[3])axis);
    Phys_AxisToNitrousMat((float (*)[3])axis, outMat);
    Phys_Vec3ToNitrousVec(glass->origin, &outMat->w);
    bpei->m_data = outMat;
  }
  if ( !bpei->m_data
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
          642,
          0,
          "%s",
          "bpei->m_data") )
  {
    __debugbreak();
  }
  ent_info = (gjk_entity_info_t *)bpei->m_data;
  if ( ent_info->m_ent_type != ET_GLASS
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
          644,
          0,
          "%s",
          "ent_info->is_glass()") )
  {
    __debugbreak();
  }
  if ( ent_info->m_query_visitor_count != this->m_gent_query_visitor_count )
  {
    ent_info->m_query_visitor_count = this->m_gent_query_visitor_count;
    gjk_query_output::set_local_query_info(this, input, ent_info);
    create_gjk_geom(glass, this, input->m_contents);
  }
}

void __thiscall gjk_query_output::add(gjk_query_output *this, const gjk_query_input *input, const DynEntityDef *dent)
{
  float axis[9]; // [esp+4h] [ebp-44h] BYREF
  DynEntityDrawType v5; // [esp+28h] [ebp-20h]
  DynEntityPose *dynEntPose; // [esp+2Ch] [ebp-1Ch]
  DynEntityDrawType drawType; // [esp+30h] [ebp-18h]
  phys_mat44 *outMat; // [esp+34h] [ebp-14h]
  unsigned __int16 dynEntId; // [esp+38h] [ebp-10h]
  unsigned int dent_id; // [esp+3Ch] [ebp-Ch]
  broad_phase_environment_info *bpei; // [esp+40h] [ebp-8h]
  gjk_entity_info_t *ent_info; // [esp+44h] [ebp-4h]

  dent_id = (unsigned int)dent;
  bpei = gjk_query_output::get_ent_info(this, (unsigned int)dent);
  if ( !bpei->m_data )
  {
    outMat = (phys_mat44 *)gjk_query_output::create_entity_info(this);
    LODWORD(outMat[1].x.x) = 2;
    LODWORD(outMat[1].x.y) = dent;
    v5 = dent->xModel == 0;
    drawType = v5;
    dynEntId = DynEnt_GetId(dent, v5);
    dynEntPose = DynEnt_GetClientPose(dynEntId, v5);
    UnitQuatToAxis(dynEntPose->pose.quat, (float (*)[3])axis);
    Phys_AxisToNitrousMat((float (*)[3])axis, outMat);
    Phys_Vec3ToNitrousVec(dynEntPose->pose.origin, &outMat->w);
    bpei->m_data = outMat;
  }
  if ( !bpei->m_data
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
          667,
          0,
          "%s",
          "bpei->m_data") )
  {
    __debugbreak();
  }
  ent_info = (gjk_entity_info_t *)bpei->m_data;
  if ( ent_info->m_ent_type != ET_DENT
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
          669,
          0,
          "%s",
          "ent_info->is_dent()") )
  {
    __debugbreak();
  }
  if ( ent_info->m_query_visitor_count != this->m_dent_query_visitor_count )
  {
    ent_info->m_query_visitor_count = this->m_dent_query_visitor_count;
    gjk_query_output::set_local_query_info(this, input, ent_info);
    create_gjk_geom(dent, this, input->m_contents);
  }
}

void __cdecl gjk_query_prims(const gjk_query_input *input, gjk_query_output *output)
{
  const cbrush_t *brush; // [esp+14h] [ebp-50h]
  const CollisionAabbTree *tree; // [esp+38h] [ebp-2Ch]
  int i; // [esp+3Ch] [ebp-28h]
  const col_prim_t *prim; // [esp+40h] [ebp-24h]
  col_prim_t *prims; // [esp+44h] [ebp-20h]
  float query_maxs[3]; // [esp+48h] [ebp-1Ch] BYREF
  int nprims; // [esp+54h] [ebp-10h]
  float query_mins[3]; // [esp+58h] [ebp-Ch] BYREF

  if ( input->m_proximity_data->overflow
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
          680,
          0,
          "%s",
          "input.m_proximity_data->overflow == false") )
  {
    __debugbreak();
  }
  prims = input->m_proximity_data->prims;
  nprims = input->m_proximity_data->nprims;
  Phys_NitrousVecToVec3(&output->m_query_aabb_min, query_mins);
  Phys_NitrousVecToVec3(&output->m_query_aabb_max, query_maxs);
  prim = prims;
  for ( i = 0; i < nprims; ++i )
  {
    if ( prim->type )
    {
      if ( prim->type != 1
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
              710,
              0,
              "%s",
              "prim->type == COL_BRUSH") )
      {
        __debugbreak();
      }
      brush = prim->brush;
      if ( (brush->contents & input->m_contents) != 0
        && query_maxs[0] >= brush->mins[0]
        && query_maxs[1] >= brush->mins[1]
        && query_maxs[2] >= brush->mins[2]
        && brush->maxs[0] >= query_mins[0]
        && brush->maxs[1] >= query_mins[1]
        && brush->maxs[2] >= query_mins[2] )
      {
        gjk_query_output::add(output, input, brush, query_mins, query_maxs);
      }
    }
    else
    {
      tree = prim->tree;
      if ( (cm.materials[tree->materialIndex].contentFlags & input->m_contents) != 0
        && query_maxs[0] >= (float)(tree->origin[0] - tree->halfSize[0])
        && query_maxs[1] >= (float)(tree->origin[1] - tree->halfSize[1])
        && query_maxs[2] >= (float)(tree->origin[2] - tree->halfSize[2])
        && (float)(tree->origin[0] + tree->halfSize[0]) >= query_mins[0]
        && (float)(tree->origin[1] + tree->halfSize[1]) >= query_mins[1]
        && (float)(tree->origin[2] + tree->halfSize[2]) >= query_mins[2] )
      {
        gjk_query_output::add(output, input, &cm.partitions[tree->u.firstChildIndex], tree);
      }
    }
    ++prim;
  }
}

void __cdecl gjk_query_terrain(const gjk_query_input *input, gjk_query_output *output)
{
  int j; // [esp+20h] [ebp-10A4h]
  int i; // [esp+28h] [ebp-109Ch]
  colgeom_visitor_t v4; // [esp+2Ch] [ebp-1098h] BYREF
  int v5; // [esp+9Ch] [ebp-1028h]
  unsigned int v6[512]; // [esp+A0h] [ebp-1024h]
  int v7; // [esp+8A0h] [ebp-824h]
  unsigned int v8[513]; // [esp+8A4h] [ebp-820h]
  float mx[3]; // [esp+10ACh] [ebp-18h] BYREF
  float outVector[3]; // [esp+10B8h] [ebp-Ch] BYREF

  Phys_NitrousVecToVec3(&output->m_query_aabb_min, outVector);
  Phys_NitrousVecToVec3(&output->m_query_aabb_max, mx);
  colgeom_visitor_t::colgeom_visitor_t(&v4);
  v4.__vftable = (colgeom_visitor_t_vtbl *)&static_colgeom_visitor_t::`vftable';
  v5 = 0;
  v7 = 0;
  colgeom_visitor_t::intersect_box(&v4, outVector, mx, input->m_contents);
  for ( i = 0; i < v7; ++i )
    gjk_query_output::add(output, input, (const cbrush_t *)v8[i], outVector, mx);
  for ( j = 0; j < v5; ++j )
    gjk_query_output::add(output, input, &cm.partitions[*(unsigned int *)(v6[j] + 28)], (const CollisionAabbTree *)v6[j]);
}

void __cdecl gjk_query_gents(const gjk_query_input *input, gjk_query_output *output)
{
  DObj *ServerDObj; // [esp+0h] [ebp-1034h]
  gentity_s *gent; // [esp+8h] [ebp-102Ch]
  int i; // [esp+10h] [ebp-1024h]
  int entityList[1025]; // [esp+14h] [ebp-1020h] BYREF
  float maxs[3]; // [esp+1018h] [ebp-1Ch] BYREF
  int v7; // [esp+1024h] [ebp-10h]
  float outVector[3]; // [esp+1028h] [ebp-Ch] BYREF
  int savedregs; // [esp+1034h] [ebp+0h] BYREF

  ++output->m_gent_query_visitor_count;
  Phys_NitrousVecToVec3(&output->m_query_aabb_min, outVector);
  Phys_NitrousVecToVec3(&output->m_query_aabb_max, maxs);
  v7 = CM_AreaEntities(outVector, maxs, entityList, 1024, input->m_contents);
  for ( i = 0; i < v7; ++i )
  {
    gent = &g_entities[entityList[i]];
    if ( !query_should_pass_gentity(input, gent) )
    {
      if ( BG_ClipMoveToDobj(&gent->s) )
        ServerDObj = Com_GetServerDObj(gent->s.number);
      else
        ServerDObj = 0;
      if ( !ServerDObj || DObjHasCollmap(ServerDObj) && DObjHasContents(ServerDObj, input->m_contents) )
        gjk_query_output::add(output, (broad_phase_environment_info *)&savedregs, input, gent);
    }
  }
}

char __cdecl query_should_pass_gentity(const gjk_query_input *input, const gentity_s *gent)
{
  if ( input->m_pass_entity_num == 1023 )
    return ((unsigned int)&cls.rankedServers[711].game[35] & gent->s.lerp.eFlags2) != 0;
  if ( gent->s.number == input->m_pass_entity_num )
    return 1;
  if ( !EntHandle::isDefined(&gent->r.ownerNum) )
    return ((unsigned int)&cls.rankedServers[711].game[35] & gent->s.lerp.eFlags2) != 0;
  if ( EntHandle::entnum(&gent->r.ownerNum) == input->m_pass_entity_num )
    return 1;
  return EntHandle::entnum(&gent->r.ownerNum) == input->m_pass_owner_num
      || ((unsigned int)&cls.rankedServers[711].game[35] & gent->s.lerp.eFlags2) != 0;
}

void __cdecl gjk_query_cents(const gjk_query_input *input, gjk_query_output *output)
{
  DObj *ClientDObj; // [esp+0h] [ebp-1058h]
  int entityIndex; // [esp+24h] [ebp-1034h]
  int localClientNum; // [esp+28h] [ebp-1030h]
  centity_s *cent; // [esp+2Ch] [ebp-102Ch]
  int i; // [esp+34h] [ebp-1024h]
  int entityList[1025]; // [esp+38h] [ebp-1020h] BYREF
  float maxs[3]; // [esp+103Ch] [ebp-1Ch] BYREF
  int v10; // [esp+1048h] [ebp-10h]
  float outVector[3]; // [esp+104Ch] [ebp-Ch] BYREF
  int savedregs; // [esp+1058h] [ebp+0h] BYREF

  ++output->m_cent_query_visitor_count;
  Phys_NitrousVecToVec3(&output->m_query_aabb_min, outVector);
  Phys_NitrousVecToVec3(&output->m_query_aabb_max, maxs);
  v10 = CG_AreaEntities(outVector, maxs, entityList, 1024, input->m_contents);
  for ( i = 0; i < v10; ++i )
  {
    entityIndex = entityList[i];
    if ( ((unsigned int)&cls.rankedServers[711].game[35] & CG_GetEntity(0, entityIndex)->nextState.lerp.eFlags2) == 0
      && entityIndex != input->m_pass_entity_num
      && entityIndex < 1024 )
    {
      localClientNum = cgCollWorldLocalClientNum;
      cent = CG_GetEntity(cgCollWorldLocalClientNum, entityIndex);
      if ( (cent->nextState.lerp.eFlags & 1) == 0 )
      {
        if ( BG_ClipMoveToDobj(&cent->nextState) )
          ClientDObj = Com_GetClientDObj(entityIndex, localClientNum);
        else
          ClientDObj = 0;
        if ( ClientDObj )
        {
          if ( DObjHasCollmap(ClientDObj) && DObjHasContents(ClientDObj, input->m_contents) )
            goto LABEL_22;
        }
        else if ( cent->nextState.solid
               && ((char *)cent->nextState.solid != &cls.rankedServers[711].game[34]
                || (cent->nextState.lerp.eFlags & 1) == 0)
               && (input->m_contents & CG_GetEntityBModelContents(cent)) != 0 )
        {
LABEL_22:
          gjk_query_output::add(output, (broad_phase_environment_info *)&savedregs, input, cent);
          continue;
        }
      }
    }
  }
}

void __cdecl gjk_query_glass(const gjk_query_input *input, gjk_query_output *output)
{
  unsigned int i; // [esp+0h] [ebp-1024h]
  float maxs[3]; // [esp+8h] [ebp-101Ch] BYREF
  unsigned int v4; // [esp+14h] [ebp-1010h]
  float outVector[3]; // [esp+18h] [ebp-100Ch] BYREF
  const Glass *glasses[1024]; // [esp+24h] [ebp-1000h] BYREF

  if ( (input->m_contents & 0x10) != 0 )
  {
    Phys_NitrousVecToVec3(&output->m_query_aabb_min, outVector);
    Phys_NitrousVecToVec3(&output->m_query_aabb_max, maxs);
    if ( input->m_is_server_thread )
      v4 = GlassSv_AreaGlasses(outVector, maxs, glasses, 0x400u);
    else
      v4 = GlassCl_AreaGlasses(outVector, maxs, glasses, 0x400u);
    for ( i = 0; i < v4; ++i )
      gjk_query_output::add(output, input, glasses[i]);
  }
}

void __cdecl gjk_query_dents(const gjk_query_input *input, gjk_query_output *output)
{
  const DynEntityDef *dynEntDef; // [esp+4h] [ebp-2044h]
  unsigned __int16 Id; // [esp+8h] [ebp-2040h]
  int i; // [esp+Ch] [ebp-203Ch]
  DynEntityDrawType drawType; // [esp+10h] [ebp-2038h]
  DynEntityAreaParms areaParms; // [esp+14h] [ebp-2034h] BYREF
  _WORD v7[4098]; // [esp+28h] [ebp-2020h] BYREF
  float v8[3]; // [esp+2030h] [ebp-18h] BYREF
  float outVector[3]; // [esp+203Ch] [ebp-Ch] BYREF

  ++output->m_dent_query_visitor_count;
  Phys_NitrousVecToVec3(&output->m_query_aabb_min, outVector);
  Phys_NitrousVecToVec3(&output->m_query_aabb_max, v8);
  areaParms.mins = outVector;
  areaParms.maxs = v8;
  areaParms.contentMask = input->m_contents;
  areaParms.list = v7;
  areaParms.maxCount = 4096;
  for ( drawType = DYNENT_DRAW_MODEL; drawType < DYNENT_DRAW_COUNT; ++drawType )
  {
    areaParms.count = 0;
    DynEntCl_AreaEntities_r((DynEntityCollType)drawType, 1u, &areaParms);
    for ( i = 0; i < areaParms.count; ++i )
    {
      dynEntDef = DynEnt_GetEntityDef(v7[i], drawType);
      if ( (dynEntDef->flags & 2) != 0 && DynEnt_GetEntityProps(dynEntDef->type)->usePhysics )
      {
        Id = DynEnt_GetId(dynEntDef, drawType);
        if ( !DynEnt_GetClientEntity(Id, drawType)->physObjId )
          gjk_query_output::add(output, input, dynEntDef);
      }
    }
  }
}

void __cdecl gjk_query(const gjk_query_input *input, gjk_query_output *output)
{
  float mins[3]; // [esp+0h] [ebp-24h] BYREF
  float expand_vec[3]; // [esp+Ch] [ebp-18h] BYREF
  float maxs[3]; // [esp+18h] [ebp-Ch] BYREF

  ++output->m_total_query_count;
  if ( (input->m_gjk_query_flags & 1) != 0 )
  {
    if ( input->m_proximity_data )
    {
      Phys_NitrousVecToVec3(&output->m_query_aabb_min, mins);
      Phys_NitrousVecToVec3(&output->m_query_aabb_max, maxs);
      expand_vec[0] = FLOAT_70_0;
      expand_vec[1] = FLOAT_70_0;
      expand_vec[2] = FLOAT_20_0;
      input->m_proximity_data->update(input->m_proximity_data, mins, maxs, input->m_proximity_mask, expand_vec);
      if ( (input->m_contents & input->m_proximity_mask) != input->m_contents
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
              994,
              0,
              "%s",
              "(input.m_proximity_mask & input.m_contents) == input.m_contents") )
      {
        __debugbreak();
      }
      if ( input->m_proximity_data->overflow )
        gjk_query_terrain(input, output);
      else
        gjk_query_prims(input, output);
    }
    else
    {
      gjk_query_terrain(input, output);
    }
  }
  if ( (input->m_gjk_query_flags & 2) != 0 )
  {
    if ( input->m_is_server_thread )
      gjk_query_gents(input, output);
    else
      gjk_query_cents(input, output);
    gjk_query_glass(input, output);
  }
  if ( (input->m_gjk_query_flags & 4) != 0 )
    gjk_query_dents(input, output);
}

void __userpurge gjk_query_output::cached_query_resize(
        gjk_query_output *this@<ecx>,
        int a2@<ebp>,
        bool is_server_thread,
        colgeom_visitor_inlined_t<200> *proximity_data,
        int proximity_mask)
{
  phys_vec3 *v5; // eax
  phys_vec3 *v6; // [esp-20h] [ebp-E0h]
  phys_vec3 *v7; // [esp-1Ch] [ebp-DCh]
  phys_vec3 *p_m_query_aabb_max; // [esp-18h] [ebp-D8h]
  phys_vec3 *p_m_query_aabb_min; // [esp-14h] [ebp-D4h]
  cached_query_info_t *p_m_cached_query_info; // [esp-10h] [ebp-D0h]
  float x; // [esp-Ch] [ebp-CCh]
  float y; // [esp-8h] [ebp-C8h]
  float z; // [esp-4h] [ebp-C4h]
  float saved_query_aabb_max_4; // [esp+4h] [ebp-BCh]
  float saved_query_aabb_max_8; // [esp+8h] [ebp-B8h]
  float saved_query_aabb_max_12; // [esp+Ch] [ebp-B4h]
  int v17; // [esp+24h] [ebp-9Ch] BYREF
  gjk_query_input resize_input; // [esp+30h] [ebp-90h] BYREF
  gjk_query_output *v19; // [esp+B0h] [ebp-10h]
  int v20; // [esp+B4h] [ebp-Ch]
  void *v21; // [esp+B8h] [ebp-8h]
  void *retaddr; // [esp+C0h] [ebp+0h]

  v20 = a2;
  v21 = retaddr;
  v19 = this;
  if ( cached_query_info_t::is_empty(&this->m_cached_query_info)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
          1023,
          0,
          "%s",
          "!m_cached_query_info.is_empty()") )
  {
    __debugbreak();
  }
  if ( !cached_query_info_t::aabb_is_valid(&v19->m_cached_query_info)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
          1024,
          0,
          "%s",
          "m_cached_query_info.aabb_is_valid()") )
  {
    __debugbreak();
  }
  resize_input.m_proximity_data = 0;
  resize_input.m_proximity_mask = (int)&resize_input.m_proximity_data;
  resize_input.m_gjk_query_flags = 0;
  LODWORD(resize_input.m_ac_eps_vec.y) = v19->m_cached_query_info.m_query_contents;
  LODWORD(resize_input.m_ac_eps_vec.z) = 1023;
  LODWORD(resize_input.m_ac_eps_vec.w) = 1023;
  LOBYTE(resize_input.m_contents) = is_server_thread;
  resize_input.m_pass_entity_num = (int)proximity_data;
  resize_input.m_pass_owner_num = proximity_mask;
  *(unsigned int *)&resize_input.m_is_server_thread = v19->m_cached_query_info.m_query_flags;
  resize_input.m_proximity_data = 0;
  resize_input.m_proximity_mask = (int)&resize_input.m_proximity_data;
  resize_input.m_gjk_query_flags = 0;
  saved_query_aabb_max_4 = v19->m_query_aabb_min.x;
  saved_query_aabb_max_8 = v19->m_query_aabb_min.y;
  saved_query_aabb_max_12 = v19->m_query_aabb_min.z;
  x = v19->m_query_aabb_max.x;
  y = v19->m_query_aabb_max.y;
  z = v19->m_query_aabb_max.z;
  p_m_cached_query_info = &v19->m_cached_query_info;
  p_m_query_aabb_min = &v19->m_query_aabb_min;
  v19->m_query_aabb_min.x = v19->m_cached_query_info.m_query_aabb_min.x;
  p_m_query_aabb_min->y = p_m_cached_query_info->m_query_aabb_min.y;
  p_m_query_aabb_min->z = p_m_cached_query_info->m_query_aabb_min.z;
  p_m_query_aabb_max = &v19->m_cached_query_info.m_query_aabb_max;
  v7 = &v19->m_query_aabb_max;
  v19->m_query_aabb_max.x = v19->m_cached_query_info.m_query_aabb_max.x;
  v7->y = p_m_query_aabb_max->y;
  v7->z = p_m_query_aabb_max->z;
  gjk_query((const gjk_query_input *)&v17, v19);
  v6 = &v19->m_query_aabb_min;
  v19->m_query_aabb_min.x = saved_query_aabb_max_4;
  v6->y = saved_query_aabb_max_8;
  v6->z = saved_query_aabb_max_12;
  v5 = &v19->m_query_aabb_max;
  v19->m_query_aabb_max.x = x;
  v5->y = y;
  v5->z = z;
}

bool __thiscall cached_query_info_t::aabb_is_valid(cached_query_info_t *this)
{
  return this->m_query_aabb_max.x >= this->m_query_aabb_min.x
      && this->m_query_aabb_max.y >= this->m_query_aabb_min.y
      && this->m_query_aabb_max.z >= this->m_query_aabb_min.z;
}

void __userpurge gjk_query_output::cached_query_prolog(
        gjk_query_output *this@<ecx>,
        int a2@<ebp>,
        bool is_server_thread,
        colgeom_visitor_inlined_t<200> *proximity_data,
        int proximity_mask,
        const phys_vec3 *start_origin)
{
  phys_transient_allocator::allocator_state *p_m_allocator_state; // edx
  char *m_cur; // [esp-70h] [ebp-7Ch]
  char *m_end; // [esp-6Ch] [ebp-78h]
  unsigned int m_total_memory_allocated; // [esp-68h] [ebp-74h]
  phys_vec3 v10; // [esp-50h] [ebp-5Ch] BYREF
  float v11; // [esp-40h] [ebp-4Ch]
  float v12; // [esp-3Ch] [ebp-48h]
  float v13; // [esp-38h] [ebp-44h]
  cached_query_info_t *p_m_accum_query_info; // [esp-34h] [ebp-40h]
  phys_vec3 v15; // [esp-30h] [ebp-3Ch] BYREF
  float v16; // [esp-14h] [ebp-20h]
  float v17; // [esp-10h] [ebp-1Ch]
  float v18; // [esp-Ch] [ebp-18h]
  phys_vec3 *p_m_query_aabb_max; // [esp-8h] [ebp-14h]
  gjk_query_output *v20; // [esp-4h] [ebp-10h]
  unsigned int v21[2]; // [esp+0h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]

  v21[0] = a2;
  v21[1] = retaddr;
  v20 = this;
  gjk_query_output::verify_empty(this);
  if ( cached_query_info_t::is_empty(&v20->m_accum_query_info) )
  {
    if ( !cached_query_info_t::is_empty(&v20->m_cached_query_info)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
            1069,
            0,
            "%s",
            "m_cached_query_info.is_empty()") )
    {
      __debugbreak();
    }
  }
  else
  {
    if ( !cached_query_info_t::aabb_is_valid(&v20->m_accum_query_info)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
            1062,
            0,
            "%s",
            "m_accum_query_info.aabb_is_valid()") )
    {
      __debugbreak();
    }
    p_m_query_aabb_max = &v20->m_accum_query_info.m_query_aabb_max;
    v18 = v20->m_accum_query_info.m_query_aabb_max.x + start_origin->x;
    v17 = v20->m_accum_query_info.m_query_aabb_max.y + start_origin->y;
    v16 = v20->m_accum_query_info.m_query_aabb_max.z + start_origin->z;
    v15.x = v18;
    v15.y = v17;
    v15.z = v16;
    p_m_accum_query_info = &v20->m_accum_query_info;
    v13 = v20->m_accum_query_info.m_query_aabb_min.x + start_origin->x;
    v12 = v20->m_accum_query_info.m_query_aabb_min.y + start_origin->y;
    v11 = v20->m_accum_query_info.m_query_aabb_min.z + start_origin->z;
    v10.x = v13;
    v10.y = v12;
    v10.z = v11;
    cached_query_info_t::init_query(
      &v20->m_cached_query_info,
      &v10,
      &v15,
      &CACHED_QUERY_RESIZE_EXTRA,
      v20->m_accum_query_info.m_query_contents,
      v20->m_accum_query_info.m_query_flags);
    gjk_query_output::cached_query_resize(v20, (int)v21, is_server_thread, proximity_data, proximity_mask);
  }
  gjk_query_output::accum_query_reset(v20, start_origin);
  m_cur = v20->m_allocator.m_cur;
  m_end = v20->m_allocator.m_end;
  m_total_memory_allocated = v20->m_allocator.m_total_memory_allocated;
  p_m_allocator_state = &v20->m_allocator_state;
  v20->m_allocator_state.m_first_block = v20->m_allocator.m_first_block;
  p_m_allocator_state->m_cur = m_cur;
  p_m_allocator_state->m_end = m_end;
  p_m_allocator_state->m_total_memory_allocated = m_total_memory_allocated;
}

void __thiscall cached_query_info_t::init_query(
        cached_query_info_t *this,
        const phys_vec3 *query_aabb_min,
        const phys_vec3 *query_aabb_max,
        const phys_vec3 *extra,
        int query_contents,
        unsigned int query_flags)
{
  float v6; // [esp-3Ch] [ebp-48h]
  float v7; // [esp-38h] [ebp-44h]
  float v8; // [esp-1Ch] [ebp-28h]
  float v9; // [esp-18h] [ebp-24h]

  v8 = query_aabb_min->y - extra->y;
  v9 = query_aabb_min->z - extra->z;
  this->m_query_aabb_min.x = query_aabb_min->x - extra->x;
  this->m_query_aabb_min.y = v8;
  this->m_query_aabb_min.z = v9;
  v6 = query_aabb_max->y + extra->y;
  v7 = query_aabb_max->z + extra->z;
  this->m_query_aabb_max.x = query_aabb_max->x + extra->x;
  this->m_query_aabb_max.y = v6;
  this->m_query_aabb_max.z = v7;
  this->m_query_contents = query_contents;
  this->m_query_flags = query_flags;
}

void __thiscall gjk_query_output::cached_query_epilog(gjk_query_output *this)
{
  char *m_cur; // [esp+2Ch] [ebp-Ch]
  char *m_end; // [esp+30h] [ebp-8h]
  unsigned int m_total_memory_allocated; // [esp+34h] [ebp-4h]

  this->m_accum_query_info.m_query_aabb_min.x = this->m_accum_query_info.m_query_aabb_min.x
                                              - this->m_accum_start_origin.x;
  this->m_accum_query_info.m_query_aabb_min.y = this->m_accum_query_info.m_query_aabb_min.y
                                              - this->m_accum_start_origin.y;
  this->m_accum_query_info.m_query_aabb_min.z = this->m_accum_query_info.m_query_aabb_min.z
                                              - this->m_accum_start_origin.z;
  this->m_accum_query_info.m_query_aabb_max.x = this->m_accum_query_info.m_query_aabb_max.x
                                              - this->m_accum_start_origin.x;
  this->m_accum_query_info.m_query_aabb_max.y = this->m_accum_query_info.m_query_aabb_max.y
                                              - this->m_accum_start_origin.y;
  this->m_accum_query_info.m_query_aabb_max.z = this->m_accum_query_info.m_query_aabb_max.z
                                              - this->m_accum_start_origin.z;
  gjk_query_output::reset_cache(this);
  m_cur = this->m_allocator.m_cur;
  m_end = this->m_allocator.m_end;
  m_total_memory_allocated = this->m_allocator.m_total_memory_allocated;
  this->m_allocator_state.m_first_block = this->m_allocator.m_first_block;
  this->m_allocator_state.m_cur = m_cur;
  this->m_allocator_state.m_end = m_end;
  this->m_allocator_state.m_total_memory_allocated = m_total_memory_allocated;
}

void __thiscall gjk_query_output::accum_query_reset(gjk_query_output *this, const phys_vec3 *start_origin)
{
  this->m_accum_start_origin.x = start_origin->x;
  this->m_accum_start_origin.y = start_origin->y;
  this->m_accum_start_origin.z = start_origin->z;
  this->m_accum_query_info.m_query_contents = 0;
  this->m_accum_query_info.m_query_flags = 0;
}

void  gjk_query_cached(float a1@<ebp>, const gjk_query_input *input, gjk_query_output *output)
{
  _BYTE v3[12]; // [esp-Ch] [ebp-3Ch] BYREF
  phys_vec3 mx; // [esp+0h] [ebp-30h] BYREF
  phys_vec3 mn; // [esp+10h] [ebp-20h] BYREF
  gjk_geom_info_t *i; // [esp+20h] [ebp-10h]
  float hit_time[2]; // [esp+24h] [ebp-Ch] BYREF
  float retaddr; // [esp+30h] [ebp+0h]

  hit_time[0] = a1;
  hit_time[1] = retaddr;
  if ( !input->m_contents
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
          1122,
          0,
          "%s",
          "input.m_contents != 0") )
  {
    __debugbreak();
  }
  ++output->m_total_cached_query_count;
  if ( !cached_query_info_t::is_subset(
          &output->m_cached_query_info,
          &output->m_query_aabb_min,
          &output->m_query_aabb_max,
          input->m_contents,
          input->m_gjk_query_flags) )
  {
    cached_query_info_t::add_query(
      &output->m_cached_query_info,
      (int)hit_time,
      &output->m_query_aabb_min,
      &output->m_query_aabb_max,
      &CACHED_QUERY_RESIZE_EXTRA,
      input->m_contents,
      input->m_gjk_query_flags);
    gjk_query_output::cached_query_resize(
      output,
      (int)hit_time,
      input->m_is_server_thread,
      input->m_proximity_data,
      input->m_proximity_mask);
  }
  cached_query_info_t::add_query(
    &output->m_accum_query_info,
    (int)hit_time,
    &output->m_query_aabb_min,
    &output->m_query_aabb_max,
    &PHYS_ZERO_VEC_42,
    input->m_contents,
    input->m_gjk_query_flags);
  for ( i = output->m_total_list_geom_info; i; i = i->m_total_next_link )
  {
    if ( (input->m_contents & gjk_base_t::get_contents(i->m_cg)) != 0 )
    {
      if ( i->m_ent_info )
        LODWORD(mn.w) = i->m_ent_info->m_ent_type == ET_DENT ? 4 : 2;
      else
        LODWORD(mn.w) = 1;
      if ( (input->m_gjk_query_flags & LODWORD(mn.w)) != 0
        && !query_should_pass_entity(input, i->m_ent_info)
        && i->m_query_visitor_count != output->m_query_visitor_count )
      {
        if ( gjk_query_input::is_in_skip_list(input, i)
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
                1149,
                0,
                "%s",
                "!input.is_in_skip_list(gi)") )
        {
          __debugbreak();
        }
        i->m_query_visitor_count = output->m_query_visitor_count;
        mn.z = *(float *)&FLOAT_0_0;
        operator-((phys_vec3 *)&mx.y, &i->m_aabb_min, &input->m_cg_aabb_max);
        operator-((phys_vec3 *)v3, &i->m_aabb_max, &input->m_cg_aabb_min);
        if ( gjk_trace_aabb(
               &input->m_cg_position,
               &input->m_cg_translation,
               (phys_vec3 *)&mx.y,
               (const phys_vec3 *)v3,
               &mn.z) )
        {
          i->m_hit_time = mn.z;
          phys_link_list<gjk_geom_info_t>::add(&output->m_list_geom_info, i);
        }
      }
    }
  }
}

int __thiscall gjk_base_t::get_contents(gjk_base_t *this)
{
  if ( (this->m_flags & 0x10) == 0
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
         90,
         "get_flag(FLAG_CONTENTS_VALID)",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  return this->m_contents;
}

char __thiscall gjk_query_input::is_in_skip_list(gjk_query_input *this, gjk_geom_info_t *gi_)
{
  gjk_geom_info_t *gi_i; // [esp+14h] [ebp-4h]

  for ( gi_i = this->m_geom_skip_list.m_first; gi_i; gi_i = gi_i->m_next_link )
  {
    if ( gi_i == gi_ )
      return 1;
  }
  return 0;
}

void __userpurge cached_query_info_t::add_query(
        cached_query_info_t *this@<ecx>,
        int a2@<ebp>,
        const phys_vec3 *query_aabb_min,
        const phys_vec3 *query_aabb_max,
        const phys_vec3 *extra,
        int query_contents,
        unsigned int query_flags)
{
  const phys_vec3 *v7; // eax
  phys_vec3 *v8; // [esp-C4h] [ebp-D0h]
  float v9; // [esp-BCh] [ebp-C8h]
  float v10; // [esp-B8h] [ebp-C4h]
  phys_vec3 v11; // [esp-A0h] [ebp-ACh] BYREF
  phys_vec3 v12; // [esp-90h] [ebp-9Ch] BYREF
  float v13; // [esp-80h] [ebp-8Ch]
  float v14; // [esp-7Ch] [ebp-88h]
  float v15; // [esp-78h] [ebp-84h]
  phys_vec3 *p_m_query_aabb_max; // [esp-74h] [ebp-80h]
  float v17; // [esp-70h] [ebp-7Ch]
  float v18; // [esp-6Ch] [ebp-78h]
  float v19; // [esp-68h] [ebp-74h]
  float v20; // [esp-60h] [ebp-6Ch]
  float v21; // [esp-5Ch] [ebp-68h]
  float v22; // [esp-58h] [ebp-64h]
  const phys_vec3 *v23; // [esp-54h] [ebp-60h]
  phys_vec3 v24; // [esp-50h] [ebp-5Ch] BYREF
  phys_vec3 v25; // [esp-40h] [ebp-4Ch] BYREF
  float v26; // [esp-2Ch] [ebp-38h]
  float v27; // [esp-28h] [ebp-34h]
  float v28; // [esp-24h] [ebp-30h]
  bool v29; // [esp-20h] [ebp-2Ch]
  float v30; // [esp-1Ch] [ebp-28h]
  float z; // [esp-18h] [ebp-24h]
  float v32; // [esp-14h] [ebp-20h]
  float y; // [esp-10h] [ebp-1Ch]
  float v34; // [esp-Ch] [ebp-18h]
  float x; // [esp-8h] [ebp-14h]
  cached_query_info_t *v36; // [esp-4h] [ebp-10h]
  int v37; // [esp+0h] [ebp-Ch]
  void *v38; // [esp+4h] [ebp-8h]
  void *retaddr; // [esp+Ch] [ebp+0h]

  v37 = a2;
  v38 = retaddr;
  v36 = this;
  x = query_aabb_min->x;
  v34 = query_aabb_max->x;
  v29 = v34 >= x
     && (y = query_aabb_min->y, v32 = query_aabb_max->y, v32 >= y)
     && (z = query_aabb_min->z, v30 = query_aabb_max->z, v30 >= z);
  if ( !v29
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.h",
          234,
          0,
          "%s",
          "::aabb_is_valid(query_aabb_min,query_aabb_max)") )
  {
    __debugbreak();
  }
  if ( !query_contents
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.h",
          235,
          0,
          "%s",
          "query_contents != 0") )
  {
    __debugbreak();
  }
  if ( !query_flags
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.h",
          236,
          0,
          "%s",
          "query_flags != 0") )
  {
    __debugbreak();
  }
  if ( cached_query_info_t::is_empty(v36) )
  {
    cached_query_info_t::init_query(v36, query_aabb_min, query_aabb_max, extra, query_contents, query_flags);
  }
  else
  {
    v28 = v36->m_query_aabb_min.x + extra->x;
    v27 = v36->m_query_aabb_min.y + extra->y;
    v26 = v36->m_query_aabb_min.z + extra->z;
    v25.x = v28;
    v25.y = v27;
    v25.z = v26;
    v23 = phys_min(&v24, &v25, query_aabb_min);
    v22 = v23->x - extra->x;
    v21 = v23->y - extra->y;
    v20 = v23->z - extra->z;
    v17 = v22;
    v18 = v21;
    v19 = v20;
    v36->m_query_aabb_min.x = v22;
    v36->m_query_aabb_min.y = v18;
    v36->m_query_aabb_min.z = v19;
    p_m_query_aabb_max = &v36->m_query_aabb_max;
    v15 = v36->m_query_aabb_max.x - extra->x;
    v14 = v36->m_query_aabb_max.y - extra->y;
    v13 = v36->m_query_aabb_max.z - extra->z;
    v12.x = v15;
    v12.y = v14;
    v12.z = v13;
    v7 = phys_max(&v11, &v12, query_aabb_max);
    v9 = v7->y + extra->y;
    v10 = v7->z + extra->z;
    v8 = &v36->m_query_aabb_max;
    v36->m_query_aabb_max.x = v7->x + extra->x;
    v8->y = v9;
    v8->z = v10;
    v36->m_query_contents |= query_contents;
    v36->m_query_flags |= query_flags;
  }
}

bool __thiscall cached_query_info_t::is_subset(
        cached_query_info_t *this,
        const phys_vec3 *query_aabb_min,
        const phys_vec3 *query_aabb_max,
        int query_contents,
        unsigned int query_flags)
{
  bool v7; // [esp+4h] [ebp-50h]

  v7 = query_aabb_max->x >= query_aabb_min->x
    && query_aabb_max->y >= query_aabb_min->y
    && query_aabb_max->z >= query_aabb_min->z;
  if ( !v7
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.h",
          268,
          0,
          "%s",
          "::aabb_is_valid(query_aabb_min,query_aabb_max)") )
  {
    __debugbreak();
  }
  if ( !query_contents
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.h",
          269,
          0,
          "%s",
          "query_contents != 0") )
  {
    __debugbreak();
  }
  if ( !query_flags
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.h",
          270,
          0,
          "%s",
          "query_flags != 0") )
  {
    __debugbreak();
  }
  return !cached_query_info_t::is_empty(this)
      && cached_query_info_t::is_subset_aabb(this, query_aabb_min, query_aabb_max)
      && (query_contents & this->m_query_contents) == query_contents
      && (query_flags & this->m_query_flags) == query_flags;
}

bool __thiscall cached_query_info_t::is_subset_aabb(
        cached_query_info_t *this,
        const phys_vec3 *query_aabb_min,
        const phys_vec3 *query_aabb_max)
{
  return query_aabb_min->x >= this->m_query_aabb_min.x
      && query_aabb_min->y >= this->m_query_aabb_min.y
      && query_aabb_min->z >= this->m_query_aabb_min.z
      && this->m_query_aabb_max.x >= query_aabb_max->x
      && this->m_query_aabb_max.y >= query_aabb_max->y
      && this->m_query_aabb_max.z >= query_aabb_max->z;
}

char __cdecl gjk_trace_aabb(
        const phys_vec3 *p0,
        const phys_vec3 *dir,
        const phys_vec3 *aabb_min,
        const phys_vec3 *aabb_max,
        float *t_)
{
  float d1; // [esp+30h] [ebp-14h]
  float d1a; // [esp+30h] [ebp-14h]
  int i; // [esp+38h] [ebp-Ch]
  float min_t; // [esp+3Ch] [ebp-8h] BYREF
  float max_t; // [esp+40h] [ebp-4h] BYREF

  min_t = *(float *)&FLOAT_0_0;
  max_t = FLOAT_1_0;
  for ( i = 0; i < 3; ++i )
  {
    if ( i < 0
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
           32,
           "i >= 0 && i < 3",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    if ( i < 0
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
           32,
           "i >= 0 && i < 3",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    d1 = *(&p0->x + i) - *(&aabb_max->x + i);
    if ( i < 0
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
           32,
           "i >= 0 && i < 3",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    if ( !gjk_trace_plane_1d(d1, d1 + *(&dir->x + i), &min_t, &max_t, 0) )
      return 0;
    if ( i < 0
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
           32,
           "i >= 0 && i < 3",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    if ( i < 0
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
           32,
           "i >= 0 && i < 3",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    LODWORD(d1a) = COERCE_UNSIGNED_INT(*(&p0->x + i) - *(&aabb_min->x + i)) ^ _mask__NegFloat_;
    if ( i < 0 )
    {
      if ( _tlAssert(
             "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
             32,
             "i >= 0 && i < 3",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
    }
    if ( !gjk_trace_plane_1d(d1a, d1a - *(&dir->x + i), &min_t, &max_t, 0) )
      return 0;
  }
  *t_ = min_t;
  return 1;
}

bool __cdecl gjk_trace_plane_1d(float d1, float d2, float *min_t, float *max_t, bool use_iw_logic)
{
  float v6; // [esp+0h] [ebp-10h]
  float v7; // [esp+8h] [ebp-8h]
  float delta; // [esp+Ch] [ebp-4h]
  float d1a; // [esp+18h] [ebp+8h]

  if ( d1 <= 0.0 )
  {
    if ( d2 > 0.0 )
    {
      v7 = d1 - d2;
      if ( (float)(d1 - d2) >= 0.0
        && !Assert_MyHandler(
              "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.h",
              644,
              0,
              "%s",
              "delta < 0.0f") )
      {
        __debugbreak();
      }
      if ( !use_iw_logic )
        d1 = d1 - 0.0625;
      if ( (float)(*max_t * v7) > d1 )
      {
        *max_t = d1 / v7;
        if ( *min_t >= *max_t )
          return 0;
      }
    }
    return 1;
  }
  if ( d1 >= 0.125 )
    v6 = FLOAT_0_125;
  else
    v6 = d1;
  if ( d2 < v6 )
  {
    delta = d1 - d2;
    if ( (float)(d1 - d2) <= 0.0
      && !Assert_MyHandler(
            "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.h",
            629,
            0,
            "%s",
            "delta > 0.0f") )
    {
      __debugbreak();
    }
    d1a = d1 - 0.125;
    if ( d1a > (float)(*min_t * delta) )
    {
      *min_t = d1a / delta;
      if ( *min_t >= *max_t )
        return 0;
    }
    return 1;
  }
  return d1 <= 0.0625 && !use_iw_logic;
}

char __cdecl query_should_pass_entity(const gjk_query_input *input, gjk_entity_info_t *ent_info)
{
  const gentity_s *gent; // eax
  int entityIndex; // [esp+8h] [ebp-8h]

  if ( !ent_info )
    return 0;
  if ( ent_info->m_ent_type )
  {
    if ( ent_info->m_ent_type == ET_CENT )
    {
      entityIndex = gjk_entity_info_t::get_cent(ent_info)->nextState.number;
      return ((unsigned int)&cls.rankedServers[711].game[35] & CG_GetEntity(0, entityIndex)->nextState.lerp.eFlags2) != 0
          || entityIndex == input->m_pass_entity_num;
    }
    else if ( ent_info->m_ent_type == ET_GLASS )
    {
      return 0;
    }
    else
    {
      if ( ent_info->m_ent_type != ET_DENT
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
              1112,
              0,
              "%s",
              "ent_info->is_dent()") )
      {
        __debugbreak();
      }
      gjk_entity_info_t::get_dent(ent_info);
      return 0;
    }
  }
  else
  {
    gent = gjk_entity_info_t::get_gent(ent_info);
    return query_should_pass_gentity(input, gent);
  }
}

const DynEntityDef *__thiscall gjk_entity_info_t::get_dent(gjk_entity_info_t *this)
{
  if ( (this->m_ent_type != ET_DENT || !this->m_ent)
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.h",
          88,
          0,
          "%s",
          "m_ent_type == ET_DENT && m_ent != NULL") )
  {
    __debugbreak();
  }
  return (const DynEntityDef *)this->m_ent;
}

void  setup_gjk_capsule(
        float a1@<ebp>,
        float *mins,
        float *maxs,
        float radius_adjust,
        gjk_double_sphere_t *gjk_capsule)
{
  float v5; // [esp-Ch] [ebp-ACh]
  float v6; // [esp-8h] [ebp-A8h]
  float v7; // [esp-4h] [ebp-A4h]
  float v8; // [esp+14h] [ebp-8Ch]
  float v9; // [esp+18h] [ebp-88h]
  float v10; // [esp+1Ch] [ebp-84h]
  float v11; // [esp+50h] [ebp-50h]
  float v12[2]; // [esp+74h] [ebp-2Ch] BYREF
  float v13; // [esp+7Ch] [ebp-24h]
  phys_vec3 pv_maxs; // [esp+80h] [ebp-20h] BYREF
  phys_vec3 pv_mins; // [esp+90h] [ebp-10h]
  float retaddr; // [esp+A0h] [ebp+0h]

  pv_mins.y = a1;
  pv_mins.z = retaddr;
  Phys_Vec3ToNitrousVec(mins, (phys_vec3 *)&pv_maxs.y);
  Phys_Vec3ToNitrousVec(maxs, (phys_vec3 *)v12);
  if ( (float)(v12[0] - pv_maxs.y) <= (float)(v13 - pv_maxs.w) )
    v11 = v12[0] - pv_maxs.y;
  else
    v11 = v13 - pv_maxs.w;
  v8 = v12[0] - (float)(0.5 * v11);
  v9 = v12[1] - (float)(0.5 * v11);
  v10 = v13 - (float)(0.5 * v11);
  v5 = pv_maxs.y + (float)(0.5 * v11);
  v6 = pv_maxs.z + (float)(0.5 * v11);
  v7 = pv_maxs.w + (float)(0.5 * v11);
  gjk_capsule->m_list_center[0].x = v8;
  gjk_capsule->m_list_center[0].y = v9;
  gjk_capsule->m_list_center[0].z = v10;
  gjk_capsule->m_list_center[1].x = v5;
  gjk_capsule->m_list_center[1].y = v6;
  gjk_capsule->m_list_center[1].z = v7;
  gjk_capsule->m_list_radius[0] = *(float *)&FLOAT_0_0;
  gjk_capsule->m_list_radius[1] = *(float *)&FLOAT_0_0;
  gjk_capsule->m_geom_radius = (float)(0.5 * v11) + radius_adjust;
  if ( gjk_capsule->m_geom_radius <= 0.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
          1193,
          0,
          "%s",
          "gjk_capsule->m_geom_radius > 0.0f") )
  {
    __debugbreak();
  }
  gjk_capsule->m_center.x = 0.5 * (float)(v8 + v5);
  gjk_capsule->m_center.y = 0.5 * (float)(v9 + v6);
  gjk_capsule->m_center.z = 0.5 * (float)(v10 + v7);
  gjk_capsule->m_count = 2;
}

void  setup_gjk_cylinder(
        float a1@<ebp>,
        float *mins,
        float *maxs,
        float radius_adjust,
        gjk_cylinder_t *gjk_cylinder)
{
  float v5; // [esp-2Ch] [ebp-74h]
  float v6; // [esp-8h] [ebp-50h]
  float v7; // [esp+1Ch] [ebp-2Ch] BYREF
  float v8; // [esp+24h] [ebp-24h]
  phys_vec3 pv_maxs; // [esp+28h] [ebp-20h] BYREF
  phys_vec3 pv_mins; // [esp+38h] [ebp-10h]
  float retaddr; // [esp+48h] [ebp+0h]

  pv_mins.y = a1;
  pv_mins.z = retaddr;
  Phys_Vec3ToNitrousVec(mins, (phys_vec3 *)&pv_maxs.y);
  Phys_Vec3ToNitrousVec(maxs, (phys_vec3 *)&v7);
  if ( (float)(v7 - pv_maxs.y) <= (float)(v8 - pv_maxs.w) )
    v6 = v7 - pv_maxs.y;
  else
    v6 = v8 - pv_maxs.w;
  gjk_cylinder->halfHeight = 0.5 * (float)(v8 - pv_maxs.w);
  gjk_cylinder->radius = 0.5 * v6;
  gjk_cylinder->direction = 2;
  gjk_cylinder->m_geom_radius = radius_adjust;
  phys_mat44::operator=(&gjk_cylinder->xform, &PHYS_IDENTITY_MATRIX_42);
  v5 = (float)(pv_maxs.w + v8) * 0.5;
  gjk_cylinder->xform.w.x = *(float *)&FLOAT_0_0;
  gjk_cylinder->xform.w.y = *(float *)&FLOAT_0_0;
  gjk_cylinder->xform.w.z = v5;
}

bool __userpurge gjk_aabb_t::is_walkable@<al>(
        gjk_aabb_t *this@<ecx>,
        float a2@<ebp>,
        const phys_vec3 *hit_point,
        const phys_vec3 *up)
{
  char v5; // [esp-36h] [ebp-9Ah]
  bool v6; // [esp-35h] [ebp-99h]
  float v7; // [esp-30h] [ebp-94h]
  float v8; // [esp-2Ch] [ebp-90h]
  float v9; // [esp-28h] [ebp-8Ch]
  char v10; // [esp-1Eh] [ebp-82h]
  bool v11; // [esp-1Dh] [ebp-81h]
  float v12; // [esp-18h] [ebp-7Ch]
  float v13; // [esp-14h] [ebp-78h]
  float v14; // [esp-10h] [ebp-74h]
  int j; // [esp-Ch] [ebp-70h]
  unsigned int v16[2]; // [esp-8h] [ebp-6Ch]
  int i; // [esp+0h] [ebp-64h]
  int v18; // [esp+14h] [ebp-50h]
  float v19; // [esp+18h] [ebp-4Ch]
  float v20; // [esp+1Ch] [ebp-48h]
  phys_vec3 *v21; // [esp+20h] [ebp-44h]
  phys_vec3 *v22; // [esp+24h] [ebp-40h]
  unsigned int v23[3]; // [esp+28h] [ebp-3Ch]
  phys_vec3 aabb_min; // [esp+34h] [ebp-30h]
  phys_vec3 *p_m_center_local; // [esp+44h] [ebp-20h]
  phys_vec3 *p_m_dims; // [esp+48h] [ebp-1Ch]
  float v27; // [esp+4Ch] [ebp-18h]
  float v28; // [esp+50h] [ebp-14h]
  gjk_aabb_t *v29; // [esp+54h] [ebp-10h]
  float furthest_walk_normal; // [esp+58h] [ebp-Ch]
  float furthest_dist; // [esp+5Ch] [ebp-8h]
  float retaddr; // [esp+64h] [ebp+0h]

  furthest_walk_normal = a2;
  furthest_dist = retaddr;
  v29 = this;
  v28 = FLOAT_N100000_0;
  v27 = *(float *)&FLOAT_0_0;
  p_m_dims = &this->m_dims;
  p_m_center_local = &this->m_center_local;
  aabb_min.w = this->m_center_local.x - this->m_dims.x;
  aabb_min.z = this->m_center_local.y - this->m_dims.y;
  aabb_min.y = this->m_center_local.z - this->m_dims.z;
  v23[0] = LODWORD(aabb_min.w);
  v23[1] = LODWORD(aabb_min.z);
  v23[2] = LODWORD(aabb_min.y);
  v22 = &this->m_dims;
  v21 = &this->m_center_local;
  v20 = this->m_center_local.x + this->m_dims.x;
  v19 = this->m_center_local.y + this->m_dims.y;
  *(float *)&v18 = this->m_center_local.z + this->m_dims.z;
  *(float *)v16 = v20;
  *(float *)&v16[1] = v19;
  i = v18;
  for ( j = 0; j < 3; ++j )
  {
    if ( j < 0
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
           32,
           "i >= 0 && i < 3",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    if ( j < 0
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
           32,
           "i >= 0 && i < 3",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    if ( j < 0
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
           32,
           "i >= 0 && i < 3",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    LODWORD(v14) = *((unsigned int *)&hit_point->x + j) ^ _mask__NegFloat_;
    LODWORD(v13) = v23[j] ^ _mask__NegFloat_;
    LODWORD(v12) = *((unsigned int *)&up->x + j) ^ _mask__NegFloat_;
    v11 = v14 >= (float)(0.99900001 * v13) || (float)(0.99900001 * v14) >= v13;
    if ( v11 && v12 >= 0.69999999 )
    {
      v10 = 1;
    }
    else
    {
      if ( (float)(v14 - v13) > v28 )
      {
        v28 = v14 - v13;
        v27 = v12;
      }
      v10 = 0;
    }
    if ( v10 )
      return 1;
    if ( j < 0
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
           32,
           "i >= 0 && i < 3",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    if ( j < 0
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
           32,
           "i >= 0 && i < 3",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    if ( j < 0
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
           32,
           "i >= 0 && i < 3",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    v9 = *(&hit_point->x + j);
    v8 = *(float *)&v16[j];
    v7 = *(&up->x + j);
    v6 = v9 >= (float)(0.99900001 * v8) || (float)(0.99900001 * v9) >= v8;
    if ( v6 && v7 >= 0.69999999 )
    {
      v5 = 1;
    }
    else
    {
      if ( (float)(v9 - v8) > v28 )
      {
        v28 = v9 - v8;
        v27 = v7;
      }
      v5 = 0;
    }
    if ( v5 )
      return 1;
  }
  return v27 >= 0.69999999;
}

bool  is_walkable@<al>(
        unsigned int a1@<ebp>,
        const cbrush_t *brush,
        const phys_vec3 *hit_point_loc,
        const phys_vec3 *up_loc)
{
  char v5; // [esp-1Eh] [ebp-7Eh]
  bool v6; // [esp-1Dh] [ebp-7Dh]
  float dist; // [esp-18h] [ebp-78h]
  float v8; // [esp-14h] [ebp-74h]
  float v9; // [esp-10h] [ebp-70h]
  float v10; // [esp-Ch] [ebp-6Ch] BYREF
  float v11; // [esp-8h] [ebp-68h]
  float v12; // [esp-4h] [ebp-64h]
  cbrushside_t *v13; // [esp+10h] [ebp-50h]
  unsigned int k; // [esp+14h] [ebp-4Ch]
  float v15; // [esp+18h] [ebp-48h]
  cbrushside_t *side; // [esp+1Ch] [ebp-44h]
  float v17; // [esp+20h] [ebp-40h]
  float v18; // [esp+24h] [ebp-3Ch]
  float v19; // [esp+28h] [ebp-38h]
  float v20; // [esp+2Ch] [ebp-34h]
  float v21; // [esp+30h] [ebp-30h]
  char v22; // [esp+36h] [ebp-2Ah]
  bool v23; // [esp+37h] [ebp-29h]
  float v24; // [esp+38h] [ebp-28h]
  float v25; // [esp+3Ch] [ebp-24h]
  float v26; // [esp+40h] [ebp-20h]
  float v27; // [esp+44h] [ebp-1Ch]
  unsigned int j; // [esp+48h] [ebp-18h]
  float v29; // [esp+4Ch] [ebp-14h]
  float v30; // [esp+50h] [ebp-10h]
  unsigned int i; // [esp+54h] [ebp-Ch]
  float furthest_walk_normal; // [esp+58h] [ebp-8h]
  float retaddr; // [esp+60h] [ebp+0h]

  i = a1;
  furthest_walk_normal = retaddr;
  v30 = FLOAT_N100000_0;
  v29 = *(float *)&FLOAT_0_0;
  for ( j = 0; j < 3; ++j )
  {
    LODWORD(v27) = *((unsigned int *)&hit_point_loc->x + j) ^ _mask__NegFloat_;
    LODWORD(v26) = LODWORD(brush->mins[j]) ^ _mask__NegFloat_;
    LODWORD(v25) = *((unsigned int *)&up_loc->x + j) ^ _mask__NegFloat_;
    v24 = FLOAT_0_99900001;
    v23 = v27 >= (float)(0.99900001 * v26) || (float)(v24 * v27) >= v26;
    if ( v23 && v25 >= 0.69999999 )
    {
      v22 = 1;
    }
    else
    {
      v21 = v27 - v26;
      if ( (float)(v27 - v26) > v30 )
      {
        v30 = v21;
        v29 = v25;
      }
      v22 = 0;
    }
    if ( v22 )
      return 1;
    v20 = *(&hit_point_loc->x + j);
    v19 = brush->maxs[j];
    v18 = *(&up_loc->x + j);
    v17 = FLOAT_0_99900001;
    HIBYTE(side) = v20 >= (float)(0.99900001 * v19) || (float)(v17 * v20) >= v19;
    if ( HIBYTE(side) && v18 >= 0.69999999 )
    {
      BYTE2(side) = 1;
    }
    else
    {
      v15 = v20 - v19;
      if ( (float)(v20 - v19) > v30 )
      {
        v30 = v15;
        v29 = v18;
      }
      BYTE2(side) = 0;
    }
    if ( BYTE2(side) )
      return 1;
  }
  for ( k = 0; k < brush->numsides; ++k )
  {
    v13 = &brush->sides[k];
    Phys_Vec3ToNitrousVec(v13->plane->normal, (phys_vec3 *)&v10);
    v9 = (float)((float)(v10 * up_loc->x) + (float)(v11 * up_loc->y)) + (float)(v12 * up_loc->z);
    v8 = (float)((float)(hit_point_loc->x * v10) + (float)(hit_point_loc->y * v11)) + (float)(hit_point_loc->z * v12);
    dist = v13->plane->dist;
    v6 = v8 >= (float)(0.99900001 * dist) || (float)(0.99900001 * v8) >= dist;
    if ( v6 && v9 >= 0.69999999 )
    {
      v5 = 1;
    }
    else
    {
      if ( (float)(v8 - dist) > v30 )
      {
        v30 = v8 - dist;
        v29 = v9;
      }
      v5 = 0;
    }
    if ( v5 )
      return 1;
  }
  return v29 >= 0.69999999;
}

bool  is_walkable@<al>(
        int a1@<ebp>,
        const CollisionPartition *partition,
        const phys_vec3 *hit_point_loc,
        const phys_vec3 *up_loc)
{
  char v5; // [esp-12h] [ebp-82h]
  bool v6; // [esp-11h] [ebp-81h]
  float v7; // [esp-Ch] [ebp-7Ch]
  float v8; // [esp-8h] [ebp-78h]
  float v9; // [esp-4h] [ebp-74h]
  float walk_normal; // [esp+0h] [ebp-70h]
  float plane_dist; // [esp+4h] [ebp-6Ch] BYREF
  float hit_point_dist; // [esp+8h] [ebp-68h]
  float nnormal; // [esp+Ch] [ebp-64h]
  float v14[3]; // [esp+20h] [ebp-50h] BYREF
  float triNormalScaledByAreaX2[3]; // [esp+2Ch] [ebp-44h] BYREF
  float v0_v2[3]; // [esp+38h] [ebp-38h] BYREF
  float v0_v1[3]; // [esp+44h] [ebp-2Ch]
  const float *v2; // [esp+50h] [ebp-20h]
  const float *v1; // [esp+54h] [ebp-1Ch]
  const float *v0; // [esp+58h] [ebp-18h]
  unsigned __int16 *indices; // [esp+5Ch] [ebp-14h]
  int triIndex; // [esp+60h] [ebp-10h]
  int i; // [esp+64h] [ebp-Ch]
  float furthest_walk_normal; // [esp+68h] [ebp-8h]
  float retaddr; // [esp+70h] [ebp+0h]

  i = a1;
  furthest_walk_normal = retaddr;
  *(float *)&triIndex = FLOAT_N100000_0;
  indices = *(unsigned __int16 **)&FLOAT_0_0;
  for ( v0 = 0; (int)v0 < partition->triCount; v0 = (const float *)((char *)v0 + 1) )
  {
    v1 = (const float *)((char *)v0 + partition->firstTri);
    v2 = (const float *)&cm.triIndices[3 * (unsigned int)v1];
    LODWORD(v0_v1[2]) = cm.verts[*(unsigned __int16 *)v2];
    LODWORD(v0_v1[1]) = cm.verts[*((unsigned __int16 *)v2 + 1)];
    LODWORD(v0_v1[0]) = cm.verts[*((unsigned __int16 *)v2 + 2)];
    v0_v2[0] = *(float *)LODWORD(v0_v1[2]) - *(float *)LODWORD(v0_v1[1]);
    v0_v2[1] = *(float *)(LODWORD(v0_v1[2]) + 4) - *(float *)(LODWORD(v0_v1[1]) + 4);
    v0_v2[2] = *(float *)(LODWORD(v0_v1[2]) + 8) - *(float *)(LODWORD(v0_v1[1]) + 8);
    triNormalScaledByAreaX2[0] = *(float *)LODWORD(v0_v1[2]) - *(float *)LODWORD(v0_v1[0]);
    triNormalScaledByAreaX2[1] = *(float *)(LODWORD(v0_v1[2]) + 4) - *(float *)(LODWORD(v0_v1[0]) + 4);
    triNormalScaledByAreaX2[2] = *(float *)(LODWORD(v0_v1[2]) + 8) - *(float *)(LODWORD(v0_v1[0]) + 8);
    Vec3Cross(triNormalScaledByAreaX2, v0_v2, v14);
    Phys_Vec3ToNitrousVec(v14, (phys_vec3 *)&plane_dist);
    walk_normal = Abs(&plane_dist);
    if ( walk_normal > 0.000099999997 )
    {
      v9 = (float)((float)((float)(hit_point_loc->x * plane_dist) + (float)(hit_point_loc->y * hit_point_dist))
                 + (float)(hit_point_loc->z * nnormal))
         / walk_normal;
      v8 = (float)((float)((float)(*(float *)LODWORD(v0_v1[2]) * v14[0])
                         + (float)(*(float *)(LODWORD(v0_v1[2]) + 4) * v14[1]))
                 + (float)(*(float *)(LODWORD(v0_v1[2]) + 8) * v14[2]))
         / walk_normal;
      v7 = (float)((float)((float)(plane_dist * up_loc->x) + (float)(hit_point_dist * up_loc->y))
                 + (float)(nnormal * up_loc->z))
         / walk_normal;
      v6 = v9 >= (float)(0.99900001 * v8) || (float)(0.99900001 * v9) >= v8;
      if ( v6 && v7 >= 0.69999999 )
      {
        v5 = 1;
      }
      else
      {
        if ( (float)(v9 - v8) > *(float *)&triIndex )
        {
          *(float *)&triIndex = v9 - v8;
          *(float *)&indices = v7;
        }
        v5 = 0;
      }
      if ( v5 )
        return 1;
    }
  }
  return *(float *)&indices >= 0.69999999;
}

void  project(
        int a1@<ebp>,
        const phys_vec3 *point,
        phys_static_array<geom_plane,128> *list_geom_plane,
        phys_vec3 *result)
{
  float v4; // [esp-2Ch] [ebp-178h]
  float v5; // [esp-28h] [ebp-174h]
  float v6; // [esp-14h] [ebp-160h]
  int m; // [esp-Ch] [ebp-158h]
  float v8; // [esp-8h] [ebp-154h]
  float v9; // [esp-4h] [ebp-150h]
  float v10; // [esp+0h] [ebp-14Ch]
  float v11; // [esp+4h] [ebp-148h]
  float v12; // [esp+8h] [ebp-144h]
  float ck_[3]; // [esp+Ch] [ebp-140h]
  float temp1[4]; // [esp+18h] [ebp-134h]
  int k; // [esp+28h] [ebp-124h]
  float ndist_sq; // [esp+2Ch] [ebp-120h]
  float det; // [esp+30h] [ebp-11Ch]
  int j; // [esp+34h] [ebp-118h]
  float v20[4]; // [esp+44h] [ebp-108h]
  int new_set; // [esp+54h] [ebp-F8h]
  float *new_dist_sq; // [esp+58h] [ebp-F4h]
  int i; // [esp+5Ch] [ebp-F0h]
  unsigned int v24[2]; // [esp+60h] [ebp-ECh]
  geom_plane *list_gp[4]; // [esp+6Ch] [ebp-E0h] BYREF
  float mat[4][4]; // [esp+7Ch] [ebp-D0h]
  float rs[4]; // [esp+BCh] [ebp-90h]
  int max_index; // [esp+CCh] [ebp-80h]
  int v29; // [esp+D0h] [ebp-7Ch]
  int v30; // [esp+D4h] [ebp-78h]
  int v31; // [esp+D8h] [ebp-74h]
  int new_index; // [esp+DCh] [ebp-70h]
  float *p_x; // [esp+E0h] [ebp-6Ch]
  float v34; // [esp+E4h] [ebp-68h]
  float dist; // [esp+E8h] [ebp-64h]
  geom_plane *v36; // [esp+ECh] [ebp-60h]
  geom_plane *m_slot_array; // [esp+F0h] [ebp-5Ch]
  float v38; // [esp+F4h] [ebp-58h]
  float *v39; // [esp+F8h] [ebp-54h]
  int m_alloc_count; // [esp+FCh] [ebp-50h]
  int support_gp_dist; // [esp+100h] [ebp-4Ch]
  geom_plane *support_gp; // [esp+104h] [ebp-48h]
  float v43; // [esp+108h] [ebp-44h]
  int iter; // [esp+10Ch] [ebp-40h]
  int break_val; // [esp+110h] [ebp-3Ch]
  int cur_set; // [esp+114h] [ebp-38h]
  float cur_dist_sq; // [esp+118h] [ebp-34h]
  phys_vec3 cur_result; // [esp+11Ch] [ebp-30h]
  geom_plane *v49; // [esp+12Ch] [ebp-20h]
  geom_plane *v50; // [esp+130h] [ebp-1Ch]
  geom_plane *v51; // [esp+134h] [ebp-18h]
  geom_plane *gp; // [esp+138h] [ebp-14h]
  phys_static_array<geom_plane,128>::iterator gp_i_end; // [esp+13Ch] [ebp-10h]
  int v54; // [esp+140h] [ebp-Ch]
  phys_static_array<geom_plane,128>::iterator gp_i; // [esp+144h] [ebp-8h]
  geom_plane *retaddr; // [esp+14Ch] [ebp+0h]

  v54 = a1;
  gp_i.m_ptr = retaddr;
  gp_i_end.m_ptr = list_geom_plane->m_slot_array;
  gp = gp_i_end.m_ptr;
  v51 = &list_geom_plane->m_slot_array[list_geom_plane->m_alloc_count];
  v50 = v51;
  while ( v50 != gp )
  {
    v49 = gp;
    gp->m_lambda = *(float *)&FLOAT_0_0;
    ++gp;
  }
  break_val = SLODWORD(point->x);
  cur_set = SLODWORD(point->y);
  cur_dist_sq = point->z;
  cur_result.x = point->w;
  iter = *(int *)&FLOAT_0_0;
  v43 = 0.0;
  support_gp = (geom_plane *)-1;
  for ( support_gp_dist = 0; ; ++support_gp_dist )
  {
    m_alloc_count = list_geom_plane->m_alloc_count;
    if ( support_gp_dist >= m_alloc_count )
      break;
    v39 = 0;
    v38 = FLOAT_9_9999998e12;
    m_slot_array = list_geom_plane->m_slot_array;
    v36 = m_slot_array;
    LODWORD(dist) = &list_geom_plane->m_slot_array[list_geom_plane->m_alloc_count];
    v34 = dist;
    while ( (geom_plane *)LODWORD(v34) != v36 )
    {
      p_x = &v36->m_normal.x;
      if ( v36->m_active && p_x[9] == 0.0 )
      {
        *(float *)&new_index = (float)((float)((float)(*(float *)&break_val * *p_x)
                                             + (float)(*(float *)&cur_set * p_x[1]))
                                     + (float)(cur_dist_sq * p_x[2]))
                             - p_x[10];
        if ( v38 > *(float *)&new_index )
        {
          v38 = *(float *)&new_index;
          v39 = p_x;
        }
      }
      ++v36;
    }
    if ( !v39 )
    {
      support_gp = 0;
      break;
    }
    if ( v38 >= 0.0 )
    {
      support_gp = (geom_plane *)1;
      break;
    }
    v39[9] = FLOAT_N1_0;
    if ( LODWORD(v43) > 0xE
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
            1494,
            0,
            "%s",
            "cur_set >= 0 && cur_set < 15") )
    {
      __debugbreak();
    }
    if ( (LOBYTE(v43) & 1) != 0 )
    {
      if ( (LOBYTE(v43) & 2) != 0 )
        v31 = ((LOBYTE(v43) & 4) != 0) + 2;
      else
        v31 = 1;
      v30 = v31;
    }
    else
    {
      v30 = 0;
    }
    v29 = v30;
    if ( (LOBYTE(v43) & 8) != 0 )
    {
      max_index = 4;
    }
    else
    {
      if ( (LOBYTE(v43) & 4) != 0 )
      {
        LODWORD(rs[3]) = 3;
      }
      else
      {
        if ( (LOBYTE(v43) & 2) != 0 )
          LODWORD(rs[2]) = 2;
        else
          LODWORD(rs[2]) = (LOBYTE(v43) & 1) != 0;
        rs[3] = rs[2];
      }
      max_index = LODWORD(rs[3]);
    }
    LODWORD(rs[1]) = max_index;
    mat[3][v29 + 1] = v39[10]
                    - (float)((float)((float)(point->x * *v39) + (float)(point->y * v39[1])) + (float)(point->z * v39[2]));
    *(&list_gp[4 * v29 + 1] + v29) = (geom_plane *)LODWORD(FLOAT_1_0);
    v24[v29] = v39;
    for ( i = 0; i < SLODWORD(rs[1]); ++i )
    {
      if ( (LODWORD(v43) & (1 << i)) != 0 )
      {
        new_dist_sq = (float *)v24[i];
        new_set = v24[v29];
        *((float *)&list_gp[4 * v29 + 1] + i) = (float)((float)(*(float *)new_set * *new_dist_sq)
                                                      + (float)(*(float *)(new_set + 4) * new_dist_sq[1]))
                                              + (float)(*(float *)(new_set + 8) * new_dist_sq[2]);
        *(&list_gp[4 * i + 1] + v29) = *(&list_gp[4 * v29 + 1] + i);
      }
    }
    v20[3] = mat[3][v29 + 1];
    v20[2] = v20[3] * v20[3];
    LODWORD(v20[1]) = 1 << v29;
    v20[v29 + 1] = mat[3][v29 + 1];
    for ( j = 0; j < SLODWORD(rs[1]); ++j )
    {
      if ( (LODWORD(v43) & (1 << j)) != 0 )
      {
        det = mat[3][v29 + 1] - (float)(mat[3][j + 1] * *((float *)&list_gp[4 * v29 + 1] + j));
        if ( det >= 0.0 )
        {
          ndist_sq = mat[3][j + 1] - (float)(mat[3][v29 + 1] * *((float *)&list_gp[4 * v29 + 1] + j));
          if ( ndist_sq >= 0.0 )
          {
            k = *((int *)&list_gp[4 * v29 + 1] + j);
            temp1[3] = 1.0 - (float)(*(float *)&k * *(float *)&k);
            if ( temp1[3] > 0.0000099999997 )
            {
              temp1[2] = (float)((float)((float)((float)((float)(2.0 * ndist_sq) * *((float *)&list_gp[4 * v29 + 1] + j))
                                               + det)
                                       * det)
                               + (float)(ndist_sq * ndist_sq))
                       / (float)(temp1[3] * temp1[3]);
              if ( temp1[2] > v20[2] )
              {
                v20[v29 + 1] = det / temp1[3];
                v20[j + 1] = ndist_sq / temp1[3];
                v20[2] = temp1[2];
                LODWORD(v20[1]) = (1 << j) | (1 << v29);
              }
            }
          }
        }
        LODWORD(temp1[1]) = j + 1;
        while ( SLODWORD(temp1[1]) < SLODWORD(rs[1]) )
        {
          if ( (LODWORD(v43) & (1 << SLOBYTE(temp1[1]))) != 0 )
          {
            ck_[v29] = (float)(*((float *)&list_gp[4 * LODWORD(temp1[1]) + 1] + j) * mat[3][LODWORD(temp1[1]) + 1])
                     - (float)(mat[3][j + 1] * *((float *)&list_gp[4 * LODWORD(temp1[1]) + 1] + LODWORD(temp1[1])));
            ck_[j] = (float)(COERCE_FLOAT((unsigned int)*(&list_gp[4 * LODWORD(temp1[1]) + 1] + v29) ^ _mask__NegFloat_)
                           * mat[3][LODWORD(temp1[1]) + 1])
                   + (float)(mat[3][v29 + 1] * *((float *)&list_gp[4 * LODWORD(temp1[1]) + 1] + LODWORD(temp1[1])));
            ck_[LODWORD(temp1[1])] = (float)(*((float *)&list_gp[4 * LODWORD(temp1[1]) + 1] + v29) * mat[3][j + 1])
                                   - (float)(mat[3][v29 + 1] * *((float *)&list_gp[4 * LODWORD(temp1[1]) + 1] + j));
            v12 = (float)((float)(ck_[v29] * *((float *)&list_gp[4 * j + 1] + v29))
                        + (float)(ck_[j] * *((float *)&list_gp[4 * j + 1] + j)))
                + (float)(ck_[LODWORD(temp1[1])] * *((float *)&list_gp[4 * j + 1] + LODWORD(temp1[1])));
            if ( v12 >= 0.0 )
            {
              LODWORD(v11) = COERCE_UNSIGNED_INT(
                               (float)((float)(ck_[v29] * *((float *)&list_gp[4 * v29 + 1] + v29))
                                     + (float)(ck_[j] * *((float *)&list_gp[4 * v29 + 1] + j)))
                             + (float)(ck_[LODWORD(temp1[1])] * *((float *)&list_gp[4 * v29 + 1] + LODWORD(temp1[1]))))
                           ^ _mask__NegFloat_;
              if ( v11 >= 0.0 )
              {
                ck_[v29] = (float)(*((float *)&list_gp[4 * v29 + 1] + j)
                                 * *((float *)&list_gp[4 * j + 1] + LODWORD(temp1[1])))
                         - (float)(*((float *)&list_gp[4 * j + 1] + j)
                                 * *((float *)&list_gp[4 * v29 + 1] + LODWORD(temp1[1])));
                ck_[j] = (float)(COERCE_FLOAT((unsigned int)*(&list_gp[4 * v29 + 1] + v29) ^ _mask__NegFloat_)
                               * *((float *)&list_gp[4 * j + 1] + LODWORD(temp1[1])))
                       + (float)(*((float *)&list_gp[4 * j + 1] + v29)
                               * *((float *)&list_gp[4 * v29 + 1] + LODWORD(temp1[1])));
                ck_[LODWORD(temp1[1])] = (float)(*((float *)&list_gp[4 * v29 + 1] + v29)
                                               * *((float *)&list_gp[4 * j + 1] + j))
                                       - (float)(*((float *)&list_gp[4 * j + 1] + v29)
                                               * *((float *)&list_gp[4 * v29 + 1] + j));
                v10 = (float)((float)(ck_[v29] * mat[3][v29 + 1]) + (float)(ck_[j] * mat[3][j + 1]))
                    + (float)(ck_[LODWORD(temp1[1])] * mat[3][LODWORD(temp1[1]) + 1]);
                if ( v10 >= 0.0 )
                {
                  v9 = (float)((float)(ck_[v29] * *((float *)&list_gp[4 * LODWORD(temp1[1]) + 1] + v29))
                             + (float)(ck_[j] * *((float *)&list_gp[4 * LODWORD(temp1[1]) + 1] + j)))
                     + (float)(ck_[LODWORD(temp1[1])]
                             * *((float *)&list_gp[4 * LODWORD(temp1[1]) + 1] + LODWORD(temp1[1])));
                  if ( v9 > 0.0000099999997 )
                  {
                    v8 = (float)((float)((float)((float)(v12 * v12) + (float)(v11 * v11)) + (float)(v10 * v10))
                               + (float)((float)((float)((float)((float)(v11 * *((float *)&list_gp[4 * v29 + 1] + j))
                                                               + (float)(v10
                                                                       * *((float *)&list_gp[4 * v29 + 1]
                                                                         + LODWORD(temp1[1]))))
                                                       * v12)
                                               + (float)((float)(v11 * v10)
                                                       * *((float *)&list_gp[4 * j + 1] + LODWORD(temp1[1]))))
                                       * 2.0))
                       / (float)(v9 * v9);
                    if ( v8 > v20[2] )
                    {
                      v20[v29 + 1] = v12 / v9;
                      v20[j + 1] = v11 / v9;
                      v20[LODWORD(temp1[1]) + 1] = v10 / v9;
                      v20[2] = v8;
                      LODWORD(v20[1]) = (1 << SLOBYTE(temp1[1])) | (1 << j) | (1 << v29);
                    }
                  }
                }
              }
            }
          }
          ++LODWORD(temp1[1]);
        }
      }
    }
    if ( *(float *)&iter > v20[2] )
    {
      support_gp = (geom_plane *)2;
      break;
    }
    iter = SLODWORD(v20[2]);
    v43 = v20[1];
    break_val = SLODWORD(point->x);
    cur_set = SLODWORD(point->y);
    cur_dist_sq = point->z;
    for ( m = 0; m < 4; ++m )
    {
      if ( (LODWORD(v43) & (1 << m)) != 0 )
      {
        v6 = v20[m + 1];
        v4 = v6 * *(float *)(v24[m] + 4);
        v5 = v6 * *(float *)(v24[m] + 8);
        *(float *)&break_val = *(float *)&break_val + (float)(v6 * *(float *)v24[m]);
        *(float *)&cur_set = *(float *)&cur_set + v4;
        cur_dist_sq = cur_dist_sq + v5;
      }
    }
  }
  result->x = *(float *)&break_val;
  result->y = *(float *)&cur_set;
  result->z = cur_dist_sq;
}

void __cdecl get_material_from_brush(const cbrush_t *brush, int *sflags)
{
  cplane_s *plane; // edx
  unsigned int j; // [esp+18h] [ebp-1Ch]
  float best_dot; // [esp+28h] [ebp-Ch]
  unsigned int i; // [esp+2Ch] [ebp-8h]
  unsigned int ia; // [esp+2Ch] [ebp-8h]
  unsigned int ib; // [esp+2Ch] [ebp-8h]

  if ( s_caulk_sflags == 0x7FFFFFFF )
  {
    s_caulk_sflags = 2147483646;
    for ( i = 0; i < cm.numMaterials; ++i )
    {
      if ( !strcmp(cm.materials[i].material, "caulk") )
      {
        s_caulk_sflags = cm.materials[i].surfaceFlags;
        break;
      }
    }
  }
  *sflags = brush->axial_sflags[1][2];
  if ( *sflags == s_caulk_sflags )
  {
    *sflags = 0x7FFFFFFF;
    best_dot = FLOAT_N10_0;
    for ( ia = 0; ia < brush->numsides; ++ia )
    {
      if ( brush->sides[ia].sflags != s_caulk_sflags )
      {
        plane = brush->sides[ia].plane;
        if ( (float)((float)((float)(plane->normal[0] * 0.0) + (float)(plane->normal[1] * 0.0))
                   + (float)(plane->normal[2] * 1.0)) > best_dot )
        {
          best_dot = (float)((float)(plane->normal[0] * 0.0) + (float)(plane->normal[1] * 0.0))
                   + (float)(plane->normal[2] * 1.0);
          *sflags = brush->sides[ia].sflags;
        }
      }
    }
    if ( *sflags == 0x7FFFFFFF )
    {
      for ( ib = 0; ib < 2; ++ib )
      {
        for ( j = 0; j < 3; ++j )
        {
          if ( brush->axial_sflags[ib][j] != s_caulk_sflags )
          {
            *sflags = brush->axial_sflags[ib][j];
            return;
          }
        }
      }
      *sflags = s_caulk_sflags;
    }
  }
}

void __cdecl fill_results(const gjk_trace_output_t *gto, bool is_walkable, trace_t *results)
{
  const cbrush_t *v3; // eax
  unsigned __int16 number; // [esp+4h] [ebp-10h]
  unsigned __int16 v5; // [esp+6h] [ebp-Eh]
  dmaterial_t *materialInfo; // [esp+8h] [ebp-Ch]

  results->fraction = gto->m_hit_time;
  results->allsolid = 0;
  results->startsolid = gto->m_hit_dist < 0.0;
  Phys_NitrousVecToVec3(&gto->m_hit_normal, results->normal.vec.v);
  results->walkable = is_walkable;
  if ( gto->m_gi->m_ent_info )
  {
    if ( gto->m_gi->m_ent_info->m_ent_type )
    {
      if ( gto->m_gi->m_ent_info->m_ent_type == ET_CENT )
      {
        number = gjk_entity_info_t::get_cent(gto->m_gi->m_ent_info)->nextState.number;
        if ( !results
          && !Assert_MyHandler(
                "c:\\projects_pc\\cod\\codsrc\\src\\physics\\../qcommon/cm_public.h",
                175,
                0,
                "%s",
                "trace") )
        {
          __debugbreak();
        }
        results->hitType = TRACE_HITTYPE_ENTITY;
        results->hitId = number;
      }
      else if ( gto->m_gi->m_ent_info->m_ent_type == ET_GLASS )
      {
        if ( !results
          && !Assert_MyHandler(
                "c:\\projects_pc\\cod\\codsrc\\src\\physics\\../qcommon/cm_public.h",
                175,
                0,
                "%s",
                "trace") )
        {
          __debugbreak();
        }
        results->hitType = TRACE_HITTYPE_ENTITY;
        results->hitId = 1022;
      }
      else
      {
        if ( gto->m_gi->m_ent_info->m_ent_type != ET_DENT
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
                1717,
                0,
                "%s",
                "gto.m_gi->m_ent_info->is_dent()") )
        {
          __debugbreak();
        }
        if ( !results
          && !Assert_MyHandler(
                "c:\\projects_pc\\cod\\codsrc\\src\\physics\\../qcommon/cm_public.h",
                175,
                0,
                "%s",
                "trace") )
        {
          __debugbreak();
        }
        results->hitType = TRACE_HITTYPE_ENTITY;
        results->hitId = 1022;
      }
    }
    else
    {
      v5 = gjk_entity_info_t::get_gent(gto->m_gi->m_ent_info)->s.number;
      if ( !results
        && !Assert_MyHandler(
              "c:\\projects_pc\\cod\\codsrc\\src\\physics\\../qcommon/cm_public.h",
              175,
              0,
              "%s",
              "trace") )
      {
        __debugbreak();
      }
      results->hitType = TRACE_HITTYPE_ENTITY;
      results->hitId = v5;
    }
  }
  else
  {
    if ( !results
      && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\physics\\../qcommon/cm_public.h", 175, 0, "%s", "trace") )
    {
      __debugbreak();
    }
    results->hitType = TRACE_HITTYPE_ENTITY;
    results->hitId = 1022;
  }
  if ( gto->m_gi->m_cg->get_brush(gto->m_gi->m_cg) )
  {
    v3 = gto->m_gi->m_cg->get_brush(gto->m_gi->m_cg);
    results->cflags = v3->contents;
    get_material_from_brush(v3, &results->sflags);
  }
  else if ( gto->m_gi->m_cg->get_type(gto->m_gi->m_cg) == 3 )
  {
    materialInfo = &cm.materials[*(unsigned __int16 *)(LODWORD(gto->m_gi->m_cg[1].m_aabb_mn_loc.x) + 12)];
    results->sflags = materialInfo->surfaceFlags;
    results->cflags = materialInfo->contentFlags;
  }
  else
  {
    results->cflags = 0;
    results->sflags = 0;
  }
  results->modelIndex = 0;
  results->partName = 0;
  results->partGroup = 0;
  results->boneIndex = 254;
  results->staticModel = 0;
}

void __cdecl fill_results_type_and_id(const gjk_trace_output_t *gto, trace_t *results)
{
  unsigned __int16 index; // [esp+2h] [ebp-6h]
  unsigned __int16 number; // [esp+4h] [ebp-4h]
  unsigned __int16 v4; // [esp+6h] [ebp-2h]

  if ( gto->m_gi->m_ent_info )
  {
    if ( gto->m_gi->m_ent_info->m_ent_type )
    {
      if ( gto->m_gi->m_ent_info->m_ent_type == ET_CENT )
      {
        number = gjk_entity_info_t::get_cent(gto->m_gi->m_ent_info)->nextState.number;
        if ( !results
          && !Assert_MyHandler(
                "c:\\projects_pc\\cod\\codsrc\\src\\physics\\../qcommon/cm_public.h",
                175,
                0,
                "%s",
                "trace") )
        {
          __debugbreak();
        }
        results->hitType = TRACE_HITTYPE_ENTITY;
        results->hitId = number;
      }
      else if ( gto->m_gi->m_ent_info->m_ent_type == ET_GLASS )
      {
        index = gjk_entity_info_t::get_glass(gto->m_gi->m_ent_info)->index;
        if ( !results
          && !Assert_MyHandler(
                "c:\\projects_pc\\cod\\codsrc\\src\\physics\\../qcommon/cm_public.h",
                175,
                0,
                "%s",
                "trace") )
        {
          __debugbreak();
        }
        results->hitType = TRACE_HITTYPE_GLASS;
        results->hitId = index;
      }
      else
      {
        if ( gto->m_gi->m_ent_info->m_ent_type != ET_DENT
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.cpp",
                1765,
                0,
                "%s",
                "gto.m_gi->m_ent_info->is_dent()") )
        {
          __debugbreak();
        }
        if ( !results
          && !Assert_MyHandler(
                "c:\\projects_pc\\cod\\codsrc\\src\\physics\\../qcommon/cm_public.h",
                175,
                0,
                "%s",
                "trace") )
        {
          __debugbreak();
        }
        results->hitType = TRACE_HITTYPE_ENTITY;
        results->hitId = 1022;
      }
    }
    else
    {
      v4 = gjk_entity_info_t::get_gent(gto->m_gi->m_ent_info)->s.number;
      if ( !results
        && !Assert_MyHandler(
              "c:\\projects_pc\\cod\\codsrc\\src\\physics\\../qcommon/cm_public.h",
              175,
              0,
              "%s",
              "trace") )
      {
        __debugbreak();
      }
      results->hitType = TRACE_HITTYPE_ENTITY;
      results->hitId = v4;
    }
  }
  else
  {
    if ( !results
      && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\physics\\../qcommon/cm_public.h", 175, 0, "%s", "trace") )
    {
      __debugbreak();
    }
    results->hitType = TRACE_HITTYPE_ENTITY;
    results->hitId = 1022;
  }
}

const Glass *__thiscall gjk_entity_info_t::get_glass(gjk_entity_info_t *this)
{
  if ( (this->m_ent_type != ET_GLASS || !this->m_ent)
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_gjk_collision_detection.h",
          89,
          0,
          "%s",
          "m_ent_type == ET_GLASS && m_ent != NULL") )
  {
    __debugbreak();
  }
  return (const Glass *)this->m_ent;
}

void __cdecl fill_results_no_hit(trace_t *results)
{
  results->fraction = FLOAT_1_0;
  results->allsolid = 0;
  results->startsolid = 0;
  results->normal.vec.u[0] = *(unsigned int *)&FLOAT_0_0;
  results->normal.vec.u[1] = *(unsigned int *)&FLOAT_0_0;
  results->normal.vec.u[2] = *(unsigned int *)&FLOAT_0_0;
  results->walkable = 0;
  if ( !results
    && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\physics\\../qcommon/cm_public.h", 175, 0, "%s", "trace") )
  {
    __debugbreak();
  }
  results->hitType = TRACE_HITTYPE_NONE;
  results->hitId = 0;
  results->cflags = 0;
  results->sflags = 0;
  results->modelIndex = 0;
  results->partName = 0;
  results->partGroup = 0;
  results->boneIndex = 254;
  results->staticModel = 0;
}

bool  is_walkable@<al>(int a1@<ebp>, const gjk_trace_output_t *gto)
{
  float v3; // [esp-Ch] [ebp-7Ch] BYREF
  float v4; // [esp-8h] [ebp-78h]
  float v5; // [esp-4h] [ebp-74h]
  const phys_vec3 *v6; // [esp+10h] [ebp-60h]
  phys_vec3 v7; // [esp+14h] [ebp-5Ch] BYREF
  float x; // [esp+24h] [ebp-4Ch] BYREF
  float y; // [esp+28h] [ebp-48h]
  float v10; // [esp+2Ch] [ebp-44h]
  phys_vec3 *v11; // [esp+40h] [ebp-30h]
  phys_vec3 v12; // [esp+44h] [ebp-2Ch] BYREF
  gjk_entity_info_t *m_ent_info; // [esp+5Ch] [ebp-14h]
  float z; // [esp+60h] [ebp-10h]
  int v15; // [esp+64h] [ebp-Ch] BYREF
  const phys_mat44 *mat; // [esp+68h] [ebp-8h]
  const phys_mat44 *retaddr; // [esp+70h] [ebp+0h]

  v15 = a1;
  mat = retaddr;
  if ( !gto->m_is_foot )
    return 0;
  z = gto->m_hit_normal.z;
  if ( z >= 0.69999999 )
    return 1;
  if ( gto->m_gi->m_ent_info )
  {
    m_ent_info = gto->m_gi->m_ent_info;
    v11 = phys_full_inv_multiply((int)&v15, &v12, &m_ent_info->m_mat, &gto->m_hit_point);
    x = v11->x;
    y = v11->y;
    v10 = v11->z;
    v6 = phys_inv_multiply(&v7, &m_ent_info->m_mat, &PHYS_Z_VEC_62);
    v3 = v6->x;
    v4 = v6->y;
    v5 = v6->z;
  }
  else
  {
    x = gto->m_hit_point.x;
    y = gto->m_hit_point.y;
    v10 = gto->m_hit_point.z;
    v3 = PHYS_Z_VEC_62.x;
    v4 = PHYS_Z_VEC_62.y;
    v5 = PHYS_Z_VEC_62.z;
  }
  return gto->m_gi->m_cg->is_walkable(gto->m_gi->m_cg, (const phys_vec3 *)&x, (const phys_vec3 *)&v3);
}

phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *__thiscall phys_simple_allocator<phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal>::allocate(
        phys_simple_allocator<phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal> *this)
{
  char *slot; // [esp+30h] [ebp-4h]

  slot = PMM_ALLOC(0x90u, 0x10u);
  if ( !slot )
    return 0;
  ++this->m_count;
  phys_gjk_cache_info::phys_gjk_cache_info((phys_gjk_cache_info *)slot);
  return (phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *)slot;
}

phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *__thiscall phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::find(
        phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor> *this,
        const phys_gjk_geom_id_pair_key *key)
{
  bool v3; // [esp+0h] [ebp-18h]
  phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *cur; // [esp+14h] [ebp-4h]

  cur = this->m_tree_root;
  while ( cur )
  {
    if ( key->m_id1 == cur->m_key.m_id1 && key->m_id2 == cur->m_key.m_id2 )
      break;
    if ( key->m_id1 == cur->m_key.m_id1 )
      v3 = key->m_id2 < cur->m_key.m_id2;
    else
      v3 = key->m_id1 < cur->m_key.m_id1;
    if ( v3 )
      cur = cur->m_avl_tree_node.m_left;
    else
      cur = cur->m_avl_tree_node.m_right;
  }
  return cur;
}

void __thiscall phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::add(
        phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor> *this,
        const phys_gjk_geom_id_pair_key *key,
        phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *data)
{
  unsigned int m_id2; // edx
  bool v4; // [esp+4h] [ebp-180h]
  bool v5; // [esp+8h] [ebp-17Ch]
  phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal **m_node; // [esp+74h] [ebp-110h]
  phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *root; // [esp+78h] [ebp-10Ch]
  phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::stack_item *next_item; // [esp+7Ch] [ebp-108h]
  phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::stack_item *cur_item; // [esp+80h] [ebp-104h]
  phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::stack_item the_stack[32]; // [esp+84h] [ebp-100h] BYREF

  cur_item = the_stack;
  the_stack[0].m_node = &this->m_tree_root;
  while ( *cur_item->m_node )
  {
    root = *cur_item->m_node;
    if ( &cur_item[1] - the_stack >= 32
      && _tlAssert(
           "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_avl_tree.h",
           103,
           "cur_item + 1 - the_stack < 32",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    next_item = cur_item + 1;
    if ( key->m_id1 == root->m_key.m_id1 )
      v5 = key->m_id2 < root->m_key.m_id2;
    else
      v5 = key->m_id1 < root->m_key.m_id1;
    if ( v5 )
    {
      cur_item->m_child = -1;
      next_item->m_node = &root->m_avl_tree_node.m_left;
    }
    else
    {
      if ( key->m_id1 == root->m_key.m_id1 )
        v4 = root->m_key.m_id2 < key->m_id2;
      else
        v4 = root->m_key.m_id1 < key->m_id1;
      if ( !v4 )
      {
        if ( _tlAssert(
               "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_avl_tree.h",
               112,
               "key > accessor::get_avl_key(root)",
               &toastPopupTitle) )
        {
          __debugbreak();
        }
      }
      cur_item->m_child = 1;
      next_item->m_node = &root->m_avl_tree_node.m_right;
    }
    cur_item = next_item;
  }
  *cur_item->m_node = data;
  data->m_avl_tree_node.m_left = 0;
  data->m_avl_tree_node.m_right = 0;
  data->m_avl_tree_node.m_balance = 0;
  m_id2 = key->m_id2;
  data->m_key.m_id1 = key->m_id1;
  data->m_key.m_id2 = m_id2;
  do
  {
    if ( cur_item <= the_stack )
      break;
    --cur_item;
    m_node = cur_item->m_node;
    (*cur_item->m_node)->m_avl_tree_node.m_balance += cur_item->m_child;
    if ( (*m_node)->m_avl_tree_node.m_balance == -2 )
    {
      if ( (*m_node)->m_avl_tree_node.m_left->m_avl_tree_node.m_balance != -1
        && (*m_node)->m_avl_tree_node.m_left->m_avl_tree_node.m_balance != 1
        && _tlAssert(
             "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_avl_tree.h",
             130,
             "accessor::get_avl_node(accessor::get_avl_node(root)->m_left)->m_balance == -1 || accessor::get_avl_node(acc"
             "essor::get_avl_node(root)->m_left)->m_balance == 1",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
      if ( (*m_node)->m_avl_tree_node.m_left->m_avl_tree_node.m_balance == 1 )
        phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::rotate_left(
          this,
          &(*m_node)->m_avl_tree_node.m_left);
      phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::rotate_right(
        this,
        m_node);
      if ( (*m_node)->m_avl_tree_node.m_balance
        && _tlAssert(
             "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_avl_tree.h",
             134,
             "accessor::get_avl_node(root)->m_balance == 0",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
    }
    else if ( (*m_node)->m_avl_tree_node.m_balance == 2 )
    {
      if ( (*m_node)->m_avl_tree_node.m_right->m_avl_tree_node.m_balance != -1
        && (*m_node)->m_avl_tree_node.m_right->m_avl_tree_node.m_balance != 1
        && _tlAssert(
             "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_avl_tree.h",
             138,
             "accessor::get_avl_node(accessor::get_avl_node(root)->m_right)->m_balance == -1 || accessor::get_avl_node(ac"
             "cessor::get_avl_node(root)->m_right)->m_balance == 1",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
      if ( (*m_node)->m_avl_tree_node.m_right->m_avl_tree_node.m_balance == -1 )
        phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::rotate_right(
          this,
          &(*m_node)->m_avl_tree_node.m_right);
      phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::rotate_left(
        this,
        m_node);
      if ( (*m_node)->m_avl_tree_node.m_balance
        && _tlAssert(
             "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_avl_tree.h",
             142,
             "accessor::get_avl_node(root)->m_balance == 0",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
    }
  }
  while ( (*m_node)->m_avl_tree_node.m_balance );
}

phys_gjk_cache_info *__thiscall phys_gjk_cache_info::phys_gjk_cache_info(phys_gjk_cache_info *this)
{
  int v2; // [esp+8h] [ebp-14h]
  cached_simplex_info *j; // [esp+Ch] [ebp-10h]
  int v4; // [esp+14h] [ebp-8h]
  cached_simplex_info *i; // [esp+18h] [ebp-4h]

  v4 = 3;
  for ( i = &this->m_support_a; --v4 >= 0; i = (cached_simplex_info *)((char *)i + 16) )
    ;
  v2 = 3;
  for ( j = &this->m_support_b; --v2 >= 0; j = (cached_simplex_info *)((char *)j + 16) )
    ;
  return this;
}

