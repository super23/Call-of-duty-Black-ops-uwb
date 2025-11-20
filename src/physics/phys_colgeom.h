#pragma once

PhysGeomList *__cdecl xmodel_get_geomlist(const XModel *model, int bone_index);
gjk_aabb_t *__cdecl gjk_aabb_t::create(
        const phys_vec3 *center,
        const phys_vec3 *dims,
        int stype,
        gjk_collision_visitor *allocator);
unsigned int __thiscall gjk_unique_id_database_t::get_unique_id(gjk_unique_id_database_t *this);
void __thiscall gjk_base_t::set_geom_id_new(gjk_base_t *this, unsigned int geom_id);
void __thiscall gjk_aabb_t::support(
        gjk_aabb_t *this,
        const phys_vec3 *v,
        phys_vec3 *support_vert,
        phys_vec3 *support_ind);
void __thiscall gjk_aabb_t::get_simplex(
        gjk_aabb_t *this,
        const cached_simplex_info *cache_info,
        int index_count,
        phys_vec3 *simplex_verts,
        phys_vec3 *simplex_inds);
const phys_vec3 *__thiscall gjk_aabb_t::get_center(gjk_polygon_cylinder_t *this, phys_vec3 *result);
void __userpurge gjk_aabb_t::get_feature(
        gjk_aabb_t *this@<ecx>,
        const phys_vec3 *a2@<ebp>,
        phys_contact_manifold *cman);
void __thiscall phys_contact_manifold::add_feature_point(phys_contact_manifold *this, const phys_vec3 *p);
char *__thiscall phys_memory_heap::fast_allocate(phys_memory_heap *this, int size, const char *error_msg);
void __thiscall gjk_aabb_t::calc_aabb(
        gjk_aabb_t *this,
        const phys_mat44 *xform,
        phys_vec3 *aabb_min,
        phys_vec3 *aabb_max);
const cbrush_t *__thiscall gjk_aabb_t::get_brush(gjk_aabb_t *this);
void __cdecl gjk_aabb_t::destroy(gjk_aabb_t *geom);
gjk_obb_t *__cdecl gjk_obb_t::create(
        const phys_mat44 *xform,
        const phys_vec3 *dims,
        int stype,
        gjk_collision_visitor *allocator);
void __userpurge gjk_obb_t::support(
        gjk_obb_t *this@<ecx>,
        int a2@<ebp>,
        const phys_vec3 *v,
        phys_vec3 *support_vert,
        phys_vec3 *support_ind);
void __userpurge gjk_obb_t::get_simplex(
        gjk_obb_t *this@<ecx>,
        int a2@<ebp>,
        const cached_simplex_info *cache_info,
        int index_count,
        phys_vec3 *simplex_verts,
        phys_vec3 *simplex_inds);
const phys_vec3 *__thiscall gjk_obb_t::get_center(gjk_obb_t *this, phys_vec3 *result);
void __userpurge gjk_obb_t::get_feature(gjk_obb_t *this@<ecx>, const phys_vec3 *a2@<ebp>, phys_contact_manifold *cman);
void __userpurge gjk_obb_t::calc_aabb(
        gjk_obb_t *this@<ecx>,
        int a2@<ebp>,
        const phys_mat44 *xform,
        phys_vec3 *aabb_min,
        phys_vec3 *aabb_max);
void  phys_aabb_add_point(
        float a1@<ebp>,
        const phys_mat44 *xform,
        const phys_vec3 *point,
        phys_vec3 *aabb_min,
        phys_vec3 *aabb_max);
unsigned int __thiscall gjk_obb_t::get_type(gjk_obb_t *this);
void __cdecl gjk_obb_t::destroy(gjk_cylinder_t *geom);
gjk_brush_t * gjk_brush_t::create@<eax>(
        float a1@<ebp>,
        const cbrush_t *brush,
        int stype,
        gjk_collision_visitor *allocator);
void __thiscall gjk_base_t::set_contents(gjk_base_t *this, int contents);
void __thiscall gjk_brush_t::support(
        gjk_brush_t *this,
        const phys_vec3 *v,
        phys_vec3 *support_vert,
        phys_vec3 *support_ind);
void __thiscall gjk_brush_t::get_simplex(
        gjk_brush_t *this,
        const cached_simplex_info *cache_info,
        int index_count,
        phys_vec3 *simplex_verts,
        phys_vec3 *simplex_inds);
void __userpurge gjk_brush_t::get_feature(gjk_brush_t *this@<ecx>, int a2@<ebp>, phys_contact_manifold *cman);
void __userpurge gjk_brush_t::calc_aabb(
        gjk_brush_t *this@<ecx>,
        int a2@<ebp>,
        const phys_mat44 *xform,
        phys_vec3 *aabb_min,
        phys_vec3 *aabb_max);
const cbrush_t *__thiscall gjk_brush_t::get_brush(gjk_brush_t *this);
unsigned int __thiscall gjk_brush_t::get_type(gjk_brush_t *this);
char __thiscall gjk_obb_t::is_polyhedron(gjk_query_output *this);
void __cdecl gjk_brush_t::destroy(gjk_brush_t *geom);
gjk_partition_t *__cdecl gjk_partition_t::create(const CollisionAabbTree *tree, gjk_collision_visitor *allocator);
void __thiscall gjk_partition_t::support(
        gjk_partition_t *this,
        const phys_vec3 *v,
        phys_vec3 *support_vert,
        phys_vec3 *support_ind);
void __thiscall gjk_partition_t::get_simplex(
        gjk_partition_t *this,
        const cached_simplex_info *cache_info,
        int index_count,
        phys_vec3 *simplex_verts,
        phys_vec3 *simplex_inds);
const phys_vec3 *__thiscall gjk_brush_t::get_center(gjk_partition_t *this, const phys_vec3 *result);
void __userpurge gjk_partition_t::get_feature(gjk_partition_t *this@<ecx>, int a2@<ebp>, phys_contact_manifold *cman);
void __userpurge gjk_partition_t::calc_aabb(
        gjk_partition_t *this@<ecx>,
        unsigned __int16 *a2@<ebp>,
        const phys_mat44 *xform,
        phys_vec3 *aabb_min,
        phys_vec3 *aabb_max);
unsigned int __thiscall gjk_partition_t::get_type(gjk_partition_t *this);
void __cdecl gjk_partition_t::destroy(gjk_partition_t *geom);
gjk_double_sphere_t *__thiscall gjk_double_sphere_t::gjk_double_sphere_t(gjk_double_sphere_t *this);
void __thiscall gjk_double_sphere_t::support(
        gjk_double_sphere_t *this,
        const phys_vec3 *v,
        phys_vec3 *support_vert,
        phys_vec3 *support_ind);
void __thiscall gjk_double_sphere_t::get_simplex(
        gjk_double_sphere_t *this,
        const cached_simplex_info *cache_info,
        int index_count,
        phys_vec3 *simplex_verts,
        phys_vec3 *simplex_inds);
void __thiscall gjk_double_sphere_t::set_simplex(
        gjk_double_sphere_t *this,
        const phys_vec3 *simplex_inds,
        int w_set,
        const phys_vec3 *normal,
        cached_simplex_info *cache_info);
const phys_vec3 *__thiscall gjk_double_sphere_t::get_center(gjk_double_sphere_t *this, phys_vec3 *result);
void __userpurge gjk_double_sphere_t::get_feature(
        gjk_double_sphere_t *this@<ecx>,
        int a2@<ebp>,
        phys_contact_manifold *cman);
void __thiscall gjk_double_sphere_t::calc_aabb(
        gjk_double_sphere_t *this,
        const phys_mat44 *xform,
        phys_vec3 *aabb_min,
        phys_vec3 *aabb_max);
void  phys_aabb_init_sphere(
        float a1@<ebp>,
        const phys_mat44 *xform,
        const phys_vec3 *center,
        float radius,
        phys_vec3 *aabb_min,
        phys_vec3 *aabb_max);
void  phys_aabb_add_sphere(
        int a1@<ebp>,
        const phys_mat44 *xform,
        const phys_vec3 *center,
        float radius,
        phys_vec3 *aabb_min,
        phys_vec3 *aabb_max);
double __thiscall gjk_double_sphere_t::get_geom_radius(gjk_double_sphere_t *this);
bool __thiscall gjk_double_sphere_t::is_polyhedron(gjk_double_sphere_t *this);
gjk_double_sphere_t *__cdecl gjk_double_sphere_t::create(
        const phys_vec3 *c0,
        const phys_vec3 *c1,
        float r,
        int stype,
        gjk_collision_visitor *allocator);
void __cdecl gjk_double_sphere_t::destroy(gjk_double_sphere_t *geom);
gjk_cylinder_t *__cdecl gjk_cylinder_t::create(
        int _direction,
        float _halfHeight,
        float _radius,
        const phys_mat44 *_xform,
        int stype,
        gjk_collision_visitor *allocator);
void __userpurge gjk_cylinder_t::support(
        gjk_cylinder_t *this@<ecx>,
        int a2@<ebp>,
        const phys_vec3 *v,
        phys_vec3 *support_vert,
        phys_vec3 *support_ind);
const phys_vec3 *__userpurge gjk_cylinder_t::get_dims@<eax>(
        gjk_cylinder_t *this@<ecx>,
        int a2@<ebp>,
        const phys_vec3 *result);
void __userpurge gjk_cylinder_t::get_simplex(
        gjk_cylinder_t *this@<ecx>,
        int a2@<ebp>,
        const cached_simplex_info *cache_info,
        int index_count,
        phys_vec3 *simplex_verts,
        phys_vec3 *simplex_inds);
const phys_vec3 *__userpurge gjk_cylinder_t::get_center@<eax>(
        gjk_cylinder_t *this@<ecx>,
        int a2@<ebp>,
        const phys_vec3 *result);
void __userpurge gjk_cylinder_t::get_feature(gjk_cylinder_t *this@<ecx>, int a2@<ebp>, phys_contact_manifold *cman);
void __userpurge gjk_cylinder_t::calc_aabb(
        gjk_cylinder_t *this@<ecx>,
        int a2@<ebp>,
        const phys_mat44 *xform_,
        phys_vec3 *aabb_min,
        phys_vec3 *aabb_max);
unsigned int __thiscall gjk_cylinder_t::get_type(gjk_cylinder_t *this);
double __thiscall gjk_cylinder_t::get_geom_radius(gjk_cylinder_t *this);
void __cdecl gjk_cylinder_t::destroy(gjk_cylinder_t *geom);
void  setup_gjk_polygon_cylinder(
        float a1@<ebp>,
        float *mins,
        float *maxs,
        float radius_adjust,
        gjk_polygon_cylinder_t *gjk_cylinder);
gjk_polygon_cylinder_t *__cdecl gjk_polygon_cylinder_t::create(
        float (*mins)[3],
        float (*maxs)[3],
        float radius_adjust,
        int stype,
        gjk_collision_visitor *allocator);
void __cdecl gjk_polygon_cylinder_t::destroy(gjk_polygon_cylinder_t *geom);
void __cdecl destroy_gjk_geom(gjk_aabb_t *geom);
gjk_aabb_t * create_aabb_gjk_geom@<eax>(
        float a1@<ebp>,
        const float *mn,
        const float *mx,
        int stype,
        gjk_collision_visitor *allocator);
gjk_obb_t * create_obb_gjk_geom@<eax>(
        float a1@<ebp>,
        float (*orientation)[3],
        float *offset,
        float *halfLengths,
        int stype,
        gjk_collision_visitor *allocator);
gjk_brush_t *__cdecl create_brush_gjk_geom(const cbrush_t *brush, int stype, gjk_collision_visitor *allocator);
gjk_double_sphere_t * create_capsule_gjk_geom@<eax>(
        float a1@<ebp>,
        float *center,
        float radius,
        float halfHeight,
        unsigned int direction,
        int stype,
        gjk_collision_visitor *allocator);
gjk_cylinder_t * create_cylinder_gjk_geom@<eax>(
        float a1@<ebp>,
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
void __thiscall query_brush_model_gjk_geom_visitor::visit(
        query_brush_model_gjk_geom_visitor *this,
        const cbrush_t *brush);
void __thiscall query_brush_model_gjk_geom_visitor::update(
        query_brush_model_gjk_geom_visitor *this,
        const float *_mn,
        const float *_mx,
        int _mask,
        const float *expand_vec);
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
void __thiscall gjk_base_t::set_xform(gjk_base_t *this, const phys_mat44 *xform);
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
gjk_aabb_t *__thiscall phys_simple_allocator<gjk_aabb_t>::allocate(phys_simple_allocator<gjk_aabb_t> *this);
gjk_obb_t *__thiscall phys_simple_allocator<gjk_obb_t>::allocate(phys_simple_allocator<gjk_obb_t> *this);
gjk_brush_t *__thiscall phys_simple_allocator<gjk_brush_t>::allocate(phys_simple_allocator<gjk_brush_t> *this);
void __thiscall phys_simple_allocator<gjk_brush_t>::free(phys_simple_allocator<gjk_brush_t> *this, gjk_brush_t *slot);
gjk_partition_t *__thiscall phys_simple_allocator<gjk_partition_t>::allocate(
        phys_simple_allocator<gjk_partition_t> *this);
void __thiscall phys_simple_allocator<gjk_partition_t>::free(
        phys_simple_allocator<gjk_partition_t> *this,
        gjk_partition_t *slot);
gjk_double_sphere_t *__thiscall phys_simple_allocator<gjk_double_sphere_t>::allocate(
        phys_simple_allocator<gjk_double_sphere_t> *this);
void __thiscall phys_simple_allocator<gjk_double_sphere_t>::free(
        phys_simple_allocator<gjk_double_sphere_t> *this,
        gjk_double_sphere_t *slot);
gjk_cylinder_t *__thiscall phys_simple_allocator<gjk_cylinder_t>::allocate(phys_simple_allocator<gjk_cylinder_t> *this);
void __thiscall phys_simple_allocator<gjk_cylinder_t>::free(
        phys_simple_allocator<gjk_cylinder_t> *this,
        gjk_cylinder_t *slot);
gjk_polygon_cylinder_t *__thiscall phys_simple_allocator<gjk_polygon_cylinder_t>::allocate(
        phys_simple_allocator<gjk_polygon_cylinder_t> *this);
void __thiscall phys_simple_allocator<gjk_polygon_cylinder_t>::free(
        phys_simple_allocator<gjk_polygon_cylinder_t> *this,
        gjk_polygon_cylinder_t *slot);
void __thiscall gjk_polygon_cylinder_t::poly_verts::poly_verts(gjk_polygon_cylinder_t::poly_verts *this);
void __cdecl destroy_gjk_geom(gjk_geom_list_t *gjk_geom_list);
