#include "ragdoll_controller.h"

const cpose_t *__cdecl Ragdoll_HandleBody(const cpose_t *pose)
{
  return pose;
}

BoneOrientation *__cdecl Ragdoll_BodyBoneOrientations(RagdollBody *body)
{
  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_controller.cpp", 37, 0, "%s", "body") )
  {
    __debugbreak();
  }
  if ( body->curOrientationBuffer >= 2u
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_controller.cpp",
          38,
          0,
          "body->curOrientationBuffer doesn't index 2\n\t%i not in [0, %i)",
          body->curOrientationBuffer,
          2) )
  {
    __debugbreak();
  }
  return body->boneOrientations[body->curOrientationBuffer];
}

BoneOrientation *__cdecl Ragdoll_BodyPrevBoneOrientations(RagdollBody *body)
{
  if ( !body
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_controller.cpp", 45, 0, "%s", "body") )
  {
    __debugbreak();
  }
  if ( body->curOrientationBuffer >= 2u
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_controller.cpp",
          46,
          0,
          "body->curOrientationBuffer doesn't index 2\n\t%i not in [0, %i)",
          body->curOrientationBuffer,
          2) )
  {
    __debugbreak();
  }
  return body->boneOrientations[body->curOrientationBuffer ^ 1];
}

void __cdecl Ragdoll_DoControllers(const cpose_t *pose, const DObj *obj, int *partBits)
{
  RagdollBody *v3; // eax
  float v4; // [esp+0h] [ebp-3Ch]
  float v5; // [esp+Ch] [ebp-30h]
  BoneOrientation *boneOrientation; // [esp+1Ch] [ebp-20h]
  RagdollBody *body; // [esp+20h] [ebp-1Ch]
  DObjAnimMat *boneMat; // [esp+24h] [ebp-18h]
  DObjAnimMat *boneMata; // [esp+24h] [ebp-18h]
  DObjAnimMat *skel; // [esp+28h] [ebp-14h]
  LerpBone *lerpBone; // [esp+2Ch] [ebp-10h]
  int boneIdx; // [esp+30h] [ebp-Ch]
  int boneIdxa; // [esp+30h] [ebp-Ch]
  int i; // [esp+34h] [ebp-8h]
  int ia; // [esp+34h] [ebp-8h]
  Bone *bone; // [esp+38h] [ebp-4h]

  if ( !pose
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_controller.cpp", 63, 0, "%s", "pose") )
  {
    __debugbreak();
  }
  if ( !obj
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ragdoll\\ragdoll_controller.cpp", 64, 0, "%s", "obj") )
  {
    __debugbreak();
  }
  if ( pose->isRagdoll && (pose->ragdollHandle || pose->killcamRagdollHandle) )
  {
    v3 = (RagdollBody *)(pose->killcamRagdollHandle
                       ? Ragdoll_HandleBody((const cpose_t *)pose->killcamRagdollHandle)
                       : Ragdoll_HandleBody((const cpose_t *)pose->ragdollHandle));
    body = v3;
    if ( Ragdoll_BodyPoseValid(v3) )
    {
      skel = DObjGetRotTransArray(obj);
      if ( skel )
      {
        bone = body->bones;
        boneOrientation = Ragdoll_BodyBoneOrientations(body);
        for ( i = 0; i < body->numBones; ++i )
        {
          boneIdx = bone->animBones[0];
          if ( DObjSetSkelRotTransIndex(obj, partBits, boneIdx) )
          {
            boneMat = &skel[boneIdx];
            boneMat->trans[0] = boneOrientation->origin[0];
            boneMat->trans[1] = boneOrientation->origin[1];
            boneMat->trans[2] = boneOrientation->origin[2];
            boneMat->quat[0] = boneOrientation->orientation[0];
            boneMat->quat[1] = boneOrientation->orientation[1];
            boneMat->quat[2] = boneOrientation->orientation[2];
            boneMat->quat[3] = boneOrientation->orientation[3];
            v5 = Vec4LengthSq(boneMat->quat);
            if ( v5 == 0.0 )
            {
              boneMat->quat[3] = FLOAT_1_0;
              boneMat->transWeight = FLOAT_2_0;
            }
            else
            {
              boneMat->transWeight = 2.0 / v5;
            }
          }
          ++bone;
          ++boneOrientation;
        }
        lerpBone = body->lerpBones;
        for ( ia = 0; ia < body->numLerpBones; ++ia )
        {
          boneIdxa = lerpBone->animBone;
          if ( DObjSetSkelRotTransIndex(obj, partBits, boneIdxa) )
          {
            boneMata = &skel[boneIdxa];
            boneMata->trans[0] = boneOrientation->origin[0];
            boneMata->trans[1] = boneOrientation->origin[1];
            boneMata->trans[2] = boneOrientation->origin[2];
            boneMata->quat[0] = boneOrientation->orientation[0];
            boneMata->quat[1] = boneOrientation->orientation[1];
            boneMata->quat[2] = boneOrientation->orientation[2];
            boneMata->quat[3] = boneOrientation->orientation[3];
            v4 = Vec4LengthSq(boneMata->quat);
            if ( v4 == 0.0 )
            {
              boneMata->quat[3] = FLOAT_1_0;
              boneMata->transWeight = FLOAT_2_0;
            }
            else
            {
              boneMata->transWeight = 2.0 / v4;
            }
          }
          ++lerpBone;
          ++boneOrientation;
        }
      }
    }
  }
}

