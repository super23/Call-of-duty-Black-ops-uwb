#include "yuv.h"

void __cdecl yuv_init_internal()
{
  const char *v0; // eax
  const char *v1; // eax
  const char *v2; // eax
  const char *v3; // eax
  const char *v4; // eax
  const char *v5; // eax
  const char *v6; // eax
  const char *v7; // eax
  const char *v8; // eax
  const char *v9; // eax
  int v10; // [esp+Ch] [ebp-78h]
  int v11; // [esp+10h] [ebp-74h]
  int v12; // [esp+14h] [ebp-70h]
  int v13; // [esp+18h] [ebp-6Ch]
  int v14; // [esp+1Ch] [ebp-68h]
  int v15; // [esp+20h] [ebp-64h]
  int v16; // [esp+24h] [ebp-60h]
  int v17; // [esp+28h] [ebp-5Ch]
  int v18; // [esp+2Ch] [ebp-58h]
  int v19; // [esp+30h] [ebp-54h]
  int v20; // [esp+34h] [ebp-50h]
  int v21; // [esp+38h] [ebp-4Ch]
  int v22; // [esp+3Ch] [ebp-48h]
  int v23; // [esp+40h] [ebp-44h]
  int v24; // [esp+44h] [ebp-40h]
  int v25; // [esp+48h] [ebp-3Ch]
  int v26; // [esp+4Ch] [ebp-38h]
  int v27; // [esp+50h] [ebp-34h]
  int v28; // [esp+54h] [ebp-30h]
  int hr; // [esp+58h] [ebp-2Ch]
  _D3DVERTEXELEMENT9 VertexElements[3]; // [esp+5Ch] [ebp-28h] BYREF
  int width; // [esp+74h] [ebp-10h]
  int height; // [esp+78h] [ebp-Ch]
  float *vtx; // [esp+7Ch] [ebp-8h] BYREF
  int semaphore; // [esp+80h] [ebp-4h]

  width = yuv_globals.width;
  height = yuv_globals.height;
  semaphore = R_AcquireDXDeviceOwnership(0);
  yuv_globals.outY = (unsigned __int8 *)operator new[](width * (height + 16));
  memset(yuv_globals.outY, 0, width * (height + 16));
  yuv_globals.outU = (unsigned __int8 *)operator new[](width * (height + 16) / 4);
  memset(yuv_globals.outU, 0, width * (height + 16) / 4);
  yuv_globals.outV = (unsigned __int8 *)operator new[](width * (height + 16) / 4);
  memset(yuv_globals.outV, 0, width * (height + 16) / 4);
  R_AssertDXDeviceOwnership();
  if ( r_logFile && r_logFile->current.integer )
    RB_LogPrint(
      "dx.device->CreateTexture(width, height, 1, (0x00000001L), D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &yuv_globals.renderTarget1, 0)\n");
  v28 = R_AcquireDXDeviceOwnership(0);
  hr = dx.device->CreateTexture(
         dx.device,
         width,
         height,
         1u,
         1u,
         D3DFMT_A8R8G8B8,
         D3DPOOL_DEFAULT,
         &yuv_globals.renderTarget1,
         0);
  if ( v28 )
    R_ReleaseDXDeviceOwnership();
  if ( hr < 0 )
  {
    ++g_disableRendering;
    v0 = R_ErrorDescription(hr);
    Com_Error(
      ERR_FATAL,
      "C:\\projects_pc\\cod\\codsrc\\src\\mjpeg\\yuv.cpp (%i) dx.device->CreateTexture(width, height, 1, (0x00000001L), D"
      "3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &yuv_globals.renderTarget1, 0) failed: %s\n",
      45,
      v0);
  }
  if ( yuv_globals.double_buffer )
  {
    R_AssertDXDeviceOwnership();
    if ( r_logFile && r_logFile->current.integer )
      RB_LogPrint(
        "dx.device->CreateTexture(width, height, 1, (0x00000001L), D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &yuv_globals.renderTarget2, 0)\n");
    v26 = R_AcquireDXDeviceOwnership(0);
    v27 = ((int (__thiscall *)(IDirect3DDevice9 *, IDirect3DDevice9 *, int, int, int, int, int, unsigned int, IDirect3DTexture9 **, unsigned int))dx.device->CreateTexture)(
            dx.device,
            dx.device,
            width,
            height,
            1,
            1,
            21,
            0,
            &yuv_globals.renderTarget2,
            0);
    if ( v26 )
      R_ReleaseDXDeviceOwnership();
    if ( v27 < 0 )
    {
      ++g_disableRendering;
      v1 = R_ErrorDescription(v27);
      Com_Error(
        ERR_FATAL,
        "C:\\projects_pc\\cod\\codsrc\\src\\mjpeg\\yuv.cpp (%i) dx.device->CreateTexture(width, height, 1, (0x00000001L),"
        " D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &yuv_globals.renderTarget2, 0) failed: %s\n",
        47,
        v1);
    }
  }
  R_AssertDXDeviceOwnership();
  if ( r_logFile && r_logFile->current.integer )
    RB_LogPrint(
      "dx.device->CreateTexture(width, height, 1, (0x00000001L), D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &yuv_globals.srcTexture, 0)\n");
  v24 = R_AcquireDXDeviceOwnership(0);
  v25 = dx.device->CreateTexture(
          dx.device,
          width,
          height,
          1u,
          1u,
          D3DFMT_A8R8G8B8,
          D3DPOOL_DEFAULT,
          &yuv_globals.srcTexture,
          0);
  if ( v24 )
    R_ReleaseDXDeviceOwnership();
  if ( v25 < 0 )
  {
    ++g_disableRendering;
    v2 = R_ErrorDescription(v25);
    Com_Error(
      ERR_FATAL,
      "C:\\projects_pc\\cod\\codsrc\\src\\mjpeg\\yuv.cpp (%i) dx.device->CreateTexture(width, height, 1, (0x00000001L), D"
      "3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &yuv_globals.srcTexture, 0) failed: %s\n",
      48,
      v2);
  }
  R_AssertDXDeviceOwnership();
  if ( r_logFile && r_logFile->current.integer )
    RB_LogPrint("dx.device->CreateTexture(width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &yuv_globals.dstTexture, 0)\n");
  v22 = R_AcquireDXDeviceOwnership(0);
  v23 = dx.device->CreateTexture(
          dx.device,
          width,
          height,
          1u,
          0,
          D3DFMT_A8R8G8B8,
          D3DPOOL_SYSTEMMEM,
          &yuv_globals.dstTexture,
          0);
  if ( v22 )
    R_ReleaseDXDeviceOwnership();
  if ( v23 < 0 )
  {
    ++g_disableRendering;
    v3 = R_ErrorDescription(v23);
    Com_Error(
      ERR_FATAL,
      "C:\\projects_pc\\cod\\codsrc\\src\\mjpeg\\yuv.cpp (%i) dx.device->CreateTexture(width, height, 1, 0, D3DFMT_A8R8G8"
      "B8, D3DPOOL_SYSTEMMEM, &yuv_globals.dstTexture, 0) failed: %s\n",
      49,
      v3);
  }
  R_AssertDXDeviceOwnership();
  if ( r_logFile && r_logFile->current.integer )
    RB_LogPrint("dx.device->CreateVertexBuffer(24 * 4, 0, 0, D3DPOOL_MANAGED, &yuv_globals.vertexBuffer, 0)\n");
  v20 = R_AcquireDXDeviceOwnership(0);
  v21 = dx.device->CreateVertexBuffer(dx.device, 96u, 0, 0, D3DPOOL_MANAGED, &yuv_globals.vertexBuffer, 0);
  if ( v20 )
    R_ReleaseDXDeviceOwnership();
  if ( v21 < 0 )
  {
    ++g_disableRendering;
    v4 = R_ErrorDescription(v21);
    Com_Error(
      ERR_FATAL,
      "C:\\projects_pc\\cod\\codsrc\\src\\mjpeg\\yuv.cpp (%i) dx.device->CreateVertexBuffer(24 * 4, 0, 0, D3DPOOL_MANAGED"
      ", &yuv_globals.vertexBuffer, 0) failed: %s\n",
      50,
      v4);
  }
  R_AssertDXDeviceOwnership();
  if ( r_logFile && r_logFile->current.integer )
    RB_LogPrint("yuv_globals.vertexBuffer->Lock(0, 0, (void**)&vtx, 0)\n");
  v18 = R_AcquireDXDeviceOwnership(0);
  v19 = yuv_globals.vertexBuffer->Lock(yuv_globals.vertexBuffer, 0, 0, (void **)&vtx, 0);
  if ( v18 )
    R_ReleaseDXDeviceOwnership();
  if ( v19 < 0 )
  {
    ++g_disableRendering;
    v5 = R_ErrorDescription(v19);
    Com_Error(
      ERR_FATAL,
      "C:\\projects_pc\\cod\\codsrc\\src\\mjpeg\\yuv.cpp (%i) yuv_globals.vertexBuffer->Lock(0, 0, (void**)&vtx, 0) failed: %s\n",
      53,
      v5);
  }
  *vtx = FLOAT_N1_0;
  vtx[1] = FLOAT_N1_0;
  vtx[2] = *(float *)&FLOAT_0_0;
  vtx[3] = FLOAT_1_0;
  vtx[4] = *(float *)&FLOAT_0_0;
  vtx[5] = FLOAT_1_0;
  vtx[6] = FLOAT_1_0;
  vtx[7] = FLOAT_N1_0;
  vtx[8] = *(float *)&FLOAT_0_0;
  vtx[9] = FLOAT_1_0;
  vtx[10] = FLOAT_1_0;
  vtx[11] = FLOAT_1_0;
  vtx[12] = FLOAT_1_0;
  vtx[13] = FLOAT_1_0;
  vtx[14] = *(float *)&FLOAT_0_0;
  vtx[15] = FLOAT_1_0;
  vtx[16] = FLOAT_1_0;
  vtx[17] = *(float *)&FLOAT_0_0;
  vtx[18] = FLOAT_N1_0;
  vtx[19] = FLOAT_1_0;
  vtx[20] = *(float *)&FLOAT_0_0;
  vtx[21] = FLOAT_1_0;
  vtx[22] = *(float *)&FLOAT_0_0;
  vtx[23] = *(float *)&FLOAT_0_0;
  R_AssertDXDeviceOwnership();
  if ( r_logFile && r_logFile->current.integer )
    RB_LogPrint("yuv_globals.vertexBuffer->Unlock()\n");
  v16 = R_AcquireDXDeviceOwnership(0);
  v17 = ((int (__thiscall *)(IDirect3DVertexBuffer9 *, IDirect3DVertexBuffer9 *))yuv_globals.vertexBuffer->Unlock)(
          yuv_globals.vertexBuffer,
          yuv_globals.vertexBuffer);
  if ( v16 )
    R_ReleaseDXDeviceOwnership();
  if ( v17 < 0 )
  {
    ++g_disableRendering;
    v6 = R_ErrorDescription(v17);
    Com_Error(
      ERR_FATAL,
      "C:\\projects_pc\\cod\\codsrc\\src\\mjpeg\\yuv.cpp (%i) yuv_globals.vertexBuffer->Unlock() failed: %s\n",
      58,
      v6);
  }
  VertexElements[0].Stream = 0;
  VertexElements[0].Offset = 0;
  VertexElements[0].Type = 3;
  VertexElements[0].Method = 0;
  VertexElements[0].Usage = 0;
  VertexElements[0].UsageIndex = 0;
  VertexElements[1].Stream = 0;
  VertexElements[1].Offset = 16;
  VertexElements[1].Type = 1;
  VertexElements[1].Method = 0;
  VertexElements[1].Usage = 5;
  VertexElements[1].UsageIndex = 0;
  VertexElements[2].Stream = 255;
  VertexElements[2].Offset = 0;
  VertexElements[2].Type = 17;
  VertexElements[2].Method = 0;
  VertexElements[2].Usage = 0;
  VertexElements[2].UsageIndex = 0;
  R_AssertDXDeviceOwnership();
  if ( r_logFile && r_logFile->current.integer )
    RB_LogPrint("dx.device->CreateVertexDeclaration(VertexElements, &yuv_globals.vertexDeclaration)\n");
  v14 = R_AcquireDXDeviceOwnership(0);
  v15 = ((int (__thiscall *)(IDirect3DDevice9 *, IDirect3DDevice9 *, _D3DVERTEXELEMENT9 *, IDirect3DVertexDeclaration9 **))dx.device->CreateVertexDeclaration)(
          dx.device,
          dx.device,
          VertexElements,
          &yuv_globals.vertexDeclaration);
  if ( v14 )
    R_ReleaseDXDeviceOwnership();
  if ( v15 < 0 )
  {
    ++g_disableRendering;
    v7 = R_ErrorDescription(v15);
    Com_Error(
      ERR_FATAL,
      "C:\\projects_pc\\cod\\codsrc\\src\\mjpeg\\yuv.cpp (%i) dx.device->CreateVertexDeclaration(VertexElements, &yuv_glo"
      "bals.vertexDeclaration) failed: %s\n",
      67,
      v7);
  }
  R_AssertDXDeviceOwnership();
  if ( r_logFile && r_logFile->current.integer )
    RB_LogPrint("dx.device->CreateVertexShader((DWORD*)g_vs30_mjpeg_shader_vtx, &yuv_globals.vertexShader)\n");
  v12 = R_AcquireDXDeviceOwnership(0);
  v13 = dx.device->CreateVertexShader(
          dx.device,
          (const unsigned int *)g_vs30_mjpeg_shader_vtx,
          &yuv_globals.vertexShader);
  if ( v12 )
    R_ReleaseDXDeviceOwnership();
  if ( v13 < 0 )
  {
    ++g_disableRendering;
    v8 = R_ErrorDescription(v13);
    Com_Error(
      ERR_FATAL,
      "C:\\projects_pc\\cod\\codsrc\\src\\mjpeg\\yuv.cpp (%i) dx.device->CreateVertexShader((DWORD*)g_vs30_mjpeg_shader_v"
      "tx, &yuv_globals.vertexShader) failed: %s\n",
      68,
      v8);
  }
  R_AssertDXDeviceOwnership();
  if ( r_logFile && r_logFile->current.integer )
    RB_LogPrint("dx.device->CreatePixelShader((DWORD*)g_ps30_mjpeg_shader_yuv, &yuv_globals.pixelShader)\n");
  v10 = R_AcquireDXDeviceOwnership(0);
  v11 = dx.device->CreatePixelShader(dx.device, (const unsigned int *)g_ps30_mjpeg_shader_yuv, &yuv_globals.pixelShader);
  if ( v10 )
    R_ReleaseDXDeviceOwnership();
  if ( v11 < 0 )
  {
    ++g_disableRendering;
    v9 = R_ErrorDescription(v11);
    Com_Error(
      ERR_FATAL,
      "C:\\projects_pc\\cod\\codsrc\\src\\mjpeg\\yuv.cpp (%i) dx.device->CreatePixelShader((DWORD*)g_ps30_mjpeg_shader_yu"
      "v, &yuv_globals.pixelShader) failed: %s\n",
      69,
      v9);
  }
  if ( semaphore )
    R_ReleaseDXDeviceOwnership();
  yuv_globals.init = 1;
}

void __cdecl yuv_init(int width, int height, bool double_buffer)
{
  if ( !yuv_globals.init )
  {
    yuv_globals.width = width;
    yuv_globals.height = height;
    yuv_globals.double_buffer = double_buffer;
    yuv_globals.frame = 0;
    if ( Sys_IsRenderThread() )
    {
      yuv_init_internal();
    }
    else
    {
      RB_Resource_Callback(yuv_init_internal);
      RB_Resource_Flush();
    }
  }
}

char __cdecl yuv_encode_frame()
{
  const char *v1; // eax
  IDirect3DTexture9 *renderTarget2; // [esp+0h] [ebp-5Ch]
  IDirect3DTexture9 *renderTarget1; // [esp+4h] [ebp-58h]
  unsigned __int8 *pixel3; // [esp+Ch] [ebp-50h]
  unsigned __int8 *pixel1; // [esp+10h] [ebp-4Ch]
  _D3DLOCKED_RECT lockedRect; // [esp+18h] [ebp-44h] BYREF
  unsigned __int8 *outV; // [esp+20h] [ebp-3Ch]
  int width; // [esp+24h] [ebp-38h]
  int height; // [esp+28h] [ebp-34h]
  unsigned __int8 *outY; // [esp+2Ch] [ebp-30h]
  unsigned __int8 *outU; // [esp+30h] [ebp-2Ch]
  int row; // [esp+34h] [ebp-28h]
  int col; // [esp+38h] [ebp-24h]
  int v14; // [esp+3Ch] [ebp-20h]
  HRESULT hr; // [esp+40h] [ebp-1Ch]
  IDirect3DSurface9 *dstSurface; // [esp+44h] [ebp-18h] BYREF
  IDirect3DSurface9 *imageSurface; // [esp+48h] [ebp-14h] BYREF
  unsigned __int8 *pixels; // [esp+4Ch] [ebp-10h]
  int semaphore; // [esp+50h] [ebp-Ch]
  IDirect3DTexture9 *renderTarget; // [esp+54h] [ebp-8h]
  IDirect3DSurface9 *srcSurface; // [esp+58h] [ebp-4h]

  if ( !yuv_globals.init )
    return 0;
  semaphore = R_AcquireDXDeviceOwnership(0);
  ((void (__thiscall *)(IDirect3DTexture9 *, IDirect3DTexture9 *, unsigned int, IDirect3DSurface9 **))yuv_globals.srcTexture->GetSurfaceLevel)(
    yuv_globals.srcTexture,
    yuv_globals.srcTexture,
    0,
    &dstSurface);
  srcSurface = stru_B50E858.surface.color;
  R_AssertDXDeviceOwnership();
  if ( r_logFile && r_logFile->current.integer )
    RB_LogPrint("dx.device->StretchRect(srcSurface, 0, dstSurface, 0, D3DTEXF_NONE)\n");
  v14 = R_AcquireDXDeviceOwnership(0);
  hr = ((int (__thiscall *)(IDirect3DDevice9 *, IDirect3DDevice9 *, IDirect3DSurface9 *, unsigned int, IDirect3DSurface9 *, unsigned int, unsigned int))dx.device->StretchRect)(
         dx.device,
         dx.device,
         srcSurface,
         0,
         dstSurface,
         0,
         0);
  if ( v14 )
    R_ReleaseDXDeviceOwnership();
  if ( hr < 0 )
  {
    ++g_disableRendering;
    v1 = R_ErrorDescription(hr);
    Com_Error(
      ERR_FATAL,
      "C:\\projects_pc\\cod\\codsrc\\src\\mjpeg\\yuv.cpp (%i) dx.device->StretchRect(srcSurface, 0, dstSurface, 0, D3DTEX"
      "F_NONE) failed: %s\n",
      112,
      v1);
  }
  ((void (__thiscall *)(IDirect3DSurface9 *, IDirect3DSurface9 *))dstSurface->Release)(dstSurface, dstSurface);
  dx.device->SetTexture(dx.device, 0, yuv_globals.srcTexture);
  dx.device->SetVertexShader(dx.device, yuv_globals.vertexShader);
  ((void (__thiscall *)(IDirect3DDevice9 *, IDirect3DDevice9 *, unsigned int, IDirect3DVertexBuffer9 *, unsigned int, int))dx.device->SetStreamSource)(
    dx.device,
    dx.device,
    0,
    yuv_globals.vertexBuffer,
    0,
    24);
  dx.device->SetRenderState(dx.device, D3DRS_ALPHABLENDENABLE, 0);
  dx.device->SetRenderState(dx.device, D3DRS_SEPARATEALPHABLENDENABLE, 0);
  ((void (__thiscall *)(IDirect3DDevice9 *, IDirect3DDevice9 *, int, unsigned int))dx.device->SetRenderState)(
    dx.device,
    dx.device,
    15,
    0);
  dx.device->SetRenderState(dx.device, D3DRS_ZENABLE, 0);
  dx.device->SetRenderState(dx.device, D3DRS_CULLMODE, 1u);
  ((void (__thiscall *)(IDirect3DDevice9 *, IDirect3DDevice9 *, int, unsigned int))dx.device->SetRenderState)(
    dx.device,
    dx.device,
    52,
    0);
  dx.device->SetSamplerState(dx.device, 0, D3DSAMP_ADDRESSU, 3u);
  dx.device->SetSamplerState(dx.device, 0, D3DSAMP_ADDRESSV, 3u);
  dx.device->SetVertexDeclaration(dx.device, yuv_globals.vertexDeclaration);
  if ( yuv_globals.double_buffer )
  {
    if ( (yuv_globals.frame & 1) != 0 )
      renderTarget1 = yuv_globals.renderTarget1;
    else
      renderTarget1 = yuv_globals.renderTarget2;
    renderTarget = renderTarget1;
  }
  else
  {
    renderTarget = yuv_globals.renderTarget1;
  }
  ((void (__thiscall *)(IDirect3DTexture9 *, IDirect3DTexture9 *, unsigned int, IDirect3DSurface9 **))renderTarget->GetSurfaceLevel)(
    renderTarget,
    renderTarget,
    0,
    &imageSurface);
  dx.device->BeginScene(dx.device);
  ((void (__thiscall *)(IDirect3DDevice9 *, IDirect3DDevice9 *, unsigned int, IDirect3DSurface9 *))dx.device->SetRenderTarget)(
    dx.device,
    dx.device,
    0,
    imageSurface);
  dx.device->SetPixelShader(dx.device, yuv_globals.pixelShader);
  ((void (__thiscall *)(IDirect3DDevice9 *, IDirect3DDevice9 *, int, unsigned int, int))dx.device->DrawPrimitive)(
    dx.device,
    dx.device,
    6,
    0,
    2);
  dx.device->EndScene(dx.device);
  imageSurface->Release(imageSurface);
  if ( yuv_globals.double_buffer && !yuv_globals.frame )
  {
    memset(yuv_globals.outY, 0, yuv_globals.height * yuv_globals.width);
    memset(yuv_globals.outU, 0, yuv_globals.height * yuv_globals.width / 4);
    memset(yuv_globals.outV, 0, yuv_globals.height * yuv_globals.width / 4);
  }
  else
  {
    if ( yuv_globals.double_buffer )
    {
      if ( (yuv_globals.frame & 1) != 0 )
        renderTarget2 = yuv_globals.renderTarget2;
      else
        renderTarget2 = yuv_globals.renderTarget1;
      renderTarget = renderTarget2;
    }
    else
    {
      renderTarget = yuv_globals.renderTarget1;
    }
    renderTarget->GetSurfaceLevel(renderTarget, 0, &imageSurface);
    yuv_globals.dstTexture->GetSurfaceLevel(yuv_globals.dstTexture, 0, &dstSurface);
    dx.device->GetRenderTargetData(dx.device, imageSurface, dstSurface);
    imageSurface->Release(imageSurface);
    dstSurface->LockRect(dstSurface, &lockedRect, 0, 16u);
    pixels = (unsigned __int8 *)lockedRect.pBits;
    outY = yuv_globals.outY;
    outU = yuv_globals.outU;
    outV = yuv_globals.outV;
    width = yuv_globals.width;
    height = yuv_globals.height;
    for ( row = 0; row < height; row += 2 )
    {
      for ( col = 0; col < width; col += 2 )
      {
        pixel1 = &pixels[4 * col + 4 * width * row];
        pixel3 = &pixels[4 * col + 4 * width * (row + 1)];
        outY[col + width * row] = pixel1[2];
        outY[col + 1 + width * row] = pixel1[6];
        outY[col + width * (row + 1)] = pixel3[2];
        outY[col + 1 + width * (row + 1)] = pixel3[6];
        *outU++ = pixel3[5] + pixel3[1] + pixel1[5] + pixel1[1];
        *outV++ = pixel3[4] + *pixel3 + pixel1[4] + *pixel1;
      }
    }
    dstSurface->UnlockRect(dstSurface);
    ((void (__thiscall *)(IDirect3DSurface9 *, IDirect3DSurface9 *))dstSurface->Release)(dstSurface, dstSurface);
  }
  R_InitCmdBufState(&gfxCmdBufState);
  if ( semaphore )
    R_ReleaseDXDeviceOwnership();
  ++yuv_globals.frame;
  return 1;
}

void __cdecl yuv_get_buffers(unsigned __int8 **y, unsigned __int8 **u, unsigned __int8 **v)
{
  *y = yuv_globals.outY;
  *u = yuv_globals.outU;
  *v = yuv_globals.outV;
}

void __cdecl yuv_shutdown_internal()
{
  IDirect3DSurface9 *vertexDeclaration; // [esp+Ch] [ebp-20h]
  IDirect3DSurface9 *vertexBuffer; // [esp+10h] [ebp-1Ch]
  IDirect3DSurface9 *vertexShader; // [esp+14h] [ebp-18h]
  IDirect3DSurface9 *pixelShader; // [esp+18h] [ebp-14h]
  IDirect3DSurface9 *dstTexture; // [esp+1Ch] [ebp-10h]
  IDirect3DSurface9 *srcTexture; // [esp+20h] [ebp-Ch]
  IDirect3DSurface9 *var; // [esp+24h] [ebp-8h]
  IDirect3DTexture9 *varCopy; // [esp+28h] [ebp-4h]

  operator delete[](yuv_globals.outY);
  operator delete[](yuv_globals.outU);
  operator delete[](yuv_globals.outV);
  do
  {
    if ( r_logFile && r_logFile->current.integer )
      RB_LogPrint("yuv_globals.renderTarget1->Release()\n");
    varCopy = yuv_globals.renderTarget1;
    yuv_globals.renderTarget1 = 0;
    R_ReleaseAndSetNULL<IDirect3DIndexBuffer9>(
      (IDirect3DSurface9 *)varCopy,
      "yuv_globals.renderTarget1",
      "C:\\projects_pc\\cod\\codsrc\\src\\mjpeg\\yuv.cpp",
      223);
  }
  while ( alwaysfails );
  if ( yuv_globals.double_buffer )
  {
    do
    {
      if ( r_logFile && r_logFile->current.integer )
        RB_LogPrint("yuv_globals.renderTarget2->Release()\n");
      var = (IDirect3DSurface9 *)yuv_globals.renderTarget2;
      yuv_globals.renderTarget2 = 0;
      R_ReleaseAndSetNULL<IDirect3DIndexBuffer9>(
        var,
        "yuv_globals.renderTarget2",
        "C:\\projects_pc\\cod\\codsrc\\src\\mjpeg\\yuv.cpp",
        225);
    }
    while ( alwaysfails );
  }
  do
  {
    if ( r_logFile && r_logFile->current.integer )
      RB_LogPrint("yuv_globals.srcTexture->Release()\n");
    srcTexture = (IDirect3DSurface9 *)yuv_globals.srcTexture;
    yuv_globals.srcTexture = 0;
    R_ReleaseAndSetNULL<IDirect3DIndexBuffer9>(
      srcTexture,
      "yuv_globals.srcTexture",
      "C:\\projects_pc\\cod\\codsrc\\src\\mjpeg\\yuv.cpp",
      226);
  }
  while ( alwaysfails );
  do
  {
    if ( r_logFile && r_logFile->current.integer )
      RB_LogPrint("yuv_globals.dstTexture->Release()\n");
    dstTexture = (IDirect3DSurface9 *)yuv_globals.dstTexture;
    yuv_globals.dstTexture = 0;
    R_ReleaseAndSetNULL<IDirect3DIndexBuffer9>(
      dstTexture,
      "yuv_globals.dstTexture",
      "C:\\projects_pc\\cod\\codsrc\\src\\mjpeg\\yuv.cpp",
      227);
  }
  while ( alwaysfails );
  do
  {
    if ( r_logFile && r_logFile->current.integer )
      RB_LogPrint("yuv_globals.pixelShader->Release()\n");
    pixelShader = (IDirect3DSurface9 *)yuv_globals.pixelShader;
    yuv_globals.pixelShader = 0;
    R_ReleaseAndSetNULL<IDirect3DIndexBuffer9>(
      pixelShader,
      "yuv_globals.pixelShader",
      "C:\\projects_pc\\cod\\codsrc\\src\\mjpeg\\yuv.cpp",
      228);
  }
  while ( alwaysfails );
  do
  {
    if ( r_logFile && r_logFile->current.integer )
      RB_LogPrint("yuv_globals.vertexShader->Release()\n");
    vertexShader = (IDirect3DSurface9 *)yuv_globals.vertexShader;
    yuv_globals.vertexShader = 0;
    R_ReleaseAndSetNULL<IDirect3DIndexBuffer9>(
      vertexShader,
      "yuv_globals.vertexShader",
      "C:\\projects_pc\\cod\\codsrc\\src\\mjpeg\\yuv.cpp",
      229);
  }
  while ( alwaysfails );
  do
  {
    if ( r_logFile && r_logFile->current.integer )
      RB_LogPrint("yuv_globals.vertexBuffer->Release()\n");
    vertexBuffer = (IDirect3DSurface9 *)yuv_globals.vertexBuffer;
    yuv_globals.vertexBuffer = 0;
    R_ReleaseAndSetNULL<IDirect3DIndexBuffer9>(
      vertexBuffer,
      "yuv_globals.vertexBuffer",
      "C:\\projects_pc\\cod\\codsrc\\src\\mjpeg\\yuv.cpp",
      230);
  }
  while ( alwaysfails );
  do
  {
    if ( r_logFile && r_logFile->current.integer )
      RB_LogPrint("yuv_globals.vertexDeclaration->Release()\n");
    vertexDeclaration = (IDirect3DSurface9 *)yuv_globals.vertexDeclaration;
    yuv_globals.vertexDeclaration = 0;
    R_ReleaseAndSetNULL<IDirect3DIndexBuffer9>(
      vertexDeclaration,
      "yuv_globals.vertexDeclaration",
      "C:\\projects_pc\\cod\\codsrc\\src\\mjpeg\\yuv.cpp",
      231);
  }
  while ( alwaysfails );
  yuv_globals.init = 0;
}

void __cdecl yuv_shutdown()
{
  if ( yuv_globals.init )
  {
    if ( Sys_IsRenderThread() )
    {
      yuv_shutdown_internal();
    }
    else
    {
      RB_Resource_Callback(yuv_shutdown_internal);
      RB_Resource_Flush();
    }
  }
}

void __cdecl yuv_lost_device()
{
  if ( yuv_globals.init )
  {
    yuv_shutdown();
    yuv_globals.lost_device = 1;
  }
}

void __cdecl yuv_recover_device()
{
  if ( yuv_globals.lost_device )
  {
    yuv_init(yuv_globals.width, yuv_globals.height, yuv_globals.double_buffer);
    yuv_globals.lost_device = 0;
  }
}

