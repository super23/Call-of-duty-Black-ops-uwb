#pragma once

double __cdecl SND_LosOcclusionTrace(bool fancy, int *cache, const float *listener, const float *playback);
void __cdecl Snd_LosOcclusionMultiTrace(
        int *global_cache,
        const float *listener,
        const float *playback,
        const float *dlistener,
        const float *dplayback,
        float listen_step,
        float playback_step,
        unsigned int listen_rays,
        unsigned int playback_rays,
        unsigned int *valid,
        unsigned int *hits);
void __cdecl Vec3ScaleAdd(const float *base, const float *dir, float scale, float *dst);
// local variable allocation has failed, the output may be wrong!
void  SND_TraceProximity(
        int a1@<ebp>,
        float *front,
        float *origin,
        float *resultDistance,
        float *resultReflection);
double __cdecl SND_SurfaceTypeToReflectance(unsigned int surface);
void __cdecl SND_UpdateProximity();
void __cdecl SND_LosOcclusionCmd();
void __cdecl SND_LosOcclusionUpdate();
void __cdecl SND_LosOcclusionThreadMain();
void __cdecl SND_LosOcclusionInit();
void __cdecl SND_LosOcclusionFini();
