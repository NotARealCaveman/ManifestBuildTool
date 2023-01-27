#pragma once
#include <type_traits>
#include <concepts>

#include <ManifestUtility/Typenames.h>


using namespace Manifest_Utility;

namespace Manifest_Persistence
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
	struct TableIterator
	{
		MFsize tableEntries{ 0 };
		MFsize tableSize;
		Key* keys;//byte 16
		Value* values;//byte 16 + sizeof(Key)	
		///TABLE BEGIN AND END FUNCTIONS
		//default begin for when Type(Key)==Type(Value) - avoids branching
		//default, .begin<>(), search is for values; .begin<false>(), search is for keys
		template <bool valueSearch = true>
			requires EqualType<Key, Value>
		inline Key* begin()
		{
			return *(&keys + valueSearch);//returns field by using size as offset
		}
		//specialized functions for begin - fewer instructions than defualt
		template<typename KIterator>
			requires KeyIterator<KIterator, Key, Value>
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