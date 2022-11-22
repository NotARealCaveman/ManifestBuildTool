#include "GEX_ObjectRef.h"

using namespace Manifest_Parser;

DDL_Structure GEX_ObjectRef::Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap)
{
	ReserveReferenceContainers(*this);

	DDL_Structure result;
	ParseStructureHeader(partitionedStructure, result);
	for (const std::string& subStructure : PartitionDDLSubStructures(partitionedStructure))	
		switch (ExtractStructureType(subStructure))
		{
			case DDL_BufferTypes::DDL_ref:
				result.subSutructres.emplace_back(DDL_Reference::Build(subStructure, referenceMap));
				//MapStructureName(result.subSutructres.back(), referenceMap);
				break;
			DEFAULT_BREAK
		}
	result.typeHeap = static_cast<void*>(this);
	MapStructureName(result, referenceMap);
	
	return result;
}