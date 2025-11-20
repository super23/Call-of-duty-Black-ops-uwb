#include "r_exposure.h"

void __cdecl R_DefaultExposureValue(int index)
{
  exposureSettings.exposureValues[index].blackPoint[0] = *(float *)&FLOAT_0_0;
  exposureSettings.exposureValues[index].blackPoint[1] = *(float *)&FLOAT_0_0;
  exposureSettings.exposureValues[index].blackPoint[2] = *(float *)&FLOAT_0_0;
  exposureSettings.exposureValues[index].blackPoint[3] = FLOAT_8_0;
  exposureSettings.exposureValues[index].whitePoint[0] = FLOAT_8_0;
  exposureSettings.exposureValues[index].whitePoint[1] = FLOAT_8_0;
  exposureSettings.exposureValues[index].whitePoint[2] = FLOAT_8_0;
  exposureSettings.exposureValues[index].whitePoint[3] = *(float *)&FLOAT_0_0;
  exposureSettings.exposureValues[index].linearStart[0] = *(float *)&FLOAT_0_0;
  exposureSettings.exposureValues[index].linearStart[1] = *(float *)&FLOAT_0_0;
  exposureSettings.exposureValues[index].linearStart[2] = *(float *)&FLOAT_0_0;
  exposureSettings.exposureValues[index].linearStart[3] = *(float *)&FLOAT_0_0;
  exposureSettings.exposureValues[index].linearEnd[0] = FLOAT_1_0;
  exposureSettings.exposureValues[index].linearEnd[1] = FLOAT_1_0;
  exposureSettings.exposureValues[index].linearEnd[2] = FLOAT_1_0;
  exposureSettings.exposureValues[index].linearEnd[3] = *(float *)&FLOAT_0_0;
  exposureSettings.exposureValues[index].remapStart[0] = *(float *)&FLOAT_0_0;
  exposureSettings.exposureValues[index].remapStart[1] = *(float *)&FLOAT_0_0;
  exposureSettings.exposureValues[index].remapStart[2] = *(float *)&FLOAT_0_0;
  exposureSettings.exposureValues[index].remapStart[3] = *(float *)&FLOAT_0_0;
  exposureSettings.exposureValues[index].remapEnd[0] = FLOAT_1_0;
  exposureSettings.exposureValues[index].remapEnd[1] = FLOAT_1_0;
  exposureSettings.exposureValues[index].remapEnd[2] = FLOAT_1_0;
  exposureSettings.exposureValues[index].remapEnd[3] = *(float *)&FLOAT_0_0;
  exposureSettings.exposureValues[index].scurveStart[0] = FLOAT_N0_5;
  exposureSettings.exposureValues[index].scurveStart[1] = FLOAT_N0_5;
  exposureSettings.exposureValues[index].scurveStart[2] = FLOAT_N0_5;
  exposureSettings.exposureValues[index].scurveStart[3] = *(float *)&FLOAT_0_0;
  exposureSettings.exposureValues[index].scurveEnd[0] = FLOAT_N0_5;
  exposureSettings.exposureValues[index].scurveEnd[1] = FLOAT_N0_5;
  exposureSettings.exposureValues[index].scurveEnd[2] = FLOAT_N0_5;
  exposureSettings.exposureValues[index].scurveEnd[3] = *(float *)&FLOAT_0_0;
  exposureSettings.exposureValues[index].bloomCurveLDR[0] = FLOAT_1_0;
  exposureSettings.exposureValues[index].bloomCurveLDR[1] = FLOAT_1_0;
  exposureSettings.exposureValues[index].bloomCurveLDR[2] = FLOAT_1_0;
  exposureSettings.exposureValues[index].bloomCurveLDR[3] = FLOAT_1_0;
  exposureSettings.exposureValues[index].bloomCurveHDR[0] = FLOAT_1_0;
  exposureSettings.exposureValues[index].bloomCurveHDR[1] = FLOAT_1_0;
  exposureSettings.exposureValues[index].bloomCurveHDR[2] = FLOAT_1_0;
  exposureSettings.exposureValues[index].bloomCurveHDR[3] = FLOAT_1_0;
  exposureSettings.exposureValues[index].bloomScale[0] = *(float *)&FLOAT_0_0;
  exposureSettings.exposureValues[index].bloomScale[1] = *(float *)&FLOAT_0_0;
  exposureSettings.exposureValues[index].bloomScale[2] = *(float *)&FLOAT_0_0;
  exposureSettings.exposureValues[index].bloomScale[3] = *(float *)&FLOAT_0_0;
}

void __cdecl R_SetExposureToDefault()
{
  int i; // [esp+0h] [ebp-4h]

  for ( i = 0; i < 256; ++i )
    R_DefaultExposureValue(i);
}

void __cdecl R_LoadExposureData(unsigned int *data)
{
  unsigned int i; // [esp+4h] [ebp-Ch]

  for ( i = 0; i < 0x2C04; ++i )
    *(&exposureSettings.header.identifier + i) = data[i];
}

