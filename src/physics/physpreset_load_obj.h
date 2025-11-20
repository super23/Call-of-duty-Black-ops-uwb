#pragma once

unsigned __int8 *__cdecl Hunk_AllocPhysPresetPrecache(int size);
PhysPreset *__cdecl PhysPresetLoadFile(const char *name, void *(__cdecl *Alloc)(int));
void __cdecl PhysPreset_Strcpy(unsigned __int8 *member, const char *keyValue);
PhysPreset *__cdecl PhysPresetPrecache(const char *name, void *(__cdecl *Alloc)(int));
unsigned __int8 *__cdecl Hunk_AllocPhysPresetPrecache(unsigned int size);
PhysPreset *__cdecl PhysPreset_Register(const char *name);
PhysPreset *__cdecl PhysPreset_Register_FastFile(const char *name);
PhysPreset *__cdecl PhysPreset_Register_LoadObj(const char *name);
