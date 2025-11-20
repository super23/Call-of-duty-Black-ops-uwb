#pragma once

SDXA2Effect *__thiscall SDXA2Effect::SDXA2Effect(SDXA2Effect *this, XAPO_REGISTRATION_PROPERTIES *props);
HRESULT __stdcall CXAPOBase::Initialize(CXAPOBase *this, const void *__formal, unsigned int DataByteSize);
unsigned int __stdcall CXAPOBase::CalcInputFrames(CXAPOBase *this, unsigned int InputFrameCount);
void __thiscall SDXA2Effect::~SDXA2Effect(SDXA2Effect *this);
void __stdcall SDXA2Effect::GetParameters(SDXA2Effect *this, void *pParameters, unsigned int ParameterByteSize);
HRESULT __stdcall SDXA2Effect::LockForProcess(
        SDXA2Effect *this,
        unsigned int InputLockedParameterCount,
        const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS *in,
        unsigned int OutputLockedParameterCount,
        const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS *out);
HRESULT __stdcall SDXA2Effect::QueryInterface(SDXA2Effect *this, const _GUID *riid, void **ppInterface);
HRESULT __stdcall CXAPOBase::QueryInterface(CXAPOBase *this, const _GUID *riid, void **ppInterface);
int __stdcall SDXA2Effect::AddRef(SDXA2Effect *this);
int __stdcall SDXA2Effect::Release(SDXA2Effect *this);
int __stdcall CXAPOBase::Release(CXAPOBase *this);
void __stdcall SDXA2Effect::Reset(SDXA2Effect *this);
void __stdcall SDXA2Effect::UnlockForProcess(SDXA2Effect *this);
void __stdcall SDXA2Effect::Process(
        SDXA2Effect *this,
        unsigned int InputProcessParameterCount,
        const XAPO_PROCESS_BUFFER_PARAMETERS *pInputProcessParameters,
        unsigned int OutputProcessParameterCount,
        XAPO_PROCESS_BUFFER_PARAMETERS *pOutputProcessParameters,
        int IsEnabled);
SDXA2SourceEffect *__thiscall SDXA2SourceEffect::SDXA2SourceEffect(SDXA2SourceEffect *this);
void __thiscall SDXA2SourceEffect::Clear(SDXA2SourceEffect *this);
void __thiscall SDXA2SourceEffect::Process(
        SDXA2SourceEffect *this,
        unsigned int channelCount,
        unsigned int frameCount,
        float *data);
void __stdcall SDXA2SourceEffect::SetParameters(SDXA2SourceEffect *this, const void *pParams, unsigned int cbParams);
SDXA2MasterNoVoiceBusEffect *__thiscall SDXA2MasterNoVoiceBusEffect::SDXA2MasterNoVoiceBusEffect(
        SDXA2MasterNoVoiceBusEffect *this);
void __thiscall SDXA2MasterNoVoiceBusEffect::Process(
        SDXA2MasterNoVoiceBusEffect *this,
        unsigned int channelCount,
        unsigned int frameCount,
        float *data);
void __stdcall SDXA2MasterNoVoiceBusEffect::SetParameters(
        SDXA2MasterNoVoiceBusEffect *this,
        const void *pParams,
        unsigned int cbParams);
SDXA2MasterBusEffect *__thiscall SDXA2MasterBusEffect::SDXA2MasterBusEffect(SDXA2MasterBusEffect *this);
void __thiscall SDXA2MasterBusEffect::Process(
        SDXA2MasterBusEffect *this,
        unsigned int channelCount,
        unsigned int frameCount,
        float *data);
void __stdcall SDXA2MasterBusEffect::SetParameters(
        SDXA2MasterBusEffect *this,
        const void *pParams,
        unsigned int cbParams);
SDXA2RadverbEffect *__thiscall SDXA2RadverbEffect::SDXA2RadverbEffect(SDXA2RadverbEffect *this);
void __thiscall SDXA2RadverbEffect::Process(
        SDXA2RadverbEffect *this,
        unsigned int channelCount,
        unsigned int frameCount,
        float *data);
void __stdcall SDXA2RadverbEffect::SetParameters(SDXA2RadverbEffect *this, const void *pParams, unsigned int cbParams);
