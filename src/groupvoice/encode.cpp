#include "encode.h"

void __cdecl Encode_SetOptions(int frequency, int quality)
{
  speex_encoder_ctl((int)g_encoder);
  speex_encoder_ctl((int)g_encoder);
  speex_encoder_ctl((int)g_encoder);
  speex_encoder_ctl((int)g_encoder);
  speex_encoder_ctl((int)g_encoder);
  g_encoder_quality = quality;
  g_encoder_samplerate = frequency;
}

bool __cdecl Encode_Init(int bandwidthEnum)
{
  void *v1; // eax

  if ( bandwidthEnum )
  {
    if ( bandwidthEnum == 1 )
    {
      v1 = (void *)speex_encoder_init((int)&speex_wb_mode);
    }
    else
    {
      if ( bandwidthEnum != 2 )
      {
        Com_Printf(9, "Unknown bandwidth mode %i\n", bandwidthEnum);
        return 0;
      }
      v1 = (void *)speex_encoder_init((int)&speex_uwb_mode);
    }
  }
  else
  {
    v1 = (void *)speex_encoder_init((int)&speex_nb_mode);
  }
  g_encoder = v1;
  speex_bits_init((unsigned int **)&encodeBits);
  g_speex_initialized = 1;
  Encode_SetOptions(g_encoder_samplerate, g_encoder_quality);
  speex_encoder_ctl((int)g_encoder);
  g_current_bandwidth_setting = bandwidthEnum;
  return g_encoder != 0;
}

char __cdecl Encode_Shutdown()
{
  if ( g_encoder )
    speex_encoder_destroy((int)g_encoder);
  g_encoder = 0;
  if ( g_speex_initialized )
  {
    speex_bits_destroy((int)&encodeBits);
    g_speex_initialized = 0;
  }
  return 1;
}

int __cdecl Encode_Sample(__int16 *buffer_in, char *buffer_out, int maxLength)
{
  if ( sv_voiceQuality->current.integer != g_encoder_quality )
  {
    g_encoder_quality = sv_voiceQuality->current.integer;
    speex_encoder_ctl((int)g_encoder);
  }
  speex_bits_reset((int)&encodeBits);
  speex_encode_int((int)g_encoder, (int)buffer_in, (int)&encodeBits);
  return speex_bits_write(&encodeBits, (int)buffer_out, maxLength);
}

int __cdecl Encode_GetFrameSize()
{
  return g_frame_size;
}

