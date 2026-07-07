#pragma once

#include <list>
#include "glass_client.h"

struct SmallAllocator // sizeof=0x18
{                                       // XREF: GlassRenderer/r
    void *memory;
    void *freeHead;
    unsigned int blockSize;
    unsigned int numBlocks;
    unsigned int numUsed;
    unsigned int maxUsed;

    void __thiscall Init(void *buffer, unsigned int bs, unsigned int nb);
    void **__thiscall Allocate(unsigned int size);
    void __thiscall Free(void **ptr, unsigned int num);
    bool __thiscall IsValidPointer(void *ptr);
};

template <typename T>
struct SmallAllocatorTemplate // sizeof=0x4
{
    using value_type = T;

    SmallAllocator *alloc;

    // Decomp: CoDMPServer.c:859036 (inlined in FixedSizeAllocator ctor list setup)
    SmallAllocatorTemplate(SmallAllocator *allocator) noexcept : alloc(allocator) {}

    template<class U>
    SmallAllocatorTemplate(const SmallAllocatorTemplate<U> &other) noexcept
        : alloc(other.alloc) {
    }

    // Decomp: CoDMPServer.c:853899 via SmallAllocator::Allocate
    T *allocate(std::size_t count)
    {
        return reinterpret_cast<T *>(alloc->Allocate(sizeof(T) * count));
    }

    // Decomp: CoDMPServer.c:853939 via SmallAllocator::Free
    void deallocate(T *ptr, std::size_t)
    {
        alloc->Free(reinterpret_cast<void **>(ptr), 1);
    }

    template<class U>
    struct rebind
    {
        using other = SmallAllocatorTemplate<U>;
    };
};

template <typename T>
struct FixedSizeAllocator
{
    // Retail MSVC2005 lists use SmallAllocatorTemplate<T**> in decomp names; modern STL requires value_type == T.
    using ListAllocator = SmallAllocatorTemplate<T>;

    void *memory;
    std::list<T, ListAllocator> used;
    std::list<T, ListAllocator> free;
    unsigned int maxUsed;

    // Decomp: CoDMPServer.c:855843 (GlassesClient::Allocate for 64-byte pool object)
    void *operator new(size_t size)
    {
        return GlassesClient::Allocate(size, "C:\\projects_pc\\cod\\codsrc\\src\\glass\\glass_renderer.cpp", 75);
    }

    // Decomp: CoDMPServer.c:855741 via GlassesClient::Free
    void operator delete(void *ptr)
    {
        GlassesClient::Free((char *)ptr);
    }

    // Decomp: CoDMPServer.c:859027
    FixedSizeAllocator(void *objectBuffer, unsigned int numObjects, SmallAllocator *allocator)
        : memory(nullptr)
        , used(ListAllocator(allocator))
        , free(ListAllocator(allocator))
        , maxUsed(0)
    {
        used.clear();
        free.clear();

        T objectPtr = static_cast<T>(objectBuffer);
        for (unsigned int objectIndex = 0; objectIndex < numObjects; ++objectIndex)
        {
            free.push_front(objectPtr);
            ++objectPtr;
        }

        memory = objectBuffer;
    }

    // Decomp: CoDMPServer.c:859068
    T Allocate()
    {
        T allocatedObject = nullptr;

        if (!free.empty())
        {
            allocatedObject = free.front();
            free.erase(free.begin());
            used.push_front(allocatedObject);
        }

        if ((unsigned int)used.size() > maxUsed)
            maxUsed = (unsigned int)used.size();

        return allocatedObject;
    }

    // Decomp: CoDMPServer.c:859520
    bool IsValidUsedPtr(T ptr)
    {
        if (!ptr)
            return true;

        for (typename std::list<T, ListAllocator>::iterator usedIt = used.begin(); usedIt != used.end(); ++usedIt)
        {
            if (*usedIt == ptr)
                return true;
        }

        return false;
    }

    // Decomp: CoDMPServer.c:859168
    void Free(T obj)
    {
        if (obj)
        {
            used.remove(obj);
            free.push_front(obj);
        }
    }

    // Decomp: CoDMPServer.c:859243
    void FreeAll()
    {
        while (!used.empty())
            Free(used.front());
    }
};

struct Allocator // sizeof=0x14
{                                       // XREF: GlassRenderer/r
    struct Memory // sizeof=0x14
    {
        Allocator::Memory *next;
        Allocator::Memory *prev;
        void *userData;
        Allocator::Memory *nextFree;
        Allocator::Memory *prevFree;

        void __thiscall Init();
        char __thiscall MakeFree();
    };
    void *buffer;
    Allocator::Memory *head;
    Allocator::Memory *tail;
    Allocator::Memory *freeHead;
    int maxUsed;


    void __thiscall Init(void *buf, int size);
    void __thiscall FreeAll();
    Memory **__thiscall Allocate(int size, void *userData);
    void __thiscall Free(unsigned int *ptr);
    unsigned int __thiscall GetMemorySize(unsigned int *ptr);
    void __thiscall Split(Memory *mem, int size);
    void __thiscall Merge(Memory *mem1, Memory *mem2);
    int __thiscall Defrag(void(__cdecl *func)(void *), int count);
    int __thiscall GetFree();
    int __thiscall GetLargestFree();
};
