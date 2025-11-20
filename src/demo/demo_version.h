#pragma once

int __cdecl Demo_GetVersion();
const NetFieldList *__cdecl Demo_GetNetFieldListForType(netFieldTypes_t fieldType);
const NetFieldList *__cdecl Demo_GetStateFieldListForEntityType(int eType);
