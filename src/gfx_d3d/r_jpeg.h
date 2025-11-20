#pragma once

void __cdecl R_SaveJpg(
        char *filename,
        int quality,
        unsigned int image_width,
        unsigned int image_height,
        unsigned __int8 *image_buffer,
        unsigned __int8 *output_buffer);
void __cdecl jpegDest(jpeg_common_struct *cinfo, unsigned __int8 *outfile, int size);
void __cdecl init_destination(jpeg_compress_struct *cinfo);
void __cdecl term_destination(jpeg_compress_struct *cinfo);
unsigned int *__cdecl Z_MallocJpeg(unsigned int size);
void __cdecl Jpeg_Print(char *message);
jpeg_decompress_struct *__cdecl R_ReadJpgSetup(
        const unsigned __int8 *data,
        unsigned int dataLen,
        int *width,
        int *height,
        _D3DFORMAT *imageFormat);
unsigned __int8 *__cdecl Jpeg_HunkAlloc(unsigned int size);
void Jpeg_Error();
char __cdecl R_ReadJpg(jpeg_decompress_struct *context, unsigned __int8 *pixels, int pitch, int height);
