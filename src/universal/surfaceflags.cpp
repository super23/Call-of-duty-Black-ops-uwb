#include "surfaceflags.h"

int __cdecl Com_SurfaceTypeFromName(const char *name)
{
  int i; // [esp+0h] [ebp-4h]

  if ( !name )
    return -1;
  if ( !I_stricmp(name, "default") )
    return 0;
  for ( i = 0; i < 30; ++i )
  {
    if ( !I_stricmp(name, infoParms[i].name) )
      return (unsigned __int8)((int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20] & dword_E0F618[5 * i]) >> 20);
  }
  return -1;
}

const char *__cdecl Com_SurfaceTypeToName(int iTypeIndex)
{
  if ( iTypeIndex <= 0 || iTypeIndex >= 31 )
    return "default";
  if ( (unsigned __int8)((int)((unsigned int)&bg_vehicleInfos[11].rotorTailStartFx[20] & dword_E0F618[5 * iTypeIndex - 5]) >> 20) != iTypeIndex
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\surfaceflags.cpp",
          149,
          0,
          "%s",
          "SURF_TYPEINDEX( infoParms[iTypeIndex - 1].surfaceFlags ) == iTypeIndex") )
  {
    __debugbreak();
  }
  return infoParms[iTypeIndex - 1].name;
}

bool __cdecl Com_SurfaceBurns(int iTypeIndex)
{
  bool burns; // [esp+7h] [ebp-1h]

  switch ( iTypeIndex )
  {
    case 1:
    case 3:
    case 4:
    case 7:
    case 8:
    case 10:
    case 15:
    case 21:
    case 23:
    case 24:
    case 25:
      burns = 1;
      break;
    default:
      burns = 0;
      break;
  }
  return burns;
}

bool __cdecl Com_SurfaceFloats(int iTypeIndex)
{
  return iTypeIndex == 21 || iTypeIndex > 23 && iTypeIndex <= 27;
}

double __cdecl Com_SurfaceDensity(int iTypeIndex)
{
  float density; // [esp+4h] [ebp-4h]

  switch ( iTypeIndex )
  {
    case 16:
      density = FLOAT_0_30000001;
      break;
    case 21:
      density = FLOAT_0_5;
      break;
    case 24:
      density = FLOAT_0_25;
      break;
    case 25:
      density = FLOAT_0_60000002;
      break;
    case 26:
      density = FLOAT_0_2;
      break;
    case 27:
      density = FLOAT_0_15000001;
      break;
    default:
      density = FLOAT_1_0;
      break;
  }
  return density;
}

double __cdecl Com_SurfaceFrictionScale(int iTypeIndex)
{
  float friction; // [esp+4h] [ebp-4h]

  switch ( iTypeIndex )
  {
    case 6:
    case 10:
    case 11:
      friction = FLOAT_0_89999998;
      break;
    case 12:
      friction = FLOAT_0_75;
      break;
    case 14:
    case 19:
      friction = FLOAT_0_80000001;
      break;
    default:
      friction = FLOAT_1_0;
      break;
  }
  return friction;
}

double __cdecl Com_SurfaceBounceScale(int iTypeIndex)
{
  float bounce; // [esp+4h] [ebp-4h]

  switch ( iTypeIndex )
  {
    case 6:
    case 10:
    case 11:
      bounce = FLOAT_0_30000001;
      break;
    case 14:
      bounce = FLOAT_0_1;
      break;
    case 19:
      bounce = FLOAT_0_2;
      break;
    default:
      bounce = FLOAT_1_0;
      break;
  }
  return bounce;
}

