// Reference extract from CoDBORadiant.decompile.c — NOT in default CoDBORadiant build.
#include "../../../tools/CodBORadiant/common/rad_decomp_internal.h"
#ifdef RAD_ENABLE_DECOMP
#ifndef __thiscall
#define __thiscall __cdecl
#endif
#define this this_param

//----- (00465600) --------------------------------------------------------
char *sub_465600()
{
  int *v0; // esi
  void *v1; // eax
  struct AFX_MODULE_STATE *ModuleState; // eax
  unsigned int ProfileIntA; // eax
  struct AFX_MODULE_STATE *v4; // eax
  struct AFX_MODULE_STATE *v5; // eax
  struct AFX_MODULE_STATE *v6; // eax
  struct AFX_MODULE_STATE *v7; // eax
  struct AFX_MODULE_STATE *v8; // eax
  char **ProfileStringA; // eax
  int v10; // eax
  char **v11; // eax
  int v12; // eax
  struct AFX_MODULE_STATE *v13; // eax
  struct AFX_MODULE_STATE *v14; // eax
  struct AFX_MODULE_STATE *v15; // eax
  struct AFX_MODULE_STATE *v16; // eax
  struct AFX_MODULE_STATE *v17; // eax
  struct AFX_MODULE_STATE *v18; // eax
  struct AFX_MODULE_STATE *v19; // eax
  struct AFX_MODULE_STATE *v20; // eax
  struct AFX_MODULE_STATE *v21; // eax
  struct AFX_MODULE_STATE *v22; // eax
  char **v23; // eax
  int v24; // eax
  char *v25; // edi
  struct AFX_MODULE_STATE *v26; // eax
  unsigned int v27; // eax
  struct AFX_MODULE_STATE *v28; // eax
  struct AFX_MODULE_STATE *v29; // eax
  struct AFX_MODULE_STATE *v30; // eax
  struct AFX_MODULE_STATE *v31; // eax
  struct AFX_MODULE_STATE *v32; // eax
  struct AFX_MODULE_STATE *v33; // eax
  struct AFX_MODULE_STATE *v34; // eax
  struct AFX_MODULE_STATE *v35; // eax
  struct AFX_MODULE_STATE *v36; // eax
  struct AFX_MODULE_STATE *v37; // eax
  char **v38; // eax
  int v39; // eax
  struct AFX_MODULE_STATE *v40; // eax
  struct AFX_MODULE_STATE *v41; // eax
  struct AFX_MODULE_STATE *v42; // eax
  struct AFX_MODULE_STATE *v43; // eax
  struct AFX_MODULE_STATE *v44; // eax
  struct AFX_MODULE_STATE *v45; // eax
  struct AFX_MODULE_STATE *v46; // eax
  struct AFX_MODULE_STATE *v47; // eax
  struct AFX_MODULE_STATE *v48; // eax
  struct AFX_MODULE_STATE *v49; // eax
  struct AFX_MODULE_STATE *v50; // eax
  struct AFX_MODULE_STATE *v51; // eax
  struct AFX_MODULE_STATE *v52; // eax
  struct AFX_MODULE_STATE *v53; // eax
  struct AFX_MODULE_STATE *v54; // eax
  struct AFX_MODULE_STATE *v55; // eax
  char **v56; // eax
  int v57; // eax
  char **v58; // eax
  int v59; // eax
  char **v60; // eax
  int v61; // eax
  char **v62; // eax
  int v63; // eax
  char **v64; // eax
  int v65; // eax
  char **v66; // eax
  int v67; // eax
  char **v68; // eax
  int v69; // eax
  char **v70; // eax
  int v71; // eax
  struct AFX_MODULE_STATE *v72; // eax
  struct AFX_MODULE_STATE *v73; // eax
  struct AFX_MODULE_STATE *v74; // eax
  struct AFX_MODULE_STATE *v75; // eax
  struct AFX_MODULE_STATE *v76; // eax
  struct AFX_MODULE_STATE *v77; // eax
  struct AFX_MODULE_STATE *v78; // eax
  struct AFX_MODULE_STATE *v79; // eax
  unsigned int v80; // eax
  struct AFX_MODULE_STATE *v81; // eax
  struct AFX_MODULE_STATE *v82; // eax
  struct AFX_MODULE_STATE *v83; // eax
  struct AFX_MODULE_STATE *v84; // eax
  struct AFX_MODULE_STATE *v85; // eax
  struct AFX_MODULE_STATE *v86; // eax
  struct AFX_MODULE_STATE *v87; // eax
  struct AFX_MODULE_STATE *v88; // eax
  struct AFX_MODULE_STATE *v89; // eax
  struct AFX_MODULE_STATE *v90; // eax
  struct AFX_MODULE_STATE *v91; // eax
  struct AFX_MODULE_STATE *v92; // eax
  struct AFX_MODULE_STATE *v93; // eax
  struct AFX_MODULE_STATE *v94; // eax
  struct AFX_MODULE_STATE *v95; // eax
  struct AFX_MODULE_STATE *v96; // eax
  struct AFX_MODULE_STATE *v97; // eax
  struct AFX_MODULE_STATE *v98; // eax
  struct AFX_MODULE_STATE *v99; // eax
  struct AFX_MODULE_STATE *v100; // eax
  struct AFX_MODULE_STATE *v101; // eax
  struct AFX_MODULE_STATE *v102; // eax
  struct AFX_MODULE_STATE *v103; // eax
  struct AFX_MODULE_STATE *v104; // eax
  struct AFX_MODULE_STATE *v105; // eax
  struct AFX_MODULE_STATE *v106; // eax
  struct AFX_MODULE_STATE *v107; // eax
  struct AFX_MODULE_STATE *v108; // eax
  struct AFX_MODULE_STATE *v109; // eax
  struct AFX_MODULE_STATE *v110; // eax
  struct AFX_MODULE_STATE *v111; // eax
  struct AFX_MODULE_STATE *v112; // eax
  struct AFX_MODULE_STATE *v113; // eax
  struct AFX_MODULE_STATE *v114; // eax
  struct AFX_MODULE_STATE *v115; // eax
  struct AFX_MODULE_STATE *v116; // eax
  struct AFX_MODULE_STATE *v117; // eax
  struct AFX_MODULE_STATE *v118; // eax
  struct AFX_MODULE_STATE *v119; // eax
  char **v120; // eax
  int v121; // eax
  char *result; // eax
  char *String; // [esp+30h] [ebp-14h] BYREF
  int v124; // [esp+34h] [ebp-10h] BYREF
  int v125; // [esp+40h] [ebp-4h]

  v0 = off_74CD1C;
  v1 = sub_5A634E();
  if ( !v1 )
    sub_4025A0(-2147467259);
  String = (char *)((*(int (__thiscall **)(void *))(*(_DWORD *)v1 + 12))(v1) + 16);
  v125 = 0;
  ModuleState = AfxGetModuleState();
  ProfileIntA = CWinApp::GetProfileIntA(*((CWinApp **)ModuleState + 1), (DWORD)"Prefs", "MouseButtons", 1);
  v0[113] = ProfileIntA;
  v0[193] = (ProfileIntA != 0) + 2;
  v4 = AfxGetModuleState();
  v0[114] = CWinApp::GetProfileIntA(*((CWinApp **)v4 + 1), (DWORD)"Prefs", "QE4StyleWindows", 0);
  v5 = AfxGetModuleState();
  v0[173] = CWinApp::GetProfileIntA(*((CWinApp **)v5 + 1), (DWORD)"Prefs", "FloatingWindows", 0);
  v6 = AfxGetModuleState();
  v0[115] = CWinApp::GetProfileIntA(*((CWinApp **)v6 + 1), (DWORD)"Prefs", "TextureLock", 1);
  v7 = AfxGetModuleState();
  v0[153] = CWinApp::GetProfileIntA(*((CWinApp **)v7 + 1), (DWORD)"Prefs", "RotateLock", 1);
  v8 = AfxGetModuleState();
  v0[116] = CWinApp::GetProfileIntA(*((CWinApp **)v8 + 1), (DWORD)"Prefs", "LightmapLock", 0);
  AfxGetModuleState();
  ProfileStringA = (char **)CWinApp::GetProfileStringA((int)&v124, "Prefs", "LastProject", (int)Default);
  LOBYTE(v125) = 1;
  sub_41A530((volatile signed __int32 **)v0 + 124, ProfileStringA);
  LOBYTE(v125) = 0;
  v10 = v124 - 16;
  if ( _InterlockedDecrement((volatile signed __int32 *)(v124 - 16 + 12)) <= 0 )
    (*(void (__stdcall **)(int))(**(_DWORD **)v10 + 4))(v10);
  AfxGetModuleState();
  v11 = (char **)CWinApp::GetProfileStringA((int)&v124, "Prefs", "LastMap", (int)Default);
  LOBYTE(v125) = 2;
  sub_41A530((volatile signed __int32 **)v0 + 125, v11);
  LOBYTE(v125) = 0;
  v12 = v124 - 16;
  if ( _InterlockedDecrement((volatile signed __int32 *)(v124 - 16 + 12)) <= 0 )
    (*(void (__stdcall **)(int))(**(_DWORD **)v12 + 4))(v12);
  v13 = AfxGetModuleState();
  v0[117] = CWinApp::GetProfileIntA(*((CWinApp **)v13 + 1), (DWORD)"Prefs", "LoadLast", 1);
  v14 = AfxGetModuleState();
  v0[118] = CWinApp::GetProfileIntA(*((CWinApp **)v14 + 1), (DWORD)"Internals", "RunBefore", 0);
  v15 = AfxGetModuleState();
  v0[119] = CWinApp::GetProfileIntA(*((CWinApp **)v15 + 1), (DWORD)"Prefs", "CameraMode", 1);
  v16 = AfxGetModuleState();
  v0[120] = CWinApp::GetProfileIntA(*((CWinApp **)v16 + 1), (DWORD)"Prefs", "CameraMasked", 1);
  v17 = AfxGetModuleState();
  v0[126] = CWinApp::GetProfileIntA(*((CWinApp **)v17 + 1), (DWORD)"Prefs", "NewFaceGrab", 1);
  v18 = AfxGetModuleState();
  v0[127] = CWinApp::GetProfileIntA(*((CWinApp **)v18 + 1), (DWORD)"Prefs", "NewRightClick", 1);
  v19 = AfxGetModuleState();
  v0[128] = CWinApp::GetProfileIntA(*((CWinApp **)v19 + 1), (DWORD)"Prefs", "Autosave", 1);
  v20 = AfxGetModuleState();
  v0[129] = CWinApp::GetProfileIntA(*((CWinApp **)v20 + 1), (DWORD)"Prefs", "ApplyDismissesSurface", 0);
  v21 = AfxGetModuleState();
  v0[131] = CWinApp::GetProfileIntA(*((CWinApp **)v21 + 1), (DWORD)"Prefs", "LoadLastMap", 0);
  v22 = AfxGetModuleState();
  v0[136] = CWinApp::GetProfileIntA(*((CWinApp **)v22 + 1), (DWORD)"Prefs", "CleanTinyBrushes", 0);
  AfxGetModuleState();
  v23 = (char **)CWinApp::GetProfileStringA((int)&v124, "Prefs", "CleanTinyBrusheSize", (int)"0.5");
  LOBYTE(v125) = 3;
  sub_41A530((volatile signed __int32 **)&String, v23);
  LOBYTE(v125) = 0;
  v24 = v124 - 16;
  if ( _InterlockedDecrement((volatile signed __int32 *)(v124 - 16 + 12)) <= 0 )
    (*(void (__stdcall **)(int))(**(_DWORD **)v24 + 4))(v24);
  v25 = String;
  *((float *)v0 + 134) = atof(String);
  v26 = AfxGetModuleState();
  v27 = CWinApp::GetProfileIntA(*((CWinApp **)v26 + 1), (DWORD)"Prefs", "AutosaveMinutes", 5);
  v0[196] = v27;
  sub_40F4A0(v0 + 130, "%i", v27);
  v28 = AfxGetModuleState();
  v0[132] = CWinApp::GetProfileIntA(*((CWinApp **)v28 + 1), (DWORD)"Prefs", "Snapshots", 0);
  v29 = AfxGetModuleState();
  v0[133] = CWinApp::GetProfileIntA(*((CWinApp **)v29 + 1), (DWORD)"Prefs", "DefaultSaveNo", 0);
  v30 = AfxGetModuleState();
  v0[137] = CWinApp::GetProfileIntA(*((CWinApp **)v30 + 1), (DWORD)"Prefs", "StatusPointSize", 10);
  v31 = AfxGetModuleState();
  v0[195] = CWinApp::GetProfileIntA(*((CWinApp **)v31 + 1), (DWORD)"Prefs", "MoveSpeed", 350);
  v32 = AfxGetModuleState();
  v0[194] = CWinApp::GetProfileIntA(*((CWinApp **)v32 + 1), (DWORD)"Prefs", "AngleSpeed", 150);
  v33 = AfxGetModuleState();
  v0[138] = CWinApp::GetProfileIntA(*((CWinApp **)v33 + 1), (DWORD)"Prefs", "CamXYUpdate", 0);
  v34 = AfxGetModuleState();
  *((_BYTE *)v0 + 788) = CWinApp::GetProfileIntA(*((CWinApp **)v34 + 1), (DWORD)"Prefs", "CubicClipping", 1) != 0;
  v35 = AfxGetModuleState();
  v0[198] = CWinApp::GetProfileIntA(*((CWinApp **)v35 + 1), (DWORD)"Prefs", "CubicScale", 13);
  v36 = AfxGetModuleState();
  v0[141] = CWinApp::GetProfileIntA(*((CWinApp **)v36 + 1), (DWORD)"Prefs", "ALTEdgeDrag", 1);
  v37 = AfxGetModuleState();
  v0[142] = CWinApp::GetProfileIntA(*((CWinApp **)v37 + 1), (DWORD)"Prefs", "UseTextureBar", 0);
  AfxGetModuleState();
  v38 = (char **)CWinApp::GetProfileStringA((int)&v124, "Prefs", "WhichGame", (int)&off_6F01E8);
  LOBYTE(v125) = 4;
  sub_41A530((volatile signed __int32 **)v0 + 140, v38);
  LOBYTE(v125) = 0;
  v39 = v124 - 16;
  if ( _InterlockedDecrement((volatile signed __int32 *)(v124 - 16 + 12)) <= 0 )
    (*(void (__stdcall **)(int))(**(_DWORD **)v39 + 4))(v39);
  v40 = AfxGetModuleState();
  v0[143] = CWinApp::GetProfileIntA(*((CWinApp **)v40 + 1), (DWORD)"Prefs", "SnapT", 0);
  v41 = AfxGetModuleState();
  v0[144] = CWinApp::GetProfileIntA(*((CWinApp **)v41 + 1), (DWORD)"Prefs", "LinkSelect", 0);
  v42 = AfxGetModuleState();
  v0[145] = CWinApp::GetProfileIntA(*((CWinApp **)v42 + 1), (DWORD)"Prefs", "XZVIS", 0);
  v43 = AfxGetModuleState();
  v0[146] = CWinApp::GetProfileIntA(*((CWinApp **)v43 + 1), (DWORD)"Prefs", "YZVIS", 0);
  v44 = AfxGetModuleState();
  v0[147] = CWinApp::GetProfileIntA(*((CWinApp **)v44 + 1), (DWORD)"Prefs", "ZVIS", 1);
  v45 = AfxGetModuleState();
  v0[148] = CWinApp::GetProfileIntA(*((CWinApp **)v45 + 1), (DWORD)"Prefs", "SizePainting", 1);
  v46 = AfxGetModuleState();
  v0[149] = CWinApp::GetProfileIntA(*((CWinApp **)v46 + 1), (DWORD)"Prefs", "ZoomToCursor", 1);
  v47 = AfxGetModuleState();
  v0[150] = CWinApp::GetProfileIntA(*((CWinApp **)v47 + 1), (DWORD)"Prefs", "ShowPrefabOrigin", 1);
  v48 = AfxGetModuleState();
  v0[151] = CWinApp::GetProfileIntA(*((CWinApp **)v48 + 1), (DWORD)"Prefs", "CopyCrossDifferentLayer", 1);
  v49 = AfxGetModuleState();
  v0[187] = CWinApp::GetProfileIntA(*((CWinApp **)v49 + 1), (DWORD)"Prefs", "NewEntityWindow", 1);
  v50 = AfxGetModuleState();
  v0[152] = CWinApp::GetProfileIntA(*((CWinApp **)v50 + 1), (DWORD)"Prefs", "CullSkies", 1);
  v51 = AfxGetModuleState();
  v0[135] = CWinApp::GetProfileIntA(*((CWinApp **)v51 + 1), (DWORD)"Prefs", "DropHeight", 28);
  v52 = AfxGetModuleState();
  v0[154] = CWinApp::GetProfileIntA(*((CWinApp **)v52 + 1), (DWORD)"Prefs", "NoClamp", 0);
  v53 = AfxGetModuleState();
  v0[155] = CWinApp::GetProfileIntA(*((CWinApp **)v53 + 1), (DWORD)"Prefs", "DisplayMemoryInfo", 0);
  v54 = AfxGetModuleState();
  v0[156] = CWinApp::GetProfileIntA(*((CWinApp **)v54 + 1), (DWORD)"Prefs", "DropModel", 0);
  v55 = AfxGetModuleState();
  v0[157] = CWinApp::GetProfileIntA(*((CWinApp **)v55 + 1), (DWORD)"Prefs", "OrientModel", 0);
  AfxGetModuleState();
  v56 = (char **)CWinApp::GetProfileStringA((int)&v124, "Prefs", "ScriptGroupKey", (int)"script_group");
  LOBYTE(v125) = 5;
  sub_41A530((volatile signed __int32 **)v0 + 212, v56);
  LOBYTE(v125) = 0;
  v57 = v124 - 16;
  if ( _InterlockedDecrement((volatile signed __int32 *)(v124 - 16 + 12)) <= 0 )
    (*(void (__stdcall **)(int))(**(_DWORD **)v57 + 4))(v57);
  AfxGetModuleState();
  v58 = (char **)CWinApp::GetProfileStringA((int)&v124, "Prefs", "ScriptGroupTokenKey", (int)"script_group_tokens");
  LOBYTE(v125) = 6;
  sub_41A530((volatile signed __int32 **)v0 + 213, v58);
  LOBYTE(v125) = 0;
  v59 = v124 - 16;
  if ( _InterlockedDecrement((volatile signed __int32 *)(v124 - 16 + 12)) <= 0 )
    (*(void (__stdcall **)(int))(**(_DWORD **)v59 + 4))(v59);
  AfxGetModuleState();
  v60 = (char **)CWinApp::GetProfileStringA((int)&v124, "Prefs", "ScriptColorTeamKey", (int)"script_color_allies");
  LOBYTE(v125) = 7;
  sub_41A530((volatile signed __int32 **)v0 + 214, v60);
  LOBYTE(v125) = 0;
  v61 = v124 - 16;
  if ( _InterlockedDecrement((volatile signed __int32 *)(v124 - 16 + 12)) <= 0 )
    (*(void (__stdcall **)(int))(**(_DWORD **)v61 + 4))(v61);
  AfxGetModuleState();
  v62 = (char **)CWinApp::GetProfileStringA((int)&v124, "Prefs", "ScriptColorKey", (int)&off_6F05C8);
  LOBYTE(v125) = 8;
  sub_41A530((volatile signed __int32 **)v0 + 215, v62);
  LOBYTE(v125) = 0;
  v63 = v124 - 16;
  if ( _InterlockedDecrement((volatile signed __int32 *)(v124 - 16 + 12)) <= 0 )
    (*(void (__stdcall **)(int))(**(_DWORD **)v63 + 4))(v63);
  AfxGetModuleState();
  v64 = (char **)CWinApp::GetProfileStringA((int)&v124, "Prefs", "ScriptSubKey_key", (int)"script_objective_active");
  LOBYTE(v125) = 9;
  sub_41A530((volatile signed __int32 **)v0 + 217, v64);
  LOBYTE(v125) = 0;
  v65 = v124 - 16;
  if ( _InterlockedDecrement((volatile signed __int32 *)(v124 - 16 + 12)) <= 0 )
    (*(void (__stdcall **)(int))(**(_DWORD **)v65 + 4))(v65);
  AfxGetModuleState();
  v66 = (char **)CWinApp::GetProfileStringA((int)&v124, "Prefs", "ScriptSubValue_key", (int)Default);
  LOBYTE(v125) = 10;
  sub_41A530((volatile signed __int32 **)v0 + 218, v66);
  LOBYTE(v125) = 0;
  v67 = v124 - 16;
  if ( _InterlockedDecrement((volatile signed __int32 *)(v124 - 16 + 12)) <= 0 )
    (*(void (__stdcall **)(int))(**(_DWORD **)v67 + 4))(v67);
  AfxGetModuleState();
  v68 = (char **)CWinApp::GetProfileStringA((int)&v124, "Prefs", "UserINIPath", (int)Default);
  LOBYTE(v125) = 11;
  sub_41A530((volatile signed __int32 **)v0 + 158, v68);
  LOBYTE(v125) = 0;
  v69 = v124 - 16;
  if ( _InterlockedDecrement((volatile signed __int32 *)(v124 - 16 + 12)) <= 0 )
    (*(void (__stdcall **)(int))(**(_DWORD **)v69 + 4))(v69);
  AfxGetModuleState();
  v70 = (char **)CWinApp::GetProfileStringA((int)&v124, "Prefs", "UserFiltersPath", (int)Default);
  LOBYTE(v125) = 12;
  sub_41A530((volatile signed __int32 **)v0 + 159, v70);
  LOBYTE(v125) = 0;
  v71 = v124 - 16;
  if ( _InterlockedDecrement((volatile signed __int32 *)(v124 - 16 + 12)) <= 0 )
    (*(void (__stdcall **)(int))(**(_DWORD **)v71 + 4))(v71);
  v72 = AfxGetModuleState();
  v0[160] = CWinApp::GetProfileIntA(*((CWinApp **)v72 + 1), (DWORD)"Prefs", "Rotation", 45);
  v73 = AfxGetModuleState();
  v0[175] = CWinApp::GetProfileIntA(*((CWinApp **)v73 + 1), (DWORD)"Prefs", "Farplane", 0x2000);
  v74 = AfxGetModuleState();
  v0[176] = CWinApp::GetProfileIntA(*((CWinApp **)v74 + 1), (DWORD)"Prefs", "TolerantWeldThreshold", 24);
  v75 = AfxGetModuleState();
  v0[177] = CWinApp::GetProfileIntA(*((CWinApp **)v75 + 1), (DWORD)"Prefs", "VehArrowTime", 1000);
  v76 = AfxGetModuleState();
  v0[178] = CWinApp::GetProfileIntA(*((CWinApp **)v76 + 1), (DWORD)"Prefs", "VehArrowSize", 128);
  v77 = AfxGetModuleState();
  v0[179] = CWinApp::GetProfileIntA(*((CWinApp **)v77 + 1), (DWORD)"Prefs", "SplayDistance", 128);
  v78 = AfxGetModuleState();
  v0[161] = CWinApp::GetProfileIntA(*((CWinApp **)v78 + 1), (DWORD)"Prefs", "ChaseMouse", 1);
  v79 = AfxGetModuleState();
  v80 = CWinApp::GetProfileIntA(*((CWinApp **)v79 + 1), (DWORD)"Prefs", "EntityShow", 0);
  v0[200] = v80;
  if ( !v80 )
    v0[200] = 65552;
  v81 = AfxGetModuleState();
  v0[201] = CWinApp::GetProfileIntA(*((CWinApp **)v81 + 1), (DWORD)"Prefs", "TextureScale", 50);
  v82 = AfxGetModuleState();
  v0[162] = CWinApp::GetProfileIntA(*((CWinApp **)v82 + 1), (DWORD)"Prefs", "TextureScrollbar", 1);
  v83 = AfxGetModuleState();
  v0[203] = CWinApp::GetProfileIntA(*((CWinApp **)v83 + 1), (DWORD)"Prefs", "SwitchClipKey", 1);
  v84 = AfxGetModuleState();
  v0[204] = CWinApp::GetProfileIntA(*((CWinApp **)v84 + 1), (DWORD)"Prefs", "SelectWholeEntitiesKey", 1);
  v85 = AfxGetModuleState();
  v0[163] = CWinApp::GetProfileIntA(*((CWinApp **)v85 + 1), (DWORD)"Prefs", "ThickLines", 1);
  v86 = AfxGetModuleState();
  v0[164] = CWinApp::GetProfileIntA(*((CWinApp **)v86 + 1), (DWORD)"Prefs", "ColoredEnts", 0);
  v87 = AfxGetModuleState();
  v0[165] = CWinApp::GetProfileIntA(*((CWinApp **)v87 + 1), (DWORD)"Prefs", "TolerantWeld", 0);
  v88 = AfxGetModuleState();
  v0[166] = CWinApp::GetProfileIntA(*((CWinApp **)v88 + 1), (DWORD)"Prefs", "VertSnapModel", 0);
  v89 = AfxGetModuleState();
  v0[167] = CWinApp::GetProfileIntA(*((CWinApp **)v89 + 1), (DWORD)"Prefs", "VertSnapBrush", 0);
  v90 = AfxGetModuleState();
  v0[168] = CWinApp::GetProfileIntA(*((CWinApp **)v90 + 1), (DWORD)"Prefs", "VertSnapPrefab", 0);
  v91 = AfxGetModuleState();
  v0[169] = CWinApp::GetProfileIntA(*((CWinApp **)v91 + 1), (DWORD)"Prefs", "ModelSelection", 0);
  v92 = AfxGetModuleState();
  v0[170] = CWinApp::GetProfileIntA(*((CWinApp **)v92 + 1), (DWORD)"Prefs", "2dTextured", 0);
  v93 = AfxGetModuleState();
  v0[171] = CWinApp::GetProfileIntA(*((CWinApp **)v93 + 1), (DWORD)"Prefs", "2dMeshTextured", 0);
  v94 = AfxGetModuleState();
  v0[172] = CWinApp::GetProfileIntA(*((CWinApp **)v94 + 1), (DWORD)"Prefs", "Fast2dDragging", 1);
  v95 = AfxGetModuleState();
  v0[174] = CWinApp::GetProfileIntA(*((CWinApp **)v95 + 1), (DWORD)"Prefs", "TransparentBackground", 0);
  v96 = AfxGetModuleState();
  v0[180] = CWinApp::GetProfileIntA(*((CWinApp **)v96 + 1), (DWORD)"Prefs", "UndoLevels", 10);
  v97 = AfxGetModuleState();
  v0[209] = CWinApp::GetProfileIntA(*((CWinApp **)v97 + 1), (DWORD)"Prefs", "PatchWireframe", 0);
  v98 = AfxGetModuleState();
  *((_BYTE *)v0 + 840) = CWinApp::GetProfileIntA(*((CWinApp **)v98 + 1), (DWORD)"Prefs", "PatchWeld", 1) != 0;
  v99 = AfxGetModuleState();
  *((_BYTE *)v0 + 841) = CWinApp::GetProfileIntA(*((CWinApp **)v99 + 1), (DWORD)"Prefs", "PatchDrillDown", 1) != 0;
  v100 = AfxGetModuleState();
  v0[121] = CWinApp::GetProfileIntA(*((CWinApp **)v100 + 1), (DWORD)"Prefs", "EntitiesOff", 0);
  v101 = AfxGetModuleState();
  v0[122] = CWinApp::GetProfileIntA(*((CWinApp **)v101 + 1), (DWORD)"Prefs", "SkyBrushOff", 0);
  v102 = AfxGetModuleState();
  v0[123] = CWinApp::GetProfileIntA(*((CWinApp **)v102 + 1), (DWORD)"Prefs", "DrawToggle", 0);
  v103 = AfxGetModuleState();
  v0[181] = CWinApp::GetProfileIntA(*((CWinApp **)v103 + 1), (DWORD)"Prefs", "ScaleBase", 100);
  v104 = AfxGetModuleState();
  v0[182] = CWinApp::GetProfileIntA(*((CWinApp **)v104 + 1), (DWORD)"Prefs", "ScaleRange", 30);
  v105 = AfxGetModuleState();
  v124 = CWinApp::GetProfileIntA(*((CWinApp **)v105 + 1), (DWORD)"Prefs", off_6F080C, 65);
  *((float *)v0 + 183) = (float)(unsigned int)v124;
  v106 = AfxGetModuleState();
  v0[211] = CWinApp::GetProfileIntA(*((CWinApp **)v106 + 1), (DWORD)"Prefs", "CameraUseWheel", 1);
  v107 = AfxGetModuleState();
  v124 = CWinApp::GetProfileIntA(*((CWinApp **)v107 + 1), (DWORD)"Prefs", "ModelOrgSize", 4);
  *((float *)v0 + 185) = (float)(unsigned int)v124;
  v108 = AfxGetModuleState();
  v124 = CWinApp::GetProfileIntA(*((CWinApp **)v108 + 1), (DWORD)"Prefs", "PrefabOrgSize", 16);
  *((float *)v0 + 186) = (float)(unsigned int)v124;
  v109 = AfxGetModuleState();
  v0[205] = CWinApp::GetProfileIntA(*((CWinApp **)v109 + 1), (DWORD)"Prefs", "LightPreviewEnable", 1);
  v110 = AfxGetModuleState();
  v0[206] = CWinApp::GetProfileIntA(*((CWinApp **)v110 + 1), (DWORD)"Prefs", "SunLightPreviewEnable", 0);
  v111 = AfxGetModuleState();
  v0[166] = CWinApp::GetProfileIntA(*((CWinApp **)v111 + 1), (DWORD)"Prefs", "SnapModel", 0);
  v112 = AfxGetModuleState();
  v0[167] = CWinApp::GetProfileIntA(*((CWinApp **)v112 + 1), (DWORD)"Prefs", "SnapBrush", 0);
  v113 = AfxGetModuleState();
  v0[168] = CWinApp::GetProfileIntA(*((CWinApp **)v113 + 1), (DWORD)"Prefs", "SnapPrefab", 0);
  v114 = AfxGetModuleState();
  v0[207] = CWinApp::GetProfileIntA(*((CWinApp **)v114 + 1), (DWORD)"Prefs", "ShowLayerPaths", 0);
  v115 = AfxGetModuleState();
  v0[188] = CWinApp::GetProfileIntA(*((CWinApp **)v115 + 1), (DWORD)"Prefs", "Show2DPlaneBox", 1);
  v116 = AfxGetModuleState();
  v0[189] = CWinApp::GetProfileIntA(*((CWinApp **)v116 + 1), (DWORD)"Prefs", "ManipulatorHandles", 0);
  v117 = AfxGetModuleState();
  v0[190] = CWinApp::GetProfileIntA(*((CWinApp **)v117 + 1), (DWORD)"Prefs", "SunEntity", 0);
  v118 = AfxGetModuleState();
  v0[191] = CWinApp::GetProfileIntA(*((CWinApp **)v118 + 1), (DWORD)"Prefs", "ScaleLock", 1);
  v119 = AfxGetModuleState();
  v0[191] = CWinApp::GetProfileIntA(*((CWinApp **)v119 + 1), (DWORD)"Prefs", "TextureVertexLock", 1);
  AfxGetModuleState();
  v120 = (char **)CWinApp::GetProfileStringA((int)&v124, "Prefs", "AutoPrefix", (int)Default);
  LOBYTE(v125) = 13;
  sub_41A530((volatile signed __int32 **)v0 + 208, v120);
  LOBYTE(v125) = 0;
  v121 = v124 - 16;
  if ( _InterlockedDecrement((volatile signed __int32 *)(v124 - 16 + 12)) <= 0 )
    (*(void (__stdcall **)(int))(**(_DWORD **)v121 + 4))(v121);
  if ( !v0[118] )
    sub_466730((int)v0);
  result = v25 - 16;
  v125 = -1;
  if ( _InterlockedDecrement((volatile signed __int32 *)v25 - 1) <= 0 )
    return (char *)(*(int (__thiscall **)(_DWORD, char *))(**(_DWORD **)result + 4))(*(_DWORD *)result, v25 - 16);
  return result;
}
// 6F01E8: using guessed type _UNKNOWN *off_6F01E8;
// 6F05C8: using guessed type _UNKNOWN *off_6F05C8;
// 74CD1C: using guessed type int *off_74CD1C;


#endif
