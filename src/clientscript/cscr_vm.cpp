#include "cscr_vm.h"
#include "cscr_stringlist.h"
#include "cscr_compiler.h"
#include <qcommon/cmd.h>
#include "cscr_parser.h"
#include "scr_const.h"
#include "cscr_readwrite.h"
#include "cscr_memorytree.h"

#include <cstring>
#include <xmmintrin.h>
#include <qcommon/common.h>
#include <minwindef.h>
#include "cscr_evaluate.h"
#include <win32/win_main.h>
#include <csetjmp>
#include <cgame/cg_scr_main.h>
#include <game_mp/g_spawn_mp.h>
#include <win32/win_shared.h>
#include <monkey/monkey.h>
#include <gfx_d3d/r_draw_cmdbuf.h>

#undef GetObject

scrVmGlob_t gScrVmGlob[2];
scrVmPub_t gScrVmPub[2];
scrVmDebugPub_t gScrVmDebugPub[2];

function_stack_t gFs[2];
int gThreadCount[2];
int gOpcode[2];
int gCaseCount[2];

int gScrExecuteTime[2];

const dvar_t *logScriptTimes;
const dvar_t *scrVmEnableScripts;
const dvar_t *scrShowVarUseage;
const dvar_t *scrShowStrUsage;
const dvar_t *sv_clientside;

unsigned int __cdecl VM_Execute_0(scriptInstance_t inst);

void __cdecl Scr_AddStruct()
{
    Scr_AddStruct(SCRIPTINSTANCE_SERVER);
}

void __cdecl Scr_ResetTimeout()
{
    Scr_ResetTimeout(SCRIPTINSTANCE_SERVER);
}

void __cdecl RemoveRefToValue(scriptInstance_t inst, int type, VariableUnion u)
{
    unsigned int value; // [esp+4h] [ebp-4h]

    value = type - 1;
    if ( (unsigned int)(type - 1) < 4 )
    {
        if ( type == 1 )
        {
            RemoveRefToObject(inst, u.stringValue);
        }
        else if ( value > 2 )
        {
            if ( type != 4
                && !Assert_MyHandler(
                            "c:\\projects_pc\\cod\\codsrc\\src\\clientscript\\scr_variable.h",
                            446,
                            0,
                            "%s\n\t(value) = %i",
                            "(value == VAR_VECTOR - VAR_BEGIN_REF)",
                            value) )
            {
                __debugbreak();
            }
            RemoveRefToVector(inst, u.vectorValue);
        }
        else
        {
            SL_RemoveRefToString(inst, u.stringValue);
        }
    }
}

void __cdecl Scr_ClearErrorMessage(scriptInstance_t inst)
{
    gScrVarPub[inst].error_message = 0;
    gScrVmGlob[inst].dialog_error_message = 0;
    gScrVarPub[inst].error_index = 0;
}

void __cdecl Scr_Init(scriptInstance_t inst)
{
    if ( gScrVarPub[inst].bInited
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    618,
                    0,
                    "%s",
                    "!gScrVarPub[inst].bInited") )
    {
        __debugbreak();
    }
    Scr_InitClassMap(inst);
    Scr_VM_Init(inst);
    gScrCompilePub[inst].script_loading = 0;
    gScrAnimPub[inst].animtree_loading = 0;
    gScrCompilePub[inst].scriptsPos = 0;
    gScrCompilePub[inst].scriptsCount = 0;
    gScrCompilePub[inst].loadedscripts = 0;
    gScrAnimPub[inst].animtrees = 0;
    gScrCompilePub[inst].builtinMeth = 0;
    gScrCompilePub[inst].builtinFunc = 0;
    gScrVarPub[inst].bInited = 1;
}

cmd_function_s VM_DumpScriptProfileInfo_VAR;

void __cdecl Scr_VM_Init(scriptInstance_t inst)
{
    gScrVarPub[inst].varUsagePos = "<script init variable>";
    gScrVmPub[inst].maxstack = &gScrVmPub[inst].stack[2047];
    gScrVmPub[inst].top = gScrVmPub[inst].stack;
    gScrVmPub[inst].function_count = 0;
    gScrVmPub[inst].function_frame = gScrVmPub[inst].function_frame_start;
    gScrVmPub[inst].localVars = (unsigned int *)&gScrVmGlob[inst].starttime;
    gScrVarPub[inst].evaluate = 0;
    gScrVmPub[inst].debugCode = 0;
    Scr_ClearErrorMessage(inst);
    gScrVmPub[inst].terminal_error = 0;
    gScrVmPub[inst].outparamcount = 0;
    gScrVmPub[inst].inparamcount = 0;
    gScrVarPub[inst].tempVariable = AllocValue(inst);
    gScrVarPub[inst].timeArrayId = 0;
    gScrVarPub[inst].pauseArrayId = 0;
    gScrVarPub[inst].levelId = 0;
    gScrVarPub[inst].gameId = 0;
    gScrVarPub[inst].animId = 0;
    gScrVarPub[inst].freeEntList = 0;
    gScrVmPub[inst].stack[0].type = 7;
    gScrVmGlob[inst].loading = 0;
    gScrVmGlob[inst].recordPlace = 0;
    gScrVmGlob[inst].lastFileName = 0;
    gScrVmGlob[inst].lastLine = 0;
    gScrVarPub[inst].ext_threadcount = 0;
    gScrVarPub[inst].numScriptThreads = 0;
    gScrVarPub[inst].varUsagePos = 0;
    logScriptTimes = _Dvar_RegisterBool("logScriptTimes", 0, 0, "Log times for every print called from script");
    scrVmEnableScripts = _Dvar_RegisterBool("scrVmEnableScripts", 1, 0, "Enables script execution");
    scrShowVarUseage = _Dvar_RegisterBool("scrShowVarUseage", 0, 0, "Displays var useage at compile time."); // they seriously spelled it this way
    scrShowStrUsage = _Dvar_RegisterBool("scrShowStrUsage", 0, 0, "Displays script string usage at compile time.");
    if (inst == SCRIPTINSTANCE_SERVER)
        sv_clientside = _Dvar_RegisterBool(
            "sv_clientside",
            0,
            0,
            "Used to toggle systems in script on and off on the server.");
    //Cmd_AddCommandInternal("scrProfileInfo", KISAK_NULLSUB, &VM_DumpScriptProfileInfo_VAR);
}

void __cdecl Scr_Settings(int developer, int developer_script, int abort_on_error, scriptInstance_t inst)
{
    if (abort_on_error
        && !developer
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            639,
            0,
            "%s",
            "!abort_on_error || developer"))
    {
        __debugbreak();
    }
    gScrVarPub[inst].developer = developer != 0;
    gScrVarPub[inst].developer_script = developer_script != 0;
    gScrVmPub[inst].abort_on_error = abort_on_error != 0;
}

void __cdecl Scr_Shutdown(scriptInstance_t inst)
{
    if (gScrVarPub[inst].bInited)
    {
        gScrVarPub[inst].bInited = 0;
        VM_Shutdown(inst);
        Scr_ShutdownVariables(inst);
    }
}

void __cdecl VM_Shutdown(scriptInstance_t inst)
{
    if (gScrVarPub[inst].tempVariable)
    {
        FreeValue(inst, gScrVarPub[inst].tempVariable);
        gScrVarPub[inst].tempVariable = 0;
    }
}

void __cdecl Scr_SetLoading(int bLoading, scriptInstance_t inst)
{
    gScrVmGlob[inst].loading = bLoading;
}

int __cdecl Scr_GetNumScriptThreads(scriptInstance_t inst)
{
    return gScrVarPub[inst].numScriptThreads;
}

void __cdecl Scr_ClearOutParams(scriptInstance_t inst)
{
    while (gScrVmPub[inst].outparamcount)
    {
        RemoveRefToValue(inst, gScrVmPub[inst].top->type, gScrVmPub[inst].top->u);
        --gScrVmPub[inst].top;
        --gScrVmPub[inst].outparamcount;
    }
}

char *__cdecl Scr_GetReturnPos(scriptInstance_t inst, unsigned int *localId)
{
    char *pos; // [esp+0h] [ebp-4h]

    if (gScrVmPub[inst].function_count <= 1)
        return 0;
    pos = (char *)gScrVmPub[inst].function_frame[-1].fs.pos;
    if (pos == &g_EndPos)
        return 0;
    *localId = gScrVmPub[inst].function_frame[-1].fs.localId;
    return pos;
}

char *__cdecl Scr_GetNextCodepos(
    scriptInstance_t inst,
    VariableValue *top,
    const char *pos,
    int opcode,
    int mode,
    unsigned int *localId)
{
    char *result; // eax
    int type; // edx
    int v8; // edx
    const char *v9; // eax
    int v10; // [esp+4h] [ebp-84h]
    const char *v11; // [esp+38h] [ebp-50h]
    int v12; // [esp+40h] [ebp-48h]
    unsigned __int16 v13; // [esp+44h] [ebp-44h]
    unsigned __int16 v14; // [esp+54h] [ebp-34h]
    unsigned __int16 v15; // [esp+5Ch] [ebp-2Ch]
    unsigned int caseValue; // [esp+74h] [ebp-14h]
    int caseCount; // [esp+7Ch] [ebp-Ch]
    VariableValue value; // [esp+80h] [ebp-8h] BYREF
    const char *posb; // [esp+98h] [ebp+10h]
    const char *posa; // [esp+98h] [ebp+10h]
    const char *posc; // [esp+98h] [ebp+10h]

    *localId = gScrVmPub[inst].function_frame->fs.localId;
    while (2)
    {
        ++pos;
        if (gScrVarPub[inst].error_message
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                2958,
                0,
                "%s",
                "!gScrVarPub[inst].error_message"))
        {
            __debugbreak();
        }
        if (!top && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 2959, 0, "%s", "top"))
            __debugbreak();
        if (!pos && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 2960, 0, "%s", "pos"))
            __debugbreak();
        if (!gScrVarPub[inst].evaluate
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                2961,
                0,
                "%s",
                "gScrVarPub[inst].evaluate"))
        {
            __debugbreak();
        }
        if (mode == 2)
        {
            switch (opcode)
            {
            case 'O':
            case 'P':
            case 'T':
                goto $LN46_5;
            case 'Q':
            case 'U':
                goto $LN42_9;
            case 'R':
            case 'V':
                if (top->type != 1)
                    goto LABEL_23;
            $LN46_5:
                if (gScrVmPub[inst].function_count >= 32)
                    goto LABEL_23;
                *localId = 0;
                result = *(char **)pos;
                break;
            case 'S':
            case 'W':
                if (top[-1].type != 1)
                    goto LABEL_23;
            $LN42_9:
                if (top->type != 9 || gScrVmPub[inst].function_count >= 32)
                    goto LABEL_23;
                *localId = 0;
                result = (char *)top->u.intValue;
                break;
            default:
                goto LABEL_23;
            }
        }
        else
        {
        LABEL_23:
            switch (opcode)
            {
            case 0:
            case 1:
                return Scr_GetReturnPos(inst, localId);
            case 2:
            case 3:
            case 13:
            case 14:
            case 15:
            case 16:
            case 17:
            case 18:
            case 20:
            case 24:
            case 25:
            case 26:
            case 27:
            case 28:
            case 29:
            case 32:
            case 33:
            case 35:
            case 36:
            case 37:
            case 38:
            case 49:
            case 52:
            case 53:
            case 54:
            case 57:
            case 60:
            case 76:
            case 77:
            case 78:
            case 81:
            case 83:
            case 88:
            case 89:
            case 91:
            case 92:
            case 93:
            case 100:
            case 101:
            case 102:
            case 103:
            case 104:
            case 105:
            case 106:
            case 107:
            case 108:
            case 109:
            case 110:
            case 111:
            case 112:
            case 113:
            case 114:
            case 115:
            case 116:
            case 117:
            case 118:
            case 120:
            case 121:
            case 122:
            case 123:
            case 126:
            case 127:
            case 128:
            case 135:
            case 136:
            case 137:
                goto LABEL_76;
            case 4:
            case 5:
            case 23:
            case 30:
            case 31:
            case 34:
            case 50:
            case 51:
            case 55:
            case 61:
            case 90:
            case 119:
            case 133:
            case 134:
                ++pos;
                goto LABEL_76;
            case 6:
            case 7:
            case 62:
            case 63:
            case 64:
            case 65:
            case 66:
            case 67:
            case 69:
            case 70:
            case 71:
            case 72:
            case 73:
            case 74:
                pos += 2;
                goto LABEL_76;
            case 8:
            case 19:
            case 21:
            case 79:
            case 80:
            case 82:
            case 85:
            case 87:
                pos += 4;
                goto LABEL_76;
            case 9:
                pos += 4;
                goto LABEL_76;
            case 10:
            case 11:
            case 22:
            case 39:
            case 40:
            case 41:
            case 42:
            case 43:
            case 44:
            case 45:
            case 46:
            case 47:
            case 48:
            case 56:
            case 58:
            case 59:
            case 130:
            case 131:
            case 132:
                pos += 2;
                goto LABEL_76;
            case 12:
                pos += 12;
                goto LABEL_76;
            case 68:
            case 75:
                pos += 3;
                goto LABEL_76;
            case 84:
            case 86:
            case 129:
                pos += 8;
                goto LABEL_76;
            case 94:
            case 96:
                type = top->type;
                value.u.intValue = top->u.intValue;
                value.type = type;
                AddRefToValue(inst, type, value.u);
                Scr_CastBool(inst, &value);
                v15 = *(_WORD *)pos;
                pos += 2;
                if (gScrVarPub[inst].error_message)
                    goto LABEL_76;
                if (value.type != 6
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        3017,
                        0,
                        "%s",
                        "value.type == VAR_INTEGER"))
                {
                    __debugbreak();
                }
                if (value.u.intValue)
                    return (char *)pos;
                else
                    return (char *)&pos[v15];
            case 95:
            case 97:
                v8 = top->type;
                value.u.intValue = top->u.intValue;
                value.type = v8;
                AddRefToValue(inst, v8, value.u);
                Scr_CastBool(inst, &value);
                v14 = *(_WORD *)pos;
                pos += 2;
                if (gScrVarPub[inst].error_message)
                    goto LABEL_76;
                if (value.type != 6
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        3033,
                        0,
                        "%s",
                        "value.type == VAR_INTEGER"))
                {
                    __debugbreak();
                }
                if (value.u.intValue)
                    return (char *)&pos[v14];
                else
                    return (char *)pos;
            case 98:
                return (char *)&pos[*(_DWORD *)pos + 4];
            case 99:
                return (char *)&pos[-*(unsigned __int16 *)pos + 2];
            case 124:
                posb = &pos[*(_DWORD *)pos + 4];
                v13 = *(_WORD *)posb;
                posa = posb + 2;
                caseCount = v13;
                v10 = top->type;
                if (v10 == 2)
                {
                    caseValue = top->u.intValue;
                }
                else
                {
                    if (v10 != 6)
                        return (char *)&posa[8 * v13];
                    if (!IsValidArrayIndex(inst, top->u.intValue))
                        return (char *)&posa[8 * v13];
                    caseValue = GetInternalVariableIndex(inst, top->u.intValue);
                }
                if (!v13)
                    return (char *)posa;
                if (!caseValue
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        3081,
                        0,
                        "%s",
                        "caseValue"))
                {
                    __debugbreak();
                }
                break;
            case 125:
                return (char *)&pos[8 * *(unsigned __int16 *)pos + 2];
            default:
                v9 = va("unknown opcode %d", opcode);
                if (!Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3269, 0, v9))
                    __debugbreak();
            LABEL_76:
                Scr_ClearErrorMessage(inst);
                opcode = *pos;
                if (opcode == 57)
                    continue;
                return (char *)pos;
            }
            do
            {
                v12 = *(_DWORD *)posa;
                posc = posa + 4;
                v11 = *(const char **)posc;
                posa = posc + 4;
                if (v12 == caseValue)
                {
                    if (!v11
                        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3090, 0, "%s", "pos"))
                    {
                        __debugbreak();
                    }
                    return (char *)v11;
                }
                --caseCount;
            } while (caseCount);
            if (!v12)
            {
                posa = v11;
                if (!v11
                    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3100, 0, "%s", "pos"))
                {
                    __debugbreak();
                }
            }
            return (char *)posa;
        }
        return result;
    }
}

void __cdecl VM_CancelNotify(scriptInstance_t inst, unsigned int notifyListOwnerId, unsigned int startLocalId)
{
    unsigned int Variable; // eax
    unsigned __int16 ThreadNotifyName; // ax
    unsigned int v5; // eax
    unsigned int stringValue; // [esp+0h] [ebp-Ch]
    unsigned int notifyListId; // [esp+4h] [ebp-8h]
    unsigned int notifyNameListId; // [esp+8h] [ebp-4h]

    Variable = FindVariable(inst, notifyListOwnerId, 0x17FFEu);
    notifyListId = FindObject(inst, Variable);
    ThreadNotifyName = Scr_GetThreadNotifyName(inst, startLocalId);
    stringValue = ThreadNotifyName;
    if ( !ThreadNotifyName
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3315, 0, "%s", "stringValue") )
    {
        __debugbreak();
    }
    v5 = FindVariable(inst, notifyListId, stringValue);
    notifyNameListId = FindObject(inst, v5);
    VM_CancelNotifyInternal(inst, notifyListOwnerId, startLocalId, notifyListId, notifyNameListId, stringValue);
}

void __cdecl VM_CancelNotifyInternal(
                scriptInstance_t inst,
                unsigned int notifyListOwnerId,
                unsigned int startLocalId,
                unsigned int notifyListId,
                unsigned int notifyNameListId,
                unsigned int stringValue)
{
    unsigned int Variable; // eax
    unsigned int v7; // eax

    if ( stringValue != (unsigned __int16)Scr_GetThreadNotifyName(inst, startLocalId)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    3289,
                    0,
                    "%s",
                    "stringValue == Scr_GetThreadNotifyName( inst, startLocalId )") )
    {
        __debugbreak();
    }
    Variable = FindVariable(inst, notifyListOwnerId, 0x17FFEu);
    if ( notifyListId != FindObject(inst, Variable)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    3290,
                    0,
                    "%s",
                    "notifyListId == FindObject( inst, FindVariable( inst, notifyListOwnerId, OBJECT_NOTIFY_LIST ) )") )
    {
        __debugbreak();
    }
    v7 = FindVariable(inst, notifyListId, stringValue);
    if ( notifyNameListId != FindObject(inst, v7)
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    3291,
                    0,
                    "%s",
                    "notifyNameListId == FindObject( inst, FindVariable( inst, notifyListId, stringValue ) )") )
    {
        __debugbreak();
    }
    Scr_RemoveThreadNotifyName(inst, startLocalId);
    RemoveObjectVariable(inst, notifyNameListId, startLocalId);
    if ( !GetArraySize(inst, notifyNameListId) )
    {
        RemoveVariable(inst, notifyListId, stringValue);
        if ( !GetArraySize(inst, notifyListId) )
            RemoveVariable(inst, notifyListOwnerId, 0x17FFEu);
    }
}

bool __cdecl Scr_IsEndonThread(scriptInstance_t inst, unsigned int localId)
{
    unsigned int stackId; // [esp+0h] [ebp-8h]
    unsigned int type; // [esp+4h] [ebp-4h]

    if ( GetObjectType(inst, localId) != 14 )
        return 0;
    if ( GetStartLocalId(inst, localId) != localId )
        return 0;
    stackId = Scr_GetWaittillThreadStackId(inst, localId, localId);
    type = GetValueType(inst, stackId);
    if ( type
        && type != 10
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    3913,
                    0,
                    "%s",
                    "(type == VAR_UNDEFINED) || (type == VAR_STACK)") )
    {
        __debugbreak();
    }
    return type == 0;
}

unsigned int __cdecl Scr_GetWaittillThreadStackId(
    scriptInstance_t inst,
    unsigned int localId,
    unsigned int startLocalId)
{
    unsigned __int16 ThreadNotifyName; // ax
    unsigned int ObjectVariable; // eax
    unsigned int v5; // eax
    VariableValueInternal_u *VariableValueAddress; // eax
    unsigned int Variable; // eax
    unsigned int v8; // eax
    unsigned int Object; // eax
    unsigned int stringValue; // [esp+0h] [ebp-18h]
    unsigned int selfNameId; // [esp+4h] [ebp-14h]
    unsigned int notifyListId; // [esp+8h] [ebp-10h]
    unsigned int selfId; // [esp+14h] [ebp-4h]

    if (!gScrVarPub[inst].developer
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            3883,
            0,
            "%s",
            "gScrVarPub[inst].developer"))
    {
        __debugbreak();
    }
    ThreadNotifyName = Scr_GetThreadNotifyName(inst, startLocalId);
    stringValue = ThreadNotifyName;
    if (!ThreadNotifyName)
        return FindVariable(inst, startLocalId, 0x17FFFu);
    selfId = Scr_GetSelf(inst, startLocalId);
    ObjectVariable = FindObjectVariable(inst, gScrVarPub[inst].pauseArrayId, selfId);
    selfNameId = FindObject(inst, ObjectVariable);
    v5 = FindObjectVariable(inst, selfNameId, startLocalId);
    VariableValueAddress = GetVariableValueAddress(inst, v5);
    Variable = FindVariable(inst, VariableValueAddress->next, 0x17FFEu);
    notifyListId = FindObject(inst, Variable);
    v8 = FindVariable(inst, notifyListId, stringValue);
    Object = FindObject(inst, v8);
    return FindObjectVariable(inst, Object, startLocalId);
}

char *__cdecl Scr_GetThreadPos(scriptInstance_t inst, unsigned int localId)
{
    unsigned int ValueType; // eax
    unsigned int v4; // eax
    VariableValueInternal_u *VariableValueAddress; // eax
    unsigned int ObjectType; // [esp+0h] [ebp-10h]
    unsigned int stackId; // [esp+8h] [ebp-8h]
    unsigned int startLocalId; // [esp+Ch] [ebp-4h]

    if (!gScrVarPub[inst].developer
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            3951,
            0,
            "%s",
            "gScrVarPub[inst].developer"))
    {
        __debugbreak();
    }
    startLocalId = GetStartLocalId(inst, localId);
    ObjectType = GetObjectType(inst, startLocalId);
    switch (ObjectType)
    {
    case 0xDu:
        return Scr_GetRunningThreadPos(inst, localId);
    case 0xEu:
        stackId = Scr_GetWaittillThreadStackId(inst, localId, startLocalId);
        if (!stackId
            && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3961, 0, "%s", "stackId"))
        {
            __debugbreak();
        }
        if (GetValueType(inst, stackId) != 10)
        {
            ValueType = GetValueType(inst, stackId);
            if (!Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                3962,
                0,
                "%s\n\t(GetValueType( inst, stackId )) = %i",
                "(GetValueType( inst, stackId ) == VAR_STACK)",
                ValueType))
                __debugbreak();
        }
        goto LABEL_26;
    case 0xFu:
        stackId = Scr_GetWaitThreadStackId(inst, localId, startLocalId);
        if (!stackId
            && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3967, 0, "%s", "stackId"))
        {
            __debugbreak();
        }
        if (GetValueType(inst, stackId) != 10)
        {
            v4 = GetValueType(inst, stackId);
            if (!Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                3968,
                0,
                "%s\n\t(GetValueType( inst, stackId )) = %i",
                "(GetValueType( inst, stackId ) == VAR_STACK)",
                v4))
                __debugbreak();
        }
    LABEL_26:
        VariableValueAddress = GetVariableValueAddress(inst, stackId);
        return (char *)Scr_GetStackThreadPos(inst, localId, VariableValueAddress->u.stackValue, 0);
    }
    if (!Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3972, 0, "unreachable"))
        __debugbreak();
    return 0;
}

const char *__cdecl Scr_GetStackThreadPos(
    scriptInstance_t inst,
    unsigned int endLocalId,
    VariableStackBuffer *stackValue,
    bool killThread)
{
    const char *pos; // [esp+0h] [ebp-20h]
    unsigned int startLocalId; // [esp+4h] [ebp-1Ch]
    unsigned int localId; // [esp+8h] [ebp-18h]
    const char *buf; // [esp+Ch] [ebp-14h]
    const char *bufa; // [esp+Ch] [ebp-14h]
    int size; // [esp+10h] [ebp-10h]
    unsigned int parentLocalId; // [esp+14h] [ebp-Ch]
    VariableUnion u; // [esp+18h] [ebp-8h]

    if (!gScrVarPub[inst].developer
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            3574,
            0,
            "%s",
            "gScrVarPub[inst].developer"))
    {
        __debugbreak();
    }
    startLocalId = GetStartLocalId(inst, endLocalId);
    if (!startLocalId
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3577, 0, "%s", "startLocalId"))
    {
        __debugbreak();
    }
    size = stackValue->size;
    localId = stackValue->localId;
    buf = &stackValue->buf[5 * size];
    pos = stackValue->pos;
    while (size)
    {
        bufa = buf - 4;
        u.intValue = *(int *)bufa;
        buf = bufa - 1;
        --size;
        if (*buf == 7)
        {
            parentLocalId = GetParentLocalId(inst, localId);
            if (localId == endLocalId)
            {
                if (startLocalId == localId
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        3609,
                        0,
                        "%s",
                        "startLocalId != localId"))
                {
                    __debugbreak();
                }
                break;
            }
            if (killThread)
                Scr_DebugKillThread(inst, localId, pos);
            localId = parentLocalId;
            if (!u.intValue
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    3604,
                    0,
                    "%s",
                    "u.codePosValue"))
            {
                __debugbreak();
            }
            pos = (const char *)u.intValue;
        }
    }
    if (killThread)
        Scr_DebugKillThread(inst, localId, pos);
    return pos;
}

char *__cdecl Scr_GetRunningThreadPos(scriptInstance_t inst, unsigned int localId)
{
    char *pos; // [esp+0h] [ebp-Ch]
    int function_count; // [esp+4h] [ebp-8h]

    if (!gScrVarPub[inst].developer
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            3742,
            0,
            "%s",
            "gScrVarPub[inst].developer"))
    {
        __debugbreak();
    }
    if (!gScrVmPub[inst].function_count
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            3744,
            0,
            "%s",
            "gScrVmPub[inst].function_count"))
    {
        __debugbreak();
    }
    for (function_count = gScrVmPub[inst].function_count; function_count; --function_count)
    {
        if (gScrVmPub[inst].function_frame_start[function_count].fs.localId == localId)
        {
            pos = (char *)gScrVmPub[inst].function_frame_start[function_count].fs.pos;
            return &g_EndPos != pos ? pos : 0;
        }
    }
    if (!Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3756, 0, "unreachable"))
        __debugbreak();
    return 0;
}

unsigned int __cdecl Scr_GetWaitThreadStackId(scriptInstance_t inst, unsigned int localId, unsigned int startLocalId)
{
    unsigned int Variable; // eax
    unsigned int time; // [esp+0h] [ebp-8h]
    unsigned int id; // [esp+4h] [ebp-4h]

    if (!gScrVarPub[inst].developer
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            3796,
            0,
            "%s",
            "gScrVarPub[inst].developer"))
    {
        __debugbreak();
    }
    time = Scr_GetThreadWaitTime(inst, startLocalId);
    Variable = FindVariable(inst, gScrVarPub[inst].timeArrayId, time);
    id = FindObject(inst, Variable);
    return FindObjectVariable(inst, id, startLocalId);
}

void __cdecl Scr_NotifyNum_Internal(
    scriptInstance_t inst,
    int clientNum,
    unsigned int entnum,
    unsigned int classnum,
    unsigned int stringValue,
    unsigned int paramcount)
{
    VariableValue *top; // [esp+Ch] [ebp-14h]
    const char *varUsagePos; // [esp+10h] [ebp-10h]
    VariableValue *startTop; // [esp+14h] [ebp-Ch]
    int type; // [esp+18h] [ebp-8h]
    unsigned int id; // [esp+1Ch] [ebp-4h]
    unsigned int paramcounta; // [esp+3Ch] [ebp+1Ch]

    SL_CheckExists(inst, stringValue);
    varUsagePos = gScrVarPub[inst].varUsagePos;
    if (!varUsagePos)
        gScrVarPub[inst].varUsagePos = "<script notify variable>";
    if (!gScrVarPub[inst].timeArrayId
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4249,
            0,
            "%s",
            "gScrVarPub[inst].timeArrayId"))
    {
        __debugbreak();
    }
    if (paramcount > gScrVmPub[inst].inparamcount
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4250,
            0,
            "%s",
            "paramcount <= gScrVmPub[inst].inparamcount"))
    {
        __debugbreak();
    }
    Scr_ClearOutParams(inst);
    startTop = &gScrVmPub[inst].top[-(int)paramcount];
    paramcounta = gScrVmPub[inst].inparamcount - paramcount;
    id = FindEntityId(inst, entnum, classnum, clientNum);
    if (id)
    {
        if (gScrVmDebugPub[inst].checkBreakon)
        {
            top = gScrVmPub[inst].top;
            gScrVmPub[inst].inparamcount = 0;
            Scr_CheckBreakonNotify(inst, id, stringValue, gScrVmPub[inst].top, 0, 0);
            gScrVmPub[inst].top = top;
            if (gScrVmPub[inst].outparamcount)
            {
                if (!Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    4269,
                    0,
                    "%s",
                    "!gScrVmPub[inst].outparamcount"))
                    __debugbreak();
            }
        }
        type = startTop->type;
        startTop->type = 8;
        gScrVmPub[inst].inparamcount = 0;
        VM_Notify(inst, id, stringValue, gScrVmPub[inst].top);
        startTop->type = type;
    }
    while (gScrVmPub[inst].top != startTop)
    {
        RemoveRefToValue(inst, gScrVmPub[inst].top->type, gScrVmPub[inst].top->u);
        --gScrVmPub[inst].top;
    }
    gScrVmPub[inst].inparamcount = paramcounta;
    if (gScrVmPub[inst].outparamcount
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4287,
            0,
            "%s",
            "!gScrVmPub[inst].outparamcount"))
    {
        __debugbreak();
    }
    gScrVarPub[inst].varUsagePos = varUsagePos;
    SL_CheckExists(inst, stringValue);
}

void __cdecl VM_Notify(
    scriptInstance_t inst,
    unsigned int notifyListOwnerId,
    unsigned int stringValue,
    VariableValue *top)
{
    unsigned int ObjectVariable; // eax
    int type; // ecx
    unsigned int Variable; // eax
    unsigned int Array; // eax
    unsigned int v8; // [esp-4h] [ebp-A0h]
    VariableUnion stackValue; // [esp+3Ch] [ebp-60h]
    unsigned int notifyListIndex; // [esp+40h] [ebp-5Ch]
    VariableValue tempValue2; // [esp+44h] [ebp-58h] BYREF
    VariableValue tempValue3; // [esp+4Ch] [ebp-50h] BYREF
    unsigned int stackId; // [esp+54h] [ebp-48h]
    unsigned int startLocalId; // [esp+58h] [ebp-44h]
    VariableStackBuffer *newStackValue; // [esp+5Ch] [ebp-40h]
    VariableValue tempValue5; // [esp+60h] [ebp-3Ch] BYREF
    VariableValue *currentValue; // [esp+68h] [ebp-34h]
    char *buf; // [esp+6Ch] [ebp-30h]
    unsigned int selfNameId; // [esp+70h] [ebp-2Ch]
    int size; // [esp+74h] [ebp-28h]
    int len; // [esp+78h] [ebp-24h]
    unsigned int notifyListId; // [esp+7Ch] [ebp-20h]
    unsigned int notifyNameListId; // [esp+80h] [ebp-1Ch]
    int newSize; // [esp+84h] [ebp-18h]
    int bufLen; // [esp+88h] [ebp-14h]
    bool bNoStack; // [esp+8Fh] [ebp-Dh]
    VariableUnion *tempValue; // [esp+90h] [ebp-Ch]
    unsigned int selfId; // [esp+94h] [ebp-8h]
    unsigned int notifyListEntry; // [esp+98h] [ebp-4h]

    if (inst == SCRIPTINSTANCE_SERVER && (stringValue == scr_const.death || stringValue == scr_const.disconnect))
        VM_Notify(SCRIPTINSTANCE_SERVER, notifyListOwnerId, scr_const.death_or_disconnect, top);
    notifyListId = FindVariable(inst, notifyListOwnerId, 0x17FFEu);
    if (notifyListId)
    {
        notifyListId = FindObject(inst, notifyListId);
        if (!notifyListId
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                4025,
                0,
                "%s",
                "notifyListId"))
        {
            __debugbreak();
        }
        notifyNameListId = FindVariable(inst, notifyListId, stringValue);
        if (notifyNameListId)
        {
            notifyNameListId = FindObject(inst, notifyNameListId);
            if (!notifyNameListId
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    4032,
                    0,
                    "%s",
                    "notifyNameListId"))
            {
                __debugbreak();
            }
            AddRefToObject(inst, notifyNameListId);
            if (gScrVarPub[inst].evaluate
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    4036,
                    0,
                    "%s",
                    "!gScrVarPub[inst].evaluate"))
            {
                __debugbreak();
            }
            gScrVarPub[inst].evaluate = 1;
            notifyListEntry = notifyNameListId;
            while (1)
            {
                notifyListIndex = FindLastSibling(inst, notifyListEntry);
            next:
                if (!notifyListIndex)
                    break;
                notifyListEntry = gScrVarGlob[inst].variableList[notifyListIndex + 0x8000].hash.id;
                if (!notifyListEntry
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        4050,
                        0,
                        "%s",
                        "notifyListEntry"))
                {
                    __debugbreak();
                }
                startLocalId = GetVariableKeyObject(inst, notifyListEntry);
                selfId = Scr_GetSelf(inst, startLocalId);
                ObjectVariable = FindObjectVariable(inst, gScrVarPub[inst].pauseArrayId, selfId);
                selfNameId = FindObject(inst, ObjectVariable);
                if (GetValueType(inst, notifyListEntry))
                {
                    if (GetValueType(inst, notifyListEntry) != 10
                        && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                            4075,
                            0,
                            "%s",
                            "GetValueType( inst, notifyListEntry ) == VAR_STACK"))
                    {
                        __debugbreak();
                    }
                    tempValue = (VariableUnion *)GetVariableValueAddress(inst, notifyListEntry);
                    stackValue.intValue = tempValue->intValue;
                    if (*(_BYTE *)(*(_DWORD *)tempValue->intValue - 1) == 119)
                    {
                        size = **(char **)stackValue.intValue;
                        if (size < 0
                            && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                4083,
                                0,
                                "%s",
                                "size >= 0"))
                        {
                            __debugbreak();
                        }
                        if (size > *(unsigned __int16 *)(stackValue.intValue + 4)
                            && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                4084,
                                0,
                                "size <= stackValue->size\n\t%i, %i",
                                size,
                                *(unsigned __int16 *)(stackValue.intValue + 4)))
                        {
                            __debugbreak();
                        }
                        buf = (char *)(stackValue.intValue + 5 * (*(unsigned __int16 *)(stackValue.intValue + 4) - size) + 13);
                        for (currentValue = top; size; --currentValue)
                        {
                            if (currentValue->type == 7
                                && !Assert_MyHandler(
                                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                    4091,
                                    0,
                                    "%s",
                                    "currentValue->type != VAR_CODEPOS"))
                            {
                                __debugbreak();
                            }
                            if (currentValue->type == 8)
                            {
                                notifyListIndex = gScrVarGlob[inst].variableList[notifyListIndex + 0x8000].hash.u.prev;
                                goto next;
                            }
                            --size;
                            tempValue3.type = (unsigned __int8)*buf;
                            if (tempValue3.type == 7
                                && !Assert_MyHandler(
                                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                    4100,
                                    0,
                                    "%s",
                                    "tempValue3.type != VAR_CODEPOS"))
                            {
                                __debugbreak();
                            }
                            if (tempValue3.type == 8)
                                break;
                            tempValue3.u.intValue = *(_DWORD *)++buf;
                            buf += 4;
                            AddRefToValue(inst, tempValue3.type, tempValue3.u);
                            type = currentValue->type;
                            tempValue2.u.intValue = currentValue->u.intValue;
                            tempValue2.type = type;
                            AddRefToValue(inst, type, tempValue2.u);
                            Scr_EvalEquality(inst, &tempValue3, &tempValue2);
                            if (gScrVarPub[inst].error_message)
                            {
                                RuntimeError(
                                    inst,
                                    *(char **)stackValue.intValue,
                                    **(char **)stackValue.intValue - size + 3,
                                    gScrVarPub[inst].error_message,
                                    gScrVmGlob[inst].dialog_error_message);
                                Scr_ClearErrorMessage(inst);
                                notifyListIndex = gScrVarGlob[inst].variableList[notifyListIndex + 0x8000].hash.u.prev;
                                goto next;
                            }
                            if (tempValue3.type != 6
                                && !Assert_MyHandler(
                                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                    4116,
                                    0,
                                    "%s",
                                    "tempValue3.type == VAR_INTEGER"))
                            {
                                __debugbreak();
                            }
                            if (!tempValue3.u.intValue)
                            {
                                notifyListIndex = gScrVarGlob[inst].variableList[notifyListIndex + 0x8000].hash.u.prev;
                                goto next;
                            }
                        }
                        ++ * (_DWORD *)stackValue.intValue;
                        bNoStack = 1;
                    }
                    else
                    {
                        if (top->type == 7
                            && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                4138,
                                0,
                                "%s",
                                "top->type != VAR_CODEPOS"))
                        {
                            __debugbreak();
                        }
                        bNoStack = top->type == 8;
                    }
                    tempValue5.type = 10;
                    tempValue5.u = stackValue;
                    v8 = startLocalId;
                    Variable = GetVariable(inst, gScrVarPub[inst].timeArrayId, gScrVarPub[inst].time);
                    Array = GetArray(inst, Variable);
                    stackId = GetNewObjectVariable(inst, Array, v8);
                    SetNewVariableValue(inst, stackId, &tempValue5);
                    tempValue = (VariableUnion *)GetVariableValueAddress(inst, stackId);
                    VM_CancelNotifyInternal(inst, notifyListOwnerId, startLocalId, notifyListId, notifyNameListId, stringValue);
                    RemoveObjectVariable(inst, selfNameId, startLocalId);
                    if (!GetArraySize(inst, selfNameId))
                        RemoveObjectVariable(inst, gScrVarPub[inst].pauseArrayId, selfId);
                    Scr_SetThreadWaitTime(inst, startLocalId, gScrVarPub[inst].time);
                    if (bNoStack)
                    {
                        notifyListEntry = notifyNameListId;
                    }
                    else
                    {
                        if (top->type == 8
                            && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                4164,
                                0,
                                "%s\n\t(top - gScrVmPub[inst].stack) = %i",
                                "(top->type != VAR_PRECODEPOS)",
                                top - gScrVmPub[inst].stack))
                        {
                            __debugbreak();
                        }
                        if (top->type == 7
                            && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                4165,
                                0,
                                "%s\n\t(top - gScrVmPub[inst].stack) = %i",
                                "(top->type != VAR_CODEPOS)",
                                top - gScrVmPub[inst].stack))
                        {
                            __debugbreak();
                        }
                        size = *(unsigned __int16 *)(stackValue.intValue + 4);
                        newSize = size;
                        currentValue = top;
                        do
                        {
                            ++newSize;
                            --currentValue;
                            if (currentValue->type == 7
                                && !Assert_MyHandler(
                                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                    4175,
                                    0,
                                    "%s\n\t(currentValue - gScrVmPub[inst].stack) = %i",
                                    "(currentValue->type != VAR_CODEPOS)",
                                    currentValue - gScrVmPub[inst].stack))
                            {
                                __debugbreak();
                            }
                        } while (currentValue->type != 8);
                        if ((unsigned int)newSize >= 0x10000
                            && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                4178,
                                0,
                                "%s",
                                "newSize >= 0 && newSize < (1 << 16)"))
                        {
                            __debugbreak();
                        }
                        len = 5 * size;
                        bufLen = 5 * newSize + 13;
                        if (!MT_Realloc(inst, *(unsigned __int16 *)(stackValue.intValue + 6), bufLen))
                        {
                            newStackValue = (VariableStackBuffer *)MT_Alloc(bufLen, 1, inst);
                            newStackValue->bufLen = bufLen;
                            newStackValue->pos = *(const char **)stackValue.intValue;
                            newStackValue->localId = *(_DWORD *)(stackValue.intValue + 8);
                            memcpy((unsigned __int8 *)newStackValue->buf, (unsigned __int8 *)(stackValue.intValue + 13), len);
                            MT_Free((uint8*)stackValue.intValue, *(unsigned __int16 *)(stackValue.intValue + 6), inst);
                            stackValue.intValue = (int)newStackValue;
                            tempValue->intValue = (int)newStackValue;
                        }
                        *(_WORD *)(stackValue.intValue + 4) = newSize;
                        buf = (char *)(stackValue.intValue + len + 13);
                        newSize -= size;
                        if (!newSize
                            && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                4203,
                                0,
                                "%s",
                                "newSize"))
                        {
                            __debugbreak();
                        }
                        do
                        {
                            ++currentValue;
                            AddRefToValue(inst, currentValue->type, currentValue->u);
                            if (currentValue->type >= 0x16u
                                && !Assert_MyHandler(
                                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                    4209,
                                    0,
                                    "%s",
                                    "(unsigned)currentValue->type < VAR_COUNT"))
                            {
                                __debugbreak();
                            }
                            *buf++ = currentValue->type;
                            *(_DWORD *)buf = currentValue->u.intValue;
                            buf += 4;
                            --newSize;
                        } while (newSize);
                        if (&buf[-stackValue.intValue] != (char *)bufLen
                            && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                4217,
                                0,
                                "%s",
                                "buf - (const char *)stackValue == bufLen"))
                        {
                            __debugbreak();
                        }
                        notifyListEntry = notifyNameListId;
                    }
                }
                else
                {
                    VM_CancelNotifyInternal(inst, notifyListOwnerId, startLocalId, notifyListId, notifyNameListId, stringValue);
                    Scr_KillEndonThread(inst, startLocalId);
                    RemoveObjectVariable(inst, selfNameId, startLocalId);
                    if (!GetArraySize(inst, selfNameId))
                        RemoveObjectVariable(inst, gScrVarPub[inst].pauseArrayId, selfId);
                    Scr_TerminateThread(inst, selfId);
                    notifyListEntry = notifyNameListId;
                }
            }
            RemoveRefToObject(inst, notifyNameListId);
            if (!gScrVarPub[inst].evaluate
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    4223,
                    0,
                    "%s",
                    "gScrVarPub[inst].evaluate"))
            {
                __debugbreak();
            }
            gScrVarPub[inst].evaluate = 0;
        }
    }
}

void __cdecl Scr_TerminateThread(scriptInstance_t inst, unsigned int localId)
{
    unsigned int ObjectType; // [esp+0h] [ebp-8h]
    unsigned int startLocalId; // [esp+4h] [ebp-4h]

    startLocalId = GetStartLocalId(inst, localId);
    ObjectType = GetObjectType(inst, startLocalId);
    switch ( ObjectType )
    {
        case 0xDu:
            Scr_TerminateRunningThread(inst, localId);
            break;
        case 0xEu:
            Scr_TerminateWaittillThread(inst, localId, startLocalId);
            break;
        case 0xFu:
            Scr_TerminateWaitThread(inst, localId, startLocalId);
            break;
        default:
            if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3938, 0, "unreachable") )
                __debugbreak();
            break;
    }
}

void __cdecl Scr_TerminateRunningThread(scriptInstance_t inst, unsigned int localId)
{
    int function_count; // [esp+0h] [ebp-Ch]
    int topThread; // [esp+4h] [ebp-8h]
    unsigned int threadId; // [esp+8h] [ebp-4h]

    if ( !gScrVmPub[inst].function_count
        && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    3704,
                    0,
                    "%s",
                    "gScrVmPub[inst].function_count") )
    {
        __debugbreak();
    }
    function_count = gScrVmPub[inst].function_count;
    topThread = function_count;
    while ( 1 )
    {
        if ( !function_count
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        3709,
                        0,
                        "%s",
                        "function_count") )
        {
            __debugbreak();
        }
        threadId = gScrVmPub[inst].function_frame_start[function_count].fs.localId;
        if ( threadId == localId )
            break;
        --function_count;
        if ( !GetSafeParentLocalId(inst, threadId) )
            topThread = function_count;
    }
    while ( topThread >= function_count )
    {
        if ( gScrVarPub[inst].developer )
            Scr_DebugTerminateThread(inst, topThread);
        else
            gScrVmPub[inst].function_frame_start[topThread].fs.pos = &g_EndPos;
        --topThread;
    }
}

void __cdecl Scr_TerminateWaitThread(scriptInstance_t inst, unsigned int localId, unsigned int startLocalId)
{
    unsigned int Variable; // eax
    VariableStackBuffer *stackValue; // [esp+0h] [ebp-10h]
    unsigned int stackId; // [esp+4h] [ebp-Ch]
    unsigned int time; // [esp+8h] [ebp-8h]
    unsigned int id; // [esp+Ch] [ebp-4h]

    time = Scr_GetThreadWaitTime(inst, startLocalId);
    Scr_ClearWaitTime(inst, startLocalId);
    Variable = FindVariable(inst, gScrVarPub[inst].timeArrayId, time);
    id = FindObject(inst, Variable);
    stackId = FindObjectVariable(inst, id, startLocalId);
    if (!stackId
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3773, 0, "%s", "stackId"))
    {
        __debugbreak();
    }
    if (GetValueType(inst, stackId) != 10
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            3774,
            0,
            "%s",
            "GetValueType( inst, stackId ) == VAR_STACK"))
    {
        __debugbreak();
    }
    stackValue = (VariableStackBuffer *)GetVariableValueAddress(inst, stackId)->next;
    if (gScrVarPub[inst].developer)
        Scr_GetStackThreadPos(inst, localId, stackValue, 1);
    RemoveObjectVariable(inst, id, startLocalId);
    if (!GetArraySize(inst, id) && time != gScrVarPub[inst].time)
        RemoveVariable(inst, gScrVarPub[inst].timeArrayId, time);
    VM_TerminateStack(inst, localId, startLocalId, stackValue);
}

void __cdecl VM_TerminateStack(
    scriptInstance_t inst,
    unsigned int endLocalId,
    unsigned int startLocalId,
    VariableStackBuffer *stackValue)
{
    unsigned int Variable; // eax
    unsigned int Array; // eax
    unsigned int stackId; // [esp+8h] [ebp-24h]
    unsigned int localId; // [esp+Ch] [ebp-20h]
    char *buf; // [esp+10h] [ebp-1Ch]
    char *bufa; // [esp+10h] [ebp-1Ch]
    int size; // [esp+14h] [ebp-18h]
    int sizea; // [esp+14h] [ebp-18h]
    unsigned int parentLocalId; // [esp+18h] [ebp-14h]
    const char *u; // [esp+1Ch] [ebp-10h]
    VariableValue tempValue; // [esp+24h] [ebp-8h] BYREF

    if (!startLocalId
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3497, 0, "%s", "startLocalId"))
    {
        __debugbreak();
    }
    size = stackValue->size;
    localId = stackValue->localId;
    buf = &stackValue->buf[5 * size];
    while (size)
    {
        bufa = buf - 4;
        u = *(const char **)bufa;
        buf = bufa - 1;
        --size;
        if (*buf == 7)
        {
            parentLocalId = GetParentLocalId(inst, localId);
            Scr_KillThread(inst, localId);
            RemoveRefToObject(inst, localId);
            if (localId == endLocalId)
            {
                if (startLocalId == localId
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        3528,
                        0,
                        "%s",
                        "startLocalId != localId"))
                {
                    __debugbreak();
                }
                sizea = size + 1;
                *buf = 0;
                if (stackValue->size < sizea
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        3533,
                        0,
                        "%s",
                        "stackValue->size >= size"))
                {
                    __debugbreak();
                }
                Scr_SetThreadWaitTime(inst, startLocalId, gScrVarPub[inst].time);
                if (!u
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        3536,
                        0,
                        "%s",
                        "u.codePosValue"))
                {
                    __debugbreak();
                }
                stackValue->pos = u;
                stackValue->localId = parentLocalId;
                stackValue->size = sizea;
                tempValue.type = 10;
                tempValue.u.intValue = (int)stackValue;
                Variable = GetVariable(inst, gScrVarPub[inst].timeArrayId, gScrVarPub[inst].time);
                Array = GetArray(inst, Variable);
                stackId = GetNewObjectVariable(inst, Array, startLocalId);
                SetNewVariableValue(inst, stackId, &tempValue);
                return;
            }
            localId = parentLocalId;
        }
        else
        {
            RemoveRefToValue(inst, *(unsigned __int8 *)buf, (VariableUnion)u);
        }
    }
    if (localId != endLocalId
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            3549,
            0,
            "%s",
            "localId == endLocalId"))
    {
        __debugbreak();
    }
    if (startLocalId != localId
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            3550,
            0,
            "%s",
            "startLocalId == localId"))
    {
        __debugbreak();
    }
    Scr_KillThread(inst, localId);
    RemoveRefToObject(inst, localId);
    --gScrVarPub[inst].numScriptThreads;
    MT_Free((uint8*)stackValue, stackValue->bufLen, inst);
}

void __cdecl Scr_TerminateWaittillThread(scriptInstance_t inst, unsigned int localId, unsigned int startLocalId)
{
    unsigned __int16 ThreadNotifyName; // ax
    unsigned int ObjectVariable; // eax
    unsigned int v5; // eax
    unsigned int Variable; // eax
    unsigned int v7; // eax
    VariableStackBuffer *stackValue; // [esp+0h] [ebp-20h]
    unsigned int stringValue; // [esp+4h] [ebp-1Ch]
    unsigned int stackId; // [esp+8h] [ebp-18h]
    unsigned int stackIda; // [esp+8h] [ebp-18h]
    unsigned int selfNameId; // [esp+Ch] [ebp-14h]
    unsigned int notifyListId; // [esp+10h] [ebp-10h]
    unsigned int notifyNameListId; // [esp+14h] [ebp-Ch]
    unsigned int notifyListOwnerId; // [esp+18h] [ebp-8h]
    unsigned int selfId; // [esp+1Ch] [ebp-4h]

    ThreadNotifyName = Scr_GetThreadNotifyName(inst, startLocalId);
    stringValue = ThreadNotifyName;
    if (ThreadNotifyName)
    {
        selfId = Scr_GetSelf(inst, startLocalId);
        ObjectVariable = FindObjectVariable(inst, gScrVarPub[inst].pauseArrayId, selfId);
        selfNameId = FindObject(inst, ObjectVariable);
        v5 = FindObjectVariable(inst, selfNameId, startLocalId);
        notifyListOwnerId = GetVariableValueAddress(inst, v5)->next;
        Variable = FindVariable(inst, notifyListOwnerId, 0x17FFEu);
        notifyListId = FindObject(inst, Variable);
        v7 = FindVariable(inst, notifyListId, stringValue);
        notifyNameListId = FindObject(inst, v7);
        stackId = FindObjectVariable(inst, notifyNameListId, startLocalId);
        if (!stackId
            && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3841, 0, "%s", "stackId"))
        {
            __debugbreak();
        }
        if (GetValueType(inst, stackId) != 10
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                3842,
                0,
                "%s",
                "GetValueType( inst, stackId ) == VAR_STACK"))
        {
            __debugbreak();
        }
        stackValue = (VariableStackBuffer *)GetVariableValueAddress(inst, stackId)->next;
        if (gScrVarPub[inst].developer)
            Scr_GetStackThreadPos(inst, localId, stackValue, 1);
        VM_CancelNotifyInternal(inst, notifyListOwnerId, startLocalId, notifyListId, notifyNameListId, stringValue);
        RemoveObjectVariable(inst, selfNameId, startLocalId);
        if (!GetArraySize(inst, selfNameId))
            RemoveObjectVariable(inst, gScrVarPub[inst].pauseArrayId, selfId);
    }
    else
    {
        stackIda = FindVariable(inst, startLocalId, 0x17FFFu);
        if (!stackIda
            && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3858, 0, "%s", "stackId"))
        {
            __debugbreak();
        }
        if (GetValueType(inst, stackIda) != 10
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                3859,
                0,
                "%s",
                "GetValueType( inst, stackId ) == VAR_STACK"))
        {
            __debugbreak();
        }
        stackValue = (VariableStackBuffer *)GetVariableValueAddress(inst, stackIda)->next;
        if (gScrVarPub[inst].developer)
            Scr_GetStackThreadPos(inst, localId, stackValue, 1);
        RemoveVariable(inst, startLocalId, 0x17FFFu);
    }
    VM_TerminateStack(inst, localId, startLocalId, stackValue);
}

void __cdecl Scr_NotifyNum(
                unsigned int entnum,
                unsigned int classnum,
                unsigned int stringValue,
                unsigned int paramcount)
{
    Scr_NotifyNum_Internal(SCRIPTINSTANCE_SERVER, 0, entnum, classnum, stringValue, paramcount);
}

void __cdecl CScr_NotifyNum(
                int clientNum,
                unsigned int entnum,
                unsigned int classnum,
                unsigned int stringValue,
                unsigned int paramcount)
{
    Scr_NotifyNum_Internal(SCRIPTINSTANCE_CLIENT, clientNum, entnum, classnum, stringValue, paramcount);
}

void __cdecl Scr_CancelNotifyList(scriptInstance_t inst, unsigned int notifyListOwnerId)
{
    VariableValueInternal_u *VariableValueAddress; // eax
    VariableStackBuffer *stackValuea; // [esp+Ch] [ebp-1Ch]
    VariableStackBuffer *stackValue; // [esp+Ch] [ebp-1Ch]
    unsigned int stackId; // [esp+10h] [ebp-18h]
    unsigned int stackIda; // [esp+10h] [ebp-18h]
    unsigned int startLocalId; // [esp+14h] [ebp-14h]
    unsigned int selfStartLocalId; // [esp+18h] [ebp-10h]
    unsigned int notifyListId; // [esp+1Ch] [ebp-Ch]
    unsigned int notifyListIda; // [esp+1Ch] [ebp-Ch]
    unsigned int notifyNameListId; // [esp+20h] [ebp-8h]
    unsigned int notifyNameListIda; // [esp+20h] [ebp-8h]
    unsigned int selfLocalId; // [esp+24h] [ebp-4h]

    while ( 1 )
    {
        notifyListId = FindVariable(inst, notifyListOwnerId, 0x17FFEu);
        if ( !notifyListId )
            break;
        notifyListIda = FindObject(inst, notifyListId);
        if ( !notifyListIda
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        4328,
                        0,
                        "%s",
                        "notifyListId") )
        {
            __debugbreak();
        }
        notifyNameListId = FindFirstSibling(inst, notifyListIda);
        if ( !notifyNameListId )
            break;
        notifyNameListIda = FindObject(inst, notifyNameListId);
        if ( !notifyNameListIda
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        4335,
                        0,
                        "%s",
                        "notifyNameListId") )
        {
            __debugbreak();
        }
        stackId = FindFirstSibling(inst, notifyNameListIda);
        if ( !stackId )
            break;
        startLocalId = GetVariableKeyObject(inst, stackId);
        if ( !startLocalId
            && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        4342,
                        0,
                        "%s",
                        "startLocalId") )
        {
            __debugbreak();
        }
        if ( GetValueType(inst, stackId) == 10 )
        {
            stackValuea = (VariableStackBuffer *)GetVariableValueAddress(inst, stackId)->next;
            Scr_CancelWaittill(inst, startLocalId);
            VM_TrimStack(inst, startLocalId, stackValuea, 0);
        }
        else
        {
            AddRefToObject(inst, startLocalId);
            Scr_CancelWaittill(inst, startLocalId);
            selfLocalId = Scr_GetSelf(inst, startLocalId);
            selfStartLocalId = GetStartLocalId(inst, selfLocalId);
            stackIda = FindVariable(inst, selfStartLocalId, 0x17FFFu);
            if ( stackIda )
            {
                if ( (unsigned __int16)Scr_GetThreadNotifyName(inst, selfStartLocalId)
                    && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                4361,
                                0,
                                "%s",
                                "!Scr_GetThreadNotifyName( inst, selfStartLocalId )") )
                {
                    __debugbreak();
                }
                if ( GetValueType(inst, stackIda) != 10
                    && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                4362,
                                0,
                                "%s",
                                "GetValueType( inst, stackId ) == VAR_STACK") )
                {
                    __debugbreak();
                }
                VariableValueAddress = GetVariableValueAddress(inst, stackIda);
                stackValue = (VariableStackBuffer *)VariableValueAddress->next;
                if ( *(unsigned int *)VariableValueAddress->next )
                {
                    if ( !Assert_MyHandler(
                                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                    4364,
                                    0,
                                    "%s",
                                    "!stackValue->pos") )
                        __debugbreak();
                }
                VM_TrimStack(inst, selfStartLocalId, stackValue, 1);
            }
            Scr_KillEndonThread(inst, startLocalId);
            RemoveRefToEmptyObject(inst, startLocalId);
        }
    }
}

void __cdecl VM_TrimStack(
    scriptInstance_t inst,
    unsigned int startLocalId,
    VariableStackBuffer *stackValue,
    bool fromEndon)
{
    unsigned int NewVariable; // eax
    unsigned int localId; // [esp+8h] [ebp-20h]
    const char *buf; // [esp+Ch] [ebp-1Ch]
    const char *bufa; // [esp+Ch] [ebp-1Ch]
    int size; // [esp+10h] [ebp-18h]
    unsigned int parentLocalId; // [esp+14h] [ebp-14h]
    VariableUnion u; // [esp+18h] [ebp-10h]
    VariableValue tempValue; // [esp+20h] [ebp-8h] BYREF

    if (!startLocalId
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3630, 0, "%s", "startLocalId"))
    {
        __debugbreak();
    }
    size = stackValue->size;
    localId = stackValue->localId;
    buf = &stackValue->buf[5 * size];
    while (size)
    {
        bufa = buf - 4;
        u.intValue = *(int *)bufa;
        buf = bufa - 1;
        --size;
        if (*buf == 7)
        {
            if (FindObjectVariable(inst, gScrVarPub[inst].pauseArrayId, localId))
            {
                if (startLocalId == localId
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        3654,
                        0,
                        "%s",
                        "startLocalId != localId"))
                {
                    __debugbreak();
                }
                stackValue->localId = localId;
                stackValue->size = size + 1;
                Scr_StopThread(inst, localId);
                if (!fromEndon)
                {
                    Scr_SetThreadNotifyName(inst, startLocalId, 0);
                    stackValue->pos = 0;
                    tempValue.type = 10;
                    tempValue.u.intValue = (int)stackValue;
                    NewVariable = GetNewVariable(inst, startLocalId, 0x17FFFu);
                    SetNewVariableValue(inst, NewVariable, &tempValue);
                }
                return;
            }
            parentLocalId = GetParentLocalId(inst, localId);
            Scr_KillThread(inst, localId);
            RemoveRefToObject(inst, localId);
            localId = parentLocalId;
        }
        else
        {
            RemoveRefToValue(inst, *(unsigned __int8 *)buf, u);
        }
    }
    if (startLocalId != localId
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            3683,
            0,
            "%s",
            "startLocalId == localId"))
    {
        __debugbreak();
    }
    if (fromEndon)
        RemoveVariable(inst, startLocalId, 0x17FFFu);
    Scr_KillThread(inst, startLocalId);
    RemoveRefToObject(inst, startLocalId);
    --gScrVarPub[inst].numScriptThreads;
    MT_Free((uint8*)stackValue, stackValue->bufLen, inst);
}

void __cdecl Scr_CancelWaittill(scriptInstance_t inst, unsigned int startLocalId)
{
    unsigned int ObjectVariable; // eax
    unsigned int v3; // eax
    VariableValueInternal_u *VariableValueAddress; // eax
    unsigned int selfNameId; // [esp+0h] [ebp-Ch]
    unsigned int selfId; // [esp+8h] [ebp-4h]

    selfId = Scr_GetSelf(inst, startLocalId);
    ObjectVariable = FindObjectVariable(inst, gScrVarPub[inst].pauseArrayId, selfId);
    selfNameId = FindObject(inst, ObjectVariable);
    v3 = FindObjectVariable(inst, selfNameId, startLocalId);
    VariableValueAddress = GetVariableValueAddress(inst, v3);
    VM_CancelNotify(inst, VariableValueAddress->next, startLocalId);
    RemoveObjectVariable(inst, selfNameId, startLocalId);
    if (!GetArraySize(inst, selfNameId))
        RemoveObjectVariable(inst, gScrVarPub[inst].pauseArrayId, selfId);
}

unsigned __int16 __cdecl Scr_ExecThread(scriptInstance_t inst, int handle, unsigned int paramcount)
{
    unsigned int v3; // eax
    char *pos; // [esp+14h] [ebp-Ch]
    const char *varUsagePos; // [esp+18h] [ebp-8h]
    unsigned int id; // [esp+1Ch] [ebp-4h]

    pos = (char *)&gScrVarPub[inst].programBuffer[handle];
    _mm_prefetch(pos, 1);
    if (!gScrVmPub[inst].function_count)
    {
        if ((int *)gScrVmPub[inst].localVars != &gScrVmGlob[inst].starttime
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                4583,
                0,
                "%s",
                "gScrVmPub[inst].localVars == gScrVmGlob[inst].localVarsStack - 1"))
        {
            __debugbreak();
        }
        Scr_ResetTimeout(inst);
    }
    if (!gScrVarPub[inst].timeArrayId
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4589,
            0,
            "%s",
            "gScrVarPub[inst].timeArrayId"))
    {
        __debugbreak();
    }
    if (!handle
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 4590, 0, "%s", "handle"))
    {
        __debugbreak();
    }
    ++gScrVarPub[inst].ext_threadcount;
    Scr_IsInOpcodeMemory(inst, pos);
    varUsagePos = gScrVarPub[inst].varUsagePos;
    gScrVarPub[inst].varUsagePos = pos + 1;
    AddRefToObject(inst, gScrVarPub[inst].levelId);
    v3 = AllocThread(inst, gScrVarPub[inst].levelId);
    id = VM_Execute(inst, v3, pos, paramcount);
    gScrVarPub[inst].varUsagePos = varUsagePos;
    if (gScrVarDebugPub[inst])
        ++gScrVarDebugPub[inst]->extRefCount[id];
    RemoveRefToValue(inst, gScrVmPub[inst].top->type, gScrVmPub[inst].top->u);
    gScrVmPub[inst].top->type = 0;
    --gScrVmPub[inst].top;
    --gScrVmPub[inst].inparamcount;
    if (!gScrVmPub[inst].function_count
        && (int *)gScrVmPub[inst].localVars != &gScrVmGlob[inst].starttime
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4624,
            0,
            "%s",
            "gScrVmPub[inst].localVars == gScrVmGlob[inst].localVarsStack - 1"))
    {
        __debugbreak();
    }
    return id;
}

unsigned int __cdecl VM_Execute(scriptInstance_t inst, unsigned int localId, char *pos, unsigned int paramcount)
{
    function_stack_t *v4; // edx
    function_stack_t *v5; // ecx
    function_stack_t *v6; // ecx
    function_stack_t *v7; // edx
    function_stack_t fs_backup[2]; // [esp+Ch] [ebp-34h] BYREF
    VariableValue *startTop; // [esp+34h] [ebp-Ch]
    int type; // [esp+38h] [ebp-8h]
    int thread_count_backup; // [esp+3Ch] [ebp-4h]
    unsigned int localIda; // [esp+4Ch] [ebp+Ch]
    unsigned int paramcounta; // [esp+54h] [ebp+14h]

    if (paramcount > gScrVmPub[inst].inparamcount
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4494,
            0,
            "%s",
            "paramcount <= gScrVmPub[inst].inparamcount"))
    {
        __debugbreak();
    }
    Scr_ClearOutParams(inst);
    startTop = &gScrVmPub[inst].top[-(int)paramcount];
    paramcounta = gScrVmPub[inst].inparamcount - paramcount;
    if (gScrVmPub[inst].function_count >= 30)
    {
        Scr_KillThread(inst, localId);
        gScrVmPub[inst].inparamcount = paramcounta + 1;
        if (gScrVmPub[inst].outparamcount
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                4552,
                0,
                "%s",
                "!gScrVmPub[inst].outparamcount"))
        {
            __debugbreak();
        }
        while (paramcounta)
        {
            RemoveRefToValue(inst, gScrVmPub[inst].top->type, gScrVmPub[inst].top->u);
            --gScrVmPub[inst].top;
            --paramcounta;
        }
        gScrVmPub[inst].top[1].type = 0;
        ++gScrVmPub[inst].top;
        RuntimeError(inst, pos, 0, "script stack overflow (too many embedded function calls)", 0);
        return localId;
    }
    else
    {
        v4 = &gFs[inst];
        v5 = &fs_backup[inst];
        v5->pos = v4->pos;
        v5->localId = v4->localId;
        v5->localVarCount = v4->localVarCount;
        v5->top = v4->top;
        v5->startTop = v4->startTop;
        thread_count_backup = gThreadCount[inst];
        gFs[inst].localId = localId;
        gFs[inst].startTop = startTop;
        if (gScrVmPub[inst].function_count)
        {
            ++gScrVmPub[inst].function_count;
            ++gScrVmPub[inst].function_frame;
            gScrVmPub[inst].function_frame->fs.localId = 0;
        }
        gScrVmPub[inst].function_frame->fs.pos = pos;
        ++gScrVmPub[inst].function_count;
        ++gScrVmPub[inst].function_frame;
        gScrVmPub[inst].function_frame->fs.localId = localId;
        type = startTop->type;
        startTop->type = 8;
        gScrVmPub[inst].inparamcount = 0;
        gFs[inst].top = gScrVmPub[inst].top;
        gFs[inst].pos = pos;
        gFs[inst].localVarCount = 0;
        gThreadCount[inst] = 0;
        localIda = VM_Execute_0(inst);
        v6 = &fs_backup[inst];
        v7 = &gFs[inst];
        v7->pos = v6->pos;
        v7->localId = v6->localId;
        v7->localVarCount = v6->localVarCount;
        v7->top = v6->top;
        v7->startTop = v6->startTop;
        gThreadCount[inst] = thread_count_backup;
        startTop->type = type;
        gScrVmPub[inst].top = startTop + 1;
        gScrVmPub[inst].inparamcount = paramcounta + 1;
        if (gScrVmPub[inst].outparamcount
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                4537,
                0,
                "%s",
                "!gScrVmPub[inst].outparamcount"))
        {
            __debugbreak();
        }
        ClearVariableValue(inst, gScrVarPub[inst].tempVariable);
        if (gScrVmPub[inst].function_count)
        {
            --gScrVmPub[inst].function_count;
            --gScrVmPub[inst].function_frame;
        }
        return localIda;
    }
}

void __cdecl RemoveRefToValue(scriptInstance_t inst, VariableValue *value)
{
    RemoveRefToValue(inst, value->type, value->u);
}

unsigned int __cdecl Scr_GetLocalVar(scriptInstance_t inst, const char *pos)
{
    return gScrVmPub[inst].localVars[-*(unsigned __int8 *)pos];
}

unsigned __int16 __cdecl Scr_ReadUnsignedShort(const char **pos)
{
    __int16 v2; // [esp+0h] [ebp-4h]

    v2 = *(_WORD *)*pos;
    *pos += 2;
    return v2;
}

const char *__cdecl Scr_ReadCodePos(scriptInstance_t inst, const char **pos)
{
    int v3; // [esp+0h] [ebp-8h]

    v3 = *(unsigned int *)*pos;
    *pos += 4;
    return (const char *)v3;
}

unsigned int __cdecl GetDummyObject(scriptInstance_t inst)
{
    ClearVariableValue(inst, gScrVarPub[inst].tempVariable);
    return GetObject(inst, gScrVarPub[inst].tempVariable);
}

int __cdecl GetDummyFieldValue(scriptInstance_t inst)
{
    ClearVariableValue(inst, gScrVarPub[inst].tempVariable);
    return gScrVarPub[inst].tempVariable;
}

void __cdecl VM_PrintJumpHistory(scriptInstance_t inst)
{
    char *pos; // [esp+0h] [ebp-8h]
    int index; // [esp+4h] [ebp-4h]

    Com_Printf(24, "********************************\n");
    Com_Printf(24, "Recent loop history (from most recent) :\n");
    index = gScrVmDebugPub[inst].jumpbackHistoryIndex;
    do
    {
        if (!index)
            index = 128;
        pos = (char *)gScrVmDebugPub[inst].jumpbackHistory[--index];
        if (!pos)
            break;
        Scr_PrintPrevCodePos(inst, 24, pos, 0);
    } while (index != gScrVmDebugPub[inst].jumpbackHistoryIndex);
    Com_Printf(24, "********************************\n");
}

VariableStackBuffer *__cdecl VM_ArchiveStack(scriptInstance_t inst, function_stack_t *stack)
{
    char *stackValue; // [esp+4h] [ebp-18h]
    VariableValue *top; // [esp+8h] [ebp-14h]
    char *buf; // [esp+Ch] [ebp-10h]
    char *bufa; // [esp+Ch] [ebp-10h]
    unsigned int localId; // [esp+10h] [ebp-Ch]
    signed int size; // [esp+14h] [ebp-8h]
    int bufLen; // [esp+18h] [ebp-4h]

    top = stack->top;
    size = top - stack->startTop;
    if (size != (unsigned __int16)size
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            3333,
            0,
            "%s",
            "size == (unsigned short)size"))
    {
        __debugbreak();
    }
    bufLen = 5 * size + 13;
    if (bufLen != (unsigned __int16)bufLen
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            3335,
            0,
            "%s",
            "bufLen == (unsigned short)bufLen"))
    {
        __debugbreak();
    }
    stackValue = MT_Alloc(bufLen, 1, inst);
    ++gScrVarPub[inst].numScriptThreads;
    localId = stack->localId;
    *((_DWORD *)stackValue + 2) = localId;
    *((_WORD *)stackValue + 2) = size;
    *((_WORD *)stackValue + 3) = bufLen;
    *(_DWORD *)stackValue = (DWORD)stack->pos;
    stackValue[12] = gScrVarPub[inst].time;
    gScrVmPub[inst].localVars -= stack->localVarCount;
    buf = &stackValue[5 * size + 13];
    while (size)
    {
        bufa = buf - 4;
        if (top->type == 7)
        {
            --gScrVmPub[inst].function_count;
            --gScrVmPub[inst].function_frame;
            *(_DWORD *)bufa = (DWORD)gScrVmPub[inst].function_frame->fs.pos;
            gScrVmPub[inst].localVars -= gScrVmPub[inst].function_frame->fs.localVarCount;
            localId = GetParentLocalId(inst, localId);
        }
        else
        {
            *(_DWORD *)bufa = top->u.intValue;
        }
        buf = bufa - 1;
        if (top->type >= 0x100u
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                3370,
                0,
                "%s",
                "top->type >= 0 && top->type < (1 << 8)"))
        {
            __debugbreak();
        }
        *buf = top->type;
        --top;
        --size;
    }
    --gScrVmPub[inst].function_count;
    --gScrVmPub[inst].function_frame;
    AddRefToObject(inst, localId);
    stack->localId = localId;
    return (VariableStackBuffer *)stackValue;
}

unsigned __int16 __cdecl Scr_ExecEntThreadNum(
    scriptInstance_t inst,
    unsigned int entnum,
    unsigned int classnum,
    int handle,
    unsigned int paramcount,
    unsigned __int16 localClientNum)
{
    unsigned int v6; // eax
    char *pos; // [esp+14h] [ebp-10h]
    const char *varUsagePos; // [esp+18h] [ebp-Ch]
    unsigned int objId; // [esp+1Ch] [ebp-8h]
    unsigned int id; // [esp+20h] [ebp-4h]

    pos = (char *)&gScrVarPub[inst].programBuffer[handle];
    _mm_prefetch(pos, 1);
    if (!gScrVmPub[inst].function_count)
    {
        if ((int *)gScrVmPub[inst].localVars != &gScrVmGlob[inst].starttime
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                4646,
                0,
                "%s",
                "gScrVmPub[inst].localVars == gScrVmGlob[inst].localVarsStack - 1"))
        {
            __debugbreak();
        }
        Scr_ResetTimeout(inst);
    }
    if (!gScrVarPub[inst].timeArrayId
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4652,
            0,
            "%s",
            "gScrVarPub[inst].timeArrayId"))
    {
        __debugbreak();
    }
    if (!handle
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 4653, 0, "%s", "handle"))
    {
        __debugbreak();
    }
    ++gScrVarPub[inst].ext_threadcount;
    if (!Scr_IsInOpcodeMemory(inst, pos)
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4659,
            0,
            "%s",
            "Scr_IsInOpcodeMemory( inst, pos )"))
    {
        __debugbreak();
    }
    varUsagePos = gScrVarPub[inst].varUsagePos;
    gScrVarPub[inst].varUsagePos = pos + 1;
    objId = Scr_GetEntityId(inst, entnum, classnum, localClientNum);
    AddRefToObject(inst, objId);
    v6 = AllocThread(inst, objId);
    id = VM_Execute(inst, v6, pos, paramcount);
    gScrVarPub[inst].varUsagePos = varUsagePos;
    if (gScrVarDebugPub[inst])
        ++gScrVarDebugPub[inst]->extRefCount[id];
    RemoveRefToValue(inst, gScrVmPub[inst].top->type, gScrVmPub[inst].top->u);
    gScrVmPub[inst].top->type = 0;
    --gScrVmPub[inst].top;
    --gScrVmPub[inst].inparamcount;
    if (!gScrVmPub[inst].function_count
        && (int *)gScrVmPub[inst].localVars != &gScrVmGlob[inst].starttime
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4688,
            0,
            "%s",
            "gScrVmPub[inst].localVars == gScrVmGlob[inst].localVarsStack - 1"))
    {
        __debugbreak();
    }
    return id;
}

void __cdecl Scr_AddExecThread(scriptInstance_t inst, int handle, unsigned int paramcount)
{
    unsigned int v3; // eax
    unsigned int v4; // eax
    char *pos; // [esp+8h] [ebp-8h]
    const char *varUsagePos; // [esp+Ch] [ebp-4h]

    pos = (char *)&gScrVarPub[inst].programBuffer[handle];
    _mm_prefetch(pos, 1);
    if (!gScrVmPub[inst].function_count)
    {
        if ((int *)gScrVmPub[inst].localVars != &gScrVmGlob[inst].starttime
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                4708,
                0,
                "%s",
                "gScrVmPub[inst].localVars == gScrVmGlob[inst].localVarsStack - 1"))
        {
            __debugbreak();
        }
        Scr_ResetTimeout(inst);
    }
    if (!gScrVarPub[inst].timeArrayId
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4714,
            0,
            "%s",
            "gScrVarPub[inst].timeArrayId"))
    {
        __debugbreak();
    }
    if (!handle
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 4715, 0, "%s", "handle"))
    {
        __debugbreak();
    }
    if (!Scr_IsInOpcodeMemory(inst, pos)
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4717,
            0,
            "%s",
            "Scr_IsInOpcodeMemory( inst, pos )"))
    {
        __debugbreak();
    }
    varUsagePos = gScrVarPub[inst].varUsagePos;
    gScrVarPub[inst].varUsagePos = pos + 1;
    AddRefToObject(inst, gScrVarPub[inst].levelId);
    v3 = AllocThread(inst, gScrVarPub[inst].levelId);
    v4 = VM_Execute(inst, v3, pos, paramcount);
    RemoveRefToObject(inst, v4);
    gScrVarPub[inst].varUsagePos = varUsagePos;
    ++gScrVmPub[inst].outparamcount;
    --gScrVmPub[inst].inparamcount;
    if (!gScrVmPub[inst].function_count
        && (int *)gScrVmPub[inst].localVars != &gScrVmGlob[inst].starttime
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4738,
            0,
            "%s",
            "gScrVmPub[inst].localVars == gScrVmGlob[inst].localVarsStack - 1"))
    {
        __debugbreak();
    }
}

void __cdecl Scr_FreeThread(unsigned __int16 handle, scriptInstance_t inst)
{
    if (!gScrVarPub[inst].timeArrayId
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4795,
            0,
            "%s",
            "gScrVarPub[inst].timeArrayId"))
    {
        __debugbreak();
    }
    if (!handle
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 4796, 0, "%s", "handle"))
    {
        __debugbreak();
    }
    if (gScrVarDebugPub[inst])
        --gScrVarDebugPub[inst]->extRefCount[handle];
    RemoveRefToObject(inst, handle);
    --gScrVarPub[inst].ext_threadcount;
}

void __cdecl Scr_ExecCode(scriptInstance_t inst, char *pos, unsigned int localId)
{
    unsigned int localIda; // [esp+14h] [ebp+10h]

    Scr_ResetTimeout(inst);
    if (!gScrVarPub[inst].timeArrayId
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4815,
            0,
            "%s",
            "gScrVarPub[inst].timeArrayId"))
    {
        __debugbreak();
    }
    if (gScrVmPub[inst].inparamcount
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4816,
            0,
            "%s",
            "!gScrVmPub[inst].inparamcount"))
    {
        __debugbreak();
    }
    if (gScrVmPub[inst].outparamcount
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4817,
            0,
            "%s",
            "!gScrVmPub[inst].outparamcount"))
    {
        __debugbreak();
    }
    if (gScrVarPub[inst].evaluate
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4819,
            0,
            "%s",
            "!gScrVarPub[inst].evaluate"))
    {
        __debugbreak();
    }
    if (gScrVmPub[inst].debugCode
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4821,
            0,
            "%s",
            "!gScrVmPub[inst].debugCode"))
    {
        __debugbreak();
    }
    gScrVmPub[inst].debugCode = 1;
    if (localId)
    {
        VM_Execute(inst, localId, pos, 0);
    }
    else
    {
        AddRefToObject(inst, gScrVarPub[inst].levelId);
        localIda = AllocThread(inst, gScrVarPub[inst].levelId);
        VM_Execute(inst, localIda, pos, 0);
        Scr_KillThread(inst, localIda);
        RemoveRefToObject(inst, localIda);
    }
    if (!gScrVmPub[inst].debugCode
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4837,
            0,
            "%s",
            "gScrVmPub[inst].debugCode"))
    {
        __debugbreak();
    }
    gScrVmPub[inst].debugCode = 0;
    if (gScrVmPub[inst].inparamcount != 1
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4840,
            0,
            "%s",
            "gScrVmPub[inst].inparamcount == 1"))
    {
        __debugbreak();
    }
    if (gScrVmPub[inst].outparamcount
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4841,
            0,
            "%s",
            "!gScrVmPub[inst].outparamcount"))
    {
        __debugbreak();
    }
    if (gScrVmPub[inst].function_count)
    {
        --gScrVmPub[inst].function_count;
        --gScrVmPub[inst].function_frame;
    }
    --gScrVmPub[inst].top;
    gScrVmPub[inst].inparamcount = 0;
}

bool __cdecl Scr_IsSystemInitied(scriptInstance_t inst)
{
    return gScrVarPub[inst].timeArrayId != 0;
}

void __cdecl Scr_InitSystem(scriptInstance_t inst, int sys)
{
    if (sys != 1
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4885,
            0,
            "%s",
            "sys == SCR_SYS_GAME"))
    {
        __debugbreak();
    }
    if (gScrVarPub[inst].timeArrayId
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4886,
            0,
            "%s",
            "!gScrVarPub[inst].timeArrayId"))
    {
        __debugbreak();
    }
    if (gScrVarPub[inst].ext_threadcount
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4888,
            0,
            "%s",
            "!gScrVarPub[inst].ext_threadcount"))
    {
        __debugbreak();
    }
    if (gScrVarPub[inst].varUsagePos
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4892,
            0,
            "%s",
            "!gScrVarPub[inst].varUsagePos"))
    {
        __debugbreak();
    }
    gScrVarPub[inst].varUsagePos = "<script init variable>";
    gScrVarPub[inst].timeArrayId = AllocObject(inst);
    if (gScrVarDebugPub[inst])
        ++gScrVarDebugPub[inst]->extRefCount[gScrVarPub[inst].timeArrayId];
    if (gScrVarPub[inst].pauseArrayId
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4902,
            0,
            "%s",
            "!gScrVarPub[inst].pauseArrayId"))
    {
        __debugbreak();
    }
    gScrVarPub[inst].pauseArrayId = Scr_AllocArray(inst);
    if (gScrVarDebugPub[inst])
        ++gScrVarDebugPub[inst]->extRefCount[gScrVarPub[inst].pauseArrayId];
    if (gScrVarPub[inst].levelId
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4909,
            0,
            "%s",
            "!gScrVarPub[inst].levelId"))
    {
        __debugbreak();
    }
    gScrVarPub[inst].levelId = AllocObject(inst);
    if (gScrVarDebugPub[inst])
        ++gScrVarDebugPub[inst]->extRefCount[gScrVarPub[inst].levelId];
    if (gScrVarPub[inst].animId
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4916,
            0,
            "%s",
            "!gScrVarPub[inst].animId"))
    {
        __debugbreak();
    }
    gScrVarPub[inst].animId = AllocObject(inst);
    if (gScrVarDebugPub[inst])
        ++gScrVarDebugPub[inst]->extRefCount[gScrVarPub[inst].animId];
    if (gScrVarPub[inst].freeEntList
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4923,
            0,
            "%s",
            "!gScrVarPub[inst].freeEntList"))
    {
        __debugbreak();
    }
    gScrVarPub[inst].time = 0;
    g_script_error_level[inst] = -1;
    if (inst == SCRIPTINSTANCE_SERVER)
        Scr_InitDebuggerSystem(SCRIPTINSTANCE_SERVER);
    gScrVarPub[inst].varUsagePos = 0;
}

void __cdecl Scr_ShutdownSystem(scriptInstance_t inst, unsigned __int8 sys, int bComplete)
{
    unsigned int Object; // eax
    unsigned int v4; // eax
    unsigned int parentId; // [esp+24h] [ebp-Ch]
    int function_count; // [esp+28h] [ebp-8h]
    unsigned int id; // [esp+2Ch] [ebp-4h]
    unsigned int ida; // [esp+2Ch] [ebp-4h]
    unsigned int idb; // [esp+2Ch] [ebp-4h]

    if (sys != 1
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4946,
            0,
            "%s",
            "sys == SCR_SYS_GAME"))
    {
        __debugbreak();
    }
    gScrVarPub[inst].varUsagePos = "<script shutdown variable>";
    Scr_ShutdownDebuggerSystem(inst);
    Scr_FreeEntityList(inst);
    if (gScrVarPub[inst].timeArrayId)
    {
        Scr_FreeGameVariable(inst, bComplete);
        function_count = gScrVmPub[inst].function_count;
        gScrVmPub[inst].function_count = 0;
        for (id = FindFirstSibling(inst, gScrVarPub[inst].timeArrayId); id; id = FindNextSibling(inst, id))
        {
            Object = FindObject(inst, id);
            VM_TerminateTime(inst, Object);
        }
        while (1)
        {
            ida = FindFirstSibling(inst, gScrVarPub[inst].pauseArrayId);
            if (!ida)
                break;
            v4 = FindObject(inst, ida);
            idb = FindFirstSibling(inst, v4);
            if (!idb
                && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 4980, 0, "%s", "id"))
            {
                __debugbreak();
            }
            parentId = GetVariableValueAddress(inst, idb)->next;
            AddRefToObject(inst, parentId);
            Scr_CancelNotifyList(inst, parentId);
            RemoveRefToObject(inst, parentId);
        }
        if (!gScrVarPub[inst].levelId
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                4987,
                0,
                "%s",
                "gScrVarPub[inst].levelId"))
        {
            __debugbreak();
        }
        ClearObject(inst, gScrVarPub[inst].levelId);
        if (gScrVarDebugPub[inst])
            --gScrVarDebugPub[inst]->extRefCount[gScrVarPub[inst].levelId];
        RemoveRefToEmptyObject(inst, gScrVarPub[inst].levelId);
        gScrVarPub[inst].levelId = 0;
        if (!gScrVarPub[inst].animId
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                4996,
                0,
                "%s",
                "gScrVarPub[inst].animId"))
        {
            __debugbreak();
        }
        ClearObject(inst, gScrVarPub[inst].animId);
        if (gScrVarDebugPub[inst])
            --gScrVarDebugPub[inst]->extRefCount[gScrVarPub[inst].animId];
        RemoveRefToEmptyObject(inst, gScrVarPub[inst].animId);
        gScrVarPub[inst].animId = 0;
        if (!gScrVarPub[inst].timeArrayId
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                5005,
                0,
                "%s",
                "gScrVarPub[inst].timeArrayId"))
        {
            __debugbreak();
        }
        ClearObject(inst, gScrVarPub[inst].timeArrayId);
        if (gScrVarDebugPub[inst])
            --gScrVarDebugPub[inst]->extRefCount[gScrVarPub[inst].timeArrayId];
        RemoveRefToEmptyObject(inst, gScrVarPub[inst].timeArrayId);
        gScrVarPub[inst].timeArrayId = 0;
        if (!gScrVarPub[inst].pauseArrayId
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                5014,
                0,
                "%s",
                "gScrVarPub[inst].pauseArrayId"))
        {
            __debugbreak();
        }
        if (GetArraySize(inst, gScrVarPub[inst].pauseArrayId)
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                5015,
                0,
                "%s",
                "!GetArraySize( inst, gScrVarPub[inst].pauseArrayId )"))
        {
            __debugbreak();
        }
        if (gScrVarDebugPub[inst])
            --gScrVarDebugPub[inst]->extRefCount[gScrVarPub[inst].pauseArrayId];
        RemoveRefToEmptyObject(inst, gScrVarPub[inst].pauseArrayId);
        gScrVarPub[inst].pauseArrayId = 0;
        if (gScrVarPub[inst].freeEntList
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                5023,
                0,
                "%s",
                "!gScrVarPub[inst].freeEntList"))
        {
            __debugbreak();
        }
        Scr_FreeObjects(inst);
        gScrVarPub[inst].varUsagePos = 0;
        if (function_count)
        {
            gScrVarPub[inst].bInited = 0;
            Scr_Init(inst);
            if (gScrStringDebugGlob[inst])
                gScrStringDebugGlob[inst]->ignoreLeaks = 1;
        }
        else
        {
            if (gScrVarPub[inst].ext_threadcount
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    5034,
                    0,
                    "%s",
                    "!gScrVarPub[inst].ext_threadcount"))
            {
                __debugbreak();
            }
            if (!Scr_IsStackClear(inst)
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    5036,
                    0,
                    "%s",
                    "Scr_IsStackClear(inst)"))
            {
                __debugbreak();
            }
        }
    }
    else
    {
        gScrVarPub[inst].varUsagePos = 0;
    }
}

void __cdecl VM_TerminateTime(scriptInstance_t inst, unsigned int timeId)
{
    VariableStackBuffer *stackValue; // [esp+8h] [ebp-Ch]
    unsigned int stackId; // [esp+Ch] [ebp-8h]
    unsigned int startLocalId; // [esp+10h] [ebp-4h]

    if (!timeId
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 4382, 0, "%s", "timeId"))
    {
        __debugbreak();
    }
    if (gScrVmPub[inst].function_count
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4383,
            0,
            "%s",
            "!gScrVmPub[inst].function_count"))
    {
        __debugbreak();
    }
    AddRefToObject(inst, timeId);
    while (1)
    {
        stackId = FindFirstSibling(inst, timeId);
        if (!stackId)
            break;
        startLocalId = GetVariableKeyObject(inst, stackId);
        if (!startLocalId
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                4392,
                0,
                "%s",
                "startLocalId"))
        {
            __debugbreak();
        }
        if (GetValueType(inst, stackId) != 10
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                4394,
                0,
                "%s",
                "GetValueType( inst, stackId ) == VAR_STACK"))
        {
            __debugbreak();
        }
        stackValue = (VariableStackBuffer *)GetVariableValueAddress(inst, stackId)->next;
        RemoveObjectVariable(inst, timeId, startLocalId);
        Scr_ClearWaitTime(inst, startLocalId);
        VM_TerminateStack(inst, startLocalId, startLocalId, stackValue);
    }
    RemoveRefToObject(inst, timeId);
}

bool __cdecl Scr_IsSystemActive(unsigned __int8 sys, scriptInstance_t inst)
{
    return gScrVarPub[inst].timeArrayId && !gScrVarPub[inst].error_message;
}

VariableUnion __cdecl Scr_GetInt(unsigned int index, scriptInstance_t inst)
{
    const char *v3; // eax
    const char *v4; // eax
    VariableValue *value; // [esp+0h] [ebp-4h]

    if (index < gScrVmPub[inst].outparamcount)
    {
        value = &gScrVmPub[inst].top[-(int)index];
        if (value->type == 6)
            return value->u;
        gScrVarPub[inst].error_index = index + 1;
        v3 = va("type %s is not an int", var_typename[value->type]);
        Scr_Error(inst, v3, 0);
    }
    v4 = va("parameter %d does not exist", index + 1);
    Scr_Error(inst, v4, 0);
    return 0;
}

scr_anim_s __cdecl Scr_GetAnim(unsigned int index, XAnimTree_s *tree, scriptInstance_t inst)
{
    const char *v4; // eax
    VariableValue *value; // [esp+10h] [ebp-4h]

    if (index < gScrVmPub[inst].outparamcount)
    {
        value = &gScrVmPub[inst].top[-(int)index];
        if (value->type == 11)
            return (scr_anim_s)value->u.intValue;
        gScrVarPub[inst].error_message = va("type %s is not an anim", var_typename[value->type]);
        RemoveRefToValue(inst, value->type, value->u);
        value->type = 0;
        gScrVarPub[inst].error_index = index + 1;
        Scr_ErrorInternal(inst);
    }
    v4 = va("parameter %d does not exist", index + 1);
    Scr_Error(inst, v4, 0);
    return 0;
}

void __cdecl Scr_ErrorInternal(scriptInstance_t inst)
{
    if (!gScrVarPub[inst].error_message
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            666,
            0,
            "%s",
            "gScrVarPub[inst].error_message"))
    {
        __debugbreak();
    }
    if (!gScrVarPub[inst].evaluate && !gScrCompilePub[inst].script_loading)
    {
        if (gScrVmPub[inst].function_count || gScrVmPub[inst].debugCode)
        {
            Com_PrintMessage(6, (char*)"throwing script exception: ", 0);
            Com_PrintMessage(6, (char *)gScrVarPub[inst].error_message, 0);
            Com_PrintMessage(6, (char *)"\n", 0);
            if ((unsigned int)g_script_error_level[inst] >= 0x21
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    703,
                    0,
                    "g_script_error_level[inst] doesn't index ARRAY_COUNT( g_script_error[inst] )\n\t%i not in [0, %i)",
                    g_script_error_level[inst],
                    33))
            {
                __debugbreak();
            }
            longjmp(g_script_error[inst][g_script_error_level[inst]], -1);
        }
    error_2:
        Sys_Error((char *)"%s", gScrVarPub[inst].error_message);
    }
    if (gScrVmPub[inst].terminal_error)
        goto error_2;
}

scr_animtree_t __cdecl Scr_GetAnimTree(unsigned int index, unsigned int user, scriptInstance_t inst)
{
    const char *v4; // eax
    unsigned int i; // [esp+8h] [ebp-8h]
    VariableValue *value; // [esp+Ch] [ebp-4h]

    if (index < gScrVmPub[inst].outparamcount)
    {
        value = &gScrVmPub[inst].top[-(int)index];
        if (value->type == 6)
        {
            i = value->u.intValue;
            if (value->u.intValue <= gScrAnimPub[inst].xanim_num[user] && gScrAnimPub[inst].xanim_lookup[user][i].anims)
                return (scr_animtree_t)gScrAnimPub[inst].xanim_lookup[user][i].anims;
            gScrVarPub[inst].error_message = "bad anim tree";
        }
        else
        {
            gScrVarPub[inst].error_message = va("type %s is not an animtree", var_typename[value->type]);
        }
        RemoveRefToValue(inst, value->type, value->u);
        value->type = 0;
        gScrVarPub[inst].error_index = index + 1;
        Scr_ErrorInternal(inst);
    }
    v4 = va("parameter %d does not exist", index + 1);
    Scr_Error(inst, v4, 0);
    return (scr_animtree_t)gScrAnimPub[inst].xanim_lookup[user][0].anims;
}

double __cdecl Scr_GetFloat(unsigned int index, scriptInstance_t inst)
{
    const char *v3; // eax
    const char *v4; // eax
    VariableValue *value; // [esp+0h] [ebp-4h]

    if (index < gScrVmPub[inst].outparamcount)
    {
        value = &gScrVmPub[inst].top[-(int)index];
        if (value->type == 5)
            return value->u.floatValue;
        if (value->type == 6)
            return (double)value->u.intValue;
        gScrVarPub[inst].error_index = index + 1;
        v3 = va("type %s is not a float", var_typename[value->type]);
        Scr_Error(inst, v3, 0);
    }
    v4 = va("parameter %d does not exist", index + 1);
    Scr_Error(inst, v4, 0);
    return 0.0;
}

VariableUnion __cdecl Scr_GetConstString(unsigned int index, scriptInstance_t inst)
{
    const char *v3; // eax
    VariableValue *value; // [esp+0h] [ebp-4h]

    if (index >= gScrVmPub[inst].outparamcount)
        goto LABEL_8;
    value = &gScrVmPub[inst].top[-(int)index];
    if (!Scr_CastString(inst, value))
    {
        gScrVarPub[inst].error_index = index + 1;
        Scr_ErrorInternal(inst);
    LABEL_8:
        v3 = va("parameter %d does not exist", index + 1);
        Scr_Error(inst, v3, 0);
        return 0;
    }
    if (value->type != 2
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5172,
            0,
            "%s",
            "value->type == VAR_STRING"))
    {
        __debugbreak();
    }
    SL_CheckExists(inst, value->u.intValue);
    return value->u;
}

VariableUnion __cdecl Scr_GetConstLowercaseString(unsigned int index, scriptInstance_t inst)
{
    unsigned int String; // eax
    const char *v4; // eax
    char *v5; // [esp+0h] [ebp-2018h]
    VariableUnion stringValue; // [esp+4h] [ebp-2014h]
    char str[8196]; // [esp+8h] [ebp-2010h] BYREF
    int i; // [esp+2010h] [ebp-8h]
    VariableValue *value; // [esp+2014h] [ebp-4h]

    if (index >= gScrVmPub[inst].outparamcount)
        goto LABEL_14;
    value = &gScrVmPub[inst].top[-(int)index];
    if (!Scr_CastString(inst, value))
    {
        gScrVarPub[inst].error_index = index + 1;
        Scr_ErrorInternal(inst);
    LABEL_14:
        v4 = va("parameter %d does not exist", index + 1);
        Scr_Error(inst, v4, 0);
        return 0;
    }
    if (value->type != 2
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5202,
            0,
            "%s",
            "value->type == VAR_STRING"))
    {
        __debugbreak();
    }
    stringValue.intValue = value->u.intValue;
    v5 = SL_ConvertToString(value->u.intValue, inst);
    for (i = 0; ; ++i)
    {
        str[i] = tolower(v5[i]);
        if (!v5[i])
            break;
    }
    if (value->type != 2
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5213,
            0,
            "%s",
            "value->type == VAR_STRING"))
    {
        __debugbreak();
    }
    String = SL_GetString(str, 0, inst);
    value->u.intValue = String;
    SL_RemoveRefToString(inst, stringValue.stringValue);
    SL_CheckExists(inst, value->u.intValue);
    return value->u;
}

char *__cdecl Scr_GetString(unsigned int index, scriptInstance_t inst)
{
    VariableUnion v2; // eax

    v2.intValue = Scr_GetConstString(index, inst).intValue;
    return SL_ConvertToString(v2.stringValue, inst);
}

VariableUnion __cdecl Scr_GetConstStringIncludeNull(unsigned int index, scriptInstance_t inst)
{
    if (index >= gScrVmPub[inst].outparamcount || gScrVmPub[inst].top[-(int)index].type)
        return Scr_GetConstString(index, inst);
    return 0;
}

char *__cdecl Scr_GetDebugString(unsigned int index, scriptInstance_t inst)
{
    const char *v3; // eax
    VariableValue *value; // [esp+0h] [ebp-4h]

    if (index >= gScrVmPub[inst].outparamcount)
    {
        v3 = va("parameter %d does not exist", index + 1);
        Scr_Error(inst, v3, 0);
        return 0;
    }
    else
    {
        value = &gScrVmPub[inst].top[-(int)index];
        Scr_CastDebugString(inst, value);
        if (value->type != 2
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                5260,
                0,
                "%s",
                "value->type == VAR_STRING"))
        {
            __debugbreak();
        }
        return SL_ConvertToString(value->u.intValue, inst);
    }
}

VariableUnion __cdecl Scr_GetConstIString(unsigned int index, scriptInstance_t inst)
{
    const char *v3; // eax
    const char *v4; // eax
    VariableValue *value; // [esp+0h] [ebp-4h]

    if (index < gScrVmPub[inst].outparamcount)
    {
        value = &gScrVmPub[inst].top[-(int)index];
        if (value->type == 3)
            return value->u;
        gScrVarPub[inst].error_index = index + 1;
        v3 = va("type %s is not a localized string", var_typename[value->type]);
        Scr_Error(inst, v3, 0);
    }
    v4 = va("parameter %d does not exist", index + 1);
    Scr_Error(inst, v4, 0);
    return 0;
}

char *__cdecl Scr_GetIString(unsigned int index, scriptInstance_t inst)
{
    VariableUnion v2; // eax

    v2.intValue = Scr_GetConstIString(index, inst).intValue;
    return SL_ConvertToString(v2.stringValue, inst);
}

void __cdecl Scr_GetVector(unsigned int index, float *vectorValue, scriptInstance_t inst)
{
    const char *v3; // eax
    const char *v4; // eax
    float *intValue; // [esp+0h] [ebp-8h]
    VariableValue *value; // [esp+4h] [ebp-4h]

    if (index < gScrVmPub[inst].outparamcount)
    {
        value = &gScrVmPub[inst].top[-(int)index];
        if (value->type == 4)
        {
            intValue = (float *)value->u.intValue;
            *vectorValue = *(float *)value->u.intValue;
            vectorValue[1] = intValue[1];
            vectorValue[2] = intValue[2];
            return;
        }
        gScrVarPub[inst].error_index = index + 1;
        v3 = va("type %s is not a vector", var_typename[value->type]);
        Scr_Error(inst, v3, 0);
    }
    v4 = va("parameter %d does not exist", index + 1);
    Scr_Error(inst, v4, 0);
}

unsigned int __cdecl Scr_GetFunc(unsigned int index, scriptInstance_t inst)
{
    const char *v3; // eax
    const char *v4; // eax
    VariableValue *value; // [esp+0h] [ebp-4h]

    if (index >= gScrVmPub[inst].outparamcount)
        goto LABEL_8;
    value = &gScrVmPub[inst].top[-(int)index];
    if (value->type != 9)
    {
        gScrVarPub[inst].error_index = index + 1;
        v3 = va("type %s is not a function", var_typename[value->type]);
        Scr_Error(inst, v3, 0);
    LABEL_8:
        v4 = va("parameter %d does not exist", index + 1);
        Scr_Error(inst, v4, 0);
        return 0;
    }
    if (!Scr_IsInOpcodeMemory(inst, value->u.codePosValue)
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5319,
            0,
            "%s",
            "Scr_IsInOpcodeMemory( inst, value->u.codePosValue )"))
    {
        __debugbreak();
    }
    return value->u.intValue - (unsigned int)gScrVarPub[inst].programBuffer;
}

scr_entref_t __cdecl Scr_GetEntityRef(unsigned int index, scriptInstance_t inst)
{
    unsigned int ObjectType; // eax
    const char *v5; // eax
    const char *v6; // eax
    const char *v7; // eax
    scr_entref_t v8; // [esp+0h] [ebp-20h] BYREF
    scr_entref_t v9; // [esp+Ah] [ebp-16h]
    scr_entref_t entref; // [esp+10h] [ebp-10h]
    VariableValue *value; // [esp+18h] [ebp-8h]
    unsigned int id; // [esp+1Ch] [ebp-4h]

    if (index < gScrVmPub[inst].outparamcount)
    {
        value = &gScrVmPub[inst].top[-(int)index];
        if (value->type == 1)
        {
            id = value->u.intValue;
            if (GetObjectType(inst, id) == 19)
            {
                return Scr_GetEntityIdRef(inst, id);
            }
            gScrVarPub[inst].error_index = index + 1;
            ObjectType = GetObjectType(inst, id);
            v5 = va("type %s is not an entity", var_typename[ObjectType]);
            Scr_Error(inst, v5, 0);
        }
        gScrVarPub[inst].error_index = index + 1;
        v6 = va("type %s is not an entity", var_typename[value->type]);
        Scr_Error(inst, v6, 0);
    }
    v7 = va("parameter %d does not exist", index + 1);
    Scr_Error(inst, v7, 0);
    return 0;
}

VariableUnion __cdecl Scr_GetObject(unsigned int index, scriptInstance_t inst)
{
    const char *v3; // eax
    const char *v4; // eax
    VariableValue *value; // [esp+0h] [ebp-4h]

    if (index < gScrVmPub[inst].outparamcount)
    {
        value = &gScrVmPub[inst].top[-(int)index];
        if (value->type == 1)
            return value->u;
        gScrVarPub[inst].error_index = index + 1;
        v3 = va("type %s is not an object", var_typename[value->type]);
        Scr_Error(inst, v3, 0);
    }
    v4 = va("parameter %d does not exist", index + 1);
    Scr_Error(inst, v4, 0);
    return 0;
}

int __cdecl Scr_GetType(unsigned int index, scriptInstance_t inst)
{
    const char *v3; // eax

    if (index < gScrVmPub[inst].outparamcount)
        return gScrVmPub[inst].top[-(int)index].type;
    v3 = va("parameter %d does not exist", index + 1);
    Scr_Error(inst, v3, 0);
    return 0;
}

const char *__cdecl Scr_GetTypeName(unsigned int index, scriptInstance_t inst)
{
    const char *v3; // eax

    if (index < gScrVmPub[inst].outparamcount)
        return var_typename[gScrVmPub[inst].top[-(int)index].type];
    v3 = va("parameter %d does not exist", index + 1);
    Scr_Error(inst, v3, 0);
    return 0;
}

unsigned int __cdecl Scr_GetPointerType(unsigned int index, scriptInstance_t inst)
{
    const char *v3; // eax
    const char *v4; // eax

    if (index < gScrVmPub[inst].outparamcount)
    {
        if (gScrVmPub[inst].top[-(int)index].type == 1)
            return GetObjectType(inst, gScrVmPub[inst].top[-(int)index].u.stringValue);
        v3 = va("type %s is not an object", var_typename[gScrVmPub[inst].top[-(int)index].type]);
        Scr_Error(inst, v3, 0);
    }
    v4 = va("parameter %d does not exist", index + 1);
    Scr_Error(inst, v4, 0);
    return 0;
}

int __cdecl Scr_GetNumParam(scriptInstance_t inst)
{
    return gScrVmPub[inst].outparamcount;
}

void __cdecl Scr_AddBool(unsigned int value, scriptInstance_t inst)
{
    if (value >= 2
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5417,
            0,
            "%s",
            "value == 0 || value == 1"))
    {
        __debugbreak();
    }
    IncInParam(inst);
    gScrVmPub[inst].top->type = 6;
    gScrVmPub[inst].top->u.intValue = value;
}

void __cdecl IncInParam(scriptInstance_t inst)
{
    if ((gScrVmPub[inst].top < (VariableValue *)(8232 * inst + 168191936)
        || gScrVmPub[inst].top > gScrVmGlob[inst].eval_stack)
        && (gScrVmPub[inst].top < gScrVmPub[inst].stack || gScrVmPub[inst].top > gScrVmPub[inst].maxstack)
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4473,
            0,
            "%s",
            "((gScrVmPub[inst].top >= gScrVmGlob[inst].eval_stack - 1) && (gScrVmPub[inst].top <= gScrVmGlob[inst].eval_sta"
            "ck)) || ((gScrVmPub[inst].top >= gScrVmPub[inst].stack) && (gScrVmPub[inst].top <= gScrVmPub[inst].maxstack))"))
    {
        __debugbreak();
    }
    Scr_ClearOutParams(inst);
    if (gScrVmPub[inst].top == gScrVmPub[inst].maxstack)
        Sys_Error((char*)"Internal script stack overflow");
    ++gScrVmPub[inst].top;
    ++gScrVmPub[inst].inparamcount;
    if ((gScrVmPub[inst].top < gScrVmGlob[inst].eval_stack || gScrVmPub[inst].top > &gScrVmGlob[inst].eval_stack[1])
        && (gScrVmPub[inst].top < gScrVmPub[inst].stack || gScrVmPub[inst].top > gScrVmPub[inst].maxstack)
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4483,
            0,
            "%s",
            "((gScrVmPub[inst].top >= gScrVmGlob[inst].eval_stack) && (gScrVmPub[inst].top <= gScrVmGlob[inst].eval_stack +"
            " 1)) || ((gScrVmPub[inst].top >= gScrVmPub[inst].stack) && (gScrVmPub[inst].top <= gScrVmPub[inst].maxstack))"))
    {
        __debugbreak();
    }
}

void __cdecl Scr_AddInt(int value, scriptInstance_t inst)
{
    IncInParam(inst);
    gScrVmPub[inst].top->type = 6;
    gScrVmPub[inst].top->u.intValue = value;
}

void __cdecl Scr_AddFloat(VariableUnion value, scriptInstance_t inst)
{
    IncInParam(inst);
    gScrVmPub[inst].top->type = 5;
    *(float *)&gScrVmPub[inst].top->u.intValue = value.floatValue;
}

void __cdecl Scr_AddAnim(scr_anim_s value, scriptInstance_t inst)
{
    IncInParam(inst);
    gScrVmPub[inst].top->type = 11;
    gScrVmPub[inst].top->u.intValue = (int)value.linkPointer;
}

void __cdecl Scr_AddUndefined(scriptInstance_t inst)
{
    IncInParam(inst);
    gScrVmPub[inst].top->type = 0;
}

void __cdecl Scr_AddObject(unsigned int id, scriptInstance_t inst)
{
    if (!id && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 5458, 0, "%s", "id"))
        __debugbreak();
    if (GetObjectType(inst, id) == 13
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5461,
            0,
            "%s",
            "GetObjectType( inst, id ) != VAR_THREAD"))
    {
        __debugbreak();
    }
    if (GetObjectType(inst, id) == 14
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5462,
            0,
            "%s",
            "GetObjectType( inst, id ) != VAR_NOTIFY_THREAD"))
    {
        __debugbreak();
    }
    if (GetObjectType(inst, id) == 15
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5463,
            0,
            "%s",
            "GetObjectType( inst, id ) != VAR_TIME_THREAD"))
    {
        __debugbreak();
    }
    if (GetObjectType(inst, id) == 16
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5464,
            0,
            "%s",
            "GetObjectType( inst, id ) != VAR_CHILD_THREAD"))
    {
        __debugbreak();
    }
    if (GetObjectType(inst, id) == 21
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5465,
            0,
            "%s",
            "GetObjectType( inst, id ) != VAR_DEAD_THREAD"))
    {
        __debugbreak();
    }
    IncInParam(inst);
    gScrVmPub[inst].top->type = 1;
    gScrVmPub[inst].top->u.intValue = id;
    AddRefToObject(inst, id);
}

void __cdecl Scr_AddEntityNum(unsigned int entnum, unsigned int classnum, scriptInstance_t inst, int clientNum)
{
    unsigned int EntityId; // eax
    const char *varUsagePos; // [esp+0h] [ebp-4h]

    varUsagePos = gScrVarPub[inst].varUsagePos;
    if (!varUsagePos)
        gScrVarPub[inst].varUsagePos = "<script entity variable>";
    EntityId = Scr_GetEntityId(inst, entnum, classnum, clientNum);
    Scr_AddObject(EntityId, inst);
    gScrVarPub[inst].varUsagePos = varUsagePos;
}

void __cdecl Scr_AddStruct(scriptInstance_t inst)
{
    unsigned int id; // [esp+0h] [ebp-4h]

    id = AllocObject(inst);
    Scr_AddObject(id, inst);
    RemoveRefToObject(inst, id);
}

void __cdecl Scr_AddString(char *value, scriptInstance_t inst)
{
    if (!value
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 5512, 0, "%s", "value"))
    {
        __debugbreak();
    }
    IncInParam(inst);
    gScrVmPub[inst].top->type = 2;
    gScrVmPub[inst].top->u.intValue = SL_GetString(value, 0, inst);
}

void __cdecl Scr_AddIString(char *value, scriptInstance_t inst)
{
    if (!value
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 5522, 0, "%s", "value"))
    {
        __debugbreak();
    }
    IncInParam(inst);
    gScrVmPub[inst].top->type = 3;
    gScrVmPub[inst].top->u.intValue = SL_GetString(value, 0, inst);
}

void __cdecl Scr_AddConstString(unsigned int value, scriptInstance_t inst)
{
    if (!value
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 5532, 0, "%s", "value"))
    {
        __debugbreak();
    }
    IncInParam(inst);
    gScrVmPub[inst].top->type = 2;
    gScrVmPub[inst].top->u.intValue = value;
    SL_AddRefToString(value, inst);
}

void __cdecl Scr_AddVector(float *value, scriptInstance_t inst)
{
    IncInParam(inst);
    gScrVmPub[inst].top->type = 4;
    gScrVmPub[inst].top->u.intValue = (int)Scr_AllocVector(inst, value);
}

void __cdecl Scr_MakeArray(scriptInstance_t inst)
{
    IncInParam(inst);
    gScrVmPub[inst].top->type = 1;
    gScrVmPub[inst].top->u.intValue = Scr_AllocArray(inst);
}

void __cdecl Scr_AddArray(scriptInstance_t inst)
{
    unsigned int ArraySize; // eax
    const char *varUsagePos; // [esp+0h] [ebp-8h]
    unsigned int id; // [esp+4h] [ebp-4h]

    varUsagePos = gScrVarPub[inst].varUsagePos;
    if (!varUsagePos)
        gScrVarPub[inst].varUsagePos = "<script array variable>";
    if (!gScrVmPub[inst].inparamcount
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5568,
            0,
            "%s",
            "gScrVmPub[inst].inparamcount"))
    {
        __debugbreak();
    }
    --gScrVmPub[inst].top;
    --gScrVmPub[inst].inparamcount;
    if (gScrVmPub[inst].top->type != 1
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5571,
            0,
            "%s",
            "gScrVmPub[inst].top->type == VAR_POINTER"))
    {
        __debugbreak();
    }
    ArraySize = GetArraySize(inst, gScrVmPub[inst].top->u.stringValue);
    id = GetNewArrayVariable(inst, gScrVmPub[inst].top->u.stringValue, ArraySize);
    SetNewVariableValue(inst, id, gScrVmPub[inst].top + 1);
    gScrVarPub[inst].varUsagePos = varUsagePos;
}

void __cdecl Scr_AddArrayStringIndexed(unsigned int stringValue, scriptInstance_t inst)
{
    unsigned int id; // [esp+0h] [ebp-4h]

    if (!gScrVmPub[inst].inparamcount
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5585,
            0,
            "%s",
            "gScrVmPub[inst].inparamcount"))
    {
        __debugbreak();
    }
    --gScrVmPub[inst].top;
    --gScrVmPub[inst].inparamcount;
    if (gScrVmPub[inst].top->type != 1
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5588,
            0,
            "%s",
            "gScrVmPub[inst].top->type == VAR_POINTER"))
    {
        __debugbreak();
    }
    id = GetNewVariable(inst, gScrVmPub[inst].top->u.stringValue, stringValue);
    SetNewVariableValue(inst, id, gScrVmPub[inst].top + 1);
}

void __cdecl Scr_Error(const char *error, bool force_terminal)
{
    Scr_Error(SCRIPTINSTANCE_SERVER, error, force_terminal);
}

void __cdecl Scr_Error(scriptInstance_t inst, const char *error, bool force_terminal)
{
    Scr_SetErrorMessage(inst, error);
    gScrVmPub[inst].terminal_error = force_terminal;
    Scr_ErrorInternal(inst);
}

char error_message[1024];
void __cdecl Scr_SetErrorMessage(scriptInstance_t inst, const char *error)
{
    if (!gScrVarPub[inst].error_message)
    {
        I_strncpyz(error_message, error, 1024);
        gScrVarPub[inst].error_message = error_message;
    }
}

void __cdecl Scr_TerminalError(const char *error, scriptInstance_t inst)
{
    Scr_DumpScriptThreads(inst);
    Scr_DumpScriptVariablesDefault(inst);
    gScrVmPub[inst].terminal_error = 1;
    Scr_Error(inst, error, 0);
}

void __cdecl Scr_NeverTerminalError(const char *error, scriptInstance_t inst)
{
    if (gScrVmGlob[inst].loading)
    {
        Scr_SetErrorMessage(inst, error);
        longjmp(g_script_error[inst][g_script_error_level[inst]], -1);
    }
    Scr_Error(inst, error, 0);
}

void __cdecl Scr_ParamError(unsigned int index, const char *error, scriptInstance_t inst)
{
    if (index >= gScrVmPub[inst].outparamcount
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5649,
            0,
            "%s",
            "index < gScrVmPub[inst].outparamcount"))
    {
        __debugbreak();
    }
    gScrVarPub[inst].error_index = index + 1;
    Scr_Error(inst, error, 0);
}

void __cdecl Scr_ObjectError(const char *error, scriptInstance_t inst)
{
    gScrVarPub[inst].error_index = -1;
    Scr_Error(inst, error, 0);
}

char __cdecl SetEntityFieldValue(
    scriptInstance_t inst,
    unsigned int classnum,
    unsigned int entnum,
    unsigned __int16 clientNum,
    unsigned int offset,
    VariableValue *value)
{
    int bSet; // [esp+Ch] [ebp-4h]

    if (value - gScrVmPub[inst].stack <= 0
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5664,
            0,
            "%s\n\t(value - gScrVmPub[inst].stack) = %i",
            "(value - gScrVmPub[inst].stack > 0)",
            value - gScrVmPub[inst].stack))
    {
        __debugbreak();
    }
    if (value - gScrVmPub[inst].maxstack > 0
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5665,
            0,
            "%s\n\t(value - gScrVmPub[inst].maxstack) = %i",
            "(value - gScrVmPub[inst].maxstack <= 0)",
            value - gScrVmPub[inst].maxstack))
    {
        __debugbreak();
    }
    if (gScrVmPub[inst].inparamcount
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5666,
            0,
            "%s",
            "!gScrVmPub[inst].inparamcount"))
    {
        __debugbreak();
    }
    if (gScrVmPub[inst].outparamcount
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5667,
            0,
            "%s",
            "!gScrVmPub[inst].outparamcount"))
    {
        __debugbreak();
    }
    gScrVmPub[inst].outparamcount = 1;
    gScrVmPub[inst].top = value;
    if (inst == SCRIPTINSTANCE_CLIENT)
        bSet = CScr_SetObjectField(classnum, entnum, clientNum, offset);
    else
        bSet = Scr_SetObjectField(classnum, entnum, offset);
    if (bSet)
    {
        if (gScrVmPub[inst].inparamcount
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                5691,
                0,
                "%s",
                "!gScrVmPub[inst].inparamcount"))
        {
            __debugbreak();
        }
        if (gScrVmPub[inst].outparamcount)
        {
            if (gScrVmPub[inst].outparamcount != 1
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    5695,
                    0,
                    "%s",
                    "gScrVmPub[inst].outparamcount == 1"))
            {
                __debugbreak();
            }
            RemoveRefToValue(inst, gScrVmPub[inst].top->type, gScrVmPub[inst].top->u);
            --gScrVmPub[inst].top;
            gScrVmPub[inst].outparamcount = 0;
        }
        return 1;
    }
    else
    {
        if (gScrVmPub[inst].inparamcount
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                5685,
                0,
                "%s",
                "!gScrVmPub[inst].inparamcount"))
        {
            __debugbreak();
        }
        if (gScrVmPub[inst].outparamcount != 1
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                5686,
                0,
                "%s",
                "gScrVmPub[inst].outparamcount == 1"))
        {
            __debugbreak();
        }
        gScrVmPub[inst].outparamcount = 0;
        return 0;
    }
}

VariableValue __cdecl GetEntityFieldValue(
    scriptInstance_t inst,
    unsigned int classnum,
    unsigned int entnum,
    unsigned __int16 clientNum,
    unsigned int offset)
{
    VariableValue result; // rax

    if (gScrVmPub[inst].inparamcount
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5708,
            0,
            "%s",
            "!gScrVmPub[inst].inparamcount"))
    {
        __debugbreak();
    }
    if (gScrVmPub[inst].outparamcount
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5709,
            0,
            "%s",
            "!gScrVmPub[inst].outparamcount"))
    {
        __debugbreak();
    }
    gScrVmPub[inst].top = (VariableValue *)(8232 * inst + 168191936);
    gScrVmGlob[inst].eval_stack[0].type = 0;
    if (inst == SCRIPTINSTANCE_CLIENT)
        CScr_GetObjectField(classnum, entnum, clientNum, offset);
    else
        Scr_GetObjectField(classnum, entnum, offset);
    if (gScrVmPub[inst].inparamcount >= 2
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5723,
            0,
            "%s",
            "!gScrVmPub[inst].inparamcount || gScrVmPub[inst].inparamcount == 1"))
    {
        __debugbreak();
    }
    if (gScrVmPub[inst].outparamcount
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5724,
            0,
            "%s",
            "!gScrVmPub[inst].outparamcount"))
    {
        __debugbreak();
    }
    if (&gScrVmPub[inst].top[-(int)gScrVmPub[inst].inparamcount] != (VariableValue *)(8232 * inst + 168191936)
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5725,
            0,
            "%s",
            "gScrVmPub[inst].top - gScrVmPub[inst].inparamcount == gScrVmGlob[inst].eval_stack - 1"))
    {
        __debugbreak();
    }
    gScrVmPub[inst].inparamcount = 0;
    result.u.intValue = gScrVmGlob[inst].eval_stack[0].u.intValue;
    result.type = gScrVmGlob[inst].eval_stack[0].type;
    return result;
}

void __cdecl Scr_SetStructField(unsigned int structId, unsigned int index, scriptInstance_t inst)
{
    unsigned int fieldValueId; // [esp+0h] [ebp-8h]

    if (gScrVmPub[inst].outparamcount
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5737,
            0,
            "%s",
            "!gScrVmPub[inst].outparamcount"))
    {
        __debugbreak();
    }
    if (gScrVmPub[inst].inparamcount != 1
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5738,
            0,
            "%s",
            "gScrVmPub[inst].inparamcount == 1"))
    {
        __debugbreak();
    }
    gScrVarPub[inst].varUsagePos = "<radiant field variable>";
    fieldValueId = gScrVarGlob[inst].variableList[Scr_GetVariableFieldIndex(inst, structId, index) + 0x8000].hash.id;
    if (gScrVmPub[inst].inparamcount != 1
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5748,
            0,
            "%s",
            "gScrVmPub[inst].inparamcount == 1"))
    {
        __debugbreak();
    }
    gScrVmPub[inst].inparamcount = 0;
    SetVariableFieldValue(inst, fieldValueId, gScrVmPub[inst].top);
    if (gScrVmPub[inst].inparamcount
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5753,
            0,
            "%s",
            "!gScrVmPub[inst].inparamcount"))
    {
        __debugbreak();
    }
    if (gScrVmPub[inst].outparamcount
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5754,
            0,
            "%s",
            "!gScrVmPub[inst].outparamcount"))
    {
        __debugbreak();
    }
    --gScrVmPub[inst].top;
    gScrVarPub[inst].varUsagePos = 0;
}

void __cdecl Scr_SetDynamicEntityField(
    unsigned int entnum,
    unsigned int classnum,
    unsigned int index,
    scriptInstance_t inst,
    unsigned __int16 localClientNum)
{
    unsigned int entId; // [esp+0h] [ebp-4h]

    gScrVarPub[inst].varUsagePos = "<radiant field variable>";
    entId = Scr_GetEntityId(inst, entnum, classnum, localClientNum);
    if (GetObjectType(inst, entId) != 19
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5774,
            0,
            "%s",
            "GetObjectType( inst, entId ) == VAR_ENTITY"))
    {
        __debugbreak();
    }
    gScrVarPub[inst].varUsagePos = 0;
    Scr_SetStructField(entId, index, inst);
}

void __cdecl Scr_IncTime(scriptInstance_t inst)
{
    Scr_RunCurrentThreads(inst);
    Scr_FreeEntityList(inst);
    if ((gScrVarPub[inst].time & 0xFF000000) != 0
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5789,
            0,
            "%s",
            "!(gScrVarPub[inst].time & ~VAR_NAME_LOW_MASK)"))
    {
        __debugbreak();
    }
    ++gScrVarPub[inst].time;
    gScrVarPub[inst].time &= 0xFFFFFFu;
    gScrVmPub[inst].showError = gScrVmPub[inst].abort_on_error;
}

void __cdecl Scr_DecTime(scriptInstance_t inst)
{
    if ((gScrVarPub[inst].time & 0xFF000000) != 0
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5801,
            0,
            "%s",
            "!(gScrVarPub[inst].time & ~VAR_NAME_LOW_MASK)"))
    {
        __debugbreak();
    }
    --gScrVarPub[inst].time;
    gScrVarPub[inst].time &= 0xFFFFFFu;
}

void __cdecl Scr_RunCurrentThreads(scriptInstance_t inst)
{
    DWORD pre_time; // [esp+0h] [ebp-4h]

    if (scrVmEnableScripts->current.enabled)
    {
        pre_time = Sys_MillisecondsRaw();
        if (gScrVmPub[inst].function_count
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                5818,
                0,
                "%s",
                "!gScrVmPub[inst].function_count"))
        {
            __debugbreak();
        }
        if (gScrVarPub[inst].error_message
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                5819,
                0,
                "%s",
                "!gScrVarPub[inst].error_message"))
        {
            __debugbreak();
        }
        if (gScrVarPub[inst].error_index
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                5820,
                0,
                "%s",
                "!gScrVarPub[inst].error_index"))
        {
            __debugbreak();
        }
        if (gScrVmPub[inst].outparamcount
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                5821,
                0,
                "%s",
                "!gScrVmPub[inst].outparamcount"))
        {
            __debugbreak();
        }
        if (gScrVmPub[inst].inparamcount
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                5822,
                0,
                "%s",
                "!gScrVmPub[inst].inparamcount"))
        {
            __debugbreak();
        }
        if (gScrVmPub[inst].top != gScrVmPub[inst].stack
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                5823,
                0,
                "%s",
                "gScrVmPub[inst].top == gScrVmPub[inst].stack"))
        {
            __debugbreak();
        }
        VM_SetTime(inst);
        gScrExecuteTime[inst] += Sys_MillisecondsRaw() - pre_time;
    }
}

void __cdecl VM_SetTime(scriptInstance_t inst)
{
    DWORD v1; // eax
    unsigned int Object; // eax
    unsigned int id; // [esp+0h] [ebp-4h]

    if ((gScrVarPub[inst].time & 0xFF000000) != 0
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4858,
            0,
            "%s",
            "!(gScrVarPub[inst].time & ~VAR_NAME_LOW_MASK)"))
    {
        __debugbreak();
    }
    if (gScrVarPub[inst].timeArrayId)
    {
        id = FindVariable(inst, gScrVarPub[inst].timeArrayId, gScrVarPub[inst].time);
        if (id)
        {
            if (!logScriptTimes
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    4866,
                    0,
                    "%s",
                    "logScriptTimes"))
            {
                __debugbreak();
            }
            if (logScriptTimes->current.enabled)
            {
                v1 = Sys_Milliseconds();
                Com_Printf(24, "SET TIME: %d\n", v1);
            }
            Object = FindObject(inst, id);
            VM_Resume(inst, Object);
            SafeRemoveVariable(inst, gScrVarPub[inst].timeArrayId, gScrVarPub[inst].time);
        }
    }
}

void __cdecl VM_Resume(scriptInstance_t inst, unsigned int timeId)
{
    unsigned int v2; // eax
    VariableStackBuffer *stackValue; // [esp+1Ch] [ebp-10h]
    unsigned int stackId; // [esp+20h] [ebp-Ch]
    unsigned int startLocalId; // [esp+28h] [ebp-4h]

    //PIXBeginNamedEvent(-1, "VM_Resume");
    if (gScrVmPub[inst].top != gScrVmPub[inst].stack
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4418,
            0,
            "%s",
            "gScrVmPub[inst].top == gScrVmPub[inst].stack"))
    {
        __debugbreak();
    }
    Scr_ResetAbortDebugger(inst);
    Scr_ResetTimeout(inst);
    if (!timeId
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 4426, 0, "%s", "timeId"))
    {
        __debugbreak();
    }
    AddRefToObject(inst, timeId);
    gFs[inst].startTop = gScrVmPub[inst].stack;
    gThreadCount[inst] = 0;
    while (1)
    {
        if (gScrVarPub[inst].error_message
            && !va("%s", gScrVarPub[inst].error_message)
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                4437,
                0,
                "%s",
                "va(\"%s\", gScrVarPub[inst].error_message)"))
        {
            __debugbreak();
        }
        if (gScrVarPub[inst].error_index
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                4439,
                0,
                "%s",
                "!gScrVarPub[inst].error_index"))
        {
            __debugbreak();
        }
        if (gScrVmPub[inst].outparamcount
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                4440,
                0,
                "%s",
                "!gScrVmPub[inst].outparamcount"))
        {
            __debugbreak();
        }
        if (gScrVmPub[inst].inparamcount
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                4441,
                0,
                "%s",
                "!gScrVmPub[inst].inparamcount"))
        {
            __debugbreak();
        }
        if (gScrVmPub[inst].function_count
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                4442,
                0,
                "%s",
                "!gScrVmPub[inst].function_count"))
        {
            __debugbreak();
        }
        if ((int *)gScrVmPub[inst].localVars != &gScrVmGlob[inst].starttime
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                4443,
                0,
                "%s",
                "gScrVmPub[inst].localVars == gScrVmGlob[inst].localVarsStack - 1"))
        {
            __debugbreak();
        }
        if (gFs[inst].startTop != gScrVmPub[inst].stack
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                4444,
                0,
                "%s",
                "gFs[inst].startTop == &gScrVmPub[inst].stack[0]"))
        {
            __debugbreak();
        }
        if (gThreadCount[inst]
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                4445,
                0,
                "%s",
                "!gThreadCount[inst]"))
        {
            __debugbreak();
        }
        stackId = FindFirstSibling(inst, timeId);
        if (!stackId)
            break;
        startLocalId = GetVariableKeyObject(inst, stackId);
        if (!startLocalId
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                4452,
                0,
                "%s",
                "startLocalId"))
        {
            __debugbreak();
        }
        if (GetValueType(inst, stackId) != 10
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                4454,
                0,
                "%s",
                "GetValueType( inst, stackId ) == VAR_STACK"))
        {
            __debugbreak();
        }
        stackValue = (VariableStackBuffer *)GetVariableValueAddress(inst, stackId)->next;
        RemoveObjectVariable(inst, timeId, startLocalId);
        VM_UnarchiveStack(inst, startLocalId, stackValue);
        v2 = VM_Execute_0(inst);
        RemoveRefToObject(inst, v2);
        RemoveRefToValue(inst, gScrVmPub[inst].stack[1].type, gScrVmPub[inst].stack[1].u);
    }
    RemoveRefToObject(inst, timeId);
    ClearVariableValue(inst, gScrVarPub[inst].tempVariable);
    gScrVmPub[inst].top = gScrVmPub[inst].stack;
    //if (g_DXDeviceThread == GetCurrentThreadId())
    //    //D3DPERF_EndEvent();
}

void __cdecl VM_UnarchiveStack(scriptInstance_t inst, unsigned int startLocalId, VariableStackBuffer *stackValue)
{
    VariableValue *top; // [esp+8h] [ebp-14h]
    char *buf; // [esp+Ch] [ebp-10h]
    const char **bufa; // [esp+Ch] [ebp-10h]
    unsigned int localId; // [esp+10h] [ebp-Ch]
    int function_count; // [esp+14h] [ebp-8h]
    int size; // [esp+18h] [ebp-4h]

    if (gScrVmPub[inst].function_count
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            3411,
            0,
            "%s",
            "!gScrVmPub[inst].function_count"))
    {
        __debugbreak();
    }
    if (!stackValue->pos
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            3412,
            0,
            "%s",
            "stackValue->pos"))
    {
        __debugbreak();
    }
    if (gFs[inst].startTop != gScrVmPub[inst].stack
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            3413,
            0,
            "%s",
            "gFs[inst].startTop == &gScrVmPub[inst].stack[0]"))
    {
        __debugbreak();
    }
    _mm_prefetch(stackValue->pos, 1);
    gScrVmPub[inst].function_frame->fs.pos = stackValue->pos;
    ++gScrVmPub[inst].function_count;
    ++gScrVmPub[inst].function_frame;
    size = stackValue->size;
    buf = stackValue->buf;
    top = gScrVmPub[inst].stack;
    while (size)
    {
        --size;
        top[1].type = (unsigned __int8)*buf;
        ++top;
        bufa = (const char **)(buf + 1);
        if (top->type == 7)
        {
            if (gScrVmPub[inst].function_count >= 32
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    3433,
                    0,
                    "%s",
                    "gScrVmPub[inst].function_count < MAX_VM_STACK_DEPTH"))
            {
                __debugbreak();
            }
            gScrVmPub[inst].function_frame->fs.pos = *bufa;
            ++gScrVmPub[inst].function_count;
            ++gScrVmPub[inst].function_frame;
        }
        else
        {
            top->u.intValue = (int)*bufa;
        }
        buf = (char *)(bufa + 1);
    }
    gFs[inst].pos = stackValue->pos;
    gFs[inst].top = top;
    localId = stackValue->localId;
    gFs[inst].localId = localId;
    Scr_ClearWaitTime(inst, startLocalId);
    if (gScrVmPub[inst].function_count >= 32
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            3453,
            0,
            "%s",
            "gScrVmPub[inst].function_count < MAX_VM_STACK_DEPTH"))
    {
        __debugbreak();
    }
    function_count = gScrVmPub[inst].function_count;
    while (1)
    {
        gScrVmPub[inst].function_frame_start[function_count--].fs.localId = localId;
        if (!function_count)
            break;
        localId = GetParentLocalId(inst, localId);
    }
    while (++function_count != gScrVmPub[inst].function_count)
        gScrVmPub[inst].function_frame_start[function_count].fs.localVarCount = Scr_AddLocalVars(
            inst,
            gScrVmPub[inst].function_frame_start[function_count].fs.localId);
    gFs[inst].localVarCount = Scr_AddLocalVars(inst, gFs[inst].localId);
    if (stackValue->time != LOBYTE(gScrVarPub[inst].time))
        Scr_ResetTimeout(inst);
    --gScrVarPub[inst].numScriptThreads;
    MT_Free((uint8*)stackValue, stackValue->bufLen, inst);
    if (gScrVmPub[inst].stack[0].type != 7
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            3482,
            0,
            "%s",
            "gScrVmPub[inst].stack[0].type == VAR_CODEPOS"))
    {
        __debugbreak();
    }
}

int __cdecl Scr_AddLocalVars(scriptInstance_t inst, unsigned int localId)
{
    int localVarCount; // [esp+1Ch] [ebp-8h]
    unsigned int fieldIndex; // [esp+20h] [ebp-4h]

    localVarCount = 0;
    for ( fieldIndex = FindLastSibling(inst, localId);
                fieldIndex;
                fieldIndex = gScrVarGlob[inst].variableList[fieldIndex + 0x8000].hash.u.prev )
    {
        *++gScrVmPub[inst].localVars = gScrVarGlob[inst].variableList[fieldIndex + 0x8000].hash.id;
        ++localVarCount;
    }
    return localVarCount;
}

void __cdecl Scr_ResetTimeout(scriptInstance_t inst)
{
    DWORD v1; // eax

    gScrVmGlob[inst].starttime = Sys_Milliseconds();
    if (!logScriptTimes
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5847,
            0,
            "%s",
            "logScriptTimes"))
    {
        __debugbreak();
    }
    if (logScriptTimes->current.enabled)
    {
        v1 = Sys_Milliseconds();
        Com_Printf(24, "RESET TIME: %d\n", v1);
    }
    memset((unsigned __int8 *)gScrVmDebugPub[inst].jumpbackHistory, 0, sizeof(gScrVmDebugPub[inst].jumpbackHistory));
}

bool __cdecl Scr_IsStackClear(scriptInstance_t inst)
{
    return gScrVmPub[inst].top == gScrVmPub[inst].stack;
}

void __cdecl Scr_StackClear(scriptInstance_t inst)
{
    gScrVmPub[inst].top = gScrVmPub[inst].stack;
}

unsigned int __cdecl VM_Execute_0(scriptInstance_t inst)
{
    int v1; // ecx
    unsigned int Self; // eax
    VariableValue v3; // rax
    const char *v4; // eax
    const char *v5; // eax
    VariableUnion v6; // eax
    unsigned int Variable; // eax
    VariableValue VariableField; // rax
    unsigned int v9; // eax
    unsigned int v10; // eax
    const char *v11; // eax
    const char *v12; // eax
    char v13; // fl
    bool v14; // cf
    bool v15; // zf
    char v16; // sf
    char v17; // of
    char v18; // pf
    const char *v19; // eax
    unsigned int v20; // eax
    unsigned int v21; // eax
    const char *v22; // eax
    const char *v23; // eax
    unsigned int PrimDrawSurfInt; // eax
    unsigned int v25; // eax
    const char *v26; // eax
    unsigned int v27; // eax
    unsigned int v28; // eax
    const char *v29; // eax
    unsigned int LocalVar; // eax
    DWORD v31; // eax
    const char *v32; // eax
    const char *v33; // eax
    unsigned int v34; // eax
    unsigned int Array; // eax
    unsigned int v36; // eax
    unsigned int v37; // eax
    unsigned int ObjectVariable; // eax
    unsigned int v39; // eax
    unsigned int NewObjectVariable; // eax
    unsigned int v41; // eax
    unsigned int v42; // eax
    unsigned int v43; // eax
    unsigned int v44; // eax
    unsigned int v45; // eax
    unsigned int v46; // eax
    unsigned int v47; // eax
    const char *v48; // eax
    const char *v49; // eax
    unsigned int EntityId; // eax
    unsigned __int16 UnsignedShort; // ax
    unsigned __int16 v53; // ax
    unsigned int v54; // eax
    unsigned __int16 v55; // ax
    const char *v56; // eax
    const char *v57; // eax
    unsigned int localId; // [esp-8h] [ebp-344h]
    unsigned int v59; // [esp-8h] [ebp-344h]
    unsigned int v60; // [esp-8h] [ebp-344h]
    VariableValue *top; // [esp-4h] [ebp-340h]
    VariableValue *v62; // [esp-4h] [ebp-340h]
    //int v63; // [esp-4h] [ebp-340h]
    unsigned int v64; // [esp-4h] [ebp-340h]
    unsigned int v65; // [esp-4h] [ebp-340h]
    int v66; // [esp+8h] [ebp-334h]
    function_frame_t *function_frame; // [esp+18h] [ebp-324h]
    const char *v68; // [esp+1Ch] [ebp-320h]
    unsigned __int16 v69; // [esp+44h] [ebp-2F8h]
    unsigned __int16 v70; // [esp+54h] [ebp-2E8h]
    unsigned __int16 v71; // [esp+80h] [ebp-2BCh]
    unsigned __int16 v72; // [esp+90h] [ebp-2ACh]
    unsigned __int16 v73; // [esp+A0h] [ebp-29Ch]
    unsigned __int16 v74; // [esp+E4h] [ebp-258h]
    unsigned __int16 v75; // [esp+E8h] [ebp-254h]
    unsigned __int16 v76; // [esp+ECh] [ebp-250h]
    unsigned __int16 v77; // [esp+F0h] [ebp-24Ch]
    unsigned __int16 v78; // [esp+10Ch] [ebp-230h]
    unsigned __int16 v79; // [esp+1D0h] [ebp-16Ch]
    VariableUnion v80; // [esp+1D4h] [ebp-168h]
    VariableUnion v81; // [esp+1DCh] [ebp-160h]
    const char *pos; // [esp+200h] [ebp-13Ch]
    unsigned __int16 v83; // [esp+204h] [ebp-138h]
    unsigned __int16 v84; // [esp+208h] [ebp-134h]
    VariableUnion v85; // [esp+20Ch] [ebp-130h]
    VariableUnion v86; // [esp+210h] [ebp-12Ch]
    unsigned __int16 v87; // [esp+214h] [ebp-128h]
    unsigned __int16 v88; // [esp+218h] [ebp-124h]
    VariableValue v89; // [esp+22Ch] [ebp-110h]
    VariableValue v90; // [esp+234h] [ebp-108h]
    VariableValue v91; // [esp+23Ch] [ebp-100h]
    scr_entref_t v92; // [esp+244h] [ebp-F8h] BYREF
    scr_entref_t v93; // [esp+24Eh] [ebp-EEh]
    VariableValue v94; // [esp+254h] [ebp-E8h]
    VariableValue v95; // [esp+25Ch] [ebp-E0h]
    VariableValue v96; // [esp+264h] [ebp-D8h]
    VariableValue v97; // [esp+26Ch] [ebp-D0h]
    VariableValue v98; // [esp+274h] [ebp-C8h]
    VariableValue v99; // [esp+27Ch] [ebp-C0h]
    VariableValue v100; // [esp+284h] [ebp-B8h]
    VariableValue v101; // [esp+28Ch] [ebp-B0h]
    VariableValue v102; // [esp+294h] [ebp-A8h]
    VariableValue v103; // [esp+29Ch] [ebp-A0h]
    VariableValue v104; // [esp+2A4h] [ebp-98h]
    unsigned int v105; // [esp+2ACh] [ebp-90h]
    unsigned int outparamcount; // [esp+2B0h] [ebp-8Ch]
    scr_entref_t entref; // [esp+2B4h] [ebp-88h]
    unsigned int classnum; // [esp+2BCh] [ebp-80h]
    VariableValue stackValue; // [esp+2C0h] [ebp-7Ch] BYREF
    unsigned int builtinIndex; // [esp+2C8h] [ebp-74h]
    unsigned int stringValue; // [esp+2CCh] [ebp-70h]
    const char *currentCodePos; // [esp+2D0h] [ebp-6Ch]
    const char *tempCodePos; // [esp+2D4h] [ebp-68h]
    unsigned int stackId; // [esp+2D8h] [ebp-64h]
    unsigned int fieldValueId; // [esp+2DCh] [ebp-60h]
    int waitTime; // [esp+2E0h] [ebp-5Ch]
    int jumpOffset; // [esp+2E4h] [ebp-58h]
    function_stack_t localFs; // [esp+2E8h] [ebp-54h] BYREF
    unsigned int objectId; // [esp+2FCh] [ebp-40h]
    unsigned int caseValue; // [esp+300h] [ebp-3Ch]
    unsigned int fieldName; // [esp+304h] [ebp-38h]
    unsigned int threadId; // [esp+308h] [ebp-34h]
    unsigned int currentCaseValue; // [esp+30Ch] [ebp-30h]
    unsigned int parentLocalId; // [esp+310h] [ebp-2Ch]
    unsigned int paramcount; // [esp+314h] [ebp-28h]
    int type; // [esp+318h] [ebp-24h]
    int entnum; // [esp+31Ch] [ebp-20h]
    unsigned int fieldValueIndex; // [esp+320h] [ebp-1Ch]
    const char *debugpos; // [esp+324h] [ebp-18h]
    unsigned int selfId; // [esp+328h] [ebp-14h]
    VariableValue tempValue; // [esp+32Ch] [ebp-10h] BYREF
    unsigned int id; // [esp+334h] [ebp-8h]
    unsigned __int8 removeCount; // [esp+33Bh] [ebp-1h]

    ++g_script_error_level[inst];
    localFs = gFs[inst];
    while (1)
    {
        if ((unsigned int)g_script_error_level[inst] >= 0x21
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                869,
                0,
                "g_script_error_level[inst] doesn't index ARRAY_COUNT( g_script_error[inst] )\n\t%i not in [0, %i)",
                g_script_error_level[inst],
                33))
        {
            __debugbreak();
        }
        if (!_setjmp(g_script_error[inst][g_script_error_level[inst]]))
            break;
    error_1:
        Monkey_GrabComPrints(1);
        Com_PrintMessage(6, (char*)"caught script exception\n", 0);
        switch (gOpcode[inst])
        {
        case 33:
        case 34:
        case 35:
        case 36:
        case 132:
            if (gScrVarPub[inst].error_index < -1
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    2621,
                    0,
                    "%s",
                    "gScrVarPub[inst].error_index >= -1"))
            {
                __debugbreak();
            }
            if (gScrVarPub[inst].error_index < 0)
                gScrVarPub[inst].error_index = 1;
            break;
        case 41:
        case 42:
        case 47:
        case 57:
        case 59:
        case 100:
        case 101:
            gScrVarPub[inst].error_index = 0;
            break;
        case 62:
        case 63:
        case 64:
        case 65:
        case 66:
        case 67:
        case 68:
            if (gScrVarPub[inst].error_index < 0
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    2643,
                    0,
                    "%s",
                    "gScrVarPub[inst].error_index >= 0"))
            {
                __debugbreak();
            }
            if (gScrVarPub[inst].error_index > 0)
                gScrVarPub[inst].error_index = gScrVmPub[inst].outparamcount + 1 - gScrVarPub[inst].error_index;
            break;
        case 69:
        case 70:
        case 71:
        case 72:
        case 73:
        case 74:
        case 75:
            if (gScrVarPub[inst].error_index < -1
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    2657,
                    0,
                    "%s",
                    "gScrVarPub[inst].error_index >= -1"))
            {
                __debugbreak();
            }
            if (gScrVarPub[inst].error_index <= 0)
            {
                if (gScrVarPub[inst].error_index < 0)
                    gScrVarPub[inst].error_index = 1;
            }
            else
            {
                gScrVarPub[inst].error_index = gScrVmPub[inst].outparamcount + 2 - gScrVarPub[inst].error_index;
            }
            break;
        default:
            break;
        }
        RuntimeError(
            inst,
            (char *)localFs.pos,
            gScrVarPub[inst].error_index,
            gScrVarPub[inst].error_message,
            gScrVmGlob[inst].dialog_error_message);
        Scr_ClearErrorMessage(inst);
        switch (gOpcode[inst])
        {
        case OP_EvalLocalArrayCached:
        case OP_EvalArray:
            RemoveRefToValue(inst, localFs.top--);
            RemoveRefToValue(inst, localFs.top);
            localFs.top->type = 0;
            break;
        case OP_EvalLocalArrayRefCached0:
        case OP_EvalLocalArrayRefCached:
        case OP_EvalArrayRef:
        case OP_EvalLocalVariableRef:
            fieldValueIndex = 0;
            fieldValueId = GetDummyFieldValue(inst);
            goto $LN25_31;
        case OP_ClearArray:
        case OP_wait:
            goto $LN25_31;
        case OP_GetSelfObject:
            goto $LN38_13;
        case OP_EvalSelfFieldVariable:
        case OP_EvalFieldVariable:
            localFs.top->type = 0;
            break;
        case OP_EvalSelfFieldVariableRef:
        case OP_EvalFieldVariableRef:
            fieldValueIndex = 0;
            fieldValueId = GetDummyFieldValue(inst);
            break;
        case OP_ClearFieldVariable:
            if (gScrVmPub[inst].outparamcount)
            {
                if (gScrVmPub[inst].outparamcount != 1
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        2709,
                        0,
                        "%s",
                        "gScrVmPub[inst].outparamcount == 1"))
                {
                    __debugbreak();
                }
                if (gScrVmPub[inst].top->type
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        2711,
                        0,
                        "%s",
                        "gScrVmPub[inst].top->type == VAR_UNDEFINED"))
                {
                    __debugbreak();
                }
                gScrVmPub[inst].outparamcount = 0;
            }
            break;
        case OP_checkclearparams:
            if (localFs.top->type == 7
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    2801,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top->type != VAR_CODEPOS)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            while (localFs.top->type != 8)
            {
                RemoveRefToValue(inst, localFs.top--);
                if (localFs.top->type == 7
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        2806,
                        0,
                        "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                        "(localFs.top->type != VAR_CODEPOS)",
                        localFs.top - gScrVmPub[inst].stack))
                {
                    __debugbreak();
                }
            }
            localFs.top->type = 7;
            break;
        case OP_SetVariableField:
            if (gScrVmPub[inst].outparamcount)
            {
                if (gScrVmPub[inst].outparamcount != 1
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        2724,
                        0,
                        "%s",
                        "gScrVmPub[inst].outparamcount == 1"))
                {
                    __debugbreak();
                }
                if (gScrVmPub[inst].top != localFs.top
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        2726,
                        0,
                        "%s",
                        "gScrVmPub[inst].top == localFs.top"))
                {
                    __debugbreak();
                }
                RemoveRefToValue(inst, localFs.top);
                gScrVmPub[inst].outparamcount = 0;
            }
            goto error_dec_top;
        case OP_SetSelfFieldVariableField:
            RemoveRefToValue(inst, localFs.top);
            gScrVmPub[inst].outparamcount = 0;
            goto error_dec_top;
        case OP_CallBuiltin0:
        case OP_CallBuiltin1:
        case OP_CallBuiltin2:
        case OP_CallBuiltin3:
        case OP_CallBuiltin4:
        case OP_CallBuiltin5:
        case OP_CallBuiltin:
        case OP_CallBuiltinMethod0:
        case OP_CallBuiltinMethod1:
        case OP_CallBuiltinMethod2:
        case OP_CallBuiltinMethod3:
        case OP_CallBuiltinMethod4:
        case OP_CallBuiltinMethod5:
        case OP_CallBuiltinMethod:
            Scr_ClearOutParams(inst);
            localFs.top = gScrVmPub[inst].top + 1;
            localFs.top->type = 0;
            break;
        case OP_ScriptFunctionCall2:
        case OP_ScriptFunctionCall:
        case OP_ScriptMethodCall:
            Scr_ReadCodePos(inst, &localFs.pos);
            goto $LN18_47;
        case OP_ScriptFunctionCallPointer:
        case OP_ScriptMethodCallPointer:
        $LN18_47:
            if (localFs.top->type == 7
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    2819,
                    0,
                    "%s",
                    "localFs.top->type != VAR_CODEPOS"))
            {
                __debugbreak();
            }
            while (localFs.top->type != 8)
            {
                RemoveRefToValue(inst, localFs.top--);
                if (localFs.top->type == 7
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        2824,
                        0,
                        "%s",
                        "localFs.top->type != VAR_CODEPOS"))
                {
                    __debugbreak();
                }
            }
            localFs.top->type = 0;
            break;
        case OP_ScriptThreadCall:
        case OP_ScriptMethodThreadCall:
            Scr_ReadCodePos(inst, &localFs.pos);
            goto $LN14_59;
        case OP_ScriptThreadCallPointer:
        case OP_ScriptMethodThreadCallPointer:
        $LN14_59:
            for (paramcount = R_ReadPrimDrawSurfInt((GfxReadCmdBuf *)&localFs); paramcount; --paramcount)
                RemoveRefToValue(inst, localFs.top--);
            localFs.top[1].type = 0;
            ++localFs.top;
            break;
        case OP_CastFieldObject:
            objectId = GetDummyObject(inst);
            goto error_dec_top;
        case OP_EvalLocalVariableObjectCached:
            ++localFs.pos;
        $LN38_13:
            objectId = GetDummyObject(inst);
            break;
        case OP_JumpOnFalse:
        case OP_JumpOnTrue:
        case OP_JumpOnFalseExpr:
        case OP_JumpOnTrueExpr:
            Scr_ReadUnsignedShort(&localFs.pos);
            goto error_dec_top;
        case OP_jumpback:
            jumpOffset = Scr_ReadUnsignedShort(&localFs.pos);
            localFs.pos -= jumpOffset;
            break;
        case OP_bit_or:
        case OP_bit_ex_or:
        case OP_bit_and:
        case OP_equality:
        case OP_inequality:
        case OP_less:
        case OP_greater:
        case OP_less_equal:
        case OP_greater_equal:
        case OP_shift_left:
        case OP_shift_right:
        case OP_plus:
        case OP_minus:
        case OP_multiply:
        case OP_divide:
        case OP_mod:
            goto error_dec_top;
        case OP_waittillmatch:
            ++localFs.pos;
            goto $LN26_29;
        case OP_waittill:
        case OP_endon:
        $LN26_29:
            RemoveRefToValue(inst, localFs.top--);
            goto $LN25_31;
        case OP_notify:
            if (localFs.top->type == 7
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    2855,
                    0,
                    "%s",
                    "localFs.top->type != VAR_CODEPOS"))
            {
                __debugbreak();
            }
            while (localFs.top->type != 8)
            {
                RemoveRefToValue(inst, localFs.top--);
                if (localFs.top->type == 7
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        2860,
                        0,
                        "%s",
                        "localFs.top->type != VAR_CODEPOS"))
                {
                    __debugbreak();
                }
            }
            goto $LN25_31;
        case OP_switch:
            if (gCaseCount[inst])
            {
                do
                {
                    currentCaseValue = R_ReadPrimDrawSurfInt((GfxReadCmdBuf *)&localFs);
                    currentCodePos = Scr_ReadCodePos(inst, &localFs.pos);
                    --gCaseCount[inst];
                } while (gCaseCount[inst]);
                if (!currentCaseValue)
                {
                    localFs.pos = currentCodePos;
                    if (!currentCodePos
                        && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                            2877,
                            0,
                            "%s",
                            "localFs.pos"))
                    {
                        __debugbreak();
                    }
                }
            }
        $LN25_31:
            RemoveRefToValue(inst, localFs.top);
        error_dec_top:
            --localFs.top;
            break;
        default:
            break;
        }
        if (gScrVmPub[inst].inparamcount
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                2884,
                0,
                "%s",
                "!gScrVmPub[inst].inparamcount"))
        {
            __debugbreak();
        }
        if (gScrVmPub[inst].outparamcount
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                2885,
                0,
                "%s",
                "!gScrVmPub[inst].outparamcount"))
        {
            __debugbreak();
        }
        Monkey_Error(0);
        if (gScrVmPub[inst].showError && !gScrVmPub[inst].debugCode && !Scr_IgnoreErrors(inst))
        {
            gScrVmPub[inst].showError = 0;
            //BG_EvalVehicleName();
            Scr_HitBreakpoint(inst, localFs.top, localFs.pos, localFs.localId, 0);
        }
    }
    while (2)
    {
        if (gScrVarPub[inst].error_message
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                878,
                0,
                "%s\n\t(gScrVarPub[inst].error_message) = %s",
                "(!gScrVarPub[inst].error_message)",
                gScrVarPub[inst].error_message))
        {
            __debugbreak();
        }
        if (gScrVarPub[inst].error_index
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                879,
                0,
                "%s",
                "!gScrVarPub[inst].error_index"))
        {
            __debugbreak();
        }
        if (gScrVmPub[inst].outparamcount
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                880,
                0,
                "%s",
                "!gScrVmPub[inst].outparamcount"))
        {
            __debugbreak();
        }
        if (gScrVmPub[inst].inparamcount
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                881,
                0,
                "%s",
                "!gScrVmPub[inst].inparamcount"))
        {
            __debugbreak();
        }
        gOpcode[inst] = (unsigned __int8)*localFs.pos++;
    interrupt_return:
        gScrVarPub[inst].varUsagePos = localFs.pos;
        switch (gOpcode[inst])
        {
        case 0:
            parentLocalId = GetSafeParentLocalId(inst, localFs.localId);
            Scr_KillThread(inst, localFs.localId);
            gScrVmPub[inst].localVars -= localFs.localVarCount;
            if (localFs.top->type == 8
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    918,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top->type != VAR_PRECODEPOS)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            while (localFs.top->type != 7)
            {
                RemoveRefToValue(inst, localFs.top->type, localFs.top->u);
                --localFs.top;
                if (localFs.top->type == 8
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        923,
                        0,
                        "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                        "(localFs.top->type != VAR_PRECODEPOS)",
                        localFs.top - gScrVmPub[inst].stack))
                {
                    __debugbreak();
                }
            }
            --gScrVmPub[inst].function_count;
            --gScrVmPub[inst].function_frame;
            if (!parentLocalId)
            {
                if (localFs.top != localFs.startTop
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        930,
                        0,
                        "%s\n\t(localFs.top - localFs.startTop) = %i",
                        "(localFs.top == localFs.startTop)",
                        localFs.top - localFs.startTop))
                {
                    __debugbreak();
                }
            thread_end:
                localFs.startTop[1].type = 0;
                goto thread_return;
            }
            if (localFs.top->type != 7
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    934,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top->type == VAR_CODEPOS)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            localFs.top->type = 0;
            goto end;
        case 1:
            parentLocalId = GetSafeParentLocalId(inst, localFs.localId);
            Scr_KillThread(inst, localFs.localId);
            gScrVmPub[inst].localVars -= localFs.localVarCount;
            v1 = localFs.top->type;
            tempValue.u.intValue = localFs.top->u.intValue;
            tempValue.type = v1;
            --localFs.top;
            if (localFs.top->type == 8
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    961,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top->type != VAR_PRECODEPOS)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            while (localFs.top->type != 7)
            {
                RemoveRefToValue(inst, localFs.top->type, localFs.top->u);
                --localFs.top;
                if (localFs.top->type == 8
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        966,
                        0,
                        "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                        "(localFs.top->type != VAR_PRECODEPOS)",
                        localFs.top - gScrVmPub[inst].stack))
                {
                    __debugbreak();
                }
            }
            --gScrVmPub[inst].function_count;
            --gScrVmPub[inst].function_frame;
            if (parentLocalId)
            {
                if (localFs.top->type != 7
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        978,
                        0,
                        "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                        "(localFs.top->type == VAR_CODEPOS)",
                        localFs.top - gScrVmPub[inst].stack))
                {
                    __debugbreak();
                }
                *localFs.top = tempValue;
            end:
                if (localFs.top == localFs.startTop
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        937,
                        0,
                        "%s",
                        "localFs.top != localFs.startTop"))
                {
                    __debugbreak();
                }
                RemoveRefToObject(inst, localFs.localId);
                localFs.pos = gScrVmPub[inst].function_frame->fs.pos;
                if (!localFs.pos
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        949,
                        0,
                        "%s",
                        "localFs.pos"))
                {
                    __debugbreak();
                }
                localFs.localVarCount = gScrVmPub[inst].function_frame->fs.localVarCount;
                localFs.localId = parentLocalId;
                continue;
            }
            if (localFs.top != localFs.startTop
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    973,
                    0,
                    "%s\n\t(localFs.top - localFs.startTop) = %i",
                    "(localFs.top == localFs.startTop)",
                    localFs.top - localFs.startTop))
            {
                __debugbreak();
            }
            localFs.top[1] = tempValue;
        thread_return:
            if (gThreadCount[inst])
            {
                --gThreadCount[inst];
                RemoveRefToObject(inst, localFs.localId);
                localFs = gScrVmPub[inst].function_frame->fs;
                localFs.top->type = gScrVmPub[inst].function_frame->topType;
                ++localFs.top;
                continue;
            }
            if (g_script_error_level[inst] < 0
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    2511,
                    0,
                    "%s\n\t(g_script_error_level[inst]) = %i",
                    "(g_script_error_level[inst] >= 0)",
                    g_script_error_level[inst]))
            {
                __debugbreak();
            }
            --g_script_error_level[inst];
            gFs[inst] = localFs;
            return localFs.localId;
        case 2:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    983,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top >= gScrVmPub[inst].stack)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    984,
                    0,
                    "%s\n\t(localFs.top+1 - gScrVmPub[inst].stack) = %i",
                    "(localFs.top+1 <= gScrVmPub[inst].maxstack)",
                    &localFs.top[1] - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            localFs.top[1].type = 0;
            ++localFs.top;
            continue;
        case 3:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    990,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top >= gScrVmPub[inst].stack)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    991,
                    0,
                    "%s\n\t(localFs.top+1 - gScrVmPub[inst].stack) = %i",
                    "(localFs.top+1 <= gScrVmPub[inst].maxstack)",
                    &localFs.top[1] - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            localFs.top[1].type = 6;
            localFs.top[1].u.intValue = 0;
            ++localFs.top;
            continue;
        case 4:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    998,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top >= gScrVmPub[inst].stack)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    999,
                    0,
                    "%s\n\t(localFs.top+1 - gScrVmPub[inst].stack) = %i",
                    "(localFs.top+1 <= gScrVmPub[inst].maxstack)",
                    &localFs.top[1] - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            localFs.top[1].type = 6;
            localFs.top[1].u.intValue = (unsigned __int8)*localFs.pos;
            ++localFs.top;
            ++localFs.pos;
            continue;
        case 5:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1007,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top >= gScrVmPub[inst].stack)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1008,
                    0,
                    "%s\n\t(localFs.top+1 - gScrVmPub[inst].stack) = %i",
                    "(localFs.top+1 <= gScrVmPub[inst].maxstack)",
                    &localFs.top[1] - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            localFs.top[1].type = 6;
            localFs.top[1].u.intValue = -(unsigned __int8)*localFs.pos;
            ++localFs.top;
            ++localFs.pos;
            continue;
        case 6:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1016,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top >= gScrVmPub[inst].stack)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1017,
                    0,
                    "%s\n\t(localFs.top+1 - gScrVmPub[inst].stack) = %i",
                    "(localFs.top+1 <= gScrVmPub[inst].maxstack)",
                    &localFs.top[1] - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            localFs.top[1].type = 6;
            v88 = *(_WORD *)localFs.pos;
            localFs.pos += 2;
            localFs.top[1].u.intValue = v88;
            ++localFs.top;
            continue;
        case 7:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1024,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top >= gScrVmPub[inst].stack)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1025,
                    0,
                    "%s\n\t(localFs.top+1 - gScrVmPub[inst].stack) = %i",
                    "(localFs.top+1 <= gScrVmPub[inst].maxstack)",
                    &localFs.top[1] - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            localFs.top[1].type = 6;
            v87 = *(_WORD *)localFs.pos;
            localFs.pos += 2;
            localFs.top[1].u.intValue = -v87;
            ++localFs.top;
            continue;
        case 8:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1032,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top >= gScrVmPub[inst].stack)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1033,
                    0,
                    "%s\n\t(localFs.top+1 - gScrVmPub[inst].stack) = %i",
                    "(localFs.top+1 <= gScrVmPub[inst].maxstack)",
                    &localFs.top[1] - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            localFs.top[1].type = 6;
            v86.intValue = *(int *)localFs.pos;
            localFs.pos += 4;
            localFs.top[1].u = v86;
            ++localFs.top;
            continue;
        case 9:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1040,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top >= gScrVmPub[inst].stack)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1041,
                    0,
                    "%s\n\t(localFs.top+1 - gScrVmPub[inst].stack) = %i",
                    "(localFs.top+1 <= gScrVmPub[inst].maxstack)",
                    &localFs.top[1] - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            localFs.top[1].type = 5;
            v85.intValue = *(int *)localFs.pos;
            localFs.pos += 4;
            localFs.top[1].u = v85;
            ++localFs.top;
            continue;
        case 10:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1048,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top >= gScrVmPub[inst].stack)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1049,
                    0,
                    "%s\n\t(localFs.top+1 - gScrVmPub[inst].stack) = %i",
                    "(localFs.top+1 <= gScrVmPub[inst].maxstack)",
                    &localFs.top[1] - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            localFs.top[1].type = 2;
            v84 = *(_WORD *)localFs.pos;
            localFs.pos += 2;
            localFs.top[1].u.intValue = v84;
            SL_AddRefToString(localFs.top[1].u.stringValue, inst);
            ++localFs.top;
            continue;
        case 11:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1057,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top >= gScrVmPub[inst].stack)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1058,
                    0,
                    "%s\n\t(localFs.top+1 - gScrVmPub[inst].stack) = %i",
                    "(localFs.top+1 <= gScrVmPub[inst].maxstack)",
                    &localFs.top[1] - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            localFs.top[1].type = 3;
            v83 = *(_WORD *)localFs.pos;
            localFs.pos += 2;
            localFs.top[1].u.intValue = v83;
            SL_AddRefToString(localFs.top[1].u.stringValue, inst);
            ++localFs.top;
            continue;
        case 12:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1066,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top >= gScrVmPub[inst].stack)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1067,
                    0,
                    "%s\n\t(localFs.top+1 - gScrVmPub[inst].stack) = %i",
                    "(localFs.top+1 <= gScrVmPub[inst].maxstack)",
                    &localFs.top[1] - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            localFs.top[1].type = 4;
            pos = localFs.pos;
            localFs.pos += 12;
            localFs.top[1].u.intValue = (int)pos;
            ++localFs.top;
            continue;
        case 13:
            objectId = gScrVarPub[inst].levelId;
            continue;
        case 14:
            objectId = gScrVarPub[inst].animId;
            continue;
        case 15:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1082,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top >= gScrVmPub[inst].stack)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1083,
                    0,
                    "%s\n\t(localFs.top+1 - gScrVmPub[inst].stack) = %i",
                    "(localFs.top+1 <= gScrVmPub[inst].maxstack)",
                    &localFs.top[1] - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            localFs.top[1].type = 1;
            Self = Scr_GetSelf(inst, localFs.localId);
            localFs.top[1].u.intValue = Self;
            AddRefToObject(inst, localFs.top[1].u.stringValue);
            ++localFs.top;
            continue;
        case 16:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1091,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top >= gScrVmPub[inst].stack)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1092,
                    0,
                    "%s\n\t(localFs.top+1 - gScrVmPub[inst].stack) = %i",
                    "(localFs.top+1 <= gScrVmPub[inst].maxstack)",
                    &localFs.top[1] - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            localFs.top[1].type = 1;
            localFs.top[1].u.intValue = gScrVarPub[inst].levelId;
            ++localFs.top;
            AddRefToObject(inst, gScrVarPub[inst].levelId);
            continue;
        case 17:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1100,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top >= gScrVmPub[inst].stack)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1101,
                    0,
                    "%s\n\t(localFs.top+1 - gScrVmPub[inst].stack) = %i",
                    "(localFs.top+1 <= gScrVmPub[inst].maxstack)",
                    &localFs.top[1] - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            v104 = Scr_EvalVariable(inst, gScrVarPub[inst].gameId);
            localFs.top[1] = v104;
            ++localFs.top;
            continue;
        case 18:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1107,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top >= gScrVmPub[inst].stack)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1108,
                    0,
                    "%s\n\t(localFs.top+1 - gScrVmPub[inst].stack) = %i",
                    "(localFs.top+1 <= gScrVmPub[inst].maxstack)",
                    &localFs.top[1] - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            localFs.top[1].type = 1;
            localFs.top[1].u.intValue = gScrVarPub[inst].animId;
            ++localFs.top;
            AddRefToObject(inst, gScrVarPub[inst].animId);
            continue;
        case 19:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1121,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top >= gScrVmPub[inst].stack)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1122,
                    0,
                    "%s\n\t(localFs.top+1 - gScrVmPub[inst].stack) = %i",
                    "(localFs.top+1 <= gScrVmPub[inst].maxstack)",
                    &localFs.top[1] - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            localFs.top[1].type = 11;
            v81.intValue = *(int *)localFs.pos;
            localFs.pos += 4;
            localFs.top[1].u = v81;
            ++localFs.top;
            continue;
        case 20:
            fieldValueIndex = 0;
            fieldValueId = gScrVarPub[inst].gameId;
            continue;
        case 21:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1134,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top >= gScrVmPub[inst].stack)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1135,
                    0,
                    "%s\n\t(localFs.top+1 - gScrVmPub[inst].stack) = %i",
                    "(localFs.top+1 <= gScrVmPub[inst].maxstack)",
                    &localFs.top[1] - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            localFs.top[1].type = 9;
            v80.intValue = *(int *)localFs.pos;
            localFs.pos += 4;
            localFs.top[1].u = v80;
            ++localFs.top;
            continue;
        case 22:
            ++gScrVmPub[inst].localVars;
            ++localFs.localVarCount;
            v79 = *(_WORD *)localFs.pos;
            localFs.pos += 2;
            *gScrVmPub[inst].localVars = GetNewVariable(inst, localFs.localId, v79);
            continue;
        case 23:
            removeCount = *localFs.pos++;
            gScrVmPub[inst].localVars -= removeCount;
            localFs.localVarCount -= removeCount;
            while (removeCount)
            {
                RemoveNextVariable(inst, localFs.localId);
                --removeCount;
            }
            continue;
        case 24:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1160,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top >= gScrVmPub[inst].stack)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1161,
                    0,
                    "%s\n\t(localFs.top+1 - gScrVmPub[inst].stack) = %i",
                    "(localFs.top+1 <= gScrVmPub[inst].maxstack)",
                    &localFs.top[1] - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            v103 = Scr_EvalVariable(inst, *gScrVmPub[inst].localVars);
            localFs.top[1] = v103;
            ++localFs.top;
            continue;
        case 25:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1167,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top >= gScrVmPub[inst].stack)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1168,
                    0,
                    "%s\n\t(localFs.top+1 - gScrVmPub[inst].stack) = %i",
                    "(localFs.top+1 <= gScrVmPub[inst].maxstack)",
                    &localFs.top[1] - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            v102 = Scr_EvalVariable(inst, *(gScrVmPub[inst].localVars - 1));
            localFs.top[1] = v102;
            ++localFs.top;
            continue;
        case 26:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1174,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top >= gScrVmPub[inst].stack)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1175,
                    0,
                    "%s\n\t(localFs.top+1 - gScrVmPub[inst].stack) = %i",
                    "(localFs.top+1 <= gScrVmPub[inst].maxstack)",
                    &localFs.top[1] - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            v101 = Scr_EvalVariable(inst, *(gScrVmPub[inst].localVars - 2));
            localFs.top[1] = v101;
            ++localFs.top;
            continue;
        case 27:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1181,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top >= gScrVmPub[inst].stack)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1182,
                    0,
                    "%s\n\t(localFs.top+1 - gScrVmPub[inst].stack) = %i",
                    "(localFs.top+1 <= gScrVmPub[inst].maxstack)",
                    &localFs.top[1] - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            v100 = Scr_EvalVariable(inst, *(gScrVmPub[inst].localVars - 3));
            localFs.top[1] = v100;
            ++localFs.top;
            continue;
        case 28:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1188,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top >= gScrVmPub[inst].stack)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1189,
                    0,
                    "%s\n\t(localFs.top+1 - gScrVmPub[inst].stack) = %i",
                    "(localFs.top+1 <= gScrVmPub[inst].maxstack)",
                    &localFs.top[1] - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            v99 = Scr_EvalVariable(inst, *(gScrVmPub[inst].localVars - 4));
            localFs.top[1] = v99;
            ++localFs.top;
            continue;
        case 29:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1195,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top >= gScrVmPub[inst].stack)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1196,
                    0,
                    "%s\n\t(localFs.top+1 - gScrVmPub[inst].stack) = %i",
                    "(localFs.top+1 <= gScrVmPub[inst].maxstack)",
                    &localFs.top[1] - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            v98 = Scr_EvalVariable(inst, *(gScrVmPub[inst].localVars - 5));
            localFs.top[1] = v98;
            ++localFs.top;
            continue;
        case 30:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1202,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top >= gScrVmPub[inst].stack)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1203,
                    0,
                    "%s\n\t(localFs.top+1 - gScrVmPub[inst].stack) = %i",
                    "(localFs.top+1 <= gScrVmPub[inst].maxstack)",
                    &localFs.top[1] - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            v3 = Scr_EvalVariable(inst, gScrVmPub[inst].localVars[-(unsigned __int8)*localFs.pos]);
            v97 = v3;
            v3.type = (int)localFs.top;
            localFs.top[1].u.intValue = v3.u.intValue;
            *(_DWORD *)(v3.type + 12) = v97.type;
            ++localFs.top;
            ++localFs.pos;
            continue;
        case 31:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1210,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top >= gScrVmPub[inst].stack)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1211,
                    0,
                    "%s\n\t(localFs.top+1 - gScrVmPub[inst].stack) = %i",
                    "(localFs.top+1 <= gScrVmPub[inst].maxstack)",
                    &localFs.top[1] - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            v96 = Scr_EvalVariable(inst, gScrVmPub[inst].localVars[-(unsigned __int8)*localFs.pos]);
            localFs.top[1] = v96;
            ++localFs.top;
            ++localFs.pos;
            goto $LN290;
        case 32:
        $LN290:
            Scr_EvalArray(inst, localFs.top, localFs.top - 1);
            goto loop_dec_top;
        case 33:
            fieldValueId = *gScrVmPub[inst].localVars;
            goto $LN286;
        case 34:
            fieldValueId = gScrVmPub[inst].localVars[-(unsigned __int8)*localFs.pos++];
            goto $LN286;
        case 35:
        $LN286:
            objectId = Scr_EvalArrayRef(inst, fieldValueId);
            if (localFs.top->type == 6)
            {
                if (!IsValidArrayIndex(inst, localFs.top->u.stringValue))
                {
                    gFs[inst] = localFs;
                    v4 = va("array index %d out of range", localFs.top->u.intValue);
                    Scr_Error(inst, v4, 0);
                }
                fieldValueIndex = GetArrayVariableIndex(inst, objectId, localFs.top->u.stringValue);
            }
            else if (localFs.top->type == 2)
            {
                fieldValueIndex = GetVariableIndexInternal(inst, objectId, localFs.top->u.stringValue);
                SL_RemoveRefToString(inst, localFs.top->u.stringValue);
            }
            else
            {
                gFs[inst] = localFs;
                v5 = va("%s is not an array index", var_typename[localFs.top->type]);
                Scr_Error(inst, v5, 0);
            }
            fieldValueId = gScrVarGlob[inst].variableList[fieldValueIndex + 0x8000].hash.id;
            if (!fieldValueId
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1254,
                    0,
                    "%s",
                    "fieldValueId"))
            {
                __debugbreak();
            }
            goto loop_dec_top;
        case 36:
            ClearArray(inst, fieldValueId, localFs.top);
            goto loop_dec_top;
        case 37:
            localFs.top[1].type = 1;
            v6.intValue = Scr_AllocArray(inst);
            localFs.top[1].u = v6;
            ++localFs.top;
            continue;
        case 38:
            objectId = Scr_GetSelf(inst, localFs.localId);
            if (IsFieldObject(inst, objectId))
                continue;
            goto not_an_object;
        case 39:
            objectId = gScrVarPub[inst].levelId;
            goto EvalFieldVariable;
        case 40:
            objectId = gScrVarPub[inst].animId;
        EvalFieldVariable:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1278,
                    0,
                    "%s",
                    "localFs.top >= gScrVmPub[inst].stack"))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1279,
                    0,
                    "%s",
                    "localFs.top+1 <= gScrVmPub[inst].maxstack"))
            {
                __debugbreak();
            }
            v78 = *(_WORD *)localFs.pos;
            localFs.pos += 2;
            Variable = FindVariable(inst, objectId, v78);
            v95 = Scr_EvalVariable(inst, Variable);
            localFs.top[1] = v95;
            ++localFs.top;
            continue;
        case 41:
            objectId = Scr_GetSelf(inst, localFs.localId);
            if (IsFieldObject(inst, objectId))
                goto $LN272;
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1292,
                    0,
                    "%s",
                    "localFs.top >= gScrVmPub[inst].stack"))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1293,
                    0,
                    "%s",
                    "localFs.top+1 <= gScrVmPub[inst].maxstack"))
            {
                __debugbreak();
            }
            ++localFs.top;
            localFs.pos += 2;
        not_an_object:
            type = GetObjectType(inst, objectId);
            goto not_an_object_error;
        case 42:
        $LN272:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1301,
                    0,
                    "%s",
                    "localFs.top >= gScrVmPub[inst].stack"))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1302,
                    0,
                    "%s",
                    "localFs.top+1 <= gScrVmPub[inst].maxstack"))
            {
                __debugbreak();
            }
            v77 = *(_WORD *)localFs.pos;
            localFs.pos += 2;
            VariableField = Scr_FindVariableField(inst, objectId, v77);
            v94 = VariableField;
            VariableField.type = (int)localFs.top;
            localFs.top[1].u.intValue = VariableField.u.intValue;
            *(_DWORD *)(VariableField.type + 12) = v94.type;
            ++localFs.top;
            continue;
        case 43:
            objectId = gScrVarPub[inst].levelId;
            goto EvalFieldVariableRef;
        case 44:
            objectId = gScrVarPub[inst].animId;
            goto EvalFieldVariableRef;
        case 45:
            objectId = Scr_GetSelf(inst, localFs.localId);
            goto EvalFieldVariableRef;
        case 46:
        EvalFieldVariableRef:
            v76 = *(_WORD *)localFs.pos;
            localFs.pos += 2;
            fieldValueIndex = Scr_GetVariableFieldIndex(inst, objectId, v76);
            fieldValueId = gScrVarGlob[inst].variableList[fieldValueIndex + 0x8000].hash.id;
            continue;
        case 47:
            v75 = *(_WORD *)localFs.pos;
            localFs.pos += 2;
            ClearVariableField(inst, objectId, v75, localFs.top);
            continue;
        case 48:
            ++gScrVmPub[inst].localVars;
            ++localFs.localVarCount;
            v74 = *(_WORD *)localFs.pos;
            localFs.pos += 2;
            *gScrVmPub[inst].localVars = GetNewVariable(inst, localFs.localId, v74);
            goto $LN261_1;
        case 49:
        $LN261_1:
            if (localFs.top->type == 7
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1336,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top->type != VAR_CODEPOS)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (localFs.top->type != 8)
                goto LABEL_298;
            continue;
        case 50:
            if (localFs.top->type == 7
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1345,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top->type != VAR_CODEPOS)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (localFs.top->type != 8)
                goto LABEL_304;
            ++localFs.pos;
            continue;
        case 51:
            if (localFs.top->type == 8
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1356,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top->type != VAR_PRECODEPOS)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (localFs.top->type != 7)
                goto LABEL_304;
            ClearVariableValue(inst, gScrVmPub[inst].localVars[-(unsigned __int8)*localFs.pos++]);
            continue;
        case 52:
            if (localFs.top->type == 8
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1368,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top->type != VAR_PRECODEPOS)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            while (localFs.top->type != 7)
            {
                RemoveRefToValue(inst, localFs.top->type, localFs.top->u);
                --localFs.top;
                if (localFs.top->type == 8
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        1373,
                        0,
                        "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                        "(localFs.top->type != VAR_PRECODEPOS)",
                        localFs.top - gScrVmPub[inst].stack))
                {
                    __debugbreak();
                }
            }
            continue;
        case 53:
            if (localFs.top->type == 7
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1378,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top->type != VAR_CODEPOS)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            if (localFs.top->type == 8)
            {
                if (gScrVarPub[inst].numScriptValues > 0x3F37E || gScrVarPub[inst].numScriptObjects > 0x737E)
                {
                    gFs[inst] = localFs;
                    if (gScrVmPub[inst].showError)
                    {
                        Scr_DumpScriptThreads(inst);
                        Scr_DumpScriptVariablesDefault(inst);
                        Scr_Error(inst, "exceeded maximum number of script variables", 0);
                    }
                    Sys_Error((char*)"exceeded maximum number of script variables");
                }
                localFs.top->type = 7;
            }
            else
            {
                gFs[inst] = localFs;
                Scr_Error(inst, "function called with too many parameters", 0);
            $LN247:
                fieldValueIndex = 0;
                fieldValueId = *gScrVmPub[inst].localVars;
            }
            continue;
        case 54:
            goto $LN247;
        case 55:
            fieldValueIndex = 0;
            fieldValueId = gScrVmPub[inst].localVars[-(unsigned __int8)*localFs.pos++];
            continue;
        case 56:
            v73 = *(_WORD *)localFs.pos;
            localFs.pos += 2;
            top = localFs.top;
            v9 = GetVariable(inst, gScrVarPub[inst].levelId, v73);
            SetVariableValue(inst, v9, top);
            goto loop_dec_top;
        case 57:
            goto $LN1040;
        case 58:
            v71 = *(_WORD *)localFs.pos;
            localFs.pos += 2;
            v62 = localFs.top;
            v10 = GetVariable(inst, gScrVarPub[inst].animId, v71);
            SetVariableValue(inst, v10, v62);
            goto loop_dec_top;
        case 59:
            v72 = *(_WORD *)localFs.pos;
            localFs.pos += 2;
            fieldName = v72;
            objectId = Scr_GetSelf(inst, localFs.localId);
            fieldValueIndex = Scr_GetVariableFieldIndex(inst, objectId, fieldName);
            fieldValueId = gScrVarGlob[inst].variableList[fieldValueIndex + 0x8000].hash.id;
        $LN1040:
            if (fieldValueIndex)
            {
                if (!fieldValueId
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        1428,
                        0,
                        "%s",
                        "fieldValueId"))
                {
                    __debugbreak();
                }
                if (localFs.top->type)
                    SetVariableValue(inst, fieldValueId, localFs.top);
                else
                    RemoveVariableValue(inst, objectId, fieldValueIndex);
            }
            else
            {
                SetVariableFieldValue(inst, fieldValueId, localFs.top);
            }
            goto loop_dec_top;
        case 60:
        LABEL_298:
            SetVariableValue(inst, *gScrVmPub[inst].localVars, localFs.top);
            goto loop_dec_top;
        case 61:
        LABEL_304:
            SetVariableValue(inst, gScrVmPub[inst].localVars[-(unsigned __int8)*localFs.pos++], localFs.top);
            goto loop_dec_top;
        case 62:
        case 63:
        case 64:
        case 65:
        case 66:
        case 67:
            if (gScrVmPub[inst].outparamcount
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1462,
                    0,
                    "%s",
                    "!gScrVmPub[inst].outparamcount"))
            {
                __debugbreak();
            }
            gScrVmPub[inst].outparamcount = gOpcode[inst] - 62;
            goto CallBuiltin;
        case 68:
            if (gScrVmPub[inst].outparamcount
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1467,
                    0,
                    "%s",
                    "!gScrVmPub[inst].outparamcount"))
            {
                __debugbreak();
            }
            gScrVmPub[inst].outparamcount = (unsigned __int8)*localFs.pos++;
        CallBuiltin:
            if (gScrVmPub[inst].inparamcount
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1474,
                    0,
                    "%s",
                    "!gScrVmPub[inst].inparamcount"))
            {
                __debugbreak();
            }
            debugpos = localFs.pos;
            v70 = *(_WORD *)localFs.pos;
            localFs.pos += 2;
            builtinIndex = v70;
            gScrVmPub[inst].function_frame->fs.pos = localFs.pos;
            if (gScrVmDebugPub[inst].func_table[builtinIndex].breakpointCount)
            {
                outparamcount = gScrVmPub[inst].outparamcount;
                Scr_HitBuiltinBreakpoint(
                    inst,
                    localFs.top,
                    debugpos,
                    localFs.localId,
                    gOpcode[inst],
                    builtinIndex,
                    outparamcount);
                gScrVmPub[inst].outparamcount = outparamcount;
            }
            gScrVmPub[inst].top = localFs.top;
            ((void (*)(void))gScrCompilePub[inst].func_table[builtinIndex])();
            goto post_builtin;
        case 69:
        case 70:
        case 71:
        case 72:
        case 73:
        case 74:
            if (gScrVmPub[inst].outparamcount
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1565,
                    0,
                    "%s",
                    "!gScrVmPub[inst].outparamcount"))
            {
                __debugbreak();
            }
            gScrVmPub[inst].outparamcount = gOpcode[inst] - 69;
            goto CallBuiltinMethod;
        case 75:
            if (gScrVmPub[inst].outparamcount
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1570,
                    0,
                    "%s",
                    "!gScrVmPub[inst].outparamcount"))
            {
                __debugbreak();
            }
            gScrVmPub[inst].outparamcount = (unsigned __int8)*localFs.pos++;
        CallBuiltinMethod:
            if (gScrVmPub[inst].inparamcount
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1577,
                    0,
                    "%s",
                    "!gScrVmPub[inst].inparamcount"))
            {
                __debugbreak();
            }
            debugpos = localFs.pos;
            gScrVmPub[inst].top = localFs.top - 1;
            v69 = *(_WORD *)localFs.pos;
            localFs.pos += 2;
            builtinIndex = v69;
            if (localFs.top->type != 1)
                goto LABEL_395;
            objectId = localFs.top->u.stringValue;
            if (GetObjectType(inst, objectId) != 19)
            {
                type = GetObjectType(inst, objectId);
                RemoveRefToObject(inst, objectId);
                gScrVarPub[inst].error_index = -1;
                gFs[inst] = localFs;
                v11 = va("%s is not an entity", var_typename[type]);
                Scr_Error(inst, v11, 0);
            LABEL_395:
                type = localFs.top->type;
                RemoveRefToValue(inst, localFs.top->type, localFs.top->u);
                gScrVarPub[inst].error_index = -1;
                gFs[inst] = localFs;
                v12 = va("%s is not an entity", var_typename[type]);
                Scr_Error(inst, v12, 0);
            $LN221_0:
                if (!Scr_IsInOpcodeMemory(inst, localFs.pos)
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        1659,
                        0,
                        "%s",
                        "Scr_IsInOpcodeMemory( inst, localFs.pos )"))
                {
                    __debugbreak();
                }
                if (localFs.top->type == 5)
                {
                    if (*(float *)&localFs.top->u.intValue < 0.0)
                        goto negWait;
                    waitTime = (int)((float)((float)(inst != SCRIPTINSTANCE_SERVER ? 60 : 20)
                        * *(float *)&localFs.top->u.intValue)
                        + 9.313225746154785e-10);
                    if (!waitTime)
                    {
                        v14 = *(float *)&localFs.top->u.intValue < 0.0;
                        v18 = 0;
                        v15 = *(float *)&localFs.top->u.intValue == 0.0;
                        v16 = 0;
                        v17 = 0;
                        //waitTime = __SETP__(v13 & 0x44, 0);
                        waitTime = isnan(*(float *)&localFs.top->u.intValue) ? 1 : 0;
                    }
                }
                else if (localFs.top->type == 6)
                {
                    waitTime = localFs.top->u.intValue * (inst != SCRIPTINSTANCE_SERVER ? 60 : 20);
                }
                else
                {
                    gScrVarPub[inst].error_index = 2;
                    gFs[inst] = localFs;
                    v19 = va("type %s is not a float", var_typename[localFs.top->type]);
                    Scr_Error(inst, v19, 0);
                }
                if ((unsigned int)waitTime < 0xFFFFFF)
                {
                    if (waitTime)
                        Scr_ResetTimeout(inst);
                    waitTime = (waitTime + gScrVarPub[inst].time) & 0xFFFFFF;
                    --localFs.top;
                    stackValue.type = 10;
                    stackValue.u.intValue = (int)VM_ArchiveStack(inst, &localFs);
                    v20 = GetVariable(inst, gScrVarPub[inst].timeArrayId, waitTime);
                    id = GetArray(inst, v20);
                    stackId = GetNewObjectVariable(inst, id, localFs.localId);
                    SetNewVariableValue(inst, stackId, &stackValue);
                    Scr_SetThreadWaitTime(inst, localFs.localId, waitTime);
                    goto thread_end;
                }
                gScrVarPub[inst].error_index = 2;
                if (waitTime >= 0)
                {
                    gFs[inst] = localFs;
                    Scr_Error(inst, "wait is too long", 0);
                }
            negWait:
                gFs[inst] = localFs;
                Scr_Error(inst, "negative wait is not allowed", 0);
            $LN209:
                if (!Scr_IsInOpcodeMemory(inst, localFs.pos)
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        1712,
                        0,
                        "%s",
                        "Scr_IsInOpcodeMemory( inst, localFs.pos )"))
                {
                    __debugbreak();
                }
                if ((gScrVarPub[inst].time & 0xFF000000) != 0
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        1713,
                        0,
                        "%s",
                        "!(gScrVarPub[inst].time & ~VAR_NAME_LOW_MASK)"))
                {
                    __debugbreak();
                }
                stackValue.type = 10;
                stackValue.u.intValue = (int)VM_ArchiveStack(inst, &localFs);
                v21 = GetVariable(inst, gScrVarPub[inst].timeArrayId, gScrVarPub[inst].time);
                id = GetArray(inst, v21);
                stackId = GetNewObjectVariableReverse(inst, id, localFs.localId);
                SetNewVariableValue(inst, stackId, &stackValue);
                Scr_SetThreadWaitTime(inst, localFs.localId, gScrVarPub[inst].time);
                goto thread_end;
            }
            v93 = Scr_GetEntityIdRef(inst, objectId);
            entref = v93;
            RemoveRefToObject(inst, objectId);
            gScrVmPub[inst].function_frame->fs.pos = localFs.pos;
            if (gScrVmGlob[inst].recordPlace)
                Scr_GetFileAndLine(inst, localFs.pos, (char **)&gScrVmGlob[inst].lastFileName, &gScrVmGlob[inst].lastLine);
            if (gScrVmDebugPub[inst].func_table[builtinIndex].breakpointCount)
            {
                if (gScrVmPub[inst].top != localFs.top - 1
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        1611,
                        0,
                        "%s",
                        "gScrVmPub[inst].top == localFs.top - 1"))
                {
                    __debugbreak();
                }
                v105 = gScrVmPub[inst].outparamcount;
                Scr_HitBuiltinBreakpoint(inst, localFs.top, debugpos, localFs.localId, gOpcode[inst], builtinIndex, v105 + 1);
                gScrVmPub[inst].outparamcount = v105;
                gScrVmPub[inst].top = localFs.top - 1;
            }
            gFs[inst] = localFs;
            //LOWORD(v63) = entref.client;
            ((void(__cdecl *)(_DWORD, int))gScrCompilePub[inst].func_table[builtinIndex])(*(_DWORD *)&entref.entnum, entref.client);
        post_builtin:
            localFs.top = gScrVmPub[inst].top;
            localFs.pos = gScrVmPub[inst].function_frame->fs.pos;
            if (gScrVmPub[inst].outparamcount)
            {
                paramcount = gScrVmPub[inst].outparamcount;
                gScrVmPub[inst].outparamcount = 0;
                gScrVmPub[inst].top -= paramcount;
                do
                {
                    RemoveRefToValue(inst, localFs.top->type, localFs.top->u);
                    --localFs.top;
                    --paramcount;
                } while (paramcount);
            }
            if (gScrVmPub[inst].inparamcount)
            {
                if (gScrVmPub[inst].inparamcount != 1
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        1545,
                        0,
                        "%s",
                        "gScrVmPub[inst].inparamcount == 1"))
                {
                    __debugbreak();
                }
                gScrVmPub[inst].inparamcount = 0;
                if (localFs.top != gScrVmPub[inst].top
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        1547,
                        0,
                        "%s",
                        "localFs.top == gScrVmPub[inst].top"))
                {
                    __debugbreak();
                }
            }
            else
            {
                if (localFs.top != gScrVmPub[inst].top
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        1552,
                        0,
                        "%s",
                        "localFs.top == gScrVmPub[inst].top"))
                {
                    __debugbreak();
                }
                localFs.top[1].type = 0;
                ++localFs.top;
            }
            continue;
        case 76:
            goto $LN221_0;
        case 77:
            goto $LN209;
        case 78:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1727,
                    0,
                    "%s",
                    "localFs.top >= gScrVmPub[inst].stack"))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1728,
                    0,
                    "%s",
                    "localFs.top+1 <= gScrVmPub[inst].maxstack"))
            {
                __debugbreak();
            }
            localFs.top[1].type = 8;
            ++localFs.top;
            continue;
        case 79:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1734,
                    0,
                    "%s",
                    "localFs.top >= gScrVmPub[inst].stack"))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1735,
                    0,
                    "%s",
                    "localFs.top+1 <= gScrVmPub[inst].maxstack"))
            {
                __debugbreak();
            }
            localFs.top[1].type = 8;
            ++localFs.top;
            goto $LN205_0;
        case 80:
        $LN205_0:
            if (gScrVmPub[inst].function_count < 31)
            {
                selfId = Scr_GetSelf(inst, localFs.localId);
                AddRefToObject(inst, selfId);
                localFs.localId = AllocChildThread(inst, selfId, localFs.localId);
                gScrVmPub[inst].function_frame->fs.pos = localFs.pos;
                function_frame = gScrVmPub[inst].function_frame;
                v68 = *(const char **)function_frame->fs.pos;
                function_frame->fs.pos += 4;
                localFs.pos = v68;
                goto function_call;
            }
            gFs[inst] = localFs;
            Scr_Error(inst, "script stack overflow (too many embedded function calls)", 0);
        $LN202_0:
            if (localFs.top->type != 9)
                goto LABEL_442;
            if (gScrVmPub[inst].function_count < 31)
            {
                selfId = Scr_GetSelf(inst, localFs.localId);
                AddRefToObject(inst, selfId);
                localFs.localId = AllocChildThread(inst, selfId, localFs.localId);
                gScrVmPub[inst].function_frame->fs.pos = localFs.pos;
                localFs.pos = localFs.top->u.codePosValue;
                --localFs.top;
                goto function_call;
            }
            gScrVarPub[inst].error_index = 1;
            gFs[inst] = localFs;
            Scr_Error(inst, "script stack overflow (too many embedded function calls)", 0);
        LABEL_442:
            gFs[inst] = localFs;
            v22 = va("%s is not a function pointer", var_typename[localFs.top->type]);
            Scr_Error(inst, v22, 0);
        $LN198:
            if (localFs.top->type != 1)
                goto not_an_object1;
            if (gScrVmPub[inst].function_count < 31)
            {
                localFs.localId = AllocChildThread(inst, localFs.top->u.stringValue, localFs.localId);
                --localFs.top;
                gScrVmPub[inst].function_frame->fs.pos = localFs.pos;
                localFs.pos = Scr_ReadCodePos(inst, &gScrVmPub[inst].function_frame->fs.pos);
                goto function_call;
            }
            gFs[inst] = localFs;
            Scr_Error(inst, "script stack overflow (too many embedded function calls)", 0);
        not_an_object1:
            type = localFs.top->type;
            goto not_an_object_error1;
        case 81:
            goto $LN202_0;
        case 82:
            goto $LN198;
        case 83:
            if (localFs.top->type == 9)
            {
                tempCodePos = localFs.top->u.codePosValue;
                --localFs.top;
                if (localFs.top->type != 1)
                    goto not_an_object2;
                if (gScrVmPub[inst].function_count < 31)
                {
                    localFs.localId = AllocChildThread(inst, localFs.top->u.stringValue, localFs.localId);
                    --localFs.top;
                    gScrVmPub[inst].function_frame->fs.pos = localFs.pos;
                    localFs.pos = tempCodePos;
                    goto function_call;
                }
                gScrVarPub[inst].error_index = 1;
                gFs[inst] = localFs;
                Scr_Error(inst, "script stack overflow (too many embedded function calls)", 0);
            }
            else
            {
                RemoveRefToValue(inst, localFs.top--);
                gFs[inst] = localFs;
                v23 = va("%s is not a function pointer", var_typename[localFs.top[1].type]);
                Scr_Error(inst, v23, 0);
            $LN187:
                if (gScrVmPub[inst].function_count < 31)
                {
                    selfId = Scr_GetSelf(inst, localFs.localId);
                    AddRefToObject(inst, selfId);
                    localFs.localId = AllocThread(inst, selfId);
                    gScrVmPub[inst].function_frame->fs.pos = localFs.pos;
                    gScrVmPub[inst].function_frame->fs.startTop = localFs.startTop;
                    localFs.pos = Scr_ReadCodePos(inst, &gScrVmPub[inst].function_frame->fs.pos);
                    PrimDrawSurfInt = R_ReadPrimDrawSurfInt((GfxReadCmdBuf *)gScrVmPub[inst].function_frame);
                    localFs.startTop = &localFs.top[-(int)PrimDrawSurfInt];
                    goto thread_call;
                }
                gScrVarPub[inst].error_index = 1;
                gFs[inst] = localFs;
                Scr_Error(inst, "script stack overflow (too many embedded function calls)", 0);
            $LN184_0:
                if (localFs.top->type == 9)
                {
                    if (gScrVmPub[inst].function_count < 31)
                    {
                        tempCodePos = localFs.top->u.codePosValue;
                        --localFs.top;
                        selfId = Scr_GetSelf(inst, localFs.localId);
                        AddRefToObject(inst, selfId);
                        localFs.localId = AllocThread(inst, selfId);
                        gScrVmPub[inst].function_frame->fs.pos = localFs.pos;
                        gScrVmPub[inst].function_frame->fs.startTop = localFs.startTop;
                        localFs.pos = tempCodePos;
                        v25 = R_ReadPrimDrawSurfInt((GfxReadCmdBuf *)gScrVmPub[inst].function_frame);
                        localFs.startTop = &localFs.top[-(int)v25];
                        goto thread_call;
                    }
                    gScrVarPub[inst].error_index = 1;
                    gFs[inst] = localFs;
                    Scr_Error(inst, "script stack overflow (too many embedded function calls)", 0);
                }
                gFs[inst] = localFs;
                v26 = va("%s is not a function pointer", var_typename[localFs.top->type]);
                Scr_Error(inst, v26, 0);
            $LN180_0:
                if (localFs.top->type == 1)
                {
                    if (gScrVmPub[inst].function_count < 31)
                    {
                        localFs.localId = AllocThread(inst, localFs.top->u.stringValue);
                        --localFs.top;
                        gScrVmPub[inst].function_frame->fs.pos = localFs.pos;
                        gScrVmPub[inst].function_frame->fs.startTop = localFs.startTop;
                        localFs.pos = Scr_ReadCodePos(inst, &gScrVmPub[inst].function_frame->fs.pos);
                        v27 = R_ReadPrimDrawSurfInt((GfxReadCmdBuf *)gScrVmPub[inst].function_frame);
                        localFs.startTop = &localFs.top[-(int)v27];
                        goto thread_call;
                    }
                    gScrVarPub[inst].error_index = 1;
                    gFs[inst] = localFs;
                    Scr_Error(inst, "script stack overflow (too many embedded function calls)", 0);
                }
            }
            goto not_an_object2;
        case 84:
            goto $LN187;
        case 85:
            goto $LN184_0;
        case 86:
            goto $LN180_0;
        case 87:
            if (localFs.top->type != 9)
            {
                RemoveRefToValue(inst, localFs.top--);
                gFs[inst] = localFs;
                v29 = va("%s is not a function pointer", var_typename[localFs.top[1].type]);
                Scr_Error(inst, v29, 0);
            $LN169:
                RemoveRefToValue(inst, localFs.top);
                goto loop_dec_top;
            }
            tempCodePos = localFs.top->u.codePosValue;
            --localFs.top;
            if (localFs.top->type != 1)
                goto not_an_object2;
            if (gScrVmPub[inst].function_count >= 31)
            {
                gScrVarPub[inst].error_index = 1;
                gFs[inst] = localFs;
                Scr_Error(inst, "script stack overflow (too many embedded function calls)", 0);
            not_an_object2:
                type = localFs.top->type;
                goto not_an_object_error2;
            }
            localFs.localId = AllocThread(inst, localFs.top->u.stringValue);
            --localFs.top;
            gScrVmPub[inst].function_frame->fs.pos = localFs.pos;
            gScrVmPub[inst].function_frame->fs.startTop = localFs.startTop;
            localFs.pos = tempCodePos;
            v28 = R_ReadPrimDrawSurfInt((GfxReadCmdBuf *)gScrVmPub[inst].function_frame);
            localFs.startTop = &localFs.top[-(int)v28];
        thread_call:
            gScrVmPub[inst].function_frame->fs.top = localFs.startTop;
            gScrVmPub[inst].function_frame->topType = localFs.startTop->type;
            localFs.startTop->type = 8;
            ++gThreadCount[inst];
        function_call:
            gScrVmPub[inst].function_frame->fs.localVarCount = localFs.localVarCount;
            localFs.localVarCount = 0;
            ++gScrVmPub[inst].function_count;
            ++gScrVmPub[inst].function_frame;
            gScrVmPub[inst].function_frame->fs.localId = localFs.localId;
            if (!localFs.pos
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    2496,
                    0,
                    "%s",
                    "localFs.pos"))
            {
                __debugbreak();
            }
            continue;
        case 88:
            goto $LN169;
        case 89:
            objectId = Scr_EvalFieldObject(inst, gScrVarPub[inst].tempVariable, localFs.top);
            goto loop_dec_top;
        case 90:
            LocalVar = Scr_GetLocalVar(inst, localFs.pos);
            objectId = Scr_EvalVariableObject(inst, LocalVar);
            ++localFs.pos;
            continue;
        case 91:
            Scr_CastBool(inst, localFs.top);
            continue;
        case 92:
            Scr_EvalBoolNot(inst, localFs.top);
            continue;
        case 93:
            Scr_EvalBoolComplement(inst, localFs.top);
            continue;
        case 94:
            Scr_CastBool(inst, localFs.top);
            if (localFs.top->type != 6
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1944,
                    0,
                    "%s",
                    "localFs.top->type == VAR_INTEGER"))
            {
                __debugbreak();
            }
            jumpOffset = Scr_ReadUnsignedShort(&localFs.pos);
            if (!localFs.top->u.intValue)
                localFs.pos += jumpOffset;
            goto loop_dec_top;
        case 95:
            Scr_CastBool(inst, localFs.top);
            if (localFs.top->type != 6
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1953,
                    0,
                    "%s",
                    "localFs.top->type == VAR_INTEGER"))
            {
                __debugbreak();
            }
            jumpOffset = Scr_ReadUnsignedShort(&localFs.pos);
            if (localFs.top->u.intValue)
                localFs.pos += jumpOffset;
            goto loop_dec_top;
        case 96:
            Scr_CastBool(inst, localFs.top);
            if (localFs.top->type != 6
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1962,
                    0,
                    "%s",
                    "localFs.top->type == VAR_INTEGER"))
            {
                __debugbreak();
            }
            jumpOffset = Scr_ReadUnsignedShort(&localFs.pos);
            if (localFs.top->u.intValue)
                goto loop_dec_top;
            localFs.pos += jumpOffset;
            continue;
        case 97:
            Scr_CastBool(inst, localFs.top);
            if (localFs.top->type != 6
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    1971,
                    0,
                    "%s",
                    "localFs.top->type == VAR_INTEGER"))
            {
                __debugbreak();
            }
            jumpOffset = Scr_ReadUnsignedShort(&localFs.pos);
            if (!localFs.top->u.intValue)
                goto loop_dec_top;
            localFs.pos += jumpOffset;
            continue;
        case 98:
            jumpOffset = R_ReadPrimDrawSurfInt((GfxReadCmdBuf *)&localFs);
            localFs.pos += jumpOffset;
            continue;
        case 99:
            if (gScrVarPub[inst].numScriptValues > 0x3F37E || gScrVarPub[inst].numScriptObjects > 0x737E)
            {
                if (gScrVmPub[inst].showError)
                {
                    Scr_DumpScriptThreads(inst);
                    Scr_DumpScriptVariablesDefault(inst);
                    gFs[inst] = localFs;
                    Scr_Error(inst, "exceeded maximum number of script variables", 0);
                }
                Sys_Error((char*)"exceeded maximum number of script variables");
            }
            if ((int)(Sys_Milliseconds() - gScrVmGlob[inst].starttime) >= 2500)
            {
                if (!logScriptTimes
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        2013,
                        0,
                        "%s",
                        "logScriptTimes"))
                {
                    __debugbreak();
                }
                if (logScriptTimes->current.enabled)
                {
                    v31 = Sys_Milliseconds();
                    Com_Printf(24, "EXCEED TIME: %d\n", v31);
                }
                if (!gScrVmGlob[inst].loading)
                {
                    if (RETURN_ZERO32())
                        goto ignore_error;
                    VM_PrintJumpHistory(inst);
                    if (gScrVmPub[inst].showError)
                    {
                        if (gScrVmPub[inst].debugCode
                            && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                2046,
                                0,
                                "%s",
                                "!gScrVmPub[inst].debugCode"))
                        {
                            __debugbreak();
                        }
                        Scr_DumpScriptThreads(inst);
                        Scr_DumpScriptVariablesDefault(inst);
                        gFs[inst] = localFs;
                        Scr_Error(inst, "potential infinite loop in script", 0);
                    }
                    if (!gScrVmPub[inst].abort_on_error)
                    {
                        if (inst == SCRIPTINSTANCE_CLIENT)
                            Com_PrintError(
                                24,
                                "%s script runtime error: potential infinite loop in script - killing thread.\n",
                                "client");
                        else
                            Com_PrintError(
                                24,
                                "%s script runtime error: potential infinite loop in script - killing thread.\n",
                                "server");
                        Scr_PrintPrevCodePos(inst, 24, (char *)localFs.pos, 0);
                        Scr_ResetTimeout(inst);
                        while (1)
                        {
                            parentLocalId = GetSafeParentLocalId(inst, localFs.localId);
                            Scr_KillThread(inst, localFs.localId);
                            gScrVmPub[inst].localVars -= localFs.localVarCount;
                            if (localFs.top->type == 8
                                && !Assert_MyHandler(
                                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                    2541,
                                    0,
                                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                                    "(localFs.top->type != VAR_PRECODEPOS)",
                                    localFs.top - gScrVmPub[inst].stack))
                            {
                                __debugbreak();
                            }
                            while (localFs.top->type != 7)
                            {
                                RemoveRefToValue(inst, localFs.top--);
                                if (localFs.top->type == 8
                                    && !Assert_MyHandler(
                                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                        2546,
                                        0,
                                        "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                                        "(localFs.top->type != VAR_PRECODEPOS)",
                                        localFs.top - gScrVmPub[inst].stack))
                                {
                                    __debugbreak();
                                }
                            }
                            --gScrVmPub[inst].function_count;
                            --gScrVmPub[inst].function_frame;
                            if (!parentLocalId)
                                break;
                            if (localFs.top == localFs.startTop
                                && !Assert_MyHandler(
                                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                    2557,
                                    0,
                                    "%s",
                                    "localFs.top != localFs.startTop"))
                            {
                                __debugbreak();
                            }
                            RemoveRefToObject(inst, localFs.localId);
                            if (localFs.top->type != 7
                                && !Assert_MyHandler(
                                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                    2565,
                                    0,
                                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                                    "(localFs.top->type == VAR_CODEPOS)",
                                    localFs.top - gScrVmPub[inst].stack))
                            {
                                __debugbreak();
                            }
                            localFs.pos = gScrVmPub[inst].function_frame->fs.pos;
                            if (!localFs.pos
                                && !Assert_MyHandler(
                                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                    2567,
                                    0,
                                    "%s",
                                    "localFs.pos"))
                            {
                                __debugbreak();
                            }
                            localFs.localVarCount = gScrVmPub[inst].function_frame->fs.localVarCount;
                            localFs.localId = parentLocalId;
                            --localFs.top;
                        }
                        if (localFs.top != localFs.startTop
                            && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                2553,
                                0,
                                "%s\n\t(localFs.top - localFs.startTop) = %i",
                                "(localFs.top == localFs.startTop)",
                                localFs.top - localFs.startTop))
                        {
                            __debugbreak();
                        }
                        goto thread_end;
                    }
                    Scr_Error(inst, "potential infinite loop in script", 0);
                $LN142:
                    if (localFs.top < gScrVmPub[inst].stack
                        && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                            2068,
                            0,
                            "%s",
                            "localFs.top >= gScrVmPub[inst].stack"))
                    {
                        __debugbreak();
                    }
                    if (&localFs.top[1] > gScrVmPub[inst].maxstack
                        && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                            2069,
                            0,
                            "%s",
                            "localFs.top+1 <= gScrVmPub[inst].maxstack"))
                    {
                        __debugbreak();
                    }
                    v91 = Scr_EvalVariableField(inst, fieldValueId);
                    localFs.top[1] = v91;
                    if (localFs.top[1].type == 6)
                    {
                        ++localFs.top[1].u.intValue;
                        if (*localFs.pos != 57
                            && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                2074,
                                0,
                                "%s",
                                "*localFs.pos == OP_SetVariableField"))
                        {
                            __debugbreak();
                        }
                    }
                    else
                    {
                        ++localFs.top;
                        gFs[inst] = localFs;
                        v32 = va("++ must be applied to an int (applied to %s)", var_typename[localFs.top->type]);
                        Scr_Error(inst, v32, 0);
                    $LN140_0:
                        if (localFs.top < gScrVmPub[inst].stack
                            && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                2085,
                                0,
                                "%s",
                                "localFs.top >= gScrVmPub[inst].stack"))
                        {
                            __debugbreak();
                        }
                        if (&localFs.top[1] > gScrVmPub[inst].maxstack
                            && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                2086,
                                0,
                                "%s",
                                "localFs.top+1 <= gScrVmPub[inst].maxstack"))
                        {
                            __debugbreak();
                        }
                        v90 = Scr_EvalVariableField(inst, fieldValueId);
                        localFs.top[1] = v90;
                        if (localFs.top[1].type != 6)
                        {
                            ++localFs.top;
                            gFs[inst] = localFs;
                            v33 = va("-- must be applied to an int (applied to %s)", var_typename[localFs.top->type]);
                            Scr_Error(inst, v33, 0);
                        $LN138:
                            Scr_EvalOr(inst, localFs.top - 1, localFs.top);
                            goto loop_dec_top;
                        }
                        --localFs.top[1].u.intValue;
                        if (*localFs.pos != 57
                            && !Assert_MyHandler(
                                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                                2091,
                                0,
                                "%s",
                                "*localFs.pos == OP_SetVariableField"))
                        {
                            __debugbreak();
                        }
                    }
                    ++localFs.pos;
                    SetVariableFieldValue(inst, fieldValueId, ++localFs.top);
                loop_dec_top:
                    --localFs.top;
                    continue;
                }
                if (gScrVmPub[inst].abort_on_error)
                    gScrVmPub[inst].showError = 1;
            ignore_error:
                Com_PrintWarning(24, "script runtime warning: potential infinite loop in script.\n");
                Scr_PrintPrevCodePos(inst, 24, (char *)localFs.pos, 0);
                jumpOffset = Scr_ReadUnsignedShort(&localFs.pos);
                localFs.pos -= jumpOffset;
                Scr_ResetTimeout(inst);
            }
            else
            {
                gScrVmDebugPub[inst].jumpbackHistory[gScrVmDebugPub[inst].jumpbackHistoryIndex] = localFs.pos;
                gScrVmDebugPub[inst].jumpbackHistoryIndex = (gScrVmDebugPub[inst].jumpbackHistoryIndex + 1) % 0x80u;
                jumpOffset = Scr_ReadUnsignedShort(&localFs.pos);
                localFs.pos -= jumpOffset;
            }
            continue;
        case 100:
            goto $LN142;
        case 101:
            goto $LN140_0;
        case 102:
            goto $LN138;
        case 103:
            Scr_EvalExOr(inst, localFs.top - 1, localFs.top);
            goto loop_dec_top;
        case 104:
            Scr_EvalAnd(inst, localFs.top - 1, localFs.top);
            goto loop_dec_top;
        case 105:
            Scr_EvalEquality(inst, localFs.top - 1, localFs.top);
            goto loop_dec_top;
        case 106:
            Scr_EvalInequality(inst, localFs.top - 1, localFs.top);
            goto loop_dec_top;
        case 107:
            Scr_EvalLess(inst, localFs.top - 1, localFs.top);
            goto loop_dec_top;
        case 108:
            Scr_EvalGreater(inst, localFs.top - 1, localFs.top);
            goto loop_dec_top;
        case 109:
            Scr_EvalLessEqual(inst, localFs.top - 1, localFs.top);
            goto loop_dec_top;
        case 110:
            Scr_EvalGreaterEqual(inst, localFs.top - 1, localFs.top);
            goto loop_dec_top;
        case 111:
            Scr_EvalShiftLeft(inst, localFs.top - 1, localFs.top);
            goto loop_dec_top;
        case 112:
            Scr_EvalShiftRight(inst, localFs.top - 1, localFs.top);
            goto loop_dec_top;
        case 113:
            Scr_EvalPlus(inst, localFs.top - 1, localFs.top);
            goto loop_dec_top;
        case 114:
            Scr_EvalMinus(inst, localFs.top - 1, localFs.top);
            goto loop_dec_top;
        case 115:
            Scr_EvalMultiply(inst, localFs.top - 1, localFs.top);
            goto loop_dec_top;
        case 116:
            Scr_EvalDivide(inst, localFs.top - 1, localFs.top);
            goto loop_dec_top;
        case 117:
            Scr_EvalMod(inst, localFs.top - 1, localFs.top);
            goto loop_dec_top;
        case 118:
            Scr_EvalSizeValue(inst, localFs.top);
            continue;
        case 119:
        case 120:
            if (!Scr_IsInOpcodeMemory(inst, localFs.pos)
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    2171,
                    0,
                    "%s",
                    "Scr_IsInOpcodeMemory( inst, localFs.pos )"))
            {
                __debugbreak();
            }
            if (localFs.top->type != 1)
                goto not_an_object2;
            if (!IsFieldObject(inst, localFs.top->u.stringValue))
                goto not_an_object2a;
            tempValue.u.intValue = localFs.top->u.intValue;
            --localFs.top;
            if (localFs.top->type != 2)
            {
                ++localFs.top;
                gScrVarPub[inst].error_index = 3;
                gFs[inst] = localFs;
                Scr_Error(inst, "first parameter of waittill must evaluate to a string", 0);
            not_an_object2a:
                type = GetObjectType(inst, localFs.top->u.stringValue);
            not_an_object_error2:
                gScrVarPub[inst].error_index = 2;
            not_an_object_error:
                gFs[inst] = localFs;
                v57 = va("%s is not an object", var_typename[type]);
                Scr_Error(inst, v57, 0);
                goto error_1;
            }
            stringValue = localFs.top->u.stringValue;
            --localFs.top;
            if (GetObjectType(inst, tempValue.u.stringValue) == 13
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    2186,
                    0,
                    "%s",
                    "GetObjectType( inst, tempValue.u.pointerValue ) != VAR_THREAD"))
            {
                __debugbreak();
            }
            if (GetObjectType(inst, tempValue.u.stringValue) == 14
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    2187,
                    0,
                    "%s",
                    "GetObjectType( inst, tempValue.u.pointerValue ) != VAR_NOTIFY_THREAD"))
            {
                __debugbreak();
            }
            if (GetObjectType(inst, tempValue.u.stringValue) == 15
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    2188,
                    0,
                    "%s",
                    "GetObjectType( inst, tempValue.u.pointerValue ) != VAR_TIME_THREAD"))
            {
                __debugbreak();
            }
            if (GetObjectType(inst, tempValue.u.stringValue) == 16
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    2189,
                    0,
                    "%s",
                    "GetObjectType( inst, tempValue.u.pointerValue ) != VAR_CHILD_THREAD"))
            {
                __debugbreak();
            }
            if (GetObjectType(inst, tempValue.u.stringValue) == 21
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    2190,
                    0,
                    "%s",
                    "GetObjectType( inst, tempValue.u.pointerValue ) != VAR_DEAD_THREAD"))
            {
                __debugbreak();
            }
            stackValue.type = 10;
            stackValue.u.intValue = (int)VM_ArchiveStack(inst, &localFs);
            v64 = stringValue;
            v34 = GetVariable(inst, tempValue.u.stringValue, 0x17FFEu);
            Array = GetArray(inst, v34);
            v36 = GetVariable(inst, Array, v64);
            id = GetArray(inst, v36);
            stackId = GetNewObjectVariable(inst, id, localFs.localId);
            SetNewVariableValue(inst, stackId, &stackValue);
            tempValue.type = 1;
            localId = localFs.localId;
            v37 = Scr_GetSelf(inst, localFs.localId);
            ObjectVariable = GetObjectVariable(inst, gScrVarPub[inst].pauseArrayId, v37);
            v39 = GetArray(inst, ObjectVariable);
            NewObjectVariable = GetNewObjectVariable(inst, v39, localId);
            SetNewVariableValue(inst, NewObjectVariable, &tempValue);
            Scr_SetThreadNotifyName(inst, localFs.localId, stringValue);
            goto thread_end;
        case 121:
            if (localFs.top->type != 1)
                goto not_an_object2;
            id = localFs.top->u.stringValue;
            if (!IsFieldObject(inst, id))
                goto not_an_object2a;
            --localFs.top;
            if (localFs.top->type != 2)
            {
                ++localFs.top;
                gScrVarPub[inst].error_index = 1;
                gFs[inst] = localFs;
                Scr_Error(inst, "first parameter of notify must evaluate to a string", 0);
                goto not_an_object2a;
            }
            stringValue = localFs.top->u.stringValue;
            --localFs.top;
            if (gScrVmDebugPub[inst].checkBreakon)
                Scr_CheckBreakonNotify(inst, id, stringValue, localFs.top, localFs.pos, localFs.localId);
            gScrVmPub[inst].function_frame->fs.pos = localFs.pos;
            VM_Notify(inst, id, stringValue, localFs.top);
            localFs.pos = gScrVmPub[inst].function_frame->fs.pos;
            RemoveRefToObject(inst, id);
            SL_RemoveRefToString(inst, stringValue);
            if (localFs.top->type == 7
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    2246,
                    0,
                    "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                    "(localFs.top->type != VAR_CODEPOS)",
                    localFs.top - gScrVmPub[inst].stack))
            {
                __debugbreak();
            }
            while (localFs.top->type != 8)
            {
                RemoveRefToValue(inst, localFs.top--);
                if (localFs.top->type == 7
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        2251,
                        0,
                        "%s\n\t(localFs.top - gScrVmPub[inst].stack) = %i",
                        "(localFs.top->type != VAR_CODEPOS)",
                        localFs.top - gScrVmPub[inst].stack))
                {
                    __debugbreak();
                }
            }
            goto loop_dec_top;
        case 122:
            if (localFs.top->type != 1)
                goto not_an_object1;
            if (!IsFieldObject(inst, localFs.top->u.stringValue))
                goto LABEL_633;
            if (localFs.top[-1].type == 2)
            {
                stringValue = localFs.top[-1].u.stringValue;
                AddRefToObject(inst, localFs.localId);
                threadId = AllocThread(inst, localFs.localId);
                if (GetObjectType(inst, localFs.top->u.stringValue) == 13
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        2280,
                        0,
                        "%s",
                        "GetObjectType( inst, localFs.top->u.pointerValue ) != VAR_THREAD"))
                {
                    __debugbreak();
                }
                if (GetObjectType(inst, localFs.top->u.stringValue) == 14
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        2281,
                        0,
                        "%s",
                        "GetObjectType( inst, localFs.top->u.pointerValue ) != VAR_NOTIFY_THREAD"))
                {
                    __debugbreak();
                }
                if (GetObjectType(inst, localFs.top->u.stringValue) == 15
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        2282,
                        0,
                        "%s",
                        "GetObjectType( inst, localFs.top->u.pointerValue ) != VAR_TIME_THREAD"))
                {
                    __debugbreak();
                }
                if (GetObjectType(inst, localFs.top->u.stringValue) == 16
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        2283,
                        0,
                        "%s",
                        "GetObjectType( inst, localFs.top->u.pointerValue ) != VAR_CHILD_THREAD"))
                {
                    __debugbreak();
                }
                if (GetObjectType(inst, localFs.top->u.stringValue) == 21
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        2284,
                        0,
                        "%s",
                        "GetObjectType( inst, localFs.top->u.pointerValue ) != VAR_DEAD_THREAD"))
                {
                    __debugbreak();
                }
                v65 = threadId;
                v59 = stringValue;
                v41 = GetVariable(inst, localFs.top->u.stringValue, 0x17FFEu);
                v42 = GetArray(inst, v41);
                v43 = GetVariable(inst, v42, v59);
                v44 = GetArray(inst, v43);
                GetObjectVariable(inst, v44, v65);
                RemoveRefToObject(inst, threadId);
                tempValue.type = 1;
                tempValue.u.intValue = localFs.top->u.intValue;
                v60 = threadId;
                v45 = GetObjectVariable(inst, gScrVarPub[inst].pauseArrayId, localFs.localId);
                v46 = GetArray(inst, v45);
                v47 = GetNewObjectVariable(inst, v46, v60);
                SetNewVariableValue(inst, v47, &tempValue);
                Scr_SetThreadNotifyName(inst, threadId, stringValue);
                localFs.top -= 2;
                continue;
            }
            gFs[inst] = localFs;
            Scr_Error(inst, "first parameter of endon must evaluate to a string", 0);
        LABEL_633:
            type = GetObjectType(inst, localFs.top->u.stringValue);
        not_an_object_error1:
            gScrVarPub[inst].error_index = 1;
            goto not_an_object_error;
        case 123:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    2314,
                    0,
                    "%s",
                    "localFs.top >= gScrVmPub[inst].stack"))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    2315,
                    0,
                    "%s",
                    "localFs.top+1 <= gScrVmPub[inst].maxstack"))
            {
                __debugbreak();
            }
            localFs.top[1].type = 8;
            ++localFs.top;
            continue;
        case 124:
            jumpOffset = R_ReadPrimDrawSurfInt((GfxReadCmdBuf *)&localFs);
            localFs.pos += jumpOffset;
            gCaseCount[inst] = Scr_ReadUnsignedShort(&localFs.pos);
            v66 = localFs.top->type;
            if (v66 == 2)
                goto LABEL_647;
            if (v66 == 6)
            {
                if (IsValidArrayIndex(inst, localFs.top->u.stringValue))
                {
                    caseValue = GetInternalVariableIndex(inst, localFs.top->u.stringValue);
                }
                else
                {
                    gFs[inst] = localFs;
                    v48 = va("switch index %d out of range", localFs.top->u.intValue);
                    Scr_Error(inst, v48, 0);
                LABEL_647:
                    caseValue = localFs.top->u.stringValue;
                    SL_RemoveRefToString(inst, localFs.top->u.stringValue);
                }
            }
            else
            {
                gFs[inst] = localFs;
                v49 = va("cannot switch on %s", var_typename[localFs.top->type]);
                Scr_Error(inst, v49, 0);
            }
            if (!gCaseCount[inst])
                goto loop_dec_top;
            if (!caseValue
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    2352,
                    0,
                    "%s",
                    "caseValue"))
            {
                __debugbreak();
            }
            do
            {
                currentCaseValue = R_ReadPrimDrawSurfInt((GfxReadCmdBuf *)&localFs);
                currentCodePos = Scr_ReadCodePos(inst, &localFs.pos);
                if (currentCaseValue == caseValue)
                {
                    localFs.pos = currentCodePos;
                    if (!currentCodePos
                        && !Assert_MyHandler(
                            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                            2361,
                            0,
                            "%s",
                            "localFs.pos"))
                    {
                        __debugbreak();
                    }
                    goto loop_dec_top;
                }
                --gCaseCount[inst];
            } while (gCaseCount[inst]);
            if (!currentCaseValue)
            {
                localFs.pos = currentCodePos;
                if (!currentCodePos
                    && !Assert_MyHandler(
                        "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                        2371,
                        0,
                        "%s",
                        "localFs.pos"))
                {
                    __debugbreak();
                }
            }
            goto loop_dec_top;
        case 125:
            gCaseCount[inst] = Scr_ReadUnsignedShort(&localFs.pos);
            R_ReadPrimDrawSurfData((GfxReadCmdBuf *)&localFs, 2 * gCaseCount[inst]);
            continue;
        case 126:
            localFs.top -= 2;
            Scr_CastVector(inst, localFs.top);
            continue;
        case 127:
            continue;
        case 128:
            if (g_script_error_level[inst] < 0
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    2387,
                    0,
                    "%s\n\t(g_script_error_level[inst]) = %i",
                    "(g_script_error_level[inst] >= 0)",
                    g_script_error_level[inst]))
            {
                __debugbreak();
            }
            --g_script_error_level[inst];
            gFs[inst] = localFs;
            return 0;
        case 129:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    2397,
                    0,
                    "%s",
                    "localFs.top >= gScrVmPub[inst].stack"))
            {
                __debugbreak();
            }
            if (localFs.top > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    2398,
                    0,
                    "%s",
                    "localFs.top <= gScrVmPub[inst].maxstack"))
            {
                __debugbreak();
            }
            classnum = R_ReadPrimDrawSurfInt((GfxReadCmdBuf *)&localFs);
            entnum = R_ReadPrimDrawSurfInt((GfxReadCmdBuf *)&localFs);
            EntityId = FindEntityId(inst, entnum, classnum, 0);
            localFs.top[1].u.intValue = EntityId;
            if (localFs.top[1].u.intValue)
            {
                ++localFs.top;
            }
            else
            {
                localFs.top[1].type = 0;
                ++localFs.top;
                gFs[inst] = localFs;
                Scr_Error(inst, "unknown object", 0);
            }
            goto object;
        case 130:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    2422,
                    0,
                    "%s",
                    "localFs.top >= gScrVmPub[inst].stack"))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    2423,
                    0,
                    "%s",
                    "localFs.top+1 <= gScrVmPub[inst].maxstack"))
            {
                __debugbreak();
            }
            UnsignedShort = Scr_ReadUnsignedShort(&localFs.pos);
            localFs.top[1].u.intValue = UnsignedShort;
            ++localFs.top;
        object:
            localFs.top->type = 1;
            AddRefToObject(inst, localFs.top->u.stringValue);
            continue;
        case 131:
            if (localFs.top < gScrVmPub[inst].stack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    2429,
                    0,
                    "%s",
                    "localFs.top >= gScrVmPub[inst].stack"))
            {
                __debugbreak();
            }
            if (&localFs.top[1] > gScrVmPub[inst].maxstack
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    2430,
                    0,
                    "%s",
                    "localFs.top+1 <= gScrVmPub[inst].maxstack"))
            {
                __debugbreak();
            }
            v53 = Scr_ReadUnsignedShort(&localFs.pos);
            v54 = FindVariable(inst, localFs.localId, v53);
            v89 = Scr_EvalVariable(inst, v54);
            localFs.top[1] = v89;
            ++localFs.top;
            continue;
        case 132:
            fieldValueIndex = 0;
            v55 = Scr_ReadUnsignedShort(&localFs.pos);
            fieldValueId = FindVariable(inst, localFs.localId, v55);
            if (fieldValueId)
                continue;
            gFs[inst] = localFs;
            Scr_Error(inst, "cannot create a new local variable in the debugger", 0);
            goto $LN74_5;
        case 133:
        $LN74_5:
            ++localFs.pos;
            continue;
        case 134:
            ++localFs.pos;
            continue;
        case 135:
            if (!gScrVarPub[inst].developer)
                continue;
            gOpcode[inst] = Scr_HitBreakpoint(inst, localFs.top, localFs.pos, localFs.localId, 0);
            goto interrupt_return;
        case 136:
            gOpcode[inst] = Scr_HitAssignmentBreakpoint(inst, localFs.top, localFs.pos, localFs.localId, 0);
            goto interrupt_return;
        case 137:
            gOpcode[inst] = Scr_HitAssignmentBreakpoint(inst, localFs.top, localFs.pos, localFs.localId, 1);
            goto interrupt_return;
        default:
            gScrVmPub[inst].terminal_error = 1;
            v56 = va("CODE ERROR: unknown opcode %d", gOpcode[inst]);
            RuntimeError(inst, (char *)localFs.pos, 0, v56, 0);
            continue;
        }
    }
}