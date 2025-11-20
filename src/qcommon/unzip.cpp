#include "unzip.h"

unz_s *__cdecl unzReOpen(const char *path, unsigned __int8 *file)
{
  _iobuf *fin; // [esp+0h] [ebp-8h]
  unz_s *s; // [esp+4h] [ebp-4h]

  fin = FS_FileOpenReadBinary(path);
  if ( !fin )
    return 0;
  s = (unz_s *)malloc(0x80u);
  Com_Memcpy(s, file, 128);
  s->file = fin;
  s->pfile_in_zip_read = 0;
  return s;
}

unz_s *__cdecl unzOpen(const char *path)
{
  unsigned int central_pos; // [esp+8h] [ebp-A4h]
  unz_s us; // [esp+Ch] [ebp-A0h] BYREF
  unsigned int number_disk_with_CD; // [esp+90h] [ebp-1Ch] BYREF
  unsigned int number_entry_CD; // [esp+94h] [ebp-18h] BYREF
  int err; // [esp+98h] [ebp-14h]
  unsigned int uL; // [esp+9Ch] [ebp-10h] BYREF
  _iobuf *fin; // [esp+A0h] [ebp-Ch]
  unz_s *s; // [esp+A4h] [ebp-8h]
  unsigned int number_disk; // [esp+A8h] [ebp-4h] BYREF

  err = 0;
  if ( unz_copyright[0] != 32 )
    return 0;
  fin = FS_FileOpenReadBinary(path);
  if ( !fin )
    return 0;
  central_pos = unzlocal_SearchCentralDir(fin);
  if ( !central_pos )
    err = -1;
  if ( FS_FileSeek(fin, central_pos, 2) )
    err = -1;
  if ( unzlocal_getLong(fin, &uL) )
    err = -1;
  if ( unzlocal_getShort(fin, &number_disk) )
    err = -1;
  if ( unzlocal_getShort(fin, &number_disk_with_CD) )
    err = -1;
  if ( unzlocal_getShort(fin, &us.gi.number_entry) )
    err = -1;
  if ( unzlocal_getShort(fin, &number_entry_CD) )
    err = -1;
  if ( number_entry_CD != us.gi.number_entry || number_disk_with_CD || number_disk )
    err = -103;
  if ( unzlocal_getLong(fin, &us.size_central_dir) )
    err = -1;
  if ( unzlocal_getLong(fin, &us.offset_central_dir) )
    err = -1;
  if ( unzlocal_getShort(fin, &us.gi.size_comment) )
    err = -1;
  if ( central_pos < us.size_central_dir + us.offset_central_dir && !err )
    err = -103;
  if ( err )
  {
    FS_FileClose(fin);
    return 0;
  }
  else
  {
    us.file = fin;
    us.byte_before_the_zipfile = central_pos - (us.size_central_dir + us.offset_central_dir);
    us.central_pos = central_pos;
    us.pfile_in_zip_read = 0;
    s = (unz_s *)malloc(0x80u);
    memcpy(s, &us, sizeof(unz_s));
    unzGoToFirstFile((unsigned __int8 *)s);
    return s;
  }
}

int __cdecl unzlocal_getShort(_iobuf *fin, unsigned int *pX)
{
  __int16 v; // [esp+0h] [ebp-4h] BYREF

  FS_FileRead(&v, 2u, fin);
  *pX = v;
  return 0;
}

int __cdecl unzlocal_getLong(_iobuf *fin, unsigned int *pX)
{
  int v; // [esp+0h] [ebp-4h] BYREF

  FS_FileRead(&v, 4u, fin);
  *pX = v;
  return 0;
}

unsigned int __cdecl unzlocal_SearchCentralDir(_iobuf *fin)
{
  int v2; // edx
  unsigned int v3; // [esp+0h] [ebp-24h]
  unsigned int uReadPos; // [esp+8h] [ebp-1Ch]
  int i; // [esp+Ch] [ebp-18h]
  unsigned __int8 *buf; // [esp+10h] [ebp-14h]
  unsigned int uMaxBack; // [esp+14h] [ebp-10h]
  unsigned int uBackRead; // [esp+18h] [ebp-Ch]
  unsigned int uPosFound; // [esp+1Ch] [ebp-8h]
  unsigned int uSizeFile; // [esp+20h] [ebp-4h]

  uMaxBack = 0xFFFF;
  uPosFound = 0;
  uSizeFile = FS_FileGetFileSize(fin);
  if ( uSizeFile < 0xFFFF )
    uMaxBack = uSizeFile;
  buf = (unsigned __int8 *)malloc(0x404u);
  if ( !buf )
    return 0;
  uBackRead = 4;
  do
  {
    if ( uBackRead >= uMaxBack )
      break;
    if ( uBackRead + 1024 <= uMaxBack )
      uBackRead += 1024;
    else
      uBackRead = uMaxBack;
    uReadPos = uSizeFile - uBackRead;
    v3 = uBackRead <= 0x404 ? uBackRead : 1028;
    if ( FS_FileSeek(fin, uReadPos, 2) || FS_FileRead(buf, v3, fin) != v3 )
      break;
    i = v3 - 3;
    while ( 1 )
    {
      v2 = i--;
      if ( v2 <= 0 )
        break;
      if ( buf[i] == 80 && buf[i + 1] == 75 && buf[i + 2] == 5 && buf[i + 3] == 6 )
      {
        uPosFound = i + uReadPos;
        break;
      }
    }
  }
  while ( !uPosFound );
  free(buf);
  return uPosFound;
}

int __cdecl unzClose(unsigned __int8 *file)
{
  if ( !file )
    return -102;
  if ( *((unsigned int *)file + 31) )
    unzCloseCurrentFile(file);
  FS_FileClose(*(_iobuf **)file);
  free(file);
  return 0;
}

int __cdecl unzGetGlobalInfo(unsigned __int8 *file, unz_global_info_s *pglobal_info)
{
  unsigned int v3; // eax

  if ( !file )
    return -102;
  v3 = *((unsigned int *)file + 2);
  pglobal_info->number_entry = *((unsigned int *)file + 1);
  pglobal_info->size_comment = v3;
  return 0;
}

int __cdecl unzGetCurrentFileInfo(
        unsigned __int8 *file,
        unz_file_info_s *pfile_info,
        char *szFileName,
        unsigned int fileNameBufferSize,
        void *extraField,
        unsigned int extraFieldBufferSize,
        char *szComment,
        unsigned int commentBufferSize)
{
  return unzlocal_GetCurrentFileInfoInternal(
           file,
           pfile_info,
           0,
           szFileName,
           fileNameBufferSize,
           extraField,
           extraFieldBufferSize,
           szComment,
           commentBufferSize);
}

int __cdecl unzlocal_GetCurrentFileInfoInternal(
        unsigned __int8 *file,
        unz_file_info_s *pfile_info,
        unz_file_info_internal_s *pfile_info_internal,
        char *szFileName,
        unsigned int fileNameBufferSize,
        void *extraField,
        unsigned int extraFieldBufferSize,
        char *szComment,
        unsigned int commentBufferSize)
{
  unsigned int size_file_comment; // [esp+8h] [ebp-70h]
  unsigned int len; // [esp+Ch] [ebp-6Ch]
  unsigned int uSizeRead; // [esp+10h] [ebp-68h]
  int err; // [esp+14h] [ebp-64h]
  unz_file_info_internal_s file_info_internal; // [esp+18h] [ebp-60h] BYREF
  int lSeek; // [esp+1Ch] [ebp-5Ch]
  unz_file_info_s file_info; // [esp+20h] [ebp-58h] BYREF
  unsigned int uMagic; // [esp+70h] [ebp-8h] BYREF
  unz_s *s; // [esp+74h] [ebp-4h]

  err = 0;
  lSeek = 0;
  if ( !file )
    return -102;
  s = (unz_s *)file;
  if ( FS_FileSeek(*(_iobuf **)file, *((unsigned int *)file + 3) + *((unsigned int *)file + 5), 2) )
    err = -1;
  if ( !err )
  {
    if ( unzlocal_getLong(s->file, &uMagic) )
    {
      err = -1;
    }
    else if ( (char *)uMagic != &cls.wagerServers[5557].hostName[19] )
    {
      err = -103;
    }
  }
  if ( unzlocal_getShort(s->file, &file_info.version) )
    err = -1;
  if ( unzlocal_getShort(s->file, &file_info.version_needed) )
    err = -1;
  if ( unzlocal_getShort(s->file, &file_info.flag) )
    err = -1;
  if ( unzlocal_getShort(s->file, &file_info.compression_method) )
    err = -1;
  if ( unzlocal_getLong(s->file, &file_info.dosDate) )
    err = -1;
  unzlocal_DosDateToTmuDate(file_info.dosDate, &file_info.tmu_date);
  if ( unzlocal_getLong(s->file, &file_info.crc) )
    err = -1;
  if ( unzlocal_getLong(s->file, &file_info.compressed_size) )
    err = -1;
  if ( unzlocal_getLong(s->file, &file_info.uncompressed_size) )
    err = -1;
  if ( unzlocal_getShort(s->file, &file_info.size_filename) )
    err = -1;
  if ( unzlocal_getShort(s->file, &file_info.size_file_extra) )
    err = -1;
  if ( unzlocal_getShort(s->file, &file_info.size_file_comment) )
    err = -1;
  if ( unzlocal_getShort(s->file, &file_info.disk_num_start) )
    err = -1;
  if ( unzlocal_getShort(s->file, &file_info.internal_fa) )
    err = -1;
  if ( unzlocal_getLong(s->file, &file_info.external_fa) )
    err = -1;
  if ( unzlocal_getLong(s->file, &file_info_internal.offset_curfile) )
    err = -1;
  lSeek += file_info.size_filename;
  if ( !err && szFileName )
  {
    if ( file_info.size_filename >= fileNameBufferSize )
    {
      uSizeRead = fileNameBufferSize;
    }
    else
    {
      szFileName[file_info.size_filename] = 0;
      uSizeRead = file_info.size_filename;
    }
    if ( file_info.size_filename && fileNameBufferSize && FS_FileRead(szFileName, uSizeRead, s->file) != uSizeRead )
      err = -1;
    lSeek -= uSizeRead;
  }
  if ( err || !extraField )
  {
    lSeek += file_info.size_file_extra;
  }
  else
  {
    if ( file_info.size_file_extra >= extraFieldBufferSize )
      len = extraFieldBufferSize;
    else
      len = file_info.size_file_extra;
    if ( lSeek )
    {
      if ( FS_FileSeek(s->file, lSeek, 0) )
        err = -1;
      else
        lSeek = 0;
    }
    if ( file_info.size_file_extra && extraFieldBufferSize && FS_FileRead(extraField, len, s->file) != len )
      err = -1;
    lSeek += file_info.size_file_extra - len;
  }
  if ( err || !szComment )
  {
    lSeek += file_info.size_file_comment;
  }
  else
  {
    if ( file_info.size_file_comment >= commentBufferSize )
    {
      size_file_comment = commentBufferSize;
    }
    else
    {
      szComment[file_info.size_file_comment] = 0;
      size_file_comment = file_info.size_file_comment;
    }
    if ( lSeek )
    {
      if ( FS_FileSeek(s->file, lSeek, 0) )
        err = -1;
      else
        lSeek = 0;
    }
    if ( file_info.size_file_comment
      && commentBufferSize
      && FS_FileRead(szComment, size_file_comment, s->file) != size_file_comment )
    {
      err = -1;
    }
    lSeek += file_info.size_file_comment - size_file_comment;
  }
  if ( !err && pfile_info )
    memcpy(pfile_info, &file_info, sizeof(unz_file_info_s));
  if ( !err )
  {
    if ( pfile_info_internal )
      pfile_info_internal->offset_curfile = file_info_internal.offset_curfile;
  }
  return err;
}

void __cdecl unzlocal_DosDateToTmuDate(unsigned int ulDosDate, tm_unz_s *ptm)
{
  ptm->tm_mday = BYTE2(ulDosDate) & 0x1F;
  ptm->tm_mon = ((unsigned __int16)(HIWORD(ulDosDate) & 0x1E0) >> 5) - 1;
  ptm->tm_year = ((unsigned __int16)(HIWORD(ulDosDate) & 0xFE00) >> 9) + 1980;
  ptm->tm_hour = (unsigned __int16)(ulDosDate & 0xF800) >> 11;
  ptm->tm_min = (unsigned __int16)(ulDosDate & 0x7E0) >> 5;
  ptm->tm_sec = 2 * (ulDosDate & 0x1F);
}

int __cdecl unzGoToFirstFile(unsigned __int8 *file)
{
  int result; // eax

  if ( !file )
    return -102;
  *((unsigned int *)file + 5) = *((unsigned int *)file + 9);
  *((unsigned int *)file + 4) = 0;
  result = unzlocal_GetCurrentFileInfoInternal(
             file,
             (unz_file_info_s *)(file + 40),
             (unz_file_info_internal_s *)file + 30,
             0,
             0,
             0,
             0,
             0,
             0);
  *((unsigned int *)file + 6) = result == 0;
  return result;
}

int __cdecl unzGoToNextFile(unsigned __int8 *file)
{
  int result; // eax

  if ( !file )
    return -102;
  if ( !*((unsigned int *)file + 6) )
    return -100;
  if ( *((unsigned int *)file + 4) + 1 == *((unsigned int *)file + 1) )
    return -100;
  *((unsigned int *)file + 5) += *((unsigned int *)file + 20) + *((unsigned int *)file + 18) + *((unsigned int *)file + 19) + 46;
  ++*((unsigned int *)file + 4);
  result = unzlocal_GetCurrentFileInfoInternal(
             file,
             (unz_file_info_s *)(file + 40),
             (unz_file_info_internal_s *)file + 30,
             0,
             0,
             0,
             0,
             0,
             0);
  *((unsigned int *)file + 6) = result == 0;
  return result;
}

int __cdecl unzGetCurrentFileInfoPosition(unsigned __int8 *file, unsigned int *pos)
{
  if ( !file )
    return -102;
  *pos = *((unsigned int *)file + 5);
  return 0;
}

int __cdecl unzSetCurrentFileInfoPosition(unsigned __int8 *file, unsigned int pos)
{
  if ( !file )
    return -102;
  *((unsigned int *)file + 5) = pos;
  *((unsigned int *)file + 6) = unzlocal_GetCurrentFileInfoInternal(
                            file,
                            (unz_file_info_s *)(file + 40),
                            (unz_file_info_internal_s *)file + 30,
                            0,
                            0,
                            0,
                            0,
                            0,
                            0) == 0;
  return 0;
}

int __cdecl unzOpenCurrentFile(unsigned __int8 *file)
{
  char *v2; // eax
  unsigned int offset_local_extrafield; // [esp+0h] [ebp-1Ch] BYREF
  int err; // [esp+4h] [ebp-18h]
  int Store; // [esp+8h] [ebp-14h]
  unsigned int iSizeVar; // [esp+Ch] [ebp-10h] BYREF
  file_in_zip_read_info_s *pfile_in_zip_read_info; // [esp+10h] [ebp-Ch]
  unsigned int size_local_extrafield; // [esp+14h] [ebp-8h] BYREF
  unz_s *s; // [esp+18h] [ebp-4h]

  err = 0;
  if ( !file )
    return -102;
  s = (unz_s *)file;
  if ( !*((unsigned int *)file + 6) )
    return -102;
  if ( s->pfile_in_zip_read )
    unzCloseCurrentFile(file);
  if ( unzlocal_CheckCurrentFileCoherencyHeader(s, &iSizeVar, &offset_local_extrafield, &size_local_extrafield) )
    return -103;
  pfile_in_zip_read_info = (file_in_zip_read_info_s *)malloc(0x60u);
  if ( !pfile_in_zip_read_info )
    return -104;
  v2 = (char *)malloc(0x4000u);
  pfile_in_zip_read_info->read_buffer = v2;
  pfile_in_zip_read_info->offset_local_extrafield = offset_local_extrafield;
  pfile_in_zip_read_info->size_local_extrafield = size_local_extrafield;
  pfile_in_zip_read_info->pos_local_extrafield = 0;
  if ( pfile_in_zip_read_info->read_buffer )
  {
    pfile_in_zip_read_info->stream_initialised = 0;
    if ( s->cur_file_info.compression_method && s->cur_file_info.compression_method != 8 )
      err = -103;
    Store = s->cur_file_info.compression_method == 0;
    pfile_in_zip_read_info->compression_method = s->cur_file_info.compression_method;
    pfile_in_zip_read_info->file = s->file;
    pfile_in_zip_read_info->byte_before_the_zipfile = s->byte_before_the_zipfile;
    pfile_in_zip_read_info->stream.total_out = 0;
    if ( !Store )
    {
      pfile_in_zip_read_info->stream.zalloc = 0;
      pfile_in_zip_read_info->stream.zfree = 0;
      pfile_in_zip_read_info->stream.opaque = 0;
      err = inflateInit2_(&pfile_in_zip_read_info->stream, -15, "1.2.3", 52);
      if ( !err )
        pfile_in_zip_read_info->stream_initialised = 1;
    }
    pfile_in_zip_read_info->rest_read_compressed = s->cur_file_info.compressed_size;
    pfile_in_zip_read_info->rest_read_uncompressed = s->cur_file_info.uncompressed_size;
    pfile_in_zip_read_info->pos_in_zipfile = s->cur_file_info_internal.offset_curfile + iSizeVar + 30;
    pfile_in_zip_read_info->stream.avail_in = 0;
    s->pfile_in_zip_read = pfile_in_zip_read_info;
    return 0;
  }
  else
  {
    free(pfile_in_zip_read_info);
    return -104;
  }
}

int __cdecl unzlocal_CheckCurrentFileCoherencyHeader(
        unz_s *s,
        unsigned int *piSizeVar,
        unsigned int *poffset_local_extrafield,
        unsigned int *psize_local_extrafield)
{
  unsigned int uFlags; // [esp+0h] [ebp-18h] BYREF
  unsigned int size_filename; // [esp+4h] [ebp-14h] BYREF
  int err; // [esp+8h] [ebp-10h]
  unsigned int uMagic; // [esp+Ch] [ebp-Ch] BYREF
  unsigned int size_extra_field; // [esp+10h] [ebp-8h] BYREF
  unsigned int uData; // [esp+14h] [ebp-4h] BYREF

  err = 0;
  *piSizeVar = 0;
  *poffset_local_extrafield = 0;
  *psize_local_extrafield = 0;
  if ( FS_FileSeek(s->file, s->byte_before_the_zipfile + s->cur_file_info_internal.offset_curfile, 2) )
    return -1;
  if ( !err )
  {
    if ( unzlocal_getLong(s->file, &uMagic) )
    {
      err = -1;
    }
    else if ( (char *)uMagic != &info5[15624] )
    {
      err = -103;
    }
  }
  if ( unzlocal_getShort(s->file, &uData) )
    err = -1;
  if ( unzlocal_getShort(s->file, &uFlags) )
    err = -1;
  if ( unzlocal_getShort(s->file, &uData) )
  {
    err = -1;
  }
  else if ( !err && uData != s->cur_file_info.compression_method )
  {
    err = -103;
  }
  if ( !err && s->cur_file_info.compression_method && s->cur_file_info.compression_method != 8 )
    err = -103;
  if ( unzlocal_getLong(s->file, &uData) )
    err = -1;
  if ( unzlocal_getLong(s->file, &uData) )
  {
    err = -1;
  }
  else if ( !err && uData != s->cur_file_info.crc && (uFlags & 8) == 0 )
  {
    err = -103;
  }
  if ( unzlocal_getLong(s->file, &uData) )
  {
    err = -1;
  }
  else if ( !err && uData != s->cur_file_info.compressed_size && (uFlags & 8) == 0 )
  {
    err = -103;
  }
  if ( unzlocal_getLong(s->file, &uData) )
  {
    err = -1;
  }
  else if ( !err && uData != s->cur_file_info.uncompressed_size && (uFlags & 8) == 0 )
  {
    err = -103;
  }
  if ( unzlocal_getShort(s->file, &size_filename) )
  {
    err = -1;
  }
  else if ( !err && size_filename != s->cur_file_info.size_filename )
  {
    err = -103;
  }
  *piSizeVar += size_filename;
  if ( unzlocal_getShort(s->file, &size_extra_field) )
    err = -1;
  *poffset_local_extrafield = s->cur_file_info_internal.offset_curfile + size_filename + 30;
  *psize_local_extrafield = size_extra_field;
  *piSizeVar += size_extra_field;
  return err;
}

unsigned int __cdecl unzReadCurrentFile(unsigned __int8 *file, unsigned __int8 *buf, unsigned int len)
{
  unsigned int uTotalOutBefore; // [esp+4h] [ebp-30h]
  unsigned int uOutThis; // [esp+10h] [ebp-24h]
  unsigned int uDoCopy; // [esp+18h] [ebp-1Ch]
  unsigned int i; // [esp+1Ch] [ebp-18h]
  unsigned int uReadThis; // [esp+20h] [ebp-14h]
  int err; // [esp+24h] [ebp-10h]
  unsigned int iRead; // [esp+28h] [ebp-Ch]
  file_in_zip_read_info_s *pfile_in_zip_read_info; // [esp+2Ch] [ebp-8h]

  err = 0;
  iRead = 0;
  if ( !file )
    return -102;
  pfile_in_zip_read_info = (file_in_zip_read_info_s *)*((unsigned int *)file + 31);
  if ( !pfile_in_zip_read_info )
    return -102;
  if ( !pfile_in_zip_read_info->read_buffer )
    return -100;
  if ( !len )
    return 0;
  pfile_in_zip_read_info->stream.next_out = buf;
  pfile_in_zip_read_info->stream.avail_out = len;
  if ( len > pfile_in_zip_read_info->rest_read_uncompressed )
    pfile_in_zip_read_info->stream.avail_out = pfile_in_zip_read_info->rest_read_uncompressed;
  while ( pfile_in_zip_read_info->stream.avail_out )
  {
    if ( !pfile_in_zip_read_info->stream.avail_in && pfile_in_zip_read_info->rest_read_compressed )
    {
      uReadThis = 0x4000;
      if ( pfile_in_zip_read_info->rest_read_compressed < 0x4000 )
        uReadThis = pfile_in_zip_read_info->rest_read_compressed;
      if ( !uReadThis )
        return 0;
      if ( FS_FileSeek(
             pfile_in_zip_read_info->file,
             pfile_in_zip_read_info->byte_before_the_zipfile + pfile_in_zip_read_info->pos_in_zipfile,
             2) )
      {
        return -1;
      }
      if ( FS_FileRead(pfile_in_zip_read_info->read_buffer, uReadThis, pfile_in_zip_read_info->file) != uReadThis )
        return -1;
      pfile_in_zip_read_info->pos_in_zipfile += uReadThis;
      pfile_in_zip_read_info->rest_read_compressed -= uReadThis;
      pfile_in_zip_read_info->stream.next_in = (unsigned __int8 *)pfile_in_zip_read_info->read_buffer;
      pfile_in_zip_read_info->stream.avail_in = uReadThis;
    }
    if ( pfile_in_zip_read_info->compression_method )
    {
      uTotalOutBefore = pfile_in_zip_read_info->stream.total_out;
      err = inflate(&pfile_in_zip_read_info->stream, 2);
      uOutThis = pfile_in_zip_read_info->stream.total_out - uTotalOutBefore;
      pfile_in_zip_read_info->rest_read_uncompressed -= uOutThis;
      iRead += uOutThis;
      if ( err == 1 )
        return iRead != 0 ? iRead : 0;
      if ( err )
        break;
    }
    else
    {
      if ( pfile_in_zip_read_info->stream.avail_out >= pfile_in_zip_read_info->stream.avail_in )
        uDoCopy = pfile_in_zip_read_info->stream.avail_in;
      else
        uDoCopy = pfile_in_zip_read_info->stream.avail_out;
      if ( buf )
      {
        for ( i = 0; i < uDoCopy; ++i )
          pfile_in_zip_read_info->stream.next_out[i] = pfile_in_zip_read_info->stream.next_in[i];
      }
      pfile_in_zip_read_info->rest_read_uncompressed -= uDoCopy;
      pfile_in_zip_read_info->stream.avail_in -= uDoCopy;
      pfile_in_zip_read_info->stream.avail_out -= uDoCopy;
      if ( buf )
        pfile_in_zip_read_info->stream.next_out += uDoCopy;
      pfile_in_zip_read_info->stream.next_in += uDoCopy;
      pfile_in_zip_read_info->stream.total_out += uDoCopy;
      iRead += uDoCopy;
    }
  }
  if ( err )
    return err;
  else
    return iRead;
}

unsigned int __cdecl unztell(unsigned __int8 *file)
{
  file_in_zip_read_info_s *pfile_in_zip_read_info; // [esp+0h] [ebp-8h]

  if ( !file )
    return -102;
  pfile_in_zip_read_info = (file_in_zip_read_info_s *)*((unsigned int *)file + 31);
  if ( pfile_in_zip_read_info )
    return pfile_in_zip_read_info->stream.total_out;
  else
    return -102;
}

int __cdecl unzCloseCurrentFile(unsigned __int8 *file)
{
  file_in_zip_read_info_s *pfile_in_zip_read_info; // [esp+4h] [ebp-8h]

  if ( !file )
    return -102;
  pfile_in_zip_read_info = (file_in_zip_read_info_s *)*((unsigned int *)file + 31);
  if ( !pfile_in_zip_read_info )
    return -102;
  free(pfile_in_zip_read_info->read_buffer);
  pfile_in_zip_read_info->read_buffer = 0;
  if ( pfile_in_zip_read_info->stream_initialised )
    inflateEnd(&pfile_in_zip_read_info->stream);
  pfile_in_zip_read_info->stream_initialised = 0;
  free(pfile_in_zip_read_info);
  *((unsigned int *)file + 31) = 0;
  return 0;
}

