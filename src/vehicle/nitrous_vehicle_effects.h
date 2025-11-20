#pragma once

void __userpurge NitrousVehicle::_update_wheel_effect_states(NitrousVehicle *this@<ecx>, int a2@<ebp>, float delta_t);
void __cdecl GetWheelStateColor(WheelEffectState state, float *color, float rate);
double __userpurge NitrousVehicle::debug_render_wheel_effects@<st0>(
        NitrousVehicle *this@<ecx>,
        WheelEffectState a2@<ebp>,
        int wheel);
phys_vec3 *__thiscall phys_vec3::operator-=(phys_vec3 *this, const phys_vec3 *v);
phys_vec3 *__cdecl operator/(phys_vec3 *result, const phys_vec3 *a, float d);
void  NitrousVehicle::debug_render_effects(NitrousVehicle *this@<ecx>, int a2@<ebp>);
