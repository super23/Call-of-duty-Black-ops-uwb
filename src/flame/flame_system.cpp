#include "flame_system.h"

void __cdecl Flame_Init_FlameVars()
{
  int i; // [esp+14h] [ebp-4h]

  memset((unsigned __int8 *)flameVarList, 0, sizeof(flameVarList));
  numFlameVars = 0;
  for ( i = 0; i < bg_iNumFlameTableFields; ++i )
  {
    if ( flameVars[i].dvarName )
      flameDvars[i] = _Dvar_RegisterFloat(
                        flameVars[i].dvarName,
                        *((float *)&unk_E12B68 + 6 * i),
                        *((float *)&unk_E12B6C + 6 * i),
                        *((float *)&unk_E12B70 + 6 * i),
                        0,
                        off_E12B74[6 * i]);
  }
}

void __cdecl Flame_Reset_FlameVars()
{
  memset((unsigned __int8 *)flameVarList, 0, sizeof(flameVarList));
  numFlameVars = 0;
}

void __cdecl Flame_Init_Classes()
{
  flame_freeze_id = 0;
  flame_spawn_id = 0;
  Flame_Class_Chunk_Init();
  Flame_Class_Fire_Init();
  Flame_Class_Smoke_Init();
  Flame_Class_Stream_Init();
  Flame_Class_Drips_Init();
}

void __cdecl Flame_Init_Sources()
{
  int i; // [esp+0h] [ebp-4h]

  Flame_Init_DVars();
  memset((unsigned __int8 *)flameSources, 0, sizeof(flameSources));
  memset(flameSourceLookup, 0, 0x3FEu);
  memset((unsigned __int8 *)sv_flameSources, 0, sizeof(sv_flameSources));
  memset(sv_flameSourceLookup, 0, 0x3FEu);
  for ( i = 0; i < 64; ++i )
    sv_flameSources[i].is_server_alloc = 1;
}

void __cdecl Flame_Init_DVars()
{
  if ( !flame_test )
  {
    flame_test = _Dvar_RegisterBool("flame_test", 0, 0x4080u, "Turn on/off Flamethrower testing");
    flame_use_dvars = _Dvar_RegisterBool(
                        "flame_use_dvars",
                        0,
                        0x4080u,
                        "Turn on/off use of dvars for flamethrower behavior");
    flame_render = _Dvar_RegisterBool("flame_render", 1, 0x4080u, "Turn on/off Flamethrower rendering");
    flame_team_damage = _Dvar_RegisterBool(
                          "flame_team_damage",
                          0,
                          0x4080u,
                          "Turn on/off Flamethrower damage to friendlies");
    flame_debug_render = _Dvar_RegisterInt(
                           "flame_debug_render",
                           0,
                           0,
                           2,
                           0x4080u,
                           "Turn on/off Flamethrower debug rendering");
    flame_config_valid = _Dvar_RegisterBool("flame_config_valid", 0, 0x4000u, "Internal use only");
    default_flameVars_initialHitDamage = _Dvar_RegisterFloat(
                                           "defaultHitDamage",
                                           5.0,
                                           1.0,
                                           200.0,
                                           0,
                                           "default initial damage delivered by getting hit by a flame ball");
    default_flameVars_timedDamageDuration = _Dvar_RegisterFloat(
                                              "defaultDamageDuration",
                                              300.0,
                                              0.0,
                                              10000.0,
                                              0,
                                              "default duration in milliseconds that damage should continue to get delivered");
    default_flameVars_timedDamageInterval = _Dvar_RegisterFloat(
                                              "defaultDamageInterval",
                                              300.0,
                                              0.0,
                                              10000.0,
                                              0,
                                              "default interval in milliseconds that damage should continue to get delivered");
    flameVar_editingFlameTable = _Dvar_RegisterString(
                                   "flameVar_editingFlameTable",
                                   "none",
                                   0,
                                   "This is the most recent flametable that has been put up for editing, mostly included to help artists");
    flameVar_lastFlameTable = _Dvar_RegisterString(
                                "flameVar_lastFlameTable",
                                "none",
                                0,
                                "This is the previous flametable that was put up for editing, mostly included to help artists");
    flame_kick_offset = _Dvar_RegisterVec3(
                          "flame_kick_offset",
                          COERCE_UNSIGNED_INT(-2.0),
                          COERCE_UNSIGNED_INT(0.0),
                          COERCE_UNSIGNED_INT(-0.5),
                          -3.4028235e38,
                          3.4028235e38,
                          0x80u,
                          "Origin offset for flamethrower view model while firing.");
    flame_kick_speed = _Dvar_RegisterFloat(
                         "flame_kick_speed",
                         4.0,
                         0.0,
                         3.4028235e38,
                         0x80u,
                         "Speed at which the flamethrower view model transitions to flame_kick_offset.");
    flame_kick_recover_speed = _Dvar_RegisterFloat(
                                 "flame_kick_recover_speed",
                                 4.0,
                                 0.0,
                                 3.4028235e38,
                                 0x80u,
                                 "Speed at which the flamethrower view model transitions back from flame_kick_offset.");
  }
}

char __cdecl Flame_TableInDevGui(const char *tableName)
{
  int i; // [esp+0h] [ebp-4h]

  for ( i = 0; i < numFlameTablesLoaded; ++i )
  {
    if ( !I_strcmp(tableName, loadedFlameTables[i]) )
      return 1;
  }
  return 0;
}

void __cdecl Flame_InitDevGui()
{
  char *v0; // eax
  char *v1; // eax
  unsigned int j; // [esp+0h] [ebp-Ch]
  int i; // [esp+4h] [ebp-8h]
  int ia; // [esp+4h] [ebp-8h]
  const WeaponDef *weapDef; // [esp+8h] [ebp-4h]

  numFlameTablesLoaded = 0;
  for ( j = 0; j < BG_GetNumWeapons(); ++j )
  {
    weapDef = BG_GetWeaponDef(j);
    if ( weapDef && weapDef->flameTableFirstPersonPtr && !Flame_TableInDevGui(weapDef->flameTableFirstPerson) )
    {
      if ( numFlameTablesLoaded >= 8
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\flame\\flame_system.cpp",
              333,
              0,
              "%s\n\t(numFlameTablesLoaded) = %i",
              "(numFlameTablesLoaded < 8)",
              numFlameTablesLoaded) )
      {
        __debugbreak();
      }
      loadedFlameTables[numFlameTablesLoaded] = weapDef->flameTableFirstPerson;
      if ( !numFlameTablesLoaded )
      {
        Dvar_SetString((dvar_s *)flameVar_editingFlameTable, loadedFlameTables[0]);
        Flame_DVarsToFlameVars(weapDef->flameTableFirstPersonPtr);
      }
      ++numFlameTablesLoaded;
    }
    if ( weapDef && weapDef->flameTableThirdPersonPtr && !Flame_TableInDevGui(weapDef->flameTableThirdPerson) )
    {
      if ( numFlameTablesLoaded >= 8
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\flame\\flame_system.cpp",
              347,
              0,
              "%s\n\t(numFlameTablesLoaded) = %i",
              "(numFlameTablesLoaded < 8)",
              numFlameTablesLoaded) )
      {
        __debugbreak();
      }
      loadedFlameTables[numFlameTablesLoaded] = weapDef->flameTableThirdPerson;
      if ( !numFlameTablesLoaded )
      {
        Dvar_SetString((dvar_s *)flameVar_editingFlameTable, loadedFlameTables[0]);
        Flame_DVarsToFlameVars(weapDef->flameTableThirdPersonPtr);
      }
      ++numFlameTablesLoaded;
    }
  }
  Cmd_AddCommandInternal("flame_printdvars", Flame_CMD_PrintDVarsToConsol, &Flame_CMD_PrintDVarsToConsol_VAR);
  Cmd_AddCommandInternal("flame_test_toggle", Flame_CMD_Test_Toggle, &Flame_CMD_Test_Toggle_VAR);
  Cmd_AddCommandInternal("flame_use_dvars_toggle", Flame_CMD_Use_Dvars_Toggle, &Flame_CMD_Use_Dvars_Toggle_VAR);
  Cbuf_InsertText(0, "devgui_cmd \"FT/Toggle Flamethrower:1\" flame_test_toggle");
  Cbuf_InsertText(0, "devgui_cmd \"FT/Toggle Dvar Use:2\" flame_use_dvars_toggle");
  for ( i = 0; i < numFlameTablesLoaded; ++i )
  {
    v0 = va(
           "devgui_cmd \"FT/Flametables:3/%s:%i\" \"set flameVar_editingFlameTable %s\"",
           loadedFlameTables[i],
           i + 1,
           loadedFlameTables[i]);
    Cbuf_InsertText(0, v0);
  }
  for ( ia = 0; ia < 128; ++ia )
  {
    if ( (&off_E12B64)[6 * ia] )
    {
      v1 = va("devgui_dvar \"FT/Edit Dvars:4/%s:%i\" %s", (&off_E12B64)[6 * ia], ia + 1, flameVars[ia].dvarName);
      Cbuf_InsertText(0, v1);
    }
  }
  Cbuf_InsertText(0, "devgui_cmd \"FT/Print DVars:5\" flame_printdvars");
}

void __cdecl Flame_Init()
{
  Flame_Init_Classes();
  Flame_Init_Sources();
}

void __cdecl Flame_Reset()
{
  Flame_Init_Classes();
  Flame_Init_Sources();
  Flame_Reset_FlameVars();
}

void __cdecl Flame_CMD_PrintDVarsToConsol()
{
  int i; // [esp+8h] [ebp-4h]

  Com_Printf(0, "\n------------------------------------------\nDumping Flamethrower Dvars...\n\n[BEGIN]\n");
  Com_Printf(
    0,
    "These are the float values for the flametable %s. BE CAREFUL NOT TO CLOBBER THE MATERIAL AND SOUND NAMES IN THE WEAP"
    "ONSETTINGS.GDT!\n"
    "\n",
    flameVar_editingFlameTable->current.string);
  for ( i = 0; i < bg_iNumFlameTableFields; ++i )
  {
    if ( flameVars[i].dvarName )
      Com_Printf(0, "    \"%s\" \"%f\"\n", flameVars[i].dvarName, flameDvars[i]->current.value);
  }
  Com_Printf(0, "[END]\n\nDone.\n------------------------------------------\n");
}

void __cdecl Flame_CMD_Test_Toggle()
{
  if ( flame_test->current.enabled )
    Cbuf_InsertText(0, "flame_test 0");
  else
    Cbuf_InsertText(0, "flame_test 1");
}

void __cdecl Flame_CMD_Use_Dvars_Toggle()
{
  if ( flame_use_dvars->current.enabled )
    Cbuf_InsertText(0, "flame_use_dvars 0");
  else
    Cbuf_InsertText(0, "flame_use_dvars 1");
}

void __cdecl Flame_DVarsToFlameVars(flameTable *fTable)
{
  const cspField_t *field; // [esp+4h] [ebp-10h]
  const cspField_t *fielda; // [esp+4h] [ebp-10h]
  float flt; // [esp+8h] [ebp-Ch]
  flameTable *fltptr; // [esp+Ch] [ebp-8h]
  flameTable *fltptra; // [esp+Ch] [ebp-8h]
  int i; // [esp+10h] [ebp-4h]
  int ia; // [esp+10h] [ebp-4h]

  if ( !fTable
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\flame\\flame_system.cpp", 467, 0, "%s", "fTable") )
  {
    __debugbreak();
  }
  if ( !fTable->name
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\flame\\flame_system.cpp", 468, 0, "%s", "fTable->name") )
  {
    __debugbreak();
  }
  if ( I_strcmp(flameVar_editingFlameTable->current.string, flameVar_lastFlameTable->current.string) )
  {
    i = 0;
    fltptr = fTable;
    field = flameTableFields;
    while ( i < bg_iNumFlameTableFields )
    {
      if ( field->iFieldType == 7 )
        Dvar_SetFloatByName(field->szName, fltptr->flameVar_streamChunkGravityStart);
      ++field;
      ++i;
      fltptr = (flameTable *)((char *)fltptr + 4);
    }
    Dvar_SetString((dvar_s *)flameVar_lastFlameTable, flameVar_editingFlameTable->current.string);
  }
  ia = 0;
  fielda = flameTableFields;
  fltptra = fTable;
  while ( ia < bg_iNumFlameTableFields )
  {
    if ( fielda->iFieldType == 7 )
    {
      flt = Dvar_GetFloat(fielda->szName);
      fltptra->flameVar_streamChunkGravityStart = flt;
    }
    ++fielda;
    ++ia;
    fltptra = (flameTable *)((char *)fltptra + 4);
  }
}

flameTable *__cdecl Flame_FindFlameTable(const char *tableName)
{
  unsigned int i; // [esp+0h] [ebp-Ch]
  unsigned int numWeapons; // [esp+4h] [ebp-8h]
  const WeaponDef *weapDef; // [esp+8h] [ebp-4h]

  numWeapons = BG_GetNumWeapons();
  for ( i = 0; i < numWeapons; ++i )
  {
    weapDef = BG_GetWeaponDef(i);
    if ( weapDef && weapDef->flameTableFirstPersonPtr && !I_strcmp(weapDef->flameTableFirstPerson, tableName) )
      return weapDef->flameTableFirstPersonPtr;
    if ( weapDef && weapDef->flameTableThirdPersonPtr && !I_strcmp(weapDef->flameTableThirdPerson, tableName) )
      return weapDef->flameTableThirdPersonPtr;
  }
  return 0;
}

double __cdecl Flame_SwayRand(float x, float y, int time)
{
  long double v4; // [esp+0h] [ebp-18h]
  float v5; // [esp+0h] [ebp-18h]
  float v6; // [esp+4h] [ebp-14h]
  float v7; // [esp+8h] [ebp-10h]

  v7 = fmod((double)time * x * 3.1415927 + (double)time * x * 3.1415927, 6.2831855);
  __libm_sse2_sin(v4);
  v6 = fmod((double)time * y * 3.1415927 + (double)time * y * 3.1415927, 6.2831855);
  v5 = cos(v6);
  return v5 * v7;
}

flameSource_t *__cdecl SV_Flame_Source_Get(int entityNum)
{
  if ( sv_flameSourceLookup[entityNum] )
    return &sv_flameSources[sv_flameSourceLookup[entityNum] - 1];
  else
    return 0;
}

flameSource_t *__cdecl Flame_Source_Get(int entityNum)
{
  if ( flameSourceLookup[entityNum] )
    return &flameSources[flameSourceLookup[entityNum] - 1];
  else
    return 0;
}

flameRender_s *__cdecl Flame_Get_FlameRender(const char *name)
{
  char v1; // cl
  flameRender_s *v3; // [esp+8h] [ebp-38h]
  const char *v4; // [esp+Ch] [ebp-34h]
  bool is_found; // [esp+37h] [ebp-9h]
  flameRender_s *trav; // [esp+38h] [ebp-8h]
  int i; // [esp+3Ch] [ebp-4h]

  if ( strlen(name) >= 0x7F
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\flame\\flame_system.cpp",
          579,
          0,
          "%s\n\t%s",
          "strlen( name ) < (MAX_FLAMEVAR_NAME-1)",
          "Flame config name is too long") )
  {
    __debugbreak();
  }
  is_found = 0;
  trav = flameVarList;
  for ( i = 0; i < numFlameVars; ++i )
  {
    if ( !strcmp(name, trav->name) )
    {
      is_found = 1;
      break;
    }
    ++trav;
  }
  if ( !is_found )
  {
    if ( numFlameVars >= 15
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\flame\\flame_system.cpp",
            594,
            0,
            "%s",
            "numFlameVars < MAX_FLAMEVARLIST-1") )
    {
      __debugbreak();
    }
    trav = &flameVarList[numFlameVars++];
    v4 = name;
    v3 = trav;
    do
    {
      v1 = *v4;
      v3->name[0] = *v4++;
      v3 = (flameRender_s *)((char *)v3 + 1);
    }
    while ( v1 );
  }
  return trav;
}

void __cdecl Flame_Item_Init(flameGeneric_s *item, unsigned int itemSize)
{
  flameList_t globalList; // [esp+0h] [ebp-10h]
  flameList_t localList; // [esp+8h] [ebp-8h]

  globalList = item->listGlobal;
  localList = item->listLocal;
  memset((unsigned __int8 *)item, 0, itemSize);
  item->listGlobal = globalList;
  item->listLocal = localList;
}

void __cdecl Flame_List_Init(flameGeneric_s *list, int itemSize, int listSize)
{
  int i; // [esp+4h] [ebp-8h]

  for ( i = 0; i < listSize; ++i )
  {
    list->listGlobal.prev = (flameGeneric_s *)((char *)list - itemSize);
    if ( !i )
      list->listGlobal.prev = 0;
    list->listGlobal.next = (flameGeneric_s *)((char *)list + itemSize);
    if ( i >= listSize - 1 )
      list->listGlobal.next = 0;
    list = (flameGeneric_s *)((char *)list + itemSize);
  }
}

void __cdecl Flame_List_Move_Global(flameGeneric_s *item, flameGeneric_s **fromList, flameGeneric_s **toList)
{
  if ( *fromList )
  {
    if ( item == *fromList )
      *fromList = item->listGlobal.next;
    if ( item->listGlobal.prev )
      item->listGlobal.prev->listGlobal.next = item->listGlobal.next;
    if ( item->listGlobal.next )
      item->listGlobal.next->listGlobal.prev = item->listGlobal.prev;
    item->listGlobal.next = 0;
    item->listGlobal.prev = 0;
    if ( *toList )
    {
      item->listGlobal.next = *toList;
      (*toList)->listGlobal.prev = item;
    }
    *toList = item;
  }
}

void __cdecl Flame_List_Add_Local(flameGeneric_s *item, flameGeneric_s **localList)
{
  item->listLocal.prev = 0;
  item->listLocal.next = *localList;
  if ( *localList )
    (*localList)->listLocal.prev = item;
  *localList = item;
}

void __cdecl Flame_List_Sub_Local(flameGeneric_s *item, flameGeneric_s **localList)
{
  if ( *localList == item )
    *localList = item->listLocal.next;
  if ( item->listLocal.prev )
    item->listLocal.prev->listLocal.next = item->listLocal.next;
  if ( item->listLocal.next )
    item->listLocal.next->listLocal.prev = item->listLocal.prev;
  item->listLocal.prev = 0;
  item->listLocal.next = 0;
}

void __cdecl CG_Flame_Age_All_Objects(int time)
{
  //PIXBeginNamedEvent(-1, "CG_Flame_Age_All_Objects");
  cl_flames.m_alloc_count = 0;
  Flame_Class_Chunk_Age(0, time);
  Flame_Class_Fire_Age(0, time);
  Flame_Class_Smoke_Age(0, time);
  Flame_Class_Stream_Age(0);
  Flame_Class_Drips_Age(0, time);
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

void __cdecl SV_Flame_Age_All_Objects(int time)
{
  //PIXBeginNamedEvent(-1, "SV_Flame_Age_All_Objects");
  sv_flames.m_alloc_count = 0;
  Flame_Class_Chunk_Age(1, time);
  Flame_Class_Fire_Age(1, time);
  Flame_Class_Smoke_Age(1, time);
  Flame_Class_Stream_Age(1);
  Flame_Class_Drips_Age(1, time);
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
}

flameSource_t *__cdecl Flame_Source_Alloc(int entityNum)
{
  flameSource_t *source; // [esp+0h] [ebp-8h]
  unsigned __int8 i; // [esp+7h] [ebp-1h]

  i = 0;
  source = flameSources;
  while ( i < 0x40u )
  {
    if ( !source->inUse )
    {
      source->entityNum = entityNum;
      source->inUse = 1;
      flameSourceLookup[entityNum] = i + 1;
      Flame_Class_Source_Sound_Init(source);
      return source;
    }
    ++i;
    ++source;
  }
  return 0;
}

flameSource_t *__cdecl SV_Flame_Source_Alloc(int entityNum)
{
  flameSource_t *v2; // [esp+0h] [ebp-8h]
  unsigned __int8 i; // [esp+7h] [ebp-1h]

  i = 0;
  v2 = sv_flameSources;
  while ( i < 0x40u )
  {
    if ( !v2->inUse )
    {
      v2->entityNum = entityNum;
      v2->inUse = 1;
      sv_flameSourceLookup[entityNum] = i + 1;
      return v2;
    }
    ++i;
    ++v2;
  }
  return 0;
}

void __cdecl Flame_Render_Sprite(
        cg_s *clientGlobals,
        Material *material,
        flameGeneric_s *flameGeneric,
        float alpha,
        float frame,
        unsigned __int8 col)
{
  float angles[3]; // [esp+Ch] [ebp-10h] BYREF
  unsigned __int8 rgbaColor[4]; // [esp+18h] [ebp-4h] BYREF

  rgbaColor[0] = col;
  rgbaColor[1] = col;
  rgbaColor[2] = col;
  rgbaColor[3] = (int)(alpha * 255.0);
  AxisToAngles(clientGlobals->refdef.viewaxis, angles);
  angles[2] = flameGeneric->phys.rotation;
  if ( frame < 0.0 )
    frame = (float)(flameGeneric->age.lastUpdateTime - flameGeneric->age.startTime)
          / (float)(flameGeneric->age.endTime - flameGeneric->age.startTime);
  R_BeginCodeMeshVerts();
  R_GenerateQuadStampCodeMeshVerts(
    material,
    angles,
    flameGeneric->phys.origin,
    flameGeneric->size.current,
    rgbaColor,
    frame);
  R_EndCodeMeshVerts();
}

void  Flame_Render_Sprites(
        float a1@<ebp>,
        cg_s *clientGlobals,
        Material *material,
        flameGeneric_s **flameGenericList,
        int numItems)
{
  void *v5; // esp
  float *v6; // [esp-4048h] [ebp-4054h]
  renderQuad_t *v7; // [esp-4044h] [ebp-4050h]
  renderQuad_t v8[512]; // [esp-4040h] [ebp-404Ch] BYREF
  int v9; // [esp-34h] [ebp-40h]
  int v10; // [esp-30h] [ebp-3Ch]
  float **v11; // [esp-2Ch] [ebp-38h]
  float v12[13]; // [esp-28h] [ebp-34h] BYREF
  float retaddr; // [esp+Ch] [ebp+0h]

  v12[10] = a1;
  v12[11] = retaddr;
  v5 = alloca(16464);
  LODWORD(v12[9]) = 512;
  AxisCopy(clientGlobals->refdef.viewaxis, (float (*)[3])v12);
  v11 = (float **)&flameGenericList[numItems - 1];
  R_BeginCodeMeshVerts();
  v10 = 0;
  do
  {
    v9 = 0;
    while ( v10 < numItems && v9 < 512 )
    {
      v7 = &v8[v9];
      v6 = *v11;
      v7->pos[0] = **v11;
      v7->pos[1] = v6[1];
      v7->pos[2] = v6[2];
      v8[v9].rotation = (*v11)[11];
      v8[v9].radius = (*v11)[13];
      v8[v9++].lifeFrac = (float)(*((unsigned int *)*v11 + 15) - *((unsigned int *)*v11 + 16))
                        / (float)(*((unsigned int *)*v11 + 17) - *((unsigned int *)*v11 + 16));
      --v11;
      ++v10;
    }
    R_GenerateQuadStampCodeMeshVertsArray(material, v8, v9, (float (*)[3])v12);
  }
  while ( v10 < numItems );
  R_EndCodeMeshVerts();
}

int __cdecl Flame_GetLocalClientFlameSource(int localClientNum, int EntNum)
{
  return EntNum;
}

unsigned __int8 __cdecl Flame_GetLocalClientSourceRange()
{
  return 1;
}

void __cdecl Flame_Source_Update(flameSource_t *source, flameWeaponConfig_t *weaponConfig)
{
  source->bIsFiring = weaponConfig->bIsFiring;
}

void __cdecl SV_Flame_Update_Source_Internal(
        flameSource_t *source,
        int entityNum,
        flameWeaponConfig_t *weaponConfig,
        int time)
{
  if ( weaponConfig->bIsFiring != source->bIsFiring )
    source->currentStream = 0;
  if ( weaponConfig->bIsFiring || weaponConfig->bFireWhileIdle )
    Flame_Class_Stream_Fire_Chunks(source, weaponConfig, time, 0);
  Flame_Source_Update(source, weaponConfig);
}

void __cdecl CG_Flame_Update_Source_Internal(
        flameSource_t *source,
        int entityNum,
        flameWeaponConfig_t *weaponConfig,
        int time,
        flameRender_s *flameRend)
{
  if ( weaponConfig->bIsFiring != source->bIsFiring )
    source->currentStream = 0;
  if ( weaponConfig->bIsFiring || weaponConfig->bFireWhileIdle )
    Flame_Class_Stream_Fire_Chunks(source, weaponConfig, time, flameRend);
  Flame_Source_Update(source, weaponConfig);
}

void __cdecl SV_Flame_Update_Source(int entityNum, flameWeaponConfig_t *weaponConfig, int time)
{
  flameSource_t *source; // [esp+0h] [ebp-4h]

  source = SV_Flame_Source_Get(entityNum);
  if ( !source )
  {
    source = SV_Flame_Source_Alloc(entityNum);
    if ( !source
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\flame\\flame_system.cpp",
            1003,
            0,
            "%s\n\t%s",
            "source",
            "No free flameSources") )
    {
      __debugbreak();
    }
  }
  SV_Flame_Update_Source_Internal(source, entityNum, weaponConfig, time);
}

void __cdecl CG_Flame_Update_Source(int localClientNum)
{
  int LocalClientFlameSource; // eax
  int v2; // eax
  int v3; // eax
  int v4; // [esp+8h] [ebp-20Ch]
  int time; // [esp+Ch] [ebp-208h]
  float dir[3]; // [esp+34h] [ebp-1E0h] BYREF
  float start[3]; // [esp+40h] [ebp-1D4h] BYREF
  trace_t trace; // [esp+4Ch] [ebp-1C8h] BYREF
  flameSource_t *source; // [esp+88h] [ebp-18Ch]
  const clientInfo_t *ci; // [esp+8Ch] [ebp-188h]
  const centity_s *player; // [esp+90h] [ebp-184h]
  int j; // [esp+94h] [ebp-180h]
  int weapon; // [esp+98h] [ebp-17Ch]
  const WeaponDef *gunnerWeapDef; // [esp+9Ch] [ebp-178h]
  int gunnerIndex; // [esp+A0h] [ebp-174h]
  const vehicle_info_t *info; // [esp+A4h] [ebp-170h]
  unsigned __int8 thirdBoneIndex; // [esp+AAh] [ebp-16Ah] BYREF
  bool isFireEvent; // [esp+ABh] [ebp-169h]
  flameWeaponConfig_t flameWeaponConfig; // [esp+ACh] [ebp-168h] BYREF
  int thirdDobjHandle; // [esp+F4h] [ebp-120h]
  bool isFirstPerson; // [esp+FBh] [ebp-119h]
  flameRender_s *flameRend; // [esp+FCh] [ebp-118h]
  unsigned __int8 firstBoneIndex; // [esp+103h] [ebp-111h] BYREF
  orientation_t thirdOrient; // [esp+104h] [ebp-110h] BYREF
  int firstDobjHandle; // [esp+134h] [ebp-E0h]
  orientation_t firstOrient; // [esp+138h] [ebp-DCh] BYREF
  bool isFiring; // [esp+16Bh] [ebp-A9h]
  unsigned int bits; // [esp+16Ch] [ebp-A8h]
  int indexHigh; // [esp+170h] [ebp-A4h]
  unsigned __int16 boneName; // [esp+174h] [ebp-A0h]
  const centity_s *centForFlags; // [esp+178h] [ebp-9Ch]
  cg_s *cgameGlob; // [esp+17Ch] [ebp-98h]
  centity_s *cent; // [esp+180h] [ebp-94h]
  unsigned int centInNextSnapshot[32]; // [esp+184h] [ebp-90h] BYREF
  int usingVehicleTagFlash; // [esp+204h] [ebp-10h]
  int i; // [esp+208h] [ebp-Ch]
  const WeaponDef *weapDef; // [esp+20Ch] [ebp-8h]
  int clientNum; // [esp+210h] [ebp-4h]

  boneName = scr_const.tag_flash;
  weapDef = 0;
  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  if ( cgameGlob->nextSnap && (!zombietron->current.enabled || localClientNum <= 0) )
  {
    memcpy(centInNextSnapshot, cgameGlob->centInNextSnapshot, sizeof(centInNextSnapshot));
    clientNum = cgameGlob->clientNum;
    centInNextSnapshot[clientNum >> 5] |= 0x80000000 >> (clientNum & 0x1F);
    clientNum = cgameGlob->predictedPlayerState.clientNum;
    centInNextSnapshot[clientNum >> 5] |= 0x80000000 >> (clientNum & 0x1F);
    for ( indexHigh = 0; indexHigh < 32; ++indexHigh )
    {
      bits = centInNextSnapshot[indexHigh];
      i = 32 * indexHigh;
      while ( bits )
      {
        if ( (bits & 0x80000000) != 0 )
        {
          cent = CG_GetEntity(localClientNum, i);
          centForFlags = cent;
          usingVehicleTagFlash = 0;
          if ( ((*((unsigned int *)cent + 201) >> 1) & 1) != 0 )
          {
            weapDef = BG_GetWeaponDef(cent->nextState.weapon);
            if ( cent->nextState.eType == 14 )
            {
              info = CG_GetVehicleInfo(cent->nextState.un2.vehicleState.vehicleInfoIndex);
              for ( gunnerIndex = 0; gunnerIndex < 4; ++gunnerIndex )
              {
                weapon = info->gunnerWeaponIndex[gunnerIndex];
                gunnerWeapDef = BG_GetWeaponDef(weapon);
                if ( gunnerWeapDef->weapType == WEAPTYPE_GAS )
                {
                  boneName = scr_const.tag_flash_gunner[gunnerIndex];
                  usingVehicleTagFlash = 1;
                  weapDef = gunnerWeapDef;
                  for ( j = 0; j < com_maxclients->current.integer; ++j )
                  {
                    player = CG_GetEntity(localClientNum, j);
                    if ( player->nextState.eType == 1 )
                    {
                      ci = &cgameGlob->bgs.clientinfo[player->nextState.clientNum];
                      if ( ci->attachedVehEntNum == i && ci->attachedVehSeat == gunnerIndex + 1 )
                      {
                        centForFlags = player;
                        goto LABEL_25;
                      }
                    }
                  }
                  break;
                }
              }
            }
LABEL_25:
            if ( centForFlags && weapDef && weapDef->weapType == WEAPTYPE_GAS )
            {
              thirdDobjHandle = cent->nextState.number;
              firstDobjHandle = CG_WeaponDObjHandle(localClientNum);
              isFirstPerson = cgameGlob->nextSnap->ps.clientNum == i
                           && ((cgameGlob->nextSnap->ps.otherFlags & 2) == 0
                            || cgameGlob->renderingThirdPerson != TP_FOR_MODEL);
              if ( Demo_IsThirdPersonCamera() || Demo_IsMovieCamera() )
                isFirstPerson = 0;
              if ( !weapDef->bUseAltTagFlash || isFirstPerson )
              {
                if ( !usingVehicleTagFlash )
                  boneName = scr_const.tag_flash;
              }
              else
              {
                boneName = scr_const.tag_flash_alt;
              }
              firstBoneIndex = -2;
              thirdBoneIndex = -2;
              isFireEvent = cent->nextState.eType == 50;
              if ( cent->nextState.eType == 6 && (centForFlags->nextState.lerp.eFlags & 0x40) != 0 )
                isFireEvent = 1;
              if ( (isFirstPerson
                 || isFireEvent
                 || CG_GetBoneIndex(localClientNum, thirdDobjHandle, boneName, &thirdBoneIndex))
                && (!isFirstPerson || CG_GetBoneIndex(localClientNum, firstDobjHandle, boneName, &firstBoneIndex)) )
              {
                isFiring = flame_test->current.enabled
                        || (centForFlags->nextState.lerp.eFlags & 0x40) != 0
                        && (cent->nextState.lerp.eFlags & 0x300) == 0
                        && cgameGlob->predictedPlayerState.waterlevel < 3
                        || isFireEvent;
                if ( isFiring )
                  *((unsigned int *)cent + 201) |= 4u;
                if ( isFiring && isFirstPerson && cent->lastMuzzleFlash < cgameGlob->time - weapDef->iRechamberBoltTime )
                {
                  *((unsigned int *)cent + 201) |= 4u;
                  cent->lastMuzzleFlash = cgameGlob->time;
                }
                if ( isFiring )
                {
                  if ( isFirstPerson )
                  {
                    FX_GetBoneOrientation(localClientNum, firstDobjHandle, firstBoneIndex, &firstOrient);
                  }
                  else if ( isFireEvent )
                  {
                    thirdOrient.origin[0] = cent->pose.origin[0];
                    thirdOrient.origin[1] = cent->pose.origin[1];
                    thirdOrient.origin[2] = cent->pose.origin[2];
                    AnglesToAxis(cent->pose.angles, thirdOrient.axis);
                  }
                  else
                  {
                    FX_GetBoneOrientation(localClientNum, thirdDobjHandle, thirdBoneIndex, &thirdOrient);
                  }
                }
                if ( isFirstPerson )
                {
                  flameWeaponConfig.fTable = weapDef->flameTableFirstPersonPtr;
                  flameRend = Flame_Get_FlameRender(weapDef->flameTableFirstPersonPtr->name);
                }
                else
                {
                  flameWeaponConfig.fTable = weapDef->flameTableThirdPersonPtr;
                  flameRend = Flame_Get_FlameRender(weapDef->flameTableThirdPersonPtr->name);
                }
                if ( isFiring )
                {
                  if ( isFirstPerson )
                  {
                    flameWeaponConfig.origin[0] = firstOrient.origin[0];
                    flameWeaponConfig.origin[1] = firstOrient.origin[1];
                    flameWeaponConfig.origin[2] = firstOrient.origin[2];
                    AxisToAngles(firstOrient.axis, flameWeaponConfig.angle);
                  }
                  else
                  {
                    flameWeaponConfig.origin[0] = thirdOrient.origin[0];
                    flameWeaponConfig.origin[1] = thirdOrient.origin[1];
                    flameWeaponConfig.origin[2] = thirdOrient.origin[2];
                    AxisToAngles(thirdOrient.axis, flameWeaponConfig.angle);
                  }
                }
                else
                {
                  flameWeaponConfig.origin[0] = cent->nextState.lerp.pos.trBase[0];
                  flameWeaponConfig.origin[1] = cent->nextState.lerp.pos.trBase[1];
                  flameWeaponConfig.origin[2] = cent->nextState.lerp.pos.trBase[2];
                  flameWeaponConfig.angle[0] = cent->nextState.lerp.apos.trBase[0];
                  flameWeaponConfig.angle[1] = cent->nextState.lerp.apos.trBase[1];
                  flameWeaponConfig.angle[2] = cent->nextState.lerp.apos.trBase[2];
                }
                flameWeaponConfig.bFireWhileIdle = 0;
                flameWeaponConfig.bIsFiring = isFiring;
                flameWeaponConfig.burnRate = FLOAT_1_0;
                flameWeaponConfig.strength = FLOAT_1_0;
                flameWeaponConfig.thickness = FLOAT_1_0;
                flameWeaponConfig.entityOrigin[0] = cent->nextState.lerp.pos.trBase[0];
                flameWeaponConfig.entityOrigin[1] = cent->nextState.lerp.pos.trBase[1];
                flameWeaponConfig.entityOrigin[2] = cent->nextState.lerp.pos.trBase[2];
                flameWeaponConfig.damage = weapDef->damage;
                LocalClientFlameSource = Flame_GetLocalClientFlameSource(localClientNum, i);
                source = Flame_Source_Get(LocalClientFlameSource);
                if ( !source )
                {
                  CG_GetLocalClientGlobals(localClientNum);
                  v2 = Flame_GetLocalClientFlameSource(localClientNum, i);
                  source = Flame_Source_Alloc(v2);
                  if ( !source
                    && !Assert_MyHandler(
                          "C:\\projects_pc\\cod\\codsrc\\src\\flame\\flame_system.cpp",
                          1271,
                          0,
                          "%s\n\t%s",
                          "source",
                          "No free flameSources") )
                  {
                    __debugbreak();
                  }
                }
                if ( source )
                {
                  v3 = Flame_GetLocalClientFlameSource(localClientNum, i);
                  source->entityNum = v3;
                  source->firstDobjHandle = firstDobjHandle;
                  source->firstBoneIndex = firstBoneIndex;
                  source->thirdDobjHandle = thirdDobjHandle;
                  source->thirdBoneIndex = thirdBoneIndex;
                  source->isKillcamFlame = cgameGlob->inKillCam;
                  source->firingThroughGeo = 0;
                  if ( isFiring && !isFirstPerson )
                  {
                    memset(&trace, 0, 16);
                    AngleVectors(flameWeaponConfig.angle, dir, 0, 0);
                    start[0] = (float)(-23.0 * dir[0]) + flameWeaponConfig.origin[0];
                    start[1] = (float)(-23.0 * dir[1]) + flameWeaponConfig.origin[1];
                    start[2] = (float)(-23.0 * dir[2]) + flameWeaponConfig.origin[2];
                    if ( flame_debug_render->current.integer > 0 )
                      CG_DebugLine(start, flameWeaponConfig.origin, colorRed, 0, 300);
                    CG_LocationalTrace(
                      &trace,
                      start,
                      flameWeaponConfig.origin,
                      source->entityNum,
                      (int)&cls.recentServers[7543].countrycode[1],
                      0,
                      0);
                    if ( trace.fraction < 1.0 )
                    {
                      flameWeaponConfig.origin[0] = start[0];
                      flameWeaponConfig.origin[1] = start[1];
                      flameWeaponConfig.origin[2] = start[2];
                      source->firingThroughGeo = 1;
                    }
                  }
                  time = CG_GetLocalClientGlobals(localClientNum)->time;
                  CG_Flame_Update_Source_Internal(source, i, &flameWeaponConfig, time, flameRend);
                  v4 = CG_GetLocalClientGlobals(localClientNum)->time;
                  Flame_Class_Source_Sound(
                    localClientNum,
                    isFiring,
                    isFirstPerson,
                    v4,
                    flameWeaponConfig.fTable,
                    source);
                }
              }
            }
          }
        }
        ++i;
        bits *= 2;
      }
    }
  }
}

void __cdecl Flame_Generate_Verts(int localClientNum)
{
  char *Name; // [esp+8h] [ebp-Ch]
  int i; // [esp+10h] [ebp-4h]

  Name = va("Flame_Generate_Verts(cl=%d)", localClientNum);
  //PIXBeginNamedEvent(-1, Name);
  if ( flame_render->current.enabled )
  {
    Flame_Class_Smoke_Render_All(localClientNum);
    Flame_Class_Fire_Render_All(localClientNum);
    Flame_Class_Drips_Render_All(localClientNum);
    Flame_Class_Chunk_Render_All(localClientNum);
    Flame_Class_Stream_Render_All(localClientNum);
    for ( i = 0; i < numFlameVars; ++i )
    {
      Flame_Class_Fire_Render_Local_List(localClientNum, (flameFire_t *)flameVarList[i].fireList);
      Flame_Class_Drips_Render_Local_List(localClientNum, (flameDrips_t *)flameVarList[i].dripsList);
    }
  }
  if ( GetCurrentThreadId() == g_DXDeviceThread )
    D3DPERF_EndEvent();
}

void __cdecl CG_Flame_Update_ViewModel(int localClientNum, centity_s *cent)
{
  int k; // [esp+18h] [ebp-14h]
  float oldOffset; // [esp+1Ch] [ebp-10h]
  int j; // [esp+20h] [ebp-Ch]
  int i; // [esp+24h] [ebp-8h]
  cg_s *cgameGlob; // [esp+28h] [ebp-4h]

  cgameGlob = CG_GetLocalClientGlobals(localClientNum);
  if ( (cent->nextState.lerp.eFlags & 0x40) != 0 )
  {
    for ( i = 0; i < 3; ++i )
    {
      cgameGlob->flamethrowerKickOffset[i] = (float)((float)((float)(flame_kick_offset->current.vector[i]
                                                                   * flame_kick_speed->current.value)
                                                           * (float)cgameGlob->frametime)
                                                   / 1000.0)
                                           + cgameGlob->flamethrowerKickOffset[i];
      if ( COERCE_FLOAT(LODWORD(cgameGlob->flamethrowerKickOffset[i]) & _mask__AbsFloat_) > COERCE_FLOAT(
                                                                                              *(&flame_kick_offset->current.integer
                                                                                              + i)
                                                                                            & _mask__AbsFloat_) )
        cgameGlob->flamethrowerKickOffset[i] = flame_kick_offset->current.vector[i];
    }
  }
  else
  {
    for ( j = 0; j < 3; ++j )
    {
      oldOffset = cgameGlob->flamethrowerKickOffset[j];
      cgameGlob->flamethrowerKickOffset[j] = oldOffset
                                           - (float)((float)((float)(flame_kick_offset->current.vector[j]
                                                                   * flame_kick_recover_speed->current.value)
                                                           * (float)cgameGlob->frametime)
                                                   / 1000.0);
      if ( COERCE_FLOAT(LODWORD(cgameGlob->flamethrowerKickOffset[j]) & _mask__AbsFloat_) > COERCE_FLOAT(
                                                                                              LODWORD(oldOffset)
                                                                                            & _mask__AbsFloat_) )
        cgameGlob->flamethrowerKickOffset[j] = *(float *)&FLOAT_0_0;
    }
  }
  for ( k = 0; k < 3; ++k )
  {
    cgameGlob->viewModelAxis[3][0] = (float)(cgameGlob->flamethrowerKickOffset[k] * cgameGlob->viewModelAxis[k][0])
                                   + cgameGlob->viewModelAxis[3][0];
    cgameGlob->viewModelAxis[3][1] = (float)(cgameGlob->flamethrowerKickOffset[k] * cgameGlob->viewModelAxis[k][1])
                                   + cgameGlob->viewModelAxis[3][1];
    cgameGlob->viewModelAxis[3][2] = (float)(cgameGlob->flamethrowerKickOffset[k] * cgameGlob->viewModelAxis[k][2])
                                   + cgameGlob->viewModelAxis[3][2];
  }
}

double __cdecl Flame_Random(bool is_server)
{
  if ( is_server )
    return G_random();
  flame_randomseed = 214013 * flame_randomseed + 2531011;
  return (double)((flame_randomseed >> 16) & 0x7FFF) / 32768.0;
}

double __cdecl Flame_CRandom(bool is_server)
{
  return Flame_Random(is_server) * 2.0 - 1.0;
}

double __cdecl Flame_CalcTimeScale(int startTime, int endTime)
{
  if ( endTime - startTime <= 0 )
    return 0.0;
  else
    return 1.0 / (double)(endTime - startTime);
}

double __cdecl Flame_CalcInvStartSpeed(float invInitialSpeed, float speedScale)
{
  float v3; // [esp+0h] [ebp-8h]
  float speedScalea; // [esp+14h] [ebp+Ch]

  if ( invInitialSpeed == 0.0 )
    v3 = *(float *)&FLOAT_0_0;
  else
    v3 = 1.0 / invInitialSpeed;
  speedScalea = speedScale * v3;
  if ( speedScalea == 0.0 )
    return 0.0;
  else
    return 1.0 / speedScalea;
}

