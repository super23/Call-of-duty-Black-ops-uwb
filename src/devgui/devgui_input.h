#pragma once

void __cdecl DevGui_InputInit();
void __cdecl DevGui_InputShutdown();
void __cdecl DevGui_SelectGamepad(int gamePadIndex);
char __cdecl DevGui_InputUpdate(int localClientNum, float deltaTime);
void __cdecl DevGui_UpdateScrollInputs(int localClientNum);
void __cdecl DevGui_UpdateScrollStates(float deltaTime, DevGuiInputState *states, float *axis, float *times);
void __cdecl DevGui_UpdateMenuScroll(float deltaTime);
char __cdecl DevGui_InputUpdateGamepad();
void DevGui_InputUpdateMouse();
void __cdecl DevGui_MouseEvent(int dx, int dy);
__int16 __cdecl DevGui_GetMenuScroll(DevGuiInputAxis axis);
__int64 __cdecl DevGui_UpdateIntScroll(float deltaTime, __int64 value, __int64 min, __int64 max, DevGuiInputAxis axis);
double __cdecl DevGui_UpdateFloatScroll(
        float deltaTime,
        float value,
        float min,
        float max,
        float step,
        DevGuiInputAxis axis);
bool __cdecl DevGui_IsButtonDown(DevGuiInputButton button);
bool __cdecl DevGui_IsButtonPressed(DevGuiInputButton button);
bool __cdecl DevGui_IsButtonReleased(DevGuiInputButton button);
