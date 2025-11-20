#pragma once

void __cdecl create_broad_phase_info(rigid_body *body);
void __cdecl destroy_broad_phase_info(rigid_body *body);
jqBatchGroup *__thiscall jqBatchGroup::jqBatchGroup(jqBatchGroup *this);
void  comp_trace_volume(
        int a1@<ebp>,
        const phys_vec3 *aabb1_min,
        const phys_vec3 *aabb1_max,
        const phys_vec3 *aabb2_min,
        const phys_vec3 *aabb2_max,
        phys_vec3 *p1,
        phys_vec3 *p2,
        phys_vec3 *half_dims);
void __userpurge broad_phase_base::get_aabb(broad_phase_base *this@<ecx>, int a2@<ebp>, phys_vec3 *aabb);
void __cdecl setup_gjk_input_from_pcp(phys_gjk_input *pgi, phys_collision_pair *pcp);
void __userpurge phys_wheel_collide_info::collision_process(
        phys_wheel_collide_info *this@<ecx>,
        int a2@<ebp>,
        int a3@<edi>,
        int a4@<esi>,
        broad_phase_info *bpi);
void __userpurge phys_wheel_collide_info::collision_epilog(
        phys_wheel_collide_info *this@<ecx>,
        int a2@<ebp>,
        rigid_body_constraint_wheel *rbc_wheel);
void __thiscall axis_aligned_sweep_and_prune::sap_node::init(
        axis_aligned_sweep_and_prune::sap_node *this,
        broad_phase_base *bpb,
        axis_aligned_sweep_and_prune::axis_element **xlist,
        axis_aligned_sweep_and_prune::axis_element **ylist,
        axis_aligned_sweep_and_prune::axis_element **zlist);
bool __thiscall axis_aligned_sweep_and_prune::are_overlapping(
        axis_aligned_sweep_and_prune *this,
        axis_aligned_sweep_and_prune::sap_node *n1,
        axis_aligned_sweep_and_prune::sap_node *n2);
void __thiscall axis_aligned_sweep_and_prune::remove(
        axis_aligned_sweep_and_prune *this,
        axis_aligned_sweep_and_prune::axis_element **axis_list,
        axis_aligned_sweep_and_prune::sap_node *node);
void __cdecl init_bpeqi(broad_phase_environment_query_input *bpeqi, broad_phase_base *bpb);
void __cdecl check_terrain_query_params(broad_phase_base *bpb);
void  calc_largest_vel_sq(int a1@<ebp>, broad_phase_info *bpi);
void __cdecl broad_phase_reset_buffer();
broad_phase_memory_info *__thiscall broad_phase_memory_info::broad_phase_memory_info(broad_phase_memory_info *this);
int  bp_env_jq_batch_function1@<eax>(int a1@<ebp>, jqBatch *pBatch);
void __cdecl merge_sort_bpb(broad_phase_base **list, int list_count);
void  broad_phase_process_object_environment_collision(int a1@<ebp>, bpi_environment_collision_info *eci);
void __cdecl broad_phase_process_object_environment_collision();
void __thiscall axis_aligned_sweep_and_prune::swap(
        axis_aligned_sweep_and_prune *this,
        axis_aligned_sweep_and_prune::axis_element **a1_ptr,
        axis_aligned_sweep_and_prune::axis_element **a2_ptr);
void __thiscall axis_aligned_sweep_and_prune::merge_sort(
        axis_aligned_sweep_and_prune *this,
        axis_aligned_sweep_and_prune::axis_element **list,
        int list_count);
void __cdecl aasap_list_add(broad_phase_base *bpb);
void __cdecl aasap_list_remove(broad_phase_base *bpb);
void  do_initial_tunnel_test(
        float a1@<ebp>,
        broad_phase_group *bpg,
        const broad_phase_environement_query_results *bpeqr);
broad_phase_info *__cdecl create_broad_phase_info();
void __cdecl destroy_broad_phase_info(broad_phase_info *bpi);
void __cdecl destroy_broad_phase_info_list(broad_phase_info *list_bpi);
broad_phase_collision_pair *__cdecl create_broad_phase_collision_pair();
void __cdecl destroy_broad_phase_collision_pair(broad_phase_collision_pair *bpcp);
void __cdecl destroy_broad_phase_collision_pair_list(broad_phase_collision_pair *list_bpcp);
broad_phase_group *__cdecl create_broad_phase_group();
void __cdecl destroy_broad_phase_group(broad_phase_group *bpg);
char  bpi_do_gjk_intersect@<al>(
        phys_gjk_cache_info *a1@<ebp>,
        broad_phase_info *p1,
        broad_phase_info *p2,
        phys_collision_pair *hit_time);
void __cdecl collide_bpi_environment(float bpi, broad_phase_base_list::node *bpeqr);
void  collide_bpg_environment(
        broad_phase_base_list::node *a1@<ebp>,
        broad_phase_info *bpg,
        const broad_phase_environement_query_results *bpeqr);
int  bp_env_jq_batch_function2@<eax>(int a1@<ebp>);
void  broad_phase_process(float a1@<ebp>);
void __cdecl broad_phase_system_init(
        const broad_phase_memory_info *bpmi,
        bool (__cdecl *should_collide_callback)(const broad_phase_base *, const broad_phase_base *));
void __cdecl broad_phase_system_shutdown();
void __thiscall phys_free_list<broad_phase_info>::debug_remove(
        phys_free_list<broad_phase_info> *this,
        phys_free_list<broad_phase_info>::T_internal *T_i);
void __thiscall phys_free_list<broad_phase_group>::debug_remove(
        phys_free_list<broad_phase_group> *this,
        phys_free_list<broad_phase_group>::T_internal *T_i);
void __thiscall phys_free_list<broad_phase_collision_pair>::debug_remove(
        phys_free_list<broad_phase_collision_pair> *this,
        phys_free_list<broad_phase_collision_pair>::T_internal *T_i);
void __thiscall tlAtomicReadWriteMutex::~tlAtomicReadWriteMutex(tlAtomicReadWriteMutex *this);
void __thiscall rigid_body::adjust_col_moved_vec(rigid_body *this, float lambda);
void  broad_phase_group::collision_prolog(broad_phase_group *this@<ecx>, int a2@<ebp>);
void __thiscall broad_phase_group::collision_epilog(broad_phase_group *this);
void  axis_aligned_sweep_and_prune::sap_node::update_ae_val(
        axis_aligned_sweep_and_prune::sap_node *this@<ecx>,
        float a2@<ebp>);
void __thiscall axis_aligned_sweep_and_prune::init_system(axis_aligned_sweep_and_prune *this, int max_num_active_pairs);
void  process_cluster_environment_collision_prolog(
        int a1@<ebp>,
        broad_phase_info *bpb,
        broad_phase_base *info);
bool __cdecl compare_bpb(broad_phase_base *bpb1, broad_phase_base *bpb2);
void __thiscall phys_link_list<phys_collision_pair>::add_mt(
        phys_link_list<phys_collision_pair> *this,
        phys_collision_pair *p);
void __thiscall phys_free_list<broad_phase_info>::debug_add(
        phys_free_list<broad_phase_info> *this,
        phys_free_list<broad_phase_info>::T_internal *T_i);
void __thiscall phys_free_list<broad_phase_info>::remove(
        phys_free_list<broad_phase_info> *this,
        phys_free_list<broad_phase_info>::T_internal *data);
void __thiscall phys_free_list<broad_phase_group>::debug_add(
        phys_free_list<broad_phase_group> *this,
        phys_free_list<broad_phase_group>::T_internal *T_i);
void __thiscall phys_free_list<broad_phase_group>::remove(
        phys_free_list<broad_phase_group> *this,
        phys_free_list<broad_phase_group>::T_internal *data);
void __thiscall phys_free_list<broad_phase_collision_pair>::debug_add(
        phys_free_list<broad_phase_collision_pair> *this,
        phys_free_list<broad_phase_collision_pair>::T_internal *T_i);
void __thiscall phys_free_list<broad_phase_collision_pair>::remove(
        phys_free_list<broad_phase_collision_pair> *this,
        phys_free_list<broad_phase_collision_pair>::T_internal *data);
char  phys_are_potentially_colliding_whace<broad_phase_info,broad_phase_info>@<al>(
        float a1@<ebp>,
        broad_phase_info *p1,
        broad_phase_info *p2,
        float *hit_time);
void __cdecl add_collision_pair(
        broad_phase_info *bpi1,
        broad_phase_info *bpi2,
        float hit_time,
        phys_gjk_cache_info *gjk_ci);
void __cdecl add_collision_pair_mutex(
        broad_phase_info *bpi1,
        broad_phase_info *bpi2,
        float hit_time,
        phys_gjk_cache_info *gjk_ci);
void __thiscall axis_aligned_sweep_and_prune::add_active_pair(
        axis_aligned_sweep_and_prune *this,
        axis_aligned_sweep_and_prune::sap_node *p1,
        axis_aligned_sweep_and_prune::sap_node *p2);
void __thiscall axis_aligned_sweep_and_prune::create_sap_node(
        axis_aligned_sweep_and_prune *this,
        broad_phase_base *bpb);
void __thiscall axis_aligned_sweep_and_prune::remove(
        axis_aligned_sweep_and_prune *this,
        axis_aligned_sweep_and_prune::active_pair **list_ap,
        axis_aligned_sweep_and_prune::sap_node *node);
void __thiscall axis_aligned_sweep_and_prune::destroy_sap_node(
        axis_aligned_sweep_and_prune *this,
        broad_phase_base *bpb);
void  broad_phase_process_collision_pairs(float a1@<ebp>);
void __thiscall phys_free_list<broad_phase_info>::remove_all(phys_free_list<broad_phase_info> *this);
void __thiscall phys_free_list<broad_phase_collision_pair>::remove_all(
        phys_free_list<broad_phase_collision_pair> *this);
// attributes: thunk
void __thiscall phys_heap_gjk_cache_system_avl_tree::~phys_heap_gjk_cache_system_avl_tree(
        phys_heap_gjk_cache_system_avl_tree *this);
// attributes: thunk
void __thiscall bpei_database_t::~bpei_database_t(bpei_database_t *this);
void __thiscall phys_free_list<broad_phase_info>::~phys_free_list<broad_phase_info>(
        phys_free_list<broad_phase_info> *this);
void __thiscall phys_free_list<broad_phase_group>::~phys_free_list<broad_phase_group>(
        phys_free_list<broad_phase_group> *this);
void __thiscall phys_free_list<broad_phase_collision_pair>::~phys_free_list<broad_phase_collision_pair>(
        phys_free_list<broad_phase_collision_pair> *this);
phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *__thiscall phys_heap_gjk_cache_system_avl_tree::get_gjk_cache_info_mutex(
        phys_heap_gjk_cache_system_avl_tree *this,
        unsigned int id1,
        unsigned int id2,
        tlAtomicReadWriteMutex *query_mutex,
        bool __formal);
broad_phase_memory *__thiscall broad_phase_memory::broad_phase_memory(broad_phase_memory *this);
void  axis_aligned_sweep_and_prune::process_active_pair_list(
        axis_aligned_sweep_and_prune *this@<ecx>,
        axis_aligned_sweep_and_prune::active_pair **a2@<ebp>);
void __thiscall axis_aligned_sweep_and_prune::process(axis_aligned_sweep_and_prune *this);
void __cdecl process_cluster_environment_collision(broad_phase_base *bpb, broad_phase_base_list::node *bpeqr);
void __thiscall broad_phase_memory::~broad_phase_memory(broad_phase_memory *this);
broad_phase_memory *__cdecl broad_phase_memory::allocate_buffer(const broad_phase_memory_info *bpmi);
