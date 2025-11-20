#include "demo_version.h"

int __cdecl Demo_GetVersion()
{
  if ( Demo_IsIdle() )
    Com_Error(ERR_DROP, &byte_D59F38);
  return demo.header.version;
}

const NetFieldList *__cdecl Demo_GetNetFieldListForType(netFieldTypes_t fieldType)
{
  int netFieldsVersionIndex; // [esp+0h] [ebp-8h]

  netFieldsVersionIndex = 1 - Demo_GetVersion();
  if ( netFieldsVersionIndex < 0 )
    Com_Error(ERR_DROP, &byte_D59F78);
  return &s_demoOtherNetFieldLists[netFieldsVersionIndex][fieldType];
}

const NetFieldList *__cdecl Demo_GetStateFieldListForEntityType(int eType)
{
  int v2; // [esp+0h] [ebp-Ch]
  int netFieldsVersionIndex; // [esp+4h] [ebp-8h]

  netFieldsVersionIndex = 1 - Demo_GetVersion();
  if ( netFieldsVersionIndex < 0 )
    Com_Error(ERR_DROP, &byte_D59F78);
  if ( eType > 21 )
    v2 = 21;
  else
    v2 = eType;
  return &s_demoEntityNetFieldLists[netFieldsVersionIndex][v2];
}

