#include "Manifest_Allocator.h"

using namespace Manifest_Memory;

void Manifest_Allocator::CreateHeap(const size_t& allocationInBytes)
{
	heap = malloc(heapSizeInBytes = allocationInBytes);
	maxHeapOffset = 0;
}

void* Manifest_Allocator::Alloc(const size_t& allocationInBytes)
{
	//check to see if an old allocation may be reused
	for (auto allocation = 0; allocation < freelist.size(); ++allocation)
	{
		if (allocationInBytes <= freelist[allocation].second  )
		{
			allocationList.insert(freelist[allocation]);
			void* result = freelist[allocation].first;			
			freelist.erase(std::remove(freelist.begin(), freelist.end(), freelist[allocation]), freelist.end());
			return result;
		}
	}
	//check to make sure the allocation can be made
	if (maxHeapOffset + allocationInBytes > heapSizeInBytes)
		return nullptr;
	//make new allocation now
	void* result = reinterpret_cast<Byte*>(heap) + maxHeapOffset;
	maxHeapOffset += allocationInBytes;
	allocationList.insert({ result,allocationInBytes });

	return result;
}

void Manifest_Allocator::Dealloc(void* allocationAddress)
{
	auto allocationRecord = allocationList.find(allocationAddress);
	if (allocationRecord == allocationList.end())
		return;
	const auto& allocation = *allocationRecord;
	freelist.emplace_back(allocation);
	allocationList.erase(allocationAddress);
}

Manifest_Allocator::~Manifest_Allocator()
{
	if (heap)
		delete heap;
}