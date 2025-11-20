#include "r_skybox.h"

void R_RegisterSkyboxModel()
{
  if ( rgp.world->skyBoxModel )
  {
    if ( *rgp.world->skyBoxModel )
      R_RegisterSkyboxModel(rgp.world->skyBoxModel);
  }
}

void __cdecl R_RegisterSkyboxModel(char *xmodelName)
{
  g_skyboxModel = R_RegisterModel(xmodelName);
}

void __cdecl R_ClearSkyboxModel()
{
  g_skyboxModel = 0;
}

void __cdecl R_AddSkyboxModel(const float *eyePos)
{
  GfxScaledPlacement placement; // [esp+4h] [ebp-20h] BYREF

  if ( g_skyboxModel )
  {
    placement.scale = FLOAT_1_0;
    placement.base.origin[0] = *eyePos;
    placement.base.origin[1] = eyePos[1];
    placement.base.origin[2] = eyePos[2];
    memset(&placement, 0, 12);
    placement.base.quat[3] = FLOAT_1_0;
    R_FilterXModelIntoScene(g_skyboxModel, &placement, 0, &myhandle, 0.0);
  }
}

XModel *__cdecl R_MapGetSkyboxModel()
{
  return g_skyboxModel;
}

