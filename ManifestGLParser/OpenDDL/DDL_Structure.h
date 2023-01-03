#pragma once
#include <vector>
#include <map>
#include <string>

#include <EXPERIMENTAL/Manifest_Allocator.h>

using namespace Manifest_Memory;

namespace Manifest_Parser
{		
	constexpr uint32_t  VECTOR_RESERVATION_SIZE = 64;//to change

	typedef ScratchPadString DDL_Identifier;
	typedef ScratchPadString DDL_Name;

	struct DDL_Structure
	{
		DDL_Identifier identifier;
		DDL_Name name;		
		ScratchPadVector<DDL_Structure*> subSutructres;
		void* typeHeap;
	};		
	template<typename Type>
	const Type& HeapData(const DDL_Structure& structure)
	{
		return *reinterpret_cast<Type*>(structure.typeHeap);
	}

	struct DDL_ReferenceMap
	{		
		//std::map<DDL_Name,const DDL_Structure> referenceMap;
		ScratchPadUMap<DDL_Name, const DDL_Structure> referenceMap;
		uint32_t unnamedStructureCount{ 0 };
	};
	void MapStructureName(DDL_Structure& structure, DDL_ReferenceMap& referenceMap);
	
	struct DDL_Property
	{
		std::string key;
		std::string value;
	};
	typedef ScratchPadVector<DDL_Property> PropertyList;
	typedef ScratchPadVector<std::string> ReferenceList;
}