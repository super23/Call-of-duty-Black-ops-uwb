#include "cscr_readwrite.h"

unsigned int __cdecl FindLastSibling(scriptInstance_t inst, unsigned int parentId)
{
  VariableValueInternal *parentValue; // [esp+20h] [ebp-10h]
  unsigned int index; // [esp+28h] [ebp-8h]
  unsigned int id; // [esp+2Ch] [ebp-4h]

  if ( !parentId
    && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\clientscript\\scr_variable.h", 665, 0, "%s", "parentId") )
  {
    __debugbreak();
  }
  parentValue = &gScrVarGlob[inst].variableList[parentId + 1];
  if ( (parentValue->w.status & 0x60) == 0
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\clientscript\\scr_variable.h",
          323,
          0,
          "%s",
          "(entryValue->w.status & VAR_STAT_MASK) != VAR_STAT_FREE") )
  {
    __debugbreak();
  }
  if ( (parentValue->w.status & 0x1F) < 0xD
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\clientscript\\scr_variable.h",
          667,
          0,
          "%s",
          "IsObject( parentValue )") )
  {
    __debugbreak();
  }
  if ( (parentValue->w.status & 0x60) != 0x60
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\clientscript\\scr_variable.h",
          668,
          0,
          "%s\n\t(parentValue->w.status & VAR_STAT_MASK) = %i",
          "((parentValue->w.status & VAR_STAT_MASK) == VAR_STAT_EXTERNAL)",
          parentValue->w.status & 0x60) )
  {
    __debugbreak();
  }
  id = gScrVarGlob[inst].variableList[parentValue->v.next + 1].hash.u.prev;
  if ( !id )
    return 0;
  index = FindVariableIndexInternal(inst, parentId, gScrVarGlob[inst].variableList[id + 0x8000].w.status >> 8);
  if ( !index
    && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\clientscript\\scr_variable.h", 676, 0, "%s", "index") )
  {
    __debugbreak();
  }
  return index;
}

unsigned int __cdecl FindVariableIndexInternal(scriptInstance_t inst, unsigned int parentId, unsigned int name)
{
  VariableValueInternal *parentValue; // [esp+18h] [ebp-8h]

  if ( !parentId
    && !Assert_MyHandler("c:\\projects_pc\\cod\\codsrc\\src\\clientscript\\scr_variable.h", 550, 0, "%s", "parentId") )
  {
    __debugbreak();
  }
  parentValue = &gScrVarGlob[inst].variableList[parentId + 1];
  if ( (parentValue->w.status & 0x60) != 0x60
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\clientscript\\scr_variable.h",
          552,
          0,
          "%s",
          "(parentValue->w.status & VAR_STAT_MASK) == VAR_STAT_EXTERNAL") )
  {
    __debugbreak();
  }
  if ( (parentValue->w.status & 0x60) == 0
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\clientscript\\scr_variable.h",
          323,
          0,
          "%s",
          "(entryValue->w.status & VAR_STAT_MASK) != VAR_STAT_FREE") )
  {
    __debugbreak();
  }
  if ( (parentValue->w.status & 0x1F) < 0xD
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\clientscript\\scr_variable.h",
          553,
          0,
          "%s",
          "IsObject( parentValue )") )
  {
    __debugbreak();
  }
  return FindVariableIndexInternal2(inst, name, (parentId + name) % 0x3FFFD + 1);
}

unsigned int __cdecl FindVariableIndexInternal2(scriptInstance_t inst, unsigned int name, unsigned int index)
{
  Variable *entry; // [esp+0h] [ebp-18h]
  unsigned int newIndex; // [esp+4h] [ebp-14h]
  VariableValueInternal *list; // [esp+8h] [ebp-10h]
  VariableValueInternal *entryValue; // [esp+Ch] [ebp-Ch]
  VariableValueInternal *newEntryValue; // [esp+10h] [ebp-8h]
  Variable *newEntry; // [esp+14h] [ebp-4h]

  list = gScrVarGlob[inst].variableList + 0x8000;
  if ( (name & 0xFF000000) != 0
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\clientscript\\scr_variable.h",
          501,
          0,
          "%s",
          "!(name & ~VAR_NAME_LOW_MASK)") )
  {
    __debugbreak();
  }
  if ( index >= 0x3FFFE
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\clientscript\\scr_variable.h",
          502,
          0,
          "index doesn't index VARIABLELIST_CHILD_SIZE(inst)\n\t%i not in [0, %i)",
          index,
          262142) )
  {
    __debugbreak();
  }
  entry = &list[index].hash;
  if ( entry->id >= 0x3FFFE
    && !Assert_MyHandler(
          "c:\\projects_pc\\cod\\codsrc\\src\\clientscript\\scr_variable.h",
          506,
          0,
          "entry->id doesn't index VARIABLELIST_CHILD_SIZE(inst)\n\t%i not in [0, %i)",
          entry->id,
          262142) )
  {
    __debugbreak();
  }
  entryValue = &list[entry->id];
  if ( (entryValue->w.status & 0x60) == 0x40 )
  {
    if ( (entryValue->w.status & 0x60) == 0
      && !Assert_MyHandler(
            "c:\\projects_pc\\cod\\codsrc\\src\\clientscript\\scr_variable.h",
            323,
            0,
            "%s",
            "(entryValue->w.status & VAR_STAT_MASK) != VAR_STAT_FREE") )
    {
      __debugbreak();
    }
    if ( (entryValue->w.status & 0x1F) >= 0xD
      && !Assert_MyHandler(
            "c:\\projects_pc\\cod\\codsrc\\src\\clientscript\\scr_variable.h",
            511,
            0,
            "%s",
            "!IsObject( entryValue )") )
    {
      __debugbreak();
    }
    if ( entryValue->w.status >> 8 == name )
      return index;
    newIndex = entryValue->v.next;
    for ( newEntry = &list[newIndex].hash; newEntry != entry; newEntry = &list[newIndex].hash )
    {
      newEntryValue = &list[newEntry->id];
      if ( (newEntryValue->w.status & 0x60) != 0x20
        && !Assert_MyHandler(
              "c:\\projects_pc\\cod\\codsrc\\src\\clientscript\\scr_variable.h",
              523,
              0,
              "%s",
              "(newEntryValue->w.status & VAR_STAT_MASK) == VAR_STAT_MOVABLE") )
      {
        __debugbreak();
      }
      if ( (newEntryValue->w.status & 0x60) == 0
        && !Assert_MyHandler(
              "c:\\projects_pc\\cod\\codsrc\\src\\clientscript\\scr_variable.h",
              323,
              0,
              "%s",
              "(entryValue->w.status & VAR_STAT_MASK) != VAR_STAT_FREE") )
      {
        __debugbreak();
      }
      if ( (newEntryValue->w.status & 0x1F) >= 0xD
        && !Assert_MyHandler(
              "c:\\projects_pc\\cod\\codsrc\\src\\clientscript\\scr_variable.h",
              524,
              0,
              "%s",
              "!IsObject( newEntryValue )") )
      {
        __debugbreak();
      }
      if ( newEntryValue->w.status >> 8 == name )
        return newIndex;
      newIndex = newEntryValue->v.next;
    }
  }
  return 0;
}

