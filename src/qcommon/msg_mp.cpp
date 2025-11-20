#include "msg_mp.h"

float (*__cdecl MSG_GetMapCenter())[3]
{
  return SV_GetMapCenter();
}

int __cdecl GetMinBitCountForNum(unsigned int num)
{
  int v2; // eax

  if ( !_BitScanReverse((unsigned int *)&v2, num) )
    v2 = `CountLeadingZeros'::`2'::notFound;
  return 32 - (v2 ^ 0x1F);
}

void __cdecl MSG_Init(msg_t *buf, unsigned __int8 *data, int length)
{
  if ( !msgInit )
    MSG_InitHuffman();
  memset((unsigned __int8 *)buf, 0, sizeof(msg_t));
  buf->data = data;
  buf->maxsize = length;
  buf->readOnly = 0;
  buf->splitData = 0;
  buf->splitSize = 0;
}

void __cdecl MSG_InitReadOnly(msg_t *buf, unsigned __int8 *data, int length)
{
  if ( !data && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 94, 0, "%s", "data") )
    __debugbreak();
  if ( !msgInit )
    MSG_InitHuffman();
  buf->readOnly = 1;
  buf->data = data;
  buf->maxsize = length;
  buf->cursize = length;
  buf->splitData = 0;
  buf->splitSize = 0;
}

void __cdecl MSG_InitReadOnlySplit(msg_t *buf, unsigned __int8 *data, int length, unsigned __int8 *data2, int length2)
{
  if ( !data && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 111, 0, "%s", "data") )
    __debugbreak();
  if ( !data2 && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 112, 0, "%s", "data2") )
    __debugbreak();
  if ( !msgInit )
    MSG_InitHuffman();
  buf->readOnly = 1;
  buf->data = data;
  buf->maxsize = length2 + length;
  buf->cursize = length;
  buf->splitData = data2;
  buf->splitSize = length2;
}

void __cdecl MSG_BeginReading(msg_t *msg)
{
  msg->overflowed = 0;
  msg->readcount = 0;
  msg->bit = 0;
}

void __cdecl MSG_Discard(msg_t *msg)
{
  msg->overflowed = 1;
  msg->cursize = msg->readcount;
  msg->splitSize = 0;
}

int __cdecl MSG_GetUsedBitCount(const msg_t *msg)
{
  return 8 * (msg->splitSize + msg->cursize) - ((8 - msg->bit) & 7);
}

int __cdecl MSG_GetNumBitsRead(const msg_t *msg)
{
  return 8 * msg->readcount - ((8 - msg->bit) & 7);
}

void __cdecl MSG_SetBookmark(const msg_t *msg, msg_bookmark_t *bookmark)
{
  int msgBit; // [esp+0h] [ebp-4h]

  if ( msg->readOnly
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 184, 0, "%s", "!msg->readOnly") )
  {
    __debugbreak();
  }
  msgBit = msg->bit;
  bookmark->overflowed = msg->overflowed;
  bookmark->cursize = msg->cursize;
  bookmark->bit = msgBit;
  bookmark->lastEntityRef = msg->lastEntityRef;
  bookmark->bitByte = msg->data[msgBit >> 3];
  bookmark->readcount = msg->readcount;
}

void __cdecl MSG_GotoBookmark(msg_t *msg, const msg_bookmark_t *bookmark)
{
  int msgBit; // [esp+0h] [ebp-4h]

  if ( msg->readOnly
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 197, 0, "%s", "!msg->readOnly") )
  {
    __debugbreak();
  }
  msgBit = bookmark->bit;
  msg->overflowed = bookmark->overflowed;
  msg->cursize = bookmark->cursize;
  msg->bit = msgBit;
  msg->lastEntityRef = bookmark->lastEntityRef;
  msg->data[msgBit >> 3] = bookmark->bitByte;
  msg->readcount = bookmark->readcount;
}

void __cdecl MSG_WriteBits(msg_t *msg, int value, unsigned int bits)
{
  int bit; // [esp+4h] [ebp-4h]

  if ( bits > 0x20
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 277, 0, "%s", "(unsigned)bits <= 32") )
  {
    __debugbreak();
  }
  if ( msg->readOnly
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 278, 0, "%s", "!msg->readOnly") )
  {
    __debugbreak();
  }
  if ( msg->maxsize - msg->cursize >= 4 )
  {
    while ( bits )
    {
      --bits;
      bit = msg->bit & 7;
      if ( !bit )
      {
        msg->bit = 8 * msg->cursize;
        msg->data[msg->cursize++] = 0;
      }
      if ( (value & 1) != 0 )
        msg->data[msg->bit >> 3] |= 1 << bit;
      ++msg->bit;
      value >>= 1;
    }
  }
  else
  {
    msg->overflowed = 1;
  }
}

void __cdecl MSG_WriteBit0(msg_t *msg)
{
  if ( msg->readOnly )
  {
    if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 309, 0, "%s", "!msg->readOnly") )
      __debugbreak();
  }
  if ( (msg->bit & 7) == 0 )
  {
    if ( msg->cursize >= msg->maxsize )
    {
      msg->overflowed = 1;
      return;
    }
    msg->bit = 8 * msg->cursize;
    msg->data[msg->cursize++] = 0;
  }
  ++msg->bit;
}

void __cdecl MSG_WriteBit1(msg_t *msg)
{
  int bit; // [esp+4h] [ebp-4h]

  if ( msg->readOnly )
  {
    if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 332, 0, "%s", "!msg->readOnly") )
      __debugbreak();
  }
  bit = msg->bit & 7;
  if ( !bit )
  {
    if ( msg->cursize >= msg->maxsize )
    {
      msg->overflowed = 1;
      return;
    }
    msg->bit = 8 * msg->cursize;
    msg->data[msg->cursize++] = 0;
  }
  msg->data[msg->bit++ >> 3] |= 1 << bit;
}

int __cdecl MSG_ReadBits(msg_t *msg, unsigned int bits)
{
  int bit; // [esp+0h] [ebp-Ch]
  int i; // [esp+4h] [ebp-8h]
  int value; // [esp+8h] [ebp-4h]

  if ( bits > 0x20
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 359, 0, "%s", "(unsigned)bits <= 32") )
  {
    __debugbreak();
  }
  value = 0;
  for ( i = 0; i < (int)bits; ++i )
  {
    bit = msg->bit & 7;
    if ( !bit )
    {
      if ( msg->readcount >= msg->splitSize + msg->cursize )
      {
        msg->overflowed = 1;
        return -1;
      }
      msg->bit = 8 * msg->readcount++;
    }
    value |= ((MSG_GetByte(msg, msg->bit >> 3) >> bit) & 1) << i;
    ++msg->bit;
  }
  return value;
}

int __cdecl MSG_GetByte(msg_t *msg, int where)
{
  if ( where < msg->cursize )
    return msg->data[where];
  if ( !msg->splitData
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 147, 0, "%s", "msg->splitData") )
  {
    __debugbreak();
  }
  return msg->splitData[where - msg->cursize];
}

int __cdecl MSG_ReadBit(msg_t *msg)
{
  int Byte; // eax
  int bit; // [esp+0h] [ebp-8h]

  bit = msg->bit & 7;
  if ( !bit )
  {
    if ( msg->readcount >= msg->splitSize + msg->cursize )
    {
      msg->overflowed = 1;
      return -1;
    }
    msg->bit = 8 * msg->readcount++;
  }
  Byte = MSG_GetByte(msg, msg->bit >> 3);
  ++msg->bit;
  return (Byte >> bit) & 1;
}

unsigned int __cdecl MSG_CompressWithZLib(
        unsigned __int8 *from,
        unsigned int inSizeBytes,
        unsigned __int8 *to,
        unsigned int outSizeBytes)
{
  int v5; // [esp+0h] [ebp-3Ch]
  int err; // [esp+4h] [ebp-38h]
  int erra; // [esp+4h] [ebp-38h]
  z_stream_s stream; // [esp+8h] [ebp-34h] BYREF

  stream.next_in = from;
  stream.avail_in = inSizeBytes;
  stream.next_out = to;
  stream.avail_out = outSizeBytes;
  memset(&stream.zalloc, 0, 16);
  err = deflateInit2_(&stream, 9, 8, -13, 1, 0, "1.2.3", 52);
  if ( !err )
  {
    erra = deflate(&stream, 4u);
    if ( erra == 1 )
    {
      err = deflateEnd(&stream);
    }
    else
    {
      deflateEnd(&stream);
      if ( erra )
        v5 = erra;
      else
        v5 = -5;
      err = v5;
    }
  }
  if ( err )
    return 0;
  else
    return stream.total_out;
}

int __cdecl MSG_WriteBitsCompress(
        bool trainHuffman,
        unsigned __int8 *from,
        int fromSizeBytes,
        unsigned __int8 *to,
        int toSizeBytes)
{
  int bit; // [esp+0h] [ebp-Ch] BYREF
  int compressedSize; // [esp+4h] [ebp-8h]
  int i; // [esp+8h] [ebp-4h]

  if ( msg_zlibCompress->current.enabled
    && (*to = 2, (compressedSize = MSG_CompressWithZLib(from, fromSizeBytes, to + 1, toSizeBytes - 1)) != 0)
    && compressedSize < fromSizeBytes + 1 )
  {
    if ( msg_zlibCompressOutput->current.enabled )
      Com_Printf(15, "zlib compress %d->%d\n", fromSizeBytes, compressedSize);
    return compressedSize + 1;
  }
  else
  {
    if ( trainHuffman )
    {
      for ( i = 0; i < fromSizeBytes; ++i )
        ++huffBytesSeen[from[i]];
    }
    *to = 1;
    bit = 0;
    for ( i = 0; i < fromSizeBytes; ++i )
      Huff_offsetTransmit(&msgHuff.compressDecompress, from[i], to + 1, &bit);
    compressedSize = ((bit + 7) >> 3) + 1;
    if ( compressedSize > fromSizeBytes + 1 )
    {
      *to = 0;
      memcpy(to + 1, from, fromSizeBytes);
      return fromSizeBytes + 1;
    }
    return compressedSize;
  }
}

void __cdecl showHuffmanData()
{
  unsigned int i; // [esp+0h] [ebp-4h]

  for ( i = 0; i < 0x100; ++i )
    Com_Printf(15, "\t%i,\t\t// %i\n", huffBytesSeen[i] >> 4, i);
}

unsigned int __cdecl MSG_DecompressWithZLib(
        unsigned __int8 *from,
        unsigned int inSizeBytes,
        unsigned __int8 *to,
        unsigned int outSizeBytes)
{
  int v5; // [esp+0h] [ebp-3Ch]
  int err; // [esp+4h] [ebp-38h]
  int erra; // [esp+4h] [ebp-38h]
  z_stream_s stream; // [esp+8h] [ebp-34h] BYREF

  stream.next_in = from;
  stream.avail_in = inSizeBytes;
  stream.next_out = to;
  stream.avail_out = outSizeBytes;
  memset(&stream.zalloc, 0, 16);
  err = inflateInit2_(&stream, -13, "1.2.3", 52);
  if ( !err )
  {
    erra = inflate(&stream, 4);
    if ( erra == 1 )
    {
      err = inflateEnd(&stream);
    }
    else
    {
      inflateEnd(&stream);
      if ( erra )
        v5 = erra;
      else
        v5 = -5;
      err = v5;
    }
  }
  if ( err )
    return 0;
  else
    return stream.total_out;
}

unsigned int __cdecl MSG_ReadBitsCompress(
        unsigned __int8 *from,
        int fromSizeBytes,
        unsigned __int8 *to,
        unsigned int toSizeBytes)
{
  const char *v5; // eax
  int bit; // [esp+4h] [ebp-18h] BYREF
  unsigned __int8 *data; // [esp+8h] [ebp-14h]
  int bits; // [esp+Ch] [ebp-10h]
  unsigned __int8 compressionType; // [esp+13h] [ebp-9h]
  int i; // [esp+14h] [ebp-8h]
  int get; // [esp+18h] [ebp-4h] BYREF
  unsigned __int8 *froma; // [esp+24h] [ebp+8h]
  unsigned int fromSizeBytesa; // [esp+28h] [ebp+Ch]
  unsigned int toSizeBytesa; // [esp+30h] [ebp+14h]

  compressionType = *from;
  froma = from + 1;
  fromSizeBytesa = fromSizeBytes - 1;
  if ( compressionType )
  {
    if ( compressionType == 1 )
    {
      bits = 8 * fromSizeBytesa;
      i = 0;
      data = to;
      bit = 0;
      while ( bit < bits )
      {
        Huff_offsetReceive(msgHuff.compressDecompress.tree, &get, froma, &bit);
        *data++ = get;
      }
      return data - to;
    }
    else if ( compressionType == 2 )
    {
      toSizeBytesa = MSG_DecompressWithZLib(froma, fromSizeBytesa, to, toSizeBytes);
      if ( msg_zlibCompressOutput->current.enabled )
        Com_Printf(15, "zlib decompress %d\n", toSizeBytesa);
      return toSizeBytesa;
    }
    else
    {
      v5 = va("Unknown compression protocol %d", compressionType);
      if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 578, 0, v5) )
        __debugbreak();
      return 0;
    }
  }
  else
  {
    memcpy(to, froma, fromSizeBytesa);
    return fromSizeBytesa;
  }
}

void __cdecl MSG_Embed(msg_t *dest, msg_t *src)
{
  unsigned int padBits; // [esp+0h] [ebp-4h]

  if ( dest->maxsize - dest->cursize >= src->cursize + 1 )
  {
    padBits = (5 - dest->bit) & 7;
    MSG_WriteBits(dest, padBits, 3u);
    MSG_WriteBits(dest, 0, padBits);
    memcpy(&dest->data[dest->cursize], src->data, src->cursize);
    dest->bit = src->bit + 8 * dest->cursize;
    dest->cursize += src->cursize;
  }
  else
  {
    dest->overflowed = 1;
  }
}

void __cdecl MSG_UnEmbed(msg_t *msg)
{
  unsigned int padBits; // [esp+0h] [ebp-4h]

  padBits = MSG_ReadBits(msg, 3u);
  MSG_ReadBits(msg, padBits);
}

void __cdecl MSG_WriteByte(msg_t *msg, unsigned __int8 c)
{
  if ( msg->readOnly
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 620, 0, "%s", "!msg->readOnly") )
  {
    __debugbreak();
  }
  if ( msg->cursize >= msg->maxsize )
    msg->overflowed = 1;
  else
    msg->data[msg->cursize++] = c;
}

void __cdecl MSG_WriteData(msg_t *buf, unsigned __int8 *data, unsigned int length)
{
  int newsize; // [esp+0h] [ebp-4h]

  if ( buf->readOnly
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 636, 0, "%s", "!buf->readOnly") )
  {
    __debugbreak();
  }
  newsize = length + buf->cursize;
  if ( newsize > buf->maxsize )
  {
    buf->overflowed = 1;
  }
  else
  {
    memcpy(&buf->data[buf->cursize], data, length);
    buf->cursize = newsize;
  }
}

void __cdecl MSG_WriteShort(msg_t *msg, __int16 c)
{
  int newsize; // [esp+0h] [ebp-4h]

  if ( msg->readOnly
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 653, 0, "%s", "!msg->readOnly") )
  {
    __debugbreak();
  }
  newsize = msg->cursize + 2;
  if ( newsize > msg->maxsize )
  {
    msg->overflowed = 1;
  }
  else
  {
    *(_WORD *)&msg->data[msg->cursize] = c;
    msg->cursize = newsize;
  }
}

void __cdecl MSG_WriteLong(msg_t *msg, int c)
{
  int newsize; // [esp+0h] [ebp-4h]

  if ( msg->readOnly
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 671, 0, "%s", "!msg->readOnly") )
  {
    __debugbreak();
  }
  newsize = msg->cursize + 4;
  if ( newsize > msg->maxsize )
  {
    msg->overflowed = 1;
  }
  else
  {
    *(unsigned int *)&msg->data[msg->cursize] = c;
    msg->cursize = newsize;
  }
}

void __cdecl MSG_WriteFloat(msg_t *sb, int f)
{
  if ( *(float *)&f == NAN
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 694, 0, "%s", "dat.l != -1") )
  {
    __debugbreak();
  }
  MSG_WriteLong(sb, f);
}

void __cdecl MSG_WriteInt64(msg_t *msg, unsigned __int64 c)
{
  int newsize; // [esp+4h] [ebp-4h]

  if ( msg->readOnly
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 703, 0, "%s", "!msg->readOnly") )
  {
    __debugbreak();
  }
  newsize = msg->cursize + 8;
  if ( newsize > msg->maxsize )
  {
    msg->overflowed = 1;
  }
  else
  {
    *(_QWORD *)&msg->data[msg->cursize] = LittleLong64(c);
    msg->cursize = newsize;
  }
}

void __cdecl MSG_WriteString(msg_t *sb, const char *s)
{
  unsigned __int8 v2; // al
  int l; // [esp+10h] [ebp-40Ch]
  char string[1024]; // [esp+14h] [ebp-408h] BYREF
  int i; // [esp+418h] [ebp-4h]

  if ( !s && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 722, 0, "%s", "s") )
    __debugbreak();
  if ( sb->readOnly
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 723, 0, "%s", "!sb->readOnly") )
  {
    __debugbreak();
  }
  l = strlen(s);
  if ( l < 1024 )
  {
    for ( i = 0; i < l; ++i )
    {
      v2 = I_CleanChar(s[i]);
      string[i] = v2;
    }
    string[i] = 0;
    MSG_WriteData(sb, (unsigned __int8 *)string, l + 1);
  }
  else
  {
    Com_Printf(16, "MSG_WriteString: MAX_STRING_CHARS");
    MSG_WriteData(sb, (unsigned __int8 *)&toastPopupTitle, 1u);
  }
}

void __cdecl MSG_WriteBigString(msg_t *sb, char *s)
{
  unsigned __int8 v2; // al
  int v3; // [esp+10h] [ebp-400Ch]
  char dest[16384]; // [esp+14h] [ebp-4008h] BYREF
  int i; // [esp+4018h] [ebp-4h]

  if ( !s && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 747, 0, "%s", "s") )
    __debugbreak();
  if ( sb->readOnly
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 748, 0, "%s", "!sb->readOnly") )
  {
    __debugbreak();
  }
  v3 = strlen(s);
  if ( v3 < 0x4000 )
  {
    I_strncpyz(dest, s, 0x4000);
    for ( i = 0; i < v3; ++i )
    {
      v2 = I_CleanChar(dest[i]);
      dest[i] = v2;
    }
    MSG_WriteData(sb, (unsigned __int8 *)dest, v3 + 1);
  }
  else
  {
    Com_Printf(16, "MSG_WriteString: BIG_INFO_STRING");
    MSG_WriteData(sb, (unsigned __int8 *)&toastPopupTitle, 1u);
  }
}

void __cdecl MSG_WriteAngle(msg_t *sb, unsigned int bits, float f)
{
  if ( sb->readOnly
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 769, 0, "%s", "!sb->readOnly") )
  {
    __debugbreak();
  }
  MSG_WriteBits(sb, ((1 << bits) - 1) & (int)(float)((float)((float)(1 << bits) * f) / 360.0), bits);
}

void __cdecl MSG_WriteAngle16(msg_t *sb, float f)
{
  if ( sb->readOnly
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 776, 0, "%s", "!sb->readOnly") )
  {
    __debugbreak();
  }
  MSG_WriteShort(sb, (int)(float)(f * 182.04445));
}

void __cdecl MSG_WriteRangedFloatBits(msg_t *sb, float f, float begin, float end, unsigned int bits)
{
  int v5; // [esp+0h] [ebp-1Ch]
  int v6; // [esp+Ch] [ebp-10h]
  float fa; // [esp+28h] [ebp+Ch]

  fa = (float)((float)((1 << bits) - 1) / (float)(end - begin)) * (float)(f - begin);
  if ( (int)(float)(fa + 0.5) < (int)(float)((1 << bits) - 1) )
    v6 = (int)(float)(fa + 0.5);
  else
    v6 = (int)(float)((1 << bits) - 1);
  if ( v6 > 0 )
    v5 = v6;
  else
    v5 = 0;
  MSG_WriteBits(sb, v5, bits);
}

int __cdecl MSG_ReadByte(msg_t *msg)
{
  int c; // [esp+0h] [ebp-4h]

  if ( msg->readcount >= msg->splitSize + msg->cursize )
  {
    msg->overflowed = 1;
    return -1;
  }
  else
  {
    c = MSG_GetByte(msg, msg->readcount);
    if ( c != (unsigned __int8)c
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
            811,
            0,
            "c == static_cast< byte >( c )\n\t%i, %i",
            c,
            (unsigned __int8)c) )
    {
      __debugbreak();
    }
    ++msg->readcount;
    return c;
  }
}

int __cdecl MSG_ReadShort(msg_t *msg)
{
  int where; // [esp+0h] [ebp-14h]
  int i; // [esp+4h] [ebp-10h]
  int c; // [esp+8h] [ebp-Ch]
  int newcount; // [esp+Ch] [ebp-8h]
  __int16 read[2]; // [esp+10h] [ebp-4h]

  newcount = msg->readcount + 2;
  if ( newcount > msg->splitSize + msg->cursize )
  {
    msg->overflowed = 1;
    return -1;
  }
  else
  {
    where = msg->readcount;
    for ( i = 0; i < 2; ++i )
      *((_BYTE *)read + i) = MSG_GetByte(msg, where++);
    c = read[0];
    msg->readcount = newcount;
    return c;
  }
}

int __cdecl MSG_ReadLong(msg_t *msg)
{
  int where; // [esp+0h] [ebp-10h]
  int i; // [esp+4h] [ebp-Ch]
  int c; // [esp+8h] [ebp-8h]
  int newcount; // [esp+Ch] [ebp-4h]

  newcount = msg->readcount + 4;
  if ( newcount > msg->splitSize + msg->cursize )
  {
    msg->overflowed = 1;
    return -1;
  }
  else
  {
    where = msg->readcount;
    for ( i = 0; i < 4; ++i )
      *((_BYTE *)&c + i) = MSG_GetByte(msg, where++);
    msg->readcount = newcount;
    return c;
  }
}

unsigned __int64 __cdecl MSG_ReadInt64(msg_t *msg)
{
  int where; // [esp+0h] [ebp-18h]
  int i; // [esp+4h] [ebp-14h]
  unsigned __int64 c; // [esp+8h] [ebp-10h]
  int newcount; // [esp+14h] [ebp-4h]

  newcount = msg->readcount + 8;
  if ( newcount > msg->splitSize + msg->cursize )
  {
    msg->overflowed = 1;
    return 0;
  }
  else
  {
    where = msg->readcount;
    for ( i = 0; i < 8; ++i )
      *((_BYTE *)&c + i) = MSG_GetByte(msg, where++);
    c = LittleLong64(c);
    msg->readcount = newcount;
    return c;
  }
}

double __cdecl MSG_ReadFloat(msg_t *msg)
{
  MSG_ReadFloat::__l2::<unnamed_type_dat> dat; // [esp+0h] [ebp-4h]

  dat.f = COERCE_FLOAT(MSG_ReadLong(msg));
  if ( dat.f != NAN )
    return dat.f;
  msg->overflowed = 1;
  return -1.0;
}

char *__cdecl MSG_ReadString(msg_t *msg, char *string, unsigned int maxChars)
{
  int c; // [esp+0h] [ebp-8h]
  unsigned int l; // [esp+4h] [ebp-4h]

  if ( !string && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 894, 0, "%s", "string") )
    __debugbreak();
  for ( l = 0; ; ++l )
  {
    c = MSG_ReadByte(msg);
    if ( c == -1 )
      c = 0;
    if ( l < maxChars )
      string[l] = I_CleanChar(c);
    if ( !c )
      break;
  }
  string[maxChars - 1] = 0;
  return string;
}

char *__cdecl MSG_ReadBigString(msg_t *msg)
{
  int c; // [esp+0h] [ebp-8h]
  unsigned int l; // [esp+4h] [ebp-4h]

  if ( !Sys_IsMainThread()
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 921, 0, "%s", "Sys_IsMainThread()") )
  {
    __debugbreak();
  }
  for ( l = 0; ; ++l )
  {
    c = MSG_ReadByte(msg);
    if ( c == 37 )
    {
      c = 46;
    }
    else if ( c == -1 )
    {
      c = 0;
    }
    if ( l < 0x4000 )
      string[l] = I_CleanChar(c);
    if ( !c )
      break;
  }
  byte_9763FCF = 0;
  return string;
}

char *__cdecl MSG_ReadStringLine(msg_t *msg, char *string, unsigned int maxChars)
{
  int c; // [esp+0h] [ebp-8h]
  unsigned int l; // [esp+4h] [ebp-4h]

  for ( l = 0; ; ++l )
  {
    c = MSG_ReadByte(msg);
    if ( c == 37 )
    {
      c = 46;
    }
    else if ( c == 10 || c == -1 )
    {
      c = 0;
    }
    if ( l < maxChars )
      string[l] = I_CleanChar(c);
    if ( !c )
      break;
  }
  string[maxChars - 1] = 0;
  return string;
}

double __cdecl MSG_ReadAngle(msg_t *msg, unsigned int bits)
{
  return (double)MSG_ReadBits(msg, bits) * (360.0 / (double)(1 << bits));
}

double __cdecl MSG_ReadAngle16(msg_t *msg)
{
  return (double)MSG_ReadShort(msg) * 0.0054931641;
}

double __cdecl MSG_ReadRangedFloatBits(msg_t *sb, float begin, float end, unsigned int bits)
{
  float fa; // [esp+10h] [ebp-8h]
  float f; // [esp+10h] [ebp-8h]

  fa = (double)(unsigned int)MSG_ReadBits(sb, bits) * ((float)(end - begin) / (float)((1 << bits) - 1));
  f = fa + begin;
  if ( f < begin
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 999, 0, "%s", "f >= begin") )
  {
    __debugbreak();
  }
  if ( end < f && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 1000, 0, "%s", "f <= end") )
    __debugbreak();
  return f;
}

void __cdecl MSG_ReadData(msg_t *msg, unsigned __int8 *data, int len)
{
  int newcount; // [esp+0h] [ebp-8h]
  signed int cursize; // [esp+4h] [ebp-4h]

  newcount = len + msg->readcount;
  if ( newcount > msg->cursize )
  {
    if ( newcount > msg->splitSize + msg->cursize )
    {
      msg->overflowed = 1;
      memset(data, 0xFFu, len);
    }
    else
    {
      cursize = msg->cursize - msg->readcount;
      if ( cursize > 0 )
      {
        memcpy(data, &msg->data[msg->readcount], cursize);
        len -= cursize;
        data += cursize;
      }
      if ( len > 0 )
        memcpy(data, &msg->splitData[msg->readcount - msg->cursize], len);
      msg->readcount = newcount;
    }
  }
  else
  {
    memcpy(data, &msg->data[msg->readcount], len);
    msg->readcount = newcount;
  }
}

void __cdecl MSG_WriteDeltaKey(msg_t *msg, int key, int oldV, int newV, unsigned int bits)
{
  if ( msg->readOnly
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 1059, 0, "%s", "!msg->readOnly") )
  {
    __debugbreak();
  }
  if ( oldV == newV )
  {
    MSG_WriteBit0(msg);
  }
  else
  {
    MSG_WriteBit1(msg);
    MSG_WriteBits(msg, key ^ newV, bits);
  }
}

unsigned int __cdecl MSG_ReadDeltaKey(msg_t *msg, int key, int oldV, unsigned int bits)
{
  if ( MSG_ReadBit(msg) )
    return kbitmask[bits] & key ^ MSG_ReadBits(msg, bits);
  else
    return oldV;
}

void __cdecl MSG_WriteKey(msg_t *msg, int key, int newV, unsigned int bits)
{
  if ( msg->readOnly
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 1082, 0, "%s", "!msg->readOnly") )
  {
    __debugbreak();
  }
  MSG_WriteBits(msg, key ^ newV, bits);
}

unsigned int __cdecl MSG_ReadKey(msg_t *msg, int key, unsigned int bits)
{
  return kbitmask[bits] & key ^ MSG_ReadBits(msg, bits);
}

void __cdecl MSG_WriteDeltaKeyByte(msg_t *msg, char key, char oldV, char newV)
{
  if ( msg->readOnly
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 1095, 0, "%s", "!msg->readOnly") )
  {
    __debugbreak();
  }
  if ( oldV == newV )
  {
    MSG_WriteBit0(msg);
  }
  else
  {
    MSG_WriteBit1(msg);
    MSG_WriteByte(msg, key ^ newV);
  }
}

int __cdecl MSG_ReadDeltaKeyByte(msg_t *msg, unsigned __int8 key, int oldV)
{
  if ( MSG_ReadBit(msg) )
    return key ^ (unsigned __int8)MSG_ReadByte(msg);
  else
    return oldV;
}

void __cdecl MSG_WriteDeltaKeyShort(msg_t *msg, __int16 key, __int16 oldV, __int16 newV)
{
  if ( msg->readOnly
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 1117, 0, "%s", "!msg->readOnly") )
  {
    __debugbreak();
  }
  if ( oldV == newV )
  {
    MSG_WriteBit0(msg);
  }
  else
  {
    MSG_WriteBit1(msg);
    MSG_WriteShort(msg, key ^ newV);
  }
}

int __cdecl MSG_ReadDeltaKeyShort(msg_t *msg, __int16 key, int oldV)
{
  if ( MSG_ReadBit(msg) )
    return key ^ (unsigned __int16)MSG_ReadShort(msg);
  else
    return oldV;
}

void __cdecl MSG_SetDefaultUserCmd(playerState_s *ps, usercmd_s *cmd)
{
  int i; // [esp+4h] [ebp-4h]

  memset((unsigned __int8 *)cmd, 0, sizeof(usercmd_s));
  cmd->weapon = ps->weapon;
  cmd->offHandIndex = ps->offHandIndex;
  for ( i = 0; i < 2; ++i )
    cmd->angles[i] = (unsigned __int16)(int)(float)((float)(ps->viewangles[i] - ps->delta_angles[i]) * 182.04445);
  if ( (ps->otherFlags & 4) != 0 )
  {
    if ( (ps->eFlags & 8) != 0 )
    {
      bitarray<51>::setBit(&cmd->button_bits, 8u);
    }
    else if ( (ps->eFlags & 4) != 0 )
    {
      bitarray<51>::setBit(&cmd->button_bits, 9u);
    }
    if ( ps->leanf <= 0.0 )
    {
      if ( ps->leanf < 0.0 )
        bitarray<51>::setBit(&cmd->button_bits, 6u);
    }
    else
    {
      bitarray<51>::setBit(&cmd->button_bits, 7u);
    }
    if ( ps->fWeaponPosFrac != 0.0 )
      bitarray<51>::setBit(&cmd->button_bits, 0xBu);
    if ( (ps->pm_flags & 0x8000) != 0 )
      bitarray<51>::setBit(&cmd->button_bits, 1u);
  }
}

void __cdecl MSG_WriteDeltaUsercmdKey(
        msg_t *msg,
        int key,
        const usercmd_s *from,
        const usercmd_s *to,
        int forceSendAngles)
{
  bool v5; // esi
  bool v6; // al
  bool v7; // al
  bool v8; // [esp+4h] [ebp-2Ch]
  bool v9; // [esp+8h] [ebp-28h]
  int i; // [esp+18h] [ebp-18h]
  bool noButtonChange; // [esp+1Eh] [ebp-12h]
  bitarray<51> ignoreMask; // [esp+20h] [ebp-10h] BYREF
  unsigned int delta; // [esp+28h] [ebp-8h]
  bool buttonsAllEqual; // [esp+2Eh] [ebp-2h]
  bool noAngleChange; // [esp+2Fh] [ebp-1h]
  int keyb; // [esp+3Ch] [ebp+Ch]
  int keya; // [esp+3Ch] [ebp+Ch]

  if ( msg->readOnly
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 1255, 0, "%s", "!msg->readOnly") )
  {
    __debugbreak();
  }
  delta = to->serverTime - from->serverTime;
  if ( delta >= 0x100 )
  {
    MSG_WriteBit0(msg);
    MSG_WriteLong(msg, to->serverTime);
  }
  else
  {
    MSG_WriteBit1(msg);
    MSG_WriteByte(msg, delta);
  }
  v9 = from->forwardmove == to->forwardmove && from->rightmove == to->rightmove;
  v8 = forceSendAngles && from->yawmove == to->yawmove && from->pitchmove == to->pitchmove;
  noAngleChange = v8;
  bitarray<51>::bitarray<51>(&ignoreMask, 0, -1);
  buttonsAllEqual = bitarray<51>::areAllBitsEqual(&from->button_bits, &to->button_bits, &ignoreMask);
  if ( buttonsAllEqual
    && from->weapon == to->weapon
    && from->offHandIndex == to->offHandIndex
    && from->angles[2] == to->angles[2]
    && from->meleeChargeYaw == to->meleeChargeYaw
    && from->meleeChargeDist == to->meleeChargeDist )
  {
    v5 = bitarray<51>::testBit(&from->button_bits, 0);
    noButtonChange = v5 == bitarray<51>::testBit(&to->button_bits, 0);
    if ( from->angles[0] == to->angles[0] && from->angles[1] == to->angles[1] && noButtonChange && v9 && noAngleChange )
    {
      MSG_WriteKey(msg, key, 0, 1u);
    }
    else
    {
      MSG_WriteKey(msg, key, 1, 1u);
      MSG_WriteKey(msg, key, 0, 1u);
      keyb = to->serverTime ^ key;
      v6 = bitarray<51>::testBit(&to->button_bits, 0);
      MSG_WriteKey(msg, keyb, v6, 1u);
      MSG_WriteDeltaKeyShort(msg, keyb, from->angles[0], to->angles[0]);
      MSG_WriteDeltaKeyShort(msg, keyb, from->angles[1], to->angles[1]);
      MSG_WriteDeltaKeyByte(msg, keyb, from->forwardmove, to->forwardmove);
      MSG_WriteDeltaKeyByte(msg, keyb, from->rightmove, to->rightmove);
      MSG_WriteDeltaKeyByte(msg, keyb, from->pitchmove, to->pitchmove);
      MSG_WriteDeltaKeyByte(msg, keyb, from->yawmove, to->yawmove);
    }
  }
  else
  {
    MSG_WriteKey(msg, key, 1, 1u);
    MSG_WriteKey(msg, key, 1, 1u);
    v7 = bitarray<51>::testBit(&to->button_bits, 0);
    MSG_WriteKey(msg, key, v7, 1u);
    MSG_WriteDeltaKeyShort(msg, key, from->angles[0], to->angles[0]);
    MSG_WriteDeltaKeyShort(msg, key, from->angles[1], to->angles[1]);
    MSG_WriteDeltaKeyByte(msg, key, from->forwardmove, to->forwardmove);
    MSG_WriteDeltaKeyByte(msg, key, from->rightmove, to->rightmove);
    MSG_WriteDeltaKeyByte(msg, key, from->pitchmove, to->pitchmove);
    MSG_WriteDeltaKeyByte(msg, key, from->yawmove, to->yawmove);
    keya = to->serverTime ^ key;
    MSG_WriteDeltaKeyShort(msg, keya, from->angles[2], to->angles[2]);
    for ( i = 0; i < 2; ++i )
      MSG_WriteDeltaKey(msg, keya, from->button_bits.array[i], to->button_bits.array[i], 0x20u);
    MSG_WriteDeltaKey(msg, keya, from->weapon, to->weapon, 0xBu);
    MSG_WriteDeltaKey(msg, keya, from->offHandIndex, to->offHandIndex, 0xBu);
    MSG_WriteDeltaKey(msg, keya, from->lastWeaponAltModeSwitch, to->lastWeaponAltModeSwitch, 0xBu);
    if ( bitarray<51>::testBit(&to->button_bits, 0x10u) )
    {
      MSG_WriteDeltaKeyByte(msg, keya, from->selectedLocation[0], to->selectedLocation[0]);
      MSG_WriteDeltaKeyByte(msg, keya, from->selectedLocation[1], to->selectedLocation[1]);
      MSG_WriteDeltaKeyByte(msg, keya, from->selectedYaw, to->selectedYaw);
    }
    if ( bitarray<51>::testBit(&to->button_bits, 2u) )
    {
      MSG_WriteDeltaKeyShort(
        msg,
        keya,
        (int)(float)(from->meleeChargeYaw * 182.04445),
        (int)(float)(to->meleeChargeYaw * 182.04445));
      MSG_WriteDeltaKey(msg, keya, from->meleeChargeDist, to->meleeChargeDist, 8u);
    }
  }
}

void __cdecl MSG_ReadDeltaUsercmdKey(msg_t *msg, int key, const usercmd_s *from, usercmd_s *to)
{
  int i; // [esp+8h] [ebp-Ch]
  unsigned int set_attack; // [esp+10h] [ebp-4h]
  int keya; // [esp+20h] [ebp+Ch]
  int keyb; // [esp+20h] [ebp+Ch]

  memcpy(to, from, sizeof(usercmd_s));
  if ( MSG_ReadBit(msg) )
    to->serverTime = from->serverTime + MSG_ReadByte(msg);
  else
    to->serverTime = MSG_ReadLong(msg);
  if ( MSG_ReadKey(msg, key, 1u) )
  {
    bitarray<51>::resetBit(&to->button_bits, 0);
    if ( MSG_ReadKey(msg, key, 1u) )
    {
      set_attack = MSG_ReadKey(msg, key, 1u);
      to->angles[0] = (unsigned __int16)MSG_ReadDeltaKeyShort(msg, key, from->angles[0]);
      to->angles[1] = (unsigned __int16)MSG_ReadDeltaKeyShort(msg, key, from->angles[1]);
      to->forwardmove = MSG_ReadDeltaKeyByte(msg, key, from->forwardmove);
      to->rightmove = MSG_ReadDeltaKeyByte(msg, key, from->rightmove);
      to->pitchmove = MSG_ReadDeltaKeyByte(msg, key, from->pitchmove);
      to->yawmove = MSG_ReadDeltaKeyByte(msg, key, from->yawmove);
      keyb = to->serverTime ^ key;
      to->angles[2] = (unsigned __int16)MSG_ReadDeltaKeyShort(msg, keyb, from->angles[2]);
      for ( i = 0; i < 2; ++i )
        to->button_bits.array[i] = MSG_ReadDeltaKey(msg, keyb, from->button_bits.array[i], 0x20u);
      if ( set_attack )
        bitarray<51>::setBit(&to->button_bits, 0);
      to->weapon = MSG_ReadDeltaKey(msg, keyb, from->weapon, 0xBu);
      to->offHandIndex = MSG_ReadDeltaKey(msg, keyb, from->offHandIndex, 0xBu);
      to->lastWeaponAltModeSwitch = MSG_ReadDeltaKey(msg, keyb, from->lastWeaponAltModeSwitch, 0xBu);
      if ( bitarray<51>::testBit(&to->button_bits, 0x10u) )
      {
        to->selectedLocation[0] = MSG_ReadDeltaKeyByte(msg, keyb, from->selectedLocation[0]);
        to->selectedLocation[1] = MSG_ReadDeltaKeyByte(msg, keyb, from->selectedLocation[1]);
        to->selectedYaw = MSG_ReadDeltaKeyByte(msg, keyb, from->selectedYaw);
      }
      if ( bitarray<51>::testBit(&to->button_bits, 2u) )
      {
        to->meleeChargeYaw = (float)MSG_ReadDeltaKeyShort(
                                      msg,
                                      keyb,
                                      (unsigned __int16)(int)(float)(from->meleeChargeYaw * 182.04445))
                           * 0.0054931641;
        to->meleeChargeDist = MSG_ReadDeltaKey(msg, keyb, from->meleeChargeDist, 8u);
      }
    }
    else
    {
      keya = to->serverTime ^ key;
      if ( MSG_ReadKey(msg, keya, 1u) )
        bitarray<51>::setBit(&to->button_bits, 0);
      to->angles[0] = (unsigned __int16)MSG_ReadDeltaKeyShort(msg, keya, from->angles[0]);
      to->angles[1] = (unsigned __int16)MSG_ReadDeltaKeyShort(msg, keya, from->angles[1]);
      to->forwardmove = MSG_ReadDeltaKeyByte(msg, keya, from->forwardmove);
      to->rightmove = MSG_ReadDeltaKeyByte(msg, keya, from->rightmove);
      to->pitchmove = MSG_ReadDeltaKeyByte(msg, keya, from->pitchmove);
      to->yawmove = MSG_ReadDeltaKeyByte(msg, keya, from->yawmove);
    }
  }
}

void __cdecl MSG_ClearLastReferencedEntity(msg_t *msg)
{
  msg->lastEntityRef = -1;
}

double __cdecl MSG_ReadDemoRoundedFloat(
        msg_t *msg,
        int bits,
        float oldValue,
        unsigned int diffBits,
        unsigned int fullBits)
{
  int fullBias; // [esp+4h] [ebp-20h]
  int roundedCenter; // [esp+Ch] [ebp-18h]
  int index; // [esp+10h] [ebp-14h]

  fullBias = 1 << (fullBits - 1);
  if ( MSG_ReadBit(msg) )
  {
    if ( bits == -103 || bits == -102 || bits == -101 )
    {
      if ( bits == -103 )
      {
        index = 0;
      }
      else if ( bits == -102 )
      {
        index = 1;
      }
      else
      {
        index = 2;
      }
      roundedCenter = (int)(float)((*MSG_GetMapCenter())[index] + 0.5);
      return (float)(roundedCenter
                   + (((int)oldValue + fullBias - roundedCenter) ^ MSG_ReadBits(msg, fullBits))
                   - fullBias);
    }
    else
    {
      return (float)(((fullBias + (int)oldValue) ^ MSG_ReadBits(msg, fullBits)) - fullBias);
    }
  }
  else
  {
    return (float)((float)(MSG_ReadBits(msg, diffBits) - (1 << (diffBits - 1))) + oldValue);
  }
}

double __cdecl MSG_ReadRangedFloat(
        msg_t *msg,
        float oldValue,
        float begin,
        float end,
        int precision,
        bool oftenZeroOrOne)
{
  float v7; // [esp+18h] [ebp-3Ch]
  float v8; // [esp+2Ch] [ebp-28h]
  float v9; // [esp+30h] [ebp-24h]
  unsigned int rangeSize; // [esp+38h] [ebp-1Ch]
  int rangeBits; // [esp+40h] [ebp-14h]
  float oldVal; // [esp+44h] [ebp-10h]
  float range; // [esp+4Ch] [ebp-8h]
  int f; // [esp+50h] [ebp-4h]

  if ( end < begin
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
          1748,
          0,
          "%s\n\t%s",
          "end >= begin",
          "MSG_ReadRangedFloat, end of range is less then the begining.") )
  {
    __debugbreak();
  }
  if ( precision <= 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
          1749,
          0,
          "%s\n\t%s",
          "precision > 0",
          "MSG_ReadRangedFloat, precision must be positive.") )
  {
    __debugbreak();
  }
  range = (float)(end - begin) * (float)precision;
  if ( range <= 4294967300.0 )
  {
    if ( oftenZeroOrOne && MSG_ReadBit(msg) )
    {
      if ( MSG_ReadBit(msg) )
        v8 = FLOAT_1_0;
      else
        v8 = *(float *)&FLOAT_0_0;
      *(float *)&f = v8;
    }
    else
    {
      rangeSize = (__int64)range;
      rangeBits = 0;
      if ( rangeSize )
        rangeBits = GetMinBitCountForNum(rangeSize);
      if ( (float)(oldValue - end) < 0.0 )
        v9 = oldValue;
      else
        v9 = end;
      if ( (float)(begin - oldValue) < 0.0 )
        v7 = v9;
      else
        v7 = begin;
      oldVal = (float)((float)(v7 - begin) * (float)precision) + 0.5;
      if ( oldVal > range )
        oldVal = (float)(end - begin) * (float)precision;
      *(float *)&f = (double)(MSG_ReadBits(msg, rangeBits) ^ (unsigned int)(__int64)oldVal) / (double)precision + begin;
    }
  }
  else
  {
    Com_PrintWarning(
      15,
      "The numerical space defined by range/precision combination is too large. No compression performed.");
    f = LODWORD(oldValue) ^ MSG_ReadLong(msg);
  }
  return *(float *)&f;
}

int __cdecl MSG_ReadEntityIndex(msg_t *msg, unsigned int indexBits)
{
  if ( MSG_ReadBit(msg) )
  {
    if ( msg_printEntityNums->current.enabled )
      Com_Printf(16, "Entity num: 1 bit (inc)\n");
    ++msg->lastEntityRef;
  }
  else if ( indexBits != 10 || MSG_ReadBit(msg) )
  {
    if ( msg_printEntityNums->current.enabled )
      Com_Printf(16, "Entity num: %i bits (full)\n", indexBits + 2);
    msg->lastEntityRef = MSG_ReadBits(msg, indexBits);
  }
  else
  {
    if ( msg_printEntityNums->current.enabled )
      Com_Printf(16, "Entity num: %i bits (delta)\n", 6);
    msg->lastEntityRef += MSG_ReadBits(msg, 4u);
  }
  if ( msg_printEntityNums->current.enabled )
    Com_Printf(16, "Read entity num %i\n", msg->lastEntityRef);
  if ( msg->lastEntityRef < 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
          1828,
          0,
          "%s\n\t(msg->lastEntityRef) = %i",
          "(msg->lastEntityRef >= 0)",
          msg->lastEntityRef) )
  {
    __debugbreak();
  }
  return msg->lastEntityRef;
}

void __cdecl MSG_ReadDeltaField(
        msg_t *msg,
        int time,
        char *from,
        char *to,
        const NetField *field,
        int print,
        bool noXor)
{
  int Bit; // eax
  int Byte; // eax
  int Long; // eax
  int v10; // eax
  int v11; // eax
  int v12; // eax
  int v13; // eax
  int v14; // eax
  double RangedFloat; // st7
  double v16; // st7
  double v17; // st7
  double v18; // st7
  double v19; // st7
  int EFlags; // eax
  int DeltaTime; // eax
  unsigned int MinBitCountForNum; // eax
  int Bits; // esi
  int v24; // eax
  double OriginFloat; // st7
  double OriginZFloat; // st7
  double v27; // st7
  double v28; // st7
  double DemoRoundedFloat; // st7
  double v30; // st7
  double Angle; // st7
  double DeltaAngle; // st7
  double Angle16; // st7
  int v34; // eax
  int v35; // [esp+18h] [ebp-D4h]
  int v36; // [esp+1Ch] [ebp-D0h]
  int v37; // [esp+20h] [ebp-CCh]
  int v38; // [esp+24h] [ebp-C8h]
  int v39; // [esp+28h] [ebp-C4h]
  int v40; // [esp+2Ch] [ebp-C0h]
  int v41; // [esp+30h] [ebp-BCh]
  int v42; // [esp+34h] [ebp-B8h]
  int v43; // [esp+38h] [ebp-B4h]
  int size; // [esp+3Ch] [ebp-B0h]
  float v45; // [esp+40h] [ebp-ACh]
  float v46; // [esp+44h] [ebp-A8h]
  float v47; // [esp+48h] [ebp-A4h]
  int v48; // [esp+5Ch] [ebp-90h]
  int v49; // [esp+68h] [ebp-84h]
  int v50; // [esp+70h] [ebp-7Ch]
  int ValueNoXor; // [esp+78h] [ebp-74h]
  int v52; // [esp+80h] [ebp-6Ch]
  int v53; // [esp+88h] [ebp-64h]
  int DeltaEventParamField; // [esp+90h] [ebp-5Ch]
  int DeltaEventField; // [esp+98h] [ebp-54h]
  int DeltaGroundEntity; // [esp+A0h] [ebp-4Ch]
  float v57; // [esp+A8h] [ebp-44h]
  float v58; // [esp+B0h] [ebp-3Ch]
  float v59; // [esp+B8h] [ebp-34h]
  const hudelem_color_t *fromColor; // [esp+C8h] [ebp-24h]
  hudelem_color_t *toColora; // [esp+CCh] [ebp-20h]
  hudelem_color_t *toColor; // [esp+CCh] [ebp-20h]
  int delta; // [esp+D0h] [ebp-1Ch]
  int zeroVal; // [esp+D8h] [ebp-14h] BYREF
  int trunc; // [esp+DCh] [ebp-10h]
  int *toF; // [esp+E0h] [ebp-Ch]
  const int *fromF; // [esp+E4h] [ebp-8h]
  int value; // [esp+E8h] [ebp-4h]

  zeroVal = 0;
  if ( noXor )
    fromF = &zeroVal;
  else
    fromF = (const int *)&from[field->offset];
  toF = (int *)&to[field->offset];
  if ( msg->overflowed
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 1895, 0, "%s", "!msg->overflowed") )
  {
    __debugbreak();
  }
  if ( msg->overflowed
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 1922, 0, "%s", "!msg->overflowed") )
  {
    __debugbreak();
  }
  switch ( field->bits )
  {
    case 0:
      if ( MSG_ReadBit(msg) )
      {
        if ( MSG_ReadBit(msg) )
        {
          Long = MSG_ReadLong(msg);
          *toF = Long;
          *toF ^= *fromF;
          if ( print )
            Com_Printf(16, "%s:%f ", field->name, *(float *)toF);
        }
        else
        {
          trunc = MSG_ReadBits(msg, 5u);
          Byte = MSG_ReadByte(msg);
          trunc += 32 * Byte;
          trunc ^= (int)*(float *)fromF + 4096;
          trunc -= 4096;
          *(float *)toF = (float)trunc;
          if ( print )
            Com_Printf(16, "%s:%i ", field->name, trunc);
        }
      }
      else
      {
        Bit = MSG_ReadBit(msg);
        *toF = Bit << 31;
        if ( *(float *)toF != 0.0
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
                1929,
                0,
                "%s",
                "*reinterpret_cast< float * >( toF ) == 0.0f") )
        {
          __debugbreak();
        }
      }
      return;
    case 0xFFFFFFC1:
    case 0xFFFFFFC3:
LABEL_25:
      v10 = MSG_ReadLong(msg);
      *toF = v10;
      return;
    case 0xFFFFFFA7:
      if ( MSG_ReadBit(msg) )
      {
        v12 = MSG_ReadLong(msg);
        *toF = v12;
        *toF ^= *fromF;
        if ( print )
          Com_Printf(16, "%s:%f ", field->name, *(float *)toF);
      }
      else
      {
        trunc = MSG_ReadBits(msg, 5u);
        v11 = MSG_ReadByte(msg);
        trunc += 32 * v11;
        trunc ^= (int)*(float *)fromF + 4096;
        trunc -= 4096;
        *(float *)toF = (float)trunc;
        if ( print )
          Com_Printf(16, "%s:%i ", field->name, trunc);
      }
      break;
    case 0xFFFFFFA8:
      v13 = MSG_ReadLong(msg);
      *toF = v13;
      *toF ^= *fromF;
      if ( print )
        Com_Printf(16, "%s:%f ", field->name, *(float *)toF);
      break;
    case 0xFFFFFF9D:
      if ( MSG_ReadBit(msg) )
      {
        if ( MSG_ReadBit(msg) )
        {
          value = MSG_ReadLong(msg);
          value ^= *fromF;
          *toF = value;
          if ( print )
            Com_Printf(16, "%s:%f ", field->name, *(float *)toF);
        }
        else
        {
          trunc = MSG_ReadBits(msg, 4u);
          v14 = MSG_ReadByte(msg);
          trunc += 16 * v14;
          trunc ^= (int)*(float *)fromF + 2048;
          trunc -= 2048;
          *(float *)toF = (float)trunc;
          if ( print )
            Com_Printf(16, "%s:%i ", field->name, trunc);
        }
      }
      else
      {
        *toF = 0;
        if ( *(float *)toF != 0.0
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
                2008,
                0,
                "%s",
                "*reinterpret_cast< float * >( toF ) == 0.0f") )
        {
          __debugbreak();
        }
      }
      if ( (unsigned int)(__int64)(*(float *)toF + 2048.0) >= 0x1000
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
              2040,
              0,
              "*(float *)toF + HUDELEM_COORD_BIAS doesn't index 1 << HUDELEM_COORD_BITS\n\t%i not in [0, %i)",
              (int)(float)(*(float *)toF + 2048.0),
              4096) )
      {
        __debugbreak();
      }
      break;
    case 0xFFFFFFAF:
      RangedFloat = MSG_ReadRangedFloat(msg, *(float *)fromF, 0.0, 1.0, 100, 0);
      *(float *)toF = RangedFloat;
      return;
    case 0xFFFFFFB0:
      v16 = MSG_ReadRangedFloat(msg, *(float *)fromF, 0.0, 1.0, 1000, 0);
      *(float *)toF = v16;
      return;
    case 0xFFFFFFB3:
      v17 = MSG_ReadRangedFloat(msg, *(float *)fromF, -1.0, 1.0, 100, 0);
      *(float *)toF = v17;
      return;
    case 0xFFFFFFB4:
      v59 = MSG_ReadRangedFloat(msg, (float)*(__int16 *)fromF * 0.000030517578, -1.0, 1.0, 10, 0);
      v47 = floor(v59 * 32768.0 + 0.5);
      *(_WORD *)toF = (int)v47;
      return;
    case 0xFFFFFFB5:
      v58 = MSG_ReadRangedFloat(msg, (float)*(__int16 *)fromF * 0.0054931641, -90.0, 90.0, 10, 0);
      v46 = floor(v58 * 182.04445 + 0.5);
      *(_WORD *)toF = (int)v46;
      return;
    case 0xFFFFFFB6:
      v57 = MSG_ReadRangedFloat(msg, (float)*(__int16 *)fromF * 0.0054931641, -180.0, 180.0, 10, 0);
      v45 = floor(v57 * 182.04445 + 0.5);
      *(_WORD *)toF = (int)v45;
      return;
    case 0xFFFFFFB7:
      v18 = MSG_ReadRangedFloat(msg, *(float *)fromF, -12.0, 12.0, 10, 0);
      *(float *)toF = v18;
      return;
    case 0xFFFFFFB8:
      v19 = MSG_ReadRangedFloat(msg, *(float *)fromF, -6000.0, 6000.0, 10, 0);
      *(float *)toF = v19;
      return;
    case 0xFFFFFF9E:
      EFlags = MSG_ReadEFlags(msg, *fromF);
      *toF = EFlags;
      return;
    case 0xFFFFFF9F:
    case 0xFFFFFFB9:
    case 0xFFFFFFBA:
    case 0xFFFFFFBC:
      DeltaTime = MSG_ReadDeltaTime(msg, time);
      *toF = DeltaTime;
      return;
    case 0xFFFFFFBB:
    case 0xFFFFFFBD:
      goto LABEL_25;
    case 0xFFFFFFA0:
      DeltaGroundEntity = MSG_ReadDeltaGroundEntity(msg);
      size = field->size;
      switch ( size )
      {
        case 1:
          *(_BYTE *)toF = DeltaGroundEntity;
          break;
        case 2:
          *(_WORD *)toF = DeltaGroundEntity;
          break;
        case 4:
          *toF = DeltaGroundEntity;
          break;
      }
      break;
    case 0xFFFFFFA2:
      DeltaEventField = MSG_ReadDeltaEventField(msg);
      v43 = field->size;
      switch ( v43 )
      {
        case 1:
          *(_BYTE *)toF = DeltaEventField;
          break;
        case 2:
          *(_WORD *)toF = DeltaEventField;
          break;
        case 4:
          *toF = DeltaEventField;
          break;
      }
      break;
    case 0xFFFFFFA3:
      DeltaEventParamField = MSG_ReadDeltaEventParamField(msg);
      v42 = field->size;
      switch ( v42 )
      {
        case 1:
          *(_BYTE *)toF = DeltaEventParamField;
          break;
        case 2:
          *(_WORD *)toF = DeltaEventParamField;
          break;
        case 4:
          *toF = DeltaEventParamField;
          break;
      }
      break;
    case 0xFFFFFFC5:
      if ( MSG_ReadBit(msg) == 1 )
      {
        MinBitCountForNum = GetMinBitCountForNum(0x10u);
        Bits = MSG_ReadBits(msg, MinBitCountForNum);
        v53 = MSG_GetField(fromF, field->size) + Bits + 1;
        v41 = field->size;
        switch ( v41 )
        {
          case 1:
            *(_BYTE *)toF = v53;
            break;
          case 2:
            *(_WORD *)toF = v53;
            break;
          case 4:
            *toF = v53;
            break;
        }
      }
      else
      {
        v52 = MSG_ReadValue(msg, fromF, toF, 8, field->size);
        v40 = field->size;
        switch ( v40 )
        {
          case 1:
            *(_BYTE *)toF = v52;
            break;
          case 2:
            *(_WORD *)toF = v52;
            break;
          case 4:
            *toF = v52;
            break;
        }
      }
      break;
    case 0xFFFFFFA1:
      v24 = MSG_ReadBits(msg, 7u);
      *toF = 100 * v24;
      break;
    case 0xFFFFFFC6:
      ValueNoXor = MSG_ReadValueNoXor(msg, 8);
      v39 = field->size;
      switch ( v39 )
      {
        case 1:
          *(_BYTE *)toF = ValueNoXor;
          break;
        case 2:
          *(_WORD *)toF = ValueNoXor;
          break;
        case 4:
          *toF = ValueNoXor;
          break;
      }
      break;
    case 0xFFFFFFA4:
    case 0xFFFFFFA5:
    case 0xFFFFFFBE:
    case 0xFFFFFFBF:
      OriginFloat = MSG_ReadOriginFloat(field->bits, msg, *(float *)fromF);
      *(float *)toF = OriginFloat;
      if ( print )
        Com_Printf(16, "%s:%f ", field->name, *(float *)toF);
      break;
    case 0xFFFFFFA6:
    case 0xFFFFFFC0:
      OriginZFloat = MSG_ReadOriginZFloat(msg, *(float *)fromF);
      *(float *)toF = OriginZFloat;
      if ( print )
        Com_Printf(16, "%s:%f ", field->name, *(float *)toF);
      break;
    case 0xFFFFFF96:
      v27 = MSG_ReadRangedFloat(msg, *(float *)fromF, 0.0, 255.0, 100, 0);
      *(float *)toF = v27;
      break;
    case 0xFFFFFF95:
      v28 = MSG_ReadRangedFloat(msg, *(float *)fromF, 0.0, 60.0, 100, 0);
      *(float *)toF = v28;
      break;
    case 0xFFFFFF99:
    case 0xFFFFFF9A:
    case 0xFFFFFF9B:
      DemoRoundedFloat = MSG_ReadDemoRoundedFloat(msg, field->bits, *(float *)fromF, 5u, 0x10u);
      *(float *)toF = DemoRoundedFloat;
      if ( print )
        Com_Printf(16, "%s:%f ", field->name, *(float *)toF);
      break;
    case 0xFFFFFF98:
      v30 = MSG_ReadDemoRoundedFloat(msg, field->bits, *(float *)fromF, 7u, 0xCu);
      *(float *)toF = v30;
      if ( print )
        Com_Printf(16, "%s:%f ", field->name, *(float *)toF);
      break;
    case 0xFFFFFF97:
      Angle = MSG_ReadAngle(msg, 0xAu);
      *(float *)toF = Angle;
      break;
    case 0xFFFFFF9C:
    case 0xFFFFFFC2:
      DeltaAngle = MSG_ReadDeltaAngle(msg, *(float *)fromF);
      *(float *)toF = DeltaAngle;
      break;
    case 0xFFFFFFA9:
      Angle16 = MSG_ReadAngle16(msg);
      *(float *)toF = Angle16;
      break;
    case 0xFFFFFFC4:
      if ( MSG_ReadBit(msg) == 1 )
      {
        delta = MSG_ReadBits(msg, 5u);
        v50 = MSG_GetField(fromF, field->size) + delta - 16;
        v38 = field->size;
        switch ( v38 )
        {
          case 1:
            *(_BYTE *)toF = v50;
            break;
          case 2:
            *(_WORD *)toF = v50;
            break;
          case 4:
            *toF = v50;
            break;
        }
      }
      else
      {
        v49 = MSG_ReadValue(msg, fromF, toF, -8, field->size);
        v37 = field->size;
        switch ( v37 )
        {
          case 1:
            *(_BYTE *)toF = v49;
            break;
          case 2:
            *(_WORD *)toF = v49;
            break;
          case 4:
            *toF = v49;
            break;
        }
      }
      break;
    case 0xFFFFFFAA:
      v34 = MSG_ReadBits(msg, 6u);
      *(float *)toF = (float)((float)((float)v34 * 1.0) / 10.0) + 1.0;
      break;
    case 0xFFFFFFAB:
      if ( MSG_ReadBit(msg) )
      {
        fromColor = (const hudelem_color_t *)fromF;
        toColora = (hudelem_color_t *)toF;
        *toF = *fromF;
        toColora->a = fromColor->a != 0 ? 0 : -1;
      }
      else
      {
        toColor = (hudelem_color_t *)toF;
        if ( !MSG_ReadBit(msg) )
        {
          toColor->r = MSG_ReadByte(msg);
          toColor->g = MSG_ReadByte(msg);
          toColor->b = MSG_ReadByte(msg);
        }
        toColor->a = 8 * MSG_ReadBits(msg, 5u);
        toColor->a |= ((int)toColor->a >> 5) & 7;
      }
      break;
    default:
      if ( MSG_ReadBit(msg) )
      {
        v48 = MSG_ReadValue(msg, fromF, toF, field->bits, field->size);
        v35 = field->size;
        switch ( v35 )
        {
          case 1:
            *(_BYTE *)toF = v48;
            break;
          case 2:
            *(_WORD *)toF = v48;
            break;
          case 4:
            *toF = v48;
            break;
        }
      }
      else
      {
        v36 = field->size;
        switch ( v36 )
        {
          case 1:
            *(_BYTE *)toF = 0;
            break;
          case 2:
            *(_WORD *)toF = 0;
            break;
          case 4:
            *toF = 0;
            break;
        }
      }
      break;
  }
}

int __cdecl MSG_ReadDeltaTime(msg_t *msg, int timeBase)
{
  if ( MSG_ReadBit(msg) )
    return MSG_ReadLong(msg);
  else
    return timeBase - MSG_ReadBits(msg, 8u);
}

int __cdecl MSG_ReadDeltaGroundEntity(msg_t *msg)
{
  int j; // [esp+4h] [ebp-10h]
  int value; // [esp+10h] [ebp-4h]

  if ( MSG_ReadBit(msg) == 1 )
    return 1022;
  if ( MSG_ReadBit(msg) == 1 )
    return 0;
  value = MSG_ReadBits(msg, 2u);
  for ( j = 2; j < 10; j += 8 )
    value |= MSG_ReadByte(msg) << j;
  return value;
}

int __cdecl MSG_ReadDeltaEventField(msg_t *msg)
{
  return MSG_ReadByte(msg);
}

int __cdecl MSG_ReadDeltaEventParamField(msg_t *msg)
{
  return MSG_ReadBits(msg, 0xBu);
}

int __cdecl MSG_ReadEFlags(msg_t *msg, int oldFlags)
{
  unsigned int bitChanged; // [esp+0h] [ebp-8h]

  if ( msg->overflowed
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 1571, 0, "%s", "!msg->overflowed") )
  {
    __debugbreak();
  }
  if ( MSG_ReadBit(msg) == 1 )
    return MSG_ReadBits(msg, 0x18u);
  bitChanged = MSG_ReadBits(msg, 5u);
  if ( bitChanged >= 0x18
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
          1582,
          0,
          "%s\n\t(bitChanged) = %i",
          "(bitChanged >= 0 && bitChanged < 24)",
          bitChanged) )
  {
    __debugbreak();
  }
  return oldFlags ^ (1 << bitChanged);
}

double __cdecl MSG_ReadOriginFloat(int bits, msg_t *msg, float oldValue)
{
  int roundedCenter; // [esp+24h] [ebp-14h]
  int index; // [esp+28h] [ebp-10h]

  if ( MSG_ReadBit(msg) )
  {
    if ( bits == -92 || bits == -66 )
    {
      index = 0;
    }
    else
    {
      if ( bits != -91
        && bits != -65
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
              1615,
              0,
              "%s",
              "(bits == MSG_FIELD_ORIGINY) || (bits == MSG_FIELD_ES_ORIGINY)") )
      {
        __debugbreak();
      }
      index = 1;
    }
    roundedCenter = (int)((*MSG_GetMapCenter())[index] + 9.313225746154785e-10);
    return (float)(roundedCenter
                 + (((int)(oldValue + 9.313225746154785e-10) + 0x8000 - roundedCenter) ^ MSG_ReadBits(msg, 0x10u))
                 - 0x8000);
  }
  else
  {
    return (float)((float)(MSG_ReadBits(msg, 7u) - 64) + oldValue);
  }
}

double __cdecl MSG_ReadOriginZFloat(msg_t *msg, float oldValue)
{
  int roundedCenter; // [esp+24h] [ebp-10h]

  if ( MSG_ReadBit(msg) )
  {
    roundedCenter = (int)((*MSG_GetMapCenter())[2] + 9.313225746154785e-10);
    return (float)(roundedCenter
                 + (((int)(oldValue + 9.313225746154785e-10) + 0x10000 - roundedCenter) ^ MSG_ReadBits(msg, 0x11u))
                 - 0x10000);
  }
  else
  {
    return (float)((float)(MSG_ReadBits(msg, 7u) - 64) + oldValue);
  }
}

int __cdecl MSG_ReadDeltaEntityStruct(
        msg_t *msg,
        int time,
        char *from,
        char *to,
        unsigned int number,
        int totalFields,
        int skippedFieldBits)
{
  bool IsPlaying; // al
  const char *v9; // eax
  const char *v10; // eax
  int v11; // eax
  char *EntityTypeName; // eax
  int v13; // [esp+20h] [ebp-6Ch]
  bool v14; // [esp+24h] [ebp-68h]
  int bits; // [esp+28h] [ebp-64h]
  char v16; // [esp+46h] [ebp-46h]
  NetField *v18; // [esp+50h] [ebp-3Ch]
  const NetField *field; // [esp+58h] [ebp-34h]
  int lastChanged; // [esp+64h] [ebp-28h]
  int nextChanged; // [esp+68h] [ebp-24h]
  const NetFieldList *stateFieldList; // [esp+6Ch] [ebp-20h]
  unsigned int lc; // [esp+74h] [ebp-18h]
  const NetField *stateFields; // [esp+78h] [ebp-14h]
  int etype; // [esp+80h] [ebp-Ch]
  bool justUnlinked; // [esp+87h] [ebp-5h]
  unsigned int i; // [esp+88h] [ebp-4h]
  unsigned int ia; // [esp+88h] [ebp-4h]
  unsigned int ib; // [esp+88h] [ebp-4h]

  if ( number >= 0x400
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
          2290,
          0,
          "%s",
          "number < (1u << GENTITYNUM_BITS)") )
  {
    __debugbreak();
  }
  if ( MSG_ReadBit(msg) == 1 )
  {
    if ( cl_shownet && (cl_shownet->current.integer >= 2 || cl_shownet->current.integer == -1) )
      Com_Printf(16, "%3i: #%-3i remove\n", msg->readcount, number);
    return 1;
  }
  else if ( MSG_ReadBit(msg) )
  {
    justUnlinked = MSG_ReadBit(msg) != 0;
    lc = MSG_ReadLastChangedField(msg, totalFields);
    if ( cl_shownet && (cl_shownet->current.integer >= 2 || cl_shownet->current.integer == -1) )
      Com_Printf(16, "%3i: #%-3i ", msg->readcount, *(unsigned int *)to);
    *(unsigned int *)to = number;
    if ( strcmp(entityStateFields[0].name, "eType")
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
            2348,
            0,
            "%s",
            "strcmp( entityStateFields[0].name, \"eType\" ) == 0") )
    {
      __debugbreak();
    }
    lastChanged = -1;
    nextChanged = MSG_ReadNumFieldsSkipped(msg, skippedFieldBits, lc + 1) - 1;
    if ( nextChanged <= 0 )
    {
      MSG_ReadDeltaField(msg, time, from, to, entityStateFields, 0, 0);
      lastChanged = 0;
      if ( lc )
        nextChanged = MSG_ReadNumFieldsSkipped(msg, skippedFieldBits, lc);
    }
    else
    {
      MSG_CopyFieldOver(entityStateFields, from, to, 0);
    }
    etype = MSG_GetField((const int *)(to + 190), 2);
    IsPlaying = Demo_IsPlaying();
    stateFieldList = MSG_GetStateFieldListForEntityType(etype, IsPlaying);
    stateFields = stateFieldList->array;
    if ( lc <= stateFieldList->count )
    {
      if ( lastChanged == lc )
      {
        lastChanged = lc;
      }
      else
      {
        do
        {
          for ( i = lastChanged + 1; i < nextChanged; ++i )
            MSG_CopyFieldOver(stateFields, from, to, i);
          field = &stateFields[nextChanged];
          v14 = 0;
          if ( justUnlinked )
          {
            bits = field->bits;
            if ( bits >= -66 && bits <= -61 )
              v14 = 1;
          }
          MSG_ReadDeltaField(msg, time, from, to, field, 0, v14);
          lastChanged = nextChanged;
          if ( nextChanged == lc )
            break;
          nextChanged += MSG_ReadNumFieldsSkipped(msg, skippedFieldBits, lc - nextChanged);
          if ( (int)lc < nextChanged )
          {
            v9 = va("nextChanged is %i, lc is %i", nextChanged, lc);
            if ( !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
                    2440,
                    0,
                    "%s\n\t%s",
                    "static_cast<int>( lc ) >= nextChanged",
                    v9) )
              __debugbreak();
          }
          if ( nextChanged <= lastChanged )
          {
            v10 = va("nextChanged is %i, lastChanged is %i", nextChanged, lastChanged);
            if ( !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
                    2441,
                    0,
                    "%s\n\t%s",
                    "nextChanged > lastChanged",
                    v10) )
              __debugbreak();
          }
        }
        while ( nextChanged <= (int)lc );
      }
      for ( ia = 0; ia < lc; ++ia )
      {
        if ( stateFields[ia].changeHints == 2 )
        {
          v18 = (NetField *)&stateFields[ia];
          if ( justUnlinked && ((v13 = v18->bits, v13 < -66) || v13 > -61 ? (v16 = 0) : (v16 = 1), v16) )
            MSG_ReadDeltaField(msg, time, from, to, v18, 0, 1);
          else
            MSG_ReadDeltaField(msg, time, from, to, v18, 0, 0);
        }
      }
      if ( lastChanged != lc
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
              2491,
              0,
              "%s",
              "lastChanged == static_cast<int>( lc )") )
      {
        __debugbreak();
      }
      if ( lastChanged < 0
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
              2492,
              0,
              "%s",
              "lastChanged >= 0") )
      {
        __debugbreak();
      }
      for ( ib = lastChanged + 1; ib < stateFieldList->count; ++ib )
        MSG_CopyFieldOver(stateFields, from, to, ib);
      if ( msg_dumpEnts->current.enabled )
      {
        v11 = MSG_GetField((const int *)(to + 190), 2);
        EntityTypeName = BG_GetEntityTypeName(v11);
        Com_Printf(14, "%3i: changed ent, eType %s\n", number, EntityTypeName);
      }
      if ( strcmp(stateFields->name, "eType")
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
              2503,
              0,
              "%s",
              "strcmp( stateFields[0].name, \"eType\" ) == 0") )
      {
        __debugbreak();
      }
      return 0;
    }
    else
    {
      Com_Printf(14, "Last changed field was %i, but there are only %i fields\n", lc, stateFieldList->count);
      msg->overflowed = 1;
      return 0;
    }
  }
  else
  {
    memcpy(to, from, 0xE0u);
    return 0;
  }
}

int __cdecl MSG_ReadDeltaArchivedEntity(
        msg_t *msg,
        int time,
        const archivedEntity_s *from,
        archivedEntity_s *to,
        unsigned int number)
{
  if ( MSG_ReadDeltaEntity(msg, time, &from->s, &to->s, number) )
    return 1;
  else
    return MSG_ReadAppendedDeltaStruct(
             msg,
             time,
             (char *)&from->r,
             (char *)&to->r,
             number,
             numArchivedEntityFields,
             10,
             archivedEntityFields,
             1);
}

int __cdecl MSG_ReadAppendedDeltaStruct(
        msg_t *msg,
        int time,
        char *from,
        char *to,
        unsigned int number,
        int numFields,
        char indexBits,
        const NetField *stateFields,
        int skippedFieldBits)
{
  if ( number >= 1 << indexBits
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
          2260,
          0,
          "%s",
          "number < (1u << indexBits)") )
  {
    __debugbreak();
  }
  MSG_ReadDeltaFields(msg, time, from, to, numFields, stateFields, skippedFieldBits);
  return 0;
}

void __cdecl MSG_ReadDeltaPlayerstate(
        int localClientNum,
        msg_t *msg,
        playerState_s *from,
        bool predictedFieldsIgnoreXor)
{
  bool IsPlaying; // al
  bool v5; // al
  const char *v6; // eax
  const char *v7; // eax
  const char *v8; // eax
  int v9; // eax
  const char *v10; // eax
  int v11; // eax
  const char *v12; // eax
  int v13; // eax
  const char *v14; // eax
  int v15; // eax
  const char *v16; // eax
  int v17; // eax
  int v18; // eax
  int v19; // eax
  const char *v20; // eax
  int v21; // eax
  int v22; // eax
  int v23; // eax
  const char *v24; // eax
  bool v25; // al
  bool v26; // al
  int v27; // eax
  int v28; // eax
  const char *v29; // eax
  int v30; // eax
  const char *v31; // eax
  bool v32; // [esp+16h] [ebp-270Ah]
  bool v33; // [esp+17h] [ebp-2709h]
  int i; // [esp+34h] [ebp-26ECh]
  clientActive_t *LocalClientGlobals; // [esp+38h] [ebp-26E8h]
  int v36; // [esp+3Ch] [ebp-26E4h]
  int v37; // [esp+40h] [ebp-26E0h]
  unsigned int numFields; // [esp+44h] [ebp-26DCh]
  int print; // [esp+48h] [ebp-26D8h]
  NetField *field; // [esp+4Ch] [ebp-26D4h]
  NetField *fielda; // [esp+4Ch] [ebp-26D4h]
  signed int count; // [esp+50h] [ebp-26D0h]
  int LastChangedField; // [esp+54h] [ebp-26CCh]
  NetField *array; // [esp+58h] [ebp-26C8h]
  float *v45; // [esp+5Ch] [ebp-26C4h]
  unsigned __int8 dst[9896]; // [esp+60h] [ebp-26C0h] BYREF
  int Bits; // [esp+2708h] [ebp-18h]
  float *v48; // [esp+270Ch] [ebp-14h]
  bool v49; // [esp+2710h] [ebp-10h]
  int v50; // [esp+2714h] [ebp-Ch]
  int fieldNum; // [esp+2718h] [ebp-8h]
  NetField *stateFields; // [esp+271Ch] [ebp-4h]
  int src_4; // [esp+2738h] [ebp+18h]
  char v54; // [esp+273Ch] [ebp+1Ch]

  if ( !predictedFieldsIgnoreXor )
  {
    *(unsigned int *)&predictedFieldsIgnoreXor = dst;
    memset(dst, 0, 0x26A4u);
  }
  memcpy((unsigned __int8 *)src_4, (unsigned __int8 *)predictedFieldsIgnoreXor, 0x26A4u);
  if ( cl_shownet && (cl_shownet->current.integer >= 2 || cl_shownet->current.integer == -2) )
  {
    print = 1;
    Com_Printf(16, "%3i: playerstate ", msg->readcount);
  }
  else
  {
    print = 0;
  }
  v49 = MSG_ReadBit(msg) > 0;
  IsPlaying = Demo_IsPlaying();
  stateFields = (NetField *)MSG_GetNetFieldList(NET_FIELD_TYPE_PLAYERSTATE, IsPlaying)->array;
  v5 = Demo_IsPlaying();
  count = MSG_GetNetFieldList(NET_FIELD_TYPE_PLAYERSTATE, v5)->count;
  LastChangedField = MSG_ReadLastChangedField(msg, count + 1);
  if ( LastChangedField < 0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp", 2671, 0, "%s", "lc >= 0") )
  {
    __debugbreak();
  }
  if ( LastChangedField <= count + 1 )
  {
    if ( Demo_IsPlaying() )
    {
      v6 = va("PlayerState - Lastchangedfield: %d\n", LastChangedField);
      Demo_Printf(512, v6);
    }
    fieldNum = 0;
    v36 = -1;
    if ( LastChangedField > 0 )
    {
      --LastChangedField;
      while ( v36 < LastChangedField )
      {
        if ( msg->overflowed
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
                2692,
                0,
                "%s",
                "!msg->overflowed") )
        {
          __debugbreak();
        }
        v37 = v36 + MSG_ReadNumFieldsSkipped(msg, 4, LastChangedField - v36);
        Bits = 0;
        if ( Demo_IsPlaying() )
          Bits = MSG_GetNumBitsRead(msg);
        for ( fieldNum = v36 + 1; fieldNum < v37; ++fieldNum )
        {
          if ( stateFields[fieldNum].changeHints != 2 )
            MSG_CopyFieldOver(stateFields, (const void *)predictedFieldsIgnoreXor, (void *)src_4, fieldNum);
        }
        field = &stateFields[v37];
        if ( v37 > LastChangedField )
        {
          v7 = va("nextChanged == %i, lc == %i", v37, LastChangedField);
          if ( !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
                  2713,
                  0,
                  "%s\n\t%s",
                  "nextChanged <= lc",
                  v7) )
            __debugbreak();
        }
        if ( v37 <= v36 )
        {
          v8 = va("nextChanged == %i, lastChanged == %i", v37, v36);
          if ( !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
                  2714,
                  0,
                  "%s\n\t%s",
                  "nextChanged > lastChanged",
                  v8) )
            __debugbreak();
        }
        if ( msg->overflowed
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
                2716,
                0,
                "%s",
                "!msg->overflowed") )
        {
          __debugbreak();
        }
        v33 = v54 && v49 && field->changeHints == 3;
        MSG_ReadDeltaField(
          msg,
          (const int)from,
          (const void *)predictedFieldsIgnoreXor,
          (void *)src_4,
          field,
          print,
          v33);
        if ( msg->overflowed
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
                2718,
                0,
                "%s",
                "!msg->overflowed") )
        {
          __debugbreak();
        }
        v36 = v37;
        if ( Demo_IsPlaying() )
        {
          v48 = (float *)(field->offset + predictedFieldsIgnoreXor);
          v45 = (float *)(field->offset + src_4);
          if ( Demo_GetAnalyzePrintDataType(field->bits) )
          {
            v11 = MSG_GetNumBitsRead(msg);
            v12 = va(
                    "PlayerState - FieldName: %s From: %d To: %d BitsUsed: %d\n",
                    field->name,
                    *(unsigned int *)v48,
                    *(unsigned int *)v45,
                    v11 - Bits);
            Demo_Printf(512, v12);
          }
          else
          {
            v9 = MSG_GetNumBitsRead(msg);
            v10 = va(
                    "PlayerState - FieldName: %s From: %.3f To: %.3f BitsUsed: %d\n",
                    field->name,
                    *v48,
                    *v45,
                    v9 - Bits);
            Demo_Printf(512, v10);
          }
        }
      }
      if ( v36 != LastChangedField
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
              2733,
              0,
              "%s",
              "lastChanged == lc") )
      {
        __debugbreak();
      }
    }
    for ( fieldNum = v36 + 1; fieldNum < count; ++fieldNum )
      MSG_CopyFieldOver(stateFields, (const void *)predictedFieldsIgnoreXor, (void *)src_4, fieldNum);
    for ( fieldNum = 0; fieldNum < LastChangedField; ++fieldNum )
    {
      fielda = &stateFields[fieldNum];
      if ( fielda->changeHints == 2 )
      {
        v32 = v54 && v49 && fielda->changeHints == 3;
        MSG_ReadDeltaField(
          msg,
          (const int)from,
          (const void *)predictedFieldsIgnoreXor,
          (void *)src_4,
          fielda,
          print,
          v32);
      }
    }
    if ( !v49 )
    {
      LocalClientGlobals = CL_GetLocalClientGlobals(localClientNum);
      if ( !CL_GetPredictedOriginForServerTime(
              LocalClientGlobals,
              *(unsigned int *)src_4,
              (float *)(src_4 + 36),
              (float *)(src_4 + 48),
              (float *)(src_4 + 384),
              (int *)(src_4 + 8),
              (int *)(src_4 + 220)) )
      {
        Com_PrintError(14, "Unable to find the origin we sent, delta is not going to work");
        *(float *)(src_4 + 36) = *(float *)(predictedFieldsIgnoreXor + 36);
        *(float *)(src_4 + 40) = *(float *)(predictedFieldsIgnoreXor + 40);
        *(float *)(src_4 + 44) = *(float *)(predictedFieldsIgnoreXor + 44);
        *(float *)(src_4 + 48) = *(float *)(predictedFieldsIgnoreXor + 48);
        *(float *)(src_4 + 52) = *(float *)(predictedFieldsIgnoreXor + 52);
        *(float *)(src_4 + 56) = *(float *)(predictedFieldsIgnoreXor + 56);
        *(unsigned int *)(src_4 + 8) = *(unsigned int *)(predictedFieldsIgnoreXor + 8);
        *(unsigned int *)(src_4 + 220) = *(unsigned int *)(predictedFieldsIgnoreXor + 220);
        *(float *)(src_4 + 384) = *(float *)(predictedFieldsIgnoreXor + 384);
        *(float *)(src_4 + 388) = *(float *)(predictedFieldsIgnoreXor + 388);
        *(float *)(src_4 + 392) = *(float *)(predictedFieldsIgnoreXor + 392);
      }
    }
    Bits = 0;
    if ( Demo_IsPlaying() )
      Bits = MSG_GetNumBitsRead(msg);
    if ( MSG_ReadBit(msg) )
    {
      if ( cl_shownet && cl_shownet->current.integer == 4 )
        Com_Printf(16, "%s ", "PS_STATS");
      Bits = MSG_ReadBits(msg, 5);
      if ( (Bits & 1) != 0 )
        *(unsigned int *)(src_4 + 452) = MSG_ReadShort(msg);
      if ( (Bits & 2) != 0 )
        *(unsigned int *)(src_4 + 456) = MSG_ReadShort(msg);
      if ( (Bits & 4) != 0 )
        *(unsigned int *)(src_4 + 460) = MSG_ReadShort(msg);
      if ( (Bits & 8) != 0 )
        *(unsigned int *)(src_4 + 464) = MSG_ReadBits(msg, 5);
      if ( (Bits & 0x10) != 0 )
        *(unsigned int *)(src_4 + 468) = MSG_ReadByte(msg);
    }
    if ( Demo_IsPlaying() )
    {
      v13 = MSG_GetNumBitsRead(msg);
      v14 = va("PlayerState - FieldName: PS_STATS BitsUsed: %d\n", v13 - Bits);
      Demo_Printf(512, v14);
      Bits = MSG_GetNumBitsRead(msg);
    }
    if ( MSG_ReadBit(msg) )
    {
      for ( i = 0; i < 15; ++i )
      {
        if ( MSG_ReadBit(msg) )
        {
          if ( cl_shownet && cl_shownet->current.integer == 4 )
            Com_Printf(16, "%s ", "PS_WEAPON");
          if ( MSG_ReadBit(msg) )
            *(unsigned int *)(src_4 + 24 * i + 472) = MSG_ReadBits(msg, 11);
          if ( MSG_ReadBit(msg) )
            *(_BYTE *)(src_4 + 24 * i + 476) = MSG_ReadBits(msg, 4);
          if ( MSG_ReadBit(msg) )
            *(unsigned int *)(src_4 + 24 * i + 480) = MSG_ReadBits(msg, 32);
          if ( MSG_ReadBit(msg) )
            *(unsigned int *)(src_4 + 24 * i + 484) = MSG_ReadBits(msg, 32);
          *(_BYTE *)(src_4 + 24 * i + 488) = MSG_ReadBit(msg) != 0;
          *(_BYTE *)(src_4 + 24 * i + 489) = MSG_ReadBit(msg) != 0;
          *(_BYTE *)(src_4 + 24 * i + 490) = MSG_ReadBit(msg) != 0;
          *(_BYTE *)(src_4 + 24 * i + 491) = MSG_ReadBit(msg) != 0;
        }
      }
    }
    if ( Demo_IsPlaying() )
    {
      v15 = MSG_GetNumBitsRead(msg);
      v16 = va("PlayerState - FieldName: PS_WEAPON BitsUsed: %d\n", v15 - Bits);
      Demo_Printf(512, v16);
      Bits = MSG_GetNumBitsRead(msg);
    }
    while ( MSG_ReadBit(msg) )
    {
      v50 = MSG_ReadBits(msg, 4);
      if ( cl_shownet && cl_shownet->current.integer == 4 )
        Com_Printf(16, "%s ", "PS_AMMO");
      v17 = MSG_ReadBits(msg, 11);
      *(unsigned int *)(src_4 + 8 * v50 + 832) = v17;
      v18 = MSG_ReadBits(msg, 10);
      *(unsigned int *)(src_4 + 8 * v50 + 836) = v18;
    }
    if ( Demo_IsPlaying() )
    {
      v19 = MSG_GetNumBitsRead(msg);
      v20 = va("PlayerState - FieldName: PS_AMMO BitsUsed: %d\n", v19 - Bits);
      Demo_Printf(512, v20);
      Bits = MSG_GetNumBitsRead(msg);
    }
    while ( MSG_ReadBit(msg) )
    {
      v50 = MSG_ReadBits(msg, 4);
      if ( cl_shownet && cl_shownet->current.integer == 4 )
        Com_Printf(16, "%s ", "PS_AMMOCLIP");
      v21 = MSG_ReadBits(msg, 11);
      *(unsigned int *)(src_4 + 8 * v50 + 952) = v21;
      v22 = MSG_ReadBits(msg, 10);
      *(unsigned int *)(src_4 + 8 * v50 + 956) = v22;
    }
    if ( Demo_IsPlaying() )
    {
      v23 = MSG_GetNumBitsRead(msg);
      v24 = va("PlayerState - FieldName: PS_AMMOCLIP BitsUsed: %d\n", v23 - Bits);
      Demo_Printf(512, v24);
      Bits = MSG_GetNumBitsRead(msg);
    }
    if ( MSG_ReadBit(msg) )
    {
      v25 = Demo_IsPlaying();
      array = (NetField *)MSG_GetNetFieldList(NET_FIELD_TYPE_OBJECTIVE, v25)->array;
      v26 = Demo_IsPlaying();
      numFields = MSG_GetNetFieldList(NET_FIELD_TYPE_OBJECTIVE, v26)->count;
      for ( fieldNum = 0; fieldNum < 32; ++fieldNum )
      {
        v27 = MSG_ReadBits(msg, 3);
        *(unsigned int *)(src_4 + 48 * fieldNum + 1400) = v27;
        MSG_ReadDeltaFields(
          msg,
          (const int)from,
          (const void *)(predictedFieldsIgnoreXor + 48 * fieldNum + 1400),
          (void *)(src_4 + 48 * fieldNum + 1400),
          numFields,
          array,
          1,
          0);
      }
    }
    if ( Demo_IsPlaying() )
    {
      v28 = MSG_GetNumBitsRead(msg);
      v29 = va("PlayerState - FieldName: PS_OBJECTIVE BitsUsed: %d\n", v28 - Bits);
      Demo_Printf(512, v29);
      Bits = MSG_GetNumBitsRead(msg);
    }
    if ( MSG_ReadBit(msg) )
    {
      MSG_ReadDeltaHudElems(
        msg,
        (const int)from,
        (const hudelem_s *)(predictedFieldsIgnoreXor + 6420),
        (hudelem_s *)(src_4 + 6420),
        31);
      MSG_ReadDeltaHudElems(
        msg,
        (const int)from,
        (const hudelem_s *)(predictedFieldsIgnoreXor + 2948),
        (hudelem_s *)(src_4 + 2948),
        31);
    }
    if ( Demo_IsPlaying() )
    {
      v30 = MSG_GetNumBitsRead(msg);
      v31 = va("PlayerState - FieldName: PS_HUDELEMS BitsUsed: %d\n", v30 - Bits);
      Demo_Printf(512, v31);
    }
  }
  else
  {
    MSG_Discard(msg);
  }
}

void __cdecl MSG_ReadDeltaHudElems(msg_t *msg, int time, const hudelem_s *from, hudelem_s *to, int count)
{
  bool IsPlaying; // al
  bool v6; // al
  const char *v7; // eax
  const char *v8; // eax
  int alignY; // [esp+0h] [ebp-28h]
  int alignYa; // [esp+0h] [ebp-28h]
  int nextChanged; // [esp+8h] [ebp-20h]
  int nextChangeda; // [esp+8h] [ebp-20h]
  int lastChanged; // [esp+Ch] [ebp-1Ch]
  int field; // [esp+10h] [ebp-18h]
  signed int numFields; // [esp+14h] [ebp-14h]
  unsigned int lc; // [esp+18h] [ebp-10h]
  signed int lca; // [esp+18h] [ebp-10h]
  const NetField *fields; // [esp+1Ch] [ebp-Ch]
  int i; // [esp+20h] [ebp-8h]
  int inuse; // [esp+24h] [ebp-4h]

  if ( count != 31
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
          2555,
          0,
          "%s",
          "count == MAX_HUDELEMS_ARCHIVAL || count == MAX_HUDELEMS_CURRENT") )
  {
    __debugbreak();
  }
  IsPlaying = Demo_IsPlaying();
  fields = MSG_GetNetFieldList(NET_FIELD_TYPE_HUDELEMS, IsPlaying)->array;
  v6 = Demo_IsPlaying();
  numFields = MSG_GetNetFieldList(NET_FIELD_TYPE_HUDELEMS, v6)->count;
  inuse = MSG_ReadBits(msg, 5u);
  for ( i = 0; i < inuse; ++i )
  {
    lc = MSG_ReadBits(msg, 6u);
    lastChanged = -1;
    nextChanged = -1;
    if ( lc )
    {
      lca = lc - 1;
      while ( lastChanged < lca )
      {
        nextChanged = lastChanged + MSG_ReadNumFieldsSkipped(msg, 3, lca - lastChanged);
        if ( lca < nextChanged )
        {
          v7 = va("lc is %i, nextChanged is %i", lca, nextChanged);
          if ( !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
                  2579,
                  0,
                  "%s\n\t%s",
                  "static_cast<int>( lc ) >= nextChanged",
                  v7) )
            __debugbreak();
        }
        if ( nextChanged <= lastChanged )
        {
          v8 = va("nextChanged is %i, lastChanged is %i", nextChanged, lastChanged);
          if ( !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
                  2580,
                  0,
                  "%s\n\t%s",
                  "nextChanged > lastChanged",
                  v8) )
            __debugbreak();
        }
        for ( field = lastChanged + 1; field < nextChanged; ++field )
          MSG_CopyFieldOver(fields, (char *)&from[i], (char *)&to[i], field);
        MSG_ReadDeltaField(msg, time, (char *)&from[i], (char *)&to[i], &fields[nextChanged], 0, 0);
        lastChanged = nextChanged;
      }
    }
    for ( nextChangeda = nextChanged + 1; nextChangeda < numFields; ++nextChangeda )
      MSG_CopyFieldOver(fields, (char *)&from[i], (char *)&to[i], nextChangeda);
    if ( (from[i].alignOrg & 0xF0) != 0
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
            2594,
            0,
            "%s\n\t(from[i].alignOrg) = %i",
            "(!(from[i].alignOrg & ~15))",
            from[i].alignOrg) )
    {
      __debugbreak();
    }
    if ( (to[i].alignOrg & 0xF0) != 0
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
            2595,
            0,
            "%s\n\t(from[i].alignOrg) = %i",
            "(!(to[i].alignOrg & ~15))",
            from[i].alignOrg) )
    {
      __debugbreak();
    }
    if ( (((int)from[i].alignOrg >> 2) & 3) == 3
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
            2598,
            0,
            "%s\n\t(from[i].alignOrg) = %i",
            "(alignX == 0 || alignX == 1 || alignX == 2)",
            from[i].alignOrg) )
    {
      __debugbreak();
    }
    alignY = from[i].alignOrg & 3;
    if ( (from[i].alignOrg & 3) != 0
      && alignY != 1
      && alignY != 2
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
            2601,
            0,
            "%s\n\t(from[i].alignOrg) = %i",
            "(alignY == 0 || alignY == 1 || alignY == 2)",
            from[i].alignOrg) )
    {
      __debugbreak();
    }
    if ( (((int)to[i].alignOrg >> 2) & 3) == 3
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
            2604,
            0,
            "%s\n\t(to[i].alignOrg) = %i",
            "(alignX == 0 || alignX == 1 || alignX == 2)",
            to[i].alignOrg) )
    {
      __debugbreak();
    }
    alignYa = to[i].alignOrg & 3;
    if ( (to[i].alignOrg & 3) != 0
      && alignYa != 1
      && alignYa != 2
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
            2607,
            0,
            "%s\n\t(to[i].alignOrg) = %i",
            "(alignY == 0 || alignY == 1 || alignY == 2)",
            to[i].alignOrg) )
    {
      __debugbreak();
    }
  }
  while ( inuse < count && to[inuse].type )
  {
    memset((unsigned __int8 *)&to[inuse], 0, sizeof(hudelem_s));
    if ( to[inuse].type
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\qcommon\\msg_mp.cpp",
            2614,
            0,
            "%s",
            "to[inuse].type == HE_TYPE_FREE") )
    {
      __debugbreak();
    }
    ++inuse;
  }
}

void __cdecl MSG_InitHuffman()
{
  msgInit = 1;
  MSG_initHuffmanInternal();
}

void MSG_initHuffmanInternal()
{
  unsigned inttime2; // [esp+0h] [ebp-8h]
  unsigned inttime; // [esp+4h] [ebp-4h]

  Huff_Init(&msgHuff);
  time = Sys_Milliseconds();
  Huff_BuildFromData(&msgHuff.compressDecompress, msg_hData);
  time2 = Sys_Milliseconds();
  Com_Printf(16, "Huffman Took %d Milliseconds\n", time2 - time);
}

void __cdecl MSG_DumpNetFieldChanges_f()
{
  int arraySize[6]; // [esp+0h] [ebp-58h]
  const int *array; // [esp+18h] [ebp-40h]
  int iSize; // [esp+1Ch] [ebp-3Ch]
  const char *arrayNames[6]; // [esp+20h] [ebp-38h]
  int i; // [esp+38h] [ebp-20h]
  unsigned int iArrayNum; // [esp+3Ch] [ebp-1Ch]
  const int *changeArray[6]; // [esp+40h] [ebp-18h]

  changeArray[0] = (const int *)&orderInfo;
  changeArray[1] = orderInfo.arcEntState;
  changeArray[2] = orderInfo.clientState;
  changeArray[3] = orderInfo.playerState;
  changeArray[4] = orderInfo.objective;
  changeArray[5] = orderInfo.hudElem;
  arraySize[0] = 79;
  arraySize[1] = 10;
  arraySize[2] = 52;
  arraySize[3] = 179;
  arraySize[4] = 9;
  arraySize[5] = 43;
  arrayNames[0] = "Entity State";
  arrayNames[1] = "Archived Entity State";
  arrayNames[2] = "Client State";
  arrayNames[3] = "Player State";
  arrayNames[4] = "Objective";
  arrayNames[5] = "HUD Elem";
  Com_Printf(0, "========================================\n");
  Com_Printf(0, "NetField changes. format: field# : #changes\n");
  for ( iArrayNum = 0; iArrayNum < 6; ++iArrayNum )
  {
    Com_Printf(0, "========================================\n");
    Com_Printf(0, "    %s\n", arrayNames[iArrayNum]);
    Com_Printf(0, "--------------------\n");
    array = changeArray[iArrayNum];
    iSize = arraySize[iArrayNum];
    for ( i = 0; i < iSize; ++i )
    {
      if ( array[i] )
        Com_Printf(0, "%3i :%8i\n", i, array[i]);
    }
  }
  Com_Printf(0, "========================================\n");
  Com_Printf(0, "========================================\n");
}

char __thiscall bitarray<51>::areAllBitsEqual(
        bitarray<51> *this,
        const bitarray<51> *otherBitSet,
        const bitarray<51> *ignoreMaskBitSet)
{
  int i; // [esp+Ch] [ebp-4h]

  for ( i = 0; i < 2; ++i )
  {
    if ( (this->array[i] & ~ignoreMaskBitSet->array[i]) != (otherBitSet->array[i] & ~ignoreMaskBitSet->array[i]) )
      return 0;
  }
  return 1;
}

