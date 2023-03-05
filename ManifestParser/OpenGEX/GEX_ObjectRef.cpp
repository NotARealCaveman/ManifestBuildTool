#include "GEX_ObjectRef.h"

using namespace Manifest_Parser;

DDL_Structure* GEX_ObjectRef::Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap)
{
	ReserveReferenceContainers(*this);

	auto result = New<DDL_Structure, ScratchPad<DDL_Structure>>(1);
	ParseStructureHeader(partitionedStructureView, *result);
	for (const auto& subStructure : PartitionDDLSubStructures(partitionedStructureView))
		switch (ExtractStructureType(subStructure))
		{
			case DDL_BufferTypes::DDL_ref:
				result->subSutructres.emplace_back(DDL_Reference::Build(subStructure, referenceMap));
				//MapStructureName(result.subSutructres.back(), referenceMap);
				break;
			DEFAULT_BREAK
		}
	result->typeHeap = static_cast<void*>(this);
	MapStructureName(*result, referenceMap);
	
	return result;
}