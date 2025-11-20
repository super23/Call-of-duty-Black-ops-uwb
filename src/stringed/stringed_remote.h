#pragma once

char __cdecl ProcessStringEdCmds();
char *__cdecl GetString(const char *PackageAndStringReference);
EXT_Link<LocalizeEntry> *__thiscall EXT_List<LocalizeEntry>::Add(
        EXT_List<LocalizeEntry> *this,
        LocalizeEntry *m_Data,
        bool First);
