#include "cl_compositing.h"

void __cdecl CL_CompositeSetupImageCallback(unsigned int *param)
{
  *param = CL_CompositeSetupImage();
}

GfxImage *__cdecl CL_CompositeSetupImage()
{
  return R_StreamAlloc_SetupTempImage(D3DFMT_A8R8G8B8, 0, 0x80u, 0x80u);
}

void __cdecl CL_CompositeReleaseImage(GfxImage *image)
{
  R_StreamAlloc_ReleaseTempImage(image);
}

bool __cdecl CL_CompositeValidateImage(GfxImage *image)
{
  return image && R_StreamImageCheck(image, 0);
}

int __cdecl CL_CompositePushEmblem(CompositeEmblemLayer *layers, int layerCount)
{
  int idx; // [esp+0h] [ebp-8h]
  CompositeJob *job; // [esp+4h] [ebp-4h]

  if ( !Sys_IsMainThread()
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\client\\cl_compositing.cpp",
          115,
          0,
          "%s",
          "Sys_IsMainThread()") )
  {
    __debugbreak();
  }
  for ( idx = 0; idx < 4 && s_jobs[idx].state; ++idx )
    ;
  if ( idx == 4 )
    return 0;
  job = &s_jobs[idx];
  if ( job->state
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\client\\cl_compositing.cpp",
          127,
          0,
          "%s",
          "job->state == COMPOSITE_STATE_IDLE") )
  {
    __debugbreak();
  }
  memset((unsigned __int8 *)job, 0, sizeof(CompositeJob));
  job->type = COMPOSITE_EMBLEM;
  job->state = COMPOSITE_STATE_PRE;
  memcpy((unsigned __int8 *)job->layers, (unsigned __int8 *)layers, 32 * layerCount);
  job->layerCount = layerCount;
  if ( ++s_jobID < 0 )
    s_jobID = 1;
  job->id = s_jobID;
  return s_jobID;
}

char __cdecl CL_CompositePushCancel(int id)
{
  int idx; // [esp+0h] [ebp-4h]

  if ( !Sys_IsMainThread()
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\client\\cl_compositing.cpp",
          152,
          0,
          "%s",
          "Sys_IsMainThread()") )
  {
    __debugbreak();
  }
  for ( idx = 0; idx < 4; ++idx )
  {
    if ( s_jobs[idx].id == id )
    {
      s_jobs[idx].cancel = 1;
      return 1;
    }
  }
  return 0;
}

char __cdecl CL_CompositeIsComplete(int id)
{
  int idx; // [esp+0h] [ebp-4h]

  if ( !Sys_IsMainThread()
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\client\\cl_compositing.cpp",
          170,
          0,
          "%s",
          "Sys_IsMainThread()") )
  {
    __debugbreak();
  }
  for ( idx = 0; idx < 4; ++idx )
  {
    if ( s_jobs[idx].id == id && s_jobs[idx].state == COMPOSITE_STATE_COMPLETE )
      return 1;
  }
  return 0;
}

GfxImage *__cdecl CL_CompositePopImage(int id)
{
  int idx; // [esp+0h] [ebp-4h]

  if ( !Sys_IsMainThread()
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\client\\cl_compositing.cpp",
          185,
          0,
          "%s",
          "Sys_IsMainThread()") )
  {
    __debugbreak();
  }
  for ( idx = 0; idx < 4; ++idx )
  {
    if ( s_jobs[idx].id == id && s_jobs[idx].state == COMPOSITE_STATE_COMPLETE )
    {
      s_jobs[idx].state = COMPOSITE_STATE_IDLE;
      return s_jobs[idx].resultImage;
    }
  }
  return 0;
}

bool __cdecl CL_CompositeUseGenerated(float w, float h)
{
  return w <= 192.0 && h <= 192.0;
}

bool __cdecl CL_CompositeCheckStreaming(CompositeEmblemLayer *layers, int layerCount)
{
  GfxImage *image; // [esp+0h] [ebp-Ch]
  bool valid; // [esp+7h] [ebp-5h]
  int idx; // [esp+8h] [ebp-4h]

  valid = 1;
  for ( idx = 0; idx < layerCount; ++idx )
  {
    image = BG_EmblemsGetIconImage(layers[idx].icon);
    if ( image )
    {
      if ( !R_StreamTouchImageAndCheck(image, 0) )
        valid = 0;
    }
  }
  return valid;
}

void __cdecl CL_CompositeEmblemCallback(GfxImage *result)
{
  int idx; // [esp+Ch] [ebp-4h]

  R_HW_InsertFence(&s_compositingFence);
  for ( idx = 0; idx < 4; ++idx )
  {
    if ( s_jobs[idx].state == COMPOSITE_STATE_RENDERING )
    {
      if ( s_jobs[idx].type != COMPOSITE_EMBLEM
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\client\\cl_compositing.cpp",
              360,
              0,
              "%s",
              "s_jobs[idx].type == COMPOSITE_EMBLEM") )
      {
        __debugbreak();
      }
      s_jobs[idx].state = COMPOSITE_STATE_POST;
      s_jobs[idx].renderImage = result;
      return;
    }
  }
  if ( !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\client\\cl_compositing.cpp",
          367,
          0,
          "emblem render callback without active emblem render") )
    __debugbreak();
}

void __cdecl R_HW_InsertFence(IDirect3DQuery9 **fence)
{
  const char *v1; // eax
  int v2; // [esp+0h] [ebp-Ch]
  int hr; // [esp+4h] [ebp-8h]
  int semaphore; // [esp+8h] [ebp-4h]

  R_AssertDXDeviceOwnership();
  *fence = dx.fencePool[dx.nextFence];
  dx.nextFence = (dx.nextFence + 1) % 8;
  semaphore = R_AcquireDXDeviceOwnership(0);
  R_AssertDXDeviceOwnership();
  if ( r_logFile && r_logFile->current.integer )
    RB_LogPrint("(*fence)->Issue( (1 << 0) )\n");
  v2 = R_AcquireDXDeviceOwnership(0);
  hr = (*fence)->Issue(*fence, 1u);
  if ( v2 )
    R_ReleaseDXDeviceOwnership();
  if ( hr < 0 )
  {
    ++g_disableRendering;
    v1 = R_ErrorDescription(hr);
    Com_Error(
      ERR_FATAL,
      "c:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\r_fence_pc.h (%i) (*fence)->Issue( (1 << 0) ) failed: %s\n",
      17,
      v1);
  }
  if ( semaphore )
    R_ReleaseDXDeviceOwnership();
}

void __cdecl CL_PCCopyImageGenMIPCallback(unsigned int *job)
{
  if ( !job && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\client\\cl_compositing.cpp", 375, 0, "%s", "pJob") )
    __debugbreak();
  *job = 6;
}

void __cdecl CL_CompositeRender()
{
  CompositeState state; // [esp+1Ch] [ebp-1Ch]
  int idx; // [esp+2Ch] [ebp-Ch]
  int idxa; // [esp+2Ch] [ebp-Ch]
  CompositeJob *job; // [esp+30h] [ebp-8h]
  CompositeJob *joba; // [esp+30h] [ebp-8h]
  bool renderBusy; // [esp+37h] [ebp-1h]
  int savedregs; // [esp+38h] [ebp+0h] BYREF

  renderBusy = 0;
  if ( !Sys_IsMainThread() )
  {
    if ( !Sys_IsRenderThread()
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\client\\cl_compositing.cpp",
            395,
            0,
            "%s",
            "Sys_IsRenderThread()") )
    {
      __debugbreak();
    }
    return;
  }
  for ( idx = 0; idx < 4; ++idx )
  {
    job = &s_jobs[idx];
    state = job->state;
    if ( job->state == COMPOSITE_STATE_RENDERING )
    {
      if ( renderBusy
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\client\\cl_compositing.cpp",
              405,
              0,
              "%s",
              "renderBusy == false") )
      {
        __debugbreak();
      }
      renderBusy = 1;
    }
    else
    {
      if ( state == COMPOSITE_STATE_POST )
      {
        if ( renderBusy
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\client\\cl_compositing.cpp",
                410,
                0,
                "%s",
                "renderBusy == false") )
        {
          __debugbreak();
        }
        if ( job->resultImage
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\client\\cl_compositing.cpp",
                412,
                0,
                "%s",
                "job->resultImage == NULL") )
        {
          __debugbreak();
        }
        if ( job->cancel )
        {
          job->state = COMPOSITE_STATE_IDLE;
          continue;
        }
        RB_Resource_CallbackParam((void (__cdecl *)(void *))CL_CompositeSetupImageCallback, &job->resultImage);
        RB_Resource_Flush();
        if ( !job->resultImage )
        {
          renderBusy = 1;
          continue;
        }
        job->state = COMPOSITE_STATE_POST_ALLOCED;
      }
      else if ( state != COMPOSITE_STATE_POST_ALLOCED )
      {
        continue;
      }
      if ( renderBusy
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\client\\cl_compositing.cpp",
              436,
              0,
              "%s",
              "renderBusy == false") )
      {
        __debugbreak();
      }
      renderBusy = 1;
      if ( !R_HW_IsFencePending(&s_compositingFence) )
      {
        if ( !job->renderImage
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\client\\cl_compositing.cpp",
                444,
                0,
                "%s",
                "job->renderImage != NULL") )
        {
          __debugbreak();
        }
        if ( !job->resultImage
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\client\\cl_compositing.cpp",
                445,
                0,
                "%s",
                "job->resultImage != NULL") )
        {
          __debugbreak();
        }
        job->state = COMPOSITE_STATE_PC_WAITING_FOR_GPU;
        R_BeginCompositingCmdList();
        R_AddCmdPCCopyImageGenMIP((void (__cdecl *)(void *))CL_PCCopyImageGenMIPCallback, job->resultImage, job);
        R_AddCmdEndOfList();
      }
    }
  }
  for ( idxa = 0; idxa < 4; ++idxa )
  {
    joba = &s_jobs[idxa];
    if ( joba->state == COMPOSITE_STATE_PRE )
    {
      if ( joba->cancel )
        joba->state = COMPOSITE_STATE_IDLE;
      if ( !renderBusy && joba->type == COMPOSITE_EMBLEM && CL_CompositeCheckStreaming(joba->layers, joba->layerCount) )
      {
        joba->state = COMPOSITE_STATE_RENDERING;
        renderBusy = 1;
        R_BeginCompositingCmdList();
        R_AddCmdProjectionSet2D();
        R_AddCmdClearScreen(1, colorBlackBlank, 1.0, 0);
        CL_CompositeDrawEmblemPhysical(
          (GfxColor)&savedregs,
          0.0,
          0.0,
          256.0,
          256.0,
          colorWhite,
          joba->layers,
          joba->layerCount);
        R_AddCmdResolveComposite(CL_CompositeEmblemCallback);
        R_AddCmdEndOfList();
      }
    }
    else if ( joba->state == COMPOSITE_STATE_COMPLETE && joba->cancel )
    {
      if ( !joba->resultImage
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\client\\cl_compositing.cpp",
              519,
              0,
              "%s",
              "job->resultImage") )
      {
        __debugbreak();
      }
      CL_CompositeReleaseImage(joba->resultImage);
      joba->state = COMPOSITE_STATE_IDLE;
    }
  }
}

bool __cdecl R_HW_IsFencePending(IDirect3DQuery9 *const *fence)
{
  HRESULT hr; // [esp+4h] [ebp-Ch]
  unsigned int data; // [esp+8h] [ebp-8h] BYREF
  int sem; // [esp+Ch] [ebp-4h]

  sem = R_AcquireDXDeviceOwnership(0);
  if ( *fence )
  {
    hr = (*(int (__stdcall **)(unsigned int, unsigned int *, int, int))(**(unsigned int **)fence + 28))(*fence, &data, 4, 1);
    if ( sem )
      R_ReleaseDXDeviceOwnership();
    return hr && hr != -2005530520;
  }
  else
  {
    if ( sem )
      R_ReleaseDXDeviceOwnership();
    return 0;
  }
}

// local variable allocation has failed, the output may be wrong!
char  CL_CompositeDrawEmblemPhysical@<al>(
        GfxColor a1@<ebp>,
        float x,
        float y,
        float w,
        float h,
        const float *color,
        CompositeEmblemLayer *layers,
        int layerCount)
{
  float v9; // xmm0_4
  float v10; // xmm0_4
  long double smoothSize; // [esp+Ch] [ebp-16Ch]
  long double v12; // [esp+Ch] [ebp-16Ch]
  long double v13; // [esp+14h] [ebp-164h]
  long double v14; // [esp+14h] [ebp-164h]
  float *p_packed; // [esp+1Ch] [ebp-15Ch]
  float v16[3]; // [esp+20h] [ebp-158h] BYREF
  _BYTE matPlacement_12[32]; // [esp+38h] [ebp-140h] OVERLAPPED BYREF
  float matScale[3][3]; // [esp+58h] [ebp-120h] BYREF
  float matRotate[3][3]; // [esp+7Ch] [ebp-FCh] BYREF
  float matResult[3][3]; // [esp+A0h] [ebp-D8h]
  float outlineSize; // [esp+C4h] [ebp-B4h] BYREF
  GfxImage *image; // [esp+C8h] [ebp-B0h]
  const CompositeEmblemLayer *layer; // [esp+CCh] [ebp-ACh] BYREF
  float coords[4][2]; // [esp+D0h] [ebp-A8h] BYREF
  int layerIdx; // [esp+F0h] [ebp-88h]
  float v26; // [esp+F4h] [ebp-84h]
  float v27; // [esp+F8h] [ebp-80h]
  float v28; // [esp+FCh] [ebp-7Ch]
  float smooth; // [esp+100h] [ebp-78h]
  float widthRange; // [esp+104h] [ebp-74h]
  float smoothRange; // [esp+108h] [ebp-70h]
  float widthMax; // [esp+10Ch] [ebp-6Ch]
  float widthMin; // [esp+110h] [ebp-68h]
  float smoothMin; // [esp+114h] [ebp-64h] BYREF
  float smoothMax; // [esp+118h] [ebp-60h]
  GfxQuadVertex verts[4]; // [esp+120h] [ebp-58h] BYREF
  Material *emblemMaterial; // [esp+170h] [ebp-8h]
  Material *retaddr; // [esp+178h] [ebp+0h]

  verts[3].color = a1;
  emblemMaterial = retaddr;
  if ( !CL_CompositeCheckStreaming(layers, layerCount) )
    return 0;
  LODWORD(verts[3].st[0]) = Material_Register("emblem_lut", 3);
  smoothMin = x;
  smoothMax = y;
  verts[0].st[0] = (float)(x + w) - 1.0;
  verts[0].st[1] = y;
  verts[1].st[0] = verts[0].st[0];
  verts[1].st[1] = (float)(y + h) - 1.0;
  verts[2].st[0] = x;
  verts[2].st[1] = verts[1].st[1];
  for ( widthMin = 0.0; SLODWORD(widthMin) < 4; ++LODWORD(widthMin) )
    R_ConvertColorToBytes(color, (unsigned __int8 *)&verts[LODWORD(widthMin)].xy[1]);
  widthMax = FLOAT_0_47999999;
  smoothRange = FLOAT_0_1;
  widthRange = FLOAT_314_0;
  smooth = FLOAT_64_0;
  v28 = 0.47999999 - 0.1;
  v27 = 64.0 - 314.0;
  *(float *)&layerIdx = (float)((float)((float)(w - 314.0) / (float)(64.0 - 314.0)) * (float)(0.47999999 - 0.1)) + 0.1;
  if ( (float)(*(float *)&layerIdx - 0.47999999) < 0.0 )
    v9 = *(float *)&layerIdx;
  else
    v9 = widthMax;
  coords[3][1] = v9;
  if ( (float)(smoothRange - *(float *)&layerIdx) < 0.0 )
    v10 = coords[3][1];
  else
    v10 = smoothRange;
  coords[3][0] = v10;
  v26 = v10;
  LODWORD(coords[2][1]) = layerCount - 1;
  while ( coords[2][1] >= 0.0 )
  {
    outlineSize = FLOAT_N0_5;
    image = (GfxImage *)LODWORD(FLOAT_N0_5);
    layer = (const CompositeEmblemLayer *)LODWORD(FLOAT_0_5);
    *(_QWORD *)&coords[0][0] = __PAIR64__(LODWORD(FLOAT_0_5), LODWORD(FLOAT_N0_5));
    *(_QWORD *)&coords[1][0] = __PAIR64__(LODWORD(FLOAT_N0_5), LODWORD(FLOAT_0_5));
    coords[2][0] = FLOAT_0_5;
    LODWORD(matResult[2][2]) = &layers[LODWORD(coords[2][1])];
    LODWORD(matResult[2][1]) = BG_EmblemsGetIconImage(*(_WORD *)LODWORD(matResult[2][2]));
    if ( *(_BYTE *)(LODWORD(matResult[2][2]) + 28) )
      matResult[2][0] = BG_EmblemsGetIconOutlineSize(*(_WORD *)LODWORD(matResult[2][2]));
    else
      matResult[2][0] = *(float *)&FLOAT_0_0;
    if ( LODWORD(matResult[2][1]) && R_StreamTouchImageAndCheck((GfxImage *)LODWORD(matResult[2][1]), 0) )
    {
      MatrixIdentity33((float (*)[3])matRotate[2]);
      matResult[1][0] = FLOAT_0_5;
      matResult[1][1] = FLOAT_0_5;
      MatrixRotationZ((float (*)[3])matScale[2], *(float *)(LODWORD(matResult[2][2]) + 24));
      MatrixIdentity33((float (*)[3])&matPlacement_12[20]);
      *(unsigned int *)&matPlacement_12[16] = *(unsigned int *)(LODWORD(matResult[2][2]) + 16);
      __libm_sse2_pow(smoothSize, v13);
      *(float *)&matPlacement_12[20] = 1.0 / (float)2.0;
      *(unsigned int *)&matPlacement_12[12] = *(unsigned int *)(LODWORD(matResult[2][2]) + 20);
      __libm_sse2_pow(v12, v14);
      matScale[0][1] = 1.0 / (float)2.0;
      if ( *(_BYTE *)(LODWORD(matResult[2][2]) + 29) )
        *(unsigned int *)&matPlacement_12[20] ^= _mask__NegFloat_;
      MatrixIdentity33((float (*)[3])v16);
      *(unsigned int *)matPlacement_12 = *(unsigned int *)(LODWORD(matResult[2][2]) + 8) ^ _mask__NegFloat_;
      *(unsigned int *)&matPlacement_12[4] = *(unsigned int *)(LODWORD(matResult[2][2]) + 12) ^ _mask__NegFloat_;
      MatrixMultiplyEquals((const float (*)[3])&matPlacement_12[20], (float (*)[3])matRotate[2]);
      MatrixMultiplyEquals((const float (*)[3])matScale[2], (float (*)[3])matRotate[2]);
      MatrixMultiplyEquals((const float (*)[3])v16, (float (*)[3])matRotate[2]);
      UV_Transform((const float (*)[3])matRotate[2], &outlineSize);
      UV_Transform((const float (*)[3])matRotate[2], (float *)&layer);
      UV_Transform((const float (*)[3])matRotate[2], &coords[0][1]);
      UV_Transform((const float (*)[3])matRotate[2], &coords[1][1]);
      for ( widthMin = 0.0; SLODWORD(widthMin) < 4; ++LODWORD(widthMin) )
      {
        p_packed = (float *)&verts[LODWORD(widthMin) - 1].color.packed;
        HIDWORD(v13) = &outlineSize + 2 * LODWORD(widthMin);
        *p_packed = *(float *)HIDWORD(v13);
        p_packed[1] = *(float *)(HIDWORD(v13) + 4);
      }
      *(float *)&v13 = v26 - matResult[2][0];
      if ( (float)(0.1 - (float)(v26 - matResult[2][0])) < 0.0 )
        *((float *)&smoothSize + 1) = v26 - matResult[2][0];
      else
        *((float *)&smoothSize + 1) = FLOAT_0_1;
      R_AddCmdDrawEmblemLayer(
        (Material *)LODWORD(verts[3].st[0]),
        (const GfxImage *)LODWORD(matResult[2][1]),
        *(unsigned int *)(LODWORD(matResult[2][2]) + 4),
        matResult[2][0],
        *((float *)&smoothSize + 1),
        (GfxQuadVertex *)&smoothMin);
    }
    --LODWORD(coords[2][1]);
  }
  return 1;
}

void __cdecl UV_Transform(const float (*mat)[3], float *vec)
{
  float temp; // [esp+0h] [ebp-4h]

  temp = (float)((float)(*vec * (*mat)[0]) + (float)(vec[1] * (float)(*mat)[3])) + (float)(*mat)[6];
  vec[1] = (float)((float)(*vec * (float)(*mat)[1]) + (float)(vec[1] * (float)(*mat)[4])) + (float)(*mat)[7];
  *vec = temp;
}

bool __cdecl CL_CompositeIsLayerEmpty(CompositeEmblemLayer *layer)
{
  if ( !layer
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\client\\cl_compositing.cpp", 642, 0, "%s", "layer") )
  {
    __debugbreak();
  }
  return layer->icon == -1;
}

void __cdecl CL_CompositeResetLayer(CompositeEmblemLayer *layer)
{
  if ( !layer
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\client\\cl_compositing.cpp", 648, 0, "%s", "layer") )
  {
    __debugbreak();
  }
  layer->pos[0] = *(float *)&FLOAT_0_0;
  layer->pos[1] = *(float *)&FLOAT_0_0;
  layer->scale[0] = *(float *)&FLOAT_0_0;
  layer->scale[1] = *(float *)&FLOAT_0_0;
  layer->angle = *(float *)&FLOAT_0_0;
  layer->outline = 0;
  layer->flip = 0;
}

void __cdecl CL_CompositeClearLayer(CompositeEmblemLayer *layer)
{
  if ( !layer
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\client\\cl_compositing.cpp", 660, 0, "%s", "layer") )
  {
    __debugbreak();
  }
  layer->pos[0] = *(float *)&FLOAT_0_0;
  layer->pos[1] = *(float *)&FLOAT_0_0;
  layer->scale[0] = *(float *)&FLOAT_0_0;
  layer->scale[1] = *(float *)&FLOAT_0_0;
  layer->angle = *(float *)&FLOAT_0_0;
  layer->outline = 0;
  layer->flip = 0;
  layer->colorIdx = 0;
  layer->icon = -1;
}

