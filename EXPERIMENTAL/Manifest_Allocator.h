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

        MFAllocator() = default;        
        template<class U>
        constexpr MFAllocator(const MFAllocator <U>&) noexcept {};
        

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
    
    
    struct ThreadMemoryHandles
    {        
        Byte* threadHeap;//entire thread memory         
        //linear allocator 
        Byte* linearBegin;
        Byte* linearHeap;
        Byte* linearEnd;
        std::vector<void*> deferredLinearDeallocations;
        //heuristics
        MFsize usedBytes;//thread wide
        MFu64 nAllocations;//thread wide        
    };
    constexpr MFu32 NUMBER_PROGRAM_EXECUTIVE_THREADS{ 2 };
    static ThreadMemoryHandles PROGRAM_EXECUTIVE_THREAD_MEMORY_HANDLES[NUMBER_PROGRAM_EXECUTIVE_THREADS];
    static std::thread::id REGISTERED_PROGRAM_EXECUTIVE_THREAD_IDS[NUMBER_PROGRAM_EXECUTIVE_THREADS];
    static std::atomic<MFu8> registerLock{ 0 };
    static MFu8 registeredExecutiveThreads{ 0 };
    //stores the calling threads id for later use when requesting reserved memory handles for the threads
    void RegisterProgramExecutiveThread();
    //checks calling thread id against registered thread ids and returns allocated memory handles reserved for thread
    ThreadMemoryHandles* GetThreadMemoryHandles();

    void INIT_MEMORY_RESERVES();
        
    template<typename T,typename Alloc,typename... Args>
    inline T* New(const MFsize& count, Args&&... args)
    {        
        return new(Alloc{}.allocate(count))T{std::forward<Args>(args)... };
    }

    template<typename T,typename Alloc>
    inline void Delete(T* p, const MFsize& size = {})
    {
        Alloc{}.deallocate(p,size);
    }
        
    //returns aligned pointer to the requested alignment boundary
    inline Byte* AlignAllocation
    (void* heap, const uintptr_t& alignment) noexcept
    {        
        auto iptr = reinterpret_cast<uintptr_t>(heap);
        if ((iptr & (alignment - 1)) == 0x00)
            return (Byte*)heap;//ptr is already aligned to boundary

        auto alignmentMask = ~(alignment - 1);
        uintptr_t alignedPtr = (iptr + alignment - 1) & alignmentMask;
        return (Byte*)alignedPtr;
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
                auto memoryHandles = GetThreadMemoryHandles();
                //get current heap for allocation
                auto heap = memoryHandles->linearHeap;
                DLOG(31, "Sending heap: " << (void*)heap << " for alignment");                
                auto alignedHeap = AlignAllocation(heap, alignment);
                if (alignedHeap == heap)
                    DLOG(32, "Heap was aligned to boundary");
                else
                    DLOG(33, "Heap aligned to " << (void*)alignedHeap);
                //move heap forward
                auto allocationBytes = sizeof(T) * allocation;
                memoryHandles->linearHeap = alignedHeap + allocationBytes;
                DLOG(34, "Moving heap from: " << (void*)heap << " to: " << (void*)memoryHandles->linearHeap);
                DLOG(35, "Aligned Padding: " << ((uintptr_t)alignedHeap-(uintptr_t)heap) << " Allocated Bytes: " << allocationBytes <<" Allocated Objects: " << allocation <<" Alignment: " << alignment);
                
                return reinterpret_cast<T*>(alignedHeap);
            };
            
            void deallocate(T* p,[[maybe_unused]] std::size_t allocation) noexcept final
            {
                auto memoryHandles = GetThreadMemoryHandles();
                memoryHandles->deferredLinearDeallocations.emplace_back(p);
                DLOG(36, "Deferring deallocation of: " << (void*)p << " Current deferred deallocations: " << memoryHandles->deferredLinearDeallocations.size());
            };
    };
    template<class T, class U>
    bool operator==(const DeferredLinearAllocator<T>&, const DeferredLinearAllocator<U>&) { return true; }
    template<class T, class U>
    bool operator!=(const DeferredLinearAllocator<T>&, const DeferredLinearAllocator<U>&) { return false; }
}