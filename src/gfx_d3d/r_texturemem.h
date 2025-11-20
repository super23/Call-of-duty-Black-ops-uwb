#pragma once

unsigned int __cdecl R_AvailableTextureMemory();
unsigned int __cdecl R_VideoMemory();
const DDraw *__cdecl DDraw::Instance();
DDraw *__thiscall DDraw::DDraw(DDraw *this);
unsigned int __cdecl R_VideoMemoryForDevice(_GUID *lpGUID);
int __stdcall R_DDEnumCallback(
        _GUID *lpGUID,
        char *lpDriverDescription,
        char *lpDriverName,
        unsigned int *lpContext,
        HMONITOR__ *hm);
unsigned int __cdecl R_DrasticVideoMemoryForDevice(_GUID *lpGUID);
int __stdcall R_DDEnumDrasticCallback(
        _GUID *lpGUID,
        char *lpDriverDescription,
        char *lpDriverName,
        unsigned int *lpContext,
        HMONITOR__ *hm);
