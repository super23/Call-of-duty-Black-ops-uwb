#include "xdoll.h"

bool __cdecl XDoll_IsXDollConstraint(ConstraintType type)
{
  return type >= CONSTRAINT_POINT && type <= CONSTRAINT_ACTUATOR;
}

void __cdecl XDoll_SetCollides(XDollBone *bone, bool collides)
{
  unsigned int m_env_collision_flags; // [esp+8h] [ebp-18h]
  unsigned int v3; // [esp+8h] [ebp-18h]
  broad_phase_info *i; // [esp+Ch] [ebp-14h]
  broad_phase_info *bpi; // [esp+14h] [ebp-Ch]
  int flags; // [esp+18h] [ebp-8h]
  int flagsa; // [esp+18h] [ebp-8h]
  PhysObjUserData *userData; // [esp+1Ch] [ebp-4h]

  if ( bone->rigidBody )
  {
    userData = (PhysObjUserData *)bone->rigidBody;
    if ( (userData->m_bpb->m_flags & 1) != 0 )
    {
      bpi = broad_phase_base::get_bpi(userData->m_bpb);
      flags = bpi->m_env_collision_flags;
      if ( collides )
        flagsa = flags | 3;
      else
        flagsa = flags & 0xFFFFFFFC;
      bpi->m_env_collision_flags = flagsa;
    }
    else
    {
      for ( i = broad_phase_base::get_bpg(userData->m_bpb)->m_list_bpi_head; i; i = (broad_phase_info *)i->m_list_bpb_next )
      {
        m_env_collision_flags = i->m_env_collision_flags;
        if ( collides )
          v3 = m_env_collision_flags | 3;
        else
          v3 = m_env_collision_flags & 0xFFFFFFFC;
        i->m_env_collision_flags = v3;
      }
    }
  }
  if ( collides )
    bone->flags |= 2u;
  else
    bone->flags &= ~2u;
}

void __cdecl XDoll_SetCollides(XDollBody *body, bool collides)
{
  int i; // [esp+0h] [ebp-4h]

  for ( i = 0; i < body->numBones; ++i )
    XDoll_SetCollides(&body->bones[i], collides);
}

void __cdecl XDoll_SetCollides(int xdoll_handle, bool collides)
{
  XDollBody *body; // [esp+0h] [ebp-4h]

  if ( xdoll_handle )
  {
    body = XDoll_HandleBody(xdoll_handle);
    XDoll_SetCollides(body, collides);
  }
}

int __cdecl XDoll_GetStableTime(int xdoll_handle)
{
  return XDoll_HandleBody(xdoll_handle)->stableTime;
}

void __cdecl XDoll_UpdateStability(XDollBody *body, int msec)
{
  int ci; // [esp+4h] [ebp-14h]
  bool hasConstraints; // [esp+Bh] [ebp-Dh]
  int bi; // [esp+10h] [ebp-8h]
  bool allStable; // [esp+17h] [ebp-1h]

  allStable = 1;
  for ( bi = 0; bi < body->numBones; ++bi )
  {
    if ( body->bones[bi].rigidBody )
    {
      hasConstraints = 0;
      for ( ci = 0; ci < 4; ++ci )
      {
        if ( body->constraintInfos[ci].constraint )
        {
          hasConstraints = 1;
          break;
        }
      }
      if ( hasConstraints || !Phys_ObjIsAsleep(body->bones[bi].rigidBody) )
      {
        allStable = 0;
      }
      else
      {
        Phys_ObjDestroy(0, body->bones[bi].rigidBody);
        body->bones[bi].rigidBody = 0;
        body->bones[bi].flags |= 1u;
      }
    }
  }
  if ( allStable )
    body->stableTime += msec;
  else
    body->stableTime = 0;
}

void __cdecl XDoll_Update(int msec)
{
  XDollBoneOrientation *v1; // eax
  DObj *obj; // [esp+8h] [ebp-40h]
  float origin[3]; // [esp+Ch] [ebp-3Ch] BYREF
  centity_s *cent; // [esp+18h] [ebp-30h]
  float mat[3][3]; // [esp+1Ch] [ebp-2Ch] BYREF
  XDollBody *body; // [esp+40h] [ebp-8h]
  int i; // [esp+44h] [ebp-4h]
  int savedregs; // [esp+48h] [ebp+0h] BYREF

  if ( !Sys_IsMainThread()
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\xdoll.cpp", 412, 0, "%s", "Sys_IsMainThread()") )
  {
    __debugbreak();
  }
  xdollTime += msec;
  for ( i = 0; i < 128; ++i )
  {
    body = &xdollBodies[i];
    if ( body->references > 0 )
    {
      v1 = XDoll_BodyBoneOrientations(body);
      XDoll_SnapshotBonePositions(body, v1);
      XDoll_UpdateTimers(body, msec);
      XDoll_UpdateStability(body, msec);
      if ( body->userBody )
      {
        cent = CG_GetEntity(body->client_index, body->entity_index);
        obj = Com_GetClientDObj(body->entity_index, body->client_index);
        if ( ((*((unsigned int *)cent + 201) >> 1) & 1) != 0 && cent->destructible && obj )
        {
          if ( body->userBodyBoneHash )
          {
            if ( CG_DObjGetWorldTagMatrix(&cent->pose, obj, body->userBodyBoneHash, mat, origin) )
              Phys_SetUserBody((int)&savedregs, body->userBody, origin);
          }
        }
        else
        {
          XDoll_DestroyXDoll(body);
        }
      }
    }
  }
}

void __cdecl XDoll_SnapshotBonePositions(XDollBody *body, XDollBoneOrientation *boneSnapshot)
{
  int i; // [esp+0h] [ebp-Ch]
  XDollBoneOrientation *snapshot; // [esp+4h] [ebp-8h]
  XDollBone *bone; // [esp+8h] [ebp-4h]

  if ( !boneSnapshot
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\xdoll.cpp", 100, 0, "%s", "boneSnapshot") )
  {
    __debugbreak();
  }
  bone = body->bones;
  snapshot = boneSnapshot;
  if ( body->numBones >= 4
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\xdoll.cpp",
          104,
          0,
          "%s",
          "body.numBones < XDOLL_MAX_BONES") )
  {
    __debugbreak();
  }
  for ( i = 0; i < body->numBones; ++i )
  {
    if ( bone->rigidBody )
      Phys_ObjGetInterpolatedState(bone->rigidBody, snapshot->origin, snapshot->orientation);
    ++bone;
    ++snapshot;
  }
}

void __cdecl XDoll_UpdateTimers(XDollBody *body, int msec)
{
  unsigned int ci; // [esp+8h] [ebp-8h]
  PhysConstraints *constraintsDef; // [esp+Ch] [ebp-4h]

  if ( body->constraintsDef )
  {
    constraintsDef = body->constraintsDef;
    if ( constraintsDef->count >= 4
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\physics\\xdoll.cpp",
            352,
            0,
            "%s",
            "constraintsDef.count < XDOLL_MAX_CONSTRAINTS") )
    {
      __debugbreak();
    }
    for ( ci = 0; ci < constraintsDef->count; ++ci )
    {
      if ( body->constraintInfos[ci].constraint )
      {
        body->constraintInfos[ci].timer += msec;
        if ( constraintsDef->data[ci].timeout )
        {
          if ( body->constraintInfos[ci].timer > 1000 * constraintsDef->data[ci].timeout )
          {
            XDoll_DestroyConstraint(&body->constraintInfos[ci]);
            XDoll_SetCollides(body, 1);
          }
        }
      }
    }
  }
}

void __cdecl XDoll_DestroyConstraint(XDoll_ConstraintInfo *cinfo)
{
  int type; // [esp+0h] [ebp-4h]

  if ( cinfo->constraint )
  {
    type = cinfo->type;
    if ( cinfo->type == 1 )
    {
      phys_sys::destroy(cinfo->point_constraint);
    }
    else if ( type == 3 )
    {
      phys_sys::destroy(cinfo->hinge_constraint);
    }
    else if ( type == 5 )
    {
      phys_sys::destroy(cinfo->actuator_constraint);
    }
    cinfo->constraint = 0;
  }
}

void __cdecl XDoll_DestroyXDoll(int xdoll_handle)
{
  XDollBody *body; // [esp+0h] [ebp-4h]

  if ( xdoll_handle )
  {
    body = XDoll_HandleBody(xdoll_handle);
    XDoll_DestroyXDoll(body);
  }
}

void __cdecl XDoll_DestroyXDoll(XDollBody *body)
{
  unsigned int ci; // [esp+0h] [ebp-Ch]
  int bi; // [esp+8h] [ebp-4h]

  for ( bi = 0; bi < body->numBones; ++bi )
  {
    if ( body->bones[bi].rigidBody )
      Phys_ObjDestroy(0, body->bones[bi].rigidBody);
    body->bones[bi].rigidBody = 0;
    if ( body->constraintsDef )
    {
      for ( ci = 0; ci < body->constraintsDef->count; ++ci )
      {
        if ( body->constraintInfos[ci].constraint )
          body->constraintInfos[ci].constraint = 0;
      }
    }
  }
  if ( body->userBody )
  {
    Phys_ObjDestroy(0, body->userBody);
    body->userBody = 0;
  }
  body->references = 0;
}

int __cdecl XDoll_CreateXDollForBoneLaunch(
        int localClientNum,
        int entity_index,
        int piece_index,
        unsigned int bone_name,
        PhysPreset *physPreset)
{
  int bi; // [esp+Ch] [ebp-20h]
  XDollBody *v7; // [esp+10h] [ebp-1Ch]
  int i; // [esp+14h] [ebp-18h]
  const DObj *obj; // [esp+18h] [ebp-14h]
  XModel *model; // [esp+1Ch] [ebp-10h]
  int xdoll_handle; // [esp+20h] [ebp-Ch]
  centity_s *cent; // [esp+24h] [ebp-8h]
  XDollBody *body; // [esp+28h] [ebp-4h]

  obj = Com_GetClientDObj(entity_index, localClientNum);
  if ( !obj )
    return 0;
  model = DObjGetModel(obj, 0);
  if ( !physPreset )
    return 0;
  for ( i = 0; i < 128; ++i )
  {
    v7 = &xdollBodies[i];
    if ( v7->references > 0 && v7->piece_index == piece_index && v7->entity_index == entity_index )
    {
      ++xdollBodies[i].references;
      return i + 1;
    }
  }
  xdoll_handle = XDoll_GetUnusedBody();
  if ( !xdoll_handle )
    return 0;
  body = XDoll_HandleBody(xdoll_handle);
  body->constraintsDef = 0;
  body->numBones = 0;
  body->entity_index = entity_index;
  body->client_index = localClientNum;
  body->piece_index = piece_index;
  body->numBones = 1;
  body->bones[0].name_hash = bone_name;
  cent = CG_GetEntity(localClientNum, entity_index);
  for ( bi = 0; bi < body->numBones; ++bi )
  {
    DObjGetModelBoneIndex(obj, 0, body->bones[bi].name_hash, &body->bones[bi].index);
    XDoll_CreatePhysObj(body, &body->bones[bi], model, &cent->pose, physPreset, 1);
  }
  return xdoll_handle;
}

int __cdecl XDoll_GetUnusedBody()
{
  int i; // [esp+4h] [ebp-4h]

  for ( i = 0; i < 128; ++i )
  {
    if ( xdollBodies[i].references <= 0 )
    {
      XDoll_InitBody(&xdollBodies[i]);
      ++xdollBodies[i].references;
      return i + 1;
    }
  }
  return 0;
}

void __cdecl XDoll_InitBody(XDollBody *body)
{
  memset((unsigned __int8 *)body, 0, sizeof(XDollBody));
}

char __cdecl XDoll_CreatePhysObj(
        XDollBody *body,
        XDollBone *bone,
        XModel *model,
        const cpose_t *pose,
        PhysPreset *physPreset,
        bool collide)
{
  char *v6; // eax
  gjk_aabb_t *aabb_gjk_geom; // eax
  int surfFlags; // [esp+0h] [ebp-60h]
  DObj *obj; // [esp+8h] [ebp-58h]
  float origin[3]; // [esp+Ch] [ebp-54h] BYREF
  PhysObjUserData *userData; // [esp+18h] [ebp-48h]
  float quat[4]; // [esp+1Ch] [ebp-44h] BYREF
  float mat[3][3]; // [esp+2Ch] [ebp-34h] BYREF
  create_gjk_geom_collision_visitor collision_visitor; // [esp+50h] [ebp-10h] BYREF
  gjk_geom_list_t gjk_geom_list; // [esp+58h] [ebp-8h] BYREF
  int savedregs; // [esp+60h] [ebp+0h] BYREF

  obj = XDoll_BodyDObj(body, pose->localClientNum);
  if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\xdoll.cpp", 281, 0, "%s", "obj") )
    __debugbreak();
  if ( CG_DObjGetWorldTagMatrix(pose, obj, bone->name_hash, mat, origin) )
  {
    AxisToQuat(mat, quat);
    if ( !physPreset )
      Com_PrintWarning(14, "XDoll: Failed to model physPreset\n");
    gjk_geom_list.m_first_geom = 0;
    gjk_geom_list.m_geom_count = 0;
    collision_visitor.__vftable = (create_gjk_geom_collision_visitor_vtbl *)&create_gjk_geom_collision_visitor::`vftable';
    collision_visitor.gjk_geom_list = &gjk_geom_list;
    if ( model && XModelHasCollmap(model) && !bone->index )
    {
      create_xmodel_gjk_geom(model, &collision_visitor, -1, (unsigned int)&cls.recentServers[7546].city[57], 1, 0, 0);
    }
    else if ( XModelGetCollmapForBoneIndex(model, bone->index) )
    {
      create_xmodel_gjk_geom(model, &collision_visitor, bone->index, 0xFFFFFFFF, 1, 0, 0);
    }
    else
    {
      if ( model->collSurfs )
        surfFlags = model->collSurfs->surfFlags;
      else
        surfFlags = (int)"t";
      aabb_gjk_geom = create_aabb_gjk_geom(
                        COERCE_FLOAT(&savedregs),
                        model->mins,
                        model->maxs,
                        (unsigned __int8)((int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20] & surfFlags) >> 20),
                        &g_empty_collision_visitor);
      gjk_geom_list_t::add_geom(&gjk_geom_list, aabb_gjk_geom);
    }
    Sys_EnterCriticalSection(CRITSECT_PHYSICS);
    bone->physPreset = physPreset;
    bone->rigidBody = (int)Phys_ObjCreate(0, origin, quat, vec3_origin, physPreset, &gjk_geom_list, 1, -1);
    userData = (PhysObjUserData *)bone->rigidBody;
    if ( !userData->m_bpb
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\xdoll.cpp", 327, 0, "%s", "userData->m_bpb") )
    {
      __debugbreak();
    }
    aasap_list_remove(userData->m_bpb);
    Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
    if ( bone->rigidBody )
    {
      XDoll_SetCollides(bone, collide);
      bone->flags &= ~1u;
      return 1;
    }
    else
    {
      Com_PrintWarning(14, "XDoll: Failed to create rigid body\n");
      return 0;
    }
  }
  else
  {
    v6 = SL_ConvertToString(bone->name_hash, SCRIPTINSTANCE_SERVER);
    Com_PrintWarning(14, "XDoll: Failed to get world tag matrix for bone %s in XDoll_CreatePhysObj!\n", v6);
    return 0;
  }
}

int __cdecl XDoll_CreateXDollForConstraints(
        int localClientNum,
        int entity_index,
        int piece_index,
        int health,
        PhysConstraints *constraints,
        PhysPreset *physPreset)
{
  PhysObjUserData *UserBody; // eax
  int k; // [esp+Ch] [ebp-68h]
  int j; // [esp+10h] [ebp-64h]
  unsigned int LowercaseString; // [esp+14h] [ebp-60h]
  int bi; // [esp+18h] [ebp-5Ch]
  unsigned int bone_name_hash; // [esp+1Ch] [ebp-58h]
  const PhysConstraint *constraint; // [esp+20h] [ebp-54h]
  unsigned int ci; // [esp+24h] [ebp-50h]
  XDollBody *v15; // [esp+28h] [ebp-4Ch]
  int i; // [esp+2Ch] [ebp-48h]
  DObj *obj; // [esp+30h] [ebp-44h]
  float origin[3]; // [esp+34h] [ebp-40h] BYREF
  XModel *model; // [esp+40h] [ebp-34h]
  int xdoll_handle; // [esp+44h] [ebp-30h]
  centity_s *cent; // [esp+48h] [ebp-2Ch]
  XDollBody *body; // [esp+4Ch] [ebp-28h]
  float mat[3][3]; // [esp+50h] [ebp-24h] BYREF
  int savedregs; // [esp+74h] [ebp+0h] BYREF

  obj = Com_GetClientDObj(entity_index, localClientNum);
  if ( !obj )
    return 0;
  model = DObjGetModel(obj, 0);
  if ( !constraints || !physPreset )
    return 0;
  if ( !XDoll_Activate(constraints, health) )
    return 0;
  for ( i = 0; i < 128; ++i )
  {
    v15 = &xdollBodies[i];
    if ( v15->references > 0 && v15->piece_index == piece_index && v15->entity_index == entity_index )
    {
      ++xdollBodies[i].references;
      return i + 1;
    }
  }
  xdoll_handle = XDoll_GetUnusedBody();
  if ( !xdoll_handle )
    return 0;
  body = XDoll_HandleBody(xdoll_handle);
  body->constraintsDef = constraints;
  body->numBones = 0;
  body->entity_index = entity_index;
  body->client_index = localClientNum;
  body->piece_index = piece_index;
  for ( ci = 0; ci < body->constraintsDef->count; ++ci )
  {
    constraint = &body->constraintsDef->data[ci];
    if ( XDoll_IsXDollConstraint(body->constraintsDef->data[ci].type) )
    {
      if ( I_stricmp(constraint->target_bone1, "parent") )
      {
        bone_name_hash = SL_FindLowercaseString(constraint->target_bone1, SCRIPTINSTANCE_SERVER);
        for ( bi = 0; bi < body->numBones && bone_name_hash != body->bones[bi].name_hash; ++bi )
          ;
        if ( bi == body->numBones )
        {
          body->bones[body->numBones++].name_hash = bone_name_hash;
          if ( body->numBones >= 4
            && !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\physics\\xdoll.cpp",
                  685,
                  0,
                  "%s",
                  "body.numBones < XDOLL_MAX_BONES") )
          {
            __debugbreak();
          }
        }
      }
      if ( I_stricmp(constraint->target_bone2, "parent") )
      {
        LowercaseString = SL_FindLowercaseString(constraint->target_bone2, SCRIPTINSTANCE_SERVER);
        for ( j = 0; j < body->numBones && LowercaseString != body->bones[j].name_hash; ++j )
          ;
        if ( j == body->numBones )
        {
          body->bones[body->numBones++].name_hash = LowercaseString;
          if ( body->numBones >= 4
            && !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\physics\\xdoll.cpp",
                  701,
                  0,
                  "%s",
                  "body.numBones < XDOLL_MAX_BONES") )
          {
            __debugbreak();
          }
        }
      }
    }
  }
  cent = CG_GetEntity(localClientNum, entity_index);
  for ( k = 0; k < body->numBones; ++k )
  {
    DObjGetModelBoneIndex(obj, 0, body->bones[k].name_hash, &body->bones[k].index);
    XDoll_CreatePhysObj(body, &body->bones[k], model, &cent->pose, physPreset, 0);
  }
  body->userBody = 0;
  body->userBodyBoneHash = SL_FindLowercaseString("body_animate_jnt", SCRIPTINSTANCE_SERVER);
  if ( body->userBodyBoneHash )
  {
    if ( CG_DObjGetWorldTagMatrix(&cent->pose, obj, body->userBodyBoneHash, mat, origin) )
    {
      UserBody = Phys_CreateUserBody(COERCE_FLOAT(&savedregs), origin, entity_index, PHYS_GEOM_CAPSULE);
      body->userBody = (int)UserBody;
      if ( body->userBody )
        Phys_SetUserBody((int)&savedregs, body->userBody, origin);
    }
  }
  return xdoll_handle;
}

char __cdecl XDoll_Activate(const PhysConstraints *constraintsDef, int health)
{
  unsigned int ci; // [esp+4h] [ebp-4h]

  if ( constraintsDef->count >= 4
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\xdoll.cpp",
          203,
          0,
          "%s",
          "constraintsDef.count < XDOLL_MAX_CONSTRAINTS") )
  {
    __debugbreak();
  }
  for ( ci = 0; ci < constraintsDef->count; ++ci )
  {
    if ( XDoll_IsXDollConstraint(constraintsDef->data[ci].type)
      && health > constraintsDef->data[ci].min_health
      && health <= constraintsDef->data[ci].max_health )
    {
      return 1;
    }
  }
  return 0;
}

void __cdecl XDoll_UpdateHealth(int xdoll_handle, int health)
{
  XDoll_ConstraintInfo *cinfo; // [esp+0h] [ebp-14h]
  unsigned int ci; // [esp+8h] [ebp-Ch]
  const PhysConstraints *constraintsDef; // [esp+Ch] [ebp-8h]
  XDollBody *body; // [esp+10h] [ebp-4h]
  int savedregs; // [esp+14h] [ebp+0h] BYREF

  body = XDoll_HandleBody(xdoll_handle);
  if ( body->constraintsDef )
  {
    constraintsDef = body->constraintsDef;
    if ( constraintsDef->count >= 4
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\physics\\xdoll.cpp",
            739,
            0,
            "%s",
            "constraintsDef.count < XDOLL_MAX_CONSTRAINTS") )
    {
      __debugbreak();
    }
    for ( ci = 0; ci < constraintsDef->count; ++ci )
    {
      if ( XDoll_IsXDollConstraint(constraintsDef->data[ci].type) )
      {
        cinfo = &body->constraintInfos[ci];
        if ( body->constraintInfos[ci].constraint )
        {
          if ( health < constraintsDef->data[ci].min_health || health >= constraintsDef->data[ci].max_health )
          {
            XDoll_DestroyConstraint(cinfo);
            XDoll_SetCollides(body, 1);
          }
        }
        else if ( health > constraintsDef->data[ci].min_health
               && health <= constraintsDef->data[ci].max_health
               && (body->constraintInfos[ci].timer < 1000 * constraintsDef->data[ci].timeout
                || !constraintsDef->data[ci].timeout) )
        {
          XDoll_CreateConstraint((PhysObjUserData *)&savedregs, body, &constraintsDef->data[ci], cinfo);
          XDoll_SetCollides(body, 0);
        }
      }
    }
  }
}

void  XDoll_CreateConstraint(
        PhysObjUserData *a1@<ebp>,
        XDollBody *body,
        const PhysConstraint *constraint,
        XDoll_ConstraintInfo *cinfo)
{
  const phys_vec3 *v4; // eax
  const phys_vec3 *v5; // eax
  const phys_vec3 *power; // [esp+8h] [ebp-F0h]
  const phys_vec3 *powera; // [esp+8h] [ebp-F0h]
  _BYTE v8[12]; // [esp+Ch] [ebp-ECh] BYREF
  phys_vec3 b1_ref_axis_loc; // [esp+18h] [ebp-E0h] BYREF
  phys_vec3 b1_hinge_axis_loc; // [esp+28h] [ebp-D0h] BYREF
  phys_vec3 ref_axis_loc; // [esp+38h] [ebp-C0h] BYREF
  phys_vec3 hinge_axis_loc; // [esp+48h] [ebp-B0h] BYREF
  phys_vec3 v13; // [esp+5Ch] [ebp-9Ch] BYREF
  int v14; // [esp+78h] [ebp-80h]
  float v15; // [esp+7Ch] [ebp-7Ch]
  float v16; // [esp+80h] [ebp-78h]
  float damp; // [esp+84h] [ebp-74h]
  char *highStop; // [esp+88h] [ebp-70h]
  float lowStop; // [esp+8Ch] [ebp-6Ch] BYREF
  _BYTE v20[12]; // [esp+9Ch] [ebp-5Ch] BYREF
  phys_vec3 b1_anchor_loc; // [esp+A8h] [ebp-50h] BYREF
  char *v22; // [esp+C4h] [ebp-34h]
  char *m_userdata; // [esp+C8h] [ebp-30h]
  _BYTE v24[12]; // [esp+CCh] [ebp-2Ch] BYREF
  phys_vec3 anchor_loc; // [esp+D8h] [ebp-20h]
  environment_rigid_body *RigidBody; // [esp+E8h] [ebp-10h]
  PhysObjUserData *userData; // [esp+ECh] [ebp-Ch] BYREF
  rigid_body *rb2; // [esp+F0h] [ebp-8h]
  rigid_body *retaddr; // [esp+F8h] [ebp+0h]

  userData = a1;
  rb2 = retaddr;
  RigidBody = XDoll_GetRigidBody(body, constraint->target_bone1);
  LODWORD(anchor_loc.w) = XDoll_GetRigidBody(body, constraint->target_bone2);
  if ( RigidBody && LODWORD(anchor_loc.w) )
  {
    if ( (environment_rigid_body *)LODWORD(anchor_loc.w) == phys_sys::get_environment_rigid_body() && body->userBody )
    {
      LODWORD(anchor_loc.z) = body->userBody;
      anchor_loc.w = *(float *)LODWORD(anchor_loc.z);
    }
    if ( !LODWORD(anchor_loc.w)
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\physics\\xdoll.cpp", 489, 0, "%s", "rb2") )
    {
      __debugbreak();
    }
    cinfo->type = constraint->type;
    cinfo->timer = 0;
    LODWORD(anchor_loc.y) = constraint->type;
    switch ( LODWORD(anchor_loc.y) )
    {
      case 1:
        Phys_Vec3ToNitrousVec(constraint->offset, (phys_vec3 *)v24);
        m_userdata = (char *)RigidBody->m_userdata;
        v22 = m_userdata + 32;
        power = (const phys_vec3 *)(m_userdata + 80);
        v4 = phys_multiply((phys_vec3 *)&b1_anchor_loc.y, (const phys_mat44 *)(m_userdata + 32), (const phys_vec3 *)v24);
        operator+((phys_vec3 *)v20, v4, power);
        cinfo->constraint = XDoll_CreatePointConstraint(
                              (int)&userData,
                              RigidBody,
                              (environment_rigid_body *)LODWORD(anchor_loc.w),
                              (const phys_vec3 *)v20);
        break;
      case 3:
        Phys_Vec3ToNitrousVec(constraint->offset, (phys_vec3 *)&lowStop);
        highStop = (char *)RigidBody->m_userdata;
        damp = constraint->damp;
        v16 = constraint->minAngle * 0.017453292;
        v15 = constraint->maxAngle * 0.017453292;
        v14 = (int)(highStop + 32);
        powera = (const phys_vec3 *)(highStop + 80);
        v5 = phys_multiply(&v13, (const phys_mat44 *)(highStop + 32), (const phys_vec3 *)&lowStop);
        operator+((phys_vec3 *)&hinge_axis_loc.y, v5, powera);
        ref_axis_loc.y = *(float *)&FLOAT_0_0;
        ref_axis_loc.z = *(float *)&FLOAT_0_0;
        ref_axis_loc.w = FLOAT_1_0;
        b1_hinge_axis_loc.y = *(float *)&FLOAT_0_0;
        b1_hinge_axis_loc.z = FLOAT_1_0;
        b1_hinge_axis_loc.w = *(float *)&FLOAT_0_0;
        phys_multiply(
          (phys_vec3 *)&b1_ref_axis_loc.y,
          (const phys_mat44 *)(highStop + 32),
          (phys_vec3 *)&ref_axis_loc.y);
        phys_multiply((const phys_vec3 *)v8, (const phys_mat44 *)(highStop + 32), (phys_vec3 *)&b1_hinge_axis_loc.y);
        cinfo->constraint = XDoll_CreateHingeConstraint(
                              (int)&userData,
                              RigidBody,
                              (environment_rigid_body *)LODWORD(anchor_loc.w),
                              (phys_vec3 *)&hinge_axis_loc.y,
                              (phys_vec3 *)&b1_ref_axis_loc.y,
                              (const phys_vec3 *)v8,
                              damp,
                              v16,
                              v15);
        break;
      case 5:
        cinfo->constraint = XDoll_CreateActuatorConstraint(
                              (const phys_mat44 *)&userData,
                              RigidBody,
                              (environment_rigid_body *)LODWORD(anchor_loc.w),
                              constraint->power);
        break;
    }
  }
}

rigid_body_constraint_hinge * XDoll_CreateHingeConstraint@<eax>(
        int a1@<ebp>,
        environment_rigid_body *rb1,
        environment_rigid_body *rb2,
        const phys_vec3 *b1_anchor_loc,
        const phys_vec3 *b1_hinge_axis_loc,
        const phys_vec3 *b1_ref_axis_loc,
        float damp,
        float lowStop,
        float highStop)
{
  _BYTE v10[12]; // [esp+Ch] [ebp-7Ch] BYREF
  phys_vec3 b2_ref_axis_loc; // [esp+18h] [ebp-70h] BYREF
  phys_vec3 ref_axis_abs; // [esp+28h] [ebp-60h] BYREF
  phys_vec3 b2_hinge_axis_loc; // [esp+38h] [ebp-50h] BYREF
  phys_vec3 hinge_axis_abs; // [esp+48h] [ebp-40h] BYREF
  phys_vec3 b2_anchor_loc; // [esp+58h] [ebp-30h] BYREF
  rigid_body_constraint_hinge *rbc_hinge; // [esp+78h] [ebp-10h]
  unsigned int v17[2]; // [esp+7Ch] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+88h] [ebp+0h]

  v17[0] = a1;
  v17[1] = retaddr;
  rbc_hinge = phys_sys::create_rbc_hinge(rb1, rb2, 0);
  if ( rbc_hinge )
  {
    phys_full_multiply((int)v17, (phys_vec3 *)&b2_anchor_loc.y, &rb1->m_mat, b1_anchor_loc);
    phys_full_inv_multiply((int)v17, (phys_vec3 *)&hinge_axis_abs.y, &rb2->m_mat, (phys_vec3 *)&b2_anchor_loc.y);
    phys_multiply((phys_vec3 *)&b2_hinge_axis_loc.y, &rb1->m_mat, b1_hinge_axis_loc);
    phys_inv_multiply((phys_vec3 *)&ref_axis_abs.y, &rb2->m_mat, (phys_vec3 *)&b2_hinge_axis_loc.y);
    phys_multiply((phys_vec3 *)&b2_ref_axis_loc.y, &rb1->m_mat, b1_ref_axis_loc);
    phys_inv_multiply((const phys_vec3 *)v10, &rb2->m_mat, (phys_vec3 *)&b2_ref_axis_loc.y);
    rigid_body_constraint_hinge::set(
      rbc_hinge,
      (int)v17,
      b1_anchor_loc,
      (phys_vec3 *)&hinge_axis_abs.y,
      b1_hinge_axis_loc,
      (phys_vec3 *)&ref_axis_abs.y,
      b1_ref_axis_loc,
      (const phys_vec3 *)v10,
      lowStop,
      highStop,
      damp);
    return rbc_hinge;
  }
  else
  {
    Com_PrintWarning(20, "Physics: Out of hinge constraints\n");
    return 0;
  }
}

rigid_body_constraint_point * XDoll_CreatePointConstraint@<eax>(
        int a1@<ebp>,
        environment_rigid_body *rb1,
        environment_rigid_body *rb2,
        const phys_vec3 *b1_anchor_loc)
{
  _BYTE v5[12]; // [esp-Ch] [ebp-3Ch] BYREF
  phys_vec3 b2_anchor_loc; // [esp+0h] [ebp-30h] BYREF
  rigid_body_constraint_point *rbc_point; // [esp+20h] [ebp-10h]
  unsigned int v8[2]; // [esp+24h] [ebp-Ch] BYREF
  _UNKNOWN *retaddr; // [esp+30h] [ebp+0h]

  v8[0] = a1;
  v8[1] = retaddr;
  rbc_point = phys_sys::create_rbc_point(rb1, rb2, 0);
  if ( rbc_point )
  {
    phys_full_multiply((int)v8, (phys_vec3 *)&b2_anchor_loc.y, &rb1->m_mat, b1_anchor_loc);
    phys_full_inv_multiply((int)v8, (phys_vec3 *)v5, &rb2->m_mat, (phys_vec3 *)&b2_anchor_loc.y);
    rigid_body_constraint_point::set(rbc_point, b1_anchor_loc, (const phys_vec3 *)v5);
    return rbc_point;
  }
  else
  {
    Com_PrintWarning(20, "Physics: Out of point constraints\n");
    return 0;
  }
}

rigid_body_constraint_angular_actuator * XDoll_CreateActuatorConstraint@<eax>(
        const phys_mat44 *a1@<ebp>,
        environment_rigid_body *rb1,
        environment_rigid_body *rb2,
        float power)
{
  _BYTE v5[12]; // [esp+18h] [ebp-5Ch] BYREF
  phys_mat44 target_mat; // [esp+24h] [ebp-50h]
  rigid_body_constraint_angular_actuator *rbc_angular_actuator; // [esp+64h] [ebp-10h]
  const phys_mat44 *rb2_mat; // [esp+68h] [ebp-Ch] BYREF
  const phys_mat44 *rb1_mat; // [esp+6Ch] [ebp-8h]
  const phys_mat44 *retaddr; // [esp+74h] [ebp+0h]

  rb2_mat = a1;
  rb1_mat = retaddr;
  rbc_angular_actuator = phys_sys::create_rbc_angular_actuator(rb1, rb2, 0);
  if ( rbc_angular_actuator )
  {
    LODWORD(target_mat.w.w) = &rb1->m_mat;
    LODWORD(target_mat.w.z) = &rb2->m_mat;
    phys_inv_multiply_mat((int)&rb2_mat, (phys_mat44 *)v5, &rb1->m_mat, &rb2->m_mat);
    rigid_body_constraint_angular_actuator::set(rbc_angular_actuator, power / rb1->m_inv_mass, (const phys_mat44 *)v5);
    return rbc_angular_actuator;
  }
  else
  {
    Com_PrintWarning(20, "Physics: Out of actuator constraints\n");
    return 0;
  }
}

environment_rigid_body *__cdecl XDoll_GetRigidBody(XDollBody *body, const char *bone_name)
{
  int bi; // [esp+4h] [ebp-10h]
  rigid_body *rb; // [esp+8h] [ebp-Ch]
  unsigned int bone_name_hash; // [esp+Ch] [ebp-8h]

  rb = 0;
  bone_name_hash = SL_FindLowercaseString(bone_name, SCRIPTINSTANCE_SERVER);
  if ( bone_name_hash == SL_FindLowercaseString("parent", SCRIPTINSTANCE_SERVER) )
    return Destructible_GetRigidBody();
  for ( bi = 0; bi < body->numBones; ++bi )
  {
    if ( bone_name_hash == body->bones[bi].name_hash )
    {
      if ( !body->bones[bi].rigidBody )
        return 0;
      return *(environment_rigid_body **)body->bones[bi].rigidBody;
    }
  }
  return (environment_rigid_body *)rb;
}

void __cdecl XDoll_ApplyForce(int xdoll_handle, const float *hitp, const float *hitd, int mod)
{
  XModel *model; // [esp+8h] [ebp-18h]
  const DObj *obj; // [esp+Ch] [ebp-14h]
  centity_s *cent; // [esp+10h] [ebp-10h]
  int bi; // [esp+18h] [ebp-8h]
  XDollBody *body; // [esp+1Ch] [ebp-4h]

  body = XDoll_HandleBody(xdoll_handle);
  for ( bi = 0; bi < body->numBones; ++bi )
  {
    if ( (body->bones[bi].flags & 1) != 0 )
    {
      cent = CG_GetEntity(body->client_index, body->entity_index);
      obj = Com_GetClientDObj(body->entity_index, body->client_index);
      if ( obj )
      {
        model = DObjGetModel(obj, 0);
        XDoll_CreatePhysObj(
          body,
          &body->bones[bi],
          model,
          &cent->pose,
          body->bones[bi].physPreset,
          (body->bones[bi].flags & 2) != 0);
      }
    }
    if ( body->bones[bi].rigidBody )
      Phys_ObjAddCustomForce(body->bones[bi].rigidBody, hitp, hitd, mod, body->bones[bi].physPreset);
  }
}

XDollBody *__cdecl XDoll_HandleBody(int xdollHandle)
{
  if ( (unsigned int)(xdollHandle - 1) >= 0x80
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\xdoll.cpp",
          795,
          0,
          "xdollHandle - 1 doesn't index XDOLL_MAX\n\t%i not in [0, %i)",
          xdollHandle - 1,
          128) )
  {
    __debugbreak();
  }
  return &xdollBodies[xdollHandle - 1];
}

DObj *__cdecl XDoll_BodyDObj(XDollBody *body, int localClientNum)
{
  return Com_GetClientDObj(body->entity_index, localClientNum);
}

XDollBoneOrientation *__cdecl XDoll_BodyBoneOrientations(XDollBody *body)
{
  return body->boneOrientations;
}

void __cdecl XDoll_DoControllers(int xdoll_handle, int *partBits, int localClientNum)
{
  float v3; // [esp+0h] [ebp-2Ch]
  XDollBone *bone; // [esp+Ch] [ebp-20h]
  int i; // [esp+10h] [ebp-1Ch]
  XDollBoneOrientation *boneOrientation; // [esp+14h] [ebp-18h]
  DObj *obj; // [esp+18h] [ebp-14h]
  XDollBody *body; // [esp+1Ch] [ebp-10h]
  DObjAnimMat *boneMat; // [esp+20h] [ebp-Ch]
  DObjAnimMat *skel; // [esp+28h] [ebp-4h]

  if ( xdoll_handle )
  {
    body = XDoll_HandleBody(xdoll_handle);
    obj = XDoll_BodyDObj(body, localClientNum);
    skel = DObjGetRotTransArray(obj);
    if ( skel )
    {
      boneOrientation = XDoll_BodyBoneOrientations(body);
      if ( body->numBones >= 4
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\physics\\xdoll.cpp",
              862,
              0,
              "%s",
              "body->numBones < XDOLL_MAX_BONES") )
      {
        __debugbreak();
      }
      for ( i = 0; i < body->numBones; ++i )
      {
        bone = &body->bones[i];
        if ( bone->index && DObjSetSkelRotTransIndex(obj, partBits, bone->index) )
        {
          boneMat = &skel[bone->index];
          boneMat->trans[0] = boneOrientation->origin[0];
          boneMat->trans[1] = boneOrientation->origin[1];
          boneMat->trans[2] = boneOrientation->origin[2];
          boneMat->quat[0] = boneOrientation->orientation[0];
          boneMat->quat[1] = boneOrientation->orientation[1];
          boneMat->quat[2] = boneOrientation->orientation[2];
          boneMat->quat[3] = boneOrientation->orientation[3];
          v3 = Vec4LengthSq(boneMat->quat);
          if ( v3 == 0.0 )
          {
            boneMat->quat[3] = FLOAT_1_0;
            boneMat->transWeight = FLOAT_2_0;
          }
          else
          {
            boneMat->transWeight = 2.0 / v3;
          }
        }
        ++boneOrientation;
      }
    }
  }
}

