#pragma once
#include <utility>
#include <vector>
#include <map>
#include <limits>
#include <thread>

#include <cstdlib>
#include <new>
#include <limits>
#include <iostream>

#include "Manifest_Memory.h"
#include <ManifestGLUtility/DebugLogger.h>

namespace Manifest_Memory
{
    //MANIFST ALLOCATOR WILL ULTIMATELY BE A COLAESCING BUCKET BASED ARENA ALLOCATOR
    //MLCTR V1 - SIMPLE BUMP ALLOCATOR 
    //MLCTR V2 - V1 + HOT RELEASE(EX:CLOSE FILE - UNWIND BUFFER AND MARK FREE)
    //MLCTR V3 rev 1 - async dealloc/conc alloc + v3
    //MLCTR V3 - V2 + EXECUTIVE ARENAS<--- U R HERE
    //MLCTR V4 - V3 + BUCKETS
    //MLCTR V5 - V4 + COALSECING CLEAN UP

    template<class T>
    struct MFAllocator
    {
        using value_type = T;

        template<class U>
        constexpr MFAllocator(const MFAllocator <U>&) noexcept {}
        MFAllocator() = default;

        virtual T* allocate(const MFsize& size, const uintptr_t& alignment = alignof(T)) = 0;
        virtual void deallocate(T* p, std::size_t n) noexcept = 0;
        void report(T* p, std::size_t n, bool alloc = true) const
        {
            std::cout << this << '\n' << (alloc ? "Alloc: " : "Dealloc: ") << sizeof(T) * n << " bytes at " << std::hex << std::showbase << reinterpret_cast<void*>(p) << std::dec << '\n';
        }
    };
    template<class T, class U>
    bool operator==(const MFAllocator<T>&, const MFAllocator<U>&) { return true; }
    template<class T, class U>
    bool operator!=(const MFAllocator<T>&, const MFAllocator<U>&) { return false; }


    struct AllocationHeader
    {
        MFsize allocatedBytes;
        MFsize allocationPadding;
    };

    struct Freelist
    {
        struct ListNode
        {
            ListNode* next{ nullptr };
            MFsize freeBytes;
        }*root{ nullptr };
    };

    constexpr MFsize NUMBER_OF_EXECUTIVE_THREADS{ 2 };
    struct ProgramMemory
    {
        static Byte* threadHeaps[NUMBER_OF_EXECUTIVE_THREADS];
        static Freelist* threadFreelists[NUMBER_OF_EXECUTIVE_THREADS];
        static std::vector<std::thread::id> allocationThreads;
        //lienar allocator - currently testing
        static Byte* begin[NUMBER_OF_EXECUTIVE_THREADS];
        static Byte* heap[NUMBER_OF_EXECUTIVE_THREADS];
        static MFsize usedBytes[NUMBER_OF_EXECUTIVE_THREADS];
        static MFu64 nAllocations[NUMBER_OF_EXECUTIVE_THREADS];

        static Freelist* GetThreadFreelist();
    };

    inline std::size_t align_forward_adjustment
    (const void* const ptr, uintptr_t alignment) noexcept
    {
        DLOG(31, "ptr: " << ptr << " alignment: " << alignment);
        const auto iptr = reinterpret_cast<uintptr_t>(ptr);
        DLOG(32, "iptr: " << iptr);
        const auto aligned = (iptr - 1u + alignment) & -*reinterpret_cast<intptr_t*>(&alignment);
        DLOG(33, "1u+alignemnt: " << 1u + alignment);
        DLOG(35, "(iptr - 1u + alignment): " << (iptr - 1u + alignment));
        DLOG(36, "-alignment: " << -*reinterpret_cast<intptr_t*>(&alignment));
        DLOG(37, "(iptr - 1u + alignment) & -alignment: " << ((iptr - 1u + alignment) & -*reinterpret_cast<intptr_t*>(&alignment)));
        DLOG(38, "aligned: " << aligned);
        DLOG(39, "result: " << aligned - iptr);

        return aligned - iptr;
    }

    inline Byte* ptr_add(const void* const p, const std::uintptr_t& amount) noexcept
    {
        DLOG(31, "ptr: " << p << " size: " << amount);
        DLOG(32, "reinterpret_cast<std::uintptr_t>(p): " << reinterpret_cast<std::uintptr_t>(p));
        DLOG(33, "reinterpret_cast<std::uintptr_t>(p): " << (reinterpret_cast<std::uintptr_t>(p) + amount));
        DLOG(34, "reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(p) + amount): "
            << reinterpret_cast<Byte*>
            (reinterpret_cast<std::uintptr_t>(p) + amount));
        return reinterpret_cast<Byte*>
            (reinterpret_cast<std::uintptr_t>(p) + amount);
    }

    template<typename T>
    class LinearAllocator : public MFAllocator<T>
    {
        LinearAllocator()
        {
            heap = begin = (Byte*)malloc(1024);

        }
        T* allocate(const MFsize& size, const uintptr_t& alignment = alignof(T)) final
        {
            MFsize adjustment = align_forward_adjustment(heap, alignment);
            Byte* alignedAddr = ptr_add(alignedAddr, size);
            heap = ptr_add(alignedAddr, size);
            usedBytes = reinterpret_cast<uintptr_t>(heap) - reinterpret_cast<uintptr_t>(begin);
            ++nAllocations;

            return alignedAddr;
        };

        void deallocate(T* p, std::size_t n) noexcept final
        {
        };

        

    };

    template<typename T>
    class FreelistAllocator : public MFAllocator<T>
    {
    public:
        FreelistAllocator() = default;

        template<class U>
        constexpr FreelistAllocator(const FreelistAllocator <U>&) noexcept {}
        T* allocate(const MFsize& size, const MFsize& uintptr_t = alignof(T)) final
        {
            using ListNode = Freelist::ListNode;

            auto freelist = ProgramMemory::GetThreadFreelist()->root;

            ListNode* prevFreeNode{ nullptr };
            ListNode* prevBestNode{ nullptr };
            ListNode* bestNode{ nullptr };

            MFsize bestNodeAdjustment{ 0 };
            MFsize bestNodeTotalSize{ 0 };

            while (freelist)
            {

            }
            return nullptr;
        }
        void deallocate(T* p, std::size_t n) noexcept final
        {

        }
    };

    template<class T, class U>
    bool operator==(const FreelistAllocator<T>&, const FreelistAllocator<U>&) { return true; }
    template<class T, class U>
    bool operator!=(const FreelistAllocator<T>&, const FreelistAllocator<U>&) { return false; }
}