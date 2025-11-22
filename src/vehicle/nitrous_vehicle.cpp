#include "nitrous_vehicle.h"

VehicleParameter *__thiscall VehicleParameter::VehicleParameter(VehicleParameter *this)
{
  float *m_buoyancybox_max; // ecx

  this->m_speed_max = FLOAT_1100_0;
  this->m_accel_max = FLOAT_300_0;
  this->m_reverse_scale = FLOAT_0_80000001;
  this->m_steer_angle_max = FLOAT_0_60000002;
  this->m_steer_speed = FLOAT_5_0;
  this->m_wheel_radius = FLOAT_15_0;
  this->m_susp_spring_k = FLOAT_20_0;
  this->m_susp_damp_k = FLOAT_1_0;
  this->m_susp_adj = FLOAT_10_0;
  this->m_susp_hard_limit = FLOAT_23_0;
  this->m_susp_min_height = FLOAT_N100_0;
  this->m_tire_fric_fwd = FLOAT_2_5;
  this->m_tire_fric_side = FLOAT_2_3;
  this->m_tire_fric_brake = FLOAT_2_3;
  this->m_tire_fric_hand_brake = *(float *)&FLOAT_0_0;
  this->m_body_mass = FLOAT_1_0;
  this->m_roll_stability = FLOAT_10_0;
  this->m_pitch_roll_resistance = FLOAT_50_0;
  this->m_yaw_resistance = *(float *)&FLOAT_0_0;
  this->m_upright_strength = FLOAT_50_0;
  this->m_tilt_fakey = FLOAT_0_25;
  this->m_peel_out_max_speed = FLOAT_150_0;
  this->m_tire_damp_coast = FLOAT_10_0;
  this->m_tire_damp_brake = FLOAT_100_0;
  this->m_tire_damp_hand = FLOAT_10000_0;
  this->m_traction_type = TRACTION_TYPE_ALL_WD;
  this->m_water_ebrake_power = FLOAT_1_5;
  this->m_tire_fric_side_max = *(float *)&FLOAT_0_0;
  this->m_bbox_min[0] = *(float *)&FLOAT_0_0;
  this->m_bbox_min[1] = *(float *)&FLOAT_0_0;
  this->m_bbox_min[2] = *(float *)&FLOAT_0_0;
  this->m_bbox_max[0] = *(float *)&FLOAT_0_0;
  this->m_bbox_max[1] = *(float *)&FLOAT_0_0;
  this->m_bbox_max[2] = *(float *)&FLOAT_0_0;
  this->m_mass_center_offset[0] = *(float *)&FLOAT_0_0;
  this->m_mass_center_offset[1] = *(float *)&FLOAT_0_0;
  this->m_mass_center_offset[2] = *(float *)&FLOAT_0_0;
  this->m_buoyancybox_min[0] = *(float *)&FLOAT_0_0;
  this->m_buoyancybox_min[1] = *(float *)&FLOAT_0_0;
  this->m_buoyancybox_min[2] = *(float *)&FLOAT_0_0;
  this->m_buoyancybox_max[0] = *(float *)&FLOAT_0_0;
  m_buoyancybox_max = this->m_buoyancybox_max;
  m_buoyancybox_max[1] = *(float *)&FLOAT_0_0;
  m_buoyancybox_max[2] = *(float *)&FLOAT_0_0;
  this->m_mass_center_offset[2] = mass_z_offset;
  return this;
}

NitrousVehicle *__thiscall NitrousVehicle::NitrousVehicle(NitrousVehicle *this)
{
  int k; // [esp+18h] [ebp-10h]
  int v3; // [esp+1Ch] [ebp-Ch]
  phys_mat44 *j; // [esp+20h] [ebp-8h]
  int i; // [esp+24h] [ebp-4h]

  v3 = 6;
  for ( j = this->m_wheel_orig_relpo; --v3 >= 0; ++j )
    ;
  this->m_parameter = 0;
  this->m_script_brake = *(float *)&FLOAT_0_0;
  this->m_hand_brake_friction_time = *(float *)&FLOAT_0_0;
  this->m_owner = 0;
  this->m_entnum = 0;
  this->m_vehicle_info = 0;
  this->m_phys_user_data = 0;
  this->m_xmodel = 0;
  this->m_orientation_constraint = 0;
  this->m_vpc = 0;
  this->m_notify_flags = 0;
  this->m_server_notify_flags = 0;
  for ( k = 0; k < 2; ++k )
    this->mVehicleController.m_cmd.button_bits.array[k] = 0;
  this->m_fake_rpm = *(float *)&FLOAT_0_0;
  this->m_trackDistAccumLeftInches = 0;
  this->m_trackDistAccumRightInches = 0;
  this->m_sfx_state_flags = 0;
  this->m_flags = 0;
  this->refcount = 0;
  this->m_state_flags = 0;
  this->m_flags |= 1u;
  this->m_flags &= ~2u;
  for ( i = 0; i < 6; ++i )
    this->m_wheels[i] = 0;
  return this;
}

void __thiscall NitrousVehicle::init(NitrousVehicle *this, gentity_s *owner, const VehicleParameter *parameter)
{
  DObj *ServerDObj; // eax
  float axis[9]; // [esp+28h] [ebp-44h] BYREF
  phys_vec3 *p_m_ground_vel; // [esp+4Ch] [ebp-20h]
  float *m_networkErrorAngles; // [esp+50h] [ebp-1Ch]
  float *m_networkErrorOrigin; // [esp+54h] [ebp-18h]
  float *m_angles; // [esp+58h] [ebp-14h]
  float *currentAngles; // [esp+5Ch] [ebp-10h]
  float *m_origin; // [esp+60h] [ebp-Ch]
  float *currentOrigin; // [esp+64h] [ebp-8h]
  int i; // [esp+68h] [ebp-4h]
  int savedregs; // [esp+6Ch] [ebp+0h] BYREF

  if ( !parameter )
    parameter = &g_default_params;
  NitrousVehicle::pause_physics(this, 0);
  this->m_owner = owner;
  this->m_entnum = owner->s.number;
  this->m_vehicle_info = BG_GetVehicleInfo(owner->scr_vehicle->infoIdx);
  ServerDObj = Com_GetServerDObj(this->m_entnum);
  this->m_xmodel = DObjGetModel(ServerDObj, 0);
  this->m_parameter = parameter;
  this->m_hand_brake = *(float *)&FLOAT_0_0;
  this->m_brake = *(float *)&FLOAT_0_0;
  NitrousVehicle::set_throttle(this, 0.0);
  this->m_steer_factor = *(float *)&FLOAT_0_0;
  this->m_forward_vel = *(float *)&FLOAT_0_0;
  this->m_script_brake = *(float *)&FLOAT_0_0;
  this->m_stuck_time = *(float *)&FLOAT_0_0;
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
  this->m_networkErrorOrigin[0] = *(float *)&FLOAT_0_0;
  m_networkErrorOrigin[1] = *(float *)&FLOAT_0_0;
  m_networkErrorOrigin[2] = *(float *)&FLOAT_0_0;
  m_networkErrorAngles = this->m_networkErrorAngles;
  this->m_networkErrorAngles[0] = *(float *)&FLOAT_0_0;
  m_networkErrorAngles[1] = *(float *)&FLOAT_0_0;
  m_networkErrorAngles[2] = *(float *)&FLOAT_0_0;
  p_m_ground_vel = &this->m_ground_vel;
  this->m_ground_vel.x = *(float *)&FLOAT_0_0;
  p_m_ground_vel->y = *(float *)&FLOAT_0_0;
  p_m_ground_vel->z = *(float *)&FLOAT_0_0;
  this->m_lastErrorReductionTime = 0;
  this->m_speed_scale = FLOAT_1_0;
  this->m_drivepath_scale = FLOAT_1_0;
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
  if ( COERCE_FLOAT(LODWORD(this->m_prev_rb_mat.w.x) & _mask__AbsFloat_) > 100000.0
    || COERCE_FLOAT(LODWORD(this->m_prev_rb_mat.w.y) & _mask__AbsFloat_) > 100000.0
    || COERCE_FLOAT(LODWORD(this->m_prev_rb_mat.w.z) & _mask__AbsFloat_) > 100000.0 )
  {
    phys_exec_debug_callback(0);
  }
  NitrousVehicle::_setup_wheels(this, (DObj *)&savedregs, 0);
  for ( i = 0; i < 4; ++i )
  {
    this->m_wheel_state[i].m_state = WHEEL_STATE_AIRBORN;
    this->m_wheel_state[i].m_last_state = WHEEL_STATE_AIRBORN;
    this->m_wheel_state[i].m_state_time = *(float *)&FLOAT_0_0;
    this->m_wheel_state[i].m_rate = *(float *)&FLOAT_0_0;
  }
}

void __thiscall NitrousVehicle::set_throttle(NitrousVehicle *this, float throttle)
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

void __thiscall NitrousVehicle::init(
        NitrousVehicle *this,
        int localClientNum,
        centity_s *owner,
        const VehicleParameter *parameter)
{
  float axis[9]; // [esp+1Ch] [ebp-4Ch] BYREF
  phys_vec3 *p_m_ground_vel; // [esp+40h] [ebp-28h]
  float *m_networkErrorAngles; // [esp+44h] [ebp-24h]
  float *m_networkErrorOrigin; // [esp+48h] [ebp-20h]
  float *m_angles; // [esp+4Ch] [ebp-1Ch]
  float *angles; // [esp+50h] [ebp-18h]
  float *m_origin; // [esp+54h] [ebp-14h]
  float *origin; // [esp+58h] [ebp-10h]
  int i; // [esp+5Ch] [ebp-Ch]
  const DObj *obj; // [esp+60h] [ebp-8h]
  const cgs_t *cgs; // [esp+64h] [ebp-4h]
  int savedregs; // [esp+68h] [ebp+0h] BYREF

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
    this->m_vehicle_info = CG_GetVehicleInfo(owner->nextState.un2.vehicleState.vehicleInfoIndex);
    this->m_parameter = parameter;
    this->m_hand_brake = *(float *)&FLOAT_0_0;
    this->m_brake = *(float *)&FLOAT_0_0;
    NitrousVehicle::set_throttle(this, 0.0);
    this->m_steer_factor = *(float *)&FLOAT_0_0;
    this->m_forward_vel = *(float *)&FLOAT_0_0;
    this->m_script_brake = *(float *)&FLOAT_0_0;
    this->m_stuck_time = *(float *)&FLOAT_0_0;
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
    this->m_networkErrorOrigin[0] = *(float *)&FLOAT_0_0;
    m_networkErrorOrigin[1] = *(float *)&FLOAT_0_0;
    m_networkErrorOrigin[2] = *(float *)&FLOAT_0_0;
    m_networkErrorAngles = this->m_networkErrorAngles;
    this->m_networkErrorAngles[0] = *(float *)&FLOAT_0_0;
    m_networkErrorAngles[1] = *(float *)&FLOAT_0_0;
    m_networkErrorAngles[2] = *(float *)&FLOAT_0_0;
    p_m_ground_vel = &this->m_ground_vel;
    this->m_ground_vel.x = *(float *)&FLOAT_0_0;
    p_m_ground_vel->y = *(float *)&FLOAT_0_0;
    p_m_ground_vel->z = *(float *)&FLOAT_0_0;
    this->m_speed_scale = FLOAT_1_0;
    this->m_drivepath_scale = FLOAT_1_0;
    this->m_boost_start_time = 0;
    this->m_boost_time_pool = this->m_vehicle_info->boostDuration;
    this->m_throttle_held_down = 0;
    AnglesToAxis(this->m_angles, (float (*)[3])axis);
    Phys_AxisToNitrousMat((float (*)[3])axis, &this->m_prev_rb_mat);
    Phys_Vec3ToNitrousVec(this->m_origin, &this->m_prev_rb_mat.w);
    if ( COERCE_FLOAT(LODWORD(this->m_prev_rb_mat.w.x) & _mask__AbsFloat_) > 100000.0
      || COERCE_FLOAT(LODWORD(this->m_prev_rb_mat.w.y) & _mask__AbsFloat_) > 100000.0
      || COERCE_FLOAT(LODWORD(this->m_prev_rb_mat.w.z) & _mask__AbsFloat_) > 100000.0 )
    {
      phys_exec_debug_callback(0);
    }
    NitrousVehicle::_setup_wheels(this, (DObj *)&savedregs, localClientNum);
    for ( i = 0; i < 4; ++i )
    {
      this->m_wheel_state[i].m_state = WHEEL_STATE_AIRBORN;
      this->m_wheel_state[i].m_last_state = WHEEL_STATE_AIRBORN;
      this->m_wheel_state[i].m_state_time = *(float *)&FLOAT_0_0;
      this->m_wheel_state[i].m_rate = *(float *)&FLOAT_0_0;
    }
  }
}

PhysObjUserData *__cdecl Phys_ObjCreateNitrousVehicle(
        const float *position,
        const float *angles,
        const float *velocity,
        const float *translation,
        const PhysPreset *physPreset,
        gjk_geom_list_t *gjk_geom_list)
{
  double v6; // xmm0_8
  BodyState state; // [esp+18h] [ebp-B0h] BYREF
  float axis[3][3]; // [esp+A4h] [ebp-24h] BYREF
  int savedregs; // [esp+C8h] [ebp+0h] BYREF

  if ( ((*(unsigned int *)position & 0x7F800000) == 0x7F800000
     || ((unsigned int)position[1] & 0x7F800000) == 0x7F800000
     || ((unsigned int)position[2] & 0x7F800000) == 0x7F800000)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
          232,
          0,
          "%s",
          "!IS_NAN((position)[0]) && !IS_NAN((position)[1]) && !IS_NAN((position)[2])") )
  {
    __debugbreak();
  }
  if ( ((*(unsigned int *)velocity & 0x7F800000) == 0x7F800000
     || ((unsigned int)velocity[1] & 0x7F800000) == 0x7F800000
     || ((unsigned int)velocity[2] & 0x7F800000) == 0x7F800000)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
          233,
          0,
          "%s",
          "!IS_NAN((velocity)[0]) && !IS_NAN((velocity)[1]) && !IS_NAN((velocity)[2])") )
  {
    __debugbreak();
  }
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
    v6 = phys_gravity->current.value * -1.1;
  else
    v6 = phys_gravity->current.value * -0.9;
  state.buoyancy = (int)v6;
  state.underwater = 0;
  state.id = -1;
  return Phys_CreateBodyFromState((int)&savedregs, 0, &state, gjk_geom_list, 1);
}

void  NitrousVehicle::unpause_physics(NitrousVehicle *this@<ecx>, int a2@<ebp>)
{
  PhysObjUserData *NitrousVehicle; // eax
  float *v3; // [esp-54h] [ebp-104h]
  float v4; // [esp-48h] [ebp-F8h]
  float v5; // [esp-44h] [ebp-F4h]
  float v6; // [esp-30h] [ebp-E0h]
  float value; // [esp-10h] [ebp-C0h]
  float v8[3]; // [esp-Ch] [ebp-BCh] BYREF
  phys_vec3 gravity_dir; // [esp+0h] [ebp-B0h]
  broad_phase_info *i; // [esp+10h] [ebp-A0h]
  broad_phase_group *v11; // [esp+14h] [ebp-9Ch]
  unsigned int v12; // [esp+18h] [ebp-98h]
  unsigned int m_env_collision_flags; // [esp+1Ch] [ebp-94h]
  broad_phase_group *bpg; // [esp+20h] [ebp-90h]
  int flags[2]; // [esp+24h] [ebp-8Ch] BYREF
  broad_phase_info *bpi; // [esp+2Ch] [ebp-84h] BYREF
  create_gjk_geom_collision_visitor collision_visitor; // [esp+30h] [ebp-80h] BYREF
  gjk_geom_list_t gjk_geom_list; // [esp+38h] [ebp-78h]
  float vel[4]; // [esp+40h] [ebp-70h] BYREF
  PhysPreset physPreset; // [esp+50h] [ebp-60h]
  int v21; // [esp+A4h] [ebp-Ch]
  void *v22; // [esp+A8h] [ebp-8h]
  void *retaddr; // [esp+B0h] [ebp+0h]

  v21 = a2;
  v22 = retaddr;
  LODWORD(physPreset.buoyancyBoxMax[2]) = this;
  if ( (this->m_flags & 1) != 0 )
  {
    *(unsigned int *)(LODWORD(physPreset.buoyancyBoxMax[2]) + 600) &= ~1u;
    physPreset.buoyancyBoxMax[1] = *(float *)(LODWORD(physPreset.buoyancyBoxMax[2]) + 600);
    *(float *)&physPreset.name = FLOAT_0_30000001;
    physPreset.mass = FLOAT_1_0;
    physPreset.bounce = FLOAT_1_0;
    if ( *(_WORD *)(*(unsigned int *)(LODWORD(physPreset.buoyancyBoxMax[2]) + 556) + 64) == 4 )
      *(float *)&physPreset.flags = FLOAT_1_0;
    else
      *(float *)&physPreset.flags = FLOAT_0_40000001;
    vel[0] = *(float *)(LODWORD(physPreset.buoyancyBoxMax[2]) + 496);
    vel[3] = *(float *)(LODWORD(vel[0]) + 60);
    physPreset.sndAliasPrefix = (const char *)(*(float *)(*(unsigned int *)(LODWORD(physPreset.buoyancyBoxMax[2]) + 496) + 208) != 0.0);
    collision_visitor.gjk_geom_list = *(gjk_geom_list_t **)&FLOAT_0_0;
    gjk_geom_list.m_first_geom = *(gjk_base_t **)&FLOAT_0_0;
    gjk_geom_list.m_geom_count = *(unsigned int *)&FLOAT_0_0;
    bpi = 0;
    collision_visitor.__vftable = 0;
    flags[0] = (int)&create_gjk_geom_collision_visitor::`vftable';
    flags[1] = (int)&bpi;
    create_xmodel_gjk_geom(
      *(const XModel **)(LODWORD(physPreset.buoyancyBoxMax[2]) + 564),
      (gjk_collision_visitor *)flags,
      -1,
      (unsigned int)&cls.recentServers[7546].city[57],
      1,
      0,
      0);
    NitrousVehicle = Phys_ObjCreateNitrousVehicle(
                       (const float *)(LODWORD(physPreset.buoyancyBoxMax[2]) + 568),
                       (const float *)(LODWORD(physPreset.buoyancyBoxMax[2]) + 580),
                       (const float *)&collision_visitor.gjk_geom_list,
                       (const float *)(*(unsigned int *)(LODWORD(physPreset.buoyancyBoxMax[2]) + 496) + 196),
                       (const PhysPreset *)&vel[1],
                       (gjk_geom_list_t *)&bpi);
    *(unsigned int *)(LODWORD(physPreset.buoyancyBoxMax[2]) + 560) = NitrousVehicle;
    *(float *)(*(unsigned int *)(LODWORD(physPreset.buoyancyBoxMax[2]) + 560) + 4) = physPreset.buoyancyBoxMax[2];
    if ( (*(unsigned int *)(*(unsigned int *)(*(unsigned int *)(LODWORD(physPreset.buoyancyBoxMax[2]) + 560) + 160) + 48) & 1) != 0 )
    {
      bpg = (broad_phase_group *)broad_phase_base::get_bpi(*(broad_phase_base **)(*(unsigned int *)(LODWORD(physPreset.buoyancyBoxMax[2])
                                                                                            + 560)
                                                                                + 160));
      m_env_collision_flags = bpg->m_env_collision_flags;
      v12 = (m_env_collision_flags
           | (8 * (*(unsigned int *)(*(unsigned int *)(LODWORD(physPreset.buoyancyBoxMax[2]) + 556) + 68) != 0) + 8))
          & 0xFFFFFF7F;
      bpg->m_env_collision_flags = v12;
    }
    else
    {
      v11 = broad_phase_base::get_bpg(*(broad_phase_base **)(*(unsigned int *)(LODWORD(physPreset.buoyancyBoxMax[2]) + 560)
                                                           + 160));
      for ( i = v11->m_list_bpi_head; i; i = (broad_phase_info *)i->m_list_bpb_next )
      {
        LODWORD(gravity_dir.w) = i->m_env_collision_flags;
        LODWORD(gravity_dir.z) = (LODWORD(gravity_dir.w)
                                | (8 * (*(unsigned int *)(*(unsigned int *)(LODWORD(physPreset.buoyancyBoxMax[2]) + 556) + 68) != 0)
                                 + 8))
                               & 0xFFFFFF7F;
        i->m_env_collision_flags = LODWORD(gravity_dir.z);
      }
    }
    Phys_Vec3ToNitrousVec(&phys_gravity_dir->current.value, (phys_vec3 *)v8);
    value = phys_gravity->current.value;
    v6 = phys_vehicleGravityMultiplier->current.value;
    v4 = v6 * (float)(value * v8[1]);
    v5 = v6 * (float)(value * v8[2]);
    v3 = (float *)(**(unsigned int **)(LODWORD(physPreset.buoyancyBoxMax[2]) + 560) + 128);
    *v3 = v6 * (float)(value * v8[0]);
    v3[1] = v4;
    v3[2] = v5;
    NitrousVehicle::update_parms(
      (NitrousVehicle *)LODWORD(physPreset.buoyancyBoxMax[2]),
      *(const VehicleParameter **)(LODWORD(physPreset.buoyancyBoxMax[2]) + 496),
      1);
    *(unsigned int *)(LODWORD(physPreset.buoyancyBoxMax[2]) + 600) |= 2u;
    NitrousVehicleController::Init((NitrousVehicleController *)(LODWORD(physPreset.buoyancyBoxMax[2]) + 720));
  }
}

void __thiscall NitrousVehicleController::Init(NitrousVehicleController *this)
{
  this->m_stuck_time = *(float *)&FLOAT_0_0;
  this->m_stopped_time = *(float *)&FLOAT_0_0;
  memset((unsigned __int8 *)&this->m_cmd, 0, sizeof(this->m_cmd));
  this->m_cmd_local_client_num = 0;
  this->m_cmd_valid = 1;
  this->m_stop_at_goal = 0;
}

void __thiscall NitrousVehicle::pause_physics(NitrousVehicle *this, bool shutdown)
{
  if ( (this->m_flags & 1) == 0 )
  {
    Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
    if ( !shutdown )
    {
      phys_mat44::operator=(&this->m_prev_rb_mat, &this->m_phys_user_data->body->m_mat);
      NitrousVehicle::remove_wheels(this);
      if ( this->m_orientation_constraint )
        phys_sys::destroy(this->m_orientation_constraint);
      this->m_orientation_constraint = 0;
    }
    NitrousVehicle::cleanup_path(this);
    this->m_flags |= 1u;
    Phys_ObjDestroy(0, (int)this->m_phys_user_data);
    this->m_phys_user_data = 0;
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
  }
}

void __thiscall NitrousVehicle::_update_pause(NitrousVehicle *this)
{
  bool v1; // [esp+0h] [ebp-18h]
  int savedregs; // [esp+18h] [ebp+0h] BYREF

  this->m_flags |= 0x40u;
  v1 = (this->m_flags & 4) != 0 || (this->m_flags & 0x80) != 0 || (this->m_flags & 0x100) != 0;
  if ( this->m_phys_user_data )
    rigid_body::set_flag(this->m_phys_user_data->body, 0x40u, v1 | 1);
  if ( (this->m_flags & 1) != 0 )
    NitrousVehicle::unpause_physics(this, (int)&savedregs);
}

void __thiscall rigid_body::set_flag(rigid_body *this, unsigned int f, int b)
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

unsigned int __thiscall rigid_body::is_group_stable(rigid_body *this)
{
  return this->m_flags & 8;
}

void __userpurge NitrousVehicle::update_parms(
        NitrousVehicle *this@<ecx>,
        int a2@<ebp>,
        const VehicleParameter *params,
        bool initialization)
{
  const VehicleParameter *m_parameter; // eax
  phys_vec3 *v5; // ecx
  double maximum_acceleration; // st7
  float fwd_fric_k; // [esp+8h] [ebp-32Ch]
  float suspension_stiffness_k; // [esp+10h] [ebp-324h]
  float suspension_damp_k; // [esp+14h] [ebp-320h]
  float hard_limit_dist; // [esp+18h] [ebp-31Ch]
  float roll_stability_factor; // [esp+1Ch] [ebp-318h]
  float v12; // [esp+40h] [ebp-2F4h]
  float v13; // [esp+ACh] [ebp-288h]
  float v14; // [esp+B0h] [ebp-284h]
  rigid_body_constraint_wheel *v15; // [esp+E4h] [ebp-250h]
  rigid_body_constraint_wheel *v16; // [esp+100h] [ebp-234h]
  float v17; // [esp+104h] [ebp-230h]
  float v18; // [esp+108h] [ebp-22Ch] BYREF
  float v19; // [esp+10Ch] [ebp-228h]
  float front_back_length; // [esp+110h] [ebp-224h]
  phys_vec3 front_steer_pt; // [esp+114h] [ebp-220h]
  int v22; // [esp+12Ch] [ebp-208h]
  int v23; // [esp+130h] [ebp-204h]
  int v25; // [esp+138h] [ebp-1FCh]
  bool v26; // [esp+13Ch] [ebp-1F8h]
  TractionType m_traction_type; // [esp+140h] [ebp-1F4h]
  int v28; // [esp+144h] [ebp-1F0h]
  int v29; // [esp+148h] [ebp-1ECh]
  int v31; // [esp+150h] [ebp-1E4h]
  int v32; // [esp+154h] [ebp-1E0h]
  int v34; // [esp+15Ch] [ebp-1D8h]
  float side_fric_max; // [esp+160h] [ebp-1D4h]
  float m_wheel_radius; // [esp+164h] [ebp-1D0h]
  unsigned int v37[2]; // [esp+168h] [ebp-1CCh] BYREF
  float wheel_radius; // [esp+170h] [ebp-1C4h]
  phys_vec3 susp_dir; // [esp+174h] [ebp-1C0h]
  float m_body_mass; // [esp+184h] [ebp-1B0h]
  const VehicleParameter *susp_damp_k; // [esp+188h] [ebp-1ACh]
  float susp_spring_k; // [esp+18Ch] [ebp-1A8h]
  float mass; // [esp+190h] [ebp-1A4h]
  float v44; // [esp+194h] [ebp-1A0h]
  int v45; // [esp+198h] [ebp-19Ch]
  float v46; // [esp+19Ch] [ebp-198h]
  float com_spring_factor; // [esp+1A0h] [ebp-194h]
  float dist_to_com; // [esp+1A4h] [ebp-190h]
  unsigned int v49[2]; // [esp+1A8h] [ebp-18Ch] BYREF
  float v50; // [esp+1B0h] [ebp-184h]
  phys_vec3 wheel_center_loc; // [esp+1B4h] [ebp-180h]
  float v52; // [esp+1C8h] [ebp-16Ch]
  float v53; // [esp+1CCh] [ebp-168h]
  float v54; // [esp+1D0h] [ebp-164h]
  phys_vec3 *v55; // [esp+1D4h] [ebp-160h]
  float x; // [esp+1D8h] [ebp-15Ch]
  float y; // [esp+1DCh] [ebp-158h]
  float z; // [esp+1E0h] [ebp-154h]
  phys_vec3 translation; // [esp+1E4h] [ebp-150h]
  bool v60; // [esp+1F4h] [ebp-140h]
  bool v61; // [esp+1F8h] [ebp-13Ch]
  rigid_body_constraint_wheel *rbc_wheel; // [esp+1FCh] [ebp-138h]
  int hasWheel; // [esp+200h] [ebp-134h]
  float v64; // [esp+204h] [ebp-130h]
  float v65; // [esp+208h] [ebp-12Ch] BYREF
  int i; // [esp+20Ch] [ebp-128h]
  float wheel_to_wheel_length; // [esp+210h] [ebp-124h]
  float v68; // [esp+224h] [ebp-110h]
  int v69; // [esp+228h] [ebp-10Ch]
  float v70; // [esp+22Ch] [ebp-108h]
  phys_vec3 *v71; // [esp+230h] [ebp-104h]
  phys_vec3 *p_w; // [esp+234h] [ebp-100h]
  const VehicleParameter *v73; // [esp+238h] [ebp-FCh]
  float v74; // [esp+23Ch] [ebp-F8h]
  environment_rigid_body *body; // [esp+240h] [ebp-F4h]
  phys_mat44 *p_cg2rb; // [esp+244h] [ebp-F0h]
  float maxAvel; // [esp+248h] [ebp-ECh] BYREF
  rigid_body *rb; // [esp+24Ch] [ebp-E8h]
  int v79; // [esp+250h] [ebp-E4h]
  int v80; // [esp+25Ch] [ebp-D8h]
  int v81; // [esp+260h] [ebp-D4h]
  int v82; // [esp+264h] [ebp-D0h]
  int v83; // [esp+268h] [ebp-CCh] BYREF
  int v84; // [esp+26Ch] [ebp-C8h]
  int v85; // [esp+270h] [ebp-C4h]
  phys_vec3 _com; // [esp+274h] [ebp-C0h]
  const VehicleParameter *v87; // [esp+284h] [ebp-B0h]
  float v88; // [esp+288h] [ebp-ACh] BYREF
  float v89; // [esp+28Ch] [ebp-A8h]
  float v90; // [esp+290h] [ebp-A4h]
  float v91; // [esp+2A4h] [ebp-90h] BYREF
  _BYTE v92[8]; // [esp+2A8h] [ebp-8Ch] BYREF
  phys_vec3 phys_dim; // [esp+2B4h] [ebp-80h] BYREF
  float v94; // [esp+2C4h] [ebp-70h]
  float v95; // [esp+2C8h] [ebp-6Ch]
  float dim[3]; // [esp+2CCh] [ebp-68h] BYREF
  float com[3]; // [esp+2D8h] [ebp-5Ch] BYREF
  float v98; // [esp+2E4h] [ebp-50h]
  float v99; // [esp+2E8h] [ebp-4Ch]
  float vmax[3]; // [esp+2ECh] [ebp-48h] BYREF
  float vmin[3]; // [esp+2F8h] [ebp-3Ch] BYREF
  phys_vec3 gjk_geom_list_aabb_mn_loc; // [esp+304h] [ebp-30h] BYREF
  NitrousVehicle *v103; // [esp+324h] [ebp-10h]
  unsigned int v104[2]; // [esp+328h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+334h] [ebp+0h]

  v104[0] = a2;
  v104[1] = retaddr;
  v103 = this;
  if ( params )
    v103->m_parameter = params;
  if ( phys_heavyTankSwitch->current.enabled )
  {
    if ( v103->m_parameter->m_susp_damp_k == S_T34_DAMP )
    {
      v103->m_parameter->m_accel_max = S_HEAVY_TANK_ACCEL_0;
      v103->m_parameter->m_tilt_fakey = S_HEAVY_TILT_FAKEY_0;
    }
    else
    {
      v103->m_parameter->m_accel_max = S_HEAVY_TANK_ACCEL;
      v103->m_parameter->m_tilt_fakey = S_HEAVY_TILT_FAKEY;
    }
  }
  if ( (v103->m_flags & 1) != 0 )
  {
    NitrousVehicle::unpause_physics(v103, (int)v104);
  }
  else
  {
    if ( v103->m_parameter->m_bbox_min[0] == 0.0 )
    {
      gjk_geom_list_t::comp_aabb_loc(
        &v103->m_phys_user_data->m_gjk_geom_list,
        (int)v104,
        (phys_vec3 *)vmin,
        (phys_vec3 *)&gjk_geom_list_aabb_mn_loc.y);
      Phys_NitrousVecToVec3((const phys_vec3 *)vmin, vmax);
      Phys_NitrousVecToVec3((phys_vec3 *)&gjk_geom_list_aabb_mn_loc.y, &com[2]);
    }
    else
    {
      LODWORD(com[1]) = v103->m_parameter->m_bbox_min;
      vmax[0] = *(float *)LODWORD(com[1]);
      vmax[1] = *(float *)(LODWORD(com[1]) + 4);
      vmax[2] = *(float *)(LODWORD(com[1]) + 8);
      m_parameter = v103->m_parameter;
      LODWORD(com[0]) = m_parameter->m_bbox_max;
      com[2] = m_parameter->m_bbox_max[0];
      v98 = m_parameter->m_bbox_max[1];
      v99 = m_parameter->m_bbox_max[2];
    }
    dim[0] = 0.5 * (float)(com[2] + vmax[0]);
    dim[1] = 0.5 * (float)(v98 + vmax[1]);
    dim[2] = 0.5 * (float)(v99 + vmax[2]);
    phys_dim.w = com[2] - vmax[0];
    v94 = v98 - vmax[1];
    v95 = v99 - vmax[2];
    Phys_Vec3ToNitrousVec(&phys_dim.w, (phys_vec3 *)v92);
    nuge::calc_box_inertia((const phys_vec3 *)v92, (phys_vec3 *)&v88, &v91);
    v87 = v103->m_parameter;
    _com.w = v87->m_body_mass / v91;
    v88 = v88 * _com.w;
    v89 = v89 * _com.w;
    v90 = v90 * _com.w;
    LODWORD(_com.z) = v103->m_parameter->m_mass_center_offset;
    dim[0] = dim[0] + *(float *)LODWORD(_com.z);
    dim[1] = dim[1] + *(float *)(LODWORD(_com.z) + 4);
    dim[2] = dim[2] + *(float *)(LODWORD(_com.z) + 8);
    Phys_Vec3ToNitrousVec(dim, (phys_vec3 *)&v83);
    v82 = v83 ^ _mask__NegFloat_;
    v81 = v84 ^ _mask__NegFloat_;
    v80 = v85 ^ _mask__NegFloat_;
    LODWORD(maxAvel) = v83 ^ _mask__NegFloat_;
    rb = (rigid_body *)(v84 ^ _mask__NegFloat_);
    v79 = v85 ^ _mask__NegFloat_;
    p_cg2rb = &v103->m_phys_user_data->cg2rb;
    phys_vec3::operator=(&p_cg2rb->w, (const phys_vec3 *)&maxAvel);
    body = (environment_rigid_body *)v103->m_phys_user_data->body;
    if ( v103->m_vehicle_info->type == 2 )
    {
      v74 = v103->m_vehicle_info->rotRate * 0.017453292;
      body->m_max_avel = v74;
    }
    else
    {
      body->m_max_avel = FLOAT_10_0;
    }
    v73 = v103->m_parameter;
    rigid_body::set_mass(body, v73->m_body_mass);
    rigid_body::set_inertia(body, (const phys_vec3 *)&v88);
    p_w = &v103->m_wheel_orig_relpo[2].w;
    v71 = &v103->m_wheel_orig_relpo[0].w;
    v70 = v103->m_wheel_orig_relpo[0].w.x - v103->m_wheel_orig_relpo[2].w.x;
    *(float *)&v69 = v103->m_wheel_orig_relpo[0].w.y - v103->m_wheel_orig_relpo[2].w.y;
    v68 = v103->m_wheel_orig_relpo[0].w.z - v103->m_wheel_orig_relpo[2].w.z;
    v65 = v70;
    i = v69;
    wheel_to_wheel_length = v68;
    v64 = Abs(&v65);
    for ( hasWheel = 0; hasWheel < 6; ++hasWheel )
    {
      rbc_wheel = (rigid_body_constraint_wheel *)LODWORD(v103->m_wheel_orig_relpo[hasWheel].w.y);
      v61 = *(float *)&rbc_wheel != 0.0;
      v60 = v61;
      if ( *(float *)&rbc_wheel == 0.0 )
      {
        v103->m_wheels[hasWheel] = 0;
      }
      else
      {
        translation.w = 0.0;
        if ( initialization )
        {
          LODWORD(translation.w) = phys_sys::create_rbc_wheel(body, 0, 0);
          if ( !LODWORD(translation.w)
            && !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
                  497,
                  0,
                  "%s\n\t%s",
                  "rbc_wheel",
                  "Ran out of wheel constraints.") )
          {
            __debugbreak();
          }
          v103->m_wheels[hasWheel] = (rigid_body_constraint_wheel *)LODWORD(translation.w);
        }
        else
        {
          LODWORD(translation.w) = v103->m_wheels[hasWheel];
        }
        v5 = &v103->m_phys_user_data->cg2rb.w;
        x = v5->x;
        y = v5->y;
        z = v5->z;
        translation.x = v5->w;
        v55 = &v103->m_wheel_orig_relpo[hasWheel].w;
        v54 = v55->x + x;
        v53 = v103->m_wheel_orig_relpo[hasWheel].w.y + y;
        v52 = v103->m_wheel_orig_relpo[hasWheel].w.z + z;
        wheel_center_loc.y = v54;
        wheel_center_loc.z = v53;
        wheel_center_loc.w = v52;
        *(float *)v49 = v54;
        *(float *)&v49[1] = v53;
        v50 = v52;
        dist_to_com = v52;
        com_spring_factor = (float)(v52 - v103->m_parameter->m_susp_adj) + v103->m_parameter->m_wheel_radius;
        v50 = com_spring_factor;
        v46 = v54;
        v45 = LODWORD(v54) & _mask__AbsFloat_;
        v44 = COERCE_FLOAT(LODWORD(v54) & _mask__AbsFloat_) / v64;
        if ( (float)(v44 - 1.0) < 0.0 )
          mass = v44;
        else
          mass = FLOAT_1_0;
        if ( (float)(0.0 - v44) < 0.0 )
          susp_spring_k = mass;
        else
          susp_spring_k = *(float *)&FLOAT_0_0;
        v44 = (float)(1.0 - susp_spring_k) * 2.0;
        susp_damp_k = v103->m_parameter;
        m_body_mass = susp_damp_k->m_body_mass;
        susp_dir.w = (float)((float)(v103->m_parameter->m_susp_spring_k * phys_vehicleGravityMultiplier->current.value)
                           * v44)
                   * m_body_mass;
        susp_dir.z = (float)((float)(v103->m_parameter->m_susp_damp_k * phys_vehicleGravityMultiplier->current.value)
                           * v44)
                   * m_body_mass;
        v37[0] = *(unsigned int *)&FLOAT_0_0;
        v37[1] = *(unsigned int *)&FLOAT_0_0;
        wheel_radius = FLOAT_N1_0;
        m_wheel_radius = v103->m_parameter->m_wheel_radius;
        if ( v103->m_parameter->m_tire_fric_side_max == 0.0 )
          side_fric_max = FLOAT_3_4028235e38;
        else
          side_fric_max = v103->m_parameter->m_tire_fric_side_max;
        rigid_body_constraint_wheel::set(
          (rigid_body_constraint_wheel *)LODWORD(translation.w),
          (const phys_vec3 *)v49,
          (const phys_vec3 *)v37,
          &gWheelAxisLoc,
          m_wheel_radius,
          v103->m_parameter->m_tire_fric_fwd / phys_vehicleGravityMultiplier->current.value,
          v103->m_parameter->m_tire_fric_side / phys_vehicleGravityMultiplier->current.value,
          susp_dir.w,
          susp_dir.z,
          v103->m_parameter->m_susp_hard_limit,
          v103->m_parameter->m_roll_stability,
          v103->m_parameter->m_roll_stability,
          side_fric_max);
        if ( v103->m_vehicle_info->type == 2 || v103->m_vehicle_info->isFourWheelSteering )
        {
          *(unsigned int *)(LODWORD(translation.w) + 176) |= 8u;
          v34 = *(unsigned int *)(LODWORD(translation.w) + 176);
        }
        else
        {
          if ( (unsigned int)hasWheel < 2 || hasWheel == 4 || hasWheel == 5 )
            *(unsigned int *)(LODWORD(translation.w) + 176) |= 8u;
          else
            *(unsigned int *)(LODWORD(translation.w) + 176) &= ~8u;
          v32 = *(unsigned int *)(LODWORD(translation.w) + 176);
        }
        *(unsigned int *)(LODWORD(translation.w) + 176) &= ~0x80u;
        v31 = *(unsigned int *)(LODWORD(translation.w) + 176);
        if ( hasWheel == 2 || hasWheel == 3 )
          *(unsigned int *)(LODWORD(translation.w) + 176) |= 0x20u;
        else
          *(unsigned int *)(LODWORD(translation.w) + 176) &= ~0x20u;
        v29 = *(unsigned int *)(LODWORD(translation.w) + 176);
        *(unsigned int *)(LODWORD(translation.w) + 176) |= 0x40u;
        v28 = *(unsigned int *)(LODWORD(translation.w) + 176);
        m_traction_type = v103->m_parameter->m_traction_type;
        if ( m_traction_type )
        {
          if ( m_traction_type == TRACTION_TYPE_BACK )
          {
            if ( hasWheel == 2 || hasWheel == 3 )
              *(unsigned int *)(LODWORD(translation.w) + 176) |= 0x10u;
            else
              *(unsigned int *)(LODWORD(translation.w) + 176) &= ~0x10u;
            v23 = *(unsigned int *)(LODWORD(translation.w) + 176);
          }
          else if ( m_traction_type == TRACTION_TYPE_ALL_WD )
          {
            *(unsigned int *)(LODWORD(translation.w) + 176) |= 0x10u;
            v22 = *(unsigned int *)(LODWORD(translation.w) + 176);
          }
        }
        else
        {
          v26 = (unsigned int)hasWheel < 2;
          if ( (unsigned int)hasWheel >= 2 )
            *(unsigned int *)(LODWORD(translation.w) + 176) &= ~0x10u;
          else
            *(unsigned int *)(LODWORD(translation.w) + 176) |= 0x10u;
          v25 = *(unsigned int *)(LODWORD(translation.w) + 176);
        }
      }
    }
    if ( v103->m_vehicle_info->type == 4 )
    {
      front_steer_pt.y = FLOAT_50_0;
      front_steer_pt.z = *(float *)&FLOAT_0_0;
      front_steer_pt.w = *(float *)&FLOAT_0_0;
      v18 = FLOAT_50_0;
      v19 = *(float *)&FLOAT_0_0;
      front_back_length = *(float *)&FLOAT_0_0;
      v17 = FLOAT_100_0;
    }
    else
    {
      v16 = v103->m_wheels[1];
      v15 = v103->m_wheels[0];
      v13 = v15->m_b1_wheel_center_loc.y + v16->m_b1_wheel_center_loc.y;
      v14 = v15->m_b1_wheel_center_loc.z + v16->m_b1_wheel_center_loc.z;
      v18 = (float)(v15->m_b1_wheel_center_loc.x + v16->m_b1_wheel_center_loc.x) * 0.5;
      v19 = v13 * 0.5;
      front_back_length = v14 * 0.5;
      v17 = v18
          - (float)((float)(v103->m_wheels[2]->m_b1_wheel_center_loc.x + v103->m_wheels[3]->m_b1_wheel_center_loc.x)
                  * 0.5);
    }
    maximum_acceleration = NitrousVehicle::get_maximum_acceleration(v103);
    v12 = maximum_acceleration * v103->m_parameter->m_body_mass;
    roll_stability_factor = v103->m_parameter->m_steer_speed;
    hard_limit_dist = v103->m_parameter->m_steer_angle_max;
    suspension_damp_k = v103->m_parameter->m_wheel_radius;
    suspension_stiffness_k = v103->m_parameter->m_tire_damp_coast * v103->m_parameter->m_body_mass;
    fwd_fric_k = NitrousVehicle::get_max_speed(v103, 1);
    NitrousVehicle::set(
      v103,
      v103->m_parameter->m_tire_damp_hand * v103->m_parameter->m_body_mass,
      v103->m_parameter->m_tire_damp_brake * v103->m_parameter->m_body_mass,
      fwd_fric_k,
      v12,
      suspension_stiffness_k,
      suspension_damp_k,
      hard_limit_dist,
      roll_stability_factor,
      (phys_vec3 *)&v18,
      v17);
    if ( v103->m_orientation_constraint )
    {
      v103->m_orientation_constraint->m_torque_resistance_pitch_roll = v103->m_parameter->m_pitch_roll_resistance
                                                                     * v103->m_parameter->m_body_mass;
      v103->m_orientation_constraint->m_torque_resistance_yaw = v103->m_parameter->m_yaw_resistance
                                                              * v103->m_parameter->m_body_mass;
      v103->m_orientation_constraint->m_upright_strength = v103->m_parameter->m_upright_strength
                                                         * v103->m_parameter->m_body_mass;
      NitrousVehicle::_update_orientation_constraint(v103);
    }
  }
}

// bad sp value at call has been detected, the output may be wrong!
void __userpurge gjk_geom_list_t::comp_aabb_loc(
        gjk_geom_list_t *this@<ecx>,
        int a2@<ebp>,
        phys_vec3 *aabb_mn_loc,
        phys_vec3 *aabb_mx_loc)
{
  const phys_vec3 *v4; // [esp-6Ch] [ebp-B0h]
  phys_vec3 v5; // [esp-68h] [ebp-ACh] BYREF
  phys_vec3 m_aabb_mx_loc; // [esp-58h] [ebp-9Ch] BYREF
  const phys_vec3 *v7; // [esp-3Ch] [ebp-80h]
  phys_vec3 v8; // [esp-38h] [ebp-7Ch] BYREF
  phys_vec3 m_aabb_mn_loc; // [esp-28h] [ebp-6Ch] BYREF
  gjk_base_t *i; // [esp-Ch] [ebp-50h]
  float x; // [esp-8h] [ebp-4Ch]
  float y; // [esp-4h] [ebp-48h]
  gjk_base_t *geom; // [esp+0h] [ebp-44h]
  float w; // [esp+4h] [ebp-40h]
  gjk_base_t *v15; // [esp+14h] [ebp-30h]
  float v16; // [esp+18h] [ebp-2Ch]
  float v17; // [esp+1Ch] [ebp-28h]
  float v18; // [esp+20h] [ebp-24h]
  int v19; // [esp+24h] [ebp-20h]
  float *m_first_geom; // [esp+30h] [ebp-14h]
  gjk_geom_list_t *v21; // [esp+34h] [ebp-10h]
  int v22; // [esp+38h] [ebp-Ch]
  void *v23; // [esp+3Ch] [ebp-8h]
  void *retaddr; // [esp+44h] [ebp+0h]

  v22 = a2;
  v23 = retaddr;
  v21 = this;
  if ( !this->m_first_geom
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\src\\vehicle\\../physics/phys_colgeom.h",
         1040,
         "m_first_geom",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  v21->m_first_geom->comp_aabb_loc(v21->m_first_geom);
  m_first_geom = (float *)v21->m_first_geom;
  if ( ((unsigned int)m_first_geom[15] & 2) == 0
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\src\\vehicle\\../physics/phys_colgeom.h",
         82,
         "get_flag(FLAG_AABB_LOC_VALID)",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  v16 = m_first_geom[4];
  v17 = m_first_geom[5];
  v18 = m_first_geom[6];
  v19 = *((unsigned int *)m_first_geom + 7);
  aabb_mn_loc->x = v16;
  aabb_mn_loc->y = v17;
  aabb_mn_loc->z = v18;
  v15 = v21->m_first_geom;
  if ( (v15->m_flags & 2) == 0
    && _tlAssert(
         "c:\\projects_pc\\cod\\codsrc\\src\\vehicle\\../physics/phys_colgeom.h",
         83,
         "get_flag(FLAG_AABB_LOC_VALID)",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  x = v15->m_aabb_mx_loc.x;
  y = v15->m_aabb_mx_loc.y;
  geom = (gjk_base_t *)LODWORD(v15->m_aabb_mx_loc.z);
  w = v15->m_aabb_mx_loc.w;
  aabb_mx_loc->x = x;
  aabb_mx_loc->y = y;
  LODWORD(aabb_mx_loc->z) = geom;
  for ( i = v21->m_first_geom->m_next_geom; i; i = i->m_next_geom )
  {
    i->comp_aabb_loc(i);
    if ( (i->m_flags & 2) == 0
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\src\\vehicle\\../physics/phys_colgeom.h",
           82,
           "get_flag(FLAG_AABB_LOC_VALID)",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    m_aabb_mn_loc = i->m_aabb_mn_loc;
    v7 = phys_min(&v8, aabb_mn_loc, &m_aabb_mn_loc);
    aabb_mn_loc->x = v7->x;
    aabb_mn_loc->y = v7->y;
    aabb_mn_loc->z = v7->z;
    if ( (i->m_flags & 2) == 0 )
    {
      if ( _tlAssert(
             "c:\\projects_pc\\cod\\codsrc\\src\\vehicle\\../physics/phys_colgeom.h",
             83,
             "get_flag(FLAG_AABB_LOC_VALID)",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
    }
    m_aabb_mx_loc = i->m_aabb_mx_loc;
    v4 = phys_max(&v5, aabb_mx_loc, &m_aabb_mx_loc);
    aabb_mx_loc->x = v4->x;
    aabb_mx_loc->y = v4->y;
    aabb_mx_loc->z = v4->z;
  }
}

// local variable allocation has failed, the output may be wrong!
void __userpurge NitrousVehicle::_setup_wheels(NitrousVehicle *this@<ecx>, DObj *a2@<ebp>, int localClientNum)
{
  const char *v3; // eax
  unsigned int *p_x; // [esp+24h] [ebp-220h]
  int i; // [esp+40h] [ebp-204h]
  phys_vec3 *v6; // [esp+44h] [ebp-200h]
  phys_vec3 *v7; // [esp+64h] [ebp-1E0h]
  phys_vec3 *v8; // [esp+84h] [ebp-1C0h]
  float v9; // [esp+8Ch] [ebp-1B8h]
  phys_vec3 *v10; // [esp+C4h] [ebp-180h]
  float v11; // [esp+C8h] [ebp-17Ch]
  float v12; // [esp+CCh] [ebp-178h]
  float v13; // [esp+D0h] [ebp-174h]
  float v14; // [esp+10Ch] [ebp-138h]
  phys_vec3 *v15; // [esp+124h] [ebp-120h]
  float v16; // [esp+12Ch] [ebp-118h]
  phys_vec3 *p_w; // [esp+164h] [ebp-E0h]
  float v18; // [esp+168h] [ebp-DCh]
  float v19; // [esp+16Ch] [ebp-D8h]
  float v20; // [esp+170h] [ebp-D4h]
  float y; // [esp+1ACh] [ebp-98h]
  float v22[3]; // [esp+1D0h] [ebp-74h] BYREF
  float mat34[4][3]; // [esp+1DCh] [ebp-68h] BYREF
  DObjAnimMat mat; // [esp+20Ch] [ebp-38h]
  unsigned int backWheel; // [esp+22Ch] [ebp-18h]
  unsigned int frontWheel; // [esp+230h] [ebp-14h] BYREF
  NitrousVehicle *v27; // [esp+234h] [ebp-10h]
  DObj *obj; // [esp+238h] [ebp-Ch]
  void *v29; // [esp+23Ch] [ebp-8h] OVERLAPPED
  void *retaddr; // [esp+244h] [ebp+0h]

  obj = a2;
  v29 = retaddr;
  v27 = this;
  HIBYTE(frontWheel) = 0;
  backWheel = 0;
  if ( !this->m_xmodel )
  {
    v3 = va(
           "NitrousVehicle:  Trying to setup wheels on vehicle with invalid model.  Vehicle %s at (%g %g %g).\n",
           v27->m_vehicle_info->name,
           v27->m_origin[0],
           v27->m_origin[1],
           v27->m_origin[2]);
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
            631,
            0,
            "%s\n\t%s",
            "m_xmodel",
            v3) )
      __debugbreak();
  }
  if ( v27->m_owner )
  {
    LODWORD(mat.transWeight) = v27->m_owner;
    backWheel = (unsigned int)Com_GetServerDObj(*(unsigned int *)LODWORD(mat.transWeight));
  }
  else
  {
    backWheel = (unsigned int)Com_GetClientDObj(v27->m_entnum, localClientNum);
  }
  if ( v27->m_vehicle_info->type == 1 )
  {
    LODWORD(mat.trans[2]) = SL_FindLowercaseString("tag_wheel_front", SCRIPTINSTANCE_SERVER);
    LODWORD(mat.trans[1]) = SL_FindLowercaseString("tag_wheel_back", SCRIPTINSTANCE_SERVER);
    if ( DObjGetBoneIndex((const DObj *)backWheel, LODWORD(mat.trans[2]), (unsigned __int8 *)&frontWheel + 3, -1) )
    {
      DObjGetBasePoseMatrix((DObj *)backWheel, HIBYTE(frontWheel), (DObjAnimMat *)mat34[3]);
      Ragdoll_AnimMatToMat43((const DObjAnimMat *)mat34[3], (float (*)[3])v22);
      if ( ((LODWORD(mat34[2][0]) & 0x7F800000) == 0x7F800000
         || (LODWORD(mat34[2][1]) & 0x7F800000) == 0x7F800000
         || (LODWORD(mat34[2][2]) & 0x7F800000) == 0x7F800000)
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
              653,
              0,
              "%s",
              "!IS_NAN((mat34[3])[0]) && !IS_NAN((mat34[3])[1]) && !IS_NAN((mat34[3])[2])") )
      {
        __debugbreak();
      }
      Phys_Mat4ToNitrousMat((float (*)[3])v22, v27->m_wheel_orig_relpo);
      phys_mat44::operator=(&v27->m_wheel_orig_relpo[1], v27->m_wheel_orig_relpo);
      y = v27->m_wheel_orig_relpo[0].w.y;
      v18 = v27->m_wheel_orig_relpo[0].w.x + 0.0;
      v19 = y + COERCE_FLOAT(LODWORD(wheelSeperation) ^ _mask__NegFloat_);
      v20 = v27->m_wheel_orig_relpo[0].w.z + 0.0;
      p_w = &v27->m_wheel_orig_relpo[0].w;
      v27->m_wheel_orig_relpo[0].w.x = v18;
      p_w->y = v19;
      p_w->z = v20;
      v16 = y + wheelSeperation;
      v15 = &v27->m_wheel_orig_relpo[1].w;
      v27->m_wheel_orig_relpo[1].w.x = v18;
      v15->y = v16;
      v15->z = v20;
    }
    else if ( !Assert_MyHandler(
                 "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
                 662,
                 0,
                 "Failed to find tag_wheel_front for motorcycle.") )
    {
      __debugbreak();
    }
    if ( DObjGetBoneIndex((const DObj *)backWheel, LODWORD(mat.trans[1]), (unsigned __int8 *)&frontWheel + 3, -1) )
    {
      DObjGetBasePoseMatrix((DObj *)backWheel, HIBYTE(frontWheel), (DObjAnimMat *)mat34[3]);
      Ragdoll_AnimMatToMat43((const DObjAnimMat *)mat34[3], (float (*)[3])v22);
      if ( ((LODWORD(mat34[2][0]) & 0x7F800000) == 0x7F800000
         || (LODWORD(mat34[2][1]) & 0x7F800000) == 0x7F800000
         || (LODWORD(mat34[2][2]) & 0x7F800000) == 0x7F800000)
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
              669,
              0,
              "%s",
              "!IS_NAN((mat34[3])[0]) && !IS_NAN((mat34[3])[1]) && !IS_NAN((mat34[3])[2])") )
      {
        __debugbreak();
      }
      Phys_Mat4ToNitrousMat((float (*)[3])v22, &v27->m_wheel_orig_relpo[2]);
      phys_mat44::operator=(&v27->m_wheel_orig_relpo[3], &v27->m_wheel_orig_relpo[2]);
      v14 = v27->m_wheel_orig_relpo[2].w.y;
      v11 = v27->m_wheel_orig_relpo[2].w.x + 0.0;
      v12 = v14 + COERCE_FLOAT(LODWORD(wheelSeperation) ^ _mask__NegFloat_);
      v13 = v27->m_wheel_orig_relpo[2].w.z + 0.0;
      v10 = &v27->m_wheel_orig_relpo[2].w;
      v27->m_wheel_orig_relpo[2].w.x = v11;
      v10->y = v12;
      v10->z = v13;
      v9 = v14 + wheelSeperation;
      v8 = &v27->m_wheel_orig_relpo[3].w;
      v27->m_wheel_orig_relpo[3].w.x = v11;
      v8->y = v9;
      v8->z = v13;
    }
    else if ( !Assert_MyHandler(
                 "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
                 678,
                 0,
                 "Failed to find tag_wheel_back for motorcycle.") )
    {
      __debugbreak();
    }
    v7 = &v27->m_wheel_orig_relpo[4].w;
    v27->m_wheel_orig_relpo[4].w.x = *(float *)&FLOAT_0_0;
    v7->y = *(float *)&FLOAT_0_0;
    v7->z = *(float *)&FLOAT_0_0;
    v6 = &v27->m_wheel_orig_relpo[5].w;
    v27->m_wheel_orig_relpo[5].w.x = *(float *)&FLOAT_0_0;
    v6->y = *(float *)&FLOAT_0_0;
    v6->z = *(float *)&FLOAT_0_0;
  }
  else
  {
    for ( i = 0; i < 6; ++i )
    {
      HIBYTE(frontWheel) = 0;
      if ( DObjGetBoneIndex((const DObj *)backWheel, *s_wheelTags[i], (unsigned __int8 *)&frontWheel + 3, -1) )
      {
        DObjGetBasePoseMatrix((DObj *)backWheel, HIBYTE(frontWheel), (DObjAnimMat *)mat34[3]);
        Ragdoll_AnimMatToMat43((const DObjAnimMat *)mat34[3], (float (*)[3])v22);
        Phys_Mat4ToNitrousMat((float (*)[3])v22, &v27->m_wheel_orig_relpo[i]);
      }
      else
      {
        p_x = (unsigned int *)&v27->m_wheel_orig_relpo[i].w.x;
        *p_x = *(unsigned int *)&FLOAT_0_0;
        p_x[1] = *(unsigned int *)&FLOAT_0_0;
        p_x[2] = *(unsigned int *)&FLOAT_0_0;
      }
      v27->m_wheel_damage[i] = 0;
    }
  }
}

void __cdecl Phys_Mat4ToNitrousMat(float (*inMat)[3], phys_mat44 *outMat)
{
  Phys_Vec3ToNitrousVec((float *)inMat, &outMat->x);
  Phys_Vec3ToNitrousVec(&(*inMat)[3], &outMat->y);
  Phys_Vec3ToNitrousVec(&(*inMat)[6], &outMat->z);
  Phys_Vec3ToNitrousVec(&(*inMat)[9], &outMat->w);
}

double __thiscall NitrousVehicle::_calc_initial_susp_spring_k(
        NitrousVehicle *this,
        rigid_body_constraint_wheel *wheel_constraint)
{
  float v3; // [esp-8h] [ebp-40h]
  float v4; // [esp-4h] [ebp-3Ch]
  float susp_spring_k; // [esp+0h] [ebp-38h]

  susp_spring_k = COERCE_FLOAT(LODWORD(wheel_constraint->m_b1_wheel_center_loc.x) & _mask__AbsFloat_)
                / this->m_steer_front_back_length;
  if ( (float)(susp_spring_k - 1.0) < 0.0 )
    v4 = COERCE_FLOAT(LODWORD(wheel_constraint->m_b1_wheel_center_loc.x) & _mask__AbsFloat_)
       / this->m_steer_front_back_length;
  else
    v4 = FLOAT_1_0;
  if ( (float)(0.0 - susp_spring_k) < 0.0 )
    v3 = v4;
  else
    v3 = *(float *)&FLOAT_0_0;
  return (float)((float)((float)(this->m_parameter->m_susp_spring_k * phys_vehicleGravityMultiplier->current.value)
                       * (float)((float)(1.0 - v3) * 2.0))
               * this->m_parameter->m_body_mass);
}

bool __thiscall NitrousVehicle::is_peeling_out(NitrousVehicle *this)
{
  return this->m_throttle > 0.89999998 && this->m_parameter->m_peel_out_max_speed > this->m_forward_vel;
}

void __thiscall NitrousVehicle::destructible_damage(NitrousVehicle *this, const char *notifyLabel, int damageStage)
{
  int j; // [esp+10h] [ebp-10h]
  int i; // [esp+14h] [ebp-Ch]
  int max; // [esp+18h] [ebp-8h]
  int min; // [esp+1Ch] [ebp-4h]

  min = 0;
  max = -1;
  if ( notifyLabel )
  {
    if ( !s_tankWheelJnts[0] )
    {
      s_tankWheelJnts[0] = scr_const.tag_wheel_front_left;
      dword_9BE57A4 = scr_const.tag_wheel_middle_left;
      dword_9BE57A8 = scr_const.tag_wheel_back_left;
      dword_9BE57AC = scr_const.tag_wheel_front_right;
      dword_9BE57B0 = scr_const.tag_wheel_middle_right;
      dword_9BE57B4 = scr_const.tag_wheel_back_right;
    }
    if ( I_strcmp(notifyLabel, "left_tread_destroyed") )
    {
      if ( !I_strcmp(notifyLabel, "right_tread_destroyed") )
      {
        min = 3;
        max = 6;
        this->m_speed_scale = *(float *)&FLOAT_0_0;
      }
    }
    else
    {
      min = 0;
      max = 3;
      this->m_speed_scale = *(float *)&FLOAT_0_0;
    }
    if ( this->m_speed_scale < 0.0 )
      this->m_speed_scale = FLOAT_1_0;
    for ( i = min; i < max; ++i )
    {
      for ( j = 0; j < 6; ++j )
      {
        if ( s_tankWheelJnts[i] == *s_wheelTags[j] )
          this->m_wheels[j]->m_wheel_radius = this->m_parameter->m_wheel_radius * tempRadiusPercent;
      }
    }
  }
}

void __thiscall NitrousVehicle::damage(NitrousVehicle *this, unsigned int boneName, int damageStage)
{
  ;
}

void __thiscall NitrousVehicle::damage(NitrousVehicle *this, int damage, const float *point, const float *dir, int mod)
{
  gentity_s *m_owner; // edx
  gentity_s *tempent; // [esp+34h] [ebp-18h]
  float force[3]; // [esp+38h] [ebp-14h] BYREF
  float forcescale; // [esp+44h] [ebp-8h]
  bool doForce; // [esp+4Bh] [ebp-1h]

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
      tempent = G_TempEntity(force, 75);
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

phys_vec3 *__thiscall NitrousVehicle::get_velocity(NitrousVehicle *this, phys_vec3 *result)
{
  rigid_body *body; // ecx
  float v5; // [esp+10h] [ebp-8h]
  float v6; // [esp+14h] [ebp-4h]

  if ( this->m_phys_user_data )
  {
    body = this->m_phys_user_data->body;
    v5 = body->m_t_vel.y - this->m_ground_vel.y;
    v6 = body->m_t_vel.z - this->m_ground_vel.z;
    result->x = body->m_t_vel.x - this->m_ground_vel.x;
    result->y = v5;
    result->z = v6;
  }
  else
  {
    result->x = *(float *)&FLOAT_0_0;
    result->y = *(float *)&FLOAT_0_0;
    result->z = *(float *)&FLOAT_0_0;
  }
  return result;
}

phys_vec3 *__thiscall NitrousVehicle::get_a_velocity(NitrousVehicle *this, phys_vec3 *result)
{
  if ( this->m_phys_user_data )
  {
    *result = this->m_phys_user_data->body->m_a_vel;
  }
  else
  {
    result->x = *(float *)&FLOAT_0_0;
    result->y = *(float *)&FLOAT_0_0;
    result->z = *(float *)&FLOAT_0_0;
  }
  return result;
}

void __thiscall NitrousVehicle::_update_orientation_constraint(NitrousVehicle *this)
{
  bool v1; // [esp+2h] [ebp-36h]
  bool v2; // [esp+3h] [ebp-35h]
  rigid_body_constraint_wheel *rbc_wheel; // [esp+2Ch] [ebp-Ch]
  int i; // [esp+30h] [ebp-8h]

  if ( !this->m_orientation_constraint
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
         880,
         "m_orientation_constraint",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( !this->m_phys_user_data
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
         881,
         "m_phys_user_data",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  if ( !this->m_phys_user_data->body
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
         882,
         "m_phys_user_data->body",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  v2 = ((this->m_flags & 4) != 0 || (this->m_flags & 0x40) != 0 || (this->m_flags & 0x100) != 0)
    && (this->m_flags & 0x200) == 0
    || (this->m_flags & 0x80) != 0;
  v1 = !this->m_num_colliding_wheels && !this->m_phys_user_data->body->m_contact_count;
  this->m_orientation_constraint->m_active = v2;
  this->m_orientation_constraint->m_no_orientation_correction = v1;
  if ( (this->m_flags & 0x1000) != 0 )
  {
    this->m_orientation_constraint->m_active = 0;
    this->m_orientation_constraint->m_torque_resistance_pitch_roll = *(float *)&FLOAT_0_0;
    this->m_orientation_constraint->m_torque_resistance_yaw = *(float *)&FLOAT_0_0;
    this->m_orientation_constraint->m_upright_strength = *(float *)&FLOAT_0_0;
    this->m_orientation_constraint->m_no_orientation_correction = 1;
    for ( i = 0; i < 6; ++i )
    {
      rbc_wheel = this->m_wheels[i];
      if ( rbc_wheel )
        rbc_wheel->m_roll_stability_factor = *(float *)&FLOAT_0_0;
    }
  }
}

double __thiscall NitrousVehicle::_get_stuck_accel_factor(NitrousVehicle *this, float delta_t)
{
  float v4; // [esp+8h] [ebp-1Ch]
  float accel_scale; // [esp+1Ch] [ebp-8h]
  float abs_steer_factor; // [esp+20h] [ebp-4h]

  if ( !this->m_phys_user_data )
    return 1.0;
  if ( this->m_throttle <= 0.2 )
  {
    this->m_stuck_time = *(float *)&FLOAT_0_0;
  }
  else if ( this->m_phys_user_data )
  {
    LODWORD(abs_steer_factor) = LODWORD(this->m_steer_factor) & _mask__AbsFloat_;
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
        this->m_stuck_time = *(float *)&FLOAT_0_0;
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
    v4 = (float)((float)(this->m_stuck_time - 0.40000001) * 30.0) + 1.0;
  else
    v4 = FLOAT_40_0;
  if ( (float)(1.0 - accel_scale) < 0.0 )
    return v4;
  else
    return FLOAT_1_0;
}

void __thiscall NitrousVehicle::_update_boost(NitrousVehicle *this, float delta_t)
{
  float boostDuration; // [esp+0h] [ebp-40h]
  float m_boost_time_pool; // [esp+4h] [ebp-3Ch]
  bool v4; // [esp+Ch] [ebp-34h]
  bool v5; // [esp+10h] [ebp-30h]
  bool tryingToBoost; // [esp+3Ah] [ebp-6h]
  bool wasBoosting; // [esp+3Dh] [ebp-3h]

  tryingToBoost = bitarray<51>::testBit(&this->mVehicleController.m_cmd.button_bits, 0x2Bu);
  wasBoosting = (this->m_flags & 0x800) != 0;
  if ( (this->m_flags & 0x800) != 0 )
    this->m_boost_time_pool = this->m_boost_time_pool - delta_t;
  if ( this->m_boost_start_time )
    v5 = this->mVehicleController.m_cmd.serverTime < this->m_boost_start_time
                                                   + (int)(float)(this->m_vehicle_info->boostInterval * 1000.0);
  else
    v5 = 0;
  if ( this->m_boost_start_time )
    v4 = this->mVehicleController.m_cmd.serverTime < this->m_boost_start_time
                                                   + (int)(float)(this->m_vehicle_info->boostContinuousRegenCooldown
                                                                * 1000.0);
  else
    v4 = 1;
  if ( this->m_vehicle_info->boostMode )
  {
    if ( this->m_vehicle_info->boostMode != 1 )
      goto LABEL_37;
    if ( !tryingToBoost || this->m_boost_time_pool <= 0.0 )
    {
      if ( !tryingToBoost && !v4 && this->m_vehicle_info->boostDuration > this->m_boost_time_pool )
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
    NitrousVehicle::set_boosting(this, 1);
    return;
  }
  if ( this->m_vehicle_info->boostInterval > this->m_vehicle_info->boostDuration )
  {
    if ( wasBoosting && this->m_boost_time_pool > 0.0 )
    {
      NitrousVehicle::set_boosting(this, wasBoosting);
      return;
    }
    if ( !tryingToBoost || this->m_boost_time_pool <= 0.0 || v5 )
    {
      if ( v5 && this->m_vehicle_info->boostDuration > this->m_boost_time_pool )
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
  NitrousVehicle::set_boosting(this, 0);
}

void __userpurge NitrousVehicle::_update_prolog(NitrousVehicle *this@<ecx>, int a2@<ebp>, float delta_t)
{
  const VehicleParameter *m_parameter; // esi
  const VehicleParameter *v4; // esi
  double Value; // st7
  float v6; // xmm0_4
  rigid_body *body; // [esp+58h] [ebp-E8h]
  float v8; // [esp+5Ch] [ebp-E4h]
  float maximum_acceleration_factor; // [esp+88h] [ebp-B8h]
  float fraction; // [esp+9Ch] [ebp-A4h]
  float v11; // [esp+A4h] [ebp-9Ch]
  float v12; // [esp+ACh] [ebp-94h]
  float speed; // [esp+B0h] [ebp-90h]
  float v14; // [esp+B4h] [ebp-8Ch]
  float v15; // [esp+C0h] [ebp-80h]
  float v16; // [esp+C4h] [ebp-7Ch]
  float v17; // [esp+C8h] [ebp-78h]
  float v18; // [esp+D0h] [ebp-70h]
  float v19; // [esp+D4h] [ebp-6Ch]
  float v20; // [esp+DCh] [ebp-64h]
  float v21; // [esp+E0h] [ebp-60h]
  float z; // [esp+E8h] [ebp-58h]
  int v23; // [esp+100h] [ebp-40h]
  rigid_body_constraint_wheel *accel_factor; // [esp+104h] [ebp-3Ch]
  int j; // [esp+108h] [ebp-38h]
  int v26; // [esp+10Ch] [ebp-34h]
  float rbc_wheela; // [esp+110h] [ebp-30h]
  float rbc_wheel; // [esp+110h] [ebp-30h]
  int i; // [esp+114h] [ebp-2Ch]
  float ib; // [esp+114h] [ebp-2Ch]
  int ia; // [esp+114h] [ebp-2Ch]
  unsigned int v33[2]; // [esp+134h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+140h] [ebp+0h]

  v33[0] = a2;
  v33[1] = retaddr;
  NitrousVehicle::_update_pause(this);
  this->m_notify_flags = 0;
  NitrousVehicleController::UpdateControls(&this->mVehicleController, COERCE_FLOAT(v33), this, delta_t);
  NitrousVehicle::_update_boost(this, delta_t);
  if ( this->m_throttle <= 0.0 )
    this->m_state_flags &= ~4u;
  else
    this->m_state_flags |= 4u;
  if ( this->m_hand_brake <= 0.0 )
    this->m_state_flags &= ~1u;
  else
    this->m_state_flags |= 1u;
  m_parameter = this->m_parameter;
  *(float *)&i = NitrousVehicle::get_maximum_acceleration(this) * m_parameter->m_body_mass;
  rbc_wheela = NitrousVehicle::get_max_speed(this, 1);
  rbc_wheel = rbc_wheela * this->m_speed_scale;
  if ( this->m_vehicle_info->type == 2 )
  {
    v26 = 0;
    for ( j = 0; j < 6; ++j )
    {
      accel_factor = this->m_wheels[j];
      if ( accel_factor )
      {
        if ( (accel_factor->m_wheel_flags & 1) != 0 )
          ++v26;
      }
    }
    if ( v26 <= 1 )
      v23 = 2;
    else
      v23 = v26;
    ib = *(float *)&i * (float)(COERCE_FLOAT(LODWORD(this->m_throttle) & _mask__AbsFloat_) * (float)(6.0 / (float)v23));
    *(float *)&ia = NitrousVehicle::_get_stuck_accel_factor(this, delta_t) * ib;
  }
  else
  {
    *(float *)&ia = COERCE_FLOAT(LODWORD(this->m_throttle) & _mask__AbsFloat_) * *(float *)&i;
    if ( this->m_throttle < 0.0 )
    {
      *(float *)&ia = *(float *)&ia * this->m_parameter->m_reverse_scale;
      rbc_wheel = rbc_wheel * this->m_parameter->m_reverse_scale;
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
          v19 = z + pitchFakeyZone;
        else
          v19 = *(float *)&FLOAT_0_0;
        if ( (float)(-1.0 - (float)(z + pitchFakeyZone)) < 0.0 )
          v18 = v19;
        else
          v18 = FLOAT_N1_0;
        z = v18;
      }
      else
      {
        if ( (float)((float)(z - pitchFakeyZone) - 1.0) < 0.0 )
          v21 = z - pitchFakeyZone;
        else
          v21 = FLOAT_1_0;
        if ( (float)(0.0 - (float)(z - pitchFakeyZone)) < 0.0 )
          v20 = v21;
        else
          v20 = *(float *)&FLOAT_0_0;
        z = v20;
      }
    }
    if ( (float)((float)(z / 0.40000001) - 1.0) < 0.0 )
      v17 = z / 0.40000001;
    else
      v17 = FLOAT_1_0;
    if ( (float)(-1.0 - (float)(z / 0.40000001)) < 0.0 )
      v16 = v17;
    else
      v16 = FLOAT_N1_0;
    v15 = (float)((float)(1.0 - COERCE_FLOAT(LODWORD(this->m_steer_factor) & _mask__AbsFloat_)) * this->m_throttle)
        * v16;
    if ( (float)(v15 - 1.0) < 0.0 )
      v14 = (float)((float)(1.0 - COERCE_FLOAT(LODWORD(this->m_steer_factor) & _mask__AbsFloat_)) * this->m_throttle)
          * v16;
    else
      v14 = FLOAT_1_0;
    if ( (float)(0.0 - v15) < 0.0 )
      speed = v14;
    else
      speed = *(float *)&FLOAT_0_0;
    v4 = this->m_parameter;
    v12 = NitrousVehicle::get_maximum_acceleration(this) * v4->m_body_mass;
    if ( phys_heavyTankSwitch->current.enabled )
      *(float *)&ia = (float)(speed * climbHeavyAccelAdd) + *(float *)&ia;
    *(float *)&ia = (float)(speed * v12) + *(float *)&ia;
  }
  if ( (this->m_flags & 1) == 0 )
  {
    LODWORD(v11) = LODWORD(this->m_forward_vel) & _mask__AbsFloat_;
    if ( this->m_vehicle_info->accelGraph )
    {
      if ( (float)((float)(v11 / rbc_wheel) - 1.0) < 0.0 )
        fraction = v11 / rbc_wheel;
      else
        fraction = FLOAT_1_0;
      if ( (float)(0.0 - (float)(v11 / rbc_wheel)) < 0.0 )
        Value = GraphFloat_GetValue(this->m_vehicle_info->accelGraph, fraction);
      else
        Value = GraphFloat_GetValue(this->m_vehicle_info->accelGraph, 0.0);
      *(float *)&ia = Value * *(float *)&ia;
    }
    else if ( low_end_speed > v11 )
    {
      *(float *)&ia = (float)((float)(*(float *)&ia * (float)(1.0 - (float)(v11 / low_end_speed))) * percent_to_give)
                    + *(float *)&ia;
    }
  }
  if ( scale_max_speed )
    rbc_wheel = rbc_wheel * COERCE_FLOAT(LODWORD(this->m_throttle) & _mask__AbsFloat_);
  if ( *(float *)&ia <= NitrousVehicle::get_maximum_acceleration_factor(this) )
    maximum_acceleration_factor = NitrousVehicle::get_maximum_acceleration_factor(this);
  else
    maximum_acceleration_factor = *(float *)&ia;
  this->m_acceleration_factor = maximum_acceleration_factor;
  this->m_desired_speed_factor = rbc_wheel;
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
      v6 = *(float *)&ia;
    else
      v6 = *(float *)&ia * 2.0;
    if ( v6 <= NitrousVehicle::get_maximum_acceleration_factor(this) )
      v8 = NitrousVehicle::get_maximum_acceleration_factor(this);
    else
      v8 = v6;
    this->m_acceleration_factor = v8;
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
        && COERCE_FLOAT(LODWORD(this->m_parameter->m_peel_out_max_speed) ^ _mask__NegFloat_) > this->m_forward_vel )
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
    this->m_forward_vel = *(float *)&FLOAT_0_0;
  }
  if ( this->m_script_brake > 0.0 )
    this->m_state_flags |= 2u;
  if ( (this->m_flags & 1) == 0 )
  {
    NitrousVehicle::update_braking_and_acceleration(this, delta_t);
    NitrousVehicle::update_steering(this, COERCE_FLOAT(v33), delta_t);
    NitrousVehicle::_update_fakey_stuff(this, delta_t);
    NitrousVehicle::_update_friction(this, delta_t);
    NitrousVehicle::_update_orientation_constraint(this);
    if ( (this->m_flags & 0x80) != 0 )
    {
      if ( !this->m_vpc )
        NitrousVehicle::start_path(this, 0);
      if ( this->m_vpc )
        path_constraint_update((int)v33, this->m_vpc, this->m_owner);
    }
  }
}

void __thiscall NitrousVehicle::_update_epilog(NitrousVehicle *this, float delta_t)
{
  int savedregs; // [esp+8h] [ebp+0h] BYREF

  NitrousVehicle::_update_wheel_effect_states(this, (int)&savedregs, delta_t);
}

void __thiscall NitrousVehicle::_update_friction(NitrousVehicle *this, float delta_t)
{
  float v2; // [esp+0h] [ebp-28h]
  float v4; // [esp+10h] [ebp-18h]
  float fric_scaler; // [esp+1Ch] [ebp-Ch]
  float fric_scalera; // [esp+1Ch] [ebp-Ch]
  float up; // [esp+24h] [ebp-4h]

  if ( (this->m_flags & 0x80) == 0 )
  {
    this->m_hand_brake_friction_time = this->m_hand_brake_friction_time - delta_t;
    if ( this->m_hand_brake > 0.1 && this->m_parameter->m_tire_fric_hand_brake > 0.0 )
      this->m_hand_brake_friction_time = hand_brake_friction_time;
    up = this->m_phys_user_data->body->m_mat.z.z;
    fric_scaler = FLOAT_1_0;
    if ( this->m_vehicle_info->max_fric_tilt_angle > up && this->m_vehicle_info->max_fric_tilt_angle > 0.0 )
    {
      fric_scalera = (float)(this->m_vehicle_info->max_fric_tilt / this->m_vehicle_info->max_fric_tilt_angle) * up;
      if ( (float)(fric_scalera - 1.0) < 0.0 )
        v4 = (float)(this->m_vehicle_info->max_fric_tilt / this->m_vehicle_info->max_fric_tilt_angle) * up;
      else
        v4 = FLOAT_1_0;
      if ( (float)(0.15000001 - fric_scalera) < 0.0 )
        v2 = v4;
      else
        v2 = FLOAT_0_15000001;
      fric_scaler = v2;
    }
    if ( NitrousVehicle::is_boosting(this) && this->m_vehicle_info->boostFrictionMultiplier > 0.0 )
      fric_scaler = fric_scaler * this->m_vehicle_info->boostFrictionMultiplier;
    if ( (this->m_flags & 0x80) != 0 )
    {
      this->m_current_front_side_fric_scale = *(float *)&FLOAT_0_0;
      this->m_current_front_fwd_fric_scale = *(float *)&FLOAT_0_0;
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
        if ( COERCE_FLOAT(LODWORD(this->m_throttle) & _mask__AbsFloat_) <= 0.1 )
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

void __thiscall NitrousVehicle::_update_fakey_stuff(NitrousVehicle *this, float delta_t)
{
  environment_rigid_body *environment_rigid_body; // eax
  float v4; // [esp+Ch] [ebp-7Ch]
  rigid_body_constraint_wheel *v5; // [esp+48h] [ebp-40h]
  int j; // [esp+4Ch] [ebp-3Ch]
  rigid_body_constraint_wheel *rbc_wheel; // [esp+50h] [ebp-38h]
  int i; // [esp+54h] [ebp-34h]
  float rpm_lerp_rate; // [esp+58h] [ebp-30h]
  environment_rigid_body *rb; // [esp+5Ch] [ebp-2Ch]
  float spring_delta; // [esp+60h] [ebp-28h]
  float spring_k; // [esp+64h] [ebp-24h]
  int num_colliding_wheels; // [esp+6Ch] [ebp-1Ch]
  bool high_rpm; // [esp+73h] [ebp-15h]
  float rpm_target; // [esp+7Ch] [ebp-Ch]
  bool peeling_out; // [esp+83h] [ebp-5h]
  rigid_body_constraint_wheel *wheel; // [esp+84h] [ebp-4h]
  rigid_body_constraint_wheel *wheela; // [esp+84h] [ebp-4h]
  rigid_body_constraint_wheel *wheelb; // [esp+84h] [ebp-4h]
  rigid_body_constraint_wheel *wheelc; // [esp+84h] [ebp-4h]

  rb = (environment_rigid_body *)this->m_phys_user_data->body;
  num_colliding_wheels = 0;
  spring_delta = *(float *)&FLOAT_0_0;
  if ( this->m_throttle <= 0.5 )
  {
    if ( this->m_throttle < -0.5 )
      spring_delta = (float)(COERCE_FLOAT(LODWORD(this->m_parameter->m_susp_spring_k) ^ _mask__NegFloat_)
                           * this->m_parameter->m_tilt_fakey)
                   * this->m_parameter->m_body_mass;
  }
  else
  {
    spring_delta = (float)(this->m_parameter->m_susp_spring_k * this->m_parameter->m_tilt_fakey)
                 * this->m_parameter->m_body_mass;
  }
  spring_k = FLOAT_30_0;
  wheel = this->m_wheels[1];
  if ( wheel )
  {
    spring_k = NitrousVehicle::_calc_initial_susp_spring_k(this, wheel);
    wheel->m_suspension_stiffness_k = spring_k + spring_delta;
    if ( (wheel->m_wheel_flags & 1) != 0 )
      num_colliding_wheels = 1;
  }
  wheela = this->m_wheels[0];
  if ( wheela )
  {
    wheela->m_suspension_stiffness_k = spring_k + spring_delta;
    if ( (wheela->m_wheel_flags & 1) != 0 )
      ++num_colliding_wheels;
  }
  wheelb = this->m_wheels[3];
  if ( wheelb )
  {
    spring_k = NitrousVehicle::_calc_initial_susp_spring_k(this, wheelb);
    if ( (wheelb->m_wheel_flags & 1) != 0 )
      ++num_colliding_wheels;
    wheelb->m_suspension_stiffness_k = spring_k - spring_delta;
  }
  wheelc = this->m_wheels[2];
  if ( wheelc )
  {
    wheelc->m_suspension_stiffness_k = spring_k - spring_delta;
    if ( (wheelc->m_wheel_flags & 1) != 0 )
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
    environment_rigid_body = phys_sys::get_environment_rigid_body();
    this->m_orientation_constraint = phys_sys::create_rbc_custom_orientation(rb, environment_rigid_body, 0);
    this->m_orientation_constraint->m_torque_resistance_pitch_roll = this->m_parameter->m_pitch_roll_resistance
                                                                   * this->m_parameter->m_body_mass;
    this->m_orientation_constraint->m_torque_resistance_yaw = this->m_parameter->m_yaw_resistance
                                                            * this->m_parameter->m_body_mass;
    this->m_orientation_constraint->m_upright_strength = this->m_parameter->m_upright_strength
                                                       * this->m_parameter->m_body_mass;
  }
  peeling_out = NitrousVehicle::is_peeling_out(this);
  high_rpm = 0;
  if ( this->m_vehicle_info->type != 2 && this->m_vehicle_info->type != 4 )
  {
    for ( j = 0; j < 6; ++j )
    {
      v5 = this->m_wheels[j];
      if ( v5 && !v5->m_wheel_state && (peeling_out || this->m_throttle > 0.69999999 && (v5->m_wheel_flags & 1) == 0) )
      {
        high_rpm = 1;
        v5->m_wheel_pos = (float)(40.0 * delta_t) + v5->m_wheel_pos;
      }
    }
  }
  if ( peeling_out && this->m_throttle > 0.69999999 )
    high_rpm = 1;
  v4 = this->m_forward_vel / NitrousVehicle::get_max_speed(this, 1);
  rpm_target = COERCE_FLOAT(LODWORD(this->m_throttle) & _mask__AbsFloat_) * COERCE_FLOAT(LODWORD(v4) & _mask__AbsFloat_);
  if ( this->m_vehicle_info->type == 4 )
  {
    if ( (float)(this->m_throttle * 0.80000001) > rpm_target )
      rpm_target = this->m_throttle * 0.80000001;
    if ( (this->m_flags & 0x400) == 0 )
      rpm_target = FLOAT_1_0;
  }
  if ( high_rpm && rpm_target < 0.80000001 )
    rpm_target = FLOAT_0_80000001;
  rpm_lerp_rate = FLOAT_1_5;
  if ( snd_boat_using_lerp_rpm && snd_boat_using_lerp_rpm->current.enabled )
    rpm_lerp_rate = snd_boat_lerp_rpm->current.value;
  this->m_fake_rpm = (float)((float)(delta_t * (float)(rpm_target - this->m_fake_rpm)) * rpm_lerp_rate)
                   + this->m_fake_rpm;
}

void __thiscall NitrousVehicle::start_path(NitrousVehicle *this, int attach_mode)
{
  float axis[9]; // [esp+30h] [ebp-3Ch] BYREF
  float *currentOrigin; // [esp+54h] [ebp-18h]
  float origin[3]; // [esp+58h] [ebp-14h] BYREF
  rigid_body_constraint_wheel *rbc_wheel; // [esp+64h] [ebp-8h]
  int i; // [esp+68h] [ebp-4h]
  int savedregs; // [esp+6Ch] [ebp+0h] BYREF

  Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
  Sys_EnterCriticalSection(CRITSECT_PHYSICS);
  this->m_flags &= ~4u;
  this->m_flags &= ~0x100u;
  this->m_flags &= ~0x80u;
  if ( attach_mode )
  {
    this->m_flags |= 0x100u;
    NitrousVehicle::unpause_physics(this, (int)&savedregs);
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
      if ( COERCE_FLOAT(LODWORD(this->m_prev_rb_mat.w.x) & _mask__AbsFloat_) > 100000.0
        || COERCE_FLOAT(LODWORD(this->m_prev_rb_mat.w.y) & _mask__AbsFloat_) > 100000.0
        || COERCE_FLOAT(LODWORD(this->m_prev_rb_mat.w.z) & _mask__AbsFloat_) > 100000.0 )
      {
        phys_exec_debug_callback(0);
      }
      SV_LinkEntity((int)&savedregs, this->m_owner);
    }
    else if ( !this->m_vpc )
    {
      this->m_vpc = path_constraint_create((int)&savedregs, this->m_owner);
      for ( i = 0; i < 6; ++i )
      {
        rbc_wheel = this->m_wheels[i];
        if ( rbc_wheel )
        {
          rbc_wheel->m_fwd_fric_k = FLOAT_0_0099999998;
          rbc_wheel->m_side_fric_k = FLOAT_0_0099999998;
        }
      }
    }
  }
  Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
  Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
}

void __thiscall NitrousVehicle::end_path(NitrousVehicle *this)
{
  Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
  Sys_EnterCriticalSection(CRITSECT_PHYSICS);
  this->m_flags &= ~0x100u;
  this->m_flags &= ~0x80u;
  NitrousVehicle::cleanup_path(this);
  Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
  Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
}

void __thiscall NitrousVehicle::cleanup_path(NitrousVehicle *this)
{
  if ( this->m_vpc )
  {
    path_constraint_destroy(this->m_vpc);
    this->m_vpc = 0;
  }
}

void __cdecl NitrousVehicle::frame_prolog_all_systems(float delta_t)
{
  phys_free_list<NitrousVehicle>::T_internal_base *i; // [esp+14h] [ebp-8h]
  int savedregs; // [esp+1Ch] [ebp+0h] BYREF

  //PIXBeginNamedEvent(-1, "NitrousVehicle::frame_prolog");
  for ( i = g_rb_vehicle_list.m_dummy_head.m_next_T_internal;
        &g_rb_vehicle_list != (phys_free_list<NitrousVehicle> *)i;
        i = i->m_next_T_internal )
  {
    NitrousVehicle::_update_prolog((NitrousVehicle *)&i[2], (int)&savedregs, delta_t);
  }
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

void __cdecl NitrousVehicle::frame_epilog_all_systems(float delta_t)
{
  phys_free_list<NitrousVehicle>::T_internal_base *i; // [esp+14h] [ebp-8h]

  //PIXBeginNamedEvent(-1, "NitrousVehicle::frame_epilog");
  for ( i = g_rb_vehicle_list.m_dummy_head.m_next_T_internal;
        &g_rb_vehicle_list != (phys_free_list<NitrousVehicle> *)i;
        i = i->m_next_T_internal )
  {
    NitrousVehicle::_update_epilog((NitrousVehicle *)&i[2], delta_t);
  }
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

NitrousVehicle **__cdecl NitrousVehicle::add_vehicle(NitrousVehicle *id)
{
  phys_free_list<NitrousVehicle> *i; // [esp+24h] [ebp-8h]
  NitrousVehicle *vehicle; // [esp+28h] [ebp-4h]

  if ( id != (NitrousVehicle *)-1 )
  {
    for ( i = (phys_free_list<NitrousVehicle> *)g_rb_vehicle_list.m_dummy_head.m_next_T_internal;
          &g_rb_vehicle_list != i;
          i = (phys_free_list<NitrousVehicle> *)i->m_dummy_head.m_next_T_internal )
    {
      if ( i->m_ptr_list[244] == id )
      {
        ++i->m_ptr_list[245];
        return i->m_ptr_list;
      }
    }
  }
  vehicle = phys_free_list<NitrousVehicle>::add(&g_rb_vehicle_list, 1, "phys_free_list error: out of memory.");
  if ( (!vehicle || vehicle->refcount)
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
         1553,
         "vehicle && vehicle->refcount == 0",
         "Failed to add a new NitrousVehicle.") )
  {
    __debugbreak();
  }
  if ( (vehicle->m_flags & 1) == 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
          1554,
          0,
          "%s",
          "vehicle->get_flag(FLAG_PAUSED)") )
  {
    __debugbreak();
  }
  vehicle->id = (int)id;
  vehicle->refcount = 1;
  return (NitrousVehicle **)vehicle;
}

void __cdecl NitrousVehicle::reinit_parms()
{
  phys_free_list<NitrousVehicle>::T_internal_base *i; // [esp+10h] [ebp-4h]
  int savedregs; // [esp+14h] [ebp+0h] BYREF

  for ( i = g_rb_vehicle_list.m_dummy_head.m_next_T_internal;
        &g_rb_vehicle_list != (phys_free_list<NitrousVehicle> *)i;
        i = i->m_next_T_internal )
  {
    NitrousVehicle::update_parms(
      (NitrousVehicle *)&i[2],
      (int)&savedregs,
      (const VehicleParameter *)i[64].m_prev_T_internal,
      0);
  }
}

void __cdecl NitrousVehicle::reset_vehicle_physics()
{
  phys_free_list<NitrousVehicle>::T_internal_base *i; // [esp+Ch] [ebp-4h]
  int savedregs; // [esp+10h] [ebp+0h] BYREF

  for ( i = g_rb_vehicle_list.m_dummy_head.m_next_T_internal;
        &g_rb_vehicle_list != (phys_free_list<NitrousVehicle> *)i;
        i = i->m_next_T_internal )
  {
    NitrousVehicle::pause_physics((NitrousVehicle *)&i[2], 0);
    NitrousVehicle::unpause_physics((NitrousVehicle *)&i[2], (int)&savedregs);
  }
}

void __cdecl NitrousVehicle::remove_vehicle(NitrousVehicle *const v)
{
  if ( !--v->refcount )
  {
    NitrousVehicle::pause_physics(v, 1);
    phys_free_list<NitrousVehicle>::remove(&g_rb_vehicle_list, v);
  }
}

void __cdecl NitrousVehicle::debug_render_all()
{
  phys_free_list<NitrousVehicle>::T_internal_base *i; // [esp+8h] [ebp-4h]
  int savedregs; // [esp+Ch] [ebp+0h] BYREF

  for ( i = g_rb_vehicle_list.m_dummy_head.m_next_T_internal;
        &g_rb_vehicle_list != (phys_free_list<NitrousVehicle> *)i;
        i = i->m_next_T_internal )
  {
    NitrousVehicle::debug_render((NitrousVehicle *)&i[2], (int)&savedregs);
  }
}

void  NitrousVehicle::debug_render(NitrousVehicle *this@<ecx>, int a2@<ebp>)
{
  phys_vec3 *v2; // eax
  const phys_vec3 *v3; // eax
  const phys_vec3 *v4; // eax
  float *v5; // ecx
  const phys_vec3 *scale_4; // [esp+10h] [ebp-640h]
  const phys_vec3 *scale_4a; // [esp+10h] [ebp-640h]
  char v8[12]; // [esp+1Ch] [ebp-634h] BYREF
  float buffer[126]; // [esp+28h] [ebp-628h] BYREF
  float v10; // [esp+220h] [ebp-430h]
  float textPos[4]; // [esp+224h] [ebp-42Ch]
  float v12; // [esp+234h] [ebp-41Ch]
  float v13; // [esp+238h] [ebp-418h]
  float v14; // [esp+23Ch] [ebp-414h]
  float *v15; // [esp+240h] [ebp-410h]
  float v16; // [esp+244h] [ebp-40Ch]
  float v17; // [esp+248h] [ebp-408h]
  float v18; // [esp+24Ch] [ebp-404h]
  float v19; // [esp+254h] [ebp-3FCh]
  float v20; // [esp+258h] [ebp-3F8h]
  float v21; // [esp+25Ch] [ebp-3F4h]
  float *v22; // [esp+260h] [ebp-3F0h]
  float v23; // [esp+264h] [ebp-3ECh]
  float v24; // [esp+268h] [ebp-3E8h]
  float v25; // [esp+26Ch] [ebp-3E4h] BYREF
  float v26; // [esp+270h] [ebp-3E0h]
  float v27; // [esp+274h] [ebp-3DCh]
  float arm_col[4]; // [esp+278h] [ebp-3D8h] BYREF
  float _pos[3]; // [esp+288h] [ebp-3C8h] BYREF
  float _dir[3]; // [esp+294h] [ebp-3BCh] BYREF
  phys_vec3 dir; // [esp+2A0h] [ebp-3B0h] BYREF
  float v32; // [esp+2B0h] [ebp-3A0h]
  phys_vec3 v33; // [esp+2B4h] [ebp-39Ch] BYREF
  phys_vec3 v34; // [esp+2C4h] [ebp-38Ch] BYREF
  int v35; // [esp+2E0h] [ebp-370h]
  float v36; // [esp+2E4h] [ebp-36Ch] BYREF
  float v37; // [esp+2E8h] [ebp-368h]
  float v38; // [esp+2ECh] [ebp-364h]
  phys_vec3 limit_pos; // [esp+2F0h] [ebp-360h]
  float v40; // [esp+300h] [ebp-350h]
  float v41; // [esp+304h] [ebp-34Ch]
  float v42; // [esp+308h] [ebp-348h]
  float v43; // [esp+30Ch] [ebp-344h]
  float v44; // [esp+310h] [ebp-340h]
  float v45; // [esp+314h] [ebp-33Ch]
  float v46; // [esp+318h] [ebp-338h]
  float v47; // [esp+31Ch] [ebp-334h]
  float v48; // [esp+324h] [ebp-32Ch]
  float v49; // [esp+328h] [ebp-328h]
  float v50; // [esp+32Ch] [ebp-324h]
  float v51; // [esp+330h] [ebp-320h]
  float v52; // [esp+334h] [ebp-31Ch]
  float v53; // [esp+338h] [ebp-318h]
  float v54; // [esp+33Ch] [ebp-314h]
  float v55; // [esp+340h] [ebp-310h]
  float v56; // [esp+350h] [ebp-300h]
  float v57[2]; // [esp+354h] [ebp-2FCh] BYREF
  float v58; // [esp+35Ch] [ebp-2F4h]
  phys_vec3 hit_pos_displaced; // [esp+360h] [ebp-2F0h]
  float v60; // [esp+370h] [ebp-2E0h]
  float v61; // [esp+374h] [ebp-2DCh]
  float v62; // [esp+378h] [ebp-2D8h]
  float v63; // [esp+37Ch] [ebp-2D4h]
  float v64; // [esp+384h] [ebp-2CCh]
  float v65; // [esp+388h] [ebp-2C8h]
  float v66; // [esp+38Ch] [ebp-2C4h]
  const phys_vec3 *v67; // [esp+390h] [ebp-2C0h]
  phys_vec3 v68; // [esp+394h] [ebp-2BCh] BYREF
  phys_vec3 v69; // [esp+3A4h] [ebp-2ACh] BYREF
  float v70; // [esp+3C0h] [ebp-290h]
  float v71; // [esp+3C4h] [ebp-28Ch] BYREF
  float y; // [esp+3C8h] [ebp-288h]
  float v73; // [esp+3CCh] [ebp-284h]
  phys_vec3 hit_pos; // [esp+3D0h] [ebp-280h] BYREF
  phys_vec3 v75; // [esp+3E4h] [ebp-26Ch] BYREF
  int v76; // [esp+400h] [ebp-250h]
  unsigned int v77[3]; // [esp+404h] [ebp-24Ch] BYREF
  phys_vec3 wheel_pos; // [esp+410h] [ebp-240h] BYREF
  float v79; // [esp+430h] [ebp-220h]
  float x; // [esp+434h] [ebp-21Ch]
  const phys_vec3 *v81; // [esp+438h] [ebp-218h]
  float radius; // [esp+43Ch] [ebp-214h] BYREF
  rigid_body *b1; // [esp+440h] [ebp-210h]
  rigid_body_constraint_wheel *rbc_wheel; // [esp+444h] [ebp-20Ch]
  float col[4]; // [esp+448h] [ebp-208h] BYREF
  int i; // [esp+458h] [ebp-1F8h]
  float alpha; // [esp+45Ch] [ebp-1F4h]
  float v88; // [esp+468h] [ebp-1E8h]
  float v89; // [esp+46Ch] [ebp-1E4h]
  float v90; // [esp+470h] [ebp-1E0h]
  phys_mat44 v91; // [esp+474h] [ebp-1DCh] BYREF
  phys_vec3 v92; // [esp+4B4h] [ebp-19Ch] BYREF
  float v93; // [esp+4C8h] [ebp-188h]
  float v94; // [esp+4CCh] [ebp-184h]
  float v95; // [esp+4D0h] [ebp-180h]
  float v96; // [esp+4D4h] [ebp-17Ch]
  float v97; // [esp+4D8h] [ebp-178h]
  float v98; // [esp+4DCh] [ebp-174h]
  float v99; // [esp+4E8h] [ebp-168h]
  float v100; // [esp+4ECh] [ebp-164h]
  float v101; // [esp+4F0h] [ebp-160h]
  float v102; // [esp+4F4h] [ebp-15Ch]
  float v103; // [esp+4F8h] [ebp-158h]
  float v104; // [esp+4FCh] [ebp-154h]
  float v105; // [esp+508h] [ebp-148h]
  float v106; // [esp+50Ch] [ebp-144h]
  float v107; // [esp+510h] [ebp-140h]
  phys_vec3 v108; // [esp+514h] [ebp-13Ch] BYREF
  phys_vec3 v109; // [esp+524h] [ebp-12Ch] BYREF
  phys_vec3 v110; // [esp+534h] [ebp-11Ch] BYREF
  float v111; // [esp+548h] [ebp-108h]
  float v112; // [esp+54Ch] [ebp-104h]
  float v113; // [esp+550h] [ebp-100h]
  _BYTE v114[12]; // [esp+554h] [ebp-FCh] BYREF
  phys_mat44 transform; // [esp+560h] [ebp-F0h] BYREF
  float *v116; // [esp+5A0h] [ebp-B0h]
  float v117; // [esp+5A4h] [ebp-ACh] BYREF
  float w; // [esp+5A8h] [ebp-A8h]
  float z; // [esp+5ACh] [ebp-A4h]
  phys_vec3 com; // [esp+5B0h] [ebp-A0h]
  float v121; // [esp+5C0h] [ebp-90h]
  float v122; // [esp+5C4h] [ebp-8Ch]
  float v123; // [esp+5C8h] [ebp-88h]
  float v124; // [esp+5CCh] [ebp-84h]
  float v125; // [esp+5D8h] [ebp-78h]
  float v126; // [esp+5DCh] [ebp-74h]
  float v127; // [esp+5E0h] [ebp-70h]
  float v128; // [esp+5E4h] [ebp-6Ch]
  float v129; // [esp+5E8h] [ebp-68h]
  float v130; // [esp+5ECh] [ebp-64h]
  phys_vec3 dim; // [esp+5F0h] [ebp-60h]
  float v132; // [esp+600h] [ebp-50h]
  float v133; // [esp+604h] [ebp-4Ch] BYREF
  float v134; // [esp+608h] [ebp-48h]
  float v135; // [esp+60Ch] [ebp-44h]
  phys_vec3 vmax; // [esp+610h] [ebp-40h] BYREF
  phys_vec3 vmin; // [esp+620h] [ebp-30h] BYREF
  phys_vec3 pos; // [esp+630h] [ebp-20h]
  int v139; // [esp+644h] [ebp-Ch] BYREF
  NitrousVehicle *thisa; // [esp+648h] [ebp-8h]
  NitrousVehicle *retaddr; // [esp+650h] [ebp+0h]

  v139 = a2;
  thisa = retaddr;
  LODWORD(pos.w) = this;
  if ( (phys_drawNitrousVehicle->current.enabled || (*(unsigned int *)(LODWORD(pos.w) + 600) & 8) != 0)
    && *(unsigned int *)(LODWORD(pos.w) + 560) )
  {
    LODWORD(pos.z) = **(unsigned int **)(LODWORD(pos.w) + 560) + 48;
    vmin.y = *(float *)(LODWORD(pos.z) + 48);
    vmin.z = *(float *)(LODWORD(pos.z) + 52);
    vmin.w = *(float *)(LODWORD(pos.z) + 56);
    pos.x = *(float *)(LODWORD(pos.z) + 60);
    Phys_DebugBox((const phys_mat44 *)(**(unsigned int **)(LODWORD(pos.w) + 560) + 48), colorYellow, 1.0);
    if ( *(float *)(*(unsigned int *)(LODWORD(pos.w) + 496) + 172) != 0.0 )
    {
      Phys_Vec3ToNitrousVec((float *)(*(unsigned int *)(LODWORD(pos.w) + 496) + 172), (phys_vec3 *)&vmax.y);
      Phys_Vec3ToNitrousVec((float *)(*(unsigned int *)(LODWORD(pos.w) + 496) + 184), (phys_vec3 *)&v133);
      v132 = v133 - vmax.y;
      dim.w = v134 - vmax.z;
      dim.z = v135 - vmax.w;
      v128 = v133 - vmax.y;
      v129 = v134 - vmax.z;
      v130 = v135 - vmax.w;
      v127 = v133 + vmax.y;
      v126 = v134 + vmax.z;
      v125 = v135 + vmax.w;
      v122 = v133 + vmax.y;
      v123 = v134 + vmax.z;
      v124 = v135 + vmax.w;
      v121 = 0.5 * (float)(v133 + vmax.y);
      com.w = 0.5 * (float)(v134 + vmax.z);
      com.z = 0.5 * (float)(v135 + vmax.w);
      v117 = v121;
      w = com.w;
      z = com.z;
      v116 = (float *)(*(unsigned int *)(LODWORD(pos.w) + 560) + 80);
      v117 = v121 + *v116;
      w = com.w + v116[1];
      z = com.z + v116[2];
      phys_mat44::operator=((phys_mat44 *)v114, &PHYS_IDENTITY_MATRIX_34);
      phys_vec3::operator=((phys_vec3 *)&transform.z.y, (const phys_vec3 *)&v117);
      phys_full_multiply_mat(
        (int)&v139,
        (phys_mat44 *)v114,
        (const phys_mat44 *)v114,
        (const phys_mat44 *)(**(unsigned int **)(LODWORD(pos.w) + 560) + 48));
      v113 = 0.5 * v128;
      v112 = 0.5 * v129;
      v111 = 0.5 * v130;
      v110.x = 0.5 * v128;
      v110.y = 0.5 * v129;
      v110.z = 0.5 * v130;
      Phys_DebugBox((const phys_mat44 *)v114, &v110, colorGreen);
    }
    if ( *(float *)(*(unsigned int *)(LODWORD(pos.w) + 496) + 208) != 0.0 )
    {
      Phys_Vec3ToNitrousVec((float *)(*(unsigned int *)(LODWORD(pos.w) + 496) + 208), &v109);
      Phys_Vec3ToNitrousVec((float *)(*(unsigned int *)(LODWORD(pos.w) + 496) + 220), &v108);
      v107 = v108.x - v109.x;
      v106 = v108.y - v109.y;
      v105 = v108.z - v109.z;
      v102 = v108.x - v109.x;
      v103 = v108.y - v109.y;
      v104 = v108.z - v109.z;
      v101 = v108.x + v109.x;
      v100 = v108.y + v109.y;
      v99 = v108.z + v109.z;
      v96 = v108.x + v109.x;
      v97 = v108.y + v109.y;
      v98 = v108.z + v109.z;
      v95 = 0.5 * (float)(v108.x + v109.x);
      v94 = 0.5 * (float)(v108.y + v109.y);
      v93 = 0.5 * (float)(v108.z + v109.z);
      v92.x = v95;
      v92.y = v94;
      v92.z = v93;
      phys_mat44::operator=(&v91, &PHYS_IDENTITY_MATRIX_34);
      phys_vec3::operator=(&v91.w, &v92);
      phys_full_multiply_mat((int)&v139, &v91, &v91, (const phys_mat44 *)(**(unsigned int **)(LODWORD(pos.w) + 560) + 48));
      v90 = 0.5 * v102;
      v89 = 0.5 * v103;
      v88 = 0.5 * v104;
      col[3] = 0.5 * v102;
      *(float *)&i = 0.5 * v103;
      alpha = 0.5 * v104;
      Phys_DebugBox(&v91, (const phys_vec3 *)&col[3], colorBlue);
    }
    col[2] = FLOAT_0_18000001;
    for ( col[1] = 0.0; SLODWORD(col[1]) < 6; ++LODWORD(col[1]) )
    {
      radius = *(float *)&FLOAT_0_0;
      b1 = (rigid_body *)LODWORD(FLOAT_1_0);
      rbc_wheel = *(rigid_body_constraint_wheel **)&FLOAT_0_0;
      col[0] = col[2];
      v81 = *(const phys_vec3 **)(LODWORD(pos.w) + 4 * LODWORD(col[1]) + 984);
      if ( v81 )
      {
        x = v81->x;
        v79 = v81[6].x;
        v2 = operator*((phys_vec3 *)&wheel_pos.y, v81[10].y, v81 + 4);
        operator-((phys_vec3 *)v77, v81 + 3, v2);
        v76 = LODWORD(x) + 48;
        scale_4 = (const phys_vec3 *)(LODWORD(x) + 96);
        v3 = phys_multiply(&v75, (const phys_mat44 *)(LODWORD(x) + 48), (const phys_vec3 *)v77);
        operator+((phys_vec3 *)&hit_pos.y, v3, scale_4);
        v77[0] = LODWORD(hit_pos.y);
        v77[1] = LODWORD(hit_pos.z);
        v77[2] = LODWORD(hit_pos.w);
        v71 = v81[1].x;
        y = v81[1].y;
        v73 = v81[1].z;
        hit_pos.x = v81[1].w;
        v70 = v81[7].w;
        v69 = v81[4];
        v67 = phys_multiply(&v68, (const phys_mat44 *)(LODWORD(x) + 48), &v69);
        v66 = v70 * v67->x;
        v65 = v70 * v67->y;
        v64 = v70 * v67->z;
        v61 = v66;
        v62 = v65;
        v63 = v64;
        v60 = v71 - v66;
        hit_pos_displaced.w = y - v65;
        hit_pos_displaced.z = v73 - v64;
        v57[0] = v71 - v66;
        v57[1] = y - v65;
        v58 = v73 - v64;
        if ( (LODWORD(v81[11].x) & 1) != 0 )
        {
          radius = FLOAT_1_0;
          b1 = (rigid_body *)LODWORD(FLOAT_1_0);
          rbc_wheel = *(rigid_body_constraint_wheel **)&FLOAT_0_0;
          col[0] = col[2];
        }
        if ( (LODWORD(v81[11].x) & 4) != 0 )
        {
          radius = FLOAT_1_0;
          b1 = *(rigid_body **)&FLOAT_0_0;
          rbc_wheel = *(rigid_body_constraint_wheel **)&FLOAT_0_0;
          col[0] = col[2];
        }
        if ( (LODWORD(v81[11].x) & 2) != 0 )
        {
          v56 = v81[7].z;
          v52 = v81[4].x;
          v53 = v81[4].y;
          v54 = v81[4].z;
          v55 = v81[4].w;
          v51 = v56 + v79;
          v50 = (float)(v56 + v79) * v52;
          v49 = (float)(v56 + v79) * v53;
          v48 = (float)(v56 + v79) * v54;
          v45 = v50;
          v46 = v49;
          v47 = v48;
          v41 = v81[3].x;
          v42 = v81[3].y;
          v43 = v81[3].z;
          v44 = v81[3].w;
          v40 = v41 - v50;
          limit_pos.w = v42 - v49;
          limit_pos.z = v43 - v48;
          v36 = v41 - v50;
          v37 = v42 - v49;
          v38 = v43 - v48;
          v35 = LODWORD(x) + 48;
          scale_4a = (const phys_vec3 *)(LODWORD(x) + 96);
          v4 = phys_multiply(&v34, (const phys_mat44 *)(LODWORD(x) + 48), (const phys_vec3 *)&v36);
          operator+(&v33, v4, scale_4a);
          v36 = v33.x;
          v37 = v33.y;
          v38 = v33.z;
          if ( (LODWORD(v81[11].x) & 2) != 0 )
            Phys_DebugPoint((const phys_vec3 *)&v36, sphere_size, colorRed);
          else
            Phys_DebugPoint((const phys_vec3 *)&v36, sphere_size, colorGreen);
        }
        Phys_DebugPoint((const phys_vec3 *)&v71, sphere_size, colorRed);
        Phys_DebugPoint((const phys_vec3 *)v57, sphere_size, colorBlue);
        Phys_DebugPoint((const phys_vec3 *)v77, v79, &radius);
        if ( *(_WORD *)(*(unsigned int *)(LODWORD(pos.w) + 556) + 64) == 2 || render_wheel_cir )
        {
          dir.y = v81[5].x;
          dir.z = v81[5].y;
          dir.w = v81[5].z;
          v32 = v81[5].w;
          phys_multiply((const phys_vec3 *)_dir, (const phys_mat44 *)(LODWORD(x) + 48), (phys_vec3 *)&dir.y);
          Phys_NitrousVecToVec3((const phys_vec3 *)_dir, _pos);
          Phys_NitrousVecToVec3((const phys_vec3 *)v77, &arm_col[1]);
          G_DebugCircleEx(&arm_col[1], v79, _pos, colorWhite, 0, 0);
        }
        v25 = *(float *)&FLOAT_0_0;
        v26 = FLOAT_1_0;
        v27 = FLOAT_1_0;
        arm_col[0] = FLOAT_1_0;
        v24 = vmin.w;
        v23 = v58;
        if ( v58 > (float)(vmin.w - 5.0) )
        {
          v25 = FLOAT_1_0;
          v26 = *(float *)&FLOAT_0_0;
          v27 = *(float *)&FLOAT_0_0;
          arm_col[0] = FLOAT_1_0;
        }
        render_line((phys_vec3 *)&vmin.y, (const phys_vec3 *)v57, &v25, 0, 0);
      }
    }
    v22 = (float *)(**(unsigned int **)(LODWORD(pos.w) + 560) + 48);
    v21 = 130.0 * *v22;
    v20 = 130.0 * v22[1];
    v19 = 130.0 * v22[2];
    v16 = v21;
    v17 = v20;
    v18 = v19;
    vmin.y = vmin.y + v21;
    vmin.z = vmin.z + v20;
    vmin.w = vmin.w + v19;
    if ( (*(unsigned int *)(LODWORD(pos.w) + 600) & 0x40) != 0 )
    {
      v5 = **(float ***)(LODWORD(pos.w) + 560);
      v15 = v5 + 12;
      v14 = 100.0 * v5[12];
      v13 = 100.0 * v5[13];
      v12 = 100.0 * v5[14];
      textPos[0] = v14;
      textPos[1] = v13;
      textPos[2] = v12;
      vmin.y = vmin.y + v14;
      vmin.z = vmin.z + v13;
      vmin.w = vmin.w + v12;
    }
    Phys_NitrousVecToVec3((phys_vec3 *)&vmin.y, &buffer[124]);
    v10 = v10 + 50.0;
    sprintf(v8, "Speed %.1f %.0f", (float)(*(float *)(LODWORD(pos.w) + 520) / 17.6), *(float *)(LODWORD(pos.w) + 520));
    CL_AddDebugString(&buffer[124], colorWhite, 1.0, v8, 1);
    v10 = v10 - 20.0;
    sprintf(v8, "Rpm %.2f", *(float *)(LODWORD(pos.w) + 880));
    CL_AddDebugString(&buffer[124], colorWhite, 1.0, v8, 1);
    if ( snd_boat_using_lerp_rpm && snd_boat_using_lerp_rpm->current.enabled )
    {
      v10 = v10 - 20.0;
      sprintf(v8, "boat rpm %.2f", snd_boat_current_rpm->current.value);
      CL_AddDebugString(&buffer[124], colorWhite, 1.0, v8, 1);
    }
  }
  NitrousVehicle::debug_render_effects((NitrousVehicle *)LODWORD(pos.w), (int)&v139);
}

// local variable allocation has failed, the output may be wrong!
void  phys_full_multiply_mat(int a1@<ebp>, phys_mat44 *dest, const phys_mat44 *left, const phys_mat44 *right)
{
  _BYTE v4[76]; // [esp-Ch] [ebp-CCh] OVERLAPPED BYREF
  const phys_vec3 *v5; // [esp+40h] [ebp-80h]
  phys_vec3 v6; // [esp+44h] [ebp-7Ch] BYREF
  phys_vec3 *p_z; // [esp+5Ch] [ebp-64h]
  const phys_vec3 *v8; // [esp+60h] [ebp-60h]
  phys_vec3 v9; // [esp+64h] [ebp-5Ch] BYREF
  phys_vec3 *p_y; // [esp+7Ch] [ebp-44h]
  const phys_vec3 *v11; // [esp+80h] [ebp-40h]
  phys_vec3 v12; // [esp+84h] [ebp-3Ch] BYREF
  const phys_vec3 *v13; // [esp+A0h] [ebp-20h]
  phys_vec3 v14; // [esp+A4h] [ebp-1Ch] BYREF
  unsigned int v15[3]; // [esp+B4h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+C0h] [ebp+0h]

  v15[0] = a1;
  v15[1] = retaddr;
  if ( dest == left )
  {
    memcpy(v4, left, 0x40u);
    phys_full_multiply_mat(dest, (const phys_mat44 *)v4, right);
  }
  else
  {
    v13 = phys_multiply(&v14, left, &right->x);
    dest->x.x = v13->x;
    dest->x.y = v13->y;
    dest->x.z = v13->z;
    v11 = phys_multiply(&v12, left, &right->y);
    p_y = &dest->y;
    dest->y.x = v11->x;
    p_y->y = v11->y;
    p_y->z = v11->z;
    v8 = phys_multiply(&v9, left, &right->z);
    p_z = &dest->z;
    dest->z.x = v8->x;
    p_z->y = v8->y;
    p_z->z = v8->z;
    v5 = phys_full_multiply((int)v15, &v6, left, &right->w);
    *(unsigned int *)&v4[72] = &dest->w;
    dest->w.x = v5->x;
    *(float *)(*(unsigned int *)&v4[72] + 4) = v5->y;
    *(float *)(*(unsigned int *)&v4[72] + 8) = v5->z;
  }
}

void __thiscall NitrousVehicle::remove_wheels(NitrousVehicle *this)
{
  int i; // [esp+4h] [ebp-4h]

  for ( i = 0; i < 6; ++i )
  {
    if ( this->m_wheels[i] )
      this->m_wheels[i] = 0;
  }
}

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
  this->m_steer_factor = *(float *)&FLOAT_0_0;
  this->m_steer_current_angle = *(float *)&FLOAT_0_0;
  this->m_state_flags = 0;
  this->m_forward_vel = *(float *)&FLOAT_0_0;
}

// local variable allocation has failed, the output may be wrong!
void __userpurge NitrousVehicle::update_steering(NitrousVehicle *this@<ecx>, float a2@<ebp>, float delta_t)
{
  long double v3; // [esp-24h] [ebp-15Ch]
  int m; // [esp-Ch] [ebp-144h]
  float v5; // [esp-8h] [ebp-140h]
  float v6; // [esp-4h] [ebp-13Ch]
  float *v7; // [esp+0h] [ebp-138h]
  float v8; // [esp+14h] [ebp-124h]
  float v9; // [esp+18h] [ebp-120h]
  float sign; // [esp+1Ch] [ebp-11Ch] BYREF
  float y; // [esp+20h] [ebp-118h]
  float nwheel_axis; // [esp+24h] [ebp-114h]
  phys_vec3 wheel_axis; // [esp+28h] [ebp-110h]
  float v14; // [esp+38h] [ebp-100h]
  float v15; // [esp+3Ch] [ebp-FCh]
  float v16; // [esp+40h] [ebp-F8h]
  float v17; // [esp+44h] [ebp-F4h]
  int v18; // [esp+48h] [ebp-F0h]
  int v19; // [esp+54h] [ebp-E4h]
  int k; // [esp+58h] [ebp-E0h]
  unsigned int v21[3]; // [esp+5Ch] [ebp-DCh] BYREF
  float wheel_ratios[6]; // [esp+68h] [ebp-D0h] BYREF
  float total_ratio; // [esp+80h] [ebp-B8h]
  int num_accel_wheels; // [esp+84h] [ebp-B4h]
  phys_vec3 turning_center; // [esp+88h] [ebp-B0h]
  int v26; // [esp+98h] [ebp-A0h]
  float v27; // [esp+9Ch] [ebp-9Ch]
  float v28; // [esp+A0h] [ebp-98h]
  int v29; // [esp+A4h] [ebp-94h]
  float v30; // [esp+ACh] [ebp-8Ch]
  float v31; // [esp+B0h] [ebp-88h]
  float v32; // [esp+B4h] [ebp-84h]
  float v33; // [esp+B8h] [ebp-80h]
  float v34; // [esp+BCh] [ebp-7Ch] BYREF
  float v35; // [esp+C0h] [ebp-78h]
  float turning_center_radius; // [esp+C4h] [ebp-74h]
  phys_vec3 turning_center_radius_vec; // [esp+C8h] [ebp-70h]
  float v38; // [esp+E4h] [ebp-54h]
  float v39; // [esp+E8h] [ebp-50h]
  float v40; // [esp+ECh] [ebp-4Ch]
  float v41; // [esp+F0h] [ebp-48h]
  float width; // [esp+F4h] [ebp-44h]
  int j; // [esp+F8h] [ebp-40h]
  char v44; // [esp+FFh] [ebp-39h]
  int v45; // [esp+100h] [ebp-38h]
  int v46; // [esp+104h] [ebp-34h]
  int v47; // [esp+108h] [ebp-30h] OVERLAPPED
  int v48; // [esp+10Ch] [ebp-2Ch]
  int n; // [esp+110h] [ebp-28h]
  int v50; // [esp+114h] [ebp-24h]
  float v51; // [esp+118h] [ebp-20h]
  int i; // [esp+11Ch] [ebp-1Ch]
  float v53; // [esp+120h] [ebp-18h]
  float angle_delta; // [esp+124h] [ebp-14h]
  float max_delta_angle; // [esp+128h] [ebp-10h]
  float new_steer_angle; // [esp+12Ch] [ebp-Ch]
  float steer; // [esp+130h] [ebp-8h]
  float retaddr; // [esp+138h] [ebp+0h]

  new_steer_angle = a2;
  steer = retaddr;
  LODWORD(max_delta_angle) = this;
  angle_delta = this->m_steer_factor;
  if ( angle_delta <= 1.0 )
  {
    if ( angle_delta < -1.0 )
      angle_delta = FLOAT_N1_0;
  }
  else
  {
    angle_delta = FLOAT_1_0;
  }
  v53 = *(float *)(LODWORD(max_delta_angle) + 1036) * angle_delta;
  *(float *)&i = *(float *)(LODWORD(max_delta_angle) + 1040) * delta_t;
  v51 = v53 - *(float *)(LODWORD(max_delta_angle) + 1032);
  if ( v51 <= *(float *)&i )
  {
    if ( COERCE_FLOAT(i ^ _mask__NegFloat_) > v51 )
      LODWORD(v51) = i ^ _mask__NegFloat_;
  }
  else
  {
    v51 = *(float *)&i;
  }
  *(float *)(LODWORD(max_delta_angle) + 1032) = *(float *)(LODWORD(max_delta_angle) + 1032) + v51;
  v50 = *(unsigned int *)(LODWORD(max_delta_angle) + 1032);
  if ( COERCE_FLOAT(v50 & _mask__AbsFloat_) > 0.0049999999 )
  {
    v44 = 1;
    for ( j = 0; j < 6; ++j )
    {
      if ( *(unsigned int *)(LODWORD(max_delta_angle) + 4 * j + 984) )
      {
        if ( (*(unsigned int *)(*(unsigned int *)(LODWORD(max_delta_angle) + 4 * j + 984) + 176) & 8) == 0
          || (width = *(float *)(LODWORD(max_delta_angle) + 508), width > 0.1)
          && (*(unsigned int *)(*(unsigned int *)(LODWORD(max_delta_angle) + 4 * j + 984) + 176) & 0x80) != 0 )
        {
          v44 = 0;
          break;
        }
      }
    }
    if ( v44 )
    {
      v41 = *(float *)(LODWORD(max_delta_angle) + 1032);
      __libm_sse2_tan(v3);
      v40 = v41;
      v39 = (float)(*(float *)(LODWORD(max_delta_angle) + 1072) * 0.5) / v41;
      v38 = COERCE_FLOAT(*(unsigned int *)(LODWORD(max_delta_angle) + 1072) ^ _mask__NegFloat_) * 0.5;
      turning_center_radius_vec.y = v38;
      turning_center_radius_vec.z = v39;
      turning_center_radius_vec.w = *(float *)&FLOAT_0_0;
      v34 = v38;
    }
    else
    {
      v32 = *(float *)(LODWORD(max_delta_angle) + 1032);
      __libm_sse2_tan(v3);
      v31 = v32;
      v39 = *(float *)(LODWORD(max_delta_angle) + 1072) / v32;
      LODWORD(v30) = *(unsigned int *)(LODWORD(max_delta_angle) + 1072) ^ _mask__NegFloat_;
      v27 = v30;
      v28 = v39;
      v29 = *(unsigned int *)&FLOAT_0_0;
      v34 = v30;
    }
    v35 = v39;
    turning_center_radius = *(float *)&FLOAT_0_0;
    v33 = Abs(&v34);
    v26 = LODWORD(max_delta_angle) + 1056;
    turning_center.w = *(float *)(LODWORD(max_delta_angle) + 1056) + v34;
    turning_center.z = *(float *)(LODWORD(max_delta_angle) + 1060) + v35;
    turning_center.y = *(float *)(LODWORD(max_delta_angle) + 1064) + turning_center_radius;
    wheel_ratios[5] = turning_center.w;
    total_ratio = turning_center.z;
    num_accel_wheels = SLODWORD(turning_center.y);
    memset(v21, 0, sizeof(v21));
    memset(wheel_ratios, 0, 20);
    for ( k = 0; k < 6; ++k )
    {
      if ( *(unsigned int *)(LODWORD(max_delta_angle) + 4 * k + 984) )
      {
        v19 = *(unsigned int *)(LODWORD(max_delta_angle) + 4 * k + 984);
        v15 = *(float *)(v19 + 48);
        v16 = *(float *)(v19 + 52);
        v17 = *(float *)(v19 + 56);
        v18 = *(unsigned int *)(v19 + 60);
        v14 = wheel_ratios[5] - v15;
        wheel_axis.w = total_ratio - v16;
        wheel_axis.z = *(float *)&num_accel_wheels - v17;
        sign = wheel_ratios[5] - v15;
        y = total_ratio - v16;
        nwheel_axis = *(float *)&FLOAT_0_0;
        v9 = Abs(&sign);
        if ( (*(unsigned int *)(*(unsigned int *)(LODWORD(max_delta_angle) + 4 * k + 984) + 176) & 8) != 0 )
        {
          if ( *(float *)(LODWORD(max_delta_angle) + 1032) < 0.0 )
            v8 = FLOAT_N1_0;
          else
            v8 = FLOAT_1_0;
          if ( v9 <= 0.001 )
          {
            sign = gWheelAxisLoc.x;
            y = gWheelAxisLoc.y;
            nwheel_axis = gWheelAxisLoc.z;
          }
          else
          {
            sign = sign * (float)(v8 / v9);
            y = y * (float)(v8 / v9);
            nwheel_axis = nwheel_axis * (float)(v8 / v9);
          }
          if ( *(float *)(LODWORD(max_delta_angle) + 508) > 0.1
            && (*(unsigned int *)(*(unsigned int *)(LODWORD(max_delta_angle) + 4 * k + 984) + 176) & 0x80) != 0 )
          {
            sign = gWheelAxisLoc.x;
            y = gWheelAxisLoc.y;
            nwheel_axis = gWheelAxisLoc.z;
          }
          v7 = (float *)(*(unsigned int *)(LODWORD(max_delta_angle) + 4 * k + 984) + 80);
          *v7 = sign;
          v7[1] = y;
          v7[2] = nwheel_axis;
        }
        v6 = v9 / v33;
        if ( (*(unsigned int *)(*(unsigned int *)(LODWORD(max_delta_angle) + 4 * k + 984) + 176) & 0x10) != 0 )
        {
          ++LODWORD(wheel_ratios[4]);
          *(float *)&v21[k] = v6;
          wheel_ratios[3] = wheel_ratios[3] + v6;
        }
      }
    }
    v5 = (float)SLODWORD(wheel_ratios[4]) / wheel_ratios[3];
    for ( m = 0; m < 6; ++m )
    {
      if ( *(unsigned int *)(LODWORD(max_delta_angle) + 4 * m + 984) )
      {
        *(unsigned int *)(*(unsigned int *)(LODWORD(max_delta_angle) + 4 * m + 984) + 132) = v21[m];
        *(float *)(*(unsigned int *)(LODWORD(max_delta_angle) + 4 * m + 984) + 136) = *(float *)&v21[m] * v5;
      }
    }
  }
  else
  {
    for ( n = 0; n < 6; ++n )
    {
      if ( *(unsigned int *)(LODWORD(max_delta_angle) + 4 * n + 984) )
      {
        v48 = *(unsigned int *)(LODWORD(max_delta_angle) + 4 * n + 984);
        v47 = v48 + 80;
        *(float *)(v48 + 80) = gWheelAxisLoc.x;
        *(float *)(v47 + 4) = gWheelAxisLoc.y;
        *(float *)(v47 + 8) = gWheelAxisLoc.z;
        v46 = *(unsigned int *)(LODWORD(max_delta_angle) + 4 * n + 984);
        *(float *)(v46 + 132) = FLOAT_1_0;
        v45 = *(unsigned int *)(LODWORD(max_delta_angle) + 4 * n + 984);
        *(float *)(v45 + 136) = FLOAT_1_0;
      }
    }
  }
}

void __thiscall NitrousVehicle::update_braking_and_acceleration(NitrousVehicle *this, float __formal)
{
  rigid_body_constraint_wheel *rbc_wheel; // [esp+Ch] [ebp-8h]
  int i; // [esp+10h] [ebp-4h]

  for ( i = 0; i < 6; ++i )
  {
    rbc_wheel = this->m_wheels[i];
    if ( rbc_wheel )
    {
      if ( (this->m_state_flags & 1) != 0 && (rbc_wheel->m_wheel_flags & 0x20) != 0 )
      {
        rigid_body_constraint_wheel::set_wheel_state_braking(rbc_wheel, this->m_power_braking_factor);
      }
      else if ( (this->m_state_flags & 2) != 0 && (rbc_wheel->m_wheel_flags & 0x40) != 0 )
      {
        rigid_body_constraint_wheel::set_wheel_state_braking(rbc_wheel, this->m_braking_factor);
      }
      else if ( (this->m_state_flags & 4) != 0 && (rbc_wheel->m_wheel_flags & 0x10) != 0 )
      {
        rigid_body_constraint_wheel::set_wheel_state_accelerating(
          rbc_wheel,
          this->m_desired_speed_factor / this->m_reference_wheel_radius,
          this->m_acceleration_factor);
      }
      else if ( (this->m_state_flags & 8) != 0 && (rbc_wheel->m_wheel_flags & 0x10) != 0 )
      {
        rigid_body_constraint_wheel::set_wheel_state_accelerating(
          rbc_wheel,
          COERCE_FLOAT(LODWORD(this->m_desired_speed_factor) ^ _mask__NegFloat_) / this->m_reference_wheel_radius,
          this->m_acceleration_factor);
      }
      else if ( (this->m_state_flags & 0x10) != 0 )
      {
        rigid_body_constraint_wheel::set_wheel_state_braking(rbc_wheel, this->m_coasting_factor);
      }
      else
      {
        rigid_body_constraint_wheel::set_wheel_state_braking(rbc_wheel, 0.0);
      }
    }
  }
}

phys_vec3 *__cdecl phys_cross(phys_vec3 *result, const phys_vec3 *a, const phys_vec3 *b)
{
  float v4; // [esp+4h] [ebp-8h]
  float v5; // [esp+8h] [ebp-4h]

  v4 = (float)(a->z * b->x) - (float)(a->x * b->z);
  v5 = (float)(a->x * b->y) - (float)(a->y * b->x);
  result->x = (float)(a->y * b->z) - (float)(a->z * b->y);
  result->y = v4;
  result->z = v5;
  return result;
}

void __thiscall NitrousVehicle::set_boosting(NitrousVehicle *this, bool boosting)
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

bool __thiscall NitrousVehicle::is_boosting(NitrousVehicle *this)
{
  return (this->m_flags & 0x800) != 0;
}

double __thiscall NitrousVehicle::get_maximum_acceleration_factor(NitrousVehicle *this)
{
  return 2.0;
}

double __thiscall NitrousVehicle::get_maximum_acceleration(NitrousVehicle *this)
{
  float maximum_acceleration; // [esp+4h] [ebp-4h]

  maximum_acceleration = this->m_parameter->m_accel_max;
  if ( NitrousVehicle::is_boosting(this) )
    return (float)(maximum_acceleration * this->m_vehicle_info->boostAccelMultiplier);
  return maximum_acceleration;
}

double __thiscall NitrousVehicle::get_max_speed(NitrousVehicle *this, bool include_perks)
{
  float max_speed; // [esp+4h] [ebp-4h]

  max_speed = this->m_parameter->m_speed_max;
  if ( NitrousVehicle::is_boosting(this) )
    return (float)(max_speed * this->m_vehicle_info->boostSpeedIncrease);
  return max_speed;
}

void __thiscall NitrousVehicle::update_control_from_network(NitrousVehicle *this, LerpEntityStateVehicle *vehicleState)
{
  float v2; // [esp+4h] [ebp-10h]
  float steerYaw; // [esp+10h] [ebp-4h]

  if ( this->m_phys_user_data )
  {
    NitrousVehicle::set_throttle(this, (float)vehicleState->throttle * 0.000030517578);
    if ( (float)(vehicleState->steerYaw - 1.0) < 0.0 )
      steerYaw = vehicleState->steerYaw;
    else
      steerYaw = FLOAT_1_0;
    if ( (float)(-1.0 - vehicleState->steerYaw) < 0.0 )
      v2 = steerYaw;
    else
      v2 = FLOAT_N1_0;
    this->m_steer_factor = v2;
    this->mVehicleController.m_cmd_valid = 0;
  }
}

// local variable allocation has failed, the output may be wrong!
void __userpurge NitrousVehicle::update_from_network(
        NitrousVehicle *this@<ecx>,
        phys_vec3 *a2@<ebp>,
        trajectory_t pos,
        trajectory_t apos,
        LerpEntityStateVehicle *vehicleState,
        float timeDifference)
{
  const phys_vec3 *v6; // eax
  double v7; // st7
  float v8[3]; // [esp+10h] [ebp-294h] BYREF
  float currentAnglesError[3]; // [esp+1Ch] [ebp-288h] BYREF
  float predictedAngles[3]; // [esp+28h] [ebp-27Ch] BYREF
  float predictedPhysAxis[3][3]; // [esp+34h] [ebp-270h] BYREF
  float localPhysicsAngles[3]; // [esp+58h] [ebp-24Ch] BYREF
  phys_mat44 rv; // [esp+64h] [ebp-240h] BYREF
  float currentOriginError[3]; // [esp+A4h] [ebp-200h] BYREF
  float predictedPos[3]; // [esp+B0h] [ebp-1F4h]
  int v16; // [esp+BCh] [ebp-1E8h]
  int v17; // [esp+C0h] [ebp-1E4h]
  int v18; // [esp+C4h] [ebp-1E0h]
  int v19; // [esp+C8h] [ebp-1DCh]
  int v20; // [esp+CCh] [ebp-1D8h]
  int v21; // [esp+D0h] [ebp-1D4h]
  int *v22; // [esp+D4h] [ebp-1D0h]
  float *v23; // [esp+D8h] [ebp-1CCh]
  float *v24; // [esp+DCh] [ebp-1C8h]
  float *v25; // [esp+E0h] [ebp-1C4h]
  bool v26; // [esp+E7h] [ebp-1BDh]
  phys_vec3 v27; // [esp+E8h] [ebp-1BCh] BYREF
  phys_vec3 v28; // [esp+F8h] [ebp-1ACh] BYREF
  float v29; // [esp+114h] [ebp-190h]
  float v30[2]; // [esp+118h] [ebp-18Ch] BYREF
  float acceptablePosError; // [esp+120h] [ebp-184h]
  phys_vec3 networkPredictedRBPosition; // [esp+124h] [ebp-180h]
  float v33; // [esp+13Ch] [ebp-168h]
  float v34; // [esp+140h] [ebp-164h]
  float v35; // [esp+144h] [ebp-160h]
  float v36; // [esp+148h] [ebp-15Ch]
  float v37; // [esp+14Ch] [ebp-158h]
  float v38; // [esp+150h] [ebp-154h]
  float v39; // [esp+158h] [ebp-14Ch]
  float v40; // [esp+15Ch] [ebp-148h]
  float v41; // [esp+160h] [ebp-144h]
  int v42; // [esp+164h] [ebp-140h]
  float v43; // [esp+168h] [ebp-13Ch] BYREF
  float v44; // [esp+16Ch] [ebp-138h]
  float v45; // [esp+170h] [ebp-134h]
  _BYTE networkVel_12[12]; // [esp+180h] [ebp-124h] OVERLAPPED BYREF
  float localPhysAxis[3][3]; // [esp+18Ch] [ebp-118h] BYREF
  phys_vec3 phys_axis[3]; // [esp+1B4h] [ebp-F0h] BYREF
  int v49; // [esp+1E4h] [ebp-C0h]
  float v50; // [esp+1E8h] [ebp-BCh] BYREF
  float v51; // [esp+1ECh] [ebp-B8h]
  float v52; // [esp+1F0h] [ebp-B4h]
  float v53; // [esp+204h] [ebp-A0h]
  float v54; // [esp+208h] [ebp-9Ch]
  float v55; // [esp+20Ch] [ebp-98h]
  float networkAngles[3]; // [esp+210h] [ebp-94h] BYREF
  float networkRBPos[3]; // [esp+21Ch] [ebp-88h] BYREF
  float translation[3]; // [esp+228h] [ebp-7Ch] BYREF
  float networkAxisTranspose[3][3]; // [esp+234h] [ebp-70h] BYREF
  float localTrans[3]; // [esp+258h] [ebp-4Ch] BYREF
  float networkAxis[3][3]; // [esp+264h] [ebp-40h] BYREF
  float localPos[3]; // [esp+288h] [ebp-1Ch]
  phys_vec3 *aVelocity; // [esp+294h] [ebp-10h]
  phys_vec3 *velocity; // [esp+298h] [ebp-Ch] BYREF
  phys_mat44 *mat; // [esp+29Ch] [ebp-8h]
  phys_mat44 *retaddr; // [esp+2A4h] [ebp+0h]

  velocity = a2;
  mat = retaddr;
  aVelocity = (phys_vec3 *)this;
  if ( this->m_phys_user_data )
  {
    NitrousVehicle::update_control_from_network((NitrousVehicle *)aVelocity, vehicleState);
    LODWORD(localPos[2]) = *(unsigned int *)LODWORD(aVelocity[35].x) + 48;
    LODWORD(localPos[1]) = *(unsigned int *)LODWORD(aVelocity[35].x) + 144;
    LODWORD(localPos[0]) = *(unsigned int *)LODWORD(aVelocity[35].x) + 160;
    Phys_NitrousVecToVec3((const phys_vec3 *)(LODWORD(localPos[2]) + 48), networkAxis[2]);
    AnglesToAxis(apos.trBase, (float (*)[3])localTrans);
    Phys_NitrousVecToVec3((const phys_vec3 *)(LODWORD(aVelocity[35].x) + 80), networkAxisTranspose[2]);
    MatrixTranspose((const float (*)[3])localTrans, (float (*)[3])translation);
    MatrixVecMultiply((const float (*)[3])translation, networkAxisTranspose[2], networkRBPos);
    networkAngles[0] = pos.trBase[0] - networkRBPos[0];
    networkAngles[1] = pos.trBase[1] - networkRBPos[1];
    networkAngles[2] = pos.trBase[2] - networkRBPos[2];
    v53 = apos.trBase[0];
    v54 = apos.trBase[1];
    v55 = apos.trBase[2];
    Phys_Vec3ToNitrousVec(networkAngles, (phys_vec3 *)&v50);
    v49 = 3;
    for ( LODWORD(localPhysAxis[2][0]) = &localPhysAxis[2][1]; --v49 >= 0; LODWORD(localPhysAxis[2][0]) += 16 )
      ;
    Phys_Vec3ToNitrousVec(localTrans, (phys_vec3 *)&localPhysAxis[2][1]);
    Phys_Vec3ToNitrousVec(networkAxis[0], (phys_vec3 *)&phys_axis[0].y);
    Phys_Vec3ToNitrousVec(networkAxis[1], (phys_vec3 *)&phys_axis[1].y);
    Phys_NitrousMat44ToVec33((const phys_mat44 *)LODWORD(localPos[2]), (float (*)[3])networkVel_12);
    Phys_Vec3ToNitrousVec(pos.trDelta, (phys_vec3 *)&v43);
    v42 = LODWORD(timeDifference) & _mask__AbsFloat_;
    if ( timeCap > COERCE_FLOAT(LODWORD(timeDifference) & _mask__AbsFloat_) )
    {
      v41 = timeDifference * v43;
      v40 = timeDifference * v44;
      v39 = timeDifference * v45;
      v36 = timeDifference * v43;
      v37 = timeDifference * v44;
      v38 = timeDifference * v45;
      v35 = v50 + (float)(timeDifference * v43);
      v34 = v51 + (float)(timeDifference * v44);
      v33 = v52 + (float)(timeDifference * v45);
      networkPredictedRBPosition.y = v35;
      networkPredictedRBPosition.z = v34;
      networkPredictedRBPosition.w = v33;
      v30[0] = v35;
      v30[1] = v34;
      acceptablePosError = v33;
      if ( !CompareVec3ToPhysVec(pos.trDelta, (const phys_vec3 *)LODWORD(localPos[1]), acceptableVelError) )
        Phys_Vec3ToNitrousVec(pos.trDelta, (phys_vec3 *)LODWORD(localPos[1]));
      if ( !CompareVec3ToPhysVec(apos.trDelta, (const phys_vec3 *)LODWORD(localPos[0]), acceptableAVelError) )
        Phys_Vec3ToNitrousVec(apos.trDelta, (phys_vec3 *)LODWORD(localPos[0]));
      v29 = acceptablePosErrorBase;
      v6 = operator-(&v28, (const phys_vec3 *)v30, (const phys_vec3 *)(LODWORD(localPos[2]) + 48));
      phys_vec3::operator=(&v27, v6);
      v7 = AbsSquared(&v27);
      v26 = v29 * v29 > v7;
      if ( v29 * v29 <= v7 )
      {
        Phys_Vec3ToNitrousVec(pos.trDelta, (phys_vec3 *)LODWORD(localPos[1]));
        Phys_Vec3ToNitrousVec(apos.trDelta, (phys_vec3 *)LODWORD(localPos[0]));
        phys_vec3::operator=((phys_vec3 *)LODWORD(localPos[2]), (const phys_vec3 *)&localPhysAxis[2][1]);
        phys_vec3::operator=((phys_vec3 *)(LODWORD(localPos[2]) + 16), (phys_vec3 *)&phys_axis[0].y);
        phys_vec3::operator=((phys_vec3 *)(LODWORD(localPos[2]) + 32), (phys_vec3 *)&phys_axis[1].y);
        phys_vec3::operator=((phys_vec3 *)(LODWORD(localPos[2]) + 48), (const phys_vec3 *)v30);
        v25 = *(float **)LODWORD(aVelocity[35].x);
        *v25 = v50;
        v25[1] = v51;
        v25[2] = v52;
        v24 = *(float **)LODWORD(aVelocity[35].x);
        v23 = *(float **)LODWORD(aVelocity[35].x);
        if ( *v24 != *v23 )
          goto LABEL_19;
        v22 = *(int **)LODWORD(aVelocity[35].x);
        v21 = *v22;
        if ( COERCE_FLOAT(v21 & _mask__AbsFloat_) > 100000.0 )
          goto LABEL_19;
        v20 = *(unsigned int *)LODWORD(aVelocity[35].x);
        v19 = *(unsigned int *)LODWORD(aVelocity[35].x);
        if ( *(float *)(v20 + 4) != *(float *)(v19 + 4)
          || (v18 = *(unsigned int *)LODWORD(aVelocity[35].x),
              v17 = *(unsigned int *)(v18 + 4),
              COERCE_FLOAT(v17 & _mask__AbsFloat_) > 100000.0)
          || (v16 = *(unsigned int *)LODWORD(aVelocity[35].x),
              predictedPos[2] = *(float *)LODWORD(aVelocity[35].x),
              *(float *)(v16 + 8) != *(float *)(LODWORD(predictedPos[2]) + 8))
          || (predictedPos[1] = *(float *)LODWORD(aVelocity[35].x),
              predictedPos[0] = *(float *)(LODWORD(predictedPos[1]) + 8),
              COERCE_FLOAT(LODWORD(predictedPos[0]) & _mask__AbsFloat_) > 100000.0) )
        {
LABEL_19:
          phys_exec_debug_callback(*(void **)LODWORD(aVelocity[35].x));
        }
        Phys_NitrousVecToVec3((const phys_vec3 *)v30, currentOriginError);
        Vec3Sub(networkAxis[2], currentOriginError, &rv.w.y);
        Vec3Add(&rv.w.y, &aVelocity[57].y, &aVelocity[57].y);
        XAnimClientNotifyList::GetNotifyList((XAnimClientNotifyList *)localPhysicsAngles);
        make_rotate(
          (int)&velocity,
          (phys_mat44 *)localPhysicsAngles,
          (const phys_vec3 *)LODWORD(localPos[0]),
          timeDifference,
          1000.0);
        phys_multiply_mat(
          (int)&velocity,
          (phys_mat44 *)LODWORD(localPos[2]),
          (const phys_mat44 *)localPhysicsAngles,
          (const phys_mat44 *)LODWORD(localPos[2]));
        AxisToAngles((const float (*)[3])networkVel_12, predictedPhysAxis[2]);
        Phys_NitrousMat44ToVec33((const phys_mat44 *)LODWORD(localPos[2]), (float (*)[3])predictedAngles);
        AxisToAngles((const float (*)[3])predictedAngles, currentAnglesError);
        AnglesSubtract(predictedPhysAxis[2], currentAnglesError, v8);
        Vec3Add(v8, &aVelocity[58].x, &aVelocity[58].x);
        if ( phys_drawNitrousVehicle->current.enabled )
        {
          G_DebugStar(networkAngles, colorGreen, duration);
          G_DebugStar(currentOriginError, colorYellow, duration);
          G_DebugStar(networkAxis[2], colorRed, duration);
          CG_DebugLine(networkAngles, currentOriginError, colorWhite, 1, duration);
          CG_DebugLine(currentOriginError, networkAxis[2], colorWhite, 1, duration);
        }
      }
    }
  }
}

double __cdecl AbsSquared(const phys_vec3 *a)
{
  return a->x * a->x + a->y * a->y + a->z * a->z;
}

// local variable allocation has failed, the output may be wrong!
void  phys_multiply_mat(int a1@<ebp>, phys_mat44 *dest, const phys_mat44 *left, const phys_mat44 *right)
{
  _BYTE v4[76]; // [esp-Ch] [ebp-ACh] OVERLAPPED BYREF
  const phys_vec3 *v5; // [esp+40h] [ebp-60h]
  phys_vec3 v6; // [esp+44h] [ebp-5Ch] BYREF
  phys_vec3 *p_y; // [esp+5Ch] [ebp-44h]
  const phys_vec3 *v8; // [esp+60h] [ebp-40h]
  phys_vec3 v9; // [esp+64h] [ebp-3Ch] BYREF
  const phys_vec3 *v10; // [esp+80h] [ebp-20h]
  phys_vec3 v11; // [esp+84h] [ebp-1Ch] BYREF
  int v12; // [esp+94h] [ebp-Ch]
  void *v13; // [esp+98h] [ebp-8h]
  void *retaddr; // [esp+A0h] [ebp+0h]

  v12 = a1;
  v13 = retaddr;
  if ( dest == left )
  {
    memcpy(v4, left, 0x40u);
    phys_multiply_mat(dest, (const phys_mat44 *)v4, right);
  }
  else
  {
    v10 = phys_multiply(&v11, left, &right->x);
    dest->x.x = v10->x;
    dest->x.y = v10->y;
    dest->x.z = v10->z;
    v8 = phys_multiply(&v9, left, &right->y);
    p_y = &dest->y;
    dest->y.x = v8->x;
    p_y->y = v8->y;
    p_y->z = v8->z;
    v5 = phys_multiply(&v6, left, &right->z);
    *(unsigned int *)&v4[72] = &dest->z;
    dest->z.x = v5->x;
    *(float *)(*(unsigned int *)&v4[72] + 4) = v5->y;
    *(float *)(*(unsigned int *)&v4[72] + 8) = v5->z;
  }
}

void  make_rotate(
        int a1@<ebp>,
        phys_mat44 *mat,
        const phys_vec3 *v,
        float theta_factor,
        float max_rotation_radians)
{
  long double x; // [esp+4h] [ebp-50h]
  float xa; // [esp+4h] [ebp-50h]
  long double x_4; // [esp+8h] [ebp-4Ch]
  unsigned int v8[3]; // [esp+18h] [ebp-3Ch] BYREF
  float v9; // [esp+34h] [ebp-20h]
  float v10; // [esp+38h] [ebp-1Ch]
  float v11; // [esp+3Ch] [ebp-18h]
  float v12; // [esp+40h] [ebp-14h]
  float ca; // [esp+44h] [ebp-10h]
  int v14; // [esp+48h] [ebp-Ch]
  void *v15; // [esp+4Ch] [ebp-8h]
  void *retaddr; // [esp+54h] [ebp+0h]

  v14 = a1;
  v15 = retaddr;
  ca = Abs(&v->x);
  if ( ca >= 0.0000099999997 )
  {
    v12 = 1.0 / ca;
    v11 = (float)(1.0 / ca) * v->x;
    v10 = (float)(1.0 / ca) * v->y;
    v9 = (float)(1.0 / ca) * v->z;
    *(float *)v8 = v11;
    *(float *)&v8[1] = v10;
    *(float *)&v8[2] = v9;
    ca = ca * theta_factor;
    if ( ca > max_rotation_radians )
      ca = max_rotation_radians;
    __libm_sse2_sin(x_4);
    *(float *)&x = ca;
    __libm_sse2_cos(x);
    make_rotate(mat, (const phys_vec3 *)v8, ca, xa);
  }
  else
  {
    phys_mat44::operator=(mat, &PHYS_IDENTITY_MATRIX_34);
  }
}

void __cdecl make_rotate(phys_mat44 *m, const phys_vec3 *u, float ca, float sa)
{
  float yy; // [esp+1Ch] [ebp-28h]
  float xsa; // [esp+20h] [ebp-24h]
  float xy; // [esp+24h] [ebp-20h]
  float zsa; // [esp+28h] [ebp-1Ch]
  float zz; // [esp+2Ch] [ebp-18h]
  float ysa; // [esp+34h] [ebp-10h]
  float xz; // [esp+38h] [ebp-Ch]
  float yz; // [esp+3Ch] [ebp-8h]

  xy = (float)(u->x * u->y) * (float)(1.0 - ca);
  xz = (float)(u->x * u->z) * (float)(1.0 - ca);
  yy = (float)(u->y * u->y) * (float)(1.0 - ca);
  yz = (float)(u->y * u->z) * (float)(1.0 - ca);
  zz = (float)(u->z * u->z) * (float)(1.0 - ca);
  xsa = u->x * sa;
  ysa = u->y * sa;
  zsa = u->z * sa;
  m->x.x = (float)((float)(u->x * u->x) * (float)(1.0 - ca)) + ca;
  m->y.x = xy - zsa;
  m->z.x = xz + ysa;
  m->w.x = *(float *)&FLOAT_0_0;
  *phys_vec3::operator[]<int>(&m->x, 1u) = xy + zsa;
  *phys_vec3::operator[]<int>(&m->y, 1u) = yy + ca;
  *phys_vec3::operator[]<int>(&m->z, 1u) = yz - xsa;
  *phys_vec3::operator[]<int>(&m->w, 1u) = *(float *)&FLOAT_0_0;
  *phys_vec3::operator[]<int>(&m->x, 2u) = xz - ysa;
  *phys_vec3::operator[]<int>(&m->y, 2u) = yz + xsa;
  *phys_vec3::operator[]<int>(&m->z, 2u) = zz + ca;
  *phys_vec3::operator[]<int>(&m->w, 2u) = *(float *)&FLOAT_0_0;
  phys_mat44::fix_w_column(m);
}

void __thiscall phys_mat44::fix_w_column(phys_mat44 *this)
{
  this->x.w = *(float *)&FLOAT_0_0;
  this->y.w = *(float *)&FLOAT_0_0;
  this->z.w = *(float *)&FLOAT_0_0;
  this->w.w = FLOAT_1_0;
}

void __cdecl Phys_NitrousMat44ToVec33(const phys_mat44 *inMat, float (*outAxis)[3])
{
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

bool __cdecl CompareVec3ToPhysVec(const float *a, const phys_vec3 *b, float acceptableError)
{
  float diff_4; // [esp+10h] [ebp-8h]
  float diff_8; // [esp+14h] [ebp-4h]

  diff_4 = b->y - a[1];
  diff_8 = b->z - a[2];
  return (float)(acceptableError * acceptableError) > (float)((float)((float)((float)(b->x - *a) * (float)(b->x - *a))
                                                                    + (float)(diff_4 * diff_4))
                                                            + (float)(diff_8 * diff_8));
}

void __thiscall NitrousVehicle::GetEntityPose(
        NitrousVehicle *this,
        float *origin,
        float *angles,
        int curTime,
        bool isDriver)
{
  float v5; // [esp+0h] [ebp-90h]
  float v6; // [esp+4h] [ebp-8Ch]
  float v8; // [esp+3Ch] [ebp-54h]
  float v9; // [esp+50h] [ebp-40h]
  float dt; // [esp+54h] [ebp-3Ch]
  float reduction[3]; // [esp+58h] [ebp-38h] BYREF
  float scale; // [esp+64h] [ebp-2Ch]
  float rbPosition[3]; // [esp+68h] [ebp-28h] BYREF
  float rbAngles[3]; // [esp+74h] [ebp-1Ch] BYREF
  float rbRotation[4]; // [esp+80h] [ebp-10h] BYREF

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
          v9 = errorReductionRate * dt;
        else
          v9 = FLOAT_1_0;
        if ( (float)(0.0 - (float)(errorReductionRate * dt)) < 0.0 )
          v6 = v9;
        else
          v6 = *(float *)&FLOAT_0_0;
        scale = v6;
        reduction[0] = v6 * this->m_networkErrorOrigin[0];
        reduction[1] = v6 * this->m_networkErrorOrigin[1];
        reduction[2] = v6 * this->m_networkErrorOrigin[2];
        this->m_networkErrorOrigin[0] = this->m_networkErrorOrigin[0] - reduction[0];
        this->m_networkErrorOrigin[1] = this->m_networkErrorOrigin[1] - reduction[1];
        this->m_networkErrorOrigin[2] = this->m_networkErrorOrigin[2] - reduction[2];
        if ( (float)((float)(errorAReductionRate * dt) - 1.0) < 0.0 )
          v8 = errorAReductionRate * dt;
        else
          v8 = FLOAT_1_0;
        if ( (float)(0.0 - (float)(errorAReductionRate * dt)) < 0.0 )
          v5 = v8;
        else
          v5 = *(float *)&FLOAT_0_0;
        scale = v5;
        reduction[0] = v5 * this->m_networkErrorAngles[0];
        reduction[1] = v5 * this->m_networkErrorAngles[1];
        reduction[2] = v5 * this->m_networkErrorAngles[2];
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
    if ( ((*(unsigned int *)origin & 0x7F800000) == 0x7F800000
       || ((unsigned int)origin[1] & 0x7F800000) == 0x7F800000
       || ((unsigned int)origin[2] & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
            2369,
            0,
            "%s",
            "!IS_NAN((origin)[0]) && !IS_NAN((origin)[1]) && !IS_NAN((origin)[2])") )
    {
      __debugbreak();
    }
    if ( ((*(unsigned int *)angles & 0x7F800000) == 0x7F800000
       || ((unsigned int)angles[1] & 0x7F800000) == 0x7F800000
       || ((unsigned int)angles[2] & 0x7F800000) == 0x7F800000)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.cpp",
            2370,
            0,
            "%s",
            "!IS_NAN((angles)[0]) && !IS_NAN((angles)[1]) && !IS_NAN((angles)[2])") )
    {
      __debugbreak();
    }
  }
  else
  {
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
  }
}

void __cdecl G_SetVehDriverInputs(int localClientNum, int vehEntNum, usercmd_s *cmd)
{
  float *inVector; // [esp+8h] [ebp-D4h]
  float axis[9]; // [esp+10h] [ebp-CCh] BYREF
  float *m_networkErrorAngles; // [esp+34h] [ebp-A8h]
  float *v6; // [esp+38h] [ebp-A4h]
  float *m_networkErrorOrigin; // [esp+60h] [ebp-7Ch]
  float *v8; // [esp+64h] [ebp-78h]
  float *origin; // [esp+68h] [ebp-74h]
  NitrousVehicle *nitrousVehicle; // [esp+6Ch] [ebp-70h]
  NitrousVehicleController *p_mVehicleController; // [esp+70h] [ebp-6Ch]
  phys_vec3 *velocity; // [esp+74h] [ebp-68h]
  phys_vec3 *aVelocity; // [esp+78h] [ebp-64h]
  float localPos[3]; // [esp+7Ch] [ebp-60h] BYREF
  float currentOriginError[3]; // [esp+88h] [ebp-54h]
  phys_mat44 *mat; // [esp+94h] [ebp-48h]
  float localPhysAxis[3][3]; // [esp+98h] [ebp-44h] BYREF
  float localAngles[3]; // [esp+BCh] [ebp-20h] BYREF
  float currentAnglesError[3]; // [esp+C8h] [ebp-14h] BYREF
  gentity_s *ent; // [esp+D4h] [ebp-8h]
  gentity_s *self; // [esp+D8h] [ebp-4h]

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
      if ( EntHandle::isDefined(&self->r.ownerNum) )
      {
        ent = EntHandle::ent(&self->r.ownerNum);
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
          v8 = self->scr_vehicle->nitrousVehicle->m_networkErrorOrigin;
          *m_networkErrorOrigin = currentOriginError[0] + *v8;
          m_networkErrorOrigin[1] = currentOriginError[1] + v8[1];
          m_networkErrorOrigin[2] = currentOriginError[2] + v8[2];
          Phys_NitrousMat44ToVec33(mat, localPhysAxis);
          AxisToAngles(localPhysAxis, localAngles);
          AnglesSubtract(localAngles, svs.clients[ent->s.clientNum].header.vehicle.angles, currentAnglesError);
          m_networkErrorAngles = self->scr_vehicle->nitrousVehicle->m_networkErrorAngles;
          v6 = self->scr_vehicle->nitrousVehicle->m_networkErrorAngles;
          *m_networkErrorAngles = currentAnglesError[0] + *v6;
          m_networkErrorAngles[1] = currentAnglesError[1] + v6[1];
          m_networkErrorAngles[2] = currentAnglesError[2] + v6[2];
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

void __cdecl G_ClearVehicleInputs()
{
  phys_free_list<NitrousVehicle>::T_internal_base *i; // [esp+24h] [ebp-8h]

  if ( g_rb_vehicle_list.m_list_count > 0 )
  {
    //PIXBeginNamedEvent(-1, "G_ClearVehicleInputs");
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
    if ( g_DXDeviceThread == GetCurrentThreadId() )
      D3DPERF_EndEvent();
  }
}

void __cdecl CG_SetVehDriverInputs(int localClientNum, int vehEntNum, usercmd_s *cmd)
{
  NitrousVehicle *nitrousVeh; // [esp+8h] [ebp-14h]
  centity_s *self; // [esp+18h] [ebp-4h]

  self = CG_GetEntity(localClientNum, vehEntNum);
  if ( self->nitrousVeh )
  {
    nitrousVeh = self->nitrousVeh;
    memcpy(&nitrousVeh->mVehicleController.m_cmd, cmd, sizeof(nitrousVeh->mVehicleController.m_cmd));
    nitrousVeh->mVehicleController.m_cmd_local_client_num = localClientNum;
    nitrousVeh->mVehicleController.m_cmd_valid = 1;
  }
}

void __cdecl Vehicle_Launch(int localClientNum, centity_s *cent, float *hitp, const float *force, bool relative)
{
  float value; // [esp+4h] [ebp-14h]
  float scaledForce[3]; // [esp+Ch] [ebp-Ch] BYREF
  int savedregs; // [esp+18h] [ebp+0h] BYREF

  if ( !localClientNum && cent->nitrousVeh )
  {
    if ( cent->nitrousVeh->m_phys_user_data )
    {
      value = dynEnt_bulletForce->current.value;
      scaledForce[0] = value * *force;
      scaledForce[1] = value * force[1];
      scaledForce[2] = value * force[2];
      Phys_ObjAddForce((int)&savedregs, (int)cent->nitrousVeh->m_phys_user_data, hitp, scaledForce, relative);
    }
  }
}

NitrousVehicle *__thiscall phys_free_list<NitrousVehicle>::add(
        phys_free_list<NitrousVehicle> *this,
        int no_error,
        const char *error_msg)
{
  phys_free_list<NitrousVehicle>::T_internal *ptr; // [esp+20h] [ebp-4h]

  ptr = (phys_free_list<NitrousVehicle>::T_internal *)PMM_ALLOC(0x460u, 0x10u);
  if ( ptr )
  {
    NitrousVehicle::NitrousVehicle(&ptr->m_data);
    ptr->m_prev_T_internal = &this->m_dummy_head;
    ptr->m_next_T_internal = this->m_dummy_head.m_next_T_internal;
    this->m_dummy_head.m_next_T_internal->m_prev_T_internal = ptr;
    this->m_dummy_head.m_next_T_internal = ptr;
    ++this->m_list_count;
    phys_free_list<NitrousVehicle>::debug_add(this, ptr);
    return &ptr->m_data;
  }
  else
  {
    if ( !no_error )
    {
      if ( _tlAssert("c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h", 470, "no_error", error_msg) )
        __debugbreak();
    }
    return 0;
  }
}

void __thiscall phys_free_list<NitrousVehicle>::remove(phys_free_list<NitrousVehicle> *this, NitrousVehicle *data_)
{
  if ( data_ )
  {
    PMM_VALIDATE((char *)&data_[-1].m_steer_front_back_length, 0x460u, 0x10u);
    phys_free_list<NitrousVehicle>::remove(
      this,
      (phys_free_list<NitrousVehicle>::T_internal *)&data_[-1].m_steer_front_back_length);
  }
}

void __thiscall phys_free_list<NitrousVehicle>::debug_add(
        phys_free_list<NitrousVehicle> *this,
        phys_free_list<NitrousVehicle>::T_internal *T_i)
{
  int m_list_count; // [esp+0h] [ebp-10h]

  if ( this->m_list_count_high_water <= this->m_list_count )
    m_list_count = this->m_list_count;
  else
    m_list_count = this->m_list_count_high_water;
  this->m_list_count_high_water = m_list_count;
  if ( this->m_ptr_list_count >= 256 )
  {
    T_i->m_ptr_list_index = -1;
  }
  else
  {
    T_i->m_ptr_list_index = this->m_ptr_list_count;
    this->m_ptr_list[this->m_ptr_list_count++] = &T_i->m_data;
  }
}

void __thiscall phys_free_list<NitrousVehicle>::remove(
        phys_free_list<NitrousVehicle> *this,
        phys_free_list<NitrousVehicle>::T_internal *data)
{
  phys_free_list<NitrousVehicle>::T_internal_base *next; // [esp+14h] [ebp-8h]
  phys_free_list<NitrousVehicle>::T_internal_base *prev; // [esp+18h] [ebp-4h]

  if ( !data
    && _tlAssert("c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h", 477, "data", &toastPopupTitle) )
  {
    __debugbreak();
  }
  --this->m_list_count;
  phys_free_list<NitrousVehicle>::debug_remove(this, data);
  next = data->m_next_T_internal;
  prev = data->m_prev_T_internal;
  prev->m_next_T_internal = next;
  next->m_prev_T_internal = prev;
  PMM_FREE((unsigned __int8 *)data, 0x460u, 0x10u);
}

void __thiscall phys_free_list<NitrousVehicle>::debug_remove(
        phys_free_list<NitrousVehicle> *this,
        phys_free_list<NitrousVehicle>::T_internal *T_i)
{
  if ( T_i->m_ptr_list_index != -1 )
  {
    if ( T_i->m_ptr_list_index >= 0x100u
      && _tlAssert(
           "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
           421,
           "T_i->m_ptr_list_index >= 0 && T_i->m_ptr_list_index < PTR_LIST_SIZE",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    if ( this->m_ptr_list[T_i->m_ptr_list_index] != &T_i->m_data )
    {
      if ( _tlAssert(
             "c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h",
             422,
             "m_ptr_list[T_i->m_ptr_list_index] == &T_i->m_data",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
    }
    this->m_ptr_list[--this->m_ptr_list_count][1].m_wheel_state[0].m_state = T_i->m_ptr_list_index;
    this->m_ptr_list[T_i->m_ptr_list_index] = this->m_ptr_list[this->m_ptr_list_count];
  }
}

void  make_rotate(float a1@<ebp>, phys_mat44 *mat, const phys_vec3 *v1, const phys_vec3 *v2)
{
  float v4; // [esp+8h] [ebp-2Ch]
  float v5; // [esp+Ch] [ebp-28h]
  float len; // [esp+14h] [ebp-20h]
  float lena; // [esp+14h] [ebp-20h]
  float co_; // [esp+18h] [ebp-1Ch] BYREF
  float v9; // [esp+1Ch] [ebp-18h]
  float si_; // [esp+20h] [ebp-14h]
  phys_vec3 ud; // [esp+24h] [ebp-10h]
  float retaddr; // [esp+34h] [ebp+0h]

  ud.y = a1;
  ud.z = retaddr;
  phys_cross((phys_vec3 *)&co_, v1, v2);
  len = (float)((float)(co_ * co_) + (float)(v9 * v9)) + (float)(si_ * si_);
  if ( len >= 0.0000099999997 )
  {
    lena = fsqrt(len);
    co_ = co_ * (float)(1.0 / lena);
    v9 = v9 * (float)(1.0 / lena);
    si_ = si_ * (float)(1.0 / lena);
    v5 = (float)((float)(v1->x * v2->x) + (float)(v1->y * v2->y)) + (float)(v1->z * v2->z);
    v4 = fsqrt((float)(v5 * v5) + (float)(lena * lena));
    make_rotate(mat, (const phys_vec3 *)&co_, v5 / v4, lena / v4);
  }
  else
  {
    phys_mat44::operator=(mat, &PHYS_IDENTITY_MATRIX_44);
  }
}

phys_mat44 *__thiscall phys_mat44::phys_mat44(
        phys_mat44 *this,
        const phys_vec3 *x_,
        const phys_vec3 *y_,
        const phys_vec3 *z_,
        const phys_vec3 *w_)
{
  phys_mat44 *result; // eax

  result = this;
  this->x = *x_;
  this->y = *y_;
  this->z = *z_;
  this->w = *w_;
  return result;
}

