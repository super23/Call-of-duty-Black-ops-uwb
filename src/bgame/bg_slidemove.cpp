#include "bg_slidemove.h"

void __cdecl Phys_Vec3ToNitrousVec(float *inVector, phys_vec3 *outVector)
{
  outVector->x = *inVector;
  outVector->y = inVector[1];
  outVector->z = inVector[2];
}

gjk_polygon_cylinder_t *__cdecl create_gjkcc_gjk_geom(
        float (*mins)[3],
        float (*maxs)[3],
        int stype,
        gjk_collision_visitor *allocator)
{
  gjk_polygon_cylinder_t *gjk_geom; // [esp+Ch] [ebp-4h]
  int savedregs; // [esp+10h] [ebp+0h] BYREF

  gjk_geom = gjk_polygon_cylinder_t::create(mins, maxs, 0.5, stype, allocator);
  setup_gjk_polygon_cylinder(COERCE_FLOAT(&savedregs), (float *)mins, (float *)maxs, 0.5, gjk_geom);
  return gjk_geom;
}

gjkcc_info *__cdecl create_gjkcc_info(unsigned int gjkcc_id, bool is_server_thread, const float *origin)
{
  gjkcc_info *gcci; // [esp+58h] [ebp-8h]

  gcci = phys_simple_allocator<gjkcc_info>::allocate(&g_gjkcc_info_allocator);
  if ( gjkcc_id )
    gcci->m_gjk_cache.m_max_num_gjk_ci = 256;
  else
    gcci->m_gjk_cache.m_max_num_gjk_ci = 0;
  gcci->m_gjk_cache.m_list_head = 0;
  gcci->m_active = 0;
  gcci->m_is_server_thread = is_server_thread;
  phys_mat44::operator=(&gcci->m_cg_to_world_xform, &PHYS_IDENTITY_MATRIX_1);
  gcci->m_mins[0] = FLOAT_N15_0;
  gcci->m_mins[1] = FLOAT_N15_0;
  gcci->m_mins[2] = *(float *)&FLOAT_0_0;
  gcci->m_maxs[0] = FLOAT_15_0;
  gcci->m_maxs[1] = FLOAT_15_0;
  gcci->m_maxs[2] = FLOAT_70_0;
  gcci->m_cg_ = gjk_polygon_cylinder_t::create(
                  (float (*)[3])gcci->m_mins,
                  (float (*)[3])gcci->m_maxs,
                  0.5,
                  0,
                  &g_empty_collision_visitor);
  gjkcc_info::update_cg(gcci, gcci->m_mins, gcci->m_maxs, 1);
  gcci->m_last_origin[0] = *origin;
  gcci->m_last_origin[1] = origin[1];
  gcci->m_last_origin[2] = origin[2];
  return gcci;
}

phys_mat44 *__thiscall phys_mat44::operator=(phys_mat44 *this, const phys_mat44 *__that)
{
  this->x.x = __that->x.x;
  this->x.y = __that->x.y;
  this->x.z = __that->x.z;
  this->y.x = __that->y.x;
  this->y.y = __that->y.y;
  this->y.z = __that->y.z;
  this->z.x = __that->z.x;
  this->z.y = __that->z.y;
  this->z.z = __that->z.z;
  this->w.x = __that->w.x;
  this->w.y = __that->w.y;
  this->w.z = __that->w.z;
  return this;
}

void __thiscall gjkcc_info::update_cg(gjkcc_info *this, float *mins, float *maxs, bool force)
{
  int savedregs; // [esp+1Ch] [ebp+0h] BYREF

  if ( *mins != this->m_mins[0]
    || mins[1] != this->m_mins[1]
    || mins[2] != this->m_mins[2]
    || *maxs != this->m_maxs[0]
    || maxs[1] != this->m_maxs[1]
    || maxs[2] != this->m_maxs[2]
    || force )
  {
    this->m_mins[0] = *mins;
    this->m_mins[1] = mins[1];
    this->m_mins[2] = mins[2];
    this->m_maxs[0] = *maxs;
    this->m_maxs[1] = maxs[1];
    this->m_maxs[2] = maxs[2];
    if ( this->m_cg_->get_type(this->m_cg_) == 4 )
    {
      setup_gjk_capsule(COERCE_FLOAT(&savedregs), mins, maxs, 0.5, (gjk_double_sphere_t *)this->m_cg_);
    }
    else if ( this->m_cg_->get_type(this->m_cg_) == 7 )
    {
      setup_gjk_polygon_cylinder(COERCE_FLOAT(&savedregs), mins, maxs, 0.5, (gjk_polygon_cylinder_t *)this->m_cg_);
    }
    else if ( this->m_cg_->get_type(this->m_cg_) == 5 )
    {
      setup_gjk_cylinder(COERCE_FLOAT(&savedregs), mins, maxs, 0.5, (gjk_cylinder_t *)this->m_cg_);
    }
    this->m_cg_->calc_aabb(this->m_cg_, &PHYS_IDENTITY_MATRIX_1, &this->m_cg_aabb_min, &this->m_cg_aabb_max);
    this->m_cg_aabb_min.x = this->m_cg_aabb_min.x + PT_AC_EPS_VEC.x;
    this->m_cg_aabb_min.y = this->m_cg_aabb_min.y + PT_AC_EPS_VEC.y;
    this->m_cg_aabb_min.z = this->m_cg_aabb_min.z + PT_AC_EPS_VEC.z;
    this->m_cg_aabb_max.x = this->m_cg_aabb_max.x - PT_AC_EPS_VEC.x;
    this->m_cg_aabb_max.y = this->m_cg_aabb_max.y - PT_AC_EPS_VEC.y;
    this->m_cg_aabb_max.z = this->m_cg_aabb_max.z - PT_AC_EPS_VEC.z;
  }
}

void __cdecl destroy_gjkcc_info(char *gcci_)
{
  if ( !gcci_ && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp", 108, 0, "%s", "gcci_") )
    __debugbreak();
  if ( _InterlockedCompareExchange((volatile signed __int32 *)gcci_ + 120, 1, 1) == 1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp",
          110,
          0,
          "%s",
          "gcci->IsLocked() == false") )
  {
    __debugbreak();
  }
  gjk_polygon_cylinder_t::destroy(*((gjk_polygon_cylinder_t **)gcci_ + 122));
  gjk_query_output::verify_empty((gjk_query_output *)(gcci_ + 144));
  phys_heap_gjk_cache_system_avl_tree::shutdown((phys_heap_gjk_cache_system_avl_tree *)(gcci_ + 120));
  phys_simple_allocator<gjkcc_info>::free(&g_gjkcc_info_allocator, (gjkcc_info *)gcci_);
}

void __thiscall phys_heap_gjk_cache_system_avl_tree::shutdown(phys_heap_gjk_cache_system_avl_tree *this)
{
  phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *next; // [esp+154h] [ebp-4h]

  while ( this->m_list_head )
  {
    next = this->m_list_head->m_next_gjk_ci;
    phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::remove(
      &this->m_search_tree,
      &this->m_list_head->m_key);
    phys_simple_allocator<phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal>::free(
      &this->m_list_phys_gjk_cache_info_internal,
      this->m_list_head);
    this->m_list_head = next;
  }
  if ( this->m_list_phys_gjk_cache_info_internal.m_count )
  {
    if ( _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_gjk_cache_system.h",
           260,
           "m_list_phys_gjk_cache_info_internal.get_count() == 0",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
  }
}

void __thiscall gjk_query_output::verify_empty(gjk_query_output *this)
{
  if ( this->m_ent_count
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../physics/phys_gjk_collision_detection.h",
          333,
          0,
          "%s",
          "m_ent_count == 0") )
  {
    __debugbreak();
  }
  if ( this->m_geom_count
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../physics/phys_gjk_collision_detection.h",
          334,
          0,
          "%s",
          "m_geom_count == 0") )
  {
    __debugbreak();
  }
  if ( this->m_query_visitor_count
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../physics/phys_gjk_collision_detection.h",
          335,
          0,
          "%s",
          "m_query_visitor_count == 0") )
  {
    __debugbreak();
  }
  if ( this->m_gent_query_visitor_count
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../physics/phys_gjk_collision_detection.h",
          336,
          0,
          "%s",
          "m_gent_query_visitor_count == 0") )
  {
    __debugbreak();
  }
  if ( this->m_cent_query_visitor_count
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../physics/phys_gjk_collision_detection.h",
          337,
          0,
          "%s",
          "m_cent_query_visitor_count == 0") )
  {
    __debugbreak();
  }
  if ( this->m_dent_query_visitor_count
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../physics/phys_gjk_collision_detection.h",
          338,
          0,
          "%s",
          "m_dent_query_visitor_count == 0") )
  {
    __debugbreak();
  }
  if ( this->m_list_geom_info.m_first
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../physics/phys_gjk_collision_detection.h",
          339,
          0,
          "%s",
          "m_list_geom_info.get_first() == NULL") )
  {
    __debugbreak();
  }
  if ( this->m_allocator.m_cur
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../physics/phys_gjk_collision_detection.h",
          340,
          0,
          "%s",
          "m_allocator.is_empty()") )
  {
    __debugbreak();
  }
  if ( this->m_total_list_geom_info
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../physics/phys_gjk_collision_detection.h",
          341,
          0,
          "%s",
          "m_total_list_geom_info == NULL") )
  {
    __debugbreak();
  }
  if ( !cached_query_info_t::is_empty(&this->m_cached_query_info)
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../physics/phys_gjk_collision_detection.h",
          343,
          0,
          "%s",
          "m_cached_query_info.is_empty()") )
  {
    __debugbreak();
  }
}

char __thiscall cached_query_info_t::is_empty(cached_query_info_t *this)
{
  if ( this->m_query_contents )
    return 0;
  if ( this->m_query_flags )
  {
    if ( !Assert_MyHandler(
            "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../physics/phys_gjk_collision_detection.h",
            212,
            0,
            "%s",
            "m_query_flags == 0") )
      __debugbreak();
  }
  return 1;
}

gjkcc_info *__thiscall gjkcc_info_database_t::gjkcc_info_find_or_create(
        gjkcc_info_database_t *this,
        unsigned int gjkcc_id,
        bool is_server_thread,
        const float *origin)
{
  generic_avl_map_node_t *m_tree_root; // [esp+8h] [ebp-14h]
  gjkcc_info *data; // [esp+10h] [ebp-Ch]
  generic_avl_map_node_t *gamn; // [esp+14h] [ebp-8h]
  gjkcc_info *gcci; // [esp+18h] [ebp-4h]

  gjkcc_info_database_t::verify_internal(this, is_server_thread);
  gjkcc_info_database_t::Lock(this);
  m_tree_root = this->m_map.m_tree_root;
  while ( m_tree_root && gjkcc_id != m_tree_root->m_avl_key )
  {
    if ( gjkcc_id >= m_tree_root->m_avl_key )
      m_tree_root = m_tree_root->m_avl_node_info.m_right;
    else
      m_tree_root = m_tree_root->m_avl_node_info.m_left;
  }
  gamn = m_tree_root;
  if ( !m_tree_root )
  {
    data = create_gjkcc_info(gjkcc_id, is_server_thread, origin);
    gamn = generic_avl_map_add(&this->m_map, data, gjkcc_id);
    ++this->m_gcci_count;
  }
  if ( !gamn && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp", 156, 0, "%s", "gamn") )
    __debugbreak();
  gcci = (gjkcc_info *)gamn->m_data;
  if ( !gamn->m_data
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp", 158, 0, "%s", "gcci") )
  {
    __debugbreak();
  }
  if ( _InterlockedCompareExchange(&gcci->m_active, 1, 1) == 1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp",
          159,
          0,
          "%s",
          "gcci->IsLocked() == false") )
  {
    __debugbreak();
  }
  if ( gcci->m_is_server_thread != is_server_thread
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp",
          160,
          0,
          "%s",
          "gcci->m_is_server_thread == is_server_thread") )
  {
    __debugbreak();
  }
  gjkcc_info_database_t::Unlock(this);
  return gcci;
}

void __thiscall gjkcc_info_database_t::Lock(gjkcc_info_database_t *this)
{
  if ( _InterlockedCompareExchange(&this->m_token, 1, 0) )
  {
    if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp", 135, 0, "%s", "retv") )
      __debugbreak();
  }
}

void __thiscall gjkcc_info_database_t::Unlock(gjkcc_info_database_t *this)
{
  if ( _InterlockedCompareExchange(&this->m_token, 0, 1) != 1
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp", 136, 0, "%s", "retv") )
  {
    __debugbreak();
  }
}

void __thiscall gjkcc_info_database_t::verify_internal(gjkcc_info_database_t *this, bool is_server_thread)
{
  if ( this->m_is_server_thread != is_server_thread
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp",
          137,
          0,
          "%s",
          "m_is_server_thread == is_server_thread") )
  {
    __debugbreak();
  }
}

gjkcc_info *__thiscall gjkcc_info_database_t::gjkcc_info_find(
        gjkcc_info_database_t *this,
        unsigned int gjkcc_id,
        bool is_server_thread)
{
  generic_avl_map_node_t *m_tree_root; // [esp+8h] [ebp-10h]
  gjkcc_info *gcci; // [esp+14h] [ebp-4h]

  gjkcc_info_database_t::verify_internal(this, is_server_thread);
  gjkcc_info_database_t::Lock(this);
  m_tree_root = this->m_map.m_tree_root;
  while ( m_tree_root && gjkcc_id != m_tree_root->m_avl_key )
  {
    if ( gjkcc_id >= m_tree_root->m_avl_key )
      m_tree_root = m_tree_root->m_avl_node_info.m_right;
    else
      m_tree_root = m_tree_root->m_avl_node_info.m_left;
  }
  if ( !m_tree_root
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp", 173, 0, "%s", "gamn") )
  {
    __debugbreak();
  }
  gcci = (gjkcc_info *)m_tree_root->m_data;
  if ( !m_tree_root->m_data
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp", 175, 0, "%s", "gcci") )
  {
    __debugbreak();
  }
  if ( _InterlockedCompareExchange(&gcci->m_active, 1, 1) != 1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp",
          176,
          0,
          "%s",
          "gcci->IsLocked() == true") )
  {
    __debugbreak();
  }
  if ( gcci->m_is_server_thread != is_server_thread
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp",
          177,
          0,
          "%s",
          "gcci->m_is_server_thread == is_server_thread") )
  {
    __debugbreak();
  }
  gjkcc_info_database_t::Unlock(this);
  return gcci;
}

void __thiscall gjkcc_info_database_t::gjkcc_info_destroy(
        gjkcc_info_database_t *this,
        unsigned int gjkcc_id,
        bool is_server_thread)
{
  char *gcci_; // [esp+8h] [ebp-4h]

  gjkcc_info_database_t::verify_internal(this, is_server_thread);
  gjkcc_info_database_t::Lock(this);
  gcci_ = (char *)generic_avl_map_destroy(&this->m_map, gjkcc_id);
  if ( gcci_ )
  {
    destroy_gjkcc_info(gcci_);
    --this->m_gcci_count;
  }
  gjkcc_info_database_t::Unlock(this);
}

void __cdecl destroy_client_gjkcc_info(int localClientNum)
{
  if ( localClientNum
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp",
          220,
          0,
          "%s",
          "localClientNum >= 0 && localClientNum < MAX_LOCAL_CLIENTS") )
  {
    __debugbreak();
  }
  gjkcc_info_database_t::gjkcc_info_destroy(&g_gjkcc_info_client_database, (unsigned int)&cg_pmove[localClientNum], 0);
}

void  gjkcc_prolog(int a1@<ebp>, const gjkcc_input_t *gjkcc_in, float *origin)
{
  const phys_vec3 *v3; // eax
  phys_vec3 v4; // [esp-20h] [ebp-2Ch] BYREF
  gjkcc_info *v5; // [esp-4h] [ebp-10h]
  unsigned int v6[2]; // [esp+0h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]

  v6[0] = a1;
  v6[1] = retaddr;
  if ( gjkcc_in->is_server_thread )
    v5 = gjkcc_info_database_t::gjkcc_info_find_or_create(&g_gjkcc_info_server_database, gjkcc_in->gjkcc_id, 1, origin);
  else
    v5 = gjkcc_info_database_t::gjkcc_info_find_or_create(&g_gjkcc_info_client_database, gjkcc_in->gjkcc_id, 0, origin);
  gjkcc_info::Lock(v5);
  v3 = Phys_Vec3ToNitrousVec(&v4, origin);
  gjk_query_output::cached_query_prolog(
    &v5->m_gjk_query_output,
    (int)v6,
    gjkcc_in->is_server_thread,
    gjkcc_in->proximity_data,
    gjkcc_in->proximity_mask,
    v3);
}

const phys_vec3 *__cdecl Phys_Vec3ToNitrousVec(const phys_vec3 *result, float *inVector)
{
  float v3; // [esp-8h] [ebp-18h]
  float v4; // [esp-4h] [ebp-14h]
  float outVector; // [esp+0h] [ebp-10h]

  v3 = inVector[1];
  v4 = inVector[2];
  result->x = *inVector;
  result->y = v3;
  result->z = v4;
  result->w = outVector;
  return result;
}

void __thiscall gjkcc_info::Lock(gjkcc_info *this)
{
  if ( _InterlockedCompareExchange(&this->m_active, 1, 0) )
  {
    if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp", 78, 0, "%s", "retv") )
      __debugbreak();
  }
}

void __cdecl gjkcc_epilog(const gjkcc_input_t *gjkcc_in, const float *origin)
{
  gjkcc_info *gcci; // [esp+20h] [ebp-4h]

  gcci = find_gjkcc_info(gjkcc_in->gjkcc_id, gjkcc_in->is_server_thread);
  phys_heap_gjk_cache_system_avl_tree::update_cache(&gcci->m_gjk_cache);
  gjk_query_output::cached_query_epilog(&gcci->m_gjk_query_output);
  gcci->m_last_origin[0] = *origin;
  gcci->m_last_origin[1] = origin[1];
  gcci->m_last_origin[2] = origin[2];
  gjkcc_info::Unlock(gcci);
}

void __thiscall phys_heap_gjk_cache_system_avl_tree::update_cache(phys_heap_gjk_cache_system_avl_tree *this)
{
  phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal **gjk_ci_i; // [esp+158h] [ebp-8h]
  phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *gjk_ci; // [esp+15Ch] [ebp-4h]

  gjk_ci_i = &this->m_list_head;
  for ( gjk_ci = this->m_list_head; gjk_ci; gjk_ci = *gjk_ci_i )
  {
    if ( (gjk_ci->m_flags & 1) != 0 )
    {
      gjk_ci_i = &gjk_ci->m_next_gjk_ci;
      gjk_ci->m_flags &= ~1u;
    }
    else
    {
      *gjk_ci_i = gjk_ci->m_next_gjk_ci;
      phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::remove(
        &this->m_search_tree,
        &gjk_ci->m_key);
      phys_simple_allocator<phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal>::free(
        &this->m_list_phys_gjk_cache_info_internal,
        gjk_ci);
    }
  }
}

void __thiscall gjkcc_info::Unlock(gjkcc_info *this)
{
  if ( _InterlockedCompareExchange(&this->m_active, 0, 1) != 1
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp", 79, 0, "%s", "retv") )
  {
    __debugbreak();
  }
}

gjkcc_info *__cdecl find_gjkcc_info(unsigned int gjkcc_id, bool is_server_thread)
{
  if ( is_server_thread )
    return gjkcc_info_database_t::gjkcc_info_find(&g_gjkcc_info_server_database, gjkcc_id, 1);
  else
    return gjkcc_info_database_t::gjkcc_info_find(&g_gjkcc_info_client_database, gjkcc_id, 0);
}

bool __thiscall gjk_double_sphere_t::is_foot(gjk_double_sphere_t *this, const phys_vec3 *hit_point)
{
  return (float)(this->m_list_center[1].z - 1.0) >= hit_point->z;
}

bool __userpurge gjk_cylinder_t::is_foot@<al>(gjk_cylinder_t *this@<ecx>, int a2@<ebp>, const phys_vec3 *hit_point)
{
  unsigned int direction; // [esp-10h] [ebp-30h]
  unsigned int v5[3]; // [esp-Ch] [ebp-2Ch] BYREF
  gjk_cylinder_t *v6; // [esp+10h] [ebp-10h]
  unsigned int v7[2]; // [esp+14h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+20h] [ebp+0h]

  v7[0] = a2;
  v7[1] = retaddr;
  v6 = this;
  phys_full_inv_multiply((int)v7, (phys_vec3 *)v5, &this->xform, hit_point);
  direction = v6->direction;
  if ( direction > 2
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
         32,
         "i >= 0 && i < 3",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  return COERCE_FLOAT(LODWORD(v6->halfHeight) ^ _mask__NegFloat_) >= *(float *)&v5[direction];
}

phys_vec3 * phys_full_inv_multiply@<eax>(
        int a1@<ebp>,
        phys_vec3 *result,
        const phys_mat44 *mat,
        const phys_vec3 *v)
{
  phys_vec3 v5; // [esp-20h] [ebp-2Ch] BYREF
  float v6; // [esp-10h] [ebp-1Ch]
  float v7; // [esp-Ch] [ebp-18h]
  float v8; // [esp-8h] [ebp-14h]
  phys_vec3 *p_w; // [esp-4h] [ebp-10h]
  int v10; // [esp+0h] [ebp-Ch]
  void *v11; // [esp+4h] [ebp-8h]
  void *retaddr; // [esp+Ch] [ebp+0h]

  v10 = a1;
  v11 = retaddr;
  p_w = &mat->w;
  v8 = v->x - mat->w.x;
  v7 = v->y - mat->w.y;
  v6 = v->z - mat->w.z;
  v5.x = v8;
  v5.y = v7;
  v5.z = v6;
  phys_inv_multiply(result, mat, &v5);
  return result;
}

const phys_vec3 *__cdecl phys_inv_multiply(const phys_vec3 *result, const phys_mat44 *mat, const phys_vec3 *v)
{
  float v4; // [esp+4h] [ebp-10h]
  float v5; // [esp+Ch] [ebp-8h]

  v5 = (float)((float)(v->x * mat->z.x) + (float)(v->y * mat->z.y)) + (float)(v->z * mat->z.z);
  v4 = (float)((float)(v->x * mat->y.x) + (float)(v->y * mat->y.y)) + (float)(v->z * mat->y.z);
  result->x = (float)((float)(v->x * mat->x.x) + (float)(v->y * mat->x.y)) + (float)(v->z * mat->x.z);
  result->y = v4;
  result->z = v5;
  return result;
}

bool __thiscall gjk_polygon_cylinder_t::is_foot(gjk_polygon_cylinder_t *this, const phys_vec3 *hit_point)
{
  long double thisa; // [esp+0h] [ebp-1Ch]

  LODWORD(thisa) = this;
  if ( this->m_mode )
    return (float)((float)((float)(this->m_center.z - this->m_half_height) + this->m_capsule_radius) - 1.0) >= hit_point->z;
  if ( (_S2 & 1) == 0 )
  {
    _S2 |= 1u;
    __libm_sse2_sin(thisa);
    offset_0 = *(float *)(LODWORD(thisa) + 108) * (float)((float)(30.0 * 3.1415927) / 180.0);
  }
  return (float)((float)((float)(*(float *)(LODWORD(thisa) + 88) - *(float *)(LODWORD(thisa) + 104))
                       + *(float *)(LODWORD(thisa) + 116))
               - offset_0) >= hit_point->z;
}

bool __thiscall gjk_brush_t::is_walkable(gjk_brush_t *this, const phys_vec3 *hit_point, const phys_vec3 *up)
{
  int savedregs; // [esp+4h] [ebp+0h] BYREF

  return is_walkable((unsigned int)&savedregs, this->brush, hit_point, up);
}

bool __thiscall gjk_partition_t::is_walkable(gjk_partition_t *this, const phys_vec3 *hit_point, const phys_vec3 *up)
{
  int savedregs; // [esp+4h] [ebp+0h] BYREF

  return is_walkable((int)&savedregs, this->partition, hit_point, up);
}

void __cdecl calc_time(phys_gjk_info *gjk_info, bool keep_all_collisions, float *time, float *dist)
{
  float v4; // [esp+20h] [ebp-18h]

  v4 = (float)((float)(gjk_info->m_cg1_relative_translation_loc.x * gjk_info->cg1_cinfo_loc.m_n.x)
             + (float)(gjk_info->m_cg1_relative_translation_loc.y * gjk_info->cg1_cinfo_loc.m_n.y))
     + (float)(gjk_info->m_cg1_relative_translation_loc.z * gjk_info->cg1_cinfo_loc.m_n.z);
  *dist = (float)((float)(gjk_info->m_continuous_collision_lambda * v4)
                + (float)((float)((float)((float)(gjk_info->cg1_cinfo_loc.m_p1.x - gjk_info->cg1_cinfo_loc.m_p2.x)
                                        * gjk_info->cg1_cinfo_loc.m_n.x)
                                + (float)((float)(gjk_info->cg1_cinfo_loc.m_p1.y - gjk_info->cg1_cinfo_loc.m_p2.y)
                                        * gjk_info->cg1_cinfo_loc.m_n.y))
                        + (float)((float)(gjk_info->cg1_cinfo_loc.m_p1.z - gjk_info->cg1_cinfo_loc.m_p2.z)
                                * gjk_info->cg1_cinfo_loc.m_n.z)))
        + 0.125;
  if ( v4 >= 0.0 )
  {
    if ( *dist <= (float)(0.5 * 0.125) || keep_all_collisions )
      *time = gjk_info->m_continuous_collision_lambda;
    else
      *time = FLOAT_1_0;
  }
  else
  {
    *time = gjk_info->m_continuous_collision_lambda;
  }
}

void __cdecl POLYGON_CYLINDER_HACK(phys_gjk_input *pgi, const gjk_base_t *cg1, const gjk_base_t *cg2)
{
  if ( cg1->get_type(cg1) == 7 )
  {
    if ( cg2->get_brush(cg2) )
      cg1[1].m_aabb_mx_loc.z = 0.0;
    else
      LODWORD(cg1[1].m_aabb_mx_loc.z) = 1;
    pgi->cg1_radius = cg1->get_geom_radius(cg1);
  }
}

// local variable allocation has failed, the output may be wrong!
char  gjk_collide@<al>(
        float a1@<ebp>,
        phys_gjk_info *gjk_info,
        phys_gjk_input *pgi,
        gjk_trace_output_t *gto,
        const gjk_trace_input_t *gti,
        gjk_geom_info_t *gi)
{
  char v7[12]; // [esp+18h] [ebp-154h] BYREF
  char buf[256]; // [esp+24h] [ebp-148h]
  float v9; // [esp+124h] [ebp-48h]
  float v10; // [esp+128h] [ebp-44h]
  float v11; // [esp+134h] [ebp-38h]
  float v12; // [esp+138h] [ebp-34h]
  float v13; // [esp+13Ch] [ebp-30h]
  phys_vec3 *p_m_p2; // [esp+140h] [ebp-2Ch]
  phys_vec3 *p_w; // [esp+144h] [ebp-28h]
  phys_vec3 *p_m_arm; // [esp+148h] [ebp-24h]
  phys_gjk_collision_info *p_cg1_cinfo_loc; // [esp+14Ch] [ebp-20h]
  float v18; // [esp+150h] [ebp-1Ch] BYREF
  float v19; // [esp+154h] [ebp-18h] BYREF
  float hit_dist; // [esp+160h] [ebp-Ch] BYREF
  void *v22; // [esp+164h] [ebp-8h] OVERLAPPED
  void *retaddr; // [esp+16Ch] [ebp+0h]

  hit_dist = a1;
  v22 = retaddr;
  POLYGON_CYLINDER_HACK(pgi, gti->m_cg, gi->m_cg);
  if ( !phys_gjk_info::phys_collide_do_gjk_collide(gjk_info, (int)&hit_dist, pgi) )
    return 0;
  calc_time(gjk_info, gti->m_keep_all_collisions, &v18, &v19);
  if ( pgi->m_end_time <= v18 && v19 >= 0.0 )
    return 0;
  if ( !gto && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp", 657, 0, "%s", "gto") )
    __debugbreak();
  p_cg1_cinfo_loc = &gjk_info->cg1_cinfo_loc;
  p_m_arm = &gto->m_arm;
  gto->m_arm.x = gjk_info->cg1_cinfo_loc.m_p1.x;
  p_m_arm->y = p_cg1_cinfo_loc->m_p1.y;
  p_m_arm->z = p_cg1_cinfo_loc->m_p1.z;
  p_w = &gti->m_gcci->m_cg_to_world_xform.w;
  p_m_p2 = &gjk_info->cg1_cinfo_loc.m_p2;
  v13 = gjk_info->cg1_cinfo_loc.m_p2.x + p_w->x;
  v12 = gjk_info->cg1_cinfo_loc.m_p2.y + p_w->y;
  v11 = gjk_info->cg1_cinfo_loc.m_p2.z + p_w->z;
  *(float *)&buf[252] = v13;
  v9 = v12;
  v10 = v11;
  *(unsigned int *)&buf[248] = &gto->m_hit_point;
  gto->m_hit_point.x = v13;
  *(float *)(*(unsigned int *)&buf[248] + 4) = v9;
  *(float *)(*(unsigned int *)&buf[248] + 8) = v10;
  *(unsigned int *)&buf[244] = &gjk_info->cg1_cinfo_loc.m_n;
  gto->m_hit_normal.x = gjk_info->cg1_cinfo_loc.m_n.x;
  gto->m_hit_normal.y = *(float *)(*(unsigned int *)&buf[244] + 4);
  gto->m_hit_normal.z = *(float *)(*(unsigned int *)&buf[244] + 8);
  gto->m_hit_time = v18;
  gto->m_hit_dist = v19;
  if ( v19 < 0.0 && (v19 > 0.0 || v18 != 0.0) )
  {
    sprintf(v7, "bad hit_time, hit_dist:%f, %f\n", v18, v19);
    tlWarning(v7);
    Com_PrintError(1, v7);
  }
  gto->m_is_foot = gti->m_cg->is_foot(gti->m_cg, &gto->m_arm);
  gto->m_gi = gi;
  return 1;
}

void __cdecl sort_gi_list(gjk_geom_info_t **list, int list_count)
{
  gjk_geom_info_t *v2; // [esp+4h] [ebp-28h]
  gjk_geom_info_t *v3; // [esp+8h] [ebp-24h]
  gjk_geom_info_t *v4; // [esp+10h] [ebp-1Ch]
  gjk_geom_info_t **cur; // [esp+14h] [ebp-18h]
  gjk_geom_info_t **prev_cur; // [esp+18h] [ebp-14h]
  gjk_geom_info_t **middle; // [esp+1Ch] [ebp-10h]
  gjk_geom_info_t **prev_middle; // [esp+20h] [ebp-Ch]

  if ( list_count >= 3 )
  {
    sort_gi_list(list, list_count / 2);
    sort_gi_list(&list[list_count / 2], list_count - list_count / 2);
    middle = &list[list_count / 2];
    for ( prev_middle = middle - 1;
          middle < &list[list_count] && (*prev_middle)->m_hit_time > (*middle)->m_hit_time;
          prev_middle = middle++ )
    {
      v3 = *prev_middle;
      *prev_middle = *middle;
      *middle = v3;
      cur = prev_middle;
      for ( prev_cur = prev_middle - 1; cur > list && (*prev_cur)->m_hit_time > (*cur)->m_hit_time; --prev_cur )
      {
        v2 = *prev_cur;
        *prev_cur = *cur;
        *cur = v2;
        cur = prev_cur;
      }
    }
  }
  else if ( list_count == 2 && (*list)->m_hit_time > list[1]->m_hit_time )
  {
    v4 = *list;
    *list = list[1];
    list[1] = v4;
  }
}

void  gjk_trace(int a1@<ebp>, const gjk_trace_input_t *gti, list_gjk_trace_output *list)
{
  float v3; // xmm0_4
  gjk_trace_output_t *v4; // [esp-30h] [ebp-4C4h]
  void *v5; // [esp-2Ch] [ebp-4C0h]
  gjk_geom_info_t *v6; // [esp-Ch] [ebp-4A0h]
  int j; // [esp-8h] [ebp-49Ch]
  gjk_trace_output_t *v8; // [esp-4h] [ebp-498h]
  gjk_trace_output_t *v9; // [esp+0h] [ebp-494h]
  gjk_trace_output_t *v10; // [esp+4h] [ebp-490h]
  gjk_geom_info_t *i; // [esp+34h] [ebp-460h]
  int gi_i; // [esp+40h] [ebp-454h]
  gjk_geom_info_t **v13; // [esp+44h] [ebp-450h]
  _BYTE v14[12]; // [esp+58h] [ebp-43Ch] BYREF
  phys_gjk_input pgi; // [esp+64h] [ebp-430h]
  _BYTE v17[12]; // [esp+B8h] [ebp-3DCh] BYREF
  float v18; // [esp+464h] [ebp-30h]
  gjkcc_info *v19; // [esp+468h] [ebp-2Ch]
  float z; // [esp+46Ch] [ebp-28h]
  float v21; // [esp+470h] [ebp-24h]
  gjkcc_info *v22; // [esp+474h] [ebp-20h]
  float y; // [esp+478h] [ebp-1Ch]
  float v24; // [esp+47Ch] [ebp-18h]
  gjkcc_info *m_gcci; // [esp+480h] [ebp-14h]
  float x; // [esp+484h] [ebp-10h]
  unsigned int v27[3]; // [esp+488h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+494h] [ebp+0h]

  v27[0] = a1;
  v27[1] = retaddr;
  //PIXBeginNamedEvent(-1, "gjk_trace_query");
  x = gti->m_query_input.m_cg_position.x;
  m_gcci = gti->m_gcci;
  v24 = m_gcci->m_cg_to_world_xform.w.x;
  if ( x != v24
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp",
          725,
          0,
          "%s",
          "gti.m_query_input.m_cg_position.GetX() == get_mat_wrow(gti.get_cg_mat()).GetX()") )
  {
    __debugbreak();
  }
  y = gti->m_query_input.m_cg_position.y;
  v22 = gti->m_gcci;
  v21 = v22->m_cg_to_world_xform.w.y;
  if ( y != v21
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp",
          726,
          0,
          "%s",
          "gti.m_query_input.m_cg_position.GetY() == get_mat_wrow(gti.get_cg_mat()).GetY()") )
  {
    __debugbreak();
  }
  z = gti->m_query_input.m_cg_position.z;
  v19 = gti->m_gcci;
  v18 = v19->m_cg_to_world_xform.w.z;
  if ( z != v18
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp",
          727,
          0,
          "%s",
          "gti.m_query_input.m_cg_position.GetZ() == get_mat_wrow(gti.get_cg_mat()).GetZ()") )
  {
    __debugbreak();
  }
  gjk_query_output::query_prolog(gti->m_query_output, &gti->m_query_input);
  gjk_query_cached(COERCE_FLOAT(v27), &gti->m_query_input, gti->m_query_output);
  gjk_query_output::query_epilog(gti->m_query_output);
  phys_gjk_info::phys_gjk_info((phys_gjk_info *)v17);
  list->m_list.m_first = 0;
  list->m_list.m_last_next_ptr = &list->m_list.m_first;
  list->m_list.m_alloc_count = 0;
  list->m_first_hit = 0;
  if ( gti->m_query_output->m_list_geom_info.m_alloc_count )
  {
    init_pgi((phys_gjk_input *)v14, gti);
    v13 = (gjk_geom_info_t **)phys_transient_allocator::allocate(
                                gti->m_allocator,
                                4 * gti->m_query_output->m_list_geom_info.m_alloc_count,
                                4,
                                0,
                                "phys_transient_allocator out of memory.");
    gi_i = 0;
    for ( i = gti->m_query_output->m_list_geom_info.m_first; i; i = i->m_next_link )
      v13[gi_i++] = i;
    sort_gi_list(v13, gi_i);
    if ( g_DXDeviceThread == GetCurrentThreadId() )
      D3DPERF_EndEvent();
    //PIXBeginNamedEvent(-1, "gjk_trace_collide");
    v10 = (gjk_trace_output_t *)phys_transient_allocator::allocate(
                                  gti->m_allocator,
                                  80,
                                  16,
                                  0,
                                  "phys_transient_allocator out of memory.");
    if ( v10 )
      v9 = v10;
    else
      v9 = 0;
    v8 = v9;
    for ( j = 0; j < gi_i && pgi.m_cg2_translation.z >= v13[j]->m_hit_time; ++j )
    {
      v6 = v13[j];
      pgi.m_cg2_translation.y = v6->m_hit_time;
      set_pgi_cg2((phys_gjk_input *)v14, gti, v6);
      if ( gjk_collide(COERCE_FLOAT(v27), (phys_gjk_info *)v17, (phys_gjk_input *)v14, v8, gti, v6) )
      {
        phys_link_list<gjk_trace_output_t>::add(&list->m_list, v8);
        if ( pgi.m_cg2_translation.z > v8->m_hit_time )
        {
          if ( (float)(v8->m_hit_time + gti->m_extra_time) <= 1.0 )
            v3 = v8->m_hit_time + gti->m_extra_time;
          else
            v3 = FLOAT_1_0;
          pgi.m_cg2_translation.z = v3;
        }
        if ( list->m_first_hit )
        {
          if ( list->m_first_hit->m_hit_time <= v8->m_hit_time )
          {
            if ( v8->m_hit_time == 0.0 && list->m_first_hit->m_hit_dist > v8->m_hit_dist )
            {
              if ( list->m_first_hit->m_hit_time != 0.0
                && !Assert_MyHandler(
                      "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp",
                      790,
                      0,
                      "%s",
                      "list->m_first_hit->m_hit_time == 0.0f") )
              {
                __debugbreak();
              }
              list->m_first_hit = v8;
            }
          }
          else
          {
            list->m_first_hit = v8;
          }
        }
        else
        {
          list->m_first_hit = v8;
        }
        v5 = phys_transient_allocator::allocate(gti->m_allocator, 80, 16, 0, "phys_transient_allocator out of memory.");
        v4 = v5 ? (gjk_trace_output_t *)v5 : 0;
        v8 = v4;
        if ( gti->m_exit_on_penetration && list->m_first_hit->m_hit_dist < 0.0 )
          break;
      }
    }
    if ( g_DXDeviceThread == GetCurrentThreadId() )
      D3DPERF_EndEvent();
  }
  else if ( GetCurrentThreadId() == g_DXDeviceThread )
  {
    D3DPERF_EndEvent();
  }
}

void *__thiscall phys_transient_allocator::allocate(
        phys_transient_allocator *this,
        int size,
        int alignment,
        int no_error,
        const char *error_msg)
{
  int v7; // [esp+4h] [ebp-14h]
  int v8; // [esp+Ch] [ebp-Ch]
  void *ptr; // [esp+14h] [ebp-4h]

  transient_allocator_update_largest_size(size);
  v8 = ~(alignment - 1) & (int)&this->m_cur[alignment - 1];
  if ( (char *)(size + v8) <= this->m_end )
  {
    this->m_cur = (char *)(size + v8);
    ptr = (void *)v8;
  }
  else
  {
    ptr = 0;
  }
  if ( !ptr )
  {
    phys_transient_allocator::resize(this);
    v7 = ~(alignment - 1) & (int)&this->m_cur[alignment - 1];
    if ( (char *)(size + v7) <= this->m_end )
    {
      this->m_cur = (char *)(size + v7);
      ptr = (void *)v7;
    }
    else
    {
      ptr = 0;
    }
    if ( !ptr
      && _tlAssert(
           "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_transient_allocator.h",
           79,
           "ptr",
           "transient allocation too large, increase block_size.") )
    {
      __debugbreak();
    }
  }
  if ( !ptr
    && !no_error
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_transient_allocator.h",
         81,
         "ptr || no_error",
         error_msg) )
  {
    __debugbreak();
  }
  return ptr;
}

void __cdecl init_pgi(phys_gjk_input *pgi, const gjk_trace_input_t *gti)
{
  const gjk_base_t *m_cg; // [esp+4h] [ebp-14h]
  float v3; // [esp+8h] [ebp-10h]
  gjkcc_info *m_gcci; // [esp+Ch] [ebp-Ch]

  pgi->m_cg1_translation.x = gti->m_query_input.m_cg_translation.x;
  pgi->m_cg1_translation.y = gti->m_query_input.m_cg_translation.y;
  pgi->m_cg1_translation.z = gti->m_query_input.m_cg_translation.z;
  pgi->m_cg2_translation.x = PHYS_ZERO_VEC_1.x;
  pgi->m_cg2_translation.y = PHYS_ZERO_VEC_1.y;
  pgi->m_cg2_translation.z = PHYS_ZERO_VEC_1.z;
  pgi->m_start_time = *(float *)&FLOAT_0_0;
  pgi->m_end_time = FLOAT_1_0;
  m_gcci = gti->m_gcci;
  m_cg = gti->m_cg;
  v3 = gti->m_cg->get_geom_radius(gti->m_cg);
  pgi->gjk_cg1 = m_cg;
  pgi->gjk_cg2 = 0;
  pgi->cg1_to_world_xform = &m_gcci->m_cg_to_world_xform;
  pgi->cg2_to_world_xform = 0;
  pgi->cg1_radius = v3;
  pgi->cg2_radius = *(float *)&FLOAT_0_0;
  pgi->gjk_ci = 0;
  pgi->m_sep_thresh = gti->m_gjk_ac_eps;
  pgi->m_intersection_test_only = 0;
  pgi->m_continuous_collision = 1;
}

void __cdecl set_pgi_cg2(phys_gjk_input *pgi, const gjk_trace_input_t *gti, gjk_geom_info_t *gi)
{
  pgi->gjk_cg2 = gi->m_cg;
  pgi->cg2_to_world_xform = (const phys_mat44 *)gjk_geom_info_t::get_xform(gi);
  pgi->cg2_radius = gi->m_cg->get_geom_radius(gi->m_cg);
  pgi->gjk_ci = get_gjk_cache_info(gti->m_gjk_cache, gti->m_cg, gi->m_cg);
}

gjk_entity_info_t *__thiscall gjk_geom_info_t::get_xform(gjk_geom_info_t *this)
{
  if ( (this->m_cg->m_flags & 8) != 0 )
    return (gjk_entity_info_t *)gjk_base_t::get_xform(this->m_cg);
  if ( this->m_ent_info )
    return this->m_ent_info;
  else
    return (gjk_entity_info_t *)&PHYS_IDENTITY_MATRIX_1;
}

const phys_mat44 *__thiscall gjk_base_t::get_xform(gjk_base_t *this)
{
  if ( (this->m_flags & 8) == 0
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
         105,
         "get_flag(FLAG_XFORM_VALID)",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  return this->m_xform_;
}

phys_gjk_info *__thiscall phys_gjk_info::phys_gjk_info(phys_gjk_info *this)
{
  int v2; // [esp+4h] [ebp-28h]
  phys_vec3 *n; // [esp+8h] [ebp-24h]
  int v4; // [esp+Ch] [ebp-20h]
  phys_vec3 *m; // [esp+10h] [ebp-1Ch]
  int v6; // [esp+14h] [ebp-18h]
  phys_vec3 *k; // [esp+18h] [ebp-14h]
  int v8; // [esp+1Ch] [ebp-10h]
  phys_vec3 *j; // [esp+20h] [ebp-Ch]
  int v10; // [esp+24h] [ebp-8h]
  phys_vec3 *i; // [esp+28h] [ebp-4h]

  v10 = 4;
  for ( i = this->m_w_verts; --v10 >= 0; ++i )
    ;
  v8 = 4;
  for ( j = this->m_a_verts; --v8 >= 0; ++j )
    ;
  v6 = 4;
  for ( k = this->m_b_verts; --v6 >= 0; ++k )
    ;
  v4 = 4;
  for ( m = this->m_a_inds; --v4 >= 0; ++m )
    ;
  v2 = 4;
  for ( n = this->m_b_inds; --v2 >= 0; ++n )
    ;
  return this;
}

void __cdecl setup_query_input(
        const gjkcc_input_t *gjkcc_in,
        const phys_vec3 *aabb_min,
        const phys_vec3 *aabb_max,
        const phys_vec3 *position,
        const phys_vec3 *translation,
        int passEntityNum,
        int contentMask,
        gjk_query_input *query_input)
{
  IgnoreEntParams ignoreEntParams; // [esp+14h] [ebp-Ch] BYREF

  query_input->m_cg_aabb_min.x = aabb_min->x;
  query_input->m_cg_aabb_min.y = aabb_min->y;
  query_input->m_cg_aabb_min.z = aabb_min->z;
  query_input->m_cg_aabb_max.x = aabb_max->x;
  query_input->m_cg_aabb_max.y = aabb_max->y;
  query_input->m_cg_aabb_max.z = aabb_max->z;
  query_input->m_cg_position.x = position->x;
  query_input->m_cg_position.y = position->y;
  query_input->m_cg_position.z = position->z;
  query_input->m_cg_translation.x = translation->x;
  query_input->m_cg_translation.y = translation->y;
  query_input->m_cg_translation.z = translation->z;
  query_input->m_ac_eps_vec.x = PT_AC_EPS_VEC.x;
  query_input->m_ac_eps_vec.y = PT_AC_EPS_VEC.y;
  query_input->m_ac_eps_vec.z = PT_AC_EPS_VEC.z;
  query_input->m_contents = contentMask;
  if ( gjkcc_in->is_server_thread )
  {
    SV_SetupIgnoreEntParams(&ignoreEntParams, passEntityNum);
    query_input->m_pass_entity_num = ignoreEntParams.baseEntity;
    query_input->m_pass_owner_num = ignoreEntParams.parentEntity;
  }
  else
  {
    query_input->m_pass_entity_num = passEntityNum;
    query_input->m_pass_owner_num = -1;
  }
  query_input->m_is_server_thread = gjkcc_in->is_server_thread;
  query_input->m_proximity_data = gjkcc_in->proximity_data;
  query_input->m_proximity_mask = gjkcc_in->proximity_mask;
  query_input->m_gjk_query_flags = gjkcc_in->m_gjk_query_flags;
  query_input->m_geom_skip_list.m_first = 0;
  query_input->m_geom_skip_list.m_last_next_ptr = &query_input->m_geom_skip_list.m_first;
  query_input->m_geom_skip_list.m_alloc_count = 0;
}

void  setup_trace_info(
        int a1@<ebp>,
        const gjkcc_input_t *gjkcc_in,
        float *start,
        float *mins,
        float *maxs,
        float *end,
        int passEntityNum,
        int contentMask,
        gjk_trace_input_t *gti)
{
  float v9; // [esp-4h] [ebp-5Ch] BYREF
  gjk_query_input *query_input; // [esp+0h] [ebp-58h]
  float v11; // [esp+4h] [ebp-54h]
  phys_vec3 translation; // [esp+8h] [ebp-50h]
  float v13; // [esp+18h] [ebp-40h]
  float v14; // [esp+1Ch] [ebp-3Ch] BYREF
  float v15; // [esp+20h] [ebp-38h]
  float v16; // [esp+24h] [ebp-34h]
  phys_vec3 pv_end; // [esp+28h] [ebp-30h] BYREF
  gjkcc_info *gjkcc_info; // [esp+48h] [ebp-10h]
  int v19; // [esp+4Ch] [ebp-Ch]
  void *v20; // [esp+50h] [ebp-8h]
  void *retaddr; // [esp+58h] [ebp+0h]

  v19 = a1;
  v20 = retaddr;
  gjkcc_info = find_gjkcc_info(gjkcc_in->gjkcc_id, gjkcc_in->is_server_thread);
  gti->m_gcci = gjkcc_info;
  gjkcc_info::update_cg(gti->m_gcci, mins, maxs, 0);
  gti->m_cg = gjkcc_info->m_cg_;
  gti->m_gjk_cache = &gjkcc_info->m_gjk_cache;
  gti->m_query_output = &gjkcc_info->m_gjk_query_output;
  Phys_Vec3ToNitrousVec((phys_vec3 *)&pv_end.y, start);
  Phys_Vec3ToNitrousVec((const phys_vec3 *)&v14, end);
  v13 = v14 - pv_end.y;
  translation.w = v15 - pv_end.z;
  translation.z = v16 - pv_end.w;
  v9 = v14 - pv_end.y;
  *(float *)&query_input = v15 - pv_end.z;
  v11 = v16 - pv_end.w;
  gjk_trace_input_t::set_cg_position(gti, (phys_vec3 *)&pv_end.y);
  gti->m_query_input.m_cg_translation.x = v9;
  gti->m_query_input.m_cg_translation.y = *(float *)&query_input;
  gti->m_query_input.m_cg_translation.z = v11;
  gti->m_gjk_ac_eps = PT_AC_EPS;
  gti->m_keep_all_collisions = 0;
  gti->m_exit_on_penetration = 1;
  gti->m_extra_time = *(float *)&FLOAT_0_0;
  gti->m_allocator = &gti->m_query_output->m_allocator;
  setup_query_input(
    gjkcc_in,
    &gjkcc_info->m_cg_aabb_min,
    &gjkcc_info->m_cg_aabb_max,
    (phys_vec3 *)&pv_end.y,
    (const phys_vec3 *)&v9,
    passEntityNum,
    contentMask,
    &gti->m_query_input);
  if ( !gjkcc_in->gjkcc_id )
  {
    if ( gjkcc_in->m_gjk_cg )
    {
      gti->m_cg = gjkcc_in->m_gjk_cg;
      phys_mat44::operator=(&gti->m_gcci->m_cg_to_world_xform, gjkcc_in->m_mat);
      gjk_trace_input_t::set_cg_position(gti, (phys_vec3 *)&pv_end.y);
      gti->m_cg->calc_aabb(
        gti->m_cg,
        &PHYS_IDENTITY_MATRIX_1,
        (phys_vec3 *)&gti->m_query_input,
        &gti->m_query_input.m_cg_aabb_max);
    }
    else
    {
      phys_mat44::operator=(&gti->m_gcci->m_cg_to_world_xform, &PHYS_IDENTITY_MATRIX_1);
      gjk_trace_input_t::set_cg_position(gti, (phys_vec3 *)&pv_end.y);
    }
  }
}

void __thiscall gjk_trace_input_t::set_cg_position(gjk_trace_input_t *this, const phys_vec3 *position)
{
  phys_vec3 *p_w; // [esp+Ch] [ebp-4h]

  p_w = &this->m_gcci->m_cg_to_world_xform.w;
  p_w->x = position->x;
  p_w->y = position->y;
  p_w->z = position->z;
  this->m_query_input.m_cg_position.x = position->x;
  this->m_query_input.m_cg_position.y = position->y;
  this->m_query_input.m_cg_position.z = position->z;
}

void  gjk_player_trace(
        int a1@<ebp>,
        const gjkcc_input_t *gjkcc_in,
        trace_t *results,
        float *start,
        float *mins,
        float *maxs,
        float *end,
        int passEntityNum,
        int contentMask)
{
  bool v9; // al
  int i; // [esp-Ch] [ebp-D8h]
  unsigned int v11[2]; // [esp-8h] [ebp-D4h] BYREF
  gjk_trace_output_t *gto; // [esp+0h] [ebp-CCh]
  list_gjk_trace_output list; // [esp+4h] [ebp-C8h] BYREF
  gjk_trace_input_t gti; // [esp+1Ch] [ebp-B0h] BYREF
  _UNKNOWN *retaddr; // [esp+CCh] [ebp+0h]

  *((unsigned int *)&gti.m_allocator + 1) = a1;
  *((unsigned int *)&gti.m_allocator + 2) = retaddr;
  list.m_list.m_alloc_count = (int)&gti.m_query_output;
  list.m_list.m_last_next_ptr = (gjk_trace_output_t **)&gti.m_query_input.m_proximity_data;
  gti.m_query_input.m_proximity_data = 0;
  gti.m_query_input.m_proximity_mask = (int)&gti.m_query_input.m_proximity_data;
  gti.m_query_input.m_gjk_query_flags = 0;
  setup_trace_info(
    (int)(&gti.m_allocator + 1),
    gjkcc_in,
    start,
    mins,
    maxs,
    end,
    passEntityNum,
    contentMask,
    (gjk_trace_input_t *)&list.m_first_hit);
  v11[0] = 0;
  v11[1] = v11;
  gto = 0;
  list.m_list.m_first = 0;
  gjk_trace((int)(&gti.m_allocator + 1), (const gjk_trace_input_t *)&list.m_first_hit, (list_gjk_trace_output *)v11);
  if ( list.m_list.m_first && list.m_list.m_first->m_hit_dist >= 0.0 )
  {
    for ( i = v11[0]; i; i = *(unsigned int *)(i + 64) )
    {
      if ( *(float *)(i + 52) < 0.0
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp",
              930,
              0,
              "%s",
              "gto->m_hit_dist >= 0.0f") )
      {
        __debugbreak();
      }
    }
  }
  if ( list.m_list.m_first )
  {
    v9 = is_walkable((int)(&gti.m_allocator + 1), list.m_list.m_first);
    fill_results(list.m_list.m_first, v9, results);
  }
  else
  {
    fill_results_no_hit(results);
  }
}

char __cdecl no_push_out(const gjk_trace_output_t *gto)
{
  const centity_s *cent; // eax
  const gentity_s *gent; // eax
  const vehicle_info_t *info; // [esp+0h] [ebp-8h]
  int etype; // [esp+4h] [ebp-4h]

  if ( !gto->m_gi->m_ent_info )
    return 0;
  switch ( gto->m_gi->m_ent_info->m_ent_type )
  {
    case ET_CENT:
      etype = gjk_entity_info_t::get_cent(gto->m_gi->m_ent_info)->nextState.eType;
LABEL_12:
      if ( etype == 1 || etype == 17 )
        return 1;
      if ( etype == 14 )
      {
        if ( gto->m_gi->m_ent_info->m_ent_type == ET_CENT )
        {
          cent = gjk_entity_info_t::get_cent(gto->m_gi->m_ent_info);
          info = CG_GetVehicleInfo(cent->nextState.un2.vehicleState.vehicleInfoIndex);
        }
        else
        {
          if ( gto->m_gi->m_ent_info->m_ent_type
            && !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp",
                  987,
                  0,
                  "%s",
                  "gto->m_gi->m_ent_info->is_gent()") )
          {
            __debugbreak();
          }
          gent = gjk_entity_info_t::get_gent(gto->m_gi->m_ent_info);
          info = CG_GetVehicleInfo(gent->s.un2.vehicleState.vehicleInfoIndex);
        }
        if ( info && info->remoteControl )
          return 1;
      }
      return 0;
    case ET_GENT:
      etype = gjk_entity_info_t::get_gent(gto->m_gi->m_ent_info)->s.eType;
      goto LABEL_12;
    case ET_GLASS:
      return 0;
  }
  if ( gto->m_gi->m_ent_info->m_ent_type != ET_DENT
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp",
          971,
          0,
          "%s",
          "gto->m_gi->m_ent_info->is_dent()") )
  {
    __debugbreak();
  }
  return 0;
}

const gentity_s *__thiscall gjk_entity_info_t::get_gent(gjk_entity_info_t *this)
{
  if ( (this->m_ent_type || !this->m_ent)
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../physics/phys_gjk_collision_detection.h",
          86,
          0,
          "%s",
          "m_ent_type == ET_GENT && m_ent != NULL") )
  {
    __debugbreak();
  }
  return (const gentity_s *)this->m_ent;
}

const centity_s *__thiscall gjk_entity_info_t::get_cent(gjk_entity_info_t *this)
{
  if ( (this->m_ent_type != ET_CENT || !this->m_ent)
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../physics/phys_gjk_collision_detection.h",
          87,
          0,
          "%s",
          "m_ent_type == ET_CENT && m_ent != NULL") )
  {
    __debugbreak();
  }
  return (const centity_s *)this->m_ent;
}

void __cdecl set_hit_info(geom_plane *gp, const gjk_trace_output_t *gto)
{
  int savedregs; // [esp+Ch] [ebp+0h] BYREF

  gp->m_normal.x = gto->m_hit_normal.x;
  gp->m_normal.y = gto->m_hit_normal.y;
  gp->m_normal.z = gto->m_hit_normal.z;
  gp->m_arm.x = gto->m_arm.x;
  gp->m_arm.y = gto->m_arm.y;
  gp->m_arm.z = gto->m_arm.z;
  gp->m_d = (float)((float)(gp->m_normal.x * gto->m_hit_point.x) + (float)(gp->m_normal.y * gto->m_hit_point.y))
          + (float)(gp->m_normal.z * gto->m_hit_point.z);
  gp->m_active = 1;
  gp->m_walkable = is_walkable((int)&savedregs, gto);
  gp->m_no_push_out = no_push_out(gto);
}

void __cdecl add_hit_info(
        const gjk_trace_output_t *gto,
        phys_static_array<geom_plane,128> *list_geom_plane,
        phys_link_list<gjk_geom_info_t> *geom_skip_list)
{
  geom_plane *v3; // eax

  v3 = phys_static_array<geom_plane,128>::add(list_geom_plane, 0, "phys array add overflow.");
  set_hit_info(v3, gto);
  phys_link_list<gjk_geom_info_t>::add(geom_skip_list, gto->m_gi);
}

char __cdecl project_succeeded(
        phys_static_array<geom_plane,128> *list_geom_plane,
        const phys_vec3 *new_position,
        float PROJECT_FAIL_THRESH)
{
  geom_plane *gp_i; // [esp+14h] [ebp-4h]

  for ( gp_i = list_geom_plane->m_slot_array; &list_geom_plane->m_slot_array[list_geom_plane->m_alloc_count] != gp_i; ++gp_i )
  {
    if ( COERCE_FLOAT(LODWORD(PROJECT_FAIL_THRESH) ^ _mask__NegFloat_) > (float)((float)((float)((float)(new_position->x * gp_i->m_normal.x)
                                                                                               + (float)(new_position->y * gp_i->m_normal.y))
                                                                                       + (float)(new_position->z
                                                                                               * gp_i->m_normal.z))
                                                                               - gp_i->m_right_side) )
      return 0;
  }
  return 1;
}

char  gjk_push_out@<al>(
        int a1@<ebp>,
        const gjkcc_input_t *gjkcc_in,
        gjk_slide_move_input_t *input,
        gjk_slide_move_output_t *output)
{
  void *v4; // esp
  int i; // [esp+10h] [ebp-2110h]
  const float *velocity; // [esp+18h] [ebp-2108h]
  int m; // [esp+6Ch] [ebp-20B4h]
  float x; // [esp+74h] [ebp-20ACh]
  float v10; // [esp+78h] [ebp-20A8h]
  float v11; // [esp+7Ch] [ebp-20A4h]
  geom_plane *v12; // [esp+B0h] [ebp-2070h]
  geom_plane *v13; // [esp+B8h] [ebp-2068h]
  gjk_trace_output_t v14; // [esp+C4h] [ebp-205Ch] BYREF
  float z; // [esp+118h] [ebp-2008h]
  float y; // [esp+11Ch] [ebp-2004h]
  gjk_geom_info_t *v17; // [esp+120h] [ebp-2000h]
  geom_plane *v18; // [esp+124h] [ebp-1FFCh]
  int v19; // [esp+128h] [ebp-1FF8h]
  int k; // [esp+12Ch] [ebp-1FF4h]
  int j; // [esp+130h] [ebp-1FF0h]
  phys_gjk_input v22; // [esp+134h] [ebp-1FECh] BYREF
  phys_vec3 *v23; // [esp+188h] [ebp-1F98h]
  float v24; // [esp+18Ch] [ebp-1F94h]
  int v25; // [esp+190h] [ebp-1F90h]
  phys_gjk_info v26; // [esp+194h] [ebp-1F8Ch] BYREF
  float v27; // [esp+534h] [ebp-1BECh]
  float v28; // [esp+538h] [ebp-1BE8h]
  float v29; // [esp+53Ch] [ebp-1BE4h]
  float v30; // [esp+54Ch] [ebp-1BD4h]
  float v31; // [esp+550h] [ebp-1BD0h]
  float v32; // [esp+554h] [ebp-1BCCh]
  float v33; // [esp+558h] [ebp-1BC8h]
  int n; // [esp+55Ch] [ebp-1BC4h]
  char v35; // [esp+563h] [ebp-1BBDh]
  phys_vec3 v36; // [esp+564h] [ebp-1BBCh] BYREF
  float v37; // [esp+574h] [ebp-1BACh]
  float v38; // [esp+578h] [ebp-1BA8h]
  float v39; // [esp+57Ch] [ebp-1BA4h]
  float v40; // [esp+584h] [ebp-1B9Ch]
  float v41; // [esp+588h] [ebp-1B98h]
  float v42; // [esp+58Ch] [ebp-1B94h]
  phys_vec3 *p_m_arm; // [esp+590h] [ebp-1B90h]
  geom_plane *v44; // [esp+594h] [ebp-1B8Ch]
  geom_plane *v45; // [esp+598h] [ebp-1B88h]
  geom_plane *v46; // [esp+59Ch] [ebp-1B84h]
  geom_plane *v47; // [esp+5A0h] [ebp-1B80h]
  geom_plane *m_slot_array; // [esp+5A4h] [ebp-1B7Ch]
  int v49; // [esp+5A8h] [ebp-1B78h]
  unsigned int v50[129]; // [esp+5ACh] [ebp-1B74h]
  gjk_trace_output_t *v51; // [esp+7B0h] [ebp-1970h]
  gjk_trace_output_t *m_next_link; // [esp+7B4h] [ebp-196Ch]
  gjk_trace_output_t *v53; // [esp+7B8h] [ebp-1968h]
  gjk_trace_output_t *m_first; // [esp+7BCh] [ebp-1964h]
  int m_alloc_count; // [esp+7C0h] [ebp-1960h]
  float v56; // [esp+7C4h] [ebp-195Ch]
  float v57; // [esp+7C8h] [ebp-1958h]
  float v58; // [esp+7CCh] [ebp-1954h]
  float v59; // [esp+7D4h] [ebp-194Ch]
  float v60; // [esp+7D8h] [ebp-1948h]
  float v61; // [esp+7DCh] [ebp-1944h]
  phys_vec3 *p_m_cg_translation; // [esp+7E0h] [ebp-1940h]
  phys_vec3 v63; // [esp+7E4h] [ebp-193Ch] BYREF
  float v64; // [esp+7FCh] [ebp-1924h]
  float v65; // [esp+800h] [ebp-1920h]
  float v66; // [esp+804h] [ebp-191Ch]
  gjkcc_info *m_gcci; // [esp+808h] [ebp-1918h]
  int v68; // [esp+80Ch] [ebp-1914h]
  int v69; // [esp+810h] [ebp-1910h]
  list_gjk_trace_output v70; // [esp+814h] [ebp-190Ch] BYREF
  gjk_trace_input_t v71; // [esp+824h] [ebp-18FCh] BYREF
  phys_vec3 v72; // [esp+8D4h] [ebp-184Ch] BYREF
  phys_vec3 v73; // [esp+8E4h] [ebp-183Ch] BYREF
  phys_static_array<geom_plane,128> v74; // [esp+8F4h] [ebp-182Ch] BYREF
  unsigned int v75[3]; // [esp+2114h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+2120h] [ebp+0h]

  v75[0] = a1;
  v75[1] = retaddr;
  v4 = alloca(8464);
  v74.m_slot_array = (geom_plane *const)&v74;
  v74.m_alloc_count = 0;
  Phys_Vec3ToNitrousVec(&v73, (float *)input->position);
  v72 = v73;
  gjk_query_input::gjk_query_input(&v71.m_query_input);
  setup_trace_info(
    (int)v75,
    gjkcc_in,
    (float *)input->position,
    (float *)input->mins,
    (float *)input->maxs,
    (float *)input->position,
    input->clientnum,
    input->tracemask,
    &v71);
  v71.m_keep_all_collisions = 1;
  v71.m_exit_on_penetration = 0;
  v71.m_extra_time = FLOAT_1_0;
  v70.m_list.m_first = 0;
  v70.m_list.m_last_next_ptr = (gjk_trace_output_t **)&v70;
  *(_QWORD *)&v70.m_list.m_alloc_count = 0;
  v69 = 5;
  v68 = 0;
LABEL_2:
  if ( v68 >= 5 )
    goto LABEL_54;
  m_gcci = v71.m_gcci;
  phys_vec3::operator=(&v71.m_gcci->m_cg_to_world_xform.w, &v73);
  phys_vec3::operator=(&v71.m_query_input.m_cg_position, &v73);
  v66 = v72.x - v73.x;
  v65 = v72.y - v73.y;
  v64 = v72.z - v73.z;
  v63.x = v72.x - v73.x;
  v63.y = v72.y - v73.y;
  v63.z = v72.z - v73.z;
  phys_vec3::operator=(&v71.m_query_input.m_cg_translation, &v63);
  gjk_trace((int)v75, &v71, &v70);
  if ( !v70.m_first_hit )
  {
    p_m_cg_translation = &v71.m_query_input.m_cg_translation;
    v61 = v73.x + v71.m_query_input.m_cg_translation.x;
    v60 = v73.y + v71.m_query_input.m_cg_translation.y;
    v59 = v73.z + v71.m_query_input.m_cg_translation.z;
    v56 = v73.x + v71.m_query_input.m_cg_translation.x;
    v57 = v73.y + v71.m_query_input.m_cg_translation.y;
    v58 = v73.z + v71.m_query_input.m_cg_translation.z;
    v72.x = v73.x + v71.m_query_input.m_cg_translation.x;
    v72.y = v73.y + v71.m_query_input.m_cg_translation.y;
    v72.z = v73.z + v71.m_query_input.m_cg_translation.z;
LABEL_54:
    Phys_NitrousVecToVec3(&v72, output->new_position);
    velocity = (const float *)input->velocity;
    output->new_velocity[0] = *velocity;
    output->new_velocity[1] = velocity[1];
    output->new_velocity[2] = velocity[2];
    v70.m_list.m_first = 0;
    v70.m_list.m_last_next_ptr = (gjk_trace_output_t **)&v70;
    v70.m_list.m_alloc_count = 0;
    v70.m_first_hit = 0;
    for ( i = 0; i < v74.m_alloc_count; ++i )
      ;
    return 1;
  }
  m_alloc_count = v74.m_alloc_count;
  m_first = v70.m_list.m_first;
  v53 = 0;
  while ( v53 != m_first )
  {
    v51 = m_first;
    v50[128] = v74.m_alloc_count;
    v50[v74.m_alloc_count] = m_first->m_gi;
    add_hit_info(v51, &v74, &v71.m_query_input.m_geom_skip_list);
    input->custom_process(input, v51);
    m_next_link = m_first->m_next_link;
    m_first = m_next_link;
  }
  v49 = v74.m_alloc_count;
  if ( v74.m_alloc_count <= m_alloc_count
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp",
          1078,
          0,
          "%s",
          "list_geom_plane.get_count() > plane_count") )
  {
    __debugbreak();
  }
  m_slot_array = v74.m_slot_array;
  v47 = v74.m_slot_array;
  v46 = &v74.m_slot_array[v74.m_alloc_count];
  v45 = v46;
  while ( v45 != v47 )
  {
    v44 = v47;
    v47->m_lambda = *(float *)&FLOAT_0_0;
    p_m_arm = &v44->m_arm;
    v42 = v73.x + v44->m_arm.x;
    v41 = v73.y + v44->m_arm.y;
    v40 = v73.z + v44->m_arm.z;
    v37 = v42;
    v38 = v41;
    v39 = v40;
    v44->m_right_side = v44->m_d
                      - (float)((float)((float)(v42 * v44->m_normal.x) + (float)(v41 * v44->m_normal.y))
                              + (float)(v40 * v44->m_normal.z));
    if ( v44->m_no_push_out && v44->m_right_side > 0.0 )
      v44->m_right_side = *(float *)&FLOAT_0_0;
    ++v47;
  }
  project((int)v75, &PHYS_ZERO_VEC_1, &v74, &v36);
  if ( project_succeeded(&v74, &v36, 5.0) )
  {
    v33 = Abs(&v36.x);
    if ( v33 <= 0.001 )
    {
      if ( v68
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp",
              1101,
              0,
              "%s",
              "bumpcount == 0") )
      {
        __debugbreak();
      }
      goto LABEL_54;
    }
    v32 = v73.x + v36.x;
    v31 = v73.y + v36.y;
    v30 = v73.z + v36.z;
    v27 = v73.x + v36.x;
    v28 = v73.y + v36.y;
    v29 = v73.z + v36.z;
    v72.x = v73.x + v36.x;
    v72.y = v73.y + v36.y;
    v72.z = v73.z + v36.z;
    phys_gjk_info::phys_gjk_info(&v26);
    v25 = 5;
    v24 = 0.1 * 0.1;
    v23 = &v71.m_query_input.m_cg_translation;
    v71.m_query_input.m_cg_translation.x = PHYS_ZERO_VEC_1.x;
    v71.m_query_input.m_cg_translation.y = PHYS_ZERO_VEC_1.y;
    v71.m_query_input.m_cg_translation.z = PHYS_ZERO_VEC_1.z;
    init_pgi(&v22, &v71);
    v22.m_sep_thresh = v33 + 1000000000.0;
    for ( j = 0; ; ++j )
    {
      if ( j >= 5 )
      {
LABEL_53:
        ++v68;
        goto LABEL_2;
      }
      gjk_trace_input_t::set_cg_position(&v71, &v72);
      for ( k = 0; ; ++k )
      {
        v19 = v74.m_alloc_count;
        if ( k >= v74.m_alloc_count )
          break;
        v18 = phys_static_array<geom_plane,128>::operator[](&v74, k);
        v17 = (gjk_geom_info_t *)v50[k];
        set_pgi_cg2(&v22, &v71, v17);
        if ( v22.m_cg1_translation.x != 0.0
          || (y = v22.m_cg1_translation.y, v22.m_cg1_translation.y != 0.0)
          || (z = v22.m_cg1_translation.z, v22.m_cg1_translation.z != 0.0) )
        {
          if ( !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp",
                  1159,
                  0,
                  "%s",
                  "pgi.m_cg1_translation.GetX() == 0.0f && pgi.m_cg1_translation.GetY() == 0.0f && pgi.m_cg1_translation.GetZ() == 0.0f") )
            __debugbreak();
        }
        if ( !gjk_collide(COERCE_FLOAT(v75), &v26, &v22, &v14, &v71, v17)
          && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp", 1162, 0, "%s", "retv") )
        {
          __debugbreak();
        }
        set_hit_info(v18, &v14);
      }
      v13 = v74.m_slot_array;
      v12 = &v74.m_slot_array[v74.m_alloc_count];
      while ( v12 != v13 )
      {
        v13->m_lambda = *(float *)&FLOAT_0_0;
        v13->m_right_side = v13->m_d
                          - (float)((float)((float)((float)(v73.x + v13->m_arm.x) * v13->m_normal.x)
                                          + (float)((float)(v73.y + v13->m_arm.y) * v13->m_normal.y))
                                  + (float)((float)(v73.z + v13->m_arm.z) * v13->m_normal.z));
        if ( v13->m_no_push_out && v13->m_right_side > 0.0 )
          v13->m_right_side = *(float *)&FLOAT_0_0;
        ++v13;
      }
      x = v36.x;
      v10 = v36.y;
      v11 = v36.z;
      project((int)v75, &PHYS_ZERO_VEC_1, &v74, &v36);
      if ( !project_succeeded(&v74, &v36, 5.0) )
        break;
      v72.x = v73.x + v36.x;
      v72.y = v73.y + v36.y;
      v72.z = v73.z + v36.z;
      v33 = Abs(&v36.x);
      if ( v24 >= (float)((float)((float)((float)(v36.x - x) * (float)(v36.x - x))
                                + (float)((float)(v36.y - v10) * (float)(v36.y - v10)))
                        + (float)((float)(v36.z - v11) * (float)(v36.z - v11))) )
        goto LABEL_53;
    }
    v70.m_list.m_first = 0;
    v70.m_list.m_last_next_ptr = (gjk_trace_output_t **)&v70;
    v70.m_list.m_alloc_count = 0;
    v70.m_first_hit = 0;
    for ( m = 0; m < v74.m_alloc_count; ++m )
      ;
    return 0;
  }
  else
  {
    v35 = 0;
    v70.m_list.m_first = 0;
    v70.m_list.m_last_next_ptr = (gjk_trace_output_t **)&v70;
    v70.m_list.m_alloc_count = 0;
    v70.m_first_hit = 0;
    for ( n = 0; n < v74.m_alloc_count; ++n )
      ;
    return v35;
  }
}

phys_vec3 *__thiscall phys_vec3::operator=(phys_vec3 *this, const phys_vec3 *v)
{
  this->x = v->x;
  this->y = v->y;
  this->z = v->z;
  return this;
}

double __cdecl Abs(const float *v)
{
  return (float)sqrt((float)((float)((float)(*v * *v) + (float)(v[1] * v[1])) + (float)(v[2] * v[2])));
}

void __cdecl Phys_NitrousVecToVec3(const phys_vec3 *inVector, float *outVector)
{
  *outVector = inVector->x;
  outVector[1] = inVector->y;
  outVector[2] = inVector->z;
}

gjk_query_input *__thiscall gjk_query_input::gjk_query_input(gjk_query_input *this)
{
  this->m_geom_skip_list.m_first = 0;
  this->m_geom_skip_list.m_last_next_ptr = &this->m_geom_skip_list.m_first;
  this->m_geom_skip_list.m_alloc_count = 0;
  return this;
}

bool  gjk_slide_move1@<eax>(
        int a1@<ebp>,
        const gjkcc_input_t *gjkcc_in,
        gjk_slide_move_input_t *input,
        const bool no_push_out,
        gjk_slide_move_output_t *output,
        bool *needs_push_out)
{
  void *v6; // esp
  float v7; // xmm0_4
  int m; // [esp-1DDCh] [ebp-1DE8h]
  geom_plane *v10; // [esp-1DACh] [ebp-1DB8h]
  geom_plane *v11; // [esp-1DA4h] [ebp-1DB0h]
  float v12[3]; // [esp-1D9Ch] [ebp-1DA8h] BYREF
  float v13[3]; // [esp-1D90h] [ebp-1D9Ch] BYREF
  float v14; // [esp-1D84h] [ebp-1D90h]
  float v15; // [esp-1D80h] [ebp-1D8Ch]
  float v16; // [esp-1D7Ch] [ebp-1D88h]
  float v17; // [esp-1D78h] [ebp-1D84h]
  float v18; // [esp-1D6Ch] [ebp-1D78h]
  float v19; // [esp-1D68h] [ebp-1D74h]
  float v20; // [esp-1D64h] [ebp-1D70h]
  phys_vec3 *v21; // [esp-1D60h] [ebp-1D6Ch]
  geom_plane *v22; // [esp-1D5Ch] [ebp-1D68h]
  geom_plane *v23; // [esp-1D58h] [ebp-1D64h]
  geom_plane *v24; // [esp-1D54h] [ebp-1D60h]
  geom_plane *v25; // [esp-1D50h] [ebp-1D5Ch]
  geom_plane *v26; // [esp-1D4Ch] [ebp-1D58h]
  float v27; // [esp-1D48h] [ebp-1D54h]
  geom_plane *v28; // [esp-1D44h] [ebp-1D50h]
  float v29; // [esp-1D40h] [ebp-1D4Ch]
  float v30; // [esp-1D3Ch] [ebp-1D48h]
  float v31; // [esp-1D38h] [ebp-1D44h]
  float v32; // [esp-1D2Ch] [ebp-1D38h]
  float v33; // [esp-1D28h] [ebp-1D34h]
  float v34; // [esp-1D24h] [ebp-1D30h]
  float v35; // [esp-1D20h] [ebp-1D2Ch]
  float v36; // [esp-1D1Ch] [ebp-1D28h]
  float v37; // [esp-1D18h] [ebp-1D24h]
  float v38; // [esp-1D08h] [ebp-1D14h]
  float v39; // [esp-1D04h] [ebp-1D10h]
  float v40; // [esp-1D00h] [ebp-1D0Ch]
  float v41; // [esp-1CFCh] [ebp-1D08h]
  float v42; // [esp-1CF8h] [ebp-1D04h]
  float v43; // [esp-1CF4h] [ebp-1D00h]
  float v44; // [esp-1CF0h] [ebp-1CFCh]
  float v45; // [esp-1CECh] [ebp-1CF8h]
  float v46; // [esp-1CE8h] [ebp-1CF4h]
  float v47; // [esp-1CD4h] [ebp-1CE0h]
  float v48; // [esp-1CD0h] [ebp-1CDCh]
  float v49; // [esp-1CCCh] [ebp-1CD8h]
  float *v50; // [esp-1CC8h] [ebp-1CD4h]
  float *p_x; // [esp-1CC4h] [ebp-1CD0h]
  geom_plane *v52; // [esp-1CC0h] [ebp-1CCCh]
  geom_plane *v53; // [esp-1CBCh] [ebp-1CC8h]
  geom_plane *v54; // [esp-1CB8h] [ebp-1CC4h]
  geom_plane *v55; // [esp-1CB4h] [ebp-1CC0h]
  float v56; // [esp-1CB0h] [ebp-1CBCh]
  float *v57; // [esp-1CACh] [ebp-1CB8h]
  int v58; // [esp-1CA8h] [ebp-1CB4h]
  int k; // [esp-1CA4h] [ebp-1CB0h]
  phys_vec3 v60; // [esp-1CA0h] [ebp-1CACh] BYREF
  phys_vec3 v61; // [esp-1C90h] [ebp-1C9Ch] BYREF
  float v62; // [esp-1C74h] [ebp-1C80h]
  float v63; // [esp-1C70h] [ebp-1C7Ch]
  float v64; // [esp-1C6Ch] [ebp-1C78h]
  float v65; // [esp-1C68h] [ebp-1C74h]
  float v66; // [esp-1C5Ch] [ebp-1C68h]
  float v67; // [esp-1C58h] [ebp-1C64h]
  float v68; // [esp-1C54h] [ebp-1C60h]
  phys_vec3 *v69; // [esp-1C50h] [ebp-1C5Ch]
  geom_plane *v70; // [esp-1C4Ch] [ebp-1C58h]
  geom_plane *v71; // [esp-1C48h] [ebp-1C54h]
  geom_plane *v72; // [esp-1C44h] [ebp-1C50h]
  geom_plane *v73; // [esp-1C40h] [ebp-1C4Ch]
  geom_plane *v74; // [esp-1C3Ch] [ebp-1C48h]
  int v75; // [esp-1C38h] [ebp-1C44h]
  float v76; // [esp-1C34h] [ebp-1C40h]
  float v77; // [esp-1C30h] [ebp-1C3Ch]
  float v78; // [esp-1C2Ch] [ebp-1C38h]
  float v79; // [esp-1C28h] [ebp-1C34h]
  float v80; // [esp-1C14h] [ebp-1C20h]
  float v81; // [esp-1C10h] [ebp-1C1Ch]
  float v82; // [esp-1C0Ch] [ebp-1C18h]
  float v83; // [esp-1C08h] [ebp-1C14h]
  float v84; // [esp-1C04h] [ebp-1C10h]
  geom_plane *v85; // [esp-1C00h] [ebp-1C0Ch]
  geom_plane *v86; // [esp-1BFCh] [ebp-1C08h]
  geom_plane *v87; // [esp-1BF8h] [ebp-1C04h]
  geom_plane *v88; // [esp-1BF4h] [ebp-1C00h]
  geom_plane *v89; // [esp-1BF0h] [ebp-1BFCh]
  int j; // [esp-1BECh] [ebp-1BF8h]
  int v91; // [esp-1BE8h] [ebp-1BF4h]
  float v92; // [esp-1BE4h] [ebp-1BF0h]
  float v93; // [esp-1BE0h] [ebp-1BECh]
  float v94; // [esp-1BDCh] [ebp-1BE8h]
  float v95; // [esp-1BD8h] [ebp-1BE4h]
  float v96; // [esp-1BCCh] [ebp-1BD8h]
  float v97; // [esp-1BC8h] [ebp-1BD4h]
  float v98; // [esp-1BC4h] [ebp-1BD0h]
  float v99; // [esp-1BC0h] [ebp-1BCCh]
  float v100; // [esp-1BBCh] [ebp-1BC8h]
  float v101; // [esp-1BB8h] [ebp-1BC4h]
  float v102; // [esp-1BACh] [ebp-1BB8h]
  float v103; // [esp-1BA8h] [ebp-1BB4h]
  float v104; // [esp-1BA4h] [ebp-1BB0h]
  float v105; // [esp-1BA0h] [ebp-1BACh]
  float v106; // [esp-1B9Ch] [ebp-1BA8h]
  float v107; // [esp-1B98h] [ebp-1BA4h]
  float v108; // [esp-1B8Ch] [ebp-1B98h]
  float v109; // [esp-1B88h] [ebp-1B94h]
  float v110; // [esp-1B84h] [ebp-1B90h]
  phys_vec3 *p_m_arm; // [esp-1B80h] [ebp-1B8Ch]
  geom_plane *v112; // [esp-1B7Ch] [ebp-1B88h]
  geom_plane *v113; // [esp-1B78h] [ebp-1B84h]
  geom_plane *v114; // [esp-1B74h] [ebp-1B80h]
  geom_plane *v115; // [esp-1B70h] [ebp-1B7Ch]
  geom_plane *m_slot_array; // [esp-1B6Ch] [ebp-1B78h]
  int v117; // [esp-1B68h] [ebp-1B74h]
  phys_vec3 *v118; // [esp-1B64h] [ebp-1B70h]
  float v119; // [esp-1B60h] [ebp-1B6Ch]
  float v120; // [esp-1B5Ch] [ebp-1B68h]
  float v121; // [esp-1B58h] [ebp-1B64h]
  float v122; // [esp-1B48h] [ebp-1B54h]
  float v123; // [esp-1B44h] [ebp-1B50h]
  float v124; // [esp-1B40h] [ebp-1B4Ch]
  float m_hit_dist; // [esp-1B3Ch] [ebp-1B48h]
  float v126; // [esp-1B38h] [ebp-1B44h] BYREF
  float v127; // [esp-1B2Ch] [ebp-1B38h] BYREF
  phys_vec3 v128; // [esp-1B20h] [ebp-1B2Ch] BYREF
  float v129; // [esp-1B10h] [ebp-1B1Ch]
  float v130; // [esp-1B0Ch] [ebp-1B18h]
  float v131; // [esp-1B08h] [ebp-1B14h]
  phys_vec3 *p_m_hit_point; // [esp-1B04h] [ebp-1B10h]
  float v133; // [esp-1B00h] [ebp-1B0Ch]
  float v134; // [esp-1AFCh] [ebp-1B08h]
  float v135; // [esp-1AF8h] [ebp-1B04h]
  float v136; // [esp-1AECh] [ebp-1AF8h]
  float v137; // [esp-1AE8h] [ebp-1AF4h]
  float v138; // [esp-1AE4h] [ebp-1AF0h]
  phys_vec3 m_hit_point; // [esp-1AE0h] [ebp-1AECh] BYREF
  gjk_trace_output_t *v140; // [esp-1AC4h] [ebp-1AD0h]
  gjk_trace_output_t *v141; // [esp-1AC0h] [ebp-1ACCh]
  gjk_trace_output_t *v142; // [esp-1ABCh] [ebp-1AC8h]
  gjk_trace_output_t *v143; // [esp-1AB8h] [ebp-1AC4h]
  int v144; // [esp-1AB4h] [ebp-1AC0h]
  float v145; // [esp-1AB0h] [ebp-1ABCh]
  float v146; // [esp-1AACh] [ebp-1AB8h]
  float v147; // [esp-1AA8h] [ebp-1AB4h]
  float v148; // [esp-1AA0h] [ebp-1AACh]
  float v149; // [esp-1A9Ch] [ebp-1AA8h]
  float v150; // [esp-1A98h] [ebp-1AA4h]
  float v151; // [esp-1A94h] [ebp-1AA0h]
  float v152; // [esp-1A90h] [ebp-1A9Ch]
  float v153; // [esp-1A8Ch] [ebp-1A98h]
  float v154; // [esp-1A88h] [ebp-1A94h]
  float v155; // [esp-1A78h] [ebp-1A84h]
  float v156; // [esp-1A74h] [ebp-1A80h]
  float v157; // [esp-1A70h] [ebp-1A7Ch]
  float m_hit_time; // [esp-1A6Ch] [ebp-1A78h]
  phys_vec3 *v159; // [esp-1A68h] [ebp-1A74h]
  gjk_trace_output_t *m_first_hit; // [esp-1A64h] [ebp-1A70h]
  float v161; // [esp-1A60h] [ebp-1A6Ch]
  float v162; // [esp-1A5Ch] [ebp-1A68h]
  float x; // [esp-1A58h] [ebp-1A64h]
  gjk_base_t *m_cg; // [esp-1A54h] [ebp-1A60h]
  const phys_mat44 *p_m_mat; // [esp-1A50h] [ebp-1A5Ch]
  gjk_entity_info_t *m_ent_info; // [esp-1A4Ch] [ebp-1A58h]
  const cbrush_t *v167; // [esp-1A48h] [ebp-1A54h]
  gjk_trace_output_t *v168; // [esp-1A44h] [ebp-1A50h]
  gjk_trace_output_t *m_next_link; // [esp-1A40h] [ebp-1A4Ch]
  gjk_trace_output_t *v170; // [esp-1A3Ch] [ebp-1A48h]
  gjk_trace_output_t *m_first; // [esp-1A38h] [ebp-1A44h]
  phys_vec3 *p_m_cg_translation; // [esp-1A34h] [ebp-1A40h]
  float v173; // [esp-1A30h] [ebp-1A3Ch]
  float v174; // [esp-1A2Ch] [ebp-1A38h]
  gjkcc_info *m_gcci; // [esp-1A28h] [ebp-1A34h]
  int i; // [esp-1A24h] [ebp-1A30h]
  int v177; // [esp-1A20h] [ebp-1A2Ch]
  int m_alloc_count; // [esp-1A1Ch] [ebp-1A28h]
  float v179; // [esp-1A18h] [ebp-1A24h]
  float v180; // [esp-1A14h] [ebp-1A20h]
  phys_vec3 v181; // [esp-1A10h] [ebp-1A1Ch]
  float v182; // [esp-1A00h] [ebp-1A0Ch]
  float v183; // [esp-19FCh] [ebp-1A08h]
  float v184; // [esp-19F8h] [ebp-1A04h]
  float v185; // [esp-19F4h] [ebp-1A00h]
  list_gjk_trace_output v186; // [esp-19F0h] [ebp-19FCh] BYREF
  phys_vec3 v187; // [esp-19E0h] [ebp-19ECh] BYREF
  phys_vec3 v188; // [esp-19D0h] [ebp-19DCh] BYREF
  phys_vec3 v189; // [esp-19C0h] [ebp-19CCh] BYREF
  float v190; // [esp-19B0h] [ebp-19BCh]
  float v191; // [esp-19ACh] [ebp-19B8h]
  float v192; // [esp-19A8h] [ebp-19B4h]
  float frametime; // [esp-19A4h] [ebp-19B0h]
  phys_vec3 v194; // [esp-19A0h] [ebp-19ACh] BYREF
  float v195; // [esp-1988h] [ebp-1994h]
  float v196; // [esp-1984h] [ebp-1990h]
  float v197; // [esp-1980h] [ebp-198Ch]
  float v198[3]; // [esp-197Ch] [ebp-1988h] BYREF
  phys_vec3 v199; // [esp-1970h] [ebp-197Ch] BYREF
  float v200; // [esp-195Ch] [ebp-1968h]
  float v201; // [esp-1958h] [ebp-1964h]
  float v202; // [esp-1954h] [ebp-1960h]
  float v203; // [esp-1950h] [ebp-195Ch]
  float v204; // [esp-194Ch] [ebp-1958h]
  float v205; // [esp-1948h] [ebp-1954h]
  float v206; // [esp-1940h] [ebp-194Ch]
  float v207; // [esp-193Ch] [ebp-1948h]
  float v208; // [esp-1938h] [ebp-1944h]
  float v209; // [esp-1934h] [ebp-1940h]
  float v210; // [esp-1930h] [ebp-193Ch]
  float v211; // [esp-192Ch] [ebp-1938h]
  float v212; // [esp-1928h] [ebp-1934h]
  float v213; // [esp-191Ch] [ebp-1928h]
  float v214; // [esp-1918h] [ebp-1924h]
  float v215; // [esp-1914h] [ebp-1920h]
  gjk_trace_input_t v216; // [esp-1910h] [ebp-191Ch] BYREF
  float *p_z; // [esp-1854h] [ebp-1860h]
  phys_vec3 v218; // [esp-1850h] [ebp-185Ch] BYREF
  phys_vec3 v219; // [esp-1840h] [ebp-184Ch] BYREF
  phys_vec3 v220; // [esp-1830h] [ebp-183Ch] BYREF
  phys_static_array<geom_plane,128> v221; // [esp-1820h] [ebp-182Ch] BYREF
  unsigned int v222[3]; // [esp+0h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]

  v222[0] = a1;
  v222[1] = retaddr;
  v6 = alloca(7648);
  v221.m_slot_array = (geom_plane *const)&v221;
  v221.m_alloc_count = 0;
  Phys_Vec3ToNitrousVec(&v220, (float *)input->position);
  Phys_Vec3ToNitrousVec(&v219, (float *)input->velocity);
  v218 = v219;
  if ( input->has_gravity )
  {
    p_z = &v218.z;
    v218.z = v218.z - (float)((float)input->gravity * input->frametime);
  }
  gjk_query_input::gjk_query_input(&v216.m_query_input);
  v215 = v219.x + v218.x;
  v214 = v219.y + v218.y;
  v213 = v219.z + v218.z;
  v210 = v219.x + v218.x;
  v211 = v219.y + v218.y;
  v212 = v219.z + v218.z;
  v209 = input->frametime * 0.5;
  v208 = v209 * (float)(v219.x + v218.x);
  v207 = v209 * (float)(v219.y + v218.y);
  v206 = v209 * (float)(v219.z + v218.z);
  v203 = v208;
  v204 = v207;
  v205 = v206;
  v202 = v220.x + v208;
  v201 = v220.y + v207;
  v200 = v220.z + v206;
  v199.x = v220.x + v208;
  v199.y = v220.y + v207;
  v199.z = v220.z + v206;
  Phys_NitrousVecToVec3(&v199, v198);
  setup_trace_info(
    (int)v222,
    gjkcc_in,
    (float *)input->position,
    (float *)input->mins,
    (float *)input->maxs,
    v198,
    input->clientnum,
    input->tracemask,
    &v216);
  v216.m_exit_on_penetration = 0;
  v197 = v199.x - v220.x;
  v196 = v199.y - v220.y;
  v195 = v199.z - v220.z;
  v194.x = v199.x - v220.x;
  v194.y = v199.y - v220.y;
  v194.z = v199.z - v220.z;
  frametime = input->frametime;
  v192 = frametime * v218.x;
  v191 = frametime * v218.y;
  v190 = frametime * v218.z;
  v189.x = frametime * v218.x;
  v189.y = frametime * v218.y;
  v189.z = frametime * v218.z;
  v188.x = v199.x - v220.x;
  v188.y = v199.y - v220.y;
  v188.z = v199.z - v220.z;
  v187.x = frametime * v218.x;
  v187.y = frametime * v218.y;
  v187.z = frametime * v218.z;
  v186.m_list.m_first = 0;
  v186.m_list.m_last_next_ptr = (gjk_trace_output_t **)&v186;
  v186.m_list.m_alloc_count = 0;
  v186.m_first_hit = 0;
  *needs_push_out = 0;
  v185 = 20.0 * 20.0;
  v184 = (float)(4.0 * 4.0)
       * (float)((float)((float)(v194.x * v194.x) + (float)(v194.y * v194.y)) + (float)(v194.z * v194.z));
  if ( v184 <= (float)(20.0 * 20.0) )
    v7 = v185;
  else
    v7 = v184;
  v183 = v7;
  v182 = v7;
  v181 = v220;
  v180 = FLOAT_0_016666668;
  v179 = *(float *)&FLOAT_0_0;
  m_alloc_count = v221.m_alloc_count;
  v177 = 128 - v221.m_alloc_count;
  i = 0;
  if ( g_bDebugRenderPlayerCollision->current.enabled )
  {
    Sys_EnterCriticalSection(CRTISECT_DEBUG_BRUSHES_AND_PATCHES);
    clear_debug_brushes_and_patches();
  }
  for ( i = 0; i < v177; ++i )
  {
    m_gcci = v216.m_gcci;
    phys_vec3::operator=(&v216.m_gcci->m_cg_to_world_xform.w, &v220);
    phys_vec3::operator=(&v216.m_query_input.m_cg_position, &v220);
    phys_vec3::operator=(&v216.m_query_input.m_cg_translation, &v188);
    v174 = input->frametime - v179;
    if ( v180 <= v174 )
      v173 = v180 / v174;
    else
      v173 = FLOAT_1_0;
    v216.m_extra_time = v173;
    gjk_trace((int)v222, &v216, &v186);
    if ( !v186.m_first_hit )
    {
      p_m_cg_translation = &v216.m_query_input.m_cg_translation;
      v220.x = v220.x + v216.m_query_input.m_cg_translation.x;
      v220.y = v220.y + v216.m_query_input.m_cg_translation.y;
      v220.z = v220.z + v216.m_query_input.m_cg_translation.z;
      break;
    }
    if ( g_bDebugRenderPlayerCollision->current.enabled )
    {
      m_first = v186.m_list.m_first;
      v170 = 0;
      while ( v170 != m_first )
      {
        v168 = m_first;
        if ( m_first->m_gi->m_cg->get_brush(m_first->m_gi->m_cg) )
        {
          v167 = v168->m_gi->m_cg->get_brush(v168->m_gi->m_cg);
          if ( v168->m_gi->m_ent_info )
            m_ent_info = v168->m_gi->m_ent_info;
          else
            m_ent_info = 0;
          p_m_mat = &m_ent_info->m_mat;
          add_debug_brush(v167, &m_ent_info->m_mat);
        }
        else if ( v168->m_gi->m_cg->get_type(v168->m_gi->m_cg) == 3 )
        {
          m_cg = v168->m_gi->m_cg;
          x = m_cg[1].m_aabb_mn_loc.x;
          add_debug_patch((const CollisionAabbTree *)LODWORD(x));
        }
        m_next_link = m_first->m_next_link;
        m_first = m_next_link;
      }
    }
    v162 = v186.m_first_hit->m_hit_time * v174;
    v161 = *(float *)&FLOAT_0_0;
    if ( v162 > v180 )
    {
      m_first_hit = v186.m_first_hit;
      v159 = &v216.m_query_input.m_cg_translation;
      m_hit_time = v186.m_first_hit->m_hit_time;
      v157 = m_hit_time * v216.m_query_input.m_cg_translation.x;
      v156 = m_hit_time * v216.m_query_input.m_cg_translation.y;
      v155 = m_hit_time * v216.m_query_input.m_cg_translation.z;
      v152 = m_hit_time * v216.m_query_input.m_cg_translation.x;
      v153 = m_hit_time * v216.m_query_input.m_cg_translation.y;
      v154 = m_hit_time * v216.m_query_input.m_cg_translation.z;
      v220.x = v220.x + (float)(m_hit_time * v216.m_query_input.m_cg_translation.x);
      v220.y = v220.y + (float)(m_hit_time * v216.m_query_input.m_cg_translation.y);
      v220.z = v220.z + (float)(m_hit_time * v216.m_query_input.m_cg_translation.z);
      v151 = 1.0 - v186.m_first_hit->m_hit_time;
      v150 = v151 * v188.x;
      v149 = v151 * v188.y;
      v148 = v151 * v188.z;
      v145 = v151 * v188.x;
      v146 = v151 * v188.y;
      v147 = v151 * v188.z;
      v194.x = v151 * v188.x;
      v194.y = v151 * v188.y;
      v194.z = v151 * v188.z;
      v189.x = v187.x;
      v189.y = v187.y;
      v189.z = v187.z;
      v179 = v179 + v162;
      v161 = v162;
    }
    v144 = v221.m_alloc_count;
    v143 = v186.m_list.m_first;
    v142 = 0;
    while ( v142 != v143 )
    {
      v140 = v143;
      if ( v180 >= (float)((float)(v143->m_hit_time * v174) - v161) )
      {
        if ( gjkcc_in->m_ent_num != -1 )
        {
          m_hit_point = v140->m_hit_point;
          v138 = 20.0 * v140->m_hit_normal.x;
          v137 = 20.0 * v140->m_hit_normal.y;
          v136 = 20.0 * v140->m_hit_normal.z;
          v133 = v138;
          v134 = v137;
          v135 = v136;
          p_m_hit_point = &v140->m_hit_point;
          v131 = v140->m_hit_point.x + v138;
          v130 = v140->m_hit_point.y + v137;
          v129 = v140->m_hit_point.z + v136;
          v128.x = v131;
          v128.y = v130;
          v128.z = v129;
          Phys_NitrousVecToVec3(&m_hit_point, &v127);
          Phys_NitrousVecToVec3(&v128, &v126);
        }
        if ( no_push_out && v140->m_hit_dist < 0.0 )
        {
          m_hit_dist = v140->m_hit_dist;
          v124 = m_hit_dist * v140->m_hit_normal.x;
          v123 = m_hit_dist * v140->m_hit_normal.y;
          v122 = m_hit_dist * v140->m_hit_normal.z;
          v119 = v124;
          v120 = v123;
          v121 = v122;
          v118 = &v140->m_hit_point;
          v140->m_hit_point.x = v140->m_hit_point.x + v124;
          v118->y = v118->y + v120;
          v118->z = v118->z + v121;
          v140->m_hit_dist = *(float *)&FLOAT_0_0;
        }
        add_hit_info(v140, &v221, &v216.m_query_input.m_geom_skip_list);
        input->custom_process(input, v140);
      }
      v141 = v143->m_next_link;
      v143 = v141;
    }
    v117 = v221.m_alloc_count;
    if ( v221.m_alloc_count <= v144
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp",
            1325,
            0,
            "%s",
            "list_geom_plane.get_count() > plane_count") )
    {
      __debugbreak();
    }
    m_slot_array = v221.m_slot_array;
    v115 = v221.m_slot_array;
    v114 = &v221.m_slot_array[v221.m_alloc_count];
    v113 = v114;
    while ( v113 != v115 )
    {
      v112 = v115;
      v115->m_lambda = *(float *)&FLOAT_0_0;
      p_m_arm = &v112->m_arm;
      v110 = v220.x + v112->m_arm.x;
      v109 = v220.y + v112->m_arm.y;
      v108 = v220.z + v112->m_arm.z;
      v105 = v110;
      v106 = v109;
      v107 = v108;
      v112->m_right_side = v112->m_d
                         - (float)((float)((float)(v110 * v112->m_normal.x) + (float)(v109 * v112->m_normal.y))
                                 + (float)(v108 * v112->m_normal.z));
      if ( (no_push_out || v112->m_no_push_out) && v112->m_right_side > 0.0 )
        v112->m_right_side = *(float *)&FLOAT_0_0;
      ++v115;
    }
    project((int)v222, &v194, &v221, &v188);
    v104 = v220.x + v188.x;
    v103 = v220.y + v188.y;
    v102 = v220.z + v188.z;
    v99 = v220.x + v188.x;
    v100 = v220.y + v188.y;
    v101 = v220.z + v188.z;
    v98 = (float)(v220.x + v188.x) - v181.x;
    v97 = (float)(v220.y + v188.y) - v181.y;
    v96 = (float)(v220.z + v188.z) - v181.z;
    v93 = v98;
    v94 = v97;
    v95 = v96;
    v92 = (float)((float)(v93 * v93) + (float)(v94 * v94)) + (float)(v95 * v95);
    if ( v92 > v182 )
    {
      *needs_push_out = 1;
      v91 = 0;
      v186.m_list.m_first = 0;
      v186.m_list.m_last_next_ptr = (gjk_trace_output_t **)&v186;
      v186.m_list.m_alloc_count = 0;
      v186.m_first_hit = 0;
      for ( j = 0; j < v221.m_alloc_count; ++j )
        ;
      return v91;
    }
    v89 = v221.m_slot_array;
    v88 = v221.m_slot_array;
    v87 = &v221.m_slot_array[v221.m_alloc_count];
    v86 = v87;
    while ( v86 != v88 )
    {
      v85 = v88;
      if ( v88->m_right_side > 0.0 )
        v85->m_right_side = *(float *)&FLOAT_0_0;
      ++v88;
    }
    project((int)v222, &v189, &v221, &v187);
    v84 = (float)((float)(v188.x * v188.x) + (float)(v188.y * v188.y)) + (float)(v188.z * v188.z);
    if ( (float)(0.0099999998 * 0.0099999998) > v84 )
      break;
  }
  if ( g_bDebugRenderPlayerCollision->current.enabled )
    Sys_LeaveCriticalSection(CRTISECT_DEBUG_BRUSHES_AND_PATCHES);
  v83 = 1.0 / input->frametime;
  v82 = v83 * v187.x;
  v81 = v83 * v187.y;
  v80 = v83 * v187.z;
  v77 = v83 * v187.x;
  v78 = v83 * v187.y;
  v79 = v83 * v187.z;
  v219.x = v83 * v187.x;
  v219.y = v83 * v187.y;
  v219.z = v83 * v187.z;
  if ( do_step_down && input->do_step_down )
  {
    v76 = FLOAT_10000_0;
    v75 = 0;
    v74 = v221.m_slot_array;
    v73 = v221.m_slot_array;
    v72 = &v221.m_slot_array[v221.m_alloc_count];
    v71 = v72;
    while ( v71 != v73 )
    {
      v70 = v73;
      v73->m_active = 0;
      if ( v70->m_walkable )
      {
        v69 = &v70->m_arm;
        v68 = v220.x + v70->m_arm.x;
        v67 = v220.y + v70->m_arm.y;
        v66 = v220.z + v70->m_arm.z;
        v63 = v68;
        v64 = v67;
        v65 = v66;
        v62 = (float)((float)((float)(v68 * v70->m_normal.x) + (float)(v67 * v70->m_normal.y))
                    + (float)(v66 * v70->m_normal.z))
            - v70->m_d;
        if ( v76 > v62 )
          v76 = v62;
      }
      else
      {
        ++v75;
      }
      ++v73;
    }
    if ( v76 > WALKABLE_DIST_THRESH && v76 != 10000.0 )
    {
      v61.x = *(float *)&FLOAT_0_0;
      v61.y = *(float *)&FLOAT_0_0;
      v61.z = FLOAT_N30_0;
      v60.x = *(float *)&FLOAT_0_0;
      v60.y = *(float *)&FLOAT_0_0;
      v60.z = FLOAT_N30_0;
      v60.w = v61.w;
      for ( k = 0; ; ++k )
      {
        v58 = v221.m_alloc_count;
        if ( k >= v221.m_alloc_count )
          break;
        v57 = 0;
        v56 = FLOAT_1_0;
        v55 = v221.m_slot_array;
        v54 = v221.m_slot_array;
        v53 = &v221.m_slot_array[v221.m_alloc_count];
        v52 = v53;
        while ( v52 != v54 )
        {
          p_x = &v54->m_normal.x;
          if ( !v54->m_active )
          {
            v50 = p_x + 4;
            v49 = v220.x + p_x[4];
            v48 = v220.y + p_x[5];
            v47 = v220.z + p_x[6];
            v44 = v49;
            v45 = v48;
            v46 = v47;
            v43 = (float)((float)((float)(v49 * *p_x) + (float)(v48 * p_x[1])) + (float)(v47 * p_x[2])) - p_x[8];
            v42 = (float)((float)(v60.x * *p_x) + (float)(v60.y * p_x[1])) + (float)(v60.z * p_x[2]);
            if ( COERCE_FLOAT(LODWORD(v42) & _mask__AbsFloat_) > 0.001 )
            {
              v41 = COERCE_FLOAT(LODWORD(v43) ^ _mask__NegFloat_) / v42;
              if ( v56 > v41 )
              {
                v57 = p_x;
                v56 = v41;
              }
            }
          }
          ++v54;
        }
        if ( !v57 )
          break;
        *((_BYTE *)v57 + 44) = 1;
        v40 = v56 * v60.x;
        v39 = v56 * v60.y;
        v38 = v56 * v60.z;
        v35 = v56 * v60.x;
        v36 = v56 * v60.y;
        v37 = v56 * v60.z;
        v34 = v220.x + (float)(v56 * v60.x);
        v33 = v220.y + (float)(v56 * v60.y);
        v32 = v220.z + (float)(v56 * v60.z);
        v29 = v34;
        v30 = v33;
        v31 = v32;
        if ( *((_BYTE *)v57 + 45) )
        {
          v28 = (geom_plane *)v57;
        }
        else
        {
          v28 = 0;
          v27 = WALKABLE_DIST_THRESH;
          v26 = v221.m_slot_array;
          v25 = v221.m_slot_array;
          v24 = &v221.m_slot_array[v221.m_alloc_count];
          v23 = v24;
          while ( v23 != v25 )
          {
            v22 = v25;
            if ( v25->m_walkable )
            {
              if ( v22->m_active
                && !Assert_MyHandler(
                      "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp",
                      1482,
                      0,
                      "%s",
                      "gp->m_active == false") )
              {
                __debugbreak();
              }
              v21 = &v22->m_arm;
              v20 = v29 + v22->m_arm.x;
              v19 = v30 + v22->m_arm.y;
              v18 = v31 + v22->m_arm.z;
              v15 = v20;
              v16 = v19;
              v17 = v18;
              v14 = (float)((float)((float)(v20 * v22->m_normal.x) + (float)(v19 * v22->m_normal.y))
                          + (float)(v18 * v22->m_normal.z))
                  - v22->m_d;
              if ( v27 >= v14 )
              {
                v28 = v22;
                v27 = v14;
              }
            }
            ++v25;
          }
        }
        if ( v28 )
        {
          if ( v56 > 0.0 )
          {
            v220.x = v29;
            v220.y = v30;
            v220.z = v31;
            Phys_NitrousVecToVec3(&v28->m_normal, v13);
            Phys_NitrousVecToVec3(&v219, v12);
            PM_ProjectVelocity(v12, v13, v12);
            Phys_Vec3ToNitrousVec(v12, &v219);
          }
          break;
        }
        v11 = v221.m_slot_array;
        v10 = &v221.m_slot_array[v221.m_alloc_count];
        while ( v10 != v11 )
        {
          if ( *((_BYTE *)v57 + 44) )
          {
            v11->m_right_side = v11->m_d
                              - (float)((float)((float)((float)(v220.x + v11->m_arm.x) * v11->m_normal.x)
                                              + (float)((float)(v220.y + v11->m_arm.y) * v11->m_normal.y))
                                      + (float)((float)(v220.z + v11->m_arm.z) * v11->m_normal.z));
            if ( v11->m_right_side > 0.0 )
              v11->m_right_side = *(float *)&FLOAT_0_0;
          }
          ++v11;
        }
        project((int)v222, &v61, &v221, &v60);
      }
    }
  }
  Phys_NitrousVecToVec3(&v220, output->new_position);
  Phys_NitrousVecToVec3(&v219, output->new_velocity);
  v186.m_list.m_first = 0;
  v186.m_list.m_last_next_ptr = (gjk_trace_output_t **)&v186;
  v186.m_list.m_alloc_count = 0;
  v186.m_first_hit = 0;
  for ( m = 0; m < v221.m_alloc_count; ++m )
    ;
  return v221.m_alloc_count > 0;
}

int __cdecl gjk_slide_move(
        const gjkcc_input_t *gjkcc_in,
        gjk_slide_move_input_t *input,
        gjk_slide_move_output_t *output)
{
  const float *velocity; // [esp+0h] [ebp-28h]
  const float *position; // [esp+4h] [ebp-24h]
  float p1[3]; // [esp+8h] [ebp-20h] BYREF
  float p0[3]; // [esp+14h] [ebp-14h] BYREF
  int retv; // [esp+20h] [ebp-8h]
  bool needs_push_out; // [esp+27h] [ebp-1h] BYREF
  int savedregs; // [esp+28h] [ebp+0h] BYREF

  output->expensive_push_out = 0;
  output->expensive_push_out_failed = 0;
  retv = gjk_slide_move1((int)&savedregs, gjkcc_in, input, 0, output, &needs_push_out);
  if ( !needs_push_out )
    return retv;
  if ( phys_debugExpensivePushout->current.enabled )
  {
    position = (const float *)input->position;
    p0[0] = *position;
    p0[1] = position[1];
    p0[2] = position[2];
    velocity = (const float *)input->velocity;
    p1[0] = *velocity;
    p1[1] = velocity[1];
    p1[2] = velocity[2];
    CG_DebugLine(p0, p1, colorRed, 1, 1000);
    p1[0] = p0[0];
    p1[1] = p0[1];
    p1[2] = p0[2] + 10000.0;
    CG_DebugLine(p0, p1, colorBlue, 1, 1000);
  }
  output->expensive_push_out = 1;
  if ( !gjk_push_out((int)&savedregs, gjkcc_in, input, output) )
  {
    output->expensive_push_out_failed = 1;
    gjk_slide_move1((int)&savedregs, gjkcc_in, input, 1, output, &needs_push_out);
  }
  return 1;
}

void  setup_player_push_slide_move_input(
        int a1@<ebp>,
        player_push_slide_move_input_t *input,
        pmove_t *pm,
        pml_t *pml,
        const phys_vec3 *push_movement)
{
  phys_vec3 v5; // [esp-30h] [ebp-3Ch] BYREF
  float v6; // [esp-14h] [ebp-20h]
  float v7; // [esp-10h] [ebp-1Ch]
  float v8; // [esp-Ch] [ebp-18h]
  float v9; // [esp-8h] [ebp-14h]
  float v10; // [esp-4h] [ebp-10h]
  int v11; // [esp+0h] [ebp-Ch]
  void *v12; // [esp+4h] [ebp-8h]
  void *retaddr; // [esp+Ch] [ebp+0h]

  v11 = a1;
  v12 = retaddr;
  v10 = FLOAT_0_033333335;
  v9 = 1.0 / 0.033333335;
  v8 = (float)(1.0 / 0.033333335) * push_movement->x;
  v7 = (float)(1.0 / 0.033333335) * push_movement->y;
  v6 = (float)(1.0 / 0.033333335) * push_movement->z;
  v5.x = v8;
  v5.y = v7;
  v5.z = v6;
  Phys_NitrousVecToVec3(&v5, input->velocity_);
  input->clientnum = pm->ps->clientNum;
  input->do_step_down = 1;
  input->frametime = v10;
  input->gravity = 0;
  input->has_gravity = 0;
  input->maxs = (const float (*)[3])pm->maxs;
  input->mins = (const float (*)[3])pm->mins;
  input->position = (const float (*)[3])pm->ps->origin;
  input->tracemask = pm->tracemask;
  input->velocity = (const float (*)[3])input->velocity_;
}

void  gjk_sentient_push(
        int a1@<ebp>,
        pmove_t *pm,
        pml_t *pml,
        float *origin,
        float *mins,
        float *maxs,
        int contentMask,
        int passEntityNum,
        const gjkcc_input_t *gjkcc_in,
        const float pushRadius,
        float frameTime,
        float *velocity_,
        float *origin_)
{
  void *v13; // esp
  int i; // [esp-1AC8h] [ebp-1AD4h]
  const phys_vec3 *v15; // [esp-1AC4h] [ebp-1AD0h]
  phys_vec3 v16; // [esp-1AC0h] [ebp-1ACCh] BYREF
  geom_plane *v17; // [esp-1AA4h] [ebp-1AB0h]
  geom_plane *v18; // [esp-1AA0h] [ebp-1AACh]
  geom_plane *v19; // [esp-1A9Ch] [ebp-1AA8h]
  geom_plane *v20; // [esp-1A98h] [ebp-1AA4h]
  geom_plane *m_slot_array; // [esp-1A94h] [ebp-1AA0h]
  gjk_slide_move_output_t v22; // [esp-1A90h] [ebp-1A9Ch] BYREF
  player_push_slide_move_input_t v23; // [esp-1A74h] [ebp-1A80h] BYREF
  float v24; // [esp-1A3Ch] [ebp-1A48h]
  float v25; // [esp-1A38h] [ebp-1A44h]
  float v26; // [esp-1A34h] [ebp-1A40h]
  phys_vec3 v27; // [esp-1A30h] [ebp-1A3Ch] BYREF
  phys_vec3 *p_m_arm; // [esp-1A14h] [ebp-1A20h]
  float v29; // [esp-1A10h] [ebp-1A1Ch]
  float v30; // [esp-1A0Ch] [ebp-1A18h]
  float v31; // [esp-1A08h] [ebp-1A14h]
  float v32; // [esp-19F4h] [ebp-1A00h]
  float v33; // [esp-19F0h] [ebp-19FCh]
  float v34; // [esp-19ECh] [ebp-19F8h]
  int v35; // [esp-19E8h] [ebp-19F4h]
  geom_plane *v36; // [esp-19E4h] [ebp-19F0h]
  float v37; // [esp-19E0h] [ebp-19ECh]
  float v38; // [esp-19DCh] [ebp-19E8h]
  float v39; // [esp-19D8h] [ebp-19E4h]
  float v40; // [esp-19D0h] [ebp-19DCh]
  float v41; // [esp-19CCh] [ebp-19D8h]
  float v42; // [esp-19C8h] [ebp-19D4h]
  float v43; // [esp-19C4h] [ebp-19D0h]
  float v44; // [esp-19C0h] [ebp-19CCh]
  float v45; // [esp-19BCh] [ebp-19C8h]
  float v46; // [esp-19B8h] [ebp-19C4h]
  float v47; // [esp-19B4h] [ebp-19C0h]
  float v48; // [esp-19B0h] [ebp-19BCh]
  float v49; // [esp-19ACh] [ebp-19B8h]
  float v50; // [esp-19A8h] [ebp-19B4h]
  float v51; // [esp-199Ch] [ebp-19A8h]
  float v52; // [esp-1998h] [ebp-19A4h]
  float v53; // [esp-1994h] [ebp-19A0h]
  float *p_x; // [esp-1990h] [ebp-199Ch]
  gjk_entity_info_t *xform; // [esp-198Ch] [ebp-1998h]
  float v56; // [esp-1988h] [ebp-1994h]
  gjk_base_t *m_cg; // [esp-1984h] [ebp-1990h]
  float v58; // [esp-1980h] [ebp-198Ch]
  bool v59; // [esp-1979h] [ebp-1985h]
  gjk_geom_info_t *v60; // [esp-1978h] [ebp-1984h]
  gjk_geom_info_t *m_next_link; // [esp-1974h] [ebp-1980h]
  gjk_geom_info_t *v62; // [esp-1970h] [ebp-197Ch]
  gjk_geom_info_t *v63; // [esp-196Ch] [ebp-1978h]
  gjk_geom_info_t *m_first; // [esp-1968h] [ebp-1974h]
  char v65; // [esp-1961h] [ebp-196Dh]
  phys_static_array<geom_plane,128> v66; // [esp-1960h] [ebp-196Ch] BYREF
  gjk_trace_input_t v67; // [esp-150h] [ebp-15Ch] BYREF
  phys_vec3 v68; // [esp-A0h] [ebp-ACh] BYREF
  float v69; // [esp-90h] [ebp-9Ch]
  float v70; // [esp-8Ch] [ebp-98h]
  float v71; // [esp-88h] [ebp-94h]
  float v72; // [esp-80h] [ebp-8Ch]
  float v73; // [esp-7Ch] [ebp-88h]
  float v74; // [esp-78h] [ebp-84h]
  const phys_vec3 *v75; // [esp-74h] [ebp-80h]
  phys_vec3 v76; // [esp-70h] [ebp-7Ch] BYREF
  float v77; // [esp-60h] [ebp-6Ch]
  float v78; // [esp-5Ch] [ebp-68h]
  float v79; // [esp-58h] [ebp-64h]
  float v80; // [esp-50h] [ebp-5Ch]
  float v81; // [esp-4Ch] [ebp-58h]
  float v82; // [esp-48h] [ebp-54h]
  const phys_vec3 *v83; // [esp-44h] [ebp-50h]
  phys_vec3 v84; // [esp-40h] [ebp-4Ch] BYREF
  float v85; // [esp-30h] [ebp-3Ch]
  float v86; // [esp-2Ch] [ebp-38h]
  float v87; // [esp-28h] [ebp-34h]
  float v88; // [esp-14h] [ebp-20h]
  int v89; // [esp-10h] [ebp-1Ch]
  int v90; // [esp-Ch] [ebp-18h]
  int v91; // [esp-8h] [ebp-14h]
  unsigned int v92[3]; // [esp+0h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]

  v92[0] = a1;
  v92[1] = retaddr;
  v13 = alloca(6864);
  if ( frameTime <= 0.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp",
          1603,
          0,
          "%s",
          "frameTime > 0.0f") )
  {
    __debugbreak();
  }
  v89 = *(unsigned int *)&FLOAT_0_0;
  v90 = *(unsigned int *)&FLOAT_0_0;
  v91 = *(unsigned int *)&FLOAT_0_0;
  v88 = FLOAT_2_0;
  v85 = pushRadius + 2.0;
  v86 = pushRadius + 2.0;
  v87 = *(float *)&FLOAT_0_0;
  v83 = Phys_Vec3ToNitrousVec(&v84, mins);
  v82 = v83->x - v85;
  v81 = v83->y - v86;
  v80 = v83->z - v87;
  v77 = v82;
  v78 = v81;
  v79 = v80;
  v75 = Phys_Vec3ToNitrousVec(&v76, maxs);
  v74 = v75->x + v85;
  v73 = v75->y + v86;
  v72 = v75->z + v87;
  v69 = v74;
  v70 = v73;
  v71 = v72;
  Phys_Vec3ToNitrousVec(&v68, origin);
  gjk_query_input::gjk_query_input(&v67.m_query_input);
  setup_trace_info((int)v92, gjkcc_in, origin, mins, maxs, origin, passEntityNum, contentMask, &v67);
  v67.m_query_input.m_cg_aabb_min.x = v77;
  v67.m_query_input.m_cg_aabb_min.y = v78;
  v67.m_query_input.m_cg_aabb_min.z = v79;
  v67.m_query_input.m_cg_aabb_max.x = v69;
  v67.m_query_input.m_cg_aabb_max.y = v70;
  v67.m_query_input.m_cg_aabb_max.z = v71;
  v67.m_query_input.m_gjk_query_flags = 2;
  gjk_query_output::query_prolog(v67.m_query_output, &v67.m_query_input);
  gjk_query_cached(COERCE_FLOAT(v92), &v67.m_query_input, v67.m_query_output);
  gjk_query_output::query_epilog(v67.m_query_output);
  v66.m_slot_array = (geom_plane *const)&v66;
  v66.m_alloc_count = 0;
  v65 = 0;
  m_first = v67.m_query_output->m_list_geom_info.m_first;
  v63 = m_first;
  v62 = 0;
  while ( v62 != v63 )
  {
    v60 = v63;
    if ( v63->m_ent_info )
    {
      v59 = 0;
      if ( v60->m_ent_info->m_ent_type )
      {
        if ( v60->m_ent_info->m_ent_type == ET_CENT
          && (gjk_entity_info_t::get_cent(v60->m_ent_info)->nextState.eType == 17
           || gjk_entity_info_t::get_cent(v60->m_ent_info)->nextState.eType == 1) )
        {
          v59 = 1;
        }
      }
      else
      {
        v59 = gjk_entity_info_t::get_gent(v60->m_ent_info)->sentient != 0;
      }
      if ( v59 && v60->m_cg->get_type(v60->m_cg) == 7 )
      {
        v58 = pushRadius + 15.0;
        m_cg = v60->m_cg;
        v56 = m_cg[1].m_aabb_mn_loc.w + m_cg[1].m_aabb_mn_loc.y;
        xform = gjk_geom_info_t::get_xform(v60);
        p_x = &xform->m_mat.w.x;
        v53 = v68.x - xform->m_mat.w.x;
        v52 = v68.y - xform->m_mat.w.y;
        v51 = v68.z - xform->m_mat.w.z;
        v48 = v53;
        v49 = v52;
        v50 = *(float *)&FLOAT_0_0;
        v47 = (float)((float)(v48 * v48) + (float)(v49 * v49)) + (float)(0.0 * 0.0);
        v46 = (float)((float)(pushRadius + 15.0) + v56) + v88;
        v45 = v46 * v46;
        if ( (float)(v46 * v46) > v47 && v47 > 0.001 )
        {
          v44 = fsqrt(v47);
          v43 = 1.0 / v44;
          v42 = (float)(1.0 / v44) * v48;
          v41 = (float)(1.0 / v44) * v49;
          v40 = (float)(1.0 / v44) * v50;
          v37 = v42;
          v38 = v41;
          v39 = v40;
          v36 = phys_static_array<geom_plane,128>::add(&v66, 0, "phys array add overflow.");
          v36->m_normal.x = v42;
          v36->m_normal.y = v38;
          v36->m_normal.z = v39;
          v35 = LODWORD(v58) ^ _mask__NegFloat_;
          v34 = COERCE_FLOAT(LODWORD(v58) ^ _mask__NegFloat_) * v37;
          v33 = COERCE_FLOAT(LODWORD(v58) ^ _mask__NegFloat_) * v38;
          v32 = COERCE_FLOAT(LODWORD(v58) ^ _mask__NegFloat_) * v39;
          v29 = v34;
          v30 = v33;
          v31 = v32;
          p_m_arm = &v36->m_arm;
          v36->m_arm.x = v34;
          p_m_arm->y = v30;
          p_m_arm->z = v31;
          v36->m_d = (float)((float)((float)(v36->m_normal.x * *p_x) + (float)(v36->m_normal.y * p_x[1]))
                           + (float)(v36->m_normal.z * p_x[2]))
                   + v56;
          v36->m_lambda = *(float *)&FLOAT_0_0;
          v36->m_right_side = (float)(v36->m_d
                                    - (float)((float)((float)(v68.x * v36->m_normal.x) + (float)(v68.y * v36->m_normal.y))
                                            + (float)(v68.z * v36->m_normal.z)))
                            + v58;
          v36->m_active = 1;
          v36->m_walkable = 0;
          v36->m_no_push_out = 0;
          v65 = 1;
        }
      }
    }
    m_next_link = v63->m_next_link;
    v63 = m_next_link;
  }
  if ( v65 )
  {
    project((int)v92, &PHYS_ZERO_VEC_1, &v66, &v27);
    v26 = (float)((float)(v27.x * v27.x) + (float)(v27.y * v27.y)) + (float)(v27.z * v27.z);
    if ( v26 > (float)(pushRadius * pushRadius) )
    {
      v25 = fsqrt(v26);
      v24 = pushRadius / v25;
      v27.x = v27.x * (float)(pushRadius / v25);
      v27.y = v27.y * (float)(pushRadius / v25);
      v27.z = v27.z * (float)(pushRadius / v25);
    }
    v23.__vftable = (player_push_slide_move_input_t_vtbl *)&player_push_slide_move_input_t::`vftable';
    setup_player_push_slide_move_input((int)v92, &v23, pm, pml, &v27);
    gjk_slide_move(gjkcc_in, &v23, &v22);
    *origin_ = v22.new_position[0];
    origin_[1] = v22.new_position[1];
    origin_[2] = v22.new_position[2];
    m_slot_array = v66.m_slot_array;
    v20 = v66.m_slot_array;
    v19 = &v66.m_slot_array[v66.m_alloc_count];
    v18 = v19;
    while ( v18 != v20 )
    {
      v17 = v20;
      if ( v20->m_right_side > 0.0 )
        v17->m_right_side = *(float *)&FLOAT_0_0;
      ++v20;
    }
    v15 = Phys_Vec3ToNitrousVec(&v16, pm->ps->velocity);
    v27.x = v15->x;
    v27.y = v15->y;
    v27.z = v15->z;
    project((int)v92, &v27, &v66, &v27);
    Phys_NitrousVecToVec3(&v27, pm->ps->velocity);
  }
  for ( i = 0; i < v66.m_alloc_count; ++i )
    ;
}

// local variable allocation has failed, the output may be wrong!
void  render_gjkcc_collision(float a1@<ebp>, float (*mins)[3], float (*maxs)[3], float (*origin)[3])
{
  const phys_vec3 *v4; // eax
  const phys_vec3 *v5; // eax
  const phys_vec3 *v6; // eax
  const phys_vec3 *v7; // eax
  const phys_vec3 *v8; // eax
  const phys_vec3 *v9; // eax
  const phys_vec3 *v10; // eax
  const phys_vec3 *v11; // eax
  const phys_vec3 *v12; // eax
  const phys_vec3 *v13; // eax
  const phys_vec3 *v14; // eax
  const phys_vec3 *v15; // [esp-8h] [ebp-39Ch]
  const phys_vec3 *v16; // [esp-8h] [ebp-39Ch]
  phys_vec3 v17; // [esp+18h] [ebp-37Ch] BYREF
  phys_vec3 v18; // [esp+28h] [ebp-36Ch] BYREF
  phys_vec3 v19; // [esp+38h] [ebp-35Ch] BYREF
  phys_vec3 v20; // [esp+48h] [ebp-34Ch] BYREF
  phys_vec3 v21; // [esp+58h] [ebp-33Ch] BYREF
  phys_vec3 v22; // [esp+68h] [ebp-32Ch] BYREF
  phys_vec3 v23; // [esp+78h] [ebp-31Ch] BYREF
  phys_vec3 v24; // [esp+88h] [ebp-30Ch] BYREF
  phys_vec3 v25; // [esp+98h] [ebp-2FCh] BYREF
  phys_vec3 v26; // [esp+A8h] [ebp-2ECh] BYREF
  phys_vec3 v27; // [esp+B8h] [ebp-2DCh] BYREF
  phys_vec3 v28; // [esp+C8h] [ebp-2CCh] BYREF
  phys_vec3 v29; // [esp+D8h] [ebp-2BCh] BYREF
  phys_vec3 v30; // [esp+E8h] [ebp-2ACh] BYREF
  float v31; // [esp+FCh] [ebp-298h]
  float v32; // [esp+100h] [ebp-294h]
  float v33; // [esp+104h] [ebp-290h]
  phys_vec3 v34; // [esp+108h] [ebp-28Ch] BYREF
  float v35; // [esp+11Ch] [ebp-278h]
  float v36; // [esp+120h] [ebp-274h]
  float v37; // [esp+124h] [ebp-270h]
  float v38; // [esp+128h] [ebp-26Ch]
  float v39; // [esp+12Ch] [ebp-268h]
  float v40; // [esp+130h] [ebp-264h]
  float v41; // [esp+13Ch] [ebp-258h]
  float v42; // [esp+140h] [ebp-254h]
  float v43; // [esp+144h] [ebp-250h]
  float v44[3]; // [esp+148h] [ebp-24Ch] BYREF
  phys_vec3 rvec2; // [esp+154h] [ebp-240h]
  float v46; // [esp+164h] [ebp-230h]
  float v47[3]; // [esp+168h] [ebp-22Ch] BYREF
  phys_vec3 rvec1; // [esp+174h] [ebp-220h] BYREF
  float v49; // [esp+184h] [ebp-210h] BYREF
  int v50; // [esp+188h] [ebp-20Ch]
  float co2; // [esp+18Ch] [ebp-208h] BYREF
  float si2; // [esp+190h] [ebp-204h] BYREF
  int next_i; // [esp+194h] [ebp-200h]
  float co1; // [esp+198h] [ebp-1FCh] BYREF
  float si1; // [esp+19Ch] [ebp-1F8h]
  int i; // [esp+1A0h] [ebp-1F4h]
  int v57; // [esp+1B4h] [ebp-1E0h]
  float v58; // [esp+1B8h] [ebp-1DCh] BYREF
  float v59; // [esp+1BCh] [ebp-1D8h]
  float v60; // [esp+1C0h] [ebp-1D4h]
  float v61; // [esp+1D4h] [ebp-1C0h]
  float v62[3]; // [esp+1D8h] [ebp-1BCh] BYREF
  phys_vec3 foot_pos; // [esp+1E4h] [ebp-1B0h]
  float v64; // [esp+1F4h] [ebp-1A0h]
  int v65; // [esp+1F8h] [ebp-19Ch]
  int v66; // [esp+1FCh] [ebp-198h]
  float v67; // [esp+200h] [ebp-194h]
  float v68; // [esp+214h] [ebp-180h]
  float v69[3]; // [esp+218h] [ebp-17Ch] BYREF
  phys_vec3 head_pos; // [esp+224h] [ebp-170h]
  float v71; // [esp+234h] [ebp-160h]
  int v72; // [esp+238h] [ebp-15Ch]
  int v73; // [esp+23Ch] [ebp-158h]
  float v74; // [esp+240h] [ebp-154h]
  float w; // [esp+250h] [ebp-144h]
  float v76; // [esp+254h] [ebp-140h]
  float v77; // [esp+258h] [ebp-13Ch] BYREF
  float v78; // [esp+25Ch] [ebp-138h]
  float radius; // [esp+260h] [ebp-134h]
  phys_vec3 pos; // [esp+264h] [ebp-130h]
  float v81; // [esp+274h] [ebp-120h]
  int *p_m_contents; // [esp+278h] [ebp-11Ch]
  float *p_y; // [esp+27Ch] [ebp-118h]
  float v84[2]; // [esp+280h] [ebp-114h] BYREF
  float mx[3]; // [esp+28Ch] [ebp-108h] BYREF
  float mn[3]; // [esp+298h] [ebp-FCh] BYREF
  phys_vec3 aabb_min; // [esp+2A4h] [ebp-F0h] BYREF
  _BYTE aabb_max_4[76]; // [esp+2B8h] [ebp-DCh] OVERLAPPED BYREF
  phys_vec3 position; // [esp+304h] [ebp-90h] BYREF
  gjk_polygon_cylinder_t gjk_geom; // [esp+314h] [ebp-80h] BYREF
  int retaddr; // [esp+394h] [ebp+0h]

  gjk_geom.m_foot_offset = a1;
  gjk_geom.m_mode = retaddr;
  gjk_base_t::gjk_base_t((gjk_base_t *)&position.y);
  LODWORD(position.y) = &gjk_polygon_cylinder_t::`vftable';
  setup_gjk_polygon_cylinder(
    COERCE_FLOAT((gjk_polygon_cylinder_t *)&gjk_geom.m_foot_offset),
    (float *)mins,
    (float *)maxs,
    0.5,
    (gjk_polygon_cylinder_t *)&position.y);
  Phys_Vec3ToNitrousVec((const phys_vec3 *)&aabb_max_4[64], (float *)origin);
  if ( render_bounding_box )
  {
    memcpy(aabb_max_4, &PHYS_IDENTITY_MATRIX_1, 0x40u);
    phys_vec3::operator=((phys_vec3 *)&aabb_max_4[48], (const phys_vec3 *)&aabb_max_4[64]);
    gjk_polygon_cylinder_t::calc_aabb(
      (gjk_polygon_cylinder_t *)&position.y,
      (int)&gjk_geom.m_foot_offset,
      (const phys_mat44 *)aabb_max_4,
      (phys_vec3 *)mn,
      (phys_vec3 *)&aabb_min.y);
    Phys_NitrousVecToVec3((const phys_vec3 *)mn, mx);
    Phys_NitrousVecToVec3((phys_vec3 *)&aabb_min.y, v84);
    render_box(COERCE_FLOAT((gjk_polygon_cylinder_t *)&gjk_geom.m_foot_offset), mx, v84, colorRed, 0);
  }
  p_y = &position.y;
  p_m_contents = &gjk_geom.m_contents;
  v81 = *(float *)&gjk_geom.m_contents + *(float *)&aabb_max_4[64];
  pos.w = *((float *)&gjk_geom.m_contents + 1) + *(float *)&aabb_max_4[68];
  pos.z = *((float *)&gjk_geom.m_contents + 2) + *(float *)&aabb_max_4[72];
  v77 = *(float *)&gjk_geom.m_contents + *(float *)&aabb_max_4[64];
  v78 = *((float *)&gjk_geom.m_contents + 1) + *(float *)&aabb_max_4[68];
  radius = *((float *)&gjk_geom.m_contents + 2) + *(float *)&aabb_max_4[72];
  v76 = gjk_geom.m_center.y + gjk_geom.m_polygon_cylinder_radius;
  w = gjk_geom.m_center.w;
  v72 = *(unsigned int *)&FLOAT_0_0;
  v73 = *(unsigned int *)&FLOAT_0_0;
  v74 = gjk_geom.m_center.w;
  v71 = (float)(*(float *)&gjk_geom.m_contents + *(float *)&aabb_max_4[64]) + 0.0;
  head_pos.w = (float)(*((float *)&gjk_geom.m_contents + 1) + *(float *)&aabb_max_4[68]) + 0.0;
  head_pos.z = (float)(*((float *)&gjk_geom.m_contents + 2) + *(float *)&aabb_max_4[72]) + gjk_geom.m_center.w;
  v69[0] = v71;
  v69[1] = head_pos.w;
  v69[2] = head_pos.z;
  v68 = gjk_geom.m_center.w;
  v65 = *(unsigned int *)&FLOAT_0_0;
  v66 = *(unsigned int *)&FLOAT_0_0;
  v67 = gjk_geom.m_center.w;
  v64 = (float)(*(float *)&gjk_geom.m_contents + *(float *)&aabb_max_4[64]) - 0.0;
  foot_pos.w = (float)(*((float *)&gjk_geom.m_contents + 1) + *(float *)&aabb_max_4[68]) - 0.0;
  foot_pos.z = (float)(*((float *)&gjk_geom.m_contents + 2) + *(float *)&aabb_max_4[72]) - gjk_geom.m_center.w;
  v62[0] = v64;
  v62[1] = foot_pos.w;
  v62[2] = foot_pos.z;
  v61 = gjk_geom.m_center.w - gjk_geom.m_capsule_radius;
  v58 = *(float *)&FLOAT_0_0;
  v59 = *(float *)&FLOAT_0_0;
  v60 = gjk_geom.m_center.w - gjk_geom.m_capsule_radius;
  *(float *)&v57 = COERCE_FLOAT(LODWORD(gjk_geom.m_center.w) ^ _mask__NegFloat_) + gjk_geom.m_half_height;
  co1 = *(float *)&FLOAT_0_0;
  si1 = *(float *)&FLOAT_0_0;
  i = v57;
  for ( next_i = 0; next_i < 12; ++next_i )
  {
    gjk_polygon_cylinder_t::poly_verts::get_co_si(&gjk_polygon_cylinder_t::s_poly_verts, next_i, &co2, &si2);
    v50 = (next_i + 1) % 12;
    gjk_polygon_cylinder_t::poly_verts::get_co_si(&gjk_polygon_cylinder_t::s_poly_verts, v50, &rvec1.w, &v49);
    rvec1.z = v76 * co2;
    rvec1.y = v76 * si2;
    v47[0] = v76 * co2;
    v47[1] = v76 * si2;
    v47[2] = *(float *)&FLOAT_0_0;
    v46 = v76 * rvec1.w;
    rvec2.w = v76 * v49;
    v44[0] = v76 * rvec1.w;
    v44[1] = v76 * v49;
    v44[2] = *(float *)&FLOAT_0_0;
    v43 = v77 + co1;
    v42 = v78 + si1;
    v41 = radius + *(float *)&i;
    v38 = v77 + co1;
    v39 = v78 + si1;
    v40 = radius + *(float *)&i;
    v37 = (float)(v77 + co1) + (float)(v76 * co2);
    v36 = (float)(v78 + si1) + (float)(v76 * si2);
    v35 = (float)(radius + *(float *)&i) + 0.0;
    v34.x = v37;
    v34.y = v36;
    v34.z = v35;
    v33 = v77 + v58;
    v32 = v78 + v59;
    v31 = radius + v60;
    v30.x = v77 + v58;
    v30.y = v78 + v59;
    v30.z = radius + v60;
    v4 = operator+(&v29, &v30, (const phys_vec3 *)v47);
    render_line(v4, &v34, colorBlue, 0, 0);
    v5 = operator+(&v28, (const phys_vec3 *)&v77, (const phys_vec3 *)&v58);
    v15 = operator+(&v27, v5, (const phys_vec3 *)v44);
    v6 = operator+(&v26, (const phys_vec3 *)&v77, (const phys_vec3 *)&v58);
    v7 = operator+(&v25, v6, (const phys_vec3 *)v47);
    render_line(v7, v15, colorBlue, 0, 0);
    v8 = operator+(&v24, (const phys_vec3 *)&v77, (const phys_vec3 *)&co1);
    v16 = operator+(&v23, v8, (const phys_vec3 *)v44);
    v9 = operator+(&v22, (const phys_vec3 *)&v77, (const phys_vec3 *)&co1);
    v10 = operator+(&v21, v9, (const phys_vec3 *)v47);
    render_line(v10, v16, colorBlue, 0, 0);
    v11 = operator+(&v20, (const phys_vec3 *)&v77, (const phys_vec3 *)&v58);
    v12 = operator+(&v19, v11, (const phys_vec3 *)v47);
    render_line(v12, (const phys_vec3 *)v69, colorBlue, 0, 0);
    v13 = operator+(&v18, (const phys_vec3 *)&v77, (const phys_vec3 *)&co1);
    v14 = operator+(&v17, v13, (const phys_vec3 *)v47);
    render_line(v14, (const phys_vec3 *)v62, colorBlue, 0, 0);
  }
  gjk_base_t::~gjk_base_t((gjk_base_t *)&position.y);
}

phys_vec3 *__cdecl operator+(phys_vec3 *result, const phys_vec3 *a, const phys_vec3 *b)
{
  float v4; // [esp+4h] [ebp-8h]
  float v5; // [esp+8h] [ebp-4h]

  v4 = a->y + b->y;
  v5 = a->z + b->z;
  result->x = a->x + b->x;
  result->y = v4;
  result->z = v5;
  return result;
}

void __thiscall phys_gjk_geom::set_simplex(
        phys_gjk_geom *this,
        const phys_vec3 *simplex_inds,
        int w_set,
        const phys_vec3 *normal,
        cached_simplex_info *cache_info)
{
  const phys_vec3 *v5; // [esp+4h] [ebp-Ch]
  int i; // [esp+8h] [ebp-8h]

  for ( i = 0; i < 4; ++i )
  {
    if ( (w_set & (1 << i)) != 0 )
    {
      v5 = &simplex_inds[i];
      cache_info->m_indices[0].x = v5->x;
      cache_info->m_indices[0].y = v5->y;
      cache_info->m_indices[0].z = v5->z;
      cache_info = (cached_simplex_info *)((char *)cache_info + 16);
    }
  }
}

bool __thiscall phys_gjk_geom::ray_cast(
        phys_gjk_geom *this,
        const phys_vec3 *__formal,
        const phys_vec3 *ray_dir,
        float t_input,
        float *t_output,
        phys_vec3 *hitn)
{
  return 0;
}

gjk_base_t *__thiscall gjk_base_t::gjk_base_t(gjk_base_t *this)
{
  this->__vftable = (gjk_base_t_vtbl *)&phys_gjk_geom::`vftable';
  this->__vftable = (gjk_base_t_vtbl *)&gjk_base_t::`vftable';
  this->m_flags = 0;
  return this;
}

double __thiscall phys_gjk_geom::get_geom_radius(phys_gjk_geom *this)
{
  return 0.0;
}

void __thiscall gjk_base_t::~gjk_base_t(gjk_base_t *this)
{
  this->__vftable = (gjk_base_t_vtbl *)&gjk_base_t::`vftable';
  if ( (this->m_flags & 1) != 0 )
  {
    if ( _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
           87,
           "!get_flag(FLAG_TEMP_ALLOCATION)",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
  }
}

void __thiscall gjk_base_t::comp_aabb_loc(gjk_base_t *this)
{
  if ( (this->m_flags & 2) == 0 )
  {
    this->m_flags |= 2u;
    this->calc_aabb(this, &PHYS_IDENTITY_MATRIX_1, &this->m_aabb_mn_loc, &this->m_aabb_mx_loc);
  }
}

const cbrush_t *__thiscall gjk_base_t::get_brush(gjk_base_t *this)
{
  return 0;
}

bool __thiscall gjk_base_t::is_foot(gjk_base_t *this, const phys_vec3 *hit_point)
{
  return 0;
}

bool __thiscall gjk_base_t::is_walkable(gjk_base_t *this, const phys_vec3 *hit_point, const phys_vec3 *up)
{
  return 0;
}

void __thiscall gjk_polygon_cylinder_t::poly_verts::get_co_si(
        gjk_polygon_cylinder_t::poly_verts *this,
        int i,
        float *co_,
        float *si_)
{
  int ii; // [esp+8h] [ebp-4h]

  if ( (unsigned int)i >= 0xC
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
          802,
          0,
          "%s",
          "i >= 0 && i < NUM_VERTS_") )
  {
    __debugbreak();
  }
  ii = `gjk_polygon_cylinder_t::poly_verts::get_co_si'::`2'::a[i / 3]
     + i * `gjk_polygon_cylinder_t::poly_verts::get_co_si'::`2'::b[i / 3];
  *co_ = `gjk_polygon_cylinder_t::poly_verts::get_co_si'::`2'::cos[i / 3] * this->m_co[ii];
  *si_ = `gjk_polygon_cylinder_t::poly_verts::get_co_si'::`2'::sis[i / 3] * this->m_si[ii];
}

void __userpurge gjk_polygon_cylinder_t::support(
        gjk_polygon_cylinder_t *this@<ecx>,
        int a2@<ebp>,
        const phys_vec3 *v,
        phys_vec3 *support_vert,
        phys_vec3 *support_ind)
{
  float v5; // [esp-Ch] [ebp-11Ch]
  float v6; // [esp-8h] [ebp-118h]
  float v7; // [esp-4h] [ebp-114h]
  float v8; // [esp+14h] [ebp-FCh]
  float v9; // [esp+18h] [ebp-F8h]
  float v10; // [esp+1Ch] [ebp-F4h]
  float v11; // [esp+3Ch] [ebp-D4h]
  float v12; // [esp+74h] [ebp-9Ch]
  float v13; // [esp+78h] [ebp-98h]
  float v14; // [esp+7Ch] [ebp-94h]
  float v15; // [esp+A4h] [ebp-6Ch]
  float v16; // [esp+A8h] [ebp-68h]
  float v17; // [esp+ACh] [ebp-64h]
  float v18; // [esp+E4h] [ebp-2Ch] BYREF
  float v19; // [esp+E8h] [ebp-28h] BYREF
  int v20; // [esp+ECh] [ebp-24h]
  float co_; // [esp+F0h] [ebp-20h]
  float si_; // [esp+F4h] [ebp-1Ch]
  int foot_or_head_ind; // [esp+F8h] [ebp-18h]
  float foot_or_head_z; // [esp+FCh] [ebp-14h]
  gjk_polygon_cylinder_t *disc_z; // [esp+100h] [ebp-10h]
  int v26; // [esp+104h] [ebp-Ch]
  int best_i; // [esp+108h] [ebp-8h]
  int retaddr; // [esp+110h] [ebp+0h]

  v26 = a2;
  best_i = retaddr;
  disc_z = this;
  if ( this->m_mode )
  {
    if ( disc_z->m_half_height < disc_z->m_capsule_radius
      && !Assert_MyHandler(
            "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
            859,
            0,
            "%s",
            "m_half_height >= m_capsule_radius") )
    {
      __debugbreak();
    }
    v11 = disc_z->m_half_height - disc_z->m_capsule_radius;
    v8 = disc_z->m_center.x + 0.0;
    v9 = disc_z->m_center.y + 0.0;
    v10 = disc_z->m_center.z + v11;
    v5 = disc_z->m_center.x - 0.0;
    v6 = disc_z->m_center.y - 0.0;
    v7 = disc_z->m_center.z - v11;
    if ( (float)((float)((float)((float)(v8 - v5) * v->x) + (float)((float)(v9 - v6) * v->y))
               + (float)((float)(v10 - v7) * v->z)) < 0.0 )
    {
      LODWORD(support_ind->x) = 1;
      support_vert->x = v5;
      support_vert->y = v6;
      support_vert->z = v7;
    }
    else
    {
      support_ind->x = 0.0;
      support_vert->x = v8;
      support_vert->y = v9;
      support_vert->z = v10;
    }
  }
  else
  {
    foot_or_head_z = COERCE_FLOAT(gjk_polygon_cylinder_t::poly_verts::support(&gjk_polygon_cylinder_t::s_poly_verts, v));
    foot_or_head_ind = SLODWORD(v->z);
    if ( *(float *)&foot_or_head_ind < 0.0 )
    {
      LODWORD(support_ind->x) = LODWORD(foot_or_head_z) + 12;
      si_ = COERCE_FLOAT(LODWORD(disc_z->m_half_height) ^ _mask__NegFloat_) + disc_z->m_foot_offset;
      LODWORD(co_) = LODWORD(disc_z->m_half_height) ^ _mask__NegFloat_;
      v20 = 25;
    }
    else
    {
      support_ind->x = foot_or_head_z;
      si_ = disc_z->m_half_height - disc_z->m_head_offset;
      co_ = disc_z->m_half_height;
      v20 = 24;
    }
    gjk_polygon_cylinder_t::poly_verts::get_co_si(
      &gjk_polygon_cylinder_t::s_poly_verts,
      SLODWORD(foot_or_head_z),
      &v18,
      &v19);
    v15 = (float)(disc_z->m_polygon_cylinder_radius * v18) + disc_z->m_center.x;
    v16 = (float)(disc_z->m_polygon_cylinder_radius * v19) + disc_z->m_center.y;
    v17 = si_ + disc_z->m_center.z;
    v12 = disc_z->m_center.x + 0.0;
    v13 = disc_z->m_center.y + 0.0;
    v14 = co_ + disc_z->m_center.z;
    if ( (float)((float)((float)((float)(v15 - v12) * v->x) + (float)((float)(v16 - v13) * v->y))
               + (float)((float)(v17 - v14) * v->z)) < 0.0 )
    {
      LODWORD(support_ind->x) = v20;
      support_vert->x = v12;
      support_vert->y = v13;
      support_vert->z = v14;
    }
    else
    {
      support_vert->x = v15;
      support_vert->y = v16;
      support_vert->z = v17;
    }
  }
}

int __thiscall gjk_polygon_cylinder_t::poly_verts::support(
        gjk_polygon_cylinder_t::poly_verts *this,
        const phys_vec3 *v)
{
  double v2; // xmm0_8
  float v3; // xmm0_4
  long double thisa; // [esp+0h] [ebp-1Ch]
  long double v6; // [esp+8h] [ebp-14h]
  float at; // [esp+Ch] [ebp-10h]

  LODWORD(thisa) = this;
  *(float *)&v6 = v->x;
  HIDWORD(thisa) = LODWORD(v->y);
  v2 = *((float *)&thisa + 1);
  __libm_sse2_atan2(thisa, v6);
  v3 = v2;
  at = v3;
  if ( v3 < 0.0 )
    at = (float)(2.0 * 3.1415927) + v3;
  if ( (unsigned int)((int)(float)((float)((float)(12.0 * at) / (float)(2.0 * 3.1415927)) + 0.5) % 12) >= 0xC
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
          791,
          0,
          "%s",
          "best_i >= 0 && best_i < NUM_VERTS_") )
  {
    __debugbreak();
  }
  return (int)(float)((float)((float)(12.0 * at) / (float)(2.0 * 3.1415927)) + 0.5) % 12;
}

void __userpurge gjk_polygon_cylinder_t::get_simplex(
        gjk_polygon_cylinder_t *this@<ecx>,
        int a2@<ebp>,
        const cached_simplex_info *cache_info,
        int index_count,
        phys_vec3 *simplex_verts,
        phys_vec3 *simplex_inds)
{
  phys_vec3 *v6; // eax
  phys_vec3 *v7; // eax
  phys_vec3 *v8; // edx
  float v9; // [esp-60h] [ebp-1A8h]
  float v10; // [esp-5Ch] [ebp-1A4h]
  float v11; // [esp-30h] [ebp-178h]
  float v12; // [esp-2Ch] [ebp-174h]
  float x; // [esp-Ch] [ebp-154h]
  int j; // [esp-8h] [ebp-150h]
  float v15; // [esp+4h] [ebp-144h]
  float v16; // [esp+20h] [ebp-128h]
  float v17; // [esp+24h] [ebp-124h]
  phys_vec3 *v18; // [esp+58h] [ebp-F0h]
  float v19; // [esp+60h] [ebp-E8h]
  float v20; // [esp+64h] [ebp-E4h]
  float v21; // [esp+A0h] [ebp-A8h]
  float v22; // [esp+A4h] [ebp-A4h]
  float v23; // [esp+DCh] [ebp-6Ch] BYREF
  float v24[2]; // [esp+E0h] [ebp-68h] BYREF
  phys_vec3 *v25; // [esp+E8h] [ebp-60h]
  float v26; // [esp+ECh] [ebp-5Ch]
  int ind1; // [esp+F0h] [ebp-58h]
  float v28; // [esp+F4h] [ebp-54h]
  float v29; // [esp+FCh] [ebp-4Ch]
  int v30; // [esp+100h] [ebp-48h]
  float v31; // [esp+104h] [ebp-44h]
  int p_m_center; // [esp+108h] [ebp-40h]
  float v33; // [esp+10Ch] [ebp-3Ch]
  float v34; // [esp+110h] [ebp-38h]
  float v35; // [esp+114h] [ebp-34h]
  float v36; // [esp+11Ch] [ebp-2Ch]
  float v37; // [esp+120h] [ebp-28h]
  float v38; // [esp+124h] [ebp-24h]
  float v39; // [esp+128h] [ebp-20h] BYREF
  float v40; // [esp+12Ch] [ebp-1Ch] BYREF
  float v41; // [esp+130h] [ebp-18h]
  int co_; // [esp+134h] [ebp-14h]
  gjk_polygon_cylinder_t *si_; // [esp+138h] [ebp-10h]
  int ind; // [esp+13Ch] [ebp-Ch]
  int i; // [esp+140h] [ebp-8h]
  int retaddr; // [esp+148h] [ebp+0h]

  ind = a2;
  i = retaddr;
  si_ = this;
  if ( this->m_mode )
  {
    if ( si_->m_half_height < si_->m_capsule_radius
      && !Assert_MyHandler(
            "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
            911,
            0,
            "%s",
            "m_half_height >= m_capsule_radius") )
    {
      __debugbreak();
    }
    v15 = si_->m_half_height - si_->m_capsule_radius;
    for ( j = 0; j < index_count; ++j )
    {
      x = cache_info->m_indices[j].x;
      if ( LODWORD(x) >= 2
        && _tlAssert(
             "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
             916,
             "ind == 0 || ind == 1",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
      simplex_inds[j].x = x;
      if ( x == 0.0 )
      {
        v11 = si_->m_center.y + 0.0;
        v12 = si_->m_center.z + v15;
        v8 = &simplex_verts[j];
        v8->x = si_->m_center.x + 0.0;
        v8->y = v11;
        v8->z = v12;
      }
      else
      {
        v9 = si_->m_center.y - 0.0;
        v10 = si_->m_center.z - v15;
        simplex_verts[j].x = si_->m_center.x - 0.0;
        simplex_verts[j].y = v9;
        simplex_verts[j].z = v10;
      }
    }
  }
  else
  {
    for ( co_ = 0; co_ < index_count; ++co_ )
    {
      v41 = cache_info->m_indices[co_].x;
      if ( v41 < 0.0
        && _tlAssert("c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h", 883, "ind >= 0", &toastPopupTitle) )
      {
        __debugbreak();
      }
      simplex_inds[co_].x = v41;
      if ( SLODWORD(v41) >= 12 )
      {
        if ( SLODWORD(v41) >= 24 )
        {
          if ( LODWORD(v41) == 24 )
          {
            v19 = si_->m_center.y + 0.0;
            v20 = si_->m_half_height + si_->m_center.z;
            v18 = &simplex_verts[co_];
            v18->x = si_->m_center.x + 0.0;
            v18->y = v19;
            v18->z = v20;
          }
          else
          {
            if ( LODWORD(v41) != 25 )
            {
              if ( _tlAssert(
                     "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
                     904,
                     "ind == 2 * s_poly_verts.get_num_verts() + 1",
                     &toastPopupTitle) )
              {
                __debugbreak();
              }
            }
            v16 = si_->m_center.y + 0.0;
            v17 = COERCE_FLOAT(LODWORD(si_->m_half_height) ^ _mask__NegFloat_) + si_->m_center.z;
            v7 = &simplex_verts[co_];
            v7->x = si_->m_center.x + 0.0;
            v7->y = v16;
            v7->z = v17;
          }
        }
        else
        {
          LODWORD(v24[1]) = LODWORD(v41) - 12;
          gjk_polygon_cylinder_t::poly_verts::get_co_si(
            &gjk_polygon_cylinder_t::s_poly_verts,
            LODWORD(v41) - 12,
            &v23,
            v24);
          v21 = (float)(si_->m_polygon_cylinder_radius * v24[0]) + si_->m_center.y;
          v22 = (float)(COERCE_FLOAT(LODWORD(si_->m_half_height) ^ _mask__NegFloat_) + si_->m_foot_offset)
              + si_->m_center.z;
          v6 = &simplex_verts[co_];
          v6->x = (float)(si_->m_polygon_cylinder_radius * v23) + si_->m_center.x;
          v6->y = v21;
          v6->z = v22;
        }
      }
      else
      {
        gjk_polygon_cylinder_t::poly_verts::get_co_si(&gjk_polygon_cylinder_t::s_poly_verts, SLODWORD(v41), &v39, &v40);
        v38 = si_->m_polygon_cylinder_radius * v39;
        v37 = si_->m_polygon_cylinder_radius * v40;
        v36 = si_->m_half_height - si_->m_head_offset;
        v33 = v38;
        v34 = v37;
        v35 = v36;
        p_m_center = (int)&si_->m_center;
        v31 = v38 + si_->m_center.x;
        *(float *)&v30 = v37 + si_->m_center.y;
        v29 = v36 + si_->m_center.z;
        v26 = v31;
        ind1 = v30;
        v28 = v29;
        v25 = &simplex_verts[co_];
        v25->x = v31;
        LODWORD(v25->y) = ind1;
        v25->z = v28;
      }
    }
  }
}

void __userpurge gjk_polygon_cylinder_t::calc_aabb(
        gjk_polygon_cylinder_t *this@<ecx>,
        int a2@<ebp>,
        const phys_mat44 *xform,
        phys_vec3 *aabb_min,
        phys_vec3 *aabb_max)
{
  const phys_vec3 *v5; // eax
  float v6; // [esp+10h] [ebp-188h]
  float v7; // [esp+14h] [ebp-184h]
  float v8; // [esp+50h] [ebp-148h]
  float v9; // [esp+54h] [ebp-144h]
  float m_geom_radius; // [esp+8Ch] [ebp-10Ch]
  float v11; // [esp+ACh] [ebp-ECh] BYREF
  float v12; // [esp+B0h] [ebp-E8h]
  float v13; // [esp+B4h] [ebp-E4h]
  phys_vec3 center; // [esp+B8h] [ebp-E0h] BYREF
  float v15; // [esp+CCh] [ebp-CCh]
  float v16; // [esp+D0h] [ebp-C8h]
  float v17; // [esp+D4h] [ebp-C4h]
  float v18; // [esp+DCh] [ebp-BCh]
  float v19; // [esp+E0h] [ebp-B8h]
  float v20; // [esp+E4h] [ebp-B4h]
  const phys_vec3 *v21; // [esp+E8h] [ebp-B0h]
  phys_vec3 v22; // [esp+ECh] [ebp-ACh] BYREF
  float v23; // [esp+FCh] [ebp-9Ch]
  float v24; // [esp+100h] [ebp-98h]
  float v25; // [esp+104h] [ebp-94h]
  float v26; // [esp+10Ch] [ebp-8Ch]
  float v27; // [esp+110h] [ebp-88h]
  float v28; // [esp+114h] [ebp-84h]
  const phys_vec3 *v29; // [esp+118h] [ebp-80h]
  phys_vec3 v30; // [esp+11Ch] [ebp-7Ch] BYREF
  unsigned int v31[3]; // [esp+12Ch] [ebp-6Ch] BYREF
  float v32; // [esp+148h] [ebp-50h]
  float v33; // [esp+14Ch] [ebp-4Ch]
  float v34; // [esp+150h] [ebp-48h]
  float v35; // [esp+154h] [ebp-44h]
  phys_vec3 *v36; // [esp+158h] [ebp-40h]
  unsigned int v37[3]; // [esp+15Ch] [ebp-3Ch] BYREF
  phys_vec3 top; // [esp+168h] [ebp-30h]
  float v39; // [esp+178h] [ebp-20h]
  float v40; // [esp+17Ch] [ebp-1Ch]
  float m_half_height; // [esp+180h] [ebp-18h]
  phys_vec3 *p_z; // [esp+184h] [ebp-14h]
  gjk_polygon_cylinder_t *v43; // [esp+188h] [ebp-10h]
  int v44; // [esp+18Ch] [ebp-Ch]
  void *v45; // [esp+190h] [ebp-8h]
  void *retaddr; // [esp+198h] [ebp+0h]

  v44 = a2;
  v45 = retaddr;
  v43 = this;
  gjk_polygon_cylinder_t::calc_disc_aabb(&xform->z, this->m_polygon_cylinder_radius, aabb_min, aabb_max);
  p_z = &xform->z;
  m_half_height = v43->m_half_height;
  v40 = m_half_height * xform->z.x;
  v39 = m_half_height * xform->z.y;
  top.w = m_half_height * xform->z.z;
  *(float *)v37 = v40;
  *(float *)&v37[1] = v39;
  v37[2] = LODWORD(top.w);
  v36 = &xform->z;
  LODWORD(v35) = LODWORD(v43->m_half_height) ^ _mask__NegFloat_;
  v34 = v35 * xform->z.x;
  v33 = v35 * xform->z.y;
  v32 = v35 * xform->z.z;
  *(float *)v31 = v34;
  *(float *)&v31[1] = v33;
  *(float *)&v31[2] = v32;
  v29 = phys_min(&v30, (const phys_vec3 *)v37, (const phys_vec3 *)v31);
  v28 = aabb_min->x + v29->x;
  v27 = aabb_min->y + v29->y;
  v26 = aabb_min->z + v29->z;
  v23 = v28;
  v24 = v27;
  v25 = v26;
  aabb_min->x = v28;
  aabb_min->y = v24;
  aabb_min->z = v25;
  v21 = phys_max(&v22, (const phys_vec3 *)v37, (const phys_vec3 *)v31);
  v20 = aabb_max->x + v21->x;
  v19 = aabb_max->y + v21->y;
  v18 = aabb_max->z + v21->z;
  v15 = v20;
  v16 = v19;
  v17 = v18;
  aabb_max->x = v20;
  aabb_max->y = v16;
  aabb_max->z = v17;
  if ( v43->m_half_height < v43->m_head_offset
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
          961,
          0,
          "%s",
          "m_head_offset <= m_half_height") )
  {
    __debugbreak();
  }
  if ( v43->m_half_height < v43->m_foot_offset
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
          962,
          0,
          "%s",
          "m_foot_offset <= m_half_height") )
  {
    __debugbreak();
  }
  if ( v43->m_polygon_cylinder_radius < v43->m_capsule_radius
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
          972,
          0,
          "%s",
          "m_capsule_radius <= m_polygon_cylinder_radius") )
  {
    __debugbreak();
  }
  v5 = phys_multiply((phys_vec3 *)&center.y, xform, &v43->m_center);
  operator+((phys_vec3 *)&v11, v5, &xform->w);
  m_geom_radius = v43->m_geom_radius;
  v8 = (float)(aabb_min->y + v12) - m_geom_radius;
  v9 = (float)(aabb_min->z + v13) - m_geom_radius;
  aabb_min->x = (float)(aabb_min->x + v11) - m_geom_radius;
  aabb_min->y = v8;
  aabb_min->z = v9;
  v6 = (float)(aabb_max->y + v12) + m_geom_radius;
  v7 = (float)(aabb_max->z + v13) + m_geom_radius;
  aabb_max->x = (float)(aabb_max->x + v11) + m_geom_radius;
  aabb_max->y = v6;
  aabb_max->z = v7;
}

const phys_vec3 *__cdecl phys_min(const phys_vec3 *result, const phys_vec3 *v1, const phys_vec3 *v2)
{
  float x; // [esp+0h] [ebp-24h]
  float y; // [esp+4h] [ebp-20h]
  float z; // [esp+8h] [ebp-1Ch]

  if ( v2->z < v1->z )
    z = v2->z;
  else
    z = v1->z;
  if ( v2->y < v1->y )
    y = v2->y;
  else
    y = v1->y;
  if ( v2->x < v1->x )
    x = v2->x;
  else
    x = v1->x;
  result->x = x;
  result->y = y;
  result->z = z;
  return result;
}

const phys_vec3 *__cdecl phys_max(const phys_vec3 *result, const phys_vec3 *v1, const phys_vec3 *v2)
{
  float x; // [esp+0h] [ebp-24h]
  float y; // [esp+4h] [ebp-20h]
  float z; // [esp+8h] [ebp-1Ch]

  if ( v1->z < v2->z )
    z = v2->z;
  else
    z = v1->z;
  if ( v1->y < v2->y )
    y = v2->y;
  else
    y = v1->y;
  if ( v1->x < v2->x )
    x = v2->x;
  else
    x = v1->x;
  result->x = x;
  result->y = y;
  result->z = z;
  return result;
}

void __cdecl gjk_polygon_cylinder_t::calc_disc_aabb(
        const phys_vec3 *axis,
        float radius,
        phys_vec3 *aabb_min,
        phys_vec3 *aabb_max)
{
  float v4; // [esp-88h] [ebp-A8h]
  float v5; // [esp-84h] [ebp-A4h]
  float v6; // [esp-20h] [ebp-40h]
  float v7; // [esp-14h] [ebp-34h]
  float v8; // [esp-10h] [ebp-30h]

  v8 = axis->x * axis->x;
  v7 = axis->y * axis->y;
  v6 = axis->z * axis->z;
  aabb_max->x = radius * fsqrt(v7 + v6);
  aabb_max->y = radius * fsqrt(v8 + v6);
  aabb_max->z = radius * fsqrt(v8 + v7);
  LODWORD(v4) = LODWORD(aabb_max->y) ^ _mask__NegFloat_;
  LODWORD(v5) = LODWORD(aabb_max->z) ^ _mask__NegFloat_;
  LODWORD(aabb_min->x) = LODWORD(aabb_max->x) ^ _mask__NegFloat_;
  aabb_min->y = v4;
  aabb_min->z = v5;
}

unsigned int __thiscall gjk_polygon_cylinder_t::get_type(gjk_polygon_cylinder_t *this)
{
  return 7;
}

double __thiscall gjk_polygon_cylinder_t::get_geom_radius(gjk_polygon_cylinder_t *this)
{
  if ( this->m_mode )
    return this->m_geom_radius + this->m_capsule_radius;
  else
    return this->m_geom_radius;
}

void __cdecl render_gjkcc_collision(pmove_t *pm)
{
  int savedregs; // [esp+0h] [ebp+0h] BYREF

  if ( render_player_collision->current.integer == 1 )
    render_gjkcc_collision(
      COERCE_FLOAT(&savedregs),
      (float (*)[3])pm->mins,
      (float (*)[3])pm->maxs,
      (float (*)[3])pm->ps->origin);
}

void  PM_gjk_ground_trace(
        int a1@<ebp>,
        const gjkcc_input_t *gjkcc_in,
        trace_t *results,
        float *start,
        float *mins,
        float *maxs,
        float *end,
        int passEntityNum,
        int contentMask,
        float *first_hit_time)
{
  bool v10; // [esp-Bh] [ebp-109h]
  float v11; // [esp-Ah] [ebp-108h]
  float v12; // [esp-6h] [ebp-104h]
  const gjk_trace_output_t *i; // [esp-2h] [ebp-100h]
  bool hit_time_3; // [esp+5h] [ebp-F9h]
  float m_hit_time; // [esp+6h] [ebp-F8h]
  gjk_trace_output_t *gto; // [esp+Ah] [ebp-F4h]
  float v17; // [esp+Eh] [ebp-F0h]
  gjk_trace_output_t *first_hit_timea; // [esp+12h] [ebp-ECh] BYREF
  gjk_trace_output_t *first_gto; // [esp+16h] [ebp-E8h]
  float first_depth; // [esp+1Ah] [ebp-E4h]
  list_gjk_trace_output list; // [esp+1Eh] [ebp-E0h]
  gjk_query_output **p_m_query_output; // [esp+2Eh] [ebp-D0h]
  float depth[2]; // [esp+32h] [ebp-CCh] BYREF
  char v24; // [esp+3Bh] [ebp-C3h]
  gjk_trace_input_t gti; // [esp+3Eh] [ebp-C0h] BYREF
  float v26; // [esp+EEh] [ebp-10h]
  unsigned int v27[2]; // [esp+F2h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+FEh] [ebp+0h]

  v27[0] = a1;
  v27[1] = retaddr;
  v26 = FLOAT_0_5;
  p_m_query_output = &gti.m_query_output;
  list.m_first_hit = (gjk_trace_output_t *)&gti.m_query_input.m_proximity_data;
  gti.m_query_input.m_proximity_data = 0;
  gti.m_query_input.m_proximity_mask = (int)&gti.m_query_input.m_proximity_data;
  gti.m_query_input.m_gjk_query_flags = 0;
  setup_trace_info((int)v27, gjkcc_in, start, mins, maxs, end, passEntityNum, contentMask, (gjk_trace_input_t *)depth);
  v24 = 0;
  *(float *)&list.m_list.m_alloc_count = start[2] - end[2];
  if ( *(float *)&list.m_list.m_alloc_count < 0.0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp", 1799, 0, "%s", "depth >= 0.0f") )
  {
    __debugbreak();
  }
  if ( v26 <= *(float *)&list.m_list.m_alloc_count )
    *(float *)&list.m_list.m_last_next_ptr = v26 / *(float *)&list.m_list.m_alloc_count;
  else
    *(float *)&list.m_list.m_last_next_ptr = FLOAT_1_0;
  gti.m_cg = (const gjk_base_t *)list.m_list.m_last_next_ptr;
  first_hit_timea = 0;
  first_gto = (gjk_trace_output_t *)&first_hit_timea;
  first_depth = 0.0;
  list.m_list.m_first = 0;
  gjk_trace((int)v27, (const gjk_trace_input_t *)depth, (list_gjk_trace_output *)&first_hit_timea);
  if ( list.m_list.m_first )
  {
    if ( first_hit_time )
      *first_hit_time = list.m_list.m_first->m_hit_time;
    v17 = (float)((float)(end[2] - start[2]) * list.m_list.m_first->m_hit_time) + start[2];
    gto = first_hit_timea;
    m_hit_time = first_hit_timea->m_hit_time;
    hit_time_3 = is_walkable((int)v27, first_hit_timea);
    for ( i = gto->m_next_link; i; i = i->m_next_link )
    {
      v12 = (float)((float)(end[2] - start[2]) * i->m_hit_time) + start[2];
      if ( v17 < v12
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp",
              1816,
              0,
              "%s",
              "depth <= first_depth") )
      {
        __debugbreak();
      }
      v11 = i->m_hit_time;
      if ( i->m_gi->m_cg->get_type(i->m_gi->m_cg) == 3 )
        v11 = v11 - 0.001;
      if ( v26 >= (float)(v17 - v12) )
      {
        if ( hit_time_3 )
        {
          if ( m_hit_time > v11 && is_walkable((int)v27, i) )
          {
            m_hit_time = v11;
            gto = (gjk_trace_output_t *)i;
          }
        }
        else
        {
          v10 = is_walkable((int)v27, i);
          if ( m_hit_time > v11 || v10 )
          {
            m_hit_time = v11;
            gto = (gjk_trace_output_t *)i;
            hit_time_3 = v10;
          }
        }
      }
    }
    fill_results(gto, hit_time_3, results);
  }
  else
  {
    fill_results_no_hit(results);
  }
}

double __cdecl PM_PermuteRestrictiveClipPlanes(
        const float *velocity,
        int planeCount,
        const float (*planes)[3],
        int *permutation)
{
  int permutedIndex; // [esp+8h] [ebp-28h]
  float parallel[8]; // [esp+Ch] [ebp-24h]
  int planeIndex; // [esp+2Ch] [ebp-4h]

  if ( !velocity
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp", 1924, 0, "%s", "velocity") )
  {
    __debugbreak();
  }
  if ( (planeCount <= 0 || planeCount > 8)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp",
          1925,
          0,
          "%s\n\t(planeCount) = %i",
          "(planeCount > 0 && planeCount <= 8)",
          planeCount) )
  {
    __debugbreak();
  }
  if ( !planes
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp", 1926, 0, "%s", "planes") )
  {
    __debugbreak();
  }
  if ( !permutation
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp", 1927, 0, "%s", "permutation") )
  {
    __debugbreak();
  }
  for ( planeIndex = 0; planeIndex < planeCount; ++planeIndex )
  {
    parallel[planeIndex] = (float)((float)(*velocity * (float)(*planes)[3 * planeIndex])
                                 + (float)(velocity[1] * (float)(*planes)[3 * planeIndex + 1]))
                         + (float)(velocity[2] * (float)(*planes)[3 * planeIndex + 2]);
    for ( permutedIndex = planeIndex;
          permutedIndex && parallel[planeIndex] <= parallel[permutation[permutedIndex - 1]];
          --permutedIndex )
    {
      permutation[permutedIndex] = permutation[permutedIndex - 1];
    }
    permutation[permutedIndex] = planeIndex;
  }
  return parallel[*permutation];
}

void __cdecl setup_player_gjk_slide_move_input(
        player_gjk_slide_move_input_t *input,
        pmove_t *pm,
        pml_t *pml,
        int gravity)
{
  bool v4; // [esp+0h] [ebp-4h]

  input->clientnum = pm->ps->clientNum;
  v4 = !gravity && pml->groundPlane && (pm->ps->pm_flags & 8) == 0;
  input->do_step_down = v4;
  input->frametime = pml->frametime;
  input->gravity = pm->ps->gravity;
  input->has_gravity = gravity;
  input->maxs = (const float (*)[3])pm->maxs;
  input->mins = (const float (*)[3])pm->mins;
  input->position = (const float (*)[3])pm->ps->origin;
  input->tracemask = pm->tracemask;
  input->velocity = (const float (*)[3])pm->ps->velocity;
  input->pm = pm;
}

void __cdecl PM_StepSlideMove(pmove_t *pm, pml_t *pml, int gravity)
{
  float *v3; // [esp+10h] [ebp-150h]
  float *v4; // [esp+14h] [ebp-14Ch]
  float *v5; // [esp+30h] [ebp-130h]
  float *v6; // [esp+38h] [ebp-128h]
  float *v7; // [esp+3Ch] [ebp-124h]
  float *v8; // [esp+48h] [ebp-118h]
  float *v9; // [esp+4Ch] [ebp-114h]
  float *v10; // [esp+50h] [ebp-110h]
  float *v11; // [esp+54h] [ebp-10Ch]
  float *v12; // [esp+58h] [ebp-108h]
  float *v13; // [esp+5Ch] [ebp-104h]
  float *velocity; // [esp+64h] [ebp-FCh]
  float *origin; // [esp+68h] [ebp-F8h]
  float v16; // [esp+70h] [ebp-F0h]
  float v17; // [esp+74h] [ebp-ECh]
  int old; // [esp+88h] [ebp-D8h]
  float fSpeedScale; // [esp+94h] [ebp-CCh]
  int iDelta; // [esp+9Ch] [ebp-C4h]
  int iDeltaa; // [esp+9Ch] [ebp-C4h]
  float flatDelta; // [esp+A4h] [ebp-BCh]
  float flatDelta_4; // [esp+A8h] [ebp-B8h]
  float down_v; // [esp+B4h] [ebp-ACh]
  float down_v_4; // [esp+B8h] [ebp-A8h]
  float down_v_8; // [esp+BCh] [ebp-A4h]
  int jumping; // [esp+C0h] [ebp-A0h]
  int iBumps; // [esp+C8h] [ebp-98h]
  float start_o[3]; // [esp+CCh] [ebp-94h] BYREF
  trace_t trace; // [esp+D8h] [ebp-88h] BYREF
  float endpos[3]; // [esp+114h] [ebp-4Ch] BYREF
  float start_v[3]; // [esp+120h] [ebp-40h] BYREF
  float down_o[3]; // [esp+12Ch] [ebp-34h] BYREF
  float up[3]; // [esp+138h] [ebp-28h] BYREF
  int bHadGround; // [esp+144h] [ebp-1Ch]
  float down[3]; // [esp+148h] [ebp-18h] BYREF
  playerState_s *ps; // [esp+154h] [ebp-Ch]
  float fStepSize; // [esp+158h] [ebp-8h] BYREF
  float fStepAmount; // [esp+15Ch] [ebp-4h]
  int savedregs; // [esp+160h] [ebp+0h] BYREF

  memset(&trace, 0, 16);
  fStepAmount = *(float *)&FLOAT_0_0;
  ps = pm->ps;
  if ( !ps && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp", 2239, 0, "%s", "ps") )
    __debugbreak();
  jumping = 0;
  if ( (ps->pm_flags & 8) != 0 )
  {
    bHadGround = 0;
    Jump_ClearState(ps);
  }
  else if ( pml->groundPlane )
  {
    bHadGround = 1;
  }
  else
  {
    bHadGround = 0;
    if ( (ps->pm_flags & 0x4000) != 0 && ps->pm_time )
      Jump_ClearState(ps);
  }
  start_o[0] = ps->origin[0];
  start_o[1] = ps->origin[1];
  start_o[2] = ps->origin[2];
  start_v[0] = ps->velocity[0];
  start_v[1] = ps->velocity[1];
  start_v[2] = ps->velocity[2];
  iBumps = PM_SlideMove(pm, pml, gravity);
  if ( (ps->pm_flags & 1) != 0 )
    fStepSize = FLOAT_10_0;
  else
    fStepSize = FLOAT_18_0;
  if ( ps->groundEntityNum != 1023 )
    goto LABEL_27;
  if ( (ps->pm_flags & 0x4000) != 0 && ps->pm_time )
    Jump_ClearState(ps);
  if ( iBumps && (ps->pm_flags & 0x4000) != 0 && Jump_GetStepHeight(ps, start_o, &fStepSize) )
  {
    if ( fStepSize < 1.0 )
      return;
    jumping = 1;
  }
  if ( jumping || (ps->pm_flags & 8) != 0 && ps->velocity[2] > 0.0 )
  {
LABEL_27:
    down_o[0] = ps->origin[0];
    down_o[1] = ps->origin[1];
    down_o[2] = ps->origin[2];
    down_v = ps->velocity[0];
    down_v_4 = ps->velocity[1];
    down_v_8 = ps->velocity[2];
    flatDelta = down_o[0] - start_o[0];
    flatDelta_4 = down_o[1] - start_o[1];
    if ( iBumps || pml->groundPlane && pml->groundTrace.normal.vec.v[2] < 0.89999998 )
    {
      up[0] = start_o[0];
      up[1] = start_o[1];
      up[2] = (float)(fStepSize + 1.0) + start_o[2];
      PM_playerTrace(pm, &trace, start_o, pm->mins, pm->maxs, up, ps->clientNum, pm->tracemask);
      fStepAmount = (float)((float)(fStepSize + 1.0) * trace.fraction) - 1.0;
      if ( fStepAmount >= 1.0 )
      {
        origin = ps->origin;
        v16 = up[1];
        v17 = start_o[2] + fStepAmount;
        ps->origin[0] = up[0];
        origin[1] = v16;
        origin[2] = v17;
        velocity = ps->velocity;
        ps->velocity[0] = start_v[0];
        velocity[1] = start_v[1];
        velocity[2] = start_v[2];
        PM_SlideMove(pm, pml, gravity);
      }
      else
      {
        fStepAmount = *(float *)&FLOAT_0_0;
      }
    }
    if ( bHadGround || fStepAmount != 0.0 )
    {
      down[0] = ps->origin[0];
      down[1] = ps->origin[1];
      down[2] = ps->origin[2];
      down[2] = down[2] - fStepAmount;
      if ( bHadGround )
        down[2] = down[2] - 9.0;
      if ( phys_player_collision_mode->current.integer == 1 )
        PM_gjk_ground_trace(
          (int)&savedregs,
          pm->m_gjkcc_input,
          &trace,
          ps->origin,
          pm->mins,
          pm->maxs,
          down,
          ps->clientNum,
          pm->tracemask,
          0);
      else
        PM_playerTrace(pm, &trace, ps->origin, pm->mins, pm->maxs, down, ps->clientNum, pm->tracemask);
      if ( Trace_GetEntityHitId(&trace) < com_maxclients->current.integer )
      {
        v13 = ps->origin;
        ps->origin[0] = down_o[0];
        v13[1] = down_o[1];
        v13[2] = down_o[2];
        v12 = ps->velocity;
        ps->velocity[0] = down_v;
        v12[1] = down_v_4;
        v12[2] = down_v_8;
        return;
      }
      if ( trace.fraction >= 1.0 )
      {
        if ( fStepAmount != 0.0 )
          ps->origin[2] = ps->origin[2] - fStepAmount;
      }
      else
      {
        if ( phys_player_collision_mode->current.integer == 1 )
        {
          if ( !trace.walkable )
          {
            v11 = ps->origin;
            ps->origin[0] = down_o[0];
            v11[1] = down_o[1];
            v11[2] = down_o[2];
            v10 = ps->velocity;
            ps->velocity[0] = down_v;
            v10[1] = down_v_4;
            v10[2] = down_v_8;
            return;
          }
        }
        else if ( !trace.walkable && trace.normal.vec.v[2] < 0.30000001 )
        {
          v9 = ps->origin;
          ps->origin[0] = down_o[0];
          v9[1] = down_o[1];
          v9[2] = down_o[2];
          v8 = ps->velocity;
          ps->velocity[0] = down_v;
          v8[1] = down_v_4;
          v8[2] = down_v_8;
          return;
        }
        Vec3Lerp(ps->origin, down, trace.fraction, ps->origin);
        PM_ProjectVelocity(ps->velocity, trace.normal.vec.v, ps->velocity);
      }
    }
    if ( (float)((float)((float)(flatDelta * start_v[0]) + (float)(flatDelta_4 * start_v[1])) + 0.001) >= (float)((float)((float)(ps->origin[0] - start_o[0]) * start_v[0]) + (float)((float)(ps->origin[1] - start_o[1]) * start_v[1]))
      || jumping && Jump_IsPlayerAboveMax(ps) )
    {
      v7 = ps->origin;
      ps->origin[0] = down_o[0];
      v7[1] = down_o[1];
      v7[2] = down_o[2];
      v6 = ps->velocity;
      ps->velocity[0] = down_v;
      v6[1] = down_v_4;
      v6[2] = down_v_8;
      fStepAmount = *(float *)&FLOAT_0_0;
      if ( bHadGround )
      {
        down[0] = ps->origin[0];
        down[1] = ps->origin[1];
        down[2] = ps->origin[2];
        down[2] = down[2] - 9.0;
        PM_playerTrace(pm, &trace, ps->origin, pm->mins, pm->maxs, down, ps->clientNum, pm->tracemask);
        if ( trace.fraction < 1.0 )
        {
          Vec3Lerp(ps->origin, down, trace.fraction, endpos);
          fStepAmount = endpos[2] - ps->origin[2];
          v5 = ps->origin;
          ps->origin[0] = endpos[0];
          v5[1] = endpos[1];
          v5[2] = endpos[2];
          if ( phys_player_collision_mode->current.integer == 1 )
            PM_ProjectVelocity(ps->velocity, trace.normal.vec.v, ps->velocity);
          else
            PM_ClipVelocity(ps->velocity, trace.normal.vec.v, ps->velocity);
        }
      }
    }
    if ( jumping )
      Jump_ClampVelocity(ps, down_o);
    if ( bHadGround )
    {
      if ( ps->pm_type < 9 )
      {
        if ( PM_VerifyPronePosition(pm, start_o, start_v) )
        {
          if ( COERCE_FLOAT(COERCE_UNSIGNED_INT(ps->origin[2] - down_o[2]) & _mask__AbsFloat_) > 0.5 )
          {
            iDelta = (int)((float)(ps->origin[2] - down_o[2]) + 9.313225746154785e-10);
            if ( iDelta )
            {
              if ( pm->viewChangeTime < ps->commandTime || pm->viewChangeTime > ps->commandTime + 1000 )
              {
                pm->viewChange = (float)(ps->origin[2] - down_o[2]) + pm->viewChange;
                pm->viewChangeTime = ps->commandTime;
              }
              fSpeedScale = (float)(1.0 - 0.80000001)
                          + (float)((float)(1.0
                                          - (float)(COERCE_FLOAT(COERCE_UNSIGNED_INT(ps->origin[2] - start_o[2]) & _mask__AbsFloat_)
                                                  / fStepSize))
                                  * 0.80000001);
              v3 = ps->velocity;
              v4 = ps->velocity;
              ps->velocity[0] = fSpeedScale * ps->velocity[0];
              v3[1] = fSpeedScale * v4[1];
              v3[2] = fSpeedScale * v4[2];
              pm->xyspeed = Vec2Length(ps->velocity);
              if ( (int)abs(iDelta) > 3 && ps->groundEntityNum != 1023 && PM_ShouldMakeFootsteps(pm) )
              {
                iDeltaa = (int)abs(iDelta) / 2;
                if ( iDeltaa > 4 )
                  iDeltaa = 4;
                old = ps->bobCycle;
                ps->bobCycle = (unsigned __int8)(int)(float)((float)old + (float)((float)((float)iDeltaa * 1.25) + 7.0));
                PM_FootstepEvent(pm, pml, old, ps->bobCycle, 1);
              }
            }
          }
        }
      }
    }
  }
}

int __cdecl PM_VerifyPronePosition(pmove_t *pm, float *vFallbackOrg, float *vFallbackVel)
{
  unsigned __int8 v4; // al
  float save_v; // [esp+44h] [ebp-20h]
  float save_v_4; // [esp+48h] [ebp-1Ch]
  float save_v_8; // [esp+4Ch] [ebp-18h]
  float save_o; // [esp+50h] [ebp-14h]
  float save_o_4; // [esp+54h] [ebp-10h]
  float save_o_8; // [esp+58h] [ebp-Ch]
  int bProneOK; // [esp+5Ch] [ebp-8h]
  playerState_s *ps; // [esp+60h] [ebp-4h]
  int savedregs; // [esp+64h] [ebp+0h] BYREF

  ps = pm->ps;
  if ( !pm->ps && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp", 1871, 0, "%s", "ps") )
    __debugbreak();
  if ( return_true )
    return 1;
  if ( (ps->pm_flags & 1) == 0 )
    return 1;
  v4 = BG_CheckProne(
         (cStaticModel_s *)&savedregs,
         ps,
         ps->clientNum,
         ps->origin,
         15.0,
         30.0,
         ps->proneDirection,
         &ps->fTorsoPitch,
         &ps->fWaistPitch,
         1,
         1,
         1,
         pm->handler,
         PCT_CLIENT,
         50.0);
  bProneOK = v4;
  if ( !v4 )
  {
    save_o = ps->origin[0];
    save_o_4 = ps->origin[1];
    save_o_8 = ps->origin[2];
    save_v = ps->velocity[0];
    save_v_4 = ps->velocity[1];
    save_v_8 = ps->velocity[2];
    ps->origin[0] = *vFallbackOrg;
    ps->origin[1] = vFallbackOrg[1];
    ps->origin[2] = vFallbackOrg[2];
    ps->velocity[0] = *vFallbackVel;
    ps->velocity[1] = vFallbackVel[1];
    ps->velocity[2] = vFallbackVel[2];
    if ( BG_CheckProne(
           (cStaticModel_s *)&savedregs,
           ps,
           ps->clientNum,
           ps->origin,
           15.0,
           30.0,
           ps->proneDirection,
           &ps->fTorsoPitch,
           &ps->fWaistPitch,
           1,
           1,
           1,
           pm->handler,
           PCT_CLIENT,
           50.0) )
    {
      return 0;
    }
    else
    {
      ps->origin[0] = save_o;
      ps->origin[1] = save_o_4;
      ps->origin[2] = save_o_8;
      ps->velocity[0] = save_v;
      ps->velocity[1] = save_v_4;
      ps->velocity[2] = save_v_8;
      return 1;
    }
  }
  return bProneOK;
}

int __cdecl PM_SlideMove(pmove_t *pm, pml_t *pml, int gravity)
{
  float *velocity; // edx
  unsigned __int16 EntityHitId; // ax
  float *v6; // [esp+8h] [ebp-1F4h]
  float *v7; // [esp+Ch] [ebp-1F0h]
  float *v8; // [esp+10h] [ebp-1ECh]
  float *v9; // [esp+14h] [ebp-1E8h]
  float *v10; // [esp+18h] [ebp-1E4h]
  float *v11; // [esp+30h] [ebp-1CCh]
  float *v12; // [esp+34h] [ebp-1C8h]
  float *v13; // [esp+38h] [ebp-1C4h]
  float *v14; // [esp+3Ch] [ebp-1C0h]
  float *v15; // [esp+44h] [ebp-1B8h]
  float *v16; // [esp+48h] [ebp-1B4h]
  float *v17; // [esp+4Ch] [ebp-1B0h]
  float *origin; // [esp+74h] [ebp-188h]
  player_gjk_slide_move_input_t input; // [esp+78h] [ebp-184h] BYREF
  int retv; // [esp+A8h] [ebp-154h]
  gjk_slide_move_output_t output; // [esp+ACh] [ebp-150h] BYREF
  const gjkcc_input_t *gjkcc_in; // [esp+C8h] [ebp-134h]
  int j; // [esp+CCh] [ebp-130h]
  float dir[3]; // [esp+D0h] [ebp-12Ch] BYREF
  float d; // [esp+DCh] [ebp-120h]
  int numbumps; // [esp+E0h] [ebp-11Ch]
  float endClipVelocity[3]; // [esp+E4h] [ebp-118h] BYREF
  int k; // [esp+F0h] [ebp-10Ch]
  float planes[8][3]; // [esp+F4h] [ebp-108h] BYREF
  int permutation[8]; // [esp+158h] [ebp-A4h] BYREF
  float time_left; // [esp+178h] [ebp-84h]
  float end[3]; // [esp+17Ch] [ebp-80h] BYREF
  int numplanes; // [esp+188h] [ebp-74h]
  int bumpcount; // [esp+18Ch] [ebp-70h]
  float primal_velocity[3]; // [esp+190h] [ebp-6Ch]
  trace_t trace; // [esp+19Ch] [ebp-60h] BYREF
  float endVelocity[3]; // [esp+1D8h] [ebp-24h] BYREF
  float clipVelocity[3]; // [esp+1E4h] [ebp-18h] BYREF
  int i; // [esp+1F0h] [ebp-Ch]
  playerState_s *ps; // [esp+1F4h] [ebp-8h]
  float into; // [esp+1F8h] [ebp-4h]

  if ( phys_player_collision_mode->current.integer == 1 )
  {
    input.__vftable = (player_gjk_slide_move_input_t_vtbl *)&player_gjk_slide_move_input_t::`vftable';
    setup_player_gjk_slide_move_input(&input, pm, pml, gravity);
    gjkcc_in = pm->m_gjkcc_input;
    retv = gjk_slide_move(gjkcc_in, &input, &output);
    origin = pm->ps->origin;
    *origin = output.new_position[0];
    origin[1] = output.new_position[1];
    origin[2] = output.new_position[2];
    velocity = pm->ps->velocity;
    *velocity = output.new_velocity[0];
    velocity[1] = output.new_velocity[1];
    velocity[2] = output.new_velocity[2];
    return retv;
  }
  else
  {
    memset(&trace, 0, 16);
    ps = pm->ps;
    if ( !ps && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_slidemove.cpp", 2031, 0, "%s", "ps") )
      __debugbreak();
    numbumps = 4;
    primal_velocity[0] = ps->velocity[0];
    primal_velocity[1] = ps->velocity[1];
    primal_velocity[2] = ps->velocity[2];
    endVelocity[0] = ps->velocity[0];
    endVelocity[1] = ps->velocity[1];
    endVelocity[2] = ps->velocity[2];
    if ( gravity )
    {
      endVelocity[2] = endVelocity[2] - (float)((float)ps->gravity * pml->frametime);
      ps->velocity[2] = (float)(ps->velocity[2] + endVelocity[2]) * 0.5;
      primal_velocity[2] = endVelocity[2];
      if ( pml->groundPlane )
        PM_ClipVelocity(ps->velocity, pml->groundTrace.normal.vec.v, ps->velocity);
    }
    time_left = pml->frametime;
    if ( pml->groundPlane )
    {
      *(_QWORD *)&planes[0][0] = *(_QWORD *)pml->groundTrace.normal.vec.v;
      LODWORD(planes[0][2]) = pml->groundTrace.normal.vec.u[2];
      numplanes = 1;
    }
    else
    {
      numplanes = 0;
    }
    Vec3NormalizeTo(ps->velocity, planes[numplanes++]);
    for ( bumpcount = 0; bumpcount < numbumps; ++bumpcount )
    {
      end[0] = (float)(time_left * ps->velocity[0]) + ps->origin[0];
      end[1] = (float)(time_left * ps->velocity[1]) + ps->origin[1];
      end[2] = (float)(time_left * ps->velocity[2]) + ps->origin[2];
      PM_playerTrace(pm, &trace, ps->origin, pm->mins, pm->maxs, end, ps->clientNum, pm->tracemask);
      if ( trace.allsolid )
      {
        ps->velocity[2] = *(float *)&FLOAT_0_0;
        return 1;
      }
      if ( trace.startsolid && do_push )
      {
        v16 = ps->origin;
        v17 = ps->origin;
        ps->origin[0] = ps->origin[0] + trace.normal.vec.v[0];
        v16[1] = v17[1] + trace.normal.vec.v[1];
        v16[2] = v17[2] + trace.normal.vec.v[2];
      }
      if ( trace.fraction > 0.0 )
        Vec3Lerp(ps->origin, end, trace.fraction, ps->origin);
      if ( trace.fraction == 1.0 )
        break;
      EntityHitId = Trace_GetEntityHitId(&trace);
      PM_AddTouchEnt(pm, EntityHitId);
      time_left = time_left - (float)(time_left * trace.fraction);
      if ( numplanes >= 8 )
      {
        v15 = ps->velocity;
        ps->velocity[0] = *(float *)&FLOAT_0_0;
        v15[1] = *(float *)&FLOAT_0_0;
        v15[2] = *(float *)&FLOAT_0_0;
        return 1;
      }
      for ( i = 0; i < numplanes; ++i )
      {
        if ( (float)((float)((float)(trace.normal.vec.v[0] * planes[i][0])
                           + (float)(trace.normal.vec.v[1] * planes[i][1]))
                   + (float)(trace.normal.vec.v[2] * planes[i][2])) > 0.99900001 )
        {
          PM_ClipVelocity(ps->velocity, trace.normal.vec.v, ps->velocity);
          v13 = ps->velocity;
          v14 = ps->velocity;
          ps->velocity[0] = trace.normal.vec.v[0] + ps->velocity[0];
          v13[1] = trace.normal.vec.v[1] + v14[1];
          v13[2] = trace.normal.vec.v[2] + v14[2];
          break;
        }
      }
      if ( i >= numplanes )
      {
        v12 = planes[numplanes];
        *(_QWORD *)v12 = *(_QWORD *)trace.normal.vec.v;
        v12[2] = trace.normal.vec.v[2];
        into = PM_PermuteRestrictiveClipPlanes(ps->velocity, ++numplanes, planes, permutation);
        if ( into < 0.1 )
        {
          if ( COERCE_FLOAT(LODWORD(into) ^ _mask__NegFloat_) > pml->impactSpeed )
            LODWORD(pml->impactSpeed) = LODWORD(into) ^ _mask__NegFloat_;
          PM_ClipVelocity(ps->velocity, planes[permutation[0]], clipVelocity);
          PM_ClipVelocity(endVelocity, planes[permutation[0]], endClipVelocity);
          for ( j = 1; j < numplanes; ++j )
          {
            v11 = planes[permutation[j]];
            if ( (float)((float)((float)(clipVelocity[0] * *v11) + (float)(clipVelocity[1] * v11[1]))
                       + (float)(clipVelocity[2] * v11[2])) < 0.1 )
            {
              PM_ClipVelocity(clipVelocity, planes[permutation[j]], clipVelocity);
              PM_ClipVelocity(endClipVelocity, planes[permutation[j]], endClipVelocity);
              if ( (float)((float)((float)(clipVelocity[0] * planes[permutation[0]][0])
                                 + (float)(clipVelocity[1] * planes[permutation[0]][1]))
                         + (float)(clipVelocity[2] * planes[permutation[0]][2])) < 0.0 )
              {
                Vec3Cross(planes[permutation[0]], planes[permutation[j]], dir);
                Vec3Normalize(dir);
                d = (float)((float)(dir[0] * ps->velocity[0]) + (float)(dir[1] * ps->velocity[1]))
                  + (float)(dir[2] * ps->velocity[2]);
                clipVelocity[0] = d * dir[0];
                clipVelocity[1] = d * dir[1];
                clipVelocity[2] = d * dir[2];
                d = (float)((float)(dir[0] * endVelocity[0]) + (float)(dir[1] * endVelocity[1]))
                  + (float)(dir[2] * endVelocity[2]);
                endClipVelocity[0] = d * dir[0];
                endClipVelocity[1] = d * dir[1];
                endClipVelocity[2] = d * dir[2];
                for ( k = 1; k < numplanes; ++k )
                {
                  if ( k != j )
                  {
                    v10 = planes[permutation[k]];
                    if ( (float)((float)((float)(clipVelocity[0] * *v10) + (float)(clipVelocity[1] * v10[1]))
                               + (float)(clipVelocity[2] * v10[2])) < 0.1 )
                    {
                      v9 = ps->velocity;
                      ps->velocity[0] = *(float *)&FLOAT_0_0;
                      v9[1] = *(float *)&FLOAT_0_0;
                      v9[2] = *(float *)&FLOAT_0_0;
                      return 1;
                    }
                  }
                }
              }
            }
          }
          v8 = ps->velocity;
          ps->velocity[0] = clipVelocity[0];
          v8[1] = clipVelocity[1];
          v8[2] = clipVelocity[2];
          endVelocity[0] = endClipVelocity[0];
          endVelocity[1] = endClipVelocity[1];
          endVelocity[2] = endClipVelocity[2];
        }
      }
    }
    if ( gravity )
    {
      v7 = ps->velocity;
      ps->velocity[0] = endVelocity[0];
      v7[1] = endVelocity[1];
      v7[2] = endVelocity[2];
    }
    if ( ps->pm_time )
    {
      v6 = ps->velocity;
      ps->velocity[0] = primal_velocity[0];
      v6[1] = primal_velocity[1];
      v6[2] = primal_velocity[2];
    }
    return bumpcount != 0;
  }
}

void __thiscall player_gjk_slide_move_input_t::custom_process(
        player_gjk_slide_move_input_t *this,
        gjk_trace_output_t *gto)
{
  unsigned __int16 EntityHitId; // ax
  unsigned int GlassHitId; // eax
  trace_t trace; // [esp+4h] [ebp-3Ch] BYREF

  memset(&trace, 0, 16);
  fill_results_type_and_id(gto, &trace);
  EntityHitId = Trace_GetEntityHitId(&trace);
  PM_AddTouchEnt(this->pm, EntityHitId);
  GlassHitId = Trace_GetGlassHitId(&trace);
  PM_AddTouchGlass(this->pm, GlassHitId);
}

void __thiscall phys_simple_allocator<phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal>::free(
        phys_simple_allocator<phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal> *this,
        phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *slot)
{
  if ( slot )
  {
    PMM_VALIDATE((char *)slot, 0x90u, 0x10u);
    --this->m_count;
    PMM_FREE((unsigned __int8 *)slot, 0x90u, 0x10u);
  }
}

void __thiscall phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::remove(
        phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor> *this,
        const phys_gjk_geom_id_pair_key *key)
{
  bool v2; // [esp+4h] [ebp-190h]
  bool v4; // [esp+Ch] [ebp-188h]
  phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal **m_node; // [esp+74h] [ebp-120h]
  phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::stack_item *v7; // [esp+78h] [ebp-11Ch]
  phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::stack_item *right_item; // [esp+7Ch] [ebp-118h]
  phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *replace_node; // [esp+80h] [ebp-114h]
  phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *root; // [esp+84h] [ebp-110h]
  phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::stack_item *next_item; // [esp+88h] [ebp-10Ch]
  phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::stack_item *cur_item; // [esp+8Ch] [ebp-108h]
  phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal **node_to_be_removed; // [esp+90h] [ebp-104h]
  phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::stack_item the_stack[32]; // [esp+94h] [ebp-100h] BYREF

  cur_item = the_stack;
  the_stack[0].m_node = &this->m_tree_root;
  while ( 1 )
  {
    root = *cur_item->m_node;
    if ( !root
      && _tlAssert("C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_avl_tree.h", 161, "root", &toastPopupTitle) )
    {
      __debugbreak();
    }
    if ( &cur_item[1] - the_stack >= 32
      && _tlAssert(
           "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_avl_tree.h",
           162,
           "cur_item + 1 - the_stack < 32",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    next_item = cur_item + 1;
    if ( key->m_id1 == root->m_key.m_id1 )
      v4 = key->m_id2 < root->m_key.m_id2;
    else
      v4 = key->m_id1 < root->m_key.m_id1;
    if ( v4 )
    {
      cur_item->m_child = -1;
      next_item->m_node = &root->m_avl_tree_node.m_left;
      goto LABEL_26;
    }
    if ( !(key->m_id1 == root->m_key.m_id1 ? root->m_key.m_id2 < key->m_id2 : root->m_key.m_id1 < key->m_id1) )
      break;
    cur_item->m_child = 1;
    next_item->m_node = &root->m_avl_tree_node.m_right;
LABEL_26:
    cur_item = next_item;
  }
  v2 = key->m_id1 == root->m_key.m_id1 && key->m_id2 == root->m_key.m_id2;
  if ( !v2
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_avl_tree.h",
         176,
         "key == accessor::get_avl_key(root)",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  node_to_be_removed = cur_item->m_node;
  if ( (*node_to_be_removed)->m_avl_tree_node.m_right )
  {
    cur_item->m_child = 1;
    ++cur_item;
    cur_item->m_node = &(*node_to_be_removed)->m_avl_tree_node.m_right;
    right_item = cur_item;
    while ( (*cur_item->m_node)->m_avl_tree_node.m_left )
    {
      if ( &cur_item[1] - the_stack >= 32 )
      {
        if ( _tlAssert(
               "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_avl_tree.h",
               191,
               "cur_item + 1 - the_stack < 32",
               &toastPopupTitle) )
        {
          __debugbreak();
        }
      }
      v7 = cur_item + 1;
      cur_item->m_child = -1;
      v7->m_node = &(*cur_item->m_node)->m_avl_tree_node.m_left;
      cur_item = v7;
    }
    replace_node = *cur_item->m_node;
    *cur_item->m_node = replace_node->m_avl_tree_node.m_right;
    replace_node->m_avl_tree_node.m_left = (*node_to_be_removed)->m_avl_tree_node.m_left;
    replace_node->m_avl_tree_node.m_right = (*node_to_be_removed)->m_avl_tree_node.m_right;
    replace_node->m_avl_tree_node.m_balance = (*node_to_be_removed)->m_avl_tree_node.m_balance;
    right_item->m_node = &replace_node->m_avl_tree_node.m_right;
    *node_to_be_removed = replace_node;
  }
  else
  {
    *node_to_be_removed = (*node_to_be_removed)->m_avl_tree_node.m_left;
  }
  do
  {
    if ( cur_item <= the_stack )
      break;
    --cur_item;
    m_node = cur_item->m_node;
    (*cur_item->m_node)->m_avl_tree_node.m_balance -= cur_item->m_child;
    if ( (*m_node)->m_avl_tree_node.m_balance == -2 )
    {
      if ( (*m_node)->m_avl_tree_node.m_left->m_avl_tree_node.m_balance == 1 )
        phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::rotate_left(
          this,
          &(*m_node)->m_avl_tree_node.m_left);
      phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::rotate_right(
        this,
        m_node);
      if ( (*m_node)->m_avl_tree_node.m_balance >= 2u
        && _tlAssert(
             "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_avl_tree.h",
             218,
             "accessor::get_avl_node(root)->m_balance == 0 || accessor::get_avl_node(root)->m_balance == +1",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
    }
    else if ( (*m_node)->m_avl_tree_node.m_balance == 2 )
    {
      if ( (*m_node)->m_avl_tree_node.m_right->m_avl_tree_node.m_balance == -1 )
        phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::rotate_right(
          this,
          &(*m_node)->m_avl_tree_node.m_right);
      phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::rotate_left(
        this,
        m_node);
      if ( (*m_node)->m_avl_tree_node.m_balance
        && (*m_node)->m_avl_tree_node.m_balance != -1
        && _tlAssert(
             "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_avl_tree.h",
             225,
             "accessor::get_avl_node(root)->m_balance == 0 || accessor::get_avl_node(root)->m_balance == -1",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
    }
  }
  while ( !(*m_node)->m_avl_tree_node.m_balance );
}

void __thiscall phys_link_list<gjk_geom_info_t>::add(phys_link_list<gjk_geom_info_t> *this, gjk_geom_info_t *p)
{
  if ( !this->m_last_next_ptr
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         230,
         "m_last_next_ptr",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  p->m_next_link = 0;
  ++this->m_alloc_count;
  *this->m_last_next_ptr = p;
  this->m_last_next_ptr = &p->m_next_link;
}

gjkcc_info *__thiscall phys_simple_allocator<gjkcc_info>::allocate(phys_simple_allocator<gjkcc_info> *this)
{
  char *slot; // [esp+38h] [ebp-4h]

  slot = PMM_ALLOC(0x200u, 0x10u);
  if ( !slot )
    return 0;
  ++this->m_count;
  *((unsigned int *)slot + 30) = 0;
  *((unsigned int *)slot + 31) = 0;
  gjk_query_output::gjk_query_output((gjk_query_output *)(slot + 144));
  return (gjkcc_info *)slot;
}

gjk_query_output *__thiscall gjk_query_output::gjk_query_output(gjk_query_output *this)
{
  this->__vftable = (gjk_query_output_vtbl *)&gjk_collision_visitor::`vftable';
  this->__vftable = (gjk_query_output_vtbl *)&gjk_query_output::`vftable';
  this->m_bpei_database.m_bpei_map.m_tree_root = 0;
  this->m_bpei_database.m_bpei_allocator.m_count = 0;
  this->m_bpei_database.m_mutex.m_count = 1;
  this->m_bpei_database.m_bpei_list = 0;
  this->m_allocator.m_first_block = 0;
  this->m_allocator.m_cur = 0;
  this->m_allocator.m_end = 0;
  this->m_allocator.m_total_memory_allocated = 0;
  this->m_allocator.m_mutex.m_count = 1;
  this->m_allocator.m_slot_pool = 0;
  this->m_list_geom_info.m_first = 0;
  this->m_list_geom_info.m_last_next_ptr = &this->m_list_geom_info.m_first;
  this->m_list_geom_info.m_alloc_count = 0;
  this->m_total_query_count = 0;
  this->m_total_cached_query_count = 0;
  gjk_query_output::reset_cache(this);
  gjk_query_output::accum_query_reset(this, &PHYS_ZERO_VEC_1);
  return this;
}

void __thiscall gjk_collision_visitor::get_local_query_aabb(
        gjk_collision_visitor *this,
        float *local_query_aabb_min,
        float *local_query_aabb_max)
{
  if ( !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h", 35, 0, "%s", "0") )
    __debugbreak();
}

char __thiscall gjk_collision_visitor::query_create_prolog_1(
        gjk_collision_visitor *this,
        const float *local_aabb_min,
        const float *local_aabb_max,
        const void *geom)
{
  return 1;
}

void __thiscall gjk_collision_visitor::query_create_epilog_1(gjk_collision_visitor *this, gjk_base_t *gjk_geom)
{
  ;
}

void __thiscall phys_simple_allocator<gjkcc_info>::free(phys_simple_allocator<gjkcc_info> *this, gjkcc_info *slot)
{
  if ( slot )
  {
    PMM_VALIDATE((char *)slot, 0x200u, 0x10u);
    --this->m_count;
    gjk_query_output::~gjk_query_output(&slot->m_gjk_query_output);
    phys_heap_gjk_cache_system_avl_tree::shutdown(&slot->m_gjk_cache);
    PMM_FREE((unsigned __int8 *)slot, 0x200u, 0x10u);
  }
}

void __thiscall phys_link_list<gjk_trace_output_t>::add(
        phys_link_list<gjk_trace_output_t> *this,
        gjk_trace_output_t *p)
{
  if ( !this->m_last_next_ptr
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         230,
         "m_last_next_ptr",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  p->m_next_link = 0;
  ++this->m_alloc_count;
  *this->m_last_next_ptr = p;
  this->m_last_next_ptr = &p->m_next_link;
}

geom_plane *__thiscall phys_static_array<geom_plane,128>::add(
        phys_static_array<geom_plane,128> *this,
        int no_error,
        const char *error_msg)
{
  if ( this->m_alloc_count < 128 )
  {
    return &this->m_slot_array[this->m_alloc_count++];
  }
  else
  {
    if ( !no_error )
      tlFatal(error_msg);
    return 0;
  }
}

geom_plane *__thiscall phys_static_array<geom_plane,128>::operator[](phys_static_array<geom_plane,128> *this, int i)
{
  if ( i < 0 || i >= this->m_alloc_count )
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
  return &this->m_slot_array[i];
}

void __thiscall gjk_query_output::~gjk_query_output(gjk_query_output *this)
{
  if ( this->m_allocator.m_first_block
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_transient_allocator.h",
         69,
         "m_first_block == NULL",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  bpei_database_t::purge_database(&this->m_bpei_database);
}

void __thiscall bpei_database_t::purge_database(bpei_database_t *this)
{
  broad_phase_environment_info *next_bpei; // [esp+154h] [ebp-8h]
  broad_phase_environment_info *bpei; // [esp+158h] [ebp-4h]

  for ( bpei = this->m_bpei_list; bpei; bpei = next_bpei )
  {
    next_bpei = bpei->m_next_bpei;
    phys_inplace_avl_tree<bpei_database_id,broad_phase_environment_info,broad_phase_environment_info::avl_tree_accessor>::remove(
      &this->m_bpei_map,
      &bpei->m_database_id);
    phys_simple_allocator<broad_phase_environment_info>::free(&this->m_bpei_allocator, bpei);
  }
  this->m_bpei_list = 0;
}

void __thiscall phys_inplace_avl_tree<bpei_database_id,broad_phase_environment_info,broad_phase_environment_info::avl_tree_accessor>::remove(
        phys_inplace_avl_tree<bpei_database_id,broad_phase_environment_info,broad_phase_environment_info::avl_tree_accessor> *this,
        const bpei_database_id *key)
{
  bool v2; // [esp+4h] [ebp-190h]
  bool v4; // [esp+Ch] [ebp-188h]
  broad_phase_environment_info **m_node; // [esp+74h] [ebp-120h]
  phys_inplace_avl_tree<bpei_database_id,broad_phase_environment_info,broad_phase_environment_info::avl_tree_accessor>::stack_item *v7; // [esp+78h] [ebp-11Ch]
  phys_inplace_avl_tree<bpei_database_id,broad_phase_environment_info,broad_phase_environment_info::avl_tree_accessor>::stack_item *right_item; // [esp+7Ch] [ebp-118h]
  broad_phase_environment_info *replace_node; // [esp+80h] [ebp-114h]
  broad_phase_environment_info *root; // [esp+84h] [ebp-110h]
  phys_inplace_avl_tree<bpei_database_id,broad_phase_environment_info,broad_phase_environment_info::avl_tree_accessor>::stack_item *next_item; // [esp+88h] [ebp-10Ch]
  phys_inplace_avl_tree<bpei_database_id,broad_phase_environment_info,broad_phase_environment_info::avl_tree_accessor>::stack_item *cur_item; // [esp+8Ch] [ebp-108h]
  broad_phase_environment_info **node_to_be_removed; // [esp+90h] [ebp-104h]
  phys_inplace_avl_tree<bpei_database_id,broad_phase_environment_info,broad_phase_environment_info::avl_tree_accessor>::stack_item the_stack[32]; // [esp+94h] [ebp-100h] BYREF

  cur_item = the_stack;
  the_stack[0].m_node = &this->m_tree_root;
  while ( 1 )
  {
    root = *cur_item->m_node;
    if ( !root
      && _tlAssert("C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_avl_tree.h", 161, "root", &toastPopupTitle) )
    {
      __debugbreak();
    }
    if ( &cur_item[1] - the_stack >= 32
      && _tlAssert(
           "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_avl_tree.h",
           162,
           "cur_item + 1 - the_stack < 32",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    next_item = cur_item + 1;
    if ( key->m_id1 == root->m_database_id.m_id1 )
      v4 = key->m_id2 < root->m_database_id.m_id2;
    else
      v4 = key->m_id1 < root->m_database_id.m_id1;
    if ( v4 )
    {
      cur_item->m_child = -1;
      next_item->m_node = &root->m_avl_tree_node.m_left;
      goto LABEL_26;
    }
    if ( !(key->m_id1 == root->m_database_id.m_id1
         ? root->m_database_id.m_id2 < key->m_id2
         : root->m_database_id.m_id1 < key->m_id1) )
      break;
    cur_item->m_child = 1;
    next_item->m_node = &root->m_avl_tree_node.m_right;
LABEL_26:
    cur_item = next_item;
  }
  v2 = key->m_id1 == root->m_database_id.m_id1 && key->m_id2 == root->m_database_id.m_id2;
  if ( !v2
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_avl_tree.h",
         176,
         "key == accessor::get_avl_key(root)",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  node_to_be_removed = cur_item->m_node;
  if ( (*node_to_be_removed)->m_avl_tree_node.m_right )
  {
    cur_item->m_child = 1;
    ++cur_item;
    cur_item->m_node = &(*node_to_be_removed)->m_avl_tree_node.m_right;
    right_item = cur_item;
    while ( (*cur_item->m_node)->m_avl_tree_node.m_left )
    {
      if ( &cur_item[1] - the_stack >= 32 )
      {
        if ( _tlAssert(
               "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_avl_tree.h",
               191,
               "cur_item + 1 - the_stack < 32",
               &toastPopupTitle) )
        {
          __debugbreak();
        }
      }
      v7 = cur_item + 1;
      cur_item->m_child = -1;
      v7->m_node = &(*cur_item->m_node)->m_avl_tree_node.m_left;
      cur_item = v7;
    }
    replace_node = *cur_item->m_node;
    *cur_item->m_node = replace_node->m_avl_tree_node.m_right;
    replace_node->m_avl_tree_node.m_left = (*node_to_be_removed)->m_avl_tree_node.m_left;
    replace_node->m_avl_tree_node.m_right = (*node_to_be_removed)->m_avl_tree_node.m_right;
    replace_node->m_avl_tree_node.m_balance = (*node_to_be_removed)->m_avl_tree_node.m_balance;
    right_item->m_node = &replace_node->m_avl_tree_node.m_right;
    *node_to_be_removed = replace_node;
  }
  else
  {
    *node_to_be_removed = (*node_to_be_removed)->m_avl_tree_node.m_left;
  }
  do
  {
    if ( cur_item <= the_stack )
      break;
    --cur_item;
    m_node = cur_item->m_node;
    (*cur_item->m_node)->m_avl_tree_node.m_balance -= cur_item->m_child;
    if ( (*m_node)->m_avl_tree_node.m_balance == -2 )
    {
      if ( (*m_node)->m_avl_tree_node.m_left->m_avl_tree_node.m_balance == 1 )
        phys_inplace_avl_tree<centity_s const *,auto_rigid_body,auto_rigid_body>::rotate_left(
          this,
          &(*m_node)->m_avl_tree_node.m_left);
      phys_inplace_avl_tree<centity_s const *,auto_rigid_body,auto_rigid_body>::rotate_right(this, m_node);
      if ( (*m_node)->m_avl_tree_node.m_balance >= 2u
        && _tlAssert(
             "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_avl_tree.h",
             218,
             "accessor::get_avl_node(root)->m_balance == 0 || accessor::get_avl_node(root)->m_balance == +1",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
    }
    else if ( (*m_node)->m_avl_tree_node.m_balance == 2 )
    {
      if ( (*m_node)->m_avl_tree_node.m_right->m_avl_tree_node.m_balance == -1 )
        phys_inplace_avl_tree<centity_s const *,auto_rigid_body,auto_rigid_body>::rotate_right(
          this,
          &(*m_node)->m_avl_tree_node.m_right);
      phys_inplace_avl_tree<centity_s const *,auto_rigid_body,auto_rigid_body>::rotate_left(this, m_node);
      if ( (*m_node)->m_avl_tree_node.m_balance
        && (*m_node)->m_avl_tree_node.m_balance != -1
        && _tlAssert(
             "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_avl_tree.h",
             225,
             "accessor::get_avl_node(root)->m_balance == 0 || accessor::get_avl_node(root)->m_balance == -1",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
    }
  }
  while ( !(*m_node)->m_avl_tree_node.m_balance );
}

void __thiscall phys_simple_allocator<broad_phase_environment_info>::free(
        phys_simple_allocator<broad_phase_environment_info> *this,
        broad_phase_environment_info *slot)
{
  if ( slot )
  {
    PMM_VALIDATE((char *)slot, 0x24u, 4u);
    --this->m_count;
    PMM_FREE((unsigned __int8 *)slot, 0x24u, 4u);
  }
}

void __thiscall phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::rotate_right(
        phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor> *this,
        phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal **root)
{
  int m_balance; // [esp+0h] [ebp-24h]
  int v3; // [esp+8h] [ebp-1Ch]
  phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *save_left; // [esp+20h] [ebp-4h]

  save_left = (*root)->m_avl_tree_node.m_left;
  (*root)->m_avl_tree_node.m_left = save_left->m_avl_tree_node.m_right;
  save_left->m_avl_tree_node.m_right = *root;
  if ( save_left->m_avl_tree_node.m_balance >= 0 )
    v3 = 0;
  else
    v3 = -save_left->m_avl_tree_node.m_balance;
  (*root)->m_avl_tree_node.m_balance += v3 + 1;
  if ( (*root)->m_avl_tree_node.m_balance <= 0 )
    m_balance = 0;
  else
    m_balance = (*root)->m_avl_tree_node.m_balance;
  save_left->m_avl_tree_node.m_balance += m_balance + 1;
  *root = save_left;
}

void __thiscall phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::rotate_left(
        phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor> *this,
        phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal **root)
{
  int v2; // [esp+0h] [ebp-24h]
  int m_balance; // [esp+8h] [ebp-1Ch]
  phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *save_right; // [esp+20h] [ebp-4h]

  save_right = (*root)->m_avl_tree_node.m_right;
  (*root)->m_avl_tree_node.m_right = save_right->m_avl_tree_node.m_left;
  save_right->m_avl_tree_node.m_left = *root;
  if ( save_right->m_avl_tree_node.m_balance <= 0 )
    m_balance = 0;
  else
    m_balance = save_right->m_avl_tree_node.m_balance;
  (*root)->m_avl_tree_node.m_balance -= m_balance + 1;
  if ( (*root)->m_avl_tree_node.m_balance >= 0 )
    v2 = 0;
  else
    v2 = -(*root)->m_avl_tree_node.m_balance;
  save_right->m_avl_tree_node.m_balance -= v2 + 1;
  *root = save_right;
}

bool __thiscall gjk_collision_visitor::is_query(gjk_collision_visitor *this)
{
  return 0;
}

const phys_vec3 *__cdecl phys_multiply(const phys_vec3 *result, const phys_mat44 *mat, const phys_vec3 *v)
{
  float v4; // [esp-ACh] [ebp-B8h]
  float v5; // [esp-A8h] [ebp-B4h]
  float y; // [esp-34h] [ebp-40h]
  float z; // [esp-4h] [ebp-10h]

  z = v->z;
  y = v->y;
  v5 = (float)((float)(v->x * mat->x.y) + (float)(y * mat->y.y)) + (float)(z * mat->z.y);
  v4 = (float)((float)(v->x * mat->x.z) + (float)(y * mat->y.z)) + (float)(z * mat->z.z);
  result->x = (float)((float)(v->x * mat->x.x) + (float)(y * mat->y.x)) + (float)(z * mat->z.x);
  result->y = v5;
  result->z = v4;
  return result;
}

