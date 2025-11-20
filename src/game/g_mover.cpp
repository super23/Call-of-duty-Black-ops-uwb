#include "g_mover.h"

mover_info_t *__cdecl get_mover_info(gentity_s *ent)
{
  generic_avl_map_node_t *m_tree_root; // [esp+0h] [ebp-Ch]

  m_tree_root = g_mover_info_map.m_tree_root;
  while ( m_tree_root && ent != (gentity_s *)m_tree_root->m_avl_key )
  {
    if ( (unsigned int)ent >= m_tree_root->m_avl_key )
      m_tree_root = m_tree_root->m_avl_node_info.m_right;
    else
      m_tree_root = m_tree_root->m_avl_node_info.m_left;
  }
  if ( m_tree_root )
    return (mover_info_t *)m_tree_root->m_data;
  else
    return 0;
}

mover_info_t *__cdecl create_mover_info(gentity_s *ent)
{
  mover_info_t *mi; // [esp+10h] [ebp-8h]

  mi = phys_simple_allocator<mover_info_t>::allocate(&g_mover_info_allocator);
  generic_avl_map_add(&g_mover_info_map, mi, (unsigned int)ent);
  return mi;
}

bool __cdecl entity_is_a_mover(int entnum)
{
  return entnum >= 0 && entnum < level.num_entities && get_mover_info(&level.gentities[entnum]) != 0;
}

void __userpurge mover_info_t::apply_rotation(mover_info_t *this@<ecx>, int a2@<ebp>, float *v)
{
  const phys_vec3 *v3; // eax
  const phys_vec3 *v4; // [esp-30h] [ebp-50h]
  phys_vec3 v5; // [esp-2Ch] [ebp-4Ch] BYREF
  phys_vec3 v6; // [esp-1Ch] [ebp-3Ch] BYREF
  unsigned int v7[3]; // [esp-Ch] [ebp-2Ch] BYREF
  mover_info_t *v8; // [esp+10h] [ebp-10h]
  int v9; // [esp+14h] [ebp-Ch]
  void *v10; // [esp+18h] [ebp-8h]
  void *retaddr; // [esp+20h] [ebp+0h]

  v9 = a2;
  v10 = retaddr;
  v8 = this;
  Phys_Vec3ToNitrousVec(v, (phys_vec3 *)v7);
  v3 = phys_inv_multiply(&v6, &v8->m_prev_mat, (const phys_vec3 *)v7);
  v4 = phys_multiply(&v5, &v8->m_mat, v3);
  v7[0] = LODWORD(v4->x);
  v7[1] = LODWORD(v4->y);
  v7[2] = LODWORD(v4->z);
  Phys_NitrousVecToVec3((const phys_vec3 *)v7, v);
}

void __userpurge mover_info_t::apply_rotation_translation(mover_info_t *this@<ecx>, int a2@<ebp>, float *v)
{
  phys_vec3 *v3; // eax
  const phys_vec3 *v4; // [esp-30h] [ebp-50h]
  phys_vec3 v5; // [esp-2Ch] [ebp-4Ch] BYREF
  phys_vec3 v6; // [esp-1Ch] [ebp-3Ch] BYREF
  unsigned int v7[3]; // [esp-Ch] [ebp-2Ch] BYREF
  mover_info_t *v8; // [esp+10h] [ebp-10h]
  unsigned int v9[2]; // [esp+14h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+20h] [ebp+0h]

  v9[0] = a2;
  v9[1] = retaddr;
  v8 = this;
  Phys_Vec3ToNitrousVec(v, (phys_vec3 *)v7);
  v3 = phys_full_inv_multiply((int)v9, &v6, &v8->m_prev_mat, (const phys_vec3 *)v7);
  v4 = phys_full_multiply((int)v9, &v5, &v8->m_mat, v3);
  v7[0] = LODWORD(v4->x);
  v7[1] = LODWORD(v4->y);
  v7[2] = LODWORD(v4->z);
  Phys_NitrousVecToVec3((const phys_vec3 *)v7, v);
}

void __cdecl G_PushEntity(gentity_s *check, gentity_s *pusher, const float *move, const float *amove, mover_info_t *mi)
{
  gclient_s *client; // edx
  float vOrigin[3]; // [esp+118h] [ebp-Ch] BYREF
  int savedregs; // [esp+124h] [ebp+0h] BYREF

  vOrigin[0] = check->r.currentOrigin[0];
  vOrigin[1] = check->r.currentOrigin[1];
  vOrigin[2] = check->r.currentOrigin[2];
  mover_info_t::apply_rotation_translation(mi, (int)&savedregs, vOrigin);
  check->r.currentOrigin[0] = vOrigin[0];
  check->r.currentOrigin[1] = vOrigin[1];
  check->r.currentOrigin[2] = vOrigin[2];
  check->s.lerp.pos.trBase[0] = vOrigin[0];
  check->s.lerp.pos.trBase[1] = vOrigin[1];
  check->s.lerp.pos.trBase[2] = vOrigin[2];
  if ( check->client )
  {
    check->client->ps.delta_angles[1] = check->client->ps.delta_angles[1] + amove[1];
    client = check->client;
    client->ps.origin[0] = vOrigin[0];
    client->ps.origin[1] = vOrigin[1];
    client->ps.origin[2] = vOrigin[2];
  }
  if ( check->s.eType == 4 )
  {
    check->r.currentAngles[0] = check->r.currentAngles[0] + *amove;
    check->r.currentAngles[1] = check->r.currentAngles[1] + amove[1];
    check->r.currentAngles[2] = check->r.currentAngles[2] + amove[2];
    check->s.lerp.apos.trBase[0] = check->s.lerp.apos.trBase[0] + *amove;
    check->s.lerp.apos.trBase[1] = check->s.lerp.apos.trBase[1] + amove[1];
    check->s.lerp.apos.trBase[2] = check->s.lerp.apos.trBase[2] + amove[2];
    mover_info_t::apply_rotation(mi, (int)&savedregs, &check->mover.midTime);
  }
}

char __cdecl IsEntityLinkedToMe(gentity_s *self, gentity_s *other)
{
  tagInfo_s *tagInfo; // [esp+0h] [ebp-4h]

  if ( !self && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_mover.cpp", 302, 0, "%s", "self") )
    __debugbreak();
  if ( !other && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_mover.cpp", 303, 0, "%s", "other") )
    __debugbreak();
  for ( tagInfo = other->tagInfo; tagInfo; tagInfo = tagInfo->parent->tagInfo )
  {
    if ( tagInfo->parent == self )
      return 1;
  }
  return 0;
}

gentity_s *__cdecl GetEntity(const unsigned __int16 *targetname)
{
  int i; // [esp+0h] [ebp-4h]

  for ( i = 0; i < level.num_entities; ++i )
  {
    if ( g_entities[i].targetname == *targetname )
      return &g_entities[i];
  }
  return 0;
}

void  CreateConstraint(rigid_body *a1@<ebp>, PhysConstraint *constraint)
{
  const phys_vec3 *v2; // eax
  phys_vec3 *max_distance; // [esp+4h] [ebp-120h]
  rigid_body_constraint_distance *rbc_dist; // [esp+8h] [ebp-11Ch]
  float v5; // [esp+Ch] [ebp-118h]
  gentity_s *v6; // [esp+10h] [ebp-114h]
  rigid_body_constraint_distance *phys_constraint; // [esp+14h] [ebp-110h]
  float temp[3]; // [esp+18h] [ebp-10Ch] BYREF
  float v9; // [esp+24h] [ebp-100h]
  float rb2_anchor_loc[3]; // [esp+28h] [ebp-FCh] BYREF
  const phys_vec3 *v11; // [esp+44h] [ebp-E0h]
  phys_vec3 v12; // [esp+48h] [ebp-DCh] BYREF
  _BYTE v13[12]; // [esp+58h] [ebp-CCh] BYREF
  float v14[3]; // [esp+74h] [ebp-B0h] BYREF
  float ent2_anchor_loc[3]; // [esp+80h] [ebp-A4h]
  float v16; // [esp+8Ch] [ebp-98h] BYREF
  gentity_s *ent2; // [esp+90h] [ebp-94h]
  float v18; // [esp+94h] [ebp-90h]
  float rb1_anchor_loc[3]; // [esp+98h] [ebp-8Ch] BYREF
  phys_vec3 rb1_anchor_loc_; // [esp+A4h] [ebp-80h] BYREF
  phys_vec3 v21; // [esp+B8h] [ebp-6Ch] BYREF
  float *p_m_smallest_lambda; // [esp+D4h] [ebp-50h]
  _BYTE v23[12]; // [esp+D8h] [ebp-4Ch] BYREF
  float v24[3]; // [esp+F4h] [ebp-30h] BYREF
  float ent1_anchor_loc[3]; // [esp+100h] [ebp-24h]
  float x; // [esp+10Ch] [ebp-18h]
  gentity_s *ent1; // [esp+110h] [ebp-14h]
  rigid_body *rb2; // [esp+114h] [ebp-10h]
  rigid_body *rb1; // [esp+118h] [ebp-Ch] BYREF
  PhysObjUserData *userData2; // [esp+11Ch] [ebp-8h]
  PhysObjUserData *retaddr; // [esp+124h] [ebp+0h]

  rb1 = a1;
  userData2 = retaddr;
  rb2 = 0;
  ent1 = 0;
  x = COERCE_FLOAT(phys_sys::get_environment_rigid_body());
  LODWORD(ent1_anchor_loc[2]) = phys_sys::get_environment_rigid_body();
  if ( constraint->type == CONSTRAINT_ROPE )
  {
    if ( constraint->attach_point_type1 == ATTACH_POINT_ENT )
    {
      LODWORD(ent1_anchor_loc[1]) = GetEntity(&constraint->target_ent1);
      constraint->target_index1 = *(unsigned int *)LODWORD(ent1_anchor_loc[1]);
      if ( !*(unsigned int *)(LODWORD(ent1_anchor_loc[1]) + 740)
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_mover.cpp", 624, 0, "%s", "ent1->physObjId") )
      {
        __debugbreak();
      }
      rb2 = *(rigid_body **)(LODWORD(ent1_anchor_loc[1]) + 740);
      x = rb2->m_last_position.x;
      LODWORD(ent1_anchor_loc[0]) = constraint->pos;
      v24[0] = constraint->pos[0];
      v24[1] = constraint->pos[1];
      v24[2] = constraint->pos[2];
      Phys_Vec3ToNitrousVec(v24, (phys_vec3 *)v23);
      p_m_smallest_lambda = &rb2->m_smallest_lambda;
      max_distance = &rb2->m_mat.z;
      v2 = phys_multiply(&v21, (const phys_mat44 *)&rb2->m_smallest_lambda, (const phys_vec3 *)v23);
      operator+((phys_vec3 *)&rb1_anchor_loc_.y, v2, max_distance);
      rb1_anchor_loc[0] = rb1_anchor_loc_.y;
      rb1_anchor_loc[1] = rb1_anchor_loc_.z;
      rb1_anchor_loc[2] = rb1_anchor_loc_.w;
      Phys_NitrousVecToVec3((const phys_vec3 *)rb1_anchor_loc, &v16);
    }
    else
    {
      LODWORD(ent2_anchor_loc[2]) = constraint->pos;
      v16 = constraint->pos[0];
      ent2 = (gentity_s *)LODWORD(constraint->pos[1]);
      v18 = constraint->pos[2];
      Phys_Vec3ToNitrousVec(&v16, (phys_vec3 *)rb1_anchor_loc);
    }
    if ( constraint->attach_point_type2 == ATTACH_POINT_ENT )
    {
      LODWORD(ent2_anchor_loc[1]) = GetEntity(&constraint->target_ent2);
      constraint->target_index2 = *(unsigned int *)LODWORD(ent2_anchor_loc[1]);
      ent1 = *(gentity_s **)(LODWORD(ent2_anchor_loc[1]) + 740);
      ent1_anchor_loc[2] = rb2->m_last_position.x;
      LODWORD(ent2_anchor_loc[0]) = constraint->pos2;
      v14[0] = constraint->pos2[0];
      v14[1] = constraint->pos2[1];
      v14[2] = constraint->pos2[2];
      Phys_Vec3ToNitrousVec(v14, (phys_vec3 *)v13);
      v11 = phys_full_multiply((int)&rb1, &v12, (const phys_mat44 *)&ent1->s.lerp.pos.trBase[2], (const phys_vec3 *)v13);
      rb2_anchor_loc[0] = v11->x;
      rb2_anchor_loc[1] = v11->y;
      rb2_anchor_loc[2] = v11->z;
      Phys_NitrousVecToVec3((const phys_vec3 *)rb2_anchor_loc, &temp[1]);
    }
    else
    {
      temp[1] = constraint->pos2[0];
      temp[2] = constraint->pos2[1];
      v9 = constraint->pos2[2];
      Phys_Vec3ToNitrousVec(&temp[1], (phys_vec3 *)rb2_anchor_loc);
    }
    if ( (environment_rigid_body *)LODWORD(x) == phys_sys::get_environment_rigid_body() )
    {
      x = ent1_anchor_loc[2];
      LODWORD(ent1_anchor_loc[2]) = phys_sys::get_environment_rigid_body();
      v5 = temp[1];
      v6 = (gentity_s *)LODWORD(temp[2]);
      phys_constraint = (rigid_body_constraint_distance *)LODWORD(v9);
      temp[1] = v16;
      LODWORD(temp[2]) = ent2;
      v9 = v18;
      v16 = v5;
      ent2 = v6;
      v18 = *(float *)&phys_constraint;
    }
    rbc_dist = phys_sys::create_rbc_dist(
                 (environment_rigid_body *)LODWORD(x),
                 (environment_rigid_body *)LODWORD(ent1_anchor_loc[2]),
                 0);
    if ( rbc_dist )
    {
      rigid_body_constraint_distance::set(
        rbc_dist,
        (const phys_vec3 *)rb1_anchor_loc,
        (const phys_vec3 *)rb2_anchor_loc,
        0.0,
        constraint->distance);
      constraint->constraintHandle = (int)rbc_dist;
    }
  }
}

void __cdecl DeleteConstraint(phys_free_list<RagdollBody>::T_internal_base *rope_index)
{
  PhysConstraint *constraint; // [esp+4h] [ebp-8h]
  int i; // [esp+8h] [ebp-4h]

  for ( i = 0; i < cm.num_constraints; ++i )
  {
    constraint = &cm.constraints[i];
    if ( constraint->type == CONSTRAINT_ROPE
      && (phys_free_list<RagdollBody>::T_internal_base *)constraint->rope_index == rope_index
      && constraint->constraintHandle )
    {
      phys_sys::destroy((rigid_body_constraint_distance *const)constraint->constraintHandle);
      constraint->constraintHandle = 0;
      break;
    }
  }
  Ragdoll_RemoveConstraintsForRope(rope_index);
}

void __cdecl G_MoverTeam(gentity_s *ent, mover_info_t *mi)
{
  void (__cdecl *reached)(gentity_s *); // [esp+0h] [ebp-4h]
  void (__cdecl *reacheda)(gentity_s *); // [esp+0h] [ebp-4h]
  int savedregs; // [esp+4h] [ebp+0h] BYREF

  if ( ent->s.lerp.pos.trType == 10 )
  {
    GlassSv_PredictTouch(ent);
    G_CalcEntityPhysicsPositions(ent);
    SV_LinkEntity((int)&savedregs, ent);
  }
  else if ( !Com_IsRagdollTrajectory(&ent->s.lerp.pos) && ent->s.lerp.pos.trType != 11 )
  {
    G_MoverPush(ent, mi);
    if ( ent->s.lerp.pos.trType )
    {
      if ( ent->s.lerp.pos.trType != 10
        && ent->s.lerp.pos.trType != 2
        && level.time >= ent->s.lerp.pos.trDuration + ent->s.lerp.pos.trTime )
      {
        reached = (void (__cdecl *)(gentity_s *))dword_E07CD4[12 * ent->handler];
        if ( reached )
          reached(ent);
      }
    }
    if ( ent->s.lerp.apos.trType
      && ent->s.lerp.pos.trType != 10
      && ent->s.lerp.apos.trType != 2
      && level.time >= ent->s.lerp.apos.trDuration + ent->s.lerp.apos.trTime )
    {
      reacheda = (void (__cdecl *)(gentity_s *))dword_E07CD4[12 * ent->handler];
      if ( reacheda )
        reacheda(ent);
    }
  }
}

void __cdecl G_MoverPush(gentity_s *pusher, mover_info_t *mi)
{
  float outMaxs[3]; // [esp+28h] [ebp-108Ch] BYREF
  float outMins[3]; // [esp+34h] [ebp-1080h] BYREF
  int v4; // [esp+40h] [ebp-1074h]
  int j; // [esp+44h] [ebp-1070h]
  float maxs[3]; // [esp+48h] [ebp-106Ch] BYREF
  float v7[3]; // [esp+54h] [ebp-1060h] BYREF
  float v8[3]; // [esp+60h] [ebp-1054h] BYREF
  float move; // [esp+6Ch] [ebp-1048h] BYREF
  float v10; // [esp+70h] [ebp-1044h]
  float v11; // [esp+74h] [ebp-1040h]
  float v12[3]; // [esp+78h] [ebp-103Ch]
  float __formal; // [esp+84h] [ebp-1030h] BYREF
  float v14; // [esp+88h] [ebp-102Ch]
  float v15; // [esp+8Ch] [ebp-1028h]
  float mins[3]; // [esp+90h] [ebp-1024h] BYREF
  float v17; // [esp+9Ch] [ebp-1018h]
  float v18[3]; // [esp+A0h] [ebp-1014h]
  int entityList[1024]; // [esp+ACh] [ebp-1008h] BYREF
  int i; // [esp+10ACh] [ebp-8h]
  gentity_s *other; // [esp+10B0h] [ebp-4h]
  int savedregs; // [esp+10B4h] [ebp+0h] BYREF

  move = mi->m_origin[0] - mi->m_prev_origin[0];
  v10 = mi->m_origin[1] - mi->m_prev_origin[1];
  v11 = mi->m_origin[2] - mi->m_prev_origin[2];
  __formal = mi->m_angles[0] - mi->m_prev_angles[0];
  v14 = mi->m_angles[1] - mi->m_prev_angles[1];
  v15 = mi->m_angles[2] - mi->m_prev_angles[2];
  mins[0] = pusher->r.mins[0];
  mins[1] = pusher->r.mins[1];
  mins[2] = pusher->r.mins[2];
  maxs[0] = pusher->r.maxs[0];
  maxs[1] = pusher->r.maxs[1];
  maxs[2] = pusher->r.maxs[2];
  parented_pathnode_list_update((generic_avl_map_node_t *)&savedregs, pusher, &mi->m_mat);
  if ( pusher->s.eType == 6 && pusher->model && G_GetModelBounds(pusher->model, outMins, outMaxs) )
  {
    for ( i = 0; i < 3; ++i )
    {
      if ( mins[i] > outMins[i] )
        mins[i] = outMins[i];
      if ( outMaxs[i] > maxs[i] )
        maxs[i] = outMaxs[i];
    }
  }
  if ( pusher->r.currentAngles[0] == 0.0
    && pusher->r.currentAngles[1] == 0.0
    && pusher->r.currentAngles[2] == 0.0
    && __formal == 0.0
    && v14 == 0.0
    && v15 == 0.0 )
  {
    for ( i = 0; i < 3; ++i )
    {
      v8[i] = (float)(pusher->r.currentOrigin[i] + mins[i]) - 1.0;
      v7[i] = (float)(pusher->r.currentOrigin[i] + maxs[i]) + 1.0;
    }
  }
  else
  {
    v17 = RadiusFromBounds(mins, maxs);
    for ( i = 0; i < 3; ++i )
    {
      v8[i] = pusher->r.currentOrigin[i] - v17;
      v7[i] = pusher->r.currentOrigin[i] + v17;
    }
  }
  for ( i = 0; i < 3; ++i )
  {
    v12[i] = v8[i] + *(&move + i);
    v18[i] = v7[i] + *(&move + i);
    if ( *(&move + i) <= 0.0 )
      v8[i] = v8[i] + *(&move + i);
    else
      v7[i] = v7[i] + *(&move + i);
  }
  SV_UnlinkEntity(pusher);
  v4 = CM_AreaEntities(v8, v7, entityList, 1024, 100696448);
  pusher->r.currentOrigin[0] = pusher->r.currentOrigin[0] + move;
  pusher->r.currentOrigin[1] = pusher->r.currentOrigin[1] + v10;
  pusher->r.currentOrigin[2] = pusher->r.currentOrigin[2] + v11;
  pusher->r.currentAngles[0] = pusher->r.currentAngles[0] + __formal;
  pusher->r.currentAngles[1] = pusher->r.currentAngles[1] + v14;
  pusher->r.currentAngles[2] = pusher->r.currentAngles[2] + v15;
  SV_LinkEntity((int)&savedregs, pusher);
  for ( j = 0; j < v4; ++j )
  {
    other = &g_entities[entityList[j]];
    if ( (other->s.eType == 4 || other->s.eType == 3 || other->s.eType == 1 || other->physicsObject)
      && !IsEntityLinkedToMe(pusher, other)
      && !other->tagInfo
      && other->s.groundEntityNum == pusher->s.number )
    {
      G_PushEntity(other, pusher, &move, &__formal, mi);
    }
  }
}

void __cdecl G_CalcEntityPhysicsPositions(gentity_s *ent)
{
  if ( !ent && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\game\\g_mover.cpp", 719, 0, "%s", "ent") )
    __debugbreak();
  if ( (ent->s.lerp.pos.trType != 10 || ent->s.lerp.apos.trType != 10)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_mover.cpp",
          720,
          0,
          "%s",
          "ent->s.lerp.pos.trType == TR_PHYSICS && ent->s.lerp.apos.trType == TR_PHYSICS") )
  {
    __debugbreak();
  }
  if ( !ent->physObjId )
  {
    G_CreatePhysicsObject(ent);
    if ( ent->physObjId != -1 )
    {
      *(_WORD *)(ent->physObjId + 244) = 0;
      ent->s.lerp.pos.trDuration = 0;
    }
  }
  if ( ent->physObjId == -1 )
  {
    ent->s.lerp.pos.trType = 0;
    ent->s.lerp.apos.trType = 0;
    Scr_Notify(ent, scr_const.stationary, 0);
  }
  else
  {
    G_UpdatePhysicsPose(ent);
    ent->s.lerp.pos.trBase[0] = ent->r.currentOrigin[0];
    ent->s.lerp.pos.trBase[1] = ent->r.currentOrigin[1];
    ent->s.lerp.pos.trBase[2] = ent->r.currentOrigin[2];
    ent->s.lerp.apos.trBase[0] = ent->r.currentAngles[0];
    ent->s.lerp.apos.trBase[1] = ent->r.currentAngles[1];
    ent->s.lerp.apos.trBase[2] = ent->r.currentAngles[2];
    if ( ent->physObjId )
    {
      if ( Phys_ObjIsAsleep(ent->physObjId) )
      {
        Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
        Phys_ObjDestroy(0, ent->physObjId);
        ent->physObjId = 0;
        Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
        ent->s.lerp.pos.trType = 0;
        ent->s.lerp.apos.trType = 0;
        ent->s.groundEntityNum = 1022;
        ent->s.lerp.pos.trBase[0] = ent->r.currentOrigin[0];
        ent->s.lerp.pos.trBase[1] = ent->r.currentOrigin[1];
        ent->s.lerp.pos.trBase[2] = ent->r.currentOrigin[2];
        ent->s.lerp.apos.trBase[0] = ent->r.currentAngles[0];
        ent->s.lerp.apos.trBase[1] = ent->r.currentAngles[1];
        ent->s.lerp.apos.trBase[2] = ent->r.currentAngles[2];
        Scr_Notify(ent, scr_const.stationary, 0);
      }
    }
    ent->s.lerp.pos.trBase[0] = (float)(int)ent->s.lerp.pos.trBase[0];
    ent->s.lerp.pos.trBase[1] = (float)(int)ent->s.lerp.pos.trBase[1];
    ent->s.lerp.pos.trBase[2] = (float)(int)ent->s.lerp.pos.trBase[2];
  }
}

void __cdecl G_CreatePhysicsObject(gentity_s *ent)
{
  const char *Name; // eax
  const char *v2; // [esp+8h] [ebp-120h]
  int surfFlags; // [esp+Ch] [ebp-11Ch]
  PhysConstraint *constraint; // [esp+28h] [ebp-100h]
  int i; // [esp+2Ch] [ebp-FCh]
  XModel *model; // [esp+34h] [ebp-F4h]
  float v7[3]; // [esp+38h] [ebp-F0h] BYREF
  int surfaceFlags; // [esp+44h] [ebp-E4h]
  float dims[3]; // [esp+48h] [ebp-E0h] BYREF
  int surfaceType; // [esp+54h] [ebp-D4h]
  PhysPreset defaultPreset; // [esp+58h] [ebp-D0h] BYREF
  float velocity[3]; // [esp+B4h] [ebp-74h] BYREF
  int physObjId; // [esp+C0h] [ebp-68h]
  DObj *obj; // [esp+C4h] [ebp-64h]
  PhysPreset *physPreset; // [esp+C8h] [ebp-60h]
  float mins[3]; // [esp+CCh] [ebp-5Ch] BYREF
  PhysObjUserData *userData; // [esp+D8h] [ebp-50h]
  float quat[4]; // [esp+DCh] [ebp-4Ch] BYREF
  int brushModel; // [esp+ECh] [ebp-3Ch]
  float maxs[3]; // [esp+F0h] [ebp-38h] BYREF
  create_gjk_geom_collision_visitor collision_visitor; // [esp+FCh] [ebp-2Ch] BYREF
  float speed; // [esp+104h] [ebp-24h]
  gjk_geom_list_t gjk_geom_list; // [esp+108h] [ebp-20h] BYREF
  float direction[3]; // [esp+110h] [ebp-18h] BYREF
  float position[3]; // [esp+11Ch] [ebp-Ch] BYREF
  int savedregs; // [esp+128h] [ebp+0h] BYREF

  Sys_EnterCriticalSection(CRITSECT_PHYSICS_UPDATE);
  obj = 0;
  physPreset = 0;
  brushModel = 0;
  memset(velocity, 0, sizeof(velocity));
  position[0] = ent->s.lerp.pos.trBase[0];
  position[1] = ent->s.lerp.pos.trBase[1];
  position[2] = ent->s.lerp.pos.trBase[2];
  velocity[0] = ent->s.lerp.apos.trDelta[0];
  velocity[1] = ent->s.lerp.apos.trDelta[1];
  velocity[2] = ent->s.lerp.apos.trDelta[2];
  AnglesToQuat(ent->s.lerp.apos.trBase, quat);
  memset((unsigned __int8 *)&defaultPreset, 0, sizeof(defaultPreset));
  if ( ent->classname == scr_const.script_brushmodel )
  {
    brushModel = ent->s.index.brushmodel;
    physPreset = &defaultPreset;
    CM_ModelBounds(brushModel, mins, maxs);
    dims[0] = maxs[0] - mins[0];
    dims[1] = maxs[1] - mins[1];
    dims[2] = maxs[2] - mins[2];
    surfaceFlags = CM_ModelSurfaceFlags(brushModel);
    surfaceType = (unsigned __int8)((int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20] & surfaceFlags) >> 20);
    Phys_CalcPreset(&defaultPreset, dims, surfaceType);
  }
  else
  {
    obj = Com_GetServerDObj(ent->s.number);
    if ( !obj
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_mover.cpp",
            515,
            0,
            "%s\n\t%s",
            "obj",
            "Trying to put an entity in physics with no model (no dobj).") )
    {
      __debugbreak();
    }
    if ( !obj )
    {
      ent->physObjId = -1;
      Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
      return;
    }
    DObjPhysicsGetBounds(obj, mins, maxs);
    v7[0] = maxs[0] - mins[0];
    v7[1] = maxs[1] - mins[1];
    v7[2] = maxs[2] - mins[2];
    physPreset = DObjGetPhysPreset(obj);
    if ( !physPreset )
    {
      model = DObjGetModel(obj, 0);
      if ( model )
      {
        if ( model->collSurfs )
          surfFlags = model->collSurfs->surfFlags;
        else
          surfFlags = 13631488;
        Phys_CalcPreset(
          &defaultPreset,
          v7,
          (unsigned __int8)((int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20] & surfFlags) >> 20));
        physPreset = &defaultPreset;
      }
    }
  }
  if ( physPreset )
  {
    Sys_EnterCriticalSection(CRITSECT_PHYSICS);
    gjk_geom_list.m_first_geom = 0;
    gjk_geom_list.m_geom_count = 0;
    collision_visitor.__vftable = (create_gjk_geom_collision_visitor_vtbl *)&create_gjk_geom_collision_visitor::`vftable';
    collision_visitor.gjk_geom_list = &gjk_geom_list;
    create_gjk_geom(ent, &collision_visitor, 0, (unsigned int)&cls.recentServers[7546].city[57], 1);
    physObjId = (int)Phys_ObjCreate(0, position, quat, velocity, physPreset, &gjk_geom_list, 1, ent->s.number);
    ent->physObjId = physObjId;
    if ( physObjId )
    {
      userData = (PhysObjUserData *)physObjId;
      if ( *(unsigned int *)(physObjId + 224) == 1 )
      {
        direction[0] = ent->s.lerp.apos.trDelta[0];
        direction[1] = ent->s.lerp.apos.trDelta[1];
        direction[2] = ent->s.lerp.apos.trDelta[2];
        speed = Vec3Normalize(direction);
        Phys_ObjBulletImpact(physObjId, ent->s.lerp.pos.trDelta, direction, speed, physPreset->bulletForceScale);
      }
      for ( i = 0; i < cm.num_constraints; ++i )
      {
        constraint = &cm.constraints[i];
        if ( constraint->attach_point_type1 == ATTACH_POINT_ENT && constraint->target_ent1 == ent->targetname
          || constraint->attach_point_type2 == ATTACH_POINT_ENT && constraint->target_ent2 == ent->targetname )
        {
          CreateConstraint((rigid_body *)&savedregs, constraint);
        }
      }
      Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
      Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
    }
    else
    {
      Name = DObjGetName(obj);
      Com_PrintWarning(1, "Failed to create physics object for '%s'.\n", Name);
      ent->physObjId = -1;
      Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
      Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
    }
  }
  else
  {
    ent->physObjId = -1;
    if ( obj )
    {
      v2 = DObjGetName(obj);
      Com_PrintWarning(1, "Failed to create physics object for '%s'.  No physics preset.\n", v2);
    }
    else
    {
      Com_PrintWarning(1, "Failed to create physics object for '%s'.  No physics preset.\n", "sbmodel");
    }
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS_UPDATE);
  }
}

void __cdecl G_UpdatePhysicsPose(gentity_s *ent)
{
  float quat[4]; // [esp+0h] [ebp-10h] BYREF

  if ( (!ent->physObjId || ent->physObjId == -1)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_mover.cpp",
          708,
          0,
          "%s",
          "ent->physObjId != PHYS_OBJ_ID_NULL && ent->physObjId != PHYS_OBJ_ID_DEAD") )
  {
    __debugbreak();
  }
  Sys_EnterCriticalSection(CRITSECT_PHYSICS);
  Phys_ObjGetInterpolatedState(ent->physObjId, ent->r.currentOrigin, quat);
  Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
  UnitQuatToAngles(quat, ent->r.currentAngles);
}

void __cdecl G_MoverTeam_New(gentity_s *ent)
{
  float *v1; // [esp+68h] [ebp-9Ch]
  float *v2; // [esp+70h] [ebp-94h]
  float *v3; // [esp+78h] [ebp-8Ch]
  float *v4; // [esp+80h] [ebp-84h]
  float *m_prev_angles; // [esp+88h] [ebp-7Ch]
  float *m_angles; // [esp+8Ch] [ebp-78h]
  float *m_prev_origin; // [esp+94h] [ebp-70h]
  float *m_origin; // [esp+98h] [ebp-6Ch]
  mover_info_t *mover_info; // [esp+C8h] [ebp-3Ch]
  float origin[3]; // [esp+CCh] [ebp-38h] BYREF
  float angles[3]; // [esp+D8h] [ebp-2Ch] BYREF
  mover_info_t *mi; // [esp+E4h] [ebp-20h]
  float save_pos_delta[3]; // [esp+E8h] [ebp-1Ch]
  float save_apos_delta[3]; // [esp+F4h] [ebp-10h]
  unsigned __int8 save_apos_tr_type; // [esp+102h] [ebp-2h]
  unsigned __int8 save_pos_tr_type; // [esp+103h] [ebp-1h]

  if ( ent->s.lerp.pos.trType || ent->s.lerp.apos.trType )
  {
    if ( ent->s.lerp.pos.trType == 1 && ent->s.lerp.apos.trType == 1 )
    {
      mi = get_mover_info(ent);
      if ( mi )
      {
        mover_info_t::update(mi, ent->r.currentOrigin, ent->r.currentAngles);
      }
      else
      {
        mi = create_mover_info(ent);
        mover_info_t::init(mi, ent->r.currentOrigin, ent->r.currentAngles, 0);
      }
      save_pos_delta[0] = ent->s.lerp.pos.trDelta[0];
      save_pos_delta[1] = ent->s.lerp.pos.trDelta[1];
      save_pos_delta[2] = ent->s.lerp.pos.trDelta[2];
      save_apos_delta[0] = ent->s.lerp.apos.trDelta[0];
      save_apos_delta[1] = ent->s.lerp.apos.trDelta[1];
      save_apos_delta[2] = ent->s.lerp.apos.trDelta[2];
      m_prev_origin = mi->m_prev_origin;
      m_origin = mi->m_origin;
      ent->s.lerp.pos.trDelta[0] = mi->m_origin[0] - mi->m_prev_origin[0];
      ent->s.lerp.pos.trDelta[1] = m_origin[1] - m_prev_origin[1];
      ent->s.lerp.pos.trDelta[2] = m_origin[2] - m_prev_origin[2];
      m_prev_angles = mi->m_prev_angles;
      m_angles = mi->m_angles;
      ent->s.lerp.apos.trDelta[0] = mi->m_angles[0] - mi->m_prev_angles[0];
      ent->s.lerp.apos.trDelta[1] = m_angles[1] - m_prev_angles[1];
      ent->s.lerp.apos.trDelta[2] = m_angles[2] - m_prev_angles[2];
      v4 = mi->m_prev_origin;
      ent->r.currentOrigin[0] = mi->m_prev_origin[0];
      ent->r.currentOrigin[1] = v4[1];
      ent->r.currentOrigin[2] = v4[2];
      v3 = mi->m_prev_angles;
      ent->r.currentAngles[0] = mi->m_prev_angles[0];
      ent->r.currentAngles[1] = v3[1];
      ent->r.currentAngles[2] = v3[2];
      save_pos_tr_type = ent->s.lerp.pos.trType;
      save_apos_tr_type = ent->s.lerp.apos.trType;
      ent->s.lerp.pos.trType = 2;
      ent->s.lerp.apos.trType = 2;
      G_MoverTeam(ent, mi);
      ent->s.lerp.pos.trType = save_pos_tr_type;
      ent->s.lerp.apos.trType = save_apos_tr_type;
      v2 = mi->m_origin;
      ent->r.currentOrigin[0] = mi->m_origin[0];
      ent->r.currentOrigin[1] = v2[1];
      ent->r.currentOrigin[2] = v2[2];
      v1 = mi->m_angles;
      ent->r.currentAngles[0] = mi->m_angles[0];
      ent->r.currentAngles[1] = v1[1];
      ent->r.currentAngles[2] = v1[2];
      ent->s.lerp.pos.trDelta[0] = save_pos_delta[0];
      ent->s.lerp.pos.trDelta[1] = save_pos_delta[1];
      ent->s.lerp.pos.trDelta[2] = save_pos_delta[2];
      ent->s.lerp.apos.trDelta[0] = save_apos_delta[0];
      ent->s.lerp.apos.trDelta[1] = save_apos_delta[1];
      ent->s.lerp.apos.trDelta[2] = save_apos_delta[2];
    }
    else
    {
      mover_info = get_mover_info(ent);
      if ( !mover_info )
        mover_info = create_mover_info(ent);
      BG_EvaluateTrajectory(&ent->s.lerp.pos, level.time, origin);
      BG_EvaluateTrajectory(&ent->s.lerp.apos, level.time, angles);
      mover_info_t::init(mover_info, origin, angles, ent->r.currentOrigin, ent->r.currentAngles, 1);
      G_MoverTeam(ent, mover_info);
    }
  }
}

void __thiscall mover_info_t::init(mover_info_t *this, float *origin, const float *angles, bool do_collision)
{
  float axis[9]; // [esp+24h] [ebp-24h] BYREF

  AnglesToAxis(angles, (float (*)[3])axis);
  Phys_AxisToNitrousMat((float (*)[3])axis, &this->m_mat);
  Phys_Vec3ToNitrousVec(origin, &this->m_mat.w);
  phys_mat44::operator=(&this->m_prev_mat, &this->m_mat);
  this->m_origin[0] = *origin;
  this->m_origin[1] = origin[1];
  this->m_origin[2] = origin[2];
  this->m_prev_origin[0] = *origin;
  this->m_prev_origin[1] = origin[1];
  this->m_prev_origin[2] = origin[2];
  this->m_angles[0] = *angles;
  this->m_angles[1] = angles[1];
  this->m_angles[2] = angles[2];
  this->m_prev_angles[0] = *angles;
  this->m_prev_angles[1] = angles[1];
  this->m_prev_angles[2] = angles[2];
  this->m_do_collision = do_collision;
}

void __thiscall mover_info_t::init(
        mover_info_t *this,
        float *origin,
        const float *angles,
        float *prev_origin,
        const float *prev_angles,
        bool do_collision)
{
  float v7[9]; // [esp+18h] [ebp-48h] BYREF
  float axis[9]; // [esp+3Ch] [ebp-24h] BYREF

  AnglesToAxis(angles, (float (*)[3])axis);
  Phys_AxisToNitrousMat((float (*)[3])axis, &this->m_mat);
  Phys_Vec3ToNitrousVec(origin, &this->m_mat.w);
  AnglesToAxis(prev_angles, (float (*)[3])v7);
  Phys_AxisToNitrousMat((float (*)[3])v7, &this->m_prev_mat);
  Phys_Vec3ToNitrousVec(prev_origin, &this->m_prev_mat.w);
  this->m_origin[0] = *origin;
  this->m_origin[1] = origin[1];
  this->m_origin[2] = origin[2];
  this->m_prev_origin[0] = *prev_origin;
  this->m_prev_origin[1] = prev_origin[1];
  this->m_prev_origin[2] = prev_origin[2];
  this->m_angles[0] = *angles;
  this->m_angles[1] = angles[1];
  this->m_angles[2] = angles[2];
  this->m_prev_angles[0] = *prev_angles;
  this->m_prev_angles[1] = prev_angles[1];
  this->m_prev_angles[2] = prev_angles[2];
  this->m_do_collision = do_collision;
}

void __thiscall mover_info_t::update(mover_info_t *this, float *origin, const float *angles)
{
  float axis[13]; // [esp+1Ch] [ebp-34h] BYREF

  phys_mat44::operator=(&this->m_prev_mat, &this->m_mat);
  AnglesToAxis(angles, (float (*)[3])axis);
  Phys_AxisToNitrousMat((float (*)[3])axis, &this->m_mat);
  Phys_Vec3ToNitrousVec(origin, &this->m_mat.w);
  this->m_prev_origin[0] = this->m_origin[0];
  this->m_prev_origin[1] = this->m_origin[1];
  this->m_prev_origin[2] = this->m_origin[2];
  this->m_origin[0] = *origin;
  this->m_origin[1] = origin[1];
  this->m_origin[2] = origin[2];
  this->m_prev_angles[0] = this->m_angles[0];
  this->m_prev_angles[1] = this->m_angles[1];
  this->m_prev_angles[2] = this->m_angles[2];
  this->m_angles[0] = *angles;
  this->m_angles[1] = angles[1];
  this->m_angles[2] = angles[2];
}

void __cdecl G_RunMover(gentity_s *ent)
{
  float *v1; // [esp+4h] [ebp-4h]
  int savedregs; // [esp+8h] [ebp+0h] BYREF

  if ( ent->tagInfo )
  {
    if ( !zombietron->current.enabled || ent->s.lerp.apos.trType == 1 )
    {
      G_GeneralLink(ent);
    }
    else
    {
      G_SetFixedLink(ent, 2);
      G_SetOrigin(ent, ent->r.currentOrigin);
      ent->s.lerp.pos.trType = 1;
      SV_LinkEntity((int)&savedregs, ent);
      v1 = ent->tagInfo->axis[3];
      ent->s.lerp.pos.trDelta[0] = *v1;
      ent->s.lerp.pos.trDelta[1] = v1[1];
      ent->s.lerp.pos.trDelta[2] = v1[2];
    }
  }
  G_MoverTeam_New(ent);
  G_RunThink(ent);
}

void __cdecl trigger_use_shared(gentity_s *self, SpawnVar *spawnVar)
{
  char v2; // cl
  char *v3; // [esp+34h] [ebp-820h]
  const char *v4; // [esp+38h] [ebp-81Ch]
  char cursorHintStr[1024]; // [esp+44h] [ebp-810h] BYREF
  char szConfigString[1028]; // [esp+444h] [ebp-410h] BYREF
  const char *cursorhint; // [esp+84Ch] [ebp-8h] BYREF
  unsigned int i; // [esp+850h] [ebp-4h]
  int savedregs; // [esp+854h] [ebp+0h] BYREF

  if ( self->s.eType == 4
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\game\\g_mover.cpp",
          1065,
          0,
          "%s",
          "self->s.eType != ET_MISSILE") )
  {
    __debugbreak();
  }
  if ( SV_SetBrushModel(self) )
  {
    self->r.contents = 0x200000;
    self->trigger.perk = 52;
    self->s.otherEntityNum = 1023;
    SV_LinkEntity((int)&savedregs, self);
    self->item[1].ammoCount = 1023;
    self->s.lerp.pos.trType = 0;
    self->s.lerp.pos.trBase[0] = self->r.currentOrigin[0];
    self->s.lerp.pos.trBase[1] = self->r.currentOrigin[1];
    self->s.lerp.pos.trBase[2] = self->r.currentOrigin[2];
    self->r.svFlags = 1;
    self->s.lerp.eFlags |= 1u;
    if ( !self->model )
      self->s.lerp.eFlags |= 0x20u;
    self->handler = 22;
    self->s.un3.item = 3;
    if ( G_SpawnString(spawnVar, "cursorhint", &toastPopupTitle, &cursorhint) )
    {
      if ( I_stricmp(cursorhint, "HINT_INHERIT") )
      {
        for ( i = 1; i < 8; ++i )
        {
          if ( !I_stricmp(cursorhint, hintStrings[i]) )
          {
            self->s.un3.item = i;
            break;
          }
        }
      }
      else
      {
        self->s.un3.item = -1;
      }
    }
    self->s.un1.scale = -1;
    if ( G_SpawnString(spawnVar, "hintstring", &toastPopupTitle, &cursorhint) )
    {
      v4 = cursorhint;
      v3 = cursorHintStr;
      do
      {
        v2 = *v4;
        *v3++ = *v4++;
      }
      while ( v2 );
      for ( i = 0; i < 0x60; ++i )
      {
        SV_GetConfigstring(i + 419, szConfigString, 1024);
        if ( !szConfigString[0] )
        {
          SV_SetConfigstring(i + 419, cursorHintStr);
          self->s.un1.scale = i;
          break;
        }
        if ( !strcmp(cursorHintStr, szConfigString) )
        {
          self->s.un1.scale = i;
          break;
        }
      }
      if ( i == 96 )
        Com_Error(ERR_DROP, &byte_CBF558, 96);
    }
  }
  else
  {
    Com_PrintError(
      1,
      "Killing trigger_use_shared at (%f %f %f) because the brush model is invalid.\n",
      self->s.lerp.pos.trBase[0],
      self->s.lerp.pos.trBase[1],
      self->s.lerp.pos.trBase[2]);
    G_FreeEntity(self);
  }
}

void __cdecl trigger_use_touch(gentity_s *ent, SpawnVar *spawnVar)
{
  trigger_use_shared(ent, spawnVar);
}

mover_info_t *__thiscall phys_simple_allocator<mover_info_t>::allocate(phys_simple_allocator<mover_info_t> *this)
{
  char *slot; // [esp+18h] [ebp-4h]

  slot = PMM_ALLOC(0xC0u, 0x10u);
  if ( !slot )
    return 0;
  ++this->m_count;
  return (mover_info_t *)slot;
}

gentity_s *__cdecl GetEntity(scr_entref_t entref)
{
  if ( entref.classnum )
  {
    Scr_ObjectError("not an entity", SCRIPTINSTANCE_SERVER);
    return 0;
  }
  else
  {
    if ( entref.entnum >= 0x400u
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\game_mp\\g_scr_main_mp.cpp",
            449,
            0,
            "%s",
            "entref.entnum < MAX_GENTITIES") )
    {
      __debugbreak();
    }
    return &g_entities[entref.entnum];
  }
}

