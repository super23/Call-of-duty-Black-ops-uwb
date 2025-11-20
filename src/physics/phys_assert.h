#pragma once

void __cdecl phys_set_debug_callback(void (__cdecl *debug_callback)(void *));
void __cdecl phys_exec_debug_callback(void *data);
void __cdecl PHYS_WARNING(const char *file, int line, const char *expr, const char *desc);
phys_assert_info *__thiscall phys_assert_info::phys_assert_info(
        phys_assert_info *this,
        int max_hits_total,
        int max_hits_per_frame,
        bool use_warnings_only);
void __thiscall phys_assert_info::frame_advance(phys_assert_info *this);
void __cdecl phys_assert_info_frame_advance_all();
