#pragma once

void __cdecl R_RegisterDvars();
const dvar_s *__cdecl R_RegisterWorkerThreadDvar(char *name, unsigned int workerIndex);
char __cdecl R_CheckDvarModified(const dvar_s *dvar);
