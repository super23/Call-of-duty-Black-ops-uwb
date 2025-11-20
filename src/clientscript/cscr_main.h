#pragma once

bool __cdecl Scr_IsInOpcodeMemory(scriptInstance_t inst, const char *pos);
char __cdecl Scr_IsIdentifier(const char *token);
int __cdecl Scr_GetFunctionHandle(scriptInstance_t inst, const char *filename, const char *name);
unsigned int __cdecl SL_TransferToCanonicalString(scriptInstance_t inst, unsigned int stringValue);
unsigned int __cdecl SL_GetCanonicalString(scriptInstance_t inst, char *str);
void __cdecl Scr_BeginLoadScripts(scriptInstance_t inst, int user);
void __cdecl SL_BeginLoadScripts(scriptInstance_t inst);
void __cdecl Scr_BeginLoadAnimTrees(scriptInstance_t inst, int user);
int __cdecl Scr_ScanFile(scriptInstance_t inst, char *buf, int max_size);
unsigned int __cdecl Scr_LoadScriptInternal(
        scriptInstance_t inst,
        const char *filename,
        PrecacheEntry *entries,
        int entriesCount);
unsigned int __cdecl Scr_LoadScript(scriptInstance_t inst, char *filename);
void __cdecl Scr_PostCompileScripts(scriptInstance_t inst);
void __cdecl Scr_EndLoadScripts(scriptInstance_t inst);
void __cdecl Scr_PrecacheAnimTrees(scriptInstance_t inst, void *(__cdecl *Alloc)(int), int user, bool modChecksum);
void __cdecl Scr_EndLoadAnimTrees(scriptInstance_t inst);
void __cdecl Scr_FreeScripts(scriptInstance_t inst, unsigned __int8 sys);
void __cdecl Scr_GetGenericField(
        unsigned __int8 *b,
        fieldtype_t type,
        int ofs,
        scriptInstance_t inst,
        unsigned int whichbits);
void __cdecl Scr_SetGenericField(
        unsigned __int8 *b,
        fieldtype_t type,
        int ofs,
        scriptInstance_t inst,
        unsigned int whichbits);
