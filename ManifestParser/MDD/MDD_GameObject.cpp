#include "MDD_GameObject.h"

using namespace Manifest_Parser;

DDL_Structure* MDD_GameObject::Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap)
{
	auto result = New<DDL_Structure, ScratchPad<DDL_Structure>>(1);

	for (const auto& subStructure : PartitionDDLSubStructures(partitionedStructureView))
		switch (ExtractStructureType(subStructure))
		{
			case DDL_BufferTypes::DDL_int64:			
				OBJECT_TYPE.Build(subStructure, referenceMap);
				break;			
			case DDL_BufferTypes::DDL_uint64:			
				objectID.Build(subStructure, referenceMap);
				break;
			case DDL_BufferTypes::DDL_ref:			
				objectReferences.Build(subStructure, referenceMap);
				break;

			DEFAULT_BREAK
		}

	result->typeHeap = static_cast<void*>(this);
	MapStructureName(*result, referenceMap);

	return result;
}