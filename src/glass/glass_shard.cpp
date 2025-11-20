#include "glass_shard.h"

void __cdecl GlassShard::Defrag(GlassShard *ptr)
{
  GlassShard::Defrag(ptr);
}

void __userpurge GlassPhysics::CreateAxis(
        GlassPhysics *this@<ecx>,
        int a2@<ebp>,
        float *position,
        const float (*axis)[3],
        float *tVel,
        float *aVel,
        float mass,
        float *localBBoxMin,
        float *localBBoxMax)
{
  phys_vec3 v9; // [esp-6Ch] [ebp-2BCh] BYREF
  float v10; // [esp-5Ch] [ebp-2ACh]
  float v11; // [esp-58h] [ebp-2A8h]
  float v12; // [esp-54h] [ebp-2A4h]
  phys_vec3 *p_m_t_vel; // [esp-50h] [ebp-2A0h]
  float v14; // [esp-4Ch] [ebp-29Ch]
  float v15; // [esp-48h] [ebp-298h]
  float v16; // [esp-44h] [ebp-294h]
  float v17; // [esp-3Ch] [ebp-28Ch]
  float v18; // [esp-38h] [ebp-288h]
  float v19; // [esp-34h] [ebp-284h]
  phys_mat44 *p_m_inv_inertia; // [esp-30h] [ebp-280h]
  float v21; // [esp-2Ch] [ebp-27Ch]
  float v22; // [esp-28h] [ebp-278h]
  float v23; // [esp-24h] [ebp-274h]
  float v24; // [esp-18h] [ebp-268h]
  float v25; // [esp-14h] [ebp-264h]
  float v26; // [esp-10h] [ebp-260h]
  _BYTE v27[12]; // [esp-Ch] [ebp-25Ch] BYREF
  phys_mat44 rb2w; // [esp+0h] [ebp-250h] BYREF
  phys_mat44 rb2m; // [esp+40h] [ebp-210h] BYREF
  int v30; // [esp+88h] [ebp-1C8h]
  int v31; // [esp+8Ch] [ebp-1C4h]
  int v32; // [esp+90h] [ebp-1C0h]
  float v33; // [esp+94h] [ebp-1BCh]
  float w; // [esp+98h] [ebp-1B8h]
  float z; // [esp+9Ch] [ebp-1B4h]
  phys_vec3 center; // [esp+A0h] [ebp-1B0h]
  float v37; // [esp+B0h] [ebp-1A0h]
  float v38; // [esp+B4h] [ebp-19Ch]
  float v39; // [esp+B8h] [ebp-198h]
  float v40; // [esp+BCh] [ebp-194h]
  float v41; // [esp+C8h] [ebp-188h]
  float v42; // [esp+CCh] [ebp-184h]
  float v43; // [esp+D0h] [ebp-180h]
  _BYTE v44[12]; // [esp+D4h] [ebp-17Ch] BYREF
  phys_mat44 cg2rb; // [esp+E0h] [ebp-170h] BYREF
  float v46; // [esp+128h] [ebp-128h]
  float v47; // [esp+12Ch] [ebp-124h]
  float v48; // [esp+130h] [ebp-120h]
  phys_vec3 v49; // [esp+134h] [ebp-11Ch] BYREF
  float v50; // [esp+148h] [ebp-108h]
  float v51; // [esp+14Ch] [ebp-104h]
  float v52; // [esp+150h] [ebp-100h]
  phys_vec3 v53; // [esp+154h] [ebp-FCh] BYREF
  float v54; // [esp+168h] [ebp-E8h]
  float v55; // [esp+16Ch] [ebp-E4h]
  float v56; // [esp+170h] [ebp-E0h]
  _BYTE v57[12]; // [esp+174h] [ebp-DCh] BYREF
  phys_mat44 m2w; // [esp+180h] [ebp-D0h] BYREF
  float v59; // [esp+1C4h] [ebp-8Ch]
  float v60; // [esp+1C8h] [ebp-88h]
  float v61; // [esp+1CCh] [ebp-84h]
  float v62; // [esp+1D0h] [ebp-80h]
  float v63; // [esp+1D4h] [ebp-7Ch] BYREF
  float v64; // [esp+1D8h] [ebp-78h]
  float v65; // [esp+1DCh] [ebp-74h]
  float v66; // [esp+1F0h] [ebp-60h] BYREF
  float v67[2]; // [esp+1F4h] [ebp-5Ch] BYREF
  float volume; // [esp+1FCh] [ebp-54h]
  phys_vec3 dim; // [esp+200h] [ebp-50h]
  float v70; // [esp+210h] [ebp-40h]
  float v71; // [esp+214h] [ebp-3Ch] BYREF
  float v72; // [esp+218h] [ebp-38h]
  float v73; // [esp+21Ch] [ebp-34h]
  phys_vec3 lbbMax; // [esp+220h] [ebp-30h] BYREF
  GlassPhysics *v75; // [esp+240h] [ebp-10h]
  unsigned int v76[2]; // [esp+244h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+250h] [ebp+0h]

  v76[0] = a2;
  v76[1] = retaddr;
  v75 = this;
  Phys_Vec3ToNitrousVec(localBBoxMin, (phys_vec3 *)&lbbMax.y);
  Phys_Vec3ToNitrousVec(localBBoxMax, (phys_vec3 *)&v71);
  v70 = v71 - lbbMax.y;
  dim.w = v72 - lbbMax.z;
  dim.z = v73 - lbbMax.w;
  v67[0] = v71 - lbbMax.y;
  v67[1] = v72 - lbbMax.z;
  volume = v73 - lbbMax.w;
  nuge::calc_box_inertia((const phys_vec3 *)v67, (phys_vec3 *)&v63, &v66);
  v62 = mass / v66;
  v63 = v63 * (float)(mass / v66);
  v64 = v64 * (float)(mass / v66);
  v65 = v65 * (float)(mass / v66);
  v61 = (*axis)[0];
  v60 = (*axis)[1];
  v59 = (*axis)[2];
  m2w.w.y = v61;
  m2w.w.z = v60;
  m2w.w.w = v59;
  phys_vec3::operator=((phys_vec3 *)v57, (const phys_vec3 *)&m2w.w.y);
  v56 = (*axis)[3];
  v55 = (*axis)[4];
  v54 = (*axis)[5];
  v53.x = v56;
  v53.y = v55;
  v53.z = v54;
  phys_vec3::operator=((phys_vec3 *)&m2w.x.y, &v53);
  v52 = (*axis)[6];
  v51 = (*axis)[7];
  v50 = (*axis)[8];
  v49.x = v52;
  v49.y = v51;
  v49.z = v50;
  phys_vec3::operator=((phys_vec3 *)&m2w.y.y, &v49);
  v48 = *position;
  v47 = position[1];
  v46 = position[2];
  cg2rb.w.y = v48;
  cg2rb.w.z = v47;
  cg2rb.w.w = v46;
  phys_vec3::operator=((phys_vec3 *)&m2w.z.y, (const phys_vec3 *)&cg2rb.w.y);
  phys_mat44::operator=((phys_mat44 *)v44, &PHYS_IDENTITY_MATRIX_67);
  v43 = v71 + lbbMax.y;
  v42 = v72 + lbbMax.z;
  v41 = v73 + lbbMax.w;
  v38 = v71 + lbbMax.y;
  v39 = v72 + lbbMax.z;
  v40 = v73 + lbbMax.w;
  v37 = 0.5 * (float)(v71 + lbbMax.y);
  center.w = 0.5 * (float)(v72 + lbbMax.z);
  center.z = 0.5 * (float)(v73 + lbbMax.w);
  v33 = v37;
  w = center.w;
  z = center.z;
  v32 = LODWORD(v37) ^ _mask__NegFloat_;
  v31 = LODWORD(center.w) ^ _mask__NegFloat_;
  v30 = LODWORD(center.z) ^ _mask__NegFloat_;
  LODWORD(rb2m.w.y) = LODWORD(v37) ^ _mask__NegFloat_;
  LODWORD(rb2m.w.z) = LODWORD(center.w) ^ _mask__NegFloat_;
  LODWORD(rb2m.w.w) = LODWORD(center.z) ^ _mask__NegFloat_;
  phys_vec3::operator=((phys_vec3 *)&cg2rb.z.y, (const phys_vec3 *)&rb2m.w.y);
  phys_full_inverse((int)v76, (phys_mat44 *)&rb2w.w.y, (const phys_mat44 *)v44);
  phys_full_multiply_mat((int)v76, (phys_mat44 *)v27, (const phys_mat44 *)v57, (phys_mat44 *)&rb2w.w.y);
  v75->m_inv_mass = 1.0 / mass;
  v26 = 1.0 / v63;
  v25 = 1.0 / v64;
  v24 = 1.0 / v65;
  v21 = 1.0 / v63;
  v22 = 1.0 / v64;
  v23 = 1.0 / v65;
  p_m_inv_inertia = (phys_mat44 *)&v75->m_inv_inertia;
  v75->m_inv_inertia.x = 1.0 / v63;
  p_m_inv_inertia->x.y = v22;
  p_m_inv_inertia->x.z = v23;
  phys_mat44::operator=(&v75->m_mat, (const phys_mat44 *)v27);
  v19 = *tVel;
  v18 = tVel[1];
  v17 = tVel[2];
  v14 = v19;
  v15 = v18;
  v16 = v17;
  p_m_t_vel = &v75->m_t_vel;
  v75->m_t_vel.x = v19;
  p_m_t_vel->y = v15;
  p_m_t_vel->z = v16;
  v12 = *aVel;
  v11 = aVel[1];
  v10 = aVel[2];
  v9.x = v12;
  v9.y = v11;
  v9.z = v10;
  phys_vec3::operator=(&v75->m_a_vel, &v9);
  phys_vec3::operator=(&v75->m_force_sum, &PHYS_ZERO_VEC_67);
  phys_vec3::operator=(&v75->m_torque_sum, &PHYS_ZERO_VEC_67);
}

void __userpurge GlassPhysics::AddForce(GlassPhysics *this@<ecx>, int a2@<ebp>, float *worldPos, float *impulse)
{
  phys_vec3 *v4; // eax
  phys_vec3 *p_m_torque_sum; // [esp-74h] [ebp-B4h]
  phys_vec3 v6; // [esp-6Ch] [ebp-ACh] BYREF
  phys_vec3 v7; // [esp-5Ch] [ebp-9Ch] BYREF
  float v8; // [esp-44h] [ebp-84h]
  float v9; // [esp-40h] [ebp-80h]
  float v10; // [esp-3Ch] [ebp-7Ch]
  phys_vec3 *p_w; // [esp-38h] [ebp-78h]
  phys_vec3 *v12; // [esp-34h] [ebp-74h]
  phys_vec3 *p_m_force_sum; // [esp-30h] [ebp-70h]
  float v14; // [esp-2Ch] [ebp-6Ch]
  float v15; // [esp-28h] [ebp-68h]
  float v16; // [esp-24h] [ebp-64h]
  float v17; // [esp-18h] [ebp-58h]
  float v18; // [esp-14h] [ebp-54h]
  float v19; // [esp-10h] [ebp-50h]
  float v20; // [esp-Ch] [ebp-4Ch]
  float v21; // [esp-8h] [ebp-48h]
  float v22; // [esp-4h] [ebp-44h]
  phys_vec3 pos; // [esp+0h] [ebp-40h]
  float v24; // [esp+10h] [ebp-30h]
  float w; // [esp+14h] [ebp-2Ch] BYREF
  float z; // [esp+18h] [ebp-28h]
  float y; // [esp+1Ch] [ebp-24h]
  phys_vec3 force; // [esp+20h] [ebp-20h]
  GlassPhysics *v29; // [esp+30h] [ebp-10h]
  int v30; // [esp+34h] [ebp-Ch]
  void *v31; // [esp+38h] [ebp-8h]
  void *retaddr; // [esp+40h] [ebp+0h]

  v30 = a2;
  v31 = retaddr;
  v29 = this;
  force.w = *impulse;
  force.z = impulse[1];
  force.y = impulse[2];
  w = force.w;
  z = force.z;
  y = force.y;
  v24 = *worldPos;
  pos.w = worldPos[1];
  pos.z = worldPos[2];
  v20 = v24;
  v21 = pos.w;
  v22 = pos.z;
  w = force.w * 0.001;
  z = force.z * 0.001;
  y = force.y * 0.001;
  if ( Abs(&w) >= 1000000.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp",
          102,
          0,
          "%s",
          "Abs(force) < 1000000.0f") )
  {
    __debugbreak();
  }
  if ( (float)((float)((float)(v20 * v20) + (float)(v21 * v21)) + (float)(v22 * v22)) == 0.0 )
  {
    v19 = 0.001 * w;
    v18 = 0.001 * z;
    v17 = 0.001 * y;
    v14 = 0.001 * w;
    v15 = 0.001 * z;
    v16 = 0.001 * y;
    p_m_force_sum = &v29->m_force_sum;
    v29->m_force_sum.x = v29->m_force_sum.x + (float)(0.001 * w);
    p_m_force_sum->y = p_m_force_sum->y + v15;
    p_m_force_sum->z = p_m_force_sum->z + v16;
  }
  else
  {
    v12 = &v29->m_force_sum;
    v29->m_force_sum.x = v29->m_force_sum.x + w;
    v12->y = v12->y + z;
    v12->z = v12->z + y;
    p_w = &v29->m_mat.w;
    v10 = v20 - v29->m_mat.w.x;
    v9 = v21 - v29->m_mat.w.y;
    v8 = v22 - v29->m_mat.w.z;
    v7.x = v10;
    v7.y = v9;
    v7.z = v8;
    v4 = phys_cross(&v6, &v7, (const phys_vec3 *)&w);
    p_m_torque_sum = &v29->m_torque_sum;
    v29->m_torque_sum.x = v29->m_torque_sum.x + v4->x;
    p_m_torque_sum->y = p_m_torque_sum->y + v4->y;
    p_m_torque_sum->z = p_m_torque_sum->z + v4->z;
  }
}

void __thiscall GlassPhysics::SetVelocity(GlassPhysics *this, float *t_vel)
{
  float v2; // [esp-1Ch] [ebp-28h]
  float v3; // [esp-18h] [ebp-24h]

  v2 = t_vel[1];
  v3 = t_vel[2];
  this->m_t_vel.x = *t_vel;
  this->m_t_vel.y = v2;
  this->m_t_vel.z = v3;
}

void __thiscall GlassPhysics::SetAngularVelocity(GlassPhysics *this, float *a_vel)
{
  float v2; // [esp-1Ch] [ebp-28h]
  float v3; // [esp-18h] [ebp-24h]

  v2 = a_vel[1];
  v3 = a_vel[2];
  this->m_a_vel.x = *a_vel;
  this->m_a_vel.y = v2;
  this->m_a_vel.z = v3;
}

void __thiscall GlassPhysics::Step(GlassPhysics *this, float deltaTime)
{
  int savedregs; // [esp+10h] [ebp+0h] BYREF

  GlassPhysics::IntegrateVelocity(this, (int)&savedregs, deltaTime);
  GlassPhysics::IntegratePos(this, (int)&savedregs, deltaTime);
  this->m_force_sum.x = PHYS_ZERO_VEC_67.x;
  this->m_force_sum.y = PHYS_ZERO_VEC_67.y;
  this->m_force_sum.z = PHYS_ZERO_VEC_67.z;
  this->m_torque_sum.x = PHYS_ZERO_VEC_67.x;
  this->m_torque_sum.y = PHYS_ZERO_VEC_67.y;
  this->m_torque_sum.z = PHYS_ZERO_VEC_67.z;
}

void __userpurge GlassPhysics::tensor_transform_principle(
        GlassPhysics *this@<ecx>,
        int a2@<ebp>,
        const phys_vec3 *diag,
        const phys_mat44 *mat,
        phys_mat44 *tensor)
{
  float v5; // [esp-Ch] [ebp-27Ch]
  float v6; // [esp-8h] [ebp-278h]
  float v7; // [esp-4h] [ebp-274h]
  float v8; // [esp+50h] [ebp-220h]
  float v9; // [esp+70h] [ebp-200h]
  float v10; // [esp+90h] [ebp-1E0h]
  float v11; // [esp+94h] [ebp-1DCh]
  float v12; // [esp+98h] [ebp-1D8h]
  float v13; // [esp+9Ch] [ebp-1D4h]
  float v14; // [esp+F0h] [ebp-180h]
  float v15; // [esp+110h] [ebp-160h]
  float v16; // [esp+130h] [ebp-140h]
  float v17[3]; // [esp+134h] [ebp-13Ch] BYREF
  phys_vec3 x_; // [esp+140h] [ebp-130h]
  float v19; // [esp+150h] [ebp-120h]
  float v20; // [esp+154h] [ebp-11Ch]
  float v21; // [esp+158h] [ebp-118h]
  float v22; // [esp+15Ch] [ebp-114h]
  float v23; // [esp+168h] [ebp-108h]
  float v24; // [esp+16Ch] [ebp-104h]
  float v25; // [esp+170h] [ebp-100h]
  float v26; // [esp+174h] [ebp-FCh]
  float v27; // [esp+178h] [ebp-F8h]
  float v28; // [esp+17Ch] [ebp-F4h]
  float v29; // [esp+184h] [ebp-ECh]
  float v30; // [esp+188h] [ebp-E8h]
  float v31; // [esp+18Ch] [ebp-E4h]
  float v32; // [esp+190h] [ebp-E0h]
  float v33; // [esp+194h] [ebp-DCh]
  float v34; // [esp+198h] [ebp-D8h]
  float v35; // [esp+19Ch] [ebp-D4h]
  float v36; // [esp+1A4h] [ebp-CCh]
  float v37; // [esp+1A8h] [ebp-C8h]
  float v38; // [esp+1ACh] [ebp-C4h]
  float v39; // [esp+1B0h] [ebp-C0h]
  float v40; // [esp+1B4h] [ebp-BCh]
  float v41; // [esp+1B8h] [ebp-B8h]
  float v42; // [esp+1BCh] [ebp-B4h]
  float v43; // [esp+1C4h] [ebp-ACh]
  float v44; // [esp+1C8h] [ebp-A8h]
  float v45; // [esp+1CCh] [ebp-A4h]
  float v46; // [esp+1D0h] [ebp-A0h]
  float v47; // [esp+1D4h] [ebp-9Ch]
  float v48; // [esp+1D8h] [ebp-98h]
  float v49; // [esp+1DCh] [ebp-94h]
  float v50; // [esp+1F0h] [ebp-80h]
  float v51; // [esp+1F4h] [ebp-7Ch]
  float v52; // [esp+1F8h] [ebp-78h]
  phys_vec3 *p_z; // [esp+1FCh] [ebp-74h]
  float z; // [esp+200h] [ebp-70h]
  float v55; // [esp+204h] [ebp-6Ch]
  float v56; // [esp+208h] [ebp-68h]
  float v57; // [esp+20Ch] [ebp-64h]
  float v58; // [esp+220h] [ebp-50h]
  float v59; // [esp+224h] [ebp-4Ch]
  float v60; // [esp+228h] [ebp-48h]
  phys_vec3 *p_y; // [esp+22Ch] [ebp-44h]
  float y; // [esp+230h] [ebp-40h]
  float v63; // [esp+234h] [ebp-3Ch]
  float v64; // [esp+238h] [ebp-38h]
  float v65; // [esp+23Ch] [ebp-34h]
  float v66; // [esp+250h] [ebp-20h]
  float v67; // [esp+254h] [ebp-1Ch]
  float v68; // [esp+258h] [ebp-18h]
  float x; // [esp+25Ch] [ebp-14h]
  GlassPhysics *v70; // [esp+260h] [ebp-10h]
  int v71; // [esp+264h] [ebp-Ch]
  void *v72; // [esp+268h] [ebp-8h]
  void *retaddr; // [esp+270h] [ebp+0h]

  v71 = a2;
  v72 = retaddr;
  v70 = this;
  x = diag->x;
  v68 = x * mat->x.x;
  v67 = x * mat->x.y;
  v66 = x * mat->x.z;
  v63 = v68;
  v64 = v67;
  v65 = v66;
  y = diag->y;
  p_y = &mat->y;
  v60 = y * mat->y.x;
  v59 = y * mat->y.y;
  v58 = y * mat->y.z;
  v55 = v60;
  v56 = v59;
  v57 = v58;
  z = diag->z;
  p_z = &mat->z;
  v52 = z * mat->z.x;
  v51 = z * mat->z.y;
  v50 = z * mat->z.z;
  v47 = v52;
  v48 = v51;
  v49 = v50;
  v46 = mat->z.x;
  v45 = v46 * v52;
  v44 = v46 * v51;
  v43 = v46 * v50;
  v40 = v46 * v52;
  v41 = v46 * v51;
  v42 = v46 * v50;
  v39 = mat->y.x;
  v38 = v39 * v60;
  v37 = v39 * v59;
  v36 = v39 * v58;
  v33 = v39 * v60;
  v34 = v39 * v59;
  v35 = v39 * v58;
  v32 = mat->x.x;
  v31 = v32 * v68;
  v30 = v32 * v67;
  v29 = v32 * v66;
  v26 = v32 * v68;
  v27 = v32 * v67;
  v28 = v32 * v66;
  v25 = (float)(v32 * v68) + (float)(v39 * v60);
  v24 = (float)(v32 * v67) + (float)(v39 * v59);
  v23 = (float)(v32 * v66) + (float)(v39 * v58);
  v20 = v25;
  v21 = v24;
  v22 = v23;
  v19 = v25 + (float)(v46 * v52);
  x_.w = v24 + (float)(v46 * v51);
  x_.z = v23 + (float)(v46 * v50);
  v17[0] = v19;
  v17[1] = x_.w;
  v17[2] = x_.z;
  v16 = mat->z.y;
  v15 = mat->y.y;
  v14 = mat->x.y;
  v11 = (float)((float)(v14 * v68) + (float)(v15 * v60)) + (float)(v16 * v52);
  v12 = (float)((float)(v14 * v67) + (float)(v15 * v59)) + (float)(v16 * v51);
  v13 = (float)((float)(v14 * v66) + (float)(v15 * v58)) + (float)(v16 * v50);
  v10 = mat->z.z;
  v9 = mat->y.z;
  v8 = mat->x.z;
  v5 = (float)((float)(v8 * v68) + (float)(v9 * v60)) + (float)(v10 * v52);
  v6 = (float)((float)(v8 * v67) + (float)(v9 * v59)) + (float)(v10 * v51);
  v7 = (float)((float)(v8 * v66) + (float)(v9 * v58)) + (float)(v10 * v50);
  phys_vec3::operator=(&tensor->x, (const phys_vec3 *)v17);
  tensor->y.x = v11;
  tensor->y.y = v12;
  tensor->y.z = v13;
  tensor->z.x = v5;
  tensor->z.y = v6;
  tensor->z.z = v7;
}

void __userpurge GlassPhysics::IntegrateVelocity(GlassPhysics *this@<ecx>, int a2@<ebp>, float deltaTime)
{
  const phys_vec3 *v3; // eax
  float v4; // xmm0_4
  float v5; // xmm0_4
  phys_vec3 *v6; // [esp-2Ch] [ebp-240h]
  phys_vec3 *v7; // [esp-1Ch] [ebp-230h]
  phys_vec3 *v8; // [esp-8h] [ebp-21Ch]
  float v9; // [esp-4h] [ebp-218h]
  float v10; // [esp+4h] [ebp-210h]
  int v11; // [esp+8h] [ebp-20Ch] BYREF
  phys_vec3 v12; // [esp+18h] [ebp-1FCh] BYREF
  float v13; // [esp+2Ch] [ebp-1E8h]
  float v14; // [esp+30h] [ebp-1E4h]
  float v15; // [esp+34h] [ebp-1E0h]
  float v16; // [esp+38h] [ebp-1DCh]
  float v17; // [esp+3Ch] [ebp-1D8h]
  float v18; // [esp+40h] [ebp-1D4h]
  float v19; // [esp+54h] [ebp-1C0h]
  float v20; // [esp+58h] [ebp-1BCh]
  float v21; // [esp+5Ch] [ebp-1B8h]
  phys_vec3 *v22; // [esp+60h] [ebp-1B4h]
  phys_vec3 *v23; // [esp+64h] [ebp-1B0h]
  phys_vec3 v24; // [esp+68h] [ebp-1ACh] BYREF
  unsigned int v25[3]; // [esp+78h] [ebp-19Ch] BYREF
  phys_vec3 a_mom_loc1; // [esp+84h] [ebp-190h]
  phys_vec3 *p_m_inv_inertia; // [esp+94h] [ebp-180h]
  float v28[3]; // [esp+98h] [ebp-17Ch] BYREF
  phys_vec3 *p_m_a_vel; // [esp+B4h] [ebp-160h]
  float v30; // [esp+B8h] [ebp-15Ch]
  float v31; // [esp+BCh] [ebp-158h]
  float v32; // [esp+C0h] [ebp-154h]
  float v33; // [esp+C8h] [ebp-14Ch]
  float v34; // [esp+CCh] [ebp-148h]
  float v35; // [esp+D0h] [ebp-144h]
  const phys_vec3 *v36; // [esp+D4h] [ebp-140h]
  phys_vec3 v37; // [esp+D8h] [ebp-13Ch] BYREF
  phys_vec3 *p_m_t_vel; // [esp+F4h] [ebp-120h]
  float v39; // [esp+F8h] [ebp-11Ch]
  float v40; // [esp+FCh] [ebp-118h]
  float v41; // [esp+100h] [ebp-114h]
  float v42; // [esp+10Ch] [ebp-108h]
  float v43; // [esp+110h] [ebp-104h]
  float v44; // [esp+114h] [ebp-100h]
  float v45; // [esp+118h] [ebp-FCh]
  float v46; // [esp+11Ch] [ebp-F8h]
  float v47; // [esp+120h] [ebp-F4h]
  float v48; // [esp+12Ch] [ebp-E8h]
  float v49; // [esp+130h] [ebp-E4h]
  float v50; // [esp+134h] [ebp-E0h]
  float v51; // [esp+138h] [ebp-DCh]
  float v52; // [esp+13Ch] [ebp-D8h]
  float v53; // [esp+140h] [ebp-D4h]
  float v54; // [esp+154h] [ebp-C0h]
  float v55; // [esp+158h] [ebp-BCh]
  float v56; // [esp+15Ch] [ebp-B8h]
  float m_inv_mass; // [esp+160h] [ebp-B4h]
  phys_vec3 *v58; // [esp+164h] [ebp-B0h]
  float v59; // [esp+168h] [ebp-ACh]
  float v60; // [esp+16Ch] [ebp-A8h]
  float v61; // [esp+170h] [ebp-A4h]
  phys_vec3 m_gravity_acc_vec; // [esp+174h] [ebp-A0h]
  float v63; // [esp+188h] [ebp-8Ch]
  float v64; // [esp+18Ch] [ebp-88h]
  float v65; // [esp+190h] [ebp-84h]
  float value; // [esp+194h] [ebp-80h]
  float v67; // [esp+198h] [ebp-7Ch] BYREF
  float v68; // [esp+19Ch] [ebp-78h]
  float v69; // [esp+1A0h] [ebp-74h]
  phys_vec3 gravity_dir; // [esp+1A4h] [ebp-70h] BYREF
  float v71; // [esp+1F4h] [ebp-20h]
  phys_vec3 *p_m_torque_sum; // [esp+1F8h] [ebp-1Ch]
  float v73; // [esp+1FCh] [ebp-18h]
  phys_vec3 *p_m_force_sum; // [esp+200h] [ebp-14h]
  GlassPhysics *v75; // [esp+204h] [ebp-10h]
  unsigned int v76[2]; // [esp+208h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+214h] [ebp+0h]

  v76[0] = a2;
  v76[1] = retaddr;
  v75 = this;
  p_m_force_sum = &this->m_force_sum;
  v73 = 1.0 / deltaTime;
  this->m_force_sum.x = this->m_force_sum.x * (float)(1.0 / deltaTime);
  p_m_force_sum->y = p_m_force_sum->y * v73;
  p_m_force_sum->z = p_m_force_sum->z * v73;
  p_m_torque_sum = &v75->m_torque_sum;
  v71 = 1.0 / deltaTime;
  v75->m_torque_sum.x = v75->m_torque_sum.x * (float)(1.0 / deltaTime);
  p_m_torque_sum->y = p_m_torque_sum->y * v71;
  p_m_torque_sum->z = p_m_torque_sum->z * v71;
  GlassPhysics::tensor_transform_principle(
    v75,
    (int)v76,
    &v75->m_inv_inertia,
    &v75->m_mat,
    (phys_mat44 *)&gravity_dir.y);
  Phys_Vec3ToNitrousVec(&phys_gravity_dir->current.value, (phys_vec3 *)&v67);
  value = phys_gravity->current.value;
  v65 = value * v67;
  v64 = value * v68;
  v63 = value * v69;
  m_gravity_acc_vec.y = value * v67;
  m_gravity_acc_vec.z = value * v68;
  m_gravity_acc_vec.w = value * v69;
  v59 = value * v67;
  v60 = value * v68;
  v61 = value * v69;
  v58 = &v75->m_force_sum;
  m_inv_mass = v75->m_inv_mass;
  v56 = m_inv_mass * v75->m_force_sum.x;
  v55 = m_inv_mass * v75->m_force_sum.y;
  v54 = m_inv_mass * v75->m_force_sum.z;
  v51 = v56;
  v52 = v55;
  v53 = v54;
  v50 = v56 + (float)(value * v67);
  v49 = v55 + (float)(value * v68);
  v48 = v54 + (float)(value * v69);
  v45 = v50;
  v46 = v49;
  v47 = v48;
  v44 = deltaTime * v50;
  v43 = deltaTime * v49;
  v42 = deltaTime * v48;
  v39 = deltaTime * v50;
  v40 = deltaTime * v49;
  v41 = deltaTime * v48;
  p_m_t_vel = &v75->m_t_vel;
  v75->m_t_vel.x = v75->m_t_vel.x + (float)(deltaTime * v50);
  p_m_t_vel->y = p_m_t_vel->y + v40;
  p_m_t_vel->z = p_m_t_vel->z + v41;
  v36 = phys_multiply(&v37, (const phys_mat44 *)&gravity_dir.y, &v75->m_torque_sum);
  v35 = deltaTime * v36->x;
  v34 = deltaTime * v36->y;
  v33 = deltaTime * v36->z;
  v30 = v35;
  v31 = v34;
  v32 = v33;
  p_m_a_vel = &v75->m_a_vel;
  v75->m_a_vel.x = v75->m_a_vel.x + v35;
  p_m_a_vel->y = p_m_a_vel->y + v31;
  p_m_a_vel->z = p_m_a_vel->z + v32;
  phys_inv_multiply((const phys_vec3 *)v28, &v75->m_mat, &v75->m_a_vel);
  p_m_inv_inertia = &v75->m_inv_inertia;
  a_mom_loc1.w = v28[0] / v75->m_inv_inertia.x;
  a_mom_loc1.z = v28[1] / v75->m_inv_inertia.y;
  a_mom_loc1.y = v28[2] / v75->m_inv_inertia.z;
  v25[0] = LODWORD(a_mom_loc1.w);
  v25[1] = LODWORD(a_mom_loc1.z);
  v25[2] = LODWORD(a_mom_loc1.y);
  v23 = phys_cross(&v24, (const phys_vec3 *)v25, (const phys_vec3 *)v28);
  v22 = &v75->m_inv_inertia;
  v21 = v75->m_inv_inertia.x * v23->x;
  v20 = v75->m_inv_inertia.y * v23->y;
  v19 = v75->m_inv_inertia.z * v23->z;
  v16 = v21;
  v17 = v20;
  v18 = v19;
  v15 = deltaTime * v21;
  v14 = deltaTime * v20;
  v13 = deltaTime * v19;
  v12.x = deltaTime * v21;
  v12.y = deltaTime * v20;
  v12.z = deltaTime * v19;
  v3 = phys_multiply((const phys_vec3 *)&v11, &v75->m_mat, &v12);
  phys_vec3::operator+=(&v75->m_a_vel, v3);
  v10 = AbsSquared(&v75->m_a_vel);
  if ( v10 > (float)(16.0 * 16.0) )
  {
    if ( v10 <= (float)(0.0000099999997 * 0.0000099999997)
      && _tlAssert(
           "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp",
           183,
           "navel_sq > phys_sqr(0.00001f)",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    v9 = 16.0 / fsqrt(v10);
    v8 = &v75->m_a_vel;
    v75->m_a_vel.x = v75->m_a_vel.x * v9;
    v8->y = v8->y * v9;
    v8->z = v8->z * v9;
  }
  if ( (float)(deltaTime * phys_dragLinear->current.value) >= 0.89999998 )
    v4 = FLOAT_0_89999998;
  else
    v4 = deltaTime * phys_dragLinear->current.value;
  v7 = &v75->m_t_vel;
  v75->m_t_vel.x = v75->m_t_vel.x * (float)(1.0 - v4);
  v7->y = v7->y * (float)(1.0 - v4);
  v7->z = v7->z * (float)(1.0 - v4);
  if ( (float)(deltaTime * phys_dragAngular->current.value) >= 0.89999998 )
    v5 = FLOAT_0_89999998;
  else
    v5 = deltaTime * phys_dragAngular->current.value;
  v6 = &v75->m_a_vel;
  v75->m_a_vel.x = v75->m_a_vel.x * (float)(1.0 - v5);
  v6->y = v6->y * (float)(1.0 - v5);
  v6->z = v6->z * (float)(1.0 - v5);
}

phys_vec3 *__thiscall phys_vec3::operator+=(phys_vec3 *this, const phys_vec3 *v)
{
  this->x = this->x + v->x;
  this->y = this->y + v->y;
  this->z = this->z + v->z;
  return this;
}

void __userpurge GlassPhysics::IntegratePos(GlassPhysics *this@<ecx>, int a2@<ebp>, float deltaTime)
{
  _BYTE v3[12]; // [esp+8h] [ebp-8Ch] BYREF
  phys_vec3 *p_w; // [esp+54h] [ebp-40h]
  float v5; // [esp+58h] [ebp-3Ch]
  float v6; // [esp+5Ch] [ebp-38h]
  float v7; // [esp+60h] [ebp-34h]
  float v8; // [esp+74h] [ebp-20h]
  float v9; // [esp+78h] [ebp-1Ch]
  float v10; // [esp+7Ch] [ebp-18h]
  phys_vec3 *p_m_t_vel; // [esp+80h] [ebp-14h]
  GlassPhysics *v12; // [esp+84h] [ebp-10h]
  unsigned int v13[2]; // [esp+88h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+94h] [ebp+0h]

  v13[0] = a2;
  v13[1] = retaddr;
  v12 = this;
  p_m_t_vel = &this->m_t_vel;
  v10 = deltaTime * this->m_t_vel.x;
  v9 = deltaTime * this->m_t_vel.y;
  v8 = deltaTime * this->m_t_vel.z;
  v5 = v10;
  v6 = v9;
  v7 = v8;
  p_w = &this->m_mat.w;
  this->m_mat.w.x = this->m_mat.w.x + v10;
  p_w->y = p_w->y + v6;
  p_w->z = p_w->z + v7;
  make_rotate((int)v13, (phys_mat44 *)v3, &v12->m_a_vel, deltaTime, 1000.0);
  phys_multiply_mat((int)v13, &v12->m_mat, (const phys_mat44 *)v3, &v12->m_mat);
}

void __thiscall GlassPhysics::GetPosition(GlassPhysics *this, float *position, float (*axis)[3])
{
  Phys_NitrousVecToVec3(&this->m_mat.w, position);
  Phys_NitrousMat44ToVec33(&this->m_mat, axis);
}

int __thiscall GlassShard::Outline::Init(
        GlassShard::Outline *this,
        GlassShard::Outline::Vertex *ptr,
        unsigned __int8 num)
{
  this->verts = ptr;
  this->maxVerts = num;
  this->numVerts = 0;
  return 24 * this->maxVerts;
}

int __thiscall GlassShard::Outline::SetPointers(GlassShard::Outline *this, unsigned __int8 *ptr)
{
  this->verts = (GlassShard::Outline::Vertex *)ptr;
  return 24 * this->numVerts;
}

GlassShard::Outline *__thiscall GlassShard::Outline::operator=(GlassShard::Outline *this, GlassShard::Outline *o)
{
  if ( !this->verts
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 462, 0, "%s", "verts") )
  {
    __debugbreak();
  }
  if ( !o->verts
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 463, 0, "%s", "o.verts") )
  {
    __debugbreak();
  }
  if ( this->maxVerts < (int)o->numVerts
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp",
          464,
          0,
          "%s",
          "maxVerts >= o.numVerts") )
  {
    __debugbreak();
  }
  this->isClosed = o->isClosed;
  this->numVerts = o->numVerts;
  memcpy((unsigned __int8 *)this->verts, (unsigned __int8 *)o->verts, 24 * this->numVerts);
  this->length = GlassShard::Outline::Length(o);
  this->area = GlassShard::Outline::Area(o);
  GlassShard::Outline::Verify(this);
  return this;
}

double __thiscall GlassShard::Outline::Length(GlassShard::Outline *this)
{
  if ( !this->isClosed
    && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.h", 145, 0, "%s", "isClosed") )
  {
    __debugbreak();
  }
  return this->length;
}

GlassShard::Outline::Vertex *__thiscall GlassShard::Outline::Add(GlassShard::Outline *this, const float *pos)
{
  float *dir; // [esp+4h] [ebp-20h]
  GlassShard::Outline::Vertex *v5; // [esp+8h] [ebp-1Ch]
  float v6; // [esp+Ch] [ebp-18h]
  float *v; // [esp+10h] [ebp-14h]
  GlassShard::Outline::Vertex *v8; // [esp+14h] [ebp-10h]

  if ( this->numVerts < (int)this->maxVerts )
  {
    if ( this->numVerts )
    {
      v = this->verts[this->numVerts - 1].edge.dir;
      v8 = &this->verts[this->numVerts - 1];
      *v = *pos - v8->edge.origin[0];
      v[1] = pos[1] - v8->edge.origin[1];
      v6 = Vec2Normalize(v);
      this->verts[this->numVerts - 1].edge.len = v6;
    }
    v5 = &this->verts[this->numVerts];
    v5->edge.origin[0] = *pos;
    v5->edge.origin[1] = pos[1];
    dir = this->verts[this->numVerts].edge.dir;
    *dir = *(float *)&FLOAT_0_0;
    dir[1] = *(float *)&FLOAT_0_0;
    this->verts[this->numVerts].edge.len = *(float *)&FLOAT_0_0;
    this->verts[this->numVerts++].isOriginalEdge = 0;
  }
  return &this->verts[this->numVerts - 1];
}

GlassShard::Outline::Vertex *__thiscall GlassShard::Outline::Add(GlassShard::Outline *this, float *dir, float len)
{
  float *v5; // [esp+4h] [ebp-18h]
  GlassShard::Outline::Vertex *v6; // [esp+8h] [ebp-14h]
  GlassShard::Outline::Vertex *v7; // [esp+Ch] [ebp-10h]
  float v8; // [esp+14h] [ebp-8h]
  float *v9; // [esp+18h] [ebp-4h]

  if ( !this->numVerts
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 503, 0, "%s", "numVerts > 0") )
  {
    __debugbreak();
  }
  if ( this->numVerts < (int)this->maxVerts )
  {
    v9 = this->verts[this->numVerts - 1].edge.dir;
    *v9 = *dir;
    v9[1] = dir[1];
    this->verts[this->numVerts - 1].edge.len = len;
    v6 = &this->verts[this->numVerts];
    v7 = &this->verts[this->numVerts - 1];
    v8 = v7->edge.len;
    v6->edge.origin[0] = (float)(v8 * v7->edge.dir[0]) + v7->edge.origin[0];
    v6->edge.origin[1] = (float)(v8 * v7->edge.dir[1]) + v7->edge.origin[1];
    v5 = this->verts[this->numVerts].edge.dir;
    *v5 = *(float *)&FLOAT_0_0;
    v5[1] = *(float *)&FLOAT_0_0;
    this->verts[this->numVerts].edge.len = *(float *)&FLOAT_0_0;
    this->verts[this->numVerts++].isOriginalEdge = 0;
  }
  return &this->verts[this->numVerts - 1];
}

char __thiscall GlassShard::Outline::CloseOutline(GlassShard::Outline *this)
{
  float v3; // [esp+1Ch] [ebp-4Ch]
  float *dir; // [esp+20h] [ebp-48h]
  GlassShard::Outline::Vertex *v5; // [esp+24h] [ebp-44h]
  GlassShard::Outline::Vertex *v6; // [esp+28h] [ebp-40h]
  float v7; // [esp+4Ch] [ebp-1Ch]
  float *v; // [esp+50h] [ebp-18h]
  GlassShard::Outline::Vertex *v9; // [esp+54h] [ebp-14h]
  GlassShard::Outline::Vertex *verts; // [esp+58h] [ebp-10h]
  int v11; // [esp+5Ch] [ebp-Ch]
  unsigned int i; // [esp+60h] [ebp-8h]
  unsigned __int8 v2; // [esp+66h] [ebp-2h]
  unsigned __int8 v1; // [esp+67h] [ebp-1h]

  if ( this->numVerts <= 2u
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 525, 0, "%s", "numVerts > 2") )
  {
    __debugbreak();
  }
  v = this->verts[this->numVerts - 1].edge.dir;
  v9 = &this->verts[this->numVerts - 1];
  verts = this->verts;
  *v = this->verts->edge.origin[0] - v9->edge.origin[0];
  v[1] = verts->edge.origin[1] - v9->edge.origin[1];
  v7 = Vec2Normalize(v);
  this->verts[this->numVerts - 1].edge.len = v7;
  v1 = 0;
  while ( v1 < (int)this->numVerts )
  {
    if ( this->verts[v1].edge.len >= 0.0099999998 )
    {
      ++v1;
    }
    else
    {
      if ( this->numVerts == 3 )
        return 0;
      for ( v2 = v1; v2 < this->numVerts - 1; ++v2 )
        GlassShard::Outline::Vertex::operator=(&this->verts[v2], &this->verts[v2 + 1]);
      --this->numVerts;
      dir = this->verts[v1].edge.dir;
      v5 = &this->verts[v1];
      v6 = &this->verts[((signed int)-abs(v1 - (this->numVerts - 1)) >> 31) & (v1 + 1)];
      *dir = v6->edge.origin[0] - v5->edge.origin[0];
      dir[1] = v6->edge.origin[1] - v5->edge.origin[1];
      v3 = Vec2Normalize(dir);
      this->verts[v1].edge.len = v3;
      v1 = 0;
    }
  }
  this->length = *(float *)&FLOAT_0_0;
  this->area = *(float *)&FLOAT_0_0;
  for ( i = 0; i < this->numVerts; ++i )
  {
    v11 = ((signed int)-abs(i - (this->numVerts - 1)) >> 31) & (i + 1);
    this->length = this->length + this->verts[i].edge.len;
    this->area = (float)((float)((float)(this->verts[i].edge.origin[0] * this->verts[v11].edge.origin[1])
                               - (float)(this->verts[i].edge.origin[1] * this->verts[v11].edge.origin[0]))
                       * 0.5)
               + this->area;
  }
  LODWORD(this->area) &= _mask__AbsFloat_;
  this->isClosed = 1;
  GlassShard::Outline::Verify(this);
  return 1;
}

void __thiscall GlassShard::Outline::Vertex::operator=(
        GlassShard::Outline::Vertex *this,
        const GlassShard::Outline::Vertex *other)
{
  *this = *other;
}

void __thiscall GlassShard::Outline::GetBBox(GlassShard::Outline *this, float *mn, float *mx)
{
  GlassShard::Outline::Vertex *v3; // eax
  float v4; // [esp+0h] [ebp-28h]
  float v5; // [esp+4h] [ebp-24h]
  float v6; // [esp+8h] [ebp-20h]
  float v7; // [esp+Ch] [ebp-1Ch]
  GlassShard::Outline::Vertex *v8; // [esp+14h] [ebp-14h]
  GlassShard::Outline::Vertex *v9; // [esp+18h] [ebp-10h]
  GlassShard::Outline::Vertex *verts; // [esp+20h] [ebp-8h]
  unsigned int i; // [esp+24h] [ebp-4h]

  verts = this->verts;
  *mn = this->verts->edge.origin[0];
  mn[1] = verts->edge.origin[1];
  v3 = this->verts;
  *mx = this->verts->edge.origin[0];
  mx[1] = v3->edge.origin[1];
  for ( i = 1; i < this->numVerts; ++i )
  {
    v9 = &this->verts[i];
    if ( *mn <= v9->edge.origin[0] )
      v7 = *mn;
    else
      v7 = v9->edge.origin[0];
    *mn = v7;
    if ( mn[1] <= v9->edge.origin[1] )
      v6 = mn[1];
    else
      v6 = v9->edge.origin[1];
    mn[1] = v6;
    v8 = &this->verts[i];
    if ( v8->edge.origin[0] <= *mx )
      v5 = *mx;
    else
      v5 = v8->edge.origin[0];
    *mx = v5;
    if ( v8->edge.origin[1] <= mx[1] )
      v4 = mx[1];
    else
      v4 = v8->edge.origin[1];
    mx[1] = v4;
  }
}

double __thiscall GlassShard::Outline::Extent(GlassShard::Outline *this)
{
  float v[2]; // [esp+8h] [ebp-18h] BYREF
  float mx[2]; // [esp+10h] [ebp-10h] BYREF
  float mn[2]; // [esp+18h] [ebp-8h] BYREF

  GlassShard::Outline::GetBBox(this, mn, mx);
  v[0] = mx[0] - mn[0];
  v[1] = mx[1] - mn[1];
  return Vec2Length(v);
}

void __thiscall GlassShard::Outline::Recenter(GlassShard::Outline *this, bool flip, float *offset)
{
  GlassShard::Outline::Vertex *v4; // [esp+4h] [ebp-1Ch]
  unsigned int i; // [esp+Ch] [ebp-14h]
  float mx[2]; // [esp+10h] [ebp-10h] BYREF
  float mn[2]; // [esp+18h] [ebp-8h] BYREF

  if ( flip )
    GlassShard::Outline::Reverse(this);
  GlassShard::Outline::GetBBox(this, mn, mx);
  *offset = (float)(mn[0] + mx[0]) * 0.5;
  offset[1] = (float)(mn[1] + mx[1]) * 0.5;
  for ( i = 0; i < this->numVerts; ++i )
  {
    v4 = &this->verts[i];
    v4->edge.origin[0] = v4->edge.origin[0] - *offset;
    v4->edge.origin[1] = v4->edge.origin[1] - offset[1];
  }
}

bool __thiscall GlassShard::Outline::HasNarrowAngle(GlassShard::Outline *this)
{
  float v2; // [esp+4h] [ebp-20h]
  unsigned int i; // [esp+1Ch] [ebp-8h]
  float val; // [esp+20h] [ebp-4h]

  val = FLOAT_1_0;
  for ( i = 0; i < this->numVerts; ++i )
  {
    if ( (float)((float)((float)(this->verts[i].edge.dir[0] * this->verts[(i + 1) % this->numVerts].edge.dir[0])
                       + (float)(this->verts[i].edge.dir[1] * this->verts[(i + 1) % this->numVerts].edge.dir[1]))
               - val) < 0.0 )
      v2 = (float)(this->verts[i].edge.dir[0] * this->verts[(i + 1) % this->numVerts].edge.dir[0])
         + (float)(this->verts[i].edge.dir[1] * this->verts[(i + 1) % this->numVerts].edge.dir[1]);
    else
      v2 = val;
    val = v2;
  }
  return val < -0.89999998;
}

bool __thiscall GlassShard::Outline::IsNarrow(GlassShard::Outline *this)
{
  float v2; // [esp+0h] [ebp-38h]
  float v3; // [esp+4h] [ebp-34h]
  float v4; // [esp+8h] [ebp-30h]
  float area2bboxRatio; // [esp+1Ch] [ebp-1Ch]
  float mx[2]; // [esp+20h] [ebp-18h] BYREF
  float mn[2]; // [esp+28h] [ebp-10h] BYREF
  float extent[2]; // [esp+30h] [ebp-8h]

  GlassShard::Outline::GetBBox(this, mn, mx);
  extent[0] = mx[0] - mn[0];
  extent[1] = mx[1] - mn[1];
  if ( (float)((float)(mx[0] - mn[0]) - (float)(mx[1] - mn[1])) < 0.0 )
    v4 = extent[1];
  else
    v4 = extent[0];
  if ( (float)(extent[1] - extent[0]) < 0.0 )
    v3 = extent[1];
  else
    v3 = extent[0];
  if ( (float)(v4 / v3) > clGlasses->renderer->narrowShardRatio->current.value )
    return 1;
  v2 = extent[0] * extent[1];
  area2bboxRatio = v2 / GlassShard::Outline::Area(this);
  return area2bboxRatio > clGlasses->renderer->narrowShardRatio->current.value;
}

void __thiscall GlassShard::Outline::Reverse(GlassShard::Outline *this)
{
  int v1; // [esp+0h] [ebp-630h]
  GlassShard::Outline::Vertex *v3; // [esp+8h] [ebp-628h]
  float len; // [esp+Ch] [ebp-624h]
  GlassShard::Outline::Vertex *v5; // [esp+10h] [ebp-620h]
  float dir; // [esp+1Ch] [ebp-614h]
  float dir_4; // [esp+20h] [ebp-610h]
  int j1; // [esp+24h] [ebp-60Ch]
  unsigned int i; // [esp+2Ch] [ebp-604h]
  GlassShard::Outline::Vertex tmpVerts[64]; // [esp+30h] [ebp-600h] BYREF

  if ( !this->isClosed
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 634, 0, "%s", "isClosed") )
  {
    __debugbreak();
  }
  for ( i = 0; i < this->numVerts; ++i )
  {
    j1 = this->numVerts - 1 - i;
    if ( j1 <= 0 )
      v1 = this->numVerts - 1;
    else
      v1 = j1 - 1;
    LODWORD(dir) = LODWORD(this->verts[v1].edge.dir[0]) ^ _mask__NegFloat_;
    LODWORD(dir_4) = LODWORD(this->verts[v1].edge.dir[1]) ^ _mask__NegFloat_;
    v3 = &tmpVerts[i];
    len = this->verts[v1].edge.len;
    v5 = &this->verts[j1];
    v3->edge.origin[0] = v5->edge.origin[0];
    v3->edge.origin[1] = v5->edge.origin[1];
    v3->edge.dir[0] = dir;
    v3->edge.dir[1] = dir_4;
    v3->edge.len = len;
    tmpVerts[i].isOriginalEdge = this->verts[v1].isOriginalEdge;
  }
  memcpy((unsigned __int8 *)this->verts, (unsigned __int8 *)tmpVerts, 24 * this->numVerts);
  GlassShard::Outline::Verify(this);
}

int __thiscall GlassShard::Outline::GetNumIntersections(
        GlassShard::Outline *this,
        const float *start,
        const float *dir,
        float *nearestDist,
        int *nearestedge)
{
  double SegmentParam; // st7
  float dist; // [esp+44h] [ebp-20h]
  float p[2]; // [esp+48h] [ebp-1Ch] BYREF
  unsigned int i; // [esp+50h] [ebp-14h]
  float TEST_LENGTH; // [esp+54h] [ebp-10h]
  float end[2]; // [esp+58h] [ebp-Ch] BYREF
  int n; // [esp+60h] [ebp-4h]

  TEST_LENGTH = FLOAT_1000_0;
  n = 0;
  end[0] = (float)(1000.0 * *dir) + *start;
  end[1] = (float)(1000.0 * dir[1]) + start[1];
  *nearestDist = FLOAT_1000_0;
  for ( i = 0; i < this->numVerts; ++i )
  {
    if ( Vec2IntersectSegments(
           this->verts[i].edge.origin,
           this->verts[(i + 1) % this->numVerts].edge.origin,
           start,
           end,
           p) )
    {
      SegmentParam = GetSegmentParam(start, end, p);
      dist = SegmentParam * TEST_LENGTH;
      if ( *nearestDist > dist )
      {
        *nearestDist = dist;
        *nearestedge = i;
      }
      ++n;
    }
  }
  return n;
}

double __cdecl GetSegmentParam(const float *a1, const float *a2, const float *p)
{
  float v5; // [esp+8h] [ebp-4h]

  v5 = *a2 - *a1;
  if ( (float)(COERCE_FLOAT(LODWORD(v5) & _mask__AbsFloat_) - 0.0000099999997) < 0.0 )
    return (float)((float)(p[1] - a1[1]) / (float)(a2[1] - a1[1]));
  else
    return (float)((float)(*p - *a1) / v5);
}

bool __cdecl Vec2IntersectSegments(const float *a1, const float *a2, const float *b1, const float *b2, float *ret)
{
  bool result; // al
  float t2; // [esp+28h] [ebp-8h]
  float t1; // [esp+2Ch] [ebp-4h]

  result = 0;
  if ( Vec2IntesectLines(a1, a2, b1, b2, ret) )
  {
    t1 = GetSegmentParam(a1, a2, ret);
    if ( t1 > 0.0000099999997 && (float)(1.0 - 0.0000099999997) > t1 )
    {
      t2 = GetSegmentParam(b1, b2, ret);
      if ( t2 > 0.0000099999997 && (float)(1.0 - 0.0000099999997) > t2 )
        return 1;
    }
  }
  return result;
}

char __cdecl Vec2IntesectLines(const float *a1, const float *a2, const float *b1, const float *b2, float *ret)
{
  float det; // [esp+14h] [ebp-4h]

  det = (float)((float)(*a1 - *a2) * (float)(b1[1] - b2[1])) - (float)((float)(a1[1] - a2[1]) * (float)(*b1 - *b2));
  if ( COERCE_FLOAT(LODWORD(det) & _mask__AbsFloat_) <= 0.0000099999997 )
    return 0;
  *ret = (float)((float)((float)(*b1 - *b2) * (float)((float)(*a1 * a2[1]) - (float)(a1[1] * *a2)))
               - (float)((float)(*a1 - *a2) * (float)((float)(*b1 * b2[1]) - (float)(b1[1] * *b2))))
       / det;
  ret[1] = (float)((float)((float)(b1[1] - b2[1]) * (float)((float)(*a1 * a2[1]) - (float)(a1[1] * *a2)))
                 - (float)((float)(a1[1] - a2[1]) * (float)((float)(*b1 * b2[1]) - (float)(b1[1] * *b2))))
         / det;
  return 1;
}

char __thiscall GlassShard::Outline::DoesIntersect(
        GlassShard::Outline *this,
        const float *start,
        const float *dir,
        float len,
        float padding)
{
  unsigned int numVerts; // [esp+0h] [ebp-7Ch]
  float v8; // [esp+38h] [ebp-44h]
  GlassShard::Outline::Vertex *v9; // [esp+3Ch] [ebp-40h]
  float *v10; // [esp+44h] [ebp-38h]
  GlassShard::Outline::Vertex *v11; // [esp+4Ch] [ebp-30h]
  float *v12; // [esp+50h] [ebp-2Ch]
  float p1[2]; // [esp+54h] [ebp-28h] BYREF
  float p2[2]; // [esp+5Ch] [ebp-20h] BYREF
  float p[2]; // [esp+64h] [ebp-18h] BYREF
  unsigned int i; // [esp+6Ch] [ebp-10h]
  float end[2]; // [esp+70h] [ebp-Ch] BYREF
  unsigned int num; // [esp+78h] [ebp-4h]

  end[0] = (float)(len * *dir) + *start;
  end[1] = (float)(len * dir[1]) + start[1];
  if ( this->isClosed )
    numVerts = this->numVerts;
  else
    numVerts = this->numVerts - 1;
  num = numVerts;
  for ( i = 0; i < num; ++i )
  {
    v12 = this->verts[i].edge.dir;
    v11 = &this->verts[i];
    p1[0] = (float)(COERCE_FLOAT(LODWORD(padding) ^ _mask__NegFloat_) * *v12) + v11->edge.origin[0];
    p1[1] = (float)(COERCE_FLOAT(LODWORD(padding) ^ _mask__NegFloat_) * v12[1]) + v11->edge.origin[1];
    v10 = this->verts[i].edge.dir;
    v8 = this->verts[i].edge.len + padding;
    v9 = &this->verts[i];
    p2[0] = (float)(v8 * *v10) + v9->edge.origin[0];
    p2[1] = (float)(v8 * v10[1]) + v9->edge.origin[1];
    if ( Vec2IntersectSegments(p1, p2, start, end, p) )
      return 1;
  }
  return 0;
}

void __thiscall GlassShard::Outline::GetNearestDistances(
        GlassShard::Outline *this,
        const float *p,
        GlassShard::Outline::EdgeDistance *dists,
        int nDists)
{
  GlassShard::Outline::EdgeDistance *v4; // edx
  GlassShard::Outline::EdgeDistance *v5; // eax
  int v6; // [esp+0h] [ebp-7Ch]
  float v7; // [esp+4h] [ebp-78h]
  float v8; // [esp+1Ch] [ebp-60h]
  GlassShard::Outline::Vertex *v9; // [esp+20h] [ebp-5Ch]
  float v10; // [esp+2Ch] [ebp-50h]
  float v11; // [esp+30h] [ebp-4Ch]
  float len; // [esp+44h] [ebp-38h]
  int n; // [esp+48h] [ebp-34h]
  int k; // [esp+4Ch] [ebp-30h]
  int j; // [esp+50h] [ebp-2Ch]
  __int64 np; // [esp+58h] [ebp-24h]
  float d2; // [esp+68h] [ebp-14h]
  signed int m; // [esp+74h] [ebp-8h]
  int i; // [esp+78h] [ebp-4h]

  for ( i = 0; i < nDists; ++i )
    dists[i].edgeIndex = -1;
  for ( m = 0; m < this->numVerts; ++m )
  {
    len = this->verts[m].edge.len;
    v10 = (float)((float)((float)(len * this->verts[m].edge.dir[0]) * (float)(*p - this->verts[m].edge.origin[0]))
                + (float)((float)(len * this->verts[m].edge.dir[1]) * (float)(p[1] - this->verts[m].edge.origin[1])))
        / (float)(len * len);
    if ( (float)(v10 - 1.0) < 0.0 )
      v11 = (float)((float)((float)(len * this->verts[m].edge.dir[0]) * (float)(*p - this->verts[m].edge.origin[0]))
                  + (float)((float)(len * this->verts[m].edge.dir[1]) * (float)(p[1] - this->verts[m].edge.origin[1])))
          / (float)(len * len);
    else
      v11 = FLOAT_1_0;
    if ( (float)(0.0 - v10) < 0.0 )
      v7 = v11;
    else
      v7 = *(float *)&FLOAT_0_0;
    v8 = v7 * this->verts[m].edge.len;
    v9 = &this->verts[m];
    *(float *)&np = (float)(v8 * v9->edge.dir[0]) + v9->edge.origin[0];
    *((float *)&np + 1) = (float)(v8 * v9->edge.dir[1]) + v9->edge.origin[1];
    d2 = (float)((float)(*p - *(float *)&np) * (float)(*p - *(float *)&np))
       + (float)((float)(p[1] - *((float *)&np + 1)) * (float)(p[1] - *((float *)&np + 1)));
    for ( j = 0; ; ++j )
    {
      v6 = nDists < m ? nDists : m;
      if ( j >= v6 )
        break;
      if ( dists[j].dist > d2 )
      {
        for ( k = nDists - 1; k > j; --k )
        {
          v4 = &dists[k - 1];
          v5 = &dists[k];
          v5->edgeIndex = v4->edgeIndex;
          v5->edgeParam = v4->edgeParam;
          v5->dist = v4->dist;
          v5->closestPoint[0] = v4->closestPoint[0];
          v5->closestPoint[1] = v4->closestPoint[1];
        }
        break;
      }
    }
    if ( j < nDists )
    {
      dists[j].edgeIndex = m;
      dists[j].edgeParam = v7;
      dists[j].dist = d2;
      *(_QWORD *)dists[j].closestPoint = np;
    }
  }
  for ( n = 0; n < nDists; ++n )
  {
    if ( dists[n].edgeIndex == -1
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp",
            760,
            0,
            "%s",
            "dists[i].edgeIndex != uint(-1)") )
    {
      __debugbreak();
    }
    dists[n].dist = fsqrt(dists[n].dist);
  }
}

void __thiscall GlassShard::Outline::Verify(GlassShard::Outline *this)
{
  if ( !this->isClosed
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 781, 0, "%s", "isClosed") )
  {
    __debugbreak();
  }
}

void __thiscall GlassShard::Mesh::Clear(GlassShard::Mesh *this)
{
  this->normArray = 0;
  this->indices = 0;
  *(unsigned int *)&this->numNorm = 0;
  *(unsigned int *)&this->numIndicesLow = 0;
}

void __thiscall GlassShard::Mesh::SetTriangles(
        GlassShard::Mesh *this,
        unsigned __int8 *ptr,
        unsigned __int8 *triangles,
        unsigned int numTriIdxs)
{
  GlassShard::Mesh::Clear(this);
  this->indices = ptr;
  memcpy(this->indices, triangles, numTriIdxs);
}

int __thiscall GlassShard::Mesh::SetPointers(GlassShard::Mesh *this, PackedUnitVec *ptr)
{
  this->normArray = ptr;
  this->indices = (unsigned __int8 *)&ptr[this->numNorm];
  return this->numIndices + 4 * this->numNorm;
}

void __thiscall GlassShard::Mesh::Init(
        GlassShard::Mesh *this,
        const GlassShard::Outline *outline,
        float thickness,
        const float *tangt)
{
  PackedUnitVec v5; // [esp+Ch] [ebp-208h]
  float v6; // [esp+38h] [ebp-1DCh]
  float v7; // [esp+44h] [ebp-1D0h]
  float v8; // [esp+50h] [ebp-1C4h]
  PackedUnitVec *v9; // [esp+5Ch] [ebp-1B8h]
  unsigned int v10; // [esp+60h] [ebp-1B4h]
  PackedUnitVec *v11; // [esp+7Ch] [ebp-198h]
  unsigned int v12; // [esp+80h] [ebp-194h]
  float d1[3]; // [esp+A0h] [ebp-174h] BYREF
  float normal[3]; // [esp+ACh] [ebp-168h] BYREF
  float p1[3]; // [esp+B8h] [ebp-15Ch]
  float p3[3]; // [esp+C4h] [ebp-150h]
  float d2[3]; // [esp+D0h] [ebp-144h] BYREF
  int v2; // [esp+DCh] [ebp-138h]
  float p2[3]; // [esp+E0h] [ebp-134h]
  int v1; // [esp+ECh] [ebp-128h]
  int j; // [esp+F0h] [ebp-124h]
  int i; // [esp+F4h] [ebp-120h]
  int numTriVerts; // [esp+F8h] [ebp-11Ch]
  unsigned __int8 triangles[260]; // [esp+FCh] [ebp-118h] BYREF
  unsigned __int8 *ptr; // [esp+204h] [ebp-10h]
  float vec[3]; // [esp+208h] [ebp-Ch]

  if ( !this->normArray )
  {
    numTriVerts = 3 * (outline->numVerts - 2);
    memcpy(triangles, this->indices, numTriVerts);
    ptr = this->indices;
    this->normArray = (PackedUnitVec *)ptr;
    ptr += 4 * outline->numVerts + 8;
    this->indices = ptr;
    ptr += 12 * outline->numVerts - 12;
    this->numNorm = 0;
    vec[0] = *(float *)&FLOAT_0_0;
    vec[1] = *(float *)&FLOAT_0_0;
    vec[2] = FLOAT_1_0;
    v11 = &this->normArray[this->numNorm++];
    LOBYTE(v12) = (int)(float)((float)(vec[0] * 127.0) + 127.5);
    BYTE1(v12) = (int)(float)((float)(vec[1] * 127.0) + 127.5);
    BYTE2(v12) = (int)(float)((float)(vec[2] * 127.0) + 127.5);
    HIBYTE(v12) = 63;
    v11->packed = v12;
    vec[0] = *(float *)&FLOAT_0_0;
    vec[1] = *(float *)&FLOAT_0_0;
    vec[2] = FLOAT_N1_0;
    v9 = &this->normArray[this->numNorm++];
    LOBYTE(v10) = (int)(float)((float)(vec[0] * 127.0) + 127.5);
    BYTE1(v10) = (int)(float)((float)(vec[1] * 127.0) + 127.5);
    BYTE2(v10) = (int)(float)((float)(vec[2] * 127.0) + 127.5);
    HIBYTE(v10) = 63;
    v9->packed = v10;
    this->numIndices = 0;
    for ( i = 0; i < numTriVerts; i += 3 )
    {
      this->indices[this->numIndices++] = 2 * triangles[i];
      this->indices[this->numIndices++] = 2 * triangles[i + 1];
      this->indices[this->numIndices++] = 2 * triangles[i + 2];
    }
    for ( j = 0; j < numTriVerts; j += 3 )
    {
      this->indices[this->numIndices++] = 2 * triangles[j] + 1;
      this->indices[this->numIndices++] = 2 * triangles[j + 2] + 1;
      this->indices[this->numIndices++] = 2 * triangles[j + 1] + 1;
    }
    this->numIndicesLow = this->numIndices;
    this->numVerts = 2 * outline->numVerts;
    this->numVertsLow = this->numVerts;
    for ( v1 = 0; v1 < outline->numVerts; ++v1 )
    {
      v2 = (v1 + 1) % outline->numVerts;
      v8 = outline->verts[v1].edge.origin[1];
      p1[0] = outline->verts[v1].edge.origin[0];
      p1[1] = v8;
      p1[2] = *(float *)&FLOAT_0_0;
      v7 = outline->verts[v2].edge.origin[1];
      p2[0] = outline->verts[v2].edge.origin[0];
      p2[1] = v7;
      p2[2] = *(float *)&FLOAT_0_0;
      v6 = outline->verts[v1].edge.origin[1];
      p3[0] = outline->verts[v1].edge.origin[0];
      p3[1] = v6;
      p3[2] = thickness;
      d1[0] = p1[0] - p2[0];
      d1[1] = v8 - v7;
      d1[2] = 0.0 - 0.0;
      d2[0] = p1[0] - p3[0];
      d2[1] = v8 - v6;
      d2[2] = 0.0 - thickness;
      Vec3Cross(d2, d1, normal);
      Vec3Normalize(normal);
      v5.array[0] = (int)(float)((float)(normal[0] * 127.0) + 127.5);
      v5.array[1] = (int)(float)((float)(normal[1] * 127.0) + 127.5);
      v5.array[2] = (int)(float)((float)(normal[2] * 127.0) + 127.5);
      v5.array[3] = 63;
      this->normArray[this->numNorm] = v5;
      this->indices[this->numIndices++] = this->numVerts;
      this->indices[this->numIndices++] = this->numVerts + 1;
      this->indices[this->numIndices++] = this->numVerts + 2;
      this->indices[this->numIndices++] = this->numVerts + 1;
      this->indices[this->numIndices++] = this->numVerts + 3;
      this->indices[this->numIndices++] = this->numVerts + 2;
      ++this->numNorm;
      this->numVerts += 4;
    }
    if ( this->numNorm != outline->numVerts + 2
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp",
            912,
            0,
            "%s",
            "numNorm == NumNorm( outline.NumVerts() )") )
    {
      __debugbreak();
    }
    if ( this->numVerts != 6 * outline->numVerts
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp",
            913,
            0,
            "%s",
            "numVerts == NumVerts( outline.NumVerts() )") )
    {
      __debugbreak();
    }
    if ( this->numIndices != 12 * (outline->numVerts - 1)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp",
            914,
            0,
            "%s",
            "numIndices == NumIndices( outline.NumVerts() )") )
    {
      __debugbreak();
    }
  }
}

void __cdecl GlassShard::Mesh::InitVertexList(unsigned __int8 numOutlineVerts, GlassShardMeshVertex *verts)
{
  unsigned __int8 v2; // [esp+0h] [ebp-4h]
  unsigned __int8 v1; // [esp+1h] [ebp-3h]
  unsigned __int8 i; // [esp+2h] [ebp-2h]
  unsigned __int8 numVerts; // [esp+3h] [ebp-1h]
  unsigned __int8 numVertsa; // [esp+3h] [ebp-1h]

  numVerts = 0;
  for ( i = 0; i < (int)numOutlineVerts; ++i )
  {
    verts[numVerts].pos = numVerts;
    verts[numVerts].norm = 0;
    numVertsa = numVerts + 1;
    verts[numVertsa].pos = numVertsa;
    verts[numVertsa].norm = 1;
    numVerts = numVertsa + 1;
  }
  for ( v1 = 0; v1 < (int)numOutlineVerts; ++v1 )
  {
    v2 = (v1 + 1) % numOutlineVerts;
    verts[numVerts].pos = 2 * v1;
    verts[numVerts].norm = v1 + 2;
    verts[numVerts + 1].pos = 2 * v1 + 1;
    verts[numVerts + 1].norm = v1 + 2;
    verts[numVerts + 2].pos = 2 * v2;
    verts[numVerts + 2].norm = v1 + 2;
    verts[numVerts + 3].pos = 2 * v2 + 1;
    verts[numVerts + 3].norm = v1 + 2;
    numVerts += 4;
  }
}

unsigned int __cdecl GlassShard::Mesh::GetMemorySize(unsigned int numOutineVerts)
{
  return 12 * (numOutineVerts - 1) + 4 * numOutineVerts + 8;
}

void __thiscall GlassShard::Triangles::Triangles(GlassShard::Triangles *this, const GlassShard::Outline *ol)
{
  this->outline = ol;
  this->nIndices = 0;
}

double __thiscall GlassShard::Triangles::CalcCross(GlassShard::Triangles *this, int idx1, int idx2, int idx3)
{
  return (float)(this->outline->verts[idx2].edge.origin[0] - this->outline->verts[idx1].edge.origin[0])
       * (float)(this->outline->verts[idx3].edge.origin[1] - this->outline->verts[idx2].edge.origin[1])
       - (float)(this->outline->verts[idx2].edge.origin[1] - this->outline->verts[idx1].edge.origin[1])
       * (float)(this->outline->verts[idx3].edge.origin[0] - this->outline->verts[idx2].edge.origin[0]);
}

char __thiscall GlassShard::Triangles::AddTri(
        GlassShard::Triangles *this,
        unsigned __int8 v1,
        unsigned __int8 v2,
        unsigned __int8 v3)
{
  if ( this->nIndices + 3 >= 0x100 )
    return 0;
  this->triangleIndices[this->nIndices++] = v1;
  this->triangleIndices[this->nIndices++] = v2;
  this->triangleIndices[this->nIndices++] = v3;
  return 1;
}

char __thiscall GlassShard::Triangles::Triangulate(GlassShard::Triangles *this)
{
  int m; // [esp+80h] [ebp-13Ch]
  int j; // [esp+84h] [ebp-138h]
  bool foundInside; // [esp+8Bh] [ebp-131h]
  int idx1; // [esp+8Ch] [ebp-130h]
  int idx2; // [esp+90h] [ebp-12Ch]
  int idx3; // [esp+94h] [ebp-128h]
  int k; // [esp+98h] [ebp-124h]
  int numBefore; // [esp+9Ch] [ebp-120h]
  int i; // [esp+A0h] [ebp-11Ch]
  _LARGE_INTEGER timer; // [esp+A4h] [ebp-118h]
  unsigned __int64 *timer_8; // [esp+ACh] [ebp-110h]
  int num; // [esp+B8h] [ebp-104h]
  int hullVtxIndex[64]; // [esp+BCh] [ebp-100h]

  timer_8 = &clGlasses->renderer->triangulateTimer;
  timer = tlPcGetTick();
  num = this->outline->numVerts;
  for ( i = 0; i < num; ++i )
    hullVtxIndex[i] = i;
  while ( num > 3 )
  {
    numBefore = num;
    for ( k = 0; k < num; ++k )
    {
      idx1 = hullVtxIndex[(k + num - 1) % num];
      idx2 = hullVtxIndex[k];
      idx3 = hullVtxIndex[(k + 1) % num];
      if ( GlassShard::Triangles::CalcCross(this, idx1, idx2, idx3) < 0.0 )
      {
        foundInside = 0;
        for ( j = 0; j < num - 3 && !foundInside; ++j )
          foundInside = IsInside(
                          this->outline->verts[idx1].edge.origin,
                          this->outline->verts[idx2].edge.origin,
                          this->outline->verts[idx3].edge.origin,
                          this->outline->verts[hullVtxIndex[(k + j + 2) % num]].edge.origin);
        if ( !foundInside )
        {
          if ( !GlassShard::Triangles::AddTri(
                  this,
                  hullVtxIndex[(k + num - 1) % num],
                  hullVtxIndex[k],
                  hullVtxIndex[(k + 1) % num]) )
            goto LABEL_14;
          for ( m = k; m < num - 1; ++m )
            hullVtxIndex[m] = hullVtxIndex[m + 1];
          --num;
          break;
        }
      }
    }
    if ( numBefore == num )
      goto LABEL_14;
  }
  if ( num != 3
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 1102, 0, "%s", "num == 3") )
  {
    __debugbreak();
  }
  if ( !GlassShard::Triangles::AddTri(this, hullVtxIndex[0], hullVtxIndex[1], hullVtxIndex[2]) )
  {
LABEL_14:
    *timer_8 += *(_QWORD *)&tlPcGetTick() - timer.QuadPart;
    return 0;
  }
  *timer_8 += *(_QWORD *)&tlPcGetTick() - timer.QuadPart;
  return 1;
}

bool __cdecl IsInside(const float *v1, const float *v2, const float *v3, const float *p)
{
  return (float)((float)((float)(*v2 - *v1) * (float)(p[1] - v1[1]))
               - (float)((float)(v2[1] - v1[1]) * (float)(*p - *v1))) <= 0.0
      && (float)((float)((float)(*v3 - *v2) * (float)(p[1] - v2[1]))
               - (float)((float)(v3[1] - v2[1]) * (float)(*p - *v2))) <= 0.0
      && (float)((float)((float)(*v1 - *v3) * (float)(p[1] - v3[1]))
               - (float)((float)(v1[1] - v3[1]) * (float)(*p - *v3))) <= 0.0;
}

void __thiscall GlassShard::Init(GlassShard *this)
{
  this->physObjId = 0;
  this->glassPhysics = 0;
  this->group = 0;
  this->remove = 0;
  this->inGroupChange = 0;
  this->memoryPtr = 0;
  this->delayedDrop = 0;
  this->worldBBoxMin[0] = FLOAT_N3_4028235e38;
  this->worldBBoxMin[1] = FLOAT_N3_4028235e38;
  this->worldBBoxMin[2] = FLOAT_N3_4028235e38;
  this->worldBBoxMax[0] = FLOAT_3_4028235e38;
  this->worldBBoxMax[1] = FLOAT_3_4028235e38;
  this->worldBBoxMax[2] = FLOAT_3_4028235e38;
}

void __thiscall GlassShard::Destroy(GlassShard *this)
{
  GlassShard::DestroyPhysicsObj(this);
  GlassShard::FreeMemory(this);
  memset((unsigned __int8 *)this, 0x29u, sizeof(GlassShard));
}

double __thiscall GlassShard::EdgeRatio(GlassShard *this)
{
  float len; // [esp+0h] [ebp-14h]
  unsigned int i; // [esp+Ch] [ebp-8h]
  float originalEdgeRatio; // [esp+10h] [ebp-4h]

  originalEdgeRatio = *(float *)&FLOAT_0_0;
  for ( i = 0; i < this->outline.numVerts; ++i )
  {
    if ( this->outline.verts[i].isOriginalEdge )
      len = this->outline.verts[i].edge.len;
    else
      len = *(float *)&FLOAT_0_0;
    originalEdgeRatio = originalEdgeRatio + len;
  }
  return originalEdgeRatio / GlassShard::Outline::Length(&this->outline);
}

void __thiscall GlassShard::UnEdge(GlassShard *this)
{
  if ( this->isOriginalEdge )
  {
    this->isOriginalEdge = 0;
    GlassShard::Outline::MarkAllEdge(&this->outline, 0);
  }
}

void __thiscall GlassShard::Outline::MarkAllEdge(GlassShard::Outline *this, bool e)
{
  unsigned int i; // [esp+4h] [ebp-4h]

  if ( !this->isClosed
    && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.h", 128, 0, "%s", "isClosed") )
  {
    __debugbreak();
  }
  for ( i = 0; i < this->numVerts; ++i )
    this->verts[i].isOriginalEdge = e;
}

char __thiscall GlassShard::Create(GlassShard *this, const Glass *glass)
{
  int i; // [esp+30h] [ebp-724h]
  GlassShard::Triangles triangles; // [esp+34h] [ebp-720h] BYREF
  ShardGroup *grp; // [esp+140h] [ebp-614h]
  TempOutline newOutline; // [esp+144h] [ebp-610h] BYREF

  if ( glass->numOutlineVerts < 3u )
    return 0;
  newOutline.verts = newOutline.v;
  newOutline.numVerts = 0;
  newOutline.maxVerts = 64;
  newOutline.isClosed = 0;
  for ( i = 0; i < glass->numOutlineVerts; ++i )
    GlassShard::Outline::Add(&newOutline, glass->outline[i]);
  GlassShard::Outline::CloseOutline(&newOutline);
  GlassShard::Outline::MarkAllEdge(&newOutline, glass->isPlanar);
  GlassShard::Triangles::Triangles(&triangles, &newOutline);
  if ( !GlassShard::Triangles::Triangulate(&triangles) )
    return 0;
  if ( !GlassShard::AllocateMemory(this, newOutline.numVerts, &triangles) )
    return 0;
  GlassShard::Outline::operator=(&this->outline, &newOutline);
  this->thickness = glass->thickness;
  this->uvScale = glass->uvScale;
  this->glassIndex = glass->index;
  this->axis[0][0] = glass->outlineAxis[0][0];
  this->axis[0][1] = glass->outlineAxis[0][1];
  this->axis[0][2] = glass->outlineAxis[0][2];
  this->axis[1][0] = glass->outlineAxis[1][0];
  this->axis[1][1] = glass->outlineAxis[1][1];
  this->axis[1][2] = glass->outlineAxis[1][2];
  this->axis[2][0] = glass->outlineAxis[2][0];
  this->axis[2][1] = glass->outlineAxis[2][1];
  this->axis[2][2] = glass->outlineAxis[2][2];
  this->origin[0] = glass->outlineOrigin[0];
  this->origin[1] = glass->outlineOrigin[1];
  this->origin[2] = glass->outlineOrigin[2];
  this->isOriginalEdge = glass->isPlanar;
  grp = (ShardGroup *)GlassRenderer::GetShardGroup(clGlasses->renderer, this->origin, glass->glassDef);
  if ( !grp )
    return 0;
  GlassShard::UpdateBBox(this);
  ShardGroup::Add(grp, this);
  return 1;
}

bool __thiscall GlassShard::CanSplit(GlassShard *this, float maxShardSize, float minShardSize)
{
  float v4; // [esp+4h] [ebp-10h]

  if ( (float)((float)(minShardSize * 2.5) - maxShardSize) < 0.0 )
    v4 = minShardSize * 2.5;
  else
    v4 = maxShardSize;
  return GlassShard::Outline::Area(&this->outline) > v4;
}

bool __thiscall GlassShard::CanSplit(GlassShard *this, bool shatter)
{
  float value; // [esp+8h] [ebp-14h]

  if ( !this->group
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 1252, 0, "%s", "group") )
  {
    __debugbreak();
  }
  if ( shatter )
    value = FLOAT_1_0;
  else
    value = clGlasses->renderer->shardShatterSizeLimitScale->current.value;
  return GlassShard::CanSplit(this, this->group->glassDef->maxShardSize, this->group->glassDef->minShardSize * value);
}

int __thiscall GlassShard::Shatter(GlassShard *this, GlassShard **newShards, int maxNewShards)
{
  double v3; // st7
  signed int v5; // [esp+2Ch] [ebp-A0h]
  float minShardSize; // [esp+30h] [ebp-9Ch]
  float maxShardSize; // [esp+34h] [ebp-98h]
  float v9; // [esp+54h] [ebp-78h]
  signed int j; // [esp+8Ch] [ebp-40h]
  int numNewVerts; // [esp+90h] [ebp-3Ch]
  int i; // [esp+94h] [ebp-38h]
  int ns; // [esp+98h] [ebp-34h]
  float glassSize; // [esp+9Ch] [ebp-30h]
  int numTries; // [esp+A0h] [ebp-2Ch]
  int maxSplits; // [esp+A4h] [ebp-28h]
  signed int numNewShards; // [esp+ACh] [ebp-20h]
  _LARGE_INTEGER timer; // [esp+B4h] [ebp-18h]
  unsigned __int64 *timer_8; // [esp+BCh] [ebp-10h]
  int maxSplitTries; // [esp+C8h] [ebp-4h]

  PIXBeginNamedEvent(-1, "GlassShard.Shatter");
  timer_8 = &clGlasses->renderer->shatterTimer;
  timer = tlPcGetTick();
  ++clGlasses->renderer->numShatters;
  if ( !this->group
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 1266, 0, "%s", "group") )
  {
    __debugbreak();
  }
  glassSize = GlassShard::Outline::Area(&this->outline);
  if ( (float)((float)(glassSize / 10.0) - this->group->glassDef->maxShardSize) < 0.0 )
    maxShardSize = glassSize / 10.0;
  else
    maxShardSize = this->group->glassDef->maxShardSize;
  if ( (float)((float)(maxShardSize / 5.0) - this->group->glassDef->minShardSize) < 0.0 )
    minShardSize = maxShardSize / 5.0;
  else
    minShardSize = this->group->glassDef->minShardSize;
  *newShards = this;
  numNewShards = 1;
  maxSplits = clGlasses->renderer->maxShardSplit->current.integer;
  if ( this->group->glassDef->maxShards > 0 )
    maxSplits = this->group->glassDef->maxShards;
  if ( maxNewShards - 1 < maxSplits )
    v5 = maxNewShards - 1;
  else
    v5 = maxSplits;
  maxSplitTries = 3 * v5;
  numTries = 0;
  GlassShard::splitFailCount[0] = 0;
  qword_A707074 = 0;
  qword_A70707C = 0;
  dword_A707084 = 0;
  dword_A707088 = 0;
  dword_A70708C = 0;
  PIXBeginNamedEvent(0, "Split");
  while ( numNewShards < v5 && GlassShard::CanSplit(*newShards, maxShardSize, minShardSize) && maxSplitTries > 0 )
  {
    ns = 0;
    for ( i = 0; i < numNewShards && !ns && GlassShard::CanSplit(newShards[i], maxShardSize, minShardSize); ++i )
    {
      ++numTries;
      ns = GlassShard::Split(newShards[i], &newShards[numNewShards], minShardSize, 0, -1.0);
      --maxSplitTries;
    }
    if ( ns )
    {
      numNewShards += ns;
      qsort(newShards, numNewShards, 4u, (int (__cdecl *)(const void *, const void *))compareShards);
      if ( maxShardSize > GlassShard::Outline::Area(&(*newShards)->outline) )
        break;
    }
  }
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  if ( clGlasses->renderer->debugSplit->current.enabled )
  {
    numNewVerts = 0;
    for ( j = 0; j < numNewShards; ++j )
      numNewVerts += newShards[j]->outline.numVerts;
    v9 = GlassShard::Outline::Area(&newShards[numNewShards - 1]->outline);
    v3 = GlassShard::Outline::Area(&(*newShards)->outline);
    Com_Printf(
      0,
      "Shatter: original size=%.1f num tries=%d new shards=%d largest=%.1f smallest=%.1f numVerts=%d(%.1f)\n",
      glassSize,
      numTries,
      numNewShards,
      v3,
      v9,
      numNewVerts,
      (float)((float)numNewVerts / (float)numNewShards));
    Com_Printf(
      0,
      "         fail: sPos=%d sDir=%d init=%d verts=%d area=%d angle=%d narrow=%d mem=%d\n",
      GlassShard::splitFailCount[0],
      (unsigned int)qword_A707074,
      HIDWORD(qword_A707074),
      (unsigned int)qword_A70707C,
      HIDWORD(qword_A70707C),
      dword_A707088,
      dword_A70708C,
      dword_A707084);
  }
  *timer_8 += *(_QWORD *)&tlPcGetTick() - timer.QuadPart;
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  return numNewShards;
}

int __cdecl compareShards(const void *s1, const GlassShard **s2)
{
  float v4; // [esp+4h] [ebp-10h]
  float v5; // [esp+8h] [ebp-Ch]
  const GlassShard *shard2; // [esp+10h] [ebp-4h]

  shard2 = *s2;
  v5 = GlassShard::Outline::Area((GlassShard::Outline *)(*(unsigned int *)s1 + 12));
  v4 = GlassShard::Outline::Area(&shard2->outline);
  if ( (float)(v5 - v4) < 0.0 )
    return (int)FLOAT_1_0;
  else
    return (int)FLOAT_N1_0;
}

void __thiscall GlassShard::InitPhysics(
        GlassShard *this,
        GlassShard **newShards,
        int numNewShards,
        float glassExtent,
        const float *pos,
        const float *dir)
{
  ShardGroup *group; // esi
  float v7; // [esp+Ch] [ebp-30h]
  int j; // [esp+2Ch] [ebp-10h]
  int i; // [esp+30h] [ebp-Ch]
  int numKeepShards; // [esp+38h] [ebp-4h]

  if ( !this->group )
  {
    for ( i = 0; i < numNewShards; ++i )
      GlassShard::Remove(newShards[i], (GlassShard::RemoveReason)8, 0);
    return;
  }
  if ( (float)((float)((float)(*dir * *dir) + (float)(dir[1] * dir[1])) + (float)(dir[2] * dir[2])) == 0.0 )
    v7 = FLOAT_0_5;
  else
    v7 = FLOAT_1_0;
  numKeepShards = (int)(float)((float)((float)((float)numNewShards * this->group->glassDef->shardLifeProbablility) * v7)
                             + 0.5);
  PIXBeginNamedEvent(0, "InitPhysics");
  for ( j = 0; j < numNewShards; ++j )
  {
    GlassShard::InitMesh(newShards[j]);
    if ( clGlasses->renderer->freezeShards->current.enabled )
    {
      GlassShard::UpdateBBox(newShards[j]);
    }
    else if ( !GlassShard::InitPhysics(newShards[j], j < numKeepShards, pos, dir, glassExtent, 1.0) )
    {
      continue;
    }
    if ( !newShards[j]->group )
    {
      if ( !this->group )
      {
        GlassShard::Remove(newShards[j], (GlassShard::RemoveReason)8, 0);
        continue;
      }
      ShardGroup::Add(this->group, newShards[j]);
    }
    group = newShards[j]->group;
    if ( group->packedPos != GlassRenderer::CalcPackedPos(clGlasses->renderer, newShards[j]->origin) )
      GlassRenderer::AddGroupChange(clGlasses->renderer, newShards[j]);
  }
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
}

void __thiscall GlassShard::InitMesh(GlassShard *this)
{
  GlassShard::Recenter(this);
  GlassShard::Mesh::Init(&this->mesh, &this->outline, this->thickness, this->axis[2]);
}

char __thiscall GlassShard::AllocateMemory(
        GlassShard *this,
        unsigned int nHull,
        const GlassShard::Triangles *triangles)
{
  __int16 MemorySize; // [esp+0h] [ebp-10h]
  unsigned __int8 *ptr; // [esp+Ch] [ebp-4h]

  GlassShard::FreeMemory(this);
  if ( triangles )
    MemorySize = GlassShard::Mesh::GetMemorySize(nHull);
  else
    MemorySize = 0;
  this->memorySize = MemorySize + 24 * nHull;
  this->memoryPtr = (unsigned __int8 *)GlassRenderer::AllocateShardMemory(clGlasses->renderer, this->memorySize, this);
  if ( !this->memoryPtr )
    return 0;
  ptr = this->memoryPtr;
  if ( nHull )
  {
    if ( nHull >= 0x100
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 1667, 0, "%s", "nHull < 256") )
    {
      __debugbreak();
    }
    ptr += GlassShard::Outline::Init(&this->outline, (GlassShard::Outline::Vertex *)ptr, nHull);
  }
  if ( triangles )
  {
    if ( triangles->nIndices != 3 * (nHull - 2)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp",
            1672,
            0,
            "%s",
            "triangles->nIndices == (nHull-2)*3") )
    {
      __debugbreak();
    }
    GlassShard::Mesh::SetTriangles(&this->mesh, ptr, triangles->triangleIndices, triangles->nIndices);
  }
  return 1;
}

void __thiscall GlassShard::FreeMemory(GlassShard *this)
{
  if ( this->memoryPtr )
  {
    GlassShard::lastFreeMemorySize = GlassRenderer::ShardMemorySize(clGlasses->renderer, (unsigned int *)this->memoryPtr);
    GlassRenderer::FreeShardMemory(clGlasses->renderer, (unsigned int *)this->memoryPtr);
    this->memoryPtr = 0;
    this->outline.verts = 0;
    this->outline.numVerts = 0;
    this->outline.maxVerts = 0;
    this->outline.isClosed = 0;
    GlassShard::Mesh::Clear(&this->mesh);
  }
}

void __thiscall GlassShard::Defrag(GlassShard *this)
{
  int memorySize; // esi
  int v2; // edi
  int v3; // eax
  int v4; // eax
  unsigned __int8 backupBuffer[1024]; // [esp+1Ch] [ebp-408h] BYREF
  unsigned __int8 *ptr; // [esp+420h] [ebp-4h]

  if ( this->memorySize >= 0x400u
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp",
          1698,
          0,
          "%s",
          "memorySize < ARRAY_COUNT( backupBuffer )") )
  {
    __debugbreak();
  }
  if ( !this->mesh.normArray
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 1699, 0, "%s", "mesh.normArray") )
  {
    __debugbreak();
  }
  if ( !this->mesh.indices
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 1700, 0, "%s", "mesh.indices") )
  {
    __debugbreak();
  }
  memcpy(backupBuffer, this->memoryPtr, this->memorySize);
  GlassRenderer::FreeShardMemory(clGlasses->renderer, (unsigned int *)this->memoryPtr);
  memorySize = this->memorySize;
  v2 = 24 * this->outline.numVerts;
  if ( memorySize != GlassShard::Mesh::GetMemorySize(this->outline.numVerts) + v2
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp",
          1709,
          0,
          "%s",
          "memorySize == outline.NumVerts()*sizeof(Outline::Vertex) + Mesh::GetMemorySize(outline.NumVerts())") )
  {
    __debugbreak();
  }
  this->memoryPtr = (unsigned __int8 *)GlassRenderer::AllocateShardMemory(clGlasses->renderer, this->memorySize, this);
  if ( this->memoryPtr )
  {
    memcpy(this->memoryPtr, backupBuffer, this->memorySize);
    ptr = this->memoryPtr;
    v3 = GlassShard::Outline::SetPointers(&this->outline, ptr);
    ptr += v3;
    v4 = GlassShard::Mesh::SetPointers(&this->mesh, (PackedUnitVec *)ptr);
    ptr += v4;
    if ( (unsigned __int8 *)(ptr - this->memoryPtr) != (unsigned __int8 *)this->memorySize
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp",
            1726,
            0,
            "%s",
            "ptr - memoryPtr == int(memorySize)") )
    {
      __debugbreak();
    }
  }
  else
  {
    if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 1713, 0, "%s", "0") )
      __debugbreak();
    GlassRenderer::FreeShard(clGlasses->renderer, this);
  }
}

void __thiscall GlassShard::ToWorldPos(GlassShard *this, float *pLocal, float *pWorld, bool is3D)
{
  float v4; // [esp+8h] [ebp-18h]
  float v5; // [esp+10h] [ebp-10h]
  float v6; // [esp+18h] [ebp-8h]

  v6 = *pLocal;
  *pWorld = (float)(*pLocal * this->axis[0][0]) + this->origin[0];
  pWorld[1] = (float)(v6 * this->axis[0][1]) + this->origin[1];
  pWorld[2] = (float)(v6 * this->axis[0][2]) + this->origin[2];
  v5 = pLocal[1];
  *pWorld = (float)(v5 * this->axis[1][0]) + *pWorld;
  pWorld[1] = (float)(v5 * this->axis[1][1]) + pWorld[1];
  pWorld[2] = (float)(v5 * this->axis[1][2]) + pWorld[2];
  if ( is3D )
  {
    v4 = pLocal[2];
    *pWorld = (float)(v4 * this->axis[2][0]) + *pWorld;
    pWorld[1] = (float)(v4 * this->axis[2][1]) + pWorld[1];
    pWorld[2] = (float)(v4 * this->axis[2][2]) + pWorld[2];
  }
}

void __thiscall GlassShard::ToWorldDir(GlassShard *this, float *dLocal, float *dWorld, bool is3D)
{
  float v4; // [esp+8h] [ebp-14h]
  float v5; // [esp+10h] [ebp-Ch]
  float v6; // [esp+18h] [ebp-4h]

  v6 = *dLocal;
  *dWorld = (float)(*dLocal * this->axis[0][0]) + 0.0;
  dWorld[1] = (float)(v6 * this->axis[0][1]) + 0.0;
  dWorld[2] = (float)(v6 * this->axis[0][2]) + 0.0;
  v5 = dLocal[1];
  *dWorld = (float)(v5 * this->axis[1][0]) + *dWorld;
  dWorld[1] = (float)(v5 * this->axis[1][1]) + dWorld[1];
  dWorld[2] = (float)(v5 * this->axis[1][2]) + dWorld[2];
  if ( is3D )
  {
    v4 = dLocal[2];
    *dWorld = (float)(v4 * this->axis[2][0]) + *dWorld;
    dWorld[1] = (float)(v4 * this->axis[2][1]) + dWorld[1];
    dWorld[2] = (float)(v4 * this->axis[2][2]) + dWorld[2];
  }
}

void __thiscall GlassShard::ToLocal(GlassShard *this, float *pos, float *dir, float *localPos, float *localDir)
{
  float v5; // [esp+8h] [ebp-58h]
  float v6; // [esp+10h] [ebp-50h]
  float v7; // [esp+14h] [ebp-4Ch]
  float v8; // [esp+20h] [ebp-40h]
  float v9; // [esp+28h] [ebp-38h]
  float v10; // [esp+2Ch] [ebp-34h]
  float invXform[4][3]; // [esp+30h] [ebp-30h] BYREF

  MatrixInverseOrthogonal43(this->axis, invXform);
  if ( localDir )
  {
    v10 = *dir;
    *localDir = (float)(*dir * invXform[0][0]) + 0.0;
    localDir[1] = (float)(v10 * invXform[0][1]) + 0.0;
    localDir[2] = (float)(v10 * invXform[0][2]) + 0.0;
    v9 = dir[1];
    *localDir = (float)(v9 * invXform[1][0]) + *localDir;
    localDir[1] = (float)(v9 * invXform[1][1]) + localDir[1];
    localDir[2] = (float)(v9 * invXform[1][2]) + localDir[2];
    v8 = dir[2];
    *localDir = (float)(v8 * invXform[2][0]) + *localDir;
    localDir[1] = (float)(v8 * invXform[2][1]) + localDir[1];
    localDir[2] = (float)(v8 * invXform[2][2]) + localDir[2];
  }
  if ( localPos )
  {
    v7 = *pos;
    *localPos = (float)(*pos * invXform[0][0]) + invXform[3][0];
    localPos[1] = (float)(v7 * invXform[0][1]) + invXform[3][1];
    localPos[2] = (float)(v7 * invXform[0][2]) + invXform[3][2];
    v6 = pos[1];
    *localPos = (float)(v6 * invXform[1][0]) + *localPos;
    localPos[1] = (float)(v6 * invXform[1][1]) + localPos[1];
    localPos[2] = (float)(v6 * invXform[1][2]) + localPos[2];
    v5 = pos[2];
    *localPos = (float)(v5 * invXform[2][0]) + *localPos;
    localPos[1] = (float)(v5 * invXform[2][1]) + localPos[1];
    localPos[2] = (float)(v5 * invXform[2][2]) + localPos[2];
  }
}

bool __thiscall GlassShard::Intersect(GlassShard *this, float *pos, float *dir, float *hitPoint)
{
  float testDir[2]; // [esp+Ch] [ebp-3Ch] BYREF
  float d; // [esp+14h] [ebp-34h] BYREF
  int edge; // [esp+18h] [ebp-30h] BYREF
  float localHit[3]; // [esp+20h] [ebp-28h]
  float t; // [esp+2Ch] [ebp-1Ch]
  float localPos[3]; // [esp+30h] [ebp-18h] BYREF
  float localDir[3]; // [esp+3Ch] [ebp-Ch] BYREF

  GlassShard::ToLocal(this, pos, dir, localPos, localDir);
  if ( COERCE_FLOAT(LODWORD(localDir[2]) & _mask__AbsFloat_) <= 0.0000099999997 )
    return 0;
  t = COERCE_FLOAT(LODWORD(localPos[2]) ^ _mask__NegFloat_) / localDir[2];
  if ( t < 0.0 )
    return 0;
  localHit[0] = (float)(t * localDir[0]) + localPos[0];
  localHit[1] = (float)(t * localDir[1]) + localPos[1];
  localHit[2] = (float)(t * localDir[2]) + localPos[2];
  *hitPoint = localHit[0];
  hitPoint[1] = localHit[1];
  testDir[0] = FLOAT_1_0;
  testDir[1] = *(float *)&FLOAT_0_0;
  return GlassShard::Outline::GetNumIntersections(&this->outline, hitPoint, testDir, &d, &edge) % 2 == 1;
}

void __thiscall GlassShard::ExplosionEvent(
        GlassShard *this,
        const float *expOrigin,
        float damageInner,
        float damageOuter,
        float radius,
        int mod)
{
  float v7; // [esp+4h] [ebp-4Ch]
  float force[3]; // [esp+2Ch] [ebp-24h] BYREF
  float damage; // [esp+38h] [ebp-18h]
  float dir[3]; // [esp+3Ch] [ebp-14h] BYREF
  float dist; // [esp+48h] [ebp-8h]
  float distSq; // [esp+4Ch] [ebp-4h]

  distSq = Vec3DistanceSq(expOrigin, this->origin);
  if ( (float)(radius * radius) > distSq )
  {
    dist = fsqrt(distSq);
    damage = (float)((float)(damageInner - damageOuter) * (float)(1.0 - (float)(dist / radius))) + damageOuter;
    dir[0] = this->origin[0] - *expOrigin;
    dir[1] = this->origin[1] - expOrigin[1];
    dir[2] = this->origin[2] - expOrigin[2];
    Vec3Normalize(dir);
    if ( GlassShard::InitPhysicsObj(this, 1) )
    {
      v7 = (float)(damage * 0.0099999998) * clGlasses->renderer->forceMultiplier->current.value;
      force[0] = v7 * dir[0];
      force[1] = v7 * dir[1];
      force[2] = v7 * dir[2];
      GlassShard::AddForce(this, this->origin, force);
    }
    if ( this->isOriginalEdge )
      GlassShard::UnEdge(this);
  }
}

char __thiscall GlassShard::TracePoint(
        GlassShard *this,
        float *p0,
        const float *p1,
        float *dir,
        float length,
        const float *mins,
        float *maxs)
{
  double v7; // st7
  double v8; // st7
  ShardGroup *group; // esi
  float v11; // [esp+14h] [ebp-21Ch]
  float v12; // [esp+18h] [ebp-218h]
  float v16; // [esp+40h] [ebp-1F0h]
  float v17; // [esp+58h] [ebp-1D8h]
  float v18; // [esp+64h] [ebp-1CCh]
  float v19; // [esp+70h] [ebp-1C0h]
  char v20; // [esp+8Fh] [ebp-1A1h]
  const float *a; // [esp+90h] [ebp-1A0h]
  float *b; // [esp+94h] [ebp-19Ch]
  float v23[3]; // [esp+98h] [ebp-198h] BYREF
  float v24[3]; // [esp+A4h] [ebp-18Ch] BYREF
  float v25[3]; // [esp+B0h] [ebp-180h] BYREF
  float v26[3]; // [esp+BCh] [ebp-174h] BYREF
  float v27[3]; // [esp+C8h] [ebp-168h] BYREF
  float v28[3]; // [esp+D4h] [ebp-15Ch] BYREF
  float v29[3]; // [esp+E0h] [ebp-150h] BYREF
  float v30[3]; // [esp+ECh] [ebp-144h] BYREF
  float v31[3]; // [esp+F8h] [ebp-138h] BYREF
  float product[3]; // [esp+104h] [ebp-12Ch] BYREF
  float v33[3]; // [esp+110h] [ebp-120h] BYREF
  float res[3]; // [esp+11Ch] [ebp-114h] BYREF
  float v[3]; // [esp+128h] [ebp-108h] BYREF
  float diff[3]; // [esp+134h] [ebp-FCh] BYREF
  float sum[3]; // [esp+140h] [ebp-F0h] BYREF
  float v38[3]; // [esp+14Ch] [ebp-E4h] BYREF
  float v39[3]; // [esp+158h] [ebp-D8h] BYREF
  float v40[3]; // [esp+164h] [ebp-CCh] BYREF
  float result[3]; // [esp+170h] [ebp-C0h] BYREF
  float *worldBBoxMax; // [esp+17Ch] [ebp-B4h]
  float *worldBBoxMin; // [esp+180h] [ebp-B0h]
  int k; // [esp+184h] [ebp-ACh]
  float pos2d[3]; // [esp+188h] [ebp-A8h] BYREF
  int v46; // [esp+194h] [ebp-9Ch]
  int v47; // [esp+198h] [ebp-98h]
  int maxSplits; // [esp+19Ch] [ebp-94h]
  float SHARD_CHIP_RATIO; // [esp+1A0h] [ebp-90h]
  GlassShard *fixedShard; // [esp+1A4h] [ebp-8Ch]
  int maxSplitTries; // [esp+1ACh] [ebp-84h]
  int j; // [esp+1B0h] [ebp-80h]
  int i; // [esp+1B4h] [ebp-7Ch]
  int NUM_TRIES; // [esp+1B8h] [ebp-78h]
  int ns; // [esp+1C0h] [ebp-70h]
  GlassShard::Outline::EdgeDistance dists[1]; // [esp+1C8h] [ebp-68h] BYREF
  float maxShardSize; // [esp+1DCh] [ebp-54h]
  int numNewShards; // [esp+1E0h] [ebp-50h]
  GlassShard *newShards[10]; // [esp+1E4h] [ebp-4Ch] BYREF
  float minShardSize; // [esp+20Ch] [ebp-24h]
  float EDGE_DIST_RATIO; // [esp+210h] [ebp-20h]
  float hitPoint3d[3]; // [esp+214h] [ebp-1Ch] BYREF
  float glassExtent; // [esp+220h] [ebp-10h]
  float STICKINESS; // [esp+224h] [ebp-Ch]
  float hitPoint2d[2]; // [esp+228h] [ebp-8h] BYREF

  worldBBoxMax = this->worldBBoxMax;
  worldBBoxMin = this->worldBBoxMin;
  if ( this->worldBBoxMax[0] < *mins
    || worldBBoxMax[1] < mins[1]
    || worldBBoxMax[2] < mins[2]
    || *maxs < *worldBBoxMin
    || maxs[1] < worldBBoxMin[1]
    || maxs[2] < worldBBoxMin[2] )
  {
    return 0;
  }
  a = this->worldBBoxMax;
  b = this->worldBBoxMin;
  Vec3Min(p0, p1, result);
  Vec3Max(p0, p1, v40);
  if ( this->worldBBoxMax[0] < result[0]
    || this->worldBBoxMax[1] < result[1]
    || this->worldBBoxMax[2] < result[2]
    || v40[0] < *b
    || v40[1] < this->worldBBoxMin[1]
    || v40[2] < this->worldBBoxMin[2] )
  {
    goto LABEL_23;
  }
  Vec3Sub(p1, p0, diff);
  Vec3Avg(a, b, sum);
  Vec3Sub(a, b, v);
  Vec3Scale(v, 0.5, v);
  Vec3Sub(p0, sum, v39);
  Vec3Abs(diff, res);
  v25[0] = res[1];
  v25[1] = res[2];
  v25[2] = res[0];
  Vec3Mul(v, v25, product);
  v23[0] = v[1];
  v23[1] = v[2];
  v23[2] = v[0];
  Vec3Mul(v23, res, v27);
  v30[0] = diff[1];
  v30[1] = diff[2];
  v30[2] = diff[0];
  Vec3Mul(v39, v30, v26);
  v28[0] = v39[1];
  v28[1] = v39[2];
  v28[2] = v39[0];
  Vec3Mul(v28, diff, v33);
  Vec3Sub(v26, v33, v31);
  Vec3Add(product, v27, v29);
  Vec3Abs(v31, v24);
  Vec3Sub(v29, v24, v38);
  if ( v38[0] <= 0.0 )
    goto LABEL_23;
  if ( v38[1] > 0.0 && v38[2] > 0.0 )
    v20 = 1;
  else
LABEL_23:
    v20 = 0;
  if ( !v20 || !GlassShard::Intersect(this, p0, dir, hitPoint2d) )
    return 0;
  GlassShard::ToWorldPos(this, hitPoint2d, hitPoint3d, 0);
  glassExtent = GlassShard::Outline::Extent(&this->outline);
  newShards[0] = this;
  memset(&newShards[1], 0, 36);
  numNewShards = 1;
  minShardSize = this->group->glassDef->minShardSize * clGlasses->renderer->shardShatterSizeLimitScale->current.value;
  maxShardSize = this->group->glassDef->maxShardSize;
  GlassShard::splitFailCount[0] = 0;
  qword_A707074 = 0;
  qword_A70707C = 0;
  dword_A707084 = 0;
  dword_A707088 = 0;
  dword_A70708C = 0;
  GlassShard::Outline::GetNearestDistances(&this->outline, hitPoint2d, dists, 1);
  EDGE_DIST_RATIO = FLOAT_0_02;
  v7 = GlassShard::Outline::Length(&this->outline);
  if ( dists[0].dist <= v7 * EDGE_DIST_RATIO )
  {
    PIXBeginNamedEvent(-1, "Split 1");
    hitPoint2d[0] = dists[0].closestPoint[0];
    hitPoint2d[1] = dists[0].closestPoint[1];
    GlassShard::ToWorldPos(this, hitPoint2d, hitPoint3d, 0);
    NUM_TRIES = 5;
    ns = 0;
    for ( i = 0; i < 5 && !ns && GlassShard::CanSplit(newShards[0], maxShardSize, minShardSize); ++i )
      ns = GlassShard::Split(
             newShards[0],
             &newShards[numNewShards],
             minShardSize,
             dists[0].edgeIndex,
             dists[0].edgeParam);
    numNewShards += ns;
    if ( g_DXDeviceThread != GetCurrentThreadId() )
      goto LABEL_38;
  }
  else
  {
    PIXBeginNamedEvent(-1, "Chip");
    if ( GlassShard::Chip(this, dists, hitPoint2d, &newShards[numNewShards], minShardSize) )
      ++numNewShards;
    if ( g_DXDeviceThread != GetCurrentThreadId() )
      goto LABEL_38;
  }
  D3DPERF_EndEvent();
LABEL_38:
  for ( j = 0; j < numNewShards; ++j )
  {
    if ( GlassShard::EdgeRatio(newShards[j]) < 0.15000001 )
      GlassShard::UnEdge(newShards[j]);
  }
  STICKINESS = FLOAT_1_0;
  if ( numNewShards == 1 )
  {
    if ( newShards[0]->isOriginalEdge && !GlassShard::LeaveOnEdge(newShards[0], STICKINESS) )
      GlassShard::UnEdge(newShards[0]);
  }
  else
  {
    if ( numNewShards != 2
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp",
            1912,
            0,
            "%s",
            "numNewShards == 2") )
    {
      __debugbreak();
    }
    fixedShard = 0;
    if ( newShards[0]->isOriginalEdge && newShards[1]->isOriginalEdge )
    {
      v19 = GlassShard::Outline::Area(&newShards[0]->outline);
      if ( GlassShard::Outline::Area(&newShards[1]->outline) <= v19 )
      {
        GlassShard::UnEdge(newShards[1]);
        fixedShard = newShards[0];
      }
      else
      {
        GlassShard::UnEdge(newShards[0]);
        fixedShard = newShards[1];
      }
    }
    else if ( newShards[0]->isOriginalEdge )
    {
      if ( GlassShard::LeaveOnEdge(newShards[0], STICKINESS) )
        fixedShard = newShards[0];
      else
        GlassShard::UnEdge(newShards[0]);
    }
    else if ( newShards[1]->isOriginalEdge )
    {
      if ( GlassShard::LeaveOnEdge(newShards[1], STICKINESS) )
        fixedShard = newShards[1];
      else
        GlassShard::UnEdge(newShards[1]);
    }
    SHARD_CHIP_RATIO = FLOAT_3_0;
    v18 = GlassShard::Outline::Area(&newShards[0]->outline);
    v12 = v18 * SHARD_CHIP_RATIO;
    if ( GlassShard::Outline::Area(&newShards[1]->outline) <= v12 )
    {
      v17 = GlassShard::Outline::Area(&newShards[1]->outline);
      v11 = v17 * SHARD_CHIP_RATIO;
      if ( GlassShard::Outline::Area(&newShards[0]->outline) > v11 )
        fixedShard = newShards[0];
    }
    else
    {
      fixedShard = newShards[1];
    }
    PIXBeginNamedEvent(-1, "Split 2");
    maxSplits = 10;
    maxSplitTries = 30;
    while ( numNewShards < maxSplits
         && GlassShard::CanSplit(newShards[0], maxShardSize, minShardSize)
         && maxSplitTries > 0 )
    {
      v47 = 0;
      v46 = 0;
      while ( v46 < numNewShards && !v47 && GlassShard::CanSplit(newShards[v46], maxShardSize, minShardSize) )
      {
        if ( newShards[v46] != fixedShard )
        {
          GlassShard::ToLocal(newShards[v46], hitPoint3d, 0, pos2d, 0);
          GlassShard::Outline::GetNearestDistances(&newShards[v46]->outline, pos2d, dists, 1);
          v47 = GlassShard::Split(
                  newShards[v46],
                  &newShards[numNewShards],
                  minShardSize,
                  dists[0].edgeIndex,
                  dists[0].edgeParam);
        }
        ++v46;
        --maxSplitTries;
      }
      if ( v47 )
      {
        numNewShards += v47;
        qsort(newShards, numNewShards, 4u, (int (__cdecl *)(const void *, const void *))compareShards);
      }
    }
    if ( GetCurrentThreadId() == g_DXDeviceThread )
      D3DPERF_EndEvent();
  }
  if ( clGlasses->renderer->debugSplit->current.enabled )
  {
    v16 = GlassShard::Outline::Area(&newShards[numNewShards - 1]->outline);
    v8 = GlassShard::Outline::Area(&newShards[0]->outline);
    Com_Printf(0, "TracePoint: new shards=%d largest=%.1f smallest=%.1f\n", numNewShards, v8, v16);
    Com_Printf(
      0,
      "         fail: sPos=%d sDir=%d init=%d verts=%d area=%d angle=%d narrow=%d mem=%d\n",
      GlassShard::splitFailCount[0],
      (unsigned int)qword_A707074,
      HIDWORD(qword_A707074),
      (unsigned int)qword_A70707C,
      HIDWORD(qword_A70707C),
      dword_A707088,
      dword_A70708C,
      dword_A707084);
  }
  PIXBeginNamedEvent(0, "InitPhysics");
  for ( k = 0; k < numNewShards; ++k )
  {
    GlassShard::InitMesh(newShards[k]);
    if ( newShards[k]->isOriginalEdge || clGlasses->renderer->freezeShards->current.enabled )
    {
      GlassShard::UpdateBBox(newShards[k]);
    }
    else if ( !GlassShard::InitPhysics(newShards[k], 1, hitPoint3d, dir, glassExtent, 1.0) )
    {
      continue;
    }
    if ( !newShards[k]->group )
      ShardGroup::Add(this->group, newShards[k]);
    group = newShards[k]->group;
    if ( group->packedPos != GlassRenderer::CalcPackedPos(clGlasses->renderer, newShards[k]->origin) )
      GlassRenderer::AddGroupChange(clGlasses->renderer, newShards[k]);
  }
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
  return 1;
}

void __cdecl Vec3Avg(const float *a, const float *b, float *sum)
{
  *sum = (float)(*a + *b) * 0.5;
  sum[1] = (float)(a[1] + b[1]) * 0.5;
  sum[2] = (float)(a[2] + b[2]) * 0.5;
}

void __cdecl Vec3Mul(const float *a, const float *b, float *product)
{
  *product = *a * *b;
  product[1] = a[1] * b[1];
  product[2] = a[2] * b[2];
}

void __thiscall GlassShard::GetLocalBBox(GlassShard *this, float *mins, float *maxs)
{
  float v4; // [esp+20h] [ebp-34h]
  float v5; // [esp+30h] [ebp-24h]
  float v6; // [esp+3Ch] [ebp-18h]
  float pos[3]; // [esp+44h] [ebp-10h] BYREF
  unsigned int i; // [esp+50h] [ebp-4h]

  v6 = this->outline.verts->edge.origin[1];
  *mins = this->outline.verts->edge.origin[0];
  mins[1] = v6;
  mins[2] = *(float *)&FLOAT_0_0;
  v5 = this->outline.verts->edge.origin[1];
  *maxs = this->outline.verts->edge.origin[0];
  maxs[1] = v5;
  maxs[2] = *(float *)&FLOAT_0_0;
  for ( i = 1; i < this->outline.numVerts; ++i )
  {
    v4 = this->outline.verts[i].edge.origin[1];
    pos[0] = this->outline.verts[i].edge.origin[0];
    pos[1] = v4;
    pos[2] = *(float *)&FLOAT_0_0;
    Vec3Min(pos, mins, mins);
    Vec3Max(pos, maxs, maxs);
  }
  if ( this->thickness <= 0.0 )
    mins[2] = this->thickness;
  else
    maxs[2] = this->thickness;
  if ( !mins && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 2159, 0, "%s", "mins") )
    __debugbreak();
  if ( !maxs && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 2159, 0, "%s", "maxs") )
    __debugbreak();
  if ( *maxs < *mins
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp",
          2159,
          0,
          "%s",
          "maxs[0] >= mins[0]") )
  {
    __debugbreak();
  }
  if ( maxs[1] < mins[1]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp",
          2159,
          0,
          "%s",
          "maxs[1] >= mins[1]") )
  {
    __debugbreak();
  }
  if ( maxs[2] < mins[2]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp",
          2159,
          0,
          "%s",
          "maxs[2] >= mins[2]") )
  {
    __debugbreak();
  }
  if ( ((*(unsigned int *)mins & 0x7F800000) == 0x7F800000
     || ((unsigned int)mins[1] & 0x7F800000) == 0x7F800000
     || ((unsigned int)mins[2] & 0x7F800000) == 0x7F800000)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp",
          2159,
          0,
          "%s",
          "!IS_NAN((mins)[0]) && !IS_NAN((mins)[1]) && !IS_NAN((mins)[2])") )
  {
    __debugbreak();
  }
  if ( ((*(unsigned int *)maxs & 0x7F800000) == 0x7F800000
     || ((unsigned int)maxs[1] & 0x7F800000) == 0x7F800000
     || ((unsigned int)maxs[2] & 0x7F800000) == 0x7F800000)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp",
          2159,
          0,
          "%s",
          "!IS_NAN((maxs)[0]) && !IS_NAN((maxs)[1]) && !IS_NAN((maxs)[2])") )
  {
    __debugbreak();
  }
}

void __thiscall GlassShard::UpdateBBox(GlassShard *this)
{
  float v2; // [esp+8h] [ebp-44h]
  float v3; // [esp+10h] [ebp-3Ch]
  float thickness; // [esp+1Ch] [ebp-30h]
  GlassShard::Outline::Vertex *pos; // [esp+2Ch] [ebp-20h]
  float temp[3]; // [esp+30h] [ebp-1Ch] BYREF
  unsigned int i; // [esp+3Ch] [ebp-10h]
  float offset[3]; // [esp+40h] [ebp-Ch]

  this->worldBBoxMin[0] = FLOAT_3_4028235e38;
  this->worldBBoxMin[1] = FLOAT_3_4028235e38;
  this->worldBBoxMin[2] = FLOAT_3_4028235e38;
  this->worldBBoxMax[0] = FLOAT_N3_4028235e38;
  this->worldBBoxMax[1] = FLOAT_N3_4028235e38;
  this->worldBBoxMax[2] = FLOAT_N3_4028235e38;
  thickness = this->thickness;
  offset[0] = thickness * this->axis[2][0];
  offset[1] = thickness * this->axis[2][1];
  offset[2] = thickness * this->axis[2][2];
  for ( i = 0; i < this->outline.numVerts; ++i )
  {
    pos = &this->outline.verts[i];
    v3 = pos->edge.origin[0];
    temp[0] = (float)(pos->edge.origin[0] * this->axis[0][0]) + this->origin[0];
    temp[1] = (float)(v3 * this->axis[0][1]) + this->origin[1];
    temp[2] = (float)(v3 * this->axis[0][2]) + this->origin[2];
    v2 = pos->edge.origin[1];
    temp[0] = (float)(v2 * this->axis[1][0]) + temp[0];
    temp[1] = (float)(v2 * this->axis[1][1]) + temp[1];
    temp[2] = (float)(v2 * this->axis[1][2]) + temp[2];
    Vec3Min(temp, this->worldBBoxMin, this->worldBBoxMin);
    Vec3Max(temp, this->worldBBoxMax, this->worldBBoxMax);
    temp[0] = temp[0] + offset[0];
    temp[1] = temp[1] + offset[1];
    temp[2] = temp[2] + offset[2];
    Vec3Min(temp, this->worldBBoxMin, this->worldBBoxMin);
    Vec3Max(temp, this->worldBBoxMax, this->worldBBoxMax);
  }
}

void __thiscall GlassShard::Update(GlassShard *this, float deltaTime)
{
  ShardGroup *group; // esi
  bool v3; // [esp+Ch] [ebp-1Ch]
  bool v4; // [esp+14h] [ebp-14h]
  bool isStable; // [esp+26h] [ebp-2h]
  bool hitBottom; // [esp+27h] [ebp-1h]
  int savedregs; // [esp+28h] [ebp+0h] BYREF

  v4 = this->physObjId || this->glassPhysics || this->delayedDrop;
  if ( !v4
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 2191, 0, "%s", "NeedUpdate()") )
  {
    __debugbreak();
  }
  if ( this->delayedDrop )
  {
    this->timeUntilAction = this->timeUntilAction - deltaTime;
    if ( this->timeUntilAction < 0.0 )
      GlassShard::InitPhysics(this, 1, vec3_origin, vec3_origin, 0.0, 0.0);
  }
  else
  {
    isStable = 0;
    hitBottom = 0;
    if ( this->glassPhysics )
    {
      if ( deltaTime > 0.0 )
      {
        GlassPhysics::Step(this->glassPhysics, deltaTime);
        GlassPhysics::GetPosition(this->glassPhysics, this->origin, this->axis);
        this->timeUntilAction = this->timeUntilAction - deltaTime;
        hitBottom = this->timeUntilAction < 0.0;
        isStable = 0;
      }
    }
    else
    {
      Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
      Sys_EnterCriticalSection(CRITSECT_PHYSICS);
      Phys_ObjGetPosition((int)&savedregs, this->physObjId, this->origin, this->axis);
      isStable = rigid_body::is_stable(*(rigid_body **)this->physObjId) != 0;
      Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
      Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
    }
    v3 = hitBottom || (float)(s_world.mins[2] + 5.0) > this->origin[2];
    if ( (isStable || v3) && (GlassShard::DestroyPhysicsObj(this), v3) )
    {
      GlassShard::Remove(this, REMOVE_HIT_BOTTOM, 1);
    }
    else
    {
      GlassShard::UpdateBBox(this);
      group = this->group;
      if ( group->packedPos != GlassRenderer::CalcPackedPos(clGlasses->renderer, this->origin) )
        GlassRenderer::AddGroupChange(clGlasses->renderer, this);
    }
  }
}

void __thiscall GlassShard::ChangeGroup(GlassShard *this)
{
  ShardGroup *grp; // [esp+Ch] [ebp-Ch]
  const GlassDef *glassDef; // [esp+10h] [ebp-8h]
  bool freeShard; // [esp+17h] [ebp-1h]

  freeShard = 1;
  if ( this->group )
  {
    glassDef = this->group->glassDef;
    ShardGroup::Remove(this->group, this);
    if ( !this->remove )
    {
      grp = (ShardGroup *)GlassRenderer::GetShardGroup(clGlasses->renderer, this->origin, glassDef);
      if ( grp )
      {
        ShardGroup::Add(grp, this);
        freeShard = 0;
      }
    }
  }
  this->inGroupChange = 0;
  if ( freeShard )
    GlassRenderer::FreeShard(clGlasses->renderer, this);
}

void __thiscall GlassShard::Remove(GlassShard *this, GlassShard::RemoveReason reason, bool delay)
{
  if ( !FixedSizeAllocator<GlassPhysics *>::IsValidUsedPtr(
          (FixedSizeAllocator<GlassPhysics *> *)clGlasses->renderer->shardsAllocator,
          (GlassPhysics *)this)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp",
          2285,
          0,
          "%s",
          "clGlasses->Renderer()->IsValidUsedShardPtr( this )") )
  {
    __debugbreak();
  }
  if ( delay )
  {
    if ( !this->remove )
    {
      this->remove = 1;
      GlassRenderer::AddGroupChange(clGlasses->renderer, this);
      ++GlassShard::removeReasonsCount[reason];
    }
  }
  else
  {
    ++GlassShard::removeReasonsCount[reason];
    if ( this->group )
      ShardGroup::Remove(this->group, this);
    GlassRenderer::FreeShard(clGlasses->renderer, this);
  }
}

void __thiscall GlassShard::GenerateVerts(
        GlassShard *this,
        bool highLod,
        GfxPackedVertex *baseVerts,
        unsigned __int16 vertsBaseIndex,
        unsigned __int16 *idxOut)
{
  unsigned __int8 numIndices; // [esp+2h] [ebp-154Ah]
  unsigned __int8 numVertsLow; // [esp+3h] [ebp-1549h]
  int numNorm; // [esp+4h] [ebp-1548h]
  __int16 v8; // [esp+8h] [ebp-1544h]
  __int16 v9; // [esp+Ch] [ebp-1540h]
  float *v11; // [esp+14h] [ebp-1538h]
  PackedUnitVec v12; // [esp+24h] [ebp-1528h]
  int v13; // [esp+40h] [ebp-150Ch]
  PackedUnitVec out; // [esp+78h] [ebp-14D4h] BYREF
  float v15; // [esp+7Ch] [ebp-14D0h]
  float v16; // [esp+80h] [ebp-14CCh]
  PackedUnitVec in; // [esp+84h] [ebp-14C8h]
  float v18; // [esp+8Ch] [ebp-14C0h]
  int v19; // [esp+90h] [ebp-14BCh]
  int v20; // [esp+94h] [ebp-14B8h]
  int v21; // [esp+98h] [ebp-14B4h]
  float v22; // [esp+9Ch] [ebp-14B0h]
  float v23; // [esp+A0h] [ebp-14ACh]
  int v24; // [esp+A4h] [ebp-14A8h]
  int v25; // [esp+A8h] [ebp-14A4h]
  int v26; // [esp+ACh] [ebp-14A0h]
  float v27; // [esp+B0h] [ebp-149Ch]
  int v28; // [esp+B4h] [ebp-1498h]
  int v29; // [esp+B8h] [ebp-1494h]
  int v30; // [esp+BCh] [ebp-1490h]
  float *v31; // [esp+C0h] [ebp-148Ch]
  float *v32; // [esp+C4h] [ebp-1488h]
  float v33; // [esp+C8h] [ebp-1484h]
  float *v34; // [esp+CCh] [ebp-1480h]
  float v35; // [esp+D0h] [ebp-147Ch]
  float *origin; // [esp+D4h] [ebp-1478h]
  unsigned __int8 numVerts; // [esp+DBh] [ebp-1471h]
  float thickness; // [esp+DCh] [ebp-1470h]
  float *v39; // [esp+E0h] [ebp-146Ch]
  int v40; // [esp+E4h] [ebp-1468h]
  unsigned int k; // [esp+E8h] [ebp-1464h]
  unsigned int v42; // [esp+ECh] [ebp-1460h]
  float v43; // [esp+F0h] [ebp-145Ch]
  float v44; // [esp+F4h] [ebp-1458h]
  float v45; // [esp+F8h] [ebp-1454h]
  PackedUnitVec v46; // [esp+FCh] [ebp-1450h]
  float v47; // [esp+100h] [ebp-144Ch]
  float v48; // [esp+104h] [ebp-1448h]
  unsigned int j; // [esp+108h] [ebp-1444h]
  GlassShard::Outline::Vertex *v50; // [esp+10Ch] [ebp-1440h]
  unsigned int i; // [esp+110h] [ebp-143Ch]
  unsigned int v52[769]; // [esp+114h] [ebp-1438h] BYREF
  unsigned int *v53; // [esp+D18h] [ebp-834h]
  unsigned int v54[256]; // [esp+D1Ch] [ebp-830h] BYREF
  unsigned __int8 *indices; // [esp+111Ch] [ebp-430h]
  PackedUnitVec v56; // [esp+1120h] [ebp-42Ch]
  unsigned int v57; // [esp+1124h] [ebp-428h]
  float v58; // [esp+1128h] [ebp-424h]
  float v59; // [esp+112Ch] [ebp-420h]
  float v60; // [esp+1130h] [ebp-41Ch]
  unsigned int v61[257]; // [esp+1134h] [ebp-418h]
  int v62; // [esp+153Ch] [ebp-10h]
  unsigned int v63; // [esp+1540h] [ebp-Ch]
  unsigned int v64; // [esp+1544h] [ebp-8h]
  float *v65; // [esp+1548h] [ebp-4h]

  if ( !baseVerts
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 2311, 0, "%s", "baseVerts") )
  {
    __debugbreak();
  }
  v65 = (float *)v52;
  v53 = v54;
  thickness = this->thickness;
  v39 = this->axis[2];
  v58 = thickness * this->axis[2][0];
  v59 = thickness * this->axis[2][1];
  v60 = thickness * this->axis[2][2];
  for ( i = 0; ; ++i )
  {
    numVerts = this->outline.numVerts;
    if ( i >= numVerts )
      break;
    v50 = &this->outline.verts[i];
    v34 = this->axis[0];
    v35 = v50->edge.origin[0];
    origin = this->origin;
    *v65 = (float)(v35 * this->axis[0][0]) + this->origin[0];
    v65[1] = (float)(v35 * v34[1]) + origin[1];
    v65[2] = (float)(v35 * v34[2]) + origin[2];
    v32 = this->axis[1];
    v33 = v50->edge.origin[1];
    *v65 = (float)(v33 * this->axis[1][0]) + *v65;
    v65[1] = (float)(v33 * v32[1]) + v65[1];
    v65[2] = (float)(v33 * v32[2]) + v65[2];
    v31 = v65 + 3;
    v65[3] = *v65 + v58;
    v31[1] = v65[1] + v59;
    v31[2] = v65[2] + v60;
    v23 = v50->edge.origin[0] * this->uvScale;
    v27 = v23;
    v25 = (SLODWORD(v23) >> 16) & 0xC000;
    v26 = (int)((2 * LODWORD(v23)) ^ 0x80000000) >> 14;
    if ( v26 < 0x3FFF )
      v24 = v26;
    else
      v24 = 0x3FFF;
    if ( v24 > -16384 )
      v9 = v24;
    else
      v9 = -16384;
    v26 = v9 & 0x3FFF;
    v28 = v26 | v25;
    v18 = v50->edge.origin[1] * this->uvScale;
    v22 = v18;
    v20 = (SLODWORD(v18) >> 16) & 0xC000;
    v21 = (int)((2 * LODWORD(v18)) ^ 0x80000000) >> 14;
    if ( v21 < 0x3FFF )
      v19 = v21;
    else
      v19 = 0x3FFF;
    if ( v19 > -16384 )
      v8 = v19;
    else
      v8 = -16384;
    v21 = v8 & 0x3FFF;
    v29 = v21 | v20;
    v30 = (v21 | v20) + (v28 << 16);
    v40 = v30;
    v53[1] = v30;
    *v53 = v53[1];
    v65 += 6;
    v53 += 2;
  }
  if ( highLod )
    numNorm = this->mesh.numNorm;
  else
    numNorm = 2;
  v64 = numNorm;
  for ( j = 0; j < v64; ++j )
  {
    out.packed = (unsigned int)this->mesh.normArray[j];
    v15 = *(float *)&out.packed;
    v16 = *(float *)&out.packed;
    in.packed = out.packed;
    Vec3UnpackUnitVec(out, (float *)&out.packed);
    in.packed = *(unsigned int *)&FLOAT_0_0;
    v46.packed = out.packed;
    v47 = v15;
    v48 = v16;
    v43 = *(float *)&out.packed * this->axis[0][0];
    v44 = *(float *)&out.packed * this->axis[0][1];
    v45 = *(float *)&out.packed * this->axis[0][2];
    v43 = (float)(v15 * this->axis[1][0]) + v43;
    v44 = (float)(v15 * this->axis[1][1]) + v44;
    v45 = (float)(v15 * this->axis[1][2]) + v45;
    v43 = (float)(v16 * this->axis[2][0]) + v43;
    v44 = (float)(v16 * this->axis[2][1]) + v44;
    v45 = (float)(v16 * this->axis[2][2]) + v45;
    LOBYTE(v13) = (int)(float)((float)(v43 * 127.0) + 127.5);
    BYTE1(v13) = (int)(float)((float)(v44 * 127.0) + 127.5);
    BYTE2(v13) = (int)(float)((float)(v45 * 127.0) + 127.5);
    HIBYTE(v13) = 63;
    v61[j] = v13;
  }
  v12.array[0] = (int)(float)((float)(this->axis[0][0] * 127.0) + 127.5);
  v12.array[1] = (int)(float)((float)(this->axis[0][1] * 127.0) + 127.5);
  v12.array[2] = (int)(float)((float)(this->axis[0][2] * 127.0) + 127.5);
  v12.array[3] = 63;
  v56.packed = v12.packed;
  v62 = *(&clGlasses->renderer->maxNumGroupChanges + this->outline.numVerts);
  if ( highLod )
    numVertsLow = this->mesh.numVerts;
  else
    numVertsLow = this->mesh.numVertsLow;
  v63 = numVertsLow;
  v42 = 0;
  while ( v42 < v63 )
  {
    v11 = (float *)&v52[3 * *(unsigned __int8 *)(v62 + 2 * v42)];
    baseVerts->xyz[0] = *v11;
    baseVerts->xyz[1] = v11[1];
    baseVerts->xyz[2] = v11[2];
    baseVerts->normal.packed = v61[*(unsigned __int8 *)(v62 + 2 * v42 + 1)];
    baseVerts->tangent = v56;
    baseVerts->color.packed = -1;
    baseVerts->binormalSign = FLOAT_1_0;
    baseVerts->texCoord.packed = v54[*(unsigned __int8 *)(v62 + 2 * v42++)];
    ++baseVerts;
  }
  indices = this->mesh.indices;
  if ( highLod )
    numIndices = this->mesh.numIndices;
  else
    numIndices = this->mesh.numIndicesLow;
  v57 = numIndices;
  for ( k = 0; k < v57; ++k )
    *idxOut++ = vertsBaseIndex + *indices++;
}

int __thiscall GlassShard::Split(
        GlassShard *this,
        GlassShard **newShards,
        float minShardSize,
        unsigned int startEdge,
        float startEdgeParam)
{
  float max; // [esp+4h] [ebp-DB4h]
  float v7; // [esp+8h] [ebp-DB0h]
  float v8; // [esp+10h] [ebp-DA8h]
  float v9; // [esp+18h] [ebp-DA0h]
  bool v11; // [esp+63h] [ebp-D55h]
  bool v12; // [esp+83h] [ebp-D35h]
  bool v13; // [esp+96h] [ebp-D22h]
  bool isOriginalEdge; // [esp+97h] [ebp-D21h]
  GlassShard::Outline::Vertex *v15; // [esp+A4h] [ebp-D14h]
  float *v16; // [esp+B0h] [ebp-D08h]
  float v17; // [esp+B4h] [ebp-D04h]
  GlassShard::Outline::Vertex *v18; // [esp+B8h] [ebp-D00h]
  float *dir; // [esp+C0h] [ebp-CF8h]
  float v20; // [esp+C4h] [ebp-CF4h]
  GlassShard::Outline::Vertex *v21; // [esp+C8h] [ebp-CF0h]
  float *v22; // [esp+D0h] [ebp-CE8h]
  float *v23; // [esp+D4h] [ebp-CE4h]
  float v24; // [esp+E4h] [ebp-CD4h]
  GlassRenderer *v25; // [esp+ECh] [ebp-CCCh]
  GlassRenderer *renderer; // [esp+F0h] [ebp-CC8h]
  int inited; // [esp+FCh] [ebp-CBCh]
  float d[2]; // [esp+110h] [ebp-CA8h] BYREF
  float newAngle; // [esp+118h] [ebp-CA0h]
  int m; // [esp+11Ch] [ebp-C9Ch]
  int v31; // [esp+120h] [ebp-C98h]
  bool found; // [esp+127h] [ebp-C91h]
  unsigned int nextEdgeIndex; // [esp+128h] [ebp-C90h]
  unsigned int edgeIndex; // [esp+12Ch] [ebp-C8Ch]
  float v35; // [esp+130h] [ebp-C88h]
  float v36; // [esp+134h] [ebp-C84h]
  int k; // [esp+138h] [ebp-C80h]
  float start[2]; // [esp+13Ch] [ebp-C7Ch] BYREF
  float len; // [esp+144h] [ebp-C74h]
  unsigned int i; // [esp+148h] [ebp-C70h]
  float pos[2]; // [esp+14Ch] [ebp-C6Ch] BYREF
  float startParam; // [esp+154h] [ebp-C64h]
  float firstAngle; // [esp+158h] [ebp-C60h]
  int j; // [esp+15Ch] [ebp-C5Ch]
  float minEdgeLength; // [esp+160h] [ebp-C58h]
  float maxEdgeLength; // [esp+164h] [ebp-C54h]
  bool dirSign; // [esp+16Bh] [ebp-C4Dh]
  float newDir[2]; // [esp+16Ch] [ebp-C4Ch] BYREF
  int NUM_TRIES; // [esp+174h] [ebp-C44h]
  TempOutline otherOutline; // [esp+178h] [ebp-C40h] BYREF
  float newEdgeDist; // [esp+78Ch] [ebp-62Ch] BYREF
  GlassTimer timer; // [esp+790h] [ebp-628h]
  int newEdgeIdx; // [esp+7A4h] [ebp-614h] BYREF
  TempOutline newOutline; // [esp+7A8h] [ebp-610h] BYREF

  timer.counter = &clGlasses->renderer->splitTimer;
  timer.start = tlPcGetTick().QuadPart;
  ++clGlasses->renderer->numSplits;
  renderer = clGlasses->renderer;
  minEdgeLength = GlassShard::Outline::Length(&this->outline) * renderer->shardEdgeSize->current.value;
  v25 = clGlasses->renderer;
  maxEdgeLength = GlassShard::Outline::Length(&this->outline) * v25->shardEdgeSize->current.vector[1];
  dirSign = 0;
  newEdgeDist = FLOAT_N1_0;
  newEdgeIdx = -1;
  NUM_TRIES = 10;
  if ( startEdgeParam >= 0.0 )
  {
    dir = this->outline.verts[startEdge].edge.dir;
    v17 = this->outline.verts[startEdge].edge.len * startEdgeParam;
    v18 = &this->outline.verts[startEdge];
    start[0] = (float)(v17 * *dir) + v18->edge.origin[0];
    start[1] = (float)(v17 * dir[1]) + v18->edge.origin[1];
    for ( k = 0; k < 10; ++k )
    {
      dirSign = irand(0, 2) != 0;
      v7 = (float)(dirSign ? 1 : -1);
      v36 = flrand(30.0, 150.0) * v7;
      v16 = this->outline.verts[startEdge].edge.dir;
      newDir[0] = *v16;
      newDir[1] = v16[1];
      VectorAngleMultiply(newDir, COERCE_FLOAT(LODWORD(v36) ^ _mask__NegFloat_));
      if ( GlassShard::Outline::GetNumIntersections(&this->outline, start, newDir, &newEdgeDist, &newEdgeIdx) % 2 == 1
        && newEdgeDist > minEdgeLength )
      {
        break;
      }
      newEdgeIdx = -1;
    }
  }
  else
  {
    startEdge = 64;
    startEdgeParam = *(float *)&FLOAT_0_0;
    for ( j = 0; j < 10 && startEdge >= 0x40; ++j )
    {
      max = GlassShard::Outline::Length(&this->outline);
      startParam = flrand(0.0, max);
      for ( i = 0; i < this->outline.numVerts; ++i )
      {
        len = this->outline.verts[i].edge.len;
        if ( startParam <= len )
        {
          if ( len / GlassShard::Outline::Length(&this->outline) > 0.025 )
          {
            startEdge = i;
            if ( (float)((float)(startParam / len) - 0.75) < 0.0 )
              v24 = startParam / len;
            else
              v24 = FLOAT_0_75;
            if ( (float)(0.25 - (float)(startParam / len)) < 0.0 )
              v9 = v24;
            else
              v9 = FLOAT_0_25;
            startEdgeParam = v9;
          }
          break;
        }
        startParam = startParam - len;
      }
      if ( startEdge < 0x40 )
      {
        dirSign = irand(0, 2) != 0;
        v8 = (float)(dirSign ? 1 : -1);
        firstAngle = flrand(
                       clGlasses->renderer->shardSplitDir->current.value,
                       clGlasses->renderer->shardSplitDir->current.vector[1])
                   * v8
                   + 90.0;
        v23 = this->outline.verts[startEdge].edge.dir;
        newDir[0] = *v23;
        newDir[1] = v23[1];
        VectorAngleMultiply(newDir, COERCE_FLOAT(LODWORD(firstAngle) ^ _mask__NegFloat_));
        v22 = this->outline.verts[startEdge].edge.dir;
        v20 = this->outline.verts[startEdge].edge.len * startEdgeParam;
        v21 = &this->outline.verts[startEdge];
        pos[0] = (float)(v20 * *v22) + v21->edge.origin[0];
        pos[1] = (float)(v20 * v22[1]) + v21->edge.origin[1];
        if ( GlassShard::Outline::GetNumIntersections(&this->outline, pos, newDir, &newEdgeDist, &newEdgeIdx) % 2 == 1
          && newEdgeDist > minEdgeLength )
        {
          break;
        }
        newEdgeIdx = -1;
      }
    }
  }
  if ( newEdgeIdx >= 0 )
  {
    newOutline.verts = newOutline.v;
    newOutline.numVerts = 0;
    newOutline.maxVerts = 64;
    newOutline.isClosed = 0;
    otherOutline.verts = otherOutline.v;
    otherOutline.numVerts = 0;
    otherOutline.maxVerts = 64;
    otherOutline.isClosed = 0;
    v15 = &this->outline.verts[startEdge];
    GlassShard::Outline::Add(&newOutline, v15->edge.origin)->isOriginalEdge = v15->isOriginalEdge;
    GlassShard::Outline::Add(
      &newOutline,
      this->outline.verts[startEdge].edge.dir,
      this->outline.verts[startEdge].edge.len * startEdgeParam)->isOriginalEdge = 0;
    GlassShard::Outline::Add(&otherOutline, newOutline.verts[1].edge.origin);
    while ( 1 )
    {
      v35 = flrand(minEdgeLength, maxEdgeLength);
      if ( (float)(minEdgeLength * 0.5) > (float)(newEdgeDist - v35) )
        break;
      GlassShard::Outline::Add(&newOutline, newDir, v35)->isOriginalEdge = 0;
      GlassShard::Outline::Add(&otherOutline, newOutline.verts[newOutline.numVerts - 1].edge.origin)->isOriginalEdge = 0;
      v31 = 5;
      found = 0;
      for ( m = 0; m < 5; ++m )
      {
        newAngle = flrand(
                     clGlasses->renderer->shardSplitDir2->current.value,
                     clGlasses->renderer->shardSplitDir2->current.vector[1]);
        newAngle = (float)(dirSign ? -1 : 1) * newAngle;
        d[0] = newDir[0];
        d[1] = newDir[1];
        VectorAngleMultiply(d, newAngle);
        if ( !GlassShard::Outline::DoesIntersect(
                &newOutline,
                newOutline.verts[newOutline.numVerts - 1].edge.origin,
                d,
                10000.0,
                minEdgeLength)
          && GlassShard::Outline::GetNumIntersections(
               &this->outline,
               newOutline.verts[newOutline.numVerts - 1].edge.origin,
               d,
               &newEdgeDist,
               &newEdgeIdx) )
        {
          newDir[0] = d[0];
          newDir[1] = d[1];
          found = 1;
          break;
        }
      }
      dirSign = !dirSign;
      if ( !found )
      {
        ++qword_A707074;
        *timer.counter += *(_QWORD *)&tlPcGetTick() - timer.start;
        return 0;
      }
    }
    isOriginalEdge = this->outline.verts[newEdgeIdx].isOriginalEdge;
    GlassShard::Outline::Add(&newOutline, newDir, newEdgeDist)->isOriginalEdge = isOriginalEdge;
    v13 = this->outline.verts[newEdgeIdx].isOriginalEdge;
    GlassShard::Outline::Add(&otherOutline, newOutline.verts[newOutline.numVerts - 1].edge.origin)->isOriginalEdge = v13;
    for ( edgeIndex = ((signed int)-abs(newEdgeIdx - (this->outline.numVerts - 1)) >> 31) & (newEdgeIdx + 1);
          edgeIndex != startEdge;
          edgeIndex = ((signed int)-abs(edgeIndex - (this->outline.numVerts - 1)) >> 31) & (edgeIndex + 1) )
    {
      v12 = this->outline.verts[edgeIndex].isOriginalEdge;
      GlassShard::Outline::Add(&newOutline, this->outline.verts[edgeIndex].edge.origin)->isOriginalEdge = v12;
    }
    if ( !GlassShard::Outline::CloseOutline(&newOutline) )
      goto LABEL_36;
    for ( edgeIndex = newEdgeIdx; edgeIndex != startEdge; edgeIndex = nextEdgeIndex )
    {
      nextEdgeIndex = this->outline.numVerts
                    - 1
                    + (((signed int)-abs(edgeIndex) >> 31) & (edgeIndex - this->outline.numVerts));
      v11 = this->outline.verts[nextEdgeIndex].isOriginalEdge;
      GlassShard::Outline::Add(&otherOutline, this->outline.verts[edgeIndex].edge.origin)->isOriginalEdge = v11;
    }
    if ( GlassShard::Outline::CloseOutline(&otherOutline) )
    {
      inited = GlassShard::InitSplitShards(this, &newOutline, &otherOutline, newShards, minShardSize);
      *timer.counter += *(_QWORD *)&tlPcGetTick() - timer.start;
      return inited;
    }
    else
    {
LABEL_36:
      *timer.counter += *(_QWORD *)&tlPcGetTick() - timer.start;
      return 0;
    }
  }
  else
  {
    ++GlassShard::splitFailCount[0];
    *timer.counter += *(_QWORD *)&tlPcGetTick() - timer.start;
    return 0;
  }
}

int __thiscall GlassShard::Chip(
        GlassShard *this,
        const GlassShard::Outline::EdgeDistance *dist,
        const float *hitPoint,
        GlassShard **newShards,
        float minShardSize)
{
  unsigned int v6; // [esp+8h] [ebp-C9Ch]
  float v7; // [esp+Ch] [ebp-C98h]
  bool v9; // [esp+29h] [ebp-C7Bh]
  bool v10; // [esp+30h] [ebp-C74h]
  bool v11; // [esp+32h] [ebp-C72h]
  bool v12; // [esp+33h] [ebp-C71h]
  bool isOriginalEdge; // [esp+47h] [ebp-C5Dh]
  int inited; // [esp+50h] [ebp-C54h]
  TempOutline outline1; // [esp+5Ch] [ebp-C48h] BYREF
  float newDir[2]; // [esp+66Ch] [ebp-638h] BYREF
  unsigned int edgeIndex; // [esp+674h] [ebp-630h]
  float newEdgeDist; // [esp+678h] [ebp-62Ch] BYREF
  GlassTimer timer; // [esp+67Ch] [ebp-628h]
  float firstAngle; // [esp+68Ch] [ebp-618h]
  int newEdgeIdx; // [esp+690h] [ebp-614h] BYREF
  TempOutline outline2; // [esp+694h] [ebp-610h] BYREF

  timer.counter = &clGlasses->renderer->splitTimer;
  timer.start = tlPcGetTick().QuadPart;
  ++clGlasses->renderer->numSplits;
  outline1.verts = outline1.v;
  outline1.numVerts = 0;
  outline1.maxVerts = 64;
  outline1.isClosed = 0;
  outline2.verts = outline2.v;
  outline2.numVerts = 0;
  outline2.maxVerts = 64;
  outline2.isClosed = 0;
  isOriginalEdge = this->outline.verts[dist->edgeIndex].isOriginalEdge;
  GlassShard::Outline::Add(&outline1, this->outline.verts[dist->edgeIndex].edge.origin)->isOriginalEdge = isOriginalEdge;
  GlassShard::Outline::Add(&outline1, dist->closestPoint)->isOriginalEdge = 0;
  GlassShard::Outline::Add(&outline2, dist->closestPoint)->isOriginalEdge = 0;
  GlassShard::Outline::Add(&outline1, hitPoint)->isOriginalEdge = 0;
  GlassShard::Outline::Add(&outline2, hitPoint)->isOriginalEdge = 0;
  v7 = (float)(irand(0, 2) != 0 ? 1 : -1);
  firstAngle = flrand(30.0, 60.0) * v7;
  newDir[0] = outline1.verts[1].edge.dir[0];
  newDir[1] = outline1.verts[1].edge.dir[1];
  VectorAngleMultiply(newDir, firstAngle);
  newEdgeDist = FLOAT_N1_0;
  newEdgeIdx = -1;
  if ( GlassShard::Outline::GetNumIntersections(&this->outline, hitPoint, newDir, &newEdgeDist, &newEdgeIdx) >= 1 )
  {
    v12 = this->outline.verts[newEdgeIdx].isOriginalEdge;
    GlassShard::Outline::Add(&outline1, newDir, newEdgeDist)->isOriginalEdge = v12;
    v11 = this->outline.verts[newEdgeIdx].isOriginalEdge;
    GlassShard::Outline::Add(&outline2, newDir, newEdgeDist)->isOriginalEdge = v11;
    edgeIndex = newEdgeIdx + 1;
    if ( newEdgeIdx + 1 >= (unsigned int)this->outline.numVerts )
      edgeIndex = 0;
    while ( edgeIndex != dist->edgeIndex )
    {
      v10 = this->outline.verts[edgeIndex].isOriginalEdge;
      GlassShard::Outline::Add(&outline1, this->outline.verts[edgeIndex++].edge.origin)->isOriginalEdge = v10;
      if ( edgeIndex >= this->outline.numVerts )
        edgeIndex = 0;
    }
    GlassShard::Outline::CloseOutline(&outline1);
    for ( edgeIndex = newEdgeIdx; edgeIndex != dist->edgeIndex; edgeIndex = v6 )
    {
      if ( edgeIndex )
        v6 = edgeIndex - 1;
      else
        v6 = this->outline.numVerts - 1;
      v9 = this->outline.verts[v6].isOriginalEdge;
      GlassShard::Outline::Add(&outline2, this->outline.verts[edgeIndex].edge.origin)->isOriginalEdge = v9;
    }
    GlassShard::Outline::CloseOutline(&outline2);
    inited = GlassShard::InitSplitShards(this, &outline1, &outline2, newShards, minShardSize);
    *timer.counter += *(_QWORD *)&tlPcGetTick() - timer.start;
    return inited;
  }
  else
  {
    *timer.counter += *(_QWORD *)&tlPcGetTick() - timer.start;
    return 0;
  }
}

int __thiscall GlassShard::InitSplitShards(
        GlassShard *this,
        GlassShard::Outline *outline1,
        GlassShard::Outline *outline2,
        GlassShard **newShards,
        float minShardSize)
{
  float otherOffset[2]; // [esp+14h] [ebp-10h] BYREF
  float offset[2]; // [esp+1Ch] [ebp-8h] BYREF

  if ( outline1->numVerts < 3u || outline2->numVerts < 3u || outline1->numVerts > 0x16u || outline2->numVerts > 0x16u )
  {
    ++qword_A70707C;
    return 0;
  }
  if ( minShardSize > GlassShard::Outline::Area(outline1) )
  {
    ++MEMORY[0xA707080];
    return 0;
  }
  if ( minShardSize > GlassShard::Outline::Area(outline2) )
  {
    ++MEMORY[0xA707080];
    return 0;
  }
  if ( GlassShard::Outline::HasNarrowAngle(outline1) || GlassShard::Outline::HasNarrowAngle(outline2) )
  {
    ++dword_A707088;
    return 0;
  }
  if ( GlassShard::Outline::IsNarrow(outline1) || GlassShard::Outline::IsNarrow(outline2) )
  {
    ++dword_A70708C;
    return 0;
  }
  *newShards = GlassRenderer::AllocShard(clGlasses->renderer);
  if ( *newShards )
  {
    otherOffset[0] = *(float *)&FLOAT_0_0;
    otherOffset[1] = *(float *)&FLOAT_0_0;
    GlassShard::Outline::Reverse(outline2);
    if ( GlassShard::Init(*newShards, this, outline2, otherOffset) )
    {
      offset[0] = *(float *)&FLOAT_0_0;
      offset[1] = *(float *)&FLOAT_0_0;
      if ( GlassShard::Init(this, this, outline1, offset) )
      {
        return 1;
      }
      else
      {
        GlassRenderer::FreeShard(clGlasses->renderer, *newShards);
        if ( !GlassShard::Init(this, this, outline2, otherOffset) )
        {
          if ( !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp",
                  2851,
                  0,
                  "Out of shard memory") )
            __debugbreak();
          Com_Error(ERR_DROP, "Out of shard memory\n");
        }
        ++MEMORY[0xA707078];
        return 0;
      }
    }
    else
    {
      ++MEMORY[0xA707078];
      GlassRenderer::FreeShard(clGlasses->renderer, *newShards);
      return 0;
    }
  }
  else
  {
    ++dword_A707084;
    return 0;
  }
}

char __thiscall GlassShard::Init(
        GlassShard *this,
        const GlassShard *other,
        GlassShard::Outline *newOutline,
        float *offset)
{
  float v6; // [esp+10h] [ebp-150h]
  float v7; // [esp+20h] [ebp-140h]
  unsigned int i; // [esp+4Ch] [ebp-114h]
  GlassShard::Triangles triangles; // [esp+50h] [ebp-110h] BYREF

  GlassShard::Outline::Verify(newOutline);
  GlassShard::FreeMemory(this);
  GlassShard::Triangles::Triangles(&triangles, newOutline);
  if ( !GlassShard::Triangles::Triangulate(&triangles) )
    return 0;
  if ( !GlassShard::AllocateMemory(this, newOutline->numVerts, &triangles) )
    return 0;
  GlassShard::Outline::operator=(&this->outline, newOutline);
  this->thickness = other->thickness;
  this->uvScale = other->uvScale;
  this->glassIndex = other->glassIndex;
  this->axis[0][0] = other->axis[0][0];
  this->axis[0][1] = other->axis[0][1];
  this->axis[0][2] = other->axis[0][2];
  this->axis[1][0] = other->axis[1][0];
  this->axis[1][1] = other->axis[1][1];
  this->axis[1][2] = other->axis[1][2];
  this->axis[2][0] = other->axis[2][0];
  this->axis[2][1] = other->axis[2][1];
  this->axis[2][2] = other->axis[2][2];
  this->origin[0] = other->origin[0];
  this->origin[1] = other->origin[1];
  this->origin[2] = other->origin[2];
  v7 = *offset;
  this->origin[0] = (float)(*offset * this->axis[0][0]) + this->origin[0];
  this->origin[1] = (float)(v7 * this->axis[0][1]) + this->origin[1];
  this->origin[2] = (float)(v7 * this->axis[0][2]) + this->origin[2];
  v6 = offset[1];
  this->origin[0] = (float)(v6 * this->axis[1][0]) + this->origin[0];
  this->origin[1] = (float)(v6 * this->axis[1][1]) + this->origin[1];
  this->origin[2] = (float)(v6 * this->axis[1][2]) + this->origin[2];
  this->isOriginalEdge = 0;
  for ( i = 0; i < this->outline.numVerts && !this->isOriginalEdge; ++i )
    this->isOriginalEdge = this->outline.verts[i].isOriginalEdge;
  return 1;
}

double __thiscall GlassShard::GetMass(GlassShard *this)
{
  float v2; // [esp+4h] [ebp-1Ch]
  float volume; // [esp+10h] [ebp-10h]

  LODWORD(v2) = LODWORD(this->thickness) & _mask__AbsFloat_;
  volume = GlassShard::Outline::Area(&this->outline) * v2;
  if ( volume < 100.0 )
    return 0.001;
  if ( volume <= 1000.0 )
    return (0.0099999998 - 0.001) * ((volume - 100.0) / (1000.0 - 100.0)) + 0.001;
  return 0.0099999998;
}

bool __thiscall GlassShard::InitPhysicsObj(GlassShard *this, bool enableCollisions)
{
  gjk_aabb_t *aabb_gjk_geom; // eax
  float mass; // [esp+0h] [ebp-C4h]
  GlassShard *other; // [esp+30h] [ebp-94h]
  PhysPreset physPreset; // [esp+34h] [ebp-90h] BYREF
  float localBBoxMin[3]; // [esp+90h] [ebp-34h] BYREF
  float quat[4]; // [esp+9Ch] [ebp-28h] BYREF
  broad_phase_base *bpb; // [esp+ACh] [ebp-18h]
  float localBBoxMax[3]; // [esp+B0h] [ebp-14h] BYREF
  gjk_geom_list_t gjk_geom_list; // [esp+BCh] [ebp-8h] BYREF
  int savedregs; // [esp+C4h] [ebp+0h] BYREF

  this->timeUntilAction = clGlasses->renderer->maxShardLife->current.value;
  if ( this->physObjId || this->glassPhysics )
    return 1;
  if ( enableCollisions && GlassRenderer::AddColidingShard(clGlasses->renderer, this) )
  {
    other = (GlassShard *)GlassRenderer::GetSmallestColidingShard(clGlasses->renderer);
    if ( other == this )
    {
      GlassRenderer::RemoveColidingShard(clGlasses->renderer, this);
      enableCollisions = 0;
    }
    else
    {
      GlassShard::DisableCollisions(other);
    }
  }
  GlassShard::GetLocalBBox(this, localBBoxMin, localBBoxMax);
  if ( enableCollisions )
  {
    Com_Memset((unsigned int *)&physPreset, 0, 84);
    physPreset.mass = GlassShard::GetMass(this);
    physPreset.friction = FLOAT_0_5;
    AxisToQuat(this->axis, quat);
    Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
    Sys_EnterCriticalSection(CRITSECT_PHYSICS);
    gjk_geom_list.m_first_geom = 0;
    gjk_geom_list.m_geom_count = 0;
    aabb_gjk_geom = create_aabb_gjk_geom(
                      COERCE_FLOAT(&savedregs),
                      localBBoxMin,
                      localBBoxMax,
                      9,
                      &g_empty_collision_visitor);
    gjk_geom_list_t::add_geom(&gjk_geom_list, aabb_gjk_geom);
    this->physObjId = (int)Phys_ObjCreate(1, this->origin, quat, vec3_origin, &physPreset, &gjk_geom_list, 1, -1);
    bpb = *(broad_phase_base **)(this->physObjId + 160);
    aasap_list_remove(bpb);
    bpb->m_env_collision_flags &= ~0x40u;
    bpb->m_my_collision_type_flags |= 0x100u;
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
    return 1;
  }
  else
  {
    this->glassPhysics = GlassRenderer::AllocPhysics(clGlasses->renderer);
    if ( this->glassPhysics )
    {
      mass = GlassShard::GetMass(this);
      GlassPhysics::CreateAxis(
        this->glassPhysics,
        (int)&savedregs,
        this->origin,
        this->axis,
        (float *)vec3_origin,
        (float *)vec3_origin,
        mass,
        localBBoxMin,
        localBBoxMax);
    }
    return this->glassPhysics != 0;
  }
}

void __thiscall GlassShard::DestroyPhysicsObj(GlassShard *this)
{
  if ( this->physObjId )
  {
    GlassRenderer::RemoveColidingShard(clGlasses->renderer, this);
    Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
    Sys_EnterCriticalSection(CRITSECT_PHYSICS);
    Phys_ObjDestroy(1, this->physObjId);
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
    this->physObjId = 0;
  }
  if ( this->glassPhysics )
  {
    GlassRenderer::FreePhysics(clGlasses->renderer, this->glassPhysics);
    this->glassPhysics = 0;
  }
}

void __thiscall GlassShard::DisableCollisions(GlassShard *this)
{
  float mass; // [esp+0h] [ebp-48h]
  float localBBoxMin[3]; // [esp+18h] [ebp-30h] BYREF
  float localBBoxMax[3]; // [esp+24h] [ebp-24h] BYREF
  float aVel[3]; // [esp+30h] [ebp-18h] BYREF
  float tVel[3]; // [esp+3Ch] [ebp-Ch] BYREF
  int savedregs; // [esp+48h] [ebp+0h] BYREF

  if ( !this->physObjId
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 3009, 0, "%s", "physObjId") )
  {
    __debugbreak();
  }
  if ( this->glassPhysics
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 3010, 0, "%s", "!glassPhysics") )
  {
    __debugbreak();
  }
  GlassRenderer::RemoveColidingShard(clGlasses->renderer, this);
  this->glassPhysics = GlassRenderer::AllocPhysics(clGlasses->renderer);
  if ( this->glassPhysics )
  {
    Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
    Sys_EnterCriticalSection(CRITSECT_PHYSICS);
    Phys_ObjGetVelocities(this->physObjId, tVel, aVel);
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
    GlassShard::GetLocalBBox(this, localBBoxMin, localBBoxMax);
    mass = GlassShard::GetMass(this);
    GlassPhysics::CreateAxis(
      this->glassPhysics,
      (int)&savedregs,
      this->origin,
      this->axis,
      tVel,
      aVel,
      mass,
      localBBoxMin,
      localBBoxMax);
  }
  else
  {
    GlassShard::Remove(this, REMOVE_OUT_OF_PHYSICS, 1);
  }
  Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
  Sys_EnterCriticalSection(CRITSECT_PHYSICS);
  Phys_ObjDestroy(1, this->physObjId);
  Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
  Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
  this->physObjId = 0;
}

void __thiscall GlassShard::AddForce(GlassShard *this, float *pos, const float *forceIn)
{
  float v4; // [esp+4h] [ebp-20h]
  float force[3]; // [esp+14h] [ebp-10h] BYREF
  float size; // [esp+20h] [ebp-4h]
  int savedregs; // [esp+24h] [ebp+0h] BYREF

  size = Vec3NormalizeTo(forceIn, force);
  if ( size > 0.0 )
  {
    if ( (float)(500000.0 - size) < 0.0 )
      v4 = FLOAT_500000_0;
    else
      v4 = size;
    force[0] = v4 * force[0];
    force[1] = v4 * force[1];
    force[2] = v4 * force[2];
    if ( this->physObjId )
    {
      Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
      Sys_EnterCriticalSection(CRITSECT_PHYSICS);
      Phys_ObjAddForce((int)&savedregs, this->physObjId, pos, force, 0);
      Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
      Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
    }
    else
    {
      if ( !this->glassPhysics
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 3059, 0, "%s", "glassPhysics") )
      {
        __debugbreak();
      }
      GlassPhysics::AddForce(this->glassPhysics, (int)&savedregs, pos, force);
    }
  }
}

void __thiscall GlassShard::SetVelocity(GlassShard *this, float *t_vel, float *a_vel)
{
  int savedregs; // [esp+4h] [ebp+0h] BYREF

  if ( this->physObjId )
  {
    Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
    Sys_EnterCriticalSection(CRITSECT_PHYSICS);
    Phys_ObjSetVelocity((int)&savedregs, this->physObjId, t_vel);
    Phys_ObjSetAngularVelocity((int)&savedregs, this->physObjId, a_vel);
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
  }
  else
  {
    if ( !this->glassPhysics
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 3078, 0, "%s", "glassPhysics") )
    {
      __debugbreak();
    }
    GlassPhysics::SetVelocity(this->glassPhysics, t_vel);
    GlassPhysics::SetAngularVelocity(this->glassPhysics, a_vel);
  }
}

bool __thiscall GlassShard::LeaveOnEdge(GlassShard *this, float stickiness)
{
  float prob; // [esp+8h] [ebp-8h]
  bool ret; // [esp+Fh] [ebp-1h]

  if ( !this->isOriginalEdge
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 3086, 0, "%s", "isOriginalEdge") )
  {
    __debugbreak();
  }
  ret = 0;
  if ( stickiness > 0.0 )
  {
    prob = (GlassShard::EdgeRatio(this) - 0.15000001) * stickiness / (0.34999999 - 0.15000001);
    return prob > 0.0 && prob > random();
  }
  return ret;
}

bool __thiscall GlassShard::IsOnBottomEdge(GlassShard *this)
{
  float v2; // [esp+0h] [ebp-58h]
  float v3; // [esp+4h] [ebp-54h]
  float wPos[3]; // [esp+24h] [ebp-34h] BYREF
  unsigned int j; // [esp+30h] [ebp-28h]
  float minOthers; // [esp+34h] [ebp-24h]
  float minEdge; // [esp+38h] [ebp-20h]
  unsigned int edgeIdx2; // [esp+3Ch] [ebp-1Ch]
  float wDir[3]; // [esp+40h] [ebp-18h] BYREF
  unsigned int i; // [esp+4Ch] [ebp-Ch]
  unsigned int edgeIdx; // [esp+50h] [ebp-8h]
  int numEdges; // [esp+54h] [ebp-4h]

  if ( !this->isOriginalEdge
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp", 3100, 0, "%s", "isOriginalEdge") )
  {
    __debugbreak();
  }
  if ( COERCE_FLOAT(LODWORD(this->axis[2][2]) & _mask__AbsFloat_) >= 0.0099999998 )
    return 0;
  numEdges = 0;
  edgeIdx = -1;
  for ( i = 0; i < this->outline.numVerts; ++i )
  {
    if ( this->outline.verts[i].isOriginalEdge )
    {
      if ( ++numEdges == 2 )
        return 0;
      GlassShard::ToWorldDir(this, this->outline.verts[i].edge.dir, wDir, 0);
      if ( COERCE_FLOAT(LODWORD(wDir[2]) & _mask__AbsFloat_) < 0.0099999998 )
        edgeIdx = i;
    }
  }
  if ( edgeIdx == -1 )
    return 0;
  edgeIdx2 = ((signed int)-abs(edgeIdx - (this->outline.numVerts - 1)) >> 31) & (edgeIdx + 1);
  minEdge = FLOAT_3_4028235e38;
  minOthers = FLOAT_3_4028235e38;
  for ( j = 0; j < this->outline.numVerts; ++j )
  {
    GlassShard::ToWorldPos(this, this->outline.verts[j].edge.origin, wPos, 0);
    if ( j == edgeIdx || j == edgeIdx2 )
    {
      if ( (float)(wPos[2] - minEdge) < 0.0 )
        v3 = wPos[2];
      else
        v3 = minEdge;
      minEdge = v3;
    }
    else
    {
      if ( (float)(wPos[2] - minOthers) < 0.0 )
        v2 = wPos[2];
      else
        v2 = minOthers;
      minOthers = v2;
    }
  }
  return minOthers > minEdge;
}

void __thiscall GlassShard::Recenter(GlassShard *this)
{
  float v2; // [esp+Ch] [ebp-20h]
  float v3; // [esp+1Ch] [ebp-10h]
  float offset[2]; // [esp+24h] [ebp-8h] BYREF

  GlassShard::Outline::Recenter(&this->outline, 0, offset);
  v3 = offset[0];
  this->origin[0] = (float)(offset[0] * this->axis[0][0]) + this->origin[0];
  this->origin[1] = (float)(v3 * this->axis[0][1]) + this->origin[1];
  this->origin[2] = (float)(v3 * this->axis[0][2]) + this->origin[2];
  v2 = offset[1];
  this->origin[0] = (float)(offset[1] * this->axis[1][0]) + this->origin[0];
  this->origin[1] = (float)(v2 * this->axis[1][1]) + this->origin[1];
  this->origin[2] = (float)(v2 * this->axis[1][2]) + this->origin[2];
}

char __thiscall GlassShard::InitPhysics(
        GlassShard *this,
        bool enableCollisions,
        const float *hitPos,
        const float *hitDir,
        float glassSize,
        float stickiness)
{
  float v7; // [esp+8h] [ebp-B0h]
  float v9; // [esp+10h] [ebp-A8h]
  float v10; // [esp+14h] [ebp-A4h]
  float v11; // [esp+18h] [ebp-A0h]
  float v12; // [esp+20h] [ebp-98h]
  float v13; // [esp+28h] [ebp-90h]
  float v14; // [esp+2Ch] [ebp-8Ch]
  float value; // [esp+34h] [ebp-84h]
  float v16; // [esp+40h] [ebp-78h]
  float v17; // [esp+48h] [ebp-70h]
  float v18; // [esp+50h] [ebp-68h]
  float v19; // [esp+58h] [ebp-60h]
  float v20; // [esp+5Ch] [ebp-5Ch]
  float v21; // [esp+60h] [ebp-58h]
  float v; // [esp+70h] [ebp-48h] BYREF
  float v23; // [esp+74h] [ebp-44h]
  float v24; // [esp+78h] [ebp-40h]
  float dist; // [esp+7Ch] [ebp-3Ch]
  float distAtten; // [esp+80h] [ebp-38h]
  float dir[3]; // [esp+84h] [ebp-34h] BYREF
  float strength; // [esp+90h] [ebp-28h]
  float EXTRA_VEL; // [esp+94h] [ebp-24h]
  float velocity[3]; // [esp+98h] [ebp-20h] BYREF
  float EXTRA_ANG_VEL; // [esp+A4h] [ebp-14h]
  float angVelocity[3]; // [esp+A8h] [ebp-10h] BYREF
  float dirLengthSq; // [esp+B4h] [ebp-4h]

  if ( this->outline.verts )
  {
    this->delayedDrop = 0;
    if ( !this->isOriginalEdge || stickiness <= 0.0 || GlassShard::IsOnBottomEdge(this) )
    {
      if ( GlassShard::InitPhysicsObj(this, enableCollisions) )
      {
        dirLengthSq = (float)((float)(*hitDir * *hitDir) + (float)(hitDir[1] * hitDir[1]))
                    + (float)(hitDir[2] * hitDir[2]);
        if ( dirLengthSq == 0.0 )
        {
          EXTRA_VEL = clGlasses->renderer->extraVelocity->current.value;
          v19 = flrand(COERCE_FLOAT(LODWORD(EXTRA_VEL) ^ _mask__NegFloat_), EXTRA_VEL);
          v20 = flrand(COERCE_FLOAT(LODWORD(EXTRA_VEL) ^ _mask__NegFloat_), EXTRA_VEL);
          v21 = flrand(COERCE_FLOAT(LODWORD(EXTRA_VEL) ^ _mask__NegFloat_), EXTRA_VEL);
          velocity[0] = v19;
          velocity[1] = v20;
          velocity[2] = v21;
        }
        else if ( dirLengthSq <= 1.01 )
        {
          if ( glassSize <= 0.0
            && !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_shard.cpp",
                  3227,
                  0,
                  "%s",
                  "glassSize > 0.0f") )
          {
            __debugbreak();
          }
          v = this->origin[0] - *hitPos;
          v23 = this->origin[1] - hitPos[1];
          v24 = this->origin[2] - hitPos[2];
          dist = Vec3Normalize(&v);
          value = clGlasses->renderer->forceOriginMult->current.value;
          v = (float)(value * v) + *hitDir;
          v23 = (float)(value * v23) + hitDir[1];
          v24 = (float)(value * v24) + hitDir[2];
          v13 = (float)(clGlasses->renderer->forceAttenuation->current.value * dist) / glassSize;
          if ( (float)(v13 - 1.0) < 0.0 )
            v14 = (float)(clGlasses->renderer->forceAttenuation->current.value * dist) / glassSize;
          else
            v14 = FLOAT_1_0;
          if ( (float)(0.25 - v13) < 0.0 )
            v7 = v14;
          else
            v7 = FLOAT_0_25;
          distAtten = 1.0 - v7;
          v = (float)(1.0 - v7) * v;
          v23 = (float)(1.0 - v7) * v23;
          v24 = (float)(1.0 - v7) * v24;
          v12 = clGlasses->renderer->forceMultiplier->current.value;
          velocity[0] = v12 * v;
          velocity[1] = v12 * v23;
          velocity[2] = v12 * v24;
        }
        else
        {
          dir[0] = this->origin[0] - *hitPos;
          dir[1] = this->origin[1] - hitPos[1];
          dir[2] = this->origin[2] - hitPos[2];
          Vec3Normalize(dir);
          strength = fsqrt(dirLengthSq) - 1.0;
          if ( (float)((float)(strength * 2.0) - 1.0) < 0.0 )
            v18 = strength * 2.0;
          else
            v18 = FLOAT_1_0;
          if ( (float)(0.2 - (float)(strength * 2.0)) < 0.0 )
            v17 = v18;
          else
            v17 = FLOAT_0_2;
          dir[0] = v17 * dir[0];
          dir[1] = v17 * dir[1];
          dir[2] = v17 * dir[2];
          v16 = clGlasses->renderer->forceMultiplier->current.value;
          velocity[0] = v16 * dir[0];
          velocity[1] = v16 * dir[1];
          velocity[2] = v16 * dir[2];
        }
        EXTRA_ANG_VEL = clGlasses->renderer->extraVelocity->current.value;
        v9 = flrand(COERCE_FLOAT(LODWORD(EXTRA_ANG_VEL) ^ _mask__NegFloat_), EXTRA_ANG_VEL);
        v10 = flrand(COERCE_FLOAT(LODWORD(EXTRA_ANG_VEL) ^ _mask__NegFloat_), EXTRA_ANG_VEL);
        v11 = flrand(COERCE_FLOAT(LODWORD(EXTRA_ANG_VEL) ^ _mask__NegFloat_), EXTRA_ANG_VEL);
        angVelocity[0] = v9;
        angVelocity[1] = v10;
        angVelocity[2] = v11;
        GlassShard::SetVelocity(this, velocity, angVelocity);
        GlassShard::UnEdge(this);
        return 1;
      }
      else
      {
        GlassShard::Remove(this, REMOVE_OUT_OF_PHYSICS, 1);
        return 0;
      }
    }
    else
    {
      GlassShard::UpdateBBox(this);
      if ( !GlassShard::LeaveOnEdge(this, stickiness) )
      {
        this->delayedDrop = 1;
        this->timeUntilAction = flrand(
                                  clGlasses->renderer->timeUntilDropRange->current.value,
                                  clGlasses->renderer->timeUntilDropRange->current.vector[1]);
      }
      return 1;
    }
  }
  else
  {
    GlassShard::Remove(this, REMOVE_OUT_OF_SHARD_MEMORY, 1);
    return 0;
  }
}

void __thiscall GlassShard::DrawOutline(GlassShard *this)
{
  char *v1; // eax
  bool IsNarrow; // al
  char *v3; // eax
  float v4; // [esp+28h] [ebp-3B0h]
  float v5; // [esp+2Ch] [ebp-3ACh]
  float v6; // [esp+30h] [ebp-3A8h]
  float *color; // [esp+34h] [ebp-3A4h]
  unsigned __int8 numVerts; // [esp+3Fh] [ebp-399h]
  float v10; // [esp+40h] [ebp-398h]
  float *v11; // [esp+58h] [ebp-380h]
  float v12; // [esp+7Ch] [ebp-35Ch]
  float area2bboxRatio; // [esp+8Ch] [ebp-34Ch]
  float val; // [esp+90h] [ebp-348h]
  float localBBoxMin[3]; // [esp+94h] [ebp-344h] BYREF
  float bboxExtent[3]; // [esp+A0h] [ebp-338h]
  float localBBoxMax[3]; // [esp+ACh] [ebp-32Ch] BYREF
  unsigned int j; // [esp+B8h] [ebp-320h]
  float pos[3]; // [esp+BCh] [ebp-31Ch]
  float temp[3]; // [esp+C8h] [ebp-310h]
  unsigned int i; // [esp+D4h] [ebp-304h]
  float v[64][3]; // [esp+D8h] [ebp-300h] BYREF

  for ( i = 0; i < this->outline.numVerts; ++i )
  {
    v12 = this->outline.verts[i].edge.origin[1];
    pos[0] = this->outline.verts[i].edge.origin[0];
    pos[1] = v12;
    pos[2] = *(float *)&FLOAT_0_0;
    temp[0] = (float)(pos[0] * this->axis[0][0]) + this->origin[0];
    temp[1] = (float)(pos[0] * this->axis[0][1]) + this->origin[1];
    temp[2] = (float)(pos[0] * this->axis[0][2]) + this->origin[2];
    temp[0] = (float)(v12 * this->axis[1][0]) + temp[0];
    temp[1] = (float)(v12 * this->axis[1][1]) + temp[1];
    temp[2] = (float)(v12 * this->axis[1][2]) + temp[2];
    v11 = v[i];
    *v11 = (float)(0.0 * this->axis[2][0]) + temp[0];
    v11[1] = (float)(0.0 * this->axis[2][1]) + temp[1];
    v11[2] = (float)(0.0 * this->axis[2][2]) + temp[2];
  }
  for ( j = 0; j < this->outline.numVerts; ++j )
  {
    if ( this->outline.verts[j].isOriginalEdge )
      color = (float *)colorBlue;
    else
      color = (float *)colorYellow;
    CL_AddDebugLine(v[j], v[(j + 1) % this->outline.numVerts], color, 1, 0);
  }
  if ( clGlasses->renderer->drawShardOutline->current.integer <= 0 )
  {
    if ( clGlasses->renderer->pickShards->current.enabled )
    {
      val = GlassShard::EdgeRatio(this) * 100.0;
      GlassShard::GetLocalBBox(this, localBBoxMin, localBBoxMax);
      bboxExtent[0] = localBBoxMax[0] - localBBoxMin[0];
      bboxExtent[1] = localBBoxMax[1] - localBBoxMin[1];
      bboxExtent[2] = localBBoxMax[2] - localBBoxMin[2];
      if ( (float)((float)(localBBoxMax[0] - localBBoxMin[0]) - (float)(localBBoxMax[1] - localBBoxMin[1])) < 0.0 )
        v6 = bboxExtent[1];
      else
        v6 = bboxExtent[0];
      if ( (float)(bboxExtent[1] - bboxExtent[0]) < 0.0 )
        v5 = bboxExtent[1];
      else
        v5 = bboxExtent[0];
      v4 = bboxExtent[0] * bboxExtent[1];
      area2bboxRatio = v4 / GlassShard::Outline::Area(&this->outline);
      v10 = GlassShard::Outline::Area(&this->outline);
      numVerts = this->outline.numVerts;
      IsNarrow = GlassShard::Outline::IsNarrow(&this->outline);
      v3 = va("%d %.1f %.2f (%.2f %.2f) %d", numVerts, v10, val, (float)(v6 / v5), area2bboxRatio, IsNarrow);
      CL_AddDebugString(this->origin, colorWhite, 0.25, v3, 0);
      CL_AddDebugBox(this->axis, this->origin, localBBoxMin, localBBoxMax, colorBlue, 1, 0);
    }
  }
  else if ( clGlasses->renderer->drawShardOutline->current.integer > 1 )
  {
    v1 = va("%d", this->outline.numVerts);
    CL_AddDebugString(this->origin, colorWhite, 0.5, v1, 0);
  }
}

