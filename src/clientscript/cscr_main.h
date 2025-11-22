#pragma once

enum scriptInstance_t : __int32
{                                       // XREF: ScriptTokenizer/r
                                        // ?Path_CallFunctionForNodes@@YAXW4scriptInstance_t@@P6AX0PAUpathnode_t@@@Z@Z/r ...
    SCRIPTINSTANCE_SERVER = 0x0,
    SCRIPTINSTANCE_CLIENT = 0x1,
    SCRIPT_INSTANCE_MAX   = 0x2,
};

struct ScriptFunctions // sizeof=0xC
{                                       // XREF: ?CGScr_LoadClientScriptsAndAnims@@YAXXZ/r
                                        // CGScr_LoadScriptsAndAnims/r
    int maxSize;                        // XREF: CGScr_LoadClientScriptsAndAnims(void)+9/w
                                        // CGScr_LoadClientScriptsAndAnims(void)+43/w ...
    int count;                          // XREF: CGScr_LoadClientScriptsAndAnims(void)+10/w
                                        // CGScr_LoadClientScriptsAndAnims(void)+40/r ...
    int *address;                       // XREF: CGScr_LoadClientScriptsAndAnims(void)+1D/w
                                        // CGScr_LoadScriptsAndAnims+1D/w
};

struct PrecacheEntry // sizeof=0x8
{                                       // XREF: ?Scr_LoadScript@@YAIW4scriptInstance_t@@PBD@Z/r
    unsigned __int16 filename;
    bool include;
    // padding byte
    unsigned int sourcePos;
};

enum fieldtype_t : __int32
{                                       // XREF: sentient_fields_s/r
                                        // node_field_t/r ...
    F_INT            = 0x0,             // XREF: .rdata:cent_fields/s
                                        // .rdata:fields/s ...
    F_SHORT          = 0x1,             // XREF: .data:fields_3/s
    F_BYTE           = 0x2,             // XREF: .rdata:fields_0/s
                                        // .rdata:aifields/s ...
    F_FLOAT          = 0x3,             // XREF: .rdata:fields/s
                                        // .rdata:fields_0/s ...
    F_LSTRING        = 0x4,             // XREF: .rdata:cent_fields/s
                                        // .rdata:fields/s ...
    F_STRING         = 0x5,             // XREF: .rdata:cent_fields/s
                                        // .rdata:client_spawn_fields/s ...
    F_VECTOR         = 0x6,             // XREF: .rdata:cent_fields/s
                                        // .rdata:client_spawn_fields/s ...
    F_ENTITY         = 0x7,             // XREF: .rdata:cent_fields/s
                                        // .rdata:fields/s
    F_ENTHANDLE      = 0x8,             // XREF: .rdata:aifields/s
                                        // .rdata:sentientfields/s ...
    F_ACTOR          = 0x9,
    F_SENTIENT       = 0xA,
    F_SENTIENTHANDLE = 0xB,             // XREF: .rdata:aifields/s
    F_CLIENT         = 0xC,
    F_PATHNODE       = 0xD,             // XREF: .rdata:sentientfields/s
                                        // .rdata:fields_2/s
    F_ACTORGROUP     = 0xE,
    F_VECTORHACK     = 0xF,             // XREF: .data:fields_3/s
    F_OBJECT         = 0x10,            // XREF: .rdata:fields/s
    F_MODEL          = 0x11,            // XREF: .rdata:client_spawn_fields/s
                                        // .rdata:fields_1/s ...
    F_BITFLAG        = 0x12,            // XREF: .rdata:fields/s
};





struct __declspec(align(4)) RefString // sizeof=0x8
{
    //$DA181ABD9DD4DBD447601B1FE2E6E195 ___u0;
    union //$DA181ABD9DD4DBD447601B1FE2E6E195 // sizeof=0x4
    {                                       // XREF: RefString/r
        //$119B815E6C15BED54461C272BD343858 __s0;
        struct //$119B815E6C15BED54461C272BD343858 // sizeof=0x4
        {                                       // XREF: $DA181ABD9DD4DBD447601B1FE2E6E195/r
            unsigned __int32 refCount : 16;
            unsigned __int32 user : 8;
            unsigned __int32 byteLen : 8;
        };
        volatile int data;
    };
    char str[1];
    // padding byte
    // padding byte
    // padding byte
};

namespace XExpr
{
    enum MathTypes : __int32
    {                                       // XREF: ParseValue/r
                                            // StandardParams/r ...
        NUMBER = 0x0,               // XREF: .data:g_stdParamList/s
        VECTOR_2D = 0x1,
        VECTOR_3D = 0x2,
        MAX_MATH_TYPES = 0x3,
    };

    enum MathOpcodes : __int32
    {                                       // XREF: ExpressionOperatorData/r
        OP_PUSH_VALUE = 0x0,
        OP_PUSH_PARAM = 0x1,
        OP_PUSH_STD_PARAM = 0x2,
        OP_ADD_VALUES = 0x3,         // XREF: .rdata:g_ExprOperatorList/s
        OP_SUBTRACT_2_FROM_1 = 0x4,         // XREF: .rdata:g_ExprOperatorList/s
        OP_MULTIPLY_VALUES = 0x5,         // XREF: .rdata:g_ExprOperatorList/s
        OP_DIVIDE_2_FROM_1 = 0x6,         // XREF: .rdata:g_ExprOperatorList/s
        OP_CONSTRUCT_VEC2 = 0x7,         // XREF: .rdata:g_ExprOperatorList/s
        OP_CONSTRUCT_VEC3 = 0x8,         // XREF: .rdata:g_ExprOperatorList/s
        OP_DOT_PRODUCT = 0x9,         // XREF: .rdata:g_ExprOperatorList/s
        OP_CROSS_PRODUCT = 0xA,         // XREF: .rdata:g_ExprOperatorList/s
        MAX_NUM_OPCODES = 0xB,         // XREF: .rdata:g_ExprOperatorList/s
    };

    enum ParamTypes : __int32
    {                                       // XREF: ParseParameter/r
        PT_FLOAT = 0x0,
        PT_NORM_FLOAT = 0x1,
        PT_VEC2 = 0x2,
        PT_NORM_VEC2 = 0x3,
        PT_UNIT_VEC2 = 0x4,
        PT_VEC3 = 0x5,
        PT_NORM_VEC3 = 0x6,
        PT_UNIT_VEC3 = 0x7,
        PT_MAX_PARAM_TYPES = 0x8,
    };
}

const struct ParseValue // sizeof=0x8
{                                       // XREF: ParseTopRankExpr+89/w
                                        // ParseRank2Expr+89/w ...
    unsigned int scriptExprData;        // XREF: StoreExprInList(scriptInstance_t,uint,ParseValue)+65/r
    // ParseNode+637/w ...
    XExpr::MathTypes exprType;          // XREF: StoreExprInList(scriptInstance_t,uint,ParseValue)+4A/r
    // ParseNode+644/w ...
};

bool __cdecl Scr_IsInOpcodeMemory(scriptInstance_t inst, const char *pos);
char __cdecl Scr_IsIdentifier(const char *token);
int __cdecl Scr_GetFunctionHandle(scriptInstance_t inst, const char *filename, const char *name);
unsigned int __cdecl SL_TransferToCanonicalString(scriptInstance_t inst, unsigned int stringValue);
unsigned int __cdecl SL_GetCanonicalString(scriptInstance_t inst, char *str);
void __cdecl Scr_BeginLoadScripts(scriptInstance_t inst, int user);
void __cdecl SL_BeginLoadScripts(scriptInstance_t inst);
void __cdecl Scr_BeginLoadAnimTrees(scriptInstance_t inst, int user);
int __cdecl Scr_ScanFile(scriptInstance_t inst, char *buf, int max_size);
unsigned int __cdecl Scr_LoadScriptInternal(
        scriptInstance_t inst,
        const char *filename,
        PrecacheEntry *entries,
        int entriesCount);
unsigned int __cdecl Scr_LoadScript(scriptInstance_t inst, char *filename);
void __cdecl Scr_PostCompileScripts(scriptInstance_t inst);
void __cdecl Scr_EndLoadScripts(scriptInstance_t inst);
void __cdecl Scr_PrecacheAnimTrees(scriptInstance_t inst, void *(__cdecl *Alloc)(int), int user, bool modChecksum);
void __cdecl Scr_EndLoadAnimTrees(scriptInstance_t inst);
void __cdecl Scr_FreeScripts(scriptInstance_t inst, unsigned __int8 sys);
void __cdecl Scr_GetGenericField(
        unsigned __int8 *b,
        fieldtype_t type,
        int ofs,
        scriptInstance_t inst,
        unsigned int whichbits);
void __cdecl Scr_SetGenericField(
        unsigned __int8 *b,
        fieldtype_t type,
        int ofs,
        scriptInstance_t inst,
        unsigned int whichbits);
