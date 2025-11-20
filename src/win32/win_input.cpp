#include "win_input.h"

void __cdecl IN_StartupGamepads()
{
  GPad_InitAll();
}

void __cdecl IN_RecenterMouse()
{
  tagRECT window_rect; // [esp+0h] [ebp-10h] BYREF

  GetWindowRect(g_wv.hWnd, &window_rect);
  window_center_x = (window_rect.left + window_rect.right) / 2;
  window_center_y = (window_rect.bottom + window_rect.top) / 2;
  SetCursorPos((window_rect.left + window_rect.right) / 2, (window_rect.bottom + window_rect.top) / 2);
}

bool __cdecl IN_IsForegroundWindow()
{
  return GetForegroundWindow() == g_wv.hWnd;
}

void __cdecl IN_ActivateMouse(int force)
{
  if ( s_wmv.mouseInitialized )
  {
    if ( !r_fullscreen
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\win32\\win_input.cpp", 332, 0, "%s", "r_fullscreen") )
    {
      __debugbreak();
    }
    if ( in_mouse->current.enabled )
    {
      if ( force || !s_wmv.mouseActive )
        s_wmv.mouseActive = IN_IsForegroundWindow();
    }
    else
    {
      s_wmv.mouseActive = 0;
    }
  }
}

void __cdecl IN_DeactivateMouse()
{
  if ( s_wmv.mouseInitialized && s_wmv.mouseActive )
  {
    s_wmv.mouseActive = 0;
    IN_DeactivateWin32Mouse();
  }
}

void IN_DeactivateWin32Mouse()
{
  IN_ShowSystemCursor(1);
}

void __cdecl IN_StartupMouse()
{
  s_wmv.mouseInitialized = 0;
  if ( in_mouse->current.enabled )
  {
    s_wmv.mouseInitialized = 1;
    BG_EvalVehicleName();
  }
  else
  {
    Com_Printf(16, "Mouse control not active.\n");
  }
}

void __cdecl IN_MouseEvent(int mstate)
{
  int diff; // [esp+0h] [ebp-8h]
  int button; // [esp+4h] [ebp-4h]

  if ( s_wmv.mouseInitialized )
  {
    diff = s_wmv.oldButtonState ^ mstate;
    if ( s_wmv.oldButtonState != mstate )
    {
      BG_EvalVehicleName();
      for ( button = 0; button < 5; ++button )
      {
        if ( (diff & (1 << button)) != 0 )
          Sys_QueEvent(time, SE_KEY, button + 200, (mstate & (1 << button)) != 0, 0, 0);
      }
      s_wmv.oldButtonState = mstate;
    }
  }
}

void __cdecl IN_SetCursorPos(unsigned int x, unsigned int y)
{
  tagPOINT curPos; // [esp+0h] [ebp-8h] BYREF

  curPos = (tagPOINT)__PAIR64__(y, x);
  ClientToScreen(g_wv.hWnd, &curPos);
  SetCursorPos(curPos.x, curPos.y);
  s_wmv.oldPos = curPos;
}

void __cdecl IN_ShowSystemCursor(bool show)
{
  int actualShow; // [esp+0h] [ebp-8h]
  int desiredShow; // [esp+4h] [ebp-4h]

  g_showCursor = show;
  desiredShow = show - 1;
  for ( actualShow = ShowCursor(show); actualShow != desiredShow; actualShow = ShowCursor(actualShow < desiredShow) )
    ;
}

void __cdecl IN_Startup()
{
  IN_StartupMouse();
  IN_StartupGamepads();
  Dvar_ClearModified(in_mouse);
}

void __cdecl IN_Shutdown()
{
  IN_DeactivateMouse();
}

void __cdecl IN_Init()
{
  in_mouse = _Dvar_RegisterBool("in_mouse", 1, 0x21u, "Initialize the mouse drivers");
  IN_Startup();
}

void __cdecl IN_Activate(int active)
{
  in_appactive = active;
  if ( active )
    IN_ActivateMouse(1);
  else
    IN_DeactivateMouse();
}

void __cdecl IN_Frame()
{
  if ( Dvar_GetBool("ClickToContinue") )
    PostMessageA(g_wv.hWnd, 0x201u, 1u, 0);
  if ( s_wmv.mouseInitialized )
  {
    if ( in_appactive )
    {
      IN_ActivateMouse(0);
      IN_MouseMove();
      if ( IN_IsForegroundWindow() )
        IN_GamepadsMove();
    }
    else
    {
      IN_DeactivateMouse();
    }
  }
}

void IN_GamepadsMove()
{
  int ry; // [esp+0h] [ebp-2Ch]
  unsigned int butIndex; // [esp+4h] [ebp-28h]
  int controllerIndex; // [esp+8h] [ebp-24h]
  int rightTrig; // [esp+Ch] [ebp-20h]
  int lx; // [esp+14h] [ebp-18h]
  char gpadPresent; // [esp+1Bh] [ebp-11h]
  unsigned inttime; // [esp+1Ch] [ebp-10h]
  int rx; // [esp+20h] [ebp-Ch]
  int leftTrig; // [esp+24h] [ebp-8h]
  int ly; // [esp+28h] [ebp-4h]

  gpadPresent = 0;
  GPad_UpdateAll();
  time = Sys_Milliseconds();
  for ( controllerIndex = 0; controllerIndex < 1; ++controllerIndex )
  {
    if ( GPad_IsActive(controllerIndex) )
    {
      gpadPresent = 1;
      lx = (int)(GPad_GetStick(controllerIndex, GPAD_LX) * 65535.0);
      ly = (int)(GPad_GetStick(controllerIndex, GPAD_LY) * 65535.0);
      rx = (int)(GPad_GetStick(controllerIndex, GPAD_RX) * 65535.0);
      ry = (int)(GPad_GetStick(controllerIndex, GPAD_RY) * 65535.0);
      leftTrig = (int)(GPad_GetButton(controllerIndex, GPAD_L_TRIG) * 65535.0);
      rightTrig = (int)(GPad_GetButton(controllerIndex, GPAD_R_TRIG) * 65535.0);
      CL_GamepadEvent(controllerIndex, 0, rx);
      CL_GamepadEvent(controllerIndex, 1u, ry);
      CL_GamepadEvent(controllerIndex, 2u, lx);
      CL_GamepadEvent(controllerIndex, 3u, ly);
      CL_GamepadEvent(controllerIndex, 5u, leftTrig);
      CL_GamepadEvent(controllerIndex, 4u, rightTrig);
      for ( butIndex = 0; butIndex < 0x10; ++butIndex )
      {
        if ( GPad_IsButtonPressed(controllerIndex, *(&buttonList + 2 * butIndex)) )
        {
          CL_GamepadButtonEventForPort(
            controllerIndex,
            dword_E0FC74[2 * butIndex],
            1,
            time,
            *(&buttonList + 2 * butIndex));
        }
        else if ( GPad_ButtonRequiresUpdates(controllerIndex, *(&buttonList + 2 * butIndex)) )
        {
          CL_GamepadButtonEventForPort(
            controllerIndex,
            dword_E0FC74[2 * butIndex],
            2,
            time,
            *(&buttonList + 2 * butIndex));
        }
        else if ( GPad_IsButtonReleased(controllerIndex, *(&buttonList + 2 * butIndex)) )
        {
          CL_GamepadButtonEventForPort(
            controllerIndex,
            dword_E0FC74[2 * butIndex],
            0,
            time,
            *(&buttonList + 2 * butIndex));
        }
      }
    }
  }
  if ( gpad_present && gpad_present->current.color[0] != gpadPresent )
    Dvar_SetBool((dvar_s *)gpad_present, gpadPresent);
}

void IN_MouseMove()
{
  int v0; // [esp+0h] [ebp-10h]
  tagPOINT curPos; // [esp+4h] [ebp-Ch] BYREF
  int dy; // [esp+Ch] [ebp-4h]

  if ( !s_wmv.mouseInitialized
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\win32\\win_input.cpp",
          475,
          0,
          "%s",
          "s_wmv.mouseInitialized") )
  {
    __debugbreak();
  }
  if ( !r_fullscreen
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\win32\\win_input.cpp", 476, 0, "%s", "r_fullscreen") )
  {
    __debugbreak();
  }
  if ( IN_IsForegroundWindow() )
  {
    GetCursorPos(&curPos);
    if ( r_fullscreen->current.enabled )
      IN_ClampMouseMove(&curPos);
    v0 = curPos.x - s_wmv.oldPos.x;
    dy = curPos.y - s_wmv.oldPos.y;
    if ( curPos.x != s_wmv.oldPos.x || dy )
      BG_EvalVehicleName();
    s_wmv.oldPos = curPos;
    ScreenToClient(g_wv.hWnd, &curPos);
    g_wv.recenterMouse = CL_MouseEvent(curPos.x, curPos.y, v0, dy);
    if ( g_wv.recenterMouse && (v0 || dy) )
    {
      IN_RecenterMouse();
      s_wmv.oldPos.x = window_center_x;
      s_wmv.oldPos.y = window_center_y;
    }
  }
}

void __cdecl IN_ClampMouseMove(tagPOINT *curPos)
{
  bool isClamped; // [esp+3h] [ebp-11h]
  tagRECT rc; // [esp+4h] [ebp-10h] BYREF

  GetWindowRect(g_wv.hWnd, &rc);
  isClamped = 0;
  if ( curPos->x >= rc.left )
  {
    if ( curPos->x >= rc.right )
    {
      curPos->x = rc.right - 1;
      isClamped = 1;
    }
  }
  else
  {
    curPos->x = rc.left;
    isClamped = 1;
  }
  if ( curPos->y >= rc.top )
  {
    if ( curPos->y >= rc.bottom )
    {
      curPos->y = rc.bottom - 1;
      isClamped = 1;
    }
  }
  else
  {
    curPos->y = rc.top;
    isClamped = 1;
  }
  if ( isClamped )
    SetCursorPos(curPos->x, curPos->y);
}

