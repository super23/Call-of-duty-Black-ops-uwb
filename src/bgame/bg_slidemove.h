#pragma once

void __cdecl Phys_Vec3ToNitrousVec(float *inVector, phys_vec3 *outVector);
gjk_polygon_cylinder_t *__cdecl create_gjkcc_gjk_geom(
        float (*mins)[3],
        float (*maxs)[3],
        int stype,
        gjk_collision_visitor *allocator);
gjkcc_info *__cdecl create_gjkcc_info(unsigned int gjkcc_id, bool is_server_thread, const float *origin);
phys_mat44 *__thiscall phys_mat44::operator=(phys_mat44 *this, const phys_mat44 *__that);
void __thiscall gjkcc_info::update_cg(gjkcc_info *this, float *mins, float *maxs, bool force);
void __cdecl destroy_gjkcc_info(char *gcci_);
void __thiscall phys_heap_gjk_cache_system_avl_tree::shutdown(phys_heap_gjk_cache_system_avl_tree *this);
void __thiscall gjk_query_output::verify_empty(gjk_query_output *this);
char __thiscall cached_query_info_t::is_empty(cached_query_info_t *this);
gjkcc_info *__thiscall gjkcc_info_database_t::gjkcc_info_find_or_create(
        gjkcc_info_database_t *this,
        unsigned int gjkcc_id,
        bool is_server_thread,
        const float *origin);
void __thiscall gjkcc_info_database_t::Lock(gjkcc_info_database_t *this);
void __thiscall gjkcc_info_database_t::Unlock(gjkcc_info_database_t *this);
void __thiscall gjkcc_info_database_t::verify_internal(gjkcc_info_database_t *this, bool is_server_thread);
gjkcc_info *__thiscall gjkcc_info_database_t::gjkcc_info_find(
        gjkcc_info_database_t *this,
        unsigned int gjkcc_id,
        bool is_server_thread);
void __thiscall gjkcc_info_database_t::gjkcc_info_destroy(
        gjkcc_info_database_t *this,
        unsigned int gjkcc_id,
        bool is_server_thread);
void __cdecl destroy_client_gjkcc_info(int localClientNum);
void  gjkcc_prolog(int a1@<ebp>, const gjkcc_input_t *gjkcc_in, float *origin);
const phys_vec3 *__cdecl Phys_Vec3ToNitrousVec(const phys_vec3 *result, float *inVector);
void __thiscall gjkcc_info::Lock(gjkcc_info *this);
void __cdecl gjkcc_epilog(const gjkcc_input_t *gjkcc_in, const float *origin);
void __thiscall phys_heap_gjk_cache_system_avl_tree::update_cache(phys_heap_gjk_cache_system_avl_tree *this);
void __thiscall gjkcc_info::Unlock(gjkcc_info *this);
gjkcc_info *__cdecl find_gjkcc_info(unsigned int gjkcc_id, bool is_server_thread);
bool __thiscall gjk_double_sphere_t::is_foot(gjk_double_sphere_t *this, const phys_vec3 *hit_point);
bool __userpurge gjk_cylinder_t::is_foot@<al>(gjk_cylinder_t *this@<ecx>, int a2@<ebp>, const phys_vec3 *hit_point);
phys_vec3 * phys_full_inv_multiply@<eax>(
        int a1@<ebp>,
        phys_vec3 *result,
        const phys_mat44 *mat,
        const phys_vec3 *v);
const phys_vec3 *__cdecl phys_inv_multiply(const phys_vec3 *result, const phys_mat44 *mat, const phys_vec3 *v);
bool __thiscall gjk_polygon_cylinder_t::is_foot(gjk_polygon_cylinder_t *this, const phys_vec3 *hit_point);
bool __thiscall gjk_brush_t::is_walkable(gjk_brush_t *this, const phys_vec3 *hit_point, const phys_vec3 *up);
bool __thiscall gjk_partition_t::is_walkable(gjk_partition_t *this, const phys_vec3 *hit_point, const phys_vec3 *up);
void __cdecl calc_time(phys_gjk_info *gjk_info, bool keep_all_collisions, float *time, float *dist);
void __cdecl POLYGON_CYLINDER_HACK(phys_gjk_input *pgi, const gjk_base_t *cg1, const gjk_base_t *cg2);
// local variable allocation has failed, the output may be wrong!
char  gjk_collide@<al>(
        float a1@<ebp>,
        phys_gjk_info *gjk_info,
        phys_gjk_input *pgi,
        gjk_trace_output_t *gto,
        const gjk_trace_input_t *gti,
        gjk_geom_info_t *gi);
void __cdecl sort_gi_list(gjk_geom_info_t **list, int list_count);
void  gjk_trace(int a1@<ebp>, const gjk_trace_input_t *gti, list_gjk_trace_output *list);
void *__thiscall phys_transient_allocator::allocate(
        phys_transient_allocator *this,
        int size,
        int alignment,
        int no_error,
        const char *error_msg);
void __cdecl init_pgi(phys_gjk_input *pgi, const gjk_trace_input_t *gti);
void __cdecl set_pgi_cg2(phys_gjk_input *pgi, const gjk_trace_input_t *gti, gjk_geom_info_t *gi);
gjk_entity_info_t *__thiscall gjk_geom_info_t::get_xform(gjk_geom_info_t *this);
const phys_mat44 *__thiscall gjk_base_t::get_xform(gjk_base_t *this);
phys_gjk_info *__thiscall phys_gjk_info::phys_gjk_info(phys_gjk_info *this);
void __cdecl setup_query_input(
        const gjkcc_input_t *gjkcc_in,
        const phys_vec3 *aabb_min,
        const phys_vec3 *aabb_max,
        const phys_vec3 *position,
        const phys_vec3 *translation,
        int passEntityNum,
        int contentMask,
        gjk_query_input *query_input);
void  setup_trace_info(
        int a1@<ebp>,
        const gjkcc_input_t *gjkcc_in,
        float *start,
        float *mins,
        float *maxs,
        float *end,
        int passEntityNum,
        int contentMask,
        gjk_trace_input_t *gti);
void __thiscall gjk_trace_input_t::set_cg_position(gjk_trace_input_t *this, const phys_vec3 *position);
void  gjk_player_trace(
        int a1@<ebp>,
        const gjkcc_input_t *gjkcc_in,
        trace_t *results,
        float *start,
        float *mins,
        float *maxs,
        float *end,
        int passEntityNum,
        int contentMask);
char __cdecl no_push_out(const gjk_trace_output_t *gto);
const gentity_s *__thiscall gjk_entity_info_t::get_gent(gjk_entity_info_t *this);
const centity_s *__thiscall gjk_entity_info_t::get_cent(gjk_entity_info_t *this);
void __cdecl set_hit_info(geom_plane *gp, const gjk_trace_output_t *gto);
void __cdecl add_hit_info(
        const gjk_trace_output_t *gto,
        phys_static_array<geom_plane,128> *list_geom_plane,
        phys_link_list<gjk_geom_info_t> *geom_skip_list);
char __cdecl project_succeeded(
        phys_static_array<geom_plane,128> *list_geom_plane,
        const phys_vec3 *new_position,
        float PROJECT_FAIL_THRESH);
char  gjk_push_out@<al>(
        int a1@<ebp>,
        const gjkcc_input_t *gjkcc_in,
        gjk_slide_move_input_t *input,
        gjk_slide_move_output_t *output);
phys_vec3 *__thiscall phys_vec3::operator=(phys_vec3 *this, const phys_vec3 *v);
double __cdecl Abs(const float *v);
void __cdecl Phys_NitrousVecToVec3(const phys_vec3 *inVector, float *outVector);
gjk_query_input *__thiscall gjk_query_input::gjk_query_input(gjk_query_input *this);
bool  gjk_slide_move1@<eax>(
        int a1@<ebp>,
        const gjkcc_input_t *gjkcc_in,
        gjk_slide_move_input_t *input,
        const bool no_push_out,
        gjk_slide_move_output_t *output,
        bool *needs_push_out);
int __cdecl gjk_slide_move(
        const gjkcc_input_t *gjkcc_in,
        gjk_slide_move_input_t *input,
        gjk_slide_move_output_t *output);
void  setup_player_push_slide_move_input(
        int a1@<ebp>,
        player_push_slide_move_input_t *input,
        pmove_t *pm,
        pml_t *pml,
        const phys_vec3 *push_movement);
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
        float *origin_);
// local variable allocation has failed, the output may be wrong!
void  render_gjkcc_collision(float a1@<ebp>, float (*mins)[3], float (*maxs)[3], float (*origin)[3]);
phys_vec3 *__cdecl operator+(phys_vec3 *result, const phys_vec3 *a, const phys_vec3 *b);
void __thiscall phys_gjk_geom::set_simplex(
        phys_gjk_geom *this,
        const phys_vec3 *simplex_inds,
        int w_set,
        const phys_vec3 *normal,
        cached_simplex_info *cache_info);
bool __thiscall phys_gjk_geom::ray_cast(
        phys_gjk_geom *this,
        const phys_vec3 *__formal,
        const phys_vec3 *ray_dir,
        float t_input,
        float *t_output,
        phys_vec3 *hitn);
gjk_base_t *__thiscall gjk_base_t::gjk_base_t(gjk_base_t *this);
double __thiscall phys_gjk_geom::get_geom_radius(phys_gjk_geom *this);
void __thiscall gjk_base_t::~gjk_base_t(gjk_base_t *this);
void __thiscall gjk_base_t::comp_aabb_loc(gjk_base_t *this);
const cbrush_t *__thiscall gjk_base_t::get_brush(gjk_base_t *this);
bool __thiscall gjk_base_t::is_foot(gjk_base_t *this, const phys_vec3 *hit_point);
bool __thiscall gjk_base_t::is_walkable(gjk_base_t *this, const phys_vec3 *hit_point, const phys_vec3 *up);
void __thiscall gjk_polygon_cylinder_t::poly_verts::get_co_si(
        gjk_polygon_cylinder_t::poly_verts *this,
        int i,
        float *co_,
        float *si_);
void __userpurge gjk_polygon_cylinder_t::support(
        gjk_polygon_cylinder_t *this@<ecx>,
        int a2@<ebp>,
        const phys_vec3 *v,
        phys_vec3 *support_vert,
        phys_vec3 *support_ind);
int __thiscall gjk_polygon_cylinder_t::poly_verts::support(
        gjk_polygon_cylinder_t::poly_verts *this,
        const phys_vec3 *v);
void __userpurge gjk_polygon_cylinder_t::get_simplex(
        gjk_polygon_cylinder_t *this@<ecx>,
        int a2@<ebp>,
        const cached_simplex_info *cache_info,
        int index_count,
        phys_vec3 *simplex_verts,
        phys_vec3 *simplex_inds);
void __userpurge gjk_polygon_cylinder_t::calc_aabb(
        gjk_polygon_cylinder_t *this@<ecx>,
        int a2@<ebp>,
        const phys_mat44 *xform,
        phys_vec3 *aabb_min,
        phys_vec3 *aabb_max);
const phys_vec3 *__cdecl phys_min(const phys_vec3 *result, const phys_vec3 *v1, const phys_vec3 *v2);
const phys_vec3 *__cdecl phys_max(const phys_vec3 *result, const phys_vec3 *v1, const phys_vec3 *v2);
void __cdecl gjk_polygon_cylinder_t::calc_disc_aabb(
        const phys_vec3 *axis,
        float radius,
        phys_vec3 *aabb_min,
        phys_vec3 *aabb_max);
unsigned int __thiscall gjk_polygon_cylinder_t::get_type(gjk_polygon_cylinder_t *this);
double __thiscall gjk_polygon_cylinder_t::get_geom_radius(gjk_polygon_cylinder_t *this);
void __cdecl render_gjkcc_collision(pmove_t *pm);
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
        float *first_hit_time);
double __cdecl PM_PermuteRestrictiveClipPlanes(
        const float *velocity,
        int planeCount,
        const float (*planes)[3],
        int *permutation);
void __cdecl setup_player_gjk_slide_move_input(
        player_gjk_slide_move_input_t *input,
        pmove_t *pm,
        pml_t *pml,
        int gravity);
void __cdecl PM_StepSlideMove(pmove_t *pm, pml_t *pml, int gravity);
int __cdecl PM_VerifyPronePosition(pmove_t *pm, float *vFallbackOrg, float *vFallbackVel);
int __cdecl PM_SlideMove(pmove_t *pm, pml_t *pml, int gravity);
void __thiscall player_gjk_slide_move_input_t::custom_process(
        player_gjk_slide_move_input_t *this,
        gjk_trace_output_t *gto);
void __thiscall phys_simple_allocator<phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal>::free(
        phys_simple_allocator<phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal> *this,
        phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *slot);
void __thiscall phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::remove(
        phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor> *this,
        const phys_gjk_geom_id_pair_key *key);
void __thiscall phys_link_list<gjk_geom_info_t>::add(phys_link_list<gjk_geom_info_t> *this, gjk_geom_info_t *p);
gjkcc_info *__thiscall phys_simple_allocator<gjkcc_info>::allocate(phys_simple_allocator<gjkcc_info> *this);
gjk_query_output *__thiscall gjk_query_output::gjk_query_output(gjk_query_output *this);
void __thiscall gjk_collision_visitor::get_local_query_aabb(
        gjk_collision_visitor *this,
        float *local_query_aabb_min,
        float *local_query_aabb_max);
char __thiscall gjk_collision_visitor::query_create_prolog_1(
        gjk_collision_visitor *this,
        const float *local_aabb_min,
        const float *local_aabb_max,
        const void *geom);
void __thiscall gjk_collision_visitor::query_create_epilog_1(gjk_collision_visitor *this, gjk_base_t *gjk_geom);
void __thiscall phys_simple_allocator<gjkcc_info>::free(phys_simple_allocator<gjkcc_info> *this, gjkcc_info *slot);
void __thiscall phys_link_list<gjk_trace_output_t>::add(
        phys_link_list<gjk_trace_output_t> *this,
        gjk_trace_output_t *p);
geom_plane *__thiscall phys_static_array<geom_plane,128>::add(
        phys_static_array<geom_plane,128> *this,
        int no_error,
        const char *error_msg);
geom_plane *__thiscall phys_static_array<geom_plane,128>::operator[](phys_static_array<geom_plane,128> *this, int i);
void __thiscall gjk_query_output::~gjk_query_output(gjk_query_output *this);
void __thiscall bpei_database_t::purge_database(bpei_database_t *this);
void __thiscall phys_inplace_avl_tree<bpei_database_id,broad_phase_environment_info,broad_phase_environment_info::avl_tree_accessor>::remove(
        phys_inplace_avl_tree<bpei_database_id,broad_phase_environment_info,broad_phase_environment_info::avl_tree_accessor> *this,
        const bpei_database_id *key);
void __thiscall phys_simple_allocator<broad_phase_environment_info>::free(
        phys_simple_allocator<broad_phase_environment_info> *this,
        broad_phase_environment_info *slot);
void __thiscall phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::rotate_right(
        phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor> *this,
        phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal **root);
void __thiscall phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor>::rotate_left(
        phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor> *this,
        phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal **root);
bool __thiscall gjk_collision_visitor::is_query(gjk_collision_visitor *this);
const phys_vec3 *__cdecl phys_multiply(const phys_vec3 *result, const phys_mat44 *mat, const phys_vec3 *v);
