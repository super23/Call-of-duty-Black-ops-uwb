#include "play.h"

unsigned int __cdecl Sound_UpdateSample(dsound_sample_t *sample, char *data, unsigned int data_len)
{
  return DSound_UpdateSample(sample, data, data_len);
}

dsound_sample_t *__cdecl Sound_NewSample()
{
  return DSound_NewSample();
}

int __cdecl Sound_DestroySample(dsound_sample_t *sample)
{
  return Record_DestroySample(sample);
}

char __cdecl Sound_StopSample(dsound_sample_t *sample)
{
  return DSound_StopSample(sample);
}

void __cdecl Sound_SampleFrame(dsound_sample_t *sample)
{
  DSound_SampleFrame(sample);
}

int __cdecl Sound_Init(HWND__ *handle)
{
  return DSound_Init(1, handle);
}

void __cdecl Sound_Shutdown()
{
  DSound_Shutdown();
}

