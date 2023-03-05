#include "GEX_Name.h"

using namespace Manifest_Parser;

DDL_Structure* GEX_Name::Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap)
{	
	auto result = New<DDL_Structure, ScratchPad<DDL_Structure>>(1);
	ParseStructureHeader(partitionedStructureView, *result);
	for (const auto& subStructure : PartitionDDLSubStructures( partitionedStructureView))
		switch (ExtractStructureType(subStructure))
		{
			case DDL_BufferTypes::DDL_string:
				result->subSutructres.emplace_back(name.Build(subStructure, referenceMap));
				break;
			DEFAULT_BREAK
		}
	result->typeHeap = static_cast<void*>(this);
	MapStructureName(*result, referenceMap);
	
	return result;
}