#include "cscr_vm.h"

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
  MEMORY[0xA05AB8C][29 * inst] = 0;
  dword_A0667D8[2058 * inst] = 0;
  MEMORY[0xA05AB90][29 * inst] = 0;
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

void __cdecl Scr_VM_Init(scriptInstance_t inst)
{
  MEMORY[0xA05ABE4][29 * inst] = (int)"<script init variable>";
  MEMORY[0xA05AC8C][4298 * inst] = (int)&MEMORY[0xA05EFA8] + 17192 * inst;
  MEMORY[0xA05AC98][4298 * inst] = (int)&MEMORY[0xA05AFB0] + 17192 * inst;
  MEMORY[0xA05AC90][4298 * inst] = 0;
  MEMORY[0xA05AC94][4298 * inst] = (int)&MEMORY[0xA05ACB0][4298 * inst];
  gScrVmPub[inst].localVars = (unsigned int *)&dword_A0667E0[2058 * inst];
  MEMORY[0xA05AB88][116 * inst] = 0;
  MEMORY[0xA05AC9C][17192 * inst] = 0;
  Scr_ClearErrorMessage(inst);
  MEMORY[0xA05AC9E][17192 * inst] = 0;
  MEMORY[0xA05ACA4][4298 * inst] = 0;
  MEMORY[0xA05ACA0][4298 * inst] = 0;
  MEMORY[0xA05ABB0][29 * inst] = AllocValue(inst);
  MEMORY[0xA05AB98][29 * inst] = 0;
  MEMORY[0xA05AB9C][29 * inst] = 0;
  MEMORY[0xA05ABA0][29 * inst] = 0;
  MEMORY[0xA05ABA4][29 * inst] = 0;
  MEMORY[0xA05ABA8][29 * inst] = 0;
  MEMORY[0xA05ABAC][29 * inst] = 0;
  MEMORY[0xA05AFB4][4298 * inst] = 7;
  dword_A0667DC[2058 * inst] = 0;
  byte_A0687E4[8232 * inst] = 0;
  dword_A0687E8[2058 * inst] = 0;
  dword_A0687EC[2058 * inst] = 0;
  MEMORY[0xA05ABE8][29 * inst] = 0;
  MEMORY[0xA05ABD8][29 * inst] = 0;
  MEMORY[0xA05ABE4][29 * inst] = 0;
  logScriptTimes = _Dvar_RegisterBool("logScriptTimes", 0, 0, "Log times for every print called from script");
  scrVmEnableScripts = _Dvar_RegisterBool("scrVmEnableScripts", 1, 0, "Enables script execution");
  scrShowVarUseage = _Dvar_RegisterBool("scrShowVarUseage", 0, 0, "Displays var useage at compile time.");
  scrShowStrUsage = _Dvar_RegisterBool("scrShowStrUsage", 0, 0, "Displays script string usage at compile time.");
  if ( inst == SCRIPTINSTANCE_SERVER )
    sv_clientside = _Dvar_RegisterBool(
                      "sv_clientside",
                      0,
                      0,
                      "Used to toggle systems in script on and off on the server.");
  Cmd_AddCommandInternal("scrProfileInfo", BG_EvalVehicleName, &VM_DumpScriptProfileInfo_VAR);
}

void __cdecl Scr_Settings(int developer, int developer_script, int abort_on_error, scriptInstance_t inst)
{
  if ( abort_on_error
    && !developer
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          639,
          0,
          "%s",
          "!abort_on_error || developer") )
  {
    __debugbreak();
  }
  MEMORY[0xA05AB86][116 * inst] = developer != 0;
  MEMORY[0xA05AB87][116 * inst] = developer_script != 0;
  MEMORY[0xA05AC9D][17192 * inst] = abort_on_error != 0;
}

void __cdecl Scr_Shutdown(scriptInstance_t inst)
{
  if ( MEMORY[0xA05ABB4][116 * inst] )
  {
    MEMORY[0xA05ABB4][116 * inst] = 0;
    VM_Shutdown(inst);
    Scr_ShutdownVariables(inst);
  }
}

void __cdecl VM_Shutdown(scriptInstance_t inst)
{
  if ( MEMORY[0xA05ABB0][29 * inst] )
  {
    FreeValue(inst, MEMORY[0xA05ABB0][29 * inst]);
    MEMORY[0xA05ABB0][29 * inst] = 0;
  }
}

void __cdecl Scr_SetLoading(int bLoading, scriptInstance_t inst)
{
  dword_A0667DC[2058 * inst] = bLoading;
}

int __cdecl Scr_GetNumScriptThreads(scriptInstance_t inst)
{
  return MEMORY[0xA05ABD8][29 * inst];
}

void __cdecl Scr_ClearOutParams(scriptInstance_t inst)
{
  while ( MEMORY[0xA05ACA4][4298 * inst] )
  {
    RemoveRefToValue(
      inst,
      *(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] + 4),
      *(VariableUnion *)MEMORY[0xA05AC98][4298 * inst]);
    MEMORY[0xA05AC98][4298 * inst] -= 8;
    --MEMORY[0xA05ACA4][4298 * inst];
  }
}

char *__cdecl Scr_GetReturnPos(scriptInstance_t inst, unsigned int *localId)
{
  char *pos; // [esp+0h] [ebp-4h]

  if ( MEMORY[0xA05AC90][4298 * inst] <= 1 )
    return 0;
  pos = *(char **)(MEMORY[0xA05AC94][4298 * inst] - 24);
  if ( pos == &g_EndPos )
    return 0;
  *localId = *(unsigned int *)(MEMORY[0xA05AC94][4298 * inst] - 20);
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

  *localId = *(unsigned int *)(MEMORY[0xA05AC94][4298 * inst] + 4);
  while ( 2 )
  {
    ++pos;
    if ( MEMORY[0xA05AB8C][29 * inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            2958,
            0,
            "%s",
            "!gScrVarPub[inst].error_message") )
    {
      __debugbreak();
    }
    if ( !top && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 2959, 0, "%s", "top") )
      __debugbreak();
    if ( !pos && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 2960, 0, "%s", "pos") )
      __debugbreak();
    if ( !MEMORY[0xA05AB88][116 * inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            2961,
            0,
            "%s",
            "gScrVarPub[inst].evaluate") )
    {
      __debugbreak();
    }
    if ( mode == 2 )
    {
      switch ( opcode )
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
          if ( top->type != 1 )
            goto LABEL_23;
$LN46_5:
          if ( MEMORY[0xA05AC90][4298 * inst] >= 32 )
            goto LABEL_23;
          *localId = 0;
          result = *(char **)pos;
          break;
        case 'S':
        case 'W':
          if ( top[-1].type != 1 )
            goto LABEL_23;
$LN42_9:
          if ( top->type != 9 || MEMORY[0xA05AC90][4298 * inst] >= 32 )
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
      switch ( opcode )
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
          if ( MEMORY[0xA05AB8C][29 * inst] )
            goto LABEL_76;
          if ( value.type != 6
            && !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                  3017,
                  0,
                  "%s",
                  "value.type == VAR_INTEGER") )
          {
            __debugbreak();
          }
          if ( value.u.intValue )
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
          if ( MEMORY[0xA05AB8C][29 * inst] )
            goto LABEL_76;
          if ( value.type != 6
            && !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                  3033,
                  0,
                  "%s",
                  "value.type == VAR_INTEGER") )
          {
            __debugbreak();
          }
          if ( value.u.intValue )
            return (char *)&pos[v14];
          else
            return (char *)pos;
        case 98:
          return (char *)&pos[*(unsigned int *)pos + 4];
        case 99:
          return (char *)&pos[-*(unsigned __int16 *)pos + 2];
        case 124:
          posb = &pos[*(unsigned int *)pos + 4];
          v13 = *(_WORD *)posb;
          posa = posb + 2;
          caseCount = v13;
          v10 = top->type;
          if ( v10 == 2 )
          {
            caseValue = top->u.intValue;
          }
          else
          {
            if ( v10 != 6 )
              return (char *)&posa[8 * v13];
            if ( !IsValidArrayIndex(inst, top->u.intValue) )
              return (char *)&posa[8 * v13];
            caseValue = GetInternalVariableIndex(inst, top->u.intValue);
          }
          if ( !v13 )
            return (char *)posa;
          if ( !caseValue
            && !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                  3081,
                  0,
                  "%s",
                  "caseValue") )
          {
            __debugbreak();
          }
          break;
        case 125:
          return (char *)&pos[8 * *(unsigned __int16 *)pos + 2];
        default:
          v9 = va("unknown opcode %d", opcode);
          if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3269, 0, v9) )
            __debugbreak();
LABEL_76:
          Scr_ClearErrorMessage(inst);
          opcode = *pos;
          if ( opcode == 57 )
            continue;
          return (char *)pos;
      }
      do
      {
        v12 = *(unsigned int *)posa;
        posc = posa + 4;
        v11 = *(const char **)posc;
        posa = posc + 4;
        if ( v12 == caseValue )
        {
          if ( !v11
            && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3090, 0, "%s", "pos") )
          {
            __debugbreak();
          }
          return (char *)v11;
        }
        --caseCount;
      }
      while ( caseCount );
      if ( !v12 )
      {
        posa = v11;
        if ( !v11
          && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3100, 0, "%s", "pos") )
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
  VariableValueInternal::<unnamed_type_u> *VariableValueAddress; // eax
  unsigned int Variable; // eax
  unsigned int v8; // eax
  unsigned int Object; // eax
  unsigned int stringValue; // [esp+0h] [ebp-18h]
  unsigned int selfNameId; // [esp+4h] [ebp-14h]
  unsigned int notifyListId; // [esp+8h] [ebp-10h]
  ObjectInfo::<unnamed_type_u> selfId; // [esp+14h] [ebp-4h]

  if ( !MEMORY[0xA05AB86][116 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          3883,
          0,
          "%s",
          "gScrVarPub[inst].developer") )
  {
    __debugbreak();
  }
  ThreadNotifyName = Scr_GetThreadNotifyName(inst, startLocalId);
  stringValue = ThreadNotifyName;
  if ( !ThreadNotifyName )
    return FindVariable(inst, startLocalId, 0x17FFFu);
  selfId = Scr_GetSelf(inst, startLocalId);
  ObjectVariable = FindObjectVariable(inst, MEMORY[0xA05AB9C][29 * inst], selfId.nextEntId);
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
  VariableValueInternal::<unnamed_type_u> *VariableValueAddress; // eax
  unsigned int ObjectType; // [esp+0h] [ebp-10h]
  unsigned int stackId; // [esp+8h] [ebp-8h]
  unsigned int startLocalId; // [esp+Ch] [ebp-4h]

  if ( !MEMORY[0xA05AB86][116 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          3951,
          0,
          "%s",
          "gScrVarPub[inst].developer") )
  {
    __debugbreak();
  }
  startLocalId = GetStartLocalId(inst, localId);
  ObjectType = GetObjectType(inst, startLocalId);
  switch ( ObjectType )
  {
    case 0xDu:
      return Scr_GetRunningThreadPos(inst, localId);
    case 0xEu:
      stackId = Scr_GetWaittillThreadStackId(inst, localId, startLocalId);
      if ( !stackId
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3961, 0, "%s", "stackId") )
      {
        __debugbreak();
      }
      if ( GetValueType(inst, stackId) != 10 )
      {
        ValueType = GetValueType(inst, stackId);
        if ( !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                3962,
                0,
                "%s\n\t(GetValueType( inst, stackId )) = %i",
                "(GetValueType( inst, stackId ) == VAR_STACK)",
                ValueType) )
          __debugbreak();
      }
      goto LABEL_26;
    case 0xFu:
      stackId = Scr_GetWaitThreadStackId(inst, localId, startLocalId);
      if ( !stackId
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3967, 0, "%s", "stackId") )
      {
        __debugbreak();
      }
      if ( GetValueType(inst, stackId) != 10 )
      {
        v4 = GetValueType(inst, stackId);
        if ( !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                3968,
                0,
                "%s\n\t(GetValueType( inst, stackId )) = %i",
                "(GetValueType( inst, stackId ) == VAR_STACK)",
                v4) )
          __debugbreak();
      }
LABEL_26:
      VariableValueAddress = GetVariableValueAddress(inst, stackId);
      return (char *)Scr_GetStackThreadPos(inst, localId, VariableValueAddress->u.stackValue, 0);
  }
  if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3972, 0, "unreachable") )
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

  if ( !MEMORY[0xA05AB86][116 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          3574,
          0,
          "%s",
          "gScrVarPub[inst].developer") )
  {
    __debugbreak();
  }
  startLocalId = GetStartLocalId(inst, endLocalId);
  if ( !startLocalId
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3577, 0, "%s", "startLocalId") )
  {
    __debugbreak();
  }
  size = stackValue->size;
  localId = stackValue->localId;
  buf = &stackValue->buf[5 * size];
  pos = stackValue->pos;
  while ( size )
  {
    bufa = buf - 4;
    u.intValue = *(int *)bufa;
    buf = bufa - 1;
    --size;
    if ( *buf == 7 )
    {
      parentLocalId = GetParentLocalId(inst, localId);
      if ( localId == endLocalId )
      {
        if ( startLocalId == localId
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                3609,
                0,
                "%s",
                "startLocalId != localId") )
        {
          __debugbreak();
        }
        break;
      }
      if ( killThread )
        Scr_DebugKillThread(inst, localId, pos);
      localId = parentLocalId;
      if ( !u.intValue
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
              3604,
              0,
              "%s",
              "u.codePosValue") )
      {
        __debugbreak();
      }
      pos = (const char *)u.intValue;
    }
  }
  if ( killThread )
    Scr_DebugKillThread(inst, localId, pos);
  return pos;
}

char *__cdecl Scr_GetRunningThreadPos(scriptInstance_t inst, unsigned int localId)
{
  char *pos; // [esp+0h] [ebp-Ch]
  int function_count; // [esp+4h] [ebp-8h]

  if ( !MEMORY[0xA05AB86][116 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          3742,
          0,
          "%s",
          "gScrVarPub[inst].developer") )
  {
    __debugbreak();
  }
  if ( !MEMORY[0xA05AC90][4298 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          3744,
          0,
          "%s",
          "gScrVmPub[inst].function_count") )
  {
    __debugbreak();
  }
  for ( function_count = MEMORY[0xA05AC90][4298 * inst]; function_count; --function_count )
  {
    if ( MEMORY[0xA05ACB4][4298 * inst + 6 * function_count] == localId )
    {
      pos = (char *)MEMORY[0xA05ACB0][4298 * inst + 6 * function_count];
      return &g_EndPos != pos ? pos : 0;
    }
  }
  if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3756, 0, "unreachable") )
    __debugbreak();
  return 0;
}

unsigned int __cdecl Scr_GetWaitThreadStackId(scriptInstance_t inst, unsigned int localId, unsigned int startLocalId)
{
  unsigned int Variable; // eax
  unsigned int time; // [esp+0h] [ebp-8h]
  unsigned int id; // [esp+4h] [ebp-4h]

  if ( !MEMORY[0xA05AB86][116 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          3796,
          0,
          "%s",
          "gScrVarPub[inst].developer") )
  {
    __debugbreak();
  }
  time = Scr_GetThreadWaitTime(inst, startLocalId);
  Variable = FindVariable(inst, MEMORY[0xA05AB98][29 * inst], time);
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
  varUsagePos = (const char *)MEMORY[0xA05ABE4][29 * inst];
  if ( !varUsagePos )
    MEMORY[0xA05ABE4][29 * inst] = (int)"<script notify variable>";
  if ( !MEMORY[0xA05AB98][29 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4249,
          0,
          "%s",
          "gScrVarPub[inst].timeArrayId") )
  {
    __debugbreak();
  }
  if ( paramcount > MEMORY[0xA05ACA0][4298 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4250,
          0,
          "%s",
          "paramcount <= gScrVmPub[inst].inparamcount") )
  {
    __debugbreak();
  }
  Scr_ClearOutParams(inst);
  startTop = (VariableValue *)(MEMORY[0xA05AC98][4298 * inst] - 8 * paramcount);
  paramcounta = MEMORY[0xA05ACA0][4298 * inst] - paramcount;
  id = FindEntityId(inst, entnum, classnum, clientNum);
  if ( id )
  {
    if ( dword_A0642E8[1155 * inst] )
    {
      top = (VariableValue *)MEMORY[0xA05AC98][4298 * inst];
      MEMORY[0xA05ACA0][4298 * inst] = 0;
      Scr_CheckBreakonNotify(inst, id, stringValue, (VariableValue *)MEMORY[0xA05AC98][4298 * inst], 0, 0);
      MEMORY[0xA05AC98][4298 * inst] = (int)top;
      if ( MEMORY[0xA05ACA4][4298 * inst] )
      {
        if ( !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                4269,
                0,
                "%s",
                "!gScrVmPub[inst].outparamcount") )
          __debugbreak();
      }
    }
    type = startTop->type;
    startTop->type = 8;
    MEMORY[0xA05ACA0][4298 * inst] = 0;
    VM_Notify(inst, id, stringValue, (VariableValue *)MEMORY[0xA05AC98][4298 * inst]);
    startTop->type = type;
  }
  while ( (VariableValue *)MEMORY[0xA05AC98][4298 * inst] != startTop )
  {
    RemoveRefToValue(
      inst,
      *(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] + 4),
      *(VariableUnion *)MEMORY[0xA05AC98][4298 * inst]);
    MEMORY[0xA05AC98][4298 * inst] -= 8;
  }
  MEMORY[0xA05ACA0][4298 * inst] = paramcounta;
  if ( MEMORY[0xA05ACA4][4298 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4287,
          0,
          "%s",
          "!gScrVmPub[inst].outparamcount") )
  {
    __debugbreak();
  }
  MEMORY[0xA05ABE4][29 * inst] = (int)varUsagePos;
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

  if ( inst == SCRIPTINSTANCE_SERVER && (stringValue == scr_const.death || stringValue == scr_const.disconnect) )
    VM_Notify(SCRIPTINSTANCE_SERVER, notifyListOwnerId, scr_const.death_or_disconnect, top);
  notifyListId = FindVariable(inst, notifyListOwnerId, 0x17FFEu);
  if ( notifyListId )
  {
    notifyListId = FindObject(inst, notifyListId);
    if ( !notifyListId
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4025,
            0,
            "%s",
            "notifyListId") )
    {
      __debugbreak();
    }
    notifyNameListId = FindVariable(inst, notifyListId, stringValue);
    if ( notifyNameListId )
    {
      notifyNameListId = FindObject(inst, notifyNameListId);
      if ( !notifyNameListId
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
              4032,
              0,
              "%s",
              "notifyNameListId") )
      {
        __debugbreak();
      }
      AddRefToObject(inst, notifyNameListId);
      if ( MEMORY[0xA05AB88][116 * inst]
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
              4036,
              0,
              "%s",
              "!gScrVarPub[inst].evaluate") )
      {
        __debugbreak();
      }
      MEMORY[0xA05AB88][116 * inst] = 1;
      notifyListEntry = notifyNameListId;
      while ( 1 )
      {
        notifyListIndex = FindLastSibling(inst, notifyListEntry);
next:
        if ( !notifyListIndex )
          break;
        notifyListEntry = gScrVarGlob[inst].variableList[notifyListIndex + 0x8000].hash.id;
        if ( !notifyListEntry
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                4050,
                0,
                "%s",
                "notifyListEntry") )
        {
          __debugbreak();
        }
        startLocalId = GetVariableKeyObject(inst, notifyListEntry);
        selfId = Scr_GetSelf(inst, startLocalId).nextEntId;
        ObjectVariable = FindObjectVariable(inst, MEMORY[0xA05AB9C][29 * inst], selfId);
        selfNameId = FindObject(inst, ObjectVariable);
        if ( GetValueType(inst, notifyListEntry) )
        {
          if ( GetValueType(inst, notifyListEntry) != 10
            && !Assert_MyHandler(
                  "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                  4075,
                  0,
                  "%s",
                  "GetValueType( inst, notifyListEntry ) == VAR_STACK") )
          {
            __debugbreak();
          }
          tempValue = (VariableUnion *)GetVariableValueAddress(inst, notifyListEntry);
          stackValue.intValue = tempValue->intValue;
          if ( *(_BYTE *)(*(unsigned int *)tempValue->intValue - 1) == 119 )
          {
            size = **(char **)stackValue.intValue;
            if ( size < 0
              && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    4083,
                    0,
                    "%s",
                    "size >= 0") )
            {
              __debugbreak();
            }
            if ( size > *(unsigned __int16 *)(stackValue.intValue + 4)
              && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    4084,
                    0,
                    "size <= stackValue->size\n\t%i, %i",
                    size,
                    *(unsigned __int16 *)(stackValue.intValue + 4)) )
            {
              __debugbreak();
            }
            buf = (char *)(stackValue.intValue + 5 * (*(unsigned __int16 *)(stackValue.intValue + 4) - size) + 13);
            for ( currentValue = top; size; --currentValue )
            {
              if ( currentValue->type == 7
                && !Assert_MyHandler(
                      "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                      4091,
                      0,
                      "%s",
                      "currentValue->type != VAR_CODEPOS") )
              {
                __debugbreak();
              }
              if ( currentValue->type == 8 )
              {
                notifyListIndex = gScrVarGlob[inst].variableList[notifyListIndex + 0x8000].hash.u.prev;
                goto next;
              }
              --size;
              tempValue3.type = (unsigned __int8)*buf;
              if ( tempValue3.type == 7
                && !Assert_MyHandler(
                      "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                      4100,
                      0,
                      "%s",
                      "tempValue3.type != VAR_CODEPOS") )
              {
                __debugbreak();
              }
              if ( tempValue3.type == 8 )
                break;
              tempValue3.u.intValue = *(unsigned int *)++buf;
              buf += 4;
              AddRefToValue(inst, tempValue3.type, tempValue3.u);
              type = currentValue->type;
              tempValue2.u.intValue = currentValue->u.intValue;
              tempValue2.type = type;
              AddRefToValue(inst, type, tempValue2.u);
              Scr_EvalEquality(inst, &tempValue3, &tempValue2);
              if ( MEMORY[0xA05AB8C][29 * inst] )
              {
                RuntimeError(
                  inst,
                  *(char **)stackValue.intValue,
                  **(char **)stackValue.intValue - size + 3,
                  (const char *)MEMORY[0xA05AB8C][29 * inst],
                  (const char *)dword_A0667D8[2058 * inst]);
                Scr_ClearErrorMessage(inst);
                notifyListIndex = gScrVarGlob[inst].variableList[notifyListIndex + 0x8000].hash.u.prev;
                goto next;
              }
              if ( tempValue3.type != 6
                && !Assert_MyHandler(
                      "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                      4116,
                      0,
                      "%s",
                      "tempValue3.type == VAR_INTEGER") )
              {
                __debugbreak();
              }
              if ( !tempValue3.u.intValue )
              {
                notifyListIndex = gScrVarGlob[inst].variableList[notifyListIndex + 0x8000].hash.u.prev;
                goto next;
              }
            }
            ++*(unsigned int *)stackValue.intValue;
            bNoStack = 1;
          }
          else
          {
            if ( top->type == 7
              && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    4138,
                    0,
                    "%s",
                    "top->type != VAR_CODEPOS") )
            {
              __debugbreak();
            }
            bNoStack = top->type == 8;
          }
          tempValue5.type = 10;
          tempValue5.u = stackValue;
          v8 = startLocalId;
          Variable = GetVariable(inst, MEMORY[0xA05AB98][29 * inst], MEMORY[0xA05AB94][29 * inst]);
          Array = GetArray(inst, Variable);
          stackId = GetNewObjectVariable(inst, Array, v8);
          SetNewVariableValue(inst, stackId, &tempValue5);
          tempValue = (VariableUnion *)GetVariableValueAddress(inst, stackId);
          VM_CancelNotifyInternal(inst, notifyListOwnerId, startLocalId, notifyListId, notifyNameListId, stringValue);
          RemoveObjectVariable(inst, selfNameId, startLocalId);
          if ( !GetArraySize(inst, selfNameId) )
            RemoveObjectVariable(inst, MEMORY[0xA05AB9C][29 * inst], selfId);
          Scr_SetThreadWaitTime(inst, startLocalId, MEMORY[0xA05AB94][29 * inst]);
          if ( bNoStack )
          {
            notifyListEntry = notifyNameListId;
          }
          else
          {
            if ( top->type == 8
              && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    4164,
                    0,
                    "%s\n\t(top - gScrVmPub[inst].stack) = %i",
                    "(top->type != VAR_PRECODEPOS)",
                    ((char *)top - ((char *)&MEMORY[0xA05AFB0] + 17192 * inst)) >> 3) )
            {
              __debugbreak();
            }
            if ( top->type == 7
              && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    4165,
                    0,
                    "%s\n\t(top - gScrVmPub[inst].stack) = %i",
                    "(top->type != VAR_CODEPOS)",
                    ((char *)top - ((char *)&MEMORY[0xA05AFB0] + 17192 * inst)) >> 3) )
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
              if ( currentValue->type == 7
                && !Assert_MyHandler(
                      "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                      4175,
                      0,
                      "%s\n\t(currentValue - gScrVmPub[inst].stack) = %i",
                      "(currentValue->type != VAR_CODEPOS)",
                      ((char *)currentValue - ((char *)&MEMORY[0xA05AFB0] + 17192 * inst)) >> 3) )
              {
                __debugbreak();
              }
            }
            while ( currentValue->type != 8 );
            if ( (unsigned int)newSize >= 0x10000
              && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    4178,
                    0,
                    "%s",
                    "newSize >= 0 && newSize < (1 << 16)") )
            {
              __debugbreak();
            }
            len = 5 * size;
            bufLen = 5 * newSize + 13;
            if ( !MT_Realloc(inst, *(unsigned __int16 *)(stackValue.intValue + 6), bufLen) )
            {
              newStackValue = (VariableStackBuffer *)MT_Alloc(bufLen, 1, inst);
              newStackValue->bufLen = bufLen;
              newStackValue->pos = *(const char **)stackValue.intValue;
              newStackValue->localId = *(unsigned int *)(stackValue.intValue + 8);
              memcpy((unsigned __int8 *)newStackValue->buf, (unsigned __int8 *)(stackValue.intValue + 13), len);
              MT_Free((VariableUnion)stackValue.intValue, *(unsigned __int16 *)(stackValue.intValue + 6), inst);
              stackValue.intValue = (int)newStackValue;
              tempValue->intValue = (int)newStackValue;
            }
            *(_WORD *)(stackValue.intValue + 4) = newSize;
            buf = (char *)(stackValue.intValue + len + 13);
            newSize -= size;
            if ( !newSize
              && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    4203,
                    0,
                    "%s",
                    "newSize") )
            {
              __debugbreak();
            }
            do
            {
              ++currentValue;
              AddRefToValue(inst, currentValue->type, currentValue->u);
              if ( currentValue->type >= 0x16u
                && !Assert_MyHandler(
                      "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                      4209,
                      0,
                      "%s",
                      "(unsigned)currentValue->type < VAR_COUNT") )
              {
                __debugbreak();
              }
              *buf++ = currentValue->type;
              *(unsigned int *)buf = currentValue->u.intValue;
              buf += 4;
              --newSize;
            }
            while ( newSize );
            if ( &buf[-stackValue.intValue] != (char *)bufLen
              && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                    4217,
                    0,
                    "%s",
                    "buf - (const char *)stackValue == bufLen") )
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
          if ( !GetArraySize(inst, selfNameId) )
            RemoveObjectVariable(inst, MEMORY[0xA05AB9C][29 * inst], selfId);
          Scr_TerminateThread(inst, selfId);
          notifyListEntry = notifyNameListId;
        }
      }
      RemoveRefToObject(inst, notifyNameListId);
      if ( !MEMORY[0xA05AB88][116 * inst]
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
              4223,
              0,
              "%s",
              "gScrVarPub[inst].evaluate") )
      {
        __debugbreak();
      }
      MEMORY[0xA05AB88][116 * inst] = 0;
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
  Variable = FindVariable(inst, MEMORY[0xA05AB98][29 * inst], time);
  id = FindObject(inst, Variable);
  stackId = FindObjectVariable(inst, id, startLocalId);
  if ( !stackId
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3773, 0, "%s", "stackId") )
  {
    __debugbreak();
  }
  if ( GetValueType(inst, stackId) != 10
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          3774,
          0,
          "%s",
          "GetValueType( inst, stackId ) == VAR_STACK") )
  {
    __debugbreak();
  }
  stackValue = (VariableStackBuffer *)GetVariableValueAddress(inst, stackId)->next;
  if ( MEMORY[0xA05AB86][116 * inst] )
    Scr_GetStackThreadPos(inst, localId, stackValue, 1);
  RemoveObjectVariable(inst, id, startLocalId);
  if ( !GetArraySize(inst, id) && time != MEMORY[0xA05AB94][29 * inst] )
    RemoveVariable(inst, MEMORY[0xA05AB98][29 * inst], time);
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
  const char *buf; // [esp+10h] [ebp-1Ch]
  const char *bufa; // [esp+10h] [ebp-1Ch]
  int size; // [esp+14h] [ebp-18h]
  int sizea; // [esp+14h] [ebp-18h]
  unsigned int parentLocalId; // [esp+18h] [ebp-14h]
  const char *u; // [esp+1Ch] [ebp-10h]
  VariableValue tempValue; // [esp+24h] [ebp-8h] BYREF

  if ( !startLocalId
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3497, 0, "%s", "startLocalId") )
  {
    __debugbreak();
  }
  size = stackValue->size;
  localId = stackValue->localId;
  buf = &stackValue->buf[5 * size];
  while ( size )
  {
    bufa = buf - 4;
    u = *(const char **)bufa;
    buf = bufa - 1;
    --size;
    if ( *buf == 7 )
    {
      parentLocalId = GetParentLocalId(inst, localId);
      Scr_KillThread(inst, localId);
      RemoveRefToObject(inst, localId);
      if ( localId == endLocalId )
      {
        if ( startLocalId == localId
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                3528,
                0,
                "%s",
                "startLocalId != localId") )
        {
          __debugbreak();
        }
        sizea = size + 1;
        *buf = 0;
        if ( stackValue->size < sizea
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                3533,
                0,
                "%s",
                "stackValue->size >= size") )
        {
          __debugbreak();
        }
        Scr_SetThreadWaitTime(inst, startLocalId, MEMORY[0xA05AB94][29 * inst]);
        if ( !u
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                3536,
                0,
                "%s",
                "u.codePosValue") )
        {
          __debugbreak();
        }
        stackValue->pos = u;
        stackValue->localId = parentLocalId;
        stackValue->size = sizea;
        tempValue.type = 10;
        tempValue.u.intValue = (int)stackValue;
        Variable = GetVariable(inst, MEMORY[0xA05AB98][29 * inst], MEMORY[0xA05AB94][29 * inst]);
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
  if ( localId != endLocalId
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          3549,
          0,
          "%s",
          "localId == endLocalId") )
  {
    __debugbreak();
  }
  if ( startLocalId != localId
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          3550,
          0,
          "%s",
          "startLocalId == localId") )
  {
    __debugbreak();
  }
  Scr_KillThread(inst, localId);
  RemoveRefToObject(inst, localId);
  --MEMORY[0xA05ABD8][29 * inst];
  MT_Free(stackValue, stackValue->bufLen, inst);
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
  if ( ThreadNotifyName )
  {
    selfId = Scr_GetSelf(inst, startLocalId).nextEntId;
    ObjectVariable = FindObjectVariable(inst, MEMORY[0xA05AB9C][29 * inst], selfId);
    selfNameId = FindObject(inst, ObjectVariable);
    v5 = FindObjectVariable(inst, selfNameId, startLocalId);
    notifyListOwnerId = GetVariableValueAddress(inst, v5)->next;
    Variable = FindVariable(inst, notifyListOwnerId, 0x17FFEu);
    notifyListId = FindObject(inst, Variable);
    v7 = FindVariable(inst, notifyListId, stringValue);
    notifyNameListId = FindObject(inst, v7);
    stackId = FindObjectVariable(inst, notifyNameListId, startLocalId);
    if ( !stackId
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3841, 0, "%s", "stackId") )
    {
      __debugbreak();
    }
    if ( GetValueType(inst, stackId) != 10
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            3842,
            0,
            "%s",
            "GetValueType( inst, stackId ) == VAR_STACK") )
    {
      __debugbreak();
    }
    stackValue = (VariableStackBuffer *)GetVariableValueAddress(inst, stackId)->next;
    if ( MEMORY[0xA05AB86][116 * inst] )
      Scr_GetStackThreadPos(inst, localId, stackValue, 1);
    VM_CancelNotifyInternal(inst, notifyListOwnerId, startLocalId, notifyListId, notifyNameListId, stringValue);
    RemoveObjectVariable(inst, selfNameId, startLocalId);
    if ( !GetArraySize(inst, selfNameId) )
      RemoveObjectVariable(inst, MEMORY[0xA05AB9C][29 * inst], selfId);
  }
  else
  {
    stackIda = FindVariable(inst, startLocalId, 0x17FFFu);
    if ( !stackIda
      && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3858, 0, "%s", "stackId") )
    {
      __debugbreak();
    }
    if ( GetValueType(inst, stackIda) != 10
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            3859,
            0,
            "%s",
            "GetValueType( inst, stackId ) == VAR_STACK") )
    {
      __debugbreak();
    }
    stackValue = (VariableStackBuffer *)GetVariableValueAddress(inst, stackIda)->next;
    if ( MEMORY[0xA05AB86][116 * inst] )
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
  VariableValueInternal::<unnamed_type_u> *VariableValueAddress; // eax
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
  ObjectInfo::<unnamed_type_u> selfLocalId; // [esp+24h] [ebp-4h]

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
      selfStartLocalId = GetStartLocalId(inst, selfLocalId.nextEntId);
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

  if ( !startLocalId
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 3630, 0, "%s", "startLocalId") )
  {
    __debugbreak();
  }
  size = stackValue->size;
  localId = stackValue->localId;
  buf = &stackValue->buf[5 * size];
  while ( size )
  {
    bufa = buf - 4;
    u.intValue = *(int *)bufa;
    buf = bufa - 1;
    --size;
    if ( *buf == 7 )
    {
      if ( FindObjectVariable(inst, MEMORY[0xA05AB9C][29 * inst], localId) )
      {
        if ( startLocalId == localId
          && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
                3654,
                0,
                "%s",
                "startLocalId != localId") )
        {
          __debugbreak();
        }
        stackValue->localId = localId;
        stackValue->size = size + 1;
        Scr_StopThread(inst, localId);
        if ( !fromEndon )
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
  if ( startLocalId != localId
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          3683,
          0,
          "%s",
          "startLocalId == localId") )
  {
    __debugbreak();
  }
  if ( fromEndon )
    RemoveVariable(inst, startLocalId, 0x17FFFu);
  Scr_KillThread(inst, startLocalId);
  RemoveRefToObject(inst, startLocalId);
  --MEMORY[0xA05ABD8][29 * inst];
  MT_Free(stackValue, stackValue->bufLen, inst);
}

void __cdecl Scr_CancelWaittill(scriptInstance_t inst, unsigned int startLocalId)
{
  unsigned int ObjectVariable; // eax
  unsigned int v3; // eax
  VariableValueInternal::<unnamed_type_u> *VariableValueAddress; // eax
  unsigned int selfNameId; // [esp+0h] [ebp-Ch]
  unsigned int selfId; // [esp+8h] [ebp-4h]

  selfId = Scr_GetSelf(inst, startLocalId).nextEntId;
  ObjectVariable = FindObjectVariable(inst, MEMORY[0xA05AB9C][29 * inst], selfId);
  selfNameId = FindObject(inst, ObjectVariable);
  v3 = FindObjectVariable(inst, selfNameId, startLocalId);
  VariableValueAddress = GetVariableValueAddress(inst, v3);
  VM_CancelNotify(inst, VariableValueAddress->next, startLocalId);
  RemoveObjectVariable(inst, selfNameId, startLocalId);
  if ( !GetArraySize(inst, selfNameId) )
    RemoveObjectVariable(inst, MEMORY[0xA05AB9C][29 * inst], selfId);
}

unsigned __int16 __cdecl Scr_ExecThread(scriptInstance_t inst, int handle, unsigned int paramcount)
{
  unsigned int v3; // eax
  char *pos; // [esp+14h] [ebp-Ch]
  const char *varUsagePos; // [esp+18h] [ebp-8h]
  unsigned int id; // [esp+1Ch] [ebp-4h]

  pos = (char *)&gScrVarPub[inst].programBuffer[handle];
  _mm_prefetch(pos, 1);
  if ( !gScrVmPub[inst].function_count )
  {
    if ( (int *)gScrVmPub[inst].localVars != &dword_A0667E0[2058 * inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4583,
            0,
            "%s",
            "gScrVmPub[inst].localVars == gScrVmGlob[inst].localVarsStack - 1") )
    {
      __debugbreak();
    }
    Scr_ResetTimeout(inst);
  }
  if ( !gScrVarPub[inst].timeArrayId
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4589,
          0,
          "%s",
          "gScrVarPub[inst].timeArrayId") )
  {
    __debugbreak();
  }
  if ( !handle
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 4590, 0, "%s", "handle") )
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
  if ( gScrVarDebugPub[inst] )
    ++gScrVarDebugPub[inst]->extRefCount[id];
  RemoveRefToValue(inst, gScrVmPub[inst].top->type, gScrVmPub[inst].top->u);
  gScrVmPub[inst].top->type = 0;
  --gScrVmPub[inst].top;
  --gScrVmPub[inst].inparamcount;
  if ( !gScrVmPub[inst].function_count
    && (int *)gScrVmPub[inst].localVars != &dword_A0667E0[2058 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4624,
          0,
          "%s",
          "gScrVmPub[inst].localVars == gScrVmGlob[inst].localVarsStack - 1") )
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

  if ( paramcount > MEMORY[0xA05ACA0][4298 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4494,
          0,
          "%s",
          "paramcount <= gScrVmPub[inst].inparamcount") )
  {
    __debugbreak();
  }
  Scr_ClearOutParams(inst);
  startTop = (VariableValue *)(MEMORY[0xA05AC98][4298 * inst] - 8 * paramcount);
  paramcounta = MEMORY[0xA05ACA0][4298 * inst] - paramcount;
  if ( MEMORY[0xA05AC90][4298 * inst] >= 30 )
  {
    Scr_KillThread(inst, localId);
    MEMORY[0xA05ACA0][4298 * inst] = paramcounta + 1;
    if ( MEMORY[0xA05ACA4][4298 * inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4552,
            0,
            "%s",
            "!gScrVmPub[inst].outparamcount") )
    {
      __debugbreak();
    }
    while ( paramcounta )
    {
      RemoveRefToValue(
        inst,
        *(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] + 4),
        *(VariableUnion *)MEMORY[0xA05AC98][4298 * inst]);
      MEMORY[0xA05AC98][4298 * inst] -= 8;
      --paramcounta;
    }
    *(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] + 12) = 0;
    MEMORY[0xA05AC98][4298 * inst] += 8;
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
    if ( MEMORY[0xA05AC90][4298 * inst] )
    {
      ++MEMORY[0xA05AC90][4298 * inst];
      MEMORY[0xA05AC94][4298 * inst] += 24;
      *(unsigned int *)(MEMORY[0xA05AC94][4298 * inst] + 4) = 0;
    }
    *(unsigned int *)MEMORY[0xA05AC94][4298 * inst] = pos;
    ++MEMORY[0xA05AC90][4298 * inst];
    MEMORY[0xA05AC94][4298 * inst] += 24;
    *(unsigned int *)(MEMORY[0xA05AC94][4298 * inst] + 4) = localId;
    type = startTop->type;
    startTop->type = 8;
    MEMORY[0xA05ACA0][4298 * inst] = 0;
    gFs[inst].top = (VariableValue *)MEMORY[0xA05AC98][4298 * inst];
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
    MEMORY[0xA05AC98][4298 * inst] = (int)&startTop[1];
    MEMORY[0xA05ACA0][4298 * inst] = paramcounta + 1;
    if ( MEMORY[0xA05ACA4][4298 * inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4537,
            0,
            "%s",
            "!gScrVmPub[inst].outparamcount") )
    {
      __debugbreak();
    }
    ClearVariableValue(inst, MEMORY[0xA05ABB0][29 * inst]);
    if ( MEMORY[0xA05AC90][4298 * inst] )
    {
      --MEMORY[0xA05AC90][4298 * inst];
      MEMORY[0xA05AC94][4298 * inst] -= 24;
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
  ClearVariableValue(inst, MEMORY[0xA05ABB0][29 * inst]);
  return GetObject(inst, MEMORY[0xA05ABB0][29 * inst]);
}

int __cdecl GetDummyFieldValue(scriptInstance_t inst)
{
  ClearVariableValue(inst, MEMORY[0xA05ABB0][29 * inst]);
  return MEMORY[0xA05ABB0][29 * inst];
}

void __cdecl VM_PrintJumpHistory(scriptInstance_t inst)
{
  char *pos; // [esp+0h] [ebp-8h]
  int index; // [esp+4h] [ebp-4h]

  Com_Printf(24, "********************************\n");
  Com_Printf(24, "Recent loop history (from most recent) :\n");
  index = dword_A0644EC[1155 * inst];
  do
  {
    if ( !index )
      index = 128;
    --index;
    pos = (char *)dword_A0642EC[1155 * inst + index];
    if ( !pos )
      break;
    Scr_PrintPrevCodePos(inst, 24, pos, 0);
  }
  while ( index != dword_A0644EC[1155 * inst] );
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
  if ( size != (unsigned __int16)size
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          3333,
          0,
          "%s",
          "size == (unsigned short)size") )
  {
    __debugbreak();
  }
  bufLen = 5 * size + 13;
  if ( bufLen != (unsigned __int16)bufLen
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          3335,
          0,
          "%s",
          "bufLen == (unsigned short)bufLen") )
  {
    __debugbreak();
  }
  stackValue = MT_Alloc(bufLen, 1, inst);
  ++MEMORY[0xA05ABD8][29 * inst];
  localId = stack->localId;
  *((unsigned int *)stackValue + 2) = localId;
  *((_WORD *)stackValue + 2) = size;
  *((_WORD *)stackValue + 3) = bufLen;
  *(unsigned int *)stackValue = stack->pos;
  stackValue[12] = MEMORY[0xA05AB94][29 * inst];
  gScrVmPub[inst].localVars -= stack->localVarCount;
  buf = &stackValue[5 * size + 13];
  while ( size )
  {
    bufa = buf - 4;
    if ( top->type == 7 )
    {
      --MEMORY[0xA05AC90][4298 * inst];
      MEMORY[0xA05AC94][4298 * inst] -= 24;
      *(unsigned int *)bufa = *(unsigned int *)MEMORY[0xA05AC94][4298 * inst];
      gScrVmPub[inst].localVars -= *(unsigned int *)(MEMORY[0xA05AC94][4298 * inst] + 8);
      localId = GetParentLocalId(inst, localId);
    }
    else
    {
      *(unsigned int *)bufa = top->u.intValue;
    }
    buf = bufa - 1;
    if ( top->type >= 0x100u
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            3370,
            0,
            "%s",
            "top->type >= 0 && top->type < (1 << 8)") )
    {
      __debugbreak();
    }
    *buf = top->type;
    --top;
    --size;
  }
  --MEMORY[0xA05AC90][4298 * inst];
  MEMORY[0xA05AC94][4298 * inst] -= 24;
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

  pos = (char *)(handle + MEMORY[0xA05ABC8][29 * inst]);
  _mm_prefetch(pos, 1);
  if ( !MEMORY[0xA05AC90][4298 * inst] )
  {
    if ( (int *)gScrVmPub[inst].localVars != &dword_A0667E0[2058 * inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4646,
            0,
            "%s",
            "gScrVmPub[inst].localVars == gScrVmGlob[inst].localVarsStack - 1") )
    {
      __debugbreak();
    }
    Scr_ResetTimeout(inst);
  }
  if ( !MEMORY[0xA05AB98][29 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4652,
          0,
          "%s",
          "gScrVarPub[inst].timeArrayId") )
  {
    __debugbreak();
  }
  if ( !handle
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 4653, 0, "%s", "handle") )
  {
    __debugbreak();
  }
  ++MEMORY[0xA05ABE8][29 * inst];
  if ( !Scr_IsInOpcodeMemory(inst, pos)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4659,
          0,
          "%s",
          "Scr_IsInOpcodeMemory( inst, pos )") )
  {
    __debugbreak();
  }
  varUsagePos = (const char *)MEMORY[0xA05ABE4][29 * inst];
  MEMORY[0xA05ABE4][29 * inst] = (int)(pos + 1);
  objId = Scr_GetEntityId(inst, entnum, classnum, localClientNum);
  AddRefToObject(inst, objId);
  v6 = AllocThread(inst, objId);
  id = VM_Execute(inst, v6, pos, paramcount);
  MEMORY[0xA05ABE4][29 * inst] = (int)varUsagePos;
  if ( gScrVarDebugPub[inst] )
    ++gScrVarDebugPub[inst]->extRefCount[id];
  RemoveRefToValue(
    inst,
    *(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] + 4),
    *(VariableUnion *)MEMORY[0xA05AC98][4298 * inst]);
  *(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] + 4) = 0;
  MEMORY[0xA05AC98][4298 * inst] -= 8;
  --MEMORY[0xA05ACA0][4298 * inst];
  if ( !MEMORY[0xA05AC90][4298 * inst]
    && (int *)gScrVmPub[inst].localVars != &dword_A0667E0[2058 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4688,
          0,
          "%s",
          "gScrVmPub[inst].localVars == gScrVmGlob[inst].localVarsStack - 1") )
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

  pos = (char *)(handle + MEMORY[0xA05ABC8][29 * inst]);
  _mm_prefetch(pos, 1);
  if ( !MEMORY[0xA05AC90][4298 * inst] )
  {
    if ( (int *)gScrVmPub[inst].localVars != &dword_A0667E0[2058 * inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4708,
            0,
            "%s",
            "gScrVmPub[inst].localVars == gScrVmGlob[inst].localVarsStack - 1") )
    {
      __debugbreak();
    }
    Scr_ResetTimeout(inst);
  }
  if ( !MEMORY[0xA05AB98][29 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4714,
          0,
          "%s",
          "gScrVarPub[inst].timeArrayId") )
  {
    __debugbreak();
  }
  if ( !handle
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 4715, 0, "%s", "handle") )
  {
    __debugbreak();
  }
  if ( !Scr_IsInOpcodeMemory(inst, pos)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4717,
          0,
          "%s",
          "Scr_IsInOpcodeMemory( inst, pos )") )
  {
    __debugbreak();
  }
  varUsagePos = (const char *)MEMORY[0xA05ABE4][29 * inst];
  MEMORY[0xA05ABE4][29 * inst] = (int)(pos + 1);
  AddRefToObject(inst, MEMORY[0xA05ABA0][29 * inst]);
  v3 = AllocThread(inst, MEMORY[0xA05ABA0][29 * inst]);
  v4 = VM_Execute(inst, v3, pos, paramcount);
  RemoveRefToObject(inst, v4);
  MEMORY[0xA05ABE4][29 * inst] = (int)varUsagePos;
  ++MEMORY[0xA05ACA4][4298 * inst];
  --MEMORY[0xA05ACA0][4298 * inst];
  if ( !MEMORY[0xA05AC90][4298 * inst]
    && (int *)gScrVmPub[inst].localVars != &dword_A0667E0[2058 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4738,
          0,
          "%s",
          "gScrVmPub[inst].localVars == gScrVmGlob[inst].localVarsStack - 1") )
  {
    __debugbreak();
  }
}

void __cdecl Scr_FreeThread(unsigned __int16 handle, scriptInstance_t inst)
{
  if ( !MEMORY[0xA05AB98][29 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4795,
          0,
          "%s",
          "gScrVarPub[inst].timeArrayId") )
  {
    __debugbreak();
  }
  if ( !handle
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 4796, 0, "%s", "handle") )
  {
    __debugbreak();
  }
  if ( gScrVarDebugPub[inst] )
    --gScrVarDebugPub[inst]->extRefCount[handle];
  RemoveRefToObject(inst, handle);
  --MEMORY[0xA05ABE8][29 * inst];
}

void __cdecl Scr_ExecCode(scriptInstance_t inst, char *pos, unsigned int localId)
{
  unsigned int localIda; // [esp+14h] [ebp+10h]

  Scr_ResetTimeout(inst);
  if ( !MEMORY[0xA05AB98][29 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4815,
          0,
          "%s",
          "gScrVarPub[inst].timeArrayId") )
  {
    __debugbreak();
  }
  if ( MEMORY[0xA05ACA0][4298 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4816,
          0,
          "%s",
          "!gScrVmPub[inst].inparamcount") )
  {
    __debugbreak();
  }
  if ( MEMORY[0xA05ACA4][4298 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4817,
          0,
          "%s",
          "!gScrVmPub[inst].outparamcount") )
  {
    __debugbreak();
  }
  if ( MEMORY[0xA05AB88][116 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4819,
          0,
          "%s",
          "!gScrVarPub[inst].evaluate") )
  {
    __debugbreak();
  }
  if ( MEMORY[0xA05AC9C][17192 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4821,
          0,
          "%s",
          "!gScrVmPub[inst].debugCode") )
  {
    __debugbreak();
  }
  MEMORY[0xA05AC9C][17192 * inst] = 1;
  if ( localId )
  {
    VM_Execute(inst, localId, pos, 0);
  }
  else
  {
    AddRefToObject(inst, MEMORY[0xA05ABA0][29 * inst]);
    localIda = AllocThread(inst, MEMORY[0xA05ABA0][29 * inst]);
    VM_Execute(inst, localIda, pos, 0);
    Scr_KillThread(inst, localIda);
    RemoveRefToObject(inst, localIda);
  }
  if ( !MEMORY[0xA05AC9C][17192 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4837,
          0,
          "%s",
          "gScrVmPub[inst].debugCode") )
  {
    __debugbreak();
  }
  MEMORY[0xA05AC9C][17192 * inst] = 0;
  if ( MEMORY[0xA05ACA0][4298 * inst] != 1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4840,
          0,
          "%s",
          "gScrVmPub[inst].inparamcount == 1") )
  {
    __debugbreak();
  }
  if ( MEMORY[0xA05ACA4][4298 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4841,
          0,
          "%s",
          "!gScrVmPub[inst].outparamcount") )
  {
    __debugbreak();
  }
  if ( MEMORY[0xA05AC90][4298 * inst] )
  {
    --MEMORY[0xA05AC90][4298 * inst];
    MEMORY[0xA05AC94][4298 * inst] -= 24;
  }
  MEMORY[0xA05AC98][4298 * inst] -= 8;
  MEMORY[0xA05ACA0][4298 * inst] = 0;
}

bool __cdecl Scr_IsSystemInitied(scriptInstance_t inst)
{
  return MEMORY[0xA05AB98][29 * inst] != 0;
}

void __cdecl Scr_InitSystem(scriptInstance_t inst, int sys)
{
  if ( sys != 1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4885,
          0,
          "%s",
          "sys == SCR_SYS_GAME") )
  {
    __debugbreak();
  }
  if ( MEMORY[0xA05AB98][29 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4886,
          0,
          "%s",
          "!gScrVarPub[inst].timeArrayId") )
  {
    __debugbreak();
  }
  if ( MEMORY[0xA05ABE8][29 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4888,
          0,
          "%s",
          "!gScrVarPub[inst].ext_threadcount") )
  {
    __debugbreak();
  }
  if ( MEMORY[0xA05ABE4][29 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4892,
          0,
          "%s",
          "!gScrVarPub[inst].varUsagePos") )
  {
    __debugbreak();
  }
  MEMORY[0xA05ABE4][29 * inst] = (int)"<script init variable>";
  MEMORY[0xA05AB98][29 * inst] = AllocObject(inst);
  if ( gScrVarDebugPub[inst] )
    ++gScrVarDebugPub[inst]->extRefCount[MEMORY[0xA05AB98][29 * inst]];
  if ( MEMORY[0xA05AB9C][29 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4902,
          0,
          "%s",
          "!gScrVarPub[inst].pauseArrayId") )
  {
    __debugbreak();
  }
  MEMORY[0xA05AB9C][29 * inst] = Scr_AllocArray(inst);
  if ( gScrVarDebugPub[inst] )
    ++gScrVarDebugPub[inst]->extRefCount[MEMORY[0xA05AB9C][29 * inst]];
  if ( MEMORY[0xA05ABA0][29 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4909,
          0,
          "%s",
          "!gScrVarPub[inst].levelId") )
  {
    __debugbreak();
  }
  MEMORY[0xA05ABA0][29 * inst] = AllocObject(inst);
  if ( gScrVarDebugPub[inst] )
    ++gScrVarDebugPub[inst]->extRefCount[MEMORY[0xA05ABA0][29 * inst]];
  if ( MEMORY[0xA05ABA8][29 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4916,
          0,
          "%s",
          "!gScrVarPub[inst].animId") )
  {
    __debugbreak();
  }
  MEMORY[0xA05ABA8][29 * inst] = AllocObject(inst);
  if ( gScrVarDebugPub[inst] )
    ++gScrVarDebugPub[inst]->extRefCount[MEMORY[0xA05ABA8][29 * inst]];
  if ( MEMORY[0xA05ABAC][29 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4923,
          0,
          "%s",
          "!gScrVarPub[inst].freeEntList") )
  {
    __debugbreak();
  }
  MEMORY[0xA05AB94][29 * inst] = 0;
  g_script_error_level[inst] = -1;
  if ( inst == SCRIPTINSTANCE_SERVER )
    Scr_InitDebuggerSystem(SCRIPTINSTANCE_SERVER);
  MEMORY[0xA05ABE4][29 * inst] = 0;
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

  if ( sys != 1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4946,
          0,
          "%s",
          "sys == SCR_SYS_GAME") )
  {
    __debugbreak();
  }
  MEMORY[0xA05ABE4][29 * inst] = (int)"<script shutdown variable>";
  Scr_ShutdownDebuggerSystem(inst);
  Scr_FreeEntityList(inst);
  if ( MEMORY[0xA05AB98][29 * inst] )
  {
    Scr_FreeGameVariable(inst, bComplete);
    function_count = MEMORY[0xA05AC90][4298 * inst];
    MEMORY[0xA05AC90][4298 * inst] = 0;
    for ( id = FindFirstSibling(inst, MEMORY[0xA05AB98][29 * inst]); id; id = FindNextSibling(inst, id) )
    {
      Object = FindObject(inst, id);
      VM_TerminateTime(inst, Object);
    }
    while ( 1 )
    {
      ida = FindFirstSibling(inst, MEMORY[0xA05AB9C][29 * inst]);
      if ( !ida )
        break;
      v4 = FindObject(inst, ida);
      idb = FindFirstSibling(inst, v4);
      if ( !idb
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 4980, 0, "%s", "id") )
      {
        __debugbreak();
      }
      parentId = GetVariableValueAddress(inst, idb)->next;
      AddRefToObject(inst, parentId);
      Scr_CancelNotifyList(inst, parentId);
      RemoveRefToObject(inst, parentId);
    }
    if ( !MEMORY[0xA05ABA0][29 * inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4987,
            0,
            "%s",
            "gScrVarPub[inst].levelId") )
    {
      __debugbreak();
    }
    ClearObject(inst, MEMORY[0xA05ABA0][29 * inst]);
    if ( gScrVarDebugPub[inst] )
      --gScrVarDebugPub[inst]->extRefCount[MEMORY[0xA05ABA0][29 * inst]];
    RemoveRefToEmptyObject(inst, MEMORY[0xA05ABA0][29 * inst]);
    MEMORY[0xA05ABA0][29 * inst] = 0;
    if ( !MEMORY[0xA05ABA8][29 * inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4996,
            0,
            "%s",
            "gScrVarPub[inst].animId") )
    {
      __debugbreak();
    }
    ClearObject(inst, MEMORY[0xA05ABA8][29 * inst]);
    if ( gScrVarDebugPub[inst] )
      --gScrVarDebugPub[inst]->extRefCount[MEMORY[0xA05ABA8][29 * inst]];
    RemoveRefToEmptyObject(inst, MEMORY[0xA05ABA8][29 * inst]);
    MEMORY[0xA05ABA8][29 * inst] = 0;
    if ( !MEMORY[0xA05AB98][29 * inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5005,
            0,
            "%s",
            "gScrVarPub[inst].timeArrayId") )
    {
      __debugbreak();
    }
    ClearObject(inst, MEMORY[0xA05AB98][29 * inst]);
    if ( gScrVarDebugPub[inst] )
      --gScrVarDebugPub[inst]->extRefCount[MEMORY[0xA05AB98][29 * inst]];
    RemoveRefToEmptyObject(inst, MEMORY[0xA05AB98][29 * inst]);
    MEMORY[0xA05AB98][29 * inst] = 0;
    if ( !MEMORY[0xA05AB9C][29 * inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5014,
            0,
            "%s",
            "gScrVarPub[inst].pauseArrayId") )
    {
      __debugbreak();
    }
    if ( GetArraySize(inst, MEMORY[0xA05AB9C][29 * inst])
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5015,
            0,
            "%s",
            "!GetArraySize( inst, gScrVarPub[inst].pauseArrayId )") )
    {
      __debugbreak();
    }
    if ( gScrVarDebugPub[inst] )
      --gScrVarDebugPub[inst]->extRefCount[MEMORY[0xA05AB9C][29 * inst]];
    RemoveRefToEmptyObject(inst, MEMORY[0xA05AB9C][29 * inst]);
    MEMORY[0xA05AB9C][29 * inst] = 0;
    if ( MEMORY[0xA05ABAC][29 * inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5023,
            0,
            "%s",
            "!gScrVarPub[inst].freeEntList") )
    {
      __debugbreak();
    }
    Scr_FreeObjects(inst);
    MEMORY[0xA05ABE4][29 * inst] = 0;
    if ( function_count )
    {
      MEMORY[0xA05ABB4][116 * inst] = 0;
      Scr_Init(inst);
      if ( gScrStringDebugGlob[inst] )
        gScrStringDebugGlob[inst]->ignoreLeaks = 1;
    }
    else
    {
      if ( MEMORY[0xA05ABE8][29 * inst]
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
              5034,
              0,
              "%s",
              "!gScrVarPub[inst].ext_threadcount") )
      {
        __debugbreak();
      }
      if ( !Scr_IsStackClear(inst)
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
              5036,
              0,
              "%s",
              "Scr_IsStackClear(inst)") )
      {
        __debugbreak();
      }
    }
  }
  else
  {
    MEMORY[0xA05ABE4][29 * inst] = 0;
  }
}

void __cdecl VM_TerminateTime(scriptInstance_t inst, unsigned int timeId)
{
  VariableStackBuffer *stackValue; // [esp+8h] [ebp-Ch]
  unsigned int stackId; // [esp+Ch] [ebp-8h]
  unsigned int startLocalId; // [esp+10h] [ebp-4h]

  if ( !timeId
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 4382, 0, "%s", "timeId") )
  {
    __debugbreak();
  }
  if ( MEMORY[0xA05AC90][4298 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4383,
          0,
          "%s",
          "!gScrVmPub[inst].function_count") )
  {
    __debugbreak();
  }
  AddRefToObject(inst, timeId);
  while ( 1 )
  {
    stackId = FindFirstSibling(inst, timeId);
    if ( !stackId )
      break;
    startLocalId = GetVariableKeyObject(inst, stackId);
    if ( !startLocalId
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4392,
            0,
            "%s",
            "startLocalId") )
    {
      __debugbreak();
    }
    if ( GetValueType(inst, stackId) != 10
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4394,
            0,
            "%s",
            "GetValueType( inst, stackId ) == VAR_STACK") )
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
  return MEMORY[0xA05AB98][29 * inst] && !MEMORY[0xA05AB8C][29 * inst];
}

VariableUnion __cdecl Scr_GetInt(unsigned int index, scriptInstance_t inst)
{
  const char *v3; // eax
  const char *v4; // eax
  VariableValue *value; // [esp+0h] [ebp-4h]

  if ( index < MEMORY[0xA05ACA4][4298 * inst] )
  {
    value = (VariableValue *)(MEMORY[0xA05AC98][4298 * inst] - 8 * index);
    if ( value->type == 6 )
      return value->u;
    MEMORY[0xA05AB90][29 * inst] = index + 1;
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

  if ( index < MEMORY[0xA05ACA4][4298 * inst] )
  {
    value = (VariableValue *)(MEMORY[0xA05AC98][4298 * inst] - 8 * index);
    if ( value->type == 11 )
      return (scr_anim_s)value->u.intValue;
    MEMORY[0xA05AB8C][29 * inst] = (int)va("type %s is not an anim", var_typename[value->type]);
    RemoveRefToValue(inst, value->type, value->u);
    value->type = 0;
    MEMORY[0xA05AB90][29 * inst] = index + 1;
    Scr_ErrorInternal(inst);
  }
  v4 = va("parameter %d does not exist", index + 1);
  Scr_Error(inst, v4, 0);
  return 0;
}

void __cdecl Scr_ErrorInternal(scriptInstance_t inst)
{
  if ( !MEMORY[0xA05AB8C][29 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          666,
          0,
          "%s",
          "gScrVarPub[inst].error_message") )
  {
    __debugbreak();
  }
  if ( !MEMORY[0xA05AB88][116 * inst] && !MEMORY[0x9D16AAC][135224 * inst] )
  {
    if ( MEMORY[0xA05AC90][4298 * inst] || MEMORY[0xA05AC9C][17192 * inst] )
    {
      Com_PrintMessage(6, "throwing script exception: ", 0);
      Com_PrintMessage(6, (const char *)MEMORY[0xA05AB8C][29 * inst], 0);
      Com_PrintMessage(6, "\n", 0);
      if ( (unsigned int)g_script_error_level[inst] >= 0x21
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
              703,
              0,
              "g_script_error_level[inst] doesn't index ARRAY_COUNT( g_script_error[inst] )\n\t%i not in [0, %i)",
              g_script_error_level[inst],
              33) )
      {
        __debugbreak();
      }
      longjmp(g_script_error[inst][g_script_error_level[inst]], -1);
    }
error_2:
    Sys_Error("%s", (const char *)MEMORY[0xA05AB8C][29 * inst]);
  }
  if ( MEMORY[0xA05AC9E][17192 * inst] )
    goto error_2;
}

scr_animtree_t __cdecl Scr_GetAnimTree(unsigned int index, unsigned int user, scriptInstance_t inst)
{
  const char *v4; // eax
  unsigned int i; // [esp+8h] [ebp-8h]
  VariableValue *value; // [esp+Ch] [ebp-4h]

  if ( index < MEMORY[0xA05ACA4][4298 * inst] )
  {
    value = (VariableValue *)(MEMORY[0xA05AC98][4298 * inst] - 8 * index);
    if ( value->type == 6 )
    {
      i = value->u.intValue;
      if ( value->u.intValue <= (unsigned int)MEMORY[0x9CF663C][263 * inst + user]
        && MEMORY[0x9CF623C][263 * inst + 128 * user + i] )
      {
        return (scr_animtree_t)MEMORY[0x9CF623C][263 * inst + 128 * user + i];
      }
      MEMORY[0xA05AB8C][29 * inst] = (int)"bad anim tree";
    }
    else
    {
      MEMORY[0xA05AB8C][29 * inst] = (int)va("type %s is not an animtree", var_typename[value->type]);
    }
    RemoveRefToValue(inst, value->type, value->u);
    value->type = 0;
    MEMORY[0xA05AB90][29 * inst] = index + 1;
    Scr_ErrorInternal(inst);
  }
  v4 = va("parameter %d does not exist", index + 1);
  Scr_Error(inst, v4, 0);
  return (scr_animtree_t)MEMORY[0x9CF623C][263 * inst + 128 * user];
}

double __cdecl Scr_GetFloat(unsigned int index, scriptInstance_t inst)
{
  const char *v3; // eax
  const char *v4; // eax
  VariableValue *value; // [esp+0h] [ebp-4h]

  if ( index < MEMORY[0xA05ACA4][4298 * inst] )
  {
    value = (VariableValue *)(MEMORY[0xA05AC98][4298 * inst] - 8 * index);
    if ( value->type == 5 )
      return value->u.floatValue;
    if ( value->type == 6 )
      return (double)value->u.intValue;
    MEMORY[0xA05AB90][29 * inst] = index + 1;
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

  if ( index >= MEMORY[0xA05ACA4][4298 * inst] )
    goto LABEL_8;
  value = (VariableValue *)(MEMORY[0xA05AC98][4298 * inst] - 8 * index);
  if ( !Scr_CastString(inst, value) )
  {
    MEMORY[0xA05AB90][29 * inst] = index + 1;
    Scr_ErrorInternal(inst);
LABEL_8:
    v3 = va("parameter %d does not exist", index + 1);
    Scr_Error(inst, v3, 0);
    return 0;
  }
  if ( value->type != 2
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5172,
          0,
          "%s",
          "value->type == VAR_STRING") )
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

  if ( index >= MEMORY[0xA05ACA4][4298 * inst] )
    goto LABEL_14;
  value = (VariableValue *)(MEMORY[0xA05AC98][4298 * inst] - 8 * index);
  if ( !Scr_CastString(inst, value) )
  {
    MEMORY[0xA05AB90][29 * inst] = index + 1;
    Scr_ErrorInternal(inst);
LABEL_14:
    v4 = va("parameter %d does not exist", index + 1);
    Scr_Error(inst, v4, 0);
    return 0;
  }
  if ( value->type != 2
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5202,
          0,
          "%s",
          "value->type == VAR_STRING") )
  {
    __debugbreak();
  }
  stringValue.intValue = (int)value->u;
  v5 = SL_ConvertToString(value->u.intValue, inst);
  for ( i = 0; ; ++i )
  {
    str[i] = tolower(v5[i]);
    if ( !v5[i] )
      break;
  }
  if ( value->type != 2
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5213,
          0,
          "%s",
          "value->type == VAR_STRING") )
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
  if ( index >= MEMORY[0xA05ACA4][4298 * inst] || *(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] - 8 * index + 4) )
    return Scr_GetConstString(index, inst);
  return 0;
}

char *__cdecl Scr_GetDebugString(unsigned int index, scriptInstance_t inst)
{
  const char *v3; // eax
  VariableValue *value; // [esp+0h] [ebp-4h]

  if ( index >= MEMORY[0xA05ACA4][4298 * inst] )
  {
    v3 = va("parameter %d does not exist", index + 1);
    Scr_Error(inst, v3, 0);
    return 0;
  }
  else
  {
    value = (VariableValue *)(MEMORY[0xA05AC98][4298 * inst] - 8 * index);
    Scr_CastDebugString(inst, value);
    if ( value->type != 2
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5260,
            0,
            "%s",
            "value->type == VAR_STRING") )
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

  if ( index < MEMORY[0xA05ACA4][4298 * inst] )
  {
    value = (VariableValue *)(MEMORY[0xA05AC98][4298 * inst] - 8 * index);
    if ( value->type == 3 )
      return value->u;
    MEMORY[0xA05AB90][29 * inst] = index + 1;
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

  if ( index < MEMORY[0xA05ACA4][4298 * inst] )
  {
    value = (VariableValue *)(MEMORY[0xA05AC98][4298 * inst] - 8 * index);
    if ( value->type == 4 )
    {
      intValue = (float *)value->u.intValue;
      *vectorValue = *(float *)value->u.intValue;
      vectorValue[1] = intValue[1];
      vectorValue[2] = intValue[2];
      return;
    }
    MEMORY[0xA05AB90][29 * inst] = index + 1;
    v3 = va("type %s is not a vector", var_typename[value->type]);
    Scr_Error(inst, v3, 0);
  }
  v4 = va("parameter %d does not exist", index + 1);
  Scr_Error(inst, v4, 0);
}

int __cdecl Scr_GetFunc(unsigned int index, scriptInstance_t inst)
{
  const char *v3; // eax
  const char *v4; // eax
  VariableValue *value; // [esp+0h] [ebp-4h]

  if ( index >= MEMORY[0xA05ACA4][4298 * inst] )
    goto LABEL_8;
  value = (VariableValue *)(MEMORY[0xA05AC98][4298 * inst] - 8 * index);
  if ( value->type != 9 )
  {
    MEMORY[0xA05AB90][29 * inst] = index + 1;
    v3 = va("type %s is not a function", var_typename[value->type]);
    Scr_Error(inst, v3, 0);
LABEL_8:
    v4 = va("parameter %d does not exist", index + 1);
    Scr_Error(inst, v4, 0);
    return 0;
  }
  if ( !Scr_IsInOpcodeMemory(inst, value->u.codePosValue)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5319,
          0,
          "%s",
          "Scr_IsInOpcodeMemory( inst, value->u.codePosValue )") )
  {
    __debugbreak();
  }
  return value->u.intValue - MEMORY[0xA05ABC8][29 * inst];
}

scr_entref_t *__cdecl Scr_GetEntityRef(scr_entref_t *__return_ptr retstr, unsigned int index, scriptInstance_t inst)
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

  if ( index < MEMORY[0xA05ACA4][4298 * inst] )
  {
    value = (VariableValue *)(MEMORY[0xA05AC98][4298 * inst] - 8 * index);
    if ( value->type == 1 )
    {
      id = value->u.intValue;
      if ( GetObjectType(inst, id) == 19 )
      {
        v9 = *Scr_GetEntityIdRef(&v8, inst, id);
        *retstr = v9;
        return retstr;
      }
      MEMORY[0xA05AB90][29 * inst] = index + 1;
      ObjectType = GetObjectType(inst, id);
      v5 = va("type %s is not an entity", var_typename[ObjectType]);
      Scr_Error(inst, v5, 0);
    }
    MEMORY[0xA05AB90][29 * inst] = index + 1;
    v6 = va("type %s is not an entity", var_typename[value->type]);
    Scr_Error(inst, v6, 0);
  }
  v7 = va("parameter %d does not exist", index + 1);
  Scr_Error(inst, v7, 0);
  *(unsigned int *)&retstr->entnum = 0;
  retstr->client = entref.client;
  return retstr;
}

VariableUnion __cdecl Scr_GetObject(unsigned int index, scriptInstance_t inst)
{
  const char *v3; // eax
  const char *v4; // eax
  VariableValue *value; // [esp+0h] [ebp-4h]

  if ( index < MEMORY[0xA05ACA4][4298 * inst] )
  {
    value = (VariableValue *)(MEMORY[0xA05AC98][4298 * inst] - 8 * index);
    if ( value->type == 1 )
      return value->u;
    MEMORY[0xA05AB90][29 * inst] = index + 1;
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

  if ( index < MEMORY[0xA05ACA4][4298 * inst] )
    return *(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] - 8 * index + 4);
  v3 = va("parameter %d does not exist", index + 1);
  Scr_Error(inst, v3, 0);
  return 0;
}

const char *__cdecl Scr_GetTypeName(unsigned int index, scriptInstance_t inst)
{
  const char *v3; // eax

  if ( index < MEMORY[0xA05ACA4][4298 * inst] )
    return var_typename[*(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] - 8 * index + 4)];
  v3 = va("parameter %d does not exist", index + 1);
  Scr_Error(inst, v3, 0);
  return 0;
}

unsigned int __cdecl Scr_GetPointerType(unsigned int index, scriptInstance_t inst)
{
  const char *v3; // eax
  const char *v4; // eax

  if ( index < MEMORY[0xA05ACA4][4298 * inst] )
  {
    if ( *(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] - 8 * index + 4) == 1 )
      return GetObjectType(inst, *(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] - 8 * index));
    v3 = va("type %s is not an object", var_typename[*(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] - 8 * index + 4)]);
    Scr_Error(inst, v3, 0);
  }
  v4 = va("parameter %d does not exist", index + 1);
  Scr_Error(inst, v4, 0);
  return 0;
}

int __cdecl Scr_GetNumParam(scriptInstance_t inst)
{
  return MEMORY[0xA05ACA4][4298 * inst];
}

void __cdecl Scr_AddBool(unsigned int value, scriptInstance_t inst)
{
  if ( value >= 2
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5417,
          0,
          "%s",
          "value == 0 || value == 1") )
  {
    __debugbreak();
  }
  IncInParam(inst);
  *(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] + 4) = 6;
  *(unsigned int *)MEMORY[0xA05AC98][4298 * inst] = value;
}

void __cdecl IncInParam(scriptInstance_t inst)
{
  if ( (MEMORY[0xA05AC98][4298 * inst] < (unsigned int)(8232 * inst + 168191936)
     || MEMORY[0xA05AC98][4298 * inst] > (unsigned int)&gScrVmGlob[inst])
    && (MEMORY[0xA05AC98][4298 * inst] < (unsigned int)&MEMORY[0xA05AFB0] + 17192 * inst
     || MEMORY[0xA05AC98][4298 * inst] > (unsigned int)MEMORY[0xA05AC8C][4298 * inst])
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4473,
          0,
          "%s",
          "((gScrVmPub[inst].top >= gScrVmGlob[inst].eval_stack - 1) && (gScrVmPub[inst].top <= gScrVmGlob[inst].eval_sta"
          "ck)) || ((gScrVmPub[inst].top >= gScrVmPub[inst].stack) && (gScrVmPub[inst].top <= gScrVmPub[inst].maxstack))") )
  {
    __debugbreak();
  }
  Scr_ClearOutParams(inst);
  if ( MEMORY[0xA05AC98][4298 * inst] == MEMORY[0xA05AC8C][4298 * inst] )
    Sys_Error("Internal script stack overflow");
  MEMORY[0xA05AC98][4298 * inst] += 8;
  ++MEMORY[0xA05ACA0][4298 * inst];
  if ( (MEMORY[0xA05AC98][4298 * inst] < (unsigned int)&gScrVmGlob[inst]
     || MEMORY[0xA05AC98][4298 * inst] > (unsigned int)&unk_A0667D0 + 8232 * inst)
    && (MEMORY[0xA05AC98][4298 * inst] < (unsigned int)&MEMORY[0xA05AFB0] + 17192 * inst
     || MEMORY[0xA05AC98][4298 * inst] > (unsigned int)MEMORY[0xA05AC8C][4298 * inst])
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4483,
          0,
          "%s",
          "((gScrVmPub[inst].top >= gScrVmGlob[inst].eval_stack) && (gScrVmPub[inst].top <= gScrVmGlob[inst].eval_stack +"
          " 1)) || ((gScrVmPub[inst].top >= gScrVmPub[inst].stack) && (gScrVmPub[inst].top <= gScrVmPub[inst].maxstack))") )
  {
    __debugbreak();
  }
}

void __cdecl Scr_AddInt(int value, scriptInstance_t inst)
{
  IncInParam(inst);
  *(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] + 4) = 6;
  *(unsigned int *)MEMORY[0xA05AC98][4298 * inst] = value;
}

void __cdecl Scr_AddFloat(float value, scriptInstance_t inst)
{
  IncInParam(inst);
  *(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] + 4) = 5;
  *(float *)MEMORY[0xA05AC98][4298 * inst] = value;
}

void __cdecl Scr_AddAnim(scr_anim_s value, scriptInstance_t inst)
{
  IncInParam(inst);
  *(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] + 4) = 11;
  *(scr_anim_s *)MEMORY[0xA05AC98][4298 * inst] = value;
}

void __cdecl Scr_AddUndefined(scriptInstance_t inst)
{
  IncInParam(inst);
  *(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] + 4) = 0;
}

void __cdecl Scr_AddObject(unsigned int id, scriptInstance_t inst)
{
  if ( !id && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 5458, 0, "%s", "id") )
    __debugbreak();
  if ( GetObjectType(inst, id) == 13
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5461,
          0,
          "%s",
          "GetObjectType( inst, id ) != VAR_THREAD") )
  {
    __debugbreak();
  }
  if ( GetObjectType(inst, id) == 14
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5462,
          0,
          "%s",
          "GetObjectType( inst, id ) != VAR_NOTIFY_THREAD") )
  {
    __debugbreak();
  }
  if ( GetObjectType(inst, id) == 15
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5463,
          0,
          "%s",
          "GetObjectType( inst, id ) != VAR_TIME_THREAD") )
  {
    __debugbreak();
  }
  if ( GetObjectType(inst, id) == 16
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5464,
          0,
          "%s",
          "GetObjectType( inst, id ) != VAR_CHILD_THREAD") )
  {
    __debugbreak();
  }
  if ( GetObjectType(inst, id) == 21
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5465,
          0,
          "%s",
          "GetObjectType( inst, id ) != VAR_DEAD_THREAD") )
  {
    __debugbreak();
  }
  IncInParam(inst);
  *(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] + 4) = 1;
  *(unsigned int *)MEMORY[0xA05AC98][4298 * inst] = id;
  AddRefToObject(inst, id);
}

void __cdecl Scr_AddEntityNum(unsigned int entnum, unsigned int classnum, scriptInstance_t inst, int clientNum)
{
  unsigned int EntityId; // eax
  const char *varUsagePos; // [esp+0h] [ebp-4h]

  varUsagePos = (const char *)MEMORY[0xA05ABE4][29 * inst];
  if ( !varUsagePos )
    MEMORY[0xA05ABE4][29 * inst] = (int)"<script entity variable>";
  EntityId = Scr_GetEntityId(inst, entnum, classnum, clientNum);
  Scr_AddObject(EntityId, inst);
  MEMORY[0xA05ABE4][29 * inst] = (int)varUsagePos;
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
  if ( !value
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 5512, 0, "%s", "value") )
  {
    __debugbreak();
  }
  IncInParam(inst);
  *(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] + 4) = 2;
  *(unsigned int *)MEMORY[0xA05AC98][4298 * inst] = SL_GetString(value, 0, inst);
}

void __cdecl Scr_AddIString(char *value, scriptInstance_t inst)
{
  if ( !value
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 5522, 0, "%s", "value") )
  {
    __debugbreak();
  }
  IncInParam(inst);
  *(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] + 4) = 3;
  *(unsigned int *)MEMORY[0xA05AC98][4298 * inst] = SL_GetString(value, 0, inst);
}

void __cdecl Scr_AddConstString(unsigned int value, scriptInstance_t inst)
{
  if ( !value
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 5532, 0, "%s", "value") )
  {
    __debugbreak();
  }
  IncInParam(inst);
  *(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] + 4) = 2;
  *(unsigned int *)MEMORY[0xA05AC98][4298 * inst] = value;
  SL_AddRefToString(value, inst);
}

void __cdecl Scr_AddVector(float *value, scriptInstance_t inst)
{
  IncInParam(inst);
  *(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] + 4) = 4;
  *(unsigned int *)MEMORY[0xA05AC98][4298 * inst] = Scr_AllocVector(inst, value);
}

void __cdecl Scr_MakeArray(scriptInstance_t inst)
{
  IncInParam(inst);
  *(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] + 4) = 1;
  *(unsigned int *)MEMORY[0xA05AC98][4298 * inst] = Scr_AllocArray(inst);
}

void __cdecl Scr_AddArray(scriptInstance_t inst)
{
  unsigned int ArraySize; // eax
  const char *varUsagePos; // [esp+0h] [ebp-8h]
  unsigned int id; // [esp+4h] [ebp-4h]

  varUsagePos = (const char *)MEMORY[0xA05ABE4][29 * inst];
  if ( !varUsagePos )
    MEMORY[0xA05ABE4][29 * inst] = (int)"<script array variable>";
  if ( !MEMORY[0xA05ACA0][4298 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5568,
          0,
          "%s",
          "gScrVmPub[inst].inparamcount") )
  {
    __debugbreak();
  }
  MEMORY[0xA05AC98][4298 * inst] -= 8;
  --MEMORY[0xA05ACA0][4298 * inst];
  if ( *(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] + 4) != 1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5571,
          0,
          "%s",
          "gScrVmPub[inst].top->type == VAR_POINTER") )
  {
    __debugbreak();
  }
  ArraySize = GetArraySize(inst, *(unsigned int *)MEMORY[0xA05AC98][4298 * inst]);
  id = GetNewArrayVariable(inst, *(unsigned int *)MEMORY[0xA05AC98][4298 * inst], ArraySize);
  SetNewVariableValue(inst, id, (VariableValue *)(MEMORY[0xA05AC98][4298 * inst] + 8));
  MEMORY[0xA05ABE4][29 * inst] = (int)varUsagePos;
}

void __cdecl Scr_AddArrayStringIndexed(unsigned int stringValue, scriptInstance_t inst)
{
  unsigned int id; // [esp+0h] [ebp-4h]

  if ( !MEMORY[0xA05ACA0][4298 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5585,
          0,
          "%s",
          "gScrVmPub[inst].inparamcount") )
  {
    __debugbreak();
  }
  MEMORY[0xA05AC98][4298 * inst] -= 8;
  --MEMORY[0xA05ACA0][4298 * inst];
  if ( *(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] + 4) != 1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5588,
          0,
          "%s",
          "gScrVmPub[inst].top->type == VAR_POINTER") )
  {
    __debugbreak();
  }
  id = GetNewVariable(inst, *(unsigned int *)MEMORY[0xA05AC98][4298 * inst], stringValue);
  SetNewVariableValue(inst, id, (VariableValue *)(MEMORY[0xA05AC98][4298 * inst] + 8));
}

void __cdecl Scr_Error(const char *error, bool force_terminal)
{
  Scr_Error(SCRIPTINSTANCE_SERVER, error, force_terminal);
}

void __cdecl Scr_Error(scriptInstance_t inst, const char *error, bool force_terminal)
{
  Scr_SetErrorMessage(inst, error);
  MEMORY[0xA05AC9E][17192 * inst] = force_terminal;
  Scr_ErrorInternal(inst);
}

void __cdecl Scr_SetErrorMessage(scriptInstance_t inst, const char *error)
{
  if ( !MEMORY[0xA05AB8C][29 * inst] )
  {
    I_strncpyz(error_message, error, 1024);
    MEMORY[0xA05AB8C][29 * inst] = (int)error_message;
  }
}

void __cdecl Scr_TerminalError(const char *error, scriptInstance_t inst)
{
  Scr_DumpScriptThreads(inst);
  Scr_DumpScriptVariablesDefault(inst);
  MEMORY[0xA05AC9E][17192 * inst] = 1;
  Scr_Error(inst, error, 0);
}

void __cdecl Scr_NeverTerminalError(const char *error, scriptInstance_t inst)
{
  if ( dword_A0667DC[2058 * inst] )
  {
    Scr_SetErrorMessage(inst, error);
    longjmp(g_script_error[inst][g_script_error_level[inst]], -1);
  }
  Scr_Error(inst, error, 0);
}

void __cdecl Scr_ParamError(unsigned int index, const char *error, scriptInstance_t inst)
{
  if ( index >= MEMORY[0xA05ACA4][4298 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5649,
          0,
          "%s",
          "index < gScrVmPub[inst].outparamcount") )
  {
    __debugbreak();
  }
  MEMORY[0xA05AB90][29 * inst] = index + 1;
  Scr_Error(inst, error, 0);
}

void __cdecl Scr_ObjectError(const char *error, scriptInstance_t inst)
{
  MEMORY[0xA05AB90][29 * inst] = -1;
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

  if ( ((char *)value - ((char *)&MEMORY[0xA05AFB0] + 17192 * inst)) >> 3 <= 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5664,
          0,
          "%s\n\t(value - gScrVmPub[inst].stack) = %i",
          "(value - gScrVmPub[inst].stack > 0)",
          ((char *)value - ((char *)&MEMORY[0xA05AFB0] + 17192 * inst)) >> 3) )
  {
    __debugbreak();
  }
  if ( ((int)value - MEMORY[0xA05AC8C][4298 * inst]) >> 3 > 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5665,
          0,
          "%s\n\t(value - gScrVmPub[inst].maxstack) = %i",
          "(value - gScrVmPub[inst].maxstack <= 0)",
          ((int)value - MEMORY[0xA05AC8C][4298 * inst]) >> 3) )
  {
    __debugbreak();
  }
  if ( MEMORY[0xA05ACA0][4298 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5666,
          0,
          "%s",
          "!gScrVmPub[inst].inparamcount") )
  {
    __debugbreak();
  }
  if ( MEMORY[0xA05ACA4][4298 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5667,
          0,
          "%s",
          "!gScrVmPub[inst].outparamcount") )
  {
    __debugbreak();
  }
  MEMORY[0xA05ACA4][4298 * inst] = 1;
  MEMORY[0xA05AC98][4298 * inst] = (int)value;
  if ( inst == SCRIPTINSTANCE_CLIENT )
    bSet = CScr_SetObjectField(classnum, entnum, clientNum, offset);
  else
    bSet = Scr_SetObjectField(classnum, entnum, offset);
  if ( bSet )
  {
    if ( MEMORY[0xA05ACA0][4298 * inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5691,
            0,
            "%s",
            "!gScrVmPub[inst].inparamcount") )
    {
      __debugbreak();
    }
    if ( MEMORY[0xA05ACA4][4298 * inst] )
    {
      if ( MEMORY[0xA05ACA4][4298 * inst] != 1
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
              5695,
              0,
              "%s",
              "gScrVmPub[inst].outparamcount == 1") )
      {
        __debugbreak();
      }
      RemoveRefToValue(
        inst,
        *(unsigned int *)(MEMORY[0xA05AC98][4298 * inst] + 4),
        *(VariableUnion *)MEMORY[0xA05AC98][4298 * inst]);
      MEMORY[0xA05AC98][4298 * inst] -= 8;
      MEMORY[0xA05ACA4][4298 * inst] = 0;
    }
    return 1;
  }
  else
  {
    if ( MEMORY[0xA05ACA0][4298 * inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5685,
            0,
            "%s",
            "!gScrVmPub[inst].inparamcount") )
    {
      __debugbreak();
    }
    if ( MEMORY[0xA05ACA4][4298 * inst] != 1
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5686,
            0,
            "%s",
            "gScrVmPub[inst].outparamcount == 1") )
    {
      __debugbreak();
    }
    MEMORY[0xA05ACA4][4298 * inst] = 0;
    return 0;
  }
}

VariableUnion __cdecl GetEntityFieldValue(
        scriptInstance_t inst,
        unsigned int classnum,
        unsigned int entnum,
        unsigned __int16 clientNum,
        unsigned int offset)
{
  if ( MEMORY[0xA05ACA0][4298 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5708,
          0,
          "%s",
          "!gScrVmPub[inst].inparamcount") )
  {
    __debugbreak();
  }
  if ( MEMORY[0xA05ACA4][4298 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5709,
          0,
          "%s",
          "!gScrVmPub[inst].outparamcount") )
  {
    __debugbreak();
  }
  MEMORY[0xA05AC98][4298 * inst] = 8232 * inst + 168191936;
  dword_A0667CC[2058 * inst] = 0;
  if ( inst == SCRIPTINSTANCE_CLIENT )
    CScr_GetObjectField(classnum, entnum, clientNum, offset);
  else
    Scr_GetObjectField(classnum, entnum, offset);
  if ( (unsigned int)MEMORY[0xA05ACA0][4298 * inst] >= 2
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5723,
          0,
          "%s",
          "!gScrVmPub[inst].inparamcount || gScrVmPub[inst].inparamcount == 1") )
  {
    __debugbreak();
  }
  if ( MEMORY[0xA05ACA4][4298 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5724,
          0,
          "%s",
          "!gScrVmPub[inst].outparamcount") )
  {
    __debugbreak();
  }
  if ( MEMORY[0xA05AC98][4298 * inst] - 8 * MEMORY[0xA05ACA0][4298 * inst] != 8232 * inst + 168191936
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5725,
          0,
          "%s",
          "gScrVmPub[inst].top - gScrVmPub[inst].inparamcount == gScrVmGlob[inst].eval_stack - 1") )
  {
    __debugbreak();
  }
  MEMORY[0xA05ACA0][4298 * inst] = 0;
  return gScrVmGlob[inst].eval_stack[0].u;
}

void __cdecl Scr_SetStructField(unsigned int structId, unsigned int index, scriptInstance_t inst)
{
  unsigned int fieldValueId; // [esp+0h] [ebp-8h]

  if ( MEMORY[0xA05ACA4][4298 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5737,
          0,
          "%s",
          "!gScrVmPub[inst].outparamcount") )
  {
    __debugbreak();
  }
  if ( MEMORY[0xA05ACA0][4298 * inst] != 1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5738,
          0,
          "%s",
          "gScrVmPub[inst].inparamcount == 1") )
  {
    __debugbreak();
  }
  MEMORY[0xA05ABE4][29 * inst] = (int)"<radiant field variable>";
  fieldValueId = gScrVarGlob[inst].variableList[Scr_GetVariableFieldIndex(inst, structId, index) + 0x8000].hash.id;
  if ( MEMORY[0xA05ACA0][4298 * inst] != 1
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5748,
          0,
          "%s",
          "gScrVmPub[inst].inparamcount == 1") )
  {
    __debugbreak();
  }
  MEMORY[0xA05ACA0][4298 * inst] = 0;
  SetVariableFieldValue(inst, fieldValueId, (VariableValue *)MEMORY[0xA05AC98][4298 * inst]);
  if ( MEMORY[0xA05ACA0][4298 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5753,
          0,
          "%s",
          "!gScrVmPub[inst].inparamcount") )
  {
    __debugbreak();
  }
  if ( MEMORY[0xA05ACA4][4298 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5754,
          0,
          "%s",
          "!gScrVmPub[inst].outparamcount") )
  {
    __debugbreak();
  }
  MEMORY[0xA05AC98][4298 * inst] -= 8;
  MEMORY[0xA05ABE4][29 * inst] = 0;
}

void __cdecl Scr_SetDynamicEntityField(
        unsigned int entnum,
        unsigned int classnum,
        unsigned int index,
        scriptInstance_t inst,
        unsigned __int16 localClientNum)
{
  unsigned int entId; // [esp+0h] [ebp-4h]

  MEMORY[0xA05ABE4][29 * inst] = (int)"<radiant field variable>";
  entId = Scr_GetEntityId(inst, entnum, classnum, localClientNum);
  if ( GetObjectType(inst, entId) != 19
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5774,
          0,
          "%s",
          "GetObjectType( inst, entId ) == VAR_ENTITY") )
  {
    __debugbreak();
  }
  MEMORY[0xA05ABE4][29 * inst] = 0;
  Scr_SetStructField(entId, index, inst);
}

void __cdecl Scr_IncTime(scriptInstance_t inst)
{
  Scr_RunCurrentThreads(inst);
  Scr_FreeEntityList(inst);
  if ( (MEMORY[0xA05AB94][29 * inst] & 0xFF000000) != 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5789,
          0,
          "%s",
          "!(gScrVarPub[inst].time & ~VAR_NAME_LOW_MASK)") )
  {
    __debugbreak();
  }
  ++MEMORY[0xA05AB94][29 * inst];
  MEMORY[0xA05AB94][29 * inst] &= (unsigned int)&cls.rankedServers[711].game[34];
  MEMORY[0xA05ACAC][17192 * inst] = MEMORY[0xA05AC9D][17192 * inst] != 0;
}

void __cdecl Scr_DecTime(scriptInstance_t inst)
{
  if ( (MEMORY[0xA05AB94][29 * inst] & 0xFF000000) != 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5801,
          0,
          "%s",
          "!(gScrVarPub[inst].time & ~VAR_NAME_LOW_MASK)") )
  {
    __debugbreak();
  }
  --MEMORY[0xA05AB94][29 * inst];
  MEMORY[0xA05AB94][29 * inst] &= (unsigned int)&cls.rankedServers[711].game[34];
}

void __cdecl Scr_RunCurrentThreads(scriptInstance_t inst)
{
  unsigned intpre_time; // [esp+0h] [ebp-4h]

  if ( scrVmEnableScripts->current.enabled )
  {
    pre_time = Sys_MillisecondsRaw();
    if ( MEMORY[0xA05AC90][4298 * inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5818,
            0,
            "%s",
            "!gScrVmPub[inst].function_count") )
    {
      __debugbreak();
    }
    if ( MEMORY[0xA05AB8C][29 * inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5819,
            0,
            "%s",
            "!gScrVarPub[inst].error_message") )
    {
      __debugbreak();
    }
    if ( MEMORY[0xA05AB90][29 * inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5820,
            0,
            "%s",
            "!gScrVarPub[inst].error_index") )
    {
      __debugbreak();
    }
    if ( MEMORY[0xA05ACA4][4298 * inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5821,
            0,
            "%s",
            "!gScrVmPub[inst].outparamcount") )
    {
      __debugbreak();
    }
    if ( MEMORY[0xA05ACA0][4298 * inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5822,
            0,
            "%s",
            "!gScrVmPub[inst].inparamcount") )
    {
      __debugbreak();
    }
    if ( (_UNKNOWN *)MEMORY[0xA05AC98][4298 * inst] != (_UNKNOWN *)((char *)&MEMORY[0xA05AFB0] + 17192 * inst)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            5823,
            0,
            "%s",
            "gScrVmPub[inst].top == gScrVmPub[inst].stack") )
    {
      __debugbreak();
    }
    VM_SetTime(inst);
    gScrExecuteTime[inst] += Sys_MillisecondsRaw() - pre_time;
  }
}

void __cdecl VM_SetTime(scriptInstance_t inst)
{
  unsigned intv1; // eax
  unsigned int Object; // eax
  unsigned int id; // [esp+0h] [ebp-4h]

  if ( (MEMORY[0xA05AB94][29 * inst] & 0xFF000000) != 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4858,
          0,
          "%s",
          "!(gScrVarPub[inst].time & ~VAR_NAME_LOW_MASK)") )
  {
    __debugbreak();
  }
  if ( MEMORY[0xA05AB98][29 * inst] )
  {
    id = FindVariable(inst, MEMORY[0xA05AB98][29 * inst], MEMORY[0xA05AB94][29 * inst]);
    if ( id )
    {
      if ( !logScriptTimes
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
              4866,
              0,
              "%s",
              "logScriptTimes") )
      {
        __debugbreak();
      }
      if ( logScriptTimes->current.enabled )
      {
        v1 = Sys_Milliseconds();
        Com_Printf(24, "SET TIME: %d\n", v1);
      }
      Object = FindObject(inst, id);
      VM_Resume(inst, Object);
      SafeRemoveVariable(inst, MEMORY[0xA05AB98][29 * inst], MEMORY[0xA05AB94][29 * inst]);
    }
  }
}

void __cdecl VM_Resume(scriptInstance_t inst, unsigned int timeId)
{
  unsigned int v2; // eax
  VariableStackBuffer *stackValue; // [esp+1Ch] [ebp-10h]
  unsigned int stackId; // [esp+20h] [ebp-Ch]
  unsigned int startLocalId; // [esp+28h] [ebp-4h]

  PIXBeginNamedEvent(-1, "VM_Resume");
  if ( (_UNKNOWN *)MEMORY[0xA05AC98][4298 * inst] != (_UNKNOWN *)((char *)&MEMORY[0xA05AFB0] + 17192 * inst)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          4418,
          0,
          "%s",
          "gScrVmPub[inst].top == gScrVmPub[inst].stack") )
  {
    __debugbreak();
  }
  Scr_ResetAbortDebugger(inst);
  Scr_ResetTimeout(inst);
  if ( !timeId
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp", 4426, 0, "%s", "timeId") )
  {
    __debugbreak();
  }
  AddRefToObject(inst, timeId);
  gFs[inst].startTop = (VariableValue *)((char *)&MEMORY[0xA05AFB0] + 17192 * inst);
  gThreadCount[inst] = 0;
  while ( 1 )
  {
    if ( MEMORY[0xA05AB8C][29 * inst]
      && !va("%s", (const char *)MEMORY[0xA05AB8C][29 * inst])
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4437,
            0,
            "%s",
            "va(\"%s\", gScrVarPub[inst].error_message)") )
    {
      __debugbreak();
    }
    if ( MEMORY[0xA05AB90][29 * inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4439,
            0,
            "%s",
            "!gScrVarPub[inst].error_index") )
    {
      __debugbreak();
    }
    if ( MEMORY[0xA05ACA4][4298 * inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4440,
            0,
            "%s",
            "!gScrVmPub[inst].outparamcount") )
    {
      __debugbreak();
    }
    if ( MEMORY[0xA05ACA0][4298 * inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4441,
            0,
            "%s",
            "!gScrVmPub[inst].inparamcount") )
    {
      __debugbreak();
    }
    if ( MEMORY[0xA05AC90][4298 * inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4442,
            0,
            "%s",
            "!gScrVmPub[inst].function_count") )
    {
      __debugbreak();
    }
    if ( (int *)gScrVmPub[inst].localVars != &dword_A0667E0[2058 * inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4443,
            0,
            "%s",
            "gScrVmPub[inst].localVars == gScrVmGlob[inst].localVarsStack - 1") )
    {
      __debugbreak();
    }
    if ( gFs[inst].startTop != (VariableValue *)((char *)&MEMORY[0xA05AFB0] + 17192 * inst)
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4444,
            0,
            "%s",
            "gFs[inst].startTop == &gScrVmPub[inst].stack[0]") )
    {
      __debugbreak();
    }
    if ( gThreadCount[inst]
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4445,
            0,
            "%s",
            "!gThreadCount[inst]") )
    {
      __debugbreak();
    }
    stackId = FindFirstSibling(inst, timeId);
    if ( !stackId )
      break;
    startLocalId = GetVariableKeyObject(inst, stackId);
    if ( !startLocalId
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4452,
            0,
            "%s",
            "startLocalId") )
    {
      __debugbreak();
    }
    if ( GetValueType(inst, stackId) != 10
      && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
            4454,
            0,
            "%s",
            "GetValueType( inst, stackId ) == VAR_STACK") )
    {
      __debugbreak();
    }
    stackValue = (VariableStackBuffer *)GetVariableValueAddress(inst, stackId)->next;
    RemoveObjectVariable(inst, timeId, startLocalId);
    VM_UnarchiveStack(inst, startLocalId, stackValue);
    v2 = VM_Execute_0(inst);
    RemoveRefToObject(inst, v2);
    RemoveRefToValue(
      inst,
      *((unsigned int *)&MEMORY[0xA05AFB8] + 4298 * inst + 1),
      *(VariableUnion *)((char *)&MEMORY[0xA05AFB8] + 17192 * inst));
  }
  RemoveRefToObject(inst, timeId);
  ClearVariableValue(inst, MEMORY[0xA05ABB0][29 * inst]);
  MEMORY[0xA05AC98][4298 * inst] = (int)&MEMORY[0xA05AFB0] + 17192 * inst;
  if ( g_DXDeviceThread == GetCurrentThreadId() )
    D3DPERF_EndEvent();
}

void __cdecl VM_UnarchiveStack(scriptInstance_t inst, unsigned int startLocalId, VariableStackBuffer *stackValue)
{
  VariableValue *top; // [esp+8h] [ebp-14h]
  char *buf; // [esp+Ch] [ebp-10h]
  const char *bufa; // [esp+Ch] [ebp-10h]
  unsigned int localId; // [esp+10h] [ebp-Ch]
  int function_count; // [esp+14h] [ebp-8h]
  int size; // [esp+18h] [ebp-4h]

  if ( MEMORY[0xA05AC90][4298 * inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          3411,
          0,
          "%s",
          "!gScrVmPub[inst].function_count") )
  {
    __debugbreak();
  }
  if ( !stackValue->pos
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          3412,
          0,
          "%s",
          "stackValue->pos") )
  {
    __debugbreak();
  }
  if ( gFs[inst].startTop != (VariableValue *)((char *)&MEMORY[0xA05AFB0] + 17192 * inst)
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          3413,
          0,
          "%s",
          "gFs[inst].startTop == &gScrVmPub[inst].stack[0]") )
  {
    __debugbreak();
  }
  _mm_prefetch(stackValue->pos, 1);
  *(unsigned int *)MEMORY[0xA05AC94][4298 * inst] = stackValue->pos;
  ++MEMORY[0xA05AC90][4298 * inst];
  MEMORY[0xA05AC94][4298 * inst] += 24;
  size = stackValue->size;
  buf = stackValue->buf;
  top = (VariableValue *)((char *)&MEMORY[0xA05AFB0] + 17192 * inst);
  while ( size )
  {
    --size;
    top[1].type = (unsigned __int8)*buf;
    ++top;
    bufa = buf + 1;
    if ( top->type == 7 )
    {
      if ( MEMORY[0xA05AC90][4298 * inst] >= 32
        && !Assert_MyHandler(
              "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
              3433,
              0,
              "%s",
              "gScrVmPub[inst].function_count < MAX_VM_STACK_DEPTH") )
      {
        __debugbreak();
      }
      *(unsigned int *)MEMORY[0xA05AC94][4298 * inst] = *(unsigned int *)bufa;
      ++MEMORY[0xA05AC90][4298 * inst];
      MEMORY[0xA05AC94][4298 * inst] += 24;
    }
    else
    {
      top->u.intValue = *(unsigned int *)bufa;
    }
    buf = (char *)(bufa + 4);
  }
  gFs[inst].pos = stackValue->pos;
  gFs[inst].top = top;
  localId = stackValue->localId;
  gFs[inst].localId = localId;
  Scr_ClearWaitTime(inst, startLocalId);
  if ( MEMORY[0xA05AC90][4298 * inst] >= 32
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          3453,
          0,
          "%s",
          "gScrVmPub[inst].function_count < MAX_VM_STACK_DEPTH") )
  {
    __debugbreak();
  }
  function_count = MEMORY[0xA05AC90][4298 * inst];
  while ( 1 )
  {
    MEMORY[0xA05ACB4][4298 * inst + 6 * function_count--] = localId;
    if ( !function_count )
      break;
    localId = GetParentLocalId(inst, localId);
  }
  while ( ++function_count != MEMORY[0xA05AC90][4298 * inst] )
    MEMORY[0xA05ACB8][4298 * inst + 6 * function_count] = Scr_AddLocalVars(
                                                            inst,
                                                            MEMORY[0xA05ACB4][4298 * inst + 6 * function_count]);
  gFs[inst].localVarCount = Scr_AddLocalVars(inst, gFs[inst].localId);
  if ( stackValue->time != LOBYTE(MEMORY[0xA05AB94][29 * inst]) )
    Scr_ResetTimeout(inst);
  --MEMORY[0xA05ABD8][29 * inst];
  MT_Free(stackValue, stackValue->bufLen, inst);
  if ( MEMORY[0xA05AFB4][4298 * inst] != 7
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          3482,
          0,
          "%s",
          "gScrVmPub[inst].stack[0].type == VAR_CODEPOS") )
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
  unsigned intv1; // eax

  dword_A0667E0[2058 * inst] = Sys_Milliseconds();
  if ( !logScriptTimes
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_vm.cpp",
          5847,
          0,
          "%s",
          "logScriptTimes") )
  {
    __debugbreak();
  }
  if ( logScriptTimes->current.enabled )
  {
    v1 = Sys_Milliseconds();
    Com_Printf(24, "RESET TIME: %d\n", v1);
  }
  memset((unsigned __int8 *)&dword_A0642EC[1155 * inst], 0, 0x200u);
}

bool __cdecl Scr_IsStackClear(scriptInstance_t inst)
{
  return MEMORY[0xA05AC98][4298 * inst] == (unsigned int)&MEMORY[0xA05AFB0] + 17192 * inst;
}

void __cdecl Scr_StackClear(scriptInstance_t inst)
{
  MEMORY[0xA05AC98][4298 * inst] = (int)&MEMORY[0xA05AFB0] + 17192 * inst;
}

