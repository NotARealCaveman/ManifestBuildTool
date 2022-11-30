#pragma once
#include <type_traits>

#include <ManifestGLPersistence/DatabaseTypes.h>

using namespace Manifest_Persistence;

namespace Manifest_Experimental
{
	template<typename Key, typename Value>
	struct Table
	{
		MFsize tableEntries;
		MFsize tableSize;
		Key* keys;
		Value* values;
		//default chosen if Key == Value
		template<typename Iterator>
		requires std::is_same<Iterator,Key>::value
		Iterator* begin()
		{
			return keys;
		}
		template<typename Iterator>
		requires std::is_same<Iterator, Value>::value
		Iterator* begin()
		{
			return values;
		}
		//default chosen if Key == Value
		template<typename Iterator>
		requires std::is_same<Iterator, Key>::value
		Iterator* end()
		{
			return keys + tableEntries;
		}
		template<typename Iterator>
		requires std::is_same<Iterator, Value>::value
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