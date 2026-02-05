#pragma once

#include "phys_local.h"
#include "phys_traverse.h"
#include <xanim/xanim.h>
#include <universal/q_shared.h>
#include <qcommon/cm_trace.h>
#include <qcommon/statmonitor.h>
#include "phys_gjk_collision_detection.h"

#define SURF_TYPECOUNT 31

struct cpose_t;
struct gentity_s;
struct Glass;
struct DynEntityDef;
struct gjk_base_t;
struct cLeafBrushNode_s;
struct cbrush_t;

struct visitor_base_t // sizeof=0x4
{                                                                             // XREF: colgeom_visitor_t/r
        //visitor_base_t_vtbl *__vftable;         // XREF: CG_Vehicle_PreControllers(int,DObj const *,centity_s *)+2AD/w
                                                                                // CG_Vehicle_PreControllers(int,DObj const *,centity_s *):loc_50E81E/r ...
        virtual ~visitor_base_t() = default;
};

struct col_prim_t // sizeof=0x8
{                                       // XREF: colgeom_visitor_inlined_t<200>/r
                                        // colgeom_visitor_inlined_t<500>/r
    int type;
    //$9DB6BB71550D5B679E9F92FB2EA07EBE ___u1;
    union //$9DB6BB71550D5B679E9F92FB2EA07EBE // sizeof=0x4
    {                                       // XREF: col_prim_t/r
        const struct CollisionAabbTree *tree;
        const struct cbrush_t *brush;
    };
};

template <int NUM_PRIMS>
struct colgeom_visitor_inlined_t : colgeom_visitor_t // sizeof=0x6B8
{                                                                             // XREF: .data:dummy/r
    int nprims;                                                 // XREF: debug_loop(void)+385/r
    // Rope_CollideWorld(int)+28A/r
    bool overflow;
    // padding byte
    // padding byte
    // padding byte
    col_prim_t prims[NUM_PRIMS];                            // XREF: debug_loop(void)+38C/o
    // Rope_CollideWorld(int)+27E/o

    colgeom_visitor_inlined_t() : colgeom_visitor_t()
    {
        reset();
    }

    colgeom_visitor_inlined_t &operator=(const colgeom_visitor_inlined_t *that)
    {
        colgeom_visitor_t::operator=((colgeom_visitor_t*)that);
        this->nprims = that->nprims;
        this->overflow = that->overflow;

        for (int i = 0; i < NUM_PRIMS; i++)
        {
            this->prims[i].type = that->prims[i].type;
            this->prims[i].tree = that->prims[i].tree;
        }
    }

    void visit(const cbrush_t *brush)
    {
        col_prim_t *prim;

        if (this->nprims < NUM_PRIMS)
        {
            prim = &this->prims[this->nprims++];
            prim->type = 1;
            prim->tree = (const CollisionAabbTree *)brush;
        }
    }

    void visit(const CollisionAabbTree *tree)
    {
        col_prim_t *prim; // [esp+4h] [ebp-4h]

        if (this->nprims < NUM_PRIMS)
        {
            prim = &this->prims[this->nprims++];
            prim->type = 0;
            prim->tree = tree;
        }
    }

    void reset()
    {
        this->nprims = 0;
        this->overflow = 0;

        this->m_mn.vec.v[0] = 9.9999997e37; // cool float constant, man
        this->m_mn.vec.v[1] = 9.9999997e37;
        this->m_mn.vec.v[2] = 9.9999997e37;

        this->m_mx.vec.v[0] = -9.9999997e37;
        this->m_mx.vec.v[1] = -9.9999997e37;
        this->m_mx.vec.v[2] = -9.9999997e37;
    }

    void intersect_box(float *mn, float *mx, int mask);
    void intersect_box_brushes(cLeaf_s *leaf);
    void intersect_box_brushnode(cLeafBrushNode_s *node);
    void intersect_box_partitions(cLeaf_s *leaf);
    void intersect_box_partitions_r(CollisionAabbTree *aabbTree);

    void update(
        const float *_mn,
        const float *_mx,
        int mask,
        const float *expand_vec)
    {
        bool v5; // [esp+0h] [ebp-58h]
        float result[3]; // [esp+18h] [ebp-40h] BYREF
        float b[3]; // [esp+24h] [ebp-34h] BYREF
        float a[3]; // [esp+30h] [ebp-28h] BYREF
        bool inside; // [esp+3Fh] [ebp-19h]
        float mx[3]; // [esp+40h] [ebp-18h] BYREF
        float mn[3]; // [esp+4Ch] [ebp-Ch] BYREF

        a[0] = this->m_mn.vec.v[0] - *_mn;
        a[1] = this->m_mn.vec.v[1] - _mn[1];
        a[2] = this->m_mn.vec.v[2] - _mn[2];
        b[0] = *_mx - this->m_mx.vec.v[0];
        b[1] = _mx[1] - this->m_mx.vec.v[1];
        b[2] = _mx[2] - this->m_mx.vec.v[2];
        Vec3Max(a, b, result);
        v5 = result[0] < 0.0 && result[1] < 0.0 && result[2] < 0.0;
        inside = v5;
        if (this->m_mask != mask || !inside)
        {
            mn[0] = *_mn - *expand_vec;
            mn[1] = _mn[1] - expand_vec[1];
            mn[2] = _mn[2] - expand_vec[2];
            mx[0] = *_mx + *expand_vec;
            mx[1] = _mx[1] + expand_vec[1];
            mx[2] = _mx[2] + expand_vec[2];
            //colgeom_visitor_inlined_t<500>::reset(this);
            reset();
            //colgeom_visitor_t::intersect_box(this, mn, mx, mask);
            intersect_box(mn, mx, mask);
            if (this->nprims == NUM_PRIMS)
            {
                StatMon_Warning(8, 3000, (char*)"code_warning_collision");
                this->nprims = 0;
                this->overflow = 1;
            }
        }
    }

    void update(
        const float *start,
        const float *end,
        const float *mins,
        const float *maxs,
        int mask)
    {
        float _mn[3]; // [esp+14h] [ebp-60h] BYREF
        float extents_start[3]; // [esp+20h] [ebp-54h] BYREF
        float extents_end[3]; // [esp+2Ch] [ebp-48h] BYREF
        float _mx[3]; // [esp+38h] [ebp-3Ch] BYREF
        float offset[3]; // [esp+44h] [ebp-30h]
        float size[3]; // [esp+50h] [ebp-24h]
        float expand_vec[3]; // [esp+5Ch] [ebp-18h] BYREF
        float fudge[3]; // [esp+68h] [ebp-Ch]

        fudge[0] = 1.0f;//`colgeom_visitor_inlined_t<200 > ::update'::`2'::fFudge;
        fudge[1] = 1.0f;//`colgeom_visitor_inlined_t<200 > ::update'::`2'::fFudge;
        fudge[2] = 1.0f;//`colgeom_visitor_inlined_t<200 > ::update'::`2'::fFudge;
        offset[0] = (float)(0.5 * *mins) + (float)(0.5 * *maxs);
        offset[1] = (float)(0.5 * mins[1]) + (float)(0.5 * maxs[1]);
        offset[2] = (float)(0.5 * mins[2]) + (float)(0.5 * maxs[2]);
        size[0] = *maxs - offset[0];
        size[1] = maxs[1] - offset[1];
        size[2] = maxs[2] - offset[2];
        extents_start[0] = *start + offset[0];
        extents_start[1] = start[1] + offset[1];
        extents_start[2] = start[2] + offset[2];
        extents_end[0] = *end + offset[0];
        extents_end[1] = end[1] + offset[1];
        extents_end[2] = end[2] + offset[2];
        Vec3Min(extents_start, extents_end, _mn);
        Vec3Max(extents_start, extents_end, _mx);
        _mn[0] = _mn[0] - size[0];
        _mn[1] = _mn[1] - size[1];
        _mn[2] = _mn[2] - size[2];
        _mx[0] = _mx[0] + size[0];
        _mx[1] = _mx[1] + size[1];
        _mx[2] = _mx[2] + size[2];
        _mn[0] = _mn[0] - fudge[0];
        _mn[1] = _mn[1] - fudge[1];
        _mn[2] = _mn[2] - fudge[2];
        _mx[0] = _mx[0] + fudge[0];
        _mx[1] = _mx[1] + fudge[1];
        _mx[2] = _mx[2] + fudge[2];
        expand_vec[0] = 70.0f;
        expand_vec[1] = 70.0f;
        expand_vec[2] = 20.0f;
        this->update(_mn, _mx, mask, expand_vec);
    }
};

struct query_brush_model_gjk_geom_visitor : colgeom_visitor_t // sizeof=0x74
{                                                                             // XREF: ?query_brush_model_gjk_geom@@YAXGHPAVgjk_collision_visitor@@@Z/r
        struct gjk_collision_visitor *m_allocator; // XREF: query_brush_model_gjk_geom(ushort,int,gjk_collision_visitor *)+A9/w

        void visit(const cbrush_t *brush);
        void update(
                const float *_mn,
                const float *_mx,
                int _mask,
                const float *expand_vec);
};

const struct cached_simplex_info // sizeof=0x30
{                                                                             // XREF: phys_gjk_cache_info/r
    phys_vec3 m_indices[3];

    cached_simplex_info& operator=(const cached_simplex_info *__that);
};

struct __declspec(align(16)) contact_manifold_mesh_point // sizeof=0x20
{
        phys_vec3 m_p;
        phys_vec2 m_contact_p;
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
};

struct __declspec(align(16)) phys_contact_manifold // sizeof=0x60
{                                                                             // XREF: phys_contact_manifold_process/r
                                                                                // phys_contact_manifold_process/r
        phys_vec3 m_feature_normal;
        phys_vec3 m_feature_hitp;
        phys_vec3 m_feature_hitn;
        float m_feature_distance_eps;
        float m_sin_feautre_angular_eps_sq;
        int m_close_mesh_point_count;
        phys_memory_heap *m_allocator;
        contact_manifold_mesh_point *m_list_mesh_point;
        int m_list_mesh_point_count;
        contact_manifold_mesh_point **m_list_sorted_mesh_point;
        contact_manifold_mesh_point **m_list_contact_point;
        int m_list_contact_point_count;
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

        void add_feature_point(const phys_vec3 *p);
};


struct gjk_base_t : public phys_gjk_geom // sizeof=0x50
{                                                                             // XREF: gjk_brush_t/r
public:
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
        phys_vec3 m_aabb_mn_loc;
        phys_vec3 m_aabb_mx_loc;
        unsigned int m_gjk_geom_id;
        const phys_mat44 *m_xform_;
        gjk_base_t *m_next_geom;
        unsigned int m_flags;
        int stype;
        int m_contents;
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte
        // padding byte

        static const int FLAG_TEMP_ALLOCATION = 1;

        inline int get_flag(int flag)
        {
                return m_flags & flag;
        }

        virtual ~gjk_base_t()
        {
            iassert(!get_flag(FLAG_TEMP_ALLOCATION));
        }

        gjk_base_t() : phys_gjk_geom()
        {
            this->m_flags = 0;
        }

        ~gjk_base_t();


        unsigned int get_geom_id();
        const phys_mat44 *get_xform();

        int get_contents();
        void set_contents(int contents);

        void set_geom_id_new(unsigned int geom_id);
        void set_xform(const phys_mat44 *xform);

        virtual void comp_aabb_loc();
        virtual unsigned int get_type() const = 0;
        virtual bool is_foot(const phys_vec3 *hit_point) const;
        virtual bool is_walkable(const phys_vec3 *hit_point, const phys_vec3 *up);
        virtual const cbrush_t *get_brush() const;
};

struct gjk_geom_list_t // sizeof=0x8
{                                                                             // XREF: PhysObjUserData/r
                                                                                // ?InitPhysicsObj@GlassShard@@QAE_N_N@Z/r ...
        gjk_base_t *m_first_geom;                     // XREF: DynEntCl_CreatePhysObj(DynEntityDef const *,DynEntityClient *,GfxPlacement const *):loc_5B18F9/w
                                                                                // DynEntPieces_SpawnPhysObj:loc_5BCF29/w ...
        int m_geom_count;                                     // XREF: DynEntCl_CreatePhysObj(DynEntityDef const *,DynEntityClient *,GfxPlacement const *)+D0/w
                                                                                // DynEntPieces_SpawnPhysObj+50/w ...
public:
        int get_geom_count();
        void add_geom(gjk_base_t *geom)
        {
            iassert(geom);
            geom->m_next_geom = this->m_first_geom;
            this->m_first_geom = geom;
            ++this->m_geom_count;
        }
        void comp_aabb_loc(
            phys_vec3 *aabb_mn_loc,
            phys_vec3 *aabb_mx_loc);
};

struct __declspec(align(16)) gjk_aabb_t : gjk_base_t // sizeof=0x80
{
        phys_vec3 m_center_local;
        phys_vec3 m_dims;
        const struct cbrush_t *m_brush;
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

        bool is_walkable(const phys_vec3 *hit_point, const phys_vec3 *up);

        static gjk_aabb_t *__cdecl create(
                const phys_vec3 *center,
                const phys_vec3 *dims,
                int stype,
                gjk_collision_visitor *allocator);


        void support(
                const phys_vec3 *v,
                phys_vec3 *support_vert,
                phys_vec3 *support_ind);
        void get_simplex(
                const cached_simplex_info *cache_info,
                int index_count,
                phys_vec3 *simplex_verts,
                phys_vec3 *simplex_inds);
        const phys_vec3 * get_center(phys_vec3 *result);
        void get_feature(phys_contact_manifold *cman);
        void calc_aabb(
                const phys_mat44 *xform,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max);
        const cbrush_t * get_brush();
        static void __cdecl destroy(gjk_aabb_t *geom);
};

struct BrushWrapper // sizeof=0x60
{
        float mins[3];
        int contents;
        float maxs[3];
        unsigned int numsides;
        cbrushside_t *sides;
        int axial_cflags[2][3];
        int axial_sflags[2][3];
        unsigned int numverts;
        float (*verts)[3];
        cplane_s *planes;
};

struct PhysGeomInfo // sizeof=0x44
{
        BrushWrapper *brush;
        int type;
        float orientation[3][3];
        float offset[3];
        float halfLengths[3];
};

struct PhysGeomList // sizeof=0xC
{
        unsigned int count;
        PhysGeomInfo *geoms;
        int contents;
};

struct gjk_obb_t : gjk_base_t // sizeof=0xA0
{
        phys_vec3 m_dims;
        phys_mat44 m_xform;

        static gjk_obb_t *__cdecl create(
            const phys_mat44 *xform,
            const phys_vec3 *dims,
            int stype,
            gjk_collision_visitor *allocator);

        void calc_aabb(
            const phys_mat44 *xform,
            phys_vec3 *aabb_min,
            phys_vec3 *aabb_max);

        static void __cdecl destroy(gjk_obb_t *geom);
        const phys_vec3 *get_center(phys_vec3 *result);
        void get_feature(phys_contact_manifold *cman);
        void get_simplex(
            const cached_simplex_info *cache_info,
            int index_count,
            phys_vec3 *simplex_verts,
            phys_vec3 *simplex_inds);
        unsigned int get_type();
        bool is_polyhedron();
        void support(
                const phys_vec3 *v,
                phys_vec3 *support_vert,
                phys_vec3 *support_ind);
};

struct __declspec(align(8)) gjk_brush_t : gjk_base_t // sizeof=0x60
{
        int nverts;
        const float (*verts)[3];
        const cbrush_t *brush;
        // padding byte
        // padding byte
        // padding byte
        // padding byte

        bool is_walkable(const phys_vec3 *hit_point, const phys_vec3 *up);

        gjk_brush_t *create(
                const cbrush_t *brush,
                int stype,
                gjk_collision_visitor *allocator);
        void support(
                const phys_vec3 *v,
                phys_vec3 *support_vert,
                phys_vec3 *support_ind);
        void get_simplex(
                const cached_simplex_info *cache_info,
                int index_count,
                phys_vec3 *simplex_verts,
                phys_vec3 *simplex_inds);
        void get_feature(phys_contact_manifold *cman);
        void calc_aabb(
                const phys_mat44 *xform,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max);
        const cbrush_t *get_brush();
        unsigned int get_type();
        static void __cdecl destroy(gjk_brush_t *geom);
        const phys_vec3 *get_center(phys_vec3 *result);
};

struct CollisionBorder // sizeof=0x1C
{
        float distEq[3];
        float zBase;
        float zSlope;
        float start;
        float length;
};

const struct CollisionPartition // sizeof=0x14
{
        unsigned __int8 triCount;
        unsigned __int8 borderCount;
        // padding byte
        // padding byte
        int firstTri;
        int nuinds;
        int fuind;
        CollisionBorder *borders;
};

union CollisionAabbTreeIndex // sizeof=0x4
{                                                                             // XREF: CollisionAabbTree/r
        int firstChildIndex;
        int partitionIndex;
};

const struct CollisionAabbTree // sizeof=0x20
{
        float origin[3];
        unsigned __int16 materialIndex;
        unsigned __int16 childCount;
        float halfSize[3];
        CollisionAabbTreeIndex u;
};

struct __declspec(align(16)) gjk_partition_t : gjk_base_t // sizeof=0x70
{
        int ninds;
        const float (*verts)[3];
        unsigned __int16 *inds;
        const CollisionPartition *partition;
        const CollisionAabbTree *tree;
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

        bool is_walkable(const phys_vec3 *hit_point, const phys_vec3 *up);


        gjk_partition_t *__cdecl create(const CollisionAabbTree *tree, gjk_collision_visitor *allocator);
        void support(
                const phys_vec3 *v,
                phys_vec3 *support_vert,
                phys_vec3 *support_ind);
        void get_simplex(
                const cached_simplex_info *cache_info,
                int index_count,
                phys_vec3 *simplex_verts,
                phys_vec3 *simplex_inds);
        void get_feature(phys_contact_manifold *cman);
        void calc_aabb(
                const phys_mat44 *xform,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max);
        unsigned int get_type();
        static void __cdecl destroy(gjk_partition_t *geom);
};

struct gjk_double_sphere_t : gjk_base_t // sizeof=0x90
{
        phys_vec3 m_list_center[2];
        phys_vec3 m_center;
        float m_list_radius[2];
        float m_geom_radius;
        int m_count;

        gjk_double_sphere_t();

        bool is_foot(const phys_vec3 *hit_point);

        void support(
                const phys_vec3 *v,
                phys_vec3 *support_vert,
                phys_vec3 *support_ind);
        void get_simplex(
                const cached_simplex_info *cache_info,
                int index_count,
                phys_vec3 *simplex_verts,
                phys_vec3 *simplex_inds);
        void set_simplex(
                const phys_vec3 *simplex_inds,
                int w_set,
                const phys_vec3 *normal,
                cached_simplex_info *cache_info);
        const phys_vec3 *get_center(phys_vec3 *result);
        void get_feature(phys_contact_manifold *cman);
        void calc_aabb(
                const phys_mat44 *xform,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max);
        float get_geom_radius() const;
        bool is_polyhedron();
        gjk_double_sphere_t *__cdecl create(
                const phys_vec3 *c0,
                const phys_vec3 *c1,
                float r,
                int stype,
                gjk_collision_visitor *allocator);
        static void __cdecl destroy(gjk_double_sphere_t *geom);
};

struct gjk_cylinder_t : gjk_base_t // sizeof=0xA0
{
        int direction;
        float halfHeight;
        float radius;
        float m_geom_radius;
        phys_mat44 xform;

        bool is_foot(const phys_vec3 *hit_point);

        gjk_cylinder_t *__cdecl create(
                int _direction,
                float _halfHeight,
                float _radius,
                const phys_mat44 *_xform,
                int stype,
                gjk_collision_visitor *allocator);
        void support(
                const phys_vec3 *v,
                phys_vec3 *support_vert,
                phys_vec3 *support_ind);
        const phys_vec3 * get_dims(const phys_vec3 *result);
        void get_simplex(
                const cached_simplex_info *cache_info,
                int index_count,
                phys_vec3 *simplex_verts,
                phys_vec3 *simplex_inds);
        const phys_vec3 * get_center(phys_vec3 *result);
        void get_feature(phys_contact_manifold *cman);
        void calc_aabb(
                const phys_mat44 *xform_,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max);
        unsigned int get_type();
        float get_geom_radius() const;
        static void __cdecl destroy(gjk_cylinder_t *geom);
};

struct gjk_unique_id_database_t // sizeof=0x4
{                                                                             // XREF: .data:gjk_unique_id_database_t g_gjk_unique_id_database/r
        volatile unsigned int m_counter;


        unsigned int get_unique_id();
};

struct __declspec(align(8)) gjk_polygon_cylinder_t : gjk_base_t // sizeof=0x80
{                                                                             // XREF: ?render_gjkcc_collision@@YAXAAY02$$CBM00@Z/r
        phys_vec3 m_center;
        float m_polygon_cylinder_radius;
        float m_capsule_radius;
        float m_half_height;
        float m_geom_radius;
        float m_head_offset;
        float m_foot_offset;
        int m_mode;                                                 // XREF: render_gjkcc_collision(float const (&)[3],float const (&)[3],float const (&)[3])+10/w
        // padding byte
        // padding byte
        // padding byte
        // padding byte

        struct poly_verts // sizeof=0x20
        {                                                                             // XREF: .data:poly_verts s_poly_verts/r
                float m_co[4];
                float m_si[4];

                poly_verts();
                //void poly_verts::poly_verts(poly_verts *this);

                void get_co_si(int i, float *co_, float *si_);
                int __thiscall support(const phys_vec3 *v);
        };

        static gjk_polygon_cylinder_t *__cdecl create(
                float (*mins)[3],
                float (*maxs)[3],
                float radius_adjust,
                int stype,
                gjk_collision_visitor *allocator);
        static void __cdecl destroy(gjk_polygon_cylinder_t *geom);

        
        static void support(
            const phys_vec3 *v,
            phys_vec3 *support_vert,
            phys_vec3 *support_ind);
        static void get_simplex(
            const cached_simplex_info *cache_info,
            int index_count,
            phys_vec3 *simplex_verts,
            phys_vec3 *simplex_inds);
        static void calc_aabb(
            const phys_mat44 *xform,
            phys_vec3 *aabb_min,
            phys_vec3 *aabb_max);
        static void __cdecl calc_disc_aabb(
            const phys_vec3 *axis,
            float radius,
            phys_vec3 *aabb_min,
            phys_vec3 *aabb_max);

        bool __thiscall is_foot(const phys_vec3 *hit_point);
        unsigned int __thiscall get_type();
        float __thiscall get_geom_radius() const;
};

PhysGeomList *__cdecl xmodel_get_geomlist(const XModel *model, int bone_index);

void    phys_aabb_add_point(
        const phys_mat44 *xform,
        const phys_vec3 *point,
        phys_vec3 *aabb_min,
        phys_vec3 *aabb_max);

void    phys_aabb_init_sphere(
                const phys_mat44 *xform,
                const phys_vec3 *center,
                float radius,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max);
void    phys_aabb_add_sphere(
                const phys_mat44 *xform,
                const phys_vec3 *center,
                float radius,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max);


void    setup_gjk_polygon_cylinder(
                float *mins,
                float *maxs,
                float radius_adjust,
                gjk_polygon_cylinder_t *gjk_cylinder);

gjk_aabb_t * create_aabb_gjk_geom(
                const float *mn,
                const float *mx,
                int stype,
                gjk_collision_visitor *allocator);
gjk_obb_t * create_obb_gjk_geom(
                float (*orientation)[3],
                float *offset,
                float *halfLengths,
                int stype,
                gjk_collision_visitor *allocator);
gjk_brush_t *__cdecl create_brush_gjk_geom(const cbrush_t *brush, int stype, gjk_collision_visitor *allocator);
gjk_double_sphere_t * create_capsule_gjk_geom(
                float *center,
                float radius,
                float halfHeight,
                unsigned int direction,
                int stype,
                gjk_collision_visitor *allocator);
gjk_cylinder_t * create_cylinder_gjk_geom(
                float (*rot)[3],
                float *trans,
                float radius,
                float halfHeight,
                int stype,
                gjk_collision_visitor *allocator);
void __cdecl create_brush_model_gjk_geom_r(
                cLeafBrushNode_s *node,
                int *index_base,
                gjk_collision_visitor *allocator,
                int contents_mask);
void __cdecl query_brush_model_gjk_geom(
                unsigned __int16 brushModel,
                int contents_mask,
                gjk_collision_visitor *allocator);
void __cdecl create_brush_model_gjk_geom(
                unsigned __int16 brushModel,
                int contents_mask,
                gjk_collision_visitor *allocator);
void __cdecl create_xmodel_gjk_geom(
                const XModel *model,
                gjk_collision_visitor *allocator,
                int bone_index,
                unsigned int brush_mask,
                bool b_fallback_to_bounding_box,
                int contents_for_proxy_collision,
                phys_mat44 *worldMat);
void __cdecl create_gjk_geom(
                int localClientNum,
                const centity_s *cent,
                gjk_collision_visitor *allocator,
                bool calc_bone_mats,
                unsigned int brush_mask,
                bool b_fallback_to_bounding_box,
                bool b_use_smallerbox_for_characters);
void __cdecl create_gjk_dobj_geom(
                DObj *obj,
                const cpose_t *pose,
                gentity_s *gent,
                bool calc_bone_mats,
                unsigned int brush_mask,
                bool b_fallback_to_bounding_box,
                int contents_for_proxy_collision,
                gjk_collision_visitor *allocator);
void __cdecl create_gjk_geom(
                gentity_s *gent,
                gjk_collision_visitor *allocator,
                bool calc_bone_mats,
                unsigned int brush_mask,
                bool b_fallback_to_bounding_box);


void __cdecl create_gjk_geom(const Glass *glass, gjk_collision_visitor *allocator, unsigned int brush_mask);
void __cdecl create_gjk_geom(const DynEntityDef *dynEntDef, gjk_collision_visitor *allocator, unsigned int brush_mask);

void __cdecl destroy_gjk_geom(gjk_geom_list_t *gjk_geom_list);


extern create_gjk_geom_collision_visitor g_empty_collision_visitor;