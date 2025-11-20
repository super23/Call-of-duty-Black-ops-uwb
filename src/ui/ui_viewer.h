#pragma once

void __cdecl UI_ViewerRotateLeftRepeatEnabled_f();
void __cdecl UI_ViewerRotateRightRepeatEnabled_f();
void __thiscall UIViewer::Init(UIViewer *this, bool _ingame);
void __thiscall UIViewer::Shutdown(UIViewer *this);
void __thiscall UIViewer::LoadMap(UIViewer *this);
void __thiscall UIViewer::Show(UIViewer *this);
void __thiscall UIViewer::State::Invalidate(UIViewer::State *this);
void __thiscall UIViewer::Hide(UIViewer *this);
void __thiscall UIViewer::UpdateCamera(UIViewer *this, refdef_s *refdef, float deltaTime);
void __thiscall UIViewer::CameraParams::SetRefDef(UIViewer::CameraParams *this, refdef_s *refdef);
DObj *__thiscall UIViewer::AllocDobj(UIViewer *this, unsigned int *entnum);
centity_s *__thiscall UIViewer::GetPlayerEntity(UIViewer *this);
XAnimTree_s *__thiscall UIViewer::GetAnimTree(UIViewer *this);
bgs_t *__thiscall UIViewer::GetBGS(UIViewer *this);
void __thiscall UIViewer::FreeDobjs(UIViewer *this);
int __thiscall UIViewer::GetLoadout(UIViewer *this, loadoutSlot_t loadoutSlot, bool defaultIfZero);
void __thiscall UIViewer::PlayerParams::PlayerParams(UIViewer::PlayerParams *this);
void __thiscall UIViewer::WeaponParams::WeaponParams(UIViewer::WeaponParams *this, UIViewer::WeaponSlot slot);
void __thiscall UIViewer::Update(UIViewer *this, float deltaTime);
void __thiscall UIViewer::DrawScene(UIViewer *this, unsigned int eyeToRender);
void __thiscall UIViewer::DrawDobj::Init(UIViewer::DrawDobj *this);
void __thiscall UIViewer::DrawDobj::Set(
        UIViewer::DrawDobj *this,
        const UIViewer::WeaponParams *weapParam,
        const WeaponVariantDef *wvd,
        bool hero);
void __thiscall UIViewer::DrawDobj::Set(
        UIViewer::DrawDobj *this,
        const UIViewer::PlayerParams *playerParams,
        const UIViewer::WeaponParams *weapParam,
        const WeaponVariantDef *wvd,
        bool hero,
        bool lock);
char __thiscall UIViewer::DrawDobj::Draw(UIViewer::DrawDobj *this, float deltaTime, float lightMultiplier);
const WeaponVariantDef *__thiscall UIViewer::GetWeaponVariantDef(
        UIViewer *this,
        const UIViewer::WeaponParams *weapParams);
const WeaponVariantDef *__thiscall UIViewer::AddWeaponXmodel(
        UIViewer *this,
        const UIViewer::WeaponParams *weapParams,
        DObjModel_s *dobjModel,
        bool world);
const WeaponVariantDef *__thiscall UIViewer::AddWeaponXmodel(
        UIViewer *this,
        const WeaponVariantDef *weaponVariantDef,
        DObjModel_s *dobjModel,
        bool world);
void __thiscall UIViewer::HideWeaponTags(
        UIViewer *this,
        DObj *dobj,
        const WeaponVariantDef *weapon,
        unsigned int *partBits,
        bool dualWield);
void __thiscall UIViewer::HideWeaponTags(
        UIViewer *this,
        DObj *dobj,
        const unsigned __int16 *hideTags,
        unsigned int *partBits);
void __thiscall UIViewer::UnhideWeaponTags(
        UIViewer *this,
        DObj *dobj,
        const unsigned __int16 *hideTags,
        unsigned int *partBits);
char __thiscall UIViewer::AddBodyPart(
        UIViewer *this,
        const char *partName,
        const char *partType,
        DObjModel_s *dobjModel,
        const char *bone);
void __thiscall UIViewer::UpdatePlayerDObj(
        UIViewer *this,
        const UIViewer::PlayerParams *playerParams,
        const WeaponVariantDef *weaponVariantDef,
        const UIViewer::WeaponParams *weapParams,
        bool locked);
void __thiscall UIViewer::UpdatePlayerAnim(UIViewer *this, const WeaponVariantDef *weaponVariantDef);
const char *__thiscall UIViewer::GetFaction(UIViewer *this);
void __thiscall UIViewer::AddPlayerToScene(
        UIViewer *this,
        const UIViewer::PlayerParams *playerParams,
        const UIViewer::WeaponParams *weapParams,
        bool locked);
bool __thiscall UIViewer::State::operator!=(UIViewer::State *this, const UIViewer::State *o);
bool __thiscall UIViewer::State::HasPlayerChanged(UIViewer::State *this, const UIViewer::State *o);
int *__thiscall UIViewer::GetAttachmentIndexPtr(UIViewer *this, UIViewer::WeaponParams *weapParams);
eAttachmentPoint __cdecl GetCurrentAttachPoint();
int __thiscall UIViewer::GetAttachmentIndex(UIViewer *this, UIViewer::WeaponParams *weapParams);
void __thiscall UIViewer::SetAttachmentIndex(UIViewer *this, UIViewer::WeaponParams *weapParams, int index);
void __thiscall UIViewer::ClearAttachmentIndex(UIViewer *this, UIViewer::WeaponParams *weapParams);
const char *__thiscall UIViewer::GetAttachmentSlotName(UIViewer *this);
void __thiscall UIViewer::AddWeaponToScene(
        UIViewer *this,
        UIViewer::WeaponParams *weapParams,
        UIViewer::WeaponParams *baseWeapParams);
char __thiscall UIViewer::SetCameraPos(UIViewer *this, char *string, bool animate);
char __thiscall UIViewer::SetCameraPos(UIViewer *this, char *weapon, char *attachment, bool animate);
void __thiscall UIViewer::SetCameraPos(
        UIViewer *this,
        const float *lookat,
        float dist,
        float yaw,
        float pitch,
        bool animate);
void __thiscall UIViewer::InitFov(UIViewer *this);
void __cdecl UIViewer::SetCameraPosCmd();
void __cdecl UIViewer::ResetCameraCmd();
void __cdecl UIViewer::SetControllerIndexCmd();
DObj *__cdecl UIViewer::GetDObj(int handle);
void __thiscall UIViewer::Draw(UIViewer *this, int localClientNum, bool _ingame, unsigned int eyeToRender);
void __thiscall UIViewer::SetupStreamer(UIViewer *this);
void __cdecl ForceLoadWeapon(XAssetHeader header);
void __cdecl UI_ViewerDraw(unsigned int eyeToRender);
void __cdecl UI_ViewerShutdown();
void __cdecl UI_ViewerCheckStreamer();
void __thiscall UIViewer::State::State(UIViewer::State *this);
