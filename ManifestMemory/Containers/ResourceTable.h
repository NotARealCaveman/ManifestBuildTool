#pragma once
#include <cassert>
#include <stack>

#include <ManifestPersistence/DatabaseTypes.h>

using namespace Manifest_Persistence;

namespace Manifest_Memory
{
	constexpr MFu32 RESOURCE_NOT_PRESENT{ std::numeric_limits<MFu32>::max() };

	//TODO: HANDLE ADDITIONAL/RE ALLOCATION
	template<typename Resource, typename KeyType = PrimaryKey>
	class ResourceTable
	{
	public:		
		ResourceTable() = default;
		ResourceTable(const MFsize allocation)
			:allocatedResources{ allocation }, activeResources{ 0 }
		{
			if (allocatedResources)
			{
				keys = new KeyType[allocatedResources];
				resources = new Resource[allocatedResources];
			}
		};		
		~ResourceTable()
		{
			if (keys)
			{
				delete[] keys;
				keys = nullptr;
			}
			if (resources)
			{
				delete[] resources;
				resources = nullptr;
			}
		}

		//TODO: HANDLE MAX ENTRIES
		MFu32 Insert(const KeyType resourceKey,const Resource& resource)
		{
			MFu32 result{ RESOURCE_NOT_PRESENT };

			if (resourceIndexFreeList.size())
			{
				result = resourceIndexFreeList.top();
				resourceIndexFreeList.pop();
			}
			else
				result = activeResources++;

			keys[result] = resourceKey;
			resources[result] = std::move(resource);

			return result;
		}
		MFu32 FindKeyIndex(const KeyType resourceKey) const
		{			
			KeyType* cachedKey{ std::find(&keys[0], &keys[activeResources], resourceKey) };

			if (cachedKey == &keys[activeResources])
				return RESOURCE_NOT_PRESENT;

			return std::distance(&keys[0], cachedKey);
		}
		Resource& GetResource(const MFu32 resourceIndex) const
		{
			assert(resourceIndex != RESOURCE_NOT_PRESENT);
			assert(resourceIndex < allocatedResources); 
			assert(resourceIndex >= 0);
			
			
			return resources[resourceIndex];
		}
		//removes key entry - does not clean up resource memory slot
		void Remove(const KeyType resourceKey)
		{
			MFu32 removedIndex{ FindKeyIndex(resourceKey) };
			resourceIndexFreeList.push(removedIndex);
			keys[removedIndex] = RESOURCE_NOT_PRESENT;			
		}

		KeyType const* const GetTableKeys()const { return keys; };
		Resource const* const GetTableResources()const { return resources; };

		MFsize activeResources;
	private:
		std::stack<MFu32> resourceIndexFreeList;
		KeyType* keys;
		Resource* resources;
		MFsize allocatedResources;
	};
}