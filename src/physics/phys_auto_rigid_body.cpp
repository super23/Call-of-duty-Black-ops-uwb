#include "phys_auto_rigid_body.h"

void __cdecl auto_rigid_body::add(const centity_s *cent, gjk_physics_collision_visitor *collision_visitor, int mask)
{
  phys_mat44 *p_m_mat; // [esp+Ch] [ebp-BCh]
  phys_mat44 *outMat; // [esp+28h] [ebp-A0h]
  float *inVector; // [esp+2Ch] [ebp-9Ch]
  float axis[13]; // [esp+34h] [ebp-94h] BYREF
  auto_rigid_body *m_right; // [esp+68h] [ebp-60h]
  volatile int *p_ReadLockCount; // [esp+90h] [ebp-38h]
  auto_rigid_body *m_tree_root; // [esp+94h] [ebp-34h]
  DObj *obj; // [esp+B8h] [ebp-10h]
  XModel *model; // [esp+BCh] [ebp-Ch]
  unsigned int collflag; // [esp+C0h] [ebp-8h]
  auto_rigid_body *arb; // [esp+C4h] [ebp-4h]

  if ( !cent
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_auto_rigid_body.cpp",
          33,
          0,
          "%s",
          "cent != NULL") )
  {
    __debugbreak();
  }
  if ( cent->currentState.pos.trType != 10
    && cent->nextState.lerp.pos.trType != 10
    && !CG_IsRagdollTrajectory(&cent->currentState.pos)
    && !CG_IsRagdollTrajectory(&cent->nextState.lerp.pos) )
  {
    if ( (char *)cent->nextState.solid == &cls.rankedServers[711].game[34]
      && cent->nextState.eType != 14
      && cent->nextState.eType != 16
      || (obj = Com_GetClientDObj(cent->nextState.number, cent->pose.localClientNum)) != 0
      && ((model = DObjGetModel(obj, 0), XModelHasCollmap(model)) || cent->nextState.eType == 17) )
    {
      tlAtomicReadWriteMutex::ReadLock(&g_auto_rigid_body_map_mutex);
      m_tree_root = g_auto_rigid_body_map.m_tree_root;
      while ( m_tree_root && cent != m_tree_root->cent )
      {
        if ( cent >= m_tree_root->cent )
          m_tree_root = m_tree_root->m_avl_node_info.m_right;
        else
          m_tree_root = m_tree_root->m_avl_node_info.m_left;
      }
      arb = m_tree_root;
      p_ReadLockCount = &g_auto_rigid_body_map_mutex.ThisPtr->ReadLockCount;
      _InterlockedExchangeAdd(&g_auto_rigid_body_map_mutex.ThisPtr->ReadLockCount, 0xFFFFFFFF);
      if ( !m_tree_root )
      {
        tlAtomicReadWriteMutex::WriteLock(&g_auto_rigid_body_map_mutex);
        m_right = g_auto_rigid_body_map.m_tree_root;
        while ( m_right && cent != m_right->cent )
        {
          if ( cent >= m_right->cent )
            m_right = m_right->m_avl_node_info.m_right;
          else
            m_right = m_right->m_avl_node_info.m_left;
        }
        arb = m_right;
        if ( !m_right )
        {
          arb = phys_simple_allocator<auto_rigid_body>::allocate(&g_auto_rigid_body_allocator);
          arb->cent = cent;
          arb->frame_count = 3;
          arb->rb = phys_sys::create_user_rigid_body(0);
          outMat = &arb->rb->m_dictator_mat;
          inVector = cent->pose.origin;
          AnglesToAxis(cent->pose.angles, (float (*)[3])axis);
          Phys_AxisToNitrousMat((float (*)[3])axis, outMat);
          Phys_Vec3ToNitrousVec(inVector, &outMat->w);
          user_rigid_body::set(arb->rb, &arb->rb->m_dictator_mat);
          rigid_body::set_flag(arb->rb, 0x40u, 1);
          arb->next = g_auto_rigid_body_list;
          g_auto_rigid_body_list = arb;
          phys_inplace_avl_tree<centity_s const *,auto_rigid_body,auto_rigid_body>::add(
            &g_auto_rigid_body_map,
            &cent,
            arb);
        }
        tlAtomicReadWriteMutex::WriteUnlock(&g_auto_rigid_body_map_mutex);
      }
      collision_visitor->cent = cent;
      collision_visitor->dynEntDef = 0;
      collision_visitor->glass = 0;
      collflag = 2;
      if ( cent->nextState.eType == 17 )
        collflag = 128;
      if ( arb )
      {
        arb->frame_count = 3;
        p_m_mat = &arb->rb->m_mat;
        collision_visitor->rb = arb->rb;
        collision_visitor->rb_to_world_xform = p_m_mat;
        collision_visitor->cg_to_world_xform = p_m_mat;
        collision_visitor->cg_to_rb_xform = &PHYS_IDENTITY_MATRIX_38;
      }
      else
      {
        collision_visitor->rb = phys_sys::get_environment_rigid_body();
        collision_visitor->rb_to_world_xform = &PHYS_IDENTITY_MATRIX_38;
        collision_visitor->cg_to_world_xform = 0;
        collision_visitor->cg_to_rb_xform = 0;
      }
      collision_visitor->env_collision_flags = collflag;
      create_entity_bpi(collision_visitor, mask);
    }
  }
}

void __thiscall tlAtomicReadWriteMutex::WriteLock(tlAtomicReadWriteMutex *this)
{
  int Target; // [esp+30h] [ebp-24h] BYREF
  volatile signed __int32 *p_WriteLockCount; // [esp+34h] [ebp-20h]
  tlAtomicReadWriteMutex *v4; // [esp+38h] [ebp-1Ch]
  volatile signed __int32 *p_ReadLockCount; // [esp+3Ch] [ebp-18h]
  tlAtomicReadWriteMutex *v6; // [esp+40h] [ebp-14h]
  tlAtomicReadWriteMutex *ThisPtr; // [esp+48h] [ebp-Ch]
  unsigned __int64 CurThread; // [esp+4Ch] [ebp-8h]

  CurThread = GetCurrentThreadId();
  ThisPtr = this->ThisPtr;
  if ( _InterlockedCompareExchange64((volatile signed __int64 *)ThisPtr, CurThread, CurThread) == CurThread )
  {
    _InterlockedExchangeAdd(&this->ThisPtr->WriteLockCount, 1u);
    return;
  }
  while ( 1 )
  {
    v6 = this->ThisPtr;
    if ( _InterlockedCompareExchange64((volatile signed __int64 *)v6, CurThread, 0) )
      goto LABEL_4;
    p_ReadLockCount = &this->ThisPtr->ReadLockCount;
    if ( !_InterlockedCompareExchange(p_ReadLockCount, 0, 0) )
      break;
    do
      v4 = this->ThisPtr;
    while ( _InterlockedCompareExchange64((volatile signed __int64 *)v4, 0, CurThread) != CurThread );
LABEL_4:
    SwitchToThread();
  }
  p_WriteLockCount = &this->ThisPtr->WriteLockCount;
  _InterlockedExchangeAdd(p_WriteLockCount, 1u);
  Target = 0;
  InterlockedExchange(&Target, 0);
}

void __thiscall tlAtomicReadWriteMutex::ReadLock(tlAtomicReadWriteMutex *this)
{
  int Target; // [esp+30h] [ebp-20h] BYREF
  tlAtomicReadWriteMutex *v3; // [esp+34h] [ebp-1Ch]
  volatile signed __int32 *v4; // [esp+38h] [ebp-18h]
  tlAtomicReadWriteMutex *v5; // [esp+3Ch] [ebp-14h]
  volatile signed __int32 *p_ReadLockCount; // [esp+40h] [ebp-10h]
  tlAtomicReadWriteMutex *ThisPtr; // [esp+44h] [ebp-Ch]
  unsigned __int64 CurThread; // [esp+48h] [ebp-8h]

  CurThread = GetCurrentThreadId();
  ThisPtr = this->ThisPtr;
  if ( _InterlockedCompareExchange64((volatile signed __int64 *)ThisPtr, CurThread, CurThread) == CurThread )
  {
    p_ReadLockCount = &this->ThisPtr->ReadLockCount;
    _InterlockedExchangeAdd(p_ReadLockCount, 1u);
  }
  else
  {
    while ( 1 )
    {
      v5 = this->ThisPtr;
      if ( !_InterlockedCompareExchange64((volatile signed __int64 *)v5, CurThread, 0) )
        break;
      SwitchToThread();
    }
    v4 = &this->ThisPtr->ReadLockCount;
    _InterlockedExchangeAdd(v4, 1u);
    do
      v3 = this->ThisPtr;
    while ( _InterlockedCompareExchange64((volatile signed __int64 *)v3, 0, CurThread) != CurThread );
  }
  Target = 0;
  InterlockedExchange(&Target, 0);
}

void __thiscall tlAtomicReadWriteMutex::WriteUnlock(tlAtomicReadWriteMutex *this)
{
  signed __int64 v1; // [esp+Ch] [ebp-20h]
  int Target; // [esp+1Ch] [ebp-10h] BYREF
  volatile signed __int32 *p_WriteLockCount; // [esp+20h] [ebp-Ch]
  unsigned __int64 CurThread; // [esp+24h] [ebp-8h]

  CurThread = GetCurrentThreadId();
  p_WriteLockCount = &this->ThisPtr->WriteLockCount;
  if ( !_InterlockedDecrement(p_WriteLockCount) )
  {
    Target = 0;
    InterlockedExchange(&Target, 0);
    do
      v1 = _InterlockedCompareExchange64((volatile signed __int64 *)this->ThisPtr, 0, CurThread);
    while ( v1 != CurThread );
  }
}

// local variable allocation has failed, the output may be wrong!
void  auto_rigid_body::update(auto_rigid_body *a1@<ebp>)
{
  _BYTE v1[12]; // [esp-Ch] [ebp-8Ch] BYREF
  _BYTE dictator_36[36]; // [esp+24h] [ebp-5Ch] OVERLAPPED BYREF
  float *angles; // [esp+60h] [ebp-20h]
  float *origin; // [esp+64h] [ebp-1Ch]
  auto_rigid_body *next; // [esp+68h] [ebp-18h]
  auto_rigid_body **p_next; // [esp+6Ch] [ebp-14h]
  auto_rigid_body *v7; // [esp+70h] [ebp-10h]
  auto_rigid_body *iter_next; // [esp+74h] [ebp-Ch]
  auto_rigid_body **prev_next; // [esp+78h] [ebp-8h]
  auto_rigid_body **retaddr; // [esp+80h] [ebp+0h]

  iter_next = a1;
  prev_next = retaddr;
  v7 = g_auto_rigid_body_list;
  p_next = &g_auto_rigid_body_list;
  while ( v7 )
  {
    if ( --v7->frame_count <= 0
      || CG_IsRagdollTrajectory(&v7->cent->nextState.lerp.pos)
      || v7->cent->nextState.lerp.pos.trType == 10 )
    {
      next = v7->next;
      phys_inplace_avl_tree<centity_s const *,auto_rigid_body,auto_rigid_body>::remove(
        &g_auto_rigid_body_map,
        &v7->cent);
      phys_sys::destroy(v7->rb);
      phys_simple_allocator<auto_rigid_body>::free(&g_auto_rigid_body_allocator, v7);
      *p_next = next;
      v7 = next;
    }
    else
    {
      origin = v7->cent->pose.origin;
      angles = v7->cent->pose.angles;
      AnglesToAxis(angles, (float (*)[3])&dictator_36[24]);
      Phys_AxisToNitrousMat((float (*)[3])&dictator_36[24], (phys_mat44 *)v1);
      Phys_Vec3ToNitrousVec(origin, (phys_vec3 *)dictator_36);
      user_rigid_body::setPosition(v7->rb, (const phys_mat44 *const)v1);
      p_next = &v7->next;
      v7 = v7->next;
    }
  }
}

void __cdecl fixup_wheel_constraints_arb(auto_rigid_body *arb)
{
  phys_free_list<rigid_body_constraint_wheel>::T_internal_base *wci; // [esp+Ch] [ebp-8h]
  phys_free_list<rigid_body_constraint_wheel> *wci_end; // [esp+10h] [ebp-4h]

  wci = g_physics_system->m_list_rbc_wheel.m_dummy_head.m_next_T_internal;
  wci_end = &g_physics_system->m_list_rbc_wheel;
  while ( wci_end != (phys_free_list<rigid_body_constraint_wheel> *)wci )
  {
    if ( wci != (phys_free_list<rigid_body_constraint_wheel>::T_internal_base *)-16
      && wci[2].m_next_T_internal == (phys_free_list<rigid_body_constraint_wheel>::T_internal_base *)arb->rb )
    {
      rigid_body_constraint_wheel::set_no_collision((rigid_body_constraint_wheel *)&wci[2]);
    }
    wci = wci->m_next_T_internal;
  }
}

void __cdecl auto_rigid_body::remove_ent(const centity_s *cent)
{
  auto_rigid_body *m_tree_root; // [esp+144h] [ebp-Ch]
  auto_rigid_body **prev_next; // [esp+148h] [ebp-8h]

  m_tree_root = g_auto_rigid_body_map.m_tree_root;
  while ( m_tree_root && cent != m_tree_root->cent )
  {
    if ( cent >= m_tree_root->cent )
      m_tree_root = m_tree_root->m_avl_node_info.m_right;
    else
      m_tree_root = m_tree_root->m_avl_node_info.m_left;
  }
  if ( m_tree_root )
  {
    prev_next = &g_auto_rigid_body_list;
    while ( *prev_next != m_tree_root )
    {
      prev_next = &(*prev_next)->next;
      if ( !*prev_next
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_auto_rigid_body.cpp",
              159,
              0,
              "%s",
              "*prev_next") )
      {
        __debugbreak();
      }
    }
    *prev_next = m_tree_root->next;
    phys_inplace_avl_tree<centity_s const *,auto_rigid_body,auto_rigid_body>::remove(
      &g_auto_rigid_body_map,
      &m_tree_root->cent);
    fixup_wheel_constraints_arb(m_tree_root);
    phys_sys::destroy(m_tree_root->rb);
    phys_simple_allocator<auto_rigid_body>::free(&g_auto_rigid_body_allocator, m_tree_root);
  }
}

user_rigid_body *__cdecl auto_rigid_body::ent_has_auto_rigid_body(const centity_s *cent)
{
  auto_rigid_body *m_tree_root; // [esp+0h] [ebp-8h]

  m_tree_root = g_auto_rigid_body_map.m_tree_root;
  while ( m_tree_root && cent != m_tree_root->cent )
  {
    if ( cent >= m_tree_root->cent )
      m_tree_root = m_tree_root->m_avl_node_info.m_right;
    else
      m_tree_root = m_tree_root->m_avl_node_info.m_left;
  }
  if ( !m_tree_root )
    return 0;
  m_tree_root->frame_count = 3;
  return m_tree_root->rb;
}

auto_rigid_body *__thiscall phys_simple_allocator<auto_rigid_body>::allocate(
        phys_simple_allocator<auto_rigid_body> *this)
{
  char *slot; // [esp+14h] [ebp-4h]

  slot = PMM_ALLOC(0x1Cu, 4u);
  if ( !slot )
    return 0;
  ++this->m_count;
  return (auto_rigid_body *)slot;
}

void __thiscall phys_simple_allocator<auto_rigid_body>::free(
        phys_simple_allocator<auto_rigid_body> *this,
        auto_rigid_body *slot)
{
  if ( slot )
  {
    PMM_VALIDATE((char *)slot, 0x1Cu, 4u);
    --this->m_count;
    PMM_FREE((unsigned __int8 *)slot, 0x1Cu, 4u);
  }
}

