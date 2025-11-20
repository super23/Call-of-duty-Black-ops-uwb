#pragma once

// local variable allocation has failed, the output may be wrong!
void __userpurge NitrousVehicleController::UpdateScriptVehicleControl(
        NitrousVehicleController *this@<ecx>,
        int a2@<ebp>,
        NitrousVehicle *rbveh,
        float delta_t);
void __thiscall NitrousVehicle::set_hand_brake(NitrousVehicle *this, float braking);
void __thiscall NitrousVehicleController::UpdateControlsStrafeMode(
        NitrousVehicleController *this,
        NitrousVehicle *rbveh);
// local variable allocation has failed, the output may be wrong!
void __userpurge NitrousVehicleController::UpdateVehicleInputs(
        NitrousVehicleController *this@<ecx>,
        const PhysObjUserData *a2@<ebp>,
        NitrousVehicle *rbveh,
        float target_yaw,
        float target_accel,
        float forward_preference);
void __thiscall NitrousVehicle::set_brake(NitrousVehicle *this, float braking);
void __thiscall NitrousVehicleController::UpdateStun(
        NitrousVehicleController *this,
        NitrousVehicle *rbveh,
        float delta_t);
double __cdecl StunRandomBrake(const usercmd_s *cmd, float desired_brake, float last_brake);
void __userpurge NitrousVehicleController::UpdateControls(
        NitrousVehicleController *this@<ecx>,
        float a2@<ebp>,
        NitrousVehicle *rbveh,
        float delta_t);
double __cdecl RecalibrateInputCustom(float val, float threshold);
// local variable allocation has failed, the output may be wrong!
void __userpurge NitrousVehicleController::UpdateControlsDefault(
        NitrousVehicleController *this@<ecx>,
        int a2@<ebp>,
        NitrousVehicle *rbveh,
        float delta_t);
void __thiscall NitrousVehicleController::UpdateControlsTank(NitrousVehicleController *this, NitrousVehicle *rbveh);
void __thiscall NitrousVehicleController::SetScriptTarget(
        NitrousVehicleController *this,
        NitrousVehicle *rbveh,
        phys_vec3 *goal_position,
        float goal_radius,
        float goal_speed,
        bool stopAtGoal);
void __userpurge NitrousVehicleController::UpdateJump(
        NitrousVehicleController *this@<ecx>,
        rigid_body *a2@<ebp>,
        NitrousVehicle *rbveh,
        float delta_t);
void __thiscall rigid_body::add_force(rigid_body *this, const phys_vec3 *force);
// local variable allocation has failed, the output may be wrong!
void __userpurge NitrousVehicleController::UpdateApplyBoatAccel(
        NitrousVehicleController *this@<ecx>,
        const VehicleParameter *a2@<ebp>,
        NitrousVehicle *rbveh,
        float delta_t);
void __thiscall rigid_body::add_force(
        rigid_body *this,
        const phys_vec3 *force,
        const phys_vec3 *point,
        float torque_mult);
