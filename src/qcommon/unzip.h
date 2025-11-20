#pragma once

unz_s *__cdecl unzReOpen(const char *path, unsigned __int8 *file);
unz_s *__cdecl unzOpen(const char *path);
int __cdecl unzlocal_getShort(_iobuf *fin, unsigned int *pX);
int __cdecl unzlocal_getLong(_iobuf *fin, unsigned int *pX);
unsigned int __cdecl unzlocal_SearchCentralDir(_iobuf *fin);
int __cdecl unzClose(unsigned __int8 *file);
int __cdecl unzGetGlobalInfo(unsigned __int8 *file, unz_global_info_s *pglobal_info);
int __cdecl unzGetCurrentFileInfo(
        unsigned __int8 *file,
        unz_file_info_s *pfile_info,
        char *szFileName,
        unsigned int fileNameBufferSize,
        void *extraField,
        unsigned int extraFieldBufferSize,
        char *szComment,
        unsigned int commentBufferSize);
int __cdecl unzlocal_GetCurrentFileInfoInternal(
        unsigned __int8 *file,
        unz_file_info_s *pfile_info,
        unz_file_info_internal_s *pfile_info_internal,
        char *szFileName,
        unsigned int fileNameBufferSize,
        void *extraField,
        unsigned int extraFieldBufferSize,
        char *szComment,
        unsigned int commentBufferSize);
void __cdecl unzlocal_DosDateToTmuDate(unsigned int ulDosDate, tm_unz_s *ptm);
int __cdecl unzGoToFirstFile(unsigned __int8 *file);
int __cdecl unzGoToNextFile(unsigned __int8 *file);
int __cdecl unzGetCurrentFileInfoPosition(unsigned __int8 *file, unsigned int *pos);
int __cdecl unzSetCurrentFileInfoPosition(unsigned __int8 *file, unsigned int pos);
int __cdecl unzOpenCurrentFile(unsigned __int8 *file);
int __cdecl unzlocal_CheckCurrentFileCoherencyHeader(
        unz_s *s,
        unsigned int *piSizeVar,
        unsigned int *poffset_local_extrafield,
        unsigned int *psize_local_extrafield);
unsigned int __cdecl unzReadCurrentFile(unsigned __int8 *file, unsigned __int8 *buf, unsigned int len);
unsigned int __cdecl unztell(unsigned __int8 *file);
int __cdecl unzCloseCurrentFile(unsigned __int8 *file);
