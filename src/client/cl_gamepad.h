#pragma once

void __cdecl CL_ResetLastGamePadEventTime();
void __cdecl CL_InitGamepadCommands();
void __cdecl Bind_GP_SticksConfigs_f();
void __cdecl Bind_GP_ButtonsConfigs_f();
void __cdecl CL_InitGamepadAxisBindings();
void __cdecl Axis_Bind_f();
int __cdecl Gamepad_StringToPhysicalAxis(const char *axisName);
int __cdecl Axis_StringToVirtualAxis(const char *axisName);
int __cdecl Gamepad_InputTypeStringToId(const char *name);
void __cdecl Gamepad_BindAxis(
        GpadAxesGlob *gaGlob,
        GamepadPhysicalAxis realIndex,
        GamepadVirtualAxis axisIndex,
        GamepadMapping mapType);
void __cdecl Axis_Unbindall_f();
void __cdecl Gamepad_WriteBindings(int localClientNum, int f);
const char *__cdecl Axis_NumToAxisString(GamepadPhysicalAxis axis);
const char *__cdecl Gamepad_VirtualAxisName(GamepadVirtualAxis axis);
const char *__cdecl Gamepad_InputTypeName(GamepadMapping mapType);
void __cdecl CL_GamepadEvent(int portIndex, unsigned int physicalAxis, int value);
void __cdecl CL_GamepadGenerateAPad(int localClientNum, int portIndex, unsigned int physicalAxis);
double __cdecl CL_GamepadAxisValue(int localClientNum, unsigned int virtualAxis);
void __cdecl CL_GamepadResetMenuScrollTime(int localClientNum, int key, int down, unsigned int time);
void __cdecl CL_GamepadButtonEvent(
        int localClientNum,
        int controllerIndex,
        int key,
        int buttonEvent,
        unsigned int time,
        GamePadButton gamePadButton);
bool __cdecl CL_CheckForIgnoreDueToRepeat(int localClientNum, int key, int repeatCount, int time);
char __cdecl IsGamepadKey(int key);
void __cdecl CL_GamepadButtonEventForPort(
        int portIndex,
        int key,
        int buttonEvent,
        unsigned int time,
        GamePadButton button);
