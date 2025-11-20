#pragma once

void __cdecl R_CinematicInitSound(const _GUID *guid);
void __cdecl Unlock_Bink_textures2(IDirect3DDevice9 *d3d_device, BINKTEXTURESET *set_textures);
void __cdecl Unlock_Bink_textures_Callback();
void __cdecl R_Cinematic_BeginLostDevice();
void R_Cinematic_RelinquishIO();
const char *R_Cinematic_CheckBinkError();
void __cdecl R_Cinematic_EndLostDevice();
CinematicTextureSet *R_Cinematic_MakeBinkDrawTextures();
IDirect3DTexture9 *__cdecl R_Cinematic_MakeBinkTexture_PC(
        GfxImage *image,
        unsigned int width,
        unsigned int height,
        int baseImageFlags);
void R_Cinematic_ClearBinkDrawTextures();
void __cdecl R_Cinematic_ClearTexture(IDirect3DTexture9 *texture, int width, int height, unsigned __int8 clearValue);
void __cdecl R_Cinematic_Init();
unsigned int *R_Cinematic_ReserveMemory();
void __cdecl R_Cinematic_Shutdown();
void __cdecl R_Cinematic_ReleaseImages(CinematicTextureSet *textureSet);
void __cdecl R_Cinematic_StartPlayback_Internal(const char *name, unsigned int playbackFlags, int volume);
void __cdecl R_Cinematic_StartNextPlayback();
bool __cdecl R_Cinematic_IsNextReady_Internal();
void __cdecl R_Cinematic_StopPlayback();
int __cdecl _UpdateFrameCallback();
void __cdecl R_Cinematic_UpdateFrame_Core(bool force_wait);
char __cdecl R_Cinematic_AreHunksOpen();
char __cdecl CinematicHunk_IsOpen(CinematicHunk *hunk);
void R_Cinematic_HunksClose();
void __cdecl CinematicHunk_Close(CinematicHunk *hunk);
char __cdecl R_Cinematic_Advance(bool force_wait);
unsigned int __cdecl R_Cinematic_GetPercentageFull();
const char *R_Cinematic_SeizeIO();
void __cdecl R_Cinematic_UpdateTimeInMsec(const BINKREALTIME *binkRealtime);
int __cdecl R_Cinematic_BinkShouldSkip(bool useCustomLogic);
int __cdecl R_Cinematic_BinkShouldWait(bool useCustomLogic);
void R_Cinematic_StopPlayback_Now();
void __cdecl CinematicHunk_Reset(CinematicHunk *hunk);
char __cdecl R_Cinematic_StartPlayback_Now(const char *filename, char playbackFlags);
bool __cdecl CinematicHunk_IsEmpty(CinematicHunk *hunk);
char __cdecl R_Cinematic_HunksOpen(int activeTexture, char playbackFlags);
void __cdecl CinematicHunk_Open(CinematicHunk *hunk, char *memory, int size);
void __cdecl R_Cinematic_HunksAllocate(int activeTexture, char playbackFlags);
int __cdecl CinematicHunk_Alloc(CinematicHunk *hunk, int size);
void __cdecl R_Cinematic_HunksReset(int activeTexture, char playbackFlags);
CinematicTextureSet *R_Cinematic_InitBinkTextures();
void __cdecl R_Cinematic_BlackRendererImages();
void *__stdcall R_Cinematic_Bink_Alloc(unsigned int bytes);
void __stdcall R_Cinematic_Bink_Free(void *ptr);
bool __cdecl R_Cinematic_BinkOpen(const char *filename, char playbackFlags, char *errText, unsigned int errTextSize);
char __cdecl R_Cinematic_BinkOpenPath(char *filepath, char playbackFlags, char *errText, unsigned int errTextSize);
int __cdecl CinematicHunk_GetFreeSpace(CinematicHunk *hunk);
void __cdecl R_Cinematic_InitBinkVolumes();
void __cdecl R_Cinematic_UpdateFrame(bool force_wait);
void R_Cinematic_UpdateRendererImages();
void __cdecl R_Cinematic_SetRendererImagesToFrame(int frameToSetTo);
char __cdecl R_Cinematic_GetFilenameAndTimeInMsec(char *outName, int outNameSize, unsigned int *outTimeInMsec);
bool __cdecl R_Cinematic_IsFinished();
bool __cdecl R_Cinematic_IsStarted();
bool __cdecl R_Cinematic_IsNextReady();
char __cdecl R_Cinematic_IsUnderrun();
void __cdecl R_Cinematic_ForceRelinquishIO();
