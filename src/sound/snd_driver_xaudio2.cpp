#include "snd_driver_xaudio2.h"

bool __cdecl SD_Xaudio2CanInit()
{
  return g_xaudio27 != 0;
}

IXAudio2SourceVoice **__cdecl SDXA2_VoiceDspAllocate()
{
  SDXA2SourceEffect *dsp; // [esp+4h] [ebp-8h]
  unsigned int i; // [esp+8h] [ebp-4h]

  for ( i = 0; i < 0x94; ++i )
  {
    dsp = (SDXA2SourceEffect *)&g_sd.voices[5856 * i - 866688];
    if ( dsp->AddRef(dsp) == 2 )
    {
      if ( dsp->Release(dsp) != 1
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
              173,
              0,
              "%s",
              "count == 1") )
      {
        __debugbreak();
      }
      return &g_sd.voices[5856 * i - 866688];
    }
    dsp->Release(dsp);
  }
  if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp", 182, 0, "%s", "0") )
    __debugbreak();
  return 0;
}

void __cdecl SD_TruncateAudioDeviceNames(Font_s *font, float scale, int size)
{
  int j; // [esp+18h] [ebp-Ch]
  char *name; // [esp+1Ch] [ebp-8h]
  int i; // [esp+20h] [ebp-4h]

  for ( i = 0; i < 16; ++i )
  {
    name = sd_xa2_device_names[i];
    for ( j = strlen(name) - 1; j >= 0 && UI_TextWidth(name, 0, font, scale) > size; --j )
      name[j] = 0;
  }
}

void __cdecl SD_XAudio2Done()
{
  if ( g_sd.xa2 )
  {
    g_sd.xa2->Release(g_sd.xa2);
    g_sd.xa2 = 0;
    g_sd.deviceGUIDValid = 0;
    g_sd.deviceIndex = -1;
    g_sd.deviceGUID.Data1 = 0;
    *(unsigned int *)&g_sd.deviceGUID.Data2 = 0;
    *(unsigned int *)g_sd.deviceGUID.Data4 = 0;
    *(unsigned int *)&g_sd.deviceGUID.Data4[4] = 0;
    CoUninitialize();
  }
}

unsigned int __cdecl SDXA2_ChannelFixup(unsigned int outputChannels, unsigned int channel)
{
  if ( outputChannels != 8 )
    return channel;
  switch ( channel )
  {
    case 4u:
      return 6;
    case 5u:
      return 7;
    case 6u:
      return 4;
    case 7u:
      return 5;
    default:
      return channel;
  }
}

IXAudio2SubmixVoice *__cdecl SD_CreateBus(
        unsigned int inputChannelCount,
        unsigned int outputChannelCount,
        unsigned int sampleRate,
        unsigned int stage,
        SDXA2Effect *dsp,
        IXAudio2Voice *outputVoice)
{
  float v7; // [esp+0h] [ebp-140h]
  HRESULT v8; // [esp+4h] [ebp-13Ch]
  unsigned int j; // [esp+8h] [ebp-138h]
  unsigned int i; // [esp+Ch] [ebp-134h]
  HRESULT hr; // [esp+10h] [ebp-130h]
  IXAudio2SubmixVoice *voice; // [esp+14h] [ebp-12Ch] BYREF
  float matrix[65]; // [esp+18h] [ebp-128h] BYREF
  XAUDIO2_VOICE_SENDS sends; // [esp+11Ch] [ebp-24h] BYREF
  XAUDIO2_EFFECT_CHAIN effects; // [esp+124h] [ebp-1Ch] BYREF
  XAUDIO2_EFFECT_DESCRIPTOR effectDescriptors[1]; // [esp+12Ch] [ebp-14h] BYREF
  XAUDIO2_SEND_DESCRIPTOR outputSend; // [esp+138h] [ebp-8h] BYREF

  outputSend.Flags = 0;
  outputSend.pOutputVoice = outputVoice;
  sends.SendCount = 1;
  sends.pSends = &outputSend;
  effectDescriptors[0].pEffect = dsp;
  effectDescriptors[0].InitialState = 1;
  effectDescriptors[0].OutputChannels = inputChannelCount;
  effects.EffectCount = 1;
  effects.pEffectDescriptors = effectDescriptors;
  hr = g_sd.xa2->CreateSubmixVoice(
         g_sd.xa2,
         &voice,
         inputChannelCount,
         sampleRate,
         0,
         stage,
         &sends,
         dsp != 0 ? &effects : 0);
  if ( hr
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
          650,
          0,
          "%s\n\t(hr) = %i",
          "(hr == ((HRESULT)0L))",
          hr) )
  {
    __debugbreak();
  }
  if ( inputChannelCount > 8
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
          652,
          0,
          "%s",
          "inputChannelCount <= 8") )
  {
    __debugbreak();
  }
  if ( outputChannelCount > 8
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
          653,
          0,
          "%s",
          "outputChannelCount <= 8") )
  {
    __debugbreak();
  }
  for ( i = 0; i < inputChannelCount; ++i )
  {
    for ( j = 0; j < outputChannelCount; ++j )
    {
      if ( i == j )
        v7 = FLOAT_1_0;
      else
        v7 = *(float *)&FLOAT_0_0;
      matrix[i + j * inputChannelCount] = v7;
    }
  }
  v8 = voice->SetOutputMatrix(voice, outputVoice, inputChannelCount, outputChannelCount, matrix, 0);
  if ( v8
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
          668,
          0,
          "%s\n\t(hr) = %i",
          "(hr == ((HRESULT)0L))",
          v8) )
  {
    __debugbreak();
  }
  return voice;
}

void __cdecl SD_StopVoice(int voiceIndex)
{
  unsigned int i; // [esp+0h] [ebp-8h]
  int streamVoice; // [esp+4h] [ebp-4h]

  if ( g_sd.voices[voiceIndex] )
  {
    g_sd.voices[voiceIndex]->Stop(g_sd.voices[voiceIndex], 0, 0);
    g_sd.voices[voiceIndex]->DestroyVoice(g_sd.voices[voiceIndex]);
    g_sd.voices[voiceIndex] = 0;
  }
  if ( SND_IsStream(voiceIndex) )
  {
    streamVoice = iSND_GetStreamChannel(voiceIndex);
    for ( i = 0; i < 2; ++i )
      iSND_ReleaseStreamBuffer(streamVoice, i);
    if ( Snd_StreamStatus(streamVoice) )
      Snd_StreamClose(streamVoice);
  }
}

unsigned int __cdecl iSND_GetStreamChannel(unsigned int index)
{
  if ( index >= 0xA
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
          473,
          0,
          "%s\n\t(index) = %i",
          "(index >= (0) && index < (0) + 10)",
          index) )
  {
    __debugbreak();
  }
  return index;
}

void __cdecl iSND_ReleaseStreamBuffer(unsigned int streamVoice, unsigned int bufferIndex)
{
  char *releaseWindow; // [esp+4h] [ebp-8h]

  if ( streamVoice >= 0xA
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
          605,
          0,
          "%s",
          "streamVoice < SND_MAX_STREAM_VOICES") )
  {
    __debugbreak();
  }
  if ( bufferIndex >= 2
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
          606,
          0,
          "%s",
          "bufferIndex < StreamVoice::BUFFER_COUNT") )
  {
    __debugbreak();
  }
  releaseWindow = (char *)InterlockedExchange(
                            (volatile int *)&g_sd.streamVoices[streamVoice].buffers[bufferIndex].pAudioData,
                            0);
  if ( releaseWindow )
    Snd_StreamReleaseWindow(streamVoice, releaseWindow);
}

int __cdecl SND_StartAliasStream(SndStartAliasInfo *startAliasInfo, unsigned int voiceIndex)
{
  unsigned int primeSize; // [esp+8h] [ebp-118h]
  char *primeData; // [esp+Ch] [ebp-114h]
  char filename2[260]; // [esp+10h] [ebp-110h] BYREF
  int streamVoice; // [esp+118h] [ebp-8h]
  const snd_alias_t *alias; // [esp+11Ch] [ebp-4h]

  alias = startAliasInfo->alias;
  streamVoice = iSND_GetStreamChannel(voiceIndex);
  SND_AliasGetFileName(alias, filename2, 256);
  if ( alias->soundFile->exists )
  {
    primeSize = 0;
    primeData = 0;
    if ( (alias->flags & 0xC000) >> 14 == 3 && alias->soundFile->u.loadSnd->sound.version )
    {
      primeData = *(char **)(alias->soundFile->u.loadSnd->sound.version + 4);
      primeSize = *(unsigned int *)(alias->soundFile->u.loadSnd->sound.version + 8);
    }
    Snd_StreamOpen(streamVoice, filename2, (alias->flags & 1) != 0, primeSize, primeData);
    SND_SetVoiceStartInfo(voiceIndex, startAliasInfo);
    SND_SetSoundFileVoiceInfo(voiceIndex, 0, 0, 0, 0, SFLS_LOADING);
    SD_UpdateVoice(voiceIndex);
    if ( g_snd.voiceAliasHash[118 * voiceIndex - 8708] )
      return startAliasInfo->playbackId;
    else
      return -1;
  }
  else
  {
    Com_PrintError(
      1,
      "Tried to play streamed sound '%s' from alias '%s', but it was not found at load time.\n",
      filename2,
      alias->name);
    return SND_SetPlaybackIdNotPlayed(voiceIndex);
  }
}

void __cdecl SD_UpdateStreamVoice(int voiceIndex)
{
  XAUDIO2_BUFFER *v1; // eax
  unsigned int bufferIndex; // [esp+Ch] [ebp-48h]
  StreamVoice *sv; // [esp+14h] [ebp-40h]
  char *data; // [esp+18h] [ebp-3Ch] BYREF
  XAUDIO2_VOICE_STATE voiceState; // [esp+1Ch] [ebp-38h] BYREF
  unsigned int size; // [esp+2Ch] [ebp-28h] BYREF
  unsigned int position; // [esp+30h] [ebp-24h] BYREF
  unsigned int lengthMS; // [esp+34h] [ebp-20h]
  const snd_asset *header; // [esp+38h] [ebp-1Ch]
  snd_voice_t *voice; // [esp+3Ch] [ebp-18h]
  snd_stream_status status; // [esp+40h] [ebp-14h]
  int streamVoice; // [esp+44h] [ebp-10h]
  XAUDIO2_BUFFER_WMA bufferWMA; // [esp+48h] [ebp-Ch] BYREF
  int isLooping; // [esp+50h] [ebp-4h]

  streamVoice = iSND_GetStreamChannel(voiceIndex);
  status = Snd_StreamStatus(streamVoice);
  voice = (snd_voice_t *)&g_snd.voiceAliasHash[118 * voiceIndex - 8732];
  isLooping = (voice->alias->flags & 1) != 0;
  header = 0;
  bufferWMA.pDecodedPacketCumulativeBytes = 0;
  bufferWMA.PacketCount = 0;
  if ( status && status != SND_STREAM_STARVING && Snd_StreamGetHeader(streamVoice) )
  {
    if ( status == SND_STREAM_ERROR )
    {
LABEL_5:
      iSND_ChannelError(voiceIndex);
      return;
    }
    if ( g_sd.voices[voiceIndex] )
      goto LABEL_11;
    if ( status == SND_STREAM_OK )
    {
      header = Snd_StreamGetHeader(streamVoice);
      if ( !iSND_CreateVoice(
              voiceIndex,
              header,
              isLooping,
              (*(unsigned int *)(g_snd.voiceAliasHash[118 * voiceIndex - 8708] + 20) & 0x3000u) >> 12) )
        goto LABEL_5;
      lengthMS = 1000LL * header->frame_count / header->frame_rate;
      SND_SetSoundFileVoiceInfo(voiceIndex, header->channel_count, header->frame_rate, lengthMS, 0, SFLS_LOADED);
      SND_UpdateVoice((snd_voice_t *)&g_snd.voiceAliasHash[118 * voiceIndex - 8732], 0.0);
      if ( header->format == SND_ASSET_FORMAT_WMA )
      {
        bufferWMA.PacketCount = header->seek_table_count;
        bufferWMA.pDecodedPacketCumulativeBytes = header->seek_table;
      }
LABEL_11:
      while ( Snd_StreamGetFreeWindows(streamVoice) )
      {
        g_sd.voices[voiceIndex]->GetState(g_sd.voices[voiceIndex], &voiceState);
        if ( voiceState.BuffersQueued >= 2 )
          break;
        status = Snd_StreamAcquireWindow(streamVoice, &size, &position, &data);
        switch ( status )
        {
          case SND_STREAM_OK:
            if ( !header )
            {
              header = Snd_StreamGetHeader(streamVoice);
              if ( header->format == SND_ASSET_FORMAT_WMA )
              {
                bufferWMA.PacketCount = header->seek_table_count;
                bufferWMA.pDecodedPacketCumulativeBytes = header->seek_table;
              }
            }
            sv = &g_sd.streamVoices[streamVoice];
            bufferIndex = sv->bufferQueuedCount % 2;
            ++sv->bufferQueuedCount;
            v1 = &sv->buffers[bufferIndex];
            v1->Flags = 0;
            v1->AudioBytes = 0;
            v1->pAudioData = 0;
            v1->PlayBegin = 0;
            v1->PlayLength = 0;
            v1->LoopBegin = 0;
            v1->LoopLength = 0;
            v1->LoopCount = 0;
            v1->pContext = 0;
            sv->buffers[bufferIndex].AudioBytes = size;
            if ( sv->buffers[bufferIndex].pAudioData )
            {
              if ( !Assert_MyHandler(
                      "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
                      1225,
                      0,
                      "%s",
                      "!buffer->pAudioData") )
                __debugbreak();
            }
            sv->buffers[bufferIndex].pAudioData = (const unsigned __int8 *)data;
            sv->buffers[bufferIndex].pContext = (void *)(bufferIndex | (streamVoice << 8));
            g_sd.voices[voiceIndex]->SubmitSourceBuffer(
              g_sd.voices[voiceIndex],
              &sv->buffers[bufferIndex],
              header->format != SND_ASSET_FORMAT_WMA ? 0 : &bufferWMA);
            break;
          case SND_STREAM_STARVING:
            return;
          case SND_STREAM_EOF:
            if ( !voiceState.BuffersQueued )
              SND_StopVoice(voiceIndex);
            return;
          case SND_STREAM_ERROR:
            iSND_ChannelError(voiceIndex);
            return;
        }
      }
    }
  }
}

int __cdecl iSND_CreateVoice(int voiceIndex, const snd_asset *snd, int isLooping, int bus)
{
  bool IsStream; // al
  void *p_WFXE; // [esp+14h] [ebp-F4h]
  StreamVoice *v7; // [esp+18h] [ebp-F0h]
  IXAudio2SubmixVoice *masterBus; // [esp+1Ch] [ebp-ECh]
  snd_asset_format format; // [esp+20h] [ebp-E8h]
  tWAVEFORMATEX *p_AWFT; // [esp+24h] [ebp-E4h]
  unsigned int StreamChannel; // [esp+28h] [ebp-E0h]
  HRESULT v12; // [esp+34h] [ebp-D4h]
  HRESULT hr; // [esp+38h] [ebp-D0h]
  int isStreaming; // [esp+3Ch] [ebp-CCh]
  $adpcmwaveformat_tag$_extraBytes_28 AWFT; // [esp+40h] [ebp-C8h] BYREF
  XAUDIO2_BUFFER buffer; // [esp+74h] [ebp-94h] BYREF
  XAUDIO2_EFFECT_DESCRIPTOR effect; // [esp+98h] [ebp-70h] BYREF
  int streamVoice; // [esp+A4h] [ebp-64h]
  WAVEFORMATEXTENSIBLE WFXE; // [esp+A8h] [ebp-60h] BYREF
  VoiceInfo *voiceInfo; // [esp+D4h] [ebp-34h]
  XAUDIO2_BUFFER_WMA bufferWMA; // [esp+D8h] [ebp-30h] BYREF
  XAUDIO2_SEND_DESCRIPTOR outputVoices[2]; // [esp+E0h] [ebp-28h] BYREF
  tWAVEFORMATEX *wfx; // [esp+F0h] [ebp-18h]
  bool isStereo; // [esp+F6h] [ebp-12h]
  bool isADPCM; // [esp+F7h] [ebp-11h]
  XAUDIO2_VOICE_SENDS sendList; // [esp+F8h] [ebp-10h] BYREF
  XAUDIO2_EFFECT_CHAIN fxChain; // [esp+100h] [ebp-8h] BYREF

  IsStream = SND_IsStream(voiceIndex);
  isStreaming = IsStream;
  if ( IsStream )
    StreamChannel = iSND_GetStreamChannel(voiceIndex);
  else
    StreamChannel = -1;
  streamVoice = StreamChannel;
  isStereo = snd->channel_count != 1;
  isADPCM = snd->format == SND_ASSET_FORMAT_MSADPCM;
  voiceInfo = &g_sd.voiceInfos[voiceIndex];
  memset(&WFXE, 0, sizeof(WFXE));
  memset(&AWFT, 0, 22);
  AWFT.aCoef[0].iCoef1 = 256;
  AWFT.aCoef[0].iCoef2 = 0;
  AWFT.aCoef[1].iCoef1 = 512;
  AWFT.aCoef[1].iCoef2 = -256;
  AWFT.aCoef[2].iCoef1 = 0;
  AWFT.aCoef[2].iCoef2 = 0;
  AWFT.aCoef[3].iCoef1 = 192;
  AWFT.aCoef[3].iCoef2 = 64;
  AWFT.aCoef[4].iCoef1 = 240;
  AWFT.aCoef[4].iCoef2 = 0;
  AWFT.aCoef[5].iCoef1 = 460;
  AWFT.aCoef[5].iCoef2 = -208;
  AWFT.aCoef[6].iCoef1 = 392;
  AWFT.aCoef[6].iCoef2 = -232;
  if ( isADPCM )
    p_AWFT = (tWAVEFORMATEX *)&AWFT;
  else
    p_AWFT = (tWAVEFORMATEX *)&WFXE;
  wfx = p_AWFT;
  p_AWFT->nChannels = snd->channel_count;
  format = snd->format;
  if ( format )
  {
    if ( format == SND_ASSET_FORMAT_MSADPCM )
    {
      wfx->wFormatTag = 2;
      wfx->nChannels = snd->channel_count;
      wfx->nBlockAlign = 262 * snd->channel_count;
      AWFT.wSamplesPerBlock = 2 * wfx->nBlockAlign / wfx->nChannels - 12;
      AWFT.wNumCoef = 7;
      wfx->nSamplesPerSec = snd->frame_rate;
      wfx->wBitsPerSample = 4;
      wfx->cbSize = 32;
      wfx->nAvgBytesPerSec = (__int64)(wfx->nSamplesPerSec * (unsigned __int64)wfx->nBlockAlign) / AWFT.wSamplesPerBlock;
    }
    else
    {
      if ( format != SND_ASSET_FORMAT_WMA )
      {
        Com_PrintError(9, "invalid asset for alias %s\n", *(const char **)g_snd.voiceAliasHash[118 * voiceIndex - 8708]);
        return 0;
      }
      if ( isStreaming
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
              953,
              0,
              "%s",
              "!isStreaming") )
      {
        __debugbreak();
      }
      if ( !snd->seek_table
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
              954,
              0,
              "%s",
              "snd->seek_table") )
      {
        __debugbreak();
      }
      wfx->wFormatTag = 353;
      wfx->nBlockAlign = isStereo ? 4096 : 2230;
      wfx->nSamplesPerSec = snd->frame_rate;
      wfx->nAvgBytesPerSec = 6000 * snd->channel_count;
      wfx->wBitsPerSample = 16;
      wfx->cbSize = 0;
    }
  }
  else
  {
    wfx->wFormatTag = 1;
    wfx->nBlockAlign = (unsigned __int16)(LOWORD(snd->channel_count) * wfx->wBitsPerSample) / 8;
    wfx->nSamplesPerSec = snd->frame_rate;
    wfx->nAvgBytesPerSec = wfx->nSamplesPerSec * wfx->nBlockAlign;
    wfx->wBitsPerSample = 16;
    wfx->cbSize = 21072;
  }
  memset(&buffer.AudioBytes, 0, 32);
  bufferWMA.pDecodedPacketCumulativeBytes = 0;
  bufferWMA.PacketCount = 0;
  buffer.Flags = 64;
  if ( snd->format == SND_ASSET_FORMAT_WMA )
  {
    bufferWMA.PacketCount = snd->seek_table_count;
    bufferWMA.pDecodedPacketCumulativeBytes = snd->seek_table;
    buffer.AudioBytes = bufferWMA.PacketCount * wfx->nBlockAlign;
    if ( buffer.AudioBytes > snd->data_size
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
            995,
            0,
            "%s",
            "buffer.AudioBytes <= snd->data_size") )
    {
      __debugbreak();
    }
  }
  else
  {
    buffer.AudioBytes = snd->data_size;
  }
  if ( buffer.pAudioData
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
          1002,
          0,
          "%s",
          "!buffer.pAudioData") )
  {
    __debugbreak();
  }
  buffer.pAudioData = (const unsigned __int8 *)snd->data;
  buffer.LoopCount = (unsigned __int8)-(isLooping != 0);
  outputVoices[0].Flags = 0;
  if ( bus == 2 )
    masterBus = g_sd.masterBus;
  else
    masterBus = g_sd.novoiceBus;
  outputVoices[0].pOutputVoice = masterBus;
  outputVoices[1].Flags = 0;
  outputVoices[1].pOutputVoice = g_sd.radverbBus;
  sendList.SendCount = 2;
  sendList.pSends = outputVoices;
  voiceInfo->sourceDsp = (SDXA2SourceEffect *)SDXA2_VoiceDspAllocate();
  effect.pEffect = voiceInfo->sourceDsp;
  effect.InitialState = 1;
  effect.OutputChannels = snd->channel_count;
  fxChain.EffectCount = 1;
  fxChain.pEffectDescriptors = &effect;
  SDXA2SourceEffect::Clear(voiceInfo->sourceDsp);
  if ( isStreaming )
    v7 = &g_sd.streamVoices[streamVoice];
  else
    v7 = 0;
  if ( isADPCM )
    p_WFXE = &AWFT;
  else
    p_WFXE = &WFXE;
  hr = ((int (__stdcall *)(IXAudio2 *, int, void *, unsigned int, unsigned int, StreamVoice *, XAUDIO2_VOICE_SENDS *, XAUDIO2_EFFECT_CHAIN *))g_sd.xa2->CreateSourceVoice)(
         g_sd.xa2,
         4 * voiceIndex + 172630016,
         p_WFXE,
         0,
         2.0,
         v7,
         &sendList,
         &fxChain);
  if ( hr
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
          1033,
          0,
          "%s\n\t(hr) = %i",
          "(hr == ((HRESULT)0L))",
          hr) )
  {
    __debugbreak();
  }
  if ( g_sd.voices[voiceIndex] )
  {
    if ( isStreaming )
    {
      g_sd.streamVoices[streamVoice].bufferQueuedCount = 0;
    }
    else
    {
      v12 = g_sd.voices[voiceIndex]->SubmitSourceBuffer(
              g_sd.voices[voiceIndex],
              &buffer,
              snd->seek_table != 0 ? &bufferWMA : 0);
      if ( v12
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
              1047,
              0,
              "%s\n\t(hr) = %i",
              "(hr == ((HRESULT)0L))",
              v12) )
      {
        __debugbreak();
      }
    }
    return 1;
  }
  else
  {
    if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp", 1037, 0, "%s", "0") )
      __debugbreak();
    return 0;
  }
}

void __cdecl iSND_ChannelError(int voiceIndex)
{
  const snd_alias_t *alias; // [esp+4h] [ebp-4h]

  alias = (const snd_alias_t *)g_snd.voiceAliasHash[118 * voiceIndex - 8708];
  SND_LengthNotify(voiceIndex, 0);
  SND_StopVoice(voiceIndex);
  alias->soundFile->exists = 0;
}

char __cdecl SD_Init()
{
  if ( g_xaudio27 )
  {
    g_sd.deviceIndex = -1;
    g_sd.deviceGUIDValid = 0;
    g_sd.deviceGUID.Data1 = 0;
    *(unsigned int *)&g_sd.deviceGUID.Data2 = 0;
    *(unsigned int *)g_sd.deviceGUID.Data4 = 0;
    *(unsigned int *)&g_sd.deviceGUID.Data4[4] = 0;
    g_sd.xa2 = 0;
    memset((unsigned __int8 *)&g_sd.details, 0, sizeof(g_sd.details));
    g_sd.masterVoice = 0;
    g_sd.masterVoiceDetails.CreationFlags = 0;
    g_sd.masterVoiceDetails.InputChannels = 0;
    g_sd.masterVoiceDetails.InputSampleRate = 0;
    g_sd.masterBus = 0;
    memset((unsigned __int8 *)&g_sd.masterParams, 0, sizeof(g_sd.masterParams));
    g_sd.novoiceBus = 0;
    g_sd.novoiceParams.compE = 0.0;
    g_sd.novoiceParams.compPG = 0.0;
    g_sd.novoiceParams.compMG = 0.0;
    g_sd.novoiceParams.compT = 0.0;
    g_sd.novoiceParams.compR = 0.0;
    g_sd.novoiceParams.compTA = 0.0;
    g_sd.novoiceParams.compTR = 0.0;
    g_sd.radverbBus = 0;
    memset((unsigned __int8 *)&g_sd.radverbParams, 0, sizeof(g_sd.radverbParams));
    memset((unsigned __int8 *)g_sd.voices, 0, sizeof(g_sd.voices));
    memset((unsigned __int8 *)&g_sd.perfData, 0, sizeof(g_sd.perfData));
    SDXA2_VoiceDspAssertUnused();
    if ( SD_XAudio2Init() )
    {
      SND_InitMasterVoice();
      return 1;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    Com_Error(ERR_DROP, "Cannot run with sound built on wrong version of XDK");
    return 0;
  }
}

void SDXA2_VoiceDspAssertUnused()
{
  unsigned int i; // [esp+8h] [ebp-4h]

  for ( i = 0; i < 0x94; ++i )
  {
    if ( ((int (__stdcall *)(IXAudio2SourceVoice **))g_sd.voices[5856 * i - 866688][1].__vftable)(&g_sd.voices[5856 * i - 866688]) != 2
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
            156,
            0,
            "%s",
            "count == 2") )
    {
      __debugbreak();
    }
    if ( ((int (__thiscall *)(IXAudio2SourceVoice **, IXAudio2SourceVoice **))g_sd.voices[5856 * i - 866688][2].__vftable)(
           &g_sd.voices[5856 * i - 866688],
           &g_sd.voices[5856 * i - 866688]) != 1
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
            158,
            0,
            "%s",
            "count == 1") )
    {
      __debugbreak();
    }
  }
}

bool __cdecl SD_XAudio2Init()
{
  HRESULT hr; // [esp+28h] [ebp-Ch]
  unsigned int debug; // [esp+2Ch] [ebp-8h]

  if ( g_sd.xa2 )
    return 0;
  debug = 0;
  sd_xa2_num_devices = _Dvar_RegisterInt("sd_xa2_num_devices", 0, 0, 16, 0, "Number of xaudio2 devices");
  SD_XAudio2EnumerateDevices();
  if ( sd_xa2_num_devices->current.integer )
  {
    hr = XAudio2Create(&g_sd.xa2, debug, XAUDIO2_ANY_PROCESSOR);
    if ( hr )
    {
      if ( !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
              427,
              0,
              "%s\n\t(hr) = %i",
              "(hr == ((HRESULT)0L))",
              hr) )
        __debugbreak();
    }
    SD_SwitchDevice();
  }
  sd_xa2_can_switch_device = _Dvar_RegisterBool(
                               "sd_can_switch_device",
                               sd_xa2_num_devices->current.integer != 0,
                               0x40u,
                               "Allow Switching sound device");
  return g_sd.xa2 != 0;
}

HRESULT __cdecl XAudio2Create(
        IXAudio2 **ppXAudio2,
        unsigned int Flags,
        XAUDIO2_WINDOWS_PROCESSOR_SPECIFIER XAudio2Processor)
{
  GUID *v4; // [esp+0h] [ebp-10h]
  int hr; // [esp+8h] [ebp-8h]
  IXAudio2 *pXAudio2; // [esp+Ch] [ebp-4h] BYREF

  if ( (Flags & 1) != 0 )
    v4 = &_GUID_db05ea35_0329_4d4b_a53a_6dead03d3852;
  else
    v4 = &_GUID_5a508685_a254_4fba_9b82_9a24b00306af;
  hr = CoCreateInstance(v4, 0, 1u, &_GUID_8bcf1f58_9fe7_4583_8ac6_e2adc465c8bb, (LPVOID *)&pXAudio2);
  if ( hr >= 0 )
  {
    hr = pXAudio2->Initialize(pXAudio2, Flags, XAudio2Processor);
    if ( hr < 0 )
      pXAudio2->Release(pXAudio2);
    else
      *ppXAudio2 = pXAudio2;
  }
  return hr;
}

char __cdecl SD_XAudio2EnumerateDevices()
{
  int v1; // [esp+0h] [ebp-45Ch]
  int n; // [esp+10h] [ebp-44Ch]
  unsigned int deviceIndex; // [esp+14h] [ebp-448h]
  int defaultDevice; // [esp+18h] [ebp-444h]
  XAUDIO2_DEVICE_DETAILS deviceDetails; // [esp+1Ch] [ebp-440h] BYREF
  unsigned int deviceCount; // [esp+450h] [ebp-Ch] BYREF
  HRESULT hr; // [esp+454h] [ebp-8h]
  IXAudio2 *xa2; // [esp+458h] [ebp-4h] BYREF

  xa2 = 0;
  if ( sd_xa2_num_devices->current.integer )
    return 1;
  CoInitializeEx(0, 0);
  hr = XAudio2Create(&xa2, 0, XAUDIO2_ANY_PROCESSOR);
  if ( hr < 0 )
    return 0;
  defaultDevice = 0;
  hr = xa2->GetDeviceCount(xa2, &deviceCount);
  if ( hr >= 0 )
  {
    if ( deviceCount >= sd_max_num_devices )
      v1 = sd_max_num_devices;
    else
      v1 = deviceCount;
    deviceCount = v1;
    for ( deviceIndex = 0; deviceIndex < deviceCount; ++deviceIndex )
    {
      xa2->GetDeviceDetails(xa2, deviceIndex, &deviceDetails);
      if ( SD_XAudio2CheckDevice(xa2, &deviceDetails, deviceIndex) )
      {
        n = sd_xa2_num_devices->current.integer;
        Dvar_SetInt((dvar_s *)sd_xa2_num_devices, n + 1);
        WideCharToMultiByte(0, 0, deviceDetails.DisplayName, -1, sd_xa2_device_names[n], 256, 0, 0);
        SD_XAudio2GetDeviceGUID(deviceDetails.DeviceID, &sd_deviceGUIDs[n], sd_xa2_device_guids[n], 256);
        sd_xa2_devices_value_list[n] = sd_xa2_device_names[n];
        sd_xa2_device_guids_value_list[n] = sd_xa2_device_guids[n];
        sd_xa2_device_indices[n] = deviceIndex;
        if ( deviceDetails.Role == GlobalDefaultDevice )
          defaultDevice = n;
      }
    }
    sd_xa2_devices_value_list[sd_xa2_num_devices->current.integer] = 0;
    sd_xa2_device_guids_value_list[sd_xa2_num_devices->current.integer] = 0;
    sd_xa2_device_guid = _Dvar_RegisterEnum(
                           "sd_xa2_device_guid",
                           sd_xa2_device_guids_value_list,
                           defaultDevice,
                           1u,
                           "prefered audio device");
    sd_xa2_device_name = _Dvar_RegisterEnum(
                           "sd_xa2_device_name",
                           sd_xa2_devices_value_list,
                           sd_xa2_device_guid->current.integer,
                           0,
                           "Available Sound devices");
  }
  ((void (__thiscall *)(IXAudio2 *, IXAudio2 *))xa2->Release)(xa2, xa2);
  return 1;
}

char __cdecl SD_XAudio2GetDeviceGUID(wchar_t *inGUID, _GUID *guid, char *outGUID, int size)
{
  while ( *inGUID )
  {
    if ( !CLSIDFromString(inGUID, guid) )
    {
      WideCharToMultiByte(0, 0, inGUID, -1, outGUID, size, 0, 0);
      return 1;
    }
    ++inGUID;
  }
  return 0;
}

char __cdecl SD_XAudio2CheckDevice(
        IXAudio2 *pXAudio2,
        const XAUDIO2_DEVICE_DETAILS *deviceDetails,
        unsigned int deviceIndex)
{
  IXAudio2MasteringVoice *testVoice; // [esp+4h] [ebp-Ch] BYREF
  HRESULT hr; // [esp+8h] [ebp-8h]
  unsigned int nChannels; // [esp+Ch] [ebp-4h]

  if ( deviceDetails->OutputFormat.Format.nChannels < 2u )
    return 0;
  nChannels = deviceDetails->OutputFormat.Format.nChannels <= 8u ? 0 : 8;
  testVoice = 0;
  hr = pXAudio2->CreateMasteringVoice(pXAudio2, &testVoice, nChannels, 48000u, 0, deviceIndex, 0);
  if ( hr )
  {
    hr = ((int (__thiscall *)(IXAudio2 *, IXAudio2 *, IXAudio2MasteringVoice **, unsigned int, int, unsigned int, unsigned int, unsigned int))pXAudio2->CreateMasteringVoice)(
           pXAudio2,
           pXAudio2,
           &testVoice,
           nChannels,
           deviceDetails->OutputFormat.Format.nSamplesPerSec > 0xBB80 ? 0xBB80 : 0,
           0,
           deviceIndex,
           0);
    if ( hr )
      return 0;
  }
  testVoice->DestroyVoice(testVoice);
  return 1;
}

void SD_SwitchDevice()
{
  int deviceIndex; // [esp+0h] [ebp-4h]

  if ( sd_xa2_num_devices->current.integer )
  {
    if ( !g_sd.xa2
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp", 305, 0, "%s", "g_sd.xa2") )
    {
      __debugbreak();
    }
    deviceIndex = sd_xa2_device_indices[sd_xa2_device_guid->current.integer];
    if ( g_sd.deviceIndex == deviceIndex
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
            307,
            0,
            "%s",
            "g_sd.deviceIndex != deviceIndex") )
    {
      __debugbreak();
    }
    g_sd.deviceIndex = deviceIndex;
    ((void (__thiscall *)(IXAudio2 *, IXAudio2 *, int, XAUDIO2_DEVICE_DETAILS *))g_sd.xa2->GetDeviceDetails)(
      g_sd.xa2,
      g_sd.xa2,
      deviceIndex,
      &g_sd.details);
    g_sd.deviceGUID = sd_deviceGUIDs[deviceIndex];
    g_sd.deviceGUIDValid = 1;
    R_CinematicInitSound(&g_sd.deviceGUID);
  }
}

void SND_InitMasterVoice()
{
  unsigned int index; // [esp+8h] [ebp-11Ch]
  HRESULT v1; // [esp+Ch] [ebp-118h]
  int i; // [esp+10h] [ebp-114h]
  HRESULT hr; // [esp+14h] [ebp-110h]
  float matrix[64]; // [esp+1Ch] [ebp-108h] BYREF
  int inputChannels; // [esp+11Ch] [ebp-8h]
  int inputSampleRate; // [esp+120h] [ebp-4h]

  inputChannels = g_sd.details.OutputFormat.Format.nChannels <= 8u ? 0 : 8;
  inputSampleRate = g_sd.details.OutputFormat.Format.nSamplesPerSec > 0xBB80 ? 0xBB80 : 0;
  hr = ((int (__thiscall *)(IXAudio2 *, IXAudio2 *, IXAudio2MasteringVoice **, int, int, unsigned int, int, unsigned int))g_sd.xa2->CreateMasteringVoice)(
         g_sd.xa2,
         g_sd.xa2,
         &g_sd.masterVoice,
         inputChannels,
         inputSampleRate,
         0,
         g_sd.deviceIndex < 0 ? 0 : g_sd.deviceIndex,
         0);
  if ( hr
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
          685,
          0,
          "%s\n\t(hr) = %i",
          "(hr == ((HRESULT)0L))",
          hr) )
  {
    __debugbreak();
  }
  if ( g_sd.masterVoice )
  {
    g_sd.masterVoice->GetVoiceDetails(g_sd.masterVoice, &g_sd.masterVoiceDetails);
    if ( g_sd.masterVoiceDetails.InputChannels > 8
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
            694,
            0,
            "%s",
            "g_sd.masterVoiceDetails.InputChannels <= 8") )
    {
      __debugbreak();
    }
    g_sd.masterBus = SD_CreateBus(
                       g_sd.masterVoiceDetails.InputChannels,
                       g_sd.masterVoiceDetails.InputChannels,
                       0xBB80u,
                       3u,
                       &g_sd.masterDsp,
                       g_sd.masterVoice);
    g_sd.novoiceBus = SD_CreateBus(
                        g_sd.masterVoiceDetails.InputChannels,
                        g_sd.masterVoiceDetails.InputChannels,
                        0xBB80u,
                        2u,
                        &g_sd.novoiceDsp,
                        g_sd.masterBus);
    g_sd.radverbBus = SD_CreateBus(
                        4u,
                        g_sd.masterVoiceDetails.InputChannels,
                        0xBB80u,
                        1u,
                        &g_sd.radverbDsp,
                        g_sd.novoiceBus);
    if ( g_sd.masterVoiceDetails.InputChannels > 8
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
            720,
            0,
            "%s",
            "g_sd.masterVoiceDetails.InputChannels <= 8") )
    {
      __debugbreak();
    }
    for ( i = 0; i < 64; ++i )
      matrix[i] = *(float *)&FLOAT_0_0;
    switch ( g_sd.masterVoiceDetails.InputChannels )
    {
      case 1u:
        matrix[0] = fsqrt(0.25);
        matrix[1] = matrix[0];
        matrix[2] = matrix[0];
        matrix[3] = matrix[0];
        break;
      case 2u:
      case 3u:
      case 4u:
      case 5u:
      case 7u:
        memset(matrix, 0, 12);
        matrix[3] = FLOAT_1_0;
        matrix[4] = *(float *)&FLOAT_0_0;
        matrix[5] = FLOAT_1_0;
        matrix[6] = *(float *)&FLOAT_0_0;
        matrix[7] = *(float *)&FLOAT_0_0;
        break;
      case 6u:
        memset(matrix, 0, 12);
        matrix[3] = FLOAT_1_0;
        matrix[4] = *(float *)&FLOAT_0_0;
        matrix[5] = FLOAT_1_0;
        matrix[6] = *(float *)&FLOAT_0_0;
        matrix[7] = *(float *)&FLOAT_0_0;
        matrix[16] = FLOAT_1_0;
        memset(&matrix[17], 0, 20);
        matrix[22] = FLOAT_1_0;
        matrix[23] = *(float *)&FLOAT_0_0;
        break;
      case 8u:
        memset(matrix, 0, 12);
        matrix[3] = FLOAT_1_0;
        matrix[4] = *(float *)&FLOAT_0_0;
        matrix[5] = FLOAT_1_0;
        matrix[6] = *(float *)&FLOAT_0_0;
        matrix[7] = *(float *)&FLOAT_0_0;
        matrix[16] = fsqrt(0.5);
        memset(&matrix[17], 0, 20);
        matrix[22] = matrix[16];
        matrix[23] = *(float *)&FLOAT_0_0;
        matrix[24] = matrix[16];
        memset(&matrix[25], 0, 20);
        matrix[30] = matrix[16];
        matrix[31] = *(float *)&FLOAT_0_0;
        break;
      default:
        break;
    }
    v1 = g_sd.radverbBus->SetOutputMatrix(
           g_sd.radverbBus,
           g_sd.novoiceBus,
           4u,
           g_sd.masterVoiceDetails.InputChannels,
           matrix,
           0);
    if ( v1 )
    {
      if ( !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
              815,
              0,
              "%s\n\t(hr) = %i",
              "(hr == ((HRESULT)0L))",
              v1) )
        __debugbreak();
    }
    Snd_SetConfigStringsBySpeakerCount(g_sd.masterVoiceDetails.InputChannels);
    for ( index = 0; index < SND_GetSpeakerConfigCount(); ++index )
    {
      if ( g_sd.masterVoiceDetails.InputChannels == Snd_GetSpeakerConfig(index)->speakerCount )
      {
        Dvar_SetInt((dvar_s *)snd_speakerConfiguration, index);
        return;
      }
    }
  }
  else
  {
    Com_PrintError(9, "Unable to initialize master voice\n");
  }
}

void __thiscall SD_Shutdown(void *this)
{
  SND_ShutdownMasterVoice(this);
  SD_XAudio2Done();
}

void __thiscall SND_ShutdownMasterVoice(void *this)
{
  unsigned int i; // [esp+0h] [ebp-4h]
  unsigned int ia; // [esp+0h] [ebp-4h]

  ((void (__stdcall *)(IXAudio2 *, void *))g_sd.xa2->StopEngine)(g_sd.xa2, this);
  for ( i = 0; i < 0x4A; i = ia + 1 )
    SND_StopVoice(i);
  g_sd.radverbBus->DestroyVoice(g_sd.radverbBus);
  g_sd.radverbBus = 0;
  g_sd.novoiceBus->DestroyVoice(g_sd.novoiceBus);
  g_sd.novoiceBus = 0;
  ((void (__thiscall *)(IXAudio2SubmixVoice *, IXAudio2SubmixVoice *))g_sd.masterBus->DestroyVoice)(
    g_sd.masterBus,
    g_sd.masterBus);
  g_sd.masterBus = 0;
  g_sd.masterVoice->DestroyVoice(g_sd.masterVoice);
  g_sd.masterVoice = 0;
  g_sd.xa2->Release(g_sd.xa2);
  g_sd.xa2 = 0;
  SDXA2_VoiceDspAssertUnused();
}

void __cdecl SD_PreUpdate()
{
  unsigned int CurrentReverb; // eax
  double v1; // xmm0_8
  float v2; // xmm0_4
  long double v3; // [esp+0h] [ebp-24h]
  float v4; // [esp+0h] [ebp-24h]
  long double v5; // [esp+8h] [ebp-1Ch]
  float v6; // [esp+8h] [ebp-1Ch]
  unsigned int i; // [esp+18h] [ebp-Ch]
  const snd_radverb *radverb; // [esp+1Ch] [ebp-8h]
  const snd_master *master; // [esp+20h] [ebp-4h]

  SND_RvParamsDefault(&g_sd.radverbParams);
  CurrentReverb = SND_GetCurrentReverb();
  radverb = SND_GetRadverb(CurrentReverb);
  g_sd.radverbParams.frameRate = FLOAT_48000_0;
  if ( radverb )
  {
    g_sd.radverbParams.smoothing = radverb->smoothing;
    g_sd.radverbParams.earlyTime = radverb->earlyTime;
    g_sd.radverbParams.lateTime = radverb->lateTime;
    g_sd.radverbParams.earlyGain = radverb->earlyGain;
    g_sd.radverbParams.lateGain = radverb->lateGain;
    g_sd.radverbParams.lateGainProx[3] = g_snd.proximityLateModifier[0];
    g_sd.radverbParams.lateGainProx[1] = g_snd.proximityLateModifier[1];
    g_sd.radverbParams.lateGainProx[2] = g_snd.proximityLateModifier[2];
    g_sd.radverbParams.lateGainProx[0] = g_snd.proximityLateModifier[3];
    g_sd.radverbParams.returnGain = radverb->returnGain;
    g_sd.radverbParams.earlyLpf = radverb->earlyLpf;
    g_sd.radverbParams.lateLpf = radverb->lateLpf;
    g_sd.radverbParams.inputLpf = radverb->inputLpf;
    g_sd.radverbParams.dampLpf = radverb->dampLpf;
    for ( i = 0; i < 4; ++i )
      g_sd.radverbParams.wallReflect[i] = radverb->wallReflect;
    g_sd.radverbParams.dryGain = radverb->dryGain;
    g_sd.radverbParams.earlySize = radverb->earlySize;
    g_sd.radverbParams.lateSize = radverb->lateSize;
    g_sd.radverbParams.diffusion = radverb->diffusion;
    g_sd.radverbParams.delayMatrix = snd_radverb_matrix->current.color[0];
    if ( SND_ActiveListenerCount() == 1 )
    {
      HIDWORD(v5) = LODWORD(g_snd.listeners[0].orient.axis[0][0]);
      v1 = g_snd.listeners[0].orient.axis[0][0];
      __libm_sse2_atan2(v3, v5);
      v2 = v1;
      if ( (float)((float)(v2 + 3.1415927) - 6.2831855) < 0.0 )
        v6 = v2 + 3.1415927;
      else
        v6 = FLOAT_6_2831855;
      if ( (float)(0.0 - (float)(v2 + 3.1415927)) < 0.0 )
        v4 = v6;
      else
        v4 = *(float *)&FLOAT_0_0;
      g_sd.radverbParams.angle = v4;
    }
    else
    {
      g_sd.radverbParams.angle = *(float *)&FLOAT_0_0;
    }
  }
  if ( g_sd.radverbParams.frameRate <= 1000.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
          1346,
          0,
          "%s",
          "g_sd.radverbParams.frameRate > 1000.0f") )
  {
    __debugbreak();
  }
  if ( g_sd.radverbParams.frameRate >= 100000.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
          1347,
          0,
          "%s",
          "g_sd.radverbParams.frameRate < 100000.0f") )
  {
    __debugbreak();
  }
  ((void (__thiscall *)(IXAudio2SubmixVoice *, IXAudio2SubmixVoice *, unsigned int, snd_rv_params *, int, unsigned int))g_sd.radverbBus->SetEffectParameters)(
    g_sd.radverbBus,
    g_sd.radverbBus,
    0,
    &g_sd.radverbParams,
    100,
    0);
  memset((unsigned __int8 *)&g_sd.masterParams, 0, sizeof(g_sd.masterParams));
  master = SND_GetMasterCurrent();
  g_sd.masterParams.lowE = master->lowE;
  g_sd.masterParams.lowG = master->lowG;
  g_sd.masterParams.lowF = master->lowF;
  g_sd.masterParams.lowQ = master->lowQ;
  g_sd.masterParams.peak1E = master->peak1E;
  g_sd.masterParams.peak1G = master->peak1G;
  g_sd.masterParams.peak1F = master->peak1F;
  g_sd.masterParams.peak1Q = master->peak1Q;
  g_sd.masterParams.peak2E = master->peak2E;
  g_sd.masterParams.peak2G = master->peak2G;
  g_sd.masterParams.peak2F = master->peak2F;
  g_sd.masterParams.peak2Q = master->peak2Q;
  g_sd.masterParams.hiE = master->hiE;
  g_sd.masterParams.hiG = master->hiG;
  g_sd.masterParams.hiF = master->hiF;
  g_sd.masterParams.hiQ = master->hiQ;
  g_sd.masterParams.eqG = master->eqG;
  g_sd.masterParams.limitE = master->limitE;
  g_sd.masterParams.limitPG = master->limitPG;
  g_sd.masterParams.limitMG = master->limitMG;
  g_sd.masterParams.limitT = master->limitT;
  g_sd.masterParams.limitR = master->limitR;
  g_sd.masterParams.limitTA = master->limitTA;
  g_sd.masterParams.limitTR = master->limitTR;
  g_sd.masterBus->SetEffectParameters(g_sd.masterBus, 0, &g_sd.masterParams, 96u, 0);
  g_sd.novoiceParams.compE = 0.0;
  g_sd.novoiceParams.compPG = 0.0;
  g_sd.novoiceParams.compMG = 0.0;
  g_sd.novoiceParams.compT = 0.0;
  g_sd.novoiceParams.compR = 0.0;
  g_sd.novoiceParams.compTA = 0.0;
  g_sd.novoiceParams.compTR = 0.0;
  g_sd.novoiceParams = *(snd_dsp_master_no_voice_params *)&master->compE;
  g_sd.novoiceBus->SetEffectParameters(g_sd.novoiceBus, 0, &g_sd.novoiceParams, 28u, 0);
}

void __cdecl SD_PauseVoice(int voiceIndex)
{
  LOBYTE(g_snd.voiceAliasHash[118 * voiceIndex - 8618]) = 1;
  if ( g_snd.voiceAliasHash[118 * voiceIndex - 8732] != 1 )
    g_sd.voices[voiceIndex]->Stop(g_sd.voices[voiceIndex], 0, 0);
}

void __cdecl SD_UnpauseVoice(int voiceIndex)
{
  if ( g_snd.voiceAliasHash[118 * voiceIndex - 8732] != 1 )
    g_sd.voices[voiceIndex]->Start(g_sd.voices[voiceIndex], 0, 0);
}

void __cdecl SD_UpdateVoice(unsigned int voiceIndex)
{
  float Pitch; // [esp+14h] [ebp-20h]
  HRESULT hr; // [esp+1Ch] [ebp-18h]
  XAUDIO2_VOICE_STATE state; // [esp+20h] [ebp-14h] BYREF
  snd_voice_t *voice; // [esp+30h] [ebp-4h]

  if ( voiceIndex >= 0x4A
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
          1431,
          0,
          "voiceIndex doesn't index SND_MAX_VOICES\n\t%i not in [0, %i)",
          voiceIndex,
          74) )
  {
    __debugbreak();
  }
  if ( !g_snd.voiceAliasHash[voiceIndex]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
          1433,
          0,
          "%s",
          "!SND_IsVoiceFree(voiceIndex)") )
  {
    __debugbreak();
  }
  if ( !SND_IsStream(voiceIndex) || (SD_UpdateStreamVoice(voiceIndex), g_snd.voiceAliasHash[voiceIndex]) )
  {
    if ( g_snd.voiceAliasHash[118 * voiceIndex - 8732] != 1 )
    {
      voice = (snd_voice_t *)&g_snd.voiceAliasHash[118 * voiceIndex - 8732];
      SDXA2_UpdateVoiceSends(voiceIndex);
      g_sd.voices[voiceIndex]->GetState(g_sd.voices[voiceIndex], &state);
      if ( !voice->startDelay && !state.SamplesPlayed )
      {
        if ( state.BuffersQueued )
        {
          if ( !voice->paused )
          {
            hr = g_sd.voices[voiceIndex]->Start(g_sd.voices[voiceIndex], 0, 0);
            if ( hr )
            {
              if ( !Assert_MyHandler(
                      "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
                      1461,
                      0,
                      "%s\n\t(hr) = %i",
                      "(hr == ((HRESULT)0L))",
                      hr) )
                __debugbreak();
            }
          }
        }
      }
      if ( state.BuffersQueued )
      {
        Pitch = SND_GetPitch(voice);
        ((void (__stdcall *)(IXAudio2SourceVoice *, unsigned int, unsigned int))g_sd.voices[voiceIndex]->SetFrequencyRatio)(
          g_sd.voices[voiceIndex],
          LODWORD(Pitch),
          0);
      }
      else
      {
        SND_StopVoice(voiceIndex);
      }
    }
  }
}

void __cdecl SDXA2_UpdateVoiceSends(int voiceIndex)
{
  int v1; // eax
  unsigned int out; // [esp+Ch] [ebp-13Ch]
  unsigned int j; // [esp+14h] [ebp-134h]
  unsigned int channel; // [esp+18h] [ebp-130h]
  HRESULT hr; // [esp+1Ch] [ebp-12Ch]
  unsigned int ii; // [esp+20h] [ebp-128h]
  unsigned int n; // [esp+24h] [ebp-124h]
  unsigned int m; // [esp+28h] [ebp-120h]
  unsigned int k; // [esp+2Ch] [ebp-11Ch]
  unsigned int i; // [esp+30h] [ebp-118h]
  unsigned int outputChannels; // [esp+34h] [ebp-114h]
  snd_voice_t *voice; // [esp+38h] [ebp-110h]
  unsigned int inputChannels; // [esp+3Ch] [ebp-10Ch]
  IXAudio2SourceVoice *xvoice; // [esp+40h] [ebp-108h]
  VoiceInfo *params; // [esp+44h] [ebp-104h]
  float volumes[64]; // [esp+48h] [ebp-100h] BYREF

  if ( !g_snd.voiceAliasHash[voiceIndex]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
          499,
          0,
          "%s",
          "!SND_IsVoiceFree(voiceIndex)") )
  {
    __debugbreak();
  }
  voice = (snd_voice_t *)&g_snd.voiceAliasHash[118 * voiceIndex - 8732];
  xvoice = g_sd.voices[voiceIndex];
  params = &g_sd.voiceInfos[voiceIndex];
  params->params.lpfAttenuation = SND_GetLpfLevel(voice);
  params->params.lpfRatio = FLOAT_0_25;
  params->params.frameRate = FLOAT_48000_0;
  params->params.futz.blend = voice->futzBlend;
  params->params.futz.bpfF = snd_futz_bpf_f->current.value;
  params->params.futz.bpfQ = snd_futz_bpf_q->current.value;
  params->params.futz.lsG = snd_futz_ls_g->current.value;
  params->params.futz.lsF = snd_futz_ls_f->current.value;
  params->params.futz.lsQ = snd_futz_ls_q->current.value;
  params->params.futz.distortion = snd_futz_distortion->current.value;
  params->params.futz.pregain = snd_futz_preg->current.value;
  params->params.futz.postgain = snd_futz_postg->current.value;
  params->params.futz.squelch.tg = snd_futz_tg->current.value;
  params->params.futz.squelch.th = snd_futz_th->current.value;
  params->params.futz.preclip = snd_futz_clip_pre->current.value;
  params->params.futz.postclip = snd_futz_clip_post->current.value;
  xvoice->SetEffectParameters(xvoice, 0, params, 64u, 0);
  inputChannels = voice->pan.input_channel_count;
  outputChannels = voice->pan.output_channel_count;
  for ( i = 0; i < 0x40; ++i )
    volumes[i] = *(float *)&FLOAT_0_0;
  switch ( outputChannels )
  {
    case 1u:
      for ( k = 0; k < inputChannels; ++k )
      {
        volumes[k] = Snd_SpeakerMapGetVolume(&voice->pan, k, 0) * 0.5 * voice->wetLevel;
        volumes[inputChannels + k] = Snd_SpeakerMapGetVolume(&voice->pan, k, 0) * 0.5 * voice->wetLevel;
        volumes[2 * inputChannels + k] = Snd_SpeakerMapGetVolume(&voice->pan, k, 0) * 0.5 * voice->wetLevel;
        volumes[3 * inputChannels + k] = Snd_SpeakerMapGetVolume(&voice->pan, k, 0) * 0.5 * voice->wetLevel;
      }
      break;
    case 2u:
      for ( m = 0; m < inputChannels; ++m )
      {
        volumes[m] = Snd_SpeakerMapGetVolume(&voice->pan, m, 0) * 0.70710677 * voice->wetLevel;
        volumes[inputChannels + m] = Snd_SpeakerMapGetVolume(&voice->pan, m, 1) * 0.70710677 * voice->wetLevel;
        volumes[2 * inputChannels + m] = Snd_SpeakerMapGetVolume(&voice->pan, m, 1) * 0.70710677 * voice->wetLevel;
        volumes[3 * inputChannels + m] = Snd_SpeakerMapGetVolume(&voice->pan, m, 0) * 0.70710677 * voice->wetLevel;
      }
      break;
    case 6u:
      for ( n = 0; n < inputChannels; ++n )
      {
        volumes[n] = Snd_SpeakerMapGetVolume(&voice->pan, n, 4) * voice->wetLevel;
        volumes[inputChannels + n] = Snd_SpeakerMapGetVolume(&voice->pan, n, 1) * voice->wetLevel;
        volumes[2 * inputChannels + n] = Snd_SpeakerMapGetVolume(&voice->pan, n, 5) * voice->wetLevel;
        volumes[3 * inputChannels + n] = Snd_SpeakerMapGetVolume(&voice->pan, n, 0) * voice->wetLevel;
        volumes[3 * inputChannels + n] = Snd_SpeakerMapGetVolume(&voice->pan, n, 2) * 0.70710677 * voice->wetLevel
                                       + volumes[3 * inputChannels + n];
        volumes[inputChannels + n] = Snd_SpeakerMapGetVolume(&voice->pan, n, 2) * 0.70710677 * voice->wetLevel
                                   + volumes[inputChannels + n];
      }
      break;
    case 8u:
      for ( ii = 0; ii < inputChannels; ++ii )
      {
        volumes[inputChannels + ii] = Snd_SpeakerMapGetVolume(&voice->pan, ii, 1) * voice->wetLevel;
        volumes[3 * inputChannels + ii] = Snd_SpeakerMapGetVolume(&voice->pan, ii, 0) * voice->wetLevel;
        volumes[3 * inputChannels + ii] = Snd_SpeakerMapGetVolume(&voice->pan, ii, 2) * 0.70710677 * voice->wetLevel
                                        + volumes[3 * inputChannels + ii];
        volumes[inputChannels + ii] = Snd_SpeakerMapGetVolume(&voice->pan, ii, 2) * 0.70710677 * voice->wetLevel
                                    + volumes[inputChannels + ii];
        volumes[2 * inputChannels + ii] = Snd_SpeakerMapGetVolume(&voice->pan, ii, 5) * 0.70710677 * voice->wetLevel;
        volumes[2 * inputChannels + ii] = Snd_SpeakerMapGetVolume(&voice->pan, ii, 7) * 0.70710677 * voice->wetLevel
                                        + volumes[2 * inputChannels + ii];
        volumes[ii] = Snd_SpeakerMapGetVolume(&voice->pan, ii, 6) * 0.70710677 * voice->wetLevel;
        volumes[ii] = Snd_SpeakerMapGetVolume(&voice->pan, ii, 4) * 0.70710677 * voice->wetLevel + volumes[ii];
      }
      break;
  }
  hr = g_sd.voices[voiceIndex]->SetOutputMatrix(g_sd.voices[voiceIndex], g_sd.radverbBus, inputChannels, 4u, volumes, 0);
  if ( hr
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
          587,
          0,
          "%s\n\t(hr) = %i",
          "(hr == ((HRESULT)0L))",
          hr) )
  {
    __debugbreak();
  }
  for ( channel = 0; channel < outputChannels; ++channel )
  {
    for ( j = 0; j < inputChannels; ++j )
    {
      out = SDXA2_ChannelFixup(outputChannels, channel);
      volumes[j + channel * inputChannels] = Snd_SpeakerMapGetVolume(&voice->pan, j, out) * voice->dryLevel;
    }
  }
  if ( (voice->alias->flags & 0x3000) >> 12 == 2 )
    v1 = ((int (__thiscall *)(IXAudio2SourceVoice *, IXAudio2SourceVoice *, IXAudio2SubmixVoice *, unsigned int, unsigned int, float *, unsigned int))xvoice->SetOutputMatrix)(
           xvoice,
           xvoice,
           g_sd.masterBus,
           inputChannels,
           outputChannels,
           volumes,
           0);
  else
    v1 = ((int (__thiscall *)(IXAudio2SourceVoice *, IXAudio2SourceVoice *, IXAudio2SubmixVoice *, unsigned int, unsigned int, float *, unsigned int))xvoice->SetOutputMatrix)(
           xvoice,
           xvoice,
           g_sd.novoiceBus,
           inputChannels,
           outputChannels,
           volumes,
           0);
  if ( v1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
          599,
          0,
          "%s\n\t(hr) = %i",
          "(hr == ((HRESULT)0L))",
          v1) )
  {
    __debugbreak();
  }
}

int __cdecl SD_StartAlias(SndStartAliasInfo *startAliasInfo, unsigned int voice)
{
  if ( voice >= 0x4A
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
          1476,
          0,
          "voice doesn't index SND_MAX_VOICES\n\t%i not in [0, %i)",
          voice,
          74) )
  {
    __debugbreak();
  }
  if ( SND_IsStream(voice) )
    return SND_StartAliasStream(startAliasInfo, voice);
  else
    return SND_StartAliasRam(startAliasInfo, voice);
}

int __cdecl SND_StartAliasRam(SndStartAliasInfo *startAliasInfo, int voiceIndex)
{
  unsigned int v3; // [esp+20h] [ebp-18h]
  unsigned int rate; // [esp+28h] [ebp-10h]
  snd_asset *snd; // [esp+30h] [ebp-8h]
  bool isLooping; // [esp+34h] [ebp-4h]

  if ( g_snd.voiceAliasHash[voiceIndex]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
          1057,
          0,
          "%s",
          "SND_IsVoiceFree(voiceIndex)") )
  {
    __debugbreak();
  }
  isLooping = (startAliasInfo->alias->flags & 1) != 0;
  snd = &startAliasInfo->alias->soundFile->u.loadSnd->sound;
  if ( !iSND_CreateVoice(voiceIndex, snd, isLooping, (startAliasInfo->alias->flags & 0x3000) >> 12) )
    return -1;
  rate = snd->frame_rate;
  if ( isLooping )
    v3 = 0;
  else
    v3 = startAliasInfo->startDelay + (__int64)((double)snd->frame_count * 1000.0 / (double)rate);
  SND_SetSoundFileVoiceInfo(voiceIndex, snd->channel_count, rate, v3, 0, SFLS_LOADED);
  SND_SetVoiceStartInfo(voiceIndex, startAliasInfo);
  if ( startAliasInfo->playbackId == -1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2.cpp",
          1073,
          0,
          "%s",
          "startAliasInfo->playbackId != SND_PLAYBACKID_NOTPLAYED") )
  {
    __debugbreak();
  }
  return startAliasInfo->playbackId;
}

SoundState *__thiscall SoundState::SoundState(SoundState *this)
{
  int v3; // [esp+4h] [ebp-10h]
  StreamVoice *j; // [esp+8h] [ebp-Ch]
  int v5; // [esp+Ch] [ebp-8h]
  SDXA2SourceEffect *i; // [esp+10h] [ebp-4h]

  SDXA2MasterBusEffect::SDXA2MasterBusEffect(&this->masterDsp);
  SDXA2MasterNoVoiceBusEffect::SDXA2MasterNoVoiceBusEffect(&this->novoiceDsp);
  SDXA2RadverbEffect::SDXA2RadverbEffect(&this->radverbDsp);
  v5 = 148;
  for ( i = this->voiceDsp; --v5 >= 0; ++i )
    SDXA2SourceEffect::SDXA2SourceEffect(i);
  v3 = 10;
  for ( j = this->streamVoices; --v3 >= 0; ++j )
    j->__vftable = (StreamVoice_vtbl *)&StreamVoice::`vftable';
  return this;
}

void __stdcall StreamVoice::OnVoiceProcessingPassStart(StreamVoice *this)
{
  ;
}

void __stdcall StreamVoice::OnLoopEnd(StreamVoice *this, void *__formal)
{
  ;
}

void __stdcall StreamVoice::OnBufferEnd(StreamVoice *this, unsigned int p)
{
  iSND_ReleaseStreamBuffer(p >> 8, (unsigned __int8)p);
}

void __stdcall StreamVoice::OnVoiceError(StreamVoice *this, void *__formal, HRESULT a3)
{
  OutputDebugStringA("StreamVoiceError\n");
}

void __thiscall SoundState::~SoundState(SoundState *this)
{
  int v2; // [esp+4h] [ebp-8h]
  SDXA2Effect *i; // [esp+8h] [ebp-4h]

  v2 = 148;
  for ( i = (SDXA2Effect *)this->voices; --v2 >= 0; SDXA2Effect::~SDXA2Effect(i) )
    i = (SDXA2Effect *)((char *)i - 23424);
  SDXA2Effect::~SDXA2Effect(&this->radverbDsp);
  SDXA2Effect::~SDXA2Effect(&this->novoiceDsp);
  SDXA2Effect::~SDXA2Effect(&this->masterDsp);
}

