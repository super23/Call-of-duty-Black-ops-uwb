#pragma once

unsigned int __cdecl phys_slot_pool::encode_size_alignment(unsigned int size, unsigned int alignment);
void __cdecl phys_memory_manager_term();
void __thiscall phys_slot_pool::init(phys_slot_pool *this, unsigned int slot_size, unsigned int slot_alignment);
void __cdecl ppu_pmm_get_linear_buffer(char ***linear_buffer_cur, char **linear_buffer_end);
void __cdecl PSP_FREE(phys_slot_pool *slot_pool, unsigned __int8 *slot);
void __cdecl transient_allocator_update_largest_size();
int __thiscall phys_memory_manager::allocate(phys_memory_manager *this, unsigned int size, unsigned int alignment);
phys_slot_pool *__thiscall phys_memory_manager::allocate_slot_pool(phys_memory_manager *this);
phys_slot_pool *__thiscall phys_memory_manager::get_slot_pool(
        phys_memory_manager *this,
        unsigned int slot_size,
        unsigned int slot_alignment);
void *__cdecl PMM_PERM_ALLOCATE(unsigned int size, unsigned int alignment);
// bad sp value at call has been detected, the output may be wrong!
char *__cdecl PMM_ALLOC(unsigned int size, unsigned int alignment);
void __cdecl PMM_FREE(unsigned __int8 *ptr, unsigned int size, unsigned int alignment);
// bad sp value at call has been detected, the output may be wrong!
char *__cdecl PSP_ALLOC(phys_slot_pool *slot_pool);
phys_slot_pool *__cdecl GET_PHYS_SLOT_POOL(unsigned int size, unsigned int alignment);
void __cdecl PMM_VALIDATE(char *ptr, unsigned int size, unsigned int alignment);
phys_memory_manager *__thiscall phys_memory_manager::phys_memory_manager(
        phys_memory_manager *this,
        char *memory_buffer,
        int memory_buffer_size);
void __cdecl phys_memory_manager_init(void *const memory_buffer, int memory_buffer_size);
void __thiscall phys_slot_pool::extra_info_init(phys_slot_pool *this, char *slot);
void __thiscall phys_slot_pool::extra_info_allocate(phys_slot_pool *this, char *slot);
void __thiscall phys_slot_pool::extra_info_free(phys_slot_pool *this, unsigned __int8 *slot);
void __thiscall phys_slot_pool::validate_slot(phys_slot_pool *this, char *slot);
void __userpurge phys_slot_pool::free_slot(phys_slot_pool *this@<ecx>, unsigned int a2@<ebp>, unsigned __int8 *slot);
void __thiscall minspec_hash_table<phys_slot_pool,64>::add(
        minspec_hash_table<phys_slot_pool,64> *this,
        unsigned int key,
        phys_slot_pool *entry_to_add);
char * phys_slot_pool::allocate_slot@<eax>(phys_slot_pool *this@<ecx>, unsigned int a2@<ebp>);
