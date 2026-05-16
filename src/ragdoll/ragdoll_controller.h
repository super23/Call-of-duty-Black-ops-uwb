#pragma once

struct cpose_t;
struct RagdollBody;

RagdollBody *__cdecl Ragdoll_HandleBody(int handle);
struct BoneOrientation *__cdecl Ragdoll_BodyBoneOrientations(struct RagdollBody *body);
struct BoneOrientation *__cdecl Ragdoll_BodyPrevBoneOrientations(struct RagdollBody *body);
void __cdecl Ragdoll_DoControllers(const cpose_t *pose, const struct DObj *obj, int *partBits);
