#include "r_image_wavelet.h"

void __cdecl Wavelet_DecompressLevel(unsigned __int8 *src, unsigned __int8 *dst, WaveletDecode *decode)
{
  int v3; // eax
  int v4; // eax
  int v5; // eax
  int v6; // eax
  int v7; // eax
  int v8; // eax
  _BYTE v9[3]; // [esp+4h] [ebp-70h]
  bool needsMipDelta; // [esp+7h] [ebp-6Dh]
  int dstChanOffset[4]; // [esp+8h] [ebp-6Ch] BYREF
  unsigned __int8 *dstChan; // [esp+18h] [ebp-5Ch]
  int size; // [esp+1Ch] [ebp-58h]
  int stride; // [esp+20h] [ebp-54h]
  int chanIndex; // [esp+24h] [ebp-50h]
  int coeff[4][3]; // [esp+28h] [ebp-4Ch]
  int evenOddParity; // [esp+58h] [ebp-1Ch]
  int dstBpp; // [esp+5Ch] [ebp-18h]
  int base; // [esp+60h] [ebp-14h]
  int x; // [esp+64h] [ebp-10h]
  int y; // [esp+68h] [ebp-Ch]
  int h; // [esp+6Ch] [ebp-8h]
  int w; // [esp+70h] [ebp-4h]

  if ( (decode->bpp < 1 || decode->bpp > 4)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_image_wavelet.cpp",
          103,
          0,
          "decode->bpp not in [1, 4]\n\t%i not in [%i, %i]",
          decode->bpp,
          1,
          4) )
  {
    __debugbreak();
  }
  if ( decode->bpp != decode->channels
    && (decode->bpp != 4 || decode->channels != 3)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_image_wavelet.cpp",
          104,
          0,
          "%s",
          "decode->bpp == decode->channels || (decode->bpp == 4 && decode->channels == 3)") )
  {
    __debugbreak();
  }
  if ( decode->mipLevel < 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_image_wavelet.cpp",
          105,
          0,
          "%s",
          "decode->mipLevel >= 0") )
  {
    __debugbreak();
  }
  dstBpp = decode->bpp;
  switch ( dstBpp )
  {
    case 1:
      dstChanOffset[0] = 0;
      break;
    case 2:
      dstChanOffset[0] = 0;
      dstChanOffset[1] = 1;
      break;
    case 3:
    case 4:
      dstChanOffset[0] = 0;
      dstChanOffset[1] = 1;
      dstChanOffset[2] = 2;
      dstChanOffset[3] = 3;
      break;
    default:
      break;
  }
  w = decode->width >> decode->mipLevel;
  h = decode->height >> decode->mipLevel;
  if ( w > 1 && h > 1 )
  {
    if ( !decode->dataInitialized )
    {
      decode->value = *(_WORD *)decode->data;
      decode->bit = 0;
      decode->data += 2;
      decode->dataInitialized = 1;
    }
    needsMipDelta = (decode->value & 1) != 0;
    Wavelet_ConsumeBits(1u, decode);
    if ( needsMipDelta )
      Wavelet_AddDeltaToMipmap(src, h * w / 4, decode, dstChanOffset);
    stride = dstBpp * w;
    for ( y = 0; y < h; y += 2 )
    {
      for ( x = 0; x < w; x += 2 )
      {
        if ( &dst[stride + dstBpp] > src
          && dst <= src
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_image_wavelet.cpp",
                171,
                0,
                "%s",
                "dst + stride + dstBpp <= src || dst > src") )
        {
          __debugbreak();
        }
        if ( decode->channels != 1 )
        {
          evenOddParity = decode->value & 1;
          Wavelet_ConsumeBits(1u, decode);
          coeff[0][0] = Wavelet_DecodeValue(waveletDecodeBlue, 9u, 255, decode);
          coeff[0][1] = Wavelet_DecodeValue(waveletDecodeBlue, 9u, 255, decode);
          coeff[0][2] = Wavelet_DecodeValue(waveletDecodeBlue, 9u, 255, decode);
          base = 2 * src[dstChanOffset[0]];
          dstChan = &dst[dstChanOffset[0]];
          dst[dstChanOffset[0]] = evenOddParity + ((coeff[0][2] + coeff[0][1] + coeff[0][0] + base) >> 1);
          dstChan[dstBpp] = (coeff[0][0] + base - (coeff[0][2] + coeff[0][1])) >> 1;
          dstChan[stride] = (coeff[0][1] - coeff[0][2] + base - coeff[0][0]) >> 1;
          dstChan[dstBpp + stride] = (base - coeff[0][0] - (coeff[0][1] - coeff[0][2])) >> 1;
          if ( decode->channels >= 3 )
          {
            evenOddParity = decode->value & 1;
            Wavelet_ConsumeBits(1u, decode);
            v3 = Wavelet_DecodeValue(waveletDecodeRedGreen, 0xAu, 510, decode);
            coeff[1][0] = coeff[0][0] + v3;
            v4 = Wavelet_DecodeValue(waveletDecodeRedGreen, 0xAu, 510, decode);
            coeff[1][1] = coeff[0][1] + v4;
            v5 = Wavelet_DecodeValue(waveletDecodeRedGreen, 0xAu, 510, decode);
            coeff[1][2] = coeff[0][2] + v5;
            base = 2 * src[dstChanOffset[1]];
            dstChan = &dst[dstChanOffset[1]];
            dst[dstChanOffset[1]] = evenOddParity + ((coeff[0][2] + v5 + coeff[1][1] + coeff[1][0] + base) >> 1);
            dstChan[dstBpp] = (coeff[1][0] + base - (coeff[1][2] + coeff[1][1])) >> 1;
            dstChan[stride] = (coeff[1][1] - coeff[1][2] + base - coeff[1][0]) >> 1;
            dstChan[dstBpp + stride] = (base - coeff[1][0] - (coeff[1][1] - coeff[1][2])) >> 1;
            evenOddParity = decode->value & 1;
            Wavelet_ConsumeBits(1u, decode);
            v6 = Wavelet_DecodeValue(waveletDecodeRedGreen, 0xAu, 510, decode);
            coeff[2][0] = coeff[0][0] + v6;
            v7 = Wavelet_DecodeValue(waveletDecodeRedGreen, 0xAu, 510, decode);
            coeff[2][1] = coeff[0][1] + v7;
            v8 = Wavelet_DecodeValue(waveletDecodeRedGreen, 0xAu, 510, decode);
            coeff[2][2] = coeff[0][2] + v8;
            base = 2 * src[dstChanOffset[2]];
            dstChan = &dst[dstChanOffset[2]];
            dst[dstChanOffset[2]] = evenOddParity + ((coeff[0][2] + v8 + coeff[2][1] + coeff[2][0] + base) >> 1);
            dstChan[dstBpp] = (coeff[2][0] + base - (coeff[2][2] + coeff[2][1])) >> 1;
            dstChan[stride] = (coeff[2][1] - coeff[2][2] + base - coeff[2][0]) >> 1;
            dstChan[dstBpp + stride] = (base - coeff[2][0] - (coeff[2][1] - coeff[2][2])) >> 1;
          }
        }
        if ( decode->channels == 3 )
        {
          if ( decode->channels != decode->bpp )
          {
            dstChan = &dst[dstChanOffset[3]];
            dst[dstChanOffset[3]] = -1;
            dstChan[dstBpp] = -1;
            dstChan[stride] = -1;
            dstChan[dstBpp + stride] = -1;
          }
        }
        else
        {
          evenOddParity = decode->value & 1;
          Wavelet_ConsumeBits(1u, decode);
          coeff[3][0] = Wavelet_DecodeValue(waveletDecodeAlpha, 9u, 255, decode);
          coeff[3][1] = Wavelet_DecodeValue(waveletDecodeAlpha, 9u, 255, decode);
          coeff[3][2] = Wavelet_DecodeValue(waveletDecodeAlpha, 9u, 255, decode);
          base = 2 * src[*(unsigned int *)&v9[4 * decode->channels]];
          dstChan = &dst[*(unsigned int *)&v9[4 * decode->channels]];
          *dstChan = evenOddParity + ((coeff[3][2] + coeff[3][1] + coeff[3][0] + base) >> 1);
          dstChan[dstBpp] = (coeff[3][0] + base - (coeff[3][2] + coeff[3][1])) >> 1;
          dstChan[stride] = (coeff[3][1] - coeff[3][2] + base - coeff[3][0]) >> 1;
          dstChan[dstBpp + stride] = (base - coeff[3][0] - (coeff[3][1] - coeff[3][2])) >> 1;
        }
        src += dstBpp;
        dst += 2 * dstBpp;
      }
      dst += stride;
    }
  }
  else
  {
    if ( w < 1 )
      w = 1;
    if ( h < 1 )
      h = 1;
    size = h * w;
    if ( h * w < 1
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_image_wavelet.cpp", 139, 0, "%s", "size >= 1") )
    {
      __debugbreak();
    }
    do
    {
      for ( chanIndex = 0; chanIndex < decode->channels; ++chanIndex )
        dst[dstChanOffset[chanIndex]] = *decode->data++;
      if ( decode->bpp != decode->channels )
        dst[dstChanOffset[3]] = -1;
      dst += dstBpp;
      --size;
    }
    while ( size );
  }
}

void __cdecl Wavelet_ConsumeBits(unsigned __int16 bitCount, WaveletDecode *decode)
{
  if ( (!bitCount || bitCount > 0x10u)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_image_wavelet.cpp",
          35,
          0,
          "%s",
          "bitCount > 0 && bitCount <= 16") )
  {
    __debugbreak();
  }
  if ( decode->bit >= 8u
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_image_wavelet.cpp",
          36,
          0,
          "%s",
          "decode->bit < 8") )
  {
    __debugbreak();
  }
  decode->value >>= bitCount;
  decode->value |= ((*((unsigned __int8 *)decode->data + 3) << 24)
                  | (*((unsigned __int8 *)decode->data + 2) << 16)
                  | (unsigned int)*(unsigned __int16 *)decode->data) >> decode->bit << (16 - bitCount);
  decode->bit += bitCount;
  decode->data += (int)decode->bit >> 3;
  decode->bit &= 7u;
}

int __cdecl Wavelet_DecodeValue(
        const WaveletHuffmanDecode *decodeTable,
        unsigned __int16 bitCount,
        int bias,
        WaveletDecode *decode)
{
  int index; // [esp+0h] [ebp-8h]
  int value; // [esp+4h] [ebp-4h]

  if ( (1 << bitCount) - 1 < 2 * bias - 1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_image_wavelet.cpp",
          51,
          0,
          "%s",
          "(1 << bitCount) - 1 >= bias * 2 - 1") )
  {
    __debugbreak();
  }
  if ( (1 << (bitCount - 1)) - 1 >= 2 * bias - 1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_image_wavelet.cpp",
          52,
          0,
          "%s",
          "(1 << (bitCount - 1)) - 1 < bias * 2 - 1") )
  {
    __debugbreak();
  }
  index = decode->value & 0xFFF;
  Wavelet_ConsumeBits(decodeTable[index].bits, decode);
  value = decodeTable[index].value;
  if ( value == -32768 )
  {
    value = (((1 << bitCount) - 1) & decode->value) - bias;
    Wavelet_ConsumeBits(bitCount, decode);
  }
  return value;
}

void __cdecl Wavelet_AddDeltaToMipmap(
        unsigned __int8 *inout,
        int size,
        WaveletDecode *decode,
        const int *dstChanOffset)
{
  unsigned __int8 v4; // bl
  int chanIndex; // [esp+4h] [ebp-8h]
  unsigned __int8 *value; // [esp+8h] [ebp-4h]

  do
  {
    chanIndex = 0;
    do
    {
      value = &inout[dstChanOffset[chanIndex]];
      v4 = *value;
      *value = Wavelet_DecodeValue(waveletDecodeAlpha, 9u, 255, decode) + v4;
      ++chanIndex;
    }
    while ( chanIndex < decode->channels );
    inout += decode->bpp;
    --size;
  }
  while ( size );
}

