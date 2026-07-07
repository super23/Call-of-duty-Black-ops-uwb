#include "glass_allocator.h"
#include "glass_client.h"
#include <universal/assertive.h>

// Decomp: CoDMPServer.c:853876
void __thiscall SmallAllocator::Init(void *buffer, unsigned int bs, unsigned int nb)
{
    char *blockPtr;

    this->memory = buffer;
    this->blockSize = bs;
    this->numBlocks = nb;
    this->numUsed = 0;
    this->maxUsed = 0;
    this->freeHead = this->memory;
    blockPtr = (char *)this->memory;
    for (unsigned int blockIndex = 0; blockIndex < this->numBlocks - 1; ++blockIndex)
    {
        *(unsigned int *)blockPtr = (unsigned int)&blockPtr[this->blockSize];
        blockPtr += this->blockSize;
    }
    *(unsigned int *)blockPtr = 0;
}

// Decomp: CoDMPServer.c:853899
void **__thiscall SmallAllocator::Allocate(unsigned int size)
{
    if (size > this->blockSize
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_allocator.cpp",
            59,
            0,
            "%s",
            "size <= blockSize"))
    {
        __debugbreak();
    }
    if (!this->freeHead
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_allocator.cpp", 60, 0, "%s", "freeHead"))
    {
        __debugbreak();
    }
    ++this->numUsed;
    if ((signed int)this->maxUsed < (signed int)this->numUsed)
        this->maxUsed = this->numUsed;
    void **allocatedBlock = (void **)this->freeHead;
    this->freeHead = *allocatedBlock;
    return allocatedBlock;
}

// Decomp: CoDMPServer.c:853939
void __thiscall SmallAllocator::Free(void **ptr, unsigned int num)
{
    if (num != 1
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_allocator.cpp", 73, 0, "%s", "num == 1"))
    {
        __debugbreak();
    }
    if (!SmallAllocator::IsValidPointer(ptr)
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_allocator.cpp",
            74,
            0,
            "%s",
            "IsValidPointer( ptr )"))
    {
        __debugbreak();
    }
    if (!this->numUsed
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_allocator.cpp", 75, 0, "%s", "numUsed > 0"))
    {
        __debugbreak();
    }
    --this->numUsed;
    *ptr = this->freeHead;
    this->freeHead = ptr;
}

// Decomp: CoDMPServer.c:853983
bool __thiscall SmallAllocator::IsValidPointer(void *ptr)
{
    return ptr >= this->memory
        && !(((unsigned int)ptr - (unsigned int)this->memory) % this->blockSize)
        && (unsigned int)ptr - (unsigned int)this->memory < this->numBlocks * this->blockSize;
}

// Decomp: CoDMPServer.c:853991
void __thiscall Allocator::Memory::Init()
{
    this->next = 0;
    this->prev = 0;
    this->userData = 0;
    this->nextFree = 0;
    this->prevFree = 0;
}

// Decomp: CoDMPServer.c:854001
char __thiscall Allocator::Memory::MakeFree()
{
    Allocator::Memory *nextBlock;
    Allocator::Memory *prevBlock;

    for (nextBlock = this->next; nextBlock && !nextBlock->nextFree; nextBlock = nextBlock->next)
        ;
    if (nextBlock)
    {
        this->nextFree = nextBlock;
        this->prevFree = nextBlock->prevFree;
        nextBlock->prevFree = this;
        if (this->prevFree)
            this->prevFree->nextFree = this;
    }
    else
    {
        for (prevBlock = this->prev; prevBlock && !prevBlock->nextFree; prevBlock = prevBlock->prev)
            ;
        if (!prevBlock)
            return 0;
        this->prevFree = prevBlock;
        this->nextFree = prevBlock->nextFree;
        prevBlock->nextFree = this;
        if (this->nextFree)
            this->nextFree->prevFree = this;
    }
    return 1;
}

// Decomp: CoDMPServer.c:854032
void __thiscall Allocator::Init(void *buf, int size)
{
    this->buffer = buf;
    Allocator::Memory *end = (Allocator::Memory *)(((int)this->buffer + size - 21) & 0xFFFFFFF0);
    this->head = (Allocator::Memory *)(((int)this->buffer + 15) & 0xFFFFFFF0);
    this->head->Init();
    this->tail = end;
    this->tail->Init();
    Allocator::FreeAll();
    const int heapSpan = (char *)this->tail - (char *)this->head;
    this->maxUsed = heapSpan - Allocator::GetFree();
}

// Decomp: CoDMPServer.c:854051
void __thiscall Allocator::FreeAll()
{
    this->head->nextFree = this->tail;
    this->head->next = this->head->nextFree;
    this->head->prevFree = 0;
    this->head->prev = 0;
    this->tail->prevFree = this->head;
    this->tail->prev = this->tail->prevFree;
    this->tail->nextFree = 0;
    this->tail->next = 0;
    this->freeHead = this->head;
}

// Decomp: CoDMPServer.c:854068
Allocator::Memory **__thiscall Allocator::Allocate(int size, void *userData)
{
    if (this->freeHead == this->tail)
        return 0;

    Allocator::Memory *bestFit = 0;
    const signed int alignedSize = (size + 31) & 0xFFFFFFF0;
    for (Allocator::Memory *freeBlock = this->freeHead; freeBlock != this->tail; freeBlock = freeBlock->nextFree)
    {
        const int freeBlockSize = freeBlock->next ? (char *)freeBlock->next - (char *)freeBlock : 0;
        if (freeBlockSize >= alignedSize)
        {
            if (!bestFit)
            {
                bestFit = freeBlock;
            }
            else
            {
                const int bestFitBlockSize = bestFit->next ? (char *)bestFit->next - (char *)bestFit : 0;
                if (freeBlockSize < bestFitBlockSize)
                    bestFit = freeBlock;
            }
        }
    }
    if (!bestFit)
        return 0;

    const int bestFitSize = bestFit->next ? (char *)bestFit->next - (char *)bestFit : 0;
    if (bestFitSize > alignedSize)
        Allocator::Split(bestFit, alignedSize);
    bestFit->userData = userData;
    if (bestFit->prevFree)
    {
        bestFit->prevFree->nextFree = bestFit->nextFree;
    }
    else
    {
        if (this->freeHead != bestFit
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_allocator.cpp",
                206,
                0,
                "%s",
                "freeHead == bestFit"))
        {
            __debugbreak();
        }
        this->freeHead = bestFit->nextFree;
    }
    if (bestFit->nextFree)
        bestFit->nextFree->prevFree = bestFit->prevFree;
    bestFit->nextFree = 0;
    bestFit->prevFree = 0;
    const int heapSpan = (char *)this->tail - (char *)this->head;
    const int usedBytes = heapSpan - Allocator::GetFree();
    if (this->maxUsed < usedBytes)
        this->maxUsed = usedBytes;
    return &bestFit->prevFree;
}

// Decomp: CoDMPServer.c:854147
void __thiscall Allocator::Free(unsigned int *ptr)
{
    if (ptr)
    {
        Allocator::Memory *mem = (Allocator::Memory *)(ptr - 4);
        if (*(ptr - 1)
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_allocator.cpp",
                233,
                0,
                "%s",
                "!mem->IsFree()"))
        {
            __debugbreak();
        }
        if (this->freeHead == this->tail)
        {
            *ptr = 0;
            mem->nextFree = this->tail;
            this->freeHead = mem;
        }
        else
        {
            if (!mem->MakeFree()
                && !Assert_MyHandler(
                    "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_allocator.cpp",
                    250,
                    0,
                    "couldn't find a free Memory block!?!?!?"))
            {
                __debugbreak();
            }
            if (!*ptr)
                this->freeHead = mem;
        }
        while (mem->prev && mem->prev == mem->prevFree)
            mem = mem->prev;
        while (mem->nextFree && mem->nextFree == mem->next && mem->next != this->tail)
            Allocator::Merge(mem, mem->next);
    }
}

// Decomp: CoDMPServer.c:854196
unsigned int __thiscall Allocator::GetMemorySize(unsigned int *ptr)
{
    if (!ptr)
        return -1;
    if (*(ptr - 4))
        return *(ptr - 4) - (unsigned int)(ptr - 4);
    else
        return 0;
}

// Decomp: CoDMPServer.c:854210
void __thiscall Allocator::Split(Allocator::Memory *mem, int size)
{
    if (!mem->nextFree
        && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_allocator.cpp", 277, 0, "%s", "mem->IsFree()"))
    {
        __debugbreak();
    }
    const int memSize = mem->next ? (char *)mem->next - (char *)mem : 0;
    if (memSize <= size
        && !Assert_MyHandler(
            "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_allocator.cpp",
            278,
            0,
            "%s",
            "mem->GetSize() > size"))
    {
        __debugbreak();
    }
    if (memSize - size > 32)
    {
        Allocator::Memory *splitBlock = (Allocator::Memory *)((char *)mem + size);
        *(void **)((char *)&mem->userData + size) = 0;
        splitBlock->next = mem->next;
        splitBlock->prev = mem;
        splitBlock->nextFree = mem->nextFree;
        splitBlock->prevFree = mem;
        if (*(Allocator::Memory **)((char *)&mem->next + size))
            splitBlock->next->prev = splitBlock;
        if (splitBlock->nextFree)
            splitBlock->nextFree->prevFree = splitBlock;
        mem->next = splitBlock;
        mem->nextFree = splitBlock;
    }
}

// Decomp: CoDMPServer.c:854264
void __thiscall Allocator::Merge(Allocator::Memory *mem1, Allocator::Memory *mem2)
{
    if (mem2 != this->tail)
    {
        if (!mem1->nextFree
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_allocator.cpp",
                305,
                0,
                "%s",
                "mem1->IsFree()"))
        {
            __debugbreak();
        }
        if (!mem2->nextFree
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_allocator.cpp",
                306,
                0,
                "%s",
                "mem2->IsFree()"))
        {
            __debugbreak();
        }
        if (&mem1->prevFree >= &mem2->prevFree
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_allocator.cpp",
                307,
                0,
                "%s",
                "mem1->GetPtr() < mem2->GetPtr()"))
        {
            __debugbreak();
        }
        if (mem1->next != mem2
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_allocator.cpp",
                308,
                0,
                "%s",
                "mem1->next == mem2"))
        {
            __debugbreak();
        }
        mem1->next = mem2->next;
        mem1->nextFree = mem2->nextFree;
        if (mem1->next)
            mem1->next->prev = mem1;
        if (mem1->nextFree)
            mem1->nextFree->prevFree = mem1;
    }
}

// Decomp: CoDMPServer.c:854322
int __thiscall Allocator::Defrag(void (__cdecl *func)(void *), int count)
{
    while (count > 0 && this->freeHead != this->tail && this->freeHead->next != this->tail)
    {
        if (this->freeHead->next->nextFree
            && !Assert_MyHandler(
                "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_allocator.cpp",
                401,
                0,
                "%s",
                "!freeHead->next->IsFree()"))
        {
            __debugbreak();
        }
        if (this->freeHead->next->userData)
            func(this->freeHead->next->userData);
        --count;
    }
    return count;
}

// Decomp: CoDMPServer.c:854348
int __thiscall Allocator::GetFree()
{
    int freeBytes = 0;
    for (Allocator::Memory *freeBlock = this->freeHead; freeBlock; freeBlock = freeBlock->nextFree)
    {
        const int blockSize = freeBlock->next ? (char *)freeBlock->next - (char *)freeBlock : 0;
        freeBytes += blockSize;
    }
    return freeBytes;
}

// Decomp: CoDMPServer.c:854367
int __thiscall Allocator::GetLargestFree()
{
    int largestFree = 0;
    for (Allocator::Memory *freeBlock = this->freeHead; freeBlock; freeBlock = freeBlock->nextFree)
    {
        const int blockSize = freeBlock->next ? (char *)freeBlock->next - (char *)freeBlock : 0;
        if (largestFree < blockSize)
            largestFree = blockSize;
    }
    return largestFree;
}
