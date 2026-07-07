#pragma once
#include <clientscript/cscr_vm.h>

void(__cdecl *__cdecl Sentient_GetMethod(const char **pName))(scr_entref_t);
sentient_s *__fastcall Sentient_Get(scr_entref_t entref);
void __cdecl SentientCmd_GetClosestEnemySqDist(scr_entref_t entref);
void __cdecl SentientCmd_GetEnemySqDist(scr_entref_t entref);
void __cdecl SentientCmd_GetThreatBias(scr_entref_t entref);
void __cdecl SentientCmd_GetThreatBiasGroup(scr_entref_t entref);
void __cdecl SentientCmd_IsNoTarget(scr_entref_t entref);
void __cdecl SentientCmd_SetThreatBiasGroup(scr_entref_t entref);
void __cdecl SentientCmd_SetThreatBias();
void __cdecl SentientCmd_SetThreatBiasAgainstAll();
void __cdecl SentientCmd_ThreatBiasGroupExists();
void __cdecl SentientCmd_SetIgnoreMeGroup();
void __cdecl SentientCmd_CreateThreatBiasGroup();
