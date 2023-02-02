#pragma once
#include <ManifestPersistence/DatabaseTypes.h>

using namespace Manifest_Persistence;

namespace Manifest_Memory
{
	template<typename T>
	class ResourceTable
	{
	private:
	public:
		ResourceTable(const MFsize& maxResources)
			:MAX_RESOURCES{ maxResources }, activeResources{ 0 }, keys{ new PrimaryKey[maxResources] }, resources{ new T[maxResources] } {};
		~ResourceTable()
		{
			//requires proper thread sync on shut down - will address before addressing resizing.
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