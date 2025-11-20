#include "eval.h"

bool __cdecl Eval_AnyMissingOperands(const Eval *eval)
{
  int opIndex; // [esp+0h] [ebp-8h]
  int requiredOperandCount; // [esp+4h] [ebp-4h]

  requiredOperandCount = 1;
  for ( opIndex = 0; opIndex < eval->opStackPos; ++opIndex )
    requiredOperandCount += s_consumedOperandCount[eval->opStack[opIndex]];
  return requiredOperandCount != eval->valStackPos;
}

char  Eval_PushOperator@<al>(long double a1@<esi:edi>, Eval *eval, EvalOperatorType op)
{
  bool v4; // [esp+0h] [ebp-10h]
  const char *v5; // [esp+4h] [ebp-Ch] BYREF
  const char *pExceptionObject; // [esp+8h] [ebp-8h] BYREF
  bool leftToRight; // [esp+Dh] [ebp-3h]
  char precedence; // [esp+Eh] [ebp-2h]
  bool higherPrecedence; // [esp+Fh] [ebp-1h]
  int savedregs; // [esp+10h] [ebp+0h] BYREF

  if ( s_precedence[op] < 0 )
    return 0;
  if ( op == EVAL_OP_RPAREN && !eval->parenCount )
    return 0;
  if ( op == EVAL_OP_LPAREN )
  {
    if ( eval->valStackPos && !eval->pushedOp )
      return 0;
    ++eval->parenCount;
  }
  if ( op == EVAL_OP_PLUS )
  {
    if ( Eval_IsUnaryOp(eval) )
      op = EVAL_OP_UNARY_PLUS;
  }
  else if ( op == EVAL_OP_MINUS && Eval_IsUnaryOp(eval) )
  {
    op = EVAL_OP_UNARY_MINUS;
  }
  if ( (unsigned int)op >= EVAL_OP_COUNT
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\eval.cpp",
          549,
          0,
          "%s",
          "op >= 0 && op < ARRAY_COUNT( s_precedence )") )
  {
    __debugbreak();
  }
  precedence = s_precedence[op];
  while ( eval->opStackPos > 0 )
  {
    higherPrecedence = s_precedence[eval->opStack[eval->opStackPos - 1]] > precedence;
    v4 = s_precedence[eval->opStack[eval->opStackPos - 1]] == precedence
      && !s_rightToLeft[eval->opStack[eval->opStackPos - 1]];
    leftToRight = v4;
    if ( !higherPrecedence && !leftToRight )
      break;
    if ( eval->opStack[eval->opStackPos - 1] == EVAL_OP_LPAREN )
    {
      if ( op == EVAL_OP_RPAREN )
      {
        --eval->parenCount;
        --eval->opStackPos;
        eval->pushedOp = 0;
        return 1;
      }
      break;
    }
    if ( !Eval_EvaluationStep((int)&savedregs, a1, eval) )
      return 0;
  }
  if ( op != EVAL_OP_COLON || eval->opStackPos && eval->opStack[eval->opStackPos - 1] == EVAL_OP_QUESTION )
  {
    if ( eval->opStackPos == 1024 )
    {
      v5 = "evaluation stack overflow - expression is too complex";
      _CxxThrowException(&v5, &PA.deinit);
    }
    eval->opStack[eval->opStackPos++] = op;
    eval->pushedOp = 1;
    return 1;
  }
  else
  {
    if ( eval->parenCount )
    {
      pExceptionObject = "found ':' without preceding '?' in expression of type 'a ? b : c'";
      _CxxThrowException(&pExceptionObject, &PA.deinit);
    }
    return 0;
  }
}

// local variable allocation has failed, the output may be wrong!
bool  Eval_EvaluationStep@<al>(int a1@<ebp>, long double a2@<esi:edi>, Eval *eval)
{
  bool result; // al
  int v4; // esi
  int v5; // esi
  EvalValue *v6; // edx
  EvalOperatorType *v7; // ecx
  long double v9; // [esp+10h] [ebp-B4h]
  const char *v10; // [esp+1Ch] [ebp-A8h] BYREF
  int v11; // [esp+20h] [ebp-A4h]
  int v12; // [esp+24h] [ebp-A0h]
  const char *v13; // [esp+28h] [ebp-9Ch] BYREF
  int i; // [esp+2Ch] [ebp-98h]
  EvalOperatorType v15; // [esp+30h] [ebp-94h]
  EvalOperatorType v16; // [esp+34h] [ebp-90h]
  EvalOperatorType v17; // [esp+38h] [ebp-8Ch]
  EvalOperatorType v18; // [esp+3Ch] [ebp-88h]
  bool v19; // [esp+43h] [ebp-81h]
  EvalOperatorType v20; // [esp+44h] [ebp-80h]
  bool v21; // [esp+4Bh] [ebp-79h]
  const char *v22; // [esp+4Ch] [ebp-78h] BYREF
  double v23; // [esp+50h] [ebp-74h] OVERLAPPED
  long double dQuotientFloor; // [esp+5Ch] [ebp-68h] BYREF
  const char *v25; // [esp+64h] [ebp-60h] BYREF
  unsigned __int8 *v26; // [esp+68h] [ebp-5Ch]
  int v27; // [esp+6Ch] [ebp-58h]
  char *s; // [esp+74h] [ebp-50h]
  const char *v29; // [esp+78h] [ebp-4Ch]
  const char *v30; // [esp+7Ch] [ebp-48h]
  unsigned int v31; // [esp+80h] [ebp-44h]
  int v32; // [esp+84h] [ebp-40h]
  unsigned int v33; // [esp+88h] [ebp-3Ch]
  int length[2]; // [esp+8Ch] [ebp-38h]
  const char *v35; // [esp+94h] [ebp-30h]
  const char *v36; // [esp+98h] [ebp-2Ch]
  const char *v37; // [esp+9Ch] [ebp-28h] BYREF
  const char *v38; // [esp+A0h] [ebp-24h] BYREF
  EvalOperatorType v39; // [esp+A4h] [ebp-20h]
  const char *v40; // [esp+A8h] [ebp-1Ch] BYREF
  __int32 v41; // [esp+ACh] [ebp-18h]
  const char *v42; // [esp+B0h] [ebp-14h] BYREF
  unsigned int v43[4]; // [esp+B4h] [ebp-10h] BYREF
  _UNKNOWN *retaddr; // [esp+C4h] [ebp+0h]

  v43[1] = a1;
  v43[2] = retaddr;
  if ( !eval->opStackPos )
    return 0;
  if ( eval->opStack[--eval->opStackPos] == EVAL_OP_LPAREN )
    return 1;
  if ( eval->opStack[eval->opStackPos] == EVAL_OP_QUESTION )
  {
    v43[0] = "found '?' with no following ':' in expression of type 'a ? b : c'";
    _CxxThrowException(v43, &PA.deinit);
  }
  if ( !eval->valStackPos )
  {
    v42 = "missing operand (for example, 'a + ' or ' / b')";
    _CxxThrowException(&v42, &PA.deinit);
  }
  v41 = eval->opStack[eval->opStackPos] - 2;
  switch ( v41 )
  {
    case 0:
      if ( eval->valStackPos < 3 )
      {
        v13 = "missing operand (for example, 'a + ' or ' / b')";
        _CxxThrowException(&v13, &PA.deinit);
      }
      if ( eval->opStack[4 * eval->valStackPos + 1012] )
      {
        if ( eval->opStack[4 * eval->valStackPos + 1012] != EVAL_OP_RPAREN )
        {
          v10 = "can only switch on numbers";
          _CxxThrowException(&v10, &PA.deinit);
        }
        v11 = -(eval->opStack[4 * eval->valStackPos + 1014] != EVAL_OP_LPAREN) - 1;
      }
      else
      {
        if ( *(double *)&eval->opStack[4 * eval->valStackPos + 1014] == 0.0 )
          v12 = -1;
        else
          v12 = -2;
        v11 = v12;
      }
      if ( eval->opStack[4 * eval->valStackPos + 1016] == EVAL_OP_COLON
        && eval->opStack[4 * eval->valStackPos + 1020] == EVAL_OP_COLON )
      {
        free(eval->valStack[1 - v11 + eval->valStackPos].u.s);
      }
      else
      {
        Eval_PrepareBinaryOpSameTypes(eval);
      }
      v6 = &eval->valStack[v11 + eval->valStackPos];
      v7 = &eval->opStack[4 * eval->valStackPos + 1012];
      *v7 = (EvalOperatorType)v6->type;
      v7[1] = *((EvalOperatorType *)&v6->type + 1);
      v7[2] = (EvalOperatorType)v6->u.i;
      v7[3] = *((EvalOperatorType *)&v6->u.s + 1);
      eval->valStackPos -= 2;
      --eval->opStackPos;
      goto LABEL_121;
    case 2:
      if ( eval->valStackPos >= 2
        && eval->opStack[4 * eval->valStackPos + 1016] == EVAL_OP_COLON
        && eval->opStack[4 * eval->valStackPos + 1020] == EVAL_OP_COLON )
      {
        v36 = (const char *)eval->opStack[4 * eval->valStackPos + 1018];
        length[1] = (int)(v36 + 1);
        v35 = &v36[strlen(v36) + 1];
        v33 = v35 - (v36 + 1);
        v31 = v33;
        v30 = (const char *)eval->opStack[4 * eval->valStackPos + 1022];
        s = (char *)(v30 + 1);
        v29 = &v30[strlen(v30) + 1];
        v27 = v29 - (v30 + 1);
        v32 = v27;
        v26 = (unsigned __int8 *)malloc(v33 + v29 - v30);
        memcpy(v26, (unsigned __int8 *)eval->opStack[4 * eval->valStackPos + 1018], v33);
        memcpy(&v26[v33], (unsigned __int8 *)eval->opStack[4 * eval->valStackPos + 1022], v29 - v30);
        free((void *)eval->opStack[4 * eval->valStackPos + 1018]);
        free((void *)eval->opStack[4 * eval->valStackPos + 1022]);
        eval->opStack[4 * eval->valStackPos + 1018] = (EvalOperatorType)v26;
      }
      else
      {
        Eval_PrepareBinaryOpSameTypes(eval);
        if ( eval->opStack[4 * eval->valStackPos + 1016] )
          eval->opStack[4 * eval->valStackPos + 1018] += eval->opStack[4 * eval->valStackPos + 1022];
        else
          *(double *)&eval->opStack[4 * eval->valStackPos + 1018] = *(double *)&eval->opStack[4 * eval->valStackPos
                                                                                            + 1018]
                                                                  + *(double *)&eval->opStack[4 * eval->valStackPos
                                                                                            + 1022];
      }
      --eval->valStackPos;
      goto LABEL_121;
    case 3:
      Eval_PrepareBinaryOpSameTypes(eval);
      if ( eval->opStack[4 * eval->valStackPos + 1016] )
        eval->opStack[4 * eval->valStackPos + 1018] -= eval->opStack[4 * eval->valStackPos + 1022];
      else
        *(double *)&eval->opStack[4 * eval->valStackPos + 1018] = *(double *)&eval->opStack[4 * eval->valStackPos + 1018]
                                                                - *(double *)&eval->opStack[4 * eval->valStackPos + 1022];
      --eval->valStackPos;
      goto LABEL_121;
    case 4:
      goto LABEL_121;
    case 5:
      if ( eval->opStack[4 * eval->valStackPos + 1020] == EVAL_OP_RPAREN )
      {
        eval->opStack[4 * eval->valStackPos + 1022] = -eval->opStack[4 * eval->valStackPos + 1022];
      }
      else
      {
        if ( eval->opStack[4 * eval->valStackPos + 1020] )
        {
          v40 = "cannot negate strings";
          _CxxThrowException(&v40, &PA.deinit);
        }
        *(double *)&eval->opStack[4 * eval->valStackPos + 1022] = -*(double *)&eval->opStack[4 * eval->valStackPos
                                                                                           + 1022];
      }
      goto LABEL_121;
    case 6:
      Eval_PrepareBinaryOpSameTypes(eval);
      if ( eval->opStack[4 * eval->valStackPos + 1016] )
        eval->opStack[4 * eval->valStackPos + 1018] *= eval->opStack[4 * eval->valStackPos + 1022];
      else
        *(double *)&eval->opStack[4 * eval->valStackPos + 1018] = *(double *)&eval->opStack[4 * eval->valStackPos + 1018]
                                                                * *(double *)&eval->opStack[4 * eval->valStackPos + 1022];
      --eval->valStackPos;
      goto LABEL_121;
    case 7:
      Eval_PrepareBinaryOpSameTypes(eval);
      if ( eval->opStack[4 * eval->valStackPos + 1016] )
      {
        if ( eval->opStack[4 * eval->valStackPos + 1022] == EVAL_OP_LPAREN )
        {
          HIDWORD(dQuotientFloor) = "divide by zero";
          _CxxThrowException((char *)&dQuotientFloor + 4, &PA.deinit);
        }
        eval->opStack[4 * eval->valStackPos + 1018] /= eval->opStack[4 * eval->valStackPos + 1022];
      }
      else
      {
        if ( *(double *)&eval->opStack[4 * eval->valStackPos + 1022] == 0.0 )
        {
          v25 = "divide by zero";
          _CxxThrowException(&v25, &PA.deinit);
        }
        *(double *)&eval->opStack[4 * eval->valStackPos + 1018] = *(double *)&eval->opStack[4 * eval->valStackPos + 1018]
                                                                / *(double *)&eval->opStack[4 * eval->valStackPos + 1022];
      }
      --eval->valStackPos;
      goto LABEL_121;
    case 8:
      Eval_PrepareBinaryOpSameTypes(eval);
      if ( eval->opStack[4 * eval->valStackPos + 1016] )
      {
        if ( eval->opStack[4 * eval->valStackPos + 1022] == EVAL_OP_LPAREN )
        {
          v22 = "divide by zero";
          _CxxThrowException(&v22, &PA.deinit);
        }
        eval->opStack[4 * eval->valStackPos + 1018] %= eval->opStack[4 * eval->valStackPos + 1022];
      }
      else
      {
        if ( *(double *)&eval->opStack[4 * eval->valStackPos + 1022] == 0.0 )
        {
          LODWORD(dQuotientFloor) = "divide by zero";
          _CxxThrowException(&dQuotientFloor, &PA.deinit);
        }
        v23 = floor(
                *(double *)&eval->opStack[4 * eval->valStackPos + 1018]
              / *(double *)&eval->opStack[4 * eval->valStackPos + 1022]);
        *(double *)&eval->opStack[4 * eval->valStackPos + 1018] = *(double *)&eval->opStack[4 * eval->valStackPos + 1018]
                                                                - *(double *)&eval->opStack[4 * eval->valStackPos + 1022]
                                                                * v23;
      }
      --eval->valStackPos;
      goto LABEL_121;
    case 9:
      Eval_PrepareBinaryOpSameTypes(eval);
      if ( eval->opStack[4 * eval->valStackPos + 1016] )
      {
        eval->opStack[4 * eval->valStackPos + 1018] <<= eval->opStack[4 * eval->valStackPos + 1022];
      }
      else
      {
        v4 = eval->valStackPos - 2;
        __libm_sse2_pow(a2, v9);
        *(double *)&eval->opStack[4 * eval->valStackPos + 1018] = 2.0 * eval->valStack[v4].u.d;
      }
      --eval->valStackPos;
      goto LABEL_121;
    case 10:
      Eval_PrepareBinaryOpSameTypes(eval);
      if ( eval->opStack[4 * eval->valStackPos + 1016] )
      {
        eval->opStack[4 * eval->valStackPos + 1018] >>= eval->opStack[4 * eval->valStackPos + 1022];
      }
      else
      {
        v5 = eval->valStackPos - 2;
        __libm_sse2_pow(a2, v9);
        *(double *)&eval->opStack[4 * eval->valStackPos + 1018] = 2.0 * eval->valStack[v5].u.d;
      }
      --eval->valStackPos;
      goto LABEL_121;
    case 11:
      if ( eval->opStack[4 * eval->valStackPos + 1020] )
      {
        if ( eval->opStack[4 * eval->valStackPos + 1016] == EVAL_OP_COLON )
        {
          v37 = "cannot bitwise invert strings";
          _CxxThrowException(&v37, &PA.deinit);
        }
      }
      else
      {
        eval->opStack[4 * eval->valStackPos + 1022] = (int)*(double *)&eval->opStack[4 * eval->valStackPos + 1022];
        eval->opStack[4 * eval->valStackPos + 1020] = EVAL_OP_RPAREN;
      }
      eval->opStack[4 * eval->valStackPos + 1022] = ~eval->opStack[4 * eval->valStackPos + 1022];
      goto LABEL_121;
    case 12:
      Eval_PrepareBinaryOpIntegers(eval);
      eval->opStack[4 * eval->valStackPos + 1018] &= eval->opStack[4 * eval->valStackPos + 1022];
      --eval->valStackPos;
      goto LABEL_121;
    case 13:
      Eval_PrepareBinaryOpIntegers(eval);
      eval->opStack[4 * eval->valStackPos + 1018] |= eval->opStack[4 * eval->valStackPos + 1022];
      --eval->valStackPos;
      goto LABEL_121;
    case 14:
      Eval_PrepareBinaryOpIntegers(eval);
      eval->opStack[4 * eval->valStackPos + 1018] ^= eval->opStack[4 * eval->valStackPos + 1022];
      --eval->valStackPos;
      goto LABEL_121;
    case 15:
      if ( eval->opStack[4 * eval->valStackPos + 1020] )
      {
        if ( eval->opStack[4 * eval->valStackPos + 1020] != EVAL_OP_RPAREN )
        {
          v38 = "cannot logical invert strings";
          _CxxThrowException(&v38, &PA.deinit);
        }
        eval->opStack[4 * eval->valStackPos + 1022] = eval->opStack[4 * eval->valStackPos + 1022] == EVAL_OP_LPAREN;
      }
      else
      {
        v39 = *(double *)&eval->opStack[4 * eval->valStackPos + 1022] == 0.0;
        eval->opStack[4 * eval->valStackPos + 1022] = v39;
      }
      eval->opStack[4 * eval->valStackPos + 1020] = EVAL_OP_RPAREN;
      goto LABEL_121;
    case 16:
      Eval_PrepareBinaryOpBoolean(eval);
      eval->opStack[4 * eval->valStackPos + 1018] &= eval->opStack[4 * eval->valStackPos + 1022];
      --eval->valStackPos;
      goto LABEL_121;
    case 17:
      Eval_PrepareBinaryOpBoolean(eval);
      eval->opStack[4 * eval->valStackPos + 1018] |= eval->opStack[4 * eval->valStackPos + 1022];
      --eval->valStackPos;
      goto LABEL_121;
    case 18:
      if ( eval->valStackPos >= 2
        && eval->opStack[4 * eval->valStackPos + 1016] == EVAL_OP_COLON
        && eval->opStack[4 * eval->valStackPos + 1020] == EVAL_OP_COLON )
      {
        v21 = _stricmp(
                (const char *)eval->opStack[4 * eval->valStackPos + 1018],
                (const char *)eval->opStack[4 * eval->valStackPos + 1022]) == 0;
        free((void *)eval->opStack[4 * eval->valStackPos + 1018]);
        free((void *)eval->opStack[4 * eval->valStackPos + 1022]);
        eval->opStack[4 * eval->valStackPos + 1016] = EVAL_OP_RPAREN;
        eval->opStack[4 * eval->valStackPos + 1018] = v21;
      }
      else
      {
        Eval_PrepareBinaryOpSameTypes(eval);
        if ( eval->opStack[4 * eval->valStackPos + 1016] )
        {
          eval->opStack[4 * eval->valStackPos + 1018] = eval->opStack[4 * eval->valStackPos + 1018] == eval->opStack[4 * eval->valStackPos + 1022];
        }
        else
        {
          v20 = *(double *)&eval->opStack[4 * eval->valStackPos + 1018] == *(double *)&eval->opStack[4 * eval->valStackPos + 1022];
          eval->opStack[4 * eval->valStackPos + 1018] = v20;
          eval->opStack[4 * eval->valStackPos + 1016] = EVAL_OP_RPAREN;
        }
      }
      --eval->valStackPos;
      goto LABEL_121;
    case 19:
      if ( eval->valStackPos >= 2
        && eval->opStack[4 * eval->valStackPos + 1016] == EVAL_OP_COLON
        && eval->opStack[4 * eval->valStackPos + 1020] == EVAL_OP_COLON )
      {
        v19 = _stricmp(
                (const char *)eval->opStack[4 * eval->valStackPos + 1018],
                (const char *)eval->opStack[4 * eval->valStackPos + 1022]) == 0;
        free((void *)eval->opStack[4 * eval->valStackPos + 1018]);
        free((void *)eval->opStack[4 * eval->valStackPos + 1022]);
        eval->opStack[4 * eval->valStackPos + 1016] = EVAL_OP_RPAREN;
        eval->opStack[4 * eval->valStackPos + 1018] = !v19;
      }
      else
      {
        Eval_PrepareBinaryOpSameTypes(eval);
        if ( eval->opStack[4 * eval->valStackPos + 1016] )
        {
          eval->opStack[4 * eval->valStackPos + 1018] = eval->opStack[4 * eval->valStackPos + 1018] != eval->opStack[4 * eval->valStackPos + 1022];
        }
        else
        {
          v18 = *(double *)&eval->opStack[4 * eval->valStackPos + 1018] != *(double *)&eval->opStack[4 * eval->valStackPos + 1022];
          eval->opStack[4 * eval->valStackPos + 1018] = v18;
          eval->opStack[4 * eval->valStackPos + 1016] = EVAL_OP_RPAREN;
        }
      }
      --eval->valStackPos;
      goto LABEL_121;
    case 20:
      Eval_PrepareBinaryOpSameTypes(eval);
      if ( eval->opStack[4 * eval->valStackPos + 1016] )
      {
        eval->opStack[4 * eval->valStackPos + 1018] = eval->opStack[4 * eval->valStackPos + 1018] < eval->opStack[4 * eval->valStackPos + 1022];
      }
      else
      {
        v17 = *(double *)&eval->opStack[4 * eval->valStackPos + 1022] > *(double *)&eval->opStack[4 * eval->valStackPos
                                                                                                + 1018];
        eval->opStack[4 * eval->valStackPos + 1018] = v17;
        eval->opStack[4 * eval->valStackPos + 1016] = EVAL_OP_RPAREN;
      }
      --eval->valStackPos;
      goto LABEL_121;
    case 21:
      Eval_PrepareBinaryOpSameTypes(eval);
      if ( eval->opStack[4 * eval->valStackPos + 1016] )
      {
        eval->opStack[4 * eval->valStackPos + 1018] = eval->opStack[4 * eval->valStackPos + 1018] <= eval->opStack[4 * eval->valStackPos + 1022];
      }
      else
      {
        v16 = *(double *)&eval->opStack[4 * eval->valStackPos + 1022] >= *(double *)&eval->opStack[4 * eval->valStackPos
                                                                                                 + 1018];
        eval->opStack[4 * eval->valStackPos + 1018] = v16;
        eval->opStack[4 * eval->valStackPos + 1016] = EVAL_OP_RPAREN;
      }
      --eval->valStackPos;
      goto LABEL_121;
    case 22:
      Eval_PrepareBinaryOpSameTypes(eval);
      if ( eval->opStack[4 * eval->valStackPos + 1016] )
      {
        eval->opStack[4 * eval->valStackPos + 1018] = eval->opStack[4 * eval->valStackPos + 1018] > eval->opStack[4 * eval->valStackPos + 1022];
      }
      else
      {
        v15 = *(double *)&eval->opStack[4 * eval->valStackPos + 1018] > *(double *)&eval->opStack[4 * eval->valStackPos
                                                                                                + 1022];
        eval->opStack[4 * eval->valStackPos + 1018] = v15;
        eval->opStack[4 * eval->valStackPos + 1016] = EVAL_OP_RPAREN;
      }
      --eval->valStackPos;
      goto LABEL_121;
    case 23:
      Eval_PrepareBinaryOpSameTypes(eval);
      if ( eval->opStack[4 * eval->valStackPos + 1016] )
      {
        eval->opStack[4 * eval->valStackPos + 1018] = eval->opStack[4 * eval->valStackPos + 1018] >= eval->opStack[4 * eval->valStackPos + 1022];
      }
      else
      {
        i = *(double *)&eval->opStack[4 * eval->valStackPos + 1018] >= *(double *)&eval->opStack[4 * eval->valStackPos
                                                                                               + 1022];
        eval->opStack[4 * eval->valStackPos + 1018] = i;
        eval->opStack[4 * eval->valStackPos + 1016] = EVAL_OP_RPAREN;
      }
      --eval->valStackPos;
LABEL_121:
      result = 1;
      break;
    default:
      if ( !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\eval.cpp", 488, 0, "unknown operator type") )
        __debugbreak();
      result = 0;
      break;
  }
  return result;
}

void __cdecl Eval_PrepareBinaryOpSameTypes(Eval *eval)
{
  int v1; // eax
  const char *v2; // [esp+4h] [ebp-8h] BYREF
  const char *pExceptionObject; // [esp+8h] [ebp-4h] BYREF

  if ( eval->valStackPos < 2 )
  {
    pExceptionObject = "missing operand (for example, 'a + ' or ' / b')";
    _CxxThrowException(&pExceptionObject, &PA.deinit);
  }
  if ( eval->opStack[4 * eval->valStackPos + 1016] == EVAL_OP_COLON
    || eval->opStack[4 * eval->valStackPos + 1020] == EVAL_OP_COLON )
  {
    v2 = "operation not valid on strings";
    _CxxThrowException(&v2, &PA.deinit);
  }
  if ( eval->opStack[4 * eval->valStackPos + 1016] != eval->opStack[4 * eval->valStackPos + 1020] )
  {
    if ( eval->opStack[4 * eval->valStackPos + 1016] == EVAL_OP_RPAREN )
    {
      *(double *)&eval->opStack[4 * eval->valStackPos + 1018] = (double)(int)eval->opStack[4 * eval->valStackPos + 1018];
      v1 = eval->valStackPos - 2;
    }
    else
    {
      *(double *)&eval->opStack[4 * eval->valStackPos + 1022] = (double)(int)eval->opStack[4 * eval->valStackPos + 1022];
      v1 = eval->valStackPos - 1;
    }
    eval->valStack[v1].type = EVAL_VALUE_DOUBLE;
  }
}

void __cdecl Eval_PrepareBinaryOpIntegers(Eval *eval)
{
  const char *v1; // [esp+0h] [ebp-8h] BYREF
  const char *pExceptionObject; // [esp+4h] [ebp-4h] BYREF

  if ( eval->valStackPos < 2 )
  {
    pExceptionObject = "missing operand (for example, 'a + ' or ' / b')";
    _CxxThrowException(&pExceptionObject, &PA.deinit);
  }
  if ( eval->opStack[4 * eval->valStackPos + 1016] == EVAL_OP_COLON
    || eval->opStack[4 * eval->valStackPos + 1020] == EVAL_OP_COLON )
  {
    v1 = "operation not valid on strings";
    _CxxThrowException(&v1, &PA.deinit);
  }
  if ( eval->opStack[4 * eval->valStackPos + 1016] == EVAL_OP_LPAREN )
  {
    eval->opStack[4 * eval->valStackPos + 1018] = (int)*(double *)&eval->opStack[4 * eval->valStackPos + 1018];
    eval->opStack[4 * eval->valStackPos + 1016] = EVAL_OP_RPAREN;
  }
  if ( eval->opStack[4 * eval->valStackPos + 1020] == EVAL_OP_LPAREN )
  {
    eval->opStack[4 * eval->valStackPos + 1022] = (int)*(double *)&eval->opStack[4 * eval->valStackPos + 1022];
    eval->opStack[4 * eval->valStackPos + 1020] = EVAL_OP_RPAREN;
  }
}

void __cdecl Eval_PrepareBinaryOpBoolean(Eval *eval)
{
  const char *v1; // [esp+8h] [ebp-8h] BYREF
  const char *pExceptionObject; // [esp+Ch] [ebp-4h] BYREF

  if ( eval->valStackPos < 2 )
  {
    pExceptionObject = "missing operand (for example, 'a + ' or ' / b')";
    _CxxThrowException(&pExceptionObject, &PA.deinit);
  }
  if ( eval->opStack[4 * eval->valStackPos + 1016] == EVAL_OP_COLON
    || eval->opStack[4 * eval->valStackPos + 1020] == EVAL_OP_COLON )
  {
    v1 = "operation not valid on strings";
    _CxxThrowException(&v1, &PA.deinit);
  }
  if ( eval->opStack[4 * eval->valStackPos + 1016] )
  {
    eval->opStack[4 * eval->valStackPos + 1018] = eval->opStack[4 * eval->valStackPos + 1018] != EVAL_OP_LPAREN;
  }
  else
  {
    eval->opStack[4 * eval->valStackPos + 1018] = *(double *)&eval->opStack[4 * eval->valStackPos + 1018] != 0.0;
    eval->opStack[4 * eval->valStackPos + 1016] = EVAL_OP_RPAREN;
  }
  if ( eval->opStack[4 * eval->valStackPos + 1020] )
  {
    eval->opStack[4 * eval->valStackPos + 1022] = eval->opStack[4 * eval->valStackPos + 1022] != EVAL_OP_LPAREN;
  }
  else
  {
    eval->opStack[4 * eval->valStackPos + 1022] = *(double *)&eval->opStack[4 * eval->valStackPos + 1022] != 0.0;
    eval->opStack[4 * eval->valStackPos + 1020] = EVAL_OP_RPAREN;
  }
}

bool __cdecl Eval_IsUnaryOp(const Eval *eval)
{
  return !eval->valStackPos || eval->pushedOp;
}

char __cdecl Eval_PushInteger(Eval *eval, int value)
{
  if ( !Eval_CanPushValue(eval) )
    return 0;
  eval->valStack[eval->valStackPos].type = EVAL_VALUE_INT;
  eval->valStack[eval->valStackPos++].u.i = value;
  eval->pushedOp = 0;
  return 1;
}

bool __cdecl Eval_CanPushValue(const Eval *eval)
{
  const char *pExceptionObject; // [esp+0h] [ebp-4h] BYREF

  if ( !eval->valStackPos )
    return 1;
  if ( eval->valStackPos == 1024 )
  {
    pExceptionObject = "evaluation stack overflow - expression is too complex";
    _CxxThrowException(&pExceptionObject, &PA.deinit);
  }
  return eval->pushedOp;
}

char __cdecl Eval_PushNumber(Eval *eval, long double value)
{
  if ( !Eval_CanPushValue(eval) )
    return 0;
  eval->valStack[eval->valStackPos].type = EVAL_VALUE_DOUBLE;
  eval->valStack[eval->valStackPos++].u.d = value;
  eval->pushedOp = 0;
  return 1;
}

EvalValue * Eval_Solve@<eax>(long double a1@<esi:edi>, EvalValue *result, Eval *eval)
{
  int v4; // [esp+0h] [ebp-2Ch] BYREF
  const char *v6; // [esp+14h] [ebp-18h] BYREF
  unsigned int pExceptionObject[5]; // [esp+18h] [ebp-14h] BYREF
  int savedregs; // [esp+2Ch] [ebp+0h] BYREF

  pExceptionObject[1] = &v4;
  pExceptionObject[4] = 0;
  if ( eval->parenCount )
  {
    pExceptionObject[0] = "missing ')'";
    _CxxThrowException(pExceptionObject, &PA.deinit);
  }
  while ( Eval_EvaluationStep((int)&savedregs, a1, eval) )
    ;
  if ( eval->opStackPos
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\universal\\eval.cpp",
          649,
          0,
          "%s",
          "eval->opStackPos == 0") )
  {
    __debugbreak();
  }
  if ( eval->valStackPos > 1 )
  {
    v6 = "extra operand (for example, 'a b +')";
    _CxxThrowException(&v6, &PA.deinit);
  }
  *result = eval->valStack[0];
  return result;
}

bool __cdecl Eval_OperatorForToken(const char *text, EvalOperatorType *op)
{
  bool result; // al

  if ( !text && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\eval.cpp", 674, 0, "%s", "text") )
    __debugbreak();
  if ( !op && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\universal\\eval.cpp", 675, 0, "%s", "op") )
    __debugbreak();
  switch ( *text )
  {
    case '!':
      if ( text[1] == '=' )
        *op = EVAL_OP_NOT_EQUAL;
      else
        *op = EVAL_OP_LOGICAL_NOT;
      result = 1;
      break;
    case '%':
      *op = EVAL_OP_MODULUS;
      result = 1;
      break;
    case '&':
      if ( text[1] == '&' )
        *op = EVAL_OP_LOGICAL_AND;
      else
        *op = EVAL_OP_BITWISE_AND;
      result = 1;
      break;
    case '(':
      *op = EVAL_OP_LPAREN;
      result = 1;
      break;
    case ')':
      *op = EVAL_OP_RPAREN;
      result = 1;
      break;
    case '*':
      *op = EVAL_OP_MULTIPLY;
      result = 1;
      break;
    case '+':
      *op = EVAL_OP_PLUS;
      result = 1;
      break;
    case '-':
      *op = EVAL_OP_MINUS;
      result = 1;
      break;
    case '/':
      *op = EVAL_OP_DIVIDE;
      result = 1;
      break;
    case ':':
      *op = EVAL_OP_COLON;
      result = 1;
      break;
    case '<':
      if ( text[1] == '<' )
      {
        *op = EVAL_OP_LSHIFT;
        result = 1;
      }
      else
      {
        if ( text[1] == '=' )
          *op = EVAL_OP_LESS_EQUAL;
        else
          *op = EVAL_OP_LESS;
        result = 1;
      }
      break;
    case '=':
      if ( text[1] != '=' )
        goto LABEL_46;
      *op = EVAL_OP_EQUALS;
      result = 1;
      break;
    case '>':
      if ( text[1] == '>' )
      {
        *op = EVAL_OP_RSHIFT;
        result = 1;
      }
      else
      {
        if ( text[1] == 61 )
          *op = EVAL_OP_GREATER_EQUAL;
        else
          *op = EVAL_OP_GREATER;
        result = 1;
      }
      break;
    case '?':
      *op = EVAL_OP_QUESTION;
      result = 1;
      break;
    case '^':
      *op = EVAL_OP_BITWISE_XOR;
      result = 1;
      break;
    case '|':
      if ( text[1] == '|' )
        *op = EVAL_OP_LOGICAL_OR;
      else
        *op = EVAL_OP_BITWISE_OR;
      result = 1;
      break;
    case '~':
      *op = EVAL_OP_BITWISE_NOT;
      result = 1;
      break;
    default:
LABEL_46:
      result = 0;
      break;
  }
  return result;
}

