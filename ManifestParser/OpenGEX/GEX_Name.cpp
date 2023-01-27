#include "GEX_Name.h"

using namespace Manifest_Parser;

DDL_Structure* GEX_Name::Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap)
{	
	auto result = New<DDL_Structure, ScratchPad<DDL_Structure>>(1);
	ParseStructureHeader(partitionedStructure, *result);
	for (const auto& subStructure : PartitionDDLSubStructuresV2({ partitionedStructure.c_str() }))
		switch (ExtractStructureType(subStructure.c_str()))
		{
			case DDL_BufferTypes::DDL_string:
				result->subSutructres.emplace_back(name.Build(subStructure.c_str(), referenceMap));
				break;
			DEFAULT_BREAK
		}
	result->typeHeap = static_cast<void*>(this);
	MapStructureName(*result, referenceMap);
	
	return result;
}