#include "phys_collision.h"
#include "phys_collision_multithreaded.h"
#include "rigid_body.h"
#include "phys_transient_allocator.h"
#include "phys_assert.h"
#include <DynEntity/DynEntity_client.h>
#include <bgame/bg_slidemove.h>
#include <cgame/cg_drawtools.h>
#include "phys_render.h"
#include "physics_system.h"
#include "phys_contact_manifold.h"
#include <tl/physics/rbc_def_generic.h>
#include <universal/com_math_anglevectors.h>
#include <DynEntity/DynEntity_load_obj.h>
#include <new>
#include <qcommon/cm_load.h>
#include <cgame/cg_world.h>
#include <qcommon/dobj_management.h>
#include <xanim/dobj_utils.h>
#include <xanim/xmodel_utils.h>
#include "phys_auto_rigid_body.h"
#include <cgame_mp/cg_vehicles_mp.h>
#include <glass/glass_client.h>
#include <algorithm>

phys_assert_info pai_create_cpi = { 0, 2, true };
phys_simple_allocator<generic_avl_map_node_t> g_generic_avl_map_node_allocator;
standard_query g_standard_query;

//rigid_body_constraint_contact *__cdecl avl_tree_find<rigid_body_pair_key, rigid_body_constraint_contact, rigid_body_constraint_contact::avl_tree_accessor>(
rigid_body_constraint_contact *__cdecl avl_tree_find(rigid_body_constraint_contact *tree_root, const rigid_body_pair_key *key)
{
    rigid_body_constraint_contact *result; // eax
    rigid_body *m_b1; // edx
    rigid_body *v4; // ecx
    bool v5; // cf

    result = tree_root;
    if (tree_root)
    {
        m_b1 = key->m_b1;
        do
        {
            if (m_b1 == result->m_avl_key.m_b1 && key->m_b2 == result->m_avl_key.m_b2)
                break;
            v4 = result->m_avl_key.m_b1;
            v5 = m_b1 < v4;
            if (m_b1 == v4)
                v5 = key->m_b2 < result->m_avl_key.m_b2;
            result = v5 ? result->m_avl_tree_node.m_left : result->m_avl_tree_node.m_right;
        } while (result);
    }
    return result;
}

void __thiscall contact_point_info::get_closest_psc(
                const phys_vec3 *normal,
                const phys_vec3 *b1_r_loc,
                const phys_vec3 *b2_r_loc,
                float *closest_error,
                const contact_point_info::pulse_sum_cache_info **closest_psc)
{
    phys_vec3 *m_list_b1_r_loc; // edx
    phys_vec3 *m_list_b2_r_loc; // esi
    const phys_vec3 *v8; // edi
    double v9; // st7
    double v10; // st6
    float v11; // [esp-30h] [ebp-3Ch]
    float v12; // [esp-2Ch] [ebp-38h]
    float v13; // [esp-28h] [ebp-34h]
    float v14; // [esp-20h] [ebp-2Ch]
    float v15; // [esp-20h] [ebp-2Ch]
    float v16; // [esp-1Ch] [ebp-28h]
    float v17; // [esp-1Ch] [ebp-28h]
    float v18; // [esp-18h] [ebp-24h]
    float v19; // [esp-18h] [ebp-24h]
    float v20; // [esp-Ch] [ebp-18h]
    float v21; // [esp-Ch] [ebp-18h]
    float v22; // [esp-Ch] [ebp-18h]
    float v23; // [esp-Ch] [ebp-18h]
    int v24; // [esp-8h] [ebp-14h]
    int v25; // [esp-4h] [ebp-10h]

    m_list_b1_r_loc = this->m_list_b1_r_loc;
    m_list_b2_r_loc = this->m_list_b2_r_loc;
    v24 = 0;
    if ( this->m_point_pair_count > 0 )
    {
        v8 = b1_r_loc;
        v14 = normal->x - this->m_normal.x;
        v16 = normal->y - this->m_normal.y;
        v18 = normal->z - this->m_normal.z;
        v20 = v14 * v14 + v16 * v16 + v18 * v18;
        v9 = v20;
        v25 = 0;
        do
        {
            v15 = m_list_b1_r_loc->x - v8->x;
            v17 = m_list_b1_r_loc->y - v8->y;
            v19 = m_list_b1_r_loc->z - v8->z;
            v11 = m_list_b2_r_loc->x - b2_r_loc->x;
            v12 = m_list_b2_r_loc->y - b2_r_loc->y;
            v13 = m_list_b2_r_loc->z - b2_r_loc->z;
            v21 = v19 * v19 + v15 * v15 + v17 * v17;
            v10 = v21 + v9;
            v22 = v13 * v13 + v12 * v12 + v11 * v11;
            v23 = v10 + v22;
            if ( *closest_error > (double)v23 )
            {
                *closest_error = v23;
                *closest_psc = &this->m_list_pulse_sum_cache_info[v25];
                v8 = b1_r_loc;
            }
            ++v24;
            ++v25;
            ++m_list_b1_r_loc;
            ++m_list_b2_r_loc;
        }
        while ( v24 < this->m_point_pair_count );
    }
}

void phys_contact_manifold::xform_and_translate_mesh_points(
                const phys_mat44 *xform,
                const phys_vec3 *translation)
{
    phys_contact_manifold *v4; // esi
    const phys_vec3 **m_list_sorted_mesh_point; // eax
    const phys_vec3 **v6; // ecx
    phys_vec3 *v7; // esi
    const phys_vec3 *v8; // eax
    double v9; // st7
    const phys_vec3 **v10; // eax
    const phys_vec3 *v11; // eax
    phys_vec3 v12; // [esp-40h] [ebp-4Ch] BYREF
    float v13; // [esp-30h] [ebp-3Ch]
    float v14; // [esp-2Ch] [ebp-38h]
    float v15; // [esp-28h] [ebp-34h]
    float v16; // [esp-20h] [ebp-2Ch]
    float v17; // [esp-1Ch] [ebp-28h]
    float v18; // [esp-18h] [ebp-24h]
    phys_contact_manifold *v19; // [esp-Ch] [ebp-18h]
    const phys_vec3 **v20; // [esp-8h] [ebp-14h]
    const phys_vec3 **v21; // [esp-4h] [ebp-10h]
    int v22; // [esp+0h] [ebp-Ch]
    //contact_manifold_mesh_point **last_mp_i; // [esp+4h] [ebp-8h]
    //contact_manifold_mesh_point **retaddr; // [esp+Ch] [ebp+0h]

    //v22 = a2;
    //last_mp_i = retaddr;
    v4 = this;
    m_list_sorted_mesh_point = (const phys_vec3 **)this->m_list_sorted_mesh_point;
    v6 = &m_list_sorted_mesh_point[this->m_list_mesh_point_count];
    v19 = v4;
    v20 = v6;
    v21 = m_list_sorted_mesh_point;
    if ( m_list_sorted_mesh_point != v6 )
    {
        while ( 1 )
        {
            v7 = (phys_vec3 *)*m_list_sorted_mesh_point;
            v8 = phys_multiply(&v12, xform, *m_list_sorted_mesh_point);
            v16 = v8->x + xform->w.x;
            v17 = v8->y + xform->w.y;
            v18 = v8->z + xform->w.z;
            v13 = translation->x + v16;
            v14 = translation->y + v17;
            v9 = translation->z + v18;
            v10 = ++v21;
            v15 = v9;
            v7->x = v13;
            v7->y = v14;
            v7->z = v15;
            if ( v10 == v20 )
                break;
            m_list_sorted_mesh_point = v21;
        }
        v4 = v19;
    }
    v11 = phys_multiply(&v12, xform, &v4->m_feature_normal);
    v4->m_feature_normal.x = v11->x;
    v4->m_feature_normal.y = v11->y;
    v4->m_feature_normal.z = v11->z;
}



void __thiscall phys_contact_manifold_process::comp_contact_mat(const phys_vec3 *contact_normal)
{
    double v3; // st6
    double v4; // st7
    float v5; // [esp-1Ch] [ebp-3Ch]
    float v6; // [esp-1Ch] [ebp-3Ch]
    float v7; // [esp-1Ch] [ebp-3Ch]
    float v8; // [esp-18h] [ebp-38h]
    float v9; // [esp-18h] [ebp-38h]
    float v10; // [esp-18h] [ebp-38h]
    float v11; // [esp-14h] [ebp-34h]
    float v12; // [esp-14h] [ebp-34h]
    float v13; // [esp-14h] [ebp-34h]
    float v14; // [esp-Ch] [ebp-2Ch]
    float v15; // [esp-Ch] [ebp-2Ch]
    float v16; // [esp-8h] [ebp-28h]
    float v17; // [esp-8h] [ebp-28h]
    float v18; // [esp-4h] [ebp-24h]
    float v19; // [esp+10h] [ebp-10h]
    float y; // [esp+10h] [ebp-10h]
    float v21; // [esp+10h] [ebp-10h]
    float v22; // [esp+10h] [ebp-10h]
    float v23; // [esp+10h] [ebp-10h]

    v19 = fabs(contact_normal->x);
    if ( v19 >= 0.800000011920929 )
    {
        y = contact_normal->y;
        v15 = contact_normal->x * y;
        v17 = y * y;
        v18 = y * contact_normal->z;
        v4 = 0.0;
        v5 = 0.0 - v15;
        v3 = 1.0 - v17;
    }
    else
    {
        v14 = contact_normal->x * contact_normal->x;
        v16 = contact_normal->y * contact_normal->x;
        v18 = contact_normal->x * contact_normal->z;
        v5 = 1.0 - v14;
        v3 = 0.0 - v16;
        v4 = 0.0;
    }
    v8 = v3;
    v11 = v4 - v18;
    v21 = v8 * v8 + v5 * v5 + v11 * v11;
    v22 = sqrt(v21);
    if ( v22 <= 0.1000000014901161
        && _tlAssert(
                 "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_contact_manifold.h",
                 397,
                 "nyrow > 0.1f",
                 "") )
    {
        __debugbreak();
    }
    v23 = 1.0 / v22;
    v6 = v23 * v5;
    v9 = v8 * v23;
    v12 = v23 * v11;
    this->contact_mat.y.x = v6;
    this->contact_mat.y.y = v9;
    this->contact_mat.y.z = v12;
    v7 = contact_normal->z * this->contact_mat.y.y - contact_normal->y * this->contact_mat.y.z;
    v10 = contact_normal->x * this->contact_mat.y.z - contact_normal->z * this->contact_mat.y.x;
    v13 = this->contact_mat.y.x * contact_normal->y - contact_normal->x * this->contact_mat.y.y;
    this->contact_mat.x.x = v7;
    this->contact_mat.x.y = v10;
    this->contact_mat.x.z = v13;
    this->contact_mat.z.x = contact_normal->x;
    this->contact_mat.z.y = contact_normal->y;
    this->contact_mat.z.z = contact_normal->z;
}

#if 0 // KISAKTODO 
rigid_body_constraint_contact *__cdecl avl_tree_find<rigid_body_pair_key,rigid_body_constraint_contact,rigid_body_constraint_contact::avl_tree_accessor>(
                rigid_body_constraint_contact *tree_root,
                const rigid_body_pair_key *key)
{
    rigid_body_constraint_contact *result; // eax
    rigid_body *m_b1; // edx
    rigid_body *v4; // ecx
    bool v5; // cf

    result = tree_root;
    if ( tree_root )
    {
        m_b1 = key->m_b1;
        do
        {
            if ( m_b1 == result->m_avl_key.m_b1 && key->m_b2 == result->m_avl_key.m_b2 )
                break;
            v4 = result->m_avl_key.m_b1;
            v5 = m_b1 < v4;
            if ( m_b1 == v4 )
                v5 = key->m_b2 < result->m_avl_key.m_b2;
            result = v5 ? result->m_avl_tree_node.m_left : result->m_avl_tree_node.m_right;
        }
        while ( result );
    }
    return result;
}
#endif
contact_point_info *__cdecl contact_point_info::create_cpi(
                int point_pair_count,
                bool no_error,
                phys_transient_allocator *allocator)
{
    unsigned int v3; // edi
    contact_point_info *result; // eax

    if ( point_pair_count <= 0
        && _tlAssert(
                 "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\rbc_defs\\rbc_def_contact.h",
                 159,
                 "point_pair_count > 0",
                 "") )
    {
        __debugbreak();
    }
    v3 = (32 * point_pair_count + 95) & 0xFFFFFFF0;
    //result = (contact_point_info *)phys_transient_allocator::mt_allocate(
    result = (contact_point_info *)allocator->mt_allocate(v3 + ((12 * point_pair_count + 15) & 0xFFFFFFF0), 16, 1, "contact_point_info buffer overflow");
    if ( result )
    {
        result->m_fric_coef = -1.0;
        result->m_bounce_coef = -1.0;
        result->m_max_restitution_vel = -1.0;
        result->m_list_b1_r_loc = &result[1].m_normal;
        result->m_list_b2_r_loc = &result[1].m_normal + point_pair_count;
        result->m_list_pulse_sum_cache_info = (contact_point_info::pulse_sum_cache_info *)((char *)result + v3);
        result->m_point_pair_count = point_pair_count;
        result->m_flags = 0;
        result->m_rb2_entity = (const void *)-1;
    }
    else if ( !no_error )
    {
        if ( (pai_create_cpi.m_hits_total_count < pai_create_cpi.m_max_hits_total || !pai_create_cpi.m_max_hits_total)
            && pai_create_cpi.m_hits_frame_count < pai_create_cpi.m_max_hits_per_frame )
        {
            if ( pai_create_cpi.m_use_warnings_only )
            {
                PHYS_WARNING(
                    "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\rbc_defs\\rbc_def_contact.h",
                    173,
                    "cpi || no_error",
                    "contact_point_info buffer overflow");
            }
            else if ( _tlAssert(
                                    "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\rbc_defs\\rbc_def_contact.h",
                                    173,
                                    "cpi || no_error",
                                    "contact_point_info buffer overflow") )
            {
                __debugbreak();
            }
        }
        _InterlockedExchangeAdd(&pai_create_cpi.m_hits_total_count, 1u);
        _InterlockedExchangeAdd(&pai_create_cpi.m_hits_frame_count, 1u);
        return 0;
    }
    return result;
}

void __thiscall contact_point_info::set_solver_priority(unsigned int priority)
{
    if ((priority & 7) != priority
        && _tlAssert(
            "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\rbc_defs\\rbc_def_contact.h",
            78,
            "(priority & FLAG_SOLVER_PRIORITY_MASK) == priority",
            ""))
    {
        __debugbreak();
    }
    this->m_flags &= 0xFFFFFFF8;
    this->m_flags |= priority;
}

void __thiscall contact_point_info::set_rb2_entity(const void *rb2_entity)
{
    this->m_flags |= 0x10u;
    this->m_rb2_entity = rb2_entity;
}

void __thiscall contact_point_info::check_surface_properties()
{
    if (this->m_fric_coef < 0.0
        && _tlAssert(
            "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\rbc_defs\\rbc_def_contact.h",
            137,
            "m_fric_coef >= 0.0f",
            ""))
    {
        __debugbreak();
    }
    if ((this->m_bounce_coef > 1.0 || this->m_bounce_coef < 0.0)
        && _tlAssert(
            "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\rbc_defs\\rbc_def_contact.h",
            138,
            "m_bounce_coef <= 1.0f && m_bounce_coef >= 0.0f",
            ""))
    {
        __debugbreak();
    }
    if (this->m_max_restitution_vel < 0.0
        && _tlAssert(
            "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\rbc_defs\\rbc_def_contact.h",
            139,
            "m_max_restitution_vel >= 0.0f",
            ""))
    {
        __debugbreak();
    }
}

extern const char *g_contact_manifold_error_msg;
void __thiscall phys_contact_manifold::set_get_feature_params(
                const phys_vec3 *hitp,
                const phys_vec3 *hitn,
                float feature_distance_eps,
                float sin_feautre_angular_eps_sq)
{
    phys_memory_heap *m_allocator; // edi
    const char *v7; // ecx
    unsigned int v8; // eax

    this->m_feature_hitp.x = hitp->x;
    this->m_feature_hitp.y = hitp->y;
    this->m_feature_hitp.z = hitp->z;
    this->m_feature_hitn.x = hitn->x;
    this->m_feature_hitn.y = hitn->y;
    this->m_feature_hitn.z = hitn->z;
    m_allocator = this->m_allocator;
    this->m_feature_distance_eps = feature_distance_eps;
    this->m_sin_feautre_angular_eps_sq = sin_feautre_angular_eps_sq;
    v7 = g_contact_manifold_error_msg;
    v8 = (int)(m_allocator->m_buffer_cur + 15) & 0xFFFFFFF0;
    m_allocator->m_buffer_cur = (char *)v8;
    if ( (char *)v8 >= m_allocator->m_buffer_end
        && _tlAssert(
                 "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
                 114,
                 "m_buffer_cur < m_buffer_end",
                 v7) )
    {
        __debugbreak();
    }
    this->m_list_mesh_point = (contact_manifold_mesh_point *)m_allocator->m_buffer_cur;
    this->m_list_mesh_point_count = 0;
    this->m_close_mesh_point_count = 0;
    this->m_list_sorted_mesh_point = 0;
    this->m_list_contact_point = 0;
}

void __cdecl contact_point_info::set_closest_cached_psc(
                contact_point_info *cached_cpi,
                const phys_vec3 *normal,
                const phys_vec3 *b1_r_loc,
                const phys_vec3 *b2_r_loc,
                contact_point_info::pulse_sum_cache_info *psc)
{
    contact_point_info *v5; // esi
    const contact_point_info::pulse_sum_cache_info *v6; // eax
    float closest_error; // [esp+4h] [ebp-8h] BYREF
    const contact_point_info::pulse_sum_cache_info *closest_psc; // [esp+8h] [ebp-4h] BYREF

    v5 = cached_cpi;
    closest_error = 10000000.0;
    closest_psc = 0;
    if ( !cached_cpi )
        goto LABEL_5;
    do
    {
        //contact_point_info::get_closest_psc(v5, normal, b1_r_loc, b2_r_loc, &closest_error, &closest_psc);
        v5->get_closest_psc(normal, b1_r_loc, b2_r_loc, &closest_error, &closest_psc);
        v5 = v5->m_next_link;
    }
    while ( v5 );
    v6 = closest_psc;
    if ( closest_psc )
    {
        psc->m_ps_cache_list[0].m_pulse_sum = closest_psc->m_ps_cache_list[0].m_pulse_sum;
        psc->m_ps_cache_list[1].m_pulse_sum = v6->m_ps_cache_list[1].m_pulse_sum;
        psc->m_ps_cache_list[2].m_pulse_sum = v6->m_ps_cache_list[2].m_pulse_sum;
    }
    else
    {
LABEL_5:
        if ( psc )
        {
            psc->m_ps_cache_list[0].m_pulse_sum = 0.0;
            psc->m_ps_cache_list[1].m_pulse_sum = 0.0;
            psc->m_ps_cache_list[2].m_pulse_sum = 0.0;
        }
    }
}

void __thiscall contact_point_info::set_closest_cached_psc(contact_point_info *cached_cpi)
{
    contact_point_info::pulse_sum_cache_info *m_list_pulse_sum_cache_info; // esi
    phys_vec3 *m_list_b1_r_loc; // ebx
    contact_point_info::pulse_sum_cache_info *last_psci_i; // [esp+Ch] [ebp-8h]
    const phys_vec3 *b2_r_loc; // [esp+10h] [ebp-4h]

    m_list_pulse_sum_cache_info = this->m_list_pulse_sum_cache_info;
    m_list_b1_r_loc = this->m_list_b1_r_loc;
    b2_r_loc = this->m_list_b2_r_loc;
    for ( last_psci_i = &m_list_pulse_sum_cache_info[this->m_point_pair_count];
                m_list_pulse_sum_cache_info != last_psci_i;
                ++m_list_b1_r_loc )
    {
        contact_point_info::set_closest_cached_psc(
            cached_cpi,
            &this->m_normal,
            m_list_b1_r_loc,
            b2_r_loc++,
            m_list_pulse_sum_cache_info++);
    }
}

const phys_surface_type_info *__cdecl surface_type_info_database_get(int surface_type_1, int surface_type_2)
{
    if (!G_BPM->g_surface_type_info_database
        && _tlAssert(
            "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_broad_phase_inline.h",
            357,
            "G_BPM->g_surface_type_info_database",
            ""))
    {
        __debugbreak();
    }
    return &G_BPM->g_surface_type_info_database[surface_type_info_database_get_index(surface_type_1, surface_type_2)];
}

void __cdecl set_cpi_params(contact_point_info *cpi, phys_collision_pair *pcp)
{
    const phys_surface_type_info *psti; // [esp+1Ch] [ebp-20h]
    const PhysObjUserData *userData2; // [esp+20h] [ebp-1Ch]
    broad_phase_info *bpi1; // [esp+24h] [ebp-18h]
    float bounce; // [esp+28h] [ebp-14h]
    unsigned int priority; // [esp+2Ch] [ebp-10h]
    broad_phase_info *bpi2; // [esp+30h] [ebp-Ch]
    const PhysObjUserData *userData1; // [esp+34h] [ebp-8h]
    float friction; // [esp+38h] [ebp-4h]

    bpi1 = pcp->m_bpi1;
    bpi2 = pcp->m_bpi2;
    userData1 = (const PhysObjUserData *)bpi1->m_rb->m_userdata;
    userData2 = (const PhysObjUserData *)bpi2->m_rb->m_userdata;
    if (!userData1
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp", 59, 0, "%s", "userData1"))
    {
        __debugbreak();
    }
    priority = 0;
    if (userData1->vehicle)
    {
        priority = 1;
        friction = phys_vehicleFriction->current.value;
        bounce = userData1->bounce;
        if (userData1->vehicle->m_vehicle_info->type == 4 && cpi->m_normal.z < -0.69999999)
            friction = userData1->friction;
        if (bpi1->m_rb->m_mat.z.z < -0.1)
            friction = 1.0f;
    }
    else if (userData2)
    {
        friction = (float)(userData1->friction + userData2->friction) * 0.5;
        bounce = (float)(userData1->bounce + userData2->bounce) * 0.5;
    }
    else
    {
        psti = surface_type_info_database_get(bpi1->m_surface_type, bpi2->m_surface_type);
        friction = userData1->friction * psti->m_friction_coef;
        bounce = userData1->bounce * psti->m_bounce_coef;
    }
    cpi->m_fric_coef = friction;
    cpi->m_bounce_coef = bounce;
    cpi->m_max_restitution_vel = 3400.0f;
    //contact_point_info::check_surface_properties(cpi);
    cpi->check_surface_properties();
    //contact_point_info::set_solver_priority(cpi, priority);
    cpi->set_solver_priority(priority);
    cpi->m_flags &= ~8u;
    //contact_point_info::set_rb2_entity(cpi, pcp->m_bpi2);
    cpi->set_rb2_entity(pcp->m_bpi2);
}

const centity_s *get_entity(gjk_physics_collision_visitor *collision_visitor)
{
    if (collision_visitor->cent)
        return collision_visitor->cent;
    if (collision_visitor->glass)
        return (const centity_s *)collision_visitor->glass;

    iassert(collision_visitor->dynEntDef);
    //if (!collision_visitor->dynEntDef
    //    && !Assert_MyHandler(
    //        a1,
    //        "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp",
    //        405,
    //        0,
    //        "%s",
    //        "collision_visitor->dynEntDef"))
    //{
    //    __debugbreak();
    //}
    return (const centity_s *)collision_visitor->dynEntDef;
}

// bad sp value at call has been detected, the output may be wrong!
void phys_contact_manifold_process::process(
                phys_collision_pair *pcp,
                phys_gjk_info *gjk_info)
{
#if 0
    broad_phase_info *m_bpi1; // eax
    rigid_body *m_cg_to_world_xform; // edi
    const phys_mat44 *m_cg_to_rb_xform; // eax
    const phys_mat44 *m_rb_to_world_xform; // edx
    phys_vec3 *p_m_n; // edi
    double v12; // st6
    float y; // edx
    double v14; // st6
    float x; // edx
    double v16; // st7
    float v17; // edx
    double v18; // st7
    float w; // edx
    double v20; // rt1
    double v21; // st6
    double z; // st5
    double v23; // st7
    phys_vec3 *v25; // eax
    int m_list_mesh_point_count; // eax
    int v27; // ecx
    double STD_COMP_FEATURE_NORMAL_SIN_ANGULAR_EPS_SQ; // st7
    phys_contact_manifold *v29; // ecx
    phys_contact_manifold *v30; // ecx
    int v31; // eax
    int v32; // ecx
    int m_list_contact_point_count; // eax
    int v34; // ecx
    int m_contact_point_count; // eax
    contact_manifold_mesh_point **v36; // eax
    const phys_vec3 *v37; // eax
    contact_manifold_mesh_point *v38; // ecx
    double v39; // st7
    double v40; // st7
    const phys_mat44 *v41; // eax
    const phys_vec3 *v42; // eax
    float *p_x; // ecx
    phys_contact_manifold *v44; // ecx
    phys_contact_manifold *v45; // edx
    int v46; // eax
    double v47; // st6
    contact_manifold_mesh_point **v48; // ecx
    contact_manifold_mesh_point ***v49; // eax
    double v50; // st7
    contact_manifold_mesh_point **m_list_isect_point; // edx
    double v52; // st7
    double v53; // st6
    contact_manifold_mesh_point *v54; // edx
    double v55; // st5
    contact_manifold_mesh_point **v56; // edx
    contact_manifold_mesh_point **v57; // eax
    contact_manifold_mesh_point *v58; // eax
    double v59; // st7
    contact_manifold_mesh_point **v60; // eax
    contact_manifold_mesh_point **v61; // ecx
    float **v62; // eax
    contact_manifold_mesh_point **v63; // edx
    contact_manifold_mesh_point **v64; // edx
    contact_manifold_mesh_point **v65; // ecx
    contact_manifold_mesh_point *v66; // edx
    float *v67; // eax
    contact_manifold_mesh_point *v68; // ecx
    double v69; // st7
    phys_vec2 *p_m_contact_p; // edx
    double v71; // st7
    contact_manifold_mesh_point **v72; // eax
    contact_point_info *cpi; // eax
    contact_manifold_mesh_point **m_list_b1_r_loc; // edx
    int v75; // ecx
    contact_manifold_mesh_point *v76; // eax
    phys_contact_manifold *v77; // eax
    double v78; // st6
    double v79; // st6
    double v80; // st2
    const phys_vec3 *v81; // eax
    double v82; // st7
    contact_manifold_mesh_point **v83; // eax
    double v84; // st5
    double v85; // st4
    double v86; // st2
    const phys_vec3 *v87; // eax
    double v88; // st7
    double v89; // st7
    contact_manifold_mesh_point **v90; // ecx
    contact_manifold_mesh_point **v91; // eax
    double v92; // st7
    const phys_vec3 *v93; // eax
    phys_contact_manifold *m_cpi; // ecx
    double v95; // st7
    contact_point_info *v96; // ecx
    contact_point_info *v97; // ecx
    bool v98; // zf
    rigid_body *v99; // ecx
    rigid_body *m_rb; // eax
    contact_point_info *v101; // edx
    const rigid_body_pair_key *v102; // eax
    rigid_body_constraint_contact *v103; // eax
    contact_point_info *v104; // ecx
    phys_vec3 *v105; // edx
    phys_vec3 *m_list_b2_r_loc; // edi
    double v107; // st7
    const contact_point_info *m_first; // eax
    int v110; // [esp+10h] [ebp-158h]
    phys_vec3 v111; // [esp+1Ch] [ebp-14Ch] BYREF
    phys_vec3 v112; // [esp+2Ch] [ebp-13Ch] BYREF
    phys_vec3 v113; // [esp+3Ch] [ebp-12Ch] BYREF
    float v114; // [esp+4Ch] [ebp-11Ch]
    float v115; // [esp+50h] [ebp-118h]
    float v116; // [esp+54h] [ebp-114h]
    phys_vec3 *v117; // [esp+60h] [ebp-108h]
    float n2; // [esp+64h] [ebp-104h]
    float n1; // [esp+68h] [ebp-100h]
    phys_vec3 v120; // [esp+6Ch] [ebp-FCh] BYREF
    float dist_p1_p2_n; // [esp+88h] [ebp-E0h]
    phys_vec3 ip_3d_2; // [esp+8Ch] [ebp-DCh]
    const phys_mat44 *cg1_to_rb1_xform; // [esp+A8h] [ebp-C0h]
    float v124; // [esp+ACh] [ebp-BCh]
    float v125; // [esp+B0h] [ebp-B8h]
    float v126; // [esp+B4h] [ebp-B4h]
    float v127; // [esp+B8h] [ebp-B0h]
    contact_manifold_mesh_point **last_ip_i; // [esp+C8h] [ebp-A0h]
    phys_vec3 v129; // [esp+CCh] [ebp-9Ch] BYREF
    contact_manifold_mesh_point **last_mp_i; // [esp+E8h] [ebp-80h]
    phys_vec3 p2_displaced; // [esp+ECh] [ebp-7Ch] BYREF
    phys_vec3 cg1_relative_translation_loc; // [esp+FCh] [ebp-6Ch]
    phys_vec3 ip_3d_; // [esp+10Ch] [ebp-5Ch] BYREF
    float v134; // [esp+124h] [ebp-44h]
    contact_manifold_mesh_point *closest_mp; // [esp+128h] [ebp-40h]
    contact_manifold_mesh_point **MAX_MP_I; // [esp+12Ch] [ebp-3Ch]
    int v137; // [esp+130h] [ebp-38h]
    contact_manifold_mesh_point **cur_mp_i; // [esp+134h] [ebp-34h]
    rigid_body *rb1; // [esp+138h] [ebp-30h]
    float d1; // [esp+13Ch] [ebp-2Ch]
    float d2; // [esp+140h] [ebp-28h]
    //phys_contact_manifold *p_cman1; // [esp+144h] [ebp-24h]
    phys_contact_manifold *sin_feautre_angular_eps_sq; // [esp+148h] [ebp-20h]
    contact_manifold_mesh_point **smallest_area_mp_i; // [esp+14Ch] [ebp-1Ch]
    float dist; // [esp+150h] [ebp-18h]
    contact_manifold_mesh_point **next_mp_i; // [esp+154h] [ebp-14h]
    contact_manifold_mesh_point **next_next_mp_i; // [esp+158h] [ebp-10h]
    //_UNKNOWN *v148[2]; // [esp+15Ch] [ebp-Ch] BYREF
    //phys_gjk_info *gjk_infoa; // [esp+164h] [ebp-4h] BYREF
    //int vars0; // [esp+168h] [ebp+0h]
    //
    //*(float *)v148 = a2;
    //v148[1] = (_UNKNOWN *)vars0;
    //v110 = a4;
    //last_ip_i = (contact_manifold_mesh_point **)this;


    this->cman1.m_list_mesh_point = 0;
    this->cman1.m_list_sorted_mesh_point = 0;
    this->cman1.m_list_contact_point = 0;
    this->cman2.m_list_mesh_point = 0;
    this->cman2.m_list_sorted_mesh_point = 0;
    this->cman2.m_list_contact_point = 0;
    //p_cman1 = &this->cman1;
    this->m_list_isect_point = 0;
    sin_feautre_angular_eps_sq = &this->cman2;
    this->m_allocator.m_buffer_cur = this->m_allocator.m_buffer_start;
    m_bpi1 = pcp->m_bpi1;
    m_cg_to_world_xform = (rigid_body *)m_bpi1->m_cg_to_world_xform;
    m_cg_to_rb_xform = m_bpi1->m_cg_to_rb_xform;
    this->m_cpi = 0;
    m_rb_to_world_xform = pcp->m_bpi2->m_rb_to_world_xform;
    cg1_to_rb1_xform = m_cg_to_rb_xform;
    rb1 = m_cg_to_world_xform;
    //LODWORD(v134) = &this->cg1_to_rb2_xform;
    phys_full_inv_multiply_mat(
        &this->cg1_to_rb2_xform,
        m_rb_to_world_xform,
        (const phys_mat44 *)m_cg_to_world_xform);
    p_m_n = &gjk_info->cg1_cinfo_loc.m_n;
    v12 = gjk_info->cg1_cinfo_loc.m_n.x * 0.3400000035762787;
    ip_3d_.x = gjk_info->cg1_cinfo_loc.m_p1.x;
    y = gjk_info->cg1_cinfo_loc.m_p1.y;
    cg1_relative_translation_loc.x = v12;
    ip_3d_.y = y;
    v14 = gjk_info->cg1_cinfo_loc.m_n.y * 0.3400000035762787;
    ip_3d_.z = gjk_info->cg1_cinfo_loc.m_p1.z;
    ip_3d_.w = gjk_info->cg1_cinfo_loc.m_p1.w;
    x = gjk_info->cg1_cinfo_loc.m_p2.x;
    cg1_relative_translation_loc.y = v14;
    v16 = 0.3400000035762787 * gjk_info->cg1_cinfo_loc.m_n.z;
    p2_displaced.x = x;
    v17 = gjk_info->cg1_cinfo_loc.m_p2.y;
    cg1_relative_translation_loc.z = v16;
    p2_displaced.y = v17;
    v18 = gjk_info->cg1_cinfo_loc.m_p1.x;
    p2_displaced.z = gjk_info->cg1_cinfo_loc.m_p2.z;
    w = gjk_info->cg1_cinfo_loc.m_p2.w;
    v117 = &gjk_info->cg1_cinfo_loc.m_n;
    v20 = cg1_relative_translation_loc.x;
    p2_displaced.w = w;
    gjk_info->cg1_cinfo_loc.m_p1.x = v18 + cg1_relative_translation_loc.x;
    v21 = cg1_relative_translation_loc.y;
    gjk_info->cg1_cinfo_loc.m_p1.y = cg1_relative_translation_loc.y + gjk_info->cg1_cinfo_loc.m_p1.y;
    z = cg1_relative_translation_loc.z;
    gjk_info->cg1_cinfo_loc.m_p1.z = gjk_info->cg1_cinfo_loc.m_p1.z + cg1_relative_translation_loc.z;
    gjk_info->cg1_cinfo_loc.m_p2.x = gjk_info->cg1_cinfo_loc.m_p2.x - v20;
    gjk_info->cg1_cinfo_loc.m_p2.y = gjk_info->cg1_cinfo_loc.m_p2.y - v21;
    gjk_info->cg1_cinfo_loc.m_p2.z = gjk_info->cg1_cinfo_loc.m_p2.z - z;
    cg1_relative_translation_loc.x = ip_3d_.x - p2_displaced.x;
    cg1_relative_translation_loc.y = ip_3d_.y - p2_displaced.y;
    cg1_relative_translation_loc.z = ip_3d_.z - p2_displaced.z;
    dist_p1_p2_n = gjk_info->cg1_cinfo_loc.m_n.y * cg1_relative_translation_loc.y
        + gjk_info->cg1_cinfo_loc.m_n.x * cg1_relative_translation_loc.x
        + gjk_info->cg1_cinfo_loc.m_n.z * cg1_relative_translation_loc.z;
    *(float *)&next_next_mp_i = dist_p1_p2_n / -10.20000076293945;
    v23 = 0.0;
    if (*(float *)&next_next_mp_i >= 0.0)
    {
        v23 = 1.0;
        if (*(float *)&next_next_mp_i <= 1.0)
            v23 = *(float *)&next_next_mp_i;
    }
    *(float *)&next_mp_i = v23;
    d1 = phys_contact_manifold::get_STD_GET_FEATURE_DISTANCE_EPS(*(float *)&next_mp_i);
    d2 = phys_contact_manifold::get_STD_GET_FEATURE_SIN_ANGULAR_EPS_SQ(*(float *)&next_mp_i);
    //phys_contact_manifold::set_get_feature_params(p_cman1, &gjk_info->cg1_cinfo_loc.m_p1, p_m_n, d1, d2);
    this->cman1.set_get_feature_params(&gjk_info->cg1_cinfo_loc.m_p1, p_m_n, d1, d2);

    pcp->m_bpi1->m_gjk_geom->get_feature(&this->cman1);
    //((void(__thiscall *)(const phys_gjk_geom *, phys_contact_manifold *, int, int))pcp->m_bpi1->m_gjk_geom->get_feature)(
    //    pcp->m_bpi1->m_gjk_geom,
    //    p_cman1,
    //    a3,
    //    v110);

    cg1_relative_translation_loc.x = -p_m_n->x;
    cg1_relative_translation_loc.y = -gjk_info->cg1_cinfo_loc.m_n.y;
    cg1_relative_translation_loc.z = -gjk_info->cg1_cinfo_loc.m_n.z;
    v129.x = gjk_info->cg2_to_cg1_xform.x.z * cg1_relative_translation_loc.z
        + gjk_info->cg2_to_cg1_xform.x.y * cg1_relative_translation_loc.y
        + cg1_relative_translation_loc.x * gjk_info->cg2_to_cg1_xform.x.x;
    v129.y = gjk_info->cg2_to_cg1_xform.y.y * cg1_relative_translation_loc.y
        + cg1_relative_translation_loc.x * gjk_info->cg2_to_cg1_xform.y.x
        + gjk_info->cg2_to_cg1_xform.y.z * cg1_relative_translation_loc.z;
    v129.z = cg1_relative_translation_loc.z * gjk_info->cg2_to_cg1_xform.z.z
        + cg1_relative_translation_loc.y * gjk_info->cg2_to_cg1_xform.z.y
        + cg1_relative_translation_loc.x * gjk_info->cg2_to_cg1_xform.z.x;

    *(float *)&next_mp_i = d2;
    dist = d1;
    smallest_area_mp_i = (contact_manifold_mesh_point **)&v129;
    v25 = phys_full_inv_multiply(&v120, &gjk_info->cg2_to_cg1_xform, &gjk_info->cg1_cinfo_loc.m_p2);

    //phys_contact_manifold::set_get_feature_params(
    //    sin_feautre_angular_eps_sq,
    //    v25,
    //    (const phys_vec3 *)smallest_area_mp_i,
    //    dist,
    //    *(float *)&next_mp_i);

    sin_feautre_angular_eps_sq->set_get_feature_params(v25, &v129, dist, d2);

    pcp->m_bpi2->m_gjk_geom->get_feature(sin_feautre_angular_eps_sq);
    next_next_mp_i = (contact_manifold_mesh_point **)LODWORD(gjk_info->m_continuous_collision_lambda);
    cg1_relative_translation_loc.x = *(float *)&next_next_mp_i * gjk_info->m_cg1_relative_translation_loc.x;
    cg1_relative_translation_loc.y = gjk_info->m_cg1_relative_translation_loc.y * *(float *)&next_next_mp_i;
    m_list_mesh_point_count = this->cman1.m_list_mesh_point_count;
    cg1_relative_translation_loc.z = *(float *)&next_next_mp_i * gjk_info->m_cg1_relative_translation_loc.z;
    if (m_list_mesh_point_count < 2)
        goto LABEL_17;
    v27 = this->cman2.m_list_mesh_point_count;
    if (v27 < 2 || m_list_mesh_point_count == 2 && v27 == 2)
        goto LABEL_17;
    d2 = phys_contact_manifold::get_STD_COMP_FEATURE_NORMAL_DISTANCE_EPS(*(float *)&next_mp_i);
    STD_COMP_FEATURE_NORMAL_SIN_ANGULAR_EPS_SQ = phys_contact_manifold::get_STD_COMP_FEATURE_NORMAL_SIN_ANGULAR_EPS_SQ(*(float *)&next_mp_i);
    v29 = &this->cman1;
    d1 = STD_COMP_FEATURE_NORMAL_SIN_ANGULAR_EPS_SQ;
    this->cman1.m_feature_distance_eps = d2;
    v29->m_sin_feautre_angular_eps_sq = d1;
    //phys_contact_manifold::comp_feature_normal(v29);
    v29->comp_feature_normal();
    v30 = sin_feautre_angular_eps_sq;
    sin_feautre_angular_eps_sq->m_feature_distance_eps = d2;
    v30->m_sin_feautre_angular_eps_sq = d1;
    //phys_contact_manifold::comp_feature_normal(v30);
    v30->comp_feature_normal();
    v31 = this->cman1.m_list_mesh_point_count;
    if (v31 < 2)
        goto LABEL_17;
    v32 = this->cman2.m_list_mesh_point_count;
    if (v32 < 2 || v31 == 2 && v32 == 2)
        goto LABEL_17;
    //phys_contact_manifold_process::comp_contact_mat(v6, p_m_n);
    this->comp_contact_mat(p_m_n);
    //phys_contact_manifold::generate_convex_poly(p_cman1, &v6->contact_mat);
    this->cman1.generate_convex_poly(&this->contact_mat);
    v129.x = -cg1_relative_translation_loc.x;
    v129.y = -cg1_relative_translation_loc.y;
    v129.z = -cg1_relative_translation_loc.z;

    sin_feautre_angular_eps_sq->xform_and_translate_mesh_points(&gjk_info->cg2_to_cg1_xform, &v129);
    //phys_contact_manifold::xform_and_translate_mesh_points(
    //    sin_feautre_angular_eps_sq,
    //    (int)v148,
    //    &gjk_info->cg2_to_cg1_xform,
    //    &v129);

    //phys_contact_manifold::generate_convex_poly(sin_feautre_angular_eps_sq, &v6->contact_mat);
    sin_feautre_angular_eps_sq->generate_convex_poly(&this->contact_mat);

    m_list_contact_point_count = this->cman1.m_list_contact_point_count;
    if (m_list_contact_point_count < 2)
        goto LABEL_17;
    v34 = this->cman2.m_list_contact_point_count;
    if (v34 < 2 || m_list_contact_point_count == 2 && v34 == 2)
        goto LABEL_17;
    //phys_contact_manifold_process::intersect_poly_poly(v6);
    this->intersect_poly_poly();
    m_contact_point_count = this->m_contact_point_count;
    if (!m_contact_point_count)
    {
    LABEL_17:
        //*(float *)&v36 = COERCE_FLOAT(contact_point_info::create_cpi(1, 0, v6->m_cpi_allocator));
        //next_next_mp_i = v36;
        //v6->m_cpi = (contact_point_info *)v36;
        this->m_cpi = (contact_point_info *)contact_point_info::create_cpi(1, 0, this->m_cpi_allocator);
        //if (*(float *)&v36 == 0.0)
        if (!this->m_cpi)
            return;
        //v37 = phys_full_multiply(&v120, cg1_to_rb1_xform, &ip_3d_);
        //v38 = next_next_mp_i[9];
        //v38->m_p.x = v37->x;
        //v39 = v37->y;
        //sin_feautre_angular_eps_sq = (phys_contact_manifold *)&p2_displaced;
        //v38->m_p.y = v39;
        //v40 = v37->z;
        //v41 = (const phys_mat44 *)LODWORD(v134);
        //v38->m_p.z = v40;
        //v42 = phys_full_multiply(&v120, v41, &sin_feautre_angular_eps_sq->m_feature_normal);
        //p_x = &v6->m_cpi->m_list_b2_r_loc->x;
        //*p_x = v42->x;
        //p_x[1] = v42->y;
        //p_x[2] = v42->z;
        //v6->m_contact_point_count = 1;
        //goto LABEL_69;

        // contact point on body1
        phys_vec3 *p1 = this->m_cpi->m_list_b1_r_loc;
        const phys_vec3 *r1 = phys_full_multiply(&v120, cg1_to_rb1_xform, &ip_3d_);

        p1->x = r1->x;
        p1->y = r1->y;
        p1->z = r1->z;

        // contact point on body2
        phys_vec3 *p2 = this->m_cpi->m_list_b2_r_loc;
        const phys_vec3 *r2 = phys_full_multiply(&v120, v41, &p2_displaced);

        p2->x = r2->x;
        p2->y = r2->y;
        p2->z = r2->z;

        this->m_contact_point_count = 1;
        goto LABEL_69;

    }
    if (m_contact_point_count <= 0
        && _tlAssert("source/phys_collision.cpp", 83, "m_contact_point_count > 0", ""))
    {
        __debugbreak();
    }
    if (!this->m_list_isect_point && _tlAssert("source/phys_collision.cpp", 84, "m_list_isect_point", ""))
        __debugbreak();
    v44 = &this->cman1;
    v45 = sin_feautre_angular_eps_sq;
    d1 = gjk_info->cg1_cinfo_loc.m_n.y * v44->m_feature_normal.y
        + v44->m_feature_normal.x * gjk_info->cg1_cinfo_loc.m_n.x
        + gjk_info->cg1_cinfo_loc.m_n.z * v44->m_feature_normal.z;
    d2 = gjk_info->cg1_cinfo_loc.m_n.y * v45->m_feature_normal.y
        + gjk_info->cg1_cinfo_loc.m_n.x * v45->m_feature_normal.x
        + gjk_info->cg1_cinfo_loc.m_n.z * sin_feautre_angular_eps_sq->m_feature_normal.z;
    *(float *)&next_next_mp_i = fabs(d1);
    if (*(float *)&next_next_mp_i <= 0.0000001000000011686097)
    {
        if (_tlAssert("source/phys_collision.cpp", 88, "fabsf(d1) > 0.0000001f", ""))
            __debugbreak();
        v45 = sin_feautre_angular_eps_sq;
        v44 = &this->cman1;
    }
    *(float *)&next_next_mp_i = fabs(d2);
    if (*(float *)&next_next_mp_i <= 0.0000001000000011686097)
    {
        if (_tlAssert("source/phys_collision.cpp", 89, "fabsf(d2) > 0.0000001f", ""))
            __debugbreak();
        v45 = sin_feautre_angular_eps_sq;
        v44 = &this->cman1;
    }
    v46 = this->m_contact_point_count;
    n1 = v44->m_feature_normal.y * ip_3d_.y + v44->m_feature_normal.x * ip_3d_.x + v44->m_feature_normal.z * ip_3d_.z;
    n2 = v45->m_feature_normal.y * p2_displaced.y
        + v45->m_feature_normal.x * p2_displaced.x
        + v45->m_feature_normal.z * p2_displaced.z;
    if (v46 > 5)
    {
        closest_mp = 0;
        *(float *)&next_next_mp_i = 1.0 / d1;
        p2_displaced.x = v44->m_feature_normal.x * *(float *)&next_next_mp_i;
        p2_displaced.y = v44->m_feature_normal.y * *(float *)&next_next_mp_i;
        v47 = *(float *)&next_next_mp_i * v44->m_feature_normal.z;
        v48 = &this->m_list_isect_point[v46];
        v49 = (contact_manifold_mesh_point ***)(v48 - 2);
        p2_displaced.z = v47;
        last_mp_i = v48;
        *(float *)&next_next_mp_i = 1.0 / d2;
        ip_3d_.x = v45->m_feature_normal.x * *(float *)&next_next_mp_i;
        ip_3d_.y = v45->m_feature_normal.y * *(float *)&next_next_mp_i;
        v50 = *(float *)&next_next_mp_i * v45->m_feature_normal.z;
        cur_mp_i = v48 - 1;
        m_list_isect_point = this->m_list_isect_point;
        ip_3d_.z = v50;
        next_mp_i = m_list_isect_point;
        v124 = ip_3d_.x - p2_displaced.x;
        v125 = ip_3d_.y - p2_displaced.y;
        v126 = ip_3d_.z - p2_displaced.z;
        *(float *)&MAX_MP_I = this->contact_mat.x.z * v126 + this->contact_mat.x.y * v125 + v124 * this->contact_mat.x.x;
        *(float *)&v137 = v126 * this->contact_mat.y.z + v125 * this->contact_mat.y.y + v124 * this->contact_mat.y.x;
        *(float *)&next_next_mp_i = 10000000.0;
        if (m_list_isect_point == v48)
            goto LABEL_89;
        v52 = *(float *)&v137;
        v53 = *(float *)&MAX_MP_I;
        do
        {
            v54 = *cur_mp_i;
            smallest_area_mp_i = *v49;
            v55 = (*next_mp_i)->m_contact_p.x;
            LODWORD(dist) = (DWORD)&(*next_mp_i)->m_contact_p;
            ip_3d_2.z = v55 - *((float *)smallest_area_mp_i + 4);
            ip_3d_2.w = *(float *)(LODWORD(dist) + 4) - *((float *)smallest_area_mp_i + 5);
            *(float *)&MAX_MP_I = v54->m_contact_p.x - *((float *)smallest_area_mp_i + 4);
            *(float *)&v137 = v54->m_contact_p.y - *((float *)smallest_area_mp_i + 5);
            dist = ip_3d_2.w * *(float *)&MAX_MP_I - *(float *)&v137 * ip_3d_2.z;
            dist = fabs(dist);
            v54->m_p.x = dist;
            dist = v54->m_contact_p.y * v52 + v53 * v54->m_contact_p.x;
            if (*(float *)&next_next_mp_i > (double)dist)
            {
                *(float *)&next_next_mp_i = dist;
                closest_mp = v54;
            }
            v49 = (contact_manifold_mesh_point ***)cur_mp_i;
            cur_mp_i = next_mp_i++;
        } while (next_mp_i != v48);
        if (!closest_mp)
        {
        LABEL_89:
            if (_tlAssert("source/phys_collision.cpp", 115, "closest_mp", ""))
                __debugbreak();
            v48 = last_mp_i;
        }
        v137 = (int)(this->m_list_isect_point + 5);
        while ((unsigned int)v48 > v137)
        {
            v56 = this->m_list_isect_point;
            v57 = v56 + 1;
            smallest_area_mp_i = v56;
            for (cur_mp_i = v56 + 1; v57 != v48; cur_mp_i = v57)
            {
                v58 = *v57;
                v56 = smallest_area_mp_i;
                if (v58 == closest_mp)
                {
                    v60 = cur_mp_i;
                }
                else
                {
                    v59 = v58->m_p.x;
                    v60 = cur_mp_i;
                    if ((*smallest_area_mp_i)->m_p.x > v59)
                    {
                        v56 = cur_mp_i;
                        smallest_area_mp_i = cur_mp_i;
                    }
                }
                v57 = v60 + 1;
            }
            v61 = v48 - 1;
            last_mp_i = v61;
            v62 = (float **)v61;
            if (v56 > this->m_list_isect_point)
                v62 = (float **)(v56 - 1);
            if (v56 < v61)
                v63 = v56 + 1;
            else
                v63 = this->m_list_isect_point;
            next_mp_i = v63;
            v64 = v61;
            if ((contact_manifold_mesh_point **)v62 > this->m_list_isect_point)
                v64 = (contact_manifold_mesh_point **)(v62 - 1);
            if (next_mp_i < v61)
                v65 = next_mp_i + 1;
            else
                v65 = this->m_list_isect_point;
            v66 = *v64;
            v67 = *v62;
            next_next_mp_i = v65;
            v68 = *next_mp_i;
            v126 = (*next_mp_i)->m_contact_p.x - v66->m_contact_p.x;
            v127 = v68->m_contact_p.y - v66->m_contact_p.y;
            ip_3d_2.z = v67[4] - v66->m_contact_p.x;
            v69 = v67[5] - v66->m_contact_p.y;
            p_m_contact_p = &(*next_next_mp_i)->m_contact_p;
            ip_3d_2.w = v69;
            dist = v127 * ip_3d_2.z - ip_3d_2.w * v126;
            dist = fabs(dist);
            *v67 = dist;
            ip_3d_.z = p_m_contact_p->x - v67[4];
            ip_3d_.w = p_m_contact_p->y - v67[5];
            p2_displaced.z = v68->m_contact_p.x - v67[4];
            v71 = v68->m_contact_p.y - v67[5];
            v72 = smallest_area_mp_i;
            p2_displaced.w = v71;
            dist = ip_3d_.w * p2_displaced.z - p2_displaced.w * ip_3d_.z;
            dist = fabs(dist);
            v68->m_p.x = dist;
            v48 = last_mp_i;
            if (v72 < last_mp_i)
            {
                memcpy(v72, v72 + 1, 4 * (((unsigned int)((char *)last_mp_i - (char *)v72 - 1) >> 2) + 1));
                p_m_n = v117;
                v48 = last_mp_i;
                //v6 = (phys_contact_manifold_process *)last_ip_i;
            }
        }
        this->m_contact_point_count = 5;
    }
    cpi = contact_point_info::create_cpi(this->m_contact_point_count, 0, this->m_cpi_allocator);
    this->m_cpi = cpi;
    if (cpi)
    {
        m_list_b1_r_loc = (contact_manifold_mesh_point **)cpi->m_list_b1_r_loc;
        v75 = this->m_contact_point_count;
        next_next_mp_i = (contact_manifold_mesh_point **)cpi->m_list_b2_r_loc;
        v76 = (contact_manifold_mesh_point *)this->m_list_isect_point;
        smallest_area_mp_i = m_list_b1_r_loc;
        closest_mp = v76;
        last_ip_i = (contact_manifold_mesh_point **)(&v76->m_p.x + v75);
        if (v76 != (contact_manifold_mesh_point *)last_ip_i)
        {
            while (1)
            {
                phys_v2_to_v3_multiply(&ip_3d_, &this->contact_mat, (const phys_vec2 *)(LODWORD(v76->m_p.x) + 16));
                v77 = &this->cman1;
                v78 = this->cman1.m_feature_normal.x;
                sin_feautre_angular_eps_sq = (phys_contact_manifold *)&v113;
                //p_cman1 = (phys_contact_manifold *)cg1_to_rb1_xform;
                v79 = v78 * ip_3d_.x + ip_3d_.y * v77->m_feature_normal.y;
                v80 = ip_3d_.z * v77->m_feature_normal.z;
                //d2 = COERCE_FLOAT(&v111);
                *(float *)&v137 = v79 + v80;
                *(float *)&v137 = (n1 - *(float *)&v137) / d1;
                p2_displaced.x = *(float *)&v137 * p_m_n->x;
                p2_displaced.y = p_m_n->y * *(float *)&v137;
                p2_displaced.z = *(float *)&v137 * p_m_n->z;
                v113.x = ip_3d_.x + p2_displaced.x;
                v113.y = ip_3d_.y + p2_displaced.y;
                v113.z = ip_3d_.z + p2_displaced.z;
                v81 = phys_multiply(&v111, cg1_to_rb1_xform, &v113);
                v124 = v81->x + cg1_to_rb1_xform->w.x;
                v125 = v81->y + cg1_to_rb1_xform->w.y;
                v82 = v81->z;
                v83 = smallest_area_mp_i;
                v126 = v82 + cg1_to_rb1_xform->w.z;
                *(float *)smallest_area_mp_i = v124;
                *((float *)v83 + 1) = v125;
                *((float *)v83 + 2) = v126;
                ip_3d_2.x = ip_3d_.x + cg1_relative_translation_loc.x;
                ip_3d_2.y = ip_3d_.y + cg1_relative_translation_loc.y;
                ip_3d_2.z = ip_3d_.z + cg1_relative_translation_loc.z;
                v84 = ip_3d_2.y * sin_feautre_angular_eps_sq->m_feature_normal.y;
                v85 = sin_feautre_angular_eps_sq->m_feature_normal.x;
                //d1 = COERCE_FLOAT(&v120);
                //*(float *)&rb1 = v134;
                v86 = ip_3d_2.z * sin_feautre_angular_eps_sq->m_feature_normal.z;
                cur_mp_i = (contact_manifold_mesh_point **)&v112;
                *(float *)&v137 = v85 * ip_3d_2.x + v84 + v86;
                *(float *)&v137 = (n2 - *(float *)&v137) / d2;
                v114 = *(float *)&v137 * p_m_n->x;
                v115 = p_m_n->y * *(float *)&v137;
                v116 = *(float *)&v137 * p_m_n->z;
                v120.x = ip_3d_2.x + v114;
                v120.y = ip_3d_2.y + v115;
                v120.z = ip_3d_2.z + v116;
                v87 = phys_multiply(&v112, (const phys_mat44 *)LODWORD(v134), &v120);
                v88 = v87->x + *(float *)(LODWORD(v134) + 48);
                smallest_area_mp_i += 4;
                v129.x = v88;
                v129.y = v87->y + *(float *)(LODWORD(v134) + 52);
                v89 = v87->z + *(float *)(LODWORD(v134) + 56);
                v91 = next_next_mp_i + 4;
                v129.z = v89;
                closest_mp = (contact_manifold_mesh_point *)((char *)closest_mp + 4);
                v90 = (contact_manifold_mesh_point **)closest_mp;
                next_next_mp_i = v91;
                *(v91 - 4) = (contact_manifold_mesh_point *)LODWORD(v129.x);
                *(v91 - 3) = (contact_manifold_mesh_point *)LODWORD(v129.y);
                *(v91 - 2) = (contact_manifold_mesh_point *)LODWORD(v129.z);
                if (v90 == last_ip_i)
                    break;
                v76 = closest_mp;
            }
        }
    LABEL_69:
        if (!this->m_cpi && _tlAssert("source/phys_collision.cpp", 155, "m_cpi", ""))
            __debugbreak();
        v120.x = -p_m_n->x;
        //*(float *)&next_mp_i = COERCE_FLOAT(&v120);
        v92 = p_m_n->y;
        dist = *(float *)&rb1;
        v120.y = -v92;
        smallest_area_mp_i = (contact_manifold_mesh_point **)&v112;
        v120.z = -p_m_n->z;
        v93 = phys_multiply(&v112, (const phys_mat44 *)rb1, &v120);
        m_cpi = (phys_contact_manifold *)this->m_cpi;
        m_cpi->m_feature_normal.x = v93->x;
        sin_feautre_angular_eps_sq = m_cpi;
        m_cpi->m_feature_normal.y = v93->y;
        m_cpi->m_feature_normal.z = v93->z;
        PHYS_ASSERT_UNIT(&sin_feautre_angular_eps_sq->m_feature_normal);
        *(float *)&rb1 = p_m_n->y * gjk_info->m_cg1_relative_translation_loc.y
            + p_m_n->x * gjk_info->m_cg1_relative_translation_loc.x
            + p_m_n->z * gjk_info->m_cg1_relative_translation_loc.z;
        if (*(float *)&rb1 >= -0.17)
        {
            this->m_cpi->m_translation_lambda = 1.0;
        }
        else
        {
            *(float *)&rb1 = -dist_p1_p2_n / *(float *)&rb1;
            v95 = 0.0;
            if (*(float *)&rb1 < 0.0 || (v95 = *(float *)&rb1, *(float *)&rb1 <= 1.0))
            {
                v97 = this->m_cpi;
                v134 = v95;
                v97->m_translation_lambda = v134;
            }
            else
            {
                v96 = this->m_cpi;
                v134 = 1.0;
                v96->m_translation_lambda = 1.0;
            }
        }
        set_cpi_params(this->m_cpi, pcp);
        v98 = this->m_list_cpi.m_last_next_ptr == 0;
        rb1 = (rigid_body *)this->m_cpi;
        if (v98
            && _tlAssert(
                "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
                230,
                "m_last_next_ptr",
                ""))
        {
            __debugbreak();
        }
        v99 = rb1;
        rb1->m_mat.x.x = 0.0;
        ++this->m_list_cpi.m_alloc_count;
        *this->m_list_cpi.m_last_next_ptr = (contact_point_info *)v99;
        this->m_list_cpi.m_last_next_ptr = (contact_point_info **)&v99->m_mat;
        m_rb = pcp->m_bpi1->m_rb;
        v101 = this->m_cpi;
        next_mp_i = (contact_manifold_mesh_point **)pcp->m_bpi2->m_rb;
        dist = *(float *)&m_rb;
        rb1 = m_rb;
        v101->m_pcp = pcp;

        //v102 = rigid_body_pair_key::rigid_body_pair_key((rigid_body_pair_key *)&ip_3d_.z, (rigid_body *const)LODWORD(dist), (rigid_body *const)next_mp_i);
        rigid_body_pair_key fk(rigid_body_pair_key((rigid_body *)LODWORD(dist), (rigid_body *)next_mp_i));
        v102 = &fk;

        //v103 = avl_tree_find<rigid_body_pair_key, rigid_body_constraint_contact, rigid_body_constraint_contact::avl_tree_accessor>(v6->m_rbc_contact_search_tree_root, v102);
        v103 = avl_tree_find(this->m_rbc_contact_search_tree_root, v102);

        this->m_cpi->m_rbc_contact = v103;
        if (v103)
        {
            if (v103->b1 != rb1)
            {
                v104 = this->m_cpi;
                v105 = v104->m_list_b1_r_loc;
                m_list_b2_r_loc = v104->m_list_b2_r_loc;
                v129.x = -v104->m_normal.x;
                v104->m_list_b1_r_loc = m_list_b2_r_loc;
                v107 = v104->m_normal.y;
                v104->m_list_b2_r_loc = v105;
                v129.y = -v107;
                v129.z = -v104->m_normal.z;
                v104->m_normal.x = v129.x;
                v104->m_normal.y = v129.y;
                v104->m_normal.z = v129.z;
            }
            m_first = v103->m_list_contact_point_info_buffer_2.m_first;
        }
        else
        {
            m_first = 0;
        }
        //contact_point_info::set_closest_cached_psc(v6->m_cpi, m_first);
        this->m_cpi->set_closest_cached_psc((contact_point_info*)m_first);
    }
#else
    // Reset manifolds and contact lists
    cman1.m_list_mesh_point = nullptr;
    cman1.m_list_sorted_mesh_point = nullptr;
    cman1.m_list_contact_point = nullptr;
    cman2.m_list_mesh_point = nullptr;
    cman2.m_list_sorted_mesh_point = nullptr;
    cman2.m_list_contact_point = nullptr;
    m_list_isect_point = nullptr;
    m_cpi = nullptr;

    m_allocator.m_buffer_cur = m_allocator.m_buffer_start;

    rigid_body* rb1 = (rigid_body*)pcp->m_bpi1->m_cg_to_world_xform;
    const phys_mat44* cg1_to_rb1_xform = pcp->m_bpi1->m_cg_to_rb_xform;
    const phys_mat44* rb2_to_world_xform = pcp->m_bpi2->m_rb_to_world_xform;

    // Compute cg1 -> rb2 transform
    phys_full_inv_multiply_mat(&cg1_to_rb2_xform, rb2_to_world_xform, (const phys_mat44*)rb1);

    // Compute relative translation for contact point
    phys_vec3 cg1_relative_translation_loc;
    cg1_relative_translation_loc.x = gjk_info->cg1_cinfo_loc.m_n.x * 0.34f;
    cg1_relative_translation_loc.y = gjk_info->cg1_cinfo_loc.m_n.y * 0.34f;
    cg1_relative_translation_loc.z = gjk_info->cg1_cinfo_loc.m_n.z * 0.34f;

    phys_vec3 p2_displaced = gjk_info->cg1_cinfo_loc.m_p2;

    // Offset p1 by cg1_relative_translation_loc
    gjk_info->cg1_cinfo_loc.m_p1.x += cg1_relative_translation_loc.x;
    gjk_info->cg1_cinfo_loc.m_p1.y += cg1_relative_translation_loc.y;
    gjk_info->cg1_cinfo_loc.m_p1.z += cg1_relative_translation_loc.z;

    // Offset p2 in opposite direction
    gjk_info->cg1_cinfo_loc.m_p2.x -= cg1_relative_translation_loc.x;
    gjk_info->cg1_cinfo_loc.m_p2.y -= cg1_relative_translation_loc.y;
    gjk_info->cg1_cinfo_loc.m_p2.z -= cg1_relative_translation_loc.z;

    // Compute vector between displaced points
    cg1_relative_translation_loc.x = gjk_info->cg1_cinfo_loc.m_p1.x - p2_displaced.x;
    cg1_relative_translation_loc.y = gjk_info->cg1_cinfo_loc.m_p1.y - p2_displaced.y;
    cg1_relative_translation_loc.z = gjk_info->cg1_cinfo_loc.m_p1.z - p2_displaced.z;

    float dist_p1_p2_n = gjk_info->cg1_cinfo_loc.m_n.x * cg1_relative_translation_loc.x +
                          gjk_info->cg1_cinfo_loc.m_n.y * cg1_relative_translation_loc.y +
                          gjk_info->cg1_cinfo_loc.m_n.z * cg1_relative_translation_loc.z;

    float t = dist_p1_p2_n / -10.2f;
    t = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);

    // Set feature parameters in manifolds
    float d1 = phys_contact_manifold::get_STD_GET_FEATURE_DISTANCE_EPS(t);
    float d2 = phys_contact_manifold::get_STD_GET_FEATURE_SIN_ANGULAR_EPS_SQ(t);
    cman1.set_get_feature_params(&gjk_info->cg1_cinfo_loc.m_p1, &gjk_info->cg1_cinfo_loc.m_n, d1, d2);

    // Extract features from GJK geometry
    pcp->m_bpi1->m_gjk_geom->get_feature(&cman1);

    // Prepare second manifold
    phys_vec3 neg_n = {-gjk_info->cg1_cinfo_loc.m_n.x,
                       -gjk_info->cg1_cinfo_loc.m_n.y,
                       -gjk_info->cg1_cinfo_loc.m_n.z};

    phys_vec3 v129;
    v129.x = gjk_info->cg2_to_cg1_xform.x.x * neg_n.x + gjk_info->cg2_to_cg1_xform.x.y * neg_n.y +
              gjk_info->cg2_to_cg1_xform.x.z * neg_n.z;
    v129.y = gjk_info->cg2_to_cg1_xform.y.x * neg_n.x + gjk_info->cg2_to_cg1_xform.y.y * neg_n.y +
              gjk_info->cg2_to_cg1_xform.y.z * neg_n.z;
    v129.z = gjk_info->cg2_to_cg1_xform.z.x * neg_n.x + gjk_info->cg2_to_cg1_xform.z.y * neg_n.y +
              gjk_info->cg2_to_cg1_xform.z.z * neg_n.z;

    phys_vec3 v120;
    phys_vec3* p_feature_pos = phys_full_inv_multiply(&v120, &gjk_info->cg2_to_cg1_xform, &gjk_info->cg1_cinfo_loc.m_p2);
    cman2.set_get_feature_params(p_feature_pos, &v129, d1, d2);
    pcp->m_bpi2->m_gjk_geom->get_feature(&cman2);

    // Generate convex polys for both manifolds
    cman1.generate_convex_poly(&contact_mat);
    phys_vec3 translation = {-cg1_relative_translation_loc.x, -cg1_relative_translation_loc.y, -cg1_relative_translation_loc.z};
    cman2.xform_and_translate_mesh_points(&gjk_info->cg2_to_cg1_xform, &translation);
    cman2.generate_convex_poly(&contact_mat);

    phys_vec3 *p1;
    phys_vec3 *p2;
    // Fallback if not enough contact points
    if (m_contact_point_count == 0)
    {
        m_cpi = contact_point_info::create_cpi(1, false, m_cpi_allocator);
        if (!m_cpi)
            return;

        p1 = m_cpi->m_list_b1_r_loc;
        const phys_vec3* r1 = phys_full_multiply(&v120, cg1_to_rb1_xform, &gjk_info->cg1_cinfo_loc.m_p1);
        *p1 = *r1;

        p2 = m_cpi->m_list_b2_r_loc;
        const phys_vec3* r2 = phys_full_multiply(&v120, &cg1_to_rb2_xform, &p2_displaced);
        *p2 = *r2;

        m_contact_point_count = 1;
        return;
    }

    // Safety checks
    if (m_contact_point_count <= 0 && _tlAssert("source/phys_collision.cpp", 83, "m_contact_point_count > 0", ""))
        __debugbreak();

    if (!m_list_isect_point && _tlAssert("source/phys_collision.cpp", 84, "m_list_isect_point", ""))
        __debugbreak();

    // Feature normals
    phys_contact_manifold *cmanA = &this->cman1;
    phys_contact_manifold *cmanB = &this->cman2;

    // Dot products of GJK normal with feature normals
    d1 = gjk_info->cg1_cinfo_loc.m_n.dot(cmanA->m_feature_normal);
    d2 = gjk_info->cg1_cinfo_loc.m_n.dot(cmanB->m_feature_normal);

    // Ensure they are non-zero
    if (fabsf(d1) <= 1e-7f)
    {
        if (_tlAssert("source/phys_collision.cpp", 88, "fabsf(d1) > 0.0000001f", ""))
            __debugbreak();
    }

    if (fabsf(d2) <= 1e-7f)
    {
        if (_tlAssert("source/phys_collision.cpp", 89, "fabsf(d2) > 0.0000001f", ""))
            __debugbreak();
    }

    //phys_vec3 ip_3d_ = *p1; // body 1 contact position
    //// Compute projected distances along feature normals
    //float n1 = cmanA->m_feature_normal.dot(ip_3d_);
    //float n2 = cmanB->m_feature_normal.dot(p2_displaced);

    contact_manifold_mesh_point *first_mp = m_list_isect_point[0];

    // Convert its 2D contact coordinates to 3D
    phys_vec3 ip_3d_;
    phys_vec2 contact_pos_2d = { first_mp->m_contact_p.x, first_mp->m_contact_p.y };
    phys_v2_to_v3_multiply(&ip_3d_, &contact_mat, &contact_pos_2d);

    // Now project along feature normals
    float n1 = cmanA->m_feature_normal.dot(ip_3d_);
    float n2 = cmanB->m_feature_normal.dot(p2_displaced);

    // If too many contact points, prune to 5
    if (m_contact_point_count > 5)
    {
        contact_manifold_mesh_point *closest_mp = nullptr;

        // Compute displacement along normals
        phys_vec3 dispA = cmanA->m_feature_normal / d1;
        phys_vec3 dispB = cmanB->m_feature_normal / d2;

        // Compute initial metric for contact points (using contact_mat as weight)
        phys_vec3 delta = dispB - dispA;
        float metricX = contact_mat.x.dot(delta);   // dot with contact_mat row X
        float metricY = contact_mat.y.dot(delta);   // dot with contact_mat row Y
        float minMetric = 1e7f;

        // Find closest contact point according to metric
        for (int i = 0; i < m_contact_point_count; ++i)
        {
            contact_manifold_mesh_point *mp = m_list_isect_point[i];
            contact_manifold_mesh_point *prev_mp = (i > 0) ? m_list_isect_point[i - 1] : nullptr;

            // Area-like metric (2D cross product)
            float dx = mp->m_contact_p.x - prev_mp->m_contact_p.x;
            float dy = mp->m_contact_p.y - prev_mp->m_contact_p.y;
            float cross = fabsf(dy * (dispB.x - dispA.x) - dx * (dispB.y - dispA.y));

            mp->m_p.x = cross;

            float metric = mp->m_contact_p.y * metricY + mp->m_contact_p.x * metricX;
            if (metric < minMetric)
            {
                minMetric = metric;
                closest_mp = mp;
            }
        }

        if (!closest_mp && _tlAssert("source/phys_collision.cpp", 115, "closest_mp", ""))
            __debugbreak();

        // Reorder remaining points to keep the top 5
        std::sort(m_list_isect_point, m_list_isect_point + m_contact_point_count,
            [](const contact_manifold_mesh_point *a, const contact_manifold_mesh_point *b) {
                return a->m_p.x < b->m_p.x;
            });

        m_contact_point_count = 5;
    }

    // Create contact point info structure
    this->m_cpi = contact_point_info::create_cpi(this->m_contact_point_count, false, this->m_cpi_allocator);

    if (this->m_cpi)
    {
        auto *cpi = this->m_cpi;

        contact_manifold_mesh_point **b1_list = reinterpret_cast<contact_manifold_mesh_point **>(cpi->m_list_b1_r_loc);
        contact_manifold_mesh_point **b2_list = reinterpret_cast<contact_manifold_mesh_point **>(cpi->m_list_b2_r_loc);

        contact_manifold_mesh_point **src_points = this->m_list_isect_point;

        // Transform and copy each contact point
        for (int i = 0; i < m_contact_point_count; ++i)
        {
            contact_manifold_mesh_point *mp = src_points[i];

            // Convert 2D manifold coordinates to 3D using contact_mat
            phys_vec3 ip_3d;
            phys_v2_to_v3_multiply(&ip_3d, &this->contact_mat, &mp->m_contact_p);

            // Displacement along cman1 feature normal
            phys_vec3 disp1 = mp->m_p * ((ip_3d.dot(this->cman1.m_feature_normal) - n1) / d1);
            phys_vec3 world_pos1 = ip_3d + disp1;

            // Apply transformation to body 1
            phys_vec3 rb1_pos;
            phys_multiply(&rb1_pos, cg1_to_rb1_xform, &world_pos1);
            rb1_pos += cg1_to_rb1_xform->w; // translation
            //b1_list[i]->m_contact_p = rb1_pos;
            b1_list[i]->m_contact_p.x = rb1_pos.x;
            b1_list[i]->m_contact_p.y = rb1_pos.y;

            // Displacement along cman2 feature normal relative to body 2
            phys_vec3 ip2 = ip_3d + cg1_relative_translation_loc;
            phys_vec3 disp2 = mp->m_p * ((ip2.dot(this->cman2.m_feature_normal) - n2) / d2);
            phys_vec3 world_pos2 = ip2 + disp2;

            // Apply transformation to body 2
            phys_vec3 rb2_pos;
            phys_mat44 *cg2_to_rb2_xform = &pcp->m_bpi2->m_rb->m_mat;
            phys_multiply(&rb2_pos, cg2_to_rb2_xform, &world_pos2);
            rb2_pos += cg2_to_rb2_xform->w; // translation
            //b2_list[i]->m_contact_p = rb2_pos;
            b2_list[i]->m_contact_p.x = rb2_pos.x;
            b2_list[i]->m_contact_p.y = rb2_pos.y;
        }

        // Set the normal for the CPI (negated contact manifold normal transformed to body A space)
        phys_vec3 p_m_n = this->cman1.m_feature_normal;
        phys_vec3 negated_p = -p_m_n;
        phys_vec3 cpi_normal;
        phys_multiply(&cpi_normal, cg1_to_rb1_xform, &negated_p);
        cpi->m_normal = cpi_normal;
        PHYS_ASSERT_UNIT(&cpi->m_normal);

        // Compute translation lambda along GJK relative translation
        float denom = p_m_n.dot(gjk_info->m_cg1_relative_translation_loc);
        float lambda = -(dist_p1_p2_n) / denom;
        cpi->m_translation_lambda = (denom >= -0.17f) ? 1.0f : std::clamp(lambda, 0.0f, 1.0f);

        // Set CPI parameters from the physics pair
        set_cpi_params(cpi, pcp);

        // Add CPI to linked list
        if (!this->m_list_cpi.m_last_next_ptr && _tlAssert("phys_mem.h", 230, "m_last_next_ptr", ""))
            __debugbreak();

        *this->m_list_cpi.m_last_next_ptr = cpi;
        this->m_list_cpi.m_last_next_ptr = &cpi->m_next_link;
        ++this->m_list_cpi.m_alloc_count;

        // Set rigid bodies
        rigid_body *rbA = pcp->m_bpi1->m_rb;
        rigid_body *rbB = pcp->m_bpi2->m_rb;
        cpi->m_pcp = pcp;

        // Lookup contact in AVL tree
        rigid_body_pair_key key(rbA, rbB);
        cpi->m_rbc_contact = avl_tree_find(this->m_rbc_contact_search_tree_root, &key);

        // If found contact has bodies flipped, swap
        if (cpi->m_rbc_contact && cpi->m_rbc_contact->b1 != rbA)
        {
            std::swap(cpi->m_list_b1_r_loc, cpi->m_list_b2_r_loc);
            cpi->m_normal = -cpi->m_normal;
        }

        // Set closest cached PSC
        contact_point_info *closest_psc = cpi->m_rbc_contact ? cpi->m_rbc_contact->m_list_contact_point_info_buffer_2.m_first : nullptr;
        cpi->set_closest_cached_psc(closest_psc);
    }
#endif
}

const bpei_database_id *__cdecl get_database_id(bpei_database_id *result, gjk_physics_collision_visitor *collision_visitor)
{
    if (collision_visitor->cent)
    {
        result->m_id1 = (unsigned int)collision_visitor->cent;
        result->m_id2 = 0;
        return result;
    }
    else if (collision_visitor->glass)
    {
        result->m_id1 = (unsigned int)collision_visitor->glass;
        result->m_id2 = 0;
        return result;
    }
    else
    {
        if (!collision_visitor->dynEntDef
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp",
                418,
                0,
                "%s",
                "collision_visitor->dynEntDef"))
        {
            __debugbreak();
        }
        result->m_id1 = (unsigned int)collision_visitor->dynEntDef;
        result->m_id2 = 0;
        return result;
    }
}

phys_mat44 *__cdecl create_ent_mat(gjk_physics_collision_visitor *collision_visitor)
{
    phys_mat44 *v3; // [esp+0h] [ebp-A8h]
    float v4[9]; // [esp+8h] [ebp-A0h] BYREF
    DynEntityDrawType v5; // [esp+2Ch] [ebp-7Ch]
    const float *origin; // [esp+30h] [ebp-78h]
    float v7[9]; // [esp+38h] [ebp-70h] BYREF
    const float *inVector; // [esp+5Ch] [ebp-4Ch]
    float axis[9]; // [esp+64h] [ebp-44h] BYREF
    int v10; // [esp+88h] [ebp-20h]
    int v11; // [esp+8Ch] [ebp-1Ch]
    int v12; // [esp+90h] [ebp-18h]
    phys_mat44 *v13; // [esp+94h] [ebp-14h]
    DynEntityPose *dynEntPose; // [esp+98h] [ebp-10h]
    DynEntityDrawType drawType; // [esp+9Ch] [ebp-Ch]
    unsigned __int16 dynEntId; // [esp+A0h] [ebp-8h]
    phys_mat44 *ent_mat; // [esp+A4h] [ebp-4h]

    v12 = 4;
    v11 = 16;
    v10 = (int)collision_visitor->allocate(64, 16, 0);
    v13 = (phys_mat44 *)v10;
    if (v10)
        v3 = v13;
    else
        v3 = 0;
    ent_mat = v3;
    if (collision_visitor->cent)
    {
        inVector = collision_visitor->cent->pose.origin;
        AnglesToAxis(collision_visitor->cent->pose.angles, (float (*)[3])axis);
        Phys_AxisToNitrousMat((float (*)[3])axis, ent_mat);
        Phys_Vec3ToNitrousVec(inVector, &ent_mat->w);
    }
    else if (collision_visitor->glass)
    {
        origin = collision_visitor->glass->origin;
        AnglesToAxis(collision_visitor->glass->angles, (float (*)[3])v7);
        Phys_AxisToNitrousMat((float (*)[3])v7, ent_mat);
        Phys_Vec3ToNitrousVec(origin, &ent_mat->w);
    }
    else
    {
        if (!collision_visitor->dynEntDef
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp",
                361,
                0,
                "%s",
                "collision_visitor->dynEntDef"))
        {
            __debugbreak();
        }
        v5 = (DynEntityDrawType)(collision_visitor->dynEntDef->xModel == 0);
        drawType = v5;
        dynEntId = DynEnt_GetId(collision_visitor->dynEntDef, v5);
        dynEntPose = DynEnt_GetClientPose(dynEntId, v5);
        UnitQuatToAxis(dynEntPose->pose.quat, (float (*)[3])v4);
        Phys_AxisToNitrousMat((float (*)[3])v4, ent_mat);
        Phys_Vec3ToNitrousVec(dynEntPose->pose.origin, &ent_mat->w);
    }
    return ent_mat;
}

phys_auto_activate_callback *__cdecl create_ent_aac(gjk_physics_collision_visitor *collision_visitor)
{
    DynEntityDef *dynEntDef; // [esp+10h] [ebp-30h]
    //phys_auto_activate_callback_vtbl *cent; // [esp+18h] [ebp-28h]
    dynamic_ent_aa *v6; // [esp+28h] [ebp-18h]
    destructible_ent_aa *v7; // [esp+34h] [ebp-Ch]

    if (collision_visitor->cent)
    {
        if (collision_visitor->cent->destructible && (collision_visitor->bpeqi->env_collision_flags & 8) != 0)
        {
            v7 = (destructible_ent_aa *)collision_visitor->allocate(12, 4, 0);
            if (!v7)
                return 0;
            //cent = (phys_auto_activate_callback_vtbl *)collision_visitor->cent;
            //v7->__vftable = (destructible_ent_aa_vtbl *)&phys_auto_activate_callback::`vftable';
            //v7->__vftable = (destructible_ent_aa_vtbl *)&destructible_ent_aa::`vftable';
            new ((void *)v7) destructible_ent_aa();
            
            v7->m_has_auto_activated = 0;
            //v7->m_cent = (centity_s *)cent;
            return (dynamic_ent_aa *)v7;
        }
        else
        {
            return 0;
        }
    }
    else if (collision_visitor->glass)
    {
        return 0;
    }
    else
    {
        if (!collision_visitor->dynEntDef
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp",
                391,
                0,
                "%s",
                "collision_visitor->dynEntDef"))
        {
            __debugbreak();
        }
        v6 = (dynamic_ent_aa *)collision_visitor->allocate(12, 4, 0);
        if (!v6)
            return 0;
        dynEntDef = (DynEntityDef*)collision_visitor->dynEntDef;

        
        //v6->__vftable = (dynamic_ent_aa_vtbl *)&phys_auto_activate_callback::`vftable';
        //v6->__vftable = (dynamic_ent_aa_vtbl *)&dynamic_ent_aa::`vftable';
        new ((void *)v6) dynamic_ent_aa();
        v6->m_has_auto_activated = 0;
        v6->m_dynEntDef = dynEntDef;
        return v6;
    }
}

void create_entity_bpi(gjk_physics_collision_visitor *collision_visitor, int mask)
{
    const centity_s *entity; // eax
    phys_mat44 **v4; // [esp+30h] [ebp-14h]
    bpei_database_id database_id; // [esp+34h] [ebp-10h] BYREF
    broad_phase_environment_info *bpei; // [esp+3Ch] [ebp-8h]
    entity_bpi_header *ebpih; // [esp+40h] [ebp-4h]
    int savedregs; // [esp+44h] [ebp+0h] BYREF

    get_database_id(&database_id, collision_visitor);
    //bpei = bpei_database_t::get_bpei_mt(&G_BPM->g_bpei_database, database_id);
    bpei = G_BPM->g_bpei_database.get_bpei_mt(database_id);
    if (!bpei->m_data)
    {
        //minspec_mutex::Lock(&bpei->m_mutex);
        bpei->m_mutex.Lock();
        if (!bpei->m_data)
        {
            v4 = (phys_mat44 **)collision_visitor->allocate(8, 4, 0);
            //if (rigid_body::is_environment_rigid_body(collision_visitor->rb))
            if (collision_visitor->rb->is_environment_rigid_body())
                *v4 = create_ent_mat(collision_visitor);
            else
                *v4 = 0;
            v4[1] = (phys_mat44 *)create_ent_aac(collision_visitor);
            if (!v4
                && _tlAssert("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp", 456, "ebpih", ""))
            {
                __debugbreak();
            }
            bpei->m_data = v4;
        }
        //minspec_mutex::Unlock(&bpei->m_mutex);
        bpei->m_mutex.Unlock();
    }
    if (!bpei->m_data
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp", 458, 0, "%s", "bpei->m_data"))
    {
        __debugbreak();
    }
    ebpih = (entity_bpi_header *)bpei->m_data;
    //if (rigid_body::is_environment_rigid_body(collision_visitor->rb))
    if (collision_visitor->rb->is_environment_rigid_body())
    {
        if (collision_visitor->cg_to_world_xform
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp",
                463,
                0,
                "%s",
                "collision_visitor->cg_to_world_xform == NULL"))
        {
            __debugbreak();
        }
        if (collision_visitor->cg_to_rb_xform
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp",
                464,
                0,
                "%s",
                "collision_visitor->cg_to_rb_xform == NULL"))
        {
            __debugbreak();
        }
        if (!ebpih->m_mat
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp",
                465,
                0,
                "%s",
                "ebpih->m_mat"))
        {
            __debugbreak();
        }
        collision_visitor->cg_to_world_xform = ebpih->m_mat;
        collision_visitor->cg_to_rb_xform = ebpih->m_mat;
    }
    collision_visitor->auto_activate_callback = ebpih->m_aac;
    if (!collision_visitor->rb_to_world_xform
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp",
            472,
            0,
            "%s",
            "collision_visitor->rb_to_world_xform"))
    {
        __debugbreak();
    }
    if (!collision_visitor->cg_to_world_xform
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp",
            473,
            0,
            "%s",
            "collision_visitor->cg_to_world_xform"))
    {
        __debugbreak();
    }
    if (!collision_visitor->cg_to_rb_xform
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp",
            474,
            0,
            "%s",
            "collision_visitor->cg_to_rb_xform"))
    {
        __debugbreak();
    }
    entity = get_entity(collision_visitor);
    //gjk_physics_collision_visitor::set_local_query_info(collision_visitor, (int)&savedregs, entity);
    collision_visitor->set_local_query_info(entity);
    if (collision_visitor->cent)
    {
        create_gjk_geom(0, collision_visitor->cent, collision_visitor, 0, mask, 0, 1);
    }
    else if (collision_visitor->glass)
    {
        create_gjk_geom(collision_visitor->glass, collision_visitor, mask);
    }
    else
    {
        if (!collision_visitor->dynEntDef
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp",
                487,
                0,
                "%s",
                "collision_visitor->dynEntDef"))
        {
            __debugbreak();
        }
        create_gjk_geom(collision_visitor->dynEntDef, collision_visitor, mask);
    }
}

void __cdecl phys_aabb_add_hace(phys_vec3 *aabb_min, phys_vec3 *aabb_max)
{
    aabb_min->x = aabb_min->x - 0.50999999;
    aabb_min->y = aabb_min->y - 0.50999999;
    aabb_min->z = aabb_min->z - 0.50999999;
    aabb_max->x = aabb_max->x + 0.50999999;
    aabb_max->y = aabb_max->y + 0.50999999;
    aabb_max->z = aabb_max->z + 0.50999999;
}

void __cdecl set_bp_standard_query()
{
    G_BPM->g_broad_phase_terrain_query_callback = &g_standard_query;
}

void __cdecl debug_callback(void*)
{
    if (phys_debugCallback->current.enabled)
    {
        if (_tlAssert("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp", 796, "0", ""))
            __debugbreak();
    }
}

void __cdecl set_debug_callback()
{
    phys_set_debug_callback(debug_callback);
}

void broad_phase_info::set(
    rigid_body *rb,
    const phys_mat44 *rb_to_world_xform,
    const phys_mat44 *cg_to_world_xform,
    const phys_mat44 *cg_to_rb_xform,
    const phys_gjk_geom *gjk_geom,
    unsigned int gjk_geom_id,
    bool calc_cg_to_world_xform,
    int surface_type,
    void *user_data,
    unsigned int env_collision_flags)
{
    this->m_flags = 0;
    this->m_list_bpb_next = 0;
    this->m_sap_node = 0;
    this->m_flags |= 1u;
    this->m_rb = rb;
    this->m_rb_to_world_xform = rb_to_world_xform;
    this->m_cg_to_world_xform = cg_to_world_xform;
    this->m_cg_to_rb_xform = cg_to_rb_xform;
    this->m_gjk_geom = gjk_geom;
    this->m_gjk_geom_id = gjk_geom_id;
    if (calc_cg_to_world_xform)
        this->m_flags |= 0x200u;
    else
        this->m_flags &= ~0x200u;
    this->m_surface_type = surface_type;
    this->m_user_data = user_data;
    this->m_env_collision_flags = env_collision_flags;
    this->m_my_collision_type_flags = 0;
}

void broad_phase_info::collision_prolog()
{
    float y; // [esp+18h] [ebp-2Ch]
    float z; // [esp+1Ch] [ebp-28h]
    rigid_body *m_rb; // [esp+30h] [ebp-14h]

    if ((this->m_flags & 0x200) != 0)
        phys_full_multiply_mat((phys_mat44 *)this->m_cg_to_world_xform, this->m_rb_to_world_xform, this->m_cg_to_rb_xform);

    ((phys_gjk_geom *)this->m_gjk_geom)->calc_aabb(this->m_cg_to_world_xform, &this->m_rb->m_last_position, &this->m_trace_aabb_max_whace);

    phys_aabb_add_hace(&this->m_trace_aabb_min_whace, &this->m_trace_aabb_max_whace);
    m_rb = this->m_rb;
    y = m_rb->m_moved_vec.y;
    z = m_rb->m_moved_vec.z;
    this->m_trace_translation.x = m_rb->m_moved_vec.x;
    this->m_trace_translation.y = y;
    this->m_trace_translation.z = z;

    if ((this->m_rb->m_flags & 0x30) == 0)
        calc_largest_vel_sq(this);
}


void broad_phase_info::set_bpi_env(phys_auto_activate_callback *auto_activate_callback)
{
    if ((this->m_flags & 1) == 0
        && _tlAssert(
            "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\collision\\phys_broad_phase_base.h",
            163,
            "is_bpi()",
            "call broad_phase_info::set first."))
    {
        __debugbreak();
    }
    iassert(m_sap_node == NULL);
    this->m_flags = 0;
    this->m_sap_node = auto_activate_callback;
    this->m_flags |= 4u;
    if (auto_activate_callback)
        this->m_flags |= 0x80u;
    else
        this->m_flags &= ~0x80u;
}

generic_avl_map_node_t *__cdecl generic_avl_map_add(
    phys_inplace_avl_tree<unsigned int, generic_avl_map_node_t, generic_avl_map_node_t> *gam,
    void *data,
    unsigned int avl_key)
{
    //generic_avl_map_node_t *m_tree_root; // [esp+15Ch] [ebp-8h]
    generic_avl_map_node_t *gamn; // [esp+160h] [ebp-4h]

    //m_tree_root = gam->m_tree_root;
    //while (m_tree_root && avl_key != m_tree_root->m_avl_key)
    //{
    //    if (avl_key >= m_tree_root->m_avl_key)
    //        m_tree_root = m_tree_root->m_avl_tree_node.m_right;
    //    else
    //        m_tree_root = m_tree_root->m_avl_tree_node.m_left;
    //}
    //if (m_tree_root
    //    && !Assert_MyHandler(
    //        "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp",
    //        1243,
    //        0,
    //        "%s",
    //        "gam->find(avl_key) == NULL"))
    //{
    //    __debugbreak();
    //}
    // 

    iassert(gam->find(avl_key) == NULL);

    //gamn = phys_simple_allocator<generic_avl_map_node_t>::allocate(&g_generic_avl_map_node_allocator);
    gamn = g_generic_avl_map_node_allocator.allocate();

    iassert(gamn);

    gamn->m_data = data;
    //phys_inplace_avl_tree<unsigned int, generic_avl_map_node_t, generic_avl_map_node_t>::add(gam, &avl_key, gamn);
    gam->add(avl_key, gamn);
    return gamn;
}

void *__cdecl generic_avl_map_destroy(
    phys_inplace_avl_tree<unsigned int, generic_avl_map_node_t, generic_avl_map_node_t> *gam,
    unsigned int avl_key)
{
    generic_avl_map_node_t *m_tree_root; // [esp+144h] [ebp-Ch]
    void *data; // [esp+148h] [ebp-8h]

    m_tree_root = gam->m_tree_root;
    while (m_tree_root && avl_key != m_tree_root->m_avl_key)
    {
        if (avl_key >= m_tree_root->m_avl_key)
            m_tree_root = m_tree_root->m_avl_tree_node.m_right;
        else
            m_tree_root = m_tree_root->m_avl_tree_node.m_left;
    }
    if (!m_tree_root)
        return 0;
    data = m_tree_root->m_data;
    //phys_inplace_avl_tree<unsigned int, generic_avl_map_node_t, generic_avl_map_node_t>::remove(gam, &avl_key);
    gam->remove(avl_key);
    //phys_simple_allocator<generic_avl_map_node_t>::free(&g_generic_avl_map_node_allocator, m_tree_root);
    g_generic_avl_map_node_allocator.free(m_tree_root);
    return data;
}

PhysObjUserData *__cdecl Phys_GetUserData(int id)
{
    iassert(id);

    return (PhysObjUserData *)id;
}

int __cdecl get_physics_contents_mask(char phys_env_collision_flags)
{
    int mask; // [esp+0h] [ebp-4h]

    mask = 0x280EC93;
    if ((phys_env_collision_flags & 0x20) != 0 && (phys_env_collision_flags & 0x10) == 0)
        return 0x281EE93;
    if ((phys_env_collision_flags & 8) != 0 || (phys_env_collision_flags & 0x10) != 0)
        return 0x211;
    return mask;
}

broad_phase_info *__cdecl allocate_bpi_env()
{
    signed __int32 v1; // [esp+0h] [ebp-4Ch]
    void *v2; // [esp+40h] [ebp-Ch]
    broad_phase_info *first; // [esp+44h] [ebp-8h]

    _InterlockedExchangeAdd(&G_BPM->m_bpi_env_count, 1u);
    //v2 = phys_transient_allocator::mt_allocate(
    v2 = G_BPM->g_collision_memory_buffer.mt_allocate(
        112,
        16,
        0,
        "broad phase collision out of memory.");
    if (v2)
        v1 = (signed __int32)v2;
    else
        v1 = 0;
    do
    {
        first = G_BPM->m_list_bpi_env;
        *(_DWORD *)(v1 + 56) = (DWORD)first;
    } while ((broad_phase_info *)_InterlockedCompareExchange(
        (volatile unsigned __int32 *)&G_BPM->m_list_bpi_env,
        v1,
        (signed __int32)first) != first);
    return (broad_phase_info *)v1;
}

char are_intersecting(
    const broad_phase_environment_query_input *bpeqi,
    const phys_vec3 *aabb_min,
    const phys_vec3 *aabb_max,
    const phys_vec3 *aabb_trans)
{
    float v6; // [esp-Ch] [ebp-30h] BYREF
    phys_vec3 v7; // [esp-8h] [ebp-2Ch] OVERLAPPED BYREF
    float trans_4; // [esp+8h] [ebp-1Ch]
    float trans_8; // [esp+Ch] [ebp-18h]
    float trans_12; // [esp+10h] [ebp-14h]
    const phys_vec3 *p_trace_translation; // [esp+14h] [ebp-10h]
    //int v12; // [esp+18h] [ebp-Ch]
    //void *v13; // [esp+1Ch] [ebp-8h]
    //void *retaddr; // [esp+24h] [ebp+0h]
    //
    //v12 = a1;
    //v13 = retaddr;
    p_trace_translation = &bpeqi->trace_translation;
    trans_12 = bpeqi->trace_translation.x - aabb_trans->x;
    trans_8 = bpeqi->trace_translation.y - aabb_trans->y;
    trans_4 = bpeqi->trace_translation.z - aabb_trans->z;
    v7.x = trans_12;
    v7.y = trans_8;
    v7.z = trans_4;
    return phys_are_potentially_colliding(
        &bpeqi->trace_aabb_min_wace,
        &bpeqi->trace_aabb_max_wace,
        &v7,
        aabb_min,
        aabb_max,
        &v6);
}

void standard_query::query(
    const broad_phase_environment_query_input *bpeqi,
    broad_phase_environement_query_results *bpeqr)
{
    void *v4; // esp
    unsigned int geom_id; // eax
    broad_phase_info *v6; // eax
    unsigned int v7; // eax
    broad_phase_info *v8; // eax
    int Contents; // eax
    unsigned int v10; // eax
    broad_phase_info *v11; // eax
    int stype; // [esp-35CCh] [ebp-35D8h]
    int v13; // [esp-35CCh] [ebp-35D8h]
    int v14; // [esp-35CCh] [ebp-35D8h]
    const phys_mat44 *p_m_mat; // [esp-35BCh] [ebp-35C8h]
    broad_phase_info *v16; // [esp-35B8h] [ebp-35C4h]
    broad_phase_environment_info *v17; // [esp-35B4h] [ebp-35C0h]
    gjk_base_t *m_first_geom; // [esp-35A0h] [ebp-35ACh]
    PhysObjUserData *v19; // [esp-3598h] [ebp-35A4h]
    int jj; // [esp-3594h] [ebp-35A0h]
    const Glass *v21; // [esp-358Ch] [ebp-3598h]
    unsigned int ii; // [esp-3588h] [ebp-3594h]
    unsigned int v23; // [esp-3584h] [ebp-3590h]
    const Glass *v24[129]; // [esp-3580h] [ebp-358Ch] BYREF
    const vehicle_info_t *VehicleInfo; // [esp-337Ch] [ebp-3388h]
    __int16 vehicleInfoIndex; // [esp-3378h] [ebp-3384h]
    const XModel *Model; // [esp-3374h] [ebp-3380h]
    const DObj *ClientDObj; // [esp-3370h] [ebp-337Ch]
    const centity_s *Entity; // [esp-336Ch] [ebp-3378h]
    int n; // [esp-3368h] [ebp-3374h]
    int v31; // [esp-3364h] [ebp-3370h]
    int v32[17]; // [esp-3360h] [ebp-336Ch] BYREF
    DynEntityClient *ClientEntity; // [esp-331Ch] [ebp-3328h]
    unsigned __int16 Id; // [esp-3318h] [ebp-3324h]
    const DynEntityDef *EntityDef; // [esp-3314h] [ebp-3320h]
    int m; // [esp-3310h] [ebp-331Ch]
    DynEntityDrawType k; // [esp-330Ch] [ebp-3318h]
    _WORD v38[4098]; // [esp-3308h] [ebp-3314h] BYREF
    DynEntityAreaParms v39; // [esp-1304h] [ebp-1310h] BYREF
    broad_phase_info *v40; // [esp-12F0h] [ebp-12FCh]
    gjk_partition_t *v41; // [esp-12ECh] [ebp-12F8h]
    broad_phase_environment_info *v42; // [esp-12E8h] [ebp-12F4h]
    CollisionPartition *v43; // [esp-12E4h] [ebp-12F0h]
    int v44; // [esp-12E0h] [ebp-12ECh]
    CollisionPartition *v45; // [esp-12DCh] [ebp-12E8h]
    int v46; // [esp-12D8h] [ebp-12E4h]
    float v47; // [esp-12D4h] [ebp-12E0h] BYREF
    phys_vec3 v48; // [esp-12D0h] [ebp-12DCh] BYREF
    phys_vec3 v49; // [esp-12C0h] [ebp-12CCh] BYREF
    phys_vec3 v50; // [esp-12B0h] [ebp-12BCh] BYREF
    float v51; // [esp-129Ch] [ebp-12A8h]
    float v52; // [esp-1298h] [ebp-12A4h]
    float v53; // [esp-1294h] [ebp-12A0h]
    phys_vec3 v54; // [esp-1290h] [ebp-129Ch] BYREF
    float v55; // [esp-127Ch] [ebp-1288h]
    float v56; // [esp-1278h] [ebp-1284h]
    float v57; // [esp-1274h] [ebp-1280h]
    phys_vec3 v58; // [esp-1270h] [ebp-127Ch] BYREF
    phys_vec3 v59; // [esp-1260h] [ebp-126Ch] BYREF
    CollisionPartition *v60; // [esp-124Ch] [ebp-1258h]
    const CollisionAabbTree *v61; // [esp-1248h] [ebp-1254h]
    int j; // [esp-1244h] [ebp-1250h]
    broad_phase_info *bpi_env; // [esp-1240h] [ebp-124Ch]
    gjk_brush_t *v64; // [esp-123Ch] [ebp-1248h]
    int v65; // [esp-1238h] [ebp-1244h]
    broad_phase_environment_info *bpei_mt; // [esp-1234h] [ebp-1240h]
    const cbrush_t *v67; // [esp-1230h] [ebp-123Ch]
    int v68; // [esp-122Ch] [ebp-1238h]
    const cbrush_t *v69; // [esp-1228h] [ebp-1234h]
    int v70; // [esp-1224h] [ebp-1230h]
    phys_vec3 v71; // [esp-1220h] [ebp-122Ch] BYREF
    phys_vec3 v72; // [esp-1210h] [ebp-121Ch] BYREF
    phys_vec3 v73; // [esp-1200h] [ebp-120Ch] BYREF
    const cbrush_t *v74; // [esp-11E8h] [ebp-11F4h]
    int i; // [esp-11E4h] [ebp-11F0h]
    static_colgeom_visitor_t v76; // [esp-11E0h] [ebp-11ECh] BYREF
    int v77; // [esp-1170h] [ebp-117Ch]
    _DWORD v78[512]; // [esp-116Ch] [ebp-1178h]
    int v79; // [esp-96Ch] [ebp-978h]
    _DWORD v80[512]; // [esp-968h] [ebp-974h]
    int physics_contents_mask; // [esp-168h] [ebp-174h]
    environment_rigid_body *environment_rigid_body; // [esp-164h] [ebp-170h]
    gjk_physics_collision_visitor v83; // [esp-160h] [ebp-16Ch] BYREF
    float v84[3]; // [esp-DCh] [ebp-E8h] BYREF
    phys_vec3 v85; // [esp-D0h] [ebp-DCh] BYREF
    float v86; // [esp-B4h] [ebp-C0h]
    float v87; // [esp-B0h] [ebp-BCh]
    float v88; // [esp-ACh] [ebp-B8h]
    const phys_vec3 *v89; // [esp-A8h] [ebp-B4h]
    float v90[3]; // [esp-A4h] [ebp-B0h] BYREF
    float v91[3]; // [esp-98h] [ebp-A4h] BYREF
    float v92[3]; // [esp-8Ch] [ebp-98h] BYREF
    phys_vec3 v93; // [esp-80h] [ebp-8Ch] BYREF
    phys_vec3 v94; // [esp-70h] [ebp-7Ch] BYREF
    float v95; // [esp-54h] [ebp-60h]
    float v96; // [esp-50h] [ebp-5Ch]
    float v97; // [esp-4Ch] [ebp-58h]
    const phys_vec3 *p_trace_aabb_max_wace; // [esp-48h] [ebp-54h]
    const phys_vec3 *v99; // [esp-44h] [ebp-50h]
    phys_vec3 v100; // [esp-40h] [ebp-4Ch] BYREF
    phys_vec3 v101; // [esp-30h] [ebp-3Ch] BYREF
    float v102; // [esp-18h] [ebp-24h]
    float v103; // [esp-14h] [ebp-20h]
    float v104; // [esp-10h] [ebp-1Ch]
    const phys_vec3 *p_trace_translation; // [esp-Ch] [ebp-18h]
    standard_query *v106; // [esp-8h] [ebp-14h]
    //_DWORD v107[2]; // [esp+0h] [ebp-Ch] BYREF
    //int v108; // [esp+8h] [ebp-4h] BYREF
    //_UNKNOWN *retaddr; // [esp+Ch] [ebp+0h]
    //
    //v107[0] = a2;
    //v107[1] = retaddr;
    v4 = alloca(13760);
    v106 = this;
    p_trace_translation = &bpeqi->trace_translation;
    v104 = bpeqi->trace_aabb_min_wace.x + bpeqi->trace_translation.x;
    v103 = bpeqi->trace_aabb_min_wace.y + bpeqi->trace_translation.y;
    v102 = bpeqi->trace_aabb_min_wace.z + bpeqi->trace_translation.z;
    v101.x = v104;
    v101.y = v103;
    v101.z = v102;
    phys_min(&v100, &bpeqi->trace_aabb_min_wace, &v101);
    v99 = &bpeqi->trace_translation;
    p_trace_aabb_max_wace = &bpeqi->trace_aabb_max_wace;
    v97 = bpeqi->trace_aabb_max_wace.x + bpeqi->trace_translation.x;
    v96 = bpeqi->trace_aabb_max_wace.y + bpeqi->trace_translation.y;
    v95 = bpeqi->trace_aabb_max_wace.z + bpeqi->trace_translation.z;
    v94.x = v97;
    v94.y = v96;
    v94.z = v95;
    phys_max(&v93, &bpeqi->trace_aabb_max_wace, &v94);
    Phys_NitrousVecToVec3(&v100, v92);
    Phys_NitrousVecToVec3(&v93, v91);
    if (phys_debugBigQueries->current.enabled && Abs(&bpeqi->trace_translation.x) > 500.0)
    {
        //minspec_mutex::Lock(&g_render_mutex);
        g_render_mutex.Lock();
        render_box(&bpeqi->trace_aabb_min_wace, &bpeqi->trace_aabb_max_wace, colorBlue, 1000);
        Phys_NitrousVecToVec3(&bpeqi->trace_aabb_min_wace, v90);
        v89 = &bpeqi->trace_translation;
        v88 = bpeqi->trace_aabb_min_wace.x + bpeqi->trace_translation.x;
        v87 = bpeqi->trace_aabb_min_wace.y + bpeqi->trace_translation.y;
        v86 = bpeqi->trace_aabb_min_wace.z + bpeqi->trace_translation.z;
        v85.x = v88;
        v85.y = v87;
        v85.z = v86;
        Phys_NitrousVecToVec3(&v85, v84);
        CG_DebugLine(v90, v84, colorRed, 1, 1000);
        v84[0] = v90[0];
        v84[1] = v90[1];
        v84[2] = v90[2] + 10000.0;
        CG_DebugLine(v90, v84, colorWhite, 1, 1000);
        //minspec_mutex::Unlock(&g_render_mutex);
        g_render_mutex.Unlock();
    }
    //v83.__vftable = (gjk_physics_collision_visitor_vtbl *)&gjk_physics_collision_visitor::`vftable';
    v83.bpeqi = bpeqi;
    v83.bpeqr = bpeqr;
    bpeqr->m_list_bpi_env.m_list_cur = &bpeqr->m_list_bpi_env.m_list;
    bpeqr->m_list_bpi_env_count = 0;
    bpeqr->m_env_collision_flags = 0;
    environment_rigid_body = phys_sys::get_environment_rigid_body();
    physics_contents_mask = (int)get_physics_contents_mask(bpeqi->env_collision_flags);
    if ((bpeqi->env_collision_flags & 1) != 0)
    {
        //colgeom_visitor_t::colgeom_visitor_t(&v76);
        //v76.__vftable = (colgeom_visitor_t_vtbl *)&static_colgeom_visitor_t::`vftable';
        v77 = 0;
        v79 = 0;
        //colgeom_visitor_t::intersect_box(&v76, v92, v91, physics_contents_mask);
        v76.intersect_box(v92, v91, physics_contents_mask);
        for (i = 0; i < v79; ++i)
        {
            v74 = (const cbrush_t *)v80[i];
            Phys_Vec3ToNitrousVec(v74->mins, &v73);
            Phys_Vec3ToNitrousVec(v74->maxs, &v72);
            v71 = PHYS_ZERO_VEC;
            if (are_intersecting(bpeqi, &v73, &v72, &v71))
            {
                v69 = v74;
                v70 = 0;
                v67 = v74;
                v68 = 0;
                //bpei_mt = bpei_database_t::get_bpei_mt(&G_BPM->g_bpei_database, (bpei_database_id)(unsigned int)v74);
                bpei_mt = G_BPM->g_bpei_database.get_bpei_mt((bpei_database_id)(unsigned int)v74);
                if (!bpei_mt->m_data)
                {
                    //minspec_mutex::Lock(&bpei_mt->m_mutex);
                    bpei_mt->m_mutex.Lock();
                    if (!bpei_mt->m_data)
                    {
                        v65 = (unsigned __int8)((int)(0x3F00000
                            & v74->axial_sflags[0][0]) >> 20);
                        v64 = gjk_brush_t::create(v74, v65, &v83);
                        //gjk_base_t::set_geom_id_new(v64, bpei_mt->m_gjk_geom_id);
                        v64->set_geom_id_new(bpei_mt->m_gjk_geom_id);
                        bpi_env = allocate_bpi_env();
                        stype = v64->stype;
                        //geom_id = gjk_base_t::get_geom_id(v64);
                        geom_id = v64->get_geom_id();
                        //broad_phase_info::set(
                            bpi_env->set(
                            environment_rigid_body,
                            &PHYS_IDENTITY_MATRIX,
                            &PHYS_IDENTITY_MATRIX,
                            0,
                            v64,
                            geom_id,
                            0,
                            stype,
                            0,
                            1u);
                        //broad_phase_info::set_bpi_env(bpi_env, 0);
                        bpi_env->set_bpi_env(0);
                        if (bpi_env)
                        {
                            if ((bpi_env->m_flags & 4) == 0
                                && _tlAssert(
                                    "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp",
                                    591,
                                    "bpi_env->is_bpi_env()",
                                    ""))
                            {
                                __debugbreak();
                            }
                            //v6 = broad_phase_base::get_bpi_env(bpi_env);
                            v6 = bpi_env->get_bpi_env();
                            //broad_phase_info::collision_prolog(v6);
                            v6->collision_prolog();
                            bpei_mt->m_data = bpi_env;
                        }
                    }
                    //minspec_mutex::Unlock(&bpei_mt->m_mutex);
                    bpei_mt->m_mutex.Unlock();
                }
                if (bpei_mt->m_data)
                {
                    //broad_phase_environement_query_results::add(bpeqr, (broad_phase_base *)bpei_mt->m_data);
                    bpeqr->add((broad_phase_base *)bpei_mt->m_data);
                }
            }
        }
        for (j = 0; j < v77; ++j)
        {
            v61 = (const CollisionAabbTree *)v78[j];
            v60 = &cm.partitions[v61->u.firstChildIndex];
            Phys_Vec3ToNitrousVec(v61->halfSize, &v59);
            Phys_Vec3ToNitrousVec(v61->origin, &v58);
            v57 = v58.x - v59.x;
            v56 = v58.y - v59.y;
            v55 = v58.z - v59.z;
            v54.x = v58.x - v59.x;
            v54.y = v58.y - v59.y;
            v54.z = v58.z - v59.z;
            v53 = v58.x + v59.x;
            v52 = v58.y + v59.y;
            v51 = v58.z + v59.z;
            v50.x = v58.x + v59.x;
            v50.y = v58.y + v59.y;
            v50.z = v58.z + v59.z;
            v49 = PHYS_ZERO_VEC;
            //operator-(&v48, &bpeqi->trace_translation, &v49);
            v48 = bpeqi->trace_translation - v49;
            if (phys_are_potentially_colliding(
                &bpeqi->trace_aabb_min_wace,
                &bpeqi->trace_aabb_max_wace,
                &v48,
                &v54,
                &v50,
                &v47))
            {
                v45 = v60;
                v46 = 0;
                v43 = v60;
                v44 = 0;
                //v42 = bpei_database_t::get_bpei_mt(&G_BPM->g_bpei_database, (bpei_database_id)(unsigned int)v60);
                v42 = G_BPM->g_bpei_database.get_bpei_mt((bpei_database_id)(unsigned int)v60);
                if (!v42->m_data)
                {
                    //minspec_mutex::Lock(&v42->m_mutex);
                    v42->m_mutex.Lock();
                    if (!v42->m_data)
                    {
                        v41 = gjk_partition_t::create(v61, &v83);
                        //gjk_base_t::set_geom_id_new(v41, v42->m_gjk_geom_id);
                        v41->set_geom_id_new(v42->m_gjk_geom_id);
                        v40 = allocate_bpi_env();
                        v13 = v41->stype;
                        //v7 = gjk_base_t::get_geom_id(v41);
                        v7 = v41->get_geom_id();
                        //broad_phase_info::set(
                            v40->set(
                            environment_rigid_body,
                            &PHYS_IDENTITY_MATRIX,
                            &PHYS_IDENTITY_MATRIX,
                            0,
                            v41,
                            v7,
                            0,
                            v13,
                            0,
                            1u);
                        //broad_phase_info::set_bpi_env(v40, 0);
                        v40->set_bpi_env(0);
                        if (v40)
                        {
                            if ((v40->m_flags & 4) == 0
                                && _tlAssert(
                                    "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp",
                                    619,
                                    "bpi_env->is_bpi_env()",
                                    ""))
                            {
                                __debugbreak();
                            }
                            //v8 = broad_phase_base::get_bpi_env(v40);
                            v8 = v40->get_bpi_env();
                            //broad_phase_info::collision_prolog(v8);
                            v8->collision_prolog();
                            v42->m_data = v40;
                        }
                    }
                    //minspec_mutex::Unlock(&v42->m_mutex);
                    v42->m_mutex.Unlock();
                }
                if (v42->m_data)
                {
                    //broad_phase_environement_query_results::add(bpeqr, (broad_phase_base *)v42->m_data);
                    bpeqr->add((broad_phase_base *)v42->m_data);
                }
            }
        }
        //v76.__vftable = (colgeom_visitor_t_vtbl *)&visitor_base_t::`vftable';
    }
    if (phys_entityCollision->current.enabled)
    {
        if ((bpeqi->env_collision_flags & 0x10) == 0 && (bpeqi->env_collision_flags & 4) != 0)
        {
            v39.mins = v92;
            v39.maxs = v91;
            v39.contentMask = -1;
            v39.list = v38;
            v39.maxCount = 4096;
            for (k = DYNENT_DRAW_MODEL; k < DYNENT_DRAW_COUNT; ++k)
            {
                v39.count = 0;
                DynEntCl_AreaEntities_r((DynEntityCollType)k, 1u, &v39);
                for (m = 0; m < v39.count; ++m)
                {
                    EntityDef = DynEnt_GetEntityDef(v38[m], k);
                    if ((bpeqi->env_collision_flags & 8) != 0 || (EntityDef->flags & 2) != 0)
                    {
                        Id = DynEnt_GetId(EntityDef, k);
                        ClientEntity = DynEnt_GetClientEntity(Id, k);
                        if (!ClientEntity->physObjId)
                        {
                            v83.cent = 0;
                            v83.dynEntDef = EntityDef;
                            v83.glass = 0;
                            v83.rb = environment_rigid_body;
                            v83.rb_to_world_xform = &PHYS_IDENTITY_MATRIX;
                            v83.cg_to_world_xform = 0;
                            v83.cg_to_rb_xform = 0;
                            v83.env_collision_flags = 4;
                            create_entity_bpi(&v83, physics_contents_mask);
                        }
                    }
                }
            }
        }
        if ((bpeqi->env_collision_flags & 2) != 0)
        {
            v31 = CG_AreaEntities(v92, v91, v32, 16, physics_contents_mask);
            for (n = 0; n < v31; ++n)
            {
                Entity = CG_GetEntity(0, v32[n]);
                ClientDObj = Com_GetClientDObj(v32[n], 0);
                if (ClientDObj)
                {
                    Model = DObjGetModel(ClientDObj, 0);
                    if (Model)
                    {
                        Contents = XModelGetContents(Model);
                        if ((physics_contents_mask & Contents) == 0 && Entity->nextState.eType != 17)
                            continue;
                    }
                }
                if (Entity->nextState.eType == 6)
                {
                    auto_rigid_body::add(Entity, &v83, physics_contents_mask);
                }
                else if (Entity->nextState.eType == 14 || Entity->nextState.eType == 16)
                {
                    vehicleInfoIndex = Entity->nextState.vehicleState.vehicleInfoIndex;
                    VehicleInfo = CG_GetVehicleInfo(vehicleInfoIndex);
                    if (!VehicleInfo->name[0])
                        VehicleInfo = BG_GetVehicleInfo(vehicleInfoIndex);
                    if (VehicleInfo->name[0] <= 0
                        && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp",
                            717,
                            0,
                            "%s",
                            "vehicleInfo->name[0] > 0"))
                    {
                        __debugbreak();
                    }
                    if (!VehicleInfo->isNitrous)
                        auto_rigid_body::add(Entity, &v83, physics_contents_mask);
                }
                else if (Entity->nextState.eType == 17 && dynEnt_sentientAutoActivate->current.enabled)
                {
                    auto_rigid_body::add(Entity, &v83, physics_contents_mask);
                }
            }
        }
        if ((bpeqi->env_collision_flags & 0x40) != 0)
        {
            v23 = GlassCl_AreaGlasses(v92, v91, v24, 0x80u);
            for (ii = 0; ii < v23; ++ii)
            {
                v21 = v24[ii];
                v83.cent = 0;
                v83.dynEntDef = 0;
                v83.glass = v21;
                v83.rb = phys_sys::get_environment_rigid_body();
                v83.rb_to_world_xform = &PHYS_IDENTITY_MATRIX;
                v83.cg_to_world_xform = 0;
                v83.cg_to_rb_xform = 0;
                v83.env_collision_flags = 64;
                create_entity_bpi(&v83, physics_contents_mask);
            }
        }
        if ((bpeqi->env_collision_flags & 0x10) != 0
            && (bpeqi->env_collision_flags & 8) == 0
            && (bpeqi->env_collision_flags & 0x100) != 0)
        {
            for (jj = 0; jj < 16; ++jj)
            {
                v19 = physGlob.userRigidBodies[jj];
                if (v19)
                {
                    if (v19->m_gjk_geom_list.m_geom_count < 0
                        && _tlAssert(
                            "c:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_colgeom.h",
                            1035,
                            "m_geom_count >= 0",
                            ""))
                    {
                        __debugbreak();
                    }
                    m_first_geom = v19->m_gjk_geom_list.m_first_geom;
                    //v17 = bpei_database_t::get_bpei_mt(&G_BPM->g_bpei_database, (bpei_database_id)(unsigned int)v19);
                    v17 = G_BPM->g_bpei_database.get_bpei_mt((bpei_database_id)(unsigned int)v19);
                    if (!v17->m_data)
                    {
                        v17->m_mutex.Lock();
                        if (!v17->m_data)
                        {
                            v16 = allocate_bpi_env();
                            p_m_mat = &v19->body->m_mat;
                            v14 = m_first_geom->stype;
                            //v10 = gjk_base_t::get_geom_id(m_first_geom);
                            v10 = m_first_geom->get_geom_id();
                            //broad_phase_info::set(v16, v19->body, p_m_mat, &v19->cg2w, &v19->cg2rb, m_first_geom, v10, 0, v14, 0, 2u);
                            v16->set(v19->body, p_m_mat, &v19->cg2w, &v19->cg2rb, m_first_geom, v10, 0, v14, 0, 2u);
                            //broad_phase_info::set_bpi_env(v16, 0);
                            v16->set_bpi_env(0);
                            if (v16)
                            {
                                if ((v16->m_flags & 4) == 0
                                    && _tlAssert(
                                        "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp",
                                        776,
                                        "bpi_env->is_bpi_env()",
                                        ""))
                                {
                                    __debugbreak();
                                }
                                //v11 = broad_phase_base::get_bpi_env(v16);
                                v11 = v16->get_bpi_env();
                                //broad_phase_info::collision_prolog(v11);
                                v11->collision_prolog();
                                v17->m_data = v16;
                            }
                        }
                        v17->m_mutex.Unlock();
                    }
                    if (v17->m_data)
                    {
                        //broad_phase_environement_query_results::add(bpeqr, (broad_phase_base *)v17->m_data);
                        bpeqr->add((broad_phase_base *)v17->m_data);
                    }
                }
            }
        }
    }
}

void gjk_physics_collision_visitor::set_local_query_info(
    const void *entity)
{
    const phys_vec3 *v3; // eax
    phys_vec3 *p_m_local_query_trace_translation; // [esp-58h] [ebp-64h]
    phys_vec3 v5; // [esp-50h] [ebp-5Ch] BYREF
    phys_vec3 v6; // [esp-40h] [ebp-4Ch] BYREF
    float v7; // [esp-30h] [ebp-3Ch]
    float v8; // [esp-2Ch] [ebp-38h]
    float v9; // [esp-28h] [ebp-34h]
    const float *p_x; // [esp-24h] [ebp-30h]
    phys_vec3 m_moved_vec; // [esp-20h] [ebp-2Ch]
    rigid_body *rb; // [esp-8h] [ebp-14h]
    gjk_physics_collision_visitor *v13; // [esp-4h] [ebp-10h]
    //int v14; // [esp+0h] [ebp-Ch]
    //void *v15; // [esp+4h] [ebp-8h]
    //void *retaddr; // [esp+Ch] [ebp+0h]
    //
    //v14 = a2;
    //v15 = retaddr;
    v13 = this;
    phys_calc_local_aabb(
        &this->bpeqi->trace_aabb_min_wace,
        &this->bpeqi->trace_aabb_max_wace,
        this->cg_to_world_xform,
        &this->m_local_query_trace_aabb_min,
        &this->m_local_query_trace_aabb_max);
    rb = v13->rb;
    m_moved_vec = rb->m_moved_vec;
    p_x = &v13->bpeqi->trace_translation.x;
    v9 = *p_x - m_moved_vec.x;
    v8 = p_x[1] - m_moved_vec.y;
    v7 = p_x[2] - m_moved_vec.z;
    v6.x = v9;
    v6.y = v8;
    v6.z = v7;
    v3 = phys_inv_multiply(&v5, v13->cg_to_world_xform, &v6);
    p_m_local_query_trace_translation = &v13->m_local_query_trace_translation;
    v13->m_local_query_trace_translation.x = v3->x;
    p_m_local_query_trace_translation->y = v3->y;
    p_m_local_query_trace_translation->z = v3->z;
    v13->m_local_entity = entity;
}

void gjk_physics_collision_visitor::get_local_query_aabb(
    float *local_query_aabb_min,
    float *local_query_aabb_max)
{
    phys_vec3 v4; // [esp-Ch] [ebp-8Ch] BYREF
    phys_vec3 m_local_query_aabb_max_4; // [esp+4h] [ebp-7Ch] BYREF
    float v6; // [esp+20h] [ebp-60h]
    float v7; // [esp+24h] [ebp-5Ch]
    float v8; // [esp+28h] [ebp-58h]
    phys_vec3 *p_m_local_query_trace_aabb_max; // [esp+2Ch] [ebp-54h]
    phys_vec3 *v10; // [esp+30h] [ebp-50h]
    phys_vec3 m_local_query_aabb_min; // [esp+34h] [ebp-4Ch] BYREF
    phys_vec3 v12; // [esp+44h] [ebp-3Ch] BYREF
    float v13; // [esp+5Ch] [ebp-24h]
    float v14; // [esp+60h] [ebp-20h]
    float v15; // [esp+64h] [ebp-1Ch]
    phys_vec3 *p_m_local_query_trace_aabb_min; // [esp+68h] [ebp-18h]
    phys_vec3 *p_m_local_query_trace_translation; // [esp+6Ch] [ebp-14h]
    gjk_physics_collision_visitor *thisa; // [esp+70h] [ebp-10h]
    //_UNKNOWN *v19; // [esp+74h] [ebp-Ch]
    //float *local_query_aabb_mina; // [esp+78h] [ebp-8h]
    //int vars0; // [esp+80h] [ebp+0h]
    //
    //v19 = a2;
    //local_query_aabb_mina = (float *)vars0;
    thisa = this;
    p_m_local_query_trace_translation = &this->m_local_query_trace_translation;
    p_m_local_query_trace_aabb_min = &this->m_local_query_trace_aabb_min;
    v15 = this->m_local_query_trace_aabb_min.x + this->m_local_query_trace_translation.x;
    v14 = this->m_local_query_trace_aabb_min.y + this->m_local_query_trace_translation.y;
    v13 = this->m_local_query_trace_aabb_min.z + this->m_local_query_trace_translation.z;
    v12.x = v15;
    v12.y = v14;
    v12.z = v13;
    phys_min(&m_local_query_aabb_min, &this->m_local_query_trace_aabb_min, &v12);
    v10 = &thisa->m_local_query_trace_translation;
    p_m_local_query_trace_aabb_max = &thisa->m_local_query_trace_aabb_max;
    v8 = thisa->m_local_query_trace_aabb_max.x + thisa->m_local_query_trace_translation.x;
    v7 = thisa->m_local_query_trace_aabb_max.y + thisa->m_local_query_trace_translation.y;
    v6 = thisa->m_local_query_trace_aabb_max.z + thisa->m_local_query_trace_translation.z;
    m_local_query_aabb_max_4.x = v8;
    m_local_query_aabb_max_4.y = v7;
    m_local_query_aabb_max_4.z = v6;
    phys_max(&v4, &thisa->m_local_query_trace_aabb_max, &m_local_query_aabb_max_4);
    Phys_NitrousVecToVec3(&m_local_query_aabb_min, local_query_aabb_min);
    Phys_NitrousVecToVec3(&v4, local_query_aabb_max);
}

bool gjk_physics_collision_visitor::query_create_prolog(const void *geom)
{
    if (!geom
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp", 290, 0, "%s", "geom"))
    {
        __debugbreak();
    }
    this->m_local_database_id.m_id1 = (unsigned int)this->m_local_entity;
    this->m_local_database_id.m_id2 = (unsigned int)geom;
    //this->m_local_bpei = bpei_database_t::get_bpei_mt(&G_BPM->g_bpei_database, this->m_local_database_id);
    this->m_local_bpei = G_BPM->g_bpei_database.get_bpei_mt(this->m_local_database_id);

    if (!this->m_local_bpei->m_data)
    {
        //minspec_mutex::Lock(&this->m_local_bpei->m_mutex);
        this->m_local_bpei->m_mutex.Lock();
        if (!this->m_local_bpei->m_data)
            return 1;
        //minspec_mutex::Unlock(&this->m_local_bpei->m_mutex);
        this->m_local_bpei->m_mutex.Unlock();
    }

    if (!this->m_local_bpei->m_data
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp",
            300,
            0,
            "%s",
            "m_local_bpei->m_data"))
    {
        __debugbreak();
    }

    //broad_phase_environement_query_results::add(this->bpeqr, (broad_phase_base *)this->m_local_bpei->m_data);
    this->bpeqr->add((broad_phase_base *)this->m_local_bpei->m_data);
    return 0;
}

bool gjk_physics_collision_visitor::query_create_prolog_1(
    float *local_aabb_min,
    float *local_aabb_max,
    const void *geom)
{
    float v6; // [esp-Ch] [ebp-40h] BYREF
    phys_vec3 v7; // [esp-8h] [ebp-3Ch] BYREF
    phys_vec3 pv_local_aabb_min; // [esp+8h] [ebp-2Ch] BYREF
    gjk_physics_collision_visitor *thisa; // [esp+24h] [ebp-10h]
    //_UNKNOWN *v10; // [esp+28h] [ebp-Ch]
    //const float *local_aabb_mina; // [esp+2Ch] [ebp-8h]
    //const void *geoma; // [esp+34h] [ebp+0h]
    //
    //v10 = a2;
    //local_aabb_mina = (const float *)geoma;
    thisa = this;
    Phys_Vec3ToNitrousVec(local_aabb_min, &pv_local_aabb_min);
    Phys_Vec3ToNitrousVec(local_aabb_max, &v7);
    return phys_are_potentially_colliding(
        &thisa->m_local_query_trace_aabb_min,
        &thisa->m_local_query_trace_aabb_max,
        &thisa->m_local_query_trace_translation,
        &pv_local_aabb_min,
        &v7,
        &v6)
        && thisa->query_create_prolog(geom);
}

void gjk_physics_collision_visitor::query_create_epilog(gjk_base_t *gjk_geom)
{
    unsigned int geom_id; // eax
    int stype; // [esp-Ch] [ebp-90h]
    unsigned int env_collision_flags; // [esp-4h] [ebp-88h]
    BOOL v6; // [esp+0h] [ebp-84h]
    broad_phase_info *bpi_env; // [esp+80h] [ebp-4h]

    if (!gjk_geom
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp", 307, 0, "%s", "gjk_geom"))
    {
        __debugbreak();
    }
    if (gjk_geom->stype > 0x22u
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp",
            308,
            0,
            "%s",
            "gjk_geom->stype >= 0 && gjk_geom->stype < MAX_NUM_SURFACE_TYPES"))
    {
        __debugbreak();
    }
    if (!this->m_local_bpei
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp", 309, 0, "%s", "m_local_bpei"))
    {
        __debugbreak();
    }
    if (this->m_local_bpei->m_data
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp",
            310,
            0,
            "%s",
            "m_local_bpei->m_data == NULL"))
    {
        __debugbreak();
    }
    v6 = this->m_local_bpei->m_database_id.m_id1 == this->m_local_database_id.m_id1
        && this->m_local_bpei->m_database_id.m_id2 == this->m_local_database_id.m_id2;
    if (!v6
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp",
            311,
            0,
            "%s",
            "m_local_bpei->m_database_id == m_local_database_id"))
    {
        __debugbreak();
    }
    //gjk_base_t::set_geom_id_new(gjk_geom, this->m_local_bpei->m_gjk_geom_id);
    gjk_geom->set_geom_id_new(this->m_local_bpei->m_gjk_geom_id);
    bpi_env = allocate_bpi_env();
    env_collision_flags = this->env_collision_flags;
    stype = gjk_geom->stype;
    //geom_id = gjk_base_t::get_geom_id(gjk_geom);
    geom_id = gjk_geom->get_geom_id();
    //broad_phase_info::set(
        bpi_env->set(
        this->rb,
        this->rb_to_world_xform,
        this->cg_to_world_xform,
        this->cg_to_rb_xform,
        gjk_geom,
        geom_id,
        0,
        stype,
        0,
        env_collision_flags);
    //broad_phase_info::set_bpi_env(bpi_env, this->auto_activate_callback);
    bpi_env->set_bpi_env(this->auto_activate_callback);
    //broad_phase_info::collision_prolog(bpi_env);
    bpi_env->collision_prolog();
    this->m_local_bpei->m_data = bpi_env;
    //minspec_mutex::Unlock(&this->m_local_bpei->m_mutex);
    this->m_local_bpei->m_mutex.Unlock();
    if (!this->m_local_bpei->m_data
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\physics\\phys_collision.cpp",
            321,
            0,
            "%s",
            "m_local_bpei->m_data"))
    {
        __debugbreak();
    }
    //broad_phase_environement_query_results::add(this->bpeqr, (broad_phase_base *)this->m_local_bpei->m_data);
    this->bpeqr->add((broad_phase_base *)this->m_local_bpei->m_data);
}

void __thiscall gjk_physics_collision_visitor::query_create_epilog_1(
    gjk_base_t *gjk_geom)
{
    this->query_create_epilog(gjk_geom);
}