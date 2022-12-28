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

        virtual T* allocate(const MFsize& allocation, const uintptr_t& alignment = alignof(T)) = 0;
        virtual void deallocate(T* p, std::size_t allocation) noexcept = 0;
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
    private:
        inline static std::vector<std::thread::id>::const_iterator ThisThread(const std::vector<std::thread::id>::const_iterator& begin, const std::vector<std::thread::id>::const_iterator& end);
    public:
        static Byte* threadMainHeaps[NUMBER_OF_EXECUTIVE_THREADS];
        static Freelist* threadFreelists[NUMBER_OF_EXECUTIVE_THREADS];
        static std::vector<std::thread::id> allocationThreads;
        //linear allocator - currently testing
        static Byte* linearBegin[NUMBER_OF_EXECUTIVE_THREADS];
        static Byte* linearHeap[NUMBER_OF_EXECUTIVE_THREADS];
        static MFsize usedBytes[NUMBER_OF_EXECUTIVE_THREADS];
        static MFu64 nAllocations[NUMBER_OF_EXECUTIVE_THREADS];
        
        static Freelist* GetThreadFreelist(MFu32* threadIndex);
        static Byte* GetThreadLinearHeap(MFu32* threadIndex);     
    };

    void INIT_MEMORY_RESERVES();
        
    //returns the current heap ptr to the aligned boundary
    //moves the heap forward and places additional tracking information in heap for deallocation
    inline Byte* AlignedAllocation
    (const void* const ptr, const MFsize& allocation, const uintptr_t& alignment) noexcept
    {   
        uintptr_t alignmentMask = ~(alignment-1);
        uintptr_t iptr = reinterpret_cast<uintptr_t>(ptr);        
        //is ptr already alignment to boundary
        if ((iptr & (alignment - 1)) == 0x00)
        {
            DLOG(33, "Ptr already aligned to boundary: " << ptr);
            return static_cast<Byte*>(const_cast<void*>(ptr));
        }
        uintptr_t alignedAllocation = ((uintptr_t)ptr + allocation  + alignment - 1) & alignmentMask;
        void* alignedPtr = (void*)alignedAllocation;
        DLOG(32, "Begin: " << ptr<<" Unaligned: " << static_cast<void*>(((Byte*)ptr + allocation)) <<" Aligned: " << alignedPtr << " Padding: " << ((uintptr_t)alignedPtr - (uintptr_t)ptr));
        return static_cast<Byte*>(alignedPtr);
    }

    template<typename T>
    class LinearAllocator : public MFAllocator<T>
    {
        public:
            LinearAllocator() {}
            template<class U>
            constexpr LinearAllocator(const LinearAllocator <U>&) noexcept {}
            T* allocate(const MFsize& allocation, const uintptr_t& alignment = alignof(T)) final
            {
                //get thread heap information
                MFu32 threadIndex;
                auto heap = ProgramMemory::GetThreadLinearHeap(&threadIndex);
                auto begin = ProgramMemory::linearBegin[threadIndex];
                //allocate memory and pad for alignent
                T* result = reinterpret_cast<T*>(AlignedAllocation(heap, allocation,alignment));
                
                return result;
            };

            void deallocate(T* p, std::size_t allocation) noexcept final
            {
            };
    };
    template<class T, class U>
    bool operator==(const LinearAllocator<T>&, const LinearAllocator<U>&) { return true; }
    template<class T, class U>
    bool operator!=(const LinearAllocator<T>&, const LinearAllocator<U>&) { return false; }

    template<typename T>
    class FreelistAllocator : public MFAllocator<T>
    {
    public:
        FreelistAllocator() = default;

        template<class U>
        constexpr FreelistAllocator(const FreelistAllocator <U>&) noexcept {}
        T* allocate(const MFsize& allocation, const MFsize& uintptr_t = alignof(T)) final
        {
            using ListNode = Freelist::ListNode;

            MFu32 threadIndex;
            auto freelist = ProgramMemory::GetThreadFreelist(threadIndex);
            
            ListNode* prevFreeNode{ nullptr };
            ListNode* prevBestNode{ nullptr };
            ListNode* bestNode{ nullptr };
            ListNode* freeNode{ freelist->root };

            MFsize bestNodeAdjustment{ 0 };
            MFsize bestNodeTotalSize{ 0 };

            while (freeNode)
            {

            }
            return nullptr;
        }
        void deallocate(T* p, std::size_t allocation) noexcept final
        {

        }
    };

    template<class T, class U>
    bool operator==(const FreelistAllocator<T>&, const FreelistAllocator<U>&) { return true; }
    template<class T, class U>
    bool operator!=(const FreelistAllocator<T>&, const FreelistAllocator<U>&) { return false; }
}