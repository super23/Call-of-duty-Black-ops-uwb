#pragma once

struct animStringItem_t // sizeof=0x8
{                                       // XREF: BG_AddVehicleName(char const *,int,int,int,int)+19A/w
    const char *string;                 // XREF: BG_InitWeaponString(int,char const *)+9/w
    int hash;                           // XREF: BG_InitWeaponString(int,char const *)+1F/w
};

struct animScriptCondition_t // sizeof=0x8
{                                       // XREF: animScriptItem_t/r
    int index;
    unsigned int value;
};

struct animScriptCommand_t // sizeof=0x14
{                                       // XREF: animScriptItem_t/r
    __int16 bodyPart[2];
    __int16 animIndex[2];
    unsigned __int16 animDuration[2];
    snd_alias_list_t *soundAlias;
    unsigned __int16 tagName;           // XREF: _Com_InitHunkMemory+BD/o
                                        // Sys_GetPhysicalCpuCount+131/o ...
    unsigned __int16 flags;
};

struct animScriptItem_t // sizeof=0xC8
{                                       // XREF: animScriptData_t/r
                                        // BG_AnimParseAnimScript/r
    int numConditions;
    animScriptCondition_t conditions[4];
    int numCommands;
    animScriptCommand_t commands[8];    // XREF: _Com_InitHunkMemory+BD/o
                                        // Sys_GetPhysicalCpuCount+131/o ...
};


int __cdecl BG_StringHashValue(const char *fname);
void BG_AnimParseError(const char *msg, ...);
unsigned int __cdecl BG_AnimationIndexForString(const char *string);
int __cdecl BG_IndexForString(const char *token, animStringItem_t *strings, int allowFail);
const char *__cdecl BG_StringForIndex(int index, animStringItem_t *strings, int allowFail);
void __cdecl BG_InitWeaponString(int index, const char *name);
void __cdecl BG_InitWeaponStrings();
void __cdecl BG_AddVehicleName(const char *vehName, int vehType, int drivers, int gunners, int passengers);
unsigned int __cdecl BG_AnimGetConditionValue(animScriptItem_t *scriptItem, int conditionIndex);
void __cdecl BG_InitVehicles();
int __cdecl BG_GetVehicleAnimSetIndex(unsigned __int16 animSetStr);
void __cdecl BG_ParseCommands(const char **input, animScriptItem_t *scriptItem, animScriptData_t *scriptData);
int __cdecl GetValueForBitfield(unsigned int bitfield);
bool __cdecl Com_BitCheckAssert(const unsigned int *array, int bitNum, int size);
int __cdecl BG_PlayAnim(
        playerState_s *ps,
        int animNum,
        animBodyPart_t bodyPart,
        int forceDuration,
        int setTimer,
        int isContinue,
        int force);
const char *__cdecl BG_GetCurrentStance(playerState_s *ps);
const char *__cdecl BG_GetCurrentMoveStatus(playerState_s *ps);
const char *__cdecl BG_GetCurrentDirection(playerState_s *ps);
const char *__cdecl BG_GetCurrentDmgDirection(playerState_s *ps);
const char *__cdecl BG_GetCurrentDmgType(playerState_s *ps);
const char *__cdecl BG_GetCurrentSlope(playerState_s *ps);
const char *__cdecl BG_GetCurrentWeaponName(playerState_s *ps);
const char *__cdecl BG_GetCurrentWeaponClass(playerState_s *ps);
char *__cdecl BG_GetCurrentPlayerAnimType(playerState_s *ps);
int __cdecl BG_ExecuteCommand(
        playerState_s *ps,
        animScriptCommand_t *scriptCommand,
        int setTimer,
        int isContinue,
        int force);
int __cdecl BG_AnimScriptAnimation(pmove_t *pm, aistateEnum_t state, scriptAnimMoveTypes_t movetype, int force);
animScriptItem_t *__cdecl BG_FirstValidItem(unsigned int client, animScript_t *script);
int __cdecl BG_EvaluateConditions(clientInfo_t *ci, animScriptItem_t *scriptItem);
const char *__cdecl GetWeaponTypeName(int type);
char *__cdecl BG_GetConditionString(int condition, int value);
const char *__cdecl GetBodyPart(int bodypart);
animScriptCommand_t *__cdecl BG_AnimScriptEventGetCommand(playerState_s *ps, scriptAnimEventTypes_t event);
int __cdecl BG_AnimScriptEvent(pmove_t *pm, scriptAnimEventTypes_t event, int isContinue, int force);
void __cdecl BG_SetConditionValue(unsigned int client, unsigned int condition, unsigned __int64 value);
void __cdecl BG_SetConditionBit(unsigned int client, int condition, int value);
void __cdecl Com_BitSetAssert(unsigned int *array, int bitNum, int size);
unsigned int __cdecl BG_GetConditionValue(const clientInfo_t *ci, unsigned int condition);
unsigned int __cdecl BG_GetConditionBit(const clientInfo_t *ci, unsigned int condition);
scriptAnimMoveTypes_t __cdecl BG_GetAnimMoveType(const clientInfo_t *ci);
animation_s *__cdecl BG_GetAnimationForIndex(int client, unsigned int index);
int __cdecl BG_GetPlayerStateWeaponIndex(const pmove_t *pm);
void __cdecl BG_AnimUpdatePlayerStateConditions(pmove_t *pmove);
bool __cdecl BG_IsCrouchingAnim(const clientInfo_t *ci, int animNum);
bool __cdecl BG_IsAds(const clientInfo_t *ci, int animNum);
bool __cdecl BG_IsProneAnim(const clientInfo_t *ci, int animNum);
bool __cdecl BG_IsKnifeMeleeAnim(const clientInfo_t *ci, int animNum);
bool __cdecl BG_IsGrenadeAnim(const clientInfo_t *ci, int animNum);
bool __cdecl BG_IsSliding(const clientInfo_t *ci);
void __cdecl BG_LerpOffset(float *offset_goal, float maxOffsetChange, float *offset);
double __cdecl I_rsqrt(int number);
void __cdecl BG_Player_DoControllersSetup(const entityState_s *es, clientInfo_t *ci, int frametime);
void __cdecl BG_Player_DoControllersInternal(const entityState_s *es, const clientInfo_t *ci, controller_info_t *info);
void __cdecl BG_LerpAngles(float *angles_goal, float maxAngleChange, float *angles);
void __cdecl BG_PlayerAnimation(int localClientNum, const entityState_s *es, clientInfo_t *ci);
void __cdecl BG_RunLerpFrameRate(
        int localClientNum,
        clientInfo_t *ci,
        lerpFrame_t *lf,
        int newAnimation,
        const entityState_s *es);
void __cdecl BG_SetNewAnimation(
        int localClientNum,
        clientInfo_t *ci,
        lerpFrame_t *lf,
        int newAnimation,
        const entityState_s *es);
void __cdecl BG_PlayerAnimation_VerifyAnim(XAnimTree_s *pAnimTree, lerpFrame_t *lf);
void __cdecl BG_PlayerAngles(const entityState_s *es, clientInfo_t *ci);
void __cdecl BG_SwingAngles(
        float destination,
        float swingTolerance,
        float clampTolerance,
        float speed,
        float *angle,
        int *swinging);
void __cdecl BG_AnimPlayerConditions(const entityState_s *es);
void __cdecl BG_UpdatePlayerDObj(
        int localClientNum,
        DObj *pDObj,
        entityState_s *es,
        clientInfo_t *ci,
        int attachIgnoreCollision);
int __cdecl BG_GetPlayerWeaponForDObj(int localClientNum, entityState_s *es);
int __cdecl BG_GetPlayerOffhandWeaponForDObj(int localClientNum, entityState_s *es);
void __cdecl BG_FindAnimTrees();
scr_animtree_t __cdecl BG_FindAnimTree(const char *filename, int bEnforceExists);
void __cdecl BG_LoadAnim(const char *levelName);
void __cdecl BG_AnimParseAnimScript(
        animScriptData_t *scriptData,
        loadAnim_t *pLoadAnims,
        unsigned int *piNumAnims,
        const char *levelName);
char *__cdecl BG_CopyStringIntoBuffer(const char *string, char *buffer, unsigned int bufSize, unsigned int *offset);
void __cdecl BG_ParseConditionBits(
        const char **text_pp,
        animStringItem_t *stringTable,
        int condIndex,
        unsigned int *result);
void __cdecl BG_SetAnimConditionFlags(int condIndex, unsigned int result);
int __cdecl BG_ParseConditions(const char **text_pp, animScriptItem_t *scriptItem);
void BG_FindAnims();
void __cdecl BG_PostLoadAnim(const char *levelName);
void __cdecl BG_FinalizePlayerAnims(const char *levelName);
loadAnim_t *__cdecl BG_LoadAnimForAnimIndex(unsigned int iAnimIndex);
void __cdecl BG_SetupAnimNoteTypes(animScriptData_t *scriptData);
void __cdecl AssignToSmallerType<short>(__int16 *dest, int src);
