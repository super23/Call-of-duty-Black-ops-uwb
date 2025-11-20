#include "ragdoll.h"

void __thiscall phys_free_list<RagdollBody>::remove(
        phys_free_list<RagdollBody> *this,
        phys_free_list<RagdollBody>::T_internal *data)
{
  phys_free_list<RagdollBody>::T_internal_base *next; // [esp+14h] [ebp-8h]
  phys_free_list<RagdollBody>::T_internal_base *prev; // [esp+18h] [ebp-4h]

  if ( !data
    && _tlAssert("c:\\projects_pc\\cod\\codsrc\\tl\\physics\\include\\phys_mem.h", 477, "data", &toastPopupTitle) )
  {
    __debugbreak();
  }
  --this->m_list_count;
  phys_free_list<RagdollBody>::debug_remove(this, data);
  next = data->m_next_T_internal;
  prev = data->m_prev_T_internal;
  prev->m_next_T_internal = next;
  next->m_prev_T_internal = prev;
  PMM_FREE((unsigned __int8 *)data, 0xA34u, 4u);
}

void __cdecl Ragdoll_DebugDraw()
{
  char *v0; // eax
  char *v1; // eax
  phys_free_list<RagdollBody>::T_internal_base *body_i; // [esp+2Ch] [ebp-20h]
  int total; // [esp+30h] [ebp-1Ch]
  int states[6]; // [esp+34h] [ebp-18h] BYREF

  if ( ragdoll_debug->current.integer )
  {
    memset(states, 0, sizeof(states));
    total = 0;
    for ( body_i = g_ragdoll_body_pool.m_dummy_head.m_next_T_internal;
          &g_ragdoll_body_pool != (phys_free_list<RagdollBody> *)body_i;
          body_i = body_i->m_next_T_internal )
    {
      if ( !Ragdoll_BodyInUse((RagdollBody *)&body_i[1])
        && _tlAssert(
             "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.cpp",
             96,
             "Ragdoll_BodyInUse( body )",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
      if ( body_i[5].m_next_T_internal >= (phys_free_list<RagdollBody>::T_internal_base *)6
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.cpp",
              97,
              0,
              "(int)body->state doesn't index RAGDOLL_NUM_STATES\n\t%i not in [0, %i)",
              body_i[5].m_next_T_internal,
              6) )
      {
        __debugbreak();
      }
      ++states[(int)body_i[5].m_next_T_internal];
      ++total;
    }
    v0 = va("RB Total: %d", total);
    CG_DrawStringExt(&scrPlaceFull, 0.0, 72.0, v0, colorGreen, 0, 1, 12.0);
    v1 = va("RB State: %d %d %d %d %d %d", states[0], states[1], states[2], states[3], states[4], states[5]);
    CG_DrawStringExt(&scrPlaceFull, 0.0, 84.0, v1, colorGreen, 0, 1, 12.0);
  }
}

bool __cdecl Ragdoll_BodyInUse(RagdollBody *body)
{
  if ( !body && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.h", 313, 0, "%s", "body") )
    __debugbreak();
  return body->references > 0;
}

RagdollDef *__cdecl Ragdoll_BodyDef(RagdollBody *body)
{
  if ( !body && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.cpp", 169, 0, "%s", "body") )
    __debugbreak();
  if ( body->ragdollDef >= 2u
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.cpp",
          170,
          0,
          "body->ragdollDef doesn't index RAGDOLL_MAX_DEFS\n\t%i not in [0, %i)",
          body->ragdollDef,
          2) )
  {
    __debugbreak();
  }
  return &ragdollDefs[body->ragdollDef];
}

DObj *__cdecl Ragdoll_BodyDObj(RagdollBody *body)
{
  if ( !body && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.cpp", 178, 0, "%s", "body") )
    __debugbreak();
  if ( body->obj )
    return body->obj;
  if ( body->dobj == -1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.cpp",
          186,
          0,
          "%s",
          "body->dobj != DOBJ_HANDLE_NONE") )
  {
    __debugbreak();
  }
  return Com_GetClientDObj(body->dobj, body->localClientNum);
}

centity_s *__cdecl Ragdoll_BodyPose(RagdollBody *body)
{
  if ( !body && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.cpp", 194, 0, "%s", "body") )
    __debugbreak();
  if ( body->pose )
    return (centity_s *)body->pose;
  if ( body->dobj == -1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.cpp",
          202,
          0,
          "%s",
          "body->dobj != DOBJ_HANDLE_NONE") )
  {
    __debugbreak();
  }
  return CG_GetPose(body->localClientNum, body->dobj);
}

void __cdecl Ragdoll_BodyRootOrigin(RagdollBody *body, float *origin)
{
  BoneOrientation *boneOrientation; // [esp+0h] [ebp-4h]

  if ( Ragdoll_BodyPoseValid(body) )
  {
    boneOrientation = Ragdoll_BodyBoneOrientations(body);
    *origin = boneOrientation->origin[0];
    origin[1] = boneOrientation->origin[1];
    origin[2] = boneOrientation->origin[2];
  }
}

bool __cdecl Ragdoll_BodyPoseValid(RagdollBody *body)
{
  if ( !body && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.h", 299, 0, "%s", "body") )
    __debugbreak();
  return body->state >= BS_TUNNEL_TEST;
}

void __cdecl Ragdoll_GetRootOrigin(const cpose_t *ragdollHandle, float *origin)
{
  RagdollBody *body; // [esp+0h] [ebp-4h]

  body = (RagdollBody *)Ragdoll_HandleBody(ragdollHandle);
  if ( (!body || !Ragdoll_BodyInUse(body))
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.cpp",
          235,
          0,
          "%s",
          "body && Ragdoll_BodyInUse( body )") )
  {
    __debugbreak();
  }
  if ( Ragdoll_BodyPoseValid(body) )
    Ragdoll_BodyRootOrigin(body, origin);
}

int __cdecl Ragdoll_CountPhysicsBodies()
{
  phys_free_list<RagdollBody>::T_internal_base *body_i; // [esp+Ch] [ebp-8h]
  int running; // [esp+10h] [ebp-4h]

  running = 0;
  for ( body_i = g_ragdoll_body_pool.m_dummy_head.m_next_T_internal;
        &g_ragdoll_body_pool != (phys_free_list<RagdollBody> *)body_i;
        body_i = body_i->m_next_T_internal )
  {
    if ( !Ragdoll_BodyInUse((RagdollBody *)&body_i[1])
      && _tlAssert(
           "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.cpp",
           347,
           "Ragdoll_BodyInUse( body )",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    if ( Ragdoll_BodyHasPhysics((RagdollBody *)&body_i[1]) )
      ++running;
  }
  return running;
}

bool __cdecl Ragdoll_BodyHasPhysics(RagdollBody *body)
{
  if ( !body && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.h", 320, 0, "%s", "body") )
    __debugbreak();
  return body->state >= BS_DOBJ_WAIT && body->state <= BS_RUNNING;
}

const cpose_t *__cdecl Ragdoll_CreateRagdollForDObj(
        int localClientNum,
        int ragdollDef,
        phys_free_list<RagdollBody>::T_internal_base *dobj,
        bool reset,
        bool share)
{
  const cpose_t *ragdoll; // [esp+0h] [ebp-8h]
  RagdollBody *body; // [esp+4h] [ebp-4h]

  if ( dobj == (phys_free_list<RagdollBody>::T_internal_base *)-1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.cpp",
          360,
          0,
          "%s",
          "dobj != DOBJ_HANDLE_NONE") )
  {
    __debugbreak();
  }
  if ( !Ragdoll_BindDef(1u) )
    return 0;
  if ( Ragdoll_CountPhysicsBodies() >= ragdoll_max_simulating->current.integer )
    return 0;
  if ( share )
  {
    ragdoll = (const cpose_t *)Ragdoll_ReferenceDObjBody(dobj);
    if ( !ragdoll )
      ragdoll = (const cpose_t *)Ragdoll_GetUnusedBody();
  }
  else
  {
    ragdoll = (const cpose_t *)Ragdoll_GetUnusedBody();
  }
  if ( ragdoll )
  {
    body = (RagdollBody *)Ragdoll_HandleBody(ragdoll);
    if ( reset )
      Ragdoll_BodyNewState(body, BS_DEAD);
    if ( body->state == BS_DEAD )
    {
      body->dobj = (int)dobj;
      body->ragdollDef = 1;
      body->localClientNum = localClientNum;
      Ragdoll_BodyNewState(body, BS_DOBJ_WAIT);
    }
    return ragdoll;
  }
  else
  {
    Com_Printf(20, "Ragdoll allocation failed, out of ragdoll bodies (obj %d)\n", dobj);
    return 0;
  }
}

RagdollBody *__cdecl Ragdoll_GetUnusedBody()
{
  RagdollBody *body; // [esp+1Ch] [ebp-4h]

  if ( g_ragdoll_body_pool.m_list_count >= 32 )
    return 0;
  body = phys_free_list<RagdollBody>::add(&g_ragdoll_body_pool, 0, "phys_free_list error: out of memory.");
  if ( !body )
    return 0;
  Ragdoll_InitBody(body);
  Ragdoll_BodyNewState(body, BS_DEAD);
  if ( body->references )
  {
    if ( _tlAssert(
           "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.cpp",
           125,
           "body->references == 0",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
  }
  ++body->references;
  return body;
}

void __cdecl Ragdoll_InitBody(RagdollBody *body)
{
  if ( !body && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.cpp", 110, 0, "%s", "body") )
    __debugbreak();
  memset((unsigned __int8 *)body, 0, sizeof(RagdollBody));
}

phys_free_list<RagdollBody>::T_internal_base *__cdecl Ragdoll_ReferenceDObjBody(
        phys_free_list<RagdollBody>::T_internal_base *dobj)
{
  phys_free_list<RagdollBody>::T_internal_base *body_i; // [esp+Ch] [ebp-4h]

  for ( body_i = g_ragdoll_body_pool.m_dummy_head.m_next_T_internal;
        &g_ragdoll_body_pool != (phys_free_list<RagdollBody> *)body_i;
        body_i = body_i->m_next_T_internal )
  {
    if ( !Ragdoll_BodyInUse((RagdollBody *)&body_i[1]) )
    {
      if ( _tlAssert(
             "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.cpp",
             141,
             "Ragdoll_BodyInUse( body )",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
    }
    if ( body_i[2].m_prev_T_internal == dobj )
    {
      ++body_i[1].m_prev_T_internal;
      return body_i + 1;
    }
  }
  return 0;
}

char __cdecl Ragdoll_BindDef(unsigned int ragdollDef)
{
  RagdollDef *def; // [esp+0h] [ebp-18h]
  int nameIdx; // [esp+4h] [ebp-14h]
  BoneDef *boneDef; // [esp+8h] [ebp-10h]
  BoneDef *boneDefa; // [esp+8h] [ebp-10h]
  int i; // [esp+Ch] [ebp-Ch]
  int ia; // [esp+Ch] [ebp-Ch]
  BaseLerpBoneDef *lerpBoneDef; // [esp+10h] [ebp-8h]
  int parentIdx; // [esp+14h] [ebp-4h]

  if ( !Ragdoll_ValidateDef(ragdollDef) )
    return 0;
  def = &ragdollDefs[ragdollDef];
  if ( def->bound )
    return 1;
  boneDef = def->boneDefs;
  for ( i = 0; i < def->numBones; ++i )
  {
    for ( nameIdx = 0; nameIdx < 2; ++nameIdx )
    {
      if ( I_stricmp(boneDef->animBoneTextNames[nameIdx], "none") )
      {
        boneDef->animBoneNames[nameIdx] = SL_FindString(boneDef->animBoneTextNames[nameIdx], SCRIPTINSTANCE_SERVER);
        if ( !boneDef->animBoneNames[nameIdx] )
          return 0;
      }
      else
      {
        boneDef->animBoneNames[nameIdx] = 0;
      }
    }
    ++boneDef;
  }
  lerpBoneDef = def->baseLerpBoneDefs;
  for ( ia = 0; ia < def->numBaseLerpBones; ++ia )
  {
    lerpBoneDef->animBoneName = SL_FindString(lerpBoneDef->animBoneTextName, SCRIPTINSTANCE_SERVER);
    if ( !lerpBoneDef->animBoneName )
      return 0;
    if ( !lerpBoneDef->parentBoneIndex )
    {
      boneDefa = def->boneDefs;
      for ( parentIdx = 0; parentIdx < def->numBones; ++parentIdx )
      {
        if ( boneDefa->animBoneNames[1] == lerpBoneDef->animBoneName )
        {
          lerpBoneDef->parentBoneIndex = parentIdx;
          break;
        }
        ++boneDefa;
      }
      if ( parentIdx == def->numBones )
        return 0;
    }
    ++lerpBoneDef;
  }
  def->bound = 1;
  return 1;
}

bool __cdecl Ragdoll_ValidateDef(unsigned int ragdollDef)
{
  if ( ragdollDef >= 2
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.cpp",
          262,
          0,
          "ragdollDef doesn't index RAGDOLL_MAX_DEFS\n\t%i not in [0, %i)",
          ragdollDef,
          2) )
  {
    __debugbreak();
  }
  return ragdollDefs[ragdollDef].numBones != 0;
}

void __cdecl Ragdoll_Remove(const cpose_t *ragdoll)
{
  RagdollBody *body; // [esp+0h] [ebp-4h]

  body = (RagdollBody *)Ragdoll_HandleBody(ragdoll);
  if ( !Ragdoll_BodyInUse(body)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.cpp",
          459,
          0,
          "%s",
          "Ragdoll_BodyInUse( body )") )
  {
    __debugbreak();
  }
  if ( body->references == 1 )
  {
    Ragdoll_BodyNewState(body, BS_DEAD);
    Ragdoll_FreeBody(ragdoll);
  }
  else
  {
    --body->references;
  }
}

void __cdecl Ragdoll_FreeBody(const cpose_t *ragdollBody)
{
  RagdollBody *body; // [esp+24h] [ebp-4h]

  body = (RagdollBody *)Ragdoll_HandleBody(ragdollBody);
  if ( !body && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.cpp", 157, 0, "%s", "body") )
    __debugbreak();
  if ( body->references != 1
    && _tlAssert(
         "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.cpp",
         158,
         "body->references == 1",
         &toastPopupTitle) )
  {
    __debugbreak();
  }
  Ragdoll_BodyNewState(body, BS_DEAD);
  body->references = 0;
  phys_free_list<RagdollBody>::remove(&g_ragdoll_body_pool, body);
}

void __cdecl Ragdoll_InitDvars()
{
  ragdoll_enable = _Dvar_RegisterBool("ragdoll_enable", 1, 0, "Turn on ragdoll death animations");
  ragdoll_debug = _Dvar_RegisterInt("ragdoll_debug", 0, 0, 0x7FFFFFFF, 0x80u, "Draw ragdoll debug info (bitflags)");
  ragdoll_fps = _Dvar_RegisterInt("ragdoll_fps", 20, 0, 100, 0x80u, "Ragdoll update frames per second");
  ragdoll_max_life = _Dvar_RegisterInt(
                       "ragdoll_max_life",
                       9000,
                       0,
                       0x7FFFFFFF,
                       0x80u,
                       "Max lifetime of a ragdoll system in msec");
  ragdoll_max_simulating = _Dvar_RegisterInt(
                             "ragdoll_max_simulating",
                             16,
                             0,
                             32,
                             1u,
                             "Max number of simultaneous active ragdolls");
  ragdoll_explode_force = _Dvar_RegisterFloat(
                            "ragdoll_explode_force",
                            18000.0,
                            0.0,
                            60000.0,
                            0x80u,
                            "Explosive force applied to ragdolls");
  ragdoll_explode_upbias = _Dvar_RegisterFloat(
                             "ragdoll_explode_upbias",
                             0.80000001,
                             0.0,
                             2.0,
                             0x80u,
                             "Upwards bias applied to ragdoll explosion effects");
  ragdoll_bullet_force = _Dvar_RegisterFloat(
                           "ragdoll_bullet_force",
                           500.0,
                           0.0,
                           10000.0,
                           0x80u,
                           "Bullet force applied to ragdolls");
  ragdoll_bullet_upbias = _Dvar_RegisterFloat(
                            "ragdoll_bullet_upbias",
                            0.5,
                            0.0,
                            10000.0,
                            0x80u,
                            "Upward bias applied to ragdoll bullet effects");
  ragdoll_baselerp_time = _Dvar_RegisterInt(
                            "ragdoll_baselerp_time",
                            1000,
                            100,
                            6000,
                            0x80u,
                            "Default time ragdoll baselerp bones take to reach the base pose");
  ragdoll_jointlerp_time = _Dvar_RegisterInt(
                             "ragdoll_jointlerp_time",
                             3000,
                             100,
                             6000,
                             0x80u,
                             "Default time taken to lerp down ragdoll joint friction");
  ragdoll_rotvel_scale = _Dvar_RegisterFloat(
                           "ragdoll_rotvel_scale",
                           1.0,
                           0.0,
                           2000.0,
                           0x80u,
                           "Ragdoll rotational velocity estimate scale");
  ragdoll_jitter_scale = _Dvar_RegisterFloat(
                           "ragdoll_jitter_scale",
                           1.0,
                           0.0,
                           10.0,
                           0x80u,
                           "Scale up or down the effect of physics jitter on ragdolls");
  ragdoll_self_collision_scale = _Dvar_RegisterFloat(
                                   "ragdoll_self_collision_scale",
                                   1.2,
                                   0.1,
                                   10.0,
                                   0x80u,
                                   "Scale the size of the collision capsules used to prevent ragdoll limbs from interpenetrating");
  ragdoll_dump_anims = _Dvar_RegisterBool("ragdoll_dump_anims", 0, 0, "Dump animation data when ragdoll fails");
}

void __cdecl Ragdoll_Register()
{
  int ControllerIndex; // eax

  if ( !Sys_IsMainThread()
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.cpp", 515, 0, "%s", "Sys_IsMainThread()") )
  {
    __debugbreak();
  }
  if ( ragdollFirstInit
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.cpp", 516, 0, "%s", "!ragdollFirstInit") )
  {
    __debugbreak();
  }
  Ragdoll_InitDvars();
  Ragdoll_InitCommands();
  ControllerIndex = Com_LocalClient_GetControllerIndex(0);
  Cmd_ExecuteSingleCommand(0, ControllerIndex, "exec ragdoll.cfg");
  ragdollFirstInit = 1;
}

void __cdecl Ragdoll_Init()
{
  int i; // [esp+0h] [ebp-4h]

  if ( !Sys_IsMainThread()
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.cpp", 531, 0, "%s", "Sys_IsMainThread()") )
  {
    __debugbreak();
  }
  if ( !ragdollFirstInit
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.cpp", 532, 0, "%s", "ragdollFirstInit") )
  {
    __debugbreak();
  }
  if ( !ragdollInited )
  {
    for ( i = 0; i < 2; ++i )
    {
      byte_9C3B091[3848 * i] = 0;
      ragdollDefs[i].inUse = 0;
    }
    ragdollInited = 1;
  }
}

void __cdecl Ragdoll_Shutdown()
{
  if ( !Sys_IsMainThread()
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.cpp", 551, 0, "%s", "Sys_IsMainThread()") )
  {
    __debugbreak();
  }
  ragdollInited = 0;
}

RagdollBody *__thiscall phys_free_list<RagdollBody>::add(
        phys_free_list<RagdollBody> *this,
        int no_error,
        const char *error_msg)
{
  phys_free_list<RagdollBody>::T_internal *ptr; // [esp+1Ch] [ebp-4h]

  ptr = (phys_free_list<RagdollBody>::T_internal *)PMM_ALLOC(0xA34u, 4u);
  if ( ptr )
  {
    ptr->m_prev_T_internal = &this->m_dummy_head;
    ptr->m_next_T_internal = this->m_dummy_head.m_next_T_internal;
    this->m_dummy_head.m_next_T_internal->m_prev_T_internal = ptr;
    this->m_dummy_head.m_next_T_internal = ptr;
    ++this->m_list_count;
    phys_free_list<RagdollBody>::debug_add(this, ptr);
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

void __thiscall phys_free_list<RagdollBody>::remove(phys_free_list<RagdollBody> *this, RagdollBody *data_)
{
  if ( data_ )
  {
    PMM_VALIDATE((char *)&data_[-1].rope_id, 0xA34u, 4u);
    phys_free_list<RagdollBody>::remove(this, (phys_free_list<RagdollBody>::T_internal *)&data_[-1].rope_id);
  }
}

void __thiscall phys_free_list<RagdollBody>::debug_add(
        phys_free_list<RagdollBody> *this,
        phys_free_list<RagdollBody>::T_internal *T_i)
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

