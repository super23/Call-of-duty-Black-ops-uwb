#pragma once

void __cdecl CL_RunNetworkFrame(int localClientNum);
char __cdecl CL_AnyLocalClientsRunning();
void __cdecl CL_DontUnlockControllers(bool flag);
void __cdecl CL_Command_ProvisionallyDisableAllClients();
void __cdecl CL_Command_DisableAllButPrimaryClients();
void __cdecl CL_Command_DisableAllClients();
void __cdecl CL_Command_SetClientBeingUsedAndPrimary();
void __cdecl CL_Command_SetClientBeingUsedAndPrimaryAndActive();
void __cdecl CL_Command_SetClientPrimary();
void __cdecl CL_Command_SetClientBeingUsed();
void __cdecl CL_Command_SetClientBeingUsedAndActive();
void __cdecl CL_Command_SetClientNotBeingUsed();
void __cdecl CL_SetupClientsForIngame();
void __cdecl CL_SetupClientsForFrontend();
void __cdecl CL_DrawFramedPicPhysical(
        float x,
        float y,
        float w,
        float h,
        float thicknessW,
        float thicknessH,
        float thicknessTex,
        int sides,
        const float *color,
        Material *material);
void __cdecl CL_RegisterCommands();
void __cdecl CL_UpdateDvarsFromProfile();
void __cdecl CL_UpdateGamerProfile();
void __cdecl CL_CommitDvarToProfiles();
void __cdecl CL_UpdateProfileFromStats();
void __cdecl CL_DrawSpinnerPhysical(float x, float y, float w, float h, const float *color);
void __cdecl CL_DrawSpinner(
        const ScreenPlacement *scrPlace,
        float x,
        float y,
        float w,
        float h,
        int horzAlign,
        int vertAlign,
        const float *color);
void __cdecl CL_DrawSpinnerLoadbarPhysical(float x, float y, float w, float h, const float *color, float percentDone);
void __cdecl CL_DrawSpinnerLoadbar(
        const ScreenPlacement *scrPlace,
        float x,
        float y,
        float w,
        float h,
        int horzAlign,
        int vertAlign,
        const float *color,
        float percentDone);
void __cdecl CL_RegisterDvars();
