#include "cscr_parsetree.h"

XAssetHeader __cdecl node_pos(void *pool)
{
  return (XAssetHeader)pool;
}

sval_u __cdecl node1_(scriptInstance_t inst, int val1)
{
  return (sval_u)val1;
}

void __cdecl Scr_InitAllocNode(scriptInstance_t inst)
{
  if ( g_allocNodeUser[inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parsetree.cpp",
          79,
          0,
          "%s",
          "!g_allocNodeUser[inst]") )
  {
    __debugbreak();
  }
  g_allocNodeUser[inst] = Hunk_UserCreate(0x10000, HU_SCHEME_DEFAULT, 0, 0, "Scr_InitAllocNode", 7);
}

void __cdecl Scr_ShutdownAllocNode(scriptInstance_t inst)
{
  if ( g_allocNodeUser[inst] )
  {
    Hunk_UserDestroy(g_allocNodeUser[inst]);
    g_allocNodeUser[inst] = 0;
  }
}

sval_u __cdecl node0(scriptInstance_t inst, unsigned __int8 type)
{
  sval_u result; // eax

  result.stringValue = (unsigned int)Scr_AllocNode(inst, 1);
  *(_BYTE *)result.stringValue = type;
  return result;
}

sval_u *__cdecl Scr_AllocNode(scriptInstance_t inst, int size)
{
  if ( !g_allocNodeUser[inst]
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parsetree.cpp",
          95,
          0,
          "%s",
          "g_allocNodeUser[inst]") )
  {
    __debugbreak();
  }
  return (sval_u *)Hunk_UserAlloc(g_allocNodeUser[inst], 4 * size, 4, 0);
}

sval_u __cdecl node1(scriptInstance_t inst, unsigned __int8 type, sval_u val1)
{
  sval_u result; // eax

  result.stringValue = (unsigned int)Scr_AllocNode(inst, 2);
  *(_BYTE *)result.stringValue = type;
  *(sval_u *)(result.stringValue + 4) = val1;
  return result;
}

sval_u __cdecl node2(scriptInstance_t inst, unsigned __int8 type, sval_u val1, sval_u val2)
{
  sval_u result; // eax

  result.stringValue = (unsigned int)Scr_AllocNode(inst, 3);
  *(_BYTE *)result.stringValue = type;
  *(sval_u *)(result.stringValue + 4) = val1;
  *(sval_u *)(result.stringValue + 8) = val2;
  return result;
}

sval_u __cdecl node2_(scriptInstance_t inst, sval_u val1, sval_u val2)
{
  sval_u result; // eax

  result.stringValue = (unsigned int)Scr_AllocNode(inst, 2);
  *(unsigned int *)result.stringValue = val1.stringValue;
  *(sval_u *)(result.stringValue + 4) = val2;
  return result;
}

sval_u __cdecl node3(scriptInstance_t inst, unsigned __int8 type, sval_u val1, sval_u val2, sval_u val3)
{
  sval_u result; // eax

  result.stringValue = (unsigned int)Scr_AllocNode(inst, 4);
  *(_BYTE *)result.stringValue = type;
  *(sval_u *)(result.stringValue + 4) = val1;
  *(sval_u *)(result.stringValue + 8) = val2;
  *(sval_u *)(result.stringValue + 12) = val3;
  return result;
}

sval_u __cdecl node4(scriptInstance_t inst, unsigned __int8 type, sval_u val1, sval_u val2, sval_u val3, sval_u val4)
{
  sval_u result; // eax

  result.stringValue = (unsigned int)Scr_AllocNode(inst, 5);
  *(_BYTE *)result.stringValue = type;
  *(sval_u *)(result.stringValue + 4) = val1;
  *(sval_u *)(result.stringValue + 8) = val2;
  *(sval_u *)(result.stringValue + 12) = val3;
  *(sval_u *)(result.stringValue + 16) = val4;
  return result;
}

sval_u __cdecl node5(
        scriptInstance_t inst,
        unsigned __int8 type,
        sval_u val1,
        sval_u val2,
        sval_u val3,
        sval_u val4,
        sval_u val5)
{
  sval_u result; // eax

  result.stringValue = (unsigned int)Scr_AllocNode(inst, 6);
  *(_BYTE *)result.stringValue = type;
  *(sval_u *)(result.stringValue + 4) = val1;
  *(sval_u *)(result.stringValue + 8) = val2;
  *(sval_u *)(result.stringValue + 12) = val3;
  *(sval_u *)(result.stringValue + 16) = val4;
  *(sval_u *)(result.stringValue + 20) = val5;
  return result;
}

sval_u __cdecl node6(
        scriptInstance_t inst,
        unsigned __int8 type,
        sval_u val1,
        sval_u val2,
        sval_u val3,
        sval_u val4,
        sval_u val5,
        sval_u val6)
{
  sval_u result; // eax

  result.stringValue = (unsigned int)Scr_AllocNode(inst, 7);
  *(_BYTE *)result.stringValue = type;
  *(sval_u *)(result.stringValue + 4) = val1;
  *(sval_u *)(result.stringValue + 8) = val2;
  *(sval_u *)(result.stringValue + 12) = val3;
  *(sval_u *)(result.stringValue + 16) = val4;
  *(sval_u *)(result.stringValue + 20) = val5;
  *(sval_u *)(result.stringValue + 24) = val6;
  return result;
}

sval_u __cdecl node7(
        scriptInstance_t inst,
        unsigned __int8 type,
        sval_u val1,
        sval_u val2,
        sval_u val3,
        sval_u val4,
        sval_u val5,
        sval_u val6,
        sval_u val7)
{
  sval_u result; // eax

  result.stringValue = (unsigned int)Scr_AllocNode(inst, 8);
  *(_BYTE *)result.stringValue = type;
  *(sval_u *)(result.stringValue + 4) = val1;
  *(sval_u *)(result.stringValue + 8) = val2;
  *(sval_u *)(result.stringValue + 12) = val3;
  *(sval_u *)(result.stringValue + 16) = val4;
  *(sval_u *)(result.stringValue + 20) = val5;
  *(sval_u *)(result.stringValue + 24) = val6;
  *(sval_u *)(result.stringValue + 28) = val7;
  return result;
}

sval_u __cdecl node8(
        scriptInstance_t inst,
        unsigned __int8 type,
        sval_u val1,
        sval_u val2,
        sval_u val3,
        sval_u val4,
        sval_u val5,
        sval_u val6,
        sval_u val7,
        sval_u val8)
{
  sval_u result; // eax

  result.stringValue = (unsigned int)Scr_AllocNode(inst, 9);
  *(_BYTE *)result.stringValue = type;
  *(sval_u *)(result.stringValue + 4) = val1;
  *(sval_u *)(result.stringValue + 8) = val2;
  *(sval_u *)(result.stringValue + 12) = val3;
  *(sval_u *)(result.stringValue + 16) = val4;
  *(sval_u *)(result.stringValue + 20) = val5;
  *(sval_u *)(result.stringValue + 24) = val6;
  *(sval_u *)(result.stringValue + 28) = val7;
  *(sval_u *)(result.stringValue + 32) = val8;
  return result;
}

sval_u __cdecl linked_list_end(scriptInstance_t inst, sval_u val)
{
  sval_u result; // eax
  sval_u *node; // [esp+0h] [ebp-8h]

  node = Scr_AllocNode(inst, 2);
  *node = val;
  node[1].stringValue = 0;
  result.stringValue = (unsigned int)Scr_AllocNode(inst, 2);
  *(unsigned int *)result.stringValue = node;
  *(unsigned int *)(result.stringValue + 4) = node;
  return result;
}

sval_u __cdecl prepend_node(scriptInstance_t inst, sval_u val1, sval_u val2)
{
  sval_u *v3; // eax

  v3 = Scr_AllocNode(inst, 2);
  *v3 = val1;
  v3[1] = *val2.node;
  *(unsigned int *)val2.stringValue = v3;
  return val2;
}

sval_u __cdecl append_node(scriptInstance_t inst, sval_u val1, sval_u val2)
{
  sval_u *v3; // eax

  v3 = Scr_AllocNode(inst, 2);
  *v3 = val2;
  v3[1].stringValue = 0;
  *(unsigned int *)(*(unsigned int *)(val1.stringValue + 4) + 4) = v3;
  *(unsigned int *)(val1.stringValue + 4) = v3;
  return val1;
}

void __cdecl Scr_ClearDebugExpr(scriptInstance_t inst, debugger_sval_s *debugExprHead)
{
  while ( debugExprHead )
  {
    Scr_ClearDebugExprValue(inst, (sval_u)&debugExprHead[1]);
    debugExprHead = debugExprHead->next;
  }
}

sval_u __cdecl Scr_AllocDebugExpr(scriptInstance_t inst, unsigned __int8 type, int size, const char *name)
{
  unsigned int *v4; // eax

  v4 = Z_Malloc(size + 4, name, 0);
  *v4 = (unsigned int)g_debugExprHead;
  g_debugExprHead = (debugger_sval_s *)v4;
  *((_BYTE *)v4 + 4) = type;
  return (sval_u)(v4 + 1);
}

void __cdecl Scr_FreeDebugExpr(scriptInstance_t inst, ScriptExpression_t *expr)
{
  debugger_sval_s *debugExprHead; // [esp+0h] [ebp-Ch]
  debugger_sval_s *nextDebugExprHead; // [esp+8h] [ebp-4h]

  if ( expr->breakonExpr )
    --dword_A0642E8[1155 * inst];
  debugExprHead = expr->exprHead;
  if ( !debugExprHead
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parsetree.cpp",
          410,
          0,
          "%s",
          "debugExprHead") )
  {
    __debugbreak();
  }
  while ( debugExprHead )
  {
    Scr_FreeDebugExprValue(inst, (sval_u)&debugExprHead[1]);
    nextDebugExprHead = debugExprHead->next;
    Z_Free(debugExprHead, 0);
    debugExprHead = nextDebugExprHead;
  }
}

sval_u __cdecl debugger_node0(scriptInstance_t inst, unsigned __int8 type)
{
  return Scr_AllocDebugExpr(inst, type, 4, "debugger_node0");
}

sval_u __cdecl debugger_node1(scriptInstance_t inst, unsigned __int8 type, sval_u val1)
{
  sval_u result; // eax

  result = Scr_AllocDebugExpr(inst, type, 8, "debugger_node1");
  *(sval_u *)(result.stringValue + 4) = val1;
  return result;
}

sval_u __cdecl debugger_node2(scriptInstance_t inst, unsigned __int8 type, sval_u val1, sval_u val2)
{
  sval_u result; // eax

  result = Scr_AllocDebugExpr(inst, type, 12, "debugger_node2");
  *(sval_u *)(result.stringValue + 4) = val1;
  *(sval_u *)(result.stringValue + 8) = val2;
  return result;
}

sval_u __cdecl debugger_node3(scriptInstance_t inst, unsigned __int8 type, sval_u val1, sval_u val2, sval_u val3)
{
  sval_u result; // eax

  result = Scr_AllocDebugExpr(inst, type, 16, "debugger_node3");
  *(sval_u *)(result.stringValue + 4) = val1;
  *(sval_u *)(result.stringValue + 8) = val2;
  *(sval_u *)(result.stringValue + 12) = val3;
  return result;
}

sval_u __cdecl debugger_node4(
        scriptInstance_t inst,
        unsigned __int8 type,
        sval_u val1,
        sval_u val2,
        sval_u val3,
        sval_u val4)
{
  sval_u result; // eax

  result = Scr_AllocDebugExpr(inst, type, 20, "debugger_node4");
  *(sval_u *)(result.stringValue + 4) = val1;
  *(sval_u *)(result.stringValue + 8) = val2;
  *(sval_u *)(result.stringValue + 12) = val3;
  *(sval_u *)(result.stringValue + 16) = val4;
  return result;
}

sval_u __cdecl debugger_prepend_node(scriptInstance_t inst, sval_u val1, sval_u val2)
{
  *(unsigned int *)val2.stringValue = *(unsigned int *)&debugger_node2(inst, 0, val1, *val2.node) + 4;
  return val2;
}

sval_u __cdecl debugger_buffer(
        scriptInstance_t inst,
        unsigned __int8 type,
        char *buf,
        unsigned int size,
        int alignment)
{
  sval_u result; // [esp+4h] [ebp-8h]
  unsigned __int8 *bufCopy; // [esp+8h] [ebp-4h]
  int alignmenta; // [esp+24h] [ebp+18h]

  if ( (alignment & (alignment - 1)) != 0
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\clientscript\\cscr_parsetree.cpp",
          611,
          0,
          "%s",
          "IsPowerOf2( alignment )") )
  {
    __debugbreak();
  }
  alignmenta = alignment - 1;
  result = Scr_AllocDebugExpr(inst, type, size + alignmenta + 8, "debugger_buffer");
  bufCopy = (unsigned __int8 *)(~alignmenta & (result.stringValue + alignmenta + 8));
  memcpy(bufCopy, (unsigned __int8 *)buf, size);
  *(unsigned int *)(result.stringValue + 4) = bufCopy;
  return result;
}

sval_u __cdecl debugger_string(scriptInstance_t inst, unsigned __int8 type, char *s)
{
  return debugger_buffer(inst, type, s, strlen(s) + 1, 1);
}

