#pragma once

unsigned __int8 *__cdecl R_TakeResampledScreenshot(int width, int height, int bytesPerPixel, int headerSize);
void __cdecl R_ResampleImage(
        int oldWidth,
        int oldHeight,
        int newWidth,
        int newHeight,
        int bytesPerPixel,
        unsigned __int8 *data);
void __cdecl R_DownsamplePixelData(
        int oldSize,
        int newSize,
        int stride,
        int bytesPerPixel,
        unsigned __int8 *src,
        unsigned __int8 *dst);
void __cdecl R_UpsamplePixelData(
        int oldSize,
        int newSize,
        int stride,
        int bytesPerPixel,
        unsigned __int8 *src,
        unsigned __int8 *dst);
char __cdecl R_GetFrontBufferData(int x, int y, int width, int height, int bytesPerPixel, unsigned __int8 *buffer);
void __cdecl R_TakeScreenshotJpgCallback(char *param);
char __cdecl R_TakeScreenshot(char *filename, _D3DXIMAGE_FILEFORMAT format);
void __cdecl R_TakeScreenshotTgaCallback(char *param);
void __cdecl R_ScreenshotCommand(GfxScreenshotType type);
void __cdecl R_ScreenshotFilename(unsigned int lastNumber, const char *extension, char *fileName);
void R_LevelShot();
void __cdecl R_SaveGameShot(const char *saveName);
void __cdecl R_BeginHiResShot(int tiles);
void __cdecl R_UpdateHiResShot(float row, float col);
void __cdecl R_EndHiResShot();
void __cdecl R_BeginCubemapShot(int pixelWidthHeight, int pixelBorder);
int R_CubemapShotSetInitialState();
void __cdecl R_EndCubemapShot(CubemapShot shotIndex);
void __cdecl R_CopyCubemapShot(CubemapShot shotIndex);
void __cdecl R_CubemapShotCopySurfaceToBufferHDR(float *buffer, int bufferSizeInBytes);
char __cdecl R_GetBackBufferDataHDR(int x, int y, int width, int height, int bytesPerPixel, float *buffer);
unsigned int __cdecl half2float(unsigned __int16 h);
void initHalfToFloat();
unsigned int __cdecl convertmantissa(unsigned int i);
void R_CubemapShotRestoreState();
void __cdecl R_SaveCubemapShot(char *filename, CubemapShot shotIndex, float n0, float n1);
void __cdecl R_CubemapShotWriteTargaFile(char *filename, CubemapShot shotIndex, float n0, float n1);
void __cdecl R_CubemapShotWriteTargaHeader(int res, unsigned __int8 *fileBuffer);
void __cdecl R_CubemapShotCopyBufferToTarga(const unsigned __int8 *srcBuffer, unsigned __int8 *targa);
void __cdecl R_CubemapShotApplyFresnelToTarga(CubemapShot shotIndex, float n0, float n1, unsigned __int8 *targa);
unsigned __int8 __cdecl R_CubemapShotCalcReflectionFactor(
        int shotIndex,
        int colIndex,
        int rowIndex,
        float n0,
        float n1);
void __cdecl R_CreateReflectionRawDataFromCubemapShot(DiskGfxReflectionProbe *probeRawData, int downSampleRes);
void __cdecl R_CubemapFaceNormalize(float *inbuffer, int size, float *average);
void __cdecl R_CubemapShotRotateClockwise(float *inbuffer, float *tmpbuffer, int size);
void __cdecl R_CubemapShotRotateAntiClockwise(float *inbuffer, float *tmpbuffer, int size);
void __cdecl R_CubemapShotFlipVertical(float *buffer, int size);
void __cdecl R_CubemapShotFlipHorizontal(float *buffer, int size);
void __cdecl R_LightingFromCubemapShots(const float *baseColor);
void __cdecl R_CubemapLighting(
        float (**linearColors)[3],
        int width,
        int height,
        const float *baseColor,
        unsigned __int8 **pixels);
void __cdecl R_GetDirForCubemapPixel(int faceIndex, float x, float y, float *dir);
void __cdecl R_CubemapLightingForDir(
        float (**linearColors)[3],
        int width,
        int height,
        const float *dir,
        const float *baseColor,
        unsigned __int8 *pixel);
void __cdecl R_CubemapShotExtractLinearLight(
        unsigned __int8 **pixels,
        int width,
        int height,
        float (**linearColors)[3]);
