#pragma once
#include <utility>
#include <vector>
#include <unordered_map>
#include <map>
#include <limits>
#include <thread>

#include <cstdlib>
#include <new>
#include <limits>
#include <iostream>

#include "Manifest_Memory.h"
#include <ManifestUtility/DebugLogger.h>
#include <EXPERIMENTAL/EXPERIMENTAL_RUNTIME_DATA_STRUCTURES.h>

namespace Manifest_Memory
{
    constexpr MFsize _BYTE = 1;
    constexpr MFsize SCALE = 1024;
    constexpr MFsize KIBIBYTE = _BYTE * SCALE;
    constexpr MFsize MEBIBYTE = KIBIBYTE * SCALE;
    constexpr MFsize GIBIBYTE = MEBIBYTE * SCALE;

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
        
    template<typename T,typename Alloc>
    inline T* New(const MFsize& count)
    {        
        return new(Alloc{}.allocate(count))T[count];
    }

    template<typename T, typename Alloc>
    inline T* New(const MFsize& count, const T&& value)
    {
        return new(Alloc{}.allocate(count))T[count]{ std::forward<T>(value) };
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
                //DLOG(31, "Sending: " << (void*)heap << " for alignment. allocation request: " << sizeof(T) * allocation <<" with alignment: " << alignment);
                auto alignedHeap = AlignAllocation(heap, alignment); 
                if ((void*)alignedHeap == heap);
                //DLOG(32, "Heap already aligned for boundary");
                else;
                   // DLOG(33, "Heap aligned to " <<(void*)alignedHeap);
                //move heap forward
                auto allocationBytes = sizeof(T) * allocation;
               //DLOG(34, "Moving heap from: " << (void*)heap << " to: " << (void*)(alignedHeap + allocationBytes));
                memoryHandles->scratchPadHeap = alignedHeap + allocationBytes;                
                DLOG(35, "Total bytes allocated: " << (uintptr_t)(memoryHandles->scratchPadHeap - memoryHandles->scratchPadBegin) << " Total bytes free: " << (uintptr_t)(memoryHandles->scratchPadEnd - memoryHandles->scratchPadHeap));
                return reinterpret_cast<T*>(alignedHeap);
            };            
            //unused - when finished with scratch pad unwind is manually called. allows stl contianers to still be used
            inline void deallocate(T* p, std::size_t allocation) noexcept final {};
            void Unwind()
            {
                auto memoryHandles = GetThreadMemoryHandles();
                //DLOG(36, "Unwinding: " << (uintptr_t)(memoryHandles->scratchPadHeap - memoryHandles->scratchPadBegin) <<" Bytes");
                memoryHandles->scratchPadHeap = memoryHandles->scratchPadBegin;
                memset(memoryHandles->scratchPadBegin, 0, (uintptr_t)memoryHandles->scratchPadEnd - (uintptr_t)memoryHandles->scratchPadBegin);
            }
    };
    template<typename T>
    using ScratchPadVector = std::vector<T, ScratchPad<T>>;
    template<typename Key, typename T, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>>
    using ScratchPadUMap = std::unordered_map<Key, T, std::hash<Key>, KeyEqual, ScratchPad<std::pair<const Key,T>>>;
    using ScratchPadString = std::basic_string<char, std::char_traits<char>, ScratchPad<char>>;    

    template<class T, class U>
    bool operator==(const ScratchPad<T>&, const ScratchPad<U>&) { return true; }
    template<class T, class U>
    bool operator!=(const ScratchPad<T>&, const ScratchPad<U>&) { return false; }
}