#pragma once

void __cdecl PS_CreatePunctuationTable(script_s *script, punctuation_s *punctuations);
void ScriptError(script_s *script, const char *str, ...);
void ScriptWarning(script_s *script, const char *str, ...);
int __cdecl PS_ReadWhiteSpace(script_s *script);
int __cdecl PS_ReadEscapeCharacter(script_s *script, char *ch);
int __cdecl PS_ReadString(script_s *script, token_s *token, int quote);
int __cdecl PS_ReadName(script_s *script, token_s *token);
void __cdecl NumberValue(char *string, __int16 subtype, unsigned int *intvalue, long double *floatvalue);
int __cdecl PS_ReadNumber(script_s *script, token_s *token);
int __cdecl PS_ReadPunctuation(script_s *script, token_s *token);
int __cdecl PS_ReadPrimitive(script_s *script, token_s *token);
int __cdecl PS_ReadToken(script_s *script, token_s *token);
void __cdecl StripDoubleQuotes(char *string);
bool __cdecl EndOfScript(script_s *script);
script_s *__cdecl LoadScriptFile(const char *filename);
void __cdecl SetScriptPunctuations(script_s *script);
script_s *__cdecl LoadScriptMemory(char *ptr, unsigned int length, const char *name);
void __cdecl FreeScript(script_s *script);
