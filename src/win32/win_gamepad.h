#pragma once

void __cdecl GPad_InitAll();
void __cdecl GPad_UpdateAll();
int GPad_RefreshAll();
void __cdecl GPad_Check(int portIndex);
void __cdecl GPad_UpdateDigitals(int portIndex, const _XINPUT_GAMEPAD *xpad);
void __cdecl GPad_UpdateAnalogs(int portIndex, const _XINPUT_GAMEPAD *xpad);
void __cdecl GPad_UpdateSticks(int portIndex, const _XINPUT_GAMEPAD *xpad);
void __cdecl GPad_ConvertStickToFloat(__int16 inX, __int16 inY, float *outX, float *outY);
void __cdecl GPad_UpdateSticksDown(GamePad *gPad);
void __cdecl GPad_ResetState(int portIndex);
bool __cdecl GPad_IsActive(int portIndex);
double __cdecl GPad_GetButton(int portIndex, GamePadButton button);
bool __cdecl GPad_ButtonRequiresUpdates(int portIndex, GamePadButton button);
bool __cdecl GPad_IsButtonPressed(int portIndex, GamePadButton button);
bool __cdecl GPad_IsButtonReleased(int portIndex, GamePadButton button);
double __cdecl GPad_GetStick(int portIndex, GamePadStick stick);
bool __cdecl GPad_IsStickPressed(int portIndex, GamePadStick stick, GamePadStickDir stickDir);
bool __cdecl GPad_GetStickChangedToPressedState(
        int portIndex,
        GamePadStick stick,
        GamePadStickDir stickDir,
        bool pressedState);
bool __cdecl GPad_IsStickReleased(int portIndex, GamePadStick stick, GamePadStickDir stickDir);
