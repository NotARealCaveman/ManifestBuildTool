#pragma once
#include <type_traits>
#include <concepts>

#include <ManifestGLPersistence/DatabaseTypes.h>

using namespace Manifest_Persistence;

namespace Manifest_Experimental
{	
	//table types
	template<typename Key, typename Value>
	concept UniqueType = !std::is_same<Key, Value>::value;
	template<typename Key, typename Value>
	concept EqualType = !UniqueType<Key, Value>;
	//table iterators
	template<typename Iterator, typename Key, typename Value>	
	concept KeyIterator = std::is_same<Iterator, Key>::value && UniqueType<Key, Value>;
	template<typename Iterator, typename Key, typename Value>
	concept ValueIterator = std::is_same<Iterator, Value>::value && UniqueType<Key, Value>;

	template<typename Key, typename Value>	
	struct Table
	{
		MFsize tableEntries{0};
		MFsize tableSize;
		Key* keys;
		Value* values;	

		//TABLE FIND METHODS
		//returns the key/value if found - nullptr if not. value can be used to calculate complimentary index
		//default begin for when Type(Key)==Type(Value)
		template<typename bool valueSeach = true>
		requires EqualType<Key, Value>
		Key* find(const Key& value)
		{				
			for(auto entry = begin<std::forward<bool>(valueSeach)>(); entry < end<std::forward<bool>(valueSeach)>(); ++entry)
				if (*entry == value)
					return entry;			
			
			return nullptr;
		}
		template<typename KIterator>
		requires KeyIterator<KIterator, Key, Value>
		KIterator* find(const Key& key)
		{
			for (auto entry = 0; entry < tableEntries; ++entry)			
				if (keys[entry] == key)
					return keys + entry;			

			return nullptr;
		}
		template<typename VIterator>
		requires ValueIterator<VIterator, Key, Value>
		VIterator* find(const Value& value)
		{
			for (auto entry = 0; entry < tableEntries; ++entry)			
				if (values[entry] == value)
					return values + entry;			

			return nullptr;
		}

		///TABLE BEGIN AND END FUNCTIONS
		//default begin for when Type(Key)==Type(Value) - avoids branching
		//default, .begin<>(), search is for values; .begin<false>(), search is for keys
		template <bool valueSearch = true>
		requires EqualType<Key, Value>
		inline Key* begin()
		{			
			return *(&keys+valueSearch);
		}
		//specialized functions for begin - fewer instructions than defualt
		template<typename KIterator>
		requires KeyIterator<KIterator,Key, Value>
		inline KIterator* begin()
		{
			return keys;
		}
		template<typename VIterator>
		requires ValueIterator<VIterator, Key, Value>
		inline VIterator* begin()
		{
			return values;
		}
		//default end for when Type(Key)==Type(Value) - avoids branching
		//default, .begin<>(), search is for values; .begin<false>(), search is for keys
		template <bool valueSearch = true>
		requires EqualType<Key, Value>
		inline Key* end()
		{						
			return &(*(&keys + valueSearch))[tableEntries];
		}
		//specialized functions for end - fewer instructions than defualt
		template<typename KIterator>
		requires KeyIterator<KIterator, Key, Value>
		inline KIterator* end()
		{
			return &keys[tableEntries];
		}
		template<typename VIterator>
		requires ValueIterator<VIterator, Key, Value>
		inline VIterator* end()
		{
			return &values[tableEntries];
		}
	};
}