#pragma once
#include "phys_local.h"
#include "phys_gjk.h"
#include "rigid_body.h"
#include "phys_main.h"

struct centity_s;

struct gjk_collision_visitor // sizeof=0x4
{                                                                             // XREF: create_gjk_geom_collision_visitor/r
    virtual void *allocate(const int, const int, const bool) = 0;
    virtual bool is_query()
    {
        return false;
    }
    virtual void get_local_query_aabb(float *, float *)
    {
        //if (!Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h", 35, 0, "%s", "0"))
        //    __debugbreak();
        iassert(0);
    }
    virtual bool query_create_prolog(const void *)
    {
        return true;
    }
    virtual void query_create_epilog(gjk_base_t *gjk_geom)
    {
        ;
    }

    // LWSS: no heckin clue why these are here, just cloned because of the default virtual impl? Maybe some macro crap
    //virtual bool query_create_prolog_1(const float *, const float *, const void *)
    //{
    //    return true;
    //}
    //virtual void query_create_epilog_1(gjk_base_t *)
    //{
    //    ;
    //}
};

struct __declspec(align(16)) gjk_trace_output_t // sizeof=0x50
{
    phys_vec3 m_hit_normal;
    phys_vec3 m_hit_point;
    phys_vec3 m_arm;
    float m_hit_time;
    float m_hit_dist;
    bool m_is_foot;
    // padding byte
    // padding byte
    // padding byte
    gjk_geom_info_t *m_gi;
    gjk_trace_output_t *m_next_link;
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
};

struct broad_phase_environment_info // sizeof=0x24
{
    struct avl_tree_accessor // sizeof=0x0
    {
    };
    void *m_data;
    minspec_mutex m_mutex;
    unsigned int m_gjk_geom_id;
    broad_phase_environment_info *m_next_bpei;
    phys_inplace_avl_tree_node<broad_phase_environment_info> m_avl_tree_node;
    bpei_database_id m_database_id;
};

struct bpei_database_t // sizeof=0x10
{                                       // XREF: broad_phase_memory/r
    phys_inplace_avl_tree<bpei_database_id,broad_phase_environment_info,broad_phase_environment_info::avl_tree_accessor> m_bpei_map;
    phys_simple_allocator<broad_phase_environment_info> m_bpei_allocator;
    broad_phase_environment_info *m_bpei_list;
    minspec_read_write_mutex m_mutex;

    ~bpei_database_t();

    void update_database();
    void purge_database();

    broad_phase_environment_info *get_bpei(bpei_database_id database_id);
};

struct phys_gjk_geom_id_pair_key // sizeof=0x8
{                                       // XREF: phys_gjk_cache_info/r
    unsigned int m_id1;                 // XREF: phys_heap_gjk_cache_system_avl_tree::get_gjk_cache_info(uint,uint,bool)+8B/w
    unsigned int m_id2;                 // XREF: phys_heap_gjk_cache_system_avl_tree::get_gjk_cache_info(uint,uint,bool)+91/w
};

struct phys_heap_gjk_cache_system_avl_tree // sizeof=0x10
{                                       // XREF: broad_phase_memory/r
                                        // gjkcc_info/r
    struct phys_gjk_cache_info_internal : phys_gjk_cache_info // sizeof=0x90
    {
        struct avl_tree_accessor // sizeof=0x0
        {
        };
        phys_inplace_avl_tree_node<phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal> m_avl_tree_node;
        phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *m_next_gjk_ci;
    };
    phys_simple_allocator<phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal> m_list_phys_gjk_cache_info_internal;
    phys_inplace_avl_tree<phys_gjk_geom_id_pair_key,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal,phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal::avl_tree_accessor> m_search_tree;
    int m_max_num_gjk_ci;
    phys_gjk_cache_info_internal *m_list_head;


    ~phys_heap_gjk_cache_system_avl_tree();

    phys_gjk_cache_info_internal *get_gjk_cache_info(
        phys_heap_gjk_cache_system_avl_tree *gjk_cache,
        gjk_base_t *cg1,
        gjk_base_t *cg2);
    phys_gjk_cache_info_internal *get_gjk_cache_info(
        unsigned int id1,
        unsigned int id2,
        bool __formal);

    phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal *get_gjk_cache_info_mutex(
        unsigned int id1,
        unsigned int id2,
        struct tlAtomicReadWriteMutex *query_mutex,
        bool __formal);

    void shutdown();
    void update_cache();
};

const struct __declspec(align(16)) gjk_query_input // sizeof=0x80
{                                       // XREF: gjk_trace_input_t/r
                                        // ?cached_query_resize@gjk_query_output@@QAEX_NPAV?$colgeom_visitor_inlined_t@$0MI@@@H@Z/r
    phys_vec3 m_cg_aabb_min;
    phys_vec3 m_cg_aabb_max;
    phys_vec3 m_cg_position;
    phys_vec3 m_cg_translation;
    phys_vec3 m_ac_eps_vec;
    int m_contents;
    int m_pass_entity_num;
    int m_pass_owner_num;
    bool m_is_server_thread;
    // padding byte
    // padding byte
    // padding byte
    colgeom_visitor_inlined_t<200> *m_proximity_data;
    int m_proximity_mask;
    unsigned int m_gjk_query_flags;
    phys_link_list<gjk_geom_info_t> m_geom_skip_list;
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte

    gjk_query_input();

    void visit_skip_list(int query_visitor_count);
    char is_in_skip_list(gjk_geom_info_t *gi_);
};

struct gjk_query_output : gjk_collision_visitor // sizeof=0x150
{                                       // XREF: gjkcc_info/r
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    phys_vec3 m_query_aabb_min;
    phys_vec3 m_query_aabb_max;
    phys_vec3 m_local_query_aabb_min;
    phys_vec3 m_local_query_aabb_max;
    const gjk_query_input *m_local_query_input;
    bpei_database_id m_local_database_id;
    broad_phase_environment_info *m_local_bpei;
    gjk_entity_info_t *m_local_ent_info;
    bpei_database_t m_bpei_database;
    phys_transient_allocator m_allocator;
    phys_transient_allocator::allocator_state m_allocator_state;
    int m_ent_count;
    int m_geom_count;
    gjk_geom_info_t *m_total_list_geom_info;
    phys_link_list<gjk_geom_info_t> m_list_geom_info;
    int m_query_visitor_count;
    int m_gent_query_visitor_count;
    int m_cent_query_visitor_count;
    int m_dent_query_visitor_count;
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    cached_query_info_t m_cached_query_info;
    phys_vec3 m_accum_start_origin;
    cached_query_info_t m_accum_query_info;
    int m_total_query_count;
    int m_total_cached_query_count;
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte

    gjk_query_output();
    ~gjk_query_output();

    inline void __thiscall verify_empty()
    {
        if (this->m_ent_count
            && !Assert_MyHandler(
                "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../physics/phys_gjk_collision_detection.h",
                333,
                0,
                "%s",
                "m_ent_count == 0"))
        {
            __debugbreak();
        }
        if (this->m_geom_count
            && !Assert_MyHandler(
                "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../physics/phys_gjk_collision_detection.h",
                334,
                0,
                "%s",
                "m_geom_count == 0"))
        {
            __debugbreak();
        }
        if (this->m_query_visitor_count
            && !Assert_MyHandler(
                "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../physics/phys_gjk_collision_detection.h",
                335,
                0,
                "%s",
                "m_query_visitor_count == 0"))
        {
            __debugbreak();
        }
        if (this->m_gent_query_visitor_count
            && !Assert_MyHandler(
                "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../physics/phys_gjk_collision_detection.h",
                336,
                0,
                "%s",
                "m_gent_query_visitor_count == 0"))
        {
            __debugbreak();
        }
        if (this->m_cent_query_visitor_count
            && !Assert_MyHandler(
                "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../physics/phys_gjk_collision_detection.h",
                337,
                0,
                "%s",
                "m_cent_query_visitor_count == 0"))
        {
            __debugbreak();
        }
        if (this->m_dent_query_visitor_count
            && !Assert_MyHandler(
                "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../physics/phys_gjk_collision_detection.h",
                338,
                0,
                "%s",
                "m_dent_query_visitor_count == 0"))
        {
            __debugbreak();
        }
        if (this->m_list_geom_info.m_first
            && !Assert_MyHandler(
                "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../physics/phys_gjk_collision_detection.h",
                339,
                0,
                "%s",
                "m_list_geom_info.get_first() == NULL"))
        {
            __debugbreak();
        }
        if (this->m_allocator.m_cur
            && !Assert_MyHandler(
                "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../physics/phys_gjk_collision_detection.h",
                340,
                0,
                "%s",
                "m_allocator.is_empty()"))
        {
            __debugbreak();
        }
        if (this->m_total_list_geom_info
            && !Assert_MyHandler(
                "c:\\projects_pc\\cod\\codsrc\\src\\bgame\\../physics/phys_gjk_collision_detection.h",
                341,
                0,
                "%s",
                "m_total_list_geom_info == NULL"))
        {
            __debugbreak();
        }
        iassert(m_cached_query_info.is_empty());
    }

    void __thiscall reset_cache();
    void __thiscall query_prolog(const gjk_query_input *input);
    void calc_query_aabb(const gjk_query_input *input);
    void __thiscall query_epilog();
    void *__thiscall allocate(int size, int alignment, bool no_error);
    void __thiscall get_local_query_aabb(
        float *local_query_aabb_min,
        float *local_query_aabb_max);
    bool __thiscall query_create_prolog(const void *geom);
    void __thiscall query_create_epilog(gjk_base_t *gjk_geom);
    bool query_create_prolog_1(
        float *local_aabb_min,
        float *local_aabb_max,
        const void *geom);
    broad_phase_environment_info *__thiscall get_ent_info(unsigned int ent_id);
    void __thiscall set_local_query_info(
        const gjk_query_input *input,
        gjk_entity_info_t *ent_info);
    gjk_geom_info_t *__thiscall create_geom_info(
        gjk_base_t *cg,
        gjk_entity_info_t *ent_info,
        float *aabb_min,
        float *aabb_max);
    gjk_entity_info_t *__thiscall create_entity_info();
    void __thiscall add(
        const gjk_query_input *input,
        const CollisionPartition *partition,
        const CollisionAabbTree *tree);
    void __thiscall add(
        const gjk_query_input *input,
        const cbrush_t *brush,
        const float *query_mins,
        const float *query_maxs);
    void add(
        const gjk_query_input *input,
        gentity_s *gent);
    void add(
        const gjk_query_input *input,
        centity_s *cent);
    void __thiscall add(const gjk_query_input *input, const Glass *glass);
    void __thiscall add(const gjk_query_input *input, const DynEntityDef *dent);
    void cached_query_resize(
        bool is_server_thread,
        colgeom_visitor_inlined_t<200> *proximity_data,
        int proximity_mask);
    void cached_query_prolog(
        bool is_server_thread,
        colgeom_visitor_inlined_t<200> *proximity_data,
        int proximity_mask,
        const phys_vec3 *start_origin);
    void cached_query_epilog();
    void accum_query_reset(const phys_vec3 *start_origin);
};


struct create_gjk_geom_collision_visitor : gjk_collision_visitor // sizeof=0x8
{                                       // XREF: .data:create_gjk_geom_collision_visitor g_empty_collision_visitor/r
                                        // XDoll_CreatePhysObj/r ...
    gjk_geom_list_t *gjk_geom_list;     // XREF: DynEntCl_CreatePhysObj(DynEntityDef const *,DynEntityClient *,GfxPlacement const *)+E8/w
    // FX_SpawnModelPhysics+5D6/w ...

    void *allocate(
        int size,
        int alignment,
        bool no_error)
    {
        if (!Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h", 1140, 0, "%s", "0"))
            __debugbreak();
        return 0;
    }

    bool query_create_prolog(const void *geom) override
    {
        return 1;
    }

    void query_create_epilog(gjk_base_t *gjk_geom) override
    {
        this->gjk_geom_list->add_geom(gjk_geom);
    }
};

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


    bool has_auto_activated();
    void auto_activate(struct broad_phase_info *bpi_impactor);
};

struct gjk_physics_collision_visitor : gjk_collision_visitor // sizeof=0x80
{
    bpei_database_id m_local_database_id;
    broad_phase_environment_info *m_local_bpei;
    const void *m_local_entity;
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    phys_vec3 m_local_query_trace_aabb_min;
    phys_vec3 m_local_query_trace_aabb_max;
    phys_vec3 m_local_query_trace_translation;
    const centity_s *cent;
    const DynEntityDef *dynEntDef;
    const Glass *glass;
    struct rigid_body *rb;
    const phys_mat44 *rb_to_world_xform;
    const phys_mat44 *cg_to_world_xform;
    const phys_mat44 *cg_to_rb_xform;
    unsigned int env_collision_flags;
    const struct broad_phase_environment_query_input *bpeqi;
    struct broad_phase_environement_query_results *bpeqr;
    struct phys_auto_activate_callback *auto_activate_callback;
    // padding byte
    // padding byte
    // padding byte
    // padding byte
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
char __cdecl query_should_pass_gentity(const gjk_query_input *input, const gentity_s *gent);
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
                gjk_double_sphere_t *gjk_capsule);
void    setup_gjk_cylinder(
                float *mins,
                float *maxs,
                float radius_adjust,
                gjk_cylinder_t *gjk_cylinder);

bool    is_walkable(
                const cbrush_t *brush,
                const phys_vec3 *hit_point_loc,
                const phys_vec3 *up_loc);
bool    is_walkable(
                const CollisionPartition *partition,
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
