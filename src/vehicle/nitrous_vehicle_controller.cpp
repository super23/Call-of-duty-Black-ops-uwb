#include "nitrous_vehicle_controller.h"

// local variable allocation has failed, the output may be wrong!
void __userpurge NitrousVehicleController::UpdateScriptVehicleControl(
        NitrousVehicleController *this@<ecx>,
        int a2@<ebp>,
        NitrousVehicle *rbveh,
        float delta_t)
{
  double v4; // xmm0_8
  double max_speed; // st7
  long double v6; // [esp+Ch] [ebp-ECh]
  long double v7; // [esp+14h] [ebp-E4h]
  float v8; // [esp+18h] [ebp-E0h]
  float v9; // [esp+1Ch] [ebp-DCh]
  float v10; // [esp+2Ch] [ebp-CCh]
  float max_reverse_dist; // [esp+34h] [ebp-C4h]
  phys_vec3 *current_speed; // [esp+38h] [ebp-C0h]
  float v13; // [esp+60h] [ebp-98h]
  float v14; // [esp+68h] [ebp-90h]
  float v15[2]; // [esp+7Ch] [ebp-7Ch] BYREF
  float end[3]; // [esp+88h] [ebp-70h] BYREF
  __int128 start; // [esp+94h] [ebp-64h] OVERLAPPED
  float yaw; // [esp+A4h] [ebp-54h]
  float v19; // [esp+A8h] [ebp-50h]
  unsigned __int64 v20; // [esp+ACh] [ebp-4Ch] BYREF
  float to_goal_dist; // [esp+B4h] [ebp-44h]
  phys_vec3 to_goal; // [esp+B8h] [ebp-40h]
  unsigned int v23; // [esp+C8h] [ebp-30h]
  float v24; // [esp+CCh] [ebp-2Ch] BYREF
  float v25; // [esp+D0h] [ebp-28h]
  float v26; // [esp+D4h] [ebp-24h]
  phys_vec3 currentOrigin; // [esp+D8h] [ebp-20h]
  NitrousVehicleController *v28; // [esp+E8h] [ebp-10h]
  int v29; // [esp+ECh] [ebp-Ch] BYREF
  PhysObjUserData *userData; // [esp+F0h] [ebp-8h]
  PhysObjUserData *retaddr; // [esp+F8h] [ebp+0h]

  v29 = a2;
  userData = retaddr;
  v28 = this;
  LODWORD(currentOrigin.w) = rbveh->m_phys_user_data;
  LODWORD(currentOrigin.z) = *(unsigned int *)LODWORD(currentOrigin.w) + 48;
  v24 = *(float *)(LODWORD(currentOrigin.z) + 48);
  v25 = *(float *)(LODWORD(currentOrigin.z) + 52);
  v26 = *(float *)(LODWORD(currentOrigin.z) + 56);
  currentOrigin.x = *(float *)(LODWORD(currentOrigin.z) + 60);
  *(float *)&v23 = this->m_script_goal_position.x - v24;
  to_goal.w = this->m_script_goal_position.y - v25;
  to_goal.z = this->m_script_goal_position.z - v26;
  v20 = __PAIR64__(LODWORD(to_goal.w), v23);
  to_goal_dist = *(float *)&FLOAT_0_0;
  v19 = Abs((const float *)&v20);
  if ( v19 <= 0.0 )
  {
    DWORD1(start) = *(unsigned int *)&FLOAT_0_0;
  }
  else
  {
    yaw = 1.0 / v19;
    *(float *)&v20 = *(float *)&v20 * (float)(1.0 / v19);
    *((float *)&v20 + 1) = *((float *)&v20 + 1) * (float)(1.0 / v19);
    to_goal_dist = to_goal_dist * (float)(1.0 / v19);
    *((_QWORD *)&start + 1) = v20;
    v4 = *((float *)&v20 + 1);
    __libm_sse2_atan2(v6, v7);
    *(float *)&v4 = v4;
    *((float *)&start + 1) = (float)(*(float *)&v4 * 180.0) / 3.1415927;
    if ( *((float *)&start + 1) < 0.0 )
      *((float *)&start + 1) = *((float *)&start + 1) + 360.0;
  }
  *(float *)&start = v28->m_script_goal_speed + 10.0;
  if ( (rbveh->m_flags & 0x100) != 0 )
    *(float *)&start = v28->m_drive_path_speed;
  if ( phys_drawNitrousVehicle->current.enabled )
  {
    Phys_NitrousVecToVec3((const phys_vec3 *)&v24, end);
    Phys_NitrousVecToVec3(&v28->m_script_goal_position, v15);
    CL_AddDebugLine(end, v15, colorGreen, 0, 1);
  }
  LODWORD(v14) = COERCE_UNSIGNED_INT(
                   (float)((float)(*(float *)(*(unsigned int *)LODWORD(currentOrigin.w) + 64) * *(float *)&v20)
                         + (float)(*(float *)(*(unsigned int *)LODWORD(currentOrigin.w) + 68) * *((float *)&v20 + 1)))
                 + (float)(*(float *)(*(unsigned int *)LODWORD(currentOrigin.w) + 72) * to_goal_dist))
               & _mask__AbsFloat_;
  v13 = (float)((float)(*(float *)(*(unsigned int *)LODWORD(currentOrigin.w) + 48) * *(float *)&v20)
              + (float)(*(float *)(*(unsigned int *)LODWORD(currentOrigin.w) + 52) * *((float *)&v20 + 1)))
      + (float)(*(float *)(*(unsigned int *)LODWORD(currentOrigin.w) + 56) * to_goal_dist);
  if ( rbveh->m_script_brake == 0.0
    && *(float *)&start > 0.0
    && ((float)((float)((float)((float)(v24 - v28->m_stuck_position.x) * (float)(v24 - v28->m_stuck_position.x))
                      + (float)((float)(v25 - v28->m_stuck_position.y) * (float)(v25 - v28->m_stuck_position.y)))
              + (float)((float)(v26 - v28->m_stuck_position.z) * (float)(v26 - v28->m_stuck_position.z))) < 225.0
     || v28->m_stuck_time < 0.0) )
  {
    v28->m_stuck_time = v28->m_stuck_time + delta_t;
  }
  else
  {
    current_speed = &v28->m_stuck_position;
    v28->m_stuck_position.x = v24;
    current_speed->y = v25;
    current_speed->z = v26;
    v28->m_stuck_time = *(float *)&FLOAT_0_0;
  }
  max_reverse_dist = FLOAT_1_0;
  if ( rbveh->m_vehicle_info->type == 2 )
    max_reverse_dist = FLOAT_4_0;
  if ( v28->m_stuck_time > max_reverse_dist )
    v28->m_stuck_time = max_reverse_dist * -0.75;
  LODWORD(v10) = LODWORD(rbveh->m_forward_vel) & _mask__AbsFloat_;
  if ( rbveh->m_vehicle_info->type != 2 && v14 > 0.80000001 && v19 < 400.0 )
    goto LABEL_23;
  if ( v28->m_stuck_time >= 0.0 || v13 >= -0.5 )
  {
    if ( v28->m_stuck_time < 0.0 )
    {
LABEL_23:
      NitrousVehicleController::UpdateVehicleInputs(
        v28,
        (const PhysObjUserData *)&v29,
        rbveh,
        *((float *)&start + 1) + 180.0,
        1.0,
        -1.6);
      goto LABEL_36;
    }
    if ( v19 > 600.0 || v10 > 200.0 && v13 < -0.80000001 || (rbveh->m_flags & 0x100) != 0 )
      NitrousVehicleController::UpdateVehicleInputs(
        v28,
        (const PhysObjUserData *)&v29,
        rbveh,
        *((float *)&start + 1),
        1.0,
        4.0);
    else
      NitrousVehicleController::UpdateVehicleInputs(
        v28,
        (const PhysObjUserData *)&v29,
        rbveh,
        *((float *)&start + 1),
        1.0,
        0.0);
  }
  else if ( v19 <= 600.0 )
  {
    NitrousVehicleController::UpdateVehicleInputs(
      v28,
      (const PhysObjUserData *)&v29,
      rbveh,
      *((float *)&start + 1),
      1.0,
      1.0);
  }
  else
  {
    NitrousVehicleController::UpdateVehicleInputs(
      v28,
      (const PhysObjUserData *)&v29,
      rbveh,
      *((float *)&start + 1),
      1.0,
      -1.6);
  }
LABEL_36:
  if ( (rbveh->m_flags & 0x100) != 0 )
  {
    if ( v10 > *(float *)&start )
    {
      NitrousVehicle::set_throttle(rbveh, 0.0);
      NitrousVehicle::set_hand_brake(rbveh, 1.0);
    }
  }
  else if ( (float)(v28->m_script_goal_radius - 10.0) <= v19 )
  {
    if ( v19 >= 1000.0 || v10 <= *(float *)&start )
    {
      if ( v10 > *(float *)&start )
        NitrousVehicle::set_throttle(rbveh, 0.0);
    }
    else
    {
      v9 = v19 / 1000.0;
      max_speed = NitrousVehicle::get_max_speed(rbveh, 1);
      v8 = (max_speed - *(float *)&start) * v9 + *(float *)&start;
      if ( v10 <= v8 )
      {
        if ( v10 > *(float *)&start && v10 > 300.0 )
          NitrousVehicle::set_throttle(rbveh, 0.0);
      }
      else
      {
        NitrousVehicle::set_throttle(rbveh, 0.0);
        NitrousVehicle::set_hand_brake(rbveh, 1.0);
      }
    }
  }
  else if ( v28->m_stop_at_goal )
  {
    NitrousVehicle::set_throttle(rbveh, 0.0);
    NitrousVehicle::set_hand_brake(rbveh, 1.0);
    if ( v10 <= 1.0 )
      rbveh->m_flags &= ~4u;
  }
  else
  {
    rbveh->m_flags &= ~4u;
  }
}

void __thiscall NitrousVehicle::set_hand_brake(NitrousVehicle *this, float braking)
{
  if ( (braking < 0.0 || braking > 1.0)
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.h",
          447,
          0,
          "%s\n\t%s",
          "braking>= 0.0f && braking<= 1.0f",
          "Out of range") )
  {
    __debugbreak();
  }
  this->m_hand_brake = braking;
}

void __thiscall NitrousVehicleController::UpdateControlsStrafeMode(
        NitrousVehicleController *this,
        NitrousVehicle *rbveh)
{
  float v2; // [esp+Ch] [ebp-78h]
  float v3; // [esp+10h] [ebp-74h]
  float v4; // [esp+14h] [ebp-70h]
  float v5; // [esp+18h] [ebp-6Ch]
  float rb_angles[3]; // [esp+48h] [ebp-3Ch] BYREF
  cg_s *cgameGlob; // [esp+54h] [ebp-30h]
  gentity_s *driver; // [esp+58h] [ebp-2Ch]
  int driver_occupant; // [esp+5Ch] [ebp-28h]
  float camera_yaw; // [esp+60h] [ebp-24h]
  float move[2]; // [esp+64h] [ebp-20h]
  float forwardPreference; // [esp+6Ch] [ebp-18h]
  float vehicleDeadZone; // [esp+70h] [ebp-14h]
  float input_yaw; // [esp+74h] [ebp-10h]
  float input; // [esp+78h] [ebp-Ch]
  float vec[2]; // [esp+7Ch] [ebp-8h] BYREF
  int savedregs; // [esp+84h] [ebp+0h] BYREF

  camera_yaw = *(float *)&FLOAT_0_0;
  if ( rbveh->m_owner )
  {
    if ( !rbveh->m_owner->scr_vehicle || !rbveh->m_owner->r.inuse )
      return;
    driver = 0;
    driver_occupant = VEH_GetSeatOccupantEntNum(rbveh->m_owner, 0);
    if ( driver_occupant != 1023 )
      driver = &g_entities[driver_occupant];
    if ( driver && driver->client )
      camera_yaw = driver->client->ps.viewangles[1];
  }
  else
  {
    cgameGlob = CG_GetLocalClientGlobals(this->m_cmd_local_client_num);
    camera_yaw = cgameGlob->predictedPlayerState.viewangles[1];
  }
  if ( player_topDownCamMode->current.integer )
    camera_yaw = player_topDownCamAngles->current.vector[1];
  move[0] = (float)this->m_cmd.rightmove;
  move[1] = (float)this->m_cmd.forwardmove * -1.0;
  if ( rbveh->m_vehicle_info->cameraMode == 2 && rbveh->m_vehicle_info->type != 2 )
    move[0] = *(float *)&FLOAT_0_0;
  vehicleDeadZone = FLOAT_40_0;
  if ( COERCE_FLOAT(LODWORD(move[1]) & _mask__AbsFloat_) >= 40.0 )
  {
    if ( move[1] < 0.0 )
      v5 = FLOAT_N1_0;
    else
      v5 = FLOAT_1_0;
    vec[0] = (float)((float)(COERCE_FLOAT(LODWORD(move[1]) & _mask__AbsFloat_) - vehicleDeadZone)
                   / (float)(128.0 - vehicleDeadZone))
           * v5;
  }
  else
  {
    vec[0] = *(float *)&FLOAT_0_0;
  }
  if ( vehicleDeadZone <= COERCE_FLOAT(LODWORD(move[0]) & _mask__AbsFloat_) )
  {
    if ( move[0] < 0.0 )
      v4 = FLOAT_N1_0;
    else
      v4 = FLOAT_1_0;
    vec[1] = (float)((float)(COERCE_FLOAT(LODWORD(move[0]) & _mask__AbsFloat_) - vehicleDeadZone)
                   / (float)(128.0 - vehicleDeadZone))
           * v4;
  }
  else
  {
    vec[1] = *(float *)&FLOAT_0_0;
  }
  input = Vec2Length(vec);
  if ( input <= 1.0 )
    v3 = input;
  else
    v3 = FLOAT_1_0;
  input = v3;
  input_yaw = vectoyaw(vec) + camera_yaw + 180.0;
  input_yaw = AngleNormalize180(input_yaw);
  if ( v3 == 0.0 )
  {
    if ( rbveh->m_vehicle_info->cameraMode == 2 )
    {
      input_yaw = AngleNormalize180(camera_yaw);
    }
    else
    {
      Axis4ToAngles((const float (*)[4])&rbveh->m_phys_user_data->body->m_mat, rb_angles);
      input_yaw = rb_angles[1];
    }
  }
  forwardPreference = *(float *)&FLOAT_0_0;
  if ( !player_topDownCamMode->current.integer )
  {
    if ( rbveh->m_vehicle_info->cameraMode == 2 )
    {
      if ( this->m_cmd.forwardmove < (char)(int)COERCE_FLOAT(LODWORD(vehicleDeadZone) ^ _mask__NegFloat_) )
        v2 = FLOAT_N1_0;
      else
        v2 = FLOAT_1_0;
      forwardPreference = v2;
    }
    else
    {
      LODWORD(forwardPreference) = LODWORD(vec[0]) ^ _mask__NegFloat_;
    }
  }
  if ( rbveh->m_vehicle_info->type != 2 )
    forwardPreference = forwardPreference * 4.0;
  NitrousVehicleController::UpdateVehicleInputs(
    this,
    (const PhysObjUserData *)&savedregs,
    rbveh,
    input_yaw,
    input,
    forwardPreference);
}

// local variable allocation has failed, the output may be wrong!
void __userpurge NitrousVehicleController::UpdateVehicleInputs(
        NitrousVehicleController *this@<ecx>,
        const PhysObjUserData *a2@<ebp>,
        NitrousVehicle *rbveh,
        float target_yaw,
        float target_accel,
        float forward_preference)
{
  bool v6; // [esp+24h] [ebp-F4h]
  phys_vec3 *v7; // [esp+28h] [ebp-F0h]
  phys_vec3 v8; // [esp+2Ch] [ebp-ECh] BYREF
  int v9; // [esp+44h] [ebp-D4h]
  float m_auto_hand_brake_min_speed; // [esp+48h] [ebp-D0h]
  char v11; // [esp+4Fh] [ebp-C9h]
  float v12; // [esp+50h] [ebp-C8h]
  float hand_brake_min_speed; // [esp+54h] [ebp-C4h]
  float v14; // [esp+58h] [ebp-C0h] OVERLAPPED
  const VehicleParameter *m_parameter; // [esp+5Ch] [ebp-BCh]
  float v16; // [esp+60h] [ebp-B8h]
  float yaw_steer_range; // [esp+64h] [ebp-B4h]
  phys_vec3 *velocity; // [esp+68h] [ebp-B0h]
  int v19; // [esp+6Ch] [ebp-ACh] BYREF
  float v20; // [esp+84h] [ebp-94h]
  float v21; // [esp+88h] [ebp-90h]
  float m_steer_factor; // [esp+8Ch] [ebp-8Ch]
  float m_hand_brake; // [esp+90h] [ebp-88h]
  float v24; // [esp+94h] [ebp-84h]
  float v25; // [esp+98h] [ebp-80h]
  float v26; // [esp+9Ch] [ebp-7Ch]
  float fwd; // [esp+A0h] [ebp-78h]
  float targetSpeed; // [esp+A4h] [ebp-74h]
  float m_forward_vel; // [esp+A8h] [ebp-70h]
  float v30; // [esp+ACh] [ebp-6Ch]
  float anglePreference; // [esp+B0h] [ebp-68h]
  char v32; // [esp+B7h] [ebp-61h]
  float total_input_pref; // [esp+B8h] [ebp-60h]
  const VehicleParameter *total_base_forward_pref; // [esp+BCh] [ebp-5Ch]
  int v35; // [esp+C0h] [ebp-58h] BYREF
  float angle2; // [esp+C4h] [ebp-54h]
  float rb_angles[3]; // [esp+CCh] [ebp-4Ch] BYREF
  float _pt[3]; // [esp+D8h] [ebp-40h] BYREF
  float v39; // [esp+E4h] [ebp-34h]
  phys_vec3 pt; // [esp+E8h] [ebp-30h] BYREF
  float v41; // [esp+F8h] [ebp-20h]
  float v42; // [esp+FCh] [ebp-1Ch]
  float input_fwd[3]; // [esp+100h] [ebp-18h]
  const PhysObjUserData *userData; // [esp+10Ch] [ebp-Ch]
  float delta_yaw; // [esp+110h] [ebp-8h]
  float retaddr; // [esp+118h] [ebp+0h]

  userData = a2;
  delta_yaw = retaddr;
  LODWORD(input_fwd[2]) = this;
  input_fwd[1] = *(float *)&FLOAT_0_0;
  if ( (target_accel != 0.0 || rbveh->m_vehicle_info->type != 2) && (rbveh->m_flags & 1) == 0 )
  {
    input_fwd[0] = *(float *)&rbveh->m_phys_user_data;
    if ( phys_drawNitrousVehicle->current.enabled )
    {
      YawVectors(target_yaw, &pt.w, 0);
      LODWORD(pt.z) = *(unsigned int *)LODWORD(input_fwd[0]) + 48;
      _pt[1] = *(float *)(LODWORD(pt.z) + 48);
      _pt[2] = *(float *)(LODWORD(pt.z) + 52);
      v39 = *(float *)(LODWORD(pt.z) + 56);
      pt.x = *(float *)(LODWORD(pt.z) + 60);
      _pt[0] = v39;
      v39 = v39 + 100.0;
      Phys_NitrousVecToVec3((const phys_vec3 *)&_pt[1], rb_angles);
      pt.w = (float)(450.0 * pt.w) + rb_angles[0];
      v41 = (float)(450.0 * v41) + rb_angles[1];
      v42 = (float)(450.0 * v42) + rb_angles[2];
      CL_AddDebugLine(rb_angles, &pt.w, colorGreen, 0, 1);
    }
    Axis4ToAngles((const float (*)[4])(*(unsigned int *)LODWORD(input_fwd[0]) + 48), (float *)&v35);
    input_fwd[1] = AngleDelta(target_yaw, angle2);
  }
  if ( rbveh->m_vehicle_info->type != 2 )
  {
    total_base_forward_pref = rbveh->m_parameter;
    total_input_pref = 3.0 * total_base_forward_pref->m_tire_damp_coast;
    rbveh->m_coasting_factor = total_input_pref * rbveh->m_parameter->m_body_mass;
  }
  v32 = 0;
  anglePreference = base_forward_pref;
  v30 = input_pref;
  if ( rbveh->m_vehicle_info->type != 2 )
  {
    v30 = v30 + 20.0;
    m_forward_vel = rbveh->m_forward_vel;
    if ( m_forward_vel > speed_pref )
      anglePreference = anglePreference + 30.0;
  }
  targetSpeed = (float)(v30 * forward_preference) + anglePreference;
  if ( COERCE_FLOAT(LODWORD(input_fwd[1]) & _mask__AbsFloat_) > targetSpeed )
  {
    if ( input_fwd[1] < 0.0 )
      fwd = FLOAT_N1_0;
    else
      fwd = FLOAT_1_0;
    input_fwd[1] = input_fwd[1] - (float)(180.0 * fwd);
    v32 = 1;
  }
  if ( v32 )
    v26 = FLOAT_N1_0;
  else
    v26 = FLOAT_1_0;
  v25 = v26 * target_accel;
  NitrousVehicle::set_throttle(rbveh, v26 * target_accel);
  if ( rbveh->m_vehicle_info->cameraMode == 2 )
  {
    v24 = rbveh->m_forward_vel;
    if ( target_accel != 0.0 && (v24 > 25.0 && v32 || v24 < -25.0 && !v32) )
      input_fwd[1] = input_fwd[1] * -1.0;
    m_hand_brake = rbveh->m_hand_brake;
    if ( m_hand_brake > 0.1 && (v24 > 25.0 && v32 || v24 < -25.0 && !v32) )
    {
      m_steer_factor = rbveh->m_steer_factor;
      if ( m_steer_factor < 0.5 || (v21 = rbveh->m_steer_factor, v21 > -0.5) )
      {
        NitrousVehicle::set_brake(rbveh, 1.0);
        goto LABEL_46;
      }
    }
    goto LABEL_45;
  }
  v20 = rbveh->m_forward_vel;
  if ( target_accel == 0.0 || (v20 <= 25.0 || !v32) && (v20 >= -25.0 || v32) || rbveh->m_vehicle_info->type == 4 )
  {
LABEL_45:
    NitrousVehicle::set_brake(rbveh, 0.0);
    goto LABEL_46;
  }
  velocity = NitrousVehicle::get_velocity(rbveh, (phys_vec3 *)&v19);
  if ( (float)((float)((float)(velocity->x * velocity->x) + (float)(velocity->y * velocity->y))
             + (float)(velocity->z * velocity->z)) > (float)(steer_velocity * steer_velocity) )
    input_fwd[1] = *(float *)&FLOAT_0_0;
  input_fwd[1] = input_fwd[1] * -1.0;
  NitrousVehicle::set_brake(rbveh, 1.0);
LABEL_46:
  yaw_steer_range = *(float *)&FLOAT_0_0;
  if ( input_fwd[1] <= 0.0 )
  {
    if ( input_fwd[1] < 0.0 )
      yaw_steer_range = FLOAT_N1_0;
  }
  else
  {
    yaw_steer_range = FLOAT_1_0;
  }
  if ( v32 )
    v16 = FLOAT_N1_0;
  else
    v16 = FLOAT_1_0;
  yaw_steer_range = yaw_steer_range * v16;
  m_parameter = rbveh->m_parameter;
  v14 = m_parameter->m_steer_angle_max * 57.295776;
  if ( v32 && rbveh->m_vehicle_info->type != 2 )
    v14 = v14 * 2.0;
  if ( v14 > COERCE_FLOAT(LODWORD(input_fwd[1]) & _mask__AbsFloat_) )
    yaw_steer_range = (float)(COERCE_FLOAT(LODWORD(input_fwd[1]) & _mask__AbsFloat_) / v14) * yaw_steer_range;
  if ( (float)(yaw_steer_range - 1.0) < 0.0 )
    hand_brake_min_speed = yaw_steer_range;
  else
    hand_brake_min_speed = FLOAT_1_0;
  if ( (float)(-1.0 - yaw_steer_range) < 0.0 )
    v12 = hand_brake_min_speed;
  else
    v12 = FLOAT_N1_0;
  rbveh->m_steer_factor = v12;
  v11 = 0;
  m_auto_hand_brake_min_speed = rbveh->m_parameter->m_auto_hand_brake_min_speed;
  v9 = LODWORD(input_fwd[1]) & _mask__AbsFloat_;
  v6 = 0;
  if ( COERCE_FLOAT(LODWORD(input_fwd[1]) & _mask__AbsFloat_) > hand_brake_apply_delta )
  {
    v7 = NitrousVehicle::get_velocity(rbveh, &v8);
    if ( (float)((float)((float)(v7->x * v7->x) + (float)(v7->y * v7->y)) + (float)(v7->z * v7->z)) > (float)(m_auto_hand_brake_min_speed * m_auto_hand_brake_min_speed) )
      v6 = 1;
  }
  v11 = v6;
  if ( v6 )
  {
    if ( input_fwd[1] >= 0.0 || rbveh->m_phys_user_data->body->m_a_vel.z <= 0.0 )
    {
      if ( input_fwd[1] > 0.0 && rbveh->m_phys_user_data->body->m_a_vel.z < 0.0 )
        v11 = 0;
    }
    else
    {
      v11 = 0;
    }
  }
  if ( bitarray<51>::testBit((bitarray<51> *)(LODWORD(input_fwd[2]) + 68), 0x17u) )
    v11 = 1;
  if ( v11 )
    NitrousVehicle::set_hand_brake(rbveh, 1.0);
  else
    NitrousVehicle::set_hand_brake(rbveh, 0.0);
}

void __thiscall NitrousVehicle::set_brake(NitrousVehicle *this, float braking)
{
  if ( (braking < 0.0 || braking > 1.0)
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle.h",
          441,
          0,
          "%s\n\t%s",
          "braking>= 0.0f && braking<= 1.0f",
          "Out of range") )
  {
    __debugbreak();
  }
  this->m_brake = braking;
}

void __thiscall NitrousVehicleController::UpdateStun(
        NitrousVehicleController *this,
        NitrousVehicle *rbveh,
        float delta_t)
{
  float v4; // [esp+Ch] [ebp-14h]
  float desired_throttle; // [esp+18h] [ebp-8h]
  float desired_brake; // [esp+1Ch] [ebp-4h]

  if ( (rbveh->m_flags & 0x2000) != 0 )
  {
    desired_brake = rbveh->m_brake;
    desired_throttle = rbveh->m_throttle;
    if ( this->m_stun.m_stun_time > 0.0 )
    {
      this->m_stun.m_changed_this_frame = 0;
    }
    else
    {
      this->m_stun.m_stun_time = CG_flrand(stun_time_min, stun_time_max);
      this->m_stun.m_last_brake = StunRandomBrake(&this->m_cmd, desired_brake, this->m_stun.m_last_brake);
      if ( this->m_stun.m_last_brake <= 0.001 )
        v4 = (float)((float)((float)(throttle_max_scalar - throttle_min_scalar)
                           * (float)((float)(this->m_stun.m_stun_time - stun_time_min)
                                   / (float)(stun_time_max - stun_time_min)))
                   + throttle_min_scalar)
           * desired_throttle;
      else
        v4 = *(float *)&FLOAT_0_0;
      this->m_stun.m_last_throttle = v4;
      this->m_stun.m_changed_this_frame = 1;
    }
    NitrousVehicle::set_brake(rbveh, this->m_stun.m_last_brake);
    NitrousVehicle::set_throttle(rbveh, this->m_stun.m_last_throttle);
    this->m_stun.m_stun_time = this->m_stun.m_stun_time - delta_t;
  }
  else
  {
    this->m_stun.m_stun_time = *(float *)&FLOAT_0_0;
    this->m_stun.m_last_throttle = *(float *)&FLOAT_0_0;
    this->m_stun.m_last_brake = *(float *)&FLOAT_0_0;
    this->m_stun.m_changed_this_frame = 0;
  }
}

double __cdecl StunRandomBrake(const usercmd_s *cmd, float desired_brake, float last_brake)
{
  if ( bitarray<51>::testBit(&cmd->button_bits, 0x16u) && desired_brake > 0.5 )
  {
    if ( brake_fail_chance > CG_flrand(0.0, 1.0) )
      return 0.0;
    if ( brake_soft_chance > CG_flrand(0.0, 1.0) )
      return CG_flrand(0.0, desired_brake * brake_soft_percent);
  }
  else
  {
    if ( last_brake > 0.001 )
      return 0.0;
    if ( brake_start_chance > CG_flrand(0.0, 1.0) )
      return CG_flrand(min_brake, max_brake);
  }
  return CG_flrand(0.0, desired_brake);
}

void __userpurge NitrousVehicleController::UpdateControls(
        NitrousVehicleController *this@<ecx>,
        float a2@<ebp>,
        NitrousVehicle *rbveh,
        float delta_t)
{
  float v4; // [esp+24h] [ebp-7Ch]
  float m_forward_vel; // [esp+2Ch] [ebp-74h]
  float v6; // [esp+38h] [ebp-68h]
  float v7; // [esp+3Ch] [ebp-64h]
  scr_vehicle_s *scr_vehicle; // [esp+6Ch] [ebp-34h]
  float v9; // [esp+80h] [ebp-20h]
  float v10; // [esp+84h] [ebp-1Ch]
  float v11; // [esp+88h] [ebp-18h]
  float steering[2]; // [esp+94h] [ebp-Ch] BYREF
  float retaddr; // [esp+A0h] [ebp+0h]

  steering[0] = a2;
  steering[1] = retaddr;
  if ( (rbveh->m_flags & 4) != 0 )
  {
    NitrousVehicleController::UpdateScriptVehicleControl(this, (int)steering, rbveh, delta_t);
    if ( rbveh->m_vehicle_info->type == 4 )
      NitrousVehicleController::UpdateApplyBoatAccel(this, (const VehicleParameter *)steering, rbveh, delta_t);
    return;
  }
  if ( !this->m_cmd_valid )
    return;
  if ( (rbveh->m_flags & 0x80) == 0 )
  {
    if ( (rbveh->m_flags & 0x100) != 0 && (rbveh->m_flags & 1) == 0 )
    {
      if ( rbveh->m_owner && rbveh->m_owner->r.inuse && rbveh->m_owner->scr_vehicle )
      {
        scr_vehicle = rbveh->m_owner->scr_vehicle;
        if ( ((LODWORD(scr_vehicle->pathPos.origin[0]) & 0x7F800000) == 0x7F800000
           || (LODWORD(scr_vehicle->pathPos.origin[1]) & 0x7F800000) == 0x7F800000
           || (LODWORD(scr_vehicle->pathPos.origin[2]) & 0x7F800000) == 0x7F800000)
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle_controller.cpp",
                515,
                0,
                "%s",
                "!IS_NAN((vpp.origin)[0]) && !IS_NAN((vpp.origin)[1]) && !IS_NAN((vpp.origin)[2])") )
        {
          __debugbreak();
        }
        if ( ((LODWORD(scr_vehicle->pathPos.lookPos[0]) & 0x7F800000) == 0x7F800000
           || (LODWORD(scr_vehicle->pathPos.lookPos[1]) & 0x7F800000) == 0x7F800000
           || (LODWORD(scr_vehicle->pathPos.lookPos[2]) & 0x7F800000) == 0x7F800000)
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle_controller.cpp",
                516,
                0,
                "%s",
                "!IS_NAN((vpp.lookPos)[0]) && !IS_NAN((vpp.lookPos)[1]) && !IS_NAN((vpp.lookPos)[2])") )
        {
          __debugbreak();
        }
        v6 = scr_vehicle->pathPos.lookPos[1];
        v7 = scr_vehicle->pathPos.lookPos[2];
        this->m_script_goal_position.x = scr_vehicle->pathPos.lookPos[0];
        this->m_script_goal_position.y = v6;
        this->m_script_goal_position.z = v7;
        this->m_script_goal_radius = FLOAT_5_0;
        if ( rbveh->m_owner->scr_vehicle->manualMode == 1 )
          this->m_drive_path_speed = this->m_script_goal_speed;
        else
          this->m_drive_path_speed = scr_vehicle->pathPos.speed;
        if ( phys_drawNitrousVehicle->current.enabled )
        {
          CG_DebugSphere(scr_vehicle->pathPos.origin, 15.0, colorYellow, 10, 0, 1);
          CG_DebugSphere(scr_vehicle->pathPos.lookPos, 15.0, colorGreen, 10, 0, 1);
        }
      }
      NitrousVehicleController::UpdateScriptVehicleControl(this, (int)steering, rbveh, delta_t);
      goto LABEL_60;
    }
    if ( rbveh->m_vehicle_info->type == 2 )
    {
      if ( rbveh->m_vehicle_info->cameraMode == 5 )
        NitrousVehicleController::UpdateControlsTank(this, rbveh);
      else
        NitrousVehicleController::UpdateControlsStrafeMode(this, rbveh);
      m_forward_vel = rbveh->m_forward_vel;
      if ( m_forward_vel <= NitrousVehicle::get_max_speed(rbveh, 1) || rbveh->m_throttle <= 0.0 )
      {
        v4 = rbveh->m_forward_vel;
        if ( -NitrousVehicle::get_max_speed(rbveh, 1) > v4 && rbveh->m_throttle < 0.0 )
          NitrousVehicle::set_throttle(rbveh, 0.0);
      }
      else
      {
        NitrousVehicle::set_throttle(rbveh, 0.0);
      }
      goto LABEL_59;
    }
    if ( rbveh->m_vehicle_info->cameraMode == 2 || rbveh->m_vehicle_info->cameraMode == 3 )
      goto LABEL_56;
    if ( rbveh->m_vehicle_info->cameraMode == 5 )
      goto LABEL_52;
    if ( rbveh->m_vehicle_info->cameraMode != 4 )
    {
      NitrousVehicleController::UpdateControlsDefault(this, (int)steering, rbveh, delta_t);
      goto LABEL_59;
    }
    if ( vehControlMode && vehControlMode->current.integer )
LABEL_56:
      NitrousVehicleController::UpdateControlsStrafeMode(this, rbveh);
    else
LABEL_52:
      NitrousVehicleController::UpdateControlsTank(this, rbveh);
LABEL_59:
    NitrousVehicleController::UpdateJump(this, (rigid_body *)steering, rbveh, delta_t);
    goto LABEL_60;
  }
  if ( (rbveh->m_flags & 1) != 0 )
  {
    NitrousVehicle::start_path(rbveh, 0);
  }
  else
  {
    v11 = (float)(rbveh->m_owner->s.lerp.u.turret.gunAngles[0] * 0.017453292) * tweaker_0;
    if ( (float)(v11 - 1.0) < 0.0 )
      v10 = (float)(rbveh->m_owner->s.lerp.u.turret.gunAngles[0] * 0.017453292) * tweaker_0;
    else
      v10 = FLOAT_1_0;
    if ( (float)(-1.0 - v11) < 0.0 )
      v9 = v10;
    else
      v9 = FLOAT_N1_0;
    rbveh->m_steer_factor = v9;
    NitrousVehicle::set_throttle(rbveh, 0.0);
    NitrousVehicle::set_hand_brake(rbveh, 0.0);
  }
LABEL_60:
  if ( rbveh->m_vehicle_info->type == 4 )
    NitrousVehicleController::UpdateApplyBoatAccel(this, (const VehicleParameter *)steering, rbveh, delta_t);
  NitrousVehicleController::UpdateStun(this, rbveh, delta_t);
}

double __cdecl RecalibrateInputCustom(float val, float threshold)
{
  float v3; // [esp+0h] [ebp-8h]

  if ( threshold <= COERCE_FLOAT(LODWORD(val) & _mask__AbsFloat_) )
  {
    if ( val <= 0.0 )
      v3 = FLOAT_N1_0;
    else
      v3 = FLOAT_1_0;
    return (float)((float)((float)((float)(COERCE_FLOAT(LODWORD(val) & _mask__AbsFloat_) - threshold)
                                 / (float)(128.0 - threshold))
                         * 128.0)
                 * v3);
  }
  else
  {
    return *(float *)&FLOAT_0_0;
  }
}

// local variable allocation has failed, the output may be wrong!
void __userpurge NitrousVehicleController::UpdateControlsDefault(
        NitrousVehicleController *this@<ecx>,
        int a2@<ebp>,
        NitrousVehicle *rbveh,
        float delta_t)
{
  bool v4; // [esp+1Ch] [ebp-74h]
  phys_vec3 *velocity; // [esp+30h] [ebp-60h]
  int v6; // [esp+34h] [ebp-5Ch] BYREF
  float *v7; // [esp+4Ch] [ebp-44h]
  int p_m_mat; // [esp+50h] [ebp-40h]
  PhysObjUserData *m_phys_user_data; // [esp+54h] [ebp-3Ch]
  bool v10; // [esp+5Bh] [ebp-35h]
  float AnalogButtonValue; // [esp+5Ch] [ebp-34h]
  float v12; // [esp+60h] [ebp-30h]
  float v13; // [esp+64h] [ebp-2Ch] OVERLAPPED
  float gas; // [esp+68h] [ebp-28h]
  float fraction; // [esp+6Ch] [ebp-24h]
  float v16; // [esp+70h] [ebp-20h]
  float m_tire_damp_coast; // [esp+74h] [ebp-1Ch]
  const VehicleParameter *m_parameter; // [esp+78h] [ebp-18h]
  const vehicle_info_t *steering; // [esp+7Ch] [ebp-14h]
  NitrousVehicleController *v20; // [esp+80h] [ebp-10h]
  int v21; // [esp+84h] [ebp-Ch]
  const vehicle_info_t *info; // [esp+88h] [ebp-8h]
  const vehicle_info_t *retaddr; // [esp+90h] [ebp+0h]

  v21 = a2;
  info = retaddr;
  v20 = this;
  steering = rbveh->m_vehicle_info;
  m_parameter = rbveh->m_parameter;
  m_tire_damp_coast = m_parameter->m_tire_damp_coast;
  rbveh->m_coasting_factor = m_tire_damp_coast * rbveh->m_parameter->m_body_mass;
  v16 = (float)-v20->m_cmd.rightmove / 128.0;
  if ( steering->steerGraph )
  {
    LODWORD(fraction) = LODWORD(v16) & _mask__AbsFloat_;
    if ( v16 < 0.0 )
      gas = FLOAT_N1_0;
    else
      gas = FLOAT_1_0;
    v16 = GraphFloat_GetValue(steering->steerGraph, fraction) * gas;
  }
  if ( (float)(v16 - 1.0) < 0.0 )
    v13 = v16;
  else
    v13 = FLOAT_1_0;
  if ( (float)(-1.0 - v16) < 0.0 )
    v12 = v13;
  else
    v12 = FLOAT_N1_0;
  rbveh->m_steer_factor = v12;
  if ( rbveh->m_vehicle_info->cameraMode == 4 )
    AnalogButtonValue = (float)v20->m_cmd.forwardmove / 127.0;
  else
    AnalogButtonValue = CMD_GetAnalogButtonValue(&v20->m_cmd, 0x13u);
  v10 = rbveh->m_vehicle_info->type != 4;
  NitrousVehicle::set_hand_brake(rbveh, 0.0);
  NitrousVehicle::set_throttle(rbveh, 0.0);
  NitrousVehicle::set_brake(rbveh, 0.0);
  if ( AnalogButtonValue > 0.0 )
  {
    NitrousVehicle::set_throttle(rbveh, AnalogButtonValue);
    v20->m_stopped_time = *(float *)&FLOAT_0_0;
  }
  if ( !bitarray<51>::testBit(&v20->m_cmd.button_bits, 0x16u) || rbveh->m_vehicle_info->cameraMode == 4 )
  {
    if ( AnalogButtonValue < 0.0 && rbveh->m_vehicle_info->cameraMode == 4 )
      NitrousVehicle::set_throttle(rbveh, AnalogButtonValue);
  }
  else
  {
    if ( v10 )
    {
      m_phys_user_data = rbveh->m_phys_user_data;
      p_m_mat = (int)&m_phys_user_data->body->m_mat;
      v7 = (float *)(p_m_mat + 16);
      velocity = NitrousVehicle::get_velocity(rbveh, (phys_vec3 *)&v6);
      v4 = rbveh->m_forward_vel <= 1.0
        && (float)(COERCE_FLOAT(
                     COERCE_UNSIGNED_INT((float)((float)(velocity->x * *v7) + (float)(velocity->y * v7[1])) + (float)(velocity->z * v7[2]))
                   & _mask__AbsFloat_)
                 - 2.0) > COERCE_FLOAT(LODWORD(rbveh->m_forward_vel) & _mask__AbsFloat_)
        && required_stop_time > v20->m_stopped_time;
      if ( rbveh->m_forward_vel > 1.0 || v4 )
      {
        v20->m_stopped_time = *(float *)&FLOAT_0_0;
        if ( v4 )
          v20->m_stopped_time = FLOAT_N0_2;
        if ( COERCE_FLOAT(LODWORD(rbveh->m_steer_factor) & _mask__AbsFloat_) > breakHandbrakeThreshold
          || AnalogButtonValue > 0.0 )
        {
LABEL_36:
          NitrousVehicle::set_hand_brake(rbveh, 1.0);
          goto LABEL_42;
        }
      }
      else
      {
        v20->m_stopped_time = v20->m_stopped_time + delta_t;
        if ( v20->m_stopped_time > required_stop_time )
        {
          NitrousVehicle::set_throttle(rbveh, -1.0);
          goto LABEL_42;
        }
        if ( AnalogButtonValue > 0.0 )
          goto LABEL_36;
      }
      NitrousVehicle::set_brake(rbveh, 1.0);
      goto LABEL_42;
    }
    NitrousVehicle::set_throttle(rbveh, -1.0);
  }
LABEL_42:
  if ( !v10 )
  {
    if ( bitarray<51>::testBit(&v20->m_cmd.button_bits, 0x17u) )
      NitrousVehicle::set_hand_brake(rbveh, 1.0);
  }
}

void __thiscall NitrousVehicleController::UpdateControlsTank(NitrousVehicleController *this, NitrousVehicle *rbveh)
{
  float v2; // [esp+8h] [ebp-50h]
  float v3; // [esp+Ch] [ebp-4Ch]
  float v4; // [esp+10h] [ebp-48h]
  float v5; // [esp+14h] [ebp-44h]
  float v6; // [esp+1Ch] [ebp-3Ch]
  float v7; // [esp+20h] [ebp-38h]
  float steering; // [esp+34h] [ebp-24h]
  float normalized_x; // [esp+3Ch] [ebp-1Ch]
  float input_dist; // [esp+40h] [ebp-18h]
  float xa; // [esp+4Ch] [ebp-Ch]
  float x; // [esp+4Ch] [ebp-Ch]
  float y; // [esp+50h] [ebp-8h]
  float throttle; // [esp+54h] [ebp-4h]
  float throttlea; // [esp+54h] [ebp-4h]

  rbveh->m_coasting_factor = rbveh->m_parameter->m_tire_damp_coast * rbveh->m_parameter->m_body_mass;
  xa = (float)this->m_cmd.rightmove;
  y = RecalibrateInputCustom((float)this->m_cmd.forwardmove * -1.0, 40.0);
  x = RecalibrateInputCustom(xa, 40.0);
  normalized_x = *(float *)&FLOAT_0_0;
  input_dist = fsqrt((float)(x * x) + (float)(y * y));
  if ( input_dist != 0.0 )
    normalized_x = x / input_dist;
  if ( normalized_x < 0.0 )
    v5 = FLOAT_N1_0;
  else
    v5 = FLOAT_1_0;
  steering = COERCE_FLOAT(COERCE_UNSIGNED_INT(normalized_x * normalized_x) ^ _mask__NegFloat_) * v5;
  if ( COERCE_FLOAT(LODWORD(steering) & _mask__AbsFloat_) < 0.98000002 )
    steering = steering * steer_reduce_frac;
  if ( rbveh->m_vehicle_info->cameraMode == 4 || rbveh->m_vehicle_info->type != 2 )
    v4 = FLOAT_1_0;
  else
    v4 = FLOAT_0_25;
  throttle = (float)((float)((float)((float)(normalized_x * x) / 129.0) * v4)
                   * (float)((float)((float)(normalized_x * x) / 129.0) * v4))
           + (float)((float)(y / 129.0) * (float)(y / 129.0));
  if ( throttle == 0.0 )
    throttlea = *(float *)&FLOAT_0_0;
  else
    throttlea = throttle / fsqrt(throttle);
  if ( y > 0.0 )
    throttlea = throttlea * -1.0;
  if ( rbveh->m_forward_vel >= 0.0 || throttlea <= 0.0 )
  {
    if ( rbveh->m_forward_vel > 0.0 && throttlea < 0.0 )
      LODWORD(steering) ^= _mask__NegFloat_;
  }
  else
  {
    LODWORD(steering) ^= _mask__NegFloat_;
  }
  if ( (float)(steering - 1.0) < 0.0 )
    v7 = steering;
  else
    v7 = FLOAT_1_0;
  if ( (float)(-1.0 - steering) < 0.0 )
    v3 = v7;
  else
    v3 = FLOAT_N1_0;
  rbveh->m_steer_factor = v3;
  if ( (float)(throttlea - 1.0) < 0.0 )
    v6 = throttlea;
  else
    v6 = FLOAT_1_0;
  if ( (float)(-1.0 - throttlea) < 0.0 )
    v2 = v6;
  else
    v2 = FLOAT_N1_0;
  NitrousVehicle::set_throttle(rbveh, v2);
  NitrousVehicle::set_hand_brake(rbveh, 0.0);
  NitrousVehicle::set_brake(rbveh, 0.0);
}

void __thiscall NitrousVehicleController::SetScriptTarget(
        NitrousVehicleController *this,
        NitrousVehicle *rbveh,
        phys_vec3 *goal_position,
        float goal_radius,
        float goal_speed,
        bool stopAtGoal)
{
  float v6; // [esp+4h] [ebp-Ch]
  float v7; // [esp+Ch] [ebp-4h]

  this->m_script_goal_position.x = goal_position->x;
  this->m_script_goal_position.y = goal_position->y;
  this->m_script_goal_position.z = goal_position->z;
  if ( (float)(goal_radius - 5000.0) < 0.0 )
    v7 = goal_radius;
  else
    v7 = FLOAT_5000_0;
  if ( (float)(5.0 - goal_radius) < 0.0 )
    v6 = v7;
  else
    v6 = FLOAT_5_0;
  this->m_script_goal_radius = v6;
  this->m_script_goal_speed = goal_speed;
  this->m_stop_at_goal = stopAtGoal;
  rbveh->m_flags |= 4u;
}

void __userpurge NitrousVehicleController::UpdateJump(
        NitrousVehicleController *this@<ecx>,
        rigid_body *a2@<ebp>,
        NitrousVehicle *rbveh,
        float delta_t)
{
  unsigned int v4[3]; // [esp-Ch] [ebp-3Ch] BYREF
  float m_jump_force; // [esp+10h] [ebp-20h]
  const VehicleParameter *m_parameter; // [esp+14h] [ebp-1Ch]
  rigid_body *v7; // [esp+18h] [ebp-18h]
  int m_num_colliding_wheels; // [esp+1Ch] [ebp-14h]
  const VehicleParameter *vehParams; // [esp+20h] [ebp-10h]
  rigid_body *body; // [esp+24h] [ebp-Ch]
  void *v11; // [esp+28h] [ebp-8h]
  void *retaddr; // [esp+30h] [ebp+0h]

  body = a2;
  v11 = retaddr;
  vehParams = (const VehicleParameter *)this;
  if ( bitarray<51>::testBit(&this->m_cmd.button_bits, 0x24u) )
  {
    m_num_colliding_wheels = rbveh->m_num_colliding_wheels;
    if ( m_num_colliding_wheels >= 2 && !vehParams->m_name[14] )
    {
      v7 = rbveh->m_phys_user_data->body;
      m_parameter = rbveh->m_parameter;
      m_jump_force = m_parameter->m_jump_force;
      v4[0] = *(unsigned int *)&FLOAT_0_0;
      v4[1] = *(unsigned int *)&FLOAT_0_0;
      *(float *)&v4[2] = m_jump_force;
      rigid_body::add_force(v7, (const phys_vec3 *)v4);
      vehParams->m_name[14] = 1;
    }
  }
  else
  {
    vehParams->m_name[14] = 0;
  }
}

void __thiscall rigid_body::add_force(rigid_body *this, const phys_vec3 *force)
{
  this->m_force_sum.x = this->m_force_sum.x + force->x;
  this->m_force_sum.y = this->m_force_sum.y + force->y;
  this->m_force_sum.z = this->m_force_sum.z + force->z;
  if ( COERCE_FLOAT(LODWORD(this->m_force_sum.x) & _mask__AbsFloat_) > 100000.0
    || COERCE_FLOAT(LODWORD(this->m_force_sum.y) & _mask__AbsFloat_) > 100000.0
    || COERCE_FLOAT(LODWORD(this->m_force_sum.z) & _mask__AbsFloat_) > 100000.0 )
  {
    phys_exec_debug_callback(this);
  }
}

// local variable allocation has failed, the output may be wrong!
void __userpurge NitrousVehicleController::UpdateApplyBoatAccel(
        NitrousVehicleController *this@<ecx>,
        const VehicleParameter *a2@<ebp>,
        NitrousVehicle *rbveh,
        float delta_t)
{
  double v4; // st7
  float v5; // [esp+10h] [ebp-364h]
  float v6; // [esp+14h] [ebp-360h]
  phys_vec3 v7; // [esp+18h] [ebp-35Ch] BYREF
  float v8; // [esp+28h] [ebp-34Ch]
  float v9; // [esp+2Ch] [ebp-348h]
  bool v10; // [esp+33h] [ebp-341h]
  bool v11; // [esp+34h] [ebp-340h]
  float v12; // [esp+38h] [ebp-33Ch]
  float v13; // [esp+3Ch] [ebp-338h] OVERLAPPED
  float v14; // [esp+40h] [ebp-334h]
  float v15; // [esp+44h] [ebp-330h]
  float v16; // [esp+48h] [ebp-32Ch]
  float v17[2]; // [esp+4Ch] [ebp-328h] BYREF
  phys_vec3 end; // [esp+58h] [ebp-31Ch] BYREF
  float v19; // [esp+6Ch] [ebp-308h]
  float v20; // [esp+70h] [ebp-304h]
  float v21; // [esp+74h] [ebp-300h]
  float v22; // [esp+78h] [ebp-2FCh]
  float v23; // [esp+7Ch] [ebp-2F8h]
  float v24; // [esp+80h] [ebp-2F4h]
  float v25; // [esp+8Ch] [ebp-2E8h]
  float v26; // [esp+90h] [ebp-2E4h]
  float v27; // [esp+94h] [ebp-2E0h]
  float v28[3]; // [esp+98h] [ebp-2DCh] BYREF
  float start[5]; // [esp+A4h] [ebp-2D0h] BYREF
  _BYTE v30[12]; // [esp+B8h] [ebp-2BCh] BYREF
  phys_mat44 rv; // [esp+C4h] [ebp-2B0h]
  float v32; // [esp+104h] [ebp-270h]
  float v33; // [esp+108h] [ebp-26Ch] BYREF
  float rot; // [esp+10Ch] [ebp-268h]
  float v35; // [esp+110h] [ebp-264h]
  phys_vec3 accel; // [esp+114h] [ebp-260h] BYREF
  phys_vec3 motor_position; // [esp+124h] [ebp-250h]
  float v38; // [esp+134h] [ebp-240h]
  float v39; // [esp+138h] [ebp-23Ch]
  float v40; // [esp+13Ch] [ebp-238h]
  float v41; // [esp+140h] [ebp-234h]
  float v42; // [esp+14Ch] [ebp-228h]
  float v43; // [esp+150h] [ebp-224h]
  float v44; // [esp+154h] [ebp-220h]
  float v45; // [esp+158h] [ebp-21Ch]
  float v46; // [esp+15Ch] [ebp-218h]
  float v47; // [esp+160h] [ebp-214h]
  float v48; // [esp+174h] [ebp-200h]
  float v49; // [esp+178h] [ebp-1FCh]
  float v50; // [esp+17Ch] [ebp-1F8h]
  int v51; // [esp+180h] [ebp-1F4h]
  float v52; // [esp+184h] [ebp-1F0h]
  float v53; // [esp+188h] [ebp-1ECh]
  float v54; // [esp+18Ch] [ebp-1E8h]
  float v55; // [esp+190h] [ebp-1E4h]
  float v56; // [esp+1A4h] [ebp-1D0h]
  float v57; // [esp+1A8h] [ebp-1CCh]
  float v58; // [esp+1ACh] [ebp-1C8h]
  int v59; // [esp+1B0h] [ebp-1C4h]
  float v60; // [esp+1B4h] [ebp-1C0h]
  phys_vec3 v61; // [esp+1B8h] [ebp-1BCh] BYREF
  float v62; // [esp+1CCh] [ebp-1A8h]
  float v63; // [esp+1D0h] [ebp-1A4h]
  float v64; // [esp+1D4h] [ebp-1A0h]
  phys_vec3 v65; // [esp+1D8h] [ebp-19Ch] BYREF
  float v66; // [esp+1ECh] [ebp-188h]
  float v67; // [esp+1F0h] [ebp-184h]
  float v68; // [esp+1F4h] [ebp-180h]
  float v69; // [esp+1F8h] [ebp-17Ch]
  float v70; // [esp+1FCh] [ebp-178h]
  float v71; // [esp+200h] [ebp-174h]
  float v72; // [esp+214h] [ebp-160h]
  float v73; // [esp+218h] [ebp-15Ch]
  float v74; // [esp+21Ch] [ebp-158h]
  int v75; // [esp+220h] [ebp-154h]
  float v76; // [esp+224h] [ebp-150h]
  unsigned int v77[3]; // [esp+228h] [ebp-14Ch] BYREF
  phys_vec3 leftForce; // [esp+234h] [ebp-140h]
  int v79; // [esp+244h] [ebp-130h]
  float v80; // [esp+248h] [ebp-12Ch]
  float v81; // [esp+24Ch] [ebp-128h]
  float v82; // [esp+250h] [ebp-124h]
  float v83; // [esp+25Ch] [ebp-118h]
  float v84; // [esp+260h] [ebp-114h]
  float v85; // [esp+264h] [ebp-110h]
  float v86; // [esp+268h] [ebp-10Ch]
  float v87; // [esp+26Ch] [ebp-108h]
  float v88; // [esp+270h] [ebp-104h]
  float v89; // [esp+27Ch] [ebp-F8h]
  float v90; // [esp+280h] [ebp-F4h]
  float v91; // [esp+284h] [ebp-F0h]
  float v92; // [esp+288h] [ebp-ECh]
  float v93; // [esp+28Ch] [ebp-E8h]
  float v94; // [esp+290h] [ebp-E4h]
  float v95; // [esp+2A4h] [ebp-D0h]
  float v96; // [esp+2A8h] [ebp-CCh]
  float v97; // [esp+2ACh] [ebp-C8h]
  float m_steer_factor; // [esp+2B0h] [ebp-C4h]
  float v99; // [esp+2B4h] [ebp-C0h]
  float v100; // [esp+2B8h] [ebp-BCh] BYREF
  float v101; // [esp+2BCh] [ebp-B8h]
  float v102; // [esp+2C0h] [ebp-B4h]
  phys_vec3 rightForce; // [esp+2C4h] [ebp-B0h]
  float v104; // [esp+2DCh] [ebp-98h]
  float v105; // [esp+2E0h] [ebp-94h]
  float v106; // [esp+2E4h] [ebp-90h]
  float v107; // [esp+2E8h] [ebp-8Ch]
  float v108; // [esp+2ECh] [ebp-88h]
  float v109; // [esp+2F0h] [ebp-84h]
  float v110; // [esp+2F8h] [ebp-7Ch]
  float v111; // [esp+2FCh] [ebp-78h]
  float v112; // [esp+300h] [ebp-74h]
  float m_forward_vel; // [esp+304h] [ebp-70h]
  float m_throttle; // [esp+308h] [ebp-6Ch]
  int v115; // [esp+30Ch] [ebp-68h]
  float v116; // [esp+310h] [ebp-64h]
  float throttle; // [esp+314h] [ebp-60h]
  float v118; // [esp+318h] [ebp-5Ch]
  float rotSpeedFactor; // [esp+31Ch] [ebp-58h]
  float speedFactor; // [esp+320h] [ebp-54h]
  float turnProportion; // [esp+324h] [ebp-50h]
  float forwardProportion; // [esp+328h] [ebp-4Ch] BYREF
  float v123; // [esp+32Ch] [ebp-48h]
  float v124; // [esp+330h] [ebp-44h]
  phys_vec3 forwardForce; // [esp+334h] [ebp-40h] BYREF
  float pos[3]; // [esp+348h] [ebp-2Ch] BYREF
  phys_vec3 center; // [esp+354h] [ebp-20h]
  const vehicle_info_t *info; // [esp+364h] [ebp-10h]
  const VehicleParameter *vehParams; // [esp+368h] [ebp-Ch] BYREF
  rigid_body *body; // [esp+36Ch] [ebp-8h]
  rigid_body *retaddr; // [esp+374h] [ebp+0h]

  vehParams = a2;
  body = retaddr;
  info = (const vehicle_info_t *)this;
  if ( (rbveh->m_flags & 0x400) != 0 )
  {
    LODWORD(center.w) = rbveh->m_phys_user_data->body;
    LODWORD(center.z) = rbveh->m_parameter;
    LODWORD(center.y) = rbveh->m_vehicle_info->name;
    pos[0] = *(float *)(LODWORD(center.w) + 96);
    pos[1] = *(float *)(LODWORD(center.w) + 100);
    pos[2] = *(float *)(LODWORD(center.w) + 104);
    center.x = *(float *)(LODWORD(center.w) + 108);
    Phys_NitrousVecToVec3((const phys_vec3 *)pos, &forwardForce.z);
    forwardProportion = *(float *)(LODWORD(center.w) + 48);
    v123 = *(float *)(LODWORD(center.w) + 52);
    v124 = *(float *)(LODWORD(center.w) + 56);
    forwardForce.x = *(float *)(LODWORD(center.w) + 60);
    turnProportion = rbveh->m_forward_vel;
    LODWORD(speedFactor) = LODWORD(turnProportion) & _mask__AbsFloat_;
    rotSpeedFactor = COERCE_FLOAT(LODWORD(turnProportion) & _mask__AbsFloat_)
                   / (float)(*(float *)(LODWORD(center.z) + 232) * rbveh->m_speed_scale);
    if ( rotSpeedFactor > 1.0 )
      rotSpeedFactor = FLOAT_1_0;
    v4 = Abs((const float *)(LODWORD(center.w) + 160));
    v118 = v4 / *(float *)(LODWORD(center.z) + 244);
    if ( v118 > 1.0 )
      v118 = FLOAT_1_0;
    throttle = 1.0 - rotSpeedFactor;
    v116 = (float)(1.0 - rotSpeedFactor) * 0.5;
    if ( *(unsigned int *)(LODWORD(center.y) + 7324) )
    {
      v115 = LODWORD(rotSpeedFactor) & _mask__AbsFloat_;
      throttle = GraphFloat_GetValue(
                   (const GraphFloat *)*(unsigned int *)(LODWORD(center.y) + 7324),
                   COERCE_FLOAT(LODWORD(rotSpeedFactor) & _mask__AbsFloat_));
    }
    throttle = (float)((float)((float)(delta_t * 60.0) * *(float *)(LODWORD(center.z) + 236)) * rbveh->m_speed_scale)
             * throttle;
    v116 = (float)((float)((float)(delta_t * 60.0) * *(float *)(LODWORD(center.z) + 240)) * rbveh->m_drivepath_scale)
         * v116;
    m_throttle = rbveh->m_throttle;
    if ( m_throttle < 0.0 )
    {
      m_forward_vel = rbveh->m_forward_vel;
      if ( m_forward_vel < 1.0 )
        throttle = throttle * *(float *)(LODWORD(center.z) + 8);
    }
    if ( dothisoldstuff )
    {
      v112 = m_throttle * forwardProportion;
      v111 = m_throttle * v123;
      v110 = m_throttle * v124;
      v107 = m_throttle * forwardProportion;
      v108 = m_throttle * v123;
      v109 = m_throttle * v124;
      v106 = throttle * (float)(m_throttle * forwardProportion);
      v105 = throttle * (float)(m_throttle * v123);
      v104 = throttle * (float)(m_throttle * v124);
      rightForce.y = v106;
      rightForce.z = v105;
      rightForce.w = v104;
      forwardProportion = v106;
      v123 = v105;
      v124 = v104;
      rigid_body::add_force((rigid_body *)LODWORD(center.w), (const phys_vec3 *)&forwardProportion);
      v100 = *(float *)(LODWORD(center.w) + 48);
      v101 = *(float *)(LODWORD(center.w) + 52);
      v102 = *(float *)(LODWORD(center.w) + 56);
      rightForce.x = *(float *)(LODWORD(center.w) + 60);
      v99 = rbveh->m_throttle;
      m_steer_factor = rbveh->m_steer_factor;
      v97 = m_steer_factor * v100;
      v96 = m_steer_factor * v101;
      v95 = m_steer_factor * v102;
      v92 = m_steer_factor * v100;
      v93 = m_steer_factor * v101;
      v94 = m_steer_factor * v102;
      v91 = v116 * (float)(m_steer_factor * v100);
      v90 = v116 * (float)(m_steer_factor * v101);
      v89 = v116 * (float)(m_steer_factor * v102);
      v86 = v91;
      v87 = v90;
      v88 = v89;
      v85 = v99 * v91;
      v84 = v99 * v90;
      v83 = v99 * v89;
      v80 = v99 * v91;
      v81 = v99 * v90;
      v82 = v99 * v89;
      v100 = v99 * v91;
      v101 = v99 * v90;
      v102 = v99 * v89;
      v79 = COERCE_UNSIGNED_INT(v99 * v91) ^ _mask__NegFloat_;
      LODWORD(leftForce.w) = COERCE_UNSIGNED_INT(v99 * v90) ^ _mask__NegFloat_;
      LODWORD(leftForce.z) = COERCE_UNSIGNED_INT(v99 * v89) ^ _mask__NegFloat_;
      v77[0] = v79;
      v77[1] = LODWORD(leftForce.w);
      v77[2] = LODWORD(leftForce.z);
      v76 = (float)(*(float *)(LODWORD(center.z) + 224) - *(float *)(LODWORD(center.z) + 212)) / 2.0;
      v75 = LODWORD(center.w) + 64;
      v74 = v76 * *(float *)(LODWORD(center.w) + 64);
      v73 = v76 * *(float *)(LODWORD(center.w) + 68);
      v72 = v76 * *(float *)(LODWORD(center.w) + 72);
      v69 = v74;
      v70 = v73;
      v71 = v72;
      v68 = pos[0] + v74;
      v67 = pos[1] + v73;
      v66 = pos[2] + v72;
      v65.x = pos[0] + v74;
      v65.y = pos[1] + v73;
      v65.z = pos[2] + v72;
      rigid_body::add_force((rigid_body *)LODWORD(center.w), (const phys_vec3 *)v77, &v65, 1.0);
      v64 = pos[0] - v69;
      v63 = pos[1] - v70;
      v62 = pos[2] - v71;
      v61.x = pos[0] - v69;
      v61.y = pos[1] - v70;
      v61.z = pos[2] - v71;
      rigid_body::add_force((rigid_body *)LODWORD(center.w), (const phys_vec3 *)&v100, &v61, 1.0);
    }
    else
    {
      v60 = *(float *)(LODWORD(center.z) + 260);
      v59 = LODWORD(center.w) + 80;
      v58 = v60 * *(float *)(LODWORD(center.w) + 80);
      v57 = v60 * *(float *)(LODWORD(center.w) + 84);
      v56 = v60 * *(float *)(LODWORD(center.w) + 88);
      v53 = v58;
      v54 = v57;
      v55 = v56;
      v52 = *(float *)(LODWORD(center.z) + 252);
      v51 = LODWORD(center.w) + 48;
      v50 = v52 * *(float *)(LODWORD(center.w) + 48);
      v49 = v52 * *(float *)(LODWORD(center.w) + 52);
      v48 = v52 * *(float *)(LODWORD(center.w) + 56);
      v45 = v50;
      v46 = v49;
      v47 = v48;
      v44 = pos[0] + v50;
      v43 = pos[1] + v49;
      v42 = pos[2] + v48;
      v39 = pos[0] + v50;
      v40 = pos[1] + v49;
      v41 = pos[2] + v48;
      v38 = (float)(pos[0] + v50) + v58;
      motor_position.w = (float)(pos[1] + v49) + v57;
      motor_position.z = (float)(pos[2] + v48) + v56;
      accel.y = v38;
      accel.z = motor_position.w;
      accel.w = motor_position.z;
      v33 = *(float *)(LODWORD(center.w) + 48);
      rot = *(float *)(LODWORD(center.w) + 52);
      v35 = *(float *)(LODWORD(center.w) + 56);
      accel.x = *(float *)(LODWORD(center.w) + 60);
      v32 = rbveh->m_steer_factor;
      rv.w.w = COERCE_FLOAT(LODWORD(v32) ^ _mask__NegFloat_) * *(float *)(LODWORD(center.z) + 12);
      make_rotate((int)&vehParams, (phys_mat44 *)v30, (const phys_vec3 *)(LODWORD(center.w) + 80), rv.w.w, 1000.0);
      LODWORD(start[0]) = phys_multiply((const phys_vec3 *)&start[1], (const phys_mat44 *)v30, (const phys_vec3 *)&v33);
      v33 = *(float *)LODWORD(start[0]);
      rot = *(float *)(LODWORD(start[0]) + 4);
      v35 = *(float *)(LODWORD(start[0]) + 8);
      if ( phys_drawNitrousVehicle->current.enabled )
      {
        Phys_NitrousVecToVec3((phys_vec3 *)&accel.y, v28);
        v27 = 50.0 * v33;
        v26 = 50.0 * rot;
        v25 = 50.0 * v35;
        v22 = 50.0 * v33;
        v23 = 50.0 * rot;
        v24 = 50.0 * v35;
        v21 = accel.y - (float)(50.0 * v33);
        v20 = accel.z - (float)(50.0 * rot);
        v19 = accel.w - (float)(50.0 * v35);
        end.x = v21;
        end.y = v20;
        end.z = v19;
        Phys_NitrousVecToVec3(&end, v17);
        CL_AddDebugLine(v28, v17, colorWhite, 0, 1);
      }
      v16 = m_throttle;
      if ( rbveh->m_vehicle_info->cameraMode == 1 )
      {
        if ( v16 >= 1.0 || v16 <= 0.0 )
        {
          if ( v16 == 0.0 )
          {
            v16 = rotSpeedFactor * coast_steer_scaler;
            v15 = rbveh->m_forward_vel;
            if ( v15 < 0.0 )
              v16 = v16 * -1.0;
          }
          else if ( v16 < 0.0 )
          {
            v14 = rbveh->m_forward_vel;
            if ( v14 > 0.0 )
              v16 = v16 * -1.0;
            v13 = rbveh->m_steer_factor;
            v11 = COERCE_FLOAT(LODWORD(v13) & _mask__AbsFloat_) > 0.60000002
               && (v12 = rbveh->m_forward_vel, v12 > 100.0);
            v10 = v11;
            if ( v11 )
            {
              v116 = v116 * *(float *)(LODWORD(center.z) + 248);
              throttle = throttle * forward_accel_scale;
              *(float *)&info->name[28] = FLOAT_N0_60000002;
            }
            else if ( *(float *)&info->name[28] < 0.0 )
            {
              throttle = *(float *)&FLOAT_0_0;
              v116 = *(float *)&FLOAT_0_0;
            }
            *(float *)&info->name[28] = *(float *)&info->name[28] + delta_t;
          }
        }
        else
        {
          v16 = v16 * analog_throttle_turn_factor;
        }
        if ( (float)(v16 - 1.0) < 0.0 )
          v9 = v16;
        else
          v9 = FLOAT_1_0;
        if ( (float)(-1.0 - v16) < 0.0 )
          v8 = v9;
        else
          v8 = FLOAT_N1_0;
        v16 = v8;
      }
      v7 = *(phys_vec3 *)(LODWORD(center.w) + 64);
      v6 = (float)((float)((float)((float)(v7.x * v33) + (float)(v7.y * rot)) + (float)(v7.z * v35)) * v16) * v116;
      v7.x = v7.x * v6;
      v7.y = v7.y * v6;
      v7.z = v7.z * v6;
      rigid_body::add_force((rigid_body *)LODWORD(center.w), &v7, (phys_vec3 *)&accel.y, 1.0);
      v5 = (float)((float)((float)((float)(forwardProportion * v33) + (float)(v123 * rot)) + (float)(v124 * v35))
                 * m_throttle)
         * throttle;
      forwardProportion = forwardProportion * v5;
      v123 = v123 * v5;
      v124 = v124 * v5;
      rigid_body::add_force(
        (rigid_body *)LODWORD(center.w),
        (const phys_vec3 *)&forwardProportion,
        (phys_vec3 *)&accel.y,
        1.0);
    }
  }
}

void __thiscall rigid_body::add_force(
        rigid_body *this,
        const phys_vec3 *force,
        const phys_vec3 *point,
        float torque_mult)
{
  double v6; // st7
  float v7; // [esp-30h] [ebp-3Ch]
  float v8; // [esp-2Ch] [ebp-38h]
  float v9; // [esp-28h] [ebp-34h]
  float v10; // [esp-20h] [ebp-2Ch]
  float v11; // [esp-20h] [ebp-2Ch]
  float v12; // [esp-1Ch] [ebp-28h]
  float v13; // [esp-1Ch] [ebp-28h]
  float v14; // [esp-18h] [ebp-24h]
  float v15; // [esp-18h] [ebp-24h]
  float v16; // [esp-4h] [ebp-10h]
  float v17; // [esp-4h] [ebp-10h]
  float v18; // [esp-4h] [ebp-10h]
  float v19; // [esp-4h] [ebp-10h]
  float v20; // [esp-4h] [ebp-10h]
  float v21; // [esp-4h] [ebp-10h]

  this->m_force_sum.x = this->m_force_sum.x + force->x;
  this->m_force_sum.y = this->m_force_sum.y + force->y;
  this->m_force_sum.z = this->m_force_sum.z + force->z;
  v10 = point->x - this->m_mat.w.x;
  v12 = point->y - this->m_mat.w.y;
  v14 = point->z - this->m_mat.w.z;
  v7 = force->z * v12 - force->y * v14;
  v8 = v14 * force->x - force->z * v10;
  v9 = v10 * force->y - v12 * force->x;
  v11 = v7 * torque_mult;
  v13 = v8 * torque_mult;
  v15 = torque_mult * v9;
  this->m_torque_sum.x = this->m_torque_sum.x + v11;
  this->m_torque_sum.y = v13 + this->m_torque_sum.y;
  this->m_torque_sum.z = this->m_torque_sum.z + v15;
  v6 = 100000.0;
  v16 = fabs(this->m_force_sum.x);
  if ( v16 > 100000.0
    || (v17 = fabs(this->m_force_sum.y), v17 > 100000.0)
    || (v18 = fabs(this->m_force_sum.z), v18 > 100000.0) )
  {
    phys_exec_debug_callback(this);
    v6 = 100000.0;
  }
  v19 = fabs(this->m_torque_sum.x);
  if ( v19 > v6 || (v20 = fabs(this->m_torque_sum.y), v20 > v6) || (v21 = fabs(this->m_torque_sum.z), v6 < v21) )
    phys_exec_debug_callback(this);
}

