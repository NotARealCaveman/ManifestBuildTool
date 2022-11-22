#pragma once
#include <vector>
#include <map>
#include <string>

namespace Manifest_Parser
{		
	constexpr uint32_t VECTOR_RESERVATION_SIZE = 64;

	typedef std::string DDL_Identifier;
	typedef std::string DDL_Name;

	struct DDL_Structure
	{
		DDL_Identifier identifier;
		DDL_Name name;		
		std::vector<DDL_Structure> subSutructres;
		void* typeHeap;
	};		
	template<typename Type>
	const Type& HeapData(const DDL_Structure& structure)
	{
		return *reinterpret_cast<Type*>(structure.typeHeap);
	}

	struct DDL_ReferenceMap
	{		
		std::map<DDL_Name,const DDL_Structure> referenceMap;
		uint32_t unnamedStructureCount{ 0 };
	};
	void MapStructureName(DDL_Structure& structure, DDL_ReferenceMap& referenceMap);
	
	struct DDL_Property
	{
		std::string key;
		std::string value;
	};
	typedef std::vector<DDL_Property> PropertyList;
	typedef std::vector<std::string> ReferenceList;
}