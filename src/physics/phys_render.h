#pragma once

float *__thiscall phys_vec3::operator[]<int>(phys_vec3 *this, unsigned int i);
void  make_rotate(
        int a1@<ebp>,
        phys_mat44 *mat,
        const phys_vec3 *v,
        float theta_factor,
        float max_rotation_radians);
void __cdecl make_rotate(phys_mat44 *m, const phys_vec3 *u, float ca, float sa);
void __cdecl DebugPatchesAndBrushesProlog();
void __cdecl DebugPatchesAndBrushesEpilog();
void __cdecl clear_debug_brushes_and_patches();
void __cdecl add_debug_brush(const cbrush_t *brush, const phys_mat44 *mat);
void __userpurge debug_brush_info_t::add_brush(
        debug_brush_info_t *this@<ecx>,
        int a2@<ebp>,
        const cbrush_t *brush,
        const phys_mat44 *mat);
__int16 __thiscall debug_brush_info_t::find_index_in_brush(
        debug_brush_info_t *this,
        const cbrush_t *brush,
        const float *v);
void __cdecl add_debug_patch(const CollisionAabbTree *tree);
void __thiscall debug_patch_info_t::add_patch(debug_patch_info_t *this, const CollisionAabbTree *tree);
unsigned int __thiscall debug_patch_info_t::find_index_in_clipmap(
        debug_patch_info_t *this,
        const CollisionAabbTree *tree,
        const float *v);
void __cdecl render_debug_brushes();
void __cdecl RB_CheckTessOverflow(int vertexCount, int indexCount);
void __cdecl render_debug_patches_full();
void __cdecl render_debug_brushes_and_patches();
void  render_brush(
        float a1@<ebp>,
        const cbrush_t *brush,
        const phys_mat44 *xform,
        const float *color,
        bool bLinesOnly,
        const float *lightPos,
        bool persistent,
        bool ztest,
        const float *edgeColor);
void __cdecl calc_color(unsigned int id, float alpha, float *color);
void __cdecl render_convex_partition(const CollisionAabbTree *tree);
void  render_chull(float a1@<ebp>, const chull_t *first, const phys_mat44 *xform, const float *color);
void __cdecl render_xmodel_chull(const XModel *model, unsigned int key, const phys_mat44 *xform, const float *color);
void __cdecl render_brushmodel_chull(
        unsigned int brushmodel,
        unsigned int key,
        const phys_mat44 *xform,
        const float *color);
void __cdecl render_line(const float *p0, const float *p1, const float *color, int duration, int ztest);
void __cdecl render_line(const phys_vec3 *p0, const phys_vec3 *p1, const float *color, int duration, int ztest);
void  render_box(
        int a1@<ebp>,
        const phys_vec3 *mins,
        const phys_vec3 *maxs,
        const phys_mat44 *xform,
        const float *color,
        int duration);
void  render_box(float a1@<ebp>, float *_mn, float *_mx, const float *color, int duration);
void __cdecl render_box(const phys_vec3 *mins, const phys_vec3 *maxs, const float *color, int duration);
// bad sp value at call has been detected, the output may be wrong!
void  render_gjk_geom(float a1@<ebp>, gjk_base_t *geom, const phys_mat44 *cg2w);
void  debug_render(int a1@<ebp>, PhysObjUserData *userData);
void  clip_winding(int a1@<ebp>, phys_static_array<phys_vec3,512> *winding, const plane_lt *clip);
void  init_winding(float a1@<ebp>, const plane_lt *plane, phys_static_array<phys_vec3,512> *winding);
// local variable allocation has failed, the output may be wrong!
void  calc_winding(
        int a1@<ebp>,
        phys_static_array<plane_lt,512> *planes,
        int plane_index,
        phys_static_array<phys_vec3,512> *winding);
void __cdecl Phys_DebugPoint(const phys_vec3 *pos, float radius, const float *color);
void __cdecl Phys_DebugBox(const phys_mat44 *mat, const float *color, float scale);
void __cdecl Phys_DebugBox(const phys_mat44 *mat, const phys_vec3 *dims, const float *color);
void  Phys_RenderBase(float a1@<ebp>, const phys_mat44 *mat, float scale);
void  Phys_JointDebugRender(rigid_body *a1@<ebp>, int id);
void  make_rotate(float a1@<ebp>, phys_mat44 *mat, const phys_vec3 *v1, const phys_vec3 *v2);
void  render_contact(contact_point_info *a1@<ebp>, rigid_body_constraint_contact *rbc);
void __cdecl render_collision_tree(const CollisionAabbTree *tree, const float *color);
void __cdecl render_prims(col_prim_t *prims, int nprims);
void __cdecl render_debug_draw_gjk_trace_geom();
void __cdecl GjkTraceGeom::Render();
plane_lt *__thiscall phys_static_array<plane_lt,512>::add(
        phys_static_array<plane_lt,512> *this,
        int no_error,
        const char *error_msg);
const plane_lt *__thiscall phys_static_array<plane_lt,512>::operator[](phys_static_array<plane_lt,512> *this, int i);
phys_vec3 *__thiscall phys_static_array<phys_vec3,512>::add(
        phys_static_array<phys_vec3,512> *this,
        int no_error,
        const char *error_msg);
phys_vec3 *__thiscall phys_static_array<phys_vec3,512>::operator[](phys_static_array<phys_vec3,512> *this, int i);
int *__thiscall phys_static_array<float,512>::operator[](phys_static_array<int,512> *this, int i);
void __thiscall phys_static_array<float,512>::call_destructors(phys_static_array<int,512> *this);
const float *__thiscall phys_vec3::operator[]<int>(phys_vec3 *this, unsigned int i);
