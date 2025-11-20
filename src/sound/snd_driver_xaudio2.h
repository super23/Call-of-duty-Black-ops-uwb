#pragma once

bool __cdecl SD_Xaudio2CanInit();
IXAudio2SourceVoice **__cdecl SDXA2_VoiceDspAllocate();
void __cdecl SD_TruncateAudioDeviceNames(Font_s *font, float scale, int size);
void __cdecl SD_XAudio2Done();
unsigned int __cdecl SDXA2_ChannelFixup(unsigned int outputChannels, unsigned int channel);
IXAudio2SubmixVoice *__cdecl SD_CreateBus(
        unsigned int inputChannelCount,
        unsigned int outputChannelCount,
        unsigned int sampleRate,
        unsigned int stage,
        SDXA2Effect *dsp,
        IXAudio2Voice *outputVoice);
void __cdecl SD_StopVoice(int voiceIndex);
unsigned int __cdecl iSND_GetStreamChannel(unsigned int index);
void __cdecl iSND_ReleaseStreamBuffer(unsigned int streamVoice, unsigned int bufferIndex);
int __cdecl SND_StartAliasStream(SndStartAliasInfo *startAliasInfo, unsigned int voiceIndex);
void __cdecl SD_UpdateStreamVoice(int voiceIndex);
int __cdecl iSND_CreateVoice(int voiceIndex, const snd_asset *snd, int isLooping, int bus);
void __cdecl iSND_ChannelError(int voiceIndex);
char __cdecl SD_Init();
void SDXA2_VoiceDspAssertUnused();
bool __cdecl SD_XAudio2Init();
HRESULT __cdecl XAudio2Create(
        IXAudio2 **ppXAudio2,
        unsigned int Flags,
        XAUDIO2_WINDOWS_PROCESSOR_SPECIFIER XAudio2Processor);
char __cdecl SD_XAudio2EnumerateDevices();
char __cdecl SD_XAudio2GetDeviceGUID(wchar_t *inGUID, _GUID *guid, char *outGUID, int size);
char __cdecl SD_XAudio2CheckDevice(
        IXAudio2 *pXAudio2,
        const XAUDIO2_DEVICE_DETAILS *deviceDetails,
        unsigned int deviceIndex);
void SD_SwitchDevice();
void SND_InitMasterVoice();
void __thiscall SD_Shutdown(void *this);
void __thiscall SND_ShutdownMasterVoice(void *this);
void __cdecl SD_PreUpdate();
void __cdecl SD_PauseVoice(int voiceIndex);
void __cdecl SD_UnpauseVoice(int voiceIndex);
void __cdecl SD_UpdateVoice(unsigned int voiceIndex);
void __cdecl SDXA2_UpdateVoiceSends(int voiceIndex);
int __cdecl SD_StartAlias(SndStartAliasInfo *startAliasInfo, unsigned int voice);
int __cdecl SND_StartAliasRam(SndStartAliasInfo *startAliasInfo, int voiceIndex);
SoundState *__thiscall SoundState::SoundState(SoundState *this);
void __stdcall StreamVoice::OnVoiceProcessingPassStart(StreamVoice *this);
void __stdcall StreamVoice::OnLoopEnd(StreamVoice *this, void *__formal);
void __stdcall StreamVoice::OnBufferEnd(StreamVoice *this, unsigned int p);
void __stdcall StreamVoice::OnVoiceError(StreamVoice *this, void *__formal, HRESULT a3);
void __thiscall SoundState::~SoundState(SoundState *this);
