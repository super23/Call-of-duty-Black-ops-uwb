#pragma once

void __cdecl CG_HudElemRegisterDvars();
void __cdecl CG_TranslateHudElemMessage(
        int localClientNum,
        const char *message,
        const char *messageType,
        char *hudElemString);
char __cdecl ReplaceDirective(int localClientNum, unsigned int *searchPos, unsigned int *dstLen, char *dstString);
void __cdecl GetHudelemDirective(int localClientNum, char *directive, char *result);
void __cdecl DirectiveFakeIntroSeconds(int localClientNum, const char *arg0, char *result);
void __cdecl ParseDirective(char *directive, char *resultName, char *resultArg0);
char __cdecl WorldPosToScreenPos(int localClientNum, const float *worldPos, float *outScreenPos);
void __cdecl GetViewAxisProjections(
        const float *vieworg,
        const float (*viewaxis)[3],
        const float *worldPoint,
        float *projections);
char __cdecl WorldPosToExtraCamScreenPos(int localClientNum, const float *worldPos, float *outScreenPos);
bool __cdecl ClampScreenPosToEdges(
        int localClientNum,
        float *point,
        float padLeft,
        float padRight,
        float padTop,
        float padBottom,
        float *resultNormal,
        float *resultDist);
void __cdecl CG_Draw2dHudElems(int localClientNum, int foreground);
int __cdecl GetSortedHudElems(int localClientNum, hudelem_s **elems);
void __cdecl CopyInUseHudElems(hudelem_s **elems, int *elemCount, hudelem_s *elemSrcArray, int elemSrcArrayCount);
int __cdecl compare_hudelems(const void *pe0, const void *pe1);
int __cdecl GetSortedDebugHudElems(hudelem_s **elems);
void __cdecl CopyInUseDebugHudElems(
        hudelem_s **elems,
        int *elemCount,
        game_hudelem_s *elemSrcArray,
        int elemSrcArrayCount);
void __cdecl CG_Draw2dHudElemList(
        hudelem_s **elems,
        int elemCount,
        int localClientNum,
        int foreground,
        bool playerAlive,
        bool inKillCam);
void __cdecl DrawSingleHudElem2d(int localClientNum, const hudelem_s *elem);
ScopedScrPlaceViewStack *__thiscall ScopedScrPlaceViewStack::ScopedScrPlaceViewStack(
        ScopedScrPlaceViewStack *this,
        ScreenPlacementStack *stack,
        const ScreenPlacement *newPlacement);
void __thiscall ScopedScrPlaceViewStack::~ScopedScrPlaceViewStack(ScopedScrPlaceViewStack *this);
void __cdecl GetHudElemInfo(int localClientNum, const hudelem_s *elem, cg_hudelem_t *cghe, char *hudElemString);
void __cdecl SafeTranslateHudElemString(int localClientNum, int index, char *hudElemString);
double __cdecl HudElemStringWidth(const char *string, const hudelem_s *elem, const cg_hudelem_t *cghe);
char *__cdecl HudElemTimerString(const hudelem_s *elem, int timeNow);
int __cdecl GetHudElemTime(const hudelem_s *elem, int timeNow);
char *__cdecl HudElemTenthsTimerString(const hudelem_s *elem, int timeNow);
double __cdecl HudElemWidth(const ScreenPlacement *scrPlace, const hudelem_s *elem, const cg_hudelem_t *cghe);
double __cdecl HudElemMaterialWidth(const ScreenPlacement *scrPlace, const hudelem_s *elem, const cg_hudelem_t *cghe);
double __cdecl HudElemMaterialSpecifiedWidth(
        const ScreenPlacement *scrPlace,
        char alignScreen,
        int sizeVirtual,
        const cg_hudelem_t *cghe);
double __cdecl HudElemHeight(const ScreenPlacement *scrPlace, const hudelem_s *elem, const cg_hudelem_t *cghe);
double __cdecl HudElemMaterialHeight(const ScreenPlacement *scrPlace, const hudelem_s *elem, const cg_hudelem_t *cghe);
double __cdecl HudElemMaterialSpecifiedHeight(
        const ScreenPlacement *scrPlace,
        char alignScreen,
        int sizeVirtual,
        const cg_hudelem_t *cghe);
void __cdecl SetHudElemPos(const ScreenPlacement *scrPlace, const hudelem_s *elem, cg_hudelem_t *cghe);
void __cdecl GetHudElemOrg(
        const ScreenPlacement *scrPlace,
        int alignOrg,
        int alignScreen,
        float xVirtual,
        float yVirtual,
        float width,
        float height,
        float *orgX,
        float *orgY);
double __cdecl AlignHudElemX(int alignOrg, float x, float width);
double __cdecl AlignHudElemY(int alignOrg, float y, float height);
double __cdecl HudElemMovementFrac(const hudelem_s *elem, int timeNow);
void __cdecl ConsolidateHudElemText(const hudelem_s *elem, cg_hudelem_t *cghe, char *hudElemString);
void __cdecl CopyStringToHudElemString(char *string, char *hudElemString);
void __cdecl HudElemColorToVec4(const hudelem_color_t *hudElemColor, float *resultColor);
void __cdecl DrawHudElemString(
        int localClientNum,
        const ScreenPlacement *scrPlace,
        char *text,
        const hudelem_s *elem,
        cg_hudelem_t *cghe);
double __cdecl OffsetHudElemY(const hudelem_s *elem, const cg_hudelem_t *cghe, float offsetY);
void __cdecl DrawHudElemClock(int localClientNum, const hudelem_s *elem, const cg_hudelem_t *cghe);
void __cdecl DrawHudElemMaterial(int localClientNum, const hudelem_s *elem, cg_hudelem_t *cghe);
void __cdecl UpdateHudElemWarGameData(int localClientNum, const hudelem_s *elem);
// local variable allocation has failed, the output may be wrong!
void  DrawOffscreenViewableWaypoint(int a1@<ebp>, int localClientNum, const hudelem_s *elem);
double __cdecl GetScaleForDistance(int localClientNum, const float *worldPos);
void __cdecl WaypointTargetFade(int localClientNum, const hudelem_s *elem, const float *screenPos, float *color);
double __cdecl WaypointFadeGetTimeFrac(const cg_s *cgameGlob, int currentTime);
void __cdecl CG_AddDrawSurfsFor3dHudElems(int localClientNum);
void  AddDrawSurfForHudElemWaypoint(hudelem_color_t a1@<ebp>, int localClientNum, const hudelem_s *elem);
double __cdecl HudElemWaypointHeight(int localClientNum, const hudelem_s *elem);
void __cdecl RB_DrawWaypoints(int localClientNum);
void __cdecl RB_AddWaypoint(int localClientNum, const hudelem_s *elem);
