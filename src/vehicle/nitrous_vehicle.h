#pragma once

VehicleParameter *__thiscall VehicleParameter::VehicleParameter(VehicleParameter *this);
NitrousVehicle *__thiscall NitrousVehicle::NitrousVehicle(NitrousVehicle *this);
void __thiscall NitrousVehicle::init(NitrousVehicle *this, gentity_s *owner, const VehicleParameter *parameter);
void __thiscall NitrousVehicle::set_throttle(NitrousVehicle *this, float throttle);
void __thiscall NitrousVehicle::init(
        NitrousVehicle *this,
        int localClientNum,
        centity_s *owner,
        const VehicleParameter *parameter);
PhysObjUserData *__cdecl Phys_ObjCreateNitrousVehicle(
        const float *position,
        const float *angles,
        const float *velocity,
        const float *translation,
        const PhysPreset *physPreset,
        gjk_geom_list_t *gjk_geom_list);
void  NitrousVehicle::unpause_physics(NitrousVehicle *this@<ecx>, int a2@<ebp>);
void __thiscall NitrousVehicleController::Init(NitrousVehicleController *this);
void __thiscall NitrousVehicle::pause_physics(NitrousVehicle *this, bool shutdown);
void __thiscall NitrousVehicle::_update_pause(NitrousVehicle *this);
void __thiscall rigid_body::set_flag(rigid_body *this, unsigned int f, int b);
unsigned int __thiscall rigid_body::is_group_stable(rigid_body *this);
void __userpurge NitrousVehicle::update_parms(
        NitrousVehicle *this@<ecx>,
        int a2@<ebp>,
        const VehicleParameter *params,
        bool initialization);
// bad sp value at call has been detected, the output may be wrong!
void __userpurge gjk_geom_list_t::comp_aabb_loc(
        gjk_geom_list_t *this@<ecx>,
        int a2@<ebp>,
        phys_vec3 *aabb_mn_loc,
        phys_vec3 *aabb_mx_loc);
// local variable allocation has failed, the output may be wrong!
void __userpurge NitrousVehicle::_setup_wheels(NitrousVehicle *this@<ecx>, DObj *a2@<ebp>, int localClientNum);
void __cdecl Phys_Mat4ToNitrousMat(float (*inMat)[3], phys_mat44 *outMat);
double __thiscall NitrousVehicle::_calc_initial_susp_spring_k(
        NitrousVehicle *this,
        rigid_body_constraint_wheel *wheel_constraint);
bool __thiscall NitrousVehicle::is_peeling_out(NitrousVehicle *this);
void __thiscall NitrousVehicle::destructible_damage(NitrousVehicle *this, const char *notifyLabel, int damageStage);
void __thiscall NitrousVehicle::damage(NitrousVehicle *this, unsigned int boneName, int damageStage);
void __thiscall NitrousVehicle::damage(NitrousVehicle *this, int damage, const float *point, const float *dir, int mod);
phys_vec3 *__thiscall NitrousVehicle::get_velocity(NitrousVehicle *this, phys_vec3 *result);
phys_vec3 *__thiscall NitrousVehicle::get_a_velocity(NitrousVehicle *this, phys_vec3 *result);
void __thiscall NitrousVehicle::_update_orientation_constraint(NitrousVehicle *this);
double __thiscall NitrousVehicle::_get_stuck_accel_factor(NitrousVehicle *this, float delta_t);
void __thiscall NitrousVehicle::_update_boost(NitrousVehicle *this, float delta_t);
void __userpurge NitrousVehicle::_update_prolog(NitrousVehicle *this@<ecx>, int a2@<ebp>, float delta_t);
void __thiscall NitrousVehicle::_update_epilog(NitrousVehicle *this, float delta_t);
void __thiscall NitrousVehicle::_update_friction(NitrousVehicle *this, float delta_t);
void __thiscall NitrousVehicle::_update_fakey_stuff(NitrousVehicle *this, float delta_t);
void __thiscall NitrousVehicle::start_path(NitrousVehicle *this, int attach_mode);
void __thiscall NitrousVehicle::end_path(NitrousVehicle *this);
void __thiscall NitrousVehicle::cleanup_path(NitrousVehicle *this);
void __cdecl NitrousVehicle::frame_prolog_all_systems(float delta_t);
void __cdecl NitrousVehicle::frame_epilog_all_systems(float delta_t);
NitrousVehicle **__cdecl NitrousVehicle::add_vehicle(NitrousVehicle *id);
void __cdecl NitrousVehicle::reinit_parms();
void __cdecl NitrousVehicle::reset_vehicle_physics();
void __cdecl NitrousVehicle::remove_vehicle(NitrousVehicle *const v);
void __cdecl NitrousVehicle::debug_render_all();
void  NitrousVehicle::debug_render(NitrousVehicle *this@<ecx>, int a2@<ebp>);
// local variable allocation has failed, the output may be wrong!
void  phys_full_multiply_mat(int a1@<ebp>, phys_mat44 *dest, const phys_mat44 *left, const phys_mat44 *right);
void __thiscall NitrousVehicle::remove_wheels(NitrousVehicle *this);
void __thiscall NitrousVehicle::set(
        NitrousVehicle *this,
        float power_braking_factor,
        float braking_factor,
        float desired_speed_factor,
        float acceleration_factor,
        float coasting_factor,
        float reference_wheel_radius,
        float steer_max_angle,
        float steer_speed,
        phys_vec3 *steer_front_pt_loc,
        float steer_front_back_length);
// local variable allocation has failed, the output may be wrong!
void __userpurge NitrousVehicle::update_steering(NitrousVehicle *this@<ecx>, float a2@<ebp>, float delta_t);
void __thiscall NitrousVehicle::update_braking_and_acceleration(NitrousVehicle *this, float __formal);
phys_vec3 *__cdecl phys_cross(phys_vec3 *result, const phys_vec3 *a, const phys_vec3 *b);
void __thiscall NitrousVehicle::set_boosting(NitrousVehicle *this, bool boosting);
bool __thiscall NitrousVehicle::is_boosting(NitrousVehicle *this);
double __thiscall NitrousVehicle::get_maximum_acceleration_factor(NitrousVehicle *this);
double __thiscall NitrousVehicle::get_maximum_acceleration(NitrousVehicle *this);
double __thiscall NitrousVehicle::get_max_speed(NitrousVehicle *this, bool include_perks);
void __thiscall NitrousVehicle::update_control_from_network(NitrousVehicle *this, LerpEntityStateVehicle *vehicleState);
// local variable allocation has failed, the output may be wrong!
void __userpurge NitrousVehicle::update_from_network(
        NitrousVehicle *this@<ecx>,
        phys_vec3 *a2@<ebp>,
        trajectory_t pos,
        trajectory_t apos,
        LerpEntityStateVehicle *vehicleState,
        float timeDifference);
double __cdecl AbsSquared(const phys_vec3 *a);
// local variable allocation has failed, the output may be wrong!
void  phys_multiply_mat(int a1@<ebp>, phys_mat44 *dest, const phys_mat44 *left, const phys_mat44 *right);
void  make_rotate(
        int a1@<ebp>,
        phys_mat44 *mat,
        const phys_vec3 *v,
        float theta_factor,
        float max_rotation_radians);
void __cdecl make_rotate(phys_mat44 *m, const phys_vec3 *u, float ca, float sa);
void __thiscall phys_mat44::fix_w_column(phys_mat44 *this);
void __cdecl Phys_NitrousMat44ToVec33(const phys_mat44 *inMat, float (*outAxis)[3]);
bool __cdecl CompareVec3ToPhysVec(const float *a, const phys_vec3 *b, float acceptableError);
void __thiscall NitrousVehicle::GetEntityPose(
        NitrousVehicle *this,
        float *origin,
        float *angles,
        int curTime,
        bool isDriver);
void __cdecl G_SetVehDriverInputs(int localClientNum, int vehEntNum, usercmd_s *cmd);
void __cdecl G_ClearVehicleInputs();
void __cdecl CG_SetVehDriverInputs(int localClientNum, int vehEntNum, usercmd_s *cmd);
void __cdecl Vehicle_Launch(int localClientNum, centity_s *cent, float *hitp, const float *force, bool relative);
NitrousVehicle *__thiscall phys_free_list<NitrousVehicle>::add(
        phys_free_list<NitrousVehicle> *this,
        int no_error,
        const char *error_msg);
void __thiscall phys_free_list<NitrousVehicle>::remove(phys_free_list<NitrousVehicle> *this, NitrousVehicle *data_);
void __thiscall phys_free_list<NitrousVehicle>::debug_add(
        phys_free_list<NitrousVehicle> *this,
        phys_free_list<NitrousVehicle>::T_internal *T_i);
void __thiscall phys_free_list<NitrousVehicle>::remove(
        phys_free_list<NitrousVehicle> *this,
        phys_free_list<NitrousVehicle>::T_internal *data);
void __thiscall phys_free_list<NitrousVehicle>::debug_remove(
        phys_free_list<NitrousVehicle> *this,
        phys_free_list<NitrousVehicle>::T_internal *T_i);
void  make_rotate(float a1@<ebp>, phys_mat44 *mat, const phys_vec3 *v1, const phys_vec3 *v2);
phys_mat44 *__thiscall phys_mat44::phys_mat44(
        phys_mat44 *this,
        const phys_vec3 *x_,
        const phys_vec3 *y_,
        const phys_vec3 *z_,
        const phys_vec3 *w_);
