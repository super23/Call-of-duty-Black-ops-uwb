#pragma once

void __cdecl Wavelet_DecompressLevel(unsigned __int8 *src, unsigned __int8 *dst, WaveletDecode *decode);
void __cdecl Wavelet_ConsumeBits(unsigned __int16 bitCount, WaveletDecode *decode);
int __cdecl Wavelet_DecodeValue(
        const WaveletHuffmanDecode *decodeTable,
        unsigned __int16 bitCount,
        int bias,
        WaveletDecode *decode);
void __cdecl Wavelet_AddDeltaToMipmap(
        unsigned __int8 *inout,
        int size,
        WaveletDecode *decode,
        const int *dstChanOffset);
