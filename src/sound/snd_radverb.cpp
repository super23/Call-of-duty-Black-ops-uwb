#include "snd_radverb.h"

void __cdecl SND_RvParamsDefault(snd_rv_params *params)
{
  params->earlySize = FLOAT_1_0;
  params->lateSize = FLOAT_1_0;
  params->earlyGain = FLOAT_1_0;
  params->lateGain = FLOAT_1_0;
  params->smoothing = FLOAT_0_5;
  params->diffusion = FLOAT_0_5;
  params->dryGain = 0.0;
  params->earlyTime = FLOAT_0_80000001;
  params->lateTime = FLOAT_0_80000001;
  params->earlyLpf = 0.0;
  params->lateGainProx[0] = FLOAT_1_0;
  params->lateGainProx[1] = FLOAT_1_0;
  params->lateGainProx[2] = FLOAT_1_0;
  params->lateGainProx[3] = FLOAT_1_0;
  params->returnGain = FLOAT_1_0;
  params->lateLpf = 0.0;
  params->inputLpf = 0.0;
  params->dampLpf = 0.0;
  params->wallReflect[0] = FLOAT_0_5;
  params->wallReflect[1] = FLOAT_0_5;
  params->wallReflect[2] = FLOAT_0_5;
  params->wallReflect[3] = FLOAT_0_5;
  params->frameRate = FLOAT_48000_0;
  params->delayMatrix = 0;
}

double __cdecl SND_RvValidateRange(float value, float min, float max)
{
  float v3; // xmm0_4

  if ( IS_NAN(value)
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_radverb.cpp", 122, 0, "%s", "!IS_NAN(value)") )
  {
    __debugbreak();
  }
  v3 = value;
  if ( value < min )
  {
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_radverb.cpp",
            123,
            0,
            "%s\n\t(value) = %g",
            "(value >= min)",
            value) )
      __debugbreak();
    v3 = value;
  }
  if ( max < v3
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_radverb.cpp",
          124,
          0,
          "%s\n\t(value) = %g",
          "(value <= max)",
          v3) )
  {
    __debugbreak();
  }
  return I_fclamp(value, min, max);
}

void __cdecl SND_RvParamsValidate(snd_rv_params *params)
{
  float *lateGainProx; // edi
  int v2; // ebx
  float *wallReflect; // edi
  int v4; // ebx

  params->smoothing = SND_RvValidateRange(params->smoothing, 0.0, 1.0);
  params->earlyTime = SND_RvValidateRange(params->earlyTime, 0.0, 1.0);
  params->lateTime = SND_RvValidateRange(params->lateTime, 0.0, 1.0);
  params->earlyGain = SND_RvValidateRange(params->earlyGain, 0.0, 4.0);
  params->lateGain = SND_RvValidateRange(params->lateGain, 0.0, 4.0);
  lateGainProx = params->lateGainProx;
  v2 = 4;
  do
  {
    *lateGainProx = SND_RvValidateRange(*lateGainProx, 0.0, 4.0);
    ++lateGainProx;
    --v2;
  }
  while ( v2 );
  params->returnGain = SND_RvValidateRange(params->returnGain, 0.0, 4.0);
  params->earlyLpf = SND_RvValidateRange(params->earlyLpf, 0.0, 1.0);
  params->lateLpf = SND_RvValidateRange(params->lateLpf, 0.0, 1.0);
  params->inputLpf = SND_RvValidateRange(params->inputLpf, 0.0, 1.0);
  params->dampLpf = SND_RvValidateRange(params->dampLpf, 0.0, 1.0);
  wallReflect = params->wallReflect;
  v4 = 4;
  do
  {
    *wallReflect = SND_RvValidateRange(*wallReflect, 0.0, 1.0);
    ++wallReflect;
    --v4;
  }
  while ( v4 );
  params->dryGain = SND_RvValidateRange(params->dryGain, 0.0, 1.0);
  params->earlySize = SND_RvValidateRange(params->earlySize, 0.1, 16.0);
  params->lateSize = SND_RvValidateRange(params->lateSize, 0.1, 16.0);
  params->diffusion = SND_RvValidateRange(params->diffusion, 0.0, 1.0);
  if ( IS_NAN(params->angle) )
  {
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_radverb.cpp",
            153,
            0,
            "%s",
            "!IS_NAN(params->angle)") )
      __debugbreak();
  }
}

void __cdecl SND_RvFrameParam(snd_rv_params *params, snd_rv_state *state, unsigned int frameCount)
{
  float *wallReflect; // ebx
  double v6; // st7
  unsigned int v7; // ebx
  double v8; // st7
  float v9; // xmm0_4
  float v10; // xmm1_4
  float v11; // xmm2_4
  float v12; // xmm3_4
  unsigned int *v13; // esi
  int v14; // ebx
  int v15; // ecx
  double v16; // st7
  bool v17; // cf
  unsigned int delayMatrix; // [esp-4h] [ebp-2Ch]
  float x; // [esp+8h] [ebp-20h]
  float xa; // [esp+8h] [ebp-20h]
  _QWORD coef[2]; // [esp+18h] [ebp-10h]
  float smooth; // [esp+30h] [ebp+8h]
  float smootha; // [esp+30h] [ebp+8h]
  float *smoothb; // [esp+30h] [ebp+8h]
  float *db; // [esp+34h] [ebp+Ch]
  float dbc; // [esp+34h] [ebp+Ch]
  float dba; // [esp+34h] [ebp+Ch]
  int dbb; // [esp+34h] [ebp+Ch]
  unsigned int frameCounta; // [esp+38h] [ebp+10h]

  SND_RvParamsValidate(params);
  state->params.frameRate = params->frameRate;
  if ( params->frameRate <= 1000.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_radverb.cpp",
          165,
          0,
          "%s",
          "params->frameRate > 1000.0f") )
  {
    __debugbreak();
  }
  if ( params->frameRate >= 100000.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_radverb.cpp",
          166,
          0,
          "%s",
          "params->frameRate < 100000.0f") )
  {
    __debugbreak();
  }
  smooth = (1.0 - params->smoothing) / (params->frameRate / (double)frameCount * 0.1);
  if ( IS_NAN(smooth)
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_radverb.cpp", 170, 0, "%s", "!IS_NAN(smooth)") )
  {
    __debugbreak();
  }
  state->params.earlyTime = I_flerp(state->params.earlyTime, params->earlyTime, smooth);
  state->params.lateTime = I_flerp(state->params.lateTime, params->lateTime, smooth);
  state->params.earlyGain = I_flerp(state->params.earlyGain, params->earlyGain, smooth);
  state->params.lateGain = I_flerp(state->params.lateGain, params->lateGain, smooth);
  state->params.lateGainProx[0] = params->lateGainProx[0] * state->params.lateGain;
  state->params.lateGainProx[1] = params->lateGainProx[1] * state->params.lateGain;
  state->params.lateGainProx[2] = params->lateGainProx[2] * state->params.lateGain;
  state->params.lateGainProx[3] = params->lateGainProx[3] * state->params.lateGain;
  state->params.returnGain = I_flerp(state->params.returnGain, params->returnGain, smooth);
  state->params.earlyLpf = I_flerp(state->params.earlyLpf, params->earlyLpf, smooth);
  state->params.lateLpf = I_flerp(state->params.lateLpf, params->lateLpf, smooth);
  state->params.inputLpf = I_flerp(state->params.inputLpf, params->inputLpf, smooth);
  state->params.dampLpf = I_flerp(state->params.dampLpf, params->dampLpf, smooth);
  wallReflect = state->params.wallReflect;
  db = params->wallReflect;
  frameCounta = 4;
  do
  {
    v6 = I_flerp(*wallReflect, *db++, smooth);
    *wallReflect++ = v6;
    --frameCounta;
  }
  while ( frameCounta );
  state->params.dryGain = I_flerp(state->params.dryGain, params->dryGain, smooth);
  state->params.diffusion = I_flerp(state->params.diffusion, params->diffusion, smooth);
  state->params.earlySize = I_flerp(state->params.earlySize, params->earlySize, smooth);
  state->params.lateSize = I_flerp(state->params.lateSize, params->lateSize, smooth);
  delayMatrix = params->delayMatrix;
  state->params.delayMatrix = delayMatrix;
  SND_RvDelayInit(state, delayMatrix);
  dbc = state->params.diffusion * 1.5707964;
  smootha = cosf(dbc);
  dba = sinf(dbc);
  if ( IS_NAN(smootha)
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_radverb.cpp", 205, 0, "%s", "!IS_NAN(da)") )
  {
    __debugbreak();
  }
  if ( IS_NAN(dba)
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_radverb.cpp", 206, 0, "%s", "!IS_NAN(db)") )
  {
    __debugbreak();
  }
  state->lateReflectionCoefs[0][0] = smootha;
  state->lateReflectionCoefs[0][1] = dba;
  state->lateReflectionCoefs[1][0] = dba;
  LODWORD(state->lateReflectionCoefs[1][1]) = LODWORD(smootha) ^ _mask__NegFloat_;
  LODWORD(state->lateReflectionCoefs[2][2]) = LODWORD(smootha) ^ _mask__NegFloat_;
  state->lateReflectionCoefs[2][3] = dba;
  state->lateReflectionCoefs[3][2] = dba;
  state->lateReflectionCoefs[3][3] = smootha;
  v7 = 0;
  smoothb = state->params.wallReflect;
  do
  {
    x = (*smoothb - 1.0) * (double)v7 * 0.35355338 + 0.70710677;
    v8 = I_fmax(0.0, x);
    *((float *)coef + v7) = v8;
    xa = v8;
    if ( IS_NAN(xa)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_radverb.cpp",
            222,
            0,
            "%s",
            "!IS_NAN(coef[i])") )
    {
      __debugbreak();
    }
    ++smoothb;
    ++v7;
  }
  while ( v7 < 4 );
  v9 = *(float *)coef;
  v10 = *((float *)&coef[1] + 1);
  v11 = *(float *)&coef[1];
  v12 = *((float *)coef + 1);
  state->earlyReflectionCoefs[0][0] = *(float *)coef;
  state->earlyReflectionCoefs[0][1] = v10;
  state->earlyReflectionCoefs[0][2] = v11;
  state->earlyReflectionCoefs[0][3] = v12;
  state->earlyReflectionCoefs[1][0] = v10;
  state->earlyReflectionCoefs[1][1] = v9;
  state->earlyReflectionCoefs[1][2] = v12;
  state->earlyReflectionCoefs[1][3] = v11;
  state->earlyReflectionCoefs[2][0] = v11;
  state->earlyReflectionCoefs[2][1] = v12;
  state->earlyReflectionCoefs[2][2] = v9;
  state->earlyReflectionCoefs[2][3] = v10;
  state->earlyReflectionCoefs[3][0] = v12;
  state->earlyReflectionCoefs[3][1] = v11;
  state->earlyReflectionCoefs[3][2] = v10;
  state->earlyReflectionCoefs[3][3] = v9;
  v13 = state->earlyReflectionDelays[0];
  dbb = 4;
  do
  {
    v14 = 4;
    do
    {
      v15 = v13[105];
      coef[1] = (__int64)((double)v13[89] * params->earlySize);
      *v13 = coef[1];
      v16 = (double)(int)v13[105];
      if ( v15 < 0 )
        v16 = v16 + 4294967300.0;
      v17 = *v13 < 0x8000;
      coef[1] = (__int64)(v16 * params->lateSize);
      v13[32] = coef[1];
      if ( !v17
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_radverb.cpp",
              252,
              0,
              "%s",
              "state->earlyReflectionDelays[i][j] < SND_RV_DELAY_FRAME_COUNT") )
      {
        __debugbreak();
      }
      if ( v13[32] >= 0x8000
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_radverb.cpp",
              253,
              0,
              "%s",
              "state->lateReflectionDelays[i][j] < SND_RV_DELAY_FRAME_COUNT") )
      {
        __debugbreak();
      }
      ++v13;
      --v14;
    }
    while ( v14 );
    --dbb;
  }
  while ( dbb );
}

double __cdecl I_flerp(float a, float b, float w)
{
  return (1.0 - w) * a + b * w;
}

void __cdecl SND_RvDelayInit(snd_rv_state *state, unsigned int values)
{
  *(_QWORD *)&state->lateReflectionDelayBase[0][2] = 0;
  *(_QWORD *)&state->lateReflectionDelayBase[1][2] = 0;
  *(_QWORD *)&state->lateReflectionDelayBase[2][0] = 0;
  *(_QWORD *)&state->lateReflectionDelayBase[3][0] = 0;
  if ( values )
  {
    state->earlyReflectionDelayBase[0][0] = 587;
    state->earlyReflectionDelayBase[0][1] = 1439;
    state->earlyReflectionDelayBase[0][2] = 1373;
    state->earlyReflectionDelayBase[0][3] = 643;
    state->earlyReflectionDelayBase[1][0] = 1493;
    state->earlyReflectionDelayBase[1][1] = 373;
    state->earlyReflectionDelayBase[1][2] = 911;
    state->earlyReflectionDelayBase[1][3] = 1091;
    state->earlyReflectionDelayBase[2][0] = 1201;
    state->earlyReflectionDelayBase[2][1] = 773;
    state->earlyReflectionDelayBase[2][2] = 521;
    state->earlyReflectionDelayBase[2][3] = 1637;
    state->earlyReflectionDelayBase[3][0] = 997;
    state->earlyReflectionDelayBase[3][1] = 1297;
    state->earlyReflectionDelayBase[3][2] = 1579;
    state->earlyReflectionDelayBase[3][3] = 439;
    state->lateReflectionDelayBase[0][0] = 293;
    state->lateReflectionDelayBase[0][1] = 479;
    state->lateReflectionDelayBase[1][0] = 1051;
    state->lateReflectionDelayBase[1][1] = 1237;
    state->lateReflectionDelayBase[2][2] = 1169;
    state->lateReflectionDelayBase[2][3] = 1663;
    state->lateReflectionDelayBase[3][2] = 113;
    state->lateReflectionDelayBase[3][3] = 607;
  }
  else
  {
    state->earlyReflectionDelayBase[0][0] = 941;
    state->earlyReflectionDelayBase[0][1] = 149;
    state->earlyReflectionDelayBase[0][2] = 673;
    state->earlyReflectionDelayBase[0][3] = 277;
    state->earlyReflectionDelayBase[1][0] = 211;
    state->earlyReflectionDelayBase[1][1] = 797;
    state->earlyReflectionDelayBase[1][2] = 421;
    state->earlyReflectionDelayBase[1][3] = 577;
    state->earlyReflectionDelayBase[2][0] = 751;
    state->earlyReflectionDelayBase[2][1] = 349;
    state->earlyReflectionDelayBase[2][2] = 1009;
    state->earlyReflectionDelayBase[2][3] = 173;
    state->earlyReflectionDelayBase[3][0] = 499;
    state->earlyReflectionDelayBase[3][1] = 613;
    state->earlyReflectionDelayBase[3][2] = 239;
    state->earlyReflectionDelayBase[3][3] = 877;
    state->lateReflectionDelayBase[0][0] = 1103;
    state->lateReflectionDelayBase[0][1] = 1283;
    state->lateReflectionDelayBase[1][0] = 1439;
    state->lateReflectionDelayBase[1][1] = 1619;
    state->lateReflectionDelayBase[2][2] = 1759;
    state->lateReflectionDelayBase[2][3] = 1949;
    state->lateReflectionDelayBase[3][2] = 751;
    state->lateReflectionDelayBase[3][3] = 941;
  }
}

void __cdecl SND_RvFrame(
        snd_rv_params *params,
        snd_rv_state *state,
        const float *count,
        const float *inLF,
        const float *inRF,
        const float *inLS,
        const float *inRS,
        float *outLF,
        float *outRF,
        float *outLS,
        float *outRS)
{
  float *earlyLpfState; // edi
  float *v13; // eax
  float *v14; // ecx
  unsigned int delayIndex; // ebx
  float v16; // xmm4_4
  float v17; // xmm5_4
  float v18; // xmm0_4
  float v19; // xmm2_4
  snd_rv_state *v20; // edx
  float v21; // xmm7_4
  bool v22; // zf
  float v23; // xmm7_4
  float v24; // xmm0_4
  float v25; // xmm0_4
  float v26; // xmm2_4
  float v27; // xmm0_4
  float v28; // xmm0_4
  float v29; // xmm2_4
  float v30; // xmm0_4
  float v31; // xmm4_4
  float v32; // xmm0_4
  float v33; // xmm2_4
  float v34; // xmm0_4
  float v35; // xmm0_4
  float v36; // xmm2_4
  int v37; // [esp+10h] [ebp-10h]
  int v38; // [esp+14h] [ebp-Ch]
  int v39; // [esp+18h] [ebp-8h]
  int v40; // [esp+1Ch] [ebp-4h]
  unsigned int counta; // [esp+30h] [ebp+10h]
  unsigned int countb; // [esp+30h] [ebp+10h]
  const float *inRFa; // [esp+38h] [ebp+18h]
  int inLSa; // [esp+3Ch] [ebp+1Ch]
  const float *inRSa; // [esp+40h] [ebp+20h]
  float *outRFa; // [esp+48h] [ebp+28h]
  float *outLSa; // [esp+4Ch] [ebp+2Ch]
  float *outRSa; // [esp+50h] [ebp+30h]

  SND_RvFrameParam(params, state, (unsigned int)count);
  earlyLpfState = state->earlyLpfState;
  counta = 4;
  do
  {
    if ( IS_NAN(*(earlyLpfState - 4))
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_radverb.cpp",
            298,
            0,
            "%s",
            "!IS_NAN(state->inputLpfState[k])") )
    {
      __debugbreak();
    }
    if ( IS_NAN(*earlyLpfState)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_radverb.cpp",
            299,
            0,
            "%s",
            "!IS_NAN(state->earlyLpfState[k])") )
    {
      __debugbreak();
    }
    if ( IS_NAN(earlyLpfState[4])
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_radverb.cpp",
            300,
            0,
            "%s",
            "!IS_NAN(state->lateLpfState[k])") )
    {
      __debugbreak();
    }
    if ( IS_NAN(earlyLpfState[8])
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_radverb.cpp",
            301,
            0,
            "%s",
            "!IS_NAN(state->dampLpfState[k])") )
    {
      __debugbreak();
    }
    ++earlyLpfState;
    --counta;
  }
  while ( counta );
  if ( count )
  {
    v13 = outLS;
    v39 = (char *)inLS - (char *)outLS;
    v38 = (char *)inRF - (char *)outLS;
    v40 = (char *)outRF - (char *)outLS;
    v37 = (char *)inRS - (char *)outLS;
    inRSa = (const float *)((char *)outRS - (char *)outLS);
    outRSa = (float *)((char *)inLF - (char *)outLS);
    outRFa = (float *)((char *)outLF - (char *)outLS);
    inRFa = count;
    do
    {
      outLSa = state->dampLpfState;
      v14 = state->earlyReflectionCoefs[0];
      inLSa = 4;
      do
      {
        delayIndex = state->delayIndex;
        v16 = 0.0;
        v17 = 0.0;
        v18 = 0.0;
        v19 = 0.0;
        v20 = state;
        countb = 2;
        do
        {
          v17 = (float)(v20->delayLine[delayIndex
                                     - *((unsigned int *)v14 + 16)
                                     + ((int)(delayIndex - *((unsigned int *)v14 + 16)) < 0 ? 0x8000 : 0)]
                      * *v14)
              + v17;
          v16 = (float)(v20->delayLine[delayIndex
                                     - *((unsigned int *)v14 + 48)
                                     + ((int)(delayIndex - *((unsigned int *)v14 + 48)) < 0 ? 0x8000 : 0)]
                      * v14[32])
              + v16;
          v19 = (float)(v20->delayLine[delayIndex
                                     - *((unsigned int *)v14 + 17)
                                     + 0x8000
                                     + ((int)(delayIndex - *((unsigned int *)v14 + 17)) < 0 ? 0x8000 : 0)]
                      * v14[1])
              + v19;
          v21 = v20->delayLine[delayIndex
                             - *((unsigned int *)v14 + 49)
                             + 0x8000
                             + ((int)(delayIndex - *((unsigned int *)v14 + 49)) < 0 ? 0x8000 : 0)]
              * v14[33];
          v14 += 2;
          v20 = (snd_rv_state *)((char *)v20 + 0x40000);
          v22 = countb-- == 1;
          v23 = v21 + v18;
          v18 = v23;
        }
        while ( !v22 );
        *(outLSa - 8) = (float)((float)((float)(1.0 - state->params.earlyLpf) * state->params.earlyTime)
                              * (float)(v19 + v17))
                      + (float)(state->params.earlyLpf * *(outLSa - 8));
        *outLSa = (float)((float)((float)(1.0 - state->params.dampLpf) * state->params.lateTime) * (float)(v23 + v16))
                + (float)(state->params.dampLpf * *outLSa);
        v22 = inLSa-- == 1;
        ++outLSa;
      }
      while ( !v22 );
      *(float *)((char *)v13 + (unsigned int)outRFa) = (float)(state->dampLpfState[1] + state->dampLpfState[0]) * 0.70710677;
      *(float *)((char *)v13 + (unsigned int)inRSa) = (float)(state->dampLpfState[0] - state->dampLpfState[1]) * 0.70710677;
      *(float *)((char *)v13 + v40) = (float)(state->dampLpfState[3] - state->dampLpfState[2]) * 0.70710677;
      *v13 = (float)(state->dampLpfState[3] + state->dampLpfState[2]) * 0.70710677;
      v24 = (float)((float)(1.0 - state->params.inputLpf) * *(float *)((char *)v13 + v39))
          + (float)(state->params.inputLpf * state->inputLpfState[0]);
      state->inputLpfState[0] = v24;
      state->delayLine[state->delayIndex] = v24 + *v13;
      v25 = (float)((float)(1.0 - state->params.lateLpf) * *v13)
          + (float)(state->params.lateLpf * state->lateLpfState[0]);
      v26 = state->inputLpfState[0];
      state->lateLpfState[0] = v25;
      *v13 = (float)((float)((float)(v26 * state->params.dryGain) + (float)(v25 * state->params.lateGainProx[0]))
                   + (float)(state->params.earlyGain * state->earlyLpfState[0]))
           * state->params.returnGain;
      v27 = (float)((float)(1.0 - state->params.inputLpf) * *(float *)((char *)v13 + v38))
          + (float)(state->params.inputLpf * state->inputLpfState[1]);
      state->inputLpfState[1] = v27;
      state->delayLine[state->delayIndex + 0x8000] = v27 + *(float *)((char *)v13 + v40);
      v28 = (float)((float)(1.0 - state->params.lateLpf) * *(float *)((char *)v13 + v40))
          + (float)(state->params.lateLpf * state->lateLpfState[1]);
      v29 = state->params.lateGainProx[1] * v28;
      state->lateLpfState[1] = v28;
      *(float *)((char *)v13 + v40) = (float)((float)(v29 + (float)(state->inputLpfState[1] * state->params.dryGain))
                                            + (float)(state->params.earlyGain * state->earlyLpfState[1]))
                                    * state->params.returnGain;
      v30 = (float)((float)(1.0 - state->params.inputLpf) * *(float *)((char *)v13 + v37))
          + (float)(state->params.inputLpf * state->inputLpfState[2]);
      state->inputLpfState[2] = v30;
      state->delayLine[state->delayIndex + 0x10000] = v30 + *(float *)((char *)v13 + (unsigned int)inRSa);
      v31 = state->earlyLpfState[2];
      v32 = (float)((float)(1.0 - state->params.lateLpf) * *(float *)((char *)v13 + (unsigned int)inRSa))
          + (float)(state->params.lateLpf * state->lateLpfState[2]);
      v33 = state->inputLpfState[2];
      state->lateLpfState[2] = v32;
      *(float *)((char *)v13 + (unsigned int)inRSa) = (float)((float)((float)(v33 * state->params.dryGain)
                                                              + (float)(v31 * state->params.earlyGain))
                                                      + (float)(v32 * state->params.lateGainProx[2]))
                                              * state->params.returnGain;
      v34 = (float)((float)(1.0 - state->params.inputLpf) * *(float *)((char *)v13 + (unsigned int)outRSa))
          + (float)(state->params.inputLpf * state->inputLpfState[3]);
      state->inputLpfState[3] = v34;
      state->delayLine[state->delayIndex + 98304] = v34 + *(float *)((char *)v13 + (unsigned int)outRFa);
      v35 = (float)((float)(1.0 - state->params.lateLpf) * *(float *)((char *)v13 + (unsigned int)outRFa))
          + (float)(state->params.lateLpf * state->lateLpfState[3]);
      v36 = state->params.lateGainProx[3];
      state->lateLpfState[3] = v35;
      *(float *)((char *)v13 + (unsigned int)outRFa) = (float)((float)((float)(v36 * v35)
                                                               + (float)(state->params.earlyGain
                                                                       * state->earlyLpfState[3]))
                                                       + (float)(state->inputLpfState[3] * state->params.dryGain))
                                               * state->params.returnGain;
      if ( ++state->delayIndex >= 0x8000 )
        state->delayIndex = 0;
      ++v13;
      inRFa = (const float *)((char *)inRFa - 1);
    }
    while ( inRFa );
  }
}

