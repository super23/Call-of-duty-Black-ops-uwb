// EDITOR_PORT — InitInstance from CoDBORadiant.decompile.c sub_467BF0 (cleaned).
#include "RadiantApp.h"

#include <afxdlgs.h>
#include <cstring>

#include "../GfxWrapper.h"
#include "../radiant.h"

RadiantApp theRadiantApp;

RadiantApp::RadiantApp()
{
}

static bool FindRegistryIniBesideExe(char *outIni, size_t outSize)
{
    char modulePath[MAX_PATH] = {};
    if (!GetModuleFileNameA(nullptr, modulePath, MAX_PATH))
        return false;

    char *slash = std::strrchr(modulePath, '\\');
    if (!slash)
        return false;
    *(slash + 1) = '\0';

    std::strncpy(outIni, modulePath, outSize - 1);
    outIni[outSize - 1] = '\0';
    std::strncat(outIni, "REGISTRY.INI", outSize - std::strlen(outIni) - 1);

    const DWORD attrs = GetFileAttributesA(outIni);
    return attrs != INVALID_FILE_ATTRIBUTES && !(attrs & FILE_ATTRIBUTE_DIRECTORY);
}

BOOL RadiantApp::InitInstance()
{
    if (!AfxWinInit(::GetModuleHandle(nullptr), nullptr, ::GetCommandLineA(), SW_SHOWDEFAULT))
        return FALSE;

    InitCommonControls();

    char registryIni[MAX_PATH] = {};
    if (FindRegistryIniBesideExe(registryIni, sizeof(registryIni)))
        SetRegistryKey("iw\\CoDBORadiant");
    else
        SetRegistryKey("iw\\CoDBORadiant");

    LoadStdProfileSettings(4);

    RadiantRemote_Init();

    if (!Radiant_GfxInit())
        Radiant_Error(__FILE__, 180, 0, "DirectX error during startup");

    // CMainFrame (sub_42C390) and document/view graph still to port from decomp.
    return TRUE;
}

int RadiantApp::ExitInstance()
{
    Radiant_GfxShutdown();
    RadiantRemote_Shutdown();
    return CWinApp::ExitInstance();
}

int RadiantApp::Run()
{
    MSG msg;
    while (GetMessageA(&msg, nullptr, 0, 0) > 0)
    {
        RadiantRemote_Frame();
        if (!PreTranslateMessage(&msg))
        {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
    }
    return static_cast<int>(msg.wParam);
}
