#pragma once

PhysConstraints *__cdecl PhysConstraintsLoadFile(const char *name, void *(__cdecl *Alloc)(int));
void __cdecl PhysConstraints_Strcpy(unsigned __int8 *member, const char *keyValue);
int __cdecl CON_ParseSpecificField(unsigned __int8 *pStruct, const char *pValue, int fieldType);
void __cdecl PhysConstraintsPostLoadFixup(PhysConstraints *physConstraints);
PhysConstraints *__cdecl PhysConstraintsPrecache(const char *name, void *(__cdecl *Alloc)(int));
void __cdecl PhysConstraintsUpdateField(char *name, char *keyvalue);
PhysConstraints *__cdecl GetPhysConstraints(char *name);
