#pragma once

void __cdecl phys_task_manager_init();
void __cdecl phys_task_manager_shutdown();
bool __cdecl phys_task_manager_needs_flush();
void __cdecl phys_task_manager_flush();
void __cdecl phys_task_manager_process(jqModule *module, void *input, int input_count);
