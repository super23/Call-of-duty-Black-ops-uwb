#include "rb_fog.h"

void  R_SetFrameFog(float a1@<ebp>, GfxCmdBufInput *input, const GfxViewInfo *viewInfo)
{
  long double v3; // [esp+10h] [ebp-8Ch]
  long double v4; // [esp+10h] [ebp-8Ch]
  long double v5; // [esp+10h] [ebp-8Ch]
  long double v6; // [esp+10h] [ebp-8Ch]
  long double v7; // [esp+10h] [ebp-8Ch]
  long double v8; // [esp+10h] [ebp-8Ch]
  float v9; // [esp+24h] [ebp-78h]
  float v10; // [esp+28h] [ebp-74h]
  float v11[3]; // [esp+34h] [ebp-68h] BYREF
  float parms2[4]; // [esp+40h] [ebp-5Ch]
  float fog_maxDensity; // [esp+50h] [ebp-4Ch] BYREF
  float fog_density; // [esp+54h] [ebp-48h]
  float viewerZ; // [esp+58h] [ebp-44h]
  float fogColorVec[4]; // [esp+5Ch] [ebp-40h] BYREF
  float v17; // [esp+6Ch] [ebp-30h]
  float parms[4]; // [esp+70h] [ebp-2Ch]
  float endAng; // [esp+80h] [ebp-1Ch]
  float v20; // [esp+84h] [ebp-18h]
  GfxFog *startAng; // [esp+88h] [ebp-14h]
  const GfxBackEndData *data; // [esp+8Ch] [ebp-10h]
  float slopeAng; // [esp+90h] [ebp-Ch]
  const GfxFog *fog; // [esp+94h] [ebp-8h]
  const GfxFog *retaddr; // [esp+9Ch] [ebp+0h]

  slopeAng = a1;
  fog = retaddr;
  if ( r_fog->current.enabled )
  {
    data = input->data;
    if ( !data && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_fog.cpp", 25, 0, "%s", "data") )
      __debugbreak();
    startAng = &data->fogSettings;
    R_SetInputCodeConstantFromVec4(input, 0x3Du, data->fogSettings.sunFogColor);
    R_SetInputCodeConstantFromVec4(input, 0x3Cu, data->fogSettings.sunFogDir);
    v20 = FLOAT_1_0e7;
    endAng = data->fogSettings.sunFogStartAng * 0.017453292;
    __libm_sse2_cos(v3);
    parms[3] = endAng;
    parms[2] = data->fogSettings.sunFogEndAng * 0.017453292;
    __libm_sse2_cos(v4);
    parms[1] = parms[2];
    if ( (float)(endAng - parms[2]) != 0.0 )
      v20 = 1.0 / (float)(parms[3] - parms[1]);
    fogColorVec[2] = COERCE_FLOAT(LODWORD(parms[1]) ^ _mask__NegFloat_) * v20;
    fogColorVec[3] = v20;
    v17 = *(float *)&FLOAT_0_0;
    parms[0] = *(float *)&FLOAT_0_0;
    R_SetInputCodeConstantFromVec4(input, 0x3Bu, &fogColorVec[2]);
    LODWORD(fogColorVec[1]) = startAng->color;
    fog_maxDensity = startAng->color[0];
    fog_density = startAng->color[1];
    viewerZ = startAng->color[2];
    fogColorVec[0] = startAng->color[3];
    R_SetInputCodeConstantFromVec4(input, 0x3Au, &fog_maxDensity);
    parms2[3] = viewInfo->cullViewInfo.viewParms.origin[2] - startAng->baseHeight;
    parms2[2] = startAng->density;
    parms2[1] = startAng->maxDensity;
    if ( parms2[2] == 0.0 )
    {
      R_SetInputCodeConstant(input, 0x38u, 0.0, 0.0, 0.0, 0.0);
      R_SetInputCodeConstant(input, 0x39u, 0.0, 0.0, 0.0, 0.0);
    }
    else
    {
      parms2[0] = *(float *)&FLOAT_0_0;
      v11[2] = *(float *)&FLOAT_0_0;
      v11[1] = *(float *)&FLOAT_0_0;
      if ( parms2[1] > (float)(100.0 * parms2[2]) )
        parms2[1] = 100.0 * parms2[2];
      if ( parms2[2] > parms2[1] )
        parms2[1] = parms2[2];
      __libm_sse2_log(v5);
      __libm_sse2_log(v6);
      v10 = (float)(parms2[2] / parms2[1]) - (float)((float)(startAng->heightDensity * parms2[3]) * (float)2.0);
      fogColorVec[2] = v10;
      if ( v10 >= 0.0 )
      {
        v9 = v10 + 1.0;
      }
      else
      {
        __libm_sse2_exp(v7);
        v9 = v10;
      }
      v11[0] = v9;
      LODWORD(fogColorVec[3]) = LODWORD(parms2[1]) ^ _mask__NegFloat_;
      v17 = parms2[2] * startAng->fogStart;
      __libm_sse2_log(v7);
      parms[0] = COERCE_FLOAT(LODWORD(startAng->heightDensity) ^ _mask__NegFloat_) * (float)2.0;
      if ( startAng->heightDensity > 0.0 )
      {
        __libm_sse2_log(v8);
        fogColorVec[3] = fogColorVec[3] * (float)2.0;
      }
      R_SetInputCodeConstantFromVec4(input, 0x38u, &fogColorVec[2]);
      R_SetInputCodeConstantFromVec4(input, 0x39u, v11);
    }
  }
  else
  {
    R_SetInputCodeConstant(input, 0x38u, 0.0, 0.0, 0.0, 0.0);
    R_SetInputCodeConstant(input, 0x39u, 0.0, 0.0, 0.0, 0.0);
  }
}

