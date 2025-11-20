#pragma once

int __cdecl _lzo_config_check();
int __cdecl __lzo_init_v2(unsigned int v, int s1, int s2, int s3, int s4, int s5, int s6, int s7, int s8, int s9);
int __cdecl __lzopro_lzo_init_v2(
        unsigned int v,
        int s1,
        int s2,
        int s3,
        int s4,
        int s5,
        int s6,
        int s7,
        int s8,
        int s9);
int __cdecl lzopro_lzo1x_1_11_compress(
        const unsigned __int8 *in,
        unsigned int in_len,
        unsigned __int8 *out,
        unsigned int *out_len,
        _WORD *wrkmem);
int __cdecl lzopro_lzo1x_1_11_compress_core(
        const unsigned __int8 *in,
        unsigned int in_len,
        unsigned __int8 *out,
        unsigned int *out_len,
        _WORD *wrkmem);
int __cdecl lzopro_lzo1x_decompress(
        const unsigned __int8 *in,
        unsigned int in_len,
        unsigned __int8 *out,
        unsigned int *out_len);
