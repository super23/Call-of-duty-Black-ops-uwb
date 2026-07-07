#include "db_stringtable_load.h"
#include <cstdint>
#include <clientscript/cscr_stringlist.h>
#include "db_file_load.h"

namespace
{
    unsigned __int16 DB_ResolveLoadedScriptString(unsigned __int16 index)
    {
        // `Load_TempStringCustom` interns each table entry and leaves the script-string id
        // encoded in the pointer slot until per-asset references are patched up here.
        return static_cast<unsigned __int16>(
            reinterpret_cast<std::uintptr_t>(varXAssetList->stringList.strings[index]));
    }
}

void __cdecl Load_ScriptStringCustom(unsigned __int16 *scriptString)
{
    *scriptString = DB_ResolveLoadedScriptString(*scriptString);
}

void __cdecl Mark_ScriptStringCustom(unsigned __int16 *scriptString)
{
    if ( *scriptString )
        SL_AddUser(*scriptString, 4u, SCRIPTINSTANCE_SERVER);
}

