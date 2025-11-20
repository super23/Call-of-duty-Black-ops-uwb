#pragma once

void __cdecl SentientScr_ReadOnly(sentient_s *pSelf, const sentient_fields_s *pField);
void __cdecl SentientScr_SetTeam(sentient_s *pSelf);
void __cdecl SentientScr_GetTeam(sentient_s *pSelf);
void __cdecl GScr_AddFieldsForSentient();
void __cdecl Scr_SetSentientField(sentient_s *sentient, unsigned int offset);
void __cdecl Scr_GetSentientField(sentient_s *sentient, unsigned int offset);
