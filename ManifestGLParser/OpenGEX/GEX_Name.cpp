#include "GEX_Name.h"

using namespace Manifest_Parser;

DDL_Structure GEX_Name::Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap)
{	
	DDL_Structure result;
	ParseStructureHeader(partitionedStructure, result);
	for (const auto& subStructure : PartitionDDLSubStructures(partitionedStructure))	
		switch (ExtractStructureType(subStructure))
		{
			case DDL_BufferTypes::DDL_string:
				result.subSutructres.emplace_back(name.Build(subStructure, referenceMap));
				break;
			DEFAULT_BREAK
		}
	result.typeHeap = static_cast<void*>(this);
	MapStructureName(result, referenceMap);
	
	return result;
}