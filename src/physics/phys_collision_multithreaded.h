#pragma once

void __thiscall phys_memory_heap::set_buffer(phys_memory_heap *this, char *start, int size, unsigned int alignment);
void __thiscall rigid_body_constraint_contact::add_cpi_simple(
        rigid_body_constraint_contact *this,
        contact_point_info *cpi,
        environment_rigid_body *b1_,
        environment_rigid_body *b2_);
phys_contact_manifold_process *__thiscall phys_contact_manifold_process::phys_contact_manifold_process(
        phys_contact_manifold_process *this);
void  phys_collide_do_gjk_collide_and_contact_manifold(
        float a1@<ebp>,
        phys_collision_pair *pcp,
        phys_gjk_info *gjk_info,
        phys_contact_manifold_process *cman_process);
void __cdecl process_cpi(contact_point_info *cpi);
void __cdecl process_list_do_gjk_collide_and_contact_manifold(phys_link_list<phys_collision_pair> *list_pcd);
int  phys_gjk_collide_jq_batch_function@<eax>(int a1@<ebp>, jqBatch *pBatch);
