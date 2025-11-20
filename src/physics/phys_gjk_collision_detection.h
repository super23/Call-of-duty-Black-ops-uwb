#pragma once

phys_vec3 *__cdecl phys_Unitize(phys_vec3 *result, const phys_vec3 *a);
void __cdecl gjk_collision_epilog(bool is_server_thread);
phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *__cdecl get_gjk_cache_info(
        phys_heap_gjk_cache_system_avl_tree *gjk_cache,
        const gjk_base_t *cg1,
        const gjk_base_t *cg2);
phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *__thiscall phys_heap_gjk_cache_system_avl_tree::get_gjk_cache_info(
        phys_heap_gjk_cache_system_avl_tree *this,
        unsigned int id1,
        unsigned int id2,
        bool __formal);
// local variable allocation has failed, the output may be wrong!
void __userpurge phys_gjk_cache_info::update_swapped(phys_gjk_cache_info *this@<ecx>, int a2@<ebp>, bool swapped);
cached_simplex_info *__thiscall cached_simplex_info::operator=(
        cached_simplex_info *this,
        const cached_simplex_info *__that);
void __thiscall gjk_query_output::reset_cache(gjk_query_output *this);
void __thiscall bpei_database_t::update_database(bpei_database_t *this);
void __thiscall gjk_query_output::query_prolog(gjk_query_output *this, const gjk_query_input *input);
void __thiscall gjk_query_input::visit_skip_list(gjk_query_input *this, int query_visitor_count);
void __userpurge gjk_query_output::calc_query_aabb(
        gjk_query_output *this@<ecx>,
        int a2@<ebp>,
        const gjk_query_input *input);
void __thiscall gjk_query_output::query_epilog(gjk_query_output *this);
void *__thiscall gjk_query_output::allocate(gjk_query_output *this, int size, int alignment, bool no_error);
void __thiscall gjk_query_output::get_local_query_aabb(
        gjk_query_output *this,
        float *local_query_aabb_min,
        float *local_query_aabb_max);
bool __thiscall gjk_query_output::query_create_prolog(gjk_query_output *this, const void *geom);
broad_phase_environment_info *__thiscall bpei_database_t::get_bpei(bpei_database_t *this, bpei_database_id database_id);
const void *__thiscall gjk_entity_info_t::get_ent(gjk_entity_info_t *this);
void __thiscall gjk_query_output::query_create_epilog(gjk_query_output *this, gjk_base_t *gjk_geom);
bool __userpurge gjk_query_output::query_create_prolog_1@<al>(
        gjk_query_output *this@<ecx>,
        int a2@<ebp>,
        float *local_aabb_min,
        float *local_aabb_max,
        const void *geom);
bool __cdecl phys_are_aabb_overlapping(
        const phys_vec3 *aabb1_min,
        const phys_vec3 *aabb1_max,
        const phys_vec3 *aabb2_min,
        const phys_vec3 *aabb2_max);
broad_phase_environment_info *__thiscall gjk_query_output::get_ent_info(gjk_query_output *this, unsigned int ent_id);
void __thiscall gjk_query_output::set_local_query_info(
        gjk_query_output *this,
        const gjk_query_input *input,
        gjk_entity_info_t *ent_info);
gjk_geom_info_t *__thiscall gjk_query_output::create_geom_info(
        gjk_query_output *this,
        gjk_base_t *cg,
        gjk_entity_info_t *ent_info,
        float *aabb_min,
        float *aabb_max);
void __thiscall gjk_geom_info_t::calc_aabb(gjk_geom_info_t *this);
gjk_entity_info_t *__thiscall gjk_query_output::create_entity_info(gjk_query_output *this);
void __thiscall gjk_query_output::add(
        gjk_query_output *this,
        const gjk_query_input *input,
        const CollisionPartition *partition,
        const CollisionAabbTree *tree);
void __thiscall gjk_query_output::add(
        gjk_query_output *this,
        const gjk_query_input *input,
        const cbrush_t *brush,
        const float *query_mins,
        const float *query_maxs);
// local variable allocation has failed, the output may be wrong!
void __userpurge gjk_query_output::add(
        gjk_query_output *this@<ecx>,
        broad_phase_environment_info *a2@<ebp>,
        const gjk_query_input *input,
        gentity_s *gent);
// local variable allocation has failed, the output may be wrong!
void __userpurge gjk_query_output::add(
        gjk_query_output *this@<ecx>,
        broad_phase_environment_info *a2@<ebp>,
        const gjk_query_input *input,
        centity_s *cent);
void __thiscall gjk_query_output::add(gjk_query_output *this, const gjk_query_input *input, const Glass *glass);
void __thiscall gjk_query_output::add(gjk_query_output *this, const gjk_query_input *input, const DynEntityDef *dent);
void __cdecl gjk_query_prims(const gjk_query_input *input, gjk_query_output *output);
void __cdecl gjk_query_terrain(const gjk_query_input *input, gjk_query_output *output);
void __cdecl gjk_query_gents(const gjk_query_input *input, gjk_query_output *output);
char __cdecl query_should_pass_gentity(const gjk_query_input *input, const gentity_s *gent);
void __cdecl gjk_query_cents(const gjk_query_input *input, gjk_query_output *output);
void __cdecl gjk_query_glass(const gjk_query_input *input, gjk_query_output *output);
void __cdecl gjk_query_dents(const gjk_query_input *input, gjk_query_output *output);
void __cdecl gjk_query(const gjk_query_input *input, gjk_query_output *output);
void __userpurge gjk_query_output::cached_query_resize(
        gjk_query_output *this@<ecx>,
        int a2@<ebp>,
        bool is_server_thread,
        colgeom_visitor_inlined_t<200> *proximity_data,
        int proximity_mask);
bool __thiscall cached_query_info_t::aabb_is_valid(cached_query_info_t *this);
void __userpurge gjk_query_output::cached_query_prolog(
        gjk_query_output *this@<ecx>,
        int a2@<ebp>,
        bool is_server_thread,
        colgeom_visitor_inlined_t<200> *proximity_data,
        int proximity_mask,
        const phys_vec3 *start_origin);
void __thiscall cached_query_info_t::init_query(
        cached_query_info_t *this,
        const phys_vec3 *query_aabb_min,
        const phys_vec3 *query_aabb_max,
        const phys_vec3 *extra,
        int query_contents,
        unsigned int query_flags);
void __thiscall gjk_query_output::cached_query_epilog(gjk_query_output *this);
void __thiscall gjk_query_output::accum_query_reset(gjk_query_output *this, const phys_vec3 *start_origin);
void  gjk_query_cached(float a1@<ebp>, const gjk_query_input *input, gjk_query_output *output);
int __thiscall gjk_base_t::get_contents(gjk_base_t *this);
char __thiscall gjk_query_input::is_in_skip_list(gjk_query_input *this, gjk_geom_info_t *gi_);
void __userpurge cached_query_info_t::add_query(
        cached_query_info_t *this@<ecx>,
        int a2@<ebp>,
        const phys_vec3 *query_aabb_min,
        const phys_vec3 *query_aabb_max,
        const phys_vec3 *extra,
        int query_contents,
        unsigned int query_flags);
bool __thiscall cached_query_info_t::is_subset(
        cached_query_info_t *this,
        const phys_vec3 *query_aabb_min,
        const phys_vec3 *query_aabb_max,
        int query_contents,
        unsigned int query_flags);
bool __thiscall cached_query_info_t::is_subset_aabb(
        cached_query_info_t *this,
        const phys_vec3 *query_aabb_min,
        const phys_vec3 *query_aabb_max);
char __cdecl gjk_trace_aabb(
        const phys_vec3 *p0,
        const phys_vec3 *dir,
        const phys_vec3 *aabb_min,
        const phys_vec3 *aabb_max,
        float *t_);
bool __cdecl gjk_trace_plane_1d(float d1, float d2, float *min_t, float *max_t, bool use_iw_logic);
char __cdecl query_should_pass_entity(const gjk_query_input *input, gjk_entity_info_t *ent_info);
const DynEntityDef *__thiscall gjk_entity_info_t::get_dent(gjk_entity_info_t *this);
void  setup_gjk_capsule(
        float a1@<ebp>,
        float *mins,
        float *maxs,
        float radius_adjust,
        gjk_double_sphere_t *gjk_capsule);
void  setup_gjk_cylinder(
        float a1@<ebp>,
        float *mins,
        float *maxs,
        float radius_adjust,
        gjk_cylinder_t *gjk_cylinder);
bool __userpurge gjk_aabb_t::is_walkable@<al>(
        gjk_aabb_t *this@<ecx>,
        float a2@<ebp>,
        const phys_vec3 *hit_point,
        const phys_vec3 *up);
bool  is_walkable@<al>(
        unsigned int a1@<ebp>,
        const cbrush_t *brush,
        const phys_vec3 *hit_point_loc,
        const phys_vec3 *up_loc);
bool  is_walkable@<al>(
        int a1@<ebp>,
        const CollisionPartition *partition,
        const phys_vec3 *hit_point_loc,
        const phys_vec3 *up_loc);
void  project(
        int a1@<ebp>,
        const phys_vec3 *point,
        phys_static_array<geom_plane,128> *list_geom_plane,
        phys_vec3 *result);
void __cdecl get_material_from_brush(const cbrush_t *brush, int *sflags);
void __cdecl fill_results(const gjk_trace_output_t *gto, bool is_walkable, trace_t *results);
void __cdecl fill_results_type_and_id(const gjk_trace_output_t *gto, trace_t *results);
const Glass *__thiscall gjk_entity_info_t::get_glass(gjk_entity_info_t *this);
void __cdecl fill_results_no_hit(trace_t *results);
bool  is_walkable@<al>(int a1@<ebp>, const gjk_trace_output_t *gto);
phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *__thiscall phys_simple_allocator<phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal>::allocate(
        phys_simple_allocator<phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal> *this);
phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *__thiscall phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::find(
        phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor> *this,
        const phys_gjk_geom_id_pair_key *key);
void __thiscall phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::add(
        phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor> *this,
        const phys_gjk_geom_id_pair_key *key,
        phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *data);
phys_gjk_cache_info *__thiscall phys_gjk_cache_info::phys_gjk_cache_info(phys_gjk_cache_info *this);
