#pragma once
#include <utility>
#include <functional>
#include <shared_mutex>
#include <vector>
#include <memory>
#include <map>
#include <algorithm>
#include <typeinfo>

#include "list.h"


/*
TO DO
INTEGRATE THE LIST INTO THE HASH MAP
LEARN HOW THE HASH MAP SHOULD WOKR INTERNALLY
THESE NEED TO REPLACE THE CURRENT HASH MAPS
ALSO CONSIDER LIST, CAN A THREAD SAFE ARRAY BE MADE TO REPLACE VECTORS
NO RESIZE IS ACTUALLY REQUIRED LIST OF ARRAYS CAN BE USEDS
*/

namespace Manifest_Memory
{
	template<typename Key, typename Value, typename Hash = std::hash<Key>>
	class ThreadedHashMap
	{
	private:
		struct BucketType
		{
		private:
			typedef std::pair<Key, Value> BucketValue;
			typedef ThreadedList<BucketValue> BucketData;

			std::shared_ptr<BucketValue> FindKey(const Key& key)
			{
				std::shared_ptr<BucketValue> bValue = data.FindFirstIf([&](const BucketValue& value) {return value.first == key; });
				return bValue;
			}			
		public:
			Value Find(const Key& key, const Value& defaultValue)const
			{
				std::shared_lock<std::shared_mutex> bucketLock(exclusiveBucket);
				const std::shared_ptr<BucketValue> bValue = FindKey(key);

				return bValue.get()->second;
			}
			void UpdateAddKey(const Key& key, const Value& value)
			{
				std::shared_lock<std::shared_mutex> bucketLock(exclusiveBucket);
				const std::shared_ptr<BucketValue> bValue = FindKey(key);				
				if (!bValue.get())
					data.PushFront(BucketValue{ key,value });
				else
					bValue.get()->second = value;
			}
			void RemoveKey(const Key& key)
			{	
				std::unique_lock<std::shared_mutex> bucketLock(exclusiveBucket);
				data.RemoveIf([&](const BucketValue& bValue) {return bValue.first == key; });
			}			

			mutable std::shared_mutex exclusiveBucket;
			BucketData data;
		};
		std::vector<std::unique_ptr<BucketType>> buckets;
		Hash hash;
		BucketType& GetBucket(const Key& key) const
		{
			const std::size_t bucketIndex = hash(key) % buckets.size();
			return *buckets[bucketIndex];
		}

		ThreadedHashMap(const ThreadedHashMap& other) = delete;
		ThreadedHashMap& operator=(const ThreadedHashMap& other) = delete;
	public:
		ThreadedHashMap(const uint32_t numBuckets = 19, const Hash& _hash = Hash()) :buckets(numBuckets), hash(_hash)
		{
			for (auto i = 0; i < numBuckets; ++i)
				buckets[i].reset(new BucketType);
		}

		Value GetValue(const Key& key, const Value& defaultValue = Value()) const
		{
			return GetBucket(key).Find(key, defaultValue);
		}
		std::map<Key,Value> GetMap() const
		{
			auto size = buckets.size();
			std::vector<std::unique_lock<std::shared_mutex>> locks;
			locks.reserve(size);
			for (int i = 0; i < size; ++i)
				locks.emplace_back(buckets[i].get()->exclusiveBucket);
			std::map<Key, Value> map;
			for (int i = 0; i < size; ++i)
			{
				buckets[i].get()->data.ForEach([&](const std::pair<Key,Value>& value) {map.insert(value); });
			}

			return map;
		}
		void UpdateAdd(const Key& key, const Value& value) 
		{
			GetBucket(key).UpdateAddKey(key, value);
		}
		void Remove(const Key& key)
		{
			GetBucket(key).RemoveKey(key);
		}
	};	
}