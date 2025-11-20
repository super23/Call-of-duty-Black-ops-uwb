#pragma once

void __cdecl Expression_Free(ExpressionStatement *statement);
expressionRpn *__cdecl MakeRPN(expressionEntry *entry, int *length);
char __cdecl Expression_Parse(
        const char **text,
        ExpressionStatement *statement,
        void *compileBuffer,
        int compileBufferSize);
const char *__cdecl GetOperandAsString(Operand operand);
expressionEntry *__cdecl Expression_Alloc(ExpressionAllocState *allocState, int size);
bool __cdecl Expression_OpIsFunction(expOperationEnum op);
int __cdecl Expression_ParseOperatorToken(const char *token);
expressionEntry *__cdecl Expression_HashOperand(const char *str, ExpressionAllocState *alloc);
expressionEntry *__cdecl Expression_StringOperand(const char *str, ExpressionAllocState *alloc);
char __cdecl Expression_Read(int handle, ExpressionStatement *statement);
void __cdecl Expression_Init();
