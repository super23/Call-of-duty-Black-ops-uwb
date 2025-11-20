#pragma once

int __cdecl LargeLocalBegin(int size);
unsigned int __cdecl LargeLocalRoundSize(int size);
void __cdecl LargeLocalEnd(int startPos);
unsigned __int8 *__cdecl LargeLocalGetBuf(int startPos, int size);
LargeLocal *__thiscall LargeLocal::LargeLocal(LargeLocal *this, int sizeParam);
int __cdecl LargeLocalBeginRight(int size);
void __thiscall LargeLocal::~LargeLocal(LargeLocal *this);
void __cdecl LargeLocalEndRight(int startPos);
unsigned __int8 *__thiscall LargeLocal::GetBuf(LargeLocal *this);
void __cdecl LargeLocalReset();
