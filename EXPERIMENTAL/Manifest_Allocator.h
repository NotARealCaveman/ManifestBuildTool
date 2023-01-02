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
#include <EXPERIMENTAL/EXPERIMENTAL_RUNTIME_DATA_STRUCTURES.h>

namespace Manifest_Memory
{
    template<class T>
    class MFAllocator
    {
    public:
        using value_type = T;

        MFAllocator() = default;        
        template<class U>
        constexpr MFAllocator(const MFAllocator <U>&) noexcept {};
        
        virtual T* allocate(const MFsize& allocation, const uintptr_t& alignment = alignof(T)) = 0;        
        virtual void deallocate(T* p, std::size_t allocation) noexcept = 0;  
    };
    template<class T, class U>
    bool operator==(const MFAllocator<T>&, const MFAllocator<U>&) { return true; }
    template<class T, class U>
    bool operator!=(const MFAllocator<T>&, const MFAllocator<U>&) { return false; }
    
    
    struct ThreadMemoryHandles
    {        
        Byte* threadHeap;//entire thread memory         
        //thread specific scratch pad allocator
        Byte* scratchPadBegin;
        Byte* scratchPadHeap;
        Byte* scratchPadEnd;        
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
    //checks argument thread id against registered thread ids and returns allocated memory handles reserved for thread
    ThreadMemoryHandles* GetThreadMemoryHandles(const std::thread::id& registeredThread = std::this_thread::get_id());

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

    //scratch pad allocator - only accessable by the executive thread
    template<typename T>
    class ScratchPad : public MFAllocator<T>
    {
        public:
            ScratchPad() {}
            template<class U>
            constexpr ScratchPad(const ScratchPad <U>&) noexcept {}
            T* allocate(const MFsize& allocation, const uintptr_t& alignment = alignof(T)) final
            {
                //get thread heap information
                auto memoryHandles = GetThreadMemoryHandles();
                //get current heap for allocation
                auto heap = memoryHandles->scratchPadHeap;       
                auto alignedHeap = AlignAllocation(heap, alignment);                
                //move heap forward
                auto allocationBytes = sizeof(T) * allocation;
                memoryHandles->scratchPadHeap = alignedHeap + allocationBytes;

                return reinterpret_cast<T*>(alignedHeap);
            };            
            //unused - when finished with scratch pad unwind is manually called. allows stl contianers to still be used
            void deallocate(T* p,[[maybe_unused]] std::size_t allocation) noexcept final {};            
            void Unwind()
            {
                auto memoryHandles = GetThreadMemoryHandles();
                memoryHandles->scratchPadHeap = memoryHandles->scratchPadBegin;
            }
    };
    template<class T, class U>
    bool operator==(const ScratchPad<T>&, const ScratchPad<U>&) { return true; }
    template<class T, class U>
    bool operator!=(const ScratchPad<T>&, const ScratchPad<U>&) { return false; }
}