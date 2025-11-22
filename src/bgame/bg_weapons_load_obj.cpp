#include "bg_weapons_load_obj.h"

char *__cdecl BG_GetPlayerAnimTypeName(int index)
{
  return g_playerAnimTypeNames[index];
}

int __cdecl BG_GetPlayerAnimTypeIndex(int typeEnum)
{
  int result; // eax

  switch ( typeEnum )
  {
    case 0:
      result = g_playerAnimTypeIndex_revivee;
      break;
    case 1:
      result = g_playerAnimTypeIndex_sniper;
      break;
    case 2:
      result = g_playerAnimTypeIndex_sniper_rearclip;
      break;
    case 3:
      result = g_playerAnimTypeIndex_briefcase;
      break;
    default:
      result = -1;
      break;
  }
  return result;
}

const char *__cdecl BG_GetWeaponTypeName(weapType_t type)
{
  if ( (unsigned int)type >= WEAPTYPE_NUM
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_weapons_load_obj.cpp",
          1465,
          0,
          "type doesn't index ARRAY_COUNT( szWeapTypeNames )\n\t%i not in [0, %i)",
          type,
          8) )
  {
    __debugbreak();
  }
  return szWeapTypeNames[type];
}

const char *__cdecl BG_GetWeaponClassName(weapClass_t type)
{
  if ( (unsigned int)type >= WEAPCLASS_NUM
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_weapons_load_obj.cpp",
          1473,
          0,
          "type doesn't index ARRAY_COUNT( szWeapClassNames )\n\t%i not in [0, %i)",
          type,
          13) )
  {
    __debugbreak();
  }
  return szWeapClassNames[type];
}

const char *__cdecl BG_GetWeaponInventoryTypeName(weapInventoryType_t type)
{
  if ( (unsigned int)type >= WEAPINVENTORYCOUNT
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_weapons_load_obj.cpp",
          1481,
          0,
          "type doesn't index ARRAY_COUNT( szWeapInventoryTypeNames )\n\t%i not in [0, %i)",
          type,
          5) )
  {
    __debugbreak();
  }
  return szWeapInventoryTypeNames[type];
}

void __cdecl BG_LoadWeaponStrings()
{
  unsigned int i; // [esp+0h] [ebp-4h]

  for ( i = 0; i < g_playerAnimTypeNamesCount; ++i )
    BG_InitWeaponString(i, g_playerAnimTypeNames[i]);
}

void __cdecl BG_LoadPlayerAnimTypes()
{
  char v0; // [esp+53h] [ebp-29h]
  char *v1; // [esp+58h] [ebp-24h]
  const char *v2; // [esp+5Ch] [ebp-20h]
  char *buf; // [esp+70h] [ebp-Ch]
  const char *text_p; // [esp+74h] [ebp-8h] BYREF
  const char *token; // [esp+78h] [ebp-4h]

  g_playerAnimTypeNamesCount = 0;
  g_playerAnimTypeIndex_revivee = -1;
  buf = Com_LoadRawTextFile("mp/playeranimtypes.txt");
  if ( !buf )
    Com_Error(ERR_DROP, &byte_C754E4, "mp/playeranimtypes.txt");
  text_p = buf;
  Com_BeginParseSession("BG_AnimParseAnimScript");
  while ( 1 )
  {
    token = (const char *)Com_Parse(&text_p);
    if ( !token || !*token )
      break;
    if ( g_playerAnimTypeNamesCount >= 0x20 )
      Com_Error(ERR_DROP, &byte_C754BC);
    g_playerAnimTypeNames[g_playerAnimTypeNamesCount] = (char *)Hunk_Alloc(
                                                                  strlen(token) + 1,
                                                                  "BG_LoadPlayerAnimTypes",
                                                                  10);
    v2 = token;
    v1 = g_playerAnimTypeNames[g_playerAnimTypeNamesCount];
    do
    {
      v0 = *v2;
      *v1++ = *v2++;
    }
    while ( v0 );
    if ( !strcmp(token, "revivee") )
    {
      g_playerAnimTypeIndex_revivee = g_playerAnimTypeNamesCount;
    }
    else if ( !strcmp(token, "sniper") )
    {
      g_playerAnimTypeIndex_sniper = g_playerAnimTypeNamesCount;
    }
    else if ( !strcmp(token, "briefcase") )
    {
      g_playerAnimTypeIndex_briefcase = g_playerAnimTypeNamesCount;
    }
    else if ( !strcmp(token, "rearclipsniper") )
    {
      g_playerAnimTypeIndex_sniper_rearclip = g_playerAnimTypeNamesCount;
    }
    ++g_playerAnimTypeNamesCount;
  }
  Com_EndParseSession();
  Com_UnloadRawTextFile(buf);
}

void __cdecl BG_ClearWeaponDefInternal()
{
  surfaceTypeSoundListCount = 0;
}

void __cdecl SetConfigString(char **ppszConfigString, const char *pszKeyValue)
{
  char v2; // [esp+3h] [ebp-21h]
  char *v3; // [esp+8h] [ebp-1Ch]
  const char *v4; // [esp+Ch] [ebp-18h]
  char *buf; // [esp+20h] [ebp-4h]

  if ( *pszKeyValue )
  {
    buf = (char *)Hunk_AllocLowAlign(strlen(pszKeyValue) + 1, 1, "SetConfigString", 10);
    v4 = pszKeyValue;
    v3 = buf;
    do
    {
      v2 = *v4;
      *v3++ = *v4++;
    }
    while ( v2 );
    *ppszConfigString = buf;
  }
  else
  {
    *ppszConfigString = (char *)&toastPopupTitle;
  }
}

flameTable *__cdecl BG_LoadFlameTableInternal(const char *folder, char *name)
{
  char buffer[10244]; // [esp+14h] [ebp-2858h] BYREF
  int f; // [esp+2818h] [ebp-54h] BYREF
  int len; // [esp+281Ch] [ebp-50h]
  signed int v6; // [esp+2820h] [ebp-4Ch]
  char dest[64]; // [esp+2824h] [ebp-48h] BYREF
  flameTable *fTable; // [esp+2868h] [ebp-4h]

  len = strlen("FLAMETABLEFILE");
  fTable = (flameTable *)Hunk_AllocLow(0x1DCu, "BG_LoadFlameTableInternal", 10);
  InitFlameTable(fTable);
  Com_sprintf(dest, 0x40u, "weapons/%s/%s", folder, name);
  v6 = FS_FOpenFileByMode(dest, &f, FS_READ);
  if ( v6 >= 0 )
  {
    FS_Read((unsigned __int8 *)buffer, len, f);
    buffer[len] = 0;
    if ( !strncmp(buffer, "FLAMETABLEFILE", len) )
    {
      if ( (unsigned int)(v6 - len) < 0x2800 )
      {
        memset((unsigned __int8 *)buffer, 0, 0x2800u);
        FS_Read((unsigned __int8 *)buffer, v6 - len, f);
        buffer[v6 - len] = 0;
        FS_FCloseFile(f);
        if ( Info_Validate(buffer) )
        {
          SetConfigString((char **)&fTable->name, name);
          if ( ParseConfigStringToStruct(
                 (unsigned __int8 *)fTable,
                 flameTableFields,
                 119,
                 buffer,
                 0,
                 0,
                 SetConfigString2) )
          {
            if ( bg_nextParseFlameTableIndex >= 8
              && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_weapons_load_obj.cpp",
                    2123,
                    0,
                    "bg_nextParseFlameTableIndex doesn't index ARRAY_COUNT( bg_flameTables )\n\t%i not in [0, %i)",
                    bg_nextParseFlameTableIndex,
                    8) )
            {
              __debugbreak();
            }
            bg_flameTables[bg_nextParseFlameTableIndex++] = fTable;
            return fTable;
          }
          else
          {
            return 0;
          }
        }
        else
        {
          Com_PrintWarning(17, "WARNING: \"%s\" is not a valid flametable file\n", dest);
          return 0;
        }
      }
      else
      {
        Com_PrintWarning(
          17,
          "WARNING: \"%s\" Is too long of a flametable file to parse (fileLength = %d identifierLength = %d)\n",
          dest,
          v6,
          len);
        FS_FCloseFile(f);
        return 0;
      }
    }
    else
    {
      Com_PrintWarning(17, "WARNING: \"%s\" does not appear to be a flametable file\n", dest);
      FS_FCloseFile(f);
      return 0;
    }
  }
  else
  {
    Com_PrintWarning(17, "WARNING: Could not load flametable file '%s'\n", dest);
    return 0;
  }
}

void __cdecl SetConfigString2(unsigned __int8 *pMember, const char *pszKeyValue)
{
  SetConfigString((char **)pMember, pszKeyValue);
}

void __cdecl InitFlameTable(flameTable *fTable)
{
  const cspField_t *pField; // [esp+4h] [ebp-8h]
  int iField; // [esp+8h] [ebp-4h]

  iField = 0;
  pField = flameTableFields;
  while ( iField < 119 )
  {
    if ( !pField->iFieldType )
      *(unsigned int *)((char *)&fTable->flameVar_streamChunkGravityStart + pField->iOffset) = &toastPopupTitle;
    ++iField;
    ++pField;
  }
}

flameTable *__cdecl BG_GetFlameTable(const char *folder, char *name)
{
  unsigned int i; // [esp+0h] [ebp-4h]

  for ( i = 0; i < bg_nextParseFlameTableIndex; ++i )
  {
    if ( !I_stricmp(bg_flameTables[i]->name, name) )
      return bg_flameTables[i];
  }
  return BG_LoadFlameTableInternal(folder, name);
}

char __cdecl BG_LoadWeaponFile(char *szFileName, char *szBuffer, int iBufferSize)
{
  int hFile; // [esp+14h] [ebp-Ch] BYREF
  int iIdentifierLen; // [esp+18h] [ebp-8h]
  int iFileLength; // [esp+1Ch] [ebp-4h]

  iIdentifierLen = strlen("WEAPONFILE");
  iFileLength = FS_FOpenFileByMode(szFileName, &hFile, FS_READ);
  if ( iFileLength >= 0 )
  {
    FS_Read((unsigned __int8 *)szBuffer, iIdentifierLen, hFile);
    szBuffer[iIdentifierLen] = 0;
    if ( !strncmp(szBuffer, "WEAPONFILE", iIdentifierLen) )
    {
      if ( iFileLength - iIdentifierLen < iBufferSize )
      {
        memset((unsigned __int8 *)szBuffer, 0, iBufferSize);
        FS_Read((unsigned __int8 *)szBuffer, iFileLength - iIdentifierLen, hFile);
        szBuffer[iFileLength - iIdentifierLen] = 0;
        FS_FCloseFile(hFile);
        if ( Info_Validate(szBuffer) )
        {
          return 1;
        }
        else
        {
          Com_PrintWarning(17, "WARNING: \"%s\" is not a valid weapon file\n", szFileName);
          return 0;
        }
      }
      else
      {
        Com_PrintWarning(
          17,
          "WARNING: \"%s\" Is too long of a weapon file to parse (fileLength = %d identifierLength = %d)\n",
          szFileName,
          iFileLength,
          iIdentifierLen);
        FS_FCloseFile(hFile);
        return 0;
      }
    }
    else
    {
      Com_PrintWarning(17, "WARNING: \"%s\" does not appear to be a weapon file\n", szFileName);
      FS_FCloseFile(hFile);
      return 0;
    }
  }
  else
  {
    Com_PrintWarning(17, "WARNING: Could not load weapon file '%s'\n", szFileName);
    return 0;
  }
}

void __cdecl BG_LoadWeaponMergeSupport()
{
  char v0; // [esp+3h] [ebp-2Dh]
  char *v1; // [esp+8h] [ebp-28h]
  const char *v2; // [esp+Ch] [ebp-24h]
  char *buf; // [esp+24h] [ebp-Ch]
  const char *text_p; // [esp+28h] [ebp-8h] BYREF
  const char *token; // [esp+2Ch] [ebp-4h]

  s_numWeaponGripAnimSubstrings = 0;
  if ( !useFastFile->current.enabled )
  {
    BG_LoadWeaponAttachmentTable();
    buf = Com_LoadRawTextFile("weapon_gripanims.csv");
    if ( !buf )
      Com_Error(ERR_DROP, &byte_C754E4, "weapon_gripanims.csv");
    text_p = buf;
    Com_BeginParseSession("BG_LoadWeaponMergeSupport");
    while ( 1 )
    {
      token = (const char *)Com_Parse(&text_p);
      if ( !token || !*token )
        break;
      if ( (unsigned int)s_numWeaponGripAnimSubstrings >= 0x40 )
        Com_Error(ERR_DROP, &byte_C757BC);
      s_weaponGripAnimSubstrings[s_numWeaponGripAnimSubstrings] = (char *)Hunk_Alloc(
                                                                            strlen(token) + 1,
                                                                            "BG_LoadWeaponMergeSupport",
                                                                            10);
      v2 = token;
      v1 = s_weaponGripAnimSubstrings[s_numWeaponGripAnimSubstrings];
      do
      {
        v0 = *v2;
        *v1++ = *v2++;
      }
      while ( v0 );
      ++s_numWeaponGripAnimSubstrings;
    }
    Com_EndParseSession();
    Com_UnloadRawTextFile(buf);
  }
}

char __cdecl BG_SplitWeaponDefNames(
        const char *name,
        char **sources,
        WeaponComponentList *componentAll,
        char *outputName)
{
  int slot; // [esp+0h] [ebp-27Ch]
  WeaponComponentList componentBase; // [esp+4h] [ebp-278h] BYREF
  WeaponComponentList componentBaseAttachment; // [esp+13Ch] [ebp-140h] BYREF
  int count; // [esp+278h] [ebp-4h]

  BG_WeaponNameToComponentList(name, componentAll);
  BG_WeaponComponentListIdentify(componentAll);
  BG_WeaponComponentListSort(name, componentAll, outputName);
  count = BG_WeaponComponentListCountAttachments(componentAll);
  if ( count <= 5 )
  {
    if ( count >= 2 )
    {
      for ( slot = 1; slot < count + 1; ++slot )
      {
        BG_WeaponComponentListNthAttachment(componentAll, slot, &componentBaseAttachment);
        BG_WeaponComponentListToName(&componentBaseAttachment, sources[slot], 64);
      }
      BG_WeaponComponentListRemoveAllAttachments(componentAll, &componentBase);
      BG_WeaponComponentListToName(&componentBase, *sources, 64);
      return 1;
    }
    else
    {
      Com_PrintError(1, "Can not create merged weapon %s.  Too few attachments, must have at least two.\n", name);
      return 0;
    }
  }
  else
  {
    Com_PrintError(1, "Can not create merged weapon %s.  Too many attachments, max is %i\n", name, 5);
    return 0;
  }
}

int __cdecl BG_MergeWeaponDefClipName(char **value, char *mergedValue, int size)
{
  char v4; // [esp+3h] [ebp-529h]
  WeaponComponent *v5; // [esp+8h] [ebp-524h]
  WeaponComponent *v6; // [esp+Ch] [ebp-520h]
  char v7; // [esp+13h] [ebp-519h]
  WeaponComponent *v8; // [esp+18h] [ebp-514h]
  WeaponComponent *v9; // [esp+1Ch] [ebp-510h]
  char v10; // [esp+23h] [ebp-509h]
  WeaponComponent *v11; // [esp+28h] [ebp-504h]
  WeaponComponent *v12; // [esp+2Ch] [ebp-500h]
  int m; // [esp+30h] [ebp-4FCh]
  WeaponComponentList merged; // [esp+34h] [ebp-4F8h] BYREF
  int comp[3]; // [esp+170h] [ebp-3BCh] BYREF
  WeaponComponentList componentList[3]; // [esp+17Ch] [ebp-3B0h] BYREF

  memset((unsigned __int8 *)&merged, 0, sizeof(merged));
  for ( m = 0; m < 3; ++m )
    BG_WeaponNameToComponentList(value[m], &componentList[m]);
  memset(comp, 0, sizeof(comp));
  while ( comp[0] < componentList[0].numComponents )
  {
    if ( comp[1] < componentList[1].numComponents
      && I_stricmp(componentList[1].components[comp[1]].name, componentList[0].components[comp[0]].name) )
    {
      v12 = &componentList[1].components[comp[1]];
      v11 = &merged.components[merged.numComponents];
      do
      {
        v10 = v12->name[0];
        v11->name[0] = v12->name[0];
        v12 = (WeaponComponent *)((char *)v12 + 1);
        v11 = (WeaponComponent *)((char *)v11 + 1);
      }
      while ( v10 );
      ++merged.numComponents;
    }
    ++comp[1];
    if ( comp[2] < componentList[2].numComponents
      && I_stricmp(componentList[2].components[comp[2]].name, componentList[0].components[comp[0]].name) )
    {
      v9 = &componentList[2].components[comp[2]];
      v8 = &merged.components[merged.numComponents];
      do
      {
        v7 = v9->name[0];
        v8->name[0] = v9->name[0];
        v9 = (WeaponComponent *)((char *)v9 + 1);
        v8 = (WeaponComponent *)((char *)v8 + 1);
      }
      while ( v7 );
      ++merged.numComponents;
    }
    ++comp[2];
    v6 = &componentList[0].components[comp[0]];
    v5 = &merged.components[merged.numComponents];
    do
    {
      v4 = v6->name[0];
      v5->name[0] = v6->name[0];
      v6 = (WeaponComponent *)((char *)v6 + 1);
      v5 = (WeaponComponent *)((char *)v5 + 1);
    }
    while ( v4 );
    ++merged.numComponents;
    ++comp[0];
  }
  BG_WeaponComponentListToName(&merged, mergedValue, size);
  return 1;
}

int __cdecl BG_MergeWeaponDefFloatValue(char **value, char *mergedValue)
{
  float valueRight; // [esp+8h] [ebp-8h]
  float valueLeft; // [esp+Ch] [ebp-4h]

  valueLeft = atof(value[1]);
  valueRight = atof(value[2]);
  Com_sprintf(mergedValue, 4u, "%.2f", (float)((float)(valueLeft + valueRight) / 2.0));
  return 1;
}

int __cdecl BG_MergeWeaponDefHidetags(char **value, char *mergedValue, int size)
{
  int v4; // [esp+10h] [ebp-10E0h]
  int i; // [esp+14h] [ebp-10DCh]
  _BYTE v6[196]; // [esp+18h] [ebp-10D8h] BYREF
  char *s1; // [esp+DCh] [ebp-1014h]
  char s0[4100]; // [esp+E0h] [ebp-1010h] BYREF
  int j; // [esp+10E8h] [ebp-8h]
  char *data_p; // [esp+10ECh] [ebp-4h] BYREF

  v4 = 0;
  for ( i = 0; i < 3; ++i )
  {
    data_p = value[i];
    for ( s1 = (char *)Com_Parse((const char **)&data_p); s1 && *s1; s1 = (char *)Com_Parse((const char **)&data_p) )
    {
      if ( strlen(s1) >= 0x40
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_weapons_load_obj.cpp",
              2474,
              0,
              "%s",
              "strlen( token ) < MAX_TAG_LEN") )
      {
        __debugbreak();
      }
      for ( j = 0; j < v4; ++j )
      {
        if ( !I_stricmp(&s0[64 * j], s1) )
        {
          v6[3 * j + i] = 1;
          break;
        }
      }
      if ( j == v4 )
      {
        strncpy((unsigned __int8 *)&s0[64 * v4], (unsigned __int8 *)s1, 0x40u);
        v6[3 * v4] = 0;
        v6[3 * v4 + 1] = 0;
        v6[3 * v4 + 2] = 0;
        v6[3 * v4++ + i] = 1;
      }
    }
  }
  *mergedValue = 0;
  for ( j = 0; j < v4; ++j )
  {
    if ( !v6[3 * j] || v6[3 * j + 1] && v6[3 * j + 2] )
    {
      if ( !v6[3 * j + 1]
        && !v6[3 * j + 2]
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_weapons_load_obj.cpp",
              2505,
              0,
              "%s",
              "tagsPresent[tag][MERGE_LEFT] || tagsPresent[tag][MERGE_RIGHT]") )
      {
        __debugbreak();
      }
      I_strncat(mergedValue, size, &s0[64 * j]);
      I_strncat(mergedValue, size, "\n");
    }
  }
  return 1;
}

bool __cdecl BG_IsAnimationField(const char *fieldName)
{
  const char *v2; // [esp+Ch] [ebp-8h]

  v2 = &fieldName[strlen(fieldName) + 1];
  return v2 - (fieldName + 1) >= 4 && I_stricmp(v2 - 5, "anim") == 0;
}

bool __cdecl BG_IsADSField(const char *fieldName)
{
  return I_strnicmp(fieldName, "ads", 3) == 0;
}

char __cdecl BG_IsGripAnimationName(const char *name)
{
  int idx; // [esp+0h] [ebp-4h]

  for ( idx = 0; idx < s_numWeaponGripAnimSubstrings; ++idx )
  {
    if ( I_stristr(name, s_weaponGripAnimSubstrings[idx]) )
      return 1;
  }
  return 0;
}

int __cdecl BG_MergeWeaponDefAnimations(const char *fieldName, char **value, char *mergedValue, unsigned int size)
{
  char rightGrip; // [esp+1h] [ebp-3h]
  char leftGrip; // [esp+2h] [ebp-2h]
  bool isADS; // [esp+3h] [ebp-1h]

  leftGrip = BG_IsGripAnimationName(value[1]);
  rightGrip = BG_IsGripAnimationName(value[2]);
  isADS = BG_IsADSField(fieldName);
  if ( leftGrip && rightGrip )
  {
    Com_PrintError(
      1,
      "Can't merge anim field '%s', both '%s' and '%s' are bottom attachment point animations",
      fieldName,
      value[1],
      value[2]);
    return 0;
  }
  if ( !leftGrip && !rightGrip )
  {
    Com_PrintError(
      1,
      "Can't merge anim field '%s', neither '%s' nor '%s' are bottom attachment point animations",
      fieldName,
      value[1],
      value[2]);
    return 0;
  }
  if ( !leftGrip
    && !rightGrip
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_weapons_load_obj.cpp",
          2564,
          0,
          "%s",
          "leftGrip || rightGrip") )
  {
    __debugbreak();
  }
  if ( !isADS )
  {
    if ( !leftGrip )
      goto LABEL_15;
LABEL_16:
    strncpy((unsigned __int8 *)mergedValue, (unsigned __int8 *)value[1], size);
    return 1;
  }
  if ( !leftGrip )
    goto LABEL_16;
LABEL_15:
  strncpy((unsigned __int8 *)mergedValue, (unsigned __int8 *)value[2], size);
  return 1;
}

int __cdecl BG_MergeWeaponDefSpecialCases(const char *fieldName, char **value, char *mergedValue, int size)
{
  if ( I_stricmp(fieldName, "displayName") )
  {
    if ( I_stricmp(fieldName, "clipName") )
    {
      if ( I_stricmp(fieldName, "hideTags") )
      {
        if ( I_stricmp(fieldName, "adsViewKickCenterSpeed")
          && I_stricmp(fieldName, "hipViewKickCenterSpeed")
          && I_stricmp(fieldName, "hipViewKickYawMax") )
        {
          if ( BG_IsAnimationField(fieldName) )
            return BG_MergeWeaponDefAnimations(fieldName, value, mergedValue, size);
          else
            return 0;
        }
        else
        {
          return BG_MergeWeaponDefFloatValue(value, mergedValue);
        }
      }
      else
      {
        return BG_MergeWeaponDefHidetags(value, mergedValue, size);
      }
    }
    else
    {
      return BG_MergeWeaponDefClipName(value, mergedValue, size);
    }
  }
  else
  {
    _snprintf(mergedValue, size, "%s_DUAL_ATTACH", *value);
    return 1;
  }
}

char __cdecl BG_LoadWeaponVariantDefFile(WeaponFullDef *weapFullDef, const char *folder, char *name)
{
  char *sourceName; // [esp+10h] [ebp-2847Ch] BYREF
  char *v5; // [esp+14h] [ebp-28478h]
  _BYTE *v6; // [esp+18h] [ebp-28474h]
  char *pszBuffer; // [esp+1Ch] [ebp-28470h] BYREF
  char *v8; // [esp+20h] [ebp-2846Ch]
  char *v9; // [esp+24h] [ebp-28468h]
  int v10; // [esp+28h] [ebp-28464h]
  char *sources[6]; // [esp+2Ch] [ebp-28460h] BYREF
  WeaponComponentList componentAll; // [esp+44h] [ebp-28448h] BYREF
  char szBuffer[16384]; // [esp+17Ch] [ebp-28310h] BYREF
  char v14; // [esp+417Ch] [ebp-24310h] BYREF
  _BYTE v15[64]; // [esp+1817Ch] [ebp-10310h] BYREF
  char v16[66116]; // [esp+181BCh] [ebp-102D0h] BYREF
  char **ppszConfigString; // [esp+28400h] [ebp-8Ch]
  char outputName[64]; // [esp+28404h] [ebp-88h] BYREF
  char dest[64]; // [esp+28444h] [ebp-48h] BYREF
  int i; // [esp+28488h] [ebp-4h]

  ppszConfigString = (char **)weapFullDef;
  for ( i = 0; i < 4; ++i )
  {
    v16[0x4000 * i + 320] = 0;
    v16[64 * i + 65856] = 0;
  }
  outputName[0] = 0;
  Com_sprintf(dest, 0x40u, "weapons/%s/%s", folder, name);
  if ( (int)FS_FOpenFileByMode(dest, 0, FS_READ) <= 0 )
  {
    for ( i = 0; i < 6; ++i )
      sources[i] = &v15[64 * i];
    if ( !BG_SplitWeaponDefNames(name, sources, &componentAll, outputName) )
      return 0;
    if ( componentAll.numComponents - 1 > 6
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_weapons_load_obj.cpp",
            2663,
            0,
            "%s",
            "componentAll.numComponents - 1 <= MERGE_BASE_FILE_COUNT") )
    {
      __debugbreak();
    }
    for ( i = 0; i < componentAll.numComponents - 1; ++i )
    {
      Com_sprintf(dest, 0x40u, "weapons/%s/%s", folder, &v15[64 * i]);
      if ( !BG_LoadWeaponFile(dest, &szBuffer[0x4000 * i], 0x4000) )
      {
        Com_PrintError(
          1,
          "Can not create merged weapon %s.  Could not load base or attachment file %s",
          outputName,
          dest);
        return 0;
      }
    }
    SetConfigString(ppszConfigString, outputName);
    sourceName = v15;
    pszBuffer = szBuffer;
    v5 = v16;
    v8 = &v14;
    v10 = BG_WeaponComponentListCountAttachments(&componentAll);
    for ( i = 0; i < v10 - 1; ++i )
    {
      v6 = &v15[64 * i + 128];
      v9 = &szBuffer[0x4000 * i + 0x8000];
      if ( !ParseConfigStringToStructMerged(
              (unsigned __int8 *)weapFullDef,
              weaponDefFields,
              748,
              name,
              (const char **)&pszBuffer,
              (const char **)&sourceName,
              &v16[0x4000 * i + 320],
              43,
              (int (__cdecl *)(unsigned __int8 *, const char *, const int, const int))BG_ParseWeaponDefSpecificFieldType,
              SetConfigString2,
              BG_MergeWeaponDefSpecialCases) )
        return 0;
      componentAll.numComponents = i + 3;
      BG_WeaponComponentListToName(&componentAll, &v16[64 * i + 65856], 64);
      strcat(&v16[64 * i + 65856], "_mp");
      v5 = &v16[64 * i + 65856];
      v8 = &v16[0x4000 * i + 320];
    }
  }
  else
  {
    if ( !BG_LoadWeaponFile(dest, szBuffer, 0x4000) )
      return 0;
    SetConfigString(ppszConfigString, name);
    if ( !ParseConfigStringToStruct(
            (unsigned __int8 *)weapFullDef,
            weaponDefFields,
            748,
            szBuffer,
            43,
            (int (__cdecl *)(unsigned __int8 *, const char *, const int, const int))BG_ParseWeaponDefSpecificFieldType,
            SetConfigString2) )
      return 0;
  }
  return 1;
}

int __cdecl BG_ParseWeaponDefSpecificFieldType(WeaponFullDef *pStruct, const char *pValue, int iFieldType)
{
  unsigned __int16 StringOfSize; // ax
  unsigned __int16 v4; // ax
  unsigned __int16 LowercaseString; // ax
  unsigned __int16 v6; // ax
  int result; // eax
  char v8; // [esp+3h] [ebp-A9h]
  char *v9; // [esp+8h] [ebp-A4h]
  const char *v10; // [esp+Ch] [ebp-A0h]
  int v11; // [esp+10h] [ebp-9Ch]
  const char *pos; // [esp+48h] [ebp-64h] BYREF
  int numHideTags; // [esp+4Ch] [ebp-60h]
  WeaponFullDef *weapFullDef; // [esp+50h] [ebp-5Ch]
  int numNoteTrackMappings; // [esp+54h] [ebp-58h]
  WeaponVariantDef *weapVariantDef; // [esp+58h] [ebp-54h]
  char keyName[64]; // [esp+5Ch] [ebp-50h] BYREF
  int arrayIndex; // [esp+A0h] [ebp-Ch]
  const char *token; // [esp+A4h] [ebp-8h]
  WeaponDef *weapDef; // [esp+A8h] [ebp-4h]

  if ( !pStruct
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_weapons_load_obj.cpp", 1643, 0, "%s", "pStruct") )
  {
    __debugbreak();
  }
  if ( !pValue
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_weapons_load_obj.cpp", 1644, 0, "%s", "pValue") )
  {
    __debugbreak();
  }
  weapFullDef = pStruct;
  weapVariantDef = &pStruct->weapVariantDef;
  weapDef = &pStruct->weapDef;
  switch ( iFieldType )
  {
    case 15:
      arrayIndex = Weapon_GetStringArrayIndex(pValue, szWeapTypeNames, 8);
      if ( arrayIndex < 0 )
        Com_Error(ERR_DROP, &byte_C75EF4, pValue, weapVariantDef->szInternalName);
      weapDef->weapType = arrayIndex;
      goto LABEL_104;
    case 16:
      arrayIndex = Weapon_GetStringArrayIndex(pValue, szWeapClassNames, 13);
      if ( arrayIndex < 0 )
        Com_Error(ERR_DROP, &byte_C75ED0, pValue, weapVariantDef->szInternalName);
      weapDef->weapClass = arrayIndex;
      goto LABEL_104;
    case 17:
      arrayIndex = Weapon_GetStringArrayIndex(pValue, szWeapOverlayReticleNames, 2);
      if ( arrayIndex < 0 )
        Com_Error(ERR_DROP, &byte_C75EA0, pValue, weapVariantDef->szInternalName);
      weapDef->overlayReticle = arrayIndex;
      goto LABEL_104;
    case 18:
      arrayIndex = Weapon_GetStringArrayIndex(pValue, penetrateTypeNames, 4);
      if ( arrayIndex < 0 )
        Com_Error(ERR_DROP, &byte_C75DF8, pValue, weapVariantDef->szInternalName);
      weapDef->penetrateType = arrayIndex;
      goto LABEL_104;
    case 19:
      arrayIndex = Weapon_GetStringArrayIndex(pValue, impactTypeNames_0, 16);
      if ( arrayIndex < 0 )
        Com_Error(ERR_DROP, &byte_C75DD4, pValue, weapVariantDef->szInternalName);
      weapDef->impactType = arrayIndex;
      goto LABEL_104;
    case 20:
      arrayIndex = Weapon_GetStringArrayIndex(pValue, szWeapStanceNames, 3);
      if ( arrayIndex < 0 )
        Com_Error(ERR_DROP, &byte_C75DAC, pValue, weapVariantDef->szInternalName);
      weapDef->stance = arrayIndex;
      goto LABEL_104;
    case 21:
      arrayIndex = Weapon_GetStringArrayIndex(pValue, szProjectileExplosionNames, 10);
      if ( arrayIndex < 0 )
        Com_Error(ERR_DROP, &byte_C75D80, pValue, weapVariantDef->szInternalName);
      weapDef->projExplosion = arrayIndex;
      goto LABEL_104;
    case 22:
      arrayIndex = Weapon_GetStringArrayIndex(pValue, offhandClassNames, 5);
      if ( arrayIndex < 0 )
        Com_Error(ERR_DROP, &byte_C75D58, pValue, weapVariantDef->szInternalName);
      weapDef->offhandClass = arrayIndex;
      goto LABEL_104;
    case 23:
      arrayIndex = Weapon_GetStringArrayIndex(pValue, offhandSlotNames, 5);
      if ( arrayIndex < 0 )
        Com_Error(ERR_DROP, &byte_C75D34, pValue, weapVariantDef->szInternalName);
      weapDef->offhandSlot = arrayIndex;
      goto LABEL_104;
    case 24:
      arrayIndex = Weapon_GetStringArrayIndex(pValue, (const char **)g_playerAnimTypeNames, g_playerAnimTypeNamesCount);
      if ( arrayIndex < 0 )
        Com_Error(ERR_DROP, &byte_C75D0C, pValue, weapVariantDef->szInternalName);
      weapDef->playerAnimType = arrayIndex;
      goto LABEL_104;
    case 25:
      arrayIndex = Weapon_GetStringArrayIndex(pValue, activeReticleNames, 3);
      if ( arrayIndex < 0 )
        Com_Error(ERR_DROP, &byte_C75CE0, pValue, weapVariantDef->szInternalName);
      weapDef->activeReticleType = arrayIndex;
      goto LABEL_104;
    case 26:
      arrayIndex = Weapon_GetStringArrayIndex(pValue, guidedMissileNames, 7);
      if ( arrayIndex < 0 )
        Com_Error(ERR_DROP, &byte_C75CB4, pValue, weapVariantDef->szInternalName);
      weapDef->guidedMissileType = arrayIndex;
      goto LABEL_104;
    case 27:
      weapDef->bounceSound = (const char **)BG_RegisterSurfaceTypeSounds(pValue);
      goto LABEL_104;
    case 28:
      arrayIndex = Weapon_GetStringArrayIndex(pValue, stickinessNames, 6);
      if ( arrayIndex < 0 )
        Com_Error(ERR_DROP, &byte_C75C88, pValue, weapVariantDef->szInternalName);
      weapDef->stickiness = arrayIndex;
      goto LABEL_104;
    case 29:
      arrayIndex = Weapon_GetStringArrayIndex(pValue, rotateTypeNames, 3);
      if ( arrayIndex < 0 )
        Com_Error(ERR_DROP, &byte_C75C5C, pValue, weapVariantDef->szInternalName);
      weapDef->rotateType = arrayIndex;
      goto LABEL_104;
    case 30:
      arrayIndex = Weapon_GetStringArrayIndex(pValue, overlayInterfaceNames, 3);
      if ( arrayIndex < 0 )
        Com_Error(ERR_DROP, &byte_C75C28, pValue, weapVariantDef->szInternalName);
      weapDef->overlayInterface = arrayIndex;
      goto LABEL_104;
    case 31:
      arrayIndex = Weapon_GetStringArrayIndex(pValue, szWeapInventoryTypeNames, 5);
      if ( arrayIndex < 0 )
        Com_Error(ERR_DROP, &byte_C75E70, pValue, weapVariantDef->szInternalName);
      weapDef->inventoryType = arrayIndex;
      goto LABEL_104;
    case 32:
      arrayIndex = Weapon_GetStringArrayIndex(pValue, szWeapFireTypeNames, 7);
      if ( arrayIndex < 0 )
        Com_Error(ERR_DROP, &byte_C75E48, pValue, weapVariantDef->szInternalName);
      weapDef->fireType = arrayIndex;
      goto LABEL_104;
    case 33:
      arrayIndex = Weapon_GetStringArrayIndex(pValue, szWeapClipTypeNames, 6);
      if ( arrayIndex < 0 )
        Com_Error(ERR_DROP, &byte_C75E20, pValue, weapVariantDef->szInternalName);
      weapDef->clipType = arrayIndex;
      goto LABEL_104;
    case 34:
      arrayIndex = Weapon_GetStringArrayIndex(pValue, ammoCounterClipNames, 7);
      if ( arrayIndex < 0 )
        Com_Error(ERR_DROP, &byte_C75BF8, pValue, weapVariantDef->szInternalName);
      weapDef->ammoCounterClip = arrayIndex;
      goto LABEL_104;
    case 35:
      arrayIndex = Weapon_GetStringArrayIndex(pValue, weapIconRatioNames, 3);
      if ( arrayIndex < 0 )
        Com_Error(ERR_DROP, &byte_C75BD0, pValue, weapVariantDef->szInternalName);
      weapDef->hudIconRatio = arrayIndex;
      goto LABEL_104;
    case 36:
      arrayIndex = Weapon_GetStringArrayIndex(pValue, weapIconRatioNames, 3);
      if ( arrayIndex < 0 )
        Com_Error(ERR_DROP, &byte_C75BD0, pValue, weapVariantDef->szInternalName);
      weapDef->ammoCounterIconRatio = arrayIndex;
      goto LABEL_104;
    case 37:
      arrayIndex = Weapon_GetStringArrayIndex(pValue, weapIconRatioNames, 3);
      if ( arrayIndex < 0 )
        Com_Error(ERR_DROP, &byte_C75BD0, pValue, weapVariantDef->szInternalName);
      weapDef->killIconRatio = arrayIndex;
      goto LABEL_104;
    case 38:
      arrayIndex = Weapon_GetStringArrayIndex(pValue, weapIconRatioNames, 3);
      if ( arrayIndex < 0 )
        Com_Error(ERR_DROP, &byte_C75BD0, pValue, weapVariantDef->szInternalName);
      weapVariantDef->dpadIconRatio = arrayIndex;
      goto LABEL_104;
    case 39:
      arrayIndex = Weapon_GetStringArrayIndex(pValue, weapIconRatioNames, 3);
      if ( arrayIndex < 0 )
        Com_Error(ERR_DROP, &byte_C75BD0, pValue, weapVariantDef->szInternalName);
      weapDef->indicatorIconRatio = arrayIndex;
      goto LABEL_104;
    case 40:
      numHideTags = 0;
      pos = pValue;
      while ( 1 )
      {
        token = (const char *)Com_Parse(&pos);
        if ( !pos )
          break;
        if ( numHideTags >= 32 )
          Com_Error(ERR_DROP, &byte_C75BA4, token, numHideTags, 32);
        StringOfSize = SL_GetStringOfSize(SCRIPTINSTANCE_SERVER, (char *)token, 0, strlen(token) + 1, 10);
        weapVariantDef->hideTags[numHideTags] = StringOfSize;
        weapVariantDef->hideTags[numHideTags] = SL_ConvertToLowercase(
                                                  SCRIPTINSTANCE_SERVER,
                                                  weapVariantDef->hideTags[numHideTags],
                                                  0,
                                                  10);
        ++numHideTags;
      }
      goto LABEL_104;
    case 41:
      pos = pValue;
      token = (const char *)Com_Parse(&pos);
      if ( pos )
      {
        v4 = SL_GetStringOfSize(SCRIPTINSTANCE_SERVER, (char *)token, 0, strlen(token) + 1, 10);
        weapDef->explosionTag = v4;
        weapDef->explosionTag = SL_ConvertToLowercase(SCRIPTINSTANCE_SERVER, weapDef->explosionTag, 0, 10);
      }
      goto LABEL_104;
    case 42:
      numNoteTrackMappings = 0;
      pos = pValue;
      keyName[0] = 0;
      while ( 1 )
      {
        token = (const char *)Com_Parse(&pos);
        if ( !pos )
          break;
        if ( numNoteTrackMappings >= 20 )
          Com_Error(ERR_DROP, &byte_C75B60, 20, token);
        if ( keyName[0] )
        {
          LowercaseString = SL_GetLowercaseString(keyName, 0, SCRIPTINSTANCE_SERVER);
          weapFullDef->notetrackSoundMapKeys[numNoteTrackMappings] = LowercaseString;
          v6 = SL_GetLowercaseString(token, 0, SCRIPTINSTANCE_SERVER);
          weapFullDef->notetrackSoundMapValues[numNoteTrackMappings++] = v6;
          keyName[0] = 0;
        }
        else
        {
          v11 = strlen(token);
          if ( v11 >= 63 )
            Com_Error(ERR_DROP, &byte_C75B1C, token, v11, 63);
          v10 = token;
          v9 = keyName;
          do
          {
            v8 = *v10;
            *v9++ = *v10++;
          }
          while ( v8 );
        }
      }
      if ( keyName[0] )
        Com_PrintWarning(
          0,
          "Notetrack-to-Sound: Weapon '%s' has bad entry; notetrack '%s' doesn't have a corresponding sound.\n",
          weapVariantDef->szInternalName,
          keyName);
LABEL_104:
      result = 1;
      break;
    default:
      Com_Error(ERR_DROP, &byte_C75A9C, iFieldType, weapVariantDef->szInternalName);
      result = 0;
      break;
  }
  return result;
}

int __cdecl Weapon_GetStringArrayIndex(const char *value, const char **stringArray, int arraySize)
{
  int arrayIndex; // [esp+0h] [ebp-4h]

  if ( !value
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_weapons_load_obj.cpp", 1561, 0, "%s", "value") )
  {
    __debugbreak();
  }
  if ( !stringArray
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_weapons_load_obj.cpp",
          1562,
          0,
          "%s",
          "stringArray") )
  {
    __debugbreak();
  }
  for ( arrayIndex = 0; arrayIndex < arraySize; ++arrayIndex )
  {
    if ( !I_stricmp(value, stringArray[arrayIndex]) )
      return arrayIndex;
  }
  return -1;
}

char **__cdecl BG_RegisterSurfaceTypeSounds(const char *surfaceSoundBase)
{
  const char *v2; // eax
  unsigned __int8 *v3; // eax
  char v4; // [esp+3h] [ebp-149h]
  char *v5; // [esp+8h] [ebp-144h]
  const char *v6; // [esp+Ch] [ebp-140h]
  char v7; // [esp+23h] [ebp-129h]
  _BYTE *v8; // [esp+28h] [ebp-124h]
  char *v9; // [esp+2Ch] [ebp-120h]
  unsigned __int8 *result; // [esp+40h] [ebp-10Ch]
  char aliasName[256]; // [esp+44h] [ebp-108h] BYREF
  int i; // [esp+148h] [ebp-4h]

  if ( !surfaceSoundBase
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_weapons_load_obj.cpp",
          1584,
          0,
          "%s",
          "surfaceSoundBase") )
  {
    __debugbreak();
  }
  if ( !*surfaceSoundBase )
    return 0;
  for ( i = 0; i < surfaceTypeSoundListCount; ++i )
  {
    if ( !I_strcmp(surfaceTypeSoundLists[i].surfaceSoundBase, surfaceSoundBase) )
      return (char **)dword_E8605C[2 * i];
  }
  if ( surfaceTypeSoundListCount == 16 )
    Com_Error(ERR_DROP, &byte_C75F5C, 16);
  result = Hunk_AllocLow(0x7Cu, "BG_RegisterSurfaceTypeSounds", 17);
  for ( i = 0; i < 31; ++i )
  {
    v2 = Com_SurfaceTypeToName(i);
    Com_sprintf(aliasName, 0x100u, "%s_%s", surfaceSoundBase, v2);
    v3 = Hunk_AllocLow(&aliasName[strlen(aliasName) + 1] - &aliasName[1] + 1, "BG_RegisterSurfaceTypeSounds", 17);
    *(unsigned int *)&result[4 * i] = v3;
    v9 = aliasName;
    v8 = *(_BYTE **)&result[4 * i];
    do
    {
      v7 = *v9;
      *v8++ = *v9++;
    }
    while ( v7 );
  }
  surfaceTypeSoundLists[surfaceTypeSoundListCount].surfaceSoundBase = (char *)Hunk_AllocLow(
                                                                                strlen(surfaceSoundBase) + 1,
                                                                                "BG_RegisterSurfaceTypeSounds",
                                                                                17);
  v6 = surfaceSoundBase;
  v5 = surfaceTypeSoundLists[surfaceTypeSoundListCount].surfaceSoundBase;
  do
  {
    v4 = *v6;
    *v5++ = *v6++;
  }
  while ( v4 );
  dword_E8605C[2 * surfaceTypeSoundListCount++] = (int)result;
  return (char **)result;
}

WeaponVariantDef *__cdecl BG_LoadWeaponVariantDefInternal(const char *folder, char *name)
{
  unsigned __int8 *weapFullDef; // [esp+8h] [ebp-Ch]
  WeaponDef *weapDef; // [esp+10h] [ebp-4h]

  weapFullDef = Hunk_AllocLow(0xC48u, "BG_LoadWeaponVariantDefInternal", 10);
  InitWeaponDef((WeaponFullDef *)weapFullDef);
  weapDef = (WeaponDef *)(weapFullDef + 228);
  if ( !BG_LoadWeaponVariantDefFile((WeaponFullDef *)weapFullDef, folder, name) )
    return 0;
  *((unsigned int *)weapFullDef + 565) = 0;
  *((unsigned int *)weapFullDef + 566) = 0;
  bg_nextParseFlameTableIndex = 0;
  if ( **((_BYTE **)weapFullDef + 563) )
    *((unsigned int *)weapFullDef + 565) = BG_GetFlameTable(folder, *((char **)weapFullDef + 563));
  if ( **((_BYTE **)weapFullDef + 564) )
    *((unsigned int *)weapFullDef + 566) = BG_GetFlameTable(folder, *((char **)weapFullDef + 564));
  if ( I_stricmp(name, "defaultweapon_mp") )
  {
    if ( !*((unsigned int *)weapFullDef + 159) )
      *((unsigned int *)weapFullDef + 159) = *((unsigned int *)weapFullDef + 157);
    if ( !*((unsigned int *)weapFullDef + 160) )
      *((unsigned int *)weapFullDef + 160) = *((unsigned int *)weapFullDef + 158);
    if ( !*((unsigned int *)weapFullDef + 141) )
      *((unsigned int *)weapFullDef + 141) = "wpn_default_raise";
    if ( !*((unsigned int *)weapFullDef + 145) )
      *((unsigned int *)weapFullDef + 145) = "wpn_default_putaway";
    if ( !*((unsigned int *)weapFullDef + 88) )
      *((unsigned int *)weapFullDef + 88) = "wpn_default_pickup";
    if ( !*((unsigned int *)weapFullDef + 90) )
      *((unsigned int *)weapFullDef + 90) = "wpn_default_ammo_pickup";
    if ( !*((unsigned int *)weapFullDef + 105) )
      *((unsigned int *)weapFullDef + 105) = "wpn_default_no_ammo";
  }
  BG_SetupTransitionTimes((WeaponVariantDef *)weapFullDef);
  BG_CheckWeaponDamageRanges(weapDef);
  if ( *((float *)weapFullDef + 353) > 15000.0 )
    Com_Error(ERR_DROP, "Enemy crosshair ranges should be less than %f ", 15000.0);
  if ( *((unsigned int *)weapFullDef + 64) == 2 )
    BG_CheckProjectileValues((WeaponFullDef *)weapFullDef);
  if ( !G_ParseWeaponAccurayGraphs(weapDef) )
    return 0;
  if ( weapFullDef[1584] )
  {
    I_strlwr(*((char **)weapFullDef + 16));
    I_strlwr(*((char **)weapFullDef + 18));
  }
  else
  {
    *((unsigned int *)weapFullDef + 16) = &toastPopupTitle;
    *((unsigned int *)weapFullDef + 18) = &toastPopupTitle;
  }
  BLOPS_NULLSUB();
  return (WeaponVariantDef *)weapFullDef;
}

void __cdecl BG_SetupTransitionTimes(WeaponVariantDef *weapVariantDef)
{
  if ( weapVariantDef->iAdsTransInTime <= 0 )
    weapVariantDef->fOOPosAnimLength[0] = 1.0 / 300.0;
  else
    weapVariantDef->fOOPosAnimLength[0] = 1.0 / (float)weapVariantDef->iAdsTransInTime;
  if ( weapVariantDef->iAdsTransOutTime <= 0 )
    weapVariantDef->fOOPosAnimLength[1] = 1.0 / 500.0;
  else
    weapVariantDef->fOOPosAnimLength[1] = 1.0 / (float)weapVariantDef->iAdsTransOutTime;
}

void __cdecl BG_CheckWeaponDamageRanges(WeaponDef *weapDef)
{
  if ( weapDef->fMaxDamageRange <= 0.0 )
    weapDef->fMaxDamageRange = 999999.0f;
  if ( weapDef->fMinDamageRange <= 0.0 )
    weapDef->fMinDamageRange = 999999.12f;
}

void __cdecl BG_CheckProjectileValues(WeaponFullDef *weaponFullDef)
{
  if ( weaponFullDef->weapDef.weapType != WEAPTYPE_PROJECTILE
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\bgame\\bg_weapons_load_obj.cpp",
          1941,
          0,
          "%s",
          "weaponDef->weapType == WEAPTYPE_PROJECTILE") )
  {
    __debugbreak();
  }
  if ( (float)weaponFullDef->weapDef.iProjectileSpeed <= 0.0 )
    Com_Error(
      ERR_DROP,
      "Projectile speed for WeapType %s must be greater than 0.0",
      weaponFullDef->weapVariantDef.szDisplayName);
  if ( weaponFullDef->weapDef.destabilizationCurvatureMax >= 1000000000.0
    || weaponFullDef->weapDef.destabilizationCurvatureMax < 0.0 )
  {
    Com_Error(
      ERR_DROP,
      "Destabilization angle for for WeapType %s must be between 0 and 45 degrees",
      weaponFullDef->weapVariantDef.szDisplayName);
  }
  if ( weaponFullDef->weapDef.destabilizationRateTime < 0.0 )
    Com_Error(
      ERR_DROP,
      "Destabilization rate time for for WeapType %s must be non-negative",
      weaponFullDef->weapVariantDef.szDisplayName);
}

void __cdecl InitWeaponDef(WeaponFullDef *weapFullDef)
{
  const cspField_t *pField; // [esp+8h] [ebp-Ch]
  int iField; // [esp+Ch] [ebp-8h]

  weapFullDef->weapVariantDef.weapDef = &weapFullDef->weapDef;
  weapFullDef->weapVariantDef.szInternalName = &toastPopupTitle;
  weapFullDef->weapVariantDef.szXAnims = weapFullDef->szXAnims;
  weapFullDef->weapVariantDef.hideTags = weapFullDef->hideTags;
  weapFullDef->weapDef.gunXModel = weapFullDef->gunXModel;
  weapFullDef->weapDef.notetrackSoundMapKeys = weapFullDef->notetrackSoundMapKeys;
  weapFullDef->weapDef.notetrackSoundMapValues = weapFullDef->notetrackSoundMapValues;
  weapFullDef->weapDef.worldModel = weapFullDef->worldModel;
  weapFullDef->weapDef.parallelBounce = weapFullDef->parallelBounce;
  weapFullDef->weapDef.perpendicularBounce = weapFullDef->perpendicularBounce;
  weapFullDef->weapDef.locationDamageMultipliers = weapFullDef->locationDamageMultipliers;
  iField = 0;
  pField = weaponDefFields;
  while ( iField < 748 )
  {
    if ( !pField->iFieldType )
      *(const char **)((char *)&weapFullDef->weapVariantDef.szInternalName + pField->iOffset) = &toastPopupTitle;
    ++iField;
    ++pField;
  }
}

char __cdecl BG_WeaponUpdateField(const char *weaponName, char *keyValue)
{
  WeaponFullDef weapFullDef; // [esp+8h] [ebp-C58h] BYREF
  WeaponVariantDef *weapVariantDef; // [esp+C58h] [ebp-8h]
  int w; // [esp+C5Ch] [ebp-4h]

  w = BG_FindWeaponIndexForName(weaponName);
  if ( w )
  {
    weapVariantDef = (WeaponVariantDef *)BG_GetWeaponVariantDef(w);
    memcpy(&weapFullDef, weapVariantDef, 0xE4u);
    memcpy(&weapFullDef.weapDef, weapVariantDef->weapDef, sizeof(weapFullDef.weapDef));
    if ( !ParseConfigStringToStruct(
            (unsigned __int8 *)&weapFullDef,
            weaponDefFields,
            748,
            keyValue,
            43,
            (int (__cdecl *)(unsigned __int8 *, const char *, const int, const int))BG_ParseWeaponDefSpecificFieldType,
            SetConfigString2) )
      return 0;
    memcpy(weapVariantDef, &weapFullDef, sizeof(WeaponVariantDef));
    memcpy(weapVariantDef->weapDef, &weapFullDef.weapDef, sizeof(WeaponDef));
    BG_SetupTransitionTimes(weapVariantDef);
    BG_CheckWeaponDamageRanges(weapVariantDef->weapDef);
  }
  return 1;
}

bool __cdecl BG_FlameTableUpdateField(const char *flameTableName, char *keyValue)
{
  flameTable *ft; // [esp+0h] [ebp-4h]

  ft = Flame_FindFlameTable(flameTableName);
  return !ft
      || ParseConfigStringToStruct((unsigned __int8 *)ft, flameTableFields, 119, keyValue, 0, 0, SetConfigString2);
}

WeaponVariantDef *__cdecl BG_LoadDefaultWeaponVariantDef()
{
  if ( useFastFile->current.enabled )
    return (WeaponVariantDef *)((int (__cdecl *)(WeaponVariantDef *(__cdecl *)()))BG_LoadDefaultWeaponVariantDef_FastFile)(BG_LoadDefaultWeaponVariantDef_FastFile);
  else
    return (WeaponVariantDef *)((int (__cdecl *)(WeaponFullDef *(__cdecl *)()))BG_LoadDefaultWeaponVariantDef_LoadObj)(BG_LoadDefaultWeaponVariantDef_LoadObj);
}

WeaponFullDef *__cdecl BG_LoadDefaultWeaponVariantDef_LoadObj()
{
  InitWeaponDef(&bg_defaultWeaponFullDefs);
  bg_defaultWeaponFullDefs.weapVariantDef.weapDef = &bg_defaultWeaponFullDefs.weapDef;
  bg_defaultWeaponFullDefs.weapVariantDef.weapDef->playerAnimType = Weapon_GetStringArrayIndex(
                                                                      "default",
                                                                      (const char **)g_playerAnimTypeNames,
                                                                      g_playerAnimTypeNamesCount);
  if ( bg_defaultWeaponFullDefs.weapVariantDef.weapDef->playerAnimType < 0 )
    bg_defaultWeaponFullDefs.weapVariantDef.weapDef->playerAnimType = 0;
  bg_defaultWeaponFullDefs.weapVariantDef.szInternalName = "none";
  bg_defaultWeaponFullDefs.weapDef.accuracyGraphName[0] = "noweapon.accu";
  bg_defaultWeaponFullDefs.weapDef.accuracyGraphName[1] = "noweapon.accu";
  bg_defaultWeaponFullDefs.weapDef.sprintDurationScale = 1.75f;
  G_ParseWeaponAccurayGraphs(&bg_defaultWeaponFullDefs.weapDef);
  return &bg_defaultWeaponFullDefs;
}

WeaponVariantDef *__cdecl BG_LoadDefaultWeaponVariantDef_FastFile()
{
  return DB_FindXAssetHeader(ASSET_TYPE_WEAPON, "none", 1, -1).weapon;
}

