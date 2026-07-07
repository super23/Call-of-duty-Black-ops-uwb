// Editor fatal / log path (decomp sub_4B8480 / sub_4B7720).
#include "radiant.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

void Radiant_Printf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
}

[[noreturn]] void Radiant_Error(const char *file, int line, int fatal, const char *fmt, ...)
{
    char body[4096];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(body, sizeof(body), fmt, ap);
    va_end(ap);

    char message[4352];
    snprintf(message, sizeof(message), "FATAL ERROR: %s", body);
    (void)fatal;

#ifdef _WIN32
    char caption[512];
    snprintf(caption, sizeof(caption), "CoDBORadiant (%s:%d)", file ? file : "?", line);
    MessageBoxA(nullptr, message, caption, MB_OK | MB_ICONERROR);
#endif

    fprintf(stderr, "%s (%s:%d)\n", message, file ? file : "?", line);
    fflush(stderr);
    std::abort();
}
