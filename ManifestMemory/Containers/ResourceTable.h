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
		//used for creating a constant resoruce table atm
		ResourceTable(const std::initializer_list<std::pair<KeyType, Resource>>& keyPairs)
			: allocatedResources(keyPairs.size()), activeResources(keyPairs.size())
		{
			if (allocatedResources)
			{
				keys = new KeyType[allocatedResources];
				resources = new Resource[allocatedResources];
				MFu32 index{ 0 };
				for (const auto [key, resource] : keyPairs)
				{
					keys[index] = key;
					resources[index] = resource;
				}
			}
		}
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
		MFu32 ReserveTableEntry()
		{
			MFu32 result{ RESOURCE_NOT_PRESENT };

			if (resourceIndexFreeList.size())
			{
				result = resourceIndexFreeList.top();
				resourceIndexFreeList.pop();
			}
			else
				result = activeResources++;

			return result;
		}

		MFu32 Insert(const KeyType resourceKey,const Resource& resource)
		{
			MFu32 result{ ReserveTableEntry() };
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
		void SetResourceKey(const PrimaryKey key, const MFu32 resourceIndex)
		{
			assert(resourceIndex != RESOURCE_NOT_PRESENT);
			assert(resourceIndex < allocatedResources);
			assert(resourceIndex >= 0);

			keys[resourceIndex] = key;
		}
		void SetResourceObject(const Resource& resource, const MFu32 resourceIndex)
		{
			assert(resourceIndex != RESOURCE_NOT_PRESENT);
			assert(resourceIndex < allocatedResources);
			assert(resourceIndex >= 0);

			resources[resourceIndex] = resource;
		}

		//removes key entry - does not clean up resource memory slot
		void Remove(const KeyType resourceKey)
		{
			MFu32 removedIndex{ FindKeyIndex(resourceKey) };

			if (removedIndex == RESOURCE_NOT_PRESENT)
				return;

			resourceIndexFreeList.push(removedIndex);
			keys[removedIndex] = RESOURCE_NOT_PRESENT;			
		}

		KeyType const* const GetTableKeys()const { return keys; };
		Resource const* const GetTableResources()const { return resources; };		

		MFsize activeResources;
		std::stack<MFu32> resourceIndexFreeList;
	private:		
		KeyType* keys;
		Resource* resources;
		MFsize allocatedResources;
	};
}