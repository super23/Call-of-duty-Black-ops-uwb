#pragma once

void __cdecl UILocalVar_Init(UILocalVarContext *context);
void __cdecl UILocalVar_Shutdown(UILocalVarContext *context);
UILocalVarContext *__cdecl UILocalVar_Find(UILocalVarContext *context, const char *name);
char __cdecl UILocalVar_FindLocation(UILocalVarContext *context, const char *name, unsigned int *hashForName);
unsigned int __cdecl UILocalVar_HashName(const char *name);
UILocalVarContext *__cdecl UILocalVar_FindOrCreate(UILocalVarContext *context, char *name);
bool __cdecl UILocalVar_GetBool(const UILocalVar *var);
UILocalVar::<unnamed_type_u> __cdecl UILocalVar_GetInt(const UILocalVar *var);
double __cdecl UILocalVar_GetFloat(const UILocalVar *var);
char *__cdecl UILocalVar_GetString(const UILocalVar *var, char *stringBuf, unsigned int size);
void __cdecl UILocalVar_SetBool(const UILocalVar *var, bool b);
void __cdecl UILocalVar_SetInt(const UILocalVar *var, int i);
void __cdecl UILocalVar_SetFloat(const UILocalVar *var, float f);
void __cdecl UILocalVar_SetString(const UILocalVar *var, char *s);
