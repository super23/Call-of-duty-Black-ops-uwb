#pragma once

static const int CHANGELIST_NUMBER = 794515;
static const int BUILD_NUMBER = 61;
static const char *BUILD_MACHINE = "CODPCAB-V64";
// Build date/time come from __DATE__ / __TIME__ in com_buildinfo.cpp at compile time.

char *__cdecl Com_GetBuildVersion();
int __cdecl Com_GetBuildNumber();
const char *__cdecl Com_GetBuildDisplayName();
const char *__cdecl Com_GetBuildOfficialNameR();
const char *__cdecl Com_GetBuildDisplayNameR();
const char *__cdecl Com_GetBuildName();
int __cdecl Com_GetChangelist();
