#pragma once

bool __cdecl Eval_AnyMissingOperands(const Eval *eval);
char  Eval_PushOperator@<al>(long double a1@<esi:edi>, Eval *eval, EvalOperatorType op);
// local variable allocation has failed, the output may be wrong!
bool  Eval_EvaluationStep@<al>(int a1@<ebp>, long double a2@<esi:edi>, Eval *eval);
void __cdecl Eval_PrepareBinaryOpSameTypes(Eval *eval);
void __cdecl Eval_PrepareBinaryOpIntegers(Eval *eval);
void __cdecl Eval_PrepareBinaryOpBoolean(Eval *eval);
bool __cdecl Eval_IsUnaryOp(const Eval *eval);
char __cdecl Eval_PushInteger(Eval *eval, int value);
bool __cdecl Eval_CanPushValue(const Eval *eval);
char __cdecl Eval_PushNumber(Eval *eval, long double value);
EvalValue * Eval_Solve@<eax>(long double a1@<esi:edi>, EvalValue *result, Eval *eval);
bool __cdecl Eval_OperatorForToken(const char *text, EvalOperatorType *op);
