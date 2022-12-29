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
        MFu32 allocationSize;
        MFint8 padding;
    };

    struct Freelist
    {
        struct ListNode
        {
            ListNode* next{ nullptr };
            MFsize freeBytes;
        }*root{ nullptr };
    };    
    
    struct ThreadMemoryHandles
    {        
        Byte* heap;//entire thread memory 
        Freelist freelist;//freelist allocator                
        //linear allocator 
        Byte* linearBegin;
        Byte* linearHeap;
        Byte* linearEnd;
        std::vector<void*> deferredAllocations;
        //heuristics
        MFsize usedBytes;//thread wide
        MFu64 nAllocations;//thread wide        
    };
    constexpr MFu32 NUMBER_PROGRAM_EXECUTIVE_THREADS{ 2 };
    static ThreadMemoryHandles PROGRAM_EXECUTIVE_THREAD_MEMORY_HANDLES[NUMBER_PROGRAM_EXECUTIVE_THREADS];
    static std::thread::id REGISTERED_PROGRAM_EXECUTIVE_THREAD_IDS[NUMBER_PROGRAM_EXECUTIVE_THREADS];
    static std::atomic<MFu8> registerLock{ 0 };
    static MFu8 registeredExecutiveThreads{ 0 };
    void RegisterProgramExecutiveThread();

    void INIT_MEMORY_RESERVES();
        
    //returns the current heap ptr to the aligned boundary
    //moves the heap forward and places additional tracking information in heap for deallocation
    inline Byte* AlignedAllocation
    (void** heap, const MFsize& allocation, const uintptr_t& alignment) noexcept
    {
        //allocate and pad for allocation header
        //place allocation header just before requested allocation
        //   |*----.----*----.----.----...
        //ptr AllocHeadr Allocation Reqst
        //padding from allocation header->request :=0
        //AO(AH) = alignof(AllocationHeader)-1 
        //AllocationHeader::padding := *heap - (*heap + AO(AH) & ~AO(AH))
        {
        auto ptr = *heap;
        auto iptr = reinterpret_cast<uintptr_t>(ptr);
        auto headerAlignment = alignof(AllocationHeader) - 1;
        auto headerPadding = iptr - ((iptr + headerAlignment) & ~headerAlignment);
        auto header = new((Byte*)(iptr + headerPadding))AllocationHeader;
        header->allocationSize = allocation;
        header->padding = headerPadding;
        DLOG(31, "Header padding: " << header->padding);
        }


        auto ptr = *heap;
        uintptr_t alignmentMask = ~(alignment-1);
        uintptr_t iptr = reinterpret_cast<uintptr_t>(ptr);        
        //ptr already alignmed to boundary for Allocation Header
        if ((iptr & (alignof(AllocationHeader) - 1)) == 0x00)
        {            
            DLOG(31, "ptr alligned for allocation header");
            auto header = new((Byte*)ptr)AllocationHeader;
            auto* offsetPtr = (Byte*)ptr + sizeof(AllocationHeader);
            uintptr_t offsetiPtr = reinterpret_cast<uintptr_t>(offsetPtr);
            //ptr aligned for current allocation
            if ((offsetiPtr & (alignment - 1)) == 0x00)
            {
                header->allocationSize = allocation;
                header->padding = 0;
                return offsetPtr;
            }

        }
        uintptr_t alignedAllocation = ((uintptr_t)ptr + allocation  + alignment - 1) & alignmentMask;
        void* alignedPtr = (void*)alignedAllocation;
        DLOG(32, "Begin: " << ptr<<" Unaligned: " << static_cast<void*>(((Byte*)ptr + allocation)) <<" Aligned: " << alignedPtr << " Padding: " << ((uintptr_t)alignedPtr - (uintptr_t)ptr));
        return static_cast<Byte*>(alignedPtr);
    }

    template<typename T>
    class DeferredLinearAllocator : public MFAllocator<T>
    {
        public:
            DeferredLinearAllocator() {}
            template<class U>
            constexpr DeferredLinearAllocator(const DeferredLinearAllocator <U>&) noexcept {}
            T* allocate(const MFsize& allocation, const uintptr_t& alignment = alignof(T)) final
            {
                //get thread heap information
                
                //allocate memory and pad for aligne
                
                return new T;
            };

            void deallocate(T* p, std::size_t allocation) noexcept final
            {
            };
    };
    template<class T, class U>
    bool operator==(const DeferredLinearAllocator<T>&, const DeferredLinearAllocator<U>&) { return true; }
    template<class T, class U>
    bool operator!=(const DeferredLinearAllocator<T>&, const DeferredLinearAllocator<U>&) { return false; }

    template<typename T>
    class FreelistAllocator : public MFAllocator<T>
    {
    public:
        FreelistAllocator() = default;

        template<class U>
        constexpr FreelistAllocator(const FreelistAllocator <U>&) noexcept {}
        T* allocate(const MFsize& allocation, const MFsize& uintptr_t = alignof(T)) final
        {            
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