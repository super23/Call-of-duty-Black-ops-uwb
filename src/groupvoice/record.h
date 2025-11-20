#pragma once

void __cdecl Record_SetRecordingCallback(int (__cdecl *new_audioCallback)(audioSample_t *));
int __cdecl Record_QueueAudioDataForEncoding(audioSample_t *sample);
int __cdecl Record_AudioCallback(audioSample_t *sample);
HRESULT __cdecl Record_Start(dsound_sample_t *sample);
HRESULT __cdecl Record_Stop(dsound_sample_t *sample);
dsound_sample_t *__cdecl Record_NewSample();
int __cdecl Record_DestroySample(dsound_sample_t *sample);
void __cdecl Record_Shutdown();
void __cdecl Record_Frame();
int __cdecl Record_Init();
