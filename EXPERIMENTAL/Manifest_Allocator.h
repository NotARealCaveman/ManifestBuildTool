#pragma once
#include <utility>
#include <vector>
#include <map>
#include <limits>

#include <cstdlib>
#include <new>
#include <limits>
#include <iostream>

#include "Manifest_Memory.h"

namespace Manifest_Memory
{
	//MANIFST ALLOCATOR WILL ULTIMATELY BE A COLAESCING BUCKET BASED ARENA ALLOCATOR
	//MLCTR V1 - SIMPLE BUMP ALLOCATOR 
	//MLCTR V2 - V1 + HOT RELEASE(EX:CLOSE FILE - UNWIND BUFFER AND MARK FREE)
	//MLCTR V3 rev 1 - async dealloc/conc alloc + v3
	//MLCTR V3 - V2 + EXECUTIVE ARENAS<--- U R HERE
	//MLCTR V4 - V3 + BUCKETS
	//MLCTR V5 - V4 + COALSECING CLEAN UP
	
    struct ProgramMemoryState
    {
        static Byte* programHeap;
    };

    template<class T>
    struct MFAllocator
    {
        using value_type = T;
                
        template<class U>
        constexpr MFAllocator(const MFAllocator <U>&) noexcept {}
        MFAllocator() = default;

        virtual [[nodiscard]] T* allocate(std::size_t n) = 0;
        virtual void deallocate(T* p, std::size_t n) noexcept = 0;  
        void report(T* p, std::size_t n, bool alloc = true) const
        {
          std::cout << '\n' << (alloc ? "Alloc: " : "Dealloc: ") << sizeof(T) * n << " bytes at " << std::hex << std::showbase << reinterpret_cast<void*>(p) << std::dec << '\n';
        }
    };
    template<class T, class U>
    bool operator==(const MFAllocator<T>&, const MFAllocator<U>&) { return true; }
    template<class T, class U>
    bool operator!=(const MFAllocator<T>&, const MFAllocator<U>&) { return false; }

    template<typename T>
    class LinearAllocator : public MFAllocator<T>
    {
        public:
        LinearAllocator() = default;

        template<class U>
        constexpr LinearAllocator(const LinearAllocator <U>&) noexcept {}
        [[nodiscard]] T* allocate(std::size_t n) final 
        {
            if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
                throw std::bad_array_new_length();

            if (auto p = static_cast<T*>(std::malloc(n * sizeof(T))))
            {
                this->report(p, n);
                return p;
            }

            throw std::bad_alloc();
        }
        void deallocate(T* p, std::size_t n) noexcept final
        {
            this->report(p, n, 0);
            std::free(p);
        }
    };
    template<class T, class U>
    bool operator==(const LinearAllocator<T>&, const LinearAllocator<U>&) { return true; }
    template<class T, class U>
    bool operator!=(const LinearAllocator<T>&, const LinearAllocator<U>&) { return false; }


	//MLCTR	 V2
	 
	//class Manifest_Allocator
	//{
	//	public:
	//		Manifest_Allocator() = default;
	//		~Manifest_Allocator();
	//		void CreateHeap(const size_t& allocationInBytes);
	//		void* Alloc(const size_t& allocationInBytes);
	//		void Dealloc(void* allocationAddress);
	//
	//		std::vector<std::pair<void*, size_t>> freelist;
	//		std::map<void*, size_t> allocationList;
	//		void* heap{ nullptr };
	//		size_t heapSizeInBytes;
	//		size_t maxHeapOffset;				
	//};	
};