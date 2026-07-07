// CoDBORadiant WinMain entry (MFC AfxWinMain).
#ifndef KISAK_RADIANT_EDITOR
#error "src/radiant requires KISAK_RADIANT_EDITOR=1"
#endif

#include <afxwin.h>

#include "mfc/RadiantApp.h"

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd)
{
    if (!AfxWinInit(hInstance, hPrevInstance, lpCmdLine, nShowCmd))
        return -1;

    if (!theRadiantApp.InitInstance())
        return -1;

    return theRadiantApp.Run();
}
