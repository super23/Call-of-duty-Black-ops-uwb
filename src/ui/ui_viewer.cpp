#include "ui_viewer.h"

void __cdecl UI_ViewerRotateLeftRepeatEnabled_f()
{
  s_viewerRotateLeftRepeatEnabled = 1;
}

void __cdecl UI_ViewerRotateRightRepeatEnabled_f()
{
  s_viewerRotateRightRepeatEnabled = 1;
}

void __thiscall UIViewer::Init(UIViewer *this, bool _ingame)
{
  int i; // [esp+20h] [ebp-4h]

  if ( !this->inited )
  {
    this->loaded = _Dvar_RegisterBool("uiViewer_loaded", 0, 0, "a flag for the menu that the viewer is loaded");
    this->streaming = _Dvar_RegisterBool(
                        "uiViewer_streaming",
                        0,
                        0,
                        "The UI viewer cannot draw since the model is streaming");
    this->showDvar = _Dvar_RegisterBool("uiViewer_show", 0, 0, "Enables rendering of the ui viewer");
    this->ingameDvar = _Dvar_RegisterBool(
                         "uiViewer_ingame",
                         0,
                         0,
                         "The viewer is displayed in-game (false is in the frontend)");
    this->modeDvar = _Dvar_RegisterEnum("uiViewer_mode", modeNames, 0, 0, "current uiViewer mode");
    this->equipWeapon = _Dvar_RegisterEnum(
                          "uiViewer_equipWeapon",
                          weaponSlotNames,
                          0,
                          0,
                          "weapon to equip the player (primary/secondary)");
    this->customname = _Dvar_RegisterString(
                         "uiViewer_customname",
                         (char *)&toastPopupTitle,
                         0,
                         "Custom class name to display");
    this->weaponDvar = _Dvar_RegisterInt("uiViewer_weapon", 0, 0, 1000, 0, "Current weapon index to show");
    this->attachmentDvar = _Dvar_RegisterInt(
                             "uiViewer_attachment",
                             0,
                             0,
                             1000,
                             0,
                             "Current weapon attachment index to show");
    this->playerCameraDvar = _Dvar_RegisterString(
                               "uiViewer_playerCamera",
                               (char *)&toastPopupTitle,
                               0,
                               "UI viewer player camera name");
    this->cameraPitchRange = _Dvar_RegisterVec2(
                               "uiViewer_pitchRange",
                               COERCE_UNSIGNED_INT(-15.0),
                               COERCE_UNSIGNED_INT(30.0),
                               -40.0,
                               40.0,
                               0,
                               "Camera pitch range (x=minimum y=maximum)");
    this->allowCameraPitch = _Dvar_RegisterBool("uiViewer_allowPitch", 0, 0, "Allow the control of the camera pitch");
    this->lightMultiplier = _Dvar_RegisterFloat(
                              "uiViewer_lightMultiplier",
                              1.0,
                              0.1,
                              10.0,
                              0,
                              "UI Viewer model lighting multiplier");
    this->fov = _Dvar_RegisterFloat(
                  "uiViewer_fov",
                  65.0,
                  30.0,
                  100.0,
                  0,
                  "UI Viewer horizontal field of view (degrees)");
    this->highlightColor1 = _Dvar_RegisterColor(
                              "uiviewer_highlightColor1",
                              0.0,
                              0.0,
                              1.0,
                              1.0,
                              0,
                              "Highlight outline color #1 (pingpongs to color2)");
    this->highlightColor2 = _Dvar_RegisterColor(
                              "uiviewer_highlightColor2",
                              1.0,
                              1.0,
                              1.0,
                              1.0,
                              0,
                              "Highlight outline color #2 (pingpongs to color1)");
    this->heroHighlight = _Dvar_RegisterBool(
                            "uiViewer_heroHighlight",
                            1,
                            0,
                            "use herolight to highlight the customized part");
    this->sceneOrigin = _Dvar_RegisterVec3(
                          "uiViewer_sceneOrigin",
                          COERCE_UNSIGNED_INT(0.0),
                          COERCE_UNSIGNED_INT(0.0),
                          COERCE_UNSIGNED_INT(0.0),
                          -10000.0,
                          10000.0,
                          0,
                          "Origin of the models in ui viewer");
    this->cameraControl = _Dvar_RegisterBool(
                            "uiViewer_cameraControl",
                            0,
                            0x80u,
                            "Enable camera control in UI Viewer (for setting camera keyframes)");
    this->drawAttachPoints = _Dvar_RegisterBool(
                               "uiViewer_drawAttachPoints",
                               0,
                               0x80u,
                               "Draw a marker where weapons are attached to the player");
    this->viewerMsg = _Dvar_RegisterString("uiViewer_msg", (char *)&toastPopupTitle, 0x80u, "UI viewer message");
    this->dumpAnims = _Dvar_RegisterBool("uiViewer_dumpAnims", 0, 0x80u, "Enable dump anims");
    this->timeScale = _Dvar_RegisterFloat("uiViewer_timeScale", 1.0, 0.0099999998, 10.0, 0x80u, "Animation time scale");
    Cmd_AddCommandInternal("uiViewer_SetCameraPosition", UIViewer::SetCameraPosCmd, &SetCameraPosCmd_VAR);
    Cmd_AddCommandInternal("uiViewer_ResetCamera", UIViewer::ResetCameraCmd, &ResetCameraCmd_VAR);
    Cmd_AddCommandInternal("uiViewer_SetControllerIndex", UIViewer::SetControllerIndexCmd, &SetControllerIndexCmd_VAR);
    Cmd_AddCommandInternal(
      "viewerRotateLeftRepeatEnabled",
      UI_ViewerRotateLeftRepeatEnabled_f,
      &UI_ViewerRotateLeftRepeatEnabled_f_VAR);
    Cmd_AddCommandInternal(
      "viewerRotateRightRepeatEnabled",
      UI_ViewerRotateRightRepeatEnabled_f,
      &UI_ViewerRotateRightRepeatEnabled_f_VAR);
    for ( i = 0; i < 1; ++i )
      this->dobjsPool[i] = (DObj *)&this->dobjsBuffer[124 * i];
    StringTable_GetAsset("mp/ui_viewer_camera.csv", (XAssetHeader *)&this->cameraPosTable);
    StringTable_GetAsset("mp/bodyheadtable.csv", (XAssetHeader *)&this->bodyHeadTable);
    this->inited = 1;
    this->streamerInited = 0;
    this->mapLoaded = 0;
    this->show = 0;
    this->deferredShow = 0;
    this->ingame = _ingame;
    Dvar_SetBool((dvar_s *)this->ingameDvar, this->ingame);
    this->nCameraOverrides = 0;
  }
  this->nDrawDobjs = 0;
  if ( this->ingame != _ingame
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ui\\ui_viewer.cpp", 503, 0, "%s", "ingame == _ingame") )
  {
    __debugbreak();
  }
}

void __thiscall UIViewer::Shutdown(UIViewer *this)
{
  UIViewer::Hide(this);
  this->inited = 0;
  if ( this->loaded )
    Dvar_SetBool((dvar_s *)this->loaded, 0);
  if ( !this->ingame )
    BG_ShutdownWeaponDefFiles();
}

void __thiscall UIViewer::LoadMap(UIViewer *this)
{
  char *v1; // eax
  unsigned int i; // [esp+4h] [ebp-4h]

  if ( !this->inited
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ui\\ui_viewer.cpp", 521, 0, "%s", "inited") )
  {
    __debugbreak();
  }
  if ( !this->ingame )
  {
    LoadWorld((char *)comWorld.name);
    R_SortWorldSurfaces();
    v1 = va("%s", comWorld.name);
    CG_InitExposure(0, v1);
    Material_Sort();
    R_ClearSkyboxModel();
    CL_FinishLoadingModels();
    BG_InitDefaultWeaponDef();
    R_InitSceneData(0);
    this->animTree = 0;
  }
  if ( this->deferredShow )
    UIViewer::Show(this);
  for ( i = 0; i < 4; ++i )
    this->playerWeaponHistory[i] = 0;
  this->nextPlayerWeaponHistory = 0;
  this->mapLoaded = 1;
  Dvar_SetBool((dvar_s *)this->loaded, 1);
}

void __thiscall UIViewer::Show(UIViewer *this)
{
  int v1; // edx
  DvarValue *p_current; // [esp+18h] [ebp-Ch]
  int i; // [esp+1Ch] [ebp-8h]
  cg_s *cgameGlob; // [esp+20h] [ebp-4h]

  if ( !this->show )
  {
    if ( this->mapLoaded )
    {
      UIViewer::SetCameraPos(this, "player", "none", 0);
      UIViewer::InitFov(this);
      this->mode = MODE_PLAYER_FIRST;
      UIViewer::State::Invalidate(&this->prevState);
      p_current = &this->sceneOrigin->current;
      this->prevSceneOrigin[0] = p_current->value;
      this->prevSceneOrigin[1] = p_current->vector[1];
      this->prevSceneOrigin[2] = p_current->vector[2];
      if ( !this->ingame )
      {
        this->nextDobj = 0;
        Dvar_SetBoolByName("fx_marks", 0);
        Dvar_SetBoolByName("cg_draw2D", 0);
        Dvar_SetColor((dvar_s *)r_clearColor, 0.0, 0.0, 0.0, 0.0);
        Dvar_SetColor((dvar_s *)r_clearColor2, 0.0, 0.0, 0.0, 0.0);
        Dvar_SetBool((dvar_s *)r_setFrameBufferAlpha, 1);
        CL_ResetSkeletonCache(0);
        cgArray->predictedPlayerEntity.nextState.eType = 1;
        if ( !this->animTree )
        {
          BG_LoadPlayerAnimTypes();
          BG_InitWeaponStrings();
          this->bgs.AllocXAnim = (void *(__cdecl *)(int))Hunk_AllocXAnimPrecache;
          this->bgs.GetDObj = (DObj *(__cdecl *)(int, int))UIViewer::GetDObj;
          this->bgs.Rand = CG_rand;
          this->bgs.animData = &this->bgsAnim;
          memset((unsigned __int8 *)this->bgs.animData, 0, 0x8D388u);
          *(unsigned int *)(*((unsigned int *)NtCurrentTeb()->ThreadLocalStoragePointer + _tls_index) + 8) = UIViewer::GetBGS(this);
          Scr_BeginLoadAnimTrees(SCRIPTINSTANCE_SERVER, 0);
          BG_LoadAnim(comWorld.name);
          BG_PostLoadAnim(comWorld.name);
          *(unsigned int *)(*((unsigned int *)NtCurrentTeb()->ThreadLocalStoragePointer + _tls_index) + 8) = 0;
          this->animTree = XAnimCreateTree(
                             this->bgs.animData->generic_human.tree.anims,
                             (void *(__cdecl *)(int))Hunk_AllocXAnimPrecache);
        }
        BG_ShutdownWeaponDefFiles();
        BG_ClearWeaponDef();
        cgameGlob = CG_GetLocalClientGlobals(0);
        LODWORD(cgameGlob->bgs.clientinfo[0].xuid) = Live_GetXuid(this->controllerIndex);
        HIDWORD(cgameGlob->bgs.clientinfo[0].xuid) = v1;
        I_strncpyz(cgameGlob->visionNameNaked, "ui_viewer", 64);
        CG_VisionSetStartLerp_To(0, VISIONSETMODE_NAKED, VISIONSETLERP_TO_SMOOTH, cgameGlob->visionNameNaked, 0);
      }
      memset((unsigned __int8 *)&this->pmove, 0, sizeof(this->pmove));
      memset((unsigned __int8 *)&this->ps, 0, sizeof(this->ps));
      this->pmove.ps = &this->ps;
      for ( i = 0; i < 1; ++i )
        UIViewer::DrawDobj::Init(&this->drawDobjs[i]);
      this->show = 1;
      this->deferredShow = 0;
    }
    else
    {
      this->deferredShow = 1;
    }
  }
}

void __thiscall UIViewer::State::Invalidate(UIViewer::State *this)
{
  UIViewer::WeaponParams *p_weaponParams; // ecx

  this->mode = -1;
  this->playerParams.bodyIndex = -1;
  this->playerParams.headIndex = -1;
  this->playerParams.facePatternIndex = -1;
  this->playerParams.faceColorIndex = -1;
  p_weaponParams = &this->weaponParams;
  p_weaponParams->weaponSlot = -1;
  p_weaponParams->weaponIndex = -1;
  p_weaponParams->attachTopIndex = -1;
  p_weaponParams->attachBottomIndex = -1;
  p_weaponParams->attachTriggerIndex = -1;
  p_weaponParams->attachMuzzleIndex = -1;
  p_weaponParams->weaponOptions.i = -1;
  p_weaponParams->currentAttachmentPoint = -1;
  *(unsigned int *)this->faction = 0;
  *(unsigned int *)&this->faction[4] = 0;
  *(unsigned int *)&this->faction[8] = 0;
  *(unsigned int *)&this->faction[12] = 0;
  *(unsigned int *)&this->faction[16] = 0;
  *(unsigned int *)&this->faction[20] = 0;
  *(unsigned int *)&this->faction[24] = 0;
  *(unsigned int *)&this->faction[28] = 0;
}

void __thiscall UIViewer::Hide(UIViewer *this)
{
  this->deferredShow = 0;
  if ( this->show )
  {
    if ( !this->ingame )
    {
      Dvar_SetBoolByName("fx_marks", 1);
      Dvar_SetBoolByName("cg_draw2D", 1);
      Dvar_SetColor((dvar_s *)r_clearColor, 0.5, 0.75, 1.0, 1.0);
      Dvar_SetColor((dvar_s *)r_clearColor2, 1.0, 0.5, 0.0, 1.0);
      Dvar_SetBool((dvar_s *)r_setFrameBufferAlpha, 0);
    }
    this->show = 0;
  }
}

void __thiscall UIViewer::UpdateCamera(UIViewer *this, refdef_s *refdef, float deltaTime)
{
  float v4[3]; // [esp+10h] [ebp-B0h] BYREF
  float axis[10]; // [esp+1Ch] [ebp-A4h] BYREF
  float v6; // [esp+44h] [ebp-7Ch]
  float v7; // [esp+48h] [ebp-78h]
  float v8[4]; // [esp+4Ch] [ebp-74h] BYREF
  float pitch; // [esp+5Ch] [ebp-64h]
  float yaw; // [esp+60h] [ebp-60h]
  float angles[5]; // [esp+64h] [ebp-5Ch] BYREF
  float quatEnd[4]; // [esp+78h] [ebp-48h] BYREF
  float frac; // [esp+88h] [ebp-38h]
  float quat[4]; // [esp+8Ch] [ebp-34h] BYREF
  float quatStart[4]; // [esp+9Ch] [ebp-24h] BYREF
  float ANIM_TIME; // [esp+ACh] [ebp-14h]
  float v17; // [esp+B0h] [ebp-10h]
  cg_s *cgameGlob; // [esp+B4h] [ebp-Ch]
  int localClientNum; // [esp+B8h] [ebp-8h]
  float YAW_SPEED; // [esp+BCh] [ebp-4h]

  if ( this->cameraControl->current.enabled )
  {
    Dvar_SetString((dvar_s *)this->viewerMsg, "uiViewer control only supported on XBOX");
  }
  else if ( this->mode == MODE_PLAYER_FIRST || this->mode == MODE_WEAPON_FIRST || this->mode == MODE_EDIT_PLAYER_BODY )
  {
    YAW_SPEED = FLOAT_180_0;
    cgameGlob = CG_GetLocalClientGlobals(0);
    localClientNum = cgameGlob->localClientNum;
    if ( Key_IsDown(localClientNum, 156) || Key_IsDown(localClientNum, 200) && s_viewerRotateLeftRepeatEnabled )
    {
      s_viewerRotateRightRepeatEnabled = 0;
      this->camera.yaw = (float)((float)(-1.0 * YAW_SPEED) * deltaTime) + this->camera.yaw;
      this->camera.yaw = AngleNormalize180(this->camera.yaw);
    }
    else if ( Key_IsDown(localClientNum, 157) || Key_IsDown(localClientNum, 200) && s_viewerRotateRightRepeatEnabled )
    {
      s_viewerRotateLeftRepeatEnabled = 0;
      this->camera.yaw = (float)((float)(1.0 * YAW_SPEED) * deltaTime) + this->camera.yaw;
      this->camera.yaw = AngleNormalize180(this->camera.yaw);
    }
    else
    {
      s_viewerRotateLeftRepeatEnabled = 0;
      s_viewerRotateRightRepeatEnabled = 0;
    }
  }
  if ( this->cameraAnim )
  {
    ANIM_TIME = FLOAT_0_333;
    v17 = (float)(cls.realtime - this->cameraAnimStartTime) * 0.001;
    if ( v17 >= 0.0 && v17 <= ANIM_TIME )
    {
      frac = v17 / ANIM_TIME;
      Vec3Lerp(this->cameraAnimStart.lookAt, this->cameraAnimEnd.lookAt, v17 / ANIM_TIME, this->camera.lookAt);
      this->camera.dist = (float)((float)(this->cameraAnimEnd.dist - this->cameraAnimStart.dist)
                                * (float)(v17 / ANIM_TIME))
                        + this->cameraAnimStart.dist;
      LODWORD(v8[3]) = this->cameraAnimStart.lookAt;
      pitch = this->cameraAnimStart.pitch;
      yaw = this->cameraAnimStart.yaw;
      angles[0] = pitch;
      angles[1] = yaw;
      angles[2] = *(float *)&FLOAT_0_0;
      AnglesToQuat(angles, quatStart);
      LODWORD(axis[9]) = this->cameraAnimEnd.lookAt;
      v6 = this->cameraAnimEnd.pitch;
      v7 = this->cameraAnimEnd.yaw;
      v8[0] = v6;
      v8[1] = v7;
      v8[2] = *(float *)&FLOAT_0_0;
      AnglesToQuat(v8, quatEnd);
      QuatSlerp(quatStart, quatEnd, frac, quat);
      QuatToAxis(quat, (float (*)[3])axis);
      AxisToSignedAngles((const float (*)[3])axis, v4);
      this->camera.yaw = v4[1];
      this->camera.pitch = v4[0];
    }
    else
    {
      this->cameraAnim = 0;
      this->camera.lookAt[0] = this->cameraAnimEnd.lookAt[0];
      this->camera.lookAt[1] = this->cameraAnimEnd.lookAt[1];
      this->camera.lookAt[2] = this->cameraAnimEnd.lookAt[2];
      this->camera.dist = this->cameraAnimEnd.dist;
      this->camera.yaw = this->cameraAnimEnd.yaw;
      this->camera.pitch = this->cameraAnimEnd.pitch;
    }
  }
  UIViewer::CameraParams::SetRefDef(&this->camera, refdef);
}

void __thiscall UIViewer::CameraParams::SetRefDef(UIViewer::CameraParams *this, refdef_s *refdef)
{
  DvarValue *p_current; // [esp+8h] [ebp-28h]
  float v4; // [esp+18h] [ebp-18h]
  float angles[3]; // [esp+24h] [ebp-Ch] BYREF

  angles[0] = this->pitch;
  angles[2] = *(float *)&FLOAT_0_0;
  angles[1] = *(float *)&FLOAT_0_0;
  AnglesToAxis(angles, refdef->viewaxis);
  LODWORD(v4) = LODWORD(this->dist) ^ _mask__NegFloat_;
  refdef->vieworg[0] = (float)(v4 * refdef->viewaxis[0][0]) + this->lookAt[0];
  refdef->vieworg[1] = (float)(v4 * refdef->viewaxis[0][1]) + this->lookAt[1];
  refdef->vieworg[2] = (float)(v4 * refdef->viewaxis[0][2]) + this->lookAt[2];
  p_current = &uiViewer.sceneOrigin->current;
  refdef->vieworg[0] = refdef->vieworg[0] + uiViewer.sceneOrigin->current.value;
  refdef->vieworg[1] = refdef->vieworg[1] + p_current->vector[1];
  refdef->vieworg[2] = refdef->vieworg[2] + p_current->vector[2];
}

DObj *__thiscall UIViewer::AllocDobj(UIViewer *this, unsigned int *entnum)
{
  signed int v2; // ecx
  unsigned __int8 *Entity; // eax
  DObj *obj; // [esp+10h] [ebp-4h]

  v2 = (this->nextDobj + 1) & 0x80000000;
  if ( v2 < 0 )
    v2 = 0;
  this->nextDobj = v2;
  obj = this->dobjsPool[this->nextDobj];
  if ( this->ingame )
    *entnum = CG_Spawn(0)->nextState.number;
  else
    *entnum = this->nextDobj;
  Entity = (unsigned __int8 *)CG_GetEntity(0, *entnum);
  memset(Entity, 0, 0x328u);
  ++*entnum;
  return obj;
}

centity_s *__thiscall UIViewer::GetPlayerEntity(UIViewer *this)
{
  unsigned __int16 EntNum; // ax

  EntNum = DObjGetEntNum(this->drawDobjs[0].dobj);
  return CG_GetEntity(0, EntNum - 1);
}

XAnimTree_s *__thiscall UIViewer::GetAnimTree(UIViewer *this)
{
  if ( this->ingame )
    return CG_GetLocalClientGlobals(0)->bgs.clientinfo[0].pXAnimTree;
  else
    return this->animTree;
}

bgs_t *__thiscall UIViewer::GetBGS(UIViewer *this)
{
  if ( this->ingame )
    return &CG_GetLocalClientGlobals(0)->bgs;
  else
    return &this->bgs;
}

void __thiscall UIViewer::FreeDobjs(UIViewer *this)
{
  unsigned __int16 EntNum; // ax
  int i; // [esp+4h] [ebp-4h]

  if ( this->ingame )
  {
    for ( i = 0; i < this->nDrawDobjs; ++i )
    {
      EntNum = DObjGetEntNum(this->drawDobjs[i].dobj);
      CG_Free(0, EntNum - 1);
    }
  }
  this->nDrawDobjs = 0;
}

int __thiscall UIViewer::GetLoadout(UIViewer *this, loadoutSlot_t loadoutSlot, bool defaultIfZero)
{
  const char *String; // eax
  const char *v5; // [esp+0h] [ebp-20h]
  const char *ops[3]; // [esp+8h] [ebp-18h] BYREF
  const char *custom; // [esp+14h] [ebp-Ch]
  int ret; // [esp+18h] [ebp-8h]
  const char *cust1; // [esp+1Ch] [ebp-4h]

  cust1 = BG_UnlockablesGetLoadoutSlotName(loadoutSlot);
  ret = -1;
  if ( this->ingame && Dvar_GetInt("ui_multi_s") )
  {
    String = Dvar_GetString("ui_class");
    ret = BG_UnlockablesGetDefaultClassSlotIndexFromName(String, cust1);
  }
  else
  {
    if ( this->ingame )
      v5 = Dvar_GetString("ui_custom_name");
    else
      v5 = this->customname->current.string;
    custom = v5;
    ops[0] = "cacLoadouts";
    ops[1] = v5;
    ops[2] = cust1;
    LiveStats_GetCacDDLState();
    ret = LiveStats_GetDDLIntStat(this->controllerIndex, ops, 3);
  }
  if ( defaultIfZero && !ret )
    return BG_UnlockablesGetActualDefaultItem(loadoutSlot);
  return ret;
}

void __thiscall UIViewer::PlayerParams::PlayerParams(UIViewer::PlayerParams *this)
{
  this->bodyIndex = UIViewer::GetLoadout(&uiViewer, LOADOUTSLOT_BODY, 1);
  this->headIndex = UIViewer::GetLoadout(&uiViewer, LOADOUTSLOT_HEAD, 1);
  this->facePatternIndex = UIViewer::GetLoadout(&uiViewer, LOADOUTSLOT_FACEPAINT_PATTERN, 0);
  this->faceColorIndex = UIViewer::GetLoadout(&uiViewer, LOADOUTSLOT_FACEPAINT_COLOR, 0);
}

void __thiscall UIViewer::WeaponParams::WeaponParams(UIViewer::WeaponParams *this, UIViewer::WeaponSlot slot)
{
  this->weaponSlot = slot;
  this->weaponIndex = UIViewer::GetLoadout(
                        &uiViewer,
                        slot != WEAPON_PRIMARY ? LOADOUTSLOT_SECONDARY_WEAPON : LOADOUTSLOT_FIRST,
                        1);
  this->attachTopIndex = UIViewer::GetLoadout(
                           &uiViewer,
                           (loadoutSlot_t)(slot != WEAPON_PRIMARY
                                         ? LOADOUTSLOT_SECONDARY_ATTACHMENT_TOP
                                         : LOADOUTSLOT_PRIMARY_ATTACHMENT_TOP),
                           0);
  this->attachBottomIndex = UIViewer::GetLoadout(
                              &uiViewer,
                              (loadoutSlot_t)(slot != WEAPON_PRIMARY
                                            ? LOADOUTSLOT_SECONDARY_ATTACHMENT_BOTTOM
                                            : LOADOUTSLOT_PRIMARY_ATTACHMENT_BOTTOM),
                              0);
  this->attachTriggerIndex = UIViewer::GetLoadout(
                               &uiViewer,
                               (loadoutSlot_t)(slot != WEAPON_PRIMARY
                                             ? LOADOUTSLOT_SECONDARY_ATTACHMENT_TRIGGER
                                             : LOADOUTSLOT_PRIMARY_ATTACHMENT_TRIGGER),
                               0);
  this->attachMuzzleIndex = UIViewer::GetLoadout(
                              &uiViewer,
                              (loadoutSlot_t)(slot != WEAPON_PRIMARY
                                            ? LOADOUTSLOT_SECONDARY_ATTACHMENT_MUZZLE
                                            : LOADOUTSLOT_PRIMARY_ATTACHMENT_MUZZLE),
                              0);
  this->weaponOptions.i = 0;
  this->weaponOptions.i = UIViewer::GetLoadout(
                            &uiViewer,
                            (loadoutSlot_t)(slot != WEAPON_PRIMARY
                                          ? LOADOUTSLOT_SECONDARY_CAMO
                                          : LOADOUTSLOT_PRIMARY_CAMO),
                            0)
                        & 0x3F
                        | this->weaponOptions.i & 0xFFFFFFC0;
  this->weaponOptions.i = ((UIViewer::GetLoadout(
                              &uiViewer,
                              (loadoutSlot_t)(slot != WEAPON_PRIMARY
                                            ? LOADOUTSLOT_SECONDARY_LENS
                                            : LOADOUTSLOT_PRIMARY_LENS),
                              0)
                          & 0xF) << 6)
                        | this->weaponOptions.i & 0xFFFFFC3F;
  this->weaponOptions.i = ((UIViewer::GetLoadout(
                              &uiViewer,
                              (loadoutSlot_t)(slot != WEAPON_PRIMARY
                                            ? LOADOUTSLOT_SECONDARY_RETICLE
                                            : LOADOUTSLOT_PRIMARY_RETICLE),
                              0)
                          & 0x3F) << 10)
                        | this->weaponOptions.i & 0xFFFF03FF;
  this->weaponOptions.i = ((UIViewer::GetLoadout(
                              &uiViewer,
                              (loadoutSlot_t)(slot != WEAPON_PRIMARY
                                            ? LOADOUTSLOT_SECONDARY_RETICLE_COLOR
                                            : LOADOUTSLOT_PRIMARY_RETICLE_COLOR),
                              0)
                          & 7) << 16)
                        | this->weaponOptions.i & 0xFFF8FFFF;
  this->weaponOptions.i = ((UIViewer::GetLoadout(
                              &uiViewer,
                              (loadoutSlot_t)(slot != WEAPON_PRIMARY
                                            ? LOADOUTSLOT_SECONDARY_EMBLEM
                                            : LOADOUTSLOT_PRIMARY_EMBLEM),
                              0)
                          & 1) << 19)
                        | this->weaponOptions.i & 0xFFF7FFFF;
  this->weaponOptions.i = this->weaponOptions.i & 0xFC1FFFFF | 0x200000;
  this->currentAttachmentPoint = 0;
}

void __thiscall UIViewer::Update(UIViewer *this, float deltaTime)
{
  bool Bool; // al
  bool v3; // al
  const char *String; // eax
  centity_s *PlayerEntity; // eax
  unsigned int *v6; // [esp+8h] [ebp-158h]
  bool v7; // [esp+10h] [ebp-150h]
  DvarValue *p_current; // [esp+2Ch] [ebp-134h]
  pml_t pml; // [esp+38h] [ebp-128h] BYREF
  UIViewer::WeaponParams baseWeaponParam; // [esp+CCh] [ebp-94h] BYREF
  UIViewer::WeaponParams weapParams; // [esp+ECh] [ebp-74h] BYREF
  UIViewer::WeaponParams weaponParamForCombatRecord; // [esp+10Ch] [ebp-54h] BYREF
  UIViewer::PlayerParams playerParams; // [esp+12Ch] [ebp-34h] BYREF
  bool isLocked; // [esp+13Fh] [ebp-21h]
  UIViewer::WeaponParams weaponParam; // [esp+140h] [ebp-20h] BYREF

  this->mode = this->modeDvar->current.integer;
  UIViewer::WeaponParams::WeaponParams(&weaponParam, (UIViewer::WeaponSlot)this->equipWeapon->current.integer);
  UIViewer::PlayerParams::PlayerParams(&playerParams);
  if ( this->sceneOrigin->current.value != this->prevSceneOrigin[0]
    || this->sceneOrigin->current.vector[1] != this->prevSceneOrigin[1]
    || this->sceneOrigin->current.vector[2] != this->prevSceneOrigin[2] )
  {
    p_current = &this->sceneOrigin->current;
    this->prevSceneOrigin[0] = p_current->value;
    this->prevSceneOrigin[1] = p_current->vector[1];
    this->prevSceneOrigin[2] = p_current->vector[2];
    UIViewer::State::Invalidate(&this->prevState);
  }
  if ( weaponParam.weaponIndex != -1 )
  {
    isLocked = 0;
    switch ( this->mode )
    {
      case MODE_PLAYER_FIRST:
      case MODE_CHOOSE_CLASS_PLAYER:
        UIViewer::AddPlayerToScene(this, &playerParams, &weaponParam, 0);
        break;
      case MODE_EDIT_PLAYER_HEAD:
        playerParams.headIndex = sharedUiInfo.modIndex;
        UIViewer::AddPlayerToScene(this, &playerParams, &weaponParam, 0);
        break;
      case MODE_EDIT_PLAYER_BODY:
        playerParams.bodyIndex = BG_UnlockablesGetAssociatedBody(sharedUiInfo.modIndex);
        UIViewer::AddPlayerToScene(this, &playerParams, &weaponParam, 0);
        break;
      case MODE_EDIT_PLAYER_FACE_PATTERN:
        v7 = BG_UnlockablesIsItemClassified(sharedUiInfo.modIndex)
          || BG_UnlockablesIsItemLocked(this->controllerIndex, sharedUiInfo.modIndex);
        isLocked = v7;
        playerParams.facePatternIndex = !v7 ? sharedUiInfo.itemIndex : 0;
        UIViewer::AddPlayerToScene(this, &playerParams, &weaponParam, v7);
        break;
      case MODE_EDIT_PLAYER_FACE_PATTERN_COLOR:
        playerParams.faceColorIndex = sharedUiInfo.itemIndex;
        UIViewer::AddPlayerToScene(this, &playerParams, &weaponParam, 0);
        break;
      case MODE_COMBAT_RECORD:
        weaponParamForCombatRecord.currentAttachmentPoint = -1;
        weaponParamForCombatRecord.weaponSlot = WEAPON_PRIMARY;
        Bool = Dvar_GetBool("ui_showFriendsCombatRecord");
        weaponParamForCombatRecord.weaponIndex = (int)LiveCombatRecord_GetSortedItemData(0, Bool, ITEM_INDEX);
        memset(&weaponParamForCombatRecord.attachTopIndex, 0, 20);
        v3 = Dvar_GetBool("ui_showFriendsCombatRecord");
        playerParams.bodyIndex = (int)LiveCombatRecord_GetSortedItemData(6, v3, ITEM_INDEX);
        UIViewer::AddPlayerToScene(this, &playerParams, &weaponParamForCombatRecord, 0);
        break;
      case MODE_WEAPON_FIRST:
      case MODE_CHOOSE_CLASS_WEAPON:
        UIViewer::AddWeaponToScene(this, &weaponParam, &weaponParam);
        break;
      case MODE_EDIT_WEAPON:
        weapParams.currentAttachmentPoint = -1;
        String = Dvar_GetString("selected_loadout_slot");
        weapParams.weaponSlot = I_strcmp(String, "primary") != 0;
        weapParams.weaponIndex = sharedUiInfo.modIndex;
        memset(&weapParams.attachTopIndex, 0, 20);
        if ( BG_UnlockablesIsItemClassified(sharedUiInfo.modIndex) )
        {
          UIViewer::State::Invalidate(&this->prevState);
          UIViewer::FreeDobjs(this);
        }
        else
        {
          UIViewer::AddWeaponToScene(this, &weapParams, &weapParams);
        }
        break;
      case MODE_EDIT_CAMO:
        weaponParam.weaponOptions.i = sharedUiInfo.numItemsInSlot & 0x3F | weaponParam.weaponOptions.i & 0xFFFFFFC0;
        UIViewer::AddWeaponToScene(this, &weaponParam, &weaponParam);
        break;
      case MODE_EDIT_ATTACHMENT_TOP:
      case MODE_EDIT_ATTACHMENT_BOTTOM:
      case MODE_EDIT_ATTACHMENT_TRIGGER:
      case MODE_EDIT_ATTACHMENT_MUZZLE:
        memcpy(&baseWeaponParam, &weaponParam, sizeof(baseWeaponParam));
        UIViewer::SetAttachmentIndex(this, &weaponParam, SLODWORD(sharedUiInfo.itemColor[3]));
        UIViewer::ClearAttachmentIndex(this, &baseWeaponParam);
        weaponParam.currentAttachmentPoint = LODWORD(sharedUiInfo.itemColor[3]);
        UIViewer::AddWeaponToScene(this, &weaponParam, &baseWeaponParam);
        break;
      case MODE_EDIT_TAG:
        weaponParam.weaponOptions.i = ((sharedUiInfo.numItemsInSlot & 1) << 20)
                                    | weaponParam.weaponOptions.i & 0xFFEFFFFF;
        UIViewer::AddWeaponToScene(this, &weaponParam, &weaponParam);
        break;
      case MODE_EDIT_EMBLEM:
        weaponParam.weaponOptions.i = ((sharedUiInfo.numItemsInSlot & 1) << 19)
                                    | weaponParam.weaponOptions.i & 0xFFF7FFFF;
        UIViewer::AddWeaponToScene(this, &weaponParam, &weaponParam);
        break;
      case MODE_AAR_WEAPON:
        weaponParam.weaponSlot = WEAPON_PRIMARY;
        weaponParam.weaponIndex = Dvar_GetInt("ui_aar_curr_unlocked_weapon_index");
        memset(&weaponParam.attachTopIndex, 0, 20);
        UIViewer::AddWeaponToScene(this, &weaponParam, &weaponParam);
        break;
      default:
        break;
    }
    if ( this->nDrawDobjs > 0 && this->mode <= (unsigned int)MODE_COMBAT_RECORD )
    {
      *(unsigned int *)(*((unsigned int *)NtCurrentTeb()->ThreadLocalStoragePointer + _tls_index) + 8) = UIViewer::GetBGS(this);
      memset((unsigned __int8 *)&pml, 0, sizeof(pml));
      pml.msec = (int)(float)(deltaTime * 1000.0);
      PM_Weapon(&this->pmove, &pml);
      v6 = (unsigned int *)((char *)UIViewer::GetPlayerEntity(this) + 804);
      *v6 |= 2u;
      UIViewer::GetPlayerEntity(this)->nextState.eType = 1;
      UIViewer::GetPlayerEntity(this)->nextState.un2.animState.state = this->ps.legsAnim;
      UIViewer::GetPlayerEntity(this)->nextState.un2.anim.torsoAnim = this->ps.torsoAnim;
      this->bgs.clientinfo[0].pXAnimTree = UIViewer::GetAnimTree(this);
      PlayerEntity = UIViewer::GetPlayerEntity(this);
      BG_PlayerAnimation(0, &PlayerEntity->nextState, this->bgs.clientinfo);
      *(unsigned int *)(*((unsigned int *)NtCurrentTeb()->ThreadLocalStoragePointer + _tls_index) + 8) = 0;
    }
  }
}

void __thiscall UIViewer::DrawScene(UIViewer *this, unsigned int eyeToRender)
{
  double v2; // xmm0_8
  char v3; // al
  long double v4; // [esp+8h] [ebp-30h]
  float v5; // [esp+14h] [ebp-24h]
  int i; // [esp+18h] [ebp-20h]
  int screenWidth; // [esp+1Ch] [ebp-1Ch] BYREF
  float dxDzAtDefaultAspectRatio; // [esp+20h] [ebp-18h]
  int screenHeight; // [esp+24h] [ebp-14h] BYREF
  float aspect; // [esp+28h] [ebp-10h] BYREF
  int nDraws; // [esp+2Ch] [ebp-Ch]
  cg_s *cgameGlob; // [esp+30h] [ebp-8h]
  float deltaTime; // [esp+34h] [ebp-4h]

  HIDWORD(v4) = this;
  deltaTime = (float)((float)cls.frametime * 0.001) * this->timeScale->current.value;
  if ( (float)(deltaTime - 0.1) < 0.0 )
    v5 = deltaTime;
  else
    v5 = FLOAT_0_1;
  if ( (float)(0.0 - deltaTime) < 0.0 )
    *(float *)&v4 = v5;
  else
    LODWORD(v4) = *(unsigned int *)&FLOAT_0_0;
  deltaTime = *(float *)&v4;
  UIViewer::Update(this, *(float *)&v4);
  if ( !*(_BYTE *)(HIDWORD(v4) + 5) )
  {
    IK_UpdateTimeAll(cls.realtime, 0, 1);
    R_ClearScene(0);
    CL_ResetSkeletonCache(0);
  }
  cgameGlob = CG_GetLocalClientGlobals(0);
  if ( !*(_BYTE *)(HIDWORD(v4) + 5) )
  {
    memset((unsigned __int8 *)&cgameGlob->refdef, 0, sizeof(cgameGlob->refdef));
    cgameGlob->refdef.exposure = rgp.world->sunParse.sunSettings[0].exposure;
    v2 = (float)((float)(*(float *)(*(unsigned int *)(HIDWORD(v4) + 208) + 24) * 0.017453292) * 0.5);
    __libm_sse2_tan(v4);
    *(float *)&v2 = v2;
    dxDzAtDefaultAspectRatio = *(float *)&v2;
    cgameGlob->refdef.tanHalfFovX = *(float *)&v2;
    cgameGlob->refdef.tanHalfFovY = dxDzAtDefaultAspectRatio;
    cgameGlob->refdef.fov_x = *(float *)(*(unsigned int *)(HIDWORD(v4) + 208) + 24);
    cgameGlob->refdef.x = 0;
    cgameGlob->refdef.y = 0;
    screenWidth = 512;
    screenHeight = 512;
    CL_GetScreenDimensions(&screenWidth, &screenHeight, &aspect);
    cgameGlob->refdef.width = screenWidth;
    cgameGlob->refdef.height = screenHeight;
    cgameGlob->refdef.localClientNum = 0;
  }
  UIViewer::UpdateCamera((UIViewer *)HIDWORD(v4), &cgameGlob->refdef, deltaTime);
  R_SetLodOrigin(&cgameGlob->refdef);
  nDraws = 0;
  for ( i = 0; i < *(unsigned int *)(HIDWORD(v4) + 1004); ++i )
  {
    v3 = UIViewer::DrawDobj::Draw(
           (UIViewer::DrawDobj *)(HIDWORD(v4) + 524 * i + 480),
           deltaTime,
           *(float *)(*(unsigned int *)(HIDWORD(v4) + 204) + 24));
    nDraws += v3 != 0;
  }
  Dvar_SetBool(*(dvar_s **)(HIDWORD(v4) + 160), nDraws == 0);
  if ( *(_BYTE *)(*(unsigned int *)(HIDWORD(v4) + 236) + 24) )
    DObjDisplayAnim(*(const DObj **)(HIDWORD(v4) + 480), "After DObjInitServerTime\n");
  if ( !*(_BYTE *)(HIDWORD(v4) + 5) )
  {
    R_InitPrimaryLights(cgameGlob->refdef.primaryLights);
    R_ClearShadowedPrimaryLightHistory(0);
    CG_VisionSetApplyToRefdef(0, 0);
    Sys_WaitWorkerCmdInternal(&dobj_skelWorkerCmd);
    Sys_WaitWorkerCmdInternal(&dobj_skinWorkerCmd);
    R_ClearClientCmdList2D();
    R_UI3D_PerframeInit();
    R_UI3D_SetupBackendData(&frontEndDataOut->viewInfo[frontEndDataOut->viewInfoIndex].rbUI3D);
    CL_RenderScene(&cgameGlob->refdef, 0);
  }
}

void __thiscall UIViewer::DrawDobj::Init(UIViewer::DrawDobj *this)
{
  memset((unsigned __int8 *)this, 0, sizeof(UIViewer::DrawDobj));
  R_InitShaderConstantSet(&this->constantSet);
  R_MapShaderConstantSet(&this->constantSet, 4u, "heroLightingR");
  R_MapShaderConstantSet(&this->constantSet, 5u, "heroLightingG");
  R_MapShaderConstantSet(&this->constantSet, 6u, "heroLightingB");
}

void __thiscall UIViewer::DrawDobj::Set(
        UIViewer::DrawDobj *this,
        const UIViewer::WeaponParams *weapParam,
        const WeaponVariantDef *wvd,
        bool hero)
{
  UIViewer::PlayerParams playerParams; // [esp+4h] [ebp-10h] BYREF

  memset(&playerParams, 255, sizeof(playerParams));
  UIViewer::DrawDobj::Set(this, &playerParams, weapParam, wvd, hero, 0);
}

void __thiscall UIViewer::DrawDobj::Set(
        UIViewer::DrawDobj *this,
        const UIViewer::PlayerParams *playerParams,
        const UIViewer::WeaponParams *weapParam,
        const WeaponVariantDef *wvd,
        bool hero,
        bool lock)
{
  this->weaponOptions.i = weapParam->weaponOptions.i;
  this->weaponOptions.i = (playerParams->facePatternIndex << 26)
                        | ((unsigned int)&objBuf[1758][1] + 3) & this->weaponOptions.i;
  this->heroLighting = hero;
  this->startTime = cls.realtime;
  this->weaponVariantDef = wvd;
  this->locked = lock;
}

char __thiscall UIViewer::DrawDobj::Draw(UIViewer::DrawDobj *this, float deltaTime, float lightMultiplier)
{
  char *Name; // eax
  unsigned __int16 EntNum; // ax
  unsigned int String; // eax
  unsigned int v7; // eax
  long double thisa; // [esp+28h] [ebp-6Ch]
  float v9; // [esp+30h] [ebp-64h]
  unsigned __int8 boneIndex; // [esp+37h] [ebp-5Dh] BYREF
  float origin[3]; // [esp+38h] [ebp-5Ch] BYREF
  float axis[3][3]; // [esp+44h] [ebp-50h] BYREF
  float LIGHT_CYCLE; // [esp+68h] [ebp-2Ch]
  float MAX_LIGHT; // [esp+6Ch] [ebp-28h]
  float v15; // [esp+70h] [ebp-24h]
  float ratio; // [esp+74h] [ebp-20h]
  float l; // [esp+78h] [ebp-1Ch]
  int textureOverrideIndex; // [esp+7Ch] [ebp-18h]
  ShaderConstantSet *dobjConstantSet; // [esp+80h] [ebp-14h]
  unsigned int renderFxFlags; // [esp+84h] [ebp-10h]
  float lightingOrigin[3]; // [esp+88h] [ebp-Ch] BYREF

  LODWORD(thisa) = this;
  if ( !R_StreamTouchDObjAndCheck(this->dobj, 0) )
    return 0;
  memset(lightingOrigin, 0, sizeof(lightingOrigin));
  renderFxFlags = *(_BYTE *)(LODWORD(thisa) + 384) != 0 ? 3 : 0;
  Name = Clan_GetName(0);
  textureOverrideIndex = CG_SetupWeaponOptionsRender(
                           0,
                           (const float *)(LODWORD(thisa) + 56),
                           *(const WeaponVariantDef **)(LODWORD(thisa) + 4),
                           *(renderOptions_s *)(LODWORD(thisa) + 388),
                           (ShaderConstantSet *)(LODWORD(thisa) + 404),
                           Name);
  l = FLOAT_1_0;
  if ( *(_BYTE *)(LODWORD(thisa) + 392) )
  {
    MAX_LIGHT = FLOAT_4_0;
    LIGHT_CYCLE = FLOAT_2_0;
    v15 = (float)(cls.realtime - *(unsigned int *)(LODWORD(thisa) + 396)) * 0.001;
    __libm_sse2_sin(thisa);
    ratio = (float)((float)((float)(6.2831855 * v15) / 2.0) + 1.0) * 0.5;
    if ( uiViewer.heroHighlight->current.enabled )
      l = (float)(MAX_LIGHT * ratio) + 1.0;
  }
  if ( *(_BYTE *)(LODWORD(thisa) + 400) )
    lightMultiplier = lightMultiplier * 0.029999999;
  l = l * lightMultiplier;
  R_SetShaderConstantSetValue((ShaderConstantSet *)(LODWORD(thisa) + 404), 4u, l, 0.0, 0.0, 0.0);
  R_SetShaderConstantSetValue((ShaderConstantSet *)(LODWORD(thisa) + 404), 5u, 0.0, l, 0.0, 0.0);
  R_SetShaderConstantSetValue((ShaderConstantSet *)(LODWORD(thisa) + 404), 6u, 0.0, 0.0, l, 0.0);
  dobjConstantSet = (ShaderConstantSet *)(LODWORD(thisa) + 404);
  v9 = *(float *)(LODWORD(thisa) + 380) - uiViewer.camera.yaw;
  *(unsigned int *)(LODWORD(thisa) + 68) = *(unsigned int *)&FLOAT_0_0;
  *(float *)(LODWORD(thisa) + 72) = v9;
  *(unsigned int *)(LODWORD(thisa) + 76) = *(unsigned int *)&FLOAT_0_0;
  EntNum = DObjGetEntNum(*(const DObj **)LODWORD(thisa));
  R_AddDObjToScene(
    *(const DObj **)LODWORD(thisa),
    (const cpose_t *)(LODWORD(thisa) + 8),
    EntNum - 1,
    renderFxFlags,
    lightingOrigin,
    0.0,
    0.0,
    0.0,
    0.0,
    -1,
    textureOverrideIndex,
    dobjConstantSet,
    0,
    0.0,
    1.0);
  if ( uiViewer.drawAttachPoints->current.enabled )
  {
    boneIndex = -2;
    String = SL_FindString("tag_weapon_right", SCRIPTINSTANCE_SERVER);
    if ( DObjGetBoneIndex(*(const DObj **)LODWORD(thisa), String, &boneIndex, -1)
      && CG_DObjGetWorldBoneMatrix(
           (const cpose_t *)(LODWORD(thisa) + 8),
           *(DObj **)LODWORD(thisa),
           boneIndex,
           axis,
           origin) )
    {
      CL_AddDebugSphere(origin, 1.0, colorRed, 12, 0, 0);
    }
    boneIndex = -2;
    v7 = SL_FindString("tag_weapon_left", SCRIPTINSTANCE_SERVER);
    if ( DObjGetBoneIndex(*(const DObj **)LODWORD(thisa), v7, &boneIndex, -1) )
    {
      if ( CG_DObjGetWorldBoneMatrix(
             (const cpose_t *)(LODWORD(thisa) + 8),
             *(DObj **)LODWORD(thisa),
             boneIndex,
             axis,
             origin) )
      {
        CL_AddDebugSphere(origin, 1.0, colorLtYellow, 12, 0, 0);
      }
    }
  }
  DObjUpdateClientInfo(*(DObj **)LODWORD(thisa), deltaTime, 0);
  return 1;
}

const WeaponVariantDef *__thiscall UIViewer::GetWeaponVariantDef(
        UIViewer *this,
        const UIViewer::WeaponParams *weapParams)
{
  char *v4; // [esp+Ch] [ebp-A8h]
  char *v6; // [esp+14h] [ebp-A0h]
  const char *v7; // [esp+24h] [ebp-90h]
  unsigned int v8; // [esp+28h] [ebp-8Ch]
  char *v10; // [esp+30h] [ebp-84h]
  char v11; // [esp+37h] [ebp-7Dh]
  char *v12; // [esp+3Ch] [ebp-78h]
  const char *v13; // [esp+40h] [ebp-74h]
  const char *ItemAttachmentRef; // [esp+44h] [ebp-70h]
  unsigned int i; // [esp+4Ch] [ebp-68h]
  const char *weaponRef; // [esp+50h] [ebp-64h]
  const char *attachRefs[4]; // [esp+54h] [ebp-60h] BYREF
  char weaponDefName[68]; // [esp+64h] [ebp-50h] BYREF
  bool isDualWield; // [esp+AFh] [ebp-5h]
  int weaponDefIndex; // [esp+B0h] [ebp-4h]

  isDualWield = BG_UnlockablesGetItemAttachment(weapParams->weaponIndex, weapParams->attachBottomIndex) == ATTACHMENT_DW;
  weaponRef = BG_UnlockablesGetItemRef(weapParams->weaponIndex);
  attachRefs[0] = BG_UnlockablesGetItemAttachmentRef(weapParams->weaponIndex, weapParams->attachTopIndex);
  if ( isDualWield )
    ItemAttachmentRef = 0;
  else
    ItemAttachmentRef = BG_UnlockablesGetItemAttachmentRef(weapParams->weaponIndex, weapParams->attachBottomIndex);
  attachRefs[1] = ItemAttachmentRef;
  attachRefs[2] = BG_UnlockablesGetItemAttachmentRef(weapParams->weaponIndex, weapParams->attachTriggerIndex);
  attachRefs[3] = BG_UnlockablesGetItemAttachmentRef(weapParams->weaponIndex, weapParams->attachMuzzleIndex);
  v13 = weaponRef;
  v12 = weaponDefName;
  do
  {
    v11 = *v13;
    *v12++ = *v13++;
  }
  while ( v11 );
  if ( isDualWield )
  {
    v10 = (char *)&attachRefs[3] + 3;
    while ( *++v10 )
      ;
    strcpy(v10, "dw");
  }
  for ( i = 0; i < 4; ++i )
  {
    if ( attachRefs[i] && I_strcmp(attachRefs[i], "none") )
    {
      v7 = va("_%s", attachRefs[i]);
      v8 = (unsigned int)&v7[strlen(v7) + 1];
      v6 = (char *)&attachRefs[3] + 3;
      while ( *++v6 )
        ;
      memcpy(v6, v7, v8 - (unsigned int)v7);
    }
  }
  v4 = (char *)&attachRefs[3] + 3;
  while ( *++v4 )
    ;
  strcpy(v4, "_mp");
  weaponDefIndex = BG_GetWeaponIndexForName(weaponDefName, 0);
  return BG_GetWeaponVariantDef(weaponDefIndex);
}

const WeaponVariantDef *__thiscall UIViewer::AddWeaponXmodel(
        UIViewer *this,
        const UIViewer::WeaponParams *weapParams,
        DObjModel_s *dobjModel,
        bool world)
{
  const WeaponVariantDef *WeaponVariantDef; // eax

  WeaponVariantDef = UIViewer::GetWeaponVariantDef(this, weapParams);
  return UIViewer::AddWeaponXmodel(this, WeaponVariantDef, dobjModel, world);
}

const WeaponVariantDef *__thiscall UIViewer::AddWeaponXmodel(
        UIViewer *this,
        const WeaponVariantDef *weaponVariantDef,
        DObjModel_s *dobjModel,
        bool world)
{
  XModel *v5; // [esp+0h] [ebp-14h]
  XModel *v6; // [esp+4h] [ebp-10h]
  WeaponDef *weaponDef; // [esp+Ch] [ebp-8h]
  XModel *wepXmodel; // [esp+10h] [ebp-4h]

  if ( !weaponVariantDef )
    return 0;
  weaponDef = weaponVariantDef->weapDef;
  if ( world )
  {
    if ( weaponDef && *weaponDef->worldModel )
      v6 = *weaponDef->worldModel;
    else
      v6 = 0;
    wepXmodel = v6;
  }
  else
  {
    if ( weaponDef && *weaponDef->gunXModel )
      v5 = *weaponDef->gunXModel;
    else
      v5 = 0;
    wepXmodel = v5;
  }
  if ( !wepXmodel )
    return 0;
  dobjModel->model = wepXmodel;
  dobjModel->boneName = 0;
  dobjModel->ignoreCollision = 1;
  return weaponVariantDef;
}

void __thiscall UIViewer::HideWeaponTags(
        UIViewer *this,
        DObj *dobj,
        const WeaponVariantDef *weapon,
        unsigned int *partBits,
        bool dualWield)
{
  unsigned __int8 boneIndex; // [esp+7h] [ebp-5h] BYREF
  int tagIndex; // [esp+8h] [ebp-4h]

  for ( tagIndex = 0; tagIndex < 32 && weapon->hideTags[tagIndex]; ++tagIndex )
  {
    boneIndex = -2;
    if ( dualWield )
    {
      if ( !DObjGetModelBoneIndex(
              dobj,
              **(const char ***)weapon->weapDef->worldModel,
              weapon->hideTags[tagIndex],
              &boneIndex) )
        continue;
    }
    else if ( !DObjGetBoneIndex(dobj, weapon->hideTags[tagIndex], &boneIndex, -1) )
    {
      continue;
    }
    partBits[(int)boneIndex >> 5] |= 0x80000000 >> (boneIndex & 0x1F);
  }
}

void __thiscall UIViewer::HideWeaponTags(
        UIViewer *this,
        DObj *dobj,
        const unsigned __int16 *hideTags,
        unsigned int *partBits)
{
  unsigned __int8 boneIndex; // [esp+7h] [ebp-5h] BYREF
  int tagIndex; // [esp+8h] [ebp-4h]

  for ( tagIndex = 0; tagIndex < 32 && hideTags[tagIndex]; ++tagIndex )
  {
    boneIndex = -2;
    if ( DObjGetBoneIndex(dobj, hideTags[tagIndex], &boneIndex, -1) )
      partBits[(int)boneIndex >> 5] |= 0x80000000 >> (boneIndex & 0x1F);
  }
}

void __thiscall UIViewer::UnhideWeaponTags(
        UIViewer *this,
        DObj *dobj,
        const unsigned __int16 *hideTags,
        unsigned int *partBits)
{
  unsigned __int8 boneIndex; // [esp+7h] [ebp-5h] BYREF
  int tagIndex; // [esp+8h] [ebp-4h]

  for ( tagIndex = 0; tagIndex < 32 && hideTags[tagIndex]; ++tagIndex )
  {
    boneIndex = -2;
    if ( DObjGetBoneIndex(dobj, hideTags[tagIndex], &boneIndex, -1) )
      partBits[(int)boneIndex >> 5] &= ~(0x80000000 >> (boneIndex & 0x1F));
  }
}

char __thiscall UIViewer::AddBodyPart(
        UIViewer *this,
        const char *partName,
        const char *partType,
        DObjModel_s *dobjModel,
        const char *bone)
{
  const char *Faction; // eax
  const char *v6; // eax
  const char *v7; // eax
  unsigned __int16 String; // [esp+0h] [ebp-1Ch]
  XModel *xmodel; // [esp+Ch] [ebp-10h]
  const char *partModelName; // [esp+10h] [ebp-Ch]
  int column; // [esp+14h] [ebp-8h]
  int factionsRow; // [esp+18h] [ebp-4h]

  factionsRow = StringTable_LookupRowNumForValue(this->bodyHeadTable, 1, "factions");
  if ( factionsRow == -1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ui\\ui_viewer.cpp",
          1502,
          0,
          "%s\n\t%s",
          "factionsRow != STRINGTABLE_ROW_NOT_FOUND",
          "factions not found in table") )
  {
    __debugbreak();
  }
  Faction = UIViewer::GetFaction(this);
  column = StringTable_LookupColumnNumForValue(this->bodyHeadTable, factionsRow, Faction);
  if ( column == -1 )
  {
    v6 = UIViewer::GetFaction(this);
    v7 = va("faction '%s' not found in table", v6);
    if ( !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\ui\\ui_viewer.cpp",
            1505,
            0,
            "%s\n\t%s",
            "column != STRINGTABLE_ROW_NOT_FOUND",
            v7) )
      __debugbreak();
  }
  partModelName = StringTable_Lookup(this->bodyHeadTable, 1, partType, 2, partName, column);
  if ( !partModelName )
    return 0;
  if ( !*partModelName )
    return 0;
  xmodel = DB_FindXAssetHeader(ASSET_TYPE_XMODEL, partModelName, 1, -1).model;
  if ( !xmodel )
    return 0;
  dobjModel->model = xmodel;
  if ( bone )
    String = SL_FindString(bone, SCRIPTINSTANCE_SERVER);
  else
    String = 0;
  dobjModel->boneName = String;
  dobjModel->ignoreCollision = 1;
  return 1;
}

void __thiscall UIViewer::UpdatePlayerDObj(
        UIViewer *this,
        const UIViewer::PlayerParams *playerParams,
        const WeaponVariantDef *weaponVariantDef,
        const UIViewer::WeaponParams *weapParams,
        bool locked)
{
  const char *ItemRef; // eax
  const char *v6; // eax
  const char *v7; // eax
  const char *v8; // eax
  const char *v9; // eax
  const WeaponVariantDef *v10; // eax
  XAnimTree_s *AnimTree; // eax
  const WeaponVariantDef *v12; // eax
  __int16 v13; // [esp-10h] [ebp-144h]
  float *origin; // [esp+Ch] [ebp-128h]
  DvarValue *p_current; // [esp+10h] [ebp-124h]
  DObj *dobj; // [esp+14h] [ebp-120h]
  unsigned int numModels; // [esp+18h] [ebp-11Ch]
  unsigned int entnum; // [esp+1Ch] [ebp-118h] BYREF
  unsigned int partBits[5]; // [esp+20h] [ebp-114h] BYREF
  DObjModel_s dobjModels[32]; // [esp+34h] [ebp-100h] BYREF

  if ( !weaponVariantDef
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ui\\ui_viewer.cpp", 1527, 0, "%s", "weaponVariantDef") )
  {
    __debugbreak();
  }
  UIViewer::FreeDobjs(this);
  ItemRef = BG_UnlockablesGetItemRef(playerParams->bodyIndex);
  numModels = 1;
  if ( UIViewer::AddBodyPart(this, ItemRef, "body", dobjModels, 0) )
  {
    v6 = BG_UnlockablesGetItemRef(playerParams->headIndex);
    if ( UIViewer::AddBodyPart(this, v6, "headgear", &dobjModels[1], "J_HEAD") )
    {
      v7 = BG_UnlockablesGetItemRef(playerParams->bodyIndex);
      numModels = 3;
      if ( !UIViewer::AddBodyPart(this, v7, "head_naked", &dobjModels[2], "J_HEAD") )
        return;
    }
    else
    {
      v8 = BG_UnlockablesGetItemRef(playerParams->headIndex);
      if ( UIViewer::AddBodyPart(this, v8, "head", &dobjModels[1], "J_HEAD") )
      {
        numModels = 2;
      }
      else
      {
        v9 = BG_UnlockablesGetItemRef(playerParams->bodyIndex);
        if ( UIViewer::AddBodyPart(this, v9, "head_naked", &dobjModels[1], "J_HEAD") )
          numModels = 2;
      }
    }
    if ( UIViewer::AddWeaponXmodel(this, weaponVariantDef, &dobjModels[numModels], 1) )
    {
      dobjModels[numModels++].boneName = SL_FindString("tag_weapon_right", SCRIPTINSTANCE_SERVER);
      if ( weaponVariantDef->weapDef->bDualWield )
      {
        v10 = BG_GetWeaponVariantDef(weaponVariantDef->weapDef->dualWieldWeaponIndex);
        if ( UIViewer::AddWeaponXmodel(this, v10, &dobjModels[numModels], 1) )
          dobjModels[numModels++].boneName = SL_FindString("tag_weapon_left", SCRIPTINSTANCE_SERVER);
      }
      else if ( weaponVariantDef->weapDef->additionalMeleeModel )
      {
        dobjModels[numModels].model = weaponVariantDef->weapDef->additionalMeleeModel;
        dobjModels[numModels].boneName = 0;
        dobjModels[numModels].ignoreCollision = 1;
        dobjModels[numModels++].boneName = SL_FindString("tag_weapon_left", SCRIPTINSTANCE_SERVER);
      }
    }
    dobj = UIViewer::AllocDobj(this, &entnum);
    v13 = entnum;
    AnimTree = UIViewer::GetAnimTree(this);
    DObjCreateExt(dobjModels, numModels, AnimTree, (unsigned __int8 *)dobj, v13, 0, 1, 0);
    memset(partBits, 0, sizeof(partBits));
    UIViewer::HideWeaponTags(this, dobj, weaponVariantDef, partBits, 0);
    if ( weaponVariantDef->weapDef->bDualWield )
    {
      v12 = BG_GetWeaponVariantDef(weaponVariantDef->weapDef->dualWieldWeaponIndex);
      UIViewer::HideWeaponTags(this, dobj, v12, partBits, 1);
    }
    DObjSetHidePartBits(dobj, partBits);
    this->drawDobjs[this->nDrawDobjs].dobj = dobj;
    memset(
      (unsigned __int8 *)&this->drawDobjs[this->nDrawDobjs].pose,
      0,
      sizeof(this->drawDobjs[this->nDrawDobjs].pose));
    origin = this->drawDobjs[this->nDrawDobjs].pose.origin;
    p_current = &this->sceneOrigin->current;
    *origin = p_current->value;
    origin[1] = p_current->vector[1];
    origin[2] = p_current->vector[2];
    this->drawDobjs[this->nDrawDobjs].yaw = FLOAT_180_0;
    this->drawDobjs[this->nDrawDobjs].depthHack = 0;
    UIViewer::DrawDobj::Set(&this->drawDobjs[this->nDrawDobjs], playerParams, weapParams, weaponVariantDef, 0, locked);
    ++this->nDrawDobjs;
  }
}

void __thiscall UIViewer::UpdatePlayerAnim(UIViewer *this, const WeaponVariantDef *weaponVariantDef)
{
  int WeaponIndexForName; // eax
  unsigned int *v3; // [esp+0h] [ebp-8h]

  if ( !weaponVariantDef
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ui\\ui_viewer.cpp", 1607, 0, "%s", "weaponVariantDef") )
  {
    __debugbreak();
  }
  if ( this->nDrawDobjs > 0 )
  {
    *(unsigned int *)(*((unsigned int *)NtCurrentTeb()->ThreadLocalStoragePointer + _tls_index) + 8) = UIViewer::GetBGS(this);
    WeaponIndexForName = BG_GetWeaponIndexForName((char *)weaponVariantDef->szInternalName, 0);
    AssignToSmallerType<unsigned short>(&this->ps.weapon, WeaponIndexForName);
    UIViewer::GetPlayerEntity(this)->nextState.weapon = this->ps.weapon;
    v3 = (unsigned int *)((char *)UIViewer::GetPlayerEntity(this) + 804);
    *v3 |= 2u;
    UIViewer::GetPlayerEntity(this)->nextState.eType = 1;
    BG_SetConditionBit(0, 1, weaponVariantDef->weapDef->weapClass);
    BG_SetConditionBit(0, 0, weaponVariantDef->weapDef->playerAnimType);
    BG_SetConditionValue(0, 0x16u, 1u);
    BG_AnimScriptAnimation(&this->pmove, AISTATE_COMBAT, ANIM_MT_IDLE, 1);
    *(unsigned int *)(*((unsigned int *)NtCurrentTeb()->ThreadLocalStoragePointer + _tls_index) + 8) = 0;
    cgArray->predictedPlayerState.weapon = this->ps.weapon;
    this->currentPlayerWeapon = this->ps.weapon;
  }
}

const char *__thiscall UIViewer::GetFaction(UIViewer *this)
{
  cg_s *LocalClientGlobals; // eax

  if ( !this->ingame )
    return Dvar_GetString("ui_faction");
  LocalClientGlobals = CG_GetLocalClientGlobals(0);
  if ( LocalClientGlobals->bgs.clientinfo[LocalClientGlobals->clientNum].team == TEAM_AXIS )
    return Dvar_GetString("g_FactionName_Axis");
  else
    return Dvar_GetString("g_FactionName_Allies");
}

void __thiscall UIViewer::AddPlayerToScene(
        UIViewer *this,
        const UIViewer::PlayerParams *playerParams,
        const UIViewer::WeaponParams *weapParams,
        bool locked)
{
  const WeaponVariantDef *WeaponVariantDef; // eax
  const WeaponVariantDef *v5; // eax
  const WeaponVariantDef *v6; // eax
  char v7; // al
  char v8; // al
  char *playerCameraName; // [esp+10h] [ebp-ACh]
  char *integer; // [esp+14h] [ebp-A8h]
  char v11; // [esp+33h] [ebp-89h]
  char *v12; // [esp+38h] [ebp-84h]
  const char *v13; // [esp+3Ch] [ebp-80h]
  const char *Faction; // [esp+50h] [ebp-6Ch]
  unsigned __int16 newWeapon; // [esp+54h] [ebp-68h]
  const WeaponVariantDef *wepVariantDef; // [esp+58h] [ebp-64h]
  UIViewer::State state; // [esp+5Ch] [ebp-60h] BYREF
  bool animate; // [esp+BAh] [ebp-2h]
  bool updated; // [esp+BBh] [ebp-1h]

  Faction = UIViewer::GetFaction(this);
  memset(&state, 0, 36);
  state.playerParams = *playerParams;
  memcpy(&state.weaponParams, weapParams, sizeof(state.weaponParams));
  state.locked = locked;
  v13 = Faction;
  v12 = state.faction;
  do
  {
    v11 = *v13;
    *v12++ = *v13++;
  }
  while ( v11 );
  animate = this->prevState.mode == 0;
  updated = 0;
  if ( UIViewer::State::operator!=(&state, &this->prevState) )
  {
    wepVariantDef = UIViewer::GetWeaponVariantDef(this, weapParams);
    if ( wepVariantDef )
    {
      newWeapon = BG_GetWeaponIndexForName((char *)wepVariantDef->szInternalName, 0);
      this->pmove.cmd.weapon = BG_GetWeaponIndexForName((char *)wepVariantDef->szInternalName, 0);
      this->ps.weapon = this->pmove.cmd.weapon;
      if ( !this->prevState.mode && newWeapon == this->currentPlayerWeapon )
      {
        WeaponVariantDef = BG_GetWeaponVariantDef(this->ps.weapon);
        UIViewer::UpdatePlayerDObj(this, playerParams, WeaponVariantDef, weapParams, locked);
      }
      else
      {
        UIViewer::UpdatePlayerDObj(this, playerParams, wepVariantDef, weapParams, locked);
        UIViewer::UpdatePlayerAnim(this, wepVariantDef);
      }
      if ( !BG_PlayerHasWeapon(&this->ps, this->pmove.cmd.weapon) )
      {
        BG_TakePlayerWeapon(&this->ps, this->playerWeaponHistory[this->nextPlayerWeaponHistory % 4u]);
        if ( BG_HoldWeapon(&this->ps, this->pmove.cmd.weapon) )
        {
          this->playerWeaponHistory[this->nextPlayerWeaponHistory % 4u] = this->pmove.cmd.weapon;
          ++this->nextPlayerWeaponHistory;
        }
      }
      memcpy(&this->prevState, &state, sizeof(this->prevState));
      updated = 1;
    }
  }
  else if ( this->ps.weapon != this->currentPlayerWeapon )
  {
    v5 = BG_GetWeaponVariantDef(this->ps.weapon);
    UIViewer::UpdatePlayerDObj(this, playerParams, v5, weapParams, locked);
    v6 = BG_GetWeaponVariantDef(this->ps.weapon);
    UIViewer::UpdatePlayerAnim(this, v6);
  }
  if ( strcmp(this->playerCameraName, this->playerCameraDvar->current.string) )
  {
    if ( this->ingame )
      v7 = UIViewer::SetCameraPos(this, (char *)this->playerCameraDvar->current.integer, "ingame", animate);
    else
      v7 = UIViewer::SetCameraPos(this, (char *)this->playerCameraDvar->current.integer, "none", animate);
    if ( !v7 )
    {
      Dvar_SetString((dvar_s *)this->playerCameraDvar, "player");
      UIViewer::SetCameraPos(this, (char *)this->playerCameraDvar->current.integer, "none", animate);
    }
    integer = (char *)this->playerCameraDvar->current.integer;
    playerCameraName = this->playerCameraName;
    do
    {
      v8 = *integer;
      *playerCameraName++ = *integer++;
    }
    while ( v8 );
  }
}

bool __thiscall UIViewer::State::operator!=(UIViewer::State *this, const UIViewer::State *o)
{
  return this->mode != o->mode
      || UIViewer::State::HasPlayerChanged(this, o)
      || memcmp((const char *)&this->weaponParams, (const char *)&o->weaponParams, 32);
}

bool __thiscall UIViewer::State::HasPlayerChanged(UIViewer::State *this, const UIViewer::State *o)
{
  return memcmp((const char *)&this->playerParams, (const char *)&o->playerParams, 16)
      || memcmp(this->faction, o->faction, 0x20u)
      || this->locked != o->locked;
}

int *__thiscall UIViewer::GetAttachmentIndexPtr(UIViewer *this, UIViewer::WeaponParams *weapParams)
{
  eAttachmentPoint attachPoint; // [esp+4h] [ebp-14h]
  int *attachIndex[4]; // [esp+8h] [ebp-10h]

  attachIndex[0] = &weapParams->attachTopIndex;
  attachIndex[1] = &weapParams->attachBottomIndex;
  attachIndex[2] = &weapParams->attachTriggerIndex;
  attachIndex[3] = &weapParams->attachMuzzleIndex;
  attachPoint = GetCurrentAttachPoint();
  if ( attachPoint == ATTACHMENT_POINT_NONE )
    return 0;
  if ( (unsigned int)(attachPoint - 1) >= 4
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\ui\\ui_viewer.cpp",
          1728,
          0,
          "%s",
          "( attachPoint - ATTACHMENT_POINT_TOP ) < ARRAY_COUNT( attachIndex )") )
  {
    __debugbreak();
  }
  if ( (unsigned int)(attachPoint - 1) < 4 )
    return attachIndex[attachPoint - 1];
  else
    return 0;
}

eAttachmentPoint __cdecl GetCurrentAttachPoint()
{
  eAttachment attachment; // [esp+0h] [ebp-Ch]
  int attachmentNum; // [esp+4h] [ebp-8h]

  attachmentNum = BG_UnlockablesGetItemAttachmentNumWithAttachPoint(
                    sharedUiInfo.modIndex,
                    SLODWORD(sharedUiInfo.itemColor[3]),
                    0);
  attachment = BG_UnlockablesGetItemAttachment(sharedUiInfo.modIndex, attachmentNum);
  return BG_GetAttachmentPointIndexFromAttachment(attachment);
}

int __thiscall UIViewer::GetAttachmentIndex(UIViewer *this, UIViewer::WeaponParams *weapParams)
{
  int *attachIndex; // [esp+4h] [ebp-4h]

  attachIndex = UIViewer::GetAttachmentIndexPtr(this, weapParams);
  if ( attachIndex )
    return *attachIndex;
  else
    return 0;
}

void __thiscall UIViewer::SetAttachmentIndex(UIViewer *this, UIViewer::WeaponParams *weapParams, int index)
{
  int *attachIndex; // [esp+4h] [ebp-4h]

  attachIndex = UIViewer::GetAttachmentIndexPtr(this, weapParams);
  if ( attachIndex )
    *attachIndex = index;
}

void __thiscall UIViewer::ClearAttachmentIndex(UIViewer *this, UIViewer::WeaponParams *weapParams)
{
  int *attachIndex; // [esp+4h] [ebp-4h]

  attachIndex = UIViewer::GetAttachmentIndexPtr(this, weapParams);
  if ( attachIndex )
    *attachIndex = 0;
}

const char *__thiscall UIViewer::GetAttachmentSlotName(UIViewer *this)
{
  return modeNames[this->mode] + 14;
}

void __thiscall UIViewer::AddWeaponToScene(
        UIViewer *this,
        UIViewer::WeaponParams *weapParams,
        UIViewer::WeaponParams *baseWeapParams)
{
  int AttachmentIndex; // eax
  bool v4; // [esp+8h] [ebp-260h]
  UIViewer::Mode v9; // [esp+3Ch] [ebp-22Ch]
  float *v10; // [esp+40h] [ebp-228h]
  DvarValue *v11; // [esp+44h] [ebp-224h]
  UIViewer::Mode v12; // [esp+48h] [ebp-220h]
  float *origin; // [esp+4Ch] [ebp-21Ch]
  DvarValue *p_current; // [esp+50h] [ebp-218h]
  UIViewer::Mode mode; // [esp+54h] [ebp-214h]
  char *attachRef; // [esp+64h] [ebp-204h]
  char *attachMode; // [esp+68h] [ebp-200h]
  unsigned __int8 *buf; // [esp+6Ch] [ebp-1FCh]
  unsigned int v19; // [esp+70h] [ebp-1F8h] BYREF
  unsigned int v20[5]; // [esp+74h] [ebp-1F4h] BYREF
  int tagIndex; // [esp+88h] [ebp-1E0h]
  bool found; // [esp+8Fh] [ebp-1D9h]
  int baseTagIndex; // [esp+90h] [ebp-1D8h]
  const WeaponVariantDef *baseWeaponVariantDef; // [esp+94h] [ebp-1D4h]
  int numAttachmentTags; // [esp+98h] [ebp-1D0h]
  DObj *dobj; // [esp+9Ch] [ebp-1CCh]
  unsigned __int16 attachmentTags[34]; // [esp+A0h] [ebp-1C8h] BYREF
  int numModels; // [esp+E4h] [ebp-184h]
  const char *weaponRef; // [esp+E8h] [ebp-180h]
  unsigned int entnum; // [esp+ECh] [ebp-17Ch] BYREF
  unsigned int partBits[5]; // [esp+F0h] [ebp-178h] BYREF
  const WeaponVariantDef *weaponVariantDef; // [esp+104h] [ebp-164h]
  DObjModel_s dobjModels[32]; // [esp+108h] [ebp-160h] BYREF
  UIViewer::State state; // [esp+208h] [ebp-60h] BYREF

  state.mode = this->mode;
  memset(&state.playerParams, 255, sizeof(state.playerParams));
  memcpy(&state.weaponParams, weapParams, sizeof(state.weaponParams));
  state.locked = 0;
  memset(state.faction, 0, sizeof(state.faction));
  if ( UIViewer::State::operator!=(&state, &this->prevState) )
  {
    UIViewer::FreeDobjs(this);
    weaponVariantDef = UIViewer::AddWeaponXmodel(this, weapParams, dobjModels, 1);
    numModels = 1;
    if ( weaponVariantDef )
    {
      dobj = UIViewer::AllocDobj(this, &entnum);
      DObjCreate(dobjModels, numModels, 0, (unsigned __int8 *)dobj, entnum);
      memset(partBits, 0, sizeof(partBits));
      UIViewer::HideWeaponTags(this, dobj, weaponVariantDef, partBits, 0);
      mode = this->mode;
      if ( mode == MODE_EDIT_ATTACHMENT_TOP
        || mode == MODE_EDIT_ATTACHMENT_BOTTOM
        || mode == MODE_EDIT_ATTACHMENT_TRIGGER
        || mode == MODE_EDIT_ATTACHMENT_MUZZLE )
      {
        baseWeaponVariantDef = UIViewer::GetWeaponVariantDef(this, baseWeapParams);
        numAttachmentTags = 0;
        memset((unsigned __int8 *)attachmentTags, 0, 0x40u);
        for ( baseTagIndex = 0; baseTagIndex < 32 && baseWeaponVariantDef->hideTags[baseTagIndex]; ++baseTagIndex )
        {
          found = 0;
          for ( tagIndex = 0; tagIndex < 32 && weaponVariantDef->hideTags[tagIndex]; ++tagIndex )
          {
            if ( weaponVariantDef->hideTags[tagIndex] == baseWeaponVariantDef->hideTags[baseTagIndex] )
            {
              found = 1;
              break;
            }
          }
          if ( !found )
            attachmentTags[numAttachmentTags++] = baseWeaponVariantDef->hideTags[baseTagIndex];
        }
        UIViewer::HideWeaponTags(this, dobj, attachmentTags, partBits);
      }
      DObjSetHidePartBits(dobj, partBits);
      this->drawDobjs[this->nDrawDobjs].dobj = dobj;
      memset(
        (unsigned __int8 *)&this->drawDobjs[this->nDrawDobjs].pose,
        0,
        sizeof(this->drawDobjs[this->nDrawDobjs].pose));
      origin = this->drawDobjs[this->nDrawDobjs].pose.origin;
      p_current = &this->sceneOrigin->current;
      *origin = p_current->value;
      origin[1] = p_current->vector[1];
      origin[2] = p_current->vector[2];
      this->drawDobjs[this->nDrawDobjs].yaw = FLOAT_90_0;
      this->drawDobjs[this->nDrawDobjs].depthHack = this->mode == MODE_EDIT_ATTACHMENT_TOP;
      UIViewer::DrawDobj::Set(&this->drawDobjs[this->nDrawDobjs], weapParams, weaponVariantDef, 0);
      ++this->nDrawDobjs;
      v12 = this->mode;
      if ( v12 == MODE_EDIT_ATTACHMENT_TOP
        || v12 == MODE_EDIT_ATTACHMENT_BOTTOM
        || v12 == MODE_EDIT_ATTACHMENT_TRIGGER
        || v12 == MODE_EDIT_ATTACHMENT_MUZZLE )
      {
        buf = (unsigned __int8 *)UIViewer::AllocDobj(this, &v19);
        DObjCreate(dobjModels, numModels, 0, buf, v19);
        memset(v20, 255, sizeof(v20));
        UIViewer::UnhideWeaponTags(this, (DObj *)buf, attachmentTags, v20);
        DObjSetHidePartBits((DObj *)buf, v20);
        this->drawDobjs[this->nDrawDobjs].dobj = (DObj *)buf;
        memset(
          (unsigned __int8 *)&this->drawDobjs[this->nDrawDobjs].pose,
          0,
          sizeof(this->drawDobjs[this->nDrawDobjs].pose));
        v10 = this->drawDobjs[this->nDrawDobjs].pose.origin;
        v11 = &this->sceneOrigin->current;
        *v10 = v11->value;
        v10[1] = v11->vector[1];
        v10[2] = v11->vector[2];
        this->drawDobjs[this->nDrawDobjs].yaw = FLOAT_90_0;
        this->drawDobjs[this->nDrawDobjs].depthHack = this->mode == MODE_EDIT_ATTACHMENT_TOP;
        UIViewer::DrawDobj::Set(&this->drawDobjs[this->nDrawDobjs], weapParams, weaponVariantDef, 1);
        ++this->nDrawDobjs;
      }
      weaponRef = BG_UnlockablesGetItemRef(weapParams->weaponIndex);
      v9 = this->mode;
      if ( v9 == MODE_EDIT_ATTACHMENT_TOP
        || v9 == MODE_EDIT_ATTACHMENT_BOTTOM
        || v9 == MODE_EDIT_ATTACHMENT_TRIGGER
        || v9 == MODE_EDIT_ATTACHMENT_MUZZLE )
      {
        attachMode = (char *)UIViewer::GetAttachmentSlotName(this);
        AttachmentIndex = UIViewer::GetAttachmentIndex(this, weapParams);
        attachRef = (char *)BG_UnlockablesGetItemAttachmentRef(weapParams->weaponIndex, AttachmentIndex);
        if ( !attachRef || !strcmp(attachRef, "none") )
          attachRef = attachMode;
        if ( !UIViewer::SetCameraPos(this, (char *)weaponRef, attachRef, 1)
          && !UIViewer::SetCameraPos(this, (char *)weaponRef, attachMode, 1)
          && !UIViewer::SetCameraPos(this, (char *)weaponRef, "none", 1)
          && !UIViewer::SetCameraPos(this, "default", attachRef, 1) )
        {
          UIViewer::SetCameraPos(this, "default", "none", 1);
        }
      }
      else
      {
        v4 = this->prevState.mode > 6u
          && this->mode != MODE_EDIT_WEAPON
          && this->mode != MODE_AAR_WEAPON
          && this->mode != MODE_CHOOSE_CLASS_WEAPON;
        if ( (this->mode != MODE_EDIT_TAG || !UIViewer::SetCameraPos(this, (char *)weaponRef, "tag", v4))
          && (this->mode != MODE_EDIT_EMBLEM || !UIViewer::SetCameraPos(this, (char *)weaponRef, "emblem", v4))
          && (this->mode != MODE_AAR_WEAPON || !UIViewer::SetCameraPos(this, (char *)weaponRef, "aar", v4))
          && (this->mode != MODE_CHOOSE_CLASS_WEAPON || !UIViewer::SetCameraPos(this, (char *)weaponRef, "ingame", v4))
          && !UIViewer::SetCameraPos(this, (char *)weaponRef, "none", v4) )
        {
          UIViewer::SetCameraPos(this, "default", "none", v4);
        }
      }
      memcpy(&this->prevState, &state, sizeof(this->prevState));
    }
  }
}

char __thiscall UIViewer::SetCameraPos(UIViewer *this, char *string, bool animate)
{
  long double v3; // st7
  char str[256]; // [esp+1Ch] [ebp-130h] BYREF
  int MAX_STRING; // [esp+120h] [ebp-2Ch]
  float at[3]; // [esp+124h] [ebp-28h] BYREF
  float vals[5]; // [esp+130h] [ebp-1Ch]
  const char *s; // [esp+144h] [ebp-8h]
  int i; // [esp+148h] [ebp-4h]

  MAX_STRING = 256;
  strncpy((unsigned __int8 *)str, (unsigned __int8 *)string, 0x100u);
  str[255] = 0;
  s = strtok(str, " ,");
  for ( i = 0; i < 5 && s; ++i )
  {
    v3 = atof(s);
    vals[i] = v3;
    s = strtok(0, " ,");
  }
  if ( i < 5 )
    return 0;
  at[0] = *(float *)&FLOAT_0_0;
  at[1] = vals[0];
  at[2] = vals[1];
  UIViewer::SetCameraPos(this, at, vals[2], vals[3], vals[4], animate);
  return 1;
}

char __thiscall UIViewer::SetCameraPos(UIViewer *this, char *weapon, char *attachment, bool animate)
{
  char *v5; // eax
  int column; // [esp+2Ch] [ebp-Ch]
  int i; // [esp+30h] [ebp-8h]
  int attachmentsRow; // [esp+34h] [ebp-4h]

  strncpy((unsigned __int8 *)this->lastCameraWeapon, (unsigned __int8 *)weapon, 0x20u);
  strncpy((unsigned __int8 *)this->lastCameraAttachment, (unsigned __int8 *)attachment, 0x20u);
  for ( i = 0; i < this->nCameraOverrides; ++i )
  {
    if ( !strcmp(this->cameraOverrides[i].weapon, weapon) && !strcmp(this->cameraOverrides[i].attachment, attachment) )
      return UIViewer::SetCameraPos(this, this->cameraOverrides[i].params, animate);
  }
  attachmentsRow = StringTable_LookupRowNumForValue(this->cameraPosTable, 0, "attachments");
  if ( attachmentsRow == -1 )
    return 0;
  column = StringTable_LookupColumnNumForValue(this->cameraPosTable, attachmentsRow, attachment);
  if ( column == -1 )
    return 0;
  v5 = (char *)StringTable_Lookup(this->cameraPosTable, 0, weapon, column);
  return UIViewer::SetCameraPos(this, v5, animate);
}

void __thiscall UIViewer::SetCameraPos(
        UIViewer *this,
        const float *lookat,
        float dist,
        float yaw,
        float pitch,
        bool animate)
{
  this->cameraAnim = animate;
  if ( this->cameraAnim )
  {
    this->cameraAnimStartTime = cls.realtime;
    this->cameraAnimStart = this->camera;
    this->cameraAnimEnd.lookAt[0] = *lookat;
    this->cameraAnimEnd.lookAt[1] = lookat[1];
    this->cameraAnimEnd.lookAt[2] = lookat[2];
    this->cameraAnimEnd.dist = dist;
    this->cameraAnimEnd.yaw = yaw;
    this->cameraAnimEnd.pitch = pitch;
  }
  else
  {
    this->camera.lookAt[0] = *lookat;
    this->camera.lookAt[1] = lookat[1];
    this->camera.lookAt[2] = lookat[2];
    this->camera.dist = dist;
    this->camera.yaw = yaw;
    this->camera.pitch = pitch;
    this->cameraAnimEnd = this->camera;
  }
}

void __thiscall UIViewer::InitFov(UIViewer *this)
{
  float value; // [esp+4h] [ebp-Ch]
  const char *str; // [esp+Ch] [ebp-4h]

  str = StringTable_Lookup(this->cameraPosTable, 0, "fov_x", 1);
  if ( str )
  {
    if ( *str )
    {
      value = atof(str);
      Dvar_SetFloat((dvar_s *)this->fov, value);
    }
  }
}

void __cdecl UIViewer::SetCameraPosCmd()
{
  const char *v0; // eax
  const char *v1; // eax
  const char *v2; // eax
  const char *v3; // eax
  const char *v4; // eax
  float dist; // [esp+10h] [ebp-50h]
  float yaw; // [esp+14h] [ebp-4Ch]
  float pitch; // [esp+18h] [ebp-48h]
  float v8; // [esp+34h] [ebp-2Ch]
  float v9; // [esp+38h] [ebp-28h]
  float lookat[3]; // [esp+54h] [ebp-Ch] BYREF

  if ( Cmd_Argc() == 6 )
  {
    v0 = Cmd_Argv(1);
    v8 = atof(v0);
    v1 = Cmd_Argv(2);
    v9 = atof(v1);
    lookat[0] = *(float *)&FLOAT_0_0;
    lookat[1] = v8;
    lookat[2] = v9;
    v2 = Cmd_Argv(5);
    pitch = atof(v2);
    v3 = Cmd_Argv(4);
    yaw = atof(v3);
    v4 = Cmd_Argv(3);
    dist = atof(v4);
    UIViewer::SetCameraPos(&uiViewer, lookat, dist, yaw, pitch, 0);
  }
  else
  {
    Com_Printf(16, "usage: uiViewerSetCameraPosition <camLookatY> <camLookatZ> <dist> <yaw> <pitch>\n");
  }
}

void __cdecl UIViewer::ResetCameraCmd()
{
  UIViewer::SetCameraPos(&uiViewer, uiViewer.lastCameraWeapon, uiViewer.lastCameraAttachment, 1);
}

void __cdecl UIViewer::SetControllerIndexCmd()
{
  uiViewer.controllerIndex = 0;
}

DObj *__cdecl UIViewer::GetDObj(int handle)
{
  return uiViewer.drawDobjs[handle].dobj;
}

void __thiscall UIViewer::Draw(UIViewer *this, int localClientNum, bool _ingame, unsigned int eyeToRender)
{
  if ( !this->inited )
    UIViewer::Init(this, _ingame);
  if ( _ingame != this->ingame
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ui\\ui_viewer.cpp", 2107, 0, "%s", "_ingame == ingame") )
  {
    __debugbreak();
  }
  if ( this->ingame || DB_IsZoneLoaded("common_mp") && DB_IsZoneLoaded("ui_viewer_mp") && comWorld.name )
  {
    if ( this->ingame && !this->mapLoaded || !this->ingame && !rgp.world )
      UIViewer::LoadMap(this);
    if ( this->showDvar->current.enabled )
      UIViewer::Show(this);
    else
      UIViewer::Hide(this);
    if ( this->show )
      UIViewer::DrawScene(this, eyeToRender);
  }
}

void __thiscall UIViewer::SetupStreamer(UIViewer *this)
{
  int colCount; // [esp+8h] [ebp-1Ch]
  XModel *model; // [esp+Ch] [ebp-18h]
  const char *modelName; // [esp+10h] [ebp-14h]
  int rowCount; // [esp+14h] [ebp-10h]
  const char *partType; // [esp+18h] [ebp-Ch]
  int row; // [esp+1Ch] [ebp-8h]
  int col; // [esp+20h] [ebp-4h]

  if ( this->inited && !this->streamerInited && Sys_IsDatabaseReady2() && DB_IsZoneLoaded("ui_viewer_mp") )
  {
    GC_InitWeaponOptions();
    R_StreamSetUIConfig(1);
    rowCount = StringTable_RowCount(this->bodyHeadTable);
    colCount = StringTable_ColumnCount(this->bodyHeadTable);
    DB_EnumXAssets(ASSET_TYPE_WEAPON, (void (__cdecl *)(XAssetHeader, void *))ForceLoadWeapon, 0, 0);
    for ( row = 0; row < rowCount; ++row )
    {
      partType = StringTable_GetColumnValueForRow(this->bodyHeadTable, row, 1);
      if ( !I_stricmp(partType, "body") || !I_stricmp(partType, "head_naked") )
      {
        for ( col = 2; col < colCount; ++col )
        {
          modelName = StringTable_GetColumnValueForRow(this->bodyHeadTable, row, col);
          if ( modelName && *modelName )
          {
            model = DB_FindXAssetHeader(ASSET_TYPE_XMODEL, modelName, 1, -1).model;
            if ( model )
              R_Stream_ForceLoadModel(model, 0);
          }
        }
      }
    }
    this->streamerInited = 1;
  }
}

void __cdecl ForceLoadWeapon(XAssetHeader header)
{
  WeaponDef *weaponDef; // [esp+4h] [ebp-4h]

  if ( !header.xmodelPieces
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\ui\\ui_viewer.cpp", 2143, 0, "%s", "variantDef") )
  {
    __debugbreak();
  }
  weaponDef = (WeaponDef *)header.xmodelPieces->pieces;
  if ( *weaponDef->gunXModel )
    R_Stream_ForceLoadModel(*weaponDef->gunXModel, 0);
  if ( *weaponDef->worldModel )
    R_Stream_ForceLoadModel(*weaponDef->worldModel, 0);
}

void __cdecl UI_ViewerDraw(unsigned int eyeToRender)
{
  if ( DB_IsZoneLoaded("ui_mp") )
    UIViewer::Draw(&uiViewer, 0, 0, eyeToRender);
}

void __cdecl UI_ViewerShutdown()
{
  UIViewer::Shutdown(&uiViewer);
}

void __cdecl UI_ViewerCheckStreamer()
{
  UIViewer::SetupStreamer(&uiViewer);
}

void __thiscall UIViewer::State::State(UIViewer::State *this)
{
  this->mode = -1;
  this->playerParams.bodyIndex = -1;
  this->playerParams.headIndex = -1;
  this->playerParams.facePatternIndex = -1;
  this->playerParams.faceColorIndex = -1;
  this->weaponParams.weaponSlot = -1;
  this->weaponParams.weaponIndex = -1;
  this->weaponParams.attachTopIndex = -1;
  this->weaponParams.attachBottomIndex = -1;
  this->weaponParams.attachTriggerIndex = -1;
  this->weaponParams.attachMuzzleIndex = -1;
  this->weaponParams.weaponOptions.i = -1;
  this->weaponParams.currentAttachmentPoint = -1;
  this->locked = 0;
  *(unsigned int *)this->faction = 0;
  *(unsigned int *)&this->faction[4] = 0;
  *(unsigned int *)&this->faction[8] = 0;
  *(unsigned int *)&this->faction[12] = 0;
  *(unsigned int *)&this->faction[16] = 0;
  *(unsigned int *)&this->faction[20] = 0;
  *(unsigned int *)&this->faction[24] = 0;
  *(unsigned int *)&this->faction[28] = 0;
}

