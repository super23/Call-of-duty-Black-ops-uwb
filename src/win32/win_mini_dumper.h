#pragma once

void __cdecl Sys_StartMiniDump(bool prompt);
bool __cdecl Sys_IsMiniDumpStarted();
miniDumper *__thiscall miniDumper::miniDumper(miniDumper *this, bool bPromptUserForMiniDump);
void __thiscall miniDumper::~miniDumper(miniDumper *this);
int __stdcall miniDumper::unhandledExceptionHandler(_EXCEPTION_POINTERS *pExceptionInfo);
void __thiscall miniDumper::setMiniDumpFileName(miniDumper *this);
char __thiscall miniDumper::getImpersonationToken(miniDumper *this, void **phToken);
bool __thiscall miniDumper::enablePrivilege(
        miniDumper *this,
        const char *pszPriv,
        void *hToken,
        _TOKEN_PRIVILEGES *ptpOld);
bool __thiscall miniDumper::restorePrivilege(miniDumper *this, void *hToken, _TOKEN_PRIVILEGES *ptpOld);
int __thiscall miniDumper::writeMiniDump(miniDumper *this, _EXCEPTION_POINTERS *pExceptionInfo);
