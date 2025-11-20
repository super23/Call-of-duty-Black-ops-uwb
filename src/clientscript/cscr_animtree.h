#pragma once

XExpr::MathTypes __cdecl EqualTypeSameResult(const ParseValue *params, int iNumParams, scriptInstance_t inst);
void __cdecl AnimTreeCompileError(scriptInstance_t inst, const char *msg);
XExpr::MathTypes __cdecl EqualTypeAllowScalar(const ParseValue *params, int iNumParams, scriptInstance_t inst);
int __cdecl FloatsToVec(const ParseValue *params, int iNumParams, scriptInstance_t inst);
int __cdecl VecsToFloat(const ParseValue *params, int iNumParams, scriptInstance_t inst);
int __cdecl Vec3ToVec3(const ParseValue *params, int iNumParams, scriptInstance_t inst);
void __cdecl SetAnimCheck(int bAnimCheck, scriptInstance_t inst);
void __cdecl Scr_EmitAnimation(scriptInstance_t inst, char *pos, unsigned int animName, unsigned int sourcePos);
void __cdecl Scr_EmitAnimationInternal(scriptInstance_t inst, char *pos, unsigned int animName, unsigned int names);
void __cdecl SetVariableValue(scriptInstance_t inst, unsigned int id, VariableValue *value);
void __cdecl RemoveRefToValue(scriptInstance_t inst, int type, VariableUnion u);
void __cdecl RemoveRefToVector(scriptInstance_t inst, const float *vectorValue);
unsigned int __cdecl StoreExprInList(scriptInstance_t inst, unsigned int exprDataArray, ParseValue exprData);
void __cdecl SetIntVariable(scriptInstance_t inst, unsigned int theArray, int integerValue, char *strVarName);
void __cdecl SetPtrVariable(scriptInstance_t inst, unsigned int theArray, unsigned int arrayData, char *strVarName);
unsigned int __cdecl StoreValueInList(scriptInstance_t inst, unsigned int valueDataArray, unsigned int valueData);
unsigned int __cdecl ParseValueFromValueList(ScriptTokenizer *tokenizer, scriptInstance_t inst, unsigned int valueName);
char *__thiscall ScriptTokenizer::PopToken(ScriptTokenizer *this);
void __thiscall ScriptTokenizer::PushNextToken(ScriptTokenizer *this);
char *__thiscall ScriptTokenizer::GetAndRemoveToken(ScriptTokenizer *this);
void __thiscall ScriptTokenizer::ParseIntoTokens(ScriptTokenizer *this, char *strString);
void __cdecl SetFloatVariable(scriptInstance_t inst, unsigned int theArray, float floatValue, char *strVarName);
int __cdecl DetermineValueType(ScriptTokenizer *tokenizer);
char *__thiscall ScriptTokenizer::PeekToken(ScriptTokenizer *this);
int __cdecl Scr_GetAnimsIndex(const XAnim_s *anims, scriptInstance_t inst);
XAnim_s *__cdecl Scr_GetClientAnims(unsigned int index, scriptInstance_t inst);
XAnim_s *__cdecl Scr_GetAnims(unsigned int index, scriptInstance_t inst);
void __cdecl Scr_UsingTree(scriptInstance_t inst, const char *filename, unsigned int sourcePos);
unsigned int __cdecl Scr_UsingTreeInternal(scriptInstance_t inst, const char *filename, unsigned int *index, int user);
void __cdecl Scr_ClientUsingTree(scriptInstance_t inst, const char *filename);
void __cdecl Scr_LoadAnimTreeAtIndex(
        scriptInstance_t inst,
        unsigned int index,
        void *(__cdecl *Alloc)(int),
        int user,
        bool modCheckSum);
unsigned int __cdecl GetVariableName(scriptInstance_t inst, unsigned int id);
int __cdecl Scr_GetAnimTreeSize(scriptInstance_t inst, unsigned int parentNode);
unsigned int __cdecl FindFirstSibling(scriptInstance_t inst, unsigned int id);
unsigned int __cdecl FindNextSibling(scriptInstance_t inst, unsigned int id);
unsigned int __cdecl Scr_GetAnimTreeValueCount(scriptInstance_t inst, unsigned int rootData);
unsigned int __cdecl GetNamedArrayVarArray(scriptInstance_t inst, unsigned int theArray, char *strVarName);
void __cdecl ConnectScriptToAnim(
        scriptInstance_t inst,
        unsigned int names,
        int index,
        unsigned int filename,
        unsigned int name,
        unsigned __int16 treeIndex);
void __cdecl Scr_CheckAnimsDefined(scriptInstance_t inst, unsigned int names, unsigned int filename);
void __cdecl Scr_PrecacheAnimationTree(scriptInstance_t inst, unsigned int parentNode);
bool __cdecl Scr_LoadAnimTreeInternal(
        scriptInstance_t inst,
        const char *filename,
        unsigned int parentNode,
        unsigned int names);
void __cdecl Scr_AnimTreeParse(scriptInstance_t inst, const char *pos, unsigned int parentNode, unsigned int names);
char __cdecl AnimTreeParseInternal(
        scriptInstance_t inst,
        unsigned int parentNode,
        unsigned int names,
        bool bIncludeParent,
        bool bLoop,
        bool bComplete);
bool __thiscall ScriptTokenizer::IsAtEnd(ScriptTokenizer *this);
char __cdecl ParseNode(
        ScriptTokenizer *tokenizer,
        scriptInstance_t inst,
        unsigned int parentNode,
        unsigned int rootData,
        unsigned int names,
        bool bLoop,
        bool bComplete);
ParseValue __cdecl ParseTopRankExpr(ScriptTokenizer *tokenizer, scriptInstance_t inst, unsigned int rootData);
int __cdecl DetermineParseType(ScriptTokenizer *tokenizer);
bool __cdecl IsStandardParam(const char *strParamName);
int __cdecl GetStdParamIx(const char *strParamName);
ParseValue __cdecl BuildExpr(ExpressionParseTypes eCurrType, ParseValue *params, int iNumParams, scriptInstance_t inst);
void __cdecl CreateParameterVarName(char *strOutString, unsigned int iMaxNumChars, int iOperandIx);
ParseValue __cdecl ParseRank2Expr(ScriptTokenizer *tokenizer, scriptInstance_t inst, unsigned int rootData);
ParseValue __cdecl ParseBottomRankExpr(ScriptTokenizer *tokenizer, scriptInstance_t inst, unsigned int rootData);
const char *__cdecl ConvertParseTypeToStr(ExpressionParseTypes eType);
void __cdecl RequireParseTypePopOrError(ScriptTokenizer *tokenizer, scriptInstance_t inst, ExpressionParseTypes eTest);
ParseValue __cdecl ParseFunctionCall(
        ExpressionParseTypes eFuncType,
        ScriptTokenizer *tokenizer,
        scriptInstance_t inst,
        unsigned int rootData);
ParseValue __cdecl ParseStdParameter(ScriptTokenizer *tokenizer, scriptInstance_t inst);
ParseValue __cdecl ParseParameter(ScriptTokenizer *tokenizer, scriptInstance_t inst, unsigned int rootData);
unsigned int __cdecl GetNamedArrayVarInt(scriptInstance_t inst, unsigned int theArray, char *strVarName);
int __cdecl GetAnimTreeParseProperties(ScriptTokenizer *tokenizer);
void __cdecl StoreOptionsInNode(
        scriptInstance_t inst,
        unsigned int currentAnimArray,
        unsigned int iCurrOptionIx,
        unsigned int optionName,
        unsigned int iEntryIndex);
void __cdecl StoreValuesInNode(
        scriptInstance_t inst,
        unsigned int currentAnimArray,
        unsigned int iCurrIx,
        unsigned int iEntryIndex);
void __cdecl ParseParameterDef(ScriptTokenizer *tokenizer, scriptInstance_t inst, unsigned int rootData);
int __cdecl Scr_CreateAllAnimTreeData(
        scriptInstance_t inst,
        unsigned int rootData,
        unsigned int names,
        scr_animtree_t *animtree,
        unsigned int filenameId,
        unsigned int index,
        void *(__cdecl *Alloc)(int));
int __cdecl Scr_CreateAnimationTree(
        scriptInstance_t inst,
        unsigned int parentNode,
        unsigned int rootData,
        unsigned int names,
        XAnim_s *anims,
        unsigned int childIndex,
        const char *parentName,
        unsigned int parentIndex,
        unsigned int filename,
        int treeIndex,
        unsigned __int16 flags,
        void *(__cdecl *Alloc)(int),
        unsigned int paramMap);
void __cdecl Scr_ProcessNodeValues(
        scriptInstance_t inst,
        XAnim_s *anims,
        unsigned int name,
        unsigned int rootNode,
        unsigned int animIndex,
        unsigned int arrayNode);
void __cdecl CreateValues(scriptInstance_t inst, XAnim_s *anims, unsigned int valueArray);
double __cdecl GetNamedArrayVarFloat(scriptInstance_t inst, unsigned int theArray, char *strVarName);
scr_animtree_t __cdecl CScr_RetrieveAnimTree(
        const char *strAnimTreeName,
        unsigned int names,
        unsigned int filename,
        unsigned __int16 iOurTreeIx);
scr_animtree_t __cdecl Scr_FindAnimTree(scriptInstance_t inst, const char *filename);
VariableValue __cdecl Scr_EvalVariable(scriptInstance_t inst, unsigned int id);
void __cdecl AddRefToValue(scriptInstance_t inst, int type, VariableUnion u);
void __cdecl AddRefToObject(scriptInstance_t inst, unsigned int id);
void __cdecl AddRefToVector(scriptInstance_t inst, const float *vectorValue);
void __cdecl Scr_FindAnim(
        scriptInstance_t inst,
        const char *filename,
        const char *animName,
        scr_anim_s *anim,
        int user);
void __cdecl Scr_ShutdownAnimTrees(int user);
void __cdecl RemoveRefToValue(scriptInstance_t inst, VariableValue *value);
