#include "nitrous_vehicle_effects.h"
#include "nitrous_vehicle.h"
#include <client/cl_debugdata.h>
#include <bgame/bg_slidemove.h>
#include <physics/phys_render.h>
#include <game/g_debug.h>

float s_ableToSkidSpeed = 100.0;
float s_tireSkidSpeedMin = 100.0;
float s_tireSkidSpeedMax = 600.0;

// Decomp: CoDMPServer.c:661383  (RVA 0082E780)
void NitrousVehicle::_update_wheel_effect_states(float delta_t)
{
    double max_speed;
    double maxSpeedForRate;
    float m_forward_vel;
    float abs_slip_speed;
    float suspProjY;
    float suspProjZ;
    float velAlongSuspension;
    float wheelSpinVelY;
    float wheelSpinVelZ;
    float wheelRadius;
    phys_vec3 m_b1_wheel_axis_loc;
    phys_vec3 suspensionDir;
    phys_vec3 p_vel;
    phys_vec3 p;
    phys_vec3 m_b1_wheel_center_loc;
    phys_vec3 contactOffset;
    float radiusMinusDisplacement;
    float m_wheel_vel;
    float m_wheel_displaced_center_dist;
    float m_wheel_radius;
    phys_vec3 m_b1_suspension_dir_loc;
    phys_vec3 a_vel_loc;
    phys_vec3 t_vel_loc;
    float forward_vel_abs;
    float forward_vel;
    WheelEffectState oldState;
    rigid_body_constraint_wheel *rbc_wheel;
    int i;
    rigid_body *rb;
    NitrousVehicle *vehicle;
    vehicle = this;
    if ((this->m_flags & 1) == 0)
    {
        rb = vehicle->m_phys_user_data->body;
        for (i = 0; i < 4; ++i)
        {
            rbc_wheel = vehicle->m_wheels[i];
            if (rbc_wheel)
            {
                oldState = vehicle->m_wheel_state[i].m_last_state;
                vehicle->m_wheel_state[i].m_rate = 0.0f;
                if ((rbc_wheel->m_wheel_flags & 1) != 0)
                {
                    if ((vehicle->m_state_flags & 2) != 0
                        && (forward_vel = vehicle->m_forward_vel,
                            forward_vel_abs = fabs(forward_vel),
                            forward_vel_abs > s_ableToSkidSpeed))
                    {
                        vehicle->m_wheel_state[i].m_state = WHEEL_STATE_SKIDDING;
                        vehicle->m_wheel_state[i].m_rate = 1.0f;
                    }
                    else if ((rbc_wheel->m_wheel_flags & 4) != 0)
                    {
                        phys_inv_multiply(&t_vel_loc, &rb->m_mat, &rb->m_t_vel);
                        phys_inv_multiply(&a_vel_loc, &rb->m_mat, &rb->m_a_vel);
                        m_b1_suspension_dir_loc = rbc_wheel->m_b1_suspension_dir_loc;
                        m_wheel_radius = rbc_wheel->m_wheel_radius;
                        m_wheel_displaced_center_dist = rbc_wheel->m_wheel_displaced_center_dist;
                        radiusMinusDisplacement = m_wheel_radius - m_wheel_displaced_center_dist;
                        contactOffset.x = radiusMinusDisplacement * m_b1_suspension_dir_loc.x;
                        contactOffset.y = radiusMinusDisplacement * m_b1_suspension_dir_loc.y;
                        contactOffset.z = radiusMinusDisplacement * m_b1_suspension_dir_loc.z;
                        m_b1_wheel_center_loc = rbc_wheel->m_b1_wheel_center_loc;
                        p.x = m_b1_wheel_center_loc.x + contactOffset.x;
                        p.y = m_b1_wheel_center_loc.y + contactOffset.y;
                        p.z = m_b1_wheel_center_loc.z + contactOffset.z;
                        phys_vec3 tmp_cross;
                        phys_cross(&tmp_cross, &a_vel_loc, &p);
                        p_vel.x = t_vel_loc.x + tmp_cross.x;
                        p_vel.y = t_vel_loc.y + tmp_cross.y;
                        p_vel.z = t_vel_loc.z + tmp_cross.z;
                        suspensionDir = rbc_wheel->m_b1_suspension_dir_loc;
                        m_b1_wheel_axis_loc = rbc_wheel->m_b1_wheel_axis_loc;
                        wheelRadius = rbc_wheel->m_wheel_radius;
                        m_wheel_vel = rbc_wheel->m_wheel_vel;
                        phys_cross(&tmp_cross, &m_b1_wheel_axis_loc, &suspensionDir);
                        wheelSpinVelY = (float)(wheelRadius * m_wheel_vel) * tmp_cross.y;
                        wheelSpinVelZ = (float)(wheelRadius * m_wheel_vel) * tmp_cross.z;
                        p_vel.x = p_vel.x + (float)((float)(wheelRadius * m_wheel_vel) * tmp_cross.x);
                        p_vel.y = p_vel.y + wheelSpinVelY;
                        p_vel.z = p_vel.z + wheelSpinVelZ;
                        velAlongSuspension = (float)((float)(p_vel.x * rbc_wheel->m_b1_suspension_dir_loc.x)
                            + (float)(p_vel.y * rbc_wheel->m_b1_suspension_dir_loc.y))
                            + (float)(p_vel.z * rbc_wheel->m_b1_suspension_dir_loc.z);
                        suspProjY = velAlongSuspension * rbc_wheel->m_b1_suspension_dir_loc.y;
                        suspProjZ = velAlongSuspension * rbc_wheel->m_b1_suspension_dir_loc.z;
                        p_vel.x = p_vel.x - (float)(velAlongSuspension * rbc_wheel->m_b1_suspension_dir_loc.x);
                        p_vel.y = p_vel.y - suspProjY;
                        p_vel.z = p_vel.z - suspProjZ;
                        abs_slip_speed = Abs(&p_vel.x);
                        if (abs_slip_speed <= vehicle->m_vehicle_info->skidSpeedMin)
                        {
                            vehicle->m_wheel_state[i].m_state = WHEEL_STATE_ROLLING;
                            max_speed = NitrousVehicle::get_max_speed(0);
                            vehicle->m_wheel_state[i].m_rate = abs_slip_speed / max_speed;
                        }
                        else
                        {
                            vehicle->m_wheel_state[i].m_state = WHEEL_STATE_SKIDDING;
                            vehicle->m_wheel_state[i].m_rate = (float)(abs_slip_speed - vehicle->m_vehicle_info->skidSpeedMin)
                                / (float)(vehicle->m_vehicle_info->skidSpeedMax
                                    - vehicle->m_vehicle_info->skidSpeedMin);
                        }
                    }
                    else
                    {
                        vehicle->m_wheel_state[i].m_state = WHEEL_STATE_ROLLING;
                        m_forward_vel = vehicle->m_forward_vel;
                        maxSpeedForRate = NitrousVehicle::get_max_speed(0);
                        vehicle->m_wheel_state[i].m_rate = m_forward_vel / maxSpeedForRate;
                    }
                }
                else
                {
                    vehicle->m_wheel_state[i].m_state = WHEEL_STATE_AIRBORN;
                    vehicle->m_wheel_state[i].m_rate = 0.0f;
                }
                if (oldState == vehicle->m_wheel_state[i].m_state)
                {
                    vehicle->m_wheel_state[i].m_state_time = vehicle->m_wheel_state[i].m_state_time + delta_t;
                }
                else
                {
                    vehicle->m_wheel_state[i].m_last_state = oldState;
                    vehicle->m_wheel_state[i].m_state_time = 0.0f;
                }
            }
        }
    }
}

// Decomp: CoDMPServer.c:661581  (RVA 0082EF60)
void __cdecl GetWheelStateColor(WheelEffectState state, float *color, float rate)
{
    if ( state )
    {
        if ( state == WHEEL_STATE_AIRBORN )
        {
            *color = colorLtBlue[0];
            color[1] = 0.5;
            color[2] = 1.0;
            color[3] = 1.0;
        }
        else if ( state == WHEEL_STATE_SKIDDING )
        {
            Vec4Lerp(colorYellow, colorRed, rate, color);
        }
    }
    else
    {
        *color = colorGreen[0];
        color[1] = 1.0;
        color[2] = 0.0;
        color[3] = 1.0;
    }
}

// Decomp: CoDMPServer.c:661607  (RVA 0082F030)
double NitrousVehicle::debug_render_wheel_effects(int wheel)
{
    const phys_vec3 *wheelPosTransformed;
    float speed_clamped;
    phys_vec3 *p_w;
    phys_vec3 end;
    float arrowLength;
    float arrowLengthPos;
    float skidSpeedMax;
    phys_vec3 direction;
    float speed;
    phys_vec3 suspProjVec;
    phys_vec3 suspDir;
    float velAlongSuspension;
    float wheelSpinVelX;
    float wheelSpinVelY;
    float wheelSpinVelZ;
    phys_vec3 *axisCrossPtr;
    phys_vec3 axisCross;
    float wheelRadius;
    phys_vec3 wheelAxis;
    phys_vec3 suspensionDir;
    phys_vec3 p_vel;
    phys_vec3 *aVelCrossPtr;
    phys_vec3 aVelCross;
    phys_vec3 p;
    phys_vec3 m_b1_wheel_center_loc;
    phys_vec3 contactOffset;
    float radiusMinusDisplacement;
    float m_wheel_displaced_center_dist;
    float m_wheel_radius;
    phys_vec3 m_b1_suspension_dir_loc;
    phys_vec3 a_vel_loc;
    phys_vec3 t_vel_loc;
    rigid_body *rb;
    float _pos[3];
    float _dir[3];
    phys_vec3 dir;
    phys_vec3 m_b1_wheel_axis_loc;
    phys_vec3 wheelPosWorld;
    phys_vec3 wheelPosLocalScaled;
    phys_mat44 *p_m_mat;
    phys_vec3 wheel_pos;
    phys_vec3 suspensionOffset;
    float color[4];
    rigid_body *b1;
    float radius;
    float rate;
    WheelEffectState state;
    rigid_body_constraint_wheel *rbc_wheel;
    NitrousVehicle *vehicle;
    float m_wheel_vel;
    vehicle = this;
    rbc_wheel = this->m_wheels[wheel];
    if (!rbc_wheel)
        return 0.0;
    state = vehicle->m_wheel_state[wheel].m_state;
    rate = vehicle->m_wheel_state[wheel].m_rate;
    radius = rbc_wheel->m_wheel_radius;
    b1 = rbc_wheel->b1;
    GetWheelStateColor(state, color, rate);
    suspensionOffset = rbc_wheel->m_b1_suspension_dir_loc * rbc_wheel->m_wheel_displaced_center_dist;
    wheel_pos = rbc_wheel->m_b1_wheel_center_loc - suspensionOffset;
    p_m_mat = &b1->m_mat;
    p_w = &b1->m_mat.w;
    wheelPosTransformed = phys_multiply(&wheelPosLocalScaled, &b1->m_mat, &wheel_pos);
    wheelPosWorld = *wheelPosTransformed + *p_w;
    wheel_pos.x = wheelPosWorld.x;
    wheel_pos.y = wheelPosWorld.y;
    wheel_pos.z = wheelPosWorld.z;
    m_b1_wheel_axis_loc = rbc_wheel->m_b1_wheel_axis_loc;
    phys_multiply(&dir, &b1->m_mat, &m_b1_wheel_axis_loc);
    Phys_NitrousVecToVec3(&dir, _dir);
    Phys_NitrousVecToVec3(&wheel_pos, _pos);
    G_DebugCircleEx(_pos, radius, _dir, color, 0, 0);
    rb = vehicle->m_phys_user_data->body;
    phys_inv_multiply(&t_vel_loc, &rb->m_mat, &rb->m_t_vel);
    phys_inv_multiply(&a_vel_loc, &rb->m_mat, &rb->m_a_vel);
    m_b1_suspension_dir_loc = rbc_wheel->m_b1_suspension_dir_loc;
    m_wheel_radius = rbc_wheel->m_wheel_radius;
    m_wheel_displaced_center_dist = rbc_wheel->m_wheel_displaced_center_dist;
    radiusMinusDisplacement = m_wheel_radius - m_wheel_displaced_center_dist;
    contactOffset.x = radiusMinusDisplacement * m_b1_suspension_dir_loc.x;
    contactOffset.y = radiusMinusDisplacement * m_b1_suspension_dir_loc.y;
    contactOffset.z = radiusMinusDisplacement * m_b1_suspension_dir_loc.z;
    m_b1_wheel_center_loc = rbc_wheel->m_b1_wheel_center_loc;
    p.x = m_b1_wheel_center_loc.x + contactOffset.x;
    p.y = m_b1_wheel_center_loc.y + contactOffset.y;
    p.z = m_b1_wheel_center_loc.z + contactOffset.z;
    aVelCrossPtr = phys_cross(&aVelCross, &a_vel_loc, &p);
    p_vel.x = t_vel_loc.x + aVelCrossPtr->x;
    p_vel.y = t_vel_loc.y + aVelCrossPtr->y;
    p_vel.z = t_vel_loc.z + aVelCrossPtr->z;
    suspensionDir = rbc_wheel->m_b1_suspension_dir_loc;
    wheelAxis = rbc_wheel->m_b1_wheel_axis_loc;
    wheelRadius = rbc_wheel->m_wheel_radius;
    m_wheel_vel = rbc_wheel->m_wheel_vel;
    axisCrossPtr = phys_cross(&axisCross, &wheelAxis, &suspensionDir);
    wheelSpinVelX = (float)(wheelRadius * m_wheel_vel) * axisCrossPtr->x;
    wheelSpinVelY = (float)(wheelRadius * m_wheel_vel) * axisCrossPtr->y;
    wheelSpinVelZ = (float)(wheelRadius * m_wheel_vel) * axisCrossPtr->z;
    p_vel.x = p_vel.x + wheelSpinVelX;
    p_vel.y = p_vel.y + wheelSpinVelY;
    p_vel.z = p_vel.z + wheelSpinVelZ;
    suspDir = rbc_wheel->m_b1_suspension_dir_loc;
    velAlongSuspension = (float)((float)(p_vel.x * suspDir.x) + (float)(p_vel.y * suspDir.y)) + (float)(p_vel.z * suspDir.z);
    suspProjVec.x = velAlongSuspension * suspDir.x;
    suspProjVec.y = velAlongSuspension * suspDir.y;
    suspProjVec.z = velAlongSuspension * suspDir.z;
    p_vel -= suspProjVec;
    speed = Abs(&p_vel.x);
    direction = p_vel / speed;
    skidSpeedMax = vehicle->m_vehicle_info->skidSpeedMax;
    if ((float)(speed - skidSpeedMax) < 0.0)
        speed_clamped = speed;
    else
        speed_clamped = skidSpeedMax;
    arrowLengthPos = speed_clamped;
    if ((float)(0.0 - speed) < 0.0)
        arrowLength = arrowLengthPos;
    else
        arrowLength = 0.0f;
    end = direction * arrowLength;
    if (NitrousVehicle::is_peeling_out())
        Phys_DebugPoint(&wheel_pos, radius, colorMagenta);
    else
        Phys_DebugPoint(&wheel_pos, radius, color);
    return speed;
}

//phys_vec3 *__cdecl operator/(phys_vec3 *result, const phys_vec3 *a, float d)
//{
//    float v4; // [esp+4h] [ebp-Ch]
//    float v5; // [esp+8h] [ebp-8h]
//
//    v4 = a->y * (float)(1.0 / d);
//    v5 = a->z * (float)(1.0 / d);
//    result->x = a->x * (float)(1.0 / d);
//    result->y = v4;
//    result->z = v5;
//    return result;

//}
float scale_2 = 0.1f;

// Decomp: CoDMPServer.c:661874  (RVA 0082F970)
void    NitrousVehicle::debug_render_effects()
{
    double wheelSpeed;
    char buffer[508];
    float textPos[3];
    phys_vec3 pos;
    phys_mat44 *p_m_mat;
    float wheelSpeeds[4];
    int i;
    NitrousVehicle *vehicle;
    vehicle = this;
    if (phys_drawNitrousVehicleEffects->current.enabled && vehicle->m_phys_user_data)
    {
        for (i = 0; i < 4; ++i)
        {
            wheelSpeed = NitrousVehicle::debug_render_wheel_effects(i);
            wheelSpeeds[i] = wheelSpeed;
        }
        p_m_mat = &vehicle->m_phys_user_data->body->m_mat;
        pos = p_m_mat->w;
        Phys_NitrousVecToVec3(&pos, textPos);
        textPos[2] = textPos[2] + 50.0;
        sprintf(buffer, "Wheel Speeds %.0f %.0f %.0f %.0f", wheelSpeeds[0], wheelSpeeds[1], wheelSpeeds[2], wheelSpeeds[3]);
        CL_AddDebugString(textPos, colorWhite, scale_2, buffer, 0);
    }
}
