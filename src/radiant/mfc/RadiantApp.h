// EDITOR_PORT — minimal MFC application object.
#pragma once

#ifndef KISAK_RADIANT_EDITOR
#error "src/radiant requires KISAK_RADIANT_EDITOR=1"
#endif

#include <afxwin.h>

class RadiantApp : public CWinApp
{
public:
    RadiantApp();

    BOOL InitInstance() override;
    int ExitInstance() override;
    int Run() override;
};

extern RadiantApp theRadiantApp;
