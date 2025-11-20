#include "destructibledef_load_obj.h"

int __cdecl XModelGetBoneIndex(const XModel *model, unsigned int name, unsigned int offset, unsigned __int8 *index)
{
  unsigned int numBones; // [esp+0h] [ebp-Ch]
  unsigned int localBoneIndex; // [esp+4h] [ebp-8h]
  unsigned __int16 *boneNames; // [esp+8h] [ebp-4h]

  if ( !index && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xmodel_utils.cpp", 379, 0, "%s", "index") )
    __debugbreak();
  boneNames = model->localBoneNames;
  numBones = model->numBones;
  if ( numBones >= 0xA0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xmodel_utils.cpp",
          388,
          0,
          "%s",
          "numBones < DOBJ_MAX_PARTS") )
  {
    __debugbreak();
  }
  for ( localBoneIndex = 0; ; ++localBoneIndex )
  {
    if ( localBoneIndex >= numBones )
      return 0;
    if ( name == boneNames[localBoneIndex] )
      break;
  }
  *index = localBoneIndex + offset;
  if ( *index != localBoneIndex + offset
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\xanim\\xmodel_utils.cpp",
          395,
          0,
          "%s",
          "*index == offset + localBoneIndex") )
  {
    __debugbreak();
  }
  return 1;
}

bool __cdecl DestructiblePiece_ParseDefSpecificFieldType(unsigned __int8 *pStruct, const char *pValue, int iFieldType)
{
  if ( DestructiblePiece_SetFlag((DestructiblePiece *)pStruct, pValue, iFieldType, 15, 19, 1) )
    return 1;
  if ( DestructiblePiece_SetFlag((DestructiblePiece *)pStruct, pValue, iFieldType, 20, 24, 8) )
    return 1;
  if ( DestructiblePiece_SetFlag((DestructiblePiece *)pStruct, pValue, iFieldType, 25, 29, 2) )
    return 1;
  return DestructiblePiece_SetFlag((DestructiblePiece *)pStruct, pValue, iFieldType, 30, 34, 4) != 0;
}

int __cdecl DestructiblePiece_SetFlag(
        DestructiblePiece *piece,
        const char *pValue,
        int iFieldType,
        int minVal,
        int maxVal,
        int flag)
{
  if ( iFieldType < minVal || iFieldType > maxVal )
    return 0;
  if ( atoi(pValue) )
    piece->stages[iFieldType - minVal].flags |= flag;
  return 1;
}

DestructiblePiece *__cdecl DestructiblePieceLoadFile(const char *name, DestructiblePiece *destructiblePiece)
{
  char dest[128]; // [esp+20h] [ebp-4098h] BYREF
  char buffer[16388]; // [esp+A0h] [ebp-4018h] BYREF
  char *last; // [esp+40A8h] [ebp-10h]
  signed int v6; // [esp+40ACh] [ebp-Ch]
  int f; // [esp+40B0h] [ebp-8h] BYREF
  int len; // [esp+40B4h] [ebp-4h]

  last = "DESTRUCTIBLEPIECE";
  len = strlen("DESTRUCTIBLEPIECE");
  if ( !strlen(name) )
    return 0;
  if ( Com_sprintf(dest, 0x80u, "destructiblepiece/%s", name) >= 0 )
  {
    v6 = FS_FOpenFileByMode(dest, &f, FS_READ);
    if ( v6 >= 0 )
    {
      FS_Read((unsigned __int8 *)buffer, len, f);
      buffer[len] = 0;
      if ( !strncmp(buffer, last, len) )
      {
        if ( v6 - len < 0x4000 )
        {
          FS_Read((unsigned __int8 *)buffer, v6 - len, f);
          buffer[v6 - len] = 0;
          FS_FCloseFile(f);
          if ( Info_Validate(buffer) )
          {
            if ( !destructiblePiece
              && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\physics\\destructibledef_load_obj.cpp",
                    808,
                    0,
                    "%s",
                    "destructiblePiece") )
            {
              __debugbreak();
            }
            memset((unsigned __int8 *)destructiblePiece, 0, sizeof(DestructiblePiece));
            if ( ParseConfigStringToStruct(
                   (unsigned __int8 *)destructiblePiece,
                   destructiblePieceFields,
                   70,
                   buffer,
                   35,
                   (int (__cdecl *)(unsigned __int8 *, const char *, const int, const int))DestructiblePiece_ParseDefSpecificFieldType,
                   DestructibleDef_Strcpy) )
            {
              return destructiblePiece;
            }
            else
            {
              return 0;
            }
          }
          else
          {
            Com_PrintError(20, "ERROR: destructible piece file [%s] is not valid\n", name);
            return 0;
          }
        }
        else
        {
          Com_PrintError(20, "ERROR: destructible piece file [%s] is to big\n", name);
          FS_FCloseFile(f);
          return 0;
        }
      }
      else
      {
        Com_PrintError(20, "ERROR: file [%s] is not a destructible piece file\n", name);
        FS_FCloseFile(f);
        return 0;
      }
    }
    else
    {
      Com_PrintError(20, "ERROR: destructible piece '%s' not found\n", name);
      return 0;
    }
  }
  else
  {
    Com_PrintError(20, "ERROR: filename '%s' too long\n", dest);
    return 0;
  }
}

void __cdecl DestructibleDef_Strcpy(unsigned __int8 *member, const char *keyValue)
{
  char v2; // [esp+3h] [ebp-25h]
  unsigned __int8 *v3; // [esp+8h] [ebp-20h]
  const char *v4; // [esp+Ch] [ebp-1Ch]
  unsigned __int8 *buf; // [esp+20h] [ebp-8h]

  if ( *keyValue )
  {
    buf = Hunk_AllocLowAlign(strlen(keyValue) + 1, 1, "SetDestructibleDefString", 0);
    v4 = keyValue;
    v3 = buf;
    do
    {
      v2 = *v4;
      *v3++ = *v4++;
    }
    while ( v2 );
    *(unsigned int *)member = buf;
  }
  else
  {
    *(unsigned int *)member = &toastPopupTitle;
  }
}

int __cdecl Destructible_ParseDefSpecificFieldType(unsigned __int8 *pStruct, char *pValue, int iFieldType)
{
  float v4; // [esp+0h] [ebp-50h]
  float v5; // [esp+4h] [ebp-4Ch]
  float v6; // [esp+8h] [ebp-48h]
  float v7; // [esp+Ch] [ebp-44h]
  int v8; // [esp+3Ch] [ebp-14h]

  if ( *((unsigned int *)pStruct + 4) )
  {
    if ( iFieldType < 15 || iFieldType > 46 )
    {
      if ( iFieldType < 47 || iFieldType > 78 )
      {
        if ( iFieldType < 79 || iFieldType > 110 )
        {
          if ( iFieldType < 111 || iFieldType > 142 )
          {
            if ( iFieldType < 143 || iFieldType > 174 )
            {
              if ( iFieldType < 175 || iFieldType > 206 )
              {
                if ( iFieldType < 207 || iFieldType > 238 )
                {
                  if ( iFieldType < 239 || iFieldType > 270 )
                  {
                    if ( iFieldType < 271 || iFieldType > 302 )
                    {
                      if ( iFieldType < 303 || iFieldType > 334 )
                      {
                        if ( iFieldType < 335 || iFieldType > 366 )
                        {
                          if ( iFieldType < 367 || iFieldType > 398 )
                          {
                            if ( iFieldType < 399 || iFieldType > 430 )
                            {
                              if ( iFieldType >= 431 && iFieldType <= 462 && iFieldType - 431 < *((unsigned int *)pStruct + 3) )
                                *(_WORD *)(*((unsigned int *)pStruct + 4) + 312 * (iFieldType - 431) + 288) = SL_GetLowercaseString(pValue, 0, SCRIPTINSTANCE_SERVER);
                            }
                            else if ( iFieldType - 399 < *((unsigned int *)pStruct + 3) )
                            {
                              *(float *)(*((unsigned int *)pStruct + 4) + 312 * (iFieldType - 399) + 264) = (float)atoi(pValue) * 0.0099999998;
                            }
                          }
                          else if ( iFieldType - 367 < *((unsigned int *)pStruct + 3) )
                          {
                            DestructibleDef_Strcpy(
                              (unsigned __int8 *)(*((unsigned int *)pStruct + 4) + 312 * (iFieldType - 367) + 284),
                              pValue);
                          }
                        }
                        else if ( iFieldType - 335 < *((unsigned int *)pStruct + 3) )
                        {
                          *(unsigned int *)(*((unsigned int *)pStruct + 4) + 312 * (iFieldType - 335) + 280) = FX_Register(pValue);
                        }
                      }
                      else if ( !G_ExitAfterToolComplete() && iFieldType - 303 < *((unsigned int *)pStruct + 3) )
                      {
                        DestructibleDef_Strcpy(
                          (unsigned __int8 *)(*((unsigned int *)pStruct + 4) + 312 * (iFieldType - 303) + 276),
                          pValue);
                      }
                    }
                    else if ( iFieldType - 271 < *((unsigned int *)pStruct + 3) && pValue && *pValue )
                    {
                      *(unsigned int *)(*((unsigned int *)pStruct + 4) + 312 * (iFieldType - 271) + 268) = XModel_PhysConstraintsPrecache(
                                                                                                 pValue,
                                                                                                 0);
                    }
                  }
                  else if ( iFieldType - 239 < *((unsigned int *)pStruct + 3) )
                  {
                    v4 = atof(pValue);
                    *(float *)(*((unsigned int *)pStruct + 4) + 312 * (iFieldType - 239) + 260) = v4;
                  }
                }
                else if ( iFieldType - 207 < *((unsigned int *)pStruct + 3) )
                {
                  v5 = atof(pValue);
                  *(float *)(*((unsigned int *)pStruct + 4) + 312 * (iFieldType - 207) + 256) = v5;
                }
              }
              else if ( iFieldType - 175 < *((unsigned int *)pStruct + 3) )
              {
                v6 = atof(pValue);
                *(float *)(*((unsigned int *)pStruct + 4) + 312 * (iFieldType - 175) + 252) = v6;
              }
            }
            else if ( iFieldType - 143 < *((unsigned int *)pStruct + 3) )
            {
              v7 = atof(pValue);
              *(float *)(*((unsigned int *)pStruct + 4) + 312 * (iFieldType - 143) + 248) = v7;
            }
          }
          else if ( iFieldType - 111 < *((unsigned int *)pStruct + 3) )
          {
            *(float *)(*((unsigned int *)pStruct + 4) + 312 * (iFieldType - 111) + 244) = (float)atoi(pValue) / 100.0;
          }
        }
        else
        {
          v8 = iFieldType - 79;
          if ( iFieldType - 79 < *((unsigned int *)pStruct + 3) )
          {
            *(_BYTE *)(*((unsigned int *)pStruct + 4) + 312 * v8 + 240) = atoi(pValue);
            if ( *(unsigned __int8 *)(*((unsigned int *)pStruct + 4) + 312 * v8 + 240) != 255
              && *(unsigned __int8 *)(*((unsigned int *)pStruct + 4) + 312 * v8 + 240) >= *((int *)pStruct + 3)
              && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\physics\\destructibledef_load_obj.cpp",
                    854,
                    0,
                    "%s",
                    "ddef->pieces[pieceIndex].parentPiece < ddef->numPieces") )
            {
              __debugbreak();
            }
          }
        }
      }
      else if ( iFieldType - 47 < *((unsigned int *)pStruct + 3) )
      {
        *(unsigned int *)(*((unsigned int *)pStruct + 4) + 312 * (iFieldType - 47) + 272) = atoi(pValue);
      }
    }
    else
    {
      DestructiblePieceLoadFile(pValue, (DestructiblePiece *)(*((unsigned int *)pStruct + 4) + 312 * (iFieldType - 15)));
    }
    return 1;
  }
  else
  {
    if ( iFieldType >= 15 && iFieldType <= 46 && iFieldType - 15 + 1 > *((unsigned int *)pStruct + 3) )
      *((unsigned int *)pStruct + 3) = iFieldType - 15 + 1;
    return 1;
  }
}

DestructibleDef *__cdecl DestructibleDefLoadFile(const char *name, void *(__cdecl *Alloc)(int))
{
  unsigned __int8 *v3; // eax
  void *v4; // eax
  char *v5; // eax
  char *v6; // eax
  const char *v7; // [esp-14h] [ebp-80A8h]
  const char *v8; // [esp-Ch] [ebp-80A0h]
  const char *v9; // [esp-Ch] [ebp-80A0h]
  int v10; // [esp-8h] [ebp-809Ch]
  int v11; // [esp-8h] [ebp-809Ch]
  int v12; // [esp-4h] [ebp-8098h]
  int v13; // [esp-4h] [ebp-8098h]
  unsigned __int8 index; // [esp+27h] [ebp-806Dh] BYREF
  unsigned int namea; // [esp+28h] [ebp-806Ch]
  int j; // [esp+2Ch] [ebp-8068h]
  int i; // [esp+30h] [ebp-8064h]
  char dest[64]; // [esp+34h] [ebp-8060h] BYREF
  char buffer[32768]; // [esp+74h] [ebp-8020h] BYREF
  char *last; // [esp+8078h] [ebp-1Ch]
  unsigned __int8 *pStruct; // [esp+807Ch] [ebp-18h]
  unsigned int count; // [esp+8080h] [ebp-14h]
  signed int v23; // [esp+8084h] [ebp-10h]
  int f; // [esp+8088h] [ebp-Ch] BYREF
  XModel *model; // [esp+808Ch] [ebp-8h]
  int len; // [esp+8090h] [ebp-4h]

  last = "DESTRUCTIBLEDEF";
  len = strlen("DESTRUCTIBLEDEF");
  if ( !strlen(name) )
    return 0;
  if ( Com_sprintf(dest, 0x40u, "destructibledef/%s", name) >= 0 )
  {
    v23 = FS_FOpenFileByMode(dest, &f, FS_READ);
    if ( v23 >= 0 )
    {
      FS_Read((unsigned __int8 *)buffer, len, f);
      buffer[len] = 0;
      if ( !strncmp(buffer, last, len) )
      {
        if ( v23 - len < 0x8000 )
        {
          FS_Read((unsigned __int8 *)buffer, v23 - len, f);
          buffer[v23 - len] = 0;
          FS_FCloseFile(f);
          if ( Info_Validate(buffer) )
          {
            pStruct = (unsigned __int8 *)Alloc(24);
            if ( !pStruct
              && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\physics\\destructibledef_load_obj.cpp",
                    995,
                    0,
                    "%s",
                    "destructibleDef") )
            {
              __debugbreak();
            }
            v3 = pStruct;
            *(unsigned int *)pStruct = 0;
            *((unsigned int *)v3 + 1) = 0;
            *((unsigned int *)v3 + 2) = 0;
            *((unsigned int *)v3 + 3) = 0;
            *((unsigned int *)v3 + 4) = 0;
            *((unsigned int *)v3 + 5) = 0;
            if ( ParseConfigStringToStruct(
                   pStruct,
                   destructibleDefFields,
                   453,
                   buffer,
                   463,
                   (int (__cdecl *)(unsigned __int8 *, const char *, const int, const int))Destructible_ParseDefSpecificFieldType,
                   DestructibleDef_Strcpy) )
            {
              count = 312 * *((unsigned int *)pStruct + 3);
              v4 = Alloc(count);
              *((unsigned int *)pStruct + 4) = v4;
              memset(*((unsigned __int8 **)pStruct + 4), 0, count);
              if ( ParseConfigStringToStruct(
                     pStruct,
                     destructibleDefFields,
                     453,
                     buffer,
                     463,
                     (int (__cdecl *)(unsigned __int8 *, const char *, const int, const int))Destructible_ParseDefSpecificFieldType,
                     DestructibleDef_Strcpy) )
              {
                model = (XModel *)*((unsigned int *)pStruct + 1);
                if ( model )
                {
                  for ( i = 0; i < *((unsigned int *)pStruct + 3); ++i )
                  {
                    for ( j = 0; j < 5; ++j )
                    {
                      *(float *)(*((unsigned int *)pStruct + 4) + 312 * i + 48 * j + 4) = *(float *)(312 * i
                                                                                             + *((unsigned int *)pStruct + 4)
                                                                                             + 48 * j
                                                                                             + 4)
                                                                                  / 100.0;
                      namea = *(unsigned __int16 *)(312 * i + *((unsigned int *)pStruct + 4) + 48 * j);
                      if ( namea )
                      {
                        index = 0;
                        if ( XModelGetBoneIndex(model, namea, &index) )
                        {
                          if ( *(unsigned int *)(*((unsigned int *)pStruct + 4) + 312 * i + 48 * j + 44) )
                          {
                            if ( !XModelGetCollmapForBoneIndex(model, index) )
                            {
                              v13 = j;
                              v11 = i;
                              v9 = model->name;
                              v7 = **(const char ***)(*((unsigned int *)pStruct + 4) + 312 * i + 48 * j + 44);
                              v6 = SL_ConvertToString(namea, SCRIPTINSTANCE_SERVER);
                              Com_PrintError(
                                20,
                                "ERROR: No collmap for bone [%s] but it has a physPreset [%s] in destructible file [%s] m"
                                "odel [%s] piece [%i] stage [%i]\n",
                                v6,
                                v7,
                                name,
                                v9,
                                v11,
                                v13);
                            }
                          }
                          *(unsigned int *)(*((unsigned int *)pStruct + 4) + 312 * i + 4 * ((int)index >> 5) + 292) = *(unsigned int *)(312 * i + *((unsigned int *)pStruct + 4) + 4 * ((int)index >> 5) + 292) | (0x80000000 >> (index & 0x1F));
                        }
                        else
                        {
                          v12 = j;
                          v10 = i;
                          v8 = model->name;
                          v5 = SL_ConvertToString(namea, SCRIPTINSTANCE_SERVER);
                          Com_PrintError(
                            20,
                            "ERROR: Destructible file [%s] can not find bone [%s] in model [%s] for piece [%i] stage [%i]\n",
                            name,
                            v5,
                            v8,
                            v10,
                            v12);
                        }
                      }
                    }
                  }
                  return (DestructibleDef *)pStruct;
                }
                else
                {
                  return 0;
                }
              }
              else
              {
                return 0;
              }
            }
            else
            {
              return 0;
            }
          }
          else
          {
            Com_PrintError(20, "ERROR: Destructible file [%s] is not valid\n", name);
            return 0;
          }
        }
        else
        {
          Com_PrintError(20, "ERROR: Destructible file [%s] is too big\n", name);
          FS_FCloseFile(f);
          return 0;
        }
      }
      else
      {
        Com_PrintError(20, "ERROR: File [%s] is not a destructible file\n", name);
        FS_FCloseFile(f);
        return 0;
      }
    }
    else
    {
      Com_PrintError(20, "ERROR: Destructible '%s' not found\n", name);
      return 0;
    }
  }
  else
  {
    Com_PrintError(20, "ERROR: Filename '%s' too long\n", dest);
    return 0;
  }
}

int __cdecl XModelGetBoneIndex(const XModel *model, unsigned int name, unsigned __int8 *index)
{
  unsigned int numBones; // [esp+0h] [ebp-Ch]
  unsigned int localBoneIndex; // [esp+4h] [ebp-8h]
  unsigned __int16 *boneNames; // [esp+8h] [ebp-4h]

  if ( !index
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\destructibledef_load_obj.cpp",
          1082,
          0,
          "%s",
          "index") )
  {
    __debugbreak();
  }
  boneNames = model->localBoneNames;
  numBones = model->numBones;
  if ( numBones >= 0xA0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\destructibledef_load_obj.cpp",
          1087,
          0,
          "%s",
          "numBones < DOBJ_MAX_PARTS") )
  {
    __debugbreak();
  }
  for ( localBoneIndex = 0; ; ++localBoneIndex )
  {
    if ( localBoneIndex >= numBones )
      return 0;
    if ( name == boneNames[localBoneIndex] )
      break;
  }
  *index = localBoneIndex;
  if ( *index != localBoneIndex
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\physics\\destructibledef_load_obj.cpp",
          1094,
          0,
          "%s",
          "*index == localBoneIndex") )
  {
    __debugbreak();
  }
  return 1;
}

