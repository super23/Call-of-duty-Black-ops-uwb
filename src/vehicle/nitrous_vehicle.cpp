#include "nitrous_vehicle.h"
#include <xanim/dobj.h>
#include <bgame/bg_public.h>
#include <game/g_scr_vehicle.h>
#include <qcommon/dobj_management.h>
#include <xanim/dobj_utils.h>
#include <physics/phys_assert.h>
#include <client_mp/cl_main_mp.h>
#include <cgame_mp/cg_actors_mp.h>
#include <cgame_mp/cg_ents_mp.h>
#include <cgame_mp/cg_vehicles_mp.h>
#include <physics/physpreset_load_obj.h>
#include <physics/phys_broad_phase.h>
#include <physics/physics_system.h>
#include <bgame/bg_slidemove.h>
#include <physics/phys_util.h>
#include <clientscript/cscr_stringlist.h>
#include <ragdoll/ragdoll_update.h>
#include <clientscript/scr_const.h>
#include <game_mp/g_utils_mp.h>
#include "nitrous_vehicle_constraint.h"
#include "nitrous_vehicle_effects.h"
#include <sound/snd_dvar.h>
#include <server/sv_world.h>
#include <physics/phys_render.h>
#include <game/g_debug.h>
#include <game_mp/g_main_mp.h>
#include <server_mp/sv_main_mp.h>
#include <universal/com_math_anglevectors.h>
#include <DynEntity/DynEntity_client.h>
#include <client/cl_debugdata.h>
#include <cgame/cg_drawtools.h>
#include <xanim/xanim_clientnotify.h>
#include <cgame_mp/cg_local_mp.h>
#include <bgame/bg_misc.h>

float S_HEAVY_TILT_FAKEY = 0.40000001;
float S_HEAVY_TANK_ACCEL = 15.0;
float S_HEAVY_TILT_FAKEY_0 = 0.40000001;
float S_HEAVY_TANK_ACCEL_0 = 15.0;
float S_T34_DAMP = 3.0;
float wheelSeperation = 8.5;
float tempRadiusPercent = 0.5;
float avel_threshold = 0.30000001;
float vel_threshold = 10.0;
int scale_max_speed = 1;
float percent_to_give = 1.5;
float low_end_speed = 300.0;
float climbHeavyAccelAdd = 120.0;
float pitchFakeyZone = 0.1;
float fwd_fric_scale = 1.0;
float side_fric_scale = 1.0;
float hand_brake_friction_time = 0.30000001;
bool render_wheel_cir = true;
float sphere_size = 5.0;
int duration = 100;
float acceptablePosErrorBase = 10.0;
float acceptableAVelError = 0.1;
float acceptableVelError = 3.0;
float timeCap = 0.80000001;
float errorAReductionRate = 2.0;
float errorReductionRate = 4.0;
phys_vec3 gWheelAxisLoc = { 0.0f, 1.0f, 0.0f };

// *WARNING* One or more selections were skipped as they could not be interpreted as c data
float mass_z_offset = 20.0f;
VehicleParameter g_default_params;
phys_free_list<NitrousVehicle> g_rb_vehicle_list;

// Decomp:  CoDMPServer.c:655331  (RVA 0081E710)
VehicleParameter::VehicleParameter()
{
    float *buoyancyMax;
    this->m_speed_max = 1100.0f;
    this->m_accel_max = 300.0f;
    this->m_reverse_scale = 0.8f;
    this->m_steer_angle_max = 0.6f;
    this->m_steer_speed = 5.0f;
    this->m_wheel_radius = 15.0f;
    this->m_susp_spring_k = 20.0f;
    this->m_susp_damp_k = 1.0f;
    this->m_susp_adj = 10.0f;
    this->m_susp_hard_limit = 23.0f;
    this->m_susp_min_height = -100.0f;
    this->m_tire_fric_fwd = 2.5f;
    this->m_tire_fric_side = 2.3f;
    this->m_tire_fric_brake = 2.3f;
    this->m_tire_fric_hand_brake = 0.0f;
    this->m_body_mass = 1.0f;
    this->m_roll_stability = 10.0f;
    this->m_pitch_roll_resistance = 50.0f;
    this->m_yaw_resistance = 0.0f;
    this->m_upright_strength = 50.0f;
    this->m_tilt_fakey = 0.25f;
    this->m_peel_out_max_speed = 150.0f;
    this->m_tire_damp_coast = 10.0f;
    this->m_tire_damp_brake = 100.0f;
    this->m_tire_damp_hand = 10000.0f;
    this->m_traction_type = TRACTION_TYPE_ALL_WD;
    this->m_water_ebrake_power = 1.5f;
    this->m_tire_fric_side_max = 0.0f;
    this->m_bbox_min[0] = 0.0f;
    this->m_bbox_min[1] = 0.0f;
    this->m_bbox_min[2] = 0.0f;
    this->m_bbox_max[0] = 0.0f;
    this->m_bbox_max[1] = 0.0f;
    this->m_bbox_max[2] = 0.0f;
    this->m_mass_center_offset[0] = 0.0f;
    this->m_mass_center_offset[1] = 0.0f;
    this->m_mass_center_offset[2] = 0.0f;
    this->m_buoyancybox_min[0] = 0.0f;
    this->m_buoyancybox_min[1] = 0.0f;
    this->m_buoyancybox_min[2] = 0.0f;
    this->m_buoyancybox_max[0] = 0.0f;
    buoyancyMax = this->m_buoyancybox_max;
    buoyancyMax[1] = 0.0f;
    buoyancyMax[2] = 0.0f;
    this->m_mass_center_offset[2] = mass_z_offset;
}

// Decomp:  CoDMPServer.c:655403  (RVA 0081EA20)
NitrousVehicle::NitrousVehicle()
{
    int wheelRelPoCount;
    phys_mat44 *wheelRelPo;
    int cmdIndex;
    int wheelIndex;
    wheelRelPoCount = 6;
    for ( wheelRelPo = this->m_wheel_orig_relpo; --wheelRelPoCount >= 0; ++wheelRelPo )
        ;
    this->m_parameter = 0;
    this->m_script_brake = 0.0f;
    this->m_hand_brake_friction_time = 0.0f;
    this->m_owner = 0;
    this->m_entnum = 0;
    this->m_vehicle_info = 0;
    this->m_phys_user_data = 0;
    this->m_xmodel = 0;
    this->m_orientation_constraint = 0;
    this->m_vpc = 0;
    this->m_notify_flags = 0;
    this->m_server_notify_flags = 0;
    for ( cmdIndex = 0; cmdIndex < 2; ++cmdIndex )
        this->mVehicleController.m_cmd.button_bits.array[cmdIndex] = 0;
    this->m_fake_rpm = 0.0f;
    this->m_trackDistAccumLeftInches = 0;
    this->m_trackDistAccumRightInches = 0;
    this->m_sfx_state_flags = 0;
    this->m_flags = 0;
    this->refcount = 0;
    this->m_state_flags = 0;
    this->m_flags |= 1u;
    this->m_flags &= ~2u;
    for ( wheelIndex = 0; wheelIndex < 6; ++wheelIndex )
        this->m_wheels[wheelIndex] = 0;
}

// Decomp:  CoDMPServer.c:655445  (RVA 0081EC70)
void NitrousVehicle::init(gentity_s *owner, const VehicleParameter *parameter)
{
    DObj *ServerDObj;
    float axis[9];
    phys_vec3 *p_m_ground_vel;
    float *m_networkErrorAngles;
    float *m_networkErrorOrigin;
    float *m_angles;
    float *currentAngles;
    float *m_origin;
    float *currentOrigin;
    int wheelIndex;
    if ( !parameter )
        parameter = &g_default_params;
    //NitrousVehicle::pause_physics(this, 0);
    this->pause_physics(false);
    this->m_owner = owner;
    this->m_entnum = owner->s.number;
    this->m_vehicle_info = BG_GetVehicleInfo(owner->scr_vehicle->infoIdx);
    ServerDObj = Com_GetServerDObj(this->m_entnum);
    this->m_xmodel = DObjGetModel(ServerDObj, 0);
    this->m_parameter = (VehicleParameter*)parameter;
    this->m_hand_brake = 0.0f;
    this->m_brake = 0.0f;
    //NitrousVehicle::set_throttle(this, 0.0);
    this->set_throttle(0.0f);
    this->m_steer_factor = 0.0f;
    this->m_forward_vel = 0.0f;
    this->m_script_brake = 0.0f;
    this->m_stuck_time = 0.0f;
    m_origin = this->m_origin;
    currentOrigin = owner->r.currentOrigin;
    this->m_origin[0] = owner->r.currentOrigin[0];
    m_origin[1] = currentOrigin[1];
    m_origin[2] = currentOrigin[2];
    m_angles = this->m_angles;
    currentAngles = owner->r.currentAngles;
    this->m_angles[0] = owner->r.currentAngles[0];
    m_angles[1] = currentAngles[1];
    m_angles[2] = currentAngles[2];
    m_networkErrorOrigin = this->m_networkErrorOrigin;
    this->m_networkErrorOrigin[0] = 0.0f;
    m_networkErrorOrigin[1] = 0.0f;
    m_networkErrorOrigin[2] = 0.0f;
    m_networkErrorAngles = this->m_networkErrorAngles;
    this->m_networkErrorAngles[0] = 0.0f;
    m_networkErrorAngles[1] = 0.0f;
    m_networkErrorAngles[2] = 0.0f;
    p_m_ground_vel = &this->m_ground_vel;
    this->m_ground_vel.x = 0.0f;
    p_m_ground_vel->y = 0.0f;
    p_m_ground_vel->z = 0.0f;
    this->m_lastErrorReductionTime = 0;
    this->m_speed_scale = 1.0f;
    this->m_drivepath_scale = 1.0f;
    this->m_boost_start_time = 0;
    this->m_boost_time_pool = this->m_vehicle_info->boostDuration;
    this->m_throttle_held_down = 0;
    this->m_throttle_time_started = 0;
    this->m_flags &= ~0x100u;
    this->m_flags &= ~0x80u;
    this->m_flags &= ~4u;
    AnglesToAxis(this->m_angles, (float (*)[3])axis);
    Phys_AxisToNitrousMat((float (*)[3])axis, &this->m_prev_rb_mat);
    Phys_Vec3ToNitrousVec(this->m_origin, &this->m_prev_rb_mat.w);
    if ( fabs(this->m_prev_rb_mat.w.x) > 100000.0
        || fabs(this->m_prev_rb_mat.w.y) > 100000.0
        || fabs(this->m_prev_rb_mat.w.z) > 100000.0 )
    {
        phys_exec_debug_callback(0);
    }
    //NitrousVehicle::_setup_wheels(this, (DObj *)&savedregs, 0);
    this->_setup_wheels(0);
    for ( wheelIndex = 0; wheelIndex < 4; ++wheelIndex )
    {
        this->m_wheel_state[wheelIndex].m_state = WHEEL_STATE_AIRBORN;
        this->m_wheel_state[wheelIndex].m_last_state = WHEEL_STATE_AIRBORN;
        this->m_wheel_state[wheelIndex].m_state_time = 0.0f;
        this->m_wheel_state[wheelIndex].m_rate = 0.0f;
    }
}

// Decomp:  CoDMPServer.c:655542  (RVA 0081F3A0)
void NitrousVehicle::set_throttle(float throttle)
{
    if ( (throttle < -1.0 || throttle > 1.0)
        && !Assert_MyHandler(
                    "c:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.h",
                    453,
                    0,
                    "%s\n\t%s",
                    "throttle>= -1.0f && throttle<= 1.0f",
                    "Out of range") )
    {
        __debugbreak();
    }
    this->m_throttle = throttle;
}

// Decomp:  CoDMPServer.c:655564  (RVA 0081F410)
void NitrousVehicle::init(
                int localClientNum,
                centity_s *owner,
                const VehicleParameter *parameter)
{
    float axis[9];
    phys_vec3 *p_m_ground_vel;
    float *m_networkErrorAngles;
    float *m_networkErrorOrigin;
    float *m_angles;
    float *angles;
    float *m_origin;
    float *origin;
    int wheelIndex;
    const DObj *obj;
    const cgs_t *cgs;
    if ( this->refcount <= 1 )
    {
        if ( !parameter )
            parameter = &g_default_params;
        cgs = CG_GetLocalClientStaticGlobals(localClientNum);
        obj = CG_PreProcess_GetDObj(
                        localClientNum,
                        owner->nextState.number,
                        owner->nextState.eType,
                        cgs->gameModels[owner->nextState.un3.item],
                        0);
        if ( !obj
            && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp", 188, 0, "%s", "obj") )
        {
            __debugbreak();
        }
        this->m_xmodel = DObjGetModel(obj, 0);
        this->m_owner = 0;
        this->m_entnum = owner->nextState.number;
        this->m_vehicle_info = CG_GetVehicleInfo(owner->nextState.vehicleState.vehicleInfoIndex);
        this->m_parameter = (VehicleParameter*)parameter;
        this->m_hand_brake = 0.0f;
        this->m_brake = 0.0f;
        //NitrousVehicle::set_throttle(this, 0.0);
        this->set_throttle(0.0f);
        this->m_steer_factor = 0.0f;
        this->m_forward_vel = 0.0f;
        this->m_script_brake = 0.0f;
        this->m_stuck_time = 0.0f;
        m_origin = this->m_origin;
        origin = owner->pose.origin;
        this->m_origin[0] = owner->pose.origin[0];
        m_origin[1] = origin[1];
        m_origin[2] = origin[2];
        m_angles = this->m_angles;
        angles = owner->pose.angles;
        this->m_angles[0] = owner->pose.angles[0];
        m_angles[1] = angles[1];
        m_angles[2] = angles[2];
        m_networkErrorOrigin = this->m_networkErrorOrigin;
        this->m_networkErrorOrigin[0] = 0.0f;
        m_networkErrorOrigin[1] = 0.0f;
        m_networkErrorOrigin[2] = 0.0f;
        m_networkErrorAngles = this->m_networkErrorAngles;
        this->m_networkErrorAngles[0] = 0.0f;
        m_networkErrorAngles[1] = 0.0f;
        m_networkErrorAngles[2] = 0.0f;
        p_m_ground_vel = &this->m_ground_vel;
        this->m_ground_vel.x = 0.0f;
        p_m_ground_vel->y = 0.0f;
        p_m_ground_vel->z = 0.0f;
        this->m_speed_scale = 1.0f;
        this->m_drivepath_scale = 1.0f;
        this->m_boost_start_time = 0;
        this->m_boost_time_pool = this->m_vehicle_info->boostDuration;
        this->m_throttle_held_down = 0;
        AnglesToAxis(this->m_angles, (float (*)[3])axis);
        Phys_AxisToNitrousMat((float (*)[3])axis, &this->m_prev_rb_mat);
        Phys_Vec3ToNitrousVec(this->m_origin, &this->m_prev_rb_mat.w);
        if ( fabs(this->m_prev_rb_mat.w.x) > 100000.0
            || fabs(this->m_prev_rb_mat.w.y) > 100000.0
            || fabs(this->m_prev_rb_mat.w.z) > 100000.0 )
        {
            phys_exec_debug_callback(0);
        }
        //NitrousVehicle::_setup_wheels(this, (DObj *)&savedregs, localClientNum);
        this->_setup_wheels(localClientNum);
        for ( wheelIndex = 0; wheelIndex < 4; ++wheelIndex )
        {
            this->m_wheel_state[wheelIndex].m_state = WHEEL_STATE_AIRBORN;
            this->m_wheel_state[wheelIndex].m_last_state = WHEEL_STATE_AIRBORN;
            this->m_wheel_state[wheelIndex].m_state_time = 0.0f;
            this->m_wheel_state[wheelIndex].m_rate = 0.0f;
        }
    }
}

// Decomp:  CoDMPServer.c:655671  (RVA 0081FAB0)
PhysObjUserData *__cdecl Phys_ObjCreateNitrousVehicle(
                const float *position,
                const float *angles,
                const float *velocity,
                const float *translation,
                const PhysPreset *physPreset,
                gjk_geom_list_t *gjk_geom_list)
{
    double buoyancyValue;
    BodyState state;
    float axis[3][3];
    nanassertvec3(position);
    nanassertvec3(velocity);
    if ( !physPreset
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp", 235, 0, "%s", "physPreset") )
    {
        __debugbreak();
    }
    AnglesToAxis(angles, axis);
    AxisCopy(axis, state.rotation);
    state.position[0] = *position;
    state.position[1] = position[1];
    state.position[2] = position[2];
    state.velocity[0] = *velocity;
    state.velocity[1] = velocity[1];
    state.velocity[2] = velocity[2];
    memset(state.angVelocity, 0, 48);
    state.mass = physPreset->mass;
    state.bounce = physPreset->bounce;
    state.friction = physPreset->friction;
    state.timeLastAsleep = physGlob.timeLastUpdate;
    if ( physPreset->canFloat )
        buoyancyValue = phys_gravity->current.value * -1.1;
    else
        buoyancyValue = phys_gravity->current.value * -0.9;
    state.buoyancy = (int)buoyancyValue;
    state.underwater = 0;
    state.id = -1;
    //return Phys_CreateBodyFromState((int)&savedregs, 0, &state, gjk_geom_list, 1);
    return Phys_CreateBodyFromState(0, &state, gjk_geom_list, 1);
}

// Decomp:  CoDMPServer.c:655775  (RVA 0081FDE0)
void    NitrousVehicle::unpause_physics()
{
    PhysObjUserData *physUserData;
    phys_vec3 *p_m_gravity_acc_vec;
    float gravityAccelY;
    float gravityAccelZ;
    float gravityMultiplier;
    float gravityMagnitude;
    float gravityDirVec[3];
    phys_vec3 gravity_dir;
    broad_phase_info *bpiIter;
    broad_phase_group *bpg;
    int flags;
    unsigned int m_env_collision_flags;
    broad_phase_info *bpi;
    gjk_geom_list_t gjk_geom_list;
    float vel[3];
    const VehicleParameter *m_parameter;
    PhysPreset physPreset;
    int m_flags;
    if ((this->m_flags & 1) != 0)
    {
        this->m_flags &= ~1u;
        m_flags = this->m_flags;
        physPreset.bounce = 0.3f;
        physPreset.bulletForceScale = 1.0f;
        physPreset.explosiveForceScale = 1.0f;
        if (this->m_vehicle_info->type == 4)
            physPreset.friction = 1.0f;
        else
            physPreset.friction = 0.4f;
        m_parameter = this->m_parameter;
        physPreset.mass = m_parameter->m_body_mass;
        physPreset.canFloat = this->m_parameter->m_buoyancybox_min[0] != 0.0;
        memset(vel, 0, sizeof(vel));
        gjk_geom_list.m_first_geom = 0;
        gjk_geom_list.m_geom_count = 0;
        create_gjk_geom_collision_visitor collision_visitor;
        //collision_visitor.__vftable = (create_gjk_geom_collision_visitor_vtbl *)&create_gjk_geom_collision_visitor::`vftable';
        collision_visitor.gjk_geom_list = &gjk_geom_list;
        create_xmodel_gjk_geom(
            this->m_xmodel,
            &collision_visitor,
            -1,
            0x280EC93u,
            1,
            0,
            0);
        physUserData = Phys_ObjCreateNitrousVehicle(
            this->m_origin,
            this->m_angles,
            vel,
            this->m_parameter->m_mass_center_offset,
            &physPreset,
            &gjk_geom_list);
        this->m_phys_user_data = physUserData;
        this->m_phys_user_data->vehicle = this;
        if ( this->m_phys_user_data->m_bpb->is_bpi() )
        {
            //bpi = broad_phase_base::get_bpi(this->m_phys_user_data->m_bpb);
            bpi = this->m_phys_user_data->m_bpb->get_bpi();
            m_env_collision_flags = bpi->m_env_collision_flags;
            flags = (m_env_collision_flags | (8 * (this->m_vehicle_info->remoteControl != 0) + 8)) & 0xFFFFFF7F;
            bpi->m_env_collision_flags = flags;
        }
        else
        {
            //bpg = broad_phase_base::get_bpg(this->m_phys_user_data->m_bpb);
            bpg = this->m_phys_user_data->m_bpb->get_bpg();
            for (bpiIter = bpg->m_list_bpi_head; bpiIter; bpiIter = (broad_phase_info *)bpiIter->m_list_bpb_next)
            {
                bpiIter->m_env_collision_flags = bpiIter->m_env_collision_flags | (8 * (this->m_vehicle_info->remoteControl != 0) + 8) & 0xFFFFFF7F;
            }
        }
        Phys_Vec3ToNitrousVec(&phys_gravity_dir->current.value, (phys_vec3 *)gravityDirVec);
        gravityMagnitude = phys_gravity->current.value;
        gravityMultiplier = phys_vehicleGravityMultiplier->current.value;
        gravityAccelY = gravityMultiplier * (float)(gravityMagnitude * gravityDirVec[1]);
        gravityAccelZ = gravityMultiplier * (float)(gravityMagnitude * gravityDirVec[2]);
        p_m_gravity_acc_vec = &this->m_phys_user_data->body->m_gravity_acc_vec;
        p_m_gravity_acc_vec->x = gravityMultiplier * (float)(gravityMagnitude * gravityDirVec[0]);
        p_m_gravity_acc_vec->y = gravityAccelY;
        p_m_gravity_acc_vec->z = gravityAccelZ;
        NitrousVehicle::update_parms(this->m_parameter, 1);
        this->m_flags |= 2u;
        //NitrousVehicleController::Init(&this->mVehicleController);
        this->mVehicleController.Init();
    }
}

// Decomp:  CoDMPServer.c:655898  (RVA 00820320)
void NitrousVehicleController::Init()
{
    this->m_stuck_time = 0.0f;
    this->m_stopped_time = 0.0f;
    memset((unsigned __int8 *)&this->m_cmd, 0, sizeof(this->m_cmd));
    this->m_cmd_local_client_num = 0;
    this->m_cmd_valid = 1;
    this->m_stop_at_goal = 0;
}

// Decomp:  CoDMPServer.c:655912  (RVA 00820380)
void NitrousVehicle::pause_physics(bool shutdown)
{
    if ( (this->m_flags & 1) == 0 )
    {
        Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
        if ( !shutdown )
        {
            //phys_mat44::operator=(&this->m_prev_rb_mat, &this->m_phys_user_data->body->m_mat);
            this->m_prev_rb_mat = this->m_phys_user_data->body->m_mat;
            NitrousVehicle::remove_wheels();
            if ( this->m_orientation_constraint )
                phys_sys::destroy(this->m_orientation_constraint);
            this->m_orientation_constraint = 0;
        }
        NitrousVehicle::cleanup_path();
        this->m_flags |= 1u;
        Phys_ObjDestroy(0, (int)this->m_phys_user_data);
        this->m_phys_user_data = 0;
        Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
    }
}

// Decomp:  CoDMPServer.c:655939  (RVA 00820490)
void NitrousVehicle::_update_pause()
{
    bool shouldFreezeBody;
    this->m_flags |= 0x40u;
    shouldFreezeBody = (this->m_flags & 4) != 0 || (this->m_flags & 0x80) != 0 || (this->m_flags & 0x100) != 0;
    if (this->m_phys_user_data)
    {
        //rigid_body::set_flag(this->m_phys_user_data->body, 0x40u, shouldFreezeBody | 1);
        this->m_phys_user_data->body->set_flag(0x40u, shouldFreezeBody | 1);
    }
    if ((this->m_flags & 1) != 0)
    {
        //NitrousVehicle::unpause_physics(this, (int)&savedregs);
        NitrousVehicle::unpause_physics();
    }
}

// Decomp:  CoDMPServer.c:655966  (RVA 008205F0)
void rigid_body::set_flag(unsigned int f, int b)
{
    if ( (f & 0xFFFFFF00) != 0
        && _tlAssert(
                 "C:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\rigid_body.h",
                 59,
                 "(f & ~(FLAG_CLIENT_FLAGS_START - 1)) == 0",
                 "invalid rigid body flag") )
    {
        __debugbreak();
    }
    if ( b )
        this->m_flags |= f;
    else
        this->m_flags &= ~f;
}

// Decomp:  CoDMPServer.c:655995  (RVA 00820680)
unsigned int rigid_body::is_group_stable()
{
    return this->m_flags & 8;
}

// Decomp:  CoDMPServer.c:656001  (RVA 008206D0)
void NitrousVehicle::update_parms(
                const VehicleParameter *params,
                bool initialization)
{
    const VehicleParameter *m_parameter;
    double maximum_acceleration;
    float fwd_fric_k;
    float suspension_stiffness_k;
    float suspension_damp_k;
    float hard_limit_dist;
    float roll_stability_factor;
    float maxAccelForce;
    float frontSteerSumY;
    float frontSteerSumZ;
    rigid_body_constraint_wheel *frontLeftWheel;
    rigid_body_constraint_wheel *frontRightWheel;
    float front_back_length;
    phys_vec3 front_steer_pt;
    float defaultSteerX;
    int defaultSteerY;
    int defaultSteerZ;
    unsigned int wheelFlagsScratch;
    BOOL isFrontWheel;
    TractionType m_traction_type;
    unsigned int m_wheel_flags;
    float side_fric_max;
    float wheel_radius;
    phys_vec3 susp_dir;
    float susp_damp_k;
    float susp_spring_k;
    float mass;
    const VehicleParameter *wheelParams;
    float comFactorClampedLow;
    float comFactorClampedHigh;
    float com_spring_factor;
    float dist_to_com;
    float wheelCenterX;
    float wheelCenterZAdjusted;
    float wheelCenterZ;
    phys_vec3 wheel_center_loc;
    float wheelWorldX;
    float wheelWorldY;
    float wheelWorldZ;
    float wheelRelZ;
    float wheelRelY;
    float wheelRelX;
    phys_vec3 *wheelRelPos;
    phys_vec3 translation;
    rigid_body_constraint_wheel *rbc_wheel;
    int hasWheel;
    BOOL hasWheelYCoord;
    float y;
    int i;
    float wheel_to_wheel_length;
    float wheelDeltaVec[7];
    float wheelDeltaZ;
    float wheelDeltaY;
    float wheelDeltaX;
    phys_vec3 *frontWheelRelPos;
    phys_vec3 *p_w;
    const VehicleParameter *massParams;
    float maxAvel;
    rigid_body *rb;
    phys_mat44 *p_cg2rb;
    phys_vec3 cgTranslationNeg;
    phys_vec3 _com;
    float *m_mass_center_offset;
    float inertiaMassScale;
    const VehicleParameter *inertiaParams;
    phys_vec3 phys_vec3_unit_inertia;
    float volume;
    phys_vec3 phys_dim;
    float dim[3];
    float com[4];
    float *m_bbox_min;
    float vmax[3];
    float vmin[3];
    phys_vec3 gjk_geom_list_aabb_mn_loc;
    phys_vec3 gjk_geom_list_aabb_mx_loc;
    if (params)
        this->m_parameter = (VehicleParameter*)params;
    if (phys_heavyTankSwitch->current.enabled)
    {
        if (this->m_parameter->m_susp_damp_k == S_T34_DAMP)
        {
            this->m_parameter->m_accel_max = S_HEAVY_TANK_ACCEL_0;
            this->m_parameter->m_tilt_fakey = S_HEAVY_TILT_FAKEY_0;
        }
        else
        {
            this->m_parameter->m_accel_max = S_HEAVY_TANK_ACCEL;
            this->m_parameter->m_tilt_fakey = S_HEAVY_TILT_FAKEY;
        }
    }
    if ((this->m_flags & 1) != 0)
    {
        NitrousVehicle::unpause_physics();
    }
    else
    {
        if (this->m_parameter->m_bbox_min[0] == 0.0)
        {
            this->m_phys_user_data->m_gjk_geom_list.comp_aabb_loc(&gjk_geom_list_aabb_mn_loc, &gjk_geom_list_aabb_mx_loc);
            //gjk_geom_list_t::comp_aabb_loc(
            //    &this->m_phys_user_data->m_gjk_geom_list,
            //    (int)v92,
            //    &gjk_geom_list_aabb_mn_loc,
            //    &gjk_geom_list_aabb_mx_loc);
            Phys_NitrousVecToVec3(&gjk_geom_list_aabb_mn_loc, vmin);
            Phys_NitrousVecToVec3(&gjk_geom_list_aabb_mx_loc, vmax);
        }
        else
        {
            m_bbox_min = this->m_parameter->m_bbox_min;
            vmin[0] = *m_bbox_min;
            vmin[1] = m_bbox_min[1];
            vmin[2] = m_bbox_min[2];
            m_parameter = this->m_parameter;
            //LODWORD(com[3]) = m_parameter->m_bbox_max;
            vmax[0] = m_parameter->m_bbox_max[0];
            vmax[1] = m_parameter->m_bbox_max[1];
            vmax[2] = m_parameter->m_bbox_max[2];
        }
        com[0] = 0.5 * (float)(vmax[0] + vmin[0]);
        com[1] = 0.5 * (float)(vmax[1] + vmin[1]);
        com[2] = 0.5 * (float)(vmax[2] + vmin[2]);
        dim[0] = vmax[0] - vmin[0];
        dim[1] = vmax[1] - vmin[1];
        dim[2] = vmax[2] - vmin[2];
        Phys_Vec3ToNitrousVec(dim, &phys_dim);
        nuge::calc_box_inertia(&phys_dim, &phys_vec3_unit_inertia, &volume);
        inertiaParams = this->m_parameter;
        inertiaMassScale = inertiaParams->m_body_mass / volume;
        phys_vec3_unit_inertia.x = phys_vec3_unit_inertia.x * inertiaMassScale;
        phys_vec3_unit_inertia.y = phys_vec3_unit_inertia.y * inertiaMassScale;
        phys_vec3_unit_inertia.z = phys_vec3_unit_inertia.z * inertiaMassScale;
        m_mass_center_offset = this->m_parameter->m_mass_center_offset;
        com[0] = com[0] + *m_mass_center_offset;
        com[1] = com[1] + m_mass_center_offset[1];
        com[2] = com[2] + m_mass_center_offset[2];
        Phys_Vec3ToNitrousVec(com, &_com);
        (cgTranslationNeg.x) = -(_com.x);
        (cgTranslationNeg.y) = -(_com.y);
        (cgTranslationNeg.z) = -(_com.z);
        p_cg2rb = &this->m_phys_user_data->cg2rb;
        //phys_vec3::operator=(&p_cg2rb->w, &cgTranslationNeg);
        p_cg2rb->w = cgTranslationNeg;
        rb = this->m_phys_user_data->body;
        if (this->m_vehicle_info->type == 2)
        {
            maxAvel = this->m_vehicle_info->rotRate * 0.017453292;
            rb->m_max_avel = maxAvel;
        }
        else
        {
            rb->m_max_avel = 10.0f;
        }
        massParams = this->m_parameter;
        //rigid_body::set_mass(rb, massParams->m_body_mass);
        rb->set_mass(massParams->m_body_mass);
        //rigid_body::set_inertia(rb, &phys_vec3_unit_inertia);
        rb->set_inertia(&phys_vec3_unit_inertia);
        p_w = &this->m_wheel_orig_relpo[2].w;
        frontWheelRelPos = &this->m_wheel_orig_relpo[0].w;
        wheelDeltaX = this->m_wheel_orig_relpo[0].w.x - this->m_wheel_orig_relpo[2].w.x;
        wheelDeltaY = this->m_wheel_orig_relpo[0].w.y - this->m_wheel_orig_relpo[2].w.y;
        wheelDeltaZ = this->m_wheel_orig_relpo[0].w.z - this->m_wheel_orig_relpo[2].w.z;
        wheelDeltaVec[0] = wheelDeltaX;
        wheelDeltaVec[1] = wheelDeltaY;
        wheelDeltaVec[2] = wheelDeltaZ;
        wheel_to_wheel_length = Abs(wheelDeltaVec);
        for (i = 0; i < 6; ++i)
        {
            y = this->m_wheel_orig_relpo[i].w.y;
            hasWheelYCoord = y != 0.0;
            hasWheel = hasWheelYCoord;
            if (y == 0.0)
            {
                this->m_wheels[i] = 0;
            }
            else
            {
                rbc_wheel = 0;
                if (initialization)
                {
                    rbc_wheel = phys_sys::create_rbc_wheel((environment_rigid_body *)rb, 0, 0);
                    if (!rbc_wheel
                        && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
                            497,
                            0,
                            "%s\n\t%s",
                            "rbc_wheel",
                            "Ran out of wheel constraints."))
                    {
                        __debugbreak();
                    }
                    this->m_wheels[i] = rbc_wheel;
                }
                else
                {
                    rbc_wheel = this->m_wheels[i];
                }
                translation = this->m_phys_user_data->cg2rb.w;
                wheelRelPos = &this->m_wheel_orig_relpo[i].w;
                wheelRelX = wheelRelPos->x + translation.x;
                wheelRelY = this->m_wheel_orig_relpo[i].w.y + translation.y;
                wheelRelZ = this->m_wheel_orig_relpo[i].w.z + translation.z;
                wheelWorldX = wheelRelX;
                wheelWorldY = wheelRelY;
                wheelWorldZ = wheelRelZ;
                wheel_center_loc.x = wheelRelX;
                wheel_center_loc.y = wheelRelY;
                wheel_center_loc.z = wheelRelZ;
                wheelCenterZ = wheelRelZ;
                wheelCenterZAdjusted = (float)(wheelRelZ - this->m_parameter->m_susp_adj) + this->m_parameter->m_wheel_radius;
                wheel_center_loc.z = wheelCenterZAdjusted;
                wheelCenterX = wheelRelX;
                //LODWORD(dist_to_com) = LODWORD(wheelRelX) & _mask__AbsFloat_;
                (dist_to_com) = fabs(wheelRelX);
                //com_spring_factor = COERCE_FLOAT(LODWORD(wheelRelX) & _mask__AbsFloat_) / wheel_to_wheel_length;
                com_spring_factor = (fabs(wheelRelX)) / wheel_to_wheel_length;
                if ((float)(com_spring_factor - 1.0) < 0.0)
                    comFactorClampedHigh = com_spring_factor;
                else
                    comFactorClampedHigh = 1.0f;
                if ((float)(0.0 - com_spring_factor) < 0.0)
                    comFactorClampedLow = comFactorClampedHigh;
                else
                    comFactorClampedLow = 0.0f;
                com_spring_factor = (float)(1.0 - comFactorClampedLow) * 2.0;
                wheelParams = this->m_parameter;
                mass = wheelParams->m_body_mass;
                susp_spring_k = (float)((float)(this->m_parameter->m_susp_spring_k
                    * phys_vehicleGravityMultiplier->current.value)
                    * com_spring_factor)
                    * mass;
                susp_damp_k = (float)((float)(this->m_parameter->m_susp_damp_k * phys_vehicleGravityMultiplier->current.value)
                    * com_spring_factor)
                    * mass;
                susp_dir.x = 0.0f;
                susp_dir.y = 0.0f;
                susp_dir.z = -1.0f;
                wheel_radius = this->m_parameter->m_wheel_radius;
                if (this->m_parameter->m_tire_fric_side_max == 0.0)
                    side_fric_max = FLT_MAX;
                else
                    side_fric_max = this->m_parameter->m_tire_fric_side_max;
                //rigid_body_constraint_wheel::set(
                    rbc_wheel->set(
                    &wheel_center_loc,
                    &susp_dir,
                    &gWheelAxisLoc,
                    wheel_radius,
                    this->m_parameter->m_tire_fric_fwd / phys_vehicleGravityMultiplier->current.value,
                    this->m_parameter->m_tire_fric_side / phys_vehicleGravityMultiplier->current.value,
                    susp_spring_k,
                    susp_damp_k,
                    this->m_parameter->m_susp_hard_limit,
                    this->m_parameter->m_roll_stability,
                    this->m_parameter->m_roll_stability,
                    side_fric_max);
                if (this->m_vehicle_info->type == 2 || this->m_vehicle_info->isFourWheelSteering)
                {
                    rbc_wheel->m_wheel_flags |= 8u;
                    m_wheel_flags = rbc_wheel->m_wheel_flags;
                }
                else
                {
                    if ((unsigned int)i < 2 || i == 4 || i == 5)
                        rbc_wheel->m_wheel_flags |= 8u;
                    else
                        rbc_wheel->m_wheel_flags &= ~8u;
                    wheelFlagsScratch = rbc_wheel->m_wheel_flags;
                }
                rbc_wheel->m_wheel_flags &= ~0x80u;
                wheelFlagsScratch = rbc_wheel->m_wheel_flags;
                if (i == 2 || i == 3)
                    rbc_wheel->m_wheel_flags |= 0x20u;
                else
                    rbc_wheel->m_wheel_flags &= ~0x20u;
                wheelFlagsScratch = rbc_wheel->m_wheel_flags;
                rbc_wheel->m_wheel_flags |= 0x40u;
                wheelFlagsScratch = rbc_wheel->m_wheel_flags;
                m_traction_type = this->m_parameter->m_traction_type;
                if (m_traction_type)
                {
                    if (m_traction_type == TRACTION_TYPE_BACK)
                    {
                        if (i == 2 || i == 3)
                            rbc_wheel->m_wheel_flags |= 0x10u;
                        else
                            rbc_wheel->m_wheel_flags &= ~0x10u;
                        wheelFlagsScratch = rbc_wheel->m_wheel_flags;
                    }
                    else if (m_traction_type == TRACTION_TYPE_ALL_WD)
                    {
                        rbc_wheel->m_wheel_flags |= 0x10u;
                        wheelFlagsScratch = rbc_wheel->m_wheel_flags;
                    }
                }
                else
                {
                    isFrontWheel = (unsigned int)i < 2;
                    if ((unsigned int)i >= 2)
                        rbc_wheel->m_wheel_flags &= ~0x10u;
                    else
                        rbc_wheel->m_wheel_flags |= 0x10u;
                    wheelFlagsScratch = rbc_wheel->m_wheel_flags;
                }
            }
        }
        if (this->m_vehicle_info->type == 4)
        {
            defaultSteerX = 50.0f;
            defaultSteerY = 0.0f;
            defaultSteerZ = 0.0f;
            front_steer_pt.x = 50.0f;
            front_steer_pt.y = 0.0f;
            front_steer_pt.z = 0.0f;
            front_back_length = 100.0f;
        }
        else
        {
            frontRightWheel = this->m_wheels[1];
            frontLeftWheel = this->m_wheels[0];
            frontSteerSumY = frontLeftWheel->m_b1_wheel_center_loc.y + frontRightWheel->m_b1_wheel_center_loc.y;
            frontSteerSumZ = frontLeftWheel->m_b1_wheel_center_loc.z + frontRightWheel->m_b1_wheel_center_loc.z;
            front_steer_pt.x = (float)(frontLeftWheel->m_b1_wheel_center_loc.x + frontRightWheel->m_b1_wheel_center_loc.x) * 0.5;
            front_steer_pt.y = frontSteerSumY * 0.5;
            front_steer_pt.z = frontSteerSumZ * 0.5;
            front_back_length = front_steer_pt.x
                - (float)((float)(this->m_wheels[2]->m_b1_wheel_center_loc.x
                    + this->m_wheels[3]->m_b1_wheel_center_loc.x)
                    * 0.5);
        }
        maximum_acceleration = NitrousVehicle::get_maximum_acceleration();
        maxAccelForce = maximum_acceleration * this->m_parameter->m_body_mass;
        roll_stability_factor = this->m_parameter->m_steer_speed;
        hard_limit_dist = this->m_parameter->m_steer_angle_max;
        suspension_damp_k = this->m_parameter->m_wheel_radius;
        suspension_stiffness_k = this->m_parameter->m_tire_damp_coast * this->m_parameter->m_body_mass;
        fwd_fric_k = NitrousVehicle::get_max_speed(1);
        NitrousVehicle::set(
            this->m_parameter->m_tire_damp_hand * this->m_parameter->m_body_mass,
            this->m_parameter->m_tire_damp_brake * this->m_parameter->m_body_mass,
            fwd_fric_k,
            maxAccelForce,
            suspension_stiffness_k,
            suspension_damp_k,
            hard_limit_dist,
            roll_stability_factor,
            &front_steer_pt,
            front_back_length);
        if (this->m_orientation_constraint)
        {
            this->m_orientation_constraint->m_torque_resistance_pitch_roll = this->m_parameter->m_pitch_roll_resistance
                * this->m_parameter->m_body_mass;
            this->m_orientation_constraint->m_torque_resistance_yaw = this->m_parameter->m_yaw_resistance
                * this->m_parameter->m_body_mass;
            this->m_orientation_constraint->m_upright_strength = this->m_parameter->m_upright_strength
                * this->m_parameter->m_body_mass;
            NitrousVehicle::_update_orientation_constraint();
        }
    }
}

// Decomp:  CoDMPServer.c:656560  (RVA 00821DF0)
// Decomp:   logic matches IDA; wheel separation uses -wheelSeparation vs xor mask — equivalent
void NitrousVehicle::_setup_wheels(int localClientNum)
{
    char *assertMsg;
    phys_vec3 *wheelPos;
    int i;
    phys_vec3 *unusedWheelPos5;
    phys_vec3 *unusedWheelPos4;
    phys_vec3 *backRightWheelPos;
    float backRightSepY;
    phys_vec3 *backLeftWheelPos;
    float backLeftX;
    float backLeftSepY;
    float backLeftZ;
    float y;
    phys_vec3 *frontRightWheelPos;
    float frontRightSepY;
    phys_vec3 *p_w;
    float frontLeftX;
    float frontLeftSepY;
    float frontLeftZ;
    float pos_4;
    float mat34[4][3];
    DObjAnimMat mat;
    unsigned int backWheel;
    unsigned int frontWheel;
    gentity_s *m_owner;
    DObj *obj;
    unsigned __int8 index;
    index = 0;
    obj = 0;
    if (!this->m_xmodel)
    {
        assertMsg = va(
            "NitrousVehicle:  Trying to setup wheels on vehicle with invalid model.  Vehicle %s at (%g %g %g).\n",
            this->m_vehicle_info->name,
            this->m_origin[0],
            this->m_origin[1],
            this->m_origin[2]);
        if (!Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
            631,
            0,
            "%s\n\t%s",
            "m_xmodel",
            assertMsg))
            __debugbreak();
    }
    if (this->m_owner)
    {
        m_owner = this->m_owner;
        obj = Com_GetServerDObj(m_owner->s.number);
    }
    else
    {
        obj = Com_GetClientDObj(this->m_entnum, localClientNum);
    }
    if (this->m_vehicle_info->type == 1)
    {
        frontWheel = SL_FindLowercaseString("tag_wheel_front", SCRIPTINSTANCE_SERVER);
        backWheel = SL_FindLowercaseString("tag_wheel_back", SCRIPTINSTANCE_SERVER);
        if (DObjGetBoneIndex(obj, frontWheel, &index, -1))
        {
            DObjGetBasePoseMatrix(obj, index, &mat);
            Ragdoll_AnimMatToMat43(&mat, mat34);
            iassert(!IS_NAN((mat34[3])[0]) && !IS_NAN((mat34[3])[1]) && !IS_NAN((mat34[3])[2]));
            Phys_Mat4ToNitrousMat(mat34, this->m_wheel_orig_relpo);
            //phys_mat44::operator=(&this->m_wheel_orig_relpo[1], this->m_wheel_orig_relpo);
            this->m_wheel_orig_relpo[1] = this->m_wheel_orig_relpo[0];
            pos_4 = this->m_wheel_orig_relpo[0].w.y;
            frontLeftX = this->m_wheel_orig_relpo[0].w.x + 0.0;
            //frontLeftSepY = pos_4 + COERCE_FLOAT(LODWORD(wheelSeperation) ^ _mask__NegFloat_);
            frontLeftSepY = pos_4 + (-(wheelSeperation));
            frontLeftZ = this->m_wheel_orig_relpo[0].w.z + 0.0;
            p_w = &this->m_wheel_orig_relpo[0].w;
            this->m_wheel_orig_relpo[0].w.x = frontLeftX;
            p_w->y = frontLeftSepY;
            p_w->z = frontLeftZ;
            frontRightSepY = pos_4 + wheelSeperation;
            frontRightWheelPos = &this->m_wheel_orig_relpo[1].w;
            this->m_wheel_orig_relpo[1].w.x = frontLeftX;
            frontRightWheelPos->y = frontRightSepY;
            frontRightWheelPos->z = frontLeftZ;
        }
        else if (!Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
            662,
            0,
            "Failed to find tag_wheel_front for motorcycle."))
        {
            __debugbreak();
        }
        if (DObjGetBoneIndex(obj, backWheel, &index, -1))
        {
            DObjGetBasePoseMatrix(obj, index, &mat);
            Ragdoll_AnimMatToMat43(&mat, mat34);
            iassert(!IS_NAN((mat34[3])[0]) && !IS_NAN((mat34[3])[1]) && !IS_NAN((mat34[3])[2]));
            Phys_Mat4ToNitrousMat(mat34, &this->m_wheel_orig_relpo[2]);
            //phys_mat44::operator=(&this->m_wheel_orig_relpo[3], &this->m_wheel_orig_relpo[2]);
            this->m_wheel_orig_relpo[3] = this->m_wheel_orig_relpo[2];
            y = this->m_wheel_orig_relpo[2].w.y;
            backLeftX = this->m_wheel_orig_relpo[2].w.x + 0.0;
            //backLeftSepY = y + COERCE_FLOAT(LODWORD(wheelSeperation) ^ _mask__NegFloat_);
            backLeftSepY = y + (-(wheelSeperation));
            backLeftZ = this->m_wheel_orig_relpo[2].w.z + 0.0;
            backLeftWheelPos = &this->m_wheel_orig_relpo[2].w;
            this->m_wheel_orig_relpo[2].w.x = backLeftX;
            backLeftWheelPos->y = backLeftSepY;
            backLeftWheelPos->z = backLeftZ;
            backRightSepY = y + wheelSeperation;
            backRightWheelPos = &this->m_wheel_orig_relpo[3].w;
            this->m_wheel_orig_relpo[3].w.x = backLeftX;
            backRightWheelPos->y = backRightSepY;
            backRightWheelPos->z = backLeftZ;
        }
        else if (!Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
            678,
            0,
            "Failed to find tag_wheel_back for motorcycle."))
        {
            __debugbreak();
        }
        unusedWheelPos4 = &this->m_wheel_orig_relpo[4].w;
        this->m_wheel_orig_relpo[4].w.x = 0.0f;
        unusedWheelPos4->y = 0.0f;
        unusedWheelPos4->z = 0.0f;
        unusedWheelPos5 = &this->m_wheel_orig_relpo[5].w;
        this->m_wheel_orig_relpo[5].w.x = 0.0f;
        unusedWheelPos5->y = 0.0f;
        unusedWheelPos5->z = 0.0f;
    }
    else
    {
        for (i = 0; i < 6; ++i)
        {
            index = 0;
            if (DObjGetBoneIndex(obj, *s_wheelTags[i], &index, -1))
            {
                DObjGetBasePoseMatrix(obj, index, &mat);
                Ragdoll_AnimMatToMat43(&mat, mat34);
                Phys_Mat4ToNitrousMat(mat34, &this->m_wheel_orig_relpo[i]);
            }
            else
            {
                wheelPos = &this->m_wheel_orig_relpo[i].w;
                wheelPos->x = 0.0f;
                wheelPos->y = 0.0f;
                wheelPos->z = 0.0f;
            }
            this->m_wheel_damage[i] = 0;
        }
    }
}

// Decomp:  CoDMPServer.c:656751  (RVA 008227F0)
void __cdecl Phys_Mat4ToNitrousMat(float (*inMat)[3], phys_mat44 *outMat)
{
    Phys_Vec3ToNitrousVec((float *)inMat, &outMat->x);
    Phys_Vec3ToNitrousVec(&(*inMat)[3], &outMat->y);
    Phys_Vec3ToNitrousVec(&(*inMat)[6], &outMat->z);
    Phys_Vec3ToNitrousVec(&(*inMat)[9], &outMat->w);
}

// Decomp:  CoDMPServer.c:656760  (RVA 00822850)
double NitrousVehicle::_calc_initial_susp_spring_k(rigid_body_constraint_wheel *wheel_constraint)
{
    float comFactorLow;
    float comFactorHigh;
    float susp_spring_k;
    susp_spring_k = fabs(wheel_constraint->m_b1_wheel_center_loc.x)
                                / this->m_steer_front_back_length;
    if ( (float)(susp_spring_k - 1.0) < 0.0 )
        comFactorHigh = fabs(wheel_constraint->m_b1_wheel_center_loc.x)
             / this->m_steer_front_back_length;
    else
        comFactorHigh = 1.0f;
    if ( (float)(0.0 - susp_spring_k) < 0.0 )
        comFactorLow = comFactorHigh;
    else
        comFactorLow = 0.0f;
    return (float)((float)((float)(this->m_parameter->m_susp_spring_k * phys_vehicleGravityMultiplier->current.value)
                                             * (float)((float)(1.0 - comFactorLow) * 2.0))
                             * this->m_parameter->m_body_mass);
}

// Decomp:  CoDMPServer.c:656791  (RVA 00822990)
bool NitrousVehicle::is_peeling_out()
{
    return this->m_throttle > 0.89999998 && this->m_parameter->m_peel_out_max_speed > this->m_forward_vel;
}

unsigned int s_tankWheelJnts[6];
// Decomp:  CoDMPServer.c:656798  (RVA 008229E0)
void NitrousVehicle::destructible_damage(const char *notifyLabel, int damageStage)
{
    int wheelTagIndex;
    int i;
    int max;
    int min;
    min = 0;
    max = -1;
    if (notifyLabel)
    {
        if (!s_tankWheelJnts[0])
        {
            s_tankWheelJnts[0] = scr_const.tag_wheel_front_left;
            s_tankWheelJnts[1] = scr_const.tag_wheel_middle_left;
            s_tankWheelJnts[2] = scr_const.tag_wheel_back_left;
            s_tankWheelJnts[3] = scr_const.tag_wheel_front_right;
            s_tankWheelJnts[4] = scr_const.tag_wheel_middle_right;
            s_tankWheelJnts[5] = scr_const.tag_wheel_back_right;
        }
        if (I_strcmp(notifyLabel, "left_tread_destroyed"))
        {
            if (!I_strcmp(notifyLabel, "right_tread_destroyed"))
            {
                min = 3;
                max = 6;
                this->m_speed_scale = 0.0f;
            }
        }
        else
        {
            min = 0;
            max = 3;
            this->m_speed_scale = 0.0f;
        }
        if (this->m_speed_scale < 0.0)
            this->m_speed_scale = 1.0f;
        for (i = min; i < max; ++i)
        {
            for (wheelTagIndex = 0; wheelTagIndex < 6; ++wheelTagIndex)
            {
                if (s_tankWheelJnts[i] == *s_wheelTags[wheelTagIndex])
                    this->m_wheels[wheelTagIndex]->m_wheel_radius = this->m_parameter->m_wheel_radius * tempRadiusPercent;
            }
        }
    }
}

// Decomp:  CoDMPServer.c:13153  (idb prototype only — no separate //----- body in CoDMPServer.c MP dump)
void NitrousVehicle::damage(unsigned int boneName, int damageStage)
{
    ;
}

// Decomp:  CoDMPServer.c:656854  (RVA 00822BA0)
void NitrousVehicle::damage(int damage, const float *point, const float *dir, int mod)
{
    gentity_s *m_owner;
    gentity_s *tempent;
    float force[3];
    float forcescale;
    bool doForce;
    doForce = mod >= 3 && (mod <= 6 || mod == 15);
    if ( this->m_phys_user_data && doForce )
    {
        if ( damage <= 250 )
        {
            if ( damage < 50 )
                damage = 50;
        }
        else
        {
            damage = 250;
        }
        force[0] = *dir;
        force[1] = dir[1];
        force[2] = dir[2];
        Vec3NormalizeFast(force);
        forcescale = phys_vehicleDamageFroceScale->current.value;
        force[0] = (float)((float)damage * forcescale) * force[0];
        force[1] = (float)((float)damage * forcescale) * force[1];
        force[2] = (float)((float)damage * forcescale) * force[2];
        if ( this->m_owner )
        {
            tempent = G_TempEntity(force, EV_PHYS_LAUNCH);
            AssignToSmallerType<short>(&tempent->s.otherEntityNum, this->m_owner->s.number);
            tempent->s.lerp.pos.trDelta[0] = force[0];
            tempent->s.lerp.pos.trDelta[1] = force[1];
            tempent->s.lerp.pos.trDelta[2] = force[2];
            m_owner = this->m_owner;
            tempent->s.lerp.pos.trDelta[0] = *point - m_owner->r.currentOrigin[0];
            tempent->s.lerp.pos.trDelta[1] = point[1] - m_owner->r.currentOrigin[1];
            tempent->s.lerp.pos.trDelta[2] = point[2] - m_owner->r.currentOrigin[2];
        }
    }
}

// Decomp:  CoDMPServer.c:656906  (RVA 00822D60)
phys_vec3 *NitrousVehicle::get_velocity(phys_vec3 *result)
{
    rigid_body *body; // ecx
    float relVelY;
    float relVelZ;
    if ( this->m_phys_user_data )
    {
        body = this->m_phys_user_data->body;
        relVelY = body->m_t_vel.y - this->m_ground_vel.y;
        relVelZ = body->m_t_vel.z - this->m_ground_vel.z;
        result->x = body->m_t_vel.x - this->m_ground_vel.x;
        result->y = relVelY;
        result->z = relVelZ;
    }
    else
    {
        result->x = 0.0f;
        result->y = 0.0f;
        result->z = 0.0f;
    }
    return result;
}

// Decomp:  CoDMPServer.c:656933  (RVA 00822E40)
phys_vec3 *NitrousVehicle::get_a_velocity(phys_vec3 *result)
{
    if ( this->m_phys_user_data )
    {
        *result = this->m_phys_user_data->body->m_a_vel;
    }
    else
    {
        result->x = 0.0f;
        result->y = 0.0f;
        result->z = 0.0f;
    }
    return result;
}

// Decomp:  CoDMPServer.c:656955  (RVA 00822EC0)
void NitrousVehicle::_update_orientation_constraint()
{
    bool noOrientationCorrection;
    bool orientationActive;
    rigid_body_constraint_wheel *rbc_wheel;
    int i;
    if ( !this->m_orientation_constraint
        && _tlAssert(
                 "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
                 880,
                 "m_orientation_constraint",
                 "") )
    {
        __debugbreak();
    }
    if ( !this->m_phys_user_data
        && _tlAssert(
                 "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
                 881,
                 "m_phys_user_data",
                 "") )
    {
        __debugbreak();
    }
    if ( !this->m_phys_user_data->body
        && _tlAssert(
                 "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
                 882,
                 "m_phys_user_data->body",
                 "") )
    {
        __debugbreak();
    }
    orientationActive = ((this->m_flags & 4) != 0 || (this->m_flags & 0x40) != 0 || (this->m_flags & 0x100) != 0)
        && (this->m_flags & 0x200) == 0
        || (this->m_flags & 0x80) != 0;
    noOrientationCorrection = !this->m_num_colliding_wheels && !this->m_phys_user_data->body->m_contact_count;
    this->m_orientation_constraint->m_active = orientationActive;
    this->m_orientation_constraint->m_no_orientation_correction = noOrientationCorrection;
    if ( (this->m_flags & 0x1000) != 0 )
    {
        this->m_orientation_constraint->m_active = 0;
        this->m_orientation_constraint->m_torque_resistance_pitch_roll = 0.0f;
        this->m_orientation_constraint->m_torque_resistance_yaw = 0.0f;
        this->m_orientation_constraint->m_upright_strength = 0.0f;
        this->m_orientation_constraint->m_no_orientation_correction = 1;
        for ( i = 0; i < 6; ++i )
        {
            rbc_wheel = this->m_wheels[i];
            if ( rbc_wheel )
                rbc_wheel->m_roll_stability_factor = 0.0f;
        }
    }
}

// Decomp:  CoDMPServer.c:657013  (RVA 00823130)
double NitrousVehicle::_get_stuck_accel_factor(float delta_t)
{
    float stuckAccelScale;
    float accel_scale;
    float abs_steer_factor;
    if ( !this->m_phys_user_data )
        return 1.0;
    if ( this->m_throttle <= 0.2 )
    {
        this->m_stuck_time = 0.0f;
    }
    else if ( this->m_phys_user_data )
    {
        abs_steer_factor = fabs(this->m_steer_factor);
        if ( (float)(vel_threshold * vel_threshold) <= (float)((float)((float)(this->m_phys_user_data->body->m_t_vel.x
                                                                                                                                                 * this->m_phys_user_data->body->m_t_vel.x)
                                                                                                                                 + (float)(this->m_phys_user_data->body->m_t_vel.y
                                                                                                                                                 * this->m_phys_user_data->body->m_t_vel.y))
                                                                                                                 + (float)(this->m_phys_user_data->body->m_t_vel.z
                                                                                                                                 * this->m_phys_user_data->body->m_t_vel.z))
            || abs_steer_factor >= 0.30000001 )
        {
            if ( (float)(avel_threshold * avel_threshold) <= (float)((float)((float)(this->m_phys_user_data->body->m_a_vel.x
                                                                                                                                                         * this->m_phys_user_data->body->m_a_vel.x)
                                                                                                                                         + (float)(this->m_phys_user_data->body->m_a_vel.y
                                                                                                                                                         * this->m_phys_user_data->body->m_a_vel.y))
                                                                                                                         + (float)(this->m_phys_user_data->body->m_a_vel.z
                                                                                                                                         * this->m_phys_user_data->body->m_a_vel.z))
                || abs_steer_factor <= 0.5 )
            {
                this->m_stuck_time = 0.0f;
            }
            else
            {
                this->m_stuck_time = this->m_stuck_time + delta_t;
            }
        }
        else
        {
            this->m_stuck_time = this->m_stuck_time + delta_t;
        }
    }
    if ( this->m_stuck_time <= 0.40000001 )
        return 1.0;
    accel_scale = (float)((float)(this->m_stuck_time - 0.40000001) * 30.0) + 1.0;
    if ( (float)(accel_scale - 40.0) < 0.0 )
        stuckAccelScale = (float)((float)(this->m_stuck_time - 0.40000001) * 30.0) + 1.0;
    else
        stuckAccelScale = 40.0f;
    if ( (float)(1.0 - accel_scale) < 0.0 )
        return stuckAccelScale;
    else
        return 1.0f;
}

// Decomp:  CoDMPServer.c:657075  (RVA 00823390)
void NitrousVehicle::_update_boost(float delta_t)
{
    float boostDuration;
    float m_boost_time_pool;
    bool inContinuousRegenCooldown;
    bool inBoostIntervalCooldown;
    bool tryingToBoost;
    bool wasBoosting;
    //tryingToBoost = bitarray<51>::testBit(&this->mVehicleController.m_cmd.button_bits, 0x2Bu);
    tryingToBoost = this->mVehicleController.m_cmd.button_bits.testBit(0x2Bu);
    wasBoosting = (this->m_flags & 0x800) != 0;
    if ( (this->m_flags & 0x800) != 0 )
        this->m_boost_time_pool = this->m_boost_time_pool - delta_t;
    if ( this->m_boost_start_time )
        inBoostIntervalCooldown = this->mVehicleController.m_cmd.serverTime < this->m_boost_start_time
                                                                                                     + (int)(float)(this->m_vehicle_info->boostInterval * 1000.0);
    else
        inBoostIntervalCooldown = 0;
    if ( this->m_boost_start_time )
        inContinuousRegenCooldown = this->mVehicleController.m_cmd.serverTime < this->m_boost_start_time
                                                                                                     + (int)(float)(this->m_vehicle_info->boostContinuousRegenCooldown
                                                                                                                                * 1000.0);
    else
        inContinuousRegenCooldown = 1;
    if ( this->m_vehicle_info->boostMode )
    {
        if ( this->m_vehicle_info->boostMode != 1 )
            goto LABEL_37;
        if ( !tryingToBoost || this->m_boost_time_pool <= 0.0 )
        {
            if ( !tryingToBoost && !inContinuousRegenCooldown && this->m_vehicle_info->boostDuration > this->m_boost_time_pool )
            {
                this->m_boost_time_pool = (float)(this->m_vehicle_info->boostContinuousRegenAmount * delta_t)
                                                                + this->m_boost_time_pool;
                if ( (float)(this->m_vehicle_info->boostDuration - this->m_boost_time_pool) < 0.0 )
                    boostDuration = this->m_vehicle_info->boostDuration;
                else
                    boostDuration = this->m_boost_time_pool;
                this->m_boost_time_pool = boostDuration;
            }
            goto LABEL_37;
        }
LABEL_19:
        NitrousVehicle::set_boosting(1);
        return;
    }
    if ( this->m_vehicle_info->boostInterval > this->m_vehicle_info->boostDuration )
    {
        if ( wasBoosting && this->m_boost_time_pool > 0.0 )
        {
            NitrousVehicle::set_boosting(wasBoosting);
            return;
        }
        if ( !tryingToBoost || this->m_boost_time_pool <= 0.0 || inBoostIntervalCooldown )
        {
            if ( inBoostIntervalCooldown && this->m_vehicle_info->boostDuration > this->m_boost_time_pool )
            {
                this->m_boost_time_pool = (float)((float)(this->m_vehicle_info->boostDuration
                                                                                                / (float)(this->m_vehicle_info->boostInterval
                                                                                                                - this->m_vehicle_info->boostDuration))
                                                                                * delta_t)
                                                                + this->m_boost_time_pool;
                if ( (float)(this->m_vehicle_info->boostDuration - this->m_boost_time_pool) < 0.0 )
                    m_boost_time_pool = this->m_vehicle_info->boostDuration;
                else
                    m_boost_time_pool = this->m_boost_time_pool;
                this->m_boost_time_pool = m_boost_time_pool;
            }
            goto LABEL_37;
        }
        goto LABEL_19;
    }
    Com_PrintWarning(15, "boostDuration duration must be less than boostInterval\n");
LABEL_37:
    NitrousVehicle::set_boosting(0);
}

// Decomp:  CoDMPServer.c:657157  (RVA 00823740)
void NitrousVehicle::_update_prolog(float delta_t)
{
    const VehicleParameter *m_parameter;
    const VehicleParameter *paramsForAccel;
    double accelGraphValue;
    float tractionAccelFactor;
    rigid_body *body;
    float clampedTractionAccel;
    float maximum_acceleration_factor;
    float fraction;
    float forwardSpeed;
    float climbAccelForce;
    float speed;
    float clampedFakeySpeed;
    float fakeyBlend;
    float clampedFakeyBlend;
    float clampedPitchFactor;
    float clampedPitchLow;
    float pitchOffsetLow;
    float pitchOffsetHigh;
    float pitchOffsetHighClamp;
    float z;
    int activeTreadCount;
    rigid_body_constraint_wheel *wheel;
    int wheelIndex;
    int groundedWheelCount;
    float maxSpeedUnscaled;
    float desiredSpeedFactor;
    float baseAccelForce;
    float tankThrottleAccel;
    float desiredAccelForce;
    //unsigned int v33[2];
    //_UNKNOWN *retaddr;
    //
    //v33[0] = a2;
    //v33[1] = retaddr;
    NitrousVehicle::_update_pause();
    this->m_notify_flags = 0;
    //NitrousVehicleController::UpdateControls(&this->mVehicleController, COERCE_FLOAT(v33), this, delta_t);
    this->mVehicleController.UpdateControls(this, delta_t);
    NitrousVehicle::_update_boost(delta_t);
    if ( this->m_throttle <= 0.0 )
        this->m_state_flags &= ~4u;
    else
        this->m_state_flags |= 4u;
    if ( this->m_hand_brake <= 0.0 )
        this->m_state_flags &= ~1u;
    else
        this->m_state_flags |= 1u;
    m_parameter = this->m_parameter;
    baseAccelForce = NitrousVehicle::get_maximum_acceleration() * m_parameter->m_body_mass;
    maxSpeedUnscaled = NitrousVehicle::get_max_speed(1);
    desiredSpeedFactor = maxSpeedUnscaled * this->m_speed_scale;
    if ( this->m_vehicle_info->type == 2 )
    {
        groundedWheelCount = 0;
        for ( wheelIndex = 0; wheelIndex < 6; ++wheelIndex )
        {
            wheel = this->m_wheels[wheelIndex];
            if ( wheel )
            {
                if ( (wheel->m_wheel_flags & 1) != 0 )
                    ++groundedWheelCount;
            }
        }
        if ( groundedWheelCount <= 1 )
            activeTreadCount = 2;
        else
            activeTreadCount = groundedWheelCount;
        tankThrottleAccel = baseAccelForce * (float)(fabs(this->m_throttle) * (float)(6.0 / (float)activeTreadCount));
        desiredAccelForce = NitrousVehicle::_get_stuck_accel_factor(delta_t) * tankThrottleAccel;
    }
    else
    {
        desiredAccelForce = fabs(this->m_throttle) * baseAccelForce;
        if ( this->m_throttle < 0.0 )
        {
            desiredAccelForce = desiredAccelForce * this->m_parameter->m_reverse_scale;
            desiredSpeedFactor = desiredSpeedFactor * this->m_parameter->m_reverse_scale;
        }
    }
    if ( this->m_phys_user_data )
    {
        z = this->m_phys_user_data->body->m_mat.x.z;
        if ( phys_heavyTankSwitch->current.enabled )
        {
            if ( z <= 0.0 )
            {
                if ( (float)((float)(z + pitchFakeyZone) - 0.0) < 0.0 )
                    pitchOffsetLow = z + pitchFakeyZone;
                else
                    pitchOffsetLow = 0.0f;
                if ( (float)(-1.0 - (float)(z + pitchFakeyZone)) < 0.0 )
                    clampedPitchLow = pitchOffsetLow;
                else
                    clampedPitchLow = -1.0f;
                z = clampedPitchLow;
            }
            else
            {
                if ( (float)((float)(z - pitchFakeyZone) - 1.0) < 0.0 )
                    pitchOffsetHighClamp = z - pitchFakeyZone;
                else
                    pitchOffsetHighClamp = 1.0f;
                if ( (float)(0.0 - (float)(z - pitchFakeyZone)) < 0.0 )
                    pitchOffsetHigh = pitchOffsetHighClamp;
                else
                    pitchOffsetHigh = 0.0f;
                z = pitchOffsetHigh;
            }
        }
        if ( (float)((float)(z / 0.40000001) - 1.0) < 0.0 )
            clampedPitchFactor = z / 0.40000001;
        else
            clampedPitchFactor = 1.0f;
        if ( (float)(-1.0 - (float)(z / 0.40000001)) < 0.0 )
            clampedFakeyBlend = clampedPitchFactor;
        else
            clampedFakeyBlend = -1.0f;
        fakeyBlend = (float)((float)(1.0 - fabs(this->m_steer_factor)) * this->m_throttle)
                * clampedFakeyBlend;
        if ( (float)(fakeyBlend - 1.0) < 0.0 )
            clampedFakeySpeed = (float)((float)(1.0 - fabs(this->m_steer_factor)) * this->m_throttle)
                    * clampedFakeyBlend;
        else
            clampedFakeySpeed = 1.0f;
        if ( (float)(0.0 - fakeyBlend) < 0.0 )
            speed = clampedFakeySpeed;
        else
            speed = 0.0f;
        paramsForAccel = this->m_parameter;
        climbAccelForce = NitrousVehicle::get_maximum_acceleration() * paramsForAccel->m_body_mass;
        if ( phys_heavyTankSwitch->current.enabled )
            desiredAccelForce = (float)(speed * climbHeavyAccelAdd) + desiredAccelForce;
        desiredAccelForce = (float)(speed * climbAccelForce) + desiredAccelForce;
    }
    if ( (this->m_flags & 1) == 0 )
    {
        forwardSpeed = fabs(this->m_forward_vel);
        if ( this->m_vehicle_info->accelGraph )
        {
            if ( (float)((float)(forwardSpeed / desiredSpeedFactor) - 1.0) < 0.0 )
                fraction = forwardSpeed / desiredSpeedFactor;
            else
                fraction = 1.0f;
            if ( (float)(0.0 - (float)(forwardSpeed / desiredSpeedFactor)) < 0.0 )
                accelGraphValue = GraphFloat_GetValue(this->m_vehicle_info->accelGraph, fraction);
            else
                accelGraphValue = GraphFloat_GetValue(this->m_vehicle_info->accelGraph, 0.0);
            desiredAccelForce = accelGraphValue * desiredAccelForce;
        }
        else if ( low_end_speed > forwardSpeed )
        {
            desiredAccelForce = (float)((float)(desiredAccelForce * (float)(1.0 - (float)(forwardSpeed / low_end_speed))) * percent_to_give)
                                        + desiredAccelForce;
        }
    }
    if ( scale_max_speed )
        desiredSpeedFactor = desiredSpeedFactor * fabs(this->m_throttle);
    if ( desiredAccelForce <= NitrousVehicle::get_maximum_acceleration_factor() )
        maximum_acceleration_factor = NitrousVehicle::get_maximum_acceleration_factor();
    else
        maximum_acceleration_factor = desiredAccelForce;
    this->m_acceleration_factor = maximum_acceleration_factor;
    this->m_desired_speed_factor = desiredSpeedFactor;
    if ( this->m_hand_brake <= 0.0 && this->m_throttle <= 0.0 )
        this->m_state_flags |= 0x10u;
    else
        this->m_state_flags &= ~0x10u;
    if ( this->m_brake <= 0.0 )
        this->m_state_flags &= ~2u;
    else
        this->m_state_flags |= 2u;
    this->m_state_flags &= ~8u;
    if ( this->m_parameter->m_traction_type == TRACTION_TYPE_ALL_WD )
    {
        if ( this->m_hand_brake <= 0.0 )
            tractionAccelFactor = desiredAccelForce;
        else
            tractionAccelFactor = desiredAccelForce * 2.0;
        if ( tractionAccelFactor <= NitrousVehicle::get_maximum_acceleration_factor() )
            clampedTractionAccel = NitrousVehicle::get_maximum_acceleration_factor();
        else
            clampedTractionAccel = tractionAccelFactor;
        this->m_acceleration_factor = clampedTractionAccel;
    }
    if ( this->m_phys_user_data )
    {
        body = this->m_phys_user_data->body;
        this->m_forward_vel = (float)((float)((float)(body->m_t_vel.x - this->m_ground_vel.x) * body->m_mat.x.x)
                                                                + (float)((float)(body->m_t_vel.y - this->m_ground_vel.y) * body->m_mat.x.y))
                                                + (float)((float)(body->m_t_vel.z - this->m_ground_vel.z) * body->m_mat.x.z);
        if ( this->m_throttle >= 0.0 )
        {
            if ( this->m_throttle > 0.0
                && this->m_parameter->m_peel_out_max_speed > 5.0
                //&& COERCE_FLOAT(LODWORD(this->m_parameter->m_peel_out_max_speed) ^ _mask__NegFloat_) > this->m_forward_vel )
                && (-(this->m_parameter->m_peel_out_max_speed)) > this->m_forward_vel )
            {
                this->m_state_flags |= 2u;
            }
        }
        else if ( this->m_forward_vel <= 10.0 || this->m_num_colliding_wheels <= 1 )
        {
            this->m_state_flags |= 8u;
        }
        else
        {
            this->m_state_flags |= 2u;
        }
    }
    else
    {
        this->m_forward_vel = 0.0f;
    }
    if ( this->m_script_brake > 0.0 )
        this->m_state_flags |= 2u;
    if ( (this->m_flags & 1) == 0 )
    {
        NitrousVehicle::update_braking_and_acceleration(delta_t);
        NitrousVehicle::update_steering(delta_t);
        NitrousVehicle::_update_fakey_stuff(delta_t);
        NitrousVehicle::_update_friction(delta_t);
        NitrousVehicle::_update_orientation_constraint();
        if ( (this->m_flags & 0x80) != 0 )
        {
            if ( !this->m_vpc )
                NitrousVehicle::start_path(0);
            if ( this->m_vpc )
                path_constraint_update(this->m_vpc, this->m_owner);
        }
    }
}

// Decomp:  CoDMPServer.c:657423  (RVA 00824580)
void NitrousVehicle::_update_epilog(float delta_t)
{
    NitrousVehicle::_update_wheel_effect_states(delta_t);
}

// Decomp:  CoDMPServer.c:657431  (RVA 008245A0)
void NitrousVehicle::_update_friction(float delta_t)
{
    float minFricScaler;
    float clampedTiltFric;
    float fric_scaler;
    float fric_scalera;
    float up;
    if ( (this->m_flags & 0x80) == 0 )
    {
        this->m_hand_brake_friction_time = this->m_hand_brake_friction_time - delta_t;
        if ( this->m_hand_brake > 0.1 && this->m_parameter->m_tire_fric_hand_brake > 0.0 )
            this->m_hand_brake_friction_time = hand_brake_friction_time;
        up = this->m_phys_user_data->body->m_mat.z.z;
        fric_scaler = 1.0f;
        if ( this->m_vehicle_info->max_fric_tilt_angle > up && this->m_vehicle_info->max_fric_tilt_angle > 0.0 )
        {
            fric_scalera = (float)(this->m_vehicle_info->max_fric_tilt / this->m_vehicle_info->max_fric_tilt_angle) * up;
            if ( (float)(fric_scalera - 1.0) < 0.0 )
                clampedTiltFric = (float)(this->m_vehicle_info->max_fric_tilt / this->m_vehicle_info->max_fric_tilt_angle) * up;
            else
                clampedTiltFric = 1.0f;
            if ( (float)(0.15000001 - fric_scalera) < 0.0 )
                minFricScaler = clampedTiltFric;
            else
                minFricScaler = 0.15000001;
            fric_scaler = minFricScaler;
        }
        if ( NitrousVehicle::is_boosting() && this->m_vehicle_info->boostFrictionMultiplier > 0.0 )
            fric_scaler = fric_scaler * this->m_vehicle_info->boostFrictionMultiplier;
        if ( (this->m_flags & 0x80) != 0 )
        {
            this->m_current_front_side_fric_scale = 0.0f;
            this->m_current_front_fwd_fric_scale = 0.0f;
        }
        else if ( this->m_brake <= 0.1 )
        {
            if ( this->m_hand_brake_friction_time <= 0.0 )
            {
                this->m_current_front_side_fric_scale = (float)(this->m_parameter->m_tire_fric_side * fric_scaler)
                                                                                            * side_fric_scale;
                this->m_current_front_fwd_fric_scale = (float)(this->m_parameter->m_tire_fric_fwd * fric_scaler)
                                                                                         * fwd_fric_scale;
            }
            else
            {
                this->m_current_front_side_fric_scale = this->m_parameter->m_tire_fric_hand_brake * fric_scaler;
                if ( fabs(this->m_throttle) <= 0.1 )
                    this->m_current_front_fwd_fric_scale = this->m_current_front_side_fric_scale;
                else
                    this->m_current_front_fwd_fric_scale = this->m_parameter->m_tire_fric_fwd * fric_scaler;
            }
        }
        else
        {
            this->m_current_front_side_fric_scale = this->m_parameter->m_tire_fric_brake * fric_scaler;
            this->m_current_front_fwd_fric_scale = this->m_current_front_side_fric_scale;
        }
        this->m_current_rear_side_fric_scale = this->m_current_front_side_fric_scale;
        this->m_current_rear_fwd_fric_scale = this->m_current_front_fwd_fric_scale;
    }
}

// Decomp:  CoDMPServer.c:657499  (RVA 00824910)
void NitrousVehicle::_update_fakey_stuff(float delta_t)
{
    environment_rigid_body *env_rigid_body;
    float rpmSpeedRatio;
    rigid_body_constraint_wheel *wheel;
    int j;
    rigid_body_constraint_wheel *rbc_wheel;
    int i;
    float rpm_lerp_rate;
    environment_rigid_body *rb;
    float spring_delta;
    float spring_k;
    int num_colliding_wheels;
    bool high_rpm;
    float rpm_target;
    bool peeling_out;
    rigid_body_constraint_wheel *frontRightWheel;
    rigid_body_constraint_wheel *frontLeftWheel;
    rigid_body_constraint_wheel *rearRightWheel;
    rigid_body_constraint_wheel *rearLeftWheel;
    rb = (environment_rigid_body *)this->m_phys_user_data->body;
    num_colliding_wheels = 0;
    spring_delta = 0.0f;
    if ( this->m_throttle <= 0.5 )
    {
        if (this->m_throttle < -0.5)
        {
            //spring_delta = (float)(COERCE_FLOAT(LODWORD(this->m_parameter->m_susp_spring_k) ^ _mask__NegFloat_) * this->m_parameter->m_tilt_fakey) * this->m_parameter->m_body_mass;
            spring_delta = (float)((-(this->m_parameter->m_susp_spring_k)) * this->m_parameter->m_tilt_fakey) * this->m_parameter->m_body_mass;
        }
    }
    else
    {
        spring_delta = (float)(this->m_parameter->m_susp_spring_k * this->m_parameter->m_tilt_fakey)
                                 * this->m_parameter->m_body_mass;
    }
    spring_k = 30.0f;
    frontRightWheel = this->m_wheels[1];
    if ( frontRightWheel )
    {
        spring_k = NitrousVehicle::_calc_initial_susp_spring_k(frontRightWheel);
        frontRightWheel->m_suspension_stiffness_k = spring_k + spring_delta;
        if ( (frontRightWheel->m_wheel_flags & 1) != 0 )
            num_colliding_wheels = 1;
    }
    frontLeftWheel = this->m_wheels[0];
    if ( frontLeftWheel )
    {
        frontLeftWheel->m_suspension_stiffness_k = spring_k + spring_delta;
        if ( (frontLeftWheel->m_wheel_flags & 1) != 0 )
            ++num_colliding_wheels;
    }
    rearRightWheel = this->m_wheels[3];
    if ( rearRightWheel )
    {
        spring_k = NitrousVehicle::_calc_initial_susp_spring_k(rearRightWheel);
        if ( (rearRightWheel->m_wheel_flags & 1) != 0 )
            ++num_colliding_wheels;
        rearRightWheel->m_suspension_stiffness_k = spring_k - spring_delta;
    }
    rearLeftWheel = this->m_wheels[2];
    if ( rearLeftWheel )
    {
        rearLeftWheel->m_suspension_stiffness_k = spring_k - spring_delta;
        if ( (rearLeftWheel->m_wheel_flags & 1) != 0 )
            ++num_colliding_wheels;
    }
    if ( !this->m_num_colliding_wheels || num_colliding_wheels )
    {
        if ( !this->m_num_colliding_wheels && num_colliding_wheels )
        {
            this->m_notify_flags |= 4u;
            this->m_server_notify_flags |= 4u;
        }
    }
    else
    {
        this->m_notify_flags |= 2u;
        this->m_server_notify_flags |= 2u;
    }
    for ( i = 0; i < 6; ++i )
    {
        rbc_wheel = this->m_wheels[i];
        if ( rbc_wheel && (rbc_wheel->m_wheel_flags & 2) != 0 )
        {
            this->m_notify_flags |= 8u;
            this->m_server_notify_flags |= 8u;
            break;
        }
    }
    this->m_num_colliding_wheels = num_colliding_wheels;
    if ( this->mVehicleController.m_stun.m_changed_this_frame )
    {
        this->m_notify_flags |= 0x10u;
        this->m_server_notify_flags |= 0x10u;
    }
    if ( !this->m_orientation_constraint )
    {
        env_rigid_body = phys_sys::get_environment_rigid_body();
        this->m_orientation_constraint = phys_sys::create_rbc_custom_orientation(rb, env_rigid_body, 0);
        this->m_orientation_constraint->m_torque_resistance_pitch_roll = this->m_parameter->m_pitch_roll_resistance
                                                                                                                                     * this->m_parameter->m_body_mass;
        this->m_orientation_constraint->m_torque_resistance_yaw = this->m_parameter->m_yaw_resistance
                                                                                                                        * this->m_parameter->m_body_mass;
        this->m_orientation_constraint->m_upright_strength = this->m_parameter->m_upright_strength
                                                                                                             * this->m_parameter->m_body_mass;
    }
    peeling_out = NitrousVehicle::is_peeling_out();
    high_rpm = 0;
    if ( this->m_vehicle_info->type != 2 && this->m_vehicle_info->type != 4 )
    {
        for ( j = 0; j < 6; ++j )
        {
            wheel = this->m_wheels[j];
            if ( wheel && !wheel->m_wheel_state && (peeling_out || this->m_throttle > 0.69999999 && (wheel->m_wheel_flags & 1) == 0) )
            {
                high_rpm = 1;
                wheel->m_wheel_pos = (float)(40.0 * delta_t) + wheel->m_wheel_pos;
            }
        }
    }
    if ( peeling_out && this->m_throttle > 0.69999999 )
        high_rpm = 1;
    rpmSpeedRatio = this->m_forward_vel / NitrousVehicle::get_max_speed(1);
    rpm_target = fabs(this->m_throttle) * fabs(rpmSpeedRatio);
    if ( this->m_vehicle_info->type == 4 )
    {
        if ( (float)(this->m_throttle * 0.80000001) > rpm_target )
            rpm_target = this->m_throttle * 0.80000001;
        if ( (this->m_flags & 0x400) == 0 )
            rpm_target = 1.0f;
    }
    if ( high_rpm && rpm_target < 0.80000001 )
        rpm_target = 0.8f;
    rpm_lerp_rate = 1.5f;
    if ( snd_boat_using_lerp_rpm && snd_boat_using_lerp_rpm->current.enabled )
        rpm_lerp_rate = snd_boat_lerp_rpm->current.value;
    this->m_fake_rpm = (float)((float)(delta_t * (float)(rpm_target - this->m_fake_rpm)) * rpm_lerp_rate) + this->m_fake_rpm;
}

// Decomp:  CoDMPServer.c:657653  (RVA 00825060)
void NitrousVehicle::start_path(int attach_mode)
{
    float axis[9];
    float *currentOrigin;
    float origin[3];
    rigid_body_constraint_wheel *rbc_wheel;
    int i;
    Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
    Sys_EnterCriticalSection(CRITSECT_PHYSICS);
    this->m_flags &= ~4u;
    this->m_flags &= ~0x100u;
    this->m_flags &= ~0x80u;
    if ( attach_mode )
    {
        this->m_flags |= 0x100u;
        NitrousVehicle::unpause_physics();
    }
    else
    {
        this->m_flags |= 0x80u;
        if ( (this->m_flags & 1) != 0 )
        {
            currentOrigin = this->m_owner->r.currentOrigin;
            origin[0] = *currentOrigin;
            origin[1] = currentOrigin[1];
            origin[2] = currentOrigin[2];
            origin[2] = origin[2] + 40.0;
            AnglesToAxis(this->m_owner->r.currentAngles, (float (*)[3])axis);
            Phys_AxisToNitrousMat((float (*)[3])axis, &this->m_prev_rb_mat);
            Phys_Vec3ToNitrousVec(origin, &this->m_prev_rb_mat.w);
            if ( fabs(this->m_prev_rb_mat.w.x) > 100000.0
                || fabs(this->m_prev_rb_mat.w.y) > 100000.0
                || fabs(this->m_prev_rb_mat.w.z) > 100000.0 )
            {
                phys_exec_debug_callback(0);
            }
            SV_LinkEntity(this->m_owner);
        }
        else if ( !this->m_vpc )
        {
            this->m_vpc = path_constraint_create(this->m_owner);
            for ( i = 0; i < 6; ++i )
            {
                rbc_wheel = this->m_wheels[i];
                if ( rbc_wheel )
                {
                    rbc_wheel->m_fwd_fric_k = 0.01f;
                    rbc_wheel->m_side_fric_k = 0.01f;
                }
            }
        }
    }
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
}

// Decomp:  CoDMPServer.c:657733  (RVA 00825660)
void NitrousVehicle::end_path()
{
    Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
    Sys_EnterCriticalSection(CRITSECT_PHYSICS);
    this->m_flags &= ~0x100u;
    this->m_flags &= ~0x80u;
    NitrousVehicle::cleanup_path();
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
}

// Decomp:  CoDMPServer.c:657752  (RVA 00825740)
void NitrousVehicle::cleanup_path()
{
    if ( this->m_vpc )
    {
        path_constraint_destroy(this->m_vpc);
        this->m_vpc = 0;
    }
}

// Decomp:  CoDMPServer.c:657765  (RVA 00825780)
void __cdecl NitrousVehicle::frame_prolog_all_systems(float delta_t)
{
    // Decomp: CoDMPServer.c advances T_internal and calls _update_prolog on (NitrousVehicle *)&node[2]; m_data is same offset
#if 0
    phys_free_list<NitrousVehicle>::T_internal_base *i;
    //PIXBeginNamedEvent(-1, "NitrousVehicle::frame_prolog");
    for ( i = g_rb_vehicle_list.m_dummy_head.m_next_T_internal; &g_rb_vehicle_list != (phys_free_list<NitrousVehicle> *)i; i = i->m_next_T_internal )
    {
        //NitrousVehicle::_update_prolog((NitrousVehicle *)&i[2], (int)&savedregs, delta_t);
        ((NitrousVehicle *)&i[2])->_update_prolog(delta_t);
    }
    //if ( g_DXDeviceThread == GetCurrentThreadId() )
        //D3DPERF_EndEvent();
#endif
    using List = phys_free_list<NitrousVehicle>;
    using Node = List::T_internal;
    using NodeBase = List::T_internal_base;
    NodeBase *head = &g_rb_vehicle_list.m_dummy_head;
    PROF_SCOPED("NitrousVehicle::frame_prolog");
    for (NodeBase *it = head->m_next_T_internal;
        it != head;
        it = it->m_next_T_internal)
    {
        Node *node = static_cast<Node *>(it);
        NitrousVehicle *vehicle = &node->m_data;
        vehicle->_update_prolog(delta_t);
    }
}

// Decomp:  CoDMPServer.c:657783  (RVA 00825810)
void __cdecl NitrousVehicle::frame_epilog_all_systems(float delta_t)
{
    // Decomp: same iterator pattern as frame_prolog_all_systems (see comment there)
#if 0
    phys_free_list<NitrousVehicle>::T_internal_base *i;
    //PIXBeginNamedEvent(-1, "NitrousVehicle::frame_epilog");
    for ( i = g_rb_vehicle_list.m_dummy_head.m_next_T_internal; &g_rb_vehicle_list != (phys_free_list<NitrousVehicle> *)i; i = i->m_next_T_internal )
    {
        //NitrousVehicle::_update_epilog((NitrousVehicle *)&i[2], delta_t);
        ((NitrousVehicle *)&i[2])->_update_epilog(delta_t);
    }
    //if ( g_DXDeviceThread == GetCurrentThreadId() )
        //D3DPERF_EndEvent();
#endif
    using List = phys_free_list<NitrousVehicle>;
    using Node = List::T_internal;
    using NodeBase = List::T_internal_base;
    NodeBase *head = &g_rb_vehicle_list.m_dummy_head;
    PROF_SCOPED("NitrousVehicle::frame_epilog");
    for (NodeBase *it = head->m_next_T_internal;
        it != head;
        it = it->m_next_T_internal)
    {
        Node *node = static_cast<Node *>(it);
        NitrousVehicle *vehicle = &node->m_data;
        vehicle->_update_epilog(delta_t);
    }
}

// =============================================================================
// NitrousVehicle::add_vehicle
// Decomp:  CoDMPServer.c:657800  (RVA 008258A0)
// Retail:  N/A — symbol stripped in BlackOpsMP.retail.c
// Notes:   IDA bug at 657812 — cast list node to phys_free_list* and read
//          m_ptr_list[244/245]; corrected to T_internal walk comparing vehicle->id
// =============================================================================
NitrousVehicle *__cdecl NitrousVehicle::add_vehicle(int id)
{
    using List = phys_free_list<NitrousVehicle>;
    using Node = List::T_internal;
    using NodeBase = List::T_internal_base;
    NitrousVehicle *vehicle;
    if (id != -1)
    {
        NodeBase *head = &g_rb_vehicle_list.m_dummy_head;
        for (NodeBase *it = head->m_next_T_internal; it != head; it = it->m_next_T_internal)
        {
            Node *node = static_cast<Node *>(it);
            NitrousVehicle *existing = &node->m_data;
            // Decomp: CoDMPServer.c:657812 — vehicle->id == id (was m_ptr_list[244])
            if (existing->id == id)
            {
                // Decomp: CoDMPServer.c:657814 — ++vehicle->refcount (was m_ptr_list[245])
                ++existing->refcount;
                return existing;
            }
        }
    }
    //vehicle = phys_free_list<NitrousVehicle>::add(&g_rb_vehicle_list, 1, "phys_free_list error: out of memory.");
    vehicle = g_rb_vehicle_list.add(1, "phys_free_list error: out of memory.");
    if ((!vehicle || vehicle->refcount)
        && _tlAssert(
            "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
            1553,
            "vehicle && vehicle->refcount == 0",
            "Failed to add a new NitrousVehicle."))
    {
        __debugbreak();
    }
    if ((vehicle->m_flags & 1) == 0
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
            1554,
            0,
            "%s",
            "vehicle->get_flag(FLAG_PAUSED)"))
    {
        __debugbreak();
    }
    vehicle->id = id;
    vehicle->refcount = 1;
    return vehicle;
}

// Decomp:  CoDMPServer.c:657845  (RVA 008259C0)
void __cdecl NitrousVehicle::reinit_parms()
{
    using List = phys_free_list<NitrousVehicle>;
    using Node = List::T_internal;
    using NodeBase = List::T_internal_base;
    NodeBase *head = &g_rb_vehicle_list.m_dummy_head;
    for (NodeBase *it = head->m_next_T_internal;
        it != head;
        it = it->m_next_T_internal)
    {
        Node *node = static_cast<Node *>(it);
        NitrousVehicle *vehicle = &node->m_data;
        vehicle->update_parms(vehicle->m_parameter, 0);
    }
    //phys_free_list<NitrousVehicle>::T_internal_base *i;
    //int savedregs;
    //
    //for ( i = g_rb_vehicle_list.m_dummy_head.m_next_T_internal;
    //            &g_rb_vehicle_list != (phys_free_list<NitrousVehicle> *)i;
    //            i = i->m_next_T_internal )
    //{
    //    NitrousVehicle::update_parms(
    //        (NitrousVehicle *)&i[2],
    //        (int)&savedregs,
    //        (const VehicleParameter *)i[64].m_prev_T_internal,
    //        0);
    //}
}

// Decomp:  CoDMPServer.c:657867  (RVA 00825A30)
void __cdecl NitrousVehicle::reset_vehicle_physics()
{
    using List = phys_free_list<NitrousVehicle>;
    using Node = List::T_internal;
    using NodeBase = List::T_internal_base;
    NodeBase *head = &g_rb_vehicle_list.m_dummy_head;
    for (NodeBase *it = head->m_next_T_internal;
        it != head;
        it = it->m_next_T_internal)
    {
        Node *node = static_cast<Node *>(it);
        NitrousVehicle *vehicle = &node->m_data;
        vehicle->pause_physics(false);
        vehicle->unpause_physics();
    }
    //phys_free_list<NitrousVehicle>::T_internal_base *i;
    //int savedregs;
    //
    //for ( i = g_rb_vehicle_list.m_dummy_head.m_next_T_internal;
    //            &g_rb_vehicle_list != (phys_free_list<NitrousVehicle> *)i;
    //            i = i->m_next_T_internal )
    //{
    //    NitrousVehicle::pause_physics((NitrousVehicle *)&i[2], 0);
    //    NitrousVehicle::unpause_physics((NitrousVehicle *)&i[2], (int)&savedregs);
    //}
}

// Decomp:  CoDMPServer.c:657885  (RVA 00825A90)
void __cdecl NitrousVehicle::remove_vehicle(NitrousVehicle *const v)
{
    if ( !--v->refcount )
    {
        //NitrousVehicle::pause_physics(v, 1);
        v->pause_physics(true);
        //phys_free_list<NitrousVehicle>::remove(&g_rb_vehicle_list, v);
        g_rb_vehicle_list.remove(v);
    }
}

// Decomp:  CoDMPServer.c:657895  (RVA 00825AE0)
void __cdecl NitrousVehicle::debug_render_all()
{
    using List = phys_free_list<NitrousVehicle>;
    using Node = List::T_internal;
    using NodeBase = List::T_internal_base;
    NodeBase *head = &g_rb_vehicle_list.m_dummy_head;
    for (NodeBase *it = head->m_next_T_internal;
        it != head;
        it = it->m_next_T_internal)
    {
        Node *node = static_cast<Node *>(it);
        NitrousVehicle *vehicle = &node->m_data;
        vehicle->debug_render();
    }
    //phys_free_list<NitrousVehicle>::T_internal_base *i;
    //int savedregs;
    //
    //for ( i = g_rb_vehicle_list.m_dummy_head.m_next_T_internal;
    //            &g_rb_vehicle_list != (phys_free_list<NitrousVehicle> *)i;
    //            i = i->m_next_T_internal )
    //{
    //    NitrousVehicle::debug_render((NitrousVehicle *)&i[2], (int)&savedregs);
    //}
}

// Decomp:  CoDMPServer.c:657912  (RVA 00825B30)
void    NitrousVehicle::debug_render()
{
    //phys_vec3 *v2; // eax
    const phys_vec3 *wheelWorldPosResult;
    const phys_vec3 *limitWorldPosResult;
    rigid_body *body; // ecx
    phys_vec3 *scale_4;
    phys_vec3 *scale_4a;
    char buffer[508];
    float textPos[7];
    float pausedVelZ;
    float pausedVelY;
    float pausedVelX;
    phys_mat44 *bodyMat;
    float fwdOffsetX;
    float fwdOffsetY;
    float fwdOffsetZ;
    float fwdOffsetZ2;
    float fwdOffsetY2;
    float fwdOffsetX2;
    phys_mat44 *pausedBodyMat;
    float pausedOffsetZ;
    float z;
    float arm_col[4];
    float _pos[3];
    float _dir[3];
    phys_vec3 dir;
    phys_vec3 m_b1_wheel_axis_loc;
    phys_vec3 limitWorldPos;
    phys_vec3 limitLocalPos;
    phys_mat44 *wheelBodyMat;
    phys_vec3 limit_pos;
    float limitLocalZ;
    float limitLocalY;
    float limitLocalX;
    phys_vec3 m_b1_wheel_center_loc;
    float limitOffsetZ;
    float limitOffsetY;
    float limitOffsetX;
    float hardLimitZ;
    float hardLimitY;
    float hardLimitX;
    float limitRadius;
    phys_vec3 suspDirLocal;
    float m_hard_limit_dist;
    phys_vec3 hit_pos_displaced;
    float hitDisplacedZ;
    float hitDisplacedY;
    float hitDisplacedX;
    float stabOffsetX;
    float stabOffsetY;
    float stabOffsetZ;
    float stabOffsetZ2;
    float stabOffsetY2;
    float stabOffsetX2;
    const phys_vec3 *suspDirWorld;
    phys_vec3 suspDirWorldVec;
    phys_vec3 m_b1_suspension_dir_loc;
    float m_roll_stability_factor;
    phys_vec3 hit_pos;
    phys_vec3 wheelWorldPos;
    phys_vec3 wheelLocalPos;
    phys_mat44 *wheelMat;
    phys_vec3 wheel_pos;
    phys_vec3 suspDisplacement;
    float radius;
    rigid_body *b1;
    rigid_body_constraint_wheel *rbc_wheel;
    float col[4];
    int i;
    float alpha;
    phys_vec3 buoyHalfExtents;
    float buoyHalfZ;
    float buoyHalfY;
    float buoyHalfX;
    phys_mat44 buoyTransform;
    phys_vec3 buoyCom;
    float buoyComZ;
    float buoyComY;
    float buoyComX;
    float buoySumX;
    float buoySumY;
    float buoySumZ;
    float buoyMaxZ;
    float buoyMaxY;
    float buoyMaxX;
    float buoyDimZ;
    float buoyDimY;
    float buoyDimX;
    float bboxHalfZ;
    float bboxHalfY;
    float bboxHalfX;
    phys_vec3 buoyMaxVec;
    float buoyMinScalar;
    phys_vec3 bboxHalfVec;
    float bboxHalfY2;
    float bboxHalfZ2;
    float bboxHalfX3;
    float bboxHalfY3;
    float bboxHalfZ3;
    phys_mat44 transform;
    phys_vec3 *p_w;
    phys_vec3 com;
    float comZ;
    float comY;
    float comX;
    float comSumX;
    float comSumY;
    float comSumZ;
    float dimSumZ;
    float dimSumY;
    float dimSumX;
    phys_vec3 dim;
    float dimZ;
    float dimY;
    float dimX;
    phys_vec3 vmax;
    phys_vec3 vmin;
    phys_vec3 pos;
    phys_mat44 *p_m_mat;
    if ((phys_drawNitrousVehicle->current.enabled || (this->m_flags & 8) != 0) && this->m_phys_user_data)
    {
        p_m_mat = &this->m_phys_user_data->body->m_mat;
        pos = p_m_mat->w;
        Phys_DebugBox(&this->m_phys_user_data->body->m_mat, colorYellow, 1.0);
        if (this->m_parameter->m_bbox_min[0] != 0.0)
        {
            Phys_Vec3ToNitrousVec(this->m_parameter->m_bbox_min, &vmin);
            Phys_Vec3ToNitrousVec(this->m_parameter->m_bbox_max, &vmax);
            dimX = vmax.x - vmin.x;
            dimY = vmax.y - vmin.y;
            dimZ = vmax.z - vmin.z;
            dim.x = vmax.x - vmin.x;
            dim.y = vmax.y - vmin.y;
            dim.z = vmax.z - vmin.z;
            dimSumX = vmax.x + vmin.x;
            dimSumY = vmax.y + vmin.y;
            dimSumZ = vmax.z + vmin.z;
            comSumX = vmax.x + vmin.x;
            comSumY = vmax.y + vmin.y;
            comSumZ = vmax.z + vmin.z;
            comX = 0.5 * (float)(vmax.x + vmin.x);
            comY = 0.5 * (float)(vmax.y + vmin.y);
            comZ = 0.5 * (float)(vmax.z + vmin.z);
            com.x = comX;
            com.y = comY;
            com.z = comZ;
            p_w = &this->m_phys_user_data->cg2rb.w;
            com.x = comX + p_w->x;
            com.y = comY + p_w->y;
            com.z = comZ + p_w->z;
            //phys_mat44::operator=(&transform, &PHYS_IDENTITY_MATRIX_34);
            transform = PHYS_IDENTITY_MATRIX;
            //phys_vec3::operator=(&transform.w, &com);
            transform.w = com;
            phys_full_multiply_mat(&transform, &transform, &this->m_phys_user_data->body->m_mat);
            bboxHalfZ3 = 0.5 * dim.x;
            bboxHalfY3 = 0.5 * dim.y;
            bboxHalfX3 = 0.5 * dim.z;
            bboxHalfVec.w = 0.5 * dim.x;
            bboxHalfY2 = 0.5 * dim.y;
            bboxHalfZ2 = 0.5 * dim.z;
            Phys_DebugBox(&transform, (phys_vec3 *)&bboxHalfVec.w, colorGreen);
        }
        if (this->m_parameter->m_buoyancybox_min[0] != 0.0)
        {
            Phys_Vec3ToNitrousVec(this->m_parameter->m_buoyancybox_min, (phys_vec3 *)&buoyMinScalar);
            Phys_Vec3ToNitrousVec(this->m_parameter->m_buoyancybox_max, &buoyMaxVec);
            bboxHalfX = buoyMaxVec.x - buoyMinScalar;
            bboxHalfY = buoyMaxVec.y - bboxHalfVec.x;
            bboxHalfZ = buoyMaxVec.z - bboxHalfVec.y;
            buoyDimZ = buoyMaxVec.x - buoyMinScalar;
            buoyDimY = buoyMaxVec.y - bboxHalfVec.x;
            buoyDimX = buoyMaxVec.z - bboxHalfVec.y;
            buoyMaxX = buoyMaxVec.x + buoyMinScalar;
            buoyMaxY = buoyMaxVec.y + bboxHalfVec.x;
            buoyMaxZ = buoyMaxVec.z + bboxHalfVec.y;
            buoySumX = buoyMaxVec.x + buoyMinScalar;
            buoySumY = buoyMaxVec.y + bboxHalfVec.x;
            buoySumZ = buoyMaxVec.z + bboxHalfVec.y;
            buoyComX = 0.5 * (float)(buoyMaxVec.x + buoyMinScalar);
            buoyComY = 0.5 * (float)(buoyMaxVec.y + bboxHalfVec.x);
            buoyComZ = 0.5 * (float)(buoyMaxVec.z + bboxHalfVec.y);
            buoyCom.x = buoyComX;
            buoyCom.y = buoyComY;
            buoyCom.z = buoyComZ;
            //phys_mat44::operator=(&buoyTransform, &PHYS_IDENTITY_MATRIX_34);
            buoyTransform = PHYS_IDENTITY_MATRIX;
            //phys_vec3::operator=(&buoyTransform.w, &buoyCom);
            buoyTransform.w = buoyCom;
            phys_full_multiply_mat(&buoyTransform, &buoyTransform, &this->m_phys_user_data->body->m_mat);
            buoyHalfX = 0.5 * buoyDimZ;
            buoyHalfY = 0.5 * buoyDimY;
            buoyHalfZ = 0.5 * buoyDimX;
            buoyHalfExtents.x = 0.5 * buoyDimZ;
            buoyHalfExtents.y = 0.5 * buoyDimY;
            buoyHalfExtents.z = 0.5 * buoyDimX;
            Phys_DebugBox(&buoyTransform, &buoyHalfExtents, colorBlue);
        }
        alpha = 0.18f;
        for (i = 0; i < 6; ++i)
        {
            col[0] = 0.0f;
            col[1] = 1.0f;
            col[2] = 0.0f;
            col[3] = alpha;
            rbc_wheel = this->m_wheels[i];
            if (rbc_wheel)
            {
                b1 = rbc_wheel->b1;
                radius = rbc_wheel->m_wheel_radius;
                //v2 = operator*(&suspDisplacement, rbc_wheel->m_wheel_displaced_center_dist, &rbc_wheel->m_b1_suspension_dir_loc);
                suspDisplacement = rbc_wheel->m_b1_suspension_dir_loc * rbc_wheel->m_wheel_displaced_center_dist;
                //operator-(&wheel_pos, &rbc_wheel->m_b1_wheel_center_loc, v2);
                wheel_pos = rbc_wheel->m_b1_wheel_center_loc - suspDisplacement;
                wheelMat = &b1->m_mat;
                scale_4 = &b1->m_mat.w;
                wheelWorldPosResult = phys_multiply(&wheelLocalPos, &b1->m_mat, &wheel_pos);
                //operator+(&wheelWorldPos, wheelWorldPosResult, scale_4);
                wheelWorldPos = *wheelWorldPosResult + *scale_4;
                wheel_pos.x = wheelWorldPos.x;
                wheel_pos.y = wheelWorldPos.y;
                wheel_pos.z = wheelWorldPos.z;
                hit_pos = rbc_wheel->m_b2_hitp_loc;
                m_roll_stability_factor = rbc_wheel->m_roll_stability_factor;
                m_b1_suspension_dir_loc = rbc_wheel->m_b1_suspension_dir_loc;
                suspDirWorld = phys_multiply(&suspDirWorldVec, &b1->m_mat, &m_b1_suspension_dir_loc);
                stabOffsetX2 = m_roll_stability_factor * suspDirWorld->x;
                stabOffsetY2 = m_roll_stability_factor * suspDirWorld->y;
                stabOffsetZ2 = m_roll_stability_factor * suspDirWorld->z;
                stabOffsetX = stabOffsetX2;
                stabOffsetY = stabOffsetY2;
                stabOffsetZ = stabOffsetZ2;
                hitDisplacedX = hit_pos.x - stabOffsetX2;
                hitDisplacedY = hit_pos.y - stabOffsetY2;
                hitDisplacedZ = hit_pos.z - stabOffsetZ2;
                hit_pos_displaced.x = hit_pos.x - stabOffsetX2;
                hit_pos_displaced.y = hit_pos.y - stabOffsetY2;
                hit_pos_displaced.z = hit_pos.z - stabOffsetZ2;
                if ((rbc_wheel->m_wheel_flags & 1) != 0)
                {
                    col[0] = 1.0f;
                    col[1] = 1.0f;
                    col[2] = 0.0f;
                    col[3] = alpha;
                }
                if ((rbc_wheel->m_wheel_flags & 4) != 0)
                {
                    col[0] = 1.0f;
                    col[1] = 0.0f;
                    col[2] = 0.0f;
                    col[3] = alpha;
                }
                if ((rbc_wheel->m_wheel_flags & 2) != 0)
                {
                    m_hard_limit_dist = rbc_wheel->m_hard_limit_dist;
                    suspDirLocal = rbc_wheel->m_b1_suspension_dir_loc;
                    limitRadius = m_hard_limit_dist + radius;
                    hardLimitX = (float)(m_hard_limit_dist + radius) * suspDirLocal.x;
                    hardLimitY = (float)(m_hard_limit_dist + radius) * suspDirLocal.y;
                    hardLimitZ = (float)(m_hard_limit_dist + radius) * suspDirLocal.z;
                    limitOffsetZ = hardLimitX;
                    limitOffsetY = hardLimitY;
                    limitOffsetX = hardLimitZ;
                    m_b1_wheel_center_loc = rbc_wheel->m_b1_wheel_center_loc;
                    limitLocalX = m_b1_wheel_center_loc.x - hardLimitX;
                    limitLocalY = m_b1_wheel_center_loc.y - hardLimitY;
                    limitLocalZ = m_b1_wheel_center_loc.z - hardLimitZ;
                    limit_pos.x = m_b1_wheel_center_loc.x - hardLimitX;
                    limit_pos.y = m_b1_wheel_center_loc.y - hardLimitY;
                    limit_pos.z = m_b1_wheel_center_loc.z - hardLimitZ;
                    wheelBodyMat = &b1->m_mat;
                    scale_4a = &b1->m_mat.w;
                    limitWorldPosResult = phys_multiply(&limitLocalPos, &b1->m_mat, &limit_pos);
                    //operator+(&limitWorldPos, limitWorldPosResult, scale_4a);
                    limitWorldPos = *limitWorldPosResult + *scale_4a;
                    limit_pos.x = limitWorldPos.x;
                    limit_pos.y = limitWorldPos.y;
                    limit_pos.z = limitWorldPos.z;
                    if ((rbc_wheel->m_wheel_flags & 2) != 0)
                        Phys_DebugPoint(&limit_pos, sphere_size, colorRed);
                    else
                        Phys_DebugPoint(&limit_pos, sphere_size, colorGreen);
                }
                Phys_DebugPoint(&hit_pos, sphere_size, colorRed);
                Phys_DebugPoint(&hit_pos_displaced, sphere_size, colorBlue);
                Phys_DebugPoint(&wheel_pos, radius, col);
                if (this->m_vehicle_info->type == 2 || render_wheel_cir)
                {
                    m_b1_wheel_axis_loc = rbc_wheel->m_b1_wheel_axis_loc;
                    phys_multiply(&dir, &b1->m_mat, &m_b1_wheel_axis_loc);
                    Phys_NitrousVecToVec3(&dir, _dir);
                    Phys_NitrousVecToVec3(&wheel_pos, _pos);
                    G_DebugCircleEx(_pos, radius, _dir, colorWhite, 0, 0);
                }
                arm_col[0] = 0.0f;
                arm_col[1] = 1.0f;
                arm_col[2] = 1.0f;
                arm_col[3] = 1.0f;
                z = pos.z;
                pausedOffsetZ = hit_pos_displaced.z;
                if (hit_pos_displaced.z > (float)(pos.z - 5.0))
                {
                    arm_col[0] = 1.0f;
                    arm_col[1] = 0.0f;
                    arm_col[2] = 0.0f;
                    arm_col[3] = 1.0f;
                }
                render_line(&pos, &hit_pos_displaced, arm_col, 0, 0);
            }
        }
        pausedBodyMat = &this->m_phys_user_data->body->m_mat;
        fwdOffsetX2 = 130.0 * pausedBodyMat->x.x;
        fwdOffsetY2 = 130.0 * pausedBodyMat->x.y;
        fwdOffsetZ2 = 130.0 * pausedBodyMat->x.z;
        fwdOffsetX = fwdOffsetX2;
        fwdOffsetY = fwdOffsetY2;
        fwdOffsetZ = fwdOffsetZ2;
        pos.x = pos.x + fwdOffsetX2;
        pos.y = pos.y + fwdOffsetY2;
        pos.z = pos.z + fwdOffsetZ2;
        if ((this->m_flags & 0x40) != 0)
        {
            body = this->m_phys_user_data->body;
            bodyMat = &body->m_mat;
            pausedVelX = 100.0 * body->m_mat.x.x;
            pausedVelY = 100.0 * body->m_mat.x.y;
            pausedVelZ = 100.0 * body->m_mat.x.z;
            textPos[3] = pausedVelX;
            textPos[4] = pausedVelY;
            textPos[5] = pausedVelZ;
            pos.x = pos.x + pausedVelX;
            pos.y = pos.y + pausedVelY;
            pos.z = pos.z + pausedVelZ;
        }
        Phys_NitrousVecToVec3(&pos, textPos);
        textPos[2] = textPos[2] + 50.0;
        sprintf(buffer, "Speed %.1f %.0f", (float)(this->m_forward_vel / 17.6), this->m_forward_vel);
        CL_AddDebugString(textPos, colorWhite, 1.0, buffer, 1);
        textPos[2] = textPos[2] - 20.0;
        sprintf(buffer, "Rpm %.2f", this->m_fake_rpm);
        CL_AddDebugString(textPos, colorWhite, 1.0, buffer, 1);
        if (snd_boat_using_lerp_rpm && snd_boat_using_lerp_rpm->current.enabled)
        {
            textPos[2] = textPos[2] - 20.0;
            sprintf(buffer, "boat rpm %.2f", snd_boat_current_rpm->current.value);
            CL_AddDebugString(textPos, colorWhite, 1.0, buffer, 1);
        }
    }
    NitrousVehicle::debug_render_effects();
}

// Decomp:  CoDMPServer.c:658351  (RVA 00826CA0)
// Decomp:  retail multiplies lhs·rhs into dest including translation column (phys_full_multiply on w); matches binary
void    phys_full_multiply_mat(phys_mat44 *dest, const phys_mat44 *left, const phys_mat44 *right)
{
    phys_mat44 leftCopy;
    phys_vec3 *temp_60;
    const phys_vec3 *xAxisSrc;
    phys_vec3 xAxisResult;
    phys_vec3 *p_z;
    const phys_vec3 *yAxisSrc;
    phys_vec3 yAxisResult;
    phys_vec3 *p_y;
    const phys_vec3 *zAxisSrc;
    phys_vec3 zAxisResult;
    const phys_vec3 *wAxisSrc;
    phys_vec3 wAxisResult;
    //_UNKNOWN *v16[2];
    //const phys_mat44 *righta;
    //
    //v16[0] = a1;
    //v16[1] = righta;
    assert_mat44_initialized(*left);
    assert_mat44_initialized(*right);
    if (dest == left)
    {
        memcpy(&leftCopy, left, sizeof(leftCopy));
        phys_full_multiply_mat(dest, &leftCopy, right);
    }
    else
    {
        wAxisSrc = phys_multiply(&wAxisResult, left, &right->x);
        dest->x.x = wAxisSrc->x;
        dest->x.y = wAxisSrc->y;
        dest->x.z = wAxisSrc->z;
        zAxisSrc = phys_multiply(&zAxisResult, left, &right->y);
        p_y = &dest->y;
        dest->y.x = zAxisSrc->x;
        p_y->y = zAxisSrc->y;
        p_y->z = zAxisSrc->z;
        yAxisSrc = phys_multiply(&yAxisResult, left, &right->z);
        p_z = &dest->z;
        dest->z.x = yAxisSrc->x;
        p_z->y = yAxisSrc->y;
        p_z->z = yAxisSrc->z;
        xAxisSrc = phys_full_multiply(&xAxisResult, left, &right->w);
        temp_60 = &dest->w;
        dest->w.x = xAxisSrc->x;
        temp_60->y = xAxisSrc->y;
        temp_60->z = xAxisSrc->z;
    }
}

// Decomp:  CoDMPServer.c:658402  (RVA 00826E10)
void NitrousVehicle::remove_wheels()
{
    int i;
    for ( i = 0; i < 6; ++i )
    {
        if ( this->m_wheels[i] )
            this->m_wheels[i] = 0;
    }
}

// Decomp:  CoDMPServer.c:658414  (RVA 00826E60)
void NitrousVehicle::set(
                float power_braking_factor,
                float braking_factor,
                float desired_speed_factor,
                float acceleration_factor,
                float coasting_factor,
                float reference_wheel_radius,
                float steer_max_angle,
                float steer_speed,
                phys_vec3 *steer_front_pt_loc,
                float steer_front_back_length)
{
    this->m_power_braking_factor = power_braking_factor;
    this->m_braking_factor = braking_factor;
    this->m_desired_speed_factor = desired_speed_factor;
    this->m_acceleration_factor = acceleration_factor;
    this->m_coasting_factor = coasting_factor;
    this->m_reference_wheel_radius = reference_wheel_radius;
    this->m_steer_max_angle = steer_max_angle;
    this->m_steer_speed = steer_speed;
    this->m_steer_front_pt_loc.x = steer_front_pt_loc->x;
    this->m_steer_front_pt_loc.y = steer_front_pt_loc->y;
    this->m_steer_front_pt_loc.z = steer_front_pt_loc->z;
    this->m_steer_front_back_length = steer_front_back_length;
    this->m_steer_factor = 0.0f;
    this->m_steer_current_angle = 0.0f;
    this->m_state_flags = 0;
    this->m_forward_vel = 0.0f;
}

// Decomp:  CoDMPServer.c:658438  (RVA 00826F80)
// Decomp:  IDA local variable allocation failed at this site in CoDMPServer.c; stack layout recast as explicit locals (same behavior)
void NitrousVehicle::update_steering(float delta_t)
{
    float tanSteerAngle; // xmm0_4
    float tanSteerAnglePartial; // xmm0_4
    int m;
    float accelRatioNorm;
    float wheelSpeedRatio;
    phys_vec3 *p_m_b1_wheel_axis_loc;
    float steerSign;
    float nwheel_axis;
    phys_vec3 wheel_axis;
    float toCenterZ;
    float toCenterY;
    float toCenterX;
    float x;
    float y;
    float z;
    float w;
    rigid_body_constraint_wheel *wheel;
    int k;
    float wheel_ratios[6];
    float total_ratio;
    int num_accel_wheels;
    phys_vec3 turning_center;
    float centerZ;
    float centerY;
    float centerX;
    phys_vec3 *p_m_steer_front_pt_loc;
    float steerOffsetX;
    float steerOffsetY;
    int steerOffsetZ;
    float rearOffsetX;
    float tanRadius;
    float steerAngleTan;
    float turning_center_radius;
    phys_vec3 turning_center_radius_vec;
    float rearAxleOffset;
    float turnRadius;
    int turnCenterZ;
    float turnRadiusHalf;
    float width;
    float turnRadiusFull;
    float currentSteerAngle;
    float m_hand_brake;
    int j;
    bool all_wheel_steering;
    rigid_body_constraint_wheel *wheelForAxis;
    rigid_body_constraint_wheel *wheelForRatio;
    phys_vec3 *axisPtr;
    rigid_body_constraint_wheel *wheelForReset;
    int i;
    float m_steer_current_angle;
    float angle_delta;
    float max_delta_angle;
    float new_steer_angle;
    float steer;
    steer = this->m_steer_factor;
    if (steer <= 1.0)
    {
        if (steer < -1.0)
            steer = -1.0f;
    }
    else
    {
        steer = 1.0f;
    }
    new_steer_angle = this->m_steer_max_angle * steer;
    max_delta_angle = this->m_steer_speed * delta_t;
    angle_delta = new_steer_angle - this->m_steer_current_angle;
    if (angle_delta <= max_delta_angle)
    {
        if ((-(max_delta_angle)) > angle_delta)
            (angle_delta) = -(max_delta_angle);
    }
    else
    {
        angle_delta = max_delta_angle;
    }
    this->m_steer_current_angle = this->m_steer_current_angle + angle_delta;
    m_steer_current_angle = this->m_steer_current_angle;
    if ((fabs(m_steer_current_angle)) > 0.0049999999)
    {
        all_wheel_steering = 1;
        for (j = 0; j < 6; ++j)
        {
            if (this->m_wheels[j])
            {
                if ((this->m_wheels[j]->m_wheel_flags & 8) == 0
                    || (m_hand_brake = this->m_hand_brake, m_hand_brake > 0.1) && (this->m_wheels[j]->m_wheel_flags & 0x80) != 0)
                {
                    all_wheel_steering = 0;
                    break;
                }
            }
        }
        if (all_wheel_steering)
        {
            currentSteerAngle = this->m_steer_current_angle;
            //tanSteerAngle = __libm_sse2_tan(currentSteerAngle);
            tanSteerAngle = tan(currentSteerAngle);
            turnRadiusFull = tanSteerAngle;
            width = (float)(this->m_steer_front_back_length * 0.5) / tanSteerAngle;
            turnRadiusHalf = (-(this->m_steer_front_back_length)) * 0.5;
            rearAxleOffset = turnRadiusHalf;
            turnRadius = width;
            turnCenterZ = 0.0f;
            turning_center_radius_vec.x = turnRadiusHalf;
        }
        else
        {
            steerAngleTan = this->m_steer_current_angle;
            //tanSteerAnglePartial = __libm_sse2_tan(steerAngleTan);
            tanSteerAnglePartial = tan(steerAngleTan);
            tanRadius = tanSteerAnglePartial;
            width = this->m_steer_front_back_length / tanSteerAnglePartial;
            (rearOffsetX) = -(this->m_steer_front_back_length);
            steerOffsetX = rearOffsetX;
            steerOffsetY = width;
            steerOffsetZ = 0.0f;
            turning_center_radius_vec.x = rearOffsetX;
        }
        turning_center_radius_vec.y = width;
        turning_center_radius_vec.z = 0.0f;
        turning_center_radius = Abs(&turning_center_radius_vec.x);
        p_m_steer_front_pt_loc = &this->m_steer_front_pt_loc;
        centerX = this->m_steer_front_pt_loc.x + turning_center_radius_vec.x;
        centerY = this->m_steer_front_pt_loc.y + turning_center_radius_vec.y;
        centerZ = this->m_steer_front_pt_loc.z + turning_center_radius_vec.z;
        turning_center.x = centerX;
        turning_center.y = centerY;
        turning_center.z = centerZ;
        num_accel_wheels = 0;
        total_ratio = 0.0f;
        memset(wheel_ratios, 0, sizeof(wheel_ratios));
        for (k = 0; k < 6; ++k)
        {
            if (this->m_wheels[k])
            {
                wheel = this->m_wheels[k];
                x = wheel->m_b1_wheel_center_loc.x;
                y = wheel->m_b1_wheel_center_loc.y;
                z = wheel->m_b1_wheel_center_loc.z;
                w = wheel->m_b1_wheel_center_loc.w;
                toCenterX = turning_center.x - x;
                toCenterY = turning_center.y - y;
                toCenterZ = turning_center.z - z;
                wheel_axis.x = turning_center.x - x;
                wheel_axis.y = turning_center.y - y;
                wheel_axis.z = 0.0f;
                nwheel_axis = Abs(&wheel_axis.x);
                if ((this->m_wheels[k]->m_wheel_flags & 8) != 0)
                {
                    if (this->m_steer_current_angle < 0.0)
                        steerSign = -1.0f;
                    else
                        steerSign = 1.0f;
                    if (nwheel_axis <= 0.001)
                    {
                        wheel_axis.x = gWheelAxisLoc.x;
                        wheel_axis.y = gWheelAxisLoc.y;
                        wheel_axis.z = gWheelAxisLoc.z;
                    }
                    else
                    {
                        wheel_axis.x = wheel_axis.x * (float)(steerSign / nwheel_axis);
                        wheel_axis.y = wheel_axis.y * (float)(steerSign / nwheel_axis);
                        wheel_axis.z = wheel_axis.z * (float)(steerSign / nwheel_axis);
                    }
                    if (this->m_hand_brake > 0.1 && (this->m_wheels[k]->m_wheel_flags & 0x80) != 0)
                    {
                        wheel_axis.x = gWheelAxisLoc.x;
                        wheel_axis.y = gWheelAxisLoc.y;
                        wheel_axis.z = gWheelAxisLoc.z;
                    }
                    p_m_b1_wheel_axis_loc = &this->m_wheels[k]->m_b1_wheel_axis_loc;
                    p_m_b1_wheel_axis_loc->x = wheel_axis.x;
                    p_m_b1_wheel_axis_loc->y = wheel_axis.y;
                    p_m_b1_wheel_axis_loc->z = wheel_axis.z;
                }
                wheelSpeedRatio = nwheel_axis / turning_center_radius;
                if ((this->m_wheels[k]->m_wheel_flags & 0x10) != 0)
                {
                    ++num_accel_wheels;
                    wheel_ratios[k] = wheelSpeedRatio;
                    total_ratio = total_ratio + wheelSpeedRatio;
                }
            }
        }
        accelRatioNorm = (float)num_accel_wheels / total_ratio;
        for (m = 0; m < 6; ++m)
        {
            if (this->m_wheels[m])
            {
                this->m_wheels[m]->m_turning_radius_ratio_max_speed = wheel_ratios[m];
                this->m_wheels[m]->m_turning_radius_ratio_accel = wheel_ratios[m] * accelRatioNorm;
            }
        }
    }
    else
    {
        for (i = 0; i < 6; ++i)
        {
            if (this->m_wheels[i])
            {
                wheelForReset = this->m_wheels[i];
                axisPtr = &wheelForReset->m_b1_wheel_axis_loc;
                wheelForReset->m_b1_wheel_axis_loc.x = gWheelAxisLoc.x;
                axisPtr->y = gWheelAxisLoc.y;
                axisPtr->z = gWheelAxisLoc.z;
                wheelForRatio = this->m_wheels[i];
                wheelForRatio->m_turning_radius_ratio_max_speed = 1.0f;
                wheelForAxis = this->m_wheels[i];
                wheelForAxis->m_turning_radius_ratio_accel = 1.0f;
            }
        }
    }
}

// Decomp:  CoDMPServer.c:658679  (RVA 008278D0)
void NitrousVehicle::update_braking_and_acceleration(float delta_t)
{
    rigid_body_constraint_wheel *rbc_wheel;
    int i;
    for ( i = 0; i < 6; ++i )
    {
        rbc_wheel = this->m_wheels[i];
        if ( rbc_wheel )
        {
            if ( (this->m_state_flags & 1) != 0 && (rbc_wheel->m_wheel_flags & 0x20) != 0 )
            {
                rbc_wheel->set_wheel_state_braking(this->m_power_braking_factor);
                //rigid_body_constraint_wheel::set_wheel_state_braking(rbc_wheel, this->m_power_braking_factor);
            }
            else if ( (this->m_state_flags & 2) != 0 && (rbc_wheel->m_wheel_flags & 0x40) != 0 )
            {
                rbc_wheel->set_wheel_state_braking(this->m_braking_factor);
                //rigid_body_constraint_wheel::set_wheel_state_braking(rbc_wheel, this->m_braking_factor);
            }
            else if ( (this->m_state_flags & 4) != 0 && (rbc_wheel->m_wheel_flags & 0x10) != 0 )
            {
                rbc_wheel->set_wheel_state_accelerating(this->m_desired_speed_factor / this->m_reference_wheel_radius, this->m_acceleration_factor);
                //rigid_body_constraint_wheel::set_wheel_state_accelerating(
                //    rbc_wheel,
                //    this->m_desired_speed_factor / this->m_reference_wheel_radius,
                //    this->m_acceleration_factor);
            }
            else if ( (this->m_state_flags & 8) != 0 && (rbc_wheel->m_wheel_flags & 0x10) != 0 )
            {
                rbc_wheel->set_wheel_state_accelerating(-this->m_desired_speed_factor / this->m_reference_wheel_radius, this->m_acceleration_factor);
                //rigid_body_constraint_wheel::set_wheel_state_accelerating(
                //    rbc_wheel,
                //    COERCE_FLOAT(LODWORD(this->m_desired_speed_factor) ^ _mask__NegFloat_) / this->m_reference_wheel_radius,
                //    this->m_acceleration_factor);
            }
            else if ( (this->m_state_flags & 0x10) != 0 )
            {
                rbc_wheel->set_wheel_state_braking(this->m_coasting_factor);
                //rigid_body_constraint_wheel::set_wheel_state_braking(rbc_wheel, this->m_coasting_factor);
            }
            else
            {
                rbc_wheel->set_wheel_state_braking(0.0f);
                //rigid_body_constraint_wheel::set_wheel_state_braking(rbc_wheel, 0.0);
            }
        }
    }
}

// Decomp:  CoDMPServer.c:658727  (RVA 00827A60)
phys_vec3 *__cdecl phys_cross(phys_vec3 *result, const phys_vec3 *a, const phys_vec3 *b)
{
    float crossX;
    float crossY;
    crossX = (float)(a->z * b->x) - (float)(a->x * b->z);
    crossY = (float)(a->x * b->y) - (float)(a->y * b->x);
    result->x = (float)(a->y * b->z) - (float)(a->z * b->y);
    result->y = crossX;
    result->z = crossY;
    return result;
}

// Decomp:  CoDMPServer.c:658741  (RVA 00827B10)
void NitrousVehicle::set_boosting(bool boosting)
{
    if ( boosting != ((this->m_flags & 0x800) != 0) )
    {
        if ( boosting )
            this->m_flags |= 0x800u;
        else
            this->m_flags &= ~0x800u;
        if ( boosting )
            this->m_boost_start_time = this->mVehicleController.m_cmd.serverTime;
    }
}

// Decomp:  CoDMPServer.c:658764  (RVA 00827BC0)
bool NitrousVehicle::is_boosting()
{
    return (this->m_flags & 0x800) != 0;
}

// Decomp:  CoDMPServer.c:658770  (RVA 00827BE0)
double NitrousVehicle::get_maximum_acceleration_factor()
{
    return 2.0;
}

// Decomp:  CoDMPServer.c:658776  (RVA 00827C00)
double NitrousVehicle::get_maximum_acceleration()
{
    float maximum_acceleration;
    maximum_acceleration = this->m_parameter->m_accel_max;
    if ( NitrousVehicle::is_boosting() )
        return (float)(maximum_acceleration * this->m_vehicle_info->boostAccelMultiplier);
    return maximum_acceleration;
}

// Decomp:  CoDMPServer.c:658789  (RVA 00827C50)
double NitrousVehicle::get_max_speed(bool include_perks)
{
    float max_speed;
    max_speed = this->m_parameter->m_speed_max;
    if ( NitrousVehicle::is_boosting() )
        return (float)(max_speed * this->m_vehicle_info->boostSpeedIncrease);
    return max_speed;
}

// =============================================================================
// NitrousVehicle::update_script_target
// Decomp:  CoDMPServer.c:434860  (RVA 00677C50)
// Retail:  N/A
// Notes:   Canonical impl in nitrous_vehicle.cpp; was duplicated in g_scr_vehicle.cpp
// =============================================================================
void NitrousVehicle::update_script_target(float *goal_position)
{
    phys_vec3 goalNitrous;
    // Decomp: CoDMPServer.c:434875 — Phys_Vec3ToNitrousVec(goal_position, ...)
    Phys_Vec3ToNitrousVec(goal_position, &goalNitrous);
    // Decomp: CoDMPServer.c:434876-434879 — copy into mVehicleController.m_script_goal_position
    this->mVehicleController.m_script_goal_position.x = goalNitrous.x;
    this->mVehicleController.m_script_goal_position.y = goalNitrous.y;
    this->mVehicleController.m_script_goal_position.z = goalNitrous.z;
}

// Decomp:  CoDMPServer.c:658802  (RVA 00827CA0)
void NitrousVehicle::update_control_from_network(LerpEntityStateVehicle *vehicleState)
{
    float clampedSteerYaw;
    float steerYaw;
    if ( this->m_phys_user_data )
    {
        NitrousVehicle::set_throttle((float)vehicleState->throttle * 0.000030517578);
        if ( (float)(vehicleState->steerYaw - 1.0) < 0.0 )
            steerYaw = vehicleState->steerYaw;
        else
            steerYaw = 1.0f;
        if ( (float)(-1.0 - vehicleState->steerYaw) < 0.0 )
            clampedSteerYaw = steerYaw;
        else
            clampedSteerYaw = -1.0f;
        this->m_steer_factor = clampedSteerYaw;
        this->mVehicleController.m_cmd_valid = 0;
    }
}

// Decomp:  CoDMPServer.c:658828  (RVA 00827D70)
// Decomp:  CoDMPServer.c:658917-658918 — IDA emitted empty phys_axis advance loop; omitted (Phys_Vec3ToNitrousVec fills rows directly)
void NitrousVehicle::update_from_network(
                trajectory_t pos,
                trajectory_t apos,
                LerpEntityStateVehicle *vehicleState,
                float timeDifference)
{
    double positionErrorSq;
    float currentAnglesError[3];
    float predictedAngles[3];
    float predictedPhysAxis[3][3];
    float localPhysicsAngles[3];
    phys_mat44 rv;
    float currentOriginError[3];
    float predictedPos[3];
    float z;
    float y;
    float x;
    rigid_body *body;
    bool withinPositionTolerance;
    //int v29;
    phys_vec3 unusedDiffVec;
    float acceptablePosError;
    phys_vec3 networkPredictedRBPosition;
    float predictedPosX;
    float predictedPosY;
    float predictedPosZ;
    float predictedZ;
    float predictedY;
    float predictedX;
    float velDeltaX;
    float velDeltaY;
    float velDeltaZ;
    float velDeltaZ2;
    float velDeltaY2;
    float velDeltaX2;
    int absTimeDiff;
    phys_vec3 networkVel;
    float localPhysAxis[3][3];
    phys_vec3 phys_axis[3];
    phys_vec3 position;
    float networkAngles[3];
    float networkRBPos[3];
    float translation[3];
    float networkAxisTranspose[3][3];
    float localTrans[3];
    float networkAxis[3][3];
    float localPos[3];
    phys_vec3 *aVelocity;
    phys_vec3 *velocity;
    phys_mat44 *mat;
    if (this->m_phys_user_data)
    {
        NitrousVehicle::update_control_from_network(vehicleState);
        mat = &this->m_phys_user_data->body->m_mat;
        velocity = &this->m_phys_user_data->body->m_t_vel;
        aVelocity = &this->m_phys_user_data->body->m_a_vel;
        Phys_NitrousVecToVec3(&mat->w, localPos);
        AnglesToAxis(apos.trBase, networkAxis);
        Phys_NitrousVecToVec3(&this->m_phys_user_data->cg2rb.w, localTrans);
        MatrixTranspose(networkAxis, networkAxisTranspose);
        MatrixVecMultiply(networkAxisTranspose, localTrans, translation);
        networkRBPos[0] = pos.trBase[0] - translation[0];
        networkRBPos[1] = pos.trBase[1] - translation[1];
        networkRBPos[2] = pos.trBase[2] - translation[2];
        networkAngles[0] = apos.trBase[0];
        networkAngles[1] = apos.trBase[1];
        networkAngles[2] = apos.trBase[2];
        Phys_Vec3ToNitrousVec(networkRBPos, &position);
        Phys_Vec3ToNitrousVec(networkAxis[0], phys_axis);
        Phys_Vec3ToNitrousVec(networkAxis[1], &phys_axis[1]);
        Phys_Vec3ToNitrousVec(networkAxis[2], &phys_axis[2]);
        Phys_NitrousMat44ToVec33(mat, localPhysAxis);
        Phys_Vec3ToNitrousVec(pos.trDelta, &networkVel);
        absTimeDiff = fabs(timeDifference);
        if (timeCap > (fabs(timeDifference)))
        {
            velDeltaX2 = timeDifference * networkVel.x;
            velDeltaY2 = timeDifference * networkVel.y;
            velDeltaZ2 = timeDifference * networkVel.z;
            velDeltaX = timeDifference * networkVel.x;
            velDeltaY = timeDifference * networkVel.y;
            velDeltaZ = timeDifference * networkVel.z;
            predictedX = position.x + (float)(timeDifference * networkVel.x);
            predictedY = position.y + (float)(timeDifference * networkVel.y);
            predictedZ = position.z + (float)(timeDifference * networkVel.z);
            predictedPosX = predictedX;
            predictedPosY = predictedY;
            predictedPosZ = predictedZ;
            networkPredictedRBPosition.x = predictedX;
            networkPredictedRBPosition.y = predictedY;
            networkPredictedRBPosition.z = predictedZ;
            if (!CompareVec3ToPhysVec(pos.trDelta, velocity, acceptableVelError))
                Phys_Vec3ToNitrousVec(pos.trDelta, velocity);
            if (!CompareVec3ToPhysVec(apos.trDelta, aVelocity, acceptableAVelError))
                Phys_Vec3ToNitrousVec(apos.trDelta, aVelocity);
            acceptablePosError = acceptablePosErrorBase;
            phys_vec3 tmp;
            //v6 = operator-((phys_vec3 *)&unusedDiffVec.w, &networkPredictedRBPosition, &mat->w);
            tmp = networkPredictedRBPosition - mat->w;
            //phys_vec3::operator=((phys_vec3 *)&v29, v6);
            //positionErrorSq = AbsSquared((const phys_vec3 *)&v29);
            positionErrorSq = AbsSquared(&tmp);
            withinPositionTolerance = acceptablePosError * acceptablePosError > positionErrorSq;
            if (acceptablePosError * acceptablePosError <= positionErrorSq)
            {
                Phys_Vec3ToNitrousVec(pos.trDelta, velocity);
                Phys_Vec3ToNitrousVec(apos.trDelta, aVelocity);
                //phys_vec3::operator=(&mat->x, phys_axis);
                mat->x = phys_axis[0];
                mat->y = phys_axis[1];
                mat->z = phys_axis[2];
                mat->w = networkPredictedRBPosition;
                //phys_vec3::operator=(&mat->y, &phys_axis[1]);
                //phys_vec3::operator=(&mat->z, &phys_axis[2]);
                //phys_vec3::operator=(&mat->w, &networkPredictedRBPosition);
                body = this->m_phys_user_data->body;
                *(_QWORD *)&body->m_last_position.x = *(_QWORD *)&position.x;
                body->m_last_position.z = position.z;
                if (body->m_last_position.x != body->m_last_position.x)
                    goto LABEL_19;
                x = body->m_last_position.x;
                if ((fabs(x)) > 100000.0)
                    goto LABEL_19;
                if (body->m_last_position.y != body->m_last_position.y
                    || (y = body->m_last_position.y,
                        (fabs(y)) > 100000.0)
                    || body->m_last_position.z != body->m_last_position.z
                    || (z = body->m_last_position.z,
                        (fabs(z)) > 100000.0))
                {
                LABEL_19:
                    phys_exec_debug_callback(this->m_phys_user_data->body);
                }
                Phys_NitrousVecToVec3(&networkPredictedRBPosition, predictedPos);
                Vec3Sub(localPos, predictedPos, currentOriginError);
                Vec3Add(currentOriginError, this->m_networkErrorOrigin, this->m_networkErrorOrigin);
                //XAnimClientNotifyList::GetNotifyList((XAnimClientNotifyList *)&rv);
                make_rotate(&rv, aVelocity, timeDifference, 1000.0);
                phys_multiply_mat(mat, &rv, mat);
                AxisToAngles(localPhysAxis, localPhysicsAngles);
                Phys_NitrousMat44ToVec33(mat, predictedPhysAxis);
                AxisToAngles(predictedPhysAxis, predictedAngles);
                AnglesSubtract(localPhysicsAngles, predictedAngles, currentAnglesError);
                Vec3Add(currentAnglesError, this->m_networkErrorAngles, this->m_networkErrorAngles);
                if (phys_drawNitrousVehicle->current.enabled)
                {
                    G_DebugStar(networkRBPos, colorGreen, duration);
                    G_DebugStar(predictedPos, colorYellow, duration);
                    G_DebugStar(localPos, colorRed, duration);
                    CG_DebugLine(networkRBPos, predictedPos, colorWhite, 1, duration);
                    CG_DebugLine(predictedPos, localPos, colorWhite, 1, duration);
                }
            }
        }
    }
}

// Decomp:  CoDMPServer.c:659027  (RVA 00828770)
double __cdecl AbsSquared(const phys_vec3 *a)
{
    return a->x * a->x + a->y * a->y + a->z * a->z;
}

// Decomp:  CoDMPServer.c:659033  (RVA 008287A0)
// Decomp:  multiplies orientation columns only — translation row uses phys_full_multiply_mat path in sibling symbol
void    phys_multiply_mat(phys_mat44 *dest, const phys_mat44 *left, const phys_mat44 *right)
{
    phys_mat44 leftCopy;
    phys_vec3 *temp_60;
    const phys_vec3 *xAxisSrc;
    phys_vec3 xAxisResult;
    phys_vec3 *p_y;
    const phys_vec3 *yAxisSrc;
    phys_vec3 yAxisResult;
    const phys_vec3 *zAxisSrc;
    phys_vec3 zAxisResult;
    //_UNKNOWN *v13[2];
    //const phys_mat44 *righta;
    //
    //v13[0] = a1;
    //v13[1] = righta;
    assert_mat44_initialized(*left);
    //assert_mat44_initialized(*right);
    if (dest == left)
    {
        memcpy(&leftCopy, left, sizeof(leftCopy));
        phys_multiply_mat(dest, &leftCopy, right);
    }
    else
    {
        zAxisSrc = phys_multiply(&zAxisResult, left, &right->x);
        dest->x.x = zAxisSrc->x;
        dest->x.y = zAxisSrc->y;
        dest->x.z = zAxisSrc->z;
        yAxisSrc = phys_multiply(&yAxisResult, left, &right->y);
        p_y = &dest->y;
        dest->y.x = yAxisSrc->x;
        p_y->y = yAxisSrc->y;
        p_y->z = yAxisSrc->z;
        xAxisSrc = phys_multiply(&xAxisResult, left, &right->z);
        temp_60 = &dest->z;
        dest->z.x = xAxisSrc->x;
        temp_60->y = xAxisSrc->y;
        temp_60->z = xAxisSrc->z;
    }
}

// defined in phys_render
//void    make_rotate(
//                phys_mat44 *mat,
//                const phys_vec3 *v,
//                float theta_factor,
//                float max_rotation_radians)
//{
//    float v5; // xmm0_4
//    float ca; // xmm0_4
//    float x;
//    phys_vec3 v8;
//    float v9;
//    float v10;
//    float v11;
//    float v12;
//    float v13;
//    //int v14;
//    //void *v15;
//    //void *retaddr;
//    //
//    //v14 = a1;
//    //v15 = retaddr;
//    v13 = Abs(&v->x);
//    if (v13 >= 0.0000099999997)
//    {
//        v12 = 1.0 / v13;
//        v11 = (float)(1.0 / v13) * v->x;
//        v10 = (float)(1.0 / v13) * v->y;
//        v9 = (float)(1.0 / v13) * v->z;
//        v8.x = v11;
//        v8.y = v10;
//        v8.z = v9;
//        v13 = v13 * theta_factor;
//        if (v13 > max_rotation_radians)
//            v13 = max_rotation_radians;
//        //v5 = __libm_sse2_sin(v13);
//        v5 = sin(v13);
//        x = v5;
//        //ca = __libm_sse2_cos(v13);
//        ca = cos(v13);
//        make_rotate(mat, &v8, ca, x);
//    }
//    else
//    {
//        //phys_mat44::operator=(mat, &PHYS_IDENTITY_MATRIX_34);
//        *mat = PHYS_IDENTITY_MATRIX;
//    }
//}
// defined in phys_render
//void __cdecl make_rotate(phys_mat44 *m, const phys_vec3 *u, float ca, float sa)
//{
//    float yy;
//    float xsa;
//    float xy;
//    float zsa;
//    float zz;
//    float ysa;
//    float xz;
//    float yz;
//
//    xy = (float)(u->x * u->y) * (float)(1.0 - ca);
//    xz = (float)(u->x * u->z) * (float)(1.0 - ca);
//    yy = (float)(u->y * u->y) * (float)(1.0 - ca);
//    yz = (float)(u->y * u->z) * (float)(1.0 - ca);
//    zz = (float)(u->z * u->z) * (float)(1.0 - ca);
//    xsa = u->x * sa;
//    ysa = u->y * sa;
//    zsa = u->z * sa;
//    m->x.x = (float)((float)(u->x * u->x) * (float)(1.0 - ca)) + ca;
//    m->y.x = xy - zsa;
//    m->z.x = xz + ysa;
//    m->w.x = 0.0f;
//    //*phys_vec3::operator[]<int>(&m->x, 1u) = xy + zsa;
//    m->x[1] = xy + zsa;
//    //*phys_vec3::operator[]<int>(&m->y, 1u) = yy + ca;
//    m->y[1] = yy + ca;
//    //*phys_vec3::operator[]<int>(&m->z, 1u) = yz - xsa;
//    m->z[1] = yz - xsa;
//    //*phys_vec3::operator[]<int>(&m->w, 1u) = 0.0f;
//    m->w[1] = 0.0f;
//    //*phys_vec3::operator[]<int>(&m->x, 2u) = xz - ysa;
//    m->x[2] = xz - ysa;
//    //*phys_vec3::operator[]<int>(&m->y, 2u) = yz + xsa;
//    m->y[2] = yz + xsa;
//    //*phys_vec3::operator[]<int>(&m->z, 2u) = zz + ca;
//    m->z[2] = zz + ca;
//    //*phys_vec3::operator[]<int>(&m->w, 2u) = 0.0f;
//    m->w[2] = 0.0f;
//    //phys_mat44::fix_w_column(m);
//    m->fix_w_column();
//}
// Decomp:  CoDMPServer.c:659166  (RVA 00829110)
void __cdecl Phys_NitrousMat44ToVec33(const phys_mat44 *inMat, float (*outAxis)[3])
{
    assert_mat44_initialized(*inMat);
    (*outAxis)[0] = inMat->x.x;
    (*outAxis)[1] = inMat->x.y;
    (*outAxis)[2] = inMat->x.z;
    (*outAxis)[3] = inMat->y.x;
    (*outAxis)[4] = inMat->y.y;
    (*outAxis)[5] = inMat->y.z;
    (*outAxis)[6] = inMat->z.x;
    (*outAxis)[7] = inMat->z.y;
    (*outAxis)[8] = inMat->z.z;
}

// Decomp:  CoDMPServer.c:659180  (RVA 00829210)
bool __cdecl CompareVec3ToPhysVec(const float *a, const phys_vec3 *b, float acceptableError)
{
    float diff_4;
    float diff_8;
    diff_4 = b->y - a[1];
    diff_8 = b->z - a[2];
    return (float)(acceptableError * acceptableError) > (float)((float)((float)((float)(b->x - *a) * (float)(b->x - *a))
                                                                                                                                        + (float)(diff_4 * diff_4))
                                                                                                                        + (float)(diff_8 * diff_8));
}

// Decomp:  CoDMPServer.c:659193  (RVA 008292B0)
void NitrousVehicle::GetEntityPose(
                float *origin,
                float *angles,
                int curTime,
                bool isDriver)
{
    float angleReductionScale;
    float originReductionScale;
    float angleLerpScale;
    float originLerpScale;
    float dt;
    float reduction[3];
    float scale;
    float rbPosition[3];
    float rbAngles[3];
    float rbRotation[4];
    Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
    if ( this->m_phys_user_data )
    {
        if ( isDriver )
        {
            Phys_ObjGetInterpolatedState((int)this->m_phys_user_data, rbPosition, rbRotation);
            UnitQuatToAngles(rbRotation, rbAngles);
            *origin = rbPosition[0];
            origin[1] = rbPosition[1];
            origin[2] = rbPosition[2];
            *angles = rbAngles[0];
            angles[1] = rbAngles[1];
            angles[2] = rbAngles[2];
        }
        else
        {
            if ( curTime > this->m_lastErrorReductionTime )
            {
                dt = (float)(curTime - this->m_lastErrorReductionTime) * 0.001;
                if ( (float)((float)(errorReductionRate * dt) - 1.0) < 0.0 )
                    originLerpScale = errorReductionRate * dt;
                else
                    originLerpScale = 1.0f;
                if ( (float)(0.0 - (float)(errorReductionRate * dt)) < 0.0 )
                    originReductionScale = originLerpScale;
                else
                    originReductionScale = 0.0f;
                scale = originReductionScale;
                reduction[0] = originReductionScale * this->m_networkErrorOrigin[0];
                reduction[1] = originReductionScale * this->m_networkErrorOrigin[1];
                reduction[2] = originReductionScale * this->m_networkErrorOrigin[2];
                this->m_networkErrorOrigin[0] = this->m_networkErrorOrigin[0] - reduction[0];
                this->m_networkErrorOrigin[1] = this->m_networkErrorOrigin[1] - reduction[1];
                this->m_networkErrorOrigin[2] = this->m_networkErrorOrigin[2] - reduction[2];
                if ( (float)((float)(errorAReductionRate * dt) - 1.0) < 0.0 )
                    angleLerpScale = errorAReductionRate * dt;
                else
                    angleLerpScale = 1.0f;
                if ( (float)(0.0 - (float)(errorAReductionRate * dt)) < 0.0 )
                    angleReductionScale = angleLerpScale;
                else
                    angleReductionScale = 0.0f;
                scale = angleReductionScale;
                reduction[0] = angleReductionScale * this->m_networkErrorAngles[0];
                reduction[1] = angleReductionScale * this->m_networkErrorAngles[1];
                reduction[2] = angleReductionScale * this->m_networkErrorAngles[2];
                AnglesSubtract(this->m_networkErrorAngles, reduction, this->m_networkErrorAngles);
            }
            this->m_lastErrorReductionTime = curTime;
            Phys_ObjGetInterpolatedState((int)this->m_phys_user_data, rbPosition, rbRotation);
            UnitQuatToAngles(rbRotation, rbAngles);
            rbPosition[0] = rbPosition[0] + this->m_networkErrorOrigin[0];
            rbPosition[1] = rbPosition[1] + this->m_networkErrorOrigin[1];
            rbPosition[2] = rbPosition[2] + this->m_networkErrorOrigin[2];
            rbAngles[0] = rbAngles[0] + this->m_networkErrorAngles[0];
            rbAngles[1] = rbAngles[1] + this->m_networkErrorAngles[1];
            rbAngles[2] = rbAngles[2] + this->m_networkErrorAngles[2];
            *origin = rbPosition[0];
            origin[1] = rbPosition[1];
            origin[2] = rbPosition[2];
            *angles = rbAngles[0];
            angles[1] = rbAngles[1];
            angles[2] = rbAngles[2];
        }
        Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
        nanassertvec3(origin);
        nanassertvec3(angles);
    }
    else
    {
        Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
    }
}

int duration_0 = 100;

// Decomp:  CoDMPServer.c:659316  (RVA 00829810)
void __cdecl G_SetVehDriverInputs(int localClientNum, int vehEntNum, usercmd_s *cmd)
{
    float *inVector;
    float axis[9];
    float *m_networkErrorAngles;
    float *networkAnglesAccum;
    float *m_networkErrorOrigin;
    float *networkOriginAccum;
    float *origin;
    NitrousVehicle *nitrousVehicle;
    NitrousVehicleController *p_mVehicleController;
    phys_vec3 *velocity;
    phys_vec3 *aVelocity;
    float localPos[3];
    float currentOriginError[3];
    phys_mat44 *mat;
    float localPhysAxis[3][3];
    float localAngles[3];
    float currentAnglesError[3];
    gentity_s *ent;
    gentity_s *self;
    self = &g_entities[vehEntNum];
    if ( self->scr_vehicle )
    {
        if ( self->scr_vehicle->nitrousVehicle )
        {
            nitrousVehicle = self->scr_vehicle->nitrousVehicle;
            p_mVehicleController = &nitrousVehicle->mVehicleController;
            memcpy(&nitrousVehicle->mVehicleController.m_cmd, cmd, sizeof(nitrousVehicle->mVehicleController.m_cmd));
            nitrousVehicle->mVehicleController.m_cmd_local_client_num = localClientNum;
            p_mVehicleController->m_cmd_valid = 1;
            //if ( EntHandle::isDefined(&self->r.ownerNum) )
            if ( self->r.ownerNum.isDefined() )
            {
                //ent = EntHandle::ent(&self->r.ownerNum);
                ent = self->r.ownerNum.ent();
                if ( (ent->s.lerp.eFlags & 0x4000) == 0
                    && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
                                2385,
                                0,
                                "%s",
                                "ent->s.lerp.eFlags & EF_VEHICLE_ACTIVE") )
                {
                    __debugbreak();
                }
                if ( ent->s.number >= 32
                    && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
                                2386,
                                0,
                                "%s",
                                "ent->s.number < MAX_CLIENTS") )
                {
                    __debugbreak();
                }
                if ( svs.clients[ent->s.clientNum].header.vehicle.inVehicle )
                {
                    Sys_EnterCriticalSection(CRITSECT_PHYSICS);
                    mat = &self->scr_vehicle->nitrousVehicle->m_phys_user_data->body->m_mat;
                    velocity = &self->scr_vehicle->nitrousVehicle->m_phys_user_data->body->m_t_vel;
                    aVelocity = &self->scr_vehicle->nitrousVehicle->m_phys_user_data->body->m_a_vel;
                    Phys_NitrousVecToVec3(&mat->w, localPos);
                    origin = svs.clients[ent->s.clientNum].header.vehicle.origin;
                    currentOriginError[0] = localPos[0] - *origin;
                    currentOriginError[1] = localPos[1] - origin[1];
                    currentOriginError[2] = localPos[2] - origin[2];
                    m_networkErrorOrigin = self->scr_vehicle->nitrousVehicle->m_networkErrorOrigin;
                    networkOriginAccum = self->scr_vehicle->nitrousVehicle->m_networkErrorOrigin;
                    *m_networkErrorOrigin = currentOriginError[0] + *networkOriginAccum;
                    m_networkErrorOrigin[1] = currentOriginError[1] + networkOriginAccum[1];
                    m_networkErrorOrigin[2] = currentOriginError[2] + networkOriginAccum[2];
                    Phys_NitrousMat44ToVec33(mat, localPhysAxis);
                    AxisToAngles(localPhysAxis, localAngles);
                    AnglesSubtract(localAngles, svs.clients[ent->s.clientNum].header.vehicle.angles, currentAnglesError);
                    m_networkErrorAngles = self->scr_vehicle->nitrousVehicle->m_networkErrorAngles;
                    networkAnglesAccum = self->scr_vehicle->nitrousVehicle->m_networkErrorAngles;
                    *m_networkErrorAngles = currentAnglesError[0] + *networkAnglesAccum;
                    m_networkErrorAngles[1] = currentAnglesError[1] + networkAnglesAccum[1];
                    m_networkErrorAngles[2] = currentAnglesError[2] + networkAnglesAccum[2];
                    Phys_Vec3ToNitrousVec(svs.clients[ent->s.clientNum].header.vehicle.tVel, velocity);
                    Phys_Vec3ToNitrousVec(svs.clients[ent->s.clientNum].header.vehicle.aVel, aVelocity);
                    inVector = svs.clients[ent->s.clientNum].header.vehicle.origin;
                    AnglesToAxis(svs.clients[ent->s.clientNum].header.vehicle.angles, (float (*)[3])axis);
                    Phys_AxisToNitrousMat((float (*)[3])axis, mat);
                    Phys_Vec3ToNitrousVec(inVector, &mat->w);
                    if ( phys_drawNitrousVehicle->current.enabled )
                    {
                        G_DebugStar(svs.clients[ent->s.clientNum].header.vehicle.origin, colorGreen, duration_0);
                        G_DebugStar(localPos, colorRed, duration_0);
                    }
                    Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
                }
            }
        }
    }
}

// Decomp:  CoDMPServer.c:659433  (RVA 00829C90)
void __cdecl G_ClearVehicleInputs()
{
    phys_free_list<NitrousVehicle>::T_internal_base *i;
    if ( g_rb_vehicle_list.m_list_count > 0 )
    {
        PROF_SCOPED("G_ClearVehicleInputs");
        Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
        Sys_EnterCriticalSection(CRITSECT_PHYSICS);
        for ( i = g_rb_vehicle_list.m_dummy_head.m_next_T_internal;
                    &g_rb_vehicle_list != (phys_free_list<NitrousVehicle> *)i;
                    i = i->m_next_T_internal )
        {
            if ( i[70].m_next_T_internal
                && i[70].m_next_T_internal[43].m_prev_T_internal
                && VEH_GetSeatOccupantEntNum((gentity_s *)i[70].m_next_T_internal, 0) == 1023 )
            {
                memset((unsigned __int8 *)&i[100], 0, 0x34u);
            }
        }
        Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
        Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
    }
}

// Decomp:  CoDMPServer.c:659470  (RVA 00829DB0)
void __cdecl CG_SetVehDriverInputs(int localClientNum, int vehEntNum, usercmd_s *cmd)
{
    NitrousVehicle *nitrousVeh;
    centity_s *self;
    self = CG_GetEntity(localClientNum, vehEntNum);
    if ( self->nitrousVeh )
    {
        nitrousVeh = self->nitrousVeh;
        memcpy(&nitrousVeh->mVehicleController.m_cmd, cmd, sizeof(nitrousVeh->mVehicleController.m_cmd));
        nitrousVeh->mVehicleController.m_cmd_local_client_num = localClientNum;
        nitrousVeh->mVehicleController.m_cmd_valid = 1;
    }
}

// Decomp:  CoDMPServer.c:659488  (RVA 00829E20)
void __cdecl Vehicle_Launch(int localClientNum, centity_s *cent, float *hitp, const float *force, bool relative)
{
    float value;
    float scaledForce[3];
    if ( !localClientNum && cent->nitrousVeh )
    {
        if ( cent->nitrousVeh->m_phys_user_data )
        {
            value = dynEnt_bulletForce->current.value;
            scaledForce[0] = value * *force;
            scaledForce[1] = value * force[1];
            scaledForce[2] = value * force[2];
            Phys_ObjAddForce((int)cent->nitrousVeh->m_phys_user_data, hitp, scaledForce, relative);
        }
    }
}

//NitrousVehicle *__thiscall phys_free_list<NitrousVehicle>::add(
//                phys_free_list<NitrousVehicle> *this,
//                int no_error,
//                const char *error_msg)
//{
//    phys_free_list<NitrousVehicle>::T_internal *ptr;
//
//    ptr = (phys_free_list<NitrousVehicle>::T_internal *)PMM_ALLOC(0x460u, 0x10u);
//    if ( ptr )
//    {
//        NitrousVehicle::NitrousVehicle(&ptr->m_data);
//        ptr->m_prev_T_internal = &this->m_dummy_head;
//        ptr->m_next_T_internal = this->m_dummy_head.m_next_T_internal;
//        this->m_dummy_head.m_next_T_internal->m_prev_T_internal = ptr;
//        this->m_dummy_head.m_next_T_internal = ptr;
//        ++this->m_list_count;
//        phys_free_list<NitrousVehicle>::debug_add(this, ptr);
//        return &ptr->m_data;
//    }
//    else
//    {
//        if ( !no_error )
//        {
//            if ( _tlAssert("c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h", 470, "no_error", error_msg) )
//                __debugbreak();
//        }
//        return 0;
//    }
//}
//
//void __thiscall phys_free_list<NitrousVehicle>::remove(phys_free_list<NitrousVehicle> *this, NitrousVehicle *data_)
//{
//    if ( data_ )
//    {
//        PMM_VALIDATE((char *)&data_[-1].m_steer_front_back_length, 0x460u, 0x10u);
//        phys_free_list<NitrousVehicle>::remove(
//            this,
//            (phys_free_list<NitrousVehicle>::T_internal *)&data_[-1].m_steer_front_back_length);
//    }
//}
//
//void __thiscall phys_free_list<NitrousVehicle>::debug_add(
//                phys_free_list<NitrousVehicle> *this,
//                phys_free_list<NitrousVehicle>::T_internal *T_i)
//{
//    int m_list_count;
//
//    if ( this->m_list_count_high_water <= this->m_list_count )
//        m_list_count = this->m_list_count;
//    else
//        m_list_count = this->m_list_count_high_water;
//    this->m_list_count_high_water = m_list_count;
//    if ( this->m_ptr_list_count >= 256 )
//    {
//        T_i->m_ptr_list_index = -1;
//    }
//    else
//    {
//        T_i->m_ptr_list_index = this->m_ptr_list_count;
//        this->m_ptr_list[this->m_ptr_list_count++] = &T_i->m_data;
//    }
//}
//
//void __thiscall phys_free_list<NitrousVehicle>::remove(
//                phys_free_list<NitrousVehicle> *this,
//                phys_free_list<NitrousVehicle>::T_internal *data)
//{
//    phys_free_list<NitrousVehicle>::T_internal_base *next;
//    phys_free_list<NitrousVehicle>::T_internal_base *prev;
//
//    if ( !data
//        && _tlAssert("c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h", 477, "data", "") )
//    {
//        __debugbreak();
//    }
//    --this->m_list_count;
//    phys_free_list<NitrousVehicle>::debug_remove(this, data);
//    next = data->m_next_T_internal;
//    prev = data->m_prev_T_internal;
//    prev->m_next_T_internal = next;
//    next->m_prev_T_internal = prev;
//    PMM_FREE((unsigned __int8 *)data, 0x460u, 0x10u);
//}
//
//void __thiscall phys_free_list<NitrousVehicle>::debug_remove(
//                phys_free_list<NitrousVehicle> *this,
//                phys_free_list<NitrousVehicle>::T_internal *T_i)
//{
//    if ( T_i->m_ptr_list_index != -1 )
//    {
//        if ( T_i->m_ptr_list_index >= 0x100u
//            && _tlAssert(
//                     "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
//                     421,
//                     "T_i->m_ptr_list_index >= 0 && T_i->m_ptr_list_index < PTR_LIST_SIZE",
//                     "") )
//        {
//            __debugbreak();
//        }
//        if ( this->m_ptr_list[T_i->m_ptr_list_index] != &T_i->m_data )
//        {
//            if ( _tlAssert(
//                         "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
//                         422,
//                         "m_ptr_list[T_i->m_ptr_list_index] == &T_i->m_data",
//                         "") )
//            {
//                __debugbreak();
//            }
//        }
//        this->m_ptr_list[--this->m_ptr_list_count][1].m_wheel_state[0].m_state = T_i->m_ptr_list_index;
//        this->m_ptr_list[T_i->m_ptr_list_index] = this->m_ptr_list[this->m_ptr_list_count];
//    }
//}
// defined in phys_render
//void    make_rotate(phys_mat44 *mat, const phys_vec3 *v1, const phys_vec3 *v2)
//{
//    float v4;
//    float v5;
//    float len;
//    float lena;
//    float co_;
//    float v9;
//    float si_;
//    phys_vec3 ud;
//    float retaddr;
//
//    ud.y = a1;
//    ud.z = retaddr;
//    phys_cross((phys_vec3 *)&co_, v1, v2);
//    len = (float)((float)(co_ * co_) + (float)(v9 * v9)) + (float)(si_ * si_);
//    if ( len >= 0.0000099999997 )
//    {
//        lena = sqrtf(len);
//        co_ = co_ * (float)(1.0 / lena);
//        v9 = v9 * (float)(1.0 / lena);
//        si_ = si_ * (float)(1.0 / lena);
//        v5 = (float)((float)(v1->x * v2->x) + (float)(v1->y * v2->y)) + (float)(v1->z * v2->z);
//        v4 = sqrtf((float)(v5 * v5) + (float)(lena * lena));
//        make_rotate(mat, (const phys_vec3 *)&co_, v5 / v4, lena / v4);
//    }
//    else
//    {
//        phys_mat44::operator=(mat, &PHYS_IDENTITY_MATRIX_44);
//    }
//}
