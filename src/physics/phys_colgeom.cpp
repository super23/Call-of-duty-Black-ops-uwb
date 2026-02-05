#include "phys_colgeom.h"

#include <xanim/xmodel_utils.h>

#include <new>
#include <cgame/cg_world.h>
#include <qcommon/cm_load.h>

phys_simple_allocator<gjk_aabb_t> aabb_pool;
phys_simple_allocator<gjk_obb_t> obb_pool;
phys_simple_allocator<gjk_brush_t> brush_pool;
phys_simple_allocator<gjk_partition_t> partition_pool;

gjk_unique_id_database_t g_gjk_unique_id_database;
const phys_vec3 g_phys_vec3_box_sgn[8];

static const char *g_contact_manifold_error_msg = "contact_manifold memory overflow: INCREASE phys_contact_manifold_process::ALLOCATOR_MEMORY_SIZE";

create_gjk_geom_collision_visitor g_empty_collision_visitor;

void gjk_base_t::comp_aabb_loc()
{
    if ((this->m_flags & 2) == 0)
    {
        this->m_flags |= 2u;
        this->calc_aabb(&PHYS_IDENTITY_MATRIX, &this->m_aabb_mn_loc, &this->m_aabb_mx_loc);
    }
}

const cbrush_t *gjk_base_t::get_brush()
{
    return 0;
}

int gjk_base_t::get_contents()
{
    // KISAKTODO
    //if ((this->m_flags & 0x10) == 0
    //    && _tlAssert(
    //        "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
    //        90,
    //        "get_flag(FLAG_CONTENTS_VALID)",
    //        ""))
    //{
    //    __debugbreak();
    //}
    return this->m_contents;
}

unsigned int gjk_base_t::get_geom_id()
{
    //if ((this->m_flags & 4) == 0
    //    && _tlAssert(
    //        "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
    //        86,
    //        "get_flag(FLAG_GEOM_ID_VALID)",
    //        ""))
    //{
    //    __debugbreak();
    //}
    return this->m_gjk_geom_id;
}

const phys_mat44 *__thiscall gjk_base_t::get_xform()
{
    //if ((this->m_flags & 8) == 0
    //    && _tlAssert(
    //        "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
    //        105,
    //        "get_flag(FLAG_XFORM_VALID)",
    //        ""))
    //{
    //    __debugbreak();
    //}
    return this->m_xform_;
}

bool gjk_base_t::is_foot(const phys_vec3 *hit_point) const
{
    return 0;
}

bool gjk_base_t::is_walkable(const phys_vec3 *hit_point, const phys_vec3 *up)
{
    return 0;
}

void gjk_base_t::set_contents(int contents)
{
    this->m_flags |= 0x10u;
    this->m_contents = contents;
}

void gjk_base_t::set_geom_id_new(unsigned int geom_id)
{
    this->m_flags |= 4u;
    this->m_gjk_geom_id = geom_id;
}

void gjk_base_t::set_xform(const phys_mat44 *xform)
{
    iassert(xform);

    this->m_flags |= 8u;
    this->m_xform_ = xform;
}

PhysGeomList *__cdecl xmodel_get_geomlist(const XModel *model, int bone_index)
{
    if ( !model && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp", 58, 0, "%s", "model") )
        __debugbreak();
    if ( bone_index < 0 )
        return XModelGetCollmap(model, 0);
    else
        return XModelGetCollmapForBoneIndex(model, bone_index);
}

gjk_aabb_t *__cdecl gjk_aabb_t::create(
                const phys_vec3 *center,
                const phys_vec3 *dims,
                int stype,
                gjk_collision_visitor *allocator)
{
    unsigned int unique_id; // eax
    gjk_aabb_t *obj; // [esp+3Ch] [ebp-4h]

    if ( !allocator
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp", 71, 0, "%s", "allocator") )
    {
        __debugbreak();
    }
    if ( allocator->is_query() )
    {
        obj = (gjk_aabb_t *)allocator->allocate(128, 16, 0);
        if (obj)
        {
            new (obj) gjk_aabb_t();
            obj->m_flags = 0;
            obj->m_brush = 0;
            //obj->__vftable = (gjk_aabb_t_vtbl *)&phys_gjk_geom::`vftable';
            //obj->__vftable = (gjk_aabb_t_vtbl *)&gjk_base_t::`vftable';
            //obj->m_flags = 0;
            //obj->__vftable = (gjk_aabb_t_vtbl *)&gjk_aabb_t::`vftable';
            //obj->m_brush = 0;
        }
        else
        {
                obj = 0;
        }
        obj->m_flags |= 1u;
    }
    else
    {
        //obj = phys_simple_allocator<gjk_aabb_t>::allocate(&aabb_pool);
        obj = aabb_pool.allocate();
        //unique_id = gjk_unique_id_database_t::get_unique_id(&g_gjk_unique_id_database);
        unique_id = g_gjk_unique_id_database.get_unique_id();
        //gjk_base_t::set_geom_id_new(obj, unique_id);
        obj->set_geom_id_new(unique_id);
    }

    iassert(obj);

    obj->m_center_local.x = center->x;
    obj->m_center_local.y = center->y;
    obj->m_center_local.z = center->z;
    obj->m_dims.x = dims->x;
    obj->m_dims.y = dims->y;
    obj->m_dims.z = dims->z;

    iassert(stype < SURF_TYPECOUNT);

    obj->stype = stype;
    return obj;
}

unsigned int gjk_unique_id_database_t::get_unique_id()
{
    unsigned int id; // [esp+8h] [ebp-4h]

    do
    {
        id = this->m_counter;

        iassert(id != 0);
        //if ( !this->m_counter
        //    && _tlAssert(
        //             "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_broad_phase.h",
        //             153,
        //             "id != 0",
        //             "gjk unique_id wrapped around.") )
        //{
        //    __debugbreak();
        //}
    }
    while ( _InterlockedCompareExchange(&this->m_counter, id + 1, id) != id );

    return id;
}

void gjk_aabb_t::support(
                const phys_vec3 *v,
                phys_vec3 *support_vert,
                phys_vec3 *support_ind)
{
    float v4; // [esp-5Ch] [ebp-68h]
    float v5; // [esp-58h] [ebp-64h]
    float v6; // [esp-10h] [ebp-1Ch]
    float v7; // [esp-Ch] [ebp-18h]
    float v8; // [esp-8h] [ebp-14h]

    if ( v->z >= 0.0 )
        v8 = 1.0f;
    else
        v8 = -1.0f;
    if ( v->y >= 0.0 )
        v7 = 1.0f;
    else
        v7 = -1.0f;
    if ( v->x >= 0.0 )
        v6 = 1.0f;
    else
        v6 = -1.0f;
    support_ind->x = v6;
    support_ind->y = v7;
    support_ind->z = v8;
    v4 = (float)(support_ind->y * this->m_dims.y) + this->m_center_local.y;
    v5 = (float)(support_ind->z * this->m_dims.z) + this->m_center_local.z;
    support_vert->x = (float)(support_ind->x * this->m_dims.x) + this->m_center_local.x;
    support_vert->y = v4;
    support_vert->z = v5;
}

void gjk_aabb_t::get_simplex(
                const cached_simplex_info *cache_info,
                int index_count,
                phys_vec3 *simplex_verts,
                phys_vec3 *simplex_inds)
{
    float v5; // [esp-5Ch] [ebp-68h]
    float v6; // [esp-58h] [ebp-64h]
    phys_vec3 *v7; // [esp-10h] [ebp-1Ch]
    const float *p_x; // [esp-Ch] [ebp-18h]
    int i; // [esp-8h] [ebp-14h]

    for ( i = 0; i < index_count; ++i )
    {
        p_x = &cache_info->m_indices[i].x;
        v7 = &simplex_inds[i];
        v7->x = *p_x;
        v7->y = p_x[1];
        v7->z = p_x[2];
        v5 = (float)(p_x[1] * this->m_dims.y) + this->m_center_local.y;
        v6 = (float)(p_x[2] * this->m_dims.z) + this->m_center_local.z;
        simplex_verts[i].x = (float)(*p_x * this->m_dims.x) + this->m_center_local.x;
        simplex_verts[i].y = v5;
        simplex_verts[i].z = v6;
    }
}

const phys_vec3 *gjk_aabb_t::get_center(phys_vec3 *result)
{
    *result = this->m_center_local;
    return result;
}

void gjk_aabb_t::get_feature(phys_contact_manifold *cman)
{
    phys_vec3 v3; // [esp-50h] [ebp-5Ch] BYREF
    float v4; // [esp-40h] [ebp-4Ch]
    float v5; // [esp-3Ch] [ebp-48h]
    float v6; // [esp-38h] [ebp-44h]
    phys_vec3 *p_m_center_local; // [esp-34h] [ebp-40h]
    float v8; // [esp-30h] [ebp-3Ch]
    float v9; // [esp-2Ch] [ebp-38h]
    float v10; // [esp-28h] [ebp-34h]
    float v11; // [esp-1Ch] [ebp-28h]
    float v12; // [esp-18h] [ebp-24h]
    float v13; // [esp-14h] [ebp-20h]
    phys_vec3 *p_m_dims; // [esp-10h] [ebp-1Ch]
    const phys_vec3 *j; // [esp-Ch] [ebp-18h]
    const phys_mat44 *v16; // [esp-8h] [ebp-14h]
    gjk_aabb_t *v17; // [esp-4h] [ebp-10h]
    //const phys_vec3 *i; // [esp+0h] [ebp-Ch]
    //const phys_vec3 *last_i; // [esp+4h] [ebp-8h]
    const phys_vec3 *retaddr; // [esp+Ch] [ebp+0h]

    //i = a2;
    //last_i = retaddr;
    v17 = this;
    v16 = &PHYS_IDENTITY_MATRIX;
    for ( j = g_phys_vec3_box_sgn; j != (const phys_vec3 *)v16; ++j )
    {
        p_m_dims = &v17->m_dims;
        v13 = j->x * v17->m_dims.x;
        v12 = j->y * v17->m_dims.y;
        v11 = j->z * v17->m_dims.z;
        v8 = v13;
        v9 = v12;
        v10 = v11;
        p_m_center_local = &v17->m_center_local;
        v6 = v13 + v17->m_center_local.x;
        v5 = v12 + v17->m_center_local.y;
        v4 = v11 + v17->m_center_local.z;
        v3.x = v6;
        v3.y = v5;
        v3.z = v4;
        //phys_contact_manifold::add_feature_point(cman, &v3);
        cman->add_feature_point(&v3);
    }
}

void phys_contact_manifold::add_feature_point(const phys_vec3 *p)
{
    contact_manifold_mesh_point *mp; // [esp-8h] [ebp-58h]
    char *v3; // [esp-4h] [ebp-54h]
    float v4; // [esp+4h] [ebp-4Ch]
    float v5; // [esp+8h] [ebp-48h]
    float v6; // [esp+Ch] [ebp-44h]
    float orth_dist_sq; // [esp+14h] [ebp-3Ch]
    float orth_dist; // [esp+18h] [ebp-38h]
    float v9; // [esp+1Ch] [ebp-34h]

    orth_dist_sq = p->x - this->m_feature_hitp.x;
    orth_dist = p->y - this->m_feature_hitp.y;
    v9 = p->z - this->m_feature_hitp.z;
    v6 = (float)((float)(orth_dist_sq * this->m_feature_hitn.x) + (float)(orth_dist * this->m_feature_hitn.y))
         + (float)(v9 * this->m_feature_hitn.z);
    v5 = v6 * v6;
    v4 = (float)((float)(orth_dist_sq * orth_dist_sq) + (float)(orth_dist * orth_dist)) + (float)(v9 * v9);
    if ( this->m_feature_distance_eps >= v6 || (float)(v4 * this->m_sin_feautre_angular_eps_sq) >= v5 )
    {
        //v3 = phys_memory_heap::fast_allocate(this->m_allocator, 32, g_contact_manifold_error_msg);
        v3 = this->m_allocator->fast_allocate(32, g_contact_manifold_error_msg);
        if ( v3 )
            mp = (contact_manifold_mesh_point *)v3;
        else
            mp = 0;

        iassert((unsigned int)(mp) % PHYS_ALIGNOF(contact_manifold_mesh_point) == 0);
        iassert(mp == m_list_mesh_point + m_list_mesh_point_count);

        ++this->m_list_mesh_point_count;
        mp->m_p.x = orth_dist_sq;
        mp->m_p.y = orth_dist;
        mp->m_p.z = v9;
        mp->m_contact_p.y = v4;
        if ( v4 <= (float)(0.0033999998 * 0.0033999998) )
        {
            mp->m_contact_p.x = 2.0f;
            ++this->m_close_mesh_point_count;
        }
        else
        {
            mp->m_contact_p.x = 1.0 - (float)(v5 / v4);
        }
    }
}

char *phys_memory_heap::fast_allocate(int size, const char *error_msg)
{
    char *addr; // [esp+4h] [ebp-4h]

    addr = this->m_buffer_cur;
    this->m_buffer_cur = &addr[size];

    iassert(m_buffer_cur <= m_buffer_end);
    return addr;
}

void gjk_aabb_t::calc_aabb(
                const phys_mat44 *xform,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max)
{
    int savedregs; // [esp+128h] [ebp+0h] BYREF

    phys_calc_world_aabb(&this->m_center_local, &this->m_dims, xform, aabb_min, aabb_max);
}

const cbrush_t *gjk_aabb_t::get_brush()
{
    return this->m_brush;
}

void __cdecl gjk_aabb_t::destroy(gjk_aabb_t *geom)
{
    if ( geom )
    {
        iassert(!geom->get_flag(gjk_base_t::FLAG_TEMP_ALLOCATION));

        aabb_pool.free(geom);
        //phys_simple_allocator<gjk_polygon_cylinder_t>::free(
        //    (phys_simple_allocator<gjk_polygon_cylinder_t> *)&aabb_pool,
        //    (gjk_polygon_cylinder_t *)geom);
    }
}

gjk_obb_t dummy;
gjk_obb_t *__cdecl gjk_obb_t::create(
                const phys_mat44 *xform,
                const phys_vec3 *dims,
                int stype,
                gjk_collision_visitor *allocator)
{
    unsigned int unique_id; // eax
    gjk_obb_t *obj; // [esp+50h] [ebp-4h]

    iassert(allocator);
    if ( allocator->is_query() )
    {
        obj = (gjk_obb_t *)allocator->allocate(160, 16, 0);
        if (obj)
        {
            *obj = dummy; // set vtable this way
            //obj->__vftable = (gjk_obb_t_vtbl *)&phys_gjk_geom::`vftable';
            //obj->__vftable = (gjk_obb_t_vtbl *)&gjk_base_t::`vftable';
            //obj->__vftable = (gjk_obb_t_vtbl *)&gjk_obb_t::`vftable';
            obj->m_flags = 0;
        }

        obj->m_flags |= 1u; // seems unsafe but accurate
    }
    else
    {
        //obj = phys_simple_allocator<gjk_obb_t>::allocate(&obb_pool);
        obj = obb_pool.allocate();
        //unique_id = gjk_unique_id_database_t::get_unique_id(&g_gjk_unique_id_database);
        unique_id = g_gjk_unique_id_database.get_unique_id();
        //gjk_base_t::set_geom_id_new(obj, unique_id);
        obj->set_geom_id_new(unique_id);
    }

    iassert(obj);

    obj->m_dims.x = dims->x;
    obj->m_dims.y = dims->y;
    obj->m_dims.z = dims->z;

    //phys_mat44::operator=(&obj->m_xform, xform);
    obj->m_xform = xform;

    iassert(stype < SURF_TYPECOUNT);
    obj->stype = stype;
    return obj;
}

void gjk_obb_t::support(
                const phys_vec3 *v,
                phys_vec3 *support_vert,
                phys_vec3 *support_ind)
{
    const phys_vec3 *v5; // [esp-60h] [ebp-80h]
    phys_vec3 v6; // [esp-5Ch] [ebp-7Ch] BYREF
    phys_vec3 v7; // [esp-4Ch] [ebp-6Ch] BYREF
    float v8; // [esp-3Ch] [ebp-5Ch]
    float v9; // [esp-38h] [ebp-58h]
    float v10; // [esp-34h] [ebp-54h]
    phys_vec3 *p_m_dims; // [esp-30h] [ebp-50h]
    float v12; // [esp-2Ch] [ebp-4Ch]
    float v13; // [esp-28h] [ebp-48h]
    float v14; // [esp-24h] [ebp-44h]
    float v15; // [esp-18h] [ebp-38h]
    float v16; // [esp-14h] [ebp-34h]
    float v17; // [esp-10h] [ebp-30h]
    float v18[3]; // [esp-Ch] [ebp-2Ch] BYREF
    gjk_obb_t *v19; // [esp+10h] [ebp-10h]
    unsigned int v20[2]; // [esp+14h] [ebp-Ch] BYREF
    //_UNKNOWN *retaddr; // [esp+20h] [ebp+0h]

    //v20[0] = a2;
    //v20[1] = retaddr;
    v19 = this;
    phys_inv_multiply((const phys_vec3 *)v18, &this->m_xform, v);
    if ( v18[2] >= 0.0 )
        v17 = 1.0f;
    else
        v17 = -1.0f;
    if ( v18[1] >= 0.0 )
        v16 = 1.0f;
    else
        v16 = -1.0f;
    if ( v18[0] >= 0.0 )
        v15 = 1.0f;
    else
        v15 = -1.0f;
    v12 = v15;
    v13 = v16;
    v14 = v17;
    support_ind->x = v15;
    support_ind->y = v13;
    support_ind->z = v14;
    p_m_dims = &v19->m_dims;
    v10 = support_ind->x * v19->m_dims.x;
    v9 = support_ind->y * v19->m_dims.y;
    v8 = support_ind->z * v19->m_dims.z;
    v7.x = v10;
    v7.y = v9;
    v7.z = v8;
    v5 = phys_full_multiply((int)v20, &v6, &v19->m_xform, &v7);
    support_vert->x = v5->x;
    support_vert->y = v5->y;
    support_vert->z = v5->z;
}

void gjk_obb_t::get_simplex(
                const cached_simplex_info *cache_info,
                int index_count,
                phys_vec3 *simplex_verts,
                phys_vec3 *simplex_inds)
{
    phys_vec3 *v6; // [esp-58h] [ebp-64h]
    const phys_vec3 *v7; // [esp-54h] [ebp-60h]
    phys_vec3 v8; // [esp-50h] [ebp-5Ch] BYREF
    phys_vec3 v9; // [esp-40h] [ebp-4Ch] BYREF
    float v10; // [esp-24h] [ebp-30h]
    float v11; // [esp-20h] [ebp-2Ch]
    float v12; // [esp-1Ch] [ebp-28h]
    float *v13; // [esp-18h] [ebp-24h]
    phys_vec3 *p_m_dims; // [esp-14h] [ebp-20h]
    phys_vec3 *v15; // [esp-10h] [ebp-1Ch]
    float *p_x; // [esp-Ch] [ebp-18h]
    int j; // [esp-8h] [ebp-14h]
    gjk_obb_t *v18; // [esp-4h] [ebp-10h]
    int v19; // [esp+0h] [ebp-Ch] BYREF
    int i; // [esp+4h] [ebp-8h]
    int retaddr; // [esp+Ch] [ebp+0h]

    v19 = a2;
    i = retaddr;
    v18 = this;
    for ( j = 0; j < index_count; ++j )
    {
        p_x = &cache_info->m_indices[j].x;
        v15 = &simplex_inds[j];
        v15->x = *p_x;
        v15->y = p_x[1];
        v15->z = p_x[2];
        p_m_dims = &v18->m_dims;
        v13 = &cache_info->m_indices[j].x;
        v12 = *v13 * v18->m_dims.x;
        v11 = v13[1] * v18->m_dims.y;
        v10 = v13[2] * v18->m_dims.z;
        v9.x = v12;
        v9.y = v11;
        v9.z = v10;
        v7 = phys_full_multiply((int)&v19, &v8, &v18->m_xform, &v9);
        v6 = &simplex_verts[j];
        v6->x = v7->x;
        v6->y = v7->y;
        v6->z = v7->z;
    }
}

const phys_vec3 *gjk_obb_t::get_center(phys_vec3 *result)
{
    *result = this->m_xform.w;
    return result;
}

void gjk_obb_t::get_feature(phys_contact_manifold *cman)
{
    const phys_vec3 *v3; // eax
    phys_vec3 v4; // [esp-40h] [ebp-4Ch] BYREF
    phys_vec3 v5; // [esp-30h] [ebp-3Ch] BYREF
    float v6; // [esp-1Ch] [ebp-28h]
    float v7; // [esp-18h] [ebp-24h]
    float v8; // [esp-14h] [ebp-20h]
    phys_vec3 *p_m_dims; // [esp-10h] [ebp-1Ch]
    const phys_vec3 *j; // [esp-Ch] [ebp-18h]
    phys_mat44 *v11; // [esp-8h] [ebp-14h]
    gjk_obb_t *v12; // [esp-4h] [ebp-10h]
    const phys_vec3 *i; // [esp+0h] [ebp-Ch] BYREF
    const phys_vec3 *last_i; // [esp+4h] [ebp-8h]
    const phys_vec3 *retaddr; // [esp+Ch] [ebp+0h]

    i = a2;
    last_i = retaddr;
    v12 = this;
    v11 = &PHYS_IDENTITY_MATRIX;
    for ( j = g_phys_vec3_box_sgn; j != (const phys_vec3 *)v11; ++j )
    {
        p_m_dims = &v12->m_dims;
        v8 = j->x * v12->m_dims.x;
        v7 = j->y * v12->m_dims.y;
        v6 = j->z * v12->m_dims.z;
        v5.x = v8;
        v5.y = v7;
        v5.z = v6;
        v3 = phys_full_multiply((int)&i, &v4, &v12->m_xform, &v5);
        cman->add_feature_point(v3);
        //phys_contact_manifold::add_feature_point(cman, v3);
    }
}

void gjk_obb_t::calc_aabb(
    const phys_mat44 *xform,
    phys_vec3 *aabb_min,
    phys_vec3 *aabb_max)
{
    phys_vec3 v5; // [esp-30h] [ebp-BCh] BYREF
    float v6; // [esp-1Ch] [ebp-A8h]
    float v7; // [esp-18h] [ebp-A4h]
    float v8; // [esp-14h] [ebp-A0h]
    phys_vec3 *v9; // [esp-10h] [ebp-9Ch]
    const phys_vec3 *j; // [esp-Ch] [ebp-98h]
    phys_mat44 *v11; // [esp-8h] [ebp-94h]
    const phys_vec3 *v12; // [esp-4h] [ebp-90h]
    const phys_vec3 *i; // [esp+0h] [ebp-8Ch] BYREF
    phys_vec3 v14; // [esp+10h] [ebp-7Ch] BYREF
    float v15; // [esp+20h] [ebp-6Ch]
    float v16; // [esp+24h] [ebp-68h]
    float v17; // [esp+28h] [ebp-64h]
    phys_vec3 *p_m_dims; // [esp+2Ch] [ebp-60h]
    _BYTE v19[12]; // [esp+30h] [ebp-5Ch] BYREF
    gjk_obb_t *v20; // [esp+7Ch] [ebp-10h]
    _DWORD v21[2]; // [esp+80h] [ebp-Ch] BYREF
    _UNKNOWN *retaddr; // [esp+8Ch] [ebp+0h]

    v21[0] = a2;
    v21[1] = retaddr;
    v20 = this;
    phys_full_multiply_mat((int)v21, (phys_mat44 *)v19, xform, &this->m_xform);
    p_m_dims = &v20->m_dims;
    v17 = v20->m_dims.x * g_phys_vec3_box_sgn[0].x;
    v16 = v20->m_dims.y * g_phys_vec3_box_sgn[0].y;
    v15 = v20->m_dims.z * g_phys_vec3_box_sgn[0].z;
    v14.x = v17;
    v14.y = v16;
    v14.z = v15;
    v12 = phys_full_multiply((int)v21, (const phys_vec3 *)&i, (const phys_mat44 *)v19, &v14);
    aabb_min->x = v12->x;
    aabb_min->y = v12->y;
    aabb_min->z = v12->z;
    aabb_max->x = aabb_min->x;
    aabb_max->y = aabb_min->y;
    aabb_max->z = aabb_min->z;
    v11 = &PHYS_IDENTITY_MATRIX;
    for (j = &g_phys_vec3_box_sgn[1]; j != (const phys_vec3 *)v11; ++j)
    {
        v9 = &v20->m_dims;
        v8 = v20->m_dims.x * j->x;
        v7 = v20->m_dims.y * j->y;
        v6 = v20->m_dims.z * j->z;
        v5.x = v8;
        v5.y = v7;
        v5.z = v6;
        phys_aabb_add_point(COERCE_FLOAT(v21), (const phys_mat44 *)v19, &v5, aabb_min, aabb_max);
    }
}

void    phys_aabb_add_point(
                const phys_mat44 *xform,
                const phys_vec3 *point,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max)
{
    const phys_vec3 *v5; // [esp-40h] [ebp-50h]
    phys_vec3 v6; // [esp-3Ch] [ebp-4Ch] BYREF
    const phys_vec3 *v7; // [esp-20h] [ebp-30h]
    phys_vec3 v8; // [esp-1Ch] [ebp-2Ch] BYREF
    _BYTE v9[12]; // [esp-Ch] [ebp-1Ch] BYREF
    phys_vec3 point_abs; // [esp+0h] [ebp-10h] BYREF
    float retaddr; // [esp+10h] [ebp+0h]

    point_abs.y = a1;
    point_abs.z = retaddr;
    phys_full_multiply((int)&point_abs.y, (const phys_vec3 *)v9, xform, point);
    v7 = phys_min(&v8, aabb_min, (const phys_vec3 *)v9);
    aabb_min->x = v7->x;
    aabb_min->y = v7->y;
    aabb_min->z = v7->z;
    v5 = phys_max(&v6, aabb_max, (const phys_vec3 *)v9);
    aabb_max->x = v5->x;
    aabb_max->y = v5->y;
    aabb_max->z = v5->z;
}

unsigned int gjk_obb_t::get_type()
{
    return 6;
}

void __cdecl gjk_obb_t::destroy(gjk_obb_t *geom)
{
    if ( geom )
    {
        iassert(!geom->get_flag(gjk_base_t::FLAG_TEMP_ALLOCATION));

        //phys_simple_allocator<gjk_cylinder_t>::free((phys_simple_allocator<gjk_cylinder_t> *)&obb_pool, geom);
        obb_pool.free(geom);
    }
}

gjk_brush_t dummybrush;
gjk_brush_t * gjk_brush_t::create(
                const cbrush_t *brush,
                int stype,
                gjk_collision_visitor *allocator)
{
    unsigned int unique_id; // eax
    unsigned int numverts; // [esp-14h] [ebp-D4h]
    gjk_brush_t *v7; // [esp-Ch] [ebp-CCh]
    gjk_brush_t *v8; // [esp-8h] [ebp-C8h]
    gjk_brush_t *v9; // [esp-4h] [ebp-C4h]
    gjk_aabb_t *v10; // [esp+10h] [ebp-B0h]
    float v11[2]; // [esp+14h] [ebp-ACh] BYREF
    gjk_aabb_t *obj; // [esp+1Ch] [ebp-A4h]
    phys_vec3 half_dims; // [esp+20h] [ebp-A0h]
    float v14; // [esp+30h] [ebp-90h]
    float v15; // [esp+34h] [ebp-8Ch]
    float v16; // [esp+38h] [ebp-88h]
    float v17; // [esp+3Ch] [ebp-84h]
    float v18; // [esp+48h] [ebp-78h]
    float v19; // [esp+4Ch] [ebp-74h]
    float v20; // [esp+50h] [ebp-70h]
    float v21[3]; // [esp+54h] [ebp-6Ch] BYREF
    phys_vec3 center; // [esp+60h] [ebp-60h]
    float v23; // [esp+70h] [ebp-50h]
    float v24; // [esp+74h] [ebp-4Ch]
    float v25; // [esp+78h] [ebp-48h]
    float v26; // [esp+7Ch] [ebp-44h]
    float v27; // [esp+88h] [ebp-38h]
    float v28; // [esp+8Ch] [ebp-34h]
    float v29; // [esp+90h] [ebp-30h]
    float v30; // [esp+94h] [ebp-2Ch] BYREF
    float v31; // [esp+98h] [ebp-28h]
    float v32; // [esp+9Ch] [ebp-24h]
    phys_vec3 aabb_max; // [esp+A0h] [ebp-20h] BYREF
    phys_vec3 aabb_min; // [esp+B0h] [ebp-10h]
    float retaddr; // [esp+C0h] [ebp+0h]

    //aabb_min.y = a1;
    //aabb_min.z = retaddr;
    if ( brush->numsides )
    {
        if ( !allocator
            && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp", 122, 0, "%s", "allocator") )
        {
            __debugbreak();
        }
        if ( allocator->is_query() )
        {
            v9 = (gjk_brush_t *)allocator->allocate(96, 16, 0);
            if ( v9 )
            {
                //v9->__vftable = (gjk_brush_t_vtbl *)&phys_gjk_geom::`vftable';
                //v9->__vftable = (gjk_brush_t_vtbl *)&gjk_base_t::`vftable';
                //v9->m_flags = 0;
                //v9->__vftable = (gjk_brush_t_vtbl *)&gjk_brush_t::`vftable';
                //v8 = v9;
                *v9 = dummybrush;
                v9->m_flags = 0;
            }
            else
            {
                v8 = 0;
            }
            v7 = v8;
            v8->m_flags |= 1u;
        }
        else
        {
            //v7 = phys_simple_allocator<gjk_brush_t>::allocate(&brush_pool);
            v7 = brush_pool.allocate();
            //unique_id = gjk_unique_id_database_t::get_unique_id(&g_gjk_unique_id_database);
            unique_id = g_gjk_unique_id_database.get_unique_id();
            //gjk_base_t::set_geom_id_new(v7, unique_id);
            v7->set_geom_id_new(unique_id);
        }
        if ( !v7 && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp", 122, 0, "%s", "obj") )
            __debugbreak();
        numverts = brush->numverts;
        v7->verts = brush->verts;
        v7->nverts = numverts;
        v7->brush = brush;
        if ( stype >= 31
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp",
                        124,
                        0,
                        "%s",
                        "stype < SURF_TYPECOUNT") )
        {
            __debugbreak();
        }
        v7->stype = stype;
        //gjk_base_t::set_contents(v7, brush->contents);
        v7->set_contents(brush->contents);
        return v7;
    }
    else
    {
        Phys_Vec3ToNitrousVec((phys_vec3 *)&aabb_max.y, brush->mins);
        Phys_Vec3ToNitrousVec((const phys_vec3 *)&v30, brush->maxs);
        v29 = aabb_max.y + v30;
        v28 = aabb_max.z + v31;
        v27 = aabb_max.w + v32;
        v24 = aabb_max.y + v30;
        v25 = aabb_max.z + v31;
        v26 = aabb_max.w + v32;
        v23 = 0.5 * (float)(aabb_max.y + v30);
        center.w = 0.5 * (float)(aabb_max.z + v31);
        center.z = 0.5 * (float)(aabb_max.w + v32);
        v21[0] = v23;
        v21[1] = center.w;
        v21[2] = center.z;
        v20 = v30 - aabb_max.y;
        v19 = v31 - aabb_max.z;
        v18 = v32 - aabb_max.w;
        v15 = v30 - aabb_max.y;
        v16 = v31 - aabb_max.z;
        v17 = v32 - aabb_max.w;
        v14 = 0.5 * (float)(v30 - aabb_max.y);
        half_dims.w = 0.5 * (float)(v31 - aabb_max.z);
        half_dims.z = 0.5 * (float)(v32 - aabb_max.w);
        v11[0] = v14;
        v11[1] = half_dims.w;
        obj = (gjk_aabb_t *)LODWORD(half_dims.z);
        v10 = gjk_aabb_t::create((const phys_vec3 *)v21, (const phys_vec3 *)v11, stype, allocator);
        //gjk_base_t::set_contents(v10, brush->contents);
        v10->set_contents(brush->contents);
        v10->m_brush = brush;
        return (gjk_brush_t *)v10;
    }
}

void gjk_brush_t::support(
                const phys_vec3 *v,
                phys_vec3 *support_vert,
                phys_vec3 *support_ind)
{
    int i; // [esp+10h] [ebp-18h]
    int best_i; // [esp+14h] [ebp-14h]
    float vec3_v[3]; // [esp+18h] [ebp-10h] BYREF
    float best_d; // [esp+24h] [ebp-4h]

    Phys_NitrousVecToVec3(v, vec3_v);
    best_i = 0;
    best_d = (float)((float)((*this->verts)[0] * vec3_v[0]) + (float)((float)(*this->verts)[1] * vec3_v[1]))
                 + (float)((float)(*this->verts)[2] * vec3_v[2]);
    for ( i = 1; i < this->nverts; ++i )
    {
        if ( (float)((float)((float)((float)this->verts[i][0] * vec3_v[0]) + (float)((float)this->verts[i][1] * vec3_v[1]))
                             + (float)((float)this->verts[i][2] * vec3_v[2])) > best_d )
        {
            best_i = i;
            best_d = (float)((float)((float)this->verts[i][0] * vec3_v[0]) + (float)((float)this->verts[i][1] * vec3_v[1]))
                         + (float)((float)this->verts[i][2] * vec3_v[2]);
        }
    }
    Phys_Vec3ToNitrousVec((float *)this->verts[best_i], support_vert);
    LODWORD(support_ind->x) = best_i;
}

void gjk_brush_t::get_simplex(
                const cached_simplex_info *cache_info,
                int index_count,
                phys_vec3 *simplex_verts,
                phys_vec3 *simplex_inds)
{
    float *p_x; // [esp+4h] [ebp-10h]
    phys_vec3 *v7; // [esp+8h] [ebp-Ch]
    int i; // [esp+10h] [ebp-4h]

    for ( i = 0; i < index_count; ++i )
    {
        p_x = &cache_info->m_indices[i].x;
        v7 = &simplex_inds[i];
        v7->x = *p_x;
        v7->y = p_x[1];
        v7->z = p_x[2];
        Phys_Vec3ToNitrousVec((float *)this->verts[*(unsigned int *)p_x], &simplex_verts[i]);
    }
}

void gjk_brush_t::get_feature(phys_contact_manifold *cman)
{
    phys_vec3 vec;

    for (int i = 0; i < this->nverts; i++ )
    {
        Phys_Vec3ToNitrousVec((float *)this->verts[i], &vec);
        cman->add_feature_point(&vec);
    }
}

void gjk_brush_t::calc_aabb(
                const phys_mat44 *xform,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max)
{
    const phys_vec3 *v5; // eax
    int i; // [esp-Ch] [ebp-44h]
    phys_vec3 vec; // [esp+Ch] [ebp-2Ch] BYREF

    Phys_Vec3ToNitrousVec((float *)this->verts, &vec);
    v5 = phys_full_multiply(xform, &vec);

    aabb_min->x = v5->x;
    aabb_min->y = v5->y;
    aabb_min->z = v5->z;

    aabb_max->x = aabb_min->x;
    aabb_max->y = aabb_min->y;
    aabb_max->z = aabb_min->z;

    for (i = 1; i < this->nverts; ++i)
    {
        Phys_Vec3ToNitrousVec((float *)this->verts[i], &vec);
        phys_aabb_add_point(xform, &vec, aabb_min, aabb_max);
    }
}

const cbrush_t *gjk_brush_t::get_brush()
{
    return this->brush;
}

unsigned int gjk_brush_t::get_type()
{
    return 2;
}

bool gjk_obb_t::is_polyhedron()
{
    return 1;
}

void __cdecl gjk_brush_t::destroy(gjk_brush_t *geom)
{
    if ( geom )
    {
        iassert(!geom->get_flag(gjk_base_t::FLAG_TEMP_ALLOCATION));
        //phys_simple_allocator<gjk_brush_t>::free(&brush_pool, geom);
        brush_pool.free(geom);
    }
}

gjk_partition_t dummypart;
gjk_partition_t *__cdecl gjk_partition_t::create(const CollisionAabbTree *tree, gjk_collision_visitor *allocator)
{
    unsigned int unique_id; // eax
    gjk_partition_t *v4; // [esp+4h] [ebp-48h]
    int nuinds; // [esp+Ch] [ebp-40h]
    unsigned __int16 *v6; // [esp+10h] [ebp-3Ch]
    gjk_partition_t *obj; // [esp+3Ch] [ebp-10h]
    dmaterial_t *materialInfo; // [esp+40h] [ebp-Ch]
    const CollisionPartition *partition; // [esp+48h] [ebp-4h]

    if ( !allocator
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp", 142, 0, "%s", "allocator") )
    {
        __debugbreak();
    }
    if ( allocator->is_query() )
    {
        obj = (gjk_partition_t *)allocator->allocate(112, 16, 0);
        if ( obj )
        {
            //obj->__vftable = (gjk_partition_t_vtbl *)&phys_gjk_geom::`vftable';
            //obj->__vftable = (gjk_partition_t_vtbl *)&gjk_base_t::`vftable';
            //obj->__vftable = (gjk_partition_t_vtbl *)&gjk_partition_t::`vftable';
            //v4 = obj;
            *obj = dummypart;
            obj->m_flags = 0;
        }
        else
        {
            v4 = 0;
        }
        obj = v4;
        v4->m_flags |= 1u;
    }
    else
    {
        //obj = phys_simple_allocator<gjk_partition_t>::allocate(&partition_pool);
        obj = partition_pool.allocate();
        //unique_id = gjk_unique_id_database_t::get_unique_id(&g_gjk_unique_id_database);
        unique_id = g_gjk_unique_id_database.get_unique_id();
        //gjk_base_t::set_geom_id_new(obj, unique_id);
        obj->set_geom_id_new(unique_id);
    }
    iassert(obj);
    partition = &cm.partitions[tree->u.firstChildIndex];
    nuinds = partition->nuinds;
    v6 = &cm.uinds[partition->fuind];
    obj->verts = cm.verts;
    obj->inds = v6;
    obj->ninds = nuinds;
    obj->partition = partition;
    obj->tree = tree;
    materialInfo = &cm.materials[tree->materialIndex];
    obj->stype = (materialInfo->surfaceFlags & 0x3F00000) >> 20;
    iassert(obj->stype < SURF_TYPECOUNT);
    //gjk_base_t::set_contents(obj, materialInfo->contentFlags);
    obj->set_contents(materialInfo->contentFlags);
    return obj;
}

void gjk_partition_t::support(
                const phys_vec3 *v,
                phys_vec3 *support_vert,
                phys_vec3 *support_ind)
{
    float *v5; // [esp+4h] [ebp-24h]
    float *v6; // [esp+8h] [ebp-20h]
    int i; // [esp+10h] [ebp-18h]
    int best_i; // [esp+14h] [ebp-14h]
    float vec3_v[3]; // [esp+18h] [ebp-10h] BYREF
    float best_d; // [esp+24h] [ebp-4h]

    Phys_NitrousVecToVec3(v, vec3_v);
    best_i = 0;
    v6 = (float *)this->verts[*this->inds];
    best_d = (float)((float)(*v6 * vec3_v[0]) + (float)(v6[1] * vec3_v[1])) + (float)(v6[2] * vec3_v[2]);
    for ( i = 1; i < this->ninds; ++i )
    {
        v5 = (float *)this->verts[this->inds[i]];
        if ( (float)((float)((float)(*v5 * vec3_v[0]) + (float)(v5[1] * vec3_v[1])) + (float)(v5[2] * vec3_v[2])) > best_d )
        {
            best_i = i;
            best_d = (float)((float)(*v5 * vec3_v[0]) + (float)(v5[1] * vec3_v[1])) + (float)(v5[2] * vec3_v[2]);
        }
    }
    Phys_Vec3ToNitrousVec((float *)this->verts[this->inds[best_i]], support_vert);
    LODWORD(support_ind->x) = best_i;
}

void gjk_partition_t::get_simplex(
                const cached_simplex_info *cache_info,
                int index_count,
                phys_vec3 *simplex_verts,
                phys_vec3 *simplex_inds)
{
    const float *p_x; // [esp+4h] [ebp-10h]
    phys_vec3 *v7; // [esp+8h] [ebp-Ch]
    int i; // [esp+10h] [ebp-4h]

    for ( i = 0; i < index_count; ++i )
    {
        p_x = &cache_info->m_indices[i].x;
        v7 = &simplex_inds[i];
        v7->x = *p_x;
        v7->y = p_x[1];
        v7->z = p_x[2];
        Phys_Vec3ToNitrousVec((float *)this->verts[this->inds[*(unsigned int *)p_x]], &simplex_verts[i]);
    }
}

const phys_vec3 *gjk_brush_t::get_center(phys_vec3 *result)
{
    result->x = 0.0f;
    result->y = 0.0f;
    result->z = 0.0f;
    return result;
}

void gjk_partition_t::get_feature(phys_contact_manifold *cman)
{
    phys_vec3 v; // [esp+0h] [ebp-20h]


    for (int i = 0; i < this->ninds; i++)
    {
        //Phys_Vec3ToNitrousVec((float *)v5->verts[v5->inds[LODWORD(v.w)]], (phys_vec3 *)v3);
        Phys_Vec3ToNitrousVec((float *)this->verts[this->inds[i]], &v);
        //phys_contact_manifold::add_feature_point(cman, (const phys_vec3 *)v3);
        cman->add_feature_point(&v);
    }
}

void gjk_partition_t::calc_aabb(
                const phys_mat44 *xform,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max)
{
    phys_vec3 v5; // [esp-Ch] [ebp-5Ch] BYREF
    const phys_vec3 *v6; // [esp+10h] [ebp-40h]
    phys_vec3 v7; // [esp+14h] [ebp-3Ch] BYREF
    _BYTE v8[12]; // [esp+24h] [ebp-2Ch] BYREF
    phys_vec3 v; // [esp+30h] [ebp-20h]
    gjk_partition_t *v10; // [esp+40h] [ebp-10h]
    unsigned __int16 *last_inds_i; // [esp+44h] [ebp-Ch] BYREF
    unsigned __int16 *inds_i; // [esp+48h] [ebp-8h]
    unsigned __int16 *retaddr; // [esp+50h] [ebp+0h]

    last_inds_i = a2;
    inds_i = retaddr;
    v10 = this;
    LODWORD(v.w) = this->inds;
    LODWORD(v.z) = LODWORD(v.w) + 2 * this->ninds;
    Phys_Vec3ToNitrousVec((float *)this->verts[(unsigned __int16)*(_WORD *)LODWORD(v.w)], (phys_vec3 *)v8);
    v6 = phys_full_multiply((int)&last_inds_i, &v7, xform, (const phys_vec3 *)v8);
    aabb_min->x = v6->x;
    aabb_min->y = v6->y;
    aabb_min->z = v6->z;
    aabb_max->x = aabb_min->x;
    aabb_max->y = aabb_min->y;
    aabb_max->z = aabb_min->z;
    for ( LODWORD(v.w) += 2; LODWORD(v.w) < LODWORD(v.z); LODWORD(v.w) += 2 )
    {
        Phys_Vec3ToNitrousVec((float *)v10->verts[(unsigned __int16)*(_WORD *)LODWORD(v.w)], &v5);
        phys_aabb_add_point(COERCE_FLOAT(&last_inds_i), xform, &v5, aabb_min, aabb_max);
    }
}

unsigned int gjk_partition_t::get_type()
{
    return 3;
}

void __cdecl gjk_partition_t::destroy(gjk_partition_t *geom)
{
    if ( geom )
    {
        iassert(!geom->get_flag(gjk_base_t::FLAG_TEMP_ALLOCATION));
        //phys_simple_allocator<gjk_partition_t>::free(&partition_pool, geom);
        partition_pool.free(geom);
    }
}

gjk_double_sphere_t::gjk_double_sphere_t()
{
    int mp; // [esp+4h] [ebp-8h]
    phys_vec3 *i; // [esp+8h] [ebp-4h]

    //this->__vftable = (gjk_double_sphere_t_vtbl *)&phys_gjk_geom::`vftable';
    //this->__vftable = (gjk_double_sphere_t_vtbl *)&gjk_base_t::`vftable';
    this->m_flags = 0;
    //this->__vftable = (gjk_double_sphere_t_vtbl *)&gjk_double_sphere_t::`vftable';
    mp = 2;
    for ( i = this->m_list_center; --mp >= 0; ++i )
        ;
    //return this;
}

void gjk_double_sphere_t::support(
                const phys_vec3 *v,
                phys_vec3 *support_vert,
                phys_vec3 *support_ind)
{
    float v4; // [esp-Ch] [ebp-7Ch]
    float v5; // [esp-8h] [ebp-78h]
    float v6; // [esp-4h] [ebp-74h]
    float v7; // [esp+30h] [ebp-40h]
    float v8; // [esp+58h] [ebp-18h]

    if ( this->m_count == 1 )
    {
        support_vert->x = this->m_list_center[0].x;
        support_vert->y = this->m_list_center[0].y;
        support_vert->z = this->m_list_center[0].z;
        support_ind->x = 0.0;
    }
    else
    {
        v8 = Abs(&v->x);
        if ( v8 <= 0.000001
            && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h", 449, 0, "%s", "nv > 0.000001f") )
        {
            __debugbreak();
        }
        v7 = this->m_list_radius[1];
        v4 = this->m_list_center[1].x + (float)(v7 * (float)((float)(1.0 / v8) * v->x));
        v5 = this->m_list_center[1].y + (float)(v7 * (float)((float)(1.0 / v8) * v->y));
        v6 = this->m_list_center[1].z + (float)(v7 * (float)((float)(1.0 / v8) * v->z));
        if ( (float)((float)((float)(v->x * (float)(this->m_list_center[0].x - v4))
                                             + (float)(v->y * (float)(this->m_list_center[0].y - v5)))
                             + (float)(v->z * (float)(this->m_list_center[0].z - v6))) < 0.0 )
        {
            support_vert->x = v4;
            support_vert->y = v5;
            support_vert->z = v6;
            LODWORD(support_ind->x) = 1;
        }
        else
        {
            support_vert->x = this->m_list_center[0].x;
            support_vert->y = this->m_list_center[0].y;
            support_vert->z = this->m_list_center[0].z;
            support_ind->x = 0.0;
        }
    }
}

void gjk_double_sphere_t::get_simplex(
                const cached_simplex_info *cache_info,
                int index_count,
                phys_vec3 *simplex_verts,
                phys_vec3 *simplex_inds)
{
    float v5; // [esp-48h] [ebp-58h]
    float v6; // [esp-44h] [ebp-54h]
    float v7; // [esp-10h] [ebp-20h]
    int ind; // [esp-Ch] [ebp-1Ch]
    int i; // [esp-8h] [ebp-18h]

    for ( i = 0; i < index_count; ++i )
    {
        ind = *((unsigned int *)&cache_info->m_indices[1].x + i);
        iassert(ind >= 0 && ind < m_count);
        LODWORD(simplex_inds[i].x) = ind;
        v7 = this->m_list_radius[ind];
        v5 = this->m_list_center[ind].y + (float)(v7 * cache_info->m_indices[0].y);
        v6 = this->m_list_center[ind].z + (float)(v7 * cache_info->m_indices[0].z);
        simplex_verts[i].x = this->m_list_center[ind].x + (float)(v7 * cache_info->m_indices[0].x);
        simplex_verts[i].y = v5;
        simplex_verts[i].z = v6;
    }
}

void gjk_double_sphere_t::set_simplex(
                const phys_vec3 *simplex_inds,
                int w_set,
                const phys_vec3 *normal,
                cached_simplex_info *cache_info)
{
    int i; // [esp+8h] [ebp-Ch]
    phys_vec3 *inds; // [esp+10h] [ebp-4h]

    cache_info->m_indices[0].x = normal->x;
    cache_info->m_indices[0].y = normal->y;
    cache_info->m_indices[0].z = normal->z;
    inds = &cache_info->m_indices[1];
    for ( i = 0; i < 4; ++i )
    {
        if ( (w_set & (1 << i)) != 0 )
        {
            inds->x = simplex_inds[i].x;
            inds = (phys_vec3 *)((char *)inds + 4);
        }
    }
}

const phys_vec3 *gjk_double_sphere_t::get_center(phys_vec3 *result)
{
    *result = this->m_center;
    return result;
}

void gjk_double_sphere_t::get_feature(phys_contact_manifold *cman)
{
    phys_vec3 v3; // [esp-50h] [ebp-5Ch] BYREF
    float v4; // [esp-40h] [ebp-4Ch]
    float v5; // [esp-3Ch] [ebp-48h]
    float v6; // [esp-38h] [ebp-44h]
    float *p_x; // [esp-34h] [ebp-40h]
    float v8; // [esp-30h] [ebp-3Ch]
    float v9; // [esp-2Ch] [ebp-38h]
    float v10; // [esp-28h] [ebp-34h]
    float v11; // [esp-1Ch] [ebp-28h]
    float v12; // [esp-18h] [ebp-24h]
    float v13; // [esp-14h] [ebp-20h]
    float v14; // [esp-10h] [ebp-1Ch]
    phys_vec3 *p_m_feature_hitn; // [esp-Ch] [ebp-18h]
    int j; // [esp-8h] [ebp-14h]
    gjk_double_sphere_t *v17; // [esp-4h] [ebp-10h]
    //int v18; // [esp+0h] [ebp-Ch]
    int i; // [esp+4h] [ebp-8h]
    int retaddr; // [esp+Ch] [ebp+0h]

    //v18 = a2;
    i = retaddr;
    v17 = this;
    for ( j = 0; j < v17->m_count; ++j )
    {
        p_m_feature_hitn = &cman->m_feature_hitn;
        v14 = v17->m_list_radius[j] + v17->m_geom_radius;
        v13 = v14 * cman->m_feature_hitn.x;
        v12 = v14 * cman->m_feature_hitn.y;
        v11 = v14 * cman->m_feature_hitn.z;
        v8 = v13;
        v9 = v12;
        v10 = v11;
        p_x = &v17->m_list_center[j].x;
        v6 = *p_x - v13;
        v5 = v17->m_list_center[j].y - v12;
        v4 = v17->m_list_center[j].z - v11;
        v3.x = v6;
        v3.y = v5;
        v3.z = v4;
        cman->add_feature_point(&v3);
        //phys_contact_manifold::add_feature_point(cman, &v3);
    }
}

void gjk_double_sphere_t::calc_aabb(
                const phys_mat44 *xform,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max)
{
    int i; // [esp+1A8h] [ebp-4h]

    phys_aabb_init_sphere(
        xform,
        this->m_list_center,
        this->m_list_radius[0] + this->m_geom_radius,
        aabb_min,
        aabb_max);

    for ( i = 1; i < this->m_count; ++i )
        phys_aabb_add_sphere(
            xform,
            &this->m_list_center[i],
            this->m_list_radius[i] + this->m_geom_radius,
            aabb_min,
            aabb_max);
}

void    phys_aabb_init_sphere(
                const phys_mat44 *xform,
                const phys_vec3 *center,
                float radius,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max)
{
    float v6; // [esp-48h] [ebp-68h]
    float v7; // [esp-44h] [ebp-64h]
    float v8; // [esp-28h] [ebp-48h]
    float v9; // [esp-24h] [ebp-44h]
    phys_vec3 rvec; // [esp+0h] [ebp-20h] BYREF
    phys_vec3 center_abs; // [esp+10h] [ebp-10h] BYREF
    float retaddr; // [esp+20h] [ebp+0h]

    //center_abs.y = a1;
    //center_abs.z = retaddr;
    rvec = phys_full_multiply(xform, center);
    v8 = rvec.z - radius;
    v9 = rvec.w - radius;
    aabb_min->x = rvec.y - radius;
    aabb_min->y = v8;
    aabb_min->z = v9;
    v6 = rvec.z + radius;
    v7 = rvec.w + radius;
    aabb_max->x = rvec.y + radius;
    aabb_max->y = v6;
    aabb_max->z = v7;
}

void    phys_aabb_add_sphere(
                const phys_mat44 *xform,
                const phys_vec3 *center,
                float radius,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max)
{
    const phys_vec3 *v6; // eax
    const phys_vec3 *v7; // eax
    phys_vec3 v8; // [esp-6Ch] [ebp-9Ch] BYREF
    phys_vec3 v9; // [esp-5Ch] [ebp-8Ch] BYREF
    float v10; // [esp-4Ch] [ebp-7Ch]
    float v11; // [esp-48h] [ebp-78h]
    float v12; // [esp-44h] [ebp-74h]
    const phys_vec3 *v13; // [esp-40h] [ebp-70h]
    phys_vec3 v14; // [esp-3Ch] [ebp-6Ch] BYREF
    phys_vec3 v15; // [esp-2Ch] [ebp-5Ch] BYREF
    float v16; // [esp-18h] [ebp-48h]
    float v17; // [esp-14h] [ebp-44h]
    float v18; // [esp-10h] [ebp-40h]
    float v19; // [esp-Ch] [ebp-3Ch]
    float v20; // [esp-8h] [ebp-38h]
    float v21; // [esp-4h] [ebp-34h]
    phys_vec3 rvec; // [esp+0h] [ebp-30h] BYREF
    phys_vec3 center_abs; // [esp+10h] [ebp-20h] BYREF
    int v24; // [esp+24h] [ebp-Ch]
    void *v25; // [esp+28h] [ebp-8h]
    void *retaddr; // [esp+30h] [ebp+0h]

    v24 = a1;
    v25 = retaddr;
    v6 = phys_multiply(xform, center);
    operator+((phys_vec3 *)&rvec.y, v6, &xform->w);
    v19 = radius;
    v20 = radius;
    v21 = radius;
    v18 = rvec.y - radius;
    v17 = rvec.z - radius;
    v16 = rvec.w - radius;
    v15.x = rvec.y - radius;
    v15.y = rvec.z - radius;
    v15.z = rvec.w - radius;
    v13 = phys_min(&v14, aabb_min, &v15);
    aabb_min->x = v13->x;
    aabb_min->y = v13->y;
    aabb_min->z = v13->z;
    v12 = rvec.y + v19;
    v11 = rvec.z + v20;
    v10 = rvec.w + v21;
    v9.x = rvec.y + v19;
    v9.y = rvec.z + v20;
    v9.z = rvec.w + v21;
    v7 = phys_max(&v8, aabb_max, &v9);
    aabb_max->x = v7->x;
    aabb_max->y = v7->y;
    aabb_max->z = v7->z;
}

float gjk_double_sphere_t::get_geom_radius() const
{
    return this->m_geom_radius;
}

bool gjk_double_sphere_t::is_polyhedron()
{
    return this->m_count == 1 || this->m_list_radius[1] == 0.0;
}

gjk_double_sphere_t *__cdecl gjk_double_sphere_t::create(
                const phys_vec3 *c0,
                const phys_vec3 *c1,
                float r,
                int stype,
                gjk_collision_visitor *allocator)
{
    unsigned int unique_id; // eax
    float v7; // [esp-4Ch] [ebp-68h]
    float v8; // [esp-48h] [ebp-64h]
    gjk_double_sphere_t *v9; // [esp-Ch] [ebp-28h]
    gjk_double_sphere_t *v10; // [esp-8h] [ebp-24h]
    unsigned int *v11; // [esp-4h] [ebp-20h]
    gjk_double_sphere_t *v12; // [esp-4h] [ebp-20h]

    if ( !allocator
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp", 237, 0, "%s", "allocator") )
    {
        __debugbreak();
    }
    if ( allocator->is_query(allocator) )
    {
        v11 = (unsigned int *)allocator->allocate(allocator, 144, 0, 0);
        if ( v11 )
        {
            *v11 = &phys_gjk_geom::`vftable';
            *v11 = &gjk_base_t::`vftable';
            v11[15] = 0;
            *v11 = &gjk_double_sphere_t::`vftable';
            `vector constructor iterator'(
                (void *)0x60,
                0x10u,
                2,
                (void *(*)(void *))XAnimClientNotifyList::GetNotifyList);
            v10 = v12;
        }
        else
        {
            v10 = 0;
        }
        v9 = v10;
        v10->m_flags |= 1u;
    }
    else
    {
        v9 = phys_simple_allocator<gjk_double_sphere_t>::allocate(&double_sphere_pool);
        unique_id = gjk_unique_id_database_t::get_unique_id(&g_gjk_unique_id_database);
        gjk_base_t::set_geom_id_new(v9, unique_id);
    }
    if ( !v9 && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp", 237, 0, "%s", "obj") )
        __debugbreak();
    v7 = 0.5 * (float)(c0->y + c1->y);
    v8 = 0.5 * (float)(c0->z + c1->z);
    v9->m_center.x = 0.5 * (float)(c0->x + c1->x);
    v9->m_center.y = v7;
    v9->m_center.z = v8;
    v9->m_count = 2;
    v9->m_list_center[0].x = c0->x;
    v9->m_list_center[0].y = c0->y;
    v9->m_list_center[0].z = c0->z;
    v9->m_list_center[1].x = c1->x;
    v9->m_list_center[1].y = c1->y;
    v9->m_list_center[1].z = c1->z;
    v9->m_list_radius[0] = 0.0f;
    v9->m_list_radius[1] = 0.0f;
    v9->m_geom_radius = r;
    if ( stype >= 31
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp",
                    245,
                    0,
                    "%s",
                    "stype < SURF_TYPECOUNT") )
    {
        __debugbreak();
    }
    v9->stype = stype;
    return v9;
}

void __cdecl gjk_double_sphere_t::destroy(gjk_double_sphere_t *geom)
{
    if ( geom )
    {
        if ( (geom->m_flags & 1) != 0 )
        {
            if ( _tlAssert(
                         "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp",
                         254,
                         "!geom->get_flag( gjk_base_t::FLAG_TEMP_ALLOCATION )",
                         "") )
            {
                __debugbreak();
            }
        }
        phys_simple_allocator<gjk_double_sphere_t>::free(&double_sphere_pool, geom);
    }
}

gjk_cylinder_t *__cdecl gjk_cylinder_t::create(
                int _direction,
                float _halfHeight,
                float _radius,
                const phys_mat44 *_xform,
                int stype,
                gjk_collision_visitor *allocator)
{
    unsigned int unique_id; // eax
    gjk_cylinder_t *v8; // [esp+4h] [ebp-4Ch]
    gjk_cylinder_t *v9; // [esp+48h] [ebp-8h]
    gjk_cylinder_t *obj; // [esp+4Ch] [ebp-4h]

    if ( !allocator
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp", 261, 0, "%s", "allocator") )
    {
        __debugbreak();
    }
    if ( allocator->is_query(allocator) )
    {
        v9 = (gjk_cylinder_t *)allocator->allocate(allocator, 160, 16, 0);
        if ( v9 )
        {
            v9->__vftable = (gjk_cylinder_t_vtbl *)&phys_gjk_geom::`vftable';
            v9->__vftable = (gjk_cylinder_t_vtbl *)&gjk_base_t::`vftable';
            v9->m_flags = 0;
            v9->__vftable = (gjk_cylinder_t_vtbl *)&gjk_cylinder_t::`vftable';
            ind = v9;
        }
        else
        {
            v8 = 0;
        }
        obj = v8;
        v8->m_flags |= 1u;
    }
    else
    {
        obj = phys_simple_allocator<gjk_cylinder_t>::allocate(&cylinder_pool);
        unique_id = gjk_unique_id_database_t::get_unique_id(&g_gjk_unique_id_database);
        gjk_base_t::set_geom_id_new(obj, unique_id);
    }
    if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp", 261, 0, "%s", "obj") )
        __debugbreak();
    obj->direction = _direction;
    obj->halfHeight = _halfHeight;
    obj->radius = _radius;
    obj->m_geom_radius = 0.0f;
    phys_mat44::operator=(&obj->xform, _xform);
    if ( stype >= 31
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp",
                    267,
                    0,
                    "%s",
                    "stype < SURF_TYPECOUNT") )
    {
        __debugbreak();
    }
    obj->stype = stype;
    return obj;
}

void gjk_cylinder_t::support(
                gjk_cylinder_t *this@<ecx>,
                int a2@<ebp>,
                const phys_vec3 *v,
                phys_vec3 *support_vert,
                phys_vec3 *support_ind)
{
    const phys_vec3 *v5; // eax
    phys_vec3 v6; // [esp-5Ch] [ebp-8Ch] BYREF
    phys_vec3 v7; // [esp-4Ch] [ebp-7Ch] BYREF
    float v8; // [esp-3Ch] [ebp-6Ch]
    float v9; // [esp-38h] [ebp-68h]
    float v10; // [esp-34h] [ebp-64h]
    const phys_vec3 *dims; // [esp-30h] [ebp-60h]
    phys_vec3 v12; // [esp-2Ch] [ebp-5Ch] BYREF
    unsigned int v13; // [esp-18h] [ebp-48h]
    float v14; // [esp-14h] [ebp-44h]
    float v15; // [esp-10h] [ebp-40h]
    float v16; // [esp-Ch] [ebp-3Ch]
    unsigned int direction; // [esp-8h] [ebp-38h]
    float v18; // [esp-4h] [ebp-34h]
    int len; // [esp+0h] [ebp-30h]
    float x; // [esp+4h] [ebp-2Ch] BYREF
    float v_dir; // [esp+8h] [ebp-28h]
    float z; // [esp+Ch] [ebp-24h]
    gjk_cylinder_t *v23; // [esp+20h] [ebp-10h]
    unsigned int v24[2]; // [esp+24h] [ebp-Ch] BYREF
    _UNKNOWN *retaddr; // [esp+30h] [ebp+0h]

    v24[0] = a2;
    v24[1] = retaddr;
    v23 = this;
    phys_inv_multiply((const phys_vec3 *)&x, &this->xform, v);
    len = v23->direction;
    if ( (unsigned int)len > 2
        && _tlAssert(
                 "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
                 34,
                 "i >= 0 && i < 3",
                 "") )
    {
        __debugbreak();
    }
    v18 = *(&x + len);
    direction = v23->direction;
    if ( direction > 2
        && _tlAssert(
                 "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
                 34,
                 "i >= 0 && i < 3",
                 "") )
    {
        __debugbreak();
    }
    *(&x + direction) = 0.0f;
    v16 = Abs(&x);
    if ( v16 <= 0.001 )
    {
        x = PHYS_ZERO_VEC_39.x;
        v_dir = PHYS_ZERO_VEC_39.y;
        z = PHYS_ZERO_VEC_39.z;
    }
    else
    {
        v15 = 1.0 / v16;
        x = x * (float)(1.0 / v16);
        v_dir = v_dir * (float)(1.0 / v16);
        z = z * (float)(1.0 / v16);
    }
    if ( v18 < 0.0 )
        v14 = -1.0f;
    else
        v14 = 1.0f;
    v13 = v23->direction;
    if ( v13 > 2
        && _tlAssert(
                 "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
                 34,
                 "i >= 0 && i < 3",
                 "") )
    {
        __debugbreak();
    }
    *(&x + v13) = v14;
    support_ind->x = x;
    support_ind->y = v_dir;
    support_ind->z = z;
    dims = gjk_cylinder_t::get_dims(v23, (int)v24, &v12);
    v10 = x * dims->x;
    v9 = v_dir * dims->y;
    v8 = z * dims->z;
    v7.x = v10;
    v7.y = v9;
    v7.z = v8;
    v5 = phys_full_multiply((int)v24, &v6, &v23->xform, &v7);
    support_vert->x = v5->x;
    support_vert->y = v5->y;
    support_vert->z = v5->z;
}

const phys_vec3 *gjk_cylinder_t::get_dims@<eax>(
                gjk_cylinder_t *this@<ecx>,
                int a2@<ebp>,
                const phys_vec3 *result)
{
    unsigned int direction; // [esp-10h] [ebp-30h]
    float w; // [esp-Ch] [ebp-2Ch]
    float z; // [esp-8h] [ebp-28h]
    float y; // [esp-4h] [ebp-24h]
    phys_vec3 dims; // [esp+0h] [ebp-20h]
    gjk_cylinder_t *v9; // [esp+10h] [ebp-10h]
    int v10; // [esp+14h] [ebp-Ch]
    void *v11; // [esp+18h] [ebp-8h]
    void *retaddr; // [esp+20h] [ebp+0h]

    v10 = a2;
    v11 = retaddr;
    v9 = this;
    dims.w = this->radius;
    dims.z = this->radius;
    dims.y = this->radius;
    w = dims.w;
    z = dims.z;
    y = dims.y;
    direction = this->direction;
    if ( direction > 2
        && _tlAssert(
                 "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
                 34,
                 "i >= 0 && i < 3",
                 "") )
    {
        __debugbreak();
    }
    *(&w + direction) = v9->halfHeight;
    result->x = w;
    result->y = z;
    result->z = y;
    result->w = dims.x;
    return result;
}

void gjk_cylinder_t::get_simplex(
                gjk_cylinder_t *this@<ecx>,
                int a2@<ebp>,
                const cached_simplex_info *cache_info,
                int index_count,
                phys_vec3 *simplex_verts,
                phys_vec3 *simplex_inds)
{
    phys_vec3 *v6; // [esp-50h] [ebp-74h]
    const phys_vec3 *v7; // [esp-4Ch] [ebp-70h]
    phys_vec3 v8; // [esp-48h] [ebp-6Ch] BYREF
    phys_vec3 v9; // [esp-38h] [ebp-5Ch] BYREF
    float v10; // [esp-24h] [ebp-48h]
    float v11; // [esp-20h] [ebp-44h]
    float v12; // [esp-1Ch] [ebp-40h]
    float *v13; // [esp-18h] [ebp-3Ch]
    phys_vec3 *v14; // [esp-14h] [ebp-38h]
    float *p_x; // [esp-10h] [ebp-34h]
    int j; // [esp-Ch] [ebp-30h]
    float v17[2]; // [esp-8h] [ebp-2Ch] BYREF
    int i; // [esp+0h] [ebp-24h]
    gjk_cylinder_t *v19; // [esp+14h] [ebp-10h]
    unsigned int v20[2]; // [esp+18h] [ebp-Ch] BYREF
    _UNKNOWN *retaddr; // [esp+24h] [ebp+0h]

    v20[0] = a2;
    v20[1] = retaddr;
    v19 = this;
    gjk_cylinder_t::get_dims(this, (int)v20, (const phys_vec3 *)v17);
    for ( j = 0; j < index_count; ++j )
    {
        p_x = &cache_info->m_indices[j].x;
        v14 = &simplex_inds[j];
        v14->x = *p_x;
        v14->y = p_x[1];
        v14->z = p_x[2];
        v13 = &cache_info->m_indices[j].x;
        v12 = *v13 * v17[0];
        v11 = v13[1] * v17[1];
        v10 = v13[2] * *(float *)&i;
        v9.x = v12;
        v9.y = v11;
        v9.z = v10;
        obj = phys_full_multiply((int)v20, &ind, &v19->xform, &v9);
        v6 = &simplex_verts[j];
        v6->x = obj->x;
        v6->y = obj->y;
        v6->z = obj->z;
    }
}

const phys_vec3 *gjk_cylinder_t::get_center@<eax>(
                gjk_cylinder_t *this@<ecx>,
                int a2@<ebp>,
                const phys_vec3 *result)
{
    phys_vec3 v4; // [esp-20h] [ebp-2Ch] BYREF
    gjk_cylinder_t *v5; // [esp-4h] [ebp-10h]
    unsigned int v6[2]; // [esp+0h] [ebp-Ch] BYREF
    _UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]

    v6[0] = a2;
    v6[1] = retaddr;
    v5 = this;
    memset(&v4, 0, 12);
    phys_full_multiply((int)v6, result, &this->xform, &v4);
    return result;
}

void gjk_cylinder_t::get_feature(gjk_cylinder_t *this@<ecx>, int a2@<ebp>, phys_contact_manifold *cman)
{
    phys_vec3 *v3; // ecx
    const phys_vec3 *v4; // eax
    const phys_vec3 *v5; // eax
    int halfHeight_low; // xmm0_4
    phys_vec3 *v7; // eax
    const phys_vec3 *v8; // eax
    const phys_vec3 *v9; // eax
    phys_vec3 *v10; // [esp+8h] [ebp-204h]
    const phys_vec3 *p_w; // [esp+Ch] [ebp-200h]
    const phys_vec3 *v12; // [esp+Ch] [ebp-200h]
    phys_vec3 v13; // [esp+10h] [ebp-1FCh] BYREF
    _BYTE v14[12]; // [esp+20h] [ebp-1ECh] BYREF
    phys_vec3 p; // [esp+2Ch] [ebp-1E0h] BYREF
    phys_vec3 v16; // [esp+40h] [ebp-1CCh] BYREF
    phys_vec3 v17; // [esp+50h] [ebp-1BCh] BYREF
    int j; // [esp+60h] [ebp-1ACh]
    float v19; // [esp+64h] [ebp-1A8h]
    float radius; // [esp+68h] [ebp-1A4h]
    int i; // [esp+6Ch] [ebp-1A0h]
    _BYTE v22[12]; // [esp+70h] [ebp-19Ch] BYREF
    phys_vec3 p0; // [esp+7Ch] [ebp-190h] BYREF
    float v24; // [esp+94h] [ebp-178h]
    float v25; // [esp+98h] [ebp-174h]
    float v26; // [esp+9Ch] [ebp-170h]
    phys_vec3 v27; // [esp+A0h] [ebp-16Ch] BYREF
    float v28; // [esp+B4h] [ebp-158h]
    float v29; // [esp+B8h] [ebp-154h]
    float v30; // [esp+BCh] [ebp-150h]
    float v31; // [esp+C0h] [ebp-14Ch] BYREF
    float v32; // [esp+C4h] [ebp-148h]
    float v33; // [esp+C8h] [ebp-144h]
    phys_vec3 *v34; // [esp+DCh] [ebp-130h]
    phys_vec3 v35; // [esp+E0h] [ebp-12Ch] BYREF
    float v36; // [esp+FCh] [ebp-110h]
    phys_vec3 v37; // [esp+100h] [ebp-10Ch] BYREF
    phys_vec3 v38; // [esp+110h] [ebp-FCh] BYREF
    phys_mat44 *v39; // [esp+128h] [ebp-E4h]
    unsigned int v40; // [esp+12Ch] [ebp-E0h]
    phys_vec3 v41; // [esp+130h] [ebp-DCh] BYREF
    phys_vec3 v42; // [esp+140h] [ebp-CCh] BYREF
    phys_mat44 *p_xform; // [esp+158h] [ebp-B4h]
    unsigned int direction; // [esp+15Ch] [ebp-B0h]
    float v45[3]; // [esp+160h] [ebp-ACh] BYREF
    phys_vec3 res; // [esp+16Ch] [ebp-A0h]
    int v47; // [esp+17Ch] [ebp-90h]
    float v48; // [esp+180h] [ebp-8Ch]
    float v49; // [esp+184h] [ebp-88h]
    float v50; // [esp+188h] [ebp-84h]
    float sr; // [esp+18Ch] [ebp-80h]
    float len; // [esp+190h] [ebp-7Ch] BYREF
    float d; // [esp+194h] [ebp-78h]
    float w; // [esp+198h] [ebp-74h]
    phys_vec3 r1; // [esp+19Ch] [ebp-70h] BYREF
    const phys_vec3 *v56; // [esp+1BCh] [ebp-50h]
    phys_vec3 v57; // [esp+1C0h] [ebp-4Ch] BYREF
    phys_vec3 *p_z; // [esp+1D8h] [ebp-34h]
    phys_vec3 *p_y; // [esp+1DCh] [ebp-30h]
    float x; // [esp+1E0h] [ebp-2Ch] BYREF
    float y; // [esp+1E4h] [ebp-28h]
    float z; // [esp+1E8h] [ebp-24h]
    gjk_cylinder_t *v63; // [esp+1FCh] [ebp-10h]
    unsigned int v64[2]; // [esp+200h] [ebp-Ch] BYREF
    _UNKNOWN *retaddr; // [esp+20Ch] [ebp+0h]

    v64[0] = a2;
    v64[1] = retaddr;
    v63 = this;
    if ( this->m_geom_radius != 0.0
        && !Assert_MyHandler(
                    "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
                    584,
                    0,
                    "%s",
                    "m_geom_radius == 0.0f") )
    {
        __debugbreak();
    }
    if ( v63->direction )
    {
        if ( v63->direction == 1 )
        {
            p_y = &PHYS_IDENTITY_MATRIX_39.y;
            x = PHYS_IDENTITY_MATRIX_39.y.x;
            y = PHYS_IDENTITY_MATRIX_39.y.y;
            v3 = &PHYS_IDENTITY_MATRIX_39.y;
        }
        else
        {
            p_z = &PHYS_IDENTITY_MATRIX_39.z;
            x = PHYS_IDENTITY_MATRIX_39.z.x;
            y = PHYS_IDENTITY_MATRIX_39.z.y;
            v3 = &PHYS_IDENTITY_MATRIX_39.z;
        }
        z = v3->z;
    }
    else
    {
        x = PHYS_IDENTITY_MATRIX_39.x.x;
        y = PHYS_IDENTITY_MATRIX_39.x.y;
        z = PHYS_IDENTITY_MATRIX_39.x.z;
    }
    v56 = phys_inv_multiply(&v57, &v63->xform, &cman->m_feature_hitn);
    r1.y = v56->x;
    r1.z = v56->y;
    r1.w = v56->z;
    len = r1.y;
    d = r1.z;
    w = r1.w;
    sr = *(float *)&v63->direction;
    if ( LODWORD(sr) > 2
        && _tlAssert(
                 "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
                 34,
                 "i >= 0 && i < 3",
                 "") )
    {
        __debugbreak();
    }
    *(&len + LODWORD(sr)) = 0.0f;
    v50 = (float)((float)(x * r1.y) + (float)(y * r1.z)) + (float)(z * r1.w);
    if ( fabs(v50) >= 0.70709997 )
    {
        if ( fabs(v50) >= 0.99000001 )
        {
            v30 = *phys_vec3::operator[]<int>((phys_vec3 *)&x, 2u);
            v29 = *phys_vec3::operator[]<int>((phys_vec3 *)&x, 0);
            v28 = *phys_vec3::operator[]<int>((phys_vec3 *)&x, 1u);
            v27.x = v30;
            v27.y = v29;
            v27.z = v28;
            phys_vec3::operator=((phys_vec3 *)&len, &v27);
            v26 = *phys_vec3::operator[]<int>((phys_vec3 *)&x, 1u);
            v25 = *phys_vec3::operator[]<int>((phys_vec3 *)&x, 2u);
            v24 = *phys_vec3::operator[]<int>((phys_vec3 *)&x, 0);
            p0.y = v26;
            p0.z = v25;
            p0.w = v24;
            phys_vec3::operator=((phys_vec3 *)&v31, (phys_vec3 *)&p0.y);
        }
        else
        {
            v36 = 1.0 / Abs(&len);
            len = len * v36;
            d = d * v36;
            w = w * v36;
            v34 = phys_cross(&v35, (const phys_vec3 *)&len, (const phys_vec3 *)&x);
            v31 = v34->x;
            v32 = v34->y;
            v33 = v34->z;
        }
        memset(v22, 0, sizeof(v22));
        if ( *phys_vec3::operator[]<int>((phys_vec3 *)&r1.y, v63->direction) <= 0.0 )
            halfHeight_low = LODWORD(v63->halfHeight);
        else
            halfHeight_low = LODWORD(v63->halfHeight) ^ _mask__NegFloat_;
        i = halfHeight_low;
        *(unsigned int *)phys_vec3::operator[]<int>((phys_vec3 *)v22, v63->direction) = halfHeight_low;
        radius = v63->radius;
        len = len * radius;
        d = d * radius;
        w = w * radius;
        v19 = v63->radius;
        v31 = v31 * v19;
        v32 = v32 * v19;
        v33 = v33 * v19;
        for ( j = 0; j < 4; ++j )
        {
            v10 = operator*(&v17, `gjk_cylinder_t::get_feature'::`18'::s[j], (const phys_vec3 *)&v31);
            obj = operator*(&v16, `gjk_cylinder_t::get_feature'::`18'::c[j], (const phys_vec3 *)&len);
            ind = operator+((phys_vec3 *)&p.y, obj, v10);
            operator+((phys_vec3 *)v14, ind, (const phys_vec3 *)v22);
            v9 = phys_full_multiply((int)v64, &v13, &v63->xform, (const phys_vec3 *)v14);
            phys_contact_manifold::add_feature_point(cman, v9);
        }
    }
    else
    {
        v49 = Abs(&len);
        if ( v49 <= 0.0000099999997
            && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h", 604, 0, "%s", "len > 0.00001f") )
        {
            __debugbreak();
        }
        v48 = v63->radius / v49;
        v47 = LODWORD(v48) ^ _mask__NegFloat_;
        res.w = COERCE_FLOAT(LODWORD(v48) ^ _mask__NegFloat_) * len;
        res.z = COERCE_FLOAT(LODWORD(v48) ^ _mask__NegFloat_) * d;
        res.y = COERCE_FLOAT(LODWORD(v48) ^ _mask__NegFloat_) * w;
        v45[0] = res.w;
        v45[1] = res.z;
        v45[2] = res.y;
        direction = v63->direction;
        if ( direction > 2
            && _tlAssert(
                     "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
                     34,
                     "i >= 0 && i < 3",
                     "") )
        {
            __debugbreak();
        }
        v45[direction] = v63->halfHeight;
        p_xform = &v63->xform;
        p_w = &v63->xform.w;
        v4 = phys_multiply(&v42, &v63->xform, (const phys_vec3 *)v45);
        operator+(&v41, v4, p_w);
        phys_contact_manifold::add_feature_point(cman, &v41);
        v40 = v63->direction;
        if ( v40 > 2 )
        {
            if ( _tlAssert(
                         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
                         34,
                         "i >= 0 && i < 3",
                         "") )
            {
                __debugbreak();
            }
        }
        v45[v40] = -v63->halfHeight;
        v39 = &v63->xform;
        v12 = &v63->xform.w;
        v5 = phys_multiply(&v38, &v63->xform, (const phys_vec3 *)v45);
        operator+(&v37, v5, v12);
        phys_contact_manifold::add_feature_point(cman, &v37);
    }
}

void gjk_cylinder_t::calc_aabb(
                gjk_cylinder_t *this@<ecx>,
                int a2@<ebp>,
                const phys_mat44 *xform_,
                phys_vec3 *aabb_min,
                phys_vec3 *aabb_max)
{
    const phys_vec3 *v5; // eax
    const phys_vec3 *v6; // eax
    float radius; // [esp+0h] [ebp-78h]
    float radiusa; // [esp+0h] [ebp-78h]
    phys_vec3 v9; // [esp+Ch] [ebp-6Ch] BYREF
    phys_vec3 v10; // [esp+1Ch] [ebp-5Ch] BYREF
    float *v11; // [esp+2Ch] [ebp-4Ch]
    unsigned int v12; // [esp+30h] [ebp-48h]
    float *v13; // [esp+34h] [ebp-44h]
    unsigned int direction; // [esp+38h] [ebp-40h]
    unsigned int v15[3]; // [esp+3Ch] [ebp-3Ch] BYREF
    phys_vec3 p1; // [esp+48h] [ebp-30h] BYREF
    gjk_cylinder_t *v17; // [esp+68h] [ebp-10h]
    unsigned int v18[2]; // [esp+6Ch] [ebp-Ch] BYREF
    _UNKNOWN *retaddr; // [esp+78h] [ebp+0h]

    v18[0] = a2;
    v18[1] = retaddr;
    v17 = this;
    memset(&p1.y, 0, 12);
    memset(v15, 0, sizeof(v15));
    direction = this->direction;
    if ( direction > 2
        && _tlAssert(
                 "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
                 34,
                 "i >= 0 && i < 3",
                 "") )
    {
        __debugbreak();
    }
    v13 = &p1.y + direction;
    *v13 = *v13 + v17->halfHeight;
    v12 = v17->direction;
    if ( v12 > 2
        && _tlAssert(
                 "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
                 34,
                 "i >= 0 && i < 3",
                 "") )
    {
        __debugbreak();
    }
    v11 = (float *)&v15[v12];
    *v11 = *v11 - v17->halfHeight;
    radius = v17->radius + v17->m_geom_radius;
    v5 = phys_full_multiply((int)v18, &v10, &v17->xform, (phys_vec3 *)&p1.y);
    phys_aabb_init_sphere(COERCE_FLOAT(v18), xform_, v5, radius, aabb_min, aabb_max);
    radiusa = v17->radius + v17->m_geom_radius;
    v6 = phys_full_multiply((int)v18, &v9, &v17->xform, (const phys_vec3 *)v15);
    phys_aabb_add_sphere((int)v18, xform_, v6, radiusa, aabb_min, aabb_max);
}

unsigned int gjk_cylinder_t::get_type(gjk_cylinder_t *this)
{
    return 5;
}

float gjk_cylinder_t::get_geom_radius(gjk_cylinder_t *this)
{
    return this->m_geom_radius;
}

void __cdecl gjk_cylinder_t::destroy(gjk_cylinder_t *geom)
{
    if ( geom )
    {
        if ( (geom->m_flags & 1) != 0 )
        {
            if ( _tlAssert(
                         "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp",
                         276,
                         "!geom->get_flag(gjk_base_t::FLAG_TEMP_ALLOCATION)",
                         "") )
            {
                __debugbreak();
            }
        }
        phys_simple_allocator<gjk_cylinder_t>::free(&cylinder_pool, geom);
    }
}

void    setup_gjk_polygon_cylinder(
                float *mins,
                float *maxs,
                float radius_adjust,
                gjk_polygon_cylinder_t *gjk_cylinder)
{
    const phys_vec3 *v5; // edx
    float v6; // xmm0_4
    float v7; // xmm0_4
    long double v8; // [esp-60h] [ebp-15Ch]
    float v9; // [esp-54h] [ebp-150h]
    float m_half_height; // [esp-4Ch] [ebp-148h]
    float v11; // [esp-38h] [ebp-134h]
    float v12; // [esp-Ch] [ebp-108h]
    float v13; // [esp+4h] [ebp-F8h]
    float v14; // [esp+10h] [ebp-ECh]
    float v15; // [esp+14h] [ebp-E8h]
    float v16; // [esp+18h] [ebp-E4h]
    phys_vec3 v17; // [esp+30h] [ebp-CCh] BYREF
    phys_vec3 v18; // [esp+40h] [ebp-BCh] BYREF
    float v19; // [esp+50h] [ebp-ACh]
    float v20; // [esp+54h] [ebp-A8h]
    float v21; // [esp+58h] [ebp-A4h]
    const phys_vec3 *v22; // [esp+5Ch] [ebp-A0h]
    phys_vec3 v23; // [esp+60h] [ebp-9Ch] BYREF
    phys_vec3 v24; // [esp+70h] [ebp-8Ch] BYREF
    float v25; // [esp+84h] [ebp-78h]
    float v26; // [esp+88h] [ebp-74h]
    float v27; // [esp+8Ch] [ebp-70h]
    float v28; // [esp+90h] [ebp-6Ch] BYREF
    float v29; // [esp+94h] [ebp-68h]
    float v30; // [esp+98h] [ebp-64h]
    phys_vec3 save_pv_maxs; // [esp+9Ch] [ebp-60h] BYREF
    phys_vec3 save_pv_mins; // [esp+ACh] [ebp-50h]
    float v33; // [esp+CCh] [ebp-30h]
    float x; // [esp+D0h] [ebp-2Ch] BYREF
    float y; // [esp+D4h] [ebp-28h]
    float dim_adjust; // [esp+D8h] [ebp-24h]
    phys_vec3 pv_maxs; // [esp+DCh] [ebp-20h] BYREF
    phys_vec3 pv_mins; // [esp+ECh] [ebp-10h]
    float retaddr; // [esp+FCh] [ebp+0h]

    pv_mins.y = a1;
    pv_mins.z = retaddr;
    Phys_Vec3ToNitrousVec(mins, (phys_vec3 *)&pv_maxs.y);
    Phys_Vec3ToNitrousVec(maxs, (phys_vec3 *)&x);
    if ( radius_adjust < 0.125
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp",
                    298,
                    0,
                    "%s",
                    "radius_adjust >= SURFACE_CLIP_EPSILON") )
    {
        __debugbreak();
    }
    v33 = radius_adjust - 0.125;
    save_pv_mins.y = radius_adjust - 0.125;
    save_pv_mins.z = radius_adjust - 0.125;
    save_pv_mins.w = radius_adjust - 0.125;
    save_pv_maxs = pv_maxs;
    save_pv_mins.x = pv_mins.x;
    v28 = x;
    v29 = y;
    v30 = dim_adjust;
    v27 = pv_maxs.y + (float)(radius_adjust - 0.125);
    v26 = pv_maxs.z + (float)(radius_adjust - 0.125);
    v25 = pv_maxs.w + (float)(radius_adjust - 0.125);
    v24.x = v27;
    v24.y = v26;
    v24.z = v25;
    v22 = phys_min(&v23, &v24, (const phys_vec3 *)&v28);
    pv_maxs.y = v22->x;
    pv_maxs.z = v22->y;
    pv_maxs.w = v22->z;
    v21 = v28 - save_pv_mins.y;
    v20 = v29 - save_pv_mins.z;
    v19 = v30 - save_pv_mins.w;
    v18.x = v28 - save_pv_mins.y;
    v18.y = v29 - save_pv_mins.z;
    v18.z = v30 - save_pv_mins.w;
    v5 = phys_max(&v17, &v18, (phys_vec3 *)&save_pv_maxs.y);
    x = v5->x;
    y = v5->y;
    dim_adjust = v5->z;
    v14 = x - pv_maxs.y;
    v15 = y - pv_maxs.z;
    v16 = dim_adjust - pv_maxs.w;
    if ( (float)(x - pv_maxs.y) < 0.0
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp",
                    306,
                    0,
                    "%s",
                    "dim.GetX() >= 0.0f") )
    {
        __debugbreak();
    }
    if ( v15 < 0.0
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp",
                    307,
                    0,
                    "%s",
                    "dim.GetY() >= 0.0f") )
    {
        __debugbreak();
    }
    if ( v16 < 0.0
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp",
                    308,
                    0,
                    "%s",
                    "dim.GetZ() >= 0.0f") )
    {
        __debugbreak();
    }
    if ( v14 <= v16 )
        v13 = v14;
    else
        v13 = v16;
    if ( (float)(0.5 * v13) >= 1.0 )
        v6 = 0.5 * v13;
    else
        v6 = 1.0f;
    v12 = v6;
    if ( v6 == 1.0 )
        tlWarning("degenerate capsule.");
    if ( v6 < 0.0
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp", 312, 0, "%s", "radius >= 0.0f") )
    {
        __debugbreak();
    }
    if ( (float)(0.5 * v16) >= 1.0 )
        obj = 0.5 * v16;
    else
        obj = 1.0f;
    gjk_cylinder->m_half_height = obj;
    if ( gjk_cylinder->m_half_height == 1.0 )
        tlWarning("degenerate capsule1.");
    gjk_cylinder->m_capsule_radius = v12;
    if ( tad )
    {
        if ( (_S2_7 & 1) == 0 )
        {
            _S2_7 |= 1u;
            __libm_sse2_cos(ind);
            radius_scale_factor = 1.0 / (float)((float)(6.2831855 / 12.0) / 2.0);
        }
        gjk_cylinder->m_polygon_cylinder_radius = v12 * radius_scale_factor;
    }
    else
    {
        gjk_cylinder->m_polygon_cylinder_radius = v12;
    }
    gjk_cylinder->m_geom_radius = radius_adjust;
    v11 = (float)((float)(pv_maxs.w + dim_adjust) * 0.5) + phys_player_collision_adjust_height->current.value;
    gjk_cylinder->m_center.x = 0.0f;
    gjk_cylinder->m_center.y = 0.0f;
    gjk_cylinder->m_center.z = v11;
    if ( gjk_cylinder->m_half_height <= head_offset )
        m_half_height = gjk_cylinder->m_half_height;
    else
        m_half_height = head_offset;
    gjk_cylinder->m_head_offset = m_half_height;
    if ( gjk_cylinder->m_half_height <= foot_offset )
        v9 = gjk_cylinder->m_half_height;
    else
        v9 = foot_offset;
    gjk_cylinder->m_foot_offset = v9;
    gjk_cylinder->m_mode = 0;
}

gjk_polygon_cylinder_t *__cdecl gjk_polygon_cylinder_t::create(
                float (*mins)[3],
                float (*maxs)[3],
                float radius_adjust,
                int stype,
                gjk_collision_visitor *allocator)
{
    unsigned int unique_id; // eax
    gjk_polygon_cylinder_t *v7; // [esp+Ch] [ebp-34h]
    gjk_polygon_cylinder_t *v8; // [esp+38h] [ebp-8h]
    gjk_polygon_cylinder_t *obj; // [esp+3Ch] [ebp-4h]
    int savedregs; // [esp+40h] [ebp+0h] BYREF

    if ( !allocator
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp", 345, 0, "%s", "allocator") )
    {
        __debugbreak();
    }
    if ( allocator->is_query(allocator) )
    {
        ind = (gjk_polygon_cylinder_t *)allocator->allocate(allocator, 128, 16, 0);
        if ( ind )
        {
            ind->__vftable = (gjk_polygon_cylinder_t_vtbl *)&phys_gjk_geom::`vftable';
            ind->__vftable = (gjk_polygon_cylinder_t_vtbl *)&gjk_base_t::`vftable';
            ind->m_flags = 0;
            ind->__vftable = (gjk_polygon_cylinder_t_vtbl *)&gjk_polygon_cylinder_t::`vftable';
            obj = ind;
        }
        else
        {
            obj = 0;
        }
        obj = obj;
        v7->m_flags |= 1u;
    }
    else
    {
        obj = phys_simple_allocator<gjk_polygon_cylinder_t>::allocate(&polygon_cylinder_pool);
        unique_id = gjk_unique_id_database_t::get_unique_id(&g_gjk_unique_id_database);
        gjk_base_t::set_geom_id_new(obj, unique_id);
    }
    if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp", 345, 0, "%s", "obj") )
        __debugbreak();
    setup_gjk_polygon_cylinder(COERCE_FLOAT(&savedregs), (float *)mins, (float *)maxs, radius_adjust, obj);
    obj->stype = stype;
    return obj;
}

void __cdecl gjk_polygon_cylinder_t::destroy(gjk_polygon_cylinder_t *geom)
{
    if ( geom )
    {
        if ( (geom->m_flags & 1) != 0 )
        {
            if ( _tlAssert(
                         "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp",
                         359,
                         "!geom->get_flag(gjk_base_t::FLAG_TEMP_ALLOCATION)",
                         "") )
            {
                __debugbreak();
            }
        }
        phys_simple_allocator<gjk_polygon_cylinder_t>::free(&polygon_cylinder_pool, geom);
    }
}

void __cdecl destroy_gjk_geom(gjk_aabb_t *geom)
{
    switch ( geom->get_type(geom) )
    {
        case 1u:
            gjk_aabb_t::destroy(geom);
            break;
        case 2u:
            gjk_brush_t::destroy((gjk_brush_t *)geom);
            break;
        case 3u:
            gjk_partition_t::destroy((gjk_partition_t *)geom);
            break;
        case 4u:
            gjk_double_sphere_t::destroy((gjk_double_sphere_t *)geom);
            break;
        case 5u:
            gjk_cylinder_t::destroy((gjk_cylinder_t *)geom);
            break;
        case 6u:
            gjk_obb_t::destroy((gjk_cylinder_t *)geom);
            break;
        case 7u:
            gjk_polygon_cylinder_t::destroy((gjk_polygon_cylinder_t *)geom);
            break;
        default:
            if ( _tlAssert(
                         "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp",
                         403,
                         "0",
                         "Trying to free up unkownn geom. Leak!!!") )
            {
                __debugbreak();
            }
            break;
    }
}

gjk_aabb_t * create_aabb_gjk_geom@<eax>(
                float a1@<ebp>,
                const float *mn,
                const float *mx,
                int stype,
                gjk_collision_visitor *allocator)
{
    _BYTE v6[12]; // [esp-Ch] [ebp-4Ch] BYREF
    phys_vec3 center_; // [esp+0h] [ebp-40h] BYREF
    phys_vec3 dims_; // [esp+10h] [ebp-30h] BYREF
    float v9; // [esp+20h] [ebp-20h]
    float v10; // [esp+24h] [ebp-1Ch]
    float center[3]; // [esp+28h] [ebp-18h] BYREF
    float dims[3]; // [esp+34h] [ebp-Ch]
    float retaddr; // [esp+40h] [ebp+0h]

    dims[0] = a1;
    dims[1] = retaddr;
    center[0] = (float)(0.5 * *mx) + (float)(-0.5 * *mn);
    center[1] = (float)(0.5 * mx[1]) + (float)(-0.5 * mn[1]);
    center[2] = (float)(0.5 * mx[2]) + (float)(-0.5 * mn[2]);
    dims_.w = (float)(0.5 * *mx) + (float)(0.5 * *mn);
    v9 = (float)(0.5 * mx[1]) + (float)(0.5 * mn[1]);
    v10 = (float)(0.5 * mx[2]) + (float)(0.5 * mn[2]);
    Phys_Vec3ToNitrousVec(center, (phys_vec3 *)&center_.y);
    Phys_Vec3ToNitrousVec(&dims_.w, (phys_vec3 *)v6);
    return gjk_aabb_t::create((const phys_vec3 *)v6, (phys_vec3 *)&center_.y, stype, allocator);
}

gjk_obb_t * create_obb_gjk_geom@<eax>(
                float a1@<ebp>,
                float (*orientation)[3],
                float *offset,
                float *halfLengths,
                int stype,
                gjk_collision_visitor *allocator)
{
    _BYTE obj[12]; // [esp-Ch] [ebp-5Ch] BYREF
    phys_vec3 dims; // [esp+0h] [ebp-50h] BYREF
    phys_mat44 xform; // [esp+10h] [ebp-40h] BYREF
    float retaddr; // [esp+50h] [ebp+0h]

    xform.w.y = a1;
    xform.w.z = retaddr;
    Phys_Vec3ToNitrousVec(offset, (phys_vec3 *)&xform.z.y);
    Phys_AxisToNitrousMat(orientation, (phys_mat44 *)&dims.y);
    Phys_Vec3ToNitrousVec(halfLengths, (phys_vec3 *)v7);
    return gjk_obb_t::create((const phys_mat44 *)&dims.y, (const phys_vec3 *)obj, stype, allocator);
}

gjk_brush_t *__cdecl create_brush_gjk_geom(const cbrush_t *brush, int stype, gjk_collision_visitor *allocator)
{
    int savedregs; // [esp+4h] [ebp+0h] BYREF

    return gjk_brush_t::create(COERCE_FLOAT(&savedregs), brush, stype, allocator);
}

gjk_double_sphere_t * create_capsule_gjk_geom@<eax>(
                float a1@<ebp>,
                float *center,
                float radius,
                float halfHeight,
                unsigned int direction,
                int stype,
                gjk_collision_visitor *allocator)
{
    unsigned int ind[3]; // [esp+1Ch] [ebp-3Ch] BYREF
    phys_vec3 c1; // [esp+28h] [ebp-30h] BYREF
    phys_vec3 c0; // [esp+38h] [ebp-20h] BYREF
    phys_vec3 _center; // [esp+48h] [ebp-10h]
    float retaddr; // [esp+58h] [ebp+0h]

    _center.y = a1;
    _center.z = retaddr;
    Phys_Vec3ToNitrousVec(center, (phys_vec3 *)&c0.y);
    c1.y = c0.y;
    c1.z = c0.z;
    c1.w = c0.w;
    c0.x = _center.x;
    v8[0] = LODWORD(c0.y);
    v8[1] = LODWORD(c0.z);
    v8[2] = LODWORD(c0.w);
    c1.x = _center.x;
    if ( direction > 2
        && _tlAssert(
                 "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
                 34,
                 "i >= 0 && i < 3",
                 "") )
    {
        __debugbreak();
    }
    *(&c1.y + direction) = *(&c1.y + direction) - halfHeight;
    if ( direction > 2
        && _tlAssert(
                 "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\old_phys_math.h",
                 34,
                 "i >= 0 && i < 3",
                 "") )
    {
        __debugbreak();
    }
    *(float *)&ind[direction] = *(float *)&ind[direction] + halfHeight;
    return gjk_double_sphere_t::create((phys_vec3 *)&c1.y, (const phys_vec3 *)ind, radius, stype, allocator);
}

gjk_cylinder_t * create_cylinder_gjk_geom@<eax>(
                float a1@<ebp>,
                float (*rot)[3],
                float *trans,
                float radius,
                float halfHeight,
                int stype,
                gjk_collision_visitor *allocator)
{
    _BYTE ind[12]; // [esp+24h] [ebp-5Ch] BYREF
    phys_mat44 xform; // [esp+30h] [ebp-50h] BYREF
    phys_vec3 _trans; // [esp+70h] [ebp-10h]
    float retaddr; // [esp+80h] [ebp+0h]

    _trans.y = a1;
    _trans.z = retaddr;
    Phys_Vec3ToNitrousVec(trans, (phys_vec3 *)&xform.w.y);
    Phys_AxisToNitrousMat(rot, (phys_mat44 *)ind);
    xform.z.y = xform.w.y;
    xform.z.z = xform.w.z;
    xform.z.w = xform.w.w;
    return gjk_cylinder_t::create(0, halfHeight, radius, (const phys_mat44 *)ind, stype, allocator);
}

void __cdecl create_brush_model_gjk_geom_r(
                cLeafBrushNode_s *node,
                int *index_base,
                gjk_collision_visitor *allocator,
                int contents_mask)
{
    gjk_brush_t *gjk_geom; // [esp+0h] [ebp-18h]
    cbrush_t *brush; // [esp+Ch] [ebp-Ch]
    int i; // [esp+14h] [ebp-4h]
    int savedregs; // [esp+18h] [ebp+0h] BYREF

    if ( node->leafBrushCount )
    {
        if ( node->leafBrushCount > 0 )
        {
            for ( i = 0; i < node->leafBrushCount; ++i )
            {
                brush = &cm.brushes[node->data.leaf.brushes[i]];
                if ( (contents_mask & brush->contents) != 0
                    && allocator->query_create_prolog_1(allocator, (const float *)brush, brush->maxs, brush) )
                {
                    gjk_geom = gjk_brush_t::create(
                                             COERCE_FLOAT(&savedregs),
                                             brush,
                                             (unsigned __int8)((int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20]
                                                                                         & brush->axial_sflags[0][0]) >> 20),
                                             allocator);
                    allocator->query_create_epilog_1(allocator, gjk_geom);
                }
            }
            *index_base += node->leafBrushCount;
            return;
        }
        create_brush_model_gjk_geom_r(node + 1, index_base, allocator, contents_mask);
    }
    if ( node->data.children.childOffset[0] || node->data.children.childOffset[1] )
    {
        create_brush_model_gjk_geom_r(&node[node->data.children.childOffset[0]], index_base, allocator, contents_mask);
        create_brush_model_gjk_geom_r(&node[node->data.children.childOffset[1]], index_base, allocator, contents_mask);
    }
}

void __cdecl query_brush_model_gjk_geom(
                unsigned __int16 brushModel,
                int contents_mask,
                gjk_collision_visitor *allocator)
{
    query_brush_model_gjk_geom_visitor query_visitor; // [esp+18h] [ebp-80h] BYREF
    cmodel_t *model; // [esp+94h] [ebp-4h]

    if ( (brushModel == 0x3FFF || brushModel == 0x4000)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp",
                    516,
                    0,
                    "%s",
                    "( brushModel != CAPSULE_MODEL_HANDLE ) && ( brushModel != TEMP_BRUSH_MODEL_HANDLE )") )
    {
        __debugbreak();
    }
    if ( !allocator
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp", 517, 0, "%s", "allocator") )
    {
        __debugbreak();
    }
    colgeom_visitor_t::colgeom_visitor_t(&query_visitor);
    query_visitor.__vftable = (query_brush_model_gjk_geom_visitor_vtbl *)&query_brush_model_gjk_geom_visitor::`vftable';
    allocator->get_local_query_aabb(allocator, (float *)&query_visitor.m_mn, (float *)&query_visitor.m_mx);
    query_visitor.m_mask = contents_mask;
    query_visitor.m_allocator = allocator;
    model = CM_ClipHandleToModel(brushModel);
    colgeom_visitor_t::intersect_box_brushes(&query_visitor, &model->leaf);
}

void query_brush_model_gjk_geom_visitor::visit(
                query_brush_model_gjk_geom_visitor *this,
                const cbrush_t *brush)
{
    gjk_brush_t *gjk_geom; // [esp+4h] [ebp-Ch]
    int savedregs; // [esp+10h] [ebp+0h] BYREF

    if ( !this->m_allocator
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp", 497, 0, "%s", "m_allocator") )
    {
        __debugbreak();
    }
    if ( this->m_allocator->query_create_prolog_1(this->m_allocator, (const float *)brush, brush->maxs, brush) )
    {
        gjk_geom = gjk_brush_t::create(
                                 COERCE_FLOAT(&savedregs),
                                 brush,
                                 (unsigned __int8)((int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20]
                                                                             & brush->axial_sflags[0][0]) >> 20),
                                 this->m_allocator);
        this->m_allocator->query_create_epilog_1(this->m_allocator, gjk_geom);
    }
}

void query_brush_model_gjk_geom_visitor::update(
                query_brush_model_gjk_geom_visitor *this,
                const float *_mn,
                const float *_mx,
                int _mask,
                const float *expand_vec)
{
    ;
}

void __cdecl create_brush_model_gjk_geom(
                unsigned __int16 brushModel,
                int contents_mask,
                gjk_collision_visitor *allocator)
{
    cLeafBrushNode_s *node; // [esp+0h] [ebp-Ch]
    int index_base; // [esp+8h] [ebp-4h] BYREF

    if ( !allocator
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp", 532, 0, "%s", "allocator") )
    {
        __debugbreak();
    }
    if ( allocator->is_query(allocator) )
    {
        query_brush_model_gjk_geom(brushModel, contents_mask, allocator);
    }
    else
    {
        if ( (brushModel == 0x3FFF || brushModel == 0x4000)
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp",
                        539,
                        0,
                        "%s",
                        "( brushModel != CAPSULE_MODEL_HANDLE ) && ( brushModel != TEMP_BRUSH_MODEL_HANDLE )") )
        {
            __debugbreak();
        }
        node = &cm.leafbrushNodes[CM_ClipHandleToModel(brushModel)->leaf.leafBrushNode];
        index_base = 0;
        create_brush_model_gjk_geom_r(node, &index_base, allocator, contents_mask);
    }
}

void __cdecl create_xmodel_gjk_geom(
                const XModel *model,
                gjk_collision_visitor *allocator,
                int bone_index,
                unsigned int brush_mask,
                bool b_fallback_to_bounding_box,
                int contents_for_proxy_collision,
                phys_mat44 *worldMat)
{
    float obj; // [esp+1Ch] [ebp-E0h]
    int surfFlags; // [esp+20h] [ebp-DCh]
    gjk_aabb_t *v9; // [esp+68h] [ebp-94h]
    gjk_cylinder_t *cylinder_gjk_geom; // [esp+6Ch] [ebp-90h]
    float hheight; // [esp+70h] [ebp-8Ch]
    float v12[3]; // [esp+74h] [ebp-88h] BYREF
    float rvec[3]; // [esp+80h] [ebp-7Ch]
    float radius; // [esp+8Ch] [ebp-70h]
    float v15[3]; // [esp+90h] [ebp-6Ch] BYREF
    float bs_radius; // [esp+9Ch] [ebp-60h]
    gjk_base_t *aabb_gjk_geom; // [esp+A0h] [ebp-5Ch]
    gjk_base_t *obb_gjk_geom; // [esp+A4h] [ebp-58h]
    int j; // [esp+A8h] [ebp-54h]
    float trasformed_v[3]; // [esp+ACh] [ebp-50h] BYREF
    float v[3]; // [esp+B8h] [ebp-44h] BYREF
    int i; // [esp+C4h] [ebp-38h]
    gjk_base_t *gjk_geom; // [esp+C8h] [ebp-34h]
    const cbrush_t *brush; // [esp+CCh] [ebp-30h]
    float mins[3]; // [esp+D0h] [ebp-2Ch] BYREF
    PhysGeomInfo *geom; // [esp+DCh] [ebp-20h]
    int stype; // [esp+E0h] [ebp-1Ch]
    float maxs[3]; // [esp+E4h] [ebp-18h] BYREF
    bool b_geom_eligible; // [esp+F3h] [ebp-9h]
    PhysGeomList *geomList; // [esp+F4h] [ebp-8h]
    unsigned int geomIndex; // [esp+F8h] [ebp-4h]
    int savedregs; // [esp+FCh] [ebp+0h] BYREF

    if ( !allocator
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp", 562, 0, "%s", "allocator") )
    {
        __debugbreak();
    }
    if ( (bone_index < -1 || bone_index >= model->numBones)
        && _tlAssert(
                 "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp",
                 569,
                 "bone_index >= -1 && bone_index < model->numBones",
                 "Bad param to create_gjk_geom xmodel version.") )
    {
        __debugbreak();
    }
    if ( model->collSurfs )
        surfFlags = model->collSurfs->surfFlags;
    else
        surfFlags = (int)"t";
    stype = (unsigned __int8)((int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20] & surfFlags) >> 20);
    geomList = xmodel_get_geomlist(model, bone_index);
    b_geom_eligible = 0;
    if ( geomList )
    {
        geomIndex = 0;
        geom = geomList->geoms;
        while ( geomIndex < geomList->count )
        {
            if ( geom->brush )
            {
                if ( (brush_mask & geom->brush->contents) != 0 )
                {
                    b_geom_eligible = 1;
                    brush = (const cbrush_t *)geom->brush;
                    if ( allocator->query_create_prolog_1(allocator, (const float *)brush, brush->maxs, geom) )
                    {
                        gjk_geom = create_brush_gjk_geom(brush, stype, allocator);
                        if ( worldMat )
                            gjk_base_t::set_xform(gjk_geom, worldMat);
                        allocator->query_create_epilog_1(allocator, gjk_geom);
                    }
                }
            }
            else
            {
                b_geom_eligible = 1;
                if ( geom->type == 1 )
                {
                    if ( (float)((float)((float)(geom->orientation[0][0] * geom->orientation[0][0])
                                                         + (float)(geom->orientation[0][1] * geom->orientation[0][1]))
                                         + (float)(geom->orientation[0][2] * geom->orientation[0][2])) <= 0.0099999998
                        && (float)((float)((float)(geom->orientation[1][0] * geom->orientation[1][0])
                                                         + (float)(geom->orientation[1][1] * geom->orientation[1][1]))
                                         + (float)(geom->orientation[1][2] * geom->orientation[1][2])) <= 0.0099999998
                        && (float)((float)((float)(geom->orientation[2][0] * geom->orientation[2][0])
                                                         + (float)(geom->orientation[2][1] * geom->orientation[2][1]))
                                         + (float)(geom->orientation[2][2] * geom->orientation[2][2])) <= 0.0099999998 )
                    {
                        mins[0] = geom->offset[0] - geom->halfLengths[0];
                        mins[1] = geom->offset[1] - geom->halfLengths[1];
                        mins[2] = geom->offset[2] - geom->halfLengths[2];
                        maxs[0] = geom->offset[0] + geom->halfLengths[0];
                        maxs[1] = geom->offset[1] + geom->halfLengths[1];
                        maxs[2] = geom->offset[2] + geom->halfLengths[2];
                        if ( allocator->query_create_prolog_1(allocator, mins, maxs, geom) )
                        {
                            aabb_gjk_geom = create_aabb_gjk_geom(COERCE_FLOAT(&savedregs), mins, maxs, stype, allocator);
                            if ( worldMat )
                                gjk_base_t::set_xform(aabb_gjk_geom, worldMat);
                            gjk_base_t::set_contents(aabb_gjk_geom, contents_for_proxy_collision);
                            allocator->query_create_epilog_1(allocator, aabb_gjk_geom);
                        }
                    }
                    else
                    {
                        for ( i = 0; i < 8; ++i )
                        {
                            for ( j = 0; j < 3; ++j )
                            {
                                if ( (i & (1 << j)) != 0 )
                                    obj = geom->halfLengths[j];
                                else
                                    v7 = -geom->halfLengths[j];
                                v[j] = obj;
                            }
                            MatrixTransformVector(v, geom->orientation, trasformed_v);
                            Vec3Min(mins, trasformed_v, mins);
                            Vec3Max(maxs, trasformed_v, maxs);
                        }
                        mins[0] = mins[0] + geom->offset[0];
                        mins[1] = mins[1] + geom->offset[1];
                        mins[2] = mins[2] + geom->offset[2];
                        maxs[0] = maxs[0] + geom->offset[0];
                        maxs[1] = maxs[1] + geom->offset[1];
                        maxs[2] = maxs[2] + geom->offset[2];
                        if ( allocator->query_create_prolog_1(allocator, mins, maxs, geom) )
                        {
                            obb_gjk_geom = create_obb_gjk_geom(
                                                             COERCE_FLOAT(&savedregs),
                                                             geom->orientation,
                                                             geom->offset,
                                                             geom->halfLengths,
                                                             stype,
                                                             allocator);
                            if ( worldMat )
                                gjk_base_t::set_xform(obb_gjk_geom, worldMat);
                            gjk_base_t::set_contents(obb_gjk_geom, contents_for_proxy_collision);
                            allocator->query_create_epilog_1(allocator, obb_gjk_geom);
                        }
                    }
                }
                else
                {
                    radius = geom->halfLengths[1];
                    hheight = geom->halfLengths[0];
                    bs_radius = radius + hheight;
                    if ( (float)((float)(radius + hheight) * (float)(radius + hheight)) < (float)((float)(radius * radius)
                                                                                                                                                                            + (float)(hheight * hheight))
                        && !Assert_MyHandler(
                                    "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp",
                                    652,
                                    0,
                                    "%s",
                                    "bs_radius * bs_radius >= radius * radius + hheight * hheight") )
                    {
                        __debugbreak();
                    }
                    rvec[0] = bs_radius;
                    rvec[1] = bs_radius;
                    rvec[2] = bs_radius;
                    v12[0] = geom->offset[0] - bs_radius;
                    v12[1] = geom->offset[1] - bs_radius;
                    v12[2] = geom->offset[2] - bs_radius;
                    v15[0] = geom->offset[0] + bs_radius;
                    v15[1] = geom->offset[1] + bs_radius;
                    v15[2] = geom->offset[2] + bs_radius;
                    if ( allocator->query_create_prolog_1(allocator, v12, v15, geom) )
                    {
                        cylinder_gjk_geom = create_cylinder_gjk_geom(
                                                                    COERCE_FLOAT(&savedregs),
                                                                    geom->orientation,
                                                                    geom->offset,
                                                                    radius,
                                                                    hheight,
                                                                    stype,
                                                                    allocator);
                        if ( worldMat )
                            gjk_base_t::set_xform(cylinder_gjk_geom, worldMat);
                        gjk_base_t::set_contents(cylinder_gjk_geom, contents_for_proxy_collision);
                        allocator->query_create_epilog_1(allocator, cylinder_gjk_geom);
                    }
                }
            }
            ++geomIndex;
            ++geom;
        }
    }
    if ( !b_geom_eligible && b_fallback_to_bounding_box )
    {
        XModelGetBounds(model, mins, maxs);
        if ( mins[0] == maxs[0] || mins[1] == maxs[1] || mins[2] == maxs[2] )
        {
            mins[0] = -r_0;
            mins[1] = -r_0;
            mins[2] = -r_0;
            maxs[0] = r_0;
            maxs[1] = r_0;
            maxs[2] = r_0;
        }
        if ( allocator->query_create_prolog_1(allocator, mins, maxs, model) )
        {
            v9 = create_aabb_gjk_geom(COERCE_FLOAT(&savedregs), mins, maxs, stype, allocator);
            if ( worldMat )
                gjk_base_t::set_xform(v9, worldMat);
            gjk_base_t::set_contents(v9, contents_for_proxy_collision);
            allocator->query_create_epilog_1(allocator, v9);
        }
    }
}

void __cdecl create_gjk_geom(
                int localClientNum,
                const centity_s *cent,
                gjk_collision_visitor *allocator,
                bool calc_bone_mats,
                unsigned int brush_mask,
                bool b_fallback_to_bounding_box,
                bool b_use_smallerbox_for_characters)
{
    DObj *ClientDObj; // eax
    const DObj *dobj; // [esp+0h] [ebp-48h]
    gjk_polygon_cylinder_t *gjk_geom; // [esp+Ch] [ebp-3Ch]
    float mins[3]; // [esp+10h] [ebp-38h] BYREF
    int stype; // [esp+1Ch] [ebp-2Ch]
    float maxs[3]; // [esp+20h] [ebp-28h] BYREF
    float query_maxs[3]; // [esp+2Ch] [ebp-1Ch] BYREF
    float query_mins[3]; // [esp+38h] [ebp-10h] BYREF
    int contents_for_proxy_collision; // [esp+44h] [ebp-4h]
    int savedregs; // [esp+48h] [ebp+0h] BYREF

    if ( !allocator
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp", 765, 0, "%s", "allocator") )
    {
        __debugbreak();
    }
    dobj = Com_GetClientDObj(cent->nextState.number, localClientNum);
    if ( dobj )
        contents_for_proxy_collision = CG_GetEntityDObjContents(cent, dobj);
    else
        contents_for_proxy_collision = -1;
    if ( cent->nextState.eType == 17 || cent->nextState.eType == 1 )
    {
        stype = 29;
        if ( cent->nextState.solid )
        {
            CG_GetEntityBModelBounds((int)&savedregs, cent, mins, maxs, 0, 0);
            mins[0] = mins[0] - 1.0;
            mins[1] = mins[1] - 1.0;
            mins[2] = mins[2] - 1.0;
            maxs[0] = maxs[0] + 1.0;
            maxs[1] = maxs[1] + 1.0;
            maxs[2] = maxs[2] + 1.0;
        }
        else
        {
            mins[0] = -15.0f;
            mins[1] = -15.0f;
            mins[2] = 0.0f;
            maxs[0] = 15.0f;
            maxs[1] = 15.0f;
            maxs[2] = FLOAT_72_0;
        }
        if ( b_use_smallerbox_for_characters )
            mins[2] = mins[2] + 15.0;
        query_mins[0] = mins[0];
        query_mins[1] = mins[1];
        query_mins[2] = mins[2];
        query_maxs[0] = maxs[0];
        query_maxs[1] = maxs[1];
        query_maxs[2] = maxs[2];
        adjust_gjk_polygon_cylinder_query_aabb(query_mins, query_maxs);
        if ( allocator->query_create_prolog_1(allocator, query_mins, query_maxs, cent) )
        {
            gjk_geom = create_gjkcc_gjk_geom((float (*)[3])mins, (float (*)[3])maxs, stype, allocator);
            gjk_base_t::set_contents(gjk_geom, contents_for_proxy_collision);
            allocator->query_create_epilog_1(allocator, gjk_geom);
        }
    }
    else if ( (char *)cent->nextState.solid != &cls.rankedServers[711].game[34]
                 || cent->nextState.eType == 14
                 || cent->nextState.eType == 16 )
    {
        ClientDObj = Com_GetClientDObj(cent->nextState.number, localClientNum);
        create_gjk_dobj_geom(
            ClientDObj,
            &cent->pose,
            0,
            calc_bone_mats,
            brush_mask,
            b_fallback_to_bounding_box,
            contents_for_proxy_collision,
            allocator);
    }
    else
    {
        create_brush_model_gjk_geom(cent->nextState.index.bone, brush_mask, allocator);
    }
}

void __cdecl create_gjk_dobj_geom(
                DObj *obj,
                const cpose_t *pose,
                gentity_s *gent,
                bool calc_bone_mats,
                unsigned int brush_mask,
                bool b_fallback_to_bounding_box,
                int contents_for_proxy_collision,
                gjk_collision_visitor *allocator)
{
    XModel *model; // [esp+0h] [ebp-4h]

    if ( !allocator
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp", 693, 0, "%s", "allocator") )
    {
        __debugbreak();
    }
    if ( obj )
    {
        model = DObjGetModel(obj, 0);
        if ( model->numCollmaps )
        {
            if ( xmodel_get_geomlist(model, -1) )
                create_xmodel_gjk_geom(
                    model,
                    allocator,
                    -1,
                    brush_mask,
                    b_fallback_to_bounding_box,
                    contents_for_proxy_collision,
                    0);
        }
        else if ( b_fallback_to_bounding_box )
        {
            create_xmodel_gjk_geom(
                model,
                allocator,
                -1,
                brush_mask,
                b_fallback_to_bounding_box,
                contents_for_proxy_collision,
                0);
        }
    }
}

void __cdecl create_gjk_geom(
                gentity_s *gent,
                gjk_collision_visitor *allocator,
                bool calc_bone_mats,
                unsigned int brush_mask,
                bool b_fallback_to_bounding_box)
{
    DObj *ServerDObj; // eax
    gjk_polygon_cylinder_t *gjk_geom; // [esp+10h] [ebp-24h]
    float query_maxs[3]; // [esp+18h] [ebp-1Ch] BYREF
    float query_mins[3]; // [esp+24h] [ebp-10h] BYREF
    int contents_for_proxy_collision; // [esp+30h] [ebp-4h]

    if ( !allocator
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp", 825, 0, "%s", "allocator") )
    {
        __debugbreak();
    }
    contents_for_proxy_collision = gent->r.contents;
    if ( gent->s.eType == 17 || gent->s.eType == 1 )
    {
        query_mins[0] = gent->r.mins[0];
        query_mins[1] = gent->r.mins[1];
        query_mins[2] = gent->r.mins[2];
        query_maxs[0] = gent->r.maxs[0];
        query_maxs[1] = gent->r.maxs[1];
        query_maxs[2] = gent->r.maxs[2];
        adjust_gjk_polygon_cylinder_query_aabb(query_mins, query_maxs);
        if ( allocator->query_create_prolog_1(allocator, query_mins, query_maxs, gent) )
        {
            gjk_geom = create_gjkcc_gjk_geom((float (*)[3])gent->r.mins, (float (*)[3])gent->r.maxs, 29, allocator);
            gjk_base_t::set_contents(gjk_geom, contents_for_proxy_collision);
            allocator->query_create_epilog_1(allocator, gjk_geom);
        }
    }
    else if ( gent->classname == scr_const.script_brushmodel || gent->classname == scr_const.glass )
    {
        create_brush_model_gjk_geom(gent->s.index.bone, brush_mask, allocator);
    }
    else
    {
        ServerDObj = Com_GetServerDObj(gent->s.number);
        create_gjk_dobj_geom(
            ServerDObj,
            0,
            gent,
            calc_bone_mats,
            brush_mask,
            b_fallback_to_bounding_box,
            contents_for_proxy_collision,
            allocator);
    }
}

void __cdecl create_gjk_geom(const Glass *glass, gjk_collision_visitor *allocator, unsigned int brush_mask)
{
    if ( !allocator
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp", 860, 0, "%s", "allocator") )
    {
        __debugbreak();
    }
    create_brush_model_gjk_geom(glass->brushModel, brush_mask, allocator);
}

void __cdecl create_gjk_geom(const DynEntityDef *dynEntDef, gjk_collision_visitor *allocator, unsigned int brush_mask)
{
    gjk_aabb_t *gjk_geom; // [esp+0h] [ebp-24h]
    int stype; // [esp+4h] [ebp-20h]
    float mins[3]; // [esp+8h] [ebp-1Ch] BYREF
    float maxs[3]; // [esp+14h] [ebp-10h] BYREF
    int contents_for_proxy_collision; // [esp+20h] [ebp-4h]
    int savedregs; // [esp+24h] [ebp+0h] BYREF

    if ( !allocator
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.cpp", 869, 0, "%s", "allocator") )
    {
        __debugbreak();
    }
    contents_for_proxy_collision = dynEntDef->contents;
    if ( dynEntDef->xModel && XModelHasCollmap(dynEntDef->xModel) )
    {
        create_xmodel_gjk_geom(dynEntDef->xModel, allocator, -1, brush_mask, 1, contents_for_proxy_collision, 0);
    }
    else if ( !dynEntDef->xModel || dynEntDef->physicsBrushModel )
    {
        if ( dynEntDef->physicsBrushModel )
            create_brush_model_gjk_geom(dynEntDef->physicsBrushModel, brush_mask, allocator);
        else
            create_brush_model_gjk_geom(dynEntDef->brushModel, brush_mask, allocator);
    }
    else
    {
        DynEnt_GetLocalBounds(dynEntDef, mins, maxs);
        if ( allocator->query_create_prolog_1(allocator, mins, maxs, dynEntDef) )
        {
            stype = DynEnt_GetSurfaceType(dynEntDef);
            gjk_geom = create_aabb_gjk_geom(COERCE_FLOAT(&savedregs), mins, maxs, stype, allocator);
            gjk_base_t::set_contents(gjk_geom, contents_for_proxy_collision);
            allocator->query_create_epilog_1(allocator, gjk_geom);
        }
    }
}

gjk_aabb_t *phys_simple_allocator<gjk_aabb_t>::allocate(phys_simple_allocator<gjk_aabb_t> *this)
{
    char *slot; // [esp+18h] [ebp-4h]

    slot = PMM_ALLOC(0x80u, 0x10u);
    if ( !slot )
        return 0;
    ++this->m_count;
    *(unsigned int *)slot = &phys_gjk_geom::`vftable';
    *(unsigned int *)slot = &gjk_base_t::`vftable';
    *((unsigned int *)slot + 15) = 0;
    *(unsigned int *)slot = &gjk_aabb_t::`vftable';
    *((unsigned int *)slot + 28) = 0;
    return (gjk_aabb_t *)slot;
}

gjk_obb_t *phys_simple_allocator<gjk_obb_t>::allocate(phys_simple_allocator<gjk_obb_t> *this)
{
    char *slot; // [esp+18h] [ebp-4h]

    slot = PMM_ALLOC(0xA0u, 0x10u);
    if ( !slot )
        return 0;
    ++this->m_count;
    *(unsigned int *)slot = &phys_gjk_geom::`vftable';
    *(unsigned int *)slot = &gjk_base_t::`vftable';
    *((unsigned int *)slot + 15) = 0;
    *(unsigned int *)slot = &gjk_obb_t::`vftable';
    return (gjk_obb_t *)slot;
}

gjk_brush_t *phys_simple_allocator<gjk_brush_t>::allocate(phys_simple_allocator<gjk_brush_t> *this)
{
    char *slot; // [esp+18h] [ebp-4h]

    slot = PMM_ALLOC(0x60u, 0x10u);
    if ( !slot )
        return 0;
    ++this->m_count;
    *(unsigned int *)slot = &phys_gjk_geom::`vftable';
    *(unsigned int *)slot = &gjk_base_t::`vftable';
    *((unsigned int *)slot + 15) = 0;
    *(unsigned int *)slot = &gjk_brush_t::`vftable';
    return (gjk_brush_t *)slot;
}

void phys_simple_allocator<gjk_brush_t>::free(phys_simple_allocator<gjk_brush_t> *this, gjk_brush_t *slot)
{
    if ( slot )
    {
        PMM_VALIDATE((char *)slot, 0x60u, 0x10u);
        --this->m_count;
        gjk_base_t::~gjk_base_t(slot);
        PMM_FREE((unsigned __int8 *)slot, 0x60u, 0x10u);
    }
}

gjk_partition_t *phys_simple_allocator<gjk_partition_t>::allocate(
                phys_simple_allocator<gjk_partition_t> *this)
{
    char *slot; // [esp+18h] [ebp-4h]

    slot = PMM_ALLOC(0x70u, 0x10u);
    if ( !slot )
        return 0;
    ++this->m_count;
    *(unsigned int *)slot = &phys_gjk_geom::`vftable';
    *(unsigned int *)slot = &gjk_base_t::`vftable';
    *((unsigned int *)slot + 15) = 0;
    *(unsigned int *)slot = &gjk_partition_t::`vftable';
    return (gjk_partition_t *)slot;
}

void phys_simple_allocator<gjk_partition_t>::free(
                phys_simple_allocator<gjk_partition_t> *this,
                gjk_partition_t *slot)
{
    if ( slot )
    {
        PMM_VALIDATE((char *)slot, 0x70u, 0x10u);
        --this->m_count;
        gjk_base_t::~gjk_base_t(slot);
        PMM_FREE((unsigned __int8 *)slot, 0x70u, 0x10u);
    }
}

gjk_double_sphere_t *phys_simple_allocator<gjk_double_sphere_t>::allocate(
                phys_simple_allocator<gjk_double_sphere_t> *this)
{
    char *slot; // [esp+20h] [ebp-4h]

    slot = PMM_ALLOC(0x90u, 0x10u);
    if ( !slot )
        return 0;
    ++this->m_count;
    return gjk_double_sphere_t::gjk_double_sphere_t((gjk_double_sphere_t *)slot);
}

void phys_simple_allocator<gjk_double_sphere_t>::free(
                phys_simple_allocator<gjk_double_sphere_t> *this,
                gjk_double_sphere_t *slot)
{
    if ( slot )
    {
        PMM_VALIDATE((char *)slot, 0x90u, 0x10u);
        --this->m_count;
        gjk_base_t::~gjk_base_t(slot);
        PMM_FREE((unsigned __int8 *)slot, 0x90u, 0x10u);
    }
}

gjk_cylinder_t *phys_simple_allocator<gjk_cylinder_t>::allocate(phys_simple_allocator<gjk_cylinder_t> *this)
{
    char *slot; // [esp+18h] [ebp-4h]

    slot = PMM_ALLOC(0xA0u, 0x10u);
    if ( !slot )
        return 0;
    ++this->m_count;
    *(unsigned int *)slot = &phys_gjk_geom::`vftable';
    *(unsigned int *)slot = &gjk_base_t::`vftable';
    *((unsigned int *)slot + 15) = 0;
    *(unsigned int *)slot = &gjk_cylinder_t::`vftable';
    return (gjk_cylinder_t *)slot;
}

void phys_simple_allocator<gjk_cylinder_t>::free(
                phys_simple_allocator<gjk_cylinder_t> *this,
                gjk_cylinder_t *slot)
{
    if ( slot )
    {
        PMM_VALIDATE((char *)slot, 0xA0u, 0x10u);
        --this->m_count;
        gjk_base_t::~gjk_base_t(slot);
        PMM_FREE((unsigned __int8 *)slot, 0xA0u, 0x10u);
    }
}

gjk_polygon_cylinder_t *phys_simple_allocator<gjk_polygon_cylinder_t>::allocate(
                phys_simple_allocator<gjk_polygon_cylinder_t> *this)
{
    char *slot; // [esp+18h] [ebp-4h]

    slot = PMM_ALLOC(0x80u, 0x10u);
    if ( !slot )
        return 0;
    ++this->m_count;
    *(unsigned int *)slot = &phys_gjk_geom::`vftable';
    *(unsigned int *)slot = &gjk_base_t::`vftable';
    *((unsigned int *)slot + 15) = 0;
    *(unsigned int *)slot = &gjk_polygon_cylinder_t::`vftable';
    return (gjk_polygon_cylinder_t *)slot;
}

void phys_simple_allocator<gjk_polygon_cylinder_t>::free(
                phys_simple_allocator<gjk_polygon_cylinder_t> *this,
                gjk_polygon_cylinder_t *slot)
{
    if ( slot )
    {
        PMM_VALIDATE((char *)slot, 0x80u, 0x10u);
        --this->m_count;
        gjk_base_t::~gjk_base_t(slot);
        PMM_FREE((unsigned __int8 *)slot, 0x80u, 0x10u);
    }
}

void gjk_polygon_cylinder_t::poly_verts::poly_verts(gjk_polygon_cylinder_t::poly_verts *this)
{
    double v1; // xmm0_8
    double v2; // xmm0_8
    long double thisa; // [esp+0h] [ebp-Ch]
    long double thisb; // [esp+0h] [ebp-Ch]
    int i; // [esp+8h] [ebp-4h]

    LODWORD(thisa) = this;
    for ( i = 0; i < 4; ++i )
    {
        *((float *)&thisa + 1) = (float)((float)((float)i * 3.1415925) / 2.0) / 3.0;
        v1 = *((float *)&thisa + 1);
        __libm_sse2_cos(thisa);
        *(float *)&v1 = v1;
        *(unsigned int *)(LODWORD(thisb) + 4 * i) = LODWORD(v1);
        mp = *((float *)&thisb + 1);
        __libm_sse2_sin(thisb);
        *(float *)&mp = mp;
        *(unsigned int *)(LODWORD(thisa) + 4 * i + 16) = LODWORD(v2);
    }
}

void __cdecl destroy_gjk_geom(gjk_geom_list_t *gjk_geom_list)
{
    gjk_base_t *next_geom; // [esp+0h] [ebp-8h]
    gjk_aabb_t *geom; // [esp+4h] [ebp-4h]

    if ( gjk_geom_list->m_geom_count < 0
        && _tlAssert(
                 "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
                 1035,
                 "m_geom_count >= 0",
                 "") )
    {
        __debugbreak();
    }
    for ( geom = (gjk_aabb_t *)gjk_geom_list->m_first_geom; geom; geom = (gjk_aabb_t *)next_geom )
    {
        next_geom = geom->m_next_geom;
        destroy_gjk_geom(geom);
    }
    gjk_geom_list->m_first_geom = 0;
    gjk_geom_list->m_geom_count = 0;
}

