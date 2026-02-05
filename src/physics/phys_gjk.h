#pragma once

#include "phys_local.h"

struct phys_gjk_geom_id_pair_key // sizeof=0x8
{                                       // XREF: phys_gjk_cache_info/r
    unsigned int m_id1;                 // XREF: phys_heap_gjk_cache_system_avl_tree::get_gjk_cache_info(uint,uint,bool)+8B/w
    unsigned int m_id2;                 // XREF: phys_heap_gjk_cache_system_avl_tree::get_gjk_cache_info(uint,uint,bool)+91/w
};

struct __declspec(align(16)) gjkcc_info // sizeof=0x200
{
    phys_mat44 m_cg_to_world_xform;
    phys_vec3 m_cg_aabb_min;
    phys_vec3 m_cg_aabb_max;
    float m_mins[3];
    float m_maxs[3];
    phys_heap_gjk_cache_system_avl_tree m_gjk_cache;
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    gjk_query_output m_gjk_query_output;
    volatile unsigned int m_active;
    bool m_is_server_thread;
    // padding byte
    // padding byte
    // padding byte
    gjk_base_t *m_cg_;
    float m_last_origin[3];
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte

    void update_cg(float *mins, float *maxs, bool force);
    void Lock();
    void Unlock();
};

struct __declspec(align(16)) gjk_trace_input_t // sizeof=0xB0
{                                       // XREF: gjk_player_trace(gjkcc_input_t const &,trace_t *,float const * const,float const * const,float const * const,float const * const,int,int)+10/w
                                        // ?gjk_player_trace@@YAXABUgjkcc_input_t@@PAUtrace_t@@QBM222HH@Z/r ...
    const gjk_base_t *m_cg;
    float m_gjk_ac_eps;
    bool m_keep_all_collisions;
    bool m_exit_on_penetration;
    // padding byte
    // padding byte
    float m_extra_time;
    phys_heap_gjk_cache_system_avl_tree *m_gjk_cache;
    gjk_query_output *m_query_output;
    gjkcc_info *m_gcci;
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    gjk_query_input m_query_input;
    phys_transient_allocator *m_allocator;
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

    void set_cg_position(const phys_vec3 *position);
};

struct generic_avl_map_node_t // sizeof=0x14
{
    void *m_data;
    unsigned int m_avl_key;
    phys_inplace_avl_tree_node<generic_avl_map_node_t> m_avl_node_info;
};

struct __declspec(align(4)) gjkcc_info_database_t // sizeof=0x10
{                                       // XREF: .data:gjkcc_info_database_t g_gjkcc_info_client_database/r
                                        // .data:gjkcc_info_database_t g_gjkcc_info_server_database/r
    phys_inplace_avl_tree<unsigned int,generic_avl_map_node_t,generic_avl_map_node_t> m_map;
                                        // XREF: _dynamic_initializer_for__g_gjkcc_info_client_database__+3/w
                                        // _dynamic_initializer_for__g_gjkcc_info_server_database__+3/w
    volatile unsigned int m_token;                        // XREF: _dynamic_initializer_for__g_gjkcc_info_client_database__+D/w
                                        // _dynamic_initializer_for__g_gjkcc_info_server_database__+D/w
    int m_gcci_count;                   // XREF: _dynamic_initializer_for__g_gjkcc_info_client_database__+17/w
                                        // _dynamic_initializer_for__g_gjkcc_info_server_database__+17/w
    bool m_is_server_thread;            // XREF: _dynamic_initializer_for__g_gjkcc_info_client_database__+21/w
                                        // _dynamic_initializer_for__g_gjkcc_info_server_database__+21/w
    // padding byte
    // padding byte
    // padding byte

    gjkcc_info *__thiscall gjkcc_info_find_or_create(
        unsigned int gjkcc_id,
        bool is_server_thread,
        const float *origin);
    void __thiscall Lock();
    void __thiscall Unlock();
    void __thiscall verify_internal(bool is_server_thread);
    gjkcc_info *__thiscall gjkcc_info_find(
        unsigned int gjkcc_id,
        bool is_server_thread);
    void __thiscall gjkcc_info_destroy(
        unsigned int gjkcc_id,
        bool is_server_thread);
};

struct gjkcc_input_t // sizeof=0x20
{                                       // XREF: ?Actor_Physics_z@@YAHPAUactor_physics_t@@@Z/r
                                        // Actor_CallThink/r ...
    unsigned int gjkcc_id;              // XREF: ai_physics_trace(trace_t *,float const * const,float const * const,float const * const,float const * const,int,int,actor_physics_t *):loc_912994/w
    bool is_server_thread;              // XREF: ai_physics_trace(trace_t *,float const * const,float const * const,float const * const,float const * const,int,int,actor_physics_t *)+5B/w
    // padding byte
    // padding byte
    // padding byte
    colgeom_visitor_inlined_t<200> *proximity_data;
    // XREF: ai_physics_trace(trace_t *,float const * const,float const * const,float const * const,float const * const,int,int,actor_physics_t *)+5F/w
    int proximity_mask;                 // XREF: ai_physics_trace(trace_t *,float const * const,float const * const,float const * const,float const * const,int,int,actor_physics_t *)+66/w
    int m_ent_num;                      // XREF: ai_physics_trace(trace_t *,float const * const,float const * const,float const * const,float const * const,int,int,actor_physics_t *)+6D/w
    unsigned int m_gjk_query_flags;     // XREF: ai_physics_trace(trace_t *,float const * const,float const * const,float const * const,float const * const,int,int,actor_physics_t *)+74/w
    const gjk_base_t *m_gjk_cg;         // XREF: ai_physics_trace(trace_t *,float const * const,float const * const,float const * const,float const * const,int,int,actor_physics_t *)+7B/w
    const phys_mat44 *m_mat;            // XREF: ai_physics_trace(trace_t *,float const * const,float const * const,float const * const,float const * const,int,int,actor_physics_t *)+82/w
};

struct __declspec(align(4)) gjk_slide_move_input_t // sizeof=0x2C
{                                       // XREF: ai_gjk_slide_move_input_t/r
                                        // player_gjk_slide_move_input_t/r ...
    //gjk_slide_move_input_t_vtbl *__vftable; // XREF: PM_SlideMove+22/w
                                        // PM_SlideMove+2C/w ...
    const float (*position)[3];
    const float (*velocity)[3];
    int gravity;
    int has_gravity;
    const float (*mins)[3];
    const float (*maxs)[3];
    int tracemask;
    int clientnum;
    float frametime;
    bool do_step_down;
    // padding byte
    // padding byte
    // padding byte

    virtual void custom_process(gjk_trace_output_t *gto);
};

struct __declspec(align(4)) gjk_slide_move_output_t // sizeof=0x1C
{                                       // XREF: AIPhys_SlideMove/r
                                        // PM_SlideMove/r
    float new_position[3];              // XREF: PM_SlideMove+97/r
                                        // PM_SlideMove+A9/r ...
    float new_velocity[3];              // XREF: PM_SlideMove+DD/r
                                        // PM_SlideMove+EF/r ...
    bool expensive_push_out;
    bool expensive_push_out_failed;
    // padding byte
    // padding byte
};

struct player_gjk_slide_move_input_t : gjk_slide_move_input_t // sizeof=0x30
{                                       // XREF: PM_SlideMove/r
    pmove_t *pm;

    void custom_process(gjk_trace_output_t *gto);
};

struct player_push_slide_move_input_t : gjk_slide_move_input_t // sizeof=0x38
{
    float velocity_[3];
};

struct list_gjk_trace_output // sizeof=0x10
{                                       // XREF: ?gjk_player_trace@@YAXABUgjkcc_input_t@@PAUtrace_t@@QBM222HH@Z/r
                                        // ?PM_gjk_ground_trace@@YAXABUgjkcc_input_t@@PAUtrace_t@@QBM222HHPAM@Z/r
    phys_link_list<gjk_trace_output_t> m_list;
    gjk_trace_output_t *m_first_hit;
};

struct phys_gjk_cache_info // sizeof=0x80
{                                       // XREF: phys_heap_gjk_cache_system_avl_tree::phys_gjk_cache_info_internal/r
    phys_vec3 m_support_dir;
    cached_simplex_info m_support_a;
    cached_simplex_info m_support_b;
    int m_support_count;
    phys_gjk_geom_id_pair_key m_key;
    unsigned int m_flags;

    phys_gjk_cache_info();
    void update_swapped(bool swapped);
};

struct __declspec(align(8)) gjk_entity_info_t // sizeof=0x50
{
    enum ENTITY_TYPE : __int32
    {                                       // XREF: gjk_entity_info_t/r
        ET_GENT  = 0x0,
        ET_CENT  = 0x1,
        ET_DENT  = 0x2,
        ET_GLASS = 0x3,
        ET_NONE  = 0x4,
    };
    phys_mat44 m_mat;
    gjk_entity_info_t::ENTITY_TYPE m_ent_type;
    const void *m_ent;
    int m_query_visitor_count;
    // padding byte
    // padding byte
    // padding byte
    // padding byte

    const Glass *get_glass();
    const void *get_ent();
    const gentity_s *__thiscall get_gent();
    const centity_s *__thiscall get_cent();

    const DynEntityDef *get_dent();
};

struct __declspec(align(16)) gjk_geom_info_t // sizeof=0x40
{
    phys_vec3 m_aabb_min;
    phys_vec3 m_aabb_max;
    gjk_base_t *m_cg;
    gjk_entity_info_t *m_ent_info;
    int m_query_visitor_count;
    float m_hit_time;
    gjk_geom_info_t *m_next_link;
    gjk_geom_info_t *m_total_next_link;
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte

    void calc_aabb();
    gjk_entity_info_t *get_xform();
};

struct phys_gjk_geom // sizeof=0x4
{                                       // XREF: gjk_base_t/r
    //phys_gjk_geom_vtbl *__vftable;
    virtual void support(const phys_vec3 *, phys_vec3 *, phys_vec3 *);
    virtual void get_simplex(const cached_simplex_info *, const int, phys_vec3 *, phys_vec3 *);
    virtual void set_simplex(const phys_vec3 *, const int, const phys_vec3 *, cached_simplex_info *);
    virtual const phys_vec3 * get_center(const phys_vec3 * result);
    virtual void get_feature(phys_contact_manifold *);
    virtual float get_geom_radius();
    virtual void calc_aabb(const phys_mat44 *, phys_vec3 *, phys_vec3 *);
    virtual bool ray_cast(const phys_vec3 *, const phys_vec3 *, const float, float *, phys_vec3 *);
    virtual bool is_polyhedron();

    const phys_vec3 *support_only(const phys_vec3 *result, const phys_mat44 *xform, const phys_vec3 *v);
};

struct phys_gjk_collision_info // sizeof=0x30
{                                       // XREF: phys_gjk_info/r
    phys_vec3 m_p1;
    phys_vec3 m_p2;
    phys_vec3 m_n;
};

struct __declspec(align(8)) phys_gjk_input // sizeof=0x50
{                                       // XREF: ?gjk_trace@@YAXABUgjk_trace_input_t@@PAUlist_gjk_trace_output@@@Z/r
                                        // ?do_initial_tunnel_test@@YAXPAVbroad_phase_group@@ABVbroad_phase_environement_query_results@@@Z/r ...
    phys_vec3 m_cg1_translation;
    phys_vec3 m_cg2_translation;
    float m_start_time;
    float m_end_time;
    const phys_gjk_geom *gjk_cg1;
    const phys_gjk_geom *gjk_cg2;
    const phys_mat44 *cg1_to_world_xform;
    const phys_mat44 *cg2_to_world_xform;
    phys_gjk_cache_info *gjk_ci;
    float cg1_radius;
    float cg2_radius;
    float m_sep_thresh;
    bool m_intersection_test_only;      // XREF: phys_collide_do_gjk_collide_and_contact_manifold(phys_collision_pair *,phys_gjk_info *,phys_contact_manifold_process *)+10/w
    bool m_continuous_collision;
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
};

struct phys_gjk_info // sizeof=0x3A0
{                                       // XREF: ?gjk_trace@@YAXABUgjk_trace_input_t@@PAUlist_gjk_trace_output@@@Z/r
    enum gjk_retval_e : __int32
    {                                       // XREF: ?phys_collide_do_gjk_collide@phys_gjk_info@@QAE?B_NPBVphys_gjk_input@@@Z/r
        GJK_SEPARATED   = 0x0,
        GJK_VALID       = 0x1,
        GJK_PENETRATING = 0x2,
        GJK_INVALID     = 0x3,
    };
    struct phys_gjk_set_info // sizeof=0x14
    {                                       // XREF: phys_gjk_info/r
        float m_lamda[4];
        int m_candidate;
    };

    phys_gjk_info();

    gjk_retval_e gjk(const phys_gjk_input *d, const phys_vec3 *initial_support_dir, bool in_separation_loop);

    int seed_simplex(int cached_vert_count);
    int gjk_subalgorithm(int w_set, int new_index);
    gjk_retval_e gjk_ray_cast(
        const phys_gjk_input *d,
        const phys_vec3 *initial_support_dir,
        bool in_separation_loop);
    gjk_retval_e collide(const phys_gjk_input *d);
    char phys_collide_do_gjk_collide(const phys_gjk_input *d);
    int compress_verts(char w_set);
    void comp_closest_points(int w_set, phys_vec3 *a, phys_vec3 *b);
    void gjk_cache_update_invalid(const phys_gjk_input *d);
    const phys_vec3 *get_initial_support_dir(phys_vec3 *result, const phys_gjk_input *d);
    void gjk_cache_update_test_only_valid(const phys_gjk_input *d);
    void gjk_cache_update_colliding(const phys_gjk_input *d);
    void gjk_cache_update_test_only_penetrating(const phys_gjk_input *d);
    void comp_v(int w_set, phys_vec3 *v);
    int init_gjk(
        const phys_gjk_input *d,
        const phys_vec3 *initial_support_dir,
        bool in_separation_loop);

    phys_mat44 cg2_to_cg1_xform;
    phys_vec3 m_cg1_relative_translation_loc;
    float m_continuous_collision_lambda;
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
    phys_gjk_collision_info cg1_cinfo_loc;
    phys_vec3 m_gjk_origin;
    phys_vec3 m_w_verts[4];
    phys_vec3 m_a_verts[4];
    phys_vec3 m_b_verts[4];
    phys_vec3 m_a_inds[4];
    phys_vec3 m_b_inds[4];
    phys_vec3 m_support_dir;
    float m_geom_radii_sum;
    int m_cc_reset_iter;
    int m_flags;
    int m_w_set;
    int m_last_w_set;
    int m_gjk_iter;
    float m_gjk_sep_thresh;
    float m_gjk_pen_thresh_sq;
    float m_upper_dist_sq;
    float m_lower_dist_sq;
    float m_dot_ij[4][4];
    phys_gjk_info::phys_gjk_set_info m_set_list[16];
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
    // padding byte
};

// local variable allocation has failed, the output may be wrong!
void    phys_full_inv_multiply_mat(
                phys_mat44 *dest,
                const phys_mat44 *left,
                const phys_mat44 *right);
void __cdecl get_simplex(
                phys_gjk_geom *cg1,
                phys_gjk_geom *cg2,
                phys_gjk_cache_info *gjk_ci,
                phys_vec3 *a_verts,
                phys_vec3 *a_inds,
                phys_vec3 *b_verts,
                phys_vec3 *b_inds,
                int *vert_count);
void __cdecl set_simplex(
                phys_gjk_geom *cg1,
                phys_gjk_geom *cg2,
                phys_gjk_cache_info *gjk_ci,
                const phys_vec3 *a_normal,
                const phys_vec3 *b_normal,
                const phys_vec3 *a_inds,
                const phys_vec3 *b_inds,
                int w_set);
int    backup1(phys_gjk_info *gjk_info, int new_index, bool seed_simplex);

void __cdecl setup_gjk_input_from_pcp(phys_gjk_input *pgi, struct phys_collision_pair *pcp);






