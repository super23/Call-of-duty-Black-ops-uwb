#pragma once

unsigned __int8 *__cdecl SE_LoadFileData(const char *psFileName);
void __cdecl SE_FreeFileDataAfterLoad(unsigned __int8 *psLoadedFile);
int __cdecl SE_BuildFileList(
        const char *psStartDir,
        std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > *strResults);
void __cdecl SE_R_ListFiles(
        char *psExtension,
        const char *psDir,
        std::basic_string<char,std::char_traits<char>,Allocator<char,LocalizeStringName> > *strResults);
