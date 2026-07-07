#pragma once

#include <bgame/bg_public.h>

struct animscripted_s
{
    float axis[4][3];
    float originError[3];
    float anglesError[3];
    unsigned __int16 anim;
    unsigned __int16 root;
    unsigned __int8 bStarted;
    unsigned __int8 mode;
    float fHeightOfs;
    float fEndPitch;
    float fEndRoll;
    float fOrientLerp;
};

void __cdecl GScr_GetCycleOriginOffset();
void __cdecl G_Animscripted(
    gentity_s *ent,
    float *origin,
    const float *angles,
    unsigned int anim,
    unsigned int root,
    unsigned int notifyName,
    unsigned __int8 animMode);
void __cdecl G_Animscripted_Think(gentity_s *ent);
void __cdecl G_StopAnimScripted(gentity_s *ent, float blendTime);
