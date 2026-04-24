#pragma once
#include "phys_local.h"
#include "phys_gjk.h"
#include "rigid_body.h"

struct centity_s;
struct gentity_s;
struct cbrush_t;
struct trace_t;
struct DynEntityDef;
struct gjk_query_input;
struct gjk_query_output;
struct gjk_entity_info_t;
struct gjk_trace_output_t;

struct phys_auto_activate_callback // sizeof=0x4
{                                       // XREF: destructible_ent_aa/r
                                        // dynamic_ent_aa/r
    //phys_auto_activate_callback_vtbl *__vftable;00000000 struct /*VFT*/ phys_auto_activate_callback_vtbl // sizeof=0x8

    virtual bool has_auto_activated() = 0;
    virtual void auto_activate(struct broad_phase_info *) = 0;
};

struct destructible_ent_aa : phys_auto_activate_callback // sizeof=0xC
{
    bool m_has_auto_activated;
    // padding byte
    // padding byte
    // padding byte
    centity_s *m_cent;


    bool has_auto_activated()
    {
        return m_has_auto_activated;
    }
    void auto_activate(struct broad_phase_info *bpi_impactor);
};
static_assert(sizeof(destructible_ent_aa) == 12);

struct dynamic_ent_aa : phys_auto_activate_callback // sizeof=0xC
{
    bool m_has_auto_activated;
    // padding byte
    // padding byte
    // padding byte
    DynEntityDef *m_dynEntDef;

    bool has_auto_activated()
    {
        return m_has_auto_activated;
    }
    void auto_activate(broad_phase_info *bpi_impactor);
};

phys_vec3 *__cdecl phys_Unitize(phys_vec3 *result, const phys_vec3 *a);
void __cdecl gjk_collision_epilog(bool is_server_thread);

bool __cdecl phys_are_aabb_overlapping(
    const phys_vec3 *aabb1_min,
    const phys_vec3 *aabb1_max,
    const phys_vec3 *aabb2_min,
    const phys_vec3 *aabb2_max);

void __cdecl gjk_query_prims(const gjk_query_input *input, gjk_query_output *output);
void __cdecl gjk_query_terrain(const gjk_query_input *input, gjk_query_output *output);
void __cdecl gjk_query_gents(const gjk_query_input *input, gjk_query_output *output);
char __cdecl query_should_pass_gentity(const gjk_query_input *input, gentity_s *gent);
void __cdecl gjk_query_cents(const gjk_query_input *input, gjk_query_output *output);
void __cdecl gjk_query_glass(const gjk_query_input *input, gjk_query_output *output);
void __cdecl gjk_query_dents(const gjk_query_input *input, gjk_query_output *output);
void __cdecl gjk_query(const gjk_query_input *input, gjk_query_output *output);

void    gjk_query_cached(const gjk_query_input *input, gjk_query_output *output);


char __cdecl gjk_trace_aabb(
                const phys_vec3 *p0,
                const phys_vec3 *dir,
                const phys_vec3 *aabb_min,
                const phys_vec3 *aabb_max,
                float *t_);
bool __cdecl gjk_trace_plane_1d(float d1, float d2, float *min_t, float *max_t, bool use_iw_logic);
char __cdecl query_should_pass_entity(const gjk_query_input *input, gjk_entity_info_t *ent_info);
void    setup_gjk_capsule(
                float *mins,
                float *maxs,
                float radius_adjust,
                struct gjk_double_sphere_t *gjk_capsule);
void    setup_gjk_cylinder(
                float *mins,
                float *maxs,
                float radius_adjust,
                struct gjk_cylinder_t *gjk_cylinder);

bool    is_walkable(
                const cbrush_t *brush,
                const phys_vec3 *hit_point_loc,
                const phys_vec3 *up_loc);
bool    is_walkable(
                const struct CollisionPartition *partition,
                const phys_vec3 *hit_point_loc,
                const phys_vec3 *up_loc);

void    project(
                const phys_vec3 *point,
                phys_static_array<geom_plane,128> *list_geom_plane,
                phys_vec3 *result);
void __cdecl get_material_from_brush(const cbrush_t *brush, int *sflags);
void __cdecl fill_results(const gjk_trace_output_t *gto, bool is_walkable, trace_t *results);
void __cdecl fill_results_type_and_id(const gjk_trace_output_t *gto, trace_t *results);
void __cdecl fill_results_no_hit(trace_t *results);
bool    is_walkable(const gjk_trace_output_t *gto);

//phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *__thiscall phys_simple_allocator<phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal>::allocate(
//                phys_simple_allocator<phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal> *this);
