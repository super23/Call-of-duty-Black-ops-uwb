#pragma once

bool __cdecl SND_ShouldInit();
void __cdecl SND_SetPosition(unsigned int index, float *org);
unsigned int __cdecl SND_ActiveListenerCount();
int __cdecl SND_GetListenerIndexNearestToOrigin(const float *origin);
void __cdecl SND_GetNearestListenerPosition(const float *position, float *listener);
int __cdecl SND_SetPlaybackIdNotPlayed(unsigned int index);
int __cdecl SND_AcquirePlaybackId();
char __cdecl SND_AddLengthNotify(int playbackId, const char *lengthNotifyData, snd_length_type id);
void __cdecl DoLengthNotify(unsigned int msec, const char *lengthNotifyData, snd_length_type id);
void __cdecl SND_StartLengthNotify(unsigned int index, unsigned int totalMsec);
void __cdecl SND_ResetVoiceInfo(int index);
double __cdecl SND_GetSeed(unsigned int key, unsigned int global_age);
void __cdecl SND_SetVoiceStartInfo(unsigned int index, SndStartAliasInfo *SndStartAliasInfo);
void __cdecl SND_FaderSetRate(snd_fader_t *fader, float r);
void __cdecl SND_FaderSetGoal(snd_fader_t *fader, float g);
bool __cdecl SND_IsAliasPausable(const snd_alias_t *alias);
void __cdecl SND_SetVoiceStartFlux(snd_voice_t *voice, float *player);
void __cdecl SND_SetVoiceStartSeeds(const snd_alias_t *alias, snd_voice_t *voice);
void __cdecl SND_SetVoiceStartFades(float fadetime, snd_voice_t *voice);
void __cdecl SND_FaderSetRateTime(snd_fader_t *fader, float time);
void __cdecl SND_FaderSetValue(snd_fader_t *fader, float v);
void __cdecl SND_FaderUpdate(snd_fader_t *fader, float dt);
void __cdecl SND_UpdateVoicePosition(snd_voice_t *voice, const float *startPosition);
void __cdecl SND_SetSoundFileVoiceInfo(
        unsigned int voiceIndex,
        int srcChannelCount,
        int baserate,
        unsigned int total_msec,
        int start_msec,
        SndFileLoadingState loadingState);
unsigned int __cdecl SND_FindFreeVoice(SndStartAliasInfo *startAliasInfo);
double __cdecl Snd_GetGlobalPriority(const snd_alias_t *alias, float volume);
double __cdecl Snd_GetGlobalPriorityVolume(const snd_alias_t *alias, const float *org);
void __cdecl Snd_GetLowestPriority(float *priority, int *channel, unsigned int start, unsigned int count);
unsigned int __cdecl SND_ContinueLoopingSound(
        unsigned int aliasId,
        float volumeScale,
        SndEntHandle sndEnt,
        float *org,
        int fadeTime,
        snd_playback *playback);
void __cdecl SND_ContinueLoopingSound_Internal(
        unsigned int voiceIndex,
        float volumeScale,
        int fadeTime,
        float *org,
        snd_playback *playback);
unsigned int __cdecl SND_GetCurrentContext(unsigned int type);
snd_alias_t *__cdecl SND_PickSoundAliasFromList(const snd_alias_list_t *aliasList, int objectId);
snd_alias_t *__cdecl SND_PickSoundAlias(const char *name, int objectid);
void __cdecl SND_AssertValidData(const snd_alias_t *alias);
unsigned int __cdecl SND_PlaySoundAlias(
        const snd_alias_list_t *aliasList,
        float volumeScale,
        SndEntHandle sndEnt,
        float *org,
        int timeshift,
        int fadeTime,
        const float *direction,
        snd_playback *playback,
        snd_occlusion_start_cache *ocache);
char __cdecl SND_CheckValidSecondary(const snd_alias_t *alias, const snd_alias_t *secondaryAlias);
char __cdecl SND_Limit(
        const char *name,
        unsigned int aliasHash,
        snd_limit_type_t limitType,
        int limitCount,
        SndEntHandle ent,
        bool useEnt,
        float priority);
void __cdecl SND_GetPlayingInfo(
        unsigned int aliasHash,
        int *pcount,
        int *poldest,
        int *least,
        SndEntHandle ent,
        bool useEnt);
char __cdecl SND_LimitVoice(const snd_alias_t *alias, SndEntHandle ent);
bool __cdecl SND_IsAliasVoice(const snd_alias_t *alias);
int __cdecl SND_GetCurrentReverb();
void __cdecl SND_UpdateDebugAlias();
void __cdecl SNDL_Update();
void SND_UpdatePause();
void SND_PauseSounds();
void SND_UnpauseSounds();
void __cdecl SND_UpdateMasterVolumes(float dt);
void __cdecl SND_UpdateVoices(int frametime);
void __cdecl SND_UpdateRoomEffects(int frametime);
void SND_UpdateStaticSounds();
void __cdecl SND_StopVoice(int voiceIndex);
int __cdecl SND_Active();
void __cdecl SND_Init();
unsigned int SND_InitGroups();
void __cdecl SND_Shutdown();
void __cdecl SND_ShutdownVoices();
void __cdecl SND_InitSnapshot();
void __cdecl SND_UpdateSnapshot(float dt);
double __cdecl SND_GetPitch(snd_voice_t *voice);
bool __cdecl SND_IsAliasTimescale(const snd_alias_t *alias);
bool __cdecl SND_IsOnSameTeam(unsigned int listenerIndex, SndEntHandle entNum);
double __cdecl SND_GetBaseLevel(const snd_voice_t *voice);
double __cdecl SND_GetDryLevel(const snd_voice_t *voice);
double __cdecl SND_GetWetLevel(const snd_voice_t *voice);
double __cdecl SND_GetDistance(const snd_voice_t *channel);
double __cdecl SND_GetLpfLevel(const snd_voice_t *voice);
double __cdecl I_fmap(float minx, float maxx, float miny, float maxy, float x);
unsigned int __cdecl SND_GetVoiceLength(unsigned int voiceIndex);
void __cdecl SND_FixupStereoPan(snd_speaker_map *pan);
void __cdecl SND_UpdatePanFilter(float dt, snd_voice_t *voice);
void __cdecl SND_UpdateVoice(snd_voice_t *voice, float dt);
double __cdecl Snd_DistanceCurveEval(
        const snd_curve *minCurve,
        const snd_curve *curve,
        float minDist,
        float maxDist,
        float dist);
double __cdecl SND_GetOmni(const snd_voice_t *voice);
snd_voice_t *__cdecl SND_GetPlaybackVoice(int playbackId);
bool __cdecl SND_IsStream(unsigned int index);
