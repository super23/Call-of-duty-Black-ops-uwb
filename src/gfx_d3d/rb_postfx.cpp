#include "rb_postfx.h"

bool __cdecl RB_UsingDepthOfFieldFX(const GfxViewInfo *viewInfo)
{
  if ( viewInfo->dof.viewModelEnd > (float)(viewInfo->dof.viewModelStart + 1.0) )
    return 1;
  if ( viewInfo->dof.nearEnd <= (float)(viewInfo->dof.nearStart + 1.0) )
    return viewInfo->dof.farEnd > viewInfo->dof.farStart;
  return 1;
}

bool __cdecl RB_UsingFlameFX(const GfxViewInfo *viewInfo)
{
  if ( !r_flame_allowed->current.enabled )
    return 0;
  if ( viewInfo->flameFx.enabled )
    return 1;
  return r_flameFX_enable->current.enabled;
}

bool __cdecl RB_UsingElectrifiedFX(const GfxViewInfo *viewInfo)
{
  if ( !r_flame_allowed->current.enabled )
    return 0;
  if ( viewInfo->electrifiedFx.enabled )
    return 1;
  return r_flameFX_enable->current.enabled;
}

bool __cdecl RB_UsingTransportedFX(const GfxViewInfo *viewInfo)
{
  if ( !viewInfo
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_postfx.cpp", 59, 0, "%s", "viewInfo") )
  {
    __debugbreak();
  }
  if ( !r_flame_allowed->current.enabled )
    return 0;
  if ( viewInfo->transportedFx.enabled )
    return 1;
  return r_flameFX_enable->current.enabled;
}

bool __cdecl RB_UsingWaterSheetingFX(const GfxViewInfo *viewInfo)
{
  if ( !r_waterSheetingFX_allowed->current.enabled )
    return 0;
  if ( viewInfo->waterSheetingFx.enabled )
    return 1;
  return r_waterSheetingFX_enable->current.enabled && viewInfo->waterSheetingFx.blurRadius > 0.0;
}

bool __cdecl RB_UsingGenericFilter(const GfxViewInfo *viewInfo)
{
  int iPass; // [esp+0h] [ebp-8h]
  int iFilter; // [esp+4h] [ebp-4h]

  if ( !viewInfo
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_postfx.cpp", 110, 0, "%s", "viewInfo") )
  {
    __debugbreak();
  }
  for ( iFilter = 0; iFilter < 3; ++iFilter )
  {
    for ( iPass = 0; iPass < 16; ++iPass )
    {
      if ( viewInfo->genericFilter.passEnabled[iFilter][iPass] && viewInfo->genericFilter.passMaterial[iFilter][iPass] )
        return 1;
    }
  }
  return r_genericFilter_enable->current.enabled;
}

bool __cdecl RB_UsingReviveFX(const GfxViewInfo *viewInfo)
{
  return viewInfo->reviveFx.enabled || r_reviveFX_debug->current.enabled;
}

bool __cdecl RB_UsingBlur(const GfxViewInfo *viewInfo)
{
  return r_blur->current.value > 0.0 || viewInfo->blurRadius > 0.0;
}

bool __cdecl RB_SetBloomConstants(const GfxViewInfo *viewInfo)
{
  float v2; // [esp+10h] [ebp-ECh]
  float v3; // [esp+14h] [ebp-E8h]
  float v4; // [esp+18h] [ebp-E4h]
  float v5; // [esp+1Ch] [ebp-E0h]
  float v6; // [esp+20h] [ebp-DCh]
  float v7; // [esp+24h] [ebp-D8h]
  float v8; // [esp+28h] [ebp-D4h]
  float v9; // [esp+2Ch] [ebp-D0h]
  float v10; // [esp+30h] [ebp-CCh]
  float v11; // [esp+34h] [ebp-C8h]
  float v12; // [esp+38h] [ebp-C4h]
  float v13; // [esp+3Ch] [ebp-C0h]
  float v14; // [esp+40h] [ebp-BCh]
  float v15; // [esp+44h] [ebp-B8h]
  float v16; // [esp+48h] [ebp-B4h]
  float v17; // [esp+4Ch] [ebp-B0h]
  float v18; // [esp+50h] [ebp-ACh]
  float v19; // [esp+54h] [ebp-A8h]
  float v20; // [esp+58h] [ebp-A4h]
  float v21; // [esp+6Ch] [ebp-90h]
  float v22; // [esp+70h] [ebp-8Ch]
  float bloomExpansionYpos; // [esp+7Ch] [ebp-80h]
  float v24; // [esp+80h] [ebp-7Ch]
  float v25; // [esp+84h] [ebp-78h]
  float v26; // [esp+90h] [ebp-6Ch]
  float v27; // [esp+94h] [ebp-68h]
  float v28; // [esp+98h] [ebp-64h]
  float tintWeightG; // [esp+E0h] [ebp-1Ch]
  float desaturationA; // [esp+ECh] [ebp-10h]
  float tintWeightB; // [esp+F8h] [ebp-4h]

  tintWeightG = viewInfo->bloom.bloomTintWeights[1];
  tintWeightB = viewInfo->bloom.bloomTintWeights[2];
  desaturationA = viewInfo->bloom.bloomTintWeights[3];
  if ( (float)(viewInfo->bloom.bloomColorScale[3] * 4.0) <= 32.0 )
    v20 = viewInfo->bloom.bloomColorScale[3] * 4.0;
  else
    v20 = FLOAT_32_0;
  if ( (float)(viewInfo->bloom.bloomTintScale[3] * 4.0) <= 32.0 )
    v19 = viewInfo->bloom.bloomTintScale[3] * 4.0;
  else
    v19 = FLOAT_32_0;
  if ( v20 < v19 )
    v18 = v19;
  else
    v18 = v20 + 0.000030517578;
  if ( (float)(viewInfo->bloom.bloomCurveBreakpoint[0] * 4.0) >= 0.000030517578 )
    v17 = viewInfo->bloom.bloomCurveBreakpoint[0] * 4.0;
  else
    v17 = FLOAT_0_000030517578;
  if ( (float)(viewInfo->bloom.bloomCurveBreakpoint[1] * 4.0) >= 0.000030517578 )
    v16 = viewInfo->bloom.bloomCurveBreakpoint[1] * 4.0;
  else
    v16 = FLOAT_0_000030517578;
  if ( (float)(viewInfo->bloom.bloomCurveBreakpoint[2] * 4.0) >= 0.000030517578 )
    v15 = viewInfo->bloom.bloomCurveBreakpoint[2] * 4.0;
  else
    v15 = FLOAT_0_000030517578;
  if ( (float)(viewInfo->bloom.bloomCurveBreakpoint[3] * 4.0) >= 0.000030517578 )
    v14 = viewInfo->bloom.bloomCurveBreakpoint[3] * 4.0;
  else
    v14 = FLOAT_0_000030517578;
  if ( v17 <= 32.0 )
    v13 = v17;
  else
    v13 = FLOAT_32_0;
  if ( v16 <= 32.0 )
    v12 = v16;
  else
    v12 = FLOAT_32_0;
  if ( v15 <= 32.0 )
    v11 = v15;
  else
    v11 = FLOAT_32_0;
  if ( v14 <= 32.0 )
    v10 = v14;
  else
    v10 = FLOAT_32_0;
  gfxCmdBufSourceState.input.consts[121][0] = viewInfo->bloom.bloomTintWeights[0];
  gfxCmdBufSourceState.input.consts[121][1] = tintWeightG;
  gfxCmdBufSourceState.input.consts[121][2] = tintWeightB;
  gfxCmdBufSourceState.input.consts[121][3] = desaturationA;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x79u);
  gfxCmdBufSourceState.input.consts[122][0] = 1.0 / (float)((float)(v18 - v20) / 32.0);
  gfxCmdBufSourceState.input.consts[122][1] = COERCE_FLOAT(COERCE_UNSIGNED_INT(v20 / 32.0) ^ _mask__NegFloat_)
                                            * (float)(1.0 / (float)((float)(v18 - v20) / 32.0));
  gfxCmdBufSourceState.input.consts[122][2] = FLOAT_0_25;
  gfxCmdBufSourceState.input.consts[122][3] = FLOAT_0_5;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Au);
  gfxCmdBufSourceState.input.consts[123][0] = 1.0 / (float)(v13 / 32.0);
  gfxCmdBufSourceState.input.consts[123][1] = 1.0 / (float)(v12 / 32.0);
  gfxCmdBufSourceState.input.consts[123][2] = 1.0 / (float)(v11 / 32.0);
  gfxCmdBufSourceState.input.consts[123][3] = 1.0 / (float)(v10 / 32.0);
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Bu);
  gfxCmdBufSourceState.input.consts[124][0] = 1.0 / (float)((float)(32.0 - v13) / 32.0);
  gfxCmdBufSourceState.input.consts[124][1] = 1.0 / (float)((float)(32.0 - v12) / 32.0);
  gfxCmdBufSourceState.input.consts[124][2] = 1.0 / (float)((float)(32.0 - v11) / 32.0);
  gfxCmdBufSourceState.input.consts[124][3] = 1.0 / (float)((float)(32.0 - v10) / 32.0);
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Cu);
  gfxCmdBufSourceState.input.consts[125][0] = COERCE_FLOAT(COERCE_UNSIGNED_INT(v13 / 32.0) ^ _mask__NegFloat_)
                                            * (float)(1.0 / (float)((float)(32.0 - v13) / 32.0));
  gfxCmdBufSourceState.input.consts[125][1] = COERCE_FLOAT(COERCE_UNSIGNED_INT(v12 / 32.0) ^ _mask__NegFloat_)
                                            * (float)(1.0 / (float)((float)(32.0 - v12) / 32.0));
  gfxCmdBufSourceState.input.consts[125][2] = COERCE_FLOAT(COERCE_UNSIGNED_INT(v11 / 32.0) ^ _mask__NegFloat_)
                                            * (float)(1.0 / (float)((float)(32.0 - v11) / 32.0));
  gfxCmdBufSourceState.input.consts[125][3] = COERCE_FLOAT(COERCE_UNSIGNED_INT(v10 / 32.0) ^ _mask__NegFloat_)
                                            * (float)(1.0 / (float)((float)(32.0 - v10) / 32.0));
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Du);
  if ( viewInfo->bloom.bloomCurveLoGamma[0] == 0.0 )
    v9 = FLOAT_0_1;
  else
    v9 = viewInfo->bloom.bloomCurveLoGamma[0];
  if ( viewInfo->bloom.bloomCurveLoGamma[0] == 0.0 )
    v8 = FLOAT_0_1;
  else
    v8 = viewInfo->bloom.bloomCurveLoGamma[1];
  if ( viewInfo->bloom.bloomCurveLoGamma[0] == 0.0 )
    v7 = FLOAT_0_1;
  else
    v7 = viewInfo->bloom.bloomCurveLoGamma[2];
  if ( viewInfo->bloom.bloomCurveLoGamma[0] == 0.0 )
    v6 = FLOAT_0_1;
  else
    v6 = viewInfo->bloom.bloomCurveLoGamma[3];
  gfxCmdBufSourceState.input.consts[126][0] = 1.0 / v9;
  gfxCmdBufSourceState.input.consts[126][1] = 1.0 / v8;
  gfxCmdBufSourceState.input.consts[126][2] = 1.0 / v7;
  gfxCmdBufSourceState.input.consts[126][3] = 1.0 / v6;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Eu);
  if ( viewInfo->bloom.bloomCurveHiGamma[0] == 0.0 )
    v5 = FLOAT_0_1;
  else
    v5 = viewInfo->bloom.bloomCurveHiGamma[0];
  if ( viewInfo->bloom.bloomCurveHiGamma[0] == 0.0 )
    v4 = FLOAT_0_1;
  else
    v4 = viewInfo->bloom.bloomCurveHiGamma[1];
  if ( viewInfo->bloom.bloomCurveHiGamma[0] == 0.0 )
    v3 = FLOAT_0_1;
  else
    v3 = viewInfo->bloom.bloomCurveHiGamma[2];
  if ( viewInfo->bloom.bloomCurveHiGamma[0] == 0.0 )
    v2 = FLOAT_0_1;
  else
    v2 = viewInfo->bloom.bloomCurveHiGamma[3];
  gfxCmdBufSourceState.input.consts[127][0] = 1.0 / v5;
  gfxCmdBufSourceState.input.consts[127][1] = 1.0 / v4;
  gfxCmdBufSourceState.input.consts[127][2] = 1.0 / v3;
  gfxCmdBufSourceState.input.consts[127][3] = 1.0 / v2;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Fu);
  gfxCmdBufSourceState.input.consts[128][0] = v13 / 32.0;
  gfxCmdBufSourceState.input.consts[128][1] = v12 / 32.0;
  gfxCmdBufSourceState.input.consts[128][2] = v11 / 32.0;
  gfxCmdBufSourceState.input.consts[128][3] = v10 / 32.0;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x80u);
  gfxCmdBufSourceState.input.consts[129][0] = (float)(32.0 - v13) / 32.0;
  gfxCmdBufSourceState.input.consts[129][1] = (float)(32.0 - v12) / 32.0;
  gfxCmdBufSourceState.input.consts[129][2] = (float)(32.0 - v11) / 32.0;
  gfxCmdBufSourceState.input.consts[129][3] = (float)(32.0 - v10) / 32.0;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x81u);
  v26 = viewInfo->bloom.bloomExpansionWeights[1] / 16.0;
  v27 = viewInfo->bloom.bloomExpansionWeights[2] / 16.0;
  v28 = viewInfo->bloom.bloomExpansionWeights[3] / 16.0;
  gfxCmdBufSourceState.input.consts[130][0] = viewInfo->bloom.bloomExpansionWeights[0] / 16.0;
  gfxCmdBufSourceState.input.consts[130][1] = v26;
  gfxCmdBufSourceState.input.consts[130][2] = v27;
  gfxCmdBufSourceState.input.consts[130][3] = v28;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x82u);
  bloomExpansionYpos = viewInfo->bloomExpansionYpos;
  v24 = viewInfo->bloom.bloomExpansionControl[0];
  v25 = viewInfo->bloom.bloomExpansionControl[1];
  gfxCmdBufSourceState.input.consts[133][0] = viewInfo->bloomExpansionXpos;
  gfxCmdBufSourceState.input.consts[133][1] = bloomExpansionYpos;
  gfxCmdBufSourceState.input.consts[133][2] = v24;
  gfxCmdBufSourceState.input.consts[133][3] = v25;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x85u);
  v21 = viewInfo->bloom.bloomColorScale[1];
  v22 = viewInfo->bloom.bloomColorScale[2];
  gfxCmdBufSourceState.input.consts[135][0] = viewInfo->bloom.bloomColorScale[0];
  gfxCmdBufSourceState.input.consts[135][1] = v21;
  gfxCmdBufSourceState.input.consts[135][2] = v22;
  gfxCmdBufSourceState.input.consts[135][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x87u);
  Vec4Set(
    gfxCmdBufSourceState.input.consts[136],
    viewInfo->bloom.bloomTintScale[0],
    viewInfo->bloom.bloomTintScale[1],
    viewInfo->bloom.bloomTintScale[2],
    0.03125);
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x88u);
  return (float)((float)((float)(viewInfo->bloom.bloomExpansionWeights[0] + viewInfo->bloom.bloomExpansionWeights[1])
                       + viewInfo->bloom.bloomExpansionWeights[2])
               + viewInfo->bloom.bloomExpansionWeights[3]) != 0.0;
}

void __cdecl RB_BloomStreak(const GfxViewInfo *viewInfo, unsigned __int8 *srcRt, unsigned __int8 *dstRt)
{
  float v3; // [esp+10h] [ebp-3B8h]
  float v4; // [esp+14h] [ebp-3B4h]
  float v5; // [esp+20h] [ebp-3A8h]
  float v6; // [esp+70h] [ebp-358h]
  float v7; // [esp+74h] [ebp-354h]
  float v8; // [esp+78h] [ebp-350h]
  float v9; // [esp+84h] [ebp-344h]
  float v10; // [esp+88h] [ebp-340h]
  float v11; // [esp+8Ch] [ebp-33Ch]
  float v12; // [esp+98h] [ebp-330h]
  float v13; // [esp+9Ch] [ebp-32Ch]
  float v14; // [esp+A0h] [ebp-328h]
  float v15; // [esp+ACh] [ebp-31Ch]
  float v16; // [esp+B0h] [ebp-318h]
  float v17; // [esp+B4h] [ebp-314h]
  float v18; // [esp+C0h] [ebp-308h]
  float v19; // [esp+C4h] [ebp-304h]
  float v20; // [esp+C8h] [ebp-300h]
  float v21; // [esp+D4h] [ebp-2F4h]
  float v22; // [esp+D8h] [ebp-2F0h]
  float v23; // [esp+DCh] [ebp-2ECh]
  float v24; // [esp+E8h] [ebp-2E0h]
  float v25; // [esp+ECh] [ebp-2DCh]
  float v26; // [esp+F0h] [ebp-2D8h]
  float v27; // [esp+FCh] [ebp-2CCh]
  float v28; // [esp+100h] [ebp-2C8h]
  float v29; // [esp+104h] [ebp-2C4h]
  float v30; // [esp+110h] [ebp-2B8h]
  float v31; // [esp+114h] [ebp-2B4h]
  float v32; // [esp+118h] [ebp-2B0h]
  float v33; // [esp+124h] [ebp-2A4h]
  float v34; // [esp+128h] [ebp-2A0h]
  float v35; // [esp+12Ch] [ebp-29Ch]
  float v36; // [esp+138h] [ebp-290h]
  float v37; // [esp+13Ch] [ebp-28Ch]
  float v38; // [esp+140h] [ebp-288h]
  float v39; // [esp+150h] [ebp-278h]
  float v40; // [esp+154h] [ebp-274h]
  float v41; // [esp+158h] [ebp-270h]
  float v42; // [esp+164h] [ebp-264h]
  float v43; // [esp+170h] [ebp-258h]
  float v44; // [esp+174h] [ebp-254h]
  float v45; // [esp+178h] [ebp-250h]
  float v46; // [esp+184h] [ebp-244h]
  float v47; // [esp+188h] [ebp-240h]
  float v48; // [esp+18Ch] [ebp-23Ch]
  float v49; // [esp+198h] [ebp-230h]
  float v50; // [esp+19Ch] [ebp-22Ch]
  float v51; // [esp+1A0h] [ebp-228h]
  float v52; // [esp+1ACh] [ebp-21Ch]
  float v53; // [esp+1B0h] [ebp-218h]
  float v54; // [esp+1B4h] [ebp-214h]
  float v55; // [esp+1C0h] [ebp-208h]
  float v56; // [esp+1C4h] [ebp-204h]
  float v57; // [esp+1C8h] [ebp-200h]
  float v58; // [esp+1D4h] [ebp-1F4h]
  float v59; // [esp+1D8h] [ebp-1F0h]
  float v60; // [esp+1DCh] [ebp-1ECh]
  float v61; // [esp+1E8h] [ebp-1E0h]
  float v62; // [esp+1ECh] [ebp-1DCh]
  float v63; // [esp+1F0h] [ebp-1D8h]
  float v64; // [esp+1FCh] [ebp-1CCh]
  float v65; // [esp+200h] [ebp-1C8h]
  float v66; // [esp+204h] [ebp-1C4h]
  float v67; // [esp+210h] [ebp-1B8h]
  float v68; // [esp+214h] [ebp-1B4h]
  float v69; // [esp+218h] [ebp-1B0h]
  float v70; // [esp+224h] [ebp-1A4h]
  float v71; // [esp+228h] [ebp-1A0h]
  float v72; // [esp+22Ch] [ebp-19Ch]
  float v73; // [esp+238h] [ebp-190h]
  float v74; // [esp+23Ch] [ebp-18Ch]
  float v75; // [esp+240h] [ebp-188h]
  float v76; // [esp+24Ch] [ebp-17Ch]
  float v77; // [esp+250h] [ebp-178h]
  float v78; // [esp+254h] [ebp-174h]
  float v79; // [esp+260h] [ebp-168h]
  float v80; // [esp+264h] [ebp-164h]
  float v81; // [esp+268h] [ebp-160h]
  float v82; // [esp+274h] [ebp-154h]
  float v83; // [esp+278h] [ebp-150h]
  float v84; // [esp+27Ch] [ebp-14Ch]
  float v85; // [esp+288h] [ebp-140h]
  float v86; // [esp+28Ch] [ebp-13Ch]
  float v87; // [esp+290h] [ebp-138h]
  float v88; // [esp+29Ch] [ebp-12Ch]
  float v89; // [esp+2A0h] [ebp-128h]
  float v90; // [esp+2A4h] [ebp-124h]
  float v91; // [esp+2B4h] [ebp-114h]
  float v92; // [esp+2B8h] [ebp-110h]
  float v93; // [esp+2BCh] [ebp-10Ch]
  unsigned __int8 v94; // [esp+2C6h] [ebp-102h]
  unsigned __int8 tmp; // [esp+2C7h] [ebp-101h]
  unsigned int j; // [esp+2C8h] [ebp-100h]
  float dg; // [esp+2CCh] [ebp-FCh]
  float dga; // [esp+2CCh] [ebp-FCh]
  float r; // [esp+2D4h] [ebp-F4h]
  float ra; // [esp+2D4h] [ebp-F4h]
  float rb; // [esp+2D4h] [ebp-F4h]
  float rc; // [esp+2D4h] [ebp-F4h]
  float rd; // [esp+2D4h] [ebp-F4h]
  float re; // [esp+2D4h] [ebp-F4h]
  float rf; // [esp+2D4h] [ebp-F4h]
  float rg; // [esp+2D4h] [ebp-F4h]
  float rh; // [esp+2D4h] [ebp-F4h]
  float ri; // [esp+2D4h] [ebp-F4h]
  float rj; // [esp+2D4h] [ebp-F4h]
  float rk; // [esp+2D4h] [ebp-F4h]
  float rl; // [esp+2D4h] [ebp-F4h]
  float rm; // [esp+2D4h] [ebp-F4h]
  float rn; // [esp+2D4h] [ebp-F4h]
  float ro; // [esp+2D4h] [ebp-F4h]
  float rp; // [esp+2D4h] [ebp-F4h]
  float rq; // [esp+2D4h] [ebp-F4h]
  float rr; // [esp+2D4h] [ebp-F4h]
  float rs; // [esp+2D4h] [ebp-F4h]
  float rt; // [esp+2D4h] [ebp-F4h]
  float ru; // [esp+2D4h] [ebp-F4h]
  float rv; // [esp+2D4h] [ebp-F4h]
  float rw; // [esp+2D4h] [ebp-F4h]
  float rx; // [esp+2D4h] [ebp-F4h]
  float ry; // [esp+2D4h] [ebp-F4h]
  float rz; // [esp+2D4h] [ebp-F4h]
  float rba; // [esp+2D4h] [ebp-F4h]
  float rbb; // [esp+2D4h] [ebp-F4h]
  float rbc; // [esp+2D4h] [ebp-F4h]
  float rbd; // [esp+2D4h] [ebp-F4h]
  float rbe; // [esp+2D4h] [ebp-F4h]
  float wb; // [esp+2D8h] [ebp-F0h]
  float wba; // [esp+2D8h] [ebp-F0h]
  float dr; // [esp+2DCh] [ebp-ECh]
  float dra; // [esp+2DCh] [ebp-ECh]
  float sampleTaps[16]; // [esp+2E0h] [ebp-E8h]
  float g; // [esp+320h] [ebp-A8h]
  float b; // [esp+324h] [ebp-A4h]
  float ob; // [esp+328h] [ebp-A0h]
  float pSize; // [esp+32Ch] [ebp-9Ch]
  float wr; // [esp+330h] [ebp-98h]
  unsigned int i; // [esp+334h] [ebp-94h]
  float ib; // [esp+338h] [ebp-90h]
  float wg; // [esp+33Ch] [ebp-8Ch]
  float sampleWeight[32]; // [esp+340h] [ebp-88h]
  float ow; // [esp+3C0h] [ebp-8h]
  float db; // [esp+3C4h] [ebp-4h]

  for ( i = 0; i < 0x20; ++i )
  {
    v5 = pow(0.89999998, (double)i);
    sampleWeight[i] = v5;
  }
  j = 0;
  for ( i = 0; i < 0x10; ++i )
  {
    sampleTaps[i] = (double)j + sampleWeight[j + 1] / (sampleWeight[j] + sampleWeight[j + 1]);
    sampleWeight[i] = sampleWeight[j] + sampleWeight[j + 1];
    j += 2;
  }
  if ( (float)((float)(viewInfo->bloom.bloomStreakXTint[0] + viewInfo->bloom.bloomStreakXTint[1])
             + viewInfo->bloom.bloomStreakXTint[2]) != 0.0 )
  {
    R_SetRenderTargetSize(&gfxCmdBufSourceState, 0x1Eu);
    R_SetRenderTarget(gfxCmdBufContext, 0x1Eu);
    R_ClearRenderTargetForMultiGpu(gfxCmdBufContext, 0x1Eu);
    ib = viewInfo->bloom.bloomStreakXLevels1[0];
    if ( ib < viewInfo->bloom.bloomStreakXLevels1[1] )
      v4 = viewInfo->bloom.bloomStreakXLevels1[1];
    else
      v4 = ib + 0.00048828125;
    ob = viewInfo->bloom.bloomStreakXLevels1[2];
    ow = viewInfo->bloom.bloomStreakXLevels1[3];
    v91 = (float)(1.0 / (float)(v4 - ib)) * viewInfo->bloom.bloomStreakXLevels0[1];
    v92 = (float)(1.0 / (float)(v4 - ib)) * viewInfo->bloom.bloomStreakXLevels0[2];
    v93 = 1.0 / viewInfo->bloom.bloomStreakXLevels0[3];
    gfxCmdBufSourceState.input.consts[121][0] = (float)(1.0 / (float)(v4 - ib)) * viewInfo->bloom.bloomStreakXLevels0[0];
    gfxCmdBufSourceState.input.consts[121][1] = v91;
    gfxCmdBufSourceState.input.consts[121][2] = v92;
    gfxCmdBufSourceState.input.consts[121][3] = v93;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x79u);
    gfxCmdBufSourceState.input.consts[122][0] = COERCE_FLOAT(LODWORD(ib) ^ _mask__NegFloat_)
                                              * (float)(1.0 / (float)(v4 - ib));
    gfxCmdBufSourceState.input.consts[122][1] = ow - ob;
    gfxCmdBufSourceState.input.consts[122][2] = ob;
    gfxCmdBufSourceState.input.consts[122][3] = *(float *)&FLOAT_0_0;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Au);
    R_SetCodeImageTexture(&gfxCmdBufSourceState, 0x22u, stru_B50EA4C.image);
    RB_Filter(rgp.bloomRemapStreak, viewInfo);
    R_SetCodeImageTexture(&gfxCmdBufSourceState, 0x22u, stru_B50EA88.image);
    r = viewInfo->bloom.bloomStreakXInnerTint[0];
    g = viewInfo->bloom.bloomStreakXInnerTint[1];
    b = viewInfo->bloom.bloomStreakXInnerTint[2];
    dr = (float)(viewInfo->bloom.bloomStreakXOuterTint[0] - viewInfo->bloom.bloomStreakXInnerTint[0]) * 0.0625;
    dg = (float)(viewInfo->bloom.bloomStreakXOuterTint[1] - viewInfo->bloom.bloomStreakXInnerTint[1]) * 0.0625;
    db = (float)(viewInfo->bloom.bloomStreakXOuterTint[2] - viewInfo->bloom.bloomStreakXInnerTint[2]) * 0.0625;
    pSize = 1.0 / (float)stru_B50EA88.width;
    v88 = g * sampleWeight[0];
    v89 = b * sampleWeight[0];
    v90 = sampleTaps[0] * pSize;
    gfxCmdBufSourceState.input.consts[121][0] = r * sampleWeight[0];
    gfxCmdBufSourceState.input.consts[121][1] = v88;
    gfxCmdBufSourceState.input.consts[121][2] = v89;
    gfxCmdBufSourceState.input.consts[121][3] = v90;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x79u);
    ra = r + dr;
    g = g + dg;
    b = b + db;
    v85 = g * sampleWeight[1];
    v86 = b * sampleWeight[1];
    v87 = sampleTaps[1] * pSize;
    gfxCmdBufSourceState.input.consts[122][0] = ra * sampleWeight[1];
    gfxCmdBufSourceState.input.consts[122][1] = v85;
    gfxCmdBufSourceState.input.consts[122][2] = v86;
    gfxCmdBufSourceState.input.consts[122][3] = v87;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Au);
    rb = ra + dr;
    g = g + dg;
    b = b + db;
    v82 = g * sampleWeight[2];
    v83 = b * sampleWeight[2];
    v84 = sampleTaps[2] * pSize;
    gfxCmdBufSourceState.input.consts[123][0] = rb * sampleWeight[2];
    gfxCmdBufSourceState.input.consts[123][1] = v82;
    gfxCmdBufSourceState.input.consts[123][2] = v83;
    gfxCmdBufSourceState.input.consts[123][3] = v84;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Bu);
    rc = rb + dr;
    g = g + dg;
    b = b + db;
    v79 = g * sampleWeight[3];
    v80 = b * sampleWeight[3];
    v81 = sampleTaps[3] * pSize;
    gfxCmdBufSourceState.input.consts[124][0] = rc * sampleWeight[3];
    gfxCmdBufSourceState.input.consts[124][1] = v79;
    gfxCmdBufSourceState.input.consts[124][2] = v80;
    gfxCmdBufSourceState.input.consts[124][3] = v81;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Cu);
    rd = rc + dr;
    g = g + dg;
    b = b + db;
    v76 = g * sampleWeight[4];
    v77 = b * sampleWeight[4];
    v78 = sampleTaps[4] * pSize;
    gfxCmdBufSourceState.input.consts[125][0] = rd * sampleWeight[4];
    gfxCmdBufSourceState.input.consts[125][1] = v76;
    gfxCmdBufSourceState.input.consts[125][2] = v77;
    gfxCmdBufSourceState.input.consts[125][3] = v78;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Du);
    re = rd + dr;
    g = g + dg;
    b = b + db;
    v73 = g * sampleWeight[5];
    v74 = b * sampleWeight[5];
    v75 = sampleTaps[5] * pSize;
    gfxCmdBufSourceState.input.consts[126][0] = re * sampleWeight[5];
    gfxCmdBufSourceState.input.consts[126][1] = v73;
    gfxCmdBufSourceState.input.consts[126][2] = v74;
    gfxCmdBufSourceState.input.consts[126][3] = v75;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Eu);
    rf = re + dr;
    g = g + dg;
    b = b + db;
    v70 = g * sampleWeight[6];
    v71 = b * sampleWeight[6];
    v72 = sampleTaps[6] * pSize;
    gfxCmdBufSourceState.input.consts[127][0] = rf * sampleWeight[6];
    gfxCmdBufSourceState.input.consts[127][1] = v70;
    gfxCmdBufSourceState.input.consts[127][2] = v71;
    gfxCmdBufSourceState.input.consts[127][3] = v72;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Fu);
    rg = rf + dr;
    g = g + dg;
    b = b + db;
    v67 = g * sampleWeight[7];
    v68 = b * sampleWeight[7];
    v69 = sampleTaps[7] * pSize;
    gfxCmdBufSourceState.input.consts[128][0] = rg * sampleWeight[7];
    gfxCmdBufSourceState.input.consts[128][1] = v67;
    gfxCmdBufSourceState.input.consts[128][2] = v68;
    gfxCmdBufSourceState.input.consts[128][3] = v69;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x80u);
    rh = rg + dr;
    g = g + dg;
    b = b + db;
    v64 = g * sampleWeight[8];
    v65 = b * sampleWeight[8];
    v66 = sampleTaps[8] * pSize;
    gfxCmdBufSourceState.input.consts[129][0] = rh * sampleWeight[8];
    gfxCmdBufSourceState.input.consts[129][1] = v64;
    gfxCmdBufSourceState.input.consts[129][2] = v65;
    gfxCmdBufSourceState.input.consts[129][3] = v66;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x81u);
    ri = rh + dr;
    g = g + dg;
    b = b + db;
    v61 = g * sampleWeight[9];
    v62 = b * sampleWeight[9];
    v63 = sampleTaps[9] * pSize;
    gfxCmdBufSourceState.input.consts[130][0] = ri * sampleWeight[9];
    gfxCmdBufSourceState.input.consts[130][1] = v61;
    gfxCmdBufSourceState.input.consts[130][2] = v62;
    gfxCmdBufSourceState.input.consts[130][3] = v63;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x82u);
    rj = ri + dr;
    g = g + dg;
    b = b + db;
    v58 = g * sampleWeight[10];
    v59 = b * sampleWeight[10];
    v60 = sampleTaps[10] * pSize;
    gfxCmdBufSourceState.input.consts[131][0] = rj * sampleWeight[10];
    gfxCmdBufSourceState.input.consts[131][1] = v58;
    gfxCmdBufSourceState.input.consts[131][2] = v59;
    gfxCmdBufSourceState.input.consts[131][3] = v60;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x83u);
    rk = rj + dr;
    g = g + dg;
    b = b + db;
    v55 = g * sampleWeight[11];
    v56 = b * sampleWeight[11];
    v57 = sampleTaps[11] * pSize;
    gfxCmdBufSourceState.input.consts[132][0] = rk * sampleWeight[11];
    gfxCmdBufSourceState.input.consts[132][1] = v55;
    gfxCmdBufSourceState.input.consts[132][2] = v56;
    gfxCmdBufSourceState.input.consts[132][3] = v57;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x84u);
    rl = rk + dr;
    g = g + dg;
    b = b + db;
    v52 = g * sampleWeight[12];
    v53 = b * sampleWeight[12];
    v54 = sampleTaps[12] * pSize;
    gfxCmdBufSourceState.input.consts[133][0] = rl * sampleWeight[12];
    gfxCmdBufSourceState.input.consts[133][1] = v52;
    gfxCmdBufSourceState.input.consts[133][2] = v53;
    gfxCmdBufSourceState.input.consts[133][3] = v54;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x85u);
    rm = rl + dr;
    g = g + dg;
    b = b + db;
    v49 = g * sampleWeight[13];
    v50 = b * sampleWeight[13];
    v51 = sampleTaps[13] * pSize;
    gfxCmdBufSourceState.input.consts[134][0] = rm * sampleWeight[13];
    gfxCmdBufSourceState.input.consts[134][1] = v49;
    gfxCmdBufSourceState.input.consts[134][2] = v50;
    gfxCmdBufSourceState.input.consts[134][3] = v51;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x86u);
    rn = rm + dr;
    g = g + dg;
    b = b + db;
    v46 = g * sampleWeight[14];
    v47 = b * sampleWeight[14];
    v48 = sampleTaps[14] * pSize;
    gfxCmdBufSourceState.input.consts[135][0] = rn * sampleWeight[14];
    gfxCmdBufSourceState.input.consts[135][1] = v46;
    gfxCmdBufSourceState.input.consts[135][2] = v47;
    gfxCmdBufSourceState.input.consts[135][3] = v48;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x87u);
    g = g + dg;
    b = b + db;
    v43 = g * sampleWeight[15];
    v44 = b * sampleWeight[15];
    v45 = sampleTaps[15] * pSize;
    gfxCmdBufSourceState.input.consts[136][0] = (float)(rn + dr) * sampleWeight[15];
    gfxCmdBufSourceState.input.consts[136][1] = v43;
    gfxCmdBufSourceState.input.consts[136][2] = v44;
    gfxCmdBufSourceState.input.consts[136][3] = v45;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x88u);
    wr = viewInfo->bloom.bloomStreakXTintControl[0];
    wg = viewInfo->bloom.bloomStreakXTintControl[1];
    wb = viewInfo->bloom.bloomStreakXTintControl[2];
    ro = viewInfo->bloom.bloomStreakXTint[0];
    g = viewInfo->bloom.bloomStreakXTint[1];
    b = viewInfo->bloom.bloomStreakXTint[2];
    g = g / 8.0;
    b = b / 8.0;
    v42 = viewInfo->bloom.bloomStreakXTintControl[3];
    gfxCmdBufSourceState.input.consts[137][0] = wr;
    gfxCmdBufSourceState.input.consts[137][1] = wg;
    gfxCmdBufSourceState.input.consts[137][2] = wb;
    gfxCmdBufSourceState.input.consts[137][3] = v42;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x89u);
    gfxCmdBufSourceState.input.consts[138][0] = ro / 8.0;
    gfxCmdBufSourceState.input.consts[138][1] = g;
    gfxCmdBufSourceState.input.consts[138][2] = b;
    gfxCmdBufSourceState.input.consts[138][3] = FLOAT_1_0;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x8Au);
    R_SetRenderTargetSize(&gfxCmdBufSourceState, *dstRt);
    R_SetRenderTarget(gfxCmdBufContext, *dstRt);
    R_SetCodeImageTexture(&gfxCmdBufSourceState, 0x23u, gfxRenderTargets[*srcRt].image);
    RB_Filter(rgp.bloomStreakX, viewInfo);
    tmp = *srcRt;
    *srcRt = *dstRt;
    *dstRt = tmp;
  }
  if ( (float)((float)(viewInfo->bloom.bloomStreakYTint[0] + viewInfo->bloom.bloomStreakYTint[1])
             + viewInfo->bloom.bloomStreakYTint[2]) != 0.0 )
  {
    R_SetRenderTargetSize(&gfxCmdBufSourceState, 0x1Eu);
    R_SetRenderTarget(gfxCmdBufContext, 0x1Eu);
    R_ClearRenderTargetForMultiGpu(gfxCmdBufContext, 0x1Eu);
    ib = viewInfo->bloom.bloomStreakYLevels1[0];
    if ( ib < viewInfo->bloom.bloomStreakYLevels1[1] )
      v3 = viewInfo->bloom.bloomStreakYLevels1[1];
    else
      v3 = ib + 0.00048828125;
    ob = viewInfo->bloom.bloomStreakYLevels1[2];
    ow = viewInfo->bloom.bloomStreakYLevels1[3];
    v39 = (float)(1.0 / (float)(v3 - ib)) * viewInfo->bloom.bloomStreakYLevels0[1];
    v40 = (float)(1.0 / (float)(v3 - ib)) * viewInfo->bloom.bloomStreakYLevels0[2];
    v41 = 1.0 / viewInfo->bloom.bloomStreakYLevels0[3];
    gfxCmdBufSourceState.input.consts[121][0] = (float)(1.0 / (float)(v3 - ib)) * viewInfo->bloom.bloomStreakYLevels0[0];
    gfxCmdBufSourceState.input.consts[121][1] = v39;
    gfxCmdBufSourceState.input.consts[121][2] = v40;
    gfxCmdBufSourceState.input.consts[121][3] = v41;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x79u);
    gfxCmdBufSourceState.input.consts[122][0] = COERCE_FLOAT(LODWORD(ib) ^ _mask__NegFloat_)
                                              * (float)(1.0 / (float)(v3 - ib));
    gfxCmdBufSourceState.input.consts[122][1] = ow - ob;
    gfxCmdBufSourceState.input.consts[122][2] = ob;
    gfxCmdBufSourceState.input.consts[122][3] = *(float *)&FLOAT_0_0;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Au);
    R_SetCodeImageTexture(&gfxCmdBufSourceState, 0x22u, stru_B50EA4C.image);
    RB_Filter(rgp.bloomRemapStreak, viewInfo);
    R_SetCodeImageTexture(&gfxCmdBufSourceState, 0x22u, stru_B50EA88.image);
    rp = viewInfo->bloom.bloomStreakYInnerTint[0];
    g = viewInfo->bloom.bloomStreakYInnerTint[1];
    b = viewInfo->bloom.bloomStreakYInnerTint[2];
    dra = (float)(viewInfo->bloom.bloomStreakYOuterTint[0] - viewInfo->bloom.bloomStreakYInnerTint[0]) * 0.0625;
    dga = (float)(viewInfo->bloom.bloomStreakYOuterTint[1] - viewInfo->bloom.bloomStreakYInnerTint[1]) * 0.0625;
    db = (float)(viewInfo->bloom.bloomStreakYOuterTint[2] - viewInfo->bloom.bloomStreakYInnerTint[2]) * 0.0625;
    pSize = 1.0 / (float)stru_B50EA88.height;
    v36 = g * sampleWeight[0];
    v37 = b * sampleWeight[0];
    v38 = sampleTaps[0] * pSize;
    gfxCmdBufSourceState.input.consts[121][0] = rp * sampleWeight[0];
    gfxCmdBufSourceState.input.consts[121][1] = v36;
    gfxCmdBufSourceState.input.consts[121][2] = v37;
    gfxCmdBufSourceState.input.consts[121][3] = v38;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x79u);
    rq = rp + dra;
    g = g + dga;
    b = b + db;
    v33 = g * sampleWeight[1];
    v34 = b * sampleWeight[1];
    v35 = sampleTaps[1] * pSize;
    gfxCmdBufSourceState.input.consts[122][0] = rq * sampleWeight[1];
    gfxCmdBufSourceState.input.consts[122][1] = v33;
    gfxCmdBufSourceState.input.consts[122][2] = v34;
    gfxCmdBufSourceState.input.consts[122][3] = v35;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Au);
    rr = rq + dra;
    g = g + dga;
    b = b + db;
    v30 = g * sampleWeight[2];
    v31 = b * sampleWeight[2];
    v32 = sampleTaps[2] * pSize;
    gfxCmdBufSourceState.input.consts[123][0] = rr * sampleWeight[2];
    gfxCmdBufSourceState.input.consts[123][1] = v30;
    gfxCmdBufSourceState.input.consts[123][2] = v31;
    gfxCmdBufSourceState.input.consts[123][3] = v32;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Bu);
    rs = rr + dra;
    g = g + dga;
    b = b + db;
    v27 = g * sampleWeight[3];
    v28 = b * sampleWeight[3];
    v29 = sampleTaps[3] * pSize;
    gfxCmdBufSourceState.input.consts[124][0] = rs * sampleWeight[3];
    gfxCmdBufSourceState.input.consts[124][1] = v27;
    gfxCmdBufSourceState.input.consts[124][2] = v28;
    gfxCmdBufSourceState.input.consts[124][3] = v29;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Cu);
    rt = rs + dra;
    g = g + dga;
    b = b + db;
    v24 = g * sampleWeight[4];
    v25 = b * sampleWeight[4];
    v26 = sampleTaps[4] * pSize;
    gfxCmdBufSourceState.input.consts[125][0] = rt * sampleWeight[4];
    gfxCmdBufSourceState.input.consts[125][1] = v24;
    gfxCmdBufSourceState.input.consts[125][2] = v25;
    gfxCmdBufSourceState.input.consts[125][3] = v26;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Du);
    ru = rt + dra;
    g = g + dga;
    b = b + db;
    v21 = g * sampleWeight[5];
    v22 = b * sampleWeight[5];
    v23 = sampleTaps[5] * pSize;
    gfxCmdBufSourceState.input.consts[126][0] = ru * sampleWeight[5];
    gfxCmdBufSourceState.input.consts[126][1] = v21;
    gfxCmdBufSourceState.input.consts[126][2] = v22;
    gfxCmdBufSourceState.input.consts[126][3] = v23;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Eu);
    rv = ru + dra;
    g = g + dga;
    b = b + db;
    v18 = g * sampleWeight[6];
    v19 = b * sampleWeight[6];
    v20 = sampleTaps[6] * pSize;
    gfxCmdBufSourceState.input.consts[127][0] = rv * sampleWeight[6];
    gfxCmdBufSourceState.input.consts[127][1] = v18;
    gfxCmdBufSourceState.input.consts[127][2] = v19;
    gfxCmdBufSourceState.input.consts[127][3] = v20;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Fu);
    rw = rv + dra;
    g = g + dga;
    b = b + db;
    v15 = g * sampleWeight[7];
    v16 = b * sampleWeight[7];
    v17 = sampleTaps[7] * pSize;
    gfxCmdBufSourceState.input.consts[128][0] = rw * sampleWeight[7];
    gfxCmdBufSourceState.input.consts[128][1] = v15;
    gfxCmdBufSourceState.input.consts[128][2] = v16;
    gfxCmdBufSourceState.input.consts[128][3] = v17;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x80u);
    rx = rw + dra;
    g = g + dga;
    b = b + db;
    v12 = g * sampleWeight[8];
    v13 = b * sampleWeight[8];
    v14 = sampleTaps[8] * pSize;
    gfxCmdBufSourceState.input.consts[129][0] = rx * sampleWeight[8];
    gfxCmdBufSourceState.input.consts[129][1] = v12;
    gfxCmdBufSourceState.input.consts[129][2] = v13;
    gfxCmdBufSourceState.input.consts[129][3] = v14;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x81u);
    ry = rx + dra;
    g = g + dga;
    b = b + db;
    v9 = g * sampleWeight[9];
    v10 = b * sampleWeight[9];
    v11 = sampleTaps[9] * pSize;
    gfxCmdBufSourceState.input.consts[130][0] = ry * sampleWeight[9];
    gfxCmdBufSourceState.input.consts[130][1] = v9;
    gfxCmdBufSourceState.input.consts[130][2] = v10;
    gfxCmdBufSourceState.input.consts[130][3] = v11;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x82u);
    rz = ry + dra;
    g = g + dga;
    b = b + db;
    v6 = g * sampleWeight[10];
    v7 = b * sampleWeight[10];
    v8 = sampleTaps[10] * pSize;
    gfxCmdBufSourceState.input.consts[131][0] = rz * sampleWeight[10];
    gfxCmdBufSourceState.input.consts[131][1] = v6;
    gfxCmdBufSourceState.input.consts[131][2] = v7;
    gfxCmdBufSourceState.input.consts[131][3] = v8;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x83u);
    rba = rz + dra;
    g = g + dga;
    b = b + db;
    Vec4Set(
      gfxCmdBufSourceState.input.consts[132],
      rba * sampleWeight[11],
      g * sampleWeight[11],
      b * sampleWeight[11],
      sampleTaps[11] * pSize);
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x84u);
    rbb = rba + dra;
    g = g + dga;
    b = b + db;
    Vec4Set(
      gfxCmdBufSourceState.input.consts[133],
      rbb * sampleWeight[12],
      g * sampleWeight[12],
      b * sampleWeight[12],
      sampleTaps[12] * pSize);
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x85u);
    rbc = rbb + dra;
    g = g + dga;
    b = b + db;
    Vec4Set(
      gfxCmdBufSourceState.input.consts[134],
      rbc * sampleWeight[13],
      g * sampleWeight[13],
      b * sampleWeight[13],
      sampleTaps[13] * pSize);
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x86u);
    rbd = rbc + dra;
    g = g + dga;
    b = b + db;
    Vec4Set(
      gfxCmdBufSourceState.input.consts[135],
      rbd * sampleWeight[14],
      g * sampleWeight[14],
      b * sampleWeight[14],
      sampleTaps[14] * pSize);
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x87u);
    g = g + dga;
    b = b + db;
    R_SetCodeConstant(
      &gfxCmdBufSourceState,
      0x88u,
      (float)(rbd + dra) * sampleWeight[15],
      g * sampleWeight[15],
      b * sampleWeight[15],
      sampleTaps[15] * pSize);
    wr = viewInfo->bloom.bloomStreakYTintControl[0];
    wg = viewInfo->bloom.bloomStreakYTintControl[1];
    wba = viewInfo->bloom.bloomStreakYTintControl[2];
    rbe = viewInfo->bloom.bloomStreakYTint[0];
    g = viewInfo->bloom.bloomStreakYTint[1];
    b = viewInfo->bloom.bloomStreakYTint[2];
    g = g / 8.0;
    b = b / 8.0;
    R_SetCodeConstant(&gfxCmdBufSourceState, 0x89u, wr, wg, wba, viewInfo->bloom.bloomStreakYTintControl[3]);
    R_SetCodeConstant(&gfxCmdBufSourceState, 0x8Au, rbe / 8.0, g, b, 1.0);
    R_SetRenderTargetSize(&gfxCmdBufSourceState, *dstRt);
    R_SetRenderTarget(gfxCmdBufContext, *dstRt);
    R_SetCodeImageTexture(&gfxCmdBufSourceState, 0x23u, gfxRenderTargets[*srcRt].image);
    RB_Filter(rgp.bloomStreakY, viewInfo);
    v94 = *srcRt;
    *srcRt = *dstRt;
    *dstRt = v94;
  }
}

void __cdecl RB_SetBlurConstants(float radius, float textureScaler)
{
  double v2; // xmm0_8
  long double v3; // [esp+0h] [ebp-A8h]
  float v4; // [esp+0h] [ebp-A8h]
  float v5; // [esp+4h] [ebp-A4h]
  float v6; // [esp+8h] [ebp-A0h]
  float v7; // [esp+Ch] [ebp-9Ch]
  float v8; // [esp+10h] [ebp-98h]
  float v9; // [esp+14h] [ebp-94h]
  float v10; // [esp+18h] [ebp-90h]
  float v11; // [esp+20h] [ebp-88h]
  float v12; // [esp+2Ch] [ebp-7Ch]
  float v13; // [esp+30h] [ebp-78h]
  float v14; // [esp+34h] [ebp-74h]
  float v15; // [esp+44h] [ebp-64h]
  int j; // [esp+48h] [ebp-60h]
  int i; // [esp+4Ch] [ebp-5Ch]
  float t; // [esp+50h] [ebp-58h]
  float blurWeights[12]; // [esp+64h] [ebp-44h]
  float tap67; // [esp+94h] [ebp-14h]
  float tap89; // [esp+98h] [ebp-10h]
  float x; // [esp+9Ch] [ebp-Ch]
  float tapAB; // [esp+A0h] [ebp-8h]
  float tap45; // [esp+A4h] [ebp-4h]

  if ( (float)(radius - 3.0) < 0.0 )
    v15 = radius;
  else
    v15 = FLOAT_3_0;
  if ( (float)(1.0 - radius) < 0.0 )
    v10 = v15;
  else
    v10 = FLOAT_1_0;
  x = FLOAT_0_5;
  t = *(float *)&FLOAT_0_0;
  for ( i = 0; i < 12; ++i )
  {
    v2 = (float)((float)(x * x) * (float)(-0.5 / (float)(v10 * v10)));
    __libm_sse2_exp(v3);
    *(float *)&v2 = v2;
    blurWeights[i] = *(float *)&v2;
    t = (float)(*(float *)&v2 + *(float *)&v2) + t;
    x = x + 1.0;
  }
  for ( j = 0; j < 12; ++j )
    blurWeights[j] = blurWeights[j] / t;
  if ( (float)(blurWeights[0] + blurWeights[1]) == 0.0 )
    v9 = *(float *)&FLOAT_0_0;
  else
    v9 = (float)(blurWeights[1] / (float)(blurWeights[0] + blurWeights[1])) + 0.0;
  if ( (float)(blurWeights[2] + blurWeights[3]) == 0.0 )
    v8 = *(float *)&FLOAT_0_0;
  else
    v8 = (float)(blurWeights[3] / (float)(blurWeights[2] + blurWeights[3])) + 2.0;
  if ( (float)(blurWeights[4] + blurWeights[5]) == 0.0 )
    v7 = *(float *)&FLOAT_0_0;
  else
    v7 = (float)(blurWeights[5] / (float)(blurWeights[4] + blurWeights[5])) + 4.0;
  tap45 = v7;
  if ( (float)(blurWeights[6] + blurWeights[7]) == 0.0 )
    v6 = *(float *)&FLOAT_0_0;
  else
    v6 = (float)(blurWeights[7] / (float)(blurWeights[6] + blurWeights[7])) + 6.0;
  tap67 = v6;
  if ( (float)(blurWeights[8] + blurWeights[9]) == 0.0 )
    v5 = *(float *)&FLOAT_0_0;
  else
    v5 = (float)(blurWeights[9] / (float)(blurWeights[8] + blurWeights[9])) + 8.0;
  tap89 = v5;
  if ( (float)(blurWeights[10] + blurWeights[11]) == 0.0 )
    v4 = *(float *)&FLOAT_0_0;
  else
    v4 = (float)(blurWeights[11] / (float)(blurWeights[10] + blurWeights[11])) + 10.0;
  tap45 = tap45 * textureScaler;
  tap67 = tap67 * textureScaler;
  tap89 = tap89 * textureScaler;
  tapAB = v4 * textureScaler;
  gfxCmdBufSourceState.input.consts[121][0] = v9 * textureScaler;
  gfxCmdBufSourceState.input.consts[121][1] = v8 * textureScaler;
  gfxCmdBufSourceState.input.consts[121][2] = tap45;
  gfxCmdBufSourceState.input.consts[121][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x79u);
  gfxCmdBufSourceState.input.consts[122][0] = tap67;
  gfxCmdBufSourceState.input.consts[122][1] = tap89;
  gfxCmdBufSourceState.input.consts[122][2] = tapAB;
  gfxCmdBufSourceState.input.consts[122][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Au);
  v12 = blurWeights[2] + blurWeights[3];
  v13 = blurWeights[4] + blurWeights[5];
  v14 = blurWeights[6] + blurWeights[7];
  gfxCmdBufSourceState.input.consts[123][0] = blurWeights[0] + blurWeights[1];
  gfxCmdBufSourceState.input.consts[123][1] = v12;
  gfxCmdBufSourceState.input.consts[123][2] = v13;
  gfxCmdBufSourceState.input.consts[123][3] = v14;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Bu);
  v11 = blurWeights[10] + blurWeights[11];
  gfxCmdBufSourceState.input.consts[124][0] = blurWeights[8] + blurWeights[9];
  gfxCmdBufSourceState.input.consts[124][1] = v11;
  gfxCmdBufSourceState.input.consts[124][2] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[124][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Cu);
}

void __cdecl RB_SetFilmCurveConstants(const GfxViewInfo *viewInfo)
{
  float D; // [esp+Ch] [ebp-20h]
  float A; // [esp+10h] [ebp-1Ch]
  float F; // [esp+18h] [ebp-14h]
  float C; // [esp+1Ch] [ebp-10h]
  float B; // [esp+20h] [ebp-Ch]
  float E; // [esp+28h] [ebp-4h]

  A = viewInfo->film.sCurveShoulderStrength;
  B = viewInfo->film.sCurveLinearStrength;
  C = viewInfo->film.sCurveLinearAngle;
  D = viewInfo->film.sCurveToeStrength;
  E = viewInfo->film.sCurveToeNumerator;
  F = viewInfo->film.sCurveToeDenominator;
  gfxCmdBufSourceState.input.consts[121][0] = A;
  gfxCmdBufSourceState.input.consts[121][1] = B;
  gfxCmdBufSourceState.input.consts[121][2] = D * F;
  gfxCmdBufSourceState.input.consts[121][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x79u);
  gfxCmdBufSourceState.input.consts[122][0] = A;
  gfxCmdBufSourceState.input.consts[122][1] = C * B;
  gfxCmdBufSourceState.input.consts[122][2] = D * E;
  gfxCmdBufSourceState.input.consts[122][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Au);
  gfxCmdBufSourceState.input.consts[123][0] = COERCE_FLOAT(LODWORD(E) ^ _mask__NegFloat_) / F;
  gfxCmdBufSourceState.input.consts[123][1] = 1.0
                                            / (float)((float)((float)((float)((float)((float)(A * 8.0) + (float)(C * B))
                                                                            * 8.0)
                                                                    + (float)(D * E))
                                                            / (float)((float)((float)((float)(A * 8.0) + B) * 8.0)
                                                                    + (float)(D * F)))
                                                    - (float)(E / F));
  gfxCmdBufSourceState.input.consts[123][2] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[123][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Bu);
}

// local variable allocation has failed, the output may be wrong!
void  RB_SetVisionSetColorCorrection(int a1@<ebp>, const GfxViewInfo *viewInfo)
{
  float filmMidEnd; // xmm0_4
  float v3; // xmm0_4
  float v4; // xmm0_4
  float v5; // [esp+18h] [ebp-274h]
  float z; // [esp+1Ch] [ebp-270h]
  float y; // [esp+20h] [ebp-26Ch]
  float midEAdd; // [esp+24h] [ebp-268h]
  float midSMul; // [esp+30h] [ebp-25Ch]
  float v10; // [esp+34h] [ebp-258h]
  float lE; // [esp+3Ch] [ebp-250h]
  float lEa; // [esp+3Ch] [ebp-250h]
  float filmMidStart; // [esp+48h] [ebp-244h]
  float midE; // [esp+50h] [ebp-23Ch]
  float midS; // [esp+54h] [ebp-238h]
  float x; // [esp+58h] [ebp-234h]
  float v17; // [esp+60h] [ebp-22Ch]
  float v18; // [esp+64h] [ebp-228h]
  float v19; // [esp+68h] [ebp-224h]
  float v20; // [esp+74h] [ebp-218h]
  float v21; // [esp+78h] [ebp-214h]
  float v22; // [esp+7Ch] [ebp-210h]
  float v23; // [esp+88h] [ebp-204h]
  float v24; // [esp+8Ch] [ebp-200h]
  float v25; // [esp+90h] [ebp-1FCh]
  float v26; // [esp+9Ch] [ebp-1F0h]
  float v27; // [esp+A0h] [ebp-1ECh]
  float v28; // [esp+A4h] [ebp-1E8h]
  float v29; // [esp+B0h] [ebp-1DCh]
  float v30; // [esp+B4h] [ebp-1D8h]
  float v31; // [esp+B8h] [ebp-1D4h]
  float v32; // [esp+C4h] [ebp-1C8h]
  float v33; // [esp+C8h] [ebp-1C4h]
  float v34; // [esp+CCh] [ebp-1C0h]
  float v35; // [esp+D8h] [ebp-1B4h]
  float v36; // [esp+DCh] [ebp-1B0h]
  float v37; // [esp+E0h] [ebp-1ACh]
  float v38; // [esp+ECh] [ebp-1A0h]
  float v39; // [esp+F0h] [ebp-19Ch]
  float v40; // [esp+F4h] [ebp-198h]
  float v41; // [esp+100h] [ebp-18Ch]
  float v42; // [esp+104h] [ebp-188h]
  float v43; // [esp+108h] [ebp-184h]
  _BYTE v44[272]; // [esp+110h] [ebp-17Ch] OVERLAPPED BYREF
  float *v45; // [esp+25Ch] [ebp-30h]
  float *v46; // [esp+260h] [ebp-2Ch]
  float *v47; // [esp+264h] [ebp-28h]
  float *v48; // [esp+268h] [ebp-24h]
  float *v49; // [esp+26Ch] [ebp-20h]
  float *v50; // [esp+270h] [ebp-1Ch]
  float *v51; // [esp+274h] [ebp-18h]
  float *v52; // [esp+278h] [ebp-14h]
  float *v53; // [esp+27Ch] [ebp-10h]
  int v54; // [esp+280h] [ebp-Ch]
  void *v55; // [esp+284h] [ebp-8h]
  void *retaddr; // [esp+28Ch] [ebp+0h]

  v54 = a1;
  v55 = retaddr;
  if ( r_filmTweakRangeDebug->current.enabled )
  {
    v53 = gfxCmdBufSourceState.input.consts[121];
    gfxCmdBufSourceState.input.consts[121][0] = FLOAT_0_25;
    v53[1] = FLOAT_0_5;
    v53[2] = FLOAT_0_25;
    v53[3] = *(float *)&FLOAT_0_0;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x79u);
    v52 = gfxCmdBufSourceState.input.consts[122];
    gfxCmdBufSourceState.input.consts[122][0] = *(float *)&FLOAT_0_0;
    v52[1] = *(float *)&FLOAT_0_0;
    v52[2] = *(float *)&FLOAT_0_0;
    v52[3] = *(float *)&FLOAT_0_0;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Au);
    v51 = gfxCmdBufSourceState.input.consts[123];
    gfxCmdBufSourceState.input.consts[123][0] = FLOAT_0_25;
    v51[1] = FLOAT_0_5;
    v51[2] = FLOAT_0_25;
    v51[3] = *(float *)&FLOAT_0_0;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Bu);
    v50 = gfxCmdBufSourceState.input.consts[124];
    gfxCmdBufSourceState.input.consts[124][0] = *(float *)&FLOAT_0_0;
    v50[1] = *(float *)&FLOAT_0_0;
    v50[2] = *(float *)&FLOAT_0_0;
    v50[3] = *(float *)&FLOAT_0_0;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Cu);
    v49 = gfxCmdBufSourceState.input.consts[125];
    gfxCmdBufSourceState.input.consts[125][0] = FLOAT_0_25;
    v49[1] = FLOAT_0_5;
    v49[2] = FLOAT_0_25;
    v49[3] = *(float *)&FLOAT_0_0;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Du);
    v48 = gfxCmdBufSourceState.input.consts[126];
    gfxCmdBufSourceState.input.consts[126][0] = FLOAT_0_25;
    v48[1] = FLOAT_0_5;
    v48[2] = FLOAT_0_25;
    v48[3] = *(float *)&FLOAT_0_0;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Eu);
    v47 = gfxCmdBufSourceState.input.consts[127];
    gfxCmdBufSourceState.input.consts[127][0] = FLOAT_0_25;
    v47[1] = FLOAT_0_5;
    v47[2] = FLOAT_0_25;
    v47[3] = *(float *)&FLOAT_0_0;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Fu);
    v46 = gfxCmdBufSourceState.input.consts[128];
    gfxCmdBufSourceState.input.consts[128][0] = FLOAT_0_25;
    v46[1] = FLOAT_0_5;
    v46[2] = FLOAT_0_25;
    v46[3] = *(float *)&FLOAT_0_0;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x80u);
    v45 = gfxCmdBufSourceState.input.consts[129];
    gfxCmdBufSourceState.input.consts[129][0] = *(float *)&FLOAT_0_0;
    v45[1] = *(float *)&FLOAT_0_0;
    v45[2] = *(float *)&FLOAT_0_0;
    v45[3] = *(float *)&FLOAT_0_0;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x81u);
  }
  else
  {
    colorTempMatrix((float (*)[4])&v44[256], viewInfo->film.filmColorTemp[0]);
    colorHueMatrix((float (*)[4])&v44[192], viewInfo->film.filmHue[0]);
    colorSaturationMatrix((float (*)[4])&v44[128], viewInfo->film.filmSaturation[0]);
    MatrixMultiply44((const float (*)[4])&v44[256], (const float (*)[4])&v44[192], (float (*)[4])&v44[64]);
    MatrixMultiply44((const float (*)[4])&v44[64], (const float (*)[4])&v44[128], (float (*)[4])v44);
    *(float *)v44 = (float)(viewInfo->film.filmDarkTint[0] * viewInfo->film.filmContrast[0]) * *(float *)v44;
    *(float *)&v44[16] = (float)(viewInfo->film.filmDarkTint[0] * viewInfo->film.filmContrast[0]) * *(float *)&v44[16];
    *(float *)&v44[32] = (float)(viewInfo->film.filmDarkTint[0] * viewInfo->film.filmContrast[0]) * *(float *)&v44[32];
    *(float *)&v44[4] = (float)(viewInfo->film.filmDarkTint[1] * viewInfo->film.filmContrast[0]) * *(float *)&v44[4];
    *(float *)&v44[20] = (float)(viewInfo->film.filmDarkTint[1] * viewInfo->film.filmContrast[0]) * *(float *)&v44[20];
    *(float *)&v44[36] = (float)(viewInfo->film.filmDarkTint[1] * viewInfo->film.filmContrast[0]) * *(float *)&v44[36];
    *(float *)&v44[8] = (float)(viewInfo->film.filmDarkTint[2] * viewInfo->film.filmContrast[0]) * *(float *)&v44[8];
    *(float *)&v44[24] = (float)(viewInfo->film.filmDarkTint[2] * viewInfo->film.filmContrast[0]) * *(float *)&v44[24];
    *(float *)&v44[40] = (float)(viewInfo->film.filmDarkTint[2] * viewInfo->film.filmContrast[0]) * *(float *)&v44[40];
    v43 = *(float *)&v44[16];
    v42 = *(float *)&v44[32];
    v41 = (float)(1.0 - viewInfo->film.filmContrast[0]) * 0.5;
    gfxCmdBufSourceState.input.consts[121][0] = *(float *)v44;
    gfxCmdBufSourceState.input.consts[121][1] = v43;
    gfxCmdBufSourceState.input.consts[121][2] = v42;
    gfxCmdBufSourceState.input.consts[121][3] = v41;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x79u);
    v40 = *(float *)&v44[20];
    v39 = *(float *)&v44[36];
    v38 = (float)(1.0 - viewInfo->film.filmContrast[0]) * 0.5;
    gfxCmdBufSourceState.input.consts[122][0] = *(float *)&v44[4];
    gfxCmdBufSourceState.input.consts[122][1] = v40;
    gfxCmdBufSourceState.input.consts[122][2] = v39;
    gfxCmdBufSourceState.input.consts[122][3] = v38;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Au);
    v37 = *(float *)&v44[24];
    v36 = *(float *)&v44[40];
    v35 = (float)(1.0 - viewInfo->film.filmContrast[0]) * 0.5;
    gfxCmdBufSourceState.input.consts[123][0] = *(float *)&v44[8];
    gfxCmdBufSourceState.input.consts[123][1] = v37;
    gfxCmdBufSourceState.input.consts[123][2] = v36;
    gfxCmdBufSourceState.input.consts[123][3] = v35;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Bu);
    colorTempMatrix((float (*)[4])&v44[256], viewInfo->film.filmColorTemp[1]);
    colorHueMatrix((float (*)[4])&v44[192], viewInfo->film.filmHue[1]);
    colorSaturationMatrix((float (*)[4])&v44[128], viewInfo->film.filmSaturation[1]);
    MatrixMultiply44((const float (*)[4])&v44[256], (const float (*)[4])&v44[192], (float (*)[4])&v44[64]);
    MatrixMultiply44((const float (*)[4])&v44[64], (const float (*)[4])&v44[128], (float (*)[4])v44);
    *(float *)v44 = (float)(viewInfo->film.filmMidTint[0] * viewInfo->film.filmContrast[1]) * *(float *)v44;
    *(float *)&v44[16] = (float)(viewInfo->film.filmMidTint[0] * viewInfo->film.filmContrast[1]) * *(float *)&v44[16];
    *(float *)&v44[32] = (float)(viewInfo->film.filmMidTint[0] * viewInfo->film.filmContrast[1]) * *(float *)&v44[32];
    *(float *)&v44[4] = (float)(viewInfo->film.filmMidTint[1] * viewInfo->film.filmContrast[1]) * *(float *)&v44[4];
    *(float *)&v44[20] = (float)(viewInfo->film.filmMidTint[1] * viewInfo->film.filmContrast[1]) * *(float *)&v44[20];
    *(float *)&v44[36] = (float)(viewInfo->film.filmMidTint[1] * viewInfo->film.filmContrast[1]) * *(float *)&v44[36];
    *(float *)&v44[8] = (float)(viewInfo->film.filmMidTint[2] * viewInfo->film.filmContrast[1]) * *(float *)&v44[8];
    *(float *)&v44[24] = (float)(viewInfo->film.filmMidTint[2] * viewInfo->film.filmContrast[1]) * *(float *)&v44[24];
    *(float *)&v44[40] = (float)(viewInfo->film.filmMidTint[2] * viewInfo->film.filmContrast[1]) * *(float *)&v44[40];
    v34 = *(float *)&v44[16];
    v33 = *(float *)&v44[32];
    v32 = (float)(1.0 - viewInfo->film.filmContrast[1]) * 0.5;
    gfxCmdBufSourceState.input.consts[124][0] = *(float *)v44;
    gfxCmdBufSourceState.input.consts[124][1] = v34;
    gfxCmdBufSourceState.input.consts[124][2] = v33;
    gfxCmdBufSourceState.input.consts[124][3] = v32;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Cu);
    v31 = *(float *)&v44[20];
    v30 = *(float *)&v44[36];
    v29 = (float)(1.0 - viewInfo->film.filmContrast[1]) * 0.5;
    gfxCmdBufSourceState.input.consts[125][0] = *(float *)&v44[4];
    gfxCmdBufSourceState.input.consts[125][1] = v31;
    gfxCmdBufSourceState.input.consts[125][2] = v30;
    gfxCmdBufSourceState.input.consts[125][3] = v29;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Du);
    v28 = *(float *)&v44[24];
    v27 = *(float *)&v44[40];
    v26 = (float)(1.0 - viewInfo->film.filmContrast[1]) * 0.5;
    gfxCmdBufSourceState.input.consts[126][0] = *(float *)&v44[8];
    gfxCmdBufSourceState.input.consts[126][1] = v28;
    gfxCmdBufSourceState.input.consts[126][2] = v27;
    gfxCmdBufSourceState.input.consts[126][3] = v26;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Eu);
    colorTempMatrix((float (*)[4])&v44[256], viewInfo->film.filmColorTemp[2]);
    colorHueMatrix((float (*)[4])&v44[192], viewInfo->film.filmHue[2]);
    colorSaturationMatrix((float (*)[4])&v44[128], viewInfo->film.filmSaturation[2]);
    MatrixMultiply44((const float (*)[4])&v44[256], (const float (*)[4])&v44[192], (float (*)[4])&v44[64]);
    MatrixMultiply44((const float (*)[4])&v44[64], (const float (*)[4])&v44[128], (float (*)[4])v44);
    *(float *)v44 = (float)(viewInfo->film.filmLightTint[0] * viewInfo->film.filmContrast[2]) * *(float *)v44;
    *(float *)&v44[16] = (float)(viewInfo->film.filmLightTint[0] * viewInfo->film.filmContrast[2]) * *(float *)&v44[16];
    *(float *)&v44[32] = (float)(viewInfo->film.filmLightTint[0] * viewInfo->film.filmContrast[2]) * *(float *)&v44[32];
    *(float *)&v44[4] = (float)(viewInfo->film.filmLightTint[1] * viewInfo->film.filmContrast[2]) * *(float *)&v44[4];
    *(float *)&v44[20] = (float)(viewInfo->film.filmLightTint[1] * viewInfo->film.filmContrast[2]) * *(float *)&v44[20];
    *(float *)&v44[36] = (float)(viewInfo->film.filmLightTint[1] * viewInfo->film.filmContrast[2]) * *(float *)&v44[36];
    *(float *)&v44[8] = (float)(viewInfo->film.filmLightTint[2] * viewInfo->film.filmContrast[2]) * *(float *)&v44[8];
    *(float *)&v44[24] = (float)(viewInfo->film.filmLightTint[2] * viewInfo->film.filmContrast[2]) * *(float *)&v44[24];
    *(float *)&v44[40] = (float)(viewInfo->film.filmLightTint[2] * viewInfo->film.filmContrast[2]) * *(float *)&v44[40];
    v25 = *(float *)&v44[16];
    v24 = *(float *)&v44[32];
    v23 = (float)(1.0 - viewInfo->film.filmContrast[2]) * 0.5;
    gfxCmdBufSourceState.input.consts[127][0] = *(float *)v44;
    gfxCmdBufSourceState.input.consts[127][1] = v25;
    gfxCmdBufSourceState.input.consts[127][2] = v24;
    gfxCmdBufSourceState.input.consts[127][3] = v23;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Fu);
    v22 = *(float *)&v44[20];
    v21 = *(float *)&v44[36];
    v20 = (float)(1.0 - viewInfo->film.filmContrast[2]) * 0.5;
    gfxCmdBufSourceState.input.consts[128][0] = *(float *)&v44[4];
    gfxCmdBufSourceState.input.consts[128][1] = v22;
    gfxCmdBufSourceState.input.consts[128][2] = v21;
    gfxCmdBufSourceState.input.consts[128][3] = v20;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x80u);
    v19 = *(float *)&v44[24];
    v18 = *(float *)&v44[40];
    v17 = (float)(1.0 - viewInfo->film.filmContrast[2]) * 0.5;
    gfxCmdBufSourceState.input.consts[129][0] = *(float *)&v44[8];
    gfxCmdBufSourceState.input.consts[129][1] = v19;
    gfxCmdBufSourceState.input.consts[129][2] = v18;
    gfxCmdBufSourceState.input.consts[129][3] = v17;
    R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x81u);
  }
  x = viewInfo->film.filmBleach[0];
  midS = viewInfo->film.filmBleach[1];
  midE = viewInfo->film.filmBleach[2];
  if ( gfxCmdBufSourceState.input.consts[130][0] != x
    || gfxCmdBufSourceState.input.consts[130][1] != midS
    || gfxCmdBufSourceState.input.consts[130][2] != midE
    || gfxCmdBufSourceState.input.consts[130][3] != 1.0 )
  {
    R_SetCodeConstant(&gfxCmdBufSourceState, 0x82u, x, midS, midE, 1.0);
  }
  filmMidStart = viewInfo->film.filmMidStart;
  if ( filmMidStart <= viewInfo->film.filmMidEnd )
    filmMidEnd = viewInfo->film.filmMidEnd;
  else
    filmMidEnd = viewInfo->film.filmMidStart;
  lE = filmMidStart - (float)(filmMidStart * viewInfo->film.filmDarkFeather);
  v10 = filmMidEnd;
  midSMul = (float)((float)(1.0 - filmMidEnd) * viewInfo->film.filmLightFeather) + filmMidEnd;
  if ( lE == filmMidStart )
    v3 = lE - 0.000061035156;
  else
    v3 = filmMidStart - (float)(filmMidStart * viewInfo->film.filmDarkFeather);
  lEa = v3;
  if ( v10 == midSMul )
    v4 = midSMul + 0.000061035156;
  else
    v4 = midSMul;
  midEAdd = 1.0 / (float)(filmMidStart - lEa);
  y = COERCE_FLOAT(LODWORD(lEa) ^ _mask__NegFloat_) * midEAdd;
  z = 1.0 / (float)(v4 - v10);
  v5 = COERCE_FLOAT(LODWORD(v10) ^ _mask__NegFloat_) * z;
  if ( gfxCmdBufSourceState.input.consts[131][0] != midEAdd
    || gfxCmdBufSourceState.input.consts[131][1] != y
    || gfxCmdBufSourceState.input.consts[131][2] != z
    || gfxCmdBufSourceState.input.consts[131][3] != v5 )
  {
    R_SetCodeConstant(&gfxCmdBufSourceState, 0x83u, midEAdd, y, z, v5);
  }
}

void __cdecl RB_BloomLDR(const GfxViewInfo *viewInfo)
{
  float v1; // [esp+14h] [ebp-50h]
  float v2; // [esp+18h] [ebp-4Ch]
  float v3; // [esp+1Ch] [ebp-48h]
  float v4; // [esp+28h] [ebp-3Ch]
  float v5; // [esp+2Ch] [ebp-38h]
  float v6; // [esp+30h] [ebp-34h]
  float v7; // [esp+3Ch] [ebp-28h]
  float v8; // [esp+40h] [ebp-24h]
  float v9; // [esp+44h] [ebp-20h]
  float v10; // [esp+50h] [ebp-14h]
  float v11; // [esp+54h] [ebp-10h]
  float v12; // [esp+58h] [ebp-Ch]
  unsigned __int8 srcRt; // [esp+61h] [ebp-3h] BYREF
  unsigned __int8 dstRt; // [esp+62h] [ebp-2h] BYREF
  unsigned __int8 tmp; // [esp+63h] [ebp-1h]
  int savedregs; // [esp+64h] [ebp+0h] BYREF

  //PIXBeginNamedEvent(-1, "LDR Bloom");
  R_SetRenderTargetSize(&gfxCmdBufSourceState, 0x19u);
  R_SetRenderTarget(gfxCmdBufContext, 0x19u);
  R_ClearRenderTargetForMultiGpu(gfxCmdBufContext, 0x19u);
  v10 = -2.0 / (float)stru_B50E8A8.height;
  v11 = 2.0 / (float)stru_B50E8A8.width;
  v12 = 2.0 / (float)stru_B50E8A8.height;
  gfxCmdBufSourceState.input.consts[121][0] = -2.0 / (float)stru_B50E8A8.width;
  gfxCmdBufSourceState.input.consts[121][1] = v10;
  gfxCmdBufSourceState.input.consts[121][2] = v11;
  gfxCmdBufSourceState.input.consts[121][3] = v12;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x79u);
  R_SetCodeImageTexture(&gfxCmdBufSourceState, 0x22u, stru_B50E8A8.image);
  RB_Filter(rgp.bloomDownsampleLDR, viewInfo);
  R_SetRenderTargetSize(&gfxCmdBufSourceState, 0x1Au);
  R_SetRenderTarget(gfxCmdBufContext, 0x1Au);
  R_ClearRenderTargetForMultiGpu(gfxCmdBufContext, 0x1Au);
  R_SetCodeImageTexture(&gfxCmdBufSourceState, 0x22u, stru_B50EA24.image);
  RB_Filter(rgp.resampleFinal, viewInfo);
  R_SetRenderTargetSize(&gfxCmdBufSourceState, 0x1Bu);
  R_SetRenderTarget(gfxCmdBufContext, 0x1Bu);
  R_ClearRenderTargetForMultiGpu(gfxCmdBufContext, 0x1Bu);
  v7 = -2.0 / (float)stru_B50EA38.height;
  v8 = 2.0 / (float)stru_B50EA38.width;
  v9 = 2.0 / (float)stru_B50EA38.height;
  gfxCmdBufSourceState.input.consts[121][0] = -2.0 / (float)stru_B50EA38.width;
  gfxCmdBufSourceState.input.consts[121][1] = v7;
  gfxCmdBufSourceState.input.consts[121][2] = v8;
  gfxCmdBufSourceState.input.consts[121][3] = v9;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x79u);
  R_SetCodeImageTexture(&gfxCmdBufSourceState, 0x22u, stru_B50EA38.image);
  RB_Filter(rgp.bloomDownsampleConvolution, viewInfo);
  R_SetRenderTargetSize(&gfxCmdBufSourceState, 0x1Cu);
  R_SetRenderTarget(gfxCmdBufContext, 0x1Cu);
  R_ClearRenderTargetForMultiGpu(gfxCmdBufContext, 0x1Cu);
  RB_SetBloomConstants(viewInfo);
  R_SetCodeImageTexture(&gfxCmdBufSourceState, 0x22u, stru_B50EA4C.image);
  R_SetCodeImageTexture(&gfxCmdBufSourceState, 0x23u, stru_B50EA4C.image);
  RB_Filter(rgp.bloomRemap, viewInfo);
  RB_SetBlurConstants(viewInfo->bloom.bloomBlurRadius, 1.0 / (float)stru_B50EA4C.width);
  R_SetRenderTargetSize(&gfxCmdBufSourceState, 0x1Du);
  R_SetRenderTarget(gfxCmdBufContext, 0x1Du);
  R_ClearRenderTargetForMultiGpu(gfxCmdBufContext, 0x1Du);
  R_SetCodeImageTexture(&gfxCmdBufSourceState, 0x22u, stru_B50EA60.image);
  RB_Filter(rgp.bloomBlurX, viewInfo);
  RB_SetBlurConstants(viewInfo->bloom.bloomBlurRadius, 1.0 / (float)stru_B50EA4C.height);
  R_SetRenderTargetSize(&gfxCmdBufSourceState, 0x1Cu);
  R_SetRenderTarget(gfxCmdBufContext, 0x1Cu);
  R_ClearRenderTargetForMultiGpu(gfxCmdBufContext, 0x1Cu);
  R_SetCodeImageTexture(&gfxCmdBufSourceState, 0x22u, stru_B50EA74.image);
  RB_Filter(rgp.bloomBlurY, viewInfo);
  srcRt = 28;
  dstRt = 29;
  RB_BloomStreak(viewInfo, &srcRt, &dstRt);
  v4 = 0.5 / (float)stru_B50EA4C.height;
  v5 = 0.5 / (float)stru_B50EA4C.width;
  v6 = -1.5 / (float)stru_B50EA4C.height;
  gfxCmdBufSourceState.input.consts[121][0] = 1.5 / (float)stru_B50EA4C.width;
  gfxCmdBufSourceState.input.consts[121][1] = v4;
  gfxCmdBufSourceState.input.consts[121][2] = v5;
  gfxCmdBufSourceState.input.consts[121][3] = v6;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x79u);
  v1 = -0.5 / (float)stru_B50EA4C.height;
  v2 = -0.5 / (float)stru_B50EA4C.width;
  v3 = 1.5 / (float)stru_B50EA4C.height;
  gfxCmdBufSourceState.input.consts[122][0] = -1.5 / (float)stru_B50EA4C.width;
  gfxCmdBufSourceState.input.consts[122][1] = v1;
  gfxCmdBufSourceState.input.consts[122][2] = v2;
  gfxCmdBufSourceState.input.consts[122][3] = v3;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Au);
  R_SetRenderTargetSize(&gfxCmdBufSourceState, dstRt);
  R_SetRenderTarget(gfxCmdBufContext, dstRt);
  R_ClearRenderTargetForMultiGpu(gfxCmdBufContext, dstRt);
  R_SetCodeImageTexture(&gfxCmdBufSourceState, 0x22u, gfxRenderTargets[srcRt].image);
  RB_Filter(rgp.bloomSmoothA, viewInfo);
  tmp = srcRt;
  srcRt = dstRt;
  dstRt = tmp;
  R_SetRenderTargetSize(&gfxCmdBufSourceState, 3u);
  R_SetRenderTarget(gfxCmdBufContext, 3u);
  RB_SetFilmCurveConstants(viewInfo);
  R_SetCodeImageTexture(&gfxCmdBufSourceState, 0x22u, stru_B50E8A8.image);
  R_SetCodeImageTexture(&gfxCmdBufSourceState, 0x23u, gfxRenderTargets[srcRt].image);
  R_SetCodeImageTexture(&gfxCmdBufSourceState, 0x24u, stru_B50E8A8.image);
  RB_Filter(rgp.bloomApplyLDR, viewInfo);
  RB_SetVisionSetColorCorrection((int)&savedregs, viewInfo);
  R_Resolve(gfxCmdBufContext, stru_B50E8A8.image);
  R_SetCodeImageTexture(&gfxCmdBufSourceState, 0xAu, stru_B50E8A8.image);
  RB_Filter(rgp.postFxColorMaterial, viewInfo);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

bool __cdecl R_UsingDoubleVision(const GfxViewInfo *viewInfo)
{
  return viewInfo->doubleVision.cur > 0.0;
}

bool __cdecl RB_UsingColorManipulation(const GfxViewInfo *viewInfo)
{
  if ( !viewInfo
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_postfx.cpp", 2191, 0, "%s", "viewInfo") )
  {
    __debugbreak();
  }
  return viewInfo->film.filmLut >= 1.0 || viewInfo->film.enabled;
}

void __cdecl RB_PoisonFX(const GfxViewInfo *viewInfo)
{
  double v1; // xmm0_8
  double v2; // xmm0_8
  double v3; // xmm0_8
  double v4; // xmm0_8
  double v5; // xmm0_8
  long double v6; // [esp+10h] [ebp-5Ch]
  long double v7; // [esp+10h] [ebp-5Ch]
  long double v8; // [esp+10h] [ebp-5Ch]
  long double v9; // [esp+10h] [ebp-5Ch]
  long double v10; // [esp+10h] [ebp-5Ch]
  long double v11; // [esp+10h] [ebp-5Ch]
  long double v12; // [esp+10h] [ebp-5Ch]
  float v13; // [esp+1Ch] [ebp-50h]
  float v14; // [esp+1Ch] [ebp-50h]
  float v15; // [esp+20h] [ebp-4Ch]
  float v16; // [esp+20h] [ebp-4Ch]
  float v17; // [esp+24h] [ebp-48h]
  float v18; // [esp+24h] [ebp-48h]
  float dvision0a; // [esp+2Ch] [ebp-40h]
  float dvision0; // [esp+2Ch] [ebp-40h]
  float dvision0b; // [esp+2Ch] [ebp-40h]
  float v22; // [esp+34h] [ebp-38h]
  float v23; // [esp+38h] [ebp-34h]
  float x; // [esp+44h] [ebp-28h]
  float v25; // [esp+48h] [ebp-24h]
  float pulseX1; // [esp+54h] [ebp-18h]
  float pulseX1a; // [esp+54h] [ebp-18h]
  float v28; // [esp+58h] [ebp-14h]
  float v29; // [esp+5Ch] [ebp-10h]
  float v30; // [esp+5Ch] [ebp-10h]

  R_SetRenderTargetSize(&gfxCmdBufSourceState, 3u);
  R_SetRenderTarget(gfxCmdBufContext, 3u);
  v29 = r_poisonFX_pulse->current.value - 1.0;
  if ( v29 < 0.0 )
    v29 = *(float *)&FLOAT_0_0;
  v30 = (float)((float)((float)(-2.0 * v29) + 3.0) * v29) * v29;
  pulseX1 = viewInfo->sceneDef.floatTime / 8.4499998;
  v1 = (float)((float)((float)(viewInfo->sceneDef.floatTime / 10.0) * 3.1415927) * 2.0);
  __libm_sse2_sin(v6);
  *(float *)&v1 = v1;
  v28 = r_poisonFX_warpX->current.value * *(float *)&v1;
  __libm_sse2_sin(v7);
  pulseX1a = r_poisonFX_warpY->current.value * (float)((float)((float)(pulseX1 * 3.1415927) * 2.0) + 1.5707964);
  x = viewInfo->sceneDef.floatTime / 4.2249999;
  v2 = (float)((float)((float)(viewInfo->sceneDef.floatTime / 5.0) * 3.1415927) * 2.0);
  __libm_sse2_sin(v8);
  *(float *)&v2 = v2;
  v25 = r_poisonFX_warpX->current.value * *(float *)&v2;
  __libm_sse2_sin(v9);
  v23 = (float)((float)((float)(1.0 - v30) * v28) + (float)(v30 * v25)) * viewInfo->poisonFx.curAmount;
  v22 = (float)((float)((float)(1.0 - v30) * pulseX1a)
              + (float)(v30
                      * (float)(r_poisonFX_warpY->current.value
                              * (float)((float)((float)(x * 3.1415927) * 2.0) + 1.5707964))))
      * viewInfo->poisonFx.curAmount;
  R_UpdateCodeConstant(
    &gfxCmdBufSourceState,
    0x79u,
    v23,
    v22,
    (float)(1.0 / (float)(1.0 - v23)) * 0.5,
    (float)(1.0 / (float)(1.0 - v22)) * 0.5);
  v3 = (float)(r_poisonFX_dvisionA->current.value * 0.017453292);
  __libm_sse2_cos(v10);
  *(float *)&v3 = v3;
  dvision0a = *(float *)&v3;
  v4 = (float)(r_poisonFX_dvisionA->current.value * 0.017453292);
  __libm_sse2_sin(v11);
  *(float *)&v4 = v4;
  dvision0 = dvision0a * r_poisonFX_dvisionX->current.value;
  v17 = *(float *)&v4 * r_poisonFX_dvisionY->current.value;
  v15 = viewInfo->sceneDef.floatTime / 4.0;
  if ( ((int)v15 & 1) != 0 )
  {
    v16 = *(float *)&FLOAT_0_0;
  }
  else
  {
    __libm_sse2_sin(v12);
    v16 = (float)(v15 * 3.1415927) * 3.25;
  }
  v13 = viewInfo->sceneDef.floatTime / 2.0;
  if ( ((int)v13 & 1) != 0 )
  {
    v14 = *(float *)&FLOAT_0_0;
  }
  else
  {
    __libm_sse2_sin(v12);
    v14 = (float)(v13 * 3.1415927) * 3.75;
  }
  v5 = (float)((float)((float)(viewInfo->sceneDef.floatTime / 12.0) * 3.1415927) * 2.0);
  __libm_sse2_cos(v12);
  *(float *)&v5 = v5;
  dvision0b = (float)((float)((float)(*(float *)&v5 * 1.25)
                            + (float)((float)((float)((float)(1.0 - v30) * v16) + (float)(v30 * v14)) + dvision0))
                    / viewInfo->fullSceneViewMesh->width)
            * viewInfo->poisonFx.curAmount;
  v18 = (float)(v17 / viewInfo->fullSceneViewMesh->height) * viewInfo->poisonFx.curAmount;
  R_UpdateCodeConstant(
    &gfxCmdBufSourceState,
    0x7Au,
    dvision0b + 0.5,
    v18 + 0.5,
    COERCE_FLOAT(LODWORD(dvision0b) ^ _mask__NegFloat_) + 0.5,
    COERCE_FLOAT(LODWORD(v18) ^ _mask__NegFloat_) + 0.5);
  RB_Filter(rgp.poisonFXMaterial, viewInfo);
}

// local variable allocation has failed, the output may be wrong!
void  RB_GenericFilterFX(const GfxMatrix *a1@<ebp>, const GfxViewInfo *viewInfo)
{
  const char *v2; // eax
  const char *v3; // eax
  float v4; // [esp+3Ch] [ebp-228h] BYREF
  int v5; // [esp+40h] [ebp-224h] OVERLAPPED
  int v6; // [esp+44h] [ebp-220h]
  float colorBlack[4]; // [esp+48h] [ebp-21Ch]
  float Intensity; // [esp+58h] [ebp-20Ch]
  float v9; // [esp+5Ch] [ebp-208h]
  float v10; // [esp+60h] [ebp-204h]
  float *v11; // [esp+64h] [ebp-200h]
  float v12; // [esp+68h] [ebp-1FCh]
  float v13; // [esp+6Ch] [ebp-1F8h]
  float v14; // [esp+70h] [ebp-1F4h]
  float v15; // [esp+74h] [ebp-1F0h]
  float *v16; // [esp+78h] [ebp-1ECh]
  float v17; // [esp+7Ch] [ebp-1E8h]
  float v18; // [esp+80h] [ebp-1E4h]
  float v19; // [esp+84h] [ebp-1E0h]
  float v20; // [esp+88h] [ebp-1DCh]
  float *v21; // [esp+8Ch] [ebp-1D8h]
  float *v22; // [esp+90h] [ebp-1D4h]
  int v23; // [esp+94h] [ebp-1D0h]
  int v24; // [esp+98h] [ebp-1CCh]
  int v25; // [esp+9Ch] [ebp-1C8h]
  float v26; // [esp+A0h] [ebp-1C4h]
  float *v27; // [esp+A4h] [ebp-1C0h]
  int v28; // [esp+A8h] [ebp-1BCh]
  int v29; // [esp+ACh] [ebp-1B8h]
  int v30; // [esp+B0h] [ebp-1B4h]
  float v31; // [esp+B4h] [ebp-1B0h]
  float *v32; // [esp+B8h] [ebp-1ACh]
  int v33; // [esp+BCh] [ebp-1A8h]
  int v34; // [esp+C0h] [ebp-1A4h]
  int v35; // [esp+C4h] [ebp-1A0h]
  float v36; // [esp+C8h] [ebp-19Ch]
  float *v37; // [esp+CCh] [ebp-198h]
  int v38; // [esp+D0h] [ebp-194h]
  int v39; // [esp+D4h] [ebp-190h]
  int v40; // [esp+D8h] [ebp-18Ch]
  float v41; // [esp+DCh] [ebp-188h]
  float *v42; // [esp+E0h] [ebp-184h]
  int v43; // [esp+E4h] [ebp-180h]
  int v44; // [esp+E8h] [ebp-17Ch]
  int v45; // [esp+ECh] [ebp-178h]
  float v46; // [esp+F0h] [ebp-174h]
  float *v47; // [esp+F4h] [ebp-170h]
  int v48; // [esp+F8h] [ebp-16Ch]
  int v49; // [esp+FCh] [ebp-168h]
  int v50; // [esp+100h] [ebp-164h]
  float v51; // [esp+104h] [ebp-160h]
  float *v52; // [esp+108h] [ebp-15Ch]
  int v53; // [esp+10Ch] [ebp-158h]
  int v54; // [esp+110h] [ebp-154h]
  int v55; // [esp+114h] [ebp-150h]
  float v56; // [esp+118h] [ebp-14Ch]
  float *v57; // [esp+11Ch] [ebp-148h]
  int v58; // [esp+120h] [ebp-144h]
  int v59; // [esp+124h] [ebp-140h]
  int v60; // [esp+128h] [ebp-13Ch]
  float v61; // [esp+12Ch] [ebp-138h]
  float *v62; // [esp+130h] [ebp-134h]
  int v63; // [esp+134h] [ebp-130h]
  int v64; // [esp+138h] [ebp-12Ch]
  int v65; // [esp+13Ch] [ebp-128h]
  int v66; // [esp+140h] [ebp-124h]
  float *v67; // [esp+144h] [ebp-120h]
  int v68; // [esp+148h] [ebp-11Ch]
  int v69; // [esp+14Ch] [ebp-118h]
  int v70; // [esp+150h] [ebp-114h]
  int v71; // [esp+154h] [ebp-110h]
  float *v72; // [esp+158h] [ebp-10Ch]
  int v73; // [esp+15Ch] [ebp-108h]
  int v74; // [esp+160h] [ebp-104h]
  int v75; // [esp+164h] [ebp-100h]
  int v76; // [esp+168h] [ebp-FCh]
  float *v77; // [esp+16Ch] [ebp-F8h]
  int v78; // [esp+170h] [ebp-F4h]
  int v79; // [esp+174h] [ebp-F0h]
  int v80; // [esp+178h] [ebp-ECh]
  int v81; // [esp+17Ch] [ebp-E8h]
  float *v82; // [esp+180h] [ebp-E4h]
  float v83; // [esp+184h] [ebp-E0h]
  float v84; // [esp+188h] [ebp-DCh]
  float v85; // [esp+18Ch] [ebp-D8h]
  float *v86; // [esp+190h] [ebp-D4h]
  int v87; // [esp+194h] [ebp-D0h]
  int v88; // [esp+198h] [ebp-CCh]
  float v89; // [esp+19Ch] [ebp-C8h]
  float *v90; // [esp+1A0h] [ebp-C4h]
  int v91; // [esp+1A4h] [ebp-C0h]
  int v92; // [esp+1A8h] [ebp-BCh]
  float v93; // [esp+1ACh] [ebp-B8h]
  float *v94; // [esp+1B0h] [ebp-B4h]
  int v95; // [esp+1B4h] [ebp-B0h]
  int v96; // [esp+1B8h] [ebp-ACh]
  float v97; // [esp+1BCh] [ebp-A8h]
  float v98; // [esp+1C0h] [ebp-A4h]
  float v99; // [esp+1C4h] [ebp-A0h]
  float v100; // [esp+1C8h] [ebp-9Ch]
  float Param0to4[4]; // [esp+1CCh] [ebp-98h]
  int v102; // [esp+1DCh] [ebp-88h]
  IDirect3DSurface9 *Surface; // [esp+1E0h] [ebp-84h]
  HRESULT hr; // [esp+1E4h] [ebp-80h]
  int semaphore; // [esp+1E8h] [ebp-7Ch]
  IDirect3DSurface9 *imageSurface; // [esp+1ECh] [ebp-78h]
  unsigned __int8 v107; // [esp+1F2h] [ebp-72h]
  char v108; // [esp+1F3h] [ebp-71h]
  int v109; // [esp+1F4h] [ebp-70h] OVERLAPPED
  unsigned __int8 v110; // [esp+1FBh] [ebp-69h]
  int j; // [esp+1FCh] [ebp-68h] OVERLAPPED
  char v112; // [esp+202h] [ebp-62h]
  char v113; // [esp+203h] [ebp-61h]
  int i; // [esp+204h] [ebp-60h] OVERLAPPED
  _BYTE iPass[76]; // [esp+208h] [ebp-5Ch] OVERLAPPED BYREF
  GfxGenericFilter *p_genericFilter; // [esp+254h] [ebp-10h]
  const GfxMatrix *ProjMatrix; // [esp+258h] [ebp-Ch]
  const GfxMatrix *ViewMatrix; // [esp+25Ch] [ebp-8h]
  const GfxMatrix *retaddr; // [esp+264h] [ebp+0h]

  ProjMatrix = a1;
  ViewMatrix = retaddr;
  p_genericFilter = &viewInfo->genericFilter;
  *(unsigned int *)&iPass[72] = viewInfo;
  *(unsigned int *)&iPass[68] = &viewInfo->cullViewInfo.viewParms.projectionMatrix;
  MatrixMultiply44(
    viewInfo->cullViewInfo.viewParms.viewMatrix.m,
    viewInfo->cullViewInfo.viewParms.projectionMatrix.m,
    (float (*)[4])iPass);
  for ( i = 0; i < 3; ++i )
  {
    v113 = 0;
    v112 = 0;
    for ( j = 0; j < 16; ++j )
    {
      if ( p_genericFilter->passEnabled[i][j]
        && p_genericFilter->passMaterial[i][j]
        && (p_genericFilter->passQuads[i][j] <= 0
         || (float)(p_genericFilter->passParam[i][j][4] * p_genericFilter->passFlareOcclusion[i][j]) > 0.0)
        && r_superFlare_enable->current.enabled )
      {
        v110 = 0;
        v109 = p_genericFilter->passTarget[i][j];
        switch ( --v109 )
        {
          case 0:
            v110 = 12;
            goto LABEL_18;
          case 1:
            v110 = 13;
            goto LABEL_18;
          case 2:
            v110 = 3;
LABEL_18:
            v108 = 0;
            v107 = 0;
            imageSurface = (IDirect3DSurface9 *)p_genericFilter->passSampler0[i][j];
            imageSurface = (IDirect3DSurface9 *)((char *)imageSurface - 1);
            switch ( (unsigned int)imageSurface )
            {
              case 0u:
                v108 = 12;
                v107 = 12;
                break;
              case 1u:
                v108 = 13;
                v107 = 13;
                break;
              case 2u:
                v108 = 3;
                v107 = 6;
                break;
              case 3u:
                v108 = 0;
                v107 = 5;
                break;
              default:
                break;
            }
            HIWORD(semaphore) = 0;
            hr = p_genericFilter->passSampler1[i][j];
            switch ( --hr )
            {
              case 0:
                HIWORD(semaphore) = 3084;
                break;
              case 1:
                HIWORD(semaphore) = 3341;
                break;
              case 2:
                HIWORD(semaphore) = 774;
                break;
              case 3:
                HIWORD(semaphore) = 5;
                break;
              default:
                break;
            }
            if ( v108 == 12 || HIBYTE(semaphore) == 12 )
            {
              R_SetRenderTargetSize(&gfxCmdBufSourceState, 0xCu);
              R_SetRenderTarget(gfxCmdBufContext, 0xCu);
              R_Resolve(gfxCmdBufContext, stru_B50E920.image);
            }
            if ( v108 == 13 || HIBYTE(semaphore) == 13 )
            {
              R_SetRenderTargetSize(&gfxCmdBufSourceState, 0xDu);
              R_SetRenderTarget(gfxCmdBufContext, 0xDu);
              R_Resolve(gfxCmdBufContext, stru_B50E934.image);
            }
            if ( v108 == 3 || HIBYTE(semaphore) == 3 )
            {
              R_SetRenderTargetSize(&gfxCmdBufSourceState, 3u);
              R_SetRenderTarget(gfxCmdBufContext, 3u);
              Surface = Image_GetSurface(stru_B50E8A8.image);
              if ( !Surface
                && !Assert_MyHandler(
                      "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_postfx.cpp",
                      3174,
                      0,
                      "%s",
                      "imageSurface") )
              {
                __debugbreak();
              }
              R_AssertDXDeviceOwnership();
              if ( r_logFile && r_logFile->current.integer )
                RB_LogPrint(
                  "gfxCmdBufContext.state->prim.device->StretchRect( gfxRenderTargets[gfxCmdBufContext.state->renderTarge"
                  "tId].surface.color, 0, imageSurface, 0, D3DTEXF_LINEAR )\n");
              v102 = R_AcquireDXDeviceOwnership(0);
              LODWORD(Param0to4[3]) = gfxCmdBufContext.state->prim.device->StretchRect(
                                        gfxCmdBufContext.state->prim.device,
                                        (IDirect3DSurface9 *)dword_B50E834[5 * gfxCmdBufContext.state->renderTargetId],
                                        0,
                                        Surface,
                                        0,
                                        D3DTEXF_LINEAR);
              if ( v102 )
                R_ReleaseDXDeviceOwnership();
              if ( Param0to4[3] < 0.0 )
              {
                ++g_disableRendering;
                v2 = R_ErrorDescription(SLODWORD(Param0to4[3]));
                Com_Error(
                  ERR_FATAL,
                  "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_postfx.cpp (%i) gfxCmdBufContext.state->prim.device->St"
                  "retchRect( gfxRenderTargets[gfxCmdBufContext.state->renderTargetId].surface.color, 0, imageSurface, 0,"
                  " D3DTEXF_LINEAR ) failed: %s\n",
                  3175,
                  v2);
              }
              R_AssertDXDeviceOwnership();
              if ( r_logFile && r_logFile->current.integer )
                RB_LogPrint("imageSurface->Release()\n");
              LODWORD(Param0to4[2]) = R_AcquireDXDeviceOwnership(0);
              LODWORD(Param0to4[1]) = Surface->Release(Surface);
              if ( LODWORD(Param0to4[2]) )
                R_ReleaseDXDeviceOwnership();
              if ( Param0to4[1] < 0.0 )
              {
                ++g_disableRendering;
                v3 = R_ErrorDescription(SLODWORD(Param0to4[1]));
                Com_Error(
                  ERR_FATAL,
                  "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_postfx.cpp (%i) imageSurface->Release() failed: %s\n",
                  3176,
                  v3);
              }
            }
            if ( v107 )
              R_SetCodeImageTexture(&gfxCmdBufSourceState, 0xDu, gfxRenderTargets[v107].image);
            if ( BYTE2(semaphore) )
              R_SetCodeImageTexture(&gfxCmdBufSourceState, 0xEu, gfxRenderTargets[BYTE2(semaphore)].image);
            if ( v110 == 3 )
            {
              R_SetRenderTargetSize(&gfxCmdBufSourceState, 3u);
              R_SetRenderTarget(gfxCmdBufContext, 3u);
            }
            else
            {
              R_SetRenderTargetSize(&gfxCmdBufSourceState, v110);
              R_SetRenderTarget(gfxCmdBufContext, v110);
            }
            if ( v110 )
            {
              v98 = p_genericFilter->passParam[i][j][0];
              v99 = p_genericFilter->passParam[i][j][1];
              v100 = p_genericFilter->passParam[i][j][2];
              Param0to4[0] = p_genericFilter->passParam[i][j][3];
              if ( p_genericFilter->passQuads[i][j] > 0
                && p_genericFilter->passParam[i][j][0] == 0.0
                && p_genericFilter->passParam[i][j][1] == 0.0
                && p_genericFilter->passParam[i][j][2] == 0.0 )
              {
                v98 = p_genericFilter->sunPosition[0];
                v99 = p_genericFilter->sunPosition[1];
                v100 = p_genericFilter->sunPosition[2];
              }
              v97 = **(float **)&iPass[72];
              v96 = *(unsigned int *)(*(unsigned int *)&iPass[72] + 16);
              v95 = *(unsigned int *)(*(unsigned int *)&iPass[72] + 32);
              v94 = gfxCmdBufSourceState.input.consts[182];
              gfxCmdBufSourceState.input.consts[182][0] = v97;
              *((unsigned int *)v94 + 1) = v96;
              *((unsigned int *)v94 + 2) = v95;
              v94[3] = *(float *)&FLOAT_0_0;
              R_DirtyCodeConstant(&gfxCmdBufSourceState, 0xB6u);
              v93 = *(float *)(*(unsigned int *)&iPass[72] + 4);
              v92 = *(unsigned int *)(*(unsigned int *)&iPass[72] + 20);
              v91 = *(unsigned int *)(*(unsigned int *)&iPass[72] + 36);
              v90 = gfxCmdBufSourceState.input.consts[181];
              gfxCmdBufSourceState.input.consts[181][0] = v93;
              *((unsigned int *)v90 + 1) = v92;
              *((unsigned int *)v90 + 2) = v91;
              v90[3] = *(float *)&FLOAT_0_0;
              R_DirtyCodeConstant(&gfxCmdBufSourceState, 0xB5u);
              v89 = *(float *)(*(unsigned int *)&iPass[72] + 8);
              v88 = *(unsigned int *)(*(unsigned int *)&iPass[72] + 24);
              v87 = *(unsigned int *)(*(unsigned int *)&iPass[72] + 40);
              v86 = gfxCmdBufSourceState.input.consts[180];
              gfxCmdBufSourceState.input.consts[180][0] = v89;
              *((unsigned int *)v86 + 1) = v88;
              *((unsigned int *)v86 + 2) = v87;
              v86[3] = *(float *)&FLOAT_0_0;
              R_DirtyCodeConstant(&gfxCmdBufSourceState, 0xB4u);
              v85 = viewInfo->cullViewInfo.viewParms.origin[0];
              v84 = viewInfo->cullViewInfo.viewParms.origin[1];
              v83 = viewInfo->cullViewInfo.viewParms.origin[2];
              v82 = gfxCmdBufSourceState.input.consts[191];
              gfxCmdBufSourceState.input.consts[191][0] = v85;
              v82[1] = v84;
              v82[2] = v83;
              v82[3] = *(float *)&FLOAT_0_0;
              R_DirtyCodeConstant(&gfxCmdBufSourceState, 0xBFu);
              v81 = *(unsigned int *)iPass;
              v80 = *(unsigned int *)&iPass[4];
              v79 = *(unsigned int *)&iPass[8];
              v78 = *(unsigned int *)&iPass[12];
              v77 = gfxCmdBufSourceState.input.consts[151];
              gfxCmdBufSourceState.input.consts[151][0] = *(float *)iPass;
              *((unsigned int *)v77 + 1) = v80;
              *((unsigned int *)v77 + 2) = v79;
              *((unsigned int *)v77 + 3) = v78;
              R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x97u);
              v75 = *(unsigned int *)&iPass[20];
              v76 = *(unsigned int *)&iPass[16];
              v74 = *(unsigned int *)&iPass[24];
              v73 = *(unsigned int *)&iPass[28];
              v72 = gfxCmdBufSourceState.input.consts[152];
              gfxCmdBufSourceState.input.consts[152][0] = *(float *)&iPass[16];
              *((unsigned int *)v72 + 1) = v75;
              *((unsigned int *)v72 + 2) = v74;
              *((unsigned int *)v72 + 3) = v73;
              R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x98u);
              v70 = *(unsigned int *)&iPass[36];
              v71 = *(unsigned int *)&iPass[32];
              v69 = *(unsigned int *)&iPass[40];
              v68 = *(unsigned int *)&iPass[44];
              v67 = gfxCmdBufSourceState.input.consts[153];
              gfxCmdBufSourceState.input.consts[153][0] = *(float *)&iPass[32];
              *((unsigned int *)v67 + 1) = v70;
              *((unsigned int *)v67 + 2) = v69;
              *((unsigned int *)v67 + 3) = v68;
              R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x99u);
              v65 = *(unsigned int *)&iPass[52];
              v66 = *(unsigned int *)&iPass[48];
              v64 = *(unsigned int *)&iPass[56];
              v63 = *(unsigned int *)&iPass[60];
              v62 = gfxCmdBufSourceState.input.consts[154];
              gfxCmdBufSourceState.input.consts[154][0] = *(float *)&iPass[48];
              *((unsigned int *)v62 + 1) = v65;
              *((unsigned int *)v62 + 2) = v64;
              *((unsigned int *)v62 + 3) = v63;
              R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x9Au);
              v61 = **(float **)&iPass[72];
              v60 = *(unsigned int *)(*(unsigned int *)&iPass[72] + 4);
              v59 = *(unsigned int *)(*(unsigned int *)&iPass[72] + 8);
              v58 = *(unsigned int *)(*(unsigned int *)&iPass[72] + 12);
              v57 = gfxCmdBufSourceState.input.consts[157];
              gfxCmdBufSourceState.input.consts[157][0] = v61;
              *((unsigned int *)v57 + 1) = v60;
              *((unsigned int *)v57 + 2) = v59;
              *((unsigned int *)v57 + 3) = v58;
              R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x9Du);
              v56 = *(float *)(*(unsigned int *)&iPass[72] + 16);
              v55 = *(unsigned int *)(*(unsigned int *)&iPass[72] + 20);
              v54 = *(unsigned int *)(*(unsigned int *)&iPass[72] + 24);
              v53 = *(unsigned int *)(*(unsigned int *)&iPass[72] + 28);
              v52 = gfxCmdBufSourceState.input.consts[158];
              gfxCmdBufSourceState.input.consts[158][0] = v56;
              *((unsigned int *)v52 + 1) = v55;
              *((unsigned int *)v52 + 2) = v54;
              *((unsigned int *)v52 + 3) = v53;
              R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x9Eu);
              v51 = *(float *)(*(unsigned int *)&iPass[72] + 32);
              v50 = *(unsigned int *)(*(unsigned int *)&iPass[72] + 36);
              v49 = *(unsigned int *)(*(unsigned int *)&iPass[72] + 40);
              v48 = *(unsigned int *)(*(unsigned int *)&iPass[72] + 44);
              v47 = gfxCmdBufSourceState.input.consts[159];
              gfxCmdBufSourceState.input.consts[159][0] = v51;
              *((unsigned int *)v47 + 1) = v50;
              *((unsigned int *)v47 + 2) = v49;
              *((unsigned int *)v47 + 3) = v48;
              R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x9Fu);
              v46 = *(float *)(*(unsigned int *)&iPass[72] + 48);
              v45 = *(unsigned int *)(*(unsigned int *)&iPass[72] + 52);
              v44 = *(unsigned int *)(*(unsigned int *)&iPass[72] + 56);
              v43 = *(unsigned int *)(*(unsigned int *)&iPass[72] + 60);
              v42 = gfxCmdBufSourceState.input.consts[160];
              gfxCmdBufSourceState.input.consts[160][0] = v46;
              *((unsigned int *)v42 + 1) = v45;
              *((unsigned int *)v42 + 2) = v44;
              *((unsigned int *)v42 + 3) = v43;
              R_DirtyCodeConstant(&gfxCmdBufSourceState, 0xA0u);
              v41 = **(float **)&iPass[68];
              v40 = *(unsigned int *)(*(unsigned int *)&iPass[68] + 4);
              v39 = *(unsigned int *)(*(unsigned int *)&iPass[68] + 8);
              v38 = *(unsigned int *)(*(unsigned int *)&iPass[68] + 12);
              v37 = gfxCmdBufSourceState.input.consts[146];
              gfxCmdBufSourceState.input.consts[146][0] = v41;
              *((unsigned int *)v37 + 1) = v40;
              *((unsigned int *)v37 + 2) = v39;
              *((unsigned int *)v37 + 3) = v38;
              R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x92u);
              v36 = *(float *)(*(unsigned int *)&iPass[68] + 16);
              v35 = *(unsigned int *)(*(unsigned int *)&iPass[68] + 20);
              v34 = *(unsigned int *)(*(unsigned int *)&iPass[68] + 24);
              v33 = *(unsigned int *)(*(unsigned int *)&iPass[68] + 28);
              v32 = gfxCmdBufSourceState.input.consts[147];
              gfxCmdBufSourceState.input.consts[147][0] = v36;
              *((unsigned int *)v32 + 1) = v35;
              *((unsigned int *)v32 + 2) = v34;
              *((unsigned int *)v32 + 3) = v33;
              R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x93u);
              v31 = *(float *)(*(unsigned int *)&iPass[68] + 32);
              v30 = *(unsigned int *)(*(unsigned int *)&iPass[68] + 36);
              v29 = *(unsigned int *)(*(unsigned int *)&iPass[68] + 40);
              v28 = *(unsigned int *)(*(unsigned int *)&iPass[68] + 44);
              v27 = gfxCmdBufSourceState.input.consts[148];
              gfxCmdBufSourceState.input.consts[148][0] = v31;
              *((unsigned int *)v27 + 1) = v30;
              *((unsigned int *)v27 + 2) = v29;
              *((unsigned int *)v27 + 3) = v28;
              R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x94u);
              v26 = *(float *)(*(unsigned int *)&iPass[68] + 48);
              v25 = *(unsigned int *)(*(unsigned int *)&iPass[68] + 52);
              v24 = *(unsigned int *)(*(unsigned int *)&iPass[68] + 56);
              v23 = *(unsigned int *)(*(unsigned int *)&iPass[68] + 60);
              v22 = gfxCmdBufSourceState.input.consts[149];
              gfxCmdBufSourceState.input.consts[149][0] = v26;
              *((unsigned int *)v22 + 1) = v25;
              *((unsigned int *)v22 + 2) = v24;
              *((unsigned int *)v22 + 3) = v23;
              R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x95u);
              v21 = gfxCmdBufSourceState.input.consts[183];
              gfxCmdBufSourceState.input.consts[183][0] = v98;
              v21[1] = v99;
              v21[2] = v100;
              v21[3] = Param0to4[0];
              R_DirtyCodeConstant(&gfxCmdBufSourceState, 0xB7u);
              v20 = p_genericFilter->passParam[i][j][4];
              v19 = p_genericFilter->passParam[i][j][5];
              v18 = p_genericFilter->passParam[i][j][6];
              v17 = p_genericFilter->passParam[i][j][7];
              v16 = gfxCmdBufSourceState.input.consts[184];
              gfxCmdBufSourceState.input.consts[184][0] = v20;
              v16[1] = v19;
              v16[2] = v18;
              v16[3] = v17;
              R_DirtyCodeConstant(&gfxCmdBufSourceState, 0xB8u);
              v15 = p_genericFilter->passParam[i][j][8];
              v14 = p_genericFilter->passParam[i][j][9];
              v13 = p_genericFilter->passParam[i][j][10];
              v12 = p_genericFilter->passParam[i][j][11];
              v11 = gfxCmdBufSourceState.input.consts[185];
              gfxCmdBufSourceState.input.consts[185][0] = v15;
              v11[1] = v14;
              v11[2] = v13;
              v11[3] = v12;
              R_DirtyCodeConstant(&gfxCmdBufSourceState, 0xB9u);
              v10 = p_genericFilter->passParam[i][j][12];
              v9 = p_genericFilter->passParam[i][j][13];
              Intensity = (float)(unsigned __int16)word_B50E83C[10 * v110];
              colorBlack[3] = (float)(unsigned __int16)word_B50E83E[10 * v110];
              LODWORD(colorBlack[2]) = gfxCmdBufSourceState.input.consts[186];
              gfxCmdBufSourceState.input.consts[186][0] = v10;
              *(float *)(LODWORD(colorBlack[2]) + 4) = v9;
              *(float *)(LODWORD(colorBlack[2]) + 8) = Intensity;
              *(float *)(LODWORD(colorBlack[2]) + 12) = colorBlack[3];
              R_DirtyCodeConstant(&gfxCmdBufSourceState, 0xBAu);
              R_Set2D(&gfxCmdBufSourceState);
              if ( p_genericFilter->passQuads[i][j] <= 0 )
              {
                RB_Filter(p_genericFilter->passMaterial[i][j], viewInfo);
              }
              else
              {
                colorBlack[1] = p_genericFilter->passParam[i][j][4] * p_genericFilter->passFlareOcclusion[i][j];
                if ( !v112 )
                {
                  v4 = *(float *)&FLOAT_0_0;
                  v5 = *(unsigned int *)&FLOAT_0_0;
                  v6 = *(unsigned int *)&FLOAT_0_0;
                  colorBlack[0] = *(float *)&FLOAT_0_0;
                  R_ClearScreen(gfxCmdBufContext.state->prim.device, 1u, &v4, 0.0, 0, 0);
                  v112 = 1;
                }
                gfxCmdBufSourceState.input.consts[180][0] = colorBlack[1];
                gfxCmdBufSourceState.input.consts[180][1] = colorBlack[1];
                gfxCmdBufSourceState.input.consts[180][2] = colorBlack[1];
                gfxCmdBufSourceState.input.consts[180][3] = colorBlack[1];
                R_DirtyCodeConstant(&gfxCmdBufSourceState, 0xB4u);
                RB_DrawSW4Quads(p_genericFilter->passMaterial[i][j], p_genericFilter->passQuads[i][j]);
                v113 = 1;
              }
            }
            break;
          case 3:
            v110 = 5;
            break;
          default:
            continue;
        }
      }
    }
    if ( v113 )
    {
      RB_SetBlurConstants(viewInfo->bloom.bloomBlurRadius, 1.0 / (float)stru_B50E934.width);
      R_SetRenderTargetSize(&gfxCmdBufSourceState, 0xDu);
      R_SetRenderTarget(gfxCmdBufContext, 0xDu);
      R_ClearRenderTargetForMultiGpu(gfxCmdBufContext, 0xDu);
      R_SetCodeImageTexture(&gfxCmdBufSourceState, 0x22u, stru_B50E920.image);
      RB_Filter(rgp.bloomBlurX, viewInfo);
      RB_SetBlurConstants(viewInfo->bloom.bloomBlurRadius, 1.0 / (float)stru_B50E920.height);
      R_SetRenderTargetSize(&gfxCmdBufSourceState, 0xCu);
      R_SetRenderTarget(gfxCmdBufContext, 0xCu);
      R_ClearRenderTargetForMultiGpu(gfxCmdBufContext, 0xCu);
      R_SetCodeImageTexture(&gfxCmdBufSourceState, 0x22u, stru_B50E934.image);
      RB_Filter(rgp.bloomBlurY, viewInfo);
      gfxCmdBufSourceState.input.codeImageRenderTargetControl[13].packed = ((unsigned int)&loc_800000 + 201326592)
                                                                         | 0x62;
      R_SetRenderTargetSize(&gfxCmdBufSourceState, viewInfo->sceneComposition.mainScene);
      R_SetRenderTarget(gfxCmdBufContext, viewInfo->sceneComposition.mainScene);
      R_Set2D(&gfxCmdBufSourceState);
      RB_DrawFullSceneQuad(
        rgp.superFlareApply,
        viewInfo->sceneComposition.mainScene,
        0.0,
        0.0,
        1.0,
        1.0,
        GFX_PRIM_STATS_CODE);
      RB_EndTessSurface();
    }
  }
}

void __cdecl RB_FlameFX(const GfxViewInfo *viewInfo)
{
  float x; // [esp+10h] [ebp-34h]
  float v2; // [esp+2Ch] [ebp-18h]
  float fadeEffect; // [esp+3Ch] [ebp-8h]
  const Material *flameFxMaterial; // [esp+40h] [ebp-4h]

  R_SetCodeImageTexture(&gfxCmdBufSourceState, 8u, stru_B50E8A8.image);
  R_SetRenderTargetSize(&gfxCmdBufSourceState, 3u);
  R_SetRenderTarget(gfxCmdBufContext, 3u);
  flameFxMaterial = rgp.permapAssets.flameThrowerFXMaterial;
  R_UpdateCodeConstant(
    &gfxCmdBufSourceState,
    0x58u,
    (float)rgp.permapAssets.flameThrowerFXMaterial->info.textureAtlasColumnCount,
    (float)rgp.permapAssets.flameThrowerFXMaterial->info.textureAtlasRowCount,
    (float)(rgp.permapAssets.flameThrowerFXMaterial->info.textureAtlasColumnCount
          * rgp.permapAssets.flameThrowerFXMaterial->info.textureAtlasRowCount),
    viewInfo->flameFx.frameRate);
  if ( viewInfo->flameFx.duration )
  {
    fadeEffect = 1.0
               - (float)((float)(viewInfo->flameFx.duration
                               - (viewInfo->flameFx.currentTime
                                - viewInfo->flameFx.startMSec))
                       / (float)viewInfo->flameFx.duration);
    if ( (float)(fadeEffect - 1.0) < 0.0 )
      v2 = 1.0
         - (float)((float)(viewInfo->flameFx.duration - (viewInfo->flameFx.currentTime - viewInfo->flameFx.startMSec))
                 / (float)viewInfo->flameFx.duration);
    else
      v2 = FLOAT_1_0;
    if ( (float)(0.0 - fadeEffect) < 0.0 )
      x = v2;
    else
      x = *(float *)&FLOAT_0_0;
    R_UpdateCodeConstant(&gfxCmdBufSourceState, 0x5Cu, x, 0.0, 0.0, 0.0);
  }
  else
  {
    R_UpdateCodeConstant(&gfxCmdBufSourceState, 0x5Cu, 0.0, 0.0, 0.0, 0.0);
  }
  if ( viewInfo->flameFx.distortionScale[1] <= 0.0 )
    R_UpdateCodeConstant(
      &gfxCmdBufSourceState,
      0x56u,
      viewInfo->flameFx.distortionScale[0],
      viewInfo->flameFx.distortionScale[1],
      0.0,
      viewInfo->flameFx.distortionMagnitude);
  else
    R_UpdateCodeConstant(
      &gfxCmdBufSourceState,
      0x56u,
      viewInfo->flameFx.distortionScale[0],
      viewInfo->flameFx.distortionScale[1],
      1.0 / viewInfo->flameFx.distortionScale[1],
      viewInfo->flameFx.distortionMagnitude);
  RB_Filter(flameFxMaterial, viewInfo);
}

void __cdecl RB_ElectrifiedFX(const GfxViewInfo *viewInfo)
{
  float x; // [esp+10h] [ebp-34h]
  float v2; // [esp+2Ch] [ebp-18h]
  const Material *electrifiedMtl; // [esp+3Ch] [ebp-8h]
  float fadeEffect; // [esp+40h] [ebp-4h]

  R_SetCodeImageTexture(&gfxCmdBufSourceState, 8u, stru_B50E8A8.image);
  R_SetRenderTargetSize(&gfxCmdBufSourceState, 3u);
  R_SetRenderTarget(gfxCmdBufContext, 3u);
  electrifiedMtl = rgp.permapAssets.electrifiedFXMaterial;
  R_UpdateCodeConstant(
    &gfxCmdBufSourceState,
    0x58u,
    (float)rgp.permapAssets.electrifiedFXMaterial->info.textureAtlasColumnCount,
    (float)rgp.permapAssets.electrifiedFXMaterial->info.textureAtlasRowCount,
    (float)(rgp.permapAssets.electrifiedFXMaterial->info.textureAtlasColumnCount
          * rgp.permapAssets.electrifiedFXMaterial->info.textureAtlasRowCount),
    viewInfo->electrifiedFx.frameRate);
  if ( viewInfo->electrifiedFx.duration )
  {
    fadeEffect = 1.0
               - (float)((float)(viewInfo->electrifiedFx.duration
                               - (viewInfo->electrifiedFx.currentTime
                                - viewInfo->electrifiedFx.startMSec))
                       / (float)viewInfo->electrifiedFx.duration);
    if ( (float)(fadeEffect - 1.0) < 0.0 )
      v2 = 1.0
         - (float)((float)(viewInfo->electrifiedFx.duration
                         - (viewInfo->electrifiedFx.currentTime
                          - viewInfo->electrifiedFx.startMSec))
                 / (float)viewInfo->electrifiedFx.duration);
    else
      v2 = FLOAT_1_0;
    if ( (float)(0.0 - fadeEffect) < 0.0 )
      x = v2;
    else
      x = *(float *)&FLOAT_0_0;
    R_UpdateCodeConstant(&gfxCmdBufSourceState, 0x5Cu, x, 0.0, 0.0, 0.0);
  }
  else
  {
    R_UpdateCodeConstant(&gfxCmdBufSourceState, 0x5Cu, 0.0, 0.0, 0.0, 0.0);
  }
  if ( viewInfo->electrifiedFx.distortionScale[1] <= 0.0 )
    R_UpdateCodeConstant(
      &gfxCmdBufSourceState,
      0x56u,
      viewInfo->electrifiedFx.distortionScale[0],
      viewInfo->electrifiedFx.distortionScale[1],
      0.0,
      viewInfo->electrifiedFx.distortionMagnitude);
  else
    R_UpdateCodeConstant(
      &gfxCmdBufSourceState,
      0x56u,
      viewInfo->electrifiedFx.distortionScale[0],
      viewInfo->electrifiedFx.distortionScale[1],
      1.0 / viewInfo->electrifiedFx.distortionScale[1],
      viewInfo->electrifiedFx.distortionMagnitude);
  RB_Filter(electrifiedMtl, viewInfo);
}

void __cdecl RB_TransportedFX(const GfxViewInfo *viewInfo)
{
  float x; // [esp+10h] [ebp-34h]
  float v2; // [esp+2Ch] [ebp-18h]
  const Material *transportedMtl; // [esp+3Ch] [ebp-8h]
  float fadeEffect; // [esp+40h] [ebp-4h]

  R_SetCodeImageTexture(&gfxCmdBufSourceState, 8u, stru_B50E8A8.image);
  R_SetRenderTargetSize(&gfxCmdBufSourceState, 3u);
  R_SetRenderTarget(gfxCmdBufContext, 3u);
  transportedMtl = rgp.permapAssets.transportedFXMaterial;
  R_UpdateCodeConstant(
    &gfxCmdBufSourceState,
    0x58u,
    (float)rgp.permapAssets.transportedFXMaterial->info.textureAtlasColumnCount,
    (float)rgp.permapAssets.transportedFXMaterial->info.textureAtlasRowCount,
    (float)(rgp.permapAssets.transportedFXMaterial->info.textureAtlasColumnCount
          * rgp.permapAssets.transportedFXMaterial->info.textureAtlasRowCount),
    viewInfo->transportedFx.frameRate);
  if ( viewInfo->transportedFx.duration )
  {
    fadeEffect = 1.0
               - (float)((float)(viewInfo->transportedFx.duration
                               - (viewInfo->transportedFx.currentTime
                                - viewInfo->transportedFx.startMSec))
                       / (float)viewInfo->transportedFx.duration);
    if ( (float)(fadeEffect - 1.0) < 0.0 )
      v2 = 1.0
         - (float)((float)(viewInfo->transportedFx.duration
                         - (viewInfo->transportedFx.currentTime
                          - viewInfo->transportedFx.startMSec))
                 / (float)viewInfo->transportedFx.duration);
    else
      v2 = FLOAT_1_0;
    if ( (float)(0.0 - fadeEffect) < 0.0 )
      x = v2;
    else
      x = *(float *)&FLOAT_0_0;
    R_UpdateCodeConstant(&gfxCmdBufSourceState, 0x5Cu, x, 0.0, 0.0, 0.0);
  }
  else
  {
    R_UpdateCodeConstant(&gfxCmdBufSourceState, 0x5Cu, 0.0, 0.0, 0.0, 0.0);
  }
  if ( viewInfo->transportedFx.distortionScale[1] <= 0.0 )
    R_UpdateCodeConstant(
      &gfxCmdBufSourceState,
      0x56u,
      viewInfo->transportedFx.distortionScale[0],
      viewInfo->transportedFx.distortionScale[1],
      0.0,
      viewInfo->transportedFx.distortionMagnitude);
  else
    R_UpdateCodeConstant(
      &gfxCmdBufSourceState,
      0x56u,
      viewInfo->transportedFx.distortionScale[0],
      viewInfo->transportedFx.distortionScale[1],
      1.0 / viewInfo->transportedFx.distortionScale[1],
      viewInfo->transportedFx.distortionMagnitude);
  RB_Filter(transportedMtl, viewInfo);
}

void __cdecl RB_WaterSheetingFX(const GfxViewInfo *viewInfo)
{
  float distortionMagnitude; // [esp+8h] [ebp-4Ch]
  float v2; // [esp+14h] [ebp-40h]
  float v3; // [esp+18h] [ebp-3Ch]
  float v4; // [esp+24h] [ebp-30h]
  float v5; // [esp+28h] [ebp-2Ch]
  float gameTimeVec[4]; // [esp+38h] [ebp-1Ch] BYREF
  float y_speed; // [esp+48h] [ebp-Ch]
  float fadeEffect; // [esp+4Ch] [ebp-8h]
  float x_speed; // [esp+50h] [ebp-4h]

  R_SetCodeImageTexture(&gfxCmdBufSourceState, 8u, stru_B50E8A8.image);
  R_SetRenderTargetSize(&gfxCmdBufSourceState, 3u);
  R_SetRenderTarget(gfxCmdBufContext, 3u);
  R_CalcGameTimeVec(gfxCmdBufSourceState.materialTime, gameTimeVec);
  x_speed = viewInfo->waterSheetingFx.distortionScale[0];
  if ( viewInfo->waterSheetingFx.distortionScale[1] <= 0.0 )
    y_speed = *(float *)&FLOAT_0_0;
  else
    y_speed = 1.0 / viewInfo->waterSheetingFx.distortionScale[1];
  if ( viewInfo->waterSheetingFx.duration )
    fadeEffect = 1.0
               - (float)((float)(viewInfo->waterSheetingFx.duration
                               - (viewInfo->waterSheetingFx.currentTime
                                - viewInfo->waterSheetingFx.startMSec))
                       / (float)viewInfo->waterSheetingFx.duration);
  else
    fadeEffect = *(float *)&FLOAT_0_0;
  v4 = gameTimeVec[1] * 0.5;
  v5 = gameTimeVec[0] * 0.5;
  gfxCmdBufSourceState.input.consts[121][0] = FLOAT_1_0;
  gfxCmdBufSourceState.input.consts[121][1] = FLOAT_0_47499999;
  gfxCmdBufSourceState.input.consts[121][2] = v4;
  gfxCmdBufSourceState.input.consts[121][3] = v5;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x79u);
  v2 = 1.0 - (float)(gameTimeVec[3] * y_speed);
  v3 = (float)(gameTimeVec[0] * x_speed) * 0.47499999;
  gfxCmdBufSourceState.input.consts[122][0] = gameTimeVec[0] * x_speed;
  gfxCmdBufSourceState.input.consts[122][1] = v2;
  gfxCmdBufSourceState.input.consts[122][2] = v3;
  gfxCmdBufSourceState.input.consts[122][3] = v2 * 0.47499999;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Au);
  distortionMagnitude = viewInfo->waterSheetingFx.distortionMagnitude;
  gfxCmdBufSourceState.input.consts[123][0] = 1.0 - (float)(2.0 * fadeEffect);
  gfxCmdBufSourceState.input.consts[123][1] = distortionMagnitude;
  gfxCmdBufSourceState.input.consts[123][2] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[123][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Bu);
  RB_Filter(rgp.waterSheetingFXMaterial, viewInfo);
}

void __cdecl RB_ReviveFX(const GfxViewInfo *viewInfo)
{
  float reviveEdgeAmount; // [esp+1Ch] [ebp-108h]
  float v2; // [esp+28h] [ebp-FCh]
  float v3; // [esp+2Ch] [ebp-F8h]
  float v4; // [esp+30h] [ebp-F4h]
  float v5; // [esp+3Ch] [ebp-E8h]
  float v6; // [esp+40h] [ebp-E4h]
  float v7; // [esp+44h] [ebp-E0h]
  float v8; // [esp+50h] [ebp-D4h]
  float v9; // [esp+54h] [ebp-D0h]
  float v10; // [esp+58h] [ebp-CCh]
  float saturationMatrix[4][4]; // [esp+64h] [ebp-C0h] BYREF
  float finalMatrix[4][4]; // [esp+A4h] [ebp-80h] BYREF
  float whiteTempMatrix[4][4]; // [esp+E4h] [ebp-40h] BYREF

  //PIXBeginNamedEvent(-1, "RB_ReviveFX");
  RB_GaussianFilterImage(6.4000001, 6u, 0xCu);
  R_SetRenderTargetSize(&gfxCmdBufSourceState, 3u);
  R_SetRenderTarget(gfxCmdBufContext, 3u);
  R_SetCodeImageTexture(&gfxCmdBufSourceState, 8u, stru_B50E8A8.image);
  colorTempMatrix(whiteTempMatrix, viewInfo->reviveFx.reviveEdgeColorTemp);
  colorSaturationMatrix(saturationMatrix, viewInfo->reviveFx.reviveEdgeSaturation);
  MatrixMultiply44(whiteTempMatrix, saturationMatrix, finalMatrix);
  finalMatrix[0][0] = (float)(viewInfo->reviveFx.reviveEdgeScale[0] * viewInfo->reviveFx.reviveEdgeContrast[0])
                    * finalMatrix[0][0];
  finalMatrix[1][0] = (float)(viewInfo->reviveFx.reviveEdgeScale[0] * viewInfo->reviveFx.reviveEdgeContrast[0])
                    * finalMatrix[1][0];
  finalMatrix[2][0] = (float)(viewInfo->reviveFx.reviveEdgeScale[0] * viewInfo->reviveFx.reviveEdgeContrast[0])
                    * finalMatrix[2][0];
  finalMatrix[0][1] = (float)(viewInfo->reviveFx.reviveEdgeScale[1] * viewInfo->reviveFx.reviveEdgeContrast[1])
                    * finalMatrix[0][1];
  finalMatrix[1][1] = (float)(viewInfo->reviveFx.reviveEdgeScale[1] * viewInfo->reviveFx.reviveEdgeContrast[1])
                    * finalMatrix[1][1];
  finalMatrix[2][1] = (float)(viewInfo->reviveFx.reviveEdgeScale[1] * viewInfo->reviveFx.reviveEdgeContrast[1])
                    * finalMatrix[2][1];
  finalMatrix[0][2] = (float)(viewInfo->reviveFx.reviveEdgeScale[2] * viewInfo->reviveFx.reviveEdgeContrast[2])
                    * finalMatrix[0][2];
  finalMatrix[1][2] = (float)(viewInfo->reviveFx.reviveEdgeScale[2] * viewInfo->reviveFx.reviveEdgeContrast[2])
                    * finalMatrix[1][2];
  finalMatrix[2][2] = (float)(viewInfo->reviveFx.reviveEdgeScale[2] * viewInfo->reviveFx.reviveEdgeContrast[2])
                    * finalMatrix[2][2];
  v8 = finalMatrix[1][0];
  v9 = finalMatrix[2][0];
  v10 = viewInfo->reviveFx.reviveEdgeOffset[0] + (float)((float)(1.0 - viewInfo->reviveFx.reviveEdgeContrast[0]) * 0.5);
  gfxCmdBufSourceState.input.consts[121][0] = finalMatrix[0][0];
  gfxCmdBufSourceState.input.consts[121][1] = v8;
  gfxCmdBufSourceState.input.consts[121][2] = v9;
  gfxCmdBufSourceState.input.consts[121][3] = v10;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x79u);
  v5 = finalMatrix[1][1];
  v6 = finalMatrix[2][1];
  v7 = viewInfo->reviveFx.reviveEdgeOffset[1] + (float)((float)(1.0 - viewInfo->reviveFx.reviveEdgeContrast[1]) * 0.5);
  gfxCmdBufSourceState.input.consts[122][0] = finalMatrix[0][1];
  gfxCmdBufSourceState.input.consts[122][1] = v5;
  gfxCmdBufSourceState.input.consts[122][2] = v6;
  gfxCmdBufSourceState.input.consts[122][3] = v7;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Au);
  v2 = finalMatrix[1][2];
  v3 = finalMatrix[2][2];
  v4 = viewInfo->reviveFx.reviveEdgeOffset[2] + (float)((float)(1.0 - viewInfo->reviveFx.reviveEdgeContrast[2]) * 0.5);
  gfxCmdBufSourceState.input.consts[123][0] = finalMatrix[0][2];
  gfxCmdBufSourceState.input.consts[123][1] = v2;
  gfxCmdBufSourceState.input.consts[123][2] = v3;
  gfxCmdBufSourceState.input.consts[123][3] = v4;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Bu);
  reviveEdgeAmount = viewInfo->reviveFx.reviveEdgeAmount;
  gfxCmdBufSourceState.input.consts[124][0] = viewInfo->reviveFx.reviveEdgeMaskAdjust;
  gfxCmdBufSourceState.input.consts[124][1] = reviveEdgeAmount;
  gfxCmdBufSourceState.input.consts[124][2] = *(float *)&FLOAT_0_0;
  gfxCmdBufSourceState.input.consts[124][3] = *(float *)&FLOAT_0_0;
  R_DirtyCodeConstant(&gfxCmdBufSourceState, 0x7Cu);
  RB_Filter(rgp.reviveFXMaterial, viewInfo);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
}

void __cdecl RB_ProcessPostEffects(const GfxViewInfo *viewInfo)
{
  float blurRadius; // [esp+58h] [ebp-8h]
  int savedregs; // [esp+60h] [ebp+0h] BYREF

  if ( !viewInfo
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_postfx.cpp", 4305, 0, "%s", "viewInfo") )
  {
    __debugbreak();
  }
  RB_GetResolvedScene();
  RB_BloomLDR(viewInfo);
  if ( RB_UsingPostEffects(viewInfo) )
  {
    RB_GetResolvedScene();
    if ( RB_UsingFlameFX(viewInfo) )
    {
      //PIXBeginNamedEvent(-1, "RB_ApplyFlameFX");
      RB_FlameFX(viewInfo);
      if ( GetCurrentThreadId() == g_DXDeviceThread )
        goto LABEL_28;
    }
    else
    {
      if ( RB_UsingReviveFX(viewInfo) )
      {
        //PIXBeginNamedEvent(-1, "RB_ApplyReviveFX");
        RB_ReviveFX(viewInfo);
        if ( GetCurrentThreadId() == g_DXDeviceThread )
          D3DPERF_EndEvent();
        goto LABEL_29;
      }
      if ( RB_UsingElectrifiedFX(viewInfo) )
      {
        //PIXBeginNamedEvent(-1, "RB_ApplyElectrifiedFX");
        RB_ElectrifiedFX(viewInfo);
        if ( GetCurrentThreadId() == g_DXDeviceThread )
          goto LABEL_28;
      }
      else if ( RB_UsingTransportedFX(viewInfo) )
      {
        //PIXBeginNamedEvent(-1, "RB_ApplyTransportedFX");
        RB_TransportedFX(viewInfo);
        if ( GetCurrentThreadId() == g_DXDeviceThread )
          goto LABEL_28;
      }
      else if ( RB_UsingWaterSheetingFX(viewInfo) )
      {
        //PIXBeginNamedEvent(-1, "RB_WaterSheetingFX");
        RB_WaterSheetingFX(viewInfo);
        if ( GetCurrentThreadId() == g_DXDeviceThread )
          goto LABEL_28;
      }
      else
      {
        if ( !RB_UsingPoison(viewInfo) )
        {
          if ( !RB_UsingDepthOfFieldFX(viewInfo) )
            goto LABEL_29;
          //PIXBeginNamedEvent(-1, "RB_ApplyDepthOfField");
          RB_ApplyDepthOfField(COERCE_FLOAT(&savedregs), viewInfo);
          if ( GetCurrentThreadId() != g_DXDeviceThread )
            goto LABEL_29;
          goto LABEL_28;
        }
        //PIXBeginNamedEvent(-1, "RB_PoisonFX");
        RB_PoisonFX(viewInfo);
        if ( GetCurrentThreadId() == g_DXDeviceThread )
LABEL_28:
          D3DPERF_EndEvent();
      }
    }
LABEL_29:
    if ( RB_UsingGenericFilter(viewInfo) && !rg.renderHiResShot )
    {
      //PIXBeginNamedEvent(-1, "genericFilter effect");
      RB_GenericFilterFX((const GfxMatrix *)&savedregs, viewInfo);
      if ( GetCurrentThreadId() == g_DXDeviceThread )
        D3DPERF_EndEvent();
    }
    if ( RB_UsingBlur(viewInfo) )
    {
      blurRadius = RB_GetBlurRadius(viewInfo->blurRadius);
      if ( blurRadius <= 0.0
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_postfx.cpp",
              4388,
              1,
              "%s\n\t(blurRadius) = %g",
              "(blurRadius > 0.0f)",
              blurRadius) )
      {
        __debugbreak();
      }
      //PIXBeginNamedEvent(-1, "RB_BlurScreen()");
      RB_BlurScreen(viewInfo, blurRadius);
      if ( g_DXDeviceThread == GetCurrentThreadId() )
        D3DPERF_EndEvent();
    }
    if ( viewInfo->saveScreenFx.blendBlurredParam.enabled
      || viewInfo->saveScreenFx.blendFlashedParam.enabled
      || viewInfo->saveScreenFx.saveScreenParam.mode )
    {
      //PIXBeginNamedEvent(-1, "saveScreenFx");
      RB_SaveScreen_BlendBlurred(&viewInfo->saveScreenFx.blendBlurredParam, viewInfo);
      RB_SaveScreen_BlendFlashed(&viewInfo->saveScreenFx.blendFlashedParam, viewInfo);
      RB_SaveScreen(&viewInfo->saveScreenFx.saveScreenParam, viewInfo);
      if ( GetCurrentThreadId() == g_DXDeviceThread )
        D3DPERF_EndEvent();
    }
    R_Resolve(gfxCmdBufContext, stru_B50E8A8.image);
    R_SetCodeImageTexture(&gfxCmdBufSourceState, 0xAu, stru_B50E8A8.image);
  }
}

bool __cdecl RB_UsingPoison(const GfxViewInfo *viewInfo)
{
  return viewInfo->poisonFx.curAmount > 0.0;
}

bool __cdecl RB_UsingPostEffects(const GfxViewInfo *viewInfo)
{
  if ( r_showFbColorDebug->current.integer == 2 )
    return 1;
  if ( RB_UsingBlur(viewInfo) )
    return 1;
  if ( RB_UsingReviveFX(viewInfo) )
    return 1;
  if ( RB_UsingFlameFX(viewInfo) )
    return 1;
  if ( RB_UsingElectrifiedFX(viewInfo) )
    return 1;
  if ( RB_UsingTransportedFX(viewInfo) )
    return 1;
  if ( RB_UsingWaterSheetingFX(viewInfo) )
    return 1;
  if ( R_UsingDoubleVision(viewInfo) )
    return 1;
  if ( RB_UsingGenericFilter(viewInfo) )
    return 1;
  if ( RB_UsingPoison(viewInfo) )
    return 1;
  if ( viewInfo->saveScreenFx.saveScreenParam.mode )
    return 1;
  if ( viewInfo->saveScreenFx.blendFlashedParam.enabled )
    return 1;
  if ( viewInfo->saveScreenFx.blendBlurredParam.enabled )
    return 1;
  return RB_UsingMergedPostEffects(viewInfo);
}

bool __cdecl RB_UsingMergedPostEffects(const GfxViewInfo *viewInfo)
{
  return RB_UsingDepthOfFieldFX(viewInfo) || RB_UsingColorManipulation(viewInfo);
}

void RB_GetResolvedScene()
{
  if ( gfxCmdBufState.renderTargetId != 3
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_postfx.cpp",
          2486,
          0,
          "gfxCmdBufState.renderTargetId == R_RENDERTARGET_SCENE\n\t%i, %i",
          gfxCmdBufState.renderTargetId,
          3) )
  {
    __debugbreak();
  }
  R_Resolve(gfxCmdBufContext, stru_B50E8A8.image);
  R_SetCodeImageTexture(&gfxCmdBufSourceState, 0xAu, stru_B50E8A8.image);
}

void  RB_ApplyDepthOfField(float a1@<ebp>, const GfxViewInfo *viewInfo)
{
  double v2; // xmm0_8
  const char *v3; // eax
  float farOutOfFocus; // [esp+10h] [ebp-48h]
  long double v5; // [esp+1Ch] [ebp-3Ch]
  long double v6; // [esp+24h] [ebp-34h]
  float v7; // [esp+24h] [ebp-34h]
  float DepthOfFieldBlurFraction; // [esp+28h] [ebp-30h]
  float v9[3]; // [esp+3Ch] [ebp-1Ch] BYREF
  float dofEquation[4]; // [esp+48h] [ebp-10h]
  float retaddr; // [esp+58h] [ebp+0h]

  dofEquation[1] = a1;
  dofEquation[2] = retaddr;
  if ( !RB_UsingDepthOfFieldFX(viewInfo)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_postfx.cpp",
          3910,
          0,
          "%s",
          "RB_UsingDepthOfFieldFX( viewInfo )") )
  {
    __debugbreak();
  }
  if ( viewInfo->isMissileCamera
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_postfx.cpp",
          3912,
          0,
          "%s",
          "!viewInfo->isMissileCamera") )
  {
    __debugbreak();
  }
  if ( !dx.supportsIntZ
    && !viewInfo->needsFloatZ
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_postfx.cpp",
          3913,
          0,
          "%s",
          "dx.supportsIntZ || viewInfo->needsFloatZ") )
  {
    __debugbreak();
  }
  RB_GetSceneDepthOfFieldEquation(
    viewInfo->dof.nearStart,
    viewInfo->dof.nearEnd,
    viewInfo->dof.farStart,
    viewInfo->dof.farEnd,
    v9,
    viewInfo->cullViewInfo.viewParms.zNear);
  R_UpdateCodeConstantFromVec4(&gfxCmdBufSourceState, 0x1Bu, v9);
  RB_GetViewModelDepthOfFieldEquation(viewInfo->dof.viewModelStart, viewInfo->dof.viewModelEnd, v9);
  v2 = (float)(viewInfo->dof.farBlur / viewInfo->dof.nearBlur);
  __libm_sse2_pow(v5, v6);
  *(float *)&v2 = v2;
  dofEquation[0] = *(float *)&v2;
  R_UpdateCodeConstantFromVec4(&gfxCmdBufSourceState, 0x1Au, v9);
  farOutOfFocus = 1.0 / (double)vidConfig.sceneHeight;
  R_UpdateCodeConstant(&gfxCmdBufSourceState, 0x1Eu, 0.0, farOutOfFocus, 0.0, 0.0);
  DepthOfFieldBlurFraction = RB_GetDepthOfFieldBlurFraction(viewInfo, 1.4);
  v7 = RB_GetDepthOfFieldBlurFraction(viewInfo, 3.5999999);
  if ( DepthOfFieldBlurFraction <= 0.0 || v7 <= DepthOfFieldBlurFraction || v7 >= 1.0 )
  {
    v3 = va("%g, %g, %g, %i", DepthOfFieldBlurFraction, v7, viewInfo->dof.nearBlur, vidConfig.sceneHeight);
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_postfx.cpp",
            3928,
            0,
            "%s\n\t%s",
            "0.0f < smallFrac && smallFrac < mediumFrac && mediumFrac < 1.0f",
            v3) )
      __debugbreak();
  }
  R_UpdateCodeConstant(
    &gfxCmdBufSourceState,
    0x1Cu,
    -1.0 / DepthOfFieldBlurFraction,
    -1.0 / (float)(v7 - DepthOfFieldBlurFraction),
    -1.0 / (float)(1.0 - v7),
    1.0 / (float)(1.0 - v7));
  R_UpdateCodeConstant(
    &gfxCmdBufSourceState,
    0x1Du,
    1.0,
    v7 / (float)(v7 - DepthOfFieldBlurFraction),
    1.0 / (float)(1.0 - v7),
    COERCE_FLOAT(LODWORD(v7) ^ _mask__NegFloat_) / (float)(1.0 - v7));
  RB_GetDepthOfFieldInputImages(viewInfo->dof.nearBlur);
  R_SetRenderTargetSize(&gfxCmdBufSourceState, 2u);
  R_SetRenderTarget(gfxCmdBufContext, 2u);
  if ( dx.supportsIntZ )
    RB_Filter(rgp.postFxDofNvIntzMaterial, viewInfo);
  else
    RB_Filter(rgp.postFxDofMaterial, viewInfo);
}

void __cdecl RB_GetSceneDepthOfFieldEquation(
        float nearOutOfFocus,
        float nearInFocus,
        float farInFocus,
        float farOutOfFocus,
        float *dofEquation,
        float zNear)
{
  float v6; // [esp+0h] [ebp-14h]
  float v7; // [esp+4h] [ebp-10h]
  float v8; // [esp+8h] [ebp-Ch]
  float v9; // [esp+Ch] [ebp-8h]
  float depthScale; // [esp+10h] [ebp-4h]

  if ( zNear == 0.0
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_postfx.cpp", 2396, 0, "%s", "zNear") )
  {
    __debugbreak();
  }
  if ( dx.supportsIntZ )
    depthScale = zNear * 0.984375;
  else
    depthScale = FLOAT_1_0;
  v8 = nearOutOfFocus;
  v9 = nearInFocus;
  if ( (float)(nearOutOfFocus - zNear) < 0.0 )
    v7 = zNear;
  else
    v7 = nearOutOfFocus;
  if ( v7 >= nearInFocus )
  {
    v9 = 0.5 * zNear;
    v8 = *(float *)&FLOAT_0_0;
  }
  *dofEquation = depthScale / (float)(v8 - v9);
  dofEquation[2] = v9 / (float)(v9 - v8);
  if ( (float)(zNear - farInFocus) < 0.0 )
    v6 = farInFocus;
  else
    v6 = zNear;
  if ( v6 < farOutOfFocus )
  {
    dofEquation[1] = depthScale / (float)(farOutOfFocus - farInFocus);
    dofEquation[3] = farInFocus / (float)(farInFocus - farOutOfFocus);
  }
  else
  {
    dofEquation[1] = *(float *)&FLOAT_0_0;
    dofEquation[3] = *(float *)&FLOAT_0_0;
  }
}

void __cdecl RB_GetViewModelDepthOfFieldEquation(float outOfFocus, float inFocus, float *dofEquation)
{
  float v3; // [esp+0h] [ebp-14h]
  float v4; // [esp+4h] [ebp-10h]
  float v5; // [esp+8h] [ebp-Ch]
  float nearClip; // [esp+Ch] [ebp-8h]
  float depthScale; // [esp+10h] [ebp-4h]

  nearClip = r_znear_depthhack->current.value;
  if ( dx.supportsIntZ )
  {
    dofEquation[1] = FLOAT_0_015625;
    depthScale = nearClip * 0.015625;
  }
  else
  {
    dofEquation[1] = *(float *)&FLOAT_0_0;
    depthScale = FLOAT_1_0;
  }
  v4 = outOfFocus;
  v5 = inFocus;
  if ( (float)(outOfFocus - nearClip) < 0.0 )
    v3 = nearClip;
  else
    v3 = outOfFocus;
  if ( v3 >= inFocus )
  {
    v5 = 0.5 * nearClip;
    v4 = *(float *)&FLOAT_0_0;
  }
  *dofEquation = depthScale / (float)(v4 - v5);
  dofEquation[2] = v5 / (float)(v5 - v4);
}

double __cdecl RB_GetDepthOfFieldBlurFraction(const GfxViewInfo *viewInfo, float pixelRadiusAtSceneRes)
{
  float normalizedRadius; // [esp+1Ch] [ebp-4h]

  if ( viewInfo->dof.nearBlur < 4.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_postfx.cpp",
          3709,
          0,
          "%s\n\t(viewInfo->dof.nearBlur) = %g",
          "(viewInfo->dof.nearBlur >= 4.0f)",
          viewInfo->dof.nearBlur) )
  {
    __debugbreak();
  }
  normalizedRadius = pixelRadiusAtSceneRes * 480.0 / (double)vidConfig.sceneHeight;
  return (float)pow((float)(normalizedRadius / viewInfo->dof.nearBlur), r_dof_bias->current.value);
}

void __cdecl RB_GetDepthOfFieldInputImages(float radius)
{
  R_SetRenderTargetSize(&gfxCmdBufSourceState, 0xDu);
  R_SetRenderTarget(gfxCmdBufContext, 0xDu);
  R_ClearRenderTargetForMultiGpu(gfxCmdBufContext, 0xDu);
  if ( dx.supportsIntZ )
    RB_FullScreenFilter(rgp.dofDownsampleNvIntzMaterial);
  else
    RB_FullScreenFilter(rgp.dofDownsampleMaterial);
  RB_GaussianFilterImage(radius * 0.25, 0xDu, 0xCu);
  R_SetRenderTargetSize(&gfxCmdBufSourceState, 8u);
  R_SetRenderTarget(gfxCmdBufContext, 8u);
  R_ClearRenderTargetForMultiGpu(gfxCmdBufContext, 8u);
  RB_FullScreenFilter(rgp.dofNearCocMaterial);
  R_SetRenderTargetSize(&gfxCmdBufSourceState, 0xDu);
  R_SetRenderTarget(gfxCmdBufContext, 0xDu);
  R_SetCodeImageTexture(&gfxCmdBufSourceState, 8u, stru_B50E8D0.image);
  RB_FullScreenFilter(rgp.smallBlurMaterial);
}

double __cdecl RB_GetBlurRadius(float blurRadiusFromCode)
{
  float blurRadiusFinal; // [esp+Ch] [ebp-8h]

  blurRadiusFinal = fsqrt(
                      (float)(r_blur->current.value * r_blur->current.value)
                    + (float)(blurRadiusFromCode * blurRadiusFromCode));
  if ( blurRadiusFinal < 0.0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_postfx.cpp",
          4042,
          1,
          "%s\n\t(blurRadiusFinal) = %g",
          "(blurRadiusFinal >= 0.0f)",
          blurRadiusFinal) )
  {
    __debugbreak();
  }
  return blurRadiusFinal;
}

void __cdecl RB_BlurScreen(const GfxViewInfo *viewInfo, float blurRadius)
{
  const char *v2; // eax
  const char *v3; // eax
  int v4; // [esp+44h] [ebp-28h]
  int v5; // [esp+48h] [ebp-24h]
  int semaphore; // [esp+4Ch] [ebp-20h]
  int hr; // [esp+50h] [ebp-1Ch]
  float blurRadiusMin; // [esp+54h] [ebp-18h]
  IDirect3DSurface9 *imageSurface; // [esp+58h] [ebp-14h]
  unsigned int color; // [esp+64h] [ebp-8h]

  if ( !viewInfo
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_postfx.cpp", 4238, 0, "%s", "viewInfo") )
  {
    __debugbreak();
  }
  blurRadiusMin = 1440.0 / (float)*(int *)&gfxCmdBufSourceState.viewportIsDirty;
  color = -1;
  if ( blurRadiusMin > blurRadius )
  {
    HIBYTE(color) = (int)((float)((float)(blurRadius / blurRadiusMin) * 255.0) + 9.313225746154785e-10);
    blurRadius = 1440.0 / (float)*(int *)&gfxCmdBufSourceState.viewportIsDirty;
  }
  imageSurface = Image_GetSurface(stru_B50E8A8.image);
  if ( !imageSurface
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_postfx.cpp", 4254, 0, "%s", "imageSurface") )
  {
    __debugbreak();
  }
  R_AssertDXDeviceOwnership();
  if ( r_logFile && r_logFile->current.integer )
    RB_LogPrint(
      "gfxCmdBufContext.state->prim.device->StretchRect( gfxRenderTargets[srcRtId].surface.color, 0, imageSurface, 0, D3DTEXF_LINEAR )\n");
  semaphore = R_AcquireDXDeviceOwnership(0);
  hr = gfxCmdBufContext.state->prim.device->StretchRect(
         gfxCmdBufContext.state->prim.device,
         (IDirect3DSurface9 *)dword_B50E870,
         0,
         imageSurface,
         0,
         D3DTEXF_LINEAR);
  if ( semaphore )
    R_ReleaseDXDeviceOwnership();
  if ( hr < 0 )
  {
    ++g_disableRendering;
    v2 = R_ErrorDescription(hr);
    Com_Error(
      ERR_FATAL,
      "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_postfx.cpp (%i) gfxCmdBufContext.state->prim.device->StretchRect( g"
      "fxRenderTargets[srcRtId].surface.color, 0, imageSurface, 0, D3DTEXF_LINEAR ) failed: %s\n",
      4256,
      v2);
  }
  R_AssertDXDeviceOwnership();
  if ( r_logFile && r_logFile->current.integer )
    RB_LogPrint("imageSurface->Release()\n");
  v4 = R_AcquireDXDeviceOwnership(0);
  v5 = imageSurface->Release(imageSurface);
  if ( v4 )
    R_ReleaseDXDeviceOwnership();
  if ( v5 < 0 )
  {
    ++g_disableRendering;
    v3 = R_ErrorDescription(v5);
    Com_Error(
      ERR_FATAL,
      "C:\\projects_pc\\cod\\codsrc\\src\\gfx_d3d\\rb_postfx.cpp (%i) imageSurface->Release() failed: %s\n",
      4257,
      v3);
  }
  RB_GaussianFilterImage(blurRadius, 6u, 0xCu);
  R_SetRenderTargetSize(&gfxCmdBufSourceState, 3u);
  R_SetRenderTarget(gfxCmdBufContext, 3u);
  R_SetCodeImageTexture(&gfxCmdBufSourceState, 8u, stru_B50E920.image);
  RB_ColoredFilter(rgp.feedbackBlendMaterial, viewInfo, color);
}

