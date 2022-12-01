#pragma once
#include <type_traits>
#include <concepts>

#include <ManifestGLPersistence/DatabaseTypes.h>

using namespace Manifest_Persistence;

namespace Manifest_Experimental
{	
	//unique table types
	template<typename Key, typename Value>
	concept UniqueType = !std::is_same<Key, Value>::value;
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

		//default begin & end for when Type(Key)==Type(Value) - avoids branching
		Key* begin(const bool valueSearch = false)
		{
			return keys + valueSearch * sizeof(keys);
		}
		Key* end(const bool valueSearch = false)
		{
			return  (keys + valueSearch * sizeof(keys)) + tableEntries;
		}		
		//specialized functions for begin - fewer instructions than defualts
		template<typename Iterator>
		requires KeyIterator<Iterator,Key, Value>
		Iterator* begin()
		{
			return keys;
		}
		template<typename Iterator>
		requires ValueIterator<Iterator, Key, Value>
		Iterator* begin()
		{
			return values;
		}
		//specialized functions for end - fewer instructions than defualts
		template<typename Iterator>
		requires KeyIterator<Iterator, Key, Value>
		Iterator* end()
		{
			return keys + tableEntries;
		}
		template<typename Iterator>
		requires ValueIterator<Iterator, Key, Value>
		Iterator* end()
		{
			return values + tableEntries;
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