#pragma once

const char *__cdecl CG_GetMedalData(int medalIndex, medalTableColumns_t column);
char *__cdecl CG_GetMedalLocalizedXP(float xpScale, int medalIndex, bool teamBased);
const char *__cdecl CG_GetWagerMedalData(int wagerMedalIndex, wagerMedalTableColumns_t column);
char *__cdecl CG_GetWagerMedalLocalizedCP(int wagerMedalIndex, bool teamBased);
