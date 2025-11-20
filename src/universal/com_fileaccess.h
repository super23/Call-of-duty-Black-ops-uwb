#pragma once

unsigned int __cdecl FS_FileRead(void *ptr, unsigned int len, _iobuf *stream);
unsigned int __cdecl FS_FileWrite(const void *ptr, unsigned int len, _iobuf *stream);
_iobuf *__cdecl FileWrapper_Open(const char *ospath, const char *mode);
_iobuf *__cdecl FS_FileOpenReadBinary(const char *filename);
_iobuf *__cdecl FS_FileOpenReadText(const char *filename);
_iobuf *__cdecl FS_FileOpenWriteBinary(const char *filename);
_iobuf *__cdecl FS_FileOpenAppendText(const char *filename);
_iobuf *__cdecl FS_FileOpenWriteText(const char *filename);
void __cdecl FS_FileClose(_iobuf *stream);
int __cdecl FS_FileSeek(_iobuf *file, int offset, int whence);
int __cdecl FileWrapper_Seek(_iobuf *h, int offset, int origin);
int __cdecl FS_FileGetFileSize(_iobuf *file);
int __cdecl FileWrapper_GetFileSize(_iobuf *h);
