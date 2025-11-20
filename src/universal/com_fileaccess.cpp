#include "com_fileaccess.h"

unsigned int __cdecl FS_FileRead(void *ptr, unsigned int len, _iobuf *stream)
{
  unsigned int read_size; // [esp+0h] [ebp-4h]

  ProfLoad_BeginTrackedValue(MAP_PROFILE_FILE_READ);
  read_size = fread(ptr, 1u, len, stream);
  ProfLoad_EndTrackedValue(MAP_PROFILE_FILE_READ);
  return read_size;
}

unsigned int __cdecl FS_FileWrite(const void *ptr, unsigned int len, _iobuf *stream)
{
  return fwrite(ptr, 1u, len, stream);
}

_iobuf *__cdecl FileWrapper_Open(const char *ospath, const char *mode)
{
  int *v3; // eax
  const char *v4; // eax
  const char *v5; // eax
  _iobuf *file; // [esp+0h] [ebp-4h]

  *_errno() = 0;
  file = fopen(ospath, mode);
  if ( file != (_iobuf *)-1 )
    return file;
  v3 = _errno();
  v4 = strerror(*v3);
  v5 = va("Couldn't open file: %s %s", ospath, v4);
  if ( !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\qcommon\\../universal/com_files_wrapper_stdio.h",
          24,
          0,
          v5) )
    __debugbreak();
  return 0;
}

_iobuf *__cdecl FS_FileOpenReadBinary(const char *filename)
{
  _iobuf *file; // [esp+4h] [ebp-4h]

  ProfLoad_BeginTrackedValue(MAP_PROFILE_FILE_OPEN);
  file = FileWrapper_Open(filename, "rb");
  ProfLoad_EndTrackedValue(MAP_PROFILE_FILE_OPEN);
  return file;
}

_iobuf *__cdecl FS_FileOpenReadText(const char *filename)
{
  _iobuf *file; // [esp+4h] [ebp-4h]

  ProfLoad_BeginTrackedValue(MAP_PROFILE_FILE_OPEN);
  file = FileWrapper_Open(filename, "rt");
  ProfLoad_EndTrackedValue(MAP_PROFILE_FILE_OPEN);
  return file;
}

_iobuf *__cdecl FS_FileOpenWriteBinary(const char *filename)
{
  _iobuf *file; // [esp+4h] [ebp-4h]

  ProfLoad_BeginTrackedValue(MAP_PROFILE_FILE_OPEN);
  file = FileWrapper_Open(filename, "wb");
  ProfLoad_EndTrackedValue(MAP_PROFILE_FILE_OPEN);
  return file;
}

_iobuf *__cdecl FS_FileOpenAppendText(const char *filename)
{
  _iobuf *file; // [esp+4h] [ebp-4h]

  ProfLoad_BeginTrackedValue(MAP_PROFILE_FILE_OPEN);
  file = FileWrapper_Open(filename, "at");
  ProfLoad_EndTrackedValue(MAP_PROFILE_FILE_OPEN);
  return file;
}

_iobuf *__cdecl FS_FileOpenWriteText(const char *filename)
{
  _iobuf *file; // [esp+4h] [ebp-4h]

  ProfLoad_BeginTrackedValue(MAP_PROFILE_FILE_OPEN);
  file = FileWrapper_Open(filename, "w+t");
  ProfLoad_EndTrackedValue(MAP_PROFILE_FILE_OPEN);
  return file;
}

void __cdecl FS_FileClose(_iobuf *stream)
{
  fclose(stream);
}

int __cdecl FS_FileSeek(_iobuf *file, int offset, int whence)
{
  int seek; // [esp+4h] [ebp-4h]

  ProfLoad_BeginTrackedValue(MAP_PROFILE_FILE_SEEK);
  seek = FileWrapper_Seek(file, offset, whence);
  ProfLoad_EndTrackedValue(MAP_PROFILE_FILE_SEEK);
  return seek;
}

int __cdecl FileWrapper_Seek(_iobuf *h, int offset, int origin)
{
  const char *v4; // eax

  switch ( origin )
  {
    case 0:
      return fseek(h, offset, 1);
    case 1:
      return fseek(h, offset, 2);
    case 2:
      return fseek(h, offset, 0);
  }
  v4 = va("Bad origin %i in FS_Seek", origin);
  if ( !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\qcommon\\../universal/com_files_wrapper_stdio.h",
          111,
          0,
          v4) )
    __debugbreak();
  return 0;
}

int __cdecl FS_FileGetFileSize(_iobuf *file)
{
  return FileWrapper_GetFileSize(file);
}

int __cdecl FileWrapper_GetFileSize(_iobuf *h)
{
  int startPos; // [esp+0h] [ebp-8h]
  int fileSize; // [esp+4h] [ebp-4h]

  startPos = ftell(h);
  fseek(h, 0, 2);
  fileSize = ftell(h);
  fseek(h, startPos, 0);
  return fileSize;
}

