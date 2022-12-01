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
		MFsize tableEntries;
		MFsize tableSize;
		Key* keys;
		Value* values;
		
		//default begin for when Type(Key)==Type(Value) - avoids branching
		//default, .begin<>(), search is for values; .begin<false>(), search is for keys
		template <bool valueSearch = true>
		requires EqualType<Key, Value>
		inline Key* begin()
		{			
			return &(keys + valueSearch)[0];
		}
		//specialized functions for begin - fewer instructions than defualt
		template<typename Iterator>
		requires KeyIterator<Iterator,Key, Value>
		inline Iterator* begin()
		{
			return keys;
		}
		template<typename Iterator>
		requires ValueIterator<Iterator, Key, Value>
		inline Iterator* begin()
		{
			return values;
		}
		//default end for when Type(Key)==Type(Value) - avoids branching
		//default, .begin<>(), search is for values; .begin<false>(), search is for keys
		template <bool valueSearch = true>
		requires EqualType<Key, Value>
		inline Key* end()
		{						
			return &(keys + valueSearch)[tableEntries];
		}
		//specialized functions for end - fewer instructions than defualt
		template<typename Iterator>
		requires KeyIterator<Iterator, Key, Value>
		inline Iterator* end()
		{
			return &keys[tableEntries];
		}
		template<typename Iterator>
		requires ValueIterator<Iterator, Key, Value>
		inline Iterator* end()
		{
			return &values[tableEntries];
		}
	};

	struct Xform
	{
		MFfloat data[16];
	};

	//this will be supplied inside the simulation engine Rigidbodies::worldSpaces	
	struct WorldSpaces
	{
		Table<UniqueKey,Xform> xforms;//uses the uuid of the generated runtime object 
	};

	struct GraphicResources
	{
		Table<PrimaryKey, MFu32> VAOs;
		Table<PrimaryKey, MFu32> tIDs;
	};
}