#include "win_splash.h"

void __cdecl Sys_DestroySplashWindow()
{
  if ( g_splashWnd )
  {
    Sys_HideSplashWindow();
    DestroyWindow(g_splashWnd);
    g_splashWnd = 0;
  }
}

void __cdecl Sys_HideSplashWindow()
{
  if ( g_splashWnd )
    ShowWindow(g_splashWnd, 0);
}

