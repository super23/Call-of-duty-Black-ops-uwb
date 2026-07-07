// Reference extract from CoDBORadiant.decompile.c — NOT in default CoDBORadiant build.
// Hand-port into MainFrame.cpp or enable via tools/CodBORadiant RAD_DECOMP_MONOLITH.
#include "../../../tools/CodBORadiant/common/rad_decomp_internal.h"
#ifdef RAD_ENABLE_DECOMP
#ifndef __thiscall
#define __thiscall __cdecl
#endif
#define this this_param

//----- (0042C390) --------------------------------------------------------
CFrameWnd *__stdcall sub_42C390(CFrameWnd *a1)
{
  CFrameWnd::CFrameWnd(a1);
  *(_DWORD *)a1 = ((void*)0);
  CStatusBar::CStatusBar((CFrameWnd *)((char *)a1 + 212));
  CToolBar::CToolBar((CFrameWnd *)((char *)a1 + 364));
  *((_DWORD *)a1 + 91) = ((void*)0);
  CDialogBar::CDialogBar((CFrameWnd *)((char *)a1 + 548));
  sub_4719B0((CFrameWnd *)((char *)a1 + 712));
  CSplitterWnd::CSplitterWnd((CFrameWnd *)((char *)a1 + 1320));
  CSplitterWnd::CSplitterWnd((CFrameWnd *)((char *)a1 + 1540));
  CSplitterWnd::CSplitterWnd((CFrameWnd *)((char *)a1 + 1760));
  `eh vector constructor iterator'(
    (char *)a1 + 2008,
    4u,
    15,
    (void (__thiscall *)(void *))sub_402890,
    (void (__thiscall *)(void *))sub_402850);
  *((_BYTE *)a1 + 2080) = 0;
  *((_BYTE *)a1 + 2081) = 0;
  dword_27C32F0 = a1;
  *((_DWORD *)a1 + 495) = 0;
  *((_DWORD *)a1 + 496) = 0;
  *((_DWORD *)a1 + 497) = 0;
  *((_DWORD *)a1 + 498) = 0;
  *((_DWORD *)a1 + 500) = 0;
  *((_DWORD *)a1 + 517) = 0;
  *((_BYTE *)a1 + 2072) = 1;
  *((_DWORD *)a1 + 519) = 0;
  return a1;
}
// 6E905C: using guessed type void *CLstToolBar::;
// 6ECF94: using guessed type void *CMainFrame::;

//----- (0042F710) --------------------------------------------------------
int __thiscall sub_42F710(int this, int a2, int a3)
{
  int v4; // eax
  struct AFX_MODULE_STATE *ModuleState; // eax
  struct AFX_MODULE_STATE *v6; // eax
  CDialog *v7; // eax
  CDialog *v8; // eax
  int v9; // eax
  HWND v10; // eax
  HGDIOBJ StockObject; // eax
  HWND v12; // esi
  int v13; // eax
  CWnd *v14; // eax
  CWnd *v15; // eax
  int *v16; // ecx
  CWnd *v17; // eax
  CWnd *v18; // esi
  CWnd *v19; // ecx
  int *v20; // edx
  void *v21; // eax
  int v22; // eax
  int *v23; // ecx
  CWnd *v24; // eax
  CWnd *v25; // eax
  HWND v26; // eax
  HWND v27; // eax
  HWND v28; // eax
  HWND v29; // eax
  HWND v30; // eax
  struct CWnd *v31; // eax
  int v32; // eax
  int v33; // edx
  struct CWnd *v34; // eax
  CWnd *v35; // eax
  CWnd *v36; // eax
  CWnd *v37; // eax
  HWND v38; // eax
  CWnd *v39; // eax
  CWnd *v40; // esi
  CWnd *v41; // ecx
  HWND v42; // eax
  HWND v43; // eax
  HWND v44; // eax
  int v45; // ebx
  int v46; // eax
  struct CWnd *v47; // eax
  int v48; // eax
  int v49; // edx
  struct CWnd *v50; // eax
  CWnd *v51; // eax
  struct CWnd *v52; // eax
  int v53; // eax
  void (__thiscall *v54)(int, int, _DWORD, char **, int, int, int); // edx
  struct CWnd *v55; // eax
  HWND v56; // eax
  HWND v57; // esi
  HWND v58; // eax
  LONG WindowLongA; // eax
  HWND v60; // ecx
  double v61; // st7
  int v62; // esi
  double v63; // st5
  HWND v64; // eax
  HWND v65; // eax
  HWND v66; // eax
  HGDIOBJ v67; // eax
  int v68; // eax
  struct CWnd *Pane; // [esp+98h] [ebp-D0h]
  HWND v71; // [esp+A4h] [ebp-C4h]
  LONG dwNewLong; // [esp+BCh] [ebp-ACh]
  float dwNewLonga; // [esp+BCh] [ebp-ACh]
  int v74; // [esp+C0h] [ebp-A8h] BYREF
  int v75; // [esp+C4h] [ebp-A4h] BYREF
  int v76; // [esp+C8h] [ebp-A0h]
  DWORD v77; // [esp+CCh] [ebp-9Ch] BYREF
  struct tagRECT Rect; // [esp+D0h] [ebp-98h] BYREF
  struct tagRECT v79; // [esp+E0h] [ebp-88h] BYREF
  _DWORD v80[4]; // [esp+F4h] [ebp-74h] BYREF
  WINDOWPLACEMENT wndpl; // [esp+104h] [ebp-64h] BYREF
  struct tagWNDCLASSA WndClass; // [esp+130h] [ebp-38h] BYREF
  int v83; // [esp+164h] [ebp-4h]

  dword_27E1844 = this;
  if ( this )
    hWndTo = *(HWND *)(this + 32);
  else
    hWndTo = 0;
  v4 = (*(int (**)(void))(*(_DWORD *)this + 340))();
  if ( v4 )
    dword_27E1864 = *(HWND *)(v4 + 32);
  else
    dword_27E1864 = 0;
  *(_DWORD *)(this + 2004) = off_74CD1C[114];
  ModuleState = AfxGetModuleState();
  GetClassInfoA(*((HINSTANCE *)ModuleState + 2), "AfxFrameOrView70sd", &WndClass);
  v6 = AfxGetModuleState();
  sub_4B3600(*((HINSTANCE *)v6 + 2));
  v7 = (CDialog *)operator new(0x258u);
  v83 = 0;
  if ( v7 )
    v8 = sub_418D50(v7);
  else
    v8 = 0;
  v83 = -1;
  *(_DWORD *)(this + 1996) = v8;
  (*(void (__thiscall **)(CDialog *, int, int))(*(_DWORD *)v8 + 308))(v8, 180, this);
  v9 = *(_DWORD *)(this + 1996);
  if ( v9 )
    v10 = *(HWND *)(v9 + 32);
  else
    v10 = 0;
  dword_27E1868 = v10;
  SetParent(v10, hWndNewParent);
  CWnd::ShowWindow(*(CWnd **)(this + 1996), 5);
  StockObject = GetStockObject(17);
  SendMessageA(dword_27E1868, 0x30u, (WPARAM)StockObject, 1);
  v12 = *(HWND *)(this + 32);
  wndpl.length = 44;
  v74 = 44;
  if ( sub_4B7120((DWORD *)&v74, "Radiant::MainWindowPlace", (BYTE *)&wndpl) )
    SetWindowPlacement(v12, &wndpl);
  else
    sub_4B7200(*(HWND *)(this + 32));
  CDialog::Create((CDialog *)dword_27C39C8, (LPCSTR)0xBB, 0);
  CWnd::ShowWindow((CWnd *)dword_27C39C8, 0);
  CDialog::Create((CDialog *)&dword_27C3EE0, (LPCSTR)0xE2, 0);
  CWnd::ShowWindow((CWnd *)&dword_27C3EE0, 0);
  CDialog::Create((CDialog *)&dword_27C41C0, (LPCSTR)0xE4, 0);
  CWnd::ShowWindow((CWnd *)&dword_27C41C0, 0);
  CDialog::Create((CDialog *)&dword_27C4440, (LPCSTR)0xFB, 0);
  CWnd::ShowWindow((CWnd *)&dword_27C4440, 0);
  CDialog::Create((CDialog *)dword_27C4658, (LPCSTR)0xE5, 0);
  CWnd::ShowWindow((CWnd *)dword_27C4658, 0);
  CDialog::Create((CDialog *)&dword_27C47E0, (LPCSTR)0xE6, 0);
  CWnd::ShowWindow((CWnd *)&dword_27C47E0, 0);
  SetParent(dword_27C4800, hWndNewParent);
  CWnd::ShowWindow((CWnd *)&dword_27C47E0, 5);
  sub_41EF00();
  v71 = *(HWND *)(this + 32);
  v80[0] = 5;
  v80[1] = 25;
  v80[2] = 100;
  v80[3] = 100;
  GetClientRect(v71, &Rect);
  v13 = off_74CD1C[114];
  if ( !v13 || v13 == 3 )
  {
    (*(void (__thiscall **)(int, int, int, int, int, int))(*(_DWORD *)(this + 1320) + 312))(
      this + 1320,
      this,
      2,
      1,
      1342177280,
      59648);
    v45 = this + 1540;
    (*(void (__thiscall **)(int, int, int, int, int, int))(*(_DWORD *)(this + 1540) + 312))(
      this + 1540,
      this + 1320,
      1,
      3,
      1342177280,
      59648);
    (*(void (__thiscall **)(int, int, int, int, int, int))(*(_DWORD *)(this + 1760) + 312))(
      this + 1760,
      this + 1540,
      2,
      1,
      1342177280,
      59648);
    (*(void (__thiscall **)(int, int, _DWORD, char **, int, int, int))(*(_DWORD *)(this + 1320) + 316))(
      this + 1320,
      1,
      0,
      &off_6E66A4,
      25,
      100,
      a3);
    Pane = CSplitterWnd::GetPane((CSplitterWnd *)(this + 1320), 1, 0);
    v46 = __RTDynamicCast(
            (int)Pane,
            0,
            (struct _s_RTTICompleteObjectLocator *)&CWnd `RTTI Type Descriptor',
            &CEdit `RTTI Type Descriptor',
            0);
    dword_27C32EC = v46;
    if ( v46 )
      dword_27E1850 = *(HWND *)(v46 + 32);
    (*(void (__thiscall **)(int, _DWORD, _DWORD, char **, int, int, int))(*(_DWORD *)(this + 1760) + 316))(
      this + 1760,
      0,
      0,
      &off_6E4F58,
      25,
      100,
      a3);
    v47 = CSplitterWnd::GetPane((CSplitterWnd *)(this + 1760), 0, 0);
    v48 = __RTDynamicCast(
            (int)v47,
            0,
            (struct _s_RTTICompleteObjectLocator *)&CWnd `RTTI Type Descriptor',
            &CCamWnd `RTTI Type Descriptor',
            0);
    v49 = *(_DWORD *)v45;
    *(_DWORD *)(this + 1984) = v48;
    (*(void (__thiscall **)(int, _DWORD, int, char **, int, int, int))(v49 + 316))(
      this + 1540,
      0,
      1,
      &off_6F4E64,
      25,
      100,
      a3);
    v50 = CSplitterWnd::GetPane((CSplitterWnd *)(this + 1540), 0, 1);
    v51 = (CWnd *)__RTDynamicCast(
                    (int)v50,
                    0,
                    (struct _s_RTTICompleteObjectLocator *)&CWnd `RTTI Type Descriptor',
                    &CXYWnd `RTTI Type Descriptor',
                    0);
    *(_DWORD *)(this + 1980) = v51;
    sub_487B40(v51, (char *)2);
    *(_DWORD *)(*(_DWORD *)(this + 1984) + 84) = *(_DWORD *)(this + 1980);
    (*(void (__thiscall **)(int, _DWORD, int, char **, int, int, int))(*(_DWORD *)v45 + 316))(
      this + 1540,
      0,
      2,
      &off_6F5C9C,
      25,
      100,
      a3);
    v52 = CSplitterWnd::GetPane((CSplitterWnd *)(this + 1540), 0, 2);
    v53 = __RTDynamicCast(
            (int)v52,
            0,
            (struct _s_RTTICompleteObjectLocator *)&CWnd `RTTI Type Descriptor',
            &CZWnd `RTTI Type Descriptor',
            0);
    v54 = *(void (__thiscall **)(int, int, _DWORD, char **, int, int, int))(*(_DWORD *)(this + 1760) + 316);
    *(_DWORD *)(this + 1992) = v53;
    v54(this + 1760, 1, 0, &off_6F27D8, 25, 100, a3);
    v55 = CSplitterWnd::GetPane((CSplitterWnd *)(this + 1760), 1, 0);
    *(_DWORD *)(this + 1988) = __RTDynamicCast(
                                 (int)v55,
                                 0,
                                 (struct _s_RTTICompleteObjectLocator *)&CWnd `RTTI Type Descriptor',
                                 &CTexWnd `RTTI Type Descriptor',
                                 0);
    sub_42E9C0((HWND *)this);
    if ( !off_74CD1C[114] )
    {
      v56 = (HWND)CSplitterWnd::GetPane((CSplitterWnd *)(this + 1540), 0, 2);
      v74 = (int)v56;
      if ( v56 )
        v56 = (HWND)*((_DWORD *)v56 + 8);
      v57 = (HWND)(this + 1760);
      dwNewLong = GetWindowLongA(v56, -12);
      if ( this == -1760 )
        v58 = 0;
      else
        v58 = *(HWND *)(this + 1792);
      WindowLongA = GetWindowLongA(v58, -12);
      if ( *(float *)&v74 == 0.0 )
        v60 = 0;
      else
        v60 = *(HWND *)(v74 + 32);
      SetWindowLongA(v60, -12, WindowLongA);
      if ( this != -1760 )
        v57 = *(HWND *)(this + 1792);
      SetWindowLongA(v57, -12, dwNewLong);
    }
    CSplitterWnd::SetRowInfo((CSplitterWnd *)(this + 1320), 0, (int)((double)(Rect.bottom - Rect.top) * 0.85), 50);
    CSplitterWnd::SetRowInfo((CSplitterWnd *)(this + 1320), 1, (int)((double)(Rect.bottom - Rect.top) * 0.15), 5);
    v61 = 0.25;
    v62 = off_74CD1C[114];
    if ( v62 )
      v63 = 0.25;
    else
      v63 = 0.05;
    *(float *)&v74 = v63;
    if ( v62 )
      v61 = 0.05;
    *(double *)&v79.left = v61;
    CSplitterWnd::SetColumnInfo(
      (CSplitterWnd *)(this + 1540),
      0,
      (int)((double)(Rect.right - Rect.left) * *(float *)&v74),
      10);
    CSplitterWnd::SetColumnInfo((CSplitterWnd *)(this + 1540), 1, (int)((double)(Rect.right - Rect.left) * 0.7), 100);
    dwNewLonga = *(double *)&v79.left;
    CSplitterWnd::SetColumnInfo(
      (CSplitterWnd *)(this + 1540),
      2,
      (int)((double)(Rect.right - Rect.left) * dwNewLonga),
      25);
    CSplitterWnd::SetRowInfo((CSplitterWnd *)(this + 1760), 1, (int)((double)(Rect.bottom - Rect.top) * 0.85 * 0.4), 15);
    CSplitterWnd::SetRowInfo((CSplitterWnd *)(this + 1760), 0, (int)((double)(Rect.bottom - Rect.top) * 0.85 * 0.6), 15);
    v64 = (HWND)(this + 1320);
    if ( this != -1320 )
      v64 = *(HWND *)(this + 1352);
    sub_4B7200(v64);
    if ( this == -1540 )
      v65 = 0;
    else
      v65 = *(HWND *)(this + 1572);
    sub_4B7200(v65);
    if ( this == -1760 )
      v66 = 0;
    else
      v66 = *(HWND *)(this + 1792);
    sub_4B7200(v66);
    ShowWindow(hWndNewParent, 0);
    v77 = 8;
    if ( sub_4B7120(&v77, "Radiant::Split::Row_0", (BYTE *)&v75) )
      CSplitterWnd::SetRowInfo((CSplitterWnd *)(this + 1320), 0, v76, v75);
    if ( sub_4B7120(&v77, "Radiant::Split::Row_1", (BYTE *)&v75) )
      CSplitterWnd::SetRowInfo((CSplitterWnd *)(this + 1320), 1, v76, v75);
    if ( sub_4B7120(&v77, "Radiant::Split2::Col_0", (BYTE *)&v75) )
      CSplitterWnd::SetColumnInfo((CSplitterWnd *)(this + 1540), 0, v76, v75);
    if ( sub_4B7120(&v77, "Radiant::Split2::Col_1", (BYTE *)&v75) )
      CSplitterWnd::SetColumnInfo((CSplitterWnd *)(this + 1540), 1, v76, v75);
    if ( sub_4B7120(&v77, "Radiant::Split2::Col_2", (BYTE *)&v75) )
      CSplitterWnd::SetColumnInfo((CSplitterWnd *)(this + 1540), 2, v76, v75);
    if ( sub_4B7120(&v77, "Radiant::Split3::Row_0", (BYTE *)&v75) )
      CSplitterWnd::SetRowInfo((CSplitterWnd *)(this + 1760), 0, v76, v75);
    if ( sub_4B7120(&v77, "Radiant::Split3::Row_1", (BYTE *)&v75) )
      CSplitterWnd::SetRowInfo((CSplitterWnd *)(this + 1760), 1, v76, v75);
    (*(void (__thiscall **)(int))(*(_DWORD *)(this + 1320) + 320))(this + 1320);
    (*(void (__thiscall **)(int))(*(_DWORD *)v45 + 320))(this + 1540);
    (*(void (__thiscall **)(int))(*(_DWORD *)(this + 1760) + 320))(this + 1760);
  }
  else if ( v13 == 1 )
  {
    v14 = (CWnd *)operator new(0x220u);
    v83 = 1;
    if ( v14 )
      v15 = sub_402B90(v14);
    else
      v15 = 0;
    v83 = -1;
    v16 = off_74CD1C;
    *(_DWORD *)(this + 1984) = v15;
    (*(void (__thiscall **)(CWnd *, const CHAR *, const CHAR *, int, _DWORD *, int, int, _DWORD))(*(_DWORD *)v15 + 84))(
      v15,
      "QCamera",
      Default,
      v16[173] != 0 ? 114229248 : 1455882240,
      v80,
      this,
      1234,
      0);
    v17 = (CWnd *)operator new(0x5Cu);
    v18 = v17;
    v83 = 2;
    if ( v17 )
    {
      CWnd::CWnd(v17);
      *(_DWORD *)v18 = ((void*)0);
      v19 = v18;
    }
    else
    {
      v19 = 0;
    }
    v83 = -1;
    v20 = off_74CD1C;
    *(_DWORD *)(this + 1992) = v19;
    (*(void (__thiscall **)(CWnd *, const CHAR *, const CHAR *, int, _DWORD *, int, int, _DWORD))(*(_DWORD *)v19 + 84))(
      v19,
      "QZ",
      Default,
      v20[173] != 0 ? 114229248 : 1455882240,
      v80,
      this,
      1238,
      0);
    v21 = operator new(0x14Cu);
    v83 = 3;
    if ( v21 )
      v22 = sub_47BB70((int)v21);
    else
      v22 = 0;
    v83 = -1;
    v23 = off_74CD1C;
    *(_DWORD *)(this + 1980) = v22;
    (*(void (__thiscall **)(int, const CHAR *, const CHAR *, int, _DWORD *, int, int, _DWORD))(*(_DWORD *)v22 + 84))(
      v22,
      off_6EC808,
      Default,
      v23[173] != 0 ? 114229248 : 1455882240,
      v80,
      this,
      1235,
      0);
    sub_487B40(*(CWnd **)(this + 1980), (char *)2);
    *(_DWORD *)(*(_DWORD *)(this + 1984) + 84) = *(_DWORD *)(this + 1980);
    v24 = (CWnd *)operator new(0xA8u);
    v83 = 4;
    if ( v24 )
      v25 = sub_475A10(v24);
    else
      v25 = 0;
    v83 = -1;
    *(_DWORD *)(this + 1988) = v25;
    (*(void (__thiscall **)(CWnd *, const CHAR *, const CHAR *, int, _DWORD *, int, int, _DWORD))(*(_DWORD *)v25 + 84))(
      v25,
      "QTEX",
      Default,
      1442840576,
      v80,
      this,
      1239,
      0);
    v26 = *(HWND *)(this + 1988);
    if ( v26 )
      v26 = (HWND)*((_DWORD *)v26 + 8);
    SetParent(v26, hWndNewParent);
    v27 = *(HWND *)(this + 1980);
    if ( off_74CD1C[173] )
    {
      if ( v27 )
        v27 = (HWND)*((_DWORD *)v27 + 8);
      sub_42EE60(v27);
      v28 = *(HWND *)(this + 1984);
      if ( v28 )
        v28 = (HWND)*((_DWORD *)v28 + 8);
      sub_42EE60(v28);
      v29 = *(HWND *)(this + 1992);
      if ( v29 )
        v29 = (HWND)*((_DWORD *)v29 + 8);
    }
    else
    {
      if ( v27 )
        v27 = (HWND)*((_DWORD *)v27 + 8);
      sub_42EE60(v27);
      v30 = *(HWND *)(this + 1984);
      if ( v30 )
        v30 = (HWND)*((_DWORD *)v30 + 8);
      sub_42EE60(v30);
      v29 = *(HWND *)(this + 1992);
      if ( v29 )
        v29 = (HWND)*((_DWORD *)v29 + 8);
    }
    sub_42EE60(v29);
    if ( !off_74CD1C[147] )
      CWnd::ShowWindow(*(CWnd **)(this + 1992), 0);
    sub_42E9C0((HWND *)this);
  }
  else
  {
    (*(void (__thiscall **)(int, int, int, int, int, int))(*(_DWORD *)(this + 1320) + 312))(
      this + 1320,
      this,
      2,
      2,
      1342177280,
      59648);
    (*(void (__thiscall **)(int, _DWORD, _DWORD, char **, int, int, int))(*(_DWORD *)(this + 1320) + 316))(
      this + 1320,
      0,
      0,
      &off_6E4F58,
      25,
      100,
      a3);
    v31 = CSplitterWnd::GetPane((CSplitterWnd *)(this + 1320), 0, 0);
    v32 = __RTDynamicCast(
            (int)v31,
            0,
            (struct _s_RTTICompleteObjectLocator *)&CWnd `RTTI Type Descriptor',
            &CCamWnd `RTTI Type Descriptor',
            0);
    v33 = *(_DWORD *)(this + 1320);
    *(_DWORD *)(this + 1984) = v32;
    (*(void (__thiscall **)(int, _DWORD, int, char **, int, int, int))(v33 + 316))(
      this + 1320,
      0,
      1,
      &off_6F4E64,
      25,
      100,
      a3);
    v34 = CSplitterWnd::GetPane((CSplitterWnd *)(this + 1320), 0, 1);
    v35 = (CWnd *)__RTDynamicCast(
                    (int)v34,
                    0,
                    (struct _s_RTTICompleteObjectLocator *)&CWnd `RTTI Type Descriptor',
                    &CXYWnd `RTTI Type Descriptor',
                    0);
    *(_DWORD *)(this + 1980) = v35;
    sub_487B40(v35, (char *)2);
    (*(void (__thiscall **)(int, int, _DWORD, char **, int, int, int))(*(_DWORD *)(this + 1320) + 316))(
      this + 1320,
      1,
      0,
      &off_6F4E64,
      25,
      100,
      a3);
    (*(void (__thiscall **)(int, int, int, char **, int, int, int))(*(_DWORD *)(this + 1320) + 316))(
      this + 1320,
      1,
      1,
      &off_6F4E64,
      25,
      100,
      a3);
    *(_DWORD *)(*(_DWORD *)(this + 1984) + 84) = *(_DWORD *)(this + 1980);
    v36 = (CWnd *)operator new(0xA8u);
    v83 = 5;
    if ( v36 )
      v37 = sub_475A10(v36);
    else
      v37 = 0;
    v83 = -1;
    *(_DWORD *)(this + 1988) = v37;
    (*(void (__thiscall **)(CWnd *, const CHAR *, const CHAR *, int, _DWORD *, int, int, _DWORD))(*(_DWORD *)v37 + 84))(
      v37,
      "QTEX",
      Default,
      1442840576,
      v80,
      this,
      1237,
      0);
    v38 = *(HWND *)(this + 1988);
    if ( v38 )
      v38 = (HWND)*((_DWORD *)v38 + 8);
    SetParent(v38, hWndNewParent);
    v39 = (CWnd *)operator new(0x5Cu);
    v40 = v39;
    v83 = 6;
    if ( v39 )
    {
      CWnd::CWnd(v39);
      *(_DWORD *)v40 = ((void*)0);
      v41 = v40;
    }
    else
    {
      v41 = 0;
    }
    v83 = -1;
    *(_DWORD *)(this + 1992) = v41;
    (*(void (__thiscall **)(CWnd *, const CHAR *, const CHAR *, int, _DWORD *, int, int, _DWORD))(*(_DWORD *)v41 + 84))(
      v41,
      "QZ",
      Default,
      1455882240,
      v80,
      this,
      1236,
      0);
    CWnd::ShowWindow(*(CWnd **)(this + 1992), 0);
    v42 = *(HWND *)(this + 1988);
    if ( v42 )
      v42 = (HWND)*((_DWORD *)v42 + 8);
    sub_4B7200(v42);
    v43 = *(HWND *)(this + 2000);
    if ( v43 )
      v43 = (HWND)*((_DWORD *)v43 + 8);
    sub_4B7200(v43);
    ShowWindow(hWndNewParent, 0);
    sub_42E9C0((HWND *)this);
    GetClientRect(*(HWND *)(this + 32), &v79);
    v74 = v79.bottom - v79.top;
    CSplitterWnd::SetRowInfo((CSplitterWnd *)(this + 1320), 0, (int)((double)(v79.bottom - v79.top) * 0.5), 50);
    v74 = v79.bottom - v79.top;
    CSplitterWnd::SetRowInfo((CSplitterWnd *)(this + 1320), 1, (int)((double)(v79.bottom - v79.top) * 0.5), 50);
    v74 = v79.right - v79.left;
    CSplitterWnd::SetColumnInfo((CSplitterWnd *)(this + 1320), 0, (int)((double)(v79.right - v79.left) * 0.5), 50);
    v74 = v79.right - v79.left;
    CSplitterWnd::SetColumnInfo((CSplitterWnd *)(this + 1320), 1, (int)((double)(v79.right - v79.left) * 0.5), 50);
    if ( this == -1320 )
      v44 = 0;
    else
      v44 = *(HWND *)(this + 1352);
    sub_4B7200(v44);
    (*(void (__thiscall **)(int))(*(_DWORD *)(this + 1320) + 320))(this + 1320);
  }
  CDialog::Create((CDialog *)dword_27C49A8, (LPCSTR)0xFD, 0);
  CWnd::ShowWindow((CWnd *)dword_27C49A8, 0);
  if ( *(_DWORD *)(this + 1988) )
    dword_27C4A1C = *(_DWORD *)(this + 1988);
  if ( dword_27C32EC )
  {
    v67 = GetStockObject(17);
    SendMessageA(*(HWND *)(dword_27C32EC + 32), 0x30u, (WPARAM)v67, 1);
  }
  v68 = *(_DWORD *)(this + 1980);
  if ( v68 )
    *(_BYTE *)(v68 + 84) = 1;
  *(_BYTE *)(this + 2081) = 1;
  sub_472810((void *)dword_2857988);
  return 1;
}
// 6E4F58: using guessed type char *off_6E4F58;
// 6E66A4: using guessed type char *off_6E66A4;
// 6F27D8: using guessed type char *off_6F27D8;
// 6F4E64: using guessed type char *off_6F4E64;
// 6F5C9C: using guessed type char *off_6F5C9C;
// 6F5ECC: using guessed type void *CZWnd::;
// 742F60: using guessed type _UNKNOWN **CWnd `RTTI Type Descriptor';
// 74CD1C: using guessed type int *off_74CD1C;
// 27C32EC: using guessed type int dword_27C32EC;
// 27C39C8: using guessed type int dword_27C39C8[8];
// 27C3EE0: using guessed type int dword_27C3EE0;
// 27C41C0: using guessed type int dword_27C41C0;
// 27C4658: using guessed type int dword_27C4658[8];
// 27C49A8: using guessed type int dword_27C49A8[8];
// 27C4A1C: using guessed type int dword_27C4A1C;
// 27E1844: using guessed type int dword_27E1844;
// 2857988: using guessed type int dword_2857988;



#endif
