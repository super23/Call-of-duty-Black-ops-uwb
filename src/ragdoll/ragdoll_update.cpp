#include "ragdoll_update.h"

void __cdecl Ragdoll_AnimMatToMat43(const DObjAnimMat *mat, float (*out)[3])
{
  ConvertQuatToMat(mat, out);
  (*out)[9] = mat->trans[0];
  (*out)[10] = mat->trans[1];
  (*out)[11] = mat->trans[2];
}

char __cdecl Ragdoll_RebindBody(const cpose_t *ragdollHandle)
{
  RagdollDef *def; // [esp+0h] [ebp-1Ch]
  DObj *obj; // [esp+4h] [ebp-18h]
  RagdollBody *body; // [esp+8h] [ebp-14h]
  BoneDef *boneDef; // [esp+Ch] [ebp-10h]
  LerpBone *lerpBone; // [esp+10h] [ebp-Ch]
  int i; // [esp+14h] [ebp-8h]
  int ia; // [esp+14h] [ebp-8h]
  Bone *bone; // [esp+18h] [ebp-4h]

  body = (RagdollBody *)Ragdoll_HandleBody(ragdollHandle);
  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 838, 0, "%s", "body") )
  {
    __debugbreak();
  }
  if ( body->state <= (unsigned int)BS_DOBJ_WAIT )
    return 0;
  if ( !Ragdoll_ValidateBodyObj(body) )
    return 0;
  def = Ragdoll_BodyDef(body);
  if ( !def && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 847, 0, "%s", "def") )
    __debugbreak();
  obj = Ragdoll_BodyDObj(body);
  if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 850, 0, "%s", "obj") )
    __debugbreak();
  bone = body->bones;
  boneDef = def->boneDefs;
  for ( i = 0; i < body->numBones; ++i )
  {
    bone->parentBone = boneDef->parentBone;
    bone->animBones[1] = 0;
    bone->animBones[0] = 0;
    if ( DObjGetBoneIndex(obj, boneDef->animBoneNames[0], bone->animBones, -1) && bone->animBones[0] != 255 )
    {
      if ( boneDef->animBoneNames[1] == -1 )
      {
        bone->animBones[1] = 0;
      }
      else if ( !DObjGetBoneIndex(obj, boneDef->animBoneNames[1], &bone->animBones[1], -1) || bone->animBones[1] == 255 )
      {
        bone->animBones[1] = -1;
        bone->animBones[0] = -1;
      }
    }
    ++bone;
    ++boneDef;
  }
  lerpBone = body->lerpBones;
  for ( ia = 0; ia < body->numLerpBones; ++ia )
  {
    lerpBone->animBone = 0;
    if ( !DObjGetBoneIndex(obj, def->baseLerpBoneDefs[ia].animBoneName, &lerpBone->animBone, -1)
      || lerpBone->animBone == 255 )
    {
      return 0;
    }
    lerpBone->parentBone = def->baseLerpBoneDefs[ia].parentBoneIndex;
    ++lerpBone;
  }
  Ragdoll_SnapshotBaseLerpOffsets(body);
  return 1;
}

char __cdecl Ragdoll_ValidateBodyObj(RagdollBody *body)
{
  RagdollDef *def; // [esp+0h] [ebp-1Ch]
  DObj *obj; // [esp+4h] [ebp-18h]
  BoneDef *boneDef; // [esp+8h] [ebp-14h]
  unsigned __int8 boneIdx; // [esp+Fh] [ebp-Dh] BYREF
  int i; // [esp+10h] [ebp-Ch]
  int minBones; // [esp+14h] [ebp-8h]
  BaseLerpBoneDef *lerpBoneDef; // [esp+18h] [ebp-4h]

  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 401, 0, "%s", "body") )
  {
    __debugbreak();
  }
  def = Ragdoll_BodyDef(body);
  if ( !def && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 404, 0, "%s", "def") )
    __debugbreak();
  obj = Ragdoll_BodyDObj(body);
  if ( !obj )
    return 0;
  boneDef = def->boneDefs;
  minBones = 3;
  i = 0;
  while ( i < 3 )
  {
    boneIdx = 0;
    if ( !DObjGetBoneIndex(obj, boneDef->animBoneNames[0], &boneIdx, -1) || boneIdx == 255 )
      return 0;
    boneIdx = 0;
    if ( boneDef->animBoneNames[1] && !DObjGetBoneIndex(obj, boneDef->animBoneNames[1], &boneIdx, -1) || boneIdx == 255 )
      return 0;
    ++i;
    ++boneDef;
  }
  lerpBoneDef = def->baseLerpBoneDefs;
  i = 0;
  while ( i < body->numLerpBones )
  {
    boneIdx = 0;
    if ( !DObjGetBoneIndex(obj, lerpBoneDef->animBoneName, &boneIdx, -1) || boneIdx == 255 )
      return 0;
    ++i;
    ++lerpBoneDef;
  }
  return 1;
}

void __cdecl Ragdoll_SnapshotBaseLerpOffsets(RagdollBody *body)
{
  RagdollDef *def; // [esp+4Ch] [ebp-E8h]
  BoneOrientation *boneOrientation; // [esp+50h] [ebp-E4h]
  DObj *obj; // [esp+54h] [ebp-E0h]
  DObjAnimMat parentAnimMat; // [esp+58h] [ebp-DCh] BYREF
  DObjAnimMat boneAnimMat; // [esp+78h] [ebp-BCh] BYREF
  float boneMat[4][3]; // [esp+98h] [ebp-9Ch] BYREF
  int parentIndex; // [esp+C8h] [ebp-6Ch]
  int i; // [esp+CCh] [ebp-68h]
  float invParentMat[4][3]; // [esp+D0h] [ebp-64h] BYREF
  LerpBone *bone; // [esp+100h] [ebp-34h]
  float relMat[4][3]; // [esp+104h] [ebp-30h] BYREF

  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 760, 0, "%s", "body") )
  {
    __debugbreak();
  }
  def = Ragdoll_BodyDef(body);
  if ( !def && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 763, 0, "%s", "def") )
    __debugbreak();
  obj = Ragdoll_BodyDObj(body);
  if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 766, 0, "%s", "obj") )
    __debugbreak();
  bone = body->lerpBones;
  boneOrientation = body->lerpBoneOffsets;
  i = 0;
  while ( i < def->numBaseLerpBones )
  {
    parentIndex = bone->parentBone;
    if ( (unsigned int)parentIndex >= body->numBones
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp",
            774,
            0,
            "parentIndex doesn't index body->numBones\n\t%i not in [0, %i)",
            parentIndex,
            body->numBones) )
    {
      __debugbreak();
    }
    Ragdoll_GetDObjBaseBoneMatrix(obj, bone->animBone, &boneAnimMat);
    Ragdoll_GetDObjBaseBoneMatrix(obj, body->bones[parentIndex].animBones[0], &parentAnimMat);
    ConvertQuatToInverseMat(&parentAnimMat, invParentMat);
    Ragdoll_AnimMatToMat43(&boneAnimMat, boneMat);
    MatrixMultiply43(boneMat, invParentMat, relMat);
    AxisToQuat(relMat, boneOrientation->orientation);
    boneOrientation->origin[0] = relMat[3][0];
    boneOrientation->origin[1] = relMat[3][1];
    boneOrientation->origin[2] = relMat[3][2];
    ++i;
    ++bone;
    ++boneOrientation;
  }
}

void __cdecl Ragdoll_GetDObjBaseBoneMatrix(DObj *obj, unsigned __int8 boneIndex, DObjAnimMat *outMat)
{
  if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 96, 0, "%s", "obj") )
    __debugbreak();
  DObjGetBasePoseMatrix(obj, boneIndex, outMat);
}

bool __cdecl Ragdoll_ValidatePrecalcBoneDef(RagdollDef *def, BoneDef *bone)
{
  if ( !def && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 894, 0, "%s", "def") )
    __debugbreak();
  if ( !bone
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 895, 0, "%s", "bone") )
  {
    __debugbreak();
  }
  return COERCE_FLOAT(LODWORD(bone->mass) & _mask__AbsFloat_) >= 0.000001;
}

void __cdecl Ragdoll_ExplosionEvent(
        int localClientNum,
        bool isCylinder,
        const float *origin,
        float innerRadius,
        float outerRadius,
        const float *impulse,
        float inScale)
{
  double v7; // st7
  double v8; // st7
  double v9; // st7
  float v11; // [esp+Ch] [ebp-1D8h]
  cg_s *cgameGlob; // [esp+64h] [ebp-180h]
  RagdollBody *v13; // [esp+68h] [ebp-17Ch]
  float dist2; // [esp+6Ch] [ebp-178h]
  int i; // [esp+70h] [ebp-174h]
  RagdollBody *body; // [esp+74h] [ebp-170h]
  phys_free_list<RagdollBody>::T_internal_base *body_i; // [esp+7Ch] [ebp-168h]
  float distSqr; // [esp+80h] [ebp-164h]
  int hitCount; // [esp+84h] [ebp-160h]
  float delta; // [esp+88h] [ebp-15Ch]
  float deltaa; // [esp+88h] [ebp-15Ch]
  float delta_4; // [esp+8Ch] [ebp-158h]
  float delta_4a; // [esp+8Ch] [ebp-158h]
  float delta_8; // [esp+90h] [ebp-154h]
  float delta_8a; // [esp+90h] [ebp-154h]
  float torsoPos[3]; // [esp+94h] [ebp-150h] BYREF
  float boneScale; // [esp+A0h] [ebp-144h]
  RagdollSortStruct hitEntsSorter[32]; // [esp+A4h] [ebp-140h] BYREF
  float hitForce[3]; // [esp+1A8h] [ebp-3Ch] BYREF
  float invRange; // [esp+1B4h] [ebp-30h]
  float centerOfMass[3]; // [esp+1B8h] [ebp-2Ch] BYREF
  float ragdollOrigin[3]; // [esp+1C4h] [ebp-20h] BYREF
  int boneIdx; // [esp+1D0h] [ebp-14h]
  float innerRadiusSqr; // [esp+1D4h] [ebp-10h]
  float scale; // [esp+1D8h] [ebp-Ch]
  Bone *bone; // [esp+1DCh] [ebp-8h]
  float outerRadiusSqr; // [esp+1E0h] [ebp-4h]
  int savedregs; // [esp+1E4h] [ebp+0h] BYREF

  if ( !origin
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 957, 0, "%s", "origin") )
  {
    __debugbreak();
  }
  if ( innerRadius < 0.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp",
          958,
          0,
          "%s",
          "innerRadius >= 0.0f") )
  {
    __debugbreak();
  }
  if ( outerRadius < innerRadius
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp",
          959,
          0,
          "%s",
          "outerRadius >= innerRadius") )
  {
    __debugbreak();
  }
  if ( localClientNum == jpeg_mem_init() && outerRadius != 0.0 )
  {
    outerRadiusSqr = outerRadius * outerRadius;
    innerRadiusSqr = innerRadius * innerRadius;
    invRange = *(float *)&FLOAT_0_0;
    if ( (float)(outerRadius * outerRadius) > (float)(innerRadius * innerRadius) )
      invRange = 1.0 / (float)(outerRadiusSqr - innerRadiusSqr);
    hitCount = 0;
    for ( body_i = g_ragdoll_body_pool.m_dummy_head.m_next_T_internal;
          &g_ragdoll_body_pool != (phys_free_list<RagdollBody> *)body_i;
          body_i = body_i->m_next_T_internal )
    {
      body = (RagdollBody *)&body_i[1];
      if ( !Ragdoll_BodyInUse((RagdollBody *)&body_i[1]) )
      {
        if ( _tlAssert(
               "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp",
               978,
               "Ragdoll_BodyInUse( body )",
               &toastPopupTitle) )
        {
          __debugbreak();
        }
      }
      Ragdoll_BodyRootOrigin(body, ragdollOrigin);
      hitEntsSorter[hitCount].distSq = Vec3DistanceSq(origin, ragdollOrigin);
      hitEntsSorter[hitCount++].body = body;
    }
    std::_Sort<RagdollSortStruct *,int,bool (__cdecl *)(RagdollSortStruct const &,RagdollSortStruct const &)>(
      (MaterialMemory *)hitEntsSorter,
      (MaterialMemory *)&hitEntsSorter[hitCount],
      (8 * hitCount) >> 3,
      (bool (__cdecl *)(const MaterialMemory *, const MaterialMemory *))DynEntCl_CompareDynEntsForExplosion);
    for ( i = 0; i < 4 && i < hitCount; ++i )
    {
      v13 = hitEntsSorter[i].body;
      if ( !Ragdoll_BodyInUse(v13)
        && _tlAssert(
             "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp",
             994,
             "Ragdoll_BodyInUse( body )",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
      if ( v13->state >= BS_RUNNING )
      {
        Ragdoll_GetTorsoPosition(v13, torsoPos);
        cgameGlob = CG_GetLocalClientGlobals(localClientNum);
        dist2 = Vec3DistanceSq(cgameGlob->predictedPlayerState.origin, torsoPos);
        if ( dist2 <= (float)(ragdoll_reactivation_cutoff->current.value * ragdoll_reactivation_cutoff->current.value)
          && !R_CullPoint(localClientNum, torsoPos, -100.0) )
        {
          if ( isCylinder )
          {
            delta = *origin - torsoPos[0];
            delta_4 = origin[1] - torsoPos[1];
            delta_8 = *(float *)&FLOAT_0_0;
          }
          else
          {
            delta = torsoPos[0] - *origin;
            delta_4 = torsoPos[1] - origin[1];
            delta_8 = torsoPos[2] - origin[2];
          }
          if ( (float)((float)((float)(delta * delta) + (float)(delta_4 * delta_4)) + (float)(delta_8 * delta_8)) < outerRadiusSqr
            && (!Ragdoll_BodyIdle(v13) || Ragdoll_BodyNewState(v13, BS_RUNNING)) )
          {
            v13->stateMsec = 0;
            boneScale = 1.0 / (float)v13->numBones;
            for ( boneIdx = 0; boneIdx < v13->numBones; ++boneIdx )
            {
              scale = inScale;
              bone = &v13->bones[boneIdx];
              if ( v13->bones[boneIdx].rigidBody )
              {
                Phys_ObjGetCenterOfMass(bone->rigidBody, centerOfMass);
                v7 = flrand(-1.0, 1.0);
                centerOfMass[0] = v7 + centerOfMass[0];
                v8 = flrand(-1.0, 1.0);
                centerOfMass[1] = v8 + centerOfMass[1];
                v9 = flrand(-1.0, 1.0);
                centerOfMass[2] = v9 + centerOfMass[2];
                deltaa = centerOfMass[0] - *origin;
                delta_4a = centerOfMass[1] - origin[1];
                delta_8a = centerOfMass[2] - origin[2];
                distSqr = (float)((float)(deltaa * deltaa) + (float)(delta_4a * delta_4a))
                        + (float)(delta_8a * delta_8a);
                if ( distSqr < outerRadiusSqr )
                {
                  if ( distSqr > innerRadiusSqr )
                  {
                    if ( outerRadiusSqr <= innerRadiusSqr
                      && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp",
                            1050,
                            1,
                            "%s",
                            "outerRadiusSqr > innerRadiusSqr") )
                    {
                      __debugbreak();
                    }
                    scale = (float)((float)(outerRadiusSqr - distSqr) * invRange) * scale;
                  }
                  if ( 0.0 == *impulse && impulse[1] == 0.0 && impulse[2] == 0.0 )
                  {
                    hitForce[0] = deltaa;
                    hitForce[1] = delta_4a;
                    hitForce[2] = delta_8a;
                    if ( isCylinder )
                      hitForce[2] = *(float *)&FLOAT_0_0;
                    Vec3Normalize(hitForce);
                  }
                  else
                  {
                    hitForce[0] = *impulse;
                    hitForce[1] = impulse[1];
                    hitForce[2] = impulse[2];
                  }
                  hitForce[2] = hitForce[2] + ragdoll_explode_upbias->current.value;
                  Vec3Normalize(hitForce);
                  v11 = (float)(scale * ragdoll_explode_force->current.value) * boneScale;
                  hitForce[0] = v11 * hitForce[0];
                  hitForce[1] = v11 * hitForce[1];
                  hitForce[2] = v11 * hitForce[2];
                  Phys_ObjAddForce((int)&savedregs, bone->rigidBody, centerOfMass, hitForce, 0);
                }
              }
            }
          }
        }
      }
    }
  }
}

bool __cdecl Ragdoll_BodyIdle(RagdollBody *body)
{
  if ( !body && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll.h", 327, 0, "%s", "body") )
    __debugbreak();
  return body->state == BS_IDLE;
}

void __cdecl Ragdoll_GetTorsoPosition(RagdollBody *body, float *center)
{
  BoneOrientation *v2; // eax

  if ( (!body || !Ragdoll_BodyInUse(body))
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp",
          916,
          0,
          "%s",
          "body && Ragdoll_BodyInUse( body )") )
  {
    __debugbreak();
  }
  v2 = Ragdoll_BodyBoneOrientations(body);
  *center = v2->origin[0];
  center[1] = v2->origin[1];
  center[2] = v2->origin[2];
}

void __cdecl Ragdoll_Launch(int localClientNum, const cpose_t *ragdollHandle, const float *force, hitLocation_t hitloc)
{
  float *v4; // [esp+0h] [ebp-10h]
  float v5; // [esp+4h] [ebp-Ch]
  int numBones; // [esp+8h] [ebp-8h]
  RagdollBody *body; // [esp+Ch] [ebp-4h]

  if ( localClientNum == jpeg_mem_init() )
  {
    body = (RagdollBody *)Ragdoll_HandleBody(ragdollHandle);
    if ( !body
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 1083, 0, "%s", "body") )
    {
      __debugbreak();
    }
    if ( body->storedLaunchCount < 2 )
    {
      body->storedLaunchHitLoc[body->storedLaunchCount] = hitloc;
      numBones = body->numBones;
      if ( hitloc )
        numBones = 1;
      v4 = body->storedLaunchForce[body->storedLaunchCount];
      v5 = ragdoll_bullet_force->current.value / (float)numBones;
      *v4 = v5 * *force;
      v4[1] = v5 * force[1];
      v4[2] = v5 * force[2];
      ++body->storedLaunchCount;
    }
  }
}

void __cdecl Ragdoll_LaunchUpdate(RagdollBody *body)
{
  double v1; // st7
  double v2; // st7
  double v3; // st7
  double v4; // st7
  double v5; // st7
  double v6; // st7
  int boneIdx; // [esp+10h] [ebp-34h]
  int bi; // [esp+1Ch] [ebp-28h]
  unsigned int boneName; // [esp+20h] [ebp-24h] BYREF
  RagdollDef *def; // [esp+24h] [ebp-20h]
  float hitForce[3]; // [esp+28h] [ebp-1Ch] BYREF
  float centerOfMass[3]; // [esp+34h] [ebp-10h] BYREF
  int i; // [esp+40h] [ebp-4h]
  int savedregs; // [esp+44h] [ebp+0h] BYREF

  if ( !Ragdoll_BodyIdle(body) || Ragdoll_BodyNewState(body, BS_RUNNING) )
  {
    for ( i = 0; i < body->storedLaunchCount; ++i )
    {
      hitForce[0] = body->storedLaunchForce[i][0];
      hitForce[1] = body->storedLaunchForce[i][1];
      hitForce[2] = body->storedLaunchForce[i][2];
      if ( body->storedLaunchHitLoc[i] )
      {
        MapHitLocationToRagdollBoneName(body->storedLaunchHitLoc[i], &boneName);
        def = Ragdoll_BodyDef(body);
        for ( bi = 0; bi < def->numBones; ++bi )
        {
          if ( def->boneDefs[bi].animBoneNames[1] == boneName )
          {
            if ( body->bones[bi].rigidBody )
            {
              Phys_ObjGetCenterOfMass(body->bones[bi].rigidBody, centerOfMass);
              v1 = flrand(-1.0, 1.0);
              centerOfMass[0] = v1 + centerOfMass[0];
              v2 = flrand(-1.0, 1.0);
              centerOfMass[1] = v2 + centerOfMass[1];
              v3 = flrand(-1.0, 1.0);
              centerOfMass[2] = v3 + centerOfMass[2];
              Phys_ObjAddForce((int)&savedregs, body->bones[bi].rigidBody, centerOfMass, hitForce, 0);
            }
          }
        }
      }
      else
      {
        for ( boneIdx = 0; boneIdx < body->numBones; ++boneIdx )
        {
          if ( body->bones[boneIdx].rigidBody )
          {
            Phys_ObjGetCenterOfMass(body->bones[boneIdx].rigidBody, centerOfMass);
            v4 = flrand(-1.0, 1.0);
            centerOfMass[0] = v4 + centerOfMass[0];
            v5 = flrand(-1.0, 1.0);
            centerOfMass[1] = v5 + centerOfMass[1];
            v6 = flrand(-1.0, 1.0);
            centerOfMass[2] = v6 + centerOfMass[2];
            Phys_ObjAddForce((int)&savedregs, body->bones[boneIdx].rigidBody, centerOfMass, hitForce, 0);
          }
        }
      }
    }
    body->storedLaunchCount = 0;
  }
}

void __cdecl Ragdoll_Attach(int localClientNum, const cpose_t *ragdollHandle, int attachEnt, hitLocation_t hitloc)
{
  RagdollBody *body; // [esp+0h] [ebp-4h]

  if ( localClientNum == jpeg_mem_init() )
  {
    body = (RagdollBody *)Ragdoll_HandleBody(ragdollHandle);
    if ( !body
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 1258, 0, "%s", "body") )
    {
      __debugbreak();
    }
    if ( !body->hang_point )
    {
      body->storedAttachHitLoc = hitloc;
      body->storedAttachEnt = attachEnt;
      body->storedPhysObjId = 0;
    }
  }
}

void __cdecl Ragdoll_AttachUpdate(RagdollBody *body)
{
  if ( Ragdoll_BodyIdle(body) )
    Ragdoll_BodyNewState(body, BS_RUNNING);
}

char __cdecl Ragdoll_EnterTunnelTest(RagdollBody *body)
{
  BoneOrientation *v2; // eax
  BoneOrientation *v3; // eax
  BoneOrientation *v4; // eax
  DObj *obj; // [esp+Ch] [ebp-Ch]
  char tunnelPassed; // [esp+13h] [ebp-5h]
  centity_s *pose; // [esp+14h] [ebp-4h]
  int savedregs; // [esp+18h] [ebp+0h] BYREF

  obj = Ragdoll_BodyDObj(body);
  if ( obj && DObjGetRotTransArray(obj) && Ragdoll_CreateBodyPhysics(body) )
  {
    v2 = Ragdoll_BodyBoneOrientations(body);
    Ragdoll_SetCurrentPoseFromSnapshot(body, v2);
    Ragdoll_EstimateInitialVelocities(body);
    tunnelPassed = Ragdoll_TunnelTest((cStaticModel_s *)&savedregs, body);
    pose = Ragdoll_BodyPose(body);
    v3 = Ragdoll_BodyBoneOrientations(body);
    body->poseOffset[0] = pose->pose.origin[0] - v3->origin[0];
    body->poseOffset[1] = pose->pose.origin[1] - v3->origin[1];
    body->poseOffset[2] = pose->pose.origin[2] - v3->origin[2];
    v4 = Ragdoll_BodyBoneOrientations(body);
    Ragdoll_SnapshotBaseLerpBones(body, v4);
    if ( tunnelPassed )
    {
      Ragdoll_BodyNewState(body, BS_RUNNING);
    }
    else
    {
      Ragdoll_PrintTunnelFail(body);
      Ragdoll_BodyNewState(body, BS_DEAD);
    }
    return 1;
  }
  else
  {
    Ragdoll_BodyNewState(body, BS_DEAD);
    return 0;
  }
}

void __cdecl Ragdoll_SnapshotBaseLerpBones(RagdollBody *body, BoneOrientation *snapshot)
{
  float v2; // [esp+8h] [ebp-1A4h]
  float v3; // [esp+60h] [ebp-14Ch]
  RagdollDef *def; // [esp+64h] [ebp-148h]
  int parentBoneNum; // [esp+68h] [ebp-144h]
  DObj *obj; // [esp+6Ch] [ebp-140h]
  BoneOrientation *baseOffsetOrientation; // [esp+70h] [ebp-13Ch]
  DObjAnimMat *parentAnimMat; // [esp+74h] [ebp-138h]
  float bodyOffset[3]; // [esp+78h] [ebp-134h] BYREF
  float lerp; // [esp+84h] [ebp-128h]
  float boneMat[4][3]; // [esp+88h] [ebp-124h] BYREF
  DObjAnimMat *boneAnimMat; // [esp+B8h] [ebp-F4h]
  BoneOrientation *parentOrientation; // [esp+BCh] [ebp-F0h]
  unsigned __int8 parentBoneIdx; // [esp+C3h] [ebp-E9h]
  int goalMsec; // [esp+C4h] [ebp-E8h]
  float invAxis[3][3]; // [esp+C8h] [ebp-E4h] BYREF
  const cpose_t *pose; // [esp+ECh] [ebp-C0h]
  LerpBone *lerpBone; // [esp+F0h] [ebp-BCh]
  unsigned __int8 boneIdx; // [esp+F7h] [ebp-B5h]
  int i; // [esp+F8h] [ebp-B4h]
  float currentOffset[3]; // [esp+FCh] [ebp-B0h] BYREF
  float newLocalRot[4]; // [esp+108h] [ebp-A4h] BYREF
  float axis[3][3]; // [esp+118h] [ebp-94h] BYREF
  float currentLocalRot[4]; // [esp+13Ch] [ebp-70h] BYREF
  float invParentMat[4][3]; // [esp+14Ch] [ebp-60h] BYREF
  float relMat[4][3]; // [esp+17Ch] [ebp-30h] BYREF
  BoneOrientation *snapshota; // [esp+1B8h] [ebp+Ch]

  def = Ragdoll_BodyDef(body);
  if ( !def && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 181, 0, "%s", "def") )
    __debugbreak();
  obj = Ragdoll_BodyDObj(body);
  if ( obj )
  {
    pose = (const cpose_t *)Ragdoll_BodyPose(body);
    if ( pose )
    {
      lerpBone = body->lerpBones;
      baseOffsetOrientation = body->lerpBoneOffsets;
      snapshota = &snapshot[body->numBones];
      i = 0;
      while ( i < def->numBaseLerpBones )
      {
        goalMsec = def->baseLerpBoneDefs[i].lerpTime;
        if ( goalMsec <= 0 )
        {
          lerp = FLOAT_1_0;
        }
        else
        {
          lerp = (float)body->stateMsec / (float)goalMsec;
          v3 = (float)(lerp - 1.0) < 0.0 ? lerp : FLOAT_1_0;
          v2 = (float)(0.0 - lerp) < 0.0 ? v3 : *(float *)&FLOAT_0_0;
          lerp = v2;
        }
        boneIdx = lerpBone->animBone;
        parentBoneNum = lerpBone->parentBone;
        if ( lerpBone->parentBone >= (unsigned int)body->numBones
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp",
                212,
                0,
                "parentBoneNum doesn't index body->numBones\n\t%i not in [0, %i)",
                parentBoneNum,
                body->numBones) )
        {
          __debugbreak();
        }
        parentOrientation = &Ragdoll_BodyBoneOrientations(body)[parentBoneNum];
        parentBoneIdx = body->bones[parentBoneNum].animBones[0];
        boneAnimMat = Ragdoll_GetDObjLocalBoneMatrix(pose, obj, boneIdx);
        parentAnimMat = Ragdoll_GetDObjLocalBoneMatrix(pose, obj, parentBoneIdx);
        if ( !boneAnimMat || !parentAnimMat || boneAnimMat == parentAnimMat )
          break;
        ConvertQuatToInverseMat(parentAnimMat, invParentMat);
        Ragdoll_AnimMatToMat43(boneAnimMat, boneMat);
        MatrixMultiply43(boneMat, invParentMat, relMat);
        AxisToQuat(relMat, currentLocalRot);
        *(_QWORD *)currentOffset = *(_QWORD *)&relMat[3][0];
        currentOffset[2] = relMat[3][2];
        if ( Abs(currentOffset) <= 400.0 )
        {
          Ragdoll_QuatLerp(currentLocalRot, baseOffsetOrientation->orientation, lerp, newLocalRot);
          Vec3Lerp(currentOffset, baseOffsetOrientation->origin, lerp, bodyOffset);
        }
        else
        {
          newLocalRot[0] = baseOffsetOrientation->orientation[0];
          newLocalRot[1] = baseOffsetOrientation->orientation[1];
          newLocalRot[2] = baseOffsetOrientation->orientation[2];
          newLocalRot[3] = baseOffsetOrientation->orientation[3];
          bodyOffset[0] = baseOffsetOrientation->origin[0];
          bodyOffset[1] = baseOffsetOrientation->origin[1];
          bodyOffset[2] = baseOffsetOrientation->origin[2];
        }
        Ragdoll_QuatMul(parentOrientation->orientation, newLocalRot, snapshota->orientation);
        QuatToAxis(parentOrientation->orientation, axis);
        AxisTranspose(axis, invAxis);
        Vec3Rotate(bodyOffset, invAxis, snapshota->origin);
        snapshota->origin[0] = parentOrientation->origin[0] + snapshota->origin[0];
        snapshota->origin[1] = parentOrientation->origin[1] + snapshota->origin[1];
        snapshota->origin[2] = parentOrientation->origin[2] + snapshota->origin[2];
        ++i;
        ++lerpBone;
        ++baseOffsetOrientation;
        ++snapshota;
      }
    }
  }
}

DObjAnimMat *__cdecl Ragdoll_GetDObjLocalBoneMatrix(const cpose_t *pose, DObj *obj, unsigned __int8 boneIndex)
{
  DObjAnimMat *mat; // [esp+4h] [ebp-4h]

  if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 55, 0, "%s", "obj") )
    __debugbreak();
  CG_DObjCalcBone(pose, obj, boneIndex);
  mat = DObjGetRotTransArray(obj);
  if ( mat )
    return &mat[boneIndex];
  else
    return 0;
}

void __cdecl Ragdoll_SetCurrentPoseFromSnapshot(RagdollBody *body, BoneOrientation *snapshot)
{
  const float *qa; // [esp+4h] [ebp-3Ch]
  float error_sq; // [esp+Ch] [ebp-34h]
  rigid_body_constraint_ragdoll *id; // [esp+10h] [ebp-30h]
  int j; // [esp+14h] [ebp-2Ch]
  RagdollDef *def; // [esp+18h] [ebp-28h]
  float rotatedOrientation[4]; // [esp+1Ch] [ebp-24h] BYREF
  BoneDef *boneDef; // [esp+2Ch] [ebp-14h]
  int iters; // [esp+30h] [ebp-10h]
  int i; // [esp+34h] [ebp-Ch]
  Bone *bone; // [esp+38h] [ebp-8h]
  float max_error_sq; // [esp+3Ch] [ebp-4h]
  int savedregs; // [esp+40h] [ebp+0h] BYREF

  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 352, 0, "%s", "body") )
  {
    __debugbreak();
  }
  def = Ragdoll_BodyDef(body);
  if ( !def && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 355, 0, "%s", "def") )
    __debugbreak();
  bone = body->bones;
  boneDef = def->boneDefs;
  i = 0;
  while ( i < body->numBones )
  {
    qa = snapshot->orientation;
    if ( boneDef->mirror )
    {
      Ragdoll_QuatMul(qa, quatZRot, rotatedOrientation);
    }
    else
    {
      rotatedOrientation[0] = *qa;
      rotatedOrientation[1] = snapshot->orientation[1];
      rotatedOrientation[2] = snapshot->orientation[2];
      rotatedOrientation[3] = snapshot->orientation[3];
    }
    if ( bone->rigidBody )
      Phys_ObjSetOrientation(bone->rigidBody, snapshot->origin, rotatedOrientation);
    ++i;
    ++bone;
    ++boneDef;
    ++snapshot;
  }
  max_error_sq = FLOAT_100_0;
  iters = 0;
  while ( max_error_sq > 0.0000099999997 && iters < 100 )
  {
    ++iters;
    max_error_sq = *(float *)&FLOAT_0_0;
    for ( j = 0; j < body->numJoints; ++j )
    {
      id = (rigid_body_constraint_ragdoll *)body->joints[j].joint;
      if ( id )
      {
        error_sq = rigid_body_constraint_ragdoll::pull_together(id, (int)&savedregs);
        if ( error_sq > max_error_sq )
          max_error_sq = error_sq;
      }
    }
  }
}

char __cdecl Ragdoll_CreateBodyPhysics(RagdollBody *body)
{
  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 792, 0, "%s", "body") )
  {
    __debugbreak();
  }
  if ( body->state <= BS_VELOCITY_CAPTURE
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp",
          793,
          0,
          "%s",
          "body->state > RAGDOLL_DOBJ_VALID_STATE") )
  {
    __debugbreak();
  }
  if ( !Ragdoll_BodyInUse(body)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp",
          794,
          0,
          "%s",
          "Ragdoll_BodyInUse( body )") )
  {
    __debugbreak();
  }
  if ( Ragdoll_CreatePhysObjs(body) )
  {
    if ( Ragdoll_CreatePhysJoints(body) )
    {
      return 1;
    }
    else
    {
      Ragdoll_DestroyPhysObjs(body);
      Ragdoll_DestroyPhysJoints(body);
      return 0;
    }
  }
  else
  {
    Ragdoll_DestroyPhysObjs(body);
    return 0;
  }
}

char __cdecl Ragdoll_CreatePhysJoints(RagdollBody *body)
{
  RagdollDef *def; // [esp+0h] [ebp-8h]
  int i; // [esp+4h] [ebp-4h]

  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 618, 0, "%s", "body") )
  {
    __debugbreak();
  }
  def = Ragdoll_BodyDef(body);
  if ( !def && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 621, 0, "%s", "def") )
    __debugbreak();
  for ( i = 0; i < body->numJoints; ++i )
  {
    if ( !Ragdoll_CreatePhysJoint(body, &def->jointDefs[i], &body->joints[i]) )
      return 0;
  }
  return 1;
}

char __cdecl Ragdoll_CreatePhysJoint(RagdollBody *body, JointDef *jointDef, Joint *joint)
{
  JointType type; // [esp+10h] [ebp-9Ch]
  Bone *parentBone; // [esp+1Ch] [ebp-90h]
  float limitAxes[3][3]; // [esp+20h] [ebp-8Ch] BYREF
  float tAxis[3][3]; // [esp+44h] [ebp-68h] BYREF
  float anchor[3]; // [esp+68h] [ebp-44h] BYREF
  float fric[3]; // [esp+74h] [ebp-38h] BYREF
  int i; // [esp+80h] [ebp-2Ch]
  float axis[3][3]; // [esp+84h] [ebp-28h] BYREF
  Bone *bone; // [esp+A8h] [ebp-4h]
  int savedregs; // [esp+ACh] [ebp+0h] BYREF

  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 561, 0, "%s", "body") )
  {
    __debugbreak();
  }
  if ( !jointDef
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 562, 0, "%s", "jointDef") )
  {
    __debugbreak();
  }
  if ( !joint
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 563, 0, "%s", "joint") )
  {
    __debugbreak();
  }
  if ( (unsigned int)jointDef->bone >= body->numBones
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp",
          564,
          0,
          "jointDef->bone doesn't index body->numBones\n\t%i not in [0, %i)",
          jointDef->bone,
          body->numBones) )
  {
    __debugbreak();
  }
  bone = &body->bones[jointDef->bone];
  if ( bone->parentBone == -1 )
    parentBone = 0;
  else
    parentBone = &body->bones[bone->parentBone];
  if ( !bone->rigidBody )
    return 1;
  Phys_ObjGetPosition((int)&savedregs, bone->rigidBody, anchor, tAxis);
  AxisTranspose(tAxis, axis);
  if ( !jointDef->numLimitAxes )
  {
    jointDef->limitAxes[0][0] = *(float *)&FLOAT_0_0;
    jointDef->limitAxes[0][1] = *(float *)&FLOAT_0_0;
    jointDef->limitAxes[0][2] = FLOAT_1_0;
    jointDef->axisFriction[0] = *(float *)&FLOAT_0_0;
    jointDef->minAngles[0] = FLOAT_N1_5707964;
    jointDef->maxAngles[0] = FLOAT_1_5707964;
    ++jointDef->numLimitAxes;
  }
  for ( i = 0; i < jointDef->numLimitAxes; ++i )
    Vec3Rotate(jointDef->limitAxes[i], axis, limitAxes[i]);
  fric[0] = 15.0 * jointDef->axisFriction[0];
  fric[1] = 15.0 * jointDef->axisFriction[1];
  fric[2] = 15.0 * jointDef->axisFriction[2];
  type = jointDef->type;
  if ( type == RAGDOLL_JOINT_HINGE )
  {
    joint->joint = (int)Phys_CreateHinge(
                          (rigid_body_constraint_ragdoll *)&savedregs,
                          bone->rigidBody,
                          parentBone->rigidBody,
                          anchor,
                          limitAxes[0],
                          0.0,
                          fric[0],
                          jointDef->minAngles[0],
                          jointDef->maxAngles[0]);
    joint->joint2 = 0;
  }
  else if ( type == RAGDOLL_JOINT_SWIVEL )
  {
    joint->joint = (int)Phys_CreateSwivel(
                          (rigid_body_constraint_ragdoll *)&savedregs,
                          bone->rigidBody,
                          parentBone->rigidBody,
                          anchor,
                          jointDef->numLimitAxes,
                          limitAxes,
                          vec3_origin,
                          fric,
                          jointDef->minAngles,
                          jointDef->maxAngles);
  }
  return 1;
}

char __cdecl Ragdoll_CreatePhysObjs(RagdollBody *body)
{
  broad_phase_info *v2; // eax
  broad_phase_info *bpi; // [esp+28h] [ebp-3Ch]
  PhysObjUserData *userData; // [esp+38h] [ebp-2Ch]
  PhysObjUserData *userData1; // [esp+40h] [ebp-24h]
  broad_phase_collision_pair *bpcp; // [esp+44h] [ebp-20h]
  PhysObjUserData *userData0; // [esp+48h] [ebp-1Ch]
  int i0; // [esp+4Ch] [ebp-18h]
  int i1; // [esp+50h] [ebp-14h]
  int j; // [esp+54h] [ebp-10h]
  RagdollDef *def; // [esp+58h] [ebp-Ch]
  int i; // [esp+5Ch] [ebp-8h]
  int ia; // [esp+5Ch] [ebp-8h]

  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 635, 0, "%s", "body") )
  {
    __debugbreak();
  }
  if ( physGlob.objects_by_world[0].m_alloc_count / 12 >= 5 )
    cdl_proftimer::start_capture(&proftimer_physics_frame_advance);
  def = Ragdoll_BodyDef(body);
  if ( !def && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 646, 0, "%s", "def") )
    __debugbreak();
  Sys_EnterCriticalSection(CRITSECT_PHYSICS);
  for ( i = 0; i < body->numBones; ++i )
  {
    if ( !Ragdoll_CreatePhysObj(body, &def->boneDefs[i], &body->bones[i]) )
    {
      Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
      return 0;
    }
  }
  body->m_list_bpcp = 0;
  for ( j = 0; j < def->numSelfPairs; ++j )
  {
    i0 = def->selfPairDefs[j].bones[0];
    i1 = def->selfPairDefs[j].bones[1];
    if ( body->bones[i0].rigidBody && body->bones[i1].rigidBody )
    {
      userData0 = (PhysObjUserData *)body->bones[i0].rigidBody;
      userData1 = (PhysObjUserData *)body->bones[i1].rigidBody;
      if ( (!userData0->m_bpb || (userData0->m_bpb->m_flags & 1) == 0)
        && _tlAssert(
             "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp",
             667,
             "userData0->m_bpb && userData0->m_bpb->is_bpi()",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
      if ( !userData1->m_bpb || (userData1->m_bpb->m_flags & 1) == 0 )
      {
        if ( _tlAssert(
               "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp",
               668,
               "userData1->m_bpb && userData1->m_bpb->is_bpi()",
               &toastPopupTitle) )
        {
          __debugbreak();
        }
      }
      bpcp = create_broad_phase_collision_pair();
      if ( bpcp )
      {
        bpi = broad_phase_base::get_bpi(userData1->m_bpb);
        bpcp->m_bpi1 = broad_phase_base::get_bpi(userData0->m_bpb);
        bpcp->m_bpi2 = bpi;
        bpcp->m_next_bpcp = body->m_list_bpcp;
        body->m_list_bpcp = bpcp;
      }
    }
  }
  body->m_bpg = create_broad_phase_group();
  broad_phase_group::set(body->m_bpg);
  for ( ia = 0; ia < body->numBones; ++ia )
  {
    if ( body->bones[ia].rigidBody )
    {
      userData = (PhysObjUserData *)body->bones[ia].rigidBody;
      if ( (!userData->m_bpb || (userData->m_bpb->m_flags & 1) == 0)
        && _tlAssert(
             "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp",
             685,
             "userData->m_bpb && userData->m_bpb->is_bpi()",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
      aasap_list_remove(userData->m_bpb);
      userData->m_bpb->m_env_collision_flags &= ~0x80u;
      userData->m_bpb->m_my_collision_type_flags |= 0x200u;
      environment_collision_list_remove(userData->m_bpb);
      v2 = broad_phase_base::get_bpi(userData->m_bpb);
      broad_phase_group::add_bpi(body->m_bpg, v2);
      userData->m_bpb = 0;
    }
  }
  environment_collision_list_add(body->m_bpg);
  aasap_list_add(body->m_bpg);
  Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
  return 1;
}

char __cdecl Ragdoll_CreatePhysObj(RagdollBody *body, BoneDef *boneDef, Bone *bone)
{
  gjk_double_sphere_t *capsule_gjk_geom; // eax
  float qRot[4]; // [esp+40h] [ebp-E4h] BYREF
  RagdollDef *def; // [esp+50h] [ebp-D4h]
  float hlength; // [esp+54h] [ebp-D0h]
  DObj *obj; // [esp+58h] [ebp-CCh]
  float b1Origin[3]; // [esp+5Ch] [ebp-C8h] BYREF
  int dir; // [esp+68h] [ebp-BCh]
  float b0Origin[3]; // [esp+6Ch] [ebp-B8h] BYREF
  PhysObjUserData *userData; // [esp+78h] [ebp-ACh]
  centity_s *cent; // [esp+7Ch] [ebp-A8h]
  float b0Quat[4]; // [esp+80h] [ebp-A4h] BYREF
  float offset; // [esp+90h] [ebp-94h]
  float invPoseAxis[3][3]; // [esp+94h] [ebp-90h] BYREF
  const cpose_t *pose; // [esp+B8h] [ebp-6Ch]
  gjk_geom_list_t gjk_geom_list; // [esp+BCh] [ebp-68h] BYREF
  PhysPreset preset; // [esp+C4h] [ebp-60h] BYREF
  int boneIdx; // [esp+120h] [ebp-4h]
  int savedregs; // [esp+124h] [ebp+0h] BYREF

  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 448, 0, "%s", "body") )
  {
    __debugbreak();
  }
  if ( !boneDef
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 449, 0, "%s", "boneDef") )
  {
    __debugbreak();
  }
  if ( !bone
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 450, 0, "%s", "bone") )
  {
    __debugbreak();
  }
  def = Ragdoll_BodyDef(body);
  if ( !def && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 453, 0, "%s", "def") )
    __debugbreak();
  obj = Ragdoll_BodyDObj(body);
  if ( !obj )
    return 0;
  if ( !def->bound
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 460, 0, "%s", "def->bound") )
  {
    __debugbreak();
  }
  if ( !boneDef->animBoneNames[0]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp",
          461,
          0,
          "%s",
          "boneDef->animBoneNames[0]") )
  {
    __debugbreak();
  }
  bone->animBones[0] = 0;
  bone->animBones[1] = 0;
  if ( !DObjGetBoneIndex(obj, boneDef->animBoneNames[0], bone->animBones, -1) || bone->animBones[0] == 255 )
    return 1;
  if ( boneDef->animBoneNames[1] )
  {
    if ( !DObjGetBoneIndex(obj, boneDef->animBoneNames[1], &bone->animBones[1], -1) || bone->animBones[0] == 255 )
    {
      bone->animBones[0] = -1;
      bone->animBones[1] = -1;
      return 1;
    }
  }
  else
  {
    bone->animBones[1] = 0;
  }
  pose = (const cpose_t *)Ragdoll_BodyPose(body);
  if ( !pose
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 484, 0, "%s", "pose") )
  {
    __debugbreak();
  }
  Ragdoll_PoseInvAxis(pose, invPoseAxis);
  Ragdoll_GetDObjBaseBoneOriginQuat(obj, pose->origin, invPoseAxis, bone->animBones[0], b0Origin, qRot);
  if ( boneDef->mirror )
  {
    Ragdoll_QuatMul(qRot, quatZRot, b0Quat);
  }
  else
  {
    b0Quat[0] = qRot[0];
    b0Quat[1] = qRot[1];
    b0Quat[2] = qRot[2];
    b0Quat[3] = qRot[3];
  }
  if ( bone->animBones[1] )
  {
    Ragdoll_GetDObjBaseBoneOrigin(obj, pose->origin, invPoseAxis, bone->animBones[1], b1Origin);
  }
  else
  {
    b1Origin[0] = b0Origin[0];
    b1Origin[1] = b0Origin[1];
    b1Origin[2] = b0Origin[2];
  }
  boneIdx = bone - body->bones;
  preset.bounce = *(float *)&FLOAT_0_0;
  preset.bulletForceScale = FLOAT_1_0;
  preset.explosiveForceScale = FLOAT_1_0;
  preset.friction = boneDef->friction;
  preset.mass = boneDef->mass;
  preset.name = "ragdoll_bone";
  preset.sndAliasPrefix = 0;
  preset.canFloat = 1;
  memset(preset.centerOfMassOffset, 0, 36);
  bone->parentBone = boneDef->parentBone;
  bone->length = Vec3Distance(b0Origin, b1Origin);
  bone->center[0] = *(float *)&FLOAT_0_0;
  bone->center[1] = *(float *)&FLOAT_0_0;
  bone->center[2] = *(float *)&FLOAT_0_0;
  hlength = bone->length * 0.5;
  dir = boneIdx != 0 ? 0 : 2;
  bone->center[dir] = bone->length * 0.5;
  offset = *(float *)&FLOAT_0_0;
  if ( boneIdx == 3 )
    offset = FLOAT_2_0;
  bone->center[dir] = bone->center[dir] + offset;
  hlength = hlength + offset;
  gjk_geom_list.m_first_geom = 0;
  gjk_geom_list.m_geom_count = 0;
  capsule_gjk_geom = create_capsule_gjk_geom(
                       COERCE_FLOAT(&savedregs),
                       bone->center,
                       boneDef->radius,
                       hlength,
                       0,
                       7,
                       &g_empty_collision_visitor);
  gjk_geom_list_t::add_geom(&gjk_geom_list, capsule_gjk_geom);
  bone->rigidBody = (int)Phys_ObjCreate(2, b0Origin, b0Quat, vec3_origin, &preset, &gjk_geom_list, 0, -1);
  cent = CG_GetEntity(body->localClientNum, body->dobj);
  if ( bone->rigidBody )
  {
    userData = (PhysObjUserData *)bone->rigidBody;
    if ( ((*((unsigned int *)cent + 201) >> 19) & 1) != 0 )
      userData->m_flags |= 4u;
    if ( ((*((unsigned int *)cent + 201) >> 20) & 1) != 0 )
      userData->m_flags |= 0x10u;
    userData->m_flags |= 0x20u;
    return 1;
  }
  else
  {
    Com_PrintWarning(14, "Ragdoll: Failed to create rigid body\n");
    return 0;
  }
}

char __cdecl Ragdoll_GetDObjBaseBoneOrigin(
        DObj *obj,
        const float *offset,
        const float (*axis)[3],
        unsigned __int8 boneIndex,
        float *origin)
{
  DObjAnimMat mat; // [esp+0h] [ebp-20h] BYREF

  if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 106, 0, "%s", "obj") )
    __debugbreak();
  Ragdoll_GetDObjBaseBoneMatrix(obj, boneIndex, &mat);
  Vec3Rotate(mat.trans, axis, origin);
  *origin = *offset + *origin;
  origin[1] = offset[1] + origin[1];
  origin[2] = offset[2] + origin[2];
  return 1;
}

char __cdecl Ragdoll_GetDObjBaseBoneOriginQuat(
        DObj *obj,
        const float *offset,
        const float (*axis)[3],
        unsigned __int8 boneIndex,
        float *origin,
        float *quat)
{
  DObjAnimMat mat; // [esp+0h] [ebp-30h] BYREF
  float orientation[4]; // [esp+20h] [ebp-10h] BYREF

  if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 122, 0, "%s", "obj") )
    __debugbreak();
  Ragdoll_GetDObjBaseBoneMatrix(obj, boneIndex, &mat);
  Ragdoll_Mat33ToQuat(axis, orientation);
  Ragdoll_QuatMul(orientation, mat.quat, quat);
  Vec3Rotate(mat.trans, axis, origin);
  *origin = *offset + *origin;
  origin[1] = offset[1] + origin[1];
  origin[2] = offset[2] + origin[2];
  return 1;
}

void __cdecl Ragdoll_PoseInvAxis(const cpose_t *pose, float (*invAxis)[3])
{
  float axis[3][3]; // [esp+0h] [ebp-24h] BYREF

  if ( !pose
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 140, 0, "%s", "pose") )
  {
    __debugbreak();
  }
  AnglesToAxis(pose->angles, axis);
  AxisTranspose(axis, invAxis);
}

void __cdecl Ragdoll_DestroyPhysJoints(RagdollBody *body)
{
  memset((unsigned __int8 *)body->joints, 0, sizeof(body->joints));
}

void __cdecl Ragdoll_DestroyPhysObjs(RagdollBody *body)
{
  int i; // [esp+8h] [ebp-4h]

  Sys_EnterCriticalSection(CRITSECT_PHYSICS);
  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 720, 0, "%s", "body") )
  {
    __debugbreak();
  }
  if ( body->m_list_bpcp )
  {
    destroy_broad_phase_collision_pair_list(body->m_list_bpcp);
    body->m_list_bpcp = 0;
  }
  if ( body->m_bpg )
  {
    destroy_broad_phase_info_list(body->m_bpg->m_list_bpi_head);
    aasap_list_remove(body->m_bpg);
    environment_collision_list_remove(body->m_bpg);
    destroy_broad_phase_group(body->m_bpg);
    body->m_bpg = 0;
  }
  for ( i = 0; i < body->numBones; ++i )
  {
    if ( body->bones[i].rigidBody )
      Phys_ObjDestroy(2, body->bones[i].rigidBody);
    body->bones[i].rigidBody = 0;
  }
  Sys_LeaveCriticalSection(CRITSECT_PHYSICS);
}

void __cdecl Ragdoll_EstimateInitialVelocities(RagdollBody *body)
{
  float v1; // [esp+0h] [ebp-A4h]
  float v2; // [esp+4h] [ebp-A0h]
  float len2; // [esp+38h] [ebp-6Ch]
  float angleOffset[3]; // [esp+3Ch] [ebp-68h] BYREF
  float posOffset[3]; // [esp+48h] [ebp-5Ch] BYREF
  BoneOrientation *prevOrientation; // [esp+54h] [ebp-50h]
  float timeScale; // [esp+58h] [ebp-4Ch]
  float prevRot[4]; // [esp+5Ch] [ebp-48h] BYREF
  float curRot[4]; // [esp+6Ch] [ebp-38h] BYREF
  BoneOrientation *curOrientation; // [esp+7Ch] [ebp-28h]
  float rotOffset[4]; // [esp+80h] [ebp-24h] BYREF
  int i; // [esp+90h] [ebp-14h]
  float angleOffsetWorld[3]; // [esp+94h] [ebp-10h] BYREF
  Bone *bone; // [esp+A0h] [ebp-4h]
  int savedregs; // [esp+A4h] [ebp+0h] BYREF

  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 1175, 0, "%s", "body") )
  {
    __debugbreak();
  }
  if ( body->velCaptureMsec )
  {
    timeScale = 1000.0 / (float)body->velCaptureMsec;
    curOrientation = Ragdoll_BodyBoneOrientations(body);
    prevOrientation = Ragdoll_BodyPrevBoneOrientations(body);
    bone = body->bones;
    i = 0;
    while ( i < body->numBones )
    {
      if ( bone->rigidBody )
      {
        posOffset[0] = curOrientation->origin[0] - prevOrientation->origin[0];
        posOffset[1] = curOrientation->origin[1] - prevOrientation->origin[1];
        posOffset[2] = curOrientation->origin[2] - prevOrientation->origin[2];
        if ( (float)((float)((float)((float)(curOrientation->orientation[0] * prevOrientation->orientation[0])
                                   + (float)(curOrientation->orientation[1] * prevOrientation->orientation[1]))
                           + (float)(curOrientation->orientation[2] * prevOrientation->orientation[2]))
                   + (float)(curOrientation->orientation[3] * prevOrientation->orientation[3])) >= 0.0 )
        {
          prevRot[0] = prevOrientation->orientation[0];
          prevRot[1] = prevOrientation->orientation[1];
          prevRot[2] = prevOrientation->orientation[2];
          prevRot[3] = prevOrientation->orientation[3];
        }
        else
        {
          LODWORD(prevRot[0]) = LODWORD(prevOrientation->orientation[0]) ^ _mask__NegFloat_;
          LODWORD(prevRot[1]) = LODWORD(prevOrientation->orientation[1]) ^ _mask__NegFloat_;
          LODWORD(prevRot[2]) = LODWORD(prevOrientation->orientation[2]) ^ _mask__NegFloat_;
          LODWORD(prevRot[3]) = LODWORD(prevOrientation->orientation[3]) ^ _mask__NegFloat_;
        }
        curRot[0] = curOrientation->orientation[0];
        curRot[1] = curOrientation->orientation[1];
        curRot[2] = curOrientation->orientation[2];
        curRot[3] = curOrientation->orientation[3];
        Vec4Normalize(curRot);
        Vec4Normalize(prevRot);
        Ragdoll_QuatMulInvSecond(curRot, prevRot, rotOffset);
        Vec4Normalize(rotOffset);
        Ragdoll_QuatToAxisAngle(rotOffset, angleOffset);
        Ragdoll_QuatPointRotate(angleOffset, curRot, angleOffsetWorld);
        v2 = timeScale * ragdoll_rotvel_scale->current.value;
        angleOffset[0] = v2 * angleOffsetWorld[0];
        angleOffset[1] = v2 * angleOffsetWorld[1];
        angleOffset[2] = v2 * angleOffsetWorld[2];
        posOffset[0] = timeScale * posOffset[0];
        posOffset[1] = timeScale * posOffset[1];
        posOffset[2] = timeScale * posOffset[2];
        Phys_ObjSetAngularVelocityRaw((int)&savedregs, bone->rigidBody, angleOffset);
        len2 = (float)((float)(posOffset[0] * posOffset[0]) + (float)(posOffset[1] * posOffset[1]))
             + (float)(posOffset[2] * posOffset[2]);
        if ( len2 > (float)(600.0 * 600.0) )
        {
          v1 = 600.0 / fsqrt(len2);
          posOffset[0] = v1 * posOffset[0];
          posOffset[1] = v1 * posOffset[1];
          posOffset[2] = v1 * posOffset[2];
        }
        if ( posOffset[2] > clamp )
          posOffset[2] = clamp;
        Phys_ObjSetVelocity((int)&savedregs, bone->rigidBody, posOffset);
      }
      ++i;
      ++curOrientation;
      ++prevOrientation;
      ++bone;
    }
  }
}

char  Ragdoll_TunnelTest@<al>(cStaticModel_s *a1@<ebp>, RagdollBody *body)
{
  float *v3; // [esp+44h] [ebp-1D0h]
  float v4; // [esp+70h] [ebp-1A4h]
  float v5; // [esp+74h] [ebp-1A0h]
  float v6; // [esp+78h] [ebp-19Ch] BYREF
  float v7; // [esp+7Ch] [ebp-198h]
  float ndif; // [esp+80h] [ebp-194h]
  phys_vec3 dif; // [esp+84h] [ebp-190h]
  float v10; // [esp+94h] [ebp-180h]
  float v11; // [esp+98h] [ebp-17Ch]
  float v12; // [esp+9Ch] [ebp-178h]
  float v13; // [esp+A0h] [ebp-174h]
  phys_vec3 last_position; // [esp+A4h] [ebp-170h]
  float v15; // [esp+B4h] [ebp-160h]
  int v16; // [esp+B8h] [ebp-15Ch]
  PhysObjUserData *userData; // [esp+BCh] [ebp-158h]
  float sphere_radius; // [esp+C0h] [ebp-154h]
  float v19; // [esp+C8h] [ebp-14Ch] BYREF
  float v20; // [esp+CCh] [ebp-148h]
  float v21; // [esp+D0h] [ebp-144h]
  phys_vec3 sphere_center; // [esp+D4h] [ebp-140h] BYREF
  float newPos[3]; // [esp+ECh] [ebp-128h] BYREF
  float center[4]; // [esp+F8h] [ebp-11Ch] BYREF
  float end[3]; // [esp+108h] [ebp-10Ch] BYREF
  float v26; // [esp+114h] [ebp-100h]
  float start[3]; // [esp+118h] [ebp-FCh] BYREF
  float boneMat[3][3]; // [esp+124h] [ebp-F0h] BYREF
  float orientation[4]; // [esp+148h] [ebp-CCh]
  BoneOrientation *v30; // [esp+158h] [ebp-BCh]
  BoneOrientation *v31; // [esp+15Ch] [ebp-B8h]
  int i; // [esp+160h] [ebp-B4h]
  BoneOrientation *boneOrientations; // [esp+164h] [ebp-B0h]
  BoneOrientation *curOrientation; // [esp+168h] [ebp-ACh]
  BoneDef *boneDef; // [esp+16Ch] [ebp-A8h] BYREF
  Bone *bone; // [esp+170h] [ebp-A4h]
  RagdollDef *def; // [esp+174h] [ebp-A0h]
  trace_t revTrace; // [esp+178h] [ebp-9Ch] BYREF
  col_context_t context; // [esp+1B0h] [ebp-64h] BYREF
  trace_t trace; // [esp+1D8h] [ebp-3Ch] BYREF
  int retaddr; // [esp+214h] [ebp+0h]

  trace.staticModel = a1;
  trace.hitPartition = retaddr;
  memset(&context.locational, 0, 12);
  trace.normal.vec.u[0] = *(unsigned int *)&FLOAT_0_0;
  col_context_t::col_context_t((col_context_t *)&revTrace.walkable);
  boneDef = *(BoneDef **)&FLOAT_0_0;
  bone = *(Bone **)&FLOAT_0_0;
  def = *(RagdollDef **)&FLOAT_0_0;
  revTrace.normal.vec.u[0] = *(unsigned int *)&FLOAT_0_0;
  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 1447, 0, "%s", "body") )
  {
    __debugbreak();
  }
  curOrientation = (BoneOrientation *)Ragdoll_BodyDef(body);
  if ( !curOrientation
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 1450, 0, "%s", "def") )
  {
    __debugbreak();
  }
  boneOrientations = (BoneOrientation *)body->bones;
  i = (int)&curOrientation->origin[2];
  v31 = Ragdoll_BodyBoneOrientations(body);
  v30 = v31;
  orientation[3] = 0.0;
  while ( SLODWORD(orientation[3]) < body->numBones )
  {
    if ( LODWORD(boneOrientations->origin[2]) )
    {
      LOBYTE(orientation[2]) = *(_BYTE *)(i + 52);
      LODWORD(orientation[1]) = v31->orientation;
      if ( LOBYTE(orientation[2]) )
      {
        Ragdoll_QuatMul((const float *)LODWORD(orientation[1]), quatZRot, boneMat[2]);
      }
      else
      {
        *(_QWORD *)&boneMat[2][0] = *(_QWORD *)LODWORD(orientation[1]);
        boneMat[2][2] = *(float *)(LODWORD(orientation[1]) + 8);
        orientation[0] = *(float *)(LODWORD(orientation[1]) + 12);
      }
      QuatToAxis(boneMat[2], (float (*)[3])start);
      end[1] = v31->origin[0];
      end[2] = v31->origin[1];
      v26 = v31->origin[2];
      end[0] = *(float *)&boneOrientations->boneFlags;
      center[1] = (float)(end[0] * start[0]) + v31->origin[0];
      center[2] = (float)(end[0] * start[1]) + v31->origin[1];
      center[3] = (float)(end[0] * start[2]) + v31->origin[2];
      if ( LODWORD(orientation[3]) )
      {
        center[0] = *(float *)&boneOrientations->boneFlags * 0.5;
        newPos[0] = (float)(center[0] * start[0]) + end[1];
        newPos[1] = (float)(center[0] * start[1]) + end[2];
        newPos[2] = (float)(center[0] * start[2]) + v26;
        CM_BoxTrace(
          (trace_t *)&context.locational,
          v30->origin,
          newPos,
          vec3_origin,
          vec3_origin,
          (int)&cls.recentServers[7546].city[57],
          (col_context_t *)&revTrace.walkable);
        if ( HIBYTE(trace.hitType) )
          return 0;
        if ( BYTE2(trace.hitType)
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp",
                1486,
                0,
                "%s",
                "!trace.allsolid") )
        {
          __debugbreak();
        }
        if ( trace.normal.vec.v[1] < 1.0 )
        {
          trace.normal.vec.v[1] = trace.normal.vec.v[1] * 0.80000001;
          Vec3Lerp(v30->origin, newPos, trace.normal.vec.v[1], &sphere_center.w);
          Phys_ObjSetPosition((int)&trace.staticModel, LODWORD(boneOrientations->origin[2]), &sphere_center.w);
        }
      }
      else if ( !Ragdoll_BoneTrace((trace_t *)&context.locational, (trace_t *)&boneDef, &end[1], &center[1]) )
      {
        return 0;
      }
    }
    ++LODWORD(orientation[3]);
    boneOrientations = (BoneOrientation *)((char *)boneOrientations + 28);
    i += 80;
    ++v31;
  }
  LODWORD(sphere_center.z) = Ragdoll_BodyPose(body);
  Phys_Vec3ToNitrousVec((float *)(LODWORD(sphere_center.z) + 48), (phys_vec3 *)&v19);
  v16 = *(unsigned int *)&FLOAT_0_0;
  userData = *(PhysObjUserData **)&FLOAT_0_0;
  sphere_radius = FLOAT_15_0;
  v19 = v19 + 0.0;
  v20 = v20 + 0.0;
  v21 = v21 + 15.0;
  v15 = FLOAT_10_0;
  boneOrientations = (BoneOrientation *)body->bones;
  LODWORD(orientation[3]) = 1;
  while ( SLODWORD(orientation[3]) < body->numBones )
  {
    if ( LODWORD(boneOrientations->origin[2]) )
    {
      last_position.w = boneOrientations->origin[2];
      LODWORD(last_position.z) = *(unsigned int *)LODWORD(last_position.w) + 48;
      v11 = *(float *)(LODWORD(last_position.z) + 48);
      v12 = *(float *)(LODWORD(last_position.z) + 52);
      v13 = *(float *)(LODWORD(last_position.z) + 56);
      last_position.x = *(float *)(LODWORD(last_position.z) + 60);
      v10 = v11 - v19;
      dif.w = v12 - v20;
      dif.z = v13 - v21;
      v6 = v11 - v19;
      v7 = v12 - v20;
      ndif = v13 - v21;
      v5 = Abs(&v6);
      if ( v5 > v15 )
      {
        v4 = (float)(v5 - v15) / v5;
        v11 = v11 - (float)(v4 * v6);
        v12 = v12 - (float)(v4 * v7);
        v13 = v13 - (float)(v4 * ndif);
      }
      v3 = *(float **)LODWORD(last_position.w);
      *v3 = v11;
      v3[1] = v12;
      v3[2] = v13;
      if ( COERCE_FLOAT(**(unsigned int **)LODWORD(last_position.w) & _mask__AbsFloat_) > 100000.0
        || COERCE_FLOAT(*(unsigned int *)(*(unsigned int *)LODWORD(last_position.w) + 4) & _mask__AbsFloat_) > 100000.0
        || COERCE_FLOAT(*(unsigned int *)(*(unsigned int *)LODWORD(last_position.w) + 8) & _mask__AbsFloat_) > 100000.0 )
      {
        phys_exec_debug_callback(*(void **)LODWORD(last_position.w));
      }
      BG_EvalVehicleName();
    }
    ++LODWORD(orientation[3]);
    boneOrientations = (BoneOrientation *)((char *)boneOrientations + 28);
  }
  body->m_bpg->m_flags |= 0x200u;
  return 1;
}

char __cdecl Ragdoll_BoneTrace(trace_t *trace, trace_t *revTrace, const float *start, const float *end)
{
  col_context_t context; // [esp+0h] [ebp-28h] BYREF

  col_context_t::col_context_t(&context);
  CM_BoxTrace(trace, start, end, vec3_origin, vec3_origin, (int)&cls.recentServers[7546].city[57], &context);
  if ( trace->startsolid )
  {
    CM_BoxTrace(revTrace, end, start, vec3_origin, vec3_origin, (int)&cls.recentServers[7546].city[57], &context);
    if ( revTrace->startsolid )
      return 0;
  }
  else if ( trace->fraction != 1.0 )
  {
    CM_BoxTrace(revTrace, end, start, vec3_origin, vec3_origin, (int)&cls.recentServers[7546].city[57], &context);
    if ( revTrace->fraction != 1.0 )
      return 0;
  }
  return 1;
}

void __cdecl Ragdoll_PrintTunnelFail(RagdollBody *body)
{
  DObj *obj; // [esp+18h] [ebp-8h]
  centity_s *pose; // [esp+1Ch] [ebp-4h]

  if ( ragdoll_dump_anims->current.enabled )
  {
    pose = Ragdoll_BodyPose(body);
    obj = Ragdoll_BodyDObj(body);
    if ( !pose
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 1549, 0, "%s", "pose") )
    {
      __debugbreak();
    }
    if ( !obj
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 1550, 0, "%s", "obj") )
    {
      __debugbreak();
    }
    Com_Printf(
      20,
      "Ragdoll initial state in solid, using regular anim. Pos (%0.f %0.f %0.f)\n",
      pose->pose.origin[0],
      pose->pose.origin[1],
      pose->pose.origin[2]);
    DObjDisplayAnim(obj, "Ragdoll anim tree: ");
  }
}

void __cdecl Ragdoll_UpdateVelocityCapture(RagdollBody *body)
{
  BoneOrientation *snapshot; // [esp+0h] [ebp-4h]
  BoneOrientation *snapshota; // [esp+0h] [ebp-4h]
  BoneOrientation *snapshotb; // [esp+0h] [ebp-4h]

  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 1609, 0, "%s", "body") )
  {
    __debugbreak();
  }
  if ( body->stateFrames >= 1 )
  {
    body->velCaptureMsec = body->stateMsec;
    body->curOrientationBuffer ^= 1u;
    snapshotb = Ragdoll_BodyBoneOrientations(body);
    Ragdoll_SnapshotAnimOrientations(body, snapshotb);
    Ragdoll_BodyNewState(body, BS_TUNNEL_TEST);
  }
  else
  {
    body->curOrientationBuffer ^= 1u;
    snapshot = Ragdoll_BodyBoneOrientations(body);
    Ragdoll_SnapshotAnimOrientations(body, snapshot);
    if ( (body->flags & 4) != 0 )
    {
      body->velCaptureMsec = 1;
      body->curOrientationBuffer ^= 1u;
      snapshota = Ragdoll_BodyBoneOrientations(body);
      Ragdoll_SnapshotAnimOrientations(body, snapshota);
      Ragdoll_BodyNewState(body, BS_TUNNEL_TEST);
    }
  }
}

void __cdecl Ragdoll_SnapshotAnimOrientations(RagdollBody *body, BoneOrientation *snapshot)
{
  RagdollDef *def; // [esp+0h] [ebp-18h]
  DObj *obj; // [esp+4h] [ebp-14h]
  BoneDef *boneDef; // [esp+8h] [ebp-10h]
  centity_s *pose; // [esp+Ch] [ebp-Ch]
  int i; // [esp+10h] [ebp-8h]
  Bone *bone; // [esp+14h] [ebp-4h]

  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 315, 0, "%s", "body") )
  {
    __debugbreak();
  }
  def = Ragdoll_BodyDef(body);
  obj = Ragdoll_BodyDObj(body);
  if ( obj )
  {
    pose = Ragdoll_BodyPose(body);
    if ( def && obj && pose )
    {
      bone = body->bones;
      boneDef = def->boneDefs;
      i = 0;
      while ( i < body->numBones )
      {
        if ( bone->animBones[0] != 255 )
          Ragdoll_GetDObjWorldBoneOriginQuat(
            body->localClientNum,
            &pose->pose,
            obj,
            bone->animBones[0],
            snapshot->origin,
            snapshot->orientation);
        ++i;
        ++bone;
        ++boneDef;
        ++snapshot;
      }
    }
    else
    {
      Ragdoll_BodyNewState(body, BS_DEAD);
    }
  }
}

char __cdecl Ragdoll_GetDObjWorldBoneOriginQuat(
        int localClientNum,
        const cpose_t *pose,
        DObj *obj,
        unsigned __int8 boneIndex,
        float *origin,
        float *quat)
{
  DObjAnimMat *mat; // [esp+4h] [ebp-4h]

  if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 80, 0, "%s", "obj") )
    __debugbreak();
  if ( !pose && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 81, 0, "%s", "pose") )
    __debugbreak();
  mat = Ragdoll_GetDObjLocalBoneMatrix(pose, obj, boneIndex);
  if ( !mat )
    return 0;
  *quat = mat->quat[0];
  quat[1] = mat->quat[1];
  quat[2] = mat->quat[2];
  quat[3] = mat->quat[3];
  *origin = mat->trans[0];
  origin[1] = mat->trans[1];
  origin[2] = mat->trans[2];
  return 1;
}

char __cdecl Ragdoll_EnterDead(RagdollBody *body)
{
  int references; // [esp+0h] [ebp-8h]
  int ragdollDef; // [esp+4h] [ebp-4h]

  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 1864, 0, "%s", "body") )
  {
    __debugbreak();
  }
  Ragdoll_RemoveBodyPhysics(body);
  references = body->references;
  ragdollDef = body->ragdollDef;
  memset((unsigned __int8 *)body, 0, sizeof(RagdollBody));
  body->references = references;
  body->ragdollDef = ragdollDef;
  return 1;
}

void __cdecl Ragdoll_RemoveBodyPhysics(RagdollBody *body)
{
  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 814, 0, "%s", "body") )
  {
    __debugbreak();
  }
  Ragdoll_DestroyPhysJoints(body);
  Ragdoll_DestroyPhysObjs(body);
  body->debug_hang_point = 0;
  body->hang_point = 0;
}

char __cdecl Ragdoll_ExitDead(RagdollBody *body)
{
  RagdollDef *def; // [esp+0h] [ebp-4h]

  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 1883, 0, "%s", "body") )
  {
    __debugbreak();
  }
  def = Ragdoll_BodyDef(body);
  if ( !def && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 1886, 0, "%s", "def") )
    __debugbreak();
  body->numBones = def->numBones;
  body->numLerpBones = 0;
  body->numJoints = def->numJoints;
  return 1;
}

char __cdecl Ragdoll_ExitDObjWait(RagdollBody *body, RagdollBodyState prevState, RagdollBodyState curState)
{
  RagdollDef *def; // [esp+0h] [ebp-18h]
  DObj *obj; // [esp+4h] [ebp-14h]
  BoneDef *boneDef; // [esp+8h] [ebp-10h]
  LerpBone *lerpBone; // [esp+Ch] [ebp-Ch]
  int i; // [esp+10h] [ebp-8h]
  int ia; // [esp+10h] [ebp-8h]
  Bone *bone; // [esp+14h] [ebp-4h]

  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 1905, 0, "%s", "body") )
  {
    __debugbreak();
  }
  if ( curState == BS_DEAD )
    return 1;
  def = Ragdoll_BodyDef(body);
  if ( !def && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 1911, 0, "%s", "def") )
    __debugbreak();
  obj = Ragdoll_BodyDObj(body);
  if ( !obj && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 1914, 0, "%s", "obj") )
    __debugbreak();
  bone = body->bones;
  boneDef = def->boneDefs;
  for ( i = 0; i < body->numBones; ++i )
  {
    bone->parentBone = boneDef->parentBone;
    bone->animBones[1] = 0;
    bone->animBones[0] = 0;
    if ( DObjGetBoneIndex(obj, boneDef->animBoneNames[0], bone->animBones, -1) && bone->animBones[0] != 255 )
    {
      if ( boneDef->animBoneNames[1] == -1 )
      {
        bone->animBones[1] = 0;
      }
      else if ( !DObjGetBoneIndex(obj, boneDef->animBoneNames[1], &bone->animBones[1], -1) || bone->animBones[1] == 255 )
      {
        bone->animBones[1] = -1;
        bone->animBones[0] = -1;
      }
    }
    ++bone;
    ++boneDef;
  }
  lerpBone = body->lerpBones;
  for ( ia = 0; ia < body->numLerpBones; ++ia )
  {
    lerpBone->animBone = 0;
    if ( !DObjGetBoneIndex(obj, def->baseLerpBoneDefs[ia].animBoneName, &lerpBone->animBone, -1)
      || lerpBone->animBone == 255 )
    {
      return 0;
    }
    lerpBone->parentBone = def->baseLerpBoneDefs[ia].parentBoneIndex;
    ++lerpBone;
  }
  Ragdoll_SnapshotBaseLerpOffsets(body);
  return 1;
}

char __cdecl Ragdoll_ExitIdle(RagdollBody *body, RagdollBodyState curState, RagdollBodyState newState)
{
  BoneOrientation *v4; // eax

  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 1958, 0, "%s", "body") )
  {
    __debugbreak();
  }
  if ( (unsigned int)newState <= BS_DOBJ_WAIT )
    return 1;
  if ( !Ragdoll_ValidateBodyObj(body) || !Ragdoll_BodyPoseValid(body) )
    return 0;
  if ( Ragdoll_CountPhysicsBodies() >= ragdoll_max_simulating->current.integer )
    return 0;
  if ( !Ragdoll_CreateBodyPhysics(body) )
    return 0;
  v4 = Ragdoll_BodyBoneOrientations(body);
  Ragdoll_SetCurrentPoseFromSnapshot(body, v4);
  return 1;
}

char __cdecl Ragdoll_EnterIdle(RagdollBody *body)
{
  BoneOrientation *v1; // eax

  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 1980, 0, "%s", "body") )
  {
    __debugbreak();
  }
  if ( Ragdoll_BodyHasPhysics(body) )
  {
    v1 = Ragdoll_BodyBoneOrientations(body);
    Ragdoll_SnapshotBonePositions(body, v1);
  }
  Ragdoll_RemoveBodyPhysics(body);
  return 1;
}

void __cdecl Ragdoll_SnapshotBonePositions(RagdollBody *body, BoneOrientation *boneSnapshot)
{
  float *dest; // [esp+0h] [ebp-2Ch]
  RagdollDef *def; // [esp+8h] [ebp-24h]
  BoneOrientation *snapshot; // [esp+Ch] [ebp-20h]
  float boneRot[4]; // [esp+10h] [ebp-1Ch] BYREF
  BoneDef *boneDef; // [esp+20h] [ebp-Ch]
  int i; // [esp+24h] [ebp-8h]
  Bone *bone; // [esp+28h] [ebp-4h]

  if ( (!body || !boneSnapshot)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp",
          263,
          0,
          "%s",
          "body && boneSnapshot") )
  {
    __debugbreak();
  }
  if ( Ragdoll_BodyHasPhysics(body) )
  {
    def = Ragdoll_BodyDef(body);
    if ( !def
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 269, 0, "%s", "def") )
    {
      __debugbreak();
    }
    bone = body->bones;
    boneDef = def->boneDefs;
    snapshot = boneSnapshot;
    i = 0;
    while ( i < body->numBones )
    {
      if ( bone->rigidBody )
      {
        Phys_ObjGetInterpolatedState(bone->rigidBody, snapshot->origin, boneRot);
        dest = snapshot->orientation;
        if ( boneDef->mirror )
        {
          Ragdoll_QuatMul(boneRot, quatZRot, dest);
        }
        else
        {
          *dest = boneRot[0];
          snapshot->orientation[1] = boneRot[1];
          snapshot->orientation[2] = boneRot[2];
          snapshot->orientation[3] = boneRot[3];
        }
      }
      ++i;
      ++bone;
      ++snapshot;
      ++boneDef;
    }
    Ragdoll_SnapshotBaseLerpBones(body, boneSnapshot);
  }
}

void __cdecl Ragdoll_UpdateDObjWait(RagdollBody *body)
{
  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 1992, 0, "%s", "body") )
  {
    __debugbreak();
  }
  if ( body->stateFrames <= 3 )
  {
    if ( Ragdoll_ValidateBodyObj(body) )
      Ragdoll_BodyNewState(body, BS_VELOCITY_CAPTURE);
  }
  else
  {
    if ( body->obj )
      Com_PrintWarning(20, "Ragdoll activation timed out waiting for dobj 0x%x\n", body->dobj);
    else
      Com_PrintWarning(20, "Ragdoll activation timed out waiting for dobj 0x%x\n", body->obj);
    Ragdoll_BodyNewState(body, BS_DEAD);
  }
}

char __cdecl Ragdoll_EnterDobjWait(RagdollBody *body)
{
  Ragdoll_UpdateDObjWait(body);
  return 1;
}

void __cdecl Ragdoll_UpdateRunning(RagdollBody *body, int msec)
{
  int savedregs; // [esp+0h] [ebp+0h] BYREF

  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 2074, 0, "%s", "body") )
  {
    __debugbreak();
  }
  if ( Ragdoll_CheckIdle(body, msec) )
  {
    Ragdoll_BodyNewState(body, BS_IDLE);
  }
  else
  {
    Ragdoll_FilterBonePositions(body);
    Ragdoll_UpdateFriction(body);
    Ragdoll_LaunchUpdate(body);
    Ragdoll_AttachUpdate(body);
    Ragdoll_DebugRender((int)&savedregs, body);
  }
}

void __cdecl Ragdoll_UpdateFriction(RagdollBody *body)
{
  JointType type; // [esp+10h] [ebp-38h]
  float v2; // [esp+14h] [ebp-34h]
  float v3; // [esp+1Ch] [ebp-2Ch]
  float v4; // [esp+20h] [ebp-28h]
  RagdollDef *def; // [esp+28h] [ebp-20h]
  float frictionVec[3]; // [esp+2Ch] [ebp-1Ch] BYREF
  Joint *joint; // [esp+38h] [ebp-10h]
  float lerpScale; // [esp+3Ch] [ebp-Ch]
  int i; // [esp+40h] [ebp-8h]
  JointDef *jointDef; // [esp+44h] [ebp-4h]

  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 1648, 0, "%s", "body") )
  {
    __debugbreak();
  }
  def = Ragdoll_BodyDef(body);
  if ( !def && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 1651, 0, "%s", "def") )
    __debugbreak();
  jointDef = def->jointDefs;
  joint = body->joints;
  v3 = (float)body->stateMsec / (float)ragdoll_jointlerp_time->current.integer;
  if ( (float)(v3 - 1.0) < 0.0 )
    v4 = (float)body->stateMsec / (float)ragdoll_jointlerp_time->current.integer;
  else
    v4 = FLOAT_1_0;
  if ( (float)(0.0 - v3) < 0.0 )
    v2 = v4;
  else
    v2 = *(float *)&FLOAT_0_0;
  lerpScale = v2 + 1.0;
  if ( (body->flags & 2) != 0 )
    lerpScale = waterFrictionScale;
  i = 0;
  while ( i < def->numJoints )
  {
    type = jointDef->type;
    if ( type == RAGDOLL_JOINT_HINGE )
    {
      if ( joint->joint )
        Phys_SetHingeParams((rigid_body_constraint_ragdoll *)joint->joint, 0.0, jointDef->axisFriction[0] * lerpScale);
    }
    else if ( type == RAGDOLL_JOINT_SWIVEL )
    {
      frictionVec[0] = lerpScale * jointDef->axisFriction[0];
      frictionVec[1] = lerpScale * jointDef->axisFriction[1];
      frictionVec[2] = lerpScale * jointDef->axisFriction[2];
      if ( joint->joint )
        Phys_SetAngularMotorParams((rigid_body_constraint_ragdoll *)joint->joint, vec3_origin, frictionVec);
    }
    ++i;
    ++jointDef;
    ++joint;
  }
}

void  Ragdoll_DebugRender(int a1@<ebp>, RagdollBody *body)
{
  int joint2; // edx
  environment_rigid_body *environment_rigid_body; // eax
  phys_vec3 v4; // [esp-2Ch] [ebp-6Ch] BYREF
  float v5; // [esp-10h] [ebp-50h]
  unsigned int v6[2]; // [esp-Ch] [ebp-4Ch] BYREF
  float z; // [esp-4h] [ebp-44h]
  phys_vec3 world_pos; // [esp+0h] [ebp-40h]
  environment_rigid_body *v9; // [esp+10h] [ebp-30h]
  environment_rigid_body **v10; // [esp+14h] [ebp-2Ch]
  int rigidBody; // [esp+18h] [ebp-28h]
  rigid_body *body1; // [esp+1Ch] [ebp-24h]
  PhysObjUserData *userData; // [esp+20h] [ebp-20h]
  int id; // [esp+24h] [ebp-1Ch]
  int v15; // [esp+28h] [ebp-18h]
  int v16; // [esp+2Ch] [ebp-14h]
  int i; // [esp+30h] [ebp-10h]
  Joint joint; // [esp+34h] [ebp-Ch] BYREF
  int retaddr; // [esp+40h] [ebp+0h]

  joint.joint = a1;
  joint.joint2 = retaddr;
  if ( ragdoll_debug->current.integer > 0 )
  {
    if ( ragdoll_debug->current.integer == 4 )
    {
      for ( i = 0; i < body->numJoints; ++i )
      {
        joint2 = body->joints[i].joint2;
        v15 = body->joints[i].joint;
        v16 = joint2;
        Phys_JointDebugRender((rigid_body *)&joint, v15);
      }
    }
    for ( id = 0; id < body->numBones; ++id )
    {
      if ( (renderflag & (1 << id)) != 0 )
      {
        userData = (PhysObjUserData *)body->bones[id].rigidBody;
        body1 = (rigid_body *)userData;
        debug_render((int)&joint, userData);
      }
    }
  }
  rigidBody = body->bones[hangBone].rigidBody;
  v10 = (environment_rigid_body **)rigidBody;
  if ( ragdoll_debug->current.integer <= 1 )
  {
    if ( body->debug_hang_point )
    {
      rigid_body::set_flag(body->debug_hang_point->b1, 0x40u, 0);
      phys_sys::destroy(body->debug_hang_point);
      body->debug_hang_point = 0;
    }
  }
  else if ( !body->debug_hang_point )
  {
    v9 = *v10;
    v6[0] = LODWORD(v9->m_mat.w.x);
    v6[1] = LODWORD(v9->m_mat.w.y);
    z = v9->m_mat.w.z;
    world_pos.x = v9->m_mat.w.w;
    v5 = z;
    z = z + 60.0;
    environment_rigid_body = phys_sys::get_environment_rigid_body();
    body->debug_hang_point = phys_sys::create_rbc_point(v9, environment_rigid_body, 0);
    memset(&v4, 0, 12);
    rigid_body_constraint_point::set(body->debug_hang_point, &v4, (const phys_vec3 *)v6);
    rigid_body::set_flag(v9, 0x40u, 1);
  }
}

bool __cdecl Ragdoll_CheckIdle(RagdollBody *body, int msec)
{
  float tvel[3]; // [esp+4h] [ebp-2Ch] BYREF
  float avel[3]; // [esp+10h] [ebp-20h] BYREF
  float zvel; // [esp+1Ch] [ebp-14h]
  int i; // [esp+20h] [ebp-10h]
  int stableBoneCount; // [esp+24h] [ebp-Ch]
  int totalBoneCount; // [esp+28h] [ebp-8h]
  bool stable; // [esp+2Fh] [ebp-1h]

  if ( body->debug_hang_point )
    return 0;
  if ( body->hang_point )
    return 0;
  if ( body->rope_id )
    return 0;
  zvel = *(float *)&FLOAT_0_0;
  totalBoneCount = 0;
  for ( i = 0; i < body->numBones; ++i )
  {
    if ( body->bones[i].rigidBody )
    {
      ++totalBoneCount;
      Phys_ObjGetVelocities(body->bones[i].rigidBody, tvel, avel);
      zvel = zvel + tvel[2];
    }
  }
  zvel = zvel / (float)totalBoneCount;
  if ( zvel < -2500.0 )
    return 1;
  body->flags &= ~2u;
  if ( body->stateMsec > ragdoll_max_life->current.integer )
    return 1;
  stableBoneCount = 0;
  for ( i = 0; i < body->numBones; ++i )
  {
    if ( body->bones[i].rigidBody && Phys_ObjIsAsleepSingle(body->bones[i].rigidBody) )
      ++stableBoneCount;
  }
  stable = (float)stableBoneCount > (float)((float)totalBoneCount * 0.75);
  if ( (float)stableBoneCount <= (float)((float)totalBoneCount * 0.75) )
    body->stableTime = 0;
  else
    body->stableTime += msec;
  return body->stableTime > 250;
}

void __cdecl Ragdoll_FilterBonePositions(RagdollBody *body)
{
  BoneOrientation *v1; // eax

  v1 = Ragdoll_BodyBoneOrientations(body);
  Ragdoll_SnapshotBonePositions(body, v1);
}

char __cdecl Ragdoll_BodyNewState(RagdollBody *body, RagdollBodyState state)
{
  RagdollBodyState prevState; // [esp+4h] [ebp-4h]

  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 2132, 0, "%s", "body") )
  {
    __debugbreak();
  }
  if ( (unsigned int)state >= RAGDOLL_NUM_STATES
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp",
          2133,
          0,
          "state doesn't index RAGDOLL_NUM_STATES\n\t%i not in [0, %i)",
          state,
          6) )
  {
    __debugbreak();
  }
  prevState = body->state;
  if ( (unsigned int)prevState >= RAGDOLL_NUM_STATES
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp",
          2136,
          0,
          "prevState doesn't index RAGDOLL_NUM_STATES\n\t%i not in [0, %i)",
          prevState,
          6) )
  {
    __debugbreak();
  }
  if ( prevState == state )
    return 1;
  if ( stateEntries[prevState].exitFunc && !stateEntries[prevState].exitFunc(body, prevState, state) )
    return 0;
  body->state = state;
  if ( !stateEntries[state].enterFunc || stateEntries[state].enterFunc(body, prevState, state) )
  {
    body->stateFrames = 0;
    body->stateMsec = 0;
    body->stableTime = 0;
    return 1;
  }
  else
  {
    if ( body->state == state )
      body->state = prevState;
    return 0;
  }
}

void __cdecl Ragdoll_BodyUpdate(int msec, RagdollBody *body)
{
  StateEnt *entry; // [esp+0h] [ebp-8h]
  RagdollBodyState prevState; // [esp+4h] [ebp-4h]

  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp", 2177, 0, "%s", "body") )
  {
    __debugbreak();
  }
  if ( body->state >= (unsigned int)RAGDOLL_NUM_STATES
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp",
          2178,
          0,
          "body->state doesn't index RAGDOLL_NUM_STATES\n\t%i not in [0, %i)",
          body->state,
          6) )
  {
    __debugbreak();
  }
  do
  {
    prevState = body->state;
    entry = &stateEntries[prevState];
    if ( entry->updateFunc )
      entry->updateFunc(body, msec);
  }
  while ( prevState != body->state );
  ++body->stateFrames;
  body->stateMsec += msec;
}

void __cdecl Ragdoll_Update(int msec)
{
  phys_free_list<RagdollBody>::T_internal_base *body_i; // [esp+Ch] [ebp-4h]

  if ( !Sys_IsMainThread()
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp",
          2203,
          0,
          "%s",
          "Sys_IsMainThread()") )
  {
    __debugbreak();
  }
  if ( ragdollInited && ragdoll_enable->current.enabled && cg_paused && !cg_paused->current.integer )
  {
    for ( body_i = g_ragdoll_body_pool.m_dummy_head.m_next_T_internal;
          &g_ragdoll_body_pool != (phys_free_list<RagdollBody> *)body_i;
          body_i = body_i->m_next_T_internal )
    {
      if ( !Ragdoll_BodyInUse((RagdollBody *)&body_i[1]) )
      {
        if ( _tlAssert(
               "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp",
               2214,
               "Ragdoll_BodyInUse( body )",
               &toastPopupTitle) )
        {
          __debugbreak();
        }
      }
      Ragdoll_BodyUpdate(msec, (RagdollBody *)&body_i[1]);
    }
  }
}

void __cdecl Ragdoll_RemoveConstraintsForRope(phys_free_list<RagdollBody>::T_internal_base *rope_id)
{
  phys_free_list<RagdollBody>::T_internal_base *body_i; // [esp+10h] [ebp-4h]

  for ( body_i = g_ragdoll_body_pool.m_dummy_head.m_next_T_internal;
        &g_ragdoll_body_pool != (phys_free_list<RagdollBody> *)body_i;
        body_i = body_i->m_next_T_internal )
  {
    if ( !Ragdoll_BodyInUse((RagdollBody *)&body_i[1])
      && _tlAssert(
           "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp",
           2226,
           "Ragdoll_BodyInUse( body )",
           &toastPopupTitle) )
    {
      __debugbreak();
    }
    if ( (phys_free_list<RagdollBody>::T_internal_base *)((char *)body_i[325].m_prev_T_internal - 1) == rope_id )
    {
      if ( !body_i[325].m_next_T_internal
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp",
              2229,
              0,
              "%s",
              "body->rbc_dist") )
      {
        __debugbreak();
      }
      phys_sys::destroy((rigid_body_constraint_distance *const)body_i[325].m_next_T_internal);
      body_i[325].m_next_T_internal = 0;
      body_i[325].m_prev_T_internal = 0;
      body_i[6].m_prev_T_internal = 0;
    }
  }
}

int __cdecl Ragdoll_GetRBForBone(phys_free_list<RagdollBody>::T_internal_base *entnum, unsigned int boneName)
{
  int bi; // [esp+Ch] [ebp-14h]
  RagdollDef *def; // [esp+10h] [ebp-10h]
  RagdollBody *body; // [esp+14h] [ebp-Ch]
  phys_free_list<RagdollBody>::T_internal_base *body_i; // [esp+1Ch] [ebp-4h]

  for ( body_i = g_ragdoll_body_pool.m_dummy_head.m_next_T_internal;
        &g_ragdoll_body_pool != (phys_free_list<RagdollBody> *)body_i;
        body_i = body_i->m_next_T_internal )
  {
    body = (RagdollBody *)&body_i[1];
    if ( !Ragdoll_BodyInUse((RagdollBody *)&body_i[1]) )
    {
      if ( _tlAssert(
             "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_update.cpp",
             2244,
             "Ragdoll_BodyInUse( body )",
             &toastPopupTitle) )
      {
        __debugbreak();
      }
    }
    if ( entnum == body_i[2].m_prev_T_internal )
    {
      def = Ragdoll_BodyDef(body);
      for ( bi = 0; bi < def->numBones; ++bi )
      {
        if ( def->boneDefs[bi].animBoneNames[1] == boneName )
          return body->bones[bi].rigidBody;
      }
      return 0;
    }
  }
  return 0;
}

