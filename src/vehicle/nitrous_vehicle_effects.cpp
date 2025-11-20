#include "nitrous_vehicle_effects.h"

void __userpurge NitrousVehicle::_update_wheel_effect_states(NitrousVehicle *this@<ecx>, int a2@<ebp>, float delta_t)
{
  double max_speed; // st7
  double v4; // st7
  float m_side_fric_k; // [esp-10h] [ebp-1A8h]
  float v6; // [esp-8h] [ebp-1A0h]
  float skid_delta; // [esp+0h] [ebp-198h]
  float speed; // [esp+4h] [ebp-194h]
  float v9; // [esp+18h] [ebp-180h]
  float v10; // [esp+40h] [ebp-158h]
  float v11; // [esp+44h] [ebp-154h]
  phys_vec3 *v12; // [esp+68h] [ebp-130h]
  phys_vec3 v13; // [esp+6Ch] [ebp-12Ch] BYREF
  float v14; // [esp+84h] [ebp-114h]
  float v15; // [esp+88h] [ebp-110h]
  phys_vec3 v16; // [esp+8Ch] [ebp-10Ch] BYREF
  phys_vec3 v17; // [esp+9Ch] [ebp-FCh] BYREF
  float w; // [esp+ACh] [ebp-ECh] BYREF
  float z; // [esp+B0h] [ebp-E8h]
  float y; // [esp+B4h] [ebp-E4h]
  phys_vec3 p_vel; // [esp+B8h] [ebp-E0h]
  phys_vec3 *v22; // [esp+C8h] [ebp-D0h]
  phys_vec3 v23; // [esp+CCh] [ebp-CCh] BYREF
  float v24[3]; // [esp+DCh] [ebp-BCh] BYREF
  phys_vec3 p; // [esp+E8h] [ebp-B0h]
  float v26; // [esp+F8h] [ebp-A0h]
  float v27; // [esp+FCh] [ebp-9Ch]
  float v28; // [esp+100h] [ebp-98h]
  float v29; // [esp+104h] [ebp-94h]
  int v30; // [esp+108h] [ebp-90h]
  float v31; // [esp+10Ch] [ebp-8Ch]
  float v32; // [esp+110h] [ebp-88h]
  float v33; // [esp+114h] [ebp-84h]
  float v34; // [esp+124h] [ebp-74h]
  float v35; // [esp+128h] [ebp-70h]
  float v36; // [esp+12Ch] [ebp-6Ch]
  float v37; // [esp+130h] [ebp-68h]
  float v38; // [esp+134h] [ebp-64h]
  float v39; // [esp+138h] [ebp-60h]
  float v40; // [esp+13Ch] [ebp-5Ch]
  float v41; // [esp+140h] [ebp-58h]
  float v42; // [esp+144h] [ebp-54h]
  int v43; // [esp+148h] [ebp-50h]
  _BYTE v44[12]; // [esp+14Ch] [ebp-4Ch] BYREF
  phys_vec3 a_vel_loc; // [esp+158h] [ebp-40h] BYREF
  phys_vec3 t_vel_loc; // [esp+168h] [ebp-30h]
  int v47; // [esp+178h] [ebp-20h]
  int v48; // [esp+17Ch] [ebp-1Ch]
  int j; // [esp+180h] [ebp-18h]
  WheelEffectState oldState; // [esp+184h] [ebp-14h]
  rigid_body_constraint_wheel *rbc_wheel; // [esp+188h] [ebp-10h]
  int i; // [esp+18Ch] [ebp-Ch]
  rigid_body *rb; // [esp+190h] [ebp-8h]
  rigid_body *retaddr; // [esp+198h] [ebp+0h]

  i = a2;
  rb = retaddr;
  rbc_wheel = (rigid_body_constraint_wheel *)this;
  if ( (this->m_flags & 1) == 0 )
  {
    oldState = *(unsigned int *)LODWORD(rbc_wheel[2].m_acceleration_factor_k);
    for ( j = 0; j < 4; ++j )
    {
      v48 = *((unsigned int *)&rbc_wheel[4].m_wheel_vel + j);
      if ( v48 )
      {
        v47 = *((unsigned int *)&rbc_wheel->b2 + 4 * j);
        *((unsigned int *)&rbc_wheel->rigid_body_constraint + 4 * j + 3) = *(unsigned int *)&FLOAT_0_0;
        if ( (*(unsigned int *)(v48 + 176) & 1) != 0 )
        {
          if ( (LODWORD(rbc_wheel[5].m_b2_hitn_loc.y) & 2) != 0
            && (t_vel_loc.w = rbc_wheel[2].m_side_fric_k,
                LODWORD(t_vel_loc.z) = LODWORD(t_vel_loc.w) & _mask__AbsFloat_,
                COERCE_FLOAT(LODWORD(t_vel_loc.w) & _mask__AbsFloat_) > s_ableToSkidSpeed) )
          {
            *((unsigned int *)&rbc_wheel->b1 + 4 * j) = 2;
            *((float *)&rbc_wheel->rigid_body_constraint + 4 * j + 3) = FLOAT_1_0;
          }
          else if ( (*(unsigned int *)(v48 + 176) & 4) != 0 )
          {
            phys_inv_multiply(
              (phys_vec3 *)&a_vel_loc.y,
              (const phys_mat44 *)(oldState + 48),
              (const phys_vec3 *)(oldState + 144));
            phys_inv_multiply(
              (const phys_vec3 *)v44,
              (const phys_mat44 *)(oldState + 48),
              (const phys_vec3 *)(oldState + 160));
            v40 = *(float *)(v48 + 64);
            v41 = *(float *)(v48 + 68);
            v42 = *(float *)(v48 + 72);
            v43 = *(unsigned int *)(v48 + 76);
            v39 = *(float *)(v48 + 96);
            v38 = *(float *)(v48 + 164);
            v37 = v39 - v38;
            v36 = (float)(v39 - v38) * v40;
            v35 = (float)(v39 - v38) * v41;
            v34 = (float)(v39 - v38) * v42;
            v31 = v36;
            v32 = v35;
            v33 = v34;
            v27 = *(float *)(v48 + 48);
            v28 = *(float *)(v48 + 52);
            v29 = *(float *)(v48 + 56);
            v30 = *(unsigned int *)(v48 + 60);
            v26 = v27 + v36;
            p.w = v28 + v35;
            p.z = v29 + v34;
            v24[0] = v27 + v36;
            v24[1] = v28 + v35;
            v24[2] = v29 + v34;
            v22 = phys_cross(&v23, (const phys_vec3 *)v44, (const phys_vec3 *)v24);
            p_vel.w = a_vel_loc.y + v22->x;
            p_vel.z = a_vel_loc.z + v22->y;
            p_vel.y = a_vel_loc.w + v22->z;
            w = p_vel.w;
            z = p_vel.z;
            y = p_vel.y;
            v17 = *(phys_vec3 *)(v48 + 64);
            v16 = *(phys_vec3 *)(v48 + 80);
            v15 = *(float *)(v48 + 96);
            v14 = *(float *)(v48 + 152);
            v12 = phys_cross(&v13, &v16, &v17);
            v10 = (float)(v15 * v14) * v12->y;
            v11 = (float)(v15 * v14) * v12->z;
            w = w + (float)((float)(v15 * v14) * v12->x);
            z = z + v10;
            y = y + v11;
            v9 = (float)((float)(w * *(float *)(v48 + 64)) + (float)(z * *(float *)(v48 + 68)))
               + (float)(y * *(float *)(v48 + 72));
            skid_delta = v9 * *(float *)(v48 + 68);
            speed = v9 * *(float *)(v48 + 72);
            w = w - (float)(v9 * *(float *)(v48 + 64));
            z = z - skid_delta;
            y = y - speed;
            v6 = Abs(&w);
            if ( v6 <= *(float *)(LODWORD(rbc_wheel[2].m_desired_speed_k) + 2712) )
            {
              *((unsigned int *)&rbc_wheel->b1 + 4 * j) = 0;
              max_speed = NitrousVehicle::get_max_speed((NitrousVehicle *)rbc_wheel, 0);
              *((float *)&rbc_wheel->rigid_body_constraint + 4 * j + 3) = v6 / max_speed;
            }
            else
            {
              *((unsigned int *)&rbc_wheel->b1 + 4 * j) = 2;
              *((float *)&rbc_wheel->rigid_body_constraint + 4 * j + 3) = (float)(v6
                                                                                - *(float *)(LODWORD(rbc_wheel[2].m_desired_speed_k)
                                                                                           + 2712))
                                                                        / (float)(*(float *)(LODWORD(rbc_wheel[2].m_desired_speed_k)
                                                                                           + 2716)
                                                                                - *(float *)(LODWORD(rbc_wheel[2].m_desired_speed_k)
                                                                                           + 2712));
            }
          }
          else
          {
            *((unsigned int *)&rbc_wheel->b1 + 4 * j) = 0;
            m_side_fric_k = rbc_wheel[2].m_side_fric_k;
            v4 = NitrousVehicle::get_max_speed((NitrousVehicle *)rbc_wheel, 0);
            *((float *)&rbc_wheel->rigid_body_constraint + 4 * j + 3) = m_side_fric_k / v4;
          }
        }
        else
        {
          *((unsigned int *)&rbc_wheel->b1 + 4 * j) = 1;
          *((unsigned int *)&rbc_wheel->rigid_body_constraint + 4 * j + 3) = *(unsigned int *)&FLOAT_0_0;
        }
        if ( v47 == *((unsigned int *)&rbc_wheel->b1 + 4 * j) )
        {
          *((float *)&rbc_wheel->m_next + 4 * j) = *((float *)&rbc_wheel->m_next + 4 * j) + delta_t;
        }
        else
        {
          *((unsigned int *)&rbc_wheel->b2 + 4 * j) = v47;
          *((unsigned int *)&rbc_wheel->m_next + 4 * j) = *(unsigned int *)&FLOAT_0_0;
        }
      }
    }
  }
}

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

double __userpurge NitrousVehicle::debug_render_wheel_effects@<st0>(
        NitrousVehicle *this@<ecx>,
        WheelEffectState a2@<ebp>,
        int wheel)
{
  phys_vec3 *v4; // eax
  const phys_vec3 *v5; // eax
  float v6; // xmm0_4
  const phys_vec3 *v7; // [esp+10h] [ebp-290h]
  int v8; // [esp+24h] [ebp-27Ch] BYREF
  phys_vec3 end; // [esp+30h] [ebp-270h]
  float skidSpeedMax; // [esp+40h] [ebp-260h]
  _BYTE v11[12]; // [esp+44h] [ebp-25Ch] BYREF
  float v12; // [esp+60h] [ebp-240h]
  float v13[2]; // [esp+64h] [ebp-23Ch] BYREF
  float speed; // [esp+6Ch] [ebp-234h]
  float v15; // [esp+74h] [ebp-22Ch]
  float v16; // [esp+78h] [ebp-228h]
  float v17; // [esp+7Ch] [ebp-224h]
  float v18; // [esp+80h] [ebp-220h]
  phys_vec3 v19; // [esp+84h] [ebp-21Ch]
  phys_vec3 v20; // [esp+94h] [ebp-20Ch]
  float v21; // [esp+A4h] [ebp-1FCh]
  float v22; // [esp+A8h] [ebp-1F8h]
  float v23; // [esp+ACh] [ebp-1F4h]
  float v24; // [esp+C0h] [ebp-1E0h]
  float v25; // [esp+C4h] [ebp-1DCh]
  float v26; // [esp+C8h] [ebp-1D8h]
  float v27; // [esp+CCh] [ebp-1D4h]
  phys_vec3 *v28; // [esp+D0h] [ebp-1D0h]
  phys_vec3 v29; // [esp+D4h] [ebp-1CCh] BYREF
  float v30; // [esp+ECh] [ebp-1B4h]
  float v31; // [esp+F0h] [ebp-1B0h]
  phys_vec3 v32; // [esp+F4h] [ebp-1ACh] BYREF
  phys_vec3 v33; // [esp+104h] [ebp-19Ch] BYREF
  float w; // [esp+114h] [ebp-18Ch] BYREF
  float v35; // [esp+118h] [ebp-188h]
  float y; // [esp+11Ch] [ebp-184h]
  phys_vec3 p_vel; // [esp+120h] [ebp-180h]
  phys_vec3 *v38; // [esp+130h] [ebp-170h]
  phys_vec3 v39; // [esp+134h] [ebp-16Ch] BYREF
  float v40[3]; // [esp+144h] [ebp-15Ch] BYREF
  phys_vec3 p; // [esp+150h] [ebp-150h]
  float v42; // [esp+160h] [ebp-140h]
  float v43; // [esp+164h] [ebp-13Ch]
  float v44; // [esp+168h] [ebp-138h]
  float v45; // [esp+16Ch] [ebp-134h]
  int v46; // [esp+170h] [ebp-130h]
  float v47; // [esp+174h] [ebp-12Ch]
  float v48; // [esp+178h] [ebp-128h]
  float v49; // [esp+17Ch] [ebp-124h]
  float v50; // [esp+18Ch] [ebp-114h]
  float v51; // [esp+190h] [ebp-110h]
  float v52; // [esp+194h] [ebp-10Ch]
  float v53; // [esp+198h] [ebp-108h]
  float v54; // [esp+19Ch] [ebp-104h]
  float v55; // [esp+1A0h] [ebp-100h]
  float v56; // [esp+1A4h] [ebp-FCh]
  float v57; // [esp+1A8h] [ebp-F8h]
  float v58; // [esp+1ACh] [ebp-F4h]
  int v59; // [esp+1B0h] [ebp-F0h]
  _BYTE v60[12]; // [esp+1B4h] [ebp-ECh] BYREF
  phys_vec3 a_vel_loc; // [esp+1C0h] [ebp-E0h] BYREF
  phys_vec3 t_vel_loc; // [esp+1D0h] [ebp-D0h] BYREF
  float _pos[3]; // [esp+1E8h] [ebp-B8h] BYREF
  float _dir[3]; // [esp+1F4h] [ebp-ACh] BYREF
  phys_vec3 dir; // [esp+200h] [ebp-A0h] BYREF
  int v66; // [esp+210h] [ebp-90h]
  phys_vec3 v67; // [esp+214h] [ebp-8Ch] BYREF
  phys_vec3 v68; // [esp+224h] [ebp-7Ch] BYREF
  int v69; // [esp+240h] [ebp-60h]
  __int64 v70; // [esp+244h] [ebp-5Ch] BYREF
  float z; // [esp+24Ch] [ebp-54h]
  phys_vec3 wheel_pos; // [esp+250h] [ebp-50h] BYREF
  float v73[3]; // [esp+26Ch] [ebp-34h] BYREF
  float color[4]; // [esp+278h] [ebp-28h]
  rigid_body *b1; // [esp+288h] [ebp-18h]
  float *radius; // [esp+28Ch] [ebp-14h]
  NitrousVehicle *rate; // [esp+290h] [ebp-10h]
  WheelEffectState state; // [esp+294h] [ebp-Ch]
  rigid_body_constraint_wheel *rbc_wheel; // [esp+298h] [ebp-8h]
  rigid_body_constraint_wheel *retaddr; // [esp+2A0h] [ebp+0h]

  state = a2;
  rbc_wheel = retaddr;
  rate = this;
  radius = (float *)this->m_wheels[wheel];
  if ( !radius )
    return 0.0;
  b1 = (rigid_body *)rate->m_wheel_state[wheel].m_state;
  color[3] = rate->m_wheel_state[wheel].m_rate;
  color[2] = radius[24];
  color[1] = *radius;
  GetWheelStateColor((WheelEffectState)b1, v73, color[3]);
  v4 = operator*((phys_vec3 *)&wheel_pos.y, radius[41], (const phys_vec3 *)radius + 4);
  operator-((phys_vec3 *)&v70, (const phys_vec3 *)radius + 3, v4);
  v69 = LODWORD(color[1]) + 48;
  v7 = (const phys_vec3 *)(LODWORD(color[1]) + 96);
  v5 = phys_multiply(&v68, (const phys_mat44 *)(LODWORD(color[1]) + 48), (const phys_vec3 *)&v70);
  operator+(&v67, v5, v7);
  v70 = *(_QWORD *)&v67.x;
  z = v67.z;
  dir.y = radius[20];
  dir.z = radius[21];
  dir.w = radius[22];
  v66 = *((unsigned int *)radius + 23);
  phys_multiply((const phys_vec3 *)_dir, (const phys_mat44 *)(LODWORD(color[1]) + 48), (phys_vec3 *)&dir.y);
  Phys_NitrousVecToVec3((const phys_vec3 *)_dir, _pos);
  Phys_NitrousVecToVec3((const phys_vec3 *)&v70, &t_vel_loc.w);
  G_DebugCircleEx(&t_vel_loc.w, color[2], _pos, v73, 0, 0);
  LODWORD(t_vel_loc.z) = rate->m_phys_user_data->body;
  phys_inv_multiply(
    (phys_vec3 *)&a_vel_loc.y,
    (const phys_mat44 *)(LODWORD(t_vel_loc.z) + 48),
    (const phys_vec3 *)(LODWORD(t_vel_loc.z) + 144));
  phys_inv_multiply(
    (const phys_vec3 *)v60,
    (const phys_mat44 *)(LODWORD(t_vel_loc.z) + 48),
    (const phys_vec3 *)(LODWORD(t_vel_loc.z) + 160));
  v56 = radius[16];
  v57 = radius[17];
  v58 = radius[18];
  v59 = *((unsigned int *)radius + 19);
  v55 = radius[24];
  v54 = radius[41];
  v53 = v55 - v54;
  v52 = (float)(v55 - v54) * v56;
  v51 = (float)(v55 - v54) * v57;
  v50 = (float)(v55 - v54) * v58;
  v47 = v52;
  v48 = v51;
  v49 = v50;
  v43 = radius[12];
  v44 = radius[13];
  v45 = radius[14];
  v46 = *((unsigned int *)radius + 15);
  v42 = v43 + v52;
  p.w = v44 + v51;
  p.z = v45 + v50;
  v40[0] = v43 + v52;
  v40[1] = v44 + v51;
  v40[2] = v45 + v50;
  v38 = phys_cross(&v39, (const phys_vec3 *)v60, (const phys_vec3 *)v40);
  p_vel.w = a_vel_loc.y + v38->x;
  p_vel.z = a_vel_loc.z + v38->y;
  p_vel.y = a_vel_loc.w + v38->z;
  w = p_vel.w;
  v35 = p_vel.z;
  y = p_vel.y;
  v33 = (phys_vec3)*((_OWORD *)radius + 4);
  v32 = (phys_vec3)*((_OWORD *)radius + 5);
  v31 = radius[24];
  v30 = radius[38];
  v28 = phys_cross(&v29, &v32, &v33);
  v27 = v31 * v30;
  v26 = (float)(v31 * v30) * v28->x;
  v25 = (float)(v31 * v30) * v28->y;
  v24 = (float)(v31 * v30) * v28->z;
  v21 = v26;
  v22 = v25;
  v23 = v24;
  w = w + v26;
  v35 = v35 + v25;
  y = y + v24;
  v20 = (phys_vec3)*((_OWORD *)radius + 4);
  v19 = (phys_vec3)*((_OWORD *)radius + 4);
  v18 = (float)((float)(w * v19.x) + (float)(v35 * v19.y)) + (float)(y * v19.z);
  v17 = v18 * v20.x;
  v16 = v18 * v20.y;
  v15 = v18 * v20.z;
  v13[0] = v18 * v20.x;
  v13[1] = v18 * v20.y;
  speed = v18 * v20.z;
  phys_vec3::operator-=((phys_vec3 *)&w, (const phys_vec3 *)v13);
  v12 = Abs(&w);
  operator/((phys_vec3 *)v11, (const phys_vec3 *)&w, v12);
  skidSpeedMax = rate->m_vehicle_info->skidSpeedMax;
  if ( (float)(v12 - skidSpeedMax) < 0.0 )
    v6 = v12;
  else
    v6 = skidSpeedMax;
  end.w = v6;
  if ( (float)(0.0 - v12) < 0.0 )
    end.z = end.w;
  else
    end.z = *(float *)&FLOAT_0_0;
  operator*((phys_vec3 *)&v8, (const phys_vec3 *)v11, end.z);
  if ( NitrousVehicle::is_peeling_out(rate) )
    Phys_DebugPoint((const phys_vec3 *)&v70, color[2], colorMagenta);
  else
    Phys_DebugPoint((const phys_vec3 *)&v70, color[2], v73);
  return v12;
}

phys_vec3 *__thiscall phys_vec3::operator-=(phys_vec3 *this, const phys_vec3 *v)
{
  this->x = this->x - v->x;
  this->y = this->y - v->y;
  this->z = this->z - v->z;
  return this;
}

phys_vec3 *__cdecl operator/(phys_vec3 *result, const phys_vec3 *a, float d)
{
  float v4; // [esp+4h] [ebp-Ch]
  float v5; // [esp+8h] [ebp-8h]

  v4 = a->y * (float)(1.0 / d);
  v5 = a->z * (float)(1.0 / d);
  result->x = a->x * (float)(1.0 / d);
  result->y = v4;
  result->z = v5;
  return result;
}

void  NitrousVehicle::debug_render_effects(NitrousVehicle *this@<ecx>, int a2@<ebp>)
{
  double v2; // st7
  char v3[12]; // [esp+28h] [ebp-244h] BYREF
  float buffer[126]; // [esp+34h] [ebp-238h] BYREF
  float v5; // [esp+22Ch] [ebp-40h]
  float textPos[3]; // [esp+230h] [ebp-3Ch] BYREF
  phys_vec3 pos; // [esp+23Ch] [ebp-30h]
  float v8; // [esp+24Ch] [ebp-20h]
  float wheelSpeeds[4]; // [esp+250h] [ebp-1Ch]
  int i; // [esp+260h] [ebp-Ch] BYREF
  NitrousVehicle *thisa; // [esp+264h] [ebp-8h]
  NitrousVehicle *retaddr; // [esp+26Ch] [ebp+0h]

  i = a2;
  thisa = retaddr;
  LODWORD(wheelSpeeds[2]) = this;
  if ( phys_drawNitrousVehicleEffects->current.enabled && *(unsigned int *)(LODWORD(wheelSpeeds[2]) + 560) )
  {
    for ( wheelSpeeds[1] = 0.0; SLODWORD(wheelSpeeds[1]) < 4; *(&pos.z + LODWORD(wheelSpeeds[1])++) = v2 )
      v2 = NitrousVehicle::debug_render_wheel_effects(
             (NitrousVehicle *)LODWORD(wheelSpeeds[2]),
             (WheelEffectState)&i,
             SLODWORD(wheelSpeeds[1]));
    LODWORD(pos.y) = **(unsigned int **)(LODWORD(wheelSpeeds[2]) + 560) + 48;
    textPos[0] = *(float *)(LODWORD(pos.y) + 48);
    textPos[1] = *(float *)(LODWORD(pos.y) + 52);
    textPos[2] = *(float *)(LODWORD(pos.y) + 56);
    pos.x = *(float *)(LODWORD(pos.y) + 60);
    Phys_NitrousVecToVec3((const phys_vec3 *)textPos, &buffer[124]);
    v5 = v5 + 50.0;
    sprintf(v3, "Wheel Speeds %.0f %.0f %.0f %.0f", pos.z, pos.w, v8, wheelSpeeds[0]);
    CL_AddDebugString(&buffer[124], colorWhite, scale_2, v3, 0);
  }
}

