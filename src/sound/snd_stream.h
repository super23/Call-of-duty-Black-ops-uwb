#pragma once

void __cdecl Snd_StreamBufferInit(snd_buffer *buffer, char *data);
void __cdecl Snd_StreamReset(snd_stream *stream);
void __thiscall tlAtomicMutex::Lock(tlAtomicMutex *this);
void __cdecl Snd_StreamInit();
void __cdecl Snd_StreamFini();
void __cdecl SND_StreamCloseFiles();
void __cdecl Snd_StreamOpen(
        unsigned int index,
        char *filename,
        bool looping,
        unsigned int prime_size,
        char *prime_data);
void __thiscall scoped_performance_error::test(scoped_performance_error *this, const char *__formal);
void __cdecl Snd_StreamClose(unsigned int index);
snd_stream_status __cdecl Snd_StreamStatus(unsigned int index);
const snd_asset *__cdecl Snd_StreamGetHeader(unsigned int index);
void __cdecl Snd_StarvationError();
unsigned int __cdecl Snd_StreamGetFreeWindows(unsigned int index);
char __thiscall tlAtomicMutex::TryLock(tlAtomicMutex *this);
snd_stream_status __cdecl Snd_StreamAcquireWindow(
        unsigned int index,
        unsigned int *size,
        unsigned int *position,
        char **data);
bool __cdecl Snd_StreamIsInBufferStrict(snd_buffer *buffer, unsigned int offset_in_file);
bool __cdecl Snd_StreamIsInBuffer(
        snd_buffer *buffer,
        unsigned int offset_in_file,
        unsigned int stream_length,
        bool looping);
void __cdecl Snd_StreamGetWindow(
        snd_buffer *buffer,
        unsigned int offset_in_file,
        unsigned int stream_length,
        bool looping,
        unsigned int *size,
        char **data);
void __cdecl Snd_StreamReleaseWindow(unsigned int index, char *data);
void __cdecl Snd_StreamReleaseWindowWork(unsigned int index, char *data);
void __cdecl Snd_StreamReleaseProcess(unsigned int index);
void __cdecl Snd_StreamLoadHeader(snd_stream *s, char *data, const char *filename, unsigned int file_size);
void __cdecl Snd_StreamSetError(snd_stream *s, snd_stream_request *r);
char __cdecl Snd_LoadBuffer(unsigned int streamIndex, char *filename, unsigned int offset);
void __cdecl Snd_StreamUpdate();
void __cdecl Snd_StreamGetRequest(snd_stream *s, snd_stream_request *r);
bool __cdecl Snd_StreamSetRequest(snd_stream *s, snd_stream_request *r);
snd_buffer *__cdecl Snd_FindBuffer(const char *filename, unsigned int offset);
char __cdecl Snd_FileClose(snd_stream_file *file);
char __cdecl Snd_ReadBuffer(char *filename, unsigned int start_offset, unsigned int size, snd_buffer *buffer);
char __cdecl Snd_FileOpen(const char *filename, snd_stream_file *file);
char __cdecl Snd_FileRead(snd_stream_file *file, unsigned int offset, unsigned int size, unsigned __int8 *data);
