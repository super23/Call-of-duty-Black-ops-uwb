#include "com_buildinfo.h"
#include <stdio.h>
#include <win32/win_localize.h>
#include <string.h>

char buf[128];

// Set by the compiler at build time (e.g. "May 28 2026" and "14:30:00").
static const char COM_BUILD_DATE[] = __DATE__;
static const char COM_BUILD_TIME[] = __TIME__;

char *__cdecl Com_GetBuildVersion()
{
    if ( CHANGELIST_NUMBER )
        sprintf(buf, "%s.%s.%d CL(%d) %s %s %s", "7", "0", BUILD_NUMBER, CHANGELIST_NUMBER, BUILD_MACHINE, COM_BUILD_DATE, COM_BUILD_TIME);
    else
        sprintf(buf, "%s.%s.%d %s %s %s", "7", "0", BUILD_NUMBER, BUILD_MACHINE, COM_BUILD_DATE, COM_BUILD_TIME);
    return buf;
}

int __cdecl Com_GetBuildNumber()
{
    return BUILD_NUMBER;
}

const char *__cdecl Com_GetBuildDisplayName()
{
#ifdef KISAK_SP
#ifdef _DEBUG
    return "Call of Duty Single Player - Debug (0.1)";
#else
    return "Call of Duty Single Player - Release (0.1)";
#endif
#else
#ifdef _DEBUG
    return "Call of Duty Multiplayer - Debug (0.1)";
#else
    return "Call of Duty Multiplayer - Release (0.1)";
#endif
#endif
}

const char *__cdecl Com_GetBuildOfficialNameR()
{
    char *Language; // eax

    Language = Win_GetLanguage();
    // there is an if() here for japanese, but both branches return the same string
    //_stricmp(Language, "japanese");
    //return aCallOfDuty_1;
    return "Call of Duty - Black Ops";
}

const char *__cdecl Com_GetBuildDisplayNameR()
{
    char *Language; // eax

    Language = Win_GetLanguage();
#ifdef KISAK_SP
    (void)Language;
#ifdef _DEBUG
    return "Call of Duty(R) Single Player - Debug";
#else
    return "Call of Duty(R) Single Player - Release";
#endif
#else
    if ( !_stricmp(Language, "japanese") )
#ifdef _DEBUG
        return "Call of Duty(R) Multiplayer - Debug";
#else
        return "Call of Duty(R) Multiplayer - Release";
#endif
    else
#ifdef _DEBUG
        return "Call of Duty(R) Multiplayer - Debug";
#else
        return "Call of Duty(R) Multiplayer - Release";
#endif
#endif
}

const char *__cdecl Com_GetBuildName()
{
#ifdef KISAK_SP
    return "COD_T5_R SP EpikIzCool Build";
#else
    return "COD_T5_R MP EpikIzCool Build";
#endif
}

int __cdecl Com_GetChangelist()
{
    return CHANGELIST_NUMBER;
}

