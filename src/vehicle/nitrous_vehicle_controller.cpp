#include "nitrous_vehicle_controller.h"
#include "nitrous_vehicle.h"
#include <bgame/bg_slidemove.h>
#include <client/cl_debugdata.h>
#include <game_mp/g_main_mp.h>
#include <bgame/bg_misc.h>
#include <cgame/cg_main.h>
#include <cgame/cg_drawtools.h>
#include <physics/phys_assert.h>
#include <physics/phys_main.h>
#include <cgame_mp/cg_local_mp.h>
#include <cmath>

float hand_brake_apply_delta = 30.0;
float steer_velocity = 400.0;
float speed_pref = 90.0;
float input_pref = 35.0;
float base_forward_pref = 100.0;
float throttle_min_scalar = 0.1;
float throttle_max_scalar = 1.0;
float stun_time_min = 0.25;
float stun_time_max = 0.5;
float min_brake = 1.0;
float max_brake = 1.0;
float brake_start_chance = 0.89999998;
float brake_soft_percent = 0.40000001;
float brake_soft_chance = 0.60000002;
float brake_fail_chance = 0.5;
float tweaker_0 = 11.0;
float breakHandbrakeThreshold = 0.69999999;
float required_stop_time = 0.2;
float steer_reduce_frac = 0.5;
float forward_accel_scale = -0.25;
float coast_steer_scaler = 10.0;
float analog_throttle_turn_factor = 2.5;

// -----------------------------------------------------------------------------
// NitrousVehicleController::UpdateScriptVehicleControl
// CoDMPServer.c:659822 — RVA 0x0082A800
// -----------------------------------------------------------------------------
// local variable allocation has failed, the output may be wrong!
void NitrousVehicleController::UpdateScriptVehicleControl(
                NitrousVehicle *rbveh,
                float delta_t)
{
    double max_speed;
    float speed_at_cur_dist;
    float frac_to_goal;
    float current_speed;
    float stuck_time;
    phys_vec3 *p_m_stuck_position;
    float angle_to_front;
    float angle_to_side;
    float end[3];
    float start[3];
    float goal_speed;
    float yaw;
    float inv_goal_dist;
    float to_goal_dist;
    phys_vec3 to_goal;
    phys_vec3 currentOrigin;
    phys_mat44 *p_m_mat;
    PhysObjUserData *userData;
    userData = rbveh->m_phys_user_data;
    p_m_mat = &userData->body->m_mat;
    currentOrigin = p_m_mat->w;
    to_goal.x = this->m_script_goal_position.x - currentOrigin.x;
    to_goal.y = this->m_script_goal_position.y - currentOrigin.y;
    to_goal.z = 0.0f;
    to_goal_dist = Abs(&to_goal.x);
    if (to_goal_dist <= 0.0)
    {
        yaw = 0.0f;
    }
    else
    {
        inv_goal_dist = 1.0f / to_goal_dist;
        to_goal.x = to_goal.x * inv_goal_dist;
        to_goal.y = to_goal.y * inv_goal_dist;
        to_goal.z = to_goal.z * inv_goal_dist;
        // Decomp: replaced mistaken atan(y) with atan2(y,x) matching __libm_sse2_atan2 heading from planar dir.
        yaw = atan2f(to_goal.y, to_goal.x) * (180.0f / 3.1415927f);
        if (yaw < 0.0)
            yaw = yaw + 360.0;
    }
    goal_speed = this->m_script_goal_speed + 10.0;
    if ((rbveh->m_flags & 0x100) != 0)
        goal_speed = this->m_drive_path_speed;
    if (phys_drawNitrousVehicle->current.enabled)
    {
        Phys_NitrousVecToVec3(&currentOrigin, start);
        Phys_NitrousVecToVec3(&this->m_script_goal_position, end);
        CL_AddDebugLine(start, end, colorGreen, 0, 1);
    }
    (angle_to_side) = fabs((float)((float)(userData->body->m_mat.y.x * to_goal.x) + (float)(userData->body->m_mat.y.y * to_goal.y)) + (float)(userData->body->m_mat.y.z * to_goal.z));
    angle_to_front = (float)((float)(userData->body->m_mat.x.x * to_goal.x)
        + (float)(userData->body->m_mat.x.y * to_goal.y))
        + (float)(userData->body->m_mat.x.z * to_goal.z);
    if (rbveh->m_script_brake == 0.0
        && goal_speed > 0.0
        && ((float)((float)((float)((float)(currentOrigin.x - this->m_stuck_position.x)
            * (float)(currentOrigin.x - this->m_stuck_position.x))
            + (float)((float)(currentOrigin.y - this->m_stuck_position.y)
                * (float)(currentOrigin.y - this->m_stuck_position.y)))
            + (float)((float)(currentOrigin.z - this->m_stuck_position.z)
                * (float)(currentOrigin.z - this->m_stuck_position.z))) < 225.0
            || this->m_stuck_time < 0.0))
    {
        this->m_stuck_time = this->m_stuck_time + delta_t;
    }
    else
    {
        p_m_stuck_position = &this->m_stuck_position;
        this->m_stuck_position.x = currentOrigin.x;
        p_m_stuck_position->y = currentOrigin.y;
        p_m_stuck_position->z = currentOrigin.z;
        this->m_stuck_time = 0.0f;
    }
    stuck_time = 1.0f;
    if (rbveh->m_vehicle_info->type == 2)
        stuck_time = 4.0f;
    if (this->m_stuck_time > stuck_time)
        this->m_stuck_time = stuck_time * -0.75;
    (current_speed) = fabs(rbveh->m_forward_vel);
    if (rbveh->m_vehicle_info->type != 2 && angle_to_side > 0.80000001 && to_goal_dist < 400.0)
        goto LABEL_23;
    if (this->m_stuck_time >= 0.0 || angle_to_front >= -0.5)
    {
        if (this->m_stuck_time < 0.0)
        {
        LABEL_23:
            NitrousVehicleController::UpdateVehicleInputs(rbveh, yaw + 180.0, 1.0, -1.6);
            goto LABEL_36;
        }
        if (to_goal_dist > 600.0 || current_speed > 200.0 && angle_to_front < -0.80000001 || (rbveh->m_flags & 0x100) != 0)
            NitrousVehicleController::UpdateVehicleInputs(rbveh, yaw, 1.0, 4.0);
        else
            NitrousVehicleController::UpdateVehicleInputs(rbveh, yaw, 1.0, 0.0);
    }
    else if (to_goal_dist <= 600.0)
    {
        NitrousVehicleController::UpdateVehicleInputs(rbveh, yaw, 1.0, 1.0);
    }
    else
    {
        NitrousVehicleController::UpdateVehicleInputs(rbveh, yaw, 1.0, -1.6);
    }
LABEL_36:
    if ((rbveh->m_flags & 0x100) != 0)
    {
        if (current_speed > goal_speed)
        {
            rbveh->set_throttle(0.0f);
            //NitrousVehicle::set_throttle(rbveh, 0.0);
            rbveh->set_hand_brake(1.0f);
            //NitrousVehicle::set_hand_brake(rbveh, (VDCArgs *)&delta_ta, 1.0);
        }
    }
    else if ((float)(this->m_script_goal_radius - 10.0) <= to_goal_dist)
    {
        if (to_goal_dist >= 1000.0 || current_speed <= goal_speed)
        {
            if (current_speed > goal_speed)
            {
                //NitrousVehicle::set_throttle(rbveh, 0.0);
                rbveh->set_throttle(0.0f);
            }
        }
        else
        {
            frac_to_goal = to_goal_dist / 1000.0;
            //max_speed = NitrousVehicle::get_max_speed(rbveh, 1);
            max_speed = rbveh->get_max_speed(1);
            speed_at_cur_dist = (max_speed - goal_speed) * frac_to_goal + goal_speed;
            if (current_speed <= speed_at_cur_dist)
            {
                if (current_speed > goal_speed && current_speed > 300.0)
                {
                    //NitrousVehicle::set_throttle(rbveh, 0.0);
                    rbveh->set_throttle(0.0f);
                }
            }
            else
            {
                //NitrousVehicle::set_throttle(rbveh, 0.0);
                rbveh->set_throttle(0.0f);
                //NitrousVehicle::set_hand_brake(rbveh, (VDCArgs *)&delta_ta, 1.0);
                rbveh->set_hand_brake(1.0f);
            }
        }
    }
    else if (this->m_stop_at_goal)
    {
        //NitrousVehicle::set_throttle(rbveh, 0.0);
        rbveh->set_throttle(0.0f);
        //NitrousVehicle::set_hand_brake(rbveh, (VDCArgs *)&delta_ta, 1.0);
        rbveh->set_hand_brake(1.0f);
        if (current_speed <= 1.0)
            rbveh->m_flags &= ~4u;
    }
    else
    {
        rbveh->m_flags &= ~4u;
    }
}

// -----------------------------------------------------------------------------
// NitrousVehicle::set_hand_brake
// CoDMPServer.c:660058 — RVA 0x0082B110
// -----------------------------------------------------------------------------
void NitrousVehicle::set_hand_brake(float braking)
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

// -----------------------------------------------------------------------------
// NitrousVehicleController::UpdateControlsStrafeMode
// CoDMPServer.c:660079 — RVA 0x0082B180
// -----------------------------------------------------------------------------
void NitrousVehicleController::UpdateControlsStrafeMode(
                NitrousVehicle *rbveh)
{
    float tank_forward_sign;
    float clamped_input;
    float side_sign;
    float forward_sign;
    float rb_angles[3];
    cg_s *cgameGlob;
    gentity_s *driver;
    int driver_occupant;
    float camera_yaw;
    float move[2];
    float forwardPreference;
    float vehicleDeadZone;
    float input_yaw;
    float input;
    float vec[2];
    camera_yaw = 0.0f;
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
        move[0] = 0.0f;
    vehicleDeadZone = 40.0f;
    if ( fabs(move[1]) >= 40.0 )
    {
        if ( move[1] < 0.0 )
            forward_sign = -1.0f;
        else
            forward_sign = 1.0f;
        vec[0] = (float)((float)(fabs(move[1]) - vehicleDeadZone)
                                     / (float)(128.0 - vehicleDeadZone))
                     * forward_sign;
    }
    else
    {
        vec[0] = 0.0f;
    }
    if ( vehicleDeadZone <= fabs(move[0]) )
    {
        if ( move[0] < 0.0 )
            side_sign = -1.0f;
        else
            side_sign = 1.0f;
        vec[1] = (float)((float)(fabs(move[0]) - vehicleDeadZone)
                                     / (float)(128.0 - vehicleDeadZone))
                     * side_sign;
    }
    else
    {
        vec[1] = 0.0f;
    }
    input = Vec2Length(vec);
    if ( input <= 1.0 )
        clamped_input = input;
    else
        clamped_input = 1.0f;
    input = clamped_input;
    input_yaw = vectoyaw(vec) + camera_yaw + 180.0;
    input_yaw = AngleNormalize180(input_yaw);
    if ( clamped_input == 0.0 )
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
    forwardPreference = 0.0f;
    if ( !player_topDownCamMode->current.integer )
    {
        if ( rbveh->m_vehicle_info->cameraMode == 2 )
        {
            if ( this->m_cmd.forwardmove < (char)(int)(-(vehicleDeadZone)) )
                tank_forward_sign = -1.0f;
            else
                tank_forward_sign = 1.0f;
            forwardPreference = tank_forward_sign;
        }
        else
        {
            forwardPreference = -vec[0];
        }
    }
    if ( rbveh->m_vehicle_info->type != 2 )
        forwardPreference = forwardPreference * 4.0;
    NitrousVehicleController::UpdateVehicleInputs(
        rbveh,
        input_yaw,
        input,
        forwardPreference);
}

// -----------------------------------------------------------------------------
// NitrousVehicleController::UpdateVehicleInputs
// CoDMPServer.c:660209 — RVA 0x0082B5B0
// -----------------------------------------------------------------------------
// local variable allocation has failed, the output may be wrong!
void NitrousVehicleController::UpdateVehicleInputs(
                NitrousVehicle *rbveh,
                float target_yaw,
                float target_accel,
                float forward_preference)
{
    float hand_brake_min_speed;
    bool apply_hand_brake;
    float final_steer;
    float clamped_steer;
    float yaw_steer_range;
    float steer_reverse_sign;
    float steerFactor;
    phys_vec3 *velocity;
    phys_vec3 velocity_vec;
    float forward_vel;
    float fwd;
    float throttle_sign;
    float reverse_yaw_sign;
    float anglePreference;
    float m_forward_vel;
    float total_input_pref;
    float total_base_forward_pref;
    bool reverse;
    float coast_factor;
    const VehicleParameter *m_parameter;
    float rb_angles[3];
    float _pt[4];
    phys_vec3 pt;
    phys_mat44 *p_m_mat;
    float input_fwd[3];
    const PhysObjUserData *userData;
    float delta_yaw;
    float hand_brake;
    delta_yaw = 0.0f;
    if ((target_accel != 0.0 || rbveh->m_vehicle_info->type != 2) && (rbveh->m_flags & 1) == 0)
    {
        userData = rbveh->m_phys_user_data;
        if (phys_drawNitrousVehicle->current.enabled)
        {
            YawVectors(target_yaw, input_fwd, 0);
            p_m_mat = &userData->body->m_mat;
            pt = p_m_mat->w;
            _pt[3] = pt.z;
            pt.z = pt.z + 100.0;
            Phys_NitrousVecToVec3(&pt, _pt);
            input_fwd[0] = (float)(450.0 * input_fwd[0]) + _pt[0];
            input_fwd[1] = (float)(450.0 * input_fwd[1]) + _pt[1];
            input_fwd[2] = (float)(450.0 * input_fwd[2]) + _pt[2];
            CL_AddDebugLine(_pt, input_fwd, colorGreen, 0, 1);
        }
        Axis4ToAngles((const float (*)[4]) & userData->body->m_mat, rb_angles);
        delta_yaw = AngleDelta(target_yaw, rb_angles[1]);
    }
    if (rbveh->m_vehicle_info->type != 2)
    {
        m_parameter = rbveh->m_parameter;
        coast_factor = 3.0 * m_parameter->m_tire_damp_coast;
        rbveh->m_coasting_factor = coast_factor * rbveh->m_parameter->m_body_mass;
    }
    reverse = 0;
    total_base_forward_pref = base_forward_pref;
    total_input_pref = input_pref;
    if (rbveh->m_vehicle_info->type != 2)
    {
        total_input_pref = total_input_pref + 20.0;
        m_forward_vel = rbveh->m_forward_vel;
        if (m_forward_vel > speed_pref)
            total_base_forward_pref = total_base_forward_pref + 30.0;
    }
    anglePreference = (float)(total_input_pref * forward_preference) + total_base_forward_pref;
    if ((fabs(delta_yaw)) > anglePreference)
    {
        if (delta_yaw < 0.0)
            reverse_yaw_sign = -1.0f;
        else
            reverse_yaw_sign = 1.0f;
        delta_yaw = delta_yaw - (float)(180.0 * reverse_yaw_sign);
        reverse = 1;
    }
    if (reverse)
        throttle_sign = -1.0f;
    else
        throttle_sign = 1.0f;
    //NitrousVehicle::set_throttle(rbveh, throttle_sign * target_accel);
    rbveh->set_throttle(throttle_sign * target_accel);
    if (rbveh->m_vehicle_info->cameraMode == 2)
    {
        fwd = rbveh->m_forward_vel;
        if (target_accel != 0.0f && ((fwd > 25.0f && reverse) || (fwd < -25.0f && !reverse)))
            delta_yaw = delta_yaw * -1.0f;
        hand_brake = rbveh->m_hand_brake;
        // Decomp: IDA lists (steer>=0.5)&&(steer<=-0.5) on same register — unsatisfiable; active binary never uses it.
        if (hand_brake <= 0.1f
            || ((fwd <= 25.0f || !reverse) && (fwd >= -25.0f || reverse)))
        {
            rbveh->set_brake(0.0f);
        }
        else
        {
            rbveh->set_brake(1.0f);
        }
        goto LABEL_46;
    }
    forward_vel = rbveh->m_forward_vel;
    if (target_accel == 0.0 || (forward_vel <= 25.0 || !reverse) && (forward_vel >= -25.0 || reverse) || rbveh->m_vehicle_info->type == 4)
    {
    LABEL_45:
        rbveh->set_brake(0.0f);
        //NitrousVehicle::set_brake(rbveh, (VDCArgs *)&target_yawa, 0.0);
        goto LABEL_46;
    }
    //velocity = NitrousVehicle::get_velocity(rbveh, &velocity_vec);
    velocity = rbveh->get_velocity(&velocity_vec);
    if ((float)((float)((float)(velocity->x * velocity->x) + (float)(velocity->y * velocity->y)) + (float)(velocity->z * velocity->z)) > (float)(steer_velocity * steer_velocity))
        delta_yaw = 0.0f;
    delta_yaw = delta_yaw * -1.0;
    //NitrousVehicle::set_brake(rbveh, (VDCArgs *)&target_yawa, 1.0);
    rbveh->set_brake(1.0f);
LABEL_46:
    steerFactor = 0.0f;
    if (delta_yaw <= 0.0)
    {
        if (delta_yaw < 0.0)
            steerFactor = -1.0f;
    }
    else
    {
        steerFactor = 1.0f;
    }
    if (reverse)
        steer_reverse_sign = -1.0f;
    else
        steer_reverse_sign = 1.0f;
    steerFactor = steerFactor * steer_reverse_sign;
    yaw_steer_range = rbveh->m_parameter->m_steer_angle_max * 57.295776;
    if (reverse && rbveh->m_vehicle_info->type != 2)
        yaw_steer_range = yaw_steer_range * 2.0;
    if (yaw_steer_range > (fabs(delta_yaw)))
        steerFactor = (float)((fabs(delta_yaw)) / yaw_steer_range) * steerFactor;
    if ((float)(steerFactor - 1.0) < 0.0)
        clamped_steer = steerFactor;
    else
        clamped_steer = 1.0f;
    if ((float)(-1.0 - steerFactor) < 0.0)
        final_steer = clamped_steer;
    else
        final_steer = -1.0f;
    rbveh->m_steer_factor = final_steer;
    apply_hand_brake = 0;
    hand_brake_min_speed = rbveh->m_parameter->m_auto_hand_brake_min_speed;
    phys_vec3 tmp_vel;
    rbveh->get_velocity(&tmp_vel);
    float velSq = Vec3LengthSq((float*)&tmp_vel);
    apply_hand_brake = (fabs(delta_yaw)) > hand_brake_apply_delta && velSq > (hand_brake_min_speed * hand_brake_min_speed);
        //&& (LODWORD(v6.z) = NitrousVehicle::get_velocity(rbveh, (phys_vec3 *)&v6.w),
        //    (float)((float)((float)(*(float *)LODWORD(v6.z) * *(float *)LODWORD(v6.z))
        //        + (float)(*(float *)(LODWORD(v6.z) + 4) * *(float *)(LODWORD(v6.z) + 4)))
        //        + (float)(*(float *)(LODWORD(v6.z) + 8) * *(float *)(LODWORD(v6.z) + 8))) > (float)(hand_brake_min_speed * hand_brake_min_speed));
    if (apply_hand_brake)
    {
        if (delta_yaw >= 0.0 || rbveh->m_phys_user_data->body->m_a_vel.z <= 0.0)
        {
            if (delta_yaw > 0.0 && rbveh->m_phys_user_data->body->m_a_vel.z < 0.0)
                apply_hand_brake = 0;
        }
        else
        {
            apply_hand_brake = 0;
        }
    }
    //if (bitarray<51>::testBit(&this->m_cmd.button_bits, 0x17u))
    if (this->m_cmd.button_bits.testBit(0x17u))
        apply_hand_brake = 1;
    if (apply_hand_brake)
    {
        //NitrousVehicle::set_hand_brake(rbveh, (VDCArgs *)&target_yawa, 1.0);
        rbveh->set_hand_brake(1.0f);
    }
    else
    {
        //NitrousVehicle::set_hand_brake(rbveh, (VDCArgs *)&target_yawa, 0.0);
        rbveh->set_hand_brake(0.0f);
    }
}

// -----------------------------------------------------------------------------
// NitrousVehicle::set_brake
// CoDMPServer.c:660412 — RVA 0x0082BEC0
// -----------------------------------------------------------------------------
void NitrousVehicle::set_brake(float braking)
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

// -----------------------------------------------------------------------------
// NitrousVehicleController::UpdateStun
// CoDMPServer.c:660433 — RVA 0x0082BF30
// -----------------------------------------------------------------------------
void NitrousVehicleController::UpdateStun(
                NitrousVehicle *rbveh,
                float delta_t)
{
    float stun_throttle;
    float desired_throttle;
    float desired_brake;
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
                stun_throttle = (float)((float)((float)(throttle_max_scalar - throttle_min_scalar)
                                                     * (float)((float)(this->m_stun.m_stun_time - stun_time_min)
                                                                     / (float)(stun_time_max - stun_time_min)))
                                     + throttle_min_scalar)
                     * desired_throttle;
            else
                stun_throttle = 0.0f;
            this->m_stun.m_last_throttle = stun_throttle;
            this->m_stun.m_changed_this_frame = 1;
        }
        //NitrousVehicle::set_brake(rbveh, this->m_stun.m_last_brake);
        rbveh->set_brake(this->m_stun.m_last_brake);
        //NitrousVehicle::set_throttle(rbveh, this->m_stun.m_last_throttle);
        rbveh->set_throttle(this->m_stun.m_last_throttle);
        this->m_stun.m_stun_time = this->m_stun.m_stun_time - delta_t;
    }
    else
    {
        this->m_stun.m_stun_time = 0.0f;
        this->m_stun.m_last_throttle = 0.0f;
        this->m_stun.m_last_brake = 0.0f;
        this->m_stun.m_changed_this_frame = 0;
    }
}

// -----------------------------------------------------------------------------
// StunRandomBrake
// CoDMPServer.c:660479 — RVA 0x0082C110
// -----------------------------------------------------------------------------
double __cdecl StunRandomBrake(const usercmd_s *cmd, float desired_brake, float last_brake)
{
    //if ( bitarray<51>::testBit(&cmd->button_bits, 0x16u) && desired_brake > 0.5 )
    if ( cmd->button_bits.testBit(0x16u) && desired_brake > 0.5 )
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

// -----------------------------------------------------------------------------
// NitrousVehicleController::UpdateControls
// CoDMPServer.c:660501 — RVA 0x0082C210
// -----------------------------------------------------------------------------
void NitrousVehicleController::UpdateControls(
                NitrousVehicle *rbveh,
                float delta_t)
{
    float reverse_forward_vel;
    float m_forward_vel;
    float goal_y;
    float goal_z;
    scr_vehicle_s *scr_vehicle;
    float turret_steer_factor;
    float turret_steer_clamped;
    float turret_steer_raw;
    if ( (rbveh->m_flags & 4) != 0 )
    {
        NitrousVehicleController::UpdateScriptVehicleControl(rbveh, delta_t);
        if ( rbveh->m_vehicle_info->type == 4 )
            NitrousVehicleController::UpdateApplyBoatAccel(rbveh, delta_t);
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
                //if ( ((LODWORD(scr_vehicle->pathPos.origin[0]) & 0x7F800000) == 0x7F800000
                //     || (LODWORD(scr_vehicle->pathPos.origin[1]) & 0x7F800000) == 0x7F800000
                //     || (LODWORD(scr_vehicle->pathPos.origin[2]) & 0x7F800000) == 0x7F800000)
                //    && !Assert_MyHandler(
                //                "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle_controller.cpp",
                //                515,
                //                0,
                //                "%s",
                //                "!IS_NAN((vpp.origin)[0]) && !IS_NAN((vpp.origin)[1]) && !IS_NAN((vpp.origin)[2])") )
                //{
                //    __debugbreak();
                //}
                //if ( ((LODWORD(scr_vehicle->pathPos.lookPos[0]) & 0x7F800000) == 0x7F800000
                //     || (LODWORD(scr_vehicle->pathPos.lookPos[1]) & 0x7F800000) == 0x7F800000
                //     || (LODWORD(scr_vehicle->pathPos.lookPos[2]) & 0x7F800000) == 0x7F800000)
                //    && !Assert_MyHandler(
                //                "C:\\projects_pc\\cod\\codsrc\\src\\vehicle\\nitrous_vehicle_controller.cpp",
                //                516,
                //                0,
                //                "%s",
                //                "!IS_NAN((vpp.lookPos)[0]) && !IS_NAN((vpp.lookPos)[1]) && !IS_NAN((vpp.lookPos)[2])") )
                //{
                //    __debugbreak();
                //}
                goal_y = scr_vehicle->pathPos.lookPos[1];
                goal_z = scr_vehicle->pathPos.lookPos[2];
                this->m_script_goal_position.x = scr_vehicle->pathPos.lookPos[0];
                this->m_script_goal_position.y = goal_y;
                this->m_script_goal_position.z = goal_z;
                this->m_script_goal_radius = 5.0f;
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
            NitrousVehicleController::UpdateScriptVehicleControl(rbveh, delta_t);
            goto LABEL_60;
        }
        if ( rbveh->m_vehicle_info->type == 2 )
        {
            if ( rbveh->m_vehicle_info->cameraMode == 5 )
                NitrousVehicleController::UpdateControlsTank(rbveh);
            else
                NitrousVehicleController::UpdateControlsStrafeMode(rbveh);
            m_forward_vel = rbveh->m_forward_vel;
            //if ( m_forward_vel <= NitrousVehicle::get_max_speed(rbveh, 1) || rbveh->m_throttle <= 0.0 )
            if ( m_forward_vel <= rbveh->get_max_speed(1) || rbveh->m_throttle <= 0.0 )
            {
                reverse_forward_vel = rbveh->m_forward_vel;
                //if (-NitrousVehicle::get_max_speed(rbveh, 1) > reverse_forward_vel && rbveh->m_throttle < 0.0)
                if (-rbveh->get_max_speed(1) > reverse_forward_vel && rbveh->m_throttle < 0.0)
                {
                    rbveh->set_throttle(0.0f);
                    //NitrousVehicle::set_throttle(rbveh, 0.0);
                }
            }
            else
            {
                //NitrousVehicle::set_throttle(rbveh, 0.0);
                rbveh->set_throttle(0.0f);
            }
            goto LABEL_59;
        }
        if ( rbveh->m_vehicle_info->cameraMode == 2 || rbveh->m_vehicle_info->cameraMode == 3 )
            goto LABEL_56;
        if ( rbveh->m_vehicle_info->cameraMode == 5 )
            goto LABEL_52;
        if ( rbveh->m_vehicle_info->cameraMode != 4 )
        {
            NitrousVehicleController::UpdateControlsDefault(rbveh, delta_t);
            goto LABEL_59;
        }
        if ( vehControlMode && vehControlMode->current.integer )
LABEL_56:
            NitrousVehicleController::UpdateControlsStrafeMode(rbveh);
        else
LABEL_52:
            NitrousVehicleController::UpdateControlsTank(rbveh);
LABEL_59:
        NitrousVehicleController::UpdateJump(rbveh, delta_t);
        goto LABEL_60;
    }
    if ( (rbveh->m_flags & 1) != 0 )
    {
        //NitrousVehicle::start_path(rbveh, 0);
        rbveh->start_path(0);
    }
    else
    {
        turret_steer_raw = (float)(rbveh->m_owner->s.lerp.u.turret.gunAngles[0] * 0.017453292) * tweaker_0;
        if ( (float)(turret_steer_raw - 1.0) < 0.0 )
            turret_steer_clamped = (float)(rbveh->m_owner->s.lerp.u.turret.gunAngles[0] * 0.017453292) * tweaker_0;
        else
            turret_steer_clamped = 1.0f;
        if ( (float)(-1.0 - turret_steer_raw) < 0.0 )
            turret_steer_factor = turret_steer_clamped;
        else
            turret_steer_factor = -1.0f;
        rbveh->m_steer_factor = turret_steer_factor;
        //NitrousVehicle::set_throttle(rbveh, 0.0);
        rbveh->set_throttle(0.0f);
        //NitrousVehicle::set_hand_brake(rbveh, 0.0);
        rbveh->set_hand_brake(0.0f);
    }
LABEL_60:
    if ( rbveh->m_vehicle_info->type == 4 )
        NitrousVehicleController::UpdateApplyBoatAccel(rbveh, delta_t);
    NitrousVehicleController::UpdateStun(rbveh, delta_t);
}

// -----------------------------------------------------------------------------
// RecalibrateInputCustom
// CoDMPServer.c:660652 — RVA 0x0082C880
// -----------------------------------------------------------------------------
double __cdecl RecalibrateInputCustom(float val, float threshold)
{
    float sign;
    if ( threshold <= fabs(val) )
    {
        if ( val <= 0.0 )
            sign = -1.0f;
        else
            sign = 1.0f;
        return (float)((float)((float)((float)(fabs(val) - threshold)
                                                                 / (float)(128.0 - threshold))
                                                 * 128.0)
                                 * sign);
    }
    else
    {
        return 0.0f;
    }
}

// -----------------------------------------------------------------------------
// NitrousVehicleController::UpdateControlsDefault
// CoDMPServer.c:660679 — RVA 0x0082C920
// -----------------------------------------------------------------------------
// local variable allocation has failed, the output may be wrong!
void NitrousVehicleController::UpdateControlsDefault(
                NitrousVehicle *rbveh,
                float delta_t)
{
    bool needs_handbrake_stop;
    phys_vec3 *velocity;
    float *body_up_vec;
    int body_mat_addr;
    PhysObjUserData *m_phys_user_data;
    bool is_not_boat;
    float AnalogButtonValue;
    float final_steer;
    float clamped_steer;
    float gas;
    float fraction;
    float steer_input;
    float m_tire_damp_coast;
    const VehicleParameter *m_parameter;
    const vehicle_info_t *vehicle_info;
    vehicle_info = rbveh->m_vehicle_info;
    m_parameter = rbveh->m_parameter;
    m_tire_damp_coast = m_parameter->m_tire_damp_coast;
    rbveh->m_coasting_factor = m_tire_damp_coast * rbveh->m_parameter->m_body_mass;
    steer_input = (float)-this->m_cmd.rightmove / 128.0;
    if ( vehicle_info->steerGraph )
    {
        fraction = fabs(steer_input);
        if ( steer_input < 0.0 )
            gas = -1.0f;
        else
            gas = 1.0f;
        steer_input = GraphFloat_GetValue(vehicle_info->steerGraph, fraction) * gas;
    }
    if ( (float)(steer_input - 1.0) < 0.0 )
        clamped_steer = steer_input;
    else
        clamped_steer = 1.0f;
    if ( (float)(-1.0 - steer_input) < 0.0 )
        final_steer = clamped_steer;
    else
        final_steer = -1.0f;
    rbveh->m_steer_factor = final_steer;
    if ( rbveh->m_vehicle_info->cameraMode == 4 )
        AnalogButtonValue = (float)this->m_cmd.forwardmove / 127.0;
    else
        AnalogButtonValue = CMD_GetAnalogButtonValue(&this->m_cmd, 0x13u);
    is_not_boat = rbveh->m_vehicle_info->type != 4;
    rbveh->set_hand_brake(0.0f);
    rbveh->set_throttle(0.0f);
    rbveh->set_brake(0.0f);
    //NitrousVehicle::set_hand_brake(rbveh, 0.0);
    //NitrousVehicle::set_throttle(rbveh, 0.0);
    //NitrousVehicle::set_brake(rbveh, 0.0);
    if ( AnalogButtonValue > 0.0 )
    {
        //NitrousVehicle::set_throttle(rbveh, AnalogButtonValue);
        rbveh->set_throttle(AnalogButtonValue);
        this->m_stopped_time = 0.0f;
    }
    //if ( !bitarray<51>::testBit(&this->m_cmd.button_bits, 0x16u) || rbveh->m_vehicle_info->cameraMode == 4 )
    if ( !this->m_cmd.button_bits.testBit(0x16u) || rbveh->m_vehicle_info->cameraMode == 4 )
    {
        if (AnalogButtonValue < 0.0 && rbveh->m_vehicle_info->cameraMode == 4)
        {
            //NitrousVehicle::set_throttle(rbveh, AnalogButtonValue);
            rbveh->set_throttle(AnalogButtonValue);
        }
    }
    else
    {
        if ( is_not_boat )
        {
            m_phys_user_data = rbveh->m_phys_user_data;
            body_mat_addr = (int)&m_phys_user_data->body->m_mat;
            body_up_vec = (float *)(body_mat_addr + 16);
            //velocity = NitrousVehicle::get_velocity(rbveh, (phys_vec3 *)&v6);
            phys_vec3 tmp;
            //velocity = rbveh->get_velocity((phys_vec3 *)&v6);
            velocity = rbveh->get_velocity(&tmp);
            needs_handbrake_stop = rbveh->m_forward_vel <= 1.0
                && (float)(fabs((float)((float)(velocity->x * *body_up_vec) + (float)(velocity->y * body_up_vec[1])) + (float)(velocity->z * body_up_vec[2]))
                                 - 2.0) > fabs(rbveh->m_forward_vel)
                && required_stop_time > this->m_stopped_time;
            if ( rbveh->m_forward_vel > 1.0 || needs_handbrake_stop )
            {
                this->m_stopped_time = 0.0f;
                if ( needs_handbrake_stop )
                    this->m_stopped_time = -0.2f;
                if ( fabs(rbveh->m_steer_factor) > breakHandbrakeThreshold
                    || AnalogButtonValue > 0.0 )
                {
LABEL_36:
                    //NitrousVehicle::set_hand_brake(rbveh, 1.0);
                    rbveh->set_hand_brake(1.0f);
                    goto LABEL_42;
                }
            }
            else
            {
                this->m_stopped_time = this->m_stopped_time + delta_t;
                if ( this->m_stopped_time > required_stop_time )
                {
                    //NitrousVehicle::set_throttle(rbveh, -1.0);
                    rbveh->set_throttle(-1.0f);
                    goto LABEL_42;
                }
                if ( AnalogButtonValue > 0.0 )
                    goto LABEL_36;
            }
            //NitrousVehicle::set_brake(rbveh, 1.0);
            rbveh->set_brake(1.0f);
            goto LABEL_42;
        }
        //NitrousVehicle::set_throttle(rbveh, -1.0);
        rbveh->set_throttle(-1.0f);
    }
LABEL_42:
    if ( !is_not_boat )
    {
        //if ( bitarray<51>::testBit(&this->m_cmd.button_bits, 0x17u) )
        if (this->m_cmd.button_bits.testBit(0x17u))
        {
            //NitrousVehicle::set_hand_brake(rbveh, 1.0);
            rbveh->set_hand_brake(1.0f);
        }
    }
}

// -----------------------------------------------------------------------------
// NitrousVehicleController::UpdateControlsTank
// CoDMPServer.c:660798 — RVA 0x0082CDE0
// -----------------------------------------------------------------------------
void NitrousVehicleController::UpdateControlsTank(NitrousVehicle *rbveh)
{
    float final_throttle;
    float clamped_throttle;
    float steer_scale;
    float steer_sign;
    float final_steer;
    float clamped_steer;
    float steering;
    float normalized_x;
    float input_dist;
    float raw_rightmove;
    float x;
    float y;
    float throttle;
    float normalized_throttle;
    rbveh->m_coasting_factor = rbveh->m_parameter->m_tire_damp_coast * rbveh->m_parameter->m_body_mass;
    raw_rightmove = (float)this->m_cmd.rightmove;
    y = RecalibrateInputCustom((float)this->m_cmd.forwardmove * -1.0, 40.0);
    x = RecalibrateInputCustom(raw_rightmove, 40.0);
    normalized_x = 0.0f;
    input_dist = sqrtf((float)(x * x) + (float)(y * y));
    if ( input_dist != 0.0 )
        normalized_x = x / input_dist;
    if ( normalized_x < 0.0 )
        steer_sign = -1.0f;
    else
        steer_sign = 1.0f;
    steering = (-(normalized_x * normalized_x)) * steer_sign;
    if ( fabs(steering) < 0.98000002 )
        steering = steering * steer_reduce_frac;
    if ( rbveh->m_vehicle_info->cameraMode == 4 || rbveh->m_vehicle_info->type != 2 )
        steer_scale = 1.0f;
    else
        steer_scale = 0.25f;
    throttle = (float)((float)((float)((float)(normalized_x * x) / 129.0) * steer_scale)
                                     * (float)((float)((float)(normalized_x * x) / 129.0) * steer_scale))
                     + (float)((float)(y / 129.0) * (float)(y / 129.0));
    if ( throttle == 0.0 )
        normalized_throttle = 0.0f;
    else
        normalized_throttle = throttle / sqrtf(throttle);
    if ( y > 0.0 )
        normalized_throttle = normalized_throttle * -1.0;
    if ( rbveh->m_forward_vel >= 0.0 || normalized_throttle <= 0.0 )
    {
        if (rbveh->m_forward_vel > 0.0 && normalized_throttle < 0.0)
        {
            //LODWORD(steering) ^= _mask__NegFloat_;
            (steering) = -steering;
        }
    }
    else
    {
        //LODWORD(steering) ^= _mask__NegFloat_;
        steering = -steering;
    }
    if ( (float)(steering - 1.0) < 0.0 )
        clamped_steer = steering;
    else
        clamped_steer = 1.0f;
    if ( (float)(-1.0 - steering) < 0.0 )
        final_steer = clamped_steer;
    else
        final_steer = -1.0f;
    rbveh->m_steer_factor = final_steer;
    if ( (float)(normalized_throttle - 1.0) < 0.0 )
        clamped_throttle = normalized_throttle;
    else
        clamped_throttle = 1.0f;
    if ( (float)(-1.0 - normalized_throttle) < 0.0 )
        final_throttle = clamped_throttle;
    else
        final_throttle = -1.0f;
    rbveh->set_throttle(final_throttle);
    rbveh->set_hand_brake(0.0f);
    rbveh->set_brake(0.0f);
    //NitrousVehicle::set_throttle(rbveh, v2);
    //NitrousVehicle::set_hand_brake(rbveh, 0.0);
    //NitrousVehicle::set_brake(rbveh, 0.0);
}

// -----------------------------------------------------------------------------
// NitrousVehicleController::SetScriptTarget
// CoDMPServer.c:660879 — RVA 0x0082D1F0
// -----------------------------------------------------------------------------
void NitrousVehicleController::SetScriptTarget(
                NitrousVehicle *rbveh,
                phys_vec3 *goal_position,
                float goal_radius,
                float goal_speed,
                bool stopAtGoal)
{
    float clamped_radius;
    float clamped_radius_max;
    this->m_script_goal_position.x = goal_position->x;
    this->m_script_goal_position.y = goal_position->y;
    this->m_script_goal_position.z = goal_position->z;
    if ( (float)(goal_radius - 5000.0) < 0.0 )
        clamped_radius_max = goal_radius;
    else
        clamped_radius_max = 5000.0f;
    if ( (float)(5.0 - goal_radius) < 0.0 )
        clamped_radius = clamped_radius_max;
    else
        clamped_radius = 5.0f;
    this->m_script_goal_radius = clamped_radius;
    this->m_script_goal_speed = goal_speed;
    this->m_stop_at_goal = stopAtGoal;
    rbveh->m_flags |= 4u;
}

// -----------------------------------------------------------------------------
// NitrousVehicleController::UpdateJump
// CoDMPServer.c:660905 — RVA 0x0082D300
// -----------------------------------------------------------------------------
void NitrousVehicleController::UpdateJump(
                NitrousVehicle *rbveh,
                float delta_t)
{
    phys_vec3 jump_force_vec;
    float m_jump_force;
    const VehicleParameter *vehParams;
    rigid_body *body;
    int m_num_colliding_wheels;
    if (this->m_cmd.button_bits.testBit(0x24u))
    {
        m_num_colliding_wheels = rbveh->m_num_colliding_wheels;
        if (m_num_colliding_wheels >= 2 && !this->m_jump_button_pressed)
        {
            body = rbveh->m_phys_user_data->body;
            vehParams = rbveh->m_parameter;
            m_jump_force = vehParams->m_jump_force;
            jump_force_vec.x = 0.0f;
            jump_force_vec.y = 0.0f;
            jump_force_vec.z = m_jump_force;
            //rigid_body::add_force(body, &jump_force_vec);
            body->add_force(&jump_force_vec);
            this->m_jump_button_pressed = 1;
        }
    }
    else
    {
        this->m_jump_button_pressed = 0;
    }
}

//void rigid_body::add_force(const phys_vec3 *force)
//{
//    this->m_force_sum.x = this->m_force_sum.x + force->x;
//    this->m_force_sum.y = this->m_force_sum.y + force->y;
//    this->m_force_sum.z = this->m_force_sum.z + force->z;
//    if ( fabs(this->m_force_sum.x) > 100000.0
//        || fabs(this->m_force_sum.y) > 100000.0
//        || fabs(this->m_force_sum.z) > 100000.0 )
//    {
//        phys_exec_debug_callback(this);
//    }
//}
// -----------------------------------------------------------------------------
// NitrousVehicleController::UpdateApplyBoatAccel
// CoDMPServer.c:660979 — RVA 0x0082D7B0
// -----------------------------------------------------------------------------
// local variable allocation has failed, the output may be wrong!
void NitrousVehicleController::UpdateApplyBoatAccel(
                NitrousVehicle *rbveh,
                float delta_t)
{
    double abs_yaw_rate;
    float forward_force_scale;
    float turn_force_scale;
    phys_vec3 turn_force;
    float clamped_turn_throttle;
    float turn_throttle_clamp;
    bool apply_ebrake;
    BOOL should_apply_ebrake;
    float forward_vel_for_ebrake;
    float steer_factor_abs;
    float forward_vel_reverse;
    float forward_vel_coast;
    float turn_throttle;
    float debug_line_start[3];
    const phys_vec3 *rotated_accel;
    phys_vec3 rotated_accel_vec;
    phys_mat44 rv;
    float rot;
    float steer_factor;
    phys_vec3 accel;
    phys_vec3 motor_position;
    float motor_offset_z;
    float motor_offset_x;
    float up_offset_x;
    float up_offset_y;
    float up_offset_z;
    float forward_offset_x;
    float forward_offset_y;
    float forward_offset_z;
    float throttle;
    float rotSpeedFactor;
    float speedFactor;
    float turnProportion;
    float forwardProportion;
    float m_forward_vel;
    phys_vec3 forwardForce;
    float pos[3];
    phys_vec3 center;
    const vehicle_info_t *info;
    const VehicleParameter *vehParams;
    rigid_body *body;
    if ((rbveh->m_flags & 0x400) != 0)
    {
        body = rbveh->m_phys_user_data->body;
        vehParams = rbveh->m_parameter;
        info = rbveh->m_vehicle_info;
        center = body->m_mat.w;
        Phys_NitrousVecToVec3(&center, pos);
        forwardForce = body->m_mat.x;
        m_forward_vel = rbveh->m_forward_vel;
        forwardProportion = (fabs(m_forward_vel))
            / (float)(vehParams->m_water_speed_max * rbveh->m_speed_scale);
        if (forwardProportion > 1.0)
            forwardProportion = 1.0f;
        abs_yaw_rate = Abs(&body->m_a_vel.x);
        turnProportion = abs_yaw_rate / vehParams->m_water_turn_speed_max;
        if (turnProportion > 1.0)
            turnProportion = 1.0f;
        speedFactor = 1.0 - forwardProportion;
        rotSpeedFactor = (float)(1.0 - forwardProportion) * 0.5;
        if (info->accelGraph)
        {
            speedFactor = GraphFloat_GetValue(info->accelGraph, (fabs(forwardProportion)));
        }
        speedFactor = (float)((float)((float)(delta_t * 60.0) * vehParams->m_water_accel_max) * rbveh->m_speed_scale)
            * speedFactor;
        rotSpeedFactor = (float)((float)((float)(delta_t * 60.0) * vehParams->m_water_turn_accel) * rbveh->m_drivepath_scale)
            * rotSpeedFactor;
        throttle = rbveh->m_throttle;
        if (throttle < 0.0)
        {
            if (rbveh->m_forward_vel < 1.0)
                speedFactor = speedFactor * vehParams->m_reverse_scale;
        }
        // Decomp: CoDMPServer.c keeps twin-force path behind global `dothisoldstuff`; shipped binary uses this motor-offset branch only.
        motor_offset_z = vehParams->m_boat_motor_offset[2];
        up_offset_x = motor_offset_z * body->m_mat.z.x;
        up_offset_y = motor_offset_z * body->m_mat.z.y;
        up_offset_z = motor_offset_z * body->m_mat.z.z;
        motor_offset_x = vehParams->m_boat_motor_offset[0];
        forward_offset_x = motor_offset_x * body->m_mat.x.x;
        forward_offset_y = motor_offset_x * body->m_mat.x.y;
        forward_offset_z = motor_offset_x * body->m_mat.x.z;
        motor_position.x = (float)(center.x + forward_offset_x) + up_offset_x;
        motor_position.y = (float)(center.y + forward_offset_y) + up_offset_y;
        motor_position.z = (float)(center.z + forward_offset_z) + up_offset_z;
        accel = body->m_mat.x;
        steer_factor = rbveh->m_steer_factor;
        //rot = COERCE_FLOAT(LODWORD(steer_factor) ^ _mask__NegFloat_) * vehParams->m_steer_angle_max;
        rot = (-(steer_factor)) * vehParams->m_steer_angle_max;
        make_rotate(&rv, &body->m_mat.z, rot, 1000.0);
        rotated_accel = phys_multiply(&rotated_accel_vec, &rv, &accel);
        accel.x = rotated_accel->x;
        accel.y = rotated_accel->y;
        accel.z = rotated_accel->z;
        if (phys_drawNitrousVehicle->current.enabled)
        {
            phys_vec3 debug_line_end_src;
            float debug_line_end[3];
            Phys_NitrousVecToVec3(&motor_position, debug_line_start);
            debug_line_end_src.x = motor_position.x - (float)(50.0 * accel.x);
            debug_line_end_src.y = motor_position.y - (float)(50.0 * accel.y);
            debug_line_end_src.z = motor_position.z - (float)(50.0 * accel.z);
            Phys_NitrousVecToVec3(&debug_line_end_src, debug_line_end);
            CL_AddDebugLine(debug_line_start, debug_line_end, colorWhite, 0, 1);
        }
        turn_throttle = throttle;
        if (rbveh->m_vehicle_info->cameraMode == 1)
        {
            if (turn_throttle >= 1.0 || turn_throttle <= 0.0)
            {
                if (turn_throttle == 0.0)
                {
                    turn_throttle = forwardProportion * coast_steer_scaler;
                    forward_vel_coast = rbveh->m_forward_vel;
                    if (forward_vel_coast < 0.0)
                        turn_throttle = turn_throttle * -1.0;
                }
                else if (turn_throttle < 0.0)
                {
                    forward_vel_reverse = rbveh->m_forward_vel;
                    if (forward_vel_reverse > 0.0)
                        turn_throttle = turn_throttle * -1.0;
                    steer_factor_abs = rbveh->m_steer_factor;
                    should_apply_ebrake = (fabs(steer_factor_abs)) > 0.60000002
                        && (forward_vel_for_ebrake = rbveh->m_forward_vel, forward_vel_for_ebrake > 100.0);
                    apply_ebrake = should_apply_ebrake;
                    if (should_apply_ebrake)
                    {
                        rotSpeedFactor = rotSpeedFactor * vehParams->m_water_ebrake_power;
                        speedFactor = speedFactor * forward_accel_scale;
                        this->m_stopped_time = -0.6f;
                    }
                    else if (this->m_stopped_time < 0.0)
                    {
                        speedFactor = 0.0f;
                        rotSpeedFactor = 0.0f;
                    }
                    this->m_stopped_time = this->m_stopped_time + delta_t;
                }
            }
            else
            {
                turn_throttle = turn_throttle * analog_throttle_turn_factor;
            }
            if ((float)(turn_throttle - 1.0) < 0.0)
                turn_throttle_clamp = turn_throttle;
            else
                turn_throttle_clamp = 1.0f;
            if ((float)(-1.0 - turn_throttle) < 0.0)
                clamped_turn_throttle = turn_throttle_clamp;
            else
                clamped_turn_throttle = -1.0f;
            turn_throttle = clamped_turn_throttle;
        }
        turn_force = body->m_mat.y;
        turn_force_scale = (float)((float)((float)((float)(turn_force.x * accel.x) + (float)(turn_force.y * accel.y)) + (float)(turn_force.z * accel.z))
            * turn_throttle)
            * rotSpeedFactor;
        turn_force.x = turn_force.x * turn_force_scale;
        turn_force.y = turn_force.y * turn_force_scale;
        turn_force.z = turn_force.z * turn_force_scale;
        //rigid_body::add_force(body, &turn_force, &motor_position, 1.0);
        body->add_force(&turn_force, &motor_position, 1.0);
        forward_force_scale = (float)((float)((float)((float)(forwardForce.x * accel.x) + (float)(forwardForce.y * accel.y))
            + (float)(forwardForce.z * accel.z))
            * throttle)
            * speedFactor;
        forwardForce.x = forwardForce.x * forward_force_scale;
        forwardForce.y = forwardForce.y * forward_force_scale;
        forwardForce.z = forwardForce.z * forward_force_scale;
        //rigid_body::add_force(body, &forwardForce, &motor_position, 1.0);
        body->add_force(&forwardForce, &motor_position, 1.0);
    }
}

//void rigid_body::add_force(
//                const phys_vec3 *force,
//                const phys_vec3 *point,
//                float torque_mult)
//{
//    double v6; // st7
//    float v7; // [esp-30h] [ebp-3Ch]
//    float v8; // [esp-2Ch] [ebp-38h]
//    float v9; // [esp-28h] [ebp-34h]
//    float v10; // [esp-20h] [ebp-2Ch]
//    float v11; // [esp-20h] [ebp-2Ch]
//    float v12; // [esp-1Ch] [ebp-28h]
//    float v13; // [esp-1Ch] [ebp-28h]
//    float v14; // [esp-18h] [ebp-24h]
//    float v15; // [esp-18h] [ebp-24h]
//    float v16; // [esp-4h] [ebp-10h]
//    float v17; // [esp-4h] [ebp-10h]
//    float v18; // [esp-4h] [ebp-10h]
//    float v19; // [esp-4h] [ebp-10h]
//    float v20; // [esp-4h] [ebp-10h]
//    float v21; // [esp-4h] [ebp-10h]
//
//    this->m_force_sum.x = this->m_force_sum.x + force->x;
//    this->m_force_sum.y = this->m_force_sum.y + force->y;
//    this->m_force_sum.z = this->m_force_sum.z + force->z;
//    v10 = point->x - this->m_mat.w.x;
//    v12 = point->y - this->m_mat.w.y;
//    v14 = point->z - this->m_mat.w.z;
//    v7 = force->z * v12 - force->y * v14;
//    v8 = v14 * force->x - force->z * v10;
//    v9 = v10 * force->y - v12 * force->x;
//    v11 = v7 * torque_mult;
//    v13 = v8 * torque_mult;
//    v15 = torque_mult * v9;
//    this->m_torque_sum.x = this->m_torque_sum.x + v11;
//    this->m_torque_sum.y = v13 + this->m_torque_sum.y;
//    this->m_torque_sum.z = this->m_torque_sum.z + v15;
//    v6 = 100000.0;
//    v16 = fabs(this->m_force_sum.x);
//    if ( v16 > 100000.0
//        || (v17 = fabs(this->m_force_sum.y), v17 > 100000.0)
//        || (v18 = fabs(this->m_force_sum.z), v18 > 100000.0) )
//    {
//        phys_exec_debug_callback(this);
//        v6 = 100000.0;
//    }
//    v19 = fabs(this->m_torque_sum.x);
//    if ( v19 > v6 || (v20 = fabs(this->m_torque_sum.y), v20 > v6) || (v21 = fabs(this->m_torque_sum.z), v6 < v21) )
//        phys_exec_debug_callback(this);
//}
//
//
