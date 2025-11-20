#pragma once

unsigned int __cdecl SND_GetSpeakerConfigCount();
const snd_speaker_config *__cdecl Snd_GetSpeakerConfig(unsigned int index);
unsigned int __cdecl Snd_GetMixChannelCount(unsigned int speakerConfig);
double __cdecl Snd_PanMono(float angle);
double __cdecl Snd_NormalizeAngle(float x);
void __cdecl Snd_PanStereo(float angle, float boost, float *left, float *right);
void __cdecl Snd_Pan(unsigned int speakerCount, const float *angles, float toSound, float *levels);
bool __cdecl Snd_AngleInInterval(float angle, float left, float right);
void __cdecl Snd_Pan3d(
        const snd_speaker_config *config,
        const float *position,
        const float *forward,
        const float *listener,
        float centerSend,
        float lfeSend,
        float aliasOmni,
        snd_speaker_map *pan);
void __cdecl SND_EqualPowerFadeCoefs(float t, float *a, float *b);
void __cdecl SND_GetNearestPointOnSegment(
        const float *P,
        const float *segmentA,
        const float *segmentB,
        float *nearPoint);
void __cdecl SND_GetNearestPointOnStrip(
        const float *point,
        const float (*verts)[3],
        unsigned int count,
        float *position);
unsigned int __cdecl SND_HashAlias(const snd_alias_list_t *alias);
const char *__cdecl SND_GetAliasName(const snd_alias_list_t *alias);
double __cdecl SND_dBToLinear();
double __cdecl SND_LinearToDb(float linear);
double __cdecl SND_LinearToDbSpl(float linear);
double __cdecl SND_dBSPLToLinear();
int __cdecl SND_HashName(const char *name);
void __cdecl Snd_SpeakerMapSetVolume(snd_speaker_map *map, int in, int out, float volume);
void __cdecl Snd_SpeakerMapZero(snd_speaker_map *map);
int __cdecl Snd_SpeakerMapGetIndex(const snd_speaker_map *map, int in, int out);
double __cdecl Snd_SpeakerMapGetVolume(const snd_speaker_map *map, int in, int out);
void __cdecl SND_PanToSpeakermap(
        unsigned int inputChannelCount,
        unsigned int outputChannelCount,
        const snd_pan *pan,
        float centerSend,
        snd_speaker_map *map);
void __cdecl SND_PanToSpeakermap11(const snd_pan *pan, snd_speaker_map *map);
void __cdecl SND_PanToSpeakermap12(const snd_pan *pan, snd_speaker_map *map);
void __cdecl SND_PanToSpeakermap16(const snd_pan *pan, snd_speaker_map *map);
void __cdecl SND_PanToSpeakermap18(const snd_pan *pan, snd_speaker_map *map);
void __cdecl SND_PanToSpeakermap21(const snd_pan *pan, snd_speaker_map *map);
void __cdecl SND_PanToSpeakermap22(const snd_pan *pan, snd_speaker_map *map);
void __cdecl SND_PanToSpeakermap26(const snd_pan *pan, snd_speaker_map *map);
void __cdecl SND_PanToSpeakermap28(const snd_pan *pan, snd_speaker_map *map);
