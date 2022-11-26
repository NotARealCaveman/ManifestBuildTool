#pragma once
#include "Database_Flags.h"


namespace Manifest_Persistence
{
	typedef MFu64 PrimaryKey;//main identifier of structure table
	typedef MFu64 ForeignKey;//main idenfitier of relational structure table	
	typedef MFu64 CompositeKey;//combines two keys(bow;high bits & ward;low bits) into a single key
	typedef MFu64 UniqueKey;//represents auxillary table keys, ex: composite key bow and ward

	constexpr CompositeKey COMPOSITE_MASK{ KEY_NOT_PRESENT };

	inline static void SetCompositeBow(const MFu64& keyBow, const MFu64& bowOffset, CompositeKey& key)
	{//preserves current key ward and sets a new key bow
		key = keyBow << bowOffset | key & (COMPOSITE_MASK >> bowOffset);
	}	
	inline static void SetCompositeWard(const MFu64& keyWard, const MFu64& bowOffset, CompositeKey& key)
	{//preserves current key bow and sets a new key ward
		key = key & (COMPOSITE_MASK << bowOffset) | keyWard;
	}
	inline static UniqueKey GetCompositeBow(const CompositeKey& key, const MFu64& bowOffset)
	{//returns only the bits of the key bow as a new key which maps the bitoffset to bit 0
		return key >> bowOffset;
	};
	inline static UniqueKey GetCompositeWard(const CompositeKey& key, const MFu64& bowOffset)
	{//returns only the bits of the key ward as a new key with bits bitoffset->63=0;
		return key & (COMPOSITE_MASK >> bowOffset);
	}
}