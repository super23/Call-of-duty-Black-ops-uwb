#include "r_cinematic.h"

void __cdecl R_CinematicInitSound(const _GUID *guid)
{
  if ( g_cinematicDS )
    g_cinematicDS->Release(g_cinematicDS);
  if ( DirectSoundCreate8(guid, &g_cinematicDS, 0) )
    DirectSoundCreate8(0, &g_cinematicDS, 0);
  if ( g_cinematicDS )
  {
    if ( g_cinematicDS->SetCooperativeLevel(g_cinematicDS, g_wv.hWnd, 2u) )
    {
      if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 495, 0, "%s", "hr == S_OK") )
        __debugbreak();
    }
  }
  else
  {
    Com_PrintError(16, "R_Cinematic_Init: Unable to initialize sound\n");
  }
}

void __cdecl Unlock_Bink_textures2(IDirect3DDevice9 *d3d_device, BINKTEXTURESET *set_textures)
{
  signed int m; // [esp+0h] [ebp-30h]
  signed int k; // [esp+4h] [ebp-2Ch]
  signed int j; // [esp+8h] [ebp-28h]
  int i; // [esp+Ch] [ebp-24h]
  unsigned __int8 *srcBits; // [esp+14h] [ebp-1Ch]
  unsigned __int8 *srcBitsa; // [esp+14h] [ebp-1Ch]
  unsigned __int8 *srcBitsb; // [esp+14h] [ebp-1Ch]
  unsigned __int8 *srcBitsc; // [esp+14h] [ebp-1Ch]
  char *dstBits; // [esp+18h] [ebp-18h]
  char *dstBitsa; // [esp+18h] [ebp-18h]
  char *dstBitsb; // [esp+18h] [ebp-18h]
  char *dstBitsc; // [esp+18h] [ebp-18h]
  int srcWidth; // [esp+1Ch] [ebp-14h]
  int srcWidtha; // [esp+1Ch] [ebp-14h]
  int srcWidthb; // [esp+1Ch] [ebp-14h]
  int srcWidthc; // [esp+1Ch] [ebp-14h]
  signed int srcHeight; // [esp+20h] [ebp-10h]
  signed int srcHeighta; // [esp+20h] [ebp-10h]
  signed int srcHeightb; // [esp+20h] [ebp-10h]
  signed int srcHeightc; // [esp+20h] [ebp-10h]
  BINKFRAMEPLANESET *bp_src; // [esp+24h] [ebp-Ch]
  _D3DLOCKED_RECT lr; // [esp+28h] [ebp-8h] BYREF

  bp_src = &set_textures->bink_buffers.Frames[set_textures->bink_buffers.FrameNum];
  set_textures->tex_draw.Ytexture->LockRect(set_textures->tex_draw.Ytexture, 0, &lr, 0, 0x2000u);
  srcWidth = set_textures->bink_buffers.YABufferWidth;
  srcHeight = set_textures->bink_buffers.YABufferHeight;
  srcBits = (unsigned __int8 *)bp_src->YPlane.Buffer;
  dstBits = (char *)lr.pBits;
  if ( lr.Pitch == srcWidth )
  {
    memcpy((unsigned __int8 *)lr.pBits, srcBits, srcHeight * srcWidth);
  }
  else
  {
    for ( i = 0; i < srcHeight; ++i )
      memcpy((unsigned __int8 *)&dstBits[lr.Pitch * i], &srcBits[srcWidth * i], srcWidth);
  }
  set_textures->tex_draw.Ytexture->UnlockRect(set_textures->tex_draw.Ytexture, 0);
  set_textures->tex_draw.cRtexture->LockRect(set_textures->tex_draw.cRtexture, 0, &lr, 0, 0x2000u);
  srcWidtha = set_textures->bink_buffers.cRcBBufferWidth;
  srcHeighta = set_textures->bink_buffers.cRcBBufferHeight;
  srcBitsa = (unsigned __int8 *)bp_src->cRPlane.Buffer;
  dstBitsa = (char *)lr.pBits;
  if ( lr.Pitch == srcWidtha )
  {
    memcpy((unsigned __int8 *)lr.pBits, srcBitsa, srcHeighta * srcWidtha);
  }
  else
  {
    for ( j = 0; j < srcHeighta; ++j )
      memcpy((unsigned __int8 *)&dstBitsa[lr.Pitch * j], &srcBitsa[srcWidtha * j], srcWidtha);
  }
  set_textures->tex_draw.cRtexture->UnlockRect(set_textures->tex_draw.cRtexture, 0);
  set_textures->tex_draw.cBtexture->LockRect(set_textures->tex_draw.cBtexture, 0, &lr, 0, 0x2000u);
  srcWidthb = set_textures->bink_buffers.cRcBBufferWidth;
  srcHeightb = set_textures->bink_buffers.cRcBBufferHeight;
  srcBitsb = (unsigned __int8 *)bp_src->cBPlane.Buffer;
  dstBitsb = (char *)lr.pBits;
  if ( lr.Pitch == srcWidthb )
  {
    memcpy((unsigned __int8 *)lr.pBits, srcBitsb, srcHeightb * srcWidthb);
  }
  else
  {
    for ( k = 0; k < srcHeightb; ++k )
      memcpy((unsigned __int8 *)&dstBitsb[lr.Pitch * k], &srcBitsb[srcWidthb * k], srcWidthb);
  }
  set_textures->tex_draw.cBtexture->UnlockRect(set_textures->tex_draw.cBtexture, 0);
  if ( set_textures->tex_draw.Atexture )
  {
    set_textures->tex_draw.Atexture->LockRect(set_textures->tex_draw.Atexture, 0, &lr, 0, 0x2000u);
    srcWidthc = set_textures->bink_buffers.YABufferWidth;
    srcHeightc = set_textures->bink_buffers.YABufferHeight;
    srcBitsc = (unsigned __int8 *)bp_src->APlane.Buffer;
    dstBitsc = (char *)lr.pBits;
    if ( lr.Pitch == srcWidthc )
    {
      memcpy((unsigned __int8 *)lr.pBits, srcBitsc, srcHeightc * srcWidthc);
    }
    else
    {
      for ( m = 0; m < srcHeightc; ++m )
        memcpy((unsigned __int8 *)&dstBitsc[lr.Pitch * m], &srcBitsc[srcWidthc * m], srcWidthc);
    }
    set_textures->tex_draw.Atexture->UnlockRect(set_textures->tex_draw.Atexture, 0);
  }
}

void __cdecl Unlock_Bink_textures_Callback()
{
  Unlock_Bink_textures2(dx.device, (BINKTEXTURESET *)&cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferHeight);
}

void __cdecl R_Cinematic_BeginLostDevice()
{
  unsigned int setIter; // [esp+4h] [ebp-8h]

  Sys_EnterCriticalSection(CRITSECT_CINEMATIC);
  if ( cinematicGlob.binkTextureSet.bink_buffers.YABufferWidth )
    R_Cinematic_RelinquishIO();
  if ( R_Cinematic_IsStarted() && !*(unsigned int *)&cinematicGlob.textureSets[44] )
  {
    *(unsigned int *)&cinematicGlob.textureSets[44] = 1;
    BinkPause(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth, 1);
  }
  for ( setIter = 0; setIter != 2; ++setIter )
  {
    Image_Release((GfxImage *)(656 * setIter + 182286232));
    Image_Release((GfxImage *)(656 * setIter + 182286284));
    Image_Release((GfxImage *)(656 * setIter + 182286336));
    Image_Release((GfxImage *)(656 * setIter + 182286388));
  }
  Sys_LeaveCriticalSection(CRITSECT_CINEMATIC);
}

void R_Cinematic_RelinquishIO()
{
  if ( !cinematicGlob.binkTextureSet.bink_buffers.YABufferWidth
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
          1479,
          0,
          "%s",
          "cinematicGlob.fileIoState != CIN_IOSTATE_RELINQUISHED") )
  {
    __debugbreak();
  }
  if ( !cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
          1480,
          0,
          "%s",
          "cinematicGlob.bink") )
  {
    __debugbreak();
  }
  BinkControlBackgroundIO(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth, 1);
  R_Cinematic_CheckBinkError();
  Sys_ResumeDiscReads(THREAD_OWNER_CINEMATICS);
  cinematicGlob.binkTextureSet.bink_buffers.YABufferWidth = 0;
}

const char *R_Cinematic_CheckBinkError()
{
  const char *result; // eax
  const char *v1; // eax

  result = (const char *)BinkGetError();
  if ( result && *result )
  {
    v1 = va("BinkGetError(): \"%s\"", result);
    result = (const char *)Assert_MyHandler(
                             "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
                             504,
                             0,
                             "%s\n\t%s",
                             "!binkError || binkError[0] == '\\0'",
                             v1);
    if ( !(_BYTE)result )
      __debugbreak();
  }
  return result;
}

void __cdecl R_Cinematic_EndLostDevice()
{
  Sys_EnterCriticalSection(CRITSECT_CINEMATIC);
  if ( cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth )
  {
    R_Cinematic_MakeBinkDrawTextures();
    R_Cinematic_ClearBinkDrawTextures();
  }
  Sys_LeaveCriticalSection(CRITSECT_CINEMATIC);
}

CinematicTextureSet *R_Cinematic_MakeBinkDrawTextures()
{
  CinematicTextureSet *result; // eax
  bool useAlpha; // [esp+Bh] [ebp-9h]
  int frameIter; // [esp+Ch] [ebp-8h]
  CinematicTextureSet *textureSet; // [esp+10h] [ebp-4h]

  useAlpha = 0;
  for ( frameIter = 0; frameIter != 2; ++frameIter )
  {
    if ( cinematicGlob.binkTextureSet.bink_buffers.Frames[frameIter + 1].YPlane.Buffer )
      useAlpha = 1;
  }
  textureSet = (CinematicTextureSet *)(656 * activeTexture + 182285816);
  cinematicGlob.masterHunk.base = R_Cinematic_MakeBinkTexture_PC(
                                    (GfxImage *)(656 * activeTexture + 182286232),
                                    cinematicGlob.binkTextureSet.bink_buffers.FrameNum,
                                    cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.Allocate,
                                    0x10000);
  cinematicGlob.masterHunk.atFront = R_Cinematic_MakeBinkTexture_PC(
                                       &textureSet->drawImageCr,
                                       (unsigned int)cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.Buffer,
                                       cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.BufferPitch,
                                       0x10000);
  cinematicGlob.masterHunk.atBack = R_Cinematic_MakeBinkTexture_PC(
                                      &textureSet->drawImageCb,
                                      (unsigned int)cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.Buffer,
                                      cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.BufferPitch,
                                      0x10000);
  if ( useAlpha )
    cinematicGlob.masterHunk.end = R_Cinematic_MakeBinkTexture_PC(
                                     &textureSet->drawImageA,
                                     cinematicGlob.binkTextureSet.bink_buffers.FrameNum,
                                     cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.Allocate,
                                     0x10000);
  else
    cinematicGlob.masterHunk.end = 0;
  result = (CinematicTextureSet *)Sys_IsRenderThread();
  if ( !(_BYTE)result )
  {
    RB_Resource_Flush();
    cinematicGlob.masterHunk.base = textureSet->drawImageY.texture.basemap;
    cinematicGlob.masterHunk.atFront = textureSet->drawImageCr.texture.basemap;
    result = (CinematicTextureSet *)textureSet->drawImageCb.texture.basemap;
    cinematicGlob.masterHunk.atBack = result;
    if ( useAlpha )
    {
      result = textureSet;
      cinematicGlob.masterHunk.end = textureSet->drawImageA.texture.basemap;
    }
    else
    {
      cinematicGlob.masterHunk.end = 0;
    }
  }
  return result;
}

IDirect3DTexture9 *__cdecl R_Cinematic_MakeBinkTexture_PC(
        GfxImage *image,
        unsigned int width,
        unsigned int height,
        int baseImageFlags)
{
  if ( Sys_IsRenderThread() )
  {
    Image_Setup(image, width, height, 1, baseImageFlags | 3, D3DFMT_L8);
    return (IDirect3DTexture9 *)image->texture.basemap;
  }
  else
  {
    image->width = width;
    image->height = height;
    image->depth = 1;
    image->mapType = 3;
    RB_Resource_CreateTexture(image, 0, 1, baseImageFlags | 3, D3DFMT_L8);
    return 0;
  }
}

void R_Cinematic_ClearBinkDrawTextures()
{
  if ( !cinematicGlob.masterHunk.base
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
          2188,
          0,
          "%s",
          "textures->Ytexture") )
  {
    __debugbreak();
  }
  if ( !cinematicGlob.masterHunk.atFront
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
          2189,
          0,
          "%s",
          "textures->cRtexture") )
  {
    __debugbreak();
  }
  if ( !cinematicGlob.masterHunk.atBack
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
          2190,
          0,
          "%s",
          "textures->cBtexture") )
  {
    __debugbreak();
  }
  R_Cinematic_ClearTexture(
    (IDirect3DTexture9 *)cinematicGlob.masterHunk.base,
    cinematicGlob.binkTextureSet.bink_buffers.FrameNum,
    cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.Allocate,
    0);
  R_Cinematic_ClearTexture(
    (IDirect3DTexture9 *)cinematicGlob.masterHunk.atFront,
    (int)cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.Buffer,
    cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.BufferPitch,
    0x80u);
  R_Cinematic_ClearTexture(
    (IDirect3DTexture9 *)cinematicGlob.masterHunk.atBack,
    (int)cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.Buffer,
    cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.BufferPitch,
    0x80u);
  if ( cinematicGlob.masterHunk.end )
    R_Cinematic_ClearTexture(
      (IDirect3DTexture9 *)cinematicGlob.masterHunk.end,
      cinematicGlob.binkTextureSet.bink_buffers.FrameNum,
      cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.Allocate,
      0);
}

void __cdecl R_Cinematic_ClearTexture(IDirect3DTexture9 *texture, int width, int height, unsigned __int8 clearValue)
{
  const char *v4; // eax
  int hr; // [esp+0h] [ebp-Ch]
  _D3DLOCKED_RECT lockedRect; // [esp+4h] [ebp-8h] BYREF

  hr = texture->LockRect(texture, 0, &lockedRect, 0, 0x2000u);
  if ( hr >= 0 )
  {
    if ( lockedRect.Pitch < width
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
            2172,
            0,
            "lockedRect.Pitch >= width\n\t%i, %i",
            lockedRect.Pitch,
            width) )
    {
      __debugbreak();
    }
    memset((unsigned __int8 *)lockedRect.pBits, clearValue, lockedRect.Pitch * height);
    texture->UnlockRect(texture, 0);
  }
  else
  {
    v4 = R_ErrorDescription(hr);
    Com_PrintError(8, "LockRect failed with error 0x%08x - %s", hr, v4);
  }
}

void __cdecl R_Cinematic_Init()
{
  if ( ptr
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
          2965,
          0,
          "%s",
          "!cinematicGlob.memPool") )
  {
    __debugbreak();
  }
  if ( g_cinematicInitialized
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
          2966,
          0,
          "%s",
          "!g_cinematicInitialized") )
  {
    __debugbreak();
  }
  memset((unsigned __int8 *)&cinematicGlob, 0, 0x9F4u);
  *(unsigned int *)&cinematicGlob.textureSets[36] = -1;
  R_Cinematic_ReserveMemory();
  if ( *(unsigned int *)&cinematicGlob.textureSets[44]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
          2973,
          0,
          "%s",
          "cinematicGlob.currentPaused == CINEMATIC_NOT_PAUSED") )
  {
    __debugbreak();
  }
  if ( *(unsigned int *)&cinematicGlob.textureSets[48]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
          2974,
          0,
          "%s",
          "cinematicGlob.targetPaused == CINEMATIC_NOT_PAUSED") )
  {
    __debugbreak();
  }
  g_cinematicInitialized = 1;
  if ( !g_cinematicDS )
    R_CinematicInitSound(0);
}

unsigned int *R_Cinematic_ReserveMemory()
{
  unsigned int *result; // eax

  result = Z_Malloc((int)&cls.unrankedServers[3021].gameType[11], "R_Cinematic_ReserveMemory", 19);
  ptr = result;
  return result;
}

void __cdecl R_Cinematic_Shutdown()
{
  int setIter; // [esp+0h] [ebp-4h]

  R_Cinematic_StopPlayback();
  g_cinematicInitialized = 0;
  Sys_WaitWorkerCmdInternal(&_UpdateFrameWorkerCmd);
  for ( setIter = 0; setIter != 2; ++setIter )
    R_Cinematic_ReleaseImages((CinematicTextureSet *)(656 * setIter + 182285816));
  Z_Free((char *)ptr, 19);
  ptr = 0;
  if ( g_cinematicDS )
  {
    g_cinematicDS->Release(g_cinematicDS);
    g_cinematicDS = 0;
  }
}

void __cdecl R_Cinematic_ReleaseImages(CinematicTextureSet *textureSet)
{
  int frameIter; // [esp+0h] [ebp-4h]

  for ( frameIter = 0; frameIter != 2; ++frameIter )
  {
    Image_Release(&textureSet->imageY[frameIter]);
    Image_Release(&textureSet->imageCr[frameIter]);
    Image_Release(&textureSet->imageCb[frameIter]);
    Image_Release(&textureSet->imageA[frameIter]);
    Z_VirtualFree(textureSet->Ytexture[frameIter], 23);
    Z_VirtualFree(textureSet->cRtexture[frameIter], 23);
    Z_VirtualFree(textureSet->cBtexture[frameIter], 23);
    Z_VirtualFree(textureSet->Atexture[frameIter], 23);
  }
  Image_Release(&textureSet->drawImageY);
  Image_Release(&textureSet->drawImageCr);
  Image_Release(&textureSet->drawImageCb);
  Image_Release(&textureSet->drawImageA);
}

void __cdecl R_Cinematic_StartPlayback_Internal(const char *name, unsigned int playbackFlags, int volume)
{
  Sys_EnterCriticalSection(CRITSECT_CINEMATIC_TARGET_CHANGE);
  I_strncpyz(cinematicGlob.targetCinematicName, name, 256);
  cinematicGlob.targetCinematicChanged = 1;
  cinematicGlob.cinematicFinished = 0;
  *(unsigned int *)&cinematicGlob.textureSets[48] = (playbackFlags & 0x80u) != 0;
  cinematicGlob.playbackFlags = playbackFlags;
  dword_ADD7B34 = volume;
  Sys_LeaveCriticalSection(CRITSECT_CINEMATIC_TARGET_CHANGE);
}

void __cdecl R_Cinematic_StartNextPlayback()
{
  Sys_EnterCriticalSection(CRITSECT_CINEMATIC);
  if ( R_Cinematic_IsNextReady_Internal() )
  {
    R_Cinematic_StartPlayback_Internal(
      cinematicGlob.nextCinematicName,
      cinematicGlob.nextCinematicPlaybackFlags,
      dword_ADD7B34);
    cinematicGlob.nextCinematicName[0] = 0;
  }
  Sys_LeaveCriticalSection(CRITSECT_CINEMATIC);
}

bool __cdecl R_Cinematic_IsNextReady_Internal()
{
  return cinematicGlob.nextCinematicName[0] != 0;
}

void __cdecl R_Cinematic_StopPlayback()
{
  Sys_EnterCriticalSection(CRITSECT_CINEMATIC);
  Sys_EnterCriticalSection(CRITSECT_CINEMATIC_TARGET_CHANGE);
  cinematicGlob.targetCinematicName[0] = 0;
  cinematicGlob.targetCinematicChanged = 1;
  cinematicGlob.cinematicFinished = 0;
  Sys_LeaveCriticalSection(CRITSECT_CINEMATIC_TARGET_CHANGE);
  Sys_LeaveCriticalSection(CRITSECT_CINEMATIC);
}

int __cdecl _UpdateFrameCallback()
{
  R_Cinematic_UpdateFrame_Core(0);
  _InterlockedExchange(&cineThreadBlock, 0);
  return 0;
}

void __cdecl R_Cinematic_UpdateFrame_Core(bool force_wait)
{
  bool localTargetChanged; // [esp+3h] [ebp-111h]
  char localTargetCinematic[260]; // [esp+4h] [ebp-110h] BYREF
  unsigned int localTargetPlaybackFlags; // [esp+10Ch] [ebp-8h]
  bool isCinematicBeingPlayed; // [esp+113h] [ebp-1h]

  if ( !r_cinematic_disabled )
  {
    Sys_EnterCriticalSection(CRITSECT_CINEMATIC_TARGET_CHANGE);
    localTargetChanged = cinematicGlob.targetCinematicChanged;
    if ( cinematicGlob.targetCinematicChanged )
    {
      I_strncpyz(localTargetCinematic, cinematicGlob.targetCinematicName, 256);
      cinematicGlob.targetCinematicChanged = 0;
      localTargetPlaybackFlags = cinematicGlob.playbackFlags;
    }
    else
    {
      localTargetCinematic[0] = 0;
      localTargetPlaybackFlags = 0;
    }
    Sys_LeaveCriticalSection(CRITSECT_CINEMATIC_TARGET_CHANGE);
    if ( !localTargetChanged
      && localTargetPlaybackFlags
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
            2858,
            0,
            "%s",
            "localTargetChanged || !localTargetPlaybackFlags") )
    {
      __debugbreak();
    }
    if ( !localTargetChanged
      && localTargetCinematic
      && localTargetCinematic[0]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
            2859,
            0,
            "%s",
            "localTargetChanged || !localTargetCinematic || !localTargetCinematic[0]") )
    {
      __debugbreak();
    }
    byte_ADD7B38 = 0;
    if ( localTargetChanged )
    {
      if ( cinematicGlob.currentCinematicName[0] )
        R_Cinematic_StopPlayback_Now();
      if ( localTargetCinematic[0] && R_Cinematic_StartPlayback_Now(localTargetCinematic, localTargetPlaybackFlags) )
      {
        I_strncpyz(cinematicGlob.currentCinematicName, localTargetCinematic, 256);
        cinematicGlob.targetCinematicName[0] = 0;
      }
      else
      {
        cinematicGlob.currentCinematicName[0] = 0;
      }
    }
    isCinematicBeingPlayed = cinematicGlob.currentCinematicName[0] != 0;
    if ( cinematicGlob.currentCinematicName[0] )
    {
      *(unsigned int *)&cinematicGlob.textureSets[40] = 0;
    }
    else
    {
      if ( *(unsigned int *)&cinematicGlob.textureSets[36] != -1
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
              2910,
              0,
              "%s",
              "cinematicGlob.activeImageFrame == CINEMATIC_INVALID_IMAGE_FRAME") )
      {
        __debugbreak();
      }
      if ( R_Cinematic_AreHunksOpen() )
      {
        ++*(unsigned int *)&cinematicGlob.textureSets[40];
        if ( *(int *)&cinematicGlob.textureSets[40] >= 5 )
          R_Cinematic_HunksClose();
      }
    }
    if ( isCinematicBeingPlayed )
    {
      if ( !cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
              2926,
              0,
              "%s",
              "cinematicGlob.bink") )
      {
        __debugbreak();
      }
      if ( !cinematicGlob.cinematicFinished && !R_Cinematic_Advance(force_wait) )
      {
        SND_NotifyCinematicEnd();
        cinematicGlob.cinematicFinished = 1;
      }
    }
  }
}

char __cdecl R_Cinematic_AreHunksOpen()
{
  if ( CinematicHunk_IsOpen((CinematicHunk *)&cinematicGlob.residentHunk.atFront) )
  {
    if ( !CinematicHunk_IsOpen((CinematicHunk *)&cinematicGlob.framesStopped)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
            825,
            0,
            "%s",
            "CinematicHunk_IsOpen( &cinematicGlob.binkHunk )") )
    {
      __debugbreak();
    }
    if ( !CinematicHunk_IsOpen((CinematicHunk *)&cinematicGlob.textureSets[12])
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
            826,
            0,
            "%s",
            "CinematicHunk_IsOpen( &cinematicGlob.residentHunk )") )
    {
      __debugbreak();
    }
    return 1;
  }
  else
  {
    if ( CinematicHunk_IsOpen((CinematicHunk *)&cinematicGlob.framesStopped)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
            834,
            0,
            "%s",
            "!CinematicHunk_IsOpen( &cinematicGlob.binkHunk )") )
    {
      __debugbreak();
    }
    if ( CinematicHunk_IsOpen((CinematicHunk *)&cinematicGlob.textureSets[12])
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
            835,
            0,
            "%s",
            "!CinematicHunk_IsOpen( &cinematicGlob.residentHunk )") )
    {
      __debugbreak();
    }
    return 0;
  }
}

char __cdecl CinematicHunk_IsOpen(CinematicHunk *hunk)
{
  if ( hunk->base )
  {
    if ( !hunk->atFront
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 544, 0, "%s", "hunk->atFront") )
    {
      __debugbreak();
    }
    if ( !hunk->atBack
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 545, 0, "%s", "hunk->atBack") )
    {
      __debugbreak();
    }
    if ( !hunk->end
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 546, 0, "%s", "hunk->end") )
    {
      __debugbreak();
    }
    return 1;
  }
  else
  {
    if ( hunk->atFront
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
            551,
            0,
            "%s",
            "!hunk->atFront") )
    {
      __debugbreak();
    }
    if ( hunk->atBack
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 552, 0, "%s", "!hunk->atBack") )
    {
      __debugbreak();
    }
    if ( hunk->end
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 553, 0, "%s", "!hunk->end") )
    {
      __debugbreak();
    }
    return 0;
  }
}

void R_Cinematic_HunksClose()
{
  CinematicHunk_Close((CinematicHunk *)&cinematicGlob.residentHunk.atFront);
  CinematicHunk_Close((CinematicHunk *)&cinematicGlob.framesStopped);
  CinematicHunk_Close((CinematicHunk *)&cinematicGlob.textureSets[12]);
}

void __cdecl CinematicHunk_Close(CinematicHunk *hunk)
{
  if ( !hunk->base
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 526, 0, "%s", "hunk->base") )
  {
    __debugbreak();
  }
  if ( !hunk->atFront
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 527, 0, "%s", "hunk->atFront") )
  {
    __debugbreak();
  }
  if ( !hunk->atBack
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 528, 0, "%s", "hunk->atBack") )
  {
    __debugbreak();
  }
  if ( !hunk->end
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 529, 0, "%s", "hunk->end") )
  {
    __debugbreak();
  }
  hunk->base = 0;
  hunk->atFront = 0;
  hunk->atBack = 0;
  hunk->end = 0;
  hunk->lastAllocPtr = 0;
  hunk->fragmented = 0;
}

char __cdecl R_Cinematic_Advance(bool force_wait)
{
  bool v3; // [esp+20h] [ebp-C0h]
  bool v4; // [esp+24h] [ebp-BCh]
  IDirect3DTexture9 **bt; // [esp+8Ch] [ebp-54h]
  void **p_Buffer; // [esp+90h] [ebp-50h]
  int i; // [esp+94h] [ebp-4Ch]
  int maxMsecToWait; // [esp+98h] [ebp-48h]
  CinematicEnum targetPaused; // [esp+9Ch] [ebp-44h]
  bool useCustomSkipLogic; // [esp+A2h] [ebp-3Eh]
  BINKREALTIME binkRealtime; // [esp+A4h] [ebp-3Ch] BYREF
  unsigned int percentageFull; // [esp+DCh] [ebp-4h]

  targetPaused = *(unsigned int *)&cinematicGlob.textureSets[48];
  if ( *(unsigned int *)&cinematicGlob.textureSets[48] != 1
    && *(unsigned int *)&cinematicGlob.textureSets[48] != 2
    && *(unsigned int *)&cinematicGlob.textureSets[48]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
          1942,
          0,
          "%s\n\t(targetPaused) = %i",
          "(targetPaused == CINEMATIC_PAUSED || targetPaused == CINEMATIC_SCRIPT_PAUSED || targetPaused == CINEMATIC_NOT_PAUSED)",
          *(unsigned int *)&cinematicGlob.textureSets[48]) )
  {
    __debugbreak();
  }
  if ( *(unsigned int *)&cinematicGlob.textureSets[44] != 1
    && *(unsigned int *)&cinematicGlob.textureSets[44] != 2
    && *(unsigned int *)&cinematicGlob.textureSets[44]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
          1943,
          0,
          "%s\n\t(cinematicGlob.currentPaused) = %i",
          "(cinematicGlob.currentPaused == CINEMATIC_PAUSED || cinematicGlob.currentPaused == CINEMATIC_SCRIPT_PAUSED || "
          "cinematicGlob.currentPaused == CINEMATIC_NOT_PAUSED)",
          *(unsigned int *)&cinematicGlob.textureSets[44]) )
  {
    __debugbreak();
  }
  if ( !cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
          1945,
          0,
          "%s",
          "cinematicGlob.bink") )
  {
    __debugbreak();
  }
  if ( byte_ADD7B38
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
          1947,
          0,
          "%s",
          "!cinematicGlob.underrun") )
  {
    __debugbreak();
  }
  BG_EvalVehicleName();
  percentageFull = R_Cinematic_GetPercentageFull();
  if ( percentageFull < 0x14 && (cinematicGlob.playbackFlags & 1) == 0 && (cinematicGlob.playbackFlags & 0x10) == 0 )
  {
    byte_ADD7B38 = 1;
    targetPaused = CINEMATIC_PAUSED;
  }
  if ( targetPaused != *(unsigned int *)&cinematicGlob.textureSets[44] )
  {
    if ( targetPaused == CINEMATIC_PAUSED || targetPaused == CINEMATIC_SCRIPT_PAUSED )
      BinkPause(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth, 1);
    else
      BinkPause(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth, 0);
    R_Cinematic_CheckBinkError();
    *(unsigned int *)&cinematicGlob.textureSets[44] = targetPaused;
    if ( targetPaused == CINEMATIC_NOT_PAUSED )
    {
      *(unsigned int *)&cinematicGlob.usingAlpha = Sys_Milliseconds();
      cinematicGlob.bink = *(BINK **)(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth + 12);
    }
  }
  v4 = force_wait || (cinematicGlob.playbackFlags & 0x40) != 0 && (cinematicGlob.playbackFlags & 1) != 0;
  v3 = (cinematicGlob.playbackFlags & 0x40) != 0 && (cinematicGlob.playbackFlags & 1) != 0;
  useCustomSkipLogic = v3;
  if ( (cinematicGlob.playbackFlags & 2) != 0 )
    useCustomSkipLogic = 1;
  if ( targetPaused == CINEMATIC_NOT_PAUSED )
  {
    if ( v4 )
    {
      maxMsecToWait = 33;
      while ( R_Cinematic_BinkShouldWait(useCustomSkipLogic) )
      {
        if ( !maxMsecToWait-- )
          break;
        NET_Sleep(1u);
      }
    }
    if ( force_wait || !R_Cinematic_BinkShouldWait(useCustomSkipLogic) )
    {
      bt = &cinematicGlob.binkTextureSet.textures[0].Ytexture;
      p_Buffer = &cinematicGlob.binkTextureSet.bink_buffers.Frames[0].cRPlane.Buffer;
      for ( i = 0; i < (int)cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferHeight; ++i )
      {
        p_Buffer[1] = (void *)*((unsigned int *)&cinematicGlob.masterHunk.lastAllocPtr + i);
        p_Buffer[2] = (void *)cinematicGlob.binkTextureSet.bink_buffers.FrameNum;
        p_Buffer[4] = (void *)*((unsigned int *)&cinematicGlob.binkHunk.base + i);
        p_Buffer[5] = cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.Buffer;
        p_Buffer[7] = (void *)*((unsigned int *)&cinematicGlob.binkHunk.atBack + i);
        p_Buffer[8] = cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.Buffer;
        if ( bt[7] )
        {
          p_Buffer[10] = (void *)*((unsigned int *)&cinematicGlob.binkHunk.lastAllocPtr + i);
          p_Buffer[11] = (void *)cinematicGlob.binkTextureSet.bink_buffers.FrameNum;
        }
        bt += 8;
        p_Buffer += 12;
      }
      PIXBeginNamedEvent(-1, "R_Cinematic_Thread2");
      BinkDoFrame(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth);
      while ( R_Cinematic_BinkShouldSkip(useCustomSkipLogic)
           && ((cinematicGlob.playbackFlags & 2) != 0
            || *(unsigned int *)(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth + 12) != *(unsigned int *)(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth + 8)) )
      {
        if ( *(unsigned int *)(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth + 12) == 1 )
        {
          *(unsigned int *)&cinematicGlob.usingAlpha = Sys_Milliseconds();
          cinematicGlob.bink = *(BINK **)(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth + 12);
        }
        BinkNextFrame(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth);
        BinkDoFrame(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth);
      }
      if ( GetCurrentThreadId() == g_DXDeviceThread )
        D3DPERF_EndEvent();
      if ( Sys_IsRenderThread() )
      {
        Unlock_Bink_textures2(dx.device, (BINKTEXTURESET *)&cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferHeight);
      }
      else
      {
        RB_Resource_Callback(Unlock_Bink_textures_Callback);
        RB_Resource_Flush();
      }
      if ( *(unsigned int *)&cinematicGlob.textureSets[36] == -1 )
        cinematicGlob.firstFrameNotify = 1;
      if ( cinematicGlob.binkTextureSet.bink_buffers.Frames[0].cRPlane.Allocate >= 2u
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
              2076,
              0,
              "%s\n\t(cinematicGlob.binkTextureSet.bink_buffers.FrameNum) = %i",
              "(cinematicGlob.binkTextureSet.bink_buffers.FrameNum == 0 || cinematicGlob.binkTextureSet.bink_buffers.FrameNum == 1)",
              cinematicGlob.binkTextureSet.bink_buffers.Frames[0].cRPlane.Allocate) )
      {
        __debugbreak();
      }
      *(unsigned int *)&cinematicGlob.textureSets[36] = cinematicGlob.binkTextureSet.bink_buffers.Frames[0].cRPlane.Allocate;
      dword_ADD7B1C = activeTexture;
      if ( (cinematicGlob.playbackFlags & 2) != 0
        || *(unsigned int *)(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth + 12) != *(unsigned int *)(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth + 8) )
      {
        BinkNextFrame(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth);
      }
    }
  }
  R_Cinematic_CheckBinkError();
  if ( (cinematicGlob.playbackFlags & 2) != 0
    || *(unsigned int *)(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth + 12) != *(unsigned int *)(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth
                                                                                                + 8) )
  {
    if ( !cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
            2098,
            0,
            "%s",
            "cinematicGlob.bink") )
    {
      __debugbreak();
    }
    BinkGetRealtime(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth, &binkRealtime, 0);
    R_Cinematic_UpdateTimeInMsec(&binkRealtime);
    if ( (cinematicGlob.playbackFlags & 8) != 0 )
    {
      percentageFull = 100;
    }
    else if ( binkRealtime.ReadBufferUsed < binkRealtime.ReadBufferSize )
    {
      percentageFull = (__int64)((double)binkRealtime.ReadBufferUsed / (double)cinematicGlob.binkIOSize * 100.0);
    }
    else
    {
      percentageFull = 100;
    }
    if ( cinematicGlob.binkTextureSet.bink_buffers.YABufferWidth && percentageFull > 0x5F )
      R_Cinematic_RelinquishIO();
    if ( cinematicGlob.binkTextureSet.bink_buffers.YABufferWidth != 1 && percentageFull < 0x32 )
    {
      if ( (cinematicGlob.playbackFlags & 8) != 0
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
              2114,
              0,
              "%s",
              "!(cinematicGlob.playbackFlags & CINEMATIC_PLAYBACKFLAGS_MEMORY_RESIDENT)") )
      {
        __debugbreak();
      }
      R_Cinematic_SeizeIO();
    }
    return 1;
  }
  else
  {
    cinematicGlob.lastFrameNotify = 1;
    if ( cinematicGlob.binkTextureSet.bink_buffers.YABufferWidth )
      R_Cinematic_RelinquishIO();
    return 0;
  }
}

unsigned int __cdecl R_Cinematic_GetPercentageFull()
{
  BINKREALTIME binkRealtime; // [esp+24h] [ebp-38h] BYREF

  if ( !cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
          1445,
          0,
          "%s",
          "cinematicGlob.bink") )
  {
    __debugbreak();
  }
  BinkGetRealtime(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth, &binkRealtime, 0);
  if ( (cinematicGlob.playbackFlags & 8) != 0 )
    return 100;
  if ( binkRealtime.ReadBufferUsed < binkRealtime.ReadBufferSize )
    return (__int64)((double)binkRealtime.ReadBufferUsed / (double)cinematicGlob.binkIOSize * 100.0);
  return 100;
}

const char *R_Cinematic_SeizeIO()
{
  const char *result; // eax

  if ( cinematicGlob.binkTextureSet.bink_buffers.YABufferWidth == 1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
          1452,
          0,
          "%s",
          "cinematicGlob.fileIoState != CIN_IOSTATE_SEIZED") )
  {
    __debugbreak();
  }
  if ( !cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
          1453,
          0,
          "%s",
          "cinematicGlob.bink") )
  {
    __debugbreak();
  }
  if ( !cinematicGlob.binkTextureSet.bink_buffers.YABufferWidth )
    Sys_SuspendDiscReads(THREAD_OWNER_CINEMATICS);
  BinkControlBackgroundIO(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth, 2);
  result = R_Cinematic_CheckBinkError();
  cinematicGlob.binkTextureSet.bink_buffers.YABufferWidth = 1;
  return result;
}

void __cdecl R_Cinematic_UpdateTimeInMsec(const BINKREALTIME *binkRealtime)
{
  const char *v1; // eax
  unsigned __int64 frameNum; // [esp+8h] [ebp-20h]
  unsigned __int64 frameRateDiv; // [esp+10h] [ebp-18h]
  unsigned __int64 timeInMsec; // [esp+18h] [ebp-10h]
  unsigned __int64 frameRate; // [esp+20h] [ebp-8h]

  frameNum = binkRealtime->FrameNum;
  if ( (unsigned int)frameNum < 0x80000000 )
  {
    frameRateDiv = binkRealtime->FrameRateDiv;
    frameRate = binkRealtime->FrameRate;
    timeInMsec = 1000 * frameNum * frameRateDiv / frameRate;
    cinematicGlob.timeInMsec = timeInMsec;
    if ( timeInMsec != (unsigned int)timeInMsec )
    {
      v1 = va(
             "%08x:%08x, %08x:%08x, %08x:%08x, %08x:%08x",
             HIDWORD(timeInMsec),
             (unsigned int)timeInMsec,
             HIDWORD(frameNum),
             (unsigned int)frameNum,
             HIDWORD(frameRate),
             (unsigned int)frameRate,
             HIDWORD(frameRateDiv),
             (unsigned int)frameRateDiv);
      if ( !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
              1518,
              0,
              "%s\n\t%s",
              "cinematicGlob.timeInMsec == timeInMsec",
              v1) )
        __debugbreak();
    }
  }
  else
  {
    cinematicGlob.timeInMsec = 0;
  }
}

int __cdecl R_Cinematic_BinkShouldSkip(bool useCustomLogic)
{
  float bink; // [esp+14h] [ebp-8h]

  if ( !useCustomLogic )
    return BinkShouldSkip(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth);
  bink = (float)(int)cinematicGlob.bink;
  return *(unsigned int *)(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth + 12) < (unsigned int)(__int64)((double)(int)(Sys_Milliseconds() - *(unsigned int *)&cinematicGlob.usingAlpha) * *(float *)&cinematicGlob.fileIoState / 1000.0 + bink);
}

int __cdecl R_Cinematic_BinkShouldWait(bool useCustomLogic)
{
  float bink; // [esp+14h] [ebp-8h]

  if ( !useCustomLogic )
    return BinkWait(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth);
  bink = (float)(int)cinematicGlob.bink;
  return (unsigned int)(__int64)((double)(int)(Sys_Milliseconds() + 16 - *(unsigned int *)&cinematicGlob.usingAlpha)
                               * *(float *)&cinematicGlob.fileIoState
                               / 1000.0
                               + bink) < *(unsigned int *)(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth + 12);
}

void R_Cinematic_StopPlayback_Now()
{
  SND_NotifyCinematicEnd();
  *(unsigned int *)&cinematicGlob.textureSets[36] = -1;
  if ( cinematicGlob.binkTextureSet.bink_buffers.YABufferWidth )
    R_Cinematic_RelinquishIO();
  cinematicGlob.isPreloaded = 0;
  BinkClose(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth);
  cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth = 0;
  if ( cinematicGlob.framesStopped )
    CinematicHunk_Reset((CinematicHunk *)&cinematicGlob.framesStopped);
}

void __cdecl CinematicHunk_Reset(CinematicHunk *hunk)
{
  if ( !hunk->base
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 560, 0, "%s", "hunk->base") )
  {
    __debugbreak();
  }
  if ( !hunk->atFront
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 561, 0, "%s", "hunk->atFront") )
  {
    __debugbreak();
  }
  if ( !hunk->atBack
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 562, 0, "%s", "hunk->atBack") )
  {
    __debugbreak();
  }
  if ( !hunk->end
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 563, 0, "%s", "hunk->end") )
  {
    __debugbreak();
  }
  hunk->atFront = hunk->base;
  hunk->atBack = hunk->end;
  hunk->lastAllocPtr = 0;
  hunk->fragmented = 0;
}

char __cdecl R_Cinematic_StartPlayback_Now(const char *filename, char playbackFlags)
{
  unsigned int TrackIDsToPlay[5]; // [esp+1Ch] [ebp-9Ch] BYREF
  char errText[132]; // [esp+30h] [ebp-88h] BYREF

  TrackIDsToPlay[0] = 0;
  TrackIDsToPlay[1] = 1;
  TrackIDsToPlay[2] = 2;
  TrackIDsToPlay[3] = 3;
  TrackIDsToPlay[4] = 4;
  if ( cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
          2641,
          0,
          "%s",
          "!cinematicGlob.bink") )
  {
    __debugbreak();
  }
  *(unsigned int *)&cinematicGlob.textureSets[36] = -1;
  activeTexture ^= 1u;
  cinematicGlob.firstFrameNotify = 0;
  cinematicGlob.lastFrameNotify = 0;
  if ( R_Cinematic_AreHunksOpen() )
  {
    R_Cinematic_HunksReset(activeTexture, playbackFlags);
  }
  else if ( !R_Cinematic_HunksOpen(activeTexture, playbackFlags) )
  {
    activeTexture ^= 1u;
    cinematicGlob.targetCinematicChanged = 1;
    return 0;
  }
  SND_NotifyCinematicStart(*(float *)&dword_ADD7B34);
  if ( !CinematicHunk_IsEmpty((CinematicHunk *)&cinematicGlob.framesStopped)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
          2666,
          0,
          "%s",
          "CinematicHunk_IsEmpty( &cinematicGlob.binkHunk )") )
  {
    __debugbreak();
  }
  R_Cinematic_CheckBinkError();
  BinkSetMemory(R_Cinematic_Bink_Alloc, R_Cinematic_Bink_Free);
  R_Cinematic_CheckBinkError();
  if ( g_cinematicDS )
    BinkSetSoundSystem(BinkOpenDirectSound, g_cinematicDS);
  R_Cinematic_CheckBinkError();
  R_Cinematic_CheckBinkError();
  BinkSetSoundTrack(5, TrackIDsToPlay);
  R_Cinematic_CheckBinkError();
  cinematicGlob.timeInMsec = 0;
  errText[0] = 0;
  if ( R_Cinematic_BinkOpen(filename, playbackFlags, errText, 0x80u)
    || (Com_PrintWarning(8, "R_Cinematic_BinkOpen '%s' failed: %s; trying default.\n", filename, errText),
        CinematicHunk_Reset((CinematicHunk *)&cinematicGlob.framesStopped),
        errText[0] = 0,
        R_Cinematic_BinkOpen("default", playbackFlags, errText, 0x80u)) )
  {
    if ( Sys_IsRenderThread() )
    {
      R_Cinematic_BlackRendererImages();
    }
    else
    {
      RB_Resource_Callback(R_Cinematic_BlackRendererImages);
      RB_Resource_Flush();
    }
    R_Cinematic_CheckBinkError();
    if ( !cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
            2746,
            0,
            "%s",
            "cinematicGlob.bink") )
    {
      __debugbreak();
    }
    R_Cinematic_InitBinkVolumes();
    memset((unsigned __int8 *)&cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferHeight, 0, 0xD8u);
    BinkGetFrameBuffersInfo(
      cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth,
      &cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferHeight);
    R_Cinematic_CheckBinkError();
    R_Cinematic_InitBinkTextures();
    BinkRegisterFrameBuffers(
      cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth,
      &cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferHeight);
    R_Cinematic_CheckBinkError();
    *(unsigned int *)&cinematicGlob.textureSets[44] = 0;
    return 1;
  }
  else
  {
    Com_PrintWarning(8, "R_Cinematic_BinkOpen '%s' failed: %s; not playing movie.\n", "default", errText);
    if ( *(unsigned int *)&cinematicGlob.textureSets[36] != -1
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
            2701,
            0,
            "%s",
            "cinematicGlob.activeImageFrame == CINEMATIC_INVALID_IMAGE_FRAME") )
    {
      __debugbreak();
    }
    CinematicHunk_Reset((CinematicHunk *)&cinematicGlob.framesStopped);
    R_Cinematic_BlackRendererImages();
    return 0;
  }
}

bool __cdecl CinematicHunk_IsEmpty(CinematicHunk *hunk)
{
  if ( !hunk->base
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 574, 0, "%s", "hunk->base") )
  {
    __debugbreak();
  }
  if ( !hunk->atFront
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 575, 0, "%s", "hunk->atFront") )
  {
    __debugbreak();
  }
  if ( !hunk->atBack
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 576, 0, "%s", "hunk->atBack") )
  {
    __debugbreak();
  }
  if ( !hunk->end
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 577, 0, "%s", "hunk->end") )
  {
    __debugbreak();
  }
  return hunk->atFront == hunk->base && hunk->atBack == hunk->end;
}

char __cdecl R_Cinematic_HunksOpen(int activeTexture, char playbackFlags)
{
  CinematicHunk_Open(
    (CinematicHunk *)&cinematicGlob.residentHunk.atFront,
    (char *)ptr,
    (int)&cls.unrankedServers[3021].gameType[11]);
  R_Cinematic_HunksAllocate(activeTexture, playbackFlags);
  return 1;
}

void __cdecl CinematicHunk_Open(CinematicHunk *hunk, char *memory, int size)
{
  if ( hunk->base
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 510, 0, "%s", "!hunk->base") )
  {
    __debugbreak();
  }
  if ( hunk->atFront
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 511, 0, "%s", "!hunk->atFront") )
  {
    __debugbreak();
  }
  if ( hunk->atBack
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 512, 0, "%s", "!hunk->atBack") )
  {
    __debugbreak();
  }
  if ( hunk->end
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 513, 0, "%s", "!hunk->end") )
  {
    __debugbreak();
  }
  hunk->base = memory;
  hunk->atFront = memory;
  hunk->end = &memory[size];
  hunk->atBack = hunk->end;
  hunk->lastAllocPtr = 0;
  hunk->fragmented = 0;
}

void __cdecl R_Cinematic_HunksAllocate(int activeTexture, char playbackFlags)
{
  char *residentBufferBase; // [esp+8h] [ebp-1Ch]
  char *newResidentBufferSize; // [esp+14h] [ebp-10h]
  int newBinkBufferSize; // [esp+18h] [ebp-Ch]
  char *binkBufferBase; // [esp+1Ch] [ebp-8h]

  newBinkBufferSize = (int)&cls.unrankedServers[3021].gameType[11];
  if ( (int)&cls.unrankedServers[3021].gameType[11] <= 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
          696,
          0,
          "%s\n\t(newBinkBufferSize) = %i",
          "(newBinkBufferSize > 0)",
          &cls.unrankedServers[3021].gameType[11]) )
  {
    __debugbreak();
  }
  newResidentBufferSize = 0;
  if ( (playbackFlags & 8) != 0 )
  {
    newResidentBufferSize = &cls.rankedServers[18838].game[27];
    if ( (int)&cls.rankedServers[18838].game[27] <= 0
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
            702,
            0,
            "%s\n\t(newResidentBufferSize) = %i",
            "(newResidentBufferSize > 0)",
            newResidentBufferSize) )
    {
      __debugbreak();
    }
    newBinkBufferSize = 1572864;
  }
  binkBufferBase = (char *)CinematicHunk_Alloc((CinematicHunk *)&cinematicGlob.residentHunk.atFront, newBinkBufferSize);
  residentBufferBase = (char *)CinematicHunk_Alloc(
                                 (CinematicHunk *)&cinematicGlob.residentHunk.atFront,
                                 (int)newResidentBufferSize);
  CinematicHunk_Open((CinematicHunk *)&cinematicGlob.framesStopped, binkBufferBase, newBinkBufferSize);
  CinematicHunk_Open((CinematicHunk *)&cinematicGlob.textureSets[12], residentBufferBase, (int)newResidentBufferSize);
}

int __cdecl CinematicHunk_Alloc(CinematicHunk *hunk, int size)
{
  const char *v2; // eax
  char *alloced; // [esp+0h] [ebp-4h]

  if ( !hunk->base
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 593, 0, "%s", "hunk->base") )
  {
    __debugbreak();
  }
  if ( !hunk->atFront
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 594, 0, "%s", "hunk->atFront") )
  {
    __debugbreak();
  }
  if ( !hunk->atBack
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 595, 0, "%s", "hunk->atBack") )
  {
    __debugbreak();
  }
  if ( !hunk->end
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 596, 0, "%s", "hunk->end") )
  {
    __debugbreak();
  }
  if ( size < 0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 597, 0, "%s", "size >= 0") )
  {
    __debugbreak();
  }
  alloced = (char *)hunk->atFront;
  hunk->atFront = &alloced[size];
  if ( hunk->atFront <= hunk->atBack )
  {
    if ( hunk->fragmented )
    {
      Com_PrintWarning(0, "Bink allocating from fragmented memory at 0x%x.\n", hunk->atFront);
      hunk->fragmented = 0;
    }
    if ( hunk->atFront > hunk->atBack
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
            613,
            0,
            "%s",
            "hunk->atFront <= hunk->atBack") )
    {
      __debugbreak();
    }
    hunk->lastAllocPtr = alloced;
    return (int)alloced;
  }
  else
  {
    v2 = va("CinematicHunk_Alloc failed: 0x%08x 0x%08x 0x%08x\n", hunk->atFront, size, hunk->atBack);
    if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 603, 0, v2) )
      __debugbreak();
    return -1;
  }
}

void __cdecl R_Cinematic_HunksReset(int activeTexture, char playbackFlags)
{
  CinematicHunk_Close((CinematicHunk *)&cinematicGlob.framesStopped);
  CinematicHunk_Close((CinematicHunk *)&cinematicGlob.textureSets[12]);
  CinematicHunk_Reset((CinematicHunk *)&cinematicGlob.residentHunk.atFront);
  R_Cinematic_HunksAllocate(activeTexture, playbackFlags);
}

CinematicTextureSet *R_Cinematic_InitBinkTextures()
{
  BINKFRAMETEXTURES *textures; // [esp+0h] [ebp-10h]
  BINKFRAMETEXTURES *texturesa; // [esp+0h] [ebp-10h]
  int frameIter; // [esp+8h] [ebp-8h]
  int frameItera; // [esp+8h] [ebp-8h]
  CinematicTextureSet *textureSet; // [esp+Ch] [ebp-4h]

  textureSet = (CinematicTextureSet *)(656 * activeTexture + 182285816);
  R_Cinematic_ReleaseImages(textureSet);
  if ( cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferHeight != 2
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
          1214,
          0,
          "%s",
          "cinematicGlob.binkTextureSet.bink_buffers.TotalFrames == CINEMATIC_IMAGES_REQUIRED") )
  {
    __debugbreak();
  }
  for ( frameIter = 0; frameIter != 2; ++frameIter )
  {
    textures = (BINKFRAMETEXTURES *)(32 * frameIter + 182285596);
    textures->Ytexture = R_Cinematic_MakeBinkTexture_PC(
                           &textureSet->imageY[frameIter],
                           cinematicGlob.binkTextureSet.bink_buffers.FrameNum,
                           cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.Allocate,
                           0x40000);
    textures->cRtexture = R_Cinematic_MakeBinkTexture_PC(
                            &textureSet->imageCr[frameIter],
                            (unsigned int)cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.Buffer,
                            cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.BufferPitch,
                            0x40000);
    textures->cBtexture = R_Cinematic_MakeBinkTexture_PC(
                            &textureSet->imageCb[frameIter],
                            (unsigned int)cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.Buffer,
                            cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.BufferPitch,
                            0x40000);
    if ( cinematicGlob.binkTextureSet.bink_buffers.Frames[frameIter + 1].YPlane.Buffer )
      cinematicGlob.binkTextureSet.textures[frameIter + 1].Asize = (unsigned int)R_Cinematic_MakeBinkTexture_PC(
                                                                                   &textureSet->imageA[frameIter],
                                                                                   cinematicGlob.binkTextureSet.bink_buffers.FrameNum,
                                                                                   cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.Allocate,
                                                                                   0x40000);
    else
      cinematicGlob.binkTextureSet.textures[frameIter + 1].Asize = 0;
    cinematicGlob.residentHunk.base = (void *)cinematicGlob.binkTextureSet.bink_buffers.FrameNum;
    textureSet->Ytexture[frameIter] = Z_VirtualAlloc(
                                        cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.Allocate
                                      * cinematicGlob.binkTextureSet.bink_buffers.FrameNum,
                                        "BinkBuffers",
                                        23);
    *((unsigned int *)&cinematicGlob.masterHunk.lastAllocPtr + frameIter) = textureSet->Ytexture[frameIter];
    textureSet->cRtexture[frameIter] = Z_VirtualAlloc(
                                         cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.BufferPitch
                                       * (unsigned int)cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.Buffer,
                                         "BinkBuffers",
                                         23);
    *((unsigned int *)&cinematicGlob.binkHunk.base + frameIter) = textureSet->cRtexture[frameIter];
    textureSet->cBtexture[frameIter] = Z_VirtualAlloc(
                                         cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.BufferPitch
                                       * (unsigned int)cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.Buffer,
                                         "BinkBuffers",
                                         23);
    *((unsigned int *)&cinematicGlob.binkHunk.atBack + frameIter) = textureSet->cBtexture[frameIter];
    if ( cinematicGlob.binkTextureSet.bink_buffers.Frames[frameIter + 1].YPlane.Buffer )
    {
      textureSet->Atexture[frameIter] = Z_VirtualAlloc(
                                          cinematicGlob.binkTextureSet.bink_buffers.Frames[0].YPlane.Allocate
                                        * cinematicGlob.binkTextureSet.bink_buffers.FrameNum,
                                          "BinkBuffers",
                                          23);
      *((unsigned int *)&cinematicGlob.binkHunk.lastAllocPtr + frameIter) = textureSet->Atexture[frameIter];
    }
    else
    {
      textureSet->Atexture[frameIter] = 0;
      *((unsigned int *)&cinematicGlob.binkHunk.lastAllocPtr + frameIter) = 0;
    }
  }
  if ( !Sys_IsRenderThread() )
  {
    RB_Resource_Flush();
    for ( frameItera = 0; frameItera != 2; ++frameItera )
    {
      texturesa = (BINKFRAMETEXTURES *)(32 * frameItera + 182285596);
      cinematicGlob.binkTextureSet.textures[frameItera + 1].Ysize = (unsigned int)textureSet->imageY[frameItera].texture.basemap;
      texturesa->cRtexture = textureSet->imageCr[frameItera].texture.map;
      texturesa->cBtexture = textureSet->imageCb[frameItera].texture.map;
      if ( cinematicGlob.binkTextureSet.bink_buffers.Frames[frameItera + 1].YPlane.Buffer )
        cinematicGlob.binkTextureSet.textures[frameItera + 1].Asize = (unsigned int)textureSet->imageA[frameItera].texture.basemap;
      else
        cinematicGlob.binkTextureSet.textures[frameItera + 1].Asize = 0;
    }
  }
  return R_Cinematic_MakeBinkDrawTextures();
}

void __cdecl R_Cinematic_BlackRendererImages()
{
  if ( Sys_IsRenderThread() )
  {
    RB_UnbindAllImages();
  }
  else
  {
    RB_Resource_Callback(RB_UnbindAllImages);
    RB_Resource_Flush();
  }
  gfxCmdBufInput.codeImages[23] = rgp.blackImage;
  gfxCmdBufInput.codeImages[24] = rgp.grayImage;
  gfxCmdBufInput.codeImages[25] = rgp.grayImage;
  gfxCmdBufInput.codeImages[26] = rgp.blackImage;
  if ( !rgp.grayImage
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
          1297,
          0,
          "%s",
          "gfxCmdBufInput.codeImages[TEXTURE_SRC_CODE_CINEMATIC_CR]") )
  {
    __debugbreak();
  }
  if ( !gfxCmdBufInput.codeImages[25]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
          1298,
          0,
          "%s",
          "gfxCmdBufInput.codeImages[TEXTURE_SRC_CODE_CINEMATIC_CB]") )
  {
    __debugbreak();
  }
  if ( !gfxCmdBufInput.codeImages[26]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
          1299,
          0,
          "%s",
          "gfxCmdBufInput.codeImages[TEXTURE_SRC_CODE_CINEMATIC_A]") )
  {
    __debugbreak();
  }
  if ( !gfxCmdBufInput.codeImages[23]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
          1300,
          0,
          "%s",
          "gfxCmdBufInput.codeImages[TEXTURE_SRC_CODE_CINEMATIC_Y]") )
  {
    __debugbreak();
  }
}

void *__stdcall R_Cinematic_Bink_Alloc(unsigned int bytes)
{
  return malloc(bytes);
}

void __stdcall R_Cinematic_Bink_Free(void *ptr)
{
  char v1; // [esp+3h] [ebp-1h]

  if ( *(void **)&cinematicGlob.textureSets[4] == ptr )
  {
    cinematicGlob.currentPaused = *(unsigned int *)&cinematicGlob.textureSets[4];
    *(unsigned int *)&cinematicGlob.textureSets[4] = 0;
    v1 = 1;
  }
  else
  {
    v1 = 0;
  }
  if ( !v1 )
    cinematicGlob.textureSets[8] = 1;
}

bool __cdecl R_Cinematic_BinkOpen(const char *filename, char playbackFlags, char *errText, unsigned int errTextSize)
{
  char *cwd; // [esp+4h] [ebp-20Ch]
  char filepath[2][256]; // [esp+8h] [ebp-208h] BYREF

  cwd = Sys_Cwd();
  if ( !cwd && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 2479, 0, "%s", "cwd") )
    __debugbreak();
  filepath[1][0] = 0;
  if ( (playbackFlags & 8) != 0 )
  {
    _snprintf(filepath[0], 0x100u, "bik/%s.%s", filename, "bik");
  }
  else
  {
    _snprintf(filepath[0], 0x100u, "%s\\main\\video\\%s.%s", cwd, filename, "bik");
    _snprintf(filepath[1], 0x100u, "%s\\raw\\video\\%s.%s", cwd, filename, "bik");
  }
  if ( R_Cinematic_BinkOpenPath(filepath[0], playbackFlags, errText, errTextSize) )
    return 1;
  return filepath[1][0] && R_Cinematic_BinkOpenPath(filepath[1], playbackFlags, errText, errTextSize);
}

char __cdecl R_Cinematic_BinkOpenPath(char *filepath, char playbackFlags, char *errText, unsigned int errTextSize)
{
  const char *Error; // eax
  const char *v6; // eax
  RawFile *rawfile; // [esp+54h] [ebp-Ch]

  if ( (playbackFlags & 8) != 0 )
  {
    rawfile = DB_FindXAssetHeader(ASSET_TYPE_RAWFILE, filepath, 0, 8000).rawfile;
    if ( !rawfile )
    {
      if ( errText )
        _snprintf(errText, errTextSize, "Couldn't find rawfile '%s' in db", filepath);
      return 0;
    }
    cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth = BinkOpen(rawfile->buffer, 68174848);
    if ( !cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth )
      CinematicHunk_Reset((CinematicHunk *)&cinematicGlob.textureSets[12]);
  }
  else
  {
    if ( CinematicHunk_GetFreeSpace((CinematicHunk *)&cinematicGlob.framesStopped) <= 1572864
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
            2418,
            0,
            "%s",
            "CinematicHunk_GetFreeSpace( &cinematicGlob.binkHunk ) > BINK_MISC_BUFFER_SIZE") )
    {
      __debugbreak();
    }
    cinematicGlob.binkIOSize = CinematicHunk_GetFreeSpace((CinematicHunk *)&cinematicGlob.framesStopped) - 1572864;
    BinkSetIOSize(cinematicGlob.binkIOSize);
    cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth = BinkOpen(filepath, &cls.rankedServers[3546].game[59]);
    if ( !cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth )
    {
      Error = (const char *)BinkGetError();
      Com_Printf(16, "BinkOpen failed on %s because: %s\n", filepath, Error);
    }
    cinematicGlob.isPreloaded = 0;
    if ( cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth )
    {
      cinematicGlob.binkTextureSet.bink_buffers.YABufferWidth = 0;
      BinkPause(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth, 1);
      R_Cinematic_SeizeIO();
      while ( cinematicGlob.binkTextureSet.bink_buffers.YABufferWidth && R_Cinematic_GetPercentageFull() < 0xA )
        NET_Sleep(1u);
      cinematicGlob.isPreloaded = 1;
      BinkPause(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth, 0);
      while ( BinkWait(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth) )
        NET_Sleep(1u);
    }
  }
  if ( cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth )
  {
    *(float *)&cinematicGlob.fileIoState = (double)*(unsigned int *)(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth
                                                                   + 20)
                                         / (double)*(unsigned int *)(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth
                                                                   + 24);
    return 1;
  }
  else
  {
    if ( errText )
    {
      v6 = (const char *)BinkGetError();
      _snprintf(errText, errTextSize, "BinkOpen: %s", v6);
    }
    return 0;
  }
}

int __cdecl CinematicHunk_GetFreeSpace(CinematicHunk *hunk)
{
  if ( !hunk->base
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 583, 0, "%s", "hunk->base") )
  {
    __debugbreak();
  }
  if ( !hunk->atFront
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 584, 0, "%s", "hunk->atFront") )
  {
    __debugbreak();
  }
  if ( !hunk->atBack
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 585, 0, "%s", "hunk->atBack") )
  {
    __debugbreak();
  }
  if ( !hunk->end
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 586, 0, "%s", "hunk->end") )
  {
    __debugbreak();
  }
  return (char *)hunk->atBack - (char *)hunk->atFront;
}

void __cdecl R_Cinematic_InitBinkVolumes()
{
  int v0; // [esp+0h] [ebp-20h]
  int v1; // [esp+14h] [ebp-Ch]

  if ( (int)(__int64)(32768.0 * *(float *)&dword_ADD7B34 * 0.70700002) < 0x8000 )
    v1 = (__int64)(32768.0 * *(float *)&dword_ADD7B34 * 0.70700002);
  else
    v1 = 0x8000;
  if ( v1 > 0 )
    v0 = v1;
  else
    v0 = 0;
  BinkSetVolume(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth, 0, v0);
  BinkSetVolume(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth, 1, v0);
  BinkSetVolume(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth, 2, 0);
  BinkSetVolume(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth, 3, 0);
  BinkSetVolume(cinematicGlob.binkTextureSet.bink_buffers.cRcBBufferWidth, 4, 0);
}

void __cdecl R_Cinematic_UpdateFrame(bool force_wait)
{
  CINE_JQ_CMD cmd; // [esp+14h] [ebp-8h] BYREF

  PIXBeginNamedEvent(-1, "R_Cinematic_UpdateFrame");
  if ( g_cinematicInitialized )
  {
    do
    {
      if ( !_InterlockedExchange(&cineThreadBlock, 1) )
      {
        force_wait = 0;
        R_Cinematic_UpdateRendererImages();
        Sys_AddWorkerCmdInternal(&_UpdateFrameWorkerCmd, (unsigned __int8 *)&cmd, 0);
      }
    }
    while ( force_wait );
    if ( GetCurrentThreadId() == g_DXDeviceThread )
      D3DPERF_EndEvent();
  }
  else if ( g_DXDeviceThread == GetCurrentThreadId() )
  {
    D3DPERF_EndEvent();
  }
}

void R_Cinematic_UpdateRendererImages()
{
  if ( *(unsigned int *)&cinematicGlob.textureSets[36] == -1 )
    R_Cinematic_BlackRendererImages();
  else
    R_Cinematic_SetRendererImagesToFrame(*(int *)&cinematicGlob.textureSets[36]);
}

void __cdecl R_Cinematic_SetRendererImagesToFrame(int frameToSetTo)
{
  CinematicTextureSet *textureSet; // [esp+0h] [ebp-4h]

  textureSet = (CinematicTextureSet *)(656 * dword_ADD7B1C + 182285816);
  if ( frameToSetTo == -1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp",
          1345,
          0,
          "%s",
          "frameToSetTo != CINEMATIC_INVALID_IMAGE_FRAME") )
  {
    __debugbreak();
  }
  if ( Sys_IsRenderThread() )
  {
    RB_UnbindAllImages();
  }
  else
  {
    RB_Resource_Callback(RB_UnbindAllImages);
    RB_Resource_Flush();
  }
  gfxCmdBufInput.codeImages[23] = &textureSet->drawImageY;
  gfxCmdBufInput.codeImages[24] = &textureSet->drawImageCr;
  gfxCmdBufInput.codeImages[25] = &textureSet->drawImageCb;
  if ( textureSet->drawImageA.texture.basemap )
    gfxCmdBufInput.codeImages[26] = &textureSet->drawImageA;
  else
    gfxCmdBufInput.codeImages[26] = rgp.whiteImage;
}

char __cdecl R_Cinematic_GetFilenameAndTimeInMsec(char *outName, int outNameSize, unsigned int *outTimeInMsec)
{
  if ( !outTimeInMsec
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 3248, 0, "%s", "outTimeInMsec") )
  {
    __debugbreak();
  }
  if ( !outName
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_cinematic.cpp", 3249, 0, "%s", "outName") )
  {
    __debugbreak();
  }
  if ( !R_Cinematic_IsStarted() )
    return 0;
  *outTimeInMsec = cinematicGlob.timeInMsec;
  I_strncpyz(outName, cinematicGlob.currentCinematicName, outNameSize);
  return 1;
}

bool __cdecl R_Cinematic_IsFinished()
{
  return cinematicGlob.cinematicFinished;
}

bool __cdecl R_Cinematic_IsStarted()
{
  return !R_Cinematic_IsFinished() && cinematicGlob.currentCinematicName[0];
}

bool __cdecl R_Cinematic_IsNextReady()
{
  return R_Cinematic_IsNextReady_Internal();
}

char __cdecl R_Cinematic_IsUnderrun()
{
  return byte_ADD7B38;
}

void __cdecl R_Cinematic_ForceRelinquishIO()
{
  if ( cinematicGlob.binkTextureSet.bink_buffers.YABufferWidth )
    R_Cinematic_RelinquishIO();
}

