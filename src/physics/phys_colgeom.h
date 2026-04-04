#pragma once

#include "phys_local.h"
#include "phys_traverse.h"
#include <universal/q_shared.h>
#include <qcommon/cm_trace.h>
#include <qcommon/statmonitor.h>
#include "phys_gjk_collision_detection.h"
#include <tl/tl_system.h>

#define SURF_TYPECOUNT 31

struct cpose_t;
struct gentity_s;
struct Glass;
struct DynEntityDef;
struct gjk_base_t;
struct cLeafBrushNode_s;
struct cbrush_t;
struct XModel;
struct centity_s;
struct DObj;
struct gjk_collision_visitor;
struct cbrushside_t;
struct gjk_geom_list_t;

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

struct phys_contact_manifold;

struct phys_gjk_geom // sizeof=0x4
{                                       // XREF: gjk_base_t/r
    //phys_gjk_geom_vtbl *__vftable;
    virtual void support(const phys_vec3 *, phys_vec3 *, phys_vec3 *) const = 0;
    virtual void get_simplex(const struct cached_simplex_info *, const int, phys_vec3 *, phys_vec3 *) = 0;
    virtual void set_simplex(const phys_vec3 *, const int, const phys_vec3 *, cached_simplex_info *);
    virtual const phys_vec3 *get_center(phys_vec3 *result) const = 0;
    virtual void get_feature(struct phys_contact_manifold *) const = 0;
    virtual float get_geom_radius() const
    {
        return 0.0f;
    }
    virtual void calc_aabb(const phys_mat44 *, phys_vec3 *, phys_vec3 *) const = 0;
    virtual bool ray_cast(const phys_vec3 *, const phys_vec3 *, const float, float *, phys_vec3 *);
    virtual bool is_polyhedron()
    {
        return false;
    }

    const phys_vec3 *support_only(const phys_vec3 *result, const phys_mat44 *xform, const phys_vec3 *v) const;
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

        unsigned int get_geom_id();
        const phys_mat44 *get_xform();

        int get_contents();
        void set_contents(int contents);

        void set_geom_id_new(unsigned int geom_id);
        void set_xform(const phys_mat44 *xform);

        virtual void comp_aabb_loc();
        virtual unsigned int get_type() const
        {
            iassert(0);
            return 0;
        }
        virtual bool is_foot(const phys_vec3 *hit_point) const;
        virtual bool is_walkable(const phys_vec3 *hit_point, const phys_vec3 *up);
        virtual const cbrush_t *get_brush() const;
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
                phys_vec3 *support_ind) const;
        void get_simplex(
                const cached_simplex_info *cache_info,
                int index_count,
                phys_vec3 *simplex_verts,
                phys_vec3 *simplex_inds);
        const phys_vec3 * get_center(phys_vec3 *result) const;
        void get_feature(phys_contact_manifold *cman) const;
        void calc_aabb(
                const phys_mat44 *xform,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max) const;
        const cbrush_t * get_brush();
        static void __cdecl destroy(gjk_aabb_t *geom);

        unsigned int get_type() const
        {
            iassert(0);
            return 0;
        }
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
            phys_vec3 *aabb_max) const;

        static void __cdecl destroy(gjk_obb_t *geom);
        const phys_vec3 *get_center(phys_vec3 *result) const;
        void get_feature(phys_contact_manifold *cman) const;
        void get_simplex(
            const cached_simplex_info *cache_info,
            int index_count,
            phys_vec3 *simplex_verts,
            phys_vec3 *simplex_inds);
        unsigned int get_type() const;
        bool is_polyhedron();
        void support(
                const phys_vec3 *v,
                phys_vec3 *support_vert,
                phys_vec3 *support_ind) const;
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

        static gjk_brush_t *create(
                const cbrush_t *brush,
                int stype,
                gjk_collision_visitor *allocator);
        void support(
                const phys_vec3 *v,
                phys_vec3 *support_vert,
                phys_vec3 *support_ind) const;
        void get_simplex(
                const cached_simplex_info *cache_info,
                int index_count,
                phys_vec3 *simplex_verts,
                phys_vec3 *simplex_inds);
        void get_feature(phys_contact_manifold *cman) const;
        void calc_aabb(
                const phys_mat44 *xform,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max) const;
        const cbrush_t *get_brush();
        unsigned int get_type() const;
        static void __cdecl destroy(gjk_brush_t *geom);
        const phys_vec3 *get_center(phys_vec3 *result) const;
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

struct CollisionAabbTree;

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

        const phys_vec3 *get_center(phys_vec3 *result) const
        {
            // This doesn't exist for this class (right?)
            iassert(0);
            return NULL;
        }

        static gjk_partition_t *__cdecl create(const CollisionAabbTree *tree, gjk_collision_visitor *allocator);
        void support(
                const phys_vec3 *v,
                phys_vec3 *support_vert,
                phys_vec3 *support_ind) const;
        void get_simplex(
                const cached_simplex_info *cache_info,
                int index_count,
                phys_vec3 *simplex_verts,
                phys_vec3 *simplex_inds);
        void get_feature(phys_contact_manifold *cman) const;
        void calc_aabb(
                const phys_mat44 *xform,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max) const;
        unsigned int get_type();
        static void __cdecl destroy(gjk_partition_t *geom);

        inline unsigned int get_type() const
        {
            return 3;
        }
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
                phys_vec3 *support_ind) const;
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
        const phys_vec3 *get_center(phys_vec3 *result) const;
        void get_feature(phys_contact_manifold *cman) const;
        void calc_aabb(
                const phys_mat44 *xform,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max) const;
        float get_geom_radius() const;
        bool is_polyhedron();
        static gjk_double_sphere_t *__cdecl create(
                const phys_vec3 *c0,
                const phys_vec3 *c1,
                float r,
                int stype,
                gjk_collision_visitor *allocator);

        unsigned int get_type() const // LWSS: this isnt even a real function?
        {
            return 69;
        }

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

        static gjk_cylinder_t *__cdecl create(
                int _direction,
                float _halfHeight,
                float _radius,
                const phys_mat44 *_xform,
                int stype,
                gjk_collision_visitor *allocator);
        void support(
                const phys_vec3 *v,
                phys_vec3 *support_vert,
                phys_vec3 *support_ind) const;
        const phys_vec3 * get_dims(phys_vec3 *result) const;
        void get_simplex(
                const cached_simplex_info *cache_info,
                int index_count,
                phys_vec3 *simplex_verts,
                phys_vec3 *simplex_inds);
        const phys_vec3 * get_center(phys_vec3 *result) const;
        void get_feature(phys_contact_manifold *cman) const;
        void calc_aabb(
                const phys_mat44 *xform_,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max) const;
        unsigned int get_type() const;
        float get_geom_radius() const;
        static void __cdecl destroy(gjk_cylinder_t *geom);
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

                //void get_co_si(int i, float *co_, float *si_);
                //void __thiscall gjk_polygon_cylinder_t::poly_verts::get_co_si(int i, float *co_, float *si_)
                inline void __thiscall get_co_si(int i, float *co_, float *si_)
                {
#if 0
                    int ii; // [esp+8h] [ebp-4h]

                    if ((unsigned int)i >= 0xC
                        && !Assert_MyHandler(
                            "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
                            802,
                            0,
                            "%s",
                            "i >= 0 && i < NUM_VERTS_"))
                    {
                        __debugbreak();
                    }
                    ii = `gjk_polygon_cylinder_t::poly_verts::get_co_si'::`2'::a[i / 3]
                        + i * `gjk_polygon_cylinder_t: : poly_verts::get_co_si'::`2'::b[i / 3];
                    *co_ = `gjk_polygon_cylinder_t::poly_verts::get_co_si'::`2'::cos[i / 3] * this->m_co[ii];
                    *si_ = `gjk_polygon_cylinder_t::poly_verts::get_co_si'::`2'::sis[i / 3] * this->m_si[ii];
#endif

                    static constexpr int NUM_VERTS_ = 12;
                    static constexpr int VERTS_PER_FACE = 3;

                    static constexpr float sis[4] = { 1.0f,  1.0f, -1.0f, -1.0f };
                    static constexpr float cosv[4] = { 1.0f, -1.0f, -1.0f,  1.0f };
                    static constexpr int a[4] = { 0, 6, -6, 12 };
                    static constexpr int b[4] = { 1, -1, 1, -1 };

                    iassert(i >= 0 && i < NUM_VERTS_);

                    const int face = i / VERTS_PER_FACE;
                    const int ii = a[face] + i * b[face];

                    *co_ = cosv[face] * m_co[ii];
                    *si_ = sis[face] * m_si[ii];
                }
                //int __thiscall support(const phys_vec3 *v);
                //int gjk_polygon_cylinder_t::poly_verts::support(const phys_vec3 *v)
                inline int support(const phys_vec3 *v)
                {
                    // aislop
                    float at;

                    // atan2(y, x)
                    at = atan2f(v->y, v->x);

                    // wrap to [0, 2PI)
                    if (at < 0.0f)
                        at += 2.0f * 3.1415927f;

                    // map angle to vertex index
                    int best_i = (int)((12.0f * at) / (2.0f * 3.1415927f) + 0.5f) % 12;

                    if (best_i < 0 || best_i >= 12)
                    {
                        Assert_MyHandler(
                            "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
                            791,
                            0,
                            "%s",
                            "best_i >= 0 && best_i < NUM_VERTS_");
                        __debugbreak();
                    }

                    return best_i;
                }
        };

        static poly_verts s_poly_verts;

        static gjk_polygon_cylinder_t *__cdecl create(
                float (*mins)[3],
                float (*maxs)[3],
                float radius_adjust,
                int stype,
                gjk_collision_visitor *allocator);
        static void __cdecl destroy(gjk_polygon_cylinder_t *geom);


        const phys_vec3 *get_center(phys_vec3 *result) const
        {
            iassert(0);
            return NULL;
        }
        void get_feature(struct phys_contact_manifold *) const
        {
            iassert(0);
        }
        
        void support(
            const phys_vec3 *v,
            phys_vec3 *support_vert,
            phys_vec3 *support_ind) const;

        void get_simplex(
            const cached_simplex_info *cache_info,
            int index_count,
            phys_vec3 *simplex_verts,
            phys_vec3 *simplex_inds);

        static inline void calc_disc_aabb(
            const phys_vec3 *axis,
            float radius,
            phys_vec3 *aabb_min,
            phys_vec3 *aabb_max)
        {
            float v4; // [esp-88h] [ebp-A8h]
            float v5; // [esp-84h] [ebp-A4h]
            float v6; // [esp-20h] [ebp-40h]
            float v7; // [esp-14h] [ebp-34h]
            float v8; // [esp-10h] [ebp-30h]

            v8 = axis->x * axis->x;
            v7 = axis->y * axis->y;
            v6 = axis->z * axis->z;
            aabb_max->x = radius * sqrtf(v7 + v6);
            aabb_max->y = radius * sqrtf(v8 + v6);
            aabb_max->z = radius * sqrtf(v8 + v7);
            v4 = -aabb_max->y;
            v5 = -aabb_max->z;
            aabb_min->x = -aabb_max->x;
            aabb_min->y = v4;
            aabb_min->z = v5;
        }

        void calc_aabb(
            const phys_mat44 *xform,
            phys_vec3 *aabb_min,
            phys_vec3 *aabb_max) const;

        inline bool is_foot(const phys_vec3 *hit_point) const
        {
            static float offset_0;
            //if (this->m_mode)
            //    return (float)((float)((float)(this->m_center.z - this->m_half_height) + this->m_capsule_radius) - 1.0) >= hit_point->z;
            //
            //static int _S2 = 0;
            //if ((_S2 & 1) == 0)
            //{
            //    _S2 |= 1u;
            //    //__libm_sse2_sin(*(long double *)&thisa);
            //    offset_0 = this->m_geom_radius * (float)((float)(30.0 * 3.1415927) / 180.0);
            //}
            //return (float)((float)((float)(this->m_center.z - this->m_half_height) + this->m_foot_offset) - offset_0) >= hit_point->z;

            // aislop rewrote
            const float bottom_z = m_center.z - m_half_height;

            if (m_mode)
            {
                // Capsule-style foot
                return hit_point->z <= (bottom_z + m_capsule_radius - 1.0f);
            }

            // Cylinder-style foot with beveled edge (30 degrees)
            constexpr float sin_30 = 0.5f;
            const float bevel_offset = m_geom_radius * sin_30;

            return hit_point->z <= (bottom_z + m_foot_offset - bevel_offset);
        }

        inline unsigned int get_type() const
        {
            return 7;
        }

        float get_geom_radius() const
        {
            if (this->m_mode)
                return this->m_geom_radius + this->m_capsule_radius;
            else
                return this->m_geom_radius;
        }

        //gjk_polygon_cylinder_t::poly_verts gjk_polygon_cylinder_t::s_poly_verts
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


//extern create_gjk_geom_collision_visitor g_empty_collision_visitor;
//extern gjk_unique_id_database_t g_gjk_unique_id_database;