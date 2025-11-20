#include "snd_driver_xaudio2_dsp.h"

SDXA2Effect *__thiscall SDXA2Effect::SDXA2Effect(SDXA2Effect *this, XAPO_REGISTRATION_PROPERTIES *props)
{
  CXAPOBase::CXAPOBase(this, props);
  this->CXAPOBase::IXAPO::IUnknown::__vftable = (SDXA2Effect_vtbl *)&SDXA2Effect::`vftable'{for `CXAPOBase'};
  this->IXAPOParameters::IUnknown::__vftable = (IXAPOParameters_vtbl *)&SDXA2Effect::`vftable'{for `IXAPOParameters'};
  this->locked = 0;
  this->started = 0;
  memset((unsigned __int8 *)this->interleave, 0, sizeof(this->interleave));
  return this;
}

HRESULT __stdcall CXAPOBase::Initialize(CXAPOBase *this, const void *__formal, unsigned int DataByteSize)
{
  return 0;
}

unsigned int __stdcall CXAPOBase::CalcInputFrames(CXAPOBase *this, unsigned int InputFrameCount)
{
  return InputFrameCount;
}

void __thiscall SDXA2Effect::~SDXA2Effect(SDXA2Effect *this)
{
  this->CXAPOBase::IXAPO::IUnknown::__vftable = (SDXA2Effect_vtbl *)&SDXA2Effect::`vftable'{for `CXAPOBase'};
  this->IXAPOParameters::IUnknown::__vftable = (IXAPOParameters_vtbl *)&SDXA2Effect::`vftable'{for `IXAPOParameters'};
  CXAPOBase::~CXAPOBase(this);
}

void __stdcall SDXA2Effect::GetParameters(SDXA2Effect *this, void *pParameters, unsigned int ParameterByteSize)
{
  if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp", 36, 0, "%s", "0") )
    __debugbreak();
}

HRESULT __stdcall SDXA2Effect::LockForProcess(
        SDXA2Effect *this,
        unsigned int InputLockedParameterCount,
        const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS *in,
        unsigned int OutputLockedParameterCount,
        const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS *out)
{
  if ( this->locked
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp", 46, 0, "%s", "!locked") )
  {
    __debugbreak();
  }
  this->locked = 1;
  this->started = 0;
  if ( in->MaxFrameCount != out->MaxFrameCount
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp",
          50,
          0,
          "%s",
          "in->MaxFrameCount == out->MaxFrameCount") )
  {
    __debugbreak();
  }
  if ( in->pFormat->wBitsPerSample != out->pFormat->wBitsPerSample
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp",
          51,
          0,
          "%s",
          "in->pFormat->wBitsPerSample == out->pFormat->wBitsPerSample") )
  {
    __debugbreak();
  }
  if ( in->pFormat->wFormatTag != out->pFormat->wFormatTag
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp",
          52,
          0,
          "%s",
          "in->pFormat->wFormatTag == out->pFormat->wFormatTag") )
  {
    __debugbreak();
  }
  if ( in->pFormat->nSamplesPerSec != out->pFormat->nSamplesPerSec
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp",
          53,
          0,
          "%s",
          "in->pFormat->nSamplesPerSec == out->pFormat->nSamplesPerSec") )
  {
    __debugbreak();
  }
  if ( in->pFormat->wFormatTag != 65534
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp",
          54,
          0,
          "%s",
          "in->pFormat->wFormatTag == WAVE_FORMAT_EXTENSIBLE") )
  {
    __debugbreak();
  }
  if ( in->pFormat->wBitsPerSample != 32
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp",
          55,
          0,
          "%s",
          "in->pFormat->wBitsPerSample == 32") )
  {
    __debugbreak();
  }
  this->frameRate = in->pFormat->nSamplesPerSec;
  this->frameCount = in->MaxFrameCount;
  this->channelCount = in->pFormat->nChannels;
  return 0;
}

HRESULT __stdcall SDXA2Effect::QueryInterface(SDXA2Effect *this, const _GUID *riid, void **ppInterface)
{
  IXAPOParameters *v4; // [esp+8h] [ebp-14h]
  HRESULT hr; // [esp+18h] [ebp-4h]

  hr = 0;
  if ( memcmp((const char *)riid, (const char *)&_GUID_a90bc001_e897_e897_55e4_9e4700000001, 16) )
    return CXAPOBase::QueryInterface(this, riid, ppInterface);
  if ( this )
    v4 = &this->IXAPOParameters;
  else
    v4 = 0;
  *ppInterface = v4;
  InterlockedIncrement(&this->m_lReferenceCount);
  return hr;
}

HRESULT __stdcall CXAPOBase::QueryInterface(CXAPOBase *this, const _GUID *riid, void **ppInterface)
{
  HRESULT hr; // [esp+18h] [ebp-4h]

  hr = 0;
  if ( !memcmp((const char *)riid, (const char *)&_GUID_a90bc001_e897_e897_55e4_9e4700000000, 16) )
  {
    *ppInterface = this;
    this->AddRef(this);
  }
  else if ( !memcmp((const char *)riid, (const char *)&_GUID_00000000_0000_0000_c000_000000000046, 16) )
  {
    *ppInterface = this;
    ((void (__thiscall *)(CXAPOBase *, CXAPOBase *))this->AddRef)(this, this);
  }
  else
  {
    *ppInterface = 0;
    return -2147467262;
  }
  return hr;
}

int __stdcall SDXA2Effect::AddRef(SDXA2Effect *this)
{
  return InterlockedIncrement(&this->m_lReferenceCount);
}

int __stdcall SDXA2Effect::Release(SDXA2Effect *this)
{
  return CXAPOBase::Release(this);
}

int __stdcall CXAPOBase::Release(CXAPOBase *this)
{
  int uTmpReferenceCount; // [esp+Ch] [ebp-4h]

  uTmpReferenceCount = InterlockedDecrement(&this->m_lReferenceCount);
  if ( !uTmpReferenceCount && this )
    ((void (__thiscall *)(CXAPOBase *, int))this->~CXAPOBase)(this, 1);
  return uTmpReferenceCount;
}

void __stdcall SDXA2Effect::Reset(SDXA2Effect *this)
{
  if ( this->locked
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp", 96, 0, "%s", "!locked") )
  {
    __debugbreak();
  }
  this->locked = 0;
  this->started = 0;
}

void __stdcall SDXA2Effect::UnlockForProcess(SDXA2Effect *this)
{
  if ( !this->locked
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp", 104, 0, "%s", "locked") )
  {
    __debugbreak();
  }
  this->locked = 0;
}

void __stdcall SDXA2Effect::Process(
        SDXA2Effect *this,
        unsigned int InputProcessParameterCount,
        const XAPO_PROCESS_BUFFER_PARAMETERS *pInputProcessParameters,
        unsigned int OutputProcessParameterCount,
        XAPO_PROCESS_BUFFER_PARAMETERS *pOutputProcessParameters,
        int IsEnabled)
{
  float *input; // [esp+0h] [ebp-8h]
  float *output; // [esp+4h] [ebp-4h]

  if ( !this->locked
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp", 116, 0, "%s", "locked") )
  {
    __debugbreak();
  }
  if ( InputProcessParameterCount != 1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp",
          117,
          0,
          "%s",
          "InputProcessParameterCount == 1") )
  {
    __debugbreak();
  }
  if ( OutputProcessParameterCount != 1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp",
          118,
          0,
          "%s",
          "OutputProcessParameterCount == 1") )
  {
    __debugbreak();
  }
  if ( pInputProcessParameters->ValidFrameCount != this->frameCount
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp",
          119,
          0,
          "%s",
          "pInputProcessParameters[0].ValidFrameCount == frameCount") )
  {
    __debugbreak();
  }
  if ( this->frameCount > 0x1E0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp",
          120,
          0,
          "%s",
          "SDXA2_MAX_FRAME_COUNT >= frameCount") )
  {
    __debugbreak();
  }
  if ( pInputProcessParameters->BufferFlags == XAPO_BUFFER_SILENT )
    memset((unsigned __int8 *)pInputProcessParameters->pBuffer, 0, 4 * this->frameCount * this->channelCount);
  pOutputProcessParameters->BufferFlags = XAPO_BUFFER_VALID;
  input = (float *)pInputProcessParameters->pBuffer;
  output = (float *)pOutputProcessParameters->pBuffer;
  if ( this->frameCount != 480
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp",
          132,
          0,
          "%s",
          "frameCount == 480") )
  {
    __debugbreak();
  }
  SND_DspUninterleave(this->channelCount, this->frameCount, input, this->interleave);
  this->Process(this, this->channelCount, this->frameCount, this->interleave);
  SND_DspInterleave(this->channelCount, this->frameCount, this->interleave, output);
}

SDXA2SourceEffect *__thiscall SDXA2SourceEffect::SDXA2SourceEffect(SDXA2SourceEffect *this)
{
  SDXA2Effect::SDXA2Effect(this, &g_sourceEffectProps);
  this->SDXA2Effect::CXAPOBase::IXAPO::IUnknown::__vftable = (SDXA2SourceEffect_vtbl *)&SDXA2SourceEffect::`vftable'{for `CXAPOBase'};
  this->SDXA2Effect::IXAPOParameters::IUnknown::__vftable = (IXAPOParameters_vtbl *)&SDXA2SourceEffect::`vftable'{for `IXAPOParameters'};
  SDXA2SourceEffect::Clear(this);
  return this;
}

void __thiscall SDXA2SourceEffect::Clear(SDXA2SourceEffect *this)
{
  if ( this->locked
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp",
          165,
          0,
          "%s",
          "!locked") )
  {
    __debugbreak();
  }
  if ( this->started
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp",
          166,
          0,
          "%s",
          "!started") )
  {
    __debugbreak();
  }
  memset((unsigned __int8 *)&this->params, 0, sizeof(this->params));
  memset((unsigned __int8 *)this->state, 0, sizeof(this->state));
}

void __thiscall SDXA2SourceEffect::Process(
        SDXA2SourceEffect *this,
        unsigned int channelCount,
        unsigned int frameCount,
        float *data)
{
  unsigned int i; // [esp+4h] [ebp-4h]

  if ( channelCount > 2
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp",
          174,
          0,
          "%s",
          "channelCount <= SDXA2_MAX_SOURCE_CHANNELS") )
  {
    __debugbreak();
  }
  for ( i = 0; i < channelCount; ++i )
    SND_DspFxSourceMono(
      (const snd_dsp_futz_param *)&this->params,
      &this->state[i],
      frameCount,
      &data[frameCount * i],
      this->tempa,
      this->tempb);
}

void __stdcall SDXA2SourceEffect::SetParameters(SDXA2SourceEffect *this, const void *pParams, unsigned int cbParams)
{
  if ( cbParams != 64
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp",
          191,
          0,
          "%s",
          "cbParams == sizeof(params)") )
  {
    __debugbreak();
  }
  memcpy(&this->interleave[3832], pParams, 0x40u);
}

SDXA2MasterNoVoiceBusEffect *__thiscall SDXA2MasterNoVoiceBusEffect::SDXA2MasterNoVoiceBusEffect(
        SDXA2MasterNoVoiceBusEffect *this)
{
  SDXA2Effect::SDXA2Effect(this, &g_masterNoVoiceEffectProps);
  this->SDXA2Effect::CXAPOBase::IXAPO::IUnknown::__vftable = (SDXA2MasterNoVoiceBusEffect_vtbl *)&SDXA2MasterNoVoiceBusEffect::`vftable'{for `CXAPOBase'};
  this->SDXA2Effect::IXAPOParameters::IUnknown::__vftable = (IXAPOParameters_vtbl *)&SDXA2MasterNoVoiceBusEffect::`vftable'{for `IXAPOParameters'};
  return this;
}

void __thiscall SDXA2MasterNoVoiceBusEffect::Process(
        SDXA2MasterNoVoiceBusEffect *this,
        unsigned int channelCount,
        unsigned int frameCount,
        float *data)
{
  float frameRate; // [esp+0h] [ebp-24h]
  unsigned int i; // [esp+20h] [ebp-4h]

  for ( i = 0; i < channelCount; ++i )
  {
    frameRate = (float)this->frameRate;
    SND_DspFxMasterNoVoiceSingleChannel(
      frameCount,
      frameRate,
      &data[frameCount * i],
      &this->params,
      &this->state[i],
      &g_snd.meters[i]);
  }
}

void __stdcall SDXA2MasterNoVoiceBusEffect::SetParameters(
        SDXA2MasterNoVoiceBusEffect *this,
        const void *pParams,
        unsigned int cbParams)
{
  if ( cbParams != 28
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp",
          237,
          0,
          "%s",
          "cbParams == sizeof(params)") )
  {
    __debugbreak();
  }
  memcpy(&this->interleave[3832], pParams, 0x1Cu);
}

SDXA2MasterBusEffect *__thiscall SDXA2MasterBusEffect::SDXA2MasterBusEffect(SDXA2MasterBusEffect *this)
{
  SDXA2Effect::SDXA2Effect(this, &g_masterEffectProps);
  this->SDXA2Effect::CXAPOBase::IXAPO::IUnknown::__vftable = (SDXA2MasterBusEffect_vtbl *)&SDXA2MasterBusEffect::`vftable'{for `CXAPOBase'};
  this->SDXA2Effect::IXAPOParameters::IUnknown::__vftable = (IXAPOParameters_vtbl *)&SDXA2MasterBusEffect::`vftable'{for `IXAPOParameters'};
  memset((unsigned __int8 *)&this->params, 0, sizeof(this->params));
  memset((unsigned __int8 *)this->state, 0, sizeof(this->state));
  return this;
}

void __thiscall SDXA2MasterBusEffect::Process(
        SDXA2MasterBusEffect *this,
        unsigned int channelCount,
        unsigned int frameCount,
        float *data)
{
  float frameRate; // [esp+0h] [ebp-24Ch]
  float v5; // [esp+14h] [ebp-238h]
  float v6; // [esp+18h] [ebp-234h]
  unsigned int v7; // [esp+1Ch] [ebp-230h]
  unsigned int channel; // [esp+2Ch] [ebp-220h]
  float frameSample; // [esp+30h] [ebp-21Ch]
  unsigned int frame; // [esp+34h] [ebp-218h]
  __int16 shorts[258]; // [esp+3Ch] [ebp-210h] BYREF
  unsigned int framesProcessed; // [esp+244h] [ebp-8h]
  unsigned int i; // [esp+248h] [ebp-4h]

  for ( i = 0; i < channelCount; ++i )
  {
    frameRate = (float)this->frameRate;
    SND_DspFxMasterSingleChannel(
      frameCount,
      frameRate,
      &data[frameCount * i],
      &this->params,
      &this->state[i],
      &g_snd.meters[i]);
  }
  if ( mjpeg_is_encoding() )
  {
    for ( framesProcessed = 0; framesProcessed < frameCount; framesProcessed += v7 )
    {
      if ( frameCount - framesProcessed >= 0x100 )
        v7 = 256;
      else
        v7 = frameCount - framesProcessed;
      for ( frame = 0; frame < v7; ++frame )
      {
        frameSample = *(float *)&FLOAT_0_0;
        for ( channel = 0; channel < channelCount; ++channel )
          frameSample = frameSample + data[frameCount * channel + framesProcessed + frame];
        if ( frameSample >= 1.0 )
          v6 = FLOAT_1_0;
        else
          v6 = frameSample;
        if ( v6 <= -1.0 )
          v5 = FLOAT_N1_0;
        else
          v5 = v6;
        shorts[frame] = (int)(float)(v5 * 32767.0);
      }
      mjpeg_add_samples(shorts, v7);
    }
  }
}

void __stdcall SDXA2MasterBusEffect::SetParameters(
        SDXA2MasterBusEffect *this,
        const void *pParams,
        unsigned int cbParams)
{
  if ( cbParams != 96
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp",
          315,
          0,
          "%s",
          "cbParams == sizeof(params)") )
  {
    __debugbreak();
  }
  memcpy(&this->interleave[3832], pParams, 0x60u);
}

SDXA2RadverbEffect *__thiscall SDXA2RadverbEffect::SDXA2RadverbEffect(SDXA2RadverbEffect *this)
{
  SDXA2Effect::SDXA2Effect(this, &g_RadverbEffectProps);
  this->SDXA2Effect::CXAPOBase::IXAPO::IUnknown::__vftable = (SDXA2RadverbEffect_vtbl *)&SDXA2RadverbEffect::`vftable'{for `CXAPOBase'};
  this->SDXA2Effect::IXAPOParameters::IUnknown::__vftable = (IXAPOParameters_vtbl *)&SDXA2RadverbEffect::`vftable'{for `IXAPOParameters'};
  SND_RvParamsDefault(&this->params);
  return this;
}

void __thiscall SDXA2RadverbEffect::Process(
        SDXA2RadverbEffect *this,
        unsigned int channelCount,
        unsigned int frameCount,
        float *data)
{
  if ( channelCount != 4
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp",
          344,
          0,
          "%s",
          "channelCount == 4") )
  {
    __debugbreak();
  }
  if ( this->params.earlySize <= 0.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp",
          345,
          0,
          "%s",
          "params.earlySize > 0") )
  {
    __debugbreak();
  }
  SND_RvFrame(
    &this->params,
    &this->state,
    (const float *)frameCount,
    &data[3 * frameCount],
    &data[frameCount],
    data,
    &data[2 * frameCount],
    &this->temp[3 * frameCount],
    &this->temp[frameCount],
    this->temp,
    &this->temp[2 * frameCount]);
  memcpy((unsigned __int8 *)data, (unsigned __int8 *)this->temp, frameCount * 4 * channelCount);
}

void __stdcall SDXA2RadverbEffect::SetParameters(SDXA2RadverbEffect *this, const void *pParams, unsigned int cbParams)
{
  if ( cbParams != 100
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp",
          368,
          0,
          "%s",
          "cbParams == sizeof(params)") )
  {
    __debugbreak();
  }
  memcpy(&this->state.delayIndex + 15, pParams, 0x64u);
  if ( *((float *)&this->state.delayIndex + 15) <= 1000.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp",
          370,
          0,
          "%s",
          "params.frameRate > 1000.0f") )
  {
    __debugbreak();
  }
  if ( *((float *)&this->state.delayIndex + 15) >= 100000.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_driver_xaudio2_dsp.cpp",
          371,
          0,
          "%s",
          "params.frameRate < 100000.0f") )
  {
    __debugbreak();
  }
}

