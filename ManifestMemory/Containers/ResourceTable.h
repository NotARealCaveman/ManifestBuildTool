#pragma once
#include <unordered_map>

#include <ManifestPersistence/DatabaseTypes.h>

using namespace Manifest_Persistence;

namespace Manifest_Memory
{
	template<typename T>
	class ResourceTable
	{
	private:
		std::vector<T*> Find_Each_N2(const PrimaryKey* searchKeys, const MFsize& keyCount) const
		{
			std::vector<T*> result;

			for (auto searchIndex{ 0 }; searchIndex < keyCount; ++searchIndex)
			{
				const auto& key = searchKeys[searchIndex];
				for (auto keyIndex{ 0 }; keyIndex < activeResources; ++keyIndex)
				{
					if (key == keys[keyIndex])
					{
						result.emplace_back(&resources[keyIndex]);
						break;
					}
				}
			}

			return result;
		}
		std::vector<T*> Find_Each_N2Cached(const PrimaryKey* searchKeys, const MFsize& keyCount) const
		{
			std::vector<T*> result;
			std::unordered_map<PrimaryKey, T*> encounterMap;

			for (auto searchIndex {0}; searchIndex < keyCount; ++searchIndex)
			{
				const auto& key = searchKeys[searchIndex];
				auto encountered = encounterMap.find(key);
				if (encountered != encounterMap.end())
				{
					result.emplace_back(encounterMap[key]);
					continue;
				}
				for (auto keyIndex{ 0 }; keyIndex < activeResources; ++keyIndex)
				{
					encounterMap[keys[keyIndex]] = &resources[keyIndex];
					if (key == keys[keyIndex])
					{
						result.emplace_back(&resources[keyIndex]);
						break;
					}
				}
			}
			
			return result;
		}
	public:
		ResourceTable(const MFsize& maxResources)
			:MAX_RESOURCES{ maxResources }, activeResources{ 0 }, keys{ new PrimaryKey[maxResources] }, resources{ new T[maxResources] } {};
		~ResourceTable()
		{
			//needs to be addressed if resizing is a concern - tables shouldnt be deleted during runtime
			if (0)
			{
				if (keys)
					delete[] keys;
				if (resources)
					delete[] resources;
			}
		}
		UniqueKey Insert(const PrimaryKey& databaseKey, T&& resource)
		{
			auto result = activeResources++;
			keys[result] = databaseKey;
			resources[result] = std::move(resource);
			return result;
		}
		//later will be changed into finding a set of desired keys all at once
		T& Find(const PrimaryKey& key) const
		{
			MFsize keyIndex{ 0 };			
			while (keys[keyIndex] != key)
				++keyIndex;						
			return resources[keyIndex];
		}
		void Remove(const UniqueKey& indexKey)
		{
			//tbd
		}

		PrimaryKey* keys;
		T* resources;
		const MFsize MAX_RESOURCES;
		MFsize activeResources;
	};
}