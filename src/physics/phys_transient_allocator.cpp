#include "phys_transient_allocator.h"

void __thiscall phys_transient_allocator::reset(phys_transient_allocator *this)
{
  phys_transient_allocator::block_header *m_first_block; // eax
  phys_slot_pool *m_slot_pool; // ebx
  phys_transient_allocator::block_header *m_next_block; // esi

  m_first_block = this->m_first_block;
  m_slot_pool = (phys_slot_pool *)this->m_slot_pool;
  if ( this->m_first_block )
  {
    do
    {
      m_next_block = m_first_block->m_next_block;
      PSP_FREE(m_slot_pool, (unsigned __int8 *)m_first_block);
      m_first_block = m_next_block;
    }
    while ( m_next_block );
  }
  this->m_first_block = 0;
  this->m_cur = 0;
  this->m_end = 0;
  this->m_total_memory_allocated = 0;
}

void __thiscall phys_transient_allocator::reset_to_state(
        phys_transient_allocator *this,
        const phys_transient_allocator::allocator_state *as)
{
  phys_transient_allocator::block_header *m_first_block; // eax
  phys_transient_allocator::block_header *m_next_block; // esi
  phys_slot_pool *slot_pool; // [esp+8h] [ebp-4h]

  slot_pool = (phys_slot_pool *)this->m_slot_pool;
  m_first_block = this->m_first_block;
  if ( this->m_first_block != as->m_first_block )
  {
    do
    {
      m_next_block = m_first_block->m_next_block;
      PSP_FREE(slot_pool, (unsigned __int8 *)m_first_block);
      m_first_block = m_next_block;
    }
    while ( m_next_block != as->m_first_block );
  }
  *(phys_transient_allocator::allocator_state *)&this->m_first_block = *as;
}

void __thiscall phys_transient_allocator::resize(phys_transient_allocator *this)
{
  phys_slot_pool *m_slot_pool; // eax
  char *v3; // eax

  m_slot_pool = (phys_slot_pool *)this->m_slot_pool;
  if ( !m_slot_pool )
  {
    m_slot_pool = GET_PHYS_SLOT_POOL(0x4000u, 4u);
    this->m_slot_pool = m_slot_pool;
  }
  v3 = PSP_ALLOC(m_slot_pool);
  if ( v3 )
  {
    *(unsigned int *)v3 = 0x4000;
    *((unsigned int *)v3 + 1) = 4;
    *((unsigned int *)v3 + 2) = this->m_first_block;
    this->m_first_block = (phys_transient_allocator::block_header *)v3;
    this->m_total_memory_allocated += 0x4000;
    this->m_cur = v3 + 12;
    this->m_end = v3 + 0x4000;
  }
}

