#include "decode.h"
#include <qcommon/common.h>
#include <speex/speex.h>

char __cdecl Decode_Init(int bandwidthEnum)
{
    const SpeexMode *mode; // [esp+8h] [ebp-4h]

    if ( bandwidthEnum )
    {
        if ( bandwidthEnum == 1 )
        {
            mode = &speex_wb_mode;
        }
        else
        {
            if ( bandwidthEnum != 2 )
            {
                Com_Printf(9, "Unknown bandwidth mode %i\n", bandwidthEnum);
                return 0;
            }
            mode = &speex_uwb_mode;
        }
    }
    else
    {
        mode = &speex_nb_mode;
    }
    g_decoder = (void *)speex_decoder_init((int)mode);
    speex_decoder_ctl((int)g_decoder);
    Decode_SetOptions();
    speex_decoder_ctl((int)g_decoder);
    g_current_decode_bandwidth_setting = bandwidthEnum;
    speex_bits_init((unsigned int **)&decodeBits);
    return 1;
}

void __cdecl Decode_SetOptions()
{
    speex_decoder_ctl((int)g_decoder);
}

void __cdecl Decode_Shutdown()
{
    if ( g_decoder )
    {
        speex_bits_destroy((int)&decodeBits);
        speex_decoder_destroy((int)g_decoder);
    }
    g_decoder = 0;
}

int __cdecl Decode_Sample(char *buffer, int maxLength, __int16 *out, int frame_size)
{
    int v5; // [esp+0h] [ebp-400Ch]
    float v6[4097]; // [esp+4h] [ebp-4008h]
    int i; // [esp+4008h] [ebp-4h]

    if ( maxLength > 4096
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\groupvoice\\decode.cpp", 100, 0, "%s", "maxLength <= 4096") )
    {
        __debugbreak();
    }
    if ( maxLength > frame_size
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\groupvoice\\decode.cpp",
                    101,
                    0,
                    "%s",
                    "maxLength <= frame_size") )
    {
        __debugbreak();
    }
    speex_bits_read_from(&decodeBits, (int)buffer, maxLength);
    if ( speex_decode((int)g_decoder) )
        v5 = 0;
    else
        v5 = 2 * frame_size;
    for ( i = 0; i < v5; ++i )
        out[i] = (int)v6[i];
    return v5;
}

