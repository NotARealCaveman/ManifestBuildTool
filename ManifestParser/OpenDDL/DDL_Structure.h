#pragma once
#include <vector>
#include <map>
#include <string>
#include <string_view>

#include <ManifestMemory/Manifest_Allocator.h>

using namespace Manifest_Memory;

namespace Manifest_Parser
{		
	constexpr uint32_t  VECTOR_RESERVATION_SIZE = 64;//to change

	typedef ScratchPadString DDL_Identifier;
	typedef ScratchPadString DDL_Name;

	struct DDL_Structure
	{
		DDL_Identifier identifier;//set when header is parsed
		DDL_Name name; //set when header is parsed
		ScratchPadVector<DDL_Structure*> subSutructres;
		void* typeHeap;
	};		
	template<typename Type>
	const Type& HeapData(const DDL_Structure& structure)
	{
		return *reinterpret_cast<Type*>(structure.typeHeap);
	}

	using  DDL_ReferenceMap = MFu32;
	/*struct DDL_ReferenceMap
	{		
		//std::map<DDL_Name,const DDL_Structure> referenceMap;
		ScratchPadUMap<DDL_Name, DDL_Structure*> referenceMap;
		uint32_t unnamedStructureCount{ 0 };
	};
	*/
	void MapStructureName(DDL_Structure& structure, DDL_ReferenceMap& referenceMap);
	
	struct DDL_Property
	{
		std::string_view key;
		std::string_view value;
	};
	typedef ScratchPadVector<DDL_Property> PropertyList;
	typedef ScratchPadVector<ScratchPadString> ReferenceList;
}