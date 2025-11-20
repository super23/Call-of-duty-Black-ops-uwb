#pragma once

void __cdecl destroy_gjk_geom(gjk_aabb_t *geom);
void __cdecl Phys_Init();
void __cdecl surface_type_info_database_set(int surface_type_1, int surface_type_2, const phys_surface_type_info *pst);
void __cdecl create_broad_phase_info(rigid_body *body);
unsigned int __thiscall rigid_body::get_flag(rigid_body *this, unsigned int f);
void __thiscall broad_phase_group::set(broad_phase_group *this);
void __thiscall broad_phase_group::add_bpi(broad_phase_group *this, broad_phase_info *bpi);
void __cdecl environment_collision_list_add(broad_phase_base *bpb);
void __cdecl environment_collision_list_remove(broad_phase_base *bpb);
void __thiscall broad_phase_memory::list_bpb_remove(broad_phase_memory *this, broad_phase_base *bpb_to_remove);
int __thiscall gjk_geom_list_t::get_geom_count(gjk_geom_list_t *this);
void __cdecl destroy_broad_phase_info(rigid_body *body);
// local variable allocation has failed, the output may be wrong!
PhysObjUserData * Phys_CreateUserBody@<eax>(
        float a1@<ebp>,
        float *position,
        int id,
        PhysicsGeomType geomType);
PhysObjUserData *__cdecl Phys_CreateUserData(int worldIndex);
PhysObjUserData * Phys_CreateBodyFromState@<eax>(
        int a1@<ebp>,
        int worldIndex,
        const BodyState *state,
        gjk_geom_list_t *gjk_geom_list,
        bool do_collision_test);
void  phys_full_inverse(int a1@<ebp>, phys_mat44 *dest, const phys_mat44 *source);
void __cdecl destroy_gjk_geom(gjk_geom_list_t *gjk_geom_list);
PhysObjUserData *__cdecl Phys_ObjCreate(
        int worldIndex,
        const float *position,
        const float *quat,
        const float *velocity,
        const PhysPreset *physPreset,
        gjk_geom_list_t *gjk_geom_list,
        bool do_collision_test,
        int id);
PhysObjUserData *__cdecl Phys_ObjCreateAxis(
        int worldIndex,
        const float *position,
        const float (*axis)[3],
        const float *velocity,
        const PhysPreset *physPreset,
        gjk_geom_list_t *gjk_geom_list,
        int id,
        bool do_collision_test);
void  Phys_ObjSetPosition(int a1@<ebp>, int id, float *newPosition);
void __cdecl Phys_ObjSetOrientation(int id, const float *newPosition, const float *newOrientation);
void  Phys_ObjSetAngularVelocity(int a1@<ebp>, int id, float *angularVel);
void __thiscall rigid_body::dangerous_set_a_vel(rigid_body *this, const phys_vec3 *a_vel);
void  Phys_ObjSetVelocity(int a1@<ebp>, int id, float *velocity);
void __thiscall rigid_body::dangerous_set_t_vel(rigid_body *this, const phys_vec3 *t_vel);
void  Phys_ObjSetAngularVelocityRaw(int a1@<ebp>, int id, float *angularVel);
void  Phys_ObjGetPosition(int a1@<ebp>, int id, float *outPosition, float (*outRotation)[3]);
void __cdecl Phys_ObjGetVelocities(int id, float *tvel, float *avel);
void __cdecl Phys_ObjGetCenterOfMass(int id, float *outPosition);
void __cdecl Phys_ObjAddCollFlags(int physObjId, int collFlags);
void __cdecl Phys_ObjRemoveCollFlags(int physObjId, int collFlags);
void __cdecl fixup_wheel_constraints(rigid_body *rb);
void __cdecl Phys_ObjDestroy(int worldIndex, int id);
void __cdecl Phys_DestroyUserData(int worldIndex, PhysObjUserData *userData);
void  Phys_AddCacheImpulses(int a1@<ebp>);
void __cdecl Phys_ObjAddCustomForce(
        int physObjId,
        const float *hitPos,
        const float *hitDir,
        int mod,
        PhysPreset *physPreset);
void  Phys_ObjAddForce(int a1@<ebp>, int id, float *worldPos, float *impulse, bool relative);
void  Phys_ObjAddTorque(int a1@<ebp>, int id, float *torque);
void __thiscall rigid_body::add_torque(rigid_body *this, const phys_vec3 *torque);
void __cdecl Phys_ObjBulletImpact(
        int id,
        const float *worldPosRaw,
        const float *bulletDirRaw,
        float bulletSpeed,
        float scale);
void __cdecl Phys_TweakBulletImpact(float *worldPos, float *bulletDir, const float *centerOfMass);
int __cdecl Phys_ObjGetSnapshot(int id, float *outPos, float (*outMat)[3]);
void __cdecl Phys_RenderBulletMeshInfo(
        float *pos,
        const XModel *model,
        const char *type,
        int numTriangles,
        float avgArea);
void __cdecl Phys_ComputeStringPosition(const float *start, const float *end, float *position);
void __cdecl Phys_FindAndRenderBulletMesh(const float *start, const float *end);
chull_t *__cdecl create_chull(phys_convex_hull *pch);
void __cdecl free_chull(chull_t *first);
void __cdecl chull_list_add(chull_t *chull, unsigned int key);
void __cdecl free_chull_lists();
chull_t * generate_brush_chull@<eax>(int a1@<ebp>, const cbrush_t *brush);
void __userpurge phys_convex_hull::compute_convex_hull(
        phys_convex_hull *this@<ecx>,
        int a2@<ebp>,
        int max_verts,
        float min_expansion_volume_percent);
void __thiscall phys_convex_hull::add_convex_hull_vert(phys_convex_hull *this, phys_vec3 **vert);
void __userpurge phys_convex_hull::create_intermediate_triangle(
        phys_convex_hull *this@<ecx>,
        int a2@<ebp>,
        phys_vec3 *v0,
        phys_vec3 *v1,
        phys_vec3 *v2);
phys_vec3 **__thiscall phys_convex_hull::support_intermediate_verts(phys_convex_hull *this, const phys_vec3 *dir);
void __thiscall phys_convex_hull::init_convex_hull(phys_convex_hull *this);
void  phys_convex_hull::calculate_initial_triangle_vertices(phys_convex_hull *this@<ecx>, int a2@<ebp>);
double __thiscall phys_convex_hull::calc_expansion_volume(phys_convex_hull *this, const phys_vec3 *vert);
double __thiscall phys_convex_hull::ch_triangle::get_dist(phys_convex_hull::ch_triangle *this, const phys_vec3 *vert);
double __userpurge phys_convex_hull::tetrahedron_volume@<st0>(
        phys_convex_hull *this@<ecx>,
        int a2@<ebp>,
        const phys_vec3 *a,
        const phys_vec3 *b,
        const phys_vec3 *c,
        const phys_vec3 *d);
void __thiscall phys_convex_hull::create_edge_list(phys_convex_hull *this, const phys_vec3 *vert);
void __thiscall phys_convex_hull::add_triangle_edges(phys_convex_hull *this, phys_convex_hull::ch_triangle *tri);
void __thiscall phys_convex_hull::add_intermediate_edge(phys_convex_hull *this, phys_vec3 *v0, phys_vec3 *v1);
void __thiscall phys_convex_hull::remove_inside_verts(phys_convex_hull *this);
phys_convex_hull::ch_triangle *__thiscall phys_convex_hull::ch_triangle::operator=(
        phys_convex_hull::ch_triangle *this,
        const phys_convex_hull::ch_triangle *__that);
chull_t * generate_partition_chull@<eax>(const float *a1@<ebp>, const CollisionAabbTree *tree);
void __cdecl generate_brushmodel_chull_r(cLeafBrushNode_s *node, chull_t **hull);
chull_t *__cdecl generate_brushmodel_chull(unsigned int brushmodel);
chull_t *__cdecl generate_collmap_chull(PhysGeomList *geomList);
chull_t *__cdecl get_collmap_chull(PhysGeomList *geomList, unsigned int key);
chull_t *__cdecl get_brushmodel_chull(unsigned int brushmodel, unsigned int key);
chull_t *__cdecl get_brush_chull(const cbrush_t *brush);
chull_t *__cdecl get_partition_chull(const CollisionAabbTree *tree);
void  Phys_FindAndRenderEntityBrushes(int a1@<ebp>, const float *pos, int contentmask);
char __cdecl Phys_DebugRenderChanged();
int __cdecl Phys_GetMaskFromDVar();
void __cdecl Phys_RenderWorldCollMesh(const float *pos, bool bRenderStaticCollision, bool bRenderBrushes);
void __thiscall colgeom_debug_renderer_t::reset(colgeom_debug_renderer_t *this);
void __thiscall colgeom_debug_renderer_t::visit(colgeom_debug_renderer_t *this, const CollisionAabbTree *tree);
void __thiscall colgeom_debug_renderer_t::visit(colgeom_debug_renderer_t *this, const cbrush_t *brush);
void __thiscall colgeom_debug_renderer_t::update(
        colgeom_debug_renderer_t *this,
        const float *_mn,
        const float *_mx,
        int mask,
        const float *expand_vec);
void __cdecl debug_loop();
void __cdecl print_smodel(cStaticModel_s *smodel, int refcount, const float *dims);
void __cdecl print_static_models_stats();
int __cdecl SortModelsFunc(const void *a, const void *b);
void __cdecl draw_static_models_bounds();
int __cdecl SortSModelsByDist(float *a, float *b);
int __cdecl buoyancy_worker();
void __cdecl Phys_BodyGrabSnapshotNitrous(PhysObjUserData *userData, float deltaT);
void  UpdateRigidBody(float a1@<ebp>, float delta_t);
void __cdecl Phys_DebugRender(PhysObjUserData *userData);
void  Phys_CollisionCallback(int a1@<ebp>);
void __thiscall cdl_proftimer::start_capture(cdl_proftimer *this);
char __cdecl Phys_ShouldCollideCallback(const broad_phase_base *bpi1, const broad_phase_base *bpi2);
void __cdecl PhysicsSystem_Update();
void __cdecl Phys_RunToTime(int timeNow);
void __cdecl Phys_ObjGetInterpolatedState(int id, float *outPos, float *outQuat);
void  Phys_SetUserBody(int a1@<ebp>, int id, float *position);
bool __cdecl Phys_ObjIsAsleep(int id);
unsigned int __thiscall rigid_body::is_dangerous(rigid_body *this);
bool __cdecl Phys_ObjIsAsleepSingle(int id);
unsigned int __thiscall rigid_body::is_stable(rigid_body *this);
void __cdecl Phys_SetHingeParams(rigid_body_constraint_ragdoll *id, float motorSpeed, float damp);
rigid_body_constraint_ragdoll * Phys_CreateHinge@<eax>(
        rigid_body_constraint_ragdoll *a1@<ebp>,
        int obj1,
        int obj2,
        const float *anchor,
        float *axis,
        float motorSpeed,
        float damp,
        float lowStop,
        float highStop);
void __cdecl Phys_SetAngularMotorParams(rigid_body_constraint_ragdoll *id, const float *motorSpeeds, float *damp);
rigid_body_constraint_ragdoll * Phys_CreateSwivel@<eax>(
        rigid_body_constraint_ragdoll *a1@<ebp>,
        int obj1,
        int obj2,
        const float *anchor,
        unsigned int numAxes,
        float (*axes)[3],
        const float *motorSpeeds,
        const float *motorFMaxs,
        float *lowStops,
        float *highStops);
// local variable allocation has failed, the output may be wrong!
void  phys_inv_multiply_mat(int a1@<ebp>, phys_mat44 *dest, const phys_mat44 *left, const phys_mat44 *right);
void __cdecl Phys_CalcPreset(PhysPreset *physPreset, float *dims, int surfaceType);
int __cdecl Phys_GetCurrentTime();
PhysObjUserData *__thiscall phys_free_list<PhysObjUserData>::add(
        phys_free_list<PhysObjUserData> *this,
        int no_error,
        const char *error_msg);
void __thiscall phys_free_list<PhysObjUserData>::remove(phys_free_list<PhysObjUserData> *this, PhysObjUserData *data_);
void __thiscall phys_link_list1<PhysObjUserData>::add(phys_link_list1<PhysObjUserData> *this, PhysObjUserData *p);
void __thiscall phys_link_list1<PhysObjUserData>::remove(phys_link_list1<PhysObjUserData> *this, PhysObjUserData *p);
phys_vec3 *__thiscall phys_static_array<phys_vec3,6144>::add(
        phys_static_array<phys_vec3,6144> *this,
        int no_error,
        const char *error_msg);
phys_vec3 *__thiscall phys_static_array<phys_vec3,6144>::operator[](phys_static_array<phys_vec3,6144> *this, int i);
phys_convex_hull::ch_triangle *__thiscall phys_static_array<phys_convex_hull::ch_triangle,256>::add(
        phys_static_array<phys_convex_hull::ch_triangle,256> *this,
        int no_error,
        const char *error_msg);
void __thiscall phys_static_array<phys_convex_hull::ch_triangle,256>::remove_slow(
        phys_static_array<phys_convex_hull::ch_triangle,256> *this,
        phys_convex_hull::ch_triangle *data);
phys_convex_hull::ch_triangle *__thiscall phys_static_array<phys_convex_hull::ch_triangle,256>::get_list_head(
        phys_static_array<phys_convex_hull::ch_triangle,256> *this);
void __thiscall phys_static_array<phys_convex_hull::ch_edge,128>::remove_slow(
        phys_static_array<phys_convex_hull::ch_edge,128> *this,
        phys_convex_hull::ch_edge *data);
phys_convex_hull::ch_triangle *__thiscall phys_static_array<phys_convex_hull::ch_triangle,64>::add(
        phys_static_array<phys_convex_hull::ch_triangle,64> *this,
        int no_error,
        const char *error_msg);
void __thiscall phys_free_list<PhysObjUserData>::debug_add(
        phys_free_list<PhysObjUserData> *this,
        phys_free_list<PhysObjUserData>::T_internal *T_i);
void __thiscall phys_free_list<PhysObjUserData>::remove(
        phys_free_list<PhysObjUserData> *this,
        phys_free_list<PhysObjUserData>::T_internal *data);
void __thiscall phys_free_list<PhysObjUserData>::debug_remove(
        phys_free_list<PhysObjUserData> *this,
        phys_free_list<PhysObjUserData>::T_internal *T_i);
cdl_proftimer *__thiscall cdl_proftimer::cdl_proftimer(cdl_proftimer *this);
PhysGlob *__thiscall PhysGlob::PhysGlob(PhysGlob *this);
phys_convex_hull *__thiscall phys_convex_hull::phys_convex_hull(phys_convex_hull *this);
void __thiscall phys_free_list<PhysObjUserData>::~phys_free_list<PhysObjUserData>(
        phys_free_list<PhysObjUserData> *this);
void __thiscall phys_free_list<VehicleParameter>::~phys_free_list<VehicleParameter>(
        phys_free_list<VehicleParameter> *this);
void __thiscall phys_free_list<VehicleParameter>::remove(
        phys_free_list<VehicleParameter> *this,
        phys_free_list<VehicleParameter>::T_internal *data);
void __thiscall phys_free_list<VehicleParameter>::debug_remove(
        phys_free_list<VehicleParameter> *this,
        phys_free_list<VehicleParameter>::T_internal *T_i);
void __thiscall phys_free_list<NitrousVehicle>::~phys_free_list<NitrousVehicle>(phys_free_list<NitrousVehicle> *this);
void __thiscall phys_free_list<RagdollBody>::~phys_free_list<RagdollBody>(phys_free_list<RagdollBody> *this);
void __thiscall phys_free_list<RagdollBody>::remove(
        phys_free_list<RagdollBody> *this,
        phys_free_list<RagdollBody>::T_internal *data);
void __thiscall phys_free_list<RagdollBody>::debug_remove(
        phys_free_list<RagdollBody> *this,
        phys_free_list<RagdollBody>::T_internal *T_i);
void __thiscall phys_convex_hull::~phys_convex_hull(phys_convex_hull *this);
void __thiscall phys_free_list<RagdollBody>::remove(phys_free_list<RagdollBody> *this, RagdollBody *data_);
broad_phase_info *__cdecl create_broad_phase_info();
void __cdecl destroy_broad_phase_info(broad_phase_info *bpi);
