#pragma once

const cpose_t *__cdecl Ragdoll_HandleBody(const cpose_t *pose);
BoneOrientation *__cdecl Ragdoll_BodyBoneOrientations(RagdollBody *body);
BoneOrientation *__cdecl Ragdoll_BodyPrevBoneOrientations(RagdollBody *body);
void __cdecl Ragdoll_DoControllers(const cpose_t *pose, const DObj *obj, int *partBits);
