#pragma once

void __cdecl SND_DebugInit();
void __cdecl SND_DebugFini();
void __cdecl SND_DebugDrawWorldSounds(int debugDrawStyle);
void __cdecl DebugDrawWorldSound3D(
        unsigned int idx,
        int debugDrawStyle,
        int *offsets,
        int *closestId,
        float *closestIdDotProd);
double __cdecl FontSizeForDistance(float distance);
void __cdecl SND_PlayLocal_f();
void __cdecl RelativeToListener(const snd_listener *listener, float yaw, float pitch, float dist, float *result);
int __cdecl SND_GetSoundOverlay(snd_overlay_info *info, int start, int count);
int __cdecl SND_GetSoundOverlay(snd_overlay_type type, snd_overlay_info *info, int maxcount);
