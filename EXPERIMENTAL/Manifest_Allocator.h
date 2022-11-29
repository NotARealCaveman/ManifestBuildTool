#pragma once
#include <utility>
#include <vector>
#include <map>

#include "Manifest_Memory.h"

namespace Manifest_Memory
{
	//MANIFST ALLOCATOR WILL ULTIMATELY BE A COLAESCING BUCKET BASED ARENA ALLOCATOR
	//MLCTR V1 - SIMPLE BUMP ALLOCATOR 
	//MLCTR V2 - V1 + HOT RELEASE(EX:CLOSE FILE - UNWIND BUFFER AND MARK FREE)
	//MLCTR V3 - V2 + EXECUTIVE ARENAS<--- U R HERE
	//MLCTR V4 - V3 + BUCKETS
	//MLCTR V5 - V4 + COALSECING CLEAN UP
	class Manifest_Allocator
	{
		public:
			Manifest_Allocator() = default;
			~Manifest_Allocator();
			void CreateHeap(const size_t& allocationInBytes);
			void* Alloc(const size_t& allocationInBytes);
			void Dealloc(void* allocationAddress);

			std::vector<std::pair<void*, size_t>> freelist;
			std::map<void*, size_t> allocationList;
			void* heap{ nullptr };
			size_t heapSizeInBytes;
			size_t maxHeapOffset;				
	};

	struct RefArray
	{
		int refs[3];
	};
};