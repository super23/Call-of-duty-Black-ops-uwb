#include "phys_collision_multithreaded.h"

void __thiscall phys_memory_heap::set_buffer(phys_memory_heap *this, char *start, int size, unsigned int alignment)
{
  if ( this->m_buffer_start
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         33,
         "m_buffer_start == NULL",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( this->m_buffer_end
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         34,
         "m_buffer_end == NULL",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( this->m_buffer_cur
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         35,
         "m_buffer_cur == NULL",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( size <= 0
    && _tlAssert("C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h", 36, "size > 0", &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( (unsigned int)start % alignment
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
         37,
         "((size_t)start) % alignment == 0",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  this->m_buffer_start = start;
  this->m_buffer_cur = start;
  this->m_user_start = start;
  this->m_buffer_end = &start[size];
}

void __thiscall rigid_body_constraint_contact::add_cpi_simple(
        rigid_body_constraint_contact *this,
        contact_point_info *cpi,
        environment_rigid_body *b1_,
        environment_rigid_body *b2_)
{
  if ( !cpi
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\rbc_defs\\rbc_def_contact.h",
         364,
         "cpi",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( !cpi->m_list_b1_r_loc
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\rbc_defs\\rbc_def_contact.h",
         365,
         "cpi->m_list_b1_r_loc",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( !cpi->m_list_b2_r_loc
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\rbc_defs\\rbc_def_contact.h",
         366,
         "cpi->m_list_b2_r_loc",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( !cpi->m_list_pulse_sum_cache_info
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\rbc_defs\\rbc_def_contact.h",
         367,
         "cpi->m_list_pulse_sum_cache_info",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( cpi->m_point_pair_count <= 0
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\rbc_defs\\rbc_def_contact.h",
         368,
         "cpi->m_point_pair_count > 0",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  rigid_body_constraint_contact::verify_constraint(this, b1_, b2_);
  cpi->m_next_link = this->m_list_contact_point_info_buffer_1.m_first;
  this->m_list_contact_point_info_buffer_1.m_first = cpi;
}

phys_contact_manifold_process *__thiscall phys_contact_manifold_process::phys_contact_manifold_process(
        phys_contact_manifold_process *this)
{
  phys_memory_heap *p_m_allocator; // edi

  this->m_allocator.m_buffer_start = 0;
  this->m_allocator.m_buffer_end = 0;
  this->m_allocator.m_buffer_cur = 0;
  this->m_allocator.m_user_start = 0;
  p_m_allocator = &this->m_allocator;
  this->m_cpi_allocator = 0;
  this->m_list_cpi.m_first = 0;
  this->m_list_cpi.m_alloc_count = 0;
  this->m_list_cpi.m_last_next_ptr = &this->m_list_cpi.m_first;
  this->m_cpi = 0;
  this->m_rbc_contact_search_tree_root = (rigid_body_constraint_contact *)-1;
  phys_memory_heap::set_buffer(&this->m_allocator, this->m_allocator_memory, 0x4000, 1u);
  this->cman1.m_allocator = p_m_allocator;
  this->cman2.m_allocator = p_m_allocator;
  return this;
}

void  phys_collide_do_gjk_collide_and_contact_manifold(
        float a1@<ebp>,
        phys_collision_pair *pcp,
        phys_gjk_info *gjk_info,
        phys_contact_manifold_process *cman_process)
{
  _BYTE v4[12]; // [esp-Ch] [ebp-5Ch] BYREF
  phys_gjk_input pgi; // [esp+0h] [ebp-50h] BYREF
  _UNKNOWN *retaddr; // [esp+50h] [ebp+0h]

  pgi.m_sep_thresh = a1;
  *(unsigned int *)&pgi.m_intersection_test_only = retaddr;
  if ( (pcp->m_hit_time < 0.0 || pcp->m_hit_time > 1.0)
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_collision.h",
         29,
         "pcp->m_hit_time >= 0.0f && pcp->m_hit_time <= 1.0f",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  setup_gjk_input_from_pcp((phys_gjk_input *)v4, pcp);
  *(float *)&pgi.gjk_ci = 1.02;
  LOWORD(pgi.cg1_radius) = 256;
  if ( phys_gjk_info::phys_collide_do_gjk_collide(gjk_info, (int)&pgi.m_sep_thresh, (const phys_gjk_input *)v4) )
    phys_contact_manifold_process::process(
      cman_process,
      COERCE_FLOAT((phys_gjk_input *)&pgi.m_sep_thresh),
      (int)gjk_info,
      (int)pcp,
      pcp,
      gjk_info);
}

void __cdecl process_cpi(contact_point_info *cpi)
{
  contact_point_info *v1; // esi
  phys_collision_pair *m_pcp; // eax
  broad_phase_info *m_bpi1; // ecx
  broad_phase_info *m_bpi2; // edx
  rigid_body_constraint_contact *m_rbc_contact; // eax
  environment_rigid_body *m_rb; // edi
  environment_rigid_body *v7; // ebx
  contact_point_info *next_cpi; // [esp+Ch] [ebp+8h]

  v1 = cpi;
  if ( cpi )
  {
    do
    {
      next_cpi = v1->m_next_link;
      if ( !v1->m_pcp && _tlAssert("source/phys_collision_multithreaded.cpp", 78, "cpi->m_pcp", &toastPopupTitle) )
        __debugbreak();
      m_pcp = v1->m_pcp;
      m_bpi1 = m_pcp->m_bpi1;
      m_bpi2 = m_pcp->m_bpi2;
      m_rbc_contact = v1->m_rbc_contact;
      m_rb = (environment_rigid_body *)m_bpi1->m_rb;
      v7 = (environment_rigid_body *)m_bpi2->m_rb;
      if ( m_rbc_contact || (m_rbc_contact = phys_sys::create_rbc_contact(m_rb, v7, v1->m_flags & 8)) != 0 )
      {
        m_rbc_contact->m_solver_priority = v1->m_flags & 7;
        rigid_body_constraint_contact::add_cpi_simple(m_rbc_contact, v1, m_rb, v7);
      }
      v1 = next_cpi;
    }
    while ( next_cpi );
  }
}

void __cdecl process_list_do_gjk_collide_and_contact_manifold(phys_link_list<phys_collision_pair> *list_pcd)
{
  phys_transient_allocator *cpi_allocator; // eax
  unsigned int *v2; // eax

  if ( list_pcd->m_alloc_count > 0 )
  {
    g_list_pcp_iterator = list_pcd->m_first;
    g_list_output_cpi = 0;
    cpi_allocator = contact_point_info::get_cpi_allocator();
    v2 = phys_transient_allocator::allocate(cpi_allocator, 12, 4, 0, "phys_transient_allocator out of memory.");
    v2[2] = &g_list_pcp_iterator;
    v2[1] = g_physics_system->m_search_tree_rbc_contact.m_tree_root;
    *v2 = &g_list_output_cpi;
    phys_task_manager_process(&phys_gjk_collide_jqModule, v2, list_pcd->m_alloc_count);
    phys_task_manager_flush();
    process_cpi(g_list_output_cpi);
  }
}

int  phys_gjk_collide_jq_batch_function@<eax>(int a1@<ebp>, jqBatch *pBatch)
{
  void *v2; // esp
  volatile signed __int32 **Input; // esi
  phys_transient_allocator *cpi_allocator; // eax
  rigid_body_constraint_contact *v5; // ecx
  volatile signed __int32 *v6; // eax
  phys_collision_pair *i; // ecx
  contact_point_info *v8; // edi
  phys_gjk_info v10; // [esp-4540h] [ebp-454Ch] BYREF
  contact_point_info *m_first; // [esp-4194h] [ebp-41A0h]
  phys_contact_manifold_process v12; // [esp-4190h] [ebp-419Ch] BYREF
  unsigned int v13[3]; // [esp+0h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]

  v13[0] = a1;
  v13[1] = retaddr;
  v2 = alloca(17736);
  Input = (volatile signed __int32 **)pBatch->Input;
  phys_contact_manifold_process::phys_contact_manifold_process(&v12);
  cpi_allocator = contact_point_info::get_cpi_allocator();
  v5 = (rigid_body_constraint_contact *)Input[1];
  v12.m_cpi_allocator = cpi_allocator;
  v12.m_rbc_contact_search_tree_root = v5;
  v6 = Input[2];
  for ( i = (phys_collision_pair *)*v6; *v6; i = (phys_collision_pair *)*v6 )
  {
    if ( (phys_collision_pair *)_InterlockedCompareExchange(v6, (signed __int32)i->m_next_link, (signed __int32)i) == i )
      phys_collide_do_gjk_collide_and_contact_manifold(COERCE_FLOAT(v13), i, &v10, &v12);
    v6 = Input[2];
  }
  m_first = v12.m_list_cpi.m_first;
  if ( v12.m_list_cpi.m_first )
  {
    do
      v8 = (contact_point_info *)**Input;
    while ( (contact_point_info *)_InterlockedCompareExchange(*Input, (signed __int32)m_first, (signed __int32)v8) != v8 );
    if ( !v12.m_list_cpi.m_last_next_ptr
      && _tlAssert(
           "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
           263,
           "m_last_next_ptr",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    *v12.m_list_cpi.m_last_next_ptr = v8;
  }
  return 0;
}

