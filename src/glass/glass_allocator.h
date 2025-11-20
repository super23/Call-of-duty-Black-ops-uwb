#pragma once

void __thiscall SmallAllocator::Init(SmallAllocator *this, void *buffer, unsigned int bs, unsigned int nb);
void **__thiscall SmallAllocator::Allocate(SmallAllocator *this, unsigned int size);
void __thiscall SmallAllocator::Free(SmallAllocator *this, void **ptr, unsigned int num);
bool __thiscall SmallAllocator::IsValidPointer(SmallAllocator *this, void *ptr);
void __thiscall Allocator::Memory::Init(Allocator::Memory *this);
char __thiscall Allocator::Memory::MakeFree(Allocator::Memory *this);
void __thiscall Allocator::Init(Allocator *this, void *buf, int size);
void __thiscall Allocator::FreeAll(Allocator *this);
Allocator::Memory **__thiscall Allocator::Allocate(Allocator *this, int size, void *userData);
void __thiscall Allocator::Free(Allocator *this, unsigned int *ptr);
unsigned int __thiscall Allocator::GetMemorySize(Allocator *this, unsigned int *ptr);
void __thiscall Allocator::Split(Allocator *this, Allocator::Memory *mem, int size);
void __thiscall Allocator::Merge(Allocator *this, Allocator::Memory *mem1, Allocator::Memory *mem2);
int __thiscall Allocator::Defrag(Allocator *this, void (__cdecl *func)(void *), int count);
int __thiscall Allocator::GetFree(Allocator *this);
int __thiscall Allocator::GetLargestFree(Allocator *this);
